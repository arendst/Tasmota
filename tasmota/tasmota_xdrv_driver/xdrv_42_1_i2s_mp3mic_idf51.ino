/*
  xdrv_42_i2s_audio.ino - Audio dac support for Tasmota

  Copyright (C) 2021  Gerhard Mutz and Theo Arends

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


#if defined(ESP32) && ESP_IDF_VERSION_MAJOR >= 5
#ifdef USE_I2S_AUDIO

// uint32_t SpeakerMic(uint8_t spkr) {
// esp_err_t err = ESP_OK;

// //  audio_i2s.mode = spkr;
//   return err;
// }

// #ifdef USE_SHINE

// #include <layer3.h>
// #include <types.h>

// // micro to mp3 file or stream
// void mic_task(void *arg){
//   int8_t error = 0;
//   uint8_t *ucp;
//   int written;
//   shine_config_t  config;
//   shine_t s = nullptr;
//   uint16_t samples_per_pass;
//   File mp3_out = (File)nullptr;
//   int16_t *buffer = nullptr;
//   uint16_t bytesize;
//   uint16_t bwritten;
//   uint32_t ctime;

//   if (!audio_i2s_mp3.use_stream) {
//     mp3_out = ufsp->open(audio_i2s_mp3.mic_path, "w");
//     if (!mp3_out) {
//       error = 1;
//       goto exit;
//     }
//   } else {
//     if (!audio_i2s_mp3.stream_active) {
//       error = 2;
//       audio_i2s_mp3.use_stream = 0;
//       goto exit;
//     }
//     audio_i2s_mp3.client.flush();
//     audio_i2s_mp3.client.setTimeout(3);
//     audio_i2s_mp3.client.print("HTTP/1.1 200 OK\r\n"
//     "Content-Type: audio/mpeg;\r\n\r\n");

//    //  Webserver->send(200, "application/octet-stream", "");
//     //"Content-Type: audio/mp3;\r\n\r\n");
//   }

//   shine_set_config_mpeg_defaults(&config.mpeg);

//   if (audio_i2s.Settings->rx.channels == 1) {
//     config.mpeg.mode = MONO;
//   } else {
//     config.mpeg.mode = STEREO;
//   }
//   config.mpeg.bitr = 128;
//   config.wave.samplerate = audio_i2s.Settings->rx.sample_rate;
//   config.wave.channels = (channels)audio_i2s.Settings->rx.channels;

//   if (shine_check_config(config.wave.samplerate, config.mpeg.bitr) < 0) {
//     error = 3;
//     goto exit;
//   }

//   s = shine_initialise(&config);
//   if (!s) {
//     error = 4;
//     goto exit;
//   }

//   samples_per_pass = shine_samples_per_pass(s);
//   bytesize = samples_per_pass * 2 * audio_i2s.Settings->rx.channels;

//   buffer = (int16_t*)malloc(bytesize);
//   if (!buffer) {
//     error = 5;
//     goto exit;
//   }

//   ctime = TasmotaGlobal.uptime;

//   while (!audio_i2s_mp3.mic_stop) {
//       uint32_t bytes_read;
//       bytes_read = audio_i2s.in->readMic((uint8_t*)buffer, bytesize, true /*dc_block*/, false /*apply_gain*/, true /*lowpass*/, nullptr /*peak_ptr*/);
//       // i2s_read(audio_i2s.mic_port, (char *)buffer, bytesize, &bytes_read, (100 / portTICK_PERIOD_MS));

//       if (audio_i2s.Settings->rx.gain > 1) {
//         // set gain
//         for (uint32_t cnt = 0; cnt < bytes_read / 2; cnt++) {
//           buffer[cnt] *= audio_i2s.Settings->rx.gain;
//         }
//       }
//       ucp = shine_encode_buffer_interleaved(s, buffer, &written);

//       if (!audio_i2s.Settings->tx.stream_enable) {
//         bwritten = mp3_out.write(ucp, written);
//         if (bwritten != written) {
//           break;
//         }
//       } else {
//         audio_i2s_mp3.client.write((const char*)ucp, written);

//         if (!audio_i2s_mp3.client.connected()) {
//           break;
//         }
//       }
//       audio_i2s_mp3.recdur = TasmotaGlobal.uptime - ctime;
//   }

//   ucp = shine_flush(s, &written);

//   if (!audio_i2s_mp3.use_stream) {
//     mp3_out.write(ucp, written);
//   } else {
//     audio_i2s_mp3.client.write((const char*)ucp, written);
//   }


// exit:
//   if (s) {
//     shine_close(s);
//   }
//   if (mp3_out) {
//     mp3_out.close();
//   }
//   if (buffer) {
//     free(buffer);
//   }

//   if (audio_i2s_mp3.use_stream) {
//     audio_i2s_mp3.client.stop();
//   }

//   SpeakerMic(I2S_AUDIO_MODE_SPK);
//   audio_i2s_mp3.mic_stop = 0;
//   audio_i2s_mp3.mic_error = error;
//   AddLog(LOG_LEVEL_INFO, PSTR("mp3task result code: %d"), error);
//   audio_i2s_mp3.mic_task_handle = 0;
//   audio_i2s_mp3.recdur = 0;
//   audio_i2s_mp3.stream_active = 0;
//   vTaskDelete(NULL);

// }

// int32_t i2s_record_shine(char *path) {
// esp_err_t err = ESP_OK;

//   if (audio_i2s.in) {
//     if (audio_i2s_mp3.decoder || audio_i2s_mp3.mp3) return 0;
//   }

//   err = SpeakerMic(I2S_AUDIO_MODE_MIC);
//   if (err) {
//     if (audio_i2s.in) {
//       SpeakerMic(I2S_AUDIO_MODE_SPK);
//     }
//     AddLog(LOG_LEVEL_INFO, PSTR("mic init error: %d"), err);
//     return err;
//   }

//   strlcpy(audio_i2s_mp3.mic_path, path, sizeof(audio_i2s_mp3.mic_path));

//   audio_i2s_mp3.mic_stop = 0;

//   uint32_t stack = 4096;

//   audio_i2s_mp3.use_stream = !strcmp(audio_i2s_mp3.mic_path, "stream.mp3");

//   if (audio_i2s_mp3.use_stream) {
//     stack = 8000;
//   }

//   err = xTaskCreatePinnedToCore(mic_task, "MIC", stack, NULL, 3, &audio_i2s_mp3.mic_task_handle, 1);

//   return err;
// }

// void Cmd_MicRec(void) {

//   if (XdrvMailbox.data_len > 0) {
//     if (!strncmp(XdrvMailbox.data, "-?", 2)) {
//       Response_P("{\"I2SREC-duration\":%d}", audio_i2s_mp3.recdur);
//     } else {
//       i2s_record_shine(XdrvMailbox.data);
//       ResponseCmndChar(XdrvMailbox.data);
//     }
//   } else {
//     if (audio_i2s_mp3.mic_task_handle) {
//       // stop task
//       audio_i2s_mp3.mic_stop = 1;
//       while (audio_i2s_mp3.mic_stop) {
//         delay(1);
//       }
//       ResponseCmndChar_P(PSTR("Stopped"));
//     }
//   }

// }
// #endif // USE_SHINE


// // mic gain in factor not percent
// void Cmd_MicGain(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 256)) {
//     if (audio_i2s.in) {
//       audio_i2s.in->setRxGain(XdrvMailbox.payload);
//     }
//     if (audio_i2s.Settings) {
//       audio_i2s.Settings->rx.gain = XdrvMailbox.payload * 16;
//     }
//     I2SSettingsSave(AUDIO_CONFIG_FILENAME);
//   }
//   ResponseCmndNumber(audio_i2s.Settings->rx.gain / 16);
// }

#endif // USE_I2S_AUDIO
#endif // defined(ESP32) && ESP_IDF_VERSION_MAJOR >= 5
