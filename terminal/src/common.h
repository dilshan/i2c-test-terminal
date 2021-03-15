//----------------------------------------------------------------------------------
// I2C Test Terminal - Common Header File / Configuration Settings.
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

#ifndef I2C_TERMINAL_COMMON
#define I2C_TERMINAL_COMMON

typedef unsigned char EXEC_STATUS;

struct UsbComData
{
    unsigned char* comData;
    int deviceHandler;
};

#define EXEC_SUCCESS    0x00
#define EXEC_FAIL       0xFF

#define SYS_SIGNATURE       0x5D
#define SYS_END_SIGNATURE   0x5A

#define USB_CMD_NONE            0x00
#define USB_CMD_I2C_INIT        0x01
#define USB_CMD_I2C_START       0x02
#define USB_CMD_I2C_STOP        0x03
#define USB_CMD_I2C_WRITE_ADDR  0x04
#define USB_CMD_I2C_WRITE       0x05
#define USB_CMD_I2C_READ        0x06
#define USB_CMD_SET_VOLTAGE     0x07
#define USB_CMD_GET_VOLTAGE     0x08
#define USB_CMD_RESET           0x09

#define TWI_COM_SPEED_100   0   // 100kHz
#define TWI_COM_SPEED_250   1   // 250kHz
#define TWI_COM_SPEED_400   2   // 400kHz

#define USB_SET_COMMAND_BUFFER_SIZE 64
#define USB_GET_DATA_BUFFER_SIZE    64

#define RET_SUCCESS         0x00
#define RET_PENDING         0x01
#define RET_UNKNOWN         0x02
#define RET_TIMEOUT_FAIL    0xFF

#define I2C_OUTPUT_5V   0x01
#define I2C_OUTPUT_3V3  0x02

#endif /* I2C_TERMINAL_COMMON */