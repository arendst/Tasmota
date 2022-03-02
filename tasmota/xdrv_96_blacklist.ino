/*
  xdrv_96_blacklist.ino - Blacklist for Tasmota

  SPDX-FileCopyrightText: 2022 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#define USE_BLACKLIST

#ifdef USE_BLACKLIST
/*********************************************************************************************\
 * Blacklist support
 *
 * Check language and user set latitude/longitude against blacklist table
\*********************************************************************************************/

#define XDRV_96            96

typedef struct {
  int16_t latitude_tl;    // - 8999 to 8999
  int16_t longitude_tl;   // -17999 to 17999
  int16_t latitude_br;
  int16_t longitude_br;
  uint16_t lcid;
} tBlArray;

//const char BlacklistText[] PROGMEM = "Stop war, Help Ukrain|Stop war, Help Ukrain|";
const char BlacklistText[] PROGMEM = "Стоп войну, помоги Украине";  // Stop war, Help Ukraine

//                   lat_tl lon_tl lat_br lon_br lcid
tBlArray BlArray[] {  5900,  3200,  5300,  4400, 1049,   // Around Moscow
                      5450,  2633,  5280,  2900, 1049    // Around Minsk
                   };

uint8_t blist_loc;
uint8_t blist_show;

void BListDetect(void) {
  blist_loc = 0;
  blist_show = 0;
  int latitude = Settings->latitude / 10000;
  int longitude = Settings->longitude / 10000;
  uint32_t count = sizeof(BlArray) / sizeof(tBlArray);
  for (uint32_t i = 0; i < count; i++) {
    // Currently only supports top-right quarter of the earth
    if ((LANGUAGE_LCID == BlArray[i].lcid) &&            // Check language id
        (latitude < BlArray[i].latitude_tl) &&           // Check user set latitude and longitude against table
        (latitude > BlArray[i].latitude_br) &&
        (longitude > BlArray[i].longitude_tl) &&
        (longitude < BlArray[i].longitude_br)) {
      blist_loc = i +1;
      break;
    }
  }
}

void BListEverySecond(void) {
  if (Rtc.utc_time < 1648771200) {                       // Only until 2022-04-01
    if (0 == (TasmotaGlobal.uptime % 20)) {              // Only every 20 seconds
      if (TasmotaGlobal.power) {                         // Only if any power on

//        char bl_text[100];
//        snprintf_P(bl_text, sizeof(bl_text), PSTR("Power0 0"));   // Turn all power off - annoying
//        snprintf_P(bl_text, sizeof(bl_text), PSTR("Restart 1"));  // Restart - more annoying
//        snprintf_P(bl_text, sizeof(bl_text), PSTR("Reset 1"));    // Reset - disastrous
//        ExecuteCommand(bl_text, SRC_IGNORE);

//        char bl_text[100];
//        AddLog(LOG_LEVEL_NONE, PSTR("**** %s ****"), GetTextIndexed(bl_text, sizeof(bl_text), i, BlacklistText));
        AddLog(LOG_LEVEL_NONE, PSTR("**** Stop war, Help Ukrain ****"));
        blist_show = blist_loc;                          // Set GUI message id
      }
    } else if (0 == (TasmotaGlobal.uptime % 10)) {       // Only every 10 seconds
      blist_show = 0;                                    // Reset GUI message id after 10 seconds
    }
  } else {
    blist_loc = 0;                                       // Stop blacklist
  }
}

#ifdef USE_WEBSERVER
void BListShow(bool json) {
  if (blist_show) {
//    char bl_text[100];
//    WSContentSend_PD(PSTR("{s}**** %s ****{m}{e}"), GetTextIndexed(bl_text, sizeof(bl_text), blist_show -1, BlacklistText));
    WSContentSend_P(PSTR("{s}**** %s ****{m}{e}"), BlacklistText);
  }
}
#endif  // USE_WEBSERVER

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv96(uint8_t function) {
  bool result = false;

  if (FUNC_INIT == function) {
    BListDetect();
  }
  else if (blist_loc) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        BListEverySecond();
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        BListShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }

  return result;
}

#endif // USE_BLACKLIST
