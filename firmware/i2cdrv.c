//----------------------------------------------------------------------------------
// I2C Test Terminal Firmware - I2C Driver.
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
#include <util/twi.h>
#include <util/delay.h>

#include "i2cdrv.h"

#define TWI_SCL PORTC0
#define TWI_SDA PORTC1

void i2cInit(unsigned char comSpeed)
{
    // Setup I/O pin for I2C with pull-ups.
    DDRC |= ((1 << TWI_SDA) | (1 << TWI_SCL));
    PORTC |= ((1 << TWI_SDA) | (1 << TWI_SCL));
    DDRC  &= ~((1 << TWI_SDA) | (1 << TWI_SCL));

    // Limit speed configurations between 100kHz to 400kHz.
    if(comSpeed > TWI_COM_SPEED_400)
    {
        // Specified speed setting is not supported and switch to default speed.
        comSpeed = TWI_COM_SPEED_100;
    }

    // Set prescaler to 1.
    TWSR &= ~((1 << TWPS1) | (1 << TWPS0));

    // Setup communication speed.
    switch(comSpeed)
    {
    case TWI_COM_SPEED_100:        
        // I2C clock speed is set to 100kHz.
        TWBR = 72;
        break;
    case TWI_COM_SPEED_250:
        // I2C clock speed is set to 250kHz.
        TWBR = 24;
        break;
    case TWI_COM_SPEED_400:
        // I2C clock speed is set to 400kHz.
        TWBR = 12;    
        break;
    }    
}

unsigned char i2cStart(void (*usbProc)(void))
{
    unsigned short timeout = 0;
    
    // Send START condition to the slave device.
    TWCR =  ((1 << TWINT) | (1 << TWEN) | (1 << TWSTA));

    while (!(TWCR & (1 << TWINT)))
    {
        if((++timeout) >= I2C_TIMEOUT)
        {
            // Delay timeout has occured.
            return RET_TIMEOUT_FAIL;
        }

        _delay_us(I2C_DELAY_DELTA);
        (*usbProc)();
    }

    // Send I2C status to the host system.
    return TW_STATUS;
}

unsigned char i2cWriteAddr(void (*usbProc)(void), unsigned char addr)
{
    unsigned short timeout = 0;
    
    // Set device address with read/write flag.
    TWDR = addr;
    TWCR = ((1 << TWINT) | (1 << TWEN));

    while (!(TWCR & (1 << TWINT)))
    {
        if((++timeout) >= I2C_TIMEOUT)
        {
            // Delay timeout has occured.
            return RET_TIMEOUT_FAIL;
        }

        _delay_us(I2C_DELAY_DELTA);
        (*usbProc)();
    }

    // Send I2C status to the host system.
    return TW_STATUS;
}

unsigned char i2cWrite(void (*usbProc)(void), unsigned char data)
{
    unsigned short timeout = 0;
    
    // Write specified data into slave device.
    TWDR = data;
    TWCR = ((1 << TWINT) | (1 << TWEN));

    while (!(TWCR & (1 << TWINT)))
    {
        if((++timeout) >= I2C_TIMEOUT)
        {
            // Delay timeout has occured.
            return RET_TIMEOUT_FAIL;
        }

        _delay_us(I2C_DELAY_DELTA);
        (*usbProc)();
    }

    // Send I2C status to the host system.
    return TW_STATUS;
}

unsigned char i2cRead(void (*usbProc)(void), unsigned char ack, unsigned char *data)
{
    unsigned short timeout = 0;
    *data = 0;

    if(ack)
    {
        // Send ACK after the successful read.
        TWCR = ((1 << TWINT) | (1 << TWEN) | (1 << TWEA));
        
        while (!(TWCR & (1 << TWINT)))
        {
            if((++timeout) >= I2C_TIMEOUT)
            {
                // Delay timeout has occured.
                return RET_TIMEOUT_FAIL;
            }

            _delay_us(I2C_DELAY_DELTA);
            (*usbProc)();
        }

        *data = TWDR;

        // Send I2C status to the host system.
        return TW_STATUS;
    }
    else
    {
        // ACK is not sent after the successful read.
        TWCR = ((1 << TWINT) | (1 << TWEN));
        while (!(TWCR & (1 << TWINT)))
        {
             if((++timeout) >= I2C_TIMEOUT)
            {
                // Delay timeout has occured.
                return RET_TIMEOUT_FAIL;
            }

            _delay_us(I2C_DELAY_DELTA);
            (*usbProc)();           
        }

        *data = TWDR;

        // Send I2C status to the host system.
        return TW_STATUS;
    }
}

void i2cStop()
{
    // Send STOP condition to the device / bus.
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}