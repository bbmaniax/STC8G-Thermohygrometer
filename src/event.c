// EventManager

#ifdef __INTELLISENSE__
#define __SDCC_SYNTAX_FIX
#endif

#include "fw_reg_base.h"
#include "tick_driver.h"
#include "button_driver.h"
#include "ds18b20.h"
#include "event.h"

#define EVENT_MANAGER_TEMPERATURE_CONVERSION_MILLIS 750U

typedef struct {
    uint16_t currentMillis;
    uint16_t temperatureRequestMillis;
    int16_t currentTemperature;
    bool temperatureUpdated;
    bool buttonPressed;
    bool buttonClicked;
    bool buttonLongPressed;
} EventManager;

static __XDATA EventManager eventManager;

void EventManager_init(void)
{
    Tick_Driver_init();
    Button_Driver_init();
    DS18B20_Driver_init();
    eventManager.currentMillis = 0;
    eventManager.temperatureRequestMillis = 0;
    eventManager.currentTemperature = INT16_MIN;
    eventManager.temperatureUpdated = false;
    eventManager.buttonPressed = false;
    eventManager.buttonClicked = false;
    eventManager.buttonLongPressed = false;
}

void EventManager_begin(void)
{
    Button_Driver_begin();
    DS18B20_Driver_begin();
    Tick_Driver_begin();
    DS18B20_Driver_requestTemperature();
    eventManager.temperatureRequestMillis = Tick_Driver_getMillis();
}

void EventManager_update(void)
{
    eventManager.currentMillis = Tick_Driver_getMillis();
    eventManager.temperatureUpdated = false;

    if ((uint16_t)(eventManager.currentMillis - eventManager.temperatureRequestMillis) >= EVENT_MANAGER_TEMPERATURE_CONVERSION_MILLIS) {
        if (!DS18B20_Driver_readTemperature(&eventManager.currentTemperature)) {
            eventManager.currentTemperature = INT16_MIN;
        }
        eventManager.temperatureUpdated = true;
        DS18B20_Driver_requestTemperature();
        eventManager.temperatureRequestMillis = eventManager.currentMillis;
    }

    Button_Driver_update(eventManager.currentMillis);
    eventManager.buttonPressed = Button_Driver_isPressed();
    eventManager.buttonClicked = Button_Driver_isClicked();
    eventManager.buttonLongPressed = Button_Driver_isLongPressed();
}

uint16_t EventManager_getMillis(void)
{
    return eventManager.currentMillis;
}

int16_t EventManager_getTemperature(void)
{
    return eventManager.currentTemperature;
}

bool EventManager_isTemperatureUpdated(void)
{
    return eventManager.temperatureUpdated;
}

bool EventManager_isButtonPressed(void)
{
    return eventManager.buttonPressed;
}

bool EventManager_isButtonClicked(void)
{
    return eventManager.buttonClicked;
}

bool EventManager_isButtonLongPressed(void)
{
    return eventManager.buttonLongPressed;
}
