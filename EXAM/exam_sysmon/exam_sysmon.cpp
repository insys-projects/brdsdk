
#include <stdio.h>
#include <math.h>

#ifdef __IPC_WIN__
 #include <locale.h>
#endif

#include "gipcy.h"

#include "utypes.h"
#include "brd.h"
#include "brdpu.h"
#include "ctrlsysmon.h"

U08	g_isMon;	// 1 - непрерывная работа

REAL64 g_fTemp;		// Температура кристалла
REAL64 g_fTempMax;	// Максимальная температура
REAL64 g_fTempMin;	// Минимальная температура
U08 g_isTemp;	// 1 - следить за температурой кристалла

REAL64 g_fVCCint;		// Напряжение питания ядра
REAL64 g_fVCCintMax;		// Максимальное питание ядра ПЛИС
REAL64 g_fVCCintMin;		// Минимальное питание ядра ПЛИС
REAL64 g_fVCCintNominal;	// Номинал питания ядра
U08 g_isVCCint;	// 1 - следить за напряжением питания ядра

REAL64 g_fVCCaux;		// Напряжение питания ПЛИС
REAL64 g_fVCCauxMax;		// Максимальное питане ПЛИС
REAL64 g_fVCCauxMin;		// Минимальное питание ПЛИС
REAL64 g_fVCCauxNominal;	// Номинал питания ПЛИС
U08 g_isVCCaux; // 1 - следить за напряжением питания ПЛИС

REAL64 g_fVrefp;		// Внешнее опорное напряжение (плюс)
REAL64 g_fVrefpNominal; // Номинал внешнего опорного напряжения (плюс)
U08 g_isVrefp;	// 1 - следить за внешним опорным напряжением (плюс)

REAL64 g_fVrefn;		// Внешнее опорное напряжение  (минус)
REAL64 g_fVrefnNominal; // Номинал внешнего опорного напряжения (минус)
U08 g_isVrefn = 1;	// 1 - следить за внешним опорным напряжением (минус)

REAL64 g_fVCCbram;			// Напряжение питания блока RAM
REAL64 g_fVCCbramMax;		// Максимальное питание блока RAM
REAL64 g_fVCCbramMin;		// Минимальное питание блока RAM
REAL64 g_fVCCbramNominal;	// Номинал питания блока RAM
U08 g_isVCCbram = 1;	// 1 - следить за напряжением питания блока RAM

BRD_Handle	g_handle, g_hSysMon;
S32			g_nLid = -1;
S32			g_nPuId = -1;
U08			g_isExit = 0;

// Вывод справки
void PrintHelp();
// Разборка ini-файла
U32 ParseIniFile(BRDCHAR *pFileName);
// Проверка, загружена ли прошивка
U08 isPldLoad();
// Получение дескриптора службы
BRD_Handle GetSrvHandle(BRD_Handle handle, BRD_ServList* srv, U32 *pMode);
// Запуск монитора
void StartMonitor();
// Выход
void Exit();
// Вывести информацию о базовом модуле, субмодуле, тетрадах
void DeviceInfo();
// Вывод результатов
void PrintResult();

int	BRDC_main(int argc, BRDCHAR *argv[])
{
	BRDCHAR sIniFileName[MAX_PATH] = _BRDC("//exam_sysmon.ini");

#ifdef __IPC_WIN__
//	setlocale( LC_ALL, "Russian" );
	setlocale( LC_CTYPE, "Russian" );
#endif

	IPC_initKeyboard();

	if(argc == 2)
	{	// Вывод справки
		if(BRDC_strstr(argv[1], _BRDC("-help")))
		{
			PrintHelp();
			Exit();
			return 0;
		}
	}

	// Разборка ini-файла
	if(ParseIniFile(sIniFileName))
	{
		Exit();
		return 0;
	}

	//
	// Init BRD Shell 
	//
	{
		S32	err;
		BRD_displayMode(BRDdm_VISIBLE | BRDdm_CONSOLE);

		S32	DevNum;
		err = BRD_init(_BRDC("brd.ini"), &DevNum); // инициализировать библиотеку Bardy
		if(0 > err)
		{	// Ошибка инициализации
			if( BRD_errcmp(err,BRDerr_NONE_DEVICE) )
				BRDC_printf( _BRDC("ERROR: None Board\n") );
			else
				BRDC_printf( _BRDC("ERROR: Can't Initialize BRD Shell\n") );

			Exit();
			return 0;
		}
	}

	//
	// Define LID
	//
	if( g_nLid==-1 )
	{	// Получение LID
		U32			lidArray[16];
		U32			itemReal;

		BRD_lidList( lidArray, 16, &itemReal );

		if( itemReal==0 )
		{	// Платы не обнаружены
			BRDC_printf(_BRDC("ERROR: None Board\n"));
			Exit();
			return 0;
		}

		g_nLid = lidArray[0];
	}

	U32 nMode;

	// Открытие логической связи с базовым модулем
	g_handle = BRD_open( g_nLid, BRDopen_SHARED, &nMode);

	if(nMode == BRDopen_SPY)
	{
		BRDC_printf(_BRDC("ERROR: Capture mode: SPY\n"));
	}

	if(g_handle<=0)
	{	// Ошибка открытия
		BRDC_printf(_BRDC("ERROR: Can't Open device (LID=%d)\n"), g_nLid);
		BRD_cleanup();
		Exit();
		return 0;
	}

	if(!isPldLoad()) // Проверка загрузки прошивки
	{
		BRDC_printf(_BRDC("ERROR: PLD is NOT loaded!\n"));
		Exit();
		return 0;
	}

	// Захват службы
	U32 ItemReal;
	BRD_ServList srvList[10];

	// Получение списка всех служб для данного модуля
	BRD_serviceList(g_handle, 0, srvList, 10, &ItemReal);

	U32 iSrv;
	int nSrvFounded = 0;

	for(iSrv = 0; iSrv < ItemReal; iSrv++)
	{
		if(!BRDC_strcmp(srvList[iSrv].name, _BRDC("SYSMON0")))
		{	// Служба найдена
			g_hSysMon = GetSrvHandle(g_handle, &srvList[iSrv], &nMode);
			nSrvFounded = 1;
			break;
		}
	}

	if(g_hSysMon < 0)
	{
		BRD_close( g_handle );
		BRD_cleanup();
		Exit();
		return 0;
	}

	if(!nSrvFounded)
	{	// Служба SYSMON0 не найдена
		BRDC_printf(_BRDC("ERROR: SYSMON0 service is NOT found!\n"));
		//BRDC_printf(_BRDC("ERROR: Service SYSMON0 not founded!\n"));
		BRDC_printf(_BRDC("LOG:error\n"));
		BRDC_printf(_BRDC("LOG: System monitor is absent\n"));
		BRD_close( g_handle );
		BRD_cleanup();
		Exit();
		return 0;
	}

	StartMonitor(); // Запуск монитора

	BRD_close( g_handle );
	BRD_cleanup();
	Exit();
	return 0;
}

// Вывод справки
void PrintHelp()
{
	BRDC_printf(_BRDC("LOG: exam_sysmon [option,...]\n"));
	BRDC_printf(_BRDC("LOG: option:\n"));
	BRDC_printf(_BRDC("LOG:    -lid<lid>    - lid of device, default -1\n"));
	BRDC_printf(_BRDC("LOG:    -pu<puid>    - id number of Programable Unit, default -1\n"));
	BRDC_printf(_BRDC("LOG:    -brddir<dir> - path of brd.ini, default \"\"\n"));
	BRDC_printf(_BRDC("LOG:    -f<file>     - ini-file\n"));
}

// Разборка ini-файла
U32 ParseIniFile(BRDCHAR *pFileName)
{
  	BRDCHAR sFullName[MAX_PATH]; // Абсолютное имя файла
  	BRDCHAR Buffer[MAX_PATH];
 	//int retCode;
 
	//retCode = SearchPath( NULL, pFileName, NULL, MAX_PATH, sFullName, NULL);
	IPC_getCurrentDir(sFullName, sizeof(sFullName)/sizeof(BRDCHAR));
	BRDC_strcat(sFullName, pFileName);
  
	IPC_getPrivateProfileString(_BRDC("Option"), _BRDC("Temperature"), _BRDC("1"), Buffer, sizeof(Buffer), sFullName);
	g_isTemp = BRDC_atoi(Buffer);

	IPC_getPrivateProfileString(_BRDC("Option"), _BRDC("VCCint"), _BRDC("1"), Buffer, sizeof(Buffer), sFullName);
	g_isVCCint = BRDC_atoi(Buffer);
	
	IPC_getPrivateProfileString(_BRDC("Option"), _BRDC("VCCaux"), _BRDC("0"), Buffer, sizeof(Buffer), sFullName);
	g_isVCCaux = BRDC_atoi(Buffer);
	
	IPC_getPrivateProfileString(_BRDC("Option"), _BRDC("Vrefp"), _BRDC("0"), Buffer, sizeof(Buffer), sFullName);
	g_isVrefp = BRDC_atoi(Buffer);

	IPC_getPrivateProfileString(_BRDC("Option"), _BRDC("Vrefn"), _BRDC("0"), Buffer, sizeof(Buffer), sFullName);
	g_isVrefn = BRDC_atoi(Buffer);

	IPC_getPrivateProfileString(_BRDC("Option"), _BRDC("VCCbram"), _BRDC("0"), Buffer, sizeof(Buffer), sFullName);
	g_isVCCbram = BRDC_atoi(Buffer);

	IPC_getPrivateProfileString(_BRDC("Option"), _BRDC("IsMon"), _BRDC("0"), Buffer, sizeof(Buffer), sFullName);
	g_isMon = BRDC_atoi(Buffer);
	
	return 0;
}

// Проверка, загружена ли прошивка
U08 isPldLoad()
{
	S32	err;

	//
	// Detect EEPROM/FLASH/PLD Information
	//
	U32	puAttr;
	{
		BRD_PuList		*paPuList;
		U32				item;
		int				ii;
		int				isPuOK = 0;

		//
		// Get PU LIST
		//
		err = BRD_puList( g_handle, NULL, 0, &item );
		paPuList = (BRD_PuList*)malloc( item * sizeof(BRD_PuList) );
		if( NULL==paPuList )
		{
			BRDC_printf(_BRDC("ERROR: No Enough Memory\n\n"));
			return 1;
		}
		err = BRD_puList( g_handle, paPuList, item, &item );
		if( err<0 )
		{
			BRDC_printf(_BRDC("ERROR: Device doesn't have Programmable Units\n\n"));
			free( paPuList );
			return 1;
		}

		//
		// Check puID
		//
		puAttr = 0;

		if(g_nPuId == -1)
		{
			g_nPuId = paPuList[0].puId;
			puAttr = paPuList[0].puAttr;
			isPuOK = 1;
		}
		else
			for( ii=0; ii<(int)item; ii++ )
			{
				if( g_nPuId == paPuList[ii].puId )
				{
					isPuOK = 1;
					puAttr = paPuList[ii].puAttr;
				}
			}

			free( paPuList );

			if( isPuOK==0 )
			{
				BRDC_printf(_BRDC("ERROR: Wrong PuID parameter (%d)\n\n"), g_nPuId);
				return 1;
			}
	}

	//
	// Check g_puId
	//
	if( (g_nPuId < 0x100) || (g_nPuId >= 0x200) || (puAttr&BRDpua_Danger) )
	{	
		BRDC_printf(_BRDC("LOG: *******************************************************************\n"));
		BRDC_printf(_BRDC("LOG: * WARNING: It may by very dangerous to program PuID:%d!!!\n"), g_nPuId);
		BRDC_printf(_BRDC("LOG: *******************************************************************\n\n"));
	}

	U32	pldState = 0;
	BRD_puState(g_handle, g_nPuId, &pldState);
	return pldState;
}

// Получение дескриптора службы
BRD_Handle GetSrvHandle(BRD_Handle handle, BRD_ServList* srv, U32 *pMode)
{
	U32 mode = BRDcapt_SHARED;
	BRD_Handle hSrv = BRD_capture(handle, 0, &mode, srv->name, 10000);
	if(mode == BRDcapt_EXCLUSIVE) BRDC_printf(_BRDC("LOG: %s Capture mode: SHARED\n"), srv->name);
	if(mode == BRDcapt_SPY)	BRDC_printf(_BRDC("ERROR: %s Capture mode: SPY\n"), srv->name);
	if(hSrv <= 0)
		BRDC_printf(_BRDC("ERROR: %s is NOT capture!\n"), srv->name);
	*pMode = mode;
	return hSrv;
}

// Вывести информацию о базовом модуле, субмодуле, тетрадах
void DeviceInfo()
{
	BRD_Info info = {sizeof(info)};

	BRD_getInfo( g_nLid, &info );
//	printf( "LOG:\tLID=%2d, BOARD=%s, PID=%d(0x%X), BUS:DEV=%d:%d, SLOT=%d\n", 
//		g_nLid, info.name, info.pid, info.pid, info.bus, info.dev, info.slot );

	ULONG dev_id = info.boardType >> 16;
	if (dev_id == 0x53B1 || dev_id == 0x53B3) // FMC115cP or FMC117cP
		BRDC_printf(_BRDC("%s, DevID = 0x%x, RevID = 0x%x, Bus = %d, Dev = %d, G.adr = %d, Order = %d, PID = %d.\n"),
			info.name, info.boardType >> 16, info.boardType & 0xff, info.bus, info.dev,
			info.slot & 0xffff, info.pid >> 28, info.pid & 0xfffffff);
	else
		BRDC_printf(_BRDC("%s, DevID = 0x%x, RevID = 0x%x, Bus = %d, Dev = %d, Slot = %d, PID = %d.\n"),
			info.name, info.boardType >> 16, info.boardType & 0xff, info.bus, info.dev, info.slot, info.pid);

}

// Выход
void Exit()
{
	if(!g_isExit)
	{
		BRDC_printf(_BRDC("Press any key for leaving program...\n"));
		IPC_getch();
	}
	IPC_cleanupKeyboard();
}
// Запуск монитора
void StartMonitor()
{

	BRD_VoltNominals7s voltNominals;
	S32	err = BRD_ctrl(g_hSysMon, 0, BRDctrl_SYSMON_GETVN7S, &voltNominals);
	if(BRD_errcmp(err, BRDerr_OK))
		g_fVCCbramNominal = voltNominals.vccbram;
	else
	{
		BRD_ctrl(g_hSysMon, 0, BRDctrl_SYSMON_GETVNOMINALS, &voltNominals);
		g_isVCCbram = 0;
	}
	g_fVCCintNominal = voltNominals.vccint;
	g_fVCCauxNominal = voltNominals.vccaux;
	g_fVrefpNominal = voltNominals.vrefp;
	g_fVrefnNominal = voltNominals.vrefn;

	BRD_SysMonVal sysmon_data;
	do
	{
		//Снятие значения температуры
		if(g_isTemp)	
		{
			BRD_ctrl(g_hSysMon, 0, BRDctrl_SYSMON_GETTEMP, &sysmon_data);
			g_fTemp = sysmon_data.curv;
			g_fTempMax = sysmon_data.maxv;
			g_fTempMin = sysmon_data.minv;
		}

		//Снятие значения Vccint
		if(g_isVCCint)
		{
			BRD_ctrl(g_hSysMon, 0, BRDctrl_SYSMON_GETVCCINT, &sysmon_data);
			g_fVCCint = sysmon_data.curv;
			g_fVCCintMax = sysmon_data.maxv;
			g_fVCCintMin = sysmon_data.minv;
		}

		//Снятие значения Vccaux
		if(g_isVCCaux)
		{
			BRD_ctrl(g_hSysMon, 0, BRDctrl_SYSMON_GETVCCAUX, &sysmon_data);
			g_fVCCaux = sysmon_data.curv;
			g_fVCCauxMax = sysmon_data.maxv;
			g_fVCCauxMin = sysmon_data.minv;
		}
		
		//Снятие значения Vrefp
		if (g_isVrefp)
		{
			REAL64 val;
			BRD_ctrl(g_hSysMon, 0, BRDctrl_SYSMON_GETVREFP, &val);
			g_fVrefp = val;
		}

		//Снятие значения Vrefn
		if(g_isVrefn)
		{
			REAL64 val;
			BRD_ctrl(g_hSysMon, 0, BRDctrl_SYSMON_GETVREFN, &val);
			g_fVrefn = val;
		}

		//Снятие значения Vccint
		if(g_isVCCbram)
		{
			BRD_ctrl(g_hSysMon, 0, BRDctrl_SYSMON_GETVCCBRAM, &sysmon_data);
			g_fVCCbram = sysmon_data.curv;
			g_fVCCbramMax = sysmon_data.maxv;
			g_fVCCbramMin = sysmon_data.minv;
		}

		PrintResult(); // Вывод результатов
 		
		IPC_delay(1000);
		
		if(IPC_kbhit()) // проверяет была ли нажата клавиша
		{
			int ch = IPC_getch(); // получает клавишу
			if(0x1B == ch) // если Esc
				break;
		}

	}while(g_isMon);

}

// Вывод результатов
void PrintResult()
{
	if(g_isMon)
#ifdef __IPC_WIN__
		system("cls");
#else
		system("clear");
#endif

	DeviceInfo(); //  Информация о базовом модуле

	double err; // Ошибка

	if(g_isTemp)
	{
		BRDC_printf(_BRDC("LOG:\n"));
		BRDC_printf(_BRDC("LOG:\tTemp: %7.3f C (max %.3f,min %.3f)\n"),g_fTemp,g_fTempMax,g_fTempMin);
	}
	BRDC_printf(_BRDC("LOG:\n"));
	BRDC_printf(_BRDC("LOG:\t|         | current, V | error, %% | nominal, V | max, V | min, V |\n"));

	if(g_isVCCint)
	{
	//	err = abs(fVCCint - g_fVCCintNominal) / g_fVCCintNominal * 100.;
		err = fabs(g_fVCCint - g_fVCCintNominal);
		err = err / g_fVCCintNominal;
		err = err * 100.;

		if(g_fVCCintNominal == 0)
			BRDC_printf(_BRDC("LOG:\t| VCCint  | %4.3f      |          | %.3f      | %.3f  | %.3f  |\n"), g_fVCCint, g_fVCCintNominal, g_fVCCintMax,g_fVCCintMin);
		else if(err >= 5)
			BRDC_printf(_BRDC("ERROR:\t| VCCint  | %4.3f      | %.1f !!!  | %.3f      | %.3f  | %.3f  |\n"), g_fVCCint, err, g_fVCCintNominal, g_fVCCintMax,g_fVCCintMin);
		else
			BRDC_printf(_BRDC("LOG:\t| VCCint  | %4.3f      | %.1f      | %.3f      | %.3f  | %.3f  |\n"), g_fVCCint, err, g_fVCCintNominal, g_fVCCintMax,g_fVCCintMin);
	}

	if(g_isVCCaux)
	{
		err = fabs(g_fVCCaux - g_fVCCauxNominal) / g_fVCCauxNominal * 100.;

		if(g_fVCCauxNominal == 0)
			BRDC_printf(_BRDC("LOG:\t| VCCaux  | %4.3f      |          | %.3f      | %.3f  | %.3f  |\n"), g_fVCCaux, g_fVCCauxNominal, g_fVCCauxMax,g_fVCCauxMin);
		else if(err >= 5)
			BRDC_printf(_BRDC("ERROR:\t| VCCaux  | %4.3f      | %.1f !!! | %.3f      | %.3f  | %.3f  |\n"), g_fVCCaux, err, g_fVCCauxNominal, g_fVCCauxMax,g_fVCCauxMin);
		else
			BRDC_printf(_BRDC("LOG:\t| VCCaux  | %4.3f      | %.1f      | %.3f      | %.3f  | %.3f  |\n"), g_fVCCaux, err, g_fVCCauxNominal, g_fVCCauxMax,g_fVCCauxMin);
	}

	if(g_isVrefn)
	{
		err = fabs(g_fVrefn - g_fVrefnNominal) / g_fVrefnNominal * 100.;

		if(g_fVrefnNominal == 0)
			BRDC_printf(_BRDC("LOG:\t| Vrefn   | %4.3f      |          | %.3f      |        |        |\n"),g_fVrefn, g_fVrefnNominal);
		else if(err >= 5)
			BRDC_printf(_BRDC("ERROR:\t| Vrefn   | %4.3f      | %.1f !!! | %.3f      |        |        |\n"),g_fVrefn, err, g_fVrefnNominal);
		else
			BRDC_printf(_BRDC("LOG:\t| Vrefn   | %4.3f      | %.1f      | %.3f        |        |        |\n"),g_fVrefn, err, g_fVrefnNominal);
	}	

	if(g_isVrefp)
	{
		err = fabs(g_fVrefp - g_fVrefpNominal) / g_fVrefpNominal * 100.;

		if(g_fVrefpNominal == 0)
			BRDC_printf(_BRDC("LOG:\t| Vrefp   | %4.3f      |          | %.3f      |        |        |\n"),g_fVrefp, g_fVrefpNominal);
		else if(err >= 5)
			BRDC_printf(_BRDC("ERROR:\t| Vrefp   | %4.3f      | %.1f !!!  | %.3f      |        |        |\n"),g_fVrefp, err, g_fVrefpNominal);
		else
			BRDC_printf(_BRDC("LOG:\t| Vrefp   | %4.3f      | %.1f      | %.3f      |        |        |\n"),g_fVrefp, err, g_fVrefpNominal);
	}	

	if(g_isVCCbram)
	{
	//	err = abs(fVCCint - g_fVCCintNominal) / g_fVCCintNominal * 100.;
		err = fabs(g_fVCCbram - g_fVCCbramNominal);
		err = err / g_fVCCbramNominal;
		err = err * 100.;

		if(g_fVCCbramNominal == 0)
			BRDC_printf(_BRDC("LOG:\t| VCCbram | %4.3f      |          | %.3f      | %.3f  | %.3f  |\n"), g_fVCCbram, g_fVCCbramNominal, g_fVCCbramMax,g_fVCCbramMin);
		else if(err >= 5)
			BRDC_printf(_BRDC("ERROR:\t| VCCbram | %4.3f      | %.1f !!!  | %.3f      | %.3f  | %.3f  |\n"), g_fVCCbram, err, g_fVCCbramNominal, g_fVCCbramMax,g_fVCCbramMin);
		else
			BRDC_printf(_BRDC("LOG:\t| VCCbram | %4.3f      | %.1f      | %.3f      | %.3f  | %.3f  |\n"), g_fVCCbram, err, g_fVCCbramNominal, g_fVCCbramMax,g_fVCCbramMin);
	}

	BRDC_printf(_BRDC("LOG:\n"));

	if(g_isMon && (!g_isExit))
		BRDC_printf(_BRDC("Press ESC key for watching stop...\n"));
}
