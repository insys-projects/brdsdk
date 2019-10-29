/*
 ****************** File CtrlDac3224x192.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : DAC3224x192 section
 *
 * (C) InSys by Dorokhin Andrey Oct, 2005
 *
 *
 ************************************************************
*/

#ifndef _CTRL_DAC3224X192_H
 #define _CTRL_DAC3224X192_H

#include "ctrldac.h"

#pragma pack(push, 1)    

#define BRD_CHANCNT			32

#define MAX_SINGLE_RATE 50000.
#define MAX_DOUBLE_RATE 100000.

// Numbers of Specific Command
typedef enum _DAC3224X192_NUM_CMD
{
	DAC3224X192cmd_WRITE = 0,
} DAC3224X192_NUM_CMD;

// for set and get Start mode function
typedef struct _BRD_DacRegWrite
{
	U32		progMask;		// mask for programming DAC chips
	U32		addr;			// register address into DAC chip for programming
	U32		data;			// data for writing
} BRD_DacRegWrite, *PBRD_DacRegWrite;

#pragma pack(pop)    

#endif // _CTRL_DAC3224X192_H

//
// End of file
//