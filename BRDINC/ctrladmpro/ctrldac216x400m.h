/*
 ****************** File CtrlDac216x400m.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : DAC216x400M section
 *
 * (C) InSys by Sklyarov A., Dorokhin A. Oct 2007
 *
 *
 ************************************************************
*/

#ifndef _CTRL_DAC216X400M_H
 #define _CTRL_DAC216X400M_H

#include "ctrldac.h"

#pragma pack(push, 1)    

const int BRD_CLKDIVCNT		= 5; // Number of clock dividers = 5 (1, 2, 4, 8, 16)

// DAC216x400M Clock sources
enum {
	BRDclks_DAC_INTCLK		= 0,	// Internal SubModule clock
	BRDclks_DAC_EXTCLK		= 1,	// External SubModule clock
	BRDclks_DAC_CLKNOTSET	= 0xff,	// Clock not set
};
// DAC216x400M Reference sources
enum {
	BRDclks_DAC_INTREFPLL	= 0,	// Internal reference
	BRDclks_DAC_EXTREFPLL	= 1,	// External reference
	BRDclks_DAC_REFNOTSET	= 0xff,	// Referens not set
};

// Numbers of Specific Command
typedef enum _DAC216X400M_NUM_CMD
{
	DAC216X400Mcmd_SETINTREG	= 64, // Set DAC internal control registers (PBRD_DacIntReg)
	DAC216X400Mcmd_SETANALOGQM	= 65,
	DAC216X400Mcmd_SETCODEGRAM	= 66,
} DAC216X400M_NUM_CMD;

// for set and get DAC internal control registers function
typedef struct _BRD_DacIntReg
{
	U32		addr;			// register address into DAC chip for programming
	U32		data;			// data for writing / getting
} BRD_DacIntReg, *PBRD_DacIntReg;

typedef struct  
{
	U32		isCODEGRAM;   // 0 - непрерывный режим работы ЦАП, 1 - режим кодограммы
	U32		isCYCLE;      //0-one codegram, 1-cycle  codegram
	U32		nLCYCLE;      // Длина цикла (в тактах MCLOCK/2) 
	U32		nNCYCLES;     // Число циклов
	U32		nSSTB0;       // Старт STB0
	U32		nESTB0;       // Конец STB0
	U32		nSSTB1;       // Старт STB1
	U32		nESTB1;       // Конец STB1
	U32		nSSTB2;       // Старт STB2
	U32		nESTB2;       // Конец STB2
	U32		nSSTBDAC;     // Старт STB_DAC
	U32		nESTBDAC;     // Конец STB_DAC
	U32		nSSTBRI;      // Старт STROB_RI
	U32		nESTBRI;      // Конец STROB_RI
	U32		nSSTBADC;     // Задержка STROB_ADC
	U32		nESTBADC;     // Длина STROB_ADC

	U32		nBURST_CNT;			// число кодограмм после которого формируется ретрансмит для памяти
	U32		isBURST_DISABLE;	// 1 - запрет BURST_CNT, ретрансмит не формируется
	U32		isPIO_DAC_ENABLE;	// 1 - вывод кодограммы на разъём PIOX
} DAC216X400M_SetCodegram, *PDAC216X400M_SetCodegram;

#pragma pack(pop)    

#endif // _CTRL_DAC216X400M_H

//
// End of file
//