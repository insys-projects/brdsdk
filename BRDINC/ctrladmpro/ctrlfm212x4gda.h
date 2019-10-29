/*
 ****************** File CtrlFm212x4gda.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : FM212x4GDA section
 *
 * (C) InSys by Ekkore June 2017
 *
 ************************************************************
*/

#ifndef _CTRL_FM212x4GDA_H
 #define _CTRL_FM212x4GDA_H

#include "ctrladc.h"

#pragma pack(push, 1)    

#define BRD_ADCCHANCNT			2	// Number of channels
#define BRD_DDCCHANCNT			2	// Number of DDC channels
#define BRD_DACCHANCNT			4	// Number of DDC channels
//#define	BRD_GAINCNT			1	// Number of gains

//const int BRD_CLKDIVCNT	= 1; // Number of clock dividers = 1 (1)

// FM212x4GDA Clock sources
enum {
	BRDclks_ADC_DISABLED	= 0,	// 
	//BRDclks_ADC_SUBGEN		= 1,	// SubModule Internal Generator 
	BRDclks_ADC_SUBGEN_PLL	= 2,	// SubModule Internal Generator + PLL
	BRDclks_ADC_EXTCLK		= 0x81,	// External SubModule Clock
	BRDclks_ADC_EXTCLK_PLL	= 3,	// External SubModule Clock + PLL
	BRDclks_ADC_BASEGEN		= 0x85,	// Base Module Generator

	BRDclks_DAC_DISABLED	= 0,	// 
	//BRDclks_DAC_SUBGEN		= 1,	// SubModule Generator
	BRDclks_DAC_SUBGEN_PLL	= 2,	// SubModule Internal Generator + PLL
	BRDclks_DAC_EXTCLK		= 0x81,	// External SubModule Clock
	BRDclks_DAC_EXTCLK_PLL	= 3,	// External SubModule Clock + PLL
	BRDclks_DAC_BASEGEN		= 0x85,	// Base Module Generator
};

// FM212x4GDA start sources
enum {
	BRDsts_ADC_CHAN0	= 0,	// Start from channel 0
	BRDsts_ADC_EXT		= 2,	// Start from START IN (external)
	BRDsts_ADC_PRG		= 3,	// Program start
};



// Numbers of Specific Command
typedef enum _FM212X4GDA_NUM_CMD
{
	FM212X4GDAcmd_SETMU		= 64,
	FM212X4GDAcmd_GETMU		= 65,
	FM212X4GDAcmd_SETDDCPARAM		= 66,
	FM212X4GDAcmd_GETDDCPARAM		= 67,
	FM212X4GDAcmd_SETDACPARAM		= 68,
	FM212X4GDAcmd_GETDACPARAM		= 69,
	FM212X4GDAcmd_DAC_SETLMFCCALIBRATION	= 80,
	FM212X4GDAcmd_DAC_GETLMFCCALIBRATION	= 81,
	FM212X4GDAcmd_ADC_SETSYSREFCALIBRATION	= 82,
} FM212X4GDA_NUM_CMD;

typedef struct _FM212X4GDA_MU {
	U32		size;					// sizeof(FM212X4GDASRV_MU)
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
} FM212X4GDASRV_MU, *PF212X4GDASRV_MU;

typedef struct _FM212X4GDA_DDCPARAM {
	U32		nDdcDecimation;				// Коэффициент децимации DDC: 4, 8, 10, 16, 20, 32
	double	adDdcFc[BRD_DDCCHANCNT];	// Центральная частота для каналов DDC (Гц)
	double	adDdcPhase[BRD_DDCCHANCNT];	// Начальная фаза для каналов DDC (град): 0.0 - 360.0
} FM212X4GDASRV_DDCPARAM, *PF212X4GDASRV_DDCPARAM;

typedef struct _FM212X4GDA_DACPARAM { 
	// для AD9136 используются только поля:
	// nInvSincFilterA, 
	// dGroupDelayA,
	// adDigitalGain[0 и 1],
	// adDcOffset[0 и 1]

	S32		nDModEnableA;		// Включить модулятор каналов ЦАП0 и ЦАП1: 0-выкл. 1-вкл.
	S32		nDModEnableB;		// Включить модулятор каналов ЦАП2 и ЦАП3: 0-выкл. 1-вкл.
	REAL64	dDModFreqA;			// Центральная частота NCO каналов ЦАП0 и ЦАП1 (Гц)
	REAL64	dDModFreqB;			// Центральная частота NCO каналов ЦАП2 и ЦАП3 (Гц)

	S32		nInvSincFilterA;	// Инверсный фильтр каналов ЦАП0 и ЦАП1: 0-выкл. 1-вкл.
	S32		nInvSincFilterB;	// Инверсный фильтр каналов ЦАП2 и ЦАП3: 0-выкл. 1-вкл.
	REAL64	dPhaseAdjustA;		// Вырвнивание фазы между каналами ЦАП0 и ЦАП1: -14..+14 (градус Цельсия)
	REAL64	dPhaseAdjustB;		// Вырвнивание фазы между каналами ЦАП2 и ЦАП3: -14..+14 (градус Цельсия)
	REAL64	dGroupDelayA;		// Задержка ЦАП0 и ЦАП1 на количество циклов: -4..+3.5
	REAL64	dGroupDelayB;		// Задержка ЦАП2 и ЦАП3 на количество циклов: -4..+3.5
	REAL64	adDigitalGain[BRD_DACCHANCNT];	// Коэф. передачи ЦАП0-ЦАП3: от 0.0 до 2.0
	REAL64	adDcOffset[BRD_DACCHANCNT];		// Коррекция смещения нуля ЦАП0-ЦАП3 (ЕМР): -32768..+32767
} FM212X4GDASRV_DACPARAM,*PF212X4GDASRV_DACPARAM;

typedef struct _FM212X4GDA_AdcChipParam {
	S32		isCalibrate;					// Выполнять автокалибровку: 1-да, 0-нет
	U32		anCalibrateSig[BRD_DDCCHANCNT];	// Уровень сигнала АЦП0, для которого калибруем: 0 - выше -10dBFS, 1 - ниже -10dBFS
	S32		anOverThd0[BRD_DDCCHANCNT];		// Уровень порога перегрузки OVR_T0: 1 - 255
	S32		anOverThd1[BRD_DDCCHANCNT];		// Уровень порога перегрузки OVR_T1: 1 - 255
	double	adInpRangeV[BRD_DDCCHANCNT];	// 0 или Шкалы преобразования каналов АЦП0-1 (В)
	double	adBiasV[BRD_DDCCHANCNT];		// Смещение нуля каналов АЦП0-1 (В)
} FM212X4GDA_AdcChipParam, *PFM212X4GDA_AdcChipParam;

typedef struct  _FM212X4GDASRV_DacLmfcCalibration {
	S32		idx;			// не используется, должно быть 0
	S32		aux;			// не используется, должно быть 0
	S32		nPClockFactor;	// (только GET)
	S32		nPClockPerMF;	// (только GET)
	S32		nK;				// (только GET)
	S32		nCount;			// (только GET) Количество заполненных ячеек в aRegVal[]
	S32		aRegVal[32];	// GET: значения регистров 0x302, 0x303 для всех м/с AD9144
							// SET: aRegVal[0] значения для 0x304/0x305, [1] - для 0x306/0x307
} FM212X4GDASRV_DacLmfcCalibration, *PFM212X4GDASRV_DacLmfcCalibration;

#pragma pack(pop)    

#endif // _CTRL_FM212x4GDA_H

//
// End of file
//