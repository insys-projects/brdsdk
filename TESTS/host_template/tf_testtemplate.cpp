/*
 * TF_TestTemplate.cpp
 *
 *  Created on: Jan 29, 2017
 *      Author: Dmitry Smekhov
 */
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "tf_testtemplate.h"

//-----------------------------------------------------------------------------

struct TaskDataTemplate {
    TaskDataTemplate() {}
};

//-----------------------------------------------------------------------------

double getTime(void)
{
	clock_t t=clock();
    double ret = t / (double)CLOCKS_PER_SEC;
	return ret;
}

//-----------------------------------------------------------------------------

TF_TestTemplate::TF_TestTemplate( int argc, char **argv ) : TF_TestThread( argc, argv )
{
    //fprintf(stderr, "TF_TestTemplate::%s(): ID - %ld\n", __FUNCTION__, pthread_self());

        td = new TaskDataTemplate;

    //td->countOfCycle   = GetFromCommnadLine( argc, argv, "-count", 16 );
    //td->sizeBufferOfKb = GetFromCommnadLine( argc, argv, "-size", 256);

	m_argc=argc;
	m_argv=argv;
}

//-----------------------------------------------------------------------------

TF_TestTemplate::~TF_TestTemplate()
{
    //fprintf(stderr, "TF_TestTemplate::%s(): ID - %ld\n", __FUNCTION__, pthread_self());
    delete td;
    td = 0;
}

//-----------------------------------------------------------------------------

void	TF_TestTemplate::StepTable()
{
    //fprintf(stderr, "TF_TestTemplate::%s(): ID - %ld\n", __FUNCTION__, pthread_self());

    //if( 0 != td->countOfCycle )
    //	return;

    //unsigned blockRd=0;
    //unsigned blockOk=0;
    //unsigned blockError=0;

	for( int ii=0; ii<3; ii++ )
	{
//        /m_table->SetValueTable(m_row, 0, "%s", "TEST_MAIN");
	}
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

void TF_TestTemplate::PrepareInThread()
{
    //fprintf(stderr, "TF_TestTemplate::%s(): ID - %ld\n", __FUNCTION__, pthread_self());
}

//-----------------------------------------------------------------------------

void TF_TestTemplate::CleanupInThread()
{
    //fprintf(stderr, "TF_TestTemplate::%s(): ID - %ld\n", __FUNCTION__, pthread_self());
}

//-----------------------------------------------------------------------------

void TF_TestTemplate::Run()
{
    //fprintf(stderr, "TF_TestTemplate::%s(): ID - %ld (START)\n", __FUNCTION__, pthread_self());
    while(!m_isTerminate) {
        sleep(1);
    }
    //fprintf(stderr, "TF_TestTemplate::%s(): ID - %ld (COMPLETE)\n", __FUNCTION__, pthread_self());
}

//-----------------------------------------------------------------------------

void TF_TestTemplate::GetResultInThread()
{
    //fprintf(stderr, "TF_TestTemplate::%s(): ID - %ld\n", __FUNCTION__, pthread_self());
}

//-----------------------------------------------------------------------------
