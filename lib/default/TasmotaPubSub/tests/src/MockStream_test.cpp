/*
  MockStream_test.cpp - Unit checks for the MockStream test double.

  Verifies that bytes written to the stream (as the library does in stream-mode
  PUBLISH payload routing via `stream->write(digit)`) are captured in order,
  that both write overloads record correctly, and that the read side is inert.
*/

#include "doctest.h"

#include "MockStream.h"

TEST_SUITE("baseline") {

    TEST_CASE("MockStream records single-byte writes in order") {
        MockStream stream;

        // Simulate the library routing payload digits one byte at a time.
        CHECK(stream.write(static_cast<uint8_t>('h')) == 1);
        CHECK(stream.write(static_cast<uint8_t>('i')) == 1);
        CHECK(stream.write(static_cast<uint8_t>('!')) == 1);

        const std::vector<uint8_t>& out = stream.written();
        REQUIRE(out.size() == 3);
        CHECK(out[0] == 'h');
        CHECK(out[1] == 'i');
        CHECK(out[2] == '!');
    }

    TEST_CASE("MockStream records buffer writes in order and appends") {
        MockStream stream;

        const uint8_t first[] = {0x01, 0x02, 0x03};
        CHECK(stream.write(first, sizeof(first)) == sizeof(first));

        // A subsequent single-byte write appends after the buffered bytes.
        CHECK(stream.write(static_cast<uint8_t>(0x04)) == 1);

        const uint8_t second[] = {0x05, 0x06};
        CHECK(stream.write(second, sizeof(second)) == sizeof(second));

        const std::vector<uint8_t> expected = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
        CHECK(stream.written() == expected);
    }

    TEST_CASE("MockStream write(const char*) helper routes through the byte path") {
        MockStream stream;

        // Print::write(const char*) is a non-virtual helper; ensure it is not
        // hidden and still lands in the recorded buffer in order.
        CHECK(stream.write("ab") == 2);

        const std::vector<uint8_t> expected = {'a', 'b'};
        CHECK(stream.written() == expected);
    }

    TEST_CASE("MockStream clear() discards recorded bytes") {
        MockStream stream;
        stream.write(static_cast<uint8_t>(0xAA));
        REQUIRE(stream.written().size() == 1);

        stream.clear();
        CHECK(stream.written().empty());
    }

    TEST_CASE("MockStream read side is inert") {
        MockStream stream;
        CHECK(stream.available() == 0);
        CHECK(stream.read() == -1);
        CHECK(stream.peek() == -1);
    }

    TEST_CASE("MockStream is usable as a Stream& (library construction seam)") {
        MockStream stream;
        Stream& asStream = stream;

        // The library holds a `Stream*` and calls write(digit) through it.
        asStream.write(static_cast<uint8_t>('X'));

        REQUIRE(stream.written().size() == 1);
        CHECK(stream.written()[0] == 'X');
    }
}
