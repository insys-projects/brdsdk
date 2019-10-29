/*
 ****************** File CtrlAdc212x200m.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : ADC212x200M section
 *
 * (C) InSys by Dorokhin Andrey Apr, 2004
 *
 *
 ************************************************************
*/

#ifndef _CTRL_ADC212x200M_H
 #define _CTRL_ADC212x200M_H

#include "ctrladc.h"

#pragma pack(push, 1)    

//const int BRD_ADC212x200M_CHANCNT	= 2; // Number of channels
//const int BRD_CHANCNT	= 2; // Number of channels
#define BRD_CHANCNT			2

const int BRD_CLKDIVCNT	= 4; // Number of clock dividers = 4 (1, 2, 4, 8)

//typedef struct _BRD_Adc212x200mClkMode {
//	U32		src;	// IN - clock source
//	REAL64	value;	// IN (external clock) / OUT (real clock) 
//	U32		dblMode;// IN - double clock mode
//} BRD_Adc212x200mClkMode, *PBRD_Adc212x200mClkMode;

// ADC212x200M Clock sources
enum {
	BRDclks_ADC_DISABLED	= 0,	// 
	BRDclks_ADC_SUBGEN		= 1,	// SubModule Generator
	BRDclks_ADC_EXTCLK		= 0x81,	// External SubModule Clock
};

// ADC212x200M start sources
enum {
	BRDsts_ADC_CHAN0	= 0,	// Start from channel 0
	BRDsts_ADC_CHAN1	= 1,	// Start from channel 1
	BRDsts_ADC_EXT		= 2,	// Start from START IN (external)
	BRDsts_ADC_PRG		= 3,	// Program start
//	BRDsts_ADC_EXTX2	= 7,	// Start from X2
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
	10.0,	// 0x00
	5.0,	// 0x01
	2.0,	// 0x02
	1.0,	// 0x03
	0.5,	// 0x04
	0.25,	// 0x05
	0.1,	// 0x06
	0.05	// 0x07
};

#pragma pack(pop)    

#endif // _CTRL_ADC212x200M_H

//
// End of file
//