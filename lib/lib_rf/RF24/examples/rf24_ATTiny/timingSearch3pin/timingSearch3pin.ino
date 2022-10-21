/*
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
version 2 as published by the Free Software Foundation.

    timingSearch3pin.ino by tong67 ( https://github.com/tong67 )
    This sketch can be used to determine the best settleTime values to use in RF24::csn().
	The used settleTimeValues are 100/20. Depend on used RC combiniation and voltage drop by LED.
    It is setup to be completely selfcontained, copied defines and code from RF24 library.
    The ATtiny85 uses the tiny-core by CodingBadly (https://code.google.com/p/arduino-tiny/)
	(Intermediate) results are written to TX (PB3, pin 2). For schematic see rf24ping85.ino 
*/

// nRF24L01.h copy

/* Memory Map */
#define CONFIG      0x00
#define EN_AA       0x01
#define EN_RXADDR   0x02
#define SETUP_AW    0x03
#define SETUP_RETR  0x04
#define RF_CH       0x05
#define RF_SETUP    0x06
#define STATUS      0x07
#define OBSERVE_TX  0x08
#define CD          0x09
#define RX_ADDR_P0  0x0A
#define RX_ADDR_P1  0x0B
#define RX_ADDR_P2  0x0C
#define RX_ADDR_P3  0x0D
#define RX_ADDR_P4  0x0E
#define RX_ADDR_P5  0x0F
#define TX_ADDR     0x10
#define RX_PW_P0    0x11
#define RX_PW_P1    0x12
#define RX_PW_P2    0x13
#define RX_PW_P3    0x14
#define RX_PW_P4    0x15
#define RX_PW_P5    0x16
#define FIFO_STATUS 0x17
#define DYNPD	    0x1C
#define FEATURE	    0x1D

/* Bit Mnemonics */
#define MASK_RX_DR  6
#define MASK_TX_DS  5
#define MASK_MAX_RT 4
#define EN_CRC      3
#define CRCO        2
#define PWR_UP      1
#define PRIM_RX     0
#define ENAA_P5     5
#define ENAA_P4     4
#define ENAA_P3     3
#define ENAA_P2     2
#define ENAA_P1     1
#define ENAA_P0     0
#define ERX_P5      5
#define ERX_P4      4
#define ERX_P3      3
#define ERX_P2      2
#define ERX_P1      1
#define ERX_P0      0
#define AW          0
#define ARD         4
#define ARC         0
#define PLL_LOCK    4
#define RF_DR       3
#define RF_PWR      6
#define RX_DR       6
#define TX_DS       5
#define MAX_RT      4
#define RX_P_NO     1
#define TX_FULL     0
#define PLOS_CNT    4
#define ARC_CNT     0
#define TX_REUSE    6
#define FIFO_FULL   5
#define TX_EMPTY    4
#define RX_FULL     1
#define RX_EMPTY    0
#define DPL_P5	    5
#define DPL_P4	    4
#define DPL_P3	    3
#define DPL_P2	    2
#define DPL_P1	    1
#define DPL_P0	    0
#define EN_DPL	    2
#define EN_ACK_PAY  1
#define EN_DYN_ACK  0

/* Instruction Mnemonics */
#define R_REGISTER    0x00
#define W_REGISTER    0x20
#define REGISTER_MASK 0x1F
#define ACTIVATE      0x50
#define R_RX_PL_WID   0x60
#define R_RX_PAYLOAD  0x61
#define W_TX_PAYLOAD  0xA0
#define W_ACK_PAYLOAD 0xA8
#define FLUSH_TX      0xE1
#define FLUSH_RX      0xE2
#define REUSE_TX_PL   0xE3
#define RF24_NOP      0xFF

/* Non-P omissions */
#define LNA_HCURR   0

/* P model memory Map */
#define RPD         0x09
#define W_TX_PAYLOAD_NO_ACK  0xB0

/* P model bit Mnemonics */
#define RF_DR_LOW   5
#define RF_DR_HIGH  3
#define RF_PWR_LOW  1
#define RF_PWR_HIGH 2
/****************************************************************************/

//ATTiny support code pulled in from https://github.com/jscrane/RF24
#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
// see http://gammon.com.au/spi
#	define DI   0  // D0, pin 5  Data In
#	define DO   1  // D1, pin 6  Data Out (this is *not* MOSI)
#	define USCK 2  // D2, pin 7  Universal Serial Interface clock
#	define SS   3  // D3, pin 2  Slave Select
#elif defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
// these depend on the core used (check pins_arduino.h)
// this is for jeelabs' one (based on google-code core)
#	define DI   4   // PA6
#	define DO   5   // PA5
#	define USCK 6   // PA4
#	define SS   3   // PA7
#endif

#if defined (ARDUINO) && !defined (__arm__)
	#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
		#define RF24_TINY
	#else
//		#include <SPI.h>
	#endif
#endif

#if defined(RF24_TINY)
#include <stdio.h>
#include <Arduino.h>
#include <avr/pgmspace.h>

#define SPI_CLOCK_DIV4 0x00
#define SPI_CLOCK_DIV16 0x01
#define SPI_CLOCK_DIV64 0x02
#define SPI_CLOCK_DIV128 0x03
#define SPI_CLOCK_DIV2 0x04
#define SPI_CLOCK_DIV8 0x05
#define SPI_CLOCK_DIV32 0x06
//#define SPI_CLOCK_DIV64 0x07

#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C

#define SPI_MODE_MASK 0x0C  // CPOL = bit 3, CPHA = bit 2 on SPCR
#define SPI_CLOCK_MASK 0x03  // SPR1 = bit 1, SPR0 = bit 0 on SPCR
#define SPI_2XCLOCK_MASK 0x01  // SPI2X = bit 0 on SPSR

class SPIClass {
public:
  static byte transfer(byte _data);

  // SPI Configuration methods

  inline static void attachInterrupt();
  inline static void detachInterrupt(); // Default

  static void begin(); // Default
  static void end();

//  static void setBitOrder(uint8_t);
//  static void setDataMode(uint8_t);
//  static void setClockDivider(uint8_t);
};
extern SPIClass SPI;

#endif /* RF24_TINY */


#if defined(RF24_TINY)

void SPIClass::begin() {
  digitalWrite(SS, HIGH);
  pinMode(USCK, OUTPUT);
  pinMode(DO, OUTPUT);
  pinMode(SS, OUTPUT);
  pinMode(DI, INPUT);
  USICR = _BV(USIWM0);
}

byte SPIClass::transfer(byte b) {
  USIDR = b;
  USISR = _BV(USIOIF);
  do
    USICR = _BV(USIWM0) | _BV(USICS1) | _BV(USICLK) | _BV(USITC);
  while ((USISR & _BV(USIOIF)) == 0);
  return USIDR;
}

void SPIClass::end() {}

#endif /* RF24_TINY */

/****************************************************************************/
uint8_t ce_pin; /**< "Chip Enable" pin, activates the RX or TX role */
uint8_t csn_pin; /**< SPI Chip select */  
uint8_t csnHighSettle = 255;
uint8_t csnLowSettle = 255;
/****************************************************************************/
void ce(bool level) {
  if (ce_pin != csn_pin) digitalWrite(ce_pin,level);
}

/****************************************************************************/
void setCsnHighSettle(uint8_t level) {
  csnHighSettle = level;
}

/****************************************************************************/
void setCsnLowSettle(uint8_t level) {
  csnLowSettle = level;
}
/****************************************************************************/
void csn(bool mode) {
	if (ce_pin != csn_pin) {
		digitalWrite(csn_pin,mode);
	} else {
		if (mode == HIGH) {
			PORTB |= (1<<PINB2);  	// SCK->CSN HIGH
			delayMicroseconds(csnHighSettle);  // allow csn to settle
		} else {
			PORTB &= ~(1<<PINB2);	// SCK->CSN LOW
			delayMicroseconds(csnLowSettle);  // allow csn to settle
		}
	}	
}

/****************************************************************************/
uint8_t read_register(uint8_t reg)
{
  csn(LOW);
  SPI.transfer( R_REGISTER | ( REGISTER_MASK & reg ) );
  uint8_t result = SPI.transfer(0xff);
  csn(HIGH);
  return result;
}

/****************************************************************************/
uint8_t write_register2(uint8_t reg, uint8_t value)
{
  uint8_t status;

  csn(LOW);
  status = SPI.transfer( W_REGISTER | ( REGISTER_MASK & reg ) );
  SPI.transfer(value);
  csn(HIGH);
  return status;
}

/****************************************************************************/
#if defined(RF24_TINY)
#define CE_PIN   3
#define CSN_PIN  3
#else
#define CE_PIN   7
#define CSN_PIN  8
#endif

#define MAX_HIGH	100
#define MAX_LOW		100
#define MINIMAL		8

void setup(void) {
  uint8_t status;

  // start serial port and SPI
  Serial.begin(9600);
  SPI.begin();

  // configure ce and scn as output when used
  ce_pin = CE_PIN;
  csn_pin = CSN_PIN;

  setCsnHighSettle(MAX_HIGH);
  setCsnLowSettle(MAX_LOW);
  // csn is used in SPI transfers. Set to LOW at start and HIGH after transfer. Set to HIGH to reflect no transfer active
  // SPI command are accepted in Power Down state.
  // ce represent PRX (LOW) or PTX (HIGH) mode apart from register settings. Start in PRX mode.  
  ce(LOW);
  csn(HIGH);

  // nRF24L01 goes from to Power Down state 100ms after Power on Reset ( Vdd > 1.9V) or when PWR_UP is 0 in config register 
  // Goto Power Down state (Powerup or force) and set in transmit mode
  write_register2(CONFIG, read_register(CONFIG) & ~_BV(PWR_UP) & ~_BV(PRIM_RX));
  delay(100);
  
  // Goto Standby-I
  // Technically we require 4.5ms Tpd2stby+ 14us as a worst case. We'll just call it 5ms for good measure.
  // WARNING: Delay is based on P-variant whereby non-P *may* require different timing.
  write_register2(CONFIG, read_register(CONFIG) | _BV(PWR_UP));
  delay(5) ;

  // Goto Standby-II
  ce(HIGH);
  Serial.print("Scanning for optimal setting time for scn");
}


void loop(void) {
  uint8_t status;
  uint8_t i;
  uint8_t j;
  uint8_t k;
  bool success = true;
  uint8_t csnHigh = MAX_HIGH;
  uint8_t csnLow = MAX_LOW;
  uint8_t bottom_success;
  bool bottom_found;
  uint8_t value[] = {5,10};
  uint8_t limit[] = {MAX_HIGH,MAX_LOW};
  uint8_t advice[] = {MAX_HIGH,MAX_LOW};
  
  // check max values give correct behavior
  for (k=0;k<2;k++) {
    bottom_found = false;
    bottom_success = 0;
    while(bottom_success < 255) {
      setCsnHighSettle(limit[0]);
      setCsnLowSettle(limit[1]);
      // check current values
      i = 0;
      while(i<255 & success) {
        for (j=0;j<2;j++) {
          write_register2(EN_AA, value[j]);
          status = read_register(EN_AA);
          if (value[j] != status) {
            success = false;
          }
        }
        i++;
      }
      // process result of current values 
      if (!success) {
        Serial.print("Settle NOK. csnHigh=");
        Serial.print(limit[0],DEC);
        Serial.print(" csnLow="); 
        Serial.println(limit[1],DEC);
        limit[k]++;
        bottom_found = true;
        bottom_success = 0;
        success = true;
      } else {
        Serial.print("Settle OK. csnHigh=");
        Serial.print(limit[0],DEC);
        Serial.print(" csnLow="); 
        Serial.println(limit[1],DEC);
        if (!bottom_found) {
          limit[k]--;
          if (limit[k] == MINIMAL) {
            bottom_found = true;
            bottom_success = 0;
            success = true;
          }
        } else {
          bottom_success++;
        }
      }
    }
    Serial.print("Settle value found for ");
    if (k == 0) {
      Serial.print("csnHigh: ");
    } else {
      Serial.print("csnLow: ");
    }
    Serial.println(limit[k],DEC);
    advice[k] = limit[k] + (limit[k] / 10);
    limit[k] = 100;
  }
  Serial.print("Adviced Settle times are: csnHigh=");
  Serial.print(advice[0],DEC);
  Serial.print(" csnLow="); 
  Serial.println(advice[1],DEC);
  while (true)
  {
    ;
  }
}

