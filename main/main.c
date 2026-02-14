#include <stdio.h>
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "string.h"

#include "pinConfig.h"
#include "lcdDisplay.h"
#include "utils.h"
#include "drawing.h"
#include "menu8x8_font.h"
#include "menu.h"


#define DEBOUNCE_DELAY_MS 200


// Структура для хранения состояния кнопок
typedef struct {
    int pin;
    uint64_t last_press_time;
} Button;

Button buttons[] = {
    {PIN_BTN_UP, 0},
    {PIN_BTN_DOWN, 0},
    {PIN_BTN_LEFT, 0},
    {PIN_BTN_RIGHT, 0}
};

volatile uint8_t red = 0;
volatile uint8_t green = 0;
volatile uint8_t blue = 0;
volatile Menu menu;


void ColorInc(void){
    switch (MenuGetFocusItemIndex(menu))
    {
    case 0:
        red++;
        break;
    case 1:
        green++;
        break;
    case 2:
        blue++;
        break;
    default:
        break;
    }
}
void ColorDec(void){
    switch (MenuGetFocusItemIndex(menu))
    {
    case 0:
        red--;
        break;
    case 1:
        green--;
        break;
    case 2:
        blue--;
        break;
    default:
        break;
    }
}

/**
 * Обработчик кнопок
 */
void IRAM_ATTR button_isr_handler(void* arg) {
    Button* btn = (Button*)arg;
    uint64_t current_time = esp_timer_get_time() / 1000;
    
    // Защита от дребезга
    if ((current_time - btn->last_press_time) < DEBOUNCE_DELAY_MS) {
        return;
    }
    
    btn->last_press_time = current_time;
    
    // Определяем какая кнопка нажата
    switch (btn->pin) {
        case PIN_BTN_UP:
            MenuPrevItem(&menu);
            break;
        case PIN_BTN_DOWN:
            MenuNextItem(&menu);
            break;
        case PIN_BTN_LEFT:
        ColorDec();
            break;
        case PIN_BTN_RIGHT:
        ColorInc();
            break;
    }
}

void GpioConfig(void) {
    // Настройка всех кнопок одновременно
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << PIN_BTN_UP) | 
                       (1ULL << PIN_BTN_DOWN) | 
                       (1ULL << PIN_BTN_LEFT) | 
                       (1ULL << PIN_BTN_RIGHT),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE  // Прерывание по спаду (нажатие)
    };
    gpio_config(&io_conf);

    // Установка одного обработчика для всех кнопок
    gpio_install_isr_service(0);
    for (int i = 0; i < sizeof(buttons)/sizeof(Button); i++) {
        gpio_isr_handler_add(buttons[i].pin, button_isr_handler, (void*)&buttons[i]);
    }
}

void MenuConfig(void){
    MenuItem rMenuItem;
    MenuItemInit(&rMenuItem, "Красный канал");
    MenuItem gMenuItem;
    MenuItemInit(&gMenuItem,"Зеленый канал");
    MenuItem bMenuItem;
    MenuItemInit(&bMenuItem, "Синий канал");
    MenuAddItem(&menu, rMenuItem);
    MenuAddItem(&menu, gMenuItem);
    MenuAddItem(&menu, bMenuItem);
}

void DrawMenu(int xPos, int yPos, Font8 font, uint16_t colorBase, uint16_t colorSelected){
    for (int i = 0; i < menu.itemsCount; i++)
	{
		//Вывод текущего пункта меню (на который указвает курсор)
		if (i == menu.focusItemIndex)
		{
            DrawString(menu.items[i].text, font, xPos ,i * 10 + yPos, colorSelected);
		}
		else
		{
			DrawString(menu.items[i].text,font, xPos ,i * 10 + yPos, colorBase);
		}
        if (menu.items[i].isActivated == 1)
        {
            DrawString("v",font,xPos + (8 * strlen(menu.items[i].text)), i * 10 + yPos, colorBase);
        }
        
	}
}


void app_main() {

    GpioConfig();
    MenuConfig();
    esp_lcd_panel_handle_t panel_handle = NULL;

    InitDisplay(&panel_handle);

    initDrawingContext(panel_handle);
    uint16_t color = 0;

    Font8 fontMenu8x8;
    InitFont(&fontMenu8x8, font_menu8x8,FONT_MENU88_LENGTH);
    char fpsBuf[10];
    memset(fpsBuf,0,sizeof(fpsBuf));
    int fps = 0;
    uint32_t curTimeMs = 0;
    uint32_t prevTimeMs = 0;
    uint32_t difTimeMs = 0;

    char redBuf[10];
    char greenBuf[10];
    char blueBuf[10];

    while (1) {
        prevTimeMs = curTimeMs;
        curTimeMs = esp_timer_get_time();
        
        difTimeMs = curTimeMs - prevTimeMs;
        
        itoa( 1000000 / difTimeMs, fpsBuf, 10);

        // 1. Очистка буфера
        fillBuffer(0);

        DrawString(fpsBuf, fontMenu8x8, 0, 0, 0xffff);
        DrawString("Colors", fontMenu8x8, 6, 0, 0xffff);
        DrawMenu(8,8,fontMenu8x8,0xaaaa,0xffff);

        itoa( red, redBuf, 10);
        itoa( green, greenBuf, 10);
        itoa( blue, blueBuf, 10);
        DrawString(redBuf, fontMenu8x8, 140, 8, 0xffff);
        DrawString(greenBuf, fontMenu8x8, 140, 16, 0xffff);
        DrawString(blueBuf, fontMenu8x8, 140, 24, 0xffff);
        draw_rect_to_buffer(0,60,160, 68, toRgb565(red,green,blue));

        // 4. Обновление дисплея
        drawBufferFull();

        DELAY(100);
    }
}