/*
 ****************** File CtrlDacfm1620x1m.h *************************
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

#ifndef _CTRL_DACFM1620X1M_H
 #define _CTRL_DACFM1620X1M_H

#include "ctrldac.h"

#pragma pack(push, 1)    

//#define BRD_CHANCNT			4

//#define MAX_RATE 50000.

// Numbers of Specific Command
typedef enum _DACFM1620X1M_NUM_CMD
{
	DACFM1620X1Mcmd_SETMUTE = 64,
	DACFM1620X1Mcmd_GETMUTE = 65,
} DACFM1620X1M_NUM_CMD;

#pragma pack(pop)    

#endif // _CTRL_DACFM1620X1M_H

//
// End of file
//