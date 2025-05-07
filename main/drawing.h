#ifndef __DRAWING_H__
#define __DRAWIN_H__

#include "lcdDisplay.h"

#define BUFFER_SIZE (LCD_WIDTH * LCD_HEIGHT * sizeof(uint16_t))


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

void DrawString(char* string, Font8 font, int x, int y, uint16_t color);

void InitFont(Font8* font, uint8_t* fontData, int fontDataLen);

void initDrawingContext(esp_lcd_panel_handle_t handle);

void destroyDrawingContext(void);

#endif