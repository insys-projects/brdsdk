/*
 ****************** File CtrlDac1624x192.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : DAC1624x192 section
 *
 * (C) InSys by Dorokhin Andrey Jun, 2007
 *
 *
 ************************************************************
*/

#ifndef _CTRL_DAC1624X192_H
 #define _CTRL_DAC1624X192_H

#include "ctrldac.h"

#pragma pack(push, 1)    

//#define BRD_CHANCNT			4

//#define MAX_RATE 50000.

// Numbers of Specific Command
typedef enum _DAC1624X192_NUM_CMD
{
	DAC1624X192cmd_SETMUTE = 64,
	DAC1624X192cmd_GETMUTE = 65,
} DAC1624X192_NUM_CMD;

#pragma pack(pop)    

#endif // _CTRL_DAC1624X192_H

//
// End of file
//