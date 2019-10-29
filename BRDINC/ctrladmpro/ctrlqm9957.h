/*
 ****************** File CtrlQm9957.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : QM9957 section
 *
 * (C) InSys by Sklyarov A. Nov, 2010
 *
 *
 ************************************************************
*/

#ifndef _CTRL_QM9957_H
 #define _CTRL_QM9957_H


#include "ctrladmpro.h"
enum {
	
	BRDctrl_QM_SETMASTER			= BRDctrl_QM + 0,	// Set QM9957 master/single/slave mode (U32)
	BRDctrl_QM_GETMASTER			= BRDctrl_QM + 1,	// Get QM9957 master/single/slave mode (U32)
	BRDctrl_QM_SETCLKSOURCE			= BRDctrl_QM + 2,	// Set source of clock QM9957 (U32)
	BRDctrl_QM_GETCLKSOURCE			= BRDctrl_QM + 3,	// Get source of clock QM9957 (U32)
	BRDctrl_QM_SETCHANMASK			= BRDctrl_QM + 4,	// Set mask of QM9957 channels (U32)
	BRDctrl_QM_GETCHANMASK			= BRDctrl_QM + 5,	// Get mask of QM9957 channels (U32)
	BRDctrl_QM_SETSTARTMODE			= BRDctrl_QM + 6,   // Set QM9957 start mode (BRD_StartMode)
	BRDctrl_QM_GETSTARTMODE			= BRDctrl_QM + 7,   // Get QM9957 start mode (BRD_StartMode)
	BRDctrl_QM_FIFORESET			= BRDctrl_QM + 8,	// QM9957 FIFO reset (NULL)
	BRDctrl_QM_START				= BRDctrl_QM + 9,	// QM9957 Start
	BRDctrl_QM_STOP					= BRDctrl_QM + 10,	// QM9957 Stop
	BRDctrl_QM_FIFOSTATUS			= BRDctrl_QM + 11,	// Get QM9957 FIFO status (U32)
	BRDctrl_QM_PUTDATA				= BRDctrl_QM + 12,	// Put data buffer into QM9957 FIFO (PVOID)
	BRDctrl_QM_GETSRCSTREAM			= BRDctrl_QM + 13,	// Get source address for stream (U32)
	BRDctrl_QM_QMSETREG				= BRDctrl_QM + 14,	// Set register QM9957(BRD_QmSetReg)
	BRDctrl_QM_QMGETREG				= BRDctrl_QM + 15,	// Get register QM9857(BRD_QmSetReg)
	BRDctrl_QM_QMMASTERRESET		= BRDctrl_QM + 16,	// Reset QM9957(NULL)
	BRDctrl_QM_QMRESETIO			= BRDctrl_QM + 17,	// Reset IO QM9957(NULL)
	BRDctrl_QM_QMUPDATEIO			= BRDctrl_QM + 18,	// Update IO QM9957(U32 chipMask)
	BRDctrl_QM_SELECTPROFILE		= BRDctrl_QM + 19,	// Select profile QM9957(U32)
	BRDctrl_QM_SETPROFILE			= BRDctrl_QM + 20,	// Set profile QM9957(BRD_QmSetProfile)
	BRDctrl_QM_SETMAINPARAM			= BRDctrl_QM + 21,	// Set main param QM9957(BRD_QmSetMainParam)
	BRDctrl_QM_SETSYNCMODE			= BRDctrl_QM + 22,	// Set sync mode(BRD_QmSetSyncMode)
	BRDctrl_QM_SETCYCLEMODE			= BRDctrl_QM + 23,	// Set cycle mode FIFO(U32)
	BRDctrl_QM_GETCFG				= BRDctrl_QM + 24,	// Get CFG ADMQM9957(BRD_QmCfg)

	BRDctrl_QM_ILLEGAL
};

#pragma pack(push, 1)    


// for WriteQmReg and ReadQmReg  functions
typedef struct _BRD_QmSetReg
{
	U32 chipMask,	// mask for programming QM chips,
		addrReg,	// register address into AD9957 chip for programming
		nword,		// number of words for writing/reading (1 or 2) 
		dataL,		// Low word data for writing/reading
		dataH;		// Hith word data for writing/reading
} BRD_QmSetReg, *PBRD_QmSetReg;
//----------------------------------------------
// for  QmSetProfile function
typedef struct _BRD_QmSetProfile
{	U32 chipMask,	 // mask for programming QM chips,
		operMode, //
		profNum,	 // number of profile 
		freqNco,	 // cod frequency NCO
		phaseNco,	 // cod phase offset NCO
		cicRate,	 // CIC rate
		spectrInv,	 // invert of spectr
		invCicBypass,// bypass of cic
		outScale,	 // OutputScaleFactor
		ampScale,	 // AmplitudeScaleFactor
		errors;
} BRD_QmSetProfile,*PBRD_QmSetProfile;

// for  QmSetMainParam function
typedef struct _BRD_QmSetMainParam
{
	U32  chipMask,		// mask for programming QM chips,
		 operMode, //
		 clockMult,		// multiplier of Clock
		 pllEnb,	
		 invSyncEnb,		
		 vcoSel,		
		 chargePumpSel,	
		 syncDelay,	
		 errors;
} BRD_QmSetMainParam, *PBRD_QmSetMainParam;

// for QmSetSync  function
typedef struct _BRD_QmSetSyncMode
{
	U32  SyncMode,		// Mode of Sync
		 Master_Slave,	// Signal M/S
		 SyncEnable,	// Signal SYNC_EN
		 SyncSelect,	// Signal SYNC_SEL
		 FD_ST,			// Signal FD_ST
		 FD_CLK,		// Signal FD_CLK
		 ipdclk;		// Signal ipdclk

} BRD_QmSetSyncMode, *PBRD_QmSetSyncMode;


// getting configuration
typedef struct _BRD_QmCfg
{
	U32		FifoSize;
	U32		MaxChans;
	U32		SubIntClk;
} BRD_QmCfg, *PBRD_QmCfg;

#pragma pack(pop)    

#endif // _CTRL_QM9957_H

//
// End of file
//
