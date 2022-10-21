/*
  UdpListener.h - webserver for Tasmota

  Copyright (C) 2021  Theo Arends & Stephan Hadinger

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.@
*/

// adapted from:
/*
  UdpContext.h - UDP connection handling on top of lwIP

  Copyright (c) 2014 Ivan Grokhotkov. All rights reserved.
  This file is part of the esp8266 core for Arduino environment.

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

/*
 * This is a stripped down version of Udp handler to avoid overflowing
 * memory when lots of multicast SSDP packets arrive.
 * The pbuf is freed immediately upon arrival of the packet.
 *
 * Packet data are kept in a statically area in RAM and keeps
 * only the <n> first bytes (200 by default) of each packet.
 * The number of packets treated is limited (3 by default), any
 * new packet arriving is dropped.
 *
 * This class does only receiving multicast packets for LWIP2
*/

#ifndef UDPMULTICASTLISTENER_H
#define UDPMULTICASTLISTENER_H

#ifdef ESP8266
// #include <Arduino.h>

extern "C" {
#include <lwip/udp.h>
#include <lwip/igmp.h>
}

template <size_t PACKET_SIZE>
struct UdpPacket {
    IPAddress   srcaddr;
    IPAddress   dstaddr;
    int16_t     srcport;
    netif*      input_netif;
    size_t      len;
    uint8_t     buf[PACKET_SIZE];
};

template <size_t PACKET_SIZE>
class UdpListener
{
public:

    typedef std::function<void(void)> rxhandler_t;

    UdpListener(size_t packet_number)
    : _pcb(0)
    , _packet_number(packet_number)
    , _buffers(nullptr)
    , _udp_packets(0)
    , _udp_ready(false)
    , _udp_index(0)
    {
        _packet_number = packet_number;
        _buffers = new UdpPacket<PACKET_SIZE>[_packet_number];
        _pcb = udp_new();
    }

    ~UdpListener()
    {
        udp_remove(_pcb);
        _pcb = 0;
        delete[] _buffers;
        _buffers = nullptr;
    }

    void reset(void)
    {
        _udp_packets = 0;
        _udp_index = 0;
    }

    bool listen(const IPAddress& addr, uint16_t port)
    {
        if (!_buffers) { return false; }
        udp_recv(_pcb, &_s_recv, (void *) this);
        err_t err = udp_bind(_pcb, addr, port);
        return err == ERR_OK;
    }

    void disconnect()
    {
        udp_disconnect(_pcb);
    }

    bool next()
    {
        if (!_buffers) { return false; }
        if (_udp_packets > 0) {
            if (!_udp_ready) {
                // we just consume the first packet
                _udp_ready = true;
            } else {
                _udp_packets--;
                _udp_index = (_udp_index + 1) % _packet_number;      // advance to next buffer index in ring
                if (_udp_packets == 0) {
                    _udp_ready = false;
                }
            }
        } else {
            _udp_ready = false;
        }
        return _udp_ready;
    }

    UdpPacket<PACKET_SIZE> * read(void)
    {
        if (!_buffers) { return nullptr; }
        if (_udp_ready) {        // we have a packet ready to consume
            return &_buffers[_udp_index];
        } else {
            return nullptr;
        }
    }

private:

    void _recv(udp_pcb *upcb, pbuf *pb,
            const ip_addr_t *srcaddr, u16_t srcport)
    {
        if (!_buffers) { pbuf_free(pb); return; }
        // Serial.printf(">>> _recv: _udp_packets = %d, _udp_index = %d, tot_len = %d\n", _udp_packets, _udp_index, pb->tot_len);
        if (_udp_packets >= _packet_number) {
            // we don't have slots anymore, drop packet
            pbuf_free(pb);
            return;
        }

        uint8_t next_slot = (_udp_index + _udp_packets) % _packet_number;

        size_t packet_len = pb->tot_len;
        if (packet_len > PACKET_SIZE) { packet_len = PACKET_SIZE; }

        uint8_t * dst = &_buffers[next_slot].buf[0];
        void* buf = pbuf_get_contiguous(pb, dst, PACKET_SIZE, packet_len, 0);
        if (buf) {

            if (buf != dst)
                memcpy(dst, buf, packet_len);
            _buffers[next_slot].len = packet_len;

            _buffers[next_slot].srcaddr = srcaddr;
            _buffers[next_slot].dstaddr = ip_current_dest_addr();
            _buffers[next_slot].srcport = srcport;
            _buffers[next_slot].input_netif = ip_current_input_netif();
            _udp_packets++;            // we have one packet ready
        }
        pbuf_free(pb);      // free memory immediately
    }

    static void _s_recv(void *arg,
            udp_pcb *upcb, pbuf *p,
            const ip_addr_t *srcaddr, u16_t srcport)
    {
        reinterpret_cast<UdpListener*>(arg)->_recv(upcb, p, srcaddr, srcport);
    }

private:
    udp_pcb* _pcb;
    uint8_t _packet_number;

    UdpPacket<PACKET_SIZE> *   _buffers;

    // how many packets are ready.
    int8_t  _udp_packets;               // number of udp packets ready to consume
    bool    _udp_ready;          // is a packet currenlty consumed after a call to next()
    // ring buffer ranges from 0..(_packet_number-1)
    int8_t  _udp_index;                 // current index in the ring buffer
};

#endif // ESP8266
#endif //UDPMULTICASTLISTENER_H