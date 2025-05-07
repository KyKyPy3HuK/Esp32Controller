#ifndef __DRAWING_C__
#define __DRAWIN_C__

#include "drawing.h"

// Два DMA-буфера
uint16_t *_frame_buffer[2];
int _currentBufferNum = 0;

esp_lcd_panel_handle_t panel_handle = NULL;

/**
 * Смена буферов местами в механизме двойной буферизации
 */
void _swapBuffers(void){
    _currentBufferNum = 1 - _currentBufferNum;
}

/**
 * Выделение памяти под буфера и инициализация
 */
void _initBuffers(void) {
    _frame_buffer[0] = heap_caps_malloc(BUFFER_SIZE, MALLOC_CAP_DMA);
    _frame_buffer[1] = heap_caps_malloc(BUFFER_SIZE, MALLOC_CAP_DMA);
    memset(_frame_buffer[0], 0, BUFFER_SIZE);
    memset(_frame_buffer[1], 0, BUFFER_SIZE);
}

/**
 * Освобождение памяти буферов
 */
void _freeBuffers(void){
    free(_frame_buffer[0]);
    free(_frame_buffer[1]);
}

/**
 * Вывод всего буфера на дисплей
 */
void drawBufferFull(){
    esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, LCD_WIDTH, LCD_HEIGHT, _frame_buffer[_currentBufferNum]);
}

void drawBufferRect(int x, int y, int w, int h){
    esp_lcd_panel_draw_bitmap(panel_handle,x, y, w, h, _frame_buffer[_currentBufferNum]);
}


/**
 * Очистка всего буфера (заполнение нулями)
 */
void clearBuffer(){
    memset(_frame_buffer[_currentBufferNum], 0, BUFFER_SIZE);
}

/**
 * Заполнение всего буффера цветом 
 */
void fillBuffer(uint16_t rgb565Color){
    memset(_frame_buffer[_currentBufferNum], rgb565Color, BUFFER_SIZE);
}

void draw_rect_to_buffer(int x, int y, int w, int h, uint16_t color) {
    
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            int pos = (y + i) * LCD_WIDTH + (x + j);
            _frame_buffer[_currentBufferNum][pos] = color;
        }
    }
}

/**
 * Вывод строки на буфер
 */
void DrawString(char* string, Font8 font, int x, int y, uint16_t color){
    uint8_t charData;
    uint32_t charNum = 0;
    char curChar = string[charNum];

    while (curChar != '\0')
    {
        for (uint32_t i = 0; i < 8; i++)
        {
 
            for (uint32_t i = 0; i < 8; i++)
            {
                
            }
                
        }
        charNum++;
        curChar = string[charNum];
        if (charNum > 255)
        {
            
            break;
        }
        
    }
}

//Инициализация шрифта
void InitFont(Font8* font, uint8_t* fontData, int fontDataLen){
    font->fontData = fontData;
    font->fontDataLen = fontDataLen;
}


//Инициализация контекста рисования
void initDrawingContext(esp_lcd_panel_handle_t handle){
    panel_handle = handle;
    _initBuffers();
}

/**
 * Деинициализация контекста рисования
 */
void destroyDrawingContext(){
    _freeBuffers();
}


#endif