// App

#ifdef __INTELLISENSE__
#define __SDCC_SYNTAX_FIX
#endif

#include "event.h"
#include "formatter.h"
#include "model.h"
#include "serial_driver.h"
#include "view.h"
#include "app.h"

static void dispatch(void);
static void printTemperature(int16_t temperature);

static __XDATA char temperatureText[FORMATTER_TEMPERATURE_BUFFER_SIZE];

void setup()
{
    Serial_Driver_init();
    Model_init();
    View_init();
    EventManager_init();

    Serial_Driver_begin();
    Model_begin();
    View_begin();
    EventManager_begin();

    Serial_Driver_writeString("Ready\r\n");
}

void loop()
{
    EventManager_update();
    dispatch();
    View_update();
}

static void dispatch(void)
{
    if (EventManager_isTemperatureUpdated()) {
        printTemperature(EventManager_getTemperature());
    }

    Model_update();
}

static void printTemperature(int16_t temperature)
{
    Formatter_formatTemperature(temperatureText, temperature);
    Serial_Driver_writeString("Temperature: ");
    Serial_Driver_writeString(temperatureText);
    Serial_Driver_writeString(" C\r\n");
}
