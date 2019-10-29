/*
 ****************** File CtrlSvm.h **************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : SVEAM section
 *
 * (C) Instrumental Systems 
 *
 * Created: by Ekkore Oct, 2003
 * Modified:
 *
 ************************************************************
*/

#ifndef _CTRL_SVEAM_H
 #define _CTRL_SVEAM_H

#include "ctrl.h"



//=********************************************************
//
// Comand Codes
//
//=********************************************************

enum {
	BRDctrl_SVEAM_CBUF_ALLOC	= BRDctrl_SVEAM + 0,	// Alloc CBuf
	BRDctrl_SVEAM_CBUF_REALLOC	= BRDctrl_SVEAM + 1,	// Realloc CBuf
	BRDctrl_SVEAM_CBUF_FREE		= BRDctrl_SVEAM + 2,	// Free CBuf
	BRDctrl_SVEAM_CBUF_START	= BRDctrl_SVEAM + 3,	// Start Sveam
	BRDctrl_SVEAM_CBUF_STOP		= BRDctrl_SVEAM + 4,	// Stop Sveam
	BRDctrl_SVEAM_CBUF_STATE	= BRDctrl_SVEAM + 5,	// Get State Sveam
	BRDctrl_SVEAM_ILLEGAL
};

//=********************************************************
//
// Structure Declarations
//
//=********************************************************

#pragma pack(push, 1) 

//
// Stub Structure
//
typedef struct
{
	S32		lastBlock;				// Number of Block which was filled last Time
	U32		totalCounter;			// Total Counter of all filled Block
	U32		offset;					// First Unfilled Byte
	U32		state;					// CBUF local state
} BRDsvm_Stub, BRDctrl_SveamStub, *PBRDctrl_SveamStub;

//
// For BRDctrl_SVEAM_CBUF_ALLOC
//
typedef struct  
{
	U32				dir;		// IN,  Sveam Direction (1-to Host, 2-from Host)
	U32				isCont;		// IN, Allocation Method: 0-FileMapping, 1-System Pool (Ring 0)
	U32				blkNum;		// I/O, Number of Blocks
	U32				blkSize;	// I/O, Size of Blocks (bytes)
	U08**			ppBlk;		// OUT, Block Pointers 
	BRDsvm_Stub*	pStub;		// OUT, Stub Pointer
} BRDctrl_SveamCBufAlloc, *PBRDctrl_SveamCBufAlloc;

//
// For BRDctrl_SVEAM_CBUF_START
//
typedef struct  
{
	U32		isCycle;	// IN,  Cycle Mode (1-on, 0-off)
} BRDctrl_SveamCBufStart, *PBRDctrl_SveamCBufStart;

//
// For BRDctrl_SVEAM_CBUF_STATE
//
typedef struct  
{
	U32		blkNum;			// OUT, Last Filled Block Number
	U32		blkNumTotal;	// OUT, Total Filled Block Counter
	U32		offset;			// OUT, First Unfilled Byte
	U32		state;			// OUT, State of CBuf (1-start, 2-stop, 3-destroied)
	U32		timeout;		// IN,  Timeout (msec) (-1 - INFINITE)
} BRDctrl_SveamCBufState, *PBRDctrl_SveamCBufState;

#pragma pack(pop)    

//=********************************************************
//
// Constants
//
//=********************************************************

//
// Constants: State of CBUF
//

enum
{
	BRDsvm_STAT_RUN = 1,
	BRDsvm_STAT_STOP = 2,
	BRDsvm_STAT_DESTROY = 3
};

//
// Constants: Direction of CBUF
//

enum
{
	BRDsvm_DIR_IN = 0x1,				// To HOST
	BRDsvm_DIR_OUT = 0x2,				// From HOST
	BRDsvm_DIR_INOUT = 0x3				// Both Directions
};

//=********************************************************
//
// Error Codes
//
//=********************************************************

//
//	BRDerr_SVEAM_WARNING	
//
//  Sveam Warning
//
#define	BRDerr_SVEAM_WARNING			BRDctrl_SVEAM + 0x40000000L

//
//	BRDerr_SVEAM_ERROR	
//
//  Undefined Sveam Error
//
#define	BRDerr_SVEAM_ERROR				BRDctrl_SVEAM + 0xC0000000L

//
//	BRDerr_SVEAM_ALREADY_ALLOCATED	
//
//  If you try to Allocate Sveam in EXCLUSIVE Mode,
//  but Sveam has already been allocated
//
#define	BRDerr_SVEAM_ALREADY_ALLOCATED	BRDctrl_SVEAM + 0xC0000001L

//
//	BRDerr_SVEAM_NOT_ALLOCATED_YET
//
//  If you try to Allocate Sveam in SPY Mode,
//  but Sveam hasn't been allocated yet
//
#define	BRDerr_SVEAM_NOT_ALLOCATED_YET	BRDctrl_SVEAM + 0xC0000002L

//
//	BRDerr_SVEAM_CONTINUOUS
//
//  If you try to Allocate Sveam in SPY Mode,
//  but Sveam has been allocated as Continuous
//
#define	BRDerr_SVEAM_CONTINUOUS			BRDctrl_SVEAM + 0xC0000003L

//
//	BRDerr_SVEAM_ALREADY_DESTROYED	
//
//  1. If you try to Allocate Sveam in SPY Mode,
//  2. If you try to get state of Sveam in SPY Mode,
//
//  but Sveam has already been destroyed
//
#define	BRDerr_SVEAM_ALREADY_DESTROYED	BRDctrl_SVEAM + 0xC0000004L

//
//	BRDerr_SVEAM_UNLOCKED
//
//  Can't Probe and Lock Blocks
//
#define	BRDerr_SVEAM_UNLOCKED			BRDctrl_SVEAM + 0xC0000005L

//
//	BRDerr_SVEAM_ALREADY_STARTED	
//
//  If you try to Start Sveam in EXCLUSIVE Mode,
//  but Sveam has already been allocated
//
#define	BRDerr_SVEAM_ALREADY_STARTED	BRDctrl_SVEAM + 0xC0000006L

//
//	BRDerr_SVEAM_NOT_STARTED_YET
//
//  If you try to Stop Sveam in EXCLUSIVE Mode 
//  or to get state of Sveam,
//  but Sveam hasn't been started yet
//
#define	BRDerr_SVEAM_NOT_STARTED_YET	BRDctrl_SVEAM + 0xC0000007L

//
//	BRDerr_SVEAM_UNAVALIABLE
//
//  The Sveam isn't avaliable
//
#define	BRDerr_SVEAM_UNAVALIABLE		BRDctrl_SVEAM + 0xC0000008L




#endif // _CTRL_SVEAM_H

//
// End of file
//