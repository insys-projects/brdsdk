
#ifndef _UTYPES_LINUX_H_
#define _UTYPES_LINUX_H_

#if defined (__LINUX__) || defined(__linux__) || defined (__QNX__)

#include <stdint.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>

typedef uint8_t   u8,   UINT08, U08, *PU08, BYTE, U8, UCHAR, *PUCHAR;
typedef uint16_t  u16,  UINT16, U16, *PU16, WORD, USHORT, *PUSHORT;
typedef uint32_t  u32,  UINT32, U32, *PU32, ULONG, *PULONG, *PUINT, UINT, DWORD;
typedef uint64_t  u64,  UINT64, U64, *PU64, ULONG64, *PULUNG64;
typedef int8_t    s8,   SINT08, S08, *PS08;
typedef int16_t   s16,  SINT16, S16, *PS16;
typedef int32_t   s32,  SINT32, S32, *PS32, LONG;
typedef int64_t   s64,  SINT64, S64, *PS64, __int64;
typedef float     REAL32, *PREAL32;
typedef double    REAL64, *PREAL64;

#if !defined(TRUE) && !defined(FALSE)
typedef enum { FALSE=0, TRUE=1 } BOOL;
#endif

typedef void*     HANDLE;
typedef void*     HINSTANCE;
typedef void*     PVOID;
typedef void      VOID;

typedef int 		SOCKET;
typedef char   		TCHAR;
typedef char*  		PTCHAR;
typedef char*  		LPTSTR;

// added for 64-bit windows driver compatibility
typedef char                    BRDCHAR;
#define _BRDC(x)                x
#define BRDC_strlen             strlen
#define BRDC_strcpy             strcpy
#define BRDC_strncpy            strncpy
#define BRDC_strcmp             strcmp
#define BRDC_stricmp            _stricmp
#define BRDC_strnicmp           _strnicmp
#define BRDC_strncmp            strncmp
#define BRDC_strcat             strcat
#define BRDC_strchr             strchr
#define BRDC_strstr             strstr
#define BRDC_strtol             strtol
#define BRDC_strtoul            strtoul
#define BRDC_strtod             strtod
#define BRDC_strtok		strtok
#define BRDC_atol               atol
#define BRDC_atoi               atoi
#define BRDC_atoi64             atoll
#define BRDC_strtoui64          strtoull
#define BRDC_atof               atof
#define BRDC_strlwr             IPC_strlwr
#define BRDC_itoa               IPC_itoa
#define BRDC_printf             printf
#define BRDC_fprintf            fprintf
#define BRDC_fscanf             fscanf
#define BRDC_sprintf            sprintf
#define BRDC_vsprintf           vsprintf
#define BRDC_sscanf             sscanf
#define BRDC_fopen              fopen
#define BRDC_freopen            freopen
#define BRDC_fdopen             fdopen
#define BRDC_sopen              sopen
#define BRDC_fgets              fgets
#define BRDC_fgetc              fgetc
#define BRDC_getenv             getenv
#define BRDC_main               main
#define BRDC_toupper            toupper
#define BRDC_tolower            tolower
#define BRDC_fputs              fputs
#define BRDC_isspace            isspace
#define BRDC_isdigit            isdigit
#define BRDC_isalnum            isalnum
#define BRDC_isxdigit           isxdigit
#define BRDC_cputs              puts
#define BRDC_cscanf             scanf
#define BRDC_fputchar           putchar
#define BRDC_fgetchar           getchar
#define BRDC_flushall           IPC_flushall
#define BRDC_putch              putchar
#define BRDC_cgets              gets
#define BRDC_puts               puts
#define BRDC_gets               gets
#define BRDC_tmpnam             tmpnam
#define BRDC_perror             perror
#define BRDC_getcwd             getcwd
#define BRDC_chdir              chdir
#define BRDC_rmdir              rmdir
#define BRDC_access             access
#define BRDC_chmod              chmod
#define BRDC_creat              creat
#define BRDC_mktemp             mktemp
#define BRDC_remove             remove
#define BRDC_rename             rename
#define BRDC_asctime            asctime
#define BRDC_ctime              ctime
#define	BRDC_strftime           strftime
//-------------------------------------

#define lstrcpy strcpy
#define lstrcpyA strcpy
#define lstrcat strcat
#define lstrcatA strcat
#define lstrlen strlen
#define lstrlenA strlen
#define lstrcmpi strcasecmp
#define _tcsstr strstr
#define _tcscpy_s strcpy
#define _tcscpy strcpy
#define _tcschr strchr
#define sprintf_s sprintf
#define _tcscat_s strcat
#define _tcslen strlen
#define _tcscpy strcpy

#define _T(x)       x
#define _TEXT(x)    x
#define INFINITE    (-1)

#define _stricmp strcmp
#define stricmp strcmp
#define _strnicmp strncmp

typedef const char*	LPCTSTR;
typedef char*		PCTSTR;
typedef char*		PTSTR;
typedef void*		LPVOID;
typedef void*       LPOVERLAPPED;

#ifndef MAX_PATH
#define MAX_PATH PATH_MAX
#endif

#define BRD_ALIGN_UP(value, align)  (((value) & (align-1)) ? \
                                    (((value) + (align-1)) & ~(align-1)) : \
                                    (value))

#define BRD_ALIGN_DOWN(value, align) ((value) & ~(align-1))

#endif /* __linux__ */

#endif /* _UTYPES_LINUX_H_ */

/*
*  End of File
*/
