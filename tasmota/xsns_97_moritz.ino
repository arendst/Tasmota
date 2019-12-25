/*
  xsns_97_moritz.ino - CC1101 radio_modem MAX! support

  Copyright (C) 2019  Gerhard Mutz and Rudolf Koenig (culfw)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


/* to doo
1. set Thermostat values
2. set device names instead of hex id, ok (with eeprom)
*/

#ifdef USE_SPI
#ifdef USE_MORITZ

#define CC100_CS 0

#define XSNS_97 97

#define MORITZ_BASE_ADDRESS 0x567890
//#define MORITZ_BASE_ADDRESS 0x123456

#define MORITZ_SDEBUG

uint8_t moritz_ready = 0;
uint8_t moritz_on;
uint8_t moritz_flags;
uint8_t moritz_addr[3];
uint8_t moritz_cs;

#define MORITZ_SHOW_ALL 1
#define MORITZ_PAIR_ENABLE 2
#define MORITZ_EEPROM_FOUND 4

void ccInitChip(void);
void cc_factory_reset(void);
void ccDump(void);
void ccTX(void);
void ccRX(void);
uint8_t ccStrobe(uint8_t);
void ccreg(char*);
void ccsetpa(char*);
uint8_t cc1100_sendbyte(uint8_t data);
void cc1100_writeReg(uint8_t addr, uint8_t data);
uint8_t cc1100_readReg(uint8_t addr);
void set_ccoff(void);
void set_ccon(void);
void new_moritz(const char *hid,uint8_t type,uint8_t bs,uint8_t rfe,uint8_t bl,uint8_t rssi,uint8_t dtmp,uint16_t mtmp, uint8_t valve, uint8_t tmode);

uint8_t cc_on;


// max label size = 28
/*
#define MMLSIZ 29
struct MORITZ_LABEL {
  uint8_t id[3];
  char label[MMLSIZ];
};
*/

// Configuration Registers
#define CC1100_IOCFG2           0x00    // GDO2 output pin configuration
#define CC1100_IOCFG1           0x01    // GDO1 output pin configuration
#define CC1100_IOCFG0           0x02    // GDO0 output pin configuration
#define CC1100_FIFOTHR          0x03    // RX FIFO and TX FIFO thresholds
#define CC1100_SYNC1            0x04    // Sync word, high byte
#define CC1100_SYNC0            0x05    // Sync word, low byte
#define CC1100_PKTLEN           0x06    // Packet length
#define CC1100_PKTCTRL1         0x07    // Packet automation control
#define CC1100_PKTCTRL0         0x08    // Packet automation control
#define CC1100_ADDR             0x09    // Device address
#define CC1100_CHANNR           0x0A    // Channel number
#define CC1100_FSCTRL1          0x0B    // Frequency synthesizer control
#define CC1100_FSCTRL0          0x0C    // Frequency synthesizer control
#define CC1100_FREQ2            0x0D    // Frequency control word, high byte
#define CC1100_FREQ1            0x0E    // Frequency control word, middle byte
#define CC1100_FREQ0            0x0F    // Frequency control word, low byte
#define CC1100_MDMCFG4          0x10    // Modem configuration
#define CC1100_MDMCFG3          0x11    // Modem configuration
#define CC1100_MDMCFG2          0x12    // Modem configuration
#define CC1100_MDMCFG1          0x13    // Modem configuration
#define CC1100_MDMCFG0          0x14    // Modem configuration
#define CC1100_DEVIATN          0x15    // Modem deviation setting
#define CC1100_MCSM2            0x16    // Main Radio Cntrl State Machine config
#define CC1100_MCSM1            0x17    // Main Radio Cntrl State Machine config
#define CC1100_MCSM0            0x18    // Main Radio Cntrl State Machine config
#define CC1100_FOCCFG           0x19    // Frequency Offset Compensation config
#define CC1100_BSCFG            0x1A    // Bit Synchronization configuration
#define CC1100_AGCCTRL2         0x1B    // AGC control
#define CC1100_AGCCTRL1         0x1C    // AGC control
#define CC1100_AGCCTRL0         0x1D    // AGC control
#define CC1100_WOREVT1          0x1E    // High byte Event 0 timeout
#define CC1100_WOREVT0          0x1F    // Low byte Event 0 timeout
#define CC1100_WORCTRL          0x20    // Wake On Radio control
#define CC1100_FREND1           0x21    // Front end RX configuration
#define CC1100_FREND0           0x22    // Front end TX configuration
#define CC1100_FSCAL3           0x23    // Frequency synthesizer calibration
#define CC1100_FSCAL2           0x24    // Frequency synthesizer calibration
#define CC1100_FSCAL1           0x25    // Frequency synthesizer calibration
#define CC1100_FSCAL0           0x26    // Frequency synthesizer calibration
#define CC1100_RCCTRL1          0x27    // RC oscillator configuration
#define CC1100_RCCTRL0          0x28    // RC oscillator configuration
#define CC1100_FSTEST           0x29    // Frequency synthesizer cal control
#define CC1100_PTEST            0x2A    // Production test
#define CC1100_AGCTEST          0x2B    // AGC test
#define CC1100_TEST2            0x2C    // Various test settings
#define CC1100_TEST1            0x2D    // Various test settings
#define CC1100_TEST0            0x2E    // Various test settings

// Status registers
#define CC1100_PARTNUM          0x30    // Part number
#define CC1100_VERSION          0x31    // Current version number
#define CC1100_FREQEST          0x32    // Frequency offset estimate
#define CC1100_LQI              0x33    // Demodulator estimate for link quality
#define CC1100_RSSI             0x34    // Received signal strength indication
#define CC1100_MARCSTATE        0x35    // Control state machine state
#define CC1100_WORTIME1         0x36    // High byte of WOR timer
#define CC1100_WORTIME0         0x37    // Low byte of WOR timer
#define CC1100_PKTSTATUS        0x38    // Current GDOx status and packet status
#define CC1100_VCO_VC_DAC       0x39    // Current setting from PLL cal module
#define CC1100_TXBYTES          0x3A    // Underflow and # of bytes in TXFIFO
#define CC1100_RXBYTES          0x3B    // Overflow and # of bytes in RXFIFO

// Multi byte memory locations
#define CC1100_PATABLE          0x3E
#define CC1100_TXFIFO           0x3F
#define CC1100_RXFIFO           0x3F

// Definitions for burst/single access to registers
#define CC1100_WRITE_BURST      0x40
#define CC1100_READ_SINGLE      0x80
#define CC1100_READ_BURST       0xC0

// Strobe commands
#define CC1100_SRES             0x30        // Reset chip.
#define CC1100_SFSTXON          0x31        // Enable and calibrate frequency synthesizer (if MCSM0.FS_AUTOCAL=1).
                                            // If in RX/TX: Go to a wait state where only the synthesizer is
                                            // running (for quick RX / TX turnaround).
#define CC1100_SXOFF            0x32        // Turn off crystal oscillator.
#define CC1100_SCAL             0x33        // Calibrate frequency synthesizer and turn it off
                                            // (enables quick start).
#define CC1100_SRX              0x34        // Enable RX. Perform calibration first if coming from IDLE and
                                            // MCSM0.FS_AUTOCAL=1.
#define CC1100_STX              0x35        // In IDLE state: Enable TX. Perform calibration first if
                                            // MCSM0.FS_AUTOCAL=1. If in RX state and CCA is enabled:
                                            // Only go to TX if channel is clear.
#define CC1100_SIDLE            0x36        // Exit RX / TX, turn off frequency synthesizer and exit
                                            // Wake-On-Radio mode if applicable.
#define CC1100_SAFC             0x37        // Perform AFC adjustment of the frequency synthesizer
#define CC1100_SWOR             0x38        // Start automatic RX polling sequence (Wake-on-Radio)
#define CC1100_SPWD             0x39        // Enter power down mode when CSn goes high.
#define CC1100_SFRX             0x3A        // Flush the RX FIFO buffer.
#define CC1100_SFTX             0x3B        // Flush the TX FIFO buffer.
#define CC1100_SWORRST          0x3C        // Reset real time clock.
#define CC1100_SNOP             0x3D        // No operation. May be used to pad strobe commands to two
                                            // bytes for simpler software.


//------------------------------------------------------------------------------
// Chip Status Byte
//------------------------------------------------------------------------------

// Bit fields in the chip status byte
#define CC1100_STATUS_CHIP_RDYn_BM             0x80
#define CC1100_STATUS_STATE_BM                 0x70
#define CC1100_STATUS_FIFO_BYTES_AVAILABLE_BM  0x0F

// Chip states
#define CC1100_STATE_IDLE                      0x00
#define CC1100_STATE_RX                        0x10
#define CC1100_STATE_TX                        0x20
#define CC1100_STATE_FSTXON                    0x30
#define CC1100_STATE_CALIBRATE                 0x40
#define CC1100_STATE_SETTLING                  0x50
#define CC1100_STATE_RX_OVERFLOW               0x60
#define CC1100_STATE_TX_UNDERFLOW              0x70


//------------------------------------------------------------------------------
// Other register bit fields
//------------------------------------------------------------------------------
#define CC1100_LQI_CRC_OK_BM                   0x80
#define CC1100_LQI_EST_BM                      0x7F

#define MARCSTATE_SLEEP            0x00
#define MARCSTATE_IDLE             0x01
#define MARCSTATE_XOFF             0x02
#define MARCSTATE_VCOON_MC         0x03
#define MARCSTATE_REGON_MC         0x04
#define MARCSTATE_MANCAL           0x05
#define MARCSTATE_VCOON            0x06
#define MARCSTATE_REGON            0x07
#define MARCSTATE_STARTCAL         0x08
#define MARCSTATE_BWBOOST          0x09
#define MARCSTATE_FS_LOCK          0x0A
#define MARCSTATE_IFADCON          0x0B
#define MARCSTATE_ENDCAL           0x0C
#define MARCSTATE_RX               0x0D
#define MARCSTATE_RX_END           0x0E
#define MARCSTATE_RX_RST           0x0F
#define MARCSTATE_TXRX_SWITCH      0x10
#define MARCSTATE_RXFIFO_OVERFLOW  0x11
#define MARCSTATE_FSTXON           0x12
#define MARCSTATE_TX               0x13
#define MARCSTATE_TX_END           0x14
#define MARCSTATE_RXTX_SWITCH      0x15
#define MARCSTATE_TXFIFO_UNDERFLOW 0x16

typedef enum {
  CC_Pin_Out = 0,
  CC_Pin_CS,
  CC_Pin_In
} CC_PIN;

typedef enum {
  GPIO_PIN_RESET = 0,
  GPIO_PIN_SET
} GPIO_PinState;



#define CC1100_DEASSERT  	digitalWrite(moritz_cs,1);SPI.endTransaction();
#define CC1100_ASSERT    	SPI.beginTransaction(moritz_spiSettings);digitalWrite( moritz_cs,0)

//#define CC1100_SET_OUT		CC1100_OUT_PORT |= _BV(CC1100_OUT_PIN)
//#define CC1100_CLEAR_OUT	CC1100_OUT_PORT &= ~_BV(CC1100_OUT_PIN)

#define CCCOUNT 1
#define INIT_MODE_OUT_CS_IN    0
#define INIT_MODE_IN_CS_IN     1

#define REP_BINTIME 0
#define REP_RSSI 1

#define MAX_MORITZ_MSG 30

//Internal Transceiver
#define CC1100_0_CS_PIN		  12
#define CC1100_0_CS_BASE	  AT91C_BASE_PIOA
#define CC1100_0_OUT_PIN    30
#define CC1100_0_OUT_BASE   AT91C_BASE_PIOA
#define CC1100_0_IN_PIN     29
#define CC1100_0_IN_BASE	  AT91C_BASE_PIOA

//transceiver_t CCtransceiver[] = CCTRANSCEIVERS;


void rf_moritz_init(void);
void rf_moritz_task(void);
void moritz_func(char *in);

/*
#include <avr/io.h>                     // for _BV, bit_is_set
#include <stdint.h>                     // for uint8_t, uint32_t

#include "board.h"                      // for CC1100_CS_DDR, etc
#include "led.h"                        // for SET_BIT
#include "stringfunc.h"                 // for fromhex
#ifdef HAS_MORITZ
#include <avr/pgmspace.h>               // for PSTR, pgm_read_byte, etc

#include "cc1100.h"                     // for cc1100_readReg, ccStrobe, etc
#include "clock.h"                      // for ticks
#include "delay.h"                      // for my_delay_ms, my_delay_us
#include "display.h"                    // for DC, DH2, DNL, DS_P
#include "fband.h"                      // for checkFrequency
#include "rf_moritz.h"
#include "rf_receive.h"                 // for REP_RSSI, set_txrestore, etc
#include "rf_send.h"                    // for credit_10ms
#include "multi_CC.h"
#include "rf_mode.h"
*/

#define CC_INSTANCE       0
//#define TX_REPORT         (CC1101.tx_report[0])
#define TX_REPORT         1


void moritz_sendraw(uint8_t* buf, int longPreamble);
void moritz_sendAck(uint8_t* enc);
void moritz_handleAutoAck(uint8_t* enc);

/*
 * CC1100_PKTCTRL0.LENGTH_CONFIG = 1 //Variable packet length mode. Packet length configured by the first byte after sync word
 *                 CRC_EN = 1
 *                 PKT_FORMAT = 00 //Use FIFOs
 *                 WHITE_DATA = 0
 * MDMCFG2.SYNC_MODE = 3: 30/32 sync word bits detected
 *        .MANCHESTER_EN = 0
 *        .MOD_FORMAT = 0: 2-FSK
 *        .DEM_DCFILT_OFF = 0
 *
 * EVENT0 = 34667
 * t_Event0 = 750/26Mhz * EVENT0 * 2^(5*WOR_RES) = 1 second
 *
 * One message with 12 payload bytes takes (4 byte preamble + 4 byte sync + 12 byte payload) / 1kbit/s = 160 ms.
 */
const uint8_t PROGMEM MORITZ_CFG[60] = {
//     0x00, 0x08,
     0x00, 0x07, //IOCFG2: GDO2_CFG=7: Asserts when a packet has been received with CRC OK. De-asserts when the first byte is read from the RX FIFO
     0x02, 0x46, //IOCFG0
     0x04, 0xC6, //SYNC1
     0x05, 0x26, //SYNC0
     0x0B, 0x06, //FSCTRL1
     0x10, 0xC8, //MDMCFG4 DRATE_E=8,
     0x11, 0x93, //MDMCFG3 DRATE_M=147, data rate = (256+DRATE_M)*2^DRATE_E/2^28*f_xosc = (9992.599) 1kbit/s (at f_xosc=26 Mhz)
     0x12, 0x03, //MDMCFG2 see above
     0x13, 0x22,  //MDMCFG1 CHANSPC_E=2, NUM_PREAMBLE=2 (4 bytes), FEC_EN = 0 (disabled)
    //0x13, 0x72,  //MDMCFG1 CHANSPC_E=2, NUM_PREAMBLE=7 (24 bytes), FEC_EN = 0 (disabled)
     0x15, 0x34, //DEVIATN
//     0x17, 0x00,
     0x17, 0x3F, //MCSM1: TXOFF=RX, RXOFF=RX, CCA_MODE=3:If RSSI below threshold unless currently receiving a packet
     0x18, 0x28, //MCSM0: PO_TIMEOUT=64, FS_AUTOCAL=2: When going from idle to RX or TX automatically
     0x19, 0x16, //FOCCFG
     0x1B, 0x43, //AGCTRL2
     0x21, 0x56, //FREND1
     0x25, 0x00, //FSCAL1
     0x26, 0x11, //FSCAL0
     0x0D, 0x21, //FREQ2
     0x0E, 0x65, //FREQ1
     0x0F, 0x6A, //FREQ0
     0x07, 0x0C, //PKTCTRL1
     0x16, 0x07, //MCSM2 RX_TIME = 7 (Timeout for sync word search in RX for both WOR mode and normal RX operation = Until end of packet) RX_TIME_QUAL=0 (check if sync word is found)
     0x20, 0xF8, //WORCTRL, WOR_RES=00 (1.8-1.9 sec) EVENT1=7 (48, i.e. 1.333 – 1.385 ms)
     0x1E, 0x87, //WOREVT1 EVENT0[high]
     0x1F, 0x6B, //WOREVT0 EVENT0[low]
     0x29, 0x59, //FSTEST
     0x2C, 0x81, //TEST2
     0x2D, 0x35, //TEST1
     0x3E, 0xC3, //?? Readonly PATABLE?
     0xff
};


#define DC display_char
#define DS display_string
#define DS_P display_string_P
#define DU(a,b) display_udec(a,b,' ')
#define DH(a,b) display_hex(a,b,'0')
#define DH2(a) display_hex2(a)
#define DNL display_nl


static uint8_t autoAckAddr[3] = {0, 0, 0};
static uint8_t fakeWallThermostatAddr[3] = {0, 0, 0};
static uint32_t lastSendingTicks = 0;


#define my_delay_us delayMicroseconds
#define my_delay_ms delay


void display_char(char c) {
  Serial.printf("%c",c);
}

void display_string(char *s) {
  while (*s) display_char(*s++);
}

void display_string_P(const char *s) {
  uint8_t c;
    while ((c = pgm_read_byte(s))) {
      display_char(c);
      s++;
    }
}

void display_nl(void) {
  display_char('\r');
  display_char('\n');
}

void display_hex(uint16_t h, int8_t pad, uint8_t padc) {
  char buf[5];
  uint8_t i=5;

  buf[--i] = 0;
  do {
    uint8_t m = h%16;
    buf[--i] = (m < 10 ? '0'+m : 'A'+m-10);
    h /= 16;
    pad--;
  } while (h);

  while (--pad >= 0 && i > 0)
    buf[--i] = padc;
  DS(buf+i);
}

void display_hex2(uint8_t h) {
  display_hex(h, 2, '0');
}

void ccRX(void) {
  uint8_t cnt = 0xff;
  while(cnt-- && (ccStrobe(CC1100_SRX) & CC1100_STATUS_STATE_BM) != CC1100_STATE_RX)
    my_delay_us(10);
}

void ccTX(void) {
  uint8_t cnt = 0xff;
  //GIMSK  &= ~_BV(CC1100_INT);

  // Going from RX to TX does not work if there was a reception less than 0.5
  // sec ago. Due to CCA? Using IDLE helps to shorten this period(?)
  ccStrobe(CC1100_SIDLE);
  while(cnt-- && (ccStrobe( CC1100_STX ) & 0x70) != 2)
    my_delay_us(10);
}

void set_ccoff(void) {
  ccStrobe(CC1100_SIDLE);
  cc_on = 0;
}

void set_ccon(void) {
  //ccInitChip();
  cc_on = 1;
}

/*
ccInitChip(void) {

  GIMSK &= ~_BV(CC1100_INT);
  SET_BIT( CC1100_CS_DDR, CC1100_CS_PIN ); // CS as output
  SET_BIT(CC1100_OUT_DDR, CC1100_OUT_PIN); // GDO0 as output
  CLEAR_BIT(CC1100_IN_DDR, CC1100_IN_PIN); // GDO2 as input


  CC1100_DEASSERT;                           // Toggle chip select signal
  _delay_us(30);
  CC1100_ASSERT;
  _delay_us(30);
  CC1100_DEASSERT;
  _delay_us(45);

  ccStrobe( CC1100_SRES );                   // Send SRES command
  _delay_us(100);

  CC1100_ASSERT;                             // load configuration
  cc1100_sendbyte( 0 | CC1100_WRITE_BURST );
  for(uint8_t i = 0; i < EE_CC1100_CFG_SIZE; i++) {
    cc1100_sendbyte(CC1100_CFG[i]);
  }
  CC1100_DEASSERT;

  CC1100_ASSERT;                             // setup PA table
  cc1100_sendbyte( CC1100_PATABLE | CC1100_WRITE_BURST );
  for (uint8_t i = 0;i<CC1100_PA_SIZE;i++) {
    cc1100_sendbyte(CC1100_PA[i]);
  }
  CC1100_DEASSERT;

  ccStrobe( CC1100_SCAL );
  _delay_ms(1);
}
*/

SPISettings moritz_spiSettings;

uint8_t cc1100_sendbyte(uint8_t data) {
  return SPI.transfer(data);
}

uint8_t cc1100_readReg(uint8_t addr) {
  CC1100_ASSERT;
  cc1100_sendbyte( addr|CC1100_READ_BURST );
  uint8_t ret = cc1100_sendbyte( 0 );
  CC1100_DEASSERT;
  return ret;
}

void cc1100_writeReg(uint8_t addr, uint8_t data) {
  CC1100_ASSERT;
  cc1100_sendbyte( addr|CC1100_WRITE_BURST );
  cc1100_sendbyte( data );
  CC1100_DEASSERT;
}

uint8_t ccStrobe(uint8_t strobe) {
  CC1100_ASSERT;
  uint8_t ret = cc1100_sendbyte( strobe );
  CC1100_DEASSERT;
  return ret;
}

int fromhex(const char *in, uint8_t *out, uint8_t buflen) {
  uint8_t *op = out, c, h = 0, step = 0;
  while((c = *in++)) {
    if(c >= 'a')
      c -= 'a'-'A';
    if(c >= '0' && c <= '9') {
      h |= c-'0';
    } else if(c >= 'A' && c <= 'F') {
      h |= c-'A'+10;
    } else  {
      if(c != ' ' && c != ':')
        break;
      continue;
    }
    if(step++) {
      *op++ = h;
      if(--buflen <= 0)
        return (op-out);
      step = 0;
      h = 0;
    } else {
      h <<= 4;
    }
  }
  return op-out;
}

#define MODE_UNKNOWN    0
#define MODE_433_MHZ    1
#define MODE_868_MHZ    2

uint8_t frequencyMode = MODE_UNKNOWN;

/*
 * Check the frequency bits and set the frequencyMode value
 */
void checkFrequency(void) {
  /*
  uint32_t value_0D;
  uint16_t value_0E;
  uint8_t  value_0F;
  {
  value_0D = readEEpromValue("0F");
  value_0E = readEEpromValue("10");
  value_0F = readEEpromValue("11");
  }

  uint16_t frequency = 26*(value_0D*256*256+value_0E*256+value_0F)/65536;

  if (frequency > 500) {
    frequencyMode = MODE_868_MHZ;
  } else {
    frequencyMode = MODE_433_MHZ;
  }

*/
  frequencyMode = MODE_868_MHZ;
}

void set_txrestore() {

  if (TX_REPORT) {
    set_ccon();
    ccRX();
  } else {
    set_ccoff();
  }
}

void rf_moritz_init(void) {
//  hal_CC_GDO_init(CC_INSTANCE,INIT_MODE_IN_CS_IN);
//  hal_enable_CC_GDOin_int(CC_INSTANCE,false); // disable INT - we'll poll...

  CC1100_DEASSERT;                           // Toggle chip select signal
  my_delay_us(30);
  CC1100_ASSERT;
  my_delay_us(30);
  CC1100_DEASSERT;
  my_delay_us(45);

  ccStrobe( CC1100_SRES );                   // Send SRES command
  my_delay_us(100);

  // load configuration
  for (uint8_t i = 0; i<60; i += 2) {
    if (pgm_read_byte( &MORITZ_CFG[i] )>0x40)
      break;

    cc1100_writeReg( pgm_read_byte(&MORITZ_CFG[i]),
                     pgm_read_byte(&MORITZ_CFG[i+1]) );
  }

  ccStrobe( CC1100_SCAL );

  my_delay_ms(4); // 4ms: Found by trial and error
  //This is ccRx() but without enabling the interrupt
  uint8_t cnt = 0xff;
  //Enable RX. Perform calibration first if coming from IDLE and MCSM0.FS_AUTOCAL=1.
  //Why do it multiple times?
  while (cnt-- && (ccStrobe( CC1100_SRX ) & 0x70) != 1)
    my_delay_us(10);

  moritz_on = 1;
  checkFrequency();
}

void moritz_handleAutoAck(uint8_t* enc) {

  if (enc[3] == 0x30 /* type ShutterContactState */
    || enc[3] == 0x40 /* type SetTemperature */
    || enc[3] == 0x50 /* type PushButtonState */
  ) {
    moritz_sendAck(enc);
  }
  return;

  //Send acks to when required by "spec"
  if ((autoAckAddr[0] != 0 || autoAckAddr[1] != 0 || autoAckAddr[2] != 0) /* auto-ack enabled */
      && (
           enc[3] == 0x30 /* type ShutterContactState */
        || enc[3] == 0x40 /* type SetTemperature */
        || enc[3] == 0x50 /* type PushButtonState */
        )
      && enc[7] == autoAckAddr[0] /* dest */
      && enc[8] == autoAckAddr[1]
      && enc[9] == autoAckAddr[2])
    moritz_sendAck(enc);

  if ((fakeWallThermostatAddr[0] != 0 || fakeWallThermostatAddr[1] != 0 || fakeWallThermostatAddr[2] != 0) /* fake enabled */
      && enc[0] == 11 /* len */
      && enc[3] == 0x40 /* type SetTemperature */
      && enc[7] == fakeWallThermostatAddr[0] /* dest */
      && enc[8] == fakeWallThermostatAddr[1]
      && enc[9] == fakeWallThermostatAddr[2])
    moritz_sendAck(enc);

  return;
}

struct culpaket {
  uint8_t  length;
  uint8_t  messageCount;
  uint8_t  flag;
  uint8_t  groupid;
  uint8_t  source[3];
  uint8_t  dest[3];
  uint8_t  rawType;
  char     rawPayload[22];
  uint8_t  forMe;
  char     command[1];
  uint8_t  Payloadlength;
};

enum  ThermostatControlMode {AUTO,MANUAL,TEMPORARY,BOOST};

#define THERMOSTAT_STATE_TIME_PAYLOAD_LEN 6
#define THERMOSTAT_STATE_MEAS_PAYLOAD_LEN 5
#define THERMOSTAT_STATE_SHORT_PAYLOAD_LEN 3

void moritz_sendMsg(uint8_t cmdId, uint32_t src,uint32_t dst, uint8_t *payload, uint8_t groupId, uint8_t flags, uint8_t plen) {
  struct culpaket cp;

//S0A 00 00 00 567890 051FFC
//Z 0B 01 00 01 123456 051FFC 00 00 37

  cp.length=10+plen; // payload length
  cp.messageCount=1;
  cp.flag=flags;
  cp.groupid=groupId;
  cp.source[0]=src>>16;
  cp.source[1]=src>>8;
  cp.source[2]=src;
  cp.dest[0]=dst>>16;
  cp.dest[1]=dst>>8;
  cp.dest[2]=dst;
  cp.rawType=0;
  cp.rawPayload[0]=*payload;

  uint8_t *ucp=(uint8_t *)&cp;
  moritz_sendraw(ucp, 1);

#ifdef MORITZ_SDEBUG
  DC('S');
  for (uint8_t cnt=0;cnt<cp.length+1;cnt++) {
    DH2(ucp[cnt]);
  }
  DNL();
#endif

}

/* mode
0 => auto weekly
1 => manual
2 => vacation
3 => boost
4 => manual eco
5 => manual comfort
6 => manual window
*/
void SendModeTmp(uint32_t dst,uint8_t mode,float tmp) {
uint8_t bc[8]={0,0x81,0x82,0x83,2,3,4,0};
  mode=bc[mode&7];
  if (tmp<=4.5) {
    tmp=4.5;
  }
  if (tmp>=30.5) {
      tmp=30.5;
  }
  uint32_t man;
  man=(moritz_addr[0]<<16)|(moritz_addr[1]<<8)|(moritz_addr[2]);
  uint8_t payload[2];

  if (mode==0) {
      payload[0] = 0x00;
  } else if (mode==2) {
      payload[0] = 0x41;
  } else if (mode==3) {
      payload[0] = 0x42;
  } else if (mode==4) {
      payload[0] = 0x43;
  } else {
      payload[0]=((uint8_t)(tmp*2)&0x3f)|((mode&0x3)<<6);
  }

  moritz_sendMsg(40,man,dst,payload,1,0,1);
}

#if 0


MaxDriver.prototype.sendConfig = function (dest, comfortTemperature, ecoTemperature, minimumTemperature, maximumTemperature, offset, windowOpenTime, windowOpenTemperature, deviceType) {
            var comfortTemperatureValue, ecoTemperatureValue, maximumTemperaturenValue, minimumTemperatureValue, offsetValue, payload, windowOpenTempValue, windowOpenTimeValue;
            comfortTemperatureValue = Sprintf('%02x', comfortTemperature * 2);
            ecoTemperatureValue = Sprintf('%02x', ecoTemperature * 2);
            minimumTemperatureValue = Sprintf('%02x', minimumTemperature * 2);
            maximumTemperaturenValue = Sprintf('%02x', maximumTemperature * 2);
            offsetValue = Sprintf('%02x', (parseFloat(offset) + 3.5) * 2);
            windowOpenTempValue = Sprintf('%02x', windowOpenTemperature * 2);
            windowOpenTimeValue = Sprintf('%02x', Math.ceil(windowOpenTime / 5));
            payload = comfortTemperatureValue + ecoTemperatureValue + maximumTemperaturenValue + minimumTemperatureValue + offsetValue + windowOpenTempValue + windowOpenTimeValue;
            this.sendMsg('11', this.baseAddress, dest, payload, '00', '00', deviceType);
            return Promise.resolve(true);
        };

#endif

void rf_moritz_task(void) {
  uint8_t enc[MAX_MORITZ_MSG];
  int16_t rssi;
  struct culpaket cp;
  uint8_t ctrlMode;
  float measuredTemperature=0;

  if (!moritz_on) return;

  // see if a CRC OK pkt has been arrived
  uint8_t crc_ok=cc1100_readReg( CC1100_PKTSTATUS ); // crc ok
  if (crc_ok&4) {

    //errata #1 does not affect us, because we wait until packet is completely received
    enc[0] = cc1100_readReg( CC1100_RXFIFO ) & 0x7f; // read len

    if (enc[0]>=MAX_MORITZ_MSG) enc[0] = MAX_MORITZ_MSG-1;

    CC1100_ASSERT;
    cc1100_sendbyte( CC1100_READ_BURST | CC1100_RXFIFO );

    for (uint8_t i=0; i<enc[0]; i++) {
      enc[i+1] = cc1100_sendbyte( 0 );
    }

    // RSSI is appended to RXFIFO
    rssi = cc1100_sendbyte( 0 );
    // And Link quality indicator, too
    /* LQI = */
    cc1100_sendbyte( 0 );

    CC1100_DEASSERT;


    if (TX_REPORT & REP_BINTIME) {
      DC('z');
      for (uint8_t i=0; i<=enc[0]; i++) DC( enc[i] );
    } else {

#ifdef MORITZ_SDEBUG
      DC('Z');
      for (uint8_t i=0; i<=enc[0]; i++) DH2( enc[i] );
      if (TX_REPORT & REP_RSSI) DH2(rssi);
      DNL();
#endif

      // decode package
      if (rssi >= 128) {
        rssi = (rssi - 256) / 2 - 74;
      } else {
        rssi = rssi / 2 - 74;
      }
      cp.messageCount=enc[1];
      cp.flag=enc[2];
      cp.rawType=enc[3];
      memcpy(cp.source,&enc[4],3);
      memcpy(cp.dest,&enc[7],3);
      cp.groupid=enc[10];
      cp.Payloadlength=enc[0]-10;
      memcpy(cp.rawPayload,&enc[11],cp.Payloadlength);

      uint32_t src=(cp.source[0]<<16)|(cp.source[1]<<8)|cp.source[2];
      uint32_t dst=(cp.dest[0]<<16)|(cp.dest[1]<<8)|cp.dest[2];

      uint32_t man;
      man=(moritz_addr[0]<<16)|(moritz_addr[1]<<8)|(moritz_addr[2]);

      if (man==dst) {
        cp.forMe=1;
      } else {
        cp.forMe=0;
      }

      if (cp.forMe) moritz_handleAutoAck(enc);

      char id[6];
      id[0]=0;
      for (uint8_t i=0;i<3;i++) {
        sprintf(id,"%s%02X",id,cp.source[i]);
      }

      char params[128];
      switch (cp.rawType) {
        case 0:
          // pairing
#ifdef MORITZ_SDEBUG
          Serial.printf("pair: \n");
#endif
          if (moritz_flags&MORITZ_PAIR_ENABLE) {
            uint8_t payload[1];
            payload[0]=0;
            moritz_sendMsg(3,man,src,payload,1,0,1);
          }
          break;
        case 1:
          // pingpong
#ifdef MORITZ_SDEBUG
          Serial.printf("pingpong: \n");
#endif
          break;
        case 2:
          // ack
#ifdef MORITZ_SDEBUG
          Serial.printf("ack: \n");
#endif
          break;
        case 3:
          // time
#ifdef MORITZ_SDEBUG
          Serial.printf("time: \n");
#endif
          if (moritz_flags&MORITZ_PAIR_ENABLE) {
            uint8_t payload[5];
            payload[0]=  RtcTime.year;
            payload[1]=  RtcTime.day_of_month;
            payload[2]=  RtcTime.hour;
            payload[3]=  RtcTime.minute|((RtcTime.month&0x0c)<<4);
            payload[4]=  RtcTime.second|((RtcTime.month&0x03)<<6);
            moritz_sendMsg(1,man,src,payload,1,4,5);
          }
          break;
        case 0x30:
          // shutterContactState
          // isopen,rferror,batlow
          if (cp.forMe || (moritz_flags&MORITZ_SHOW_ALL)) {
            sprintf_P(params,PSTR("\"opn\":%d,\"rfe\":%d,\"bl\":%d,\"rssi\":%d"),(cp.rawPayload[0]>>1)&1,(cp.rawPayload[0]>>6)&1,(cp.rawPayload[0]>>7)&1,rssi);
            new_moritz(&cp.source[0],1,(cp.rawPayload[0]>>1)&1,(cp.rawPayload[0]>>6)&1,(cp.rawPayload[0]>>7)&1,rssi,0,0,0,0);
            moritz_mqtt(id,"WC",params);
          }
          break;
        case 0x42:
          // wall thermo
#ifdef MORITZ_SDEBUG
          Serial.printf("walltc: \n");
#endif
          break;
        case 0x50:
          // pushButtonState
          // buttonnr,(0,1),rferror,batlow
          if (cp.forMe || (moritz_flags&MORITZ_SHOW_ALL)) {
            sprintf_P(params,PSTR("\"bn\":%d,\"rfe\":%d,\"bl\":%d,\"rssi\":%d"),cp.rawPayload[1]&1,(cp.rawPayload[0]>>6)&1,(cp.rawPayload[0]>>7)&1,rssi);
            new_moritz(&cp.source[0],0,cp.rawPayload[1]&1,(cp.rawPayload[0]>>6)&1,(cp.rawPayload[0]>>7)&1,rssi,0,0,0,0);
            moritz_mqtt(id,"PB",params);
          }
          break;
        case 0x60:
          //ThermostatState
          ctrlMode=cp.rawPayload[0]&3;
          // ctrlMode = ThermostatControlMode 0 0-2;
          // dst status = 0,3
          // langateway = 0,4
          // lockedForManualSetPoint = 0,5
          // valve position = 1

          // desiredTemperature = (2 & 0x7f) / 2.0;
          if (cp.Payloadlength==THERMOSTAT_STATE_MEAS_PAYLOAD_LEN) {
              if (ctrlMode != TEMPORARY) {
                  uint16_t mTemp = (cp.rawPayload[3] & 0x1);
                  mTemp <<= 8;
                  mTemp |= ((cp.rawPayload[4]) & 0xff);
                  measuredTemperature = mTemp / 10.0; // temperature over 25.5
                                                      // uses extra bit in
                                                      // desiredTemperature
                                                      // byte
                  if (measuredTemperature < 4.5) {
                      measuredTemperature = 0;
                  }
              }
          }
          if (cp.forMe || (moritz_flags&MORITZ_SHOW_ALL)) {
            char ts1[8],ts2[8];
            float tmp=(float)(cp.rawPayload[2]&0x7f)/2.0;
            dtostrfd(tmp,1,ts1);
            dtostrfd(measuredTemperature,1,ts2);
            sprintf_P(params,PSTR("\"dtmp\":%s,\"mtmp\":%s,\"vpos\":%d,\"cmo\":%d,\"rfe\":%d,\"bl\":%d,\"rssi\":%d"),ts1,ts2,cp.rawPayload[1],ctrlMode,(cp.rawPayload[0]>>6)&1,(cp.rawPayload[0]>>7)&1,rssi);
            new_moritz(&cp.source[0],2,cp.rawPayload[1]&1,(cp.rawPayload[0]>>6)&1,(cp.rawPayload[0]>>7)&1,rssi,measuredTemperature*10,(cp.rawPayload[2]&0x7f),cp.rawPayload[1],ctrlMode);
            moritz_mqtt(id,"TERM",params);
          }
          break;
        case 0x70:
          // wall thermo
#ifdef MORITZ_SDEBUG
          Serial.printf("wallth: \n");
#endif
          break;
        default:
#ifdef MORITZ_SDEBUG
          Serial.printf("unknown: \n");
#endif
          break;
      }
    }
    return;
}

    if (cc1100_readReg( CC1100_MARCSTATE ) == 17) {
      ccStrobe( CC1100_SFRX  );
      ccStrobe( CC1100_SIDLE );
      ccStrobe( CC1100_SRX   );
    }
  }

void moritz_send(char *in) {
  /* we are not affected by CC1101 errata #6, because MDMCFG2.SYNC_MODE != 0 */
  uint8_t dec[MAX_MORITZ_MSG];

  uint8_t hblen = fromhex(in+1, dec, MAX_MORITZ_MSG-1);

  if ((hblen-1) != dec[0]) {
    //MULTICC_PREFIX();
    DS_P(PSTR("LENERR\r\n"));
    return;
  }
  moritz_sendraw(dec, 1);
}

#define MAX_CREDIT 3600
// max 36 seconds burst / 100% of the hourly budget

uint16_t credit_10ms = MAX_CREDIT/2;
volatile uint32_t ticks;

/* longPreamble is necessary for unsolicited messages to wakeup the receiver */
void moritz_sendraw(uint8_t *dec, int longPreamble) {
  uint8_t hblen = dec[0]+1;
  //1kb/s = 1 bit/ms. we send 1 sec preamble + hblen*8 bits
  uint32_t sum = (longPreamble ? 100 : 0) + (hblen*8)/10;
  if (credit_10ms < sum) {
    //MULTICC_PREFIX();
    DS_P(PSTR("LOVF\r\n"));
    return;
  }
  credit_10ms -= sum;

  // in Moritz mode already?
  if (!moritz_on) {
    rf_moritz_init();
  }


  if (cc1100_readReg( CC1100_MARCSTATE ) != MARCSTATE_RX) { //error
    //MULTICC_PREFIX();
    DC('Z');
    DC('E');
    DC('R');
    DC('R');
    DC('1');
    DH2(cc1100_readReg( CC1100_MARCSTATE ));
    DNL();
    rf_moritz_init();
    return;
  }

  /* We have to keep at least 20 ms of silence between two sends
   * (found out by trial and error). ticks runs at 125 Hz (8 ms per tick),
   * so we wait for 3 ticks.
   * This looks a bit cumbersome but handles overflows of ticks gracefully.
   */
  //if (lastSendingTicks)
  //  while (ticks == lastSendingTicks || ticks == lastSendingTicks+1)
  //    my_delay_ms(1);
  my_delay_ms(20);


  /* Enable TX. Perform calibration first if MCSM0.FS_AUTOCAL=1 (this is the case) (takes 809μs)
   * start sending - CC1101 will send preamble continuously until TXFIFO is filled.
   * The preamble will wake up devices. See http://e2e.ti.com/support/low_power_rf/f/156/t/142864.aspx
   * It will not go into TX mode instantly if channel is not clear (see CCA_MODE), thus ccTX tries multiple times */
  ccTX();

  if (cc1100_readReg( CC1100_MARCSTATE ) != MARCSTATE_TX) { //error
    //MULTICC_PREFIX();
    DC('Z');
    DC('E');
    DC('R');
    DC('R');
    DC('2');
    DH2(cc1100_readReg( CC1100_MARCSTATE ));
    DNL();
    rf_moritz_init();
    return;
  }

  if (longPreamble) {
    /* Send preamble for 1 sec. Keep in mind that waiting for too long may trigger the watchdog (2 seconds on CUL) */
    for(int i=0;i<10;++i)
      my_delay_ms(100); //arg is uint_8, so loop
  }

  // send
  CC1100_ASSERT;
  cc1100_sendbyte(CC1100_WRITE_BURST | CC1100_TXFIFO);

  for (uint8_t i = 0; i < hblen; i++) {
    cc1100_sendbyte(dec[i]);
  }

  CC1100_DEASSERT;

  //Wait for sending to finish (CC1101 will go to RX state automatically
  //after sending
  uint8_t i;
  uint8_t stat1,stat2;
  for (i=0; i< 250;++i) {
    stat1=cc1100_readReg( CC1100_MARCSTATE );
    stat2=cc1100_readReg( CC1100_MARCSTATE );
    if (stat1!=stat2)
        continue;
    if (stat1 == MARCSTATE_RX)
      break; //now in RX, good

    my_delay_ms(1);
  }

  if (cc1100_readReg( CC1100_MARCSTATE ) != MARCSTATE_RX) { //error
    //MULTICC_PREFIX();
    DC('Z');
    DC('E');
    DC('R');
    DC('R');
    DC('3');
    DH2(cc1100_readReg( CC1100_MARCSTATE ));
    DH2(stat1);
    DH2(i);
    DNL();
    rf_moritz_init();
  }

  if (!moritz_on) {
    set_txrestore();
  }
  lastSendingTicks = ticks;
}

void moritz_sendAck(uint8_t* enc) {
  uint8_t ackPacket[12];
  ackPacket[0] = 11; /* len*/
  ackPacket[1] = enc[1]; /* msgcnt */
  ackPacket[2] = 0; /* flag */
  ackPacket[3] = 2; /* type = Ack */
  for (int i=0;i<3;++i) /* src = enc_dst*/
    ackPacket[4+i] = enc[7+i];
  for (int i=0;i<3;++i) /* dst = enc_src */
    ackPacket[7+i] = enc[4+i];
  ackPacket[10] = 0; /* groupid */
  ackPacket[11] = 0; /* payload */

  my_delay_ms(20); /* by experiments */

  moritz_sendraw(ackPacket, 0);

  //Inform FHEM that we send an autoack
  //MULTICC_PREFIX();
#if 0
  DC('Z');
  for (uint8_t i=0; i < ackPacket[0]+1; i++)
    DH2( ackPacket[i] );
  if (TX_REPORT & REP_RSSI)
    DH2( 0 ); //fake some rssi
  DNL();
#endif

}

void moritz_func(char *in) {
  if (in[1] == 'r') {                // Reception on
    rf_moritz_init();
  } else if (in[1] == 's' || in[1] == 'f' ) {         // Send/Send fast
    uint8_t dec[MAX_MORITZ_MSG];
    uint8_t hblen = fromhex(in+2, dec, MAX_MORITZ_MSG-1);
    if ((hblen-1) != dec[0]) {
      //MULTICC_PREFIX();
      DS_P(PSTR("LENERR\r\n"));
      return;
    }
    moritz_sendraw(dec, in[1] == 's');
  } else if (in[1] == 'a') {         // Auto-Ack
    fromhex(in+2, autoAckAddr, 3);
  } else if (in[1] == 'w') {         // Fake Wall-Thermostat
    fromhex(in+2, fakeWallThermostatAddr, 3);
  } else {                          // Off
    moritz_on = 0;
  }
}



/********************************************************************************************/
uint32_t last_moritz_task;

typedef union {
  uint8_t data;
  struct {
      uint8_t type : 3;
      uint8_t nu1 : 1;
      uint8_t nu2 : 1;
      uint8_t is_open : 1;
      uint8_t rf_error : 1;
      uint8_t battery_low : 1;
  };
} MORITZ_TYPE;

// 38 bytes per entry
// 2kb max
#define MMLSIZ 28
struct MORITZ {
  uint8_t id[3];
  MORITZ_TYPE mdata;
  uint8_t dtemperature;
  uint8_t valve;
  uint16_t mtemperature;
  uint8_t tmode;
  int8_t rssi;
#ifdef USE_24C256
  char label[MMLSIZ];
#endif
};

#define MORITZ_MAX_DEVICES 50

#ifndef USE_24C256
struct MORITZ *moritz_devices[MORITZ_MAX_DEVICES];
#endif

void moritz_mqtt(const char *hid,const char *type, char *payload) {

#ifdef USE_24C256
  struct MORITZ ml;
  fromhex(hid,ml.id,3);
  if (find_MLabel(&ml) && ml.label[0]) {
    ResponseTime_P(PSTR(",\"MO_%s_%s\":{%s}}"),ml.label,type,payload);
  } else {
    ResponseTime_P(PSTR(",\"MORITZ_%s_%s\":{%s}}"),hid,type,payload);
  }
#else
  ResponseTime_P(PSTR(",\"MORITZ_%s_%s\":{%s}}"),hid,type,payload);
#endif

  MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings.flag.mqtt_sensor_retain);
}

void new_moritz(uint8_t *hid,uint8_t type,uint8_t bs,uint8_t rfe,uint8_t bl,uint8_t rssi,uint8_t dtmp,uint16_t mtmp, uint8_t valve, uint8_t tmode) {
#ifdef USE_24C256
struct MORITZ mo;
#endif
struct MORITZ *mp;

  for (uint8_t cnt=0;cnt<MORITZ_MAX_DEVICES;cnt++) {
#ifdef USE_24C256
    get_MLabel(cnt,&mo);
    mp=&mo;
    if (mo.id[0] || mo.id[1] || mo.id[2]) {
#else
    if (moritz_devices[cnt]) {
      mp=moritz_devices[cnt];
#endif
      if (mp->id[0]==hid[0] && mp->id[1]==hid[1] && mp->id[2]==hid[2]) {
        mp->mdata.type=type;
        mp->mdata.is_open=bs;
        mp->mdata.rf_error=rfe;
        mp->mdata.battery_low=bl;
        mp->rssi=rssi;
        mp->mtemperature=dtmp;
        mp->dtemperature=mtmp;
        mp->valve=valve;
        mp->tmode=tmode;
#ifdef USE_24C256
        put_MLabel(cnt,mp);
#endif
        return;
      }
    }
  }

  for (uint8_t cnt=0;cnt<MORITZ_MAX_DEVICES;cnt++) {
#ifdef USE_24C256
    get_MLabel(cnt,&mo);
    mp=&mo;
    if (!mo.id[0] && !mo.id[1] && !mo.id[2]) {
#else
    if (!moritz_devices[cnt]) {
      mp=(struct MORITZ*)calloc(sizeof(struct MORITZ),1);
      if (!mp) return;
      moritz_devices[cnt]=mp;
#endif
      mp->mdata.type=type;
      mp->mdata.is_open=bs;
      mp->mdata.rf_error=rfe;
      mp->mdata.battery_low=bl;
      mp->rssi=rssi;
      mp->mtemperature=dtmp;
      mp->dtemperature=mtmp;
      mp->valve=valve;
      mp->tmode=tmode;
      mp->id[0]=hid[0];
      mp->id[1]=hid[1];
      mp->id[2]=hid[2];
#ifdef USE_24C256
      put_MLabel(cnt,mp);
#endif
      break;
    }
  }
}


#ifdef USE_WEBSERVER
const char HTTP_MORITZ_PBUT[] PROGMEM =
"{s}<button type='button' disabled  style ='background-color: #c0c0c0'>"
"MORITZ PB-%s :rfe=%d:bl=%d:rssi=%d : bs=%d</button>{m}" "{e}";
const char HTTP_MORITZ_WCONT[] PROGMEM =
"{s}<button type='button' disabled  style ='background-color: #808080'>"
"MORITZ WC-%s :rfe=%d:bl=%d:rssi=%d : cs=%d:</button>{m}" "{e}";
const char HTTP_MORITZ_THERM[] PROGMEM =
"{s}<button name='d%d' type='button' class='button'>"
"MORITZ TH-%s :rfe=%d:bl=%d:rssi=%d : %s : dtmp=%s C : mtmp=%s C</button>{m}" "{e}";

/*
        0 => auto weekly  AW
        1 => manual       MN
        2 => vacation     VA
        3 => boost        BO
        4 => manual eco   ME
        5 => manual comfort MC
        6 => manual window MW
        */
const char *mmodes="AWMNVABOMEMCMW";

void moritz_show(void) {
char ts1[8],ts2[8];
char mod[3];
char ma[9];
char *lbl=ma;
struct MORITZ mo;
struct MORITZ *mp;

  for (uint16_t cnt=0;cnt<MORITZ_MAX_DEVICES;cnt++) {
#ifdef USE_24C256
    get_MLabel(cnt,&mo);
    mp=&mo;
    if (mo.id[0] || mo.id[1] || mo.id[2]) {
      if (mo.label[0]) {
        lbl=mo.label;
      } else {
        lbl=ma;
        sprintf(lbl,"%02x%02x%02x",mp->id[0],mp->id[1],mp->id[2]);
      }
#else
    if (moritz_devices[cnt]) {
      mp=moritz_devices[cnt];
      lbl=ma;
      sprintf(lbl,"%02x%02x%02x",mp->id[0],mp->id[1],mp->id[2]);
#endif


      switch (mp->mdata.type) {
        case 0:
          WSContentSend_PD(HTTP_MORITZ_PBUT,lbl,mp->mdata.rf_error,mp->mdata.battery_low,mp->rssi,mp->mdata.is_open);
          break;
        case 1:
          WSContentSend_PD(HTTP_MORITZ_WCONT,lbl,mp->mdata.rf_error,mp->mdata.battery_low,mp->rssi,mp->mdata.is_open);
          break;
        case 2:
          memmove(mod,&mmodes[mp->tmode*2],2);
          mod[2]=0;
          float tmp=(float)mp->dtemperature/2.0;
          dtostrfd(tmp,1,ts1);
          tmp=(float)mp->mtemperature/10.0;
          dtostrfd(tmp,1,ts2);

          WSContentSend_PD(HTTP_MORITZ_THERM,cnt+1,lbl,mp->mdata.rf_error,mp->mdata.battery_low,mp->rssi,mod,ts1,ts2);
          break;
      }
    }
  }
}
#endif

#ifdef USE_24C256
  // i2c eeprom
#include <Eeprom24C128_256.h>
#define EEPROM_ADDRESS 0x50
#define EEPROM_START_OFFSET 16384
static Eeprom24C128_256 m_eeprom(EEPROM_ADDRESS);
// eeprom.writeBytes(address, length, buffer);
#define M_EEP_WRITE(A,B,C) m_eeprom.writeBytes(A,B,(uint8_t*)C);
// eeprom.readBytes(address, length, buffer);
#define M_EEP_READ(A,B,C) m_eeprom.readBytes(A,B,(uint8_t*)C);
#endif


void CC1101_Detect() {
uint8_t spi_set=0;

  if ((pin[GPIO_SSPI_MOSI]==13) && (pin[GPIO_SSPI_MISO]==12) && (pin[GPIO_SSPI_SCLK]==14)) {
    spi_set=1;
  } else {
    if ((pin[GPIO_SPI_MOSI]==13) && (pin[GPIO_SPI_MISO]==12) && (pin[GPIO_SPI_CLK]==14)) {
      spi_set=1;
    } else {
      spi_set=0;
    }
  }
  if (!spi_set) return;

  if (pin[GPIO_CC1101_CS]<99) {
    moritz_cs=pin[GPIO_CC1101_CS];
  } else return;

  pinMode(moritz_cs, OUTPUT);
  digitalWrite(moritz_cs,1);

  pinMode(moritz_cs, OUTPUT);
  digitalWrite(moritz_cs,1);
  SPI.begin();
  moritz_spiSettings = SPISettings(5000000, MSBFIRST, SPI_MODE3);
  rf_moritz_init();
  moritz_ready=1;
  moritz_flags=MORITZ_SHOW_ALL|MORITZ_PAIR_ENABLE;
  moritz_addr[0]=MORITZ_BASE_ADDRESS>>16;
  moritz_addr[1]=(MORITZ_BASE_ADDRESS>>8)&0xff;
  moritz_addr[2]=MORITZ_BASE_ADDRESS&0xff;
  last_moritz_task=millis();

#ifdef USE_24C256
  if (i2c_flg) {
    if (I2cSetDevice(EEPROM_ADDRESS)) {
      // eeprom is present
      moritz_flags|=MORITZ_EEPROM_FOUND;
      I2cSetActiveFound(EEPROM_ADDRESS, "24C256");
    }
  }
#endif
}


#ifdef USE_24C256

void get_MLabel(uint8_t index, struct MORITZ *ml) {
  if (moritz_flags&MORITZ_EEPROM_FOUND) {
    M_EEP_READ(EEPROM_START_OFFSET+(index*sizeof(MORITZ)),sizeof(MORITZ),(char*)ml);
    //sprintf(log_data,"r:%d:%02x%02x%02x %s",index,ml->id[0],ml->id[1],ml->id[2],ml->label);
    //AddLog(LOG_LEVEL_INFO);
  } else {
    ml->id[0]=0;
    ml->id[1]=0;
    ml->id[2]=0;
  }
}

void put_MLabel(uint8_t index, struct MORITZ *ml) {
  if (moritz_flags&MORITZ_EEPROM_FOUND) {
    M_EEP_WRITE(EEPROM_START_OFFSET+(index*sizeof(MORITZ)),sizeof(MORITZ),(char*)ml);
    //sprintf(log_data,"w:%d:%02x%02x%02x %s",index,ml->id[0],ml->id[1],ml->id[2],ml->label);
    //AddLog(LOG_LEVEL_INFO);
  }
}

void set_MLabel(struct MORITZ *ml) {
  struct MORITZ mlr;

  for (uint8_t cnt=0;cnt<MORITZ_MAX_DEVICES;cnt++) {
    get_MLabel(cnt,&mlr);
    if (mlr.id[0]==0 && mlr.id[1]==0 && mlr.id[2]==0 && mlr.label[0]==0) break;
    if (ml->id[0]==mlr.id[0] && ml->id[1]==mlr.id[1] && ml->id[2]==mlr.id[2]) {
      strcpy(mlr.label,ml->label);
      put_MLabel(cnt,&mlr);
      return;
    }
  }
}

bool find_MLabel(struct MORITZ *ml) {
  struct MORITZ mlr;
  for (uint8_t cnt=0;cnt<MORITZ_MAX_DEVICES;cnt++) {
    get_MLabel(cnt,&mlr);
    if (ml->id[0]==mlr.id[0] && ml->id[1]==mlr.id[1] && ml->id[2]==mlr.id[2]) {
      memcpy(ml->label,mlr.label,MMLSIZ);
      return true;
    }
    if (mlr.id[0]==0 && mlr.id[1]==0 && mlr.id[2]==0 && mlr.label[0]==0) {
      break;
    }
  }
  return false;
}

void Moritz_Sort_List(void) {
  struct MORITZ ml,ml1;
  uint8_t flag=0;
  while (1) {
    flag=0;
    for (uint8_t cnt=0;cnt<MORITZ_MAX_DEVICES-1;cnt++) {
      get_MLabel(cnt,&ml);
      get_MLabel(cnt+1,&ml1);
      if (ml1.id[0]==0 && ml1.id[1]==0 && ml1.id[2]==0) break;

      if (ml1.mdata.type<ml.mdata.type) {
        put_MLabel(cnt+1,&ml);
        put_MLabel(cnt,&ml1);
        flag=1;
      }
    }
    if (!flag) break;
  }

  for (uint8_t cnt=0;cnt<MORITZ_MAX_DEVICES;cnt++) {
    get_MLabel(cnt,&ml);
    if (ml.id[0] || ml.id[1] || ml.id[2]) {
      char *tp;
      switch (ml.mdata.type) {
        case 0:
          tp="PB";
          break;
        case 1:
          tp="WC";
          break;
        case 2:
          tp="TH";
          break;
      }
      sprintf(log_data,"%d-%s-%02x%02x%02x - %s",cnt+1,tp,ml.id[0],ml.id[1],ml.id[2],ml.label);
      AddLog(LOG_LEVEL_INFO);
    }
  }
}
#endif


// 8 ms ticks
void CC1101_loop(void) {
  if (moritz_ready) {
    if (millis()-last_moritz_task>8) {
      rf_moritz_task();
      last_moritz_task=millis();
      if (credit_10ms < MAX_CREDIT) // 10ms/1s == 1% -> allowed talk-time without CD
        credit_10ms += 1;
      ticks++;
    }

  }
}
/* cmds:  (currently NOT permanently stored)
s 0..1 => show all devices, show only paired devices (default=1, show all)
p 0..1 => allow auto pairing (default 1)
a hexid => set hex HHHHHH id of base (default is 567890)
t hexid mode temp => set temperature and mode of Thermostat with hexid
/* mode
0 => auto weekly
1 => manual
2 => vacation
3 => boost
4 => manual eco
5 => manual comfort
6 => manual window
*/
bool XSNS_97_cmd(void) {
  bool serviced = true;
  const char S_JSON_MORITZ[] = "{\"MORITZ\":{\"%s\":%d}}";
  const char S_JSON_MORITZ1[] = "{\"MORITZ\":{\"%s\":\"%s\"}}";
  if (XdrvMailbox.data_len > 0) {
    char *cp=XdrvMailbox.data;
    if (*cp=='s') {
      cp++;
      if (*cp)  {
        if (*cp&1) {
          moritz_flags|=MORITZ_SHOW_ALL;
        } else {
          moritz_flags&=(MORITZ_SHOW_ALL^0xff);
        }
      }
      Response_P(S_JSON_MORITZ,"sa",moritz_flags&MORITZ_SHOW_ALL);
      return true;
    } else if (*cp=='p') {
      cp++;
      if (*cp)  {
        if (*cp&1) {
          moritz_flags|=MORITZ_PAIR_ENABLE;
        } else {
          moritz_flags&=(MORITZ_PAIR_ENABLE^0xff);
        }
      }
      Response_P(S_JSON_MORITZ,"pm",moritz_flags&MORITZ_PAIR_ENABLE);
      return true;
    } else if (*cp=='a') {
      cp++;
      while (*cp==' ') cp++;
      uint32_t man=0;
      if (*cp) {
        man=strtol(cp,0,16);
        moritz_addr[0]=man>>16;
        moritz_addr[1]=man>>8;
        moritz_addr[2]=man;
      }
      char ma[10];
      sprintf(ma,"%02x%02x%02x",moritz_addr[0],moritz_addr[1],moritz_addr[2]);
      Response_P(S_JSON_MORITZ1,"ad",ma);
    } else if (*cp=='t') {
      // set temp   hexid mode tmp
      float tmp=0;
      uint8_t mode=0;
      uint32_t dst=0;
      cp++;
      while (*cp==' ') cp++;
      if (*cp) {
        dst=strtol(cp,&cp,16);
        while (*cp==' ') cp++;
        mode=strtol(cp,&cp,10);
        while (*cp==' ') cp++;
        tmp=CharToFloat(cp);
        SendModeTmp(dst,mode,tmp);
      }
      char ms[32];
      ms[0]=0;
      char tstr[8];
      dtostrfd(tmp,1, tstr);
      sprintf(ms,"%02x%02x%02x %02d %s",dst>>16,(dst>>8)&0xff,dst&0xff,mode,tstr);
      Response_P(S_JSON_MORITZ1,"send",ms);
    }
#ifdef USE_24C256
    else if (*cp=='l') {
      struct MORITZ ml;
      uint8_t index=0;
      uint32_t man=0;
      char ms[32];
      cp++;
      if (*cp=='c') {
        memset(&ml,0,sizeof(struct MORITZ));
        for (uint8_t cnt=0;cnt<MORITZ_MAX_DEVICES;cnt++) {
          put_MLabel(cnt,&ml);
        }
        Response_P(S_JSON_MORITZ1,"label","clr");
        return serviced;
      }
      if (*cp=='l') {
        Moritz_Sort_List();
        Response_P(S_JSON_MORITZ1,"label","list all");
        return serviced;
      }

      while (*cp==' ') cp++;

      if (*cp) {
        man=strtol(cp,&cp,16);
        ml.id[0]=man>>16;
        ml.id[1]=man>>8;
        ml.id[2]=man;
        while (*cp==' ') cp++;
        strlcpy(ml.label,cp,MMLSIZ);
        set_MLabel(&ml);
        sprintf(ms,"%02x%02x%02x %s",ml.id[0],ml.id[1],ml.id[2],ml.label);
      }
      Response_P(S_JSON_MORITZ1,"label",ms);
    }
#endif
  }
  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns97(byte function) {
  bool result = false;

  switch (function) {
      case FUNC_MODULE_INIT:
        moritz_cs=CC100_CS;
        break;
      case FUNC_INIT:
        CC1101_Detect();
        break;
      case FUNC_LOOP:
        CC1101_loop();
        break;
      case FUNC_JSON_APPEND:
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        moritz_show();
        break;
#endif  // USE_WEBSERVER
      case FUNC_COMMAND_SENSOR:
        if (XSNS_97 == XdrvMailbox.index) {
          result = XSNS_97_cmd();
        }
        break;
  }
  return result;
}

#endif  // USE_MORITZ
#endif  // USE_SPI
