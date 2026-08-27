#ifdef __INTELLISENSE__
#define __SDCC_SYNTAX_FIX
#endif

#include <stdbool.h>
#include "fw_reg_base.h"
#include "formatter.h"

uint8_t Formatter_formatTemperature(__XDATA char* buffer, int16_t temperature)
{
    uint16_t magnitude;
    uint8_t index = 0;
    char digit;
    bool leadingDigitWritten = false;

    if (temperature < 0) {
        buffer[index++] = '-';
        magnitude = (uint16_t)(-(temperature + 1)) + 1U;
    } else {
        magnitude = (uint16_t)temperature;
    }

    digit = '0';
    while (magnitude >= 10000U) {
        magnitude -= 10000U;
        digit++;
    }
    if (digit != '0') {
        buffer[index++] = digit;
        leadingDigitWritten = true;
    }

    digit = '0';
    while (magnitude >= 1000U) {
        magnitude -= 1000U;
        digit++;
    }
    if (leadingDigitWritten || digit != '0') {
        buffer[index++] = digit;
        leadingDigitWritten = true;
    }

    digit = '0';
    while (magnitude >= 100U) {
        magnitude -= 100U;
        digit++;
    }
    if (leadingDigitWritten || digit != '0') {
        buffer[index++] = digit;
    }

    digit = '0';
    while (magnitude >= 10U) {
        magnitude -= 10U;
        digit++;
    }
    buffer[index++] = digit;
    buffer[index++] = '.';
    buffer[index++] = (char)('0' + magnitude);
    buffer[index] = '\0';

    return index;
}
