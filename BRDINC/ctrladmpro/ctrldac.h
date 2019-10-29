/*
 ****************** File CtrlDac.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : DAC section
 *
 * (C) InSys by Dorokhin Andrey Oct, 2005
 *
 ************************************************************
*/

#ifndef _CTRL_DAC_H
 #define _CTRL_DAC_H

#include "ctrladmpro.h"

enum {
	BRDctrl_DAC_SETCHANMASK		= BRDctrl_DAC + 0,	// Set mask of DAC channels (U32)
	BRDctrl_DAC_GETCHANMASK		= BRDctrl_DAC + 1,	// Get mask of DAC channels (U32)
	BRDctrl_DAC_SETGAIN			= BRDctrl_DAC + 2,	// Set gain of the DAC channel (BRD_ValChan)
	BRDctrl_DAC_GETGAIN			= BRDctrl_DAC + 3,	// Get gain of the DAC channel (BRD_ValChan)
	BRDctrl_DAC_SETOUTRANGE		= BRDctrl_DAC + 4,	// Set output range of the DAC channel in Volt (BRD_ValChan)
	BRDctrl_DAC_GETOUTRANGE		= BRDctrl_DAC + 5,	// Get output range of the DAC channel in Volt (BRD_ValChan)
	BRDctrl_DAC_SETBIAS			= BRDctrl_DAC + 6,	// Set bias of the DAC channel (BRD_ValChan)
	BRDctrl_DAC_GETBIAS			= BRDctrl_DAC + 7,	// Get bias of the DAC channel (BRD_ValChan)
	BRDctrl_DAC_SETFORMAT		= BRDctrl_DAC + 8,	// Set format (8/16/24/32-bit) data for DAC (U32)
	BRDctrl_DAC_GETFORMAT		= BRDctrl_DAC + 9,	// Get format (8/16/24/32-bit) data for DAC (U32)
	BRDctrl_DAC_SETRATE			= BRDctrl_DAC + 10,	// Set DAC sampling rate in Hz (REAL64)
	BRDctrl_DAC_GETRATE			= BRDctrl_DAC + 11,	// Get DAC sampling rate in Hz (REAL64)
	BRDctrl_DAC_SETCLKMODE		= BRDctrl_DAC + 12,	// Set DAC clock mode (BRD_ClkMode)
	BRDctrl_DAC_GETCLKMODE		= BRDctrl_DAC + 13,	// Get DAC clock mode (BRD_ClkMode)
	BRDctrl_DAC_SETSYNCMODE		= BRDctrl_DAC + 14,	// Set DAC synchronization mode (BRD_SyncMode)
	BRDctrl_DAC_GETSYNCMODE		= BRDctrl_DAC + 15,	// Get DAC synchronization mode (BRD_SyncMode)
	BRDctrl_DAC_SETMASTER		= BRDctrl_DAC + 16,	// Set DAC master/single/slave mode (U32)
	BRDctrl_DAC_GETMASTER		= BRDctrl_DAC + 17,	// Get DAC master/single/slave mode (U32)
	BRDctrl_DAC_SETSTARTMODE	= BRDctrl_DAC + 18, // Set DAC start mode (BRD_StartMode)
	BRDctrl_DAC_GETSTARTMODE	= BRDctrl_DAC + 19, // Set DAC start mode (BRD_StartMode)
	BRDctrl_DAC_SETSTDELAY		= BRDctrl_DAC + 20,	// Set DAC start delay counter (U32)
	BRDctrl_DAC_GETSTDELAY		= BRDctrl_DAC + 21,	// Get DAC start delay counter (U32)
	BRDctrl_DAC_SETOUTDATA		= BRDctrl_DAC + 22,	// Set DAC output data counter (U32)
	BRDctrl_DAC_GETOUTDATA		= BRDctrl_DAC + 23,	// Get DAC output data counter (U32)
	BRDctrl_DAC_SETSKIPDATA		= BRDctrl_DAC + 24,	// Set DAC skipped data counter (U32)
	BRDctrl_DAC_GETSKIPDATA		= BRDctrl_DAC + 25,	// Get DAC skipped data counter (U32)
	BRDctrl_DAC_SETCYCLMODE		= BRDctrl_DAC + 26,	// Set DAC cycling mode (U32)
	BRDctrl_DAC_GETCYCLMODE		= BRDctrl_DAC + 27,	// Get DAC cycling mode (U32)
	BRDctrl_DAC_SETRESIST		= BRDctrl_ADC + 28,	// Set input/output resistance of the DAC channels and other lines (BRD_UvalChan)
	BRDctrl_DAC_GETRESIST		= BRDctrl_ADC + 29,	// Get input/output resistance of the DAC channels and other lines (BRD_UvalChan)
	BRDctrl_DAC_SETSOURCE		= BRDctrl_DAC + 40,	// Set DAC data source (U32)
	BRDctrl_DAC_GETSOURCE		= BRDctrl_DAC + 41,	// Get DAC data source (U32)
	BRDctrl_DAC_SELFCLBR		= BRDctrl_DAC + 46,	// Execute self-calibration (None)
	BRDctrl_DAC_SETCODE			= BRDctrl_DAC + 52,	// Set code type (0 - two's complement, 1 - floating point, 2 - straight binary, 7 - Gray code) data for ADC (U32)
	BRDctrl_DAC_GETCODE			= BRDctrl_DAC + 53,	// Get code type (0 - two's complement, 1 - floating point, 2 - straight binary, 7 - Gray code) data for ADC (U32)
	BRDctrl_DAC_GETCFG			= BRDctrl_DAC + 55,	// Get DAC configuration (PBRD_DacCfg)
	BRDctrl_DAC_WRITEINIFILE	= BRDctrl_DAC + 56,	// Write all DAC properties into ini file (PBRD_IniFile)
	BRDctrl_DAC_READINIFILE		= BRDctrl_DAC + 57,	// Read all DAC properties from ini file (PBRD_IniFile)
	BRDctrl_DAC_SETTESTMODE		= BRDctrl_DAC + 58,	// Set DAC test mode (U32)
	BRDctrl_DAC_GETTESTMODE		= BRDctrl_DAC + 59,	// Get DAC test mode (U32)
	BRDctrl_DAC_SETSPECIFIC		= BRDctrl_DAC + 60,	// Set DAC specific parameters ()
	BRDctrl_DAC_GETSPECIFIC		= BRDctrl_DAC + 61,	// Get DAC specific parameters ()

	BRDctrl_DAC_SETCLKREFMODE	= BRDctrl_DAC + 62,	// Set DAC clock and ref mode (PBRD_ClkRefMode)
	BRDctrl_DAC_GETCLKREFMODE	= BRDctrl_DAC + 63,	// Get DAC clock and ref mode (PBRD_ClkRefMode)
	BRDctrl_DAC_SETDIVCLK		= BRDctrl_DAC + 64,	// Set divider of clock (PBRD_DivClk)
	BRDctrl_DAC_GETDIVCLK		= BRDctrl_DAC + 65,	// Get divider of clock (PBRD_DivClk)
	BRDctrl_DAC_WRDELAY			= BRDctrl_DAC + 66,	// Write delay value (BRD_StdDelay)
	BRDctrl_DAC_RDDELAY			= BRDctrl_DAC + 67,	// Read delay value (BRD_StdDelay)
	BRDctrl_DAC_RSTDELAY		= BRDctrl_DAC + 68,	// Reset delay value (BRD_StdDelay)

	BRDctrl_DAC_GETDLGPAGES		= BRDctrl_DAC + 127, // get property pages of DAC for dialog (PBRD_PropertyList)

	BRDctrl_DAC_FIFORESET		= BRDctrl_DAC + 128,	// DAC FIFO reset (NULL)
	BRDctrl_DAC_ENABLE			= BRDctrl_DAC + 129,	// DAC enable / disable (NULL)
	BRDctrl_DAC_FIFOSTATUS		= BRDctrl_DAC + 130,	// Get DAC FIFO status (U32)
	BRDctrl_DAC_PUTDATA			= BRDctrl_DAC + 131,	// Put data buffer into DAC FIFO (PVOID)
//	BRDctrl_DAC_ISBITSUNDERFLOW = BRDctrl_DAC + 132,	// Check underflow of DAC bits (U32)
	BRDctrl_DAC_OUTSYNC 		= BRDctrl_DAC + 137,	// Out sync for DAC (NULL)
	BRDctrl_DAC_GETSRCSTREAM	= BRDctrl_DAC + 138,	// Get source address for stream (U32)

	BRDctrl_DAC_SETINTERP		= BRDctrl_DAC + 139,	// Set divider of clock (PBRD_DivClk)
	BRDctrl_DAC_GETINTERP		= BRDctrl_DAC + 140,	// Get divider of clock (PBRD_DivClk)
	BRDctrl_DAC_PREPARESTART		= BRDctrl_DAC + 143,	// Called before first BRDctrl_DAC_ENABLE (NULL)

	BRDctrl_DAC_ILLEGAL
};

#pragma pack(push, 1)    

// for set and get Start mode command
typedef struct
{
	BRD_StartMode	stndStart;	// Standard Start mode
	U32		src;			// Start source
	U32		inv;			// Start inversion 
	U32		resist;			// Start Input Resistance 
	REAL64	level;			// Start level (Comparator Threshold value)
} BRD_DacStartMode, *PBRD_DacStartMode;

// getting configuration
typedef struct _BRD_DacCfg
{
	U32		Bits;						// разрядность
	U32		Encoding;					// тип кодировки (0 - прямой, 1 - двоично-дополнительный, 2 - код Грея)
	U32		MinRate;					// минимальная частота дискретизации
	U32		MaxRate;					// максимальная частота дискретизации
	U32		OutRange;					// выходной диапазон
	U32		FifoSize;					// размер FIFO ЦАП (в байтах)
	U32		MaxChan;					// максимальное число каналов
} BRD_DacCfg, *PBRD_DacCfg;

// programming specific parameters
typedef struct _BRD_DacSpec
{
	U32		command;	// command number
	PVOID	arg;		// pointer of argument
} BRD_DacSpec, *PBRD_DacSpec;


// DAC error codes
enum {
	BRDerr_DAC_ILLEGAL_CHANNEL		=	BRDctrl_DAC + 0,
	BRDerr_DAC_ILLEGAL_GAIN			=	BRDctrl_DAC + 1,
	BRDerr_DAC_ILLEGAL_OUTRANGE		=	BRDctrl_DAC + 2,
	BRDerr_DAC_UNDEFINED
};

#pragma pack(pop)    

#endif // _CTRL_DAC_H

//
// End of file
//
