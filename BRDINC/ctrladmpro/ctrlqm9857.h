/*
 ****************** File CtrlQm9857.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : QM9857 section
 *
 * (C) InSys by Sklyarov A. Sep, 2006
 *
 *
 ************************************************************
*/

#ifndef _CTRL_QM9857_H
 #define _CTRL_QM9857_H


#include "ctrladmpro.h"
enum {
	
	BRDctrl_QM_SETMASTER		= BRDctrl_QM + 0,	// Set QM9857 master/single/slave mode (U32)
	BRDctrl_QM_GETMASTER		= BRDctrl_QM + 1,	// Get QM9857 master/single/slave mode (U32)
	BRDctrl_QM_SETCHANMASK		= BRDctrl_QM + 2,	// Set mask of QM9857 channels (U32)
	BRDctrl_QM_GETCHANMASK		= BRDctrl_QM + 3,	// Get mask of QM9857 channels (U32)
	BRDctrl_QM_SETCLKSOURCE		= BRDctrl_QM + 4,	// Set source of clock QM9857 (U32)
	BRDctrl_QM_GETCLKSOURCE		= BRDctrl_QM + 5,	// Get source of clock QM9857 (U32)
	BRDctrl_QM_SETSTARTMODE		= BRDctrl_QM + 6,   // Set QM9857 start mode (BRD_StartMode)
	BRDctrl_QM_GETSTARTMODE		= BRDctrl_QM + 7,   // Get QM9857 start mode (BRD_StartMode)
	BRDctrl_QM_FIFORESET		= BRDctrl_QM + 8,	// QM9857 FIFO reset (NULL)
	BRDctrl_QM_START			= BRDctrl_QM + 9,	// QM9857 Start
	BRDctrl_QM_STOP				= BRDctrl_QM + 10,	// QM9857 Stop
	BRDctrl_QM_FIFOSTATUS		= BRDctrl_QM + 11,	// Get QM9857 FIFO status (U32)
	BRDctrl_QM_PUTDATA			= BRDctrl_QM + 12,	// Put data buffer into QM9857 FIFO (PVOID)
	BRDctrl_QM_GETSRCSTREAM		= BRDctrl_QM + 13,	// Get source address for stream (U32)
	BRDctrl_QM_QMSETREG			= BRDctrl_QM + 14,	// Set register QM9857(BRD_QmSetReg)
	BRDctrl_QM_QMGETREG			= BRDctrl_QM + 15,	// Get register QM9857(BRD_QmSetReg)
	BRDctrl_QM_QMRESET			= BRDctrl_QM + 16,	// Reset QM9857(NULL)
	BRDctrl_QM_QMRESETFULL		= BRDctrl_QM + 17,	// Full reset QM9857(NULL)
	BRDctrl_QM_SELECTPROFILE	= BRDctrl_QM + 18,	// Select profile QM9857(U32)
	BRDctrl_QM_UPDATEFREQ		= BRDctrl_QM + 19,	// Update frequency QM9857(U32)
	BRDctrl_QM_SETPROFILE		= BRDctrl_QM + 20,	// Set profile QM9857(BRD_QmSetProfile)
	BRDctrl_QM_SETMAINPARAM		= BRDctrl_QM + 21,	// Set main param QM9857(BRD_QmMainParam)
	BRDctrl_QM_QMPROG			= BRDctrl_QM + 22,	// Program QM9857(NULL)

	BRDctrl_DAC_ILLEGAL
};

#pragma pack(push, 1)    


// for WriteQmReg and ReadQmReg  functions
typedef struct _BRD_QmSetReg
{
	U32		progMask;		// mask for programming QM9857 chips
	U32		addrReg;		// register address into QM9857 chip for programming
	U32		data;			// data for writing
} BRD_QmSetReg, *PBRD_QmSetReg;
//----------------------------------------------
// for  QmSetProfile function
typedef struct _BRD_QmSetProfile
{	U32 progMask,	 // mask for programming QM chips
		profNum,	 // number of profile 
		freqNco,	 // cod frequency NCO
		cicRate,	 // CIC rate
		spectrInv,	 // invert of spectr
		invCicBypass,// bypass of cic
		outputScale;	 // output scale
} BRD_QmSetProfile,*PBRD_QmSetProfile;

// for  QmSetMainParam function
typedef struct _BRD_QmSetMainParam
{	U32  progMask,		// mask for programming QM chips,
		 pllLock,		// lock PLL
		 clockMult,		// multiplier of Clock
		 cicClear,		// clear of CIC
		 invSincBypass,  // bypass of 
		 fullSleep,		// set mode Sleep
		 autoPowerDown, // set mode PowerDown
		 operatingMode ; //
} BRD_QmSetMainParam, *PBRD_QmSetMainParam;
#pragma pack(pop)    

#endif // _CTRL_QM9857_H

//
// End of file
//