/*
 ****************** File CtrlBasef.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : BASEFMC section
 *
 * (C) InSys by Dorokhin Andrey Mar 2012
 *
 * - 10.10.2014 - add BRDctrl_BASEF_SWITCHDEV
 *
 ************************************************************
*/

#ifndef _CTRL_BASEF_H
 #define _CTRL_BASEF_H

#include "ctrladmpro.h"

enum {
	BRDctrl_BASEF_SETSWITCH		= BRDctrl_CMN + 0,		// write switch input/output (PBRD_Switch)
	BRDctrl_BASEF_GETSWITCH		= BRDctrl_CMN + 1,		// read switch input/output (PBRD_Switch)
	BRDctrl_BASEF_SWITCHONOFF	= BRDctrl_CMN + 2,		// on/off switch output (PBRD_Switch)
	BRDctrl_BASEF_GETONOFF		= BRDctrl_CMN + 3,		// read status switch output  - on or off (PBRD_Switch)
	BRDctrl_BASEF_SETCLKMODE	= BRDctrl_CMN + 4,		// set clock parameters (PBRD_BasefClkMode)
	BRDctrl_BASEF_GETCLKMODE	= BRDctrl_CMN + 5,		// get clock parameters (PBRD_BasefClkMode)
	BRDctrl_BASEF_SWITCHDEV		= BRDctrl_CMN + 6,		// set device for programming switch (BASEF_SWITCH_DEV)
	
	BRDctrl_BASEF_ILLEGAL
};

#pragma pack(push, 1)    

typedef struct _BRD_Switch {
	U32 out;				// - output of switch
	U32 val;				// - input of switch or on/off switch
} BRD_Switch, *PBRD_Switch;

typedef struct _BRD_BasefClkMode {
	U32		genEn;				// IN - enable internal generator
	U32		clkInCmpEn;			// IN - enable comparator of external input CLKIN
	REAL64	clkValue;			// IN/OUT - value (Hz) of internal generator (programmable or not)
} BRD_BasefClkMode, *PBRD_BasefClkMode;

// Switch device IDs
typedef enum _BASEF_SWITCH_DEV {
	BASEFsd_CLOCK		= 1,
	BASEFsd_SYNC		= 2,
} BASEF_SWITCH_DEV;

#pragma pack(pop)    

#endif // _CTRL_BASEF_H

//
// End of file
//
