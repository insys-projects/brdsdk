/*
 ****************** File CtrlDataInOut.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : Test section
 *
 * (C) Instrumental Systems 
 *
 * Created: by Dorokhin A. Mar, 2007
 *
 ************************************************************
*/

#ifndef _CTRL_DATAINOUT_H
 #define _CTRL_DATAINOUT_H

#include "ctrladmpro.h"

enum {
	BRDctrl_DATAIN_SETMODE		= BRDctrl_TEST + 0,	// Set Test mode (U32)
	BRDctrl_DATAIN_GETMODE		= BRDctrl_TEST + 1,	// Get Test mode (U32)
	BRDctrl_DATAOUT_SETMODE		= BRDctrl_TEST + 2,	// Set Test mode (U32)
	BRDctrl_DATAOUT_GETMODE		= BRDctrl_TEST + 3,	// Get Test mode (U32)
	BRDctrl_DATAIN_GETCFG		= BRDctrl_TEST + 4,	// Get DataIn configuration (PBRD_DataInOutCfg)
	BRDctrl_DATAOUT_GETCFG		= BRDctrl_TEST + 5,	// Get DataOut configuration (PBRD_DataInOutCfg)

	BRDctrl_DATAIN_FIFORESET	= BRDctrl_TEST + 128,	// In FIFO reset (NULL)
	BRDctrl_DATAIN_ENABLE		= BRDctrl_TEST + 129,	// Enable/disable read data (U32)
	BRDctrl_DATAIN_FIFOSTATUS	= BRDctrl_TEST + 130,	// Get In FIFO status (U32)
	BRDctrl_DATAIN_GETDATA		= BRDctrl_TEST + 131,	// Get data buffer (BRD_DataBuf)

	BRDctrl_DATAOUT_FIFORESET	= BRDctrl_TEST + 132,	// Out FIFO reset (NULL)
	BRDctrl_DATAOUT_ENABLE		= BRDctrl_TEST + 133,	// Enable/disable write data (U32)
	BRDctrl_DATAOUT_FIFOSTATUS	= BRDctrl_TEST + 134,	// Get Out FIFO status (U32)
	BRDctrl_DATAOUT_PUTDATA		= BRDctrl_TEST + 135,	// Set data buffer (BRD_DataBuf)

	BRDctrl_DATAIN_GETSRCSTREAM	 = BRDctrl_TEST + 138,	// Get source address for stream (U32)
	BRDctrl_DATAOUT_GETSRCSTREAM = BRDctrl_TEST + 139,	// Get source address for stream (U32)

	BRDctrl_TEST_ILLEGAL
};

#pragma pack(push, 1) 

// getting configuration
typedef struct _BRD_DataInOutCfg
{
	U32		FifoSize;					// размер FIFO (в байтах)
} BRD_DataInOutCfg, *PBRD_DataInOutCfg;

#pragma pack(pop)    

#endif // _CTRL_DATAINOUT_H

//
// End of file
//