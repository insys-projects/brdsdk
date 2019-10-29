


#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "utypes.h"
#include "gipcy.h"

#include "CL_AMBPEX.h"





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


#define TRD_CTRL 1

#define REG_MUX_CTRL  0x0F
#define REG_GEN_CNT1  0x1A
#define REG_GEN_CNT2  0x1B
#define REG_GEN_CTRL  0x1E
#define REG_GEN_SIZE  0x1F
#define TRD_DIO_IN    6
#define TRD_CTRL      1

////! Подготовка DDR3
//void PrepareDDR3( CL_AMBPEX *pBrd, int trd, U32 SdramFifoMode, U32 SdramAzBase, U32 SdramAzSize, U32 SdramTestSequence, U32 SdramFullSpeed )
//{
//	PrepareDDR3( pBrd, trd, SdramFifoMode, U64( SdramAzBase ), U64( SdramAzSize ), SdramTestSequence, SdramFullSpeed );
//}

//! Подготовка DDR3
void PrepareDDR3( CL_AMBPEX *pBrd, int trd, U32 SdramFifoMode, U64 SdramAzBase, U64 SdramAzSize, U32 SdramTestSequence,  U32 SdramFullSpeed )
{

	//-----------------------------------------------------------------------------

	U32 ii;

	//MAIN_trdNo=0;
    fprintf(stderr, "%s() (START)\n", __FUNCTION__);

	U32		spd_addr[]={ 4, 5, 7, 8 };
	U32		spd_data[sizeof(spd_addr)/sizeof(U32)];
    //U32		confreg;

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
        //U32 status;
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


					U32 total_size = size / 8 * primary_bus / sdram_widht*ranks;
					BRDC_printf( _BRDC( "\nCOLUMNS \t\t%d\nROWS    \t\t%d\nBANKS   \t\t%d\nRANKS   \t\t%d\nSDRAM SIZE   \t\t%d \nSDRAM WITH \t\t%d\nPRIMARY BUS \t\t%d\nTOTAL SIZE \t\t%d [MB]\n" ),
						columns, rows, banks, ranks, size, sdram_widht, primary_bus, total_size );
		 //BRDC_printf(_BRDC("\nMODE1 :\t\t0x%X\n\n"), mode1);

	}
	//----------------------------------------------------------------------------

	



		{
			U32 mode3=0;
			//if( SdramSource )
			//	mode3|=1;
			if( SdramFullSpeed )
				mode3|=0x20;

			//if( SdramFifoOutRestart )
			//	mode3|=0x100;

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
     fprintf(stderr, "%s() (WAIT...)\n", __FUNCTION__);
	IPC_delay( 200 );
	{
		U32 status;
		U32 loop=0;
		do
		{
			status=pBrd->RegPeekDir(DDR2_trdNo, 0);
			//break;
			
            BRDC_printf( _BRDC("%10d STATUS: %.4X \r\n"), loop, status ); IPC_delay( 300 ); loop++;
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
			//BRDC_printf( _BRDC("\r\nРежим FIFO\r\n")  );
		} else
		{ // Режим с автоматическим чтением

			// Начало активной зоны
			pBrd->RegPokeInd(DDR2_trdNo, 0x10, SdramAzBase & 0xFFFF );
			pBrd->RegPokeInd(DDR2_trdNo, 0x11, SdramAzBase >> 16 );

			U32 size = SdramAzSize/4; // общий размер буфера в 32-х разрядных словах
			U32 AzEnd = SdramAzBase + size - 1;

			{
				//BRDC_printf( _BRDC( "\r\nРежим с автоматическим чтением\r\n" ) );
				//BRDC_printf( _BRDC( "\r\nНачало активной зоны: 0x%.8X\r\n" ), SdramAzBase );
				//BRDC_printf( _BRDC( "\r\nКонец  активной зоны: 0x%.8X\r\n" ), AzEnd );
				//BRDC_printf( _BRDC( "\r\nРазмер активной зоны: 0x%.8X [MB]\r\n\n" ), size / ( 256 * 1024 ) );
			}
			
			pBrd->RegPokeInd(DDR2_trdNo, 0x0E, AzEnd & 0xFFFF );
			pBrd->RegPokeInd(DDR2_trdNo, 0x0F, AzEnd >> 16 );

		}
	

		pBrd->RegPokeInd( DDR2_trdNo, TRDIND_MODE0, 2 );  // Сброс FIFO

		//if( SdramFifoOutRestart )
		//{
		//	U32 mode3=0;
		//	mode3=pBrd->RegPeekInd( DDR2_trdNo, 0x0B );    
		//	pBrd->RegPokeInd( DDR2_trdNo, 0x0B, mode3 | 0x200 );    // сброс входного FIFO
		//	pBrd->RegPokeInd( DDR2_trdNo, 0x0B, mode3 );    
		//}

		pBrd->RegPokeInd( DDR2_trdNo, TRDIND_MODE0, 0 );  

	}

     fprintf(stderr, "%s() (EXIT)\n", __FUNCTION__);


}
