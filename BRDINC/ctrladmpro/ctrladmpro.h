/*
 ****************** File CtrlAdmPro.h *****************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl
 *
 * (C) InSys by Dorokhin Andrey Sep, 2005
 *
 *	26.09.2005 - build (Dorokhin A.)
 *	03.07.2006 - add BRD_MsgDataBuf struct for remote devices(Dorokhin A.)
 *	24.10.2006 - add BRD_Title, BRD_TitleMode structs (Dorokhin A.)
 *
 ************************************************************
*/

#ifndef _CTRLADMPRO_H
 #define _CTRLADMPRO_H

#include "ctrl.h"

#pragma pack(push, 1)    

// for set and get data buffer
typedef struct _BRD_DataBuf 
{
	PVOID	pData;		// pointer to data buffer
	U32		size;		// size of data buffer in bytes
} BRD_DataBuf, *PBRD_DataBuf;

// for send and receive message data buffer (for remote devices)
typedef struct _BRD_MsgDataBuf 
{
	PVOID	pData;		// pointer to message data buffer
	U32		size;		// size of message data buffer in 32-bit words 
	U32		addr;	// address on remote device
	U32		timeout;	// timeout (msec) (0xFFFFFFFFL - INFINITE)
} BRD_MsgDataBuf, *PBRD_MsgDataBuf;

// for set and get Clock mode
typedef struct _BRD_ClkMode {
	U32		src;	// IN - clock source
	REAL64	value;	// IN (external clock) / OUT (real clock) 
} BRD_ClkMode, *PBRD_ClkMode;

// for set and get Divider of clock
typedef struct _BRD_DivClk {
	U32		divClk;	// IN - divider
	REAL64	outRate;//  OUT (outrate) 
} BRD_DivClk, *PBRD_DivClk;

// for set and get Clock and Ref mode
typedef struct _BRD_ClkRefMode {
	U32		srcClk;		// IN - clock source
	U32		srcRef;		// IN - reference source
	REAL64	valueClk;	// IN (external clock) / OUT (real clock) 
	REAL64	valueRef;	// IN (external reference) / OUT (real reference) 
} BRD_ClkRefMode, *PBRD_ClkRefMode;

// for set and get Syncronization mode
typedef struct _BRD_SyncMode 
{
	U32		clkSrc;		// clock source (IN)
	REAL64	clkValue;	// clock value (IN for External clock/OUT in fact)
	REAL64	rate;		// input/output rate (IN/OUT)
} BRD_SyncMode, *PBRD_SyncMode;

// Clock sources
enum {
	BRDclks_SYSGEN		= 0,	// Base Module System Generator (usualy DSP=100 MHz or AMB=66.6666 MHz)
	BRDclks_BASEGEN0	= 1,	// Base Module Generator 0
	BRDclks_BASEGEN1	= 2,	// Base Module Generator 1
	BRDclks_EXTSDX		= 3,	// External clock from SDX
	BRDclks_EXTSYNX		= 4,	// External clock from SYNX
	BRDclks_SMCLK		= 7,	// Submodule clock
	BRDclks_TRDMAIN		= 8,	// Frequency of tetrad MAIN (tetrad 0)
	BRDclks_BASEDAC		= 9,	// Output frequency of DAC on base module (tetrad 1)
	BRDclks_BASEDDS		= 10,	// Output frequency of DDS on base module (tetrad 2)
	BRDclks_INFREQ		= 12,	// Frequency of input (Synchronous with ADC or input DIO32)
	BRDclks_SMOUTFREQ	= 13,	// Submodule frequency of output (Synchronous with SM DAC or output DIO32)

	BRDclks_EXTCLK  = 0x81,		// External clock 
};

enum 
{
	BRDims_SLAVE	= 0,	// 
	BRDims_SINGLE	= 1,	// 
	BRDims_MASTER	= 2,	// 
};

#define BRD_CMPNUM 2 // start/clock comparators numbers

// for set and get Start mode function
typedef struct _BRD_StartMode 
{
	U32			startSrc;			// Start source
	U32			startInv;			// Start inversion
//	U32			pretrig;			// Pretrigger mode on
	U32			trigOn;				// Trigger start mode on
	U32			trigStopSrc;		// Stop source for trigger start
	U32			stopInv;			// Stop inversion
	U32			reStartMode;		// Restart mode
//	BRD_EnVal	startDelayCnt;		// Start delay counter
//	BRD_EnVal	daqCnt;				// Data aquisition counter
//	BRD_EnVal	skipCnt;			// Data skip counter
//	U32			cmpSrc;				// Comparator source 
//	REAL64		cmpThr[BRD_CMPNUM];	// Comparators Thresholds values 
} BRD_StartMode, *PBRD_StartMode;

// start/stop sources
enum {
	BRDsts_PRG		= 0,	// Program start/stop
	BRDsts_CMP0		= 1,	// Start/stop from comparator 0
	BRDsts_CMP1		= 2,	// Start/stop from comparator 1
	BRDsts_EXTSYNX	= 4,	// Start/stop from SYNX
	BRDsts_SUBMOD	= 7,	// Start/stop from Submodule
	BRDsts_TRDMAIN	= 8,	// Start/stop from tetrad MAIN (tetrad 0)
	BRDsts_BASEDAC	= 9,	// Start/stop synchronous with Base DAC (tetrad 1)
	BRDsts_BASEDDS	= 10,	// Start/stop from Base DDS (tetrad 2)
	BRDsts_TRDADC	= 12,	// Start/stop from tetrad ADC (tetrad 4)
	BRDsts_TRDSMOUT	= 13,	// Start/stop from submodule output tetrad, Synchronous with SM DAC or output DIO32 (tetrad 5)
};

// start/clock comparators sources
enum 
{
	BRDcmps_EXTSTCLK, // Comparator 0 - External Start from Subunit / Comparator 1 - External Start(Clock) from SDX
	BRDcmps_CHAN0CLK, // Comparator 0 - Channel 0					/ Comparator 1 - External Start(Clock) from SDX
	BRDcmps_CHAN1CLK, // Comparator 0 - Channel 1					/ Comparator 1 - External Start(Clock) from SDX
	BRDcmps_CHAN0BOTH // Comparator 0 - Channel 0					/ Comparator 1 - Channel 0
};

// start/clock 12/14/16-bit comparators sources (for each comparator)
enum 
{
	BRDcmpex_CHAN0,		// Channel 0
	BRDcmpex_CHAN1,		// Channel 1
	BRDcmpex_EXTCLK,	// External Start(Clock) from SDX
	BRDcmpex_EXTST,		// External Start from Subunit
};

// for set and get Pretrigger mode function
typedef struct _BRD_PretrigMode 
{
	U32		enable;			// 1 - enabled pretrigger mode
	U32		external;		// 1 - enabled external pretrigger mode
	U32		assur;			// 1 - enabled assured pretrigger mode
	U32		size;			// размер претриггера (в 32-битных словах) от 2-х слов до почти размера FIFO (точное значение максимума зависит от ширины FIFO)
} BRD_PretrigMode, *PBRD_PretrigMode;

// code types
enum {
	BRDcode_TWOSCOMPLEMENT	= 0,	// Two's complement code
	BRDcode_FLOATPOINT		= 1,	// float point code
	BRDcode_STRAIGHTBINARY	= 2,	// straight binary code
	BRDcode_GRAY			= 7		// Gray code
};

// for set Parameter value from ini file function
typedef struct _BRD_IniFile
{
	BRDCHAR	fileName[MAX_PATH];		// name of ini file
	BRDCHAR	sectionName[32];		// section name into ini file
} BRD_IniFile, *PBRD_IniFile;

// for property page functions
typedef struct _BRD_PropertyPage
{
	PVOID	pPropertyPage;	// pointer to property page
	BRDCHAR	name[32];		// property page caption
//	UINT    nIDD;			// number of identification
//	HWND	hPropertyPage;	// handle of property page
} BRD_PropertyPage, *PBRD_PropertyPage;

// for property page functions
typedef struct _BRD_PropertyList
{
	U32					PagesCnt;	// число страниц свойств
	HINSTANCE			hLib;		// дескриптор DLL с этими страницами
	U32					NumDlg;		// номер диалога
//	PBRD_PropertyPage	pPages;		// описание страниц свойств
} BRD_PropertyList, *PBRD_PropertyList;

// title description
typedef struct _BRD_Title {
	U32		signature;			// 0xAA5500FF
	U32		blkNum;				// номер блока в сеансе сбора
	U32		prtCnt;				// событие старта для этого блока (номер 64-разрядного слова)
	U16		prtStatus;			// уточнение события старта для этого блока
	U08		sizeUnit;			// размерность размера заголовка и размера блока в байтах
	U08		titleSize;			// размера заголовка
	U32		blkSize;			// размер блока
	U32		progData;			// данные, программируемые через регистры 0x20С и 0x20D
	__int64	EventCnt;			// сквозной счетчик событий
} BRD_Title, *PBRD_Title;

// programming delay
typedef struct _BRD_StdDelay
{
	U32		nodeID;		// node ID: ADC/DAC channels, external start, SYNX's
	U32		value;		// delay value
} BRD_StdDelay, *PBRD_StdDelay;

// delayed nodeID 
enum {
	BRDnid_CHAN0		= 0,	// node ID: channel 0
	BRDnid_CHAN1		= 1,	// node ID: channel 1
	BRDnid_CHAN2		= 2,	// node ID: channel 2
	BRDnid_CHAN3		= 3,	// node ID: channel 3
	BRDnid_CHAN4		= 4,	// node ID: channel 4
	BRDnid_CHAN5		= 5,	// node ID: channel 5
	BRDnid_CHAN6		= 6,	// node ID: channel 6
	BRDnid_CHAN7		= 7,	// node ID: channel 7
	BRDnid_EXTSTART		= 8,	// node ID: submodule exernal start
	BRDnid_SYNX0		= 12,	// node ID: SYNX input 0
	BRDnid_SYNX1		= 13,	// node ID: SYNX input 1
	BRDnid_SYNX2		= 14,	// node ID: SYNX input 2
	BRDnid_SYNX3		= 15,	// node ID: SYNX input 3
};

#pragma pack(pop)    

#endif // _CTRLADMPRO_H

//
// End of file
//

