/*
 ****************** File CtrlFm112x2g6da.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : FM112x2G6DA section
 *
 * (C) InSys by Ekkore Feb, 2016
 *
 *
 ************************************************************
*/

#ifndef _CTRL_FM112x2G6DA_H
 #define _CTRL_FM112x2G6DA_H

#include "ctrladc.h"

#pragma pack(push, 1)    

#define BRD_ADCCHANCNT			1	// Number of channels
#define BRD_DDCCHANCNT			2	// Number of DDC channels
//#define	BRD_GAINCNT			1	// Number of gains

//const int BRD_CLKDIVCNT	= 1; // Number of clock dividers = 1 (1)

// FM112x2G6DA Clock sources
enum {
	BRDclks_ADC_DISABLED	= 0,	// 
	BRDclks_ADC_SUBGEN_PLL	= 2,	// PLL с внутренним генератором
	BRDclks_ADC_EXTCLK_PLL	= 3,	// PLL с внешним генератором
	BRDclks_ADC_BASEGEN_PLL	= 4,	// PLL с генератором базмодуля
	BRDclks_ADC_EXTCLK_DIV_PLL	= 0x43,	// DIV+PLL с внешним генератором
	BRDclks_ADC_BASEGEN_DIV_PLL	= 0x44,	// DIV+PLL с генератором базмодуля
	BRDclks_ADC_EXTCLK		= 0x81,	// внешний генератор
	BRDclks_ADC_BASEGEN		= 0x85,	// генератор базмодуля
	BRDclks_ADC_EXTCLK_DIV	= 0xC1,	// DIV и внешний генератор
	BRDclks_ADC_BASEGEN_DIV	= 0xC5,	// DIV с генератором базмодуля

	BRDclks_DAC_DISABLED		= BRDclks_ADC_DISABLED,
	BRDclks_DAC_SUBGEN_PLL		= BRDclks_ADC_SUBGEN_PLL,
	BRDclks_DAC_EXTCLK_PLL		= BRDclks_ADC_EXTCLK_PLL,
	BRDclks_DAC_BASEGEN_PLL		= BRDclks_ADC_BASEGEN_PLL,
	BRDclks_DAC_EXTCLK_DIV_PLL	= BRDclks_ADC_EXTCLK_DIV_PLL,
	BRDclks_DAC_BASEGEN_DIV_PLL	= BRDclks_ADC_BASEGEN_DIV_PLL,
	BRDclks_DAC_EXTCLK			= BRDclks_ADC_EXTCLK,
	BRDclks_DAC_BASEGEN			= BRDclks_ADC_BASEGEN,
	BRDclks_DAC_EXTCLK_DIV		= BRDclks_ADC_EXTCLK_DIV,
	BRDclks_DAC_BASEGEN_DIV		= BRDclks_ADC_BASEGEN_DIV,
};

// FM112x2G6DA start sources
enum {
	BRDsts_ADC_CHAN0	= 0,	// Start from channel 0
	BRDsts_ADC_EXT		= 2,	// Start from START IN (external)
	BRDsts_ADC_PRG		= 3,	// Program start
};

// DAC Operation Mode
enum {
	BRDmode_DAC_DDS = 0,
	BRDmode_DAC_DAC = 1,
	BRDmode_DAC_QM  = 2,
};


// Numbers of Specific Command
typedef enum _FM112X2G6DA_NUM_CMD
{
	FM112X2G6DAcmd_SETMU		= 64,
	FM112X2G6DAcmd_GETMU		= 65,
	FM112X2G6DAcmd_SETDDCPARAM	= 66,
	FM112X2G6DAcmd_PRESYNC		= 67,
	FM112X2G6DAcmd_TIMESTAMP	= 68,
	FM112X2G6DAcmd_LOOPBACK		= 69,
	FM112X2G6DAcmd_SETDATAPATHMODE	= 70,
	FM112X2G6DAcmd_ADC_SETOFFSETADJUST = 71,
	FM112X2G6DAcmd_DAC_SETLMFCCALIBRATION	= 80,
	FM112X2G6DAcmd_DAC_GETLMFCCALIBRATION	= 81,
} FM112X2G6DA_NUM_CMD;

typedef struct _FM112X2G6DA_MU {
	U32		size;					// sizeof(FM112X2G6DASRV_MU)
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
} FM112X2G6DASRV_MU, *PF112X2G6DASRV_MU;

typedef struct _FM112X2G6DA_DDCPARAM {
	U32		isDdcEnable;		// Разрешение DDC: 0-работает АЦП, 1-работает DDC
	U32		nDdcChannelMask;	// Маска каналов DDC: 0x1, 0x3, 0xF
	U32		nDdcDecimation;		// Коэффициент децимации DDC: 2, 4, 8, 16

	U32		anDdcGain[BRD_DDCCHANCNT];	// Коэф. усиления для каналов DDC: 1, 2
	double	adDdcFc[BRD_DDCCHANCNT];	// Центральная частота для каналов DDC (Гц)
} FM112X2G6DASRV_DDCPARAM, *PF112X2G6DASRV_DDCPARAM;

typedef struct  _FM112X2G6DASRV_DacLmfcCalibration {
	S32		idx;			// не используется, должно быть 0
	S32		aux;			// не используется, должно быть 0
	S32		nPClockFactor;	// (только GET)
	S32		nPClockPerMF;	// (только GET)
	S32		nK;				// (только GET)
	S32		nCount;			// (только GET) Количество заполненных ячеек в aRegVal[]
	S32		aRegVal[32];	// GET: значения регистров 0x302, 0x303 для всех м/с AD9144
							// SET: aRegVal[0] значения для 0x304/0x305, [1] - для 0x306/0x307
} FM112X2G6DASRV_DacLmfcCalibration, *PFM112X2G6DASRV_DacLmfcCalibration;

#pragma pack(pop)    

#endif // _CTRL_FM112x2G6DA_H

//
// End of file
//