// Font5x7.h - 5x7 pixel font for monochrome displays

#ifndef FONT5X7_H
#define FONT5X7_H

#include <stdint.h>
#include "fw_hal.h"

#define FONT5X7_WIDTH 5
#define FONT5X7_HEIGHT 7

#define CHAR_DEGREE 0x01

int8_t Font5x7_getIndex(char c);
const __CODE uint8_t Font5x7_getData(int8_t index);

#endif // FONT5X7_H
