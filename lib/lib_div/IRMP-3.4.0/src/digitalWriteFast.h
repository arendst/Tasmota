/*
  Optimized digital functions for AVR microcontrollers
  by Watterott electronic (www.watterott.com)
  based on http://code.google.com/p/digitalwritefast
 */

#ifndef __digitalWriteFast_h_
#define __digitalWriteFast_h_ 1

//#define SANGUINO_PINOUT //define for Sanguino pinout

// general macros/defines
#ifndef BIT_READ
# define BIT_READ(value, bit)            ((value) &   (1UL << (bit)))
#endif
#ifndef BIT_SET
# define BIT_SET(value, bit)             ((value) |=  (1UL << (bit)))
#endif
#ifndef BIT_CLEAR
# define BIT_CLEAR(value, bit)           ((value) &= ~(1UL << (bit)))
#endif
#ifndef BIT_WRITE
# define BIT_WRITE(value, bit, bitvalue) (bitvalue ? BIT_SET(value, bit) : BIT_CLEAR(value, bit))
#endif

#ifndef SWAP
# define SWAP(x,y) do{ (x)=(x)^(y); (y)=(x)^(y); (x)=(x)^(y); }while(0)
#endif

#ifndef DEC
# define DEC (10)
#endif
#ifndef HEX
# define HEX (16)
#endif
#ifndef OCT
# define OCT (8)
#endif
#ifndef BIN
# define BIN (2)
#endif


// workarounds for ARM microcontrollers
#if (!defined(__AVR__) || \
     defined(ARDUINO_ARCH_SAM) || \
     defined(ARDUINO_ARCH_SAMD))

#ifndef PROGMEM
# define PROGMEM
#endif
#ifndef PGM_P
# define PGM_P const char *
#endif
#ifndef PSTR
# define PSTR(str) (str)
#endif

#ifndef memcpy_P
# define memcpy_P(dest, src, num) memcpy((dest), (src), (num))
#endif
#ifndef strcpy_P
# define strcpy_P(dst, src)       strcpy((dst), (src))
#endif
#ifndef strcat_P
# define strcat_P(dst, src)       strcat((dst), (src))
#endif
#ifndef strcmp_P
# define strcmp_P(a, b)           strcmp((a), (b))
#endif
#ifndef strcasecmp_P
# define strcasecmp_P(a, b)       strcasecmp((a), (b))
#endif
#ifndef strncmp_P
# define strncmp_P(a, b, n)       strncmp((a), (b), (n))
#endif
#ifndef strncasecmp_P
# define strncasecmp_P(a, b, n)   strncasecmp((a), (b), (n))
#endif
#ifndef strstr_P
# define strstr_P(a, b)           strstr((a), (b))
#endif
#ifndef strlen_P
# define strlen_P(a)              strlen((a))
#endif
#ifndef sprintf_P
# define sprintf_P(s, f, ...)     sprintf((s), (f), __VA_ARGS__)
#endif

#ifndef pgm_read_byte
# define pgm_read_byte(addr)      (*(const unsigned char *)(addr))
#endif
#ifndef pgm_read_word
# define pgm_read_word(addr)      (*(const unsigned short *)(addr))
#endif
#ifndef pgm_read_dword
# define pgm_read_dword(addr)     (*(const unsigned long *)(addr))
#endif

#endif


// digital functions
//#ifndef digitalPinToPortReg
#define SPI_SW_SS_PIN   (10) //SS on Uno (for software SPI)
#define SPI_SW_MOSI_PIN (11) //MOSI on Uno (for software SPI)
#define SPI_SW_MISO_PIN (12) //MISO on Uno (for software SPI)
#define SPI_SW_SCK_PIN  (13) //SCK on Uno (for software SPI)


// --- Arduino Due and SAM3X8E based boards ---
#if (defined(ARDUINO_SAM_DUE) || \
     defined(__SAM3X8E__))

#define UART_RX_PIN     (0)
#define UART_TX_PIN     (1)

#define I2C_SDA_PIN     (20)
#define I2C_SCL_PIN     (21)

#define SPI_HW_SS_PIN   (78) //SS0:77, SS1:87, SS2:86, SS3:78
#define SPI_HW_MOSI_PIN (75) //75
#define SPI_HW_MISO_PIN (74) //74
#define SPI_HW_SCK_PIN  (76) //76


// --- Arduino Zero and SAMD21G18 based boards ---
#elif (defined(ARDUINO_SAMD_ZERO) || \
       defined(__SAMD21G18A__))

#define UART_RX_PIN     (0)
#define UART_TX_PIN     (1)

#define I2C_SDA_PIN     (16)
#define I2C_SCL_PIN     (17)

#define SPI_HW_SS_PIN   (14) //14
#define SPI_HW_MOSI_PIN (21) //21
#define SPI_HW_MISO_PIN (18) //18
#define SPI_HW_SCK_PIN  (20) //20


// --- Arduino Mega and ATmega128x/256x based boards ---
#elif (defined(ARDUINO_AVR_MEGA) || \
       defined(ARDUINO_AVR_MEGA1280) || \
       defined(ARDUINO_AVR_MEGA2560) || \
       defined(__AVR_ATmega1280__) || \
       defined(__AVR_ATmega1281__) || \
       defined(__AVR_ATmega2560__) || \
       defined(__AVR_ATmega2561__))

#define UART_RX_PIN     (0) //PE0
#define UART_TX_PIN     (1) //PE1

#define I2C_SDA_PIN     (20)
#define I2C_SCL_PIN     (21)

#define SPI_HW_SS_PIN   (53) //PB0
#define SPI_HW_MOSI_PIN (51) //PB2
#define SPI_HW_MISO_PIN (50) //PB3
#define SPI_HW_SCK_PIN  (52) //PB1

#define __digitalPinToPortReg(P) \
(((P) >= 22 && (P) <= 29) ? &PORTA : \
((((P) >= 10 && (P) <= 13) || ((P) >= 50 && (P) <= 53)) ? &PORTB : \
(((P) >= 30 && (P) <= 37) ? &PORTC : \
((((P) >= 18 && (P) <= 21) || (P) == 38) ? &PORTD : \
((((P) >= 0 && (P) <= 3) || (P) == 5) ? &PORTE : \
(((P) >= 54 && (P) <= 61) ? &PORTF : \
((((P) >= 39 && (P) <= 41) || (P) == 4) ? &PORTG : \
((((P) >= 6 && (P) <= 9) || (P) == 16 || (P) == 17) ? &PORTH : \
(((P) == 14 || (P) == 15) ? &PORTJ : \
(((P) >= 62 && (P) <= 69) ? &PORTK : &PORTL))))))))))

#define __digitalPinToDDRReg(P) \
(((P) >= 22 && (P) <= 29) ? &DDRA : \
((((P) >= 10 && (P) <= 13) || ((P) >= 50 && (P) <= 53)) ? &DDRB : \
(((P) >= 30 && (P) <= 37) ? &DDRC : \
((((P) >= 18 && (P) <= 21) || (P) == 38) ? &DDRD : \
((((P) >= 0 && (P) <= 3) || (P) == 5) ? &DDRE : \
(((P) >= 54 && (P) <= 61) ? &DDRF : \
((((P) >= 39 && (P) <= 41) || (P) == 4) ? &DDRG : \
((((P) >= 6 && (P) <= 9) || (P) == 16 || (P) == 17) ? &DDRH : \
(((P) == 14 || (P) == 15) ? &DDRJ : \
(((P) >= 62 && (P) <= 69) ? &DDRK : &DDRL))))))))))

#define __digitalPinToPINReg(P) \
(((P) >= 22 && (P) <= 29) ? &PINA : \
((((P) >= 10 && (P) <= 13) || ((P) >= 50 && (P) <= 53)) ? &PINB : \
(((P) >= 30 && (P) <= 37) ? &PINC : \
((((P) >= 18 && (P) <= 21) || (P) == 38) ? &PIND : \
((((P) >= 0 && (P) <= 3) || (P) == 5) ? &PINE : \
(((P) >= 54 && (P) <= 61) ? &PINF : \
((((P) >= 39 && (P) <= 41) || (P) == 4) ? &PING : \
((((P) >= 6 && (P) <= 9) || (P) == 16 || (P) == 17) ? &PINH : \
(((P) == 14 || (P) == 15) ? &PINJ : \
(((P) >= 62 && (P) <= 69) ? &PINK : &PINL))))))))))

#define __digitalPinToBit(P) \
(((P) >=  7 && (P) <=  9) ? (P) - 3 : \
(((P) >= 10 && (P) <= 13) ? (P) - 6 : \
(((P) >= 22 && (P) <= 29) ? (P) - 22 : \
(((P) >= 30 && (P) <= 37) ? 37 - (P) : \
(((P) >= 39 && (P) <= 41) ? 41 - (P) : \
(((P) >= 42 && (P) <= 49) ? 49 - (P) : \
(((P) >= 50 && (P) <= 53) ? 53 - (P) : \
(((P) >= 54 && (P) <= 61) ? (P) - 54 : \
(((P) >= 62 && (P) <= 69) ? (P) - 62 : \
(((P) == 0 || (P) == 15 || (P) == 17 || (P) == 21) ? 0 : \
(((P) == 1 || (P) == 14 || (P) == 16 || (P) == 20) ? 1 : \
(((P) == 19) ? 2 : \
(((P) == 5 || (P) == 6 || (P) == 18) ? 3 : \
(((P) == 2) ? 4 : \
(((P) == 3 || (P) == 4) ? 5 : 7)))))))))))))))


// --- Arduino MightyCore standard pinout ---
#elif (defined(__AVR_ATmega1284P__) || \
       defined(__AVR_ATmega1284__)  || \
       defined(__AVR_ATmega644P__)  || \
       defined(__AVR_ATmega644A__)  || \
       defined(__AVR_ATmega644__)   || \
       defined(__AVR_ATmega324PB__) || \
       defined(__AVR_ATmega324PA__) || \
       defined(__AVR_ATmega324P__)  || \
       defined(__AVR_ATmega324A__)  || \
       defined(__AVR_ATmega164P__)  || \
       defined(__AVR_ATmega164A__)  || \
       defined(__AVR_ATmega32__)    || \
       defined(__AVR_ATmega16__)    || \
       defined(__AVR_ATmega8535__))  && \
      !defined(BOBUINO_PINOUT)

#define UART_RX_PIN     (8) //PD0
#define UART_TX_PIN     (9) //PD1

#define I2C_SDA_PIN     (17) //PC1
#define I2C_SCL_PIN     (16) //PC0

#define SPI_HW_SS_PIN   (4) //PB4
#define SPI_HW_MOSI_PIN (5) //PB5
#define SPI_HW_MISO_PIN (6) //PB6
#define SPI_HW_SCK_PIN  (7) //PB7

#if defined(__AVR_ATmega324PB__)
#define __digitalPinToPortReg(P) \
(((P) >= 0 && (P) <= 7) ? &PORTB : (((P) >= 8 && (P) <= 15) ? &PORTD : (((P) >= 16 && (P) <= 23) ? &PORTC : (((P) >= 24 && (P) <= 31) ? &PORTA : &PORTE))))
#define __digitalPinToDDRReg(P) \
(((P) >= 0 && (P) <= 7) ? &DDRB : (((P) >= 8 && (P) <= 15) ? &DDRD : (((P) >= 16 && (P) <= 23) ? &DDRC : (((P) >= 24 && (P) <= 31) ? &DDRA : &DDRE))))
#define __digitalPinToPINReg(P) \
(((P) >= 0 && (P) <= 7) ? &PINB : (((P) >= 8 && (P) <= 15) ? &PIND : (((P) >= 16 && (P) <= 23) ? &PINC : (((P) >= 24 && (P) <= 31) ? &PINA : &PINE))))
# if defined(SANGUINO_PINOUT)
#define __digitalPinToBit(P) \
(((P) >= 0 && (P) <= 7) ? (P) : (((P) >= 8 && (P) <= 15) ? (P) - 8 : (((P) >= 16 && (P) <= 23) ? (P) - 16 : (((P) >= 16 && (P) <= 23) ? (7 - ((P) - 24)) : (P) - 32))))
# else //MightyCore Pinout
#define __digitalPinToBit(P) \
(((P) >= 0 && (P) <= 7) ? (P) : (((P) >= 8 && (P) <= 15) ? (P) - 8 : (((P) >= 16 && (P) <= 23) ? (P) - 16 : (((P) >= 16 && (P) <= 23) ? (P) - 24 : (P) - 32))))
# endif
#else
#define __digitalPinToPortReg(P) \
(((P) >= 0 && (P) <= 7) ? &PORTB : (((P) >= 8 && (P) <= 15) ? &PORTD : (((P) >= 16 && (P) <= 23) ? &PORTC : &PORTA)))
#define __digitalPinToDDRReg(P) \
(((P) >= 0 && (P) <= 7) ? &DDRB : (((P) >= 8 && (P) <= 15) ? &DDRD : (((P) >= 16 && (P) <= 23) ? &DDRC : &DDRA)))
#define __digitalPinToPINReg(P) \
(((P) >= 0 && (P) <= 7) ? &PINB : (((P) >= 8 && (P) <= 15) ? &PIND : (((P) >= 16 && (P) <= 23) ? &PINC : &PINA)))
# if defined(SANGUINO_PINOUT)
#define __digitalPinToBit(P) \
(((P) >= 0 && (P) <= 7) ? (P) : (((P) >= 8 && (P) <= 15) ? (P) - 8 : (((P) >= 16 && (P) <= 23) ? (P) - 16 : (7 - ((P) - 24)))))
# else //MightyCore Pinout
#define __digitalPinToBit(P) \
(((P) >= 0 && (P) <= 7) ? (P) : (((P) >= 8 && (P) <= 15) ? (P) - 8 : (((P) >= 16 && (P) <= 23) ? (P) - 16 : (P) - 24)))
# endif
#endif


// --- Arduino Leonardo and ATmega16U4/32U4 based boards ---
#elif (defined(ARDUINO_AVR_LEONARDO) || \
       defined(__AVR_ATmega16U4__) || \
       defined(__AVR_ATmega32U4__))

#define UART_RX_PIN     (0) //PD2
#define UART_TX_PIN     (1) //PD3

#define I2C_SDA_PIN     (2) //PD1
#define I2C_SCL_PIN     (3) //PD0

#define SPI_HW_SS_PIN   (17) //PB0
#define SPI_HW_MOSI_PIN (16) //PB2
#define SPI_HW_MISO_PIN (14) //PB3
#define SPI_HW_SCK_PIN  (15) //PB1

#define __digitalPinToPortReg(P) \
((((P) >= 0 && (P) <= 4) || (P) == 6 || (P) == 12 || (P) == 24 || (P) == 25 || (P) == 29) ? &PORTD : (((P) == 5 || (P) == 13) ? &PORTC : (((P) >= 18 && (P) <= 23)) ? &PORTF : (((P) == 7) ? &PORTE : &PORTB)))
#define __digitalPinToDDRReg(P) \
((((P) >= 0 && (P) <= 4) || (P) == 6 || (P) == 12 || (P) == 24 || (P) == 25 || (P) == 29) ? &DDRD : (((P) == 5 || (P) == 13) ? &DDRC : (((P) >= 18 && (P) <= 23)) ? &DDRF : (((P) == 7) ? &DDRE : &DDRB)))
#define __digitalPinToPINReg(P) \
((((P) >= 0 && (P) <= 4) || (P) == 6 || (P) == 12 || (P) == 24 || (P) == 25 || (P) == 29) ? &PIND : (((P) == 5 || (P) == 13) ? &PINC : (((P) >= 18 && (P) <= 23)) ? &PINF : (((P) == 7) ? &PINE : &PINB)))
#define __digitalPinToBit(P) \
(((P) >= 8 && (P) <= 11) ? (P) - 4 : (((P) >= 18 && (P) <= 21) ? 25 - (P) : (((P) == 0) ? 2 : (((P) == 1) ? 3 : (((P) == 2) ? 1 : (((P) == 3) ? 0 : (((P) == 4) ? 4 : (((P) == 6) ? 7 : (((P) == 13) ? 7 : (((P) == 14) ? 3 : (((P) == 15) ? 1 : (((P) == 16) ? 2 : (((P) == 17) ? 0 : (((P) == 22) ? 1 : (((P) == 23) ? 0 : (((P) == 24) ? 4 : (((P) == 25) ? 7 : (((P) == 26) ? 4 : (((P) == 27) ? 5 : 6 )))))))))))))))))))


// --- Arduino Uno and ATmega168/328 based boards ---
#elif (defined(ARDUINO_AVR_UNO) || \
       defined(ARDUINO_AVR_DUEMILANOVE) || \
       defined(__AVR_ATmega8__) || \
       defined(__AVR_ATmega48__) || \
       defined(__AVR_ATmega48P__) || \
       defined(__AVR_ATmega48PB__) || \
       defined(__AVR_ATmega88P__) || \
       defined(__AVR_ATmega88PB__) || \
       defined(__AVR_ATmega168__) || \
       defined(__AVR_ATmega168PA__) || \
       defined(__AVR_ATmega168PB__) || \
       defined(__AVR_ATmega328__) || \
       defined(__AVR_ATmega328P__) || \
       defined(__AVR_ATmega328PB__))

#define UART_RX_PIN     (0) //PD0
#define UART_TX_PIN     (1) //PD1

#define I2C_SDA_PIN     (18) //A4
#define I2C_SCL_PIN     (19) //A5

#define SPI_HW_SS_PIN   (10) //PB0
#define SPI_HW_MOSI_PIN (11) //PB2
#define SPI_HW_MISO_PIN (12) //PB3
#define SPI_HW_SCK_PIN  (13) //PB1

#if defined(__AVR_ATmega48PB__) || defined(__AVR_ATmega88PB__) || defined(__AVR_ATmega168PB__) || defined(__AVR_ATmega328PB__)
#define __digitalPinToPortReg(P) \
(((P) >= 0 && (P) <= 7) ? &PORTD : (((P) >= 8 && (P) <= 13) ? &PORTB : (((P) >= 14 && (P) <= 19) ? &PORTC : &PORTE)))
#define __digitalPinToDDRReg(P) \
(((P) >= 0 && (P) <= 7) ? &DDRD : (((P) >= 8 && (P) <= 13) ? &DDRB : (((P) >= 14 && (P) <= 19) ? &DDRC : &DDRE)))
#define __digitalPinToPINReg(P) \
(((P) >= 0 && (P) <= 7) ? &PIND : (((P) >= 8 && (P) <= 13) ? &PINB : (((P) >= 14 && (P) <= 19) ? &PINC : &PINE)))
#define __digitalPinToBit(P) \
(((P) >= 0 && (P) <= 7) ? (P) : (((P) >= 8 && (P) <= 13) ? (P) - 8 : (((P) >= 14 && (P) <= 19) ? (P) - 14 : (((P) >= 20 && (P) <= 21) ? (P) - 18 : (P) - 22))))
#else
#define __digitalPinToPortReg(P) \
(((P) >= 0 && (P) <= 7) ? &PORTD : (((P) >= 8 && (P) <= 13) ? &PORTB : &PORTC))
#define __digitalPinToDDRReg(P) \
(((P) >= 0 && (P) <= 7) ? &DDRD : (((P) >= 8 && (P) <= 13) ? &DDRB : &DDRC))
#define __digitalPinToPINReg(P) \
(((P) >= 0 && (P) <= 7) ? &PIND : (((P) >= 8 && (P) <= 13) ? &PINB : &PINC))
#define __digitalPinToBit(P) \
(((P) >= 0 && (P) <= 7) ? (P) : (((P) >= 8 && (P) <= 13) ? (P) - 8 : (P) - 14))
#endif


// --- Arduino Uno WiFi Rev 2, Nano Every ---
#elif defined(__AVR_ATmega4809__)

#define UART_RX_PIN     (0) //PB0
#define UART_TX_PIN     (1) //PB1

#define I2C_SDA_PIN     (22) //PA2
#define I2C_SCL_PIN     (23) //PA3

#define SPI_HW_SS_PIN   (8)  //PE3
#define SPI_HW_MOSI_PIN (11) //PE0
#define SPI_HW_MISO_PIN (12) //PE1
#define SPI_HW_SCK_PIN  (13) //PE2

#define __digitalPinToPortReg(P) \
(((P) == 2 || (P) == 7 ) ? &VPORTA.OUT : ((P) == 5 || (P) == 9 || (P) == 10) ? &VPORTB.OUT : ((P) == 4) ? &VPORTC.OUT : (((P) >= 14 && (P) <= 17) || (P) == 20 || (P) == 21) ? &VPORTD.OUT : ((P) == 8 || (P) == 11 || (P) == 12 || (P) == 13) ? &VPORTE.OUT : &VPORTF.OUT)
#define __digitalPinToDDRReg(P) \
(((P) == 2 || (P) == 7 ) ? &VPORTA.DIR : ((P) == 5 || (P) == 9 || (P) == 10) ? &VPORTB.DIR : ((P) == 4) ? &VPORTC.DIR : (((P) >= 14 && (P) <= 17) || (P) == 20 || (P) == 21) ? &VPORTD.DIR : ((P) == 8 || (P) == 11 || (P) == 12 || (P) == 13) ? &VPORTE.DIR : &VPORTF.DIR)
#define __digitalPinToPINReg(P) \
(((P) == 2 || (P) == 7 ) ? &VPORTA.IN : ((P) == 5 || (P) == 9 || (P) == 10) ? &VPORTB.IN : ((P) == 4) ? &VPORTC.IN : (((P) >= 14 && (P) <= 17) || (P) == 20 || (P) == 21) ? &VPORTD.IN : ((P) == 8 || (P) == 11 || (P) == 12 || (P) == 13) ? &VPORTE.IN : &VPORTF.IN)
#define __digitalPinToBit(P) \
(((P) == 2 || (P) == 9 || (P) == 11 || (P) == 17) ? 0 : ((P) == 7 || (P) == 10 || (P) == 12 || (P) == 16) ? 1 : ((P) == 5 || (P) == 13 || (P) == 15 || (P) == 18) ? 2 : ((P) == 9 || (P) == 14 || (P) == 19) ? 3 : ((P) == 6 || (P) == 20) ? 4 : ((P) == 3 || (P) == 21) ? 5 :  6 )


// TinyCore
// https://raw.githubusercontent.com/xukangmin/TinyCore/master/avr/package/package_tinycore_index.json
// https://docs.tinycore.dev/en/latest/
#elif  defined(__AVR_ATtiny1616__)  || defined(__AVR_ATtiny3216__) || defined(__AVR_ATtiny3217__)
#define __digitalPinToPortReg(P) ((P) <= 5 ? &VPORTB.OUT : ((P) <= 9 ? &VPORTC.OUT : ((P) <= 16 ? &VPORTA.OUT : ((P) <= 18 ? &VPORTB.OUT : &VPORTC.OUT))))
#define __digitalPinToDDRReg(P) ((P) <= 5 ? &VPORTB.DIR : ((P) <= 9 ? &VPORTC.DIR : ((P) <= 16 ? &VPORTA.DIR : ((P) <= 18 ? &VPORTB.DIR : &VPORTC.DIR))))
#define __digitalPinToPINReg(P) ((P) <= 5 ? &VPORTB.IN : ((P) <= 9 ? &VPORTC.IN : ((P) <= 16 ? &VPORTA.IN : ((P) <= 18 ? &VPORTB.IN : &VPORTC.IN))))
#define __digitalPinToBit(P) ( (P) <= 3 ? (3 - P) : ((P) <= 5 ? (P) : ((P) <= 9 ? (P - 6) : ((P) <= 16 ? ((P) - 9) : ((P) <= 18 ? ((P) - 11) : ((P) - 15))))) )


// --- ATtinyX5 ---
#elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
// we have only PORTB
#define __digitalPinToPortReg(P) (&PORTB)
#define __digitalPinToDDRReg(P)  (&DDRB)
#define __digitalPinToPINReg(P)  (&PINB)
#define __digitalPinToBit(P)     (((P) <= 7) ? (P) : (((P) >= 8 && (P) <= 13) ? (P) - 8 : (P) - 14))


// --- ATtiny88 ---
#elif defined(__AVR_ATtiny88__)
# if defined(ARDUINO_AVR_DIGISPARKPRO)
#define __digitalPinToPortReg(P) ((P) <= 7 ? &PORTD : ((P) <= 14 ? &PORTB : ((P) <= 18 ? &PORTA : &PORTC)))
#define __digitalPinToDDRReg(P)  ((P) <= 7 ? &DDRD : ((P) <= 14 ? &DDRB : ((P) <= 18 ? &DDRA : &DDRC)))
#define __digitalPinToPINReg(P)  ((P) <= 7 ? &PIND : ((P) <= 14 ? &PINB : ((P) <= 18 ? &PINA : &PINC)))
#define __digitalPinToBit(P) ( (P) <= 7 ? (P) : ((P) <= 13 ? ((P) - 8) : ((P) == 14 ? 7 : ((P) <= 16 ? ((P) - 14) : ((P) <= 18 ? ((P) - 17) : ((P) == 25 ? 7 : ((P) - 19)))))) )
# else
#define __digitalPinToPortReg(P) ((P) <= 7 ? &PORTD : ((P) <= 15 ? &PORTB : ((P) <= 22 ? &PORTC : ((P) <= 26 ? &PORTA : &PORTC))))
#define __digitalPinToDDRReg(P) ((P) <= 7 ? &DDRD : ((P) <= 15 ? &DDRB : ((P) <= 22 ? &DDRC : ((P) <= 26 ? &DDRA : &DDRC))))
#define __digitalPinToPINReg(P) ((P) <= 7 ? &PIND : ((P) <= 15 ? &PINB : ((P) <= 22 ? &PINC : ((P) <= 26 ? &PINA : &PINC))))
#define __digitalPinToBit(P) ((P) <= 15 ? ((P) & 0x7) : ((P) == 16 ? (7) : ((P) <= 22 ? ((P) - 17) : ((P) == 27 ? (6) : ((P) - 23)))))
# endif


// --- ATtinyX4 + ATtinyX7 ---
#elif  defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__) || defined(__AVR_ATtiny87__) || defined(__AVR_ATtiny167__)
# if defined(ARDUINO_AVR_DIGISPARKPRO)
// Strange enumeration of pins on Digispark board and core library
#define __digitalPinToPortReg(P) (((P) <= 4) ? &PORTB : &PORTA)
#define __digitalPinToDDRReg(P)  (((P) <= 4) ? &DDRB : &DDRA)
#define __digitalPinToPINReg(P)  (((P) <= 4) ? &PINB : &PINA)
#define __digitalPinToBit(P)     (((P) <= 2) ? (P) : (((P) == 3) ? 6 : (((P) == 4) ? 3 : (((P) == 5) ? 7 : (P) - 6 ))))
# else
// ATtinyX4: PORTA for 0 to 7, PORTB for 8 to 11
// ATtinyX7: PORTA for 0 to 7, PORTB for 8 to 15
#define __digitalPinToPortReg(P) (((P) <= 7) ? &PORTA : &PORTB)
#define __digitalPinToDDRReg(P)  (((P) <= 7) ? &DDRA : &DDRB)
#define __digitalPinToPINReg(P)  (((P) <= 7) ? &PINA : &PINB)
#define __digitalPinToBit(P)     (((P) <= 7) ? (P) : (P) - 8 )
# endif

// --- Other ---
#else

#define I2C_SDA_PIN     SDA
#define I2C_SCL_PIN     SCL

#define SPI_HW_SS_PIN   SS
#define SPI_HW_MOSI_PIN MOSI
#define SPI_HW_MISO_PIN MISO
#define SPI_HW_SCK_PIN  SCK


#endif
//#endif  //#ifndef digitalPinToPortReg


#ifndef digitalWriteFast
#if (defined(__AVR__) || defined(ARDUINO_ARCH_AVR))
#define digitalWriteFast(P, V) \
if (__builtin_constant_p(P)) { \
  BIT_WRITE(*__digitalPinToPortReg(P), __digitalPinToBit(P), (V)); \
} else { \
  digitalWrite((P), (V)); \
}
#else
#define digitalWriteFast digitalWrite
#endif
#endif


#ifndef pinModeFast
#if (defined(__AVR__) || defined(ARDUINO_ARCH_AVR))
#define pinModeFast(P, V) \
if (__builtin_constant_p(P) && __builtin_constant_p(V)) { \
  if (V == INPUT_PULLUP) {\
    BIT_CLEAR(*__digitalPinToDDRReg(P), __digitalPinToBit(P)); \
    BIT_SET(*__digitalPinToPortReg(P), __digitalPinToBit(P)); \
  } else { \
    BIT_WRITE(*__digitalPinToDDRReg(P), __digitalPinToBit(P), (V)); \
  } \
} else { \
  pinMode((P), (V)); \
}
#else
#define pinModeFast pinMode
#endif
#endif


#ifndef digitalReadFast
#if (defined(__AVR__) || defined(ARDUINO_ARCH_AVR))
#define digitalReadFast(P) ( (int) __digitalReadFast((P)) )
#define __digitalReadFast(P ) \
  (__builtin_constant_p(P) ) ? \
  (( BIT_READ(*__digitalPinToPINReg(P), __digitalPinToBit(P))) ? HIGH:LOW ) : \
  digitalRead((P))
#else
#define digitalReadFast digitalRead
#endif
#endif


#ifndef digitalToggleFast
#if (defined(__AVR__) || defined(ARDUINO_ARCH_AVR))
#define digitalToggleFast(P) \
if (__builtin_constant_p(P)) { \
  BIT_SET(*__digitalPinToPINReg(P), __digitalPinToBit(P)); \
} else { \
  digitalWrite(P, ! digitalRead(P)); \
}
#else
#define digitalToggleFast(P) digitalWrite(P, ! digitalRead(P))
#endif
#endif


#endif //__digitalWriteFast_h_
