//********************************************************
//
// Example Application of BRD_xxx functions for BASEDDS service.
//
// (C) InSys by Sklyarov A. Mar, 2007
//
//	Apr, 2011 - 64-bits version
//	Jul, 2011 - replace char(TCHAR) on BRDCHAR
//
//=********************************************************


#include	<stdio.h>
#include	<ctype.h>

#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
#include	<string.h>
 #include    "gipcy.h"
#else
 #include	<windows.h>
 #include	<conio.h>
#endif

#include	"brd.h"
#include	"ctrlstrm.h"
#include	"ctrldds.h"
#include	"ctrlcmpsc.h"
#define PI2   6.2831853071795864
#define		kSpace	0x3920

const U16 PLD_CFG_TAG	= 0x0500; // tag of PLD

BRD_Handle hBaseDds;

int		g_isQuiet;
int		g_lid = -1;
BRDCHAR	g_sIniFilename[FILENAME_MAX];

void DisplayError(S32 status, const BRDCHAR* cmd_str);
void ParseCommandLine( int argc, BRDCHAR *argv[] );
S32  ReadIniFile( const BRDCHAR *pFileName );
S32	 PutDataToSrv(BRD_Handle handle, BRD_ServList* srv, int idx);
S32  SetBaseDds(BRD_Handle handle);
typedef struct 
{
	U32		RefClockSource;		// 1-internal, 0-external
	double	RefClockValue;		// (Hz)
	double  OutputFreq;			// (Hz)
	U32		LengthStb;			// (clock)
	U32		PolarityStb;		// 1-positiv, 0-negative
	U32		DelayStb;			// (clock)
	U32		PeriodStb;			// (clock)
	U32		NumberStb;			// Number out strobes
	U32		ContStb;			// 1-continuous out strobes
	U32		StartStb;			// 1- out stb, 0 - not out stb
	U32		StartSource;
	U32		StartInverse;
	double	ThresholdComp1;
	U32		CPCurrentScale;		// CP Current Scale
	U32		DividerM;			
} DDS_PARAM;
DDS_PARAM   g_DdsParam=
{
 	1,				// RefClockSource: 1-internal, 0-external
	19200000.0,		// RefClockValue (Hz)
	120000000.0,	// OutputFreq (Hz)
 	1000,			// LengthStb (clock)
	1,				// PolarityStb 1-positiv, 0-negative
	100,			// DelayStb (clock)
	10000,			// PeriodStb (clock)
	1,				// NumberStb
	0,				// ContStb
	1,				// StartStb
	0,				// StartSource;
	0,				// StartInverse;
	1.0,			// ThresholdComp1;
	1,				// CPCurrentScale
	1				// DividerM
};
//=****************** ReadIniPrepare **********************
//=********************************************************
typedef struct {BRDCHAR section[14]; int format;void *ptr; BRDCHAR title[20]; } TReadIniEntry;
static TReadIniEntry    iniEntry[] =
{
	{_BRDC("Option"), 0, (void*)&g_DdsParam.RefClockSource, _BRDC("RefClockSource") },
	{_BRDC("Option"), 2, (void*)&g_DdsParam.RefClockValue, _BRDC("RefClockValue") },
	{_BRDC("Option"), 2, (void*)&g_DdsParam.OutputFreq, _BRDC("OutputFreq") },
	{_BRDC("Option"), 0, (void*)&g_DdsParam.LengthStb, _BRDC("LengthStb") },
	{_BRDC("Option"), 0, (void*)&g_DdsParam.PolarityStb, _BRDC("PolarityStb") },
	{_BRDC("Option"), 0, (void*)&g_DdsParam.DelayStb, _BRDC("DelayStb") },
	{_BRDC("Option"), 0, (void*)&g_DdsParam.PeriodStb, _BRDC("PeriodStb") },
	{_BRDC("Option"), 0, (void*)&g_DdsParam.NumberStb, _BRDC("NumberStb") },
	{_BRDC("Option"), 0, (void*)&g_DdsParam.ContStb, _BRDC("ContStb") },
	{_BRDC("Option"), 0, (void*)&g_DdsParam.StartStb, _BRDC("StartStb") },
	{_BRDC("Option"), 0, (void*)&g_DdsParam.StartSource, _BRDC("StartSource") },
	{_BRDC("Option"), 0, (void*)&g_DdsParam.StartInverse, _BRDC("StartInverse") },
	{_BRDC("Option"), 2, (void*)&g_DdsParam.ThresholdComp1, _BRDC("ThresholdComp1") },
	{_BRDC("Option"), 0, (void*)&g_DdsParam.CPCurrentScale, _BRDC("CPCurrentScale") },
	{_BRDC("Option"), 0, (void*)&g_DdsParam.DividerM, _BRDC("DividerM") }
};


//=************************* main *************************
//=********************************************************
int BRDC_main( int argc, BRDCHAR *argv[] )
{
	S32		status;
	const BRDCHAR	*pFilename = _BRDC("/exam_bdds.ini");

#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
	IPC_initKeyboard();
#endif
	//
	// Разобрать командную строку и ИНИ-файл
	//
	ParseCommandLine( argc, argv );
	if( g_sIniFilename[0] )
		pFilename = g_sIniFilename;
	if( 0>ReadIniFile( pFilename ) )
	{
		BRDC_printf( _BRDC("\nERROR: Can't find ini file '%s'\n\n"), pFilename );
		return 1;
	}


	//
	// Начать работу с устройством
	//
	BRD_displayMode(BRDdm_VISIBLE | BRDdm_CONSOLE); // режим вывода информационных сообщений : отображать все уровни на консоле
	
	//
	// Инициализация библиотеки BARDY
	//
	S32		DevNum;
	FILE	*fIni = BRDC_fopen( _BRDC("brd_bdds.ini"), _BRDC("rt") );

	if( NULL != fIni )
	{
		// инициализировать библиотеку с помощью специального файла
		fclose(fIni);
		status = BRD_init(_BRDC("brd_bdds.ini"), &DevNum); 
	}
	else
		// инициализировать библиотеку с помощью стандартного файла
		status = BRD_init(_BRDC("brd.ini"), &DevNum);  

	// получить список LID (каждая запись соответствует устройству)
	BRD_LidList lidList;
	lidList.item = 10;
	lidList.pLID = new U32[10];
	status = BRD_lidList(lidList.pLID, lidList.item, &lidList.itemReal);

	BRD_Info	info;
	info.size = sizeof(info);
	BRD_Handle handle[10];
	U32			lid;
	for(ULONG iDev = 0; iDev < lidList.itemReal; iDev++)
	{
		lid = (g_lid != -1) ? g_lid : lidList.pLID[iDev];

		BRD_getInfo( lid, &info ); // получить информацию об устройстве
		BRDC_printf(_BRDC("Board: %s, DevID = 0x%x, RevID = 0x%x, Bus = %d, Dev = %d, Slot = %d.\n"), 
			info.name, info.boardType >> 16, info.boardType & 0xff, info.bus, info.dev, info.slot);
		//handle[iDev] = BRD_open( lid, BRDopen_EXCLUSIVE, NULL ); // открыть устройство в монопольном режиме
		handle[iDev] = BRD_open( lid, BRDopen_SHARED, NULL ); // открыть устройство в разделяемом режиме
		if(handle[iDev] > 0)
		{
			BRD_PuList PuList[5];
			U32 ItemReal;
			status = BRD_puList(handle[iDev], PuList, 5, &ItemReal); // получить список программируемых устройств
			if(ItemReal <= 5)
			{
				for(U32 j = 0; j < ItemReal; j++)
				{
					BRDC_printf(_BRDC("Programable unit %d: %s, Id = %d, Code = %x, Attr = %x.\n"),
									j, PuList[j].puDescription, PuList[j].puId, PuList[j].puCode, PuList[j].puAttr);
				}
			}
			BRD_ServList srvList[10];
			status = BRD_serviceList(handle[iDev], 0, srvList, 10, &ItemReal);
			if(ItemReal <= 10)
			{
				for(U32 j = 0; j < ItemReal; j++)
				{

					BRDC_printf(_BRDC("Service %d: %s, Attr = %X.\n"),
									j, srvList[j].name, srvList[j].attr);
				}
			}

			U32 iSrv;
			for(iSrv = 0; iSrv < ItemReal; iSrv++)
			{
				if(!BRDC_strcmp(srvList[iSrv].name, _BRDC("BASEDDS0")))
					status = PutDataToSrv(handle[iDev], &srvList[iSrv], iDev * ItemReal + iSrv);
			}
			if(hBaseDds>0)BRD_release(hBaseDds,0); // освободить службу 
			status = BRD_close(handle[iDev]); // закрыть устройство 
			BRDC_printf(_BRDC("\nBRD_close: OK\n"));
		}

		//
		// Если заказана конкретная плата, то работаем только с ней одной
		//
		if( g_lid != -1 )
			break;
	}
	delete lidList.pLID;
	status = BRD_cleanup();
	BRDC_printf(_BRDC("BRD_cleanup: OK\n"));
	BRDC_printf(_BRDC("Press any key for leaving program...\n"));
	if( 0==g_isQuiet )
	{
#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
	    IPC_getch();
		IPC_cleanupKeyboard();
#else
		_getch();
#endif
	}
	return 0;
}

//=****************** DisplayError ****************************
//=************************************************************
void DisplayError(S32 status, const BRDCHAR* cmd_str)
{
	S32	real_status = BRD_errext(status);
	BRDCHAR msg[255];
	switch(real_status)
	{ 
	case BRDerr_OK:
		BRDC_sprintf(msg, _BRDC("%s: BRDerr_OK\n"), cmd_str);
		break;
	case BRDerr_BAD_MODE:
		BRDC_sprintf(msg, _BRDC("%s: BRDerr_BAD_MODE\n"), cmd_str);
		break;
	case BRDerr_INSUFFICIENT_SERVICES:
		BRDC_sprintf(msg, _BRDC("%s: BRDerr_INSUFFICIENT_SERVICES\n"), cmd_str);
		break;
	case BRDerr_DDS_INVALID_WRRDREG:
		BRDC_sprintf(msg, _BRDC("%s: BRDerr_DDS_INVALID_WRRDREG\n"), cmd_str);
		break;
	default:
		BRDC_sprintf(msg, _BRDC("%s: Unknown error\n"), cmd_str);
		break;
	}
	BRDC_printf(msg);
}

//=****************** ParseCommandLine ************************
//=************************************************************
void	ParseCommandLine( int argc, BRDCHAR *argv[] )
{
	int			ii;
	BRDCHAR		*pLin, *endptr;

	g_sIniFilename[0] = _BRDC('\0');
	g_isQuiet = 0;

	for( ii=1; ii<argc; ii++ )
	{
		if( argv[ii][0] != _BRDC('-') )
		{
			BRDC_strcpy( g_sIniFilename, argv[ii] );
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
		if( argv[ii][1] == _BRDC('q') || argv[ii][1] == _BRDC('Q') )
			g_isQuiet = 1;
	}
}

//=************************** ReadIniFile ********************
//=************************************************************
S32    ReadIniFile( const BRDCHAR *pFileName )
{
	BRDCHAR Buffer[128];
	BRDCHAR iniFilePath[MAX_PATH];
	//DWORD retCode;

	//retCode = SearchPath( NULL, pFileName, NULL, sizeof(iniFilePath)/sizeof(BRDCHAR), iniFilePath, NULL );
	//if( 0==retCode )
	//	return -1;

#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
	IPC_getCurrentDir(iniFilePath, sizeof(iniFilePath)/sizeof(BRDCHAR));
#else
	GetCurrentDirectory(sizeof(iniFilePath)/sizeof(BRDCHAR), iniFilePath);
#endif
	BRDC_strcat(iniFilePath, pFileName);

    for(int i=0; i<sizeof(iniEntry)/sizeof(iniEntry[0]); i++ )
	{
#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
		IPC_getPrivateProfileString(iniEntry[i].section, iniEntry[i].title, _BRDC(""), Buffer, sizeof(Buffer), iniFilePath);
#else
		GetPrivateProfileString(iniEntry[i].section, iniEntry[i].title, _BRDC(""), Buffer, sizeof(Buffer), iniFilePath);
#endif
		if(BRDC_strlen(Buffer) == 0)
			return 0;

		switch(iniEntry[i].format){
			case 0:	*(int*)iniEntry[i].ptr=BRDC_atoi(Buffer);break;
			case 1:	*(float*)iniEntry[i].ptr=(float)BRDC_atof(Buffer);break;
			case 2:	*(double*)iniEntry[i].ptr=BRDC_atof(Buffer);break;
			case 3:	*(int*)iniEntry[i].ptr=BRDC_atoi(Buffer);break;
 
		}
	}
	return 0;
}
//=******************** PutDataToSrv *****************************
//=************************************************************
S32 PutDataToSrv(BRD_Handle handle, BRD_ServList* srv, int idx)
{
	S32	status;
	U32 mode = BRDcapt_EXCLUSIVE;

	hBaseDds = BRD_capture(handle, 0, &mode, srv->name, 10000);
	if(mode == BRDcapt_EXCLUSIVE) BRDC_printf(_BRDC("BASEDDS0 Capture mode: EXCLUSIVE\n"));
	if(mode == BRDcapt_SPY)	BRDC_printf(_BRDC("BASEDDS0 Capture mode: SPY\n"));
	if(hBaseDds > 0)
		status = SetBaseDds(hBaseDds); // установить параметры Dds
	return status;
}

//=******************* SetBaseDds *****************************
//=************************************************************

S32 SetBaseDds(BRD_Handle handle)
{
	S32		status;

	ULONG master=BRDims_SINGLE;
	status = BRD_ctrl(handle, 0, BRDctrl_DDS_SETMASTER, &master);
	if(BRD_errcmp(status, BRDerr_OK))
		BRDC_printf(_BRDC("BRDctrl_DDS_SETMASTER:  master= %d\n"),master);
	else
		DisplayError(status, _BRDC("BRDctrl_DDS_SETMASTER"));

	BRD_ClkMode clkMode;
	clkMode.src=g_DdsParam.RefClockSource;
	clkMode.value=g_DdsParam.RefClockValue;
	status = BRD_ctrl(handle, 0, BRDctrl_DDS_SETCLKMODE, &clkMode);
	if(BRD_errcmp(status, BRDerr_OK))
		BRDC_printf(_BRDC("BRDctrl_DDS_SETCLKMODE: src= %d value = %.2f\n"),clkMode.src,clkMode.value);
	else
		DisplayError(status, _BRDC("BRDctrl_DDS_SETCLKMODE"));	
	BRD_OutFreq  outFreq;
	outFreq.Freq=g_DdsParam.OutputFreq;
	outFreq.CPCurrentScale=g_DdsParam.CPCurrentScale;
	outFreq.DividerM=g_DdsParam.DividerM; 
	status = BRD_ctrl(handle, 0, BRDctrl_DDS_SETOUTFREQ, &outFreq);

	if(BRD_errcmp(status, BRDerr_OK))
	{
		BRDC_printf(_BRDC("BRDctrl_DDS_SETOUTFREQ: OutputFreq     = %.2f\n"),g_DdsParam.OutputFreq);
		BRDC_printf(_BRDC("                        CPCurrentScale = %d\n"), g_DdsParam.CPCurrentScale);
		BRDC_printf(_BRDC("                        DividerM = %d\n"), g_DdsParam.DividerM);
	}
	else
		DisplayError(status, _BRDC("BRDctrl_DDS_SETOUTFREQ"));
	status = BRD_ctrl(handle, 0, BRDctrl_DDS_GETOUTFREQ, &outFreq);
	//----------------------- Set StartMode ------------------------
	BRD_StartMode  startMode;
		
	startMode.startSrc= g_DdsParam.StartSource;
	startMode.startInv= g_DdsParam.StartInverse;
	startMode.trigOn= 1;
	startMode.trigStopSrc= 0;
	startMode.stopInv= 0;
	startMode.reStartMode=0;

	status = BRD_ctrl(handle, 0, BRDctrl_DDS_SETSTARTMODE, &startMode);
	if(BRD_errcmp(status, BRDerr_OK))
	{
		BRDC_printf(_BRDC("BRDctrl_DDS_SETSTARTMODE: startSrc    = %d\n"),startMode.startSrc);
		BRDC_printf(_BRDC("                          startInv    = %d\n"),startMode.startInv);
	}
	else
		DisplayError(status, _BRDC("BRDctrl_DDS_SETSTARTMODE"));

	BRD_CmpSC cmp_sc;
	cmp_sc.src=0;
	cmp_sc.thr[0]=g_DdsParam.ThresholdComp1;
	cmp_sc.thr[1]= g_DdsParam.ThresholdComp1;

	status = BRD_ctrl(handle, 0, BRDctrl_CMPSC_SET, &cmp_sc);
	if(BRD_errcmp(status, BRDerr_OK))
	{
		BRDC_printf(_BRDC("BRDctrl_CMPSC_SET: sc.src      = %d\n"),cmp_sc.src);
		BRDC_printf(_BRDC("                   cmp_sc.thr[]= %f\n"),cmp_sc.thr[0]);
                
	}else
		DisplayError(status, _BRDC("BRDctrl_CMPSC_SET"));

	BRD_StbMode stbMode;
	stbMode.Length=g_DdsParam.LengthStb;
	stbMode.Polarity=g_DdsParam.PolarityStb;
	stbMode.Delay=g_DdsParam.DelayStb;
	stbMode.Period=g_DdsParam.PeriodStb;
	stbMode.Number=g_DdsParam.NumberStb;
 	stbMode.ContStb=g_DdsParam.ContStb;


 	status = BRD_ctrl(handle, 0, BRDctrl_DDS_SETSTBMODE, &stbMode);
	if(BRD_errcmp(status, BRDerr_OK))
	{    
		BRDC_printf(_BRDC("BRDctrl_DDS_SETSTBMODE: LengthStb    = %d\n"),stbMode.Length);
		BRDC_printf(_BRDC("                        PolarityStb  = %d\n"),stbMode.Polarity);
		BRDC_printf(_BRDC("                        DelayStb     = %d\n"),stbMode.Delay);
		BRDC_printf(_BRDC("                        PeriodStb    = %d\n"),stbMode.Period);
		BRDC_printf(_BRDC("                        NumberStb    = %d\n"),stbMode.Number);
		BRDC_printf(_BRDC("                        ContStb		 = %d\n"),stbMode.ContStb);
		BRDC_printf(_BRDC("                        StartStb     = %d\n"),g_DdsParam.StartStb);
	} 
	else
		DisplayError(status, _BRDC("BRDctrl_DDS_SETSTBMODE"));

    if(g_DdsParam.StartStb==1)
	{
		BRDC_printf(_BRDC("Press any key for start !\n"));
		if( 0==g_isQuiet )
#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
		    IPC_getch();
#else
			_getch();
#endif
  		status = BRD_ctrl(handle, 0, BRDctrl_DDS_SETSTBMODE, &stbMode);
    	status = BRD_ctrl(handle, 0, BRDctrl_DDS_STARTSTROBE, NULL);
		if(BRD_errcmp(status, BRDerr_OK))
			BRDC_printf(_BRDC("BRDctrl_DDS_STARTSTROBE: StartStb\n"));
		else
			DisplayError(status, _BRDC("BRDctrl_DDS_START"));
	}

	return status;


	return 0;
}

//=************************************************************
// End of file
//
