#ifdef __INTELLISENSE__
#define __SDCC_SYNTAX_FIX
#endif

#include <stdbool.h>
#include "serial_driver.h"
#include "fw_hal.h"

#define SERIAL_DRIVER_BAUD_RATE 115200UL
#define SERIAL_DRIVER_BAUD_DIVISOR 4UL
#define SERIAL_DRIVER_TIMER_RANGE 0x10000UL
#define SERIAL_DRIVER_HIGH_BYTE_SHIFT 8U
#define SERIAL_DRIVER_TIMER_RELOAD (SERIAL_DRIVER_TIMER_RANGE - (__SYSCLOCK / (SERIAL_DRIVER_BAUD_DIVISOR * SERIAL_DRIVER_BAUD_RATE)))
#define SERIAL_DRIVER_PORT UART1_AlterPort_P30_P31

void Serial_Driver_init(void)
{
}

void Serial_Driver_begin(void)
{
    UART1_SwitchPort(SERIAL_DRIVER_PORT);
    SM0 = false;
    SM1 = true;
    UART1_SetBaudSource(UART1_BaudSource_Timer1);
    TIM_Timer1_Set1TMode(HAL_State_ON);
    TIM_Timer1_SetMode(TIM_TimerMode_16BitAuto);
    TIM_Timer1_SetInitValue((uint8_t)(SERIAL_DRIVER_TIMER_RELOAD >> SERIAL_DRIVER_HIGH_BYTE_SHIFT), (uint8_t)SERIAL_DRIVER_TIMER_RELOAD);
    TIM_Timer1_SetRunState(HAL_State_ON);
}

void Serial_Driver_writeChar(char value)
{
    UART1_WriteBuffer(value);
    while (!TI);
    UART1_ClearTxInterrupt();
}

void Serial_Driver_writeString(const char* value)
{
    while (*value) {
        Serial_Driver_writeChar(*value++);
    }
}
