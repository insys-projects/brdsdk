/***************************************************
*
* UTYPES.H - define usefull types.
*
* (C) Instrumental Systems Corp. Ekkore, Dec. 1997-2001
*
****************************************************/


#ifndef _UTYPES_H_
#define _UTYPES_H_

#include <stdio.h>

#if defined (__LINUX__) || defined(__linux__) || defined (__QNX__)
#include "utypes_linux.h"
#endif

#define BRD_max(a,b) ((a) > (b) ? (a) : (b))
#define BRD_min(a,b) ((a) < (b) ? (a) : (b))

/*************************************
*
*  Types for MSDOS
*/


#ifdef __MSDOS__

typedef	unsigned char	UINT08;
typedef	signed   char	SINT08;
typedef	unsigned short	UINT16;
typedef	signed   short	SINT16;
typedef	unsigned long	UINT32;
typedef	signed   long	SINT32;

typedef	unsigned char	U08, *PU08;
typedef	signed   char	S08, *PS08;
typedef	unsigned short	U16, *PU16;
typedef	signed   short	S16, *PS16;
typedef	unsigned long	U32, *PU32;
typedef	signed   long	S32, *PS32;

typedef float			REAL32, *PREAL32;
typedef double			REAL64, *PREAL64;

//typedef enum{ FALSE=0, TRUE=1}        BOOL;
typedef unsigned char   UCHAR;
typedef unsigned short  USHORT;
typedef unsigned long   ULONG;
typedef unsigned long   HANDLE;

#endif /* __MSDOS__ */


/*************************************
*
*  Types for Windows
*/


#ifdef _WIN32

typedef	unsigned char	UINT08;
typedef	signed   char	SINT08;
typedef	unsigned short	UINT16;
typedef	signed   short	SINT16;
typedef	unsigned int	UINT32;
typedef	signed   int	SINT32;

typedef	unsigned char	U08, *PU08;
typedef	signed   char	S08, *PS08;
typedef	unsigned short	U16, *PU16;
typedef	signed   short	S16, *PS16;
typedef	unsigned int	U32, *PU32;
typedef	signed   int	S32, *PS32;

#if _MSC_VER == 1200
#else
	typedef	unsigned long long int	U64, *PU64;
	typedef	signed   long long int	S64, *PS64;
#endif

typedef float			REAL32, *PREAL32;
typedef double			REAL64, *PREAL64;

#ifdef _WIN64
typedef wchar_t			BRDCHAR;
#define _BRDC(x)		L ## x
#define BRDC_strlen		wcslen
#define BRDC_strcpy		wcscpy
#define BRDC_strncpy	wcsncpy
#define BRDC_strcmp		wcscmp
#define BRDC_stricmp	_wcsicmp
#define BRDC_strnicmp	wcsnicmp
#define BRDC_strncmp	wcsncmp
#define BRDC_strcat		wcscat
#define BRDC_strchr		wcschr
#define BRDC_strstr		wcsstr
#define BRDC_strtol		wcstol
#define BRDC_strtoul	wcstoul
#define BRDC_strtod		wcstod
#define BRDC_strtok		wcstok
#define BRDC_atol		_wtol
#define BRDC_atoi		_wtoi
#define BRDC_atoi64		_wtoi64
#define BRDC_strtoui64	_wcstoui64
#define BRDC_atof		_wtof
#define BRDC_strlwr		_wcslwr
#define BRDC_itoa		_itow
#define BRDC_printf		wprintf
#define BRDC_fprintf	fwprintf
#define BRDC_fscanf		fwscanf
#define BRDC_sprintf	swprintf
#define BRDC_sprintf_s	swprintf_s
#define BRDC_vsprintf	vswprintf
#define BRDC_sscanf		swscanf
#define BRDC_fopen		_wfopen
#define BRDC_fclose		fclose
#define BRDC_freopen	_wfreopen
#define BRDC_fdopen		_wfdopen
#define BRDC_sopen		_wsopen
#define BRDC_open		_wopen
#define BRDC_fgets		fgetws
#define BRDC_fputs		fputws
#define BRDC_fgetc		fgetwc
#define BRDC_getenv		_wgetenv
#define BRDC_main		wmain
#define BRDC_toupper	towupper
#define BRDC_tolower	towlower
#define BRDC_isspace	iswspace
#define BRDC_isdigit	iswdigit
#define BRDC_isalnum	iswalnum
#define BRDC_isxdigit	iswxdigit
#define BRDC_cputs      _cputws
#define BRDC_cscanf     _cwscanf
#define BRDC_fputchar   _fputwchar
#define BRDC_fgetchar   _fgetwchar
#define BRDC_flushall   flushall
#define BRDC_putch      _putwch
#if _MSC_VER >= 1900 
#define BRDC_cgets(str)      _getws_s( str, 65536 )		
#else
#define BRDC_cgets      _cgetws
#endif
#define BRDC_puts		_putws
#define BRDC_gets(str)		_getws_s( str, 65536 )		
#define BRDC_tmpnam		_wtmpnam
#define BRDC_perror		_wperror
#define BRDC_getcwd		_wgetcwd
#define BRDC_getdcwd	_wgetdcwd
#define BRDC_mkdir		_wmkdir
#define BRDC_chdir		_wchdir
#define BRDC_rmdir		_wrmdir
#define BRDC_access		_waccess
#define BRDC_chmod		_wchmod
#define BRDC_creat		_wcreat
#define BRDC_mktemp		_wmktemp
#define BRDC_remove		_wremove
#define BRDC_rename		_wrename
#define BRDC_findfirst	_wfindfirst
#define BRDC_findnext	_wfindnext
#define BRDC_asctime	_wasctime
#define BRDC_ctime		_wctime
#define	BRDC_strftime	wcsftime
#define BRDC_stat		_wstat
#define BRDC_mbstobcs(dst, src, maxcnt) mbstowcs( dst, src, maxcnt )
#define BRDC_bcstombs(dst, src, maxcnt) wcstombs( dst, src, maxcnt )
#define BRDC_wcstobcs(dst, src, maxcnt) strcpy( dst, src )
#define BRDC_bcstowcs(dst, src, maxcnt) strcpy( dst, src )
#else
typedef char			BRDCHAR;
#define _BRDC(x)		x
#define BRDC_strlen		strlen
#define BRDC_strcpy		strcpy
#define BRDC_strncpy	strncpy
#define BRDC_strcmp		strcmp
#define BRDC_stricmp	_stricmp
#define BRDC_strnicmp	_strnicmp
#define BRDC_strncmp	strncmp
#define BRDC_strcat		strcat
#define BRDC_strchr		strchr
#define BRDC_strstr		strstr
#define BRDC_strtol		strtol
#define BRDC_strtoul	strtoul
#define BRDC_strtod		strtod
#define BRDC_strtok		strtok
#define BRDC_atol		atol
#define BRDC_atoi		atoi
#define BRDC_atoi64		_atoi64
#define BRDC_strtoui64	_strtoui64
#define BRDC_atof		atof
#define BRDC_strlwr		_strlwr
#define BRDC_itoa		_itoa
#define BRDC_printf		printf
#define BRDC_fprintf	fprintf
#define BRDC_fscanf		fscanf
#define BRDC_sprintf	sprintf
#define BRDC_vsprintf	vsprintf
#define BRDC_sscanf		sscanf
#define BRDC_fopen		fopen
#define BRDC_fclose		fclose
#define BRDC_freopen	freopen
#define BRDC_fdopen		fdopen
#define BRDC_sopen		sopen
#define BRDC_open		_open
#define BRDC_filelength _filelength
#define BRDC_fgets		fgets
#define BRDC_fputs		fputs
#define BRDC_fgetc		fgetc
#define BRDC_getenv		getenv
#define BRDC_main		main
#define BRDC_toupper	toupper
#define BRDC_tolower	tolower
#define BRDC_isspace	isspace
#define BRDC_isdigit	isdigit
#define BRDC_isalnum	isalnum
#define BRDC_isxdigit	isxdigit
#define BRDC_cputs      cputs
#define BRDC_cscanf     cscanf
#define BRDC_fputchar   fputchar
#define BRDC_fgetchar   fgetchar
#define BRDC_flushall   flushall
#define BRDC_putch      putch
#if _MSC_VER >= 1900 
#define BRDC_cgets( str )      gets_s( str, 65536 )		
#else
#define BRDC_cgets      cgets
#endif
#define BRDC_puts		puts
#define BRDC_gets(str)		gets_s( str, 65536 )
#define BRDC_tmpnam		tmpnam
#define BRDC_perror		perror
#define BRDC_getcwd		getcwd
#define BRDC_getdcwd	_getdcwd
#define BRDC_mkdir		mkdir
#define BRDC_chdir		chdir
#define BRDC_rmdir		rmdir
#define BRDC_access		access
#define BRDC_chmod		chmod
#define BRDC_creat		creat
#define BRDC_mktemp		mktemp
#define BRDC_remove		remove
#define BRDC_rename		rename
#define BRDC_findfirst	_findfirst
#define BRDC_findnext	_findnext
#define BRDC_asctime	asctime
#define BRDC_ctime		ctime
#define	BRDC_strftime	strftime
#define BRDC_stat		stat
#define BRDC_mbstobcs(dst, src, maxcnt) strcpy( dst, src )
#define BRDC_bcstombs(dst, src, maxcnt) strcpy( dst, src )
#define BRDC_wcstobcs(dst, src, maxcnt) mbstowcs( dst, src, maxcnt )
#define BRDC_bcstowcs(dst, src, maxcnt) mbstowcs( dst, src, maxcnt )
#endif

#endif /* _WIN32 */


/*************************************
*
*  Types for TMS320C3x/C4x
*/


#if defined(_TMS320C30) || defined(_TMS320C40 )

/*
typedef	unsigned char	UINT08;
typedef	signed   char	SINT08;
typedef	unsigned short	UINT16;
typedef	signed   short	SINT16;
*/
typedef	unsigned int	UINT32;
typedef	signed   int	SINT32;

typedef	unsigned int	U32, *PU32;
typedef	signed   int	S32, *PS32;

typedef float			REAL32, *PREAL32;
typedef double			REAL64, *PREAL64;

typedef UINT32	ULONG;
typedef UINT32	USHORT;
typedef UINT32	UCHAR;

#endif /* _TMS320C30 || _TMS320C40 */


/*************************************
*
*  Types for TMS320C6x
*/


#ifdef _TMS320C6X

typedef	unsigned char	UINT08;
typedef	signed   char	SINT08;
typedef	unsigned short	UINT16;
typedef	signed   short	SINT16;
typedef	unsigned int	UINT32;
typedef	signed   int	SINT32;

typedef	unsigned char	U08, *PU08;
typedef	signed   char	S08, *PS08;
typedef	unsigned short	U16, *PU16;
typedef	signed   short	S16, *PS16;
typedef	unsigned int	U32, *PU32;
typedef	signed   int	S32, *PS32;

typedef	unsigned long long int	U64, *PU64;
typedef	signed   long long int	S64, *PS64;

typedef float			REAL32, *PREAL32;
typedef double			REAL64, *PREAL64;

typedef UINT32	ULONG;
typedef UINT16	USHORT;
typedef UINT08	UCHAR;

#endif /* _TMS320C6X */


/*************************************
*
*  Types for ADSP2106x
*/


#if defined(__ADSP21060__) || defined(__ADSP21061__) || defined(__ADSP21062__)|| defined(__ADSP21065L__)

typedef	unsigned int	UINT32;
typedef	signed   int	SINT32;

typedef	unsigned int	U32, *PU32;
typedef	signed   int	S32, *PS32;

typedef float	 REAL32, *PREAL32;
typedef double	 REAL64, *PREAL64;

typedef UINT32	ULONG;
typedef UINT32	USHORT;
typedef UINT32	UCHAR;

#endif /* __ADSP2106x__ */

/*************************************
*
*  Types for ADSP2116x
*/


#if defined(__ADSP21160__) || defined(__ADSP21161__)

typedef	unsigned int	UINT32;
typedef	signed   int	SINT32;

typedef	unsigned int	U32, *PU32;
typedef	signed   int	S32, *PS32;

typedef float	 REAL32, *PREAL32;
typedef double	 REAL64, *PREAL64;

typedef UINT32	ULONG;
typedef UINT32	USHORT;
typedef UINT32	UCHAR;

#endif /* __ADSP2116x__ */

/*************************************
*
*  Types for ADSP-TS101
*/


#if defined(__ADSPTS__)

typedef	unsigned int	UINT32;
typedef	signed   int	SINT32;

typedef	unsigned int	U32, *PU32;
typedef	signed   int	S32, *PS32;

typedef	unsigned long long int	U64, *PU64;
typedef	signed   long long int	S64, *PS64;

typedef float	 REAL32, *PREAL32;
typedef long double	 REAL64, *PREAL64;

typedef UINT32	ULONG;
typedef UINT32	USHORT;
typedef UINT32	UCHAR;

#endif /* __ADSPTS__ */

/*************************************
*
*  Types for MC24
*/

#if defined(__GNUC__) && !defined(__linux__)

typedef unsigned char   UINT08;
typedef signed   char   SINT08;
typedef unsigned short  UINT16;
typedef signed   short  SINT16;
typedef unsigned long   UINT32;
typedef signed   long   SINT32;

typedef unsigned char   U08, *PU08;
typedef signed   char   S08, *PS08;
typedef unsigned short  U16, *PU16;
typedef signed   short  S16, *PS16;
typedef unsigned long   U32, *PU32;
typedef signed   long   S32, *PS32;

typedef	unsigned long long int	U64, *PU64;
typedef	signed   long long int	S64, *PS64;

typedef float           REAL32, *PREAL32;
typedef double          REAL64, *PREAL64;

//typedef enum{ FALSE=0, TRUE=1}        BOOL;
typedef unsigned char   UCHAR;
typedef unsigned short  USHORT;
typedef unsigned long   ULONG;
typedef unsigned long   HANDLE;

#endif /* __GNUC__ */

/*************************************
*
*  Type Aliasing
*/

typedef UINT32	Uns;

/*************************************************
*
* Entry Point types
*
*/
#if !defined(WIN32) && !defined(__WIN32__)
//#ifndef WIN32
	#define FENTRY
	#define STDCALL
#else
	#include <windows.h>
	#define DllImport	__declspec( dllimport )
	#define DllExport	__declspec( dllexport )
	#define FENTRY		DllExport
	#define STDCALL		__stdcall
	#define	huge
#endif  // WIN32


#endif /* _UTYPES_H_ */

/*
*  End of File
*/



