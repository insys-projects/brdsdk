/*
 ****************** File CtrlAdc.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : ADC section
 *
 * (C) InSys by Dorokhin Andrey Jan, 2002
 *
 *	27.11.2003 - add BRDctrl_ADC_SETINPRANGE, BRDctrl_ADC_GETINPRANGE
 *	28.02.2005 - add getting ADC configuration
 *
 ************************************************************
*/

#ifndef _CTRL_ADC_H
 #define _CTRL_ADC_H

#include "ctrladmpro.h"

enum {
	BRDctrl_ADC_SETCHANMASK		= BRDctrl_ADC + 0,	// Set mask of ADC channels (U32)
	BRDctrl_ADC_GETCHANMASK		= BRDctrl_ADC + 1,	// Get mask of ADC channels (U32)
	BRDctrl_ADC_SETGAIN			= BRDctrl_ADC + 2,	// Set gain of the ADC channel (BRD_ValChan)
	BRDctrl_ADC_GETGAIN			= BRDctrl_ADC + 3,	// Get gain of the ADC channel (BRD_ValChan)
	BRDctrl_ADC_SETINPRANGE		= BRDctrl_ADC + 4,	// Set input range of the ADC channel in Volt (BRD_ValChan)
	BRDctrl_ADC_GETINPRANGE		= BRDctrl_ADC + 5,	// Get input range of the ADC channel in Volt (BRD_ValChan)
	BRDctrl_ADC_SETBIAS			= BRDctrl_ADC + 6,	// Set bias of the ADC channel (BRD_ValChan)
	BRDctrl_ADC_GETBIAS			= BRDctrl_ADC + 7,	// Get bias of the ADC channel (BRD_ValChan)
	BRDctrl_ADC_SETFORMAT		= BRDctrl_ADC + 8,	// Set format (8/16/24/32-bit) data for ADC (U32)
	BRDctrl_ADC_GETFORMAT		= BRDctrl_ADC + 9,	// Get format (8/16/24/32-bit) data for ADC (U32)
	BRDctrl_ADC_SETRATE			= BRDctrl_ADC + 10,	// Set ADC sampling rate in Hz (REAL64)
	BRDctrl_ADC_GETRATE			= BRDctrl_ADC + 11,	// Get ADC sampling rate in Hz (REAL64)
	BRDctrl_ADC_SETCLKMODE		= BRDctrl_ADC + 12,	// Set ADC clock mode (BRD_ClkMode)
	BRDctrl_ADC_GETCLKMODE		= BRDctrl_ADC + 13,	// Get ADC clock mode (BRD_ClkMode)
	BRDctrl_ADC_SETSYNCMODE		= BRDctrl_ADC + 14,	// Set ADC synchronization mode (BRD_SyncMode)
	BRDctrl_ADC_GETSYNCMODE		= BRDctrl_ADC + 15,	// Get ADC synchronization mode (BRD_SyncMode)
	BRDctrl_ADC_SETMASTER		= BRDctrl_ADC + 16,	// Set ADC master/single/slave mode (U32)
	BRDctrl_ADC_GETMASTER		= BRDctrl_ADC + 17,	// Get ADC master/single/slave mode (U32)
	BRDctrl_ADC_SETSTARTMODE	= BRDctrl_ADC + 18, // Set ADC start mode (BRD_StartMode)
	BRDctrl_ADC_GETSTARTMODE	= BRDctrl_ADC + 19, // Set ADC start mode (BRD_StartMode)
	BRDctrl_ADC_SETSTDELAY		= BRDctrl_ADC + 20,	// Set ADC start delay counter (BRD_EnVal)
	BRDctrl_ADC_GETSTDELAY		= BRDctrl_ADC + 21,	// Get ADC start delay counter (BRD_EnVal)
	BRDctrl_ADC_SETACQDATA		= BRDctrl_ADC + 22,	// Set ADC acquisition data counter (BRD_EnVal)
	BRDctrl_ADC_GETACQDATA		= BRDctrl_ADC + 23,	// Get ADC acquisition data counter (BRD_EnVal)
	BRDctrl_ADC_SETSKIPDATA		= BRDctrl_ADC + 24,	// Set ADC skipped data counter (BRD_EnVal)
	BRDctrl_ADC_GETSKIPDATA		= BRDctrl_ADC + 25,	// Get ADC skipped data counter (BRD_EnVal)
	BRDctrl_ADC_SETINPRESIST	= BRDctrl_ADC + 26,	// Set input resistance of the ADC channel (BRD_ValChan)
	BRDctrl_ADC_GETINPRESIST	= BRDctrl_ADC + 27,	// Get input resistance of the ADC channel (BRD_ValChan)
	BRDctrl_ADC_SETDCCOUPLING	= BRDctrl_ADC + 28,	// Set DC coupling ADC input (BRD_ValChan)
	BRDctrl_ADC_GETDCCOUPLING	= BRDctrl_ADC + 29,	// Get DC coupling ADC input (BRD_ValChan)
	BRDctrl_ADC_SETINPSRC		= BRDctrl_ADC + 30,	// Set input source select
	BRDctrl_ADC_GETINPSRC		= BRDctrl_ADC + 31,	// Get input source select
	BRDctrl_ADC_SETINPMUX		= BRDctrl_ADC + 32,	// Set input multiplexer (BRD_InpMux)
	BRDctrl_ADC_GETINPMUX		= BRDctrl_ADC + 33,	// Get input multiplexer (BRD_InpMux)
	BRDctrl_ADC_SETDIFMODE		= BRDctrl_ADC + 34,	// Set differential mode of inputs (U32)
	BRDctrl_ADC_GETDIFMODE		= BRDctrl_ADC + 35,	// Get differential mode of inputs (U32)
	BRDctrl_ADC_SETSAMPLEHOLD	= BRDctrl_ADC + 36,	// Set sample/hold enable (U32)
	BRDctrl_ADC_GETSAMPLEHOLD	= BRDctrl_ADC + 37,	// Get sample/hold enable (U32)
	BRDctrl_ADC_SETGAINTUNING	= BRDctrl_ADC + 38,	// Set gain tuning (BRD_ValChan)
	BRDctrl_ADC_GETGAINTUNING	= BRDctrl_ADC + 39,	// Get gain tuning (BRD_ValChan)
	BRDctrl_ADC_SETTARGET		= BRDctrl_ADC + 40,	// Set ADC data target (U32)
	BRDctrl_ADC_GETTARGET		= BRDctrl_ADC + 41,	// Get ADC data target (U32)
	BRDctrl_ADC_SETCLKPHASE		= BRDctrl_ADC + 42,	// Set ADC clock phase tuning (BRD_ValChan)
	BRDctrl_ADC_GETCLKPHASE		= BRDctrl_ADC + 43,	// Get ADC clock phase tuning (BRD_ValChan)
	BRDctrl_ADC_SETDBLCLK		= BRDctrl_ADC + 44,	// Set ADC double clocking mode (U32)
	BRDctrl_ADC_GETDBLCLK		= BRDctrl_ADC + 45,	// Get ADC double clocking mode (U32)
	BRDctrl_ADC_SELFCLBR		= BRDctrl_ADC + 46,	// Execute self-calibration (None)
	BRDctrl_ADC_SETPRETRIGMODE	= BRDctrl_ADC + 48, // Set ADC pretrigger mode (BRD_PretrigMode)
	BRDctrl_ADC_GETPRETRIGMODE	= BRDctrl_ADC + 49, // Set ADC pretrigger mode (BRD_PretrigMode)
//	BRDctrl_ADC_SETCLKTUNING	= BRDctrl_ADC + 50,	// Set ADC clock frequency tuning in % (REAL64)
//	BRDctrl_ADC_GETCLKTUNING	= BRDctrl_ADC + 51,	// Get ADC clock frequency tuning in % (REAL64)
	BRDctrl_ADC_SETCLKTHR		= BRDctrl_ADC + 50,	// Set ADC threshold clock (REAL64) - clock frequency tuning in % for ADC28X800M
	BRDctrl_ADC_GETCLKTHR		= BRDctrl_ADC + 51,	// Get ADC threshold clock (REAL64) - clock frequency tuning in % for ADC28X800M
	BRDctrl_ADC_SETCODE			= BRDctrl_ADC + 52,	// Set code type (0 - two's complement, 1 - floating point, 2 - straight binary, 7 - Gray code) data for ADC (U32)
	BRDctrl_ADC_GETCODE			= BRDctrl_ADC + 53,	// Get code type (0 - two's complement, 1 - floating point, 2 - straight binary, 7 - Gray code) data for ADC (U32)
	BRDctrl_ADC_GETCFG			= BRDctrl_ADC + 55,	// Get ADC configuration (PBRD_AdcCfg)
	BRDctrl_ADC_WRITEINIFILE	= BRDctrl_ADC + 56,	// Write all ADC properties into ini file (PBRD_IniFile)
	BRDctrl_ADC_READINIFILE		= BRDctrl_ADC + 57,	// Read all ADC properties from ini file (PBRD_IniFile)
	BRDctrl_ADC_SETTESTMODE		= BRDctrl_ADC + 58,	// Set ADC test mode (U32)
	BRDctrl_ADC_GETTESTMODE		= BRDctrl_ADC + 59,	// Get ADC test mode (U32)
	BRDctrl_ADC_SETSPECIFIC		= BRDctrl_ADC + 60,	// Set ADC specific parameters ()
	BRDctrl_ADC_GETSPECIFIC		= BRDctrl_ADC + 61,	// Get ADC specific parameters ()
	BRDctrl_ADC_SETCLKLOCATION	= BRDctrl_ADC + 62,	// Set ADC clock source location (U32)
	BRDctrl_ADC_GETCLKLOCATION	= BRDctrl_ADC + 63,	// Get ADC clock source location (U32)
	BRDctrl_ADC_SETEXTCLKTHR	= BRDctrl_ADC + 64,	// Set ADC external clock threshold (REAL64)
	BRDctrl_ADC_GETEXTCLKTHR	= BRDctrl_ADC + 65,	// Get ADC external clock threshold (REAL64)
	BRDctrl_ADC_SETTITLEMODE	= BRDctrl_ADC + 66, // Set Title mode (BRD_EnVal)
	BRDctrl_ADC_GETTITLEMODE	= BRDctrl_ADC + 67, // Get Title mode (BRD_EnVal)
	BRDctrl_ADC_SETTITLEDATA	= BRDctrl_ADC + 68,	// Set data for title programming register (U32)
	BRDctrl_ADC_GETTITLEDATA	= BRDctrl_ADC + 69,	// Get data for title programming register (U32)
	BRDctrl_ADC_SETCLKINV		= BRDctrl_ADC + 70,	// Set ADC clock invert (U32)
	BRDctrl_ADC_GETCLKINV		= BRDctrl_ADC + 71,	// Get ADC clock invert (U32)
	BRDctrl_ADC_SETINPFILTER	= BRDctrl_ADC + 72,	// Set input LPF enable/disable (BRD_ValChan)
	BRDctrl_ADC_GETINPFILTER	= BRDctrl_ADC + 73,	// Get input LPF enable/disable (BRD_ValChan)
	BRDctrl_ADC_SETMU			= BRDctrl_ADC + 74,	// Set MU (subunit specific)
	BRDctrl_ADC_GETMU			= BRDctrl_ADC + 75,	// Get MU (subunit specific)
	BRDctrl_ADC_SETSTARTSLAVE	= BRDctrl_ADC + 76,	// Set Start from Master (U32)
	BRDctrl_ADC_GETSTARTSLAVE	= BRDctrl_ADC + 77,	// Get Start from Master (U32)
	BRDctrl_ADC_SETCLOCKSLAVE	= BRDctrl_ADC + 78,	// Set Clock from Master (U32)
	BRDctrl_ADC_GETCLOCKSLAVE	= BRDctrl_ADC + 79,	// Get Clock from Master (U32)
	BRDctrl_ADC_GETCHANORDER	= BRDctrl_ADC + 80,	// Get Order of ChanNumbers into FIFO
	BRDctrl_ADC_SETTESTSEQ		= BRDctrl_ADC + 82,	// Set ADC test sequence (U32)
	BRDctrl_ADC_GETTESTSEQ		= BRDctrl_ADC + 83,	// Get ADC test sequence (U32)
	BRDctrl_ADC_WRDELAY			= BRDctrl_ADC + 84,	// Write delay value (BRD_StdDelay)
	BRDctrl_ADC_RDDELAY			= BRDctrl_ADC + 85,	// Read delay value (BRD_StdDelay)
	BRDctrl_ADC_RSTDELAY		= BRDctrl_ADC + 86,	// Reset delay value (BRD_StdDelay)
	
	//BRDctrl_ADC_ENDPAGESDLG		= BRDctrl_ADC + 124, // destroy property pages for ADC (NULL)
	//BRDctrl_ADC_SETPROPDLG		= BRDctrl_ADC + 125, // Display setting property dialog for ADC (ULONG)
	//BRDctrl_ADC_GETPROPDLG		= BRDctrl_ADC + 126, // Display getting only property dialog for ADC (ULONG)
	BRDctrl_ADC_GETDLGPAGES		= BRDctrl_ADC + 127, // get property pages of ADC for dialog (PBRD_PropertyList)

	BRDctrl_ADC_FIFORESET		= BRDctrl_ADC + 128,	// ADC FIFO reset (NULL)
	BRDctrl_ADC_ENABLE			= BRDctrl_ADC + 129,	// ADC enable / disable (U32)
	BRDctrl_ADC_FIFOSTATUS		= BRDctrl_ADC + 130,	// Get ADC FIFO status (U32)
	BRDctrl_ADC_GETDATA			= BRDctrl_ADC + 131,	// Get data buffer from ADC FIFO (BRD_DataBuf)
	BRDctrl_ADC_ISBITSOVERFLOW  = BRDctrl_ADC + 132,	// Check overflow of ADC bits (U32)
	BRDctrl_ADC_GETPRETRIGEVENT	= BRDctrl_ADC + 136,	// Get real start event by pretrigger mode (32-bit words) (U32)
	BRDctrl_ADC_GETPREVENTPREC	= BRDctrl_ADC + 137,	// Get sample number for precision real start event by pretrigger mode (U32)
	BRDctrl_ADC_GETSRCSTREAM	= BRDctrl_ADC + 138,	// Get source address for stream (U32)
	BRDctrl_ADC_GETBLKCNT		= BRDctrl_ADC + 139,	// Get block number by restart mode (U32)
	BRDctrl_ADC_ISCOMPLEX		= BRDctrl_ADC + 140,	// Check if real (0) or complex (1) ADC signal (U32)
	BRDctrl_ADC_SETFC			= BRDctrl_ADC + 141,	// Set Fc if complex ADC signal (BRD_ValChan)
	BRDctrl_ADC_GETFC			= BRDctrl_ADC + 142,	// Get Fc if complex ADC signal (BRD_ValChan)
	BRDctrl_ADC_PREPARESTART	= BRDctrl_ADC + 143,	// Called before first BRDctrl_ADC_ENABLE (NULL)

	BRDctrl_ADC_CLRBITSOVERFLOW	= BRDctrl_ADC + 144,	// Clear ADC bit overflow flags (U32)

	BRDctrl_ADC_ILLEGAL
};

#pragma pack(push, 1)    

// for set and get Start mode command
typedef struct _BRD_AdcStartMode
{
	U32		src;			// Start source
	U32		inv;			// Start inversion 
	REAL64	level;			// Start level (Comparator Threshold value)
	BRD_StartMode	stndStart;	// Standart Start mode
} BRD_AdcStartMode, *PBRD_AdcStartMode;

//#define BRD_CHANCNT			2
#define BRD_ADC_MAXINPUT	32

//const int BRD_CHANCNT		= 2; // Number of channels
//const int BRD_ADC_MAXINPUT	= 32; // Maximum inputs numbers

// programming input multiplexer
typedef struct _BRD_AdcInpMux
{
	U32			muxCnt;					// Multiplexer counter 
	BRD_ValChan mux[BRD_ADC_MAXINPUT];	// For each input
} BRD_AdcInpMux, *PBRD_AdcInpMux;

// programming input source
typedef struct _BRD_AdcInpSrc
{
	U32		source;		// Source select
	REAL64	voltage;	// Voltage (ADM12x1M/3M/10M, ADM416x200)
	U32		chan;		// Channel number (ADM216x2M5)
} BRD_AdcInpSrc, *PBRD_AdcInpSrc;

// programming delay
/*typedef struct _BRD_AdcDelay
{
	U32		nodeID;		// node ID: ADC inputs, external start, SYNX's
	U32		value;		// delay value
} BRD_AdcDelay, *PBRD_AdcDelay;
*/
// delayed nodeID (look at ctrladmpro.h)
//enum {
//	BRDnid_ADC0		= 0,	// node ID: ADC input 0
//	BRDnid_ADC1		= 1,	// node ID: ADC input 1
//	BRDnid_ADC2		= 2,	// node ID: ADC input 2
//	BRDnid_ADC3		= 3,	// node ID: ADC input 3
//	BRDnid_EXTSTART = 8,	// node ID: submodule exernal start
//	BRDnid_SYNX0	= 12,	// node ID: SYNX input 0
//	BRDnid_SYNX1	= 13,	// node ID: SYNX input 1
//	BRDnid_SYNX2	= 14,	// node ID: SYNX input 2
//	BRDnid_SYNX3	= 15,	// node ID: SYNX input 3
//};

// getting configuration
typedef struct _BRD_AdcCfg
{
	U32		Bits;			// разрядность
	U32		Encoding;		// тип кодировки (0 - прямой, 1 - двоично-дополнительный, 2 - код Грея)
	U32		MinRate;		// минимальная частота дискретизации
	U32		MaxRate;		// максимальная частота дискретизации
	U32		InpRange;		// входной диапазон
	U32		FifoSize;		// размер FIFO АЦП (в байтах)
	U32		NumChans;		// число каналов
	U32		ChanType;		// тип канала (1 - RF (gain in dB))
} BRD_AdcCfg, *PBRD_AdcCfg;

// programming specific parameters
typedef struct _BRD_AdcSpec
{
	U32		command;	// command number
	PVOID	arg;		// pointer of argument
} BRD_AdcSpec, *PBRD_AdcSpec;

// ADC error codes
enum {
	BRDerr_ADC_ILLEGAL_CHANNEL		=	BRDctrl_ADC | 0xC0000000,
	BRDerr_ADC_ILLEGAL_GAIN			=	BRDctrl_ADC | 0xC0000001,
	BRDerr_ADC_ILLEGAL_INPRANGE		=	BRDctrl_ADC | 0xC0000002,
	BRDerr_ADC_UNDEFINED
};

// Numbers of Specific Command
typedef enum _ADC_NUM_CMD
{
	ADCcmd_ADJUST		= 0,
	ADCcmd_INHALF		= 1,
} ADC_NUM_CMD;

#pragma pack(pop)    

#endif // _CTRL_ADC_H

//
// End of file
//