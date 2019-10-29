/*
 ****************** File ctrldsp134v.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : DSP134V section
 *
 * (C) InSys by Ekkore June, 2018
 *
 *
 ************************************************************
*/

#ifndef _CTRL_DSP134V_H
 #define _CTRL_DSP134V_H

#include "ctrladc.h"

#pragma pack(push, 1)    

#define BRD_ADCCHANCNT			2	// Number of channels
#define BRD_DDCCHANCNT			8	// Number of DDC channels
//#define	BRD_GAINCNT			1	// Number of gains

//const int BRD_CLKDIVCNT	= 1; // Number of clock dividers = 1 (1)

// DSP134V Clock sources
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

// DSP134V start sources
enum {
	BRDsts_ADC_CHAN0	= 0,	// Start from channel 0
	BRDsts_ADC_EXT		= 2,	// Start from START IN (external)
	BRDsts_ADC_PRG		= 3,	// Program start
};



// Numbers of Specific Command
typedef enum _DSP134V_NUM_CMD
{
	DSP134Vcmd_SETMU		= 64,
	DSP134Vcmd_GETMU		= 65,
	DSP134Vcmd_SETDDCPARAM		= 66,
	DSP134Vcmd_DAC_SETLMFCCALIBRATION	= 80,
	DSP134Vcmd_DAC_GETLMFCCALIBRATION	= 81,
} DSP134V_NUM_CMD;

typedef struct _DSP134V_MU {
	U32		size;					// sizeof(DSP134VFSRV_MU)
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
} DSP134VSRV_MU, *PDSP134VSRV_MU;

typedef struct _DSP134V_DDCPARAM {
	U32		isDdcEnable;		// ���������� DDC: 0-�������� ���, 1-�������� DDC
	U32		nDdcChannelMask;	// ����� ������� DDC: 0x1, 0x3, 0xF
	U32		nDdcDecimation;		// ����������� ��������� DDC: 2, 4, 8, 16
	U32		nDdcMixerSelect;	// ��� ������� �� ���: 0-��������������, 1-�����������

	U32		anDdcInpI[BRD_DDCCHANCNT];	// ����� ���, � �������� ���������� Input I DDC: 0, 1
	U32		anDdcInpQ[BRD_DDCCHANCNT];	// ����� ���, � �������� ���������� Input Q DDC: 0, 1
	U32		anDdcGain[BRD_DDCCHANCNT];	// ����. �������� ��� ������� DDC: 1, 2
	double	adDdcFc[BRD_DDCCHANCNT];	// ����������� ������� ��� ������� DDC (��)
	double	adDdcPhase[BRD_DDCCHANCNT];	// ��������� ���� ��� ������� DDC (����): 0.0 - 360.0
} DSP134VSRV_DDCPARAM, *PDSP134VSRV_DDCPARAM;

typedef struct  _DSP134VSRV_DacLmfcCalibration {
	S32		idx;			// �� ������������, ������ ���� 0
	S32		aux;			// �� ������������, ������ ���� 0
	S32		nPClockFactor;	// (������ GET)
	S32		nPClockPerMF;	// (������ GET)
	S32		nK;				// (������ GET)
	S32		nCount;			// (������ GET) ���������� ����������� ����� � aRegVal[]
	S32		aRegVal[32];	// GET: �������� ��������� 0x302, 0x303 ��� ���� �/� AD9144
							// SET: aRegVal[0] �������� ��� 0x304/0x305, [1] - ��� 0x306/0x307
} DSP134VSRV_DacLmfcCalibration, *PDSP134VSRV_DacLmfcCalibration;

#pragma pack(pop)    

#endif // _CTRL_DSP134V_H

//
// End of file
//