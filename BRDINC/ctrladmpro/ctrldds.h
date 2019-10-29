/*
 ****************** File CtrlDds.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : DDS section
 *
 * (C) InSys by Sklyarov A. Mar, 2007
 *
 ************************************************************
*/

#ifndef _CTRL_DDS_H
 #define _CTRL_DDS_H

#include "ctrladmpro.h"

enum {
	BRDctrl_DDS_SETMASTER		= BRDctrl_DDS + 0,	// Set DDS master/single/slave mode (U32)
	BRDctrl_DDS_GETMASTER		= BRDctrl_DDS + 1,	// Get DDS master/single/slave mode (U32)
	BRDctrl_DDS_SETCLKMODE		= BRDctrl_DDS + 2,	// Set  clock mode (BRD_ClkMode)
	BRDctrl_DDS_GETCLKMODE		= BRDctrl_DDS + 3,	// Get  clock mode (BRD_ClkMode)
	BRDctrl_DDS_SETOUTFREQ		= BRDctrl_DDS + 4,	// Set Output Freq (BRD_OutFreq)
	BRDctrl_DDS_GETOUTFREQ		= BRDctrl_DDS + 5,	// Get Output Freq (BRD_OutFreq)
	BRDctrl_DDS_STARTSTROBE		= BRDctrl_DDS + 6,	// Start Strobe(NULL)
	BRDctrl_DDS_STOPSTROBE		= BRDctrl_DDS + 7,	// Stop Strobe(NULL)
	BRDctrl_DDS_SETSTBMODE		= BRDctrl_DDS + 8,	// Set Strobe Mode (BRD_StbMode)
	BRDctrl_DDS_GETSTBMODE		= BRDctrl_DDS + 9,	// Get Strobe Mode (BRD_StbMode)
	BRDctrl_DDS_ENABLE			= BRDctrl_DDS + 10,	// Enable DDS(NULL)
	BRDctrl_DDS_DISABLE			= BRDctrl_DDS + 11,	// Disable DDS(NULL)
	BRDctrl_DDS_SETSTARTMODE	= BRDctrl_DDS + 12,	// Set Mode of Start(BRD_StartMode)
	BRDctrl_DDS_GETSTARTMODE	= BRDctrl_DDS + 13,	// Get Mode of Start(BRD_StartMode)
	BRDctrl_DDS_GETSTATUS		= BRDctrl_DDS + 14,	// Get Status DDS (U32)
	BRDctrl_DDS_SETSTARTCLK		= BRDctrl_DDS + 16,	// Set DDS_V2 clock for start (U32)
	BRDctrl_DDS_GETSTARTCLK		= BRDctrl_DDS + 17,	// Get DDS_V2 clock for start (U32)
	BRDctrl_DDS_SETMUXOUT		= BRDctrl_DDS + 18,	// Set DDS_V2 multiplexer output (U32)
	BRDctrl_DDS_GETMUXOUT		= BRDctrl_DDS + 19,	// Get DDS_V2 multiplexer output (U32)
	BRDctrl_DDS_SETDDSPOWER		= BRDctrl_DDS + 20,	// Set DDS_V2 power (U32)
	BRDctrl_DDS_GETDDSPOWER		= BRDctrl_DDS + 21,	// Get DDS_V2 power (U32)
	BRDctrl_DDS_PREPARESTART	= BRDctrl_DDS + 143,	// Called before first BRDctrl_DDS_ENABLE (NULL)
	BRDctrl_DDS_ILLEGAL
};

#pragma pack(push, 1)    

// for  SetStrobeMode/GetStrobeMode
typedef struct _BRD_StbMode
{	
	U32 Length;		// Length of stb
	U32 Polarity;	// Polarity of stb
	U32 Delay;		// Delay of stb
	U32 Period;		// Period of stb
	U32 Number;		// Number of stb
	U32 ContStb;	// Continuos out of stb
} BRD_StbMode,*PBRD_StbMode;

// for  SetOutputFreq/GetOutputFreq
typedef struct _BRD_OutFreq
{	
	double  Freq;	        // Output Frequency
	U32     CPCurrentScale;	// Scale CP
	U32     DividerM;		//
} BRD_OutFreq,*PBRD_OutFreq;

// DDS error codes
enum {
	BRDerr_DDS_INVALID_WRRDREG	=	BRDctrl_DDS + 0, // неправильная запись или чтение регистра DDS
	BRDerr_DDS_UNDEFINED
};

#pragma pack(pop)    

#endif // _CTRL_DDS_H

//
// End of file
//