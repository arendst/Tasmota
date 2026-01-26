
/*
  audio is2 support pcm bridge

  Copyright (C) 2022  Gerhard Mutz and Theo Arends

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


#ifdef ESP32
#if defined(USE_I2S_AUDIO) && ESP_IDF_VERSION_MAJOR >= 5
#ifdef USE_I2S_BRIDGE

#ifndef I2S_BRIDGE_PORT
#define I2S_BRIDGE_PORT 6970
#endif

#define I2S_BRIDGE_BUFFER_SIZE 512

#define I2S_BRIDGE_MODE_OFF 0
#define I2S_BRIDGE_MODE_READ 1
#define I2S_BRIDGE_MODE_WRITE 2

void i2s_bridge_init(uint8_t mode) {

  audio_i2s_mp3.bridge_mode.mode = mode;

  if (I2S_BRIDGE_MODE_OFF == mode) {
    audio_i2s_mp3.i2s_bridge_udp.flush();
    audio_i2s_mp3.i2s_bridge_udp.stop();
    //SpeakerMic(MODE_SPK);
    AUDIO_PWR_OFF
  } else {
    // i2s_set_clk(audio_i2s.mic_port, audio_i2s.mic_rate, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_STEREO);
    AddLog(LOG_LEVEL_INFO, PSTR("I2S_bridge: TODO - set bit rate and channels!!"));
    audio_i2s.in->startRx();

    if ((mode & 3) == I2S_BRIDGE_MODE_WRITE) {
      //SpeakerMic(MODE_MIC);
      //REG_SET_BIT(I2S_TIMING_REG(audio_i2s.mic_port), BIT(9));
      //REG_SET_BIT(I2S_CONF_REG(audio_i2s.mic_port), I2S_RX_MSB_SHIFT);
    } else {
      //SpeakerMic(MODE_SPK);
    }

    audio_i2s_mp3.i2s_bridge_udp.begin(I2S_BRIDGE_PORT);
    xTaskCreatePinnedToCore(i2s_bridge_task, "BRIDGE", 8192, NULL, 3, &audio_i2s_mp3.i2s_bridge_h, 1);
    if (!audio_i2s_mp3.bridge_mode.master) {
      AddLog(LOG_LEVEL_INFO, PSTR("I2S_bridge: slave started"));
    } else {
      char buffer[32];
      sprintf_P(buffer, PSTR("%u.%u.%u.%u"), audio_i2s_mp3.i2s_bridge_ip[0], audio_i2s_mp3.i2s_bridge_ip[1], audio_i2s_mp3.i2s_bridge_ip[2], audio_i2s_mp3.i2s_bridge_ip[3]);
      AddLog(LOG_LEVEL_INFO, PSTR("I2S_bridge: master started sending to ip: %s"), buffer);
    }
    AUDIO_PWR_ON
  }
}

// make mono
void make_mono(int16_t *packet_buffer, uint32_t size) {
    int16_t *wp = (int16_t*)packet_buffer;
    for (uint32_t cnt = 0; cnt < size / 2; cnt += 2) {
      int16_t val;
      if (audio_i2s_mp3.bridge_mode.swap_mic) {
        val = wp[cnt + 1] * audio_i2s.in->getRxGain();
      } else {
        val = wp[cnt] * audio_i2s.in->getRxGain();
      }
      wp[cnt] = val;
      wp[cnt + 1] = val;
    }
}

void i2s_bridge_task(void *arg) {
int16_t packet_buffer[I2S_BRIDGE_BUFFER_SIZE/2];
uint16_t bytesize;

  while (I2S_BRIDGE_MODE_OFF != audio_i2s_mp3.bridge_mode.mode) {
    if ((audio_i2s_mp3.bridge_mode.mode & 3) == 3) {
      // loopback test mode
      size_t bytes_read;
      bytesize = I2S_BRIDGE_BUFFER_SIZE;
      // i2s_read(audio_i2s.mic_port, (char *)packet_buffer, bytesize, &bytes_read, (100 / portTICK_RATE_MS));
      i2s_channel_read(audio_i2s.in->getRxHandle(), (void*)packet_buffer, bytesize, &bytes_read, pdMS_TO_TICKS(100));
      make_mono(packet_buffer, bytes_read);
      size_t bytes_written;
      // i2s_write(audio_i2s_mp3.i2s_port, (const uint8_t*)packet_buffer, bytes_read, &bytes_written, 0);
      i2s_channel_write(audio_i2s.out->getTxHandle(), (void*)packet_buffer, bytes_written, &bytes_written, 0);
    } else {
      if (audio_i2s_mp3.bridge_mode.mode & I2S_BRIDGE_MODE_READ) {
        if (audio_i2s_mp3.i2s_bridge_udp.parsePacket()) {
          size_t bytes_written;
          size_t len = audio_i2s_mp3.i2s_bridge_udp.available();
          if (len > I2S_BRIDGE_BUFFER_SIZE) {
            len = I2S_BRIDGE_BUFFER_SIZE;
          }
          len = audio_i2s_mp3.i2s_bridge_udp.read((uint8_t *)packet_buffer, len);
          audio_i2s_mp3.i2s_bridge_udp.flush();
          // i2s_write(audio_i2s.i2s_port, (const uint8_t*)packet_buffer, len, &bytes_written, 0);
          i2s_channel_write(audio_i2s.out->getTxHandle(), (void*)packet_buffer, bytes_written, &bytes_written, 0);
        } else {
          delay(1);
        }
      }

      if (audio_i2s_mp3.bridge_mode.mode & I2S_BRIDGE_MODE_WRITE) {
        size_t bytes_read;
        bytesize = I2S_BRIDGE_BUFFER_SIZE;
        // i2s_read(audio_i2s.mic_port, (char *)packet_buffer, bytesize, &bytes_read, (100 / portTICK_RATE_MS));
        i2s_channel_read(audio_i2s.in->getRxHandle(), (void*)packet_buffer, bytesize, &bytes_read, pdMS_TO_TICKS(100));
        make_mono(packet_buffer, bytes_read);
        audio_i2s_mp3.i2s_bridge_udp.beginPacket(audio_i2s_mp3.i2s_bridge_ip, I2S_BRIDGE_PORT);
        audio_i2s_mp3.i2s_bridge_udp.write((const uint8_t*)packet_buffer, bytes_read);
        audio_i2s_mp3.i2s_bridge_udp.endPacket();
      }
    }
  }
  AddLog(LOG_LEVEL_INFO, PSTR("I2S_bridge: stopped"));
  vTaskDelete(NULL);
}

void CmndI2SBridge(void) {
  if (XdrvMailbox.data_len > 0) {
    char *cp = XdrvMailbox.data;
    if (strchr(cp, '.')) {
      // enter destination ip
       audio_i2s_mp3.i2s_bridge_ip.fromString(cp);
       Response_P(PSTR("{\"I2S_bridge\":{\"IP\":\"%s\"}}"), cp);
    } else if (cp = strchr(cp, 'p')) {
      // enter push to talk pin
      cp++;
      audio_i2s_mp3.ptt_pin = atoi(cp);
      pinMode(audio_i2s_mp3.ptt_pin, INPUT_PULLUP);
      Response_P(PSTR("{\"I2S_bridge\":{\"PTT-PIN\":%d}}"), audio_i2s_mp3.ptt_pin);
    } else {
      I2SBridgeCmd(XdrvMailbox.payload, 1);
    }
  }
}

void SendBridgeCmd(uint8_t mode) {
char slavecmd[16];
  if (audio_i2s_mp3.bridge_mode.master) {
    sprintf(slavecmd,"cmd:%d", mode);
    audio_i2s_mp3.i2s_bridgec_udp.beginPacket(audio_i2s_mp3.i2s_bridge_ip, I2S_BRIDGE_PORT + 1);
    audio_i2s_mp3.i2s_bridgec_udp.write((const uint8_t*)slavecmd,strlen(slavecmd));
    audio_i2s_mp3.i2s_bridgec_udp.endPacket();
  }
}

void I2SBridgeCmd(uint8_t val, uint8_t flg) {
  if ((val >= 0) && (val <= 11)) {
    if (val > 3) {
      switch (val) {
        case 4:
          audio_i2s_mp3.bridge_mode.master = 1;
          break;
        case 5:
          audio_i2s_mp3.bridge_mode.master = 0;
          break;
        case 6:
          audio_i2s_mp3.bridge_mode.swap_mic = 1;
          break;
        case 7:
          audio_i2s_mp3.bridge_mode.swap_mic = 0;
          break;
      }
      Response_P(PSTR("{\"I2S_bridge\":{\"SWAP_MIC\":%d}}"), audio_i2s_mp3.bridge_mode.swap_mic);
    } else {
      if (audio_i2s_mp3.bridge_mode.mode != val) {
        if ((val == I2S_BRIDGE_MODE_OFF) && (audio_i2s_mp3.bridge_mode.mode != I2S_BRIDGE_MODE_OFF)) {
          if (flg &&  (audio_i2s_mp3.bridge_mode.master)) {
            // shutdown slave
            SendBridgeCmd(I2S_BRIDGE_MODE_OFF);
          }
          i2s_bridge_init(I2S_BRIDGE_MODE_OFF);
        } else {
          if (audio_i2s_mp3.bridge_mode.mode == I2S_BRIDGE_MODE_OFF) {
            // initial on
            i2s_bridge_init(val);
          } else {
            // change mode
            if (val & I2S_BRIDGE_MODE_READ) {
              //SpeakerMic(MODE_SPK);
            }
            if (val & I2S_BRIDGE_MODE_WRITE) {
              //SpeakerMic(MODE_MIC);
            }
          }
        }

        audio_i2s_mp3.bridge_mode.mode = val;

        if (flg) {
          if (audio_i2s_mp3.bridge_mode.master) {
            // set slave to complementary mode
            if (audio_i2s_mp3.bridge_mode.mode && ((audio_i2s_mp3.bridge_mode.mode & 3) != 3)) {
              uint8_t slavemode = I2S_BRIDGE_MODE_READ;
              if (audio_i2s_mp3.bridge_mode.mode & I2S_BRIDGE_MODE_READ) {
                slavemode = I2S_BRIDGE_MODE_WRITE;
              }
              SendBridgeCmd(slavemode);
            }
          }
        }
      }
      ResponseCmndNumber(audio_i2s_mp3.bridge_mode.mode);
    }
  }
}

void i2s_bridge_loop(void) {
  uint8_t packet_buffer[64];

  if (TasmotaGlobal.global_state.wifi_down) {
    return;
  }

  if (audio_i2s_mp3.ptt_pin >= 0) {
    if (audio_i2s_mp3.bridge_mode.mode != I2S_BRIDGE_MODE_OFF) {
      if (digitalRead(audio_i2s_mp3.ptt_pin) == 0) {
        // push to talk
        if (audio_i2s_mp3.bridge_mode.mode != I2S_BRIDGE_MODE_WRITE) {
          I2SBridgeCmd(I2S_BRIDGE_MODE_WRITE, 1);
        }

      } else {
        if (audio_i2s_mp3.bridge_mode.mode != I2S_BRIDGE_MODE_READ) {
          I2SBridgeCmd(I2S_BRIDGE_MODE_READ, 1);
        }
      }
    }
  }

  if (audio_i2s_mp3.i2s_bridgec_udp.parsePacket()) {
      // received control command
    memset(packet_buffer,0,sizeof(packet_buffer));
    audio_i2s_mp3.i2s_bridgec_udp.read(packet_buffer, 63);
    char *cp = (char*)packet_buffer;
    if (!strncmp(cp, "cmd:", 4)) {
      cp += 4;
      I2SBridgeCmd(atoi(cp), 0);
      audio_i2s_mp3.i2s_bridge_ip = audio_i2s_mp3.i2s_bridgec_udp.remoteIP();
      AddLog(LOG_LEVEL_INFO, PSTR("I2S_bridge: remote cmd %d"), audio_i2s_mp3.bridge_mode.mode);
    }
  }



}


void I2SBridgeInit(void) {
  // start udp control channel
  audio_i2s_mp3.i2s_bridgec_udp.begin(I2S_BRIDGE_PORT + 1);
  //audio_i2s.i2s_bridgec_udp.flush();
  //audio_i2s.i2s_bridgec_udp.stop();
  //I2SBridgeCmd(audio_i2s.bridge_mode.mode, 1);
  AddLog(LOG_LEVEL_INFO, PSTR("I2S_bridge: command server created on port: %d "), I2S_BRIDGE_PORT + 1);
}

#endif // USE_I2S_BRIDGE
#endif // USE_I2S_AUDIO
#endif // ESP32
