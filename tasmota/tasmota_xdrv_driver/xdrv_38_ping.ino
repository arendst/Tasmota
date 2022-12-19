/*
  xdrv_38_ping.ino - support for ICMP Ping

  Copyright (C) 2021  Theo Arends and Stephan Hadinger

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

#include "lwip/icmp.h"
#include "lwip/inet_chksum.h"
#include "lwip/raw.h"
#include "lwip/timeouts.h"

const char kPingCommands[] PROGMEM =  "|"    // no prefix
  D_CMND_PING
  ;

void (* const PingCommand[])(void) PROGMEM = {
  &CmndPing,
  };

extern "C" {

  extern uint32 system_relative_time(uint32 time);
  extern void ets_bzero(void *s, size_t n);

  const uint16_t Ping_ID = 0xAFAF;          // PING packet ID
  const size_t   Ping_data_size = 32;       // default packet size
  const uint32_t Ping_timeout_ms = 1000;    // default time-out of 1 second, which is enough for LAN/WIFI
  const uint32_t Ping_coarse = 1000;        // interval between sending packets, 1 packet every second

  typedef struct Ping_t {
    ip_addr_t   ip;                 // target IPv4 address
    Ping_t      *next;              // next object in linked list
    uint16_t    seq_num;            // next sequence number
    uint16_t    seqno;              // reject a packet already received
    uint8_t     success_count;      // sucessful responses received
    uint8_t     timeout_count;      // time-outs (no responses)
    uint8_t     to_send_count;      // number of packets remaining to send
    uint32_t    ping_time_sent;     // timestamp when the packet was sent
    uint32_t    min_time;           // minimum time in ms for a successful response
    uint32_t    max_time;           // maximum time in ms for a successful response
    uint32_t    sum_time;           // cumulated time in ms for all successful responses (used to compute the average)
    bool        done;               // indicates the ping campaign is finished
    bool        fast;               // fast mode, i.e. stop pings when first successful response
    String      hostname;           // original hostname before convertion to IP address
  } Ping_t;

  // globals
  Ping_t          *ping_head = nullptr;     // head of the Linked List for ping objects
  struct raw_pcb  *t_ping_pcb = nullptr;    // registered with first ping, deregistered after last ping, the same pcb is used for all packets
#ifdef USE_IPV6
  struct raw_pcb  *t_ping6_pcb = nullptr;   // IPv6 version, registered with first ping, deregistered after last ping, the same pcb is used for all packets
#endif // USE_IPV6

  // ================================================================================
  // Find the Ping object indexed by IP address
  // ================================================================================
  //
  // find the ping structure corresponding to the specified IP, or nullptr if not found
  //
  Ping_t ICACHE_FLASH_ATTR * t_ping_find(const ip_addr_t *ip) {
    Ping_t *ping = ping_head;
    while (ping != nullptr) {
      if (ip_addr_cmp(&ping->ip, ip)) {
        return ping;
      }
      ping = ping->next;
    }
    return nullptr;
  }

  // ================================================================================
  // Timer called a packet response is in time-out
  // ================================================================================
  //
  // called after the ICMP timeout occured
  // we never received the packet, increase the timeout count
  //
  void ICACHE_FLASH_ATTR t_ping_timeout(void* arg) {
    Ping_t *ping = (Ping_t*) arg;
    ping->timeout_count++;
  }

  // ================================================================================
  // Send ICMP packet
  // ================================================================================
  // Prepare a echo ICMP request
  //
  void t_ping_prepare_echo(struct icmp_echo_hdr *iecho, uint16_t len, Ping_t *ping) {
    size_t data_len = len - sizeof(struct icmp_echo_hdr);

    ICMPH_TYPE_SET(iecho, ICMP_ECHO);
    ICMPH_CODE_SET(iecho, 0);
    iecho->chksum = 0;
    iecho->id     = Ping_ID;
    ping->seq_num++;
    if (ping->seq_num == 0x7fff) { ping->seq_num = 0; }

    iecho->seqno  = htons(ping->seq_num);      // TODO

    /* fill the additional data buffer with some data */
    for (uint32_t i = 0; i < data_len; i++) {
      ((char*)iecho)[sizeof(struct icmp_echo_hdr) + i] = (char)i;
    }

    iecho->chksum = inet_chksum(iecho, len);
  }

#ifdef USE_IPV6
  // Prepare a echo ICMP6 request
  //
  void t_ping_prepare_echo6(struct icmp6_echo_hdr *iecho6, uint16_t len, Ping_t *ping) {
    // TODO
    size_t data_len = len - sizeof(struct icmp6_echo_hdr);

    iecho6->type = ICMP6_TYPE_EREQ;
    iecho6->code = 0;
    iecho6->chksum = 0;
    iecho6->id     = Ping_ID;
    ping->seq_num++;
    if (ping->seq_num == 0x7fff) { ping->seq_num = 0; }

    iecho6->seqno  = htons(ping->seq_num);

    /* fill the additional data buffer with some data */
    for (uint32_t i = 0; i < data_len; i++) {
      ((char*)iecho6)[sizeof(struct icmp6_echo_hdr) + i] = (char)i;
    }
    // checksum is calculated by lwip
  }
#endif // USE_IPV6

  //
  // send the ICMP packet
  //
  void ICACHE_FLASH_ATTR t_ping_send(struct raw_pcb *raw, Ping_t *ping) {
    struct pbuf *p;
    uint16_t ping_size = sizeof(struct icmp_echo_hdr) + Ping_data_size;

    ping->ping_time_sent = millis();
    p = pbuf_alloc(PBUF_IP, ping_size, PBUF_RAM);
    if (!p) { return; }
    if ((p->len == p->tot_len) && (p->next == nullptr)) {

#ifdef USE_IPV6
      // different format for IPv4 and IPv6 packets
      if (IP_IS_V6_VAL(ping->ip)) {
        // IPv6
        struct icmp6_echo_hdr *iecho6;
        iecho6 = (struct icmp6_echo_hdr *) p->payload;
        t_ping_prepare_echo6(iecho6, ping_size, ping);
        // set parameters for checksum handling
        t_ping6_pcb->chksum_reqd = 1;
        t_ping6_pcb->chksum_offset = offsetof(icmp6_echo_hdr, chksum);

        // AddLog(LOG_LEVEL_DEBUG, "PNG: sending ICMP6(%i-%i)=%*_H", p->len, ping_size, p->len, p->payload);
        raw_sendto(t_ping6_pcb, p, &ping->ip);
        // AddLog(LOG_LEVEL_DEBUG, "PNG: sending ICMP6(%i-%i)=%*_H", p->len, ping_size, p->len, p->payload);
      } else
#endif // USE_IPV6
      {
        // IPv4
        struct icmp_echo_hdr *iecho;
        iecho = (struct icmp_echo_hdr *) p->payload;
        t_ping_prepare_echo(iecho, ping_size, ping);
        raw_sendto(t_ping_pcb, p, &ping->ip);
        // AddLog(LOG_LEVEL_DEBUG, "PNG: sending ICMP4(%i-%i)=%*_H", p->len, ping_size, p->len, p->payload);
      }
    }
    pbuf_free(p);
  }

  // ================================================================================
  // Timer called when it's time to send next packet, of when finished
  // ================================================================================
  // this timer is called every x seconds to send a new packet, whatever happened to the previous packet
  static void ICACHE_FLASH_ATTR t_ping_coarse_tmr(void *arg) {
    Ping_t *ping = (Ping_t*) arg;
    if (ping->to_send_count > 0) {
      ping->to_send_count--;
      // have we sent all packets?
      t_ping_send(t_ping_pcb, ping);    // ICMP can also send ICMP6

      sys_timeout(Ping_timeout_ms, t_ping_timeout, ping);
      sys_timeout(Ping_coarse, t_ping_coarse_tmr, ping);
    } else {
      sys_untimeout(t_ping_coarse_tmr, ping);
      ping->done = true;
    }
  }

  // ================================================================================
  // Callback: a packet response was received
  // ================================================================================
  //
  // Reveived packet
  //
  static uint8_t ICACHE_FLASH_ATTR t_ping_recv(void *arg, struct raw_pcb *pcb, struct pbuf *p, const ip_addr_t *addr) {
    // AddLog(LOG_LEVEL_DEBUG, "PNG: from %s pub(%i)=%*_H", IPAddress(*addr).toString().c_str(), p->len, p->len, p->payload);
    Ping_t *ping = t_ping_find(addr);

    if (nullptr == ping) {    // unknown source address
      return 0;               // don't eat the packet and ignore it
    }

    size_t pbuf_header_len = PBUF_TRANSPORT_HLEN;
    bool ipv6 = false;
#ifdef USE_IPV6
    if (pcb == t_ping6_pcb) {
      pbuf_header_len = PBUF_IP_HLEN;
      ipv6 = true;
    }
#endif // USE_IPV6
    if (pbuf_header(p, -pbuf_header_len)==0) {
      // AddLog(LOG_LEVEL_DEBUG, "PNG: received(%i)=%*_H", p->len, p->len, p->payload);
      struct icmp_echo_hdr *iecho;
      iecho = (struct icmp_echo_hdr *)p->payload;

      uint8_t icmp_resp_type = ICMP_ER;
#ifdef USE_IPV6
      icmp_resp_type = (ipv6 ? ICMP6_TYPE_EREP : ICMP_ER);
#endif // USE_IPV6
      if ((iecho->id == Ping_ID) && (iecho->seqno == htons(ping->seq_num)) && iecho->type == icmp_resp_type) {

        if (iecho->seqno != ping->seqno){   // debounce already received packet
          /* do some ping result processing */
          sys_untimeout(t_ping_timeout, ping);      // remove time-out handler
          uint32_t delay = millis() - ping->ping_time_sent;

          ping->sum_time += delay;
          if (delay < ping->min_time) { ping->min_time = delay; }
          if (delay > ping->max_time) { ping->max_time = delay; }

          ping->success_count++;
          ping->seqno = iecho->seqno;
          if (ping->fast) {   // if fast mode, abort further pings when first successful response is received
            sys_untimeout(t_ping_coarse_tmr, ping);
            ping->done = true;
            ping->to_send_count = 0;
          }
        }

        pbuf_free(p);
        return 1; /* eat the packet */
      }
    }

    return 0; /* don't eat the packet */
  }

  // ================================================================================
  // Internal structure PCB management
  // ================================================================================
  // we are going to send a packet, make sure pcb is initialized
  void t_ping_register_pcb(void) {
    if (nullptr == t_ping_pcb) {
      t_ping_pcb = raw_new(IP_PROTO_ICMP);

      raw_recv(t_ping_pcb, t_ping_recv, nullptr);    // we cannot register data structure here as we can only register one
      raw_bind(t_ping_pcb, IP4_ADDR_ANY);
    }
#ifdef USE_IPV6
    if (nullptr == t_ping6_pcb) {
      t_ping6_pcb = raw_new(IP6_NEXTH_ICMP6);

      raw_recv(t_ping6_pcb, t_ping_recv, nullptr);    // we cannot register data structure here as we can only register one
      raw_bind(t_ping6_pcb, IP6_ADDR_ANY);
    }
#endif // USE_IPV6
  }

  // we have finsihed a ping series, deallocated if no more ongoing
  void t_ping_deregister_pcb(void) {
    if (nullptr == ping_head) {         // deregister only if no ping is flying
      raw_remove(t_ping_pcb);
      t_ping_pcb = nullptr;
#ifdef USE_IPV6
      raw_remove(t_ping6_pcb);
      t_ping6_pcb = nullptr;
#endif // USE_IPV6
    }
  }

  // ================================================================================
  // Start pings
  // ================================================================================
  // returns:
  //  0: OK
  // -1: ping already ongoing for this address
  // -2: unable to resolve address
  int32_t t_ping_start(const char *hostname, uint32_t count) {
    IPAddress ipfull;
    bool host_resolved = WifiHostByName(hostname, ipfull);

    ip_addr_t ip;
    ip_addr_set_any_val(false, ip);
#ifdef USE_IPV6
    ip = (ip_addr_t)ipfull;
#else
    ip_addr_set_ip4_u32_val(ip, (uint32_t)ipfull);
#endif

    // check if pings are already ongoing for this IP
    if (host_resolved && t_ping_find(&ip)) {
      return -1;
    }

    Ping_t *ping = new Ping_t();
    if (0 == count) {
      count = 4;
      ping->fast = true;
    }
    ping->min_time = UINT32_MAX;
    ping->ip = ip;
    ping->to_send_count = count - 1;
    ping->hostname = hostname;

    // add to Linked List from head
    ping->next = ping_head;
    ping_head = ping;         // insert at head

    if (!host_resolved) { // If invalid address, set as completed
      ping->done = true;
      return -2;
    }

    // send
    t_ping_register_pcb();
    t_ping_send(t_ping_pcb, ping);

    // set timers for time-out and cadence
    sys_timeout(Ping_timeout_ms, t_ping_timeout, ping);
    sys_timeout(Ping_coarse, t_ping_coarse_tmr, ping);

    return 0;
  }

}

// Check if any ping requests is completed, and publish the results
void PingResponsePoll(void) {
  Ping_t *ping = ping_head;
  Ping_t **prev_link = &ping_head;      // previous link pointer (used to remove en entry)

  while (ping != nullptr) {
    if (ping->done) {
      uint32_t success = ping->success_count;
      bool resolved = !ip_addr_isany_val(ping->ip);

      if (!resolved) {
        Response_P(PSTR("{\"" D_JSON_PING "\":{\"%s\":{"
                        "\"Reachable\":false"
                        ",\"IP\":\"\""
                        ",\"Success\":false"
                        "}}}"),
                        ping->hostname.c_str()
                        );
      } else {
        Response_P(PSTR("{\"" D_JSON_PING "\":{\"%s\":{"
                        "\"Reachable\":%s"
                        ",\"IP\":\"%s\""
                        ",\"Success\":%d"
                        ",\"Timeout\":%d"
                        ",\"MinTime\":%d"
                        ",\"MaxTime\":%d"
                        ",\"AvgTime\":%d"
                        "}}}"),
                        ping->hostname.c_str(),
                        success ? PSTR("true") : PSTR("false"),
#ifdef USE_IPV6
                        IPAddress(ping->ip).toString().c_str(),
#else
                        IPAddress(ip_addr_get_ip4_u32(&ping->ip)).toString().c_str(),
#endif
                        // ip & 0xFF, (ip >> 8) & 0xFF, (ip >> 16) & 0xFF, ip >> 24,
                        success,
                        ping->timeout_count,
                        success ? ping->min_time : 0,
                        ping->max_time,
                        success ? ping->sum_time / success : 0
                        );
      }
      MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_PING));

      // remove from linked list
      *prev_link = ping->next;
      // don't increment prev_link
      Ping_t *ping_to_delete = ping;
      ping = ping->next;              // move to next before deleting the object
      delete ping_to_delete;          // free memory allocated
    } else {
      prev_link = &ping->next;
      ping = ping->next;
    }
  }
}

/*********************************************************************************************\
 * Ping Command
\*********************************************************************************************/

void CmndPing(void) {
  uint32_t count = XdrvMailbox.index;

  RemoveSpace(XdrvMailbox.data);
  if (count > 10) { count = 8; }   // max 8 seconds

  int32_t res = t_ping_start(XdrvMailbox.data, count);
  if (0 == res) {
    ResponseCmndDone();
  } else if (-1 == res) {
    ResponseCmndChar_P(PSTR("Ping already ongoing for this IP"));
  } else {
    ResponseCmndChar_P(PSTR("Unable to resolve IP address"));
  }
}


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv38(uint32_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_EVERY_250_MSECOND:
    PingResponsePoll();   // TODO
    break;
    case FUNC_COMMAND:
    result = DecodeCommand(kPingCommands, PingCommand);
    break;
  }
  return result;
}

#endif // USE_PING