/*
  xdrv_91_esp32_twai.ino - ESP32 TWAI support for Tasmota

  SPDX-FileCopyrightText: 2025 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef ESP32
#if !CONFIG_IDF_TARGET_ESP32C5 // todo: remove when ESP32C5 TWAI support is added
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
 * For more information see https://tasmota.github.io/docs/TWAI/
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
  twai_handle_t bus;
  twai_mode_t mode;
  uint8_t rx_queue_len;
  uint8_t speed;
  bool installed;
} Twai[MAX_TWAI];

bool Twai_supported;

#ifdef USE_BERRY
/*********************************************************************************************\
 * Berry example code
\*********************************************************************************************/

/*
file twai_minimal.be contents:

class twai_cls
  var twai_speed, twai_mode, twai_rx_queue_len    # (int, int, int)

  def init()
    self.twai_speed = 4         # 0 = 25K, 1 = 50K, 2 = 100K, 3 = 125K, 4 = 250K, 5 = 500K, 6 = 800K, 7 = 1Mbits
    self.twai_mode = 2          # 0 = TWAI_MODE_NORMAL, 1 = TWAI_MODE_NO_ACK, 2 = TWAI_MODE_LISTEN_ONLY
    self.twai_rx_queue_len = 8  # 1 .. 64

    if global.twai_driver
      global.twai_driver.stop() # Let previous instance bail out cleanly
    end
    tasmota.add_driver(global.twai_driver := self)
  end

  def stop()
    tasmota.remove_driver(self)
  end

  #----------------------------------------------------------------------------------------------
  Allow TWAI driver configuration on restart (if this file is installed by preinit.be)
  ----------------------------------------------------------------------------------------------#
  def config(bus)                                    # This function name (config) is called by the TWAI driver!
    return (self.twai_rx_queue_len -1) << 5 | self.twai_mode << 3 | self.twai_speed  # Initial configure TWAI driver
  end

  #----------------------------------------------------------------------------------------------
  This example decodes nothing but allows for the driver to show message logging in log level 4
  ----------------------------------------------------------------------------------------------#
  def decode(param, ident, data1, data2)             # This function name (decode) is called by the TWAI driver!
    var bus = param & 0xF                            # Bus number (1..3)
    var len = param >> 4 & 0xF                       # Number of data bytes (0..8)
    var extended = ident >> 31 & 0x1                 # Extended identifier flag (0..1)
    var id = ident & 0x1fffffff

  end
end

twai = twai_cls()                                    # This class name (twai) is used by the TWAI driver!

// *******************************************************************************************

file preinit.be contents:

load('twai_minimal.be')
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

      uint32_t param = bus +1 | data_length_code << 4;
      uint32_t ident = identifier;
      uint32_t data1 = data[3] << 24 | data[2] << 16 | data[1] << 8 | data[0];
      uint32_t data2 = data[7] << 24 | data[6] << 16 | data[5] << 8 | data[4];
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("TWA: Bus%d pm %02X, id %08X, d1 %08X, d2 %08X"), bus +1, param, ident, data1, data2);

      be_pushint(vm, param);              // Bus 1, 2 or 3 | data_length
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
        // 3322222222221111111111
        // 10987654321098765432109876543210
        // xxxxxxxxxxxxxxxxxxxxxxxxxxxxx111 = speed 0..7
        // xxxxxxxxxxxxxxxxxxxxxxxxxxx11xxx = mode 0..2
        // xxxxxxxxxxxxxxxxxxxxx111111xxxxx = rx_queue_len 0..63
        Twai[bus].speed = config & 0x7;   // User input check 0..7
        uint32_t mode = config >> 3 & 0x3;
        if (mode > 2) { mode = 2; }       // User input check 0..2
        Twai[bus].mode = (twai_mode_t)mode;
        uint32_t rx_queue_len = config >> 5 & 0x3f;
        rx_queue_len++;                   // Rx queue len 1..64
        if (rx_queue_len < 8) { rx_queue_len = 8; }  // User input check 8..64
        Twai[bus].rx_queue_len = rx_queue_len;
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
  twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)tx, (gpio_num_t)rx, Twai[bus].mode);
  g_config.controller_id = bus;
  g_config.rx_queue_len = Twai[bus].rx_queue_len;

  twai_timing_config_t t_config;
  switch (Twai[bus].speed) {
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

  if (twai_driver_install_v2(&g_config, &t_config, &f_config, &Twai[bus].bus) != ESP_OK) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("TWA: Bus%d Not installed"), bus +1);
    return false;
  }
  if (twai_start_v2(Twai[bus].bus) != ESP_OK) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("TWA: Bus%d Not started"), bus +1);
    return false;
  }
  // Reconfigure alerts to detect frame receive, Bus-Off error and RX queue full states
  uint32_t alerts_to_enable = TWAI_ALERT_RX_DATA | TWAI_ALERT_RX_QUEUE_FULL | TWAI_ALERT_TX_IDLE | TWAI_ALERT_TX_SUCCESS | TWAI_ALERT_TX_FAILED | TWAI_ALERT_ERR_PASS | TWAI_ALERT_BUS_ERROR;
  if (twai_reconfigure_alerts_v2(Twai[bus].bus, alerts_to_enable, NULL) != ESP_OK) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("TWA: Bus%d Failed to reconfigure CAN alerts"), bus +1);
    return false;
  } 

  char smode[16];
  GetTextIndexed(smode, sizeof(smode), Twai[bus].mode, kTwaiModes);
  char sspeed[16];
  GetTextIndexed(sspeed, sizeof(sspeed), Twai[bus].speed, kTwaiSpeeds);
  AddLog(LOG_LEVEL_DEBUG, PSTR("TWA: Bus%d using GPIO%02d(Tx) and GPIO%02d(Rx) started in %s Mode and %sbit/s"),
    bus +1, tx, rx, smode, sspeed);

  Twai[bus].installed = true;
  return true;
}

/*********************************************************************************************/

void TWAIStop(uint32_t bus) {
  if (Twai[bus].installed) {
    twai_stop_v2(Twai[bus].bus);
    twai_driver_uninstall_v2(Twai[bus].bus);
    Twai[bus].installed = false;
    AddLog(LOG_LEVEL_DEBUG, PSTR("TWA: Bus%d stopped"), bus +1);
  }
}

/*********************************************************************************************/

uint32_t TWAICheckAlerts(uint32_t bus) {
  uint32_t alerts_triggered;
  twai_read_alerts_v2(Twai[bus].bus, &alerts_triggered, pdMS_TO_TICKS(TWAI_POLLING_RATE_MS));
  twai_status_info_t twaistatus;
  twai_get_status_info_v2(Twai[bus].bus, &twaistatus);

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
    while (twai_receive_v2(Twai[bus].bus, &message, 0) == ESP_OK) {
      uint32_t identifier = message.identifier | message.extd << 31;  // Add extended 29-bit flag
#ifdef USE_BERRY
      if (TWAIBerryDecode(bus, identifier, message.data_length_code, message.data)) { continue; }
#endif  // USE_BERRY
      // Show log if no berry found (Messages come too fast for supporting MQTT at this time)
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("TWA: Bus%d Id %08X, Rcvd '%*_H'"),
        bus +1, identifier, message.data_length_code, message.data);
    }
  }
}

/*********************************************************************************************\
 * Initialization
\*********************************************************************************************/

void TWAIInit(void) {
  Twai_supported = false;
  for (uint32_t bus = 0; bus < MAX_TWAI; bus++) {
    if (PinUsed(GPIO_TWAI_TX, bus) && PinUsed(GPIO_TWAI_RX, bus)) { 
      Twai[bus].speed = TWAI_SPEED_100KBITS;
      Twai[bus].mode = TWAI_MODE_NORMAL;  // 0 = TWAI_MODE_NORMAL, 1 = TWAI_MODE_NO_ACK, 2 = TWAI_MODE_LISTEN_ONLY
      Twai[bus].rx_queue_len = 8;
#ifdef USE_BERRY
      TWAIBerryConfig(bus);
#endif  // USE_BERRY
      if (TWAIStart(Pin(GPIO_TWAI_TX, bus), Pin(GPIO_TWAI_RX, bus), bus)) { 
        Twai_supported = true;
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
    if (!Twai[bus].installed) { return; }

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

    twai_clear_receive_queue_v2(Twai[bus].bus);
    //Queue message for transmission
    if (twai_transmit_v2(Twai[bus].bus, &message, pdMS_TO_TICKS(TWAI_POLLING_RATE_MS)) == ESP_OK) {
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
  else if (Twai_supported) {
    switch (function) {
      case FUNC_LOOP:
        for (uint32_t bus = 0; bus < MAX_TWAI; bus++) {
          if (Twai[bus].installed) {
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
#endif  // !CONFIG_IDF_TARGET_ESP32C5
#endif  // ESP32