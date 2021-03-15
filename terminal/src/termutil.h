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

#ifndef I2C_TERMINAL_UTILITIES
#define I2C_TERMINAL_UTILITIES

#define ERROR_TEXT_FORMATTER        "\x1b[31m%s\x1b[0m\n"
#define ERROR_TEXT_FORMATTER_EX     "\x1b[31m%s: %s\x1b[0m\n"
#define ERROR_UNKNOWN_FORMATTER     "\x1b[31m0x%x: %s\x1b[0m\n"
#define WARNING_TEXT_FORMATTER      "%s\n"
#define STATUS_TEXT_FORMATTER       "\x1b[33m%s\x1b[0m\n"
#define HELP_TEXT_FORMATTER         "%s\n"
#define HELP_CMDFORMAT_FORMATTER    "\x1B[33m%s\x1B[0m\n"

void printDeviceStatusMsg(unsigned char errorCode);

#define printErrorMsg(x) printf(ERROR_TEXT_FORMATTER, x)
#define printCommandError(m, p) printf(ERROR_TEXT_FORMATTER_EX, p, m)
#define printWarningMsg(x) printf(WARNING_TEXT_FORMATTER, x)
#define printData(x) printf("Data: 0x%x\n", x)
#define printStatus(x) printf(STATUS_TEXT_FORMATTER, x)
#define printHelp(x) printf(HELP_TEXT_FORMATTER, x)
#define printHelpCmdFormat(x) printf(HELP_CMDFORMAT_FORMATTER, x)

#endif /* I2C_TERMINAL_UTILITIES */