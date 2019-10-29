/*
 ****************** File CtrlAdc16214x128.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : ADC16214x128 section
 *
 * (C) InSys by Dorokhin Andrey, Ekkore, 2007-2012
 *
 *
 ************************************************************
*/

#ifndef _CTRL_ADC16214X128_H
 #define _CTRL_ADC16214X128_H

#include "ctrladc.h"

#pragma pack(push, 1)    

#define BRD_CHANCNT			16

//const int BRD_CLKDIVCNT	= 7; // Number of clock dividers = 4 (1, 2, 4, 8, 16, 32, 64)

const int BRD_GAINCNT	= 5; // Number of gains

// input gain
const double BRD_Gain[BRD_GAINCNT] =
{
	0.5,	// 0x00
	1.0,	// 0x01
	2.0,	// 0x02
	4.0,	// 0x03
	8.0		// 0x04
};

// input range (Volt)
const double BRD_Range[BRD_GAINCNT] =
{
	20.0,	// 0x00
	10.0,	// 0x01
	5.00,	// 0x02
	2.50,	// 0x03
	1.25,	// 0x04
};

// Numbers of Specific Command
typedef enum _ADC16214X128_NUM_CMD
{
//	ADC16214X128cmd_SETHPFOFF		= 64,
//	ADC16214X128cmd_GETHPFOFF		= 65,
//	ADC16214X128cmd_SETSPEEDMODE	= 66,
//	ADC16214X128cmd_GETSPEEDMODE	= 67,
} ADC16214X128_NUM_CMD;

#pragma pack(pop)    

#endif // _CTRL_ADC16214X128_H

//
// End of file
//