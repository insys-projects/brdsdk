//=********************************************************
// BRDSTRM.H - BARDY Stream/CBUF Support Declaration Header File
//
//
//=********************************************************


#ifndef	__BRDSTRM_H_
#define	__BRDSTRM_H_

//
// Constants: State of CBUF
//

enum
{
	BRDstrm_STAT_RUN = 1,
	BRDstrm_STAT_STOP = 2,
	BRDstrm_STATt_DESTROY = 3
};

//
// Constants: Direction of CBUF
//

enum
{
	BRDstrm_DIR_IN = 0x1,				// To HOST
	BRDstrm_DIR_OUT = 0x2,				// From HOST
	BRDstrm_DIR_INOUT = 0x3				// BBoth Directions
};

#pragma pack(push,1)

//
// Shareable (Global) Control Structure of CBUF
//
typedef struct
{
	S08		rootBlockName[64];		// Root Name of Blocks
	U32		rootCounter;			// Counter to Form Root Name
	U32		blockNumber;			// Number of Blocks
	U32		blockSize;				// Size of every Block (bytes)
	U32		state;					// CBUF global state
	U32		dir;					// Direction
} BRDstrm_ShareControl;

//
// Block Structure
//
typedef struct
{
	S08		*pBlock;				// Block Pointer
	HANDLE	hFileMapping;			// Handle of Block
} BRDstrm_BlockPointer;

//
// Stub Structure
//
typedef struct
{
	U32		lastFilledBlock;		// Number of Block which was filled last Time
	U32		totalFilledBlock;		// Total Counter of filled Block
	U32		state;					// CBUF local state
} BRDstrm_Stub;

typedef struct
{
	BRDstrm_Stub	*pStub;			// Stub Pointer
	HANDLE			hFileMapping;	// Handle of Stub
} BRDstrm_StubPointer;

//
// Local (Process only) Control Structure of CBUF
//
typedef struct
{
	BRDstrm_ShareControl	*pShareControl;		// Shareable Control Structure of CBUF
	HANDLE					hFileMapping;		// Handle of shareable memory for "pShareControl"
	HANDLE					hMutex;				// Named Mutex to protect "pShareControl"
	U32						rootCounter;		// Counter of Current Root Name
	BRDstrm_BlockPointer	*pBlockPointers;	// Array of Block Pointers
	BRDstrm_StubPointer		stubPointer;		// Stub Pointer
} BRDstrm_Control;

#pragma pack(pop)


#endif	//__BRDSTRM_H_

//
// End of File
//

