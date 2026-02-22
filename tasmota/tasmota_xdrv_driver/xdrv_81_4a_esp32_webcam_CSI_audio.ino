/*
  xdrv_81_4a_esp32_webcam_CSI_audio.ino - WebRTC Audio (I2S Mic → SRTP)

  Codec: Opus (USE_I2S_OPUS) or G.711 A-law fallback

  Copyright (C) 2025  Christian Baars and Theo Arends

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
#ifdef USE_CSI_WEBCAM
#ifdef USE_I2S_AUDIO

/*********************************************************************************************/
// Codec configuration
/*********************************************************************************************/

// PCM samples per 20ms frame at 8 kHz (common to both codecs)
#define WEBRTC_AUDIO_FRAME_SAMPLES   160
// Opus uses 48 kHz RTP clock (960 per 20ms), G.711 uses 8 kHz (160 per 20ms)
#ifdef USE_I2S_OPUS
#include "libopus/opus.h"
#define WEBRTC_OPUS_MAX_PACKET       256    // max encoded bytes per frame
#define WEBRTC_OPUS_RTP_PT           111    // dynamic payload type
#define WEBRTC_AUDIO_TS_INCREMENT    960
#else
#undef WEBRTC_AUDIO_TS_INCREMENT
#define WEBRTC_AUDIO_TS_INCREMENT    160
#endif
#define WEBRTC_G711A_SILENCE 0xD5

/*********************************************************************************************/
// G.711 A-law encoder (ITU-T G.711) - only when Opus not available
/*********************************************************************************************/

#ifndef USE_I2S_OPUS

static const uint8_t ALAW_COMPRESS_TABLE[] = {
  1,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,
  5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
  6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
  6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7
};

static inline uint8_t linear_to_alaw(int16_t pcm) {
  int sign, exponent, mantissa;
  uint8_t alaw;

  sign = ((~pcm) >> 8) & 0x80;
  if (sign == 0) { pcm = -pcm; }
  if (pcm > 32635) { pcm = 32635; }
  if (pcm >= 256) {
    exponent = ALAW_COMPRESS_TABLE[(pcm >> 8) & 0x7F];
    mantissa = (pcm >> (exponent + 3)) & 0x0F;
    alaw = (exponent << 4) | mantissa;
  } else {
    alaw = pcm >> 4;
  }
  alaw ^= (sign ^ 0x55);
  return alaw;
}

#endif  // !USE_I2S_OPUS

/*********************************************************************************************/
// Audio task state
/*********************************************************************************************/

struct WcAudio_State_t {
  TaskHandle_t task_handle;
  uint32_t prev_sample_rate;
  volatile bool stop_requested;
#ifdef USE_I2S_OPUS
  OpusEncoder *opus_enc;
#endif
  // diagnostics
  uint32_t frames_sent;
  uint32_t underruns;
  int16_t last_peak;
  int32_t last_dc;
};

static WcAudio_State_t *WcAudio = nullptr;

/*********************************************************************************************/
// Audio Silence
/*********************************************************************************************/

void WcSendAudioSilence(void) {
  if (!WebRTC || WebRTC->state != WEBRTC_STREAMING) return;
  uint32_t now = millis();
  if (now - WebRTC->last_audio_time < 20) return;

#ifdef USE_I2S_OPUS
  // Opus silence: encode a zero-filled PCM frame
  if (WcAudio && WcAudio->opus_enc) {
    int16_t silence_pcm[WEBRTC_AUDIO_FRAME_SAMPLES] = {0};
    uint8_t opus_pkt[WEBRTC_OPUS_MAX_PACKET];
    opus_int32 len = opus_encode(WcAudio->opus_enc, silence_pcm, WEBRTC_AUDIO_FRAME_SAMPLES, opus_pkt, WEBRTC_OPUS_MAX_PACKET);
    if (len > 0) {
      WcSendSrtpPacket(&WebRTC->audio, opus_pkt, len, false);
    }
  }
#else
  uint8_t payload[WEBRTC_AUDIO_FRAME_SAMPLES];
  memset(payload, WEBRTC_G711A_SILENCE, sizeof(payload));
  WcSendSrtpPacket(&WebRTC->audio, payload, sizeof(payload), false);
#endif

  WebRTC->audio.timestamp += WEBRTC_AUDIO_TS_INCREMENT;
  WebRTC->last_audio_time = now;
}

/*********************************************************************************************/
// I2S Mic → Encode → SRTP Task
/*********************************************************************************************/

void WcI2sWebRTCAudioTask(void *pvParameters) {
  AddLog(LOG_LEVEL_INFO, PSTR("WcAudio: Task started"));

  bool is_duplex = audio_i2s.Settings->sys.full_duplex;
  int16_t pcm_buf[WEBRTC_AUDIO_FRAME_SAMPLES];
  uint16_t rx_gain = audio_i2s.Settings->rx.gain;  // Q12.4 format, 0x10 = 1.0
  int32_t dc_x_prev = 0, dc_y_prev = 0;
  bool dc_primed = false;

#ifdef USE_I2S_OPUS
  uint8_t opus_pkt[WEBRTC_OPUS_MAX_PACKET];
  AddLog(LOG_LEVEL_INFO, PSTR("WcAudio: Opus encoder, complexity=1, VOIP mode"));
#else
  uint8_t alaw_buf[WEBRTC_AUDIO_FRAME_SAMPLES];
#endif

  // Log I2S configuration for debugging
  AddLog(LOG_LEVEL_INFO, PSTR("WcAudio: I2S cfg: rate=%d gain=0x%04X(%.1fx) duplex=%d bits=%d channels=%d"),
         audio_i2s.Settings->rx.sample_rate, rx_gain, (float)rx_gain / 16.0f,
         is_duplex, audio_i2s.Settings->rx.slot_bit_width, audio_i2s.Settings->rx.channels);

  // In full duplex the RX channel delivers interleaved stereo frames (L/R),
  // even when only one mic is connected.  We read 2× the data and decimate.
  int16_t *stereo_buf = nullptr;
  size_t mono_bytes = WEBRTC_AUDIO_FRAME_SAMPLES * sizeof(int16_t);   // 320
  size_t read_bytes = mono_bytes;

  if (is_duplex) {
    stereo_buf = (int16_t*)malloc(mono_bytes * 2);
    if (!stereo_buf) {
      AddLog(LOG_LEVEL_ERROR, PSTR("WcAudio: stereo_buf alloc failed"));
      WcAudio->task_handle = nullptr;
      vTaskDelete(NULL);
      return;
    }
    read_bytes = mono_bytes * 2;   // 640
    AddLog(LOG_LEVEL_INFO, PSTR("WcAudio: full-duplex mode, reading %u bytes, gain=0x%04X (%.1fx)"),
           read_bytes, rx_gain, (float)rx_gain / 16.0f);
  }

  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xFrameInterval = pdMS_TO_TICKS(20);  // 20ms per frame

  while (!WcAudio->stop_requested) {
    // Check WebRTC state - bail if session ended
    if (!WebRTC || WebRTC->state != WEBRTC_STREAMING || !WebRTC->audio.active) {
      vTaskDelayUntil(&xLastWakeTime, xFrameInterval);
      continue;
    }

    size_t bytes_read;
    if (is_duplex) {
      // Full duplex: read stereo, decimate to mono (left channel)
      i2s_channel_read(audio_i2s.in->getRxHandle(), (void*)stereo_buf,
                       read_bytes, &bytes_read, pdMS_TO_TICKS(25));
      for (int i = 0; i < WEBRTC_AUDIO_FRAME_SAMPLES; i++) {
        pcm_buf[i] = stereo_buf[i * 2];
      }
      bytes_read /= 2;  // reflect mono sample count

      // Apply DC blocking filter (same algorithm as TasmotaI2S::dcFilter)
      if (bytes_read >= mono_bytes) {
        if (!dc_primed) {
          int32_t sum = 0;
          for (int i = 0; i < WEBRTC_AUDIO_FRAME_SAMPLES; i++) sum += pcm_buf[i];
          dc_x_prev = sum / WEBRTC_AUDIO_FRAME_SAMPLES;
          dc_y_prev = 0;
          dc_primed = true;
        }
        for (int i = 0; i < WEBRTC_AUDIO_FRAME_SAMPLES; i++) {
          int32_t x = pcm_buf[i];
          // y[n] = x[n] - x[n-1] + 0.9921875 * y[n-1]  (Q15: 32511/32768)
          int32_t y = x - dc_x_prev + (dc_y_prev * 32511 / 32768);
          dc_x_prev = x;
          dc_y_prev = y;
          // Apply gain (Q12.4) and clip
          int32_t val = (y * rx_gain) / 0x10;
          if (val > 32767) val = 32767;
          else if (val < -32768) val = -32768;
          pcm_buf[i] = (int16_t)val;
        }
      }
    } else {
      // Simplex / exclusive: read mono directly
      int32_t ret = audio_i2s.in->readMic((uint8_t*)pcm_buf, mono_bytes,
                                           true /*dc_block*/, true /*apply_gain*/,
                                           true /*lowpass*/, nullptr /*peak*/);
      bytes_read = (ret > 0) ? (size_t)ret : 0;
    }

    if (bytes_read >= mono_bytes) {
      // Compute PCM diagnostics: peak and DC offset
      int32_t sum = 0;
      int16_t peak = 0;
      for (int i = 0; i < WEBRTC_AUDIO_FRAME_SAMPLES; i++) {
        sum += pcm_buf[i];
        int16_t abs_val = (pcm_buf[i] < 0) ? -pcm_buf[i] : pcm_buf[i];
        if (abs_val > peak) peak = abs_val;
      }
      WcAudio->last_dc = sum / WEBRTC_AUDIO_FRAME_SAMPLES;
      WcAudio->last_peak = peak;
      WcAudio->frames_sent++;

      // Log every ~5 seconds (250 frames at 50fps)
      if ((WcAudio->frames_sent % 250) == 1) {
        AddLog(LOG_LEVEL_INFO, PSTR("WcAudio: frame=%u peak=%d dc=%d underruns=%u stack_free=%u pcm[0..7]=%d,%d,%d,%d,%d,%d,%d,%d"),
               WcAudio->frames_sent, peak, WcAudio->last_dc, WcAudio->underruns,
               (uint32_t)uxTaskGetStackHighWaterMark(NULL) * 4,
               pcm_buf[0], pcm_buf[1], pcm_buf[2], pcm_buf[3],
               pcm_buf[4], pcm_buf[5], pcm_buf[6], pcm_buf[7]);
        if (is_duplex && stereo_buf) {
          AddLog(LOG_LEVEL_INFO, PSTR("WcAudio: stereo[0..7]=%d,%d,%d,%d,%d,%d,%d,%d"),
                 stereo_buf[0], stereo_buf[1], stereo_buf[2], stereo_buf[3],
                 stereo_buf[4], stereo_buf[5], stereo_buf[6], stereo_buf[7]);
        }
      }

#ifdef USE_I2S_OPUS
      // Encode PCM16 → Opus
      opus_int32 enc_len = opus_encode(WcAudio->opus_enc, pcm_buf, WEBRTC_AUDIO_FRAME_SAMPLES, opus_pkt, WEBRTC_OPUS_MAX_PACKET);
      if (enc_len > 0) {
        WcSendSrtpPacket(&WebRTC->audio, opus_pkt, enc_len, false);
      }
#else
      // Encode PCM16 → G.711 A-law
      for (int i = 0; i < WEBRTC_AUDIO_FRAME_SAMPLES; i++) {
        alaw_buf[i] = linear_to_alaw(pcm_buf[i]);
      }
      WcSendSrtpPacket(&WebRTC->audio, alaw_buf, WEBRTC_AUDIO_FRAME_SAMPLES, false);
#endif
      WebRTC->audio.timestamp += WEBRTC_AUDIO_TS_INCREMENT;
      WebRTC->last_audio_time = millis();
    } else {
      // Underrun or mic not ready - send silence
      WcAudio->underruns++;
      WcSendAudioSilence();
    }

    vTaskDelayUntil(&xLastWakeTime, xFrameInterval);
  }

  if (stereo_buf) { free(stereo_buf); }

  AddLog(LOG_LEVEL_INFO, PSTR("WcAudio: Task exited, min stack free: %u"),
         (uint32_t)uxTaskGetStackHighWaterMark(NULL) * 4);

  WcAudio->task_handle = nullptr;
  vTaskDelete(NULL);
}

/*********************************************************************************************/
// Start / Stop
/*********************************************************************************************/

void WcAudioStart(void) {
  if (!WebRTC) return;

  // Guard: already running
  if (WcAudio && WcAudio->task_handle) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("WcAudio: Already running"));
    return;
  }

  // Guard: I2S input must be configured
  if (!audio_i2s.in) {
    AddLog(LOG_LEVEL_ERROR, PSTR("WcAudio: No I2S input configured"));
    return;
  }

  if (!WcAudio) {
    WcAudio = (WcAudio_State_t*)calloc(1, sizeof(WcAudio_State_t));
    if (!WcAudio) {
      AddLog(LOG_LEVEL_ERROR, PSTR("WcAudio: Alloc failed"));
      return;
    }
  }

  WcAudio->stop_requested = false;

#ifdef USE_I2S_OPUS
  // Create Opus encoder: 8 kHz, mono, VOIP mode
  int opus_err;
  WcAudio->opus_enc = opus_encoder_create(8000, 1, OPUS_APPLICATION_VOIP, &opus_err);
  if (opus_err != OPUS_OK || !WcAudio->opus_enc) {
    AddLog(LOG_LEVEL_ERROR, PSTR("WcAudio: Opus encoder create failed (%d)"), opus_err);
    free(WcAudio);
    WcAudio = nullptr;
    return;
  }
  opus_encoder_ctl(WcAudio->opus_enc, OPUS_SET_COMPLEXITY(1));
  opus_encoder_ctl(WcAudio->opus_enc, OPUS_SET_BITRATE(16000));
  AddLog(LOG_LEVEL_INFO, PSTR("WcAudio: Opus encoder created (8kHz mono, 16kbps, complexity=1)"));
#endif

  // Save previous sample rate and switch to 8000 Hz
  WcAudio->prev_sample_rate = audio_i2s.Settings->rx.sample_rate;
  AddLog(LOG_LEVEL_INFO, PSTR("WcAudio: Switching I2S RX from %d to 8000 Hz (duplex=%d)"),
         audio_i2s.Settings->rx.sample_rate, audio_i2s.Settings->sys.full_duplex);
  audio_i2s.Settings->rx.sample_rate = 8000;
  audio_i2s.in->SetRxRate(8000);

  // Start I2S RX
  audio_i2s.in->startRx();

  // Create audio task - Opus needs more stack than G.711
#ifdef USE_I2S_OPUS
  uint32_t stack_size = 8192 * 3;
#else
  uint32_t stack_size = 4096;
#endif
  esp_err_t err = xTaskCreatePinnedToCore(WcI2sWebRTCAudioTask, "WcAudio", stack_size,
                                           NULL, 3, &WcAudio->task_handle, 1);
  if (err != pdPASS) {
    AddLog(LOG_LEVEL_ERROR, PSTR("WcAudio: Task create failed (0x%x)"), err);
    audio_i2s.in->stopRx();
    audio_i2s.Settings->rx.sample_rate = WcAudio->prev_sample_rate;
#ifdef USE_I2S_OPUS
    opus_encoder_destroy(WcAudio->opus_enc);
#endif
    free(WcAudio);
    WcAudio = nullptr;
    return;
  }

#ifdef USE_I2S_OPUS
  AddLog(LOG_LEVEL_INFO, PSTR("WcAudio: Started (I2S @ 8000 Hz, Opus)"));
#else
  AddLog(LOG_LEVEL_INFO, PSTR("WcAudio: Started (I2S @ 8000 Hz, G.711 PCMA)"));
#endif
}

void WcAudioStop(void) {
  if (!WcAudio) return;

  AddLog(LOG_LEVEL_INFO, PSTR("WcAudio: Stopping"));

  // Signal task to stop
  WcAudio->stop_requested = true;

  // Wait for task to exit
  if (WcAudio->task_handle) {
    for (int i = 0; i < 50 && WcAudio->task_handle != nullptr; i++) {
      delay(10);
    }
    if (WcAudio->task_handle) {
      AddLog(LOG_LEVEL_ERROR, PSTR("WcAudio: Task didn't exit, force deleting"));
      vTaskDelete(WcAudio->task_handle);
      WcAudio->task_handle = nullptr;
    }
  }

  // Restore previous sample rate (handles duplex clock correctly)
  if (WcAudio->prev_sample_rate > 0 && audio_i2s.Settings->rx.sample_rate != WcAudio->prev_sample_rate) {
    AddLog(LOG_LEVEL_INFO, PSTR("WcAudio: Restoring I2S RX to %d Hz"), WcAudio->prev_sample_rate);
    audio_i2s.Settings->rx.sample_rate = WcAudio->prev_sample_rate;
    audio_i2s.in->SetRxRate(WcAudio->prev_sample_rate);
  }
  audio_i2s.in->stopRx();

#ifdef USE_I2S_OPUS
  if (WcAudio->opus_enc) {
    opus_encoder_destroy(WcAudio->opus_enc);
  }
#endif

  free(WcAudio);
  WcAudio = nullptr;

  AddLog(LOG_LEVEL_INFO, PSTR("WcAudio: Stopped"));
}

#endif  // USE_I2S_AUDIO
#endif  // USE_CSI_WEBCAM
#endif  // ESP32
