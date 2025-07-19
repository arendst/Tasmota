/*
  xdrv_04_light_artnet.ino - Converter functions for lights

  Copyright (C) 2020  Stephan Hadinger & Theo Arends

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


#ifdef USE_LIGHT
#ifdef USE_LIGHT_ARTNET

#ifndef USE_WS2812
#define USE_WS2812                                    // needed since USE_LIGHT_ARTNET is enabled for ESP32 by default
#endif

#ifndef WS2812_ARTNET_UDP_BUFFER_SIZE
#define WS2812_ARTNET_UDP_BUFFER_SIZE         140      // Max 30 columns with 4 bytes per pixel
#endif

#ifndef WS2812_ARTNET_UDP_MAX_PACKETS
#define WS2812_ARTNET_UDP_MAX_PACKETS   30            // Max 30 rows (packets consecutive)
#endif

#ifndef WS2812_ARTNET_MAX_SLEEP
#define WS2812_ARTNET_MAX_SLEEP     5     // sleep at most 5ms
#endif

typedef struct {
  uint8_t rows = 1;     // number of rows (min:1)
  uint8_t cols = 0;     // number of columns (if cols == 0 then apply to the entire light)
  uint8_t offs = 0;     // offset in the led strip where the matrix starts (min: 0)
  bool    alt = false;  // are the rows in alternate directions
  uint16_t univ = 0;    // start at universe number (+1)
  uint16_t port = 6454; // UDP port number
  uint8_t dimm = 100;   // Dimmer 0..100
  bool    on = true;
  bool    matrix = true;  // true if light is a WS2812 matrix, false if single light
  // metrics
  uint32_t packet_received = 0;
  uint32_t packet_accepted = 0;
  uint32_t strip_refresh = 0;
} ArtNetConfig;

uint32_t * packets_per_row = nullptr;

ArtNetConfig artnet_conf;

#ifdef ESP8266
#include "UdpListener.h"
UdpListener<WS2812_ARTNET_UDP_BUFFER_SIZE> * ArtNetUdp = nullptr;
#else
WiFiUDP * ArtNetUdp;
#endif

bool artnet_udp_connected = false;
// IPAddress artnet_udp_remote_ip;                 // remote IP address
// uint16_t artnet_udp_remote_port;                // remote port


/*********************************************************************************************\
 * ArtNet support
\*********************************************************************************************/

void ArtNetLoadSettings(void) {
  // read settings and copy locally
  artnet_conf.dimm = Settings->light_dimmer;
  artnet_conf.cols = Settings->light_step_pixels;
  artnet_conf.rows = (artnet_conf.cols != 0) ? Settings->light_pixels / artnet_conf.cols : 0;
  artnet_conf.offs = Settings->light_rotation;
  artnet_conf.alt = Settings->flag.ws_clock_reverse;    // SetOption16
  artnet_conf.univ = Settings->artnet_universe;
  artnet_conf.on = (Light.power & 1);
  ArtNetValidate();
}

// validate that parameters in artnet_conf are in valid ranges
void ArtNetValidate(void) {
  if (artnet_conf.dimm > 100) { artnet_conf.dimm = 100; }
  if (artnet_conf.cols == 0 || artnet_conf.rows == 0) { artnet_conf.rows = 1; }    // if single light, both are supposed to be 0
  artnet_conf.matrix = (artnet_conf.cols > 0) && Ws2812StripConfigured();
  if (artnet_conf.univ > 32767) { artnet_conf.univ = 0; }
  if (artnet_conf.port == 0) { artnet_conf.port = 6454; }
}

void ArtNetSaveSettings(void) {
  ArtNetValidate();
  // write to settings
  Settings->light_dimmer = artnet_conf.dimm;
  Settings->light_step_pixels = artnet_conf.cols;
  if (artnet_conf.cols > 0) { Settings->light_pixels = artnet_conf.rows * artnet_conf.cols; }
  Settings->light_rotation = artnet_conf.offs;
  Settings->artnet_universe = artnet_conf.univ;
  Settings->flag.ws_clock_reverse = artnet_conf.alt;    // SetOption16
}


bool ArtNetSetChannels(void)
{
  if (artnet_udp_connected && ArtNetUdp != nullptr) {
    // ArtNet is running
    if (artnet_conf.matrix) {
      if (Light.power & 1) { return true; }   // serviced, don't cascade to WS2812
    } else {
      return false;   // if regular bulb, cascade change
    }
  } else if (Settings->flag6.artnet_autorun) {
    // ArtNet is not running but is planned to get running
    return true;      // if ArtNet autorun is own but has not started yet, block update to lights
  }
  return false;
}

// process ArtNet packet
// returns `true` if strip is dirty, i.e. we changed the value of some leds
void ArtNetProcessPacket(uint8_t * buf, size_t len) {
  artnet_conf.packet_received++;
  if (buf == nullptr || len <= 18) { return; }
  // is the signature correct?
  // 4172742D4E657400
  static const char ARTNET_SIG[] = "Art-Net";
  if (memcmp(buf, ARTNET_SIG, sizeof(ARTNET_SIG))) { return; }

  uint16_t opcode = buf[8] | (buf[9] << 8);
  uint16_t protocol = (buf[10] << 8) | buf[11];   // Big Endian
  uint16_t universe = buf[14] | (buf[15] << 8);
  uint16_t datalen = (buf[16] << 8) | buf[17];
  // AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("DMX: opcode=0x%04X procotol=%i universe=%i datalen=%i univ_start=%i univ_end=%i"), opcode, protocol, universe, datalen, artnet_conf.univ, artnet_conf.univ + artnet_conf.rows);
  if (opcode != 0x5000 || protocol != 14) { return; }

//  if (len + 18 < datalen) {
//    AddLog(LOG_LEVEL_DEBUG, PSTR("DMX: packet is truncated. Expected: %u Bytes, Received: %u Bytes."), datalen, len + 18);
//  }

  if (universe < artnet_conf.univ || universe >= artnet_conf.univ + artnet_conf.rows) { return; }  // universe is not ours, ignore
  size_t idx = 18;      // start of payload data in the UDP frame
  uint16_t row = universe - artnet_conf.univ;

  if (artnet_conf.matrix) {
    // Ws2812 led strip
    size_t pix_size = Ws2812StripGetPixelSize();
    // check that datalen does not exceed the number of columns
    if (datalen > artnet_conf.cols * pix_size) { datalen = artnet_conf.cols * pix_size; }
    // round to exact number of pixels
    datalen = datalen - (datalen % pix_size);

    size_t offset_in_matrix = 0;
    if (artnet_conf.alt && (row % 2)) {
      for (int32_t i = idx, j = idx + datalen - pix_size; i < j; i += pix_size, j -= pix_size) {
          for (int32_t k = 0; k < pix_size; k++) {
              uint8_t temp = buf[i+k];
              buf[i+k] = buf[j+k];
              buf[j+k] = temp;
          }
      }
      offset_in_matrix = artnet_conf.cols * pix_size - datalen; // add a potential offset if the frame is smaller than the columns
    }

    // process dimmer
    if (artnet_conf.dimm != 100) {
      // No Gamma for now
      if (pix_size == 3) {
        for (int32_t i = idx; i < idx+datalen; i += pix_size) {
          buf[i] = changeUIntScale(buf[i], 0, 100, 0, artnet_conf.dimm);
          buf[i+1] = changeUIntScale(buf[i+1], 0, 100, 0, artnet_conf.dimm);
          buf[i+2] = changeUIntScale(buf[i+2], 0, 100, 0, artnet_conf.dimm);
        }
      } else if (pix_size == 4) {
        for (int32_t i = idx; i < idx+datalen; i += pix_size) {
          buf[i] = changeUIntScale(buf[i], 0, 100, 0, artnet_conf.dimm);
          buf[i+1] = changeUIntScale(buf[i+1], 0, 100, 0, artnet_conf.dimm);
          buf[i+2] = changeUIntScale(buf[i+2], 0, 100, 0, artnet_conf.dimm);
          buf[i+3] = changeUIntScale(buf[i+2], 0, 100, 0, artnet_conf.dimm);
        }
      }
    }

    // process pixels
    size_t h_bytes = artnet_conf.cols * pix_size;   // size in bytes of a single row
    offset_in_matrix += artnet_conf.offs * pix_size + row * h_bytes;
    if (datalen > h_bytes) { datalen = h_bytes; }   // copy at most one line

    Ws2812CopyPixels(&buf[idx], datalen, offset_in_matrix);
  } else {
    // single light
    size_t offsidx = artnet_conf.offs + idx;
    uint8_t r8 = buf[offsidx+1];
    uint8_t g8 = buf[offsidx];
    uint8_t b8 = buf[offsidx+2];
    uint8_t w8 = buf[offsidx+3];
    uint8_t ww8 = buf[offsidx+4];
    // scale dimmer values to RGBWWTable calibration
    uint16_t r_dimmer = changeUIntScale(artnet_conf.dimm, 0, 100, 0, Settings->rgbwwTable[0]) * 4;
    uint16_t g_dimmer = changeUIntScale(artnet_conf.dimm, 0, 100, 0, Settings->rgbwwTable[1]) * 4;
    uint16_t b_dimmer = changeUIntScale(artnet_conf.dimm, 0, 100, 0, Settings->rgbwwTable[2]) * 4;
    uint16_t w_dimmer = changeUIntScale(artnet_conf.dimm, 0, 100, 0, Settings->rgbwwTable[3]) * 4;
    uint16_t ww_dimmer = changeUIntScale(artnet_conf.dimm, 0, 100, 0, Settings->rgbwwTable[4]) * 4;
    uint16_t color[LST_MAX] = {0};
    color[0] = changeUIntScale(r8, 0, 255, 0, r_dimmer);
    color[1] = changeUIntScale(g8, 0, 255, 0, g_dimmer);
    color[2] = changeUIntScale(b8, 0, 255, 0, b_dimmer);
    color[3] = changeUIntScale(w8, 0, 255, 0, w_dimmer);
    color[4] = changeUIntScale(ww8, 0, 255, 0, ww_dimmer);
    // AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("DMX: %02X-%02X-%02X univ=%i rows=%i max_univ=%i"), buf[idx+1], buf[idx], buf[idx+2], universe, row, artnet_conf.univ + artnet_conf.rows);
    LightSetOutputs(color);
  }
  // AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("DMX: ok universe=%i datalen=%i"), universe, datalen);
  artnet_conf.packet_accepted++;
  if (packets_per_row) {
    packets_per_row[row]++;
  }
}

//
// Called at event loop, checks for incoming data from the CC2530
//
void ArtNetLoop(void)
{
  if (artnet_udp_connected && ArtNetUdp != nullptr) {
    ArtNetLoadSettings();
    bool packet_ready = false;
    int32_t packet_len = 0;
#ifdef ESP8266
    packet_ready = ArtNetUdp->next();
    while (packet_ready) {
      UdpPacket<WS2812_ARTNET_UDP_BUFFER_SIZE> *packet;
      packet = ArtNetUdp->read();
      uint8_t * packet_buffer = (uint8_t*) &packet->buf;
      packet_len = packet->len;
#else
    packet_len = ArtNetUdp->parsePacket();
    packet_ready = (packet_len > 0);
    while (packet_ready) {
      uint8_t packet_buffer[WS2812_ARTNET_UDP_BUFFER_SIZE];     // buffer to hold incoming UDP/SSDP packet

      packet_len = ArtNetUdp->read(packet_buffer, WS2812_ARTNET_UDP_BUFFER_SIZE);
      ArtNetUdp->flush();   // Finish reading the current packet
#endif
      // AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("UDP: Packet %*_H (%d)"), 32, packet_buffer, packet_len);
      if (artnet_conf.on) {
        ArtNetProcessPacket(packet_buffer, packet_len);
      }

#ifdef ESP8266
      packet_ready = ArtNetUdp->next();
      if (!packet_ready) {
        // if no more incoming packet, still wait for 20 microseconds
        delay(1);   // delayMicroseconds seems broken, need to
        packet_ready = ArtNetUdp->next();
      }
#else
      packet_len = ArtNetUdp->parsePacket();
      packet_ready = (packet_len > 0);
      if (!packet_ready) {
        // if no more incoming packet, still wait for 20 microseconds
        delayMicroseconds(20);
        packet_len = ArtNetUdp->parsePacket();
        packet_ready = (packet_len > 0);
      }
#endif
    }
    if (artnet_conf.on) {                 // ignore action if not on
      if (artnet_conf.matrix) {
        if (Ws2812StripRefresh()) {
          artnet_conf.strip_refresh++;    // record metric
        }
      }
    }
  }
}


//
// Published state
//
void ArtNetJSONAppend(void) {
  if (artnet_udp_connected) {
    ResponseAppend_P(PSTR(",\"ArtNet\":{\"PacketsReceived\":%u,\"PacketsAccepted\":%u,\"Frames\":%u"),
                    artnet_conf.packet_received, artnet_conf.packet_accepted, artnet_conf.strip_refresh);
    if (packets_per_row) {
      ResponseAppend_P(PSTR(",\"PacketsPerRow\":["));
      for (int32_t i = 0; i < artnet_conf.rows; i++) {
        ResponseAppend_P(PSTR("%s%i"), i ? "," : "", packets_per_row[i]);
      }
      ResponseAppend_P(PSTR("]"));
    }
    ResponseAppend_P(PSTR("}"));
  }
}

//
// Command `ArtNetConfig`
// Params: JSON
// {"Rows":5, "Cols":5, "Offset":0, "Alternate":false, "Universe":0, "Port":6454}
//
void CmndArtNetConfig() {
  bool was_running = artnet_udp_connected;
  if (was_running) {
    ArtNetStop();
  }
  ArtNetLoadSettings();

  TrimSpace(XdrvMailbox.data);
  if (strlen(XdrvMailbox.data) > 0) {
    JsonParser parser(XdrvMailbox.data);
    JsonParserObject root = parser.getRootObject();
    if (!root) { ResponseCmndChar_P(PSTR(D_JSON_INVALID_JSON)); return; }

    artnet_conf.rows  = root.getUInt(PSTR("Rows"), artnet_conf.rows);
    artnet_conf.cols  = root.getUInt(PSTR("Cols"), artnet_conf.cols);
    artnet_conf.offs  = root.getUInt(PSTR("Offset"), artnet_conf.offs);
    artnet_conf.alt   = root.getBool(PSTR("Alternate"), artnet_conf.alt);
    artnet_conf.univ  = root.getUInt(PSTR("Universe"), artnet_conf.univ);
    artnet_conf.port  = root.getUInt(PSTR("Port"), artnet_conf.port);
    artnet_conf.dimm  = root.getUInt(PSTR("Dimmer"), artnet_conf.dimm);

    ArtNetSaveSettings();
  }

  if (was_running) {
    ArtNetStart();
  }
  // display the current or new configuration
  // {"Rows":5, "Cols":5, "Offset":0, "Alternate":false, "Universe":0, "Port":6454}
  Response_P(PSTR("{\"Rows\":%u,\"Cols\":%u,\"Dimmer\":%u,\"Offset\":%u"
                  ",\"Alternate\":%s,\"Universe\":%u,\"Port\":%u}"),
                  artnet_conf.rows, artnet_conf.cols, artnet_conf.dimm, artnet_conf.offs,
                  artnet_conf.alt ? "true":"false", artnet_conf.univ, artnet_conf.port);
}

// ArtNetStart
// Returns true if ok
bool ArtNetStart(void) {
  ArtNetLoadSettings();
  if (!artnet_udp_connected && !TasmotaGlobal.restart_flag) {
    if (ArtNetUdp == nullptr) {
#ifdef ESP8266
      ArtNetUdp = new UdpListener<WS2812_ARTNET_UDP_BUFFER_SIZE>(WS2812_ARTNET_UDP_MAX_PACKETS);
#else
      ArtNetUdp = new WiFiUDP();
#endif
      if (ArtNetUdp == nullptr) {
        AddLog(LOG_LEVEL_INFO, PSTR("DMX: cannot allocate memory"));
        return false;
      }
    }

#ifdef ESP8266
    ArtNetUdp->reset();
    ip_addr_t addr = IPADDR4_INIT(INADDR_ANY);
    if ((igmp_joingroup(WiFi.localIP(), IPAddress(USE_LIGHT_ARTNET_MCAST)) == ERR_OK) &&
        (ArtNetUdp->listen(&addr, artnet_conf.port))) {
    // if (ArtNetUdp->listen(&addr, artnet_conf.port)) {
#else
    if (ArtNetUdp->beginMulticast(IPAddress(USE_LIGHT_ARTNET_MCAST), artnet_conf.port)) {
#endif
      // OK
      AddLog(LOG_LEVEL_INFO, PSTR("DMX: listening to port %i"), artnet_conf.port);
      artnet_udp_connected = true;

      packets_per_row = (uint32_t*) malloc(artnet_conf.rows * sizeof(uint32_t*));
      if (packets_per_row) { memset((void*)packets_per_row, 0, artnet_conf.rows * sizeof(uint32_t*)); }
      // set sleep to at most 5
      if (TasmotaGlobal.sleep > WS2812_ARTNET_MAX_SLEEP) {
        TasmotaGlobal.sleep = WS2812_ARTNET_MAX_SLEEP;
      }

      // change settings to ArtNet specific scheme
      Settings->flag6.artnet_autorun = true;

      // change strip configuration
      if (artnet_conf.matrix) {
        if ((Settings->light_pixels != artnet_conf.rows * artnet_conf.cols + artnet_conf.offs) || (Settings->light_rotation != 0)) {
          Settings->light_pixels = artnet_conf.rows * artnet_conf.cols + artnet_conf.offs;
          Settings->light_rotation = 0;
          Ws2812InitStrip();
        } else {
          Ws2812Clear(true);
        }
      }

      // turn power on if it's not
      if (!(Light.power & 1)) {
        LightPowerOn();
      }
    } else {
      AddLog(LOG_LEVEL_INFO, PSTR("DMX: error opening port %i"), artnet_conf.port);
      return false;
    }
  }
  return true;
}

// Stop the ArtNet UDP flow and disconnect server
void ArtNetStop(void) {
  artnet_udp_connected = false;
  if (ArtNetUdp != nullptr) {
#ifdef ESP8266
    ArtNetUdp->disconnect();
#else
    ArtNetUdp->stop();
#endif
    delete ArtNetUdp;
    ArtNetUdp = nullptr;
  }
  if (packets_per_row) {
    free((void*)packets_per_row);
    packets_per_row = nullptr;
  }
}

void CmndArtNet(void) {
  if (0 == XdrvMailbox.payload) {
    ArtNetStop();
    Settings->flag6.artnet_autorun = false;    // SetOption148 - (Light) start DMX ArtNet at boot, listen to UDP port as soon as network is up
//    Settings->light_scheme = LS_POWER;         // restore default scheme
    TasmotaGlobal.sleep = Settings->sleep;     // Restore sleep value
    Light.update = true;                       // Restore old color
  }
  if (1 == XdrvMailbox.payload) {
    if (!ArtNetStart()) {
      Settings->flag6.artnet_autorun = false;  // SetOption148 - (Light) start DMX ArtNet at boot, listen to UDP port as soon as network is up
    }
  }
  ResponseCmndStateText(artnet_udp_connected & Settings->flag6.artnet_autorun);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool artnet_network_up = false;

void ArtNetFuncNetworkUp(void) {
  if (!artnet_network_up) {
    artnet_network_up = true;
    if (Settings->flag6.artnet_autorun) {
      if (!ArtNetStart()) {
        Settings->flag6.artnet_autorun = false;   // disable autorun if it failed, avoid nasty loop errors
      }
    }
  }
}

void ArtNetFuncNetworkDown(void) {
  if (artnet_network_up) {
    artnet_network_up = false;
    ArtNetStop();
  }
}

#endif // USE_LIGHT_ARTNET
#endif // USE_LIGHT
