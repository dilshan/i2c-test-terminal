//----------------------------------------------------------------------------------
// I2C Test Terminal Firmware - Core Functions and Main Entry Point.
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

#ifndef I2C_TESTER_MAIN_HEADER
#define I2C_TESTER_MAIN_HEADER

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

#define I2C_OUTPUT_5V   0x01
#define I2C_OUTPUT_3V3  0x02

static unsigned char reqBuffer[4];

static unsigned char lastCommandStatus;
static unsigned char lastCommandData;
static unsigned char lastCommand;

void initSystem();
void clearRequestBuffer();
unsigned char setOutputVoltage(unsigned char voltage, unsigned char *newVoltage);
unsigned char resetI2CSlave(unsigned char voltage);

#endif /* I2C_TESTER_MAIN_HEADER */