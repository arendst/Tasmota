/*
  PubSubClient.h - Protocol profile selector for TasmotaPubSub.

  SPDX-License-Identifier: MIT
*/

#ifndef TASMOTA_PUBSUBCLIENT_H
#define TASMOTA_PUBSUBCLIENT_H

#include <stdint.h>

// Tasmota compiles this library as separate translation units. Keep the protocol
// selector in a source-visible dependency so user_config_override.h changes
// invalidate the selected implementation and keep it aligned with the firmware.
#ifdef TASMOTA
#include <tasmota_options.h>
#endif

#define MQTT_VERSION_3_1_1 4
#define MQTT_VERSION_5_0   5

#ifndef MQTT_VERSION
#define MQTT_VERSION MQTT_VERSION_3_1_1
#endif

#if (MQTT_VERSION != MQTT_VERSION_3_1_1) && \
    (MQTT_VERSION != MQTT_VERSION_5_0)
#error "MQTT_VERSION must be MQTT_VERSION_3_1_1 or MQTT_VERSION_5_0"
#endif

static constexpr uint8_t kMqttProtocolLevel = MQTT_VERSION;
static constexpr bool kMqtt5 = (MQTT_VERSION == MQTT_VERSION_5_0);

#if MQTT_VERSION == MQTT_VERSION_3_1_1
#include "PubSubClient_311.h"
#else
#include "PubSubClient_5.h"
#endif

#endif  // TASMOTA_PUBSUBCLIENT_H
