#ifndef __LCD_DISPLAY_H__
#define __LCD_DISPLAY_H__

//Тактовый сигнал SCLK SPI дисплея
#define DISPLAY_PIN_SCLK  18
//Сигнал данных MOSI SPI дисплея
#define DISPLAY_PIN_MOSI  23
//Сигнал выбора чипа ChipSelect дисплея
#define DISPLAY_PIN_CS    5
//Сигнал выбора данных/команд Data/Command дисплея
#define DISPLAY_PIN_DC    2
//Сигнал сброса RESET дисплея
#define DISPLAY_PIN_RST   4
//Сигнал подсветки BLINK дисплея
#define DISPLAY_PIN_BLK   15  // Подсветка (опционально)

#endif