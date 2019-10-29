/*
 ****************** File CtrlTestEx.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : TestEx section
 *
 * (C) Instrumental Systems 
 *
 * Created: by Dorokhin A. Feb, 2008
 *
 *
 ************************************************************
*/

#ifndef _CTRL_TESTEX_H
 #define _CTRL_TESTEX_H

#include "ctrladmpro.h"

enum {
	BRDctrl_TESTEX_SETMUXCTRL	= BRDctrl_TEST + 0,		// Set output multiplexer (U32)
	BRDctrl_TESTEX_GETMUXCTRL	= BRDctrl_TEST + 1,		// Get output multiplexer (U32)
	BRDctrl_TESTEX_SETCHKMODE	= BRDctrl_TEST + 2,		// Set check unit mode (PBRD_TestExMode)
	BRDctrl_TESTEX_GETCHKMODE	= BRDctrl_TEST + 3,		// Get check unit mode (PBRD_TestExMode)
	BRDctrl_TESTEX_SETCHKSIZE	= BRDctrl_TEST + 4,		// Set checked block size (U32)
	BRDctrl_TESTEX_GETCHKSIZE	= BRDctrl_TEST + 5,		// Get checked block size (U32)
	BRDctrl_TESTEX_SETGENMODE	= BRDctrl_TEST + 6,		// Set generation unit mode (PBRD_TestExMode)
	BRDctrl_TESTEX_GETGENMODE	= BRDctrl_TEST + 7,		// Get generation unit mode (PBRD_TestExMode)
	BRDctrl_TESTEX_SETGENSIZE	= BRDctrl_TEST + 8,		// Set generated block size (U32)
	BRDctrl_TESTEX_GETGENSIZE	= BRDctrl_TEST + 9,		// Get generated block size (U32)
	BRDctrl_TESTEX_SETGENENCNT	= BRDctrl_TEST + 10,	// Set generation counter of enable (U32)
	BRDctrl_TESTEX_GETGENENCNT	= BRDctrl_TEST + 11,	// Get generation counter of enable (U32)
	BRDctrl_TESTEX_SETGENDSCNT	= BRDctrl_TEST + 12,	// Set generation counter of disable (U32)
	BRDctrl_TESTEX_GETGENDSCNT	= BRDctrl_TEST + 13,	// Get generation counter of disable (U32)

	BRDctrl_TESTEX_READDIR		= BRDctrl_TEST + 64,	// read value from direct register (PBRD_Reg)
	BRDctrl_TESTEX_READSDIR		= BRDctrl_TEST + 65,	// read buffer from direct register (PBRD_Reg)
	BRDctrl_TESTEX_READIND		= BRDctrl_TEST + 66,	// read value from indirect register (PBRD_Reg)
	BRDctrl_TESTEX_READSIND		= BRDctrl_TEST + 67,	// read buffer from indirect register (PBRD_Reg)
	BRDctrl_TESTEX_WRITEDIR		= BRDctrl_TEST + 68,	// write value in direct register (PBRD_Reg)
	BRDctrl_TESTEX_WRITESDIR	= BRDctrl_TEST + 69,	// write buffer in direct register (PBRD_Reg)
	BRDctrl_TESTEX_WRITEIND		= BRDctrl_TEST + 70,	// write value in indirect register (PBRD_Reg)
	BRDctrl_TESTEX_WRITESIND	= BRDctrl_TEST + 71,	// write buffer in indirect register (PBRD_Reg)

	BRDctrl_TESTEX_INFIFORESET	= BRDctrl_TEST + 128,	// Test DATA64IN FIFO reset (NULL)
	BRDctrl_TESTEX_INENABLE		= BRDctrl_TEST + 129,	// Enable/disable write to Test DATA64IN (U32)
	BRDctrl_TESTEX_INFIFOSTATUS	= BRDctrl_TEST + 130,	// Get Test DATA64IN FIFO status (U32)
	BRDctrl_TESTEX_GETDATA		= BRDctrl_TEST + 131,	// Get data buffer (BRD_DataBuf)

	BRDctrl_TESTEX_OUTFIFORESET		= BRDctrl_TEST + 132,	// Test DATA64OUT FIFO reset (NULL),	
	BRDctrl_TESTEX_OUTENABLE  		= BRDctrl_TEST + 133,	// Enable/disable write to Test DATA64OUT (U32) 	
	BRDctrl_TESTEX_OUTFIFOSTATUS	= BRDctrl_TEST + 134,	// Get Test DATA64OUT FIFO status (U32)
	BRDctrl_TESTEX_PUTDATA			= BRDctrl_TEST + 135,	// Set data buffer (BRD_DataBuf)		

	BRDctrl_TESTEX_GETINSRCSTREAM	= BRDctrl_TEST + 138,	// Get source address for input stream (U32)
	BRDctrl_TESTEX_GETOUTSRCSTREAM	= BRDctrl_TEST + 139,	// Get source address for output stream (U32)

	BRDctrl_TESTEX_CHKRESET		= BRDctrl_TEST + 140,	// Test check unit reset (NULL)
	BRDctrl_TESTEX_CHKSTART		= BRDctrl_TEST + 141,	// Start check unit (NULL)
	BRDctrl_TESTEX_CHKSTOP		= BRDctrl_TEST + 142,	// Stop check unit (NULL)
	BRDctrl_TESTEX_CHKSTATUS	= BRDctrl_TEST + 143,	// Get check unit status (U32)
	BRDctrl_TESTEX_GETCHKERROR	= BRDctrl_TEST + 144,	// Get check unit error buffer (PBRD_CheckError)
	BRDctrl_TESTEX_GETCHKERMEM	= BRDctrl_TEST + 145,	// Get check unit error memory (PBRD_CheckErMem)

	BRDctrl_TESTEX_GENRESET		= BRDctrl_TEST + 150,	// Test generation unit reset (NULL)
	BRDctrl_TESTEX_GENSTART		= BRDctrl_TEST + 151,	// Start generation unit (NULL)
	BRDctrl_TESTEX_GENSTOP		= BRDctrl_TEST + 152,	// Stop generation unit (NULL)
	BRDctrl_TESTEX_GENCNTEN		= BRDctrl_TEST + 153,	// Set generation enable counter (U32)
	BRDctrl_TESTEX_GENCNTDS		= BRDctrl_TEST + 154,	// Set generation disable counter (U32)
	BRDctrl_TESTEX_GENSTATUS	= BRDctrl_TEST + 155,	// Get generation unit status (U32)

	BRDctrl_TESTEX_ILLEGAL
};

#pragma pack(push, 1) 

// check & generation units mode
typedef struct _BRD_TestExMode
{
	U32		FixMode;	// 1 - fixed block type
	U32		BlockType;	// block type
} BRD_TestExMode, *PBRD_TestExMode;

// Block types
enum {
	BRDblks_RUNONE64	= 0,	// 64-bits running 1
	BRDblks_RUNZERO64	= 1,	// 64-bits running 0
	BRDclks_RUNONEINV	= 2,	// 64-bits running 1 + inversion
	BRDclks_RUNZEROINV	= 3,	// 64-bits running 0 + inversion
	BRDclks_RUNONEBLK	= 4,	// running 1 into block
	BRDclks_RUNZEROBLK	= 5,	// running 0 into block
	BRDclks_COUNTER64	= 6,	// 64-bits counter
	BRDclks_RANDOM		= 8,	// randomization
};

// check error
typedef struct _BRD_CheckError
{
	U32		BlockOKCnt;		// OK block counter
	U32		BlockErrCnt;	// ERROR block counter
	U32		TotalError;		// TOTAL error
} BRD_CheckError, *PBRD_CheckError;

// check error memory
typedef struct _BRD_CheckErMem
{
	U32		AdrWord;	// IN - address of word
	U32		NumError;	// IN - number of error
	U32		Data;		// OUT - error data
} BRD_CheckErMem, *PBRD_CheckErMem;

typedef struct _BRD_Reg {
	S32 tetr;
	S32 reg; 
	U32 val; 
	void *pBuf; 
	S32 bytes; 
} BRD_Reg, *PBRD_Reg;

#pragma pack(pop)

#endif // _CTRL_TESTEX_H

//
// End of file
//