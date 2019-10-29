/*
 ****************** File CtrlGc5016.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : GC5016 section
 *
 * (C) InSys by Sklyarov A. Nov, 2006
 *
 *
 ************************************************************
*/

#ifndef _CTRL_GC5016_H
 #define _CTRL_GC5016_H


#include "ctrladmpro.h"
enum {
	
	BRDctrl_GC5016_SETMASTER		= BRDctrl_GC5016 + 0,	// Set GC5016 master/single/slave mode (U32)
	BRDctrl_GC5016_GETMASTER		= BRDctrl_GC5016 + 1,	// Get GC5016 master/single/slave mode (U32)
	BRDctrl_GC5016_SETCHANMASK		= BRDctrl_GC5016 + 2,	// Set mask of GC5016 channels (U32)
	BRDctrl_GC5016_GETCHANMASK		= BRDctrl_GC5016 + 3,	// Get mask of GC5016 channels (U32)
	BRDctrl_GC5016_SETCLKMODE		= BRDctrl_GC5016 + 4,	// Set source of clock GC5016 ((BRD_ClkMode))
	BRDctrl_GC5016_GETCLKMODE		= BRDctrl_GC5016 + 5,	// Get source of clock GC5016 (BRD_(BRD_ClkMode)
	BRDctrl_GC5016_SETSTARTMODE		= BRDctrl_GC5016 + 6,   // Set GC5016 start mode (BRD_StartMode)
	BRDctrl_GC5016_GETSTARTMODE		= BRDctrl_GC5016 + 7,   // Get GC5016 start mode (BRD_StartMode)
	BRDctrl_GC5016_FIFORESET		= BRDctrl_GC5016 + 8,	// GC5016 FIFO reset (NULL)
	BRDctrl_GC5016_START			= BRDctrl_GC5016 + 9,	// GC5016 Start(NULL)
	BRDctrl_GC5016_STOP				= BRDctrl_GC5016 + 10,	// Gc5016 Stop (NULL)
	BRDctrl_GC5016_FIFOSTATUS		= BRDctrl_GC5016 + 11,	// Get GC5015 FIFO status (U32)
	BRDctrl_GC5016_GETDATA			= BRDctrl_GC5016 + 12,	// Get data buffer from GC5016 FIFO (BRD_DataBuf)
	BRDctrl_GC5016_SETTARGET		= BRDctrl_GC5016 + 13,  // Set GC5016 data target (U32)
	BRDctrl_GC5016_GETTARGET		= BRDctrl_GC5016 + 14,  // Get GC5016 data target (U32)
	BRDctrl_GC5016_GETSRCSTREAM		= BRDctrl_GC5016 + 15,	// Get source address for stream (U32)
	BRDctrl_GC5016_GCSETREG			= BRDctrl_GC5016 + 16,	// Set register GC5016(BRD_GcSetReg)
	BRDctrl_GC5016_GCGETREG			= BRDctrl_GC5016 + 17,	// Get register GC5016(BRD_GcSetReg)
	BRDctrl_GC5016_SETADCGAIN		= BRDctrl_GC5016 + 18,	// Set gain ADC GC5016(BRD_AdcSetGain)
	BRDctrl_GC5016_GETADCOVER		= BRDctrl_GC5016 + 19,	// Get Adc Over GC5016(U32)
	BRDctrl_GC5016_ADCENABLE		= BRDctrl_GC5016 + 20,	// Enable Adc GC5016(U32)
	BRDctrl_GC5016_SETDDCFC			= BRDctrl_GC5016 + 21,	// Set Fc of DDC GC5016(U32)
	BRDctrl_GC5016_SETADMMODE		= BRDctrl_GC5016 + 23,	// Set mode of ADM:0-Adc,1- Ddc(U32)
	BRDctrl_GC5016_SETDDCMODE		= BRDctrl_GC5016 + 24,	// Set mode of DDC:0-FullRata,1-SplitIQ,2-Double Rate(U32)
	BRDctrl_GC5016_SETDDCSYNC		= BRDctrl_GC5016 + 25,	// Set mode of syncro GC5016(BRD_DdcSetSync)
	BRDctrl_GC5016_PROGRAMDDC		= BRDctrl_GC5016 + 26,	// Programming of chip DDC(BRD_DdcProgram)
	BRDctrl_GC5016_STARTDDC			= BRDctrl_GC5016 + 27,	// Start DDC(NULL)
	BRDctrl_GC5016_STOPDDC			= BRDctrl_GC5016 + 28,	// Stop DDC(NULL)
	BRDctrl_GC5016_SETTESTMODE		= BRDctrl_GC5016 + 29,	// Set GC5016 test mode (U32)
	BRDctrl_GC5016_GETTESTMODE		= BRDctrl_GC5016 + 30,	// Get GC5016 test mode (U32)
	BRDctrl_GC5016_SETSTDELAY		= BRDctrl_GC5016 + 31,	// Set GC5016 start delay counter (BRD_EnVal)
	BRDctrl_GC5016_GETSTDELAY		= BRDctrl_GC5016 + 32,	// Get GC5016 start delay counter (BRD_EnVal)
	BRDctrl_GC5016_SETACQDATA		= BRDctrl_GC5016 + 33,	// Set GC5016 acquisition data counter (BRD_EnVal)
	BRDctrl_GC5016_GETACQDATA		= BRDctrl_GC5016 + 34,	// Get GC5016 acquisition data counter (BRD_EnVal)
	BRDctrl_GC5016_SETSKIPDATA		= BRDctrl_GC5016 + 35,	// Set GC5016 skipped data counter (BRD_EnVal)
	BRDctrl_GC5016_GETSKIPDATA		= BRDctrl_GC5016 + 36,	// Get GC5016 skipped data counter (BRD_EnVal)
	BRDctrl_GC5016_SETTITLEMODE		= BRDctrl_GC5016 + 37,  // Set Title mode (BRD_EnVal)
	BRDctrl_GC5016_GETTITLEMODE		= BRDctrl_GC5016 + 38,  // Get Title mode (BRD_EnVal)
	BRDctrl_GC5016_SETTITLEDATA		= BRDctrl_GC5016 + 39,	// Set data for title programming register (U32)
	BRDctrl_GC5016_GETTITLEDATA		= BRDctrl_GC5016 + 40,	// Get data for title programming register (U32)
	BRDctrl_GC5016_SETSPECIFIC		= BRDctrl_GC5016 + 41,	// Set GC5016 specific parameters ()
	BRDctrl_GC5016_GETSPECIFIC		= BRDctrl_GC5016 + 42,	// Get GC5016 specific parameters ()
	BRDctrl_GC5016_GETCFG			= BRDctrl_GC5016 + 43,	// Get GC5016 configuration (PBRD_GC5016Cfg)
	BRDctrl_GC5016_PROGRAMDDCEXT	= BRDctrl_GC5016 + 44,	// Programming of chips DDC(BRD_DdcProgramExt)
	BRDctrl_GC5016_STARTDDCEXT		= BRDctrl_GC5016 + 45,	// Start DDC(U32)
	BRDctrl_GC5016_STOPDDCEXT		= BRDctrl_GC5016 + 46,	// Stop DDC(U32)
	BRDctrl_GC5016_SETDDCFCEXT		= BRDctrl_GC5016 + 47,	// Set Fc of DDC GC5016(U32)
	BRDctrl_GC5016_SETFLT			= BRDctrl_GC5016 + 48,	// Set Flt(BRD_SetFlt)
	BRDctrl_GC5016_SETFFT			= BRDctrl_GC5016 + 49,	// Set Fft(BRD_SetFft)
	BRDctrl_GC5016_SETFRAME			= BRDctrl_GC5016 + 50,	// Set frame mode (BRD_SetFrame)
	BRDctrl_GC5016_SETNCHANS		= BRDctrl_GC5016 + 51,	// Set number channels(U32)

	BRDctrl_GC5016_ILLEGAL
};

#pragma pack(push, 1)    

// for Write GC5016Reg and Read GC5016Reg  functions
typedef struct _BRD_GC5016SetReg
{
	U32		page;		// number of page programming GC5016 chip
	U32		reg;		// register address into GC5016 chip for programming
	U32		data;		// data for writing
} BRD_GC5016SetReg, *PBRD_GC5016SetReg;
//----------------------------------------------
// for GC5016 SetSync function
typedef struct _BRD_GC5016SetSync
{
	U32		syncMode,	// mode of syncro (1,2)
			progSync,	// 1-out impulse syncro
			slave,		// 1-slave, 0-master
			resDiv;		// 1-reset divider
} BRD_GC5016SetSync, *PBRD_GC5016SetSync;
//----------------------------------------------

// for  GC5016 AdcSetGain function
typedef struct _BRD_GC5016AdcSetGain
{	
	U32 maskAdc,	 // mask for programming Adc chips (1,2 or 3)
		gainAdc;	 // gain Adc (0,1) 
} BRD_GC5016AdcSetGain,*PBRD_GC5016AdcSetGain;

// for  Set Gc5016 Set Fc function
typedef struct _BRD_GC5016SetFc
{	
	U32 maskChans,	 // mask channels for programming 
			   Fc;	 // Fc DDC (Hz) 
} BRD_GC5016SetFc,*PBRD_GC5016SetFc;

// for  ProgramDdc function
typedef struct _BRD_GC5016Program
{	U32  nItems;	// Number of items,
	U32	 *pData;	// Data for programming
} BRD_GC5016Program, *PBRD_GC5016Program;
// for  ProgramDdc function

typedef struct _BRD_GC5016ProgramExt
{	
	U32  maskDdc;	// Mask of DDC (0x0-0xf)
	U32  nItems;	// Number of items,
	U32  errMask;	// Mask of error prog DDC (0x0-0xf),
	U32	 *pData;	// Data for programming
} BRD_GC5016ProgramExt, *PBRD_GC5016ProgramExt;
// for  ProgramDdcExt function

// programming specific parameters
typedef struct _BRD_GC5016Spec
{
	U32		command;	// command number
	PVOID	arg;		// pointer of argument
} BRD_GC5016Spec, *PBRD_GC5016Spec;

typedef struct _BRD_SetFlt
{	
	U32  enableFlt;	// 0 - disable flt, 1 - enable flt
	U32  sizeFlt;	// size of flt (1..1024),
	U32	 *pData;	// Data for flt
} BRD_SetFlt, *PBRD_SetFlt;
// for  SetFlt function
typedef struct _BRD_SetFft
{	
	U32  enableFft;	// 0 - disable fft, 1 - enable fft
	U32  enableCor;	// 0 - disable correction, 1 - enable correction
	U32  enableOneChan;	// 0 - disable OneChan, 1 - enable OneChan
	U32	 *pWind;	// Data for Window
} BRD_SetFft, *PBRD_SetFft;

typedef struct _BRD_SetFrame
{	
	U32  EnableHeaderString;			// 1- вставлять заголовок в начале кадра
	U32  HeaderString;				// 16 разрядов  - синхрослово заголовка
	U32  SkipSamplesString;			// Число пропускаемых отсчётов в начале кадра (0-65535) 
	U32  GetSamplesString;			// Число получаемых отсчетов в кадре (0-65535)
	U32  EnableDelayStart;			// Включение задержки внешнего стартового импульса 
	U32  DelayStart;			// Задержка внешнего стартового импульса в отсчётах
} BRD_SetFrame, *PBRD_SetFrame;
// Numbers of Specific Command
typedef enum _GC5016_NUM_CMD
{
	GC5016cmd_SUMENABLE	= 64, // 
	GC5016cmd_SUMDISABLE= 65, // 
	GC5016cmd_SUMCOUNT	= 66, // 
	GC5016cmd_SUMSHIFT	= 67, // 
} GC5016_NUM_CMD;
// getting configuration
typedef struct _BRD_GC5016Cfg
{
	U32		SubType;				// тип субмодуля
	U32		SubVer;					// версия субмодуля
	U32		AdmConst;				// информация о прошивке субмодуля
} BRD_GC5016Cfg, *PBRD_GC5016Cfg;

#pragma pack(pop)    

#endif // _CTRL_GC5016_H

//
// End of file
//
