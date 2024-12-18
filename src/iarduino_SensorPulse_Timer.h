//  Данный файл содержит функцию Timer_Begin( частота Гц ), обращение к которой инициирует работу таймера на вызов функции Timer_Callback(Timer_Argument) с указанной частотой.
//	Данный файл подключается в блоке private основного класса библиотеки, значит функция Timer_Begin( частота Гц ) становится функцией класса.
//	О том как подключить данный файл к ВАШЕЙ библиотеке, рассказано ниже...
//  Код данного файла является собственностью интернет магазина iarduino.ru и может свободно использоваться и распространяться!
//  При публикации устройств или скетчей с использованием данного кода, как целиком, так и его частей,
//  в том числе и в некоммерческих целях, просим Вас опубликовать ссылку: http://iarduino.ru
//  Автор кода: Панькин Павел
//  Если у Вас возникли технические вопросы, напишите нам: shop@iarduino.ru
//
//	Для плат Arduino UNO R3, NANO, MINI, MEGA, MOCRO, LEONARDO и т.д. используется 2 аппаратный таймер.
//	Для плат Arduino UNO R4 используется свободный аппаратный таймер.
//	Для плат ESP32 используется 2 аппаратный таймер.
//	Для плат ESP8266 используется 1 аппаратный таймер.
//
//	------------------------------------------------------------------------------------------- //
//	ПОДКЛЮЧЕНИЕ:																				//
//																								//
//	В начале файла *.h ВАШЕЙ библиотеки необходимо вставить разкомментированный код:			//
/**																								//
#if defined(ESP32)																				//
	static hw_timer_t *Esp32Timer = timerBegin(2, 3, true);										//	Определяем структуру настройки 2 таймера, предделитель = 3 (потом его изменим), флаг = true - счёт вперёд.
	extern void timer_callback_ESP32(void);														//	Подключаем функцию обработки прерываний 2 таймера для плат ESP32.
#elif defined(ESP8266)																			//
	extern void timer_callback_ESP8266(void);													//	Подключаем функцию обработки прерываний 1 таймера для плат ESP8266.
#elif defined(RENESAS_CORTEX_M4)																//
	#include <FspTimer.h>																		//	Подключаем библиотеку управления таймерами для плат Arduino UNO R4.
	static FspTimer objTimer;																	//	Создаём объект для работы с таймером.
	extern void timer_callback_R4(timer_callback_args_t*);										//	Подключаем функцию обработки прерываний таймера для плат Arduino UNO R4.
#endif																							//
**/																								//
																								//
//	В файле *.h нужно подключить данный файл в блоке public или private основного класса:		//
/**																								//
	private:																					//
	#include "название_этого_файла.h"															//	Подключаем функцию конфигурирования таймера Timer_Begin( частота Гц ).
**/																								//
																								//
//	В файле *.cpp ВАШЕЙ библиотеки необходимо создать функцию которую будет вызывать таймер:	//
/**																								//	ОБРАТИТЕ ВНИМАНИЕ: не нужно указывать тип функции и её аргумента, это сделано для совместимости с ISR().
Timer_Callback(Timer_Argument){																	//	Функция будет вызываться автоматически по прерываниям таймера.
	любые требуемые действия																	//	Частота прерываний таймера указывается в функции Timer_Begin( частота Гц ).
}																								//
**/																								//
																								//
//	В любом месте библиотеки необходимо инициировать таймер указав частоту прерываний таймера:	//
/**																								//
тип класс::begin(параметры){																	//	В этом примере обращение к функции Timer_Begin() выполняется из функции begin() основного класса.
	Timer_Begin(ЧАСТОТА);																		//	Конфигурируем таймер на вызов функции Timer_Callback(Timer_Argument) с указанной частотой.
}																								//	ЧАСТОТА - целое положительное в разумных пределах.
**/																								//
																								//
//	------------------------------------------------------------------------------------------- //
//																								//
//	Если используется плата на базе микроконтроллера ATmega32U4:								//
#if defined(__AVR_ATmega32U4__)																	//	Arduino Micro или Arduino Leonardo.
//	Если Arduino IDE не знает, что у этих микроконтроллеров есть второй аппаратный таймер:		//
	#if !defined(TCCR2A)																		//
	//	Удивляем Arduino IDE сообщая ей информацию о наличии второго таймера:					//
		#define TCCR2A					(*(uint8_t*)0xB0)										//	Определяем смещение для регистра TCCR2A - регистр выбора режима.
		#define COM2A1					7														//	Определяем позицию  в регистре   TCCR2A для старшего бита выбора режима сравнения канала А.
		#define COM2A0					6														//	Определяем позицию  в регистре   TCCR2A для младшего бита выбора режима сравнения канала А.
		#define COM2B1					5														//	Определяем позицию  в регистре   TCCR2A для старшего бита выбора режима сравнения канала B.
		#define COM2B0					4														//	Определяем позицию  в регистре   TCCR2A для младшего бита выбора режима сравнения канала B.
		#define WGM21					1														//	Определяем позицию  в регистре   TCCR2A для 1 бита выбора режима работы таймера.
		#define WGM20					0														//	Определяем позицию  в регистре   TCCR2A для 0 бита выбора режима работы таймера.
		#define TCCR2B					(*(uint8_t*)0xB1)										//	Определяем смещение для регистра TCCR2B - рагистр делителя.
		#define FOC2A					7														//	Определяем позицию  в регистре   TCCR2B для бита принудительной установки результата сравнения канала A.
		#define FOC2B					6														//	Определяем позицию  в регистре   TCCR2B для бита принудительной установки результата сравнения канала B.
		#define WGM22					3														//	Определяем позицию  в регистре   TCCR2B для 2 бита выбора режима работы таймера.
		#define CS22					2														//	Определяем позицию  в регистре   TCCR2B для старшего бита выбора значения предделителя.
		#define CS21					1														//	Определяем позицию  в регистре   TCCR2B для среднего бита выбора значения предделителя.
		#define CS20					0														//	Определяем позицию  в регистре   TCCR2B для младшего бита выбора значения предделителя.
		#define TCNT2					(*(uint8_t*)0xB2)										//	Определяем смещение для регистра TCNT2  - счётный регистр.
		#define OCR2A					(*(uint8_t*)0xB3)										//	Определяем смещение для регистра OCR2A  - регистр сравнения канала A.
		#define OCR2B					(*(uint8_t*)0xB4)										//	Определяем смещение для регистра OCR2B  - регистр сравнения канала B.
		#define TIFR2					(*(uint8_t*)0x37)										//	Определяем смещение для регистра TIFR2  - регистр флагов.
		#define OCF2B					2														//	Определяем позицию  в регистре   TIFR2  для флага совпадения значения счетного регистра TCNT2 с регистром сравнения OCR2A.
		#define OCF2A					1														//	Определяем позицию  в регистре   TIFR2  для флага совпадения значения счетного регистра TCNT2 с регистром сравнения OCR2B.
		#define TOV2					0														//	Определяем позицию  в регистре   TIFR2  для флага переполнения счетного регистра TCNT2.
		#define TIMSK2					(*(uint8_t*)0x70)										//	Определяем смещение для регистра TIMSK2 - регистр разрешения прерываний.
		#define OCIE2B					2														//	Определяем позицию  в регистре   TIMSK2 для бита разрешения прерываний по установке флага OCF2B.
		#define OCIE2A					1														//	Определяем позицию  в регистре   TIMSK2 для бита разрешения прерываний по установке флага OCF2A.
		#define TOIE2					0														//	Определяем позицию  в регистре   TIMSK2 для бита разрешения прерываний по установке флага TOF2.
		#define TIMER2_COMPA_vect_num	13														//	Определяем номер  вектора  для  события установки флага OCF2A.
		#define TIMER2_COMPA_vect		_VECTOR(13)												//	Определяем вектор прерываний по событию установки флага OCF2A.
		#define TIMER2_COMPB_vect_num	14														//	Определяем номер  вектора  для  события установки флага OCF2B.
		#define TIMER2_COMPB_vect		_VECTOR(14)												//	Определяем вектор прерываний по событию установки флага OCF2B.
		#define TIMER2_OVF_vect_num		15														//	Определяем номер  вектора  для  события установки флага TOV2.
		#define TIMER2_OVF_vect			_VECTOR(15)												//	Определяем вектор прерываний по событию установки флага TOV2.
	#endif																						//
#endif																							//
																								//
//	ПРЕРЫВАНИЯ ОБРАБАТЫВАЮТСЯ ФУНКЦИЕЙ Timer_Callback(Timer_Argument) В ФАЙЛЕ "*.CPP":			//	Но название этой функции будет изменено препроцессором.
#if defined(__AVR__)																			//
	#define Timer_Callback				ISR														//	функция  Timer_Callback станет функцией   ISR.
	#define Timer_Argument				TIMER2_COMPA_vect										//	Аргумент Timer_Argument станет аргументом TIMER2_COMPA_vect.
#elif defined(ESP32)																			//
	#define Timer_Callback				void timer_callback_ESP32								//	функция  Timer_Callback станет функцией   timer_callback_ESP32.
	#define Timer_Argument				void													//	Аргумент Timer_Argument станет аргументом void.
#elif defined(ESP8266)																			//
	#define Timer_Callback				void timer_callback_ESP8266								//	функция  Timer_Callback станет функцией   timer_callback_ESP8266.
	#define Timer_Argument				void													//	Аргумент Timer_Argument станет аргументом void.
#elif defined(RENESAS_CORTEX_M4)																//
	#define Timer_Callback				void timer_callback_R4									//	функция  Timer_Callback станет функцией   timer_callback_R4.
	#define Timer_Argument				timer_callback_args_t*timer_argument					//	Аргумент Timer_Argument станет аргументом timer_callback_args_t* timer_argument.
#elif defined(MCU_MIK32_Amur)																	//
	#define Timer_Callback				void interrupt_handler_16_2								//	функция  Timer_Callback станет функцией   interrupt_handler_16_2.
	#define Timer_Argument				void   													//	Аргумент Timer_Argument станет аргументом void.
#endif																							//
																								//
//		КОНФИГУРАЙИЯ РЕГИСТРОВ ВТОРОГО ТАЙМЕРА НА ВЫЗОВ ПРЕРЫВАНИЙ:								//
void	Timer_Begin(uint32_t freq){																//	Параметр: частота прерываний в Гц.
			#if defined(__AVR__)																//
				uint16_t prescaler;																//	Значение предделителя: 1,8,32,64,128,256,1024. Устанавливается битами CS22-CS20 в регистре TCCR2B.
				uint8_t  CS22_CS21_CS20;														//	Значение битов CS22-CS20 в регистре TCCR2B: 001 ... 111. Устанавливает значение предделителя.
			//	Определение значения предделителя для 2 таймера:								//	                     При F_CPU = 16МГц:                                         При F_CPU = 8МГц:
				if( freq > (F_CPU/255/  1) ){ prescaler=   1; CS22_CS21_CS20=1; }else			//	Предделитель =    1: Прерывания = 62,7 кГц ... 16,0 МГц при OCR2A = 254...   0. Прерывания = 31,3 кГц ...  8,0 МГц при OCR2A = 254...   0.
				if( freq > (F_CPU/255/  8) ){ prescaler=   8; CS22_CS21_CS20=2; }else			//	Предделитель =    8: Прерывания =  7,8 кГц ... 62,5 кГц при OCR2A = 254...  30. Прерывания =  3,9 кГц ... 31,3 кГц при OCR2A = 254...  31.
				if( freq > (F_CPU/255/ 32) ){ prescaler=  32; CS22_CS21_CS20=3; }else			//	Предделитель =   32: Прерывания =  1,9 кГц ...  7,8 кГц при OCR2A = 254...  62. Прерывания =  980  Гц ...  3,9 кГц при OCR2A = 254...  64.
				if( freq > (F_CPU/255/ 64) ){ prescaler=  64; CS22_CS21_CS20=4; }else			//	Предделитель =   64: Прерывания =  980  Гц ...  1,9 кГц при OCR2A = 254... 131. Прерывания =  490  Гц ...  980  Гц при OCR2A = 254... 127.
				if( freq > (F_CPU/255/128) ){ prescaler= 128; CS22_CS21_CS20=5; }else			//	Предделитель =  128: Прерывания =  490  Гц ...  976  Гц при OCR2A = 254... 127. Прерывания =  245  Гц ...  490  Гц при OCR2A = 254... 127.
				if( freq > (F_CPU/255/256) ){ prescaler= 256; CS22_CS21_CS20=6; }else			//	Предделитель =  256: Прерывания =  245  Гц ...  488  Гц при OCR2A = 254... 127. Прерывания =  123  Гц ...  245  Гц при OCR2A = 254... 127.
											{ prescaler=1024; CS22_CS21_CS20=7; }				//	Предделитель = 1024: Прерывания =   61  Гц ...  244  Гц при OCR2A = 255...  63. Прерывания =   31  Гц ...  123  Гц при OCR2A = 255...  63.
			//	Установка регистров 2 таймера и его запуск на прерывания:						//
				TCCR2A	= 0<<COM2A1	| 0<<COM2A0	| 0<<COM2B1	| 0<<COM2B0	| 1<<WGM21	| 0<<WGM20;	//	Биты COM2... = «0» (каналы A и B таймера отключены от выводов 3 и 11), биты WGM21 и WGM20 = «10» (таймер 2 в режиме CTC).
				TCCR2B	= 0<<FOC2A	| 0<<FOC2B	| 0<<WGM22	| CS22_CS21_CS20;					//	Биты FOC2... = «0» (без принудительной установки результата сравнения), бит WGM22 = «0» (таймер 2 в режиме CTC), биты CS22, CS21, CS20 = CS22_CS21_CS20 (значение предделителя).
				OCR2A	= (uint8_t)(F_CPU/(prescaler*freq))-1;									//	Значение регистра сравнения OCR2A настраивается под частоту переполнения счётного регистра TCNT2. freq=F_CPU/(предделитель*(OCR2A+1)) => OCR2A = (F_CPU/(предделитель*freq))-1.
				TIMSK2	= 0<<OCIE2B	| 1<<OCIE2A	| 0<<TOIE2;										//	Разрешаем прерывание по совпадению счётного регистра TCNT2 и регистра сравнения OCR2A.
				SREG	= 1<<7;																	//	Устанавливаем флаг глобального разрешения прерываний в регистре SREG.
			#elif defined(ESP32)																//
			//	Определение значения предделителя для 2 таймера:								//
				uint16_t prescaler = APB_CLK_FREQ / freq / UINT16_MAX + 1;						//	Предделитель = Частота шины APB / требуемая частота / разрядность таймера + 1.
				if( prescaler<3 ){ prescaler=3; }												//	Предделитель не может быть равен 0, 1, 65535. Библиотека будет ругаться.
			//	Отключение 2 таймера (если он был включен):										//
				if( timerStarted(Esp32Timer) || timerAlarmEnabled(Esp32Timer) ){				//
					timerStop(Esp32Timer);														//	Останавливаем таймер.
					timerAlarmDisable(Esp32Timer);												//	Запрещаем прерывания.
					timerDetachInterrupt(Esp32Timer);											//	Отключаем функцию обработки прерываний.
				}																				//
			//	Установка регистров 2 таймера и его запуск на прерывания:						//
				timerSetDivider(Esp32Timer, prescaler);											//	Указываем значение предделителя.
				timerAttachInterrupt(Esp32Timer, &timer_callback_ESP32, true);					//	Определяем функцию обработки прерываний таймера.
				timerAlarmWrite(Esp32Timer, (APB_CLK_FREQ/prescaler/freq), true);				//	Определяем значение регистра сравнения.
				timerAlarmEnable(Esp32Timer);													//	Разрешаем прерывания таймера.
				timerStart(Esp32Timer);															//	Запускаем таймер (правда он и без этого запускается).
			#elif defined(ESP8266)																//
			//	Определение значения предделителя и иных настроек 1 таймера:					//
				uint8_t  prescaler = freq<10?        16 : 1;									//	Если частота ниже 10 Гц, то предделитель = 16 (точность 1/(80МГц/16)=200нс), иначе предделитель = 1 (точность 1/80МГц=12,5нс).
				uint8_t _prescaler = freq<10? TIM_DIV16 : TIM_DIV1;								//	Доступные варианты предделителя: TIM_DIV1 / TIM_DIV16 / TIM_DIV256.
				uint32_t counter   = 80000000LL / prescaler / freq;								//	Значение регистра сравнения (максимум 2^23 = 8388607 тактов).
				uint8_t  type = TIM_EDGE;														//	Срабатывание счётного регистра: TIM_EDGE по изменению / TIM_LEVEL по уровню.
				uint8_t  mode = TIM_LOOP;														//	Режим работы таймера: TIM_LOOP - перезапускается по истечении периода / TIM_SINGLE останавливается по истечении периода.
				noInterrupts();																	//	Запрещаем все прерывания.
			//	Отключение 1 таймера (если он был включен):										//
				if( timer1_enabled()     ){ timer1_disable();         }							//	Отключаем 1 таймер, если он был включён.
				if( timer1_interrupted() ){ timer1_detachInterrupt(); }							//	Отключаем функцию обработки прерываний, если она была указана.
			//	Установка регистров 1 таймера и его запуск на прерывания:						//
				timer1_isr_init();																//	Инициализируем работу с 1 таймером.
				timer1_attachInterrupt(timer_callback_ESP8266);									//	Определяем функцию обработки прерываний таймера.
				timer1_write(counter);															//	Указываем значение регистра сравнения.
				timer1_enable(_prescaler, type, mode);											//	Запускаем 1 таймер, указывая значение предделителя, тип срабатывания счёта и режим работы таймера.
				interrupts();																	//	Разрешаем прерывания.
			#elif defined(RENESAS_CORTEX_M4)													//
				uint8_t			type;															//	Тип свободного таймера: GPT_TIMER / AGT_TIMER.
				uint8_t			channel;														//	Свободный канал таймера: 0-6 для GPT таймера, 0-1 для AGT таймера.
				timer_mode_t	mode = TIMER_MODE_PERIODIC;										//	Режим работы таймера: TIMER_MODE_PERIODIC - перезапускается по истечении периода, TIMER_MODE_ONE_SHOT останавливается по истечении периода, TIMER_MODE_PWM пилообразн ШИМ, TIMER_MODE_ONE_SHOT_PULSE одиночный импульс пилообразной формы, TIMER_MODE_TRIANGLE_WAVE_SYMMETRIC_PWM симметричный ШИМ треугольной формы, TIMER_MODE_TRIANGLE_WAVE_ASYMMETRIC_PWM асимметричный ШИМ треугольной формы.
				float			perc = 50.0f;													//	Коэффициент заполнения.
			//	Отключение таймера под управлением объекта objTimer (если он был включен):		//
				if( objTimer.is_opened() ){														//
					objTimer.stop();															//
					objTimer.close();															//
					objTimer.end();																//
				}																				//
			//	Установка регистров свободеого таймера и его запуск на прерывания:				//
				channel = FspTimer::get_available_timer(type);									//	Получаем тип свободного таймера и его свободный канал.
				objTimer.begin(mode, type, channel, (float)freq, perc, timer_callback_R4);		//	Указываем параметры таймера. timer_callback_func - функция обработки прерываний.
				objTimer.setup_overflow_irq();													//	Разрешаем прерывания по переполнению.
				objTimer.open();																//	Открываем таймер.
				objTimer.start();																//	Запускаем таймер.
			#elif defined(MCU_MIK32_Amur)														//
				Timer16_HandleTypeDef htimer16;													//  Создаём handle таймера
				htimer16.Instance = TIMER16_2;													//	Заполняем данными.
				htimer16.Clock.Source = TIMER16_SOURCE_INTERNAL_SYSTEM;							//
				htimer16.CountMode = TIMER16_COUNTMODE_INTERNAL;								//
				htimer16.Clock.Prescaler = TIMER16_PRESCALER_32;								//
				htimer16.ActiveEdge = TIMER16_ACTIVEEDGE_RISING;								//
				htimer16.Preload = TIMER16_PRELOAD_AFTERWRITE;									//
				htimer16.Trigger.Source = TIMER16_TRIGGER_TIM2_GPIO2_3;							//
				htimer16.Trigger.ActiveEdge = TIMER16_TRIGGER_ACTIVEEDGE_SOFTWARE;				//
				htimer16.Trigger.TimeOut = TIMER16_TIMEOUT_DISABLE;								//
				htimer16.Filter.ExternalClock = TIMER16_FILTER_NONE;							//
				htimer16.Filter.Trigger = TIMER16_FILTER_NONE;									//
				htimer16.EncoderMode = TIMER16_ENCODER_DISABLE;									//
				htimer16.Waveform.Enable = TIMER16_WAVEFORM_GENERATION_ENABLE;					//
				htimer16.Waveform.Polarity = TIMER16_WAVEFORM_POLARITY_NONINVERTED;				//
				HAL_Timer16_Init(&htimer16);													//  Инициализируем работу с 2 таймером.
  				HAL_EPIC_MaskLevelSet(HAL_EPIC_TIMER16_2_MASK);									//
  				HAL_IRQ_EnableInterrupts();														//	Разрешаем прерывания.
  				HAL_Timer16_StartPWM_IT(&htimer16, 1000, 500);									//	Запускаем таймер.
			#else																				//
				#error Библиотека <iarduino_..._tmr> не поддерживает ваш микроконтроллер.		//
			#endif																				//
}																								//
