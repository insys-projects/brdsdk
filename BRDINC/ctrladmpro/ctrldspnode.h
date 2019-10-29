/*
 ****************** File CtrlDspnode.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : DSP node section
 *
 * (C) Instrumental Systems 
 *
 * Created: by Dorokhin A. Jun, 2005
 *
 ************************************************************
*/

#ifndef _CTRL_DSPNODE_H
 #define _CTRL_DSPNODE_H

#include "ctrladmpro.h"

enum {
	BRDctrl_DSPNODE_SETMODE			= BRDctrl_DSPNODE + 0,	// Set DSP node mode (U32)
	BRDctrl_DSPNODE_GETMODE			= BRDctrl_DSPNODE + 1,	// Get DSP node mode (U32)
	BRDctrl_DSPNODE_GETCFG			= BRDctrl_DSPNODE + 2,	// Get DSP node configuration ()

	BRDctrl_DSPNODE_GETDLGPAGES		= BRDctrl_DSPNODE + 127,	// get property pages of DSP node for dialog (PBRD_PropertyList)

	BRDctrl_DSPNODE_FIFORESET		= BRDctrl_DSPNODE + 128,	// DSP node FIFO reset (NULL)
	BRDctrl_DSPNODE_ENABLE			= BRDctrl_DSPNODE + 129,	// Enable/disable write to DSP node (U32)
	BRDctrl_DSPNODE_FIFOSTATUS		= BRDctrl_DSPNODE + 130,	// Get DSP node FIFO status (U32)
	BRDctrl_DSPNODE_GETDATA			= BRDctrl_DSPNODE + 131,	// Get data buffer (BRD_DataBuf)
	BRDctrl_DSPNODE_SETDATA			= BRDctrl_DSPNODE + 132,	// Set data buffer (BRD_DataBuf)
	BRDctrl_DSPNODE_READMEMDATA		= BRDctrl_DSPNODE + 133,	// Read data buffer from SBSRAM (BRD_DataBuf)
	BRDctrl_DSPNODE_WRITEMEMDATA	= BRDctrl_DSPNODE + 134,	// Write data buffer to SBSRAM (BRD_DataBuf)

	BRDctrl_DSPNODE_GETSRCSTREAM	= BRDctrl_DSPNODE + 138,	// Get source address for stream (U32)

	BRDctrl_DSPNODE_ILLEGAL
};

#pragma pack(push, 1) 

// getting configuration
typedef struct _BRD_DspNodeCfg
{
	U32		SramChips;			// количество микросхем статической пам€ти
	U32		SramChipSize;		// размер (глубина) микросхемы статической пам€ти
	U32		SramChipWidth;		// ширина (в битах) микросхемы статической пам€ти
} BRD_DspNodeCfg, *PBRD_DspNodeCfg;

#pragma pack(pop)    

#endif // _CTRL_DSPNODE_H

//
// End of file
//