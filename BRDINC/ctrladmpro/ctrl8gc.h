/*
 ****************** File Ctrl8gc.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : 8GC section
 *
 * (C) InSys by Sklyarov A. Apr, 2012
 *
 *
 ************************************************************
*/

#ifndef _CTRL_8GC_H
 #define _CTRL_8GC_H


#include "ctrladmpro.h"
#define BRDctrl_8GC BRDctrl_GC5016 // start comparators numbers

enum {
	
	BRDctrl_8GC_SETMASTER		= BRDctrl_8GC + 0,	// Set 8GC master/single/slave mode (*U32)
	BRDctrl_8GC_GETMASTER		= BRDctrl_8GC + 1,	// Get 8GC master/single/slave mode (*U32)
	BRDctrl_8GC_SETCHANMASK		= BRDctrl_8GC + 2,	// Set mask of 8GC channels (*U32)
	BRDctrl_8GC_GETCHANMASK		= BRDctrl_8GC + 3,	// Get mask of 8GC channels (*U32)
	BRDctrl_8GC_SETCLKMODE		= BRDctrl_8GC + 4,	// Set source of clock 8GC ((PBRD_ClkMode))
	BRDctrl_8GC_GETCLKMODE		= BRDctrl_8GC + 5,	// Get source of clock 8GC (PBRD_(PBRD_ClkMode)
	BRDctrl_8GC_SETSTARTMODE	= BRDctrl_8GC + 6,   // Set 8GC start mode (PBRD_StartMode)
	BRDctrl_8GC_GETSTARTMODE	= BRDctrl_8GC + 7,   // Get 8GC start mode (PBRD_StartMode)
	BRDctrl_8GC_FIFORESET		= BRDctrl_8GC + 8,	// 8GC FIFO reset (NULL)
	BRDctrl_8GC_START			= BRDctrl_8GC + 9,	// 8GC Start(NULL)
	BRDctrl_8GC_STOP			= BRDctrl_8GC + 10,	// 8GC Stop (NULL)
	BRDctrl_8GC_FIFOSTATUS		= BRDctrl_8GC + 11,	// Get 8GC FIFO status (*U32)
	BRDctrl_8GC_GETDATA			= BRDctrl_8GC + 12,	// Get data buffer from 8GC FIFO (PBRD_DataBuf)
	BRDctrl_8GC_SETTARGET		= BRDctrl_8GC + 13,  // Set 8GC data target (*U32)
	BRDctrl_8GC_GETTARGET		= BRDctrl_8GC + 14,  // Get 8GC data target (*U32)
	BRDctrl_8GC_GETSRCSTREAM	= BRDctrl_8GC + 15,	// Get source address for stream (*U32)
	BRDctrl_8GC_GCSETREG		= BRDctrl_8GC + 16,	// Set register 8GC(PBRD_DdcSetReg)
	BRDctrl_8GC_GCGETREG		= BRDctrl_8GC + 17,	// Get register 8GC(PBRD_DdcSetReg)
	BRDctrl_8GC_GETADCOVER		= BRDctrl_8GC + 18,	// Get Adc Over 8GC(*U32)
	BRDctrl_8GC_ADCENABLE		= BRDctrl_8GC + 19,	// Enable Adc 8GC(*U32)
	BRDctrl_8GC_SETDDCFC		= BRDctrl_8GC + 20,	// Set Fc of DDC 8GC(*U32)
	BRDctrl_8GC_SETADMMODE		= BRDctrl_8GC + 21,	// Set mode of ADM:0-Adc,1- Ddc(*U32)
	BRDctrl_8GC_SETDDCMODE		= BRDctrl_8GC + 22,	// Set mode of DDC:0-FullRata,1-SplitIQ,2-Double Rate(*U32)
	BRDctrl_8GC_SETDDCSYNC		= BRDctrl_8GC + 23,	// Set mode of syncro 8GC(PBRD_DdcSetSync)
	BRDctrl_8GC_PROGRAMDDC		= BRDctrl_8GC + 24,	// Programming of chip DDC(PBRD_DdcProgram)
	BRDctrl_8GC_PROGRAMADC		= BRDctrl_8GC + 25,	// Programming of chip ADC(BRD_AdcProgram)
	BRDctrl_8GC_STARTDDC		= BRDctrl_8GC + 26,	// Start DDC(NULL)
	BRDctrl_8GC_STOPDDC			= BRDctrl_8GC + 27,	// Stop DDC(NULL)
	BRDctrl_8GC_SETTESTMODE		= BRDctrl_8GC + 28,	// Set 8GC test mode (*U32)
	BRDctrl_8GC_GETTESTMODE		= BRDctrl_8GC + 29,	// Get 8GC test mode (*U32)
	BRDctrl_8GC_ADMGETINFO		= BRDctrl_8GC + 30,	// Get 8GC Adm Info (PBRD_AdmGetInfo)
	BRDctrl_8GC_SETDELAY		= BRDctrl_8GC + 31,	// Set 8GC delay (PBRD_SetDalay)
	BRDctrl_8GC_SETTESTSEQUENCE	= BRDctrl_8GC + 32,	// Set 8GC TestSequence (*U32)

	BRDctrl_8GC_READINIFILE		= BRDctrl_8GC + 33,	// Read ini file
	BRDctrl_8GC_SETADCLAG		= BRDctrl_8GC + 34,	// Set lag data 8GC (PBRD_SetAdcLag) 

	BRDctrl_8GC_ILLEGAL
};

#pragma pack(push, 1)    

// for Write Chip GC5016 Reg and Read Chip GC5016 Reg  functions
typedef struct _BRD_DdcSetReg
{
	U32		page;		// number of page programming GC5016 chip
	U32		reg;		// register address into GC5016 chip for programming
	U32		data;		// data for writing
} BRD_DdcSetReg, *PBRD_DdcSetReg;
//----------------------------------------------
// for 8GC SetSync function
typedef struct _BRD_DdcSetSync
{
	U32		syncMode,	// mode of syncro (1,2)
			progSync,	// 1-out impulse syncro
			slave,		// 1-slave, 0-master
			resDiv;		// 1-reset divider
} BRD_DdcSetSync, *PBRD_DdcSetSync;
//----------------------------------------------


// for  Set 8gc Set Fc function
typedef struct _BRD_DdcSetFc
{	
	U32 maskChans,	 // mask channels for programming 
			   Fc;	 // Fc DDC (Hz) 
} BRD_DdcSetFc,*PBRD_DdcSetFc;

// for  Program Ddc function
typedef struct _BRD_DdcProgram
{	U32  nItems;	// Number of items,
	U32	 *pData;	// Data for programming
} BRD_DdcProgram, *PBRD_DdcProgram;


// for  Program Adc function
typedef struct _BRD_AdcProgram
{	U32  maskAdc;	//  Маска программируемых АЦП: 1-3
	S32	 offset;	//  Смещение 0 АЦП: -127..127	
	S32	 vref;		//  Опорное напряжение АЦП:0-2.0В,1-2.1В,2-2.2В,3-2.3В,4-2.4В,5-2.5
	S32	 bufCur1;	//  Ток входного буфера 1: -100,-90..0,10,20..520,530	
	S32	 bufCur2;	//  Ток входного буфера 2: -100,-90..0,10,20..520,530
} BRD_AdcProgram, *PBRD_AdcProgram;



// programming specific parameters
typedef struct _BRD_8gcSpec
{
	U32		command;	// command number
	PVOID	arg;		// pointer of argument
} BRD_8gcSpec, *PBRD_8gcSpec;


// for  SetDelay function
typedef	struct _BRD_SetDelay
{ 
	S32 nInc;  //  -64..64  (положительное значение-увеличение задержки, отрицательное - уменьшение)
	S32	nId;   //  1- применяется для базового модуля, 2- для субмодуля
	S32	isRst; //  1 -сброс (устанавливается значение задержки  по умолчанию) 
} BRD_SetDelay, *PBRD_SetDelay;

typedef struct _BRD_AdmGetInfo
{ 
	U32 admId;
	U32 pldId, pldModification, pldVersion, pldBuild;
	U32 trdId, trdIdModification, trdVersion ;
} BRD_AdmGetInfo, *PBRD_AdmGetInfo;

// for  SetAdcLag function
typedef	struct _BRD_SetAdcLag
{ 
	U32 chan;
	U32	lag;
} BRD_SetAdcLag, *PBRD_SetAdcLag;

#pragma pack(pop)    

#endif // _CTRL_8GC_H

//
// End of file
//
