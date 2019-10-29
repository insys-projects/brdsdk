/*
 ****************** File CtrlTtlOut.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : TTLOUT section
 *
 * (C) InSys by Dorokhin Andrey Nov, 2007
 *
 *
 ************************************************************
*/

#ifndef _CTRL_TTLOUT_H
 #define _CTRL_TTLOUT_H

#include "ctrladmpro.h"

enum {
	BRDctrl_TTLOUT_SETRATE			= BRDctrl_DIOOUT + 10,	// Set TTLOUT sampling rate in Hz (REAL64)
	BRDctrl_TTLOUT_GETRATE			= BRDctrl_DIOOUT + 11,	// Get TTLOUT sampling rate in Hz (REAL64)
	BRDctrl_TTLOUT_SETCLKMODE		= BRDctrl_DIOOUT + 12,	// Set TTLOUT clock mode(BRD_ClkMode)
	BRDctrl_TTLOUT_GETCLKMODE		= BRDctrl_DIOOUT + 13,	// Get TTLOUT clock mode(BRD_ClkMode)
	BRDctrl_TTLOUT_SETSYNCMODE		= BRDctrl_DIOOUT + 14,	// Set TTLOUT synchronization mode (BRD_SyncMode)
	BRDctrl_TTLOUT_GETSYNCMODE		= BRDctrl_DIOOUT + 15,	// Get TTLOUT synchronization mode (BRD_SyncMode)
	BRDctrl_TTLOUT_SETMASTER		= BRDctrl_DIOOUT + 16,	// Set TTLOUT master/single/slave mode (U32)
	BRDctrl_TTLOUT_GETMASTER		= BRDctrl_DIOOUT + 17,	// Get TTLOUT master/single/slave mode (U32)
	BRDctrl_TTLOUT_SETSTARTMODE		= BRDctrl_DIOOUT + 18,	// Set TTLOUT start mode (BRD_StartMode)
	BRDctrl_TTLOUT_GETSTARTMODE		= BRDctrl_DIOOUT + 19,	// Set TTLOUT start mode (BRD_StartMode)

	BRDctrl_TTLOUT_FIFORESET		= BRDctrl_DIOOUT + 128,	// TTLOUT FIFO reset (NULL)
	BRDctrl_TTLOUT_ENABLE			= BRDctrl_DIOOUT + 129,	// TTLOUT disable (NULL)
	BRDctrl_TTLOUT_FIFOSTATUS		= BRDctrl_DIOOUT + 130,	// Get TTLOUT FIFO status (U32)
	BRDctrl_TTLOUT_PUTDATA			= BRDctrl_DIOOUT + 131,	// Put data buffer into TTLOUT FIFO (PVOID)
	BRDctrl_TTLOUT_GETSRCSTREAM		= BRDctrl_DIOOUT + 138,	// Get source address for stream (U32)

	BRDctrl_TTLOUT_ILLEGAL
};

#endif // _CTRL_TTLOUT_H

//
// End of file
//