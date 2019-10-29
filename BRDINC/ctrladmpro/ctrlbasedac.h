/*
 ****************** File CtrlBaseDac.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : BASEDAC section
 *
 * (C) InSys by Dorokhin Andrey Oct, 2005
 *
 *
 ************************************************************
*/

#ifndef _CTRL_BASEDAC_H
 #define _CTRL_BASEDAC_H

#include "ctrldac.h"

#pragma pack(push, 1)    

// getting configuration
typedef struct _BRD_BaseDacCfg
{
	U32		Bits;						// разрядность
	U32		Encoding;					// тип кодировки (0 - прямой, 1 - двоично-дополнительный, 2 - код Грея)
	U32		MinRate;					// минимальная частота дискретизации
	U32		MaxRate;					// максимальная частота дискретизации
	U32		OutRange;					// выходной диапазон
	U32		FifoSize;					// размер FIFO ЦАП (в байтах)
	U32		MaxChan;					// максимальное число каналов
	U32		FifoType;					// тип FIFO: 0 - внутреннее, 1 - внешнее
} BRD_BaseDacCfg, *PBRD_BaseDacCfg;

#pragma pack(pop)    

#endif // _CTRL_BASEDAC_H

//
// End of file
//