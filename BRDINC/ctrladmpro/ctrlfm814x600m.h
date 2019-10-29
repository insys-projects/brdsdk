/*
 ****************** File CtrlFm814x600m.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : FM814x600M section
 *
 * (C) InSys by Ekkore Feb, 2019
 *
 *
 ************************************************************
*/

#ifndef _CTRL_FM814x600M_H
 #define _CTRL_FM814x600M_H

#include "ctrladc.h"

#pragma pack(push, 1)    

#define BRD_ADCCHANCNT			8	// Number of ADC channels
#define BRD_DDCCHANCNT			8	// Number of DDC channels
//#define	BRD_GAINCNT			1	// Number of gains

//const int BRD_CLKDIVCNT	= 1; // Number of clock dividers = 1 (1)

// FM214x1GTRF Clock sources
enum {
	BRDclks_ADC_DISABLED	= 0,	// 
	BRDclks_ADC_SUBGEN		= 1,	// SubModule Internal Generator 
	BRDclks_ADC_SUBGEN_PLL	= 2,	// SubModule Internal Generator + PLL
	BRDclks_ADC_EXTCLK		= 0x81,	// External SubModule Clock
	BRDclks_ADC_EXTCLK_PLL	= 3,	// External SubModule Clock + PLL
	BRDclks_ADC_BASEGEN		= 0x85,	// Base Module Generator

	BRDclks_DAC_DISABLED	= 0,	// 
	BRDclks_DAC_SUBGEN		= 1,	// SubModule Generator
	BRDclks_DAC_SUBGEN_PLL	= 2,	// SubModule Internal Generator + PLL
	BRDclks_DAC_EXTCLK		= 0x81,	// External SubModule Clock
	BRDclks_DAC_EXTCLK_PLL	= 3,	// External SubModule Clock + PLL
	BRDclks_DAC_BASEGEN		= 0x85,	// Base Module Generator
};

// FM214x1GTRF start sources
enum {
	BRDsts_ADC_CHAN0	= 0,	// Start from channel 0
	BRDsts_ADC_EXT		= 2,	// Start from START IN (external)
	BRDsts_ADC_PRG		= 3,	// Program start
};



// Numbers of Specific Command
typedef enum _FM814X600M_NUM_CMD
{
	FM814X600Mcmd_SETMU		= 64,
	FM814X600Mcmd_GETMU		= 65,
	FM814X600Mcmd_SETDDCPARAM		= 66,
	FM814X600Mcmd_DAC_SETLMFCCALIBRATION	= 80,
	FM814X600Mcmd_DAC_GETLMFCCALIBRATION	= 81,
} FM814X600M_NUM_CMD;

typedef struct _FM814X600M_MU {
	U32		size;					// sizeof(FM814X600MSRV_MU)
	U32		chanMask;				// маска выбранных каналов
	U32		syncMode;				// режим синхронизации (2 - Master, 1 - Single, 0 - Slave) всегда 1
	double	samplingRate;			// частота дискретизации (Гц): 10 МГц .. 125 МГц
	double	clockValue;				// значение частоты выбранного внешнего источника (Гц)
	U32		clockSrc;				// источник тактовой частоты (0 - выключен, 1 - генератор на субмодуле, 0x81 - внешний)
	double	range[BRD_ADCCHANCNT];		// шкала преобразования для каждого канала (Вольт): 4.0, 2.0, 1.0, 0.5, 0.25
	double	bias[BRD_ADCCHANCNT];		// смещение нуля для каждого канала (Вольт): -2*ШП .. +2*ШП
	U32		inpResist[BRD_ADCCHANCNT];	// входное сопротивление для каждого канала (0 - 1 МOм, 1 - 50 Oм)
	U32		dcCoupling[BRD_ADCCHANCNT];	// открытость (1) / закрытость (0) входа по постоянной составляющей для каждого канала
	U32		format;					// 0 - 16 бит, 1 - 8 бит

	U32		startSrc;				// субмодульный источник старта: 0 - канал 0, 1 - канал 1, 3 - программный
	double	startLevel;				// уровень старта в вольтах (от -2.5 В до +2.5 В – при внешнем старте)
} FM814X600MSRV_MU, *PFM814X600MSRV_MU;

typedef struct _FM814X600M_ADCPARAM {
	U32		anInpRangeCode[BRD_ADCCHANCNT];	// Шкала преобразования в процентах от номинальной: 80, 86, 93, 100, 107, 113, 120
	double	adBias[BRD_ADCCHANCNT];			// Смещение нуля (мВольт)
	S32		anOffsetAdjust[BRD_ADCCHANCNT];	// Цифровая коррекция постю составляющей (ЕМР): -128..+127
	S32		anInputBuf1Current[BRD_ADCCHANCNT];	// Ток входного буфера АЦП (мкА): 120, 160, 200, ... 440
	S32		anInputBuf2Current[BRD_ADCCHANCNT];	// Ток входного буфера АЦП (мкА): 120, 160, 200, ... 440
	S32		anFDControl[BRD_ADCCHANCNT];		// Обнаружение сигнала в АЦП: 0-выкл., 1-вкл.
	double	adFDUpper[BRD_ADCCHANCNT];			// Верхний порог обнаружения АЦП: 0 .. -78
	double	adFDLower[BRD_ADCCHANCNT];			// Нижний порог обнаружения АЦП: 0 .. -78
	S32		anFDDwellTime[BRD_ADCCHANCNT];		// Время удержания нижнего порога АЦП (Tclk): 1 .. 65535 
} FM814X600MSRV_ADCPARAM, *PFM814X600MSRV_ADCPARAM;

typedef struct _FM814X600M_DDCPARAM {
	U32		isDdcEnable;		// Разрешение DDC: 0-работает АЦП, 1-работает DDC
	U32		nDdcChannelMask;	// Маска каналов DDC: 0x1, 0x3, 0xF
	U32		nDdcDecimation;		// Коэффициент децимации DDC: 2, 4, 8, 16
	U32		nDdcMixerSelect;	// Тип сигнала от АЦП: 0-действительный, 1-комплексный
	U32		nDdcGain;			// Коэф. усиления для каналов DDC: 1, 2

	U32		anDdcInpI[BRD_DDCCHANCNT];	// Номер АЦП, к которому подключены Input I DDC: 0, 1
	U32		anDdcInpQ[BRD_DDCCHANCNT];	// Номер АЦП, к которому подключены Input Q DDC: 0, 1
//	U32		anDdcGain[BRD_DDCCHANCNT];	// Коэф. усиления для каналов DDC: 1, 2
	double	adDdcFc[BRD_DDCCHANCNT];	// Центральная частота для каналов DDC (Гц)
	double	adDdcPhase[BRD_DDCCHANCNT];	// Начальная фаза для каналов DDC (град): 0.0 - 360.0
} FM814X600MSRV_DDCPARAM, *PFM814X600MSRV_DDCPARAM;

typedef struct  _FM814X600MSRV_DacLmfcCalibration {
	S32		idx;			// не используется, должно быть 0
	S32		aux;			// не используется, должно быть 0
	S32		nPClockFactor;	// (только GET)
	S32		nPClockPerMF;	// (только GET)
	S32		nK;				// (только GET)
	S32		nCount;			// (только GET) Количество заполненных ячеек в aRegVal[]
	S32		aRegVal[32];	// GET: значения регистров 0x302, 0x303 для всех м/с AD9144
							// SET: aRegVal[0] значения для 0x304/0x305, [1] - для 0x306/0x307
} FM814X600MSRV_DacLmfcCalibration, *PFM814X600MSRV_DacLmfcCalibration;

#pragma pack(pop)    

#endif // _CTRL_FM814x600M_H

//
// End of file
//