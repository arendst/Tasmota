 /***************************************************
 * IRremote for ESP8266
 *
 * Based on the IRremote library for Arduino by Ken Shirriff
 * Version 0.11 August, 2009
 * Copyright 2009 Ken Shirriff
 * For details, see http://arcfn.com/2009/08/multi-protocol-infrared-remote-library.html
 *
 * Edited by Mitra to add new controller SANYO
 *
 * Interrupt code based on NECIRrcv by Joe Knapp
 * http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1210243556
 * Also influenced by http://zovirl.com/2008/11/12/building-a-universal-remote-with-an-arduino/
 *
 * JVC and Panasonic protocol added by Kristian Lauszus (Thanks to zenwheel and other people at the original blog post)
 * LG added by Darryl Smith (based on the JVC protocol)
 * Whynter A/C ARC-110WD added by Francesco Meschia
 * Coolix A/C / heatpump added by bakrus
 * Denon: sendDenon, decodeDenon added by Massimiliano Pinto
          (from https://github.com/z3t0/Arduino-IRremote/blob/master/ir_Denon.cpp)
 * Kelvinator A/C and Sherwood added by crankyoldgit
 * Updated by markszabo (https://github.com/markszabo/IRremoteESP8266) for sending IR code on ESP8266
 * Updated by Sebastien Warin (http://sebastien.warin.fr) for receiving IR code on ESP8266
 *
 *  Updated by sillyfrog for Daikin, adopted from
 * (https://github.com/mharizanov/Daikin-AC-remote-control-over-the-Internet/)
 *
 *  GPL license, all text above must be included in any redistribution
 ****************************************************/

#ifndef IRremote_h
#define IRremote_h

#include <stdint.h>

// The following are compile-time library options.
// If you change them, recompile the library.
// If DEBUG is defined, a lot of debugging output will be printed during decoding.
// TEST must be defined for the IRtest unittests to work.  It will make some
// methods virtual, which will be slightly slower, which is why it is optional.
//#define DEBUG
//#define TEST

/*
 * Always add to the end of the list and should never remove entries
 * or change order. Projects may save the type number for later usage
 * so numbering should always stay the same.
 */
enum decode_type_t {
  UNKNOWN = -1,
  UNUSED = 0,
  RC5,
  RC6,
  NEC,
  SONY,
  PANASONIC,
  JVC,
  SAMSUNG,
  WHYNTER,
  AIWA_RC_T501,
  LG,
  SANYO,
  MITSUBISHI,
  DISH,
  SHARP,
  COOLIX,
  DAIKIN,
  DENON,
  KELVINATOR,
  SHERWOOD,
  MITSUBISHI_AC
};

// Results returned from the decoder
class decode_results {
public:
  int decode_type; // NEC, SONY, RC5, UNKNOWN
  union { // This is used for decoding Panasonic and Sharp data
    unsigned int panasonicAddress;
    unsigned int sharpAddress;
  };
  unsigned long value; // Decoded value
  int bits; // Number of bits in decoded value
  volatile unsigned int *rawbuf; // Raw intervals in .5 us ticks
  int rawlen; // Number of records in rawbuf.
  bool overflow;
};

// Decoded value for NEC when a repeat code is received
#define REPEAT 0xffffffff

#define SEND_PROTOCOL_NEC     case NEC: sendNEC(data, nbits); break;
#define SEND_PROTOCOL_SONY    case SONY: sendSony(data, nbits); break;
#define SEND_PROTOCOL_RC5     case RC5: sendRC5(data, nbits); break;
#define SEND_PROTOCOL_RC6     case RC6: sendRC6(data, nbits); break;
#define SEND_PROTOCOL_DISH    case DISH: sendDISH(data, nbits); break;
#define SEND_PROTOCOL_JVC     case JVC: sendJVC(data, nbits, 0); break;
#define SEND_PROTOCOL_SAMSUNG case SAMSUNG: sendSAMSUNG(data, nbits); break;
#define SEND_PROTOCOL_LG      case LG: sendLG(data, nbits); break;
#define SEND_PROTOCOL_WHYNTER case WHYNTER: sendWhynter(data, nbits); break;
#define SEND_PROTOCOL_COOLIX  case COOLIX: sendCOOLIX(data, nbits); break;
#define SEND_PROTOCOL_DENON  case DENON: sendDenon(data, nbits); break;
#define SEND_PROTOCOL_SHERWOOD case SHERWOOD: sendSherwood(data, nbits); break;

// main class for receiving IR
class IRrecv
{
public:
  IRrecv(int recvpin);
  bool decode(decode_results *results);
  void enableIRIn();
  void disableIRIn();
  void resume();
  private:
  // These are called by decode
  int getRClevel(decode_results *results, int *offset, int *used, int t1);
  bool decodeNEC(decode_results *results);
  bool decodeSony(decode_results *results);
  bool decodeSanyo(decode_results *results);
  bool decodeMitsubishi(decode_results *results);
  bool decodeRC5(decode_results *results);
  bool decodeRC6(decode_results *results);
  bool decodePanasonic(decode_results *results);
  bool decodeLG(decode_results *results);
  bool decodeJVC(decode_results *results);
  bool decodeSAMSUNG(decode_results *results);
  bool decodeWhynter(decode_results *results);
  bool decodeHash(decode_results *results);
  // COOLIX decode is not implemented yet
  //  bool decodeCOOLIX(decode_results *results);
  bool decodeDaikin(decode_results *results);
  bool decodeDenon(decode_results *results);
  int compare(unsigned int oldval, unsigned int newval);
};

// Only used for testing; can remove virtual for shorter code
#ifdef TEST
#define VIRTUAL virtual
#else
#define VIRTUAL
#endif
class IRsend
{
public:
  IRsend(int IRsendPin);
  void begin();
  void send(int type, unsigned long data, int nbits) {
    switch (type) {
        SEND_PROTOCOL_NEC
        SEND_PROTOCOL_SONY
        SEND_PROTOCOL_RC5
        SEND_PROTOCOL_RC6
        SEND_PROTOCOL_DISH
        SEND_PROTOCOL_JVC
        SEND_PROTOCOL_SAMSUNG
        SEND_PROTOCOL_LG
        SEND_PROTOCOL_WHYNTER
        SEND_PROTOCOL_COOLIX
        SEND_PROTOCOL_DENON
        SEND_PROTOCOL_SHERWOOD
      }
  };
  void sendCOOLIX(unsigned long data, int nbits);
  void sendWhynter(unsigned long data, int nbits);
  void sendNEC(unsigned long data, int nbits=32, unsigned int repeat=0);
  void sendLG(unsigned long data, int nbits);
  // sendSony() should typically be called with repeat=2 as Sony devices
  // expect the code to be sent at least 3 times. (code + 2 repeats = 3 codes)
  // As the legacy use of this procedure was only to send a single code
  // it defaults to repeat=0 for backward compatiblity.
  void sendSony(unsigned long data, int nbits, unsigned int repeat=0);
  // Neither Sanyo nor Mitsubishi send is implemented yet
  //  void sendSanyo(unsigned long data, int nbits);
  //  void sendMitsubishi(unsigned long data, int nbits);
  void sendRaw(unsigned int buf[], int len, int hz);
  void sendGC(unsigned int buf[], int len);
  void sendRC5(unsigned long data, int nbits);
  void sendRC6(unsigned long data, int nbits);
  void sendDISH(unsigned long data, int nbits);
  void sendSharp(unsigned int address, unsigned int command);
  void sendSharpRaw(unsigned long data, int nbits);
  void sendPanasonic(unsigned int address, unsigned long data);
  void sendJVC(unsigned long data, int nbits, int repeat); // *Note instead of sending the REPEAT constant if you want the JVC repeat signal sent, send the original code value and change the repeat argument from 0 to 1. JVC protocol repeats by skipping the header NOT by sending a separate code value like NEC does.
  void sendSAMSUNG(unsigned long data, int nbits);
  void sendDaikin(unsigned char daikin[]);
  void sendDaikinChunk(unsigned char buf[], int len, int start);
  void sendDenon(unsigned long data, int nbits);
  void sendKelvinator(unsigned char data[]);
  void sendSherwood(unsigned long data, int nbits=32, unsigned int repeat=1);
  void sendMitsubishiAC(unsigned char data[]);
  void enableIROut(int khz);
  VIRTUAL void mark(unsigned int usec);
  VIRTUAL void space(unsigned long usec);
private:
  int halfPeriodicTime;
  int IRpin;
  void sendMitsubishiACChunk(unsigned char data);
  void sendData(uint16_t onemark, uint32_t onespace,
                uint16_t zeromark, uint32_t zerospace,
                uint32_t data, uint8_t nbits, bool MSBfirst=true);
  void ledOff();
} ;

class IRtimer {
public:
  IRtimer();
  void reset();
  uint32_t elapsed();
private:
  uint32_t start;
};

// Some useful constants
#define USECPERTICK 50  // microseconds per clock interrupt tick
#define RAWBUF 100 // Length of raw duration buffer

// Marks tend to be 100us too long, and spaces 100us too short
// when received due to sensor lag.
#define MARK_EXCESS 100

#endif
