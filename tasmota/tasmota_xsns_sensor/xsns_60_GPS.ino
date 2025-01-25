/*
  xsns_60_GPS.ino - GPS UBLOX support for Tasmota

  Copyright (C) 2021  Theo Arends, Christian Baars and Adrian Scillato

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

#ifdef USE_GPS
/*********************************************************************************************\
  --------------------------------------------------------------------------------------------
  Version Date      Action    Description
  --------------------------------------------------------------------------------------------

  0.9.3.0 20200214  integrate - fix set lat/lon via commandd 13, V-Port now works parallel
  ---
  0.9.2.0 20200110  integrate - Added UART-over-TCP/IP-bridge (virtual serial port). Minor tweaks.
  ---
  0.9.1.0 20191216  integrate - Added pin specifications from Tasmota WEB UI. Minor tweaks.
  ---
  0.9.0.0 20190817  started   - further development by Christian Baars  - https://github.com/Staars/Sonoff-Tasmota
                    forked    - from arendst/tasmota                    - https://github.com/arendst/Sonoff-Tasmota
                    base      - code base from arendst and              - https://www.youtube.com/watch?v=TwhCX0c8Xe0

## GPS-driver for the Ublox-series 6-8
Driver is tested on a NEO-6m and a Beitian-220. Series 7 should work too. This adds only about 6kb to the program size, because the efficient UBX-protocol is used. These modules are quite cheap, starting at about 3.50€ for the NEO-6m.

## Features:
- get position and time data
- sets system time automatically and Settings->latitude and Settings->longitude via command
- can log postion data with timestamp to flash with a small memory footprint of only 12 Bytes per record
- constructs a GPX-file for download of this data
- Web-UI
- simplified NTP-server and UART-over-TCP/IP-bridge (virtual serial port)
- command interface
- velocity and heading information with #define USE_GPS_VELOCITY

## Usage:
The serial pins are GPS_RX and GPS_TX, no further installation steps needed. To get more debug information compile it with option "DEBUG_TASMOTA_SENSOR".


## Commands:

+ sensor60 0
  write to all available sectors, then restart and overwrite the older ones

+ sensor60 1
  write to all available sectors, then restart and overwrite the older ones

+ sensor60 2
  filter out horizontal drift noise

+ sensor60 3
  turn off noise filter

+ sensor60 4
  start recording, new data will be appended

+ sensor60 5
  start new recording, old data will lost

+ sensor60 6
  stop recording, download link will be visible in Web-UI

+ sensor60 7
  send mqtt on new postion + TELE -> consider to set TELE to a very high value

+ sensor60 8
  only TELE message

+ sensor60 9
  start NTP-server

+ sensor60 10
  deactivate NTP-server

+ sensor60 11
  force update of Tasmota-system-UTC with every new GPS-time-message

+ sensor60 12
  do not update of Tasmota-system-UTC with every new GPS-time-message

+ sensor60 13
  set latitude and longitude in settings

+ sensor60 14
  open virtual serial port over TCP, usable for u-center

+ sensor60 15
  pause virtual serial port over TCP

## Rules examples for SSD1306 32x128


rule1 on tele-GPS#lat do DisplayText [s1p21c1l01f1]LAT: %value% endon on tele-GPS#lon do DisplayText [s1p21c1l2]LON: %value% endon on switch1#state==3 do sensor60 4 endon on switch1#state==2 do sensor60 6 endon

rule2  on tele-GPS#int>9 do DisplayText [f0c9l4]I%value%  endon  on tele-GPS#int<10 do DisplayText [f0c9l4]I0%value%  endon on tele-GPS#fil==1 do DisplayText [f0c18l4]F endon on tele-GPS#fil==0 do DisplayText [f0c18l4]N endon

rule3 on tele-FLOG#sec do DisplayText  [f0c1l4]SAV:%value%  endon on tele-FLOG#rec==1 do DisplayText [f0c1l4]REC: endon on tele-FLOG#mode do DisplayText [f0c14l4]M%value% endon

\*********************************************************************************************/

#define XSNS_60        60

#include "NTPServer.h"
#include "NTPPacket.h"

/*********************************************************************************************\
 * constants
\*********************************************************************************************/

#define D_CMND_UBX "UBX"

const char S_JSON_UBX_COMMAND_NVALUE[] PROGMEM = "{\"" D_CMND_UBX "%s\":%d}";

const char kUBXTypes[] PROGMEM = "UBX";

#define UBX_LAT_LON_THRESHOLD 100 // filter out some noise of local drift

#define UBX_SERIAL_BUFFER_SIZE 256
#define UBX_TCP_PORT           1234
#define NTP_MILLIS_OFFSET      50              // estimated latency in milliseconds

/********************************************************************************************\
| *globals
\*********************************************************************************************/

const char UBLOX_INIT[] PROGMEM = {
  // Disable NMEA
  0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x24, // GxGGA off
  0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x2B, // GxGLL off
  0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x02,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x32, // GxGSA off
  0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x03,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x39, // GxGSV off
  0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x04,0x00,0x00,0x00,0x00,0x00,0x01,0x04,0x40, // GxRMC off
  0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x05,0x00,0x00,0x00,0x00,0x00,0x01,0x05,0x47, // GxVTG off

  // Disable UBX
  0xB5,0x62,0x06,0x01,0x08,0x00,0x01,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x17,0xDC, //NAV-PVT off
  0xB5,0x62,0x06,0x01,0x08,0x00,0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x12,0xB9, //NAV-POSLLH off
  0xB5,0x62,0x06,0x01,0x08,0x00,0x01,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x13,0xC0, //NAV-STATUS off
  0xB5,0x62,0x06,0x01,0x08,0x00,0x01,0x21,0x00,0x00,0x00,0x00,0x00,0x00,0x31,0x92, //NAV-TIMEUTC off
#ifdef USE_GPS_VELOCITY
  0xB5,0x62,0x06,0x01,0x08,0x00,0x01,0x12,0x00,0x00,0x00,0x00,0x00,0x00,0x22,0x29, //NAV-VELNED off
#endif  // USE_GPS_VELOCITY

  // Enable UBX
  // 0xB5,0x62,0x06,0x01,0x08,0x00,0x01,0x07,0x00,0x01,0x00,0x00,0x00,0x00,0x18,0xE1, //NAV-PVT on
  0xB5,0x62,0x06,0x01,0x08,0x00,0x01,0x02,0x00,0x01,0x00,0x00,0x00,0x00,0x13,0xBE, //NAV-POSLLH on
  0xB5,0x62,0x06,0x01,0x08,0x00,0x01,0x03,0x00,0x01,0x00,0x00,0x00,0x00,0x14,0xC5, //NAV-STATUS on
  0xB5,0x62,0x06,0x01,0x08,0x00,0x01,0x21,0x00,0x01,0x00,0x00,0x00,0x00,0x32,0x97, //NAV-TIMEUTC on
#ifdef USE_GPS_VELOCITY
  0xB5,0x62,0x06,0x01,0x08,0x00,0x01,0x12,0x00,0x01,0x00,0x00,0x00,0x00,0x23,0x2E, //NAV-VELNED on
#endif  // USE_GPS_VELOCITY
  // Rate - we will not reset it for the moment after restart
  //  0xB5,0x62,0x06,0x08,0x06,0x00,0x64,0x00,0x01,0x00,0x01,0x00,0x7A,0x12, //(10Hz)
  //  0xB5,0x62,0x06,0x08,0x06,0x00,0xC8,0x00,0x01,0x00,0x01,0x00,0xDE,0x6A, //(5Hz)
  //  0xB5,0x62,0x06,0x08,0x06,0x00,0xE8,0x03,0x01,0x00,0x01,0x00,0x01,0x39 //(1Hz)
  //  0xB5,0x62,0x06,0x08,0x06,0x00,0xD0,0x07,0x01,0x00,0x01,0x00,0xED,0xBD //(0.5Hz)
};

char       UBX_name[4];

struct UBX_t {
  const char UBX_HEADER[2]        = { 0xB5, 0x62 }; // TODO: Check if we really save space here inside the struct
  const char NAV_POSLLH_HEADER[2] = { 0x01, 0x02 };
  const char NAV_STATUS_HEADER[2] = { 0x01, 0x03 };
  const char NAV_TIME_HEADER[2]   = { 0x01, 0x21 };
#ifdef USE_GPS_VELOCITY
  const char NAV_VEL_HEADER[2]    = { 0x01, 0x12 };
#endif  // USE_GPS_VELOCITY

  int32_t lat;
  int32_t lon;

  struct entry_t {
    int32_t lat;               // raw sensor value
    int32_t lon;               // raw sensor value
    uint32_t time;             // local time from system (maybe provided by the sensor)
  };

  union {
    entry_t values;
    uint8_t bytes[sizeof(entry_t)];
  } rec_buffer;

  struct POLL_MSG {
    uint8_t cls;
    uint8_t id;
    uint16_t zero;
  };

  struct NAV_POSLLH {
    uint8_t cls;               // 0x01
    uint8_t id;                // 0x02
    uint16_t len;              // 28 bytes
    uint32_t iTOW;             // ms
    int32_t lon;               // 1e-7, degree
    int32_t lat;               // 1e-7, degree
    int32_t alt;               // mm
    int32_t hMSL;              // mm
    uint32_t hAcc;             // mm
    uint32_t vAcc;             // mm
  };

  struct NAV_STATUS {
    uint8_t cls;               // 0x01
    uint8_t id;                // 0x03
    uint16_t len;              // 16 bytes
    uint32_t iTOW;             // ms
    uint8_t gpsFix;            //
    uint8_t flags;             // bit 0 - gpsfix valid
    uint8_t fixStat;           //
    uint8_t flags2;            //
    uint32_t ttff;             // ms
    uint32_t msss;             // ms
  };

  struct NAV_TIME_UTC {
    uint8_t cls;               // 0x01
    uint8_t id;                // 0x21
    uint16_t len;              // 20 bytes
    uint32_t iTOW;             // ms
    uint32_t tAcc;             // ns
    int32_t nano;              // Nanoseconds of second, range -1e9 .. 1e9 (UTC)
    uint16_t year;             // y
    uint8_t month;             // month
    uint8_t day;               // d
    uint8_t hour;              // h
    uint8_t min;               // min
    uint8_t sec;               // s
    struct {
      uint8_t UTC:1;
      uint8_t WKN:1;           // week number
      uint8_t TOW:1;           // time of week
      uint8_t padding:5;
    } valid;
  };

#ifdef USE_GPS_VELOCITY
  struct NAV_VEL {
    uint8_t cls;               // 0x01
    uint8_t id;                // 0x12
    uint16_t len;              // 36 bytes
    uint32_t iTOW;             // ms
    int32_t velN;              // cm/s
    int32_t velE;              // cm/s
    int32_t velD;              // cm/s
    uint32_t speed;            // cm/s
    uint32_t gSpeed;           // cm/s
    int32_t heading;           // 1e-5, degree
    uint32_t sAcc;             // cm/s
    uint32_t cAcc;             // 1e-5, degree
  };
#endif  // USE_GPS_VELOCITY

  struct CFG_RATE {
    uint8_t cls;               // 0x06
    uint8_t id;                // 0x08
    uint16_t len;              // 6 bytes
    uint16_t measRate;         // in every ms -> 1 Hz = 1000 ms; 10 Hz = 100 ms -> x = 1000 ms / Hz
    uint16_t navRate;          //  x measurements for 1 navigation event
    uint16_t timeRef;          //  align to time system: 0= UTC, 1 = GPS, 2 = GLONASS, ...
    char CK[2];                // checksum
  };

  struct {
    uint32_t last_iTOW;
    int32_t last_alt;
    uint32_t last_hAcc;
    uint32_t last_vAcc;
    uint8_t gpsFix;
    uint8_t non_empty_loops;   // in case of an unintended reset of the GPS, the serial interface will get flooded with NMEA
    uint16_t log_interval;     // in tenth of seconds
    int32_t timeOffset;        // roughly computed offset millis() - iTOW
  } state;

  struct {
    uint32_t init:1;
    uint32_t filter_noise:1;
    uint32_t send_when_new:1;  // no teleinterval
    uint32_t send_UI_only:1;
    uint32_t runningNTP:1;
    // uint32_t blockedNTP:1;
    uint32_t forceUTCupdate:1;
    uint32_t runningVPort:1;
    // TODO: more to come
  } mode;

  union {
    NAV_POSLLH navPosllh;
    NAV_STATUS navStatus;
    NAV_TIME_UTC navTime;
#ifdef USE_GPS_VELOCITY
    NAV_VEL navVel;
#endif  // USE_GPS_VELOCITY
    POLL_MSG pollMsg;
    CFG_RATE cfgRate;
  } Message;

  uint32_t utc_time;

  uint8_t TCPbuf[UBX_SERIAL_BUFFER_SIZE];
  size_t TCPbufSize;
} UBX;

enum UBXMsgType {
  MT_NONE,
  MT_NAV_POSLLH,
  MT_NAV_STATUS,
  MT_NAV_TIME,
#ifdef USE_GPS_VELOCITY
  MT_NAV_VEL,
#endif  // USE_GPS_VELOCITY
  MT_POLL
};

#ifdef USE_FLOG
FLOG *Flog = nullptr;
#endif  // USE_FLOG
TasmotaSerial *UBXSerial;

NtpServer timeServer(PortUdp);

WiFiServer vPortServer(UBX_TCP_PORT);
WiFiClient vPortClient;

/*********************************************************************************************\
 * helper function
\*********************************************************************************************/

void UBXcalcChecksum(char* CK, size_t msgSize)
{
  memset(CK, 0, 2);
  for (int i = 0; i < msgSize; i++) {
    CK[0] += ((char*)(&UBX.Message))[i];
    CK[1] += CK[0];
  }
}

bool UBXcompareMsgHeader(const char* msgHeader)
{
  char* ptr = (char*)(&UBX.Message);
  return ptr[0] == msgHeader[0] && ptr[1] == msgHeader[1];
}

void UBXinitCFG(void)
{
  for (uint32_t i = 0; i < sizeof(UBLOX_INIT); i++) {
    UBXSerial->write( pgm_read_byte(UBLOX_INIT+i) );
  }
  DEBUG_SENSOR_LOG(PSTR("UBX: turn off NMEA"));
}

void UBXsendCFGLine(uint8_t _line)
{
  if (_line>sizeof(UBLOX_INIT)/16) return;
  for (uint32_t i = 0; i < 16; i++) {
    UBXSerial->write( pgm_read_byte(UBLOX_INIT+i+(_line*16)) );
  }
  DEBUG_SENSOR_LOG(PSTR("UBX: send line %u of UBLOX_INIT"), _line);
}

/********************************************************************************************/

void UBXDetect(void) {
  UBX.mode.init = 0;
  if (!(PinUsed(GPIO_GPS_RX, GPIO_ANY) && PinUsed(GPIO_GPS_TX))) { return; }

  uint32_t option = GetPin(Pin(GPIO_GPS_RX, GPIO_ANY)) - AGPIO(GPIO_GPS_RX);  // 0 .. 2
  uint32_t baudrate = 9600 << option;  // Support 1 (9600), 2 (19200), 3 (38400)
  UBXSerial = new TasmotaSerial(Pin(GPIO_GPS_RX, GPIO_ANY), Pin(GPIO_GPS_TX), 1, 0, UBX_SERIAL_BUFFER_SIZE); // 64 byte buffer is NOT enough
  if (!UBXSerial->begin(baudrate)) { return; }

  if (UBXSerial->hardwareSerial()) {
    ClaimSerial();
  }
#ifdef ESP32
  AddLog(LOG_LEVEL_DEBUG, PSTR("UBX: Serial UART%d"), UBXSerial->getUart());
#endif

  UBXinitCFG();                 // turn off NMEA, only use "our" UBX-messages
  UBX.mode.init = 1;

#ifdef USE_FLOG
  if (!Flog) {
    Flog = new FLOG;            // init Flash Log
    Flog->init();
  }
#endif  // USE_FLOG

  UBX.state.log_interval = 10;  // 1 second
  UBX.mode.send_UI_only = true; // send UI data ...
//  MqttPublishTeleperiodSensor();  // ... once at after start (No MQTT ready yet so do NOT try to send)
}

uint32_t UBXprocessGPS()
{
  static uint32_t fpos = 0;
  static char checksum[2];
  static uint8_t currentMsgType = MT_NONE;
  static size_t payloadSize = sizeof(UBX.Message);

  // DEBUG_SENSOR_LOG(PSTR("UBX: check for serial data"));
  uint32_t data_bytes = 0;
  while ( UBXSerial->available() ) {
    data_bytes++;
    byte c = UBXSerial->read();
    if (UBX.mode.runningVPort){
      UBX.TCPbuf[data_bytes-1] = c; // immediately copy byte to TCP-buf
      UBX.TCPbufSize = data_bytes;
    }
    if ( fpos < 2 ) {
      // For the first two bytes we are simply looking for a match with the UBX header bytes (0xB5,0x62)
      if ( c == UBX.UBX_HEADER[fpos] ) {
        fpos++;
      } else {
        fpos = 0; // Reset to beginning state.
      }
    } else {
      // If we come here then fpos >= 2, which means we have found a match with the UBX_HEADER
      // and we are now reading in the bytes that make up the payload.

      // Place the incoming byte into the ubxMessage struct. The position is fpos-2 because
      // the struct does not include the initial two-byte header (UBX_HEADER).
      if ( (fpos-2) < payloadSize ) {
        ((char*)(&UBX.Message))[fpos-2] = c;
      }
      fpos++;

      if ( fpos == 4 ) {
        // We have just received the second byte of the message type header,
        // so now we can check to see what kind of message it is.
        if ( UBXcompareMsgHeader(UBX.NAV_POSLLH_HEADER) ) {
          currentMsgType = MT_NAV_POSLLH;
          payloadSize = sizeof(UBX_t::NAV_POSLLH);
          DEBUG_SENSOR_LOG(PSTR("UBX: got NAV_POSLLH"));
        }
        else if ( UBXcompareMsgHeader(UBX.NAV_STATUS_HEADER) ) {
          currentMsgType = MT_NAV_STATUS;
          payloadSize = sizeof(UBX_t::NAV_STATUS);
          DEBUG_SENSOR_LOG(PSTR("UBX: got NAV_STATUS"));
        }
        else if ( UBXcompareMsgHeader(UBX.NAV_TIME_HEADER) ) {
          currentMsgType = MT_NAV_TIME;
          payloadSize = sizeof(UBX_t::NAV_TIME_UTC);
          DEBUG_SENSOR_LOG(PSTR("UBX: got NAV_TIME_UTC"));
        }
#ifdef USE_GPS_VELOCITY
        else if ( UBXcompareMsgHeader(UBX.NAV_VEL_HEADER) ) {
          currentMsgType = MT_NAV_VEL;
          payloadSize = sizeof(UBX_t::NAV_VEL);
          DEBUG_SENSOR_LOG(PSTR("UBX: got NAV_VEL"));
        }
#endif  // USE_GPS_VELOCITY
        else {
          // unknown message type, bail
          fpos = 0;
          continue;
        }
      }

      if ( fpos == (payloadSize+2) ) {
        // All payload bytes have now been received, so we can calculate the
        // expected checksum value to compare with the next two incoming bytes.
        UBXcalcChecksum(checksum, payloadSize);
      }
      else if ( fpos == (payloadSize+3) ) {
        // First byte after the payload, ie. first byte of the checksum.
        // Does it match the first byte of the checksum we calculated?
        if ( c != checksum[0] ) {
          // Checksum doesn't match, reset to beginning state and try again.
          fpos = 0;
        }
      }
      else if ( fpos == (payloadSize+4) ) {
        // Second byte after the payload, ie. second byte of the checksum.
        // Does it match the second byte of the checksum we calculated?
        fpos = 0; // We will reset the state regardless of whether the checksum matches.
        if ( c == checksum[1] ) {
          // Checksum matches, we have a valid message.
          return currentMsgType;
        }
      }
      else if ( fpos > (payloadSize+4) ) {
        // We have now read more bytes than both the expected payload and checksum
        // together, so something went wrong. Reset to beginning state and try again.
        fpos = 0;
      }
    }
  }
  // DEBUG_SENSOR_LOG(PSTR("UBX: got none or unknown Message"));
  if (data_bytes!=0) {
    UBX.state.non_empty_loops++;
    DEBUG_SENSOR_LOG(PSTR("UBX: got %u bytes, non-empty-loop: %u"), data_bytes, UBX.state.non_empty_loops);
  } else {
    UBX.state.non_empty_loops = 0; // now a hidden GPS-device reset is unlikely
  }
  return MT_NONE;
}

/********************************************************************************************\
| * callback functions for the download
\*********************************************************************************************/

#ifdef USE_FLOG
void UBXsendHeader(void)
{
  Webserver->setContentLength(CONTENT_LENGTH_UNKNOWN);
  Webserver->sendHeader(F("Content-Disposition"), F("attachment; filename=TASMOTA.gpx"));
  WSSend(200, CT_APP_STREAM, F(
    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\r\n"
    "<GPX version=\"1.1\" creator=\"TASMOTA\" xmlns=\"http://www.topografix.com/GPX/1/1\" \r\n"
    "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\r\n"
    "xsi:schemaLocation=\"http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd\">\r\n"
    "<trk>\r\n<trkseg>\r\n"));
}

void UBXsendRecord(uint8_t *buf)
{
	char record[100];
	char stime[32];
	UBX_t::entry_t *entry = (UBX_t::entry_t*)buf;
	snprintf_P(stime, sizeof(stime), GetDT(entry->time).c_str());
	char lat[FLOATSZ];
	char lon[FLOATSZ];
	dtostrfd((double)entry->lat/10000000.0f,7,lat);
	dtostrfd((double)entry->lon/10000000.0f,7,lon);
	snprintf_P(record, sizeof(record),PSTR("<trkpt\n\t lat=\"%s\" lon=\"%s\">\n\t<time>%s</time>\n</trkpt>\n"),lat ,lon, stime);
	// DEBUG_SENSOR_LOG(PSTR("FLOG: DL %u %u"), Flog->sector.dword_buffer[k+j],Flog->sector.dword_buffer[k+j+1]);
	Webserver->sendContent_P(record);
}

void UBXsendFooter(void)
{
  Webserver->sendContent(F("</trkseg>\n</trk>\n</GPX>"));
  Webserver->sendContent("");
  Rtc.user_time_entry = false; // we have blocked the main loop and want a new valid time
}

/********************************************************************************************/

void UBXsendFile(void)
{
  if (!HttpCheckPriviledgedAccess()) { return; }
  Flog->startDownload(sizeof(UBX.rec_buffer),UBXsendHeader,UBXsendRecord,UBXsendFooter);
}
#endif  // USE_FLOG

/********************************************************************************************/

void UBXSetRate(uint16_t interval)
{
  UBX.Message.cfgRate.cls = 0x06;
  UBX.Message.cfgRate.id = 0x08;
  UBX.Message.cfgRate.len = 6;
  uint32_t measRate = (1000*(uint32_t)interval); //seconds to milliseconds
  if (measRate > 0xffff) {
    measRate = 0xffff; // max. 65535 ms interval
  }
  UBX.Message.cfgRate.measRate = (uint16_t)measRate;
  UBX.Message.cfgRate.navRate = 1;
  UBX.Message.cfgRate.timeRef = 1;
  UBXcalcChecksum(UBX.Message.cfgRate.CK, sizeof(UBX.Message.cfgRate)-sizeof(UBX.Message.cfgRate.CK));
  DEBUG_SENSOR_LOG(PSTR("UBX: requested interval: %u seconds measRate: %u ms"), interval, UBX.Message.cfgRate.measRate);
  UBXSerial->write(UBX.UBX_HEADER[0]);
  UBXSerial->write(UBX.UBX_HEADER[1]);
  for (uint32_t i =0; i<sizeof(UBX.Message.cfgRate); i++) {
    UBXSerial->write(((uint8_t*)(&UBX.Message.cfgRate))[i]);
    DEBUG_SENSOR_LOG(PSTR("UBX: cfgRate byte %u: %x"), i, ((uint8_t*)(&UBX.Message.cfgRate))[i]);
  }
  UBX.state.log_interval = 10*interval;
}

void UBXSelectMode(uint16_t mode)
{
  DEBUG_SENSOR_LOG(PSTR("UBX: set mode to %u"),mode);
  switch(mode){
#ifdef USE_FLOG
    case 0:
      Flog->mode = 0; // write once to all available sectors, then stop
      break;
    case 1:
      Flog->mode = 1; // write to all available sectors, then restart and overwrite the older ones
      break;
    case 2:
      UBX.mode.filter_noise = true;  // filter out horizontal drift noise, TODO: find useful values
      break;
    case 3:
      UBX.mode.filter_noise = false;
      break;
    case 4:
      Flog->startRecording(true);
      AddLog(LOG_LEVEL_INFO, PSTR("UBX: start recording - appending"));
      break;
    case 5:
      Flog->startRecording(false);
      AddLog(LOG_LEVEL_INFO, PSTR("UBX: start recording - new log"));
      break;
    case 6:
      if(Flog->recording == true){
        Flog->stopRecording();
      }
      AddLog(LOG_LEVEL_INFO, PSTR("UBX: stop recording"));
      break;
#endif  // USE_FLOG
    case 7:
      UBX.mode.send_when_new = 1; // send mqtt on new postion + TELE -> consider to set TELE to a very high value
      break;
    case 8:
      UBX.mode.send_when_new = 0; // only TELE
      break;
    case 9:
      if (!TasmotaGlobal.global_state.network_down && timeServer.beginListening()) {
        UBX.mode.runningNTP = true;
      }
      break;
    case 10:
      UBX.mode.runningNTP = false;
#ifdef USE_GPS_VELOCITY
      UBXsendCFGLine(11); //NAV-POSLLH on
      UBXsendCFGLine(12); //NAV-STATUS on
      UBXsendCFGLine(14); //NAV-VELNED on
#else
      UBXsendCFGLine(10); //NAV-POSLLH on
      UBXsendCFGLine(11); //NAV-STATUS on
#endif  // USE_GPS_VELOCITY
      break;
    case 11:
      UBX.mode.forceUTCupdate = true;
      break;
    case 12:
      UBX.mode.forceUTCupdate = false;
      break;
    case 13:
      Settings->latitude = UBX.rec_buffer.values.lat/10;
      Settings->longitude = UBX.rec_buffer.values.lon/10;
      break;
    case 14:
      vPortServer.begin();
      UBX.mode.runningVPort = 1;
      break;
    case 15:
      // vPortServer.stop(); // seems not to work reliably
      UBX.mode.runningVPort = 0;
      break;
    default:
      if (mode>1000 && mode <1066) {
        UBXSetRate(mode-1000); // set interval between measurements in seconds from 1 to 65
      }
      break;
  }
  UBX.mode.send_UI_only = true;
  MqttPublishTeleperiodSensor();
}

/********************************************************************************************/

bool UBXHandlePOSLLH()
{
  DEBUG_SENSOR_LOG(PSTR("UBX: iTOW: %u"),UBX.Message.navPosllh.iTOW);
  if (UBX.state.gpsFix>1) {
    if (UBX.mode.filter_noise) {
      if ((abs(UBX.Message.navPosllh.lat-UBX.rec_buffer.values.lat)<UBX_LAT_LON_THRESHOLD)||(abs(UBX.Message.navPosllh.lon-UBX.rec_buffer.values.lon)<UBX_LAT_LON_THRESHOLD)) {
        DEBUG_SENSOR_LOG(PSTR("UBX: Diff lat: %u lon: %u "),UBX.Message.navPosllh.lat-UBX.rec_buffer.values.lat, UBX.Message.navPosllh.lon-UBX.rec_buffer.values.lon);
        return false; //no new position
      }
    }
    UBX.rec_buffer.values.lat = UBX.Message.navPosllh.lat;
    UBX.rec_buffer.values.lon = UBX.Message.navPosllh.lon;
    DEBUG_SENSOR_LOG(PSTR("UBX: lat/lon: %i / %i"), UBX.rec_buffer.values.lat, UBX.rec_buffer.values.lon);
    DEBUG_SENSOR_LOG(PSTR("UBX: hAcc: %d"), UBX.Message.navPosllh.hAcc);
    UBX.state.last_alt = UBX.Message.navPosllh.alt;
    UBX.state.last_vAcc = UBX.Message.navPosllh.vAcc;
    UBX.state.last_hAcc = UBX.Message.navPosllh.hAcc;
    if (UBX.mode.send_when_new) {
      MqttPublishTeleperiodSensor();
    }
    if (UBX.mode.runningNTP){ // after receiving pos-data at least once -> go to pure NTP-mode
      UBXsendCFGLine(7);      // NAV-POSLLH off
      UBXsendCFGLine(8);      // NAV-STATUS off
#ifdef USE_GPS_VELOCITY
      UBXsendCFGLine(10);     // NAV-VELNED off
#endif  // USE_GPS_VELOCITY
    }
    //UBX_LAT_LON_THRESHOLD = 20 * UBX.Message.navPosllh.hAcc;
    return true; // new position
  } else {
    DEBUG_SENSOR_LOG(PSTR("UBX: no valid position data"));
  }
  return false; // no GPS-fix
}

#ifdef USE_GPS_VELOCITY
void UBXHandleVEL()
  {
    DEBUG_SENSOR_LOG(PSTR("UBX: iTOWvel: %u"),UBX.Message.navVel.iTOW);
    if (UBX.state.gpsFix>1) {
      DEBUG_SENSOR_LOG(PSTR("UBX: speed: %d"), UBX.Message.navVel.gSpeed);
      DEBUG_SENSOR_LOG(PSTR("UBX: heading: %i"), UBX.Message.navVel.heading);
      DEBUG_SENSOR_LOG(PSTR("UBX: spd accuracy: %i"), UBX.Message.navVel.sAcc);
      DEBUG_SENSOR_LOG(PSTR("UBX: hdng accuracy: %i"), UBX.Message.navVel.cAcc);
    }
  
}
#endif  // USE_GPS_VELOCITY

void UBXHandleSTATUS()
{
  DEBUG_SENSOR_LOG(PSTR("UBX: gpsFix: %u, valid: %u"), UBX.Message.navStatus.gpsFix, (UBX.Message.navStatus.flags)&1);
  if ((UBX.Message.navStatus.flags)&1) {
    UBX.state.gpsFix = UBX.Message.navStatus.gpsFix; //only store fixed status if flag is valid
  } else {
    UBX.state.gpsFix = 0; // without valid flag, everything is "no fix"
  }
}

void UBXHandleTIME()
{
  DEBUG_SENSOR_LOG(PSTR("UBX: UTC-Time: %u-%u-%u %u:%u:%u"), UBX.Message.navTime.year, UBX.Message.navTime.month ,UBX.Message.navTime.day,UBX.Message.navTime.hour,UBX.Message.navTime.min,UBX.Message.navTime.sec);
 if ((UBX.Message.navTime.valid.UTC == 1) && (UBX.Message.navTime.year >= 2023)) {
    UBX.state.timeOffset =  millis(); // iTOW%1000 should be 0 here, when NTP-server is enabled and in "pure mode"
    DEBUG_SENSOR_LOG(PSTR("UBX: UTC-Time is valid"));
    bool resync = (Rtc.utc_time > UBX.utc_time);  // Sync local time every hour
    if (Rtc.user_time_entry == false || UBX.mode.forceUTCupdate || UBX.mode.runningNTP || resync) {
      TIME_T gpsTime;
      gpsTime.year = UBX.Message.navTime.year - 1970;
      gpsTime.month = UBX.Message.navTime.month;
      gpsTime.day_of_month = UBX.Message.navTime.day;
      gpsTime.hour = UBX.Message.navTime.hour;
      gpsTime.minute = UBX.Message.navTime.min;
      gpsTime.second = UBX.Message.navTime.sec;
      UBX.rec_buffer.values.time = MakeTime(gpsTime);
      if (UBX.mode.forceUTCupdate || (Rtc.user_time_entry == false) || resync) {
//        AddLog(LOG_LEVEL_INFO, PSTR("UBX: UTC-Time is valid, set system time"));
        UBX.utc_time = UBX.rec_buffer.values.time + 3600;
        Rtc.utc_time = UBX.rec_buffer.values.time;
        RtcSync("UBX");
      }
      Rtc.user_time_entry = true;
    }
  }
}

void UBXHandleOther(void)
{
  if (UBX.state.non_empty_loops>6) {  // we expect only 4-5 non-empty loops in a row, could change with other sensor speed (Hz)
    if(UBX.mode.runningVPort) return;
    UBXinitCFG();                     // this should only happen with lots of NMEA-messages, but it is only a guess!!
    AddLog(LOG_LEVEL_ERROR, PSTR("UBX: possible device-reset, will re-init"));
    UBXSerial->flush();
    UBX.state.non_empty_loops = 0;
  }
}

/********************************************************************************************/

void UBXLoop50msec(void)
{
  // handle virtual serial port
  if (UBX.mode.runningVPort){
    if(!vPortClient.connected()) {
      vPortClient = vPortServer.available();
    }
    while(vPortClient.available()) {
      byte _newByte = vPortClient.read();
      UBXSerial->write(_newByte);
    }

    if (UBX.TCPbufSize!=0){
      vPortClient.write((char*)UBX.TCPbuf, UBX.TCPbufSize);
      UBX.TCPbufSize = 0;
    }
  }
  // handle NTP-server
  if(!TasmotaGlobal.global_state.network_down && UBX.mode.runningNTP){
    timeServer.processOneRequest(UBX.rec_buffer.values.time, UBX.state.timeOffset - NTP_MILLIS_OFFSET);
  }
}

void UBXLoop(void)
{
  static uint16_t counter; //count up every 100 msec
  static bool new_position;

  uint32_t msgType = UBXprocessGPS();

  switch(msgType){
    case MT_NAV_POSLLH:
      new_position = UBXHandlePOSLLH();
      break;
    case MT_NAV_STATUS:
      UBXHandleSTATUS();
      break;
    case MT_NAV_TIME:
      UBXHandleTIME();
      break;
#ifdef USE_GPS_VELOCITY
    case MT_NAV_VEL:
      UBXHandleVEL();
      break;
#endif  // USE_GPS_VELOCITY
    default:
      UBXHandleOther();
      break;
  }

#ifdef USE_FLOG
  if (counter>UBX.state.log_interval) {
    if (Flog->recording && new_position) {
      UBX.rec_buffer.values.time = Rtc.local_time;
      Flog->addToBuffer(UBX.rec_buffer.bytes, sizeof(UBX.rec_buffer.bytes));
      counter = 0;
    }
  }
#endif  // USE_FLOG

  counter++;
}

/********************************************************************************************/
// normaly in i18n.h

#ifdef USE_WEBSERVER
// {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>

#ifdef USE_FLOG
#ifdef DEBUG_TASMOTA_SENSOR
const char HTTP_SNS_FLOGVER[] PROGMEM = "{s}FLOG with %u sectors:{m}%u bytes{e}"
                                        "{s}FLOG next sector for REC:{m} %u {e}"
                                        "{s}%u sector(s) with data at sector:{m}%u{e}";
const char HTTP_SNS_FLOGREC[] PROGMEM = "{s}RECORDING (bytes in buffer){m}%u{e}";
#endif  // DEBUG_TASMOTA_SENSOR

const char HTTP_SNS_FLOG[] PROGMEM = "{s}GPS Logging{m}%s{e}";
const char kFLOGstate[] PROGMEM = "Ready|Recording";
const char HTTP_BTN_FLOG_DL[] PROGMEM = "<button><a href='/UBX'>Download GPX-File</a></button>";
#endif  // USE_FLOG

const char HTTP_SNS_NTPSERVER[] PROGMEM = "{s}GPS NTP server{m}Active{e}";

const char HTTP_SNS_GPS[] PROGMEM = "{s}GPS " D_SAT_FIX "{m}%s{e}"
                                    "{s}GPS " D_LATITUDE "{m}%s{e}"
                                    "{s}GPS " D_LONGITUDE "{m}%s{e}"
                                    "{s}GPS " D_HORIZONTAL_ACCURACY "{m}%3_f " D_UNIT_METER "{e}"
                                    "{s}GPS " D_ALTITUDE "{m}%3_f " D_UNIT_METER "{e}"
                                    "{s}GPS " D_VERTICAL_ACCURACY "{m}%3_f " D_UNIT_METER "{e}";
#ifdef USE_GPS_VELOCITY
const char HTTP_SNS_GPS2[] PROGMEM = "{s}GPS " D_SPEED "{m}%2_f " D_UNIT_KILOMETER_PER_HOUR "{e}"
                                     "{s}GPS " D_SPEED_ACCURACY "{m}%2_f " D_UNIT_KILOMETER_PER_HOUR "{e}"
                                     "{s}GPS " D_HEADING "{m}%1_f{e}"
                                     "{s}GPS " D_HEADING_ACCURACY "{m}%1_f{e}";
#endif  // USE_GPS_VELOCITY

#ifdef USE_GPS_MAPS
const char UBX_GOOGLE_MAPS[] ="<iframe width='100%%' src='https://maps.google.com/maps?width=&amp;height=&amp;hl=en&amp;q=%s %s+(Tasmota)&amp;ie=UTF8&amp;t=&amp;z=10&amp;iwloc=B&amp;output=embed' frameborder='0' scrolling='no' marginheight='0' marginwidth='0'></iframe>";
#endif  // USE_GPS_MAPS

#endif  // USE_WEBSERVER

const char kGPSFix[] PROGMEM = D_SAT_FIX_NO_FIX "|" D_SAT_FIX_DEAD_RECK "|" D_SAT_FIX_2D "|" D_SAT_FIX_3D "|" D_SAT_FIX_GPS_DEAD "|" D_SAT_FIX_TIME;

/********************************************************************************************/

void UBXShow(bool json) {
  char fix[32];
  GetTextIndexed(fix, sizeof(fix), UBX.state.gpsFix, kGPSFix);
  char lat[FLOATSZ];
  dtostrfd((double)UBX.rec_buffer.values.lat / 10000000.0f, 7, lat);  // degrees
  char lon[FLOATSZ];
  dtostrfd((double)UBX.rec_buffer.values.lon / 10000000.0f, 7, lon);  // degrees
  float hAcc = (float)UBX.state.last_vAcc / 1000.0f;                  // mm -> meters
  float alt = (float)UBX.state.last_alt / 1000.0f;                    // mm -> meters
  float vAcc = (float)UBX.state.last_hAcc / 1000.0f;                  // mm -> meters
#ifdef USE_GPS_VELOCITY
  float spd = (float)UBX.Message.navVel.gSpeed / 27.778f;             // cm/s -> km/h
  float sAcc = (float)UBX.Message.navVel.sAcc / 27.778f;              // cm/s -> km/h
  float hdng = (float)UBX.Message.navVel.heading / 100000.0f;         // degrees
  float cAcc = (float)UBX.Message.navVel.cAcc / 100000.0f;            // degrees
  if (cAcc > 360) { cAcc = 0; }
#endif  // USE_GPS_VELOCITY

  if (json) {
    ResponseAppend_P(PSTR(",\"GPS\":{"));
    if (UBX.mode.send_UI_only) {
      uint32_t i = UBX.state.log_interval / 10;
      ResponseAppend_P(PSTR("\"Fil\":%u,\"Int\":%u}"), UBX.mode.filter_noise, i);
    } else {
      ResponseAppend_P(PSTR("\"Lat\":%s,\"Lon\":%s,\"Alt\":%3_f,\"hAcc\":%3_f,\"vAcc\":%3_f,\"Fix\":\"%s\""),
        lat, lon, &alt, &hAcc, &vAcc, fix);
#ifdef USE_GPS_VELOCITY
      ResponseAppend_P(PSTR(",\"Spd\":%2_f,\"Hdng\":%1_f,\"sAcc\":%2_f,\"cAcc\":%1_f"), 
        &spd, &hdng, &sAcc, &cAcc);
#endif  // USE_GPS_VELOCITY
      ResponseAppend_P(PSTR("}"));
    }
#ifdef USE_FLOG
    ResponseAppend_P(PSTR(",\"FLOG\":{\"Rec\":%u,\"Mode\":%u,\"Sec\":%u}"), Flog->recording, Flog->mode, Flog->sectors_left);
#endif  // USE_FLOG
    UBX.mode.send_UI_only = false;
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_SNS_GPS, fix, lat, lon, &hAcc, &alt, &vAcc);
#ifdef USE_GPS_VELOCITY
    WSContentSend_PD(HTTP_SNS_GPS2, &spd, &sAcc, &hdng, &cAcc);
#endif  // USE_GPS_VELOCITY

#ifdef USE_GPS_MAPS
    int32_t lat_diff = UBX.rec_buffer.values.lat - UBX.lat;
    int32_t lon_diff = UBX.rec_buffer.values.lon - UBX.lon;
    if ((lat_diff > 1000) || (lon_diff > 1000)) {
      UBX.lat = UBX.rec_buffer.values.lat;
      UBX.lon = UBX.rec_buffer.values.lon;
      WSContentSend_P(UBX_GOOGLE_MAPS, lat, lon);
    }
#endif  // USE_GPS_MAPS

#ifdef USE_FLOG
    WSContentSeparator(0);
#ifdef DEBUG_TASMOTA_SENSOR
    WSContentSend_PD(HTTP_SNS_FLOGVER, Flog->num_sectors, Flog->size, Flog->current_sector, Flog->sectors_left, Flog->sector.header.physical_start_sector);
    if (Flog->recording) {
      WSContentSend_PD(HTTP_SNS_FLOGREC, Flog->sector.header.buf_pointer - 8);
    }
#endif  // DEBUG_TASMOTA_SENSOR
    if (Flog->ready) {
      char flog_state[32];
      WSContentSend_P(HTTP_SNS_FLOG, GetTextIndexed(flog_state, sizeof(flog_state), Flog->recording, kFLOGstate));
    }
    if (!Flog->recording && Flog->found_saved_data) {
      WSContentSend_P(HTTP_BTN_FLOG_DL);
    }
#endif  // USE_FLOG

    if (UBX.mode.runningNTP) {
      WSContentSeparator(0);
      WSContentSend_P(HTTP_SNS_NTPSERVER);
    }

#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * check the UBX commands
\*********************************************************************************************/

bool UBXCmd(void)
{
  bool serviced = true;
  if (XdrvMailbox.data_len > 0) {
    UBXSelectMode(XdrvMailbox.payload);
    Response_P(S_JSON_UBX_COMMAND_NVALUE, XdrvMailbox.command, XdrvMailbox.payload);
  }
  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns60(uint32_t function)
{
  bool result = false;

  if (FUNC_INIT == function) {
    UBXDetect();
  }

  if (UBX.mode.init) {
    switch (function) {
      case FUNC_COMMAND_SENSOR:
        if (XSNS_60 == XdrvMailbox.index) {
          result = UBXCmd();
        }
        break;
      case FUNC_EVERY_50_MSECOND:
        UBXLoop50msec(); // handles virtual serial port and NTP server
        break;
      case FUNC_EVERY_100_MSECOND:
#ifdef USE_FLOG
        if (!Flog->running_download)
#endif  // USE_FLOG
        {
          UBXLoop();
        }
        break;
#ifdef USE_FLOG
      case FUNC_WEB_ADD_HANDLER:
        WebServer_on(PSTR("/UBX"), UBXsendFile);
        break;
#endif  // USE_FLOG
      case FUNC_JSON_APPEND:
        UBXShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
#ifdef USE_FLOG
        if (!Flog->running_download)
#endif  // USE_FLOG
        {
          UBXShow(0);
        }
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_GPS
