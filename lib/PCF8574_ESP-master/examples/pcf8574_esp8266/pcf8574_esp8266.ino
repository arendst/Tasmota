/*  Example sketch for the PCF8574 for the purposes of showing how to use the interrupt-pin.

    Attach the positive lead of an LED to PIN7 on the PCF8574 and the negative lead to GND,
    a wire from GPIO2 (Nodemcu D4) to PIN3 that will be used to trigger the interrupt,
    and the INT-pin to GPIO14 (Nodemcu D5) on the ESP8266.

    If all goes well you should see the small blue LED on the ESP-module lighting up and the
    LED connected to the PCF going off, and vice versa. */

#include <pcf8574_esp.h>
#include <ESP8266WiFi.h>
#include <Wire.h>

/*  Wire.h already defines "Wire" which the PCF8574-class would use by default,
    but for the sakes of an example let's define our own instance of it and use that instead!

    Also, since I2C is emulated on the ESP8266 let's redefine what pins to use as SDA and SCL
    and instead swap them around!
    DO NOT FORGET TO WIRE ACCORDINGLY, SDA GOES TO GPIO5, SCL TO GPIO4 (ON NODEMCU GPIO5 IS D1 AND GPIO4 IS D2) */
TwoWire testWire;
// Initialize a PCF8574 at I2C-address 0x20, using GPIO5, GPIO4 and testWire for the I2C-bus
PCF857x pcf8574(0x20, &testWire);

//If you had a PCF8575 instead you'd use the below format
//PCF857x pcf8575(0x20, &testWire, true);

bool PCFInterruptFlag = false;

void ICACHE_RAM_ATTR PCFInterrupt() {
  PCFInterruptFlag = true;
}

void setup() {
  WiFi.persistent(false);
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  delay(1);
  // The above lines turn WiFi off for lower power-consumption, since we're not using it in this sketch.
  // Remove them if you want to modify the sketch and use WiFi.

  Serial.begin(115200);
  Serial.println("Firing up...");
  pinMode(2, OUTPUT);

  testWire.begin(5, 4);
  //Specsheets say PCF8574 is officially rated only for 100KHz I2C-bus
  //PCF8575 is rated for 400KHz
  testWire.setClock(100000L);
  pcf8574.begin();
  // Most ready-made PCF8574-modules seem to lack an internal pullup-resistor, so you have to use the ESP8266-internal one.
  pinMode(14, INPUT_PULLUP);
  pcf8574.resetInterruptPin();
  attachInterrupt(digitalPinToInterrupt(14), PCFInterrupt, FALLING);
}

void loop() {
  if(PCFInterruptFlag){
    Serial.println("Got an interrupt: ");
    if(pcf8574.read(3)==HIGH) Serial.println("Pin 3 is HIGH!");
    else Serial.println("Pin 3 is LOW!");
    // DO NOTE: When you write LOW to a pin on a PCF8574 it becomes an OUTPUT.
    // It wouldn't generate an interrupt if you were to connect a button to it that pulls it HIGH when you press the button.
    // Any pin you wish to use as input must be written HIGH and be pulled LOW to generate an interrupt.
    pcf8574.write(7, pcf8574.read(3));
    PCFInterruptFlag=false;
  }
  Serial.println("Blink.");
  if(digitalRead(2)==HIGH) digitalWrite(2, LOW);
  else digitalWrite(2, HIGH);
  delay(1000);
}
