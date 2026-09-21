/*
  MqttCorpus.h - Systematic malformed MQTT 5.0 inbound-packet corpus.

  The corpus is fixture data, not a second parser: each entry is an exact packet
  represented by a sized vector through MqttPacket::raw(), alongside the outcome
  the client is expected to report. It is intentionally reusable by the Property
  22 test and by sanitizer-focused regressions.
*/

#ifndef TASMOTA_PUBSUB_TEST_MQTT_CORPUS_H
#define TASMOTA_PUBSUB_TEST_MQTT_CORPUS_H

#include <cstdint>
#include <string>
#include <vector>

#include "MqttPacket.h"

struct MqttCorpusEntry {
    std::string label;
    MqttPacket packet;
    MqttError expectedError;
    uint8_t expectedDisconnectReason;
};

// Build malformed and unsupported MQTT 5.0 inbound fixtures systematically from
// valid packet shapes. All products are sized vectors; no fixed fixture buffers are
// used, so the corpus itself cannot conceal an out-of-bounds access.
std::vector<MqttCorpusEntry> mqtt5MalformedCorpus();

#endif  // TASMOTA_PUBSUB_TEST_MQTT_CORPUS_H
