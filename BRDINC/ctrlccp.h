/*
 ****************** File CtrlCCP.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : CCPINFO section
 *
 * (C) Instrumental Systems 
 *
 ************************************************************
*/

#ifndef _CTRL_CCP_H
 #define _CTRL_CCP_H

#include "ctrl.h"



//=********************************************************
//
// Command Codes
//
//=********************************************************

#define BRDctrl_CCP BRD_SECTIONSIZE*20

enum {
	BRDctrl_CCP_INFO		= BRDctrl_CCP + 0,	// Alloc CBuf if EXCLUSIVE CaptMode (struct)
	BRDctrl_CCP_Loop		= BRDctrl_CCP + 1,
	BRDctrl_CCP_RMEM		= BRDctrl_CCP + 2,
	BRDctrl_CCP_WRMEM		= BRDctrl_CCP + 3,
	BRDctrl_CCP_SetSleep	= BRDctrl_CCP + 4, 

	BRDctrl_CCP_ILLEGAL
};

//=********************************************************
//
// Structure Declarations
//
//=********************************************************

#pragma pack(push, 1) 

typedef struct{
	U32 Bus;
	U32 *p_info_str;
	U32 len;
}BRDctrl_CCPCCBUSInfo,*PBRDctrl_CCPCCBUSInfo;

typedef struct{

	U32 busType;
	void* wr_buf;
	void* rd_buf;
	U32 sizeOfbytes;

}BRDctrl_CCP_Loopback;

typedef struct{
	U32 busType;
	U32 addr; 
	void *buf; 
	S32 sizeOfbytes;
}BRDctrl_CCPRW_RAM;

typedef struct{
	U32 modeBase;
	U32 modeCCP;
}BRDctrl_CCPSleep;
#pragma pack(pop)    


//=********************************************************
//
// Constants
//
//=********************************************************



#endif // _CTRL_CCP_H

//
// End of file
//
