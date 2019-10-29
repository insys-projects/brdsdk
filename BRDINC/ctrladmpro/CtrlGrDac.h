/*
 ****************** File CtrlGrDac.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : DAC section
 *
 * (C) InSys by Sklyarov A. Feb, 2011
 *
 *
 ************************************************************
*/

#ifndef _CTRL_GRDAC_H
 #define _CTRL_GRDAC_H


#include "ctrladmpro.h"
typedef enum {
	
	BRDctrl_GRDAC_FIFORESET				= BRDctrl_DAC ,		// GRDAC FIFO reset (NULL)
	BRDctrl_GRDAC_START					= BRDctrl_DAC + 1,	// GRDAC Start
	BRDctrl_GRDAC_STOP					= BRDctrl_DAC + 2,	// GRDAC Stop
	BRDctrl_GRDAC_FIFOSTATUS			= BRDctrl_DAC + 3,	// Get GRDAC FIFO status (U32)
	BRDctrl_GRDAC_PUTDATA				= BRDctrl_DAC + 4,	// Put data buffer into GRDAC FIFO (PVOID)
	BRDctrl_GRDAC_GETSRCSTREAM			= BRDctrl_DAC + 5,	// Get source address for stream (U32)
	BRDctrl_GRDAC_SETMODE				= BRDctrl_DAC + 6,	// Set mode GRDAC(BRD_GrDacSetMode)
	BRDctrl_GRDAC_SETCYCLEMODE			= BRDctrl_DAC + 7,	// Set cycle mode FIFO(U32)

	BRDctrl_GRDAC_ILLEGAL
};

#pragma pack(push, 1)    



// for GrDacSetMode  function
typedef struct _BRD_GrDacSetMode
{
	U32  EnableDac,   	// 0 - disable, 1 - enable
		 SampleRate,	// 0 - 48kHz, 1 - 96kHz, 2 - 192kHz
		 DeEmphasis,	// 1 - Enable  De-Emphasis Filter
		 TestSig;		// 0- left channel,1 - right channel, 2-?, 3-?
} BRD_GrDacSetMode, *PBRD_GrDacSetMode;

#pragma pack(pop)    

#endif // _CTRL_GRDAC_H

//
// End of file
//