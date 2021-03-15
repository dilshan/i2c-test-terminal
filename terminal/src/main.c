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

#include "main.h"
#include "strdef.h"
#include "termutil.h"
#include "cmdproc.h"

#include <linux/types.h>
#include <linux/input.h>
#include <linux/hidraw.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <termios.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h> 
#include <ctype.h>

int main()
{
    struct udev *udev;
    struct UsbComData comIntf;
    char *hidDevPath;
    unsigned char *cmdData;
    EXEC_STATUS status;
    pthread_t devThread;
    int termHandler;
    unsigned char currentVoltage, refreshVoltage;
        
    // Try to find the I2C terminal device on udev. If available get the device path.
    udev = udev_new();
    status = getTerminalDevicePath(udev, &hidDevPath);
    udev_unref(udev);

    if(status == EXEC_FAIL)
    {
        // Unable to find the device or udev error.
        printErrorMsg(DEV_NOT_AVAILABLE);
        return 1;
    }    

    // Open USB device for communication.
    termHandler = open(hidDevPath, (O_RDWR | O_NONBLOCK));
    if(termHandler < 0)
    {
        printErrorMsg(DEV_NOT_OPEN);
        return 1;
    }

    // Display intro message(s).
    printf(MSG_INTRO_NAME);
    printf(MSG_INTRO_HELP);

    // Get current output voltage from the device.
    currentVoltage = 0;
    refreshVoltage = 0;
    if(getCurrentOutputVoltage(termHandler, &currentVoltage) == EXEC_SUCCESS)
    {
        // Current output voltage received from the device.
        printf(MSG_OUTPUT_VOLTAGE, (currentVoltage == I2C_OUTPUT_5V) ? "5.0" : "3.3");
    }
    else
    {
        // Output voltage probe request is fail.
        printErrorMsg(DEV_COM_OUTPUT_VOLTAGE_FAIL);
        return 1;
    }
    
    // Get commands from the user.
    while(getCommand(&cmdData) == CMD_STATUS_OK)
    {
        if(cmdData != NULL)
        {            
            // Confirmation is required to change the I2C output voltage.
            if(cmdData[1] == USB_CMD_SET_VOLTAGE)
            {                
                // Check specified voltage is same as the current voltage level.
                if(cmdData[2] != currentVoltage)
                {
                    // Specified voltage level is different from current voltage level.
                    if(isContinue(PROMPT_VOLTAGE_CHANGE) == EXEC_FAIL)
                    {
                        // Voltage change is canceled by the user.
                        free(cmdData);
                        cmdData = NULL;

                        // No change required, skip USB data submission.
                        continue;
                    }
                }
                else
                {
                    // Specified voltage level is equal to the current voltage level.
                    printWarningMsg(CMD_VOLTAGE_SAME);

                    // Release command buffer.
                    free(cmdData);
                    cmdData = NULL;

                    // No change required, skip USB data submission.
                    continue;
                }

                // After the command execute get the voltage level from the device.
                refreshVoltage = 1;
            }

            // Execute command available in the data buffer.
            comIntf.comData = cmdData;
            comIntf.deviceHandler = termHandler;
            pthread_create(&devThread, NULL, sendDataToDevice, (void*)&comIntf);

            // Wait to finish the USB command.
            pthread_join(devThread, NULL);

            // Release command buffer.
            free(cmdData);
            cmdData = NULL;

            if(refreshVoltage)
            {
                // Need to refresh the voltage level of the system. (associated with voltage-change and reset commands.)
                refreshVoltage = 0;

                if(getCurrentOutputVoltage(termHandler, &currentVoltage) == EXEC_SUCCESS)
                {
                    // Current output voltage received from the device.
                    printf(MSG_OUTPUT_VOLTAGE, (currentVoltage == I2C_OUTPUT_5V) ? "5.0" : "3.3");
                }
                else
                {
                    // Output voltage probe request is fail.
                    printErrorMsg(DEV_COM_OUTPUT_VOLTAGE_FAIL);
                    return 1;
                }
            }
        }
    }

    // Release command buffer.
    if(cmdData != NULL)
    {
        free(cmdData);
        cmdData = NULL;
    }

    // Close USB device handler and terminate the application.
    close(termHandler);
    return 0;
}

EXEC_STATUS isContinue(const unsigned char *msg)
{
    char getConfirmation;
    static struct termios conConfigOld, conConfigNew;

    // Print specified message on the terminal.
    printf("%s (Y/N) ? ", msg);

    // Cancel character printing on the termina.
    tcgetattr(0, &conConfigOld);
    conConfigNew = conConfigOld;
    conConfigNew.c_lflag &= ~ICANON;
    conConfigNew.c_lflag &= ~ECHO;
    tcsetattr(0, TCSANOW, &conConfigNew);    

    // Loop until user specify the correct input.
    do
    {
        getConfirmation = getchar();
    } 
    while (!((getConfirmation == 'y') || (getConfirmation == 'Y') || (getConfirmation == 'n') || (getConfirmation == 'N')));

    // Restore terminal settings (to print characters on screen).
    tcsetattr(0, TCSANOW, &conConfigOld);

    // Print user selection and move to next line.
    printf("%c\n", toupper(getConfirmation));

    // Return success on "YES", otherwise return fail.
    return ((getConfirmation == 'y') || (getConfirmation == 'Y')) ? EXEC_SUCCESS : EXEC_FAIL;
}

EXEC_STATUS getCurrentOutputVoltage(int deviceHandler, unsigned char *voltage)
{
    unsigned char *reqData, *respData;
    struct timespec req, rem;
    int status;
    unsigned char result = EXEC_FAIL;

    // Create request buffer and send it to the device.
    reqData = createUSBBuffer(USB_CMD_GET_VOLTAGE, 0);
    respData = NULL;

    status = ioctl(deviceHandler, HIDIOCSFEATURE(USB_SET_COMMAND_BUFFER_SIZE), reqData);
    if(status >= 0)
    {
        // IOCTL is successful, creating data buffer to capture the output from the device.
        respData = (unsigned char*)calloc(USB_GET_DATA_BUFFER_SIZE, 1);
        while(ioctl(deviceHandler, HIDIOCGFEATURE(USB_GET_DATA_BUFFER_SIZE), respData) >= 0)
        {
            if((respData[1] == SYS_SIGNATURE) && (respData[2] == reqData[1]) && (respData[3] != RET_PENDING))
            {
                // Device respond with data / status.
                *voltage = respData[4];
                result = EXEC_SUCCESS;
                break;
            }
            
            // Wait thread for 250ms to get the next feature report.
            req.tv_sec = 0;
            req.tv_nsec = 250 * 1000000; 
            nanosleep(&req , &rem);

            memset(respData, 0, USB_GET_DATA_BUFFER_SIZE);
        }
    }
    
    // Release all allocated data buffers.
    free(reqData);
    reqData = NULL;

    if(respData != NULL)
    {
        free(respData);
        respData = NULL;
    }

    return result;
}

void *sendDataToDevice(void *dataPtr)
{
    struct UsbComData *comData = (struct UsbComData *)dataPtr;
    struct timespec req, rem;
    int status;
    unsigned char readBuffer[USB_GET_DATA_BUFFER_SIZE];

    // Send specified USB data buffer to the device.
    status = ioctl(comData->deviceHandler, HIDIOCSFEATURE(USB_SET_COMMAND_BUFFER_SIZE), comData->comData);
    if(status < 0)
    {
        // Communication failure has occur while setting up the feature report.
        printErrorMsg(DEV_COM_FAIL);
    }
    else
    {
        // IOCTL is successful, waiting for response from the device.
        memset(readBuffer, 0, USB_GET_DATA_BUFFER_SIZE);
        while(ioctl(comData->deviceHandler, HIDIOCGFEATURE(USB_GET_DATA_BUFFER_SIZE), readBuffer) >= 0)
        {            
            if((readBuffer[1] == SYS_SIGNATURE) && (readBuffer[2] == comData->comData[1]) && (readBuffer[3] != RET_PENDING))
            {
                // Device respond with data / status.
                break;
            }

            // Wait thread for 250ms to get the next feature report.
            req.tv_sec = 0;
            req.tv_nsec = 250 * 1000000; 
            nanosleep(&req , &rem);

            memset(readBuffer, 0, USB_GET_DATA_BUFFER_SIZE);
        }

        // print received data and status on terminal.
        printDeviceStatusMsg(readBuffer[3]);

        // On READ command show received data.            
        if(readBuffer[2] == USB_CMD_I2C_READ)
        {
            printData(readBuffer[4]);
        }
    }    
}

EXEC_STATUS getTerminalDevicePath(struct udev *udev, char **hidRawPath)
{
    EXEC_STATUS returnVal;
    struct udev_enumerate *devEnum;
    struct udev_list_entry *devices, *devEntry;
    struct udev_device *rawDev, *hidDev, *usbDev;
    const char *sysfsPath, *devPath, *devVID, *devPID;
    int parseStatus;
    unsigned short vid, pid;    

    *hidRawPath = NULL;
    returnVal = EXEC_FAIL;

    // Perform device scan on HID-RAW device class.
    devEnum = udev_enumerate_new(udev);
    udev_enumerate_add_match_subsystem(devEnum, "hidraw");
    udev_enumerate_scan_devices(devEnum);

    // Get the list of HID devices attached to the system.
    devices = udev_enumerate_get_list_entry(devEnum);

    // Check for the VID and PID to identify the I2C terminal device entry.
    udev_list_entry_foreach(devEntry, devices)
    {
        devVID = NULL;
        devPID = NULL;
        vid = 0;
        pid = 0;
        
        // Get devices HID RAW udev node.
        sysfsPath = udev_list_entry_get_name(devEntry);
        rawDev = udev_device_new_from_syspath(udev, sysfsPath);

        // Get devices HID udev node.
        devPath = udev_device_get_devnode(rawDev);
        hidDev = udev_device_get_parent_with_subsystem_devtype(rawDev, "hid", NULL);

        if(hidDev)
        {
            // Get USB device class to extract VID and PID values.
            usbDev = udev_device_get_parent_with_subsystem_devtype(rawDev, "usb", "usb_device");
            if(usbDev)
            {
                devVID = udev_device_get_sysattr_value(usbDev, "idVendor");
                devPID = udev_device_get_sysattr_value(usbDev, "idProduct");

                // Get vendor ID of the selected device.
                if((devVID) && (strlen(devVID) > 0))
                {
                    vid = strtol(devVID, NULL, 16);
                }

                // Get product ID of the selected device.
                if((devPID) && (strlen(devPID) > 0))
                {
                    pid = strtol(devPID, NULL, 16);
                }

                // Check for valid VID and PID.
                if((vid == I2C_TERMINAL_DEV_VID) && (pid == I2C_TERMINAL_DEV_PID))
                {
                    // Copy HID-RAW device path into specified variable.
                    *hidRawPath = (char*) malloc(strlen(devPath) + 1);
                    strcpy(*hidRawPath, devPath);

                    returnVal = EXEC_SUCCESS;
                }

                free((char*) devVID);
                free((char*) devPID);

                udev_device_unref(usbDev);
            }
        }

        free((char*) devPath);
    }

    // Cleanup allocated data structures.
    udev_enumerate_unref(devEnum);

    return returnVal;
}