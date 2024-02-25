#include "AX25.h"
#include <string.h>
#if !RADIOLIB_EXCLUDE_AX25

AX25Frame::AX25Frame(const char* destCallsign, uint8_t destSSID, const char* srcCallsign, uint8_t srcSSID, uint8_t control)
: AX25Frame(destCallsign, destSSID, srcCallsign, srcSSID, control, 0, NULL, 0) {

}

AX25Frame::AX25Frame(const char* destCallsign, uint8_t destSSID, const char* srcCallsign, uint8_t srcSSID, uint8_t control, uint8_t protocolID, const char* info)
  : AX25Frame(destCallsign, destSSID, srcCallsign, srcSSID, control, protocolID, (uint8_t*)info, strlen(info)) {

}

AX25Frame::AX25Frame(const char* destCallsign, uint8_t destSSID, const char* srcCallsign, uint8_t srcSSID, uint8_t control, uint8_t protocolID, uint8_t* info, uint16_t infoLen) {
  // destination callsign/SSID
  memcpy(this->destCallsign, destCallsign, strlen(destCallsign));
  this->destCallsign[strlen(destCallsign)] = '\0';
  this->destSSID = destSSID;

  // source callsign/SSID
  memcpy(this->srcCallsign, srcCallsign, strlen(srcCallsign));
  this->srcCallsign[strlen(srcCallsign)] = '\0';
  this->srcSSID = srcSSID;

  // set repeaters
  this->numRepeaters = 0;
  #if !RADIOLIB_STATIC_ONLY
    this->repeaterCallsigns = NULL;
    this->repeaterSSIDs = NULL;
  #endif

  // control field
  this->control = control;

  // sequence numbers
  this->rcvSeqNumber = 0;
  this->sendSeqNumber = 0;

  // PID field
  this->protocolID = protocolID;

  // info field
  this->infoLen = infoLen;
  if(infoLen > 0) {
    #if !RADIOLIB_STATIC_ONLY
      this->info = new uint8_t[infoLen];
    #endif
    memcpy(this->info, info, infoLen);
  }
}

AX25Frame::AX25Frame(const AX25Frame& frame) {
  *this = frame;
}

AX25Frame::~AX25Frame() {
  #if !RADIOLIB_STATIC_ONLY
    // deallocate info field
    if(infoLen > 0) {
      delete[] this->info;
    }

    // deallocate repeaters
    if(this->numRepeaters > 0) {
      for(uint8_t i = 0; i < this->numRepeaters; i++) {
        delete[] this->repeaterCallsigns[i];
      }
      delete[] this->repeaterCallsigns;
      delete[] this->repeaterSSIDs;
    }
  #endif
}

AX25Frame& AX25Frame::operator=(const AX25Frame& frame) {
  // destination callsign/SSID
  memcpy(this->destCallsign, frame.destCallsign, strlen(frame.destCallsign));
  this->destCallsign[strlen(frame.destCallsign)] = '\0';
  this->destSSID = frame.destSSID;

  // source callsign/SSID
  memcpy(this->srcCallsign, frame.srcCallsign, strlen(frame.srcCallsign));
  this->srcCallsign[strlen(frame.srcCallsign)] = '\0';
  this->srcSSID = frame.srcSSID;

  // repeaters
  this->numRepeaters = frame.numRepeaters;
  for(uint8_t i = 0; i < this->numRepeaters; i++) {
    memcpy(this->repeaterCallsigns[i], frame.repeaterCallsigns[i], strlen(frame.repeaterCallsigns[i]));
  }
  memcpy(this->repeaterSSIDs, frame.repeaterSSIDs, this->numRepeaters);

  // control field
  this->control = frame.control;

  // sequence numbers
  this->rcvSeqNumber = frame.rcvSeqNumber;
  this->sendSeqNumber = frame.sendSeqNumber;

  // PID field
  this->protocolID = frame.protocolID;

  // info field
  this->infoLen = frame.infoLen;
  memcpy(this->info, frame.info, this->infoLen);

  return(*this);
}

int16_t AX25Frame::setRepeaters(char** repeaterCallsigns, uint8_t* repeaterSSIDs, uint8_t numRepeaters) {
  // check number of repeaters
  if((numRepeaters < 1) || (numRepeaters > 8)) {
    return(RADIOLIB_ERR_INVALID_NUM_REPEATERS);
  }

  // check repeater configuration
  if((repeaterCallsigns == NULL) || (repeaterSSIDs == NULL)) {
    return(RADIOLIB_ERR_INVALID_NUM_REPEATERS);
  }
  for(uint16_t i = 0; i < numRepeaters; i++) {
    if(strlen(repeaterCallsigns[i]) > RADIOLIB_AX25_MAX_CALLSIGN_LEN) {
      return(RADIOLIB_ERR_INVALID_REPEATER_CALLSIGN);
    }
  }

  // create buffers
  #if !RADIOLIB_STATIC_ONLY
    this->repeaterCallsigns = new char*[numRepeaters];
    for(uint8_t i = 0; i < numRepeaters; i++) {
      this->repeaterCallsigns[i] = new char[strlen(repeaterCallsigns[i]) + 1];
    }
    this->repeaterSSIDs = new uint8_t[numRepeaters];
  #endif

  // copy data
  this->numRepeaters = numRepeaters;
  for(uint8_t i = 0; i < numRepeaters; i++) {
    memcpy(this->repeaterCallsigns[i], repeaterCallsigns[i], strlen(repeaterCallsigns[i]));
    this->repeaterCallsigns[i][strlen(repeaterCallsigns[i])] = '\0';
  }
  memcpy(this->repeaterSSIDs, repeaterSSIDs, numRepeaters);

  return(RADIOLIB_ERR_NONE);
}

void AX25Frame::setRecvSequence(uint8_t seqNumber) {
  this->rcvSeqNumber = seqNumber;
}

void AX25Frame::setSendSequence(uint8_t seqNumber) {
  this->sendSeqNumber = seqNumber;
}

AX25Client::AX25Client(PhysicalLayer* phy) {
  phyLayer = phy;
  #if !RADIOLIB_EXCLUDE_AFSK
  bellModem = nullptr;
  #endif
}

#if !RADIOLIB_EXCLUDE_AFSK
AX25Client::AX25Client(AFSKClient* audio) {
  phyLayer = audio->phyLayer;
  bellModem = new BellClient(audio);
  bellModem->setModem(Bell202);
}

int16_t AX25Client::setCorrection(int16_t mark, int16_t space, float length) {
  BellModem_t modem;
  modem.freqMark = Bell202.freqMark + mark;
  modem.freqSpace = Bell202.freqSpace + space;
  modem.freqMarkReply = modem.freqMark;
  modem.freqSpaceReply = modem.freqSpace;
  modem.baudRate = length*(float)Bell202.baudRate;
  bellModem->setModem(modem);
  return(RADIOLIB_ERR_NONE);
}
#endif

int16_t AX25Client::begin(const char* srcCallsign, uint8_t srcSSID, uint8_t preLen) {
  // set source SSID
  sourceSSID = srcSSID;

  // check source callsign length (6 characters max)
  if(strlen(srcCallsign) > RADIOLIB_AX25_MAX_CALLSIGN_LEN) {
    return(RADIOLIB_ERR_INVALID_CALLSIGN);
  }

  // copy callsign
  memcpy(sourceCallsign, srcCallsign, strlen(srcCallsign));
  sourceCallsign[strlen(srcCallsign)] = '\0';

  // save preamble length
  preambleLen = preLen;

  // configure for direct mode
  #if !RADIOLIB_EXCLUDE_AFSK
  if(bellModem != nullptr) {
    return(phyLayer->startDirect());
  }
  #endif
  return(RADIOLIB_ERR_NONE);
}

#if defined(RADIOLIB_BUILD_ARDUINO)
int16_t AX25Client::transmit(String& str, const char* destCallsign, uint8_t destSSID) {
  return(transmit(str.c_str(), destCallsign, destSSID));
}
#endif

int16_t AX25Client::transmit(const char* str, const char* destCallsign, uint8_t destSSID) {
  // create control field
  uint8_t controlField = RADIOLIB_AX25_CONTROL_U_UNNUMBERED_INFORMATION | RADIOLIB_AX25_CONTROL_POLL_FINAL_DISABLED | RADIOLIB_AX25_CONTROL_UNNUMBERED_FRAME;

  // build the frame
  AX25Frame frame(destCallsign, destSSID, sourceCallsign, sourceSSID, controlField, RADIOLIB_AX25_PID_NO_LAYER_3, (uint8_t*)str, strlen(str));

  // send Unnumbered Information frame
  return(sendFrame(&frame));
}

int16_t AX25Client::sendFrame(AX25Frame* frame) {
  // check destination callsign length (6 characters max)
  if(strlen(frame->destCallsign) > RADIOLIB_AX25_MAX_CALLSIGN_LEN) {
    return(RADIOLIB_ERR_INVALID_CALLSIGN);
  }

  // check repeater configuration
  #if !RADIOLIB_STATIC_ONLY
    if(!(((frame->repeaterCallsigns == NULL) && (frame->repeaterSSIDs == NULL) && (frame->numRepeaters == 0)) ||
         ((frame->repeaterCallsigns != NULL) && (frame->repeaterSSIDs != NULL) && (frame->numRepeaters != 0)))) {
      return(RADIOLIB_ERR_INVALID_NUM_REPEATERS);
    }
    for(uint16_t i = 0; i < frame->numRepeaters; i++) {
      if(strlen(frame->repeaterCallsigns[i]) > RADIOLIB_AX25_MAX_CALLSIGN_LEN) {
        return(RADIOLIB_ERR_INVALID_REPEATER_CALLSIGN);
      }
    }
  #endif

  // calculate frame length without FCS (destination address, source address, repeater addresses, control, PID, info)
  size_t frameBuffLen = ((2 + frame->numRepeaters)*(RADIOLIB_AX25_MAX_CALLSIGN_LEN + 1)) + 1 + 1 + frame->infoLen;
  // create frame buffer without preamble, start or stop flags
  #if !RADIOLIB_STATIC_ONLY
    uint8_t* frameBuff = new uint8_t[frameBuffLen + 2];
  #else
    uint8_t frameBuff[RADIOLIB_STATIC_ARRAY_SIZE];
  #endif
  uint8_t* frameBuffPtr = frameBuff;

  // set destination callsign - all address field bytes are shifted by one bit to make room for HDLC address extension bit
  memset(frameBuffPtr, ' ' << 1, RADIOLIB_AX25_MAX_CALLSIGN_LEN);
  for(size_t i = 0; i < strlen(frame->destCallsign); i++) {
    *(frameBuffPtr + i) = frame->destCallsign[i] << 1;
  }
  frameBuffPtr += RADIOLIB_AX25_MAX_CALLSIGN_LEN;

  // set destination SSID
  *(frameBuffPtr++) = RADIOLIB_AX25_SSID_RESPONSE_DEST | RADIOLIB_AX25_SSID_RESERVED_BITS | (frame->destSSID & 0x0F) << 1 | RADIOLIB_AX25_SSID_HDLC_EXTENSION_CONTINUE;

  // set source callsign - all address field bytes are shifted by one bit to make room for HDLC address extension bit
  memset(frameBuffPtr, ' ' << 1, RADIOLIB_AX25_MAX_CALLSIGN_LEN);
  for(size_t i = 0; i < strlen(frame->srcCallsign); i++) {
    *(frameBuffPtr + i) = frame->srcCallsign[i] << 1;
  }
  frameBuffPtr += RADIOLIB_AX25_MAX_CALLSIGN_LEN;

  // set source SSID
  *(frameBuffPtr++) = RADIOLIB_AX25_SSID_COMMAND_SOURCE | RADIOLIB_AX25_SSID_RESERVED_BITS | (frame->srcSSID & 0x0F) << 1 | RADIOLIB_AX25_SSID_HDLC_EXTENSION_CONTINUE;

  // set repeater callsigns
  for(uint16_t i = 0; i < frame->numRepeaters; i++) {
    memset(frameBuffPtr, ' ' << 1, RADIOLIB_AX25_MAX_CALLSIGN_LEN);
    for(size_t j = 0; j < strlen(frame->repeaterCallsigns[i]); j++) {
      *(frameBuffPtr + j) = frame->repeaterCallsigns[i][j] << 1;
    }
    frameBuffPtr += RADIOLIB_AX25_MAX_CALLSIGN_LEN;
    *(frameBuffPtr++) = RADIOLIB_AX25_SSID_HAS_NOT_BEEN_REPEATED | RADIOLIB_AX25_SSID_RESERVED_BITS | (frame->repeaterSSIDs[i] & 0x0F) << 1 | RADIOLIB_AX25_SSID_HDLC_EXTENSION_CONTINUE;
  }

  // set HDLC extension end bit
  *(frameBuffPtr - 1) |= RADIOLIB_AX25_SSID_HDLC_EXTENSION_END;

  // set sequence numbers of the frames that have it
  uint8_t controlField = frame->control;
  if((frame->control & 0x01) == 0) {
    // information frame, set both sequence numbers
    controlField |= frame->rcvSeqNumber << 5;
    controlField |= frame->sendSeqNumber << 1;
  } else if((frame->control & 0x02) == 0) {
    // supervisory frame, set only receive sequence number
    controlField |= frame->rcvSeqNumber << 5;
  }

  // set control field
  *(frameBuffPtr++) = controlField;

  // set PID field of the frames that have it
  if(frame->protocolID != 0x00) {
    *(frameBuffPtr++) = frame->protocolID;
  }

  // set info field of the frames that have it
  if(frame->infoLen > 0) {
    memcpy(frameBuffPtr, frame->info, frame->infoLen);
    frameBuffPtr += frame->infoLen;
  }

  // flip bit order
  for(size_t i = 0; i < frameBuffLen; i++) {
    frameBuff[i] = Module::reflect(frameBuff[i], 8);
  }

  // calculate
  RadioLibCRCInstance.size = 16;
  RadioLibCRCInstance.poly = RADIOLIB_CRC_CCITT_POLY;
  RadioLibCRCInstance.init = RADIOLIB_CRC_CCITT_INIT;
  RadioLibCRCInstance.out = RADIOLIB_CRC_CCITT_OUT;
  RadioLibCRCInstance.refIn = false;
  RadioLibCRCInstance.refOut = false;
  uint16_t fcs = RadioLibCRCInstance.checksum(frameBuff, frameBuffLen);
  *(frameBuffPtr++) = (uint8_t)((fcs >> 8) & 0xFF);
  *(frameBuffPtr++) = (uint8_t)(fcs & 0xFF);

  // prepare buffer for the final frame (stuffed, with added preamble + flags and NRZI-encoded)
  #if !RADIOLIB_STATIC_ONLY
    // worst-case scenario: sequence of 1s, will have 120% of the original length, stuffed frame also includes both flags
    uint8_t* stuffedFrameBuff = new uint8_t[preambleLen + 1 + (6*frameBuffLen)/5 + 2];
  #else
    uint8_t stuffedFrameBuff[RADIOLIB_STATIC_ARRAY_SIZE];
  #endif

  // initialize buffer to all zeros
  memset(stuffedFrameBuff, 0x00, preambleLen + 1 + (6*frameBuffLen)/5 + 2);

  // stuff bits (skip preamble and both flags)
  uint16_t stuffedFrameBuffLenBits = 8*(preambleLen + 1);
  uint8_t count = 0;
  for(size_t i = 0; i < frameBuffLen + 2; i++) {
    for(int8_t shift = 7; shift >= 0; shift--) {
      uint16_t stuffedFrameBuffPos = stuffedFrameBuffLenBits + 7 - 2*(stuffedFrameBuffLenBits%8);
      if((frameBuff[i] >> shift) & 0x01) {
        // copy 1 and increment counter
        SET_BIT_IN_ARRAY(stuffedFrameBuff, stuffedFrameBuffPos);
        stuffedFrameBuffLenBits++;
        count++;

        // check 5 consecutive 1s
        if(count == 5) {
          // get the new position in stuffed frame
          stuffedFrameBuffPos = stuffedFrameBuffLenBits + 7 - 2*(stuffedFrameBuffLenBits%8);

          // insert 0 and reset counter
          CLEAR_BIT_IN_ARRAY(stuffedFrameBuff, stuffedFrameBuffPos);
          stuffedFrameBuffLenBits++;
          count = 0;
        }

      } else {
        // copy 0 and reset counter
        CLEAR_BIT_IN_ARRAY(stuffedFrameBuff, stuffedFrameBuffPos);
        stuffedFrameBuffLenBits++;
        count = 0;
      }

    }
  }

  // deallocate memory
  #if !RADIOLIB_STATIC_ONLY
    delete[] frameBuff;
  #endif

  // set preamble bytes and start flag field
  for(uint16_t i = 0; i < preambleLen + 1; i++) {
    stuffedFrameBuff[i] = RADIOLIB_AX25_FLAG;
  }

  // get stuffed frame length in bytes
  size_t stuffedFrameBuffLen = stuffedFrameBuffLenBits/8 + 1;
  uint8_t trailingLen = stuffedFrameBuffLenBits % 8;

  // set end flag field (may be split into two bytes due to misalignment caused by extra stuffing bits)
  if(trailingLen != 0) {
    stuffedFrameBuffLen++;
    stuffedFrameBuff[stuffedFrameBuffLen - 2] |= RADIOLIB_AX25_FLAG >> trailingLen;
    stuffedFrameBuff[stuffedFrameBuffLen - 1] = RADIOLIB_AX25_FLAG << (8 - trailingLen);
  } else {
    stuffedFrameBuff[stuffedFrameBuffLen - 1] = RADIOLIB_AX25_FLAG;
  }

  // convert to NRZI
  for(size_t i = preambleLen + 1; i < stuffedFrameBuffLen*8; i++) {
    size_t currBitPos = i + 7 - 2*(i%8);
    size_t prevBitPos = (i - 1) + 7 - 2*((i - 1)%8);
    if(TEST_BIT_IN_ARRAY(stuffedFrameBuff, currBitPos)) {
      // bit is 1, no change, copy previous bit
      if(TEST_BIT_IN_ARRAY(stuffedFrameBuff, prevBitPos)) {
        SET_BIT_IN_ARRAY(stuffedFrameBuff, currBitPos);
      } else {
        CLEAR_BIT_IN_ARRAY(stuffedFrameBuff, currBitPos);
      }

    } else {
      // bit is 0, transition, copy inversion of the previous bit
      if(TEST_BIT_IN_ARRAY(stuffedFrameBuff, prevBitPos)) {
        CLEAR_BIT_IN_ARRAY(stuffedFrameBuff, currBitPos);
      } else {
        SET_BIT_IN_ARRAY(stuffedFrameBuff, currBitPos);
      }
    }
  }

  // transmit
  int16_t state = RADIOLIB_ERR_NONE;
  #if !RADIOLIB_EXCLUDE_AFSK
  if(bellModem != nullptr) {
    bellModem->idle();

    // iterate over all bytes in the buffer
    for(uint32_t i = 0; i < stuffedFrameBuffLen; i++) {
      bellModem->write(stuffedFrameBuff[i]);
    }

    bellModem->standby();

  } else {
  #endif
    state = phyLayer->transmit(stuffedFrameBuff, stuffedFrameBuffLen);
  #if !RADIOLIB_EXCLUDE_AFSK
  }
  #endif

  // deallocate memory
  #if !RADIOLIB_STATIC_ONLY
    delete[] stuffedFrameBuff;
  #endif

  return(state);
}

void AX25Client::getCallsign(char* buff) {
  strncpy(buff, sourceCallsign, RADIOLIB_AX25_MAX_CALLSIGN_LEN + 1);
}

uint8_t AX25Client::getSSID() {
  return(sourceSSID);
}

#endif
