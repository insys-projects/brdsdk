/*
 ****************** File Ctrl.h *****************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl
 *
 * (C) InSys by Dorokhin Andrey Jan, 2002
 *
 *	27.11.2003 - add BRDctrl_PIO (Dorokhin A.)
 *	11.02.2004 - add BRDctrl_DIO32IN, BRDctrl_DIO32OUT, BRD_CMPSRC (Dorokhin A.)
 *	06.04.2004 - add BRD_DataBuf, BRD_SyncMode, BRD_StartMode (Dorokhin A.)
 *  18.05.2005 - add BRD_IniFile, BRD_PropertyPage, BRD_PropertyList (Dorokhin A.)
 *	02.06.2005 - add BRDctrl_DSPNODE (Dorokhin A.)
 *	30.08.2005 - add BRDctrl_TEST (Dorokhin A.)
 *	30.06.2006 - add BRDctrl_FOTR (Dorokhin A.)
 *
 ************************************************************
*/

#ifndef _CTRL_H
 #define _CTRL_H

#include "utypes.h"

#define BRD_SECTIONSIZE 256 // start comparators numbers

enum {
	BRDctrl_DAQ			= BRD_SECTIONSIZE*8,	// DAQ control
	BRDctrl_CMN			= BRD_SECTIONSIZE*9,	// common control parameters
	BRDctrl_ADC			= BRD_SECTIONSIZE*10,	// ADC control parameters
	BRDctrl_DAC			= BRD_SECTIONSIZE*11,	// DAC control parameters
	BRDctrl_DDC			= BRD_SECTIONSIZE*12,	// DDC control parameters
	BRDctrl_PIO			= BRD_SECTIONSIZE*13,	// PIO control parameters
	BRDctrl_STREAM		= BRD_SECTIONSIZE*14,	// STREAM control parameters
	BRDctrl_SVEAM		= BRD_SECTIONSIZE*15,	// SVEAM control parameters
	BRDctrl_DIOIN		= BRD_SECTIONSIZE*16,	// DIOIN control parameters
	BRDctrl_DIOOUT		= BRD_SECTIONSIZE*17,	// DIOOUT control parameters
	BRDctrl_SDRAM		= BRD_SECTIONSIZE*18,	// SDRAM control parameters
	BRDctrl_DSPNODE		= BRD_SECTIONSIZE*19,	// DSP node control parameters
	BRDctrl_TEST		= BRD_SECTIONSIZE*20,	// Test control parameters
	BRDctrl_FOTR		= BRD_SECTIONSIZE*21,	// FOTR control parameters
	BRDctrl_QM			= BRD_SECTIONSIZE*22,	// QM9857 control parameters
	BRDctrl_GC5016		= BRD_SECTIONSIZE*23,	// GC5016 control parameters
	BRDctrl_DDS			= BRD_SECTIONSIZE*24,	// DDS control parameters
	BRDctrl_SYNCDAC		= BRD_SECTIONSIZE*25,	// SyncDac control parameters
	BRDctrl_COMMON		= BRD_SECTIONSIZE*26,	// real common control area
	BRDctrl_NET			= BRD_SECTIONSIZE*27,	// net (still EMAC) control parameters
	BRDctrl_ILLEGAL
};

//
// Real Common Commands
//
enum {
	BRDctrl_GETTRACETEXT	= BRDctrl_COMMON + 0,	// Получить буфер трассировки в виде текста 
	BRDctrl_GETTRACEPARAM	= BRDctrl_COMMON + 1,	// Получить буфер трассировки в виде структуры 
};

#pragma pack(push, 1)    

// for set and get Parameter value of the each channel
typedef struct _BRD_ValChan 
{
	REAL64	value;		// Parameter value
	U32		chan;		// Channel number
} BRD_ValChan, *PBRD_ValChan;

// for set and get Parameter value of the each channel
typedef struct _BRD_UvalChan 
{
	U32		value;		// Parameter value
	U32		chan;		// Channel number
} BRD_UvalChan, *PBRD_UvalChan;

// for set and get Parameter value 
typedef struct _BRD_EnVal
{
	U32		enable;	// enable/disable parameter
	U32		value;	// value  parameter
} BRD_EnVal, *PBRD_EnVal;

typedef struct _BRD_WaitEvent
{
	U32		timeout;		// IN,  Timeout (msec) (0xFFFFFFFFL - INFINITE)
	HANDLE	hAppEvent;		// application event
} BRD_WaitEvent, *PBRD_WaitEvent;

typedef struct
{
	U32		item;			// IN:  Number of Items into pItem[] Array
	U32		itemReal;		// OUT: Number of Initialized Items or Number of Really Needed Items
	void	*pItem;			// OUT: Array with Items
} BRD_ItemArray, *PBRD_ItemArray;

typedef struct
{
	U32		traceId;	// IN: Number of Trace Buffer
	U32		sizeb;		// IN: Size of Trace Buffer (bytes)
	BRDCHAR	*pText;		// OUT: Trace Buffer like Text
} BRD_TraceText, *PBRD_TraceText;

typedef struct
{
	U32		traceId;	// IN: Number of Trace Buffer
	U32		sizeb;		// IN: Size of Trace Buffer into *pParam (bytes)
	U32		sizebReal;	// OUT: Size of Real Trace Buffer (bytes)
	void	*pParam;	// OUT: Trace Buffer like Structure
} BRD_TraceParam, *PBRD_TraceParam;

#pragma pack(pop)    

#endif // _CTRL_H

//
// End of file
//

