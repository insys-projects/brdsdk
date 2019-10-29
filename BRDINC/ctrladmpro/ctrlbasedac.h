/*
 ****************** File CtrlBaseDac.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : BASEDAC section
 *
 * (C) InSys by Dorokhin Andrey Oct, 2005
 *
 *
 ************************************************************
*/

#ifndef _CTRL_BASEDAC_H
 #define _CTRL_BASEDAC_H

#include "ctrldac.h"

#pragma pack(push, 1)    

// getting configuration
typedef struct _BRD_BaseDacCfg
{
	U32		Bits;						// �����������
	U32		Encoding;					// ��� ��������� (0 - ������, 1 - �������-��������������, 2 - ��� ����)
	U32		MinRate;					// ����������� ������� �������������
	U32		MaxRate;					// ������������ ������� �������������
	U32		OutRange;					// �������� ��������
	U32		FifoSize;					// ������ FIFO ��� (� ������)
	U32		MaxChan;					// ������������ ����� �������
	U32		FifoType;					// ��� FIFO: 0 - ����������, 1 - �������
} BRD_BaseDacCfg, *PBRD_BaseDacCfg;

#pragma pack(pop)    

#endif // _CTRL_BASEDAC_H

//
// End of file
//