/*
 ****************** File CtrlDio16Out.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : DIO16OUT section
 *
 * (C) InSys by Dorokhin Andrey Feb, 2006
 *
 * 04.09.2006 - change BRDctrl_DIO16IN_SETCLKMODE/BRDctrl_DIO16IN_GETCLKMODE
 *
 ************************************************************
*/

#ifndef _CTRL_DIO16OUT_H
 #define _CTRL_DIO16OUT_H

#include "ctrladmpro.h"
#include "ctrldio.h"

enum {
	BRDctrl_DIO16OUT_SETPHASE		= BRDctrl_DIOOUT + 0,	// Set DIO16OUT phase clock (S32)
	BRDctrl_DIO16OUT_GETPHASE		= BRDctrl_DIOOUT + 1,	// Get DIO16OUT phase clock (S32)
	// ver. 2.00- !!!
//	BRDctrl_DIO16OUT_SETCLKMODE		= BRDctrl_DIOOUT + 12,	// Set DIO16OUT clock inverting (U32)
//	BRDctrl_DIO16OUT_GETCLKMODE		= BRDctrl_DIOOUT + 13,	// Get DIO16OUT clock inverting (U32)
	// ver. 2.10 only !!!
	BRDctrl_DIO16OUT_SETCLKMODE		= BRDctrl_DIOOUT + 12,	// Set DIO16OUT clock mode (BRD_DioClkMode)
	BRDctrl_DIO16OUT_GETCLKMODE		= BRDctrl_DIOOUT + 13,	// Get DIO16OUT clock mode (BRD_DioClkMode)

	BRDctrl_DIO16OUT_SETOUTDATA		= BRDctrl_DIOOUT + 22,	// Set DIO16OUT output data counter (PBRD_EnVal)
	BRDctrl_DIO16OUT_GETOUTDATA		= BRDctrl_DIOOUT + 23,	// Get DIO16OUT output data counter (PBRD_EnVal)
	BRDctrl_DIO16OUT_SETSKIPDATA	= BRDctrl_DIOOUT + 24,	// Set DIO16OUT skipped data counter (PBRD_EnVal)
	BRDctrl_DIO16OUT_GETSKIPDATA	= BRDctrl_DIOOUT + 25,	// Get DIO16OUT skipped data counter (PBRD_EnVal)

	BRDctrl_DIO16OUT_SETCYCLMODE	= BRDctrl_DIOOUT + 26,	// Set DIO16OUT cycling mode (U32)
	BRDctrl_DIO16OUT_GETCYCLMODE	= BRDctrl_DIOOUT + 27,	// Get DIO16OUT cycling mode (U32)

	BRDctrl_DIO16OUT_FIFORESET		= BRDctrl_DIOOUT + 128,	// DIO16OUT FIFO reset (NULL)
	BRDctrl_DIO16OUT_ENABLE			= BRDctrl_DIOOUT + 129,	// DIO16OUT enable / disable (NULL)
	BRDctrl_DIO16OUT_FIFOSTATUS		= BRDctrl_DIOOUT + 130,	// Get DIO16OUT FIFO status (U32)
	BRDctrl_DIO16OUT_PUTDATA		= BRDctrl_DIOOUT + 131,	// Put data buffer into DIO16OUT FIFO (PVOID)
	BRDctrl_DIO16OUT_GETSRCSTREAM	= BRDctrl_DIOOUT + 138,	// Get source address for stream (U32)

	BRDctrl_DIO16OUT_ILLEGAL
};

#endif // _CTRL_DIO16OUT_H

//
// End of file
//