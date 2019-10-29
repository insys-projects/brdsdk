//---------------------------------------------------------------------------
#include <stdio.h>

#include	"brd.h"
#include	"brderr.h"
#include	"ctrlstrm.h"
#include    "ctrlreg.h"
#include	"CL_AMBPEX.h"

#include    "gipcy.h"

int CL_AMBPEX::m_isInit = 0;

//-----------------------------------------------------------------------------

U32  CL_AMBPEX::init(U32 lid)
{
    S32 err;
    S32 num;

    try {
        m_hBrd = 0;
        m_nLid = lid;

        //BRD_displayMode(BRDdm_VISIBLE);
        //	err = BRD_initEx( BRDinit_FILE_KNOWN, "brd.ini", "log.ini", &num );

        if( 0 == m_isInit )
        {
            err = BRD_init( _BRDC( "brd.ini" ), &num );
            if( !BRD_errcmp( err, BRDerr_OK ) )
            {
                throw except_info("%s, %d: %s() - Error in BRD_init()! Error code: 0x%.8X\n", __FILE__, __LINE__, __FUNCTION__, err);
            }
            m_isInit = 1;
        }

        info.size = sizeof(info);
        {
            m_hBrd=BRD_open( lid, BRDopen_SHARED, NULL );
        }
        if( m_hBrd<=0)
        {
            throw except_info("%s, %d: %s() - Board with LID %d not found. Error in BRD_open()!\n", __FILE__, __LINE__, __FUNCTION__, lid);

        } else {
            BRDC_printf( _BRDC("BRD_open( Lid: %d ) - Ok\r\n"), lid );
            BRD_getInfo( lid, &info );

            m_blkPhysAdr = info.base[0];
            m_trdPhysAdr = info.base[1];
        }

        ShowServiceList();

        U32 atr=0;
        m_hReg = GetSrvHandle( _BRDC( "REG0" ), &atr, BRDcapt_SHARED );

        if( !m_hReg )
        {
            throw except_info("%s, %d: %s() - REG0 not found!\n", __FILE__, __LINE__, __FUNCTION__);
        }

        // сброс прошивки ПЛИС
        RegPokeInd( 0, 0, 1 );
        IPC_delay( 100 );
        RegPokeInd( 0, 0, 0 );
        IPC_delay( 100 );

        for( int trd=0; trd<8; trd++ )
        {
            for( int ii=0; ii<32; ii++ )
            {
                RegPokeInd( trd, ii, 0 );
            }
        }

    } catch (except_info_t& err) {

        cleanup();
        throw err;

    } catch (...) {

        cleanup();
        throw except_info("%s, %d: %s() - Unknown error in CL_AMBPEX::init()!\n", __FILE__, __LINE__, __FUNCTION__);
    }

    return 0;
}

//-----------------------------------------------------------------------------

void  CL_AMBPEX::cleanup()
{
    StreamStop(0);
    StreamStop(1);
    StreamDestroy(0);
    StreamDestroy(1);

    if(m_hReg) {
        BRD_release(m_hReg, 0);
    }
    if(m_hBrd) {
        BRD_close(m_hBrd);
    }
    BRD_cleanup();
}

//-----------------------------------------------------------------------------

CL_AMBPEX::CL_AMBPEX(U32 lid)
{
    init(lid);
}

//-----------------------------------------------------------------------------

CL_AMBPEX::~CL_AMBPEX()
{
    try {
        cleanup();
    } catch (except_info_t& err) {
        fprintf(stderr, "%s, %d: %s() - %s\n", __FILE__, __LINE__, __FUNCTION__, err.info.c_str());
    } catch (...) {
        fprintf(stderr, "%s, %d: %s() - Unknown error in CL_AMBPEX::~CL_AMBPEX()!\n", __FILE__, __LINE__, __FUNCTION__);
    }
}

//-----------------------------------------------------------------------------

//! Запрос дескриптора службы
/**

    \param name		имя службы
    \param atr		атрибуты службы
    \return			дескриптор службы

*/
BRD_Handle  CL_AMBPEX::GetSrvHandle( const BRDCHAR* name, U32 *atr, U32 mode )
{
    U32 iSrv;
    BRD_Handle handle;

    //! Службы модуля
    BRD_ServList srvList[20];

    //! Число служб
    U32     itemReal;




    // Определение списка служб
    BRD_serviceList( m_hBrd, 0, srvList, 20, &itemReal);
    /*
      if( itemReal <= 20)
      {

        for(U32 j = 0; j < itemReal; j++)
        {
                BRDC_printf(_BRDC("Service %d: %s, Attr = %X.\n"),
                                                j, srvList[j].name, srvList[j].attr);
        }

      }
      */

    for(iSrv = 0; iSrv < itemReal; iSrv++)
    {
        if(!BRDC_strcmp( srvList[iSrv].name, name ))
        {
            //handle = ::GetSrvHandle( m_handle, &m_srvList[iSrv] );

            // U32 mode = BRDcapt_EXCLUSIVE;
            handle = BRD_capture( m_hBrd, 0, &mode, srvList[iSrv].name, 10000);
            //if(mode == BRDcapt_EXCLUSIVE) printf("%s Capture mode: EXCLUSIVE\n", m_srvList[iSrv].name);
            //if(mode == BRDcapt_SPY)	printf("%s Capture mode: SPY\n", m_srvList[iSrv].name);
            if(handle <= 0)
            {
                printf("Error %s Capture\n", srvList[iSrv].name);
                *atr=0;
                return 0;
            }

            if( atr )
                *atr=srvList[iSrv].attr;
            return handle;
        }
    }
    *atr=0;
    return 0;
}



//! Вывод списка служб
/**

    \param name		имя службы
    \param atr		атрибуты службы
    \return			дескриптор службы

*/
void  CL_AMBPEX::ShowServiceList( void )
{
    //! Службы модуля
    BRD_ServList srvList[20];

    //! Число служб
    U32     itemReal;


    BRDC_printf( _BRDC("\n" ));

    // Определение списка служб
    BRD_serviceList( m_hBrd, 0, srvList, 20, &itemReal);
    if( itemReal <= 20)
    {
        for(U32 j = 0; j < itemReal; j++)
        {
            BRDC_printf(_BRDC("Service %d: %s, Attr = %X.\n"),
                        j, srvList[j].name, srvList[j].attr);
        }

    }

    BRDC_printf( _BRDC("\n" ));

}

//=********************* RegPokeInd *******************
//=****************************************************
void    CL_AMBPEX::RegPokeInd( S32 trdNo, S32 rgnum, U32 val )
{
    BRD_Reg params;

    params.bytes = sizeof( U32 );
    params.reg = rgnum;
    params.tetr = trdNo;
    params.val = val;

    BRD_ctrl(m_hReg, 0, BRDctrl_REG_WRITEIND, &params);
}

//=********************* RegPeekInd *******************
//=****************************************************
U32    CL_AMBPEX::RegPeekInd( S32 trdNo, S32 rgnum )
{
    BRD_Reg params;
    U32 ret;

    params.bytes = sizeof( U32 );
    params.reg = rgnum;
    params.tetr = trdNo;
    params.val = 0;

    BRD_ctrl(m_hReg, 0, BRDctrl_REG_READIND, &params);
    ret=params.val;
    ret &= 0xFFFF;
    return ret;
}

//=********************* RegPokeDir *******************
//=****************************************************
void    CL_AMBPEX::RegPokeDir( S32 trdNo, S32 rgnum, U32 val )
{
    BRD_Reg params;

    params.bytes = sizeof( U32 );
    params.reg = rgnum;
    params.tetr = trdNo;
    params.val = val;

    BRD_ctrl(m_hReg, 0, BRDctrl_REG_WRITEDIR, &params);
}

//=********************* RegPeekDir *******************
//=****************************************************
U32    CL_AMBPEX::RegPeekDir( S32 trdNo, S32 rgnum )
{
    BRD_Reg params;
    U32 ret;

    params.bytes = sizeof( U32 );
    params.reg = rgnum;
    params.tetr = trdNo;
    params.val = 0;

    BRD_ctrl(m_hReg, 0, BRDctrl_REG_READDIR, &params);
    ret=params.val;
    return ret;
}




int CL_AMBPEX::StreamInit( U32 strm, U32 cnt_buf, U32 size_one_buf_of_bytes, U32 trd, U32 dir, U32 cycle, U32 system, U32 agree_mode )
{
    if( strm>1 )
        return 1;

    StreamParam *pStrm= m_streamParam+strm;
    if( pStrm->status!=0 )
        return 1;

    pStrm->cnt_buf	= cnt_buf;
    pStrm->size_one_buf_of_bytes =	size_one_buf_of_bytes;
    pStrm->trd		= trd;
    pStrm->cycle	= cycle;
    pStrm->system	= system;

    pStrm->indexDma=-1;
    pStrm->indexPc=-1;
    pStrm->agree_mode=agree_mode;

    StreamDestroy( strm );

    U32 atr=0;
    const BRDCHAR* name;
    if( strm==0 )
        name = _BRDC("BASESTREAM0");
    else
        name = _BRDC("BASESTREAM1");

    pStrm->hStream = GetSrvHandle( name, &atr );

    if( !pStrm->hStream)
    {
        BRDC_printf( _BRDC("CL_AMBPEX::srv_init - Ошибка - служба %s не захвачена\n"), name );
        return 1; // Служба не захвачена
    }

    /*
     BRDC_printf( _BRDC("Служба %s: %.8X  %.8X\r\n"), name, pStrm->hStream, atr );
        if( atr & BRDserv_ATTR_STREAM )
          BRDC_printf( _BRDC("      BRDserv_ATTR_STREAM       - Ok\r\n") );
        if( atr & BRDserv_ATTR_DIRECTION_IN )
          BRDC_printf( _BRDC("      BRDserv_ATTR_DIRECTION_IN - Ok\r\n") );
        if( atr & BRDserv_ATTR_STREAMABLE_IN )
          BRDC_printf( _BRDC("      BRDserv_ATTR_STREAMABLE_IN - Ok\r\n") );
    */


    //U32 size=cnt_buf*size_one_buf_of_bytes/(1024*1024);
    U64 size=cnt_buf*(U64)size_one_buf_of_bytes/(1024*1024);

    U32 _size = size;
    if( system )
    {
        BRDC_printf( _BRDC("Выделение памяти: \r\n")
                     _BRDC(" Тип блока:    Непрерывный (системная память)\r\n")
                     _BRDC(" Размер блока: %d МБ  (%dx%d МБ)\r\n"), _size, cnt_buf, size_one_buf_of_bytes/(1024*1024) );
    } else
    {
        BRDC_printf( _BRDC("Выделение памяти: \r\n")
                     _BRDC(" Тип блока:    Разрывный (пользовательская память)\r\n")
                     _BRDC(" Размер блока: %d МБ  (%dx%d МБ)\r\n"), _size, cnt_buf, size_one_buf_of_bytes/(1024*1024) );
    }


    //! Выделение памяти для службы
    BRDctrl_StreamCBufAlloc	 sSCA = {
        dir,							//!< Направление работы стрима, 1 - в ПЭВМ, 2 - из ПЭВМ
        system,						//!< Как создавать составной буфера, 0 - разрывный
        cnt_buf,						//!< Количество блоков в составном буфере
        size_one_buf_of_bytes,		//!< Размер одного блока
        (void**)(pStrm->pBlk),		//!< Массив адресов блоков
        NULL							//!< Указатель на блочек
    };


    //! Вызов с реальным количеством блоков
    //sSCA.blkNum=bl_cnt2;
    S32 err = BRD_ctrl( pStrm->hStream, 0, BRDctrl_STREAM_CBUF_ALLOC, &sSCA );

    pStrm->pStub=sSCA.pStub;
    if(!BRD_errcmp(err, BRDerr_OK) )
    {
        //err_code=4; // Ошибка выделения памяти для стрима
        //printf( "Ошибка выделения памяти для стрима\r\n" );
        throw( _BRDC("Ошибка выделения памяти для стрима\r\n") );
        //            scr.gl_step( 1 );
    }  else
    {
        BRDC_printf( _BRDC("Выделение памяти - Ok\r\n\r\n") );
    }


    //err = BRD_ctrl( m_hStream, 0, BRDctrl_TESTEX_GETINSRCSTREAM, &trd );
    //printf( "Источник данных: %.8X \r\n", trd );

    err = BRD_ctrl( pStrm->hStream, 0, BRDctrl_STREAM_SETSRC, &trd );


    // Перевод на согласованный режим работы
    if( agree_mode )
    {
        BRDctrl_StreamCBufAdjust arg;
        arg.isAdjust=1;
        err = BRD_ctrl( pStrm->hStream, 0, BRDctrl_STREAM_CBUF_ADJUST, &arg );
        if(!BRD_errcmp(err, BRDerr_OK) )
        {
            BRDC_printf( _BRDC("Ошибка перевода в согласованный режим: 0x%.8X\r\n"), err );
        } else
        {
            BRDC_printf( _BRDC("Стрим переведён в согласованный режим\r\n") );
        }
    } else
    {
        BRDC_printf( _BRDC("Стрим работает в несогласованном режиме\r\n") );
    }
    BRDC_printf( _BRDC("\r\n") );

    //StreamStop( strm );

    err = BRD_ctrl( pStrm->hStream, 0, BRDctrl_STREAM_CBUF_STOP, NULL);

    err = BRD_ctrl( pStrm->hStream, 0, BRDctrl_STREAM_RESETFIFO, NULL );


    err = BRD_ctrl( pStrm->hStream, 0, BRDctrl_STREAM_RESETFIFO, NULL );

    pStrm->status=1;

    return 0;
}

/**
    \brief  Выделение памяти для стрима в режиме 8

    В режиме 8 массив ppBlk содержит физические адреса блоков памяти.
    Бит 0 адреса используется для факта создания прерывания. Наличие 1 означает что прерывание будет сформировано.

*/
int CL_AMBPEX::StreamInit_Mode8( void **ppBlk, U32 strm, U32 cnt_buf, U32 size_one_buf_of_bytes, U32 trd, U32 dir, U32 cycle,  U32 agree_mode )
{
    if( strm>1 )
        return 1;

    StreamParam *pStrm= m_streamParam+strm;
    if( pStrm->status!=0 )
        return 1;

    pStrm->cnt_buf	= cnt_buf;
    pStrm->size_one_buf_of_bytes =	size_one_buf_of_bytes;
    pStrm->trd		= trd;
    pStrm->cycle	= cycle;
    pStrm->system	= 8;

    pStrm->indexDma=-1;
    pStrm->indexPc=-1;
    pStrm->agree_mode=agree_mode;

    StreamDestroy( strm );

    U32 atr=0;
    const BRDCHAR* name = strm ? _BRDC("BASESTREAM1") : _BRDC("BASESTREAM0");

    pStrm->hStream = GetSrvHandle( name, &atr );

    if( !pStrm->hStream)
    {
        BRDC_printf( _BRDC("CL_AMBPEX::srv_init - Ошибка - служба %s не захвачена\n"), name );
        return 1; // Служба не захвачена
    }

    /*
     BRDC_printf( _BRDC("Служба %s: %.8X  %.8X\r\n"), name, pStrm->hStream, atr );
        if( atr & BRDserv_ATTR_STREAM )
          BRDC_printf( _BRDC("      BRDserv_ATTR_STREAM       - Ok\r\n") );
        if( atr & BRDserv_ATTR_DIRECTION_IN )
          BRDC_printf( _BRDC("      BRDserv_ATTR_DIRECTION_IN - Ok\r\n") );
        if( atr & BRDserv_ATTR_STREAMABLE_IN )
          BRDC_printf( _BRDC("      BRDserv_ATTR_STREAMABLE_IN - Ok\r\n") );
    */


    //U32 size=cnt_buf*size_one_buf_of_bytes/(1024*1024);
    U64 size=cnt_buf*(U64)size_one_buf_of_bytes/(1024*1024);

    U32 _size = size;
    BRDC_printf( _BRDC("Выделение памяти: \r\n")
                 _BRDC(" Режим выделения 8\r\n")
                 _BRDC(" Размер блока: %d МБ  (%dx%d МБ)\r\n"), _size, cnt_buf, size_one_buf_of_bytes/(1024*1024) );

    for( unsigned ii=0; ii<cnt_buf; ii++ )
    {
        pStrm->pBlk[ii] = ppBlk[ii];
    }

    for( int ii=cnt_buf; ii<1024; ii++ )
    {
        pStrm->pBlk[ii] = NULL;
    }

    //! Выделение памяти для службы
    BRDctrl_StreamCBufAlloc	 sSCA = {
        dir,							//!< Направление работы стрима, 1 - в ПЭВМ, 2 - из ПЭВМ
        8,							//!< Как создавать составной буфера, 8 - создание из таблицы физических адресов с маской прерывания
        cnt_buf,						//!< Количество блоков в составном буфере
        size_one_buf_of_bytes,		//!< Размер одного блока
        (void**)(pStrm->pBlk),		//!< Массив адресов блоков
        NULL							//!< Указатель на блочек
    };


    //! Вызов с реальным количеством блоков
    //sSCA.blkNum=bl_cnt2;
    S32 err = BRD_ctrl( pStrm->hStream, 0, BRDctrl_STREAM_CBUF_ALLOC, &sSCA );

    pStrm->pStub=sSCA.pStub;
    if(!BRD_errcmp(err, BRDerr_OK) )
    {
        //err_code=4; // Ошибка выделения памяти для стрима
        //printf( "Ошибка выделения памяти для стрима\r\n" );
        throw( _BRDC("Ошибка выделения памяти для стрима\r\n") );
        //            scr.gl_step( 1 );
    }  else
    {
        BRDC_printf( _BRDC("Выделение памяти - Ok\r\n\r\n") );
    }


    //err = BRD_ctrl( m_hStream, 0, BRDctrl_TESTEX_GETINSRCSTREAM, &trd );
    //printf( "Источник данных: %.8X \r\n", trd );

    err = BRD_ctrl( pStrm->hStream, 0, BRDctrl_STREAM_SETSRC, &trd );


    // Перевод на согласованный режим работы
    if( agree_mode )
    {
        BRDctrl_StreamCBufAdjust arg;
        arg.isAdjust=1;
        err = BRD_ctrl( pStrm->hStream, 0, BRDctrl_STREAM_CBUF_ADJUST, &arg );
        if(!BRD_errcmp(err, BRDerr_OK) )
        {
            BRDC_printf( _BRDC("Ошибка перевода в согласованный режим: 0x%.8X\r\n"), err );
        } else
        {
            BRDC_printf( _BRDC("Стрим переведён в согласованный режим\r\n") );
        }
    } else
    {
        BRDC_printf( _BRDC("Стрим работает в несогласованном режиме\r\n") );
    }
    BRDC_printf( _BRDC("\r\n") );

    //StreamStop( strm );

    err = BRD_ctrl( pStrm->hStream, 0, BRDctrl_STREAM_CBUF_STOP, NULL);

    err = BRD_ctrl( pStrm->hStream, 0, BRDctrl_STREAM_RESETFIFO, NULL );


    err = BRD_ctrl( pStrm->hStream, 0, BRDctrl_STREAM_RESETFIFO, NULL );

    pStrm->status=1;

    return 0;
}

int CL_AMBPEX::StreamGetNextIndex( U32 strm, U32 index )
{
    if( strm>1 )
        return 0;

    StreamParam *pStrm= m_streamParam+strm;
    unsigned n=index+1;
    if( n>=pStrm->cnt_buf )
        n=0;
    return n;

}

void CL_AMBPEX::StreamDestroy( U32 strm )
{
    if( strm>1 )
        return;

    StreamParam *pStrm= m_streamParam+strm;
    if( pStrm->status==0 )
        return;

    StreamStop( strm );
    if( pStrm->hStream )
    {
        BRD_ctrl( pStrm->hStream, 0, BRDctrl_STREAM_CBUF_FREE, NULL );
        // BRDC_printf( _BRDC("\r\nОсвобождение стрима  %.8X\r\n"), err );
        BRD_release( pStrm->hStream, 0);
        pStrm->hStream=0;
    }
    pStrm->status=0;

}

U32* CL_AMBPEX::StreamGetBufByNum( U32 strm, U32 numBuf )
{
    if( strm>1 )
        return NULL;

    StreamParam *pStrm= m_streamParam+strm;
    if( pStrm->status!=1 )
        return NULL;

    U32 *ptr;
    if( numBuf>=pStrm->cnt_buf )
        return NULL;
    ptr=(U32*)(pStrm->pBlk[numBuf]);
    return ptr;
}

void CL_AMBPEX::StreamStart( U32 strm )
{

    if( strm>1 )
        return;

    StreamParam *pStrm= m_streamParam+strm;
    if( pStrm->status!=1 )
        return;

    U32 val;

    val=RegPeekInd( pStrm->trd, 0 );
    BRD_ctrl( pStrm->hStream, 0, BRDctrl_STREAM_CBUF_STOP, NULL);
    StreamStop( strm );
    StreamStop( strm );


    pStrm->indexDma=-1;
    pStrm->indexPc=-1;

    //DRV_ctrl( g_hDrv, DDDcmd_KNOTRESETFIFO, NULL );

    //if( 0==strm )
    //break;
    val=pStrm->cycle; // 0 - однократный режим, 1 - циклический
    BRD_ctrl( pStrm->hStream, 0, BRDctrl_STREAM_CBUF_START, &val );
    pStrm->statusStart = 1;
}


void CL_AMBPEX::StreamStartEx( U32 strm )
{

    if( strm > 1 )
        return;

    StreamParam *pStrm = m_streamParam + strm;
    if( pStrm->status != 1 )
        return;

    U32 val;

    /*
    val = RegPeekInd( pStrm->trd, 0 );
    err = BRD_ctrl( pStrm->hStream, 0, BRDctrl_STREAM_CBUF_STOP, NULL );
    StreamStop( strm );
    StreamStop( strm );
    */


    pStrm->indexDma = -1;
    pStrm->indexPc = -1;

    //DRV_ctrl( g_hDrv, DDDcmd_KNOTRESETFIFO, NULL );

    //if( 0==strm )
    //break;
    val = pStrm->cycle; // 0 - однократный режим, 1 - циклический
    BRD_ctrl( pStrm->hStream, 0, BRDctrl_STREAM_CBUF_START, &val );
    pStrm->statusStart = 1;
}


void CL_AMBPEX::StreamStop( U32 strm )
{
    if( strm>1 )
        return;


    StreamParam *pStrm= m_streamParam+strm;

    pStrm->statusStart = 0;
    pStrm->indexDma = -1;
    pStrm->indexPc = -1;


    RegPokeInd( pStrm->trd, 0, 2 );

    BRD_ctrl( pStrm->hStream, 0, BRDctrl_STREAM_CBUF_STOP, NULL);

    BRD_ctrl( pStrm->hStream, 0, BRDctrl_STREAM_RESETFIFO, NULL );

    RegPokeInd( pStrm->trd, 0, 0 );

}

void CL_AMBPEX::StreamStopEx( U32 strm )
{
    if( strm>1 )
        return;


    StreamParam *pStrm= m_streamParam+strm;

    pStrm->statusStart = 0;
    pStrm->indexDma = -1;
    pStrm->indexPc = -1;


    //RegPokeInd( pStrm->trd, 0, 2 );

    BRD_ctrl( pStrm->hStream, 0, BRDctrl_STREAM_CBUF_STOP, NULL);

    //err = BRD_ctrl( pStrm->hStream, 0, BRDctrl_STREAM_RESETFIFO, NULL );

    //RegPokeInd( pStrm->trd, 0, 0 );
}

int CL_AMBPEX::StreamGetBuf( U32 strm, U32** ptr )
{
    U32 *buf;
    int ret=0;

    if( strm>1 )
        return 0;

    StreamParam *pStrm= m_streamParam+strm;

    if( 1 != pStrm->statusStart )
        return 0;


    if( pStrm->indexPc==pStrm->indexDma )
    {
        pStrm->indexDma = StreamGetIndexDma( strm );
    }
    if( (pStrm->indexPc & 0xFFFFFF)!=(pStrm->indexDma&0xFFFFFF) )
    {
        pStrm->indexPc=StreamGetNextIndex( strm, pStrm->indexPc );
        buf = StreamGetBufByNum( strm, pStrm->indexPc );
        *ptr = buf;
        ret=1;
        //StreamGetBufDone( strm );
    }
    return ret;
}

int CL_AMBPEX::StreamGetIndexDma( U32 strm )
{
    if( strm>1 )
        return -1;

    StreamParam *pStrm= m_streamParam+strm;



    /*
         BRDctrl_StreamCBufState state;

        state.timeout= 0;
        Sleep(10);


        S32 err;
        int ret=pStrm->indexDma;
        err = BRD_ctrl( pStrm->hStream, 0, BRDctrl_STREAM_CBUF_STATE, &state );
        if( BRD_errcmp(err, BRDerr_OK) )
        {
                ret=state.blkNum;

        } else
        {
            int aa=state.blkNum;

            ret=pStrm->indexDma;
            if( pStrm->indexDma!=state.blkNum )
                aa++;
        }
        return ret;
     */
    int ret = pStrm->pStub->lastBlock;
    return ret;
}

void CL_AMBPEX::StreamGetBufDone( U32 strm )
{
    if( strm>1 )
        return;

    StreamParam *pStrm= m_streamParam+strm;
    S32 err;
    static U32 err_code=0;

    if( pStrm->agree_mode )
    {
        BRDctrl_StreamCBufDone arg;
        arg.blkNo=pStrm->indexPc;
        err = BRD_ctrl( pStrm->hStream, 0, BRDctrl_STREAM_CBUF_DONE, &arg);
        if(!BRD_errcmp(err, BRDerr_OK) )
            err_code++; // Ошибка перевода в согласованный режим
    }
}


//! Запрос дескрипотора модуля
BRD_Handle  CL_AMBPEX::GetHandle( void )
{
    return m_hBrd;
}

/**
        Освобождение памяти и службы стрима без остановки тетрады
*/
void CL_AMBPEX::StreamDestroyEx( U32 strm )
{
    if( strm > 1 )
        return;

    StreamParam *pStrm = m_streamParam + strm;
    if( pStrm->status == 0 )
        return;

    //StreamStop( strm );
    if( pStrm->hStream )
    {
        BRD_ctrl( pStrm->hStream, 0, BRDctrl_STREAM_CBUF_FREE, NULL );
        // BRDC_printf( _BRDC("\r\nОсвобождение стрима  %.8X\r\n"), err );
        BRD_release( pStrm->hStream, 0 );
        pStrm->hStream = 0;
    }
    pStrm->status = 0;
}

//! Вывод информации о модуле
void CL_AMBPEX::ShowBrdInfo( void )
{
    BRD_Info  sInfo;  // Информация о модуле
    sInfo.size = sizeof( BRD_Info );
    U32 status;
    S32 err = BRD_getInfo( m_nLid, &sInfo );


    if( BRD_errcmp( err, BRDerr_OK ) ) {
        BRDC_printf( _BRDC( "\r\nПараметры модуля:\r\n" )
                     _BRDC( " LID:       %d\r\n" )
                     _BRDC( " BoardType: %.8X \r\n" )
                     _BRDC( " Имя:       %s  v%d.%d\r\n" )
                     _BRDC( " Номер:     %d\r\n" )
                     _BRDC( " Order:     %d\r\n" )
                     _BRDC( " Bus:       %d\r\n" )
                     _BRDC( " Dev:       %d\r\n" )
                     _BRDC( " Slot:      %d\r\n\r\n" ),
                     m_nLid,
                     sInfo.boardType,
                     sInfo.name,
                     ( sInfo.boardType >> 4 ) & 0xF,
                     sInfo.boardType & 0xF,
                     sInfo.pid & 0xFFFFFFF,
                     (sInfo.pid >> 28) & 0xF,
                     sInfo.bus,
                     sInfo.dev,
                     sInfo.slot
                     );
    }
    else {

        BRDC_printf( _BRDC( "\r\n Ошибка при получении информации о модуле\r\n" )
                     _BRDC( "BRD_getInfo() LID=%d  ret=%.8X\r\n\r\n" ), m_nLid, err );

        throw( 1 );
    }




    {
        U32 d, d1, d2, d3, d4, d5, ii;
        const char *str;

        BRDC_printf( _BRDC( "Прошивка ПЛИС ADM\r\n" ) );
        this->RegPokeDir( 0, 1, 1 );


        d = this->RegPeekInd( 0, 0x108 );
        if( d == 0x4953 ) {
            BRDC_printf( _BRDC( "  SIG= 0x%.4X - Ok	\n" ), d );
        }
        else {
            BRDC_printf( _BRDC( "  SIG= 0x%.4X - Ошибка, ожидается 0x4953	\n" ), d );
            throw( 1 );
        }

        d = this->RegPeekInd( 0, 0x109 );  BRDC_printf( _BRDC("   Версия интерфейса ADM:  %d.%d\n"), d >> 8, d & 0xFF );
        d = this->RegPeekInd( 0, 0x110 ); d1 = this->RegPeekInd( 0, 0x111 );
        BRDC_printf( _BRDC("   Базовый модуль: 0x%.4X  v%d.%d\n"), d, d1 >> 8, d1 & 0xFF );

        d = this->RegPeekInd( 0, 0x112 ); d1 = this->RegPeekInd( 0, 0x113 );
        BRDC_printf( _BRDC("   Субмодуль:      0x%.4X  v%d.%d\n"), d, d1 >> 8, d1 & 0xFF );

        d = this->RegPeekInd( 0, 0x10B );  BRDC_printf( _BRDC("   Модификация прошивки ПЛИС:  %d \n"), d );
        d = this->RegPeekInd( 0, 0x10A );  BRDC_printf( _BRDC( "   Версия прошивки ПЛИС:       %d.%d\n"), d >> 8, d & 0xFF );
        d = this->RegPeekInd( 0, 0x114 );  BRDC_printf( _BRDC( "   Номер сборки прошивки ПЛИС: 0x%.4X\n"), d );

        BRDC_printf( _BRDC( "\nИнформация о тетрадах:\n\n" ));
        for( ii = 0; ii < 16; ii++ ) {

            d = this->RegPeekInd( ii, 0x100 );
            d1 = this->RegPeekInd( ii, 0x101 );
            d2 = this->RegPeekInd( ii, 0x102 );
            d3 = this->RegPeekInd( ii, 0x103 );
            d4 = this->RegPeekInd( ii, 0x104 );
            d5 = this->RegPeekInd( ii, 0x105 );
            status = this->RegPeekDir( ii, 0 ) & 0xFFFF;

            switch( d ) {
            case 1:    str = "TRD_MAIN         "; break;
            case 2:    str = "TRD_BASE_DAC     "; break;
            case 3:    str = "TRD_PIO_STD      "; break;
            case 0:    str = " -               "; break;
            case 0x47: str = "SBSRAM_IN        "; break;
            case 0x48: str = "SBSRAM_OUT       "; break;
            case 0x12: str = "DIO64_OUT        "; break;
            case 0x13: str = "DIO64_IN         "; break;
            case 0x14: str = "ADM212x200M      "; break;
            case 0x5D: str = "ADM212x500M      "; break;
            case 0x41: str = "DDS9956          "; break;
            case 0x4F: str = "TEST_CTRL        "; break;
            case 0x3F: str = "ADM214x200M      "; break;
            case 0x40: str = "ADM216x100       "; break;
            case 0x2F: str = "ADM28x1G         "; break;
            case 0x2D: str = "TRD128_OUT       "; break;
            case 0x4C: str = "TRD128_IN        "; break;
            case 0x30: str = "ADMDDC5016       "; break;
            case 0x2E: str = "ADMFOTR2G        "; break;
            case 0x49: str = "ADMFOTR3G        "; break;
            case 0x67: str = "DDS9912          "; break;
            case 0x70: str = "AMBPEX5_SDRAM    "; break;
            case 0x71: str = "TRD_MSG          "; break;
            case 0x72: str = "TRD_TS201        "; break;
            case 0x73: str = "TRD_STREAM_IN    "; break;
            case 0x74: str = "TRD_STREAM_OUT   "; break;
            case 0x7C: str = "TRD_SUBUNIT      "; break;
            case 0x8A: str = "TRD_FM401S       "; break;
            case 0x8C: str = "TRD_FDSP         "; break;

            case 0xC6: str = "TRD_NIITP411_IN  "; break;
            case 0xC7: str = "TRD_NIITP411_OUT "; break;
            case 0x9B: str = "TRD_DDR3x        "; break;
            case 0x85: str = "TRD_M25P128      "; break;

            default: str = "UNKNOW        "; break;
            }
            BRDC_printf( _BRDC(" %d  0x%.4X 0x%.4X %s "), ii, status, d, str );
            if( d > 0 ) {
                BRDC_printf( _BRDC( " MOD: %-2d VER: %d.%d "), d1, d2 >> 8, d2 & 0xFF );
                if( d3 & 0x10 ) {
                    BRDC_printf( _BRDC( "FIFO IN   %dx%d\n"), d4, d5 );
                }
                else if( d3 & 0x20 ) {
                    BRDC_printf( _BRDC( "FIFO OUT  %dx%d\n"), d4, d5 );
                }
                else {
                    BRDC_printf( _BRDC( "\n") );
                }
            }
            else {
                BRDC_printf( _BRDC( "\n") );
            }

        }


        BRDC_printf( _BRDC( "\n") );

    }
}

//! Доступ к дескриптору стрима
BRD_Handle CL_AMBPEX::StreamGetHandle(U32 strm)
{
    if( strm > 1 )
        return 0;

    StreamParam *pStrm = m_streamParam + strm;
    return pStrm->hStream;
}

//! Доступ к адресу блочка
U32* CL_AMBPEX::StreamGetStub(U32 strm)
{
    if( strm > 1 )
        return 0;

    StreamParam *pStrm = m_streamParam + strm;

    U32 *ret = (U32*)(pStrm->pStub);
    return ret;

}

//! Получение физических адресов BAR0, BAR1
void CL_AMBPEX::GetPhysAdr(U64 &blkAdr, U64 &trdAdr)
{
    blkAdr = m_blkPhysAdr;
    trdAdr = m_trdPhysAdr;
}



//#pragma package(smart_init)




