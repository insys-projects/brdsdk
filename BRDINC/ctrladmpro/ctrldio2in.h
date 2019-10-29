/*
 ****************** File CtrlDio2In.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : DIO2IN section
 *
 * (C) InSys by Dorokhin Andrey Nov, 2007
 *
 *
 ************************************************************
*/

#ifndef _CTRL_DIO2IN_H
 #define _CTRL_DIO2IN_H

#include "ctrladmpro.h"

// Clock sources
enum {
	//BRDclks_BASEDDS		= 10,	// 
	BRDclks_DIO2		= 12,	//
	BRDclks_TTLIN		= 13,	//
	BRDclks_TTLOUT		= 14	//
};

enum {

	BRDctrl_DIO2IN_SETRATE			= BRDctrl_DIOIN + 10,	// Set DIO2IN sampling rate in Hz (REAL64)
	BRDctrl_DIO2IN_GETRATE			= BRDctrl_DIOIN + 11,	// Get DIO2IN sampling rate in Hz (REAL64)
	BRDctrl_DIO2IN_SETCLKMODE		= BRDctrl_DIOIN + 12,	// Set DIO2IN clock mode(BRD_ClkMode)
	BRDctrl_DIO2IN_GETCLKMODE		= BRDctrl_DIOIN + 13,	// Get DIO2IN clock mode(BRD_ClkMode)
	BRDctrl_DIO2IN_SETSYNCMODE		= BRDctrl_DIOIN + 14,	// Set DIO2IN synchronization mode (BRD_SyncMode)
	BRDctrl_DIO2IN_GETSYNCMODE		= BRDctrl_DIOIN + 15,	// Get DIO2IN synchronization mode (BRD_SyncMode)
	BRDctrl_DIO2IN_SETMASTER		= BRDctrl_DIOIN + 16,	// Set DIO2IN master/single/slave mode (U32)
	BRDctrl_DIO2IN_GETMASTER		= BRDctrl_DIOIN + 17,	// Get DIO2IN master/single/slave mode (U32)
	BRDctrl_DIO2IN_SETSTARTMODE		= BRDctrl_DIOIN + 18,	// Set DIO2IN start mode (BRD_StartMode)
	BRDctrl_DIO2IN_GETSTARTMODE		= BRDctrl_DIOIN + 19,	// Get DIO2IN start mode (BRD_StartMode)
	BRDctrl_DIO2IN_SETCMPLEVEL		= BRDctrl_DIOIN + 20,	// Set DIO2IN comparator level (BRD_CmpLevel)
	BRDctrl_DIO2IN_GETCMPLEVEL		= BRDctrl_DIOIN + 21,	// Get DIO2IN comparator level (BRD_CmpLevel)

	BRDctrl_DIO2IN_SETCNT			= BRDctrl_DIOIN + 22,	// Set DIO2IN CNT0,CNT1,CNT2 (BRD_CntParam)
	BRDctrl_DIO2IN_GETCNT			= BRDctrl_DIOIN + 23,	// Get DIO2IN CNT0,CNT1,CNT2 (BRD_CntParam)

	BRDctrl_DIO2IN_SETSYMBSYNC		= BRDctrl_DIOIN + 24,	// Get DIO2IN comparator level (BRD_CmpLevel)

	BRDctrl_DIO2IN_FIFORESET		= BRDctrl_DIOIN + 128,	// DIO2IN FIFO reset (NULL)
	BRDctrl_DIO2IN_ENABLE			= BRDctrl_DIOIN + 129,	// DIO2IN disable (NULL)
	BRDctrl_DIO2IN_FIFOSTATUS		= BRDctrl_DIOIN + 130,	// Get DIO2IN FIFO status (U32)
	BRDctrl_DIO2IN_GETDATA			= BRDctrl_DIOIN + 131,	// Put data buffer into DIO2IN FIFO (PVOID)
	BRDctrl_DIO2IN_GETSRCSTREAM		= BRDctrl_DIOIN + 138,	// Get source address for stream (U32)

	BRDctrl_DIO2IN_ILLEGAL
};

#pragma pack(push, 1)

typedef struct _BRD_CmpLevel {
	U32		num;			// Comparator number
	REAL64	level;			// Comparator level value
} BRD_CmpLevel, *PBRD_CmpLevel;

typedef struct _BRD_CntParam {
	U32		num;			// Counter number
	U32		count;			// Counter value
} BRD_CntParam, *PBRD_CntParam;

#pragma pack(pop)

#endif // _CTRL_DIO2IN_H

//
// End of file
//