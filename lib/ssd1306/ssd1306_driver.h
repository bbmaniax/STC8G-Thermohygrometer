// SSD1306_Driver

#ifndef SSD1306_DRIVER_H
#define SSD1306_DRIVER_H

#include <stdint.h>
#include "fw_hal.h"

void SSD1306_Driver_init(void);
void SSD1306_Driver_begin(void);
void SSD1306_Driver_sendCommandList(const __CODE uint8_t* commands, uint8_t count);
void SSD1306_Driver_sendData(__XDATA uint8_t* data, uint8_t count);

#endif // SSD1306_DRIVER_H
