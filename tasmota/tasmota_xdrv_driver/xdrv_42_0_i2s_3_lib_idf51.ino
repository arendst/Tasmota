/*
  xdrv_42_0_i2s_3_lib_idf51.ino - Simplified Audio library, core class

  Copyright (C) 2021  Gerhard Mutz, Theo Arends, Staars, Stephan Hadinger

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

#include "AudioOutput.h"

// If DAC is not supported, provide some placeholders
#ifndef SOC_DAC_SUPPORTED
  #define dac_continuous_enable(...)        (0xFF)
  #define dac_continuous_disable(...)       (0xFF)
  #define dac_continuous_del_channels(...)  (0xFF)
  #define dac_continuous_write(...)         (0xFF)
#endif

/*********************************************************************************************\
 * Driver Settings in memory
\*********************************************************************************************/

/*********************************************************************************************\
 * Reminder of esp_err codes
 * 
 * #define ESP_OK          0       - esp_err_t value indicating success (no error) 
 * #define ESP_FAIL        -1      - Generic esp_err_t code indicating failure 
 * 
 * #define ESP_ERR_NO_MEM              0x101  257 - Out of memory 
 * #define ESP_ERR_INVALID_ARG         0x102  258 - Invalid argument 
 * #define ESP_ERR_INVALID_STATE       0x103  259 - Invalid state 
 * #define ESP_ERR_INVALID_SIZE        0x104  260 - Invalid size 
 * #define ESP_ERR_NOT_FOUND           0x105  261 - Requested resource not found 
 * #define ESP_ERR_NOT_SUPPORTED       0x106  262 - Operation or feature not supported 
 * #define ESP_ERR_TIMEOUT             0x107  263 - Operation timed out 
 * #define ESP_ERR_INVALID_RESPONSE    0x108  264 - Received response was invalid 
 * #define ESP_ERR_INVALID_CRC         0x109  265 - CRC or checksum was invalid 
 * #define ESP_ERR_INVALID_VERSION     0x10A  266 - Version was invalid 
 * #define ESP_ERR_INVALID_MAC         0x10B  267 - MAC address was invalid 
 * #define ESP_ERR_NOT_FINISHED        0x10C  268 - Operation has not fully completed 
\*********************************************************************************************/

/*********************************************************************************************\
 * This is the central class to acccess I2S in (rx) or out (tx)
 * 
 * It inherits from AudioOutput so it can be used as output instance for ESP8266Audio library
 * 
 * It also supports microphone input
 * 
\*********************************************************************************************/

class TasmotaI2S : public AudioOutput
{
public:

  // Constructor takes no parameter, everything is configured from template and config file
  TasmotaI2S() {
    // set some defaults
    hertz = 16000;
    bps = I2S_DATA_BIT_WIDTH_16BIT;   // bps sent to consumeAudio (this is different from channels of I2S stream)
    channels = 2;     // number of channels sent to consumeAudio (this is different from channels of I2S stream)
    gainF2P6 = 32;    // equivalent of 0.5
  }

  ~TasmotaI2S() {
    delRxHandle();
    delTxHandle();
  }

  /*********************************************************************************************\
   * Global state for this class
   * 
   * _exclusive:       true if Tx and Rx are sharing a common GPIO, so only Tx or Rx can be active at the same time
   * _is_dupleix:      (virtual) true if both Rx and Tx are configured, i.e. the same instance handles both Rx/Tx
   *                   Note: there are some restrictions on protocols, for ex. PDM is not supported in duplex,
   *                         and will trigger 'exclusive'
   * 
   * _t/rx_configured: true if Tx/Rx is configured, hence should be possible to start
   * _r/tx_handle:     handle for I2S driver, or nullptr if driver not stared
   * _t/rx_running:    true if Tx/Rx is actively running and sending/receiving data
   * 
  \*********************************************************************************************/

  // ------------------------------------------------------------------------------------------
  // Setters for configuration parameters
  //
  // ------------------------------------------------------------------------------------------
  // Settings
  void setPinout(int32_t bclk, int32_t ws, int32_t dout, int32_t mclk, int32_t din,
                 bool mclk_inv = false, bool bclk_inv = false, bool ws_inv = false, bool apll = false);

  void setSlotConfig(i2s_port_t i2s_port, uint8_t tx_slot_config,
                     uint8_t tx_slot_mask, uint8_t rx_slot_mask) {
    // _i2s_port = i2s_port;
    _tx_slot_config = tx_slot_config;
  }
  void setRxFreq(uint16_t freq) { _rx_freq = freq; }

  // Settings from superclass
  virtual bool SetBitsPerSample(int bits) {
    AddLog(LOG_LEVEL_DEBUG,"I2S: SetBitsPerSample: %i", bits);
    if ( (bits != 16) && (bits != 8) ) { return false; }
    this->bps = bits;
    return true;
  }

  virtual bool SetChannels(int channels) {
    AddLog(LOG_LEVEL_DEBUG,"I2S: SetChannels: %i", channels);
    if ((channels < 1) || (channels > 2)) { return false; }
    if (channels == (int)this->channels) { return true; }
    this->channels = channels;
    return true;
  }

  virtual bool SetRate(int hz) {
    AddLog(LOG_LEVEL_DEBUG,"I2S: SetRate: %i was %i tx_running=%i", hz, this->hertz, _tx_running);
    if (hz == (int) this->hertz) { return true; }
    this->hertz = hz;
    return updateClockConfig();
  }

  virtual bool SetGain(float f) {
    AddLog(LOG_LEVEL_DEBUG, "I2S: SetGain: %_f", &f);
    return AudioOutput::SetGain(f);
  }

  // ------------------------------------------------------------------------------------------
  // Getters
  inline bool isDuplex(void) const { return _tx_configured && _rx_configured; }

  inline bool getExclusive(void) const {return _exclusive; }
  inline uint8_t getTxMode(void) const { return _tx_mode; }
  inline uint8_t getTxChannels(void) const { return _tx_channels; }
  inline bool getTxRunning(void) const { return _tx_running; }
  inline i2s_chan_handle_t getTxHandle(void) const { return _tx_handle; }
#ifdef SOC_DAC_SUPPORTED
  inline bool isDACMode(void) const { return (_tx_mode == I2S_MODE_DAC); }
#else
  inline bool isDACMode(void) const { return false; }
#endif

  inline uint8_t getRxMode(void) const { return _rx_mode; }
  inline uint8_t getRxBitsPerSample(void) const { return 16; }      // TODO - hardcoded to 16 bits for recording
  inline uint16_t getRxRate(void) const { return _rx_freq; }
  inline uint8_t getRxChannels(void) const { return _rx_channels; }
  inline bool getRxRunning(void) const { return _rx_running; }
  inline i2s_chan_handle_t getRxHandle(void) const { return _rx_handle; }
  inline float getRxGain(void) const { return ((float)_rx_gain) / 16; }
  inline int32_t getRxDCOffset(void) const { return _rx_dc_offset; }
  inline uint16_t getRxLowpassAlpha(void) const { return ((float)_rx_lowpass_alpha / 0x8000); }

  // ------------------------------------------------------------------------------------------
  // Setters
  inline void setExclusive(bool exclusive) { _exclusive = exclusive; }
  inline void setTxMode(uint8_t mode) {
    _tx_mode = mode;
    if (_tx_mode == I2S_MODE_DAC) {
      _tx_configured = true;
    }
  }
  inline void setTxChannels(uint8_t channels) { _tx_channels = channels; }
  inline void setTxRunning(bool running) { _tx_running = running; }
  inline void setRxMode(uint8_t mode) { _rx_mode = mode; }
  inline void setRxChannels(uint8_t channels) { _rx_channels = channels; }
  inline void setRxRunning(bool running) { _rx_running = running; }
  inline void setRxGain(float f) { _rx_gain = (uint16_t)(f * 16); }

  void setRxLowpassAlpha(float f) {
    if (f < 0) { f = 0; }
    if (f > 1) { f = 1; }
    _rx_lowpass_alpha = (uint16_t)(f * 0x8000);
    _rx_lowpass_one_alpha = 0x8000 - _rx_lowpass_alpha;
    _rx_lowpass_y_prev = 0;
  }

  // ------------------------------------------------------------------------------------------
  // AudioOutput has inconsistent case for methods, provide consistent setters for super class
  inline void setRate(int hz) { SetRate(hz); } 
  inline void setBitsPerSample(int bits) { SetBitsPerSample(bits); }
  inline void setChannels(int chan) { SetChannels(chan); }
  inline void setGain(float f) { SetGain(f); }

  // Tx
  virtual bool begin(void) { return beginTx(); };   // the name `begin()`is inherited from superclass, prefer `beginTx()` which is more explicit
  virtual bool stop(void) { return stopTx(); };     // the name `stop()`is inherited from superclass, prefer `stopTx()` which is more explicit
  virtual void flush(void);                         // makes sure that all stored DMA samples are consumed / played to prevent static noise after stop()
  bool beginTx(void);
  bool stopTx(void);
  bool ConsumeSample(int16_t sample[2]);
  bool startI2SChannel(bool tx, bool rx);

  int32_t readMic(uint8_t *buffer, uint32_t size, bool dc_block, bool apply_gain, bool lowpass, uint32_t *peak_ptr);

  // The following is now the preferred function
  // and allows to send multiple samples at once
  //
  // Max 128 samples, it is clipped otherwise
  // Returns: the number of samples actually consumed
  //   or -1 if an error occured
  //
  // The call is non blocking and does not wait
  int32_t consumeSamples(int16_t *samples, size_t count);

  // ------------------------------------------------------------------------------------------
  // Microphone related methods
  uint32_t startRx(void);
  bool stopRx(void);

protected:
  int16_t dcFilter(int16_t pcm_in);
  int16_t lowpassFilter(int16_t pcm_in);
  bool updateClockConfig(void);

  bool delTxHandle(void);                 // remove handle
  bool delRxHandle(void);

protected:

  bool    _exclusive = false;             // in exclusive mode, stopping this instance needs to uninstall driver, and reinstall for next use
  i2s_port_t  _i2s_port = I2S_NUM_AUTO;   // I2S port, I2S_NUM_0/I2S_NUM_1/I2S_NUM_AUTO

  // local copy of useful settings for audio
  // TX
  bool      _tx_configured = false;       // true = configured, false = not configured
  uint8_t   _tx_mode = I2S_MODE_STD;      // I2S_MODE_STD / I2S_MODE_PDM / I2S_MODE_TDM / I2S_MODE_DAC
  uint8_t   _tx_slot_mask = BIT(0)|BIT(1);// default stereo
  bool      _tx_running = false;          // true = enabled, false = disabled
  uint8_t   _tx_channels = 2;             // number of channels, 1 = mono, 2 = stereo -- `channels`
  i2s_chan_handle_t _tx_handle = nullptr; // I2S channel handle, automatically computed
  uint8_t   _tx_slot_config = I2S_SLOT_MSB;// I2S slot configuration
  uint8_t   _tx_bps = 16;                 // bits per sample, 16 or 8

  // RX
  bool      _rx_configured = false;       // true = configured, false = not configured
  uint8_t   _rx_mode = I2S_MODE_STD;      // I2S_MODE_STD / I2S_MODE_PDM / I2S_MODE_TDM / I2S_MODE_DAC
  uint8_t   _rx_slot_mask = BIT(0);       // default mono: for PDM right, for standard left
  bool      _rx_running = false;          // true = enabled, false = disabled
  uint8_t   _rx_channels = 2;             // number of channels, 1 = mono, 2 = stereo
  i2s_chan_handle_t _rx_handle = nullptr; // I2S channel handle, automatically computed
  // uint8_t   _rx_slot_config;              // I2S slot configuration
  uint16_t  _rx_freq = 32000;             // I2S Rx sampling frequency in Hz - default 32 kHz to prevent problems with Shine Encoder
  uint8_t   _rx_bps = 16;                 // bits per sample, 16 or 8

  uint16_t  _rx_gain = 0x10;              // Microphone gain in Q12.4 format (0x10 = 1.0)
  int16_t   _rx_dc_offset = 0x8000;       // DC offset for PCM data, or 0x8000 if not set yet

  // GPIOs for I2S
  gpio_num_t  _gpio_mclk = GPIO_NUM_NC;   // GPIO for master clock
  gpio_num_t  _gpio_bclk = GPIO_NUM_NC;   // GPIO for bit clock
  gpio_num_t  _gpio_ws = GPIO_NUM_NC;     // GPIO for word select
  gpio_num_t  _gpio_dout = GPIO_NUM_NC;   // GPIO for data out
  gpio_num_t  _gpio_din = GPIO_NUM_NC;    // GPIO for data in
  bool    _gpio_mclk_inv = false;         // invert master clock
  bool    _gpio_bclk_inv = false;         // invert bit clock
  bool    _gpio_ws_inv = false;           // invert word select
  bool    _apll = false;                  // use APLL instead of PLL

  // DC blocking filter
  int16_t _rx_dc_x_prev = 0;
  int16_t _rx_dc_y_prev = 0;

  // Low-pass filter
  uint16_t  _rx_lowpass_alpha = 0;           // alpha parameter for lowpass filter Q1.15
  uint16_t  _rx_lowpass_one_alpha = 0x8000;  // 1 - alpha, Q1.15
  int16_t   _rx_lowpass_y_prev = 0;
};


// ------------------------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------------------------

void TasmotaI2S::setPinout(int32_t bclk, int32_t ws, int32_t dout, int32_t mclk, int32_t din,
                bool mclk_inv, bool bclk_inv, bool ws_inv, bool apll) {
  _gpio_mclk = (gpio_num_t) mclk;
  _gpio_bclk = (gpio_num_t) bclk;
  _gpio_ws   = (gpio_num_t) ws;
  _gpio_dout = (gpio_num_t) dout;
  _gpio_din  = (gpio_num_t) din;
  _gpio_mclk_inv = mclk_inv;
  _gpio_bclk_inv = bclk_inv;
  _gpio_ws_inv = ws_inv;
  _apll = apll;

  _tx_configured = (_gpio_dout != GPIO_NUM_NC);
  _rx_configured = (_gpio_din != GPIO_NUM_NC);

  AddLog(LOG_LEVEL_DEBUG, "I2S: setPinout: gpios[%i,%i,%i,%i,%i] inv[%i,%i,%i] apll:%i _tx_configured:%i _rx_configured:%i",
        _gpio_mclk, _gpio_bclk, _gpio_ws, _gpio_dout, _gpio_din,
        _gpio_mclk_inv, _gpio_bclk_inv, _gpio_ws_inv, _apll,
        _tx_configured, _rx_configured);
}

bool TasmotaI2S::beginTx(void) {
  AddLog(LOG_LEVEL_DEBUG, "I2S: Calling beginTX (tx_handle:%i already_running:%i)", _tx_handle, _tx_running);
  if (_tx_running) { return true; }

  if (!_tx_handle){
    if (!startI2SChannel(true, false)) {
      return false;
    }
  }

  esp_err_t err = ESP_OK;
#ifdef SOC_DAC_SUPPORTED
  if (isDACMode()) {
    err = dac_continuous_enable((dac_continuous_handle_t) _tx_handle);
    dac_task_start((dac_continuous_handle_t) _tx_handle);
  } else
#endif // SOC_DAC_SUPPORTED
  {
    err = i2s_channel_enable(_tx_handle);
  }
  AddLog(LOG_LEVEL_DEBUG, "I2S: Tx i2s_channel_enable err=0x%04X", err);
  if (err != ERR_OK){
    return false;
  }
  _tx_running = true;
  AddLog(LOG_LEVEL_DEBUG, "I2S: begin _tx_running succeeded");
  return true;
}

bool TasmotaI2S::stopTx() {
  esp_err_t err = ESP_OK;
  AddLog(LOG_LEVEL_DEBUG, "I2S: calling stopTx() tx_running:%i tx_handle:%p", _tx_running, _tx_handle);
  if (!_tx_configured) { return false; }      // invalid action
  if (!_tx_handle) { return true; }           // nothing to do
  if (_tx_running) {
    if (isDACMode()) {
      dac_task_stop();
      err = dac_continuous_disable((dac_continuous_handle_t) _tx_handle);
    } else {
      uint8_t zero_buffer[240] = {0};
      size_t sz;
      for(int i = 0;i < 6;i++){
        i2s_channel_write(_tx_handle, zero_buffer, sizeof(zero_buffer), &sz, 0); // fill DMA buffer with silence
      }
      err = i2s_channel_disable(_tx_handle);
    }
    AddLog(LOG_LEVEL_DEBUG, "I2S: stopTx i2s_channel_disable err=0x%04X", err);
    _tx_running = false;
  }
  if (_exclusive) {    // exclusive mode, deregister channel
    if (_tx_handle) {
      if (isDACMode()) {
        err = dac_continuous_del_channels((dac_continuous_handle_t) _tx_handle);
      } else {
        err = i2s_del_channel(_tx_handle);
      }
      AddLog(LOG_LEVEL_DEBUG, "I2S: stopTx i2s_del_channel err=0x%04X", err);
      _tx_handle = nullptr;
    }
    AddLog(LOG_LEVEL_DEBUG, "I2S: stop: I2S channel disabled");
  }
  return true;
}

void TasmotaI2S::flush()
{
    int buffersize = 6 * 240;
    int16_t samples[2] = {0x0, 0x0};
    for (int i = 0; i < buffersize; i++)
    {
      while (!ConsumeSample(samples))
      {
        delay(1);
      }
    }
    AddLog(LOG_LEVEL_DEBUG, "I2S: flush DMA TX buffer");
}

bool TasmotaI2S::delTxHandle(void) {
  esp_err_t err = ESP_OK;
  AddLog(LOG_LEVEL_DEBUG, "I2S: calling delTxHandle() tx_running:%i tx_handle:%p", _tx_running, _tx_handle);
  if (!_tx_configured) { return false; }      // invalid action
  if (!_tx_handle) { return true; }           // nothing to do
  if (_tx_running) { stopTx(); }

  if (isDACMode()) {
    err = dac_continuous_del_channels((dac_continuous_handle_t) _tx_handle);
  } else {
    err = i2s_del_channel(_tx_handle);
  }
  AddLog(LOG_LEVEL_DEBUG, "I2S: i2s_del_channel Tx err=0x%04X", err);
  _tx_handle = nullptr;
  return true;
}

bool TasmotaI2S::delRxHandle(void) {
  esp_err_t err = ESP_OK;
  AddLog(LOG_LEVEL_DEBUG, "I2S: calling delRxHandle() rx_running:%i rx_handle:%p", _rx_running, _rx_handle);
  if (!_rx_configured) { return false; }      // invalid action
  if (!_rx_handle) { return true; }           // nothing to do
  if (_rx_running) { stopRx(); }

  err = i2s_del_channel(_rx_handle);
  AddLog(LOG_LEVEL_DEBUG, "I2S: i2s_del_channel Rx err=0x%04X", err);
  _rx_handle = nullptr;
  return true;
}

bool TasmotaI2S::stopRx(void) {
  AddLog(LOG_LEVEL_DEBUG, "I2S: calling stopRx() rx_running:%i rx_handle:%p", _rx_running, _rx_handle);
  if (!_rx_configured) { return false; }    // nothing configured
  if (!_rx_handle) { return true; }  // bothing to do

  if (_rx_running) {
    esp_err_t err = i2s_channel_disable(_rx_handle);
    AddLog(LOG_LEVEL_DEBUG, "I2S: stopRx i2s_channel_disable err=0x%04X", err);
    _rx_running = false;
  }
  if (_exclusive) {
    if (_rx_handle) {
      esp_err_t err = i2s_del_channel(_rx_handle);
      AddLog(LOG_LEVEL_DEBUG, "I2S: stopRx i2s_del_channel err=0x%04X", err);
      _rx_handle = nullptr;
    }
  }
  return true;
}

bool TasmotaI2S::ConsumeSample(int16_t sample[2]) {
  return consumeSamples(sample, 1);
}

int32_t TasmotaI2S::consumeSamples(int16_t *samples, size_t count) {
  if (!_tx_running) { return -1; }
  if (count == 0) { return 0; }
  if (count > 128) { count = 128; }

  int16_t ms[count*2];
  for (int32_t i = 0; i < count; i++) {
    int16_t left = samples[i*2 + LEFTCHANNEL];
    int16_t right = samples[i*2 + RIGHTCHANNEL];

    if (channels == 1) {    // if mono, average the two samples
      // Average the two samples and overwrite
      int32_t ttl = left + right;
      left = right = (ttl>>1) & 0xffff;
    }

    if (bps == 8) {
      left = (((int16_t)(left & 0xff)) - 128) << 8;
      right = (((int16_t)(right & 0xff)) - 128) << 8;
    }

    // apply gain
    left = Amplify(left);
    right = Amplify(right);

    if (isDACMode()) {
      ms[i*2 + LEFTCHANNEL] = left + 0x8000;
      ms[i*2 + RIGHTCHANNEL] = right + 0x8000;
    } else {
      ms[i*2 + LEFTCHANNEL] = left;
      ms[i*2 + RIGHTCHANNEL] = right;
    }

  }

  // AddLog(LOG_LEVEL_DEBUG, "I2S: consumeSamples: left=%i right=%i", ms[0], ms[1]);

  size_t i2s_bytes_written;
  esp_err_t err = ESP_OK;
  if (isDACMode()) {
    i2s_bytes_written = send_dac_data((uint8_t*)ms, sizeof(ms));
  } else {
    err = i2s_channel_write(_tx_handle, ms, sizeof(ms), &i2s_bytes_written, 0);
  }
  if (err && err != ESP_ERR_TIMEOUT) {
    AddLog(LOG_LEVEL_INFO, "I2S: Could not write samples (count=%i): %i", count, err);
    return -1;
  }
  return i2s_bytes_written;
}

// Initialize I2S channel
// return `true` if successful, `false` otherwise
bool TasmotaI2S::startI2SChannel(bool tx, bool rx) {
  AddLog(LOG_LEVEL_DEBUG, "I2S: startI2SChannel: tx:%i rx:%i tx_configured:%i rx_configured:%i", tx, rx, _tx_configured, _rx_configured);

  if (tx && !_tx_configured) { return false; }
  if (rx && !_rx_configured) { return false; }
  if (rx && tx && _exclusive) { return false; }    // cannot initialize full-duplex with exclusive
  if (!tx && !rx) { return false; }     // nothing to do

  // check if already configured
  if (tx && rx && _tx_handle && _rx_handle) { return true; }
  if (tx && _tx_handle) { return true; }
  if (rx && _rx_handle) { return true; }

  if (_exclusive) {
    // in exclusive mode, we may need to remove exisiting driver
    if (tx && _rx_handle) {
      AddLog(LOG_LEVEL_DEBUG, "I2S: (exclusive mode) forcing stopRx");
      stopRx();
    }
    if (rx && _tx_handle) {
      AddLog(LOG_LEVEL_DEBUG, "I2S: (exclusive mode) forcing stopTx");
      stopTx();
    }
  }
  
  esp_err_t err = ESP_OK;
  gpio_num_t _DIN = rx ? (gpio_num_t)_gpio_din : I2S_GPIO_UNUSED;          // no input pin if no Rx

  if (tx && !isDACMode()) {
    // default dma_desc_num = 6 (DMA buffers), dma_frame_num = 240 (frames per buffer)
    i2s_chan_config_t tx_chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(_i2s_port, I2S_ROLE_MASTER);

    AddLog(LOG_LEVEL_DEBUG, "I2S: tx_chan_cfg id:%i role:%i dma_desc_num:%i dma_frame_num:%i auto_clear:%i",
          tx_chan_cfg.id, tx_chan_cfg.role, tx_chan_cfg.dma_desc_num, tx_chan_cfg.dma_frame_num, tx_chan_cfg.auto_clear);

    err = i2s_new_channel(&tx_chan_cfg, &_tx_handle, rx ? &_rx_handle : NULL);   // configure Rx only in duplex non-exclusive
    AddLog(LOG_LEVEL_DEBUG, "I2S: i2s_new_channel Tx err:0x%04X", err);
    if (err != ERR_OK) {
      _tx_handle = nullptr;
      return false;
    }

    // by default we configure for MSB 2 slots `I2S_STD_MSB_SLOT_DEFAULT_CONFIG`
    i2s_slot_mode_t tx_slot_mode = (_tx_channels == 1) ? I2S_SLOT_MODE_MONO : I2S_SLOT_MODE_STEREO;
    i2s_data_bit_width_t tx_data_bit_width = (_tx_bps == 8) ? I2S_DATA_BIT_WIDTH_8BIT : I2S_DATA_BIT_WIDTH_16BIT;
    i2s_std_config_t tx_std_cfg = {
      .clk_cfg  = I2S_STD_CLK_DEFAULT_CONFIG(hertz),
      .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(tx_data_bit_width, tx_slot_mode),
      .gpio_cfg = {
        .mclk = _gpio_mclk,
        .bclk = _gpio_bclk,
        .ws   = _gpio_ws,
        .dout = _gpio_dout,
        .din  =  _DIN,
        .invert_flags = {
          .mclk_inv = _gpio_mclk_inv,
          .bclk_inv = _gpio_bclk_inv,
          .ws_inv   = _gpio_ws_inv,
        }
      }
    };

    // change configuration if we are using PCM or PHILIPS
    if (_tx_slot_config == I2S_SLOT_PCM) {    // PCM
      tx_std_cfg.slot_cfg = I2S_STD_PCM_SLOT_DEFAULT_CONFIG(tx_data_bit_width, tx_slot_mode);
    } else if (_tx_slot_config == I2S_SLOT_PHILIPS) { // PHILIPS
      tx_std_cfg.slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(tx_data_bit_width, tx_slot_mode);
    }
    tx_std_cfg.slot_cfg.slot_mask = (i2s_std_slot_mask_t)_tx_slot_mask;
#if SOC_I2S_SUPPORTS_APLL
    if (_apll) { tx_std_cfg.clk_cfg.clk_src = I2S_CLK_SRC_APLL; }
#endif // SOC_I2S_SUPPORTS_APLL

    err = i2s_channel_init_std_mode(_tx_handle, &tx_std_cfg);
    AddLog(LOG_LEVEL_DEBUG, "I2S: i2s_channel_init_std_mode TX channel bits:%i channels:%i hertz:%i err=0x%04X", bps, channels, hertz, err);
    if (err != ERR_OK) {
      _tx_handle = nullptr;
      return false;
    }

    if (rx) {   // full duplex mode
      err = i2s_channel_init_std_mode(_rx_handle, &tx_std_cfg);
      AddLog(LOG_LEVEL_DEBUG, "I2S: i2s_channel_init_std_mode err:%i", err);
      AddLog(LOG_LEVEL_DEBUG, "I2S: RX channel added in full duplex mode");
    }
  }   // if (tx)

#ifdef SOC_DAC_SUPPORTED
  if (tx && isDACMode()) {
    dac_continuous_config_t dac_chan_cfg = {
      .chan_mask = DAC_CHANNEL_MASK_ALL,
      .desc_num = 6,
      .buf_size = 240,
      .freq_hz = hertz,
      .offset = 0,
      .clk_src = DAC_DIGI_CLK_SRC_APLL, /*DAC_DIGI_CLK_SRC_DEFAULT*/
      .chan_mode = DAC_CHANNEL_MODE_ALTER,
    };
    // AddLog(LOG_LEVEL_DEBUG, "I2S: dac_chan_cfg chan_mask:%i clk_src:%i chan_mode:%i",
    //   dac_chan_cfg.chan_mask, dac_chan_cfg.clk_src, dac_chan_cfg.chan_mode);

    err = dac_continuous_new_channels(&dac_chan_cfg, (dac_continuous_handle_t*) &_tx_handle);
    AddLog(LOG_LEVEL_DEBUG, "I2S: dac_new_channel Tx err:0x%04X", err);
    if (err != ERR_OK) {
      _tx_handle = nullptr;
      return false;
    }
  }   // if (tx) && DAC
#endif // SOC_DAC_SUPPORTED

  // configure Rx Microphone
  if (rx) {
    gpio_num_t clk_gpio;

    i2s_slot_mode_t rx_slot_mode = (_rx_channels == 1) ? I2S_SLOT_MODE_MONO : I2S_SLOT_MODE_STEREO;
    AddLog(LOG_LEVEL_DEBUG, "I2S: mic init rx_channels:%i rx_running:%i rx_handle:%p", rx_slot_mode, _rx_running, _rx_handle);

    i2s_chan_config_t rx_chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(_i2s_port, I2S_ROLE_MASTER);
    i2s_data_bit_width_t rx_data_bit_width = (_rx_bps == 8) ? I2S_DATA_BIT_WIDTH_8BIT : I2S_DATA_BIT_WIDTH_16BIT;
    // defaults to 3 buffers of 512 samples
    rx_chan_cfg.dma_desc_num = audio_i2s.Settings->rx.dma_desc_num;
    rx_chan_cfg.dma_frame_num = audio_i2s.Settings->rx.dma_frame_num;

    AddLog(LOG_LEVEL_DEBUG, "I2S: rx_chan_cfg id:%i role:%i dma_desc_num:%i dma_frame_num:%i auto_clear:%i",
          rx_chan_cfg.id, rx_chan_cfg.role, rx_chan_cfg.dma_desc_num, rx_chan_cfg.dma_frame_num, rx_chan_cfg.auto_clear);

    err = i2s_new_channel(&rx_chan_cfg, NULL, &_rx_handle);
    AddLog(LOG_LEVEL_DEBUG, "I2S: i2s_new_channel Rx err:%i", err);
    switch (_rx_mode){
#if SOC_I2S_SUPPORTS_PDM_RX
      case I2S_MODE_PDM:
        {
          i2s_pdm_rx_config_t rx_pdm_cfg = {
            .clk_cfg = I2S_PDM_RX_CLK_DEFAULT_CONFIG(_rx_freq),
            /* The default mono slot is the left slot (whose 'select pin' of the PDM microphone is pulled down) */
            .slot_cfg = I2S_PDM_RX_SLOT_DEFAULT_CONFIG(rx_data_bit_width, rx_slot_mode),
            .gpio_cfg = {
              .clk = _gpio_ws,
              .din = _gpio_din,
              .invert_flags = {
                .clk_inv = _gpio_ws_inv,
              },
            },
          };
          rx_pdm_cfg.slot_cfg.slot_mask = (i2s_pdm_slot_mask_t)_rx_slot_mask;

          // AddLog(LOG_LEVEL_DEBUG, "I2S: rx_pdm_cfg clk_cfg sample_rate_hz:%i clk_src:%i mclk_multiple:%i dn_sample_mode:%i",
          //       rx_pdm_cfg.clk_cfg.sample_rate_hz, rx_pdm_cfg.clk_cfg.clk_src, rx_pdm_cfg.clk_cfg.mclk_multiple, rx_pdm_cfg.clk_cfg.dn_sample_mode);

          // AddLog(LOG_LEVEL_DEBUG, "I2S: rx_pdm_cfg slot_cfg data_bit_width:%i slot_bit_width:%i slot_mode:%i slot_mask:%i",
          //       rx_pdm_cfg.slot_cfg.data_bit_width, rx_pdm_cfg.slot_cfg.slot_bit_width, rx_pdm_cfg.slot_cfg.slot_mode, rx_pdm_cfg.slot_cfg.slot_mask);

          // AddLog(LOG_LEVEL_DEBUG, "I2S: rx_pdm_cfg gpio_cfg clk:%i din:%i clk_inv:%i",
          //       rx_pdm_cfg.gpio_cfg.clk, rx_pdm_cfg.gpio_cfg.din, rx_pdm_cfg.gpio_cfg.invert_flags.clk_inv);

          err = i2s_channel_init_pdm_rx_mode(_rx_handle, &rx_pdm_cfg);
          AddLog(LOG_LEVEL_DEBUG, "I2S: RX channel in PDM mode, CLK: %i, DIN: %i, 16 bit width, %i channel(s), err code: 0x%04X",
                  _gpio_ws, _gpio_din, _rx_channels, err);
          if (err) {
            _rx_handle = nullptr;
            return false;
          }
        }
        break;
#endif // SOC_I2S_SUPPORTS_PDM_RX
      case I2S_MODE_STD:
        {
          i2s_std_slot_config_t _slot_cfg = {
            .data_bit_width = rx_data_bit_width,
            .slot_bit_width = (i2s_slot_bit_width_t)audio_i2s.Settings->rx.slot_bit_width,
            .slot_mode =  rx_slot_mode,
            .slot_mask =  (i2s_std_slot_mask_t)_rx_slot_mask,
            .ws_width = audio_i2s.Settings->rx.ws_width,
            .ws_pol = audio_i2s.Settings->rx.ws_pol,
            .bit_shift = audio_i2s.Settings->rx.bit_shift,
#if SOC_I2S_HW_VERSION_1    // For esp32/esp32-s2
            .msb_right = audio_i2s.Settings->rx.bit_order_lsb, // Placeholder for now!!
#else
            .left_align = audio_i2s.Settings->rx.left_align,
            .big_endian = audio_i2s.Settings->rx.big_endian,
            .bit_order_lsb = audio_i2s.Settings->rx.bit_order_lsb,
#endif
          };
          i2s_std_config_t rx_std_cfg = {
            .clk_cfg  = I2S_STD_CLK_DEFAULT_CONFIG(_rx_freq),
            .slot_cfg = _slot_cfg,
            .gpio_cfg = {
              .mclk = (gpio_num_t)Pin(GPIO_I2S_MCLK),
              .bclk = (gpio_num_t)Pin(GPIO_I2S_BCLK),
              .ws   = (gpio_num_t)Pin(GPIO_I2S_WS),
              .dout = I2S_GPIO_UNUSED,
              .din  = (gpio_num_t)Pin(GPIO_I2S_DIN),
              .invert_flags = {
                .mclk_inv = false,
                .bclk_inv = false,
                .ws_inv   = false,
              },
            },
          };
#if SOC_I2S_SUPPORTS_APLL
          if(audio_i2s.Settings->rx.apll == 1){
              rx_std_cfg.clk_cfg.clk_src = I2S_CLK_SRC_APLL;
          }
#endif //SOC_I2S_SUPPORTS_APLL
          err = i2s_channel_init_std_mode(_rx_handle, &rx_std_cfg);
          AddLog(LOG_LEVEL_DEBUG, "I2S: RX i2s_channel_init_std_mode with err:%i", err);
          AddLog(LOG_LEVEL_DEBUG, "I2S: RX channel in standard mode with %u bit width on %i channel(s) initialized", bps, rx_slot_mode);
          if (err) {
            _rx_handle = nullptr;
            return false;
          }
        }
        break;
      default:
        AddLog(LOG_LEVEL_INFO, "I2S: invalid rx mode=%i", _rx_mode);
    }
  }
  return true;
}

// called only if Tx frequency is changed
bool TasmotaI2S::updateClockConfig(void) {
  if (!_tx_handle) { return true; }

  // I2S mode
  if (_tx_mode != I2S_MODE_DAC) {
    if (_tx_running) {
      esp_err_t err = i2s_channel_disable(_tx_handle);
      AddLog(LOG_LEVEL_INFO, "I2S: updateClockConfig i2s_channel_disable err=0x%04X", err);
    }
    i2s_std_clk_config_t clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(hertz);
  #ifdef SOC_I2S_SUPPORTS_APLL
    if (_apll) {
      clk_cfg.clk_src = I2S_CLK_SRC_APLL;
    }
  #endif
    esp_err_t result = i2s_channel_reconfig_std_clock(_tx_handle, &clk_cfg);
    AddLog(LOG_LEVEL_INFO, "I2S: updateClockConfig i2s_channel_reconfig_std_clock err=0x%04X", result);
    if (_tx_running) { 
      esp_err_t err = i2s_channel_enable(_tx_handle);
      AddLog(LOG_LEVEL_INFO, "I2S: updateClockConfig i2s_channel_enable err=0x%04X", err);
    }
    AddLog(LOG_LEVEL_DEBUG, "I2S: Updating clock config");
    return result == ESP_OK;
  }  else {

    // DAC mode
    // It looks like you can't change the DAC frequency without removing and recreating the DAC diver
    delTxHandle();
    AddLog(LOG_LEVEL_DEBUG, "I2S: Updating DAC clock config");
    return startI2SChannel(true, false);
  }

}

/*********************************************************************************************\
 * microphone related functions
\*********************************************************************************************/

uint32_t TasmotaI2S::startRx(void) {
  AddLog(LOG_LEVEL_DEBUG, "I2S: startRx called");
  if (!_rx_configured) { return 0; }   // nothing configured

  if (!_rx_handle){
    startI2SChannel(false, true);
  }

  esp_err_t err = ESP_OK;
  gpio_num_t clk_gpio;

  i2s_slot_mode_t rx_slot_mode = (_rx_channels == 1) ? I2S_SLOT_MODE_MONO : I2S_SLOT_MODE_STEREO;
  AddLog(LOG_LEVEL_DEBUG, "I2S: mic init rx_channels:%i rx_running:%i rx_handle:%p", rx_slot_mode, _rx_running, _rx_handle);

  if (!_rx_running) {
    err = i2s_channel_enable(_rx_handle);
    AddLog(LOG_LEVEL_DEBUG, "I2S: RX channel enable err:0x%04X", err);
    _rx_running = true;
  }
  return err;
}

// Read data into buffer of uint16_t[]
//
// Returns:
//  n<0: error occured
//  0: no data available
//  n>0: number of bytes read
//  peak: peak value of the mic (can go above 32767 to indicate clipping)
int32_t TasmotaI2S::readMic(uint8_t *buffer, uint32_t size, bool dc_block, bool apply_gain, bool lowpass, uint32_t *peak_ptr) {
  uint32_t peak = 0;
  if (peak_ptr) { *peak_ptr = peak; }

  if (!getRxRunning()) { return -1; }

  size_t btr = 0;
  esp_err_t err = i2s_channel_read(_rx_handle, buffer, size, &btr, 0 /* do not wait */);
  if ((err != ESP_OK) && (err != ESP_ERR_TIMEOUT)) { return -err; }
  if (btr == 0) { return 0; }
  int16_t *samples = (int16_t*) buffer;
  size_t samples_count = btr / 2;   // 16 bits signed samples

  // Apply DC block
  if (dc_block) {
    if (_rx_dc_x_prev == 0 && _rx_dc_y_prev == 0) {
      int32_t sum_samples = 0;
      for (uint32_t i=0; i<samples_count; i++) {
        sum_samples += samples[i];
      }
      _rx_dc_offset = sum_samples / samples_count;
      _rx_dc_x_prev = _rx_dc_offset;
      _rx_dc_y_prev = 0;
    }
    // apply dc_blocking filter
    for (uint32_t i=0; i<samples_count; i++) {
      samples[i] = dcFilter(samples[i]);
    }
    _rx_dc_offset = _rx_dc_x_prev - _rx_dc_y_prev;    // update new offset
  }

  // apply gain
  for (uint32_t i=0; i<samples_count; i++) {
    int32_t val = samples[i];
    if (apply_gain) {
      val = (val * _rx_gain) / 0x10;
    }
    // clipping if overflow, and compute peak value
    if (val > 0x7FFF) {
      if (val > peak) { peak = val; }     // clipping
      val = 0x7FFF;
    } else if (val < -0x8000) {
      val = -0x8000;
      if (-val > peak) { peak = -val; }     // clipping
    } else {
      if ((val > 0) && (val > peak)) { peak = val; }
      if ((val < 0) && (-val > peak)) { peak = -val; }
    }
    samples[i] = val;
  }

  // lowpass filter
  if (lowpass && _rx_lowpass_alpha != 0) {
    // apply lowpass filter
    for (uint32_t i=0; i<samples_count; i++) {
      samples[i] = lowpassFilter(samples[i]);
    }
  }

  if (peak_ptr) { *peak_ptr = peak; }
  return btr;
}

// DC Blocking filter
// 1 - 2^(-7) = 0.9921875
// binary: 0.111111011111111    Q1.15
int16_t TasmotaI2S::dcFilter(int16_t pcm_in) {
  #define A1 32511        // (1-2^(-7)) Q32:1.31
  int16_t delta_x;
  int32_t a1_y_prev;
  int16_t pcm_out;

  delta_x = pcm_in - _rx_dc_x_prev;
  a1_y_prev = (A1 * (int32_t)_rx_dc_y_prev) / 0x8000;
  pcm_out = delta_x + (int16_t)a1_y_prev;
  _rx_dc_x_prev = pcm_in;
  _rx_dc_y_prev = pcm_out;
  return pcm_out;
}

// low pass filter
// See: https://en.wikipedia.org/wiki/Low-pass_filter#Simple_infinite_impulse_response_filter
// For 3000Hz low pass filter, RC = 0.0000530786
// dt = 1/16000 = 0.0000625
// alpha = dt / (RC + dt) = 0.540757545
// alpha = (b) 0.100010100110111 = 0x4537
// 1 - alpha = 0x3A9C
int16_t TasmotaI2S::lowpassFilter(int16_t pcm_in) {
  // const int32_t alpha = 0x4537;
  // const int32_t one_alpha = 0x3A9C;
  int32_t a1_y_prev;
  int16_t pcm_out;

  a1_y_prev = (_rx_lowpass_alpha * _rx_lowpass_y_prev) / 0x8000 + (_rx_lowpass_one_alpha * pcm_in) / 0x8000;
  pcm_out = (int16_t)a1_y_prev;
  _rx_lowpass_y_prev = pcm_out;
  return pcm_out;
}

#endif // USE_I2S_AUDIO
#endif // defined(ESP32) && ESP_IDF_VERSION_MAJOR >= 5
