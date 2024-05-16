// ВЫВОД ЗНАЧЕНИЙ ПУЛЬСА В МОНИТОР ПОРТА:                 //
                                                          //
#include <iarduino_SensorPulse.h>                         // подключаем библиотеку для работы с датчиком пульса
iarduino_SensorPulse Pulse(A5);                           // Подключаем сенсор к 5 аналоговому входу. Допускается указать еще один вывод, в качестве второго параметра.
                                                          // На втором выводе библиотека будет устанавливать «1» в течении 100 мс после пика пульса.
void setup(){                                             //
     Serial.begin(9600);                                  // Инициируем связь с монитором последовательного порта на скорости 9600 бит/сек.
     Pulse.begin();                                       // Инициируем работу с датчиком пульса.
}                                                         //
                                                          //
void loop(){                                              //
     if(Pulse.check(ISP_VALID)==ISP_CONNECTED){           // Возможные значения: ISP_DISCONNECTED, ISP_CONNECTED, ISP_CHANGED.
//   Eсли датчик пульса подключён ...                     //
         Serial.println("Датчик подключён!");             // Выводим оповещающий текст.
         while(Pulse.check(ISP_VALID)==ISP_CONNECTED){    // Выполняем цикл, пока состояние датчика пульса определяется как ISP_CONNECTED.
         //  Выполняем цикл, пока сенсор подключён ...    //
             if(Pulse.check(ISP_BEEP)==0){                // Сверяемся с количеством децисекунд после пика пульса.
             //  Если после пика пульса прошло 0 дс:      // 1 дс (децисекунда) = 100 мс (миллисекунд) = 0.1 с (секунды).
                 Serial.println(Pulse.check(ISP_PULSE));  // Выводим среднее значение пульса (количество ударов в минуту).
                 delay(100);                              // Устанавливаем задержку на 100 мс. Тогда по условию оператора «if», код в 
             }                                            // его теле не сможет выполниться более 1 раза за один удар пульса.
         }                                                //
     }else{                                               // Если состояние датчика пульса отличается от ISP_CONNECTED.
//   Eсли сенсор отключён ...                             //
         Serial.println("Сенсор отключён!");              // Выводим оповещающий текст.
         while(Pulse.check(ISP_VALID)!=ISP_CONNECTED){    // Выполняем цикл пока состояние датчика пульса отличается от ISP_CONNECTED.
         //  Выполняем цикл, пока сенсор отключён ...     // Если сенсор подключится вновь, то произойдёт выход из цикла.
             delay(1000);                                 // Устанавливаем задержку на 1 секунду. Это уменьшит количество ложных определений датчика.
         }                                                // (библиотека определяет состояние датчика подключён/отключён по флуктуациям на входе).
     }                                                    //
}                                                         //
/* В библиотеке всего 2 функции: begin() и check().       //
 * Функция check() возвращает значение uint16_t:          //
 * Pulse.check(ISP_ANALOG); - возвращает число 0...1024 - Данные с аналогового входа, к которому подключён датчик.
 * Pulse.check(ISP_PULSE);  - возвращает число 0...999  - Пульс (количество ударов в минуту).
 * Pulse.check(ISP_BEEP);   - возвращает число 0...2621 - Количество десятых долей секунды, прошедшее после последнего пика пульса.
 * Pulse.check(ISP_VALID);  - возвращает состояние      - ISP_DISCONNECTED - датчик отключён  (данные не соответствуют пульсу).
 *                                                        ISP_CONNECTED    - датчик подключён (данные похожи на пульс).
 *                                                        ISP_CHANGED      - состояние датчика изменилось (с подключён на отключён или наоборот).
 */