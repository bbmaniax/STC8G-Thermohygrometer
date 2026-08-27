// SSD1306_Driver

#ifdef __INTELLISENSE__
#define __SDCC_SYNTAX_FIX
#endif

#include "ssd1306_driver.h"

#define SSD1306_I2C_ADDRESS 0x3C
#define SSD1306_I2C_ADDRESS_SHIFT 1U
#define SSD1306_I2C_COMMAND_CONTROL 0x00
#define SSD1306_I2C_DATA_CONTROL 0x40

#define SSD1306_I2C_SET_PIN_MODE GPIO_P3_SetMode
#define SSD1306_I2C_SDA_PIN GPIO_Pin_3
#define SSD1306_I2C_SCL_PIN GPIO_Pin_2
#define SSD1306_I2C_PORT I2C_AlterPort_P32_P33

#define SSD1306_I2C_CLOCK_PRESCALER 0x10

static void SSD1306_Driver_sendByte(uint8_t data) {
    I2C_MasterSendData(data);
    I2C_MasterRxAck();
}

static void SSD1306_Driver_beginTransmission(uint8_t control) {
    SFRX_ON();
    I2C_MasterStart();
    SSD1306_Driver_sendByte(SSD1306_I2C_ADDRESS << SSD1306_I2C_ADDRESS_SHIFT);
    SSD1306_Driver_sendByte(control);
}

static void SSD1306_Driver_endTransmission(void) {
    I2C_MasterStop();
    SFRX_OFF();
}

void SSD1306_Driver_init(void) {
}

void SSD1306_Driver_begin(void) {
    SSD1306_I2C_SET_PIN_MODE(SSD1306_I2C_SDA_PIN, GPIO_Mode_InOut_QBD);
    SSD1306_I2C_SET_PIN_MODE(SSD1306_I2C_SCL_PIN, GPIO_Mode_Output_PP);
    I2C_SetWorkMode(I2C_WorkMode_Master);
    I2C_SetClockPrescaler(SSD1306_I2C_CLOCK_PRESCALER);
    I2C_SetPort(SSD1306_I2C_PORT);
    I2C_SetEnabled(HAL_State_ON);
}

void SSD1306_Driver_sendCommandList(const __CODE uint8_t* commands, uint8_t count) {
    SSD1306_Driver_beginTransmission(SSD1306_I2C_COMMAND_CONTROL);
    while (count--) {
        SSD1306_Driver_sendByte(*commands++);
    }
    SSD1306_Driver_endTransmission();
}

void SSD1306_Driver_sendData(__XDATA uint8_t* data, uint8_t count) {
    SSD1306_Driver_beginTransmission(SSD1306_I2C_DATA_CONTROL);
    while (count--) {
        SSD1306_Driver_sendByte(*data++);
    }
    SSD1306_Driver_endTransmission();
}
