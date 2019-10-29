/*
 ****************** File CtrlCmpSc.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : CmpSc section (start & clock comparators)
 *
 * (C) InSys by Dorokhin Andrey Feb 2004
 *
 * 14.11.2006 - add BRDctrl_CMPSC_GETDLGPAGES
 * 03.11.2010 - add BRDctrl_CMPSC_SETEXT, BRDctrl_CMPSC_GETEXT, BRDctrl_CMPSC_GETCFG
 *
 ************************************************************
*/

#ifndef _CTRL_CMPSC_H
 #define _CTRL_CMPSC_H

#include "ctrladmpro.h"

enum {
	BRDctrl_CMPSC_SET				=	BRDctrl_CMN + 0,	// Set comparators (BRD_CmpSC)
	BRDctrl_CMPSC_GET				=	BRDctrl_CMN + 1,	// Get comparators (BRD_CmpSC)
	BRDctrl_CMPSC_SETEXT			=	BRDctrl_CMN + 2,	// Set comparators (BRD_CmpNum)
	BRDctrl_CMPSC_GETEXT			=	BRDctrl_CMN + 3,	// Get comparators (BRD_CmpNum)
	BRDctrl_CMPSC_WRDACREG			=	BRDctrl_CMN + 4,	// Write DAC registers (BRD_DacReg)
	BRDctrl_CMPSC_GETCFG			=	BRDctrl_CMN + 5,	// Get Comparators/multiplexer configuration (PBRD_ComparCfg)

	BRDctrl_CMPSC_GETDLGPAGES		=	BRDctrl_DAC + 127, // get property pages of comparators for dialog (PBRD_PropertyList)

	BRDctrl_CMPSC_ILLEGAL
};

#pragma pack(push, 1)    

typedef struct _BRD_CmpSC {
	U32		src;				// Comparator source 
	REAL64	thr[BRD_CMPNUM];	// Comparators Thresholds values 
} BRD_CmpSC, *PBRD_CmpSC;

typedef struct _BRD_CmpNum {
	U32		compar;	// Comparator number
	U32		src;	// Comparator source
	REAL64	thr;	// Comparator Threshold value 
} BRD_CmpNum, *PBRD_CmpNum;

// getting configuration
typedef struct _BRD_ComparCfg {
	U32		RefPVS;						// Basic Voltage (опорное напряжение источников программируемых напряжений (мВольт))
	U32		Bits;						// разрядность
	U32		ThdacType;					// тип поддерживаемого источника программируемых напряжений: 0 - 8-битные пороговые ЦАП, 1 - 12/14/16-битные
	U32		MuxType;					// 1 - доступен расширенный режим управления мультиплексорами компараторов
} BRD_ComparCfg, *PBRD_ComparCfg;

typedef struct _BRD_DacReg {
	U32		num;	// Threshold DAC number
	U32		reg;	// Threshold DAC register
	U32		data;	// Threshold DAC data
} BRD_DacReg, *PBRD_DacReg;

#pragma pack(pop)    

#endif // _CTRL_CMPSC_H

//
// End of file
//