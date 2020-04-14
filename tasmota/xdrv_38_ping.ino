/*
  xdrv_38_ping.ino - support for ICMP Ping

  Copyright (C) 2020  Theo Arends and Stephan Hadinger

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

#ifdef USE_PING

#define XDRV_38                    38

#include <ping.h>

const char kPingCommands[] PROGMEM =  "|"    // no prefix
  D_CMND_PING
  ;

void (* const PingCommand[])(void) PROGMEM = {
  &CmndPing,
  };

// inspired by https://github.com/dancol90/ESP8266Ping

typedef struct Ping_t {
  uint16_t    total_count;        // total count if packets sent
  uint16_t    timeout_count;      // time-outs (no responses)
  uint32_t    min_time;           // minimum time in ms for a successful response
  uint32_t    max_time;           // maximum time in ms for a successful response
  uint32_t    sum_time;           // cumulated time in ms for all successful responses (used to compute the average)
  bool        busy;               // is ping on-going
  bool        done;               // indicates the ping campaign is finished
} Ping_t;

ping_option ping_opt;
Ping_t ping;

extern "C" {
  // callbacks for ping

  // called after a ping response is received or time-out
  void ICACHE_RAM_ATTR ping_recv_cb(ping_option *popt, struct ping_resp *p_resp) {
    // If successful
    if (p_resp->ping_err >= 0) {
      uint32_t resp_time = p_resp->resp_time;
      ping.sum_time += resp_time;
      if (resp_time < ping.min_time) { ping.min_time = resp_time; }
      if (resp_time > ping.max_time) { ping.max_time = resp_time; }
    }
  }

  // called after the ping campaign is finished
  void ICACHE_RAM_ATTR ping_sent_cb(ping_option *popt, struct ping_resp *p_resp) {
    // copy counters to build the MQTT response
    ping.total_count = p_resp->total_count;
    ping.timeout_count = p_resp->timeout_count;
    ping.done = true;
  }
}

// Check if any ping requests is completed, and publish the results
void PingResponsePoll(void) {
  if (ping.done) {
    uint32_t success = ping.total_count - ping.timeout_count;
    uint32_t ip = ping_opt.ip;

    // Serial.printf(
    //         "DEBUG ping_sent_cb: ping reply\n"
    //         "\tsuccess_count = %d \n"
    //         "\ttimeout_count = %d \n"
    //         "\tmin_time = %d \n"
    //         "\tmax_time = %d \n"
    //         "\tavg_time = %d \n",
    //         success, ping.timeout_count,
    //         ping.min_time, ping.max_time,
    //         success ? ping.sum_time / success : 0
    // );

    Response_P(PSTR("{\"" D_JSON_PING "\":{\"%d.%d.%d.%d\":{"
                    "\"Reachable\":%s"
                    ",\"Success\":%d"
                    ",\"Timeout\":%d"
                    ",\"MinTime\":%d"
                    ",\"MaxTime\":%d"
                    ",\"AvgTime\":%d"
                    "}}}"),
                    ip & 0xFF, (ip >> 8) & 0xFF, (ip >> 16) & 0xFF, ip >> 24,
                    success ? "true" : "false",
                    success, ping.timeout_count,
                    ping.min_time, ping.max_time,
                    success ? ping.sum_time / success : 0
                    );
    MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_PING));
    XdrvRulesProcess();
    ping.done = false;
    ping.busy = false;
  }
}

void CmndPing(void) {
  uint32_t count = XdrvMailbox.index;
  IPAddress ip;

  RemoveSpace(XdrvMailbox.data);
  if (count > 8) { count = 8; }

  if (ping.busy) {
    ResponseCmndChar_P(PSTR("Ping busy"));
    return;
  }

  if (WiFi.hostByName(XdrvMailbox.data, ip)) {
    memset(&ping_opt, 0, sizeof(ping_opt));
    memset(&ping, 0, sizeof(ping));
    ping.min_time = UINT32_MAX;

    ping_opt.count = count;
    ping_opt.coarse_time = 1;    // wait 1 second between messages
    ping_opt.ip = ip;

    // callbacks
    ping_opt.recv_function = (ping_recv_function) ping_recv_cb;    // at each response or time-out
    ping_opt.sent_function = (ping_sent_function) ping_sent_cb;    // when all packets have been sent and reveived

    ping.busy = true;
    if (ping_start(&ping_opt)) {
      ResponseCmndDone();
    } else {
      ResponseCmndChar_P(PSTR("Unable to send Ping"));
      ping.busy = false;
    }
  } else {
    ResponseCmndChar_P(PSTR("Unable to resolve IP address"));
  }

}


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv38(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_EVERY_250_MSECOND:
    PingResponsePoll();
    break;
    case FUNC_COMMAND:
    result = DecodeCommand(kPingCommands, PingCommand);
    break;
  }
  return result;
}

#endif // USE_PING
