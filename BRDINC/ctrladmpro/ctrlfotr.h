/*
 ****************** File CtrlFotr.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : FOTR section
 *
 * (C) Instrumental Systems 
 *
 * Created: by Dorokhin A. Jun, 2006
 *
 ************************************************************
*/

#ifndef _CTRL_FOTR_H
 #define _CTRL_FOTR_H

#include "ctrladmpro.h"

enum {
	BRDctrl_FOTR_SETMODE			= BRDctrl_FOTR + 0,		// Set FOTR mode (U32)
	BRDctrl_FOTR_GETMODE			= BRDctrl_FOTR + 1,		// Get FOTR mode (U32)
	BRDctrl_FOTR_GETCFG				= BRDctrl_FOTR + 2,		// Get FOTR configuration (PBRD_FotrCfg)
	BRDctrl_FOTR_MSGFIFORESET		= BRDctrl_FOTR + 3,		// FOTR message FIFO reset (U32)
	BRDctrl_FOTR_SETTESTMODE		= BRDctrl_FOTR + 4,		// Set FOTR test mode (U32) - only for ADMFOTR3G
	BRDctrl_FOTR_GETTESTMODE		= BRDctrl_FOTR + 5,		// Get FOTR test mode (U32) - only for ADMFOTR3G
										  
	BRDctrl_FOTR_SYNC				= BRDctrl_FOTR + 32,	// Syncronization with remote device (PBRD_Sync)
	BRDctrl_FOTR_DISCONNECT			= BRDctrl_FOTR + 33,	// Disconnect from remote device (NULL)
	BRDctrl_FOTR_SENDMSG			= BRDctrl_FOTR + 34,	// Send message for remote device (PBRD_MsgDataBuf)
	BRDctrl_FOTR_RECEIVEMSG			= BRDctrl_FOTR + 35,	// Receive message from remote device (PBRD_MsgDataBuf)
	BRDctrl_FOTR_WRITERMREG			= BRDctrl_FOTR + 36,	// Write register on remote device (PBRD_RemoteReg)
	BRDctrl_FOTR_READRMREG			= BRDctrl_FOTR + 37,	// Read register on remote device (PBRD_RemoteReg)
	BRDctrl_FOTR_WRITERMREGS		= BRDctrl_FOTR + 38,	// Write registers on remote device (PBRD_RemoteReg)
	BRDctrl_FOTR_READRMREGS			= BRDctrl_FOTR + 39,	// Read registers on remote device (PBRD_RemoteReg)
	BRDctrl_FOTR_SETFIFOADDR		= BRDctrl_FOTR + 40,	// Set address for DATA FIFO (U32)
	BRDctrl_FOTR_GETFIFOADDR		= BRDctrl_FOTR + 41,	// Get address for DATA FIFO (U32)
	BRDctrl_FOTR_CONNECT			= BRDctrl_FOTR + 42,	// Connect with remote device (NULL)
	BRDctrl_FOTR_CHECKCONNECT		= BRDctrl_FOTR + 43,	// Check connect with remote device (PBRD_Sync)
	BRDctrl_FOTR_RESET				= BRDctrl_FOTR + 44,	// Reset of tetrada (NULL)

//	BRDctrl_FOTR_GETDLGPAGES		= BRDctrl_FOTR + 127,	// get property pages of Test for dialog (PBRD_PropertyList)
										  
	BRDctrl_FOTR_INFIFORESET		= BRDctrl_FOTR + 128,	// FOTR DATA32_IN FIFO reset (NULL)
	BRDctrl_FOTR_INENABLE			= BRDctrl_FOTR + 129,	// Enable/disable write to FOTR DATA32_IN (U32)
	BRDctrl_FOTR_INFIFOSTATUS		= BRDctrl_FOTR + 130,	// Get FOTR DATA32_IN FIFO status (U32)
	BRDctrl_FOTR_GETDATA			= BRDctrl_FOTR + 131,	// Get data buffer (BRD_DataBuf)

	BRDctrl_FOTR_OUTFIFORESET		= BRDctrl_FOTR + 132,	// FOTR DATA32_OUT FIFO reset (NULL)
	BRDctrl_FOTR_OUTENABLE			= BRDctrl_FOTR + 133,	// Enable/disable write to FOTR DATA32_OUT (U32)
	BRDctrl_FOTR_OUTFIFOSTATUS		= BRDctrl_FOTR + 134,	// Get FOTR DATA32_OUT FIFO status (U32)
	BRDctrl_FOTR_PUTDATA			= BRDctrl_FOTR + 135,	// Set data buffer (BRD_DataBuf)
										  
	BRDctrl_FOTR_GETINSRCSTREAM		= BRDctrl_FOTR + 138,	// Get source address for input stream (U32)
	BRDctrl_FOTR_GETOUTSRCSTREAM	= BRDctrl_FOTR + 139,	// Get source address for output stream (U32)
	BRDctrl_FOTR_PREPARESTART		= BRDctrl_FOTR + 143,	// Called before first BRDctrl_FOTR_ENABLE (NULL)

	BRDctrl_FOTR_ILLEGAL
};

#pragma pack(push, 1) 

// getting configuration
typedef struct _BRD_FotrCfg
{
	U32		RefGen;						// значение опорного генератора в Гц
	U32		Chan;						// количество каналов
	U32		IoType;						// тип разъема ввода/вывода (приемопередатчика)
	U32		InFifoSize;					// размер входного FIFO данных FDI (в байтах)
	U32		OutFifoSize;				// размер выходного FIFO данных FDO (в байтах)
} BRD_FotrCfg, *PBRD_FotrCfg;

// for sync command
typedef struct _BRD_Sync 
{
	U32		mode;		// 0 - Slave, 1 - Master
	U32		timeout;	// timeout (msec) (0xFFFFFFFFL - INFINITE)
} BRD_Sync, *PBRD_Sync;

// for write & read remote registers
typedef struct _BRD_RemoteReg
{
	U32		node;		// определяется конфигурацией подключения (0x12 - для прямого подключения устройства к ADMFOTR)
	U32		bus;		// номер шины (0 - регистры ПЛИС, 1 - локальная шина и т.д.)
	U32		addr;		// адрес регистра на шине
	U32		val;		// значение (для одиночных операций)
	U32*	pVal;		// указатель на массив значений
	U32		size;		// размер массива (в 32-разрядных словах)
	U32		incFlag;	// 0 - фиксированный адрес, 1 - последовательные адреса (для операций с массивами)
	U32		timeout;	// timeout (msec) (0xFFFFFFFFL - INFINITE)
} BRD_RemoteReg, *PBRD_RemoteReg;

enum 
{
	BRDfotr_SLAVE	= 0,	// 
	BRDfotr_MASTER	= 1,	// 
};

enum 
{
	BRDfotr_SENDFIFO	= 0,	// 
	BRDfotr_RECEIVEFIFO	= 1,	// 
};

#pragma pack(pop)    

// FOTR error codes
enum {
	BRDerr_FOTR_ILLEGAL_CHANNEL				=	BRDctrl_FOTR + 0,
	BRDerr_FOTR_WAIT_REMOTE_TRANSMITTER_OFF	=	BRDctrl_FOTR + 1,
	BRDerr_FOTR_WAIT_REMOTE_TRANSMITTER_ON	=	BRDctrl_FOTR + 2,
	BRDerr_FOTR_WAIT_REMOTE_CLK_DONE		=	BRDctrl_FOTR + 3,
	BRDerr_FOTR_WAIT_REMOTE_CONNECT			=	BRDctrl_FOTR + 4,
	BRDerr_FOTR_WAIT_REMOTE_READY			=	BRDctrl_FOTR + 5,
	BRDerr_FOTR_WAIT_SEND_READY				=	BRDctrl_FOTR + 6,
	BRDerr_FOTR_WAIT_RECEIVE_READY			=	BRDctrl_FOTR + 7,
	BRDerr_FOTR_SIZE_SEND_TOOBIG			= 	BRDctrl_FOTR + 8,
	BRDerr_FOTR_SIZE_RECEIVE_TOOBIG			= 	BRDctrl_FOTR + 9,
	BRDerr_FOTR_WAIT_LOCAL_DCM_LOCKED		=	BRDctrl_FOTR + 10,
	BRDerr_FOTR_UNDEFINED
};

#endif // _CTRL_FOTR_H

//
// End of file
//