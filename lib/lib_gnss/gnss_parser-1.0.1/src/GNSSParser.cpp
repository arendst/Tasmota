
#include "GNSSParser.h"

uint32_t GNSSParser::CRC24Q_TABLE[256];

GNSSParser::GNSSParser()
{
    static bool initialized = false;
    if (!initialized)
    {
        generateCRC24QTable(CRC24Q_TABLE);
        initialized = true;
    }
}

void GNSSParser::generateCRC24QTable(uint32_t table[256])
{
    const uint32_t polynomial = 0x1864CFB;

    for (uint32_t i = 0; i < 256; i++)
    {
        uint32_t crc = i << 16;

        for (int j = 0; j < 8; j++)
        {
            if (crc & 0x800000)
            {
                crc = (crc << 1) ^ polynomial;
            }
            else
            {
                crc = crc << 1;
            }
        }

        table[i] = crc & 0xFFFFFF;
    }
}

uint32_t GNSSParser::calculateRTCM3CRC(const uint8_t *data, size_t length)
{
    uint32_t crc = 0;

    for (size_t i = 0; i < length; i++)
    {
        uint8_t index = ((crc >> 16) ^ data[i]) & 0xFF;
        crc = ((crc << 8) & 0xFFFFFF) ^ CRC24Q_TABLE[index];
    }

    return crc & 0xFFFFFF;
}

void GNSSParser::addMessageToQueue(Message::Type type, size_t start, size_t length)
{
    if (message_queue_.size() >= MAX_MESSAGES)
    {
        return;
    }

    if (message_queue_.empty())
    {
        earliest_queued_pos_ = start;
    }

    message_queue_.push({type, start, length});
}

bool GNSSParser::tryParseRTCM3(size_t start_pos, size_t available_bytes, ParseResult &result)
{
    if (available_bytes < 6)
    {
        result = {false, false, 0, "Insufficient bytes"};
        return false;
    }

    if (buffer_[start_pos] != 0xD3)
    {
        return false;
    }

    uint16_t msg_length = ((buffer_[(start_pos + 1) % BUFFER_SIZE] & 0x03) << 8) |
                          buffer_[(start_pos + 2) % BUFFER_SIZE];

    if (msg_length > 1023)
    {
        result = {false, true, 0, "Invalid length"};
        return true;
    }

    size_t total_length = msg_length + 6; // header(3) + payload + crc(3)

    if (available_bytes < total_length)
    {
        result = {true, false, total_length, "Message incomplete"};
        return true;
    }

    bool is_valid = validateRTCM3Message(start_pos, total_length);
    result = {is_valid, true, total_length, is_valid ? nullptr : "CRC validation failed"};
    return true;
}

bool GNSSParser::tryParseNMEA(size_t start_pos, size_t available_bytes, ParseResult &result)
{
    if (buffer_[start_pos] != '$' && buffer_[start_pos] != '!')
    {
        result = {false, false, 0, "No message end found"};
        return false;
    }

    // Look for end of message within max NMEA length
    // Spec says it should be 82, but I see valid NMEA sentences which are longer.
    const size_t max_nmea_length = 128;

    size_t search_length = std::min(available_bytes, max_nmea_length);
    size_t msg_length = 0;
    bool found_end = false;
    bool full_search = false;

    for (size_t i = 0; i < search_length; i++)
    {
        if (buffer_[(start_pos + i) % BUFFER_SIZE] == '\n')
        {
            msg_length = i + 1;
            found_end = true;
            break;
        }

        if (i == max_nmea_length - 1)
        {
            full_search = true;
        }
    }

    if (!found_end)
    {
        if (full_search)
            result = {false, true, 0, "No final \\n found"};
        else
            result = {false, false, 0, "No message end found"};
        return true;
    }

    bool is_valid = validateNMEAMessage(start_pos, msg_length);

    if (!is_valid)
    {
        uint8_t log_buffer[256];
        auto len = std::min(msg_length - 1, sizeof(log_buffer));
        for (int i = 0; i < len; i++)
            log_buffer[i] = buffer_[(start_pos + i) % BUFFER_SIZE];

        log_buffer[len] = 0;
        printf("Invalid message: %s\n", log_buffer);
    }

    result = {is_valid, true, msg_length, is_valid ? nullptr : "Checksum validation failed"};
    return true;
}

void GNSSParser::scanBuffer()
{
    size_t scan_pos = read_pos_;
    size_t remaining_bytes = bytes_available_;

    while (remaining_bytes >= 6)
    {
        ParseResult result;

        if (tryParseRTCM3(scan_pos, remaining_bytes, result))
        {
            if (result.valid && result.complete)
            {
                addMessageToQueue(Message::Type::RTCM3, scan_pos, result.length);
                scan_pos = (scan_pos + result.length) % BUFFER_SIZE;
                remaining_bytes -= result.length;
                continue;
            }
            else if (!result.complete)
            {
                // Wait for more bytes
                break;
            }
        }

        if (tryParseNMEA(scan_pos, remaining_bytes, result))
        {
            if (result.valid && result.complete)
            {
                addMessageToQueue(Message::Type::NMEA, scan_pos, result.length);
                scan_pos = (scan_pos + result.length) % BUFFER_SIZE;
                remaining_bytes -= result.length;
                continue;
            }
            else if (!result.complete)
            {
                // Wait for more bytes
                break;
            }
        }

        scan_pos = (scan_pos + 1) % BUFFER_SIZE;
        remaining_bytes--;
    }

    read_pos_ = scan_pos;
    bytes_available_ = remaining_bytes;
}

bool GNSSParser::encode(uint8_t byte)
{
    // First, calculate available space
    size_t available = available_write_space();
    if (available == 0)
    {
        return false; // No space available without overwriting queued data
    }

    buffer_[write_pos_] = byte;
    write_pos_ = (write_pos_ + 1) % BUFFER_SIZE;
    bytes_available_++;

    scanBuffer();
    return !message_queue_.empty();
}

bool GNSSParser::encode(const uint8_t *buffer, size_t length)
{
    if (length > BUFFER_SIZE)
    {
        return false; // Buffer too large to process
    }

    size_t available = available_write_space();
    if (length > available)
    {
        return false; // Not enough space without overwriting queued data
    }

    // Add each byte to the circular buffer
    for (size_t i = 0; i < length; i++)
    {
        buffer_[write_pos_] = buffer[i];
        write_pos_ = (write_pos_ + 1) % BUFFER_SIZE;
        bytes_available_++;
    }

    scanBuffer();
    return true;
}

bool GNSSParser::available() const
{
    return !message_queue_.empty();
}

GNSSParser::Message GNSSParser::getMessage()
{
    if (message_queue_.empty())
    {
        return {GNSSParser::Message::Type::UNKNOWN, nullptr, 0};
    }

    StoredMessage msg = message_queue_.front();
    message_queue_.pop();

    static uint8_t msg_buffer[BUFFER_SIZE];
    for (size_t i = 0; i < msg.length; i++)
    {
        size_t pos = (msg.start + i) % BUFFER_SIZE;
        msg_buffer[i] = buffer_[pos];
    }

    if (!message_queue_.empty())
        earliest_queued_pos_ = message_queue_.front().start;

    return {msg.type, msg_buffer, msg.length};
}

size_t GNSSParser::available_write_space() const
{
    if (message_queue_.empty())
    {
        return BUFFER_SIZE - bytes_available_;
    }
    else
    {
        return BUFFER_SIZE - ((write_pos_ + BUFFER_SIZE - earliest_queued_pos_) % BUFFER_SIZE);
    }
}

void GNSSParser::clear()
{
    while (!message_queue_.empty())
    {
        message_queue_.pop();
    }
    write_pos_ = 0;
    read_pos_ = 0;
    bytes_available_ = 0;
}

bool GNSSParser::validateRTCM3Message(size_t start, size_t length)
{
    if (length < 6)
        return false;

    uint16_t msg_length = ((buffer_[(start + 1) % BUFFER_SIZE] & 0x03) << 8) |
                          buffer_[(start + 2) % BUFFER_SIZE];

    uint8_t msg_data[1029]; // Max RTCM message size (1024) + header (3) + CRC (3)

    for (size_t i = 0; i < length - 3; i++)
    { // Exclude CRC bytes
        msg_data[i] = buffer_[(start + i) % BUFFER_SIZE];
    }

    uint32_t calculated_crc = calculateRTCM3CRC(msg_data, length - 3);

    uint32_t received_crc = (static_cast<uint32_t>(buffer_[(start + length - 1) % BUFFER_SIZE])) |
                            (static_cast<uint32_t>(buffer_[(start + length - 2) % BUFFER_SIZE]) << 8) |
                            (static_cast<uint32_t>(buffer_[(start + length - 3) % BUFFER_SIZE]) << 16);

    return calculated_crc == received_crc;
}

uint8_t GNSSParser::calculateNMEAChecksum(size_t start, size_t length)
{
    uint8_t checksum = 0;
    bool started = false;
    bool ended = false;

    for (size_t i = 0; i < length && !ended; i++)
    {
        size_t pos = (start + i) % BUFFER_SIZE;
        char c = buffer_[pos];

        if (c == '$' || c == '!')
        {
            started = true;
            continue;
        }

        if (c == '*')
        {
            ended = true;
            continue;
        }

        if (started && !ended)
        {
            checksum ^= c;
        }
    }

    return checksum;
}

bool GNSSParser::validateNMEAMessage(size_t start, size_t length)
{
    uint8_t calculated_checksum = calculateNMEAChecksum(start, length);

    char c1 = buffer_[(start + length - 4) % BUFFER_SIZE];
    char c2 = buffer_[(start + length - 3) % BUFFER_SIZE];

    uint8_t received_checksum =
        ((c1 >= '0' && c1 <= '9' ? c1 - '0' : c1 >= 'A' && c1 <= 'F' ? c1 - 'A' + 10
                                                                     : 0)
         << 4) |
        (c2 >= '0' && c2 <= '9' ? c2 - '0' : c2 >= 'A' && c2 <= 'F' ? c2 - 'A' + 10
                                                                    : 0);

    return calculated_checksum == received_checksum;
}