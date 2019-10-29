/*
 * TF_TestThread.h
 *
 *  Created on: Jan 29, 2017
 *      Author: Dmitry Smekhov
 */

#ifndef TF_TestThread_H_
#define TF_TestThread_H_

#include <pthread.h>
#include "tf_test.h"


/**
 *	\brief	Base class for application with thread
 *
 *
 *
 */
class TF_TestThread: public TF_Test
{
public:
    TF_TestThread(int argc, char **argv);

	virtual ~TF_TestThread();

    // TF_Test virtual functions
    int 	Prepare(int cnt);
    void	Start();
    void 	Stop();
    int		isComplete();
    void    GetResult();

    static void*    ThreadFunc(void* lpvThreadParm);

    void* Execute();

    virtual void PrepareInThread() = 0;
    virtual void CleanupInThread() = 0;
    virtual void GetResultInThread() = 0;
    virtual void StepMainThread() = 0;
    virtual void StepTable() = 0;
    virtual void Run() = 0;

	int	m_isPrepareComplete;
	int	m_isComplete;
	int m_isTerminate;
	int	m_CycleCnt;

    pthread_mutex_t		m_ResultStartMutex;
    pthread_mutex_t		m_ResultCompleteMutex;
    pthread_mutex_t		m_StartMutex;
    pthread_mutex_t		m_ThreadExitMutex;

    pthread_t 			m_hThread;
    pthread_attr_t  	m_attrThread;
};

#endif /* TF_TestThread_H_ */
