/***************************************************
*
* EXTN_EK.H
*
* BRD_extension() cmd codes and data types
*
* (C) Instr.Sys. by Ekkore Dec, 1997-2001
*
****************************************************/


#ifndef	__EXTN_EK_H_
#define	__EXTN_EK_H_


#include	"utypes.h"

//
//=== BRD_extension() cmd codes
//

enum
{
	BRDextn_BASE_EK	= 0x4800,		// Base addres of code range
	BRDextn_SYSTEM_MEM_ALLOC_EK,	// Allocate nonpageble block of system memory
	BRDextn_SYSTEM_MEM_FREE_EK,		// Free nonpageble block of system memory
	BRDextn_PHYS_MEM_MAP_EK,		// Map Physical to Virtual Memory
	BRDextn_PHYS_MEM_UNMAP_EK,		// UnMap previousle mapped Virtual Memory
	BRDextn_ASYNC_DPRAM_RW_START_EK,// Start Access to DPRAM with DMA
	BRDextn_ASYNC_DPRAM_RW_WAIT_EK,	// Wait End of Access to DPRAM with DMA
	BRDextn_SET_DPRAM_WAIT_STATE_EK,// Specify WS of DPRAM
	BRDextn_SET_HPI_FAST_MODE_EK,	// Toggle Fast/Slow mode of HPI Access
	BRDextn_SET_SEMAPHORE_EK,		// Write to HW semaphore #0 register 
	BRDextn_GET_SEMAPHORE_EK,		// Get HW semaphore #0 state
	BRDextn_GET_DEVICE_HANDLE_EK,	// Get handle of WDM driver
	BRDextn_OWN_SEMAPHORE_EK,		// Try to own semaphore #n 
	BRDextn_FREE_SEMAPHORE_EK,		// Free semaphore #n 
	BRDextn_CHECK_SEMAPHORE_EK,		// Check state of semaphore #n (1-own, 0-not own)
	BRDextn_WAIT_SEMAPHORE_EK,		// Wait semaphore #n while it will be own
	BRDextn_SET_STREAM_CODESTART_EK,// Set Code of Start (DMAMODE reg) for PLX DMA Channel
	BRDextn_MEM_CHAIN_EK,			// Get Chain of Paged Memory Block
	BRDextn_MEM_UNCHAIN_EK,			// Free Chain of Paged Memory Block
	BRDextn_FIFO_RESET_EK,			// Clear FIFO
	BRDextn_GET_VIRT_ADR_EK			// Get Virtual Address of BAR0-BAR6
};

//
//=== BRD_extension() data types
//

typedef struct
{
	U32	offset;		// Start CRAM offset
	void	*hostAdr;	// Start HOST far address
	U32	len;		// Length in bytes
	U32	isRead;		// 0-Write, 1-Read
} TAsyncCRamRdWr;

typedef struct
{
	U32			size;			// Size of MemBlock (bytes)
	U32			linAdr;			// Linear Adr of MemBlock
	U32			phAdr;			// Phys Adr of MemBlock
} TUninsysMemBlock;

typedef struct { U32 phyAdr, sizeb; } TMemChainLoop;
typedef struct
{
	U32			size;			// Size of MemBlock (bytes)
	U32			linAdr;			// Linear Adr of MemBlock
	U32			itemNum;		// Size of ChainArray
	U32			itemNumReal;	// Real Number of Chain Loops
	TMemChainLoop *paChainArray;// Array of Chain Loops
} TGetMemChain;


#endif	// __EXTN_EK_H_ 

//
//  End of File
//


