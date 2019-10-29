/*
 ****************** File CtrlCCP.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : CCPINFO section
 *
 * (C) Instrumental Systems 
 *
 ************************************************************
*/

#ifndef _CTRL_PCK_H
 #define _CTRL_PCK_H

#include "ctrl.h"



//=********************************************************
//
// Command Codes
//
//=********************************************************

#define BRDctrl_PCK 40

enum {
	BRDctrl_PCK_ENABLE		= BRDctrl_PCK + 0,	// 
	BRDctrl_PCK_DISABLE		= BRDctrl_PCK + 1,	// 

	BRDctrl_PCK_READ		= BRDctrl_PCK + 2,	// Receive
	BRDctrl_PCK_WRITE		= BRDctrl_PCK + 3,	// Send
	BRDctrl_PCK_CHECK		= BRDctrl_PCK + 4,	// Select

	BRDctrl_PCK_ILLEGAL
};

//=********************************************************
//
// Structure Declarations
//
//=********************************************************

#pragma pack(push, 1) 

typedef struct{
	U32 nSize;
	U32 *pBuf;
	S32	timeout;
} BRDctrl_PacketIO;

#pragma pack(pop)    


//=********************************************************
//
// Constants
//
//=********************************************************



#endif // _CTRL_PCK_H

//
// End of file
//