/***************************************************
*
* EXTN_ANDOR.H
*
* BRD_extension() cmd codes and data types
*
* (C) InSys Corp. by Dorokhin Andrey Dec, 2004
*
****************************************************/


#ifndef	__EXTN_ANDOR_H_
#define	__EXTN_ANDOR_H_

#include	"ctrladmpro.h"

// BRD_extension() cmd codes
enum
{
	BRDextn_BASE_ANDOR	= 0x4800,	// Base addres of code range
	BRDextn_GET_PLDFILEINFO_ANDOR, 	// Get Pld file info ()
	BRDextn_SHOW_DIALOG_ANDOR,		// Show Property Dialog ()
	BRDextn_DEL_DIALOG_ANDOR		// Delete Modeless Property Dialog ()
};

// BRD_extension() data types

typedef struct _BRDextn_PLDFILEINFO
{
	U32	pldId;			// Programmable Unit ID
	U32	version;		// Pld version / версия
	U32	modification;	// Pld modification / модификация
	U32	build;			// Pld build / сборка
	U32	baseId;
	U32	baseVer;
	U32	submodId;
	U32	submodVer;
} BRDextn_PLDFILEINFO, *PBRDextn_PLDFILEINFO;

// for property page functions
typedef struct _BRDextn_PropertyDlg
{
	HINSTANCE			hLib;		// дескриптор библиотеки
	BOOL*				pChangeFlag;// указатель на флаг изменения пераметров
	U32					DlgMode;	// 0 bit = 1 - modal dialog, 0 bit = 0 - modalless dialog, 1 bit = 1 - not edit controls,  1 bit = 0 - edit controls, 
	U32					ListCnt;	// число структур со списком страниц для каждой службы
	PBRD_PropertyList	pList;		// указатель на списки всех страниц
	U08*				pCurPage;	// указатель на номер текущей страницы свойств (добавил 24.04.2006)
} BRDextn_PropertyDlg, *PBRDextn_PropertyDlg;

#endif	// __EXTN_ANDOR_H_ 

//
//  End of File
//
