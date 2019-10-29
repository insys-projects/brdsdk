/*
 ****************** File ctrms.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants for  ms service
 *
 * (C) InSys by Sklyarov A. Feb,2016
 *
 *
 ************************************************************
*/

#ifndef _CTRL_MS_H
 #define _CTRL_MS_H


#include "ctrladmpro.h"
enum {
	
	BRDctrl_MS_SETCLK			= BRDctrl_DAQ + 0,	// Установить режим тактирования(PBRD_SetClk)
	BRDctrl_MS_SELOUTBUF		= BRDctrl_DAQ + 1,	// Выбрать буфер вывода сигнала (0 или 1) (U32*)
	BRDctrl_MS_SETSYNC			= BRDctrl_DAQ + 2,	// Установить режим межкомплектной синхронизации (PBRD_SetSync)
	BRDctrl_MS_SETCODEG			= BRDctrl_DAQ + 3,	// Установить параметры кодограммы (PBRD_SetCodeg)
	BRDctrl_MS_LOADSIGNAL		= BRDctrl_DAQ + 4,	// Загрузить отсчёты сигнала  в память (PBRD_LoadSignal)
	BRDctrl_MS_GETSRCSTREAM		= BRDctrl_DAQ + 5,	// Получить адрес источника данных для стрима (*U32)
	BRDctrl_MS_FIFORESET		= BRDctrl_DAQ + 6,	// Сброс FIFO (NULL)
	BRDctrl_MS_SETOUTMODE		= BRDctrl_DAQ + 7,	// Установить рехим вывода данных из памяти(*U32)  (0-ретрансмит, 1-рестарт)
	BRDctrl_MS_START			= BRDctrl_DAQ + 8,	// Старт(NULL)
	BRDctrl_MS_STOP				= BRDctrl_DAQ + 9,	// Стоп (NULL)
	BRDctrl_MS_FIFOSTATUS		= BRDctrl_DAQ + 10,	// Получить статус FIFO (*U32)
	BRDctrl_MS_DACRESET			= BRDctrl_DAQ + 11,	// Сброс микросхемы ЦАП(NULL)
	BRDctrl_MS_SETDUC			= BRDctrl_DAQ + 12,	// Установить режим работы DUC(PBRD_SetDuc)
	BRDctrl_MS_STARTCODEG		= BRDctrl_DAQ + 13,	// Старт кодограммы(NULL)
	BRDctrl_MS_STOPCODEG		= BRDctrl_DAQ + 14,	// Стop кодограммы(NULL)
	BRDctrl_MS_SETMASTER		= BRDctrl_DAQ + 16,	// Установить мастер режим
	BRDctrl_MS_SETDACREG		= BRDctrl_DAQ + 17,	// Записать данные в регистр микросхемы ЦАП (PBRD_SetDacReg)
	BRDctrl_MS_OUTSYNC			= BRDctrl_DAQ + 18,	// Выдать импульс синхронизации (NULL)
	BRDctrl_MS_INITPOWER		= BRDctrl_DAQ + 19,	// Инициализировать систему мониторинга питания(U32*)
	BRDctrl_MS_GETPOWER			= BRDctrl_DAQ + 20,	// Мониторинг питания: получить значения напряжений питания и токов(PBRD_GetPower)
	BRDctrl_MS_INITPOWSIG		= BRDctrl_DAQ + 21,	// Инициализировать систему мониторинга мощности сигналов(PBRD_InitPowSig)
	BRDctrl_MS_GETPOWSIG		= BRDctrl_DAQ + 22,	// Мониторинг мощности сигналов: получить значения мощности сигналов(PBRD_GetPowSig)
	BRDctrl_MS_TESTSTROB		= BRDctrl_DAQ + 23,	// Тест стробов: установить  значения на выходах SYNC1-SYNC4, STB1-STB3  и считать состояние выходов (*U32)
	BRDctrl_MS_SETLED	 	    = BRDctrl_DAQ + 24,	// Управление светодиодом (правый верхний)(*U32)
	BRDctrl_MS_ILLEGAL
	};

#pragma pack(push, 1)  

//-----------------------------------------------------------------------------------------------
typedef struct _BRD_SetClk
{	
		U32		ClkSrc;		// Источник тактового сигнала: 0-выключен, 1-внутренний, 2-внешний, 
		U32		ClkValue;
		U32		DacDelay;	// Задержка данных при выдаче в ЦАП (0-31)
} BRD_SetClk,*PBRD_SetClk;	
//----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
typedef struct _BRD_SetSync
{	
		U32		SyncMode;	// Режим синхронизации: 0 - ведомый, комлект работает от Тр мастер-комплекта
							//					    1 - ведомый, комплект только подсинхронизирует собственное Тр от Тр мастер-комплекта 	
							//					    2 - ведущий, комплект всегда выдаёт Тр
							//						3 - ведущий, комплект  выдаёт Тр при поступлении команды 
		U32		NumSync;	// Номер входа синхронизации: 1..4
		U32		BlockSync;	// 1 - блокировка выдычи синхростробов в ведущем  режиме ( специальный режим)
} BRD_SetSync, *PBRD_SetSync;	
//----------------------------------------------------------------------------------------------
// for  Set Codegram function
typedef  struct _BRD_SetCodeg
{	
	    U32  Period_Tp;			//  Период повторения кодограммы:   (0..20000 мкс)		
		U32	 Length_Tp;			//  Длительность сигнала Tp (STR):  (0..20000 мкс)
		U32	 Delay_STB1;		//  Задержка сигнала STB1:		    (0..20000 мкс)
		U32  Length_STB1;		//  Длительность сигнала STB1:		(0..20000 мкс)
		U32  Delay_STB2;		//  Задержка сигнала STB2:			(0..20000 мкс)
		U32  Length_STB2;		//  Длительность сигнала STB2:      (0..20000 мкс)
		U32  Delay_STB3;		//  Задержка сигнала STB3:			(0..20000 мкс)
		U32  Length_STB3;		//  Длительность сигнала STB3:      (0..20000 мкс)
		U32  Delay_Tp2;			// Задержка Тп2 относительно Тп1, отсчётов DUC. Подстроечный параметр,( 0 - 64)
		U32  Lrst;				//  Длительность сигнала рестарта в тактах
	    double  SampleRate;		//  Частота дискретизации ЦАП (Гц)
} BRD_SetCodeg,*PBRD_SetCodeg;	
//----------------------------------------------------------------------------------------------
// for  Set Signal
typedef struct _BRD_LoadSignal
{	
	U32	 sel_buf;			// Выбор буфера записи: 0,1
    U32	 Carrier1;			// Первая несущая частота (Гц) 
    U32  Carrier2;			// Вторая несущая частота (Гц)
	U32	 DelayCarrier2;		// Смена частоты(Carrier1 меняется на Carrier2) от начала кадра : 0..26214 мкс
    double  SampleRate;		// Частота дискретизации ЦАП (Гц)
	U32  *samples;			// Указатель на буфер отсчётов
	U32	 nsamples;			// Числом записываемых отсчётов
} BRD_LoadSignal,*PBRD_LoadSignal;	
//----------------------------------------------------------------------------------------------
// for  Set Duc
typedef struct _BRD_SetDuc
{	
	    U32  Mode;				// Режим работы определяет сигнал на выходе DUC :
								// 0 -  сигнал с выхода CIC-фильтра DUC  (отладка)
								// 1 - сигнал с выхода квадратурного генератора (NCO ) DUC  (тестовый сигнал)
								// 2 - сигнал выходного комплексного умножителя DUC  (зондирующий сигнал)
		U32	 DistSignal;		// Направление сигнала DUC:  0 - ЦАП 0 (зондирующий), 1 - ЦАП 1 (тестовый), на ЦАП1 и ЦАП2
} BRD_SetDuc,*PBRD_SetDuc;	
//------------------------------
// for  Set DucStartMode
typedef struct _BRD_DucStartMode
{	
	    U32  startSrc;		// Источнок старта		
		U32	 startInv;		// Инверсия старта
} BRD_DucStartMode,*PBRD_DucStartMode;

// for  SetDacReg
typedef struct _BRD_SetDacReg
{	
	    U32  regNum;		// номер записываемого регистра		
		U32	 regVal;		// данные для записи
} BRD_SetDacReg,*PBRD_SetDacReg;

//----------------------------------------------------------------

// for  GetPower
typedef struct _BRD_MsGetPower
{	
	    float	 v_3_3vx;	// Напряжение цепи +3.3 VX		
	    float	 v_5_0vx;	// Напряжение цепи +5.0 VX		
	    float	 v_12_0vx;	// Напряжение цепи +12.0 VX		
		float	 i_5_0vx;		// Ток в цепи +5VX	
		float	 i_3_3vx;	// Ток в цепи +3.3VX	
		float	 i_12_0vx;	// Ток в цепи +12VX	
		float	 p_5_0vx;	// Потребляемая мощьность по  цепи +5VX	
		float	 p_3_3vx;	// Потребляемая мощьность по  +3.3VX	
		float	 p_12_0vx;	// Потребляемая мощьность по  +12VX	
	    float	 v_1_0v;	// Напряжение цепи +1.0 V		
		float	 v_1_8v;	// Напряжение цепи +1.8 V	
		float	 v_dvdv18;	// Напряжение цепи DVDD18	
		float	 v_3_3vcco;	// Напряжение цепи +3.3VCCO
		float	 v_3_3va;	// Напряжение цепи +3.3VA	
		float	 v_5_0va;		// Напряжение цепи +5VA	
		float	 Tc;		// Температура С	

} BRD_MsGetPower,*PBRD_MsGetPower;	
//----------------------------------------------------------------------------------------------
typedef struct _BRD_GetPowSig
{	
	   float	VrmsZS;		// 
	   float	VrmsCLBR;		// 
	   float	VrmsCLK;		// 
} BRD_GetPowSig,*PBRD_GetPowSig;
//-------------------------------------------------------------------
typedef struct _BRD_InitPowSig
{	
	    U32  rcfg0;		// 
	    U32  rcfg1;		// 	
	    U32  rcfg2;		// 
} BRD_InitPowSig, *PBRD_InitPowSig;
#pragma pack(pop)    

#endif // _CTRL_MS_H

//
// End of file
//
