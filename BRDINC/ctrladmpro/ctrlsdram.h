/*
 ****************** File CtrlSdram.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : SDRAM section
 *
 * (C) Instrumental Systems 
 *
 * Created: by Dorokhin A. Feb, 2005
 * 17.02.2005 - add BRDctrl_SDRAM_...DLG
 * 04.03.2005 - add BRDctrl_SDRAM_GETCFG
 *
 ************************************************************
*/

#ifndef _CTRL_SDRAM_H
 #define _CTRL_SDRAM_H

#include "ctrladmpro.h"

enum {
	BRDctrl_SDRAM_SETREADMODE		= BRDctrl_SDRAM + 0,	// Set SDRAM read mode (auto or non-auto) (U32)
	BRDctrl_SDRAM_GETREADMODE		= BRDctrl_SDRAM + 1,	// Get SDRAM read mode (auto or non-auto) (U32)
	BRDctrl_SDRAM_SETMEMSIZE		= BRDctrl_SDRAM + 2,	// Set memory size for acqusition data (32-bit words) (U32)
	BRDctrl_SDRAM_GETMEMSIZE		= BRDctrl_SDRAM + 3,	// Get memory size for acqusition data (32-bit words) (U32)
	BRDctrl_SDRAM_SETSTARTADDR		= BRDctrl_SDRAM + 4,	// Set start address for acqusition data (32-bit words) (U32)
	BRDctrl_SDRAM_GETSTARTADDR		= BRDctrl_SDRAM + 5,	// Get start address for acqusition data (32-bit words) (U32)
	BRDctrl_SDRAM_SETREADADDR		= BRDctrl_SDRAM + 6,	// Set start address for read data (32-bit words) (used by non-auto mode) (U32)
	BRDctrl_SDRAM_GETREADADDR		= BRDctrl_SDRAM + 7,	// Get start address for read data (32-bit words) (used by non-auto mode) (U32)
	BRDctrl_SDRAM_SETPOSTTRIGER		= BRDctrl_SDRAM + 8,	// Set post-trigger size (32-bit words) (used by pretrigger mode) (U32)
	BRDctrl_SDRAM_GETPOSTTRIGER		= BRDctrl_SDRAM + 9,	// Get post-trigger size (32-bit words) (used by pretrigger mode) (U32)
	BRDctrl_SDRAM_SETFIFOMODE		= BRDctrl_SDRAM + 10,	// Set FIFO mode (U32)
	BRDctrl_SDRAM_GETFIFOMODE		= BRDctrl_SDRAM + 11,	// Get FIFO mode (U32)
	BRDctrl_SDRAM_GETCFG			= BRDctrl_SDRAM + 13,	// Get SDRAM configuration (PBRD_SdramCfg)
	BRDctrl_SDRAM_SETSEL			= BRDctrl_SDRAM + 14,	// Set select source & destination for SDRAM (U32)
	BRDctrl_SDRAM_GETSEL			= BRDctrl_SDRAM + 15,	// Get select source & destination for SDRAM (U32)
	BRDctrl_SDRAM_GETCFGEX			= BRDctrl_SDRAM + 17,	// Get extension SDRAM configuration (PBRD_SdramCfgEx)
	BRDctrl_SDRAM_SETTESTSEQ		= BRDctrl_SDRAM + 18,	// Set SDRAM test sequence (U32)
	BRDctrl_SDRAM_GETTESTSEQ		= BRDctrl_SDRAM + 19,	// Get SDRAM test sequence (U32)

	//BRDctrl_SDRAM_ENDPAGESDLG		= BRDctrl_SDRAM + 124,	// destroy property pages for SDRAM (NULL)
	//BRDctrl_SDRAM_SETPROPDLG		= BRDctrl_SDRAM + 125,	// Display setting property dialog for SDRAM (ULONG)
	//BRDctrl_SDRAM_GETPROPDLG		= BRDctrl_SDRAM + 126,	// Display getting only property dialog for SDRAM (ULONG)
	BRDctrl_SDRAM_GETDLGPAGES		= BRDctrl_SDRAM + 127,	// get property pages of SDRAM for dialog (PBRD_PropertyList)

	BRDctrl_SDRAM_FIFORESET			= BRDctrl_SDRAM + 128,	// SDRAM FIFO reset (NULL)
	BRDctrl_SDRAM_ENABLE			= BRDctrl_SDRAM + 129,	// Enable/disable write to SDRAM (U32)
	BRDctrl_SDRAM_FIFOSTATUS		= BRDctrl_SDRAM + 130,	// Get SDRAM FIFO status (U32)
	BRDctrl_SDRAM_GETDATA			= BRDctrl_SDRAM + 131,	// Get data buffer from SDRAM (BRD_DataBuf)
	BRDctrl_SDRAM_READENABLE		= BRDctrl_SDRAM + 132,	// Enable/disable read to SDRAM (used by non-auto mode) (U32)
	BRDctrl_SDRAM_GETACQSIZE		= BRDctrl_SDRAM + 133,	// Get real acqusition data size (32-bit words) (U32)
	BRDctrl_SDRAM_ISACQCOMPLETE		= BRDctrl_SDRAM + 134,	// Get acqusition complete status (U32)
	BRDctrl_SDRAM_ISPASSMEMEND		= BRDctrl_SDRAM + 135,	// Get passage memory end (cycling) status by acqusition data (U32)
	BRDctrl_SDRAM_GETPRETRIGEVENT	= BRDctrl_SDRAM + 136,	// Get real start event by pretrigger mode (32-bit words) (U32)

	BRDctrl_SDRAM_GETSRCSTREAM		= BRDctrl_SDRAM + 138,	// Get source address for stream (U32)
	BRDctrl_SDRAM_IRQACQCOMPLETE	= BRDctrl_SDRAM + 139,	// Enable/disable acqusition complete interrupt (U32)
	BRDctrl_SDRAM_WAITACQCOMPLETE	= BRDctrl_SDRAM + 140,	// Wait of acqusition complete interrupt (U32)
	BRDctrl_SDRAM_WAITACQCOMPLETEEX	= BRDctrl_SDRAM + 141,	// Wait of acqusition complete interrupt (PBRD_WaitEvent)

	BRDctrl_SDRAM_FLAGCLR			= BRDctrl_SDRAM + 144,	// SDRAM FIFO status flag clear (NULL)
	BRDctrl_SDRAM_PUTDATA			= BRDctrl_SDRAM + 145,	// Put data buffer into SDRAM (BRD_DataBuf)

	BRDctrl_SDRAM_ILLEGAL
};

#pragma pack(push, 1) 

// getting configuration
typedef struct _BRD_SdramCfg
{
	U32		SlotCnt;		// количество установленных слотов
	U32		ModuleCnt;		// количество установленных DIMM-модулей (занятых слотов)
	U32		RowAddrBits;	// количество разрядов адреса строк
	U32		ColAddrBits;	// количество разрядов адреса столбцов
	U32		ModuleBanks;	// количество банков в DIMM-модуле
	U32		ChipBanks;		// количество банков в микросхемах DIMM-модуля
	U32		MemType;		// тип памяти: 01 - Fast Page Mode, 02 - EDO, 04 - SDRAM, 07 - DDR SDRAM, 08 - DDR2 SDRAM
	U32		Mode;			// флаги возможностей ввода из памяти в ПК (0x01)/ вывода в память из ПК (0x02)
} BRD_SdramCfg, *PBRD_SdramCfg;

// getting extension configuration
typedef struct _BRD_SdramCfgEx
{
	U32		Size;			// размер структуры
	U32		MemType;		// тип памяти: 01 - Fast Page Mode, 02 - EDO, 04 - SDRAM, 07 - DDR SDRAM, 08 - DDR2 SDRAM
	U32		Mode;			// флаги возможностей ввода из памяти в ПК (0x01)/ вывода в память из ПК (0x02)
	U32		ModuleCnt;		// количество установленных модулей (DIMM или SODIMM)
	U32		ModuleBanks;	// количество банков в модуле
	U32		RowAddrBits;	// число разрядов адреса строк
	U32		ColAddrBits;	// число разрядов адреса столбцов
	U32		ChipBanks;		// количество банков в микросхемах
	U32		PrimWidth;		// ширина шины (число разрядов) модуля
	U32		ChipWidth;		// ширина шины микросхемы (число разрядов в микросхемах) : 4,8,16,32
	U32		CapacityMbits;  // объём модуля в мегабитах : 256,512,1024,2048,4096,8192,16384
} BRD_SdramCfgEx, *PBRD_SdramCfgEx;

// SDRAM error codes
enum {
	BRDerr_SDRAM_NO_MEMORY		=	BRDctrl_SDRAM + 0,
	BRDerr_SDRAM_BAD_PARAMETER	=	BRDctrl_SDRAM + 1,
	BRDerr_SDRAM_NO_EQU_SPD		= 	BRDctrl_SDRAM + 2,
	BRDerr_SDRAM_UNDEFINED
};

// sources & destination
enum {
	BRDsd_ADCHOST	= 0x0000,	// ADC -> DDR -> HOST
	BRDsd_FDSPHOST	= 0x0001,	// FDSP -> DDR -> HOST
	BRDsd_HOSTHOST	= 0x0002,	// HOST -> DDR -> HOST
	BRDsd_ADCDAC	= 0x0100,	// ADC -> DDR -> DAC
	BRDsd_FDSPDAC	= 0x0101,	// FDSP -> DDR -> DAC
	BRDsd_HOSTDAC	= 0x0102,	// HOST -> DDR -> DAC
};

#pragma pack(pop)    

#endif // _CTRL_SDRAM_H

//
// End of file
//