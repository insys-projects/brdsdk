/*
 ****************** File CtrlFm214x250m.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : FM216x250M section
 *
 * (C) InSys by Ekkore Aug, 2011
 *
 *
 ************************************************************
*/

#ifndef _CTRL_FM216x250M_H
 #define _CTRL_FM216x250M_H

#include "ctrladc.h"

#pragma pack(push, 1)    

#define BRD_CHANCNT			2	// Number of channels

// FM216x250M Clock sources
enum {
	BRDclks_ADC_DISABLED	= 0,	// 
	BRDclks_ADC_SUBGEN		= 1,	// SubModule Internal Generator 
	BRDclks_ADC_EXTCLK		= 0x81	// External SubModule Clock
};

// FM216x250M start sources
enum {
	BRDsts_ADC_CHAN0	= 0,	// Start from channel 0
	BRDsts_ADC_CHAN1	= 1,	// Start from channel 1
	BRDsts_ADC_EXT		= 2,	// Start from START IN (external)
	BRDsts_ADC_PRG		= 3,	// Program start
};



// Numbers of Specific Command
typedef enum _FM216X250M_NUM_CMD
{
	FM216X250Mcmd_SETMU		= 64,
	FM216X250Mcmd_GETMU		= 65,
} FM216X250M_NUM_CMD;

typedef struct _FM216X250MSRV_MU {
	U32		size;					// sizeof(ADC214X200MSRV_MU)
	S32		subType;				// ��� ���������: 0 - ��������, 1 - WB HF, 2 - WB LF
	U32		chanMask;				// ����� ��������� �������
	U32		syncMode;				// ����� ������������� (2 - Master, 1 - Single, 0 - Slave)
	double	samplingRate;			// ������� ������������� (��) (��������� �������� �������� = 1, 2, 4, 8, 16, 32, 64, 128, 256)
	double	clockValue;				// �������� ������s ���������� ��������� ������������ (��) (��� ���������� 2, 4, 0x81, 0x82)
	U32		clockSrc;				// �������� �������� �������: 0-����., 1-����������, 2-����������+PLL, 4-�� ���������, 0x81-�������, 0x82-�������+PLL 
	
	double	range[BRD_CHANCNT];		// ������� �������� ��� ������� ������ (10.0 �����, 2.0 �����, 0.5 �����, 0.1 �����)
	double	bias[BRD_CHANCNT];		// �������� ���� ��� ������� ������
	U32		inpFilter[BRD_CHANCNT];	// ������� ������������� ��� ������� ������ (0 - 1 �O�, 1 - 50 O�)
	U32		dcCoupling[BRD_CHANCNT];// ���������� (1) / ���������� (0) ����� �� ���������� ������������ ��� ������� ������
	U32		format;					// 0 - 16 ���, 1 - 8 ���

	U32		pretrigEnable;			// ��������� ������ �����������
	U32		pretrigExernal;			// ��������� ������ �������� �����������
	U32		pretrigAssur;			// ��������� ������ ���������������� �����������
	U32		pretrigSamples;			// ����� �������� �����������
	U32		startDelayCnt;			// ������� ��������� ��������
	U32		daqCnt;					// ������� ����������� ������
	U32		skipCnt;				// ������� ����������� ������
	U32		cnt0Enable;				// ���������� �������� ��������� ��������
	U32		cnt1Enable;				// ���������� �������� ����������� ������
	U32		cnt2Enable;				// ���������� �������� ����������� ������

	U32		baseStartSrc;			// ������� �������� ������: 0 - �����������, 7 - �� ���������, 10 - �� DDS (only for AMBPEX8)
	U32		baseStartInv;			// 1 - �������� �������� ��������� ������
	U32		baseStopSrc;			// ������� �������� ��������
	U32		baseStopInv;			// 1 - �������� �������� ��������� ��������
	U32		startStopMode;			// 1 - ����� ����������� ������-��������, 0 - ����� �������������� ������-��������
	U32		reStart;				// 1 - ����� ����� � �������������� ������������
	U32		startSrc;				// ������������ �������� ������: 0 - ����� 0, 1 - ����� 1, 2 - �������, 3 - �����������
	U32		invStart;				// 1 - �������� ������������� ��������� ������
	double	startLevel;				// ������� ������ � ������� (�� -2.5 � �� +2.5 � � ��� ������� ������)
	U32		startResist;			// ��.������������� ��� ������� �������� ������: 0 - 2.5 ���, 1 - 50 ��
	double	gainTuning[BRD_CHANCNT];// ���������� �������� ������� ������ (�� -2% �� +2%)
	U32		invClk;					// 1 - �������� ������� ������������

	///////
	S32	 offset[BRD_CHANCNT];	//  �������� 0 ���: -127..127	
	S32	 vref[BRD_CHANCNT];		//  ������� ���������� ���:0-2.0�,1-2.1�,2-2.2�,3-2.3�,4-2.4�,5-2.5
	S32	 bufCur1[BRD_CHANCNT];	//  ��� �������� ������ 1: -100,-90..0,10,20..520,530	
	S32	 bufCur2[BRD_CHANCNT];	//  ��� �������� ������ 2: -100,-90..0,10,20..520,530

} FM216X250MSRV_MU, *PFM216X250MSRV_MU;


#pragma pack(pop)    

#endif // _CTRL_FM216x250M_H

//
// End of file
//