/*
 ****************** File CtrlAdc216x100m.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : ADC216x100M section
 *
 * (C) InSys by Dorokhin Andrey Jan, 2007
 *
 *
 ************************************************************
*/

#ifndef _CTRL_ADC216x100M_H
 #define _CTRL_ADC216x100M_H

#include "ctrladc.h"

#pragma pack(push, 1)    

#define BRD_CHANCNT			2	// Number of channels

const int BRD_CLKDIVCNT	= 5; // Number of clock dividers = 5 (1, 2, 4, 8, 16)

// ADC216x100M Clock sources
enum {
	BRDclks_ADC_DISABLED	= 0,	// 
	BRDclks_ADC_SUBGEN1		= 1,	// SubModule Generator 1
	BRDclks_ADC_SUBGEN2		= 2,	// SubModule Generator 2
	BRDclks_ADC_SUBGEN3		= 3,	// SubModule Generator 3
	BRDclks_ADC_EXTCLK		= 0x81,	// External SubModule Clock (from EXT CLK IN)
	BRDclks_ADC_MASTERCLK	= 0x82,	// External Clock from MASTER card (synthesizer - from CLK IN)
};

const int BRD_GAINCNT	= 8; // Number of gains

// input gain
const double BRD_Gain[BRD_GAINCNT] =
{
	0.125,	// 0x00
	0.188,	// 0x01
	0.250,	// 0x02
	0.376,	// 0x03
	2.500,	// 0x04
	3.760,	// 0x05
	5.000,	// 0x06
	7.576	// 0x07
};

// input range (Volt)
const double BRD_Range[BRD_GAINCNT] =
{
	4.000,	// 0x00
	2.660,	// 0x01
	2.000,	// 0x02
	1.330,	// 0x03
	0.200,	// 0x04
	0.133,	// 0x05
	0.100,	// 0x06
	0.066	// 0x07
};

// Numbers of Specific Command
typedef enum _ADC216X100M_NUM_CMD
{
	ADC216X100Mcmd_SETDITHER	= 64,
	ADC216X100Mcmd_GETDITHER	= 65,
	ADC216X100Mcmd_SETMU		= 66,
} ADC216X100M_NUM_CMD;

typedef struct _ADC216X100MSRV_MU {
	U32		size;					// sizeof(ADC216X100MSRV_MU)
	U32		chanMask;				// маска выбранных каналов
	U32		syncMode;				// режим синхронизации (2 - Master, 1 - Single, 0 - Slave)
	double	samplingRate;			// частота дискретизации (Гц) (возможные значения делителя = 1, 2, 4, 8, 16)
	double	clockValue;				// значение частота выбранного внешнего источника (Гц)
	U32		clockSrc;				// источник тактовой частоты (0 - выключен, 1,2,3 - генератор 1,2,3 на субмодуле, 0x81 - внешний)
	double	range[BRD_CHANCNT];		// входной диапазон для каждого канала (4.0 Вольт, 2.66 Вольт, 2.0 Вольт, 1.33 Вольт, 0.2 Вольт, 0.133 Вольт, 0.1 Вольт, 0.066 Вольт)
	double	bias[BRD_CHANCNT];		// смещение нуля для каждого канала
	U32		resist[BRD_CHANCNT];	// входное сопротивление для каждого канала (0 - 1 kOм, 1 - 50 Oм)
	U32		dcCoupling[BRD_CHANCNT];// открытость (1) / закрытость (0) входа по постоянной составляющей для каждого канала
	U32		format;					// 0 - 16 бит, 1 - 8 бит
	U32		pretrigEnable;			// включение режима претриггера
	U32		pretrigExernal;			// включение режима внешнего претриггера
	U32		pretrigAssur;			// включение режима гарантированного претриггера
	U32		pretrigSamples;			// число отсчётов претриггера
	U32		startDelayCnt;			// счетчик начальной задержки
	U32		daqCnt;					// счетчик принимаемых данных
	U32		skipCnt;				// счетчик пропущенных данных
	U32		cnt0Enable;				// разрешение счетчика начальной задержки
	U32		cnt1Enable;				// разрешение счетчика принимаемых данных
	U32		cnt2Enable;				// разрешение счетчика пропущенных данных
	U32		baseStartSrc;			// базовый источник старта: 0 - программный, 2 - SDX (компаратор 1), 10 - от DDS (only for AMBPEX8)
	U32		baseStartInv;			// 1 - инверсия базового источника старта
	U32		baseStopSrc;			// базовый источник останова
	U32		baseStopInv;			// 1 - инверсия базового источника останова
	U32		startStopMode;			// 1 - режим триггерного старта-останова, 0 - режим потенциального старта-останова
	U32		reStart;				// 1 - режим сбора с автоматическим перезапуском
	double	thrClk;					// порог тактового сигнала (от -2.5 вольт до 2.5 вольт)
	U32		dither;					// 1 - включение розового шума
} ADC216X100MSRV_MU, *PADC216X100MSRV_MU;

#pragma pack(pop)    

#endif // _CTRL_ADC216x100M_H

//
// End of file
//