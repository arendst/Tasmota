/*
  IPAddress46.h - IPv6 support for ESP32

  This class is copied from ESP8266 Arduino framework and provides
  temporary support for IPv6 on ESP32.

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

#ifndef __IPADDRESS46_H
#define __IPADDRESS46_H

#include <Printable.h>
#include <WString.h>
#include <lwip/netif.h>

class IPAddress46: public Printable {
    private:

        ip_addr_t _ip;

        // Access the raw byte array containing the address.  Because this returns a pointer
        // to the internal structure rather than a copy of the address this function should only
        // be used when you know that the usage of the returned uint8_t* will be transient and not
        // stored.
        uint8_t* raw_address() {
            return reinterpret_cast<uint8_t*>(&v4());
        }
        const uint8_t* raw_address() const {
            return reinterpret_cast<const uint8_t*>(&v4());
        }

        void ctor32 (uint32_t);

    public:
        // Constructors
        IPAddress46();
        IPAddress46(const IPAddress46& from);
        IPAddress46(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet);
        IPAddress46(uint32_t address) { ctor32(address); }
        IPAddress46(unsigned long address) { ctor32(address); }
        IPAddress46(int address) { ctor32(address); }
        IPAddress46(const uint8_t *address);

        bool fromString(const char *address);
        bool fromString(const String &address) { return fromString(address.c_str()); }

        // Overloaded cast operator to allow IPAddress46 objects to be used where a pointer
        // to a four-byte uint8_t array is expected
        operator uint32_t() const { return isV4()? v4(): (uint32_t)0; }
        operator uint32_t()       { return isV4()? v4(): (uint32_t)0; }

        bool isSet () const;
        operator bool () const { return isSet(); } // <-
        operator bool ()       { return isSet(); } // <- both are needed

        // generic IPv4 wrapper to uint32-view like arduino loves to see it
        const uint32_t& v4() const { return ip_2_ip4(&_ip)->addr; } // for raw_address(const)
              uint32_t& v4()       { return ip_2_ip4(&_ip)->addr; }

        bool operator==(const IPAddress46& addr) const {
            return ip_addr_cmp(&_ip, &addr._ip);
        }
        bool operator!=(const IPAddress46& addr) const {
            return !ip_addr_cmp(&_ip, &addr._ip);
        }
        bool operator==(uint32_t addr) const {
            return isV4() && v4() == addr;
        }
        bool operator==(unsigned long addr) const {
            return isV4() && v4() == (uint32_t)addr;
        }
        bool operator!=(uint32_t addr) const {
            return !(isV4() && v4() == addr);
        }
        bool operator!=(unsigned long addr) const {
            return isV4() && v4() != (uint32_t)addr;
        }
        bool operator==(const uint8_t* addr) const;

        int operator>>(int n) const {
            return isV4()? v4() >> n: 0;
        }

        // Overloaded index operator to allow getting and setting individual octets of the address
        uint8_t operator[](int index) const {
            return isV4()? *(raw_address() + index): 0;
        }
        uint8_t& operator[](int index) {
            setV4();
            return *(raw_address() + index);
        }

        // Overloaded copy operators to allow initialisation of IPAddress46 objects from other types
        IPAddress46& operator=(const uint8_t *address);
        IPAddress46& operator=(uint32_t address);
        IPAddress46& operator=(const IPAddress46&) = default;

        virtual size_t printTo(Print& p) const;
        String toString() const;

        void clear();

        /*
                check if input string(arg) is a valid IPV4 address or not.
                return true on valid.
                return false on invalid.
        */
        static bool isValid(const String& arg);
        static bool isValid(const char* arg);

        friend class EthernetClass;
        friend class UDP;
        friend class Client;
        friend class Server;
        friend class DhcpClass;
        friend class DNSClient;

        operator       ip_addr_t () const { return  _ip; }
        operator const ip_addr_t*() const { return &_ip; }
        operator       ip_addr_t*()       { return &_ip; }

        bool isV4() const { return IP_IS_V4_VAL(_ip); }
        void setV4() { IP_SET_TYPE_VAL(_ip, IPADDR_TYPE_V4); }

        bool isLocal () const { return ip_addr_islinklocal(&_ip); }

#if LWIP_IPV6
        IPAddress46(const ip_addr_t& lwip_addr) { ip_addr_copy(_ip, lwip_addr); }
        IPAddress46(const ip_addr_t* lwip_addr) { ip_addr_copy(_ip, *lwip_addr); }

        IPAddress46& operator=(const ip_addr_t& lwip_addr) { ip_addr_copy(_ip, lwip_addr); return *this; }
        IPAddress46& operator=(const ip_addr_t* lwip_addr) { ip_addr_copy(_ip, *lwip_addr); return *this; }

        uint16_t* raw6()
        {
            setV6();
            return reinterpret_cast<uint16_t*>(ip_2_ip6(&_ip));
        }

        const uint16_t* raw6() const
        {
            return isV6()? reinterpret_cast<const uint16_t*>(ip_2_ip6(&_ip)): nullptr;
        }

        // when not IPv6, ip_addr_t == ip4_addr_t so this one would be ambiguous
        // required otherwise
        operator const ip4_addr_t*() const { return isV4()? ip_2_ip4(&_ip): nullptr; }

        bool isV6() const { return IP_IS_V6_VAL(_ip); }
        void setV6() { IP_SET_TYPE_VAL(_ip, IPADDR_TYPE_V6); }

    protected:
        bool fromString6(const char *address);

#else

        // allow portable code when IPv6 is not enabled

        uint16_t* raw6() { return nullptr; }
        const uint16_t* raw6() const { return nullptr; }
        bool isV6() const { return false; }
        void setV6() { }

#endif

    protected:
        bool fromString4(const char *address);
};

#endif // __IPADDRESS46_H
