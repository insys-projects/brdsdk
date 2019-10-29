/*
 ****************** File ctrlrfdr4_ddc.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants for rfdr4 service
 *
 * (C) InSys by Sklyarov A. Aug,2014
 *
 *
 ************************************************************
*/

#ifndef _CTRL_RFDR4_DDC_H
 #define _CTRL_RFDR4_DDC_H


#include "ctrladmpro.h"
typedef enum {
	
	BRDctrl_RFDR4_DDC_SETCHANMASK	= BRDctrl_DAQ + 1,  // Set mask of open channels (*U32)
	BRDctrl_RFDR4_DDC_SETSTARTMODE	= BRDctrl_DAQ + 2,  // Set start mode (BRD_StartMode_ddc)
	BRDctrl_RFDR4_DDC_SETFRAME		= BRDctrl_DAQ + 3,	// Set of FRAME(PBRD_SetFrame)
	BRDctrl_RFDR4_DDC_GETSRCSTREAM	= BRDctrl_DAQ + 4,	// Get source address for stream (*U32)
	BRDctrl_RFDR4_DDC_FIFORESET		= BRDctrl_DAQ + 5,	// FIFO reset (NULL)
	BRDctrl_RFDR4_DDC_START			= BRDctrl_DAQ + 6,	// Start(NULL)
	BRDctrl_RFDR4_DDC_STOP			= BRDctrl_DAQ + 7,	// Stop (NULL)
	BRDctrl_RFDR4_DDC_FIFOSTATUS	= BRDctrl_DAQ + 8,	// Get  FIFO status (*U32)
	BRDctrl_RFDR4_DDC_STARTDDC		= BRDctrl_DAQ + 9,	// Start DDC(NULL)
    BRDctrl_RFDR4_DDC_STOPDDC		= BRDctrl_DAQ + 10,	// Stop DDC(NULL)
	BRDctrl_RFDR4_DDC_SETDDCEXT		= BRDctrl_DAQ + 11,	// Set Mode of DDC (BRD_SetDdcExt)
	BRDctrl_RFDR4_DDC_SETCORRECTEXT	= BRDctrl_DAQ + 12,	// Set correctExt(BRD_SetCorrectExt)
	BRDctrl_RFDR4_DDC_SETSCAN		= BRDctrl_DAQ + 13,	// Установка сканера (загрузка памяти частот)(BRD_SetScan)
	BRDctrl_RFDR4_DDC_SELSCAN		= BRDctrl_DAQ + 14,	// Выбор памяти сканера для работы(0 или 1 (*U32)
	BRDctrl_RFDR4_DDC_ENBSCAN		= BRDctrl_DAQ + 15,	// Включение/выключение режима сканирования (*U32)
	BRDctrl_RFDR4_DDC_SETLABEL		= BRDctrl_DAQ + 16,	// Запись временной метки(32-разрядное пользовательское слово в заголовке)(*U32)
	BRDctrl_RFDR4_DDC_INITPOWER		= BRDctrl_DAQ + 17,	// Мониторинг питания (NULL)
	BRDctrl_RFDR4_DDC_GETPOWER		= BRDctrl_DAQ + 18,	// Мониторинг питания (BRD_GetPower)
	BRDctrl_RFDR4_DDC_SETDETECT		= BRDctrl_DAQ + 19,	// Установка режима детектора тактовой (BRD_SetDetect)
	BRDctrl_RFDR4_DDC_GETDETECT		= BRDctrl_DAQ + 20,	// Установка режима детектора тактовой (*U32_)
	BRDctrl_RFDR4_DDC_SETLED		= BRDctrl_DAQ + 21,	// Установка режима светодиода (BRD_SetLed)
	BRDctrl_RFDR4_DDC_ILLEGAL
};

#pragma pack(push, 1)  


//-------------------------------------------------------------------------------
// for  SETSCAN function
typedef struct _BRD_SetScan
{	
		U32	wrscaner;	// Номер сканера: 0 или 1 
		U32	*Fc;		// Указатель на массив центральных частот групп каналов (8-мь групп) на всю процедуру 
		U32  Nscan;		// Число срезов сканирования 
		U32 SampleRate; // Частота поступления данных с АЦП (Гц)
} BRD_SetScan,*PBRD_SetScan;
//---------------------------------------------------------------------------------
// for  SETDDCEXT function
typedef struct _BRD_SetDdcExt
{	
		U32	*Fc;		// Указатель на массив центральных частот групп  каналов (8-мь групп)
		U32 SampleRate; // Частота поступления данных с АЦП (Гц)
		U32 ChanMask;   // Маска групп каналов DDC: 0x1 - 0xFF
} BRD_SetDdcExt,*PBRD_SetDdcExt;

// for  SETFRAME function
typedef struct _BRD_SetFrame
{	
	U32  FrameOn;				// 1- включить режим кадрового ввода 
	U32  HeaderOn;				// 1- вставлять заголовок в начале строки
	U32  SyncWord;				// Синхрослово заголовка (32 разряда)
	U32  UserWord;				// Синхрослово заголовка (32 разряда)
	U32  SkipSamples;			// Число пропускаемых отсчётов в начале строки (0-65535) 
	U32  GetSamples;			// Число получаемых отсчетов в строке (0-65535)
	U32  DelayStart;			// Задержка старта  в микросекундах
} BRD_SetFrame, *PBRD_SetFrame;
//---------------------------------------------------------------------------------------
// for set  SETSTARTMODE mode function
typedef struct _BRD_StartMode_ddc 
{
	U32			startSrc;			// Источник старта: 0 - внутренний, 1 - внешний
	U32			startInv;			// 1 - инверсия сигнала старта
	U32			restartNco;			// 1 - автоматически обнулять фазу NCO DDC при старте
	U32			restartCic;			// 1 - автоматически обнулять фазу CIC DDC при старте
	U32			restartFir;			// 1 - автоматически обнулять фазу FLT DDC при старте
} BRD_StartMode_ddc, *PBRD_StartMode_ddc;
//--------------------------------------------------------------------------------------
typedef struct _BRD_SetCorrectExt 
{
	U32			correctOn;			// 1 - включить коррекцию(умножать выход на корректирующие коэффициенты)
	U32			rcoef[32];			// Действительные части корректирующих коэффициенто для 32-х каналов (16р)
	U32			icoef[32];			// Мнимые  части корректирующих коэффициенто для 32-х каналов (16р)
} BRD_SetCorrectExt, *PBRD_SetCorrectExt;
//--------------------------------------------------------------------------------------
// for  SetDetect
typedef struct _BRD_SetDetect 
{
	U32			porog;			// Порог отклонения частоты тактирования: 1 - 15 (В данной прошивке всегда 15!)
	U32			mode;			// Режим  детектора тактовой: 0 - автомат, 1 - по команде
} BRD_SetDetect, *PBRD_SetDetect;
//--------------------------------------------------------------------------------------
// for  SetLed
typedef struct _BRD_SetLed 
{
	U32			control;		// Упрвление:0 - программное, 1 - от выхода детектора тактовой
	U32			level;			// Состояние светодиода при программном управлении: 0 - "не горит", 1 - "горит";
} BRD_SetLed, *PBRD_SetLed;
//--------------------------------------------------------------------------------------
// for  GetPower
typedef struct _BRD_GetPower
{	
	    float	 v_3_3vx;	// Напряжение цепи +3.3 VX		
	    float	 v_5_0vx;	// Напряжение цепи +5.0 VX		
	    float	 v_12_0vx;	// Напряжение цепи +12.0 VX		
		float	 i_5_0vx;	// Ток в цепи +5VX	
		float	 i_3_3vx;	// Ток в цепи +3.3VX	
		float	 i_12_0vx;	// Ток в цепи +12VX	
		float	 p_5_0vx;	// Потребляемая мощьность по  цепи +5VX	
		float	 p_3_3vx;	// Потребляемая мощьность по  +3.3VX	
		float	 p_12_0vx;	// Потребляемая мощьность по  +12VX	
	    float	 v_1_0v;	// Напряжение цепи +1.0 V		
		float	 v_1_8v;	// Напряжение цепи +1.8 V	
		float	 v_2_5v;	// Напряжение цепи +2.5V	
		float	 v_3_3vcco;	// Напряжение цепи +3.3VCCO
		float	 v_3_3va;	// Напряжение цепи +3.3VA
		float	 v_5_0va;	// Напряжение цепи +5.0VA	
		float	 v_12_0va;	// Напряжение цепи +12VA	
		float	 Tc;		// Температура С	
} BRD_GetPower,*PBRD_GetPower;	

#pragma pack(pop)    

#endif // _CTRL_RFDR4_DDC_H

//
// End of file
//
