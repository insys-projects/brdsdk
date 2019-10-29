/*
 ****************** File CtrlDio.h **************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : DIO section
 *
 * (C) InSys by Dorokhin Andrey Sep, 2006
 *
 *
 ************************************************************
*/

#ifndef _CTRL_DIO_H
 #define _CTRL_DIO_H

#include "ctrladmpro.h"

#pragma pack(push, 1)    

// DIO Clock sources - ver. 2.10 only !!!
enum {
	BRDclks_DIO_GEN75MHZ	= 0,	// Generator 75 MHz
	BRDclks_DIO_GEN87MHZ	= 1,	// Generator 87 MHz
	BRDclks_DIO_GEN100MHZ	= 2,	// Generator 100 MHz
	BRDclks_DIO_GEN120MHZ	= 3,	// Generator 120 MHz
};

// for set and get Clock mode
typedef struct _BRD_DioClkMode {
	U32		inv;	// IN - clock inverting (1)
	U32		src;	// IN - clock source  - ver. 2.10 only !!!
} BRD_DioClkMode, *PBRD_DioClkMode;

#endif // _CTRL_DIO_H

//
// End of file
//