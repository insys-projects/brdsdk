/*
 ****************** File CtrlAdcfm1620x1m.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : FM1620x1M section
 *
 * (C) InSys by Ekkore July, 2015
 *
 *
 ************************************************************
*/

#ifndef _CTRL_ADCFM1620X1M_H
 #define _CTRL_ADCFM1620X1M_H

#include "ctrladc.h"

#pragma pack(push, 1)    

#define BRD_CHANCNT			16

//const int BRD_CLKDIVCNT	= 7; // Number of clock dividers = 4 (1, 2, 4, 8, 16, 32, 64)

//
// FM1620X1M Clock sources
enum {
	BRDclks_ADC_DISABLED	= 0,	// 
	BRDclks_ADC_SUBGEN		= 1,	// SubModule Generator 0
	BRDclks_ADC_EXTCLK		= 0x81,	// External SubModule Clock
};

// FM1620X1M start sources
enum {
	BRDsts_ADC_CHAN0	= 0,	// Start from channel 0
	BRDsts_ADC_EXT		= 2,	// Start from START IN (external)
	BRDsts_ADC_PRG		= 3,	// Program start
};

// FM1620X1M источник сигнала для АЦП
enum {
	BRDsrc_ADCIN_GND = 0,	// Аналоговая земля
	BRDsrc_ADCIN_DAC = 1,	// Выход ЦАП0
	BRDsrc_ADCIN_REF = 2,	// Опорное напряжение
	BRDsrc_ADCIN_AIN = 3,	// Сигнал с разъема внешнего сигнала
};

// Numbers of Specific Command
typedef enum _ADCFM1620X1M_NUM_CMD
{
	ADCFM1620X1Mcmd_SETMU		= 64,
	ADCFM1620X1Mcmd_GETMU		= 65,
} ADCFM1624X1M_NUM_CMD;

#pragma pack(pop)    

#endif // _CTRL_ADCFM1620X1M_H

//
// End of file
//