/*
 ****************** File CtrlGrAdc.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : ADC section
 *
 * (C) InSys by Sklyarov A. Feb, 2011
 *
 *
 ************************************************************
*/

#ifndef _CTRL_GRADC_H
 #define _CTRL_GRADC_H


#include "ctrladmpro.h"
typedef enum {
	
	BRDctrl_GRADC_FIFORESET				= BRDctrl_ADC ,		// GRADC FIFO reset (NULL)
	BRDctrl_GRADC_START					= BRDctrl_ADC + 1,	// GRADC Start
	BRDctrl_GRADC_STOP					= BRDctrl_ADC + 2,	// GRADC Stop
	BRDctrl_GRADC_FIFOSTATUS			= BRDctrl_ADC + 3,	// Get GRADC FIFO status (U32)
	BRDctrl_GRADC_GETSRCSTREAM			= BRDctrl_ADC + 5,	// Get source address for stream (U32)
	BRDctrl_GRADC_SETCHANMASK			= BRDctrl_ADC + 6,	// Set mask enable channels(U32)
	BRDctrl_GRADC_SETMODE				= BRDctrl_ADC + 7,	// Set mode GRADC(BRD_GrAdcSetMode)
	BRDctrl_GRADC_SETMODEDDC			= BRDctrl_ADC + 9,	// Set mode GRADC DDC(BRD_GrDdcSetMode)
	BRDctrl_GRADC_SETFLTDDC				= BRDctrl_ADC + 10,	// Set filter GRADC DDC(BRD_GrDdcSetFilter)

	BRDctrl_GRADC_ILLEGAL
};

#pragma pack(push, 1)    



// for GrAdcSetMode  function
typedef struct _BRD_GrAdcSetMode
{
	U32	  DecimFlt2,	// 
		  BypassFlt3,	// 
		  CDIV,			// 
		  SCR,			// 
		  InputMux[16],		// 
	      InputAtten[16],	// 
		  InputLPF[16],		// 
	      InputGain[16],	// 
		  LowPower,		//
		  Gain[16],		//
		  Offset[16];		//
} BRD_GrAdcSetMode, *PBRD_GrAdcSetMode;

// for GrDdcSetMode  function
typedef struct _BRD_GrDdcSetMode
{
	U32	  Output,	// 
		  MaskChn,			// 
		  MaskObz,			// 
		  GainDdc,		// 
		  GainDdcObz,		// 
		  DecimCic,		// 
		  DecimFir,	
		  DecimCicObz,	
		  DecimFirObz,	
		  BypassCic,// 
		  BypassFir,// 
		  FreqNco[16];		// 
} BRD_GrDdcSetMode, *PBRD_GrDdcSetMode;
// for GrDdcSetFilter  function
typedef struct _BRD_GrDdcSetFilter
{
	U32	nDdc;	// 
	S32 Coeff[256];		// 
} BRD_GrDdcSetFilter, *PBRD_GrDdcSetFilter;
#pragma pack(pop)    

#endif // _CTRL_GRADC_H

//
// End of file
//