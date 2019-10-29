#pragma once

//#include "windows.h"
#include "brd.h"

//#include "utypes.h"

#include "gipcy.h"

class TL_SharedIsvi
{


public:

	struct IsviParam
	{
		char	deviceName[128];	//!< Имя устройства
		U32		numberOfChannels;	//!< Число каналов
		U32		numbersOfChannels[128];	//!< Номера каналов
		U32		numberOfSamples;		//!< Число отсчётов 
		U32		samplingRate;			//!< Частота дескритизации [Гц]
		U32		bytesPerSamples;		//!< Число байт на отсчёт
		U32		samplesPerBytes;		//!< Число отсчётов на байт
		U32		isComplesSignal;		//!< 1 - комплексный сигнал
		float	shiftFrequncy[128];		//!< Центральная частота сигнала для каждого канала
		float	gains[128];				//!< Коэффициенты усиления каналов
		float	voltageOffset[128];		//!< Смещение нуля
		float	voltageRange;			//!< Диапазон АЦП
		U32		bitRange;				//!< Число бит в отсчёте
	};

    TL_SharedIsvi( const char *name, U32 maxSizeMb );
	~TL_SharedIsvi();

private:

	IsviParam  m_Param;


	IPC_handle hBufFileMap;
	IPC_handle hFlgFileMap;
	IPC_handle hPostfixFileMap;

	ULONG* pFlags;
	char*  pPostfix;

	//! Максимальный размер памяти в мегабайтах
	U32 m_MaxSizeMb;

	//! Максимальный размер памяти в байтах
	U32 m_MaxSizeBytes;

	void MapWrFlagSinc( int flg, int isNewParam );

	int MapRdFlagSinc( void );

	int m_isFirst;

	//! Указатель на память
	U32  *bufIsvi;

	//! Длина параметров
	U32 IsviHeaderLen;

public:

	//! Установка параметров
	void SetParam( IsviParam *param );

	//! Проверка готовности к передаче данных
	U32 isReady( void );

	//! Передача данных
	void PutData( U32 *ptr, U32 sizeOfBytes );

	//! Запуск программы ISVI
	void LauncheIsvi( void );

};

