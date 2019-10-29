/*
 ****************** File CtrlFm412x500m.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : FM412x500M section
 *
 * (C) InSys by Ekkore Feb 2012
 *
 *
 ************************************************************
*/

#ifndef _CTRL_FM412x500M_H
 #define _CTRL_FM412x500M_H

#include "ctrladc.h"

#pragma pack(push, 1)    

#define BRD_CHANCNT			4	// Number of channels

// FM412x500M Clock sources
enum {
	BRDclks_ADC_DISABLED	= 0,	// 
	BRDclks_ADC_SUBGEN		= 1,	// SubModule Internal Generator 
	BRDclks_ADC_SUBGEN_PLL	= 2,	// SubModule Internal Generator + PLL
	BRDclks_ADC_BASEGEN		= 0x85,	// Base Module Generator
	BRDclks_ADC_EXTCLK		= 0x81,	// External SubModule Clock
	BRDclks_ADC_EXTCLK_PLL	= 0x82,	// External SubModule Clock + PLL
};

// FM412x500M start sources
enum {
	BRDsts_ADC_EXT		= 2,	// Start from START IN (external)
	BRDsts_ADC_PRG		= 3,	// Program start
};



// Numbers of Specific Command
typedef enum _FM412X500M_NUM_CMD
{
	FM412X500Mcmd_SETMU		= 64,
	FM412X500Mcmd_GETMU		= 65,
	FM412X500Mcmd_GETEVENTCNT	= 66, // Get event counter
	FM412X500Mcmd_RESETEVENTCNT	= 67, // reset event counter
	FM412X500Mcmd_SUBSYNC	= 68
} FM412X500M_NUM_CMD;

typedef struct _FM412X500MSRV_MU {
	U32		size;					// sizeof(ADC214X200MSRV_MU)
	S32		subType;				// тип субмодул€: 0 - всегда
	U32		chanMask;				// маска выбранных каналов
	U32		syncMode;				// режим синхронизации (2 - Master, 1 - Single, 0 - Slave)
	double	samplingRate;			// частота дискретизации (√ц)
	double	clockValue;			// значение частоты выбранного источника тактировани€ (√ц) (дл€ источников 2, 4, 0x81, 0x82)
	U32		clockSrc;				// источник тактовой частоты: 0-выкл., 1-внутренний, 2-внутренний+PLL, 4-от базмодул€, 0x81-внешний, 0x82-внешний+PLL
	U32		format;					// 0 - 16 бит, 1 - 8 бит

	U32		pretrigEnable;			// включение режима претриггера
	U32		pretrigExernal;			// включение режима внешнего претриггера
	U32		pretrigAssur;			// включение режима гарантированного претриггера
	U32		pretrigSamples;			// число отсчЄтов претриггера
	U32		startDelayCnt;			// счетчик начальной задержки
	U32		daqCnt;					// счетчик принимаемых данных
	U32		skipCnt;				// счетчик пропущенных данных
	U32		cnt0Enable;				// разрешение счетчика начальной задержки
	U32		cnt1Enable;				// разрешение счетчика принимаемых данных
	U32		cnt2Enable;				// разрешение счетчика пропущенных данных

	U32		baseStartSrc;			// базовый источник старта: 0 - программный, 7 - от субмодул€, 10 - от DDS базового модул€
	U32		baseStartInv;			// 1 - инверси€ базового источника старта
	U32		baseStopSrc;			// базовый источник останова
	U32		baseStopInv;			// 1 - инверси€ базового источника останова
	U32		startStopMode;			// 1 - режим триггерного старта-останова, 0 - режим потенциального старта-останова
	U32		reStart;				// 1 - режим сбора с автоматическим перезапуском
	U32		startSrc;				// субмодульный источник старта: 0 - канал 0, 1 - канал 1, 2 - внешний, 3 - программный
	U32		invStart;				// 1 - инверси€ субмодульного источника старта
	double	startLevel;				// уровень старта в вольтах (от -2.5 ¬ до +2.5 ¬ Ц при внешнем старте)
	U32		startResist;			// вх.сопротивление дл€ сигнала внешнего старта: 0 - 2.5 кќм, 1 - 50 ќм
} FM412X500MSRV_MU, *PFM412X500MSRV_MU;

#pragma pack(pop)    

#endif // _CTRL_FM412x500M_H

//
// End of file
//