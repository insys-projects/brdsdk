/*
 ****************** File ctrlrfdr4_ddc.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants for rfdr4 service
 *
 * (C) InSys by Sklyarov A. Aug,2014
 *
 *
 ************************************************************
*/

#ifndef _CTRL_RFDR4_DDC_H
 #define _CTRL_RFDR4_DDC_H


#include "ctrladmpro.h"
typedef enum {
	
	BRDctrl_RFDR4_DDC_SETCHANMASK	= BRDctrl_DAQ + 1,  // Set mask of open channels (*U32)
	BRDctrl_RFDR4_DDC_SETSTARTMODE	= BRDctrl_DAQ + 2,  // Set start mode (BRD_StartMode_ddc)
	BRDctrl_RFDR4_DDC_SETFRAME		= BRDctrl_DAQ + 3,	// Set of FRAME(PBRD_SetFrame)
	BRDctrl_RFDR4_DDC_GETSRCSTREAM	= BRDctrl_DAQ + 4,	// Get source address for stream (*U32)
	BRDctrl_RFDR4_DDC_FIFORESET		= BRDctrl_DAQ + 5,	// FIFO reset (NULL)
	BRDctrl_RFDR4_DDC_START			= BRDctrl_DAQ + 6,	// Start(NULL)
	BRDctrl_RFDR4_DDC_STOP			= BRDctrl_DAQ + 7,	// Stop (NULL)
	BRDctrl_RFDR4_DDC_FIFOSTATUS	= BRDctrl_DAQ + 8,	// Get  FIFO status (*U32)
	BRDctrl_RFDR4_DDC_STARTDDC		= BRDctrl_DAQ + 9,	// Start DDC(NULL)
    BRDctrl_RFDR4_DDC_STOPDDC		= BRDctrl_DAQ + 10,	// Stop DDC(NULL)
	BRDctrl_RFDR4_DDC_SETDDCEXT		= BRDctrl_DAQ + 11,	// Set Mode of DDC (BRD_SetDdcExt)
	BRDctrl_RFDR4_DDC_SETCORRECTEXT	= BRDctrl_DAQ + 12,	// Set correctExt(BRD_SetCorrectExt)
	BRDctrl_RFDR4_DDC_SETSCAN		= BRDctrl_DAQ + 13,	// ��������� ������� (�������� ������ ������)(BRD_SetScan)
	BRDctrl_RFDR4_DDC_SELSCAN		= BRDctrl_DAQ + 14,	// ����� ������ ������� ��� ������(0 ��� 1 (*U32)
	BRDctrl_RFDR4_DDC_ENBSCAN		= BRDctrl_DAQ + 15,	// ���������/���������� ������ ������������ (*U32)
	BRDctrl_RFDR4_DDC_SETLABEL		= BRDctrl_DAQ + 16,	// ������ ��������� �����(32-��������� ���������������� ����� � ���������)(*U32)
	BRDctrl_RFDR4_DDC_INITPOWER		= BRDctrl_DAQ + 17,	// ���������� ������� (NULL)
	BRDctrl_RFDR4_DDC_GETPOWER		= BRDctrl_DAQ + 18,	// ���������� ������� (BRD_GetPower)
	BRDctrl_RFDR4_DDC_SETDETECT		= BRDctrl_DAQ + 19,	// ��������� ������ ��������� �������� (BRD_SetDetect)
	BRDctrl_RFDR4_DDC_GETDETECT		= BRDctrl_DAQ + 20,	// ��������� ������ ��������� �������� (*U32_)
	BRDctrl_RFDR4_DDC_SETLED		= BRDctrl_DAQ + 21,	// ��������� ������ ���������� (BRD_SetLed)
	BRDctrl_RFDR4_DDC_ILLEGAL
};

#pragma pack(push, 1)  


//-------------------------------------------------------------------------------
// for  SETSCAN function
typedef struct _BRD_SetScan
{	
		U32	wrscaner;	// ����� �������: 0 ��� 1 
		U32	*Fc;		// ��������� �� ������ ����������� ������ ����� ������� (8-�� �����) �� ��� ��������� 
		U32  Nscan;		// ����� ������ ������������ 
		U32 SampleRate; // ������� ����������� ������ � ��� (��)
} BRD_SetScan,*PBRD_SetScan;
//---------------------------------------------------------------------------------
// for  SETDDCEXT function
typedef struct _BRD_SetDdcExt
{	
		U32	*Fc;		// ��������� �� ������ ����������� ������ �����  ������� (8-�� �����)
		U32 SampleRate; // ������� ����������� ������ � ��� (��)
		U32 ChanMask;   // ����� ����� ������� DDC: 0x1 - 0xFF
} BRD_SetDdcExt,*PBRD_SetDdcExt;

// for  SETFRAME function
typedef struct _BRD_SetFrame
{	
	U32  FrameOn;				// 1- �������� ����� ��������� ����� 
	U32  HeaderOn;				// 1- ��������� ��������� � ������ ������
	U32  SyncWord;				// ����������� ��������� (32 �������)
	U32  UserWord;				// ����������� ��������� (32 �������)
	U32  SkipSamples;			// ����� ������������ �������� � ������ ������ (0-65535) 
	U32  GetSamples;			// ����� ���������� �������� � ������ (0-65535)
	U32  DelayStart;			// �������� ������  � �������������
} BRD_SetFrame, *PBRD_SetFrame;
//---------------------------------------------------------------------------------------
// for set  SETSTARTMODE mode function
typedef struct _BRD_StartMode_ddc 
{
	U32			startSrc;			// �������� ������: 0 - ����������, 1 - �������
	U32			startInv;			// 1 - �������� ������� ������
	U32			restartNco;			// 1 - ������������� �������� ���� NCO DDC ��� ������
	U32			restartCic;			// 1 - ������������� �������� ���� CIC DDC ��� ������
	U32			restartFir;			// 1 - ������������� �������� ���� FLT DDC ��� ������
} BRD_StartMode_ddc, *PBRD_StartMode_ddc;
//--------------------------------------------------------------------------------------
typedef struct _BRD_SetCorrectExt 
{
	U32			correctOn;			// 1 - �������� ���������(�������� ����� �� �������������� ������������)
	U32			rcoef[32];			// �������������� ����� �������������� ������������ ��� 32-� ������� (16�)
	U32			icoef[32];			// ������  ����� �������������� ������������ ��� 32-� ������� (16�)
} BRD_SetCorrectExt, *PBRD_SetCorrectExt;
//--------------------------------------------------------------------------------------
// for  SetDetect
typedef struct _BRD_SetDetect 
{
	U32			porog;			// ����� ���������� ������� ������������: 1 - 15 (� ������ �������� ������ 15!)
	U32			mode;			// �����  ��������� ��������: 0 - �������, 1 - �� �������
} BRD_SetDetect, *PBRD_SetDetect;
//--------------------------------------------------------------------------------------
// for  SetLed
typedef struct _BRD_SetLed 
{
	U32			control;		// ���������:0 - �����������, 1 - �� ������ ��������� ��������
	U32			level;			// ��������� ���������� ��� ����������� ����������: 0 - "�� �����", 1 - "�����";
} BRD_SetLed, *PBRD_SetLed;
//--------------------------------------------------------------------------------------
// for  GetPower
typedef struct _BRD_GetPower
{	
	    float	 v_3_3vx;	// ���������� ���� +3.3 VX		
	    float	 v_5_0vx;	// ���������� ���� +5.0 VX		
	    float	 v_12_0vx;	// ���������� ���� +12.0 VX		
		float	 i_5_0vx;	// ��� � ���� +5VX	
		float	 i_3_3vx;	// ��� � ���� +3.3VX	
		float	 i_12_0vx;	// ��� � ���� +12VX	
		float	 p_5_0vx;	// ������������ ��������� ��  ���� +5VX	
		float	 p_3_3vx;	// ������������ ��������� ��  +3.3VX	
		float	 p_12_0vx;	// ������������ ��������� ��  +12VX	
	    float	 v_1_0v;	// ���������� ���� +1.0 V		
		float	 v_1_8v;	// ���������� ���� +1.8 V	
		float	 v_2_5v;	// ���������� ���� +2.5V	
		float	 v_3_3vcco;	// ���������� ���� +3.3VCCO
		float	 v_3_3va;	// ���������� ���� +3.3VA
		float	 v_5_0va;	// ���������� ���� +5.0VA	
		float	 v_12_0va;	// ���������� ���� +12VA	
		float	 Tc;		// ����������� �	
} BRD_GetPower,*PBRD_GetPower;	

#pragma pack(pop)    

#endif // _CTRL_RFDR4_DDC_H

//
// End of file
//
