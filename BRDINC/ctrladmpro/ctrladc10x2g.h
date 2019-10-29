/*
 ****************** File CtrlAdc10x2g.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : ADC10x2G section
 *
 * (C) InSys by Dorokhin Andrey Jun, 2005
 *
 *	14.03.2006 - add changies by ver. 2.0
 *	15.10.2008 - add changies by ver. 2.2
 *
 ************************************************************
*/

#ifndef _CTRL_ADC10x2G_H
 #define _CTRL_ADC10x2G_H

#include "ctrladc.h"

#pragma pack(push, 1)    

//#define BRD_CHANCNT			1  // Number of channels

const int BRD_CLKDIVCNT	= 5; // Number of clock dividers = 5 (1, 2, 4, 8, 16)

// ADC10x2G Clock sources
enum {
	BRDclks_ADC_DISABLED	= 0,	// 
	BRDclks_ADC_INTREFPLL	= 2,	// SubModule Clock from PLL (internal reference) - ver. 2.0 only !!!
	BRDclks_ADC_EXTREFPLL	= 3,	// SubModule Clock from PLL (external reference - EXT REF IN) - ver. 2.0 only !!!
	BRDclks_ADC_EXTCLK		= 0x81,	// External SubModule Clock (from EXT CLK IN)
	BRDclks_ADC_MASTERCLK	= 0x82,	// External Clock from MASTER card (synthesizer - from CLK IN)
};

// ADC10x2G start sources
enum {
	BRDsts_ADC_CHAN		 = 0,	// Start from channel
	BRDsts_ADC_EXTMASTER = 1,	// Start from START IN (external)
	BRDsts_ADC_EXT		 = 2,	// Start from EXT START IN (external)
	BRDsts_ADC_PRG		 = 3,	// Program start
};

const int BRD_GAINCNT	= 4; // Number of gains

// input gain
const double BRD_Gain[BRD_GAINCNT] =
{
	0.02,	// 0x00
	0.10,	// 0x01
	0.20,	// 0x02
	1.00,	// 0x03
};

// input range (Volt)
const double BRD_Range[BRD_GAINCNT] =
{
	5.0,	// 0x00
	1.0,	// 0x01
	0.5,	// 0x02
	0.1,	// 0x03
};

// input gain by ver. 2.0
const double BRD_Gain_v2[BRD_GAINCNT] =
{
	0.10,	// 0x00
	0.20,	// 0x01
	0.50,	// 0x02
	1.00,	// 0x03
};

// input range (Volt) by ver. 2.0
const double BRD_Range_v2[BRD_GAINCNT] =
{
	2.50,	// 0x00
	1.25,	// 0x01
	0.50,	// 0x02
	0.25,	// 0x03
};

// ADC10x2G test modes
enum {
	BRDtst_ADC_NOT	= 0,	// 
	BRDtst_ADC_PGE	= 1,	// Pattern Generator Enable
	BRDtst_ADC_BIST	= 2,	// Build-In Self-Test
	BRDtst_ADC_CAL	= 4,	// Calibration (zero level on input) - ver. 2.0 only !!!
};

// Numbers of Specific Command
typedef enum _ADC10X2G_NUM_CMD
{
	ADC10X2Gcmd_SETPLLMODE		= 0, // Set PLL mode (BRD_Adc10x2gPll) - ver. 2.0 only !!!
	ADC10X2Gcmd_GETEVENTCNT		= 1, // Get event counter - ver. 2.0 only !!!
	ADC10X2Gcmd_RESETEVENTCNT	= 2, // reset event counter - ver. 2.0 only !!!
	ADC10X2Gcmd_SETTITLEMODE	= 3, // Set Title mode - ver. 2.0 only !!!
	ADC10X2Gcmd_GETTITLEMODE	= 4, // Get Title mode - ver. 2.0 only !!!
	ADC10X2Gcmd_STABILITY		= 5, // - ver. 2.0 only !!!
	ADC10X2Gcmd_SETSTARTIN		= 6, // Set START IN - ver. 2.2 only !!!
	ADC10X2Gcmd_SETCLKIN		= 7, // Set CLK IN - ver. 2.2 only !!!
	ADC10X2Gcmd_SETTIMERSYNC	= 8, // Set sync timer mode - ver. 2.2 only !!!
	ADC10X2Gcmd_GETSTARTIN		= 9, // Get START IN - ver. 2.2 only !!!
	ADC10X2Gcmd_SETACCMODE		= 10, // Set accumulation mode (BRD_Adc10x2gAcc) - spec PLD file only !!!
} ADC10X2G_NUM_CMD;

typedef struct _BRD_Adc10x2gAcc {
	U32	AccOn;		// 1 - accumulation mode ON
	U32	AccCnt;		// accumulation counter
} BRD_Adc10x2gAcc, *PBRD_Adc10x2gAcc;

typedef struct _BRD_Adc10x2gPll {
	REAL64	ExtRef;		// IN - external reference frequency of PLL - ver. 2.0 only !!!
} BRD_Adc10x2gPll, *PBRD_Adc10x2gPll;

typedef struct _BRD_Adc10x2gTitle {
	U32		Signature;				// 0xAA5500FF
	U32		BlkNum;					// номер блока в сеансе сбора
	U32		BlkSize;				// размер блока (сейчас нет - резерв для будущего)
	U16		PrtStatus;				// уточнение события старта для этого блока
	U32		PrtCnt;					// событие старта для этого блока (номер 64-разрядного слова)
	U16		Reserve0;				// резерв (должен быть 0)
	U32		EventCntLo;				// сквозной счетчик событий (младшая часть) или таймер (счетчик тактов АЦП/8) (младшая часть)
	U32		EventCntHi;				// сквозной счетчик событий (старшая часть) или таймер (счетчик тактов АЦП/8) (старшая часть)
	U32		Reserve2;				// резерв (должен быть 0)
} BRD_Adc10x2gTitle, *PBRD_Adc10x2gTitle;

#pragma pack(pop)    

#endif // _CTRL_ADC10x2G_H

//
// End of file
//