/*
 ****************** File ctrddckush.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants for  ddckush service
 *
 * (C) InSys by Sklyarov A. July,2015
 *
 *
 ************************************************************
*/

#ifndef _CTRL_DDCKUSH_H
 #define _CTRL_DDCKUSH_H


#include "ctrladmpro.h"
typedef enum {
	
	BRDctrl_DDCKUSH_SETCHANMASK		= BRDctrl_DAQ + 0,	// Set Mask of Channels(*U32)
	BRDctrl_DDCKUSH_SETMODE			= BRDctrl_DAQ + 1,	// Set Mode of DDC(PBRD_SetMode)
	BRDctrl_DDCKUSH_SETFC			= BRDctrl_DAQ + 2,	// Set Fc of  DDC(PBRD_SetFc)
	BRDctrl_DDCKUSH_SETFCPH			= BRDctrl_DAQ + 3,	// Set Fc and Ph  DDC(PBRD_SetFcPh)
	BRDctrl_DDCKUSH_GETSRCSTREAM	= BRDctrl_DAQ + 4,	// Get source address for stream (*U32)
	BRDctrl_DDCKUSH_FIFORESET		= BRDctrl_DAQ + 5,	// FIFO reset (NULL)
	BRDctrl_DDCKUSH_START			= BRDctrl_DAQ + 6,	// Start(NULL)
	BRDctrl_DDCKUSH_STOP			= BRDctrl_DAQ + 7,	// Stop (NULL)
	BRDctrl_DDCKUSH_FIFOSTATUS		= BRDctrl_DAQ + 8,	// Get  FIFO status (*U32)

	BRDctrl_DDCKUSH_ILLEGAL
};

#pragma pack(push, 1)  

//-----------------------------------------------------------------------------------------------
// for  SetFc function
typedef struct _BRD_SetFc
{	
		U32 	SamplingRate;	// Частота дискретизации  АЦП (Гц)
		U32		*Fc;				// Указатель на массив частот центральных частоты каналов (Гц)
		U32		DblClk;			// 1- режим удвоения частоты
} BRD_SetFc,*PBRD_SetFc;	
//----------------------------------------------------------------------------------------------
// for  Set FcPh function
typedef struct _BRD_SetFcPh
{	
		U32 	SamplingRate;	// Частота дискретизации  АЦП (Гц)
		U32		*Fc;			// Указатель на массив частот центральных частоты каналов (Гц)
		S32		*Ph;			// Указатель на массив начальных фаз каналов (Градус)
		U32		DblClk;			// 1- режим удвоения частоты
} BRD_SetFcPh,*PBRD_SetFcPh;	
//----------------------------------------------------------------------------------------------
// for  Set Mode function
typedef struct _BRD_SetMode
{	
		U32 	dblclk;				// 1- режим удвоения частоты
		U32		mode;				// Режим работы (децимация!)
} BRD_SetMode,*PBRD_SetMode;	
//----------------------------------------------------------------------------------------------

#pragma pack(pop)    

#endif // _CTRL_DDCKUSH_H

//
// End of file
//