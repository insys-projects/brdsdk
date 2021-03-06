#ifdef _FM412X500M

#include	"CtrlFm412x500m.h"

FM412X500MSRV_MU mu_dflt = {
	sizeof(FM412X500MSRV_MU),
	0,							// тип субмодуля: 0 - всегда
	0x1,						// маска выбранных каналов
	1,							// режим синхронизации (2 - Master, 1 - Single, 0 - Slave)
	250000000.0,				// частота дискретизации (Гц)
	100000000.0,				// значение частоты выбранного источника тактирования (Гц) (для источников 2, 4, 0x81, 0x82)
	1,							// источник тактовой частоты: 0-выкл., 1-внутренний, 2-внутренний+PLL, 4-от базмодуля, 0x81-внешний, 0x82-внешний+PLL
	0,							// 0 - 16 бит, 1 - 8 бит
	0,							// включение режима претриггера
	0,							// включение режима внешнего претриггера
	0,							// включение режима гарантированного претриггера
	129,						// число отсчётов претриггера
	0,							// счетчик начальной задержки
	0,							// счетчик принимаемых данных
	0,							// счетчик пропущенных данных
	0,							// разрешение счетчика начальной задержки
	0,							// разрешение счетчика принимаемых данных
	0,							// разрешение счетчика пропущенных данных
	0,							// базовый источник старта: 0 - программный, 7 - от субмодуля, 10 - от DDS базового модуля
	0,							// 1 - инверсия базового источника старта
	0,							// базовый источник останова
	0,							// 1 - инверсия базового источника останова
	0,							// 1 - режим триггерного старта-останова, 0 - режим потенциального старта-останова
	0,							// 1 - режим сбора с автоматическим перезапуском
	3,							// субмодульный источник старта: 0 - канал 0, 1 - канал 1, 2 - внешний, 3 - программный
	0,							// 1 - инверсия субмодульного источника старта
	0.123,						// уровень старта в вольтах (от -2.5 В до +2.5 В – при внешнем старте)
	0,							// вх.сопротивление для сигнала внешнего старта: 0 - 2.5 кОм, 1 - 50 Ом
};

#endif // _FM412X500M

#ifdef _FM214X250M

#include	"CtrlFm214x250m.h"

FM214X250MSRV_MU mu_dflt = {
	sizeof(FM214X250MSRV_MU),
	0,							// тип субмодуля: 0 - стандарт, 1 - WB HF, 2 - WB LF
	0x1,						// маска выбранных каналов
	1,							// режим синхронизации (2 - Master, 1 - Single, 0 - Slave)
	250000000.0,				// частота дискретизации (Гц) (возможные значения делителя = 1, 2, 4, 8, 16, 32, 64, 128, 256)
	100000000.0,				// значение частоты выбранного источника тактирования (Гц) (для источников 2, 4, 0x81, 0x82)
	2,							// источник тактовой частоты: 0-выкл., 1-внутренний, 2-внутренний+PLL, 4-от базмодуля, 0x81-внешний, 0x82-внешний+PLL
	0.50, 0.50,					//входной диапазон для каждого канала (10.0 Вольт, 2.0 Вольт, 0.5 Вольт, 0.1 Вольт)
	0.360, -0.054,				// смещение нуля для каждого канала
	1, 1,						// входное сопротивление для каждого канала (0 - 1 МOм, 1 - 50 Oм)
	1, 1,						// открытость (1) / закрытость (0) входа по постоянной составляющей для каждого канала
	0,							// 0 - 16 бит, 1 - 8 бит
	0,							// включение режима претриггера
	0,							// включение режима внешнего претриггера
	0,							// включение режима гарантированного претриггера
	129,						// число отсчётов претриггера
	0,							// счетчик начальной задержки
	0,							// счетчик принимаемых данных
	0,							// счетчик пропущенных данных
	0,							// разрешение счетчика начальной задержки
	0,							// разрешение счетчика принимаемых данных
	0,							// разрешение счетчика пропущенных данных
	0,							// базовый источник старта: 0 - программный, 7 - от субмодуля, 10 - от DDS базового модуля
	0,							// 1 - инверсия базового источника старта
	0,							// базовый источник останова
	0,							// 1 - инверсия базового источника останова
	0,							// 1 - режим триггерного старта-останова, 0 - режим потенциального старта-останова
	0,							// 1 - режим сбора с автоматическим перезапуском
	3,							// субмодульный источник старта: 0 - канал 0, 1 - канал 1, 2 - внешний, 3 - программный
	0,							// 1 - инверсия субмодульного источника старта
	0.123,						// уровень старта в вольтах (от -2.5 В до +2.5 В – при внешнем старте)
	0,							// вх.сопротивление для сигнала внешнего старта: 0 - 2.5 кОм, 1 - 50 Ом
	0.0, 0.0,					// подстройка усиления каждого канала (от -2% до +2%)
	0,							// 1 - инверсия сигнала тактирования
};

#endif // _FM214X250M

#ifdef _FM814X125M

#include	"CtrlFm814x125m.h"

FM814X125MSRV_MU mu_dflt = {
	sizeof(FM814X125MSRV_MU),
	0x1,						// маска выбранных каналов
	1,							// режим синхронизации (2 - Master, 1 - Single, 0 - Slave) всегда 1
	50000000.0,					// частота дискретизации (Гц): 10 МГц .. 125 МГц
	100000000.0,				// значение частоты выбранного внешнего источника (Гц)
	1,							// источник тактовой частоты (0 - выключен, 1 - генератор на субмодуле, 0x81 - внешний)
	0.50, 0.50, 0.50, 0.50,		// шкала преобразования для каждого канала (Вольт): 4.0, 2.0, 1.0, 0.5, 0.25
	0.50, 0.50, 0.50, 0.50,		// шкала преобразования для каждого канала (Вольт): 4.0, 2.0, 1.0, 0.5, 0.25
	0.0, 0.0,					// смещение нуля для каждого канала (Вольт): -2*ШП .. +2*ШП
	0.0, 0.0,					// смещение нуля для каждого канала (Вольт): -2*ШП .. +2*ШП
	0.0, 0.0,					// смещение нуля для каждого канала (Вольт): -2*ШП .. +2*ШП
	0.0, 0.0,					// смещение нуля для каждого канала (Вольт): -2*ШП .. +2*ШП
	1, 1, 1, 1, 1, 1, 1, 1,		// входное сопротивление для каждого канала (0 - 1 МOм, 1 - 50 Oм)
	1, 1, 1, 1, 1, 1, 1, 1,		// открытость (1) / закрытость (0) входа по постоянной составляющей для каждого канала
	0,							// 0 - 16 бит, 1 - 8 бит
	3,							// субмодульный источник старта: 0 - канал 0, 1 - канал 1, 3 - программный
	0.0,						// уровень старта в вольтах (от -2.5 В до +2.5 В – при внешнем старте)
	0.0,						// порог компаратора внешней тактовой частоты (В): -2.5 .. +2.5
};

#endif // _FM814X125M

#ifdef _ADM214X200M

#include	"CtrlAdc214x200m.h"

ADC214X200MSRV_MU mu_dflt = {
	sizeof(ADC214X200MSRV_MU),
	0x1,						// маска выбранных каналов
	1,							// режим синхронизации (2 - Master, 1 - Single, 0 - Slave)
	50000000.0,					// частота дискретизации (Гц) (возможные значения делителя = 1, 2, 4, 8, 16, 32, 64, 128, 256)
	100000000.0,				// значение частота выбранного внешнего источника (Гц)
	2,							// источник тактовой частоты (0 - выключен, 1 - генератор на субмодуле (ver 1.1+), 2 - синтезатор на субмодуле, 0x81 - внешний)
	0.50, 0.50,					//входной диапазон для каждого канала (10.0 Вольт, 2.0 Вольт, 0.5 Вольт, 0.1 Вольт)
	0.360, -0.054,					// смещение нуля для каждого канала
	1, 1,						// входное сопротивление для каждого канала (0 - 1 МOм, 1 - 50 Oм)
	1, 1,						// открытость (1) / закрытость (0) входа по постоянной составляющей для каждого канала
	0,							// 0 - 16 бит, 1 - 8 бит
	0,							// включение режима претриггера
	0,							// включение режима внешнего претриггера
	0,							// включение режима гарантированного претриггера
	129,						// число отсчётов претриггера
	0,							// счетчик начальной задержки
	0,							// счетчик принимаемых данных
	0,							// счетчик пропущенных данных
	0,							// разрешение счетчика начальной задержки
	0,							// разрешение счетчика принимаемых данных
	0,							// разрешение счетчика пропущенных данных
	0,							// базовый источник старта: 0 - программный, 7 - от субмодуля, 10 - от DDS (only for AMBPEX8)
	0,							// 1 - инверсия базового источника старта
	0,							// базовый источник останова
	0,							// 1 - инверсия базового источника останова
	0,							// 1 - режим триггерного старта-останова, 0 - режим потенциального старта-останова
	0,							// 1 - режим сбора с автоматическим перезапуском
	3,							// субмодульный источник старта: 0 - канал 0, 1 - канал 1, 2 - внешний, 3 - программный
	0,							// 1 - инверсия субмодульного источника старта
	0.123,						// уровень старта в вольтах (от -2.5 В до +2.5 В – при внешнем старте)
	0.0, 0.0,					// подстройка усиления каждого канала (от -2% до +2%)
	0,							// 1 - инверсия сигнала тактирования
};

#endif // _ADM214X200M

#ifdef _ADM214X400M

#include	"CtrlAdc214x400m.h"

ADC214X400MSRV_MU mu_dflt = {
	sizeof(ADC214X400MSRV_MU),
	0,							// тип субмодуля: 0 - стандарт, 1 - WB HF, 2 - WB LF
	0x3,						// маска выбранных каналов
	1,							// режим синхронизации (2 - Master, 1 - Single, 0 - Slave)
	50000000.0,					// частота дискретизации (Гц) (возможные значения делителя = 1, 2, 4, 8, 16, 32, 64, 128, 256)
	100000000.0,				// значение частота выбранного внешнего источника (Гц)
	1,							// источник тактовой частоты (0 - выключен, 1 - генератор на субмодуле (ver 1.1+), 2 - синтезатор на субмодуле, 0x81 - внешний)
	0.50, 0.50,					//входной диапазон для каждого канала (10.0 Вольт, 2.0 Вольт, 0.5 Вольт, 0.1 Вольт)
	0.360, -0.054,					// смещение нуля для каждого канала
	1, 1,						// входное сопротивление для каждого канала (0 - 1 МOм, 1 - 50 Oм)
	1, 1,						// открытость (1) / закрытость (0) входа по постоянной составляющей для каждого канала
	0,							// 0 - 16 бит, 1 - 8 бит
	0,							// включение режима претриггера
	0,							// включение режима внешнего претриггера
	0,							// включение режима гарантированного претриггера
	129,						// число отсчётов претриггера
	0,							// счетчик начальной задержки
	0,							// счетчик принимаемых данных
	0,							// счетчик пропущенных данных
	0,							// разрешение счетчика начальной задержки
	0,							// разрешение счетчика принимаемых данных
	0,							// разрешение счетчика пропущенных данных
	0,							// базовый источник старта: 0 - программный, 7 - от субмодуля, 10 - от DDS (only for AMBPEX8)
	0,							// 1 - инверсия базового источника старта
	0,							// базовый источник останова
	0,							// 1 - инверсия базового источника останова
	0,							// 1 - режим триггерного старта-останова, 0 - режим потенциального старта-останова
	0,							// 1 - режим сбора с автоматическим перезапуском
	3,							// субмодульный источник старта: 0 - канал 0, 1 - канал 1, 2 - внешний, 3 - программный
	0,							// 1 - инверсия субмодульного источника старта
	0.123,						// уровень старта в вольтах (от -2.5 В до +2.5 В – при внешнем старте)
	0.0, 0.0,					// подстройка усиления каждого канала (от -2% до +2%)
	0,							// 1 - инверсия сигнала тактирования
};

#endif // _ADM214X400M

#ifdef _ADM216X100M

#include	"CtrlAdc216x100m.h"

ADC216X100MSRV_MU mu_dflt = {
	sizeof(ADC216X100MSRV_MU),
	0x1,						// маска выбранных каналов
	1,							// режим синхронизации (2 - Master, 1 - Single, 0 - Slave)
	50000000.0,					// частота дискретизации (Гц) (возможные значения делителя = 1, 2, 4, 8, 16)
	100000000.0,				// значение частота выбранного внешнего источника (Гц)
	1,							// источник тактовой частоты (0 - выключен, 1,2,3 - генератор 1,2,3 на субмодуле, 0x81 - внешний)
	2.0, 2.0,					//входной диапазон для каждого канала (4.0 Вольт, 2.66 Вольт, 2.0 Вольт, 1.33 Вольт, 0.2 Вольт, 0.133 Вольт, 0.1 Вольт, 0.066 Вольт)
	0.360, -0.054,				// смещение нуля для каждого канала
	1, 1,						// входное сопротивление для каждого канала (0 - 1 kOм, 1 - 50 Oм)
	1, 1,						// открытость (1) / закрытость (0) входа по постоянной составляющей для каждого канала
	0,							// 0 - 16 бит, 1 - 8 бит
	0,							// включение режима претриггера
	0,							// включение режима внешнего претриггера
	0,							// включение режима гарантированного претриггера
	129,						// число отсчётов претриггера
	0,							// счетчик начальной задержки
	0,							// счетчик принимаемых данных
	0,							// счетчик пропущенных данных
	0,							// разрешение счетчика начальной задержки
	0,							// разрешение счетчика принимаемых данных
	0,							// разрешение счетчика пропущенных данных
	0,							// базовый источник старта: 0 - программный, 2 - SDX (компаратор 1), 10 - от DDS (only for AMBPEX8)
	0,							// 1 - инверсия базового источника старта
	0,							// базовый источник останова
	0,							// 1 - инверсия базового источника останова
	0,							// 1 - режим триггерного старта-останова, 0 - режим потенциального старта-останова
	0,							// 1 - режим сбора с автоматическим перезапуском
	0.0,						// порог тактового сигнала (от -2.5 вольт до 2.5 вольт)
	0,							// 1 - включение розового шума
};

#endif // _ADM216X100M

