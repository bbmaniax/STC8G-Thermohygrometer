// Button_Driver

#ifndef BUTTON_DRIVER_H
#define BUTTON_DRIVER_H

#include <stdbool.h>
#include <stdint.h>

void Button_Driver_init(void);
void Button_Driver_begin(void);
void Button_Driver_update(uint16_t currentTime);
bool Button_Driver_isPressed(void);
bool Button_Driver_isClicked(void);
bool Button_Driver_isLongPressed(void);

#endif // BUTTON_DRIVER_H
