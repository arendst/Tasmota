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

// GCC added the __atomic built-ins in 4.7; Clang exposes them through its
// GNU-compatibility layer. Other C++11 compilers use std::atomic instead.
#if defined(__clang__)
#if __has_builtin(__atomic_load_n) && __has_builtin(__atomic_store_n)
#define UDPLISTENER_USE_GNU_ATOMICS 1
#endif
#elif defined(__GNUC__) && \
      ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)))
#define UDPLISTENER_USE_GNU_ATOMICS 1
#endif

#ifndef UDPLISTENER_USE_GNU_ATOMICS
#define UDPLISTENER_USE_GNU_ATOMICS 0
#include <atomic>
#endif

template <size_t PACKET_SIZE>
struct UdpPacket {
    IPAddress   srcaddr;
    IPAddress   dstaddr;
    uint16_t    srcport;        // unsigned, UDP source ports can exceed 32767
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
    , _head(0)
    , _tail(0)
    , _udp_ready(false)
    {
        _buffers = new UdpPacket<PACKET_SIZE>[_packet_number];
        _pcb = udp_new();           // may return nullptr when out of memory
    }

    ~UdpListener()
    {
        if (_pcb != nullptr) {
            udp_recv(_pcb, nullptr, nullptr);   // no callback into an object being destroyed
            udp_remove(_pcb);
            _pcb = 0;
        }
        delete[] _buffers;
        _buffers = nullptr;
    }

    void reset(void)
    {
        _udp_ready = false;
        uint32_t head = _load_acquire(&_head);
        _store_release(&_tail, head);  // drop all pending packets
    }

    bool listen(const IPAddress& addr, uint16_t port)
    {
        if (!_buffers || (_pcb == nullptr)) { return false; }
        udp_recv(_pcb, &_s_recv, (void *) this);
        err_t err = udp_bind(_pcb, addr, port);
        return err == ERR_OK;
    }

    void disconnect()
    {
        if (_pcb != nullptr) {
            udp_disconnect(_pcb);
        }
    }

    // Release the packet returned by the previous `read()` and make the next one current.
    // Returns true when a packet is available for `read()`.
    bool next()
    {
        if (!_buffers) { return false; }
        if (_udp_ready) {
            _udp_ready = false;
            uint32_t tail = _load_relaxed(&_tail);
            _store_release(&_tail, _incr(tail));
        }
        _udp_ready = (_available() > 0);
        return _udp_ready;
    }

    UdpPacket<PACKET_SIZE> * read(void)
    {
        if (!_buffers) { return nullptr; }
        if (_udp_ready) {        // we have a packet ready to consume
            uint32_t tail = _load_relaxed(&_tail);
            return &_buffers[_slot(tail)];
        } else {
            return nullptr;
        }
    }

private:

#if UDPLISTENER_USE_GNU_ATOMICS
    typedef uint32_t atomic_counter_t;

    static uint32_t _load_acquire(const atomic_counter_t *counter)
    {
        return __atomic_load_n(counter, __ATOMIC_ACQUIRE);
    }

    static uint32_t _load_relaxed(const atomic_counter_t *counter)
    {
        return __atomic_load_n(counter, __ATOMIC_RELAXED);
    }

    static void _store_release(atomic_counter_t *counter, uint32_t value)
    {
        __atomic_store_n(counter, value, __ATOMIC_RELEASE);
    }
#else
    typedef std::atomic<uint32_t> atomic_counter_t;

    static uint32_t _load_acquire(const atomic_counter_t *counter)
    {
        return counter->load(std::memory_order_acquire);
    }

    static uint32_t _load_relaxed(const atomic_counter_t *counter)
    {
        return counter->load(std::memory_order_relaxed);
    }

    static void _store_release(atomic_counter_t *counter, uint32_t value)
    {
        counter->store(value, std::memory_order_release);
    }
#endif

    // The ring counters run in [0..2*_packet_number), the extra bit distinguishes
    // a full ring from an empty one without giving up a slot.
    uint32_t _counter_modulo(void) const { return (uint32_t)_packet_number * 2; }

    uint32_t _incr(uint32_t counter) const
    {
        counter++;
        return (counter >= _counter_modulo()) ? 0 : counter;
    }

    uint8_t _slot(uint32_t counter) const
    {
        return (counter >= _packet_number) ? (counter - _packet_number) : counter;
    }

    // Packets waiting, including the one currently checked out by `read()`.
    // Acquire loads pair with the producer and consumer release stores, making packet
    // contents visible before consumption and consumption complete before slot reuse.
    uint32_t _available(void) const
    {
        uint32_t head = _load_acquire(&_head);
        uint32_t tail = _load_acquire(&_tail);
        return (head >= tail) ? (head - tail) : (head + _counter_modulo() - tail);
    }

    void _recv(udp_pcb *upcb, pbuf *pb,
            const ip_addr_t *srcaddr, u16_t srcport)
    {
        if (!_buffers) { pbuf_free(pb); return; }
        // Serial.printf(">>> _recv: _available() = %d, tot_len = %d\n", _available(), pb->tot_len);
        if (_available() >= _packet_number) {
            // we don't have slots anymore, drop packet
            pbuf_free(pb);
            return;
        }

        uint32_t head = _load_relaxed(&_head);
        uint8_t next_slot = _slot(head);

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
            _store_release(&_head, _incr(head));
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

    // Single-producer / single-consumer ring. `_head` is written only by `_recv()`, which
    // runs in the lwIP context and can preempt the Arduino loop, `_tail` only by the
    // consumer. Acquire/release atomics order packet access across both contexts.
    atomic_counter_t _head;            // next slot to be filled by the producer
    atomic_counter_t _tail;            // oldest slot not yet released by the consumer
    bool    _udp_ready;          // is a packet currently consumed after a call to next()
};

#undef UDPLISTENER_USE_GNU_ATOMICS

#endif // ESP8266
#endif //UDPMULTICASTLISTENER_H