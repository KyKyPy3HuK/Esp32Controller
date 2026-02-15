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
 * Вывод прямоугольника с градиентом на буфер
 */
void drawGradientRectToBuffer(int x, int y, int w, int h,
    uint16_t srcColor, uint16_t dstColor,
    int gradStartX, int gradStartY,
    int gradEndX, int gradEndY) {
       // Разбираем цвета на компоненты с повышенной точностью
    uint32_t srcR = (srcColor >> 11) & 0x1F;
    uint32_t srcG = (srcColor >> 5) & 0x3F;
    uint32_t srcB = srcColor & 0x1F;
    
    uint32_t dstR = (dstColor >> 11) & 0x1F;
    uint32_t dstG = (dstColor >> 5) & 0x3F;
    uint32_t dstB = (dstColor & 0x1F);

    // Вектор направления градиента
    float dirX = gradEndX - gradStartX;
    float dirY = gradEndY - gradStartY;
    float length = sqrtf(dirX*dirX + dirY*dirY);
    
    // Нормализуем вектор направления
    if (length > 0) {
        dirX /= length;
        dirY /= length;
    }

    // Находим минимальную и максимальную проекции для прямоугольника
    float minProj = INFINITY, maxProj = -INFINITY;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            float px = x + (i ? w : 0);
            float py = y + (j ? h : 0);
            float proj = (px - gradStartX) * dirX + (py - gradStartY) * dirY;
            minProj = fminf(minProj, proj);
            maxProj = fmaxf(maxProj, proj);
        }
    }
    float projRange = maxProj - minProj;

    // Проходим по всем пикселям прямоугольника
    for (int row = y; row < y + h; row++) {
        for (int col = x; col < x + w; col++) {
            // Проекция текущей точки на вектор градиента
            float proj = (col - gradStartX) * dirX + (row - gradStartY) * dirY;
            
            // Нормализованная позиция [0..1] в пределах прямоугольника
            float pos = (proj - minProj) / projRange;
            pos = fmaxf(0.0f, fminf(1.0f, pos));

            // Интерполяция цветов
            uint8_t r = srcR + (uint8_t)((dstR - srcR) * pos);
            uint8_t g = srcG + (uint8_t)((dstG - srcG) * pos);
            uint8_t b = srcB + (uint8_t)((dstB - srcB) * pos);

            // Собираем цвет обратно в RGB565
            uint16_t color = (r << 11) | (g << 5) | b;

            // Записываем в буфер (правильный доступ для 160x128)
            if (col >= 0 && col < 160 && row >= 0 && row < 128) {
                _frame_buffer[_currentBufferNum][row * 160 + col] = color;
            }
        }
    }
}
// _frame_buffer[_currentBufferNum]
/**
 * Вывод строки на буфер
 */
//TODO Подумать можно ли Font* заменить на Font
void DrawString(char* string, Font8* font, int x, int y, uint16_t color){

    uint32_t charNum = 0;
    uint8_t curChar = string[charNum];
    uint32_t offsetX = 0;
    uint32_t offsetY = 0;
    uint32_t fontWidth = 8;
    uint32_t fontHeight = 8;
    uint32_t boundX = 0;
    uint32_t boundY = 0;
    uint32_t newLinesCount = 0;

    uint32_t baseAdr = 0;

    offsetX = x;
    while (curChar != '\0')
    {
        if (curChar == '\n')
        {
            charNum++;
            newLinesCount++;
            curChar = string[charNum];
            offsetX = x;
            continue;
        }

        
        offsetY = y + fontHeight * newLinesCount;

        boundX = (offsetX + fontWidth > BUFFER_WIDTH) ? BUFFER_WIDTH : offsetX + fontWidth;
        boundY = (offsetY + fontHeight > BUFFER_HEIGHT) ? BUFFER_HEIGHT : offsetY + fontHeight;

        baseAdr = (curChar - 32) * 8;
        uint8_t curFontData = 0;
        uint8_t relX = 0;
        uint8_t relY = 0;
        uint8_t curPixel = 0;

        

        for (uint32_t row = offsetY; row < boundY; row++)
        {
            for (uint32_t col = offsetX; col < boundX; col++)
            {
                curFontData = font->fontData[baseAdr + relX] >> relY;
                curPixel = curFontData & 0x1;
                if (curPixel == 1) {
                    _frame_buffer[_currentBufferNum][row * BUFFER_WIDTH + col] = color;
                }
                relX++;
            }
        
            relY++;
            relX = 0;
        }
        offsetX += fontWidth;
        charNum++;
        curChar = string[charNum];
        if (charNum > 255)
        {
            //На случай ошибки
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