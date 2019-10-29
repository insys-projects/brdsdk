/***************************************************
*
* BRD.H
*
* BRD Shell Declaration
*
* (C) Instr.Sys. by Ekkore Dec, 1997-2001
*
****************************************************/


#ifndef	__BRD_H_
#define	__BRD_H_


#include	<stdio.h>
#include	<stdarg.h>
#include	"utypes.h"
#include	"brderr.h"

//
//==== Types
//

#ifndef BRD_API
	#ifdef  WIN32
	#define	BRD_API		DllImport
	#else
	#define	BRD_API
	#endif	//WIN32
#endif  //BRD_API

//
//==== Node IDs
//

enum { NODEALL = -1, NODENONE = 0, NODE0 = 1, NODE1 = 2, NODE2 = 4, NODE3 = 8 };


//
//=== Timeout
//

#define BRDtim_AT_ONCE  0x0
#define BRDtim_FOREVER  0xFFFFFFFFL


//
//=== BRD_init() flags
//
enum
{
	BRDinit_FILE_KNOWN	= 0,	// Well known INI File
	BRDinit_FILE_ENV	= 1,	// INI File is difined with Environment
	BRDinit_FILE		= 2,	// Pointed INI File
	BRDinit_REGISTRY	= 3,	// Registry
	BRDinit_STRING		= 4,	// input string
	BRDinit_LAZY		= 5,	// lazy
	BRDinit_AUTOINIT	= 0x10	// Auto Initialization Mode
};

#pragma pack(push,1)
//
//=== BRD_shell() Command Codes and Structures
//
enum
{
	BRDshl_LID_LIST = 1		// Get the LID List of All Initialized Boards
};

typedef struct
{
	U32		item;			// IN:  Number of Items into pLID[] Array
	U32		itemReal;		// OUT: Number of Initialized LIDs
	U32		*pLID;			// OUT: Array to return LIDs
} BRD_LidList, *PBRD_LidList;

//
//=== BRD_open() flags
//
enum
{
	BRDopen_EXCLUSIVE = 0x0,	// Board must be exclusive
	BRDopen_SHARED = 0x1,		// Board must be shared
	BRDopen_SPY = 0x2,			// Board is used for monitoring
	BRDopen_HIGHEST = 0x3,		// Board may be shared or spy

	BRDopen_ENUMMAX
};

//
//=== BRD_displayMode() modes
//

enum 
{ 
	BRDdm_UNVISIBLE = 0x0,	//Display nothing
	BRDdm_VISIBLE   = 0xF,	//Display all message
	BRDdm_CONSOLE   = 0x80,	//Display message to console
	BRDdm_INFO      = 0x1,	//Display info message
	BRDdm_WARN      = 0x2,	//Display warning message
	BRDdm_ERROR     = 0x4,	//Display error message
	BRDdm_FATAL     = 0x8	//Display fatal error message
};

//
//=== BRD_version() types
//

#define	VER_NONE	0xFFFFFFFFL
typedef struct
{
	U32		size;		// sizeof(BRD_Version)
	S32		brdMajor;	// BRD Shell Major Version
	S32		brdMinor;	// BRD Shell Minor Version
	S32		drvMajor;	// BRD Driver Major Version
	S32		drvMinor;	// BRD Driver Minor Version
} BRD_Version, *PBRD_Version;

//
//=== BRD_getInfo() types
//

typedef struct
{
	U32		size;		// sizeof(BRD_Info)
	S32		code;		// reserved
	U32		boardType;	// Board Type
    BRDCHAR	name[128];	// Device Name ASCII
	U32		pid;		// Board Phisical ID
    S32		busType;	// Bus Type (Unknown, PCI, ISA, RS232, 1394, USB, PCMCIA, VME, ... )
    S32		bus;		// Bus Number
    S32		dev;		// Dev Number
    S32		slot;		// Slot Number
	U32		verMajor;	// Driver Version
	U32		verMinor;	// Driver Version
	U32		subunitType[16];// Subunit Type Code
	U32		base[8];	// Base Addresses
	U32		vectors[4];	// Interrupt Vectors
} BRD_Info, *PBRD_Info;

enum					// Bus Types
{
	BRDbus_Unknown, 
	BRDbus_PCI, 
	BRDbus_ISA, 
	BRDbus_RS232, 
	BRDbus_1394, 
	BRDbus_USB, 
	BRDbus_PCMCIA, 
	BRDbus_VME,
	BRDbus_COMPACTPCI,
	BRDbus_ETHERNET,
	BRDbus_FOTR
};

//
//=== BRD_puList() types
//

typedef struct
{
	U32		puId;				// Programmable Unit ID
	U32		puCode;				// Programmable Unit Code
	U32		puAttr;				// Programmable Unit Attribute
	BRDCHAR	puDescription[128];	// Programmable Unit Description Text
} BRD_PuList, *PBRD_PuList;

//
//=== BRD_servList() types
//

enum
{
	BRDserv_ATTR_STREAMABLE_IN		= 0x1,
	BRDserv_ATTR_STREAMABLE_OUT		= 0x2,
	BRDserv_ATTR_DIRECTION_IN		= 0x4,
	BRDserv_ATTR_DIRECTION_OUT		= 0x8,
	BRDserv_ATTR_EXCLUSIVE_ONLY		= 0x10,
	BRDserv_ATTR_UNVISIBLE			= 0x20,
	BRDserv_ATTR_SUBSERVICE_ONLY	= 0x40,
	BRDserv_ATTR_STREAM				= 0x100,
	BRDserv_ATTR_SVEAM				= 0x200,
	BRDserv_ATTR_CMPABLE			= 0x400,
//	BRDserv_ATTR_SDRAM				= 0x800,
	BRDserv_ATTR_ADCABLE			= 0x800,
	BRDserv_ATTR_SDRAMABLE			= 0x1000,
	BRDserv_ATTR_DSPABLE			= 0x2000
};

typedef struct
{
	BRDCHAR	name[16];		// Service name with Number
	U32		attr;			// Attributes of Service (Look BRDattr_XXX constants)
} BRD_ServList, *PBRD_ServList;

//
//=== BRD_signalList() types
// 

typedef struct
{
	U32		sigId;					// IRQ ID
	U32		sigCode;				// IRQ Code
	U32		sigAttr;				// IRQ Attribute
	BRDCHAR	sigDescription[128];	// IRQ Description Text
} BRD_SigList, *PBRD_SigList;

enum					// Signal Attributes
{
	BRDsig_ATTR2HOST   = 0x1,		// Signal from Device to Host
	BRDsig_ATTR2DEVICE = 0x2		// Signal from Host to Device
};

//
//=== BRD_signalInfo() types
//

typedef struct
{
	U32		size;
	U32		signalCounter;			// Curren Value of IRQ Counter
	U32		pinCounter;				// Curren Value of all IRQ to the pin
	U32		lastSignalTime;			// Time From Last IRQ
} BRD_SigInfo, *PBRD_SigInfo;

//
//=== BRD_error() types
//

typedef struct
{
	S32		errCode;			// Error Code
	S32		errTime;			// Error Time
	BRDCHAR	srcModuleName[128];	// Module Name produced the Error
	BRDCHAR	srcFileName[128];	// File Name produced the Error
	S32		srcLine;			// Line produced the Error 
	BRDCHAR	errText[1024];		// Error Text
} BRD_Error, *PBRD_Error;


//
//=== BRD_capture() flags
//
enum
{
	BRDcapt_EXCLUSIVE = 0x0,	// To Control Service Exclusively
	BRDcapt_SHARED = 0x1,		// To Share Service between some Applications
	BRDcapt_SPY = 0x2			// To Observate Service State
};

#define BRD_mkBaseHandle(h)	((h)&0xFFFF)

//
//=== BRD_Handle type and Others
//

typedef	S32		BRD_Handle, *PBRD_Handle;

#pragma pack(pop)

//
//====== Functions Declaration
//

#ifdef	__cplusplus
extern "C" {
#endif

BRD_API S32		STDCALL BRD_init( const BRDCHAR *iniFile, S32 *pNum );
BRD_API S32		STDCALL BRD_initEx( U32 mode, const void *pSrc, const BRDCHAR *logFile, S32 *pNum );
BRD_API S32		STDCALL BRD_reinit( S32 *pNum );
BRD_API S32		STDCALL BRD_cleanup( void );
BRD_API S32		STDCALL BRD_shell( U32 cmd, void *arg );
BRD_API S32		STDCALL BRD_lidCtrl( U32 lid, U32 nodeId, U32 cmd, void *arg );
BRD_API S32		STDCALL BRD_lidList( U32 *pList, U32 item, U32 *pItemReal );

BRD_API BRD_Handle	STDCALL BRD_open (U32 lid, U32 flag, void *ptr ); 

BRD_API BRD_Handle	STDCALL BRD_openEx( const BRDCHAR *init, U32 flag, void *ptr);

BRD_API S32		STDCALL BRD_close(BRD_Handle handle );
BRD_API S32		STDCALL BRD_getInfo( U32 lid, BRD_Info *pInfo );
BRD_API S32		STDCALL BRD_getInfoEx(BRD_Handle handle, U32 *lid, BRD_Info *pInfo);


BRD_API S32		STDCALL BRD_load   (BRD_Handle handle, U32 nodeId, const BRDCHAR *fileName, int argc, BRDCHAR *argv[] );
BRD_API S32		STDCALL BRD_puLoad (BRD_Handle handle, U32 puId, const BRDCHAR *fileName, U32 *state );
BRD_API S32		STDCALL BRD_puState(BRD_Handle handle, U32 puId, U32 *state );
BRD_API S32		STDCALL BRD_puList (BRD_Handle handle, BRD_PuList *pList, U32 item, U32 *pItemReal );
BRD_API S32		STDCALL BRD_puRead (BRD_Handle handle, U32 puId, U32 offset, void *hostAdr, U32 size );
BRD_API S32		STDCALL BRD_puWrite(BRD_Handle handle, U32 puId, U32 offset, void *hostAdr, U32 size );
BRD_API S32		STDCALL BRD_puEnable(BRD_Handle handle, U32 puId );

BRD_API S32		STDCALL BRD_reset(BRD_Handle handle, U32 nodeId );
BRD_API S32		STDCALL BRD_start(BRD_Handle handle, U32 nodeId );
BRD_API S32		STDCALL BRD_stop (BRD_Handle handle, U32 nodeId );
BRD_API S32		STDCALL BRD_symbol (BRD_Handle handle, const BRDCHAR *fileName, const BRDCHAR *symbName, U32 *val );
BRD_API S32		STDCALL BRD_version(BRD_Handle handle, BRD_Version *pVersion );

BRD_API BRD_Handle STDCALL BRD_capture(BRD_Handle handle, U32 nodeId, U32 *pMode, const BRDCHAR *servName, U32 timeout );
BRD_API S32		STDCALL BRD_release( BRD_Handle handle, U32 nodeId );
BRD_API S32		STDCALL BRD_serviceList( BRD_Handle handle, U32 nodeId, BRD_ServList *pList, U32 item, U32 *pItemReal );
BRD_API S32		STDCALL BRD_ctrl(BRD_Handle handle, U32 nodeId, U32 cmd, void *arg );
BRD_API S32		STDCALL BRD_extension(BRD_Handle handle, U32 nodeId, U32 cmd, void *arg );

BRD_API U32		STDCALL BRD_peek     (BRD_Handle handle, U32 nodeId, U32 brdAdr );
BRD_API S32		STDCALL BRD_poke     (BRD_Handle handle, U32 nodeId, U32 brdAdr, U32 val );
BRD_API S32		STDCALL BRD_readRAM  (BRD_Handle handle, U32 nodeId, U32 brdAdr, void *hostAdr, U32 itemNum, U32 itemSize );
BRD_API S32		STDCALL BRD_writeRAM (BRD_Handle handle, U32 nodeId, U32 brdAdr, void *hostAdr, U32 itemNum, U32 itemSize );
BRD_API S32		STDCALL BRD_readFIFO (BRD_Handle handle, U32 nodeId, U32 brdAdr, void *hostAdr, U32 size, U32 timeout );
BRD_API S32		STDCALL BRD_writeFIFO(BRD_Handle handle, U32 nodeId, U32 brdAdr, void *hostAdr, U32 size, U32 timeout );
BRD_API S32		STDCALL BRD_readDPRAM (BRD_Handle handle, U32 nodeId, U32 offset, void *hostAdr, U32 size );
BRD_API S32		STDCALL BRD_writeDPRAM(BRD_Handle handle, U32 nodeId, U32 offset, void *hostAdr, U32 size );
BRD_API S32		STDCALL BRD_getMsg(BRD_Handle handle, U32 nodeId, void *hostAdr, U32 *pBytes, U32 timeout );
BRD_API S32		STDCALL BRD_putMsg(BRD_Handle handle, U32 nodeId, void *hostAdr, U32 *pBytes, U32 timeout );

BRD_API S32		STDCALL BRD_read (BRD_Handle handle, U32 nodeId, void *hostAdr, U32 size, U32 timeout );
BRD_API S32		STDCALL BRD_write(BRD_Handle handle, U32 nodeId, void *hostAdr, U32 size, U32 timeout );

BRD_API S32		STDCALL BRD_signalSend(BRD_Handle handle, U32 nodeId, U32 sigId );
BRD_API S32		STDCALL BRD_signalGrab(BRD_Handle handle, U32 nodeId, U32 sigId, U32 *pSigCounter, U32 timeout );
BRD_API S32		STDCALL BRD_signalWait(BRD_Handle handle, U32 nodeId, U32 sigId, U32 *pSigCounter, U32 timeout );
BRD_API S32		STDCALL BRD_signalIack(BRD_Handle handle, U32 nodeId, U32 sigId );
BRD_API S32		STDCALL BRD_signalFresh(BRD_Handle handle, U32 nodeId, U32 sigId, U32 *pSigCounter );
BRD_API S32		STDCALL BRD_signalInfo(BRD_Handle handle, U32 nodeId, U32 sigId, BRD_SigInfo *pInfo );
BRD_API S32		STDCALL BRD_signalList(BRD_Handle handle, U32 nodeId, BRD_SigList *pList, U32 item, U32 *pItemReal );

BRD_API void  	STDCALL BRD_displayMode(S32 errLevel );
BRD_API S32	  	STDCALL BRD_error( BRD_Error **ppError );

BRD_API S32		STDCALL BRD_lock(BRD_Handle handle, U32 nodeId, const BRDCHAR * resName, U32 timeout );
BRD_API S32		STDCALL BRD_unlock(BRD_Handle handle, U32 nodeId, const BRDCHAR * resName );

#ifdef	__cplusplus
};
#endif

#endif	// __BRD_H_ 

//
//  End of File
//


