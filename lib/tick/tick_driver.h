// Tick_Driver

#ifndef TICK_DRIVER_H
#define TICK_DRIVER_H

#include <stdint.h>

void Tick_Driver_init(void);
void Tick_Driver_begin(void);
uint16_t Tick_Driver_getMillis(void);

#endif // TICK_DRIVER_H
