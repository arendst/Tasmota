#ifndef ADAFRUIT_FINGERPRINT_H
#define ADAFRUIT_FINGERPRINT_H

/*!
 * @file Adafruit_Fingerprint.h
 */

#include "Arduino.h"
#if defined(__AVR__) || defined(ESP8266) || defined(ESP32)
//#include <SoftwareSerial.h>
#include <TasmotaSerial.h>
#elif defined(FREEDOM_E300_HIFIVE1)
#include <SoftwareSerial32.h>
#define SoftwareSerial SoftwareSerial32
#endif

#define FINGERPRINT_OK 0x00               //!< Command execution is complete
#define FINGERPRINT_PACKETRECIEVEERR 0x01 //!< Error when receiving data package
#define FINGERPRINT_NOFINGER 0x02         //!< No finger on the sensor
#define FINGERPRINT_IMAGEFAIL 0x03        //!< Failed to enroll the finger
#define FINGERPRINT_IMAGEMESS                                                  \
  0x06 //!< Failed to generate character file due to overly disorderly
       //!< fingerprint image
#define FINGERPRINT_FEATUREFAIL                                                \
  0x07 //!< Failed to generate character file due to the lack of character point
       //!< or small fingerprint image
#define FINGERPRINT_NOMATCH 0x08  //!< Finger doesn't match
#define FINGERPRINT_NOTFOUND 0x09 //!< Failed to find matching finger
#define FINGERPRINT_ENROLLMISMATCH                                             \
  0x0A //!< Failed to combine the character files
#define FINGERPRINT_BADLOCATION                                                \
  0x0B //!< Addressed PageID is beyond the finger library
#define FINGERPRINT_DBRANGEFAIL                                                \
  0x0C //!< Error when reading template from library or invalid template
#define FINGERPRINT_UPLOADFEATUREFAIL 0x0D //!< Error when uploading template
#define FINGERPRINT_PACKETRESPONSEFAIL                                         \
  0x0E //!< Module failed to receive the following data packages
#define FINGERPRINT_UPLOADFAIL 0x0F  //!< Error when uploading image
#define FINGERPRINT_DELETEFAIL 0x10  //!< Failed to delete the template
#define FINGERPRINT_DBCLEARFAIL 0x11 //!< Failed to clear finger library
#define FINGERPRINT_PASSFAIL                                                   \
  0x13 //!< Find whether the fingerprint passed or failed
#define FINGERPRINT_INVALIDIMAGE                                               \
  0x15 //!< Failed to generate image because of lac of valid primary image
#define FINGERPRINT_FLASHERR 0x18   //!< Error when writing flash
#define FINGERPRINT_INVALIDREG 0x1A //!< Invalid register number
#define FINGERPRINT_ADDRCODE 0x20   //!< Address code
#define FINGERPRINT_PASSVERIFY 0x21 //!< Verify the fingerprint passed
#define FINGERPRINT_STARTCODE                                                  \
  0xEF01 //!< Fixed falue of EF01H; High byte transferred first

#define FINGERPRINT_COMMANDPACKET 0x1 //!< Command packet
#define FINGERPRINT_DATAPACKET                                                 \
  0x2 //!< Data packet, must follow command packet or acknowledge packet
#define FINGERPRINT_ACKPACKET 0x7     //!< Acknowledge packet
#define FINGERPRINT_ENDDATAPACKET 0x8 //!< End of data packet

#define FINGERPRINT_TIMEOUT 0xFF   //!< Timeout was reached
#define FINGERPRINT_BADPACKET 0xFE //!< Bad packet was sent

#define FINGERPRINT_GETIMAGE 0x01 //!< Collect finger image
#define FINGERPRINT_IMAGE2TZ 0x02 //!< Generate character file from image
#define FINGERPRINT_SEARCH 0x04   //!< Search for fingerprint in slot
#define FINGERPRINT_REGMODEL                                                   \
  0x05 //!< Combine character files and generate template
#define FINGERPRINT_STORE 0x06          //!< Store template
#define FINGERPRINT_LOAD 0x07           //!< Read/load template
#define FINGERPRINT_UPLOAD 0x08         //!< Upload template
#define FINGERPRINT_DELETE 0x0C         //!< Delete templates
#define FINGERPRINT_EMPTY 0x0D          //!< Empty library
#define FINGERPRINT_READSYSPARAM 0x0F   //!< Read system parameters
#define FINGERPRINT_SETPASSWORD 0x12    //!< Sets passwords
#define FINGERPRINT_VERIFYPASSWORD 0x13 //!< Verifies the password
#define FINGERPRINT_HISPEEDSEARCH                                              \
  0x1B //!< Asks the sensor to search for a matching fingerprint template to the
       //!< last model generated
#define FINGERPRINT_TEMPLATECOUNT 0x1D //!< Read finger template numbers
#define FINGERPRINT_AURALEDCONFIG 0x35 //!< Aura LED control
#define FINGERPRINT_LEDON 0x50         //!< Turn on the onboard LED
#define FINGERPRINT_LEDOFF 0x51        //!< Turn off the onboard LED

#define FINGERPRINT_LED_BREATHING 0x01   //!< Breathing light
#define FINGERPRINT_LED_FLASHING 0x02    //!< Flashing light
#define FINGERPRINT_LED_ON 0x03          //!< Always on
#define FINGERPRINT_LED_OFF 0x04         //!< Always off
#define FINGERPRINT_LED_GRADUAL_ON 0x05  //!< Gradually on
#define FINGERPRINT_LED_GRADUAL_OFF 0x06 //!< Gradually off
#define FINGERPRINT_LED_RED 0x01         //!< Red LED
#define FINGERPRINT_LED_BLUE 0x02        //!< Blue LED
#define FINGERPRINT_LED_PURPLE 0x03      //!< Purple LEDpassword

#define FINGERPRINT_REG_ADDR_ERROR 0x1A //!< shows register address error
#define FINGERPRINT_WRITE_REG 0x0E      //!< Write system register instruction

#define FINGERPRINT_BAUD_REG_ADDR 0x4   //!< BAUDRATE register address
#define FINGERPRINT_BAUDRATE_9600 0x1   //!< UART baud 9600
#define FINGERPRINT_BAUDRATE_19200 0x2  //!< UART baud 19200
#define FINGERPRINT_BAUDRATE_28800 0x3  //!< UART baud 28800
#define FINGERPRINT_BAUDRATE_38400 0x4  //!< UART baud 38400
#define FINGERPRINT_BAUDRATE_48000 0x5  //!< UART baud 48000
#define FINGERPRINT_BAUDRATE_57600 0x6  //!< UART baud 57600
#define FINGERPRINT_BAUDRATE_67200 0x7  //!< UART baud 67200
#define FINGERPRINT_BAUDRATE_76800 0x8  //!< UART baud 76800
#define FINGERPRINT_BAUDRATE_86400 0x9  //!< UART baud 86400
#define FINGERPRINT_BAUDRATE_96000 0xA  //!< UART baud 96000
#define FINGERPRINT_BAUDRATE_105600 0xB //!< UART baud 105600
#define FINGERPRINT_BAUDRATE_115200 0xC //!< UART baud 115200

#define FINGERPRINT_SECURITY_REG_ADDR 0x5 //!< Security level register address
// The safety level is 1 The highest rate of false recognition , The rejection
// rate is the lowest . The safety level is 5 The lowest tate of false
// recognition, The rejection rate is the highest .
#define FINGERPRINT_SECURITY_LEVEL_1 0X1 //!< Security level 1
#define FINGERPRINT_SECURITY_LEVEL_2 0X2 //!< Security level 2
#define FINGERPRINT_SECURITY_LEVEL_3 0X3 //!< Security level 3
#define FINGERPRINT_SECURITY_LEVEL_4 0X4 //!< Security level 4
#define FINGERPRINT_SECURITY_LEVEL_5 0X5 //!< Security level 5

#define FINGERPRINT_PACKET_REG_ADDR 0x6 //!< Packet size register address
#define FINGERPRINT_PACKET_SIZE_32 0X0  //!< Packet size is 32 Byte
#define FINGERPRINT_PACKET_SIZE_64 0X1  //!< Packet size is 64 Byte
#define FINGERPRINT_PACKET_SIZE_128 0X2 //!< Packet size is 128 Byte
#define FINGERPRINT_PACKET_SIZE_256 0X3 //!< Packet size is 256 Byte

//#define FINGERPRINT_DEBUG

#define DEFAULTTIMEOUT 1000 //!< UART reading timeout in milliseconds

///! Helper class to craft UART packets
struct Adafruit_Fingerprint_Packet {

  /**************************************************************************/
  /*!
      @brief   Create a new UART-borne packet
      @param   type Command, data, ack type packet
      @param   length Size of payload
      @param   data Pointer to bytes of size length we will memcopy into the
     internal buffer
  */
  /**************************************************************************/

  Adafruit_Fingerprint_Packet(uint8_t type, uint16_t length, uint8_t *data) {
    this->start_code = FINGERPRINT_STARTCODE;
    this->type = type;
    this->length = length;
    address[0] = 0xFF;
    address[1] = 0xFF;
    address[2] = 0xFF;
    address[3] = 0xFF;
    if (length < 64)
      memcpy(this->data, data, length);
    else
      memcpy(this->data, data, 64);
  }
  uint16_t start_code; ///< "Wakeup" code for packet detection
  uint8_t address[4];  ///< 32-bit Fingerprint sensor address
  uint8_t type;        ///< Type of packet
  uint16_t length;     ///< Length of packet
  uint8_t data[64];    ///< The raw buffer for packet payload
};

///! Helper class to communicate with and keep state for fingerprint sensors
class Adafruit_Fingerprint {
public:
#if defined(__AVR__) || defined(ESP8266) || defined(ESP32) || defined(FREEDOM_E300_HIFIVE1)
//  Adafruit_Fingerprint(SoftwareSerial *ss, uint32_t password = 0x0);
  Adafruit_Fingerprint(TasmotaSerial *ss, uint32_t password = 0x0);
#endif
  Adafruit_Fingerprint(HardwareSerial *hs, uint32_t password = 0x0);
  Adafruit_Fingerprint(Stream *serial, uint32_t password = 0x0);

  void begin(uint32_t baud);

  boolean verifyPassword(void);
  uint8_t getParameters(void);

  uint8_t getImage(void);
  uint8_t image2Tz(uint8_t slot = 1);
  uint8_t createModel(void);

  uint8_t emptyDatabase(void);
  uint8_t storeModel(uint16_t id);
  uint8_t loadModel(uint16_t id);
  uint8_t getModel(void);
  uint8_t deleteModel(uint16_t id);
  uint8_t fingerFastSearch(void);
  uint8_t fingerSearch(uint8_t slot = 1);
  uint8_t getTemplateCount(void);
  uint8_t setPassword(uint32_t password);
  uint8_t LEDcontrol(bool on);
  uint8_t LEDcontrol(uint8_t control, uint8_t speed, uint8_t coloridx,
                     uint8_t count = 0);

  uint8_t setBaudRate(uint8_t baudrate);
  uint8_t setSecurityLevel(uint8_t level);
  uint8_t setPacketSize(uint8_t size);

  void writeStructuredPacket(const Adafruit_Fingerprint_Packet &p);
  uint8_t getStructuredPacket(Adafruit_Fingerprint_Packet *p,
                              uint16_t timeout = DEFAULTTIMEOUT);

  /// The matching location that is set by fingerFastSearch()
  uint16_t fingerID;
  /// The confidence of the fingerFastSearch() match, higher numbers are more
  /// confidents
  uint16_t confidence;
  /// The number of stored templates in the sensor, set by getTemplateCount()
  uint16_t templateCount;

  uint16_t status_reg = 0x0; ///< The status register (set by getParameters)
  uint16_t system_id = 0x0;  ///< The system identifier (set by getParameters)
  uint16_t capacity = 64; ///< The fingerprint capacity (set by getParameters)
  uint16_t security_level = 0; ///< The security level (set by getParameters)
  uint32_t device_addr =
      0xFFFFFFFF;             ///< The device address (set by getParameters)
  uint16_t packet_len = 64;   ///< The max packet length (set by getParameters)
  uint16_t baud_rate = 57600; ///< The UART baud rate (set by getParameters)

private:
  uint8_t checkPassword(void);
  uint8_t writeRegister(uint8_t regAdd, uint8_t value);
  uint32_t thePassword;
  uint32_t theAddress;
  uint8_t recvPacket[20];

  Stream *mySerial;
#if defined(__AVR__) || defined(ESP8266) || defined(ESP32) || defined(FREEDOM_E300_HIFIVE1)
//  SoftwareSerial *swSerial;
  TasmotaSerial *swSerial;
#endif
  HardwareSerial *hwSerial;
};

#endif
