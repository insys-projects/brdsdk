/*
 ****************** File CtrlFm212x4gda.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : FM212x4GDA section
 *
 * (C) InSys by Ekkore June 2017
 *
 ************************************************************
*/

#ifndef _CTRL_FM212x4GDA_H
 #define _CTRL_FM212x4GDA_H

#include "ctrladc.h"

#pragma pack(push, 1)    

#define BRD_ADCCHANCNT			2	// Number of channels
#define BRD_DDCCHANCNT			2	// Number of DDC channels
#define BRD_DACCHANCNT			4	// Number of DDC channels
//#define	BRD_GAINCNT			1	// Number of gains

//const int BRD_CLKDIVCNT	= 1; // Number of clock dividers = 1 (1)

// FM212x4GDA Clock sources
enum {
	BRDclks_ADC_DISABLED	= 0,	// 
	//BRDclks_ADC_SUBGEN		= 1,	// SubModule Internal Generator 
	BRDclks_ADC_SUBGEN_PLL	= 2,	// SubModule Internal Generator + PLL
	BRDclks_ADC_EXTCLK		= 0x81,	// External SubModule Clock
	BRDclks_ADC_EXTCLK_PLL	= 3,	// External SubModule Clock + PLL
	BRDclks_ADC_BASEGEN		= 0x85,	// Base Module Generator

	BRDclks_DAC_DISABLED	= 0,	// 
	//BRDclks_DAC_SUBGEN		= 1,	// SubModule Generator
	BRDclks_DAC_SUBGEN_PLL	= 2,	// SubModule Internal Generator + PLL
	BRDclks_DAC_EXTCLK		= 0x81,	// External SubModule Clock
	BRDclks_DAC_EXTCLK_PLL	= 3,	// External SubModule Clock + PLL
	BRDclks_DAC_BASEGEN		= 0x85,	// Base Module Generator
};

// FM212x4GDA start sources
enum {
	BRDsts_ADC_CHAN0	= 0,	// Start from channel 0
	BRDsts_ADC_EXT		= 2,	// Start from START IN (external)
	BRDsts_ADC_PRG		= 3,	// Program start
};



// Numbers of Specific Command
typedef enum _FM212X4GDA_NUM_CMD
{
	FM212X4GDAcmd_SETMU		= 64,
	FM212X4GDAcmd_GETMU		= 65,
	FM212X4GDAcmd_SETDDCPARAM		= 66,
	FM212X4GDAcmd_GETDDCPARAM		= 67,
	FM212X4GDAcmd_SETDACPARAM		= 68,
	FM212X4GDAcmd_GETDACPARAM		= 69,
	FM212X4GDAcmd_DAC_SETLMFCCALIBRATION	= 80,
	FM212X4GDAcmd_DAC_GETLMFCCALIBRATION	= 81,
	FM212X4GDAcmd_ADC_SETSYSREFCALIBRATION	= 82,
	FM212X4GDAcmd_DAC_KFO_SETKGALL	= 83,
	FM212X4GDAcmd_DAC_KFO_GETKGALL	= 84,
	FM212X4GDAcmd_DAC_KFO_SETKGONE	= 85,
} FM212X4GDA_NUM_CMD;

typedef struct _FM212X4GDA_MU {
	U32		size;					// sizeof(FM212X4GDASRV_MU)
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
} FM212X4GDASRV_MU, *PF212X4GDASRV_MU;

typedef struct _FM212X4GDA_DDCPARAM {
	U32		nDdcDecimation;				// ����������� ��������� DDC: 4, 8, 10, 16, 20, 32
	double	adDdcFc[BRD_DDCCHANCNT];	// ����������� ������� ��� ������� DDC (��)
	double	adDdcPhase[BRD_DDCCHANCNT];	// ��������� ���� ��� ������� DDC (����): 0.0 - 360.0
} FM212X4GDASRV_DDCPARAM, *PF212X4GDASRV_DDCPARAM;

typedef struct _FM212X4GDA_DACPARAM { 
	// ��� AD9136 ������������ ������ ����:
	// nInvSincFilterA, 
	// dGroupDelayA,
	// adDigitalGain[0 � 1],
	// adDcOffset[0 � 1]

	S32		nDModEnableA;		// �������� ��������� ������� ���0 � ���1: 0-����. 1-���.
	S32		nDModEnableB;		// �������� ��������� ������� ���2 � ���3: 0-����. 1-���.
	REAL64	dDModFreqA;			// ����������� ������� NCO ������� ���0 � ���1 (��)
	REAL64	dDModFreqB;			// ����������� ������� NCO ������� ���2 � ���3 (��)

	S32		nInvSincFilterA;	// ��������� ������ ������� ���0 � ���1: 0-����. 1-���.
	S32		nInvSincFilterB;	// ��������� ������ ������� ���2 � ���3: 0-����. 1-���.
	REAL64	dPhaseAdjustA;		// ����������� ���� ����� �������� ���0 � ���1: -14..+14 (������ �������)
	REAL64	dPhaseAdjustB;		// ����������� ���� ����� �������� ���2 � ���3: -14..+14 (������ �������)
	REAL64	dGroupDelayA;		// �������� ���0 � ���1 �� ���������� ������: -4..+3.5
	REAL64	dGroupDelayB;		// �������� ���2 � ���3 �� ���������� ������: -4..+3.5
	REAL64	adDigitalGain[BRD_DACCHANCNT];	// ����. �������� ���0-���3: �� 0.0 �� 2.0
	REAL64	adDcOffset[BRD_DACCHANCNT];		// ��������� �������� ���� ���0-���3 (���): -32768..+32767
} FM212X4GDASRV_DACPARAM,*PF212X4GDASRV_DACPARAM;

typedef struct _FM212X4GDA_AdcChipParam {
	S32		isCalibrate;					// ��������� ��������������: 1-��, 0-���
	U32		anCalibrateSig[BRD_DDCCHANCNT];	// ������� ������� ���0, ��� �������� ���������: 0 - ���� -10dBFS, 1 - ���� -10dBFS
	S32		anOverThd0[BRD_DDCCHANCNT];		// ������� ������ ���������� OVR_T0: 1 - 255
	S32		anOverThd1[BRD_DDCCHANCNT];		// ������� ������ ���������� OVR_T1: 1 - 255
	double	adInpRangeV[BRD_DDCCHANCNT];	// 0 ��� ����� �������������� ������� ���0-1 (�)
	double	adBiasV[BRD_DDCCHANCNT];		// �������� ���� ������� ���0-1 (�)
} FM212X4GDA_AdcChipParam, *PFM212X4GDA_AdcChipParam;

typedef struct  _FM212X4GDASRV_DacLmfcCalibration {
	S32		idx;			// �� ������������, ������ ���� 0
	S32		aux;			// �� ������������, ������ ���� 0
	S32		nPClockFactor;	// (������ GET)
	S32		nPClockPerMF;	// (������ GET)
	S32		nK;				// (������ GET)
	S32		nCount;			// (������ GET) ���������� ����������� ����� � aRegVal[]
	S32		aRegVal[32];	// GET: �������� ��������� 0x302, 0x303 ��� ���� �/� AD9144
							// SET: aRegVal[0] �������� ��� 0x304/0x305, [1] - ��� 0x306/0x307
} FM212X4GDASRV_DacLmfcCalibration, *PFM212X4GDASRV_DacLmfcCalibration;

#define		KFO_STBNUM		6
typedef struct  _FM212X4GDA_KFO_KGALL {
	U32		isKg;				// ���������/���������� ����������: 1-���., 0-����.
	U32		kgDuration;			// ������������ ����� ����������
	U32		loopCnt;			// ���������� �������� ����������
	U32		infinite;			// ����������� ����. 0 - �����������
	U32		aStbSet[6];			// ��������� ������� 0-5. ���� 0 - ����� ��������
	U32		aStbClr[6];			// ����� ������� 0-5. ���� 0 - ����� ��������
	U32		aStbInv[6];			// �������� ������� 0-5. ���� 0 - ��� ��������
} FM212X4GDASRV_KFO_KGALL,*PFM212X4GDASRV_KFO_KGALL;

typedef struct  _FM212X4GDA_KFO_KGONE {
	U32		stbIdx;				// ����� ���������������� ������: 0-5
	U32		stbSet;				// ��������� ������. ���� 0 - ����� ��������
	U32		stbClr;				// ����� ������. ���� 0 - ����� ��������
	U32		stbInv;				// �������� ������. ���� 0 - ��� ��������
} FM212X4GDASRV_KFO_KGONE,*PFM212X4GDASRV_KFO_KGONE;

#pragma pack(pop)    

#endif // _CTRL_FM212x4GDA_H

//
// End of file
//