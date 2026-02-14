#ifndef __LCD_DISPLAY_H__
#define __LCD_DISPLAY_H__

#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_st7735.h"

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
//Ширина дисплея в пикселях
#define LCD_WIDTH 160
//Высота дисплея в пикселях
#define LCD_HEIGHT 128
//Частота SPI 
#define DISPLAY_SPI_SPEED_MHZ 40
//Используемый интерфейс
#define LCD_HOST SPI2_HOST

/**
 * Инициализация дисплея
 * @param panel_handle хендл дисплея, который будет инициализирован
 */
void InitDisplay(esp_lcd_panel_handle_t *panel_handle);

#endif