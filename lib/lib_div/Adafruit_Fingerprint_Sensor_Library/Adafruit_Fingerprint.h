#ifndef ADAFRUIT_FINGERPRINT_H
#define ADAFRUIT_FINGERPRINT_H

/***************************************************
  This is a library for our optical Fingerprint sensor

  Designed specifically to work with the Adafruit Fingerprint sensor
  ----> http://www.adafruit.com/products/751

  These displays use TTL Serial to communicate, 2 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include "Arduino.h"
#if defined(__AVR__) || defined(ESP8266)
  //#include <SoftwareSerial.h>
  #include <TasmotaSerial.h>
#elif defined(FREEDOM_E300_HIFIVE1)
  #include <SoftwareSerial32.h>
  #define SoftwareSerial SoftwareSerial32
#endif

#define FINGERPRINT_OK 0x00
#define FINGERPRINT_PACKETRECIEVEERR 0x01
#define FINGERPRINT_NOFINGER 0x02
#define FINGERPRINT_IMAGEFAIL 0x03
#define FINGERPRINT_IMAGEMESS 0x06
#define FINGERPRINT_FEATUREFAIL 0x07
#define FINGERPRINT_NOMATCH 0x08
#define FINGERPRINT_NOTFOUND 0x09
#define FINGERPRINT_ENROLLMISMATCH 0x0A
#define FINGERPRINT_BADLOCATION 0x0B
#define FINGERPRINT_DBRANGEFAIL 0x0C
#define FINGERPRINT_UPLOADFEATUREFAIL 0x0D
#define FINGERPRINT_PACKETRESPONSEFAIL 0x0E
#define FINGERPRINT_UPLOADFAIL 0x0F
#define FINGERPRINT_DELETEFAIL 0x10
#define FINGERPRINT_DBCLEARFAIL 0x11
#define FINGERPRINT_PASSFAIL 0x13
#define FINGERPRINT_INVALIDIMAGE 0x15
#define FINGERPRINT_FLASHERR 0x18
#define FINGERPRINT_INVALIDREG 0x1A
#define FINGERPRINT_ADDRCODE 0x20
#define FINGERPRINT_PASSVERIFY 0x21

#define FINGERPRINT_STARTCODE 0xEF01

#define FINGERPRINT_COMMANDPACKET 0x1
#define FINGERPRINT_DATAPACKET 0x2
#define FINGERPRINT_ACKPACKET 0x7
#define FINGERPRINT_ENDDATAPACKET 0x8

#define FINGERPRINT_TIMEOUT 0xFF
#define FINGERPRINT_BADPACKET 0xFE

#define FINGERPRINT_GETIMAGE 0x01
#define FINGERPRINT_IMAGE2TZ 0x02
#define FINGERPRINT_REGMODEL 0x05
#define FINGERPRINT_STORE 0x06
#define FINGERPRINT_LOAD 0x07
#define FINGERPRINT_UPLOAD 0x08
#define FINGERPRINT_DELETE 0x0C
#define FINGERPRINT_EMPTY 0x0D
#define FINGERPRINT_SETPASSWORD 0x12
#define FINGERPRINT_VERIFYPASSWORD 0x13
#define FINGERPRINT_HISPEEDSEARCH 0x1B
#define FINGERPRINT_TEMPLATECOUNT 0x1D

//#define FINGERPRINT_DEBUG

#define DEFAULTTIMEOUT 1000  ///< UART reading timeout in milliseconds

///! Helper class to craft UART packets
struct Adafruit_Fingerprint_Packet {

/**************************************************************************/
/*!
    @brief   Create a new UART-borne packet
    @param   type Command, data, ack type packet
    @param   length Size of payload
    @param   data Pointer to bytes of size length we will memcopy into the internal buffer
*/
/**************************************************************************/

  Adafruit_Fingerprint_Packet(uint8_t type, uint16_t length, uint8_t * data) {
    this->start_code = FINGERPRINT_STARTCODE;
    this->type = type;
    this->length = length;
    address[0] = 0xFF; address[1] = 0xFF;
    address[2] = 0xFF; address[3] = 0xFF;
    if(length<64)
      memcpy(this->data, data, length);
    else
      memcpy(this->data, data, 64);
  }
  uint16_t start_code;      ///< "Wakeup" code for packet detection
  uint8_t address[4];       ///< 32-bit Fingerprint sensor address
  uint8_t type;             ///< Type of packet
  uint16_t length;          ///< Length of packet
  uint8_t data[64];         ///< The raw buffer for packet payload
};

///! Helper class to communicate with and keep state for fingerprint sensors
class Adafruit_Fingerprint {
 public:
   uint8_t mydata[100];

#if defined(__AVR__) || defined(ESP8266) || defined(FREEDOM_E300_HIFIVE1)
  Adafruit_Fingerprint(TasmotaSerial *ss, uint32_t password = 0x0);
#endif
  Adafruit_Fingerprint(HardwareSerial *hs, uint32_t password = 0x0);

  void begin(uint32_t baud);

  boolean verifyPassword(void);
  uint8_t getImage(void);
  uint8_t image2Tz(uint8_t slot = 1);
  uint8_t createModel(void);

  uint8_t emptyDatabase(void);
  uint8_t storeModel(uint16_t id);
  uint8_t loadModel(uint16_t id);
  uint8_t getModel(void);
  uint8_t deleteModel(uint16_t id);
  uint8_t fingerFastSearch(void);
  uint8_t getTemplateCount(void);
  uint8_t setPassword(uint32_t password);
  void writeStructuredPacket(const Adafruit_Fingerprint_Packet & p);
  uint8_t getStructuredPacket(Adafruit_Fingerprint_Packet * p, uint16_t timeout=DEFAULTTIMEOUT);

  /// The matching location that is set by fingerFastSearch()
  uint16_t fingerID;
  /// The confidence of the fingerFastSearch() match, higher numbers are more confidents
  uint16_t confidence;
  /// The number of stored templates in the sensor, set by getTemplateCount()
  uint16_t templateCount;

 private:
  uint8_t checkPassword(void);
  uint32_t thePassword;
  uint32_t theAddress;
    uint8_t recvPacket[20];

  Stream *mySerial;
#if defined(__AVR__) || defined(ESP8266) || defined(FREEDOM_E300_HIFIVE1)
  TasmotaSerial *swSerial;
#endif
  HardwareSerial *hwSerial;
};

#endif
