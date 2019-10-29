// ***************** File reg_rw_spd.cpp ************

#include "reg_rw_spd.h"

/////////////////////////////////////////////////////////////////////
// Запись и чтение регистров, указанных в файле
S32 RegRwSpd(BRD_Handle hDev, BRDCHAR* fname )
{
	U32 mode = BRDcapt_SHARED;
	BRD_Handle hReg = BRD_capture(hDev, 0, &mode, _BRDC("REG0"), 10000);
	if(hReg <= 0)
	{
		BRDC_printf(_BRDC("REG NOT capture (%X)\n"), hReg);
		return -1;
	}
	if(mode == BRDcapt_SHARED)
		BRDC_printf(_BRDC("REG Capture mode: SHARED (%X)\n"), hReg);
	else
		return -1;

	BRDC_printf(_BRDC("Programming SPD-registers from file %s\n"), fname);

	FILE *fh = BRDC_fopen( fname, _BRDC("rt") );
	if(fh == NULL)
	{
		BRDC_printf( _BRDC("Error by file open %s\n"), fname );
		return -1;
	}
	BRDCHAR fpath[MAX_PATH];
	IPC_getCurrentDir(fpath, MAX_PATH);
	BRDC_strcat(fpath, _BRDC("\\") );
	BRDC_strcat(fpath, fname);

	BRDCHAR str[256];
	BRDCHAR buf[256];
	BRDCHAR* begptr;
	BRDCHAR* endptr;

	U32 trd = 0xFFFF, dev, fl_32bits, num, synchr, reg, val;
	BRDCHAR oper[8];
	int ret;
	int fl_enum = 0;

	do
	{
		if( BRDC_fgets( str, 250, fh )==NULL )
			break;

		if( str[0] == _BRDC(';') )
			continue;

		if( str[0] == _BRDC('[') )
		{
			ret = BRDC_sscanf( str, _BRDC("[TRD:%i_DEV:%i]"), &trd, &dev);
			if(1 == ret)
			{
				BRDC_printf( _BRDC("TRD=%i IND\\DIR\n"), trd );
				continue;
			}
			else if(2 != ret)
			{
				BRDC_printf( _BRDC("Error by section %s\n"), str);
				continue;
			}
			begptr = str+1;
			int len = (int)BRDC_strlen(str);
			str[len-2] = 0;
			IPC_getPrivateProfileString(begptr, _BRDC("32BITS"), _BRDC("1"), buf, sizeof(buf), fpath);
			fl_32bits = BRDC_strtol(buf, &endptr, 0);
			IPC_getPrivateProfileString(begptr, _BRDC("NUM"), _BRDC("0"), buf, sizeof(buf), fpath);
			num = BRDC_strtol(buf, &endptr, 0);
			IPC_getPrivateProfileString(begptr, _BRDC("SYNCHR"), _BRDC("0"), buf, sizeof(buf), fpath);
			synchr = BRDC_strtol(buf, &endptr, 0);
			BRDC_printf( _BRDC("TRD=%d DEV=%d NUM=0x%X SYNCHR=%d (32BITS=%d)\n"), trd, dev, num, synchr, fl_32bits);
			fl_enum = 0;
			continue;
		}
		if(BRDC_strstr(str, _BRDC("#enum") ) && trd != 0xFFFF)
		{
			fl_enum = 1;
			continue;
		}
		if(BRDC_strstr(str, _BRDC("#end") ))
		{
			trd = 0xFFFF;
			fl_enum = 0;
			continue;
		}
		if(fl_enum == 0)
			continue;
		ret = BRDC_sscanf( str, _BRDC("%s %i %i"), oper, &reg, &val );
		if(3 == ret)
		{
			S32 status;
			BRD_Reg regdata;
			regdata.tetr = trd;
			
			if( BRDC_stricmp( oper, _BRDC("sleep") ) == 0  )
			{
				BRDC_printf( _BRDC("SLEEP [%d] (if -1, press any key)\n"), reg );
				if( (S32)reg == -1 )
					IPC_getch( );
				else
					IPC_delay( reg );
				continue;
			}
			else if( BRDC_stricmp( oper, _BRDC("wi") ) == 0  )
			{
				regdata.reg = reg;
				regdata.val = val;
				BRDC_printf( _BRDC("WRITE IND REG[0x%02X]=0x%08X \n"), reg, val );
				status = BRD_ctrl(hReg, 0, BRDctrl_REG_WRITEIND, &regdata);
				continue;
			}
			else if(  BRDC_stricmp( oper,_BRDC( "ri") ) == 0  )
			{
				regdata.reg = reg;
				status = BRD_ctrl(hReg, 0, BRDctrl_REG_READIND, &regdata);
				val = regdata.val;
				BRDC_printf(_BRDC("READ IND REG[0x%02X]=0x%04X \n"), reg, val);
				continue;
			}			
			else if( BRDC_stricmp( oper, _BRDC("wd") ) == 0  )
			{
				regdata.reg = reg;
				regdata.val = val;
				BRDC_printf( _BRDC("WRITE DIR REG[0x%02X]=0x%08X \n"), reg, val );
				status = BRD_ctrl(hReg, 0, BRDctrl_REG_WRITEDIR, &regdata);
				continue;
			}
			else if(  BRDC_stricmp( oper, _BRDC("rd") ) == 0  )
			{
				regdata.reg = reg;
				status = BRD_ctrl(hReg, 0, BRDctrl_REG_READDIR, &regdata);
				val = regdata.val;
				BRDC_printf(_BRDC("READ DIR REG[0x%02X]=0x%04X \n"), reg, val);
				continue;
			}

			if( str[0] == _BRDC('w') || str[0] == _BRDC('W') )
				if(fl_32bits)
					BRDC_printf( _BRDC("WRITE REG[0x%02X]=0x%08X \n"), reg, val );
				else
					BRDC_printf( _BRDC("WRITE REG[0x%02X]=0x%04X \n"), reg, val );

				U32 cmd = BRDctrl_REG_READSPD;
				
				if( str[0] == _BRDC('w') || str[0] == _BRDC('W') )
					cmd = BRDctrl_REG_WRITESPD;

				BRD_Spd ctrl;
				ctrl.dev = dev;
				ctrl.mode = fl_32bits;
				ctrl.num = num;
				ctrl.sync = synchr;
				ctrl.tetr = trd;
				ctrl.reg = reg;
				ctrl.val = val;
				status = BRD_ctrl(hReg, 0, cmd, &ctrl);
				
				if( str[0] == 'r' || str[0] == 'R')
				{
					val = ctrl.val;
					BRDC_printf(_BRDC("READ  REG[0x%02X]=0x%04X \n"), reg, val);
				}
				continue;
			}

	}while(1);

	fclose(fh);

	return BRD_release(hReg, 0);
}

/////////////////////////////////////////////////////////////////////
// Захват дескриптора службы REGS0
BRD_Handle	RegRw_Capture( BRD_Handle hDev )
{
	U32 mode = BRDcapt_SHARED;
	BRD_Handle hReg = BRD_capture( hDev, 0, &mode, _BRDC("REG0"), 10000);

	return hReg;
}

/////////////////////////////////////////////////////////////////////
// Освобождение дескриптора службы REGS0
S32	RegRw_Release( BRD_Handle hReg )
{
	return BRD_release( hReg, 0 );
}

/////////////////////////////////////////////////////////////////////
// Чтение прямоадресуемого регистра заданой терады
S32	RegRw_ReadDir( BRD_Handle hReg, S32 trd, S32 reg, U32 *pVal )
{
	BRD_Reg regdata;
	S32		err;

	regdata.tetr = trd;
	regdata.reg = reg;

	err = BRD_ctrl( hReg, 0, BRDctrl_REG_READDIR, &regdata );
	*pVal = regdata.val;

	return err;
}

/////////////////////////////////////////////////////////////////////
// Запись прямоадресуемого регистра заданой терады
S32	RegRw_WriteDir( BRD_Handle hReg, S32 trd, S32 reg, U32 val )
{
	BRD_Reg regdata;
	S32		err;

	regdata.tetr = trd;
	regdata.reg = reg;
	regdata.val = val;

	err = BRD_ctrl( hReg, 0, BRDctrl_REG_WRITEDIR, &regdata );

	return err;
}

/////////////////////////////////////////////////////////////////////
// Чтение косвенноадресуемого регистра заданой терады
S32	RegRw_ReadInd( BRD_Handle hReg, S32 trd, S32 reg, U32 *pVal )
{
	BRD_Reg regdata;
	S32		err;

	regdata.tetr = trd;
	regdata.reg = reg;

	err = BRD_ctrl( hReg, 0, BRDctrl_REG_READIND, &regdata );
	*pVal = regdata.val;

	return err;
}

/////////////////////////////////////////////////////////////////////
// Запись косвенноадресуемого регистра заданой терады
S32	RegRw_WriteInd( BRD_Handle hReg, S32 trd, S32 reg, U32 val )
{
	BRD_Reg regdata;
	S32		err;

	regdata.tetr = trd;
	regdata.reg = reg;
	regdata.val = val;

	err = BRD_ctrl( hReg, 0, BRDctrl_REG_WRITEIND, &regdata );

	return err;
}

/////////////////////////////////////////////////////////////////////
// Чтение SPD регистра заданой терады
S32	RegRw_ReadSpd( BRD_Handle hReg, U32 dev, U32 is32bits, U32 num, U32 synchr,
				  S32 trd, S32 reg, U32 *pVal )
{
	BRD_Spd ctrl;
	S32		err;

	ctrl.dev = dev;
	ctrl.mode = is32bits;
	ctrl.num = num;
	ctrl.sync = synchr;
	ctrl.tetr = trd;
	ctrl.reg = reg;

	err = BRD_ctrl(hReg, 0, BRDctrl_REG_READSPD, &ctrl);
	*pVal = ctrl.val;

	return err;
}

/////////////////////////////////////////////////////////////////////
// Запись SPD регистра заданой терады
S32	RegRw_WriteSpd( BRD_Handle hReg, U32 dev, U32 is32bits, U32 num, U32 synchr,
				  S32 trd, S32 reg, U32 val )
{
	BRD_Spd ctrl;
	S32		err;

	ctrl.dev = dev;
	ctrl.mode = is32bits;
	ctrl.num = num;
	ctrl.sync = synchr;
	ctrl.tetr = trd;
	ctrl.reg = reg;
	ctrl.val = val;
	err = BRD_ctrl(hReg, 0, BRDctrl_REG_WRITESPD, &ctrl);

	return err;
}

//
// End of File
//




