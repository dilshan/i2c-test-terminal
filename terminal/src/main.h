//----------------------------------------------------------------------------------
// I2C Test Terminal - Core Functions and Main Entry Point.
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

#ifndef I2C_TERMINAL_MAIN
#define I2C_TERMINAL_MAIN

#include <libudev.h>

#include "common.h"

#define I2C_TERMINAL_DEV_VID    0x16C0
#define I2C_TERMINAL_DEV_PID    0x1231

void *sendDataToDevice(void *dataPtr);
EXEC_STATUS getTerminalDevicePath(struct udev *udev, char **hidRawPath);
EXEC_STATUS getCurrentOutputVoltage(int deviceHandler, unsigned char *voltage);
EXEC_STATUS isContinue(const unsigned char *msg);

#endif /* I2C_TERMINAL_MAIN */