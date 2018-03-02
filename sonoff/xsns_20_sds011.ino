#ifdef USE_SDS011
/* novafitness dust sensors:
 * SDS011
 * SDS021
 *
 */

#include <TasmotaSerial.h>

TasmotaSerial *SDS011Serial;

struct SDS011_data {
  uint16_t pm100, pm25;
} SDS011_data;

uint8_t SDS011_valid = 0;
uint8_t SDS011_type = 1;

#ifdef USE_WEBSERVER
const char HTTP_SDS011_SNS[] PROGMEM = "%s"
    "{s}SDS011 " D_ENVIRONMENTAL_CONCENTRATION " 2.5 " D_UNIT_MICROMETER "{m}%d " D_UNIT_MICROGRAM_PER_CUBIC_METER "{e}"
    "{s}SDS011 " D_ENVIRONMENTAL_CONCENTRATION " 10 " D_UNIT_MICROMETER "{m}%d " D_UNIT_MICROGRAM_PER_CUBIC_METER "{e}";      // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER

void SDS011Show(boolean json)
{
  if (SDS011_valid) {
    if (json) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"SDS011\":{\"PM2.5\":%d,\"PM10\":%d}"), mqtt_data, SDS011_data.pm25, SDS011_data.pm100);
#ifdef USE_WEBSERVER
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SDS011_SNS, mqtt_data, SDS011_data.pm25/10, SDS011_data.pm100/10);
#endif  // USE_WEBSERVER
    }
  }
}

void SDS011Init()
{
  SDS011_type = 0;

  if (pin[GPIO_SDS011] < 99) {
    SDS011Serial = new TasmotaSerial(pin[GPIO_SDS011],-1);
    if (SDS011Serial->begin()) {
      AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("SDS011: Init successful"));
      SDS011_type = 1;
      exit;
    }
    AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("SDS011: Init failed"));
  }
}

bool SDS011ReadData() {
    
    if (! SDS011Serial->available()) {
        return false;
    }
   
    byte d[8]={0};
    while ((SDS011Serial->peek() != 0xaa) && SDS011Serial->available()) 
    {
        SDS011Serial->read();
    }
    SDS011Serial->read();//skip 0xaa
    SDS011Serial->readBytes(d, 8); 
    SDS011Serial->flush();
    AddLogSerial(LOG_LEVEL_DEBUG_MORE, d, 8);
    if (d[7] == ((d[1]+d[2]+d[3]+d[4]+d[5]+d[6])&0xff)) {
        SDS011_data.pm25 = (d[1]+256*d[2]);        
        SDS011_data.pm100 = (d[3]+256*d[4]);        
    }
    else
    {
        AddLog_P(LOG_LEVEL_DEBUG, PSTR("SDS011: " D_CHECKSUM_FAILURE));
        return false;
    }
    SDS011_valid = 10;
    return true;
}
 
void SDS011Second()
{
    if (SDS011ReadData()) {
        SDS011_valid = 10;
    } else {
        if (SDS011_valid) {
            SDS011_valid--;
        }
    }
}

#define XSNS_20

boolean Xsns20(byte function)
{ 
  boolean result = false;
  
  if (SDS011_type) {
    switch (function) {
      case FUNC_INIT:
        SDS011Init();
        break;
      case FUNC_EVERY_SECOND:
        SDS011Second();
        break;
      case FUNC_JSON_APPEND:
        SDS011Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        SDS011Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}



#endif //USE_SDS011
