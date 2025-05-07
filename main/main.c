#include <stdio.h>
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "lcdDisplay.h"
#include "utils.h"
#include "drawing.h"
#include "menu8x8_font.h"



// Параметры анимации
typedef struct {
    int x, y;       // Позиция
    int w, h;       // Размер
    int dx, dy;     // Направление
    uint16_t color; // Цвет
} DVDObject;

DVDObject dvd;

// Два DMA-буфера
uint16_t *frame_buffer[2];
int current_buffer = 0;


// Инициализация DVD объекта
void init_dvd_object() {
    dvd.w = 30;
    dvd.h = 20;
    dvd.x = rand() % (LCD_WIDTH - dvd.w);
    dvd.y = rand() % (LCD_HEIGHT - dvd.h);
    dvd.dx = (rand() % 2) ? 2 : -2;
    dvd.dy = (rand() % 2) ? 2 : -2;
    dvd.color = 0xF800; // Красный по умолчанию
}


// Обновление позиции DVD
void update_dvd_position() {


    // Отскок от границ
    if (dvd.x < 2 || dvd.x + 2 > LCD_WIDTH - dvd.w) {
        dvd.dx = -dvd.dx;
        dvd.color = rand() % 0xFFFF; // Случайный цвет при ударе
    }
    if (dvd.y < 2 || dvd.y + 2 > LCD_HEIGHT - dvd.h) {
        dvd.dy = -dvd.dy;
        dvd.color = rand() % 0xFFFF; // Случайный цвет при ударе
    }
    dvd.x += dvd.dx;
    dvd.y += dvd.dy;
}


void app_main() {
    esp_lcd_panel_handle_t panel_handle = NULL;

    InitDisplay(&panel_handle);

    init_dvd_object();
    initDrawingContext(panel_handle);
    uint16_t color = 0;

    Font8 fontMenu8x8;
    InitFont(&fontMenu8x8, font_menu8x8,FONT_MENU88_LENGTH);


    while (1) {

        // 1. Очистка буфера
        fillBuffer(1);
        DrawString("Абоба", fontMenu8x8, 0, 0, 0xffff);
        // 2. Обновление позиции
        update_dvd_position();
        draw_rect_to_buffer(0, 0, 100, 50, 0x1234);
        // 3. Рисование DVD прямоугольника
        draw_rect_to_buffer(dvd.x, dvd.y, dvd.w, dvd.h, dvd.color);
        
        // 4. Обновление дисплея
        drawBufferFull();

        //Свап 
        _swapBuffers();
        DELAY(30);
    }
}