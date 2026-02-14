#ifndef __DRAWING_H__
#define __DRAWIN_H__

#include "lcdDisplay.h"
#include "math.h"
#define BUFFER_WIDTH LCD_WIDTH
#define BUFFER_HEIGHT LCD_HEIGHT
#define BUFFER_BYTES_PER_PIXEL 2
#define BUFFER_SIZE (BUFFER_WIDTH * BUFFER_HEIGHT * BUFFER_BYTES_PER_PIXEL)

static inline uint16_t toRgb565(uint8_t red, uint8_t green, uint8_t blue) {
    // Ограничиваем значения и сдвигаем до нужного диапазона:
    // R: 5 бит (из 8), G: 6 бит (из 8), B: 5 бит (из 8)
    return ((red   ) << 11) |  // 5 бит красного
           ((green ) << 5) |  // 6 бит зеленого
           ((blue  ));   // 5 бит синего
}

static inline uint16_t rgb565(uint8_t red, uint8_t green, uint8_t blue) {
    red = red >> 3;
    green = green >> 3;
    blue = blue >> 3;
    // Ограничиваем значения и сдвигаем до нужного диапазона:
    // R: 5 бит (из 8), G: 6 бит (из 8), B: 5 бит (из 8)
    return ((blue) << 8) |  // 5 бит красного
           ((red) << 3) |  // 6 бит зеленого
           ((green) >> 3);   // 5 бит синего
}


typedef struct Font8{
    int fontDataLen;
    uint8_t * fontData;
} Font8;

void _swapBuffers(void);

void _initBuffers(void);

void _freeBuffers(void);

void drawBufferFull(void);

void drawBufferRect(int x, int y, int w, int h);

void clearBuffer(void);

void fillBuffer(uint16_t rgb565Color);

void draw_rect_to_buffer(int x, int y, int w, int h, uint16_t color);

void drawGradientRectToBuffer(int x, int y, int w, int h, uint16_t srcColor, uint16_t dstColor, int gradStartX, int gradStartY, int gradEndX, int gradEndY);

void DrawString(char* string, Font8 font, int x, int y, uint16_t color);

void InitFont(Font8* font, uint8_t* fontData, int fontDataLen);

void initDrawingContext(esp_lcd_panel_handle_t handle);

void destroyDrawingContext(void);

#endif