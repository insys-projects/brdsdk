/*
 ****************** File CtrlFm814x250m.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : FM814x250M section
 *
 * (C) InSys by Ekkore Aug, 2011
 *
 *
 ************************************************************
*/

#ifndef _CTRL_FM814x250M_H
 #define _CTRL_FM814x250M_H

#include "ctrladc.h"

#pragma pack(push, 1)    

#define BRD_CHANCNT			8	// Number of channels
#define	BRD_GAINCNT			1	// Number of gains

const int BRD_CLKDIVCNT	= 1; // Number of clock dividers = 1 (1)

// FM814x250M Clock sources
enum {
	BRDclks_ADC_DISABLED	= 0,	// 
	BRDclks_ADC_SUBGEN		= 1,	// SubModule Generator 0
	BRDclks_ADC_EXTCLK		= 0x81,	// External SubModule Clock
	BRDclks_ADC_BASEGEN		= 0x85,	// Base Module Generator
};

// FM814x250M start sources
enum {
	BRDsts_ADC_CHAN0	= 0,	// Start from channel 0
	BRDsts_ADC_EXT		= 2,	// Start from START IN (external)
	BRDsts_ADC_PRG		= 3,	// Program start
};



// Numbers of Specific Command
typedef enum _FM814X250M_NUM_CMD
{
	FM814X250Mcmd_SETMU		= 64,
	FM814X250Mcmd_GETMU		= 65,
} FM814X250M_NUM_CMD;

typedef struct _FM814X250MSRV_MU {
	U32		size;					// sizeof(FM814X250MSRV_MU)
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
} FM814X250MSRV_MU, *PFM814X250MSRV_MU;

#pragma pack(pop)    

#endif // _CTRL_FM814x250M_H

//
// End of file
//