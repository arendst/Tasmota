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
 * contains:
 *  - fix for #11047 Wiegand 26/34 missed some key press if they are press at normal speed  
 *  - removed testing code for tests without attached hardware 
 *  - added SetOption123 0-Wiegand UID decimal (default) 1-Wiegand UID hexadecimal 
 *  - added SetOption124 0-Keypad every key a single tag (default) 1-all keys up to ending char (#) send as one tag  
\*********************************************************************************************/
#warning **** Wiegand interface enabled ****

#define XSNS_82                82

#define WIEGAND_CODE_GAP_FACTOR 3  // Gap between 2 complete RFID codes send by the device. (WIEGAND_CODE_GAP_FACTOR * bitTime) to detect the end of a code 
#define WIEGAND_BIT_TIME_DEFAULT 1250  // period time of one bit (impluse + impulse_gap time) 1250µs measured by oscilloscope on my RFID Reader
#define WIEGAND_RFID_ARRAY_SIZE 5 // storage of rfids found between 2 calls of FUNC_EVERY_100_MSECOND
#define WIEGAND_OPTION_HEX 123 // Index of option to switch output between hex (1) an decimal (0) (default)

//  using #define will save some space in the final code
//  DEV_WIEGAND_TEST_MODE 2 : testing with hardware correctly connected.
#define DEV_WIEGAND_TEST_MODE  0

#ifdef DEV_WIEGAND_TEST_MODE
  #if (DEV_WIEGAND_TEST_MODE==0)
  #elif (DEV_WIEGAND_TEST_MODE==1)
    #warning "(no longer available) Wiegand Interface compiled with 'DEV_WIEGAND_TEST_MODE' 1 (Random RFID)"
  #elif (DEV_WIEGAND_TEST_MODE==2)
    #warning "Wiegand Interface compiled with 'DEV_WIEGAND_TEST_MODE' 2 (Hardware connected)"
  #else
    #warning "Wiegand Interface compiled with unknown mode"
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

  private:
    //uint64_t HexStringToDec(uint64_t);
    uint64_t CheckAndConvertRfid(uint64_t,uint16_t);
    char translateEnterEscapeKeyPress(char);
    uint8_t CalculateParities(uint64_t, int);
    bool WiegandConversion (uint64_t , uint16_t );
    void setOutputFormat(void); // fix output HEX format

    static void handleD0Interrupt(void);
    static void handleD1Interrupt(void);
    static void handleDxInterrupt(int in); // fix #11047 

    uint64_t rfid;
    uint8_t tagSize;
    char outFormat;

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
  currentFoundRFIDcount=0;
  for (int i=0; i < WIEGAND_RFID_ARRAY_SIZE; i++ )
  {
    rfid_found[i].RFID=0;
    rfid_found[i].bitCount=0;
  }
  outFormat='u';  // standard output format decimal
}

void ICACHE_RAM_ATTR Wiegand::handleD1Interrupt() {  // Receive a 1 bit. (D0=high & D1=low)
  handleDxInterrupt(1);
}

void ICACHE_RAM_ATTR Wiegand::handleD0Interrupt() {  // Receive a 0 bit. (D0=low & D1=high)
  handleDxInterrupt(0);                    
}

void ICACHE_RAM_ATTR Wiegand::handleDxInterrupt(int in) {
  
  unsigned long curTime = micros();  // to be sure I will use micros() instead of millis() overflow is handle by using the minus operator to compare
  unsigned long diffTime= curTime - lastFoundTime;
  if (diffTime > 3000000 ) {  //cancel noisy bits in buffer and start a new tag
    rfidBuffer = 0; 
    bitCount = 0;
    FirstBitTimeStamp = 0; 
   } 
  if ( (diffTime > CodeGapTime) && (bitCount > 0)) { 
    // previous RFID tag (key pad numer)is complete. Will be detected by the code ending gap
    // one bit will take the time of impulse_time + impulse_gap_time. it (bitTime) will be recalculated each time an impulse is detected
    // the devices will add some inter_code_gap_time to separate codes this will be much longer than the bit_time. (WIEGAND_CODE_GAP_FACTOR)
    // unfortunately there's no timing defined for Wiegang. On my test reader the impulse time = 125 µs impulse gap time = 950 µs.
    if (currentFoundRFIDcount < WIEGAND_RFID_ARRAY_SIZE) { // when reaching the end of rfid buffer we will overwrite the last one.
      currentFoundRFIDcount++;
    }
    // start a new tag
    rfidBuffer = 0; 
    bitCount = 0;
    FirstBitTimeStamp = 0; 
  }
  if (in ==3) {// called by ScanForTag to get the last tag, because the interrupt handler is no longer called after receiving the last bit
    return;
  }
  if (in == 0) { rfidBuffer = rfidBuffer << 1; } // Receive a 0 bit. (D0=low & D1=high): Leftshift the 0 bit is now at the end of rfidBuffer
  else {rfidBuffer = (rfidBuffer << 1) | 1; }    // Receive a 1 bit. (D0=high & D1=low): Leftshift + 1 bit

  bitCount++; 
  if (bitCount == 1) { // first bit was detected
    FirstBitTimeStamp = (curTime != 0) ? curTime : 1; // accept 1µs differenct to avoid a miss the first timestamp if curTime is 0.
  }
  else if (bitCount == 2) { // only calculate once per RFID tag
    bitTime = diffTime;  //calc maximum current length of one bit
    CodeGapTime = WIEGAND_CODE_GAP_FACTOR * bitTime;
  }   
  //save current rfid in array otherwise we will never see the last found tag
  rfid_found[currentFoundRFIDcount].RFID=rfidBuffer;
  rfid_found[currentFoundRFIDcount].bitCount= bitCount;
  lastFoundTime = curTime; // Last time a bit was detected
}

void Wiegand::Init() {
  isInit = false;
  if (PinUsed(GPIO_WIEGAND_D0) && PinUsed(GPIO_WIEGAND_D1)) {  // Only start, if the Wiegang pins are selected
#if (DEV_WIEGAND_TEST_MODE)>0
    AddLog(LOG_LEVEL_INFO, PSTR("WIE: Init()"));
#endif  // DEV_WIEGAND_TEST_MODE>0
    pinMode(Pin(GPIO_WIEGAND_D0), INPUT_PULLUP);
    pinMode(Pin(GPIO_WIEGAND_D1), INPUT_PULLUP);
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

uint64_t Wiegand::CheckAndConvertRfid(uint64_t rfidIn, uint16_t bitCount) {
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
      evenParityBit = (rfidIn & 0x400000000) ? 0x80 : 0;
      rfidIn = (rfidIn & 0x3FFFFFFFE) >>1;
    break;

    default:
    break;
  }
  calcParity = CalculateParities(rfidIn, bitCount);    // Check result on http://www.ccdesignworks.com/wiegand_calc.htm with raw tag as input
  if (calcParity != (evenParityBit | oddParityBit)) {  // Parity bit is wrong
    rfidIn=0;
    AddLog(LOG_LEVEL_DEBUG, PSTR("WIE: %llu parity error"), rfidIn);
  }
#if (DEV_WIEGAND_TEST_MODE)>0
  AddLog(LOG_LEVEL_INFO, PSTR("WIE: even (left) parity: %u "), (evenParityBit>>7));
  AddLog(LOG_LEVEL_INFO, PSTR("WIE: even (calc) parity: %u "), (calcParity & 0x80)>>7);
  AddLog(LOG_LEVEL_INFO, PSTR("WIE: odd (right) parity: %u "), oddParityBit);
  AddLog(LOG_LEVEL_INFO, PSTR("WIE: odd (calc) parity: %u "), (calcParity & 0x01));
#endif  // DEV_WIEGAND_TEST_MODE>0
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

bool Wiegand::WiegandConversion (uint64_t rfidBuffer, uint16_t bitCount) {
  bool bRet = false;
	// unsigned long nowTick = micros();
  // Add a maximum wait time for new bits
  // unsigned long diffTicks = nowTick - lastFoundTime;
  // unsigned long inter_code_gap = WIEGAND_CODE_GAP_FACTOR * bitTime;
  // if ((diffTicks > inter_code_gap) && (diffTicks >= 1000000 )) {  // Max. 4-8 secs between 2 bits comming in. depends on micros() resolution

#if (DEV_WIEGAND_TEST_MODE)>0
    AddLog(LOG_LEVEL_INFO, PSTR("WIE: Raw tag %llu, Bit count %u"), rfidBuffer, bitCount);
#endif  // DEV_WIEGAND_TEST_MODE>0
    if ((24 == bitCount) || (26 == bitCount) || (32 == bitCount) || (34 == bitCount)) {
      // 24, 26, 32, 34-bit Wiegand codes
      rfid = CheckAndConvertRfid(rfidBuffer, bitCount);
      tagSize = bitCount;
      bRet = true;
    }
    else if (4 == bitCount) {
      // 4-bit Wiegand codes for keypads
      rfid = (int)translateEnterEscapeKeyPress(rfidBuffer & 0x0000000F);
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
        rfid = (int)translateEnterEscapeKeyPress(lowNibble);
        bRet = true;
      } else {
        // lastFoundTime = nowTick;
        bRet = false;
      }
      tagSize = bitCount;
    } else {
      // Time reached but unknown bitCount, clear and start again
      // lastFoundTime = nowTick;
      bRet = false;
    }
#if (DEV_WIEGAND_TEST_MODE)>0
  AddLog(LOG_LEVEL_INFO, PSTR("WIE: Tag out %llu, tag size %u "), rfid, tagSize);
#endif  // DEV_WIEGAND_TEST_MODE>0
  return bRet;
}

void Wiegand::setOutputFormat(void)
{
  if (GetOption(WIEGAND_OPTION_HEX) == 0)  { outFormat = 'u';  }
  else  {  outFormat = 'X';  }
}

void Wiegand::ScanForTag() {
  unsigned long startTime = micros();
  handleDxInterrupt(3);
  if (currentFoundRFIDcount > 0)   {
    unsigned int lastFoundRFIDcount = currentFoundRFIDcount;
    #if (DEV_WIEGAND_TEST_MODE)>0
    AddLog(LOG_LEVEL_INFO, PSTR("WIE: ScanForTag(). bitTime: %0lu lastFoundTime: %0lu RFIDS in buffer: %lu"), bitTime, lastFoundTime, currentFoundRFIDcount);
    #endif
    // format MQTT output
    setOutputFormat();
    char sFormat[50];
    snprintf( sFormat, 50, PSTR(",\"Wiegand\":{\"UID\":%%0ll%c,\"" D_JSON_SIZE "\":%%%c}}"), outFormat, outFormat);

    for (int i= 0; i < WIEGAND_RFID_ARRAY_SIZE; i++)
    {
      if (rfid_found[i].RFID != 0) {
        uint64_t oldTag = rfid;
        bool validKey =  WiegandConversion(rfid_found[i].RFID, rfid_found[i].bitCount);
        #if (DEV_WIEGAND_TEST_MODE)>0
        AddLog(LOG_LEVEL_INFO, PSTR("WIE: Previous tag %llu"), oldTag);
        #endif  // DEV_WIEGAND_TEST_MODE>0
        if (validKey) {  // Only in case of valid key do action. Issue#10585
          if (oldTag == rfid) {
            AddLog(LOG_LEVEL_DEBUG, PSTR("WIE: Old tag"));
          }
          ResponseTime_P(sFormat, rfid,tagSize);
          MqttPublishTeleSensor();
        }
        rfid_found[i].RFID=0;
        rfid_found[i].bitCount=0;
      }
    }
    if (currentFoundRFIDcount > lastFoundRFIDcount) {
      // if that happens: we need to move the id found during the loop to top of the array
      // and correct the currentFoundRFIDcount
    }
    currentFoundRFIDcount=0; //reset array  
    #if (DEV_WIEGAND_TEST_MODE)>0
    AddLog(LOG_LEVEL_INFO, PSTR("WIE: ScanForTag() time elapsed %lu"), (micros() - startTime));  
    #endif
  }
  
}

#ifdef USE_WEBSERVER
void Wiegand::Show(void) {
  setOutputFormat(); 
  char sFormat [30];
  snprintf( sFormat, 30,PSTR("{s}Wiegand UID{m}%%ll%c {e}"), outFormat);
  WSContentSend_PD(sFormat, rfid);
  //WSContentSend_PD(PSTR("{s}Wiegand UID{m}%llX {e}"), rfid);
#if (DEV_WIEGAND_TEST_MODE)>0
  AddLog(LOG_LEVEL_INFO, PSTR("WIE: Tag %llu, Bits %u"), rfid, bitCount);
#endif  // DEV_WIEGAND_TEST_MODE>0
}
#endif  // USE_WEBSERVER

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns82(byte function) {
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
    }
  }
  return result;
}

#endif  // USE_WIEGAND