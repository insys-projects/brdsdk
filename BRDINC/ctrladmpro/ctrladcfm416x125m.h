/*
 ****************** File CtrlFm214x250m.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : FM216x250M section
 *
 * (C) InSys by Ekkore Aug, 2011
 *
 *
 ************************************************************
*/

#ifndef _CTRL_FM216x250M_H
 #define _CTRL_FM216x250M_H

#include "ctrladc.h"

#pragma pack(push, 1)    

#define BRD_CHANCNT			2	// Number of channels

// FM216x250M Clock sources
enum {
	BRDclks_ADC_DISABLED	= 0,	// 
	BRDclks_ADC_SUBGEN		= 1,	// SubModule Internal Generator 
	BRDclks_ADC_EXTCLK		= 0x81	// External SubModule Clock
};

// FM216x250M start sources
enum {
	BRDsts_ADC_CHAN0	= 0,	// Start from channel 0
	BRDsts_ADC_CHAN1	= 1,	// Start from channel 1
	BRDsts_ADC_EXT		= 2,	// Start from START IN (external)
	BRDsts_ADC_PRG		= 3,	// Program start
};



// Numbers of Specific Command
typedef enum _FM216X250M_NUM_CMD
{
	FM216X250Mcmd_SETMU		= 64,
	FM216X250Mcmd_GETMU		= 65,
} FM216X250M_NUM_CMD;

typedef struct _FM216X250MSRV_MU {
	U32		size;					// sizeof(ADC214X200MSRV_MU)
	S32		subType;				// тип субмодуля: 0 - стандарт, 1 - WB HF, 2 - WB LF
	U32		chanMask;				// маска выбранных каналов
	U32		syncMode;				// режим синхронизации (2 - Master, 1 - Single, 0 - Slave)
	double	samplingRate;			// частота дискретизации (Гц) (возможные значения делителя = 1, 2, 4, 8, 16, 32, 64, 128, 256)
	double	clockValue;				// значение частотs выбранного источника тактирования (Гц) (для источников 2, 4, 0x81, 0x82)
	U32		clockSrc;				// источник тактовой частоты: 0-выкл., 1-внутренний, 2-внутренний+PLL, 4-от базмодуля, 0x81-внешний, 0x82-внешний+PLL 
	
	double	range[BRD_CHANCNT];		// входной диапазон для каждого канала (10.0 Вольт, 2.0 Вольт, 0.5 Вольт, 0.1 Вольт)
	double	bias[BRD_CHANCNT];		// смещение нуля для каждого канала
	U32		inpFilter[BRD_CHANCNT];	// входное сопротивление для каждого канала (0 - 1 МOм, 1 - 50 Oм)
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
	U32		startResist;			// вх.сопротивление для сигнала внешнего старта: 0 - 2.5 кОм, 1 - 50 Ом
	double	gainTuning[BRD_CHANCNT];// подстройка усиления каждого канала (от -2% до +2%)
	U32		invClk;					// 1 - инверсия сигнала тактирования

	///////
	S32	 offset[BRD_CHANCNT];	//  Смещение 0 АЦП: -127..127	
	S32	 vref[BRD_CHANCNT];		//  Опорное напряжение АЦП:0-2.0В,1-2.1В,2-2.2В,3-2.3В,4-2.4В,5-2.5
	S32	 bufCur1[BRD_CHANCNT];	//  Ток входного буфера 1: -100,-90..0,10,20..520,530	
	S32	 bufCur2[BRD_CHANCNT];	//  Ток входного буфера 2: -100,-90..0,10,20..520,530

} FM216X250MSRV_MU, *PFM216X250MSRV_MU;


#pragma pack(pop)    

#endif // _CTRL_FM216x250M_H

//
// End of file
//