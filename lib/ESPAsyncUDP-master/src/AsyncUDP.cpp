#include "Arduino.h"
#include "ESPAsyncUDP.h"

extern "C" {
#include "user_interface.h"
#include "lwip/opt.h"
#include "lwip/inet.h"
#include "lwip/udp.h"
#include "lwip/igmp.h"
}

AsyncUDPMessage::AsyncUDPMessage(size_t size)
{
    _index = 0;
    if(size > 1460) {
        size = 1460;
    }
    _size = size;
    _buffer = (uint8_t *)malloc(size);
}

AsyncUDPMessage::~AsyncUDPMessage()
{
    if(_buffer) {
        free(_buffer);
    }
}

size_t AsyncUDPMessage::write(const uint8_t *data, size_t len)
{
    if(_buffer == NULL) {
        return 0;
    }
    size_t s = space();
    if(len > s) {
        len = s;
    }
    memcpy(_buffer + _index, data, len);
    _index += len;
    return len;
}

size_t AsyncUDPMessage::write(uint8_t data)
{
    return write(&data, 1);
}

size_t AsyncUDPMessage::space()
{
    if(_buffer == NULL) {
        return 0;
    }
    return _size - _index;
}

uint8_t * AsyncUDPMessage::data()
{
    return _buffer;
}

size_t AsyncUDPMessage::length()
{
    return _index;
}

void AsyncUDPMessage::flush()
{
    _index = 0;
}


AsyncUDPPacket::AsyncUDPPacket(AsyncUDP *udp, ip_addr_t *localIp, uint16_t localPort, ip_addr_t *remoteIp, uint16_t remotePort, uint8_t *data, size_t len)
{
    _udp = udp;
    _localIp = localIp;
    _localPort = localPort;
    _remoteIp = remoteIp;
    _remotePort = remotePort;
    _data = data;
    _len = len;
}

AsyncUDPPacket::~AsyncUDPPacket()
{

}

uint8_t * AsyncUDPPacket::data()
{
    return _data;
}

size_t AsyncUDPPacket::length()
{
    return _len;
}

IPAddress AsyncUDPPacket::localIP()
{
    return IPAddress(_localIp->addr);
}

uint16_t AsyncUDPPacket::localPort()
{
    return _localPort;
}

IPAddress AsyncUDPPacket::remoteIP()
{
    return IPAddress(_remoteIp->addr);
}

uint16_t AsyncUDPPacket::remotePort()
{
    return _remotePort;
}

bool AsyncUDPPacket::isBroadcast()
{
    return _localIp->addr == 0xFFFFFFFF || _localIp->addr == (uint32_t)(0);
}

bool AsyncUDPPacket::isMulticast()
{
    return ip_addr_ismulticast(_localIp);
}

size_t AsyncUDPPacket::write(const uint8_t *data, size_t len)
{
    return _udp->writeTo(data, len, _remoteIp, _remotePort);
}

size_t AsyncUDPPacket::write(uint8_t data)
{
    return write(&data, 1);
}

size_t AsyncUDPPacket::send(AsyncUDPMessage &message)
{
    return write(message.data(), message.length());
}



AsyncUDP::AsyncUDP()
{
    _pcb = NULL;
    _connected = false;
    _handler = NULL;
}

AsyncUDP::~AsyncUDP()
{
    close();
}

AsyncUDP::operator bool()
{
    return _connected;
}

bool AsyncUDP::connected()
{
    return _connected;
}

void AsyncUDP::onPacket(AuPacketHandlerFunctionWithArg cb, void * arg)
{
    onPacket(std::bind(cb, arg, std::placeholders::_1));
}

void AsyncUDP::onPacket(AuPacketHandlerFunction cb)
{
    _handler = cb;
}

void AsyncUDP::_recv(udp_pcb *upcb, pbuf *pb, ip_addr_t *addr, uint16_t port)
{
    (void)upcb; // its unused, avoid warning
    while(pb != NULL) {
        if(_handler) {
            uint8_t * data = (uint8_t*)((pb)->payload);
            size_t len = pb->len;

            ip_hdr* iphdr = reinterpret_cast<ip_hdr*>(data - UDP_HLEN - IP_HLEN);
            ip_addr_t daddr;
            daddr.addr = iphdr->dest.addr;

            udp_hdr* udphdr = reinterpret_cast<udp_hdr*>(((uint8_t*)((pb)->payload)) - UDP_HLEN);
            uint16_t dport = ntohs(udphdr->dest);

            AsyncUDPPacket packet(this, &daddr, dport, addr, port, data, len);
            _handler(packet);
        }

        pbuf * this_pb = pb;
        pb = pb->next;
        this_pb->next = NULL;
        pbuf_free(this_pb);
    }
}

#if LWIP_VERSION_MAJOR == 1
void AsyncUDP::_s_recv(void *arg, udp_pcb *upcb, pbuf *p, ip_addr_t *addr, uint16_t port)
#else
void AsyncUDP::_s_recv(void *arg, udp_pcb *upcb, pbuf *p, const ip_addr_t *addr, uint16_t port)
#endif
{
    reinterpret_cast<AsyncUDP*>(arg)->_recv(upcb, p, (ip_addr_t *)addr, port);
}

bool AsyncUDP::listen(ip_addr_t *addr, uint16_t port)
{
    close();
    _pcb = udp_new();
    if(_pcb == NULL) {
        return false;
    }
    err_t err = udp_bind(_pcb, addr, port);
    if(err != ERR_OK) {
        close();
        return false;
    }
    udp_recv(_pcb, &_s_recv, (void *) this);
    _connected = true;
    return true;
}

bool AsyncUDP::listenMulticast(ip_addr_t *addr, uint16_t port, uint8_t ttl)
{
    close();
    if(!ip_addr_ismulticast(addr)) {
        return false;
    }
    ip_addr_t multicast_if_addr;
    struct ip_info ifIpInfo;
    int mode = wifi_get_opmode();
    if(mode & STATION_MODE) {
        wifi_get_ip_info(STATION_IF, &ifIpInfo);
        multicast_if_addr.addr = ifIpInfo.ip.addr;
    } else if (mode & SOFTAP_MODE) {
        wifi_get_ip_info(SOFTAP_IF, &ifIpInfo);
        multicast_if_addr.addr = ifIpInfo.ip.addr;
    } else {
        return false;
    }
    if (igmp_joingroup(&multicast_if_addr, addr)!= ERR_OK) {
        return false;
    }
    if(!listen(IPADDR_ANY, port)) {
        return false;
    }
#if LWIP_VERSION_MAJOR == 1
    udp_set_multicast_netif_addr(_pcb, multicast_if_addr);
#else
    udp_set_multicast_netif_addr(_pcb, &multicast_if_addr);
#endif
    udp_set_multicast_ttl(_pcb, ttl);
    ip_addr_copy(_pcb->remote_ip, *addr);
    _pcb->remote_port = port;
    return true;
}

bool AsyncUDP::connect(ip_addr_t *addr, uint16_t port)
{
    close();
    _pcb = udp_new();
    if(_pcb == NULL) {
        return false;
    }
    err_t err = udp_connect(_pcb, addr, port);
    if(err != ERR_OK) {
        close();
        return false;
    }
    udp_recv(_pcb, &_s_recv, (void *) this);
    _connected = true;
    return true;
}

void AsyncUDP::close()
{
    if(_pcb != NULL) {
        if(_connected) {
            udp_disconnect(_pcb);
        }
        udp_remove(_pcb);
        _connected = false;
        _pcb = NULL;
    }
}

size_t AsyncUDP::writeTo(const uint8_t *data, size_t len, ip_addr_t *addr, uint16_t port)
{
    if(!_pcb && !connect(addr, port)) {
        return 0;
    }
    if(len > 1460) {
        len = 1460;
    }
    pbuf* pbt = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_RAM);
    if(pbt != NULL) {
        uint8_t* dst = reinterpret_cast<uint8_t*>(pbt->payload);
        memcpy(dst, data, len);
        err_t err = udp_sendto(_pcb, pbt, addr, port);
        pbuf_free(pbt);
        if(err < ERR_OK) {
            return 0;
        }
        return len;
    }
    return 0;
}

bool AsyncUDP::listen(const IPAddress addr, uint16_t port)
{
    ip_addr_t laddr;
    laddr.addr = addr;
    return listen(&laddr, port);
}

bool AsyncUDP::listen(uint16_t port)
{
    return listen(IPAddress((uint32_t)INADDR_ANY), port);
}

bool AsyncUDP::listenMulticast(const IPAddress addr, uint16_t port, uint8_t ttl)
{
    ip_addr_t laddr;
    laddr.addr = addr;
    return listenMulticast(&laddr, port, ttl);
}

bool AsyncUDP::connect(const IPAddress addr, uint16_t port)
{
    ip_addr_t daddr;
    daddr.addr = addr;
    return connect(&daddr, port);
}

size_t AsyncUDP::writeTo(const uint8_t *data, size_t len, const IPAddress addr, uint16_t port)
{
    ip_addr_t daddr;
    daddr.addr = addr;
    return writeTo(data, len, &daddr, port);
}

size_t AsyncUDP::write(const uint8_t *data, size_t len)
{
    //return writeTo(data, len, &(_pcb->remote_ip), _pcb->remote_port);
    if(_pcb){ // Patch applied (https://github.com/me-no-dev/ESPAsyncUDP/pull/21)
    	return writeTo(data, len, &(_pcb->remote_ip), _pcb->remote_port);
    }
    return 0;
}

size_t AsyncUDP::write(uint8_t data)
{
    return write(&data, 1);
}

size_t AsyncUDP::broadcastTo(uint8_t *data, size_t len, uint16_t port)
{
    ip_addr_t daddr;
    daddr.addr = 0xFFFFFFFF;
    return writeTo(data, len, &daddr, port);
}

size_t AsyncUDP::broadcastTo(const char * data, uint16_t port)
{
    return broadcastTo((uint8_t *)data, strlen(data), port);
}

size_t AsyncUDP::broadcast(uint8_t *data, size_t len)
{
    if(_pcb->local_port != 0) {
        return broadcastTo(data, len, _pcb->local_port);
    }
    return 0;
}

size_t AsyncUDP::broadcast(const char * data)
{
    return broadcast((uint8_t *)data, strlen(data));
}


size_t AsyncUDP::sendTo(AsyncUDPMessage &message, ip_addr_t *addr, uint16_t port)
{
    if(!message) {
        return 0;
    }
    return writeTo(message.data(), message.length(), addr, port);
}

size_t AsyncUDP::sendTo(AsyncUDPMessage &message, const IPAddress addr, uint16_t port)
{
    //if(!message) {
    if((!message) || (!_pcb)) { // Patch applied (https://github.com/me-no-dev/ESPAsyncUDP/pull/21)
        return 0;
    }
    return writeTo(message.data(), message.length(), addr, port);
}

size_t AsyncUDP::send(AsyncUDPMessage &message)
{
    if(!message) {
        return 0;
    }
    return writeTo(message.data(), message.length(), &(_pcb->remote_ip), _pcb->remote_port);
}

size_t AsyncUDP::broadcastTo(AsyncUDPMessage &message, uint16_t port)
{
    if(!message) {
        return 0;
    }
    return broadcastTo(message.data(), message.length(), port);
}

size_t AsyncUDP::broadcast(AsyncUDPMessage &message)
{
    if(!message) {
        return 0;
    }
    return broadcast(message.data(), message.length());
}
