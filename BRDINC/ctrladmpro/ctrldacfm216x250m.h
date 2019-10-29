/*
 ****************** File CtrlDac1624x192.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : DAC216x250mda section
 *
 * (C) InSys by Polepchuk Alexey
 *
 *
 ************************************************************
*/

#ifndef _CTRL_DAC216X250MDA_H
 #define _CTRL_DAC216X250MDA_H

#include "ctrldac.h"

#pragma pack(push, 1)    

typedef struct _FMDAC216X250MSRV_SETOFFSET {			
	U32		Interp;
} FMDAC216X250MSRV_SETINTERP, *PFMDAC216X250MSRV_SETINTERP;

// Numbers of Specific Command
typedef enum _DAC1624X192_NUM_CMD
{
	
	DACFM216X250Mcmd_PLLSTATUS		= 66,
	DACFM216X250Mcmd_SETINTERP,

	DACFM216X250Mcmd_ENABLECODOGRAM, // Enable Codegram(PBRD_Codegram)
	DACFM216X250Mcmd_DISABLECODOGRAM, // Disable Codegram(NULL)

	DACFM216X250Mcmd_SETINTREG,

	DACFM216X250Mcmd_PIO_ENABLE


} DAC1624X192_NUM_CMD;

typedef struct  _DAC1624X192_SETREG
{
	U32 reg;  
	U32 val;

} BRD_DacMdaReg;

typedef struct  _BRD_EnableCodogram
{
	U32 iscycle,  // циклический вывод кодограммы: 1-да, 0-нет 
		lcycle,   // содержимое регистра CG_CNT
		ncycles,  // содержимое регистра CG_CYCLE
		sstr0,    // содержимое регистра STB0_SET
		estr0,    // содержимое регистра STB0_CLR
		sstr1,    // содержимое регистра STB1_SET
		estr1,    // содержимое регистра STB1_CLR
		sstr2,    // содержимое регистра STB2_SET
		estr2,    // содержимое регистра STB2_CLR
		sstrdac,  // содержимое регистра STB_DAC_SET
		estrdac,  // содержимое регистра STB_DAC_CLR
		sstrobri, // содержимое регистра STB_RI_SET
		estrobri, // содержимое регистра STB_RI_CLR
		sstradc,  // содержимое регистра STB_ADC_SET
		estradc;  // содержимое регистра STB_ADC_CLR

	U32 burst;
} BRD_EnableCodogram, *PBRD_EnableCodogram;

#pragma pack(pop)    

#endif // _CTRL_DAC1624X192_H

//
// End of file
//