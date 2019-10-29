/***************************************************
*
* EXTN_DEX.H
*
* BRD_extension() cmd codes and data types
*
* (C) InSys by Dorokhin Nov, 2015
*
****************************************************/


#ifndef	__EXTN_DEX_H_
#define	__EXTN_DEX_H_


#include	"utypes.h"
#include	"ctrlstrm.h"

//
//=== BRD_extension() cmd codes
//

enum
{
	BRDextn_DEX_DMA_START_ADDR=0x7800,			// DEX start DMA address
	BRDextn_DEX_PARAM,					// DEX Parameters
	BRDextn_DEX_RESET,					// DEX Software Reset
};

#pragma pack(push, 1) 

typedef struct {
	U32	padr_dsp;			// IN - physical address of DSP
	U32	*vadr;				// OUT - virtual address of process
	U64	physadr;			// OUT - physical address on bus
	U32	maxsize;			// OUT - size of buffer
	U64 phyInt;				// OUT - DSP interrupt generate physical address
	U32	intFpgaCode;		// OUT - interrupt code from FPGA
} BRDextn_dexDmaStartAddr;

typedef struct {
	U08*			pDaqParam;	// DAQ Parameters Address	
	BRDstrm_Stub*	pStubParam;	// Stub Parameters Address	
} BRDextn_dexParam;

#pragma pack(pop)    

#endif	// __EXTN_DEX_H_ 

//
//  End of File
//


