#ifndef H_XSNS_82_WIEGAND
#define H_XSNS_82_WIEGAND

#include <stdint.h>
#include <Arduino.h>

  class Wiegand 
  {
    public:

    Wiegand(void);
    void Init(void);
    void ScanForTag(void);
   /*  uint16_t GetTagSize(void);
    void SetTagSize(uint8_t); */
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

#endif