/*
 ****************** File ctrlmrfddc.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : Ddc416x130mrf section
 *
 * (C) InSys by Sklyarov A. Apr,2012
 *
 *
 ************************************************************
*/

#ifndef _CTRL_DDCMRF_H
 #define _CTRL_DDCMRF_H


#include "ctrladmpro.h"
enum {
	
	BRDctrl_MRFDDC_ADMGETINFO		= BRDctrl_DAQ + 0,	// Get ADM INFO  (PBRD_AdmGetInfo)
	BRDctrl_MRFDDC_SETCLKMODE		= BRDctrl_DAQ + 1,	// Set mode of clock  (PBRD_ClkMode)
	BRDctrl_MRFDDC_SETCHANMASK		= BRDctrl_DAQ + 2,	// Set mask of  channels (*U32)
	BRDctrl_MRFDDC_SETSTARTMODE		= BRDctrl_DAQ + 3,  // Set start mode (PBRD_StartModeMRFDDC)
	BRDctrl_MRFDDC_SETADMMODE		= BRDctrl_DAQ + 4,	// Set mode of ADM:0-Adc,1- Ddc(*U32)
	BRDctrl_MRFDDC_GETSRCSTREAM		= BRDctrl_DAQ + 5,	// Get source address for stream (*U32)
	BRDctrl_MRFDDC_SETADC			= BRDctrl_DAQ + 6,	// Set param ADC (PBRD_SetAdc)
	BRDctrl_MRFDDC_SETDDCINP		= BRDctrl_DAQ + 7,	// Set input of DDC(PBRD_SetDdcInp)
	BRDctrl_MRFDDC_SETDDCFC			= BRDctrl_DAQ + 8,	// Set Fc of DDC(PBRD_SetDdcFc)
	BRDctrl_MRFDDC_SETDDCFLT		= BRDctrl_DAQ + 9,	// Load coeff. FIR-filter of DDC(*U32)
	BRDctrl_MRFDDC_SETDDCDECIM		= BRDctrl_DAQ + 10,	// Load Decimftion of DDC(PBRD_SetDdcDecim)
	BRDctrl_MRFDDC_FIFORESET		= BRDctrl_DAQ + 11,	// FIFO reset (NULL)
	BRDctrl_MRFDDC_START			= BRDctrl_DAQ + 12,	// Start(NULL)
	BRDctrl_MRFDDC_STOP				= BRDctrl_DAQ + 13,	// Stop (NULL)
	BRDctrl_MRFDDC_FIFOSTATUS		= BRDctrl_DAQ + 14,	// Get  FIFO status (*U32)

	BRDctrl_DDCMRF_ILLEGAL
};

#pragma pack(push, 1)    

//-----------------------------------------------------------
// for  SetAdc function
typedef struct _BRD_SetAdc
{	
		U32 dith,	 // 
			gain[4];	 //  
} BRD_SetAdc,*PBRD_SetAdc;
//----------------------------------------------------------

// for  SetFc function
typedef struct _BRD_SetDdcFc
{	
		U32	Fc0;		 // Центральная частота канала 0 (Гц) 
		U32	Fc1;		 // Центральная частота канала 1 (Гц) 
		U32	Fc2;		 // Центральная частота канала 2 (Гц) 
		U32	Fc3;		 // Центральная частота канала 3 (Гц) 
} BRD_SetDdcFc,*PBRD_SetDdcFc;
//--------------------------------------------------------------
// for  SetInp function
typedef struct _BRD_SetDdcInp
{	
		U32	Inp0;		 //  Номер АЦП подключённого к каналу 0 DDC:  0-3
		U32	Inp1;		 //  Номер АЦП подключённого к каналу 1 DDC:  0-3
		U32	Inp2;		 //  Номер АЦП подключённого к каналу 2 DDC:  0-3
		U32	Inp3;		 //  Номер АЦП подключённого к каналу 3 DDC:  0-3 
} BRD_SetDdcInp,*PBRD_SetDdcInp;
//-------------------------------------------------------------
// for  SetDecim function
typedef	struct _BRD_SetDdcDecim
{ U32  deccic,
	   decfir;
} BRD_SetDdcDecim,*PBRD_SetDdcDecim;
//--------------------------------------------------------------
typedef struct _BRD_StartModeMRFDDC 
{
	U32			startSrc;			// Источник старта: 0-внутренний, 1-внешний
	U32			startInv;			// Инверсия стартового сигнала: 0-нет инверсии, 1-инверсия
} BRD_StartModeMRFDDC, *PBRD_StartModeMRFDDC;
//---------------------------------------------------------------------
typedef struct _BRD_AdmGetInfo
{ 
	U32 admId;
	U32 pldId, pldModification, pldVersion, pldBuild;
	U32 trdId, trdIdModification, trdVersion ;
} BRD_AdmGetInfo, *PBRD_AdmGetInfo;

#pragma pack(pop)    

#endif // _CTRL_DDCMRF_H

//
// End of file
//