//----------------------------------------------------------------------------------
// I2C Test Terminal - Display and Terminal Related Helper Functions.
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

#include "termutil.h"
#include "common.h"
#include "strdef.h"

#include <stdio.h>

void printDeviceStatusMsg(unsigned char errorCode)
{
    switch(errorCode)
    {
    // Terminal base errors.
    case USB_CMD_NONE:
        // No Error.
        break;
    case RET_TIMEOUT_FAIL:
        printErrorMsg(DEV_COM_TIMEOUT);
        break;
    case RET_UNKNOWN:
        printErrorMsg(DEV_COM_UNKNOWN);
        break;
    // I2C specific status codes.
    case 0x08:
        printStatus(DEV_COM_START_TX);
        break;
    case 0x10:
        printStatus(DEV_COM_REPEAT_START);
        break;
    case 0x18:
        printStatus(DEV_COM_SLAVE_W);
        break;
    case 0x20:
        printStatus(DEV_COM_SLAVE_W_NACK);
        break;
    case 0x28:
        printStatus(DEV_COM_DATA_TX);
        break;
    case 0x30:
        printStatus(DEV_COM_DATA_TX_NACK);
        break;
    case 0x38:
        printStatus(DEV_COM_ARIB_LOST);
        break;
    case 0x40:
        printStatus(DEV_COM_NOT_ACK);
        break;
    case 0x48:
        printStatus(DEV_COM_REPEATE_WAIT);
        break;
    case 0x50:
        printStatus(DEV_COM_DATA_TX_ACK);
        break;
    case 0x58:
        printStatus(DEV_COM_DATA_RX_NACK);
        break;
    // Unknown error code.
    default:
        printf(ERROR_UNKNOWN_FORMATTER, errorCode, DEV_COM_UNKNOWN);
    }
}