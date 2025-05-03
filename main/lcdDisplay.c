#ifndef __LCD_DISPLAY_C__
#define __LCD_DISPLAY_C__

#include "lcdDisplay.h"

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

#endif