/*************************************************** 
  This is an example for the Adafruit STMPE610 Resistive
  touch screen controller breakout
  ----> http://www.adafruit.com/products/1571
 
  Check out the links above for our tutorials and wiring diagrams
  These breakouts use SPI or I2C to communicate

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include <SPI.h>
#include <Wire.h>

#include "Adafruit_STMPE610.h"

// Pick one of three wiring options below!

// Option #1 - uses I2C, connect to hardware I2C port only!
// SCL to I2C clock (#A5 on Uno) and SDA to I2C data (#A4 on Uno)
// tie MODE to GND and POWER CYCLE (there is no reset pin)
Adafruit_STMPE610 touch = Adafruit_STMPE610();

// Option #2 - use hardware SPI, connect to hardware SPI port only!
// SDI to MOSI, SDO to MISO, and SCL to SPI CLOCK
// on Arduino Uno, that's 11, 12 and 13 respectively
// Then pick a CS pin, any pin is OK but we suggest #10 on an Uno
// tie MODE to 3.3V and POWER CYCLE the STMPE610 (there is no reset pin)

//Adafruit_STMPE610 touch = Adafruit_STMPE610(STMPE_CS);

// Option #3 - use software SPI, connect to *any* 4 I/O pins!
// define the following pins to whatever 4 you want and wire up!
// Tie MODE to 3.3V and POWER CYCLE the STMPE610 (there is no reset pin)
// Adafruit_STMPE610 touch = Adafruit_STMPE610(STMPE_CS, STMPE_SDI, STMPE_SDO, STMPE_SCK);

/******************/

void setup() { 
  Serial.begin(9600);
  Serial.println("Adafruit STMPE610 example");
  Serial.flush();

  // if using hardware SPI on an Uno #10 must be an output, remove line
  // if using software SPI or I2C
  pinMode(10, OUTPUT);

  // If using I2C you can select the I2C address (there are two options) by calling
  // touch.begin(0x41), the default, or touch.begin(0x44) if A0 is tied to 3.3V
  // If no address is passed, 0x41 is used
  if (! touch.begin()) {
    Serial.println("STMPE not found!");
    while(1);
  }
  Serial.println("Waiting for touch sense");
}

void loop() {
  uint16_t x, y;
  uint8_t z;
  if (touch.touched()) {
    // read x & y & z;
    while (! touch.bufferEmpty()) {
      Serial.print(touch.bufferSize());
      touch.readData(&x, &y, &z);
      Serial.print("->("); 
      Serial.print(x); Serial.print(", "); 
      Serial.print(y); Serial.print(", "); 
      Serial.print(z);
      Serial.println(")");
    }
    touch.writeRegister8(STMPE_INT_STA, 0xFF); // reset all ints, in this example unneeded depending in use
  }
  delay(10);
}

