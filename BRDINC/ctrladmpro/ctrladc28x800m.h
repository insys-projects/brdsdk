/*
 ****************** File CtrlAdc28x800m.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : ADC28x800M section
 *
 * (C) InSys by Dorokhin Andrey Dec, 2004
 *
 *
 ************************************************************
*/

#ifndef _CTRL_ADC28x800M_H
 #define _CTRL_ADC28x800M_H

#include "ctrladc.h"

#pragma pack(push, 1)    

#define BRDctrl_ADC_SETCLKTUNING	BRDctrl_ADC_SETCLKTHR	// Set ADC clock frequency tuning in % (REAL64)
#define BRDctrl_ADC_GETCLKTUNING	BRDctrl_ADC_GETCLKTHR	// Get ADC clock frequency tuning in % (REAL64)

#define BRD_CHANCNT			2  // Number of channels

const int BRD_CLKDIVCNT		= 6; // Number of clock dividers = 6 (1, 2, 4, 8, 16, 32)
const int BRD_CLKAUXDIVCNT	= 2; // Number of auxiliary clock dividers = 2 (1, 2)

// ADC28x800M Clock sources
enum {
	BRDclks_ADC_DISABLED	= 0,	// 
	BRDclks_ADC_SUBMAINGEN	= 1,	// SubModule Main Generator
	BRDclks_ADC_SUBVCOGEN	= 2,	// SubModule Auxiliary VCO
	BRDclks_ADC_EXTCLK		= 0x81,	// External SubModule Clock
	BRDclks_ADC_MASTERCLK	= 0xff,	// External Clock from MASTER card (synthesizer)
};

// ADC28x800M start sources
enum {
	BRDsts_ADC_CHAN0	= 0,	// Start from channel 0
	BRDsts_ADC_CHAN1	= 1,	// Start from channel 1
	BRDsts_ADC_EXT		= 2,	// Start from START IN (external)
	BRDsts_ADC_PRG		= 3,	// Program start
};

const int BRD_GAINCNT	= 4; // Number of gains

// input gain
const double BRD_Gain[BRD_GAINCNT] =
{
	0.125,	// 0x00
	0.25,	// 0x01
	0.5,	// 0x02
	1.0		// 0x03
};

// input range (Volt)
const double BRD_Range[BRD_GAINCNT] =
{
	4.0,	// 0x00
	2.0,	// 0x01
	1.0,	// 0x02
	0.5		// 0x03
};

#pragma pack(pop)    

#endif // _CTRL_ADC28x800M_H

//
// End of file
//