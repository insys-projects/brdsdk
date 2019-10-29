//=********************************************************
// BRDSERV.H - BARDY Resource Support Declaration Header File
//
//=********************************************************

#ifndef	__BRDRSRC_H_
#define	__BRDRSRC_H_


//=********************************************************
//=********************************************************
//=********************************************************
//
// BRD Shell Support
//
//
//

#pragma pack(push,1)

//=******************** RES_BRD_ResList *******************
// For BRD Shell
//=********************************************************
typedef struct tagRES_BRD_ResList
{
	char		name[16];		// Resource name with Number
	U32			attr;			// Attributes of Resource (Look RESattr_XXX constants)
	S32			idxSuperRes;	// Index of SuperResource which forced this resource to be cauptured
	void		*ptrCaptor;		// NULL or Pointer to Captor To Capture/Release the Resource
} RES_BRD_ResList;

//=******************* RES_BRD_GetResList *****************
// To Exchange Data:
//    from TECH Driver to BRD Shell
//    from BASE Driver to BRD Shell
//    from BASE Driver to TECH Driver
//    from SUB Driver  to TECH Driver
//=********************************************************
typedef struct tagRES_BRD_ResListItem
{
	char	name[16];		// Resource name with(15byte) / without(12byte) Number
	U32		attr;			// Attributes of Resource (Look RESattr_XXX constants)
} RES_BRD_ResListItem;

typedef struct tagRES_BRD_GetResList
{
	U32					item;			// IN:  Items Number into pRLI Array
	U32					itemReal;		// OUT: Number of Resources
	RES_BRD_ResListItem	*pRLI;			// OUT: Array, that returns Resource Infos
} RES_BRD_GetResList;




//=********************************************************
//=********************************************************
//=********************************************************
//
// TECH Driver Support
//
//
//

//=**************** RES_TECH_MainSubunitList **************
// To Form Main Subunit List
//=********************************************************
typedef struct tagRES_TECH_MainSubunitList
{
	S32				type;			// Subunit Type ID
	HINSTANCE		hLib;			// Dll Handle 
	SUB_API_entry	*pEntry;		// SUB API Entry Point
	void			*pSub;			// SUB Driver Extension (InfoSM)
} RES_TECH_MainSubunitList;

//=******************** RES_TECH_MainResourceList *********
// To Form Main Resource List
//=********************************************************
typedef S32 RES_TECH_DevSupportEntry
		(
			void *pDev,		// TECH Driver Extension (InfoBM)
			void *pData,	// Internal Data
			S32 handle,		// Handle of this Resource 
			U32 cmd,		// Command Code (from BRD_ctrl())
			void *args 		// Command Arguments (from BRD_ctrl())
		);

typedef struct tagRES_TECH_EndPoint
{
	U32		attr;				// Attributes of EndPoint
	S32		index;				// Index of EndPoint
	S08		idxRequered[8];		// Index of Resourses that may be Used  for this EndPoint
//!!S08		idxCaptured[8];		// Index of Resourses that was captured for this EndPoint
	S08		*pIdxCaptured;		// pointer to shareable memory "idxCaptured[]"
	HANDLE	hFileMapping;		// Handle of shareable memory "idxCaptured[]"
} RES_TECH_EndPoint;

typedef struct tagRES_TECH_MainResourceList
{
	char			name[16];		// Resource name with Number
	U32				attr;			// Attributes of Resource
	S32				idxMain;		// Index of this Resource into TECH/DUPLEX Main Resource List
	S32				idxSuperRes;	// Index of Resource that has captured this Resource
	SUB_API_entry	*pBSubEntry;	// Resource Support Entry Point into BSUB Driver
	void			*pBSub;			// BSUB Driver Extension (InfoSM)
	SUB_API_entry	*pSubEntry;		// Resource Support Entry Point into SUB Driver
	void			*pSub;			// SUB Driver Extension (InfoSM)
	RES_TECH_DevSupportEntry *pDevEntry;// Resource Support Entry Point into TECH/DUPLEX Driver
	RES_TECH_EndPoint		endPoint[8];// Array of EndPoints
	HANDLE					hEPMutex;	// Mutex for Atomic access to "endPoint[]"
} RES_TECH_MainResourceList;

//=**************** RES_TECH_UnreadableRegs ***************
// To Support General Unreadable ADM Registers
//=********************************************************
typedef struct tagRES_TECH_UnreadableRegs 
{
	U32		*pRegs;				// Pointer to shareable memory
	HANDLE	hFileMapping;		// Handle of shareable memory 
} RES_TECH_UnreadableRegs;

//=**************** RES_TECH_SubunitSupport ***************
//=********************************************************
typedef struct tagRES_TECH_SubunitSupport 
{
	RES_TECH_MainSubunitList	*pMainSubunitList;	// Main Subunit Table
	RES_TECH_MainResourceList	*pMainResourceList;	// Main Resource Table
	RES_TECH_UnreadableRegs		sUnreadableRegs;	// Contents of General Unreadable ADM Registers
	HANDLE						hResMutex;			// Mutex for Atomic access to any Resources
} RES_TECH_SubunitSupport;




//=********************************************************
//=********************************************************
//=********************************************************
//
// BASE Driver Support
//
//
//

//=******************** RES_BASE_CmdList ******************
//=********************************************************
typedef S32 RES_BASE_CmdEntry
		( 
			void	*pDev,		// InfoBM
			S32		idxMain,	// Resource Index into BASE Resource List
			void	*pResData,	// Specific Resource Data
			U32		cmd,		// Command Code (from BRD_ctrl())
			void	*args 		// Command Arguments (from BRD_ctrl())
		);

typedef struct tagRES_BASE_CmdList
{
	U32			cmd;			// Command Code
	U32			isSpy:1,		// 0-Control Cmd, 1-Monitoring Cmd
				isCapture:1,	// 0-don't capture the resource, 1-capture the resource if need
				isRelease:1;	// 0-don't capture the resource, 1-capture the resource if need
	RES_BASE_CmdEntry	*pEntry;// Command Support Entry Point
} RES_BASE_CmdList;


//=******************** RES_BASE_ResourceList *************
//=********************************************************
typedef struct tagRES_BASE_ResourceList
{
	char			name[16];		// Resource name with Number
	U32				attr;			// Attributes of Resource (Look RESattr_XXX constants)
	S32				idxMain;		// Index of this Resource into BASE Resource List
	void			*pResData;		// Specific Resource Data
	RES_BASE_CmdEntry	*pCmd;		// Resource Command List
} RES_BASE_ResourceList;



//=********************************************************
//=********************************************************
//=********************************************************
//
// SUB Driver Support
//
//
//

//=**************** RES_SUB_UnreadableRegs ****************
// To Support Additional Unreadable ADM Registers
//=********************************************************
typedef struct tagRES_SUB_UnreadableRegs 
{
	U32		*pRegs;				// Pointer to shareable memory
	HANDLE	hFileMapping;		// Handle of shareable memory 
} RES_SUB_UnreadableRegs;


//=******************** RES_SUB_CmdList ******************
//=********************************************************
typedef S32 RES_SUB_CmdEntry( 
			void	*pSub,		// InfoSM
			S32		idxMain,	// Resource Index into Main Resource List
			void	*pResData,	// Specific Resource Data
			void	*pContext,	// Specific Data to send to TECH Driver
			U32		cmd,		// Command Code (from BRD_ctrl())
			void	*args 		// Command Arguments (from BRD_ctrl())
		);

typedef struct tagRES_SUB_CmdList
{
	U32			cmd;			// Command Code
	U32			isSpy:1,		// 0-Control Cmd, 1-Monitoring Cmd
				isCapture:1,	// 0-don't capture the resource, 1-capture the resource if need
				isRelease:1;	// 0-don't capture the resource, 1-capture the resource if need
	RES_SUB_CmdEntry	*pEntry;// Command Support Entry Point
} RES_SUB_CmdList;


//=******************** RES_SUB_ResourceList **************
//=********************************************************
typedef struct tagRES_SUB_ResourceList
{
	char			name[16];		// Resource name with Number
	U32				attr;			// Attributes of Resource (Look RESattr_XXX constants)
	S32				idxMain;		// Index of this Resource into TECH/DUPLEX Main Resource List
	void			*pResData;		// Specific Resource Data
	RES_SUB_CmdEntry	*pCmd;		// Resource Command List
} RES_SUB_ResourceList;

//=******************** RES_SUB_TechParams ****************
// Keeps TECH Driver Parameters
//=********************************************************
typedef struct tagRES_SUB_TechParams
{
	void			*pDev;			// InfoBM of TECH
	DEVS_API_entry	*pDevsEntry;	// Pointer to TECH DEVS_entry();
	char			baseName[32];	// BaseUnit Full Name
	HANDLE			hResMutex;		// Mutex for Atomic access to any Resources, keeps
									// the same value as "RES_TECH_SubunitSupport.hResMutex"
} RES_SUB_TechParams;


//=********************  *******************
//=********************************************************
typedef struct
{
} RES_;

#pragma pack(pop)

#endif	// __BRDRSRC_H_

//
// End of File
//

