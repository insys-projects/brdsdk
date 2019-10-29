/*
 ****************** File ctrlfmc127p.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : FMC127P section
 *
 * (C) InSys by Dorokhin Andrey Dec 2015
 *
 *
 ************************************************************
*/

#ifndef _CTRL_FMC127P_H
 #define _CTRL_FMC127P_H

#include "ctrladmpro.h"

enum {
	BRDctrl_FMC127P_SETSWITCH		= BRDctrl_CMN + 0,		// write switch input/output (PBRD_Fmc127pSwitch)
	BRDctrl_FMC127P_GETSWITCH		= BRDctrl_CMN + 1,		// read switch input/output (PBRD_Fmc127pSwitch)
	BRDctrl_FMC127P_SENSMON			= BRDctrl_CMN + 3,		// get value from sensors: Voltage, Current, Temperature (PBRD_SensMonVal)
	BRDctrl_FMC127P_SETCLKMODE		= BRDctrl_CMN + 4,		// set clock parameters (PBRD_Fmc127pClkMode)
	BRDctrl_FMC127P_GETCLKMODE		= BRDctrl_CMN + 5,		// get clock parameters (PBRD_Fmc127pClkMode)
	BRDctrl_FMC127P_WRITEUROM		= BRDctrl_CMN + 6,		// write user EEPROM (PBRD_UserEeprom)
	BRDctrl_FMC127P_READUROM		= BRDctrl_CMN + 7,		// read user EEPROM (PBRD_UserEeprom)

	BRDctrl_FMC127P_ILLEGAL
};

#pragma pack(push, 1)    

typedef struct _BRD_SensMonVal {
	U32		chip;	// IN number of chip (0-2)
	U32		mode;	// IN control registers (6 and 7) of chip
	REAL64 	inp[8]; // OUT external input voltage
	REAL64 	cur[4]; // OUT external input current
	REAL64 	tint;	// OUT temperature internal
	REAL64 	vcc;	// OUT Vcc
} BRD_SensMonVal, *PBRD_SensMonVal;

typedef struct _BRD_Fmc127pSwitch {
	U32 out;				// - output of switch
	U32 inp;				// - input of switch
} BRD_Fmc127pSwitch, *PBRD_Fmc127pSwitch;

typedef struct _BRD_Fmc127pClkMode {
	U32		genEn;				// IN - enable internal generator 
	U32		clkInCmpEn;			// IN - enable comparator of external input CLKIN
	REAL64	clkInValue;			// IN - value (Hz) of external input CLKIN
} BRD_Fmc127pClkMode, *PBRD_Fmc127pClkMode;

typedef struct _BRD_UserEeprom { 
	U32 offset;					// IN - offset into EEPROM (bytes)
	void *pBuf;					// IN/OUT - data buffer pointer
	U32 size;					// IN - data buffer size (bytes)
} BRD_UserEeprom, *PBRD_UserEeprom;

#pragma pack(pop)    

#endif // _CTRL_FMC127P_H

//
// End of file
//
