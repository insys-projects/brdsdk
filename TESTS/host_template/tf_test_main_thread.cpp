/*
 * TF_TestMainThread.cpp
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
#include "TF_WorkParam.h"
#include "tf_test_main_thread.h"
#include "table_engine.h"
#include "exceptinfo.h"
#include "config_parser.h"
#include "gipcy.h"

//-----------------------------------------------------------------------------
using namespace std;
//-----------------------------------------------------------------------------

struct TaskDataMainThread
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

    TaskDataMainThread()
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

        trdNo = 0;		// номер тетрады
        strmNo = 0;		// номер стрима
        isSdram = 0;	// 1 - проводить инициализацию SDRAM
        isTest = 6;		// 1 - проверка псевдослучайной последовательности, 2 - проверка тестовой последовательности
        isMainTest = 1; // 1 - включение режима тестирования в тетараде MAIN
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

    TaskDataMainThread(const std::vector<std::string>& optList)
    {
        TaskDataMainThread();

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

TF_TestMainThread::TF_TestMainThread(TableEngine *table, int row, CL_AMBPEX *brd, int argc, char **argv ) : TF_TestThread( argc, argv )
{
    char *cfgFile = GetConfigFileName(argc, argv, "-in");
    if(!cfgFile) {
        throw except_info("%s, %d: %s() - Not specified configuratipon file for DMA0!\n", __FILE__, __LINE__, __FUNCTION__);
    }

    std::vector<std::string> optList;
    if(!get_options(cfgFile, optList)) {
        throw except_info("%s, %d: %s() - Can't get options from configuratipon file for DMA0!\n", __FILE__, __LINE__, __FUNCTION__);
    }

    td = new TaskDataMainThread(optList);
    if(td->isTest) {
        td->testBuf.buf_check_start(32,64);
    }

    m_brd  = brd;
    m_argc = argc;
    m_argv = argv;
    m_row = row;
    m_table = table;

   //fprintf(stderr, "TF_TestMainThread::%s()\n", __FUNCTION__);
}

//-----------------------------------------------------------------------------

TF_TestMainThread::~TF_TestMainThread()
{
    delete td;
    td = 0;
   //fprintf(stderr, "TF_TestMainThread::%s()\n", __FUNCTION__);
}

//-----------------------------------------------------------------------------

void	TF_TestMainThread::StepMainThread()
{
    StepTable();
}

//-----------------------------------------------------------------------------

void	TF_TestMainThread::StepTable()
{
    //fprintf(stderr, "TF_TestMainThread::%s(): ID - %ld\n", __FUNCTION__, pthread_self());
    int col = 0;

    td->testBuf.check_result(&td->BlockOk, &td->BlockError, 0, 0, 0);

    m_table->SetValueTable(m_row, col++, "%s", "TEST_MAIN");
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

void TF_TestMainThread::PrepareInThread()
{
    //fprintf(stderr, "TF_TestMainThread::%s(): ID - %ld\n", __FUNCTION__, pthread_self());
    m_brd->StreamInit(td->strmNo, td->CntBuffer, td->SizeBufferOfBytes, td->trdNo, 1, td->isCycle, td->isSystem, td->isAgreeMode);
    U32 *pBlock = m_brd->StreamGetBufByNum(td->strmNo, 0);
    pBlock[4] = 0xBABEFACE;
}

//-----------------------------------------------------------------------------

void TF_TestMainThread::CleanupInThread()
{
   //fprintf(stderr, "TF_TestMainThread::%s(): (START)\n", __FUNCTION__);
    m_brd->StreamDestroy(td->strmNo);
   //fprintf(stderr, "TF_TestMainThread::%s(): (COMPLETE)\n", __FUNCTION__);
}

//-----------------------------------------------------------------------------

void TF_TestMainThread::Run()
{
   //fprintf(stderr, "TF_TestMainThread::%s(): (START)\n", __FUNCTION__);

    //m_brd->RegPokeInd(td->trdNo, 0xC, 0x800);
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

                    FILE *fo = fopen( "data_first.bin", "wb" );
                    if( fo )
                    {
                        fwrite( ptr, td->SizeBlockOfWords, 4, fo );
                        fclose( fo );
                    }

                    //flagFirstBlock=1;
                }

                if( (6==td->isTest) || (7==td->isTest) || (8==td->isTest) || (9==td->isTest))
                    td->testBuf.chn_start();

                U32 *ptrBlock = 0;
                for( unsigned ii=0; ii<td->CntBlockInBuffer; ii++ )
                {
                    ptrBlock = ptr+ii*td->SizeBlockOfWords;


                if( 0==flagFirstBlock )
                {
                    FILE *fo = fopen( "data_first_xx.bin", "wb" );
                    if( fo )
                    {
                        fwrite( ptr, td->SizeBlockOfWords, 4, fo );
                        fclose( fo );
                    }


                    flagFirstBlock=1;
                }
                {

                    volatile int val=0;
                    for( U32 ii=0; ii<td->SizeBlockOfWords; ii++ )
                    {
                        val +=ptrBlock[ii];
                    }

                }

//                    m_pSharedIsvi->PutData( ptrBlock, SizeBlockOfBytes );
                    if( 1==td->isTest )
                        td->testBuf.buf_check_psd( ptrBlock, td->SizeBlockOfWords );
                    else if( 6==td->isTest )
                    {
                        td->testBuf.buf_check_psd128_low( ptrBlock, td->SizeBlockOfWords );
                        td->testBuf.buf_check_psd128_high( ptrBlock, td->SizeBlockOfWords );
                    }
                    else if( 7==td->isTest )
                    {
                        td->testBuf.buf_check_psd256( ptrBlock, td->SizeBlockOfWords, 0 );
                        td->testBuf.buf_check_psd256( ptrBlock, td->SizeBlockOfWords, 1 );
                        td->testBuf.buf_check_psd256( ptrBlock, td->SizeBlockOfWords, 2 );
                        td->testBuf.buf_check_psd256( ptrBlock, td->SizeBlockOfWords, 3 );
                    }
                    td->BlockRd++;
                }


                if( (6==td->isTest) || (7==td->isTest) )
                    td->testBuf.chn_step();

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
            float v = 1000.0*(td->BlockRd-td->BlockLast)*td->SizeBufferOfBytes/t1;
            v/=1024*1024;
            td->VelocityCurrent=v;

            v = 1000.0*(td->BlockRd)*td->SizeBlockOfBytes/t2;
            v/=1024*1024;
            td->VelocityAvarage=v;
            td->time_last = currentTime;
            td->BlockLast = td->BlockRd;
        }


        if(td->ViewRequest) {
            td->TrdStatus = m_brd->RegPeekDir(td->trdNo, 0);
            td->ViewRequest = 0;
        }

    }

   //fprintf(stderr, "TF_TestMainThread::%s(): (COMPLETE)\n", __FUNCTION__);
}

//-----------------------------------------------------------------------------

void TF_TestMainThread::GetResultInThread()
{
   //fprintf(stderr, "TF_TestMainThread::%s(): (START)\n", __FUNCTION__);

    td->testBuf.check_result(&td->BlockOk, &td->BlockError, 0, 0, 0);

    if(td->isTest) {

        if( td->BlockOk>0 && td->BlockError==0 )
        {
          fprintf(stderr, "Process finished successfully.\n" );
        } else if( 0==td->BlockRd )
        {
            fprintf( stderr, "Process finished. Data not read\n");
        } else
        {
//            fprintf(stderr, "%s\n", td->testBuf.report_word_error());

              fprintf( stderr, "\nError list. Channel 0:\n");
              fprintf(stderr, "%s\n", td->testBuf.report_word_error( 0 ));

              fprintf( stderr, "\n\nError list. Channel 1:\n");

              fprintf(stderr, "%s\n", td->testBuf.report_word_error(1 ));

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
   //fprintf(stderr, "TF_TestMainThread::%s(): (COMPLETE)\n", __FUNCTION__);
}

//-----------------------------------------------------------------------------
