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

#define DEVICE_GROUPS_DEBUG

extern bool udp_connected;

struct device_group_member {
  struct device_group_member * flink;
  IPAddress ip_address;
  uint32_t timeout_time;
  uint16_t received_sequence;
  uint16_t acked_sequence;
};

struct device_group {
  uint32_t next_ack_check_time;
  uint16_t last_full_status_sequence;
  uint16_t message_length;
  uint8_t message_header_length;
  uint8_t initial_status_requests_remaining;
  bool local;
  char group_name[TOPSZ];
  char message[128];
  uint8_t group_member_count;
  struct device_group_member * device_group_members;
};

struct device_group * device_groups;
uint16_t outgoing_sequence = 0;
bool device_groups_initialized = false;
bool device_groups_initialization_failed = false;
bool building_status_message = false;
bool processing_remote_device_message = false;
bool waiting_for_acks;
bool udp_was_connected = false;

void DeviceGroupsInit(void)
{
  // Initialize the device information for each device group. The group name is the MQTT group topic.
  device_groups = (struct device_group *)calloc(device_group_count, sizeof(struct device_group));
  if (device_groups == nullptr) {
    AddLog_P2(LOG_LEVEL_ERROR, PSTR("DGR: error allocating %u-element device group array"), device_group_count);
    device_groups_initialization_failed = true;
    return;
  }

  for (uint32_t device_group_index = 0; device_group_index < device_group_count; device_group_index++) {
    struct device_group * device_group = &device_groups[device_group_index];
    strcpy(device_group->group_name, SettingsText((device_group_index == 0 ? SET_MQTT_GRP_TOPIC : SET_MQTT_GRP_TOPIC2 + device_group_index - 1)));
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

// Return true if we're configured to share the specified item.
bool DeviceGroupItemShared(bool incoming, uint8_t item)
{
  uint8_t mask = 0;
  switch (item) {
    case DGR_ITEM_LIGHT_BRI:
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
    case DGR_ITEM_BRI_MIN:
      mask = DGR_SHARE_BRI_MIN;
      break;
  }
  return (!mask || ((incoming ? Settings.device_group_share_in : Settings.device_group_share_out) & mask));
}

void SendDeviceGroupPacket(IPAddress ip, char * packet, int len, const char * label)
{
  for (int attempt = 1; attempt <= 5; attempt++) {
    if (PortUdp.beginPacket(ip, 1900)) {
      PortUdp.write(packet, len);
      if (PortUdp.endPacket()) return;
    }
    delay(10);
  }
  AddLog_P2(LOG_LEVEL_ERROR, PSTR("DGR: error sending %s packet"), label);
}

void _SendDeviceGroupMessage(uint8_t device_group_index, DeviceGroupMessageType message_type, ...)
{
  // If device groups are not enabled, ignore this request.
  if (!Settings.flag4.device_groups_enabled) return;

  // If UDP is not set up, ignore this request.
  if (!udp_connected) return;

  // If we're currently processing a remote device message, ignore this request.
  if (processing_remote_device_message) return;

  // Get a pointer to the device information for this device.
  device_group * device_group = &device_groups[device_group_index];

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
    XdrvMailbox.command_code = DGR_ITEM_STATUS;
    XdrvCall(FUNC_DEVICE_GROUP_REQUEST);
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
    bool shared;
    uint8_t item;
    uint32_t value;
    uint8_t * value_ptr;
    va_list ap;

#ifdef DEVICE_GROUPS_DEBUG
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Building device group %s packet"), device_group->group_name);
#endif  // DEVICE_GROUPS_DEBUG
    uint16_t original_sequence = outgoing_sequence;
    if (!building_status_message && message_type != DGR_MSGTYP_PARTIAL_UPDATE && !++outgoing_sequence) outgoing_sequence = 1;
    *message_ptr++ = outgoing_sequence & 0xff;
    *message_ptr++ = outgoing_sequence >> 8;

    value = 0;
    if (message_type == DGR_MSGTYP_UPDATE_MORE_TO_COME)
      value |= DGR_FLAG_MORE_TO_COME;
    else if (message_type == DGR_MSGTYP_UPDATE_DIRECT)
      value |= DGR_FLAG_DIRECT;
#ifdef DEVICE_GROUPS_DEBUG
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR(">sequence=%u, flags=%u"), outgoing_sequence, value);
#endif  // DEVICE_GROUPS_DEBUG
    *message_ptr++ = value & 0xff;
    *message_ptr++ = value >> 8;
    char * first_item_ptr = message_ptr;

    // If we're still building this update or all group members haven't acknowledged the previous
    // update yet, update the message to include these new updates. First we need to rebuild the
    // previous update message to remove any items and their values that are included in this new
    // update.
    if (device_group->message_length) {
      uint8_t item_array[32];
      int item_index = 0;
      int kept_item_count = 0;

      // Build an array of all the items in this new update.
      va_start(ap, message_type);
      while ((item = va_arg(ap, int))) {
        item_array[item_index++] = item;
        if (item <= DGR_ITEM_MAX_32BIT)
          va_arg(ap, int);
        else if (item <= DGR_ITEM_MAX_STRING)
          va_arg(ap, char *);
        else {
          switch (item) {
            case DGR_ITEM_LIGHT_CHANNELS:
              va_arg(ap, uint8_t *) ;
              break;
          }
        }
      }
      va_end(ap);
      item_array[item_index] = 0;

      // Rebuild the previous update message, removing any items their values that are included in
      // this new update.
      char * previous_message_ptr = message_ptr;
      while (item = *previous_message_ptr++) {

        // Search for this item in the new update.
        for (item_index = 0; item_array[item_index]; item_index++) {
          if (item_array[item_index] == item) break;
        }

        // If this item was found in the new update skip over it and it's value.
        if (item_array[item_index]) {
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
          else if (item <= DGR_ITEM_MAX_STRING)
            previous_message_ptr += *previous_message_ptr++;
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
            *message_ptr++ = value = *previous_message_ptr++;
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
    va_start(ap, message_type);
    while ((item = va_arg(ap, int))) {
      shared = DeviceGroupItemShared(false, item);
      if (shared) *message_ptr++ = item;
      if (item <= DGR_ITEM_MAX_32BIT) {
        value = va_arg(ap, int);
        if (shared) {
#ifdef DEVICE_GROUPS_DEBUG
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR(">item=%u, value=%u"), item, value);
#endif  // DEVICE_GROUPS_DEBUG
          *message_ptr++ = value & 0xff;
          if (item > DGR_ITEM_MAX_8BIT) {
            value >>= 8;
            *message_ptr++ = value & 0xff;
          }
          if (item > DGR_ITEM_MAX_16BIT) {
            value >>= 8;
            *message_ptr++ = value & 0xff;
            *message_ptr++ = value >> 8;
          }
        }
      }
      else if (item <= DGR_ITEM_MAX_STRING) {
        value_ptr = va_arg(ap, uint8_t *);
        if (shared) {
          value = strlen((const char *)value_ptr);
#ifdef DEVICE_GROUPS_DEBUG
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR(">item=%u, value=%s"), item, value_ptr);
#endif  // DEVICE_GROUPS_DEBUG
          *message_ptr++ = value;
          memcpy(message_ptr, value_ptr, value);
          message_ptr += value;
        }
      }
      else {
        switch (item) {
          case DGR_ITEM_LIGHT_CHANNELS:
            value_ptr = va_arg(ap, uint8_t *);
            if (shared) {
#ifdef DEVICE_GROUPS_DEBUG
              AddLog_P2(LOG_LEVEL_DEBUG, PSTR(">item=%u, value=%u,%u,%u,%u,%u"), item, *value_ptr, *(value_ptr + 1), *(value_ptr + 2), *(value_ptr + 3), *(value_ptr + 4));
#endif  // DEVICE_GROUPS_DEBUG
              memmove(message_ptr, value_ptr, 5);
              message_ptr += 5;
            }
            break;
        }
      }
    }
    va_end(ap);

    // If there weren't any items added to the message, restore the outgoing message sequence and
    // return.
    if (message_ptr == first_item_ptr) {
      outgoing_sequence = original_sequence;
      return;
    }

    // Add the EOL item code and calculate the message length.
    *message_ptr++ = 0;
    device_group->message_length = message_ptr - device_group->message;

    // If there's going to be more items added to this message, return.
    if (building_status_message || message_type == DGR_MSGTYP_PARTIAL_UPDATE) return;
  }

  // Multicast the packet.
#ifdef DEVICE_GROUPS_DEBUG
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("DGR: sending %u-byte device group %s packet via multicast, sequence=%u"), device_group->message_length, device_group->group_name, device_group->message[device_group->message_header_length] | device_group->message[device_group->message_header_length + 1] << 8);
#endif  // DEVICE_GROUPS_DEBUG
  SendDeviceGroupPacket(IPAddress(239,255,255,250), device_group->message, device_group->message_length, PSTR("Multicast"));
  device_group->next_ack_check_time = millis() + 100;

  if (message_type == DGR_MSGTYP_UPDATE_MORE_TO_COME) {
    for (struct device_group_member * device_group_member = device_group->device_group_members; device_group_member != nullptr; device_group_member = device_group_member->flink) {
      device_group_member->acked_sequence = outgoing_sequence;
    }
  }
  else {
    waiting_for_acks = true;
  }
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
  uint32_t device_group_index = 0;
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
        AddLog_P2(LOG_LEVEL_ERROR, PSTR("DGR: error allocating device group member block"));
        return;
      }
#ifdef DEVICE_GROUPS_DEBUG
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("DGR: adding member %s (%p)"), IPAddressToString(remote_ip), device_group_member);
#endif  // DEVICE_GROUPS_DEBUG
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

  // If this is an ack message, save the message sequence if it's newwer than the last ack we
  // received from this member.
  if (flags == DGR_FLAG_ACK) {
    if (message_sequence > device_group_member->acked_sequence || device_group_member->acked_sequence - message_sequence < 64536) {
      device_group_member->acked_sequence = message_sequence;
    }
    device_group_member->timeout_time = 0;
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
  if (message_sequence < device_group_member->received_sequence && device_group_member->received_sequence - message_sequence > 64536) {
#ifdef DEVICE_GROUPS_DEBUG
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("<old message"));
#endif  // DEVICE_GROUPS_DEBUG
    return;
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
    uint32_t      index           Flags
    uint32_t      data_len        String item value length
    int32_t       payload         Integer item value
    char         *topic
    char         *data            Pointer to non-integer item value
    char         *command         nullptr
  */
  XdrvMailbox.command = nullptr;  // Indicates the source is a device group update
  XdrvMailbox.index = flags;
  light_fade = Settings.light_fade;
  if (flags & (DGR_FLAG_MORE_TO_COME | DGR_FLAG_DIRECT)) Settings.light_fade = false;

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
      case DGR_ITEM_BRI_MIN:
      case DGR_ITEM_BRI_PRESET_LOW:
      case DGR_ITEM_BRI_PRESET_HIGH:
      case DGR_ITEM_BRI_POWER_ON:
      case DGR_ITEM_POWER:
      case DGR_ITEM_LIGHT_CHANNELS:
        break;
      default:
        AddLog_P2(LOG_LEVEL_ERROR, PSTR("DGR: ********** invalid item=%u received from device group %s member %s"), item, device_group->group_name, IPAddressToString(remote_ip));
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
        }
      }
      else if (item == DGR_ITEM_LIGHT_FADE) {
        light_fade = value;
      }
#ifdef DEVICE_GROUPS_DEBUG
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("<item=%u, value=%u"), item, value);
#endif  // DEVICE_GROUPS_DEBUG
      XdrvMailbox.payload = value;
    }
    else if (item <= DGR_ITEM_MAX_STRING) {
      value = *message_ptr++;
      if (value >= packet_length - (message_ptr - packet)) goto badmsg;  // Malformed message
#ifdef DEVICE_GROUPS_DEBUG
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("<item=%u, value=%.*s"), item, value, message_ptr);
#endif  // DEVICE_GROUPS_DEBUG
      XdrvMailbox.data_len = value;
      XdrvMailbox.data = message_ptr;
      message_ptr += value;
    }
    else {
      switch (item) {
        case DGR_ITEM_LIGHT_CHANNELS:
#ifdef DEVICE_GROUPS_DEBUG
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("<item=%u, value=%u,%u,%u,%u,%u"), item, *message_ptr, *(message_ptr + 1), *(message_ptr + 2), *(message_ptr + 3), *(message_ptr + 4));
#endif  // DEVICE_GROUPS_DEBUG
          XdrvMailbox.data = message_ptr;
          message_ptr += 5;
          break;
      }
    }

    if (DeviceGroupItemShared(true, item)) {
      if (item == DGR_ITEM_POWER) {
        for (uint32_t i = 0; i < devices_present; i++) {
          uint32_t mask = 1 << i;
          bool on = (value & mask);
          if (on != (power & mask)) ExecuteCommandPower(i + 1, (on ? POWER_ON : POWER_OFF), SRC_REMOTE);
        }
      }
      else {
        XdrvCall(FUNC_DEVICE_GROUP_REQUEST);
      }
    }
  }

  XdrvMailbox.command_code = DGR_ITEM_EOL;
  XdrvCall(FUNC_DEVICE_GROUP_REQUEST);
  Settings.light_fade = light_fade;

  processing_remote_device_message = false;
#ifdef DEVICE_GROUPS_DEBUG
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("<processed"));
#endif  // DEVICE_GROUPS_DEBUG
  return;

badmsg:
  AddLog_P2(LOG_LEVEL_ERROR, PSTR("DGR: malformed message received from %s"), IPAddressToString(remote_ip));
#ifdef DEVICE_GROUPS_DEBUG
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("packet_length=%u, offset=%u"), packet_length, message_ptr - packet);
#endif  // DEVICE_GROUPS_DEBUG
  processing_remote_device_message = false;
}

void DeviceGroupsLoop(void)
{
  if (!Settings.flag4.device_groups_enabled) return;
  if (udp_connected) {
    if (!udp_was_connected) {
      udp_was_connected = true;

      if (!device_groups_initialized) DeviceGroupsInit();
      if (device_groups_initialization_failed) return;

      for (uint32_t device_group_index = 0; device_group_index < device_group_count; device_group_index++) {
        device_group * device_group = &device_groups[device_group_index];
        char * message_ptr = &device_group->message[device_group->message_header_length];
        if (!++outgoing_sequence) outgoing_sequence = 1;
        *message_ptr++ = outgoing_sequence & 0xff;
        *message_ptr++ = outgoing_sequence >> 8;
        *message_ptr++ = DGR_FLAG_RESET | DGR_FLAG_STATUS_REQUEST;
        *message_ptr++ = 0;
        device_group->message_length = message_ptr - device_group->message;
        device_group->initial_status_requests_remaining = 5;
        device_group->next_ack_check_time = millis() + 1000;
      }

      waiting_for_acks = true;
    }

    if (device_groups_initialization_failed) return;

    if (waiting_for_acks) {
      uint32_t now = millis();
      waiting_for_acks = false;
      for (uint32_t device_group_index = 0; device_group_index < device_group_count; device_group_index++) {
        device_group * device_group = &device_groups[device_group_index];
        if (device_group->next_ack_check_time) {
          if (device_group->next_ack_check_time <= now) {
            if (device_group->initial_status_requests_remaining) {
#ifdef DEVICE_GROUPS_DEBUG
              AddLog_P2(LOG_LEVEL_DEBUG, PSTR("DGR: sending initial status request for group %s"), device_group->group_name);
#endif  // DEVICE_GROUPS_DEBUG
              if (--device_group->initial_status_requests_remaining) {
                SendDeviceGroupPacket(IPAddress(239,255,255,250), device_group->message, device_group->message_length, PSTR("Initial"));
                device_group->message[device_group->message_header_length + 2] = DGR_FLAG_STATUS_REQUEST; // The reset flag is on only for the first packet - turn it off now
                device_group->next_ack_check_time = now + 200;
                waiting_for_acks = true;
              }
              else {
                device_group->next_ack_check_time = 0;
                _SendDeviceGroupMessage(device_group_index, DGR_MSGTYP_FULL_STATUS);
              }
            }
            else {
              bool acked = true;
              struct device_group_member ** flink = &device_group->device_group_members;
              struct device_group_member * device_group_member;
              while ((device_group_member = *flink)) {
                if (device_group_member->acked_sequence != outgoing_sequence) {

                  if (device_group_member->timeout_time && device_group_member->timeout_time < now) {
#ifdef DEVICE_GROUPS_DEBUG
                    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("DGR: removing member %s (%p)"), IPAddressToString(device_group_member->ip_address), device_group_member);
#endif  // DEVICE_GROUPS_DEBUG
                    *flink = device_group_member->flink;
                    free(device_group_member);
                  }
                  else {
#ifdef DEVICE_GROUPS_DEBUG
                    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("DGR: sending %u-byte device group %s packet via unicast to %s, sequence %u, last message acked=%u"), device_group->message_length, device_group->group_name, IPAddressToString(device_group_member->ip_address), outgoing_sequence, device_group_member->acked_sequence);
#endif  // DEVICE_GROUPS_DEBUG
                    SendDeviceGroupPacket(device_group_member->ip_address, device_group->message, device_group->message_length, PSTR("Unicast"));
                    if (!device_group_member->timeout_time) device_group_member->timeout_time = now + 15000;
                    acked = false;
                    flink = &device_group_member->flink;
                  }
                }
                else {
                  flink = &device_group_member->flink;
                }
              }
              if (acked) {
                device_group->next_ack_check_time = 0;
                device_group->message_length = 0;
              }
              else {
                device_group->next_ack_check_time = now + 500;
                waiting_for_acks = true;
              }
            }
          }
          else {
            waiting_for_acks = true;
          }
        }
      }
    }
  }
  else {
    udp_was_connected = false;
  }
}

#endif  // USE_DEVICE_GROUPS
