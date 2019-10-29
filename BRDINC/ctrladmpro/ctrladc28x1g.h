/*
 ****************** File CtrlAdc28x1g.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : ADC28x1G section
 *
 * (C) InSys by Dorokhin Andrey Jan, 2007
 *
 *
 ************************************************************
*/

#ifndef _CTRL_ADC28x1G_H
 #define _CTRL_ADC28x1G_H

#include "ctrladc.h"

#pragma pack(push, 1)    

#define BRD_CHANCNT			2  // Number of channels

const int BRD_CLKDIVCNT		= 6; // Number of clock dividers = 5 (1, 2, 4, 8, 16, 32)
//const int BRD_CLKAUXDIVCNT	= 2; // Number of auxiliary clock dividers = 2 (1, 2)

// ADC28x1G Clock sources
enum {
	BRDclks_ADC_DISABLED	= 0,	// 
	BRDclks_ADC_REFPLL		= 2,	// SubModule Clock from PLL
	BRDclks_ADC_EXTCLK		= 0x81,	// External SubModule Clock
};

// ADC28x1G start sources
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

// Numbers of Specific Command
typedef enum _ADC28X1G_NUM_CMD
{
	ADC28X1Gcmd_SETINTREG	= 64, // Set ADC internal control registers (PBRD_AdcIntReg)
	ADC28X1Gcmd_GETINTREG	= 65, // Get ADC internal control registers (PBRD_AdcIntReg)
} ADC28X1G_NUM_CMD;

// for set and get ADC internal control registers function
typedef struct _BRD_AdcIntReg
{
	U32		addr;			// register address into ADC chip for programming
	U32		data;			// data for writing / getting
} BRD_AdcIntReg, *PBRD_AdcIntReg;

#pragma pack(pop)    

#endif // _CTRL_ADC28x1G_H

//
// End of file
//
