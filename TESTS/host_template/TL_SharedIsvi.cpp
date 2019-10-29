#include "TL_SharedIsvi.h"


TL_SharedIsvi::TL_SharedIsvi( const char *name, U32 maxSizeMb )
{
	m_isFirst = 0;
	m_MaxSizeMb = maxSizeMb;
	m_MaxSizeBytes = m_MaxSizeMb * 1024 * 1024;

	BRDCHAR nameBufMap[64];

    BRDC_sprintf( nameBufMap, _BRDC( "%s_blk0" ), name );
	//BRDC_sprintf(nameBufMap, _BRDC("data_bin"));

	U32 size = m_MaxSizeMb * 1024 * 1024;

	BRDC_printf( _BRDC( "\nОбмен с ISVI через разделяемую память \n" ) );
	BRDC_printf( _BRDC( "  Имя файла:    %s\n" ), nameBufMap );
	BRDC_printf( _BRDC( "  Размер файла: %d [Мбайт]\n\n" ), m_MaxSizeMb );
	
	hBufFileMap = IPC_createSharedMemory(nameBufMap, size);

// 	hBufFileMap = CreateFileMapping( INVALID_HANDLE_VALUE,
// 		NULL, PAGE_READWRITE,
// 		0, size, 
// 		nameBufMap );


	if( NULL == hBufFileMap )
		throw("Error on create data_bin");

	//bufIsvi = (U32 *)MapViewOfFile( hBufFileMap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0 );
	bufIsvi = (U32*)IPC_mapSharedMemory(hBufFileMap);


	BRDCHAR nameFlagMap[128];
    BRDC_sprintf( nameFlagMap, _BRDC( "%s_flg" ), name );

	hFlgFileMap = IPC_createSharedMemory(nameFlagMap, 3 * sizeof(ULONG));

// 	hFlgFileMap = CreateFileMapping( INVALID_HANDLE_VALUE,
// 		NULL, PAGE_READWRITE,
// 		0, 3 * sizeof(ULONG),
// 		nameFlagMap );

	if( NULL == hFlgFileMap )
		throw("Error on create data_flg");

	pFlags = (ULONG *)IPC_mapSharedMemory(hFlgFileMap);
	//pFlags = (ULONG*)MapViewOfFile( hFlgFileMap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0 );

	pFlags[0] = 0;
	pFlags[1] = 0;
	pFlags[2] = 0;

	BRDCHAR namePostfixMap[128];
    BRDC_sprintf( namePostfixMap, _BRDC( "%s_postfix" ), name );

	hPostfixFileMap = IPC_createSharedMemory(namePostfixMap, 4096);

// 	hPostfixFileMap = CreateFileMapping( INVALID_HANDLE_VALUE,
// 		NULL, PAGE_READWRITE,
// 		0, 4096,
// 		namePostfixMap );

	if( NULL == hPostfixFileMap )
		throw("Error on create data_adc_postfix");

	pPostfix = (char *)IPC_mapSharedMemory(hPostfixFileMap);

	//pPostfix = (char*)MapViewOfFile( hPostfixFileMap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0 );

	pPostfix[0] = 0;
}


TL_SharedIsvi::~TL_SharedIsvi()
{
	IPC_unmapSharedMemory(bufIsvi);
	IPC_deleteSharedMemory(hBufFileMap);

// 	UnmapViewOfFile( bufIsvi );
// 	CloseHandle( hBufFileMap );

	IPC_unmapSharedMemory(pFlags);
	IPC_deleteSharedMemory(hFlgFileMap);

// 	UnmapViewOfFile( pFlags );
// 	CloseHandle( hFlgFileMap );

	IPC_unmapSharedMemory(pPostfix);
	IPC_deleteSharedMemory(hPostfixFileMap);

// 	UnmapViewOfFile( pPostfix );
// 	CloseHandle( hPostfixFileMap );
}

void TL_SharedIsvi::SetParam( IsviParam *param )
{
	m_Param = *param;

	char * g_fileHeader = pPostfix;
	char g_str[256];

	U32 nchans = m_Param.numberOfChannels;
	U32 nsamples = m_Param.numberOfSamples;
	U32 sample_rate = m_Param.samplingRate;
	U32 bit_range = m_Param.bitRange;
	U32 bytes_per_sample = m_Param.bytesPerSamples;
    U32 ii;

	U32 size = nsamples * nchans * bytes_per_sample;
	if( size > m_MaxSizeBytes )
	{
		nsamples = m_MaxSizeBytes / ( nchans * bytes_per_sample );
	}
	

	g_fileHeader[0] = '\0';
	strcat( g_fileHeader, "\r\nDEVICE_NAME_________ ADC" );
    sprintf( g_str, "\r\nNUMBER_OF_CHANNELS__ %d", nchans );
	strcat( g_fileHeader, g_str );
	sprintf( g_str, "\r\nNUMBERS_OF_CHANNELS_ " );
	strcat( g_fileHeader, g_str );
	for( ii = 0; ii < nchans; ii++ )
	{
		sprintf( g_str, "%d,", ii );
		strcat( g_fileHeader, g_str );
	}

	sprintf( g_str, "\r\nNUMBER_OF_SAMPLES___ %d", nsamples );
	strcat( g_fileHeader, g_str );
	sprintf( g_str, "\r\nSAMPLING_RATE_______ %d", sample_rate );
	strcat( g_fileHeader, g_str );
	sprintf( g_str, "\r\nBYTES_PER_SAMPLES___ %d", bytes_per_sample );
	strcat( g_fileHeader, g_str );
	//strcat( g_fileHeader, "\r\nBYTES_PER_SAMPLES___ 2" );
	strcat( g_fileHeader, "\r\nSAMPLES_PER_BYTES___ 1" );
	sprintf( g_str, "\r\nIS_COMPLEX_SIGNAL?__ NO" );
	strcat( g_fileHeader, g_str );
	sprintf( g_str, "\r\nSHIFT_FREQUENCY_____ 0.0,0.0,0.0,0.0" );
	strcat( g_fileHeader, g_str );
	sprintf( g_str, "\r\nGAINS_______________ 1.0,1.0,1.0,1.0" );
	strcat( g_fileHeader, g_str );
	strcat( g_fileHeader, "\r\nVOLTAGE_OFFSETS_____ 0.0,0.0,0.0,0.0, " );
	strcat( g_fileHeader, "\r\nVOLTAGE_RANGE_______ 0.5" );

	sprintf( g_str, "\r\nBIT_RANGE___________ %d\r\n", bit_range );
	strcat( g_fileHeader, g_str );

	IsviHeaderLen = strlen( g_fileHeader );

	m_isFirst = -1;

}

void TL_SharedIsvi::MapWrFlagSinc( int flg, int isNewParam )
{
	pFlags[0] = flg;
	pFlags[1] = isNewParam;
}

int TL_SharedIsvi::MapRdFlagSinc( void )
{
	int flg = pFlags[0];
	return flg;
}

U32 TL_SharedIsvi::isReady( void )
{
	int ret = 0;
	int rr = MapRdFlagSinc();
	if( 0 == rr )
		ret = 1;

	return ret;

}

void TL_SharedIsvi::PutData( U32 *ptr, U32 sizeOfBytes )
{
    U32 size = sizeOfBytes;
	if( size > m_MaxSizeBytes )
		size = m_MaxSizeBytes;

	memcpy( bufIsvi, ptr, size );

	MapWrFlagSinc( 0xFFFFFFFF, m_isFirst );
	m_isFirst = 0;
}

void TL_SharedIsvi::LauncheIsvi( void )
{

}
