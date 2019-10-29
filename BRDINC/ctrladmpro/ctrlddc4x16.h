/*
 ****************** File CtrlDdc4x16.h *************************
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

#ifndef _CTRL_DDC4X16_H
 #define _CTRL_DDC4X16_H

#include "ctrlddc.h"

#pragma pack(push, 1)    

#define BRD_DDC_CHANCNT	16

// Clock sources
enum {
	BRDclks_DDC_REFGEN	= 0,	// SubModule Reference Generator
	BRDclks_DDC_EXTCLK	= 0x81,	// External SubModule Clock
};

// Numbers of Specific Command
typedef enum _DDC4X16_NUM_CMD
{
	DDC4X16cmd_WRITE		= 0,
	DDC4X16cmd_READ			= 1,
	DDC4X16cmd_SETADCMODE	= 2, // Set ADC mode - submodule ver 3.0 only !!!
	DDC4X16cmd_GETADCMODE	= 3, // Get ADC mode - submodule ver 3.0 only !!!
	DDC4X16cmd_SETALIGN		= 4, // Set align for DDC data - submodule ver 3.0 only !!!
	DDC4X16cmd_GETALIGN		= 5, // Get align for DDC data - submodule ver 3.0 only !!!
	DDC4X16cmd_SETFCCODE	= 6,
	DDC4X16cmd_GETFCCODE	= 7,
} DDC4X16_NUM_CMD;

typedef struct _BRD_DdcReg
{
	U32		ddcNum;			// DDC number
	U32		page;			// page number of DDC
	U32		reg;			// register address of DDC
	U32		val;			// data for writing/reading
} BRD_DdcReg, *PBRD_DdcReg;

typedef struct _BRD_Adc4x16Mode
{
	U32		gainMask;		// gain mask of ADC
	U32		rand;			// Digital Output Randomization mode of ADC
	U32		dither;			// inside DITHER mode of ADC
	U32		bits;			// bits of data from ADC to DDC (0 - 14 bit, 1 - 16 bit)
} BRD_Adc4x16Mode, *PBRD_Adc4x16Mode;

#pragma pack(pop)    

#endif // _CTRL_DDC4X16_H

//
// End of file
//