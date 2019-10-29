/*
 ****************** File CtrlReg.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : CMN section
 *
 * (C) InSys by Dorokhin Andrey Nov 2007
 *
 ************************************************************
*/

#ifndef _CTRL_REG_H
 #define _CTRL_REG_H

#include "ctrladmpro.h"

enum {
	BRDctrl_REG_READDIR			= BRDctrl_CMN + 0,		// read value from direct register (PBRD_Reg)
	BRDctrl_REG_READSDIR		= BRDctrl_CMN + 1,		// read buffer from direct register (PBRD_Reg)
	BRDctrl_REG_READIND			= BRDctrl_CMN + 2,		// read value from indirect register (PBRD_Reg)
	BRDctrl_REG_READSIND		= BRDctrl_CMN + 3,		// read buffer from indirect register (PBRD_Reg)
	BRDctrl_REG_WRITEDIR		= BRDctrl_CMN + 6,		// write value in direct register (PBRD_Reg)
	BRDctrl_REG_WRITESDIR		= BRDctrl_CMN + 7,		// write buffer in direct register (PBRD_Reg)
	BRDctrl_REG_WRITEIND		= BRDctrl_CMN + 8,		// write value in indirect register (PBRD_Reg)
	BRDctrl_REG_WRITESIND		= BRDctrl_CMN + 9,		// write buffer in indirect register (PBRD_Reg)
	BRDctrl_REG_SETSTATIRQ		= BRDctrl_CMN + 10,		// set interrupt by status register (PBRD_Irq)
	BRDctrl_REG_CLEARSTATIRQ	= BRDctrl_CMN + 11,		// clear interrupt by status register (U32)

	BRDctrl_REG_PACKEXECUTE		= BRDctrl_CMN + 12,		// execute command (read/write registers) packet (BRD_ItemArray[BRD_PckReg])

	BRDctrl_REG_WAITSTATIRQ		= BRDctrl_CMN + 15,		// wait interrupt by status register (PBRD_Irq)

	BRDctrl_REG_READSPD			= BRDctrl_CMN + 20,		// read value from spd register (PBRD_Spd)
	BRDctrl_REG_WRITESPD		= BRDctrl_CMN + 21,		// read buffer from spd register (PBRD_Spd)

	BRDctrl_REG_READHOST		= BRDctrl_CMN + 24,		// read value from block (host pld) register (PBRD_Host)
	BRDctrl_REG_WRITEHOST		= BRDctrl_CMN + 25,		// write value to block (host pld) register (PBRD_Host)

	BRDctrl_REG_ILLEGAL
};

#pragma pack(push, 1)    

typedef struct _BRD_Reg {
	S32 tetr;
	S32 reg; 
	U32 val; 
	void *pBuf; 
	S32 bytes; 
} BRD_Reg, *PBRD_Reg;

typedef struct _BRD_Spd {
	S32 dev;
	S32 num;
	S32 sync;
	S32 mode;//32 bit?
	S32 tetr;
	S32 reg; 
	U32 val; 
} BRD_Spd, *PBRD_Spd;

typedef struct _BRD_Irq {
	U32 tetr;
	U32 irqMask; 
	U32 irqInv; 
	HANDLE hEvent;
} BRD_Irq, *PBRD_Irq;

typedef struct _BRD_PckReg {
	S32 cmd;
	S32 tetr;
	S32 reg; 
	U32 val; 
} BRD_PckReg, *PBRD_PckReg;

typedef struct _BRD_Host {
	S32 blk;
	S32 reg;
	U32 val;
} BRD_Host, *PBRD_Host;

#pragma pack(pop)    

#endif // _CTRL_REG_H

//
// End of file
//