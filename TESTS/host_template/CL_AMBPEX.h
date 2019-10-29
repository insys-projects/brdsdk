//---------------------------------------------------------------------------

#ifndef TF_CheckItem_CL_AMBPEXH
#define TF_CheckItem_CL_AMBPEXH

#include "utypes.h"
#include "brd.h"
#include "time.h"
#include "ctrlstrm.h"
#include "exceptinfo.h"

class  CL_AMBPEX
{

	//! 1 - выполнена функция BRD_Init
	static int m_isInit;

	BRD_Handle m_hBrd;

	//! Логический идентификатор модуля
	U32		m_nLid;

	U64 m_blkPhysAdr;	//!< Физический адрес окна памяти 0 в ПЛИС - блоки управления

	U64 m_trdPhysAdr;	//!< Физический адрес окна памяти 1 в ПЛИС - тетрады

 public:
       // virtual char* GetName( void );  //!< Возвращает название класса

		int StreamInit( U32 strm, U32 cnt_buf, U32 size_one_buf_of_bytes, U32 trd, U32 dir, U32 cycle, U32 system, U32 agree_mode );

		int StreamInit_Mode8( void **ppBlk, U32 strm, U32 cnt_buf, U32 size_one_buf_of_bytes, U32 trd, U32 dir, U32 cycle, U32 agree_mode );

		void StreamDestroy( U32 strm );

		void StreamDestroyEx( U32 strm );

		U32* StreamGetBufByNum( U32 strm, U32 numBuf );

		void StreamStart( U32 strm );

		void StreamStartEx( U32 strm );

		void StreamStop( U32 strm );

		void StreamStopEx( U32 strm );

		int StreamGetBuf( U32 strm, U32** ptr );

		int StreamGetIndexDma( U32 strm );

		int StreamGetNextIndex( U32 strm, U32 index );

		void StreamGetBufDone( U32 strm );
		
		//! Доступ к дескриптору стрима
		BRD_Handle StreamGetHandle( U32 strm );

		//! Доступ к адресу блочка
		U32* StreamGetStub( U32 strm );

		// Доступ к регистрам 

		//! Запись в косвенно адресуемый регистр
        void RegPokeInd( S32 trdNo, S32 rgnum, U32 val );

		//! Чтение из косвенно адресуемого регистра
        U32 RegPeekInd( S32 trdNo, S32 rgnum );

		//! Запись в прямой регистр
        void RegPokeDir( S32 trdNo, S32 rgnum, U32 val );

		//! Чтение из прямого регистра
        U32 RegPeekDir( S32 trdNo, S32 rgnum );

		//! Получение физических адресов BAR0, BAR1
		void GetPhysAdr( U64 &blkAdr, U64 &trdAdr );

        CL_AMBPEX(U32 lid);

		~CL_AMBPEX( );

		//! Запрос дескриптора службы
        BRD_Handle GetSrvHandle( const BRDCHAR* name, U32 *atr, U32 mode = BRDcapt_EXCLUSIVE );

		//! Информация о модуле
		BRD_Info  info;

		//! Запрос дескрипотора модуля
		BRD_Handle  GetHandle( void );

		//! Вывод списка служб
		void ShowServiceList( void );

		//! Вывод информации о модуле
		void ShowBrdInfo( void );



private:
        //!  Инициализация модуля
        virtual U32 init( U32 lid );

        //!  Завершение работы с модулем
        virtual void cleanup( );

		//! Дескриптор служды для обращения к регистрам
		BRD_Handle  m_hReg;


	struct StreamParam 
	{
		U32 status;
		U32 statusStart;
		U32 strm;
		U32 cnt_buf;
		U32 size_one_buf_of_bytes;
		U32 trd;
		U32 cycle;
		U32 system;
		U32 dir;

		U32 indexDma;
		U32 indexPc;
		U32 agree_mode;

		BRD_Handle  hStream;
		BRDstrm_Stub	*pStub;
        void *pBlk[1024];         //!< Массив указателей на блоки памяти

		StreamParam()
		{
			memset( this, 0, sizeof( StreamParam ) );
		};
	};

		StreamParam  m_streamParam[2];


};
//---------------------------------------------------------------------------
#endif
