/***************************************************
*
* EXTN_6678.H
*
* BRD_extension() cmd codes and data types
*
* (C) InSys by Dorokhin Nov, 2012
*
****************************************************/


#ifndef	__EXTN_6678_H_
#define	__EXTN_6678_H_


#include	"utypes.h"

//
//=== BRD_extension() cmd codes
//

enum
{
	BRDextn_DMA_START_ADDR,			// start DMA address
	BRDextn_SOFT_RESET,				// start DMA address
};

#pragma pack(push, 1) 

typedef struct {
	U32	padr_ti;			// IN - physical address of TI6678
	U32	*vadr;				// OUT - virtual address of process
	U64	physadr;			// OUT - physical address on bus
	U32	maxsize;			// OUT - size of buffer
	U64 phyInt;				// OUT - TI6678 interrupt generate physical address
	U32	intFpgaCode;		// OUT - interrupt code from FPGA
	U32	*cnt_vadr;			// OUT - interrupt DMA counter virtual address
	U64	cnt_physadr;		// OUT - interrupt DMA counter physical address
} BRDextn_dmaStartAddr;

typedef struct {
	U32	timeout;			// timeout after reset
} BRDextn_softReset;

#pragma pack(pop)    

#endif	// __EXTN_DO_H_ 

//
//  End of File
//


