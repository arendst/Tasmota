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

/*********************************************************************************************\
 * Native functions mapped to Berry functions
 * 
\*********************************************************************************************/
extern "C" {
  const void* tcpserver_init(struct bvm *vm, int32_t tcp_port) {
    if (tcp_port > 0 && tcp_port < 65535) {
      WiFiServer *server_tcp = new WiFiServer(tcp_port);
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
    WiFiServer *server_tcp = (WiFiServer*) server;
    if (server_tcp) {
      server_tcp->stop();
      delete server_tcp;
    }
  }

  void tcpserver_close(void *server) {
    WiFiServer *server_tcp = (WiFiServer*) server;
    if (server_tcp) {
      server_tcp->stop();
    }
  }

  bbool tcpserver_hasclient(void *server) {
    WiFiServer *server_tcp = (WiFiServer*) server;
    return server_tcp->hasClient();
  }

  void * tcpserver_accept(struct bvm *vm, void *server) {
    WiFiServer *server_tcp = (WiFiServer*) server;
    WiFiClient * client_ptr = nullptr;
    if (server_tcp->hasClient()) {
      WiFiClient new_client = server_tcp->available();
      AddLog(LOG_LEVEL_INFO, "BRY: Got connection from %s", new_client.remoteIP().toString().c_str());
      client_ptr = new WiFiClient();
      *client_ptr = new_client;
    } else {
      be_raise(vm, "internal_error", "tcpserver has no client connected");
    }
    return client_ptr;
  }

}

#endif  // USE_BERRY_TCPSERVER
#endif  // USE_BERRY
