/*
 ****************** File CtrlFm212x1g.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : FM212x1G section
 *
 * (C) InSys by Ekkore Aug, 2011-2012
 *
 *
 ************************************************************
*/

#ifndef _CTRL_FM212x1G_H
 #define _CTRL_FM212x1G_H

#include "ctrladc.h"

#pragma pack(push, 1)    

#define BRD_CHANCNT			2	// Number of channels

// FM212x1G Clock sources
enum {
	BRDclks_ADC_DISABLED	= 0,	// 
	BRDclks_ADC_SUBGEN		= 1,	// SubModule Internal Generator 
	BRDclks_ADC_SUBGEN_PLL	= 2,	// SubModule Internal Generator + PLL
	BRDclks_ADC_EXTCLK		= 0x81,	// External SubModule Clock
	BRDclks_ADC_EXTCLK_PLL	= 0x82,	// External SubModule Clock + PLL
};

// FM212x1G start sources
enum {
	BRDsts_ADC_CHAN0	= 0,	// Start from channel 0
	BRDsts_ADC_CHAN1	= 1,	// Start from channel 1
	BRDsts_ADC_EXT		= 2,	// Start from START IN (external)
	BRDsts_ADC_PRG		= 3,	// Program start
};



// Numbers of Specific Command
typedef enum _FM212x1G_NUM_CMD
{
	FM212x1Gcmd_SETMU		= 64,
	FM212x1Gcmd_GETMU		= 65,
	FM212x1Gcmd_GETEVENTCNT		= 66, // Get event counter
	FM212x1Gcmd_RESETEVENTCNT	= 67, // reset event counter
	FM212x1Gcmd_SETCDBLCLKEX	= 68,
	FM212x1Gcmd_GETCDBLCLKEX	= 69,
	FM212x1Gcmd_SPDREAD			= 70,
	FM212x1Gcmd_SPDWRITE		= 71,
	FM212x1Gcmd_SETKFOFIR		= 72,
	FM212x1Gcmd_GETKFOFIR		= 73,
	FM212x1Gcmd_SETHDRDATA1		= 74,
	FM212x1Gcmd_SETHDRDATA2		= 75,
} FM212x1G_NUM_CMD;

typedef struct _FM212x1GSRV_DBLCLK {
	U32		size;					// sizeof(FM212x1GSRV_DBLCLK)
	U32		isDblClk;				// 1-���/0-���� ����� �������� �������
	U32		inpSrc;					// �������� ������� ��� ����� ���: 0-IN0, 1-IN1
	U32		valRange;				// �������� �������� �������������� ��������: 0 -100%..+100%; 1 - ��� ������� ���� 
	double	inp0Range;				// �������������� �������� ����� ��������. ������ 0 (%): -24.0 .. +24.0
	double	inp1Range;				// �������������� �������� ����� ��������. ������ 1 (%): -24.0 .. +24.0
	double	inp0Bias;				// �������������� �������� �������� ���� ������ 0 (%): -5.6 .. +5.6
	double	inp1Bias;				// �������������� �������� �������� ���� ������ 0 (%): -5.6 .. +5.6
	U32		rsrv[16];
} FM212x1GSRV_DBLCLK, *PFM212x1GSRV_DBLCLK;

typedef struct _FM212x1GSRV_MU {
	U32		size;					// sizeof(FM212x1GSRV_MU)
	S32		subType;				// ��� ���������: 0 - ��������, 1 - �� WB, 2 - WB
	U32		master;					// ����� ������������� (2 - Master, 1 - Single, 0 - Slave)
	U32		chanMask;				// ����� ��������� �������
	U32		clockSrc;				// �������� �������� �������: 0-����., 1-����������, 2-����������+PLL, 4-�� ���������, 0x81-�������, 0x82-�������+PLL 
	double	samplingRate;			// ������� ������������� (��) (��������� �������� �������� = 1, 2, 4, 8, 16, 32, 64, 128, 256)
	double	clockValue;				// �������� ������s ���������� ��������� ������������ (��) (��� ���������� 2, 4, 0x81, 0x82)

	double	gaindb[BRD_CHANCNT];	// ����-�� �������� ��� ������� ������ (��):-31.5 .. 0.0
	double	bias[BRD_CHANCNT];		// �������� ���� ��� ������� ������ (%): -100 .. +100
	U32		inpResist[BRD_CHANCNT];	// ������� ������������� ��� ������� ������ (0 - 1 �O�, 1 - 50 O�)
	U32		dcCoupling[BRD_CHANCNT];// ���������� (1) / ���������� (0) ����� �� ���������� ������������ ��� ������� ������
	U32		format;					// 0 - 16 ���, 1 - 8 ���

	BRD_AdcStartMode	adcStMode;	// ����� ��������� �������������
	U32		stResist;				// ������� ������������� ��� �������� ������ (0 - 2.5 �O�, 1 - 50 O�)

	U32		pretrigMode;			// ��������� ������ �����������: 0-����., 1-�������, 2-���������������, 3-�� SDRAM
	U32		pretrigSamples;			// ����� �������� �����������

	U32		cnt0Value;				// �������� �������� ��������� �������� CNT0
	U32		cnt1Value;				// �������� �������� ����������� ������ CNT1
	U32		cnt2Value;				// �������� �������� ����������� ������ CNT2
	U32		cnt0Enable;				// ���������� �������� ��������� �������� CNT0
	U32		cnt1Enable;				// ���������� �������� ����������� ������ CNT1
	U32		cnt2Enable;				// ���������� �������� ����������� ������ CNT2
	
	U32		titleEnable;			// ���������� ��������� � ������: 0 - �� ��������, 1 - ��������
	U32		titleSize;				// ������ ��������� � 32-��������� ������  
	U32		titleData;				// ������ ���������: ����� 32-��������� ����� ��� �����


	FM212x1GSRV_DBLCLK	dblClk;		// ���������� ������� �������� ������� �������������
} FM212x1GSRV_MU, *PFM212x1GSRV_MU;

typedef struct  {
	U32 spdType;
	U32 regAdr;
	U32 regVal;
} FM212x1GSRV_SPDREAD, *PFM212x1GSRV_SPDREAD,
FM212x1GSRV_SPDWRITE, *PFM212x1GSRV_SPDWRITE;

typedef struct  {
	U32 decim0;			// 1, 2, 4, 8, 16
	U32 decim2;			// 1, 2, 4, 8, 16
	REAL32 gain;		// 0.5 - 2.5
} FM212x1GSRV_KFOFIR, *PFM212x1GSRV_KFOFIR;

#pragma pack(pop)    

#endif // _CTRL_FM212x1G_H

//
// End of file
//