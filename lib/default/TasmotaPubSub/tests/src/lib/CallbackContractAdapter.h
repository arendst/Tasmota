/*
  CallbackContractAdapter.h - Faithful host reproduction of the Tasmota
  `MqttDataHandler` callback contract for the TasmotaPubSub test system
  (task 8.1).

  Feature: tasmota-pubsub-tests

  When registered as the PubSubClient callback, on each inbound PUBLISH this
  adapter reproduces exactly what the real Tasmota driver does in
  `xdrv_02_9_mqtt.ino`: it writes a NUL byte at index `data_len` of the
  delivered payload buffer (`mqtt_data[data_len] = 0`). The delivered payload
  pointer points *into the library's own working buffer allocation*, so the NUL
  write lands relative to that allocation:

      payload == buffer + (llen + 3 + tl)
      length  == len - llen - 3 - tl
      &payload[length] == &buffer[len]

  For a well-behaved PUBLISH the library guarantees `len < bufferSize` (the F-01
  sentinel-byte guard rejects `total_packet >= bufferSize` and `idx >=
  bufferSize`), so `buffer[len]` is the reserved spare byte and the NUL write is
  in-bounds. If a packet ever slipped past the guard so that `len == bufferSize`,
  the write would hit `buffer[bufferSize]` — one byte past the allocation — and
  AddressSanitizer would trip. This is what makes the F-01 consumer-side
  boundary bug observable (Requirement 15), and why the write must target the
  delivered buffer rather than a private copy.

  In addition to the NUL write, the adapter copies the delivered payload into a
  recorded buffer and captures `(topic, payloadCopy, length)` for every
  invocation, so callback-dispatch baseline assertions (Requirements 11.1/11.2)
  can check the correct topic, payload bytes, and length.

  The build is compiled with `-DESP32`, so `MQTT_CALLBACK_SIGNATURE` resolves to
  `std::function<void(char*, uint8_t*, unsigned int)>`. `callback()` returns such
  a std::function bound to this instance, suitable for
  `PubSubClient::setCallback(adapter.callback())`.
*/

#ifndef TASMOTA_PUBSUB_TEST_CALLBACK_CONTRACT_ADAPTER_H
#define TASMOTA_PUBSUB_TEST_CALLBACK_CONTRACT_ADAPTER_H

#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>
#include <vector>

class CallbackContractAdapter {
public:
    // One recorded callback invocation. `payload` is a copy of the delivered
    // bytes captured BEFORE the NUL-termination write, and `length` is the
    // authoritative payload length passed by the library.
    struct Invocation {
        std::string          topic;    // NUL-terminated C-string from the library
        std::vector<uint8_t> payload;  // copy of the delivered payload (length bytes)
        unsigned int         length = 0;
    };

    CallbackContractAdapter();

    // The MqttDataHandler-contract callback. Records the invocation and then
    // performs the `payload[length] = 0` NUL-termination write into the
    // delivered buffer (unless the write has been disabled for a record-only
    // test). Matches the MQTT_CALLBACK_SIGNATURE argument order/types.
    void onMessage(char* topic, uint8_t* payload, unsigned int length);

    // Return a std::function bound to this instance, suitable for passing to
    // PubSubClient::setCallback(). Because the build defines ESP32, the callback
    // signature is std::function<void(char*, uint8_t*, unsigned int)>.
    std::function<void(char*, uint8_t*, unsigned int)> callback();

    // --- Recorded invocations ---------------------------------------------
    const std::vector<Invocation>& invocations() const { return _invocations; }
    std::size_t count() const { return _invocations.size(); }
    const Invocation& last() const;   // precondition: count() > 0
    void clear();

    // Toggle the NUL-termination write. Enabled by default (the faithful
    // contract). Disable to record deliveries without mutating the library
    // buffer (useful when a test wants to inspect the payload independently of
    // the boundary write).
    void setPerformNulWrite(bool enable) { _performNulWrite = enable; }
    bool performsNulWrite() const { return _performNulWrite; }

private:
    std::vector<Invocation> _invocations;
    bool                    _performNulWrite;
};

#endif  // TASMOTA_PUBSUB_TEST_CALLBACK_CONTRACT_ADAPTER_H
