// Button_Driver

#ifdef __INTELLISENSE__
#define __SDCC_SYNTAX_FIX
#endif

#include "button_driver.h"
#include "fw_gpio.h"

#define BUTTON_GPIO_SetMode GPIO_P5_SetMode
#define BUTTON_GPIO_PORT GPIO_Port_5
#define BUTTON_GPIO_PIN GPIO_Pin_5
#define BUTTON_PIN P55
#define BUTTON_PRESSED_STATE RESET
#define BUTTON_DRIVER_DEBOUNCE_DELAY 20U
#define BUTTON_DRIVER_LONG_PRESS_DELAY 1000U

typedef struct {
    bool currentState;
    bool lastState;
    bool lastReading;
    uint16_t lastDebounceTime;
    uint16_t pressStartTime;
    bool longPressTriggered;
} Button_Driver;

static __XDATA Button_Driver button;

static bool Button_Driver_read(void) {
    return BUTTON_PIN == BUTTON_PRESSED_STATE;
}

void Button_Driver_init(void) {
    button.currentState = false;
    button.lastState = false;
    button.lastReading = false;
    button.lastDebounceTime = 0;
    button.pressStartTime = 0;
    button.longPressTriggered = false;
}

void Button_Driver_begin(void) {
    BUTTON_GPIO_SetMode(BUTTON_GPIO_PIN, GPIO_Mode_Input_HIP);
    GPIO_SetPullUp(BUTTON_GPIO_PORT, BUTTON_GPIO_PIN, HAL_State_ON);
}

void Button_Driver_update(uint16_t currentTime) {
    bool reading = Button_Driver_read();

    if (reading != button.lastReading) {
        button.lastDebounceTime = currentTime;
    }

    if ((uint16_t)(currentTime - button.lastDebounceTime) > BUTTON_DRIVER_DEBOUNCE_DELAY) {
        button.lastState = button.currentState;
        button.currentState = reading;

        if (!button.lastState && button.currentState) {
            button.pressStartTime = currentTime;
            button.longPressTriggered = false;
        }

        if (button.lastState && !button.currentState) {
            if (!button.longPressTriggered &&
                (uint16_t)(currentTime - button.pressStartTime) >= BUTTON_DRIVER_LONG_PRESS_DELAY) {
                button.longPressTriggered = true;
            } else {
                button.longPressTriggered = false;
            }
            button.pressStartTime = 0;
        }
    }

    button.lastReading = reading;
}

bool Button_Driver_isPressed(void) {
    return button.currentState;
}

bool Button_Driver_isClicked(void) {
    return button.lastState && !button.currentState && !button.longPressTriggered;
}

bool Button_Driver_isLongPressed(void) {
    return button.lastState && !button.currentState && button.longPressTriggered;
}
