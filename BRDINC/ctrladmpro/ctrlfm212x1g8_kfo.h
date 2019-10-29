/*
 ****************** File CtrlFm212x1g.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : FM212x1G section
 *
 * (C) InSys by Ekkore Aug, 2011-2012
 *
 *
 ************************************************************
*/

#ifndef _CTRL_FM212x1G_H
 #define _CTRL_FM212x1G_H

#include "ctrladc.h"

#pragma pack(push, 1)    

#define BRD_CHANCNT			2	// Number of channels

// FM212x1G Clock sources
enum {
	BRDclks_ADC_DISABLED	= 0,	// 
	BRDclks_ADC_SUBGEN		= 1,	// SubModule Internal Generator 
	BRDclks_ADC_SUBGEN_PLL	= 2,	// SubModule Internal Generator + PLL
	BRDclks_ADC_EXTCLK		= 0x81,	// External SubModule Clock
	BRDclks_ADC_EXTCLK_PLL	= 0x82,	// External SubModule Clock + PLL
};

// FM212x1G start sources
enum {
	BRDsts_ADC_CHAN0	= 0,	// Start from channel 0
	BRDsts_ADC_CHAN1	= 1,	// Start from channel 1
	BRDsts_ADC_EXT		= 2,	// Start from START IN (external)
	BRDsts_ADC_PRG		= 3,	// Program start
};



// Numbers of Specific Command
typedef enum _FM212x1G_NUM_CMD
{
	FM212x1Gcmd_SETMU		= 64,
	FM212x1Gcmd_GETMU		= 65,
	FM212x1Gcmd_GETEVENTCNT		= 66, // Get event counter
	FM212x1Gcmd_RESETEVENTCNT	= 67, // reset event counter
	FM212x1Gcmd_SETCDBLCLKEX	= 68,
	FM212x1Gcmd_GETCDBLCLKEX	= 69,
	FM212x1Gcmd_SPDREAD			= 70,
	FM212x1Gcmd_SPDWRITE		= 71,
	FM212x1Gcmd_SETKFOFIR		= 72,
	FM212x1Gcmd_GETKFOFIR		= 73,
	FM212x1Gcmd_SETHDRDATA1		= 74,
	FM212x1Gcmd_SETHDRDATA2		= 75,
} FM212x1G_NUM_CMD;

typedef struct _FM212x1GSRV_DBLCLK {
	U32		size;					// sizeof(FM212x1GSRV_DBLCLK)
	U32		isDblClk;				// 1-вкл/0-выкл режим удвоения частоты
	U32		inpSrc;					// источник сигнала для обоих АЦП: 0-IN0, 1-IN1
	U32		valRange;				// диапазон изменеия корректирующих значений: 0 -100%..+100%; 1 - как указано ниже 
	double	inp0Range;				// корректирующее значение шкалы преобраз. канала 0 (%): -24.0 .. +24.0
	double	inp1Range;				// корректирующее значение шкалы преобраз. канала 1 (%): -24.0 .. +24.0
	double	inp0Bias;				// корректирующее значение смещения нуля канала 0 (%): -5.6 .. +5.6
	double	inp1Bias;				// корректирующее значение смещения нуля канала 0 (%): -5.6 .. +5.6
	U32		rsrv[16];
} FM212x1GSRV_DBLCLK, *PFM212x1GSRV_DBLCLK;

typedef struct _FM212x1GSRV_MU {
	U32		size;					// sizeof(FM212x1GSRV_MU)
	S32		subType;				// тип субмодуля: 0 - стандарт, 1 - не WB, 2 - WB
	U32		master;					// режим синхронизации (2 - Master, 1 - Single, 0 - Slave)
	U32		chanMask;				// маска выбранных каналов
	U32		clockSrc;				// источник тактовой частоты: 0-выкл., 1-внутренний, 2-внутренний+PLL, 4-от базмодуля, 0x81-внешний, 0x82-внешний+PLL 
	double	samplingRate;			// частота дискретизации (Гц) (возможные значения делителя = 1, 2, 4, 8, 16, 32, 64, 128, 256)
	double	clockValue;				// значение частотs выбранного источника тактирования (Гц) (для источников 2, 4, 0x81, 0x82)

	double	gaindb[BRD_CHANCNT];	// коэф-ты передачи для каждого канала (дБ):-31.5 .. 0.0
	double	bias[BRD_CHANCNT];		// смещение нуля для каждого канала (%): -100 .. +100
	U32		inpResist[BRD_CHANCNT];	// входное сопротивление для каждого канала (0 - 1 МOм, 1 - 50 Oм)
	U32		dcCoupling[BRD_CHANCNT];// открытость (1) / закрытость (0) входа по постоянной составляющей для каждого канала
	U32		format;					// 0 - 16 бит, 1 - 8 бит

	BRD_AdcStartMode	adcStMode;	// режим стартовой синхронизации
	U32		stResist;				// входное сопротивление для внешнего старта (0 - 2.5 кOм, 1 - 50 Oм)

	U32		pretrigMode;			// включение режима претриггера: 0-выкл., 1-простой, 2-гарантированный, 3-от SDRAM
	U32		pretrigSamples;			// число отсчётов претриггера

	U32		cnt0Value;				// значение счетчика начальной задержки CNT0
	U32		cnt1Value;				// значение счетчика принимаемых данных CNT1
	U32		cnt2Value;				// значение счетчика пропущенных данных CNT2
	U32		cnt0Enable;				// разрешение счетчика начальной задержки CNT0
	U32		cnt1Enable;				// разрешение счетчика принимаемых данных CNT1
	U32		cnt2Enable;				// разрешение счетчика пропущенных данных CNT2
	
	U32		titleEnable;			// Разрешение заголовка в кадрах: 0 - не разрешен, 1 - разрешен
	U32		titleSize;				// Размер заголовка в 32-разрядных словах  
	U32		titleData;				// Данные заголовка: целое 32-разрядное число без знака


	FM212x1GSRV_DBLCLK	dblClk;		// управление режимом удвоения частоты дискретизации
} FM212x1GSRV_MU, *PFM212x1GSRV_MU;

typedef struct  {
	U32 spdType;
	U32 regAdr;
	U32 regVal;
} FM212x1GSRV_SPDREAD, *PFM212x1GSRV_SPDREAD,
FM212x1GSRV_SPDWRITE, *PFM212x1GSRV_SPDWRITE;

typedef struct  {
	U32 decim0;			// 1, 2, 4, 8, 16
	U32 decim2;			// 1, 2, 4, 8, 16
	REAL32 gain;		// 0.5 - 2.5
} FM212x1GSRV_KFOFIR, *PFM212x1GSRV_KFOFIR;

#pragma pack(pop)    

#endif // _CTRL_FM212x1G_H

//
// End of file
//