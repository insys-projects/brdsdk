//
// Настоящий (real) Sleep
//
// (C)Copyright Instrumental Systems, July 2010
//

//#include "gipcy.h"

#ifndef REALDELAY

#define REALDELAY(a,b) RealDelay( (a),(b) )

#ifndef __linux__

#include "windows.h"

void inline RealDelay( int msec, int isSleep )
{
    int st = GetTickCount();
    int ct;

    do
    {
        ct = GetTickCount();

        if( isSleep )
            Sleep(0);
    }
    while( (ct -st) < msec );
}

#else

void inline RealDelay( int msec, int isSleep )
{
    IPC_delay(msec);
}

#endif //__linux__

#endif // REALDELAY
