/*
 ****************** File ctrlemac.h *************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl : EMAC section
 *
 * (C) InSys by Dorokhin Andrey Jan 2016
 *
 *
 ************************************************************
*/

#ifndef _CTRL_EMAC_H
 #define _CTRL_EMAC_H

#include "ctrladmpro.h"

enum {
	BRDctrl_EMAC_WRITEREG			= BRDctrl_NET + 0,		// write EMAC register (PBRD_RegVal)
	BRDctrl_EMAC_READREG			= BRDctrl_NET + 1,		// read EMAC register (PBRD_RegVal)
	BRDctrl_EMAC_WRITEMDIO			= BRDctrl_NET + 2,		// write MDIO register (PBRD_RegVal)
	BRDctrl_EMAC_READMDIO			= BRDctrl_NET + 3,		// read MDIO register (PBRD_RegVal)

	BRDctrl_EMAC_TRANSMIT			= BRDctrl_NET + 8,		// transmit data package (PBRD_DataBuf)
	BRDctrl_EMAC_RECEIVE			= BRDctrl_NET + 9,		// receive data package (PBRD_DataBuf)
	BRDctrl_EMAC_ISANEGCOMPLETE		= BRDctrl_NET + 10,		// check auto negotiation complete (U32)

	BRDctrl_EMAC_ILLEGAL
};

#pragma pack(push, 1)    

typedef struct _BRD_RegVal {
	U32 adr;				// address 
	U32 val;				// value
} BRD_RegVal, *PBRD_RegVal;

#pragma pack(pop)    

#endif // _CTRL_EMAC_H

//
// End of file
//
