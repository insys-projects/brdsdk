/*
 ****************** File CtrlPio.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : PIO section
 *
 * (C) InSys by Dorokhin Andrey Mar 2006
 *
 ************************************************************
*/

#ifndef _CTRL_PIO_H
 #define _CTRL_PIO_H

#include "ctrladmpro.h"

enum {
	BRDctrl_PIO_SETDIR		= BRDctrl_PIO + 0,		// Set PIO data direction (PBRD_PioDir)
	BRDctrl_PIO_GETDIR		= BRDctrl_PIO + 1,		// Get PIO data direction (PBRD_PioDir)
	BRDctrl_PIO_SETMODE		= BRDctrl_PIO + 2,		// Set PIO mode (PBRD_PioMode)
	BRDctrl_PIO_GETMODE		= BRDctrl_PIO + 3,		// Get PIO mode (PBRD_PioMode)
	BRDctrl_PIO_WRITEDATA	= BRDctrl_PIO + 6,		// Write data to PIO (U32)
	BRDctrl_PIO_READDATA	= BRDctrl_PIO + 7,		// Read data from PIO (U32)
	BRDctrl_PIO_ENABLE		= BRDctrl_PIO + 8,		// PIO enable / disable (U32) - NOT USED !!!
	BRDctrl_PIO_ISLVDS		= BRDctrl_PIO + 9,		// Get PIO type (U32)
	BRDctrl_PIO_STATUS		= BRDctrl_PIO + 10,		// Get PIO status (U32)
	BRDctrl_PIO_FLAGCLR		= BRDctrl_PIO + 11,		// PIO status flag clear (NULL)

	BRDctrl_PIO_ILLEGAL
};

#pragma pack(push, 1)    

// Set/Get Data Direction
typedef struct _BRD_PioDir {
	U32	lbDir;		// low byte data direction
	U32	hbDir;		// high byte data direction
} BRD_PioDir, *PBRD_PioDir;

typedef struct _BRD_PioMode {
	U32	rdMode;		// read mode
	U32	wrMode;		// write mode - NOT USED (must be 0) !!!
	U32	strobeFlow;	// flow control of strobe signals (PIO_RD & PIO_WR)
	U32	ackFlow;	// flow control of acknowledge signals (ACK_RD & ACK_WR)
	U32	rdStrobe;	// read strobe signal length (PIO_RD) - (0 -255)
	U32	wrStrobe;	// write strobe signal length (PIO_WR) - (0 -255)
} BRD_PioMode, *PBRD_PioMode;

// Data Directions
enum {
	BRDpio_READ,	// 0 - read
	BRDpio_WRITE,	// 1 - write
};

#pragma pack(pop)    

#endif // _CTRL_PIO_H

//
// End of file
//