//----------------------------------------------------------------------------------
// I2C Test Terminal - String Table for Documentation / Help.
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

#ifndef I2C_TERMINAL_STR_HELP_DEF
#define I2C_TERMINAL_STR_HELP_DEF

// General help.

#define HELP_GEN_ABOUT  "Following commands are available for the I2C test terminal:"
#define HELP_GEN_CMD_HELP           "- help"
#define HELP_GEN_CMD_INIT           "- init"
#define HELP_GEN_CMD_START          "- start"
#define HELP_GEN_CMD_STOP           "- stop"
#define HELP_GEN_CMD_WRITE          "- write"
#define HELP_GEN_CMD_WRITE_ADDR     "- write-address"
#define HELP_GEN_CMD_READ           "- read"
#define HELP_GEN_CMD_OUT_VOLTAGE    "- output-voltage"
#define HELP_GEN_CMD_RESET          "- reset"
#define HELP_GEN_CMD_EXIT           "- exit"

#define HELP_USE_HELP1  "\nTo get details, enter the help command with one of the above commands."
#define HELP_USE_HELP2  "For example:"
#define HELP_USE_HELP3  "\n\033[1m\033[37m help reset\033[0m"

#define HELP_AUTO_COMPLETE1     "\nI2C test terminal has an auto-complete command prompt. To use this"
#define HELP_AUTO_COMPLETE2     "option, press the TAB key twice on the command prompt."

#define HELP_GITHUB_REF1    "\nTo get the latest version of this application, device firmware, or the"
#define HELP_GITHUB_REF2    "documentation, visit https://github.com/dilshan/i2c-test-terminal.\n"

// Help for INIT command.

#define HELP_INIT_FORMAT    "Format: init [SPEED]"
#define HELP_INIT_INTRO1    "\nInitialize the I2C bus with the given speed. For the \033[1m\033[37m[SPEED]\033[0m specify one"
#define HELP_INIT_INTRO2    "of the following speed configurations:"

#define HELP_INIT_SPEED_100     "\n100: 100kHz"
#define HELP_INIT_SPEED_250     "250: 250kHz"
#define HELP_INIT_SPEED_400     "400: 400kHz"

#define HELP_INIT_WARNING1  "\nThis command must issue before continue with any I2C request."
#define HELP_INIT_WARNING2  "\n"

// Help for START command.

#define HELP_START_FORMAT   "Format: start"
#define HELP_START_INTRO1   "\nIssue this command to raise the I2C START condition. Each I2C"
#define HELP_START_INTRO2   "session initiated by the I2C test terminal must with a START"
#define HELP_START_INTRO3   "condition.\n"

// Help for STOP command.

#define HELP_STOP_FORMAT    "Format: stop"
#define HELP_STOP_INTRO1    "\nIssue this command to raise the I2C STOP condition. A Stop condition"
#define HELP_STOP_INTRO2    "always denotes the END of a transmission.\n"

// Help for WRITE command.

#define HELP_WRITE_FORMAT   "Format: write [VALUE]"
#define HELP_WRITE_INTRO1   "\nWrite/send specified \033[1m\033[37m[VALUE]\033[0m into the I2C bus. In this command, \033[1m\033[37m[VALUE]\033[0m"
#define HELP_WRITE_INTRO2   "is an 8-bit (base 10) integer or hexadecimal value. All hexadecimal values"
#define HELP_WRITE_INTRO3   "must begin with the \"\033[1m\033[37m0x\033[0m\" prefix.\n"

// Help for WRITE-ADDRESS command.

#define HELP_WRITE_ADDR_FORMAT  "Format: write-address [VALUE]"
#define HELP_WRITE_ADDR_INTRO1  "\nWrite slave address into the I2C bus. In this command, \033[1m\033[37m[VALUE]\033[0m is a"
#define HELP_WRITE_ADDR_INTRO2  "combination of a 7-bit slave address with the read/write flag bit. The"
#define HELP_WRITE_ADDR_INTRO3  "\033[1m\033[37m[VALUE]\033[0m parameter accepts (base 10) integer or hexadecimal value. All"
#define HELP_WRITE_ADDR_INTRO4  "hexadecimal values must begin with the \"\033[1m\033[37m0x\033[0m\" prefix.\n"

// Help for READ command.

#define HELP_READ_FORMAT    "Format: read {FLAG}"
#define HELP_READ_INTRO1    "\nRead data byte available in the I2C bus. In this command, \033[1m\033[37m{FLAG}\033[0m is an"
#define HELP_READ_INTRO2    "optional value to specify the ACK or NACK condition. If \033[1m\033[37m{FLAG}\033[0m is not"
#define HELP_READ_INTRO3    "specified, the I2C terminal issue NACK to the I2C slave device."

#define HELP_READ_INT_VAL1  "\nThe \033[1m\033[37m{FLAG}\033[0m parameter also accepts integer values as ACK or NACK. For"
#define HELP_READ_INT_VAL2  "the ACK, specify 1 for the \033[1m\033[37m{FLAG}\033[0m parameter, and use 0 for the NACK"
#define HELP_READ_INT_VAL3  "condition.\n"

// Help for SET-VOLTAGE command.

#define HELP_SET_VOLTAGE_FORMAT     "Format: output-voltage [VOLTAGE]"
#define HELP_SET_VOLTAGE_INTRO1     "\nIssue this command to set the output voltage of the I2C test terminal. For"
#define HELP_SET_VOLTAGE_INTRO2     "this command, accepted values for \033[1m\033[37m[VOLTAGE]\033[0m parameter are 3.3, 3.3V or"
#define HELP_SET_VOLTAGE_INTRO3     "5, 5.0, 5V, 5.0V."

#define HELP_SET_VOLTAGE_WARNING1   "\nIt is recommended to call this function and set the output voltage at the"
#define HELP_SET_VOLTAGE_WARNING2   "beginning of the I2C emulation. This command is not advisable to use in"
#define HELP_SET_VOLTAGE_WARNING3   "the middle of the I2C session(s).\n"

// Help for RESET command.

#define HELP_RESET_FORMAT   "Format: reset"
#define HELP_RESET_INTRO1   "\nThis command resets both power and internal data buffers of the I2C test"
#define HELP_RESET_INTRO2   "terminal."

#define HELP_RESET_POWER1   "\nDuring the power reset I2C terminal does not reset the voltage level of"
#define HELP_RESET_POWER2   "the output terminal.\n"

// Help for EXIT command.

#define HELP_EXIT_FORMAT    "Format: exit"
#define HELP_EXIT_INTRO     "\nIssue this command to close the I2C test terminal session.\n"

#endif /* I2C_TERMINAL_STR_HELP_DEF */
