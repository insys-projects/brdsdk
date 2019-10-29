/*
 ****************** File ctrldac416x1g5d.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : DAC416X1G5D section
 *
 * (C) InSys, Ekkore, Mar 2014
 *
 *
 ************************************************************
*/

#ifndef _CTRL_DAC416X1G5D_H
#define _CTRL_DAC416X1G5D_H

#include "ctrldac.h"

#pragma pack(push, 1)    

const int BRD_CLKDIVCNT		= 5; // Number of clock dividers = 5 (1, 2, 4, 8, 16)

// DAC416X1G5D Clock sources
enum {
	BRDclks_DAC_DISABLED	= 0x0,	// Clock not set
	BRDclks_DAC_SUBGEN		= 0x1,	// Internal SubModule generator (Si570/571)
	BRDclks_DAC_BASEGEN		= 0x2,	// Internal Base Module generator
	BRDclks_DAC_EXTCLK		= 0x81,	// External SubModule clock (CLKIN) 
};
// DAC416X1G5D Reference sources
enum {
	BRDclks_DAC_INTREFPLL	= 0,	// Internal reference
	BRDclks_DAC_EXTREFPLL	= 1,	// External reference
	BRDclks_DAC_REFNOTSET	= 0xff,	// Referens not set
};

// Numbers of Specific Command
typedef enum 
{
	DAC416X1G5Dcmd_SETMU	= 64,
	DAC416X1G5Dcmd_GETMU	= 65,
	DAC416X1G5Dcmd_SETINP	= 66,
	DAC416X1G5Dcmd_GETINP	= 67,
	DAC416X1G5Dcmd_SETMODULATOR	= 68,

} DAC416X1G5D_NUM_CMD;

// ��������� ������ ��� ������������� ������
typedef struct
{
	U32		chanMask;
	S32		att;		// ����������: 1-�������, 0-��������, -1 - ���� �� ����������
	S32		ampLo;		// �������� ��������� ������� ����������: �� �� �����
	S32		ampHi;		// �������� ��������� ������� ����������: �� �� �����
	S32		flt0;		// ���������� 120 ���: �� �� �����
	S32		flt1;		// ���������� 30 ���: �� �� �����
} BRD_DacSetInp;

typedef struct _BRD_DacIntReg
{
	U32		addr;			// register address into DAC chip for programming
	U32		data;			// data for writing / getting
} BRD_DacIntReg, *PBRD_DacIntReg;

typedef struct
{
	S32		mode0;			// ����� ���������� 0: -1-�� ��������, 0-����., 1-����. ��������, 2-�����. ��������
	S32		mode1;			// ����� ���������� 1: -1-�� ��������, 0-����., 1-����. ��������, 2-�����. ��������
	REAL32	bias;			// �������� ���� ����������
} BRD_DacSetModulator, *PBRD_DacSetModulator;

#pragma pack(pop)    

#endif // _CTRL_DAC416X1G5D_H

//
// End of file
//