/*
 ****************** File CtrlDuc9957.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : DUC9957 section
 *
 * (C) InSys by Sklyarov A. Dec, 2009
 *
 *
 ************************************************************
*/

#ifndef _CTRL_DUC9957_H
 #define _CTRL_DUC9957_H


#include "ctrladmpro.h"
enum {
	
	BRDctrl_DUC_SETMASTER			= BRDctrl_QM + 0,	// Set DUC9957 master/single/slave mode (U32)
	BRDctrl_DUC_GETMASTER			= BRDctrl_QM + 1,	// Get DUC9957 master/single/slave mode (U32)
	BRDctrl_DUC_SETCLKSOURCE		= BRDctrl_QM + 2,	// Set source of clock DUC9957 (U32)
	BRDctrl_DUC_GETCLKSOURCE		= BRDctrl_QM + 3,	// Get source of clock DUC9957 (U32)
	BRDctrl_DUC_SETSTARTMODE		= BRDctrl_QM + 4,   // Set DUC9957 start mode (BRD_StartMode)
	BRDctrl_DUC_GETSTARTMODE		= BRDctrl_QM + 5,   // Get DUC9957 start mode (BRD_StartMode)
	BRDctrl_DUC_FIFORESET			= BRDctrl_QM + 6,	// DUC9957 FIFO reset (NULL)
	BRDctrl_DUC_START				= BRDctrl_QM + 7,	// DUC9957 Start
	BRDctrl_DUC_STOP				= BRDctrl_QM + 8,	// DUC9957 Stop
	BRDctrl_DUC_FIFOSTATUS			= BRDctrl_QM + 9,	// Get DUC9957 FIFO status (U32)
	BRDctrl_DUC_PUTDATA				= BRDctrl_QM + 10,	// Put data buffer into DUC9957 FIFO (PVOID)
	BRDctrl_DUC_GETSRCSTREAM		= BRDctrl_QM + 11,	// Get source address for stream (U32)
	BRDctrl_DUC_DUCSETREG			= BRDctrl_QM + 12,	// Set register DUC9957(BRD_DucSetReg)
	BRDctrl_DUC_DUCGETREG			= BRDctrl_QM + 13,	// Get register QM9857(BRD_DucSetReg)
	BRDctrl_DUC_DUCMASTERRESET		= BRDctrl_QM + 14,	// Reset DUC9957(NULL)
	BRDctrl_DUC_DUCRESETIO			= BRDctrl_QM + 15,	// Reset IO DUC9957(NULL)
	BRDctrl_DUC_DUCUPDATEIO			= BRDctrl_QM + 16,	// Update IO DUC9957(NULL)
	BRDctrl_DUC_SELECTPROFILE		= BRDctrl_QM + 17,	// Select profile DUC9957(U32)
	BRDctrl_DUC_SETPROFILE			= BRDctrl_QM + 18,	// Set profile DUC9957(BRD_DucSetProfile)
	BRDctrl_DUC_SETMAINPARAM		= BRDctrl_QM + 19,	// Set main param QM9857(BRD_DucSetMainParam)
	BRDctrl_DUC_SETSYNCMODE			= BRDctrl_QM + 20,	// Set sync mode(U32)
	BRDctrl_DUC_SETCYCLEMODE		= BRDctrl_QM + 21,	// Set cycle mode FIFO(U32)

	BRDctrl_DAC_ILLEGAL
};

#pragma pack(push, 1)    


// for WriteDucReg and ReadDucReg  functions
typedef struct _BRD_DucSetReg
{
	U32		addrReg;		// register address into DUC9957 chip for programming
	U32		nword;			// number of words for writing/reading (1 or 2) 
	U32		dataL;			// Low word data for writing/reading
	U32		dataH;			// Hith word data for writing/reading
} BRD_DucSetReg, *PBRD_DucSetReg;
//----------------------------------------------
// for  DucSetProfile function
typedef struct _BRD_DucSetProfile
{	U32 profNum,	 // number of profile 
		phaseNco,	 // cod phase offset NCO
		freqNco,	 // cod frequency NCO
		cicRate,	 // CIC rate
		spectrInv,	 // invert of spectr
		invCicBypass,// bypass of cic
		outputScale,	 // output scale
		errors;
} BRD_DucSetProfile,*PBRD_DucSetProfile;

// for  DucSetMainParam function
typedef struct _BRD_DucSetMainParam
{
	U32  clockMult,		// multiplier of Clock
		 invSincFilterEnable, 
		 operatingMode, //
		 errors;
} BRD_DucSetMainParam, *PBRD_DucSetMainParam;
#pragma pack(pop)    

#endif // _CTRL_DUC9957_H

//
// End of file
//