/*
 ****************** File CtrlTtlIn.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : TTLIN section
 *
 * (C) InSys by Dorokhin Andrey Nov, 2007
 *
 *
 ************************************************************
*/

#ifndef _CTRL_TTLIN_H
 #define _CTRL_TTLIN_H

#include "ctrladmpro.h"

enum {

	BRDctrl_TTLIN_SETRATE			= BRDctrl_DIOIN + 10,	// Set TTLIN sampling rate in Hz (REAL64)
	BRDctrl_TTLIN_GETRATE			= BRDctrl_DIOIN + 11,	// Get TTLIN sampling rate in Hz (REAL64)
	BRDctrl_TTLIN_SETCLKMODE		= BRDctrl_DIOIN + 12,	// Set TTLIN clock mode(BRD_ClkMode)
	BRDctrl_TTLIN_GETCLKMODE		= BRDctrl_DIOIN + 13,	// Get TTLIN clock mode(BRD_ClkMode)
	BRDctrl_TTLIN_SETSYNCMODE		= BRDctrl_DIOIN + 14,	// Set TTLIN synchronization mode (BRD_SyncMode)
	BRDctrl_TTLIN_GETSYNCMODE		= BRDctrl_DIOIN + 15,	// Get TTLIN synchronization mode (BRD_SyncMode)
	BRDctrl_TTLIN_SETMASTER			= BRDctrl_DIOIN + 16,	// Set TTLIN master/single/slave mode (U32)
	BRDctrl_TTLIN_GETMASTER			= BRDctrl_DIOIN + 17,	// Get TTLIN master/single/slave mode (U32)
	BRDctrl_TTLIN_SETSTARTMODE		= BRDctrl_DIOIN + 18,	// Set TTLIN start mode (BRD_StartMode)
	BRDctrl_TTLIN_GETSTARTMODE		= BRDctrl_DIOIN + 19,	// Set TTLIN start mode (BRD_StartMode)
	BRDctrl_TTLIN_SETSYMBSYNC		= BRDctrl_DIOIN + 20,	// Set TTLIN start mode (BRD_StartMode)

	BRDctrl_TTLIN_FIFORESET			= BRDctrl_DIOIN + 128,	// TTLIN FIFO reset (NULL)
	BRDctrl_TTLIN_ENABLE			= BRDctrl_DIOIN + 129,	// TTLIN disable (NULL)
	BRDctrl_TTLIN_FIFOSTATUS		= BRDctrl_DIOIN + 130,	// Get TTLIN FIFO status (U32)
	BRDctrl_TTLIN_GETDATA			= BRDctrl_DIOIN + 131,	// Put data buffer into TTLIN FIFO (PVOID)
	BRDctrl_TTLIN_GETSRCSTREAM		= BRDctrl_DIOIN + 138,	// Get source address for stream (U32)

	BRDctrl_TTLIN_ILLEGAL
};

#endif // _CTRL_TTLIN_H

//
// End of file
//