#pragma once

#include <Arduino.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <sys/socket.h>
#include <netinet/in.h>
//#include <arpa/inet.h>
#include <unistd.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>


typedef WiFiClient *SOCKET;
typedef WiFiUDP *UDPSOCKET;
typedef IPAddress IPADDRESS; // On linux use uint32_t in network byte order (per getpeername)
typedef uint16_t IPPORT; // on linux use network byte order

#define NULLSOCKET NULL

inline void closesocket(SOCKET s) {
//    printf("closing TCP socket\n");

    if(s) {
        s->stop();
        // delete s; TDP WiFiClients are never on the heap in arduino land?
    }
}

#define getRandom() random(65536)

inline void socketpeeraddr(SOCKET s, IPADDRESS *addr, IPPORT *port) {
    *addr = s->remoteIP();
    *port = s->remotePort();
}

inline void udpsocketclose(UDPSOCKET s) {
//    printf("closing UDP socket\n");
    if(s) {
        s->stop();
        delete s;
    }
}

inline UDPSOCKET udpsocketcreate(unsigned short portNum)
{
    UDPSOCKET s = new WiFiUDP();

    if(!s->begin(portNum)) {
//        printf("Can't bind port %d\n", portNum);
        delete s;
        return NULL;
    }

    return s;
}

// TCP sending
inline ssize_t socketsend(SOCKET sockfd, const void *buf, size_t len)
{
    return sockfd->write((uint8_t *) buf, len);
}

inline ssize_t udpsocketsend(UDPSOCKET sockfd, const void *buf, size_t len,
                             IPADDRESS destaddr, IPPORT destport)
{
    sockfd->beginPacket(destaddr, destport);
    sockfd->write((const uint8_t *)  buf, len);
    if(!sockfd->endPacket()) {
//        printf("error sending udp packet\n");
    }

    return len;
}

/**
   Read from a socket with a timeout.

   Return 0=socket was closed by client, -1=timeout, >0 number of bytes read
 */
inline int socketread(SOCKET sock, char *buf, size_t buflen, int timeoutmsec)
{
    if(!sock->connected()) {
//        printf("client has closed the socket\n");
        return 0;
    }

    int numAvail = sock->available();
    if(numAvail == 0 && timeoutmsec != 0) {
        // sleep and hope for more
        delay(timeoutmsec);
        numAvail = sock->available();
    }

    if(numAvail == 0) {
        // printf("timeout on read\n");
        return -1;
    }
    else {
        // int numRead = sock->readBytesUntil('\n', buf, buflen);
        int numRead = sock->readBytes(buf, buflen);
        // printf("bytes avail %d, read %d: %s", numAvail, numRead, buf);
        return numRead;
    }
}
