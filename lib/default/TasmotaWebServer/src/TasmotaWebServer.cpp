/*
  TasmotaWebServer.cpp - lib/default/TasmotaWebServer/library.json

  Copyright (C) 2021  Theo Arends & Stephan Hadinger

  This library is free software: you can redistribute it and/or modify
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

#ifdef ESP8266

// Same includes as the core WiFiServer.cpp, to see ClientContext and tcp_pcb_listen
#define LWIP_INTERNAL
#include <ESP8266WiFi.h>
#include "lwip/opt.h"
#include "lwip/tcp.h"
#include <include/ClientContext.h>

#include "TasmotaWebServer.h"

namespace {
// Read access to protected WiFiServer members.
// Pointers to members named through a derived class are allowed by
// C++ access rules, the class itself is never instantiated.
struct WiFiServerAccess : public WiFiServer {
  static ClientContext* unclaimed(WiFiServer& s) { return s.*(&WiFiServerAccess::_unclaimed); }
  static tcp_pcb* listen_pcb(WiFiServer& s) { return s.*(&WiFiServerAccess::_listen_pcb); }
};
}

// Equivalent of WiFiServer::hasClientData() || WiFiServer::hasMaxPendingClients()
// from esp8266/Arduino#8216
bool TasmotaWebServer::otherClientReady(void) {
  for (ClientContext* next = WiFiServerAccess::unclaimed(_server); next; next = next->next()) {
    if (next->getSize()) { return true; }
  }
#if TCP_LISTEN_BACKLOG
  tcp_pcb* pcb = WiFiServerAccess::listen_pcb(_server);
  if (pcb) {
    struct tcp_pcb_listen* lpcb = (struct tcp_pcb_listen*)pcb;
    if (lpcb->accepts_pending >= lpcb->backlog) { return true; }
  }
#endif
  return false;
}

#endif // ESP8266
