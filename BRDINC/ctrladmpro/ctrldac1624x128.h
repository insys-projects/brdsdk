/*
 ****************** File CtrlDac1624x128.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : DAC1624x128 section
 *
 * (C) InSys by Dorokhin Andrey, Ekkore, 2007-2012
 *
 *
 ************************************************************
*/

#ifndef _CTRL_DAC1624X128_H
 #define _CTRL_DAC1624X128_H

#include "ctrldac.h"

#pragma pack(push, 1)    

//#define BRD_CHANCNT			4

//#define MAX_RATE 50000.

// Numbers of Specific Command
typedef enum _DAC1624X128_NUM_CMD
{
	DAC1624X128cmd_SETMUTE = 64,
	DAC1624X128cmd_GETMUTE = 65,
} DAC1624X128_NUM_CMD;

#pragma pack(pop)    

#endif // _CTRL_DAC1624X128_H

//
// End of file
//