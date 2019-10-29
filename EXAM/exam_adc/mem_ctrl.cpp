
#include	"adc_ctrl.h"

#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
 #include	"gipcy.h"
#endif

#ifdef _WIN32
#include	<process.h> 
#endif

extern BRD_Handle g_hSRV;
extern ULONG g_MemDrqFlag;
extern unsigned long long g_bMemBufSize;
extern ULONG g_MsTimeout;

extern int g_transRate;

extern int g_PretrigMode;
extern long long g_bPostTrigSize;

// установить параметры SDRAM
S32 SdramSettings(ULONG mem_mode, BRD_Handle hADC, unsigned long long& bBuf_size)
{
	S32		status;

	ULONG target = 2; // будем осуществлять сбор данных в память
	status = BRD_ctrl(hADC, 0, BRDctrl_ADC_SETTARGET, &target);

	if(mem_mode == 2)
	{	// as a FIFO
		ULONG fifo_mode = 1; // память используется как FIFO
		status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_SETFIFOMODE, &fifo_mode);
	}
	else
	{
		ULONG fifo_mode = 0; // память используется как память
		status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_SETFIFOMODE, &fifo_mode);

		ULONG rd_mode = 0; // можно применять только автоматический режим
		status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_SETREADMODE, &rd_mode);

		ULONG addr = 0;
		status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_SETSTARTADDR, &addr); // установить адрес записи
		status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_SETREADADDR, &addr); // установить адрес чтения

		ULONG mem_size = ULONG(bBuf_size >> 2); // получить размер активной зоны в 32-разрядных словах
		status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_SETMEMSIZE, &mem_size);
		bBuf_size = __int64(mem_size) << 2; // получить фактический размер активной зоны в байтах
		if(BRD_errcmp(status, BRDerr_OK))
			BRDC_printf(_BRDC("BRDctrl_SDRAM_SETMEMSIZE: SDRAM buffer size = %lld bytes\n"), bBuf_size);
		else
			DisplayError(status, __FUNCTION__, _BRDC("BRDctrl_SDRAM_SETMEMSIZE"));

		if(g_PretrigMode == 3)
		{
			ULONG post_size = ULONG(g_bPostTrigSize >> 2);
			status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_SETPOSTTRIGER, &post_size);
			g_bPostTrigSize = (long long)post_size << 2;
			if(BRD_errcmp(status, BRDerr_OK))
				BRDC_printf(_BRDC("BRDctrl_SDRAM_SETPOSTTRIGER: Post-trigger size = %lld bytes\n"), g_bPostTrigSize);
			else
				DisplayError(status, __FUNCTION__, _BRDC("BRDctrl_SDRAM_SETPOSTTRIGER"));
		}		
	}

/*	BRD_AdcSpec adcSpec;
	adcSpec.command = FM412X500Mcmd_RESETEVENTCNT;
	status = BRD_ctrl(hADC, 0, BRDctrl_ADC_SETSPECIFIC,  &adcSpec); 
	if(!BRD_errcmp(status, BRDerr_OK))
		DisplayError(status, __FUNCTION__, _BRDC("FM412X500Mcmd_GETEVENTCNT"));*/
	return status;
}

// выполнить сбор данных в SDRAM с программным методом передачи в ПК
//S32 DaqIntoSdram(BRD_Handle hADC, PVOID pSig, ULONG bBufSize, int num_chan)
S32 DaqIntoSdram(BRD_Handle hADC)
{
	S32		status;
	ULONG Status = 0;
	ULONG Enable = 1;

	status = BRD_ctrl(hADC, 0, BRDctrl_ADC_FIFORESET, NULL); // сборс FIFO АЦП
	status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_FIFORESET, NULL); // сборс FIFO SDRAM
	status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_ENABLE, &Enable); // разрешение записи в SDRAM
	status = BRD_ctrl(hADC, 0, BRDctrl_ADC_ENABLE, &Enable); // разрешение работы АЦП

	// дожидаемся окончания сбора
	do {
//		status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_FIFOSTATUS, &Status);
		status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_ISACQCOMPLETE, &Status);
	} while(!Status);
//	} while(Status & 0x10);

	Enable = 0;
	status = BRD_ctrl(hADC, 0, BRDctrl_ADC_ENABLE, &Enable); // запрет работы АЦП
	status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_ENABLE, &Enable); // запрет записи в SDRAM

	//BRD_DataBuf data_buf;
	//data_buf.pData = pSig;
	//data_buf.size = bBufSize;
	//status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_GETDATA, &data_buf);

	return status;
}

ULONG evt_status = BRDerr_OK;

#ifdef _WIN32

typedef struct _THREAD_PARAM {
	BRD_Handle handle;
	int idx;
} THREAD_PARAM, *PTHREAD_PARAM;

HANDLE g_hThread = NULL;
THREAD_PARAM thread_par;
HANDLE g_hUserEvent = NULL;
// выполнить сбор данных в SDRAM с ПДП-методом передачи в ПК
// с использованием прерывания по окончанию сбора
unsigned __stdcall ThreadDaqIntoSdramDMA(void* pParams)
{
	S32		status;
	ULONG AdcStatus = 0;
	ULONG Status = 0;
	ULONG isAcqComplete = 0;
	evt_status = BRDerr_OK;
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
	PTHREAD_PARAM pThreadParam = (PTHREAD_PARAM)pParams;
	BRD_Handle hADC = pThreadParam->handle;
	int idx = pThreadParam->idx;

	// определение скорости сбора данных
	LARGE_INTEGER Frequency, StartPerformCount, StopPerformCount;
	int bHighRes = QueryPerformanceFrequency (&Frequency);

	ULONG Enable = 1;

	status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_IRQACQCOMPLETE, &Enable); // разрешение прерывания от флага завершения сбора в SDRAM
	//status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_FIFOSTATUS, &Status);

	status = BRD_ctrl(hADC, 0, BRDctrl_ADC_FIFORESET, NULL); // сборс FIFO АЦП
	status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_FIFORESET, NULL); // сборс FIFO SDRAM
	status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_ENABLE, &Enable); // разрешение записи в SDRAM

	QueryPerformanceCounter (&StartPerformCount);
	status = BRD_ctrl(hADC, 0, BRDctrl_ADC_ENABLE, &Enable); // разрешение работы АЦП

	// дожидаемся окончания сбора
	BRD_WaitEvent waitEvt;
	waitEvt.timeout = g_MsTimeout; // ждать окончания сбора данных до g_MsTimeout мсек.
	waitEvt.hAppEvent = g_hUserEvent;
	status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_WAITACQCOMPLETEEX, &waitEvt);
	QueryPerformanceCounter (&StopPerformCount);
	if(BRD_errcmp(status, BRDerr_WAIT_TIMEOUT))
	{	// если вышли по тайм-ауту
		evt_status = status;
		status = BRD_ctrl(hADC, 0, BRDctrl_ADC_FIFOSTATUS, &AdcStatus);
		status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_FIFOSTATUS, &Status);
		status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_ISACQCOMPLETE, &isAcqComplete);
		//BRDCHAR msg[255];
		BRDC_printf(_BRDC("\nBRDctrl_SDRAM_WAITACQCOMPLETE is TIME-OUT(%d sec.)\n    AdcFifoStatus = %08X SdramFifoStatus = %08X\n"),
															waitEvt.timeout/1000, AdcStatus, Status);
		//DisplayError(evt_status, __FUNCTION__, msg);
		// получить реальное число собранных данных
		ULONG acq_size;
		status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_GETACQSIZE, &acq_size);
		unsigned __int64 bRealSize = (unsigned __int64)acq_size << 2; // запомнить в байтах
		BRDC_printf(_BRDC("    isAcqComplete=%d, DAQ real size = %d kByte\n"), isAcqComplete, (ULONG)(bRealSize / 1024));
	}
	//evt_status = status;

	Enable = 0;

	status = BRD_ctrl(hADC, 0, BRDctrl_ADC_ENABLE, &Enable); // запрет работы АЦП
	status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_ENABLE, &Enable); // запрет записи в SDRAM

	status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_IRQACQCOMPLETE, &Enable); // запрет прерывания от флага завершения сбора в SDRAM

//	CloseHandle(g_hUserEvent);

	if(!BRD_errcmp(evt_status, BRDerr_OK))
//	if(BRD_errcmp(evt_status, BRDerr_SIGNALED_APPEVENT))
//	if(evt_status == BRDerr_SIGNALED_APPEVENT)
		return evt_status;

	double msTime = (double)(StopPerformCount.QuadPart - StartPerformCount.QuadPart) / (double)Frequency.QuadPart * 1.E3;
	if(g_transRate)
		printf("DAQ into board memory rate is %.2f Mbytes/sec\r", ((double)g_bMemBufSize / msTime)/1000.);

	// установить, что стрим работает с памятью
	ULONG tetrad;
	status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_GETSRCSTREAM, &tetrad);
	status = BRD_ctrl(hADC, 0, BRDctrl_STREAM_SETSRC, &tetrad);

	// устанавливать флаг для формирования запроса ПДП надо после установки источника (тетрады) для работы стрима
//	ULONG flag = BRDstrm_DRQ_ALMOST; // FIFO почти пустое
//	ULONG flag = BRDstrm_DRQ_READY;
//	ULONG flag = BRDstrm_DRQ_HALF; // рекомендуется флаг - FIFO наполовину заполнено
	ULONG flag = g_MemDrqFlag;
	status = BRD_ctrl(hADC, 0, BRDctrl_STREAM_SETDRQ, &flag);
	if(!BRD_errcmp(status, BRDerr_OK))
		DisplayError(status, __FUNCTION__, _BRDC("BRDctrl_STREAM_SETDRQ"));

	BRD_ctrl(hADC, 0, BRDctrl_STREAM_RESETFIFO, NULL);

//	BRDctrl_StreamCBufStart start_pars;
//	start_pars.isCycle = 0; // без зацикливания 
//	status = BRD_ctrl(hADC, 0, BRDctrl_STREAM_CBUF_START, &start_pars);
////	ULONG msTimeout = 20000; // ждать окончания передачи данных до 20 сек.
////	status = BRD_ctrl(hADC, 0, BRDctrl_STREAM_CBUF_WAITBUF, &msTimeout);
//	waitEvt.timeout = 20000; // ждать окончания сбора данных до 20 сек.
//	waitEvt.hAppEvent = g_hUserEvent;
//	status = BRD_ctrl(hADC, 0, BRDctrl_STREAM_CBUF_WAITBUFEX, &waitEvt);
//	if(BRD_errcmp(status, BRDerr_WAIT_TIMEOUT))
//	{	// если вышли по тайм-ауту, то остановимся
//		evt_status = status;
//		status = BRD_ctrl(hADC, 0, BRDctrl_STREAM_CBUF_STOP, NULL);
//		DisplayError(status, __FUNCTION__, "TIME-OUT");
//	}
//	evt_status = status;
//	if(BRD_errcmp(evt_status, BRDerr_SIGNALED_APPEVENT))
//		status = BRD_ctrl(hADC, 0, BRDctrl_STREAM_CBUF_STOP, NULL);
//	if(!BRD_errcmp(evt_status, BRDerr_OK))
//		return evt_status;

/*	long long event_counter = 0;
	BRD_AdcSpec adcSpec;
	adcSpec.command = FM412X500Mcmd_GETEVENTCNT;
	adcSpec.arg= (PVOID)&event_counter; 
	status = BRD_ctrl(hADC, 0, BRDctrl_ADC_SETSPECIFIC,  &adcSpec); 
	if(!BRD_errcmp(status, BRDerr_OK))
		DisplayError(status, __FUNCTION__, _BRDC("FM412X500Mcmd_GETEVENTCNT"));
	else
		BRDC_printf(_BRDC("FM412X500Mcmd_GETEVENTCNT: %I64u\n"), event_counter);*/
	evt_status = status;
	return status;
}

// создает событие и запускает тред
S32 StartDaqIntoSdramDMA(BRD_Handle hADC, int idx)
{
	S32		status = BRDerr_OK;
    unsigned threadID;

	thread_par.handle = hADC;
	thread_par.idx = idx;
	g_hUserEvent = CreateEvent( 
							NULL,   // default security attributes
							FALSE,  // auto-reset event object
							FALSE,  // initial state is nonsignaled
							NULL);  // unnamed object

	// Create thread
	g_hThread = (HANDLE)_beginthreadex( NULL, 0, &ThreadDaqIntoSdramDMA, &thread_par, 0, &threadID );
	return 1;
}

// проверяет завершение треда
S32 CheckDaqIntoSdramDMA()
{
	// check for terminate thread
    ULONG ret = WaitForSingleObject(g_hThread, 0);
	if(ret == WAIT_TIMEOUT)
		return 0;
	return 1;
}

// прерывает исполнение треда, находящегося в ожидании завершения сбора данных в SDRAM или передачи их по ПДП
void BreakDaqIntoSdramDMA()
{
	SetEvent(g_hUserEvent); // установить в состояние Signaled
    WaitForSingleObject(g_hThread, INFINITE); // Wait until thread terminates
//	CloseHandle(g_hUserEvent);
//	CloseHandle(g_hThread);
}

// Эта функция должна вызываться ТОЛЬКО когда тред уже закончил исполняться 
S32 EndDaqIntoSdramDMA()
{
	CloseHandle(g_hUserEvent);
	CloseHandle(g_hThread);
	g_hUserEvent = NULL;
	g_hThread = NULL;
	return evt_status;
}
#endif

// выполнить сбор данных в SDRAM с ПДП-методом передачи в ПК
// без использования прерывания по окончанию сбора
S32 DaqIntoSdramDMA(BRD_Handle hADC)
{
//	printf("DAQ into SDRAM\n");
	S32		status;
	ULONG Status = 0;
//	ULONG isAcqComplete = 0;
	ULONG Enable = 1;

	//status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_FIFOSTATUS, &Status);

	status = BRD_ctrl(hADC, 0, BRDctrl_ADC_FIFORESET, NULL); // сброс FIFO АЦП
	if(!BRD_errcmp(status, BRDerr_OK))
		DisplayError(status, __FUNCTION__, _BRDC("BRDctrl_ADC_FIFORESET"));
	status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_FIFORESET, NULL); // сброс FIFO SDRAM
	if(!BRD_errcmp(status, BRDerr_OK))
		DisplayError(status, __FUNCTION__, _BRDC("BRDctrl_SDRAM_FIFORESET"));
	status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_ENABLE, &Enable); // разрешение записи в SDRAM
	if(!BRD_errcmp(status, BRDerr_OK))
		DisplayError(status, __FUNCTION__, _BRDC("BRDctrl_SDRAM_ENABLE"));
	status = BRD_ctrl(hADC, 0, BRDctrl_ADC_ENABLE, &Enable); // разрешение работы АЦП
	if(!BRD_errcmp(status, BRDerr_OK))
		DisplayError(status, __FUNCTION__, _BRDC("BRDctrl_ADC_ENABLE"));

	ULONG cntTimeout = g_MsTimeout / 50;
	evt_status = BRDerr_WAIT_TIMEOUT;
	// дожидаемся окончания сбора
	for(ULONG i = 0; i < cntTimeout; i++)
	{
		//status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_FIFOSTATUS, &Status);
		status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_ISACQCOMPLETE, &Status);
		if(!BRD_errcmp(status, BRDerr_OK))
			DisplayError(status, __FUNCTION__, _BRDC("BRDctrl_SDRAM_ISACQCOMPLETE"));
		if(Status)	{evt_status = BRDerr_OK; break;}
		//if(!(Status & 0x10)) break;
#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
		IPC_delay(50);
#else
		Sleep(50);
#endif
	}
	if (BRD_errcmp(evt_status, BRDerr_WAIT_TIMEOUT))
	{
		ULONG AdcStatus = 0;
		ULONG SdramStatus = 0;
		status = BRD_ctrl(hADC, 0, BRDctrl_ADC_FIFOSTATUS, &AdcStatus);
		status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_FIFOSTATUS, &SdramStatus);
		BRDC_printf(_BRDC("\nBRDctrl_SDRAM_ISACQCOMPLETE is TIME-OUT(%d sec.)\n    AdcFifoStatus = %08X SdramFifoStatus = %08X\n"),
			g_MsTimeout / 1000, AdcStatus, SdramStatus);
		return evt_status;
	}

	Enable = 0;
	status = BRD_ctrl(hADC, 0, BRDctrl_ADC_ENABLE, &Enable); // запрет работы АЦП
	status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_ENABLE, &Enable); // запрет записи в SDRAM

	//if(g_PretrigMode == 3)
	//{
	//	// получить параметры, актуальные в режиме претриггера
	//	status = GetPostrigData(hADC);
	//}

	// установить, что стрим работает с памятью
	ULONG tetrad;
	status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_GETSRCSTREAM, &tetrad);
	status = BRD_ctrl(hADC, 0, BRDctrl_STREAM_SETSRC, &tetrad);

	// устанавливать флаг для формирования запроса ПДП надо после установки источника (тетрады) для работы стрима
//	ULONG flag = BRDstrm_DRQ_ALMOST; // FIFO почти пустое
//	ULONG flag = BRDstrm_DRQ_READY;
//	ULONG flag = BRDstrm_DRQ_HALF; // рекомендуется флаг - FIFO наполовину заполнено
	ULONG flag = g_MemDrqFlag;
	status = BRD_ctrl(hADC, 0, BRDctrl_STREAM_SETDRQ, &flag);
	if(!BRD_errcmp(status, BRDerr_OK))
		DisplayError(status, __FUNCTION__, _BRDC("BRDctrl_STREAM_SETDRQ"));

	BRD_ctrl(hADC, 0, BRDctrl_STREAM_RESETFIFO, NULL);

	//status = BRD_ctrl(hADC, 0, BRDctrl_STREAM_RESETFIFO, NULL);
	//if(!BRD_errcmp(status, BRDerr_OK))
	//	DisplayError(status, __FUNCTION__, _BRDC("BRDctrl_STREAM_RESETFIFO"));

	return status;
}

// получить данные из памяти
S32 DataFromMem(BRD_Handle hADC, PVOID pBuf, ULONG bBufSize, ULONG DmaOn)
{
	S32		status;
	if(DmaOn)
	{
		BRDctrl_StreamCBufStart start_pars;
		start_pars.isCycle = 0; // без зацикливания 
		status = BRD_ctrl(hADC, 0, BRDctrl_STREAM_CBUF_START, &start_pars); // стартуем передачу данных из памяти устройства в ПК
		if(!BRD_errcmp(status, BRDerr_OK))
			DisplayError(status, __FUNCTION__, _BRDC("BRDctrl_STREAM_CBUF_START"));
		else
		{
			ULONG msTimeout = 5000; // ждать окончания передачи данных до 5 сек.
			status = BRD_ctrl(hADC, 0, BRDctrl_STREAM_CBUF_WAITBUF, &msTimeout);
			if(BRD_errcmp(status, BRDerr_WAIT_TIMEOUT))
			{	// если вышли по тайм-ауту, то остановимся
				status = BRD_ctrl(hADC, 0, BRDctrl_STREAM_CBUF_STOP, NULL);
				DisplayError(status, __FUNCTION__, _BRDC("TIME-OUT"));
			}
		}
	}
	else
	{
		BRD_DataBuf data_buf;
		data_buf.pData = pBuf;
		data_buf.size = bBufSize;
		status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_GETDATA, &data_buf);
	}
	return status;
}
