/*
 ****************** File ctrddc8x32.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants for ddc8x32 service
 *
 * (C) InSys by Sklyarov A. Jul,2012
 *
 *
 ************************************************************
*/

#ifndef _CTRL_DDC8X32_H
 #define _CTRL_DDC8X32_H


#include "ctrladmpro.h"
typedef enum {
	
	BRDctrl_DDC8X32_SETCLOCK		= BRDctrl_DAQ + 1,	// Set Clock ADC Hz (*U32)
	BRDctrl_DDC8X32_SETMODE			= BRDctrl_DAQ + 2,	// Set Mode of DDC (PBRD_SetMode)
	BRDctrl_DDC8X32_SETFC			= BRDctrl_DAQ + 3,	// Set Fc of DDC(PBRD_SetFc)
	BRDctrl_DDC8X32_SETFIR			= BRDctrl_DAQ + 4,	// Load coeff. FIR-filter of DDC((PBRD_SetFir))
	BRDctrl_DDC8X32_SETDECIM		= BRDctrl_DAQ + 5,	// Load Decimftion of DDC(PBRD_SetDecim)
	BRDctrl_DDC8X32_SETCHAN			= BRDctrl_DAQ + 6,	// Set mask of  channels (PBRD_MaskChan)
//	BRDctrl_DDC8X32_SETCHAN			= BRDctrl_DAQ + 6,	// Set mask of  channels (*U32)
	BRDctrl_DDC8X32_GETSRCSTREAM	= BRDctrl_DAQ + 7,	// Get source address for stream (*U32)
	BRDctrl_DDC8X32_FIFORESET		= BRDctrl_DAQ + 8,	// FIFO reset (NULL)
	BRDctrl_DDC8X32_START			= BRDctrl_DAQ + 9,	// Start(NULL)
	BRDctrl_DDC8X32_STOP			= BRDctrl_DAQ + 10,	// Stop (NULL)
	BRDctrl_DDC8X32_STARTDDC		= BRDctrl_DAQ + 11,	// Start(NULL)
	BRDctrl_DDC8X32_STOPDDC			= BRDctrl_DAQ + 12,	// Stop (NULL)
	BRDctrl_DDC8X32_FIFOSTATUS		= BRDctrl_DAQ + 13,	// Get  FIFO status (*U32)
	BRDctrl_DDC8X32_SETDEBUG		= BRDctrl_DAQ + 14,	// Set Debug (*U32)
	BRDctrl_DDC8X32_SETOVER			= BRDctrl_DAQ + 15,	// Set param detector over(PBRD_SetOver)
	BRDctrl_DDC8X32_GETOVER			= BRDctrl_DAQ + 16,	// Get over(*U32)
	BRDctrl_DDC8X32_SETFRAME		= BRDctrl_DAQ + 17,	// Set Frame(PBRD_SetOver)

	BRDctrl_DDC8X32_ILLEGAL
};

#pragma pack(push, 1)  

// for  SetOver function
typedef struct _BRD_SetOver
{	
		U32	porog0,			// порог срабатывания детектора 0: 0-127
			porog1,			// порог срабатывания детектора 1: 0-127
			porog2,			// порог срабатывания детектора 2: 0-127
			porog3,			// порог срабатывания детектора 3: 0-127
			porog4,			// порог срабатывания детектора 4: 0-127
			porog5,			// порог срабатывания детектора 5: 0-127
			porog6,			// порог срабатывания детектора 6: 0-127
			porog7;			// порог срабатывания детектора 7: 0-127
							
} BRD_SetOver,*PBRD_SetOver;
// for  SetMode function
typedef struct _BRD_SetMode
{	
		U32	 wide,			// 0-для широкополосного режима, 1-для узкополосного 
			nslot;			// 0-3
} BRD_SetMode,*PBRD_SetMode;

//----------------------------------------------------------------------------
// for  SetFir function
typedef struct _BRD_SetFir
{	
		U32	 mode;			// 1-для широкополосного режима, 0-для узкополосного 
		S32	*coef;			// Указатель на массив коэффициентов (256 коэффициентов)
} BRD_SetFir,*PBRD_SetFir;
//----------------------------------------------------------------------------
// for  SetDecim function
typedef	struct _BRD_SetDecim
{ 
//	U32	 mode,				// 1-для широкополосного режима, 0-для узкополосного 
	U32	 dec_cic,			// Коэффициент децимации CIC-фильтра
		 dec_fir;			// Коэффициент децимации FIR-фильтра
} BRD_SetDecim,*PBRD_SetDecim;
//---------------------------------------------------------------------------
// for  SetFc function
typedef struct _BRD_SetFc
{	
		U32	mode;			//  1-для широкополосного режима, 0-для узкополосного  
		U32	*pFc;			//  Указатель на массив Fc (Гц), pFc[0]-канал 0,pFc[1]-канал 1 и т.д.
} BRD_SetFc,*PBRD_SetFc;	//  Для широкополосного режима длина массива-8, для узкополосного - 32
//--------------------------------------------------------------
// for  SetChan function
typedef	struct _BRD_MaskChan
{ 
	U32	 mode,				  // 0 - для широкополосного режима, 1 - для узкополосного режима
		 mask;				  // Маска включённых каналов. 0x0 - 0xffffffff для узкополосного режима,
} BRD_MaskChan,*PBRD_MaskChan;// 0x0 - 0xff для узкополосного режима,
//--------------------------------------------------------------
// for  SetFrame function
typedef	struct _BRD_SetFrame
{ 
	U32	 enb,				// 1 - включение кадрового режима
		 lframe,			// Длина кадра в отсчётах(0 - 65535)
		 sword1,				// 1 синхрослово (32 разряда)
		 sword2,				// 2 синхрослово (32 разряда)
		 sword3,				// 3 синхрослово (32 разряда)
		 sword4;				// 4 синхрослово (32 разряда)
} BRD_SetFrame,*PBRD_SetFrame;
//--------------------------------------------------------------

#pragma pack(pop)    

#endif // _CTRL_DDC8X32_H

//
// End of file
//