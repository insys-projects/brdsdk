//********************************************************
// exam_edac.cpp
//
// Программа EXAM_EDAC для управления ЦАПами на субмодулях 
//
// (C) ИнСис, Эккоре, Март 2014
//
//=********************************************************

#include	<stdio.h>
#include	<ctype.h>
#include	<math.h>

#include	"exam_edac.h"
#include	"gipcy.h"

//
// Globals
//
BRDCHAR		g_iniFileName[MAX_PATH] = _BRDC("exam_edac.ini");
BRDCHAR		g_sFullName[MAX_PATH];

int			g_lid = -1;
S32			g_nDevNum = 0;
BRD_Handle	g_hDev[MAX_DEVICE];
S32			g_nDacNum = 0;
TDacParam	g_aDac[MAX_DAC];
BRD_Handle	g_idx[MAX_DAC];			// Рассортированные номера ЦАПов для MASTER/SLAVE
double		g_dSamplingRate;
double		g_dSignalFreq;
FILE		*g_pFileDataBin;

//
// Params from ini-file
//
BRDCHAR		g_sServiceName[256];	// без номера службы
BRDCHAR		g_sPldFileName[MAX_PATH];
int			g_nWorkMode;
int			g_nIsAlwaysWriteSdram;
SBIG		g_nSamplesPerChannel;
int			g_nIsSystemMemory;
int			g_nSdramWriteBufSize;
int			g_nIsAlwaysLoadPld;
int			g_nMasterSlave;
int			g_nDmaBufFactor;
int			g_nIsDebugMarker;
int			g_nCycle;
int			g_nQuickQuit;

int			g_nIsNew;
S32			g_nMsTimeout = 5000;

//------------------------------------------------------------------------


//------------------------------------------------------------------------

 
//=****************************** main **************************************
//=**************************************************************************
int BRDC_main( int argc, BRDCHAR *argv[] )
{
	S32			status;
	S32			devNum;

	IPC_initKeyboard();

	ParseCommandLine( argc, argv );

	BRD_displayMode(BRDdm_VISIBLE | BRDdm_CONSOLE); // режим вывода информационных сообщений : отображать все уровни на консоле
	
	status = BRD_init(_BRDC("brd.ini"), &devNum); // инициализировать библиотеку
	if(!BRD_errcmp(status, BRDerr_OK))
	{
		BRDC_printf( _BRDC("\nERROR: BARDY Initialization = 0x%X, devNum = %d\n\n"), status, devNum );
		BRDC_printf( _BRDC("Press any key for leaving program...\n"));

		IPC_getch();
		IPC_cleanupKeyboard();
		return -1;
	}

	if( 0 > ReadIniFileOption() )
	{
		IPC_getch();
		IPC_cleanupKeyboard();
		return -1;
	}

	CaptureAllDac();

	if(g_nDacNum>0)
	{
		ReadIniFileDevice();
		SetAllDac();
		SetMasterSlave();

		WorkMode();
		//if( g_nIsNew )
		//	WorkMode();
		//else
		//	WorkSrv();
	}
	else
	{
        BRDC_printf( _BRDC("\n   No Service!\n"));
	}
	ReleaseAllDac();

    BRDC_printf( _BRDC("BRD_close: OK\n"));
	status = BRD_cleanup();
    BRDC_printf( _BRDC("BRD_cleanup: OK\n"));
    BRDC_printf( _BRDC("Press any key for leaving program...\n"));

	if( g_nQuickQuit == 0 )
		IPC_getch();
	IPC_cleanupKeyboard();

	return 0;
}

//=******************** ParseCommandLine ******************
//=********************************************************
S32 ParseCommandLine(int argc, BRDCHAR *argv[])
{
	int			ii;
	BRDCHAR		*pLin, *endptr;

	for( ii=1; ii<argc; ii++ )
	{
		if( argv[ii][0] != _BRDC('-') )
		{
			BRDC_strcpy( g_iniFileName, argv[ii] );
			continue;
		}
		
		if( tolower(argv[ii][1]) == _BRDC('b') )
		{
			pLin = &argv[ii][2];
			if( argv[ii][2] == _BRDC('\0') )
			{
				ii++;
				pLin = argv[ii];
			}
			g_lid = BRDC_strtoul( pLin, &endptr, 0 );
		}
	}

	return 0;
}

//=******************* DisplayError ***********************
//=********************************************************
S32 DisplayError( S32 status, const char* func_name, const BRDCHAR* cmd_str )
{
	S32			real_status = BRD_errext(status);
	BRDCHAR		msg[255];

	switch(real_status)
	{
		case BRDerr_OK:
			BRDC_sprintf(msg, _BRDC("%hs - %s: BRDerr_OK\n"), func_name, cmd_str);
			break;
		case BRDerr_BAD_MODE:
			BRDC_sprintf(msg, _BRDC("%hs - %s: BRDerr_BAD_MODE\n"), func_name, cmd_str);
			break;
		case BRDerr_INSUFFICIENT_SERVICES:
			BRDC_sprintf(msg, _BRDC("%hs - %s: BRDerr_INSUFFICIENT_SERVICES\n"), func_name, cmd_str);
			break;
		case BRDerr_BAD_PARAMETER:
			BRDC_sprintf(msg, _BRDC("%hs - %s: BRDerr_BAD_PARAMETER\n"), func_name, cmd_str);
			break;
		case BRDerr_BUFFER_TOO_SMALL:
			BRDC_sprintf(msg, _BRDC("%hs - %s: BRDerr_BUFFER_TOO_SMALL\n"), func_name, cmd_str);
			break;
		case BRDerr_NOT_ACTION:
			BRDC_sprintf(msg, _BRDC("%hs - %s: BRDerr_NOT_ACTION\n"), func_name, cmd_str);
			break;
		default:
			BRDC_sprintf(msg, _BRDC("%hs - %s: Unknown error, status = %8X\n"), func_name, cmd_str, real_status);
			break;
	}
	BRDC_printf( msg );

	return 0;
}

//=******************** CaptureAllDac *********************
//=********************************************************
S32 CaptureAllDac( void )
{
	S32			err;
	S32			ii;
	U32			iDev;
	BRD_LidList rLidList;	
	BRD_Info	rInfo;
	U32			aLid[MAX_DEVICE];

	for( ii=0; ii<MAX_DEVICE; ii++ )
		g_hDev[ii] = -1;
	g_nDevNum = 0;

	for( ii=0; ii<MAX_DAC; ii++ )
		g_aDac[ii].handle = -1;
	g_nDacNum = 0;

	//
	// получить список LID (каждая запись соответствует устройству)
	//
	rLidList.item = MAX_DEVICE;
	rLidList.pLID = aLid;
	err = BRD_lidList( rLidList.pLID, rLidList.item, &(rLidList.itemReal) );

	for( iDev = 0; iDev < rLidList.itemReal; iDev++)
	{
		//
		// Вывести на экран информацию об устройстве
		//
		rInfo.size = sizeof(rInfo);
		BRD_getInfo( rLidList.pLID[iDev], &rInfo );
		if(rInfo.busType == BRDbus_ETHERNET)
			BRDC_printf(_BRDC("Device %2d: %s, LID = %d\n    DevID = 0x%x, RevID = 0x%x, IP %u.%u.%u.%u, Port %u, PID = %d.\n"), 
			iDev,
			rInfo.name, 
			rLidList.pLID[iDev],
			rInfo.boardType >> 16, 
			rInfo.boardType & 0xff,
			(UCHAR)rInfo.bus, 
			(UCHAR)(rInfo.bus >> 8), 
			(UCHAR)(rInfo.bus >> 16), 
			(UCHAR)(rInfo.bus >> 24),
			rInfo.dev, 
			rInfo.pid );
		else
			BRDC_printf(_BRDC("Device %2d: %s, LID = %d\n    DevID = 0x%x, RevID = 0x%x, Bus = %d, Dev = %d, Slot = %d, PID = %d.\n"), 
			iDev,
			rInfo.name, 
			rLidList.pLID[iDev],
			rInfo.boardType >> 16, 
			rInfo.boardType & 0xff, 
			rInfo.bus, 
			rInfo.dev, 
			rInfo.slot, 
			rInfo.pid );

		//
		// Открыть устройство
		//
		g_hDev[g_nDevNum] = BRD_open( rLidList.pLID[iDev], BRDopen_SHARED, NULL); // открыть устройство в монопольном режиме
		if( g_hDev[g_nDevNum ]<=0)
		{
            BRDC_printf( _BRDC("    Open Error!\n"));
			continue;
		}

		//
		// Захватить все подходящие службы на данном устройстве
		//
		U32			mode = BRDcapt_EXCLUSIVE;
		U32			nItemReal, iSrv;
		BRD_ServList srvList[MAX_SERVICE_ON_DEVICE];
		S32			len = (S32)BRDC_strlen( g_sServiceName );

		err = BRD_serviceList( g_hDev[g_nDevNum], 0, srvList, MAX_SERVICE_ON_DEVICE, &nItemReal);
		for( ii = 0; ii < (S32)nItemReal; ii++)
			BRDC_printf( _BRDC("        Srv %d:  %15s, Attr = %X.\n"),ii, srvList[ii].name, srvList[ii].attr);

		for( iSrv = 0; iSrv < nItemReal; iSrv++ )
		{			
			if( BRDC_strnicmp(  srvList[iSrv].name, g_sServiceName, len) )
				continue;
			if( (srvList[iSrv].name[len] < '0') ||
				(srvList[iSrv].name[len] > '9') ||
				(srvList[iSrv].name[len+1] != '\0') )
				continue;

            BRDC_printf( _BRDC( "   Capture %s mode: EXCLUSIVE..."), g_sServiceName );
			BRD_Handle hSrv = BRD_capture( g_hDev[g_nDevNum], 0, &mode,srvList[iSrv].name, 10000);
			if(hSrv > 0)
			{
                BRDC_printf( _BRDC("OK!\n"));

				g_aDac[g_nDacNum].handle = hSrv;
				BRDC_sprintf( g_aDac[g_nDacNum].sSection, _BRDC("device%d_%s"), iDev, srvList[iSrv].name );
				g_nDacNum++;
			}
			else
                BRDC_printf( _BRDC("Error!\n"));
		}	

		g_nDevNum++;
	}

    BRDC_printf( _BRDC( "\n" ));

	return 0;
}

//=******************** ReleaseAllDac *********************
//=********************************************************
S32 ReleaseAllDac( void )
{
	S32			err;
	S32			ii;

	for( ii = 0; ii < g_nDacNum; ii++ )
		err = BRD_release( g_aDac[ii].handle, 0 );

	for( ii = 0; ii < g_nDevNum; ii++ )
		err = BRD_close( g_hDev[ii] ); 

	return 0;
}

//=*********************** SetAllDac **********************
//=********************************************************
S32 SetAllDac( void )
{
	S32			err;
	S32			ii;
	BRD_DacCfg	rDacCfg;

	for( ii = 0; ii < g_nDacNum; ii++ )
	{
		//
		// Определить размер FIFO в байтах, количество каналов
		//
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_DAC_GETCFG, &rDacCfg );
		BRDC_printf(_BRDC("DAC Config: FIFO size = %d kBytes\n"), rDacCfg.FifoSize / 1024);

		g_aDac[ii].nFifoSizeb = rDacCfg.FifoSize;
		g_aDac[ii].chanMaxNum = rDacCfg.MaxChan;
		if( g_aDac[ii].chanMaxNum > MAX_CHAN )
			g_aDac[ii].chanMaxNum = MAX_CHAN;

		//
		// Задать параметры работы ЦАП с помощью ini-файла
		//
		BRD_IniFile rIniFile;

		lstrcpy( rIniFile.fileName, g_sFullName );
		lstrcpy( rIniFile.sectionName, g_aDac[ii].sSection );
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_DAC_READINIFILE, &rIniFile );

		//
		// получить формат данных и размер буфера
		//
		U32	format = 0;

		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_DAC_GETFORMAT, &format);
		g_aDac[ii].sampleSizeb = format ? format : sizeof(short);

		//
		// Получить источник и значение тактовой частоты можно отдельной функцией
		//
		BRD_SyncMode	rSyncMode;
		U32				nInterpFactor;

		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_DAC_GETINTERP, &nInterpFactor );
		if( !BRD_errcmp(err, BRDerr_OK) )
		{
			nInterpFactor = 1;
			DisplayError( err, __FUNCTION__, _BRDC("BRDctrl_DAC_GETINTERP") );
		}
		BRDC_printf(_BRDC("BRDctrl_DAC_GETINTERP: Kint = %d\n"), nInterpFactor );

		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_DAC_GETSYNCMODE, &rSyncMode );
		if( BRD_errcmp(err, BRDerr_OK) )
		{
			BRDC_printf(_BRDC("BRDctrl_DAC_GETSYNCMODE: source = %d, value = %.2f MHz, "), rSyncMode.clkSrc, rSyncMode.clkValue/1000000);
			if( rSyncMode.rate < 1000000 )
				BRDC_printf(_BRDC("sample rate = %.2f kHz\n"), rSyncMode.rate/1000);
			else
				BRDC_printf(_BRDC("sample rate = %.2f MHz\n"), rSyncMode.rate/1000000);
			g_aDac[ii].dSamplingRate = rSyncMode.rate;
		}
		else
			DisplayError( err, __FUNCTION__, _BRDC("BRDctrl_DAC_GETSYNCMODE") );

		//
		// получить параметры стартовой синхронизации
		//
		// команда BRDctrl_ADC_GETSTARTMODE может получать 2 разные структуры:
		// 1) BRD_StartMode размером 24 байта или
		// 2) BRD_DacStartMode размером 44 байта
		// для определения какую из них использует данная служба 
		// применяем трюк с массивом )))
		//
		U08 aStartStruct[sizeof(BRD_DacStartMode)]; // наибольшая из структур имеет размер 44 байт
		memset( aStartStruct, 0x5A, sizeof(BRD_DacStartMode) );

		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_DAC_GETSTARTMODE, &aStartStruct );
		if( BRD_errcmp(err, BRDerr_OK) )
		{
			if( aStartStruct[1+sizeof(BRD_StartMode)] == 0x5A )
			{
				BRD_StartMode *pStart = (BRD_StartMode*)aStartStruct;
				BRDC_printf( _BRDC("BRDctrl_DAC_GETSTARTMODE: start source = %d\n"), pStart->startSrc );
			}
			else
			{
				BRD_DacStartMode *pStart = (BRD_DacStartMode*)aStartStruct;;
				BRDC_printf( _BRDC("BRDctrl_DAC_GETSTARTMODE: start source base = %d, subunit = %d\n"), pStart->stndStart.startSrc, pStart->src );
			}		
		}
		else
			DisplayError(err, __FUNCTION__, _BRDC("BRDctrl_DAC_GETSTARTMODE"));

		//
		// Скорректировать режим РЕСТАРТ для WorkMode=3 и 4
		//
		if( (g_nWorkMode==3) || (g_nWorkMode==4) )
		{
			memset( aStartStruct, 0x5A, sizeof(BRD_DacStartMode) );
			err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_DAC_GETSTARTMODE, &aStartStruct );
			if( BRD_errcmp(err, BRDerr_OK) )
			{
				if( aStartStruct[1+sizeof(BRD_StartMode)] == 0x5A )
				{
					BRD_StartMode *pStart = (BRD_StartMode*)aStartStruct;
					pStart->reStartMode = 1;

				}
				else
				{
					BRD_DacStartMode *pStart = (BRD_DacStartMode*)aStartStruct;;
					pStart->stndStart.reStartMode = 1;
				}		
				err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_DAC_SETSTARTMODE, &aStartStruct );
			}
		}
		//
		// Установить значения компараторов
		//
		BRD_CmpSC rCmpSc;
		rCmpSc.src= 0;
		rCmpSc.thr[0] = g_aDac[ii].aThdac[0];
		rCmpSc.thr[1] = g_aDac[ii].aThdac[1];

		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_CMPSC_SET, &rCmpSc );

		//
		// Получить маску выбраных каналов. Определить количество выбраных каналов.
		//
		U32 chanMask = 0;

		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_DAC_GETCHANMASK, &chanMask );
		if(BRD_errcmp(err, BRDerr_OK))
			BRDC_printf(_BRDC("BRDctrl_DAC_GETCHANMASK: chan_mask = %0X\n"), chanMask);
		else
			DisplayError(err, __FUNCTION__, _BRDC("BRDctrl_DAC_GETCHANMASK"));


		S32			jj;
		S32			chanNum  = 0;

		for( jj = 0; jj < g_aDac[ii].chanMaxNum; jj++ )
			chanNum += (chanMask >> jj) & 0x1;
		
		g_aDac[ii].chanMask = chanMask;
		g_aDac[ii].chanNum  = chanNum;

		//
		// Управление коммутатором
		//
		SetSwitch( ii );
	}

	return 0;
}

//=******************** SetSwitch *****************************
//=************************************************************
S32 SetSwitch( int idxDac )
{
	BRD_Handle	handle   = g_hDev[idxDac];
	S32			switchIn = g_aDac[idxDac].switchIn;
	S32			switchOut= g_aDac[idxDac].switchOut;

	BRDC_printf(_BRDC( "SetSwitch( %d, %d )\n"), switchIn, switchOut );
	if( (switchIn<0) || (switchOut<0) )
		return 1;

	//
	// Управление коммутатором на базовом модуле
	//
	S32 err = 0;
	U32 mode = BRDcapt_SHARED;
	BRD_Handle hSrv = BRD_capture( handle, 0, &mode, _BRDC("BASEFMC0"), 10000 );

	if(hSrv <= 0)
	{
		BRDC_printf(_BRDC("WARNING: BASEFMC0 NOT capture (%X)\n"), hSrv);
		return -1;
	}
	if(mode == BRDcapt_EXCLUSIVE) BRDC_printf(_BRDC("BASEFMC Capture mode: EXCLUSIVE (%X)\n"), hSrv);
	if(mode == BRDcapt_SHARED) BRDC_printf(_BRDC("BASEFMC Capture mode: SHARED (%X)\n"), hSrv);
	if(mode == BRDcapt_SPY)	BRDC_printf(_BRDC("BASEFMC Capture mode: SPY (%X)\n"), hSrv);

	BRD_Switch sw;
	sw.out = switchOut; // на выход SwitchOut (FPGA_CLK0)
	sw.val = switchIn;  // подаем вход SwitchIn (FMC_CLK0)
	err = BRD_ctrl(hSrv, 0, BRDctrl_BASEF_SETSWITCH, &sw);

	sw.out = switchOut; // выход SwitchOut (FPGA_CLK0)
	sw.val = 1;         // включаем (1 - on)
	err = BRD_ctrl(hSrv, 0, BRDctrl_BASEF_SWITCHONOFF, &sw);

	if(BRD_errcmp(err, BRDerr_OK))
		BRDC_printf(_BRDC( "SetSwitch: %d --> %d\n"), switchIn, switchOut );

	err = BRD_release(hSrv, 0);

	return 0;
}

//******************* SetMasterSlave **********************
//*********************************************************
S32	SetMasterSlave( void )
{
	S32				err;
	int				ii, jj;
	U32				master[MAX_DAC];

	//
	// Проверить номер ЦАП, выбранного в качестве МАСТЕРА
	//
	if( g_nMasterSlave >= g_nDacNum )
	if( g_nDacNum > 1 )
	{
		BRDC_printf(_BRDC("WARNING: Wrong Master Dac No (%d). Must be less than %d.\n"), g_nMasterSlave, g_nDacNum );
		g_nMasterSlave = -1;
	}

	//
	// Рассортировать дескрипторы ЦАПов так, чтобы первым шел МАСТЕР
	//
	if( (g_nMasterSlave>=0) && (g_nDacNum>1) )
	{
		//
		// Режим MASTER/SLAVE
		//
		g_idx[0] = g_nMasterSlave;
		jj = 1;

		for( ii=0; ii<g_nDacNum; ii++ )
			if( ii != g_nMasterSlave )
				g_idx[jj++] = ii;

		for( ii=0; ii<g_nDacNum; ii++ )
			master[ii] = 0;
		master[0] = 2;
	}
	else
	{
		//
		// Режим SINGLE
		//
		for( ii=0; ii<g_nDacNum; ii++ )
			g_idx[ii] = ii;
		for( ii=0; ii<g_nDacNum; ii++ )
			master[ii] = 1;
	}

	//
	// Установить режим МАСТЕР/СЛЕЙВ или СИНГЛ для всех ЦАПов
	//
	for( ii=0; ii<g_nDacNum; ii++ )
	{
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_DAC_SETMASTER, &master[ii]);
		if( 0 <= err )
            BRDC_printf( _BRDC("BRDctrl_DAC_SETMASTER:  DAC%d  master = %d\n"), ii, master[ii] );
		else
			DisplayError( err, __FUNCTION__, _BRDC("BRDctrl_DAC_SETMASTER") );

	}

	return 0;
}



//********************** CalcSignal ***********************
//*********************************************************
S32 CalcSignal( void *pvBuf, SBIG nSamples, int idxDac, int cnt )
{
	S32				err;
	int				ii;
	double			twiddle;
	
	//
	// Заполнить сигнал из файла
	//
	if( g_pFileDataBin )
		return FillSignalFromFile( pvBuf, nSamples, idxDac, cnt );

	//
	// Если первый буфер, запомнить начальную фазу
	//
	if( cnt==0 )
		for( ii=0; ii<MAX_CHAN; ii++ )
			g_aDac[idxDac].aPhaseKee[ii] = g_aDac[idxDac].aPhase[ii];


	twiddle = PI2 * g_aDac[idxDac].dSignalFreq / g_aDac[idxDac].dSamplingRate;

	err = CalcSignalToBuf( pvBuf, nSamples, 
							g_aDac[idxDac].signalType,
							g_aDac[idxDac].sampleSizeb, 
							g_aDac[idxDac].chanMask, 
							g_aDac[idxDac].chanMaxNum,
							twiddle,
							g_aDac[idxDac].aAmpl,
							g_aDac[idxDac].aPhaseKee
							);

	return err;
}

//******************** CalcSignalToBuf ********************
//*********************************************************
S32 CalcSignalToBuf( void *pvBuf, SBIG nSamples, S32 signalType,
					 S32 sampleWidth, S32 chanMask, S32 chanMaxNum, 
					 double twiddle, double *aAmpl, double *aPhase )
{
	int			ii;
	S32			chanNum;
	S32			aChanNo[64];

	//
	// Определить количество и номера выбранных каналов
	//
	chanNum = 0;
	for( ii=0; ii<chanMaxNum; ii++ )
		if( chanMask & (1<<ii) )
			aChanNo[chanNum++] = ii;

	//
	// Сформировать сигнал в каждом выбранном канале
	// 
	for( ii=0; ii<chanNum; ii++ )
		CalcSignalToChan( pvBuf, nSamples, signalType, sampleWidth, ii, chanNum, 
		twiddle, aAmpl[aChanNo[ii]], &(aPhase[aChanNo[ii]]) );

	return 0;
}

//******************** CalcSignalToChan *******************
//*********************************************************
S32 CalcSignalToChan( void *pvBuf, SBIG nSamples, S32 signalType,
					  S32 sampleWidth, S32 chanIdx, S32 chanNum, 
					  double twiddle, double ampl, double *pPhase )
{
	SBIG		ii;
	double		phase = *pPhase;

	//
	// Проверить аргументы
	//
	if( ampl < 0.0 )
		switch( sampleWidth )
		{
			case 1: ampl = 128/2; break;
			case 2: ampl = 32767/2; break;
			case 4: ampl = 2147483647/2; break;
		}

	//
	// Отсчеты сигнала имеют размер 1 байт
	//
	if( sampleWidth == 1 )
	{
		S08		*pBuf = (S08*)pvBuf;

		if( ampl < 0.0 )
			ampl = 64.0;

		pBuf += chanIdx;
		for( ii=0; ii<nSamples; ii++ )
		{
			*pBuf  = (S08)floor(ampl * sin( phase ) + 0.5);
			if( signalType == 2 )
				*pBuf = (S08)(( *pBuf < 0.0 ) ? -ampl : +ampl);
			pBuf  += chanNum;
			phase += twiddle;
		}
		if( g_nIsDebugMarker )
		{
			pBuf = (S08*)pvBuf;
			pBuf[ chanIdx + (nSamples-1)*chanNum ] = 0x7f;
		}
	}

	//
	// Отсчеты сигнала имеют размер 2 байта
	//
	if( sampleWidth == 2 )
	{
		S16		*pBuf = (S16*)pvBuf;

		if( ampl < 0.0 )
			ampl = 16384.0;

		pBuf += chanIdx;
		for( ii=0; ii<nSamples; ii++ )
		{
			*pBuf  = (S16)floor(ampl * sin( phase ) + 0.5);
			if( signalType == 2 )
				*pBuf = (S16)(( *pBuf < 0.0 ) ? -ampl : +ampl);
			pBuf  += chanNum;
			phase += twiddle;
		}
		if( g_nIsDebugMarker )
		{
			pBuf = (S16*)pvBuf;
			pBuf[ chanIdx + (nSamples-1)*chanNum ] = 0x7fff;
		}
	}

	//
	// Отсчеты сигнала имеют размер 4 байта
	//
	if( sampleWidth == 4 )
	{
		S32		*pBuf = (S32*)pvBuf;

		if( ampl < 0.0 )
			ampl = 1024.0 * 1024.0 * 1024.0;

		pBuf += chanIdx;
		for( ii=0; ii<nSamples; ii++ )
		{
			*pBuf  = (S32)floor(ampl * sin( phase ) + 0.5);
			if( signalType == 2 )
				*pBuf = (S32)(( *pBuf < 0.0 ) ? -ampl : +ampl);
			pBuf  += chanNum;
			phase += twiddle;
		}
		if( g_nIsDebugMarker )
		{
			pBuf = (S32*)pvBuf;
			pBuf[ chanIdx + (nSamples-1)*chanNum ] = 0x7fffffff;
		}
	}

	*pPhase = fmod( phase, PI2 );

	return 0;
}

//=****************** FillSignalFromFile ******************
//=********************************************************
S32 FillSignalFromFile( void *pvBuf, SBIG nSamples, int idxDac, int cnt )
{
	SBIG		bufSizeb;

	bufSizeb = nSamples
				* g_aDac[idxDac].chanNum
				* g_aDac[idxDac].sampleSizeb;
	//
	// Если первый буфер, позиционировать файл в начало
	//
	if( cnt==0 )
		fseek( g_pFileDataBin, 0, SEEK_SET );

	memset( pvBuf, 0, bufSizeb );
	fread( pvBuf, 1, bufSizeb, g_pFileDataBin );

	return 0;
}

//=******************** CorrectOutFreq ********************
//=********************************************************
S32 CorrectOutFreq( int idxDac )
{
	double npi;			// Число периодов выходного синуса (пока не целое)
	double nnp;			// Число периодов выходного синуса (целое гарантировано)
	double delta;		// Поворачивающий фактор
	double freq;		// Скорректированная частота

	delta = PI2 * g_aDac[idxDac].dSignalFreq / g_aDac[idxDac].dSamplingRate;
	npi   = delta * (double)g_aDac[idxDac].samplesPerChannel / PI2;
	nnp   = floor(npi+0.5);
	delta = PI2 * nnp / (double)g_aDac[idxDac].samplesPerChannel;
	freq  = floor(delta * g_aDac[idxDac].dSamplingRate / PI2 + 0.5);

	g_aDac[idxDac].dSignalFreq  = freq;

	return 0;
}

//=****************** GetInifileString ********************
//=********************************************************
S32 GetInifileString( const BRDCHAR *sSectionName, const BRDCHAR *sParamName, const BRDCHAR *defValue,
							 BRDCHAR *strValue, int strSize, const BRDCHAR *sFullName )
{
	BRDCHAR		*pChar;
	int			ii;
	int			len;

	IPC_getPrivateProfileString( sSectionName, sParamName, defValue, strValue, strSize, sFullName );
 
	//
	// удалить комментарий из строки
	//
	pChar = BRDC_strchr(strValue, ';');
	if( pChar ) 
		*pChar = 0;
	pChar = BRDC_strchr(strValue, '/');
	if( pChar ) 
		if( *(pChar+1)=='/' )
			*pChar = 0;

	//
	// Удалить пробелы в конце строки
	//
	len = (int)BRDC_strlen(strValue);
	for( ii=len-1; ii>1; ii-- )
		if(strValue[ii] != ' ' && strValue[ii] != '\t')
		{
			strValue[ii+1] = 0;
			break;
		}

	return 0;
}

//=****************** ReadIniFileOption *******************
//=********************************************************
S32  ReadIniFileOption( void )
{
	S32		err;
	S32		ii;
	S32		signalFreq, signalType;
	BRDCHAR sBuffer[1024];

	err = IPC_getFullPath( g_iniFileName, g_sFullName );
	if( 0 > err )
	{
		BRDC_printf( _BRDC("ERROR: Can't find ini-file '%s'\n\n"), g_iniFileName );
		return -1;
	}

	GetInifileString( _BRDC("Option"), _BRDC("ServiceName"), _BRDC("DACNAME"), g_sServiceName, sizeof(g_sServiceName), g_sFullName );
	GetInifileString( _BRDC("Option"), _BRDC("PldFileName"), _BRDC("PLDNAME"), g_sPldFileName, sizeof(g_sPldFileName), g_sFullName );

	IPC_getPrivateProfileString( _BRDC("Option"), _BRDC("IsNew"), _BRDC("1"), sBuffer, sizeof(sBuffer), g_sFullName );
	g_nIsNew = BRDC_atoi( sBuffer );

	IPC_getPrivateProfileString( _BRDC("Option"), _BRDC("DebugMarker"), _BRDC("0"), sBuffer, sizeof(sBuffer), g_sFullName );
	g_nIsDebugMarker = BRDC_atoi( sBuffer );

	IPC_getPrivateProfileString( _BRDC("Option"), _BRDC("Cycle"), _BRDC("1"), sBuffer, sizeof(sBuffer), g_sFullName );
	g_nCycle = BRDC_atoi( sBuffer );

	IPC_getPrivateProfileString( _BRDC("Option"), _BRDC("QuickQuit"), _BRDC("0"), sBuffer, sizeof(sBuffer), g_sFullName );
	g_nQuickQuit = BRDC_atoi( sBuffer );

	IPC_getPrivateProfileString( _BRDC("Option"), _BRDC("WorkMode"), _BRDC("0"), sBuffer, sizeof(sBuffer), g_sFullName );
	g_nWorkMode = BRDC_atoi( sBuffer );

	IPC_getPrivateProfileString( _BRDC("Option"), _BRDC("IsAlwaysWriteSdram"), _BRDC("0"), sBuffer, sizeof(sBuffer), g_sFullName );
	g_nIsAlwaysWriteSdram = BRDC_atoi( sBuffer );

	IPC_getPrivateProfileString( _BRDC("Option"), _BRDC("SamplesPerChannel"), _BRDC("1024"), sBuffer, sizeof(sBuffer), g_sFullName );
	g_nSamplesPerChannel = BRDC_atoi64( sBuffer );

	IPC_getPrivateProfileString( _BRDC("Option"), _BRDC("IsSystemMemory"), _BRDC("0"), sBuffer, sizeof(sBuffer), g_sFullName );
	g_nIsSystemMemory = BRDC_atoi( sBuffer );

	IPC_getPrivateProfileString( _BRDC("Option"), _BRDC("SdramWriteBufSize"), _BRDC("4"), sBuffer, sizeof(sBuffer), g_sFullName );
	g_nSdramWriteBufSize = BRDC_atoi( sBuffer );
	g_nSdramWriteBufSize *= 1024;

	IPC_getPrivateProfileString( _BRDC("Option"), _BRDC("IsAlwaysLoadPld"), _BRDC("0"), sBuffer, sizeof(sBuffer), g_sFullName );
	g_nIsAlwaysLoadPld = BRDC_atoi( sBuffer );

	IPC_getPrivateProfileString( _BRDC("Option"), _BRDC("MasterSlave"), _BRDC("0"), sBuffer, sizeof(sBuffer), g_sFullName );
	g_nMasterSlave = BRDC_atoi( sBuffer );

	IPC_getPrivateProfileString( _BRDC("Option"), _BRDC("DmaBufFactor"), _BRDC("4096"), sBuffer, sizeof(sBuffer), g_sFullName );
	g_nDmaBufFactor = BRDC_atoi( sBuffer );

	IPC_getPrivateProfileString( _BRDC("Option"), _BRDC("SignalFreq"), _BRDC("0"), sBuffer, sizeof(sBuffer), g_sFullName );
	signalFreq = BRDC_atoi( sBuffer );

	IPC_getPrivateProfileString( _BRDC("Option"), _BRDC("SignalType"), _BRDC("0"), sBuffer, sizeof(sBuffer), g_sFullName );
	signalType = BRDC_atoi( sBuffer );

	IPC_getPrivateProfileString( _BRDC("Option"), _BRDC("SignalFile"), _BRDC(""), sBuffer, sizeof(sBuffer), g_sFullName );
	g_pFileDataBin = BRDC_fopen( sBuffer, _BRDC("rb") );
	if( '\0' != sBuffer[0] )
		if( NULL == g_pFileDataBin )
		{
            BRDC_printf( _BRDC( "\n\tERROR: Can't open file '%s'\n\n"), sBuffer );
			return -1;
		}

	for( ii=0; ii<MAX_DAC; ii++ )
	{
		g_aDac[ii].dSignalFreq = (double)signalFreq;
		g_aDac[ii].signalType  = signalType;
	}

	IPC_getPrivateProfileString( _BRDC( "Option" ), _BRDC( "TimeoutSec" ), _BRDC( "5" ), sBuffer, sizeof( sBuffer ), g_sFullName );
	g_nMsTimeout = BRDC_atoi( sBuffer ) * 1000;
	
	return 0;
}

//=****************** ReadIniFileDevice *******************
//=********************************************************
S32  ReadIniFileDevice( void )
{
	int		ii, jj;
	BRDCHAR sBuffer[128];
	BRDCHAR sParamName[128];
	double	dAmpl;

	for( ii=0; ii<g_nDacNum; ii++ )
	{
		IPC_getPrivateProfileString( g_aDac[ii].sSection, _BRDC("Ampl"), _BRDC("-1"), sBuffer, sizeof(sBuffer), g_sFullName );
		dAmpl = BRDC_atof( sBuffer );

		for( jj=0; jj<MAX_CHAN; jj++ )
		{
			BRDC_sprintf( sParamName, _BRDC("Ampl%d"), jj );
			IPC_getPrivateProfileString( g_aDac[ii].sSection, sParamName, _BRDC(""), sBuffer, sizeof(sBuffer), g_sFullName );
			g_aDac[ii].aAmpl[jj] = (*sBuffer) ? BRDC_atof(sBuffer) : dAmpl;

			BRDC_sprintf( sParamName, _BRDC("Phase%d"), jj );
			IPC_getPrivateProfileString( g_aDac[ii].sSection, sParamName, _BRDC(""), sBuffer, sizeof(sBuffer), g_sFullName );
			g_aDac[ii].aPhase[jj] = (*sBuffer) ? BRDC_atof(sBuffer) : 0.0;
			g_aDac[ii].aPhase[jj] /= 360;
			g_aDac[ii].aPhase[jj] *= PI2;
		}

		IPC_getPrivateProfileString( g_aDac[ii].sSection, _BRDC("ThresholdComp0"), _BRDC("0.0"), sBuffer, sizeof(sBuffer), g_sFullName );
		g_aDac[ii].aThdac[0] = BRDC_atof(sBuffer);
		IPC_getPrivateProfileString( g_aDac[ii].sSection, _BRDC("ThresholdComp1"), _BRDC("0.0"), sBuffer, sizeof(sBuffer), g_sFullName );
		g_aDac[ii].aThdac[1] = BRDC_atof(sBuffer);

		IPC_getPrivateProfileString( g_aDac[ii].sSection, _BRDC("SwitchIn"), _BRDC("-1"), sBuffer, sizeof(sBuffer), g_sFullName );
		g_aDac[ii].switchIn = BRDC_atoi(sBuffer);
		IPC_getPrivateProfileString( g_aDac[ii].sSection, _BRDC("SwitchOut"), _BRDC("-1"), sBuffer, sizeof(sBuffer), g_sFullName );
		g_aDac[ii].switchOut = BRDC_atoi(sBuffer);

		GetInifileString( g_aDac[ii].sSection, _BRDC("RegFileName"), _BRDC(""), 
				g_aDac[ii].sRegRwSpdFilename, sizeof(g_aDac[0].sRegRwSpdFilename), g_sFullName );
	}

	return 0;
}

//=****************** DisplayDacTraceText *****************
//=********************************************************
S32 DisplayDacTraceText( int loop, int dacIdx )
{
	BRD_TraceText		rTraceText;
	BRDCHAR				lin[200] = _BRDC("oops");
	int					err;

	rTraceText.traceId = 1;
	rTraceText.sizeb = sizeof(lin);
	rTraceText.pText = lin;

	err = BRD_ctrl( g_aDac[dacIdx].handle, 0, BRDctrl_GETTRACETEXT, &rTraceText );
	if( err<0 )
		BRDC_printf( _BRDC("[%d]\r"), loop );
	else
		BRDC_printf( _BRDC("%d  [%s]\r"), loop, lin );

	return err;
}

//
// End of file
//

