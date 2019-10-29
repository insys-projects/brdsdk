/*
 ****************** File ctrddc24.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants for ddc1g8 service
 *
 * (C) InSys by Sklyarov A. Oct,2013
 *
 *
 ************************************************************
*/

#ifndef _CTRL_DDC24_H
 #define _CTRL_DDC24_H


#include "ctrladmpro.h"
typedef enum {
	
	BRDctrl_DDC24_SETCLOCK		= BRDctrl_DAQ + 1,	// Set Clock ADC Hz (*U32)
	BRDctrl_DDC24_SETCHAN		= BRDctrl_DAQ + 2,	// Set mask of  channels (*U32)
	BRDctrl_DDC24_SETFC			= BRDctrl_DAQ + 3,	// Set Fc of DDC(PBRD_SetFc)
	BRDctrl_DDC24_GETSRCSTREAM	= BRDctrl_DAQ + 4,	// Get source address for stream (*U32)
	BRDctrl_DDC24_FIFORESET		= BRDctrl_DAQ + 5,	// FIFO reset (NULL)
	BRDctrl_DDC24_START			= BRDctrl_DAQ + 6,	// Start(NULL)
	BRDctrl_DDC24_STOP			= BRDctrl_DAQ + 7,	// Stop (NULL)
	BRDctrl_DDC24_STARTDDC		= BRDctrl_DAQ + 8,	// Start(NULL)
	BRDctrl_DDC24_STOPDDC		= BRDctrl_DAQ + 9,	// Stop (NULL)
	BRDctrl_DDC24_FIFOSTATUS	= BRDctrl_DAQ + 10,	// Get  FIFO status (*U32)

	BRDctrl_DDC24_ILLEGAL
};

#pragma pack(push, 1)  


//----------------------------------------------------------------------------
// for  SetFc function
typedef struct _BRD_SetFc
{	
		U32	 ClockAdc;	// Частота дискретизации АЦП (Гц)
		U32	 *pFc;		// Указатель на массив частот центральных частоты каналов (Гц)
} BRD_SetFc,*PBRD_SetFc;
//----------------------------------------------------------------------------

#pragma pack(pop)    

#endif // _CTRL_DDC24_H

//
// End of file
//