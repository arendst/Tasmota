/**
 * Library extends MFRC522.h to support RATS for ISO-14443-4 PICC.
 * RATS - Request for Answer To Select.
 * @author JPG-Consulting
 */
#ifndef MFRC522Extended_h
#define MFRC522Extended_h

#include <Arduino.h>
#include "MFRC522.h"

class MFRC522Extended : public MFRC522 {
		
public:
	// ISO/IEC 14443-4 bit rates
	enum TagBitRates : byte {
		BITRATE_106KBITS = 0x00,
		BITRATE_212KBITS = 0x01,
		BITRATE_424KBITS = 0x02,
		BITRATE_848KBITS = 0x03
	};

	// Structure to store ISO/IEC 14443-4 ATS
	typedef struct {
		byte size;
		byte fsc;                 // Frame size for proximity card

		struct {
			bool transmitted;
			bool        sameD;	// Only the same D for both directions supported
			TagBitRates ds;		// Send D
			TagBitRates dr;		// Receive D
		} ta1;

		struct {
			bool transmitted;
			byte fwi;			// Frame waiting time integer
			byte sfgi;			// Start-up frame guard time integer
		} tb1;

		struct {
			bool transmitted;
			bool supportsCID;
			bool supportsNAD;
		} tc1;

		// Raw data from ATS
		byte data[FIFO_SIZE - 2]; // ATS cannot be bigger than FSD - 2 bytes (CRC), according to ISO 14443-4 5.2.2
	} Ats;

	// A struct used for passing the PICC information
	typedef struct {
		uint16_t	atqa;
		Uid			uid;
		Ats		    ats; 

		// For Block PCB
		bool blockNumber;
	} TagInfo;

	// A struct used for passing PCB Block
	typedef struct {
		struct {
			byte pcb;
			byte cid;
			byte nad;
		} prologue;
		struct {
			byte size;
			byte *data;
		} inf;
	} PcbBlock;
	
	// Member variables
	TagInfo tag;
	
	/////////////////////////////////////////////////////////////////////////////////////
	// Contructors
	/////////////////////////////////////////////////////////////////////////////////////
	MFRC522Extended() : MFRC522() {};
	MFRC522Extended(uint8_t rst) : MFRC522(rst) {};
	MFRC522Extended(uint8_t ss, uint8_t rst) : MFRC522(ss, rst) {};
	
	/////////////////////////////////////////////////////////////////////////////////////
	// Functions for communicating with PICCs
	/////////////////////////////////////////////////////////////////////////////////////
	StatusCode PICC_Select(Uid *uid, byte validBits = 0) override; // overrride
	StatusCode PICC_RequestATS(Ats *ats);
	StatusCode PICC_PPS();	                                                  // PPS command without bitrate parameter
	StatusCode PICC_PPS(TagBitRates sendBitRate, TagBitRates receiveBitRate); // Different D values
	
	/////////////////////////////////////////////////////////////////////////////////////
	// Functions for communicating with ISO/IEC 14433-4 cards
	/////////////////////////////////////////////////////////////////////////////////////
	StatusCode TCL_Transceive(PcbBlock *send, PcbBlock *back);
	StatusCode TCL_Transceive(TagInfo * tag, byte *sendData, byte sendLen, byte *backData = NULL, byte *backLen = NULL);
	StatusCode TCL_TransceiveRBlock(TagInfo *tag, bool ack, byte *backData = NULL, byte *backLen = NULL);
	StatusCode TCL_Deselect(TagInfo *tag);
	
	/////////////////////////////////////////////////////////////////////////////////////
	// Support functions
	/////////////////////////////////////////////////////////////////////////////////////
	static PICC_Type PICC_GetType(TagInfo *tag);
	using MFRC522::PICC_GetType;// // make old PICC_GetType(byte sak) available, otherwise would be hidden by PICC_GetType(TagInfo *tag)

	// Support functions for debuging
	void PICC_DumpToSerial(TagInfo *tag);
	using MFRC522::PICC_DumpToSerial; // make old PICC_DumpToSerial(Uid *uid) available, otherwise would be hidden by PICC_DumpToSerial(TagInfo *tag)
	void PICC_DumpDetailsToSerial(TagInfo *tag);
	using MFRC522::PICC_DumpDetailsToSerial; // make old PICC_DumpDetailsToSerial(Uid *uid) available, otherwise would be hidden by PICC_DumpDetailsToSerial(TagInfo *tag)
	void PICC_DumpISO14443_4(TagInfo *tag);
	
	/////////////////////////////////////////////////////////////////////////////////////
	// Convenience functions - does not add extra functionality
	/////////////////////////////////////////////////////////////////////////////////////
	bool PICC_IsNewCardPresent() override; // overrride
	bool PICC_ReadCardSerial() override; // overrride
};

#endif
