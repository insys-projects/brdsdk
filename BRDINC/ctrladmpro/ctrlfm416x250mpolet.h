/*
 ****************** File CtrlFm816x250m.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : FM816x250M section
 *
 * (C) InSys by Ekkore Aug, 2011
 *
 *
 ************************************************************
*/

#ifndef _CTRL_FM816x250M_H
 #define _CTRL_FM816x250M_H

#include "ctrladc.h"

#pragma pack(push, 1)    

#define BRD_ADCCHANCNT			4	// Number of ADC channels
#define BRD_DDCCHANCNT			2	// Number of DDC channels
#define BRD_DDCDECCNT			3	// Number of DDC R-channel per channel
#define BRD_DDCFIRCNT			64	// Number of FIR-filter points
#define	BRD_GAINCNT			6	// Number of gains

const int BRD_CLKDIVCNT	= 5; // Number of clock dividers = 9 (1, 2, 4, 8, 16)

// FM816x250M Clock sources
enum {
	BRDclks_ADC_DISABLED	= 0,	// 
	BRDclks_ADC_SUBGEN		= 1,	// SubModule Generator 0
	BRDclks_ADC_EXTCLK		= 0x81,	// External SubModule Clock
	BRDclks_ADC_CLK0_C2M	= 0x85,	// Base Unit Clock CLK0_C2M
	BRDclks_ADC_CLK1_C2M	= 0x86,	// Base Unit Clock CLK1_C2M
};

// FM816x250M start sources
enum {
	BRDsts_ADC_CHAN0	= 0,	// Start from channel 0
	BRDsts_ADC_CHAN1	= 1,	// Start from channel 1
	BRDsts_ADC_EXT		= 2,	// Start from START IN (external)
	BRDsts_ADC_PRG		= 3,	// Program start
};



// Numbers of Specific Command
typedef enum _FM816x250M_NUM_CMD
{
	FM816x250Mcmd_SETMU		= 64,
	FM816x250Mcmd_GETMU		= 65,
	FM816x250Mcmd_SUBSYNC	= 66
} FM816x250M_NUM_CMD;

typedef struct _FM816x250MSRV_MU {
	U32		size;					// sizeof(FM816x250MSRV_MU)
	U32		chanMask;				// маска выбранных каналов
	U32		syncMode;				// режим синхронизации (2 - Master, 1 - Single, 0 - Slave) всегда 1
	double	samplingRate;			// частота дискретизации (Гц): 10 МГц .. 125 МГц
	double	clockValue;				// значение частоты выбранного внешнего источника (Гц)
	U32		clockSrc;				// источник тактовой частоты (0 - выключен, 1 - генератор на субмодуле, 0x81 - внешний)
	double	range[BRD_ADCCHANCNT];		// шкала преобразования для каждого канала (Вольт): 4.0, 2.0, 1.0, 0.5, 0.25
	double	bias[BRD_ADCCHANCNT];		// смещение нуля для каждого канала (Вольт): -2*ШП .. +2*ШП
	U32		inpResist[BRD_ADCCHANCNT];	// входное сопротивление для каждого канала (0 - 1 МOм, 1 - 50 Oм)
	U32		dcCoupling[BRD_ADCCHANCNT];// открытость (1) / закрытость (0) входа по постоянной составляющей для каждого канала
	U32		format;					// 0 - 16 бит, 1 - 8 бит

	U32		startSrc;				// субмодульный источник старта: 0 - канал 0, 1 - канал 1, 3 - программный
	double	startLevel;				// уровень старта в вольтах (от -2.5 В до +2.5 В – при внешнем старте)
	double	clockLevel;				// порог компаратора внешней тактовой частоты (В): -2.5 .. +2.5
} FM816x250MSRV_MU, *PFM816x250MSRV_MU;

typedef struct _FM416X250MPOLET_DDCPARAM {
	U32		isDdcEnable;		// Разрешение DDC: 0-работает АЦП, 1-работает DDC
	U32		nDdcDecimation;		// Коэффициент децимации DDC: 16, 32, 64
	U32		nFirSize;
	double	adDdcFc[BRD_DDCCHANCNT*BRD_DDCDECCNT];	// Центральная частота для каналов DDC (Гц)
	U32		anDdcGain[BRD_DDCCHANCNT*BRD_DDCDECCNT];// Коэф. передачи для каналов DDC: 1-0xFFFF
	U32		anDdcFir[BRD_DDCCHANCNT*BRD_DDCDECCNT][BRD_DDCFIRCNT];// Коэф. FIR-фильтров
} FM416X250MPOLET_DDCPARAM,*PFM416X250MPOLET_DDCPARAM;

#pragma pack(pop)    

#endif // _CTRL_FM816x250M_H

//
// End of file
//