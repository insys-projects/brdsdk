
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <assert.h>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

#include "CL_AMBPEX.h"
#include "exceptinfo.h"
#include "config_parser.h"
#include "tf_test.h"
#include "tf_testthread.h"
#include "table_engine_console.h"
#include "tf_test_main_thread.h"
#include "tf_test_psd128_out.h"
#include "tf_test_ddr_in_thread.h"
#include "gipcy.h"

using namespace std;

static volatile int exit_flag = 0;

void signal_handler(int signo)
{
    signo = signo;
    exit_flag = 1;
    printf("\n");
}

int main(int argc, char **argv)
{
    // дополнительно тестируем второй канал на ввод
    char *strm_in = GetConfigFileName(argc, argv, "-in");
    char *strm_out = GetConfigFileName(argc, argv, "-out");
    char *ddr_in = GetConfigFileName(argc, argv, "-ddr_in");
    char *ddr_out = GetConfigFileName(argc, argv, "-ddr_out");
/*
    if(ddr_in) {
        fprintf(stderr, "Config file: %s\n", ddr_in);
        std::vector<std::string> optList;
        get_options(ddr_in, optList);
        for(unsigned i=0; i<optList.size(); i++) {
            fprintf(stderr, "%s\n", optList[i].c_str());
        }
        int val = 0x5ABC;
        if(get_value(optList, "SdramTestSequence", val)) {
            fprintf(stderr, "Hex Options: 0x%x\n", val);
        } else {
            fprintf(stderr, "Oooooops\n");
        }

        double dval = 0;
        if(get_value(optList, "DoubleVar", dval)) {
            fprintf(stderr, "Double Options: %.3f\n", dval);
        } else {
            fprintf(stderr, "Oooooops\n");
        }

        if(get_value(optList, "isSdram", val)) {
            fprintf(stderr, "Hex Options: 0x%x\n", val);
        } else {
            fprintf(stderr, "Oooooops\n");
        }
    }

    return 0;
*/
    int ok = strm_in || strm_out || ddr_in || ddr_out;

    if(!ok) {
        fprintf(stderr, "No configuration files for DMA channel select.\n");
        fprintf(stderr, "Example for testing DMA in both directions:\n");
        fprintf(stderr, "%s -in rd.cfg -out wr.cfg -ddr_in ddr_in.cfg -ddr_out ddr_out.cfg\n", argv[0]);
        fprintf(stderr, "rd.cfg - config file for DMA0.\n");
        fprintf(stderr, "wr.cfg - config file for DMA1.\n");
        return -1;
    }

    const char *headers[] = {
        "NAME", "BLOCK_WR", "BLOCK_RD", "BLOCK_OK", "BLOCK_ERROR", "SPD_CURRENT", "SPD_AVR", "STATUS", "OTHER",
    };

    TableEngine  *pTable = new TableEngineConsole();
    pTable->CreateTable(headers, sizeof(headers)/sizeof(headers[0]));

    // добавить строку в таблицу нужно
    // для каждого теста заранее
    int row1, row2, row3;
    if(strm_in) { row1 = pTable->AddRowTable(); }
    if(strm_out) { row2 = pTable->AddRowTable(); }
    if(ddr_in) { row3 = pTable->AddRowTable(); }

    signal(SIGINT, signal_handler);

    //IPC_initKeyboard();

    try {

        //fprintf(stderr, "%s(): BRD init start\n", __FUNCTION__);

        CL_AMBPEX brd(1);

        //fprintf(stderr, "%s(): BRD init complete\n", __FUNCTION__);

        std::vector<TF_Test*> test;
        if(strm_in) {
            TF_Test *pTestMain = new TF_TestMainThread(pTable, row1, &brd, argc, argv);
            test.push_back(pTestMain);
        }

        if(strm_out) {
            TF_Test *pTestPsd = new TF_TestPsd128Out(pTable, row2, &brd, argc, argv);
            test.push_back(pTestPsd);
        }

        //fprintf(stderr, "%s(): Create DDR test\n", __FUNCTION__);

        if(ddr_in) {
            TF_Test *pTestDdrIn = new TF_TestDdrInThread(pTable, row3, &brd, argc, argv);
            test.push_back(pTestDdrIn);
        }

        //fprintf(stderr, "%s(): Start prepare\n", __FUNCTION__);

        for(unsigned j=0; j<test.size(); j++) {
            for( int ii=0; ; ii++) {
                if( test[j]->Prepare(ii) )
                    break;
            }
        }

        unsigned working_tests = 0;
        for(unsigned j=0; j<test.size(); j++) {
            test[j]->Start();
            working_tests |= (1 << j);
        }

       //fprintf(stderr, "%s(): Start tests... working_tests = %x\n", __FUNCTION__, working_tests);

#if defined(IS_STEP_MAIN_THREAD)
        struct timeval start_time;
        struct timeval curr_time;
#endif

        // Сохраним координаты курсора, перед первым выводом в таблицу
        // для последующего вывода информации с нужной строки
        int X = 0;
        int Y = 0;
        pTable->GetConsolePos(X,Y);

        while(1) {

#if defined(IS_STEP_MAIN_THREAD)
            IPC_getTime(&start_time);

            for(;;) {
                // Тестирование реализовано в функции StepInMainThread()
                for(unsigned j=0; j<test.size(); j++) {
                    test[j]->StepMainThread();
                }
                IPC_getTime(&curr_time);
                if(IPC_getDiffTime(&start_time, &curr_time) > 100) {
                    break;
                }
                IPC_delay(10);
            }
#else
            // Тестирование реализовано в отдельном потоке
            IPC_delay(100);
#endif
            for(unsigned j=0; j<test.size(); j++) {
                if(test[j]->isComplete() || exit_flag) {
                    test[j]->Stop();
                    working_tests &= ~(1 << j);
                   //fprintf(stderr, "\n%s(): Stop tests... working_tests = %x\n", __FUNCTION__, working_tests);
                }
            }

            if(!working_tests) {
               //fprintf(stderr, "%s(): Exit tests... working_tests = %x\n", __FUNCTION__, working_tests);
                break;
            }
#if !defined(IS_STEP_MAIN_THREAD)
            for(unsigned j=0; j<test.size(); j++) {
                if(working_tests & (1 << j))
                    test[j]->StepTable();
            }
#endif

            //if(IPC_kbhit()) {
            //    exit_flag = 1;
            //}
        }

        // Восстановим координаты курсора для того, чтобы
        // печатаемый текст не накладывался на уже выведенный
        pTable->SetConsolePos(0, Y+1);

       //fprintf(stderr, "%s(): Start GetResult\n", __FUNCTION__);

        for(unsigned j=0; j<test.size(); j++) {
            test[j]->GetResult();
        }

       //fprintf(stderr, "%s(): Start deleting tests...\n", __FUNCTION__);
        IPC_delay(3000);

        for(unsigned j=0; j<test.size(); j++) {
            delete test[j];
        }

       //fprintf(stderr, "%s(): Start exit from try block...\n", __FUNCTION__);

    } catch(except_info_t& err) {
        fprintf(stderr, "%s\n", err.info.c_str());
    } catch( ... ) {
        fprintf(stderr, "Unknown error in application!\n");
    }

    //IPC_delay(1000);

    delete pTable;

    //IPC_cleanupKeyboard();
    //fprintf(stderr, "%s(): Exit from application...\n", __FUNCTION__);

    return 0;
}

