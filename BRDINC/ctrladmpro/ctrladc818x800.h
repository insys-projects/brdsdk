/*
 ****************** File CtrlAdc818X800.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : ADC818X800 section
 *
 * (C) InSys by Dorokhin Andrey Jun, 2007
 *
 *
 ************************************************************
*/

#ifndef _CTRL_ADC818X800_H
 #define _CTRL_ADC818X800_H

#include "ctrladc.h"

#pragma pack(push, 1)    

#define BRD_CHANCNT			8

const int BRD_GAINCNT	= 4; // Number of gains

// input gain
const double BRD_Gain[BRD_GAINCNT] =
{
	1.0,	// 0x00
	2.0,	// 0x01
	4.0,	// 0x02
	8.0	// 0x03
};

// input range (Volt)
const double BRD_Range[BRD_GAINCNT] =
{
	2.50,	// 0x00
	1.25,	// 0x01
	0.625,	// 0x02
	0.3125,	// 0x03
};

/*
// Numbers of Specific Command
typedef enum _ADC818X800_NUM_CMD
{
	ADC818X800cmd_SETWARP		= 64,
	ADC818X800cmd_GETWARP		= 65,
} ADC818X800_NUM_CMD;
*/

#pragma pack(pop)    

#endif // _CTRL_ADC818X800_H

//
// End of file
//