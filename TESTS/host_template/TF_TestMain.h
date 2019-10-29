
#include "TF_TestBufM2.h"
#include "TF_WorkParam.h"
#include "table_engine.h"
#include "tf_test.h"
#include "utypes.h"
#include "brd.h"

class CL_AMBPEX;
class TL_SharedIsvi;



class TF_TestMain : public TF_WorkParam, public TF_Test
{


    struct ParamExchange
    {
        U32 Strm;
        U32 Mode;
        U32 BlockWr;
        U32 BlockRd;
        U32 BlockOk;
        U32 BlockError;
        U32 TotalError;
        U32 DataType;	 	// Тип данных при фиксированном типе блока, 6 - счётчик, 8 - псевдослучайная последовательность
        U32 DataFix;		// 1 - фиксированный тип блока, 0 - данные в блоке записят от номера блока
        U32	trd;
        float	VelocityCurrent;
        float	VelocityAvarage;
        TF_TestBufM2 testBuf;

        U32 time_start;
        U32 time_last;
        U32 BlockLast;
        U32 freeCycle;
        U32 freeCycleZ;


        ParamExchange()
        {
            Strm=0;
            Mode=0;
            BlockWr=0;
            BlockRd=0;
            BlockOk=0;
            BlockError=0;
            TotalError=0;
            DataType=0;
            DataFix=0;
            trd=0;
            VelocityCurrent=0;
            VelocityAvarage=0;
            time_start=0;
            time_last=0;
            BlockLast=0;
            freeCycle=0;
            freeCycleZ=0;
        }
    };

    CL_AMBPEX  *pBrd;

    //! Имя теста
    BRDCHAR		m_TestName[256];

public:

    TF_TestMain( TableEngine  *pTemplateEngine, BRDCHAR* fname, CL_AMBPEX *brd, BRDCHAR* testName );
    virtual ~TF_TestMain();

    virtual int Prepare( U32 nStep );
    virtual void Start( void );
    virtual void Stop( void );
    virtual int isComplete( void );
    virtual void GetResult( void );
    virtual void StepTable( void );
    virtual void StepThread( void );




    //! Установка параметров по умолчанию
    virtual void SetDefault( void );

    //! Расчёт параметров
    virtual void CalculateParams( void );

    //! Отображение параметров
    virtual void ShowParam( void );

    S32 m_RowNumber;	//!< Номер строки таблицы для вывода значений
    TableEngine *m_pTemplateEngine;

    U32 BlockRd;
    U32 BlockOk;
    U32 BlockError;
    U32 TotalError;

    U32 CntBuffer;			// Число буферов стрима
    U32 CntBlockInBuffer;	// Число блоков в буфере
    U32 SizeBlockOfWords;	// Размер блока в словах
    U32 SizeBlockOfBytes;	// Размер блока в байтах
    U32 SizeBuferOfBytes;	// Размер буфера в байтах
    U32 SizeStreamOfBytes;	// Общий размер буфера стрима
    U32 isCycle;			// 1 - циклический режим работы стрима
    U32 isSystem;			// 1 - системная память
    U32 isAgreeMode;		// 1 - согласованный режим

    U32 trdNo;		// номер тетрады
    U32 strmNo;		// номер стрима
    U32 isSdram;	// 1 - проводить инициализацию SDRAM
    U32 isDDC;		// 1 - DDC, 2 - ADC
    U32 isTest;		// 1 - проверка псевдослучайной последовательности, 2 - проверка тестовой последовательности
    U32 isMainTest; // 1 - включение режима тестирования в тетараде MAIN
    U32 isShowParam; // 1 - отображение параметров

    U32 SdramSource;		 //!< Источник данных для SDRAM: 0 - АЦП, 1 - FDSP
    U32 SdramTestSequence;	 //!< 0x100 - включение 256-ти разрядной тестовой последовательности на входе SDRAM
    U32 SdramFullSpeed;		 //!< 1 - включение работы на полной скорости, требует установки SdramTestSequence=0x100
    U32 SdramFifoMode;		 //!< 1 - режим FIFO
    U32 SdramFifoOutRestart; //!< 1 - перезапуск только выходного FIFO тетрады TRD_DDR3x
    U32 SdramAzBase;		 //!< Начало активной зоны. Конец активной зоны определяется по размеру буфера стрима

    U32 SdramSplit;			//!< 1 - запись регистров AdrMask, AdrConst для разделения DDR3
    U32 SdramSplitMask;		//!< маска битов адреса для разделения DDR3
    U32 SdramSplitAdr;		//!< старшие биты адреса для разделения DDR3

    int m_isComplete;		//!< 1 - завершение выполнения теста



    ParamExchange	rd0;

    void ReceiveData(  void  );


    //! 1 - признак приёма первого блока
    U32 flagFirstBlock;

    //! Расчёт скорости приёма
    void CalculateVelocity( ParamExchange* pr, U32 currentTime, U32 interval, U32 scale );

private :

    //! Подготовка к работе
    void PrepareAdm( void );

    //! Программирование регистров из файла
    void PrepareAdmReg( char* fname, int showparam );

    //! Подготовка тетрады MAIN
    void PrepareMain( void );

    //! Подготовка DDR3
    void PrepareDDR3( int trd );

    //! Текущее время выполнения
    U32 currentTime;


public:

    //! 1 - измерение температуры ПЛИС
    U32 isSysmon;

    //! Получение текущей температуры ПЛИС
    void GetSysmon( U32& stat, float& temp );

    //! Получение текущей температуры ПЛИС - из тетрады MAIN, регистр 0x209
    void GetSysmonMain( float& temp );

    //! Получение текущей температуры ПЛИС - из тетрады DDR, регистр 0x207
    void GetSysmonDDR( float& temp );

    //! Вывод в ISVI через разделяемую память
    TL_SharedIsvi  *m_pSharedIsvi;


};


