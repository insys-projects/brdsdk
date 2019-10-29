/*
 ****************** File CtrlTest.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : Test section
 *
 * (C) Instrumental Systems 
 *
 * Created: by Dorokhin A. Aug, 2005
 *
 * 02.10.2007 - add TESTIN
 *
 ************************************************************
*/

#ifndef _CTRL_TEST_H
 #define _CTRL_TEST_H

#include "ctrladmpro.h"

enum {
	BRDctrl_TEST_SETMODE		= BRDctrl_TEST + 0,	// Set Test mode (U32)
	BRDctrl_TEST_GETMODE		= BRDctrl_TEST + 1,	// Get Test mode (U32)

	BRDctrl_TEST_GETCFG			= BRDctrl_TEST + 55,	// Get Test configuration (PBRD_TestCfg)
//	BRDctrl_TEST_GETDLGPAGES	= BRDctrl_TEST + 127,	// get property pages of Test for dialog (PBRD_PropertyList)

	BRDctrl_TEST_FIFORESET		= BRDctrl_TEST + 128,	// Test FIFO reset (NULL)
	BRDctrl_TEST_ENABLE			= BRDctrl_TEST + 129,	// Enable/disable write to Test (U32)
	BRDctrl_TEST_FIFOSTATUS		= BRDctrl_TEST + 130,	// Get Test FIFO status (U32)
	BRDctrl_TEST_GETDATA		= BRDctrl_TEST + 131,	// Get data buffer (BRD_DataBuf)
	BRDctrl_TEST_SETDATA		= BRDctrl_TEST + 132,	// Set data buffer (BRD_DataBuf)

	BRDctrl_TEST_GETSRCSTREAM	= BRDctrl_TEST + 138,	// Get source address for stream (U32)

	BRDctrl_TEST_ILLEGAL
};

#pragma pack(push, 1) 

// getting configuration
typedef struct _BRD_TestCfg
{
	U32		FifoSize;					// размер FIFO (в байтах)
} BRD_TestCfg, *PBRD_TestCfg;

#pragma pack(pop)    

#endif // _CTRL_TEST_H

//
// End of file
//