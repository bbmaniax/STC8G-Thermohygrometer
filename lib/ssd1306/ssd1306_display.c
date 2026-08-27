// SSD1306_Display

#ifdef __INTELLISENSE__
#define __SDCC_SYNTAX_FIX
#endif

#include "ssd1306_display.h"

#define SSD1306_TRANSFER_CHUNK_SIZE 16U
#define SSD1306_ROTATION_MASK 0x03U
#define SSD1306_ROTATION_FLIPPED_THRESHOLD 2U
#define SSD1306_PAGE_HEIGHT 8U
#define SSD1306_DISPLAY_BUFFER_SIZE 256U

static __XDATA uint8_t displayBuffer[SSD1306_DISPLAY_BUFFER_SIZE];

static const __CODE uint8_t SSD1306_INIT_COMMANDS[] = {
    0xAE, 0xD5, 0x80, 0xA8, 0x3F, 0xD3, 0x00, 0x40, 0x8D, 0x14,
    0x20, 0x00, 0xA1, 0xC8, 0xDA, 0x12, 0x81, 0x8F, 0xD9, 0xF1,
    0xDB, 0x40, 0xA4, 0xA6, 0xAF
};

static const __CODE uint8_t SSD1306_DISPLAY_COMMANDS[] = {
    0x21, 0x00, 0x7F, 0x22, 0x00, 0x01
};

static const __CODE uint8_t SSD1306_ROTATION_0_COMMANDS[] = {
    0xA1, 0xC8
};

static const __CODE uint8_t SSD1306_ROTATION_2_COMMANDS[] = {
    0xA0, 0xC0
};

void SSD1306_Display_init(void) {
    SSD1306_Driver_init();
    SSD1306_Display_clearDisplay();
}

void SSD1306_Display_begin(void) {
    SSD1306_Driver_begin();
    SSD1306_Driver_sendCommandList(SSD1306_INIT_COMMANDS, sizeof(SSD1306_INIT_COMMANDS));
}

void SSD1306_Display_clearDisplay(void) {
    uint8_t offset = 0;

    do {
        displayBuffer[offset] = 0;
    } while (++offset);
}

void SSD1306_Display_display(void) {
    uint8_t offset = 0;

    SSD1306_Driver_sendCommandList(SSD1306_DISPLAY_COMMANDS, sizeof(SSD1306_DISPLAY_COMMANDS));

    do {
        SSD1306_Driver_sendData(displayBuffer + offset, SSD1306_TRANSFER_CHUNK_SIZE);
        offset += SSD1306_TRANSFER_CHUNK_SIZE;
    } while (offset);
}

void SSD1306_Display_setRotation(uint8_t rotation) {
    if ((rotation & SSD1306_ROTATION_MASK) < SSD1306_ROTATION_FLIPPED_THRESHOLD) {
        SSD1306_Driver_sendCommandList(SSD1306_ROTATION_0_COMMANDS, sizeof(SSD1306_ROTATION_0_COMMANDS));
    } else {
        SSD1306_Driver_sendCommandList(SSD1306_ROTATION_2_COMMANDS, sizeof(SSD1306_ROTATION_2_COMMANDS));
    }
}

uint8_t SSD1306_Display_getWidth(void) {
    return SSD1306_DISPLAY_WIDTH;
}

uint8_t SSD1306_Display_getHeight(void) {
    return SSD1306_DISPLAY_HEIGHT;
}

void SSD1306_Display_drawPixel(int8_t x, int8_t y, uint8_t color) {
    uint8_t index;
    uint8_t mask;

    if (x < 0 || y < 0 || y >= (SSD1306_DISPLAY_BUFFER_SIZE / SSD1306_DISPLAY_WIDTH * SSD1306_PAGE_HEIGHT)) {
        return;
    }

    index = (uint8_t)x;
    if (y >= SSD1306_PAGE_HEIGHT) {
        index |= SSD1306_DISPLAY_WIDTH;
    }

    mask = 1U << (y & (SSD1306_PAGE_HEIGHT - 1U));
    if (color == SSD1306_WHITE) {
        displayBuffer[index] |= mask;
    } else {
        displayBuffer[index] &= ~mask;
    }
}
