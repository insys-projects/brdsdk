/*
 ****************** File ctrlrfdr4_adc.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants for rfdr4_adc service
 *
 * (C) InSys by Sklyarov A. Aug 2014
 *
 *
 ************************************************************
*/

#ifndef _CTRL_RFDR4_ADC_H
 #define _CTRL_RFDR4_ADC_H


#include "ctrladmpro.h"
enum {
	
	BRDctrl_RFDR4_ADC_SETCLOCK		= BRDctrl_DAQ + 0,	// Set  clock of system(PBRD_SetClk)
	BRDctrl_RFDR4_ADC_SETCHANMASK	= BRDctrl_DAQ + 1,  // Set mask of open channels (*U32)
	BRDctrl_RFDR4_ADC_SETSTARTMODE	= BRDctrl_DAQ + 2,	// Set mode of Start(PBRD_SetStartMode_adc)
	BRDctrl_RFDR4_ADC_INITADC		= BRDctrl_DAQ + 3,	// Init chips ADC(NULL)
	BRDctrl_RFDR4_ADC_SETBIAS		= BRDctrl_DAQ + 4,	// Set Bias (PBRD_SetBias)
	BRDctrl_RFDR4_ADC_SETBUFCUR		= BRDctrl_DAQ + 5,	// Set Buffer Current (PBRD_SetBufCur)
	BRDctrl_RFDR4_ADC_SETGAIN		= BRDctrl_DAQ + 6,	// Set Buffer Current (PBRD_SetGain)
	BRDctrl_RFDR4_ADC_SETRF			= BRDctrl_DAQ + 7,	// Set RF path  (PBRD_SetRf)
	BRDctrl_RFDR4_ADC_GETSRCSTREAM	= BRDctrl_DAQ + 8,	// Get source address for stream (*U32)
	BRDctrl_RFDR4_ADC_FIFORESET		= BRDctrl_DAQ + 9,	// FIFO reset (NULL)
	BRDctrl_RFDR4_ADC_START			= BRDctrl_DAQ + 10,	// Start(NULL)
	BRDctrl_RFDR4_ADC_STOP			= BRDctrl_DAQ + 11,	// Stop (NULL)
	BRDctrl_RFDR4_ADC_FIFOSTATUS	= BRDctrl_DAQ + 12,	// Get  FIFO status (*U32)
	BRDctrl_RFDR4_ADC_GETBITSOVERFLOW	= BRDctrl_DAQ + 13,	// Get  bits OverFlow (*U32)
	BRDctrl_RFDR4_ADC_CLRBITSOVERFLOW	= BRDctrl_DAQ + 14,	// Get  bits OverFlow (*U32)
	BRDctrl_RFDR4_ADC_SETOUTDELAY		= BRDctrl_DAQ + 15,	// Set  output_delay (PBRD_SetOutDelay)

    BRDctrl_RFDR4_ADC_ILLEGAL
};

#pragma pack(push, 1)  


//-------------------------------------------------------------------------------
// for  SetClock function
typedef struct _BRD_SetClock
{	
		U32	source;	// Источник: 0 - внутренний, 1 - внешний
		S32 value;	// Тактовая частота (Гц)
} BRD_SetClock,*PBRD_SetClock;
//-------------------------------------------------------------------------------

// for  SetBias function
typedef struct _BRD_SetBias
{	
		U32	chan;	// Номер канала
		S32 bias;	// -127...127
} BRD_SetBias,*PBRD_SetBias;
//-------------------------------------------------------------------------------
// for  SetBufCur function
typedef struct _BRD_SetBufCur
{	
		U32	chan;		// Номер канала
		S32 bufCur[2];	// -100..+530 с шагом 10
} BRD_SetBufCur,*PBRD_SetBufCur;
//-------------------------------------------------------------------------------
// for  SetGain function
typedef struct _BRD_SetGain
{	
		U32	chan;		// Номер канала
		float refGain;	// 1.0, 1.25
} BRD_SetGain,*PBRD_SetGain;
//-------------------------------------------------------------------------------
// for  SetOutDelay function
typedef struct _BRD_SetOutDelay
{
	U32	 chan;		// Номер канала (0 - 3)
	U32  outdelay;	// 0 - 31
} BRD_SetOutDelay, *PBRD_SetOutDelay;
//-------------------------------------------------------------------------------
// for  SetRf function
typedef struct _BRD_SetRf
{	
		U32	calibr;			// 0 -  калибратора включён, 1 - выкдючен
		U32	enbblkrf;		// 1 -  разрешена блокировка входа МШУ импульсом Тп
		U32 atten[4];		// Аттенюация: 0..31 (дБ)
} BRD_SetRf,*PBRD_SetRf;
//-------------------------------------------------------------------------------
// for set  Start mode function
typedef struct _BRD_StartMode_adc 
{
	U32			startSrc;			// Start source
	U32			startInv;			// Start inversion
} BRD_StartMode_adc, *PBRD_StartMode_adc;
//-------------------------------------------------------------------------------


//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------

#pragma pack(pop)    

#endif // _CTRL_RFDR4_ADC_H

//
// End of file
//
