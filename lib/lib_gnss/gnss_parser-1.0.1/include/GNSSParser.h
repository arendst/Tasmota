#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <queue>
#include <array>

#if defined(ARDUINO)
#include <Arduino.h>
#else
typedef uint8_t byte;
#define PROGMEM
#endif

class GNSSParser
{
public:
    static constexpr size_t BUFFER_SIZE = 4096;
    static constexpr size_t MAX_MESSAGES = 128;

    struct Message
    {
        enum Type
        {
            UNKNOWN,
            NMEA,
            RTCM3,
            INVALID
        };

        Type type;
        const uint8_t *data;
        size_t length;
    };

    struct ParseResult
    {
        bool valid;
        bool complete;
        size_t length;
        const char *error;
    };

    GNSSParser();
    ~GNSSParser() = default;

    bool encode(uint8_t byte);
    bool encode(const uint8_t *buffer, size_t length);
    bool available() const;
    size_t available_write_space() const;
    Message getMessage();
    void clear();

private:
    static uint32_t CRC24Q_TABLE[256];

    static void generateCRC24QTable(uint32_t table[256]);
    static uint32_t calculateRTCM3CRC(const uint8_t *data, size_t length);

    struct StoredMessage
    {
        Message::Type type;
        size_t start;
        size_t length;
        bool valid;
        const char *error;
    };

    std::array<uint8_t, BUFFER_SIZE> buffer_{};
    size_t write_pos_ = 0;
    size_t read_pos_ = 0;
    size_t bytes_available_ = 0;
    std::queue<StoredMessage> message_queue_;
    size_t earliest_queued_pos_ = 0;

    void addMessageToQueue(Message::Type type, size_t start, size_t length);
    void scanBuffer();
    bool validateRTCM3Message(size_t start, size_t length);
    uint8_t calculateNMEAChecksum(size_t start, size_t length);
    bool validateNMEAMessage(size_t start, size_t length);
    bool tryParseRTCM3(size_t start_pos, size_t available_bytes, ParseResult &result);
    bool tryParseNMEA(size_t start_pos, size_t available_bytes, ParseResult &result);
};