/*
 ****************** File CtrlBaseDacn.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : BASEDACN section
 *
 * (C) InSys by Sklyarov A., Dorokhin A. Mar 2011
 *
 *
 ************************************************************
*/

#ifndef _CTRL_BASEDACN_H
 #define _CTRL_BASEDACN_H

#include "ctrldac.h"

#pragma pack(push, 1)    

// Numbers of Specific Command
typedef enum _BASEDACN_NUM_CMD
{
	BASEDACNcmd_SETINTREG	= 64, // Set DAC internal control registers (PBRD_DacIntReg)
} BASEDACN_NUM_CMD;

// for set and get DAC internal control registers function
typedef struct _BRD_DacIntReg
{
	U32		addr;			// register address into DAC chip for programming
	U32		data;			// data for writing / getting
} BRD_DacIntReg, *PBRD_DacIntReg;

#pragma pack(pop)    

#endif // _CTRL_BASEDACN_H

//
// End of file
//