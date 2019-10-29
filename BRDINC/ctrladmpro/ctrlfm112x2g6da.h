/*
 ****************** File CtrlFm112x2g6da.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : FM112x2G6DA section
 *
 * (C) InSys by Ekkore Feb, 2016
 *
 *
 ************************************************************
*/

#ifndef _CTRL_FM112x2G6DA_H
 #define _CTRL_FM112x2G6DA_H

#include "ctrladc.h"

#pragma pack(push, 1)    

#define BRD_ADCCHANCNT			1	// Number of channels
#define BRD_DDCCHANCNT			2	// Number of DDC channels
//#define	BRD_GAINCNT			1	// Number of gains

//const int BRD_CLKDIVCNT	= 1; // Number of clock dividers = 1 (1)

// FM112x2G6DA Clock sources
enum {
	BRDclks_ADC_DISABLED	= 0,	// 
	BRDclks_ADC_SUBGEN_PLL	= 2,	// PLL � ���������� �����������
	BRDclks_ADC_EXTCLK_PLL	= 3,	// PLL � ������� �����������
	BRDclks_ADC_BASEGEN_PLL	= 4,	// PLL � ����������� ���������
	BRDclks_ADC_EXTCLK_DIV_PLL	= 0x43,	// DIV+PLL � ������� �����������
	BRDclks_ADC_BASEGEN_DIV_PLL	= 0x44,	// DIV+PLL � ����������� ���������
	BRDclks_ADC_EXTCLK		= 0x81,	// ������� ���������
	BRDclks_ADC_BASEGEN		= 0x85,	// ��������� ���������
	BRDclks_ADC_EXTCLK_DIV	= 0xC1,	// DIV � ������� ���������
	BRDclks_ADC_BASEGEN_DIV	= 0xC5,	// DIV � ����������� ���������

	BRDclks_DAC_DISABLED		= BRDclks_ADC_DISABLED,
	BRDclks_DAC_SUBGEN_PLL		= BRDclks_ADC_SUBGEN_PLL,
	BRDclks_DAC_EXTCLK_PLL		= BRDclks_ADC_EXTCLK_PLL,
	BRDclks_DAC_BASEGEN_PLL		= BRDclks_ADC_BASEGEN_PLL,
	BRDclks_DAC_EXTCLK_DIV_PLL	= BRDclks_ADC_EXTCLK_DIV_PLL,
	BRDclks_DAC_BASEGEN_DIV_PLL	= BRDclks_ADC_BASEGEN_DIV_PLL,
	BRDclks_DAC_EXTCLK			= BRDclks_ADC_EXTCLK,
	BRDclks_DAC_BASEGEN			= BRDclks_ADC_BASEGEN,
	BRDclks_DAC_EXTCLK_DIV		= BRDclks_ADC_EXTCLK_DIV,
	BRDclks_DAC_BASEGEN_DIV		= BRDclks_ADC_BASEGEN_DIV,
};

// FM112x2G6DA start sources
enum {
	BRDsts_ADC_CHAN0	= 0,	// Start from channel 0
	BRDsts_ADC_EXT		= 2,	// Start from START IN (external)
	BRDsts_ADC_PRG		= 3,	// Program start
};

// DAC Operation Mode
enum {
	BRDmode_DAC_DDS = 0,
	BRDmode_DAC_DAC = 1,
	BRDmode_DAC_QM  = 2,
};


// Numbers of Specific Command
typedef enum _FM112X2G6DA_NUM_CMD
{
	FM112X2G6DAcmd_SETMU		= 64,
	FM112X2G6DAcmd_GETMU		= 65,
	FM112X2G6DAcmd_SETDDCPARAM	= 66,
	FM112X2G6DAcmd_PRESYNC		= 67,
	FM112X2G6DAcmd_TIMESTAMP	= 68,
	FM112X2G6DAcmd_LOOPBACK		= 69,
	FM112X2G6DAcmd_SETDATAPATHMODE	= 70,
	FM112X2G6DAcmd_ADC_SETOFFSETADJUST = 71,
	FM112X2G6DAcmd_DAC_SETLMFCCALIBRATION	= 80,
	FM112X2G6DAcmd_DAC_GETLMFCCALIBRATION	= 81,
} FM112X2G6DA_NUM_CMD;

typedef struct _FM112X2G6DA_MU {
	U32		size;					// sizeof(FM112X2G6DASRV_MU)
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
} FM112X2G6DASRV_MU, *PF112X2G6DASRV_MU;

typedef struct _FM112X2G6DA_DDCPARAM {
	U32		isDdcEnable;		// ���������� DDC: 0-�������� ���, 1-�������� DDC
	U32		nDdcChannelMask;	// ����� ������� DDC: 0x1, 0x3, 0xF
	U32		nDdcDecimation;		// ����������� ��������� DDC: 2, 4, 8, 16

	U32		anDdcGain[BRD_DDCCHANCNT];	// ����. �������� ��� ������� DDC: 1, 2
	double	adDdcFc[BRD_DDCCHANCNT];	// ����������� ������� ��� ������� DDC (��)
} FM112X2G6DASRV_DDCPARAM, *PF112X2G6DASRV_DDCPARAM;

typedef struct  _FM112X2G6DASRV_DacLmfcCalibration {
	S32		idx;			// �� ������������, ������ ���� 0
	S32		aux;			// �� ������������, ������ ���� 0
	S32		nPClockFactor;	// (������ GET)
	S32		nPClockPerMF;	// (������ GET)
	S32		nK;				// (������ GET)
	S32		nCount;			// (������ GET) ���������� ����������� ����� � aRegVal[]
	S32		aRegVal[32];	// GET: �������� ��������� 0x302, 0x303 ��� ���� �/� AD9144
							// SET: aRegVal[0] �������� ��� 0x304/0x305, [1] - ��� 0x306/0x307
} FM112X2G6DASRV_DacLmfcCalibration, *PFM112X2G6DASRV_DacLmfcCalibration;

#pragma pack(pop)    

#endif // _CTRL_FM112x2G6DA_H

//
// End of file
//