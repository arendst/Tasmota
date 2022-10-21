#include <SPI.h>

/**
 * Copyright (c) 2011 panStamp <contact@panstamp.com>
 * 
 * This file is part of the panStamp project.
 * 
 * panStamp  is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * any later version.
 * 
 * panStamp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with panStamp; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 
 * USA
 * 
 * Author: Daniel Berenguer
 * Creation date: 03/03/2011
 */

#ifndef _CC1101_H
#define _CC1101_H

//#include "simplespi.h"
extern "C" {
#include <stdint.h>
#include "ccpacket.h"
}

/**
 * Carrier frequencies
 */
enum CFREQ
{
  CFREQ_868 = 0,
  CFREQ_915,
  CFREQ_433,
  CFREQ_918,
  CFREQ_LAST
};

/**
 * RF STATES
 */
enum RFSTATE
{
  RFSTATE_IDLE = 0,
  RFSTATE_RX,
  RFSTATE_TX
};


/**
 * Frequency channels
 */
#define NUMBER_OF_FCHANNELS      10

/**
 * Type of transfers
 */
#define WRITE_BURST              0x40
#define READ_SINGLE              0x80
#define READ_BURST               0xC0

/**
 * Type of register
 */
#define CC1101_CONFIG_REGISTER   READ_SINGLE
#define CC1101_STATUS_REGISTER   READ_BURST

/**
 * PATABLE & FIFO's
 */
#define CC1101_PATABLE           0x3E        // PATABLE address
#define CC1101_TXFIFO            0x3F        // TX FIFO address
#define CC1101_RXFIFO            0x3F        // RX FIFO address

/**
 * Command strobes
 */
#define CC1101_SRES              0x30        // Reset CC1101 chip
#define CC1101_SFSTXON           0x31        // Enable and calibrate frequency synthesizer (if MCSM0.FS_AUTOCAL=1). If in RX (with CCA):
                                             // Go to a wait state where only the synthesizer is running (for quick RX / TX turnaround).
#define CC1101_SXOFF             0x32        // Turn off crystal oscillator
#define CC1101_SCAL              0x33        // Calibrate frequency synthesizer and turn it off. SCAL can be strobed from IDLE mode without
                                             // setting manual calibration mode (MCSM0.FS_AUTOCAL=0)
#define CC1101_SRX               0x34        // Enable RX. Perform calibration first if coming from IDLE and MCSM0.FS_AUTOCAL=1
#define CC1101_STX               0x35        // In IDLE state: Enable TX. Perform calibration first if MCSM0.FS_AUTOCAL=1.
                                             // If in RX state and CCA is enabled: Only go to TX if channel is clear
#define CC1101_SIDLE             0x36        // Exit RX / TX, turn off frequency synthesizer and exit Wake-On-Radio mode if applicable
#define CC1101_SWOR              0x38        // Start automatic RX polling sequence (Wake-on-Radio) as described in Section 19.5 if
                                             // WORCTRL.RC_PD=0
#define CC1101_SPWD              0x39        // Enter power down mode when CSn goes high
#define CC1101_SFRX              0x3A        // Flush the RX FIFO buffer. Only issue SFRX in IDLE or RXFIFO_OVERFLOW states
#define CC1101_SFTX              0x3B        // Flush the TX FIFO buffer. Only issue SFTX in IDLE or TXFIFO_UNDERFLOW states
#define CC1101_SWORRST           0x3C        // Reset real time clock to Event1 value
#define CC1101_SNOP              0x3D        // No operation. May be used to get access to the chip status byte

/**
 * CC1101 configuration registers
 */
#define CC1101_IOCFG2            0x00        // GDO2 Output Pin Configuration
#define CC1101_IOCFG1            0x01        // GDO1 Output Pin Configuration
#define CC1101_IOCFG0            0x02        // GDO0 Output Pin Configuration
#define CC1101_FIFOTHR           0x03        // RX FIFO and TX FIFO Thresholds
#define CC1101_SYNC1             0x04        // Sync Word, High Byte
#define CC1101_SYNC0             0x05        // Sync Word, Low Byte
#define CC1101_PKTLEN            0x06        // Packet Length
#define CC1101_PKTCTRL1          0x07        // Packet Automation Control
#define CC1101_PKTCTRL0          0x08        // Packet Automation Control
#define CC1101_ADDR              0x09        // Device Address
#define CC1101_CHANNR            0x0A        // Channel Number
#define CC1101_FSCTRL1           0x0B        // Frequency Synthesizer Control
#define CC1101_FSCTRL0           0x0C        // Frequency Synthesizer Control
#define CC1101_FREQ2             0x0D        // Frequency Control Word, High Byte
#define CC1101_FREQ1             0x0E        // Frequency Control Word, Middle Byte
#define CC1101_FREQ0             0x0F        // Frequency Control Word, Low Byte
#define CC1101_MDMCFG4           0x10        // Modem Configuration
#define CC1101_MDMCFG3           0x11        // Modem Configuration
#define CC1101_MDMCFG2           0x12        // Modem Configuration
#define CC1101_MDMCFG1           0x13        // Modem Configuration
#define CC1101_MDMCFG0           0x14        // Modem Configuration
#define CC1101_DEVIATN           0x15        // Modem Deviation Setting
#define CC1101_MCSM2             0x16        // Main Radio Control State Machine Configuration
#define CC1101_MCSM1             0x17        // Main Radio Control State Machine Configuration
#define CC1101_MCSM0             0x18        // Main Radio Control State Machine Configuration
#define CC1101_FOCCFG            0x19        // Frequency Offset Compensation Configuration
#define CC1101_BSCFG             0x1A        // Bit Synchronization Configuration
#define CC1101_AGCCTRL2          0x1B        // AGC Control
#define CC1101_AGCCTRL1          0x1C        // AGC Control
#define CC1101_AGCCTRL0          0x1D        // AGC Control
#define CC1101_WOREVT1           0x1E        // High Byte Event0 Timeout
#define CC1101_WOREVT0           0x1F        // Low Byte Event0 Timeout
#define CC1101_WORCTRL           0x20        // Wake On Radio Control
#define CC1101_FREND1            0x21        // Front End RX Configuration
#define CC1101_FREND0            0x22        // Front End TX Configuration
#define CC1101_FSCAL3            0x23        // Frequency Synthesizer Calibration
#define CC1101_FSCAL2            0x24        // Frequency Synthesizer Calibration
#define CC1101_FSCAL1            0x25        // Frequency Synthesizer Calibration
#define CC1101_FSCAL0            0x26        // Frequency Synthesizer Calibration
#define CC1101_RCCTRL1           0x27        // RC Oscillator Configuration
#define CC1101_RCCTRL0           0x28        // RC Oscillator Configuration
#define CC1101_FSTEST            0x29        // Frequency Synthesizer Calibration Control
#define CC1101_PTEST             0x2A        // Production Test
#define CC1101_AGCTEST           0x2B        // AGC Test
#define CC1101_TEST2             0x2C        // Various Test Settings
#define CC1101_TEST1             0x2D        // Various Test Settings
#define CC1101_TEST0             0x2E        // Various Test Settings

/**
 * Status registers
 */
#define CC1101_PARTNUM           0x30        // Chip ID
#define CC1101_VERSION           0x31        // Chip ID
#define CC1101_FREQEST           0x32        // Frequency Offset Estimate from Demodulator
#define CC1101_LQI               0x33        // Demodulator Estimate for Link Quality
#define CC1101_RSSI              0x34        // Received Signal Strength Indication
#define CC1101_MARCSTATE         0x35        // Main Radio Control State Machine State
#define CC1101_WORTIME1          0x36        // High Byte of WOR Time
#define CC1101_WORTIME0          0x37        // Low Byte of WOR Time
#define CC1101_PKTSTATUS         0x38        // Current GDOx Status and Packet Status
#define CC1101_VCO_VC_DAC        0x39        // Current Setting from PLL Calibration Module
#define CC1101_TXBYTES           0x3A        // Underflow and Number of Bytes
#define CC1101_RXBYTES           0x3B        // Overflow and Number of Bytes
#define CC1101_RCCTRL1_STATUS    0x3C        // Last RC Oscillator Calibration Result
#define CC1101_RCCTRL0_STATUS    0x3D        // Last RC Oscillator Calibration Result 

/**
 * CC1101 configuration registers - Default values extracted from SmartRF Studio
 *
 * Configuration:
 *
 * Deviation = 20.629883 
 * Base frequency = 867.999939 
 * Carrier frequency = 867.999939 
 * Channel number = 0 
 * Carrier frequency = 867.999939 
 * Modulated = true 
 * Modulation format = GFSK 
 * Manchester enable = false
 * Data whitening = off
 * Sync word qualifier mode = 30/32 sync word bits detected 
 * Preamble count = 4 
 * Channel spacing = 199.951172 
 * Carrier frequency = 867.999939 
 * Data rate = 38.3835 Kbps
 * RX filter BW = 101.562500 
 * Data format = Normal mode 
 * Length config = Variable packet length mode. Packet length configured by the first byte after sync word 
 * CRC enable = true 
 * Packet length = 255 
 * Device address = 1 
 * Address config = Enable address check
 * Append status = Append two status bytes to the payload of the packet. The status bytes contain RSSI and
 * LQI values, as well as CRC OK
 * CRC autoflush = false 
 * PA ramping = false 
 * TX power = 12
 * GDO0 mode = Asserts when sync word has been sent / received, and de-asserts at the end of the packet.
 * In RX, the pin will also de-assert when a packet is discarded due to address or maximum length filtering
 * or when the radio enters RXFIFO_OVERFLOW state. In TX the pin will de-assert if the TX FIFO underflows
 * Settings optimized for low current consumption
 */

#define CC1101_DEFVAL_IOCFG2     0x0D        // GDO2 Output Pin Configuration
#define CC1101_DEFVAL_IOCFG1     0x2E        // GDO1 Output Pin Configuration
#define CC1101_DEFVAL_IOCFG0     0x2D        // GDO0 Output Pin Configuration
#define CC1101_DEFVAL_FIFOTHR    0x07        // RX FIFO and TX FIFO Thresholds
#define CC1101_DEFVAL_SYNC1      0xD3        // Synchronization word, high byte
#define CC1101_DEFVAL_SYNC0      0x91        // Synchronization word, low byte
#define CC1101_DEFVAL_PKTLEN     0x3D        // Packet Length
#define CC1101_DEFVAL_PKTCTRL1   0x04        // Packet Automation Control
#define CC1101_DEFVAL_PKTCTRL0   0x32        // Packet Automation Control
#define CC1101_DEFVAL_ADDR       0xFF        // Device Address
#define CC1101_DEFVAL_CHANNR     0x00        // Channel Number
#define CC1101_DEFVAL_FSCTRL1    0x06        // Frequency Synthesizer Control
#define CC1101_DEFVAL_FSCTRL0    0x00        // Frequency Synthesizer Control
// Carrier frequency = 868 MHz
#define CC1101_DEFVAL_FREQ2_868  0x21        // Frequency Control Word, High Byte
#define CC1101_DEFVAL_FREQ1_868  0x62        // Frequency Control Word, Middle Byte
#define CC1101_DEFVAL_FREQ0_868  0x76        // Frequency Control Word, Low Byte
// Carrier frequency = 902 MHz
#define CC1101_DEFVAL_FREQ2_915  0x22        // Frequency Control Word, High Byte
#define CC1101_DEFVAL_FREQ1_915  0xB1        // Frequency Control Word, Middle Byte
#define CC1101_DEFVAL_FREQ0_915  0x3B        // Frequency Control Word, Low Byte
// Carrier frequency = 918 MHz
#define CC1101_DEFVAL_FREQ2_918  0x23        // Frequency Control Word, High Byte
#define CC1101_DEFVAL_FREQ1_918  0x4E        // Frequency Control Word, Middle Byte
#define CC1101_DEFVAL_FREQ0_918  0xC4        // Frequency Control Word, Low Byte

// Carrier frequency = 433 MHz
#define CC1101_DEFVAL_FREQ2_433  0x10        // Frequency Control Word, High Byte
#define CC1101_DEFVAL_FREQ1_433  0xB0        // Frequency Control Word, Middle Byte
#define CC1101_DEFVAL_FREQ0_433  0xBD       // Frequency Control Word, Low Byte

#define CC1101_DEFVAL_MDMCFG4    0x56     // Modem Configuration
#define CC1101_DEFVAL_MDMCFG3    0x83      // Modem Configuration
#define CC1101_DEFVAL_MDMCFG2    0x30        // Modem Configuration
#define CC1101_DEFVAL_MDMCFG1    0x22        // Modem Configuration
#define CC1101_DEFVAL_MDMCFG0    0xF8        // Modem Configuration
#define CC1101_DEFVAL_DEVIATN    0x00      // Modem Deviation Setting
#define CC1101_DEFVAL_MCSM2      0x07        // Main Radio Control State Machine Configuration
#define CC1101_DEFVAL_MCSM1      0x00        // Main Radio Control State Machine Configuration
#define CC1101_DEFVAL_MCSM0      0x18        // Main Radio Control State Machine Configuration
#define CC1101_DEFVAL_FOCCFG     0x14        // Frequency Offset Compensation Configuration
#define CC1101_DEFVAL_BSCFG      0x6C        // Bit Synchronization Configuration
#define CC1101_DEFVAL_AGCCTRL2   0x07        // AGC Control
#define CC1101_DEFVAL_AGCCTRL1   0x00       // AGC Control
#define CC1101_DEFVAL_AGCCTRL0   0x91        // AGC Control
#define CC1101_DEFVAL_WOREVT1    0x87        // High Byte Event0 Timeout
#define CC1101_DEFVAL_WOREVT0    0x6B        // Low Byte Event0 Timeout
#define CC1101_DEFVAL_WORCTRL    0xF8        // Wake On Radio Control
#define CC1101_DEFVAL_FREND1     0x56        // Front End RX Configuration
#define CC1101_DEFVAL_FREND0     0x11        // Front End TX Configuration
#define CC1101_DEFVAL_FSCAL3     0xE9        // Frequency Synthesizer Calibration
#define CC1101_DEFVAL_FSCAL2     0x2A        // Frequency Synthesizer Calibration
#define CC1101_DEFVAL_FSCAL1     0x00        // Frequency Synthesizer Calibration
#define CC1101_DEFVAL_FSCAL0     0x1F        // Frequency Synthesizer Calibration
#define CC1101_DEFVAL_RCCTRL1    0x41        // RC Oscillator Configuration
#define CC1101_DEFVAL_RCCTRL0    0x00        // RC Oscillator Configuration
#define CC1101_DEFVAL_FSTEST     0x59        // Frequency Synthesizer Calibration Control
#define CC1101_DEFVAL_PTEST      0x7F        // Production Test
#define CC1101_DEFVAL_AGCTEST    0x3F        // AGC Test
#define CC1101_DEFVAL_TEST2      0x88        // Various Test Settings
#define CC1101_DEFVAL_TEST1      0x31        // Various Test Settings
#define CC1101_DEFVAL_TEST0      0x09        // Various Test Settings

/**
 * Alias for some default values
 */
#define CCDEF_CHANNR  CC1101_DEFVAL_CHANNR
#define CCDEF_SYNC0  CC1101_DEFVAL_SYNC0
#define CCDEF_SYNC1  CC1101_DEFVAL_SYNC1
#define CCDEF_ADDR  CC1101_DEFVAL_ADDR

/**
 * Macros
 */
// Read CC1101 Config register
#define readConfigReg(regAddr)    readReg(regAddr, CC1101_CONFIG_REGISTER)
// Read CC1101 Status register
#define readStatusReg(regAddr)    readReg(regAddr, CC1101_STATUS_REGISTER)
// Enter Rx state
//#define setRxState()              cmdStrobe(CC1101_SRX)
// Enter Tx state
//#define setTxState()              cmdStrobe(CC1101_STX)
// Enter IDLE state
#define setIdleState()            cmdStrobe(CC1101_SIDLE)
// Flush Rx FIFO
#define flushRxFifo()             cmdStrobe(CC1101_SFRX)
// Flush Tx FIFO
#define flushTxFifo()             cmdStrobe(CC1101_SFTX)
// Disable address check
#define disableAddressCheck()     writeReg(CC1101_PKTCTRL1, 0x04)
// Enable address check
#define enableAddressCheck()      writeReg(CC1101_PKTCTRL1, 0x06)
// Disable CCA
#define disableCCA()              writeReg(CC1101_MCSM1, 0)
// Enable CCA
#define enableCCA()               writeReg(CC1101_MCSM1, CC1101_DEFVAL_MCSM1)
// Set PATABLE single byte
#define setTxPowerAmp(setting)    paTableByte = setting
// PATABLE values
#define PA_LowPower               0x60
#define PA_LongDistance           0xC0

/**
 * Class: CC1101
 * 
 * Description:
 * CC1101 interface
 */
class CC1101
{
  private:
    /**
     * Atmega's SPI interface
     */
    SPIClass spi;

    /**
     * writeBurstReg
     * 
     * Write multiple registers into the CC1101 IC via SPI
     * 
     * 'regAddr'	Register address
     * 'buffer'	Data to be writen
     * 'len'	Data length
     */
    void writeBurstReg(uint8_t regAddr, uint8_t* buffer, uint8_t len);

    /**
     * readBurstReg
     * 
     * Read burst data from CC1101 via SPI
     * 
     * 'buffer'	Buffer where to copy the result to
     * 'regAddr'	Register address
     * 'len'	Data length
     */
    void readBurstReg(uint8_t * buffer, uint8_t regAddr, uint8_t len);

    /**
     * setRegsFromEeprom
     * 
     * Set registers from EEPROM
     */
    void setRegsFromEeprom(void);

  public:
    /*
     * RF state
     */
    uint8_t rfState;

    /**
     * Tx Power byte (single PATABLE config)
     */
    uint8_t paTableByte;

    /**
     * Carrier frequency
     */
    uint8_t carrierFreq;

    /**
     * Frequency channel
     */
    uint8_t channel;

    /**
     * Synchronization word
     */
    uint8_t syncWord[2];

    /**
     * Device address
     */
    uint8_t devAddress;

    /**
     * CC1101
     * 
     * Class constructor
     */
    CC1101(void);

    /**
     * cmdStrobe
     * 
     * Send command strobe to the CC1101 IC via SPI
     * 
     * 'cmd'	Command strobe
     */
    void cmdStrobe(uint8_t cmd);

    /**
     * wakeUp
     * 
     * Wake up CC1101 from Power Down state
     */
    void wakeUp(void);

    /**
     * readReg
     * 
     * Read CC1101 register via SPI
     * 
     * 'regAddr'	Register address
     * 'regType'	Type of register: CC1101_CONFIG_REGISTER or CC1101_STATUS_REGISTER
     * 
     * Return:
     * 	Data byte returned by the CC1101 IC
     */
    uint8_t readReg(uint8_t regAddr, uint8_t regType);

    /**
     * writeReg
     * 
     * Write single register into the CC1101 IC via SPI
     * 
     * 'regAddr'	Register address
     * 'value'	Value to be writen
     */
    void writeReg(uint8_t regAddr, uint8_t value);

    /**
     * setCCregs
     * 
     * Configure CC1101 registers
     */
    void setCCregs(void);

    /**
     * reset
     * 
     * Reset CC1101
     */
    void reset(void);
    
    /**
     * init
     * 
     * Initialize CC1101 radio
     *
     * @param freq Carrier frequency
     */
    void init(uint8_t freq=CFREQ_868);

    /**
     * setSyncWord
     * 
     * Set synchronization word
     * 
     * 'syncH'	Synchronization word - High byte
     * 'syncL'	Synchronization word - Low byte
     */
    void setSyncWord(uint8_t syncH, uint8_t syncL);

    /**
     * setSyncWord (overriding method)
     * 
     * Set synchronization word
     * 
     * 'syncH'	Synchronization word - pointer to 2-byte array
     */
    void setSyncWord(uint8_t *sync);

    /**
     * setDevAddress
     * 
     * Set device address
     * 
     * 'addr'	Device address
     */
    void setDevAddress(uint8_t addr);

    /**
     * setCarrierFreq
     * 
     * Set carrier frequency
     * 
     * 'freq'	New carrier frequency
     */
    void setCarrierFreq(uint8_t freq);
    
    /**
     * setChannel
     * 
     * Set frequency channel
     * 
     * 'chnl'	Frequency channel
     */
    void setChannel(uint8_t chnl);

    /**
     * setPowerDownState
     * 
     * Put CC1101 into power-down state
     */
    void setPowerDownState();
    
    /**
     * sendData
     * 
     * Send data packet via RF
     * 
     * 'packet'	Packet to be transmitted. First byte is the destination address
     *
     *  Return:
     *    True if the transmission succeeds
     *    False otherwise
     */
    bool sendData(CCPACKET packet);

    /**
     * receiveData
     * 
     * Read data packet from RX FIFO
     * 
     * Return:
     * 	Amount of bytes received
     */
    uint8_t receiveData(CCPACKET *packet);
    
    /**
     * setRxState
     * 
     * Enter Rx state
     */
    void setRxState(void);

    /**
     * setTxState
     * 
     * Enter Tx state
     */
    void setTxState(void);
};

#endif

