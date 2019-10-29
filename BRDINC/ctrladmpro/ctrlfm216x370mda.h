/*
 ****************** File CtrlFm216x370mda.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : FM216x370MDA section
 *
 * (C) InSys by Ekkore Feb, 2016
 *
 *
 ************************************************************
*/

#ifndef _CTRL_FM216x370MDA_H
 #define _CTRL_FM216x370MDA_H

#include "ctrladc.h"

#pragma pack(push, 1)    

#define BRD_CHANCNT			8	// Number of channels
#define	BRD_GAINCNT			1	// Number of gains

const int BRD_CLKDIVCNT	= 1; // Number of clock dividers = 1 (1)

// FM216x370MDA Clock sources
enum {
	BRDclks_ADC_DISABLED	= 0,	// 
	BRDclks_ADC_SUBGEN		= 1,	// SubModule Generator 0
	BRDclks_ADC_EXTCLK		= 0x81,	// External SubModule Clock
	BRDclks_ADC_BASEGEN		= 0x85,	// Base Module Generator

	BRDclks_DAC_DISABLED	= 0,	// 
	BRDclks_DAC_SUBGEN		= 1,	// SubModule Generator 0
	BRDclks_DAC_EXTCLK		= 0x81,	// External SubModule Clock
	BRDclks_DAC_BASEGEN		= 0x85,	// Base Module Generator
};

// FM216x370MDA start sources
enum {
	BRDsts_ADC_CHAN0	= 0,	// Start from channel 0
	BRDsts_ADC_EXT		= 2,	// Start from START IN (external)
	BRDsts_ADC_PRG		= 3,	// Program start
};



// Numbers of Specific Command
typedef enum _FM216X370MDA_NUM_CMD
{
	FM216X370MDAcmd_SETMU		= 64,
	FM216X370MDAcmd_GETMU		= 65,
	FM216X370MDAcmd_DAC_SETLMFCCALIBRATION = 80,
	FM216X370MDAcmd_DAC_GETLMFCCALIBRATION = 81,
} FM216X370MDA_NUM_CMD;

typedef struct _FM216X370MDA_MU {
	U32		size;					// sizeof(FM814X250MSRV_MU)
	U32		chanMask;				// маска выбранных каналов
	U32		syncMode;				// режим синхронизации (2 - Master, 1 - Single, 0 - Slave) всегда 1
	double	samplingRate;			// частота дискретизации (Гц): 10 МГц .. 125 МГц
	double	clockValue;				// значение частоты выбранного внешнего источника (Гц)
	U32		clockSrc;				// источник тактовой частоты (0 - выключен, 1 - генератор на субмодуле, 0x81 - внешний)
	double	range[BRD_CHANCNT];		// шкала преобразования для каждого канала (Вольт): 4.0, 2.0, 1.0, 0.5, 0.25
	double	bias[BRD_CHANCNT];		// смещение нуля для каждого канала (Вольт): -2*ШП .. +2*ШП
	U32		inpResist[BRD_CHANCNT];	// входное сопротивление для каждого канала (0 - 1 МOм, 1 - 50 Oм)
	U32		dcCoupling[BRD_CHANCNT];// открытость (1) / закрытость (0) входа по постоянной составляющей для каждого канала
	U32		format;					// 0 - 16 бит, 1 - 8 бит

	U32		startSrc;				// субмодульный источник старта: 0 - канал 0, 1 - канал 1, 3 - программный
	double	startLevel;				// уровень старта в вольтах (от -2.5 В до +2.5 В – при внешнем старте)
} FM216X370MDASRV_MU, *PFM216X370MDASRV_MU;

typedef struct  _FM216X370MDASRV_DacLmfcCalibration {
	S32		idx;			// не используется, должно быть 0
	S32		aux;			// не используется, должно быть 0
	S32		nPClockFactor;	// (только GET)
	S32		nPClockPerMF;	// (только GET)
	S32		nK;				// (только GET)
	S32		nCount;			// (только GET) Количество заполненных ячеек в aRegVal[]
	S32		aRegVal[32];	// GET: значения регистров 0x302, 0x303 для всех м/с AD9144
							// SET: aRegVal[0] значения для 0x304/0x305, [1] - для 0x306/0x307
} FM216X370MDASRV_DacLmfcCalibration, *PFM216X370MDASRV_DacLmfcCalibration;

#pragma pack(pop)    

#endif // _CTRL_FM216x370MDA_H

//
// End of file
//