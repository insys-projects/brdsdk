//********************************************************
// workmode.cpp
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
#include	"reg_rw_spd.h"

#define		FIFO_WIDTHB		64

//=********************* WorkMode *************************
//=********************************************************
S32 WorkMode( void )
{

	switch( g_nWorkMode )
	{
		case 0:
			WorkMode0();
			break;
		case 1:
			WorkMode1();
			break;
		case 2:
			WorkMode2();
			break;
		case 3:
			WorkMode3();
			break;
		case 4:
			WorkMode4();
			break;
		case 5:
			WorkMode5();
			break;
		case 6:
			WorkMode6();
			break;
		case 7:
			WorkMode7();
			break;
		case 8:
			WorkMode8();
			break;
	}
	return 0;
}

//=********************* WorkMode0 ************************
//=********************************************************
S32 WorkMode0( void )
{
	int				ii;
	volatile SBIG	tmp;
	S32				nblock;

    BRDC_printf( _BRDC("\nWorkMode 0: -- One time  CPU (FIFO) -- \n\n"));

	for( ii=0; ii<g_nDacNum; ii++ )
	{
		//
		// Скорректировать количество отсчетов на канал
		//
		g_aDac[ii].outBufSizeb = g_nSamplesPerChannel 
			                   * g_aDac[ii].sampleSizeb 
							   * g_aDac[ii].chanNum;

		if (g_aDac[ii].outBufSizeb > 0x7FFFFFFF)
			g_aDac[ii].outBufSizeb = 0x7FFFFFFF;

		tmp = g_aDac[ii].outBufSizeb / FIFO_WIDTHB;
		g_aDac[ii].outBufSizeb = tmp * FIFO_WIDTHB;
		if( g_aDac[ii].outBufSizeb > (S32)g_aDac[ii].nFifoSizeb )
		{
			g_aDac[ii].outBufSizeb = g_aDac[ii].nFifoSizeb;
            BRDC_printf( _BRDC("WARNING: OutBufSizeb > DacFifoSizeb !\n"));
		}

		g_aDac[ii].samplesPerChannel = g_aDac[ii].outBufSizeb  
									 / g_aDac[ii].sampleSizeb 
									 / g_aDac[ii].chanNum;

		BRDC_printf( _BRDC("OutBufSizeb       = %lld (0x%llX)\n"), (S64)g_aDac[ii].outBufSizeb, (S64)g_aDac[ii].outBufSizeb );
		BRDC_printf( _BRDC("SamplesPerChannel = %lld (0x%llX)\n"), (S64)g_aDac[ii].samplesPerChannel, (S64)g_aDac[ii].samplesPerChannel );
		BRDC_printf( _BRDC("SignalFreq        = %.2f Hz\n"), g_aDac[ii].dSignalFreq );

		//
		// Создать буфер для сигнала
		//
        g_aDac[ii].pBuf = IPC_virtAlloc( (int)g_aDac[ii].outBufSizeb );
		if( NULL == g_aDac[ii].pBuf )
        {
			BRDC_printf( _BRDC("ERROR: No enougth memory, dacNo = %d"), ii );
			return -1;
		}


		CalcSignal( g_aDac[ii].pBuf, g_aDac[ii].samplesPerChannel, ii, 0 );
	}

	//
	// Выводить циклически данные в FIFO с помощью стрима
	//
    BRDC_printf( _BRDC("Press any key to stop ...\n"));
	nblock = 0;
	PrepareStart( );
    while(!IPC_kbhit())
	{
		FifoOutputCPU( );
		//OldFifoOutputCPU( g_aDac[0].pBuf, g_aDac[0].outBufSizeb );
		nblock++;
        BRDC_printf( _BRDC( "\rBlock %d"), nblock );

		if( (1 != g_nCycle) && (nblock >= g_nCycle) )
			break;
	}

	//
	// Освободить все буфера
	//
	for( ii=0; ii<g_nDacNum; ii++ )
        IPC_virtFree( g_aDac[ii].pBuf );

    BRDC_printf( _BRDC("\n"));
	if( g_nQuickQuit == 0 )
	    IPC_getch();
	
	return 0;
}

//=********************* WorkMode1 ************************
//=********************************************************
S32 WorkMode1( void )
{
	S32				err;
	int				ii;
	volatile SBIG	tmp;
	S32				nblock;
	U32				tetrad;
	U32				source;

    BRDC_printf( _BRDC("\nWorkMode 1: -- One time  DMA (FIFO) -- \n\n"));

	for( ii=0; ii<g_nDacNum; ii++ )
	{
		//
		// Скорректировать количество отсчетов на канал
		//
		g_aDac[ii].outBufSizeb = g_nSamplesPerChannel 
			                   * g_aDac[ii].sampleSizeb 
							   * g_aDac[ii].chanNum;

		if (g_aDac[ii].outBufSizeb > 0xFFFFFFFF)
			g_aDac[ii].outBufSizeb = 0xFFFFFFFF;

		tmp = g_aDac[ii].outBufSizeb / FIFO_WIDTHB;
		g_aDac[ii].outBufSizeb = tmp * FIFO_WIDTHB;

		tmp = g_aDac[ii].outBufSizeb / g_nDmaBufFactor;
		if( 0 != (g_aDac[ii].outBufSizeb % g_nDmaBufFactor))
			tmp++;
		g_aDac[ii].outBufSizeb = tmp * g_nDmaBufFactor;

		if( g_aDac[ii].outBufSizeb > (S32)g_aDac[ii].nFifoSizeb )
		{
			g_aDac[ii].outBufSizeb = g_aDac[ii].nFifoSizeb;
            BRDC_printf( _BRDC("WARNING: OutBufSizeb > DacFifoSizeb !\n"));
		}

		g_aDac[ii].samplesPerChannel = g_aDac[ii].outBufSizeb  
									 / g_aDac[ii].sampleSizeb 
									 / g_aDac[ii].chanNum;

		BRDC_printf( _BRDC("OutBufSizeb       = %lld (0x%llX)\n"), (S64)g_aDac[ii].outBufSizeb, (S64)g_aDac[ii].outBufSizeb );
		BRDC_printf( _BRDC("SamplesPerChannel = %lld (0x%llX)\n"), (S64)g_aDac[ii].samplesPerChannel, (S64)g_aDac[ii].samplesPerChannel );
		BRDC_printf( _BRDC("SignalFreq        = %.2f Hz\n"), g_aDac[ii].dSignalFreq );

		//
		// Создать составной буфер для стрима
		//
		g_aDac[ii].rBufAlloc.dir = BRDstrm_DIR_OUT;
		g_aDac[ii].rBufAlloc.isCont = g_nIsSystemMemory; 
		g_aDac[ii].rBufAlloc.blkNum = 1;
		g_aDac[ii].rBufAlloc.blkSize = (U32)g_aDac[ii].outBufSizeb;
		g_aDac[ii].rBufAlloc.ppBlk = new PVOID[g_aDac[ii].rBufAlloc.blkNum];
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_STREAM_CBUF_ALLOC, &(g_aDac[ii].rBufAlloc) );
		if( 0 > err )
		{
			BRDC_printf( _BRDC("\nERROR: BRDctrl_STREAM_CBUF_ALLOC = 0x%X, dacNo = %d\n\n"), err, ii );
			return -1;
		}

		source = 0;		//  данные из   FIFO 
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_DAC_SETSOURCE, &source );
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_DAC_GETSRCSTREAM, &tetrad );
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_STREAM_SETSRC, &tetrad );

		CalcSignal( g_aDac[ii].rBufAlloc.ppBlk[0], g_aDac[ii].samplesPerChannel, ii, 0 );
		
		S16		jj, arr0[100], *pBuf = (S16*)g_aDac[ii].rBufAlloc.ppBlk[0];
		for( jj=0;jj<100;jj++ ) arr0[jj] = pBuf[jj];

		//S16		jj, arr0[16], arr1[16], *pBuf = (S16*)g_aDac[ii].rBufAlloc.ppBlk[0];

		//CalcSignalOld( (S32*)g_aDac[ii].rBufAlloc.ppBlk[0], g_aDac[ii].samplesPerChannel );
		//for( jj=0;jj<16;jj++ ) arr0[jj] = pBuf[jj];

		//CalcSignal( (S32*)g_aDac[ii].rBufAlloc.ppBlk[0], g_aDac[ii].samplesPerChannel, ii, 0 );
		//for( jj=0;jj<16;jj++ ) arr1[jj] = pBuf[jj];

		jj = 0;
	}

	//
	// Выводить циклически данные в FIFO с помощью стрима
	//
    BRDC_printf( _BRDC("Press any key to stop ...\n"));
	nblock = 0;
	PrepareStart( );
    while(!IPC_kbhit())
	{
		if( 0 > FifoOutputDMA() )
			break;
		nblock++;
        BRDC_printf( _BRDC("\rBlock %d"),nblock);

		if( (1 != g_nCycle) && (nblock >= g_nCycle) )
			break;
	}

	//
	// Освободить все буфера
	//
	for( ii=0; ii<g_nDacNum; ii++ )
	{
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_STREAM_CBUF_FREE, &(g_aDac[ii].rBufAlloc) );
		delete[] g_aDac[ii].rBufAlloc.ppBlk;
	}

    BRDC_printf( _BRDC("\n"));
	if( g_nQuickQuit == 0 )
	    IPC_getch();
	
	return 0;
}

//=********************* WorkMode2 ************************
//=********************************************************
S32 WorkMode2( void )
{
	S32				err;
	int				ii;
	volatile SBIG	tmp;
	S32				nblock;
	U32				tetrad;
	U32				source;

    BRDC_printf( _BRDC("\nWorkMode 2: -- One time  DMA (SDRAM) -- \n\n"));

	for( ii=0; ii<g_nDacNum; ii++ )
	{
		//
		// Скорректировать количество отсчетов на канал
		//
		g_aDac[ii].outBufSizeb = g_nSamplesPerChannel 
			* g_aDac[ii].sampleSizeb 
			* g_aDac[ii].chanNum;

		tmp = g_aDac[ii].outBufSizeb / g_nSdramWriteBufSize;
		if( tmp == 0 )
			tmp =  1;
		g_aDac[ii].outBufSizeb = tmp * g_nSdramWriteBufSize;

		tmp = g_aDac[ii].outBufSizeb / g_nDmaBufFactor;
		if( 0 != (g_aDac[ii].outBufSizeb % g_nDmaBufFactor))
			tmp++;
		g_aDac[ii].outBufSizeb = tmp * g_nDmaBufFactor;

		g_aDac[ii].samplesPerChannel = g_aDac[ii].outBufSizeb  
			/ g_aDac[ii].sampleSizeb 
			/ g_aDac[ii].chanNum;

		SdramSetParam( ii );
		//g_OutBufSize = g_aDac[ii].outBufSizeb;
		//OldSetParamSDRAM(g_hCaptSrv[ii]);

		BRDC_printf( _BRDC("OutBufSizeb       = %lld (0x%llX)\n"), (S64)g_aDac[ii].outBufSizeb, (S64)g_aDac[ii].outBufSizeb );
		BRDC_printf( _BRDC("SamplesPerChannel = %lld (0x%llX)\n"), (S64)g_aDac[ii].samplesPerChannel, (S64)g_aDac[ii].samplesPerChannel );
		BRDC_printf( _BRDC("SignalFreq        = %.2f Hz\n"), g_aDac[ii].dSignalFreq );

		//
		// Создать составной буфер для стрима
		//
		g_aDac[ii].rBufAlloc.dir = BRDstrm_DIR_OUT;
		g_aDac[ii].rBufAlloc.isCont = g_nIsSystemMemory; 
		g_aDac[ii].rBufAlloc.blkNum = 1;
		g_aDac[ii].rBufAlloc.blkSize = g_nSdramWriteBufSize;
		g_aDac[ii].rBufAlloc.ppBlk = new PVOID[g_aDac[ii].rBufAlloc.blkNum];
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_STREAM_CBUF_ALLOC, &(g_aDac[ii].rBufAlloc) );
		if( 0 > err )
		{
			BRDC_printf( _BRDC("ERROR: BRDctrl_STREAM_CBUF_ALLOC = 0x%X, dacNo = %d"), err, ii );
			return -1;
		}

		source = 2;		//  данные из SDRAM 
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_DAC_SETSOURCE, &source );
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_SDRAM_GETSRCSTREAM, &tetrad );
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_STREAM_SETSRC, &tetrad );
	}

	//
	// Выводить многократно данные в SDRAM с помощью стрима
	//
    BRDC_printf( _BRDC("Wait...\n"));

	nblock = 0;
	if( 0 < SdramWriteDMA() )
	{
		PrepareStart( );
        while(!IPC_kbhit())
		{	
			if( g_nIsAlwaysWriteSdram )
			{
				if( 0 > SdramWriteDMA() )
					break;
			}
			SdramOutToDAC();
			nblock++; 
            BRDC_printf( _BRDC("\rBlock %d"),nblock);

			if( (1 != g_nCycle) && (nblock >= g_nCycle) )
				break;
		}
	}

	//
	// Освободить все буфера
	//
	for( ii=0; ii<g_nDacNum; ii++ )
	{
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_STREAM_CBUF_FREE, &(g_aDac[ii].rBufAlloc) );
		delete[] g_aDac[ii].rBufAlloc.ppBlk;
	}

    BRDC_printf( _BRDC("\n"));
	if( g_nQuickQuit == 0 )
	    IPC_getch();

	return 0;
}

//=********************* WorkMode3 ************************
//=********************************************************
S32 WorkMode3( void )
{
	S32				err;
	int				ii;
	volatile SBIG	tmp;
	S32				isEnable;

    BRDC_printf( _BRDC("\nWorkMode 3: -- Restart FIFO -- \n\n"));

	for( ii=0; ii<g_nDacNum; ii++ )
	{
		//
		// Скорректировать количество отсчетов на канал
		//
		g_aDac[ii].outBufSizeb = g_nSamplesPerChannel 
			                   * g_aDac[ii].sampleSizeb 
							   * g_aDac[ii].chanNum;

		if (g_aDac[ii].outBufSizeb > 0x7FFFFFFF)
			g_aDac[ii].outBufSizeb = 0x7FFFFFFF;

		tmp = g_aDac[ii].outBufSizeb / FIFO_WIDTHB;
		g_aDac[ii].outBufSizeb = tmp * FIFO_WIDTHB;
		if( g_aDac[ii].outBufSizeb > (S32)g_aDac[ii].nFifoSizeb )
		{
			g_aDac[ii].outBufSizeb = g_aDac[ii].nFifoSizeb;
            BRDC_printf( _BRDC("WARNING: OutBufSizeb > DacFifoSizeb !\n"));
		}

		g_aDac[ii].samplesPerChannel = g_aDac[ii].outBufSizeb  
										/ g_aDac[ii].sampleSizeb 
										/ g_aDac[ii].chanNum;

		BRDC_printf( _BRDC("OutBufSizeb       = %lld (0x%llX)\n"), (S64)g_aDac[ii].outBufSizeb, (S64)g_aDac[ii].outBufSizeb );
		BRDC_printf( _BRDC("SamplesPerChannel = %lld (0x%llX)\n"), (S64)g_aDac[ii].samplesPerChannel, (S64)g_aDac[ii].samplesPerChannel );
		BRDC_printf( _BRDC("SignalFreq        = %.2f Hz\n"), g_aDac[ii].dSignalFreq );

		//
		// Создать буфер для сигнала
		//
        g_aDac[ii].pBuf = IPC_virtAlloc( (int)g_aDac[ii].outBufSizeb );
		if( NULL == g_aDac[ii].pBuf )
		{
			BRDC_printf( _BRDC("ERROR: No enougth memory, dacNo = %d"), ii );
			return -1;
		}

		CalcSignal( g_aDac[ii].pBuf, g_aDac[ii].samplesPerChannel, ii, 0 );
	}

	//
	// Выводить циклически данные в FIFO с помощью стрима
	//
    BRDC_printf( _BRDC("Press any key to stop ...\n"));
	PrepareStart( );
	FifoOutputCPUStart( 0 );
    while(!IPC_kbhit());

	//
	// Остановит все ЦАПы и освободить все буфера
	//
	for( ii=0; ii<g_nDacNum; ii++ )
	{
		isEnable = 0;
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_DAC_ENABLE, &isEnable); // запрет работы ЦАП
        IPC_virtFree( g_aDac[ii].pBuf );
	}

    BRDC_printf( _BRDC("\n"));
	if( g_nQuickQuit == 0 )
	    IPC_getch();
	
	return 0;
}

//=********************* WorkMode4 ************************
//=********************************************************
S32 WorkMode4( void )
{
	S32				err;
	int				ii;
	volatile SBIG	tmp;
	S32				isEnable;
	U32				tetrad;
	U32				source;

    BRDC_printf( _BRDC("\nWorkMode 4: -- Restart SDRAM -- \n\n"));

	for( ii=0; ii<g_nDacNum; ii++ )
	{
		//
		// Скорректировать количество отсчетов на канал
		//
		g_aDac[ii].outBufSizeb = g_nSamplesPerChannel 
								* g_aDac[ii].sampleSizeb 
								* g_aDac[ii].chanNum;

		tmp = g_aDac[ii].outBufSizeb / g_nSdramWriteBufSize;
		if( tmp == 0 )
			tmp =  1;
		g_aDac[ii].outBufSizeb = tmp * g_nSdramWriteBufSize;

		tmp = g_aDac[ii].outBufSizeb / g_nDmaBufFactor;
		if( 0 != (g_aDac[ii].outBufSizeb % g_nDmaBufFactor))
			tmp++;
		g_aDac[ii].outBufSizeb = tmp * g_nDmaBufFactor;

		g_aDac[ii].samplesPerChannel = g_aDac[ii].outBufSizeb  
										/ g_aDac[ii].sampleSizeb 
										/ g_aDac[ii].chanNum;

		SdramSetParam( ii );	// У Склярова вызывается 2 раза зачем-то

		BRDC_printf( _BRDC("OutBufSizeb       = %lld (0x%llX)\n"), (S64)g_aDac[ii].outBufSizeb, (S64)g_aDac[ii].outBufSizeb );
		BRDC_printf( _BRDC("SamplesPerChannel = %lld (0x%llX)\n"), (S64)g_aDac[ii].samplesPerChannel, (S64)g_aDac[ii].samplesPerChannel );
		BRDC_printf( _BRDC("SignalFreq        = %.2f Hz\n"), g_aDac[ii].dSignalFreq );

		//
		// Создать составной буфер для стрима
		//
		g_aDac[ii].rBufAlloc.dir = BRDstrm_DIR_OUT;
		g_aDac[ii].rBufAlloc.isCont = g_nIsSystemMemory; 
		g_aDac[ii].rBufAlloc.blkNum = 1;
		g_aDac[ii].rBufAlloc.blkSize = g_nSdramWriteBufSize;
		g_aDac[ii].rBufAlloc.ppBlk = new PVOID[g_aDac[ii].rBufAlloc.blkNum];
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_STREAM_CBUF_ALLOC, &(g_aDac[ii].rBufAlloc) );
		if( 0 > err )
		{
			BRDC_printf( _BRDC("ERROR: BRDctrl_STREAM_CBUF_ALLOC = 0x%X, dacNo = %d"), err, ii );
			return -1;
		}

		source = 2;		//  данные из SDRAM 
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_DAC_SETSOURCE, &source );
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_SDRAM_GETSRCSTREAM, &tetrad );
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_STREAM_SETSRC, &tetrad );
	}

	//
	// Выводить многократно данные в SDRAM с помощью стрима
	//
    BRDC_printf( _BRDC("Wait...\n"));

	if( 0 < SdramWriteDMA() )
	{
		PrepareStart( );
		SdramCycleOutput(0);
        BRDC_printf( _BRDC("Press any key to stop!\n"));
        while(!IPC_kbhit());
	}

	//
	// Остановить все ЦАПы и освободить все буфера
	//
	for( ii=0; ii<g_nDacNum; ii++ )
	{
		isEnable = 0;
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_DAC_ENABLE,  &isEnable); // запрещение вывода	
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_SDRAM_ENABLE,  &isEnable); // Запретить чтение из SDRAM	
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_STREAM_CBUF_FREE, &(g_aDac[ii].rBufAlloc) );
		delete[] g_aDac[ii].rBufAlloc.ppBlk;
	}

    BRDC_printf( _BRDC("\n"));
	if( g_nQuickQuit == 0 )
	    IPC_getch();

	return 0;
}

//=********************* WorkMode5 ************************
//=********************************************************
S32 WorkMode5( void )
{
	S32				err;
	int				ii;
	volatile SBIG	tmp;
	S32				isEnable;
	int				loop;

    BRDC_printf( _BRDC("\nWorkMode 5: -- Cycle FIFO -- \n\n"));

	for( ii=0; ii<g_nDacNum; ii++ )
	{
		//
		// Скорректировать количество отсчетов на канал
		//
		g_aDac[ii].outBufSizeb = g_nSamplesPerChannel 
			                   * g_aDac[ii].sampleSizeb 
							   * g_aDac[ii].chanNum;

		if (g_aDac[ii].outBufSizeb > 0x7FFFFFFF)
			g_aDac[ii].outBufSizeb = 0x7FFFFFFF;

		tmp = g_aDac[ii].outBufSizeb / FIFO_WIDTHB;
		g_aDac[ii].outBufSizeb = tmp * FIFO_WIDTHB;
		if( g_aDac[ii].outBufSizeb > (S32)g_aDac[ii].nFifoSizeb-FIFO_WIDTHB )
		{
			g_aDac[ii].outBufSizeb = g_aDac[ii].nFifoSizeb-FIFO_WIDTHB;
            BRDC_printf( _BRDC("WARNING: OutBufSizeb > DacFifoSizeb !\n"));
		}

		g_aDac[ii].samplesPerChannel = g_aDac[ii].outBufSizeb  
									 / g_aDac[ii].sampleSizeb 
									 / g_aDac[ii].chanNum;
		CorrectOutFreq( ii );

		BRDC_printf( _BRDC("OutBufSizeb       = %lld (0x%llX)\n"), (S64)g_aDac[ii].outBufSizeb, (S64)g_aDac[ii].outBufSizeb );
		BRDC_printf( _BRDC("SamplesPerChannel = %lld (0x%llX)\n"), (S64)g_aDac[ii].samplesPerChannel, (S64)g_aDac[ii].samplesPerChannel );
		BRDC_printf( _BRDC("SignalFreq        = %.2f Hz\n"), g_aDac[ii].dSignalFreq );

		//
		// Создать буфер для сигнала
		//
        g_aDac[ii].pBuf = IPC_virtAlloc( (int)g_aDac[ii].outBufSizeb );
		if( NULL == g_aDac[ii].pBuf )
		{
			BRDC_printf( _BRDC("ERROR: No enougth memory, dacNo = %d"), ii );
			return -1;
		}

		CalcSignal( g_aDac[ii].pBuf, g_aDac[ii].samplesPerChannel, ii, 0 );
	}

	//
	// Выводить циклически данные в FIFO 
	//
	PrepareStart( );
	FifoOutputCPUStart( 1 );
    BRDC_printf( _BRDC("Press any key to stop ...\n\n"));
	loop = 0;
    while(!IPC_kbhit())
	{
        IPC_delay( 200 );
		DisplayDacTraceText( loop++, 0 );
	}

	//
	// Остановить работу всех ЦАПов
	//
	isEnable = 0;
	for( ii=0; ii<g_nDacNum; ii++ )
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_DAC_ENABLE, &isEnable); // запрет работы ЦАП


	//
	// Освободить все буфера
	//
	for( ii=0; ii<g_nDacNum; ii++ )
        IPC_virtFree( g_aDac[ii].pBuf );

    BRDC_printf( _BRDC("\n"));
	if( g_nQuickQuit == 0 )
	    IPC_getch();
	
	return 0;
}

//=********************* WorkMode6 ************************
//=********************************************************
S32 WorkMode6( void )
{
	S32				err;
	int				ii;
	volatile SBIG	tmp;
	//S32				nblock;
	S32				isEnable;
	//U32				tetrad;
	//U32				source;

    BRDC_printf( _BRDC("\nWorkMode 6: -- Cycle DMA through FIFO -- \n\n"));

	for( ii=0; ii<g_nDacNum; ii++ )
	{
		g_aDac[ii].outBufSizeb = g_nSamplesPerChannel 
			                   * g_aDac[ii].sampleSizeb 
							   * g_aDac[ii].chanNum;

		if( g_aDac[ii].outBufSizeb > 0xFFFFFFFF )
			g_aDac[ii].outBufSizeb = 0xFFFFFFFF;

		tmp = g_aDac[ii].outBufSizeb / g_nDmaBufFactor;
		if( 0 != (g_aDac[ii].outBufSizeb % g_nDmaBufFactor))
			tmp++;
		g_aDac[ii].outBufSizeb = tmp * g_nDmaBufFactor;

		g_aDac[ii].samplesPerChannel = g_nSamplesPerChannel;

		//
		// Скорректировать частоту выходного сигнала
		//
		CorrectOutFreq( ii );

		BRDC_printf( _BRDC("OutBufSizeb       = %lld (0x%llX)\n"), (S64)g_aDac[ii].outBufSizeb, (S64)g_aDac[ii].outBufSizeb );
		BRDC_printf( _BRDC("SamplesPerChannel = %lld (0x%llX)\n"), (S64)g_aDac[ii].samplesPerChannel, (S64)g_aDac[ii].samplesPerChannel );
		BRDC_printf( _BRDC("SignalFreq        = %.2f Hz\n"), g_aDac[ii].dSignalFreq );

		//
		// Создать составной буфер для стрима
		//
		g_aDac[ii].rBufAlloc.dir = BRDstrm_DIR_OUT;
		g_aDac[ii].rBufAlloc.isCont = g_nIsSystemMemory; 
		g_aDac[ii].rBufAlloc.blkNum = 1;
		g_aDac[ii].rBufAlloc.blkSize = (U32)g_aDac[ii].outBufSizeb;
		g_aDac[ii].rBufAlloc.ppBlk = new PVOID[g_aDac[ii].rBufAlloc.blkNum];
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_STREAM_CBUF_ALLOC, &(g_aDac[ii].rBufAlloc) );
		if( 0 > err )
		{
			BRDC_printf( _BRDC("ERROR: BRDctrl_STREAM_CBUF_ALLOC = 0x%X, dacNo = %d"), err, ii );
			return -1;
		}

		//source = 0;		//  данные из   FIFO 
		//err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_DAC_SETSOURCE, &source );
		//err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_DAC_GETSRCSTREAM, &tetrad );
		//err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_STREAM_SETSRC, &tetrad );

		CalcSignal( g_aDac[ii].rBufAlloc.ppBlk[0], g_aDac[ii].samplesPerChannel, ii, 0 );
	}

	//
	// Выводить циклически данные в FIFO с помощью стрима
	//
    BRDC_printf( _BRDC("Press any key to stop ...\n"));
	PrepareStart( );
	FifoOutputCycleDMA();

	//
	// Остановить все ЦАПы и освободить все буфера
	//
	for( ii=0; ii<g_nDacNum; ii++ )
	{
		isEnable = 0;
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_DAC_ENABLE,  &isEnable); // запрещение вывода	
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_STREAM_CBUF_FREE, &(g_aDac[ii].rBufAlloc) );
		delete[] g_aDac[ii].rBufAlloc.ppBlk;
	}

    BRDC_printf( _BRDC("\n"));

	return 0;
}

//=********************* WorkMode7 ************************
//=********************************************************
S32 WorkMode7( void )
{
	S32				err;
	int				ii;
	volatile SBIG	tmp;
	S32				isEnable;
	S32				nblock;
	U32				tetrad;
	U32				source;
	int				loop;

    BRDC_printf( _BRDC("\nWorkMode 7: -- Cycle SDRAM -- \n\n"));

	for( ii=0; ii<g_nDacNum; ii++ )
	{
		//
		// Скорректировать количество отсчетов на канал
		//
		g_aDac[ii].outBufSizeb = g_nSamplesPerChannel 
								* g_aDac[ii].sampleSizeb 
								* g_aDac[ii].chanNum;

		tmp = g_aDac[ii].outBufSizeb / g_nSdramWriteBufSize;
		if( tmp == 0 )
			tmp =  1;
		g_aDac[ii].outBufSizeb = tmp * g_nSdramWriteBufSize;

		tmp = g_aDac[ii].outBufSizeb / g_nDmaBufFactor;
		if( 0 != (g_aDac[ii].outBufSizeb % g_nDmaBufFactor))
			tmp++;
		g_aDac[ii].outBufSizeb = tmp * g_nDmaBufFactor;

		g_aDac[ii].samplesPerChannel = g_aDac[ii].outBufSizeb  
										/ g_aDac[ii].sampleSizeb 
										/ g_aDac[ii].chanNum;

		SdramSetParam( ii );

		CorrectOutFreq( ii );

		BRDC_printf( _BRDC("OutBufSizeb       = %lld (0x%llX)\n"), (S64)g_aDac[ii].outBufSizeb, (S64)g_aDac[ii].outBufSizeb );
		BRDC_printf( _BRDC("SamplesPerChannel = %lld (0x%llX)\n"), (S64)g_aDac[ii].samplesPerChannel, (S64)g_aDac[ii].samplesPerChannel );
		BRDC_printf( _BRDC("SignalFreq        = %.2f Hz\n"), g_aDac[ii].dSignalFreq );

		//
		// Создать составной буфер для стрима
		//
		g_aDac[ii].rBufAlloc.dir = BRDstrm_DIR_OUT;
		g_aDac[ii].rBufAlloc.isCont = g_nIsSystemMemory; 
		g_aDac[ii].rBufAlloc.blkNum = 1;
		g_aDac[ii].rBufAlloc.blkSize = g_nSdramWriteBufSize;
		g_aDac[ii].rBufAlloc.ppBlk = new PVOID[g_aDac[ii].rBufAlloc.blkNum];
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_STREAM_CBUF_ALLOC, &(g_aDac[ii].rBufAlloc) );
		if( 0 > err )
		{
			BRDC_printf( _BRDC("ERROR: BRDctrl_STREAM_CBUF_ALLOC = 0x%X, dacNo = %d"), err, ii );
			return -1;
		}

		source = 2;		//  данные из SDRAM 
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_DAC_SETSOURCE, &source );
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_SDRAM_GETSRCSTREAM, &tetrad );
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_STREAM_SETSRC, &tetrad );
	}

	//
	// Выводить многократно данные в SDRAM с помощью стрима
	//
    BRDC_printf( _BRDC("Wait...\n"));

	nblock = 0;
	loop = 0;
	if( 0 < SdramWriteDMA() )
	{
		PrepareStart( );
		SdramCycleOutput(1);
        BRDC_printf( _BRDC("Press any key to stop!\n\n"));
        while(!IPC_kbhit())
		{
            IPC_delay( 200 );
			DisplayDacTraceText( loop++, 0 );
		}
	}

	//
	// Остановить все ЦАПы и освободить все буфера
	//
	for( ii=0; ii<g_nDacNum; ii++ )
	{
		isEnable = 0;
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_DAC_ENABLE,  &isEnable); // запрещение вывода	
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_SDRAM_ENABLE,  &isEnable); // Запретить чтение из SDRAM	
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_STREAM_CBUF_FREE, &(g_aDac[ii].rBufAlloc) );
		delete[] g_aDac[ii].rBufAlloc.ppBlk;
	}

    BRDC_printf( _BRDC("\n"));
	if( g_nQuickQuit == 0 )
	    IPC_getch();

	return 0;
}


//=********************* WorkMode8 ************************
//=********************************************************
S32 WorkMode8( void )
{
	S32				err;
	int				ii;
	volatile SBIG	tmp;
	//S32				nblock;
	S32				isEnable;
	U32				tetrad;
	U32				source;

    BRDC_printf( _BRDC("\nWorkMode 8: -- Cycle DMA through 'SDRAM like FIFO' -- \n\n"));

	for( ii=0; ii<g_nDacNum; ii++ )
	{
		g_aDac[ii].outBufSizeb = g_nSamplesPerChannel 
			* g_aDac[ii].sampleSizeb 
			* g_aDac[ii].chanNum;
		
		if (g_aDac[ii].outBufSizeb > 0xFFFFFFFF)
			g_aDac[ii].outBufSizeb = 0xFFFFFFFF;

		//? tmp = g_aDac[ii].outBufSizeb / g_nSdramWriteBufSize;
		//? if( tmp == 0 )
		//? 	tmp =  1;
		//? g_aDac[ii].outBufSizeb = tmp * g_nSdramWriteBufSize;

		tmp = g_aDac[ii].outBufSizeb / g_nDmaBufFactor;
		if( 0 != (g_aDac[ii].outBufSizeb % g_nDmaBufFactor))
			tmp++;
		g_aDac[ii].outBufSizeb = tmp * g_nDmaBufFactor;

		g_aDac[ii].samplesPerChannel = g_nSamplesPerChannel;

		//
		// Подготовить SDRAM к ркжиму "SDRAM как FIFO"
		//
		U32 sdramMode = 1; // память используется как FIFO
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_SDRAM_SETFIFOMODE, &sdramMode );

		//
		// Скорректировать частоту выходного сигнала
		//
		CorrectOutFreq( ii );

		BRDC_printf( _BRDC("OutBufSizeb       = %lld (0x%llX)\n"), (S64)g_aDac[ii].outBufSizeb, (S64)g_aDac[ii].outBufSizeb );
		BRDC_printf( _BRDC("SamplesPerChannel = %lld (0x%llX)\n"), (S64)g_aDac[ii].samplesPerChannel, (S64)g_aDac[ii].samplesPerChannel );
		BRDC_printf( _BRDC("SignalFreq        = %.2f Hz\n"), g_aDac[ii].dSignalFreq );

		//
		// Создать составной буфер для стрима
		//
		g_aDac[ii].rBufAlloc.dir = BRDstrm_DIR_OUT;
		g_aDac[ii].rBufAlloc.isCont = g_nIsSystemMemory; 
		g_aDac[ii].rBufAlloc.blkNum = 1;
		g_aDac[ii].rBufAlloc.blkSize = (U32)g_aDac[ii].outBufSizeb;
		g_aDac[ii].rBufAlloc.ppBlk = new PVOID[g_aDac[ii].rBufAlloc.blkNum];
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_STREAM_CBUF_ALLOC, &(g_aDac[ii].rBufAlloc) );
		if( 0 > err )
		{
			BRDC_printf( _BRDC("ERROR: BRDctrl_STREAM_CBUF_ALLOC = 0x%X, dacNo = %d"), err, ii );
			return -1;
		}

		source = 2;		//  данные из SDRAM 
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_DAC_SETSOURCE, &source );
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_SDRAM_GETSRCSTREAM, &tetrad );
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_STREAM_SETSRC, &tetrad );

		CalcSignal( g_aDac[ii].rBufAlloc.ppBlk[0], g_aDac[ii].samplesPerChannel, ii, 0 );
	}

	//
	// Выводить циклически данные в FIFO с помощью стрима
	//
    BRDC_printf( _BRDC("Press any key to stop ...\n"));
	PrepareStart( );
	SdramLikeFifoOutputCycleDMA();

	//
	// Остановить все ЦАПы и освободить все буфера
	//
	for( ii=0; ii<g_nDacNum; ii++ )
	{
		isEnable = 0;
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_DAC_ENABLE,  &isEnable); // запрещение вывода	
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_SDRAM_ENABLE,  &isEnable); // Запретить чтение из SDRAM	
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_STREAM_CBUF_FREE, &(g_aDac[ii].rBufAlloc) );
		delete[] g_aDac[ii].rBufAlloc.ppBlk;
	}

    BRDC_printf( _BRDC("\n"));

	return 0;
}

//=******************** FifoOutputCPU *********************
//=********************************************************
S32	FifoOutputCPU( )
{

	S32			err;
	int			ii;
	ULONG		isEnable;
	U32			statusFIFO=0x4;

	FifoOutputCPUStart( 0 );

	//
	// Ожидать завершения работы всех ЦАПов
	//
	for( ii=0; ii<g_nDacNum; ii++ )
	{
		statusFIFO = 0x4;
	    while( (statusFIFO & 0x4) == 0x4 )	// ждать окончания выдачи блока из FIFO  
			 err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_DAC_FIFOSTATUS, &statusFIFO); 
	}

	//
	// Остановить работу всех ЦАПов
	//
	isEnable = 0;
	for( ii=0; ii<g_nDacNum; ii++ )
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_DAC_ENABLE, &isEnable); // запрет работы ЦАП

	return 0;
}

//=**************** FifoOutputCPUCycleFIFO ****************
//=********************************************************
S32 FifoOutputCPUStart( S32 isCycle )
{
	S32			err;
	int			ii, idx;
	S32			isEnable = 0;
	BRD_DataBuf rDataBuf;
	S32			cycling = isCycle; // 0-restart,1-retransmit

	//
	// Стартовать вывод в FIFO, начиная с последнего ЦАПа так, чтобы закончить МАСТЕРОМ
	//
	for( ii=g_nDacNum-1; ii>=0; ii-- )
	{
		idx = g_idx[ii];

		err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_DAC_SETCYCLMODE, &cycling) ;

		isEnable = 0;
		err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_DAC_ENABLE, &isEnable ); // запрет работы ЦАП
		err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_DAC_FIFORESET, NULL ); // сброс FIFO ЦАП

		rDataBuf.pData = g_aDac[idx].pBuf;
		rDataBuf.size  = (U32)g_aDac[idx].outBufSizeb;
		err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_DAC_PUTDATA, &rDataBuf );

//		if(i==0) status = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_DAC_OUTSYNC,  NULL); //выдать импульс синхронизации (Master!)

		isEnable = 1;
		err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_DAC_ENABLE, &isEnable ); // разрешение работы ЦАП
	}
	return 0;
}

//=********************* FifoOutputDMA ********************
//=********************************************************
S32	FifoOutputDMA(void)
{
	S32			err;
	int			ii, idx;
	S32			isEnable;
	U32			statusFIFO;
	U32			msTimeout;

	BRDctrl_StreamCBufStart rCBufStart;

	//
	// Стартовать вывод в FIFO, начиная с последнего ЦАПа так, чтобы закончить МАСТЕРОМ
	//
	for( ii=g_nDacNum-1; ii>=0; ii-- )
	{
		idx = g_idx[ii];

		isEnable = 0;
		err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_DAC_ENABLE, &isEnable ); // запрещение вывода
		err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_DAC_FIFORESET, NULL ); // сброс FIFO 

		err = BRD_ctrl(g_aDac[idx].handle, 0, BRDctrl_STREAM_RESETFIFO, NULL); // сброс FIFO Stream

		rCBufStart.isCycle = 0;		// без зацикливания 
		err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_STREAM_CBUF_START, &rCBufStart) ; // старт ПДП

		msTimeout = 1000;			// ждать окончания передачи блока данных до 1 сек.
		err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_STREAM_CBUF_WAITBLOCK, &msTimeout );
		if( BRD_errcmp(err, BRDerr_WAIT_TIMEOUT) )
		{	// вышли по тайм-ауту
			BRDC_printf(_BRDC("\nTIME-OUT DMA! \n"));
			err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_STREAM_CBUF_STOP, NULL ); // стоп ПДП
			return -1;
		}

//		if(i==0) status = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_DAC_OUTSYNC,  NULL); //выдать импульс синхронизации (Master!)

		isEnable=1;
		err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_DAC_ENABLE,  &isEnable); // разрешение вывода
	}

	//
	// Ожидать завершения работы всех ЦАПов
	//
	for( ii=0; ii<g_nDacNum; ii++ )
	{
		statusFIFO = 0x4;

		//while( (statusFIFO & 0x4) == 0x4 )	// ждать окончания выдачи блока из FIFO  
		for(int jj = 0; jj < (1+g_nMsTimeout/50); jj++)
		{
			err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_DAC_FIFOSTATUS, &statusFIFO); 
			if((statusFIFO & 0x4) != 0x4 )
				break;
			IPC_delay(50);
		}
		if((statusFIFO & 0x4) == 0x4 )
			BRDC_printf(_BRDC("\nTIME-OUT! status FIFO = 0x%04X\n"), statusFIFO);

	}

	//
	// Остановить работу всех ЦАПов
	//
	isEnable = 0;
	for( ii=0; ii<g_nDacNum; ii++ )
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_DAC_ENABLE, &isEnable); // запрет работы ЦАП


	return 0;
}

//=***************** FifoOutputCycleDMA ********************
//=********************************************************
S32	FifoOutputCycleDMA(void)
{
	//
	// Используется только для WorkMode=6
	//

	S32			err;
	int			ii, idx;
	S32			isEnable;
	U32			statusFIFO;
	U32			msTimeout;
	U32			drqFlag; 
	S32			nblock;

	BRDctrl_StreamCBufStart rCBufStart;

	//
	// Стартовать вывод в FIFO, начиная с последнего ЦАПа так, чтобы закончить МАСТЕРОМ
	//
	for( ii=g_nDacNum-1; ii>=0; ii-- )
	{
		idx = g_idx[ii];

		drqFlag = BRDstrm_DRQ_HALF; 
		err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_STREAM_SETDRQ, &drqFlag );
		
		isEnable = 0;
		err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_DAC_ENABLE, &isEnable ); // запрещение вывода
		err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_DAC_FIFORESET, NULL ); // сброс FIFO 

		err = BRD_ctrl(g_aDac[idx].handle, 0, BRDctrl_STREAM_RESETFIFO, NULL); // сброс FIFO Stream

		rCBufStart.isCycle = 1;		// с зацикливанием 
		err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_STREAM_CBUF_START, &rCBufStart) ; // старт ПДП

//		if(i==0) status = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_DAC_OUTSYNC,  NULL); //выдать импульс синхронизации (Master!)

		isEnable=1;
		err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_DAC_ENABLE,  &isEnable); // разрешение вывода

	}

	nblock = 0;
    while(!IPC_kbhit())
	{
		msTimeout = 5000;			// ждать окончания передачи блока данных до 2 сек.
		err = BRD_ctrl( g_aDac[0].handle, 0, BRDctrl_STREAM_CBUF_WAITBLOCK, &msTimeout );
		if( BRD_errcmp(err, BRDerr_WAIT_TIMEOUT) )
		{	// вышли по тайм-ауту
            BRDC_printf( _BRDC("\nTIME-OUT! \n"));
			err = BRD_ctrl( g_aDac[0].handle, 0, BRDctrl_DAC_FIFOSTATUS, &statusFIFO); 
            BRDC_printf( _BRDC("DAC   state = 0x%X \n"), statusFIFO );
			return -1;
		}

		err = BRD_ctrl( g_aDac[0].handle, 0, BRDctrl_DAC_FIFOSTATUS, &statusFIFO); 
		if( statusFIFO & 0x100 )
		{
            BRDC_printf( _BRDC("\nUnderflow! \n"));
            BRDC_printf( _BRDC("Press any key to stop!\n"));

            IPC_getch();
			return -1;
		}

		nblock++;
        BRDC_printf( _BRDC("\rBlock %d"),nblock);
	}

    IPC_getch();

	return 0;
}

//=************* SdramLikeFifoOutputCycleDMA **************
//=********************************************************
S32	SdramLikeFifoOutputCycleDMA(void)
{
	//
	// Используется только для WorkMode=8
	// (несколько исправленная FifoOutputCycleDMA())
	//

	S32			err;
	int			ii, idx;
	S32			isEnable;
	U32			statusFIFO;
	U32			msTimeout;
	U32			drqFlag; 
	S32			nblock;

	BRDctrl_StreamCBufStart rCBufStart;

	//
	// Стартовать вывод в FIFO, начиная с последнего ЦАПа так, чтобы закончить МАСТЕРОМ
	//
	for( ii=g_nDacNum-1; ii>=0; ii-- )
	{
		idx = g_idx[ii];

		drqFlag = BRDstrm_DRQ_HALF; 
		err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_STREAM_SETDRQ, &drqFlag );

		isEnable = 0;
		err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_DAC_ENABLE, &isEnable ); // запрещение вывода
		err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_DAC_FIFORESET, NULL ); // сброс FIFO 

		isEnable = 0;
		err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_SDRAM_ENABLE, &isEnable ); // запрещение SDRAM
		err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_SDRAM_FIFORESET, NULL ); // сброс FIFO SDRAM

		err = BRD_ctrl(g_aDac[idx].handle, 0, BRDctrl_STREAM_RESETFIFO, NULL); // сброс FIFO Stream

		rCBufStart.isCycle = 1;		// с зацикливанием 
		err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_STREAM_CBUF_START, &rCBufStart) ; // старт ПДП

		isEnable = 1;
		err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_SDRAM_ENABLE, &isEnable ); // разрешение SDRAM
        IPC_delay( 100 );

		isEnable=1;
		err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_DAC_ENABLE,  &isEnable); // разрешение вывода

	}

	nblock = 0;
    while(!IPC_kbhit())
	{
		msTimeout = 5000;			// ждать окончания передачи блока данных до 5 сек.
		err = BRD_ctrl( g_aDac[0].handle, 0, BRDctrl_STREAM_CBUF_WAITBLOCK, &msTimeout );
		if( BRD_errcmp(err, BRDerr_WAIT_TIMEOUT) )
		{	// вышли по тайм-ауту
            BRDC_printf( _BRDC("\nTIME-OUT! \n"));
			err = BRD_ctrl( g_aDac[0].handle, 0, BRDctrl_SDRAM_FIFOSTATUS, &statusFIFO); 
            BRDC_printf( _BRDC("SDRAM state = 0x%X \n"), statusFIFO );
			err = BRD_ctrl( g_aDac[0].handle, 0, BRDctrl_DAC_FIFOSTATUS, &statusFIFO); 
            BRDC_printf( _BRDC("DAC   state = 0x%X \n"), statusFIFO );
			return -1;
		}

		err = BRD_ctrl( g_aDac[0].handle, 0, BRDctrl_SDRAM_FIFOSTATUS, &statusFIFO); 
		if( statusFIFO & 0x100 )
		{
            BRDC_printf( _BRDC("\nUnderflow! \n"));
            BRDC_printf( _BRDC("Press any key to stop!\n"));

            IPC_getch();
			return -1;
		}

		nblock++;
        BRDC_printf( _BRDC("\rBlock %d"),nblock);
	}

    IPC_getch();

	return 0;
}

//=******************** SdramSetParam *********************
//=********************************************************
S32 SdramSetParam( int idxDac )
{
	S32				err;
	U32				nPhysMemSizew;
	__int64			nPhysMemSizeb;
	U32				memSizew;

	BRD_SdramCfgEx rSdramConfig;
	rSdramConfig.Size = sizeof(BRD_SdramCfgEx);
	err = BRD_ctrl(g_aDac[idxDac].handle, 0, BRDctrl_SDRAM_GETCFGEX, &rSdramConfig);
	if(err < 0)
	{
		nPhysMemSizew = 0; // SDRAM отсутствует
		return -1;
	}
	if (rSdramConfig.MemType == 11 || //DDR3
		rSdramConfig.MemType == 12) //DDR4
		nPhysMemSizew = (ULONG)((
		(((__int64)rSdramConfig.CapacityMbits * 1024 * 1024) >> 3) *
			(__int64)rSdramConfig.PrimWidth / rSdramConfig.ChipWidth * rSdramConfig.ModuleBanks *
			rSdramConfig.ModuleCnt) >> 2); // в 32-битных словах
	else
		nPhysMemSizew =	(1 << rSdramConfig.RowAddrBits) *
					(1 << rSdramConfig.ColAddrBits) * 
					rSdramConfig.ModuleBanks * 
					rSdramConfig.ChipBanks *
					rSdramConfig.ModuleCnt * 2; // size (U32)
	nPhysMemSizeb = (__int64)nPhysMemSizew * 4;

    BRDC_printf( _BRDC("SDRAM total size = %d Mbytes\n"), (nPhysMemSizew/(1024*1024))*4 );

	ULONG addr = 0;
	err = BRD_ctrl( g_aDac[idxDac].handle, 0, BRDctrl_SDRAM_SETSTARTADDR, &addr); // set address for write
	err = BRD_ctrl( g_aDac[idxDac].handle, 0, BRDctrl_SDRAM_SETREADADDR, &addr);  // set address for read

	//
	// set size active zone (U32)
	//
	memSizew = (U32)(g_aDac[idxDac].outBufSizeb >> 2);
	err = BRD_ctrl( g_aDac[idxDac].handle, 0, BRDctrl_SDRAM_SETMEMSIZE, &memSizew);
	if( 0 > err )
	{
		DisplayError( err, __FUNCTION__, _BRDC("BRDctrl_SDRAM_SETMEMSIZE") );
		return -1;
	}

	__int64 nOutBufSizeb = ((__int64)memSizew) << 2;		 // get size active zone (bytes)
	if( nOutBufSizeb > nPhysMemSizeb)
		nOutBufSizeb = nPhysMemSizeb;
	if( g_aDac[idxDac].outBufSizeb > nOutBufSizeb)
		g_aDac[idxDac].outBufSizeb = (SBIG)nOutBufSizeb;

	return 0;
}

//=******************** SdramWriteDMA *********************
//=********************************************************
S32 SdramWriteDMA( void )
{
	//
	// Выполнить однократную запись данных в  SDRAM всех модулей ПДП-методом 
	//
	int				ii;
	S32				err;

	for( ii=0; ii<g_nDacNum; ii++)
	{
		err = SdramModulWriteDMA( ii );
		if( 0>err )
			return err;
	}
		

	return 1;
}

////=***************** SdramModulWriteDMA *******************
////=********************************************************
S32 SdramModulWriteDMA( int idxDac )
{
	//
	// Выполнить однократную запись данных в  SDRAM одного модуля ПДП-методом 
	//
	S32				err;
	int				ii;
	U32				drqFlag;  
	U32				msTimeout;
	int				nwrite;
	BRDctrl_StreamCBufStart rCBufStart;

	// set flag request DMA
	drqFlag = BRDstrm_DRQ_HALF;
	err = BRD_ctrl( g_aDac[idxDac].handle, 0, BRDctrl_STREAM_SETDRQ, &drqFlag );
	err = BRD_ctrl( g_aDac[idxDac].handle, 0, BRDctrl_SDRAM_FIFORESET, NULL );  // Сброс  FIFO SDRAM


	err = BRD_ctrl(g_aDac[idxDac].handle, 0, BRDctrl_STREAM_RESETFIFO, NULL); // сброс FIFO Stream

	nwrite = (int)(g_aDac[idxDac].outBufSizeb/g_nSdramWriteBufSize);
    BRDC_printf( _BRDC("To Stop press ESC...\n"));
    BRDC_printf( _BRDC("nwrite= %d\n"),nwrite);
	
	for( ii=0; ii<nwrite; ii++ )
	{
		S32		samplesPerBuf;

		samplesPerBuf = g_nSdramWriteBufSize / g_aDac[idxDac].sampleSizeb / g_aDac[idxDac].chanNum;

		CalcSignal( g_aDac[idxDac].rBufAlloc.ppBlk[0], samplesPerBuf, idxDac, ii );
	
		//
		// Вывести 1 буфер в SDRAM с помощью ПДП
		//
		rCBufStart.isCycle = 0; 
		err = BRD_ctrl( g_aDac[idxDac].handle, 0, BRDctrl_STREAM_CBUF_START, &rCBufStart );

		//
		// Ждать окончания вывода
		//
		msTimeout = 20000; 
		err = BRD_ctrl( g_aDac[idxDac].handle, 0, BRDctrl_STREAM_CBUF_WAITBUF, &msTimeout) ;
		if(BRD_errcmp(err, BRDerr_WAIT_TIMEOUT))
		{	
            BRDC_printf( _BRDC("TIME-OUT! \n"));
			err = BRD_ctrl( g_aDac[idxDac].handle, 0, BRDctrl_STREAM_CBUF_STOP, NULL ); // Стоп ПДП
			return  -1;
		}
        if(IPC_kbhit())
			if(0x1B == IPC_getch())
				return -1;

        BRDC_printf( _BRDC( "\rwrite = %d"), ii+1 );

	}
    BRDC_printf( _BRDC("\n"));

	return 0;
}

//=******************** SdramOutToDAC *********************
//=********************************************************
S32 SdramOutToDAC(void)
{		
	//
	// Выполнить однократный вывод данных из SDRAM в ЦАП ПДП-методом 
	//
	S32		err;
	int		ii, idx;
	U32		nStatus;
	U32		isEnable;
	U32		cycling;
	U32		statusFIFO;

	//
	// Стартовать все ЦАПы, начиная с последнего так, чтобы закончить МАСТЕРОМ
	//
	for( ii=g_nDacNum-1; ii>=0; ii-- )
	{
		idx = g_idx[ii];

		isEnable = 0;
		cycling = 0;
		err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_DAC_SETCYCLMODE, &cycling );
		err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_DAC_ENABLE, &isEnable ); // Стоп ЦАП 
		err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_DAC_FIFORESET, NULL ); // Сброс FIFO ЦАП

//		if(i==0) status = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_DAC_OUTSYNC,  NULL); //выдать импульс синхронизации (Master!)

		isEnable = 1;
		err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_SDRAM_ENABLE, &isEnable );    // Разрешить чтение из  SDRAM
		err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_DAC_ENABLE, &isEnable );	  // Старт ЦАП
	}

	//
	// Ждать окончания вывода из SDRAM
	//
	for( ii=0; ii<g_nDacNum; ii++ )
	{
		nStatus = 0;
		for(int jj = 0; jj < (1+g_nMsTimeout/50); jj++)
		{
			err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_SDRAM_ISACQCOMPLETE, &nStatus );
			if( nStatus )
				break;
			IPC_delay(50);
		}
		if( !nStatus )
			BRDC_printf(_BRDC( "\nTIME-OUT! status SDRAM = 0\n") );

		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_SDRAM_FLAGCLR, NULL );
		
		//
		// Запретить чтение из SDRAM
		//
		isEnable = 0;
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_SDRAM_ENABLE, &isEnable );
	}

	//
	// Ждать окончания выдачи блока из FIFO 
	//
	for( ii=0; ii<g_nDacNum; ii++ )
	{
		statusFIFO=0x4;

		//while((statusFIFO & (1<<2))==(1<<2))
		for(int jj = 0; jj < (1+g_nMsTimeout/50); jj++)
		{
			err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_DAC_FIFOSTATUS, &statusFIFO); 
			if((statusFIFO & 0x4) != 0x4 )
				break;
			IPC_delay(50);
		}
		if((statusFIFO & 0x4) == 0x4 )
			BRDC_printf(_BRDC("\nTIME-OUT! status FIFO = 0x%04X\n"), statusFIFO);
		
		//
		// Стоп  ЦАП
		//
		isEnable = 0;
		err = BRD_ctrl( g_aDac[ii].handle, 0, BRDctrl_DAC_ENABLE, &isEnable );
	}
	return 1;
}

//=******************** SdramCycleOutput ******************
//=********************************************************
S32 SdramCycleOutput( S32 cycle )
{
	//
	// Выполнить запись данных в SDRAM и запустить циклический (restart/retransmit) вывод в ЦАП из SDRAM 
	//
	S32				err;
	int				ii, idx;
	U32				isEnable;
	U32				cycling; // 0-restart,1-retransmit 

	//
	// Стартовать все ЦАПы, начиная с последнего так, чтобы закончить МАСТЕРОМ
	//
	for( ii=g_nDacNum-1; ii>=0; ii-- )
	{
		idx = g_idx[ii];

		isEnable = 0;
		err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_DAC_ENABLE, &isEnable ); // Стоп ЦАП 
		err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_DAC_FIFORESET, NULL ); // Сброс FIFO ЦАП

		cycling = cycle;
		err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_DAC_SETCYCLMODE, &cycling );
		
		// set flag request DMA
		// U32				drqFlag = BRDstrm_DRQ_HALF;

		isEnable = 0;
		err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_DAC_ENABLE, &isEnable ); // Стоп ЦАП 
		err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_DAC_FIFORESET, NULL ); // Сброс FIFO ЦАП

//		if(i==0) status = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_DAC_OUTSYNC,  NULL ); //выдать импульс синхронизации (Master!)

		isEnable = 1;
		err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_SDRAM_ENABLE, &isEnable );    // Разрешить чтение из  SDRAM
		err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_DAC_ENABLE, &isEnable );	  // Старт ЦАП
	}

	return 1;
}

#include "ctrlreg.h"
S32 RegRwSpd(BRD_Handle hDev, BRDCHAR* fname);

//=******************** PrepareStart **********************
//=********************************************************
S32 PrepareStart( )
{
	//
	// На все ЦАПы подать команду BRDctrl_DAC_PREPARESTART
	//
	S32				err;
	int				ii, idx;

	for( ii=g_nDacNum-1; ii>=0; ii-- )
	{
		idx = g_idx[ii];
		err = BRD_ctrl( g_aDac[idx].handle, 0, BRDctrl_DAC_PREPARESTART, NULL );
		if( err < 0 )
			if( !(BRD_errcmp( err, BRDerr_CMD_UNSUPPORTED ) 
			   || BRD_errcmp( err, BRDerr_INSUFFICIENT_SERVICES )) )
			{
				BRDC_printf(_BRDC("ERROR: Prepare Start has returned = 0x%X!!!\n"), err );
			}
	
		//
		// Если указан файл, то подгрузить регистры из файла
		//
		if( g_aDac[idx].sRegRwSpdFilename[0] )
			err = RegRwSpd( g_aDac[idx].handle & 0xFFFF, g_aDac[idx].sRegRwSpdFilename );
	}

	return 1;
}


//
// end of File
//

