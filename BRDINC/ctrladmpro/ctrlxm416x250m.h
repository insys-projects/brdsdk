/*
 ****************** File ctrlxm416x250m.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : XM416x250M section
 *
 * (C) InSys by Ekkore Aug, 2011
 *
 *
 ************************************************************
*/

#ifndef _CTRL_XM416x250M_H
 #define _CTRL_XM416x250M_H

#include "ctrladc.h"

#pragma pack(push, 1)    

#define BRD_CHANCNT			8	// Number of channels
#define	BRD_GAINCNT			6	// Number of gains

const int BRD_CLKDIVCNT	= 5; // Number of clock dividers = 9 (1, 2, 4, 8, 16)

// XM416x250M Clock sources
enum {
	BRDclks_ADC_DISABLED	= 0,	// 
	BRDclks_ADC_SUBGEN		= 1,	// SubModule Generator 0
	BRDclks_ADC_EXTCLK		= 0x81,	// External SubModule Clock
	BRDclks_ADC_CLK0_C2M	= 0x85,	// Base Unit Clock CLK0_C2M
	BRDclks_ADC_CLK1_C2M	= 0x86,	// Base Unit Clock CLK1_C2M
};

// XM416x250M start sources
enum {
	BRDsts_ADC_CHAN0	= 0,	// Start from channel 0
	BRDsts_ADC_CHAN1	= 1,	// Start from channel 1
	BRDsts_ADC_EXT		= 2,	// Start from START IN (external)
	BRDsts_ADC_PRG		= 3,	// Program start
};



// Numbers of Specific Command
typedef enum _XM416x250M_NUM_CMD
{
	XM416x250Mcmd_SETMU		= 64,
	XM416x250Mcmd_GETMU		= 65,
	XM416x250Mcmd_SUBSYNC	= 66
} XM416x250M_NUM_CMD;

typedef struct _XM416x250MSRV_MU {
	U32		size;					// sizeof(XM416x250MSRV_MU)
	U32		chanMask;				// ����� ��������� �������
	U32		syncMode;				// ����� ������������� (2 - Master, 1 - Single, 0 - Slave) ������ 1
	double	samplingRate;			// ������� ������������� (��): 10 ��� .. 125 ���
	double	clockValue;				// �������� ������� ���������� �������� ��������� (��)
	U32		clockSrc;				// �������� �������� ������� (0 - ��������, 1 - ��������� �� ���������, 0x81 - �������)
	double	range[BRD_CHANCNT];		// ����� �������������� ��� ������� ������ (�����): 4.0, 2.0, 1.0, 0.5, 0.25
	double	bias[BRD_CHANCNT];		// �������� ���� ��� ������� ������ (�����): -2*�� .. +2*��
	U32		inpResist[BRD_CHANCNT];	// ������� ������������� ��� ������� ������ (0 - 1 �O�, 1 - 50 O�)
	U32		dcCoupling[BRD_CHANCNT];// ���������� (1) / ���������� (0) ����� �� ���������� ������������ ��� ������� ������
	U32		format;					// 0 - 16 ���, 1 - 8 ���

	U32		startSrc;				// ������������ �������� ������: 0 - ����� 0, 1 - ����� 1, 3 - �����������
	double	startLevel;				// ������� ������ � ������� (�� -2.5 � �� +2.5 � � ��� ������� ������)
	double	clockLevel;				// ����� ����������� ������� �������� ������� (�): -2.5 .. +2.5
} XM416x250MSRV_MU, *PXM416x250MSRV_MU;

#pragma pack(pop)    

#endif // _CTRL_XM416x250M_H

//
// End of file
//