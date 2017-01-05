/*
Copyright (c) 2016 Fabrizio Guglielmino.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

#ifdef USE_DISCOVERY
#include <ESP8266mDNS.h>

boolean discoverMQTTServer() {
  if (!MDNS.begin(sysCfg.hostname)) {
    addLog_P(LOG_LEVEL_ERROR, PSTR("mDNS: Initialization failed"));
    return false;
  }

  char log[LOGSZ], ip_str[20];
  
  int n = MDNS.queryService("mqtt", "tcp"); // Search for mqtt service
  snprintf_P(log, sizeof(log), PSTR("mDNS: Query done with %d services found"), n);
  addLog(LOG_LEVEL_INFO, log);

  if (n > 0) {
    // Note: current strategy is to get the first MQTT service (even when many are found)
    IPtoCharArray(MDNS.IP(0), ip_str, 20);
    
    snprintf_P(log, sizeof(log), PSTR("mDNS: Service found on %s ip %s port %d"), MDNS.hostname(0).c_str(), ip_str, MDNS.port(0));
    addLog(LOG_LEVEL_INFO, log);

    strlcpy(sysCfg.mqtt_host, ip_str, sizeof(sysCfg.mqtt_host));
    sysCfg.mqtt_port = MDNS.port(0);
  }

  return n > 0;
}

void IPtoCharArray(IPAddress address, char *ip_str, size_t size)
{
    String str = String(address[0]);
    str += ".";
    str += String(address[1]);
    str += ".";
    str += String(address[2]);
    str += ".";
    str += String(address[3]);
    str.toCharArray(ip_str, size);
}
#endif

