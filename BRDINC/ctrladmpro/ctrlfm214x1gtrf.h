/*
 ****************** File CtrlFm214x1gtrf.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : FM214x1GTRF section
 *
 * (C) InSys by Ekkore Feb, 2016
 *
 *
 ************************************************************
*/

#ifndef _CTRL_FM214x1GTRF_H
 #define _CTRL_FM214x1GTRF_H

#include "ctrladc.h"

#pragma pack(push, 1)    

#define BRD_ADCCHANCNT			2	// Number of channels
#define BRD_DDCCHANCNT			4	// Number of DDC channels
//#define	BRD_GAINCNT			1	// Number of gains

//const int BRD_CLKDIVCNT	= 1; // Number of clock dividers = 1 (1)

// FM214x1GTRF Clock sources
enum {
	BRDclks_ADC_DISABLED	= 0,	// 
	BRDclks_ADC_SUBGEN		= 1,	// SubModule Internal Generator 
	BRDclks_ADC_SUBGEN_PLL	= 2,	// SubModule Internal Generator + PLL
	BRDclks_ADC_EXTCLK		= 0x81,	// External SubModule Clock
	BRDclks_ADC_EXTCLK_PLL	= 3,	// External SubModule Clock + PLL
	BRDclks_ADC_BASEGEN		= 0x85,	// Base Module Generator

	BRDclks_DAC_DISABLED	= 0,	// 
	BRDclks_DAC_SUBGEN		= 1,	// SubModule Generator
	BRDclks_DAC_SUBGEN_PLL	= 2,	// SubModule Internal Generator + PLL
	BRDclks_DAC_EXTCLK		= 0x81,	// External SubModule Clock
	BRDclks_DAC_EXTCLK_PLL	= 3,	// External SubModule Clock + PLL
	BRDclks_DAC_BASEGEN		= 0x85,	// Base Module Generator
};

// FM214x1GTRF start sources
enum {
	BRDsts_ADC_CHAN0	= 0,	// Start from channel 0
	BRDsts_ADC_EXT		= 2,	// Start from START IN (external)
	BRDsts_ADC_PRG		= 3,	// Program start
};



// Numbers of Specific Command
typedef enum _FM214X1GTRF_NUM_CMD
{
	FM214X1GTRFcmd_SETMU		= 64,
	FM214X1GTRFcmd_GETMU		= 65,
	FM214X1GTRFcmd_SETDDCPARAM		= 66,
	FM214X1GTRFcmd_DAC_SETLMFCCALIBRATION	= 80,
	FM214X1GTRFcmd_DAC_GETLMFCCALIBRATION	= 81,
} FM214X1GTRF_NUM_CMD;

typedef struct _FM214X1GTRF_MU {
	U32		size;					// sizeof(FM214X1GTRFSRV_MU)
	U32		chanMask;				// ����� ��������� �������
	U32		syncMode;				// ����� ������������� (2 - Master, 1 - Single, 0 - Slave) ������ 1
	double	samplingRate;			// ������� ������������� (��): 10 ��� .. 125 ���
	double	clockValue;				// �������� ������� ���������� �������� ��������� (��)
	U32		clockSrc;				// �������� �������� ������� (0 - ��������, 1 - ��������� �� ���������, 0x81 - �������)
	double	range[BRD_ADCCHANCNT];		// ����� �������������� ��� ������� ������ (�����): 4.0, 2.0, 1.0, 0.5, 0.25
	double	bias[BRD_ADCCHANCNT];		// �������� ���� ��� ������� ������ (�����): -2*�� .. +2*��
	U32		inpResist[BRD_ADCCHANCNT];	// ������� ������������� ��� ������� ������ (0 - 1 �O�, 1 - 50 O�)
	U32		dcCoupling[BRD_ADCCHANCNT];	// ���������� (1) / ���������� (0) ����� �� ���������� ������������ ��� ������� ������
	U32		format;					// 0 - 16 ���, 1 - 8 ���

	U32		startSrc;				// ������������ �������� ������: 0 - ����� 0, 1 - ����� 1, 3 - �����������
	double	startLevel;				// ������� ������ � ������� (�� -2.5 � �� +2.5 � � ��� ������� ������)
} FM214X1GTRFSRV_MU, *PF214X1GTRFSRV_MU;

typedef struct _FM214X1GTRF_DDCPARAM {
	U32		isDdcEnable;		// ���������� DDC: 0-�������� ���, 1-�������� DDC
	U32		nDdcChannelMask;	// ����� ������� DDC: 0x1, 0x3, 0xF
	U32		nDdcDecimation;		// ����������� ��������� DDC: 2, 4, 8, 16
	U32		nDdcMixerSelect;	// ��� ������� �� ���: 0-��������������, 1-�����������

	U32		anDdcInpI[BRD_DDCCHANCNT];	// ����� ���, � �������� ���������� Input I DDC: 0, 1
	U32		anDdcInpQ[BRD_DDCCHANCNT];	// ����� ���, � �������� ���������� Input Q DDC: 0, 1
	U32		anDdcGain[BRD_DDCCHANCNT];	// ����. �������� ��� ������� DDC: 1, 2
	double	adDdcFc[BRD_DDCCHANCNT];	// ����������� ������� ��� ������� DDC (��)
	double	adDdcPhase[BRD_DDCCHANCNT];	// ��������� ���� ��� ������� DDC (����): 0.0 - 360.0
} FM214X1GTRFSRV_DDCPARAM, *PF214X1GTRFSRV_DDCPARAM;

typedef struct  _FM214X1GTRFSRV_DacLmfcCalibration {
	S32		idx;			// �� ������������, ������ ���� 0
	S32		aux;			// �� ������������, ������ ���� 0
	S32		nPClockFactor;	// (������ GET)
	S32		nPClockPerMF;	// (������ GET)
	S32		nK;				// (������ GET)
	S32		nCount;			// (������ GET) ���������� ����������� ����� � aRegVal[]
	S32		aRegVal[32];	// GET: �������� ��������� 0x302, 0x303 ��� ���� �/� AD9144
							// SET: aRegVal[0] �������� ��� 0x304/0x305, [1] - ��� 0x306/0x307
} FM214X1GTRFSRV_DacLmfcCalibration, *PFM214X1GTRFSRV_DacLmfcCalibration;

#pragma pack(pop)    

#endif // _CTRL_FM214x1GTRF_H

//
// End of file
//