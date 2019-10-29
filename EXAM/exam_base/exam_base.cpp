//=********************************************************
//
// TInt.Cpp
//
// Test Interrupt Exchange.
//
// (C) Instr.Sys. by Ekkore Nov. 2002
//
//=********************************************************

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h> 
#include	<ctype.h>
#include	<time.h>
#include	<signal.h>
#include	<unistd.h>

#include	"utypes.h"
#include	"brd.h"

#define CURRFILE	__FILE__
//
//==== Global Variables
//

static int flag = 0;

static void cleanup(int signal)     /* "dummy" needed for clean msvc compiles. */
{
    flag = 1;
}


BRD_Handle	*handle;

//
//==== Function Definitions
//
void	ParseCommandLine( int argc, char *argv[] );
void	DisplayBanner(void);
void	DisplayHelp(void);
void	DisplayLidList(void);


//=************************* main *************************
//=********************************************************
int	main( int argc, char *argv[] )
{
    S32			num;
    S32			err;
    BRD_Info		info = {sizeof(info)};

    signal(SIGINT, cleanup);        /* call cleanup on Ctrl-C */

    BRD_displayMode(BRDdm_VISIBLE);

    err = BRD_init( "brd.ini", &num );
    if( 0>err )
    {
        if( BRD_errcmp(err,BRDerr_NONE_DEVICE) )
            fprintf(stderr,  CURRFILE" ERROR: None Board\n" );
        else
            fprintf(stderr,  CURRFILE" ERROR: Can't Initialize BRD Shell\n" );
        return 1;
    }

    //
    // Define LID
    //
    U32	lidArray[16];
    U32	boardReal, servReal;

    BRD_lidList( lidArray, 16, &boardReal );
    if( boardReal==0 )
    {
        fprintf(stderr,  CURRFILE" ERROR: None Board\n" );
        BRD_cleanup();
        return 1;
    }

    DisplayLidList();

    handle = new BRD_Handle[boardReal];

    //
    // Open Device
    //
    for(U32 iDev=0; iDev<boardReal; iDev++) {

        handle[iDev] = BRD_open( lidArray[iDev], BRDopen_EXCLUSIVE, NULL );
        if(handle[iDev]<=0)
        {
            fprintf(stderr,  CURRFILE" ERROR: Can't Open device (LID=%d)\n", (int)lidArray[iDev] );
            DisplayLidList();
            continue;
        }

        fprintf(stderr,  CURRFILE" Open board (LID=%d)\n", (int)lidArray[iDev] );

        const int SrvNum = 32;
        BRD_ServList srvList[SrvNum];
        BRD_serviceList(handle[iDev], 0, srvList, SrvNum, &servReal);
        if(servReal < SrvNum)
        {
            BRD_Handle *hSrv = new BRD_Handle[servReal];
            for(U32 j = 0; j < servReal; j++)
            {
                fprintf(stderr,"Service %d: %s, Attr = %X.\n", (int)j, srvList[j].name, (int)srvList[j].attr);

                U32 mode = BRDcapt_SHARED;
                hSrv[j] = BRD_capture( handle[iDev], 0, &mode, srvList[j].name, 10000);
                if(hSrv[j] <= 0) {

                    fprintf(stderr, "Error capture %s\n", srvList[j].name);
                    continue;
                }
            }

            //fprintf(stderr,"press Enter to continue...\n");
            //getchar();//sleep(1);

            for(U32 j = 0; j < servReal; j++)
            {
                fprintf(stderr, "Release %s\n", srvList[j].name);
                BRD_release(hSrv[j], 0);
            }

            delete[] hSrv; 

        } else {
            fprintf(stderr,"BRD_serviceList: Real Items = %d (> %d - ERROR!!!)\n", (int)servReal, SrvNum);
        }

        BRD_reset( handle[iDev], NODE0 );
        BRD_close( handle[iDev] );

        fprintf(stderr,  CURRFILE" Close board (LID=%d)\n", (int)lidArray[iDev] );
    }

    delete[] handle;

    BRD_cleanup();

    fprintf(stderr,  CURRFILE" Cleanup library\n" );

    return 0;
}

//=******************** DisplayLidList ********************
//=********************************************************
void	DisplayLidList(void)
{
    U32			lidArray[16];
    U32			itemReal;
    BRD_Info	info = {sizeof(info)};
    U32			ii;


    BRD_lidList( lidArray, 16, &itemReal );
    if( itemReal==0 )
    {
        fprintf(stderr,  CURRFILE" ERROR: None Board\n\n" );
        return;
    }

    for( ii=0; ii<itemReal; ii++ )
    {
        BRD_getInfo( lidArray[ii], &info );
        fprintf(stderr,  "%d: LID=%d, BOARD=%s, PID=%d(0x%X), BUS=%d, SLOT=%d\n",
                (int)ii, (int)lidArray[ii], info.name, (int)info.pid, (int)info.pid, (int)info.bus, (int)info.slot );
    }
}

//
// End ofFile
//
