/*
  support_device_groups.ino - device groups support for Tasmota

  Copyright (C) 2021  Paul C Diem

  Device group allow multiple devices to be in a group with power, light
  brightness, fade and speed settings and other module-specific settings
  kept in sync across all devices in the group.

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

#ifdef USE_DEVICE_GROUPS

//#define DEVICE_GROUPS_DEBUG
#define DGR_MEMBER_TIMEOUT        45000
#define DGR_ANNOUNCEMENT_INTERVAL 60000
#define DEVICE_GROUP_MESSAGE      "TASMOTA_DGR"

const char kDeviceGroupMessage[] PROGMEM = DEVICE_GROUP_MESSAGE;

struct device_group_member {
  struct device_group_member * flink;
  IPAddress ip_address;
  uint16_t received_sequence;
  uint16_t acked_sequence;
  uint32_t unicast_count;
};

struct device_group {
  uint32_t next_announcement_time;
  uint32_t next_ack_check_time;
  uint32_t member_timeout_time;
  uint16_t outgoing_sequence;
  uint16_t last_full_status_sequence;
  uint16_t message_length;
  uint16_t ack_check_interval;
  uint8_t message_header_length;
  uint8_t initial_status_requests_remaining;
  char group_name[TOPSZ];
  uint8_t message[128];
  struct device_group_member * device_group_members;
#ifdef USE_DEVICE_GROUPS_SEND
  uint8_t values_8bit[DGR_ITEM_LAST_8BIT];
  uint16_t values_16bit[DGR_ITEM_LAST_16BIT - DGR_ITEM_MAX_8BIT - 1];
  uint32_t values_32bit[DGR_ITEM_LAST_32BIT - DGR_ITEM_MAX_16BIT - 1];
#endif  // USE_DEVICE_GROUPS_SEND
};

WiFiUDP device_groups_udp;
struct device_group * device_groups;
uint32_t next_check_time;
bool device_groups_initialized = false;
bool device_groups_up = false;
bool building_status_message = false;
bool ignore_dgr_sends = false;

char * IPAddressToString(const IPAddress& ip_address)
{
  static char buffer[16];
  sprintf_P(buffer, PSTR("%u.%u.%u.%u"), ip_address[0], ip_address[1], ip_address[2], ip_address[3]);
  return buffer;
}

uint8_t * BeginDeviceGroupMessage(struct device_group * device_group, uint16_t flags, bool hold_sequence = false)
{
  uint8_t * message_ptr = &device_group->message[device_group->message_header_length];
  if (!hold_sequence && !++device_group->outgoing_sequence) device_group->outgoing_sequence = 1;
  *message_ptr++ = device_group->outgoing_sequence & 0xff;
  *message_ptr++ = device_group->outgoing_sequence >> 8;
  *message_ptr++ = flags & 0xff;
  *message_ptr++ = flags >> 8;
  return message_ptr;
}

// Return true if we're configured to share the specified item.
bool DeviceGroupItemShared(bool incoming, uint8_t item)
{
  uint32_t mask;
  if (item == DGR_ITEM_LIGHT_BRI || item == DGR_ITEM_BRI_POWER_ON)
    mask = DGR_SHARE_LIGHT_BRI;
  else if (item == DGR_ITEM_POWER)
    mask = DGR_SHARE_POWER;
  else if (item == DGR_ITEM_LIGHT_SCHEME)
    mask = DGR_SHARE_LIGHT_SCHEME;
  else if (item == DGR_ITEM_LIGHT_FIXED_COLOR || item == DGR_ITEM_LIGHT_CHANNELS)
    mask = DGR_SHARE_LIGHT_COLOR;
  else if (item == DGR_ITEM_LIGHT_FADE || item == DGR_ITEM_LIGHT_SPEED)
    mask = DGR_SHARE_LIGHT_FADE;
  else  if (item == DGR_ITEM_BRI_PRESET_LOW || item == DGR_ITEM_BRI_PRESET_HIGH)
    mask = DGR_SHARE_DIMMER_SETTINGS;
  else if (item == DGR_ITEM_EVENT)
    mask = DGR_SHARE_EVENT;
  else
    return true;
  return mask & (incoming ? Settings.device_group_share_in : Settings.device_group_share_out);
}

void DeviceGroupsInit(void)
{
  // If no module set the device group count, ...
  if (!device_group_count) {

    // If relays in separate device groups is enabled, set the device group count to highest numbered
    // relay.
    if (Settings.flag4.multiple_device_groups) {  // SetOption88 - Enable relays in separate device groups
      for (uint32_t relay_index = 0; relay_index < MAX_RELAYS; relay_index++) {
        if (PinUsed(GPIO_REL1, relay_index)) device_group_count = relay_index + 1;
      }
      if (device_group_count > MAX_DEV_GROUP_NAMES) device_group_count = MAX_DEV_GROUP_NAMES;
    }

    // Otherwise, set the device group count to 1.
    else {
      device_group_count = 1;
    }
  }

  // If there are more device group names set than the number of device groups needed by the
  // module, use the device group name count as the device group count.
  for (; device_group_count < MAX_DEV_GROUP_NAMES; device_group_count++) {
    if (!*SettingsText(SET_DEV_GROUP_NAME1 + device_group_count)) break;
  }

  // Initialize the device information for each device group.
  device_groups = (struct device_group *)calloc(device_group_count, sizeof(struct device_group));
  if (!device_groups) {
    AddLog_P(LOG_LEVEL_ERROR, PSTR("DGR: Error allocating %u-element array"), device_group_count);
    return;
  }

  struct device_group * device_group = device_groups;
  for (uint32_t device_group_index = 0; device_group_index < device_group_count; device_group_index++, device_group++) {
    strcpy(device_group->group_name, SettingsText(SET_DEV_GROUP_NAME1 + device_group_index));

    // If the device group name is not set, use the MQTT group topic (with the device group index +
    // 1 appended for device group indices > 0).
    if (!device_group->group_name[0]) {
      strcpy(device_group->group_name, SettingsText(SET_MQTT_GRP_TOPIC));
      if (device_group_index) {
        snprintf_P(device_group->group_name, sizeof(device_group->group_name), PSTR("%s%u"), device_group->group_name, device_group_index + 1);
      }
    }
    device_group->message_header_length = sprintf_P((char *)device_group->message, PSTR("%s%s"), kDeviceGroupMessage, device_group->group_name) + 1;
    device_group->last_full_status_sequence = -1;
  }

  // If both in and out shared items masks are 0, assume they're unitialized and initialize them.
  if (!Settings.device_group_share_in && !Settings.device_group_share_out) {
    Settings.device_group_share_in = Settings.device_group_share_out = 0xffffffff;
  }

  device_groups_initialized = true;
}

void DeviceGroupsStart()
{
  if (Settings.flag4.device_groups_enabled && !device_groups_up && !TasmotaGlobal.restart_flag) {

    // If we haven't successfuly initialized device groups yet, attempt to do it now.
    if (!device_groups_initialized) {
      DeviceGroupsInit();
      if (!device_groups_initialized) return;
    }

    // Subscribe to device groups multicasts.
    if (!device_groups_udp.beginMulticast(WiFi.localIP(), IPAddress(DEVICE_GROUPS_ADDRESS), DEVICE_GROUPS_PORT)) {
      AddLog_P(LOG_LEVEL_ERROR, PSTR("DGR: Error subscribing"));
      return;
    }
    device_groups_up = true;

    // The WiFi was down but now it's up and device groups is initialized. (Re-)discover devices in
    // our device group(s). Load the status request message for all device groups. This message will
    // be multicast 10 times at 200ms intervals.
    next_check_time = millis() + 2000;
    struct device_group * device_group = device_groups;
    for (uint32_t device_group_index = 0; device_group_index < device_group_count; device_group_index++, device_group++) {
      device_group->next_announcement_time = -1;
      device_group->message_length = BeginDeviceGroupMessage(device_group, DGR_FLAG_RESET | DGR_FLAG_STATUS_REQUEST) - device_group->message;
      device_group->initial_status_requests_remaining = 10;
      device_group->next_ack_check_time = next_check_time;
    }
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("DGR: (Re)discovering members"));
  }
}

void DeviceGroupsStop()
{
  device_groups_udp.flush();
  device_groups_up = false;
}

void SendReceiveDeviceGroupMessage(struct device_group * device_group, struct device_group_member * device_group_member, uint8_t * message, int message_length, bool received)
{
  char log_buffer[512];
  bool item_processed = false;
  uint16_t message_sequence;
  uint16_t flags;
  int device_group_index = device_group - device_groups;
  int log_length;
  int log_remaining;
  char * log_ptr;

  // Find the end and start of the actual message (after the header).
  uint8_t * message_end_ptr = message + message_length;
  uint8_t * message_ptr = message + strlen((char *)message) + 1;

  // Get the message sequence and flags.
  if (message_ptr + 4 > message_end_ptr) goto badmsg;  // Malformed message - must be at least 16-bit sequence, 16-bit flags left
  message_sequence = *message_ptr++;
  message_sequence |= *message_ptr++ << 8;
  flags = *message_ptr++;
  flags |= *message_ptr++ << 8;

  // Initialize the log buffer.
  log_length = sprintf(log_buffer, PSTR("DGR: %s %s message %s %s: seq=%u, flags=%u"), (received ? PSTR("Received") : PSTR("Sending")), device_group->group_name, (received ? PSTR("from") : PSTR("to")), (device_group_member ? IPAddressToString(device_group_member->ip_address) : received ? PSTR("local") : PSTR("network")), message_sequence, flags);
  log_ptr = log_buffer + log_length;
  log_remaining = sizeof(log_buffer) - log_length;

  // If this is an announcement, just log it.
  if (flags == DGR_FLAG_ANNOUNCEMENT) goto write_log;

  // If this is a received ack message, save the message sequence if it's newer than the last ack we
  // received from this member.
  if (flags == DGR_FLAG_ACK) {
    if (received && device_group_member && (message_sequence > device_group_member->acked_sequence || device_group_member->acked_sequence - message_sequence < 64536)) {
      device_group_member->acked_sequence = message_sequence;
    }
    goto write_log;
  }

  // If this is a received message, send an ack message to the sender.
  if (device_group_member) {
    if (received) {
      if (!(flags & DGR_FLAG_MORE_TO_COME)) {
        *(message_ptr - 2) = DGR_FLAG_ACK;
        *(message_ptr - 1) = 0;
        SendReceiveDeviceGroupMessage(device_group, device_group_member, message, message_ptr - message, false);
      }
    }

    // If we're sending this message directly to a member, it's a resend.
    else {
      log_length = snprintf(log_ptr, log_remaining, PSTR(", last ack=%u"), device_group_member->acked_sequence);
      log_ptr += log_length;
      log_remaining -= log_length;
      goto write_log;
    }
  }

  // If this is a status request message, skip item processing.
  if ((flags & DGR_FLAG_STATUS_REQUEST)) goto write_log;

  // If this is a received message, ...
  if (received) {

    // If we already processed this or a later message from this group member, ignore this message.
    if (device_group_member) {
      if (message_sequence <= device_group_member->received_sequence) {
        if (message_sequence == device_group_member->received_sequence || device_group_member->received_sequence - message_sequence > 64536) {
          log_length = snprintf(log_ptr, log_remaining, PSTR(" (old)"));
          log_ptr += log_length;
          log_remaining -= log_length;
          goto write_log;
        }
      }
      device_group_member->received_sequence = message_sequence;
    }

    /*
      XdrvMailbox
      bool          grpflg
      bool          usridx
      uint16_t      command_code    Item code
      uint32_t      index           0:15 Flags, 16:31 Message sequence
      uint32_t      data_len        String item value length
      int32_t       payload         Integer item value
      char         *topic           Pointer to device group index
      char         *data            Pointer to non-integer item value
      char         *command         nullptr
    */
    XdrvMailbox.command = nullptr;  // Indicates the source is a device group update
    XdrvMailbox.index = flags | message_sequence << 16;
    if (device_group_index == 0 && first_device_group_is_local) XdrvMailbox.index |= DGR_FLAG_LOCAL;
    XdrvMailbox.topic = (char *)&device_group_index;
    if (flags & (DGR_FLAG_MORE_TO_COME | DGR_FLAG_DIRECT)) TasmotaGlobal.skip_light_fade = true;

    // Set the flag to ignore device group send message request so callbacks from the drivers do not
    // send updates.
    ignore_dgr_sends = true;
  }

  uint8_t item;
  int32_t value;
  for (;;) {
    if (message_ptr >= message_end_ptr) goto badmsg;  // Malformed message
    item = *message_ptr++;
    if (!item) break; // Done

#ifdef DEVICE_GROUPS_DEBUG
    switch (item) {
      case DGR_ITEM_FLAGS:
      case DGR_ITEM_LIGHT_FADE:
      case DGR_ITEM_LIGHT_SPEED:
      case DGR_ITEM_LIGHT_BRI:
      case DGR_ITEM_LIGHT_SCHEME:
      case DGR_ITEM_LIGHT_FIXED_COLOR:
      case DGR_ITEM_BRI_PRESET_LOW:
      case DGR_ITEM_BRI_PRESET_HIGH:
      case DGR_ITEM_BRI_POWER_ON:
      case DGR_ITEM_POWER:
      case DGR_ITEM_EVENT:
      case DGR_ITEM_LIGHT_CHANNELS:
        break;
      default:
        AddLog_P(LOG_LEVEL_ERROR, PSTR("DGR: *** Invalid item=%u"), item);
    }
#endif  // DEVICE_GROUPS_DEBUG

    log_length = snprintf(log_ptr, log_remaining, PSTR(", %u="), item);
    log_ptr += log_length;
    log_remaining -= log_length;
    log_length = 0;
    if (item <= DGR_ITEM_LAST_32BIT) {
      value = *message_ptr++;
      if (item > DGR_ITEM_MAX_8BIT) {
        value |= *message_ptr++ << 8;
        if (item > DGR_ITEM_MAX_16BIT) {
          value |= *message_ptr++ << 16;
          value |= *message_ptr++ << 24;
#ifdef USE_DEVICE_GROUPS_SEND
          device_group->values_32bit[item - DGR_ITEM_MAX_16BIT - 1] = (item == DGR_ITEM_POWER ? value & 0xffffff : value);
#endif  // USE_DEVICE_GROUPS_SEND
        }
#ifdef USE_DEVICE_GROUPS_SEND
        else {
          device_group->values_16bit[item - DGR_ITEM_MAX_8BIT - 1] = value;
        }
#endif  // USE_DEVICE_GROUPS_SEND
      }
#ifdef USE_DEVICE_GROUPS_SEND
      else {
        device_group->values_8bit[item] = value;
      }
#endif  // USE_DEVICE_GROUPS_SEND
      log_length = snprintf(log_ptr, log_remaining, PSTR("%u"), value);
    }
    else {
      value = *message_ptr++;
      if (received) XdrvMailbox.data = (char *)message_ptr;
      if (message_ptr + value >= message_end_ptr) goto badmsg;  // Malformed message
      if (item <= DGR_ITEM_MAX_STRING) {
        log_length = snprintf(log_ptr, log_remaining, PSTR("'%s'"), message_ptr);
      }
      else {
        switch (item) {
          case DGR_ITEM_LIGHT_CHANNELS:
            log_length = snprintf(log_ptr, log_remaining, PSTR("%u,%u,%u,%u,%u,%u"), *message_ptr, *(message_ptr + 1), *(message_ptr + 2), *(message_ptr + 3), *(message_ptr + 4), *(message_ptr + 5));
            break;
        }
      }
      message_ptr += value;
    }
    log_ptr += log_length;
    log_remaining -= log_length;

    if (received && DeviceGroupItemShared(true, item)) {
      item_processed = true;
      XdrvMailbox.command_code = item;
      XdrvMailbox.payload = value;
      XdrvMailbox.data_len = value;
      *log_ptr++ = '*';
      log_remaining--;
      switch (item) {
        case DGR_ITEM_POWER:
          if (Settings.flag4.multiple_device_groups) {  // SetOption88 - Enable relays in separate device groups
            if (device_group_index < TasmotaGlobal.devices_present) {
              bool on = (value & 1);
              if (on != (TasmotaGlobal.power & (1 << device_group_index))) ExecuteCommandPower(device_group_index + 1, (on ? POWER_ON : POWER_OFF), SRC_REMOTE);
            }
          }
          else if (XdrvMailbox.index & DGR_FLAG_LOCAL) {
            uint8_t mask_devices = value >> 24;
            if (mask_devices > TasmotaGlobal.devices_present) mask_devices = TasmotaGlobal.devices_present;
            for (uint32_t i = 0; i < mask_devices; i++) {
              uint32_t mask = 1 << i;
              bool on = (value & mask);
              if (on != (TasmotaGlobal.power & mask)) ExecuteCommandPower(i + 1, (on ? POWER_ON : POWER_OFF), SRC_REMOTE);
            }
          }
          break;
#ifdef USE_RULES
        case DGR_ITEM_EVENT:
          CmndEvent();
          break;
#endif
        case DGR_ITEM_COMMAND:
          ExecuteCommand(XdrvMailbox.data, SRC_REMOTE);
          break;
      }
      XdrvCall(FUNC_DEVICE_GROUP_ITEM);
    }
  }

  if (received) {
    if (item_processed) {
      XdrvMailbox.command_code = DGR_ITEM_EOL;
      XdrvCall(FUNC_DEVICE_GROUP_ITEM);
    }
  }

write_log:
  *log_ptr++ = 0;
  AddLogData(LOG_LEVEL_DEBUG_MORE, log_buffer);

  // If this is a received status request message, then if the requestor didn't just ack our
  // previous full status update, send a full status update.
  if (received) {
    if ((flags & DGR_FLAG_STATUS_REQUEST)) {
      if ((flags & DGR_FLAG_RESET) || device_group_member->acked_sequence != device_group->last_full_status_sequence) {
        _SendDeviceGroupMessage(device_group_index, DGR_MSGTYP_FULL_STATUS);
      }
    }
  }

  // If this is a message being sent, send it.
  else {
    int attempt;
    IPAddress ip_address = (device_group_member ? device_group_member->ip_address : IPAddress(DEVICE_GROUPS_ADDRESS));
    for (attempt = 1; attempt <= 5; attempt++) {
      if (device_groups_udp.beginPacket(ip_address, DEVICE_GROUPS_PORT)) {
        device_groups_udp.write(message, message_length);
        if (device_groups_udp.endPacket()) break;
      }
      delay(10);
    }
    if (attempt > 5) AddLog_P(LOG_LEVEL_ERROR, PSTR("DGR: Error sending message"));
  }
  goto cleanup;

badmsg:
  AddLog_P(LOG_LEVEL_ERROR, PSTR("%s ** incorrect length"), log_buffer);

cleanup:
  if (received) {
    TasmotaGlobal.skip_light_fade = false;
    ignore_dgr_sends = false;
  }
}

bool _SendDeviceGroupMessage(uint8_t device_group_index, DevGroupMessageType message_type, ...)
{
  // If device groups is not up, ignore this request.
  if (!device_groups_up) return 1;

  // If the device group index is higher then the number of device groups, ignore this request.
  if (device_group_index >= device_group_count) return 0;

  // Extract the flags from the message type.
  bool with_local = ((message_type & DGR_MSGTYPFLAG_WITH_LOCAL) != 0);
  message_type = (DevGroupMessageType)(message_type & 0x7F);

  // If we're currently processing a remote device message, ignore this request.
  if (ignore_dgr_sends && message_type != DGR_MSGTYPE_UPDATE_COMMAND) return 0;

  // Get a pointer to the device information for this device.
  struct device_group * device_group = &device_groups[device_group_index];

  // If we're still sending initial status requests, ignore this request.
  if (device_group->initial_status_requests_remaining) return 1;

  // Load the message header, sequence and flags.
#ifdef DEVICE_GROUPS_DEBUG
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("DGR: Building %s %spacket"), device_group->group_name, (message_type == DGR_MSGTYP_FULL_STATUS ? PSTR("full status ") : PSTR("")));
#endif  // DEVICE_GROUPS_DEBUG
  uint16_t original_sequence = device_group->outgoing_sequence;
  uint16_t flags = 0;
  if (message_type == DGR_MSGTYP_UPDATE_MORE_TO_COME)
    flags = DGR_FLAG_MORE_TO_COME;
  else if (message_type == DGR_MSGTYP_UPDATE_DIRECT)
    flags = DGR_FLAG_DIRECT;
  uint8_t * message_ptr = BeginDeviceGroupMessage(device_group, flags, building_status_message || message_type == DGR_MSGTYP_PARTIAL_UPDATE);

  // A full status request is a request from a remote device for the status of every item we
  // control. As long as we're building it, we may as well multicast the status update to all
  // device group members.
  if (message_type == DGR_MSGTYP_FULL_STATUS) {
    device_group->last_full_status_sequence = device_group->outgoing_sequence;
    device_group->message_length = 0;

    // Set the flag indicating we're currently building a status message. SendDeviceGroupMessage
    // will build but not send messages while this flag is set.
    building_status_message = true;

    // Call the drivers to build the status update.
    SendDeviceGroupMessage(device_group_index, DGR_MSGTYP_PARTIAL_UPDATE, DGR_ITEM_POWER, TasmotaGlobal.power);
    XdrvMailbox.index = 0;
    if (device_group_index == 0 && first_device_group_is_local) XdrvMailbox.index = DGR_FLAG_LOCAL;
    XdrvMailbox.command_code = DGR_ITEM_STATUS;
    XdrvMailbox.topic = (char *)&device_group_index;
    XdrvCall(FUNC_DEVICE_GROUP_ITEM);
    building_status_message = false;

    // Set the status update flag in the outgoing message.
    *(message_ptr - 2) |= DGR_FLAG_FULL_STATUS;

    // If we have nothing to share with the other members, just send the EOL item.
    if (!device_group->message_length) {
      *message_ptr++ = 0;
      device_group->message_length = message_ptr - device_group->message;
    }
  }

  else {
#ifdef USE_DEVICE_GROUPS_SEND
    uint8_t out_buffer[128];
    bool escaped;
    char chr;
    char oper;
    uint32_t old_value;
    uint8_t * out_ptr = out_buffer;
#endif  // USE_DEVICE_GROUPS_SEND
    struct item {
      uint8_t item;
      uint32_t value;
      void * value_ptr;
    } item_array[32];
    bool shared;
    uint8_t item;
    uint32_t value;
    uint8_t * value_ptr;
    uint8_t * first_item_ptr = message_ptr;
    struct item * item_ptr;
    va_list ap;

    // Build an array of all the items and values in this update.
    item_ptr = item_array;
#ifdef USE_DEVICE_GROUPS_SEND
    if (message_type == DGR_MSGTYPE_UPDATE_COMMAND) {
      value_ptr = (uint8_t *)XdrvMailbox.data;
      while ((item = strtoul((char *)value_ptr, (char **)&value_ptr, 0))) {
        item_ptr->item = item;
        if (*value_ptr != '=') return 1;
        value_ptr++;
        if (item <= DGR_ITEM_MAX_32BIT) {
          oper = 0;
          if (*value_ptr == '@') {
            oper = value_ptr[1];
            value_ptr += 2;
          }
          value = (isdigit(*value_ptr) ? strtoul((char *)value_ptr, (char **)&value_ptr, 0) : 1);
          if (oper) {
            old_value = (item <= DGR_ITEM_MAX_8BIT ? device_group->values_8bit[item] : (item <= DGR_ITEM_MAX_16BIT ? device_group->values_16bit[item - DGR_ITEM_MAX_8BIT - 1] : device_group->values_32bit[item - DGR_ITEM_MAX_16BIT - 1]));
            value = (oper == '+' ? old_value + value : (oper == '-' ? old_value - value : (oper == '^' ? old_value ^ (value ? value : 0xffffffff) : old_value)));
          }
          item_ptr->value = value;
        }
        else {
          item_ptr->value_ptr = out_ptr;
          if (item <= DGR_ITEM_MAX_STRING) {
            escaped = false;
            while ((chr = *value_ptr++)) {
              if (chr == ' ' && !escaped) break;
              if (!(escaped = (chr == '\\' && !escaped))) *out_ptr++ = chr;
            }
            *out_ptr = 0;
          }
          else {
            switch (item) {
              case DGR_ITEM_LIGHT_CHANNELS:
                for (int i = 0; i < 6; i++) {
                  *out_ptr++ = strtoul((char *)value_ptr, (char **)&value_ptr, 0);
                  if (*value_ptr == ',') value_ptr++;
                }
                break;
            }
          }
        }
        item_ptr++;
      }
    }
    else {
#endif  // USE_DEVICE_GROUPS_SEND
      va_start(ap, message_type);
      while ((item = va_arg(ap, int))) {
        item_ptr->item = item;
        if (item <= DGR_ITEM_MAX_32BIT)
          item_ptr->value = va_arg(ap, int);
        else if (item <= DGR_ITEM_MAX_STRING)
          item_ptr->value_ptr = va_arg(ap, char *);
        else {
          item_ptr->value_ptr = va_arg(ap, uint8_t *);
        }
        item_ptr++;
      }
      va_end(ap);
#ifdef USE_DEVICE_GROUPS_SEND
    }
#endif  // USE_DEVICE_GROUPS_SEND
    item_ptr->item = 0;

    // If we're still building this update or all group members haven't acknowledged the previous
    // update yet, update the message to include these new updates. First we need to rebuild the
    // previous update message to remove any items and their values that are included in this new
    // update.
    if (device_group->message_length) {
      int kept_item_count = 0;

      // Rebuild the previous update message, removing any items whose values are included in this
      // new update.
      uint8_t * previous_message_ptr = message_ptr;
      while (item = *previous_message_ptr++) {

        // Determine the length of this item's value.
        if (item <= DGR_ITEM_MAX_32BIT) {
          value = 1;
          if (item > DGR_ITEM_MAX_8BIT) {
            value = 2;
            if (item > DGR_ITEM_MAX_16BIT) {
              value = 4;
            }
          }
        }
        else {
          value = *previous_message_ptr + 1;
        }

        // Search for this item in the new update.
        for (item_ptr = item_array; item_ptr->item; item_ptr++) {
          if (item_ptr->item == item) break;
        }

        // If this item was not found in the new update, copy it to the new update message.
        if (!item_ptr->item) {
          kept_item_count++;
          *message_ptr++ = item;
          memmove(message_ptr, previous_message_ptr, value);
          message_ptr += value;
        }

        // Advance past the item value.
        previous_message_ptr += value;
      }
#ifdef DEVICE_GROUPS_DEBUG
      AddLog_P(LOG_LEVEL_DEBUG, PSTR("DGR: %u items carried over"), kept_item_count);
#endif  // DEVICE_GROUPS_DEBUG
    }

    // Itertate through the passed items adding them and their values to the message.
    for (item_ptr = item_array; (item = item_ptr->item); item_ptr++) {

      // If this item is shared with the group add it to the message.
      shared = true;
      if (!device_group_index && message_type != DGR_MSGTYPE_UPDATE_COMMAND) shared = DeviceGroupItemShared(false, item);
      if (shared) {
        *message_ptr++ = item;

        // For integer items, add the value to the message.
        if (item <= DGR_ITEM_MAX_32BIT) {
          value = item_ptr->value;
          *message_ptr++ = value & 0xff;
          if (item > DGR_ITEM_MAX_8BIT) {
            value >>= 8;
            *message_ptr++ = value & 0xff;
            if (item > DGR_ITEM_MAX_16BIT) {
              value >>= 8;
              *message_ptr++ = value & 0xff;
              value >>= 8;
              // For the power item, the device count is overlayed onto the highest 8 bits.
              if (item == DGR_ITEM_POWER && !value) value = (device_group_index == 0 && first_device_group_is_local ? TasmotaGlobal.devices_present : 1);
              *message_ptr++ = value;
            }
          }
        }

        // For string items and special items, get the value length.
        else {
          if (item <= DGR_ITEM_MAX_STRING) {
            value = strlen((const char *)item_ptr->value_ptr) + 1;
          }
          else {
            switch (item) {
              case DGR_ITEM_LIGHT_CHANNELS:
                value = 6;
                break;
            }
          }

          // Load the length and copy the value.
          *message_ptr++ = value;
          memcpy(message_ptr, item_ptr->value_ptr, value);
          message_ptr += value;
        }
      }
    }

    // If we added any items, add the EOL item code and calculate the message length.
    if (message_ptr != first_item_ptr) {
      *message_ptr++ = 0;
      device_group->message_length = message_ptr - device_group->message;
    }

    // If there's going to be more items added to this message, return.
    if (building_status_message || message_type == DGR_MSGTYP_PARTIAL_UPDATE) return 0;
  }

  // If there is no message, restore the sequence number and return.
  if (!device_group->message_length) {
    device_group->outgoing_sequence = original_sequence;
    return 0;
  }

  // Multicast the packet.
  SendReceiveDeviceGroupMessage(device_group, nullptr, device_group->message, device_group->message_length, false);

#ifdef USE_DEVICE_GROUPS_SEND
  // If requested, handle this updated locally as well.
  if (with_local) {
    struct XDRVMAILBOX save_XdrvMailbox = XdrvMailbox;
    SendReceiveDeviceGroupMessage(device_group, nullptr, device_group->message, device_group->message_length, true);
    XdrvMailbox = save_XdrvMailbox;
  }
#endif  // USE_DEVICE_GROUPS_SEND

  uint32_t now = millis();
  if (message_type == DGR_MSGTYP_UPDATE_MORE_TO_COME) {
    device_group->message_length = 0;
    device_group->next_ack_check_time = 0;
  }
  else {
    device_group->ack_check_interval = 200;
    device_group->next_ack_check_time = now + device_group->ack_check_interval;
    if (device_group->next_ack_check_time < next_check_time) next_check_time = device_group->next_ack_check_time;
    device_group->member_timeout_time = now + DGR_MEMBER_TIMEOUT;
  }

  device_group->next_announcement_time = now + DGR_ANNOUNCEMENT_INTERVAL;
  if (device_group->next_announcement_time < next_check_time) next_check_time = device_group->next_announcement_time;
  return 0;
}

void ProcessDeviceGroupMessage(uint8_t * message, int message_length)
{
  // Search for a device group with the target group name. If one isn't found, return.
  uint8_t device_group_index = 0;
  struct device_group * device_group = device_groups;
  char * message_group_name = (char *)message + sizeof(DEVICE_GROUP_MESSAGE) - 1;
  for (;;) {
    if (!strcmp(message_group_name, device_group->group_name)) break;
    if (++device_group_index >= device_group_count) return;
    device_group++;
  }

  // Find the group member. If this is a new group member, add it.
  struct device_group_member * device_group_member;
  IPAddress remote_ip = device_groups_udp.remoteIP();
  struct device_group_member * * flink = &device_group->device_group_members;
  for (;;) {
    device_group_member = *flink;
    if (!device_group_member) {
      device_group_member = (struct device_group_member *)calloc(1, sizeof(struct device_group_member));
      if (device_group_member == nullptr) {
        AddLog_P(LOG_LEVEL_ERROR, PSTR("DGR: Error allocating member block"));
        return;
      }
      device_group_member->ip_address = remote_ip;
      *flink = device_group_member;
      AddLog_P(LOG_LEVEL_DEBUG, PSTR("DGR: Member %s added"), IPAddressToString(remote_ip));
      break;
    }
    else if (device_group_member->ip_address == remote_ip) {
      break;
    }
    flink = &device_group_member->flink;
  }

  SendReceiveDeviceGroupMessage(device_group, device_group_member, message, message_length, true);
}

void DeviceGroupStatus(uint8_t device_group_index)
{
  if (Settings.flag4.device_groups_enabled && device_group_index < device_group_count) {
    char buffer[1024];
    int member_count = 0;
    struct device_group * device_group = &device_groups[device_group_index];
    buffer[0] = buffer[1] = 0;
    for (struct device_group_member * device_group_member = device_group->device_group_members; device_group_member; device_group_member = device_group_member->flink) {
      snprintf_P(buffer, sizeof(buffer), PSTR("%s,{\"IPAddress\":\"%s\",\"ResendCount\":%u,\"LastRcvdSeq\":%u,\"LastAckedSeq\":%u}"), buffer, IPAddressToString(device_group_member->ip_address), device_group_member->unicast_count, device_group_member->received_sequence, device_group_member->acked_sequence);
      member_count++;
    }
    Response_P(PSTR("{\"" D_CMND_DEVGROUPSTATUS "\":{\"Index\":%u,\"GroupName\":\"%s\",\"MessageSeq\":%u,\"MemberCount\":%d,\"Members\":[%s]}}"), device_group_index, device_group->group_name, device_group->outgoing_sequence, member_count, &buffer[1]);
  }
}

void DeviceGroupsLoop(void)
{
  if (!device_groups_up || TasmotaGlobal.restart_flag) return;

  while (device_groups_udp.parsePacket()) {
    uint8_t packet_buffer[512];
    int length = device_groups_udp.read(packet_buffer, sizeof(packet_buffer) - 1);
    if (length > 0) {
      packet_buffer[length] = 0;
      if (!strncmp_P((char *)packet_buffer, kDeviceGroupMessage, sizeof(DEVICE_GROUP_MESSAGE) - 1)) {
        ProcessDeviceGroupMessage(packet_buffer, length);
      }
    }
  }

  uint32_t now = millis();

  // If it's time to check on things, iterate through the device groups.
  if ((long)(now - next_check_time) >= 0) {
#ifdef DEVICE_GROUPS_DEBUG
AddLog_P(LOG_LEVEL_DEBUG, PSTR("DGR: Checking next_check_time=%u, now=%u"), next_check_time, now);
#endif  // DEVICE_GROUPS_DEBUG
    next_check_time = now + DGR_ANNOUNCEMENT_INTERVAL * 2;

    struct device_group * device_group = device_groups;
    for (uint32_t device_group_index = 0; device_group_index < device_group_count; device_group_index++, device_group++) {

      // If we're still waiting for acks to the last update from this device group, ...
      if (device_group->next_ack_check_time) {

        // If it's time to check for acks, ...
        if ((long)(now - device_group->next_ack_check_time) >= 0) {

          // If we're still sending the initial status request message, send it.
          if (device_group->initial_status_requests_remaining) {
            if (--device_group->initial_status_requests_remaining) {
#ifdef DEVICE_GROUPS_DEBUG
            AddLog_P(LOG_LEVEL_DEBUG, PSTR("DGR: Sending initial status request for group %s"), device_group->group_name);
#endif  // DEVICE_GROUPS_DEBUG
              SendReceiveDeviceGroupMessage(device_group, nullptr, device_group->message, device_group->message_length, false);
              device_group->message[device_group->message_header_length + 2] = DGR_FLAG_STATUS_REQUEST; // The reset flag is on only for the first packet - turn it off now
              next_check_time = device_group->next_ack_check_time = now + 200;
              continue;
            }

            // If we've sent the initial status request message the set number of times, send our
            // status to all the members.
            else {
              _SendDeviceGroupMessage(device_group_index, DGR_MSGTYP_FULL_STATUS);
            }
          }

          // If we're done initializing, iterate through the group memebers, ...
          else {
#ifdef DEVICE_GROUPS_DEBUG
            AddLog_P(LOG_LEVEL_DEBUG, PSTR("DGR: Checking for ack's"));
#endif  // DEVICE_GROUPS_DEBUG
            bool acked = true;
            struct device_group_member ** flink = &device_group->device_group_members;
            struct device_group_member * device_group_member;
            while ((device_group_member = *flink)) {

              // If we have not received an ack to our last message from this member, ...
              if (device_group_member->acked_sequence != device_group->outgoing_sequence) {

                // If we haven't receive an ack from this member in DGR_MEMBER_TIMEOUT ms, assume
                // they're offline and remove them from the group.
                if ((long)(now - device_group->member_timeout_time) >= 0) {
                  *flink = device_group_member->flink;
                  free(device_group_member);
                  AddLog_P(LOG_LEVEL_DEBUG, PSTR("DGR: Member %s removed"), IPAddressToString(device_group_member->ip_address));
                  continue;
                }

                // Otherwise, unicast the last message directly to this member.
                SendReceiveDeviceGroupMessage(device_group, device_group_member, device_group->message, device_group->message_length, false);
                device_group_member->unicast_count++;
                acked = false;
              }
              flink = &device_group_member->flink;
            }

            // If we've received an ack to the last message from all members, clear the ack check
            // time and zero-out the message length.
            if (acked) {
              device_group->next_ack_check_time = 0;
              device_group->message_length = 0; // Let _SendDeviceGroupMessage know we're done with this update
            }

            // If there are still members we haven't received an ack from, set the next ack check
            // time. We start at 200ms and double the interval each pass with a maximum interval of
            // 5 seconds.
            else {
              device_group->ack_check_interval *= 2;
              if (device_group->ack_check_interval > 5000) device_group->ack_check_interval = 5000;
              device_group->next_ack_check_time = now + device_group->ack_check_interval;
            }
          }
        }

        if (device_group->next_ack_check_time < next_check_time) next_check_time = device_group->next_ack_check_time;
      }

      // If it's time to send a multicast announcement for this group, send it. This is to
      // announce ourself to any members that have somehow not heard about us. We send it at the
      // announcement interval plus a random number of milliseconds so that even if all the devices
      // booted at the same time, they don't all multicast their announcements at the same time.
#ifdef DEVICE_GROUPS_DEBUG
      AddLog_P(LOG_LEVEL_DEBUG, PSTR("DGR: next_announcement_time=%u, now=%u"), device_group->next_announcement_time, now);
#endif  // DEVICE_GROUPS_DEBUG
      if ((long)(now - device_group->next_announcement_time) >= 0) {
        SendReceiveDeviceGroupMessage(device_group, nullptr, device_group->message, BeginDeviceGroupMessage(device_group, DGR_FLAG_ANNOUNCEMENT, true) - device_group->message, false);
        device_group->next_announcement_time = now + DGR_ANNOUNCEMENT_INTERVAL + random(10000);
      }
      if (device_group->next_announcement_time < next_check_time) next_check_time = device_group->next_announcement_time;
    }
  }
}

#endif  // USE_DEVICE_GROUPS
