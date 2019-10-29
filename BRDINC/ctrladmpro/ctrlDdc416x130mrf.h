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
		U32	mask,	 // ����� ������� ��� ���������������� 
			Fc;		 // ����������� �������  DDC (��) 
} BRD_SetDdcFc,*PBRD_SetDdcFc;
//------------------------------------
typedef struct _BRD_SetFrame
{	
	U32  EnbFrame;				// 1- �������� �������� �����
	U32  EnbHeader;				// 1- ��������� ��������� � ������ ������ ������
  	U32	 Marker;				// ������ ����������� � ���������  (0-0xFFFF) 
	U32  StartDelay;			// ����� ������������ �������� � ������ ����� (2-65535) 
	U32  GetSamples;			// ����� ���������� �������� � ������ (0-65535)
	U32  SkipSamples;			// ����� ��j���������  �������� � ����� ������ (0-65535)
	U32  NumStrings;			// ����� ����� � ����� (0-65535)
  	U32  EnbTestCount;			// 1-������ �������� DDC ������� �������� �������� �������� � �����
} BRD_SetFrame, *PBRD_SetFrame;
//------------------------------------
// getting configuration
typedef struct _BRD_DDCMRFCfg
{
	U32		SubType;				// ��� ���������
	U32		SubVer;					// ������ ���������
	U32		AdmConst;				// ���������� � �������� ���������
} BRD_DDCMRFCfg, *PBRD_DDCMRFCfg;

typedef struct _BRD_StartModeDDCMRF 
{
	U32			startSrc;			// �������� ������: 0-����������, 1-�������
	U32			startInv;			// �������� ���������� �������: 0-��� ��������, 1-��������
	U32			admMode;			// ����� ������: 0-������� � ���, 1-������� � DDC
	U32			EnbFrame;			//1- ������� �������� �����
} BRD_StartModeDDCMRF, *PBRD_StartModeDDCMRF;

#pragma pack(pop)    

#endif // _CTRL_DDCMRF_H

//
// End of file
//