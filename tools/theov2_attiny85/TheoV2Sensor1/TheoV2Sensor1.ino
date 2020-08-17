/****************************************************************************************************************************\
* Arduino project "TheoTinySensor" Copyright 2013 Theo Arends 
* This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License 
* as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty 
* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
******************************************************************************************************************************

/***************************************************************************************************************\
* Purpose        : Arduino Source code for Sensor based on Atmel ATTiny85, running on 8MHz
* Version        : R109 - DS18B20 and BH1750 and Vcc test
* Date           : 20140502
* 
* This code turns an Atmel ATTiny85 chip into a Sensor using the TheoV2 protocol.
* This code is not of any use without running a Unit within RF range to receive sensor data.
\***************************************************************************************************************/

#define SENSOR_TYPE 1                            // 1 = DS18B20 and BH1750 sensor

// Uncomment for test purpose
//#define THEO_TEST

//*****************************************************************************

#ifdef THEO_TEST
  #define CHANNEL               0                // 0 - 7, 0 discard by plugin 
  #define SEND_DELAY            2                // Send Delay in multiples of 4 seconds, so 2 = 2x4=8 seconds
  #define VCC_LOOP              2
#else
  #define CHANNEL               1                // 0 - 7, 0 discard by plugin
  #define SEND_DELAY           30                // Send Delay in multiples of 4 seconds, so 70 = 70x4=280 seconds
  #define VCC_LOOP             24                // Send Vcc once every hour
#endif

#define VREF_35             10885                // Measured value for Vref at 3.5V Vcc * 10 (Chip dependent)
#define BATT_LOW               30                // 3.0V Battery low warning voltage * 10 (Functionality dependent)

//*****************************************************************************

#define DALLAS_PIN              1                // data to DS18B20, fysieke pin 6 on ATTiny85
#define BH1750_ADDRESS       0x23                // i2c address BH1750 

#define RF_TransmitDataPin      4                // data to RF Transmitter, fysieke pin 3 on ATTiny85

// ATMEL ATTINY85 
// AI=Analog Input 
//                         o-\/-+ 
//                 reset  1|    |8  VCC 
//    Pin 3   (AI 3) PB3  2|    |7  PB2 (AI 1) Pin 2 - SCL
//    Pin 4   (AI 2) PB4  3|    |6  PB1 PWM Pin 1 - PCINT1
//                   GND  4|    |5  PB0 PWM Pin 0 - SDA
//                         +----+ 

// Used by DS18B20
#include <OneWire.h>                             // http://www.pjrc.com/teensy/arduino_libraries/OneWire.zip

// Used by BH1750
// Prepare library TinyWireM for 8MHz by editing the following files:
// 1) file USI_TWI_Master.cpp define F_CPU from 1000000UL to 8000000UL
// 2) file USI_TWI_Master.h   define SYS_CLK from 1000.0 to 8000.0
#include <TinyWireM.h>                           // http://playground.arduino.cc/Code/USIi2c

byte VccTest;
byte Voltage;

//*****************************************************************************
// Setup stuff
//*****************************************************************************
void setup()
{
  VccTest = VCC_LOOP;

  pinMode(RF_TransmitDataPin,OUTPUT);

  TinyWireM.begin();

  Watchdog_setup(8);                             // Setup watchdog to notify us every 4 seconds
}

//*****************************************************************************
// Main loop
//*****************************************************************************
void loop()
{
  int payload1 = 0;
  int payload2 = 0;

  VccTest++;

  if (GetPayload(payload1, payload2) == 0)
    SendData(payload1, payload2);

  Watchdog_sleep(SEND_DELAY + CHANNEL);          // Go to deep sleep mode for some time
}

//*****************************************************************************
// Sleep stuff
//*****************************************************************************
#include <avr/sleep.h>
#include <avr/wdt.h>

void Watchdog_setup(int ii)
{  
  // 0=16ms, 1=32ms, 2=64ms, 3=125ms, 4=250ms, 5=500ms, 6=1s, 7=2s, 8=4s, 9=8s
  // The prescale value is held in bits 5,2,1,0
  // This block moves ii into these bits
  byte bb;
  if (ii > 9 ) ii=9;
  bb=ii & 7;
  if (ii > 7) bb|= (1<<5);
  bb|= (1<<WDCE);
  
  MCUSR &= ~(1<<WDRF);                           // Reset the watchdog reset flag
  WDTCR |= (1<<WDCE) | (1<<WDE);                 // Start timed sequence
  WDTCR = bb;                                    // Set new watchdog timeout value
  WDTCR |= _BV(WDIE);                            // Enable interrupts instead of reset
}

void Watchdog_sleep(int waitCounter)
{
  while (waitCounter != 0) 
  {
    bitClear(ADCSRA,ADEN);                       // Switch Analog to Digital converter OFF 
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);         // Set sleep mode
    sleep_mode();                                // System sleeps here
    waitCounter--;
  }
}

void Watchdog_delay(int prescale)
{
  Watchdog_setup(prescale);
  Watchdog_sleep(1);
  Watchdog_setup(8);
}

ISR(WDT_vect) 
{
  // Don't do anything here but we must include this
  // block of code otherwise the interrupt calls an
  // uninitialized interrupt handler.
}

//*****************************************************************************
// Send data via RF
//*****************************************************************************
#define RF_PULSE_0               500             // PWM: Tijdsduur van de puls bij verzenden van een '0' in uSec.
#define RF_PULSE_MID            1000             // PWM: Pulsen langer zijn '1'
#define RF_PULSE_1              1500             // PWM: Tijdsduur van de puls bij verzenden van een '1' in uSec. (3x RF_PULSE_0)
#define RF_SPACE                 500             // PWM: Tijdsduur van de space tussen de bitspuls bij verzenden van een '1' in uSec.   
#define TransmitRepeat             2

void SendData(int payload1, int payload2)
{
  struct
  {
    byte Checksum;                               // Checksum over following bytes
    byte Channel: 3;                             // 3 bits channel
    byte Type: 5;                                // 5 bits type
    byte Voltage;                                // Vcc like 45 = 4.5V, bit 8 is batt low
    int Payload1;
    int Payload2;
  } DataBlock;
  byte Size = sizeof(DataBlock);

  DataBlock.Type     = SENSOR_TYPE;  
  DataBlock.Channel  = CHANNEL;  
  if (VccTest >= VCC_LOOP)                       // Do not run the Vcc test too often to save Battery power
  {
    VccTest = 0;
    Voltage = ReadVcc();
    if (Voltage <= BATT_LOW)
      Voltage |= 0x80;
  }
  DataBlock.Voltage  = Voltage;
  DataBlock.Payload1 = payload1;
  DataBlock.Payload2 = payload2;
  byte c = 0, *B = (byte*)&DataBlock;            // bereken checksum: crc-8 uit bovenstaande bytes in de struct
  for (byte x = 1; x < Size; x++)
    c +=*(B+x); 
  DataBlock.Checksum = c;

  pinMode(RF_TransmitDataPin, OUTPUT);
  digitalWrite(RF_TransmitDataPin, LOW);         // 0
  for (byte y = 0; y < TransmitRepeat; y++)      // herhaal verzenden RF code
  {
    digitalWrite(RF_TransmitDataPin, HIGH);      // 1
    delayMicroseconds(RF_PULSE_1 * 4); 
    digitalWrite(RF_TransmitDataPin, LOW);       // 0
    delayMicroseconds(RF_SPACE * 2); 
    for (byte x = 0; x < Size; x++)
    {
      for (byte Bit = 0; Bit <= 7; Bit++)
      {
        digitalWrite(RF_TransmitDataPin, HIGH);  // 1
        if ((*(B + x) >> Bit) & 1)
          delayMicroseconds(RF_PULSE_1); 
        else 
          delayMicroseconds(RF_PULSE_0); 
        digitalWrite(RF_TransmitDataPin, LOW);   // 0
        delayMicroseconds(RF_SPACE); 
      }
    }
    delayMicroseconds(RF_PULSE_1 * 10); 
  }
}

//*****************************************************************************
// Measure battery voltage using internal bandgap voltage
//*****************************************************************************
byte ReadVcc()
{
  int result;

  bitSet(ADCSRA, ADEN);                          // switch Analog to Digital converter ON
  ADMUX = 0x0C;                                  // use VCC and internal bandgap (ATTiny85)
  delayMicroseconds(250);                        // delay substantially improves accuracy
  bitSet(ADCSRA, ADSC);                          // start conversion
  while (bit_is_set(ADCSRA, ADSC));              // wait until ADSC is clear 
  bitClear(ADCSRA, ADEN);                        // Switch Analog to Digital converter OFF 
  result = ADCW;
  return result ? VREF_35 / result : 0;          // 35 = 3.5V
}

//*****************************************************************************
// DS18B20 Temperature Sensor and BH1750 light sensor support
//*****************************************************************************
byte GetPayload(int &temperature, int &light)
{
  if (GetDS18B20(temperature) != 0) return 1;
  if (GetBH1750(light) != 0) return 1;
  return 0;
}

//*****************************************************************************
// DS18B20 Temperature
//*****************************************************************************
byte GetDS18B20(int &temperature)
{
  byte msb, lsb;
  int temp;

  OneWire ds(DALLAS_PIN);                        // Setup a oneWire instance
  ds.reset();
  ds.skip();
  ds.write(0x44);                                // Start conversion
  Watchdog_delay(6);                             // Wait 750ms. Here 1 second
  ds.reset();                                    // Read DS18B20
  ds.skip();    
  ds.write(0xBE);                                // Read scratchpad
  lsb = ds.read();
  msb = ds.read();
  ds.reset();
  temp = (msb<< 8) | lsb;
  temperature = ((((6 * temp) + temp / 4) + 5) / 10) * 10;  // multiply by (100 * 0.0625) or 6.25 and round to tenth
  return 0;
}

//*****************************************************************************
// BH1750 light
//*****************************************************************************
byte GetBH1750(int &light)
{
  byte msb, lsb;
  unsigned int value;

  TinyWireM.beginTransmission(BH1750_ADDRESS);   // Setup BH1750
  TinyWireM.send(0x20);                          // One time high resolution low power
  TinyWireM.endTransmission();
  Watchdog_delay(4);                             // Wait 160-180ms. Here 250ms
  TinyWireM.beginTransmission(BH1750_ADDRESS);   // Read BH1750
  TinyWireM.requestFrom(BH1750_ADDRESS, 2);
  msb = TinyWireM.receive();
  lsb = TinyWireM.receive();
  value = (msb<<8 | lsb) / 1.2;
  if (value > 0x7fff) value = 0x7fff;
  light = int(value);
  return 0;
}
