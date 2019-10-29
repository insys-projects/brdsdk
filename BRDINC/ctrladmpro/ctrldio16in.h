/*
 ****************** File CtrlDio16In.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : DIO16IN section
 *
 * (C) InSys by Dorokhin Andrey Feb, 2006
 *
 * 04.09.2006 - change BRDctrl_DIO16IN_SETCLKMODE/BRDctrl_DIO16IN_GETCLKMODE
 * 
 ************************************************************
*/

#ifndef _CTRL_DIO16IN_H
 #define _CTRL_DIO16IN_H

#include "ctrladmpro.h"
#include "ctrldio.h"

#pragma pack(push, 1)    

enum {
	BRDctrl_DIO16IN_SETPHASE		= BRDctrl_DIOIN + 0,	// Set DIO16IN phase clock (S32)
	BRDctrl_DIO16IN_GETPHASE		= BRDctrl_DIOIN + 1,	// Get DIO16IN phase clock (S32)
	BRDctrl_DIO16IN_SETSINGLEMODE	= BRDctrl_DIOIN + 2,	// Set DIO16IN single transfer mode (U32)
	BRDctrl_DIO16IN_GETSINGLEMODE	= BRDctrl_DIOIN + 3,	// Get DIO16IN single transfer mode (U32)
	BRDctrl_DIO16IN_SINGLEWRITE		= BRDctrl_DIOIN + 4,	// Set DIO16IN write in single transfer mode (U32)
	BRDctrl_DIO16IN_SINGLEREAD		= BRDctrl_DIOIN + 5,	// Set DIO16IN read in single transfer mode (U32)
	BRDctrl_DIO16IN_SETTESTMODE		= BRDctrl_DIOIN + 6,	// Set DIO16IN test mode (U32)
	BRDctrl_DIO16IN_GETTESTMODE		= BRDctrl_DIOIN + 7,	// Get DIO16IN test mode (U32)
	BRDctrl_DIO16IN_SETADDRDATA		= BRDctrl_DIOIN + 8,	// Set DIO16IN address or data select in single transfer mode (U32)
	BRDctrl_DIO16IN_GETADDRDATA		= BRDctrl_DIOIN + 9,	// Get DIO16IN address or data select in single transfer mode (U32)
	// ver. 2.00- !!!
//	BRDctrl_DIO16IN_SETCLKMODE		= BRDctrl_DIOIN + 12,	// Set DIO16IN clock inverting (U32)
//	BRDctrl_DIO16IN_GETCLKMODE		= BRDctrl_DIOIN + 13,	// Get DIO16IN clock inverting (U32)
	// ver. 2.10 only !!!
	BRDctrl_DIO16IN_SETCLKMODE		= BRDctrl_DIOIN + 12,	// Set DIO16IN clock mode (BRD_DioClkMode)
	BRDctrl_DIO16IN_GETCLKMODE		= BRDctrl_DIOIN + 13,	// Get DIO16IN clock mode (BRD_DioClkMode)

	BRDctrl_DIO16IN_FIFORESET		= BRDctrl_DIOIN + 128,	// DIO16IN FIFO reset (NULL)
	BRDctrl_DIO16IN_ENABLE			= BRDctrl_DIOIN + 129,	// DIO16IN enable / disable (NULL)
	BRDctrl_DIO16IN_FIFOSTATUS		= BRDctrl_DIOIN + 130,	// Get DIO16IN FIFO status (U32)
	BRDctrl_DIO16IN_GETDATA			= BRDctrl_DIOIN + 131,	// Get data buffer from DIO16IN FIFO (PVOID)
	BRDctrl_DIO16IN_GETSRCSTREAM	= BRDctrl_DIOIN + 138,	// Get source address for stream (U32)

	BRDctrl_DIO16IN_FPGALOAD		= BRDctrl_DIOIN + 160,	// Load of ADMFPGA (PVOID)
	BRDctrl_DIO16IN_FPGACFG			= BRDctrl_DIOIN + 161,	// Get configuration of ADMFPGA (U32)

	BRDctrl_DIO16IN_ILLEGAL
};


typedef struct _BRD_AdmFpgaCfg {
	U32	sig;				// OUT - signature (0x4953)
	U32	id;					// OUT - identification
	U32	mod;				// OUT - modification
	U32	ver;				// OUT - version
} BRD_AdmFpgaCfg, *PBRD_AdmFpgaCfg;

// error codes
enum {
	BRDerr_DIO16IN_PLDFORMAT	=	BRDctrl_DIOIN + 0,
	BRDerr_DIO16IN_PLDNOTRDY	=	BRDctrl_DIOIN + 1,
	BRDerr_DIO16IN_UNDEFINED
};

#pragma pack(pop)    

#endif // _CTRL_DIO16IN_H

//
// End of file
//