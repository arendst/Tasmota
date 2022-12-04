/*
 *  Udp.cpp: Library to send/receive UDP packets.
 *
 * NOTE: UDP is fast, but has some important limitations (thanks to Warren Gray for mentioning these)
 * 1) UDP does not guarantee the order in which assembled UDP packets are received. This
 * might not happen often in practice, but in larger network topologies, a UDP
 * packet can be received out of sequence.
 * 2) UDP does not guard against lost packets - so packets *can* disappear without the sender being
 * aware of it. Again, this may not be a concern in practice on small local networks.
 * For more information, see http://www.cafeaulait.org/course/week12/35.html
 *
 * MIT License:
 * Copyright (c) 2008 Bjoern Hartmann
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * bjoern@cs.stanford.edu 12/30/2008
 */

#ifndef _WIFIUDP46_H_
#define _WIFIUDP46_H_

#include <Arduino.h>
#include <Udp46.h>
#include <cbuf.h>

class WiFiUDP46 : public UDP46 {
private:
  int udp_server;
  IPAddress46 multicast_ip;
  IPAddress46 remote_ip;
  uint16_t server_port;
  uint16_t remote_port;
  char * tx_buffer;
  size_t tx_buffer_len;
  cbuf * rx_buffer;
public:
  WiFiUDP46();
  ~WiFiUDP46();
  uint8_t begin(IPAddress46 a, uint16_t p);
  uint8_t begin(uint16_t p);
  uint8_t beginMulticast(IPAddress46 a, uint16_t p);
  void stop();
  int beginMulticastPacket();
  int beginPacket();
  int beginPacket(IPAddress46 ip, uint16_t port);
  int beginPacket(const char *host, uint16_t port);
  int endPacket();
  size_t write(uint8_t);
  size_t write(const uint8_t *buffer, size_t size);
  int parsePacket();
  int available();
  int read();
  int read(unsigned char* buffer, size_t len);
  int read(char* buffer, size_t len);
  int peek();
  void flush();
  IPAddress46 remoteIP();
  uint16_t remotePort();
};

#endif /* _WIFIUDP46_H_ */
