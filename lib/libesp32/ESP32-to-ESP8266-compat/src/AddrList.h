/*
 AddrList.h - cycle through lwIP netif's ip addresses like a c++ list
 Copyright (c) 2018 david gauchard.  All right reserved.

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
  This class allows to explore all configured IP addresses
  in lwIP netifs, with that kind of c++ loop:

  for (auto a: addrList)
    out.printf("IF='%s' index=%d legacy=%d IPv4=%d local=%d hostname='%s' addr= %s\n",
               a.iface().c_str(),
               a.ifnumber(),
               a.addr().isLegacy(),
               a.addr().isV4(),
               a.addr().isLocal(),
               a.hostname().c_str(),
               a.addr().toString().c_str());

  This loop:

      while (WiFi.status() != WL_CONNECTED()) {
          Serial.print('.');
          delay(500);
      }

  can be replaced by:

      for (bool configured = false; !configured; ) {
          for (auto iface: addrList)
              if ((configured = !iface.addr().isLocal())
                  break;
          Serial.print('.');
          delay(500);
      }

  waiting for an IPv6 global address:

      for (bool configured = false; !configured; ) {
          for (auto iface: addrList)
              if ((configured = (   !iface.addr().isV4()
                                 && !iface.addr().isLocal())))
                  break;
          Serial.print('.');
          delay(500);
      }

  waiting for an IPv6 global address, on a specific interface:

      for (bool configured = false; !configured; ) {
          for (auto iface: addrList)
              if ((configured = (   !iface.addr().isV4()
                                 && !iface.addr().isLocal()
                                 && iface.ifnumber() == STATION_IF)))
                  break;
          Serial.print('.');
          delay(500);
      }
*/

#ifndef __ADDRLIST_H
#define __ADDRLIST_H

#include <IPAddress.h>
#include <lwip/netif.h>

#if LWIP_IPV6
#define IF_NUM_ADDRESSES (1 + LWIP_IPV6_NUM_ADDRESSES)
#else
#define IF_NUM_ADDRESSES (1)
#endif


namespace esp8266
{

namespace AddressListImplementation
{


struct netifWrapper
{
    netifWrapper (netif* netif) : _netif(netif), _num(-1) {}
    netifWrapper (const netifWrapper& o) : _netif(o._netif), _num(o._num) {}

    netifWrapper& operator= (const netifWrapper& o)
    {
        _netif = o._netif;
        _num = o._num;
        return *this;
    }

    bool equal(const netifWrapper& o)
    {
        return _netif == o._netif && (!_netif || _num == o._num);
    }

    // address properties
    class IPAddress4 : public IPAddress
    {
        public:
        bool isV6() const
        {
            return false;
        }
        bool isLocal() const
        {
            return false;
        }
    };
    IPAddress4 addr () const         { return ipFromNetifNum(); }
    bool isLegacy () const          { return _num == 0; }
    //bool isLocal () const           { return addr().isLocal(); }
    bool isV4 () const              { return addr().isV4(); }
    bool isV6 () const              { return !addr().isV4(); }
    String toString() const         { return addr().toString(); }

    // related to legacy address (_num=0, ipv4)
    IPAddress ipv4 () const         { return _netif->ip_addr; }
    IPAddress netmask () const      { return _netif->netmask; }
    IPAddress gw () const           { return _netif->gw; }

    // common to all addresses of this interface
    String ifname () const          { return String(_netif->name[0]) + _netif->name[1]; }
    const char* ifhostname () const { return _netif->hostname?: emptyString.c_str(); }
    const char* ifmac () const      { return (const char*)_netif->hwaddr; }
    int ifnumber () const           { return _netif->num; }
    bool ifUp () const              { return !!(_netif->flags & NETIF_FLAG_UP); }
    const netif* interface () const { return _netif; }

    const ip_addr_t* ipFromNetifNum () const
    {
#if LWIP_IPV6
        return _num ? &_netif->ip6_addr[_num - 1] : &_netif->ip_addr;
#else
        return &_netif->ip_addr;
#endif
    }

    // lwIP interface
    netif* _netif;

    // address index within interface
    // 0: legacy address (IPv4)
    // n>0: (_num-1) is IPv6 index for netif->ip6_addr[]
    int _num;
};


class AddressListIterator
{
public:
    AddressListIterator (const netifWrapper& o) : netIf(o) {}
    AddressListIterator (netif* netif) : netIf(netif)
    {
        // This constructor is called with lwIP's global netif_list, or
        // nullptr.  operator++() is designed to loop through _configured_
        // addresses.  That's why netIf's _num is initialized to -1 to allow
        // returning the first usable address to AddressList::begin().
        (void)operator++();
    }

    const netifWrapper& operator*  () const { return netIf; }
    const netifWrapper* operator-> () const { return &netIf; }

    bool operator== (AddressListIterator& o) { return netIf.equal(*o); }
    bool operator!= (AddressListIterator& o) { return !netIf.equal(*o); }

    AddressListIterator operator++ (int)
    {
        AddressListIterator ret = *this;
        (void)operator++();
        return ret;
    }

    AddressListIterator& operator++ ()
    {
        while (netIf._netif)
        {
            if (++netIf._num == IF_NUM_ADDRESSES)
            {
                // all addresses from current interface were iterated,
                // switching to next interface
                netIf = netifWrapper(netIf._netif->next);
                continue;
            }
            if (!ip_addr_isany(netIf.ipFromNetifNum()))
                // found an initialized address
                break;
        }
        return *this;
    }

    netifWrapper netIf;
};


class AddressList
{
public:
  using const_iterator = const AddressListIterator;

  const_iterator begin () const { return const_iterator(netif_list); }
  const_iterator   end () const { return const_iterator(nullptr); }

};

inline AddressList::const_iterator begin (const AddressList& a) { return a.begin(); }
inline AddressList::const_iterator   end (const AddressList& a) { return a.end(); }


} // AddressListImplementation

} // esp8266

extern AddressList addrList;


#endif
