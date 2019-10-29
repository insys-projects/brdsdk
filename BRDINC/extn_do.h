/***************************************************
*
* EXTN_DO.H
*
* BRD_extension() cmd codes and data types
*
* (C) Instr.Sys. by Dorokhin Juny, 2007
*
****************************************************/


#ifndef	__EXTN_DO_H_
#define	__EXTN_DO_H_


#include	"utypes.h"

//
//=== BRD_extension() cmd codes
//

enum
{
	BRDextn_BASE_DO	= 0x6800,		// Base addres of code range
	BRDextn_RESET_FIFO_DO			// Reset HOST FIFO
};



#endif	// __EXTN_DO_H_ 

//
//  End of File
//


