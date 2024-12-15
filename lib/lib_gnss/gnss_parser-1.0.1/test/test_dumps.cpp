#include <unity.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "GNSSParser.h"

static bool is_hex_digit(char c)
{
    return (c >= '0' && c <= '9') ||
           (c >= 'A' && c <= 'F') ||
           (c >= 'a' && c <= 'f');
}

static uint8_t hex_to_byte(char high, char low)
{
    uint8_t value = 0;

    // Convert high nibble
    if (high >= '0' && high <= '9')
        value = (high - '0') << 4;
    else if (high >= 'A' && high <= 'F')
        value = (high - 'A' + 10) << 4;
    else if (high >= 'a' && high <= 'f')
        value = (high - 'a' + 10) << 4;

    // Convert low nibble
    if (low >= '0' && low <= '9')
        value |= (low - '0');
    else if (low >= 'A' && low <= 'F')
        value |= (low - 'A' + 10);
    else if (low >= 'a' && low <= 'f')
        value |= (low - 'a' + 10);

    return value;
}

void test_parse_hex_dump(const char *filename)
{
    GNSSParser parser;
    uint32_t valid_messages = 0;
    uint32_t expected_messages = 0;
    uint32_t line_number = 0;

    FILE *file = fopen(filename, "r");
    TEST_ASSERT_NOT_NULL_MESSAGE(file, "Failed to open hex dump file");

    char line[1024];
    while (fgets(line, sizeof(line), file))
    {
        line_number++;

        char *hex_start = strstr(line, ": ");
        if (!hex_start)
        {
            continue;
        }
        hex_start += 2; // Move past ": "

        char *pipe = strchr(hex_start, '|');
        if (!pipe || (pipe - hex_start) < 3)
        {
            printf("Warning: Invalid line format at line %u: %s", line_number, line);
            continue;
        }

        uint8_t bytes[16];
        size_t byte_count = 0;
        const char *ptr = hex_start;

        while (ptr < pipe && byte_count < 16)
        {
            while (*ptr == ' ')
                ptr++;

            if (!is_hex_digit(ptr[0]))
            {
                break;
            }

            if (!is_hex_digit(ptr[0]) || !is_hex_digit(ptr[1]))
            {
                printf("Warning: Invalid hex digits at line %u: %s", line_number, line);
                break;
            }

            bytes[byte_count++] = hex_to_byte(ptr[0], ptr[1]);
            ptr += 2;
        }

        for (size_t i = 0; i < byte_count; i++)
        {
            if (bytes[i] == '$')
            {
                expected_messages++;
            }

            // Feed byte to parser
            parser.encode(bytes[i]);

            // Check for available messages
            while (parser.available())
            {
                auto msg = parser.getMessage();

                if (msg.type != GNSSParser::Message::UNKNOWN)
                {
                    valid_messages++;
                    // printf("Found valid message %d: ", valid_messages);
                    for (size_t j = 0; j < msg.length; j++)
                    {
                        printf("%c", msg.data[j]);
                    }
                }
            }
        }
    }

    fclose(file);

    printf("\nSummary:\n");
    printf("Expected messages ($ count): %d\n", expected_messages);
    printf("Valid messages parsed: %d\n", valid_messages);

    TEST_ASSERT_GREATER_THAN_MESSAGE(0, valid_messages,
                                     "No valid messages found in hex dump");
    TEST_ASSERT_LESS_OR_EQUAL(expected_messages, (int)((double)valid_messages * 0.99d));
}

void test_parse_hex_dump_1()
{
    test_parse_hex_dump("test/test-data/dump-test-1.txt");
}

void test_parse_hex_dump_2()
{
    test_parse_hex_dump("test/test-data/dump-test-2.txt");
}

void register_dump_tests()
{
    RUN_TEST(test_parse_hex_dump_1);
    RUN_TEST(test_parse_hex_dump_2);
}