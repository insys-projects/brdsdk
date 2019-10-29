/*
 ****************** File CtrlFm412x500m.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : FM412x500M section
 *
 * (C) InSys by Ekkore Feb 2012
 *
 *
 ************************************************************
*/

#ifndef _CTRL_FM412x500M_H
 #define _CTRL_FM412x500M_H

#include "ctrladc.h"

#pragma pack(push, 1)    

#define BRD_CHANCNT			4	// Number of channels

// FM412x500M Clock sources
enum {
	BRDclks_ADC_DISABLED	= 0,	// 
	BRDclks_ADC_SUBGEN		= 1,	// SubModule Internal Generator 
	BRDclks_ADC_SUBGEN_PLL	= 2,	// SubModule Internal Generator + PLL
	BRDclks_ADC_BASEGEN		= 0x85,	// Base Module Generator
	BRDclks_ADC_EXTCLK		= 0x81,	// External SubModule Clock
	BRDclks_ADC_EXTCLK_PLL	= 0x82,	// External SubModule Clock + PLL
};

// FM412x500M start sources
enum {
	BRDsts_ADC_EXT		= 2,	// Start from START IN (external)
	BRDsts_ADC_PRG		= 3,	// Program start
};



// Numbers of Specific Command
typedef enum _FM412X500M_NUM_CMD
{
	FM412X500Mcmd_SETMU		= 64,
	FM412X500Mcmd_GETMU		= 65,
	FM412X500Mcmd_GETEVENTCNT	= 66, // Get event counter
	FM412X500Mcmd_RESETEVENTCNT	= 67, // reset event counter
	FM412X500Mcmd_SUBSYNC	= 68
} FM412X500M_NUM_CMD;

typedef struct _FM412X500MSRV_MU {
	U32		size;					// sizeof(ADC214X200MSRV_MU)
	S32		subType;				// ��� ���������: 0 - ������
	U32		chanMask;				// ����� ��������� �������
	U32		syncMode;				// ����� ������������� (2 - Master, 1 - Single, 0 - Slave)
	double	samplingRate;			// ������� ������������� (��)
	double	clockValue;			// �������� ������� ���������� ��������� ������������ (��) (��� ���������� 2, 4, 0x81, 0x82)
	U32		clockSrc;				// �������� �������� �������: 0-����., 1-����������, 2-����������+PLL, 4-�� ���������, 0x81-�������, 0x82-�������+PLL
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

	U32		baseStartSrc;			// ������� �������� ������: 0 - �����������, 7 - �� ���������, 10 - �� DDS �������� ������
	U32		baseStartInv;			// 1 - �������� �������� ��������� ������
	U32		baseStopSrc;			// ������� �������� ��������
	U32		baseStopInv;			// 1 - �������� �������� ��������� ��������
	U32		startStopMode;			// 1 - ����� ����������� ������-��������, 0 - ����� �������������� ������-��������
	U32		reStart;				// 1 - ����� ����� � �������������� ������������
	U32		startSrc;				// ������������ �������� ������: 0 - ����� 0, 1 - ����� 1, 2 - �������, 3 - �����������
	U32		invStart;				// 1 - �������� ������������� ��������� ������
	double	startLevel;				// ������� ������ � ������� (�� -2.5 � �� +2.5 � � ��� ������� ������)
	U32		startResist;			// ��.������������� ��� ������� �������� ������: 0 - 2.5 ���, 1 - 50 ��
} FM412X500MSRV_MU, *PFM412X500MSRV_MU;

#pragma pack(pop)    

#endif // _CTRL_FM412x500M_H

//
// End of file
//