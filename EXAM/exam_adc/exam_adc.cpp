//********************************************************
//
// Example Application for ADC service
//
// (C) InSys, 2007-2017
//
// https://github.com/andorok/DaqTools_Tutorial - Пошаговое пособие,
// иллюстрирующее пакет DaqTools и приемы работы с АЦП в том числе
//
////********************************************************

#include	<stdio.h>
#include	<fcntl.h>
#include	<math.h>

#include	"adc_ctrl.h"

#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
 #include	<string.h>
 #include	<ctype.h>
 #include	"gipcy.h"
 #include	<signal.h>
void WriteIsviParams(IPC_handle hfile, BRD_Handle hADC, unsigned long long nNumberOfBytes);
#else
 #include	<windows.h>
 #include	<conio.h>
 #include	<sys\stat.h>
 #include	<share.h>
 #include	<io.h>
void WriteIsviParams(HANDLE hfile, BRD_Handle hADC, unsigned long long nNumberOfBytes);
#endif

void ParseCommandLine(int argc, BRDCHAR *argv[]);
void DisplayHelp( );
void GetOptions();
S32 SetParamSrv(BRD_Handle handle, BRD_ServList* srv, int idx);
S32 GetAdcData(BRD_Handle hADC, unsigned long long bBufSize, unsigned long long bMemBufSize);

S32	DataFromMemWriteFile(BRD_Handle hADC, PVOID* pBuf, unsigned long long bBufSize, unsigned long long bMemBufSize, ULONG DmaOn);
void WriteDataFile(BRD_Handle hADC, int idx, PVOID* pBuf, unsigned long long nNumberOfBytes);
//S32	DataFromMemWriteFile(BRD_Handle hADC, PVOID pBuf, unsigned long long bBufSize, unsigned long long bMemBufSize, ULONG DmaOn);
//void WriteDataFile(BRD_Handle hADC, int idx, PVOID pBuf, unsigned long long nNumberOfBytes);
void WriteFlagSinc(int flg, int isNewParam, int idx);
int ReadFlagSinc(int idx);

// Options
BRDCHAR g_AdcSrvName[64]; // с номером службы
BRDCHAR g_SrvName[64];	// без номера службы
int g_AdcSrvNum; // номер службы
BRDCHAR g_pldFileName[MAX_PATH];
int g_isPldLoadAlways = 0;
int g_OnlySetParams = 0; // 1 - только установка параметров АЦП

int	g_lid = -1;
int	g_subNo = -1;		// номер службы АЦП из командной строки
ULONG g_times = 0;

int g_fileMap = 0;
ULONG g_DmaOn = 1;
ULONG g_Cycle = 0;
ULONG g_Pause = 0;
ULONG g_MemOn = 1;
ULONG g_IsWriteFile = 1;
ULONG g_IsSysMem = 0;
unsigned long long g_samplesOfChannel = 16384;
unsigned long long g_memorySamplesOfChannel = 16384;

ULONG DspMode = 0; // использовать ли ПЛИС ЦОС
ULONG g_MemAsFifo = 0;

ULONG g_AdcDrqFlag;
ULONG g_MemDrqFlag;

int g_IoDelay;
ULONG g_SwitchOutMask;	// маска включенных выходов коммутатора для FMC-модулей

BRD_Handle g_hSRV;
unsigned long long g_bBufSize;
unsigned long long g_bMemBufSize;
ULONG g_bBlkNum;
ULONG g_FileBlkNum;
ULONG g_MsTimeout;

void ContinueDaq(ULONG BlkSize, ULONG BlkNum);
void DirectFile(ULONG bufType, ULONG FileBufSize, ULONG FileBufNum, ULONG FileBlkNum);
void WriteIsviParamDirFile();
int g_DirWriteFile;
ULONG g_FileBufSize;
BRDCHAR g_dirFileName[MAX_PATH] = _BRDC("adcdir.bin");
ULONG g_adjust_mode = 0;
ULONG g_quick_quit = 0;

int g_PretrigMode = 0;
//long long g_nPreTrigSamples = 16;
long long g_nPostTrigSamples = 16384;
long long g_bPostTrigSize = 32768;
long long g_bStartEvent;
long long g_bRealSize;
ULONG g_isPassMemEnd;

int g_numChan;

BRDCHAR g_iniFileName[FILENAME_MAX] = _BRDC("//exam_adc.ini");

BRD_Info	g_info;
double g_samplRate;

int g_transRate;

void *g_pMapBuf;

U32 mode = BRDcapt_EXCLUSIVE;

S32 RegProg(BRD_Handle hAdc, int idx, int isx);
int g_regdbg;

#if defined(__IPC_LINUX__)
static int g_StopFlag = 0;
void stop_exam(int sig)
{
	fprintf(stderr, "\n");
	fprintf(stderr, "SIGNAL = %d\n", sig);
	g_StopFlag = 1;
}
#endif

//=************************* main *************************
//=********************************************************
int BRDC_main( int argc, BRDCHAR *argv[] )
{
	S32		status;

#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
	IPC_initKeyboard();
#endif
#if defined(__IPC_LINUX__)
	signal(SIGINT, stop_exam);
#endif
	// Parse Command Line
	ParseCommandLine( argc, argv );
	//if( argc > 1 )
	//{
	//	BRDC_strcpy(g_iniFileName, _BRDC("\\"));
	//	BRDC_strcat(g_iniFileName, argv[1]);
	//}
	fflush(stdout);
	setbuf(stdout, NULL);

	BRD_displayMode(BRDdm_VISIBLE | BRDdm_CONSOLE); // режим вывода информационных сообщений : отображать все уровни на консоле
	
	S32	DevNum;
	//status = BRD_initEx(BRDinit_AUTOINIT, "brd.ini", NULL, &DevNum); // инициализировать библиотеку - автоинициализация
	status = BRD_init(_BRDC("brd.ini"), &DevNum); // инициализировать библиотеку
	if(!BRD_errcmp(status, BRDerr_OK))
	{
		BRDC_printf( _BRDC("ERROR: BARDY Initialization = 0x%X\n"), status );
		BRDC_printf( _BRDC("Press any key for leaving program...\n"));
#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
	    IPC_getch();
		IPC_cleanupKeyboard();
#else
		_getch();
#endif
		return -1;
	}

	// получить список LID (каждая запись соответствует устройству)
	BRD_LidList lidList;
	lidList.item = MAX_DEV; // считаем, что устройств может быть не больше MAX_DEV
	lidList.pLID = new U32[MAX_DEV];
	status = BRD_lidList(lidList.pLID, lidList.item, &lidList.itemReal);

	g_info.size = sizeof(g_info);
	BRD_Handle handle[MAX_DEV];

	ULONG iDev = 0;
	// отображаем информацию об всех устройствах, указанных в ini-файле
	for(iDev = 0; iDev < lidList.itemReal; iDev++)
	{
		BRD_getInfo(lidList.pLID[iDev], &g_info); // получить информацию об устройстве
		if(g_info.busType == BRDbus_ETHERNET)
			BRDC_printf(_BRDC("%s, DevID = 0x%x, RevID = 0x%x, IP %u.%u.%u.%u, Port %u, PID = %d.\n"), 
				g_info.name, g_info.boardType >> 16, g_info.boardType & 0xff,
				(UCHAR)g_info.bus, (UCHAR)(g_info.bus >> 8), (UCHAR)(g_info.bus >> 16), (UCHAR)(g_info.bus >> 24),
													g_info.dev, g_info.pid);
		else
		{
			ULONG dev_id = g_info.boardType >> 16;
			if( dev_id == 0x53B1 || dev_id == 0x53B3) // FMC115cP or FMC117cP
				BRDC_printf(_BRDC("%s, DevID = 0x%x, RevID = 0x%x, Bus = %d, Dev = %d, G.adr = %d, Order = %d, PID = %d.\n"), 
								g_info.name, g_info.boardType >> 16, g_info.boardType & 0xff, g_info.bus, g_info.dev, 
										g_info.slot & 0xffff, g_info.pid >> 28, g_info.pid & 0xfffffff);
			else
				BRDC_printf(_BRDC("%s, DevID = 0x%x, RevID = 0x%x, Bus = %d, Dev = %d, Slot = %d, PID = %d.\n"), 
					g_info.name, g_info.boardType >> 16, g_info.boardType & 0xff, g_info.bus, g_info.dev, g_info.slot, g_info.pid);
		}
	}

	GetOptions();

	iDev = 0;
	// работаем либо с первым указанным в ini-файле LID-ом, либо с LID-ом, указанным в командной строке
	if(g_lid != -1)
		lidList.pLID[iDev] = g_lid;
//	for(ULONG iDev = 0; iDev < lidList.itemReal; iDev++)
//	{
//		handle[iDev] = BRD_open(lidList.pLID[iDev], BRDopen_EXCLUSIVE, NULL); // открыть устройство в монопольном режиме
		handle[iDev] = BRD_open(lidList.pLID[iDev], BRDopen_SHARED, NULL); // открыть устройство в разделяемом режиме
		if(handle[iDev] > 0)
		{
			BRD_PuList PuList[MAX_PU];
			U32 ItemReal;
			status = BRD_puList(handle[iDev], PuList, MAX_PU, &ItemReal); // получить список программируемых устройств
			if(ItemReal <= MAX_PU)
			{
				for(U32 j = 0; j < ItemReal; j++)
				{
					if(PuList[j].puCode == PLD_CFG_TAG)
					{ // Тип программируемого устройства - ПЛИС
						U32	PldState;
						status = BRD_puState(handle[iDev], PuList[j].puId, &PldState); // получить состояние ПЛИС
						BRDC_printf(_BRDC("PU%d: %s, Id = %d, Code = %x, Attr = %x, PldState = %d\n"),
										j, PuList[j].puDescription, PuList[j].puId, PuList[j].puCode, PuList[j].puAttr, PldState);
						if(PuList[j].puId == 0x100)
						{
							if(g_isPldLoadAlways || !PldState)
							{ // загрузить ПЛИС ADM
								BRDC_printf(_BRDC("BRD_puLoad: loading...\r"));
								status = BRD_puLoad(handle[iDev], PuList[j].puId, g_pldFileName, &PldState);
								if(BRD_errcmp(status, BRDerr_OK))
									BRDC_printf(_BRDC("BRD_puLoad: load is OK\n"));
								else
									if(BRD_errcmp(status, BRDerr_PLD_TEST_DATA_ERROR))
										BRDC_printf(_BRDC("BRD_puLoad: data error starting test\n"));
									else
										if(BRD_errcmp(status, BRDerr_PLD_TEST_ADDRESS_ERROR))
											BRDC_printf(_BRDC("BRD_puLoad: address error starting test\n"));
										else
											BRDC_printf(_BRDC("BRD_puLoad: error loading\n"));
							}
							if(PldState)
							{ // если ПЛИС ADM загружена
								BRDextn_PLDINFO pld_info;
								pld_info.pldId = 0x100;
								status = BRD_extension(handle[iDev], 0, BRDextn_GET_PLDINFO, &pld_info);
								if(BRD_errcmp(status, BRDerr_OK))
									BRDC_printf(_BRDC("ADM PLD: Version %d.%d, Modification %d, Build 0x%X\n"), pld_info.version >> 8, pld_info.version & 0xff, pld_info.modification, pld_info.build);
							}
						}
						else
						{  // загрузить ПЛИС ЦОС
							//status = BRD_puLoad(handle[iDev], PuList[j].puId, _BRDC("fmc106p_exp_filter.bit"), &PldState);
							//status = BRD_puLoad(handle[iDev], PuList[j].puId, _BRDC("fmc106p_dsp_base_lx240t.bit"), &PldState);
							//status = BRD_puLoad(handle[iDev], PuList[j].puId, _BRDC("fmc106p_dsp_base_sx315t_full.bit"), &PldState);
							//status = BRD_puLoad(handle[iDev], PuList[j].puId, _BRDC("ambpcd_v10_dsp_test1.mcs"), &PldState);
							//if(BRD_errcmp(status, BRDerr_OK))
							//	BRDC_printf(_BRDC("BRD_puLoad: load is OK\n"));
							//else
							//	BRDC_printf(_BRDC("BRD_puLoad: error loading\n"));
						}
					}
				}
			}

			// получаем состояние FMC-питания (если не FMC-модуль, то ошибка)
			BRDextn_FMCPOWER power;
			power.slot = 0;
			status = BRD_extension(handle[iDev], 0, BRDextn_GET_FMCPOWER, &power);
			if(BRD_errcmp(status, BRDerr_OK))
			{
				if(power.onOff)
					BRDC_printf(_BRDC("FMC Power: ON %.2f Volt\n"), power.value/100.);
				else
					BRDC_printf(_BRDC("FMC Power: OFF %.2f Volt\n"), power.value/100.);
/*
				// отключаем, а потом включаем, FMC-питание
				power.onOff = 0;
				status = BRD_extension(handle[iDev], 0, BRDextn_SET_FMCPOWER, &power);
				status = BRD_extension(handle[iDev], 0, BRDextn_GET_FMCPOWER, &power);
				if(power.onOff)
					BRDC_printf(_BRDC("FMC Power: ON %.2f Volt\n"), power.value/100.);
				else
					BRDC_printf(_BRDC("FMC Power: OFF %.2f Volt\n"), power.value/100.);

				power.onOff = 1;
				status = BRD_extension(handle[iDev], 0, BRDextn_SET_FMCPOWER, &power);
				status = BRD_extension(handle[iDev], 0, BRDextn_GET_FMCPOWER, &power);
				if(power.onOff)
					BRDC_printf(_BRDC("FMC Power: ON %.2f Volt\n"), power.value/100.);
				else
					BRDC_printf(_BRDC("FMC Power: OFF %.2f Volt\n"), power.value/100.);
*/
			}

			BRD_ServList srvList[MAX_SRV];
			status = BRD_serviceList(handle[iDev], 0, srvList, MAX_SRV, &ItemReal);
			if(ItemReal <= MAX_SRV)
			{
				for(U32 j = 0; j < ItemReal; j++)
				{
					BRDC_printf(_BRDC("Service %d: %s, Attr = %X.\n"),
									j, srvList[j].name, srvList[j].attr);
				}
				U32 iSrv;
				for(iSrv = 0; iSrv < ItemReal; iSrv++)
				{
					if(!BRDC_stricmp(srvList[iSrv].name, g_AdcSrvName))
					{
						status = SetParamSrv(handle[iDev], &srvList[iSrv], iDev * ItemReal + iSrv);
						if(BRD_errcmp(status, BRDerr_OK))
							break;
					}
				}
				if(iSrv == ItemReal) // нужную службу не нашли
				{
					BRDC_printf(_BRDC("Service %s is not found!!!\n"), g_AdcSrvName);
					mode = BRDcapt_SPY; // это для того, чтобы обойти код сбора данных
				}
			}
			else
				BRDC_printf(_BRDC("BRD_serviceList: Real Items = %d (> %d - ERROR!!!)\n"), ItemReal, MAX_SRV);
		}
	//}
	delete lidList.pLID;

	if(g_OnlySetParams)
	{
		BRDC_printf(_BRDC("Only Set Parameters Mode!!\n"));
		mode = BRDcapt_SPY;
		//BRDC_printf(_BRDC("Press any key for leaving program...\n"));
		//status = BRD_release(g_hSRV, 0);
		//status = BRD_close(handle[iDev]); // закрыть устройство 
		//status = BRD_cleanup();
	}
	{
		if(mode != BRDcapt_SPY)
			if(BRD_errcmp(status, BRDerr_OK))
			{
				if(g_DirWriteFile)
				{
					if(g_DirWriteFile == -1)
						ContinueDaq(g_FileBufSize, g_FileBlkNum);
					else
					{
						DirectFile(g_IsSysMem, g_FileBufSize, g_DirWriteFile, g_FileBlkNum);
						if(g_IsWriteFile == 1)
							WriteIsviParamDirFile();
					}
				}
				else
					GetAdcData(g_hSRV, g_bBufSize, g_bMemBufSize);
			}
	}
	status = BRD_release(g_hSRV, 0);

//	for(ULONG iDev = 0; iDev < lidList.itemReal; iDev++)
//	{
		if(handle[iDev])
		{
			status = BRD_close(handle[iDev]); // закрыть устройство 
			BRDC_printf(_BRDC("BRD_close: OK\n"));
		}
//	}
	status = BRD_cleanup();
	BRDC_printf(_BRDC("BRD_cleanup: OK\n"));
	if (!g_quick_quit)
	{
		BRDC_printf(_BRDC("Press any key for leaving program...\n"));
#if defined(__IPC_WIN__)
		IPC_getch();
#endif
#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
		IPC_cleanupKeyboard();
#else
		_getch();
#endif
	}

}

// разобрать командную строку
void ParseCommandLine(int argc, BRDCHAR *argv[])
{
	int			ii;
	BRDCHAR		*pLin, *endptr;

	//g_iniFileName[0] = _BRDC('\0');
	//g_isQuiet = 0;

	for( ii=1; ii<argc; ii++ )
	{
		if( argv[ii][0] != _BRDC('-') )
		{
			BRDC_strcpy(g_iniFileName, _BRDC("//"));
			BRDC_strcat(g_iniFileName, argv[ii] );
			continue;
		}
		
		//
		// Вывести подсказку
		//
		if( (tolower(argv[ii][1]) == _BRDC('h')) || (argv[ii][1] == _BRDC('?')) )
		{
			DisplayHelp();
			exit(1);
		}

		//
		// Указать LID базового модуля
		//
		if( tolower(argv[ii][1]) == _BRDC('b') )
		{
			pLin = &argv[ii][2];
			if( argv[ii][2] == _BRDC('\0') )
			{
				ii++;
				pLin = argv[ii];
			}
			g_lid = BRDC_strtoul( pLin, &endptr, 0 );
			BRDC_printf(_BRDC("Command line: -b %d\n"), g_lid );			
		}

		//
		// Указать номер  субмодуля
		//
		if( tolower(argv[ii][1]) == _BRDC('m') )
		{
			pLin = &argv[ii][2];
			if( argv[ii][2] == _BRDC('\0') )
			{
				ii++;
				pLin = argv[ii];
			}
			g_subNo = BRDC_strtoul( pLin, &endptr, 0 );
			BRDC_printf(_BRDC("Command line: -m %d\n"), g_subNo );			
		}

		//if( argv[ii][1] == _BRDC('q') || argv[ii][1] == _BRDC('Q') )
		//	g_isQuiet = 1;

		//
		// Режим SPY
		//
		if( tolower(argv[ii][1]) == _BRDC('s') )
		{
			g_OnlySetParams = 1;
			BRDC_printf(_BRDC("Command line: -s\n") );			
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void DisplayHelp( )
{
	BRDC_printf(_BRDC("Usage:\n") );
	BRDC_printf(_BRDC("\n") );
	BRDC_printf(_BRDC("exam_adc.exe [ini-file] [options]\n") );
	BRDC_printf(_BRDC("\n") );
	BRDC_printf(_BRDC("[ini-file] - optional name of ini-file (default name - 'exam_adc.ini'\n") );
	BRDC_printf(_BRDC("[options]  - optional mode:\n") );
	BRDC_printf(_BRDC(" -h         - this text\n") );
	BRDC_printf(_BRDC(" -b <lid>   - LID of base unit\n") );
	BRDC_printf(_BRDC(" -m <subNo> - number of subunit\n") );
	BRDC_printf(_BRDC(" -s         - SPY mode\n") );
	BRDC_printf(_BRDC("\n") );
}

// Получить параметр типа строка из ini-файла:
//    FileName - имя ini-файла (с путем, если нужно)
//    SectionName - название секции
//    ParamName - название параметра (ключа)
//    defValue - значение параметра по-умолчанию (если параметра в файле нет)
//    strValue - значение параметра
//    strSize - максимальная длина параметра
static void GetInifileString(const BRDCHAR *FileName, const BRDCHAR *SectionName, const BRDCHAR *ParamName, BRDCHAR *defValue, BRDCHAR *strValue, int strSize)
{
#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
	IPC_getPrivateProfileString(SectionName, ParamName, defValue, strValue, strSize, FileName);
#else
	GetPrivateProfileString(SectionName, ParamName, defValue, strValue, strSize, FileName);
#endif
	// удалить комментарий из строки 
	BRDCHAR* pChar = BRDC_strchr(strValue, ';'); // признак комментария или ;
	if(pChar) *pChar = 0;
	pChar = BRDC_strchr(strValue, '/');			// или //
	if( pChar ) if( *(pChar+1)=='/' )	*pChar = 0;

	// Удалить пробелы в конце строки
	int str_size = (int)BRDC_strlen(strValue);
	for(int i = str_size - 1; i > 1; i--)
		if(strValue[i] != ' ' && strValue[i] != '\t')
		{
			strValue[i+1] = 0;
			break;
		}
}

// получить параметры из ini-файла
void GetOptions()
{
	BRDCHAR Buffer[128];
	BRDCHAR iniFilePath[MAX_PATH];
	BRDCHAR* endptr;
	IPC_getCurrentDir(iniFilePath, sizeof(iniFilePath)/sizeof(BRDCHAR));
	BRDC_strcat(iniFilePath, g_iniFileName);
	//GetPrivateProfileString("Option", "AdcServiceName", "ADC212X200M", g_SrvName, sizeof(g_SrvName), iniFilePath);
	GetInifileString(iniFilePath, _BRDC("Option"), _BRDC("AdcServiceName"), _BRDC("ADC212X200M"), g_SrvName, sizeof(g_SrvName));
	IPC_getPrivateProfileString(_BRDC("Option"), _BRDC("AdcServiceNum"), _BRDC("0"), Buffer, sizeof(Buffer), iniFilePath);
	g_AdcSrvNum = BRDC_atoi(Buffer);
	if( g_subNo >= 0 )
		g_AdcSrvNum = g_subNo;
	BRDC_sprintf(g_AdcSrvName, _BRDC("%s%d"), g_SrvName, g_AdcSrvNum);

	GetInifileString(iniFilePath, _BRDC("Option"), _BRDC("PldFileName"), _BRDC("ambpcd_v10_adm212x200m.mcs"), g_pldFileName, sizeof(g_pldFileName));
	//GetPrivateProfileString("Option", "PldFileName", "ambpcd_v10_adm212x200m.mcs", g_pldFileName, sizeof(g_pldFileName), iniFilePath);

	IPC_getPrivateProfileString(_BRDC("Option"), _BRDC("isPldLoadAlways"), _BRDC("0"), Buffer, sizeof(Buffer), iniFilePath);
	g_isPldLoadAlways = BRDC_atoi(Buffer);
	IPC_getPrivateProfileString(_BRDC("Option"), _BRDC("BusMasterEnable"), _BRDC("1"), Buffer, sizeof(Buffer), iniFilePath);
	g_DmaOn = BRDC_atoi(Buffer);
	IPC_getPrivateProfileString(_BRDC("Option"), _BRDC("Cycle"), _BRDC("0"), Buffer, sizeof(Buffer), iniFilePath);
	g_Cycle = BRDC_atoi(Buffer);
	IPC_getPrivateProfileString(_BRDC("Option"), _BRDC("Pause"), _BRDC("0"), Buffer, sizeof(Buffer), iniFilePath);
	g_Pause = BRDC_atoi(Buffer);
	IPC_getPrivateProfileString(_BRDC("Option"), _BRDC("DaqIntoMemory"), _BRDC("0"), Buffer, sizeof(Buffer), iniFilePath);
	g_MemOn = BRDC_atoi(Buffer);
	IPC_getPrivateProfileString(_BRDC("Option"), _BRDC("IsWriteFile"), _BRDC("0"), Buffer, sizeof(Buffer), iniFilePath);
	g_IsWriteFile = BRDC_atoi(Buffer);
	IPC_getPrivateProfileString(_BRDC("Option"), _BRDC("SamplesPerChannel"), _BRDC("16384"), Buffer, sizeof(Buffer), iniFilePath);
	g_samplesOfChannel = BRDC_atoi64(Buffer);
	IPC_getPrivateProfileString(_BRDC("Option"), _BRDC("MemSamplesPerChan"), _BRDC("16384"), Buffer, sizeof(Buffer), iniFilePath);
	g_memorySamplesOfChannel = BRDC_atoi64(Buffer);
	IPC_getPrivateProfileString(_BRDC("Option"), _BRDC("IsSystemMemory"), _BRDC("0"), Buffer, sizeof(Buffer), iniFilePath);
	g_IsSysMem = BRDC_atoi(Buffer);
	if(g_IsSysMem == 1 && g_info.busType == BRDbus_ETHERNET)
	{
		BRDC_printf(_BRDC("Ethernet: IsSystemMemory=0 or IsSystemMemory=2 (NOT EQUAL 1)!!!\n"));		
		g_IsSysMem = 0;
	}
	IPC_getPrivateProfileString(_BRDC("Option"), _BRDC("DirFileBufSize"), _BRDC("64"), Buffer, sizeof(Buffer), iniFilePath); // KBytes
	g_FileBufSize = BRDC_atoi(Buffer) * 1024;
	IPC_getPrivateProfileString(_BRDC("Option"), _BRDC("DirNumBufWrite"), _BRDC("0"), Buffer, sizeof(Buffer), iniFilePath); // KBytes
	g_DirWriteFile = BRDC_atoi(Buffer);
	IPC_getPrivateProfileString(_BRDC("Option"), _BRDC("DirNumBlock"), _BRDC("2"), Buffer, sizeof(Buffer), iniFilePath); // KBytes
	g_FileBlkNum = BRDC_atoi(Buffer);
	if(g_FileBlkNum < 2) g_FileBlkNum = 2;
	GetInifileString(iniFilePath, _BRDC("Option"), _BRDC("DirFileName"), _BRDC("adcdir.bin"), g_dirFileName, sizeof(g_dirFileName));

	IPC_getPrivateProfileString(_BRDC("Option"), _BRDC("AdcDrqFlag"), _BRDC("2"), Buffer, sizeof(Buffer), iniFilePath);
	g_AdcDrqFlag = BRDC_atoi(Buffer);
	IPC_getPrivateProfileString(_BRDC("Option"), _BRDC("MemDrqFlag"), _BRDC("0"), Buffer, sizeof(Buffer), iniFilePath);
	g_MemDrqFlag = BRDC_atoi(Buffer);

	IPC_getPrivateProfileString(_BRDC("Debug"), _BRDC("IoDelay"), _BRDC("128"), Buffer, sizeof(Buffer), iniFilePath);
	g_IoDelay = BRDC_atoi(Buffer);

	if(g_IsWriteFile == 4)
	{
		BRDC_printf(_BRDC("File mapping mode: IsWriteFile=0, IsSystemMemory=2 !!!\n"));		
		g_IsWriteFile = 0;
		g_fileMap = 1;
		g_IsSysMem = 2;
	}
	IPC_getPrivateProfileString(_BRDC("Option"), _BRDC("RateRate"), _BRDC("0"), Buffer, sizeof(Buffer), iniFilePath);
	g_transRate = BRDC_atoi(Buffer);
	IPC_getPrivateProfileString(_BRDC("Option"), _BRDC("TimeoutSec"), _BRDC("5"), Buffer, sizeof(Buffer), iniFilePath); // sec
	g_MsTimeout = BRDC_atoi(Buffer) * 1000;

	IPC_getPrivateProfileString(_BRDC("Option"), _BRDC("SwitchOutMask"), _BRDC("0x18"), Buffer, sizeof(Buffer), iniFilePath);
	g_SwitchOutMask = BRDC_strtol(Buffer, &endptr, 0);
	
	IPC_getPrivateProfileString(_BRDC("Option"), _BRDC("RegDbg"), _BRDC("0"), Buffer, sizeof(Buffer), iniFilePath);
	g_regdbg = BRDC_atoi(Buffer);

	IPC_getPrivateProfileString(_BRDC("Option"), _BRDC("AdjustMode"), _BRDC("0"), Buffer, sizeof(Buffer), iniFilePath);
	g_adjust_mode = BRDC_atoi(Buffer);

	IPC_getPrivateProfileString(_BRDC("Option"), _BRDC("QuickQuit"), _BRDC("0"), Buffer, sizeof(Buffer), iniFilePath);
	g_quick_quit = BRDC_atoi(Buffer);

}

#include	"ctrlbasef.h"
//=******************* SetSwitch *****************************
//=************************************************************
void SetSwitch(BRD_Handle handle)
{
	S32 status;
	U32 mode = BRDcapt_SHARED;
	BRD_Handle hSrv = BRD_capture(handle, 0, &mode, _BRDC("BASEFMC0"), 10000);
	if(hSrv <= 0)
	{
		BRDC_printf(_BRDC("BASEFMC0 NOT capture (%X)\n"), hSrv);
		return;
	}
	if(mode == BRDcapt_EXCLUSIVE) BRDC_printf(_BRDC("BASEFMC Capture mode: EXCLUSIVE (%X)\n"), hSrv);
	if(mode == BRDcapt_SHARED) BRDC_printf(_BRDC("BASEFMC Capture mode: SHARED (%X)\n"), hSrv);
	if(mode == BRDcapt_SPY)	BRDC_printf(_BRDC("BASEFMC Capture mode: SPY (%X)\n"), hSrv);

	// после подачи питания все выходы коммутатора включены
	BRD_Switch sw;
	for(int i = 0; i < 8; i++)
	{
		sw.out = i;
		sw.val = (g_SwitchOutMask >> i) & 0x1;// 1 = on
		//if(!sw.val)
		// отключаем ненужные и включаем нужные выходы коммутатора
			status = BRD_ctrl(hSrv, 0, BRDctrl_BASEF_SWITCHONOFF, &sw);
	}

	status = BRD_release(hSrv, 0);
}

// захватываем службу АЦП, 
// устанавливаем параметры АЦП, 
// получаем размер собираемых данных в байтах, 
// проверяем наличие динамической памяти на модуле и, если надо, устанавливаем ее параметры, включая размер активной зоны
S32 SetParamSrv(BRD_Handle handle, BRD_ServList* srv, int idx)
{
	S32		status;
//	U32 mode = BRDcapt_EXCLUSIVE;
	BRD_Handle hADC = BRD_capture(handle, 0, &mode, srv->name, 10000);
	if(mode == BRDcapt_EXCLUSIVE) BRDC_printf(_BRDC("%s: Capture mode EXCLUSIVE\n"), srv->name);
	if(mode == BRDcapt_SPY)	BRDC_printf(_BRDC("%s: Capture mode SPY\n"), srv->name);
	if(hADC > 0)
	{
		if(g_SwitchOutMask <= 0xFF)
			SetSwitch(handle);
		//g_hSRV[g_nSrv++] = hADC;
		g_hSRV = hADC;

		//int numChan = AdcSettings(hADC, 0, g_SrvName, g_iniFileName); // установить параметры АЦП
		//g_numChan = AdcSettings(hADC, (g_lid != -1) ? g_lid : 0, g_AdcSrvNum, g_SrvName, g_iniFileName); // установить параметры АЦП
		g_numChan = AdcSettings(hADC, 0, g_AdcSrvNum, g_SrvName, g_iniFileName); // установить параметры АЦП

		//if(g_regdbg)
		//	RegProg(hADC, (g_lid != -1) ? g_lid : 0, g_AdcSrvNum);

		ULONG format = 0;
		BRD_ctrl(hADC, 0, BRDctrl_ADC_GETFORMAT, &format);
		ULONG sample_size = format ? format : sizeof(short);
		if(format == 0x80) // упакованные 12-разрядные данные
			sample_size = 2;
		sample_size &= FORMAT_MASK;

		g_bBufSize = (g_samplesOfChannel * g_numChan) * sample_size; // получить размер собираемых данных в байтах

		if(!g_DirWriteFile)
			BRDC_printf(_BRDC("SamplesPerChannel = %lld (%lld kBytes)\n"), g_samplesOfChannel, g_bBufSize/1024);

		g_bMemBufSize = (g_memorySamplesOfChannel * g_numChan) * sample_size; // получить размер собираемых в память данных в байтах

		g_bPostTrigSize = (g_nPostTrigSamples * g_numChan) * sample_size; // получить размер пост-триггерных данных (в байтах), собираемых в память 

		// проверяем наличие динамической памяти
		BRD_SdramCfgEx SdramConfig;
		SdramConfig.Size = sizeof(BRD_SdramCfgEx);
		ULONG PhysMemSize;
		status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_GETCFGEX, &SdramConfig);
		if(status < 0)
		{
			if(g_MemOn)
			{
				BRDC_printf(_BRDC("Get SDRAM Config: Error = 0x%X!!!\n"), status);
				g_MemOn = 0;
			}
			PhysMemSize = 0;
			return BRDerr_OK;
		}
		else
		{
			if(SdramConfig.MemType == 11 || //DDR3
			   SdramConfig.MemType == 12) //DDR4
				PhysMemSize =	(ULONG)((
										(((__int64)SdramConfig.CapacityMbits * 1024 * 1024) >> 3) * 
										(__int64)SdramConfig.PrimWidth / SdramConfig.ChipWidth * SdramConfig.ModuleBanks *
										SdramConfig.ModuleCnt) >> 2); // в 32-битных словах
			else
				PhysMemSize =	(1 << SdramConfig.RowAddrBits) *
								(1 << SdramConfig.ColAddrBits) * 
								SdramConfig.ModuleBanks * 
								SdramConfig.ChipBanks *
								SdramConfig.ModuleCnt * 2; // в 32-битных словах
		}
		if(PhysMemSize && g_MemOn)
		{ // динамическая память присутствует на модуле
			BRDC_printf(_BRDC("SDRAM Config: Memory size = %d MBytes\n"), (PhysMemSize / (1024 * 1024)) * 4);

			status = SdramSettings(g_MemOn, hADC, g_bMemBufSize); // установить параметры SDRAM
			g_memorySamplesOfChannel = (g_bMemBufSize / sample_size) / g_numChan;
			if(g_MemOn == 2)
			{
				BRDC_printf(_BRDC("SDRAM as a FIFO mode!!!\n"));
				g_MemOn = 0;
				g_MemAsFifo = 1;
			}
			else
				BRDC_printf(_BRDC("Samples of channel = %lld\n"), g_memorySamplesOfChannel);

		}
		else
		{
			 // освободить службу SDRAM (она могла быть захвачена командой BRDctrl_SDRAM_GETCFG, если та отработала без ошибки)
			ULONG mem_size = 0;
			status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_SETMEMSIZE, &mem_size);
			if(g_MemOn)
			{
				BRDC_printf(_BRDC("No SDRAM on board!!!\n"));
				g_MemOn = 0;
			}

			// установка узла ЦОС в режим получения данных от ПЛИС ЦОС
			//ULONG DspMode = 1;
			//status = BRD_ctrl(hADC, 0, BRDctrl_DSPNODE_SETMODE, &DspMode);
			//if(status < 0) // с узлом ЦОС работать нельзя
			//	DspMode = 0;

		}	
	
	}



	return status;
}

	// получить параметры, актуальные в режиме претриггера при вводе через FIFO
S32 GetPretrigData(BRD_Handle hADC)
{
	S32		status;
	// получить момент срабатывания на событие старта в режиме претриггера
	ULONG start_event;
	status = BRD_ctrl(hADC, 0, BRDctrl_ADC_GETPRETRIGEVENT, &start_event);
	g_bStartEvent = start_event << 2; // запомнить в байтах
	// уточнить момент срабатывания
	ULONG prec_event;
	status = BRD_ctrl(hADC, 0, BRDctrl_ADC_GETPREVENTPREC, &prec_event);
	ULONG bPrecEvent = prec_event << 1; // запомнить в байтах
	g_bStartEvent += bPrecEvent * g_numChan;

	return status;
}

	// получить параметры, актуальные в режиме претриггера (пост-триггера) при вводе через SDRAM
S32 GetPostrigData(BRD_Handle hADC)
{
	S32		status;
	// получить момент срабатывания на событие старта в режиме претриггера
	ULONG start_event;
	status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_GETPRETRIGEVENT, &start_event);
	g_bStartEvent = start_event << 2; // запомнить в байтах
	// уточнить момент срабатывания
	ULONG prec_event;
	status = BRD_ctrl(hADC, 0, BRDctrl_ADC_GETPREVENTPREC, &prec_event);
	ULONG bPrecEvent = prec_event << 1; // запомнить в байтах
	g_bStartEvent += bPrecEvent * g_numChan;

	// выяснить был ли переход через конец активной зоны
	status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_ISPASSMEMEND, &g_isPassMemEnd);
	// получить реальное число собранных данных
	ULONG acq_size;
	status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_GETACQSIZE, &acq_size);
	g_bRealSize = acq_size << 2; // запомнить в байтах

	return status;
}

S32 GetAdcData(BRD_Handle hADC, unsigned long long bBufSize, unsigned long long bMemBufSize)
{
	S32		status;
	int idx = 0;

	status = BRD_ctrl(hADC, 0, BRDctrl_ADC_PREPARESTART, NULL);
	if( status < 0 )
		if( !(BRD_errcmp( status, BRDerr_CMD_UNSUPPORTED ) 
		   || BRD_errcmp( status, BRDerr_INSUFFICIENT_SERVICES )) )
		{
			BRDC_printf(_BRDC("Prepare Start: Error = 0x%X!!!\n"), status);
			return status;
		}

	//RegProg(hADC, (g_lid != -1) ? g_lid : 0, g_AdcSrvNum);	
	RegProg(hADC, 0, g_AdcSrvNum);

	g_bBlkNum = 1;
	//PVOID pSig = NULL;
	PVOID* pSig = NULL; // указатель на массив указателей на блоки памяти с сигналом
	PVOID pSigNonDMA = NULL; // указатель на память с сигналом при программном (не ПДП) вводе
	if(g_DmaOn)
	{
		if(!g_Cycle)
			BRDC_printf(_BRDC("AllocDaqBuf (memory type = %d)\n"), g_IsSysMem);
		//status = AllocDaqBuf(hADC, &pSig, &bBufSize, g_IsSysMem);
		status = AllocDaqBuf(hADC, pSig, &bBufSize, g_IsSysMem, &g_bBlkNum);
		if(!BRD_errcmp(status, BRDerr_OK))
			return status;
		if(!pSig)
		{
			BRDC_printf(_BRDC("ERROR: Buffer NOT allocate!!!\n"));
			return status;
		}
	}
	else
		//pSig = new char[bBufSize];
	{
		pSigNonDMA = new char[bBufSize];
		pSig = &pSigNonDMA;

		BRD_AdcCfg adc_cfg;
		status = BRD_ctrl(hADC, 0, BRDctrl_ADC_GETCFG, &adc_cfg);
		if(adc_cfg.FifoSize < bBufSize)
		{
			ULONG format = 0;
			BRD_ctrl(hADC, 0, BRDctrl_ADC_GETFORMAT, &format);
			ULONG sample_size = format ? format : sizeof(short);
			if(format == 0x80) // упакованные 12-разрядные данные
				sample_size = 2;
			sample_size &= FORMAT_MASK;
			//ULONG daq_sample_num = bBufSize / g_numChan / sample_size;
			ULONG fifo_sample_num = adc_cfg.FifoSize / g_numChan / sample_size;
			BRDC_printf(_BRDC("Real acquisition data = %d samples per channel (It's FIFO size)\n"), fifo_sample_num);
		}
	}

	//BRDC_printf(_BRDC("Press any key for continue\n"));
	//IPC_getch();


	if(g_IsWriteFile || g_fileMap)
		WriteFlagSinc(0, 0, 0);
	int newParam_fl = 0xffffffff;
	if(!g_Cycle)
	{ // однократный сбор данных
		if(!g_DmaOn)
		{ // программная передача данных
			if(g_MemOn)
			{ // сбор в память и программная передача данных
				//status = DaqIntoSdram(hADC, pSig, bBufSize, numChan); // выполнить сбор данных
				status = DaqIntoSdram(hADC); // выполнить сбор данных в память
				status = DataFromMemWriteFile(hADC, pSig, bBufSize, bMemBufSize, g_DmaOn); // передать данные из памяти в ОЗУ ПК, а затем в файл
			}
			else
			{ // сбор в FIFO и программная передача данных
				status = DaqIntoFifo(hADC, pSigNonDMA, (ULONG)bBufSize, DspMode); // выполнить сбор данных
				WriteDataFile(hADC, 0, pSig, bBufSize);
			}
//				WriteDataFile(hADC, idx, pSig, bBufSize);
		}
		else
		{ // ПДП-передача данных
			if(g_MemOn)
			{ // сбор в память и ПДП-передача данных
				BRDC_printf(_BRDC("DAQ into SDRAM\n"));
				status = DaqIntoSdramDMA(hADC); // выполнить сбор данных
				if(status != -1)
					status = DataFromMemWriteFile(hADC, pSig, bBufSize, bMemBufSize, g_DmaOn); // передать данные из памяти в ОЗУ ПК, а затем в файл
			}
			else
			{ // сбор в FIFO и ПДП-передача данных
				BRDC_printf(_BRDC("DAQ into FIFO\n"));
				status = DaqIntoFifoDMA(hADC); // выполнить сбор данных
				WriteDataFile(hADC, 0, pSig, bBufSize);
			}
		}

			if(g_IsWriteFile || g_fileMap)
			{
				int rd_fl = ReadFlagSinc(0);
				if(rd_fl == 1)
					WriteFlagSinc(1, newParam_fl, 0);
				else
					WriteFlagSinc(0xffffffff, newParam_fl, 0);
			}

	}
	else
	{ // циклический (многократный) сбор данных
		//if(g_IsWriteFile || g_fileMap)
		//	WriteFlagSinc(0, 0, 0);
//				WriteFlagSinc(0, 0, idx);
		BRDC_printf(_BRDC("\n\n"));
		//BRDC_printf(_BRDC("Debugging pause. Press any key....\r"));
		//_getch();
		int loop = 0;
		//int newParam_fl = 0xffffffff;
		do
		{
//				printf("Send Loop: [0x%08lX]\r", loop );
			BRDC_printf(_BRDC("Send Loop: [%08d]\r"), loop );
			if(!g_DmaOn)
			{ // программная передача данных
				if(g_MemOn)
				{ // сбор в память и программная передача данных
					//status = DaqIntoSdram(hADC, pSig, bBufSize, numChan); // выполнить сбор данных
					status = DaqIntoSdram(hADC); // выполнить сбор данных в память
					status = DataFromMemWriteFile(hADC, pSig, bBufSize, bMemBufSize, g_DmaOn); // передать данные из памяти в ОЗУ ПК, а затем в файл
				}
				else
				{ // сбор в FIFO и программная передача данных
					status = DaqIntoFifo(hADC, pSigNonDMA, (ULONG)bBufSize, DspMode); // выполнить сбор данных
					WriteDataFile(hADC, 0, pSig, bBufSize);
				}
//					WriteDataFile(hADC, idx, pSig, bBufSize);
			}
			else
			{ // сбор в память и ПДП-передача данных
				if(g_MemOn)
				{ // сбор в память и ПДП-передача данных
#if defined(__IPC_LINUX__)
					status = DaqIntoSdramDMA(hADC); // выполнить сбор данных в память без использования прерывания по окончанию сбора
#else
					if(g_info.busType == BRDbus_USB)
						status = DaqIntoSdramDMA(hADC); // если устройство на шине USB, то выполнить сбор данных в память без использования прерывания по окончанию сбора
					else
					{
						status = StartDaqIntoSdramDMA(hADC, idx); // стартует сбор данных в память с использованием прерывания по окончанию сбора
						while(!CheckDaqIntoSdramDMA()) // проверяет завершение сбора данных
						{
							if(_kbhit()) // проверяет была ли нажата клавиша
							{
								int ch = _getch(); // получает клавишу
								if(0x1B == ch) // если Esc
								{
									BreakDaqIntoSdramDMA(); // // прерывает сбор данных
									loop = -1;
								}
							}
						}
						status = EndDaqIntoSdramDMA(); // закрывает открытые описатели (Handles)
					}
#endif
					//if(!BRD_errcmp(status, BRDerr_OK))
					//	loop = -1;
					//if(loop != -1)
					if(loop != -1 && BRD_errcmp(status, BRDerr_OK))
						status = DataFromMemWriteFile(hADC, pSig, bBufSize, bMemBufSize, g_DmaOn); // передать данные из памяти в ОЗУ ПК, а затем в файл
				}
				else
				{ // сбор в FIFO и ПДП-передача данных
					status = DaqIntoFifoDMA(hADC); // выполнить сбор данных
					if (loop != -1)
						WriteDataFile(hADC, 0, pSig, bBufSize);
				}
				//WriteDataFile(hADC, idx, g_buf_dscr.ppBlk[0], bBufSize);
				//WriteDataFile0(idx, buf_dscr.ppBlk[0], bBufSize);
			}
			if (g_Pause)
			{
				BRDC_printf(_BRDC("\nPress any key for next start...\n"));
				IPC_getch();
			}
			if(loop == -1)
				break;
			if(g_IsWriteFile || g_fileMap)
			{
				int rd_fl = ReadFlagSinc(0);
				//BRDC_printf(_BRDC("rd_fl = %d\n"), rd_fl);
				if(rd_fl == 1)
					WriteFlagSinc(1, newParam_fl, 0);
				else
				{
					if(g_Cycle <= 1)
						WriteFlagSinc(0xffffffff, newParam_fl, 0);
					if(g_Cycle > 1 && g_times == g_Cycle)
						WriteFlagSinc(0xffffffff, 0xffffffff, 0);
				}
				//WriteFlagSinc(0xffffffff, newParam_fl, idx);
				loop++;
				int rd_idx = 0;
				newParam_fl = 0;
				do
				{
					rd_fl = ReadFlagSinc(0);
#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
					if(IPC_kbhit())
					{
						int ch = IPC_getch();
#else
					if(_kbhit())
					{
						int ch = _getch();
#endif
						if(0x1B == ch) 
							loop = 0;
						if(0x20 == ch)
						{
							status = AdcSettings(hADC, 0, g_AdcSrvNum, g_SrvName, g_iniFileName); // установить параметры АЦП
							status = BRD_ctrl(hADC, 0, BRDctrl_ADC_PREPARESTART, NULL);
							newParam_fl = 0xffffffff;
						}
					}
#if defined(__IPC_LINUX__)
					if((rd_fl == 0) || g_StopFlag)
#else
					if (rd_fl == 0)
#endif
					{
						break;
					}
					//if((rd_fl == 0xffffffff) && (rd_idx>50))
					if((rd_fl == 0xffffffff) && (rd_idx>20))
						break;
#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
					IPC_delay(30);
#else
					Sleep(30);
#endif
					rd_idx++;
				}while(loop);
			}
			else
			{
				loop++;
#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
				if(IPC_kbhit())
					if(0x1B == IPC_getch()) 
#else
				if(_kbhit())
					if(0x1B == _getch()) 
#endif
						loop = 0;
			}
			if(g_Cycle > 1 && loop == g_Cycle)
				break;
#if defined(__IPC_LINUX__)
			if(g_StopFlag)
				break;
#endif
		}while(loop);
	}
	if(g_DmaOn)
		status = FreeDaqBuf(hADC, g_bBlkNum);
	else
		delete pSigNonDMA;

	return status;
}

//S32	DataFromMemWriteFile(BRD_Handle hADC, PVOID pBuf, unsigned long long bBufSize, unsigned long long bMemBufSize, ULONG DmaOn)
S32	DataFromMemWriteFile(BRD_Handle hADC, PVOID* pBuf, unsigned long long bBufSize, unsigned long long bMemBufSize, ULONG DmaOn)
{
#ifdef __linux__
    if(g_fileMap)
        return MapDataFromMemWriteData(hADC, pBuf, bBufSize, bMemBufSize, DmaOn);
#endif

	S32	status = BRDerr_OK;
	if(!g_IsWriteFile)
		return 1;
	if(!g_Cycle)
		BRDC_printf(_BRDC("Writing file...\n"));

	if(g_PretrigMode == 3)
	{
		// получить параметры, актуальные в режиме претриггера
		status = GetPostrigData(hADC);
	}

	BRDCHAR fileName[64];
	BRDC_sprintf(fileName, _BRDC("data_%d.bin"), 0);

	IPC_handle hfile = IPC_openFile(fileName, IPC_CREATE_FILE | IPC_FILE_WRONLY);
	if(!hfile)
	{
		BRDC_printf(_BRDC("Create file %s error.\n"), fileName);
		return 1;
	}
	int bResult = TRUE;

	int nCnt = int(bMemBufSize / bBufSize);
	for(int iCnt = 0; iCnt < nCnt; iCnt++)
	{
		status = DataFromMem(hADC, *pBuf, (ULONG)bBufSize, DmaOn);
		if (!BRD_errcmp(status, BRDerr_OK))
			return status;
		bResult = IPC_writeFile(hfile, *pBuf, (ULONG)bBufSize);
	}
	ULONG ostSize = ULONG(bMemBufSize % bBufSize);
	if(ostSize)
	{
		status = DataFromMem(hADC, *pBuf, (ULONG)bBufSize, DmaOn);
		bResult = IPC_writeFile(hfile, *pBuf, ostSize);
	}

	if(g_IsWriteFile == 1)
		WriteIsviParams(hfile, hADC, bMemBufSize);

	IPC_closeFile(hfile);
	if(bResult < 0)
		BRDC_printf(_BRDC("Write file %s error.\n"), fileName);
	else
		if(!g_Cycle)
			BRDC_printf(_BRDC("Write file %s is OK.\n"), fileName);
	return status;
}

void WriteDataFile(BRD_Handle hADC, int idx, PVOID* pBuf, unsigned long long nNumberOfBytes)
{
	S32	status = BRDerr_OK;

#ifdef __linux__
	if(g_fileMap)
	{
		MapWriteData(pBuf, nNumberOfBytes);
		return;
	}
#endif

	if(!g_IsWriteFile)
		return;
	if(!g_Cycle)
		BRDC_printf(_BRDC("Writing file...\n"));

	if(g_PretrigMode)
	{   // получить параметры, актуальные в режиме претриггера
		status = GetPretrigData(hADC);
	}

	BRDCHAR fileName[64];
	BRDC_sprintf(fileName, _BRDC("data_%d.bin"), idx);
	IPC_handle hfile = 0;
	if(g_Cycle > 1 && g_times)
	{	
		hfile = IPC_openFile(fileName, IPC_OPEN_FILE | IPC_FILE_WRONLY);
		IPC_setPosFile(hfile, 0, IPC_FILE_END);
	}
	else
	{
		hfile = IPC_openFile(fileName, IPC_CREATE_FILE | IPC_FILE_WRONLY);
	}
	g_times++;
	if(!hfile)
	{
		BRDC_printf(_BRDC("Create file %s error.\n"), fileName);
		return;
	}

	int bResult;
	DWORD blockSize = DWORD(nNumberOfBytes / g_bBlkNum);
	for(ULONG i = 0; i < g_bBlkNum; i++)
	{
		bResult = IPC_writeFile(hfile, pBuf[i], blockSize);
	}
//	BOOL bResult = WriteFile(hfile, pBuf, nNumberOfBytes, &nNumberOfWriteBytes, NULL);

	if(g_IsWriteFile == 1)
		if((g_Cycle > 1 && g_times == g_Cycle) || g_Cycle <= 1)
			WriteIsviParams(hfile, hADC, nNumberOfBytes);

	IPC_closeFile(hfile);
	if(bResult < 0)
		BRDC_printf(_BRDC("Write file %s error.\n"), fileName);
	else
		if(!g_Cycle)
			BRDC_printf(_BRDC("Write file %s is OK.\n"), fileName);
}

void WriteIsviParamDirFile()
{
	BRDCHAR fileName[MAX_PATH];

	BRDC_strcpy(fileName, g_dirFileName);

	IPC_handle hfile = IPC_openFile(fileName, IPC_OPEN_FILE | IPC_FILE_RDWR);
	if(!hfile)
	{
		BRDC_printf(_BRDC("Create file %s error.\n"), fileName);
		return;
	}
	int BytesNum = IPC_setPosFile(hfile, 0, IPC_FILE_END);

	if(BytesNum == -1)
		BRDC_printf(_BRDC("File %s is too big for ISVI parameters writing.\n"), fileName);
	else
		WriteIsviParams(hfile, g_hSRV, BytesNum);
	IPC_closeFile(hfile);
}

#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
void WriteIsviParams(IPC_handle hfile, BRD_Handle hADC, unsigned long long nNumberOfBytes)
#else
void WriteIsviParams(HANDLE hfile, BRD_Handle hADC, unsigned long long nNumberOfBytes)
#endif
{
	char	fileInfo[2048];
	char    str_buf[512];

    sprintf(fileInfo, "\r\nDEVICE_NAME_________ " );
#ifdef _WIN64
	char    srvName[32];
	wcstombs(srvName, g_AdcSrvName, 32);
    lstrcatA(fileInfo, srvName);
#else
    lstrcatA(fileInfo, g_AdcSrvName);
#endif

	BRD_AdcCfg adc_cfg;
	BRD_ctrl(hADC, 0, BRDctrl_ADC_GETCFG, &adc_cfg);
	//BRD_ctrl(hADC, 0, BRDctrl_ADC_GETRATE, &g_samplRate);
	ULONG chanMask;
	BRD_ctrl(hADC, 0, BRDctrl_ADC_GETCHANMASK, &chanMask);
	ULONG format = 0;
	BRD_ctrl(hADC, 0, BRDctrl_ADC_GETFORMAT, &format);

	ULONG is_complex = 0;
	BRD_ctrl(hADC, 0, BRDctrl_ADC_ISCOMPLEX, &is_complex);

	int num_chan = 0;
	int	chans[2*MAX_CHAN];
	double gains[2*MAX_CHAN];
	double volt_offset[2*MAX_CHAN];
	ULONG mask = 1;
	BRD_ValChan val_chan;
	for(ULONG iChan = 0; iChan < MAX_CHAN; iChan++)
	{
		if (is_complex)
		{
			if (chanMask & mask)
			{
				chans[num_chan++] = iChan * 2;
				chans[num_chan++] = iChan * 2 + 1;
			}
			val_chan.chan = iChan;
			BRD_ctrl(hADC, 0, BRDctrl_ADC_GETGAIN, &val_chan);
			if (adc_cfg.ChanType == 1)
				val_chan.value = pow(10., val_chan.value / 20); // dB -> разы
			gains[iChan * 2] = val_chan.value;
			volt_offset[iChan * 2] = 0.0;
			gains[iChan * 2 + 1] = val_chan.value;
			volt_offset[iChan * 2 + 1] = 0.0;
		}
		else
		{
			if (chanMask & mask)
				chans[num_chan++] = iChan;
			val_chan.chan = iChan;
			BRD_ctrl(hADC, 0, BRDctrl_ADC_GETGAIN, &val_chan);
			if (adc_cfg.ChanType == 1)
				val_chan.value = pow(10., val_chan.value / 20); // dB -> разы
			gains[iChan] = val_chan.value;
			volt_offset[iChan] = 0.0;
		}
		mask <<= 1;
	}

	if(BRDC_strstr(g_AdcSrvName, _BRDC("ADC1624X192")) || BRDC_strstr(g_AdcSrvName, _BRDC("ADC1624X128")))
	{
		int i = 0, j = 0;
		num_chan = 0;
		for(int i = 0; i < 16; i++)
			num_chan += (chanMask >> i) & 0x1;
		for(i = 0; i < 16; i++)
		{
			if(chanMask & (0x1 << i))
				chans[j++] = i;
			i++;
		}
		for(i = 1; i < 16; i++)
		{
			if(chanMask & (0x1 << i))
				chans[j++] = i;
			i++;
		}
	}

	if(!num_chan)
	{
		BRDC_printf(_BRDC("Number of channels for Isvi Parameters is 0 - Error!!!\n"));
		return;
	}
	BRD_ItemArray chan_order;
	chan_order.item = num_chan;
	chan_order.itemReal = 0;
	chan_order.pItem = &chans;
	BRD_ctrl(hADC, 0, BRDctrl_ADC_GETCHANORDER, &chan_order);

	sprintf(str_buf, "\r\nNUMBER_OF_CHANNELS__ %d", num_chan);
    lstrcatA(fileInfo, str_buf);

	sprintf(str_buf, "\r\nNUMBERS_OF_CHANNELS_ ");
    lstrcatA(fileInfo, str_buf);
	str_buf[0] = 0;
	for(int iChan = 0; iChan < num_chan; iChan++)
	{
		char buf[16];
		sprintf(buf, "%d,", chans[iChan]);
		lstrcatA(str_buf, buf);
	}
    lstrcatA(fileInfo, str_buf);

	ULONG sample_size = format ? format : sizeof(short);
	if(format == 0x80) // упакованные 12-разрядные данные
		sample_size = 2;
	sample_size &= FORMAT_MASK;

	unsigned long long samples = nNumberOfBytes / sample_size / num_chan;
	if(g_Cycle > 1)
		 samples *= g_Cycle;
    sprintf(str_buf, "\r\nNUMBER_OF_SAMPLES___ %lld", samples );
    lstrcatA(fileInfo, str_buf);

    sprintf(str_buf, "\r\nSAMPLING_RATE_______ %f", g_samplRate );
    lstrcatA(fileInfo, str_buf);

    sprintf(str_buf, "\r\nBYTES_PER_SAMPLES___ %d", sample_size);
    lstrcatA(fileInfo, str_buf);

	if(format == 0x80) // два 12-и битных отсчета упаковано в 3 байта
	    lstrcatA(fileInfo, "\r\nSAMPLES_PER_BYTES___ 3");
	else
		lstrcatA(fileInfo, "\r\nSAMPLES_PER_BYTES___ 1");

	if(is_complex)
	    sprintf(str_buf, "\r\nIS_COMPLEX_SIGNAL?__ YES");
	else
	    sprintf(str_buf, "\r\nIS_COMPLEX_SIGNAL?__ NO");
    lstrcatA(fileInfo, str_buf);

	double fc[MAX_CHAN];
	sprintf(str_buf, "\r\nSHIFT_FREQUENCY_____ ");
    lstrcatA(fileInfo, str_buf);
	str_buf[0] = 0;
	for(int iChan = 0; iChan < num_chan; iChan++)
	{
		char buf[16];
		fc[iChan] = 0.0;
		if (is_complex)
		{
			BRD_ValChan val_chan;
			val_chan.chan = iChan / 2;
			BRD_ctrl(hADC, 0, BRDctrl_ADC_GETFC, &val_chan);
			fc[iChan] = val_chan.value;
		}
		sprintf(buf, "%.2f,", fc[iChan]);
		lstrcatA(str_buf, buf);
	}
    lstrcatA(fileInfo, str_buf);

	sprintf(str_buf, "\r\nGAINS_______________ ");
    lstrcatA(fileInfo, str_buf);
	str_buf[0] = 0;
	for(int iChan = 0; iChan < num_chan; iChan++)
	{
		char buf[16];
		sprintf(buf,"%f,", gains[chans[iChan]]);
		lstrcatA(str_buf, buf);
	}
    lstrcatA(fileInfo, str_buf);

	sprintf(str_buf, "\r\nVOLTAGE_OFFSETS_____ ");
    lstrcatA(fileInfo, str_buf);
	str_buf[0] = 0;
	for(int iChan = 0; iChan < num_chan; iChan++)
	{
		char buf[16];
		sprintf(buf,"%f,", volt_offset[iChan]);
		lstrcatA(str_buf, buf);
	}
    lstrcatA(fileInfo, str_buf);

    sprintf(str_buf, "\r\nVOLTAGE_RANGE_______ %f", adc_cfg.InpRange / 1000.);
    lstrcatA(fileInfo, str_buf);

	//int BitsPerSample = (format == 1) ? 8 : adc_cfg.Bits;
	//if(BRDC_strstr(g_AdcSrvName, _BRDC("ADC1624X192")) || BRDC_strstr(g_AdcSrvName, _BRDC("ADC818X800")))
	//	if(format == 0 || format == 2)
	//		BitsPerSample = 16;
	int BitsPerSample = adc_cfg.Bits;
	if( (format&FORMAT_MASK) == 1 )
		if( BitsPerSample > 8 )
			BitsPerSample = 8;
	if( ((format&FORMAT_MASK) == 0) || ((format&FORMAT_MASK) == 2) )
		if( BitsPerSample > 16 )
			BitsPerSample = 16;
    sprintf(str_buf, "\r\nBIT_RANGE___________ %d", BitsPerSample);
    lstrcatA(fileInfo, str_buf);
    lstrcatA(fileInfo, "\r\n");

	if(g_PretrigMode)
	{
		ULONG event_mark = ULONG(g_bStartEvent / sample_size / num_chan);
		sprintf(str_buf, "\r\nPRETRIGGER_SAMPLE___ %d", event_mark);
		lstrcatA(fileInfo, str_buf);
	}

	int len = lstrlenA(fileInfo);

#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
	int bResult = IPC_writeFile(hfile, fileInfo, len);
        if(bResult < 0)
#else
	ULONG nNumberOfWriteBytes;
	BOOL bResult = WriteFile(hfile, fileInfo, len, &nNumberOfWriteBytes, NULL);
	if(bResult != TRUE)
#endif
		BRDC_printf(_BRDC("Write ISVI info error\n"));

}

//********************************************************
void WriteFlagSinc(int flg, int isNewParam, int idx)
{
	if(g_fileMap)
		MapWrFlagSinc(flg, isNewParam);
	else
	{
		BRDCHAR fileName[64];
		BRDC_sprintf(fileName, _BRDC("data_%d.flg"), idx);
		int val[2];
#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
		IPC_handle handle = NULL;
		while(!handle)
			handle = IPC_openFile(fileName, IPC_CREATE_FILE | IPC_FILE_WRONLY);
		val[0] = flg;
		val[1] = isNewParam;
		IPC_writeFile(handle, val, 8);
		IPC_closeFile(handle);
#else
		int fs = -1;
		while(fs == -1)
			fs = BRDC_sopen(fileName, _O_WRONLY|_O_BINARY|_O_CREAT|_O_TRUNC, _SH_DENYNO, _S_IWRITE);
	//    errno_t err = _sopen_s( &fs, fileName, _O_WRONLY|_O_BINARY|_O_CREAT, _SH_DENYNO, S_IWRITE );
		val[0] = flg;
		val[1] = isNewParam;
		_write(fs, val, 8);
		_close(fs);
#endif
	}
}

//********************************************************
int ReadFlagSinc(int idx)
{
	int flg;
	if(g_fileMap)
		flg = MapRdFlagSinc();
	else
	{
		BRDCHAR fileName[64];
		BRDC_sprintf(fileName, _BRDC("data_%d.flg"), idx);
#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
		IPC_handle handle = NULL;
		while(!handle)
			handle = IPC_openFile(fileName, IPC_OPEN_FILE | IPC_FILE_RDONLY);
		IPC_readFile(handle, &flg, 4);
		IPC_closeFile(handle);
#else
	    int fs = -1;
		while(fs == -1)
			fs = BRDC_sopen(fileName, _O_RDONLY|_O_BINARY, _SH_DENYNO, _S_IREAD);
	//    errno_t err = _sopen_s( &fs, fileName, _O_RDONLY|_O_BINARY|_O_CREAT, _SH_DENYNO, S_IREAD );
		_read(fs,&flg,4);
		_close(fs);
#endif
	}
    return flg;
}

#include "ctrlreg.h"
S32 RegRwSpd(BRD_Handle hDev, BRDCHAR* fname);

S32 RegProg(BRD_Handle hAdc, int idx, int isx)
{
	S32		status;
	BRDCHAR iniFilePath[MAX_PATH];
	BRDCHAR iniSectionName[MAX_PATH];
	IPC_getCurrentDir(iniFilePath, sizeof(iniFilePath) / sizeof(BRDCHAR));
	BRDC_strcat(iniFilePath, g_iniFileName);
	BRDC_sprintf(iniSectionName, _BRDC("device%d_%s%d"), idx, g_SrvName, (g_subNo<0) ? isx : 0);

	BRDCHAR regfname[128];
	//GetInifileString(iniFilePath, iniSectionName, _BRDC("RegFileName"), _BRDC("spd_dev.ini"), regfname, sizeof(regfname));
	GetInifileString(iniFilePath, iniSectionName, _BRDC("RegFileName"), _BRDC(""), regfname, sizeof(regfname));

	// Если указан файл, то подгрузить регистры из файла
	if (regfname[0])
		status = RegRwSpd(hAdc & 0xFFFF, regfname); // hAdc & 0xFFFF - из дескриптора службы делаем дескриптор устройства

	return status;
}

//
// End of file
//
