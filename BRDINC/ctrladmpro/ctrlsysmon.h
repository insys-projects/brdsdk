/*
 ****************** File ctrlsysmon.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : CMN section
 *
 * (C) InSys by Dorokhin Andrey Mar 2012
 *
 ************************************************************
*/

#ifndef _CTRL_MON_H
 #define _CTRL_MON_H

#include "ctrladmpro.h"

enum {
	BRDctrl_SYSMON_GETTEMP		= BRDctrl_CMN + 0,		// read temperatures (PBRD_SysMonVal)
	BRDctrl_SYSMON_GETVCCINT	= BRDctrl_CMN + 1,		// read voltages of VCCint(PBRD_SysMonVal)
	BRDctrl_SYSMON_GETVCCAUX	= BRDctrl_CMN + 2,		// read voltages of VCCaux (PBRD_SysMonVal)
	BRDctrl_SYSMON_GETVREFP		= BRDctrl_CMN + 3,		// read voltage of Vrefp (REAL64*)
	BRDctrl_SYSMON_GETVREFN		= BRDctrl_CMN + 4,		// read voltage of Vrefn (REAL64*)
	BRDctrl_SYSMON_GETSTATUS	= BRDctrl_CMN + 5,		// read system monitor status (U32)
	BRDctrl_SYSMON_GETVNOMINALS	= BRDctrl_CMN + 6,		// get voltage nominals (PBRD_VoltNominals)
	BRDctrl_SYSMON_GETVN7S		= BRDctrl_CMN + 7,		// get voltage nominals for 7-series FPGA (PBRD_VoltNominals7s)
	BRDctrl_SYSMON_GETVCCBRAM	= BRDctrl_CMN + 8,		// read voltages of VCCbram for 7-series FPGA (PBRD_SysMonVal)

	BRDctrl_SYSMON_ILLEGAL
};

#pragma pack(push, 1)    

typedef struct _BRD_SysMonVal {
	REAL64 curv;
	REAL64 minv;
	REAL64 maxv;
} BRD_SysMonVal, *PBRD_SysMonVal;

typedef struct _BRD_VoltNominals {
	REAL64 vccint;
	REAL64 vccaux;
	REAL64 vrefp;
	REAL64 vrefn;
} BRD_VoltNominals, *PBRD_VoltNominals;

typedef struct _BRD_VoltNominals7s {
	REAL64 vccint;
	REAL64 vccaux;
	REAL64 vrefp;
	REAL64 vrefn;
	REAL64 vccbram;
} BRD_VoltNominals7s, *PBRD_VoltNominals7s;

#pragma pack(pop)    

#endif // _CTRL_MON_H

//
// End of file
//