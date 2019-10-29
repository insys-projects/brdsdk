/*
 * TF_TestCnt.h
 *
 *  Created on: Jan 29, 2017
 *      Author: Dmitry Smekhov
 */

#ifndef TF_TEST_DDR_IN_THREAD_H_
#define TF_TEST_DDR_IN_THREAD_H_

#include "PrepareDDR.h"
#include "tf_testthread.h"
#include "TF_TestBufM2.h"

struct TaskDataDdrThread;
struct TaskBufferStatus;
class CL_AMBPEX;
class TableEngine;

class TF_TestDdrInThread: public TF_TestThread
{
public:
    TF_TestDdrInThread(TableEngine *table, int row, CL_AMBPEX *brd, int argc, char **argv);
    virtual ~TF_TestDdrInThread();

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
    struct TaskDataDdrThread *td;
    TableEngine *m_table;
    int m_row;
    TF_TestBufM2 m2;
};

#endif /* TF_TEST_DDR_IN_THREAD_H_ */
