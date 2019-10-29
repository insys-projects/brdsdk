/*
 ****************** File CtrlAdc4x16.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : DDC4x16 section
 *
 * (C) InSys by Dorokhin Andrey Jan, 2006
 *
 *
 ************************************************************
*/

#ifndef _CTRL_ADC4X16_H
 #define _CTRL_ADC4X16_H

#include "ctrladc.h"

#pragma pack(push, 1)    

//#define BRD_CHANCNT			4

// Clock sources
enum {
	BRDclks_ADC_REFGEN	= 0,	// SubModule Reference Generator
	BRDclks_ADC_EXTCLK	= 0x81,	// External SubModule Clock
};

#pragma pack(pop)    

#endif // _CTRL_ADC4X16_H

//
// End of file
//