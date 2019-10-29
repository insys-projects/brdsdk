/*
 ****************** File CtrlAdc216x100m.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : ADC216x100M section
 *
 * (C) InSys by Dorokhin Andrey Jan, 2007
 *
 *
 ************************************************************
*/

#ifndef _CTRL_ADC216x100M_H
 #define _CTRL_ADC216x100M_H

#include "ctrladc.h"

#pragma pack(push, 1)    

#define BRD_CHANCNT			2	// Number of channels

const int BRD_CLKDIVCNT	= 5; // Number of clock dividers = 5 (1, 2, 4, 8, 16)

// ADC216x100M Clock sources
enum {
	BRDclks_ADC_DISABLED	= 0,	// 
	BRDclks_ADC_SUBGEN1		= 1,	// SubModule Generator 1
	BRDclks_ADC_SUBGEN2		= 2,	// SubModule Generator 2
	BRDclks_ADC_SUBGEN3		= 3,	// SubModule Generator 3
	BRDclks_ADC_EXTCLK		= 0x81,	// External SubModule Clock (from EXT CLK IN)
	BRDclks_ADC_MASTERCLK	= 0x82,	// External Clock from MASTER card (synthesizer - from CLK IN)
};

const int BRD_GAINCNT	= 8; // Number of gains

// input gain
const double BRD_Gain[BRD_GAINCNT] =
{
	0.125,	// 0x00
	0.188,	// 0x01
	0.250,	// 0x02
	0.376,	// 0x03
	2.500,	// 0x04
	3.760,	// 0x05
	5.000,	// 0x06
	7.576	// 0x07
};

// input range (Volt)
const double BRD_Range[BRD_GAINCNT] =
{
	4.000,	// 0x00
	2.660,	// 0x01
	2.000,	// 0x02
	1.330,	// 0x03
	0.200,	// 0x04
	0.133,	// 0x05
	0.100,	// 0x06
	0.066	// 0x07
};

// Numbers of Specific Command
typedef enum _ADC216X100M_NUM_CMD
{
	ADC216X100Mcmd_SETDITHER	= 64,
	ADC216X100Mcmd_GETDITHER	= 65,
	ADC216X100Mcmd_SETMU		= 66,
} ADC216X100M_NUM_CMD;

typedef struct _ADC216X100MSRV_MU {
	U32		size;					// sizeof(ADC216X100MSRV_MU)
	U32		chanMask;				// ����� ��������� �������
	U32		syncMode;				// ����� ������������� (2 - Master, 1 - Single, 0 - Slave)
	double	samplingRate;			// ������� ������������� (��) (��������� �������� �������� = 1, 2, 4, 8, 16)
	double	clockValue;				// �������� ������� ���������� �������� ��������� (��)
	U32		clockSrc;				// �������� �������� ������� (0 - ��������, 1,2,3 - ��������� 1,2,3 �� ���������, 0x81 - �������)
	double	range[BRD_CHANCNT];		// ������� �������� ��� ������� ������ (4.0 �����, 2.66 �����, 2.0 �����, 1.33 �����, 0.2 �����, 0.133 �����, 0.1 �����, 0.066 �����)
	double	bias[BRD_CHANCNT];		// �������� ���� ��� ������� ������
	U32		resist[BRD_CHANCNT];	// ������� ������������� ��� ������� ������ (0 - 1 kO�, 1 - 50 O�)
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
	U32		baseStartSrc;			// ������� �������� ������: 0 - �����������, 2 - SDX (���������� 1), 10 - �� DDS (only for AMBPEX8)
	U32		baseStartInv;			// 1 - �������� �������� ��������� ������
	U32		baseStopSrc;			// ������� �������� ��������
	U32		baseStopInv;			// 1 - �������� �������� ��������� ��������
	U32		startStopMode;			// 1 - ����� ����������� ������-��������, 0 - ����� �������������� ������-��������
	U32		reStart;				// 1 - ����� ����� � �������������� ������������
	double	thrClk;					// ����� ��������� ������� (�� -2.5 ����� �� 2.5 �����)
	U32		dither;					// 1 - ��������� �������� ����
} ADC216X100MSRV_MU, *PADC216X100MSRV_MU;

#pragma pack(pop)    

#endif // _CTRL_ADC216x100M_H

//
// End of file
//