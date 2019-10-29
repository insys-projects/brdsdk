/*
 * TF_TestCnt.h
 *
 *  Created on: Jan 29, 2017
 *      Author: Dmitry Smekhov
 */

#ifndef TF_TEST_MAIN_THREAD_OUT_H_
#define TF_TEST_MAIN_THREAD_OUT_H_

#include "tf_testthread.h"
#include "TF_TestBufM2.h"

struct TaskDataPsd128Thread;
struct TaskBufferStatus;
class CL_AMBPEX;
class TableEngine;

class TF_TestPsd128Out: public TF_TestThread
{
public:
    TF_TestPsd128Out(TableEngine *table, int row, CL_AMBPEX *brd, int argc, char **argv);
    virtual ~TF_TestPsd128Out();

    void PrepareInThread();
    void CleanupInThread();
    void GetResultInThread();
    void StepMainThread();
    void StepTable();
    void Run();

private:
    CL_AMBPEX *m_brd;
	int	m_argc;
	char** m_argv;
    struct TaskDataPsd128Thread *td;
    TableEngine *m_table;
    int m_row;
    TF_TestBufM2 m2;

    //! Чтение текущего состояния тетрады TRD_PSD128
    void TestPsd128_ReadStatus();

    //! Получение результата в тетраде TRD_PSD128
    void TestPsd128_Result();

    //! Получение одного слова из памяти результата
    U32 GetTestCurrentItem(U32 base_adr, U32 reg);

    //! Получение результатов теста
    void GetTestResultChn(U32 adr, U32 chn);

    //! Подготовка тетрады TRD_PSD128
    void PreparePSD128();
};

#endif /* TF_TEST_MAIN_THREAD_OUT_H_ */
