//********************************************************
//
// Example Application of BRD_xxx functions for PIO service.
//
// (C) InSys by Dorokhin Andrey Oct, 2007
//
//	Apr, 2011 - 64-bits version
//	Jul, 2011 - replace char(TCHAR) on BRDCHAR
//
//=********************************************************

#include	<stdio.h>
#include	<fcntl.h>

#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
#include	<string.h>
 #include    "gipcy.h"
#else
 #include	<windows.h>
 #include	<conio.h>
#endif

#include	"brd.h"
#include	"ctrlpio.h"

const U16 PLD_CFG_TAG	= 0x0500; // tag of PLD

void GetOptions();
BRD_Handle GetSrvHandle(BRD_Handle handle, BRD_ServList* srv);

BRDCHAR g_pldFileName[MAX_PATH];
int g_isPldLoadAlways = 0;
int g_IoDir = 1;
int g_ReadMode = 1;
int g_ReadStrobe = 4;
int g_WriteStrobe = 4;

BRD_Handle g_hPio = 0;

S32 PioWrite();
S32 PioRead();
void CheckPioFlag( int strobeFlow, int ackFlow, int isLVDS, BRD_PioDir* pio_dir);

BRDCHAR iniFileName[] = _BRDC("//exam_pio.ini");

//=************************* main *************************
//=********************************************************
int main( int argc, char *argv[] )
{
	S32		status;

#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
	IPC_initKeyboard();
#endif

	BRD_displayMode(BRDdm_VISIBLE | BRDdm_CONSOLE); // режим вывода информационных сообщений : отображать все уровни на консоле
	
	S32	DevNum;
	status = BRD_init(_BRDC("brd.ini"), &DevNum); // инициализировать библиотеку
//	BRD_Error* pErrInfo = NULL;
//	status = BRD_error(&pErrInfo); // получить информацию об ошибке

	// получить список LID (каждая запись соответствует устройству)
	BRD_LidList lidList;
	lidList.item = 10;
	lidList.pLID = new U32[10];
	status = BRD_lidList(lidList.pLID, lidList.item, &lidList.itemReal);

	GetOptions();

	BRD_Info	info;
	info.size = sizeof(info);
	BRD_Handle handle[10];

	for(ULONG iDev = 0; iDev < lidList.itemReal; iDev++)
	{
		BRD_getInfo(lidList.pLID[iDev], &info); // получить информацию об устройстве
		BRDC_printf(_BRDC("%s, DevID = 0x%x, RevID = 0x%x, Bus = %d, Dev = %d, Slot = %d, PID = %d.\n"), 
			info.name, info.boardType >> 16, info.boardType & 0xff, info.bus, info.dev, info.slot, info.pid);
//		printf("Board: %s, DevID = 0x%x, RevID = 0x%x, BusType = %d, BaseMem = %x.\n", 
//			info.name, info.boardType >> 16, info.boardType & 0xff, info.busType, info.base[0]);
		//handle[iDev] = BRD_open(lidList.pLID[iDev], BRDopen_EXCLUSIVE, NULL); // открыть устройство в монопольном режиме
		handle[iDev] = BRD_open(lidList.pLID[iDev], BRDopen_SHARED, NULL); // открыть устройство в разделяемом режиме
		if(handle[iDev] > 0)
		{
			BRD_PuList PuList[5];
			U32 ItemReal;
			status = BRD_puList(handle[iDev], PuList, 5, &ItemReal); // получить список программируемых устройств
			if(ItemReal <= 5)
			{
				for(U32 j = 0; j < ItemReal; j++)
				{
					if(PuList[j].puCode == PLD_CFG_TAG)
					{ // Тип программируемого устройства - ПЛИС
						BRDC_printf(_BRDC("PU%d: %s, Id = %d, Code = %x, Attr = %x.\n"),
										j, PuList[j].puDescription, PuList[j].puId, PuList[j].puCode, PuList[j].puAttr);
						// получить состояние ПЛИС
						U32	PldState;
						status = BRD_puState(handle[iDev], PuList[j].puId, &PldState);
						BRDC_printf(_BRDC("BRD_puState: PldState = %d\n"), PldState);
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
						}
					}
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
				if(!BRDC_strcmp(srvList[iSrv].name, _BRDC("PIOSTD0")))
				{
					g_hPio = GetSrvHandle(handle[iDev], &srvList[iSrv]);
					PioWrite();
				}
			}
			if(g_hPio)
				status = BRD_release(g_hPio, 0);
			status = BRD_close(handle[iDev]); // закрыть устройство 
			BRDC_printf(_BRDC("\nBRD_close: OK\n"));
		}
	}
	delete lidList.pLID;
	status = BRD_cleanup();
	BRDC_printf(_BRDC("BRD_cleanup: OK\n"));
	BRDC_printf(_BRDC("Press any key for leaving program...\n"));
#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
	IPC_getch();
	IPC_cleanupKeyboard();
#else
	_getch();
#endif
}

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
	default:
		BRDC_sprintf(msg, _BRDC("%s: Unknown error\n"), cmd_str);
		break;
	}
    BRDC_printf(_BRDC("%s"), msg);
}

void GetOptions()
{
	BRDCHAR Buffer[128];
	BRDCHAR iniFilePath[MAX_PATH];
#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
	IPC_getCurrentDir(iniFilePath, sizeof(iniFilePath)/sizeof(BRDCHAR));
	BRDC_strcat(iniFilePath, iniFileName);
	IPC_getPrivateProfileString(_BRDC("Option"), _BRDC("PldFileName"), _BRDC("ambpcd_v10_admddc5016.mcs"), g_pldFileName, sizeof(g_pldFileName), iniFilePath);
	IPC_getPrivateProfileString(_BRDC("Option"), _BRDC("isPldLoadAlways"), _BRDC("0"), Buffer, sizeof(Buffer), iniFilePath);
	g_isPldLoadAlways = BRDC_atoi(Buffer);
	IPC_getPrivateProfileString(_BRDC("Option"), _BRDC("IoDir"), _BRDC("1"), Buffer, sizeof(Buffer), iniFilePath);
	g_IoDir = BRDC_atoi(Buffer);
	IPC_getPrivateProfileString(_BRDC("Option"), _BRDC("ReadMode"), _BRDC("1"), Buffer, sizeof(Buffer), iniFilePath);
	g_ReadMode = BRDC_atoi(Buffer);
	IPC_getPrivateProfileString(_BRDC("Option"), _BRDC("ReadStrobe"), _BRDC("4"), Buffer, sizeof(Buffer), iniFilePath);
	g_ReadStrobe = BRDC_atoi(Buffer);
	IPC_getPrivateProfileString(_BRDC("Option"), _BRDC("WriteStrobe"), _BRDC("4"), Buffer, sizeof(Buffer), iniFilePath);
	g_WriteStrobe = BRDC_atoi(Buffer);
#else
	GetCurrentDirectory(sizeof(iniFilePath)/sizeof(BRDCHAR), iniFilePath);
	BRDC_strcat(iniFilePath, iniFileName);
	GetPrivateProfileString(_BRDC("Option"), _BRDC("PldFileName"), _BRDC("ambpcd_v10_admddc5016.mcs"), g_pldFileName, sizeof(g_pldFileName), iniFilePath);
	GetPrivateProfileString(_BRDC("Option"), _BRDC("isPldLoadAlways"), _BRDC("0"), Buffer, sizeof(Buffer), iniFilePath);
	g_isPldLoadAlways = BRDC_atoi(Buffer);
	GetPrivateProfileString(_BRDC("Option"), _BRDC("IoDir"), _BRDC("1"), Buffer, sizeof(Buffer), iniFilePath);
	g_IoDir = BRDC_atoi(Buffer);
	GetPrivateProfileString(_BRDC("Option"), _BRDC("ReadMode"), _BRDC("1"), Buffer, sizeof(Buffer), iniFilePath);
	g_ReadMode = BRDC_atoi(Buffer);
	GetPrivateProfileString(_BRDC("Option"), _BRDC("ReadStrobe"), _BRDC("4"), Buffer, sizeof(Buffer), iniFilePath);
	g_ReadStrobe = BRDC_atoi(Buffer);
	GetPrivateProfileString(_BRDC("Option"), _BRDC("WriteStrobe"), _BRDC("4"), Buffer, sizeof(Buffer), iniFilePath);
	g_WriteStrobe = BRDC_atoi(Buffer);
#endif

}

BRD_Handle GetSrvHandle(BRD_Handle handle, BRD_ServList* srv)
{
	U32 mode = BRDcapt_EXCLUSIVE;
	BRD_Handle hSrv = BRD_capture(handle, 0, &mode, srv->name, 10000);
	if(mode == BRDcapt_EXCLUSIVE) BRDC_printf(_BRDC("%s Capture mode: EXCLUSIVE\n"), srv->name);
	if(mode == BRDcapt_SPY)	BRDC_printf(_BRDC("%s Capture mode: SPY\n"), srv->name);
	if(hSrv <= 0)
		BRDC_printf(_BRDC("Error %s Capture\n"), srv->name);
	return hSrv;
}

S32 PioWrite()
{
	S32		status;
	U32 pio_type;
	status = BRD_ctrl(g_hPio, 0, BRDctrl_PIO_ISLVDS, &pio_type);
	if(BRD_errcmp(status, BRDerr_OK))
	{
		BRDCHAR buf_type[16];
		if(pio_type)
			BRDC_strcpy(buf_type, _BRDC("LVDS"));
		else
			BRDC_strcpy(buf_type, _BRDC("LVTTL"));
		BRDC_printf(_BRDC("BRDctrl_PIO_ISLVDS: type of PIO is %s\n"), buf_type);
	}
	else
		DisplayError(status, _BRDC("BRDctrl_PIO_ISLVDS"));
	BRD_PioDir pio_dir;
	pio_dir.lbDir = g_IoDir & 0x1;
	pio_dir.hbDir = (g_IoDir >> 1) & 0x1;
	status = BRD_ctrl(g_hPio, 0, BRDctrl_PIO_SETDIR, &pio_dir);
	if(BRD_errcmp(status, BRDerr_OK))
	{
		BRDCHAR buf_lbd[4], buf_hbd[4];
		BRDC_strcpy(buf_lbd, _BRDC("rd"));
		BRDC_strcpy(buf_hbd, _BRDC("rd"));
		if(pio_dir.lbDir)
			BRDC_strcpy(buf_lbd, _BRDC("wr"));
		if(pio_dir.hbDir)
			BRDC_strcpy(buf_hbd, _BRDC("wr"));
		BRDC_printf(_BRDC("BRDctrl_PIO_SETDIR: low byte diir = %d(%s), hi byte dir = %d(%s)\n"), pio_dir.lbDir, buf_lbd, pio_dir.hbDir, buf_hbd);
	}
	else
		DisplayError(status, _BRDC("BRDctrl_PIO_SETDIR"));

	BRD_PioMode pio_mode;
	pio_mode.rdMode = g_ReadMode;
	pio_mode.wrMode = 0;
	pio_mode.strobeFlow = 0;
	pio_mode.ackFlow = 0;
	pio_mode.rdStrobe = g_ReadStrobe;
	pio_mode.wrStrobe = g_WriteStrobe;
	status = BRD_ctrl(g_hPio, 0, BRDctrl_PIO_SETMODE, &pio_mode);
	if(BRD_errcmp(status, BRDerr_OK))
		BRDC_printf(_BRDC("BRDctrl_PIO_SETMODE: pio_mode.rdMode = %d, ReadStrobe = %d, WriteStrobe = %d\n"),
													pio_mode.rdMode, pio_mode.rdStrobe, pio_mode.wrStrobe);
	else
		DisplayError(status, _BRDC("BRDctrl_PIO_SETMODE"));

//	U32 enbl = 1;
//	status = BRD_ctrl(g_hPio, 0, BRDctrl_PIO_ENABLE, &enbl);
	if(pio_dir.lbDir && pio_dir.hbDir)
	{ // pio 16-bit write
		BRDC_printf(_BRDC("Write operation (in external device)\n"));
		U32 pio_data = 0;
		U32 pio_data0 = 0x5A5A;
		U32 pio_data1 = 0xA5A5;
		int fl_break = 0;
		BRDC_printf(_BRDC("Hit any key to abort...\n"));
#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
        while(!fl_break & !IPC_kbhit())
#else
		while(!fl_break & !_kbhit())
#endif
		{
			status = BRD_ctrl(g_hPio, 0, BRDctrl_PIO_WRITEDATA, &pio_data0);
			if(!BRD_errcmp(status, BRDerr_OK))
			{
				DisplayError(status, _BRDC("BRDctrl_PIO_WRITEDATA"));
				fl_break = 1;
			}
			status = BRD_ctrl(g_hPio, 0, BRDctrl_PIO_READDATA, &pio_data);
			pio_data &= 0xffff;
			BRDC_printf(_BRDC("read pio_data = 0x%X\r"), pio_data);
#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
			IPC_delay(1);
#else
			Sleep(1);
#endif
			U32 pio_data = 0;
			status = BRD_ctrl(g_hPio, 0, BRDctrl_PIO_WRITEDATA, &pio_data1);
			if(!BRD_errcmp(status, BRDerr_OK))
			{
				DisplayError(status, _BRDC("BRDctrl_PIO_WRITEDATA"));
				fl_break = 1;
			}
			status = BRD_ctrl(g_hPio, 0, BRDctrl_PIO_READDATA, &pio_data);
			pio_data &= 0xffff;
			BRDC_printf(_BRDC("read pio_data = 0x%X\r"), pio_data);
#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
			IPC_delay(1);
#else
			Sleep(1);
#endif
		}
		if(!fl_break)
#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
			IPC_getch();
#else
			_getch();
#endif
	}
	else
		if(!pio_dir.lbDir && !pio_dir.hbDir)
		{ // pio 16-bit read
			BRDC_printf(_BRDC("Read operation (from external device)\n"));
			U32 pio_data = 0;
			//U32 pio_data0 = 0x5A;
			//U32 pio_data1 = 0xA5;
			int fl_break = 0;
			BRDC_printf(_BRDC("Hit any key to abort...\n"));
#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
	        while(!fl_break & !IPC_kbhit())
#else
			while(!fl_break & !_kbhit())
#endif
			{
				status = BRD_ctrl(g_hPio, 0, BRDctrl_PIO_READDATA, &pio_data);
				if(!BRD_errcmp(status, BRDerr_OK))
				{
					DisplayError(status, _BRDC("BRDctrl_PIO_READDATA"));
					fl_break = 1;
				}
				pio_data &= 0xffff;
				BRDC_printf(_BRDC("read pio_data = 0x%X\r"), pio_data);
#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
				IPC_delay(1);
#else
				Sleep(1);
#endif
			}
			if(!fl_break)
#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
				IPC_getch();
#else
				_getch();
#endif
		}
		else
		{
			BRDC_printf(_BRDC("Write/Read operation (use service connector)\n"));
			U32 rd_data = 0;
			U32 wr_data = 0;
			//if(pio_dir.lbDir)
			//	wr_data = 0x005A;
			//else
			//	wr_data = 0x5A00;
			int err_cnt = 0;
//			int fl_break = 0;
//			printf("Hit any key to abort...\n");
//			while(!fl_break & !_kbhit())

			if(pio_type)
			{ // LVDS
                for(unsigned i = 0; i < 16; i++)
				{
					wr_data = i;
					if(pio_dir.hbDir)
						wr_data = wr_data << 4;
					status = BRD_ctrl(g_hPio, 0, BRDctrl_PIO_WRITEDATA, &wr_data);
#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
					IPC_delay(2);
#else
					Sleep(2);
#endif
					status = BRD_ctrl(g_hPio, 0, BRDctrl_PIO_READDATA, &rd_data);
					if(pio_dir.lbDir)
						rd_data = (rd_data >> 4) & 0xf;
					else
						rd_data = rd_data & 0xf;
					rd_data &= 0xff;
					if(rd_data != i)
						err_cnt++;
					BRDC_printf(_BRDC("write pio_data = 0x%X  read pio_data = 0x%X  error count = %d\r"), i, rd_data, err_cnt);
					//_getch();
				}
			}
			else
			{ // LVTTL
                for(unsigned i = 0; i < 256; i++)
				{
					wr_data = i;
					if(pio_dir.hbDir)
						wr_data = wr_data << 8;
					status = BRD_ctrl(g_hPio, 0, BRDctrl_PIO_WRITEDATA, &wr_data);
#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
					IPC_delay(2);
#else
					Sleep(2);
#endif
					status = BRD_ctrl(g_hPio, 0, BRDctrl_PIO_READDATA, &rd_data);
					if(pio_dir.lbDir)
						rd_data = (rd_data >> 8) & 0xff;
					else
						rd_data = rd_data & 0xff;
					rd_data &= 0xffff;
					if(rd_data != i)
						err_cnt++;
					BRDC_printf(_BRDC("write pio_data = 0x%X  read pio_data = 0x%X  error count = %d\r"), i, rd_data, err_cnt);
					//wr_data++;
				}
			}
			if(g_ReadMode == 1)
			{
				BRDC_printf(_BRDC("\nCheck PIO Flag\n" ));
				if(pio_type)
				{
					CheckPioFlag(0, 0, pio_type, &pio_dir);
					CheckPioFlag(0, 1, pio_type, &pio_dir);
					CheckPioFlag(1, 0, pio_type, &pio_dir);
					CheckPioFlag(1, 1, pio_type, &pio_dir);
				}
				else
				{
					CheckPioFlag(0, 0, pio_type, &pio_dir);
				}
			}
			//if(!fl_break)
			//	_getch();

		}
	return status;
}

void CheckPioFlag( int strobeFlow, int ackFlow, int isLVDS, BRD_PioDir* pio_dir) 
{

    U32				status, nAckRd, nAckWr, Status;
	U32				nAckRdIn, nAckWrIn;

	BRD_PioMode pio_mode;
	pio_mode.rdMode = g_ReadMode;
	pio_mode.wrMode = 0;
	pio_mode.strobeFlow = strobeFlow;
	pio_mode.ackFlow = ackFlow;
	pio_mode.rdStrobe = g_ReadStrobe;
	pio_mode.wrStrobe = g_WriteStrobe;
	status = BRD_ctrl(g_hPio, 0, BRDctrl_PIO_SETMODE, &pio_mode);
	//if(BRD_errcmp(status, BRDerr_OK))
	//	BRDC_printf(_BRDC("BRDctrl_PIO_SETMODE: pio_mode.rdMode = %d, ReadStrobe = %d, WriteStrobe = %d\n"),
	//												pio_mode.rdMode, pio_mode.rdStrobe, pio_mode.wrStrobe);
	//else
	//	DisplayError(status, _BRDC("BRDctrl_PIO_SETMODE"));

	//DRV_ctrl( g_hDrv, DDDcmd_KNOTCLRFLAG, &flg );
	status = BRD_ctrl(g_hPio, 0, BRDctrl_PIO_FLAGCLR, NULL);

	status = BRD_ctrl(g_hPio, 0, BRDctrl_PIO_STATUS, &Status);
	//status = (*g_pStatus) & 0xFFFF;

	if(isLVDS == 1) 
	{
		BRDC_printf(_BRDC("        strobeFlow=%d ackFlow=%d status=%.4x \n"), 
			strobeFlow,
			ackFlow,
			Status );
	} 
	else 
	{
		BRDC_printf(_BRDC("        status=%.4x \n"), 
			Status );
	}

	U32 wr_data = 1;
    if(pio_dir->hbDir) {
		if(isLVDS == 1) 
			wr_data = wr_data << 4;
		else
			wr_data = wr_data << 8;
    }
	status = BRD_ctrl(g_hPio, 0, BRDctrl_PIO_WRITEDATA, &wr_data);


	status = BRD_ctrl(g_hPio, 0, BRDctrl_PIO_STATUS, &Status);
	
	//status=(*g_pStatus) & 0xFFFF;

	nAckWrIn = (Status&0x800) >>11;
	nAckRdIn = (Status&0x1000)>>12;

	if(isLVDS == 1) 
	{  //LVDS
		if( (strobeFlow == 0) && (ackFlow == 0) )
		{
			nAckRd = 1;
			nAckWr = 0;
		}
		else 
			if( (strobeFlow == 0) && (ackFlow == 1) )
			{
				nAckRd = 0;
				nAckWr = 1;
			}
			else 
				if( (strobeFlow == 1) && (ackFlow == 0) )
				{
					nAckRd = 0;
					nAckWr = 1;
				}
				else 
				{
					nAckRd = 1;
					nAckWr = 0;
				}

		BRDC_printf(_BRDC("write:  strobeFlow=%d ackFlow=%d status=%.4x"), 
			strobeFlow,
			ackFlow,
			Status );
		BRDC_printf(_BRDC(" AckRd= %d AckWr= %d"),nAckRdIn, nAckWrIn);

		if((nAckRd==nAckRdIn) && (nAckWr==nAckWrIn))
			BRDC_printf(_BRDC(" - Ok\n"));
		else
			BRDC_printf(_BRDC(" - ERROR\n Expected AckRd= %d AckWr= %d\n"),nAckRd, nAckWr); 

	}
	else 
	{ //LVTTL
		nAckRd = 1;
		nAckWr = 0;

		BRDC_printf(_BRDC("write:  status=%.4x"), Status );

		BRDC_printf(_BRDC(" AckRd= %d AckWr= %d"),nAckRdIn, nAckWrIn);

		if((nAckRd==nAckRdIn) && (nAckWr==nAckWrIn))
			BRDC_printf(_BRDC(" - Ok\n"));
		else
			BRDC_printf(_BRDC(" - ERROR\n Expected AckRd= %d AckWr= %d\n"),nAckRd, nAckWr); 
	}

	//DRV_ctrl( g_hDrv, DDDcmd_KNOTCLRFLAG, &flg );
	status = BRD_ctrl(g_hPio, 0, BRDctrl_PIO_FLAGCLR, NULL);

	status = BRD_ctrl(g_hPio, 0, BRDctrl_PIO_STATUS, &Status);

	//status=(*g_pStatus) & 0xFFFF;

	if(isLVDS == 1) 
	{
		BRDC_printf(_BRDC("        strobeFlow=%d ackFlow=%d status=%.4x \n"), 
			strobeFlow,
			ackFlow,
			Status );
	} 
	else 
	{
		BRDC_printf(_BRDC("        status=%.4x \n"), 
			Status );
	}

	U32 rd_data = 0;
	status = BRD_ctrl(g_hPio, 0, BRDctrl_PIO_READDATA, &rd_data);

	if( isLVDS==1 ) 
	{
		if(pio_dir->lbDir)
			rd_data = (rd_data >> 4) & 0xf;
		else
			rd_data = rd_data & 0xf;
		rd_data &= 0xff;
	}
	else
	{
		if(pio_dir->lbDir)
			rd_data = (rd_data >> 8) & 0xff;
		else
			rd_data = rd_data & 0xff;
		rd_data &= 0xffff;
	}
	status = BRD_ctrl(g_hPio, 0, BRDctrl_PIO_STATUS, &Status);
	//status=(*g_pStatus) & 0xFFFF;

	nAckWrIn = (Status&0x800) >>11;
	nAckRdIn = (Status&0x1000)>>12;

	if( isLVDS==1 ) 
	{
		if( (strobeFlow==0) && (ackFlow==0) )
		{
			nAckRd = 0;
			nAckWr = 1;
		}
		else
			if( (strobeFlow==0) && (ackFlow==1) )
			{
				nAckRd = 1;
				nAckWr = 0;
			}
			else 
				if( (strobeFlow==1) && (ackFlow==0) )
				{
					nAckRd = 1;
					nAckWr = 0;
				}
				else 
				{
					nAckRd = 0;
					nAckWr = 1;
				}


		BRDC_printf(_BRDC("read:   strobeFlow=%d ackFlow=%d status=%.4x"), 
			strobeFlow,
			ackFlow,
			Status );

		BRDC_printf(_BRDC(" AckRd= %d AckWr= %d"),nAckRdIn, nAckWrIn);

		if((nAckRd==nAckRdIn) && (nAckWr==nAckWrIn))
			BRDC_printf(_BRDC(" - Ok\n"));
		else
			BRDC_printf(_BRDC(" - ERROR\n Expected AckRd= %d AckWr= %d\n"),nAckRd, nAckWr); 

	} 
	else 
	{
		nAckRd = 0;
		nAckWr = 1;

		BRDC_printf(_BRDC("read:   status=%.4x"), Status );


		BRDC_printf(_BRDC(" AckRd= %d AckWr= %d"),nAckRdIn, nAckWrIn);

		if((nAckRd==nAckRdIn) && (nAckWr==nAckWrIn))
			BRDC_printf(_BRDC(" - Ok\n"));
		else
			BRDC_printf(_BRDC(" - ERROR\n Expected AckRd= %d AckWr= %d\n"),nAckRd, nAckWr); 
	}

	status = BRD_ctrl(g_hPio, 0, BRDctrl_PIO_FLAGCLR, NULL);
	//DRV_ctrl( g_hDrv, DDDcmd_KNOTCLRFLAG, &flg );
}

//
// End of file
//
