

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "utypes.h"

#include "TF_TestMain.h"

#include "CL_AMBPEX.h"

#include "TL_SharedIsvi.h"



#define BUFSIZEPKG 62

#define TRDIND_MODE0					0x0
#define TRDIND_STMODE					0x5
#define TRDIND_MODE1					0x9
#define TRDIND_MODE2					0xA
#define TRDIND_TESTSEQUENCE				0xC

#define TRDIND_SPD_DEVICE				0x203
#define TRDIND_SPD_CTRL					0x204
#define TRDIND_SPD_ADDR					0x205
#define TRDIND_SPD_DATA					0x206
#define TRDIND_SPD_DATAH				0x206




int g_fileMap;
BRD_Handle g_hSRV;
ULONG g_MemAsFifo;
ULONG g_AdcDrqFlag;
ULONG g_MemDrqFlag;
double g_samplRate;
int g_IoDelay;

BRDCHAR g_AdcSrvName[256]; // с номером службы
ULONG g_Cycle;
U64 g_samplesOfChannel;
U64 g_bBufSize;
int g_DirWriteFile;
ULONG g_FileBufSize;
BRDCHAR g_dirFileName[256];


TF_TestMain::TF_TestMain( TableEngine  *pTemplateEngine, BRDCHAR* fname, CL_AMBPEX *brd, BRDCHAR* testName ) : TF_Test()
{

	BlockRd=0;
	BlockOk=0;
	BlockError=0;
	TotalError=0;
    m_pTemplateEngine = pTemplateEngine;

	BRDC_strcpy( m_TestName, testName );

	pBrd=brd;

	m_isComplete=0;

    //char *str = "data";
	
    m_pSharedIsvi = new TL_SharedIsvi( "data", 8 );

	TL_SharedIsvi::IsviParam  isviParam;

    //isviParam.deviceName[128];	//!< Имя устройства
	isviParam.numberOfChannels=1;	//!< Число каналов
    //isviParam.numbersOfChannels[128];	//!< Номера каналов
	isviParam.numberOfSamples=32768;		//!< Число отсчётов 
	isviParam.samplingRate=10000;			//!< Частота дескритизации [Гц]
	isviParam.bytesPerSamples=2;		//!< Число байт на отсчёт
	isviParam.samplesPerBytes=1;		//!< Число отсчётов на байт
	isviParam.isComplesSignal=0;		//!< 1 - комплексный сигнал
	//isviParam.shiftFrequncy[128];		//!< Центральная частота сигнала для каждого канала
	//isviParam.gains[128];				//!< Коэффициенты усиления каналов
	//isviParam.voltageOffset[128];		//!< Смещение нуля
	isviParam.voltageRange=1;			//!< Диапазон АЦП
	isviParam.bitRange=16;				//!< Число бит в отсчёте

	m_pSharedIsvi->SetParam( &isviParam );














//	ZeroMemory( m_BufCnt, m_CountBufCnt * 4 * 4 );


	SetDefault();
	GetParamFromFile( fname );
	CalculateParams();


	//isFirstCallStep=true;

	rd0.testBuf.buf_check_start( 32, 64 );


	rd0.time_start=0;




}

TF_TestMain::~TF_TestMain()
{
	pBrd->StreamDestroy( rd0.Strm );

	delete m_pSharedIsvi;
}

int TF_TestMain::Prepare( U32 nStep  )
{

	if( nStep )
		return 1;

	BRDC_printf( _BRDC( "%s - подготовка\n"), m_TestName );

	m_RowNumber = m_pTemplateEngine -> AddRowTable();


	PrepareAdm();

	rd0.trd=trdNo;
	rd0.Strm = strmNo;

	
	pBrd->StreamInit( rd0.Strm, CntBuffer, SizeBuferOfBytes, rd0.trd, 1, isCycle, isSystem, isAgreeMode );

	BRDC_printf( _BRDC( "\n%s - подготовка завершена\n__________________________________\n\n" ), m_TestName );


	return 1;
}

//! Запуск обмена
void TF_TestMain::Start( void )
{
	

	pBrd->StreamStart( rd0.Strm );

	rd0.time_start=rd0.time_last=IPC_getTickCount();

	pBrd->RegPokeInd( trdNo, 0, 0x0038 );

}


void TF_TestMain::Stop( void )
{
	pBrd->RegPokeInd( rd0.trd, 0, 0 );

}

void TF_TestMain::StepTable( void )
{
	
        //static int step_cnt=0;
 	

	rd0.testBuf.check_result( &rd0.BlockOk , &rd0.BlockError, NULL, NULL, NULL );


	U32 trd=rd0.trd;
	U32 status = pBrd->RegPeekDir( trd, 0 ) & 0xFFFF;
	
	U32 cnt_rd=rd0.BlockRd;


	{
		char tname[256];
		sprintf( tname, "TRD%d", rd0.trd );
        m_pTemplateEngine->SetValueTable( m_RowNumber, 0, "%s", tname );
        m_pTemplateEngine->SetValueTable( m_RowNumber, 2, "%d", cnt_rd );
        m_pTemplateEngine->SetValueTable( m_RowNumber, 3, "%d", rd0.BlockOk );
        m_pTemplateEngine->SetValueTable( m_RowNumber, 4, "%d", rd0.BlockError );
        m_pTemplateEngine->SetValueTable( m_RowNumber, 5, "%10.1f", rd0.VelocityCurrent );
        m_pTemplateEngine->SetValueTable( m_RowNumber, 6, "%10.1f", rd0.VelocityAvarage );

        m_pTemplateEngine->SetValueTable( m_RowNumber, 7, " 0x%04X", status );


		float t0=0;
		float t1;
		float t2;
		U32 stat=0;
		//GetSysmon( stat, t0 );
		GetSysmonMain( t1 );
		GetSysmonDDR(  t2 );

		sprintf( tname, "%.2X  %.1f %.1f %.1f", stat, t0, t1, t2 );

        m_pTemplateEngine->SetValueTable( m_RowNumber, 8, "%s", tname );

	}




/*
		if( isSysmon )
		{
			U32 stat;
			float temp;
			char tname[256];

			this->GetSysmon( stat, temp );
			m_pTemplateEngine->SetValueTable( m_RowNumberSysmon, 5,  temp, " %4.1f" );
			m_pTemplateEngine->SetValueTable( m_RowNumberSysmon, 6,  stat, "0x%.2X" );

			sprintf( tname, "SYSMON"  );
			m_pTemplateEngine->SetValueTable( m_RowNumberSysmon, 0,  tname );
			m_pTemplateEngine->SetValueTable( m_RowNumberSysmon, 3,  "" );
			m_pTemplateEngine->SetValueTable( m_RowNumberSysmon, 4,  "" );
			m_pTemplateEngine->SetValueTable( m_RowNumberSysmon, 7,  "" );
			m_pTemplateEngine->SetValueTable( m_RowNumberSysmon, 8,  "" );
		}
*/

/*
	U32 hour, min, sec;
	U32 _min, _sec;

	_sec = (currentTime - rd0.time_start) / 1000;
	sec = _sec%60;
	_min = _sec/60;
	min = _min %60;
	hour = _min / 60;

	double _size = rd0.BlockRd;
	_size *= SizeBlockOfBytes;
	_size /=1024; // kB
	_size /=1024; // Mb
	_size /=1024; // Gb
*/	
	
}

int TF_TestMain::isComplete( void )
{

	return m_isComplete;
}

void TF_TestMain::GetResult( void )
{


	double velocity = rd0.VelocityAvarage;
	velocity *= 8;
	velocity /= 1024;
	
	BRDC_printf( _BRDC( "\n%s - результат проверки\n"), m_TestName );

	if(rd0.BlockRd!=0 && rd0.BlockError!=0)
	{
		BRDC_printf( _BRDC("\n\nПроверка завершена. \n") );

		//BRDC_printf( _BRDC("Скорость передачи: %d Мбит/с  ( %10.1f Мбайт/с ) \n"), velocity, rd0.VelocityAvarage );
		//BRDC_printf( _BRDC("Скорость передачи: %10.4g Гбит/с  ( %10.1f Мбайт/с ) \n"), velocity, rd0.VelocityAvarage );

		BRDC_printf( _BRDC("\n\nЧисло ошибок: %d\n"), rd0.TotalError );

		switch( isTest )
		{
			case 6:	// 128

					BRDC_printf( _BRDC("\nСписок ошибок 0: \n") );
                    BRDC_printf(_BRDC("%s\n"), rd0.testBuf.report_word_error());

					BRDC_printf( _BRDC("\nСписок ошибок 1: \n") );
                    BRDC_printf(_BRDC("%s\n"), rd0.testBuf.report_word_error());

					break;


			case 7:
				{
					BRDC_printf( _BRDC("\nСписок ошибок 0: \n") );
                    BRDC_printf(_BRDC("%s\n"), rd0.testBuf.report_word_error());

					BRDC_printf( _BRDC("\nСписок ошибок 1: \n") );
                    BRDC_printf(_BRDC("%s\n"), rd0.testBuf.report_word_error());

					BRDC_printf( _BRDC("\nСписок ошибок 2: \n") );
                    BRDC_printf(_BRDC("%s\n"), rd0.testBuf.report_word_error());

					BRDC_printf( _BRDC("\nСписок ошибок 3: \n") );
                    BRDC_printf(_BRDC("%s\n"), rd0.testBuf.report_word_error());


				};
				break;
			default:
				BRDC_printf( _BRDC("\nСписок ошибок: \n") ); // rd0.TotalError 
                BRDC_printf(_BRDC("%s\n"), rd0.testBuf.report_word_error());
				break;


		}


		BRDC_printf( _BRDC("\n Press any key\n") );
	} else if( rd0.BlockRd==0 )
	{
		BRDC_printf( _BRDC("\n\nПроверка завершена.  Данные не приняты \n") );

	}	else
	{
		BRDC_printf( _BRDC("\n\nПроверка завершена.  Ошибок нет \n") );

		//BRDC_printf( _BRDC("Скорость передачи: %d Мбит/с  ( %10.1f Мбайт/с ) \n"), velocity, rd0.VelocityAvarage );
		//BRDC_printf( _BRDC("Скорость передачи: %10.4g Гбит/с  ( %10.1f Мбайт/с ) \n"), velocity, rd0.VelocityAvarage );
	}



	BRDC_printf( _BRDC( "__________________________________\n\n" ) );

	

}



//! Установка параметров по умолчанию
void TF_TestMain::SetDefault( void )
{
	int ii=0;

	array_cfg[ii++]=STR_CFG(  0, "isShowParam",			"0", (U32*)&isShowParam, "1 - отображение параметров" );

	array_cfg[ii++]=STR_CFG(  0, "CntBuffer",			"16", (U32*)&CntBuffer, "число буферов стрима" );
	array_cfg[ii++]=STR_CFG(  0, "CntBlockInBuffer",	"512",  (U32*)&CntBlockInBuffer, "Число блоков в буфере" );
	array_cfg[ii++]=STR_CFG(  0, "SizeBlockOfWords",	"2048",  (U32*)&SizeBlockOfWords, "Размер блока в словах" );
	array_cfg[ii++]=STR_CFG(  0, "isCycle",				"1",  (U32*)&isCycle, "1 - Циклический режим работы стрима" );
	//array_cfg[ii++]=STR_CFG(  0, "isRestart",			"0",  (U32*)&isRestart, "1 - Перезапуск АЦП" );
	array_cfg[ii++]=STR_CFG(  0, "isSystem",			"0",  (U32*)&isSystem, "1 - выделение системной памяти" );
	array_cfg[ii++]=STR_CFG(  0, "isAgreeMode",			"0",  (U32*)&isAgreeMode, "1 - согласованный режим" );

	array_cfg[ii++]=STR_CFG(  0, "trdNo",	"4",  (U32*)&trdNo, "Номер тетрады" );
	array_cfg[ii++]=STR_CFG(  0, "strmNo",	"0",  (U32*)&strmNo, "Номер стрма" );
	array_cfg[ii++]=STR_CFG(  0, "isSdram",	"0",  (U32*)&isSdram, "1 - тетрада SDRAM" );

	array_cfg[ii++]=STR_CFG(  0, "SdramSource",	"0",  (U32*)&SdramSource, "Источник данных для SDRAM: 0 - АЦП, 1 - FDSP" );
	array_cfg[ii++]=STR_CFG(  0, "SdramTestSequence",	"0",  (U32*)&SdramTestSequence, "0x100 - включение 256-ти разрядной тестовой последовательности на входе SDRAM" );
	array_cfg[ii++]=STR_CFG(  0, "SdramFullSpeed",	"0",  (U32*)&SdramFullSpeed, "1 - включение работы на полной скорости, требует установки SdramTestSequence=0x100" );



	array_cfg[ii++]=STR_CFG(  0, "SdramFifoMode",	"0",  (U32*)&SdramFifoMode, "1 - режим FIFO" );
	array_cfg[ii++]=STR_CFG(  0, "SdramFifoOutRestart",	"0",  (U32*)&SdramFifoOutRestart, " 1 - перезапуск только выходного FIFO тетрады TRD_DDR3x" );
	array_cfg[ii++]=STR_CFG(  0, "SdramAzBase",	"0",  (U32*)&SdramAzBase, "Начало активной зоны. Конец активной зоны определяется по размеру буфера стрима" );

	


	
	array_cfg[ii++]=STR_CFG(  0, "isDDC",	"0",  (U32*)&isDDC, "1 - DDC, 0 - ADC" );
	array_cfg[ii++]=STR_CFG(  0, "isTest",	"0",  (U32*)&isTest, "0 - нет, 1 - проверка псевдослучайной последовательности, 2 - проверка тестовой последовательности" );
	array_cfg[ii++]=STR_CFG(  0, "isMainTest",	"0",  (U32*)&isMainTest, "1 - включение режима тестирования в тетраде MAIN" );




	array_cfg[ii++]=STR_CFG(  0, "isSysmon",	"0",  (U32*)&isSysmon, "1 - чтение регистров системного монитора на ПЛИС ADM" );



	max_item=ii;

   {
	char str[1024];
    for( unsigned ii=0; ii<max_item; ii++ )
	{
		sprintf( str, "%s  %s", array_cfg[ii].name, array_cfg[ii].def );
		GetParamFromStr( str );
	}


   }

}

//! Расчёт параметров
void TF_TestMain::CalculateParams( void )
{
	SizeBlockOfBytes = SizeBlockOfWords * 4;						// Размер блока в байтах
	SizeBuferOfBytes	= CntBlockInBuffer * SizeBlockOfBytes  ;	// Размер буфера в байтах
	SizeStreamOfBytes	= CntBuffer * SizeBuferOfBytes;				// Общий размер буфера стрима

	ShowParam();
}

//! Отображение параметров
void TF_TestMain::ShowParam( void )
{

	if( 1==isShowParam )
	{
		TF_WorkParam::ShowParam();

		BRDC_printf( _BRDC("Общий размер буфера стрима: %d МБ\n\n"), SizeStreamOfBytes/(1024*1024) );
	}

}

#if 0
U32 TF_TestMain::Execute( void )
{
	rd0.testBuf.buf_check_start( 32, 64 );

	
	if( isWriteFile )
		BufWriteStart();



	U32 trd=m_trdN411;
	
	//pBrd->RegPokeInd( 3, 0x0000, 0x0000 );
	//pBrd->RegPokeInd( 9, 0x0000, 0x0000 );
	pBrd->RegPokeInd( m_trdDioOut, 0x0000, 0x0000 );

	pBrd->RegPokeInd( trd, 0, 0x2012 );
	pBrd->RegPokeInd( rd0.trd, 0, 0x2012 );
	//pBrd->RegPokeInd( 8, 0, 0x2012 );
	Sleep( 1 );
	pBrd->RegPokeInd( trd, 0, 0x2010 );
	pBrd->RegPokeInd( rd0.trd, 0, 0x2010 );
	//pBrd->RegPokeInd( 8, 0, 0x2010 );
	Sleep( 1 );


	pBrd->RegPokeInd( trd, 0, 0x2012 );
	pBrd->RegPokeInd( rd0.trd, 0, 0x2012 );
	//pBrd->RegPokeInd( 8, 0, 0x2012 );
	Sleep( 1 );
	pBrd->RegPokeInd( trd, 0, 0x2010 );
	pBrd->RegPokeInd( rd0.trd, 0, 0x2010 );
	//pBrd->RegPokeInd( 8, 0, 0x2010 );
	Sleep( 1 );




	pBrd->StreamStart( rd0.Strm );
	pBrd->RegPokeInd( rd0.trd, 0, 0x2038 );
	

	if( isPrepareNIITP411 )
	{
		pBrd->RegPokeInd( m_trdN411, 0, 0x2038 );

		if( m_TypeOut>0 )
		{
			pBrd->RegPokeInd( m_trdDioOut, 0x0000, 0x00A0 );
			Sleep( 100 );
		}
				
		// Сброс блока детектора сдвига фазы в канале
		pBrd->RegPokeInd( m_trdN411, 0x200, 4 );
		Sleep(10);
		pBrd->RegPokeInd( m_trdN411, 0x200, 0 );
		Sleep(100);
		// Включение формирователя потока и блока разборщика потока
		pBrd->RegPokeInd( m_trdN411, 0x200, 3 );
	}


	if( isAdmReg2 )
		PrepareAdmReg( fnameAdmReg2, 0 );

	//{
	//	U32 trd2_mode0= pBrd->RegPeekInd( 2, 0 );
	//	U32 trd2_reg200= pBrd->RegPeekInd( 2, 0x200 );
	//	U32 trd3_reg200= pBrd->RegPeekInd( 3, 0x200 );
	//		
	//	U32 trd8_mode0= pBrd->RegPeekInd( 8, 0 );
	//	U32 trd8_reg200= pBrd->RegPeekInd( 8, 0x200 );
	//	U32 trd9_reg200= pBrd->RegPeekInd( 9, 0x200 );

	//	BRDC_printf( _BRDC("\n\n\n\nTRD2 MODE0= 0x%.4X REG200=%.4X REG200x=0x%.4X\nTRD8 MODE0= 0x%.4X REG200=%.4X REG200x=0x%.4X\n"), 
	//		trd2_mode0, trd2_reg200, trd3_reg200, trd8_mode0, trd8_reg200, trd9_reg200 );
	//}


	flagFirstBlock=0;
	rd0.time_last=rd0.time_start=GetTickCount();

	sdram0.time_last=sdram0.time_start=rd0.time_start;
	sdram1.time_last=sdram1.time_start=rd0.time_start;
	mgt.time_last=mgt.time_start=rd0.time_start;		
	niitp.time_last = niitp.time_start = rd0.time_start;

	U32 current_time;
	for( ; ; )
	{
		if( Terminate )
		{
			//printf( "\nTerminate=%d\n", Terminate );
			break;
		}

		ReceiveData( &rd0 );
		//current_time = GetTickCount();
		//CalculateVelocity( &sdram0, current_time, 10000, 2 );
		//CalculateVelocity( &sdram1, current_time, 10000, 2 );
		//CalculateVelocity( &mgt, current_time, 10000, 1 );




		//Sleep( 100 );
	}

	if( isWriteFile )
		BufWriteStop();

	pBrd->RegPokeInd( rd0.trd, 0, 0 );
	if( isTestCtrl )
	{
		pBrd->RegPokeInd( 1, 0x1E, 0 );
		pBrd->RegPokeInd( 1, 0x1C, 0 );
	}


	pBrd->StreamStop( rd0.Strm );
	Sleep( 10 );

	pBrd->RegPokeInd( rd0.trd, 0, 2 );
	Sleep( 10 );

	pBrd->RegPokeInd( trd, TRDIND_LC_FLAG, 0 ); 
	Sleep( 1400 );

	//pBrd->RegPokeInd( trd, TRDIND_CONTROL1, 0 ); 

	pBrd->RegPokeInd( trd, TRDIND_SPD_DEVICE, 0 );	 
	

	lc_status=4;
	Sleep( 100 );
	return 1;
}
#endif


void TF_TestMain::StepThread( void )
{
	ReceiveData();
}


void TF_TestMain::ReceiveData( void )
{


	if( 0 != m_isComplete )
		return;


	ParamExchange *pr = &rd0;
	U32 *ptr;
	U32 *ptrBlock;
	U32 mode=0;

	mode |= pr->DataType<<8;
	mode |= pr->DataFix<<7;

	int ret;
	int kk;


	for( kk=0; kk<16; kk++ )
	{

		ret=pBrd->StreamGetBuf( pr->Strm, &ptr );
		if( ret )
		{ // Проверка буфера стрима

			//m_ptrCurrentBlock = ptr;

			if( (6==isTest) || (7==isTest) )
				pr->testBuf.chn_start();

			//SetEvent( m_EventCheckStart );

			if( 0==flagFirstBlock )
			{
				currentTime=rd0.time_last=rd0.time_start=IPC_getTickCount();
				flagFirstBlock=1;
			}



            for( unsigned ii=0; ii<CntBlockInBuffer; ii++ )
			{
				ptrBlock=ptr+ii*SizeBlockOfWords;

				m_pSharedIsvi->PutData( ptrBlock, SizeBlockOfBytes );

				if( 1==isTest )
					pr->testBuf.buf_check_psd( ptrBlock, SizeBlockOfWords );
					//int a=0;
				//else if( 2==isTest )
				//	pr->testBuf.buf_check( ptrBlock, pr->BlockRd, SizeBlockOfWords, BlockMode );
				else if( 6==isTest )
					pr->testBuf.buf_check_psd128_low( ptrBlock, SizeBlockOfWords );
				else if( 7==isTest )
				{
					pr->testBuf.buf_check_psd256( ptrBlock, SizeBlockOfWords, 0 );
					pr->testBuf.buf_check_psd256( ptrBlock, SizeBlockOfWords, 1 );
				}
				pr->BlockRd++;
			}

			//for( ; ; )
			//{
			//	ret=WaitForSingleObject( m_EventCheckResult, 200 );
			//	//if( Terminate )
			//	//{
			//	//	break;
			//	//}
			//	if( WAIT_OBJECT_0==ret )
			//	{
			//		break;
			//	}
			//}

			if( (6==isTest) || (7==isTest) )
				pr->testBuf.chn_step();

			if( isAgreeMode )
			{
				pBrd->StreamGetBufDone( pr->Strm );
			}


		} else
		{
			break;
		}
	}
	IPC_delay( 0 );

	
	U32 currentTime = IPC_getTickCount();
	if( (currentTime - pr->time_last)>4000 )
	{
		float t1 = currentTime - pr->time_last;
		float t2 = currentTime - pr->time_start;
		float v = 1000.0*(pr->BlockRd-pr->BlockLast)*SizeBlockOfBytes/t1;
		v/=1024*1024;
		pr->VelocityCurrent=v;

		v = 1000.0*(pr->BlockRd)*SizeBlockOfBytes/t2;
		v/=1024*1024;
		pr->VelocityAvarage=v;
		pr->time_last = currentTime;
		pr->BlockLast = pr->BlockRd;
		pr->freeCycleZ=pr->freeCycle;
		pr->freeCycle=0;
	
	}
	

}


void TF_TestMain::PrepareAdm( void )
{
	U32 trd=trdNo;
	U32 ii;
	U32 id, id_mod, ver;


	if( isMainTest )
		PrepareMain();

	if( 1==isSdram )
		PrepareDDR3( trdNo );




}

//! Подготовка MAIN
void TF_TestMain::PrepareMain( void )
{

	BRDC_printf( _BRDC("В тетраде MAIN установлен режим формирования псевдослучайной последовательности\n") );
	pBrd->RegPokeInd( 0, 12, 1 );  // Регистр TEST_MODE[0]=1 - режим формирования псевдослучайной последовательности
	pBrd->RegPokeInd( 0, 0, 2 );   // Сброс FIFO - перевод в начальное состояние
	IPC_delay( 1 );
	pBrd->RegPokeInd( 0, 0, 0 );
	IPC_delay( 1 );
}

#define TRD_CTRL 1

#define REG_MUX_CTRL  0x0F
#define REG_GEN_CNT1  0x1A
#define REG_GEN_CNT2  0x1B
#define REG_GEN_CTRL  0x1E
#define REG_GEN_SIZE  0x1F
#define TRD_DIO_IN    6
#define TRD_CTRL      1



//! Подготовка DDR3
void TF_TestMain::PrepareDDR3( int trd )
{

	//-----------------------------------------------------------------------------

	U32 ii;

	//MAIN_trdNo=0;

	U32		spd_addr[]={ 4, 5, 7, 8 };
	U32		spd_data[sizeof(spd_addr)/sizeof(U32)];
	U32		confreg;

	U32		DDR2_trdNo = trd;

	pBrd->RegPokeInd(DDR2_trdNo, 0x0, 0x3);	// MODE0 
	pBrd->RegPokeInd(DDR2_trdNo, 0x0, 0x0);	// MODE0 


	for(ii=0;ii<sizeof(spd_addr)/sizeof(U32);ii++)
	{
		pBrd->RegPokeInd(DDR2_trdNo, TRDIND_SPD_ADDR, spd_addr[ii]);
		pBrd->RegPokeInd(DDR2_trdNo, TRDIND_SPD_CTRL, 2);
		spd_data[ii]=pBrd->RegPeekInd(DDR2_trdNo, TRDIND_SPD_DATA );
	}
	//----------------------------------------------------------------------------
	{
		U32 status;
		U32 mode1=0;
		U32 columns;
		U32 rows;
		U32 banks;
		U32 ranks;
		U32 size;
		U32 primary_bus;
		U32 sdram_widht;

		switch( (spd_data[0]>>4) & 0x7 )
		{
			case 0: banks = 3; break;
			case 1: banks = 4; break;
			case 2: banks = 5; break;
			case 3: banks = 6; break;
			default: banks=0; break;
		}
		
		switch(spd_data[0] & 0xF )
		{
			case 0:	size=256; break;
			case 1:	size=512; break;
			case 2:	size=1024; break;
			case 3:	size=2048; break;
			case 4:	size=4096; break;
			case 5:	size=8192; break;
			case 6:	size=16384; break;
			default: size=0;

		}

		switch( (spd_data[1]>>3) & 0x7 )
		{
			case 0: rows=12; break;
			case 1: rows=13; break;
			case 2: rows=14; break;
			case 3: rows=15; break;
			case 4: rows=16; break;
			default: rows=0; break;
		}
	
		switch( (spd_data[1] & 0x7 ) )
		{
			case 0: columns=9; break;
			case 1: columns=10; break;
			case 2: columns=11; break;
			case 3: columns=12; break;
			default: columns=0; break;
		}
	
	
		switch( (spd_data[2]>>3) & 0x7 )
		{
			case 0: ranks=1; break;
			case 1: ranks=2; break;
			case 2: ranks=3; break;
			case 3: ranks=4; break;
			default: ranks=0; break;
		}
		
		switch( (spd_data[2]) & 0x7 )
		{
			case 0: sdram_widht=4; break;
			case 1: sdram_widht=8; break;
			case 2: sdram_widht=16; break;
			case 3: sdram_widht=32; break;
			default: ranks=0; break;
		}
		switch( (spd_data[3]) & 0x7 )
		{
			case 0: primary_bus=8; break;
			case 1: primary_bus=16; break;
			case 2: primary_bus=32; break;
			case 3: primary_bus=64; break;
			default: primary_bus=0; break;
		}

					mode1=((rows-8)<<2)&0x1C;
					mode1|=	((columns-8)<<5) & 0xE0;
					mode1|= ((ranks-1)<<8) ;
					mode1|= (1<<9);
					 mode1|=0x2000;

		            pBrd->RegPokeInd(DDR2_trdNo, TRDIND_MODE1, mode1);	// CONF_REG
		//
		//            do
		//            {
		//                    status=pBrd->RegPeekDir(DDR2_trdNo, 0);
		//            }while(!(status & 0xC000));


		 //BRDC_printf(_BRDC("\nMODE1 :\t\t0x%X\n\n"), mode1);

	}
	//----------------------------------------------------------------------------

	



		{
			U32 mode3=0;
			if( SdramSource )
				mode3|=1;
			if( SdramFullSpeed )
				mode3|=0x20;

			if( SdramFifoOutRestart )
				mode3|=0x100;

			pBrd->RegPokeInd( DDR2_trdNo, 0x0B, mode3 );    // управление источником и режимом полной скорости
		}

		
		pBrd->RegPokeInd( DDR2_trdNo, 0x0C, SdramTestSequence  );   // 0x100 - включение тестового режима
/*
		SdramSplit = 1;
		//SdramSplitMask = 0x300;
		SdramSplitMask = m_pParam->splitMask;
		SdramSplitAdr  = m_pParam->splitRxAdr;

		//SdramSplitMask = 0x3FF;
		//switch( m_ChnNumber )
		//{
		//	case 0:	SdramSplitAdr = 0x000; break;
		//	default:
		//	case 1: SdramSplitAdr = 0x040; break;
		//	//case 0:	SdramSplitAdr = 0x000; break;
		//	//default:
		//	//case 1: SdramSplitAdr = 0x200; break;
		//}


		if( SdramSplit )
		{
			if( m_isShowRowTrdDdr )
			{
				BRDC_printf( _BRDC( "\nВключён режим разделения памяти:\n" ) );
				BRDC_printf( _BRDC( " SdramSplitMask  = 0x%.4X \n" ), SdramSplitMask );
				BRDC_printf( _BRDC( " SdramSplitAdr   = 0x%.4X \n\n" ), SdramSplitAdr );
			}

			pBrd->RegPokeInd( DDR2_trdNo, 0x16, SdramSplitMask );
			pBrd->RegPokeInd( DDR2_trdNo, 0x17, SdramSplitAdr );

		}
*/


	//-----------------------------------------------------------------------------
	 BRDC_printf( _BRDC("Waiting for Memory Initialization  ... \r\n") );
	IPC_delay( 200 );
	{
		U32 status;
		U32 loop=0;
		do
		{
			status=pBrd->RegPeekDir(DDR2_trdNo, 0);
			//break;
			
			BRDC_printf( _BRDC("%10d STATUS: %.4X \r"), loop, status ); IPC_delay( 300 ); loop++;
		}while( !(status & 0x800));
	}
	 BRDC_printf( _BRDC("\r\nMemory Initialization DONE\r\n") );
	{




		if( 1==SdramFifoMode )
		{ // Режим FIFO 
			pBrd->RegPokeInd(DDR2_trdNo, 0x10, 0 );
			pBrd->RegPokeInd(DDR2_trdNo, 0x11, 0 );

			pBrd->RegPokeInd(DDR2_trdNo, 0x0E, 0 );
			pBrd->RegPokeInd(DDR2_trdNo, 0x0F, 0 );

			pBrd->RegPokeInd(DDR2_trdNo, 0x14, 0 );
			pBrd->RegPokeInd(DDR2_trdNo, 0x15, 0 );

			//pBrd->RegPokeInd(DDR2_trdNo, TRDIND_MODE2, 0x10);
			pBrd->RegPokeInd(DDR2_trdNo, TRDIND_MODE2, 0x14 ); // Режим FIFO
			BRDC_printf( _BRDC("\r\nРежим FIFO\r\n")  );
		} else
		{ // Режим с автоматическим чтением

			// Начало активной зоны
			pBrd->RegPokeInd(DDR2_trdNo, 0x10, SdramAzBase & 0xFFFF );
			pBrd->RegPokeInd(DDR2_trdNo, 0x11, SdramAzBase >> 16 );

			U32 size = CntBlockInBuffer * SizeBlockOfWords * CntBuffer; // общий размер буфера в 32-х разрядных словах
			U32 AzEnd = SdramAzBase + size - 1;

			{
				BRDC_printf( _BRDC( "\r\nРежим с автоматическим чтением\r\n" ) );
				BRDC_printf( _BRDC( "\r\nНачало активной зоны: 0x%.8X\r\n" ), SdramAzBase );
				BRDC_printf( _BRDC( "\r\nКонец  активной зоны: 0x%.8X\r\n" ), AzEnd );
				BRDC_printf( _BRDC( "\r\nРазмер активной зоны: 0x%.8X [MB]\r\n\n" ), size / ( 256 * 1024 ) );
			}
			
			pBrd->RegPokeInd(DDR2_trdNo, 0x0E, AzEnd & 0xFFFF );
			pBrd->RegPokeInd(DDR2_trdNo, 0x0F, AzEnd >> 16 );

		}
	

		pBrd->RegPokeInd( DDR2_trdNo, TRDIND_MODE0, 2 );  // Сброс FIFO

		if( SdramFifoOutRestart )
		{
			U32 mode3=0;
			mode3=pBrd->RegPeekInd( DDR2_trdNo, 0x0B );    
			pBrd->RegPokeInd( DDR2_trdNo, 0x0B, mode3 | 0x200 );    // сброс входного FIFO
			pBrd->RegPokeInd( DDR2_trdNo, 0x0B, mode3 );    
		}

		pBrd->RegPokeInd( DDR2_trdNo, TRDIND_MODE0, 0 );  

	}


}

//! Запись регистров из файла
void TF_TestMain::PrepareAdmReg( char* fname, int showparam )
{
	if( showparam ) 
	{
		BRDC_printf( _BRDC("\nУстановка регистров из файла ") );
		printf( "%s \n\n", fname );
	}

	FILE *in = fopen( fname, "rt" );
	if( in==NULL )
	{
		throw( "Ошибка доступа к файлу " );
	}

	char str[256];
	U32 trd, reg, val;
	int ret;
	for( ; ; )
	{
		if( fgets( str, 200, in )==NULL )
			break;

		if( str[0]==';' )
			continue;

		ret=sscanf( str, "%i %i %i", &trd, &reg, &val );
		if( 3==ret )
		{
			if( showparam ) 
			{
				BRDC_printf( _BRDC("  TRD: %d  REG[0x%.2X]=0x%.4X \n"), trd, reg, val );
			}
			pBrd->RegPokeInd( trd, reg, val );
		}

	}
	if( showparam )
		BRDC_printf( _BRDC("\n\n") );
}






#if 0
//! Подготовка службы АЦП
void TF_TestMain::PrepareAdc( char* fname )
{
	U32 atr;

   BRDC_printf( _BRDC("\nФайл инициализации АЦП: ") );
   printf( "%s \n", fname );
   char srvName[210];
   char srvName1[210];
   
   srvName[0]=0;

	{
	  // Определение имени службы АЦП
	  FILE *in = fopen( fname, "rt" );
	  if( NULL==in )
	  {
		  throw( _BRDC("Ошибка при открытии файла") );
	  }
	  char str[512];
	  for( ; ; )
	  {
		  if( fgets( str, 200, in )==NULL )
			  break;
		  if( strncmp( "AdcServiceName=", str, 15 )==0 )
		  {
			sscanf( str+15, "%s", srvName );
			break;
		  }

	  }
	  sprintf( srvName1, "%s0", srvName );

	}


#ifdef UNICODE
	
BRDCHAR _srvName[512];
BRDCHAR _srvName1[512];

	 MultiByteToWideChar(
			CP_ACP,			//	  __in   UINT CodePage,
			MB_PRECOMPOSED,	//	  __in   DWORD dwFlags,
			srvName,			//	  __in   LPCSTR lpMultiByteStr,
			-1,				//	  __in   int cbMultiByte,
			_srvName,			//	 __out  LPWSTR lpWideCharStr,
			512				//	__in   int cchWideChar
	);

	 MultiByteToWideChar(
			CP_ACP,			//	  __in   UINT CodePage,
			MB_PRECOMPOSED,	//	  __in   DWORD dwFlags,
			srvName1,			//	  __in   LPCSTR lpMultiByteStr,
			-1,				//	  __in   int cbMultiByte,
			_srvName1,			//	 __out  LPWSTR lpWideCharStr,
			512				//	__in   int cchWideChar
	);

	m_hAdc=pBrd->GetSrvHandle( _srvName1, &atr );
#else
	m_hAdc=pBrd->GetSrvHandle( srvName1, &atr );
#endif

	BRDC_printf( _BRDC("\n#####Служба АЦП: %s %.8X \n"), srvName1, m_hAdc );

	//m_hAdc=pBrd->GetSrvHandle( _BRDC("FM214X250M0"), &atr );

	if( !m_hAdc )
	{
	   throw( _BRDC("Служба АЦП не найдена ") );
	}

   BRDC_printf( _BRDC("\nСлужба АЦП: %.8X \n"), m_hAdc );

   if( NULL==fname )
   {
	   throw( _BRDC( "Не задано имя файла инициализации АЦП " ));
   }


   //AdcSettings( m_hAdc, 0, _BRDC("FM214X250M0"), fname );
   //AdcSettings( m_hAdc, 0, _BRDC("FM214X250M0"), _BRDC("\\exam_adc.ini") );
   
   char fname1[512];
   sprintf( fname1, "\\%s", fname );

#ifdef UNICODE
	
BRDCHAR _fname[512];

	 MultiByteToWideChar(
			CP_ACP,			//	  __in   UINT CodePage,
			MB_PRECOMPOSED,	//	  __in   DWORD dwFlags,
			fname1,			//	  __in   LPCSTR lpMultiByteStr,
			-1,				//	  __in   int cbMultiByte,
			_fname,			//	 __out  LPWSTR lpWideCharStr,
			512				//	__in   int cchWideChar
	);

	AdcSettings( m_hAdc, 0, _srvName, _fname );
#else
	
	AdcSettings( m_hAdc, 0, srvName, fname1 );
   //AdcSettings( m_hAdc, 0, srvName, "\\exam_adc.ini" );
#endif

   
}
#endif

//! Расчёт скорости приёма
void TF_TestMain::CalculateVelocity( ParamExchange* pr, U32 currentTime, U32 interval, U32 scale )
{

	U32 size_block = 1048576; // размер блока 1 Мбайт

	if( (currentTime - pr->time_last)>interval )
	{
		float t1 = currentTime - pr->time_last;
		float t2 = currentTime - pr->time_start;
		float v = 1000.0*(pr->BlockRd-pr->BlockLast)*size_block/t1;
		v/=1024*1024;
		pr->VelocityCurrent=v*scale;

		v = 1000.0*(pr->BlockRd)*size_block/t2;
		v/=1024*1024;
		pr->VelocityAvarage=v*scale;
		pr->time_last = currentTime;
		pr->BlockLast = pr->BlockRd;
	
	}
}

//! Получение текущей температуры ПЛИС
void TF_TestMain::GetSysmon( U32& stat, float& temp )
{
	U32 code;
        //float tx;

	pBrd->RegPokeInd( 0, 0x210, 0 );
	stat=pBrd->RegPeekInd( 0, 0x212 );
	code=pBrd->RegPeekInd( 0, 0x211 );

    code >>=6;
    code &= 0x3FF;
    temp = (code*503.975)/1024-273.15;

}



//! Получение текущей температуры ПЛИС - из тетрады MAIN, регистр 0x209
void TF_TestMain::GetSysmonMain( float& temp )
{
	U32 code;
        //float tx;

	code=pBrd->RegPeekInd( 0, 0x209 );

    code >>=6;
    code &= 0x3FF;
    temp = (code*503.975)/1024-273.15;

}

//! Получение текущей температуры ПЛИС - из тетрады DDR, регистр 0x207
void TF_TestMain::GetSysmonDDR( float& temp )
{
	U32 code;
        //float tx;

	code=pBrd->RegPeekInd( 5, 0x207 );

    code >>=6;
    code &= 0x3FF;
    temp = (code*503.975)/1024-273.15;

}
