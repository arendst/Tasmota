/*
    demo_terminal.ino
    Description: A terminal for Seeed Grove Multichannel gas sensor V2.0.
    2019 Copyright (c) Seeed Technology Inc.  All right reserved.
    Author: Hongtai Liu(lht856@foxmail.com)
    2019-6-18

    modify: delete GM402B GM802B moudle.
          2019-9-29

    The MIT License (MIT)
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.1  USA
*/

#include <Multichannel_Gas_GMXXX.h>

// if you use the software I2C to drive the sensor, you can uncommnet the define SOFTWAREWIRE which in Multichannel_Gas_GMXXX.h.
#ifdef SOFTWAREWIRE
    #include <SoftwareWire.h>
    SoftwareWire myWire(3, 2);
    GAS_GMXXX<SoftwareWire> gas;
#else
    #include <Wire.h>
    GAS_GMXXX<TwoWire> gas;
#endif

static uint8_t recv_cmd[8] = {};

void setup() {
    Serial.begin(9600);
    // If you have changed the I2C address of gas sensor, you must to be specify the address of I2C.
    //The default addrss is 0x08;
    gas.begin(Wire, 0x08); // use the hardware I2C
    //gas.begin(MyWire, 0x08); // use the software I2C
    printMenu();
}

void loop() {
    uint8_t len = 0;
    uint8_t addr = 0;
    uint8_t i;
    uint32_t val = 0;
    if (Serial.available()) {
        char chr = '\0';
        while (chr != '\n') { // Blockly read data from serial monitor
            chr = Serial.read();
            // Serial.print(chr);
            recv_cmd[len++] = chr;
        }
    }

    if (len > 0) {
        switch (recv_cmd[0]) {
            case '1': val = gas.getGM102B(); Serial.print("GM102B: "); Serial.print(val); Serial.print("  =  ");
                Serial.print(gas.calcVol(val)); Serial.println("V"); break;
            case '2': val = gas.getGM302B(); Serial.print("GM302B: "); Serial.print(val); Serial.print("  =  ");
                Serial.print(gas.calcVol(val)); Serial.println("V"); break;
            case '3': val = gas.getGM502B(); Serial.print("GM502B: "); Serial.print(val); Serial.print("  =  ");
                Serial.print(gas.calcVol(val)); Serial.println("V"); break;
            case '4': val = gas.getGM702B(); Serial.print("GM702B: "); Serial.print(val); Serial.print("  =  ");
                Serial.print(gas.calcVol(val)); Serial.println("V"); break;
            case 'D': Serial.println("Unpreheated！The next read will automatically preheated！"); break;
            case 'M': //the cmd M100 mean that change the sensor I2C address to 100(0x64);
                for (i = 1; recv_cmd[i + 1] != '\n'; i++) {
                    addr *= 10;
                    addr += (recv_cmd[i] - '0');
                }
                if (addr == 0 || addr > 127) {
                    Serial.println("the address must to be between 1 and 127"); break;
                } else {
                    gas.changeGMXXXAddr(addr);
                    Serial.println("change the sensor I2C address to:"); Serial.println(addr); break;
                }
            default: printMenu(); break;
        }
        // clean data buffer
        for (i = 0; i < sizeof(recv_cmd); i++) {
            recv_cmd[i] = '\0';
        }
    }
    delay(100);
}

void printMenu() {
    Serial.println(
        "----------------Multichannel_gas_sensor V2.0----------------\n"
        "         1: get the value of GM102B.                        \n"
        "         2: get the value of GM302B.                        \n"
        "         3: get the value of GM502B.                        \n"
        "         4: get the value of GM702B.                        \n"
        "         D: unpreheated the Multichannel_gas_sensor.        \n"
        "M[address]: change the Multichannel_gas_sensor I2C address. \n"
        " other key: print the meun.                                 \n"
        "------------------------------------------------------------\n"
    );
}
