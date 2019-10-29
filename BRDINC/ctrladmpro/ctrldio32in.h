/*
 ****************** File CtrlDio32In.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : DIO32_IN section
 *
 * (C) InSys by Dorokhin Andrey Sep, 2007
 *
 *
 ************************************************************
*/

#ifndef _CTRL_DIO32IN_H
 #define _CTRL_DIO32IN_H

#include "ctrladmpro.h"

enum {
	BRDctrl_DIO32IN_SETCHANMASK		= BRDctrl_DIOIN + 0,	// Set mask of DIO32IN channels (U32)
	BRDctrl_DIO32IN_GETCHANMASK		= BRDctrl_DIOIN + 1,	// Get mask of DIO32IN channels (U32)
	BRDctrl_DIO32IN_SETFORMAT		= BRDctrl_DIOIN + 8,	// Set format (8/16-bit) data for DIO32IN (U32)
	BRDctrl_DIO32IN_GETFORMAT		= BRDctrl_DIOIN + 9,	// Get format (8/16-bit) data for DIO32IN (U32)

	BRDctrl_DIO32IN_SETCLKMODE		= BRDctrl_DIOIN + 12,	// Set DIO32IN clock mode - 1 = inverting (U32)
	BRDctrl_DIO32IN_GETCLKMODE		= BRDctrl_DIOIN + 13,	// Get DIO32IN clock mode - 1 = inverting (U32)
	BRDctrl_DIO32IN_SETMASTER		= BRDctrl_DIOIN + 16,	// Set DIO32IN master/single/slave mode (U32)
	BRDctrl_DIO32IN_GETMASTER		= BRDctrl_DIOIN + 17,	// Get DIO32IN master/single/slave mode (U32)

	BRDctrl_DIO32IN_FIFORESET		= BRDctrl_DIOIN + 128,	// DIO32IN enable (NULL)
	BRDctrl_DIO32IN_ENABLE			= BRDctrl_DIOIN + 129,	// DIO32IN disable (NULL)
	BRDctrl_DIO32IN_FIFOSTATUS		= BRDctrl_DIOIN + 130,	// Get DIO32IN FIFO status (U32)
	BRDctrl_DIO32IN_GETDATA			= BRDctrl_DIOIN + 131,	// Put data buffer into DIO32IN FIFO (PVOID)
	BRDctrl_DIO32IN_GETSRCSTREAM	= BRDctrl_DIOIN + 138,	// Get source address for stream (U32)

	BRDctrl_DIO32IN_FLAGCLR			= BRDctrl_DIOIN + 144,	// DIO32IN FIFO status flag clear (U32)

	BRDctrl_DIO32IN_ILLEGAL
};

#endif // _CTRL_DIO32IN_H

//
// End of file
//