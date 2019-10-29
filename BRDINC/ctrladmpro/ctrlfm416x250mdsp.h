/*
 ****************** File CtrlFm816x250m.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : FM816x250M section
 *
 * (C) InSys by Ekkore Aug, 2011
 *
 *
 ************************************************************
*/

#ifndef _CTRL_FM816x250M_H
 #define _CTRL_FM816x250M_H

#include "ctrladc.h"

#pragma pack(push, 1)    

#define BRD_ADCCHANCNT			4	// Number of ADC channels
#define BRD_DDCCHANCNT			32	// Number of DDC channels
#define BRD_DDCDECCNT			3	// Number of DDC R-channel per channel
#define BRD_DDCFIRCNT			64	// Number of FIR-filter points
#define	BRD_GAINCNT			6	// Number of gains

const int BRD_CLKDIVCNT	= 5; // Number of clock dividers = 9 (1, 2, 4, 8, 16)

// FM816x250M Clock sources
enum {
	BRDclks_ADC_DISABLED	= 0,	// 
	BRDclks_ADC_SUBGEN		= 1,	// SubModule Generator 0
	BRDclks_ADC_EXTCLK		= 0x81,	// External SubModule Clock
	BRDclks_ADC_CLK0_C2M	= 0x85,	// Base Unit Clock CLK0_C2M
	BRDclks_ADC_CLK1_C2M	= 0x86,	// Base Unit Clock CLK1_C2M
};

// FM816x250M start sources
enum {
	BRDsts_ADC_CHAN0	= 0,	// Start from channel 0
	BRDsts_ADC_CHAN1	= 1,	// Start from channel 1
	BRDsts_ADC_EXT		= 2,	// Start from START IN (external)
	BRDsts_ADC_PRG		= 3,	// Program start
};



// Numbers of Specific Command
typedef enum _FM816x250M_NUM_CMD
{
	FM816x250Mcmd_SETMU		= 64,
	FM816x250Mcmd_GETMU		= 65,
	FM816x250Mcmd_SUBSYNC	= 66,

    FM416x250MDSPcmd_DDCENABLE = 67,
    FM416x250MDSPcmd_DDCSETCHANMASK = 68,
    FM416x250MDSPcmd_TSTMODEENABLE = 69,
    FM416x250MDSPcmd_SETDDSFREQ = 70,
    FM416x250MDSPcmd_FIRLOAD = 71
} FM816x250M_NUM_CMD;

typedef struct _FM816x250MSRV_MU {
	U32		size;					// sizeof(FM816x250MSRV_MU)
	U32		chanMask;				// ����� ��������� �������
	U32		syncMode;				// ����� ������������� (2 - Master, 1 - Single, 0 - Slave) ������ 1
	double	samplingRate;			// ������� ������������� (��): 10 ��� .. 125 ���
	double	clockValue;				// �������� ������� ���������� �������� ��������� (��)
	U32		clockSrc;				// �������� �������� ������� (0 - ��������, 1 - ��������� �� ���������, 0x81 - �������)
	double	range[BRD_ADCCHANCNT];		// ����� �������������� ��� ������� ������ (�����): 4.0, 2.0, 1.0, 0.5, 0.25
	double	bias[BRD_ADCCHANCNT];		// �������� ���� ��� ������� ������ (�����): -2*�� .. +2*��
	U32		inpResist[BRD_ADCCHANCNT];	// ������� ������������� ��� ������� ������ (0 - 1 �O�, 1 - 50 O�)
	U32		dcCoupling[BRD_ADCCHANCNT];// ���������� (1) / ���������� (0) ����� �� ���������� ������������ ��� ������� ������
	U32		format;					// 0 - 16 ���, 1 - 8 ���

	U32		startSrc;				// ������������ �������� ������: 0 - ����� 0, 1 - ����� 1, 3 - �����������
	double	startLevel;				// ������� ������ � ������� (�� -2.5 � �� +2.5 � � ��� ������� ������)
	double	clockLevel;				// ����� ����������� ������� �������� ������� (�): -2.5 .. +2.5
} FM816x250MSRV_MU, *PFM816x250MSRV_MU;

typedef struct _FM416X250MDSP_DDCPARAM {
	U32		isDdcEnable;		// ���������� DDC: 0-�������� ���, 1-�������� DDC
//	U32		nDdcDecimation;		// ����������� ��������� DDC: 16, 32, 64
//	U32		nFirSize;
	double	adDdcFc[BRD_DDCCHANCNT];	// ����������� ������� ��� ������� DDC (��)
//	U32		anDdcGain[BRD_DDCCHANCNT*BRD_DDCDECCNT];// ����. �������� ��� ������� DDC: 1-0xFFFF
//	U32		anDdcFir[BRD_DDCCHANCNT*BRD_DDCDECCNT][BRD_DDCFIRCNT];// ����. FIR-��������
	U32		nDdcChannelMask; // ����� ������� DDC, 0x1 - 16, 0x3 - 32, 0x7 - 48, 0xF - 64
	U32		nTstMode; // 1 - �������� �����
	double	dDdsFreq; // �������� ������� ��������� DDS (��)
	BRDCHAR sFirFile[256]; // ��� �����, ����������� ������������ FIR-�������
} FM416X250MDSP_DDCPARAM,*PFM416X250MDSP_DDCPARAM;

#pragma pack(pop)    

#endif // _CTRL_FM816x250M_H

//
// End of file
//