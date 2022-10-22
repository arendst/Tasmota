/*
  xdrv_23_zigbee_8_1_greenpower.ino - zigbee support for Tasmota

  Copyright (C) 2021  Theo Arends and Stephan Hadinger

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

#ifdef USE_ZIGBEE

bool convertGPDF_data(class Z_attribute_list &attr_list, uint16_t shortaddr, uint8_t gpcmd, bool wasbroadcast, const SBuffer &payload, size_t payload_start, size_t payload_len) {
  bool parsed = false;
  // register the raw command in synthetic cluster 0xF021 corresponding to the GPDF command
  SBuffer gppayload = payload.subBuffer(payload_start, payload_len);
  convertClusterSpecific(attr_list, 0xF021, gpcmd, false, shortaddr, 0xF2, gppayload);

  uint16_t cluster = 0xFFFF;
  uint8_t  cmd = 0xFF;
  SBuffer  arg(8);
  // arguments pre-munched
  uint8_t arg8 = (payload_len > 0) ? payload.get8(payload_start) : 0xFF;
  uint16_t arg16 = (payload_len > 1) ? payload.get16(payload_start) : 0xFFFF;
  uint16_t arg16_1 = (payload_len > 2) ? payload.get16(payload_start + 1) : 0xFFFF;   // moved by 1 bytes
  uint32_t arg32 = (payload_len > 3) ? payload.get32(payload_start) : 0xFFFFFFFF;
  switch (gpcmd) {
    case 0x00:  cluster = 0x0003; cmd = 0x00; arg.add16(0x003C);              break;  // Identify - Identify - 0x003C
    case 0x10 ... 0x1F: cluster = 0x0005; cmd = 0x01; arg.add8(gpcmd - 0x10); break;  // Scenes - View Scene
    case 0x20:  cluster = 0x0006; cmd = 0x00;                                 break;  // OnOff - Off
    case 0x21:  cluster = 0x0006; cmd = 0x01;                                 break;  // OnOff - On
    case 0x22:  cluster = 0x0006; cmd = 0x02;                                 break;  // OnOff - Toggle
    case 0x23:                                                                break;  // Release
    case 0x30:  cluster = 0x0008; cmd = 0x01; arg.add8(0x00); arg.add8(arg8); break;  // Level Control - Move - Up, rate=arg8
    case 0x31:  cluster = 0x0008; cmd = 0x01; arg.add8(0x01); arg.add8(arg8); break;  // Level Control - Move - Down, rate=arg8
    case 0x32:  cluster = 0x0008; cmd = 0x02; arg.add8(0x00); arg.add8(arg8); arg.add16(arg16); break; // Level Control - Step - Up, step size=arg8, rate=arg16 (opt)
    case 0x33:  cluster = 0x0008; cmd = 0x02; arg.add8(0x01); arg.add8(arg8); arg.add16(arg16); break;   // Level Control - Step - Down, step size=arg8, rate=arg16 (opt)
    case 0x34:  cluster = 0x0008; cmd = 0x03;                                 break;    // Level Control - Stop
    case 0x35:  cluster = 0x0008; cmd = 0x05; arg.add8(0x00); arg.add8(arg8); break;  // Level Control - Move - Up, rate=arg8
    case 0x36:  cluster = 0x0008; cmd = 0x05; arg.add8(0x01); arg.add8(arg8); break;  // Level Control - Move - Down, rate=arg8
    case 0x37:  cluster = 0x0008; cmd = 0x06; arg.add8(0x00); arg.add8(arg8); arg.add16(arg16_1); break;   // Level Control - Step - Up, step size=arg8, rate=arg16 (opt)
    case 0x38:  cluster = 0x0008; cmd = 0x06; arg.add8(0x01); arg.add8(arg8); arg.add16(arg16_1); break;   // Level Control - Step - Down, step size=arg8, rate=arg16 (opt)
    case 0x40:  cluster = 0x0300; cmd = 0x01; arg.add16(0x1900);              break;    // Color Control - Move Hue - Stop 25 steps
    case 0x41:  cluster = 0x0300; cmd = 0x01; arg.add8(0x01); arg.add8(arg8); break;  // Color Control - Move Hue - Up, rate=arg8
    case 0x42:  cluster = 0x0300; cmd = 0x01; arg.add8(0x03); arg.add8(arg8); break;  // Color Control - Move Hue - Down, rate=arg8
    case 0x43:  cluster = 0x0300; cmd = 0x02; arg.add8(0x01); arg.add8(arg8); arg.add8(arg16_1); break; // Color Control - Step Hue - Up - step size/rate
    case 0x44:  cluster = 0x0300; cmd = 0x02; arg.add8(0x03); arg.add8(arg8); arg.add8(arg16_1); break; // Color Control - Step Hue - Down - step size/rate
    case 0x45:  cluster = 0x0300; cmd = 0x04; arg.add16(0x1900);              break;    // Color Control - Move Sat - Stop 25 steps
    case 0x46:  cluster = 0x0300; cmd = 0x04; arg.add8(0x01); arg.add8(arg8); break;  // Color Control - Move Sat - Up, rate=arg8
    case 0x47:  cluster = 0x0300; cmd = 0x04; arg.add8(0x03); arg.add8(arg8); break;  // Color Control - Move Sat - Down, rate=arg8
    case 0x48:  cluster = 0x0300; cmd = 0x05; arg.add8(0x01); arg.add8(arg8); arg.add8(arg16_1); break; // Color Control - Step Sat - Up - step size/rate
    case 0x49:  cluster = 0x0300; cmd = 0x05; arg.add8(0x03); arg.add8(arg8); arg.add8(arg16_1); break; // Color Control - Step Sat - Down - step size/rate
    case 0x4A:  cluster = 0x0300; cmd = 0x08; arg.add32(arg32);               break;  // Color Control - Move Color - RateX/RateY
    case 0x4B:  cluster = 0x0300; cmd = 0x09; arg.addBuffer(payload);         break;  // Color Control - Step Color - copy all
    case 0x50:  cluster = 0x0101; cmd = 0x00;                                 break;    // Door Lock - Lock Door
    case 0x51:  cluster = 0x0101; cmd = 0x01;                                 break;    // Door Lock - Unlock Door
    default: break;
  }
  
  if (cluster != 0xFFFF) {
    convertClusterSpecific(attr_list, cluster, cmd, false, shortaddr, 0xF2, arg);
    parsed = true;
  }
  return parsed;
}


// Handle GPDF Commissioning 0xE0 command
bool convertGPDF_Commissioning(class Z_attribute_list &attr_list, uint16_t shortaddr, bool wasbroadcast, const SBuffer &payload, size_t payload_start, size_t payload_len) {
  uint32_t idx_offset = payload_start;    // offset compared to minimal packet
  uint8_t gpd_device_id = payload.get8(idx_offset++);    // type of device

  uint8_t gpd_options = payload.get8(idx_offset++);
  bool gpd_mac_seq_number = (gpd_options & 0x01);
  bool gpd_rx_on = (gpd_options & 0x02);              // is the device capable of receiving data (generally not)
  bool gpd_app_info_present = (gpd_options & 0x04);   // app info extensions at the end of the packet
  bool gpd_panid_request = (gpd_options & 0x10);
  bool gpd_sec_key_request = (gpd_options & 0x20);
  bool gpd_fixed_location = (gpd_options & 0x40);
  bool gpd_options_has_ext = (gpd_options & 0x80);

  uint8_t gpd_options_ext = 0;
  if (gpd_options_has_ext) {
    gpd_options_ext = payload.get8(idx_offset++);
  }
  uint8_t gpd_sec_level_capa = (gpd_options_ext & 0x03);
  uint8_t gpd_key_type = (gpd_options_ext >> 2) & 0x07;
  uint8_t gpd_key_present = (gpd_options_ext & 0x20);
  uint8_t gpd_key_encryption = (gpd_options_ext & 0x40);
  uint8_t gpd_out_counter_present = (gpd_options_ext & 0x80);

  uint64_t gpd_key_low = 0;
  uint64_t gpd_key_high = 0;
  if (gpd_key_present) {
    gpd_key_low = payload.get64(idx_offset);
    gpd_key_high = payload.get64(idx_offset + 8);
    idx_offset += 16;
  }
  uint32_t gpd_key_mic = 0;
  if (gpd_key_present && gpd_key_encryption) {
    gpd_key_mic = payload.get32(idx_offset);
    idx_offset += 4;
  }
  uint32_t gpd_out_counter = 0;
  if (gpd_out_counter_present) {
    gpd_out_counter = payload.get32(idx_offset);
    idx_offset += 4;
  }

  Response_P(PSTR("{\"ZbGPDFCommissioningReceived\":{"
                  "\"srcaddr\":\"0x%04X\"," "\"wasbroadcast\":%d,"
                  "\"deviceid\":\"0x%02X\"," "\"options\":\"0x%02X\""),
                  shortaddr, wasbroadcast,
                  gpd_device_id, gpd_options
                  );

  if (gpd_options_has_ext) {
    ResponseAppend_P(PSTR(",\"optionsext\":\"0x%02X\""), gpd_options_ext);
  }
  if (gpd_key_present) {
    ResponseAppend_P(PSTR(",\"securitykey\":\"0x%_X%_X\""), &gpd_key_high, &gpd_key_low);
  }
  if (gpd_key_present && gpd_key_encryption) {
    ResponseAppend_P(PSTR(",\"keymic\":\"0x%08X\""), gpd_key_mic);
  }
  if (gpd_out_counter_present) {
    ResponseAppend_P(PSTR(",\"outcounter\":\"0x%08X\""), gpd_out_counter);
  }

  // App info extension for Commissioning
  if (gpd_app_info_present && idx_offset < payload_len + payload_start) {
    uint8_t gp_app_info = payload.get8(idx_offset++);
    bool gp_app_manuf_present = (gp_app_info & 0x01);
    bool gp_app_modelid_present = (gp_app_info & 0x02);
    bool gp_app_cmd_list_present = (gp_app_info & 0x04);
    bool gp_app_cluster_reports_present = (gp_app_info & 0x08);

    if (gp_app_manuf_present) {
      uint16_t gp_app_manuf_id = payload.get8(idx_offset);
      idx_offset += 2;
      ResponseAppend_P(PSTR(",\"manufid\":\"0x%04X\""), gp_app_manuf_id);
    }

    if (gp_app_modelid_present) {
      uint16_t gp_app_model_id = payload.get8(idx_offset);
      idx_offset += 2;
      ResponseAppend_P(PSTR(",\"modelid\":\"0x%04X\""), gp_app_model_id);
    }

    if (gp_app_cmd_list_present) {
      uint8_t gpid_len = payload.get8(idx_offset++);    // number of entries
      JsonGeneratorArray gpdi_list;
      for (uint32_t i = 0; i < gpid_len; i++) {
        if (idx_offset >= payload_len + payload_start) { break; }   // end of payload
        gpdi_list.add(payload.get8(idx_offset++));
      }
      ResponseAppend_P(PSTR(",\"commandid\":%s"), gpdi_list.toString().c_str());
    }

    if (gp_app_cluster_reports_present) {
      uint8_t clust_report_len = payload.get8(idx_offset++);    // number of entries
      JsonGeneratorArray gpdi_list;
      for (uint32_t i = 0; i < clust_report_len; i++) {
        if (idx_offset >= payload_len + payload_start) { break; }   // end of payload
        gpdi_list.add(payload.get16(idx_offset));
        idx_offset += 2;
      }
      ResponseAppend_P(PSTR(",\"clusterreports\":%s"), gpdi_list.toString().c_str());
    }
  }

  ResponseAppend_P(PSTR("}}"));

  if (Settings->flag3.tuya_serial_mqtt_publish) {
    MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR(D_RSLT_SENSOR));
  } else {
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_ZIGBEE "%s"), ResponseData());
  }
  // TODO publish to Berry

  return true;
}

// Handle GPDF Channel Request 0xE3 command
bool convertGPDF_ChannelRequest(class Z_attribute_list &attr_list, uint16_t shortaddr, bool wasbroadcast, const SBuffer &payload, size_t payload_start, size_t payload_len) {
  uint32_t idx_offset = payload_start;    // offset compared to minimal packet
  uint8_t gpd_channel_toggling = payload.get8(idx_offset++);

  uint8_t gpd_next_channel = gpd_channel_toggling & 0x0F;
  uint8_t gpd_second_next_channel = (gpd_channel_toggling >> 4) & 0x0F;
  
  Response_P(PSTR("{\"ZbGPDFChannelRequest\":{"
                  "\"nextchannel\":%i," "\"secondnextchannel\":%i,"
                  "}}"),
                  11 + gpd_next_channel, 11 + gpd_second_next_channel
                  );

  if (Settings->flag3.tuya_serial_mqtt_publish) {
    MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR(D_RSLT_SENSOR));
  } else {
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_ZIGBEE "%s"), ResponseData());
  }

  return true; 
}

// Parse a GPDF (Green Power Data Frame) tunnelled by a Green Power Proxy
//
// Returns `true` if parsed successfully
bool convertGPDF(class Z_attribute_list &attr_list, uint16_t shortaddr, uint8_t gpcmd, bool wasbroadcast, const SBuffer &payload, size_t payload_start, size_t payload_len) {
  // dispatch depending on the GP command
  switch (gpcmd) {
    case 0x00:
    case 0x10 ... 0x6F:
      return convertGPDF_data(attr_list, shortaddr, gpcmd, wasbroadcast, payload, payload_start, payload_len);
    case ZGP_COMMISSIONING:
      return convertGPDF_Commissioning(attr_list, shortaddr, wasbroadcast, payload, payload_start, payload_len);
    case ZGP_CHANNEL_REQUEST:
      return convertGPDF_ChannelRequest(attr_list, shortaddr, wasbroadcast, payload, payload_start, payload_len);
  }
  return false;
}

bool convertGPCommissioningNotification(class Z_attribute_list &attr_list, uint16_t shortaddr, bool wasbroadcast, const SBuffer &payload) {
  uint32_t idx_offset = 0;    // offset due to address format: 0 if 4 bytes, 4 if 8 bytes

  uint16_t gp_options = payload.get16(0);
  uint8_t gp_app_id = gp_options & 0x07;
  bool    gp_rx_after_tx = (gp_options & 0x08);
  uint8_t gp_security_level = (gp_options >> 4) & 0x03;
  uint8_t gp_security_type = (gp_options >> 6) & 0x07;
  bool    gp_security_failed = (gp_options >> 9) & 0x01;
  bool    gp_bidir_cap = (gp_options >> 10) & 0x01;
  bool    gp_proxy_info_present = (gp_options >> 11) & 0x01;


// #define ZBEE_ZCL_GP_COMMISSIONING_NOTIFICATION_OPTION_APP_ID                           (7<<0)
// #define ZBEE_ZCL_GP_COMMISSIONING_NOTIFICATION_OPTION_RX_AFTER_TX                      (1<<3)
// #define ZBEE_ZCL_GP_COMMISSIONING_NOTIFICATION_OPTION_SECUR_LEVEL                      (3<<4)
// #define ZBEE_ZCL_GP_COMMISSIONING_NOTIFICATION_OPTION_SECUR_KEY_TYPE                   (7<<6)
// #define ZBEE_ZCL_GP_COMMISSIONING_NOTIFICATION_OPTION_SECUR_FAILED                     (1<<9)
// #define ZBEE_ZCL_GP_COMMISSIONING_NOTIFICATION_OPTION_BIDIR_CAP                        (1<<10)
// #define ZBEE_ZCL_GP_COMMISSIONING_NOTIFICATION_OPTION_PROXY_INFO_PRESENT               (1<<11)

  uint64_t gp_pgd_id;
  if (gp_app_id == 0x02) {
    idx_offset = 4;
    gp_pgd_id = payload.get64(2);
  } else if (gp_app_id == 0x00) {
    gp_pgd_id = payload.get32(2);
  } else {
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_ZIGBEE "Invalild GP app_id=%i"), gp_app_id);
    return false;
  }
  uint32_t gp_frame_counter = payload.get32(idx_offset + 6);
  uint8_t  gp_cmd = payload.get8(idx_offset + 10);

  uint16_t gpp_shortaddr = 0xFFFF;    // shortaddress of the proxy
  uint8_t  gpp_distance = 0xFF;       // actual LQI from the GP device to the proxy

  uint32_t gp_payload_start = idx_offset + 12;
  uint32_t gp_payload_len = payload.get8(gp_payload_start - 1);

  idx_offset = gp_payload_start + gp_payload_len;   // reset to after payload
  if (gp_proxy_info_present) {
    gpp_shortaddr = payload.get16(idx_offset);
    gpp_distance = payload.get8(idx_offset + 2);
    idx_offset += 3;
  }
  uint32_t gp_mic = 0x00000000;
  if (gp_security_failed) {
    gp_mic = payload.get32(idx_offset);
    idx_offset += 4;
  }

  Response_P(PSTR("{\"" D_JSON_ZIGBEEGP_RECEIVED "\":{"
                  "\"srcaddr\":\"0x%04X\"," "\"wasbroadcast\":%d," "\"zclcmd\":\"GP_COMMISSIONING_NOTIFICATION\","
                  "\"gpoptions\":\"0x%04X\","
                  "\"appid\":%i," "\"rxaftertx\":%i," "\"seclevel\":%i," "\"sectype\":%i," "\"secfailed\":%i," 
                  "\"bidircap\":%i," "\"proxyinfo\":%i,"
                  "\"gpsrcid\":\"0x%_X\"," "\"gpfrmcounter\":\"0x%04X\","
                  "\"gpproxy\":\"0x%04X\"," "\"gpplqi\":%i," "\"gpmic\":\"0x%08X\","
                  "\"gpcmd\":\"0x%02X\","
                  "\"gppayload\":\"%*_H\"}}"),
                  shortaddr, wasbroadcast,
                  gp_options,
                  gp_app_id, gp_rx_after_tx, gp_security_level, gp_security_type, gp_security_failed,
                  gp_bidir_cap, gp_proxy_info_present,
                  &gp_pgd_id, gp_frame_counter,
                  gpp_shortaddr, gpp_distance, gp_mic,
                  gp_cmd,
                  gp_payload_len, payload.buf(gp_payload_start)
                  );

  if (Settings->flag3.tuya_serial_mqtt_publish) {
    MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR(D_RSLT_SENSOR));
  } else {
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_ZIGBEE "%s"), ResponseData());
  }

  return convertGPDF(attr_list, shortaddr, gp_cmd, wasbroadcast, payload, gp_payload_start, gp_payload_len);
}



// Parse Green Power specific commands
// Returns: true if parsed successfully, false if revenrt to generix parsing
bool convertGPSpecific(class Z_attribute_list &attr_list, uint8_t cmd, bool direction, uint16_t shortaddr, bool wasbroadcast, const SBuffer &payload) {
  if (!direction) {
    // server commands
    switch (cmd) {
      case ZGP_COMMISSIONING_NOTIFICATION:
        return convertGPCommissioningNotification(attr_list, shortaddr, wasbroadcast, payload);
        break;
      default:
        return false;
    }
  } else {
    // client commands
    switch (cmd) {
      default:
        return false;
    }
  }
}

#endif // USE_ZIGBEE
