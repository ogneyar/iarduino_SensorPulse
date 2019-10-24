[![](https://iarduino.ru/img/logo.svg)](https://iarduino.ru)[![](https://wiki.iarduino.ru/img/git-shop.svg?3)](https://iarduino.ru) [![](https://wiki.iarduino.ru/img/git-wiki.svg?2)](https://wiki.iarduino.ru) [![](https://wiki.iarduino.ru/img/git-lesson.svg?2)](https://lesson.iarduino.ru)[![](https://wiki.iarduino.ru/img/git-forum.svg?2)](http://forum.trema.ru)

# iarduino\_SensorPulse

**This is a library for Arduino IDE. It allows to work with [Pulse Sensor](https://iarduino.ru/shop/Sensory-Datchiki/datchik-pulsa.html) module**

**Данная библиотека для Arduino IDE позволяет работать с модулем [Датчик пульса](https://iarduino.ru/shop/Sensory-Datchiki/datchik-pulsa.html)**

> Подробнее про установку библиотеки читайте в нашей [инструкции](https://wiki.iarduino.ru/page/Installing_libraries/).

> Подробнее про подключение к [Arduino UNO](https://iarduino.ru/shop/boards/arduino-uno-r3.html)/[Piranha UNO](https://iarduino.ru/shop/boards/piranha-uno-r3.html) читайте в нашем [уроке](https://lesson.iarduino.ru/page/urok-27-pulsometr/)


| Модель | Ссылка на магазин |
|---|---|
| <p></p> <img src="https://iarduino.ru/img/catalog/b55ce697191bcf7cce52b3197edb1f5b.jpg" width="100px"></img>| https://iarduino.ru/shop/Sensory-Datchiki/datchik-pulsa.html |


## Подключение библиотеки:

```C++
#include <iarduino_SensorPulse.h>

iarduino_SensorPulse Pulse(вывод_датчика [, вывод зуммера]); // если зуммер не используется, то второй аргумент не указывается.
```

## Назначение функций и переменных:

Подробное описание работы с библиотекой, находится в разделе [Урок 27. Пульсометр](https://lesson.iarduino.ru/page/urok-27-pulsometr/).

**В библиотеке реализованы 2 функции:**

Функция **begin**();

- Назначение: Инициализация работы с датчиком пульса.
- Синтаксис: begin();
- Параметры: Нет.
- Возвращаемые значения: Нет.
- Примечание: Вызывается 1 раз в коде setup.

Функция **check**();

- Назначение: Определение состояния и данных сенсора.
- Синтаксис: check(параметр);
- Параметры: функция принимает 1 из 4 параметров ...
    - Pulse.check(ISP\_ANALOG); - функция вернёт текущее значение с аналогового входа, к которому подключён датчик;
    - Pulse.check(ISP\_PULSE); - функция вернёт среднее значение пульса (количество пульсаций в минуту);
    - Pulse.check(ISP\_BEEP); - функция вернёт количество десятых долей секунды, прошедшее после последнего пика пульса;
    - Pulse.check(ISP\_VALID); - функция вернёт 1 из 3 состояний датчика:
        - ISP_CONNECTED - подключён (данные похожи на пульс);
        - ISP_DISCONNECTED - отключён (данные не соответствуют пульсу);
        - ISP_CHANGED - состояние сенсора изменилось (с подключён на отключён или наоборот).
- Возвращаемые значения: uint16\_t число.

Библиотека использует второй таймер arduino. Не выводите ШИМ на 3 или 11 вывод.

