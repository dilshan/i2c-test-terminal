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

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>

#include "usbdrv.h"

#include "i2ctester.h"
#include "i2cdrv.h"

PROGMEM const char usbHidReportDescriptor[22] = {    /* USB report descriptor */
    0x06, 0x00, 0xff,              //   USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                    //   USAGE (Vendor Usage 1)
    0xa1, 0x01,                    //   COLLECTION (Application)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x80,                    //   REPORT_COUNT (128)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)
    0xc0                           //   END_COLLECTION
};

int main()
{
    unsigned char connectDelay = 0;
    unsigned char outputVoltage = I2C_OUTPUT_3V3;
    
    // Initialize system registers and global variables.
    wdt_disable();
    initSystem();
    clearRequestBuffer();

    // Check device is connected to the USB host.
    if((PINA & 0x01) == 0x00)
    {
        // Device is not connected to the USB host, waiting for connection.
        do
        {
            // Blink Device Status LED and check USB sense pin.
            if(PORTB & 0x04)
            {
                PORTB &= 0xFB;
            }
            else
            {
                PORTB |= 0x04;
            }
            
            _delay_ms(250);
        } 
        while ((PINA & 0x01) == 0x00);
    }

    // USB device is connected to the host.
    PORTB |= 0x04;

    // Initialize V-USB driver.
    usbInit();
    usbDeviceDisconnect(); 

    while(--connectDelay)
    {
        _delay_ms(1);
    }

    usbDeviceConnect();
    sei();

    // Switch on lowest possible output voltage.
    setOutputVoltage(I2C_OUTPUT_3V3, &outputVoltage);
    
    // Main service loop.
    while(1)
    {
        // Check USB connection status.
        if((PINA & 0x01) == 0x00)
        {
            // Connection with USB host is not available, reset the device.
            PORTB &= 0xFB;

            // Activate WDT to perform device reset. 
            cli();
            wdt_enable(WDTO_15MS);
            while(1);
        }
        
        // Process pending USB messages and clear request buffer.
        if((reqBuffer[0] == SYS_SIGNATURE) && (reqBuffer[1] != USB_CMD_NONE))
        {
            switch(reqBuffer[1])
            {
            case USB_CMD_I2C_INIT:
                // Initialize I2C session.
                i2cInit(reqBuffer[2]);   // DATA0 - I2C communication speed (ref: i2cdrv.h)
                lastCommandStatus = RET_SUCCESS;
                break;
            case USB_CMD_I2C_START:
                // Send I2C start command.
                lastCommandStatus = i2cStart(usbPoll);
                break;
            case USB_CMD_I2C_STOP:
                // Send I2C stop command.
                i2cStop();
                lastCommandStatus = RET_SUCCESS;           
                break;
            case USB_CMD_I2C_WRITE_ADDR:
                // Send I2C write address command.
                lastCommandStatus = i2cWriteAddr(usbPoll, reqBuffer[2]);  // DATA0 - slave device address and read/write flag.
                break;
            case USB_CMD_I2C_WRITE:
                // Send I2C write command.
                lastCommandStatus = i2cWrite(usbPoll, reqBuffer[2]);  // DATA0 - data to write into slave device.
                break;
            case USB_CMD_I2C_READ:
                // Send I2C read command.
                lastCommandStatus = i2cRead(usbPoll, reqBuffer[2], &lastCommandData); // DATA0 - ACK status for read command.            
                break;
            case USB_CMD_SET_VOLTAGE:
                // Set I2C output voltage.                
                lastCommandStatus = setOutputVoltage(reqBuffer[2], &outputVoltage); // DATA0 - Voltage level, 0x01 - 5V; 0x02 - 3.3V;
                break;
            case USB_CMD_GET_VOLTAGE:
                // Get current I2C output voltage.
                lastCommandData = outputVoltage;
                lastCommandStatus = RET_SUCCESS;
                break;
            case USB_CMD_RESET:
                // Reset I2C slave device and I2C terminal registers.
                lastCommandStatus = resetI2CSlave(outputVoltage);
                break;            
            default:
                // Unknown command ID.
                lastCommandStatus = RET_UNKNOWN;
            }

            // Clear request buffer.
            clearRequestBuffer();
        }
        
        // Process USB messages received from the host.
        usbPoll();
    }

    return 0;
}

void initSystem()
{   
    // PORTB.0 [OUT] - 5V Control Terminal.
    // PORTB.1 [OUT] - 3.3V Control Terminal.
    // PORTB.2 [OUT] - Device Status LED.
    DDRB = 0x07;
    PORTB = I2C_OUTPUT_3V3;

    // Shutdown ADC and set PORTA.0 as USB sense pin.
    ADMUX = 0x00;
    ADCSRA = 0x00;
    ACSR = 0x80;

    DDRA = 0x00;
    PORTA = 0x00;
    
    // Initialize global variables.
    lastCommandStatus = RET_SUCCESS;
    lastCommandData = 0x00;
    lastCommand = USB_CMD_NONE;
}

void clearRequestBuffer()
{
    reqBuffer[0] = 0x00;
    reqBuffer[1] = USB_CMD_NONE;
    reqBuffer[2] = 0x00;
    reqBuffer[3] = 0x00;
}

unsigned char usbFunctionRead(unsigned char *data, unsigned char len)
{
    // Response data format:
    // SIGNATURE | COMMAND | STATUS | DATA | END SIGNATURE
    data[0] = SYS_SIGNATURE;
    data[1] = lastCommand;
    data[2] = lastCommandStatus;
    data[3] = lastCommandData;
    data[4] = SYS_END_SIGNATURE;

    return 4;
}

unsigned char usbFunctionWrite(unsigned char *data, unsigned char len)
{
    // Received command format: 
    // SIGNATURE | COMMAND | DATA BYTE | END SIGNATURE
    if((len > 4)  && (data[0] == SYS_SIGNATURE))
    {
        // Reset last command variables.
        lastCommandStatus = RET_PENDING;
        lastCommandData = 0x00;
        lastCommand = data[1];
        
        // Copy received data into request buffer.
        reqBuffer[0] = data[0];
        reqBuffer[1] = data[1];
        reqBuffer[2] = data[2];
        reqBuffer[3] = data[3];
    }
    
    // End of the data chunk.
    return 1;
}

usbMsgLen_t usbFunctionSetup(unsigned char data[8])
{
    usbRequest_t *request = (void*)data;

    if((request->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS)
    {
        // HID class request.
        if(request->bRequest == USBRQ_HID_GET_REPORT)
        {
            // use usbFunctionRead to obtain data.
            return USB_NO_MSG;
        }
        else if(request->bRequest == USBRQ_HID_SET_REPORT)
        {
            // use usbFunctionWrite to receive data from host.
            return USB_NO_MSG;
        }        
    }

    return 0;
}

unsigned char setOutputVoltage(unsigned char voltage, unsigned char *newVoltage)
{
    unsigned char countdown = 0xFF;
    
    // Shutdown both 3.3V and 5V output lines.
    PORTB &= 0xFC;

    // Wait to stable the output terminals.
    while(--countdown)
    {
        usbPoll();
        _delay_us(I2C_DELAY_DELTA);
    }

    // Set new voltage level.
    if((voltage == I2C_OUTPUT_3V3) || (voltage == I2C_OUTPUT_5V))
    {
        PORTB |= voltage;
        *newVoltage = voltage;
        return RET_SUCCESS;
    }

    // Voltage settings are invalid.
    return RET_UNKNOWN;
}

unsigned char resetI2CSlave(unsigned char voltage)
{
    unsigned char countdown = 0x96;
    
    // Shutdown both 3.3V and 5V output lines.
    PORTB &= 0xFC;

    // Reset all I2C registers to default values.
    TWBR = 0x00;
    TWCR = 0x00;
    TWSR = 0xF8;
    TWDR = 0xFF;
    TWAR = 0xFE;

    // Wait to stable the output terminals.
    while(--countdown)
    {
        usbPoll();
        _delay_us(I2C_DELAY_DELTA);
    }

    // Restore voltage to the slave device.
    PORTB |= voltage;
    return RET_SUCCESS;
}