/*
 * TF_TestPsd128Out.cpp
 *
 *  Created on: Jan 29, 2017
 *      Author: Dmitry Smekhov
 */
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "CL_AMBPEX.h"
#include "tf_test_psd128_out.h"
#include "table_engine.h"
#include "config_parser.h"
#include "gipcy.h"

//-----------------------------------------------------------------------------
using namespace std;
//-----------------------------------------------------------------------------

struct TaskDataPsd128Thread
{
    S32 m_RowNumber;	//!< Номер строки таблицы для вывода значений

    U32 CntBuffer;			// Число буферов стрима
    U32 CntBlockInBuffer;	// Число блоков в буфере
    U32 SizeBlockOfWords;	// Размер блока в словах
    U32 SizeBlockOfBytes;	// Размер блока в байтах
    U32 SizeBufferOfBytes;	// Размер буфера в байтах
    U32 SizeStreamOfBytes;	// Общий размер буфера стрима
    U32 isCycle;			// 1 - циклический режим работы стрима
    U32 isSystem;			// 1 - системная память
    U32 isAgreeMode;		// 1 - согласованный режим

    U32 trdNo;		// номер тетрады
    U32 strmNo;		// номер стрима
    U32 isSdram;	// 1 - проводить инициализацию SDRAM
    U32 isDDC;		// 1 - DDC, 2 - ADC
    U32 isTest;		// 1 - проверка псевдослучайной последовательности, 2 - проверка тестовой последовательности
    U32 isMainTest; // 1 - включение режима тестирования в тетараде MAIN
    U32 isShowParam; // 1 - отображение параметров

    U32 Mode;
    U32 BlockWr;
    U32 BlockRd;
    U32 BlockOk;
    U32 BlockError;
    U32 TotalError;
    U32 BlockLast;
    U32 DataType;	 	// Тип данных при фиксированном типе блока, 6 - счётчик, 8 - псевдослучайная последовательность
    U32 DataFix;		// 1 - фиксированный тип блока, 0 - данные в блоке записят от номера блока
    float	VelocityCurrent;
    float	VelocityAvarage;

    TF_TestBufM2 testBuf;

    U32 time_start;
    U32 time_last;
    U32 TrdStatus;
    U32 ViewRequest;

    TaskDataPsd128Thread()
    {
        CntBuffer = 8;			// Число буферов стрима
        CntBlockInBuffer = 1;	// Число блоков в буфере
        SizeBlockOfWords = 128*1024;	// Размер блока в словах
        SizeBlockOfBytes = 4*SizeBlockOfWords;	// Размер блока в байтах
        SizeBufferOfBytes = CntBlockInBuffer*SizeBlockOfBytes;	// Размер буфера в байтах
        SizeStreamOfBytes = CntBuffer*SizeBufferOfBytes;	// Общий размер буфера стрима
        isCycle = 1;			// 1 - циклический режим работы стрима
        isSystem = 1;			// 1 - системная память
        isAgreeMode = 1;		// 1 - согласованный режим

        trdNo = 8;		// номер тетрады
        strmNo = 1;		// номер стрима
        isSdram = 0;	// 1 - проводить инициализацию SDRAM
        isTest = 6;		// 1 - проверка псевдослучайной последовательности, 2 - проверка тестовой последовательности
        isMainTest = 0; // 1 - включение режима тестирования в тетараде MAIN
        isShowParam = 1;// 1 - отображение параметров

        Mode = 0;
        BlockWr = 0;
        BlockRd = 0;
        BlockOk = 0;
        BlockError = 0;
        TotalError = 0;
        BlockLast = 0;
        DataType = 0;	 	// Тип данных при фиксированном типе блока, 6 - счётчик, 8 - псевдослучайная последовательность
        DataFix = 0;		// 1 - фиксированный тип блока, 0 - данные в блоке записят от номера блока

        VelocityCurrent = 0;
        VelocityAvarage = 0;

        TrdStatus = 0;
        ViewRequest = 0;
    }

    TaskDataPsd128Thread(const std::vector<std::string>& optList) {

        TaskDataPsd128Thread();

        get_value(optList, "CntBuffer", CntBuffer);
        get_value(optList, "CntBlockInBuffer", CntBlockInBuffer);
        get_value(optList, "SizeBlockOfWords", SizeBlockOfWords);
        get_value(optList, "isCycle", isCycle);
        get_value(optList, "isSystem", isSystem);
        get_value(optList, "isAgreeMode", isAgreeMode);
        get_value(optList, "trdNo", trdNo);

        get_value(optList, "strmNo", strmNo);
        get_value(optList, "isTest", isTest);
        get_value(optList, "isMainTest", isMainTest);
        get_value(optList, "isShowParam", isShowParam);

        SizeBlockOfBytes = 4*SizeBlockOfWords;	// Размер блока в байтах
        SizeBufferOfBytes = CntBlockInBuffer*SizeBlockOfBytes;	// Размер буфера в байтах
        SizeStreamOfBytes = CntBuffer*SizeBufferOfBytes;	// Общий размер буфера стрима
    }
};

//-----------------------------------------------------------------------------

TF_TestPsd128Out::TF_TestPsd128Out(TableEngine *table, int row, CL_AMBPEX *brd, int argc, char **argv ) : TF_TestThread( argc, argv )
{
    char *cfgFile = GetConfigFileName(argc, argv, "-out");
    if(!cfgFile) {
        throw except_info("%s, %d: %s() - Not specified configuratipon file for DMA1!\n", __FILE__, __LINE__, __FUNCTION__);
    }

    std::vector<std::string> optList;
    if(!get_options(cfgFile, optList)) {
        throw except_info("%s, %d: %s() - Can't get options from configuratipon file for DMA1!\n", __FILE__, __LINE__, __FUNCTION__);
    }

    td = new TaskDataPsd128Thread(optList);
    if(td->isTest) {
        td->testBuf.buf_check_start(32,64);
    }

    m_brd  = brd;
    m_argc = argc;
    m_argv = argv;
    m_row = row;
    m_table = table;
}

//-----------------------------------------------------------------------------

TF_TestPsd128Out::~TF_TestPsd128Out()
{
    //fprintf(stderr, "TF_TestPsd128Out::%s(): ID - %ld\n", __FUNCTION__, pthread_self());
    delete td;
    td = 0;
}

//-----------------------------------------------------------------------------

void	TF_TestPsd128Out::StepMainThread()
{
    StepTable();
}

//-----------------------------------------------------------------------------

void	TF_TestPsd128Out::StepTable()
{
    //fprintf(stderr, "TF_TestPsd128Out::%s(): ID - %ld\n", __FUNCTION__, pthread_self());
    int col = 0;

    TestPsd128_ReadStatus();

    m_table->SetValueTable(m_row, col++, "%s", "TEST_PSD128");
    m_table->SetValueTable(m_row, col++, "%d", td->BlockWr);
    m_table->SetValueTable(m_row, col++, "%d", td->BlockRd);
    m_table->SetValueTable(m_row, col++, "%d", td->BlockOk);
    m_table->SetValueTable(m_row, col++, "%d", td->BlockError);
    m_table->SetValueTable(m_row, col++, "%10.1f", td->VelocityCurrent);
    m_table->SetValueTable(m_row, col++, "%10.1f", td->VelocityAvarage);
    m_table->SetValueTable(m_row, col++, "0x%04X", td->TrdStatus);

    U32 *pBlock = m_brd->StreamGetBufByNum(td->strmNo, 0);
    m_table->SetValueTable(m_row, col++, "0x%08X", pBlock[4]);

    td->ViewRequest = 1;
/*
	printf( " %7d %7d %8d %7d %7d %8d %7.1lf %7.1lf %7.1lf %7.1lf\r", blockRd, blockOk, blockError,
			td->hostBlockRd,
			//td->ptrMonitor->block[0].indexWr, td->hostMonitor->status[0].indexRd
			td->hostBlockOk,
			td->hostBlockError,
			td->velosityExtToCudaCurrent,
			td->velosityExtToCudaAvr,
			td->velosityCudaToHostCurrent,
			td->velosityCudaToHostAvr

			//td->hostMonitor->status[0].indexWr,
			//td->hostMonitor->status[0].indexRd
			);
            */
}

//-----------------------------------------------------------------------------

void TF_TestPsd128Out::PrepareInThread()
{
    PreparePSD128();

    m_brd->StreamInit(td->strmNo, td->CntBuffer, td->SizeBufferOfBytes, td->trdNo, 2, td->isCycle, td->isSystem, td->isAgreeMode);

    td->testBuf.buf_check_start(32,32);
/*
    U32 data_out = 0xA0000000;

    for(unsigned i=0; i<td->CntBuffer; i++) {
        U32 *pBlock = m_brd->StreamGetBufByNum(td->strmNo, i);
        if(pBlock) {
            for(unsigned j=0; j<td->SizeBufferOfBytes/4; j++) {
                pBlock[j] = data_out++;
            }
        }
    }
*/
    for(unsigned i=0; i<td->CntBuffer; i++) {
        U32 *pBlock = m_brd->StreamGetBufByNum(td->strmNo, i);
        if(pBlock) {
            td->testBuf.buf_set_psd64(pBlock,0,td->SizeBufferOfBytes/8,2);
            td->testBuf.buf_set_psd64(pBlock,1,td->SizeBufferOfBytes/8,2);
        }
    }

    //fprintf(stderr, "TF_TestPsd128Out::%s()\n", __FUNCTION__);
}

//-----------------------------------------------------------------------------

void TF_TestPsd128Out::CleanupInThread()
{
    //fprintf(stderr, "TF_TestPsd128Out::%s(): ID - %ld\n", __FUNCTION__, pthread_self());
    m_brd->StreamDestroy(td->strmNo);
}

//-----------------------------------------------------------------------------

void TF_TestPsd128Out::Run()
{
    //fprintf(stderr, "TF_TestPsd128Out::%s(): ID - %ld (START)\n", __FUNCTION__, pthread_self());
    //m_brd->RegPokeInd(td->trdNo, 0xC, 0x800);
    m_brd->RegPokeInd(td->trdNo, 0, 0x2);
    m_brd->RegPokeInd(td->trdNo, 0, 0x0);
    m_brd->RegPokeInd(td->trdNo, 0, 0x8);

    m_brd->StreamStart(td->strmNo);
    m_brd->RegPokeInd(td->trdNo, 0, 0x2038);

    int flagFirstBlock = 0;

    while(!m_isTerminate) {

        for(int kk=0; kk<16; kk++ )
        {
            U32 *ptr = 0;
            int ret = m_brd->StreamGetBuf( td->strmNo, &ptr );
            if( ret )
            { // Проверка буфера стрима


                if( 0==flagFirstBlock )
                {
                    td->time_last = td->time_start = IPC_getTickCount();

                    flagFirstBlock=1;
                }

                U32 *ptrBlock = 0;
                for( unsigned ii=0; ii<td->CntBlockInBuffer; ii++ )
                {
                    ptrBlock = ptr + ii*td->SizeBlockOfWords;


                    //m_pSharedIsvi->PutData( ptrBlock, SizeBlockOfBytes );
                    //if( 1==td->isTest )
                    //    td->testBuf.buf_check_psd( ptrBlock, td->SizeBlockOfWords );
                    if( 6==td->isTest )
                    {
                        td->testBuf.buf_set_psd64(ptrBlock,0,td->SizeBlockOfWords/2,2);
                        td->testBuf.buf_set_psd64(ptrBlock,1,td->SizeBlockOfWords/2,2);
                    }
                    //else if( 7==td->isTest )
                    //{
                    //   td->testBuf.buf_check_psd256( ptrBlock, td->SizeBlockOfWords, 0 );
                    //    td->testBuf.buf_check_psd256( ptrBlock, td->SizeBlockOfWords, 1 );
                    //    td->testBuf.buf_check_psd256( ptrBlock, td->SizeBlockOfWords, 2 );
                    //    td->testBuf.buf_check_psd256( ptrBlock, td->SizeBlockOfWords, 3 );
                    //}
                    td->BlockWr++;
                }


                if( td->isAgreeMode )
                {
                    m_brd->StreamGetBufDone( td->strmNo );
                }


            } else
            {
                break;
            }
        }
        //IPC_delay( 0 );


        U32 currentTime = IPC_getTickCount();
        if( (currentTime - td->time_last)>4000 )
        {
            float t1 = currentTime - td->time_last;
            float t2 = currentTime - td->time_start;
            float v = 1000.0*(td->BlockWr-td->BlockLast)*td->SizeBufferOfBytes/t1;
            v/=1024*1024;
            td->VelocityCurrent=v;

            v = 1000.0*(td->BlockWr)*td->SizeBlockOfBytes/t2;
            v/=1024*1024;
            td->VelocityAvarage=v;
            td->time_last = currentTime;
            td->BlockLast = td->BlockWr;
        }


        if(td->ViewRequest) {
            td->TrdStatus = m_brd->RegPeekDir(td->trdNo, 0);
            td->ViewRequest = 0;
        }

    }
    //fprintf(stderr, "TF_TestPsd128Out::%s(): ID - %ld (COMPLETE)\n", __FUNCTION__, pthread_self());
}

//-----------------------------------------------------------------------------

void TF_TestPsd128Out::GetResultInThread()
{


    if(td->isTest) {

        if( td->BlockOk>0 && td->BlockError==0 )
        {
          fprintf(stderr, "Process finished successfully.\n" );
        } else if( 0==td->BlockRd )
        {
            fprintf( stderr, "Process finished. Data not read\n");
        } else
        {
            TestPsd128_Result();
        }
    }
/*
    if(td->isTest) {
        fprintf(stderr, "%s\n", td->testBuf.report_word_error());
    } else {
        for(unsigned i=0; i<td->CntBuffer; i++) {
            U32 *buf = m_brd->StreamGetBufByNum( td->strmNo, i);
            if(buf) {
                fprintf(stderr, "%02d: %08X %08X %08X %08X\n", i, buf[0], buf[1], buf[2], buf[3]);
            }
        }
    }
*/


}

//-----------------------------------------------------------------------------

//! Чтение текущего состояния тетрады TRD_PSD128
void TF_TestPsd128Out::TestPsd128_ReadStatus()
{
    U32 block_rd, block_ok, block_error;
    U32 trd=td->trdNo;
    U32 reg_adr = 0x10;
    U32 reg_data = 0x208;

    U32 r0, r1;

    CL_AMBPEX *pBrd = m_brd;

    pBrd->RegPokeInd( trd, reg_adr, 0x4000 );
    r0 = pBrd->RegPeekInd( trd, reg_data );
    pBrd->RegPokeInd( trd, reg_adr, 0x4001 );
    r1 = pBrd->RegPeekInd( trd, reg_data );
    block_rd = (r1<<16) | r0;

    pBrd->RegPokeInd( trd, reg_adr, 0x4002 );
    r0 = pBrd->RegPeekInd( trd, reg_data );
    pBrd->RegPokeInd( trd, reg_adr, 0x4003 );
    r1 = pBrd->RegPeekInd( trd, reg_data );
    block_ok = (r1<<16) | r0;

    pBrd->RegPokeInd( trd, reg_adr, 0x4004 );
    r0 = pBrd->RegPeekInd( trd, reg_data );
    pBrd->RegPokeInd( trd, reg_adr, 0x4005 );
    r1 = pBrd->RegPeekInd( trd, reg_data );
    block_error = (r1<<16) | r0;

    td->BlockRd=block_rd;
    td->BlockOk=block_ok;
    td->BlockError=block_error;
}

//-----------------------------------------------------------------------------

//! Получение результата в тетраде TRD_PSD128
void TF_TestPsd128Out::TestPsd128_Result()
{
    BRDC_printf( _BRDC("\nРезультаты - канал 0\n\n"));
    GetTestResultChn( 0, 0 );

    BRDC_printf( _BRDC("\nРезультаты - канал 1\n\n"));
    GetTestResultChn( 0, 1 );
    BRDC_printf( _BRDC("\n-----------------------------\n\n"));
}

//! Получение одного слова из памяти результата
U32 TF_TestPsd128Out::GetTestCurrentItem(U32 base_adr, U32 reg)
{
    U32 trd=td->trdNo;

    U32 adr=0x10;
    U32 adr_r=0x208;

    CL_AMBPEX *pBrd = m_brd;

    U32 d, dx, d0, d1, ii;
    for( ii=0; ii<100; ii++ )
    {
        pBrd->RegPokeInd( trd, adr, reg );
        d0=pBrd->RegPeekInd( trd, adr_r ) & 0xFFFF;;
        pBrd->RegPokeInd( trd, adr, reg+1 );
        d1=pBrd->RegPeekInd( trd, adr_r ) & 0xFFFF;;
        d=(d1<<16) | d0;
        pBrd->RegPokeInd( trd, adr, reg );
        d0=pBrd->RegPeekInd( trd, adr_r ) & 0xFFFF;;
        pBrd->RegPokeInd( trd, adr, reg+1);
        d1=pBrd->RegPeekInd( trd, adr_r ) & 0xFFFF;;
        dx=(d1<<16) | d0;
        if( d==dx )
            break;
    }
    return dx;
}

//! Получение результатов теста
void TF_TestPsd128Out::GetTestResultChn(U32 adr, U32 chn)
{

    U32 base=chn*0x1000;
    U32 total_err;
    total_err=GetTestCurrentItem( adr, base+0x100 );
    U32 cnt=total_err;
    if( cnt>16 )
        cnt=16;

    cnt=8;
    U32 ii;
    U32 word;
    for( ii=0; ii<cnt; ii++ )
    {
        word=base+ii*0x10;
        __int64 data_rd, data_expect, tmp;
        U32 index, block;
        data_rd = GetTestCurrentItem( adr, word+0 );
        tmp =GetTestCurrentItem( adr, word+2 );
        data_rd |= tmp << 32;

        data_expect = GetTestCurrentItem( adr, word+4 );
        tmp =GetTestCurrentItem( adr, word+6 );
        data_expect |= tmp << 32;

        index = GetTestCurrentItem( adr, word+8 ) & 0xFFFF;

        block = GetTestCurrentItem( adr, word+9 );

        tmp = data_expect ^ data_rd;
        BRDC_printf( _BRDC("%4d  block: %-4d  index: %08X  expect: %016llX  receive: %016llX xor: %016llX \r\n"),
            ii, block, index, data_expect, data_rd, tmp );
    }
}

//! Подготовка тетрады TRD_PSD128
void TF_TestPsd128Out::PreparePSD128()
{
    CL_AMBPEX *pBrd = m_brd;
    U32 trd=td->trdNo;

    pBrd->RegPokeInd( trd, 0xB, 0 ); // MODE3 - PSD

    pBrd->RegPokeInd( trd, 0x18, 2 );
    pBrd->RegPokeInd( trd, 0x19, 0 );
    pBrd->RegPokeInd( trd, 0x1A, 0 );
    pBrd->RegPokeInd( trd, 0x1B, 0 );

    pBrd->RegPokeInd( trd, 0x1C, 0xAA55 );
    pBrd->RegPokeInd( trd, 0x1D, 0 );
    pBrd->RegPokeInd( trd, 0x1E, 0 );
    pBrd->RegPokeInd( trd, 0x1F, 0 );

}
