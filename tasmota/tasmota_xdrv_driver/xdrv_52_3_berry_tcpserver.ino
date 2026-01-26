/*
  xdrv_52_3_berry_tcpserver.ino - Berry scripting language, tcp socket and server class

  Copyright (C) 2022 Stephan Hadinger, Berry language by Guan Wenliang https://github.com/Skiars/berry

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
#ifdef USE_BERRY_TCPSERVER

#include <berry.h>

// WiFiServerAsync can return either `WiFiClient` (sync) or `AsyncTCPClient` (async)
//
class WiFiServerAsync {
  protected:
    int sockfd;
    int _accepted_sockfd = -1;
    IPAddress _addr;
    uint16_t _port;
    uint8_t _max_clients;
    bool _listening;
    bool _noDelay = false;


  public:
    WiFiServerAsync(uint16_t port=80, uint8_t max_clients=4):sockfd(-1),_accepted_sockfd(-1),_addr(),_port(port),_max_clients(max_clients),_listening(false),_noDelay(false) {
    }
    WiFiServerAsync(const IPAddress& addr, uint16_t port=80, uint8_t max_clients=4):sockfd(-1),_accepted_sockfd(-1),_addr(addr),_port(port),_max_clients(max_clients),_listening(false),_noDelay(false) {
    }
    ~WiFiServerAsync(){ end();}
    WiFiClient available();
    WiFiClient accept(){return available();}
    AsyncTCPClient * availableAsync();
    AsyncTCPClient * acceptAsync(){return availableAsync();}
    void begin(uint16_t port=0);
    void begin(uint16_t port, int reuse_enable);
    void setNoDelay(bool nodelay);
    bool getNoDelay();
    bool hasClient();

    void end();
    void close();
    void stop();
    operator bool(){return _listening;}
    int setTimeout(uint32_t seconds);
    void stopAll();  public:
};


int WiFiServerAsync::setTimeout(uint32_t seconds){
  struct timeval tv;
  tv.tv_sec = seconds;
  tv.tv_usec = 0;
  if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval)) < 0)
    return -1;
  return setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(struct timeval));
}

void WiFiServerAsync::stopAll(){}

WiFiClient WiFiServerAsync::available(){
  if(!_listening)
    return WiFiClient();
  int client_sock;
  if (_accepted_sockfd >= 0) {
    client_sock = _accepted_sockfd;
    _accepted_sockfd = -1;
  }
  else {
  struct sockaddr_in6 _client;
  int cs = sizeof(struct sockaddr_in6);
    client_sock = lwip_accept(sockfd, (struct sockaddr *)&_client, (socklen_t*)&cs);
  }
  if(client_sock >= 0){
    int val = 1;
    if(setsockopt(client_sock, SOL_SOCKET, SO_KEEPALIVE, (char*)&val, sizeof(int)) == ESP_OK) {
      val = _noDelay;
      if(setsockopt(client_sock, IPPROTO_TCP, TCP_NODELAY, (char*)&val, sizeof(int)) == ESP_OK)
        return WiFiClient(client_sock);
    }
  }
  return WiFiClient();
}

// specific Async version
AsyncTCPClient * WiFiServerAsync::availableAsync(){
  if(!_listening)
    return new AsyncTCPClient();
  int client_sock;
  if (_accepted_sockfd >= 0) {
    client_sock = _accepted_sockfd;
    _accepted_sockfd = -1;
  }
  else {
  struct sockaddr_in6 _client;
  int cs = sizeof(struct sockaddr_in6);
    client_sock = lwip_accept(sockfd, (struct sockaddr *)&_client, (socklen_t*)&cs);
  }
  if(client_sock >= 0){
    int val = 1;
    if(setsockopt(client_sock, SOL_SOCKET, SO_KEEPALIVE, (char*)&val, sizeof(int)) == ESP_OK) {
      val = _noDelay;
      if(setsockopt(client_sock, IPPROTO_TCP, TCP_NODELAY, (char*)&val, sizeof(int)) == ESP_OK) {
        if (fcntl(client_sock, F_SETFL, fcntl(sockfd, F_GETFL, 0) | O_NONBLOCK ) == ESP_OK) {    // set non-blocking
          return new AsyncTCPClient(client_sock);
        }
      }
    }
  }
  return new AsyncTCPClient();
}

void WiFiServerAsync::begin(uint16_t port){
    begin(port, 1);
}

void WiFiServerAsync::begin(uint16_t port, int enable){
  if(_listening)
    return;
  if(port){
      _port = port;
  }
  struct sockaddr_in6 server;
  sockfd = socket(AF_INET6 , SOCK_STREAM, 0);
  if (sockfd < 0)
    return;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
  server.sin6_family = AF_INET6;
  if (_addr.type() == IPv4) {
    memcpy(server.sin6_addr.s6_addr+11, (uint8_t*)&_addr[0], 4);
    server.sin6_addr.s6_addr[10] = 0xFF;
    server.sin6_addr.s6_addr[11] = 0xFF;
  } else {
    memcpy(server.sin6_addr.s6_addr, (uint8_t*)&_addr[0], 16);
  }
  memset(server.sin6_addr.s6_addr, 0x0, 16);
  server.sin6_port = htons(_port);
  if(bind(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0)
    return;
  if(listen(sockfd , _max_clients) < 0)
    return;
  fcntl(sockfd, F_SETFL, O_NONBLOCK);
  _listening = true;
  _noDelay = false;
  _accepted_sockfd = -1;
}

void WiFiServerAsync::setNoDelay(bool nodelay) {
    _noDelay = nodelay;
}

bool WiFiServerAsync::getNoDelay() {
    return _noDelay;
}

bool WiFiServerAsync::hasClient() {
    if (_accepted_sockfd >= 0) {
      return true;
    }
    struct sockaddr_in6 _client;
    int cs = sizeof(struct sockaddr_in6);
    _accepted_sockfd = lwip_accept(sockfd, (struct sockaddr *)&_client, (socklen_t*)&cs);
    if (_accepted_sockfd >= 0) {
      return true;
    }
    return false;
}

void WiFiServerAsync::end(){
  lwip_close(sockfd);
  sockfd = -1;
  _listening = false;
}

void WiFiServerAsync::close(){
  end();
}

void WiFiServerAsync::stop(){
  end();
}

/*********************************************************************************************\
 * Native functions mapped to Berry functions
 * 
\*********************************************************************************************/
extern "C" {
  const void* tcpserver_init(struct bvm *vm, int32_t tcp_port) {
    if (tcp_port > 0 && tcp_port < 65535) {
      WiFiServerAsync *server_tcp = new WiFiServerAsync(tcp_port);
      server_tcp->begin(); // start TCP server
      if (!*server_tcp) {
        be_raise(vm, "network_error", "Failed to open socket");
      }
      server_tcp->setNoDelay(true);
      return server_tcp;
    } else {
      be_raisef(vm, "value_error","Invalid port %d", tcp_port);
    }
    return NULL;
  }

  void tcpserver_deinit(void *server) {
    WiFiServerAsync *server_tcp = (WiFiServerAsync*) server;
    if (server_tcp) {
      server_tcp->stop();
      delete server_tcp;
    }
  }

  void tcpserver_close(void *server) {
    WiFiServerAsync *server_tcp = (WiFiServerAsync*) server;
    if (server_tcp) {
      server_tcp->stop();
    }
  }

  bbool tcpserver_hasclient(void *server) {
    WiFiServerAsync *server_tcp = (WiFiServerAsync*) server;
    return server_tcp->hasClient();
  }

  void * tcpserver_accept(struct bvm *vm, void *server) {
    WiFiServerAsync *server_tcp = (WiFiServerAsync*) server;
    WiFiClient * client_ptr = nullptr;
    if (server_tcp->hasClient()) {
      WiFiClient new_client = server_tcp->available();
      AddLog(LOG_LEVEL_DEBUG_MORE, "BRY: Got connection from %s:%i local %s:%i", new_client.remoteIP().toString(true).c_str(), new_client.remotePort(), new_client.localIP().toString().c_str(), new_client.localPort());
      client_ptr = new WiFiClient();
      *client_ptr = new_client;
    } else {
      be_raise(vm, "internal_error", "tcpserver has no client connected");
    }
    return client_ptr;
  }

  void * tcpserver_acceptasync(struct bvm *vm, void *server) {
    WiFiServerAsync *server_tcp = (WiFiServerAsync*) server;
    AsyncTCPClient * client_ptr = nullptr;
    if (server_tcp->hasClient()) {
      client_ptr = server_tcp->availableAsync();
      AddLog(LOG_LEVEL_DEBUG_MORE, "BRY: Got connection from %s:%i local %s:%i", client_ptr->remoteIP().toString(true).c_str(), client_ptr->remotePort(), client_ptr->localIP().toString().c_str(), client_ptr->localPort());
    } else {
      be_raise(vm, "internal_error", "tcpserver has no client connected");
    }
    return client_ptr;
  }

}

#endif  // USE_BERRY_TCPSERVER
#endif  // USE_BERRY
