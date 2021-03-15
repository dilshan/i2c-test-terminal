//----------------------------------------------------------------------------------
// I2C Test Terminal - Help / Documentation Processor.
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

#include "docuproc.h"
#include "termutil.h"
#include "strdoc.h"

#include <stdio.h>
#include <string.h>

void showGeneralHelp()
{
    printHelp(HELP_GEN_ABOUT);
    printHelp(HELP_GEN_CMD_HELP);
    printHelp(HELP_GEN_CMD_INIT);
    printHelp(HELP_GEN_CMD_START);
    printHelp(HELP_GEN_CMD_STOP);
    printHelp(HELP_GEN_CMD_WRITE);
    printHelp(HELP_GEN_CMD_WRITE_ADDR);
    printHelp(HELP_GEN_CMD_READ);
    printHelp(HELP_GEN_CMD_OUT_VOLTAGE);
    printHelp(HELP_GEN_CMD_RESET);
    printHelp(HELP_GEN_CMD_EXIT);

    printHelp(HELP_USE_HELP1);
    printHelp(HELP_USE_HELP2);
    printHelp(HELP_USE_HELP3);

    printHelp(HELP_AUTO_COMPLETE1);
    printHelp(HELP_AUTO_COMPLETE2);

    printHelp(HELP_GITHUB_REF1);
    printHelp(HELP_GITHUB_REF2);
}

void showHelp(char **topicId)
{
    if((topicId == NULL) || ((*topicId) == NULL) || (strcmp((*topicId), "help") == 0))
    {
        // Parameter for the help command is not available. Show General help.
        showGeneralHelp();
    }
    else
    {
        // Parameter is given with the help command.
        if(strcmp((*topicId), "init") == 0)
        {
            printHelpCmdFormat(HELP_INIT_FORMAT);
            printHelp(HELP_INIT_INTRO1);
            printHelp(HELP_INIT_INTRO2);

            printHelp(HELP_INIT_SPEED_100);
            printHelp(HELP_INIT_SPEED_250);
            printHelp(HELP_INIT_SPEED_400);

            printHelp(HELP_INIT_WARNING1);
            printHelp(HELP_INIT_WARNING2);
        }
        else if(strcmp((*topicId), "start") == 0)
        {
            printHelpCmdFormat(HELP_START_FORMAT);
            printHelp(HELP_START_INTRO1);
            printHelp(HELP_START_INTRO2);
            printHelp(HELP_START_INTRO3);
        }
        else if(strcmp((*topicId), "stop") == 0)
        {
            printHelpCmdFormat(HELP_STOP_FORMAT);
            printHelp(HELP_STOP_INTRO1);
            printHelp(HELP_STOP_INTRO2);
        }
        else if(strcmp((*topicId), "write") == 0)
        {
            printHelpCmdFormat(HELP_WRITE_FORMAT);
            printHelp(HELP_WRITE_INTRO1);
            printHelp(HELP_WRITE_INTRO2);
            printHelp(HELP_WRITE_INTRO3);
        }
        else if(strcmp((*topicId), "write-address") == 0)
        {
            printHelpCmdFormat(HELP_WRITE_ADDR_FORMAT);
            printHelp(HELP_WRITE_ADDR_INTRO1);
            printHelp(HELP_WRITE_ADDR_INTRO2);
            printHelp(HELP_WRITE_ADDR_INTRO3);
            printHelp(HELP_WRITE_ADDR_INTRO4);
        }
        else if(strcmp((*topicId), "read") == 0)
        {
            printHelpCmdFormat(HELP_READ_FORMAT);
            printHelp(HELP_READ_INTRO1);
            printHelp(HELP_READ_INTRO2);
            printHelp(HELP_READ_INTRO3);

            printHelp(HELP_READ_INT_VAL1);
            printHelp(HELP_READ_INT_VAL2);
            printHelp(HELP_READ_INT_VAL3);
        }
        else if(strcmp((*topicId), "output-voltage") == 0)
        {
            printHelpCmdFormat(HELP_SET_VOLTAGE_FORMAT);
            printHelp(HELP_SET_VOLTAGE_INTRO1);
            printHelp(HELP_SET_VOLTAGE_INTRO2);
            printHelp(HELP_SET_VOLTAGE_INTRO3);

            printHelp(HELP_SET_VOLTAGE_WARNING1);
            printHelp(HELP_SET_VOLTAGE_WARNING2);
            printHelp(HELP_SET_VOLTAGE_WARNING3);          
        }
        else if(strcmp((*topicId), "reset") == 0)
        {
            printHelpCmdFormat(HELP_RESET_FORMAT);
            printHelp(HELP_RESET_INTRO1);
            printHelp(HELP_RESET_INTRO2);

            printHelp(HELP_RESET_POWER1);
            printHelp(HELP_RESET_POWER2);
        }
        else if(strcmp((*topicId), "exit") == 0)
        {
            printHelpCmdFormat(HELP_EXIT_FORMAT);
            printHelp(HELP_EXIT_INTRO);
        }
        else
        {
            // Help for the specified parameter is not available, show the general help screen.
            showGeneralHelp();
        }
    }
}