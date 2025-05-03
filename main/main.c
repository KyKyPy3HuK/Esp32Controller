#include <stdio.h>
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "driver/spi_master.h"
#include "esp_lcd_st7735.h"
#include "esp_log.h"
#include "lcdDisplay.h"

#define LCD_WIDTH 128
#define LCD_HEIGHT 160
#define LCD_HOST SPI2_HOST

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


static const char *TAG = "ST7735";

void delay_ms(int delayMs) {
    vTaskDelay(delayMs / portTICK_PERIOD_MS);
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

void InitDisplay(esp_lcd_panel_handle_t *panel_handle){
    // 1. Настройка SPI
    spi_bus_config_t buscfg = {
        .miso_io_num = -1,
        .mosi_io_num = 23,
        .sclk_io_num = 18,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = LCD_WIDTH * LCD_HEIGHT * 2 + 8,
    };
    spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO);

    // 2. Настройка интерфейса дисплея
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = 2,
        .cs_gpio_num = 5,
        .pclk_hz = 40 * 1000 * 1000, // 5 MHz
        .spi_mode = 0,
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
        .trans_queue_depth = 10,
    };
    
    //Инициализация I/O хендла
    esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST, &io_config, &io_handle);

    // 3. Кастомные команды инициализации
    static const uint8_t colmod_data[] = {0x05};  // RGB565
    static const uint8_t madctl_data[] = {0xC0};  // Ориентация

    static const st7735_lcd_init_cmd_t custom_init_cmds[] = {
         {ST7735_SWRESET, NULL, 0, 150},
         {ST7735_SLPOUT, NULL, 0, 150},
         {ST7735_COLMOD, colmod_data, sizeof(colmod_data), 10},
         {ST7735_MADCTL, madctl_data, sizeof(madctl_data), 0},
         {ST7735_DISPON, NULL, 0, 20},
    };
     
    //Конфигурация команд
    st7735_vendor_config_t vendor_config = {
        .init_cmds = custom_init_cmds,
        .init_cmds_size = sizeof(custom_init_cmds) / sizeof(st7735_lcd_init_cmd_t)
    };

    // 4. Инициализация панели
    //Сброс панели, если она существовала
    if (*panel_handle != NULL)
    {
        esp_lcd_panel_del(*panel_handle);
    }

    *panel_handle = NULL;

    //Кофигурация панели
    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = 4,
        .rgb_endian = LCD_RGB_ENDIAN_BGR,
        .bits_per_pixel = 16,
        .vendor_config = &vendor_config,
    };

    //Создание хендла панели
    esp_lcd_new_panel_st7735(io_handle, &panel_config, panel_handle);
    
    //Финиш
    esp_lcd_panel_reset(*panel_handle);
    esp_lcd_panel_init(*panel_handle);
    esp_lcd_panel_disp_on_off(*panel_handle, true);

}

void app_main() {
    esp_lcd_panel_handle_t panel_handle = NULL;

    InitDisplay(&panel_handle);

    uint16_t red = 0xF800; // RGB565 красный
    uint16_t *buffer = heap_caps_malloc(LCD_WIDTH * LCD_HEIGHT * sizeof(uint16_t), MALLOC_CAP_DMA);
    
    for (int i = 0; i < LCD_WIDTH * LCD_HEIGHT; i++) {
        buffer[i] = red;
    }

    esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, LCD_WIDTH, LCD_HEIGHT, buffer);
 
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

        // 5. Задержка (30 FPS)
        vTaskDelay(pdMS_TO_TICKS(1));
     }
}