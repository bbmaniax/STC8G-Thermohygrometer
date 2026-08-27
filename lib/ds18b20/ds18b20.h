// DS18B20_Driver

#ifndef DS18B20_H
#define DS18B20_H

#include <stdbool.h>
#include <stdint.h>
#include "fw_hal.h"

void DS18B20_Driver_init(void);
void DS18B20_Driver_begin(void);
void DS18B20_Driver_requestTemperature(void);
bool DS18B20_Driver_readTemperature(__XDATA int16_t* temperature);

#endif // DS18B20_H
