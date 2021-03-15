/**
 * MFRC522_I2C.h - Library to use ARDUINO RFID MODULE KIT 13.56 MHZ WITH TAGS I2C BY AROZCAN
 * MFRC522_I2C.h - Based on ARDUINO RFID MODULE KIT 13.56 MHZ WITH TAGS SPI Library BY COOQROBOT.
 * Based on code Dr.Leong   ( WWW.B2CQSHOP.COM )
 * Created by Miguel Balboa (circuitito.com), Jan, 2012.
 * Rewritten by Søren Thing Andersen (access.thing.dk), fall of 2013 (Translation to English, refactored, comments, anti collision, cascade levels.)
 * Extended by Tom Clement with functionality to write to sector 0 of UID changeable Mifare cards.
 * Extended by Ahmet Remzi Ozcan with I2C functionality.
 * Author: arozcan @ https://github.com/arozcan/MFRC522-I2C-Library
 * Released into the public domain.
 *
 * Please read this file for an overview and then MFRC522.cpp for comments on the specific functions.
 * Search for "mf-rc522" on ebay.com to purchase the MF-RC522 board.
 *
 * There are three hardware components involved:
 * 1) The micro controller: An Arduino
 * 2) The PCD (short for Proximity Coupling Device): NXP MFRC522 Contactless Reader IC
 * 3) The PICC (short for Proximity Integrated Circuit Card): A card or tag using the ISO 14443A interface, eg Mifare or NTAG203.
 *
 * The microcontroller and card reader uses I2C for communication.
 * The protocol is described in the MFRC522 datasheet: http://www.nxp.com/documents/data_sheet/MFRC522.pdf
 *
 * The card reader and the tags communicate using a 13.56MHz electromagnetic field.
 * The protocol is defined in ISO/IEC 14443-3 Identification cards -- Contactless integrated circuit cards -- Proximity cards -- Part 3: Initialization and anticollision".
 * A free version of the final draft can be found at http://wg8.de/wg8n1496_17n3613_Ballot_FCD14443-3.pdf
 * Details are found in chapter 6, Type A – Initialization and anticollision.
 *
 * If only the PICC UID is wanted, the above documents has all the needed information.
 * To read and write from MIFARE PICCs, the MIFARE protocol is used after the PICC has been selected.
 * The MIFARE Classic chips and protocol is described in the datasheets:
 *		1K:   http://www.nxp.com/documents/data_sheet/MF1S503x.pdf
 * 		4K:   http://www.nxp.com/documents/data_sheet/MF1S703x.pdf
 * 		Mini: http://www.idcardmarket.com/download/mifare_S20_datasheet.pdf
 * The MIFARE Ultralight chip and protocol is described in the datasheets:
 *		Ultralight:   http://www.nxp.com/documents/data_sheet/MF0ICU1.pdf
 * 		Ultralight C: http://www.nxp.com/documents/short_data_sheet/MF0ICU2_SDS.pdf
 *
 * MIFARE Classic 1K (MF1S503x):
 * 		Has 16 sectors * 4 blocks/sector * 16 bytes/block = 1024 bytes.
 * 		The blocks are numbered 0-63.
 * 		Block 3 in each sector is the Sector Trailer. See http://www.nxp.com/documents/data_sheet/MF1S503x.pdf sections 8.6 and 8.7:
 * 				Bytes 0-5:   Key A
 * 				Bytes 6-8:   Access Bits
 * 				Bytes 9:     User data
 * 				Bytes 10-15: Key B (or user data)
 * 		Block 0 is read-only manufacturer data.
 * 		To access a block, an authentication using a key from the block's sector must be performed first.
 * 		Example: To read from block 10, first authenticate using a key from sector 3 (blocks 8-11).
 * 		All keys are set to FFFFFFFFFFFFh at chip delivery.
 * 		Warning: Please read section 8.7 "Memory Access". It includes this text: if the PICC detects a format violation the whole sector is irreversibly blocked.
 *		To use a block in "value block" mode (for Increment/Decrement operations) you need to change the sector trailer. Use PICC_SetAccessBits() to calculate the bit patterns.
 * MIFARE Classic 4K (MF1S703x):
 * 		Has (32 sectors * 4 blocks/sector + 8 sectors * 16 blocks/sector) * 16 bytes/block = 4096 bytes.
 * 		The blocks are numbered 0-255.
 * 		The last block in each sector is the Sector Trailer like above.
 * MIFARE Classic Mini (MF1 IC S20):
 * 		Has 5 sectors * 4 blocks/sector * 16 bytes/block = 320 bytes.
 * 		The blocks are numbered 0-19.
 * 		The last block in each sector is the Sector Trailer like above.
 *
 * MIFARE Ultralight (MF0ICU1):
 * 		Has 16 pages of 4 bytes = 64 bytes.
 * 		Pages 0 + 1 is used for the 7-byte UID.
 * 		Page 2 contains the last check digit for the UID, one byte manufacturer internal data, and the lock bytes (see http://www.nxp.com/documents/data_sheet/MF0ICU1.pdf section 8.5.2)
 * 		Page 3 is OTP, One Time Programmable bits. Once set to 1 they cannot revert to 0.
 * 		Pages 4-15 are read/write unless blocked by the lock bytes in page 2.
 * MIFARE Ultralight C (MF0ICU2):
 * 		Has 48 pages of 4 bytes = 192 bytes.
 * 		Pages 0 + 1 is used for the 7-byte UID.
 * 		Page 2 contains the last check digit for the UID, one byte manufacturer internal data, and the lock bytes (see http://www.nxp.com/documents/data_sheet/MF0ICU1.pdf section 8.5.2)
 * 		Page 3 is OTP, One Time Programmable bits. Once set to 1 they cannot revert to 0.
 * 		Pages 4-39 are read/write unless blocked by the lock bytes in page 2.
 * 		Page 40 Lock bytes
 * 		Page 41 16 bit one way counter
 * 		Pages 42-43 Authentication configuration
 * 		Pages 44-47 Authentication key
 */
#ifndef MFRC522_h
#define MFRC522_h

#include <Arduino.h>
#include <Wire.h>

// Firmware data for self-test
// Reference values based on firmware version
// Hint: if needed, you can remove unused self-test data to save flash memory
//
// Version 0.0 (0x90)
// Philips Semiconductors; Preliminary Specification Revision 2.0 - 01 August 2005; 16.1 Sefttest
const byte MFRC522_firmware_referenceV0_0[] PROGMEM = {
	0x00, 0x87, 0x98, 0x0f, 0x49, 0xFF, 0x07, 0x19,
	0xBF, 0x22, 0x30, 0x49, 0x59, 0x63, 0xAD, 0xCA,
	0x7F, 0xE3, 0x4E, 0x03, 0x5C, 0x4E, 0x49, 0x50,
	0x47, 0x9A, 0x37, 0x61, 0xE7, 0xE2, 0xC6, 0x2E,
	0x75, 0x5A, 0xED, 0x04, 0x3D, 0x02, 0x4B, 0x78,
	0x32, 0xFF, 0x58, 0x3B, 0x7C, 0xE9, 0x00, 0x94,
	0xB4, 0x4A, 0x59, 0x5B, 0xFD, 0xC9, 0x29, 0xDF,
	0x35, 0x96, 0x98, 0x9E, 0x4F, 0x30, 0x32, 0x8D
};
// Version 1.0 (0x91)
// NXP Semiconductors; Rev. 3.8 - 17 September 2014; 16.1.1 Self test
const byte MFRC522_firmware_referenceV1_0[] PROGMEM = {
	0x00, 0xC6, 0x37, 0xD5, 0x32, 0xB7, 0x57, 0x5C,
	0xC2, 0xD8, 0x7C, 0x4D, 0xD9, 0x70, 0xC7, 0x73,
	0x10, 0xE6, 0xD2, 0xAA, 0x5E, 0xA1, 0x3E, 0x5A,
	0x14, 0xAF, 0x30, 0x61, 0xC9, 0x70, 0xDB, 0x2E,
	0x64, 0x22, 0x72, 0xB5, 0xBD, 0x65, 0xF4, 0xEC,
	0x22, 0xBC, 0xD3, 0x72, 0x35, 0xCD, 0xAA, 0x41,
	0x1F, 0xA7, 0xF3, 0x53, 0x14, 0xDE, 0x7E, 0x02,
	0xD9, 0x0F, 0xB5, 0x5E, 0x25, 0x1D, 0x29, 0x79
};
// Version 2.0 (0x92)
// NXP Semiconductors; Rev. 3.8 - 17 September 2014; 16.1.1 Self test
const byte MFRC522_firmware_referenceV2_0[] PROGMEM = {
	0x00, 0xEB, 0x66, 0xBA, 0x57, 0xBF, 0x23, 0x95,
	0xD0, 0xE3, 0x0D, 0x3D, 0x27, 0x89, 0x5C, 0xDE,
	0x9D, 0x3B, 0xA7, 0x00, 0x21, 0x5B, 0x89, 0x82,
	0x51, 0x3A, 0xEB, 0x02, 0x0C, 0xA5, 0x00, 0x49,
	0x7C, 0x84, 0x4D, 0xB3, 0xCC, 0xD2, 0x1B, 0x81,
	0x5D, 0x48, 0x76, 0xD5, 0x71, 0x61, 0x21, 0xA9,
	0x86, 0x96, 0x83, 0x38, 0xCF, 0x9D, 0x5B, 0x6D,
	0xDC, 0x15, 0xBA, 0x3E, 0x7D, 0x95, 0x3B, 0x2F
};
// Clone
// Fudan Semiconductor FM17522 (0x88)
const byte FM17522_firmware_reference[] PROGMEM = {
	0x00, 0xD6, 0x78, 0x8C, 0xE2, 0xAA, 0x0C, 0x18,
	0x2A, 0xB8, 0x7A, 0x7F, 0xD3, 0x6A, 0xCF, 0x0B,
	0xB1, 0x37, 0x63, 0x4B, 0x69, 0xAE, 0x91, 0xC7,
	0xC3, 0x97, 0xAE, 0x77, 0xF4, 0x37, 0xD7, 0x9B,
	0x7C, 0xF5, 0x3C, 0x11, 0x8F, 0x15, 0xC3, 0xD7,
	0xC1, 0x5B, 0x00, 0x2A, 0xD0, 0x75, 0xDE, 0x9E,
	0x51, 0x64, 0xAB, 0x3E, 0xE9, 0x15, 0xB5, 0xAB,
	0x56, 0x9A, 0x98, 0x82, 0x26, 0xEA, 0x2A, 0x62
};

class MFRC522 {
public:
	// MFRC522 registers. Described in chapter 9 of the datasheet.
	enum PCD_Register {
		// Page 0: Command and status
		//						  0x00			// reserved for future use
		CommandReg				= 0x01 ,	// starts and stops command execution
		ComIEnReg				= 0x02 ,	// enable and disable interrupt request control bits
		DivIEnReg				= 0x03 ,	// enable and disable interrupt request control bits
		ComIrqReg				= 0x04 ,	// interrupt request bits
		DivIrqReg				= 0x05 ,	// interrupt request bits
		ErrorReg				= 0x06 ,	// error bits showing the error status of the last command executed
		Status1Reg				= 0x07 ,	// communication status bits
		Status2Reg				= 0x08 ,	// receiver and transmitter status bits
		FIFODataReg				= 0x09 ,	// input and output of 64 byte FIFO buffer
		FIFOLevelReg			= 0x0A ,	// number of bytes stored in the FIFO buffer
		WaterLevelReg			= 0x0B ,	// level for FIFO underflow and overflow warning
		ControlReg				= 0x0C ,	// miscellaneous control registers
		BitFramingReg			= 0x0D ,	// adjustments for bit-oriented frames
		CollReg					= 0x0E ,	// bit position of the first bit-collision detected on the RF interface
		//						  0x0F			// reserved for future use

		// Page 1: Command
		// 						  0x10			// reserved for future use
		ModeReg					= 0x11 ,	// defines general modes for transmitting and receiving
		TxModeReg				= 0x12 ,	// defines transmission data rate and framing
		RxModeReg				= 0x13 ,	// defines reception data rate and framing
		TxControlReg			= 0x14 ,	// controls the logical behavior of the antenna driver pins TX1 and TX2
		TxASKReg				= 0x15 ,	// controls the setting of the transmission modulation
		TxSelReg				= 0x16 ,	// selects the internal sources for the antenna driver
		RxSelReg				= 0x17 ,	// selects internal receiver settings
		RxThresholdReg			= 0x18 ,	// selects thresholds for the bit decoder
		DemodReg				= 0x19 ,	// defines demodulator settings
		// 						  0x1A			// reserved for future use
		// 						  0x1B			// reserved for future use
		MfTxReg					= 0x1C ,	// controls some MIFARE communication transmit parameters
		MfRxReg					= 0x1D ,	// controls some MIFARE communication receive parameters
		// 						  0x1E			// reserved for future use
		SerialSpeedReg			= 0x1F ,	// selects the speed of the serial UART interface

		// Page 2: Configuration
		// 						  0x20			// reserved for future use
		CRCResultRegH			= 0x21 ,	// shows the MSB and LSB values of the CRC calculation
		CRCResultRegL			= 0x22 ,
		// 						  0x23			// reserved for future use
		ModWidthReg				= 0x24 ,	// controls the ModWidth setting?
		// 						  0x25			// reserved for future use
		RFCfgReg				= 0x26 ,	// configures the receiver gain
		GsNReg					= 0x27 ,	// selects the conductance of the antenna driver pins TX1 and TX2 for modulation
		CWGsPReg				= 0x28 ,	// defines the conductance of the p-driver output during periods of no modulation
		ModGsPReg				= 0x29 ,	// defines the conductance of the p-driver output during periods of modulation
		TModeReg				= 0x2A ,	// defines settings for the internal timer
		TPrescalerReg			= 0x2B ,	// the lower 8 bits of the TPrescaler value. The 4 high bits are in TModeReg.
		TReloadRegH				= 0x2C ,	// defines the 16-bit timer reload value
		TReloadRegL				= 0x2D ,
		TCounterValueRegH		= 0x2E ,	// shows the 16-bit timer value
		TCounterValueRegL		= 0x2F ,

		// Page 3: Test Registers
		// 						  0x30			// reserved for future use
		TestSel1Reg				= 0x31 ,	// general test signal configuration
		TestSel2Reg				= 0x32 ,	// general test signal configuration
		TestPinEnReg			= 0x33 ,	// enables pin output driver on pins D1 to D7
		TestPinValueReg			= 0x34 ,	// defines the values for D1 to D7 when it is used as an I/O bus
		TestBusReg				= 0x35 ,	// shows the status of the internal test bus
		AutoTestReg				= 0x36 ,	// controls the digital self test
		VersionReg				= 0x37 ,	// shows the software version
		AnalogTestReg			= 0x38 ,	// controls the pins AUX1 and AUX2
		TestDAC1Reg				= 0x39 ,	// defines the test value for TestDAC1
		TestDAC2Reg				= 0x3A ,	// defines the test value for TestDAC2
		TestADCReg				= 0x3B 		// shows the value of ADC I and Q channels
		// 						  0x3C			// reserved for production tests
		// 						  0x3D			// reserved for production tests
		// 						  0x3E			// reserved for production tests
		// 						  0x3F			// reserved for production tests
	};

	// MFRC522 commands. Described in chapter 10 of the datasheet.
	enum PCD_Command {
		PCD_Idle				= 0x00,		// no action, cancels current command execution
		PCD_Mem					= 0x01,		// stores 25 bytes into the internal buffer
		PCD_GenerateRandomID	= 0x02,		// generates a 10-byte random ID number
		PCD_CalcCRC				= 0x03,		// activates the CRC coprocessor or performs a self test
		PCD_Transmit			= 0x04,		// transmits data from the FIFO buffer
		PCD_NoCmdChange			= 0x07,		// no command change, can be used to modify the CommandReg register bits without affecting the command, for example, the PowerDown bit
		PCD_Receive				= 0x08,		// activates the receiver circuits
		PCD_Transceive 			= 0x0C,		// transmits data from FIFO buffer to antenna and automatically activates the receiver after transmission
		PCD_MFAuthent 			= 0x0E,		// performs the MIFARE standard authentication as a reader
		PCD_SoftReset			= 0x0F		// resets the MFRC522
	};

	// MFRC522 RxGain[2:0] masks, defines the receiver's signal voltage gain factor (on the PCD).
	// Described in 9.3.3.6 / table 98 of the datasheet at http://www.nxp.com/documents/data_sheet/MFRC522.pdf
	enum PCD_RxGain {
		RxGain_18dB				= 0x00 << 4,	// 000b - 18 dB, minimum
		RxGain_23dB				= 0x01 << 4,	// 001b - 23 dB
		RxGain_18dB_2			= 0x02 << 4,	// 010b - 18 dB, it seems 010b is a duplicate for 000b
		RxGain_23dB_2			= 0x03 << 4,	// 011b - 23 dB, it seems 011b is a duplicate for 001b
		RxGain_33dB				= 0x04 << 4,	// 100b - 33 dB, average, and typical default
		RxGain_38dB				= 0x05 << 4,	// 101b - 38 dB
		RxGain_43dB				= 0x06 << 4,	// 110b - 43 dB
		RxGain_48dB				= 0x07 << 4,	// 111b - 48 dB, maximum
		RxGain_min				= 0x00 << 4,	// 000b - 18 dB, minimum, convenience for RxGain_18dB
		RxGain_avg				= 0x04 << 4,	// 100b - 33 dB, average, convenience for RxGain_33dB
		RxGain_max				= 0x07 << 4		// 111b - 48 dB, maximum, convenience for RxGain_48dB
	};

	// Commands sent to the PICC.
	enum PICC_Command {
		// The commands used by the PCD to manage communication with several PICCs (ISO 14443-3, Type A, section 6.4)
		PICC_CMD_REQA			= 0x26,		// REQuest command, Type A. Invites PICCs in state IDLE to go to READY and prepare for anticollision or selection. 7 bit frame.
		PICC_CMD_WUPA			= 0x52,		// Wake-UP command, Type A. Invites PICCs in state IDLE and HALT to go to READY(*) and prepare for anticollision or selection. 7 bit frame.
		PICC_CMD_CT				= 0x88,		// Cascade Tag. Not really a command, but used during anti collision.
		PICC_CMD_SEL_CL1		= 0x93,		// Anti collision/Select, Cascade Level 1
		PICC_CMD_SEL_CL2		= 0x95,		// Anti collision/Select, Cascade Level 2
		PICC_CMD_SEL_CL3		= 0x97,		// Anti collision/Select, Cascade Level 3
		PICC_CMD_HLTA			= 0x50,		// HaLT command, Type A. Instructs an ACTIVE PICC to go to state HALT.
		// The commands used for MIFARE Classic (from http://www.nxp.com/documents/data_sheet/MF1S503x.pdf, Section 9)
		// Use PCD_MFAuthent to authenticate access to a sector, then use these commands to read/write/modify the blocks on the sector.
		// The read/write commands can also be used for MIFARE Ultralight.
		PICC_CMD_MF_AUTH_KEY_A	= 0x60,		// Perform authentication with Key A
		PICC_CMD_MF_AUTH_KEY_B	= 0x61,		// Perform authentication with Key B
		PICC_CMD_MF_READ		= 0x30,		// Reads one 16 byte block from the authenticated sector of the PICC. Also used for MIFARE Ultralight.
		PICC_CMD_MF_WRITE		= 0xA0,		// Writes one 16 byte block to the authenticated sector of the PICC. Called "COMPATIBILITY WRITE" for MIFARE Ultralight.
		PICC_CMD_MF_DECREMENT	= 0xC0,		// Decrements the contents of a block and stores the result in the internal data register.
		PICC_CMD_MF_INCREMENT	= 0xC1,		// Increments the contents of a block and stores the result in the internal data register.
		PICC_CMD_MF_RESTORE		= 0xC2,		// Reads the contents of a block into the internal data register.
		PICC_CMD_MF_TRANSFER	= 0xB0,		// Writes the contents of the internal data register to a block.
		// The commands used for MIFARE Ultralight (from http://www.nxp.com/documents/data_sheet/MF0ICU1.pdf, Section 8.6)
		// The PICC_CMD_MF_READ and PICC_CMD_MF_WRITE can also be used for MIFARE Ultralight.
		PICC_CMD_UL_WRITE		= 0xA2		// Writes one 4 byte page to the PICC.
	};

	// MIFARE constants that does not fit anywhere else
	enum MIFARE_Misc {
		MF_ACK					= 0xA,		// The MIFARE Classic uses a 4 bit ACK/NAK. Any other value than 0xA is NAK.
		MF_KEY_SIZE				= 6			// A Mifare Crypto1 key is 6 bytes.
	};

	// PICC types we can detect. Remember to update PICC_GetTypeName() if you add more.
	enum PICC_Type {
		PICC_TYPE_UNKNOWN		= 0,
		PICC_TYPE_ISO_14443_4	= 1,	// PICC compliant with ISO/IEC 14443-4
		PICC_TYPE_ISO_18092		= 2, 	// PICC compliant with ISO/IEC 18092 (NFC)
		PICC_TYPE_MIFARE_MINI	= 3,	// MIFARE Classic protocol, 320 bytes
		PICC_TYPE_MIFARE_1K		= 4,	// MIFARE Classic protocol, 1KB
		PICC_TYPE_MIFARE_4K		= 5,	// MIFARE Classic protocol, 4KB
		PICC_TYPE_MIFARE_UL		= 6,	// MIFARE Ultralight or Ultralight C
		PICC_TYPE_MIFARE_PLUS	= 7,	// MIFARE Plus
		PICC_TYPE_TNP3XXX		= 8,	// Only mentioned in NXP AN 10833 MIFARE Type Identification Procedure
		PICC_TYPE_NOT_COMPLETE	= 255	// SAK indicates UID is not complete.
	};

	// Return codes from the functions in this class. Remember to update GetStatusCodeName() if you add more.
	enum StatusCode {
		STATUS_OK				= 1,	// Success
		STATUS_ERROR			= 2,	// Error in communication
		STATUS_COLLISION		= 3,	// Collission detected
		STATUS_TIMEOUT			= 4,	// Timeout in communication.
		STATUS_NO_ROOM			= 5,	// A buffer is not big enough.
		STATUS_INTERNAL_ERROR	= 6,	// Internal error in the code. Should not happen ;-)
		STATUS_INVALID			= 7,	// Invalid argument.
		STATUS_CRC_WRONG		= 8,	// The CRC_A does not match
		STATUS_MIFARE_NACK		= 9		// A MIFARE PICC responded with NAK.
	};

	// A struct used for passing the UID of a PICC.
	typedef struct {
		byte		size;			// Number of bytes in the UID. 4, 7 or 10.
		byte		uidByte[10];
		byte		sak;			// The SAK (Select acknowledge) byte returned from the PICC after successful selection.
	} Uid;

	// A struct used for passing a MIFARE Crypto1 key
	typedef struct {
		byte		keyByte[MF_KEY_SIZE];
	} MIFARE_Key;

	// Member variables
	Uid uid;								// Used by PICC_ReadCardSerial().

	// Size of the MFRC522 FIFO
	static const byte FIFO_SIZE = 64;		// The FIFO is 64 bytes.

	/////////////////////////////////////////////////////////////////////////////////////
	// Functions for setting up the Arduino
	/////////////////////////////////////////////////////////////////////////////////////
	MFRC522(byte chipAddress);

	/////////////////////////////////////////////////////////////////////////////////////
	// Basic interface functions for communicating with the MFRC522
	/////////////////////////////////////////////////////////////////////////////////////
	void PCD_WriteRegister(byte reg, byte value);
	void PCD_WriteRegister(byte reg, byte count, byte *values);
	byte PCD_ReadRegister(byte reg);
	void PCD_ReadRegister(byte reg, byte count, byte *values, byte rxAlign = 0);
	void setBitMask(unsigned char reg, unsigned char mask);
	void PCD_SetRegisterBitMask(byte reg, byte mask);
	void PCD_ClearRegisterBitMask(byte reg, byte mask);
	byte PCD_CalculateCRC(byte *data, byte length, byte *result);

	/////////////////////////////////////////////////////////////////////////////////////
	// Functions for manipulating the MFRC522
	/////////////////////////////////////////////////////////////////////////////////////
	void PCD_Init();
	void PCD_Reset();
	void PCD_AntennaOn();
	void PCD_AntennaOff();
	byte PCD_GetAntennaGain();
	void PCD_SetAntennaGain(byte mask);
	bool PCD_PerformSelfTest();

	/////////////////////////////////////////////////////////////////////////////////////
	// Functions for communicating with PICCs
	/////////////////////////////////////////////////////////////////////////////////////
	byte PCD_TransceiveData(byte *sendData, byte sendLen, byte *backData, byte *backLen, byte *validBits = NULL, byte rxAlign = 0, bool checkCRC = false);
	byte PCD_CommunicateWithPICC(byte command, byte waitIRq, byte *sendData, byte sendLen, byte *backData = NULL, byte *backLen = NULL, byte *validBits = NULL, byte rxAlign = 0, bool checkCRC = false);
	byte PICC_RequestA(byte *bufferATQA, byte *bufferSize);
	byte PICC_WakeupA(byte *bufferATQA, byte *bufferSize);
	byte PICC_REQA_or_WUPA(byte command, byte *bufferATQA, byte *bufferSize);
	byte PICC_Select(Uid *uid, byte validBits = 0);
	byte PICC_HaltA();

	/////////////////////////////////////////////////////////////////////////////////////
	// Functions for communicating with MIFARE PICCs
	/////////////////////////////////////////////////////////////////////////////////////
	byte PCD_Authenticate(byte command, byte blockAddr, MIFARE_Key *key, Uid *uid);
	void PCD_StopCrypto1();
	byte MIFARE_Read(byte blockAddr, byte *buffer, byte *bufferSize);
	byte MIFARE_Write(byte blockAddr, byte *buffer, byte bufferSize);
	byte MIFARE_Decrement(byte blockAddr, long delta);
	byte MIFARE_Increment(byte blockAddr, long delta);
	byte MIFARE_Restore(byte blockAddr);
	byte MIFARE_Transfer(byte blockAddr);
	byte MIFARE_Ultralight_Write(byte page, byte *buffer, byte bufferSize);
	byte MIFARE_GetValue(byte blockAddr, long *value);
	byte MIFARE_SetValue(byte blockAddr, long value);

	/////////////////////////////////////////////////////////////////////////////////////
	// Support functions
	/////////////////////////////////////////////////////////////////////////////////////
	byte PCD_MIFARE_Transceive(byte *sendData, byte sendLen, bool acceptTimeout = false);
	// old function used too much memory, now name moved to flash; if you need char, copy from flash to memory
	//const char *GetStatusCodeName(byte code);
	const __FlashStringHelper *GetStatusCodeName(byte code);
	byte PICC_GetType(byte sak);
	// old function used too much memory, now name moved to flash; if you need char, copy from flash to memory
	//const char *PICC_GetTypeName(byte type);
	const __FlashStringHelper *PICC_GetTypeName(byte type);
	void PICC_DumpToSerial(Uid *uid);
	void PICC_DumpMifareClassicToSerial(Uid *uid, byte piccType, MIFARE_Key *key);
	void PICC_DumpMifareClassicSectorToSerial(Uid *uid, MIFARE_Key *key, byte sector);
	void PICC_DumpMifareUltralightToSerial();
	void MIFARE_SetAccessBits(byte *accessBitBuffer, byte g0, byte g1, byte g2, byte g3);
	bool MIFARE_OpenUidBackdoor(bool logErrors);
	bool MIFARE_SetUid(byte *newUid, byte uidSize, bool logErrors);
	bool MIFARE_UnbrickUidSector(bool logErrors);

	/////////////////////////////////////////////////////////////////////////////////////
	// Convenience functions - does not add extra functionality
	/////////////////////////////////////////////////////////////////////////////////////
	bool PICC_IsNewCardPresent();
	bool PICC_ReadCardSerial();

private:
	byte _chipAddress;
	byte _resetPowerDownPin;	// Arduino pin connected to MFRC522's reset and power down input (Pin 6, NRSTPD, active low)
	byte MIFARE_TwoStepHelper(byte command, byte blockAddr, long data);
};

#endif
