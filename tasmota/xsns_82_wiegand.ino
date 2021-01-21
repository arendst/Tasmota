/*
  xsns_82_wiegand.ino - Support for Wiegand Interface 125kHz NFC Tag Reader for Tasmota

  Copyright (C) 2021  Sigurd Leuther and Theo Arends

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

#ifdef USE_WIEGAND
/*********************************************************************************************\
 * Wiegand 24, 26, 32, 34 bit Rfid reader 125 kHz
 *
 * Wire connections:
 * Red     Vdc
 * Black   Gnd
 * Green   D0
 * White   D1
 * Yellow  Buzzer
 * Blue    Led
 * Grey    34-bit if connected to Gnd
 *
 * MQTT:
 * %prefix%/%topic%/SENSOR = {"Time":"2021-01-13T12:30:38","Wiegand":{"UID":"rfid tag"}}
\*********************************************************************************************/
#warning **** Wiegand interface enabled ****

#define XSNS_82                82

#define WIEGAND_BIT_TIMEOUT    25 //time to be wait after last bit detected.

// Use only a randomly generate RFID for testing. using #define will save some space in the final code
//  DEV_WIEGAND_TEST_MODE 1 : testing with random rfid without hardware connected, but GPIOs set correctly
//  DEV_WIEGAND_TEST_MODE 2 : testing with hardware corretly connected.
#define DEV_WIEGAND_TEST_MODE  0

#ifdef DEV_WIEGAND_TEST_MODE
  #if (DEV_WIEGAND_TEST_MODE==0)
  #elif (DEV_WIEGAND_TEST_MODE==1)
    #warning "Wiegand Interface compiled with 'DEV_WIEGAND_TEST_MODE' 1 (Random RFID)"
  #elif (DEV_WIEGAND_TEST_MODE==2)
    #warning "Wiegand Interface compiled with 'DEV_WIEGAND_TEST_MODE' 2 (Hardware connected)"
  #else
    #warning "Wiegand Interface compiled with unknown mode"
  #endif
#endif

class Wiegand {
  public:

    Wiegand(void);
    void Init(void);
    void ScanForTag(void);
#ifdef USE_WEBSERVER
    void Show(void);
#endif

  private:

    uint64_t HexStringToDec(uint64_t);
    uint64_t CheckAndConvertRfid(uint64_t,uint16_t);
    char translateEnterEscapeKeyPress(char);
    uint8_t CalculateParities(uint64_t, int);
    bool WiegandConversion (void);
    static void handleD0Interrupt(void);
    static void handleD1Interrupt(void);

    uint64_t rfid;
    uint8_t tagSize;

    static volatile uint64_t rfidBuffer;
    static volatile uint16_t bitCount;
    static volatile uint32_t lastFoundTime;
    static volatile uint8_t timeOut;
    bool isInit = false;

#if (DEV_WIEGAND_TEST_MODE)==1
    uint64_t GetRandomRfid(uint8_t);
#endif
};

Wiegand* oWiegand = new Wiegand();
uint8_t scanDelay;

volatile uint64_t Wiegand::rfidBuffer;
volatile uint16_t Wiegand::bitCount;
volatile uint32_t Wiegand::lastFoundTime;
volatile uint8_t Wiegand::timeOut;

Wiegand::Wiegand()   {
  rfid = 0;
  lastFoundTime = 0;
  tagSize = 0;
  rfidBuffer = 0;
  bitCount = 0 ;
  timeOut = 0;
  isInit = false;
}

#if (DEV_WIEGAND_TEST_MODE)==1
uint64_t Wiegand::GetRandomRfid(uint8_t tag_size=34) {
  // Todo add support for 4 and 8 bit keyboard "tags"
  uint64_t result = (uint32_t)HwRandom();
  uint8_t parities = 0;
  bitCount = tag_size;
  timeOut = millis() - WIEGAND_BIT_TIMEOUT;
  result = result << 32;
  result += HwRandom();

  switch (tag_size){
    case 24:
      result = (result & 0x7FFFFE) >>1;
    break;
    case 26:
      result = (result & 0x1FFFFFE) >>1;
    break;
    case 32:
      result = (result & 0x7FFFFFFE) >>1;
    break;
    case 34:
      result = (result & 0x3FFFFFFFE) >>1;
    break;
    default:
    break;
  }
  parities = CalculateParities(result, tag_size);

  result = (result << 1) | (parities & 0x01);  // Set LSB parity
  if (parities & 0x80) {  // MSB parity is 1
    switch (tag_size) {
      case 24:
        result |= 0x800000;
      break;
      case 26:
        result |= 0x2000000;
      break;
      case 32:
        result |= 0x80000000;
      break;
      case 34:
        result |= 0x400000000;
      break;
      default:
      break;
    }
  }

 return result;
}
#endif

void ICACHE_RAM_ATTR Wiegand::handleD1Interrupt() {  // Receive a 1 bit. (D0=high & D1=low)
  rfidBuffer = (rfidBuffer << 1) | 1;                // Leftshift + 1 bit
  bitCount++;                                        // Increment the counter
  lastFoundTime = millis();                          // Last time bit found
}

void ICACHE_RAM_ATTR Wiegand::handleD0Interrupt() {  // Receive a 0 bit. (D0=low & D1=high)
  rfidBuffer = rfidBuffer << 1;                      // Leftshift the 0 bit is now at the end of rfidBuffer
  bitCount++;                                        // Increment the counter
  lastFoundTime = millis();                          // Last time bit found
}

void Wiegand::Init() {
  isInit = false;
  if (PinUsed(GPIO_WIEGAND_D0) && PinUsed(GPIO_WIEGAND_D1)) {  // Only start, if the Wiegang pins are
#if (DEV_WIEGAND_TEST_MODE)>0
    AddLog_P(LOG_LEVEL_INFO, PSTR("WIE: Init()"));
#endif
    pinMode(Pin(GPIO_WIEGAND_D0), INPUT_PULLUP);
    pinMode(Pin(GPIO_WIEGAND_D1), INPUT_PULLUP);
    attachInterrupt(Pin(GPIO_WIEGAND_D0), handleD0Interrupt, FALLING);
    attachInterrupt(Pin(GPIO_WIEGAND_D1), handleD1Interrupt, FALLING);
    isInit = true;                                            // Helps to run only if correctly setup
#if (DEV_WIEGAND_TEST_MODE)>0
    AddLog_P(LOG_LEVEL_INFO, PSTR("WIE: Testmode"));          // For tests without reader attaiched
    AddLog_P(LOG_LEVEL_INFO, PSTR("WIE: D0:%u"),Pin(GPIO_WIEGAND_D0));
    AddLog_P(LOG_LEVEL_INFO, PSTR("WIE: D1:%u"),Pin(GPIO_WIEGAND_D1));
#else
    AddLog_P(LOG_LEVEL_INFO, PSTR("WIE: D0=%u, D1=%u"),Pin(GPIO_WIEGAND_D0), Pin(GPIO_WIEGAND_D1));
#endif
  }
#if (DEV_WIEGAND_TEST_MODE)>0
  else {
    AddLog_P(LOG_LEVEL_INFO, PSTR("WIE: no GPIOs."));
  }
#endif
}

uint64_t Wiegand::CheckAndConvertRfid(uint64_t rfidIn, uint16_t bitcount) {
  uint8_t evenParityBit = 0;
  uint8_t oddParityBit = (uint8_t) (rfidIn & 0x1);  // Last bit = odd parity
  uint8_t calcParity = 0;
  switch (bitcount) {
    case 24:
      evenParityBit = (rfidIn & 0x800000) ? 0x80 : 0;
      rfidIn = (rfidIn & 0x7FFFFE) >>1;
    break;

    case 26:
      evenParityBit = (rfidIn & 0x2000000) ? 0x80 : 0;
      rfidIn = (rfidIn & 0x1FFFFFE) >>1;
    break;

    case 32:
      evenParityBit = (rfidIn & 0x80000000) ? 0x80 : 0;
      rfidIn = (rfidIn & 0x7FFFFFFE) >>1;
    break;

    case 34:
      evenParityBit = (rfidIn & 0x400000000) ? 0x80 : 0;
      rfidIn = (rfidIn & 0x3FFFFFFFE) >>1;
    break;

    default:
    break;
  }
  calcParity = CalculateParities(rfidIn, bitCount);    // check result on http://www.ccdesignworks.com/wiegand_calc.htm with raw tag as input
  if (calcParity != (evenParityBit | oddParityBit)) {  // Paritybit is wrong
    rfidIn=0;
    AddLog_P(LOG_LEVEL_INFO, PSTR("WIE: %llu parity error"), rfidIn);
  }
#if (DEV_WIEGAND_TEST_MODE)>0
  AddLog_P(LOG_LEVEL_INFO, PSTR("WIE: even (left) parity: %u "), (evenParityBit>>7));
  AddLog_P(LOG_LEVEL_INFO, PSTR("WIE: even (calc) parity: %u "), (calcParity & 0x80)>>7);
  AddLog_P(LOG_LEVEL_INFO, PSTR("WIE: odd (right) parity: %u "), oddParityBit);
  AddLog_P(LOG_LEVEL_INFO, PSTR("WIE: odd (calc) parity: %u "), (calcParity & 0x01));
#endif
  return rfidIn;
}

uint8_t Wiegand::CalculateParities(uint64_t tagWithoutParities, int tag_size=26) {
  // tag_size is the size of the final tag including the 2 parity bits
  // So length if the tagWithoutParities should be (tag_size-2) !! That will be not profed and
  // lead to wrong results if the input value is larger!
  // Calculated start parity (even) will be returned as bit 8
  // calculated end parity (odd) will be returned as bit 1
  uint8_t retValue=0;
  tag_size -= 2;
  if (tag_size <= 0) { return retValue; }      // Prohibit div zero exception and other wrong inputs
  uint8_t parity = 1;                          // Check for odd parity on LSB
  for (uint8_t i = 0; i < (tag_size / 2); i++) {
    parity ^= (tagWithoutParities & 1);
    tagWithoutParities >>= 1;
  }
  retValue |= parity;

  parity = 0;                                  // Check for even parity on MSB
  while (tagWithoutParities) {
    parity ^= (tagWithoutParities & 1);
    tagWithoutParities >>= 1;
  }
  retValue |= (parity << 7);

  return retValue;
}

char Wiegand::translateEnterEscapeKeyPress(char oKeyPressed) {
	switch(oKeyPressed) {
	case 0x0b:        // 11 or * key
		return 0x0d;    // 13 or ASCII ENTER

	case 0x0a:        // 10 or # key
		return 0x1b;    // 27 or ASCII ESCAPE

	default:
		return oKeyPressed;
	}
}

bool Wiegand::WiegandConversion () {
  bool bRet = false;
	unsigned long nowTick = millis();
  // Add a maximum wait time for new bits
  unsigned long diffTicks = nowTick - lastFoundTime;
  if ((diffTicks > WIEGAND_BIT_TIMEOUT) && (diffTicks >= 5000 )) {  // Max. 5 secs between 2 bits comming in
    bitCount = 0;
    rfidBuffer = 0;
    lastFoundTime = nowTick;
    return bRet;
  }
  if (diffTicks > WIEGAND_BIT_TIMEOUT) {                            // Last bit found is WIEGAND_BIT_TIMEOUT ms ago
#if (DEV_WIEGAND_TEST_MODE)>0
    AddLog_P(LOG_LEVEL_INFO, PSTR("WIE: Raw tag %llu, Bit count %u"), rfidBuffer, bitCount);
#endif
    if ((24 == bitCount) || (26 == bitCount) || (32 == bitCount) || (34 == bitCount)) {
      // 24, 26, 32, 34-bit Wiegand codes
      rfid = CheckAndConvertRfid( rfidBuffer, bitCount);
      tagSize = bitCount;
      bitCount = 0;
      rfidBuffer = 0;
      bRet = true;
    }
    else if (4 == bitCount) {
      // 4-bit Wiegand codes for keypads
      rfid = (int)translateEnterEscapeKeyPress(rfidBuffer & 0x0000000F);
      tagSize = bitCount;
      bitCount = 0;
      rfidBuffer = 0;
      bRet = true;
    }
    else if (8 == bitCount) {
      // 8-bit Wiegand codes for keypads with integrity
      // 8-bit Wiegand keyboard data, high nibble is the "NOT" of low nibble
      // eg if key 1 pressed, data=E1 in binary 11100001 , high nibble=1110 , low nibble = 0001
      char highNibble = (rfidBuffer & 0xf0) >>4;
      char lowNibble = (rfidBuffer & 0x0f);
      if (lowNibble == (~highNibble & 0x0f)) {   // Check if low nibble matches the "NOT" of high nibble.
        rfid = (int)translateEnterEscapeKeyPress(lowNibble);
        bRet = true;
      } else {
        lastFoundTime = nowTick;
        bRet = false;
      }
      tagSize = bitCount;
      bitCount = 0;
      rfidBuffer = 0;
    } else {
      // Time reached but unknown bitCount, clear and start again
      lastFoundTime = nowTick;
      bitCount = 0;
      rfidBuffer = 0;
      bRet = false;
    }
  } else {
    bRet = false; // watching time not finished
  }
#if (DEV_WIEGAND_TEST_MODE)>0
  AddLog_P(LOG_LEVEL_INFO, PSTR("WIE: tag out %llu, tag size %u "), rfid, tagSize);
#endif
  return bRet;
}

void Wiegand::ScanForTag() {
  if (!isInit) { return;}
#if (DEV_WIEGAND_TEST_MODE)>0
    AddLog_P(LOG_LEVEL_INFO, PSTR("WIE: ScanForTag()."));
#if (DEV_WIEGAND_TEST_MODE==1)
    switch (millis() %4) {
      case 0:
        rfidBuffer = GetRandomRfid(24);
      break;
      case 1:
        rfidBuffer = GetRandomRfid(26);
      break;
      case 2:
        rfidBuffer = GetRandomRfid(32);
      break;
      case 3:
        rfidBuffer = GetRandomRfid(34);
      break;
      default:
        rfidBuffer = GetRandomRfid(34);
      break;
    }
    AddLog_P(LOG_LEVEL_INFO, PSTR("WIE: raw generated: %lX"), rfidBuffer);  // for tests without reader attaiched
#endif
#endif
  if (bitCount > 0)   {
    uint64_t oldTag = rfid;
    bool validKey =  WiegandConversion();
#if (DEV_WIEGAND_TEST_MODE)>0
    AddLog_P(LOG_LEVEL_INFO, PSTR("WIE: previous tag: %llu"), oldTag);
#endif
    // only in case of valid key do action. Issue#10585
    if (validKey) {
      if (oldTag != rfid) { AddLog_P(LOG_LEVEL_INFO, PSTR("WIE:  new= %llu"), rfid); }
      else { AddLog_P(LOG_LEVEL_INFO, PSTR("WIE: prev= %llu"), rfid); }
      AddLog_P(LOG_LEVEL_INFO, PSTR("WIE: bits= %u"), tagSize);
      ResponseTime_P(PSTR(",\"Wiegand\":{\"UID\":\"%0llu\"}}"), rfid);
      MqttPublishTeleSensor();
    }
  }
}

#ifdef USE_WEBSERVER
void Wiegand::Show(void) {
  if (!isInit) { return; }
  WSContentSend_PD(PSTR("{s}Wiegand UID{m}%llu {e}"), rfid);
#if (DEV_WIEGAND_TEST_MODE)>0
  AddLog_P(LOG_LEVEL_INFO,PSTR("WIE: Tag: %llu"), rfid);
  AddLog_P(LOG_LEVEL_INFO, PSTR("WIE: %u bits"), bitCount);
#endif
}
#endif  // USE_WEBSERVER

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns82(byte function) {
  bool result = false;

  switch (function) {
    case FUNC_INIT:
      oWiegand->Init();
      scanDelay = 1;
      break;

    case FUNC_EVERY_250_MSECOND:  // Some tags need more time, don't try shorter period
#if (DEV_WIEGAND_TEST_MODE)==1
      if (scanDelay >= 4)         // Give a second because of the log entries to be send.
#else
      if (scanDelay >= 2)         // Only run every (delay * 250 ms) (every 250ms is too fast for some tags)
#endif
      {
        oWiegand->ScanForTag();
        scanDelay = 1;
      } else {
        scanDelay++;
      }
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      oWiegand->Show();
      break;
#endif  // USE_WEBSERVER
  }
  return result;
}

#endif  // USE_WIEGAND