#include <stdio.h>
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "MainMenuScene.h"
#include "SnakeGameScene.h"
#include "menu8x8_font.h"
#include "utils.h"


#define DEBOUNCE_DELAY_MS 200

Button buttons[] = {
    {PIN_BTN_UP, 0},
    {PIN_BTN_DOWN, 0},
    {PIN_BTN_LEFT, 0},
    {PIN_BTN_RIGHT, 0},
    {PIN_BTN_FNC_RIGHT, 0},
    {PIN_BTN_FNC_LEFT, 0}
};

volatile GlobalContext globalContext;
volatile SceneManager sceneManager;


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
    
    SceneManagerHandleInput(globalContext.sceneManager, btn->pin);
}

void GpioConfig(void) {
    // Настройка всех кнопок одновременно
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << PIN_BTN_UP) | 
                       (1ULL << PIN_BTN_DOWN) | 
                       (1ULL << PIN_BTN_LEFT) | 
                       (1ULL << PIN_BTN_RIGHT) |
                       (1ULL << PIN_BTN_FNC_LEFT) | 
                       (1ULL << PIN_BTN_FNC_RIGHT),
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

void app_main() {

    GpioConfig();

    esp_lcd_panel_handle_t panel_handle = NULL;

    InitDisplay(&panel_handle);

    initDrawingContext(panel_handle);

    Font8 MainFontMenu8x8;
    InitFont(&MainFontMenu8x8, font_menu8x8, FONT_MENU88_LENGTH);
    char fpsBuf[10];
    memset(fpsBuf,0,sizeof(fpsBuf));
    uint32_t curTimeMs = 0;
    uint32_t prevTimeMs = 0;
    uint32_t difTimeMs = 0;

    SceneManagerInit(&sceneManager, &globalContext);
    globalContext.sceneManager = &sceneManager;

    Scene mainMenuScene;

    InitMainMenuScene(&mainMenuScene, &MainFontMenu8x8);
    SceneManagerPushScene(globalContext.sceneManager, &mainMenuScene);

    while (1) {
        prevTimeMs = curTimeMs;
        curTimeMs = esp_timer_get_time();
        
        difTimeMs = curTimeMs - prevTimeMs;
        
        itoa( 1000000 / difTimeMs, fpsBuf, 10);

        // 1. Очистка буфера
        fillBuffer(0);
        SceneManagerUpdate(globalContext.sceneManager);
        DrawString(fpsBuf, &MainFontMenu8x8, 0, 0, 0xffff);

        // 4. Обновление дисплея
        drawBufferFull();

        DELAY(100);
    }
}