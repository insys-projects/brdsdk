/*
 ****************** File CtrlDdc.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : DDC section
 *
 * (C) InSys by Dorokhin Andrey Jan, 2006
 *
 * 12.10.2006 - submodule ver 3.0 - synchronous mode
 *
 ************************************************************
*/

#ifndef _CTRL_DDC_H
 #define _CTRL_DDC_H

#include "ctrladmpro.h"

enum {
	BRDctrl_DDC_SETCHANMASK		= BRDctrl_DDC + 0,	// Set mask of DDC channels (U32)
	BRDctrl_DDC_GETCHANMASK		= BRDctrl_DDC + 1,	// Get mask of DDC channels (U32)
	BRDctrl_DDC_SETFORMAT		= BRDctrl_DDC + 8,	// Set format (8/16/24/32-bit) data for DDC (U32)
	BRDctrl_DDC_GETFORMAT		= BRDctrl_DDC + 9,	// Get format (8/16/24/32-bit) data for DDC (U32)
	BRDctrl_DDC_SETRATE			= BRDctrl_DDC + 10,	// Set DDC sampling rate in Hz (REAL64)
	BRDctrl_DDC_GETRATE			= BRDctrl_DDC + 11,	// Get DDC sampling rate in Hz (REAL64)
	BRDctrl_DDC_SETCLKMODE		= BRDctrl_DDC + 12,	// Set DDC clock mode (BRD_ClkMode)
	BRDctrl_DDC_GETCLKMODE		= BRDctrl_DDC + 13,	// Get DDC clock mode (BRD_ClkMode)
	BRDctrl_DDC_SETSYNCMODE		= BRDctrl_DDC + 14,	// Set ADM synchronization mode (BRD_SyncMode)
	BRDctrl_DDC_GETSYNCMODE		= BRDctrl_DDC + 15,	// Get ADM synchronization mode (BRD_SyncMode)
	BRDctrl_DDC_SETMASTER		= BRDctrl_DDC + 16,	// Set DDC master/single/slave mode (U32)
	BRDctrl_DDC_GETMASTER		= BRDctrl_DDC + 17,	// Get DDC master/single/slave mode (U32)
	BRDctrl_DDC_SETSTARTMODE	= BRDctrl_DDC + 18, // Set DDC start mode (BRD_StartMode)
	BRDctrl_DDC_GETSTARTMODE	= BRDctrl_DDC + 19, // Set DDC start mode (BRD_StartMode)
	BRDctrl_DDC_SETSTDELAY		= BRDctrl_DDC + 20,	// Set DDC start delay counter (BRD_EnVal)
	BRDctrl_DDC_GETSTDELAY		= BRDctrl_DDC + 21,	// Get DDC start delay counter (BRD_EnVal)
	BRDctrl_DDC_SETACQDATA		= BRDctrl_DDC + 22,	// Set DDC acquisition data counter (BRD_EnVal)
	BRDctrl_DDC_GETACQDATA		= BRDctrl_DDC + 23,	// Get DDC acquisition data counter (BRD_EnVal)
	BRDctrl_DDC_SETSKIPDATA		= BRDctrl_DDC + 24,	// Set DDC skipped data counter (BRD_EnVal)
	BRDctrl_DDC_GETSKIPDATA		= BRDctrl_DDC + 25,	// Get DDC skipped data counter (BRD_EnVal)
	BRDctrl_DDC_SETINPSRC		= BRDctrl_DDC + 30,	// Set input ADC for DDC (BRD_EnVal) 
	BRDctrl_DDC_GETINPSRC		= BRDctrl_DDC + 31,	// Get input ADC for DDC (BRD_EnVal)
	BRDctrl_DDC_SETFC			= BRDctrl_DDC + 32,	// Set FC of DDC (BRD_ValChan)
	BRDctrl_DDC_GETFC			= BRDctrl_DDC + 33,	// Get FC of DDC (BRD_ValChan)
	BRDctrl_DDC_SETPROGRAM		= BRDctrl_DDC + 34,	// Set program DDC mode (BRD_DdcProgram)
	BRDctrl_DDC_SETDDCSYNC		= BRDctrl_DDC + 36,	// Set DDC synchronization mode (BRD_DdcSync)
	BRDctrl_DDC_GETDDCSYNC		= BRDctrl_DDC + 37,	// Get DDC synchronization mode (BRD_DdcSync)
	BRDctrl_DDC_SETDECIM		= BRDctrl_DDC + 38,	// Set DDC decimation (BRD_ValChan)
	BRDctrl_DDC_GETDECIM		= BRDctrl_DDC + 39,	// Get DDC decimation (BRD_ValChan)
	BRDctrl_DDC_SETTARGET		= BRDctrl_DDC + 40,	// Set DDC data target (U32)
	BRDctrl_DDC_GETTARGET		= BRDctrl_DDC + 41,	// Get DDC data target (U32)
	BRDctrl_DDC_SETFRAME		= BRDctrl_DDC + 42,	// Set DDC data frame length (U32)
	BRDctrl_DDC_GETFRAME		= BRDctrl_DDC + 43,	// Get DDC data frame length (U32)
	BRDctrl_DDC_GETCFG			= BRDctrl_DDC + 55,	// Get DDC configuration (PBRD_DdcCfg)
	BRDctrl_DDC_WRITEINIFILE	= BRDctrl_DDC + 56,	// Write all DDC properties into ini file (PBRD_IniFile)
	BRDctrl_DDC_READINIFILE		= BRDctrl_DDC + 57,	// Read all DDC properties from ini file (PBRD_IniFile)
	BRDctrl_DDC_SETTESTMODE		= BRDctrl_DDC + 58,	// Set DDC test mode (U32)
	BRDctrl_DDC_GETTESTMODE		= BRDctrl_DDC + 59,	// Get DDC test mode (U32)
	BRDctrl_DDC_SETSPECIFIC		= BRDctrl_DDC + 60,	// Set DDC specific parameters ()
	BRDctrl_DDC_GETSPECIFIC		= BRDctrl_DDC + 61,	// Get DDC specific parameters ()
	BRDctrl_DDC_SETTITLEMODE	= BRDctrl_DDC + 66, // Set Title mode (BRD_EnVal)
	BRDctrl_DDC_GETTITLEMODE	= BRDctrl_DDC + 67, // Get Title mode (BRD_EnVal)
	BRDctrl_DDC_SETTITLEDATA	= BRDctrl_DDC + 68,	// Set data for title programming register (U32)
	BRDctrl_DDC_GETTITLEDATA	= BRDctrl_DDC + 69,	// Get data for title programming register (U32)
	
	BRDctrl_DDC_GETDLGPAGES		= BRDctrl_DDC + 127, // get property pages of DDC for dialog (PBRD_PropertyList)

	BRDctrl_DDC_FIFORESET		= BRDctrl_DDC + 128,	// DDC FIFO reset (NULL)
	BRDctrl_DDC_ENABLE			= BRDctrl_DDC + 129,	// DDC enable / disable (U32)
	BRDctrl_DDC_FIFOSTATUS		= BRDctrl_DDC + 130,	// Get DDC FIFO status (U32)
	BRDctrl_DDC_GETDATA			= BRDctrl_DDC + 131,	// Get data buffer from DDC FIFO (BRD_DataBuf)
	BRDctrl_DDC_GETSRCSTREAM		= BRDctrl_DDC + 138,	// Get source address for stream (U32)
	BRDctrl_DDC_PREPARESTART		= BRDctrl_DDC + 143,	// Called before first BRDctrl_DDC_ENABLE (NULL)

	BRDctrl_DDC_ILLEGAL
};

#pragma pack(push, 1)

// for DDC program command
typedef struct _BRD_DdcProgram
{
	U32		mask;		// mask of DDC (IN)
	U32		num;		// number of records (IN)
	U32*	pRecords;	// pointer on records buffer (IN)
} BRD_DdcProgram, *PBRD_DdcProgram;

// for set and get DDC Syncronization mode
typedef struct _BRD_DdcSync
{
	U32		mode;		// Syncronization mode (IN)
	U32		prog;		// 1 - run program Syncronization (IN)
	U32		async;		// 1 - Asyncronization mode (IN)
} BRD_DdcSync, *PBRD_DdcSync;

// programming input source
//typedef struct _BRD_AdcInpSrc
//{
//	U32		source;		// Source select
//	REAL64	voltage;	// Voltage (ADM12x1M/3M/10M, ADM416x200)
//	U32		chan;		// Channel number (ADM216x2M5)
//} BRD_AdcInpSrc, *PBRD_AdcInpSrc;

// getting configuration
typedef struct _BRD_DdcCfg
{
	U32		Bits;						// разрядность
	U32		Encoding;					// тип кодировки (0 - прямой, 1 - двоично-дополнительный, 2 - код Грея)
	U32		DdcCnt;						// количество каналов DDC
	U32		FifoSize;					// размер FIFO DDC (в байтах)
	U32		AdmConst;					// информация о прошивке субмодуля
} BRD_DdcCfg, *PBRD_DdcCfg;

// programming specific parameters
typedef struct _BRD_DdcSpec
{
	U32		command;	// command number
	PVOID	arg;		// pointer of argument
} BRD_DdcSpec, *PBRD_DdcSpec;

// DDC error codes
enum {
	BRDerr_DDC_ILLEGAL_CHANNEL		=	BRDctrl_DDC + 0,
	BRDerr_DDC_INVALID_WRRDREG		=	BRDctrl_DDC + 1,
	BRDerr_DDC_PRGFILE_NOT			= 	BRDctrl_DDC + 2,
	BRDerr_DDC_INVALID_PRGFLCLK		= 	BRDctrl_DDC + 3,
	BRDerr_DDC_INVALID_PRGFLASYNC	= 	BRDctrl_DDC + 4,
	BRDerr_DDC_INVALID_MODEOFDDC	= 	BRDctrl_DDC + 5,
	BRDerr_DDC_UNDEFINED
};

#pragma pack(pop)    

#endif // _CTRL_DDC_H

//
// End of file
//