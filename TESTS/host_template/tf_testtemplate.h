/*
 * TF_TestCnt.h
 *
 *  Created on: Jan 29, 2017
 *      Author: Dmitry Smekhov
 */

#ifndef TF_TESTTEMPLATE_H_
#define TF_TESTTEMPLATE_H_

#include "tf_testthread.h"

struct TaskDataTemplate;
struct TaskBufferStatus;

class TF_TestTemplate: public TF_TestThread
{
public:
    TF_TestTemplate(int argc, char **argv);
    virtual ~TF_TestTemplate();

    virtual void PrepareInThread();
    virtual void CleanupInThread();
    virtual void GetResultInThread();
    virtual void StepTable();
    virtual void Run();

private:
	int	m_argc;
	char** m_argv;
    struct TaskDataTemplate *td;
};

#endif /* TF_TESTTEMPLATE_H_ */
