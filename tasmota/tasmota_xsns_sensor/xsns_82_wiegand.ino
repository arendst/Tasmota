/*
  xsns_82_wiegand.ino - Support for Wiegand Interface 125kHz Rfid Tag Reader for Tasmota

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
 * Wire connections for https://www.benselectronics.nl/wiegand-2634-bit-rfid-reader-125khze248d29925d602d.html
 * Red          Vdc
 * Black        Gnd
 * Green        D0
 * White        D1
 * Yellow/Grey  Sound Buzzer if connected to Gnd
 * Blue         Light Led if connected to Gnd
 * Grey/Purple  34-bit if connected to Gnd
 *
 * MQTT:
 * %prefix%/%topic%/SENSOR = {"Time":"2021-01-21T16:04:12","Wiegand":{"UID":7748328,"Size":26}}
 * %prefix%/%topic%/SENSOR = {"Time":"2021-01-21T15:48:49","Wiegand":{"UID":4302741608,"Size":34}}
 *
 * Rule:
 * on wiegand#uid=4302741608 do publish cmnd/ailight/power 2 endon
 *
 * 20220215
 *  - fix 34-bit size parity chk
 *  - fix 64-bit representation after removal of %llu support (Tasmota does not support 64-bit decimal output specifier (%llu) saving 60k code)
 *  - change internal rfid size from uint64_t to uint32_t
 *  - limited max amount of kaypad presses to a 32-bit number (at least 999999999)
 * ---
 * 20201101
 *  - fix for #11047 Wiegand 26/34 missed some key press if they are press at normal speed
 *  - removed testing code for tests without attached hardware
 *  - added SetOption123 0-Wiegand UID decimal (default) 1-Wiegand UID hexadecimal
 *  - added SetOption124 0-all keys up to ending char (# or *) send as one tag by MQTT (default) 1-Keypad every key a single tag
 *  - added a new realtime testing option emulating a Wiegang reader output on same GPIOs where normally reader is attached. Details below
 *  - fix timing issue when fast glitches are detected on one on the datalines. The interbitgab was too short in that case
\*********************************************************************************************/
#pragma message("**** Wiegand interface enabled ****")

#define XSNS_82                82

#define WIEGAND_CODE_GAP_FACTOR 3  // Gap between 2 complete RFID codes send by the device. (WIEGAND_CODE_GAP_FACTOR * bitTime) to detect the end of a code
#define WIEGAND_BIT_TIME_DEFAULT 1250  // period time (µs) of one bit (impluse + impulse_gap time) 1250µs measured by oscilloscope on my RFID Reader
#define WIEGAND_RFID_ARRAY_SIZE 11 // storage of rfids found between 2 calls of FUNC_EVERY_100_MSECOND
#define WIEGAND_OPTION_HEX 123 // Index of option to switch output between hex (1) an decimal (0) (default)
#define WIEGAND_OPTION_HEX_POSTFIX "h"  // will be added after UID output nothing = ""
#define WIEGAND_OPTION_KEYPAD_TO_TAG 124 //Index of option to switch output of key pad strokes between every single stroke one single char (0) (default)
                                         // or all strokes until detecting ending char (WIEGAND_OPTION_KEYPAD_END_CHAR) as one tag (1)

#define DEV_WIEGAND_TEST_MODE  0
//  using #define will save some space in the final code
//  DEV_WIEGAND_TEST_MODE 1 : Use only without Wiegand reader device attache. On a second ESP to simulate reader output!
//  DEV_WIEGAND_TEST_MODE 2 : testing with hardware correctly connected.

#ifdef DEV_WIEGAND_TEST_MODE
  #if (DEV_WIEGAND_TEST_MODE==0)
  #elif (DEV_WIEGAND_TEST_MODE==1)
    #pragma message("\nWiegand Interface code generator (testing purpose only!) compiled with 'DEV_WIEGAND_TEST_MODE' 1 \nUse only on esp WITHOUT Wiegand reader hardware attached! GPIOS will be configured as OUTPUT!" )
    // use on own risk for testing purpose only.
    // please don't attach your reader to the ESP when you use this option. The GPIOS will be defined as OUTPUT
    // the interrupts will be enabled and normally recognize the generated code, that's the idea behind for testing.
    // Commands:
    // WieBitTime [time] : get or set the bit impuls length
    // WieInterBitTime [time]: get or set the length of the gap between 2 bits
    // WieTagGap [tagGap]: get or set the current used gap time between 2 tags send in µs minimal WIEGAND_BIT_TIME_DEFAULT µs default WIEGAND_BIT_TIME_DEFAULT * WIEGAND_CODE_GAP_FACTOR
    // WieTagSize [tagsize]:  get or set the tagsize (4,8,24,26,32,34) default 26.
    // WieTag [tag]:  get or set the current used tag. For tagsize 4,8 only one char will be used.
    // WieSend [tag[:tagsize];tag[:tagsize];...] : Generate the current Tag with current TagSize to GPIOs if the paramters are used
    //                                             tags and tagsize from commandline are used as current values. If tagsize is omitted always last value will be used
    //                                             WieSend 4:4;5:8; will send 4 in 4 bit mode and 5 in 8 bit mode with a pause of current TagGab between the chars
    //                                             WieSend  will send the last used tag with last used tagsize
  #elif (DEV_WIEGAND_TEST_MODE==2)
    #pragma message("\nWiegand Interface compiled with 'DEV_WIEGAND_TEST_MODE' 2 (Hardware connected)")
  #else
    #pragma message("\nWiegand Interface compiled with unknown mode")
  #endif
#endif

typedef struct rfid_store { uint64_t RFID; uint16_t bitCount; } RFID_store;

class Wiegand {
  public:
    Wiegand(void);
    void Init(void);
    void ScanForTag(void);
#ifdef USE_WEBSERVER
    void Show(void);
#endif  // USE_WEBSERVER

    bool isInit = false;

    #if (DEV_WIEGAND_TEST_MODE!=1)
    private:
    #endif //(DEV_WIEGAND_TEST_MODE==1)
    uint32_t CheckAndConvertRfid(uint64_t,uint16_t);
    uint8_t CalculateParities(uint64_t, int);
    bool WiegandConversion (uint64_t , uint16_t );
    void HandleKeyPad(void); //handle one tag for multi key strokes

    static void handleD0Interrupt(void);
    static void handleD1Interrupt(void);
    static void handleDxInterrupt(int in); // fix #11047
    static void ClearRFIDBuffer(int);

    uint32_t rfid;
    uint32_t tagSize;
    const char* outFormat;
    uint32_t mqttRFIDKeypadBuffer;
    uint32_t webRFIDKeypadBuffer;

    static volatile uint64_t rfidBuffer;
    static volatile uint16_t bitCount;
    static volatile uint32_t lastFoundTime;
    // fix #11047
    static volatile uint32_t bitTime;
    static volatile uint32_t FirstBitTimeStamp;
    static volatile uint32_t CodeGapTime;
    static volatile bool CodeComplete;
    static volatile RFID_store rfid_found[];
    static volatile int currentFoundRFIDcount;
};

Wiegand* oWiegand = new Wiegand();

volatile uint64_t Wiegand::rfidBuffer;
volatile uint16_t Wiegand::bitCount;
volatile uint32_t Wiegand::lastFoundTime;
// fix for #11047
volatile uint32_t Wiegand::bitTime;
volatile uint32_t Wiegand::FirstBitTimeStamp;
volatile uint32_t Wiegand::CodeGapTime;
volatile bool Wiegand::CodeComplete;
volatile RFID_store Wiegand::rfid_found[WIEGAND_RFID_ARRAY_SIZE];
volatile int Wiegand::currentFoundRFIDcount;

void IRAM_ATTR Wiegand::ClearRFIDBuffer(int endIndex = WIEGAND_RFID_ARRAY_SIZE) {
  currentFoundRFIDcount = WIEGAND_RFID_ARRAY_SIZE - endIndex; // clear all buffers
  for (uint32_t i = 0; i < endIndex; i++) {
    rfid_found[i].RFID=0;
    rfid_found[i].bitCount=0;
  }
}

void IRAM_ATTR Wiegand::handleD1Interrupt() {  // Receive a 1 bit. (D0=high & D1=low)
  handleDxInterrupt(1);
}

void IRAM_ATTR Wiegand::handleD0Interrupt() {  // Receive a 0 bit. (D0=low & D1=high)
  handleDxInterrupt(0);
}

void IRAM_ATTR Wiegand::handleDxInterrupt(int in) {
  uint32_t curTime = micros();  // to be sure I will use micros() instead of millis() overflow is handle by using the minus operator to compare
  uint32_t diffTime = curTime - lastFoundTime;
  if ((diffTime > CodeGapTime) && (bitCount > 0)) {
    // previous RFID tag (key pad numer)is complete. Will be detected by the code ending gap
    // one bit will take the time of impulse_time + impulse_gap_time. it (bitTime) will be recalculated each time an impulse is detected
    // the devices will add some inter_code_gap_time to separate codes this will be much longer than the bit_time. (WIEGAND_CODE_GAP_FACTOR)
    // unfortunately there's no timing defined for Wiegand. On my test reader the impulse time = 125 µs impulse gap time = 950 µs.
    if (currentFoundRFIDcount < WIEGAND_RFID_ARRAY_SIZE) { // when reaching the end of rfid buffer we will overwrite the last one.
      currentFoundRFIDcount++;
    }
    // start a new tag
    rfidBuffer = 0;
    bitCount = 0;
    FirstBitTimeStamp = 0;
  }

  if (in == 0) { rfidBuffer = rfidBuffer << 1; } // Receive a 0 bit. (D0=low & D1=high): Leftshift the 0 bit is now at the end of rfidBuffer
  else if (in == 1)  {rfidBuffer = (rfidBuffer << 1) | 1; }    // Receive a 1 bit. (D0=high & D1=low): Leftshift + 1 bit
  else { return; } // (in==3) called by ScanForTag to get the last tag, because the interrupt handler is no longer called after receiving the last bit

  bitCount++;
  if (bitCount == 1) { // first bit was detected
    FirstBitTimeStamp = (curTime != 0) ? curTime : 1; // accept 1µs differenct to avoid a miss the first timestamp if curTime is 0.
  }
  else if (bitCount == 2) { // only calculate once per RFID tag, but restrict to values, which are in within a plausible range
    bitTime = ((diffTime > (WIEGAND_BIT_TIME_DEFAULT / 4)) && (diffTime < (4 * WIEGAND_BIT_TIME_DEFAULT))) ? diffTime : WIEGAND_BIT_TIME_DEFAULT;
    CodeGapTime = WIEGAND_CODE_GAP_FACTOR * bitTime;
  }
  //save current rfid in array otherwise we will never see the last found tag
  rfid_found[currentFoundRFIDcount].RFID=rfidBuffer;
  rfid_found[currentFoundRFIDcount].bitCount= bitCount;
  lastFoundTime = curTime; // Last time a bit was detected
}

Wiegand::Wiegand() {
  rfid = 0;
  lastFoundTime = 0;
  tagSize = 0;
  rfidBuffer = 0;
  bitCount = 0 ;
  isInit = false;
  // fix #11047
  bitTime = WIEGAND_BIT_TIME_DEFAULT;
  FirstBitTimeStamp = 0;
  CodeGapTime = WIEGAND_CODE_GAP_FACTOR * bitTime;
  CodeComplete = false;
  ClearRFIDBuffer();
  outFormat="u";  // standard output format decimal
  mqttRFIDKeypadBuffer = 0;
  webRFIDKeypadBuffer = 0;
}

void Wiegand::Init() {
  isInit = false;
  if (PinUsed(GPIO_WIEGAND_D0) && PinUsed(GPIO_WIEGAND_D1)) {  // Only start, if the Wiegang pins are selected
#if (DEV_WIEGAND_TEST_MODE)>0
    AddLog(LOG_LEVEL_INFO, PSTR("WIE: Init()"));
#endif  // DEV_WIEGAND_TEST_MODE>0
    pinMode(Pin(GPIO_WIEGAND_D0), INPUT_PULLUP);
    pinMode(Pin(GPIO_WIEGAND_D1), INPUT_PULLUP);
#if (DEV_WIEGAND_TEST_MODE==1) // overwrite the setting
    pinMode(Pin(GPIO_WIEGAND_D0), OUTPUT);
    pinMode(Pin(GPIO_WIEGAND_D1), OUTPUT);
#endif //(DEV_WIEGAND_TEST_MODE==1)
    attachInterrupt(Pin(GPIO_WIEGAND_D0), handleD0Interrupt, FALLING);
    attachInterrupt(Pin(GPIO_WIEGAND_D1), handleD1Interrupt, FALLING);
    isInit = true;                                             // Helps to run only if correctly setup
#if (DEV_WIEGAND_TEST_MODE)>0
    AddLog(LOG_LEVEL_INFO, PSTR("WIE: Testmode, D0:%u, D1:%u"), Pin(GPIO_WIEGAND_D0), Pin(GPIO_WIEGAND_D1));        // For tests without reader attaiched
#else
    AddLog(LOG_LEVEL_INFO, PSTR("WIE: Wiegand Rfid Reader detected"));
#endif  // DEV_WIEGAND_TEST_MODE>0
  }
#if (DEV_WIEGAND_TEST_MODE)>0
  else {
    AddLog(LOG_LEVEL_INFO, PSTR("WIE: no GPIOs."));
  }
#endif  // DEV_WIEGAND_TEST_MODE>0
}

uint32_t Wiegand::CheckAndConvertRfid(uint64_t rfidIn, uint16_t bitCount) {
  uint8_t evenParityBit = 0;
  uint8_t oddParityBit = (uint8_t) (rfidIn & 0x1);  // Last bit = odd parity
  uint8_t calcParity = 0;
  switch (bitCount) {
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
      evenParityBit = (rfidIn & 0x200000000) ? 0x80 : 0;
      rfidIn = (rfidIn & 0x1FFFFFFFE) >>1;
    break;

    default:
    break;
  }
  calcParity = CalculateParities(rfidIn, bitCount);    // Check result on http://www.ccdesignworks.com/wiegand_calc.htm with raw tag as input
  if (calcParity != (evenParityBit | oddParityBit)) {  // Parity bit is wrong
    AddLog(LOG_LEVEL_DEBUG, PSTR("WIE: %_X parity error"), &rfidIn);  // Print up to uint64_t
    rfidIn=0;
  }
#if (DEV_WIEGAND_TEST_MODE)>0
  AddLog(LOG_LEVEL_INFO, PSTR("WIE: even (left) parity: %u "), (evenParityBit>>7));
  AddLog(LOG_LEVEL_INFO, PSTR("WIE: even (calc) parity: %u "), (calcParity & 0x80)>>7);
  AddLog(LOG_LEVEL_INFO, PSTR("WIE: odd (right) parity: %u "), oddParityBit);
  AddLog(LOG_LEVEL_INFO, PSTR("WIE: odd (calc) parity: %u "), (calcParity & 0x01));
#endif  // DEV_WIEGAND_TEST_MODE>0
  return (uint32_t)rfidIn;
}

uint8_t Wiegand::CalculateParities(uint64_t tagWithoutParities, int tag_size = 26) {
  // tag_size is the size of the final tag including the 2 parity bits
  // So length if the tagWithoutParities should be (tag_size-2) !! That will be not profed and
  // lead to wrong results if the input value is larger!
  // Calculated start parity (even) will be returned as bit 8
  // calculated end parity (odd) will be returned as bit 1
  uint8_t retValue=0;
  tag_size -= 2;
  if (tag_size <= 0) { return retValue; }      // Prohibit div zero exception and other wrong inputs
  uint8_t parity = 1;                          // Check for odd parity on LSB
  for (uint32_t i = 0; i < (tag_size / 2); i++) {
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

bool Wiegand::WiegandConversion (uint64_t rfidBuffer, uint16_t bitCount) {
  bool bRet = false;
#if (DEV_WIEGAND_TEST_MODE)>0
  AddLog(LOG_LEVEL_INFO, PSTR("WIE: Raw tag %_X, Bit count %u"), &rfidBuffer, bitCount);  // Print up to uint64_t
#endif  // DEV_WIEGAND_TEST_MODE>0
  if ((24 == bitCount) || (26 == bitCount) || (32 == bitCount) || (34 == bitCount)) {
    // 24, 26, 32, 34-bit Wiegand codes
    rfid = CheckAndConvertRfid(rfidBuffer, bitCount);
    tagSize = bitCount;
    bRet = true;
  }
  else if (4 == bitCount) {
    // 4-bit Wiegand codes for keypads
    rfid = (uint32_t)(rfidBuffer & 0x0000000F);
    tagSize = bitCount;
    bRet = true;
  }
  else if (8 == bitCount) {
    // 8-bit Wiegand codes for keypads with integrity
    // 8-bit Wiegand keyboard data, high nibble is the "NOT" of low nibble
    // eg if key 1 pressed, data=E1 in binary 11100001 , high nibble=1110 , low nibble = 0001
    char highNibble = (rfidBuffer & 0xf0) >>4;
    char lowNibble = (rfidBuffer & 0x0f);
    if (lowNibble == (~highNibble & 0x0f)) {   // Check if low nibble matches the "NOT" of high nibble.
      rfid = (uint32_t)(lowNibble);
      bRet = true;
    } else {
      bRet = false;
    }
    tagSize = bitCount;
  } else {
    // Time reached but unknown bitCount, clear and start again
    tagSize = 0;
    bRet = false;
  }
#if (DEV_WIEGAND_TEST_MODE)>0
  AddLog(LOG_LEVEL_INFO, PSTR("WIE: Tag out %u, tag size %u "), rfid, tagSize);
#endif  // DEV_WIEGAND_TEST_MODE>0
  return bRet;
}

void Wiegand::HandleKeyPad(void) { // will be called if a valid key pad input was recognized
  if (GetOption(WIEGAND_OPTION_KEYPAD_TO_TAG) == 0) { // handle all key pad inputs as ONE Tag until # is recognized
    if ((tagSize == 4) || (tagSize == 8)) {
      //only handle Keypad strokes if it is requested
      if (rfid >= 0x0a) { // # * as end of input detected -> all key values which are larger than 9
        rfid = mqttRFIDKeypadBuffer; // original tagsize of 4 or 8 will be kept.
        webRFIDKeypadBuffer = 0; // can be resetted, because now rfid > 0 will be used at web interface
        mqttRFIDKeypadBuffer = 0;
      }
      else {
        mqttRFIDKeypadBuffer = (mqttRFIDKeypadBuffer * 10) + rfid; //left shift + new key
        webRFIDKeypadBuffer = mqttRFIDKeypadBuffer; // visualising the current typed keys
        rfid = 0;
        tagSize = 0;
      }
    }
    else { //it's not a key pad entry, so another key come in, we will reset the buffer, if it is not finished yet
      webRFIDKeypadBuffer = 0;
      mqttRFIDKeypadBuffer = 0;
    }
  }
}

void Wiegand::ScanForTag() {
  uint32_t startTime = micros();
  handleDxInterrupt(3);
  if (currentFoundRFIDcount > 0) {
    uint32_t lastFoundRFIDcount = currentFoundRFIDcount;
#if (DEV_WIEGAND_TEST_MODE)>0
    AddLog(LOG_LEVEL_INFO, PSTR("WIE: ScanForTag(). bitTime: %u lastFoundTime: %u RFIDS in buffer: %u"), bitTime, lastFoundTime, currentFoundRFIDcount);
#endif
    for (uint32_t i = 0; i < WIEGAND_RFID_ARRAY_SIZE; i++) {
  	  if (rfid_found[i].RFID != 0 || (rfid_found[i].RFID == 0 && i == 0)) {
        uint32_t oldTag = rfid;
        bool validKey =  WiegandConversion(rfid_found[i].RFID, rfid_found[i].bitCount);
#if (DEV_WIEGAND_TEST_MODE)>0
        AddLog(LOG_LEVEL_INFO, PSTR("WIE: ValidKey %d, Previous tag %u"), validKey, oldTag);
#endif  // DEV_WIEGAND_TEST_MODE>0
        if (validKey) {  // Only in case of valid key do action. Issue#10585
          HandleKeyPad();  //support one tag for multi key input
          if (tagSize > 0) { //do output only for rfids which are complete
            if (oldTag == rfid) {
              AddLog(LOG_LEVEL_DEBUG, PSTR("WIE: Old tag"));
            }
            ResponseTime_P(PSTR(",\"Wiegand\":{\"UID\":"));
            if (GetOption(WIEGAND_OPTION_HEX) == 0) {
              ResponseAppend_P(PSTR("%u"), rfid);
            } else {
              ResponseAppend_P(PSTR("\"%X" WIEGAND_OPTION_HEX_POSTFIX "\""), rfid);
            }
            ResponseAppend_P(PSTR(",\"" D_JSON_SIZE "\":%d}}"), tagSize);
            MqttPublishTeleSensor();
          }
        }
      }
    }
    if (currentFoundRFIDcount > lastFoundRFIDcount) {
      // if that happens: we need to move the id found during the loop to top of the array
      // and correct the currentFoundRFIDcount
      AddLog(LOG_LEVEL_INFO, PSTR("WIE: ScanForTag() %u tags added while working on buffer"), (currentFoundRFIDcount - lastFoundRFIDcount));
    }
    ClearRFIDBuffer(); //reset array
#if (DEV_WIEGAND_TEST_MODE)>0
    AddLog(LOG_LEVEL_INFO, PSTR("WIE: ScanForTag() time elapsed %u"), (micros() - startTime));
#endif
  }
}

#ifdef USE_WEBSERVER
void Wiegand::Show(void) {
  WSContentSend_P(PSTR("{s}Wiegand UID{m}"));
  if (GetOption(WIEGAND_OPTION_HEX) == 0) {
    WSContentSend_P(PSTR("%u"), (tagSize > 0) ? rfid : webRFIDKeypadBuffer);
  } else {
    WSContentSend_P(PSTR("%X" WIEGAND_OPTION_HEX_POSTFIX), (tagSize > 0) ? rfid : webRFIDKeypadBuffer);
  }
  WSContentSend_P(PSTR("{e}"));


#if (DEV_WIEGAND_TEST_MODE)>0
  AddLog(LOG_LEVEL_INFO, PSTR("WIE: Tag %u, Bits %u"), rfid, bitCount);
#endif  // DEV_WIEGAND_TEST_MODE>0
}
#endif  // USE_WEBSERVER

#if (DEV_WIEGAND_TEST_MODE==1)
  void CmndTag(void);
  void CmndTagSize(void);
  void CmndTagGap(void);
  void CmndTimeReset(void);
  void CmndAllReset(void);
  void CmndSend(void);
  void CmndBitTime(void);
  void CmndInterBitTime(void);
  unsigned int setTagSize( char *);
  unsigned int setTag ( char * );
  void sendBit(unsigned int b);
  void sendTag(uint32_t Tag, uint32_t TagSize);

  uint32_t currTag = 0;
  uint32_t currTagSize = 26; //default value 26 Wiegand
  uint32_t currBitTime=(WIEGAND_BIT_TIME_DEFAULT/10); //length of the bit impluse in µs
  uint32_t currInterBitTime = ((WIEGAND_BIT_TIME_DEFAULT/10)*9); //time to wait before next bit is send in µs
  uint32_t currTagGabTime = (WIEGAND_BIT_TIME_DEFAULT * WIEGAND_CODE_GAP_FACTOR) ; //time to wait before next tag is send in µs


  void CmndTag(void){
    if (XdrvMailbox.data_len > 0) {
      currTag= strtoul(XdrvMailbox.data, nullptr, 0);
    }
    ResponseCmndNumber(currTag);
  }
  void CmndTagSize(void){
    if (XdrvMailbox.data_len > 0) {
      currTagSize = setTagSize(XdrvMailbox.data);
    }
    ResponseCmndNumber(currTagSize);
  }
  void CmndTagGap(void){
    if (XdrvMailbox.data_len > 0) {
        currTagGabTime = strtoul(XdrvMailbox.data, nullptr, 0);
        if (currTagGabTime < (currBitTime+currInterBitTime) ) // doesn't make sense
        { currTagGabTime = (currBitTime+currInterBitTime) * WIEGAND_CODE_GAP_FACTOR; }
      }
      ResponseCmndNumber(currTagGabTime);
  }
  void CmndBitTime(void){
    if (XdrvMailbox.data_len > 0) {
      uint32_t newBitTime = strtoul(XdrvMailbox.data, nullptr, 0);
      if ( (newBitTime >= 100) && (newBitTime <= 500000) ) // accept only values between 100µs and 5s
      { currBitTime = newBitTime; }
    }
    ResponseCmndNumber(currBitTime);
  }
  void CmndInterBitTime(void){
    if (XdrvMailbox.data_len > 0) {
      uint32_t newInterBitTime = strtoul(XdrvMailbox.data, nullptr, 0);
      if ( (newInterBitTime >= currBitTime) && (newInterBitTime <= (100 * currBitTime)) ) // accept only values between 100µs and 5s
      { currInterBitTime = newInterBitTime; }
    }
    ResponseCmndNumber(currInterBitTime);
  }
  void CmndTimeReset(void){
    currBitTime=(WIEGAND_BIT_TIME_DEFAULT/10);
    currInterBitTime = ((WIEGAND_BIT_TIME_DEFAULT/10)*9);
    currTagGabTime = (WIEGAND_BIT_TIME_DEFAULT * WIEGAND_CODE_GAP_FACTOR) ;
    ResponseCmndChar_P(PSTR("All timings reset to default!"));
  }
  void CmndAllReset(void){
    CmndTimeReset();
    currTagSize = 26;
    ResponseCmndChar_P(PSTR("All timings and tag size reset to default"));
  }
  void CmndSend(void){
    if (XdrvMailbox.data_len > 0) { // parameter [tag[:tagsize];tag[:tagsize];...]
      char *parameter = strtok(XdrvMailbox.data, ";");
      while (parameter != nullptr) {
        char* pTagSize = strchr(parameter,':'); // find optional tagsizes
        if (pTagSize != 0) { // 2 parameters found tag:tagsize
          *pTagSize = 0;  //replace separator ':' by \0 string end
          currTag = setTag(parameter);  // is now ending before tagsize
          pTagSize++; //set the starting char of tagsize correctly
          currTagSize = setTagSize(pTagSize);
          ResponseCmndChar(pTagSize);
        }
        else {//only one parameter (tag) found
          currTag = setTag(parameter);
        }
        ResponseCmndChar(parameter);
        sendTag(currTag, currTagSize);
        ResponseCmndNumber(currTag);
        parameter = strtok(nullptr, ";");
      }
    }
    else { // send last used values again
      sendTag(currTag, currTagSize);
      ResponseCmndNumber(currTag);
    }
  }
  unsigned int setTag ( char * newTag) {
    unsigned int retValue = strtoul(newTag, nullptr, 0);
    if ( (currTagSize == 4) || (currTagSize == 8) ) //key pad input simulation requested
      { retValue &= 0x0F; }
    return retValue;
  }
  unsigned int setTagSize ( char * newTagSize) {
      unsigned int retValue = strtoul(newTagSize, nullptr, 0);
       // accept only supported TagSize
      if ( retValue <= 4) { retValue = 4;}
      else if ( retValue <= 8) { retValue = 8;}
      else if ( retValue <= 24) { retValue = 24;}
      else if ( retValue <= 26) { retValue = 26;}
      else if ( retValue <= 32) { retValue = 32;}
      else if ( retValue <= 34) { retValue = 34;}
      else { retValue = 26;} //default value
    return retValue;
  }
  void sendBit(unsigned int b) {
    int sel = (b == 0) ? Pin(GPIO_WIEGAND_D0) : Pin(GPIO_WIEGAND_D1);
    digitalWrite(sel, 0);
    delayMicroseconds(currBitTime); // bit impuls time
    digitalWrite(sel, 1);
    delayMicroseconds(currBitTime+currInterBitTime); // bit + inter bit gap time
  }
  void sendPlainTag( uint32_t pTag, uint32_t pTagSize){ // send tag without parity
    for (int i=1; i<=pTagSize; ++i)
      {
        sendBit((pTag >> (pTagSize-i)) & 1);
      }
  }
  void sendTag(uint32_t Tag, uint32_t TagSize) {
  // TagSize is the requested output tagSize. means b.e. 24 bit == 24 Tag without parity 26 bit = 24 bit with parity bits
  // supported tag sizes 4/8 for key pad simulation 24/26 and 32/34 for RFID tags
  switch (TagSize){
    case 24:
    case 32:
    case 4:
      sendPlainTag( Tag, TagSize);
      break;
    case 26:
    case 34:
      uint8_t parity;
      parity = oWiegand->CalculateParities(Tag, TagSize);
      sendBit(parity & 0x01); //even parity (starting parity)
      sendPlainTag( Tag, TagSize-2);
      sendBit(parity & 0x80); //odd parity (ending parity)
      break;
    case 8: // high nibble is ~ low nibble
      Tag = Tag & 0x0F; // low nibble in case of more the one char input it will be cut here
      Tag = Tag | ((~Tag) << 4);
      sendPlainTag ( Tag, TagSize);
      break;
  }
  //delay to simulate end of tag
  delayMicroseconds(currTagGabTime); // inter code gap
  return;
}
const char kWiegandCommands[] PROGMEM = "Wie|"  // No prefix
    "Tag|"
    "TagSize|"
    "TagGap|"
    "BitTime|"
    "InterBitTime|"
    "TimeReset|"
    "AllReset|"
    "Send";

  void (* const WiegandCommand[])(void) PROGMEM = {
    &CmndTag,
    &CmndTagSize,
    &CmndTagGap,
    &CmndBitTime,
    &CmndInterBitTime,
    &CmndTimeReset,
    &CmndAllReset,
    &CmndSend
   };
#endif //(DEV_WIEGAND_TEST_MODE==1)
/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns82(uint32_t function) {
  bool result = false;

  if (FUNC_INIT == function) {
    oWiegand->Init();
  }
  else if (oWiegand->isInit) {
    switch (function) {
      case FUNC_EVERY_100_MSECOND:       // fix for #11047 Wiegand 26/34 missed some key press
        oWiegand->ScanForTag();
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        oWiegand->Show();
        break;
#endif  // USE_WEBSERVER
#if (DEV_WIEGAND_TEST_MODE==1)
      case FUNC_COMMAND:
      result = DecodeCommand(kWiegandCommands, WiegandCommand);
      break;
#endif //(DEV_WIEGAND_TEST_MODE==1)
    }
  }
  return result;
}

#endif  // USE_WIEGAND