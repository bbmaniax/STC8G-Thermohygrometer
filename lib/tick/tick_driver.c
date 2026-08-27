// Tick_Driver

#ifdef __INTELLISENSE__
#define __SDCC_SYNTAX_FIX
#endif

#include <stdbool.h>
#include "tick_driver.h"
#include "fw_hal.h"

#define TICK_DRIVER_FREQUENCY_HZ 1000U
#define TICK_DRIVER_HIGH_BYTE_SHIFT 8U
#define TICK_DRIVER_TIMER_RELOAD (UINT16_MAX - (__SYSCLOCK / TICK_DRIVER_FREQUENCY_HZ))

static volatile uint16_t tickCount;

INTERRUPT(Tick_Driver_Timer0_Routine, EXTI_VectTimer0)
{
    tickCount++;
}

void Tick_Driver_init(void)
{
    tickCount = 0;
}

void Tick_Driver_begin(void)
{
    TIM_Timer0_Set1TMode(HAL_State_ON);
    TIM_Timer0_SetMode(TIM_TimerMode_16BitAuto);
    TIM_Timer0_SetInitValue((uint8_t)(TICK_DRIVER_TIMER_RELOAD >> TICK_DRIVER_HIGH_BYTE_SHIFT), (uint8_t)TICK_DRIVER_TIMER_RELOAD);
    EXTI_Timer0_SetIntState(HAL_State_ON);
    EXTI_Global_SetIntState(HAL_State_ON);
    TIM_Timer0_SetRunState(HAL_State_ON);
}

uint16_t Tick_Driver_getMillis(void)
{
    uint16_t value;
    bool interruptState = EA;

    EXTI_Global_SetIntState(HAL_State_OFF);
    value = tickCount;
    EXTI_Global_SetIntState(interruptState ? HAL_State_ON : HAL_State_OFF);

    return value;
}
