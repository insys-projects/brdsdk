/*
 ****************** File ctrddc640.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants for ddc1g8 service
 *
 * (C) InSys by Sklyarov A. Oct,2014
 *
 *
 ************************************************************
*/

#ifndef _CTRL_DDC640_H
 #define _CTRL_DDC640_H


#include "ctrladmpro.h"
typedef enum {
	
	BRDctrl_DDC640_SETMODE		= BRDctrl_DAQ + 0,	// Set mode(*U32): 0-samples DDC, 1-samples ADC
	BRDctrl_DDC640_SETCHANS		= BRDctrl_DAQ + 1,	// Set mask of  channels (PBRD_SetChans))
	BRDctrl_DDC640_SETFCWB		= BRDctrl_DAQ + 2,	// Set Fc of WB  DDC(PBRD_SetFcWb)
	BRDctrl_DDC640_SETFCNB		= BRDctrl_DAQ + 3,	// Set Fc of NB  DDC(PBRD_SetFcNb)
	BRDctrl_DDC640_GETSRCSTREAM	= BRDctrl_DAQ + 4,	// Get source address for stream (*U32)
	BRDctrl_DDC640_FIFORESET	= BRDctrl_DAQ + 5,	// FIFO reset (NULL)
	BRDctrl_DDC640_START		= BRDctrl_DAQ + 6,	// Start(NULL)
	BRDctrl_DDC640_STOP			= BRDctrl_DAQ + 7,	// Stop (NULL)
	BRDctrl_DDC640_FIFOSTATUS	= BRDctrl_DAQ + 8,	// Get  FIFO status (*U32)
	BRDctrl_DDC640_GETSTAT		= BRDctrl_DAQ + 9,	// Get  statistic (*U32)
	BRDctrl_DDC640_SETCHANS800	= BRDctrl_DAQ + 10,	// Set mask of  channels (PBRD_SetChans))
	BRDctrl_DDC640_SETFCWB800	= BRDctrl_DAQ + 11,	// Set Fc of WB  DDC(PBRD_SetFcWb) for 800 cyannels
	BRDctrl_DDC640_SETCHANS800_5	= BRDctrl_DAQ + 12,	// Set mask of  channels (PBRD_SetChans_5))
	BRDctrl_DDC640_ILLEGAL
};

#pragma pack(push, 1)  

//------------------------------------------------------------------------------------------------
// for  SetChans function
typedef struct _BRD_SetChans
{	
		U32	NumStreams;	// ����� ������� ������: 1-4
		U32	Mask0;		// ����� ������ ������� ������ 0 : 0x1 - 0xfffff (������ ���� - 40 �������)
		U32	Mask1;		// ����� ������ ������� ������ 1 : 0x1 - 0xfffff (������ ���� - 40 �������)	
		U32	Mask2;		// ����� ������ ������� ������ 2 : 0x1 - 0xfffff (������ ���� - 40 �������)
		U32	Mask3;		// ����� ������ ������� ������ 3 : 0x1 - 0xfffff (������ ���� - 40 �������)
} BRD_SetChans,*PBRD_SetChans;
//-------------------------------------------------------------------------------------------------
// for  SetChans function
typedef struct _BRD_SetChans_5
{	
		U32	NumStreams;	// ����� ������� ������: 1-5
		U32	Mask0;		// ����� ������ ������� ������ 0 : 0x1 - 0xfffff (������ ���� - 40 �������)
		U32	Mask1;		// ����� ������ ������� ������ 1 : 0x1 - 0xfffff (������ ���� - 40 �������)	
		U32	Mask2;		// ����� ������ ������� ������ 2 : 0x1 - 0xfffff (������ ���� - 40 �������)
		U32	Mask3;		// ����� ������ ������� ������ 3 : 0x1 - 0xfffff (������ ���� - 40 �������)
		U32	Mask4;		// ����� ������ ������� ������ 4 : 0x1 - 0xfffff (������ ���� - 40 �������)
} BRD_SetChans_5,*PBRD_SetChans_5;
//----------------------------------------------------------------------------

// for  SetFcWb function
typedef struct _BRD_SetFcWb
{	
		double	SamplingRate;	// ������� �������������  ��� (��)
		U32	 *pFc;				// ��������� �� ������ ������ ����������� ������� ������� (��)
} BRD_SetFcWb,*PBRD_SetFcWb;	
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// for  SetFcNb function
typedef struct _BRD_SetFcNb
{	
		double	SamplingRate;	// ������� ������������� ������� �������� (��)
		U32	 *pFc;				// ��������� �� ������ ������ ����������� ������� ������� (��)
} BRD_SetFcNb,*PBRD_SetFcNb;
//----------------------------------------------------------------------------


#pragma pack(pop)    

#endif // _CTRL_DDC640_H

//
// End of file
//