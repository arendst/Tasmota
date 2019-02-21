/*
* Copyright (c) 2014, Stephen Robinson
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
* 3. Neither the name of the copyright holder nor the names of its
* contributors may be used to endorse or promote products derived
* from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*/

#include <string.h>
#include "mqtt_msg.h"
#include "mqtt_config.h"
#define MQTT_MAX_FIXED_HEADER_SIZE 3

enum mqtt_connect_flag
{
  MQTT_CONNECT_FLAG_USERNAME = 1 << 7,
  MQTT_CONNECT_FLAG_PASSWORD = 1 << 6,
  MQTT_CONNECT_FLAG_WILL_RETAIN = 1 << 5,
  MQTT_CONNECT_FLAG_WILL = 1 << 2,
  MQTT_CONNECT_FLAG_CLEAN_SESSION = 1 << 1
};

struct __attribute((__packed__)) mqtt_connect_variable_header
{
  uint8_t lengthMsb;
  uint8_t lengthLsb;
#if defined(PROTOCOL_NAMEv31)
  uint8_t magic[6];
#elif defined(PROTOCOL_NAMEv311)
  uint8_t magic[4];
#else
#error "Please define protocol name"
#endif
  uint8_t version;
  uint8_t flags;
  uint8_t keepaliveMsb;
  uint8_t keepaliveLsb;
};

static int ICACHE_FLASH_ATTR append_string(mqtt_connection_t* connection, const char* string, int len)
{
  if (connection->message.length + len + 2 > connection->buffer_length)
    return -1;

  connection->buffer[connection->message.length++] = len >> 8;
  connection->buffer[connection->message.length++] = len & 0xff;
  memcpy(connection->buffer + connection->message.length, string, len);
  connection->message.length += len;

  return len + 2;
}

static uint16_t ICACHE_FLASH_ATTR append_message_id(mqtt_connection_t* connection, uint16_t message_id)
{
  // If message_id is zero then we should assign one, otherwise
  // we'll use the one supplied by the caller
  while (message_id == 0)
    message_id = ++connection->message_id;

  if (connection->message.length + 2 > connection->buffer_length)
    return 0;

  connection->buffer[connection->message.length++] = message_id >> 8;
  connection->buffer[connection->message.length++] = message_id & 0xff;

  return message_id;
}

static int ICACHE_FLASH_ATTR init_message(mqtt_connection_t* connection)
{
  connection->message.length = MQTT_MAX_FIXED_HEADER_SIZE;
  return MQTT_MAX_FIXED_HEADER_SIZE;
}

static mqtt_message_t* ICACHE_FLASH_ATTR fail_message(mqtt_connection_t* connection)
{
  connection->message.data = connection->buffer;
  connection->message.length = 0;
  return &connection->message;
}

static mqtt_message_t* ICACHE_FLASH_ATTR fini_message(mqtt_connection_t* connection, int type, int dup, int qos, int retain)
{
  int remaining_length = connection->message.length - MQTT_MAX_FIXED_HEADER_SIZE;

  if (remaining_length > 127)
  {
    connection->buffer[0] = ((type & 0x0f) << 4) | ((dup & 1) << 3) | ((qos & 3) << 1) | (retain & 1);
    connection->buffer[1] = 0x80 | (remaining_length % 128);
    connection->buffer[2] = remaining_length / 128;
    connection->message.length = remaining_length + 3;
    connection->message.data = connection->buffer;
  }
  else
  {
    connection->buffer[1] = ((type & 0x0f) << 4) | ((dup & 1) << 3) | ((qos & 3) << 1) | (retain & 1);
    connection->buffer[2] = remaining_length;
    connection->message.length = remaining_length + 2;
    connection->message.data = connection->buffer + 1;
  }

  return &connection->message;
}

void ICACHE_FLASH_ATTR mqtt_msg_init(mqtt_connection_t* connection, uint8_t* buffer, uint16_t buffer_length)
{
  memset(connection, 0, sizeof(mqtt_connection_t));
  connection->buffer = buffer;
  connection->buffer_length = buffer_length;
}

int ICACHE_FLASH_ATTR mqtt_get_total_length(uint8_t* buffer, uint16_t length)
{
  int i;
  int totlen = 0;

  for (i = 1; i < length; ++i)
  {
    totlen += (buffer[i] & 0x7f) << (7 * (i - 1));
    if ((buffer[i] & 0x80) == 0)
    {
      ++i;
      break;
    }
  }
  totlen += i;

  return totlen;
}

const char* ICACHE_FLASH_ATTR mqtt_get_publish_topic(uint8_t* buffer, uint16_t* length)
{
  int i;
  int totlen = 0;
  int topiclen;

  for (i = 1; i < *length; ++i)
  {
    totlen += (buffer[i] & 0x7f) << (7 * (i - 1));
    if ((buffer[i] & 0x80) == 0)
    {
      ++i;
      break;
    }
  }
  totlen += i;

  if (i + 2 >= *length)
    return NULL;
  topiclen = buffer[i++] << 8;
  topiclen |= buffer[i++];

  if (i + topiclen > *length)
    return NULL;

  *length = topiclen;
  return (const char*)(buffer + i);
}

const char* ICACHE_FLASH_ATTR mqtt_get_publish_data(uint8_t* buffer, uint16_t* length)
{
  int i;
  int totlen = 0;
  int topiclen;
  int blength = *length;
  *length = 0;

  for (i = 1; i < blength; ++i)
  {
    totlen += (buffer[i] & 0x7f) << (7 * (i - 1));
    if ((buffer[i] & 0x80) == 0)
    {
      ++i;
      break;
    }
  }
  totlen += i;

  if (i + 2 >= blength)
    return NULL;
  topiclen = buffer[i++] << 8;
  topiclen |= buffer[i++];

  if (i + topiclen >= blength)
    return NULL;

  i += topiclen;

  if (mqtt_get_qos(buffer) > 0)
  {
    if (i + 2 >= blength)
      return NULL;
    i += 2;
  }

  if (totlen < i)
    return NULL;

  if (totlen <= blength)
    *length = totlen - i;
  else
    *length = blength - i;
  return (const char*)(buffer + i);
}

uint16_t ICACHE_FLASH_ATTR mqtt_get_id(uint8_t* buffer, uint16_t length)
{
  if (length < 1)
    return 0;

  switch (mqtt_get_type(buffer))
  {
    case MQTT_MSG_TYPE_PUBLISH:
      {
        int i;
        int topiclen;

        for (i = 1; i < length; ++i)
        {
          if ((buffer[i] & 0x80) == 0)
          {
            ++i;
            break;
          }
        }

        if (i + 2 >= length)
          return 0;
        topiclen = buffer[i++] << 8;
        topiclen |= buffer[i++];

        if (i + topiclen >= length)
          return 0;
        i += topiclen;

        if (mqtt_get_qos(buffer) > 0)
        {
          if (i + 2 >= length)
            return 0;
          //i += 2;
        } else {
          return 0;
        }

        return (buffer[i] << 8) | buffer[i + 1];
      }
    case MQTT_MSG_TYPE_PUBACK:
    case MQTT_MSG_TYPE_PUBREC:
    case MQTT_MSG_TYPE_PUBREL:
    case MQTT_MSG_TYPE_PUBCOMP:
    case MQTT_MSG_TYPE_SUBACK:
    case MQTT_MSG_TYPE_UNSUBACK:
    case MQTT_MSG_TYPE_SUBSCRIBE:
      {
        // This requires the remaining length to be encoded in 1 byte,
        // which it should be.
        if (length >= 4 && (buffer[1] & 0x80) == 0)
          return (buffer[2] << 8) | buffer[3];
        else
          return 0;
      }

    default:
      return 0;
  }
}

mqtt_message_t* ICACHE_FLASH_ATTR mqtt_msg_connect(mqtt_connection_t* connection, mqtt_connect_info_t* info)
{
  struct mqtt_connect_variable_header* variable_header;

  init_message(connection);

  if (connection->message.length + sizeof(*variable_header) > connection->buffer_length)
    return fail_message(connection);
  variable_header = (void*)(connection->buffer + connection->message.length);
  connection->message.length += sizeof(*variable_header);

  variable_header->lengthMsb = 0;
#if defined(PROTOCOL_NAMEv31)
  variable_header->lengthLsb = 6;
  memcpy(variable_header->magic, "MQIsdp", 6);
  variable_header->version = 3;
#elif defined(PROTOCOL_NAMEv311)
  variable_header->lengthLsb = 4;
  memcpy(variable_header->magic, "MQTT", 4);
  variable_header->version = 4;
#else
#error "Please define protocol name"
#endif

  variable_header->flags = 0;
  variable_header->keepaliveMsb = info->keepalive >> 8;
  variable_header->keepaliveLsb = info->keepalive & 0xff;

  if (info->clean_session)
    variable_header->flags |= MQTT_CONNECT_FLAG_CLEAN_SESSION;

  if (info->client_id == NULL)
  {
    /* Never allowed */
    return fail_message(connection);
  }
  else if (info->client_id[0] == '\0')
  {
#ifdef PROTOCOL_NAMEv311
    /* Allowed. Format 0 Length ID */
    append_string(connection, info->client_id, 2) ;
#else
    /* 0 Length not allowed */
    return fail_message(connection);
#endif
  }
  else
  {
    /* No 0 data and at least 1 long. Good to go. */
    if(append_string(connection, info->client_id, strlen(info->client_id)) < 0)
      return fail_message(connection);
  }

  if (info->will_topic != NULL && info->will_topic[0] != '\0')
  {
    if (append_string(connection, info->will_topic, strlen(info->will_topic)) < 0)
      return fail_message(connection);

    if (append_string(connection, info->will_message, strlen(info->will_message)) < 0)
      return fail_message(connection);

    variable_header->flags |= MQTT_CONNECT_FLAG_WILL;
    if (info->will_retain)
      variable_header->flags |= MQTT_CONNECT_FLAG_WILL_RETAIN;
    variable_header->flags |= (info->will_qos & 3) << 3;
  }

  if (info->username != NULL && info->username[0] != '\0')
  {
    if (append_string(connection, info->username, strlen(info->username)) < 0)
      return fail_message(connection);

    variable_header->flags |= MQTT_CONNECT_FLAG_USERNAME;
  }

  if (info->password != NULL && info->password[0] != '\0')
  {
    if (append_string(connection, info->password, strlen(info->password)) < 0)
      return fail_message(connection);

    variable_header->flags |= MQTT_CONNECT_FLAG_PASSWORD;
  }

  return fini_message(connection, MQTT_MSG_TYPE_CONNECT, 0, 0, 0);
}

mqtt_message_t* ICACHE_FLASH_ATTR mqtt_msg_publish(mqtt_connection_t* connection, const char* topic, const char* data, int data_length, int qos, int retain, uint16_t* message_id)
{
  init_message(connection);

  if (topic == NULL || topic[0] == '\0')
    return fail_message(connection);

  if (append_string(connection, topic, strlen(topic)) < 0)
    return fail_message(connection);

  if (qos > 0)
  {
    if ((*message_id = append_message_id(connection, 0)) == 0)
      return fail_message(connection);
  }
  else
    *message_id = 0;

  if (connection->message.length + data_length > connection->buffer_length)
    return fail_message(connection);
  memcpy(connection->buffer + connection->message.length, data, data_length);
  connection->message.length += data_length;

  return fini_message(connection, MQTT_MSG_TYPE_PUBLISH, 0, qos, retain);
}

mqtt_message_t* ICACHE_FLASH_ATTR mqtt_msg_puback(mqtt_connection_t* connection, uint16_t message_id)
{
  init_message(connection);
  if (append_message_id(connection, message_id) == 0)
    return fail_message(connection);
  return fini_message(connection, MQTT_MSG_TYPE_PUBACK, 0, 0, 0);
}

mqtt_message_t* ICACHE_FLASH_ATTR mqtt_msg_pubrec(mqtt_connection_t* connection, uint16_t message_id)
{
  init_message(connection);
  if (append_message_id(connection, message_id) == 0)
    return fail_message(connection);
  return fini_message(connection, MQTT_MSG_TYPE_PUBREC, 0, 0, 0);
}

mqtt_message_t* ICACHE_FLASH_ATTR mqtt_msg_pubrel(mqtt_connection_t* connection, uint16_t message_id)
{
  init_message(connection);
  if (append_message_id(connection, message_id) == 0)
    return fail_message(connection);
  return fini_message(connection, MQTT_MSG_TYPE_PUBREL, 0, 1, 0);
}

mqtt_message_t* ICACHE_FLASH_ATTR mqtt_msg_pubcomp(mqtt_connection_t* connection, uint16_t message_id)
{
  init_message(connection);
  if (append_message_id(connection, message_id) == 0)
    return fail_message(connection);
  return fini_message(connection, MQTT_MSG_TYPE_PUBCOMP, 0, 0, 0);
}

mqtt_message_t* ICACHE_FLASH_ATTR mqtt_msg_subscribe(mqtt_connection_t* connection, const char* topic, int qos, uint16_t* message_id)
{
  init_message(connection);

  if (topic == NULL || topic[0] == '\0')
    return fail_message(connection);

  if ((*message_id = append_message_id(connection, 0)) == 0)
    return fail_message(connection);

  if (append_string(connection, topic, strlen(topic)) < 0)
    return fail_message(connection);

  if (connection->message.length + 1 > connection->buffer_length)
    return fail_message(connection);
  connection->buffer[connection->message.length++] = qos;

  return fini_message(connection, MQTT_MSG_TYPE_SUBSCRIBE, 0, 1, 0);
}

mqtt_message_t* ICACHE_FLASH_ATTR mqtt_msg_unsubscribe(mqtt_connection_t* connection, const char* topic, uint16_t* message_id)
{
  init_message(connection);

  if (topic == NULL || topic[0] == '\0')
    return fail_message(connection);

  if ((*message_id = append_message_id(connection, 0)) == 0)
    return fail_message(connection);

  if (append_string(connection, topic, strlen(topic)) < 0)
    return fail_message(connection);

  return fini_message(connection, MQTT_MSG_TYPE_UNSUBSCRIBE, 0, 1, 0);
}

mqtt_message_t* ICACHE_FLASH_ATTR mqtt_msg_pingreq(mqtt_connection_t* connection)
{
  init_message(connection);
  return fini_message(connection, MQTT_MSG_TYPE_PINGREQ, 0, 0, 0);
}

mqtt_message_t* ICACHE_FLASH_ATTR mqtt_msg_pingresp(mqtt_connection_t* connection)
{
  init_message(connection);
  return fini_message(connection, MQTT_MSG_TYPE_PINGRESP, 0, 0, 0);
}

mqtt_message_t* ICACHE_FLASH_ATTR mqtt_msg_disconnect(mqtt_connection_t* connection)
{
  init_message(connection);
  return fini_message(connection, MQTT_MSG_TYPE_DISCONNECT, 0, 0, 0);
}
