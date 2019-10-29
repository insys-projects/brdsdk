//=********************************************************
//
// BRDAPI.H
//
// BRDI, DEV, DEVS and SIDE API Definition 
//
// (C) Instr.Sys. by Ekkore May 2002
//
// Modifications:
//   14.06.2002 Ekkore: Funcs SIDE_initData(), SIDE_initAuto are got new parameter 'baseName'.
//   14.06.2002 Ekkore: Func DEV_initData() are got new parameter 'idxThisBrd'.
//
//=********************************************************

#ifndef	__BRDAPI_H_
#define	__BRDAPI_H_

#include	"brd.h"


#ifdef	__cplusplus
extern "C" {
#endif

//
//====== Constants
//

#define SERVNAME_SIZE 16
#define BOARDNAME_SIZE 32

//
//====== BRDI API Functions
//

#ifndef BRDI_API
	#if defined(__linux__) || defined(__QNX__)
		#define BRDI_API
	#else
		#ifdef BRDI_API_EXPORTS
			#define BRDI_API __declspec(dllexport)
		#else
			#define BRDI_API __declspec(dllimport)
		#endif
	#endif
#endif  //BRDI_API 

BRDI_API S32	STDCALL BRDI_setError( BRD_Error *pError );
BRDI_API int	STDCALL BRDI_print ( S32 errLevel, const BRDCHAR *title, const BRDCHAR *msg );
BRDI_API int	STDCALL BRDI_printf( S32 errLevel, const BRDCHAR *title, const BRDCHAR *format, va_list arglist );
//BRDI_API int	STDCALL BRDI_feof( FILE *stream);
//BRDI_API char*	STDCALL BRDI_fgets( char *string, int n, FILE *stream );
//BRDI_API long	STDCALL BRDI_ftell( FILE *stream );
//BRDI_API int	STDCALL BRDI_fseek( FILE *stream, long offset, int origin );
BRDI_API S32	STDCALL BRDI_support( S32 cmd, void *pArgs );

enum
{
	BRDIsup_NONE,
	BRDIsup_CAPTURE,
	BRDIsup_RELEASE,
	BRDIsup_MAX
};

#pragma pack(push,1)

typedef struct { BRD_Handle handle; U32 nodeId; S32 idxMain; U32 captMode; } BRDI_SUP_Capture, *PBRDI_SUP_Capture;
typedef struct { BRD_Handle handle; U32 nodeId; S32 idxMain; U32 captMode; } BRDI_SUP_Release, *PBRDI_SUP_Release;

//
//====== DEV API Functions and Types
//

#ifndef DEV_API
	#if defined(__linux__) || defined(__QNX__)
		#define DEV_API
	#else
		#ifdef DEV_API_EXPORTS
			#define DEV_API __declspec(dllexport)
		#else
			#define DEV_API __declspec(dllimport)
		#endif
	#endif
#endif  //DEV_API 

typedef struct { U32 val; BRDCHAR name[MAX_PATH]; } TBRD_SubEnum, BRD_SubEnum, *PBRD_SubEnum;
typedef struct { BRDCHAR key[32]; BRDCHAR val[MAX_PATH]; } TBRD_InitData, BRD_InitData, *PBRD_InitData;

typedef S32		STDCALL DEV_API_initData( S32 idxThisBrd, void **ppDev, BRDCHAR *pBoardName, TBRD_InitData *pInitData, S32 size );
typedef S32		STDCALL DEV_API_initAuto( S32 idxThisBrd, void **ppDev, BRDCHAR *pBoardName, BRDCHAR *pLin, S32 linSize, TBRD_SubEnum *pSubEnum, U32 sizeSubEnum );
typedef S32		STDCALL DEV_API_entry( void *pDev, S32 cmd, void *args );

//DEV_API S32		STDCALL DEV_initData( TBRD_InitData *pInitData, S32 size, char *pBoardName, void **ppDev, S32 idxThisBrd );
DEV_API S32		STDCALL DEV_initData( S32 idxThisBrd, void **ppDev, BRDCHAR *pBoardName, TBRD_InitData *pInitData, S32 size );
DEV_API S32		STDCALL DEV_initAuto( S32 idxThisBrd, void **ppDev, BRDCHAR *pBoardName, BRDCHAR *pLin, S32 linSize, TBRD_SubEnum *pSubEnum, U32 sizeSubEnum );
DEV_API S32		STDCALL DEV_entry( void *pDev, S32 cmd, void *args );

enum
{
	DEVcmd_INIT,
	DEVcmd_CLEANUP,
	DEVcmd_REINIT,
	DEVcmd_CHECKEXISTANCE,
	DEVcmd_OPEN,
	DEVcmd_CLOSE,
	DEVcmd_GETINFO,
	DEVcmd_LOAD,
	DEVcmd_PULOAD,
	DEVcmd_PUSTATE,
	DEVcmd_PULIST,
	DEVcmd_RESET,
	DEVcmd_START,
	DEVcmd_STOP,
	DEVcmd_SYMBOL,
	DEVcmd_VERSION,
	DEVcmd_CAPTURE,
	DEVcmd_RELEASE,
	DEVcmd_CTRL,
	DEVcmd_EXTENSION,
	DEVcmd_PEEK,
	DEVcmd_POKE,
	DEVcmd_READRAM,
	DEVcmd_WRITERAM,
	DEVcmd_READFIFO,
	DEVcmd_WRITEFIFO,
	DEVcmd_READDPRAM,
	DEVcmd_WRITEDPRAM,
	DEVcmd_READ,
	DEVcmd_WRITE,
	DEVcmd_GETMSG,
	DEVcmd_PUTMSG,
	DEVcmd_SIGNALSEND,
	DEVcmd_SIGNALGRAB,
	DEVcmd_SIGNALWAIT,
	DEVcmd_SIGNALIACK,
	DEVcmd_SIGNALFRESH,
	DEVcmd_SIGNALINFO,
	DEVcmd_SIGNALLIST,
	DEVcmd_DEVHARDWAREINIT,
	DEVcmd_DEVHARDWARECLEANUP,
	DEVcmd_SIDEHARDWAREINIT,
	DEVcmd_SIDEHARDWARECLEANUP,
	DEVcmd_GETSERVLIST,
	DEVcmd_PUREAD,
	DEVcmd_PUWRITE,
	DEVcmd_SERVCMDQUERY,
	DEVcmd_MAX
};

typedef struct { U32 dummy; }                                        DEV_CMD_Init, *PDEV_CMD_Init;
typedef struct { U32 dummy; }                                        DEV_CMD_Cleanup, *PDEV_CMD_Cleanup;
typedef struct { U32 dummy; }                                        DEV_CMD_Reinit, *PDEV_CMD_Reinit;
typedef struct { U32 *state; }                                       DEV_CMD_CheckExistance, *PDEV_CMD_CheckExistance;
typedef struct { U32 flag; void *ptr; }                              DEV_CMD_Open, *PDEV_CMD_Open;
typedef struct { U32 dummy; }                                        DEV_CMD_Close, *PDEV_CMD_Close;
typedef struct { BRD_Info *pInfo; }                                  DEV_CMD_Getinfo, *PDEV_CMD_Getinfo;
typedef struct { U32 nodeId; const BRDCHAR *fileName; int argc; void *argv; } DEV_CMD_Load, *PDEV_CMD_Load;
typedef struct { U32 puId; const BRDCHAR *fileName; U32 *state; }		 DEV_CMD_PuLoad, *PDEV_CMD_PuLoad;
typedef struct { U32 puId; U32 *state; }							 DEV_CMD_PuState, *PDEV_CMD_PuState;
typedef struct { BRD_PuList *pList; U32 item; U32 *pItemReal; }      DEV_CMD_PuList, *PDEV_CMD_PuList;
typedef struct { U32 puId; U32 offset; void *pBuf; U32 size; }		 DEV_CMD_PuRead, *PDEV_CMD_PuRead;
typedef struct { U32 puId; U32 offset; void *pBuf; U32 size; }		 DEV_CMD_PuWrite, *PDEV_CMD_PuWrite;
typedef struct { U32 nodeId; }                                       DEV_CMD_Reset, *PDEV_CMD_Reset;
typedef struct { U32 nodeId; }                                       DEV_CMD_Start, *PDEV_CMD_Start;
typedef struct { U32 nodeId; }                                       DEV_CMD_Stop, *PDEV_CMD_Stop;
typedef struct { BRDCHAR *fileName; BRDCHAR *symbName; U32 *val; }         DEV_CMD_Symbol, *PDEV_CMD_Symbol;
typedef struct { BRD_Version *pVersion; }                            DEV_CMD_Version, *PDEV_CMD_Version;
typedef struct { S32 handle; U32 nodeId; }                           DEV_CMD_Capture, *PDEV_CMD_Capture;
typedef struct { S32 handle; U32 nodeId; }                           DEV_CMD_Release, *PDEV_CMD_Release;
typedef struct { S32 handle; U32 nodeId; U32 cmd; void *arg; }       DEV_CMD_Ctrl, *PDEV_CMD_Ctrl;
typedef struct { U32 nodeId; U32 cmd; void *arg; }                   DEV_CMD_Extension, *PDEV_CMD_Extension;
typedef struct { U32 nodeId; U32 brdAdr; U32 val; }                  DEV_CMD_Peek, *PDEV_CMD_Peek;
typedef struct { U32 nodeId; U32 brdAdr; U32 val; }                  DEV_CMD_Poke, *PDEV_CMD_Poke;

typedef struct { U32 nodeId; U32 brdAdr; void *hostAdr; U32 itemNum; U32 itemSize; } DEV_CMD_ReadRAM, *PDEV_CMD_ReadRAM;
typedef struct { U32 nodeId; U32 brdAdr; void *hostAdr; U32 itemNum; U32 itemSize; } DEV_CMD_WriteRAM, *PDEV_CMD_WriteRAM;
typedef struct { U32 nodeId; U32 brdAdr; void *hostAdr; U32 size; U32 timeout; }     DEV_CMD_ReadFIFO, *PDEV_CMD_ReadFIFO;
typedef struct { U32 nodeId; U32 brdAdr; void *hostAdr; U32 size; U32 timeout; }     DEV_CMD_WriteFIFO, *PDEV_CMD_WriteFIFO;
typedef struct { U32 nodeId; U32 offset; void *hostAdr; U32 size; }                  DEV_CMD_ReadDPRAM, *PDEV_CMD_ReadDPRAM;
typedef struct { U32 nodeId; U32 offset; void *hostAdr; U32 size; }                  DEV_CMD_WriteDPRAM, *PDEV_CMD_WriteDPRAM;
typedef struct { U32 nodeId; void *hostAdr; U32 size; U32 timeout; }                 DEV_CMD_Read, *PDEV_CMD_Read;
typedef struct { U32 nodeId; void *hostAdr; U32 size; U32 timeout; }                 DEV_CMD_Write, *PDEV_CMD_Write;

typedef struct { U32 nodeId; void *hostAdr; U32 size; U32 timeout; } DEV_CMD_GetMsg, *PDEV_CMD_GetMsg; 
typedef struct { U32 nodeId; void *hostAdr; U32 size; U32 timeout; } DEV_CMD_PutMsg, *PDEV_CMD_PutMsg;

typedef struct { U32 nodeId; void *hostAdr; U32 size; U32 timeout; } DEV_CMD_ReadWrite, *PDEV_CMD_ReadWrite; 

typedef struct { U32 nodeId; U32 sigId; }								DEV_CMD_SignalSend, *PDEV_CMD_SignalSend;
typedef struct { U32 nodeId; U32 sigId; U32 sigCounter; U32 timeout; }  DEV_CMD_SignalGrab, *PDEV_CMD_SignalGrab;
typedef struct { U32 nodeId; U32 sigId; U32 sigCounter; U32 timeout; }  DEV_CMD_SignalWait, *PDEV_CMD_SignalWait;
typedef struct { U32 nodeId; U32 sigId; }                               DEV_CMD_SignalIack, *PDEV_CMD_SignalIack;
typedef struct { U32 nodeId; U32 sigId; U32 sigCounter; }               DEV_CMD_SignalFresh, *PDEV_CMD_SignalFresh;
typedef struct { U32 nodeId; U32 sigId; BRD_SigInfo *pInfo; }           DEV_CMD_SignalInfo, *PDEV_CMD_SignalInfo;
typedef struct { U32 nodeId; BRD_SigList *pList; U32 item; U32 *pItemReal; }     DEV_CMD_SignalList, *PDEV_CMD_SignalList;

typedef struct { U32 nodeId; }                                       DEV_CMD_DevHardwareInit, *PDEV_CMD_DevHardwareInit;
typedef struct { U32 nodeId; }                                       DEV_CMD_DevHardwareCleanup, *PDEV_CMD_DevHardwareCleanup;
typedef struct { U32 nodeId; }                                       DEV_CMD_SideHardwareInit, *PDEV_CMD_SideHardwareInit;
typedef struct { U32 nodeId; }                                       DEV_CMD_SideHardwareCleanup, *PDEV_CMD_SideHardwareCleanup;

typedef struct
{
	BRDCHAR	name[SERVNAME_SIZE];		// Service name with(15byte) / without(12byte) Number
	U32		attr;			// Attributes of Service (Look BRDserv_ATTR_XXX constants)
	S32		idxMain;		// Main Service List Index
} DEV_CMD_ServListItem, *PDEV_CMD_ServListItem;

typedef struct
{
	U32						item;			// IN:  Items Number into pRLI Array
	U32						itemReal;		// OUT: Number of Services
	DEV_CMD_ServListItem	*pSLI;			// OUT: Array, that returns Service Infos
} DEV_CMD_GetServList, *PDEV_CMD_GetServList;

//
//==== DEVS API Functions
//

#ifndef DEVS_API
	#if defined(__linux__) || defined(__QNX__)
		#define DEVS_API
	#else
		#ifdef DEVS_API_EXPORTS
			#define DEVS_API __declspec(dllexport)
		#else
			#define DEVS_API __declspec(dllimport)
		#endif
	#endif
#endif  // DEVS_API

typedef S32  STDCALL DEVS_API_entry( void *pDev, void *pContext, S32 cmd, void *args );
typedef S32	 STDCALL DEVS_API_PropDlg(void *pDev, void *pContext, void *args);

DEVS_API S32  STDCALL DEVS_entry( void *pDev, void *pContext, S32 cmd, void *args );
DEVS_API S32  STDCALL DEVS_propDlg(void *pDev, void *pContext, void* args);

enum
{
	DEVScmd_REGREADDIR,
	DEVScmd_REGREADSDIR,
	DEVScmd_REGREADIND,
	DEVScmd_REGREADSIND,
	DEVScmd_REGWRITEDIR,
	DEVScmd_REGWRITESDIR,
	DEVScmd_REGWRITEIND,
	DEVScmd_REGWRITESIND,
	DEVScmd_SETSTATIRQ,
	DEVScmd_CLEARSTATIRQ,
	DEVScmd_GETBASEADR,
    DEVScmd_WAITSTATIRQ,
	DEVScmd_PACKEXECUTE
};

typedef struct {  S32 idxMain; S32 tetr; S32 reg; U32 val; void *pBuf; S32 bytes; } DEVS_CMD_Reg, *PDEVS_CMD_Reg;
// for DEVScmd_SETSTATIRQ & DEVScmd_CLEARSTATIRQ: reg = irqmask; val = irqinv; pBuf = hEvent
// DEVScmd_PACKEXECUTE uses bytes and pBuf - points at DEVS_CMD_Reg: idxMain = cmd (DEVScmd_REGREADDIR = 0,	DEVScmd_REGREADIND = 2,	DEVScmd_REGWRITEDIR = 4, DEVScmd_REGWRITEIND = 6)

//
//==== SIDE API Functions
//

#ifndef SIDE_API
	#if defined(__linux__) || defined(__QNX__)
		#define SIDE_API
	#else
		#ifdef SIDE_API_EXPORTS
			#define SIDE_API __declspec(dllexport)
			#else
		#define SIDE_API __declspec(dllimport)
		#endif
	#endif
#endif  //SIDE_API 


typedef S32  STDCALL SIDE_API_initData( void **ppSide, PVOID pBufICR, PBRD_InitData pInitData, S32 size );
typedef S32  STDCALL SIDE_API_initAuto( void **ppSide, PVOID pBufICR, BRDCHAR *pLin, S32 linSize );
typedef S32  STDCALL SIDE_API_entry( void *pSide, void *pContext, S32 cmd, void *args );
typedef S32  STDCALL SIDE_API_PropDlg(void *pSide, void *pContext, void *args);

SIDE_API S32  STDCALL SIDE_initData( void **ppSide, PVOID pBufICR, PBRD_InitData pInitData, S32 size );
SIDE_API S32  STDCALL SIDE_initAuto( void **ppSide, PVOID pBufICR, BRDCHAR *pLin, S32 linSize );
SIDE_API S32  STDCALL SIDE_entry( void *pSide, void *pContext, S32 cmd, void *args );
SIDE_API S32  STDCALL SIDE_propDlg(void *pSide, void *pContext, void* args);

enum {
	SIDEcmd_INIT,
	SIDEcmd_CLEANUP,
	SIDEcmd_OPEN,
	SIDEcmd_CLOSE,
	SIDEcmd_GETINFO,
	SIDEcmd_PULOAD,
	SIDEcmd_PUSTATE,
	SIDEcmd_PULIST,
	SIDEcmd_VERSION,
	SIDEcmd_CAPTURE,
	SIDEcmd_RELEASE,
	SIDEcmd_CTRL,
	SIDEcmd_HARDWAREINIT,
	SIDEcmd_HARDWARECLEANUP,
	SIDEcmd_SETSERVLIST,
	SIDEcmd_GETSERVLIST,
	SIDEcmd_PUREAD,
	SIDEcmd_PUWRITE,
	SIDEcmd_SERVCMDQUERY,
	SIDEcmd_MAX
};

typedef struct
{
	PVOID			pDev;
	DEVS_API_entry*	pEntry;
	BRDCHAR			boardName[BOARDNAME_SIZE];
	HANDLE			hMutex;
} SIDE_CMD_Init, *PSIDE_CMD_Init;

typedef DEV_CMD_Ctrl SIDE_CMD_Ctrl, *PSIDE_CMD_Ctrl;

typedef DEV_CMD_GetServList SIDE_CMD_GetServList, *PSIDE_CMD_GetServList,
							SIDE_CMD_SetServList, *PSIDE_CMD_SetServList;

enum
{
	SERVcmd_SYS_ISAVAILABLE = 1,
	SERVcmd_SYS_CAPTURE,
	SERVcmd_SYS_RELEASE,
	SERVcmd_SYS_GETADDRDATA
};

typedef struct { U32 isAvailable, attr; } SERV_CMD_IsAvailable, *PSERV_CMD_IsAvailable; 


#pragma pack(pop)

//
//
//

#ifdef	__cplusplus
};  //extern "C"
#endif


#endif	// __BRDAPI_H_

//
// End of File
//

 
