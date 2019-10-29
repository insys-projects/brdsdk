/*
 ****************** File CtrlDsp.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : CMN section
 *
 * (C) InSys by Dorokhin S. May 2010
 *
 ************************************************************
*/

#ifndef _CTRL_DSP_H
 #define _CTRL_DSP_H

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

	BRDctrl_DSP_PROCRESET		= BRDctrl_CMN + 20,		// reset DSP processor
	BRDctrl_DSP_PROCSTART		= BRDctrl_CMN + 21,		// start DSP processor
	BRDctrl_DSP_PROCLOAD		= BRDctrl_CMN + 22,		// load application to DSP processor

	BRDctrl_DSP_GETMSG			= BRDctrl_CMN + 25,		// get message from DSP processor
	BRDctrl_DSP_PUTMSG			= BRDctrl_CMN + 26,		// put message to DSP processor

	BRDctrl_DSP_PEEKDPRAM		= BRDctrl_CMN + 30,		// read word from DPRAM
	BRDctrl_DSP_POKEDPRAM		= BRDctrl_CMN + 31,		// write word to DPRAM
	BRDctrl_DSP_READDPRAM		= BRDctrl_CMN + 32,		// read array from DPRAM
	BRDctrl_DSP_WRITEDPRAM		= BRDctrl_CMN + 33,		// write array to DPRAM

	BRDctrl_DSP_EXTENSION		= BRDctrl_CMN + 40,		// call extension

	BRDctrl_DSP_SIGNALWAIT		= BRDctrl_CMN + 50,		// wait for interrupt from DSP
	BRDctrl_DSP_SIGNALSEND		= BRDctrl_CMN + 51,		// generate interrupt to DSP

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

typedef struct _BRD_Irq {
	U32 tetr;
	U32 irqMask; 
	U32 irqInv; 
	HANDLE hEvent;
} BRD_Irq, *PBRD_Irq;

typedef struct PROC_LOAD_PARAM {
	U32		node;
	char	*filename;
	int		argc;
	char	**argv;
} PROC_LOAD_PARAM;

typedef struct MSG_PARAM {
	U32		node;
	void	*msg;
	U32		size;
	U32		timeout;
} MSG_PARAM;

typedef struct PEEK_DPRAM_PARAM {
	U32		node;
	U32		offset;
	U32		*data;
} PEEK_DPRAM_PARAM;

typedef struct POKE_DPRAM_PARAM {
	U32		node;
	U32		offset;
	U32		data;
} POKE_DPRAM_PARAM;

typedef struct RW_DPRAM_PARAM {
	U32		node;
	U32		offset;
	U32		*data;
	U32		size;
} RW_DPRAM_PARAM;

#pragma pack(pop)    

#endif // _CTRL_DSP_H

//
// End of file
//