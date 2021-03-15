//----------------------------------------------------------------------------------
// I2C Test Terminal Firmware - I2C Driver.
//
// Copyright (c) 2021 Dilshan R Jayakody [jayakody2000lk@gmail.com].
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//----------------------------------------------------------------------------------

#ifndef I2C_DRIVER_HEADER
#define I2C_DRIVER_HEADER

#define RET_SUCCESS         0x00
#define RET_PENDING         0x01
#define RET_UNKNOWN         0x02
#define RET_TIMEOUT_FAIL    0xFF

#define I2C_TIMEOUT     0x7FF
#define I2C_DELAY_DELTA     500

// I2C speed configurations.
#define TWI_COM_SPEED_100   0   // 100kHz
#define TWI_COM_SPEED_250   1   // 250kHz
#define TWI_COM_SPEED_400   2   // 400kHz

void i2cInit(unsigned char comSpeed);
unsigned char i2cStart(void (*usbProc)(void));
void i2cStop();

unsigned char i2cWriteAddr(void (*usbProc)(void), unsigned char addr);
unsigned char i2cWrite(void (*usbProc)(void), unsigned char data);
unsigned char i2cRead(void (*usbProc)(void), unsigned char ack, unsigned char *data);

#endif /* I2C_DRIVER_HEADER */