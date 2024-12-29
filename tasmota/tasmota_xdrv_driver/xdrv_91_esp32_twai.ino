/*
  xdrv_91_esp32_twai.ino - ESP32 TWAI support for Tasmota

  SPDX-FileCopyrightText: 2024 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef ESP32
#ifdef USE_ESP32_TWAI
/*********************************************************************************************\
 * ESP32 Two-Wire Automotive Interface (TWAI) or Controller Area Network (CAN) - needs external tranceiver
 * 
 * The TWAI controller is not compatible with ISO11898-1 FD Format frames, and will interpret such frames as errors.
 * CAN bus message format is 32-bit identifier and up to 8 bytes of data
 * 
 * You'll needs a berry driver with class "twai" and functions "config" and "decode" as shown below
\*********************************************************************************************/

#define XDRV_91               91

#define TWAI_POLLING_RATE_MS  100

#include "driver/twai.h"

enum TwaiSpeeds { TWAI_SPEED_25KBITS = 0,
                  TWAI_SPEED_50KBITS,
                  TWAI_SPEED_100KBITS,
                  TWAI_SPEED_125KBITS,
                  TWAI_SPEED_250KBITS,
                  TWAI_SPEED_500KBITS,
                  TWAI_SPEED_800KBITS,
                  TWAI_SPEED_1MBITS,
                  TWAI_SPEED_MAX };

const char kTwaiSpeeds[] PROGMEM = "25K|50K|100K|125K|250K|500K|800K|1M";
const char kTwaiModes[] PROGMEM = "Normal|No Ack|Listen Only";

struct TWAIs {
  twai_mode_t mode;
  uint8_t speed;
  bool supported;
  bool installed;
} Twai;

#ifdef USE_BERRY
/*********************************************************************************************\
 * Berry example code
\*********************************************************************************************/

/*
file twai.be contents:

tasmota.remove_driver(twai)

class twai_cls
  var active                                      # (bool)
  var twai_speed, twai_mode                       # (int, int)
  var am012_status, am014_substatus, am024_power  # (int, int, int)
  var pressure, setpoint, flow_temp               # (float, float, float)

  def init()
    self.active = 0
    self.twai_speed = 7        # 0 = 25K, 1 = 50K, 2 = 100K, 3 = 125K, 4 = 250K, 5 = 500K, 6 = 800K, 7 = 1Mbits
    self.twai_mode = 2         # 0 = TWAI_MODE_NORMAL, 1 = TWAI_MODE_NO_ACK, 2 = TWAI_MODE_LISTEN_ONLY
    self.am012_status = 0
    self.am014_substatus = 0
    self.am024_power = 0
    self.pressure = 0
    self.setpoint = 0
    self.flow_temp = 0
  end

  def config()
    return self.twai_mode << 3 | self.twai_speed
  end

  def decode(ident, data1, data2)
    var id = ident & 0xfff
    if id == 0x076
#      tasmota.log(f"RMH: 0x{id:03x} Count {data1}", 3)
    elif id == 0x100
      var time = 441763200 + (data2 * 24 * 60 * 60) + (data1 / 1000)
#      tasmota.log(f"RMH: 0x{id:03x} Time {tasmota.time_str(time)}", 3)
    elif id == 0x1C1
      if data1 & 0x3ff == 0x300
        self.pressure = (data2 & 0xff00) / 2560.0
      end
    elif id == 0x382
      self.am024_power = data1 & 0xff              # Relative power
      self.setpoint = (data1 & 0xffff00)/25600.0   # Setpoint
#      tasmota.log(f"RMH: 0x{id:03x} Busy {self.am024_power}%, Setpoint {self.setpoint}", 3)
    elif id == 0x282
      self.flow_temp = (data1 & 0xffff00)/25600.0
#      tasmota.log(f"RMH: 0x{id:03x} DHW temp {self.flow_temp}", 3)
    elif id == 0x481
      self.am012_status = data1 & 0xff
      self.am014_substatus = (data1 & 0xff00)/256
    else      
      return
    end
    self.active = 1                   # At least one valid decode
  end

  #- add sensor value to teleperiod -#
  def json_append()
    if !self.active return nil end    # Exit if never decoded something
    import string
    var msg = string.format(",\"Calenta\":{\"AM012\":%i,\"AM014\":%i,\"Pressure\":%.1f,\"Setpoint\":%.1f,\"Flow\":%.1f}",
                             self.am012_status, self.am014_substatus, self.pressure, self.setpoint, self.flow_temp)
    tasmota.response_append(msg)
  end

  #- display sensor value in the web UI -#
  def web_sensor()
    if !self.active return nil end    # Exit if never decoded something
    import string
    var msg = string.format("{s}AM012/AM014 State{m}%i/%i{e}"..
                            "{s}AM024 Relative Power{m}%i %%{e}"..
                            "{s}Pressure{m}%.1f{e}"..
                            "{s}Setpoint Temperature{m}%.1f{e}"..
                            "{s}Flow Temperature{m}%.1f{e}",
                            self.am012_status, self.am014_substatus, self.am024_power, self.pressure, self.setpoint, self.flow_temp)
    tasmota.web_send_decimal(msg)
  end
end
twai = twai_cls()
tasmota.add_driver(twai)

// *******************************************************************************************

file preinit.be contents:

load('twai.be')
*/

/*********************************************************************************************\
 * Berry interface
\*********************************************************************************************/

bool TWAIBerryDecode(uint32_t identifier, uint32_t data_length_code, uint8_t *data) {
  bvm *vm = berry.vm;
  if (nullptr == vm) { return false; }
  bool handled = false;
  if (be_getglobal(vm, "twai")) {         // Look for class
    if (be_getmember(vm, 1, "decode")) {  //  and function
      be_pushvalue(vm, -2);

      uint32_t ident = identifier | data_length_code << 28;
      uint32_t data1 = data[3] << 24 | data[2] << 16 | data[1] << 8 | data[0];
      uint32_t data2 = data[7] << 24 | data[6] << 16 | data[5] << 8 | data[4];
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("TWA: ident %08X, data1 %08X, data2 %08X"), ident, data1, data2);

      be_pushint(vm, ident);
      be_pushint(vm, data1);
      be_pushint(vm, data2);
      int32_t ret = be_pcall(vm, 4);      // Number of arguments, self, ident. data1, data2
      if (ret != 0) {
        be_error_pop_all(vm);             // Clear Berry stack
      }
      handled = true;
    }
  }
  be_pop(vm, be_top(vm));                 // Clean
  return handled;
}

/*********************************************************************************************/

bool TWAIBerryConfig(void) {
  bvm *vm = berry.vm;
  if (nullptr == vm) { return false; }
  bool handled = false;
  if (be_getglobal(vm, "twai")) {         // Look for class
    if (be_getmember(vm, 1, "config")) {  //  and function
      be_pushvalue(vm, -2);
      int32_t ret = be_pcall(vm, 1);      // Number of arguments, self
      if (ret != 0) {
        be_error_pop_all(vm);             // Clear Berry stack
      }
      be_pop(vm, 1);
      if (be_isint(vm, -1)) {
        uint32_t config = be_toint(vm, -1);
        Twai.speed = config & 0x7;        // User input check 0..7
        uint32_t mode = config >> 3 & 0x3;
        if (mode > 2) { mode = 2; }       // User input check 0..2
        Twai.mode = (twai_mode_t)mode;
        handled = true;
      }
    }
  }
  be_pop(vm, be_top(vm));                 // Clean
  return handled;
}

#endif  // USE_BERRY

/*********************************************************************************************\
 * TWAI low level
\*********************************************************************************************/

bool TWAIStart(void) {
  twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)Pin(GPIO_TWAI_TX), (gpio_num_t)Pin(GPIO_TWAI_RX), Twai.mode);
  g_config.rx_queue_len = 32;

  twai_timing_config_t t_config;
  switch (Twai.speed) {
    case 0:
      t_config = TWAI_TIMING_CONFIG_25KBITS();
      break;
    case 1:
      t_config = TWAI_TIMING_CONFIG_50KBITS();
      break;
    case 2:
      t_config = TWAI_TIMING_CONFIG_100KBITS();
      break;
    case 3:
      t_config = TWAI_TIMING_CONFIG_125KBITS();
      break;
    case 4:
      t_config = TWAI_TIMING_CONFIG_250KBITS();
      break;
    case 5:
      t_config = TWAI_TIMING_CONFIG_500KBITS();
      break;
    case 6:
      t_config = TWAI_TIMING_CONFIG_800KBITS();
      break;
    case 7:
      t_config = TWAI_TIMING_CONFIG_1MBITS();
      break;
    default:
      t_config = TWAI_TIMING_CONFIG_125KBITS();
      break;
  }

  twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

  if (twai_driver_install(&g_config, &t_config, &f_config) != ESP_OK) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("TWA: Not installed"));
    return false;
  }
  if (twai_start() != ESP_OK) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("TWA: Not started"));
    return false;
  }
  // Reconfigure alerts to detect frame receive, Bus-Off error and RX queue full states
  uint32_t alerts_to_enable = TWAI_ALERT_RX_DATA | TWAI_ALERT_RX_QUEUE_FULL | TWAI_ALERT_TX_IDLE | TWAI_ALERT_TX_SUCCESS | TWAI_ALERT_TX_FAILED | TWAI_ALERT_ERR_PASS | TWAI_ALERT_BUS_ERROR;
  if (twai_reconfigure_alerts(alerts_to_enable, NULL) != ESP_OK) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("TWA: Failed to reconfigure CAN alerts"));
    return false;
  } 
  Twai.installed = true;

  char smode[16];
  GetTextIndexed(smode, sizeof(smode), Twai.mode, kTwaiModes);
  char sspeed[16];
  GetTextIndexed(sspeed, sizeof(sspeed), Twai.speed, kTwaiSpeeds);
  AddLog(LOG_LEVEL_DEBUG, PSTR("TWA: Started in %s Mode and %sbit/s"), smode, sspeed);
  return true;
}

/*********************************************************************************************/

void TWAIStop(void) {
  if (Twai.installed) {
    twai_stop();
    twai_driver_uninstall();
    Twai.installed = false;
    AddLog(LOG_LEVEL_DEBUG, PSTR("TWA: Stopped"));
  }
}

/*********************************************************************************************/

uint32_t TWAICheckAlerts(void) {
  uint32_t alerts_triggered;
  twai_read_alerts(&alerts_triggered, pdMS_TO_TICKS(TWAI_POLLING_RATE_MS));
  twai_status_info_t twaistatus;
  twai_get_status_info(&twaistatus);

  // Handle alerts
  if (alerts_triggered & TWAI_ALERT_ERR_PASS) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("TWA: Alert - Controller has become error passive"));
  }
  if (alerts_triggered & TWAI_ALERT_BUS_ERROR) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("TWA: Alert - A Bit, Stuff, CRC, Form or ACK error has occurred on the bus. Errors %d"), twaistatus.bus_error_count);
  }
  if (alerts_triggered & TWAI_ALERT_RX_QUEUE_FULL) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("TWA: Alert - The RX queue is full causing a received frame to be lost. Buffered %d, Missed %d, Overrun %d"),
      twaistatus.msgs_to_rx, twaistatus.rx_missed_count, twaistatus.rx_overrun_count);
  }
  if (alerts_triggered & TWAI_ALERT_TX_FAILED) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("TWA: Alert - The Transmission failed. Buffered %d, Error %d, Failed %d"),
      twaistatus.msgs_to_tx, twaistatus.tx_error_counter, twaistatus.tx_failed_count);
  }
  if (alerts_triggered & TWAI_ALERT_TX_SUCCESS) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("TWA: Alert - The Transmission was successful. Buffered %d"), twaistatus.msgs_to_tx);
  }

  return alerts_triggered;
}

/*********************************************************************************************/

void TWAIRead(void) {
  uint32_t alerts_triggered = TWAICheckAlerts();
  // Check if message is received
  if (alerts_triggered & TWAI_ALERT_RX_DATA) {
    // One or more messages received. Handle all.
    twai_message_t message;
    while (twai_receive(&message, 0) == ESP_OK) {
#ifdef USE_BERRY
      if (TWAIBerryDecode(message.identifier, message.data_length_code, message.data)) { continue; }
#endif  // USE_BERRY
      // Show log if no berry found (Messages come too fast for supporting MQTT at this time)
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("TWA: Id 0x%08X, Rcvd '%*_H'"),
        message.identifier, message.data_length_code, message.data);
    }
  }
}

/*********************************************************************************************\
 * Initialization
\*********************************************************************************************/

void TWAIInit(void) {
  if (!PinUsed(GPIO_TWAI_TX) || !PinUsed(GPIO_TWAI_RX)) { return; }

  Twai.speed = TWAI_SPEED_1MBITS;
  Twai.mode = TWAI_MODE_LISTEN_ONLY;  // 0 = TWAI_MODE_NORMAL, 1 = TWAI_MODE_NO_ACK, 2 = TWAI_MODE_LISTEN_ONLY
#ifdef USE_BERRY
  TWAIBerryConfig();
#endif  // USE_BERRY
  if (TWAIStart()) { 
    Twai.supported = true;
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv91(uint32_t function) {
  bool result = false;

  if (FUNC_INIT == function) {
    TWAIInit();
  }
  else if (Twai.supported) {
    switch (function) {
      case FUNC_LOOP:
        if (Twai.installed) {
          TWAIRead();
        }
        break;
    }
  }
  return result;
}

#endif  // USE_ESP32_TWAI
#endif  // ESP32