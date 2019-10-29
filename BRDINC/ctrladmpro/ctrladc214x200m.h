/*
 ****************** File CtrlAdc214x200m.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : ADC214x200M section
 *
 * (C) InSys by Dorokhin Andrey Jan, 2007
 *
 *
 ************************************************************
*/

#ifndef _CTRL_ADC214x200M_H
 #define _CTRL_ADC214x200M_H

#include "ctrladc.h"

#pragma pack(push, 1)    

#define BRD_CHANCNT			2	// Number of channels

const int BRD_CLKDIVCNT	= 9; // Number of clock dividers = 9 (1, 2, 4, 8, 16, 32, 64, 128, 256)

// ADC214x200M Clock sources
enum {
	BRDclks_ADC_DISABLED	= 0,	// 
	BRDclks_ADC_SUBGEN		= 1,	// SubModule Generator (ver 1.1+)
	BRDclks_ADC_REFPLL		= 2,	// SubModule Clock from PLL
	BRDclks_ADC_EXTCLK		= 0x81,	// External SubModule Clock
};

// ADC214x200M start sources
enum {
	BRDsts_ADC_CHAN0	= 0,	// Start from channel 0
	BRDsts_ADC_CHAN1	= 1,	// Start from channel 1
	BRDsts_ADC_EXT		= 2,	// Start from START IN (external)
	BRDsts_ADC_PRG		= 3,	// Program start
};

const int BRD_GAINCNT	= 4; // Number of gains

// input gain
const double BRD_Gain[BRD_GAINCNT] =
{
	0.05,	// 0x00
	0.25,	// 0x01
	1.0,	// 0x02
	5.0		// 0x03
};

// input range (Volt)
const double BRD_Range[BRD_GAINCNT] =
{
	10.0,	// 0x00
	2.0,	// 0x01
	0.5,	// 0x02
	0.1		// 0x03
};

// Numbers of Specific Command
typedef enum _ADC214X200M_NUM_CMD
{
	ADC214X200Mcmd_SETMU		= 64,
	ADC214X200Mcmd_SETGAIN		= 66,
	ADC214X200Mcmd_SETCHANGAIN	= 67,
} ADC214X200M_NUM_CMD;

typedef struct _ADC214X200MSRV_MU {
	U32		size;					// sizeof(ADC214X200MSRV_MU)
	U32		chanMask;				// ����� ��������� �������
	U32		syncMode;				// ����� ������������� (2 - Master, 1 - Single, 0 - Slave)
	double	samplingRate;			// ������� ������������� (��) (��������� �������� �������� = 1, 2, 4, 8, 16, 32, 64, 128, 256)
	double	clockValue;				// �������� ������� ���������� �������� ��������� (��)
	U32		clockSrc;				// �������� �������� ������� (0 - ��������, 1 - ��������� �� ��������� (ver 1.1+), 2 - ���������� �� ���������, 0x81 - �������)
	double	range[BRD_CHANCNT];		// ������� �������� ��� ������� ������ (10.0 �����, 2.0 �����, 0.5 �����, 0.1 �����)
	double	bias[BRD_CHANCNT];		// �������� ���� ��� ������� ������
	U32		resist[BRD_CHANCNT];	// ������� ������������� ��� ������� ������ (0 - 1 �O�, 1 - 50 O�)
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
	double	gainTuning[BRD_CHANCNT];// ���������� �������� ������� ������ (�� -2% �� +2%)
	U32		invClk;					// 1 - �������� ������� ������������
} ADC214X200MSRV_MU, *PADC214X200MSRV_MU;

#pragma pack(pop)    

#endif // _CTRL_ADC214x200M_H

//
// End of file
//