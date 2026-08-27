// EventManager

#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <stdbool.h>
#include <stdint.h>

void EventManager_init(void);
void EventManager_begin(void);
void EventManager_update(void);
uint16_t EventManager_getMillis(void);
int16_t EventManager_getTemperature(void);
bool EventManager_isTemperatureUpdated(void);
bool EventManager_isButtonPressed(void);
bool EventManager_isButtonClicked(void);
bool EventManager_isButtonLongPressed(void);

#endif // EVENT_MANAGER_H
