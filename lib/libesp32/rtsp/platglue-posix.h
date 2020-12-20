#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

typedef int SOCKET;
typedef int UDPSOCKET;
typedef uint32_t IPADDRESS; // On linux use uint32_t in network byte order (per getpeername)
typedef uint16_t IPPORT; // on linux use network byte order

#define NULLSOCKET 0

inline void closesocket(SOCKET s) {
    close(s);
}

#define getRandom() rand()

inline void socketpeeraddr(SOCKET s, IPADDRESS *addr, IPPORT *port) {

    sockaddr_in r;
    socklen_t len = sizeof(r);
    if(getpeername(s,(struct sockaddr*)&r,&len) < 0) {
        printf("getpeername failed\n");
        *addr = 0;
        *port = 0;
    }
    else {
        //htons

        *port  = r.sin_port;
        *addr = r.sin_addr.s_addr;
    }
}

inline void udpsocketclose(UDPSOCKET s) {
    close(s);
}

inline UDPSOCKET udpsocketcreate(unsigned short portNum)
{
    sockaddr_in addr;

    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;

    int s     = socket(AF_INET, SOCK_DGRAM, 0);
    addr.sin_port = htons(portNum);
    if (bind(s,(sockaddr*)&addr,sizeof(addr)) != 0) {
        printf("Error, can't bind\n");
        close(s);
        s = 0;
    }

    return s;
}

// TCP sending
inline ssize_t socketsend(SOCKET sockfd, const void *buf, size_t len)
{
    // printf("TCP send\n");
    return send(sockfd, buf, len, 0);
}

inline ssize_t udpsocketsend(UDPSOCKET sockfd, const void *buf, size_t len,
                             IPADDRESS destaddr, uint16_t destport)
{
    sockaddr_in addr;

    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = destaddr;
    addr.sin_port = htons(destport);
    //printf("UDP send to 0x%0x:%0x\n", destaddr, destport);

    return sendto(sockfd, buf, len, 0, (sockaddr *) &addr, sizeof(addr));
}

/**
   Read from a socket with a timeout.

   Return 0=socket was closed by client, -1=timeout, >0 number of bytes read
 */
inline int socketread(SOCKET sock, char *buf, size_t buflen, int timeoutmsec)
{
    // Use a timeout on our socket read to instead serve frames
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = timeoutmsec * 1000; // send a new frame ever
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof tv);

    int res = recv(sock,buf,buflen,0);
    if(res > 0) {
        return res;
    }
    else if(res == 0) {
        return 0; // client dropped connection
    }
    else {
        if (errno == EWOULDBLOCK || errno == EAGAIN)
            return -1;
        else
            return 0; // unknown error, just claim client dropped it
    };
}
