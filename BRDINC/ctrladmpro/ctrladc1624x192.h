/*
 ****************** File CtrlAdc16214x192.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : ADC16214x192 section
 *
 * (C) InSys by Dorokhin Andrey Jun, 2007
 *
 *
 ************************************************************
*/

#ifndef _CTRL_ADC16214X192_H
 #define _CTRL_ADC16214X192_H

#include "ctrladc.h"

#pragma pack(push, 1)    

#define BRD_CHANCNT			16

//const int BRD_CLKDIVCNT	= 7; // Number of clock dividers = 4 (1, 2, 4, 8, 16, 32, 64)

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
	4.0,	// 0x00
	2.0,	// 0x01
	1.0,	// 0x02
	0.5,	// 0x03
};

typedef enum _ADC16214X192_HPF
{
	HPF_ON	= 0,
	HPF_OFF	= 1,
} ADC16214X192_HPF;

// Numbers of Specific Command
typedef enum _ADC16214X192_NUM_CMD
{
	ADC16214X192cmd_SETHPFOFF		= 64,
	ADC16214X192cmd_GETHPFOFF		= 65,
	ADC16214X192cmd_SETSPEEDMODE	= 66,
	ADC16214X192cmd_GETSPEEDMODE	= 67,
} ADC16214X192_NUM_CMD;

#pragma pack(pop)    

#endif // _CTRL_ADC16214X192_H

//
// End of file
//