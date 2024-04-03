/*
  xdrv_52_3_berry_tcpclientasync.ino - Berry scripting language, TCP client non-blocking

  Copyright (C) 2021 Stephan Hadinger, Berry language by Guan Wenliang https://github.com/Skiars/berry

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

// also includes tcp_client

#ifdef USE_BERRY

#ifdef USE_WEBCLIENT

#include <berry.h>
// #include "be_sys.h"
#include <lwip/sockets.h>
#include <lwip/netdb.h>

#define T_IN6_IS_ADDR_V4MAPPED(a) \
        ((((__const uint32_t *) (a))[0] == 0) \
         && (((__const uint32_t *) (a))[1] == 0) \
         && (((__const uint32_t *) (a))[2] == htonl (0xffff)))

enum class AsyncTCPState {
  INPROGRESS,
  CONNECTED,
  REFUSED,
  CLOSED
};

class AsyncTCPClient {
public:

  AsyncTCPClient() : sockfd(-1), state(AsyncTCPState::INPROGRESS), _timeout_ms(1), local_port(-1) {

  }

  // following is used when accepting a new connection as server
  AsyncTCPClient(int fd) : sockfd(fd), state(AsyncTCPState::CONNECTED), _timeout_ms(1), local_port(-1) {
    if (sockfd < 0) {
      state = AsyncTCPState::REFUSED;
    }
  }

  ~AsyncTCPClient() {
    this->stop();
  }

  void stop() {
    if (sockfd > 0) {
      close(sockfd);
    }
    sockfd = -1;
    state = AsyncTCPState::CLOSED;
  }

  int connect(IPAddress ip, uint16_t port) {
    struct sockaddr_storage serveraddr = {};
    sockfd = -1;

    if (state != AsyncTCPState::INPROGRESS) {
      stop();
      state = AsyncTCPState::INPROGRESS;    // reset state
    }

#ifdef USE_IPV6
    if (ip.type() == IPv6) {
        struct sockaddr_in6 *tmpaddr = (struct sockaddr_in6 *)&serveraddr;
        sockfd = socket(AF_INET6, SOCK_STREAM, 0);
        tmpaddr->sin6_family = AF_INET6;
        memcpy(tmpaddr->sin6_addr.un.u8_addr, &ip[0], 16);
        tmpaddr->sin6_port = htons(port);
#if ESP_IDF_VERSION_MAJOR >= 5
        tmpaddr->sin6_scope_id = ip.zone();
#endif
    } else {
#endif
        struct sockaddr_in *tmpaddr = (struct sockaddr_in *)&serveraddr;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        tmpaddr->sin_family = AF_INET;
        tmpaddr->sin_addr.s_addr = ip;
        tmpaddr->sin_port = htons(port);
#ifdef USE_IPV6
    }
#endif
    if (sockfd < 0) {
        AddLog(LOG_LEVEL_DEBUG, "BRY: Error: socket: %d", errno);
        return 0;
    }
    // berry_log_C("sockfd=%i", sockfd);
    fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL, 0) | O_NONBLOCK );    // set non-blocking

    fd_set fdset;
    FD_ZERO(&fdset);
    FD_SET(sockfd, &fdset);

    int res = lwip_connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (res < 0 && errno != EINPROGRESS) {
        AddLog(LOG_LEVEL_INFO, "BRY: Error: connect on fd %d, errno: %d, \"%s\"", sockfd, errno, strerror(errno));
        close(sockfd);
        return 0;
    }
    // berry_log_C("lwip_connect res=%i errno=%i (EINPROGRESS=%i)", res, errno, EINPROGRESS);
    return 1;
  }

  void _update_connection_state(void) {
    if (state != AsyncTCPState::INPROGRESS) {
      return;
    }

    fd_set fdset;
    FD_ZERO(&fdset);
    FD_SET(sockfd, &fdset);

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    int res = ::select(sockfd + 1, nullptr, &fdset, nullptr, &tv);
    if (res < 0) {
        AddLog(LOG_LEVEL_DEBUG, "BRY: select on fd %d, errno: %d, \"%s\"", sockfd, errno, strerror(errno));
        stop();
        state = AsyncTCPState::REFUSED;
        return;
    } else if (res == 0) {
        // AddLog(LOG_LEVEL_DEBUG, "BRY: select returned due to timeout %d ms for fd %d", _timeout_ms, sockfd);
        // stop();
        // state = AsyncTCPState::TIMEOUT;
        return;
    } else {
      int sockerr;
      socklen_t len = (socklen_t)sizeof(int);
      res = getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &sockerr, &len);

      if (res < 0) {
        AddLog(LOG_LEVEL_DEBUG, "BRY: getsockopt on fd %d, errno: %d, \"%s\"", sockfd, errno, strerror(errno));
        stop();
        state = AsyncTCPState::REFUSED;
        return;
      }

      if (sockerr != 0) {
        AddLog(LOG_LEVEL_DEBUG, "BRY: socket error on fd %d, errno: %d, \"%s\"", sockfd, sockerr, strerror(sockerr));
        stop();
        state = AsyncTCPState::REFUSED;
        return;
      }

      state = AsyncTCPState::CONNECTED;
      return;
    }
  }

  void _update_connected(void) {
    if (state == AsyncTCPState::INPROGRESS) {
      this->_update_connection_state();               // force an update
    }
    if (state == AsyncTCPState::CONNECTED) {
      uint8_t dummy;
      int res = recv(sockfd, &dummy, 0, MSG_DONTWAIT);
      // recv only sets errno if res is <= 0
      if (res <= 0){
        switch (errno) {
          case EWOULDBLOCK:
          case ENOENT: //caused by vfs
              // connected, don't change status
              break;
          case ENOTCONN:
          case EPIPE:
          case ECONNRESET:
          case ECONNREFUSED:
          case ECONNABORTED:
            stop();
            break;
          default:
            // AddLog(LOG_LEVEL_DEBUG, "BRY: tcpclientasync unexpected: RES: %d, ERR: %d, sockfd=%d", res, errno, sockfd);
            break;
        }
      } else {
        // connected do nothing
      }
    }
  }

  // connected()
  // returns:
  //  0:  still in progres, unknown
  //  1:  connected
  // -1:  refused
  int connected(void) {
    _update_connected();
    switch (state) {
      case AsyncTCPState::CONNECTED:  return 1;   break;
      case AsyncTCPState::INPROGRESS: return 0;   break;
      // all other values indicated that the connection is down
      case AsyncTCPState::REFUSED:
      case AsyncTCPState::CLOSED:
      default:
        return -1;
        break;
    }
  }

  size_t available(void) {
    _update_connected();
    if (state == AsyncTCPState::CONNECTED) {
      size_t bytes_available = 0;
      int res = ioctl(sockfd, FIONREAD, &bytes_available);
      if (res >= 0) {
        return bytes_available;
      } else {
        return 0;
      }
    } else {
      return 0;
    }
  }

  bool listening(void) {
    _update_connected();
    if (state == AsyncTCPState::CONNECTED) {
      fd_set fdset;
      FD_ZERO(&fdset);
      FD_SET(sockfd, &fdset);

      struct timeval tv;
      tv.tv_sec = 0;
      tv.tv_usec = 0;

      int res = ::select(sockfd + 1, nullptr, &fdset, nullptr, &tv);
      if (res < 0) {
        stop();
        return false;
      } else if (res == 0) {
        // connection is up but not ready to send
        return false;
      } else {
        // we assume that since we got select answering it's good
        return true;
      }
    }
    return false;
  }
  
  size_t write(const char *buffer, size_t size) {
    _update_connected();
    if (state == AsyncTCPState::CONNECTED) {
      // is the socket ready for writing
      fd_set set;
      struct timeval tv;
      FD_ZERO(&set);        // empties the set
      FD_SET(sockfd, &set); // adds FD to the set
      tv.tv_sec = 0;
      tv.tv_usec = 0;       // non-blocking
      
      if (select(sockfd + 1, NULL, &set, NULL, &tv) < 0) {
        return 0;           // error TODO close connection?
      }

      if (FD_ISSET(sockfd, &set)) {
        // ready for writes
        int res = send(sockfd, (void*) buffer, size, MSG_DONTWAIT);
        if (res >= 0) {
          return res;
        } else {          // res < 0
          if (errno != EAGAIN) {
            AddLog(LOG_LEVEL_DEBUG, "BRY: tcpclientasync send fail on fd %d, errno: %d, \"%s\"", sockfd, errno, strerror(errno));
            stop();
            return 0;
        }
        }
      } else {
        return 0;           // not ready for writes
      }

    } else {
      return 0;
    }
    return 0;
  }

  size_t read(uint8_t* buf, size_t size) {
    _update_connected();
    if (state == AsyncTCPState::CONNECTED) {
      if (size > 1436)  { size = 1436; }        // let's keep it reasonable

      int res = recv(sockfd, buf, size, MSG_DONTWAIT);
      if (res < 0) {                            // check error
        if (errno != EWOULDBLOCK) {             // something went wrong
          stop();
          return 0;
        }
      }
      return res;
    }
    return 0;
  }

  void update_local_addr_port(void) {
    local_port = -1;      // default to unknwon
    if (sockfd > 0) {
      struct sockaddr_storage local_address;
      socklen_t addr_size = sizeof(local_address);
      // getpeername(fd, (struct sockaddr*)&addr, &len);
      int res = getsockname(sockfd, (struct sockaddr*)&local_address, &addr_size);
      if (res != 0) { return; }

      // IPv4 socket, old way
      if (((struct sockaddr*)&local_address)->sa_family == AF_INET) {
          struct sockaddr_in *s = (struct sockaddr_in *)&local_address;
          local_port = ntohs(s->sin_port);
          local_addr = IPAddress((uint32_t)(s->sin_addr.s_addr));
      }
#ifdef USE_IPV6
      // IPv6, but it might be IPv4 mapped address
      if (((struct sockaddr*)&local_address)->sa_family == AF_INET6) {
          struct sockaddr_in6 *saddr6 = (struct sockaddr_in6 *)&local_address;
          local_port = ntohs(saddr6->sin6_port);
          if (T_IN6_IS_ADDR_V4MAPPED(saddr6->sin6_addr.un.u32_addr)) {
              local_addr = IPAddress(IPv4, (uint8_t*)saddr6->sin6_addr.s6_addr+12, 0);
          } else {
              local_addr = IPAddress(IPv6, (uint8_t*)(saddr6->sin6_addr.s6_addr), saddr6->sin6_scope_id);
          }
      }
#endif // USE_IPV6
    }
  }


  IPAddress remoteIP() const {
    struct sockaddr_storage addr;
    socklen_t len = sizeof addr;
    getpeername(sockfd, (struct sockaddr*)&addr, &len);

    // IPv4 socket, old way
    if (((struct sockaddr*)&addr)->sa_family == AF_INET) {
        struct sockaddr_in *s = (struct sockaddr_in *)&addr;
        return IPAddress((uint32_t)(s->sin_addr.s_addr));
    }

#if LWIP_IPV6
    // IPv6, but it might be IPv4 mapped address
    if (((struct sockaddr*)&addr)->sa_family == AF_INET6) {
        struct sockaddr_in6 *saddr6 = (struct sockaddr_in6 *)&addr;
        if (T_IN6_IS_ADDR_V4MAPPED(saddr6->sin6_addr.un.u32_addr)) {
            return IPAddress(IPv4, (uint8_t*)saddr6->sin6_addr.s6_addr+12, 0);
        } else {
            return IPAddress(IPv6, (uint8_t*)(saddr6->sin6_addr.s6_addr), saddr6->sin6_scope_id);
        }
    }
#endif
    return (IPAddress(0,0,0,0));

  }
  uint16_t remotePort() const {

    struct sockaddr_storage addr;
    socklen_t len = sizeof addr;
    getpeername(sockfd, (struct sockaddr*)&addr, &len);
    struct sockaddr_in *s = (struct sockaddr_in *)&addr;
    return ntohs(s->sin_port);
  }

  const IPAddress localIP() const { return local_addr; }
  uint16_t localPort() const {  return local_port; }


public:
  int           sockfd;
  AsyncTCPState state;
  uint32_t      _timeout_ms;
  IPAddress     local_addr;
  int32_t       local_port;       // -1 if unknown or invalid
};

/*********************************************************************************************\
 * Native functions mapped to Berry functions
 *
 *
\*********************************************************************************************/
extern "C" {
  // same but using `.p` argument
  AsyncTCPClient * wc_gettcpclientasync_p(struct bvm *vm) {
    be_getmember(vm, 1, ".p");
    void *p = be_tocomptr(vm, -1);
    be_pop(vm, 1);
    return (AsyncTCPClient*) p;
  }

  int32_t wc_tcpasync_init(struct bvm *vm);
  int32_t wc_tcpasync_init(struct bvm *vm) {
    int32_t argc = be_top(vm);
    AsyncTCPClient * wcl;
    if (argc >= 2 && be_iscomptr(vm, 2)) {
      wcl = (AsyncTCPClient*) be_tocomptr(vm, 2);
    } else {
      wcl = new AsyncTCPClient();
    }
    be_pushcomptr(vm, (void*) wcl);
    be_setmember(vm, 1, ".p");
    be_return_nil(vm);
  }

  int32_t wc_tcpasync_deinit(struct bvm *vm);
  int32_t wc_tcpasync_deinit(struct bvm *vm) {
    AsyncTCPClient * wcl = wc_gettcpclientasync_p(vm);
    if (wcl != nullptr) { delete wcl; }
    be_setmember(vm, 1, ".p");
    be_return_nil(vm);
  }

  // tcp.connect(address:string, port:int) -> bool
  int32_t wc_tcpasync_connect(struct bvm *vm);
  int32_t wc_tcpasync_connect(struct bvm *vm) {
    int32_t argc = be_top(vm);
    if (argc >= 3 && be_isstring(vm, 2) && be_isint(vm, 3)) {
      AsyncTCPClient * tcp = wc_gettcpclientasync_p(vm);
      const char * address = be_tostring(vm, 2);
      int32_t port = be_toint(vm, 3);
      // open connection
      IPAddress ipaddr;
      bool success = WifiHostByName(address, ipaddr);
      if (success) {
        success = tcp->connect(ipaddr, port);
      } else {
        AddLog(LOG_LEVEL_DEBUG, "BRY: tcpclientasync.connect couldn't resolve '%s'", address);
      }
      be_pushbool(vm, success);
      be_return(vm);  /* return self */
    }
    be_raise(vm, "attribute_error", NULL);
  }

  // tcp.close(void) -> nil
  int32_t wc_tcpasync_close(struct bvm *vm);
  int32_t wc_tcpasync_close(struct bvm *vm) {
    AsyncTCPClient * tcp = wc_gettcpclientasync_p(vm);
    tcp->stop();
    be_return_nil(vm);
  }

  // tcp.available(void) -> int
  int32_t wc_tcpasync_available(struct bvm *vm);
  int32_t wc_tcpasync_available(struct bvm *vm) {
    AsyncTCPClient * tcp = wc_gettcpclientasync_p(vm);
    int32_t available = tcp->available();
    be_pushint(vm, available);
    be_return(vm);
  }

  // tcp.connected(void) -> bool or nil
  // new value: returns `nil` if the status is still unknown
  int32_t wc_tcpasync_connected(struct bvm *vm);
  int32_t wc_tcpasync_connected(struct bvm *vm) {
    AsyncTCPClient * tcp = wc_gettcpclientasync_p(vm);
    int res = tcp->connected();
    if (res) {
      be_pushbool(vm, res > 0);
    } else {
      be_pushnil(vm);
    }
    be_return(vm);  /* return code */
  }

  // tcp.listening(void) -> bool
  // is the socket ready for sending
  int32_t wc_tcpasync_listening(struct bvm *vm);
  int32_t wc_tcpasync_listening(struct bvm *vm) {
    AsyncTCPClient * tcp = wc_gettcpclientasync_p(vm);
    bool res = tcp->listening();
    be_pushbool(vm, res);
    be_return(vm);  /* return code */
  }

  // tcp.info(void) -> map
  // get information about the socket
  int32_t wc_tcpasync_info(struct bvm *vm);
  int32_t wc_tcpasync_info(struct bvm *vm) {
    AsyncTCPClient * tcp = wc_gettcpclientasync_p(vm);
    be_newobject(vm, "map");
    int connected = tcp->connected();
    if (connected == 0) {
      be_map_insert_nil(vm, "connected");
    } else {
      be_map_insert_bool(vm, "connected", connected > 0);
    }
    if (connected >= 0) {
      be_map_insert_bool(vm, "available", tcp->available());
      be_map_insert_bool(vm, "listening", tcp->listening());
      be_map_insert_int(vm, "fd", tcp->sockfd);
      tcp->update_local_addr_port();
      if (tcp->local_port > 0) {
        be_map_insert_int(vm, "local_port", tcp->local_port);
        be_map_insert_str(vm, "local_addr", tcp->local_addr.toString(true).c_str());
      }
      be_map_insert_int(vm, "remote_port", tcp->remotePort());
      be_map_insert_str(vm, "remote_addr", tcp->remoteIP().toString(true).c_str());
    }
    be_pop(vm, 1);
    be_return(vm);
  }

  // tcp.write(bytes | string) -> int
  int32_t wc_tcpasync_write(struct bvm *vm);
  int32_t wc_tcpasync_write(struct bvm *vm) {
    int32_t argc = be_top(vm);
    if (argc >= 2 && (be_isstring(vm, 2) || be_isbytes(vm, 2))) {
      AsyncTCPClient * tcp = wc_gettcpclientasync_p(vm);
      const char * buf = nullptr;
      size_t buf_len = 0;
      if (be_isstring(vm, 2)) {  // string
        buf = be_tostring(vm, 2);
        buf_len = strlen(buf);
      } else { // bytes
        buf = (const char*) be_tobytes(vm, 2, &buf_len);
      }
      size_t bw = tcp->write(buf, buf_len);
      be_pushint(vm, bw);
      be_return(vm);  /* return code */
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // tcp.read() -> string
  int32_t wc_tcpasync_read(struct bvm *vm);
  int32_t wc_tcpasync_read(struct bvm *vm) {
    AsyncTCPClient * tcp = wc_gettcpclientasync_p(vm);
    int32_t max_read = -1;      // by default read as much as we can
    if (be_top(vm) >= 2 && be_isint(vm, 2)) {
      max_read = be_toint(vm, 2);
    }
    int32_t btr = tcp->available();
    if (btr <= 0) {
      be_pushstring(vm, "");
    } else {
      if ((max_read >= 0) && (btr > max_read)) {
        btr = max_read;
      }
      char * buf = (char*) be_pushbuffer(vm, btr);
      int32_t btr2 = tcp->read((uint8_t*) buf, btr);
      be_pushnstring(vm, buf, btr2);
    }
    be_return(vm);  /* return code */
  }

  // tcp.readbytes() -> bytes
  int32_t wc_tcpasync_readbytes(struct bvm *vm);
  int32_t wc_tcpasync_readbytes(struct bvm *vm) {
    AsyncTCPClient * tcp = wc_gettcpclientasync_p(vm);
    int32_t max_read = -1;      // by default read as much as we can
    if (be_top(vm) >= 2 && be_isint(vm, 2)) {
      max_read = be_toint(vm, 2);
    }
    int32_t btr = tcp->available();
    if (btr <= 0) {
      be_pushbytes(vm, nullptr, 0);
    } else {
      if ((max_read >= 0) && (btr > max_read)) {
        btr = max_read;
      }
      uint8_t * buf = (uint8_t*) be_pushbuffer(vm, btr);
      int32_t btr2 = tcp->read(buf, btr);
      be_pushbytes(vm, buf, btr2);
    }
    be_return(vm);  /* return code */
  }

}

#endif // USE_WEBCLIENT
#endif  // USE_BERRY
