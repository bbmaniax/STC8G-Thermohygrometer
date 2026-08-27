// DS18B20_Driver

#ifdef __INTELLISENSE__
#define __SDCC_SYNTAX_FIX
#endif

#include "ds18b20.h"

#define DS18B20_SET_MODE GPIO_P5_SetMode
#define DS18B20_PORT GPIO_Port_5
#define DS18B20_PIN GPIO_Pin_4
#define DS18B20_DATA_LINE P54

#define ONEWIRE_COMMAND_SKIP_ROM 0xCC
#define DS18B20_COMMAND_CONVERT_TEMPERATURE 0x44
#define DS18B20_COMMAND_READ_SCRATCHPAD 0xBE

#define DS18B20_TEMPERATURE_OFFSET -9
#define DS18B20_INVALID_DATA 0xFF
#define DS18B20_BYTE_SHIFT 8U
#define DS18B20_SCALE_MULTIPLY_SHIFT 2U
#define DS18B20_SCALE_DIVIDE_SHIFT 3U

#define ONEWIRE_RESET_LOW_US 500U
#define ONEWIRE_PRESENCE_WAIT_US 70U
#define ONEWIRE_RESET_RECOVERY_US 410U

#define ONEWIRE_WRITE_ONE_LOW_US 10U
#define ONEWIRE_WRITE_ONE_RECOVERY_US 55U
#define ONEWIRE_WRITE_ZERO_LOW_US 65U
#define ONEWIRE_WRITE_ZERO_RECOVERY_US 5U

#define ONEWIRE_READ_LOW_US 2U
#define ONEWIRE_READ_SAMPLE_US 10U
#define ONEWIRE_READ_RECOVERY_US 50U

static bool DS18B20_Driver_reset(void) {
    uint8_t interruptState = EA;
    bool present;

    EA = false;
    DS18B20_DATA_LINE = false;
    SYS_DelayUs(ONEWIRE_RESET_LOW_US);
    DS18B20_DATA_LINE = true;
    SYS_DelayUs(ONEWIRE_PRESENCE_WAIT_US);
    present = DS18B20_DATA_LINE == false;
    SYS_DelayUs(ONEWIRE_RESET_RECOVERY_US);
    EA = interruptState;

    return present;
}

static void DS18B20_Driver_writeBit(bool value) {
    uint8_t interruptState = EA;

    EA = false;
    DS18B20_DATA_LINE = false;
    if (value) {
        SYS_DelayUs(ONEWIRE_WRITE_ONE_LOW_US);
        DS18B20_DATA_LINE = true;
        SYS_DelayUs(ONEWIRE_WRITE_ONE_RECOVERY_US);
    } else {
        SYS_DelayUs(ONEWIRE_WRITE_ZERO_LOW_US);
        DS18B20_DATA_LINE = true;
        SYS_DelayUs(ONEWIRE_WRITE_ZERO_RECOVERY_US);
    }
    EA = interruptState;
}

static bool DS18B20_Driver_readBit(void) {
    uint8_t interruptState = EA;
    bool value;

    EA = false;
    DS18B20_DATA_LINE = false;
    SYS_DelayUs(ONEWIRE_READ_LOW_US);
    DS18B20_DATA_LINE = true;
    SYS_DelayUs(ONEWIRE_READ_SAMPLE_US);
    value = DS18B20_DATA_LINE != false;
    SYS_DelayUs(ONEWIRE_READ_RECOVERY_US);
    EA = interruptState;

    return value;
}

static void DS18B20_Driver_writeByte(uint8_t value) {
    uint8_t bitMask;

    for (bitMask = 1; bitMask; bitMask <<= 1) {
        DS18B20_Driver_writeBit((value & bitMask) != 0);
    }
}

static uint8_t DS18B20_Driver_readByte(void) {
    uint8_t bitMask;
    uint8_t value = 0;

    for (bitMask = 1; bitMask; bitMask <<= 1) {
        if (DS18B20_Driver_readBit()) {
            value |= bitMask;
        }
    }

    return value;
}

void DS18B20_Driver_init(void) {
}

void DS18B20_Driver_begin(void) {
    DS18B20_SET_MODE(DS18B20_PIN, GPIO_Mode_InOut_OD);
    GPIO_SetPullUp(DS18B20_PORT, DS18B20_PIN, HAL_State_ON);
    DS18B20_DATA_LINE = true;
}

void DS18B20_Driver_requestTemperature(void) {
    if (!DS18B20_Driver_reset()) {
        return;
    }

    DS18B20_Driver_writeByte(ONEWIRE_COMMAND_SKIP_ROM);
    DS18B20_Driver_writeByte(DS18B20_COMMAND_CONVERT_TEMPERATURE);
}

bool DS18B20_Driver_readTemperature(__XDATA int16_t* temperature) {
    uint8_t low;
    uint8_t high;
    int16_t raw;
    uint16_t magnitude;
    bool negative;

    if (!DS18B20_Driver_reset()) {
        return false;
    }

    DS18B20_Driver_writeByte(ONEWIRE_COMMAND_SKIP_ROM);
    DS18B20_Driver_writeByte(DS18B20_COMMAND_READ_SCRATCHPAD);
    low = DS18B20_Driver_readByte();
    high = DS18B20_Driver_readByte();

    if (low == DS18B20_INVALID_DATA && high == DS18B20_INVALID_DATA) {
        return false;
    }

    raw = ((int16_t)high << DS18B20_BYTE_SHIFT) | low;
    negative = raw < 0;
    magnitude = negative ? (uint16_t)-raw : (uint16_t)raw;
    magnitude = (magnitude + (magnitude << DS18B20_SCALE_MULTIPLY_SHIFT)) >> DS18B20_SCALE_DIVIDE_SHIFT;
    *temperature = (negative ? -(int16_t)magnitude : (int16_t)magnitude) + DS18B20_TEMPERATURE_OFFSET;

    return true;
}
