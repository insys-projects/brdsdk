
#include	"adc_ctrl.h"

#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
 #include	<string.h>
 #include    "gipcy.h"
static IPC_handle g_hBufFileMap;
static IPC_handle g_hFlgFileMap;
#else
 #include	<conio.h>
 #include	<process.h> 
static HANDLE g_hBufFileMap;
static HANDLE g_hFlgFileMap;
#endif

extern BRD_Handle g_hSRV;
extern ULONG g_MemAsFifo;
extern ULONG g_AdcDrqFlag;
extern ULONG g_MemDrqFlag;

typedef struct _THREAD_PARAM {
	BRD_Handle handle;
	int idx;
} THREAD_PARAM, *PTHREAD_PARAM;

#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
thread_value __IPC_API ContDaqFileMapping(void* pParams);
#else
unsigned __stdcall ContDaqFileMapping(void* pParams);
#endif

static int g_flbreak = 0;

ULONG g_BlkSize;
ULONG g_BlkNum;

static void* g_pBufFileMap;
static ULONG* g_pFlags;

void ContinueDaq(ULONG BlkSize, ULONG BlkNum)
{
	THREAD_PARAM thread_par;

	//SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
	//DWORD prior_class = GetPriorityClass(GetCurrentProcess());
	//BRDC_printf(_BRDC("Process Priority = %d\n"), prior_class);

	g_BlkSize = BlkSize;
	//g_fileBufNum = FileBufNum;
	g_BlkNum = BlkNum;

	g_flbreak = 0;
	thread_par.handle = g_hSRV;
	thread_par.idx = 0;
#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
	IPC_handle hThread = IPC_createThread( _BRDC("ContinueDaq"), &ContDaqFileMapping, &thread_par );
	IPC_waitThread(hThread, INFINITE );// Wait until threads terminates
	IPC_deleteThread(hThread);
#else
	unsigned threadID;
	HANDLE hThread = (HANDLE)_beginthreadex( NULL, 0, &ContDaqFileMapping, &(thread_par), 0, &(threadID) );
    WaitForSingleObject(hThread, INFINITE );    // Wait until threads terminates
    CloseHandle(hThread);
#endif
}

#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
thread_value __IPC_API ContDaqFileMapping(void* pParams)
#else
unsigned __stdcall ContDaqFileMapping(void* pParams)
#endif
{
	S32		status = BRDerr_OK;
	ULONG Status = 0;
	//SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
	//int prior = GetThreadPriority(GetCurrentThread());
	//BRDC_printf(_BRDC("Thread Priority = %d\n"), prior);
	PTHREAD_PARAM pThreadParam = (PTHREAD_PARAM)pParams;
	BRD_Handle hSrv = pThreadParam->handle;
	int idx = pThreadParam->idx;

	BRDctrl_StreamCBufAlloc buf_dscr;
	buf_dscr.dir = BRDstrm_DIR_IN;
	buf_dscr.isCont = 1; // 1 - буфер размещается в системной памяти ПК
	buf_dscr.blkNum = g_BlkNum;
	buf_dscr.blkSize = g_BlkSize;
	buf_dscr.ppBlk = new PVOID[buf_dscr.blkNum];
	status = BRD_ctrl(hSrv, 0, BRDctrl_STREAM_CBUF_ALLOC, &buf_dscr);
	if(!BRD_errcmp(status, BRDerr_OK) && !BRD_errcmp(status, BRDerr_PARAMETER_CHANGED))
	{
		BRDC_printf(_BRDC("ERROR!!! BRDctrl_STREAM_CBUF_ALLOC\n"));
#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
		return (thread_value)status;
#else
		return status;
#endif
	}
	if(BRD_errcmp(status, BRDerr_PARAMETER_CHANGED))
	{
		BRDC_printf(_BRDC("Warning!!! BRDctrl_STREAM_CBUF_ALLOC: BRDerr_PARAMETER_CHANGED\n"));
		status = BRDerr_OK;
	}
	BRDC_printf(_BRDC("Block size = %d Mbytes, Block num = %d\n"), buf_dscr.blkSize / 1024 / 1024, buf_dscr.blkNum);

	BRDCHAR nameBufMap[64] = _BRDC("data_blk0");
	//BRDC_sprintf(nameBufMap, _BRDC("data_blk%d"), i);
	BRDCHAR nameFlagMap[64] = _BRDC("data_flg");
	//BRDC_sprintf(nameFlagMap, _BRDC("data_flg"), i);
#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
		g_hBufFileMap = IPC_createSharedMemory(nameBufMap, g_BlkSize);
		g_pBufFileMap = IPC_mapSharedMemory(g_hBufFileMap);
		g_hFlgFileMap = IPC_createSharedMemory(nameFlagMap, 3* sizeof(ULONG));
		g_pFlags = (ULONG*)IPC_mapSharedMemory(g_hFlgFileMap);
#else
	g_hBufFileMap = CreateFileMapping(INVALID_HANDLE_VALUE,
										NULL, PAGE_READWRITE,
										0, g_BlkSize,
										nameBufMap);
	g_pBufFileMap = (void*)MapViewOfFile(g_hBufFileMap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

	g_hFlgFileMap = CreateFileMapping(INVALID_HANDLE_VALUE,
										NULL, PAGE_READWRITE,
										0, 3* sizeof(ULONG),
										nameFlagMap);
	g_pFlags = (ULONG*)MapViewOfFile(g_hFlgFileMap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
#endif
	g_pFlags[0] = 0;
	g_pFlags[1] = 0;
	g_pFlags[2] = buf_dscr.blkSize;

	// установить источник для работы стрима
	ULONG tetrad;
	if(g_MemAsFifo)
		status = BRD_ctrl(hSrv, 0, BRDctrl_SDRAM_GETSRCSTREAM, &tetrad); // стрим будет работать с SDRAM
	else
		status = BRD_ctrl(hSrv, 0, BRDctrl_ADC_GETSRCSTREAM, &tetrad); // стрим будет работать с АЦП
	status = BRD_ctrl(hSrv, 0, BRDctrl_STREAM_SETSRC, &tetrad);

	// устанавливать флаг для формирования запроса ПДП надо после установки источника (тетрады) для работы стрима
//	ULONG flag = BRDstrm_DRQ_ALMOST; // FIFO почти пустое
//	ULONG flag = BRDstrm_DRQ_READY;
//	ULONG flag = BRDstrm_DRQ_HALF; // рекомендуется флаг - FIFO наполовину заполнено
	ULONG flag = g_AdcDrqFlag;
	if(g_MemAsFifo)
		flag = g_MemDrqFlag;
	status = BRD_ctrl(hSrv, 0, BRDctrl_STREAM_SETDRQ, &flag);

	ULONG Enable = 1;
	status = BRD_ctrl(hSrv, 0, BRDctrl_ADC_FIFORESET, NULL); // сброс FIFO АЦП
	status = BRD_ctrl(hSrv, 0, BRDctrl_STREAM_RESETFIFO, NULL);
	if(g_MemAsFifo)
	{
		status = BRD_ctrl(hSrv, 0, BRDctrl_SDRAM_FIFORESET, NULL); // сборс FIFO SDRAM
		status = BRD_ctrl(hSrv, 0, BRDctrl_SDRAM_ENABLE, &Enable); // разрешение записи в SDRAM
	}

	BRDctrl_StreamCBufStart start_pars;
	start_pars.isCycle = 1; // с зацикливанием
	status = BRD_ctrl(hSrv, 0, BRDctrl_STREAM_CBUF_START, &start_pars); // старт ПДП

#ifdef _WIN32
	// определение скорости сбора данных
	LARGE_INTEGER Frequency, StartPerformCount, StopPerformCount;
	int bHighRes = QueryPerformanceFrequency (&Frequency);
	QueryPerformanceCounter (&StartPerformCount);
#endif
	status = BRD_ctrl(hSrv, 0, BRDctrl_ADC_ENABLE, &Enable); // разрешение работы АЦП

	//int errCnt = 0;
	//BRDC_printf(_BRDC("Data writing into file %s...\n"), fileName);
	int cnt = 0;
	ULONG msTimeout = 40000; // ждать окончания передачи данных до 40 сек.
	do
	{
		status = BRD_ctrl(hSrv, 0, BRDctrl_STREAM_CBUF_WAITBLOCK, &msTimeout);
		if(BRD_errcmp(status, BRDerr_WAIT_TIMEOUT))
		{	// если вышли по тайм-ауту, то остановимся
			status = BRD_ctrl(hSrv, 0, BRDctrl_ADC_FIFOSTATUS, &Status);
			BRDC_printf(_BRDC("ADC FIFO Status = 0x%04X\n"), Status);
			status = BRD_ctrl(hSrv, 0, BRDctrl_STREAM_CBUF_STOP, NULL);
			DisplayError(status, __FUNCTION__, _BRDC("TIME-OUT"));
			break;
		}
		if(!buf_dscr.pStub->lastBlock && !g_pFlags[0])
		{
			memcpy(g_pBufFileMap, buf_dscr.ppBlk[0], g_BlkSize);
			g_pFlags[0] = 0xffffffff;
			g_pFlags[1] = cnt == 0 ? 1 : 0;
			g_pFlags[2] = buf_dscr.blkSize;
		}

#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
		if(!idx && IPC_kbhit())
		{
			int ch = IPC_getch(); // получает клавишу
			if(0x1B == ch) // если Esc
			{
				g_flbreak = 1;
				break; 
			}
		}
#else
		if(!idx && GetAsyncKeyState(VK_ESCAPE))
		{
			g_flbreak = 1;
			_getch();
		}
#endif
		cnt = buf_dscr.pStub->totalCounter;
		if(cnt%16 == 0)
		{
#ifdef _WIN32
			QueryPerformanceCounter (&StopPerformCount);
			double msTime = (double)(StopPerformCount.QuadPart - StartPerformCount.QuadPart) / (double)Frequency.QuadPart * 1.E3;
			//printf("Transfer rate is %.2f Mbytes/sec\r", ((double)g_bBufSize / msTime)/1000.);
			BRDC_printf(_BRDC("Current: block = %d, DAQ & Transfer by bus rate is %.2f Mbytes/sec\r"), cnt, ((double)g_BlkSize*cnt / msTime)/1000.);
#else
			BRDC_printf(_BRDC("Current: block = %d\r"), cnt);
#endif
		}
		status = BRD_ctrl(hSrv, 0, BRDctrl_ADC_FIFOSTATUS, &Status);
		if(Status & 0x80)
			BRDC_printf(_BRDC("\nERROR (%s): ADC FIFO is overflow (ADC FIFO Status = 0x%04X)\n"), nameBufMap, Status);

	}while(!g_flbreak);

	Enable = 0;
	status = BRD_ctrl(hSrv, 0, BRDctrl_ADC_ENABLE, &Enable); // запрет работы АЦП
	if(g_MemAsFifo)
		status = BRD_ctrl(hSrv, 0, BRDctrl_SDRAM_ENABLE, &Enable); // запрет записи в SDRAM
//	printf("                                             \r");
//	if(errCnt)
//		BRDC_printf(_BRDC("ERROR (%s): buffers skiped %d\n"), fileName, errCnt);
#ifdef _WIN32
	double msTime = (double)(StopPerformCount.QuadPart - StartPerformCount.QuadPart) / (double)Frequency.QuadPart * 1.E3;
	BRDC_printf(_BRDC("Total: block = %d, DAQ & Transfer by bus rate is %.2f Mbytes/sec\n"),
										buf_dscr.pStub->totalCounter, ((double)g_BlkSize*cnt / msTime)/1000.);
//	BRDC_printf(_BRDC("Total: Block (%s) = %d, DAQ & Transfer by bus rate is %.2f Mbytes/sec\n"),
//										nameBufMap, buf_dscr.pStub->totalCounter, ((double)g_BlkSize*cnt / msTime)/1000.);
#else
	BRDC_printf(_BRDC("Total: block = %d\n"), buf_dscr.pStub->totalCounter);
#endif
	status = BRD_ctrl(hSrv, 0, BRDctrl_ADC_FIFOSTATUS, &Status);
	if(Status & 0x80)
		BRDC_printf(_BRDC("ERROR (%s): ADC FIFO is overflow\n"), nameBufMap);
	if(g_MemAsFifo)
	{
		status = BRD_ctrl(hSrv, 0, BRDctrl_SDRAM_FIFOSTATUS, &Status);
		if(Status & 0x80)
			BRDC_printf(_BRDC("ERROR (%s): SDRAM FIFO is overflow\n"), nameBufMap);
	}

#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
	IPC_unmapSharedMemory(g_hFlgFileMap);
	IPC_deleteSharedMemory(g_hFlgFileMap);
	IPC_unmapSharedMemory(g_pBufFileMap);
	IPC_deleteSharedMemory(g_pBufFileMap);
#else
	UnmapViewOfFile(g_pFlags);
	CloseHandle(g_hFlgFileMap);
	UnmapViewOfFile(g_pBufFileMap);
	CloseHandle(g_hBufFileMap);
#endif

	status = BRD_ctrl(hSrv, 0, BRDctrl_STREAM_CBUF_FREE, &buf_dscr);
	delete[] buf_dscr.ppBlk;

#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
	return (thread_value)status;
#else
    _endthreadex( 0 );
	return status;
#endif
}
