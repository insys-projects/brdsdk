/*
 ****************** File CtrlDac216x400mt.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : DAC216x400MT section
 *
 * (C) InSys by Sklyarov A., Dorokhin A. Oct 2007
 * Ekkore 2013
 *
 *
 ************************************************************
*/

#ifndef _CTRL_DAC216X400MT_H
 #define _CTRL_DAC216X400MT_H

#include "ctrldac.h"

#pragma pack(push, 1)    

const int BRD_CLKDIVCNT		= 5; // Number of clock dividers = 5 (1, 2, 4, 8, 16)

// DAC216x400MT Clock sources
enum {
	BRDclks_DAC_DISABLED	= 0x0,	// Clock not set
	BRDclks_DAC_INTCLK		= 0x1,	// Internal SubModule clock (Si570/571)
	BRDclks_DAC_INTCLKPLL	= 0x2,	// Internal SubModule clock + PLL
	BRDclks_DAC_EXTCLKPLL	= 0x3,	// External SubModule clock + PLL 
	BRDclks_DAC_EXTCLK		= 0x81,	// External SubModule clock (CLKIN) 
};
// DAC216x400MT Reference sources
enum {
	BRDclks_DAC_INTREFPLL	= 0,	// Internal reference
	BRDclks_DAC_EXTREFPLL	= 1,	// External reference
	BRDclks_DAC_REFNOTSET	= 0xff,	// Referens not set
};

// Numbers of Specific Command
typedef enum _DAC216X400MT_NUM_CMD
{
	DAC216X400MTcmd_SETMU	= 64,
	DAC216X400MTcmd_GETMU	= 65,
	DAC216X400MTcmd_SETINP	= 66,
	DAC216X400MTcmd_GETINP	= 67,

	DAC216X400MTcmd_SETINTREG	= 164, // Set DAC internal control registers (PBRD_DacIntReg)
	DAC216X400MTcmd_SETANALOGQM	= 165,
} DAC216X400MT_NUM_CMD;

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

#pragma pack(pop)    

#endif // _CTRL_DAC216X400MT_H

//
// End of file
//