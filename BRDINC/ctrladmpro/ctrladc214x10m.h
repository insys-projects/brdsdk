/*
 ****************** File CtrlAdc214x10m.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : ADM214x10M section
 *
 * (C) InSys by Ekkore Aug, 2009
 *
 *
 ************************************************************
*/

#ifndef _CTRL_ADC214x10M_H
 #define _CTRL_ADC214x10M_H

#include "CtrlAdc.h"

#pragma pack(push, 1)    

#define BRD_CHANCNT			16	// Number of channels

const int BRD_CLKDIVCNT	= 5; // Number of clock dividers = 9 (1, 2, 4, 8, 16)

// ADC214x10M Clock sources
enum {
	BRDclks_ADC_DISABLED	= 0,	// 
	BRDclks_ADC_SUBGEN0		= 1,	// SubModule Generator 0
	BRDclks_ADC_SUBGEN1		= 2,	// SubModule Generator 1
	BRDclks_ADC_EXTCLK		= 0x81,	// External SubModule Clock
};

// ADC214x10M start sources
/*
enum {
	BRDsts_ADC_CHAN0	= 0,	// Start from channel 0
	BRDsts_ADC_CHAN1	= 1,	// Start from channel 1
	BRDsts_ADC_EXT		= 2,	// Start from START IN (external)
	BRDsts_ADC_PRG		= 3,	// Program start
};
*/

const int BRD_GAINCNT	= 4; // Number of gains

// input gain
const double BRD_Gain[BRD_GAINCNT] =
{
	1.0,	// 0x00
	2.0,	// 0x01
	4.0,	// 0x02
	8.0		// 0x03
};

// input range (Volt)
const double BRD_Range[BRD_GAINCNT] =
{
	2.5,	// 0x00
	1.25,	// 0x01
	0.625,	// 0x02
	0.3125	// 0x03
};

#pragma pack(pop)    

#endif // _CTRL_ADC214x10M_H

//
// End of file
//