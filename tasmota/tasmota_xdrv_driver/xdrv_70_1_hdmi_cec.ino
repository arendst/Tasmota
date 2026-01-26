/*
  xdrv_70_hdmi_cec.ino - support for HDMI CEC bus (control TV via HDMI)

  Copyright (C) 2021  Theo Arends, Stephan Hadinger

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


#ifdef USE_HDMI_CEC
/*********************************************************************************************\
 * HDMI CEC send and receive using lib https://github.com/lucadentella/ArduinoLib_CEClient
\*********************************************************************************************/

#define XDRV_70             70

const char kHDMICommands[] PROGMEM = D_PRFX_HDMI "|"
  D_CMND_HDMI_SEND_RAW "|" D_CMND_HDMI_SEND "|"
  D_CMND_HDMI_TYPE "|" D_CMND_HDMI_ADDR;

void (* const HDMICommand[])(void) PROGMEM = {
  &CmndHDMISendRaw, CmndHDMISend,
  &CmndHDMIType, &CmndHDMIAddr,
  };


// This is called after the logical address has been allocated
void HDMI_OnReady(class CEC_Device* self, int logical_address) {
  int physical_address = self->getPhysicalAddress();
  AddLog(LOG_LEVEL_INFO, PSTR("CEC: HDMI CEC initialized on GPIO %i, Logical address %d, Physical address 0x%04X"), self->getGPIO(), logical_address, physical_address);
}

void HDMI_OnReceive(class CEC_Device *self, int32_t from, int32_t to, uint8_t* buf, size_t len, bool ack)
{
  if (HighestLogLevel() >= LOG_LEVEL_DEBUG) {
  	AddLog(LOG_LEVEL_DEBUG, "CEC: Packet received: (%1X->%1X) %1X%1X%*_H %s", from, to, from, to, len, buf, ack ? PSTR("ACK") : PSTR("NAK"));
  }

  Response_P(PSTR("{\"HdmiReceived\":{\"From\":%i,\"To\":%i,\"Data\":\"%*_H\"}}"), from, to, len, buf);
  if (to == self->getLogicalAddress() || to == 0x0F) {
    MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings->flag.mqtt_sensor_retain);
  }
  XdrvRulesProcess(0);     // apply rules
}

void HDMI_OnTransmit(class CEC_Device *self, uint8_t* buf, size_t len, bool ack)
{
	// This is called after a frame is transmitted.
  if (HighestLogLevel() >= LOG_LEVEL_DEBUG) {
    AddLog(LOG_LEVEL_DEBUG, "CEC: Packet sent: %*_H %s", len, buf, ack ? PSTR("ACK") : PSTR("NAK"));
  }
}

// singleton for HDMI CEC object, could be expanded if we manage multiple HDMI in parallel
CEC_Device *HDMI_CEC_device = nullptr;

void HdmiCecInit(void)
{
  // CEC device type
  CEC_Device::CEC_DEVICE_TYPE device_type = (CEC_Device::CEC_DEVICE_TYPE) Settings->hdmi_cec_device_type;
  if (device_type < 0 || device_type >= CEC_Device::CDT_LAST) {
    // if type in Settings is invalid, default to PLAYBACK_DEVICE
    device_type = CEC_Device::CDT_PLAYBACK_DEVICE;
    Settings->hdmi_cec_device_type = (uint8_t) device_type;
    SettingsSaveAll();
  }
  // GPIO configuration
  int32_t cec_gpio = Pin(GPIO_HDMI_CEC);
  if (cec_gpio >= 0) {
    HDMI_CEC_device = new CEC_Device(cec_gpio, device_type, true); // Promiscuous mode
    if (HDMI_CEC_device == nullptr) {
      AddLog(LOG_LEVEL_ERROR, PSTR("CEC: HDMI_CEC_device init failed"));
      return;
    }
    HDMI_CEC_device->setOnReceiveCB(&HDMI_OnReceive);
    HDMI_CEC_device->setOnTransmitCB(&HDMI_OnTransmit);
    HDMI_CEC_device->setOnReadyCB(&HDMI_OnReady);
    HDMI_CEC_device->start();   // start the protocol
  }
}

/*********************************************************************************************\
 * Interrupt management
\*********************************************************************************************/

void IRAM_ATTR CEC_Run(void *self) {
  CEC_Device *cec_device = (CEC_Device*)self;
  cec_device->serviceGpioISR();
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

//
// Command HdmiSendRaw
//
// HdmiSendRaw <hex>
//    Send the HEX sequence as-is with no control
//
void CmndHDMISendRaw(void) {
  if (HDMI_CEC_device) {
    RemoveSpace(XdrvMailbox.data);
    SBuffer buf = SBuffer::SBufferFromHex(XdrvMailbox.data, strlen(XdrvMailbox.data));
    if (buf.len() > 0 && buf.len() < 16) {
      bool success = HDMI_CEC_device->transmitRaw(buf.buf(), buf.len());
      if (success) {
        bool transmitting = true;
        while (transmitting) {
          HDMI_CEC_device->run();
          transmitting = HDMI_CEC_device->isTransmitting();
          if (transmitting) {
            delay(1);  // wait until next ms
          }
        }
        ResponseCmndDone();
      } else {
        ResponseCmndChar_P(PSTR("Sending failed"));
      }
    } else {
      ResponseCmndChar_P(PSTR("Buffer too large"));
    }
  }
}

//
// Command HdmiSend
//
// HdmiSend <hex>
// HdmiSend { ["To":<to>,] "Data":"<hex>"}
//    Send the HEX payload to the target (unicast of broadcast)
//    "To": 0-15 (optional) target logical address, defaults to 0 (TV)
//    "Hex": payload without the first byte (source/dst) which is inferred
//
// Examples:
//    HdmiSend 8F     -- ask TV its power state
// or HdmiSend {"Data":"8F"}
// or HdmiSend {"To":0, "Data":"8F"}
//
//
//    HdmiSend 8C     -- ask TV its vendor id
// or HdmiSend {"Data":"8C"}
// or HdmiSend {"To":0, "Data":"8C"}
//
void CmndHDMISend(void) {
  if (HDMI_CEC_device) {
    RemoveSpace(XdrvMailbox.data);
    if (XdrvMailbox.data[0] == '{') {

      // JSON
      JsonParser parser(XdrvMailbox.data);
      JsonParserObject root = parser.getRoot();

      if (!parser || !(root.isObject())) { ResponseCmndChar_P(PSTR(D_JSON_INVALID_JSON)); return; }

      JsonParserToken val;
      uint32_t to = root.getUInt(PSTR("To"), 0);
      const char * payload = root.getStr(PSTR("Data"));
      SBuffer buf = SBuffer::SBufferFromHex(payload, strlen(payload));
      if (buf.len() > 0 && buf.len() < 15) {
        bool success = HDMI_CEC_device->transmitFrame(to, buf.buf(), buf.len());
        if (success) {
          bool transmitting = true;
          while (transmitting) {
            HDMI_CEC_device->run();
            transmitting = HDMI_CEC_device->isTransmitting();
            if (transmitting) {
              delay(1);  // wait until next ms
            }
          }
          ResponseCmndDone();
        } else {
          ResponseCmndChar_P(PSTR("Sending failed"));
        }
      } else {
        if (buf.len() == 0) {
          ResponseCmndChar_P(PSTR("Buffer empty"));
        } else {
          ResponseCmndChar_P(PSTR("Buffer too large"));
        }
      }
    } else {
      // Hex
      SBuffer buf = SBuffer::SBufferFromHex(XdrvMailbox.data, strlen(XdrvMailbox.data));
      if (buf.len() > 0 && buf.len() < 15) {
        bool success = HDMI_CEC_device->transmitFrame(0, buf.buf(), buf.len());
        if (success) {
          HDMI_CEC_device->run();
          ResponseCmndDone();
        } else {
          ResponseCmndChar_P(PSTR("Sending failed"));
        }
      } else {
        if (buf.len() == 0) {
          ResponseCmndChar_P(PSTR("Buffer empty"));
        } else {
          ResponseCmndChar_P(PSTR("Buffer too large"));
        }
      }
    }
  }
}

//
// Command CmndHDMIType
//
//
void CmndHDMIType(void) {
  if (XdrvMailbox.data_len > 0) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < CEC_Device::CDT_LAST)) {
      uint8_t type = XdrvMailbox.payload;
      if (type != Settings->hdmi_cec_device_type) {
        Settings->hdmi_cec_device_type = XdrvMailbox.payload;
        SettingsSaveAll();
      }
    }
  }
  ResponseCmndNumber(Settings->hdmi_cec_device_type);
}

#define HDMI_EDID_ADDRESS      0x50    // HDMI EDID address is 0x50

// Read FULL EDID 256 bytes from address 0x50
// Return true if failed
// The buffer must be allocated to uint8_t[256] by caller
// Only checksum is checked
bool ReadEdid256(uint8_t *buf) {
  if (!TasmotaGlobal.i2c_enabled[0]) { return true; }    // abort if I2C is not started

  if (I2cReadBuffer(HDMI_EDID_ADDRESS,   0, buf      , 128)) { return true; }
  if (I2cReadBuffer(HDMI_EDID_ADDRESS, 128, buf + 128, 128)) { return true; }

  // verify checksum for block 0
  uint8_t chk0 = 0;
  for (uint32_t i = 0; i < 128; i++) {
    chk0 += buf[i];
  }
  if (chk0 != 0) { return true; }

  // verify checksum for block 1
  uint8_t chk1 = 0;
  for (uint32_t i = 128; i < 256; i++) {
    chk1 += buf[i];
  }
  if (chk1 != 0) { return true; }

  // check prefix
  uint32_t * buf32 = (uint32_t*) buf;
  if (buf32[0] != 0xFFFFFF00 || buf32[1] != 0x00FFFFFF) { return true; }

  return false;     // OK
}

// HDMI get physical address
// This is done by reading EDID via I2C, and looking for a vendor specific extension
//
// Return 0x0000 if not found
uint16_t HDMIGetPhysicalAddress(void) {
  uint8_t buf[256] = {0};
  if (HighestLogLevel() >= LOG_LEVEL_DEBUG) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CEC: trying to read physical address"));
  }
  if (ReadEdid256(buf)) { return 0x0000; }      // unable to get an address

  uint8_t edid_extensions = buf[126];
  if (HighestLogLevel() >= LOG_LEVEL_DEBUG) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CEC: successfully read EDID 256 bytes, extensions count %i"), edid_extensions);
    AddLog(LOG_LEVEL_DEBUG, PSTR("CEC: EDID: %*_H"), sizeof(buf),buf);
  }
  if (edid_extensions == 0) {
    AddLog(LOG_LEVEL_INFO, PSTR("CEC: Error: EDID has no extension"));
  }

  // Read first extension which is mandatory for HDMI
  if (buf[128] != 0x02 || buf[129] < 0x03) { return 0x0000; }   // invalid extension

  uint32_t extensions_first_byte = 128 + 4;
  uint32_t extensions_last_byte = 128 + buf[130];
  uint32_t idx = extensions_first_byte;
  while (idx < extensions_last_byte) {
    uint8_t data_block_header = buf[idx];
    uint32_t type = (data_block_header >> 5);
    uint32_t number_of_bytes = (data_block_header & 0x1F);

    // AddLog(LOG_LEVEL_DEBUG, "CEC: idx %i extension type %i, number of bytes %i", idx, type, number_of_bytes);

    if (type == 3) {
      // Vendor specific extension
      // 030C00 for "HDMI Licensing, LLC"
      if (buf[idx+1] == 0x03 && buf[idx+2] == 0x0C && buf[idx+3] == 0x00) {
        uint16_t addr = (buf[idx+4] << 8) | buf[idx+5];
        if (HighestLogLevel() >= LOG_LEVEL_DEBUG) {
          AddLog(LOG_LEVEL_DEBUG, "CEC: physical address found: 0x%04X", addr);
        }
        return addr;
      }
    }

    idx += 1 + number_of_bytes;
  }

  if (HighestLogLevel() >= LOG_LEVEL_DEBUG) {
    AddLog(LOG_LEVEL_DEBUG, "CEC: physical address not found");
  }
  return 0x0000;    // TODO
}


void CmndHDMIAddr(void) {
  if (XdrvMailbox.data_len > 0) {
    if ((XdrvMailbox.payload > 0)) {
      uint16_t hdmi_addr = XdrvMailbox.payload;
      Settings->hdmi_addr[0] = (hdmi_addr) & 0xFF;
      Settings->hdmi_addr[1] = (hdmi_addr >> 8) & 0xFF;
    }
  }
  uint16_t hdmi_addr = HDMI_CEC_device->discoverPhysicalAddress();
  Response_P(PSTR("{\"%s\":\"0x%04X\"}"), XdrvMailbox.command, hdmi_addr);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv70(uint32_t function)
{
  bool result = false;

  if (FUNC_INIT == function) {
    HdmiCecInit();
  } else if (HDMI_CEC_device) {
    switch (function) {
      case FUNC_LOOP:
      case FUNC_SLEEP_LOOP:
        HDMI_CEC_device->run();
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kHDMICommands, HDMICommand);
        break;
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  }
  return result;
}

#endif // USE_HDMI_CEC
