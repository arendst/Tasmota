/****************************************************************************************************************************\
* Arduino project "TheoTinySensor" Copyright 2013 Theo Arends 
* This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License 
* as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty 
* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
******************************************************************************************************************************

****************************************************************************************************************
* Purpose        : Arduino Source code for Sensor based on Atmel ATTiny85, running on 8MHz
* Version        : R201 - DHT11 or DHT22 and Vcc test
* Date           : 20140414
*                  20140507 - R202 - Verwijdering VCC_LOOP
* 
* This code turns an Atmel ATTiny85 chip into a Sensor using the TheoV2 protocol.
* This code is not of any use without running a Unit within RF range to receive sensor data.
\***************************************************************************************************************/

#define SENSOR_TYPE 2                            // 2 = DHT22 or DHT11 sensor
#define DHT22                                    // DHT22 Temperatuur en luchtvochtigheid
//#define DHT11                                    // DHT11 Temperatuur en luchtvochtigheid

// Uncomment for test purpose
//#define THEO_TEST

//*****************************************************************************

#ifdef THEO_TEST
  #define CHANNEL               0                // 0 - 7, 0 discard by plugin 
  #define SEND_DELAY            2                // Max Send Delay in multiples of 4.4 seconds, so 2 = 2x4.4=9 seconds
#else
  #define CHANNEL               1                // 0 - 7, 0 discard by plugin
  #define SEND_DELAY           32                // Max Send Delay in multiples of 4.4 seconds, so 32 = 32x4.4=141 seconds
#endif

#define VREF_35             10675                // Measured value for Vref at 3.5V Vcc * 10 (Chip dependent)
#define BATT_LOW               33                // 3.3V Battery low warning voltage * 10 (Functionality dependent)

//*****************************************************************************

#define DHT_PIN                 1                // data to DHT11, fysieke pin 6 on ATTiny85

#define RF_TransmitDataPin      4                // data to RF Transmitter, fysieke pin 3 on ATTiny85

// ATMEL ATTINY85 
// AI=Analog Input 
//                         o-\/-+ 
//                 reset  1|    |8  VCC 
//    Pin 3   (AI 3) PB3  2|    |7
//    Pin 4   (AI 2) PB4  3|    |6  PB1 PWM Pin 1 - PCINT1
//                   GND  4|    |5
//                         +----+ 

byte VccTest;
byte Voltage;

//*****************************************************************************
// Setup stuff
//*****************************************************************************
void setup()
{
  VccTest = 255;
  
  pinMode(RF_TransmitDataPin,OUTPUT);

  pinMode(DHT_PIN, OUTPUT);
  digitalWrite(DHT_PIN, HIGH);                   // Pull high
  delay(3000);

  Watchdog_setup(8);                             // Setup watchdog to notify us every 4.4 seconds
}

//*****************************************************************************
// Main loop
//*****************************************************************************
void loop()
{
  int payload1 = 0;
  int payload2 = 0;

  VccTest++;
  if (VccTest == 0)                              // Do not run the Vcc test too often to save Battery power
  {
    Voltage = ReadVcc();
    if (Voltage <= BATT_LOW)
      Voltage |= 0x80;
  }

  if (GetPayload(payload1, payload2) == 0)
    SendData(payload1, payload2);

//  Watchdog_sleep(SEND_DELAY + CHANNEL);          // Go to deep sleep mode for some time
  Watchdog_sleep(SEND_DELAY - SENSOR_TYPE - CHANNEL); // Go to deep sleep mode for some time but not longer than 150 sec
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
// DHT11 or DHT22 Temperature and humidity sensor support
//*****************************************************************************
byte GetPayload(int &temperature, int &humidity)
{
  return GetDHT(temperature, humidity);
}

//*****************************************************************************
// DHT11 or DHT22 Temperature and humidity
// http://forums.adafruit.com/viewtopic.php?f=25&t=43933
// https://github.com/markruys/arduino-DHT
//*****************************************************************************
byte GetDHT(int &temperature, int &humidity)
{
  unsigned long startTime = millis();
  word rawHumidity;
  word rawTemperature;
  word data;

  digitalWrite(DHT_PIN, LOW);                    // Send start signal
  pinMode(DHT_PIN, OUTPUT);
#ifdef DHT22
  delayMicroseconds(800);
#else
  delay(18);
#endif
  pinMode(DHT_PIN, INPUT);
  digitalWrite(DHT_PIN, HIGH);                   // Switch bus to receive data
  // We're going to read 83 edges:
  // - First a FALLING, RISING, and FALLING edge for the start bit
  // - Then 40 bits: RISING and then a FALLING edge per bit
  // To keep our code simple, we accept any HIGH or LOW reading if it's max 85 usecs long
  for (int8_t i = -3 ; i < 2 * 40; i++)
  {
    byte age;

    startTime = micros();
    do 
    {
      age = (unsigned long)(micros() - startTime);
      if (age > 90) return 1;
    }
    while (digitalRead(DHT_PIN) == (i & 1) ? HIGH : LOW);
    if (i >= 0 && (i & 1))
    {
      data <<= 1;                                // Now we are being fed our 40 bits
      if (age > 30) data |= 1;                   // A zero max 30 usecs, a one at least 68 usecs. We got a one
    }
    switch (i)
    {
      case 31:
        rawHumidity = data;
        break;
      case 63:
        rawTemperature = data;
        data = 0;
        break;
    }
  }
  // Verify checksum
  if ((byte)(((byte)rawHumidity) + (rawHumidity >> 8) + ((byte)rawTemperature) + (rawTemperature >> 8)) != data)
    return 2;
#ifdef DHT22
  int t = (rawTemperature & 0x7fff) * 10;
  temperature = rawTemperature & 0x8000 ? - t : t;
//  humidity = (rawHumidity / 10) * 100;           // round to whole
  humidity = rawHumidity * 10;
#else
  humidity = (rawHumidity >> 8) * 100;
  temperature = (rawTemperature >> 8) * 100;
#endif
  return 0;
}
