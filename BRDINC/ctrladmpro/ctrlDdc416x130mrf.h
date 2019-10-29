/*
 ****************** File ctrlDdc416x130mrf.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : Ddc416x130mrf section
 *
 * (C) InSys by Sklyarov A. Dec. 2011
 *
 *
 ************************************************************
*/

#ifndef _CTRL_DDCMRF_H
 #define _CTRL_DDCMRF_H


#include "ctrladmpro.h"
typedef enum {
	
	BRDctrl_DDCMRF_GETCFG			= BRDctrl_DAQ + 0,	// Get configuration 
	BRDctrl_DDCMRF_SETCLKMODE		= BRDctrl_DAQ + 1,	// Set mode of clock  (PBRD_ClkMode)
	BRDctrl_DDCMRF_SETCHANMASK		= BRDctrl_DAQ + 2,	// Set mask of  channels (U32)
	BRDctrl_DDCMRF_SETSTARTMODE		= BRDctrl_DAQ + 3,  // Set start mode (BRD_StartModeDDCMRF)
	BRDctrl_DDCMRF_SETADMMODE		= BRDctrl_DAQ + 4,	// Set mode of ADM:0-Adc,1- Ddc(U32)
	BRDctrl_DDCMRF_GETSRCSTREAM		= BRDctrl_DAQ + 5,	// Get source address for stream (U32)
	BRDctrl_DDCMRF_SETADC			= BRDctrl_DAQ + 6,	// Set param ADC (BRD_SetAdc)
	BRDctrl_DDCMRF_SETDDCFC			= BRDctrl_DAQ + 7,	// Set Fc of DDC(BRD_SetDdcFc)
	BRDctrl_DDCMRF_SETDDCFLT		= BRDctrl_DAQ + 8,	// Load coeff. FIR-filter of DDC(*U32)
	BRDctrl_DDCMRF_SETDDCDECIM		= BRDctrl_DAQ + 9,	// Load Decimftion of DDC(*U32)
	BRDctrl_DDCMRF_SETFRAME			= BRDctrl_DAQ + 10,	// Set frame mode (BRD_SetFrame)
	BRDctrl_DDCMRF_FIFORESET		= BRDctrl_DAQ + 11,	// FIFO reset (NULL)
	BRDctrl_DDCMRF_START			= BRDctrl_DAQ + 12,	// Start(NULL)
	BRDctrl_DDCMRF_STOP				= BRDctrl_DAQ + 13,	// Stop (NULL)
	BRDctrl_DDCMRF_FIFOSTATUS		= BRDctrl_DAQ + 14,	// Get  FIFO status (U32)
	BRDctrl_DDCMRF_STARTDDC			= BRDctrl_DAQ + 15,	// Start DDC(NULL)
	BRDctrl_DDCMRF_STOPDDC			= BRDctrl_DAQ + 16,	// Stop DDC(NULL)
	BRDctrl_DDCMRF_SETMARKER		= BRDctrl_DAQ + 17,	// Stop DDC(NULL)

	BRDctrl_DDCMRF_ILLEGAL
};

#pragma pack(push, 1)    

//------------------------------------
// for  SetAdc function
typedef struct _BRD_SetAdc
{	
		U32 dith,	 // 
			gain;	 //  
} BRD_SetAdc,*PBRD_SetAdc;
//------------------------------------
// for  SetFc function
typedef struct _BRD_SetDdcFc
{	
		U32	mask,	 // Маска каналов для программирования 
			Fc;		 // Центральная частота  DDC (Гц) 
} BRD_SetDdcFc,*PBRD_SetDdcFc;
//------------------------------------
typedef struct _BRD_SetFrame
{	
	U32  EnbFrame;				// 1- включить кадровый режим
	U32  EnbHeader;				// 1- вставлять заголовок в начале каждой строки
  	U32	 Marker;				// Маркер вставляемый в заголовок  (0-0xFFFF) 
	U32  StartDelay;			// Число пропускаемых отсчётов в начале кадра (2-65535) 
	U32  GetSamples;			// Число получаемых отсчетов в строке (0-65535)
	U32  SkipSamples;			// Число прjпускаемых  отсчетов в конце строки (0-65535)
	U32  NumStrings;			// Число строк в кадре (0-65535)
  	U32  EnbTestCount;			// 1-вместо отсчётов DDC выдаётся значение счётчика отсчётов в кадре
} BRD_SetFrame, *PBRD_SetFrame;
//------------------------------------
// getting configuration
typedef struct _BRD_DDCMRFCfg
{
	U32		SubType;				// тип субмодуля
	U32		SubVer;					// версия субмодуля
	U32		AdmConst;				// информация о прошивке субмодуля
} BRD_DDCMRFCfg, *PBRD_DDCMRFCfg;

typedef struct _BRD_StartModeDDCMRF 
{
	U32			startSrc;			// Источник старта: 0-внутренний, 1-внешний
	U32			startInv;			// Инверсия стартового сигнала: 0-нет инверсии, 1-инверсия
	U32			admMode;			// Режим работы: 0-отсчёты с АЦП, 1-отсчёты с DDC
	U32			EnbFrame;			//1- включён кадровый режим
} BRD_StartModeDDCMRF, *PBRD_StartModeDDCMRF;

#pragma pack(pop)    

#endif // _CTRL_DDCMRF_H

//
// End of file
//