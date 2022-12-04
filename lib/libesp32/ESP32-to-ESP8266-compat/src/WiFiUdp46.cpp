/*
  Udp.cpp - UDP class for Raspberry Pi
  Copyright (c) 2016 Hristo Gochkov  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "WiFiUdp46.h"
#include <lwip/sockets.h>
#include <lwip/netdb.h>
#include <errno.h>

#undef write
#undef read

// Tasmota Logging
extern void AddLog(uint32_t loglevel, PGM_P formatP, ...);
enum LoggingLevels {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE};

WiFiUDP46::WiFiUDP46()
: udp_server(-1)
, server_port(0)
, remote_port(0)
, tx_buffer(0)
, tx_buffer_len(0)
, rx_buffer(0)
{}

WiFiUDP46::~WiFiUDP46(){
   stop();
}

uint8_t WiFiUDP46::begin(IPAddress46 address, uint16_t port){
  stop();
  server_port = port;

  tx_buffer = new char[1460];
  if(!tx_buffer){
    log_e("could not create tx buffer: %d", errno);
    return 0;
  }

#if LWIP_IPV6
  if ((udp_server=socket(AF_INET6, SOCK_DGRAM, 0)) == -1){
#else
  if ((udp_server=socket(AF_INET, SOCK_DGRAM, 0)) == -1){
#endif
    log_e("could not create socket: %d", errno);
    return 0;
  }

  // AddLog(LOG_LEVEL_DEBUG, "WiFiUDP46::begin socket called");
  int yes = 1;
  if (setsockopt(udp_server,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes)) < 0) {
      log_e("could not set socket option: %d", errno);
      stop();
      return 0;
  }

  //AddLog(LOG_LEVEL_DEBUG, "WiFiUDP46::begin setsockopt called");

  struct sockaddr* sock_addr = NULL;
  size_t sock_size = 0;
  struct sockaddr_in addr;
#if LWIP_IPV6
  struct sockaddr_in6 addr6;
  if (address.isV6()) {
    // AddLog(LOG_LEVEL_DEBUG, "WiFiUDP46::begin set IPv6");
    memset((char *) &addr6, 0, sizeof(sockaddr_in6));
    addr6.sin6_len = sizeof(sockaddr_in6);
    addr6.sin6_family = AF_INET6;
    addr6.sin6_port = htons(server_port);
    addr6.sin6_addr = *(in6_addr*)(ip_addr_t*)address;
    addr6.sin6_addr = in6addr_any;
    addr6.sin6_flowinfo = 0;
    sock_addr = (struct sockaddr*)&addr6;
    sock_size = sizeof(sockaddr_in6);

    // AddLog(LOG_LEVEL_DEBUG, "SOCK_ADDR_TYPE_MATCH(name, sock)=%i", SOCK_ADDR_TYPE_MATCH(sock_addr, sock_size));
  } else
#endif
  if (1) {
    memset((char *) &addr, 0, sizeof(sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(server_port);
    addr.sin_addr.s_addr = (in_addr_t)address;
    sock_addr = (struct sockaddr*)&addr;
    sock_size = sizeof(sockaddr_in);
  }
  //AddLog(LOG_LEVEL_DEBUG, "WiFiUDP46::begin udp_server=%p sock_addr=%p sock_size=%i", udp_server, sock_addr, sock_size);
  if(bind(udp_server , sock_addr, sock_size) == -1){
    AddLog(LOG_LEVEL_DEBUG, "WIF: WiFiUDP46::begin bind error=%o", errno);
    log_e("could not bind socket: %d", errno);
    stop();
    return 0;
  }
  fcntl(udp_server, F_SETFL, O_NONBLOCK);
  return 1;
}

uint8_t WiFiUDP46::begin(uint16_t p){
  return begin(IPAddress46(), p);
}

uint8_t WiFiUDP46::beginMulticast(IPAddress46 a, uint16_t p){
  if(begin(IPAddress46(), p)){
    if(!ip_addr_isany((ip_addr_t*)a)){
      struct ip_mreq mreq;
      mreq.imr_multiaddr.s_addr = (in_addr_t)a;
      mreq.imr_interface.s_addr = INADDR_ANY;
      if (setsockopt(udp_server, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
          log_e("could not join igmp: %d", errno);
          stop();
          return 0;
      }
      multicast_ip = a;
    }
    return 1;
  }
  return 0;
}

void WiFiUDP46::stop(){
  if(tx_buffer){
    delete[] tx_buffer;
    tx_buffer = NULL;
  }
  tx_buffer_len = 0;
  if(rx_buffer){
    cbuf *b = rx_buffer;
    rx_buffer = NULL;
    delete b;
  }
  if(udp_server == -1)
    return;
  if(!ip_addr_isany((ip_addr_t*)multicast_ip)){
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = (in_addr_t)multicast_ip;
    mreq.imr_interface.s_addr = (in_addr_t)0;
#if LWIP_IPV6
    setsockopt(udp_server, IPPROTO_IPV6, IP_DROP_MEMBERSHIP, &mreq, sizeof(mreq));
#else
    setsockopt(udp_server, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(mreq));
#endif
    multicast_ip = IPAddress46(INADDR_ANY);
  }
  close(udp_server);
  udp_server = -1;
}

int WiFiUDP46::beginMulticastPacket(){
  if(!server_port || multicast_ip == IPAddress46(INADDR_ANY))
    return 0;
  remote_ip = multicast_ip;
  remote_port = server_port;
  return beginPacket();
}

int WiFiUDP46::beginPacket(){
  if(!remote_port)
    return 0;

  // allocate tx_buffer if is necessary
  if(!tx_buffer){
    tx_buffer = new char[1460];
    if(!tx_buffer){
      log_e("could not create tx buffer: %d", errno);
      return 0;
    }
  }

  tx_buffer_len = 0;

  // check whereas socket is already open
  if (udp_server != -1)
    return 1;

  if ((udp_server=socket(AF_INET, SOCK_DGRAM, 0)) == -1){
    log_e("could not create socket: %d", errno);
    return 0;
  }

  fcntl(udp_server, F_SETFL, O_NONBLOCK);

  return 1;
}

int WiFiUDP46::beginPacket(IPAddress46 ip, uint16_t port){
  remote_ip = ip;
  remote_port = port;
  return beginPacket();
}

int WiFiUDP46::beginPacket(const char *host, uint16_t port){
  struct hostent *server;
  server = gethostbyname(host);
  if (server == NULL){
    log_e("could not get host from dns: %d", errno);
    return 0;
  }
  return beginPacket(IPAddress46((const uint8_t *)(server->h_addr_list[0])), port);
}

int WiFiUDP46::endPacket(){
  if (remote_ip.isV4()) {
    struct sockaddr_in recipient;
    recipient.sin_len = sizeof(sockaddr_in);
    recipient.sin_addr.s_addr = (uint32_t)remote_ip;
    recipient.sin_family = AF_INET;
    recipient.sin_port = htons(remote_port);
    int sent = sendto(udp_server, tx_buffer, tx_buffer_len, 0, (struct sockaddr*) &recipient, sizeof(recipient));
    if(sent < 0){
      log_e("could not send data: %d", errno);
      return 0;
    }
  } else {
    struct sockaddr_in6 recipient;
    recipient.sin6_len = sizeof(sockaddr_in6);
    recipient.sin6_flowinfo = 0;
    recipient.sin6_addr = *(in6_addr*)(ip_addr_t*)remote_ip;
    // recipient.sin6_family = AF_INET6;
    recipient.sin6_port = htons(remote_port);
    int sent = sendto(udp_server, tx_buffer, tx_buffer_len, 0, (struct sockaddr*) &recipient, sizeof(recipient));
    if(sent < 0){
      log_e("could not send data: %d", errno);
      return 0;
    }
  }
  return 1;
}

size_t WiFiUDP46::write(uint8_t data){
  if(tx_buffer_len == 1460){
    endPacket();
    tx_buffer_len = 0;
  }
  tx_buffer[tx_buffer_len++] = data;
  return 1;
}

size_t WiFiUDP46::write(const uint8_t *buffer, size_t size){
  size_t i;
  for(i=0;i<size;i++)
    write(buffer[i]);
  return i;
}

int WiFiUDP46::parsePacket(){
  if(rx_buffer)
    return 0;
  struct sockaddr_in si_other;
  int slen = sizeof(si_other) , len;
  char * buf = new char[1460];
  if(!buf){
    return 0;
  }
  if ((len = recvfrom(udp_server, buf, 1460, MSG_DONTWAIT, (struct sockaddr *) &si_other, (socklen_t *)&slen)) == -1){
    delete[] buf;
    if(errno == EWOULDBLOCK){
      return 0;
    }
    log_e("could not receive data: %d", errno);
    return 0;
  }
  remote_ip = IPAddress46(si_other.sin_addr.s_addr);
  remote_port = ntohs(si_other.sin_port);
  if (len > 0) {
    rx_buffer = new cbuf(len);
    rx_buffer->write(buf, len);
  }
  delete[] buf;
  return len;
}

int WiFiUDP46::available(){
  if(!rx_buffer) return 0;
  return rx_buffer->available();
}

int WiFiUDP46::read(){
  if(!rx_buffer) return -1;
  int out = rx_buffer->read();
  if(!rx_buffer->available()){
    cbuf *b = rx_buffer;
    rx_buffer = 0;
    delete b;
  }
  return out;
}

int WiFiUDP46::read(unsigned char* buffer, size_t len){
  return read((char *)buffer, len);
}

int WiFiUDP46::read(char* buffer, size_t len){
  if(!rx_buffer) return 0;
  int out = rx_buffer->read(buffer, len);
  if(!rx_buffer->available()){
    cbuf *b = rx_buffer;
    rx_buffer = 0;
    delete b;
  }
  return out;
}

int WiFiUDP46::peek(){
  if(!rx_buffer) return -1;
  return rx_buffer->peek();
}

void WiFiUDP46::flush(){
  if(!rx_buffer) return;
  cbuf *b = rx_buffer;
  rx_buffer = 0;
  delete b;
}

IPAddress46 WiFiUDP46::remoteIP(){
  return remote_ip;
}

uint16_t WiFiUDP46::remotePort(){
  return remote_port;
}
