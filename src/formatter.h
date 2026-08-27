#ifndef FORMATTER_H
#define FORMATTER_H

#include <stdint.h>
#include "fw_reg_base.h"

#define FORMATTER_TEMPERATURE_BUFFER_SIZE 8U

uint8_t Formatter_formatTemperature(__XDATA char* buffer, int16_t temperature);

#endif
