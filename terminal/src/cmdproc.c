//----------------------------------------------------------------------------------
// I2C Test Terminal - Command Processor.
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

#include "cmdproc.h"
#include "termutil.h"
#include "strdef.h"
#include "common.h"
#include "docuproc.h"

#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <string.h>

#define RELEASE_STR(x) free(x);x=NULL

#define MAX_TOKEN_COUNT 2

// List of commands available with I2C terminal.
const char *cmdList[] = {"help", "init", "start", "stop", "write", "write-address", "read", "output-voltage", "reset", "exit"};

char *cmdGenerator(const char *text, int state)
{
    static int listIndex, len;
    const char *name;

    if (!state)
    {
        listIndex = 0;
        len = strlen(text);
    }

    // Looking for matching commands for the given name.
    while (name = cmdList[listIndex])
    {
        listIndex++;
        if (strncmp(name, text, len) == 0)
        {
            return strdup (name);
        }
    }

    // Unable to find matching entry from the list.
    return ((char *) NULL);
}

static char **cmdCompletion(const char *text, int start, int end)
{
    // Avoid adding spaces to the end of the matching word.
    rl_completion_append_character = '\0';

    char **matches = (char **) NULL;
    if (start == 0)
    {
        matches = rl_completion_matches((char *) text, &cmdGenerator);
    }

    return matches;
}

unsigned char *createUSBBuffer(unsigned char cmd, unsigned char data)
{
    unsigned char* usbBuffer = calloc(USB_SET_COMMAND_BUFFER_SIZE, 1);
    usbBuffer[0] = SYS_SIGNATURE;
    usbBuffer[1] = cmd;
    usbBuffer[2] = data;
    usbBuffer[3] = SYS_END_SIGNATURE;

    return usbBuffer;
}

EXEC_STATUS getSpeed(char **strBuffer, unsigned char *out)
{
    long convNum;
    
    if((*strBuffer)[0] == '\0')
    {
        // String buffer is empty.
        printErrorMsg(CMD_MSG_PARAMETER_MISSING);
        return EXEC_FAIL;
    }

    convNum = strtol((*strBuffer), NULL, 0);
    if((convNum == 100) || (convNum == 250) || (convNum == 400))
    {
        // Convert speed value into byte which identify by the firmware.
        switch(convNum)
        {
        case 100:
            *out = TWI_COM_SPEED_100;
            break;
        case 250:
            *out = TWI_COM_SPEED_250;
            break;
        case 400:
            *out = TWI_COM_SPEED_400;
            break;
        }
    }
    else
    {
        // Unsupported I2C speed value.
        printErrorMsg(CMD_MSG_SPEED_UNSUPPORT);
        return EXEC_FAIL;
    }

    return EXEC_SUCCESS;
}

EXEC_STATUS getByte(char **strBuffer, unsigned char *out)
{
    long convNum;
    
    if((*strBuffer)[0] == '\0')
    {
        // String buffer is empty.
        printErrorMsg(CMD_MSG_PARAMETER_MISSING);
        return EXEC_FAIL;
    }

    convNum = strtol((*strBuffer), NULL, 0);
    if((convNum < 0) || (convNum > 0xFF))
    {
        // Value out-of-range!
        printErrorMsg(CMD_MSG_OUTOF_RANGE);
        return EXEC_FAIL;
    }

    *out = (unsigned char)convNum;
    return EXEC_SUCCESS;
}

EXEC_STATUS getVoltageLevel(char **strBuffer, unsigned char *out)
{
    if((*strBuffer)[0] == '\0')
    {
        // String buffer is empty.
        printErrorMsg(CMD_MSG_PARAMETER_MISSING);
        return EXEC_FAIL;
    }

    if((strcmp((*strBuffer), "3.3") == 0) || (strcmp((*strBuffer), "3.3V") == 0) || (strcmp((*strBuffer), "3.3v") == 0))
    {
        // Activate 3.3V I2C output voltage.
        *out = I2C_OUTPUT_3V3;
        return EXEC_SUCCESS;
    }
    else if((strcmp((*strBuffer), "5") == 0) || (strcmp((*strBuffer), "5.0V") == 0) || (strcmp((*strBuffer), "5.0v") == 0) 
    || (strcmp((*strBuffer), "5v") == 0) || (strcmp((*strBuffer), "5V") == 0))
    {
        // Activate 5.0V I2C output voltage.
        *out = I2C_OUTPUT_5V;
        return EXEC_SUCCESS;
    }

    // Unsupported read flag.
    printErrorMsg(CMD_PARAM_INVALID_VOLTAGE);
    return EXEC_FAIL;
}

EXEC_STATUS getReadStatus(char **strBuffer, unsigned char *out)
{
    if((*strBuffer)[0] == '\0')
    {
        // String buffer is empty.
        printErrorMsg(CMD_MSG_PARAMETER_MISSING);
        return EXEC_FAIL;
    }

    if((strcmp((*strBuffer), "ack") == 0) || (strcmp((*strBuffer), "1") == 0))
    {
        // Perform read and send ACK to the device.
        *out = 1;
        return EXEC_SUCCESS;
    }
    else if((strcmp((*strBuffer), "nack") == 0) || (strcmp((*strBuffer), "0") == 0))
    {
        // Perform read and send NACK to the device.
        *out = 0;
        return EXEC_SUCCESS;
    }
    
    // Unsupported read flag.
    printErrorMsg(CMD_PARAM_READ_UNSUPPORTED);
    return EXEC_FAIL;
}

unsigned char getCommand(unsigned char **cmdParam)
{
    char *inCmd = NULL;
    char *token = NULL;
    char *helpId = NULL;
    unsigned char returnStatus = CMD_STATUS_OK;
    char *cmdData[MAX_TOKEN_COUNT];
    unsigned char tokenPos;
    unsigned char paramVal;

    *cmdParam = NULL;

    // Getting input command from the user.
    rl_attempted_completion_function = cmdCompletion;
    while ((inCmd = readline("> ")) != NULL)
    {
        if(inCmd[0] == '\0')
        {
            // Ignore empty input data buffer.
            RELEASE_STR(inCmd);
            continue;
        }
        else
        {
            // Add input command to the history list.
            add_history(inCmd);
        }
        
        // Process input commands.
        if(strcmp(inCmd, "exit") == 0)
        {
            // EXIT command. Terminate the I2C terminal.
            returnStatus = CMD_STATUS_EXIT;
            break;
        }
        
        // Extract tokens into char array.
        token = strtok(inCmd, " ");
        tokenPos = 0;
        while(token != NULL)
        {
            cmdData[tokenPos++] = token;
            token = strtok(NULL, " ");

            // Check for maximum token limit.
            if(tokenPos >= MAX_TOKEN_COUNT)
            {
                break;
            }
        }

        // Execute commands based on the token values.
        if(strcmp(cmdData[0], "help") == 0)
        {
            // If available get the help topic ID from the parameter.
            helpId = (tokenPos >= 2) ? cmdData[1] : NULL;
            showHelp(&helpId);

            // Flush help command and continue for the next input.
            RELEASE_STR(inCmd);
            continue;
        }
        else if(strcmp(cmdData[0], "init") == 0)
        {
            // I2C session initialize with given speed.
            if(tokenPos < 2)
            {
                // Required paramaters are missing. INIT <SPEED>
                printCommandError(CMD_MSG_PARAMETER_MISSING, cmdData[0]);
                RELEASE_STR(inCmd);
                continue;
            }

            if(getSpeed(&(cmdData[1]), &paramVal) == EXEC_FAIL)
            {
                // Parameter value is invalid or not specified.
                RELEASE_STR(inCmd);
                continue;
            } 

            // Create HID feature buffer to send to the device.
            *cmdParam = createUSBBuffer(USB_CMD_I2C_INIT, paramVal); 
            break;          
        }
        else if(strcmp(cmdData[0], "start") == 0)
        {
            // Send I2C start command.
            if(tokenPos > 1)
            {
                // Parameters are not required for this command.
                printWarningMsg(CMD_PARAM_IGNORE);
            }

            // Create HID feature buffer to send to the device.
            *cmdParam = createUSBBuffer(USB_CMD_I2C_START, 0x00); 
            break; 
        }
        else if(strcmp(cmdData[0], "stop") == 0)
        {
            // Send I2C stop command.
            if(tokenPos > 1)
            {
                // Parameters are not required for this command.
                printWarningMsg(CMD_PARAM_IGNORE);
            }

            // Create HID feature buffer to send to the device.
            *cmdParam = createUSBBuffer(USB_CMD_I2C_STOP, 0x00); 
            break;
        }
        else if(strcmp(cmdData[0], "write") == 0)
        {
            // Write data into I2C bus.
            if(tokenPos < 2)
            {
                // Required paramaters are missing. WRITE <DATA>
                printCommandError(CMD_MSG_PARAMETER_MISSING, cmdData[0]);
                RELEASE_STR(inCmd);
                continue;
            }

            if(getByte(&(cmdData[1]), &paramVal) == EXEC_FAIL)
            {
                // Parameter value is invalid or not specified.
                RELEASE_STR(inCmd);
                continue;
            } 

            // Create HID feature buffer to send to the device.
            *cmdParam = createUSBBuffer(USB_CMD_I2C_WRITE, paramVal); 
            break;   
        }
        else if(strcmp(cmdData[0], "write-address") == 0)
        {
            // Write device address and read/write flag into I2C bus.
            if(tokenPos < 2)
            {
                // Required paramaters are missing. WRITE-ADDRESS <ADDRESS | READ/WRITE>
                printCommandError(CMD_MSG_PARAMETER_MISSING, cmdData[0]);
                RELEASE_STR(inCmd);
                continue;
            }

            if(getByte(&(cmdData[1]), &paramVal) == EXEC_FAIL)
            {
                // Parameter value is invalid or not specified.
                RELEASE_STR(inCmd);
                continue;
            } 

            // Create HID feature buffer to send to the device.
            *cmdParam = createUSBBuffer(USB_CMD_I2C_WRITE_ADDR, paramVal); 
            break;   
        }
        else if(strcmp(cmdData[0], "read") == 0)
        {
            // Read data from slave device.
            if(tokenPos >= 2)
            {
                // ACK/NACK flag is specified by the user?
                if(getReadStatus(&(cmdData[1]), &paramVal) == EXEC_FAIL)
                {
                    // Parameter value is invalid or not specified.
                    RELEASE_STR(inCmd);
                    continue;
                }
            }
            else
            {
                // If paramater is not specified use NACK as default.
                paramVal = 0;
            }

            // Create HID feature buffer to send to the device.
            *cmdParam = createUSBBuffer(USB_CMD_I2C_READ, paramVal); 
            break;            
        }
        else if(strcmp(cmdData[0], "output-voltage") == 0)
        {
            // Set output voltage of the I2C device.
            if(tokenPos < 2)
            {
                // Required paramaters are missing. WRITE-ADDRESS <ADDRESS | READ/WRITE>
                printCommandError(CMD_MSG_PARAMETER_MISSING, cmdData[0]);
                RELEASE_STR(inCmd);
                continue;
            }

            if(getVoltageLevel(&(cmdData[1]), &paramVal) == EXEC_FAIL)
            {
                // Parameter value is invalid or not specified.
                RELEASE_STR(inCmd);
                continue;
            }

            // Create HID feature buffer to send to the device.
            *cmdParam = createUSBBuffer(USB_CMD_SET_VOLTAGE, paramVal); 
            break; 
        }
        else if(strcmp(cmdData[0], "reset") == 0)
        {
            // Reset slave device and I2C registers to default values.
            if(tokenPos > 1)
            {
                // Parameters are not required for this command.
                printWarningMsg(CMD_PARAM_IGNORE);
            }

            // Create HID feature buffer to send to the device.
            *cmdParam = createUSBBuffer(USB_CMD_RESET, 0x00); 
            break;
        }
        else
        {
            // Unknown command!
            printCommandError(CMD_MSG_UNKNOWN, cmdData[0]);
            RELEASE_STR(inCmd);
            continue;
        }        
    }

    // Release input buffer.
    if(inCmd != NULL)
    {
        RELEASE_STR(inCmd);
    }

    return returnStatus;
}