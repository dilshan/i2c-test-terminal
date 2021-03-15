//----------------------------------------------------------------------------------
// I2C Test Terminal - Generic String Table.
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

#ifndef I2C_TERMINAL_STR_DEF
#define I2C_TERMINAL_STR_DEF

#define DEV_NOT_AVAILABLE   "I2C Terminal device is not connected to the system or not functioning properly."
#define DEV_NOT_OPEN        "Unable to open the USB device."

#define MSG_INTRO_NAME      "I2C Terminal - Copyright (c) 2021 Dilshan R Jayakody. (jayakody2000lk@gmail.com)\n"
#define MSG_INTRO_HELP      "Type \"\033[1m\033[37mhelp\033[0m\" to list down the available commands. Enter \"\033[1m\033[37mhelp [COMMAND]\033[0m\" to get the information about the specific command.\n"
#define MSG_OUTPUT_VOLTAGE  "Current I2C output voltage: \033[1m\033[37m%sV\033[0m\n"

#define CMD_MSG_UNKNOWN             "Unknown command."
#define CMD_MSG_PARAMETER_MISSING   "Required parameter(s) are missing."
#define CMD_MSG_OUTOF_RANGE         "Specified parameter is out of range."
#define CMD_MSG_SPEED_UNSUPPORT     "Unsupported I2C speed, only 100kHz, 250kHz and 400kHz are supported by the device."
#define CMD_PARAM_IGNORE            "Specified parameters are ignored by the command."
#define CMD_PARAM_READ_UNSUPPORTED  "Unsupported read flag, only \033[1m\033[37mack\033[0m, \033[1m\033[37mnack\033[0m, \033[1m\033[37m1\033[0m and \033[1m\033[37m0\033[0m are allowd as parameters."
#define CMD_PARAM_INVALID_VOLTAGE   "Invalid voltage level, only 3.3V or 5V output is available with the device."
#define CMD_VOLTAGE_SAME            "Current output voltage is same as the specified voltage."

#define PROMPT_VOLTAGE_CHANGE       "Selected voltage level is different from the current output voltage, continue the voltage change"

#define DEV_COM_FAIL                "Communication failure has occur while writing data to the device."
#define DEV_COM_TIMEOUT             "I2C timeout occur, slave device is not responding."
#define DEV_COM_UNKNOWN             "Unknown I2C error."
#define DEV_COM_OUTPUT_VOLTAGE_FAIL "Unable to get I2C output voltage from the device."

#define DEV_COM_START_TX        "A START condition has been transmitted."
#define DEV_COM_REPEAT_START    "A repeated START condition has been transmitted."
#define DEV_COM_SLAVE_W         "Slave address with WRITE flag has been transmitted, ACK has been received."
#define DEV_COM_SLAVE_W_NACK    "Slave address with WRITE flag has been transmitted, NOT ACK has been received."
#define DEV_COM_NOT_ACK         "Slave address with READ flag has been transmitted, ACK has been received."
#define DEV_COM_DATA_TX         "Data byte has been transmitted, ACK has been received."
#define DEV_COM_DATA_TX_NACK    "Data byte has been transmitted, NOT ACK has been received."
#define DEV_COM_ARIB_LOST       "Arbitration lost in Slave address (with WRITE flag) or data bytes."
#define DEV_COM_REPEATE_WAIT    "Slave address with READ flag has been transmitted, NOT ACK has been received"
#define DEV_COM_DATA_TX_ACK     "Data byte has been received, ACK has been returned."
#define DEV_COM_DATA_RX_NACK    "Data byte has been received, NOT ACK has been returned."

#endif /* I2C_TERMINAL_STR_DEF */