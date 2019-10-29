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

#ifndef _CTRL_DR16_H
 #define _CTRL_DR16_H

#include "ctrladmpro.h"

enum {
	BRDctrl_DR16_SETCLKMODE		= BRDctrl_DDS + 2,	// Set  clock mode (BRD_ClkModeDr16)
	BRDctrl_DR16_SETSTARTMODE	= BRDctrl_DDS + 12,	// Set Mode of Start(BRD_StartModeDr16)
	BRDctrl_DR16_SETTPMODE		= BRDctrl_DDS + 13,	// Set Mode of Tp(BRD_TpModeDr16)
	BRDctrl_DR16_GETSTATUS		= BRDctrl_DDS + 14,	// Get Status  (U32)
	BRDctrl_DR16_RESTARTDIV		= BRDctrl_DDS + 15,	// Restart divider (NULL)
	BRDctrl_DR16_RESETJTAG		= BRDctrl_DDS + 16,	// Restart divider (NULL)
	BRDctrl_DR16_UNRESETJTAG	= BRDctrl_DDS + 17,	// Restart divider (NULL)
	BRDctrl_DR16_PUTDATA		= BRDctrl_DDS + 18,	// Put data buffer into DR16 FIFO (PVOID)
	BRDctrl_DR16_FIFORESET		= BRDctrl_DDS + 19,	// DR16 FIFO reset (NULL)
	BRDctrl_DR16_ILLEGAL
};

#pragma pack(push, 1)    

// for  SetClkModeDr16
typedef struct _BRD_ClkModeDr16
{	
	U32 ClkSource;	// Source of Clock: 0-5
	U32 DivVco;		// Divider VCO: 0-3
	U32  DivClkAdc;	// Divider Clock ADC: 0-4
} BRD_ClkModeDr16,*PBRD_ClkModeDr16;

// for  SetStartModeDr16
typedef struct _BRD_StartModeDr16
{	
	U32 Sel_ST0;	// 
	U32 Sel_ST1;		// 
	U32 Sel_ST2;	// 

} BRD_StartModeDr16,*PBRD_StartModeDr16;

// for  SetTpModeDr16
typedef struct _BRD_TpModeDr16
{	
	U32 PeriodTp;	// Period of internal  Tp (ms) 
	U32 EnableTp;	// 1 - Enable of internal Tp 
	U32 InvExtTp;	// 1 - Invert external Tp 
} BRD_TpModeDr16,*PBRD_TpModeDr16;

// DDS error codes
enum {
	BRDerr_DDS_INVALID_WRRDREG	=	BRDctrl_DDS + 0, // неправильная запись или чтение регистра DDS
	BRDerr_DDS_UNDEFINED
};

#pragma pack(pop)    

#endif // _CTRL_DR16_H

//
// End of file
//