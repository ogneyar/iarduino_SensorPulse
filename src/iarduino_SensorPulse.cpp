#include	"iarduino_SensorPulse.h"

volatile	iarduino_SensorPulse_VolatileVariableClass ISPVVC;

			iarduino_SensorPulse::iarduino_SensorPulse(uint8_t i, uint8_t j){									//	i - номер аналогового входа к которому подключён сенсор, j - вывод биппера (звукоизлучателя с генератором).
				ISPVVC.ISP_pins_SEN=i;																			//	Сохраняем номер аналогового входа, к которому подключён сенсор пульса
				ISPVVC.ISP_pins_BEP=j;																			//	Сохраняем номер цифрового  вывода, к которому подключён биппер
			}

//			инициализация счётчика пульса
void		iarduino_SensorPulse::begin(){
				ISPVVC.ISP_flag_VAL=0;																			//	флаг указывающий о том, что показания сенсора валидны
				ISPVVC.ISP_data_MAX=0;																			//  максимальное значение сенсора за текущие 2 секунды
				ISPVVC.ISP_data_MIN=ISP_ADC_RESOLUTION;															//  минимальное  значение сенсора за текущие 2 секунды
				ISPVVC.ISP_time_CNT=0;																			//	счетчик прерываний 2 таймера
				ISPVVC.ISP_data_TOP=0;																			//	количество точек графика находящихся возле вершины пульса
				ISPVVC.ISP_flag_TOP=0;																			//	флаг указывающий о том, что более 7 точек графика находятся возле вершины пульса
				ISPVVC.ISP_time_TOP=0;																			//	время прошедшее после того, как более 7 точек графика вошли в вершину пульса (в мс/4)
				ISPVVC.ISP_time_ARR[0]=0;																		//	промежуток времени между последними пульсами
				ISPVVC.ISP_time_ARR[1]=0;																		//	промежуток времени между предпоследними пульсами
				ISPVVC.ISP_time_ARR[2]=0;																		//	промежуток времени между предпредпоследними пульсами
				ISPVVC.ISP_time_ARR[3]=0;																		//	промежуток времени между предпредпредпоследними пульсами
				ISPVVC.ISP_time_ARR[4]=0;																		//	промежуток времени между предпредпредпредпоследними пульсами
				Timer_Begin(1000);																				//	Разрешаем прерывания таймера на частоте 1кГц
				if(ISPVVC.ISP_pins_BEP<255){																	//	Если указан вывод биппера
					ISPVVC.ISP_data_BEP=0;																		//	Устанавливаем значение для биппера = LOW
					ISPVVC.ISP_flag_BEP=1;																		//	Устанавливаем флаг, сигнализирующий таймеру, о необходимости вывода сигналов на биппер
					pinMode     (ISPVVC.ISP_pins_BEP, OUTPUT);													//	Переводим вывод в режим выхода
					digitalWrite(ISPVVC.ISP_pins_BEP, ISPVVC.ISP_data_BEP);										//	Применяем установленные значение к выводу биппера
				}
			}

//			проверка результата
uint16_t	iarduino_SensorPulse::check(uint8_t i){
				switch(i){
					case ISP_ANALOG:	return ISPVVC.ISP_data_PIN[0];	break;
					case ISP_BEEP:		return ISP_func_CheckBeep();	break;
					case ISP_PULSE:		return ISP_func_CheckPulse();	break;
					case ISP_VALID:		
					#if defined(ESP8266)
					yield();																					//	Обычно check(ISP_VALID) ставят в цикл, что может вызвать Soft WDT reset, по этому сбрасываем WDT.
					#endif
					return ISP_func_CheckValid();	break;
				}
			}

//			проверка вершины пульса
uint16_t	iarduino_SensorPulse::ISP_func_CheckBeep(){
				return ISPVVC.ISP_time_TOP/25;																	//	выводим время прошедшее после начала пульса, в десятых долях секунды ( return 1 = 0,1 сек )
			}

//			проверка значения пульса
uint16_t	iarduino_SensorPulse::ISP_func_CheckPulse(){
				uint32_t i = (ISPVVC.ISP_time_ARR[0]+ISPVVC.ISP_time_ARR[1]+ISPVVC.ISP_time_ARR[2]+ISPVVC.ISP_time_ARR[3]+ISPVVC.ISP_time_ARR[4])/5;
				if(ISPVVC.ISP_time_ARR[0]<10){return 0;}
				if(ISPVVC.ISP_time_ARR[1]<10){return 0;}
				if(ISPVVC.ISP_time_ARR[2]<10){return 0;}
				if(ISPVVC.ISP_time_ARR[3]<10){return 0;}
				if(ISPVVC.ISP_time_ARR[4]<10){return 0;}
				if(i>999){i=999;}             return i;
			}

//			проверка валидности результата
uint8_t		iarduino_SensorPulse::ISP_func_CheckValid(){
				uint8_t	i1, i2, j1=0, j2=0, j3=ISPVVC.ISP_flag_VAL;
					if(ISPVVC.ISP_data_PIN[0]-ISP_ADC_FLUCTUATION>ISPVVC.ISP_data_PIN[1]){i1=1;}				//	i1 - вектор направления предыдущей точки графика
					if(ISPVVC.ISP_data_PIN[0]+ISP_ADC_FLUCTUATION<ISPVVC.ISP_data_PIN[1]){i1=0;}				//	i1==1 => вверх, i1==0 => вниз
				for(uint8_t k=1; k<9; k++){																		//	Проходим по 9 последним прочитанным значениям аналогового входа
					if(ISPVVC.ISP_data_PIN[k]-ISP_ADC_FLUCTUATION>ISPVVC.ISP_data_PIN[k+1]){i2=1;}				//	i2 - вектор направления текущей точки графика
					if(ISPVVC.ISP_data_PIN[k]+ISP_ADC_FLUCTUATION<ISPVVC.ISP_data_PIN[k+1]){i2=0;}				//	i2==1 => вверх, i2==0 => вниз
					if(ISPVVC.ISP_data_PIN[k]>ISP_ADC_LIMIT_MAX||ISPVVC.ISP_data_PIN[k]<ISP_ADC_LIMIT_MIN){j1++;}//	j1 - количество точек графика за пределами нормальных значений
					if(i1!=i2){j2++;} i1=i2;																	//	j2 - количество изменений вектора направления текущей точки относительно предыдущей
				}	if(j2<2){ISPVVC.ISP_flag_VAL=1;}															//	если зафиксировано до    2 изменений вектора направления графика из 10 последних точек, то считаем что показания сенсора    валидны
					if(j2>3){ISPVVC.ISP_flag_VAL=0;}															//	если зафиксировано более 3 изменений вектора направления графика из 10 последних точек, то считаем что показания сенсора не валидны
					if(j1>5){ISPVVC.ISP_flag_VAL=0;}															//	если зафиксировано более 5 точек графика, из 10 последних, за пределами нормальных показаний, то считаем что показания сенсора не валидны
					if(check(ISP_PULSE)>300){ISPVVC.ISP_flag_VAL=0;}											//	если пульс выше 300 ударов в минуту, то считаем что показания сенсора не валидны
					if(check(ISP_PULSE)==0 ){ISPVVC.ISP_flag_VAL=0;}											//	если пульса нет, то считаем что показания сенсора не валидны
					if(j3!=ISPVVC.ISP_flag_VAL)	{return ISP_CHANGED;}
					if(ISPVVC.ISP_flag_VAL)		{return ISP_CONNECTED;}
					else						{return ISP_DISCONNECTED;}
			}

//			Обработка прерываний таймера																		//
			Timer_Callback(Timer_Argument){																		//
				ISPVVC.ISP_time_CNT++;																			//	увеличиваем счетчик прерываний
				if (ISPVVC.ISP_time_CNT%4==0){																	//	заходим в таймер 1 раз за 4 прерывания
					if (ISPVVC.ISP_time_TOP<0xFFFF){ISPVVC.ISP_time_TOP++;}										//	увеличиваем время прошедшее после того, как более 7 точек графика вошли в вершину пульса
					ISPVVC.ISP_data_TOP=0;																		//	сброс количества точек графика находящихся возле вершины пульса
					for(uint8_t i=9; i>0; i--){																	//	проход по всем элементам массива, ранее прочитанных данных
						ISPVVC.ISP_data_PIN[i]=ISPVVC.ISP_data_PIN[i-1];										//	сдвиг всех ранее прочитанных данных: 19=18, 18=17, 17=16, ... 1=0
						if(ISPVVC.ISP_data_PIN[i]>ISPVVC.ISP_data_CEN){ISPVVC.ISP_data_TOP++;}					//	подсчет количества точек графика находящихся возле вершины пульса
					}	ISPVVC.ISP_data_PIN[0]=analogRead(ISPVVC.ISP_pins_SEN);									//	считываем показания сенсора
					if (ISPVVC.ISP_time_CNT>=2000){ISPVVC.ISP_time_CNT=0;										//	если произошло переполнение счетчика прерываний (более 2 секунд), то ...
						ISPVVC.ISP_data_CEN=ISPVVC.ISP_data_MIN+(ISPVVC.ISP_data_MAX-ISPVVC.ISP_data_MIN)*2/3;	//	определяем 2/3 части от минимального до максимального значения за предыдущие 2 секунды
						ISPVVC.ISP_data_MAX=0; ISPVVC.ISP_data_MIN=ISP_ADC_RESOLUTION;							//	сбрасываем максимальное и минимальное значение сенсора за текущие 2 секунды
					}																							//
					if (ISPVVC.ISP_data_MAX<ISPVVC.ISP_data_PIN[0]){ISPVVC.ISP_data_MAX=ISPVVC.ISP_data_PIN[0];}//	обновляем максимальное значение
					if (ISPVVC.ISP_data_MIN>ISPVVC.ISP_data_PIN[0]){ISPVVC.ISP_data_MIN=ISPVVC.ISP_data_PIN[0];}//	обновляем минимальное  значение
					if (ISPVVC.ISP_data_TOP>7){																	//	если более 7 точек графика находятся возле вершины пульса
						if(ISPVVC.ISP_flag_TOP==0){																//	если до этого возле вершины пульса находились менее 3 точек графика
							for(uint8_t i=4; i>0; i--){ISPVVC.ISP_time_ARR[i]=ISPVVC.ISP_time_ARR[i-1];}		//	сдвиг всех ранее прочитанных данных о промежутках времени между пульсами: 4=3, 3=2, 2=1, 1=0
							ISPVVC.ISP_time_WAS=ISPVVC.ISP_time_NOW; ISPVVC.ISP_time_NOW=millis();				//	фиксируем текущее время
							if(ISPVVC.ISP_time_WAS>=ISPVVC.ISP_time_NOW){ISPVVC.ISP_time_WAS=0;}				//	избавляемся от переполнения millis()
							ISPVVC.ISP_time_ARR[0]=ISPVVC.ISP_time_NOW-ISPVVC.ISP_time_WAS;						//	фиксируем промежуток времени между последними пульсами
							if(ISPVVC.ISP_time_ARR[0]>0){ISPVVC.ISP_time_ARR[0]=60000/ISPVVC.ISP_time_ARR[0];}	//	переводим зафиксированный промежуток времени из мс в кол/мин
							ISPVVC.ISP_time_TOP=0;																//	сброс времени входа более 7 точек графика в вершину пульса
						}	ISPVVC.ISP_flag_TOP=1;																//	фиксируем вход в вершину пульса
					}																							//
					if (ISPVVC.ISP_data_TOP<3){ISPVVC.ISP_flag_TOP=0;}											//	фиксируем уход с вершины пульса
				}
				if (ISPVVC.ISP_flag_BEP){ if (ISPVVC.ISP_flag_VAL){ if (ISPVVC.ISP_time_TOP>0){ if (ISPVVC.ISP_time_TOP<25){ ISPVVC.ISP_data_BEP=ISPVVC.ISP_data_BEP==1?0:1; digitalWrite(ISPVVC.ISP_pins_BEP, ISPVVC.ISP_data_BEP); }}}}
			}
