#include <stdio.h>
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "lcdDisplay.h"
#include "utils.h"

#define BUFFER_SIZE (LCD_WIDTH * LCD_HEIGHT * sizeof(uint16_t))

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


// Инициализация буферов
void init_buffers() {
    frame_buffer[0] = heap_caps_malloc(BUFFER_SIZE, MALLOC_CAP_DMA);
    frame_buffer[1] = heap_caps_malloc(BUFFER_SIZE, MALLOC_CAP_DMA);
    memset(frame_buffer[0], 0, BUFFER_SIZE);
    memset(frame_buffer[1], 0, BUFFER_SIZE);
}


void fill_buffer(uint16_t color) {
    for (int i = 0; i < LCD_WIDTH * LCD_HEIGHT; i++) {
        frame_buffer[current_buffer][i] = color;
    }
}


void draw_rect_to_buffer(int x, int y, int w, int h, uint16_t color) {
    for (int dy = 0; dy < h; dy++) {
        for (int dx = 0; dx < w; dx++) {
            int pos = (y + dy) * LCD_WIDTH + (x + dx);
            frame_buffer[current_buffer][pos] = color;
        }
    }
}


void swap_buffers(esp_lcd_panel_handle_t panel_handle) {
    // Отправляем текущий буфер на дисплей
    esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, LCD_WIDTH, LCD_HEIGHT, frame_buffer[current_buffer]);
    
    // Переключаем буфер
    current_buffer = 1 - current_buffer;
    
    // Очищаем новый буфер (опционально)
    memset(frame_buffer[current_buffer], 0, BUFFER_SIZE);
}


// Заливка разными цветами
void fill_color(esp_lcd_panel_handle_t panel_handle ,uint16_t* buffer, uint16_t color) {
    for (int i = 0; i < LCD_WIDTH * LCD_HEIGHT; i++) {
        buffer[i] = color;
    }
    esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, LCD_WIDTH, LCD_HEIGHT, buffer);
}

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
    dvd.x += dvd.dx;
    dvd.y += dvd.dy;

    // Отскок от границ
    if (dvd.x <= 0 || dvd.x >= LCD_WIDTH - dvd.w) {
        dvd.dx = -dvd.dx;
        dvd.color = rand() % 0xFFFF; // Случайный цвет при ударе
    }
    if (dvd.y <= 0 || dvd.y >= LCD_HEIGHT - dvd.h) {
        dvd.dy = -dvd.dy;
        dvd.color = rand() % 0xFFFF; // Случайный цвет при ударе
    }
}


void app_main() {
    esp_lcd_panel_handle_t panel_handle = NULL;

    InitDisplay(&panel_handle);
 
    init_buffers();

    init_dvd_object();

    while (1) {
        // 1. Очистка буфера
        fill_buffer(0x0000); // Чёрный фон

        // 2. Обновление позиции
        update_dvd_position();

        // 3. Рисование DVD прямоугольника
        draw_rect_to_buffer(dvd.x, dvd.y, dvd.w, dvd.h, dvd.color);

        // 4. Обновление дисплея
        swap_buffers(panel_handle);

        DELAY(30);
    }
}