/*
  xdrv_73_6_lorawan_decode.ino - LoRaWan node decode support for Tasmota

  SPDX-FileCopyrightText: 2024 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_SPI_LORA
#ifdef USE_LORAWAN_BRIDGE
/*********************************************************************************************\
 * LoRaWan node decode and presentation
\*********************************************************************************************/

void LoraWanPublishHeader(uint32_t node, bool decoded) {
  ResponseClear();                                    // clear string

  // Do we prefix with `LwReceived`?
  if (!Settings->flag4.remove_zbreceived &&           // SetOption100 - (Zigbee) Remove LwReceived form JSON message (1)
      !Settings->flag5.zb_received_as_subtopic) {     // SetOption118 - (Zigbee) Move LwReceived from JSON message and into the subtopic replacing "SENSOR" default
    char prefix[16];
    snprintf_P(prefix, sizeof(prefix), PSTR("%s"), (decoded) ? PSTR("LwDecoded") : PSTR("LwReceived"));
    if (Settings->flag5.zigbee_include_time &&        // SetOption144 - (Zigbee) Include time in `LwReceived` messages like other sensors
       (Rtc.utc_time >= START_VALID_TIME)) {
      // Add time if needed (and if time is valid)
      ResponseAppendTimeFormat(Settings->flag2.time_format);  // CMND_TIME
      ResponseAppend_P(PSTR(",\"%s\":"), prefix);
    } else {
      ResponseAppend_P(PSTR("{\"%s\":"), prefix);
    }
  }

  if (!Settings->flag5.zb_omit_json_addr) {           // SetOption119 - (Zigbee) Remove the device addr from json payload, can be used with zb_topic_fname where the addr is already known from the topic
    ResponseAppend_P(PSTR("{\"%s\":"), EscapeJSONString(Lora->settings.end_node[node]->name.c_str()).c_str());
  }
  ResponseAppend_P(PSTR("{\"Node\":%d,\"" D_JSON_ZIGBEE_DEVICE "\":\"0x%04X\""), node +1, Lora->settings.end_node[node]->DevEUIl & 0x0000FFFF);
  if (!Lora->settings.end_node[node]->name.startsWith(F("0x"))) {
    ResponseAppend_P(PSTR(",\"" D_JSON_ZIGBEE_NAME "\":\"%s\""), EscapeJSONString(Lora->settings.end_node[node]->name.c_str()).c_str());
  }
  ResponseAppend_P(PSTR(",\"RSSI\":%1_f,\"SNR\":%1_f"), &Lora->rssi, &Lora->snr);
}

/*********************************************************************************************/

void LoraWanPublishFooter(uint32_t node, bool decoded) {
  if (!Settings->flag5.zb_omit_json_addr) {           // SetOption119 - (Zigbee) Remove the device addr from json payload, can be used with zb_topic_fname where the addr is already known from the topic
    ResponseAppend_P(PSTR("}"));
  }
  if (!Settings->flag4.remove_zbreceived &&           // SetOption100 - (Zigbee) Remove LwReceived form JSON message (1)
      !Settings->flag5.zb_received_as_subtopic) {     // SetOption118 - (Zigbee) Move LwReceived from JSON message and into the subtopic replacing "SENSOR" default
    ResponseAppend_P(PSTR("}"));
  }

#ifdef USE_INFLUXDB
  InfluxDbProcess(1);                                 // Use a copy of ResponseData
#endif

#ifdef ESP8266
  if (!Settings->flag6.mqtt_disable_publish) {        // SetOption147 - If it is activated, Tasmota will not publish MQTT messages, but it will proccess event trigger rules
#else  // ESP32
  bool decode_successful = false;
  if (!decoded) {
    String mqtt_data = TasmotaGlobal.mqtt_data;       // Backup as being destroyed by berry
    uint32_t restart_flag = TasmotaGlobal.restart_flag; // Backup restart_flag
    TasmotaGlobal.restart_flag += 17;                 // Set to non-zero (default) state
    XdrvRulesProcess(0);                              // Apply berry decoding which may reset TasmotaGlobal.restart_flag
    decode_successful = (0 == TasmotaGlobal.restart_flag);
    TasmotaGlobal.restart_flag = restart_flag;        // Restore restart_flag
    TasmotaGlobal.mqtt_data = mqtt_data;              // Restore response data
  }
  if (!decode_successful &&
      !Settings->flag6.mqtt_disable_publish) {        // SetOption147 - If it is activated, Tasmota will not publish MQTT messages, but it will proccess event trigger rules
#endif  // ESP32
    if (Settings->flag4.zigbee_distinct_topics) {     // SetOption89  - (MQTT, Zigbee) Distinct MQTT topics per device for Zigbee (1) (#7835)
      char subtopic[TOPSZ];
      // Clean special characters
      char stemp[TOPSZ];
      strlcpy(stemp, Lora->settings.end_node[node]->name.c_str(), sizeof(stemp));
      MakeValidMqtt(0, stemp);
      if (Settings->flag5.zigbee_hide_bridge_topic) { // SetOption125 - (Zigbee) Hide bridge topic from zigbee topic (use with SetOption89) (1)
        snprintf_P(subtopic, sizeof(subtopic), PSTR("%s"), stemp);
      } else {
        snprintf_P(subtopic, sizeof(subtopic), PSTR("%s/%s"), TasmotaGlobal.mqtt_topic, stemp);
      }
      char stopic[TOPSZ];
      if (Settings->flag5.zb_received_as_subtopic)    // SetOption118 - (Zigbee) Move LwReceived from JSON message and into the subtopic replacing "SENSOR" default
        GetTopic_P(stopic, TELE, subtopic, (decoded) ? PSTR("LwDecoded") : PSTR("LwReceived"));
      else
        GetTopic_P(stopic, TELE, subtopic, PSTR(D_RSLT_SENSOR));
      MqttPublish(stopic, Settings->flag.mqtt_sensor_retain);
    } else {
      MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings->flag.mqtt_sensor_retain);
    }
  }
#ifdef ESP32
  if (decoded)
#endif  // ESP32
    XdrvRulesProcess(0);                              // Apply rules
}

/*********************************************************************************************/

void LoraWanDecode(struct LoraNodeData_t* node_data) {
  /* Enter with LoraNodeData_t
    float rssi;
    float snr;
    uint8_t* payload;
    uint8_t payload_len;
    uint8_t node;
    uint8_t FPort;
  */
  if (bitRead(Lora->settings.flags, TAS_LORA_FLAG_DECODE_ENABLED)) {          // LoraOption3 1
    uint32_t manufacturer = Lora->settings.end_node[node_data->node]->DevEUIh >> 8;
    if (0x001616 == manufacturer) {                                           // MerryIoT
      if (120 == node_data->FPort) {                                          // MerryIoT door/window Sensor (DW10)
        if (9 == node_data->payload_len) {                                    // MerryIoT Sensor state 
          //  1  2  3  4  5 6  7 8 9
          // 03 0F 19 2C 8A00 040000 - button
          // 00 0F 19 2C 0000 050000 - door
          uint8_t status = node_data->payload[0];
          float battery_volt = (float)(21 + node_data->payload[1]) / 10.0;
          int temperature = node_data->payload[2]; 
          int humidity = node_data->payload[3];
          uint32_t elapsed_time = node_data->payload[4] | (node_data->payload[5] << 8);
          uint32_t events = node_data->payload[6] | (node_data->payload[7] << 8) | (node_data->payload[8] << 16);
#ifdef USE_LORA_DEBUG
          AddLog(LOG_LEVEL_DEBUG, PSTR("LOR: Node %d, DevEUI %08X%08X, Events %d, LastEvent %d min, DoorOpen %d, Button %d, Tamper %d, Tilt %d, Battery %1_fV, Temp %d, Hum %d"), 
            node_data->node +1, Lora->settings.end_node[node_data->node]->DevEUIh, Lora->settings.end_node[node_data->node]->DevEUIl,
            events, elapsed_time,
            bitRead(status, 0), bitRead(status, 1), bitRead(status, 2), bitRead(status, 3),
            &battery_volt,
            temperature, humidity);
#endif  // USE_LORA_DEBUG
          LoraWanPublishHeader(node_data->node, true);
          ResponseAppend_P(PSTR(",\"Events\":%d,\"LastEvent\":%d,\"DoorOpen\":%d,\"Button\":%d,\"Tamper\":%d,\"Tilt\":%d"
                                ",\"Battery\":%1_f,"),
            events, elapsed_time,
            bitRead(status, 0), bitRead(status, 1), bitRead(status, 2), bitRead(status, 3),
            &battery_volt);
          ResponseAppendTHD(temperature, humidity);
          ResponseAppend_P(PSTR("}"));
          LoraWanPublishFooter(node_data->node, true);
          return;
        }
      }
    }

    else if (0xA84041 == manufacturer) {                                      // Dragino
      if (10 == node_data->FPort) {                                           // Dragino LDS02
        // 8CD2 01 000010 000000 00 - Door Open, 3.282V
        // 0CD2 01 000011 000000 00 - Door Closed
        uint8_t status = node_data->payload[0];
        float battery_volt = (float)((node_data->payload[1] | (node_data->payload[0] << 8)) &0x3FFF) / 1000;
        uint8_t MOD = node_data->payload[2];  // Always 0x01
        uint32_t events = node_data->payload[5] | (node_data->payload[4] << 8) | (node_data->payload[3] << 16);
        uint32_t open_duration = node_data->payload[8] | (node_data->payload[7] << 8) | (node_data->payload[6] << 16);
        uint8_t alarm = node_data->payload[9];
#ifdef USE_LORA_DEBUG
        AddLog(LOG_LEVEL_DEBUG, PSTR("LOR: Node %d, DevEUI %08X%08X, Events %d, LastEvent %d min, DoorOpen %d, Battery %3_fV, Alarm %d"), 
          node_data->node +1, Lora->settings.end_node[node_data->node]->DevEUIh, Lora->settings.end_node[node_data->node]->DevEUIl,
          events, open_duration,
          bitRead(status, 7),
          &battery_volt,
          bitRead(alarm, 0));
#endif  // USE_LORA_DEBUG
        LoraWanPublishHeader(node_data->node, true);
        ResponseAppend_P(PSTR(",\"Events\":%d,\"LastEvent\":%d,\"DoorOpen\":%d,\"Alarm\":%d,\"Battery\":%3_f}"),
          events, open_duration, bitRead(status, 7), bitRead(alarm, 0), &battery_volt);
        LoraWanPublishFooter(node_data->node, true);
        return;
      }
    }
  }

  // Joined device without decoding
  LoraWanPublishHeader(node_data->node, false);
  ResponseAppend_P(PSTR(",\"Decoder\":\"%s\",\"DevEUIh\":\"%08X\",\"DevEUIl\":\"%08X\",\"FPort\":%d,\"Payload\":["),
    EscapeJSONString(Lora->settings.end_node[node_data->node]->decoder.c_str()).c_str(),
    Lora->settings.end_node[node_data->node]->DevEUIh,
    Lora->settings.end_node[node_data->node]->DevEUIl,
    node_data->FPort);
  for (uint32_t i = 0; i < node_data->payload_len; i++) {
    ResponseAppend_P(PSTR("%s%d"), (0==i)?"":",", node_data->payload[i]);
  }
  ResponseAppend_P(PSTR("]}"));
  LoraWanPublishFooter(node_data->node, false);
}

#endif  // USE_LORAWAN_BRIDGE
#endif  // USE_SPI_LORA
