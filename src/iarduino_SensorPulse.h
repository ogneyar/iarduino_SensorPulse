//	Библиотека для работы с датчиком пульса. http://iarduino.ru/shop/Sensory-Datchiki/datchik-pulsa.html
//  Версия: 1.0.3
//  Последнюю версию библиотеки Вы можете скачать по ссылке: http://iarduino.ru/file/261.html
//  Подробное описание функции бибилиотеки доступно по ссылке: http://lesson.iarduino.ru/page/urok-27-pulsometr/
//  Библиотека является собственностью интернет магазина iarduino.ru и может свободно использоваться и распространяться!
//  При публикации устройств или скетчей с использованием данной библиотеки, как целиком, так и её частей,
//  в том числе и в некоммерческих целях, просим Вас опубликовать ссылку: http://iarduino.ru
//  Автор библиотеки: Панькин Павел
//  Если у Вас возникли технические вопросы, напишите нам: shop@iarduino.ru

#ifndef iarduino_SensorPulse_h
#define iarduino_SensorPulse_h

#if defined(ARDUINO) && (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#if defined(ESP32)												//
	static hw_timer_t *Esp32Timer = timerBegin(2, 3, true);		//	Определяем структуру настройки 2 таймера, предделитель = 3 (потом его изменим), флаг = true - счёт вперёд.
	extern void timer_callback_ESP32(void);						//	Подключаем функцию обработки прерываний 2 таймера для плат ESP32.
#elif defined(ESP8266)											//
	extern void timer_callback_ESP8266(void);					//	Подключаем функцию обработки прерываний 1 таймера для плат ESP8266.
#elif defined(RENESAS_CORTEX_M4)								//
	#include <FspTimer.h>										//	Подключаем библиотеку управления таймерами для плат Arduino UNO R4.
	static FspTimer objTimer;									//	Создаём объект для работы с таймером.
	extern void timer_callback_R4(timer_callback_args_t*);		//	Подключаем функцию обработки прерываний таймера для плат Arduino UNO R4.
#elif defined(MCU_MIK32_Amur)
	#include "mik32_hal_timer16.h"
	#include "mik32_hal_irq.h"
	#include "wiring_LL.h"
	void interrupt_handler_16_2(void);
#endif															//
																//
#if defined(ESP32)												//
	#define	ISP_ADC_RESOLUTION		4096						//	Разрешение АЦП.
	#define	ISP_ADC_LIMIT_MAX		3600						//	Максимально допустимое значение АЦП. Значения выше считаются за пределами допустимых.
	#define	ISP_ADC_LIMIT_MIN		200							//	Минимально  допустимое значение АЦП. Значения ниже считаются за пределами допустимых.
	#define	ISP_ADC_FLUCTUATION		80							//	Значение флуктуации (шума) АЦП. Чем выше значение, тем лучше подавляются шумы, но теряется чувствительность.
#else															//
	#define	ISP_ADC_RESOLUTION		1024						//	Разрешение АЦП.
	#define	ISP_ADC_LIMIT_MAX		900							//	Максимально допустимое значение АЦП. Значения выше считаются за пределами допустимых.
	#define	ISP_ADC_LIMIT_MIN		50							//	Минимально  допустимое значение АЦП. Значения ниже считаются за пределами допустимых.
	#define	ISP_ADC_FLUCTUATION		20							//	Значение флуктуации (шума) АЦП. Чем выше значение, тем лучше подавляются шумы, но теряется чувствительность.
#endif															//
																//
#define	ISP_ANALOG					0							//	если эту константу указать в качестве аргумента функции check, то последняя вернёт - текущее значение с аналогового входа к которому подключён сенсор
#define	ISP_BEEP					1							//	если эту константу указать в качестве аргумента функции check, то последняя вернёт - десятые доли секунды прошедшие с последнего пульса
#define	ISP_PULSE					2							//	если эту константу указать в качестве аргумента функции check, то последняя вернёт - среднее значение пульса (количество ударов в минуту)
#define	ISP_VALID					3							//	если эту константу указать в качестве аргумента функции check, то последняя вернёт - состояние сенсора
																//
#define	ISP_DISCONNECTED			0							//	состояние сенсора - отключён
#define	ISP_CONNECTED				1							//	состояние сенсора - подключён
#define	ISP_CHANGED					2							//	состояние сенсора - изменилось
																//
class iarduino_SensorPulse{										//
	public:			iarduino_SensorPulse(uint8_t, uint8_t=255);	//	указываем номер аналогового входа к которому подключён сенсор [, вывод биппера (звукоизлучателя с генератором)].
	/**	пользовательские функции **/							//
		void		begin				();						//	инициализация счётчика пульса	(без параметров)
		uint16_t	check				(uint8_t);				//	проверка результата				(что проверять)
	private:													//
	/**	внутренние функции **/									//
		#include	"iarduino_SensorPulse_Timer.h"				//	Подключаем функцию конфигурирования таймера Timer_Begin( частота Гц ).
		uint16_t	ISP_func_CheckBeep	();						//	проверка времени прошедшего после вершины пульса
		uint16_t	ISP_func_CheckPulse	();						//	проверка значения пульса
		uint8_t		ISP_func_CheckValid	();						//	проверка валидности результата
	/**	внутренние переменные **/								//
};																//
																//
class iarduino_SensorPulse_VolatileVariableClass{				//
	public:														//
	/**	переменные изменяемые в прерываниях **/					//
		volatile	bool			ISP_flag_VAL;				//	флаг указывающий о том, что показания сенсора валидны
		volatile	uint8_t			ISP_pins_SEN;				//	номер аналогового входа, к которому подключён сенсор пульса
		volatile	uint8_t			ISP_pins_BEP;				//	номер цифрового  вывода, к которому подключён биппер
		volatile	bool			ISP_data_BEP;				//	состояние на выводе, к которому подключён биппер
		volatile	bool			ISP_flag_BEP;				//	флаг указывающий о том, что биппер подключён
		volatile	uint16_t		ISP_data_PIN[10];			//	10 последних значений, прочитанных с аналогового входа, к которому подключён сенсор пульса
		volatile	uint16_t		ISP_data_MAX;				//  максимальное значение сенсора за текущие 2 секунды
		volatile	uint16_t		ISP_data_MIN;				//  минимальное  значение сенсора за текущие 2 секунды
		volatile	uint16_t		ISP_data_CEN;				//  2/3 части от минимального до максимального значения за предыдущие 2 секунды
		volatile	uint16_t		ISP_time_CNT;				//	счетчик прерываний 2 таймера
		volatile	uint8_t			ISP_data_TOP;				//	количество точек графика находящихся возле вершины пульса
		volatile	bool			ISP_flag_TOP;				//	флаг указывающий о том, что более 7 точек графика находятся возле вершины пульса
		volatile	uint16_t		ISP_time_TOP;				//	время прошедшее после того, как более 7 точек графика вошли в вершину пульса (в мс/4)
		volatile	uint16_t		ISP_time_ARR[5];			//	промежутки времени между 6 последними пульсами
		volatile	unsigned long	ISP_time_NOW;				//	время фиксации последнего     пульса
		volatile	unsigned long	ISP_time_WAS;				//	время фиксации предпоследнего пульса
};																//

#endif