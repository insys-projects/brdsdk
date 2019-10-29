/*
 ****************** File ctrms.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants for  ms service
 *
 * (C) InSys by Sklyarov A. Feb,2016
 *
 *
 ************************************************************
*/

#ifndef _CTRL_MS_H
 #define _CTRL_MS_H


#include "ctrladmpro.h"
enum {
	
	BRDctrl_MS_SETCLK			= BRDctrl_DAQ + 0,	// ���������� ����� ������������(PBRD_SetClk)
	BRDctrl_MS_SELOUTBUF		= BRDctrl_DAQ + 1,	// ������� ����� ������ ������� (0 ��� 1) (U32*)
	BRDctrl_MS_SETSYNC			= BRDctrl_DAQ + 2,	// ���������� ����� �������������� ������������� (PBRD_SetSync)
	BRDctrl_MS_SETCODEG			= BRDctrl_DAQ + 3,	// ���������� ��������� ���������� (PBRD_SetCodeg)
	BRDctrl_MS_LOADSIGNAL		= BRDctrl_DAQ + 4,	// ��������� ������� �������  � ������ (PBRD_LoadSignal)
	BRDctrl_MS_GETSRCSTREAM		= BRDctrl_DAQ + 5,	// �������� ����� ��������� ������ ��� ������ (*U32)
	BRDctrl_MS_FIFORESET		= BRDctrl_DAQ + 6,	// ����� FIFO (NULL)
	BRDctrl_MS_SETOUTMODE		= BRDctrl_DAQ + 7,	// ���������� ����� ������ ������ �� ������(*U32)  (0-����������, 1-�������)
	BRDctrl_MS_START			= BRDctrl_DAQ + 8,	// �����(NULL)
	BRDctrl_MS_STOP				= BRDctrl_DAQ + 9,	// ���� (NULL)
	BRDctrl_MS_FIFOSTATUS		= BRDctrl_DAQ + 10,	// �������� ������ FIFO (*U32)
	BRDctrl_MS_DACRESET			= BRDctrl_DAQ + 11,	// ����� ���������� ���(NULL)
	BRDctrl_MS_SETDUC			= BRDctrl_DAQ + 12,	// ���������� ����� ������ DUC(PBRD_SetDuc)
	BRDctrl_MS_STARTCODEG		= BRDctrl_DAQ + 13,	// ����� ����������(NULL)
	BRDctrl_MS_STOPCODEG		= BRDctrl_DAQ + 14,	// ��op ����������(NULL)
	BRDctrl_MS_SETMASTER		= BRDctrl_DAQ + 16,	// ���������� ������ �����
	BRDctrl_MS_SETDACREG		= BRDctrl_DAQ + 17,	// �������� ������ � ������� ���������� ��� (PBRD_SetDacReg)
	BRDctrl_MS_OUTSYNC			= BRDctrl_DAQ + 18,	// ������ ������� ������������� (NULL)
	BRDctrl_MS_INITPOWER		= BRDctrl_DAQ + 19,	// ���������������� ������� ����������� �������(U32*)
	BRDctrl_MS_GETPOWER			= BRDctrl_DAQ + 20,	// ���������� �������: �������� �������� ���������� ������� � �����(PBRD_GetPower)
	BRDctrl_MS_INITPOWSIG		= BRDctrl_DAQ + 21,	// ���������������� ������� ����������� �������� ��������(PBRD_InitPowSig)
	BRDctrl_MS_GETPOWSIG		= BRDctrl_DAQ + 22,	// ���������� �������� ��������: �������� �������� �������� ��������(PBRD_GetPowSig)
	BRDctrl_MS_TESTSTROB		= BRDctrl_DAQ + 23,	// ���� �������: ����������  �������� �� ������� SYNC1-SYNC4, STB1-STB3  � ������� ��������� ������� (*U32)
	BRDctrl_MS_SETLED	 	    = BRDctrl_DAQ + 24,	// ���������� ����������� (������ �������)(*U32)
	BRDctrl_MS_ILLEGAL
	};

#pragma pack(push, 1)  

//-----------------------------------------------------------------------------------------------
typedef struct _BRD_SetClk
{	
		U32		ClkSrc;		// �������� ��������� �������: 0-��������, 1-����������, 2-�������, 
		U32		ClkValue;
		U32		DacDelay;	// �������� ������ ��� ������ � ��� (0-31)
} BRD_SetClk,*PBRD_SetClk;	
//----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
typedef struct _BRD_SetSync
{	
		U32		SyncMode;	// ����� �������������: 0 - �������, ������� �������� �� �� ������-���������
							//					    1 - �������, �������� ������ ����������������� ����������� �� �� �� ������-��������� 	
							//					    2 - �������, �������� ������ ����� ��
							//						3 - �������, ��������  ����� �� ��� ����������� ������� 
		U32		NumSync;	// ����� ����� �������������: 1..4
		U32		BlockSync;	// 1 - ���������� ������ ������������� � �������  ������ ( ����������� �����)
} BRD_SetSync, *PBRD_SetSync;	
//----------------------------------------------------------------------------------------------
// for  Set Codegram function
typedef  struct _BRD_SetCodeg
{	
	    U32  Period_Tp;			//  ������ ���������� ����������:   (0..20000 ���)		
		U32	 Length_Tp;			//  ������������ ������� Tp (STR):  (0..20000 ���)
		U32	 Delay_STB1;		//  �������� ������� STB1:		    (0..20000 ���)
		U32  Length_STB1;		//  ������������ ������� STB1:		(0..20000 ���)
		U32  Delay_STB2;		//  �������� ������� STB2:			(0..20000 ���)
		U32  Length_STB2;		//  ������������ ������� STB2:      (0..20000 ���)
		U32  Delay_STB3;		//  �������� ������� STB3:			(0..20000 ���)
		U32  Length_STB3;		//  ������������ ������� STB3:      (0..20000 ���)
		U32  Delay_Tp2;			// �������� ��2 ������������ ��1, �������� DUC. ������������ ��������,( 0 - 64)
		U32  Lrst;				//  ������������ ������� �������� � ������
	    double  SampleRate;		//  ������� ������������� ��� (��)
} BRD_SetCodeg,*PBRD_SetCodeg;	
//----------------------------------------------------------------------------------------------
// for  Set Signal
typedef struct _BRD_LoadSignal
{	
	U32	 sel_buf;			// ����� ������ ������: 0,1
    U32	 Carrier1;			// ������ ������� ������� (��) 
    U32  Carrier2;			// ������ ������� ������� (��)
	U32	 DelayCarrier2;		// ����� �������(Carrier1 �������� �� Carrier2) �� ������ ����� : 0..26214 ���
    double  SampleRate;		// ������� ������������� ��� (��)
	U32  *samples;			// ��������� �� ����� ��������
	U32	 nsamples;			// ������ ������������ ��������
} BRD_LoadSignal,*PBRD_LoadSignal;	
//----------------------------------------------------------------------------------------------
// for  Set Duc
typedef struct _BRD_SetDuc
{	
	    U32  Mode;				// ����� ������ ���������� ������ �� ������ DUC :
								// 0 -  ������ � ������ CIC-������� DUC  (�������)
								// 1 - ������ � ������ ������������� ���������� (NCO ) DUC  (�������� ������)
								// 2 - ������ ��������� ������������ ���������� DUC  (����������� ������)
		U32	 DistSignal;		// ����������� ������� DUC:  0 - ��� 0 (�����������), 1 - ��� 1 (��������), �� ���1 � ���2
} BRD_SetDuc,*PBRD_SetDuc;	
//------------------------------
// for  Set DucStartMode
typedef struct _BRD_DucStartMode
{	
	    U32  startSrc;		// �������� ������		
		U32	 startInv;		// �������� ������
} BRD_DucStartMode,*PBRD_DucStartMode;

// for  SetDacReg
typedef struct _BRD_SetDacReg
{	
	    U32  regNum;		// ����� ������������� ��������		
		U32	 regVal;		// ������ ��� ������
} BRD_SetDacReg,*PBRD_SetDacReg;

//----------------------------------------------------------------

// for  GetPower
typedef struct _BRD_MsGetPower
{	
	    float	 v_3_3vx;	// ���������� ���� +3.3 VX		
	    float	 v_5_0vx;	// ���������� ���� +5.0 VX		
	    float	 v_12_0vx;	// ���������� ���� +12.0 VX		
		float	 i_5_0vx;		// ��� � ���� +5VX	
		float	 i_3_3vx;	// ��� � ���� +3.3VX	
		float	 i_12_0vx;	// ��� � ���� +12VX	
		float	 p_5_0vx;	// ������������ ��������� ��  ���� +5VX	
		float	 p_3_3vx;	// ������������ ��������� ��  +3.3VX	
		float	 p_12_0vx;	// ������������ ��������� ��  +12VX	
	    float	 v_1_0v;	// ���������� ���� +1.0 V		
		float	 v_1_8v;	// ���������� ���� +1.8 V	
		float	 v_dvdv18;	// ���������� ���� DVDD18	
		float	 v_3_3vcco;	// ���������� ���� +3.3VCCO
		float	 v_3_3va;	// ���������� ���� +3.3VA	
		float	 v_5_0va;		// ���������� ���� +5VA	
		float	 Tc;		// ����������� �	

} BRD_MsGetPower,*PBRD_MsGetPower;	
//----------------------------------------------------------------------------------------------
typedef struct _BRD_GetPowSig
{	
	   float	VrmsZS;		// 
	   float	VrmsCLBR;		// 
	   float	VrmsCLK;		// 
} BRD_GetPowSig,*PBRD_GetPowSig;
//-------------------------------------------------------------------
typedef struct _BRD_InitPowSig
{	
	    U32  rcfg0;		// 
	    U32  rcfg1;		// 	
	    U32  rcfg2;		// 
} BRD_InitPowSig, *PBRD_InitPowSig;
#pragma pack(pop)    

#endif // _CTRL_MS_H

//
// End of file
//
