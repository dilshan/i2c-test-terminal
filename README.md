# I2C Master Mode Emulator

The I2C master mode emulator allows communication with I2C devices by sending or receiving data to/from the I2C bus. To issue the I2C commands, the emulator should connect to a PC over the USB port. After initializing the emulator, the PC and directly control the I2C slave chip/module.

This emulator is base on [ATmega16A](https://www.microchip.com/wwwproducts/en/ATmega16A) MCU. The USB communication channel is develop using the [V-USB](https://www.obdev.at/products/vusb/index.html) firmware.

To simplify the assembly, the PCB of this emulator is designed on a single-side board. The dimensions of the PCB are 96.77mm × 110.73mm. All the parts used in this project are through-hole-type, generally available components.

This emulator needs an external power supply, and the recommended supply voltage is between 12V - 15V.

The control software of the emulator is developed using libusb and tested only with *Linux* operating systems. The current firmware and control software support I2C emulation on 100kHz, 250kHz, and 400kHz clock rates.

Initially, we develop this emulator to work with 5V I2C devices, but later it has extended to work with 3.3V I2C devices. The 3.3V design is still under testing, and at the prototyping stage, we found a couple of issues in 3.3V mode.

In 3.3V mode, the required output level is available only if the emulator is in a "*ready*" state. In all the other states, it provides 5V output. **Therefore in 3.3V mode, necessary precautions need to be taken to protect the slave device from over-voltage damages.**

This project is an open-source hardware project. All the content of this project are distributed under the terms of the following license:

-   Hardware License: [CC BY 4.0](https://creativecommons.org/licenses/by/4.0/)
-   Software and firmware License: [MIT](https://github.com/dilshan/max2-audio-dac/blob/master/LICENSE)
-   [Documentation](https://github.com/dilshan/max2-audio-dac/wiki) License: [CC BY 4.0](https://creativecommons.org/licenses/by/4.0/)
