/*
 ****************** File CtrlSync8ch.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : Sync8ch section
 *
 * (C) InSys by Dorokhin A. Apr, 2009
 *
 ************************************************************
*/

#ifndef _CTRL_SYNC8CH_H
 #define _CTRL_SYNC8CH_H

#include "ctrladmpro.h"

enum {
	BRDctrl_SYNC8CH_SETMASTER		= BRDctrl_DDS + 0,	// Set master/single/slave mode (U32)
	BRDctrl_SYNC8CH_GETMASTER		= BRDctrl_DDS + 1,	// Get master/single/slave mode (U32)
	BRDctrl_SYNC8CH_SETCLKMODE		= BRDctrl_DDS + 2,	// Set clock mode (BRD_SyncClkMode)
	BRDctrl_SYNC8CH_GETCLKMODE		= BRDctrl_DDS + 3,	// Get clock mode (BRD_SyncClkMode)
	BRDctrl_SYNC8CH_SETOUTCLK		= BRDctrl_DDS + 4,	// Set Output clock (BRD_SyncOutClk)
	BRDctrl_SYNC8CH_GETOUTCLK		= BRDctrl_DDS + 5,	// Get Output clock (BRD_SyncOutClk)
	BRDctrl_SYNC8CH_SETSTBMODE		= BRDctrl_DDS + 6,	// Set Strobe Mode (BRD_SyncStbMode)
	BRDctrl_SYNC8CH_GETSTBMODE		= BRDctrl_DDS + 7,	// Get Strobe Mode (BRD_SyncStbMode)
	BRDctrl_SYNC8CH_SETOUTSTB		= BRDctrl_DDS + 8,	// Set Output clock (BRD_SyncOutStb)
	BRDctrl_SYNC8CH_GETOUTSTB		= BRDctrl_DDS + 9,	// Get Output clock (BRD_SyncOutStb)
	BRDctrl_SYNC8CH_SETSTARTMODE	= BRDctrl_DDS + 10,	// Set Start Mode (BRD_SyncStartMode)
	BRDctrl_SYNC8CH_GETSTARTMODE	= BRDctrl_DDS + 11,	// Get Start Mode (BRD_SyncStartMode)
	BRDctrl_SYNC8CH_STARTSTROBE		= BRDctrl_DDS + 12,	// Start Strobe (NULL)
	BRDctrl_SYNC8CH_STOPSTROBE		= BRDctrl_DDS + 13,	// Stop Strobe (NULL)
	BRDctrl_SYNC8CH_STARTCLOCK		= BRDctrl_DDS + 14,	// Start Clock (NULL)
	BRDctrl_SYNC8CH_STOPCLOCK		= BRDctrl_DDS + 15,	// Stop Clock (NULL)
	BRDctrl_SYNC8CH_SETPLLMODE		= BRDctrl_DDS + 16,	// Set PLL mode (BRD_PllMode)
	BRDctrl_SYNC8CH_ILLEGAL
};

#pragma pack(push, 1)    

// for SetClkMode/GetClkMode
typedef struct _BRD_SyncClkMode {
	U32		src;		// IN - clock source
	REAL64	value;		// IN (external clock) / OUT (real clock) 
	U32		preDiv;		// preliminary divider of clock
	U32		auxDiv;		// auxiliary divider of clock
	U32		clkInv;		// clock inverting
	U32		clkSlave;	// clock from SLCLKI
	U32		clkSel;		// clock select
	REAL64	pllFreq;	// PLL frequency (for SYNC8P only)
} BRD_SyncClkMode, *PBRD_SyncClkMode;

// Clock sources
enum {
	BRDclks_SYNC8CH_DISABLED	= 0,	// disable clock
	BRDclks_SYNC8CH_GEN1		= 1,	// Module Generator 1
	BRDclks_SYNC8CH_GEN2		= 2,	// Module Generator 2
	BRDclks_SYNC8CH_OCXO		= 3,	// Module OCXO Generator
	BRDclks_SYNC8CH_GEN1REFPLL	= 5,	// Module Clock from PLL (reference generator 1)
	BRDclks_SYNC8CH_GEN2REFPLL	= 6,	// Module Clock from PLL (reference generator 2)
	BRDclks_SYNC8CH_OCXOREFPLL	= 7,	// Module Clock from PLL (reference OCXO generator)
	BRDclks_SYNC8CH_EXTCLK		= 0x81,	// External Module Clock
	BRDclks_SYNC8CH_EXTREFPLL	= 0x85	// Module Clock from PLL (reference external clock)
};

// for  SetOutClk/GetOutClk
typedef struct _BRD_SyncOutClk
{	
	U32 chan;		// IN - number of clock (0 - E, 1 - F, 2 - G)
	U32 enable;		// IN/OUT - enable/disable of clock
	U32	divider;	// IN/OUT - divider of clock
} BRD_SyncOutClk,*PBRD_SyncOutClk;

// for  SetStbMode/GetStbMode
typedef struct _BRD_SyncStbMode
{	
	U32 clkDelay;		// задержка стробов относительно CLKO
	U32 signDelay;		// знак задержки
	U32 stbClkInv;		// 0/1 - стробы пересинхронизируются положительным/отрицательным фронтом CLKO
} BRD_SyncStbMode,*PBRD_SyncStbMode;

// for  SetOutStb/GetOutStb
typedef struct _BRD_SyncOutStb
{	
	U32		chan;		// number of strobe (0 - A, 1 - B, 2 - C, 3 - D)
	U32		enable;		// enable/disable of strobe
	U32		polarity;	// polarity of strobe
	U32		byPass;		// 1 - by pass PLD of strobe
	U32		delay;		// delay of strobe
	U32		width;		// width of strobe
	U32		period;		// period of strobe
	REAL64	hiLevel;	// high level of strobe
	REAL64	loLevel;	// low level of strobe
} BRD_SyncOutStb,*PBRD_SyncOutStb;

// for SetStartMode/GetStartMode
typedef struct _BRD_SyncStartMode 
{
	U32			startSrc;			// Start source
	U32			startInv;			// Start inversion
	U32			trigOn;				// Trigger start mode on
	U32			trigStopSrc;		// Stop source for trigger start
	U32			stopInv;			// Stop inversion
	U32			reStartMode;		// Restart mode
	U32			startSlave;			// Start from SLCLKI
	REAL64		levelExt;			// Level of external start
} BRD_SyncStartMode, *PBRD_SyncStartMode;

typedef struct _BRD_PllMode {
	U32		MuxOut;				// 
} BRD_PllMode, *PBRD_PllMode;

#pragma pack(pop)    

#endif // _CTRL_SYNC8CH_H

//
// End of file
//