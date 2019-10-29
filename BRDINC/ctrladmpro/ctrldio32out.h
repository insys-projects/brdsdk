/*
 ****************** File CtrlDio32Out.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : DIO32_OUT section
 *
 * (C) InSys by Dorokhin Andrey Sep, 2007
 *
 *
 ************************************************************
*/

#ifndef _CTRL_DIO32OUT_H
 #define _CTRL_DIO32OUT_H

#include "ctrladmpro.h"

enum {
	BRDctrl_DIO32OUT_SETCHANMASK	= BRDctrl_DIOOUT + 0,	// Set mask of DIO32OUT channels (U32)
	BRDctrl_DIO32OUT_GETCHANMASK	= BRDctrl_DIOOUT + 1,	// Get mask of DIO32OUT channels (U32)
	BRDctrl_DIO32OUT_SETRATE		= BRDctrl_DIOOUT + 10,	// Set DIO32OUT sampling rate in Hz (REAL64)
	BRDctrl_DIO32OUT_GETRATE		= BRDctrl_DIOOUT + 11,	// Get DIO32OUT sampling rate in Hz (REAL64)
	BRDctrl_DIO32OUT_SETCLKMODE		= BRDctrl_DIOOUT + 12,	// Set DIO32OUT clock mode (BRD_Dio32outClkMode)
	BRDctrl_DIO32OUT_GETCLKMODE		= BRDctrl_DIOOUT + 13,	// Get DIO32OUT clock mode (BRD_Dio32outClkMode)
	BRDctrl_DIO32OUT_SETMASTER		= BRDctrl_DIOOUT + 16,	// Set DIO32OUT master/single/slave mode (U32)
	BRDctrl_DIO32OUT_GETMASTER		= BRDctrl_DIOOUT + 17,	// Get DIO32OUT master/single/slave mode (U32)
	BRDctrl_DIO32OUT_SETSTARTMODE	= BRDctrl_DIOOUT + 18,	// Set DIO32OUT start mode (BRD_StartMode)
	BRDctrl_DIO32OUT_GETSTARTMODE	= BRDctrl_DIOOUT + 19,	// Set DIO32OUT start mode (BRD_StartMode)

	BRDctrl_DIO32OUT_SETPNPKMODE	= BRDctrl_DIOOUT + 64,// Set PNPK mode for DIO32OUT (U32)
	BRDctrl_DIO32OUT_GETPNPKMODE	= BRDctrl_DIOOUT + 65,// Get PNPK mode for DIO32OUT (U32)
	BRDctrl_DIO32OUT_SETINITMODE	= BRDctrl_DIOOUT + 66,// Set INIT mode for DIO32OUT (BRD_Dio32outInitMode)
	BRDctrl_DIO32OUT_GETINITMODE	= BRDctrl_DIOOUT + 67,// Get INIT mode for DIO32OUT (BRD_Dio32outInitMode)

	BRDctrl_DIO32OUT_FIFORESET		= BRDctrl_DIOOUT + 128,	// DIO32OUT enable (NULL)
	BRDctrl_DIO32OUT_ENABLE			= BRDctrl_DIOOUT + 129,	// DIO32OUT disable (NULL)
	BRDctrl_DIO32OUT_FIFOSTATUS		= BRDctrl_DIOOUT + 130,	// Get DIO32OUT FIFO status (U32)
	BRDctrl_DIO32OUT_PUTDATA		= BRDctrl_DIOOUT + 131,	// Put data buffer into DIO32OUT FIFO (PVOID)
	BRDctrl_DIO32OUT_GETSRCSTREAM	= BRDctrl_DIOOUT + 138,	// Get source address for stream (U32)

	BRDctrl_DIO32OUT_FLAGCLR		= BRDctrl_DIOOUT + 144,	// DIO32OUT FIFO status flag clear (U32)

	BRDctrl_DIO32OUT_ILLEGAL
};

#pragma pack(push, 1)    

typedef struct _BRD_Dio32outClkMode {
	U32		src;	// IN - clock source
	REAL64	value;	// IN (external clock) /OUT (real clock) 
	U32		inv;	// IN - 1 = ADM Clock Inverting
	U32		mode;	// IN - 0 = continue, 1 - only data transfering
} BRD_Dio32outClkMode, *PBRD_Dio32outClkMode;

typedef struct _BRD_Dio32outInitMode {
	U32		mode;	// IN - 00 - , 01 - reserved, 10 - , 11 - Out value
	U32		out;	// IN - output INIT-signal by Mode=11
} BRD_Dio32outInitMode, *PBRD_Dio32outInitMode;

#pragma pack(pop)    

#endif // _CTRL_DIO32OUT_H

//
// End of file
//