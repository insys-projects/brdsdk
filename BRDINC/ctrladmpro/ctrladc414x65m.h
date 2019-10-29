/*
 ****************** File CtrlAdc414x65m.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : ADC414x65M section
 *
 * (C) InSys by Dorokhin Andrey Aug, 2006
 *
 *
 ************************************************************
*/

#ifndef _CTRL_ADC414X65M_H
 #define _CTRL_ADC414X65M_H

#include "ctrladc.h"

#pragma pack(push, 1)    

//const int BRD_CHANCNT	= 4; // Number of channels
#define BRD_CHANCNT			4

const int BRD_CLKDIVCNT	= 7; // Number of clock dividers = 7 (1, 2, 4, 8, 16, 32, 64)
/*
// for set and get Clock mode
typedef struct _BRD_AdcClkMode {
	U32		src;	// IN - 1 - clock source on submodule
	BRD_ClkMode baseClk;
} BRD_AdcClkMode, *PBRD_AdcClkMode;

// for set and get Syncronization mode
typedef struct _BRD_AdcSyncMode 
{
	U32		src;	// IN - 1 - clock source on submodule
	BRD_SyncMode baseSync;
} BRD_AdcSyncMode, *PBRD_AdcSyncMode;
*/
// ADC414X65M Clock sources
enum {
	BRDclks_ADC_DISABLED	= 0,	// 
	BRDclks_ADC_SUBGEN1		= 1,	// SubModule Generator 1
	BRDclks_ADC_SUBGEN2		= 2,	// SubModule Generator 2
	BRDclks_ADC_SUBGEN3		= 3,	// SubModule Generator 3
	BRDclks_ADC_EXTCLK		= 0x81,	// External SubModule Clock
};

const int BRD_GAINCNT	= 8; // Number of gains

// input gain
const double BRD_Gain[BRD_GAINCNT] =
{
	0.05,	// 0x00
	0.1,	// 0x01
	0.25,	// 0x02
	0.5,	// 0x03
	1.0,	// 0x04
	2.0,	// 0x05
	5.0,	// 0x06
	10.0	// 0x07
};

// input range (Volt)
const double BRD_Range[BRD_GAINCNT] =
{
	5.0,	//10.0,	 
	2.5,	//5.0,	 
	1.0,	//2.0,	 
	0.5,	//1.0,	 
	0.25,	//0.5,	 
	0.125,	//0.25,	 
	0.05,	//0.1,	 
	0.025	//0.05	 
};

#pragma pack(pop)    

#endif // _CTRL_ADC414X65M_H

//
// End of file
//