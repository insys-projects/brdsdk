/*
 ****************** File CtrlAdc214x400m.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : ADM214x400M(WB) and ADM212x500M(WB) section
 *
 * (C) InSys by Ekkore Aug, 2008
 *
 *
 ************************************************************
*/

#ifndef _CTRL_ADC214x400M_H
 #define _CTRL_ADC214x400M_H

#include "ctrladc.h"

#pragma pack(push, 1)    

#define BRD_CHANCNT			2	// Number of channels

const int BRD_CLKDIVCNT	= 5; // Number of clock dividers = 9 (1, 2, 4, 8, 16)

// ADC214x400M Clock sources
enum {
	BRDclks_ADC_DISABLED	= 0,	// 
	BRDclks_ADC_SUBGEN0		= 1,	// SubModule Generator 0
	BRDclks_ADC_SUBGEN1		= 2,	// SubModule Generator 1
	BRDclks_ADC_EXTCLK		= 0x81,	// External SubModule Clock
};

// ADC214x400M start sources
enum {
	BRDsts_ADC_CHAN0	= 0,	// Start from channel 0
	BRDsts_ADC_CHAN1	= 1,	// Start from channel 1
	BRDsts_ADC_EXT		= 2,	// Start from START IN (external)
	BRDsts_ADC_PRG		= 3,	// Program start
};

const int BRD_GAINCNT	= 4; // Number of gains

// input gain
const double BRD_Gain[BRD_GAINCNT] =
{
	0.05,	// 0x00
	0.25,	// 0x01
	1.0,	// 0x02
	5.0		// 0x03
};

// input range (Volt)
const double BRD_Range[BRD_GAINCNT] =
{
	10.0,	// 0x00
	2.0,	// 0x01
	0.5,	// 0x02
	0.1		// 0x03
};

// Numbers of Specific Command
typedef enum _ADC214X400M_NUM_CMD
{
	ADC214X400Mcmd_SETMU		= 64,
	ADC214X400Mcmd_GETMU		= 65,
	ADC214X400Mcmd_SETSTARTCLKSL= 66,
	ADC214X400Mcmd_GETSTARTCLKSL= 67,
} ADC214X400M_NUM_CMD;

typedef struct _ADC214X400MSRV_MU {
	U32		size;					// sizeof(ADC214X200MSRV_MU)
	S32		subType;				// тип субмодуля: 0 - стандарт, 1 - WB HF, 2 - WB LF
	U32		chanMask;				// маска выбранных каналов
	U32		syncMode;				// режим синхронизации (2 - Master, 1 - Single, 0 - Slave)
	double	samplingRate;			// частота дискретизации (Гц) (возможные значения делителя = 1, 2, 4, 8, 16, 32, 64, 128, 256)
	double	clockValue;				// значение частота выбранного внешнего источника (Гц)
	U32		clockSrc;				// источник тактовой частоты (0 - выключен, 1 - генератор на субмодуле (ver 1.1+), 2 - синтезатор на субмодуле, 0x81 - внешний)
	double	range[BRD_CHANCNT];		// входной диапазон для каждого канала (10.0 Вольт, 2.0 Вольт, 0.5 Вольт, 0.1 Вольт)
	double	bias[BRD_CHANCNT];		// смещение нуля для каждого канала
	U32		resist[BRD_CHANCNT];	// входное сопротивление для каждого канала (0 - 1 МOм, 1 - 50 Oм)
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
	U32		baseStartSrc;			// базовый источник старта: 0 - программный, 7 - от субмодуля, 10 - от DDS (only for AMBPEX8)
	U32		baseStartInv;			// 1 - инверсия базового источника старта
	U32		baseStopSrc;			// базовый источник останова
	U32		baseStopInv;			// 1 - инверсия базового источника останова
	U32		startStopMode;			// 1 - режим триггерного старта-останова, 0 - режим потенциального старта-останова
	U32		reStart;				// 1 - режим сбора с автоматическим перезапуском
	U32		startSrc;				// субмодульный источник старта: 0 - канал 0, 1 - канал 1, 2 - внешний, 3 - программный
	U32		invStart;				// 1 - инверсия субмодульного источника старта
	double	startLevel;				// уровень старта в вольтах (от -2.5 В до +2.5 В – при внешнем старте)
	double	gainTuning[BRD_CHANCNT];// подстройка усиления каждого канала (от -2% до +2%)
	U32		invClk;					// 1 - инверсия сигнала тактирования
} ADC214X400MSRV_MU, *PADC214X400MSRV_MU;


//=************************* SM *****************************
// Специальные команды для спецпрошивки ПЛИС ADM модификации 0x100
// (узел прорерживания sumsung)
//
enum 
{
	ADC214X400Mcmd_SMSETMODE	= 71,
	ADC214X400Mcmd_SMLOADDATA	= 72,
	ADC214X400Mcmd_SMISAVAILABLE= 73
};

typedef struct  
{
	S32		isCalculate;	// 1 - включение режима обработки
	S32		isTest;			// 1 - включение режима имитатора
	S32		sumCnt;			// количество суммирований
	S32		shiftCnt;		// количество сдвигов
} ADC214X400MSRV_SMSETMODE, *PADC214X400MSRV_SMSETMODE;

typedef struct  
{
	S16		*pBuf;			// массив 16-разрядных отсчетов
	S32		size;			// пазмер массива в отсчетах
} ADC214X400MSRV_SMLOADDATA, *PADC214X400MSRV_SMLOADDATA;


// Номера управляющих регистров
enum 
{
	ADCnr_CLCTRL	= 0x300,
	ADCnr_CLSUM		= 0x301,
	ADCnr_CLSHIFT	= 0x302,
	ADCnr_CLDATA	= 0x303
};


#pragma pack(pop)    

#endif // _CTRL_ADC214x400M_H

//
// End of file
//