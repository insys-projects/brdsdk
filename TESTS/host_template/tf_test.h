/*
 * tf_test.h
 *
 *  Created on: Jan 29, 2017
 *      Author: Dmitry Smekhov
 */

#ifndef TF_TEST_H_
#define TF_TEST_H_


/**
 * 	\brief	Base class for testing device
 */
class TF_Test
{

public:
    TF_Test() {}

    virtual ~TF_Test() {}

    virtual int 	Prepare(int cnt) = 0;

    virtual void	Start() = 0;

    virtual void 	Stop() = 0;

    virtual int		isComplete() = 0;

    virtual void	StepTable() = 0;

    virtual void	StepMainThread() = 0;

    virtual void	GetResult() = 0;
};




#endif /* TF_TEST_H_ */
