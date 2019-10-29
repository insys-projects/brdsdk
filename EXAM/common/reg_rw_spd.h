// ***************** File reg_rw_spd.cpp ************

#include "gipcy.h"
#include "brd.h"
#include "ctrlreg.h"

S32 RegRwSpd(BRD_Handle hDev, BRDCHAR* fname );

BRD_Handle	RegRw_Capture( BRD_Handle hDev );
S32	RegRw_Release(  BRD_Handle hReg );
S32	RegRw_ReadDir(  BRD_Handle hReg, S32 trd, S32 reg, U32 *pVal );
S32	RegRw_WriteDir( BRD_Handle hReg, S32 trd, S32 reg, U32 val );
S32	RegRw_ReadInd(  BRD_Handle hReg, S32 trd, S32 reg, U32 *pVal );
S32	RegRw_WriteInd( BRD_Handle hReg, S32 trd, S32 reg, U32 val );
S32	RegRw_ReadSpd(  BRD_Handle hReg, U32 dev, U32 is32bits, U32 num, U32 synchr,
				  S32 trd, S32 reg, U32 *pVal );
S32	RegRw_WriteSpd( BRD_Handle hReg, U32 dev, U32 is32bits, U32 num, U32 synchr,
				  S32 trd, S32 reg, U32 val );

//
// End of File
//

