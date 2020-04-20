/*
  support_device_groups.ino - device groups support for Tasmota

  Copyright (C) 2020  Paul C Diem

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

extern bool udp_connected;

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
  uint16_t last_full_status_sequence;
  uint16_t message_length;
  uint16_t ack_check_interval;
  uint8_t message_header_length;
  uint8_t initial_status_requests_remaining;
  bool local;
  char group_name[TOPSZ];
  char message[128];
  struct device_group_member * device_group_members;
#ifdef USE_DEVICE_GROUPS_SEND
  uint8_t values_8bit[DGR_ITEM_LAST_8BIT];
  uint16_t values_16bit[DGR_ITEM_LAST_16BIT - DGR_ITEM_MAX_8BIT - 1];
  uint32_t values_32bit[DGR_ITEM_LAST_32BIT - DGR_ITEM_MAX_16BIT - 1];
#endif  // USE_DEVICE_GROUPS_SEND
};

struct device_group * device_groups;
uint32_t next_check_time = 0;
uint16_t outgoing_sequence = 0;
bool device_groups_initialized = false;
bool device_groups_initialization_failed = false;
bool building_status_message = false;
bool processing_remote_device_message = false;
bool udp_was_connected = false;

void DeviceGroupsInit(void)
{
  // If there are more device group names set than the number of device groups needed by the
  // mdoule, use the device group name count as the device group count.
  for (; device_group_count < MAX_DEV_GROUP_NAMES; device_group_count++) {
    if (!*SettingsText(SET_DEV_GROUP_NAME1 + device_group_count)) break;
  }

  // Initialize the device information for each device group.
  device_groups = (struct device_group *)calloc(device_group_count, sizeof(struct device_group));
  if (device_groups == nullptr) {
    AddLog_P2(LOG_LEVEL_ERROR, PSTR("DGR: Error allocating %u-element device group array"), device_group_count);
    device_groups_initialization_failed = true;
    return;
  }

  for (uint32_t device_group_index = 0; device_group_index < device_group_count; device_group_index++) {
    struct device_group * device_group = &device_groups[device_group_index];
    strcpy(device_group->group_name, SettingsText(SET_DEV_GROUP_NAME1 + device_group_index));

    // If the device group name is not set, use the MQTT group topic (with the device group index +
    // 1 appended for device group indices > 0).
    if (!device_group->group_name[0]) {
      strcpy(device_group->group_name, SettingsText(SET_MQTT_GRP_TOPIC));
      if (device_group_index) {
        char str[10];
        sprintf_P(str, PSTR("%u"), device_group_index + 1);
        strcat(device_group->group_name, str);
      }
    }
    device_group->message_header_length = sprintf_P(device_group->message, PSTR("%s%s HTTP/1.1\n\n"), kDeviceGroupMessage, device_group->group_name);
    device_group->last_full_status_sequence = -1;
  }

  device_groups[0].local = true;

  // If both in and out shared items masks are 0, assume they're unitialized and initialize them.
  if (!Settings.device_group_share_in && !Settings.device_group_share_out) {
    Settings.device_group_share_in = Settings.device_group_share_out = 0xffffffff;
  }

  device_groups_initialized = true;
}

char * IPAddressToString(const IPAddress& ip_address)
{
  static char buffer[16];
  sprintf_P(buffer, PSTR("%u.%u.%u.%u"), ip_address[0], ip_address[1], ip_address[2], ip_address[3]);
  return buffer;
}

char * BeginDeviceGroupMessage(struct device_group * device_group, uint16_t flags, bool hold_sequence = false)
{
  char * message_ptr = &device_group->message[device_group->message_header_length];
  if (!hold_sequence && !++outgoing_sequence) outgoing_sequence = 1;
  *message_ptr++ = outgoing_sequence & 0xff;
  *message_ptr++ = outgoing_sequence >> 8;
  *message_ptr++ = flags & 0xff;
  *message_ptr++ = flags >> 8;
  return message_ptr;
}

// Return true if we're configured to share the specified item.
bool DeviceGroupItemShared(bool incoming, uint8_t item)
{
  uint8_t mask = 0;
  switch (item) {
    case DGR_ITEM_LIGHT_BRI:
    case DGR_ITEM_BRI_POWER_ON:
      mask = DGR_SHARE_LIGHT_BRI;
      break;
    case DGR_ITEM_POWER:
      mask = DGR_SHARE_POWER;
      break;
    case DGR_ITEM_LIGHT_SCHEME:
      mask = DGR_SHARE_LIGHT_SCHEME;
      break;
    case DGR_ITEM_LIGHT_FIXED_COLOR:
    case DGR_ITEM_LIGHT_CHANNELS:
      mask = DGR_SHARE_LIGHT_COLOR;
      break;
    case DGR_ITEM_LIGHT_FADE:
    case DGR_ITEM_LIGHT_SPEED:
      mask = DGR_SHARE_LIGHT_FADE;
      break;
    case DGR_ITEM_BRI_PRESET_LOW:
    case DGR_ITEM_BRI_PRESET_HIGH:
      mask = DGR_SHARE_DIMMER_SETTINGS;
      break;
    case DGR_ITEM_EVENT:
      mask = DGR_SHARE_EVENT;
      break;
  }
  return (!mask || ((incoming ? Settings.device_group_share_in : Settings.device_group_share_out) & mask));
}

void SendDeviceGroupPacket(IPAddress ip, char * packet, int len, const char * label)
{
  if (!ip) ip = IPAddress(239,255,255,250);
  for (int attempt = 1; attempt <= 5; attempt++) {
    if (PortUdp.beginPacket(ip, 1900)) {
      PortUdp_write(packet, len);
      if (PortUdp.endPacket()) return;
    }
    delay(10);
  }
  AddLog_P2(LOG_LEVEL_ERROR, PSTR("DGR: Error sending %s packet"), label);
}

void _SendDeviceGroupMessage(uint8_t device_group_index, DevGroupMessageType message_type, ...)
{
  // If device groups are not enabled, ignore this request.
  if (!Settings.flag4.device_groups_enabled) return;

  // If UDP is not set up, ignore this request.
  if (!udp_connected) return;

  // If we're currently processing a remote device message, ignore this request.
  if (processing_remote_device_message && message_type != DGR_MSGTYPE_UPDATE_COMMAND) return;

  // Get a pointer to the device information for this device.
  struct device_group * device_group = &device_groups[device_group_index];

  // If we're still sending initial status requests, ignore this request.
  if (device_group->initial_status_requests_remaining) return;

  // A full status request is a request from a remote device for the status of every item we
  // control. As long as we're building it, we may as well multicast the status update to all
  // device group members.
  char * message_ptr = &device_group->message[device_group->message_header_length];
  if (message_type == DGR_MSGTYP_FULL_STATUS) {

    // Set the flag indicating we're currently building a status message. SendDeviceGroupMessage
    // will build but not send messages while this flag is set.
    building_status_message = true;

    // Call the drivers to build the status update.
    if (!++outgoing_sequence) outgoing_sequence = 1;
#ifdef DEVICE_GROUPS_DEBUG
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Building device group %s full status packet"), device_group->group_name);
#endif  // DEVICE_GROUPS_DEBUG
    device_group->message_length = 0;
    SendDeviceGroupMessage(device_group_index, DGR_MSGTYP_PARTIAL_UPDATE, DGR_ITEM_POWER, power);
    XdrvMailbox.index = device_group_index << 16;
    XdrvMailbox.command_code = DGR_ITEM_STATUS;
    XdrvMailbox.topic = (char *)&device_group_index;
    XdrvCall(FUNC_DEVICE_GROUP_ITEM);
    building_status_message = false;

    // If we have nothing to share with the other members, restore the message sequence and return.
    if (!device_group->message_length) {
      if (!--outgoing_sequence) outgoing_sequence = -1;
      return;
    }
    device_group->last_full_status_sequence = outgoing_sequence;

    // Set the status update flag in the outgoing message.
    *(message_ptr + 2) |= DGR_FLAG_FULL_STATUS;
  }

  else {
#ifdef USE_DEVICE_GROUPS_SEND
    bool escaped;
    bool use_command;
    char chr;
    char oper;
    uint32_t old_value;
    char * out_ptr;
#endif  // USE_DEVICE_GROUPS_SEND
    struct item {
      uint8_t item;
      uint32_t value;
      void * value_ptr;
    } item_array[32];
    bool shared;
    uint8_t item;
    uint32_t value;
    char * value_ptr;
    struct item * item_ptr;
    va_list ap;

#ifdef DEVICE_GROUPS_DEBUG
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Building device group %s packet"), device_group->group_name);
#endif  // DEVICE_GROUPS_DEBUG

    value = 0;
    if (message_type == DGR_MSGTYP_UPDATE_MORE_TO_COME)
      value |= DGR_FLAG_MORE_TO_COME;
    else if (message_type == DGR_MSGTYP_UPDATE_DIRECT)
      value |= DGR_FLAG_DIRECT;
#ifdef DEVICE_GROUPS_DEBUG
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR(">sequence=%u, flags=%u"), outgoing_sequence, value);
#endif  // DEVICE_GROUPS_DEBUG
    message_ptr = BeginDeviceGroupMessage(device_group, value, building_status_message || message_type == DGR_MSGTYP_PARTIAL_UPDATE);

    // Build an array of all the items and values in this update.
    memset(item_array, 0, sizeof(item_array));
    item_ptr = item_array;
#ifdef USE_DEVICE_GROUPS_SEND
    use_command = (message_type == DGR_MSGTYPE_UPDATE_COMMAND);
    if (use_command) {
      value_ptr = XdrvMailbox.data;
      while ((item = strtoul(value_ptr, &value_ptr, 0))) {
        item_ptr->item = item;
        if (*value_ptr == '=') {
          value_ptr++;
          if (item <= DGR_ITEM_MAX_32BIT) {
            oper = 0;
            if (*value_ptr == '@') {
              oper = value_ptr[1];
              value_ptr += 2;
            }
            value = strtoul(value_ptr, &value_ptr, 0);
            if (oper) {
              old_value = (item <= DGR_ITEM_MAX_8BIT ? device_group->values_8bit[item] : (item <= DGR_ITEM_MAX_16BIT ? device_group->values_16bit[item - DGR_ITEM_MAX_8BIT - 1] : device_group->values_32bit[item - DGR_ITEM_MAX_16BIT - 1]));
              value = (oper == '+' ? old_value + value : (oper == '-' ? old_value - value : (oper == '^' ? old_value ^ (value ? value : 0xffffffff) : old_value)));
            }
            item_ptr->value = value;
          }
          else if (item <= DGR_ITEM_MAX_STRING) {
            item_ptr->value_ptr = out_ptr = value_ptr;
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
                item_ptr->value_ptr = out_ptr = value_ptr;
                for (int i = 0; i < 5; i++) {
                  *out_ptr = strtoul(value_ptr, &value_ptr, 0);
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
          switch (item) {
            case DGR_ITEM_LIGHT_CHANNELS:
              item_ptr->value_ptr = va_arg(ap, uint8_t *);
              break;
          }
        }
        item_ptr++;
      }
      va_end(ap);
#ifdef USE_DEVICE_GROUPS_SEND
    }
#endif  // USE_DEVICE_GROUPS_SEND

    // If we're still building this update or all group members haven't acknowledged the previous
    // update yet, update the message to include these new updates. First we need to rebuild the
    // previous update message to remove any items and their values that are included in this new
    // update.
    if (device_group->message_length) {
      int kept_item_count = 0;

      // Rebuild the previous update message, removing any items whose values are included in this
      // new update.
      char * previous_message_ptr = message_ptr;
      while (item = *previous_message_ptr++) {

        // Search for this item in the new update.
        for (item_ptr = item_array; item_ptr->item; item_ptr++) {
          if (item_ptr->item == item) break;
        }

        // If this item was found in the new update skip over it and it's value.
        if (item_ptr->item) {
          if (item <= DGR_ITEM_MAX_32BIT) {
            previous_message_ptr++;
            if (item > DGR_ITEM_MAX_8BIT) {
              previous_message_ptr++;
              if (item > DGR_ITEM_MAX_16BIT) {
                previous_message_ptr++;
                previous_message_ptr++;
              }
            }
          }
          else if (item <= DGR_ITEM_MAX_STRING) {
            previous_message_ptr += strlen(previous_message_ptr) + 1;
          }
          else {
            switch (item) {
              case DGR_ITEM_LIGHT_CHANNELS:
                previous_message_ptr += 5;
                break;
            }
          }
        }

        // If this item was not found in the new udpate, copy it to the new update message.
        else {
          *message_ptr++ = item;
          if (item <= DGR_ITEM_MAX_32BIT) {
            *message_ptr++ = *previous_message_ptr++;
            if (item > DGR_ITEM_MAX_8BIT) {
              *message_ptr++ = *previous_message_ptr++;
              if (item > DGR_ITEM_MAX_16BIT) {
                *message_ptr++ = *previous_message_ptr++;
                *message_ptr++ = *previous_message_ptr++;
              }
            }
          }
          else if (item <= DGR_ITEM_MAX_STRING) {
            value = strlen(previous_message_ptr) + 1;
            memmove(message_ptr, previous_message_ptr, value);
            previous_message_ptr += value;
            message_ptr += value;
          }
          else {
            switch (item) {
              case DGR_ITEM_LIGHT_CHANNELS:
                memmove(message_ptr, previous_message_ptr, 5);
                previous_message_ptr += 5;
                message_ptr += 5;
                break;
            }
          }
          kept_item_count++;
        }
      }
#ifdef DEVICE_GROUPS_DEBUG
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%u items carried over from previous update"), kept_item_count);
#endif  // DEVICE_GROUPS_DEBUG
    }

    // Itertate through the passed items adding them and their values to the message.
    for (item_ptr = item_array; (item = item_ptr->item); item_ptr++) {

      // If this item is shared with the group add it to the message.
      shared = true;
      if (!device_group_index) shared = DeviceGroupItemShared(false, item);
      if (shared) {
        *message_ptr++ = item;

        // For integer items, add the value to the message.
        if (item <= DGR_ITEM_MAX_32BIT) {
          value = item_ptr->value;
//#ifdef DEVICE_GROUPS_DEBUG
          AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR(">%u=%u"), item, value);
//#endif  // DEVICE_GROUPS_DEBUG
          *message_ptr++ = value & 0xff;
          if (item > DGR_ITEM_MAX_8BIT) {
#ifdef USE_DEVICE_GROUPS_SEND
            old_value = value;
#endif  // USE_DEVICE_GROUPS_SEND
            value >>= 8;
            *message_ptr++ = value & 0xff;
            if (item > DGR_ITEM_MAX_16BIT) {
              value >>= 8;
              *message_ptr++ = value & 0xff;
              value >>= 8;

              // For the power item, the device count is overlayed onto the highest 8 bits.
              if (item == DGR_ITEM_POWER) {
                if (!value)
                  value = (device_group_index == 0 ? devices_present : 1);
#ifdef USE_DEVICE_GROUPS_SEND
                else
                  old_value = old_value & 0xffffff;
#endif  // USE_DEVICE_GROUPS_SEND
              }

              *message_ptr++ = value;
#ifdef USE_DEVICE_GROUPS_SEND
              device_group->values_32bit[item - DGR_ITEM_MAX_16BIT - 1] = old_value;
#endif  // USE_DEVICE_GROUPS_SEND
            }
#ifdef USE_DEVICE_GROUPS_SEND
            else {
              device_group->values_16bit[item - DGR_ITEM_MAX_8BIT - 1] = old_value;
            }
#endif  // USE_DEVICE_GROUPS_SEND
          }
#ifdef USE_DEVICE_GROUPS_SEND
          else {
            device_group->values_8bit[item] = value;
          }
#endif  // USE_DEVICE_GROUPS_SEND
        }

        // For string items, add the null-terminated string to the message.
        else if (item <= DGR_ITEM_MAX_STRING) {
          if (item_ptr->value_ptr) {
            value = strlen((const char *)item_ptr->value_ptr);
            memcpy(message_ptr, item_ptr->value_ptr, value);
            message_ptr += value;
          }
          *message_ptr++ = 0;
//#ifdef DEVICE_GROUPS_DEBUG
          AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR(">%u='%s'"), item, item_ptr->value_ptr);
//#endif  // DEVICE_GROUPS_DEBUG
        }

        // For special items, handle them individually.
        else {
          switch (item) {
            case DGR_ITEM_LIGHT_CHANNELS:
              value_ptr = (char *)item_ptr->value_ptr;
              for (int i = 0; i < 5; i++) {
                *message_ptr++ = (value_ptr ? *value_ptr++ : 0);
              }
//#ifdef DEVICE_GROUPS_DEBUG
              AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR(">%u=%u,%u,%u,%u,%u"), item, *(message_ptr - 5), *(message_ptr - 4), *(message_ptr - 3), *(message_ptr - 2), *(message_ptr - 1));
//#endif  // DEVICE_GROUPS_DEBUG
              break;
          }
        }
      }
    }

    // Add the EOL item code and calculate the message length.
    *message_ptr++ = 0;
    device_group->message_length = message_ptr - device_group->message;

    // If there's going to be more items added to this message, return.
    if (building_status_message || message_type == DGR_MSGTYP_PARTIAL_UPDATE) return;
  }

  // Multicast the packet.
#ifdef DEVICE_GROUPS_DEBUG
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("DGR: Sending %u-byte device group %s packet via multicast, sequence=%u"), device_group->message_length, device_group->group_name, device_group->message[device_group->message_header_length] | device_group->message[device_group->message_header_length + 1] << 8);
#endif  // DEVICE_GROUPS_DEBUG
  SendDeviceGroupPacket(IPAddress(0,0,0,0), device_group->message, device_group->message_length, PSTR("Multicast"));

  uint32_t now = millis();
  if (message_type == DGR_MSGTYP_UPDATE_MORE_TO_COME) {
    device_group->message_length = 0;
    device_group->next_ack_check_time = 0;
  }
  else {
    device_group->ack_check_interval = 100;
    device_group->next_ack_check_time = now + device_group->ack_check_interval;
    if (device_group->next_ack_check_time < next_check_time) next_check_time = device_group->next_ack_check_time;
    device_group->member_timeout_time = now + DGR_MEMBER_TIMEOUT;
  }

  device_group->next_announcement_time = now + DGR_ANNOUNCEMENT_INTERVAL;
  if (device_group->next_announcement_time < next_check_time) next_check_time = device_group->next_announcement_time;
}

void ProcessDeviceGroupMessage(char * packet, int packet_length)
{
  // Make the group name a null-terminated string.
  char * message_group_name = packet + sizeof(DEVICE_GROUP_MESSAGE) - 1;
  char * message_ptr = strchr(message_group_name, ' ');
  if (message_ptr == nullptr) return;
  *message_ptr = 0;

  // Search for a device group with the target group name. If one isn't found, return.
  struct device_group * device_group;
  uint8_t device_group_index = 0;
  for (;;) {
    device_group = &device_groups[device_group_index];
    if (!strcmp(message_group_name, device_group->group_name)) break;
    if (++device_group_index >= device_group_count) return;
  }
  *message_ptr++ = ' ';

  // Find the group member. If this is a new group member, add it.
  IPAddress remote_ip = PortUdp.remoteIP();
  struct device_group_member * * flink = &device_group->device_group_members;
  struct device_group_member * device_group_member;
  for (;;) {
    device_group_member = *flink;
    if (!device_group_member) {
      device_group_member = (struct device_group_member *)calloc(1, sizeof(struct device_group_member));
      if (device_group_member == nullptr) {
        AddLog_P2(LOG_LEVEL_ERROR, PSTR("DGR: Error allocating device group member block"));
        return;
      }
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("DGR: Adding member %s"), IPAddressToString(remote_ip));
      device_group_member->ip_address = remote_ip;
      *flink = device_group_member;
      break;
    }
    else if (device_group_member->ip_address == remote_ip) {
      break;
    }
    flink = &device_group_member->flink;
  }

  // Find the start of the actual message (after the http header).
  message_ptr = strstr_P(message_ptr, PSTR("\n\n"));
  if (message_ptr == nullptr) return;
  message_ptr += 2;

  bool light_fade;
  uint16_t flags;
  uint16_t item;
  uint16_t message_sequence;
  int32_t value;

  // Get the message sequence and flags.
  if (packet_length - (message_ptr - packet) < 4) goto badmsg;  // Malformed message - must be at least 16-bit sequence, 16-bit flags left
  message_sequence = *message_ptr++;
  message_sequence |= *message_ptr++ << 8;
  flags = *message_ptr++;
  flags |= *message_ptr++ << 8;
#ifdef DEVICE_GROUPS_DEBUG
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Received device group %s packet from %s: sequence=%u, flags=%u"), device_group->group_name, IPAddressToString(remote_ip), message_sequence, flags);
#endif  // DEVICE_GROUPS_DEBUG

  // If this is an announcement, simply return.
  if (flags == DGR_FLAG_ANNOUNCEMENT) return;

  // If this is an ack message, save the message sequence if it's newwer than the last ack we
  // received from this member.
  if (flags == DGR_FLAG_ACK) {
    if (message_sequence > device_group_member->acked_sequence || device_group_member->acked_sequence - message_sequence < 64536) {
      device_group_member->acked_sequence = message_sequence;
    }
#ifdef DEVICE_GROUPS_DEBUG
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("<ack"));
#endif  // DEVICE_GROUPS_DEBUG
    return;
  }

  // Send an ack message to the sender.
  if (!(flags & DGR_FLAG_MORE_TO_COME)) {
    *(message_ptr - 2) = DGR_FLAG_ACK;
    *(message_ptr - 1) = 0;
    SendDeviceGroupPacket(remote_ip, packet, message_ptr - packet, PSTR("Ack"));
  }

  // If this is a status request message, then if the requestor didn't just ack our previous full
  // status update, send a full status update.
  if ((flags & DGR_FLAG_STATUS_REQUEST)) {
    if ((flags & DGR_FLAG_RESET) || device_group_member->acked_sequence != device_group->last_full_status_sequence) {
      _SendDeviceGroupMessage(device_group_index, DGR_MSGTYP_FULL_STATUS);
    }
#ifdef DEVICE_GROUPS_DEBUG
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("<status request"));
#endif  // DEVICE_GROUPS_DEBUG
    return;
  }

  // If we already processed this or a later message from this group member, ignore this message.
  if (message_sequence <= device_group_member->received_sequence) {
    if (message_sequence == device_group_member->received_sequence || device_group_member->received_sequence - message_sequence > 64536) {
#ifdef DEVICE_GROUPS_DEBUG
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("<old message"));
#endif  // DEVICE_GROUPS_DEBUG
      return;
    }
  }
  device_group_member->received_sequence = message_sequence;

  // Set the flag indicating we're currently processing a remote device message.
  // SendDeviceGroupMessage will not send messages while this flag is set.
  processing_remote_device_message = true;

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
  XdrvMailbox.topic = (char *)&device_group_index;
  if (flags & (DGR_FLAG_MORE_TO_COME | DGR_FLAG_DIRECT)) skip_light_fade = true;

  for (;;) {
    if (packet_length - (message_ptr - packet) < 1) goto badmsg;  // Malformed message
    item = *message_ptr++;
    if (!item) break; // Done

#ifdef DEVICE_GROUPS_DEBUG
    switch (item) {
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
        AddLog_P2(LOG_LEVEL_ERROR, PSTR("DGR: ********** Invalid item=%u received from device group %s member %s"), item, device_group->group_name, IPAddressToString(remote_ip));
    }
#endif  // DEVICE_GROUPS_DEBUG

    XdrvMailbox.command_code = item;
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

//#ifdef DEVICE_GROUPS_DEBUG
      AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("<%u=%u"), item, value);
//#endif  // DEVICE_GROUPS_DEBUG
      XdrvMailbox.payload = value;
    }
    else if (item <= DGR_ITEM_MAX_STRING) {
      value = strlen(message_ptr);
      if (value >= packet_length - (message_ptr - packet)) goto badmsg;  // Malformed message
//#ifdef DEVICE_GROUPS_DEBUG
      AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("<%u='%s'"), item, message_ptr);
//#endif  // DEVICE_GROUPS_DEBUG
      XdrvMailbox.data_len = value;
      XdrvMailbox.data = message_ptr;
      message_ptr += value + 1;
    }
    else {
      switch (item) {
        case DGR_ITEM_LIGHT_CHANNELS:
//#ifdef DEVICE_GROUPS_DEBUG
          AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("<%u=%u,%u,%u,%u,%u"), item, *message_ptr, *(message_ptr + 1), *(message_ptr + 2), *(message_ptr + 3), *(message_ptr + 4));
//#endif  // DEVICE_GROUPS_DEBUG
          XdrvMailbox.data = message_ptr;
          message_ptr += 5;
          break;
      }
    }

    if (DeviceGroupItemShared(true, item)) {
      if (item == DGR_ITEM_POWER) {
        if (device_group->local) {
          uint8_t mask_devices = value >> 24;
          if (mask_devices > devices_present) mask_devices = devices_present;
          for (uint32_t i = 0; i < devices_present; i++) {
            uint32_t mask = 1 << i;
            bool on = (value & mask);
            if (on != (power & mask)) ExecuteCommandPower(i + 1, (on ? POWER_ON : POWER_OFF), SRC_REMOTE);
          }
        }
      }
      else if (item == DGR_ITEM_EVENT) {
        CmndEvent();
      }
      XdrvCall(FUNC_DEVICE_GROUP_ITEM);
    }
  }

  XdrvMailbox.command_code = DGR_ITEM_EOL;
  XdrvCall(FUNC_DEVICE_GROUP_ITEM);
  skip_light_fade = false;

  processing_remote_device_message = false;
#ifdef DEVICE_GROUPS_DEBUG
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("<processed"));
#endif  // DEVICE_GROUPS_DEBUG
  return;

badmsg:
  AddLog_P2(LOG_LEVEL_ERROR, PSTR("DGR: Malformed message received from %s"), IPAddressToString(remote_ip));
#ifdef DEVICE_GROUPS_DEBUG
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("packet_length=%u, offset=%u"), packet_length, message_ptr - packet);
#endif  // DEVICE_GROUPS_DEBUG
  processing_remote_device_message = false;
}

void DeviceGroupStatus(uint8_t device_group_index)
{
  if (Settings.flag4.device_groups_enabled && device_group_index < device_group_count) {
    char buffer[1024];
    int member_count = 0;
    struct device_group * device_group = &device_groups[device_group_index];
    buffer[0] = buffer[1] = 0;
    for (struct device_group_member * device_group_member = device_group->device_group_members; device_group_member; device_group_member = device_group_member->flink) {
      snprintf(buffer, sizeof(buffer), PSTR("%s,{\"IPAddress\":\"%s\",\"ResendCount\":%u,\"LastRcvdSeq\":%u,\"LastAckedSeq\":%u}"), buffer, IPAddressToString(device_group_member->ip_address), device_group_member->unicast_count, device_group_member->received_sequence, device_group_member->acked_sequence);
      member_count++;
    }
    Response_P(PSTR("{\"" D_CMND_DEVGROUPSTATUS "\":{\"Index\":%u,\"GroupName\":\"%s\",\"MessageSeq\":%u,\"MemberCount\":%d,\"Members\":[%s]}"), device_group_index, device_group->group_name, outgoing_sequence, member_count, &buffer[1]);
  }
}

void DeviceGroupsLoop(void)
{
  if (!Settings.flag4.device_groups_enabled) return;
  if (udp_connected) {
    uint32_t now = millis();

    // If UDP was not connected before, (re)initialize.
    if (!udp_was_connected) {
      udp_was_connected = true;

      if (!device_groups_initialized) DeviceGroupsInit();
      if (device_groups_initialization_failed) return;

      // Load the status request message for all device groups. This message will be multicast 10
      // times at 200ms intervals.
      next_check_time = now + 3000;
      for (uint32_t device_group_index = 0; device_group_index < device_group_count; device_group_index++) {
        device_group * device_group = &device_groups[device_group_index];
        device_group->message_length = BeginDeviceGroupMessage(device_group, DGR_FLAG_RESET | DGR_FLAG_STATUS_REQUEST) - device_group->message;
        device_group->initial_status_requests_remaining = 10;
        device_group->next_ack_check_time = next_check_time;
      }
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("DGR: (Re)discovering device groups"));
    }

    if (device_groups_initialization_failed) return;

    // If it's time to check on things, iterate through the device groups.
    if (next_check_time <= now) {
#ifdef DEVICE_GROUPS_DEBUG
AddLog_P2(LOG_LEVEL_DEBUG, PSTR("DGR: Ckecking next_check_time=%u, now=%u"), next_check_time, now);
#endif  // DEVICE_GROUPS_DEBUG
      next_check_time = now + DGR_ANNOUNCEMENT_INTERVAL * 2;

      for (uint32_t device_group_index = 0; device_group_index < device_group_count; device_group_index++) {
        device_group * device_group = &device_groups[device_group_index];

        // If we're still waiting for acks to the last update from this device group, ...
        if (device_group->next_ack_check_time) {

          // If it's time to check for acks, ...
          if (device_group->next_ack_check_time <= now) {

            // If we're still sending the initial status request message, send it.
            if (device_group->initial_status_requests_remaining) {
              if (--device_group->initial_status_requests_remaining) {
#ifdef DEVICE_GROUPS_DEBUG
              AddLog_P2(LOG_LEVEL_DEBUG, PSTR("DGR: Sending initial status request for group %s"), device_group->group_name);
#endif  // DEVICE_GROUPS_DEBUG
                SendDeviceGroupPacket(IPAddress(0,0,0,0), device_group->message, device_group->message_length, PSTR("Initial"));
                device_group->message[device_group->message_header_length + 2] = DGR_FLAG_STATUS_REQUEST; // The reset flag is on only for the first packet - turn it off now
                device_group->next_ack_check_time = now + 200;
              }

              // If we've sent the initial status request message the set number of times, send our
              // status to all the members.
              else {
                device_group->next_ack_check_time = 0;
                _SendDeviceGroupMessage(device_group_index, DGR_MSGTYP_FULL_STATUS);
              }
            }

            // If we're done initializing, iterate through the group memebers, ...
            else {
#ifdef DEVICE_GROUPS_DEBUG
              AddLog_P2(LOG_LEVEL_DEBUG, PSTR("DGR: Checking for ack's"));
#endif  // DEVICE_GROUPS_DEBUG
              bool acked = true;
              struct device_group_member ** flink = &device_group->device_group_members;
              struct device_group_member * device_group_member;
              while ((device_group_member = *flink)) {

                // If we have not received an ack to our last message from this member, ...
                if (device_group_member->acked_sequence != outgoing_sequence) {

                  // If we haven't receive an ack from this member in DGR_MEMBER_TIMEOUT ms, assume
                  // they're offline and remove them from the group.
                  if (device_group->member_timeout_time < now) {
                    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("DGR: Removing member %s"), IPAddressToString(device_group_member->ip_address));
                    *flink = device_group_member->flink;
                    free(device_group_member);
                  }

                  // Otherwise, unicast the last message directly to this member.
                  else {
#ifdef DEVICE_GROUPS_DEBUG
                    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("DGR: Sending %u-byte device group %s packet via unicast to %s, sequence %u, last message acked=%u"), device_group->message_length, device_group->group_name, IPAddressToString(device_group_member->ip_address), outgoing_sequence, device_group_member->acked_sequence);
#endif  // DEVICE_GROUPS_DEBUG
                    SendDeviceGroupPacket(device_group_member->ip_address, device_group->message, device_group->message_length, PSTR("Unicast"));
                    device_group_member->unicast_count++;
                    acked = false;
                    flink = &device_group_member->flink;
                  }
                }
                else {
                  flink = &device_group_member->flink;
                }
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
        // announcement ourself to any members that have somehow not heard about us. We send it at
        // the announcement interval plus a random number of milliseconds so that even if all the
        // devices booted at the same time, they don't all multicast their announcements at the same
        // time.
#ifdef DEVICE_GROUPS_DEBUG
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("DGR: next_announcement_time=%u, now=%u"), device_group->next_announcement_time, now);
#endif  // DEVICE_GROUPS_DEBUG
        if (device_group->next_announcement_time <= now) {
#ifdef DEVICE_GROUPS_DEBUG
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("DGR: Sending device group %s announcement"), device_group->group_name);
#endif  // DEVICE_GROUPS_DEBUG
          SendDeviceGroupPacket(IPAddress(0,0,0,0), device_group->message, BeginDeviceGroupMessage(device_group, DGR_FLAG_ANNOUNCEMENT, true) - device_group->message, PSTR("Announcement"));
          device_group->next_announcement_time = now + DGR_ANNOUNCEMENT_INTERVAL + random(10000);
        }
        if (device_group->next_announcement_time < next_check_time) next_check_time = device_group->next_announcement_time;
      }
    }
  }
  else {
    udp_was_connected = false;
  }
}

#endif  // USE_DEVICE_GROUPS
