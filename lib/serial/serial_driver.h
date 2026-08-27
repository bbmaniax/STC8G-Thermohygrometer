#ifndef SERIAL_DRIVER_H
#define SERIAL_DRIVER_H

void Serial_Driver_init(void);
void Serial_Driver_begin(void);
void Serial_Driver_writeChar(char value);
void Serial_Driver_writeString(const char* value);

#endif
