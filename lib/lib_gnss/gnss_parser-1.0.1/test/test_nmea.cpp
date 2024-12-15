#include <unity.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "GNSSParser.h"

const char *ONE_VALID_NMEA_MESSAGE = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\r\n";
const char *ANOHTER_VALID_NMEA_MESSAGE = "$GNRMC,140658.00,A,4430.39666339,N,02600.98986769,E,0.007,316.3,070125,6.1,E,M,S*5C\r\n";
const char *ONE_VALID_NMEA_MESSAGE_WITH_A_PREFIX = "$GPGGA$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\r\n";
const char *ONE_VALID_ONE_NOT_NMEA_MESSAGES = "$GNGGA,140656.00,4430.39666339,N,02600.98986769,E,7,27,1.0,89.7706,M,35.4899,M,,*42\r\n"
                                              "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\r\n";

const char *BULK_NMEA_MESSAGES = "$GNGGA,140656.00,4430.39666339,N,02600.98986769,E,7,17,1.0,89.7706,M,35.4899,M,,*42\r\n"
                                 "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\r\n"
                                 "$GNGSA,M,3,24,28,29,31,32,,,,,,,,1.5,1.0,1.1,1*3C\r\n"
                                 "$GNGSA,M,3,67,77,86,87,,,,,,,,,1.5,1.0,1.1,2*3B\r\n"
                                 "$GNGSA,M,3,13,19,21,,,,,,,,,,1.5,1.0,1.1,3*33\r\n"
                                 "$GNGSA,M,3,23,25,28,37,43,,,,,,,,1.5,1.0,1.1,4*32\r\n"
                                 "$GNRMC,140656.00,A,4430.39666339,N,02600.98986769,E,0.006,221.9,070125,6.1,E,M,S*5C\r\n"
                                 "$GPGSV,2,1,06,24,18,168,40,25,72,321,28,28,29,311,35,29,48,242,49,1*67\r\n"
                                 "$GPGSV,2,2,06,32,14,266,25,31,06,321,30,1*64\r\n"
                                 "$GPGSV,2,1,06,24,18,168,35,25,72,321,30,28,29,311,39,29,48,242,46,4*6A\r\n"
                                 "$GPGSV,2,2,06,32,14,266,31,31,06,321,34,4*60\r\n"
                                 "$GPGSV,1,1,03,24,18,168,29,28,29,311,38,32,14,266,30,8*59\r\n"
                                 "$GLGSV,1,1,04,86,75,146,31,87,30,198,34,67,06,296,33,77,33,319,31,1*79\r\n"
                                 "$GLGSV,1,1,01,86,75,146,32,3*45\r\n"
                                 "$GBGSV,2,1,05,37,14,309,43,25,55,142,34,43,67,195,48,23,66,298,33,1*7E\r\n"
                                 "$GBGSV,2,2,05,28,07,217,40,1*4E\r\n"
                                 "$GBGSV,2,1,05,37,14,309,40,25,55,142,36,43,67,195,46,23,66,298,31,3*71\r\n"
                                 "$GBGSV,2,2,05,28,07,217,36,3*4D\r\n"
                                 "$GBGSV,2,1,05,37,14,309,31,25,55,142,31,43,67,195,36,23,66,298,34,5*74\r\n"
                                 "$GBGSV,2,2,05,28,07,217,35,5*48\r\n"
                                 "$GBGSV,1,1,03,37,14,309,38,43,67,195,40,23,66,298,34,6*48\r\n"
                                 "$GBGSV,2,1,05,37,14,309,33,25,55,142,33,43,67,195,40,23,66,298,35,8*79\r\n"
                                 "$GBGSV,2,2,05,28,07,217,36,8*46\r\n"
                                 "$GAGSV,2,1,07,19,18,247,33,21,71,323,30,04,16,305,36,26,10,185,37,1*78\r\n"
                                 "$GAGSV,2,2,07,29,02,207,35,27,46,092,26,13,47,142,33,1*46\r\n"
                                 "$GAGSV,2,1,07,19,18,247,34,21,71,323,24,04,16,305,38,26,10,185,37,2*77\r\n"
                                 "$GAGSV,2,2,07,29,02,207,39,27,46,092,25,13,47,142,33,2*4A\r\n"
                                 "$GAGSV,2,1,05,19,18,247,29,21,71,323,29,26,10,185,31,29,02,207,34,7*72\r\n"
                                 "$GAGSV,2,2,05,13,47,142,38,7*4B\r\n";

const char *INVALID_CHECKSUM = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*XX\r\n";

void setUp(void)
{
    // Called before each test
}

void tearDown(void)
{
    // Called after each test
}

void process_nmea_messages(GNSSParser &parser, size_t &message_counter)
{
    while (parser.available())
    {
        auto msg = parser.getMessage();

        char nmea_message[128];
        uint8_t msg_length = std::min(sizeof(nmea_message) - 1, msg.length);
        strncpy(nmea_message, (char *)msg.data, msg_length);
        nmea_message[msg_length] = 0;

        TEST_ASSERT_EQUAL(GNSSParser::Message::Type::NMEA, msg.type);

        printf("Found message %d: %s", message_counter, nmea_message);
        message_counter++;
    }
}

void test_one_valid_nmea_message()
{
    GNSSParser parser;
    const uint8_t *data = (const uint8_t *)ONE_VALID_NMEA_MESSAGE;
    size_t length = strlen(ONE_VALID_NMEA_MESSAGE);
    size_t pos = 0;

    while (pos < length)
    {
        size_t available = parser.available_write_space();
        TEST_ASSERT_TRUE(available > 0); // Should never fill up with one message

        size_t to_write = std::min(available, length - pos);
        TEST_ASSERT_TRUE(parser.encode(data + pos, to_write));
        pos += to_write;
    }

    TEST_ASSERT_TRUE(parser.available());
    auto msg = parser.getMessage();
    TEST_ASSERT_EQUAL(GNSSParser::Message::Type::NMEA, msg.type);
    TEST_ASSERT_EQUAL(strlen(ONE_VALID_NMEA_MESSAGE), msg.length);
}

void test_another_valid_nmea_message()
{
    GNSSParser parser;
    const uint8_t *data = (const uint8_t *)ANOHTER_VALID_NMEA_MESSAGE;
    size_t length = strlen(ANOHTER_VALID_NMEA_MESSAGE);
    size_t pos = 0;

    while (pos < length)
    {
        size_t available = parser.available_write_space();
        TEST_ASSERT_TRUE(available > 0); // Should never fill up with one message

        size_t to_write = std::min(available, length - pos);
        TEST_ASSERT_TRUE(parser.encode(data + pos, to_write));
        pos += to_write;
    }

    TEST_ASSERT_TRUE(parser.available());
    auto msg = parser.getMessage();
    TEST_ASSERT_EQUAL(GNSSParser::Message::Type::NMEA, msg.type);
    TEST_ASSERT_EQUAL(strlen(ANOHTER_VALID_NMEA_MESSAGE), msg.length);
}

void test_one_valid_nmea_message_with_prefix()
{
    GNSSParser parser;
    const uint8_t *data = (const uint8_t *)ONE_VALID_NMEA_MESSAGE_WITH_A_PREFIX;
    size_t length = strlen(ONE_VALID_NMEA_MESSAGE_WITH_A_PREFIX);
    size_t pos = 0;

    while (pos < length)
    {
        size_t available = parser.available_write_space();
        TEST_ASSERT_TRUE(available > 0); // Should never fill up with one message

        size_t to_write = std::min(available, length - pos);
        TEST_ASSERT_TRUE(parser.encode(data + pos, to_write));
        pos += to_write;
    }

    TEST_ASSERT_TRUE(parser.available());
    auto msg = parser.getMessage();
    TEST_ASSERT_EQUAL(GNSSParser::Message::Type::NMEA, msg.type);
    TEST_ASSERT_EQUAL(strlen(ONE_VALID_NMEA_MESSAGE), msg.length);
}

void test_two_messages_one_valid_one_not()
{
    GNSSParser parser;
    size_t message_counter = 0;
    const uint8_t *data = (const uint8_t *)ONE_VALID_ONE_NOT_NMEA_MESSAGES;
    size_t length = strlen(ONE_VALID_ONE_NOT_NMEA_MESSAGES);
    size_t pos = 0;

    while (pos < length)
    {
        size_t available = parser.available_write_space();
        if (available == 0)
        {
            process_nmea_messages(parser, message_counter);
            continue;
        }

        size_t to_write = std::min(available, length - pos);
        TEST_ASSERT_TRUE(parser.encode(data + pos, to_write));
        pos += to_write;
    }

    // Process any remaining messages
    process_nmea_messages(parser, message_counter);

    printf("Messages found %d\n", message_counter);
    TEST_ASSERT_EQUAL(1, message_counter);
}

void test_invalid_checksum()
{
    GNSSParser parser;
    const uint8_t *data = (const uint8_t *)INVALID_CHECKSUM;
    size_t length = strlen(INVALID_CHECKSUM);
    size_t pos = 0;

    while (pos < length)
    {
        size_t available = parser.available_write_space();
        TEST_ASSERT_TRUE(available > 0);

        size_t to_write = std::min(available, length - pos);
        parser.encode(data + pos, to_write);
        pos += to_write;

        TEST_ASSERT_FALSE(parser.available());
    }
}

size_t countNewLines(const char *str, size_t maxLength)
{
    size_t count = 0;
    while (str && *str && maxLength--)
        count += (*str++ == '\n');
    return count;
}

void test_parse_nmea_sentences()
{
    GNSSParser parser;
    size_t message_counter = 0;
    const uint8_t *data = (const uint8_t *)BULK_NMEA_MESSAGES;
    size_t length = strlen(BULK_NMEA_MESSAGES);
    size_t pos = 0;

    while (pos < length)
    {
        size_t available = parser.available_write_space();
        if (available == 0)
        {
            process_nmea_messages(parser, message_counter);
            continue;
        }

        size_t to_write = std::min(available, length - pos);
        TEST_ASSERT_TRUE(parser.encode(data + pos, to_write));
        pos += to_write;
    }

    process_nmea_messages(parser, message_counter);

    printf("Messages found %d\n", message_counter);
    TEST_ASSERT_EQUAL(countNewLines(BULK_NMEA_MESSAGES, strlen(BULK_NMEA_MESSAGES) + 1),
                      message_counter);
}

void process_parser_messages(GNSSParser &parser, uint32_t &nmea_count, uint32_t &rtcm_count)
{
    while (parser.available())
    {
        auto msg = parser.getMessage();
        if (msg.type == GNSSParser::Message::Type::NMEA)
        {
            printf("NMEA [VALID]: ");
            fwrite(msg.data, 1, msg.length, stdout);
            printf("\n");
            nmea_count++;
        }
        else if (msg.type == GNSSParser::Message::Type::RTCM3)
        {
            // printf("RTCM3 [VALID] Length: %zu Data: ", msg.length);
            for (size_t j = 0; j < msg.length; j++)
            {
                // printf("%02X", msg.data[j]);
            }
            // printf("\n");
            rtcm_count++;
        }
    }
}

void test_parse_log_file(const char *filename, uint32_t expected_nmea_count,
                         uint32_t expected_rtcm_count)
{
    printf("\nTesting file: %s\n", filename);
    printf("Expected NMEA messages: %u, Expected RTCM messages: %u\n",
           expected_nmea_count, expected_rtcm_count);

    FILE *file = fopen(filename, "rb");
    TEST_ASSERT_NOT_NULL_MESSAGE(file, "Failed to open test file");

    GNSSParser parser;
    uint8_t buffer[16];
    size_t bytes_read;
    uint32_t nmea_count = 0;
    uint32_t rtcm_count = 0;
    uint32_t total_bytes_read = 0;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0)
    {
        total_bytes_read += bytes_read;
        size_t pos = 0;
        while (pos < bytes_read)
        {
            if (parser.available())
                process_parser_messages(parser, nmea_count, rtcm_count);

            TEST_ASSERT_FALSE(parser.available());
            size_t available = parser.available_write_space();
            TEST_ASSERT_GREATER_THAN(0, available);
            size_t to_write = std::min(available, bytes_read - pos);
            TEST_ASSERT_TRUE(parser.encode(buffer + pos, to_write));
            pos += to_write;
        }
    }

    for (int i = 0; i < sizeof(buffer) / sizeof(buffer[0]); i++)
        buffer[i] = 0;

    for (int i = 0; i < GNSSParser::BUFFER_SIZE / sizeof(buffer); i++)
    {
        if (parser.available())
            process_parser_messages(parser, nmea_count, rtcm_count);

        TEST_ASSERT_TRUE(parser.encode(buffer, sizeof(buffer)));
    }

    // Process any remaining messages
    process_parser_messages(parser, nmea_count, rtcm_count);

    fclose(file);

    printf("Results:\n");
    printf("Found NMEA messages: %u (expected %u)\n", nmea_count, expected_nmea_count);
    printf("Found RTCM messages: %u (expected %u)\n", rtcm_count, expected_rtcm_count);

    TEST_ASSERT_EQUAL_UINT32_MESSAGE(expected_nmea_count, nmea_count,
                                     "Incorrect number of valid NMEA messages");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(expected_rtcm_count, rtcm_count,
                                     "Incorrect number of valid RTCM messages");
}

void process_random_test_messages(GNSSParser &parser, size_t &messages_found, int test)
{
    while (parser.available())
    {
        auto msg = parser.getMessage();
        if (msg.type != GNSSParser::Message::UNKNOWN)
        {
            messages_found++;
            printf("WARNING: Found valid message in random data!\n");
            printf("Test iteration: %d\n", test);
            printf("Message type: %d, Length: %zu\n", msg.type, msg.length);
            printf("Message data: ");
            for (size_t j = 0; j < msg.length; j++)
            {
                printf("%02X", msg.data[j]);
            }
            printf("\n");
        }
    }
}

void test_random_data_parse()
{
    // Use fixed seed for reproducibility
    srand(12345);

    for (int test = 0; test < 1000; test++)
    {
        GNSSParser parser;
        uint8_t random_data[4096];
        size_t messages_found = 0;

        srand(time(NULL));

        // Generate random data
        for (size_t i = 0; i < sizeof(random_data); i++)
        {
            random_data[i] = rand() & 0xFF;
        }

        // Feed data in chunks based on available space
        size_t pos = 0;
        while (pos < sizeof(random_data))
        {
            size_t available = parser.available_write_space();
            if (available == 0)
            {
                process_random_test_messages(parser, messages_found, test);
                continue;
            }

            size_t to_write = std::min(available, sizeof(random_data) - pos);
            TEST_ASSERT_TRUE(parser.encode(random_data + pos, to_write));
            pos += to_write;
        }

        // Process any remaining messages
        process_random_test_messages(parser, messages_found, test);

        TEST_ASSERT_EQUAL_MESSAGE(0, messages_found,
                                  "Parser incorrectly identified valid messages in random data");
    }

    printf("Successfully completed 1000 iterations of random data testing\n");
}

void test_parse_log_file_5_2()
{
    test_parse_log_file("test/test-data/test-data-5-2.bin", 5, 2);
}

void test_parse_log_file_56_4()
{
    test_parse_log_file("test/test-data/test-data-56-5.bin", 56, 4);
}

void test_parse_log_file_656_43()
{
    test_parse_log_file("test/test-data/test-data-656-43.bin", 656, 43);
}

void test_parse_log_file_33816_2193()
{
    test_parse_log_file("test/test-data/test-data-33816-2193.bin", 33816, 2193);
}

void register_nmea_tests()
{
    RUN_TEST(test_one_valid_nmea_message);
    RUN_TEST(test_another_valid_nmea_message);
    RUN_TEST(test_one_valid_nmea_message_with_prefix);
    RUN_TEST(test_two_messages_one_valid_one_not);
    RUN_TEST(test_invalid_checksum);
    RUN_TEST(test_parse_nmea_sentences);
    RUN_TEST(test_random_data_parse);
    RUN_TEST(test_parse_log_file_5_2);
    RUN_TEST(test_parse_log_file_56_4);
    RUN_TEST(test_parse_log_file_656_43);
    RUN_TEST(test_parse_log_file_33816_2193);
}
