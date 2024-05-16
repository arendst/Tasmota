#include "Pager.h"
#include <string.h>
#include <math.h>
#if defined(ESP_PLATFORM)
#include "esp_attr.h"
#endif

#if !RADIOLIB_EXCLUDE_PAGER

#if !RADIOLIB_EXCLUDE_DIRECT_RECEIVE
// this is a massive hack, but we need a global-scope ISR to manage the bit reading
// let's hope nobody ever tries running two POCSAG receivers at the same time
static PhysicalLayer* readBitInstance = NULL;
static uint32_t readBitPin = RADIOLIB_NC;

#if defined(ESP8266) || defined(ESP32)
  IRAM_ATTR
#endif
static void PagerClientReadBit(void) {
  if(readBitInstance) {
    readBitInstance->readBit(readBitPin);
  }
}
#endif

PagerClient::PagerClient(PhysicalLayer* phy) {
  phyLayer = phy;
  #if !RADIOLIB_EXCLUDE_DIRECT_RECEIVE
  readBitInstance = phyLayer;
  #endif
  filterNumAddresses = 0;
  filterAddresses = NULL;
  filterMasks = NULL;
}

int16_t PagerClient::begin(float base, uint16_t speed, bool invert, uint16_t shift) {
  // calculate duration of 1 bit in us
  dataRate = (float)speed/1000.0f;
  bitDuration = (uint32_t)1000000/speed;

  // calculate 24-bit frequency
  baseFreq = base;
  baseFreqRaw = (baseFreq * 1000000.0) / phyLayer->getFreqStep();

  // calculate module carrier frequency resolution
  uint16_t step = round(phyLayer->getFreqStep());

  // calculate raw frequency shift
  shiftFreqHz = shift;
  shiftFreq = shiftFreqHz/step;
  inv = invert;

  // initialize BCH encoder
  RadioLibBCHInstance.begin(RADIOLIB_PAGER_BCH_N, RADIOLIB_PAGER_BCH_K, RADIOLIB_PAGER_BCH_PRIMITIVE_POLY);

  // configure for direct mode
  return(phyLayer->startDirect());
}

int16_t PagerClient::sendTone(uint32_t addr) {
  return(PagerClient::transmit(NULL, 0, addr));
}

#if defined(RADIOLIB_BUILD_ARDUINO)
int16_t PagerClient::transmit(String& str, uint32_t addr, uint8_t encoding, uint8_t function) {
  return(PagerClient::transmit(str.c_str(), addr, encoding, function));
}
#endif

int16_t PagerClient::transmit(const char* str, uint32_t addr, uint8_t encoding, uint8_t function) {
  return(PagerClient::transmit((uint8_t*)str, strlen(str), addr, encoding, function));
}

int16_t PagerClient::transmit(uint8_t* data, size_t len, uint32_t addr, uint8_t encoding, uint8_t function) {
  if(addr > RADIOLIB_PAGER_ADDRESS_MAX) {
    return(RADIOLIB_ERR_INVALID_ADDRESS_WIDTH);
  }

  if(((data == NULL) && (len > 0)) || ((data != NULL) && (len == 0))) {
    return(RADIOLIB_ERR_INVALID_PAYLOAD);
  }

  // get symbol bit length based on encoding
  uint8_t symbolLength = 0;
  if(encoding == RADIOLIB_PAGER_BCD) {
    symbolLength = 4;

  } else if(encoding == RADIOLIB_PAGER_ASCII) {
    symbolLength = 7;

  } else {
    return(RADIOLIB_ERR_INVALID_ENCODING);

  }

  // Automatically set function bits based on given encoding
  if (function == RADIOLIB_PAGER_FUNC_AUTO) {
    if(encoding == RADIOLIB_PAGER_BCD) {
      function = RADIOLIB_PAGER_FUNC_BITS_NUMERIC;

    } else if(encoding == RADIOLIB_PAGER_ASCII) {
      function = RADIOLIB_PAGER_FUNC_BITS_ALPHA;

    } else {
    return(RADIOLIB_ERR_INVALID_ENCODING);

    }
    if(len == 0) {
      function = RADIOLIB_PAGER_FUNC_BITS_TONE;
    }
  }
  if (function > RADIOLIB_PAGER_FUNC_BITS_ALPHA) {
    return(RADIOLIB_ERR_INVALID_FUNCTION);
  }

  // get target position in batch (3 LSB from address determine frame position in batch)
  uint8_t framePos = 2*(addr & 0x07);

  // get address that will be written into address frame
  uint32_t frameAddr = ((addr >> 3) << RADIOLIB_PAGER_ADDRESS_POS) | (function << RADIOLIB_PAGER_FUNC_BITS_POS);

  // calculate the number of 20-bit data blocks
  size_t numDataBlocks = (len * symbolLength) / RADIOLIB_PAGER_MESSAGE_BITS_LENGTH;
  if((len * symbolLength) % RADIOLIB_PAGER_MESSAGE_BITS_LENGTH > 0) {
    numDataBlocks += 1;
  }

  // calculate number of batches
  size_t numBatches = (1 + framePos + numDataBlocks) / RADIOLIB_PAGER_BATCH_LEN + 1;
  if((1 + numDataBlocks) % RADIOLIB_PAGER_BATCH_LEN == 0) {
    numBatches -= 1;
  }

  // calculate message length in 32-bit code words
  size_t msgLen = RADIOLIB_PAGER_PREAMBLE_LENGTH + (1 + RADIOLIB_PAGER_BATCH_LEN) * numBatches;

  #if RADIOLIB_STATIC_ONLY
    uint32_t msg[RADIOLIB_STATIC_ARRAY_SIZE];
  #else
    uint32_t* msg = new uint32_t[msgLen];
  #endif

  // build the message
  memset(msg, 0x00, msgLen*sizeof(uint32_t));

  // set preamble
  for(size_t i = 0; i < RADIOLIB_PAGER_PREAMBLE_LENGTH; i++) {
    msg[i] = RADIOLIB_PAGER_PREAMBLE_CODE_WORD;
  }

  // start by setting everything after preamble to idle
  for(size_t i = RADIOLIB_PAGER_PREAMBLE_LENGTH; i < msgLen; i++) {
    msg[i] = RADIOLIB_PAGER_IDLE_CODE_WORD;
  }

  // set frame synchronization code words
  for(size_t i = 0; i < numBatches; i++) {
    msg[RADIOLIB_PAGER_PREAMBLE_LENGTH + i*(1 + RADIOLIB_PAGER_BATCH_LEN)] = RADIOLIB_PAGER_FRAME_SYNC_CODE_WORD;
  }

  // write address code word
  msg[RADIOLIB_PAGER_PREAMBLE_LENGTH + 1 + framePos] = RadioLibBCHInstance.encode(frameAddr);

  // write the data as 20-bit code blocks
  if(len > 0) {
    int8_t remBits = 0;
    uint8_t dataPos = 0;
    for(size_t i = 0; i < numDataBlocks + numBatches - 1; i++) {
      uint8_t blockPos = RADIOLIB_PAGER_PREAMBLE_LENGTH + 1 + framePos + 1 + i;

      // check if we need to skip a frame sync marker
      if(((blockPos - (RADIOLIB_PAGER_PREAMBLE_LENGTH + 1)) % RADIOLIB_PAGER_BATCH_LEN) == 0) {
        blockPos++;
        i++;
      }

      // mark this as a message code word
      msg[blockPos] = RADIOLIB_PAGER_MESSAGE_CODE_WORD << (RADIOLIB_PAGER_CODE_WORD_LEN - 1);

      // first insert the remainder from previous code word (if any)
      if(remBits > 0) {
        // this doesn't apply to BCD messages, so no need to check that here
        uint8_t prev = Module::reflect(data[dataPos - 1], 8);
        prev >>= 1;
        msg[blockPos] |= (uint32_t)prev << (RADIOLIB_PAGER_CODE_WORD_LEN - 1 - remBits);
      }

      // set all message symbols until we overflow to the next code word or run out of message symbols
      int8_t symbolPos = RADIOLIB_PAGER_CODE_WORD_LEN - 1 - symbolLength - remBits;
      while(symbolPos > (RADIOLIB_PAGER_FUNC_BITS_POS - symbolLength)) {

        // for BCD, encode the symbol
        uint8_t symbol = data[dataPos++];
        if(encoding == RADIOLIB_PAGER_BCD) {
          symbol = encodeBCD(symbol);
        }
        symbol = Module::reflect(symbol, 8);
        symbol >>= (8 - symbolLength);

        // insert the next message symbol
        msg[blockPos] |= (uint32_t)symbol << symbolPos;
        symbolPos -= symbolLength;

        // check if we ran out of message symbols
        if(dataPos >= len) {
          // in BCD mode, pad the rest of the code word with spaces (0xC)
          if(encoding == RADIOLIB_PAGER_BCD) {
            uint8_t numSteps = (symbolPos - RADIOLIB_PAGER_FUNC_BITS_POS + symbolLength)/symbolLength;
            for(uint8_t i = 0; i < numSteps; i++) {
              symbol = encodeBCD(' ');
              symbol = Module::reflect(symbol, 8);
              symbol >>= (8 - symbolLength);
              msg[blockPos] |= (uint32_t)symbol << symbolPos;
              symbolPos -= symbolLength;
            }
          }
          break;
        }
      }

      // ensure the parity bits are not set due to overflow
      msg[blockPos] &= ~(RADIOLIB_PAGER_BCH_BITS_MASK);

      // save the number of overflown bits
      remBits = RADIOLIB_PAGER_FUNC_BITS_POS - symbolPos - symbolLength;

      // do the FEC
      msg[blockPos] = RadioLibBCHInstance.encode(msg[blockPos]);
    }
  }

  // transmit the message
  PagerClient::write(msg, msgLen);

  #if !RADIOLIB_STATIC_ONLY
    delete[] msg;
  #endif

  // turn transmitter off
  phyLayer->standby();

  return(RADIOLIB_ERR_NONE);
}

#if !RADIOLIB_EXCLUDE_DIRECT_RECEIVE
int16_t PagerClient::startReceive(uint32_t pin, uint32_t addr, uint32_t mask) {
  // save the variables
  readBitPin = pin;
  filterAddr = addr;
  filterMask = mask;
  filterAddresses = NULL;
  filterMasks = NULL;
  filterNumAddresses = 0;
  return(startReceiveCommon());
}

int16_t PagerClient::startReceive(uint32_t pin, uint32_t *addrs, uint32_t *masks, size_t numAddresses) {
  // save the variables
  readBitPin = pin;
  filterAddr = 0;
  filterMask = 0;
  filterAddresses = addrs;
  filterMasks = masks;
  filterNumAddresses = numAddresses;
  return(startReceiveCommon());
}

int16_t PagerClient::startReceiveCommon() {
  // set the carrier frequency
  int16_t state = phyLayer->setFrequency(baseFreq);
  RADIOLIB_ASSERT(state);

  // set bitrate
  state = phyLayer->setBitRate(dataRate);
  RADIOLIB_ASSERT(state);

  // set frequency deviation to 4.5 khz
  state = phyLayer->setFrequencyDeviation((float)shiftFreqHz / 1000.0f);
  RADIOLIB_ASSERT(state);

  // now set up the direct mode reception
  Module* mod = phyLayer->getMod();
  mod->hal->pinMode(readBitPin, mod->hal->GpioModeInput);

  // set direct sync word to the frame sync word
  // the logic here is inverted, because modules like SX1278
  // assume high frequency to be logic 1, which is opposite to POCSAG
  if(!inv) {
    phyLayer->setDirectSyncWord(~RADIOLIB_PAGER_FRAME_SYNC_CODE_WORD, 32);
  } else {
    phyLayer->setDirectSyncWord(RADIOLIB_PAGER_FRAME_SYNC_CODE_WORD, 32);
  }

  phyLayer->setDirectAction(PagerClientReadBit);
  phyLayer->receiveDirect();

  return(state);
}

size_t PagerClient::available() {
  return(phyLayer->available() + sizeof(uint32_t))/(sizeof(uint32_t) * (RADIOLIB_PAGER_BATCH_LEN + 1));
}

#if defined(RADIOLIB_BUILD_ARDUINO)
int16_t PagerClient::readData(String& str, size_t len, uint32_t* addr) {
  int16_t state = RADIOLIB_ERR_NONE;

  // determine the message length, based on user input or the amount of received data
  size_t length = len;
  if(length == 0) {
    // one batch can contain at most 80 message symbols
    length = available()*80;
  }

  // build a temporary buffer
  #if RADIOLIB_STATIC_ONLY
    uint8_t data[RADIOLIB_STATIC_ARRAY_SIZE + 1];
  #else
    uint8_t* data = new uint8_t[length + 1];
    if(!data) {
      return(RADIOLIB_ERR_MEMORY_ALLOCATION_FAILED);
    }
  #endif

  // read the received data
  state = readData(data, &length, addr);

  if(state == RADIOLIB_ERR_NONE) {
    // check tone-only tramsissions
    if(length == 0) {
      length = 6;
      strncpy((char*)data, "<tone>", length + 1);
    }

    // add null terminator
    data[length] = 0;

    // initialize Arduino String class
    str = String((char*)data);
  }

  // deallocate temporary buffer
  #if !RADIOLIB_STATIC_ONLY
    delete[] data;
  #endif

  return(state);
}
#endif

int16_t PagerClient::readData(uint8_t* data, size_t* len, uint32_t* addr) {
  // find the correct address
  bool match = false;
  uint8_t framePos = 0;
  uint8_t symbolLength = 0;
  while(!match && phyLayer->available()) {
    uint32_t cw = read();
    framePos++;

    // check if it's the idle code word
    if(cw == RADIOLIB_PAGER_IDLE_CODE_WORD) {
      continue;
    }

    // check if it's the sync word
    if(cw == RADIOLIB_PAGER_FRAME_SYNC_CODE_WORD) {
      framePos = 0;
      continue;
    }

    // not an idle code word, check if it's an address word
    if(cw & (RADIOLIB_PAGER_MESSAGE_CODE_WORD << (RADIOLIB_PAGER_CODE_WORD_LEN - 1))) {
      // this is pretty weird, it seems to be a message code word without address
      continue;
    }

    // should be an address code word, extract the address
    uint32_t addr_found = ((cw & RADIOLIB_PAGER_ADDRESS_BITS_MASK) >> (RADIOLIB_PAGER_ADDRESS_POS - 3)) | (framePos/2);
    if (addressMatched(addr_found)) {
      match = true;
      if(addr) {
        *addr = addr_found;
      }

      // determine the encoding from the function bits
      if((cw & RADIOLIB_PAGER_FUNCTION_BITS_MASK) >> RADIOLIB_PAGER_FUNC_BITS_POS == RADIOLIB_PAGER_FUNC_BITS_NUMERIC) {
        symbolLength = 4;
      } else {
        symbolLength = 7;
      }
    }
  }

  if(!match) {
    // address not found
    return(RADIOLIB_ERR_ADDRESS_NOT_FOUND);
  }

  // we have the address, start pulling out the message
  bool complete = false;
  size_t decodedBytes = 0;
  uint32_t prevCw = 0;
  bool overflow = false;
  int8_t ovfBits = 0;
  while(!complete && phyLayer->available()) {
    uint32_t cw = read();

    // check if it's the idle code word
    if(cw == RADIOLIB_PAGER_IDLE_CODE_WORD) {
      complete = true;
      break;
    }

    // skip the sync words
    if(cw == RADIOLIB_PAGER_FRAME_SYNC_CODE_WORD) {
      continue;
    }

    // check overflow from previous code word
    uint8_t bitPos = RADIOLIB_PAGER_CODE_WORD_LEN - 1 - symbolLength;
    if(overflow) {
      overflow = false;

      // this is a bit convoluted - first, build masks for both previous and current code word
      uint8_t currPos = RADIOLIB_PAGER_CODE_WORD_LEN - 1 - symbolLength + ovfBits;
      uint8_t prevPos = RADIOLIB_PAGER_MESSAGE_END_POS;
      uint32_t prevMask = (0x7FUL << prevPos) & ~((uint32_t)0x7FUL << (RADIOLIB_PAGER_MESSAGE_END_POS + ovfBits));
      uint32_t currMask = (0x7FUL << currPos) & ~((uint32_t)1 << (RADIOLIB_PAGER_CODE_WORD_LEN - 1));

      // next, get the two parts of the message symbol and stick them together
      uint8_t prevSymbol = (prevCw & prevMask) >> prevPos;
      uint8_t currSymbol = (cw & currMask) >> currPos;
      uint32_t symbol = prevSymbol << (symbolLength - ovfBits) | currSymbol;

      // finally, we can flip the bits
      symbol = Module::reflect((uint8_t)symbol, 8);
      symbol >>= (8 - symbolLength);

      // decode BCD and we're done
      if(symbolLength == 4) {
        symbol = decodeBCD(symbol);
      }
      data[decodedBytes++] = symbol;

      // adjust the bit position of the next message symbol
      bitPos += ovfBits;
      bitPos -= symbolLength;
    }

    // get the message symbols based on the encoding type
    while(bitPos >= RADIOLIB_PAGER_MESSAGE_END_POS) {
      // get the message symbol from the code word and reverse bits
      uint32_t symbol = (cw & (0x7FUL << bitPos)) >> bitPos;
      symbol = Module::reflect((uint8_t)symbol, 8);
      symbol >>= (8 - symbolLength);

      // decode BCD if needed
      if(symbolLength == 4) {
        symbol = decodeBCD(symbol);
      }
      data[decodedBytes++] = symbol;

      // now calculate if the next symbol is overflowing to the following code word
      int8_t remBits = bitPos - RADIOLIB_PAGER_MESSAGE_END_POS;
      if(remBits < symbolLength) {
        // overflow!
        prevCw = cw;
        overflow = true;
        ovfBits = remBits;
      }
      bitPos -= symbolLength;
    }

  }

  // save the number of decoded bytes
  *len = decodedBytes;
  return(RADIOLIB_ERR_NONE);
}
#endif

bool PagerClient::addressMatched(uint32_t addr) {
  // check whether to match single or multiple addresses/masks
  if(filterNumAddresses == 0) {
    return((addr & filterMask) == (filterAddr & filterMask));
  }
  
  // multiple addresses, check there are some to match
  if((filterAddresses == NULL) || (filterMasks == NULL)) {
    return(false);
  }

  for(size_t i = 0; i < filterNumAddresses; i++) {
    if((filterAddresses[i] & filterMasks[i]) == (addr & filterMasks[i])) {
      return(true);
    }
  }

  return(false);
}

void PagerClient::write(uint32_t* data, size_t len) {
  // write code words from buffer
  for(size_t i = 0; i < len; i++) {
    PagerClient::write(data[i]);
  }
}

void PagerClient::write(uint32_t codeWord) {
  // write single code word
  Module* mod = phyLayer->getMod();
  for(int8_t i = 31; i >= 0; i--) {
    uint32_t mask = (uint32_t)0x01 << i;
    uint32_t start = mod->hal->micros();

    // figure out the shift direction - start by assuming the bit is 0
    int16_t change = shiftFreq;

    // now check if it's actually 1
    if(codeWord & mask) {
      change = -shiftFreq;
    }

    // finally, check if inversion is enabled
    if(inv) {
      change = -change;
    }

    // now transmit the shifted frequency
    phyLayer->transmitDirect(baseFreqRaw + change);

    // this is pretty silly, while(mod->hal->micros() ... ) would be enough
    // but for some reason, MegaCore throws a linker error on it
    // "relocation truncated to fit: R_AVR_7_PCREL against `no symbol'"
    uint32_t now = mod->hal->micros();
    while(now - start < bitDuration) {
      now = mod->hal->micros();
    }
  }
}

#if !RADIOLIB_EXCLUDE_DIRECT_RECEIVE
uint32_t PagerClient::read() {
  uint32_t codeWord = 0;
  codeWord |= (uint32_t)phyLayer->read() << 24;
  codeWord |= (uint32_t)phyLayer->read() << 16;
  codeWord |= (uint32_t)phyLayer->read() << 8;
  codeWord |= (uint32_t)phyLayer->read();

  // check if we need to invert bits
  // the logic here is inverted, because modules like SX1278
  // assume high frequency to be logic 1, which is opposite to POCSAG
  if(!inv) {
    codeWord = ~codeWord;
  }

  RADIOLIB_DEBUG_PROTOCOL_PRINTLN("R\t%lX", codeWord);
  // TODO BCH error correction here
  return(codeWord);
}
#endif

uint8_t PagerClient::encodeBCD(char c) {
  switch(c) {
    case '*':
      return(0x0A);
    case 'U':
      return(0x0B);
    case ' ':
      return(0x0C);
    case '-':
      return(0x0D);
    case ')':
      return(0x0E);
    case '(':
      return(0x0F);
  }
  return(c - '0');
}

char PagerClient::decodeBCD(uint8_t b) {
  switch(b) {
    case 0x0A:
      return('*');
    case 0x0B:
      return('U');
    case 0x0C:
      return(' ');
    case 0x0D:
      return('-');
    case 0x0E:
      return(')');
    case 0x0F:
      return('(');
  }
  return(b + '0');
}

#endif
