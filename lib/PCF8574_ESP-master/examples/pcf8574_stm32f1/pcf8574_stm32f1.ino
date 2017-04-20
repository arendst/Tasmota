/*  Example sketch for the PCF8574 for the purposes of showing how to use the interrupt-pin.

    Attach the positive lead of an LED to PIN7 on the PCF8574 and the negative lead to GND,
    a wire from PC13 to pin 3 on the PCF8474, a wire from the int-pin on the PCF8574 to PC15
    and wires for SDA and SCL according to your STM32F1-module.

    If all goes well you should see the small blue LED on the ESP-module lighting up and the
    LED connected to the PCF going off, and vice versa. */

#include <pcf8574_esp.h>

/*  We need to set up the I2C-bus for the library to use */
#include <HardWire.h>
/*  Use I2C-bus 1
    Specsheets say PCF8574 is officially rated only for 100KHz I2C-bus */
HardWire HWire(1, I2C_BUS_RESET);
/*  PCF8575 is rated for 400KHz
HardWire HWire(1, I2C_FAST_MODE | I2C_BUS_RESET); */

// Initialize a PCF8574 at I2C-address 0x20, using GPIO5, GPIO4 and testWire for the I2C-bus
PCF857x pcf8574(0x20, &HWire);
//If you had a PCF8575 instead you'd use the below format
//PCF857x pcf8575(0x20, &Hwire, true);

bool PCFInterruptFlag = false;

void PCFInterrupt() {
  PCFInterruptFlag = true;
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("Firing up..."));
  pinMode(PC13, OUTPUT);

  HWire.begin();
  pcf8574.begin();
  // Most ready-made PCF8574-modules seem to lack an internal pullup-resistor, so you have to use the MCU-internal one.
  pinMode(PC15, INPUT_PULLUP);
  pcf8574.resetInterruptPin();

  attachInterrupt(PC15, PCFInterrupt, FALLING);
}

void loop() {
  if(PCFInterruptFlag){
    Serial.println(F("Got an interrupt: "));
    if(pcf8574.read(3)==1) Serial.println("Pin 3 is HIGH!");
    else Serial.println("Pin 3 is LOW!");
    // DO NOTE: When you write LOW to a pin on a PCF8574 it becomes an OUTPUT.
    // It wouldn't generate an interrupt if you were to connect a button to it that pulls it HIGH when you press the button.
    // Any pin you wish to use as input must be written HIGH and be pulled LOW to generate an interrupt.
    pcf8574.write(7, pcf8574.read(3));
    PCFInterruptFlag=false;
  }
  Serial.println(F("Blink."));
  if(digitalRead(PC13)==HIGH) digitalWrite(PC13, LOW);
  else digitalWrite(PC13, HIGH);
  delay(1000);
}
