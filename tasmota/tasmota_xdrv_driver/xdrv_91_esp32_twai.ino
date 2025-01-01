/*
  xdrv_91_esp32_twai.ino - ESP32 TWAI support for Tasmota

  SPDX-FileCopyrightText: 2025 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef ESP32
#ifdef USE_ESP32_TWAI
#if SOC_TWAI_SUPPORTED
/*********************************************************************************************\
 * ESP32 Two-Wire Automotive Interface (TWAI) or Controller Area Network (CAN) busses.
 *   - Supports all TWAI busses provided by different ESP32 architectures.
 *   - Needs external tranceiver like M5Unit Mini CAN (not isolated!) or M5Unit CANBus (isolated).
 * 
 * The TWAI controller is not compatible with ISO11898-1 FD Format frames, and will interpret
 *   such frames as errors. CAN bus message format is 32-bit identifier and up to 8 bytes of data
 * 
 * Due to the CAN-bus nature of many received datagrams you'll needs a berry driver with
 *   class "twai" and functions "config" and "decode".
 *   - See example below.
 *   - When executed by `preinit.be` it allows to configure the TWAI driver mode and speed
 * 
 * Supported command:
 * TwaiSend[<bus>] <identifier>[[[[[[[[<data1>],<data2>],<data3>],<data4>],<data5>],<data6>],<data7>],<data8>]
 * TwaiSend[<bus>] {"ID":<identifier>,["DATA":[[[[[[[<data1>],<data2>],<data3>],<data4>],<data5>],<data6>],<data7>],<data8>]]}
 *   - An <identifier> with bit 31 set (0x80000000) is a 29-bit identifier
 *   - All <data> is optional
 * 
 * Examples:
 * TwaiSend1 0x481,0x03,0x1E,0xFF,0xFF,0xFF,0x01,0x21
 * TwaiSend1 {"ID":0x481,"DATA":[0x03,0x1E,0xFF,0xFF,0xFF,0x01,0x21]}
 * TwaiSend2 {"ID":0x80000481,"DATA":[0x03,0x1E,0xFF,0xFF,0xFF,0x01,0x21]}
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
  twai_handle_t bus[MAX_TWAI];
  twai_mode_t mode[MAX_TWAI];
  uint8_t speed[MAX_TWAI];
  bool installed[MAX_TWAI];
  bool supported;
} Twai;

#ifdef USE_BERRY
/*********************************************************************************************\
 * Berry example code
\*********************************************************************************************/

/*
file twai.be contents:

class twai_cls
  var active, pressure_next                          # (bool, bool)
  var twai_speed, twai_mode                          # (int, int)
  var am012_status, am014_substatus, am024_power     # (int, int, int)
  var dz_am012_status, dz_am014_substatus            # (int, int)
  var pressure, setpoint, flow_temp                  # (float, float, float)
  var dz_pressure, dz_flow_temp                      # (float, float)

  def init()
    self.twai_speed = 7        # 0 = 25K, 1 = 50K, 2 = 100K, 3 = 125K, 4 = 250K, 5 = 500K, 6 = 800K, 7 = 1Mbits
    self.twai_mode = 2         # 0 = TWAI_MODE_NORMAL, 1 = TWAI_MODE_NO_ACK, 2 = TWAI_MODE_LISTEN_ONLY
    self.active = 0
    self.am012_status = 0
    self.am014_substatus = 0
    self.dz_am012_status = 0
    self.dz_am014_substatus = 0
    self.am024_power = 0
    self.pressure_next = 0
    self.pressure = 0
    self.dz_pressure = 0
    self.setpoint = 0
    self.flow_temp = 0
    self.dz_flow_temp = 0
  end

  def config(bus)
#    if bus != 1 return nil end                       # Exit if not my bus
    return self.twai_mode << 3 | self.twai_speed     # Initial configure TWAI driver
  end

  def decode(bus, ident, data1, data2)
#    if bus != 1 return nil end                       # Exit if not my bus
    var id = ident & 0x7ff                           # Remeha uses 11 bit Standard Frame Format
    if id == 0x076                                   # Incremental counter from 0 to 255
#      tasmota.log(f"RMH: 0x{id:03x} Count {data1}", 3)
    elif id == 0x100                                 # Date and Time
      var epoch = 441763200 + (data2 * 24 * 60 * 60) + (data1 / 1000)
#      tasmota.log(f"RMH: 0x{id:03x} Time {tasmota.time_str(epoch)}", 3)
    elif id == 0x1C1                                 # Many different data1/2
      if data1 & 0x00ffffff == 0x503f41              # Next time it's pressure
        self.pressure_next = 1
      elif self.pressure_next == 1
        self.pressure = (data2 & 0xff00)/2560.0      # This must be pressure
        self.pressure_next = 0
      end
    elif id == 0x382
      self.am024_power = data1 & 0xff                # Relative power
      self.setpoint = (data1 & 0xffff00)/25600.0     # Setpoint
#      tasmota.log(f"RMH: 0x{id:03x} Busy {self.am024_power}%, Setpoint {self.setpoint}", 3)
    elif id == 0x282
      self.flow_temp = (data1 & 0xffff00)/25600.0
#      tasmota.log(f"RMH: 0x{id:03x} DHW temp {self.flow_temp}", 3)
    elif id == 0x481                                 # Status information
      self.am012_status = data1 & 0xff
      self.am014_substatus = (data1 & 0xff00)/256
    else      
      return
    end
    self.active = 1                                  # At least one valid decode
  end

  #- add sensor value to teleperiod -#
  def json_append()
    if !self.active return nil end                   # Exit if never decoded something
    import string
    var msg = string.format(",\"Calenta\":{\"AM012\":%i,\"AM014\":%i,\"Pressure\":%.1f,\"Setpoint\":%.1f,\"Flow\":%.1f}",
                             self.am012_status, self.am014_substatus, self.pressure, self.setpoint, self.flow_temp)
    tasmota.response_append(msg)
  end

  #- perform action just after teleperiod -#
#  def after_teleperiod() 
  #- perform action every second -#
  def every_second()
    # Only send if changed to reduce TWAI wait time
    if self.dz_pressure != self.pressure
      tasmota.cmd('_DzSend1 523,' .. self.pressure)   # Send pressure to Domoticz
    end
    self.dz_pressure = self.pressure

    if self.dz_flow_temp != self.flow_temp
      tasmota.cmd('_DzSend1 526,' .. self.flow_temp)  # Send flow temp to Domoticz
    end
    self.dz_flow_temp = self.flow_temp

    if self.dz_am012_status != self.am012_status
      tasmota.cmd('_DzSend1 536,' .. self.am012_status)  # Send status to Domoticz
    end
    self.dz_am012_status = self.am012_status

    if self.dz_am014_substatus != self.am014_substatus
      tasmota.cmd('_DzSend1 537,' .. self.am014_substatus)  # Send substatus to Domoticz
    end
    self.dz_am014_substatus = self.am014_substatus
  end

  #- display sensor value in the web UI -#
  def web_sensor()
    if !self.active return nil end                   # Exit if never decoded something
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

bool TWAIBerryDecode(uint32_t bus, uint32_t identifier, uint32_t data_length_code, uint8_t *data) {
  bvm *vm = berry.vm;
  if (nullptr == vm) { return false; }
  bool handled = false;
  if (be_getglobal(vm, "twai")) {         // Look for class
    if (be_getmember(vm, 1, "decode")) {  //  and function
      be_pushvalue(vm, -2);

      uint32_t ident = identifier | data_length_code << 28;
      uint32_t data1 = data[3] << 24 | data[2] << 16 | data[1] << 8 | data[0];
      uint32_t data2 = data[7] << 24 | data[6] << 16 | data[5] << 8 | data[4];
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("TWA: Bus%d ident %08X, data1 %08X, data2 %08X"), bus +1, ident, data1, data2);

      be_pushint(vm, bus +1);             // Bus 1, 2 or 3
      be_pushint(vm, ident);
      be_pushint(vm, data1);
      be_pushint(vm, data2);
      int32_t ret = be_pcall(vm, 5);      // Number of arguments, self, bus, ident. data1, data2
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

bool TWAIBerryConfig(uint32_t bus) {
  bvm *vm = berry.vm;
  if (nullptr == vm) { return false; }
  bool handled = false;
  if (be_getglobal(vm, "twai")) {         // Look for class
    if (be_getmember(vm, 1, "config")) {  //  and function
      be_pushvalue(vm, -2);
      be_pushint(vm, bus +1);             // Bus 1, 2 or 3
      int32_t ret = be_pcall(vm, 2);      // Number of arguments, self, bus
      if (ret != 0) {
        be_error_pop_all(vm);             // Clear Berry stack
      }
      be_pop(vm, 2);
      if (be_isint(vm, -1)) {
        uint32_t config = be_toint(vm, -1);
        Twai.speed[bus] = config & 0x7;   // User input check 0..7
        uint32_t mode = config >> 3 & 0x3;
        if (mode > 2) { mode = 2; }       // User input check 0..2
        Twai.mode[bus] = (twai_mode_t)mode;
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

bool TWAIStart(int tx, int rx, uint32_t bus = 0);
bool TWAIStart(int tx, int rx, uint32_t bus) {
  twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)tx, (gpio_num_t)rx, Twai.mode[bus]);
  g_config.controller_id = bus;
  g_config.rx_queue_len = 32;

  twai_timing_config_t t_config;
  switch (Twai.speed[bus]) {
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

  if (twai_driver_install_v2(&g_config, &t_config, &f_config, &Twai.bus[bus]) != ESP_OK) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("TWA: Bus%d Not installed"), bus +1);
    return false;
  }
  if (twai_start_v2(Twai.bus[bus]) != ESP_OK) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("TWA: Bus%d Not started"), bus +1);
    return false;
  }
  // Reconfigure alerts to detect frame receive, Bus-Off error and RX queue full states
  uint32_t alerts_to_enable = TWAI_ALERT_RX_DATA | TWAI_ALERT_RX_QUEUE_FULL | TWAI_ALERT_TX_IDLE | TWAI_ALERT_TX_SUCCESS | TWAI_ALERT_TX_FAILED | TWAI_ALERT_ERR_PASS | TWAI_ALERT_BUS_ERROR;
  if (twai_reconfigure_alerts_v2(Twai.bus[bus], alerts_to_enable, NULL) != ESP_OK) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("TWA: Bus%d Failed to reconfigure CAN alerts"), bus +1);
    return false;
  } 

  char smode[16];
  GetTextIndexed(smode, sizeof(smode), Twai.mode[bus], kTwaiModes);
  char sspeed[16];
  GetTextIndexed(sspeed, sizeof(sspeed), Twai.speed[bus], kTwaiSpeeds);
  AddLog(LOG_LEVEL_DEBUG, PSTR("TWA: Bus%d using GPIO%02d(Tx) and GPIO%02d(Rx) started in %s Mode and %sbit/s"),
    bus +1, tx, rx, smode, sspeed);

  Twai.installed[bus] = true;
  return true;
}

/*********************************************************************************************/

void TWAIStop(uint32_t bus) {
  if (Twai.installed[bus]) {
    twai_stop_v2(Twai.bus[bus]);
    twai_driver_uninstall_v2(Twai.bus[bus]);
    Twai.installed[bus] = false;
    AddLog(LOG_LEVEL_DEBUG, PSTR("TWA: Bus%d stopped"), bus +1);
  }
}

/*********************************************************************************************/

uint32_t TWAICheckAlerts(uint32_t bus) {
  uint32_t alerts_triggered;
  twai_read_alerts_v2(Twai.bus[bus], &alerts_triggered, pdMS_TO_TICKS(TWAI_POLLING_RATE_MS));
  twai_status_info_t twaistatus;
  twai_get_status_info_v2(Twai.bus[bus], &twaistatus);

  // Handle alerts
  if (alerts_triggered & TWAI_ALERT_ERR_PASS) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("TWA: Bus%d Alert - Controller has become error passive"), bus +1);
  }
  if (alerts_triggered & TWAI_ALERT_BUS_ERROR) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("TWA: Bus%d Alert - A Bit, Stuff, CRC, Form or ACK error has occurred on the bus. Errors %d"), bus +1, twaistatus.bus_error_count);
  }
  if (alerts_triggered & TWAI_ALERT_RX_QUEUE_FULL) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("TWA: Bus%d Alert - RX queue is full causing a received frame to be lost. Buffered %d, Missed %d, Overrun %d"),
      bus +1, twaistatus.msgs_to_rx, twaistatus.rx_missed_count, twaistatus.rx_overrun_count);
  }
  if (alerts_triggered & TWAI_ALERT_TX_FAILED) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("TWA: Bus%d Alert - Transmission failed. Buffered %d, Error %d, Failed %d"),
      bus +1, twaistatus.msgs_to_tx, twaistatus.tx_error_counter, twaistatus.tx_failed_count);
  }
//  if (alerts_triggered & TWAI_ALERT_TX_SUCCESS) {
//    AddLog(LOG_LEVEL_DEBUG, PSTR("TWA: Bus%d Transmission successful. Buffered %d"), bus +1, twaistatus.msgs_to_tx);
//  }

  return alerts_triggered;
}

/*********************************************************************************************/

void TWAIRead(uint32_t bus) {
  uint32_t alerts_triggered = TWAICheckAlerts(bus);
  // Check if message is received
  if (alerts_triggered & TWAI_ALERT_RX_DATA) {
    // One or more messages received. Handle all.
    twai_message_t message;
    while (twai_receive_v2(Twai.bus[bus], &message, 0) == ESP_OK) {
#ifdef USE_BERRY
      if (TWAIBerryDecode(bus, message.identifier, message.data_length_code, message.data)) { continue; }
#endif  // USE_BERRY
      // Show log if no berry found (Messages come too fast for supporting MQTT at this time)
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("TWA: Bus%d Id 0x%08X, Rcvd '%*_H'"),
        bus +1, message.identifier, message.data_length_code, message.data);
    }
  }
}

/*********************************************************************************************\
 * Initialization
\*********************************************************************************************/

void TWAIInit(void) {
  Twai.supported = false;
  for (uint32_t bus = 0; bus < MAX_TWAI; bus++) {
    if (PinUsed(GPIO_TWAI_TX, bus) && PinUsed(GPIO_TWAI_RX, bus)) { 
      Twai.speed[bus] = TWAI_SPEED_1MBITS;
      Twai.mode[bus] = TWAI_MODE_LISTEN_ONLY;  // 0 = TWAI_MODE_NORMAL, 1 = TWAI_MODE_NO_ACK, 2 = TWAI_MODE_LISTEN_ONLY
#ifdef USE_BERRY
      TWAIBerryConfig(bus);
#endif  // USE_BERRY
      if (TWAIStart(Pin(GPIO_TWAI_TX, bus), Pin(GPIO_TWAI_RX, bus), bus)) { 
        Twai.supported = true;
      }
    }
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

const char kTwaiCommands[] PROGMEM = "TWAI|"  // Prefix
  "Send";

void (* const TwaiCommand[])(void) PROGMEM = {
  &CmndTWAISend };

void CmndTWAISend(void) {
  // TwaiSend<bus> <identifier>[[[[[[[[<data1>],<data2>],<data3>],<data4>],<data5>],<data6>],<data7>],<data8>]
  // TwaiSend<bus> {"ID":<identifier>,"DATA":[[[[[[[[<data1>],<data2>],<data3>],<data4>],<data5>],<data6>],<data7>],<data8>]]}
  //  An identifier with bit 32 set (0x80000000) is a 29-bit identifier
  // TwaiSend1 0x481,0x03,0x1E,0xFF,0xFF,0xFF,0x01,0x21
  // TwaiSend1 {"ID":0x481,"DATA":[0x03,0x1E,0xFF,0xFF,0xFF,0x01,0x21]}
  // TwaiSend2 {"ID":0x80000481,"DATA":[0x03,0x1E,0xFF,0xFF,0xFF,0x01,0x21]}
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_TWAI) && (XdrvMailbox.data_len > 0)) {
    uint32_t bus = XdrvMailbox.index -1;
    if (!Twai.installed[bus]) { return; }

    uint32_t data[9] = { 0 };                      // Accomodate identifier and 8 data bytes
    uint32_t data_count = 0;
    if (XdrvMailbox.data[0] == ('{')) {
      // parse JSON
      JsonParser parser(XdrvMailbox.data);
      JsonParserObject root = parser.getRootObject();
      if (!root) { return; }                       // No valid JSON

      JsonParserToken val = root[PSTR("ID")];
      if (!val) { return; }                        // We need at least an identifier

      data[0] = val.getUInt();
      data_count = 1;
      JsonParserArray arr = root[PSTR("DATA")];
      for (auto value : arr) {                     // All data is optional
        data[data_count] = value.getUInt();
        data_count++;
        if (9 == data_count) {
          break;
        }
      }
    } else {
      // parse comma separated values
      data_count = ParseParameters(9, data);
    }

    twai_message_t message;
    message.extd = (data[0] & 0x80000000) ? 1 : 0; // Standard (0) vs extended (1) format
    message.rtr = 0,                               // Data (0) vs RTR (1) frame
    message.ss = 0,                                // Whether the message is single shot (1) (i.e., does not repeat on error)
    message.self = 0,                              // Whether the message is a self reception request (1) (loopback)
    message.dlc_non_comp = 0,                      // DLC is less than 8 (0)
    message.identifier = data[0] & 0x1FFFFFFF;     // Message ID (11-bits or 29-bits)
    message.data_length_code = data_count -1;      // Payload length (0..8)
    for (uint8_t i = 0; i < message.data_length_code; i++) {
      message.data[i] = data[i +1];                // Payload
    }

    twai_clear_receive_queue_v2(Twai.bus[bus]);
    //Queue message for transmission
    if (twai_transmit_v2(Twai.bus[bus], &message, pdMS_TO_TICKS(TWAI_POLLING_RATE_MS)) == ESP_OK) {
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("TWA: Bus%d Message queued for transmission"), bus +1);
    } else {
      AddLog(LOG_LEVEL_DEBUG, PSTR("TWA: Bus%d Alert - Failed to queue message for transmission"), bus +1);
    }
    ResponseCmndDone();
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
        for (uint32_t bus = 0; bus < MAX_TWAI; bus++) {
          if (Twai.installed[bus]) {
            TWAIRead(bus);
          }
        }
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kTwaiCommands, TwaiCommand);
        break;
    }
  }
  return result;
}

#endif  // SOC_TWAI_SUPPORTED
#endif  // USE_ESP32_TWAI
#endif  // ESP32