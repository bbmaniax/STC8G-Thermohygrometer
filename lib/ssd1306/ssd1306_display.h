// SSD1306_Display

#ifndef SSD1306_DISPLAY_H
#define SSD1306_DISPLAY_H

#include "ssd1306_driver.h"

#define SSD1306_BLACK 0
#define SSD1306_WHITE 1
#define SSD1306_DISPLAY_WIDTH 128
#define SSD1306_DISPLAY_HEIGHT 64

void SSD1306_Display_init(void);
void SSD1306_Display_begin(void);
void SSD1306_Display_clearDisplay(void);
void SSD1306_Display_display(void);
void SSD1306_Display_setRotation(uint8_t rotation);
uint8_t SSD1306_Display_getWidth(void);
uint8_t SSD1306_Display_getHeight(void);
void SSD1306_Display_drawPixel(int8_t x, int8_t y, uint8_t color);

#endif // SSD1306_DISPLAY_H
