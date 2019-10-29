/*
 * TF_TestCnt.h
 *
 *  Created on: Jan 29, 2017
 *      Author: Dmitry Smekhov
 */

#ifndef TF_TEST_MAIN_THREAD_H_
#define TF_TEST_MAIN_THREAD_H_

#include "tf_testthread.h"
#include "TF_TestBufM2.h"

struct TaskDataMainThread;
struct TaskBufferStatus;
class CL_AMBPEX;
class TableEngine;

class TF_TestMainThread: public TF_TestThread
{
public:
    TF_TestMainThread(TableEngine *table, int row, CL_AMBPEX *brd, int argc, char **argv);
    virtual ~TF_TestMainThread();

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
    struct TaskDataMainThread *td;
    TableEngine *m_table;
    int m_row;
    TF_TestBufM2 m2;
};

#endif /* TF_TEST_MAIN_THREAD_H_ */
