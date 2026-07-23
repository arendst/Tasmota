/*
  CallbackContractAdapter.cpp - Implementation of the Tasmota MqttDataHandler
  contract reproduction (task 8.1). See CallbackContractAdapter.h for the
  rationale behind writing the NUL terminator into the delivered buffer.
*/

#include "CallbackContractAdapter.h"

#include <cassert>

CallbackContractAdapter::CallbackContractAdapter()
    : _invocations(), _performNulWrite(true) {}

void CallbackContractAdapter::onMessage(char* topic, uint8_t* payload,
                                        unsigned int length) {
    // Record the invocation first: copy the delivered payload bytes exactly as
    // received, before any NUL-termination write mutates the buffer. The topic
    // is a C-string the library has already NUL-terminated.
    Invocation inv;
    inv.topic = (topic != nullptr) ? std::string(topic) : std::string();
    inv.length = length;
    if (payload != nullptr && length > 0) {
        inv.payload.assign(payload, payload + length);
    }
    _invocations.push_back(std::move(inv));

    // Reproduce the exact Tasmota driver contract: mqtt_data[data_len] = 0.
    // `payload` points into the library's working buffer, so this write lands
    // relative to the library allocation. For a well-formed packet the library
    // reserves the spare byte at buffer[len], so the write is in-bounds; an
    // exact-buffer packet that slipped past the guard would write one byte past
    // the allocation and trip AddressSanitizer (F-01).
    if (_performNulWrite && payload != nullptr) {
        payload[length] = 0;
    }
}

std::function<void(char*, uint8_t*, unsigned int)>
CallbackContractAdapter::callback() {
    return [this](char* topic, uint8_t* payload, unsigned int length) {
        this->onMessage(topic, payload, length);
    };
}

const CallbackContractAdapter::Invocation& CallbackContractAdapter::last() const {
    assert(!_invocations.empty() && "last() called with no recorded invocations");
    return _invocations.back();
}

void CallbackContractAdapter::clear() {
    _invocations.clear();
}
