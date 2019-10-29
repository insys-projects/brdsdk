/*
 ****************** File ctrldac214x2g5d.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : DAC214X2G5D section
 *
 * (C) InSys, Ekkore, Mar 2014
 *
 *
 ************************************************************
*/

#ifndef _CTRL_DAC214X2G5D_H
#define _CTRL_DAC214X2G5D_H

#include "ctrldac.h"

#pragma pack(push, 1)    

const int BRD_CLKDIVCNT		= 5; // Number of clock dividers = 5 (1, 2, 4, 8, 16)

// DAC214X2G5D Clock sources
enum {
	BRDclks_DAC_DISABLED	= 0,	// 
	BRDclks_DAC_SUBGEN		= 1,	// SubModule Internal Generator 
	BRDclks_DAC_SUBGEN_PLL	= 2,	// SubModule Internal Generator + PLL
	BRDclks_DAC_EXTCLK		= 0x81,	// External SubModule Clock
	BRDclks_DAC_EXTCLK_PLL	= 0x82,	// External SubModule Clock + PLL
};

// DAC214X2G5D Reference sources
enum {
	BRDclks_DAC_INTREFPLL	= 0,	// Internal reference
	BRDclks_DAC_EXTREFPLL	= 1,	// External reference
	BRDclks_DAC_REFNOTSET	= 0xff,	// Referens not set
};

// Numbers of Specific Command
typedef enum 
{
	DAC214X2G5Dcmd_SETMU	= 64,
	DAC214X2G5Dcmd_GETMU	= 65,
	DAC214X2G5Dcmd_SETINP	= 66,
	DAC214X2G5Dcmd_GETINP	= 67,
	DAC214X2G5Dcmd_SETMODULATOR	= 68,

} DAC214X2G5D_NUM_CMD;

// Структуры данных для специфических команд
typedef struct
{
	U32		chanMask;
	S32		att;		// Аттенюатор: 1-включен, 0-выключен, -1 - поле не изменяется
	S32		ampLo;		// Выходной усилитель низкого напряжения: то же самое
	S32		ampHi;		// Выходной усилитель низкого напряжения: то же самое
	S32		flt0;		// Фильтрация 120 МГц: то же самое
	S32		flt1;		// Фильтрация 30 МГц: то же самое
} BRD_DacSetInp;

typedef struct _BRD_DacIntReg
{
	U32		addr;			// register address into DAC chip for programming
	U32		data;			// data for writing / getting
} BRD_DacIntReg, *PBRD_DacIntReg;

typedef struct
{
	S32		mode0;			// режим модулятора: -1-не изменять, 0-выкл., 1-низк. усиление, 2-высок. усиление
	S32		mode1;			// резерв
	REAL32	bias;			// смещение нуля модулятора
} BRD_DacSetModulator, *PBRD_DacSetModulator;

#pragma pack(pop)    

#endif // _CTRL_DAC214X2G5D_H

//
// End of file
//