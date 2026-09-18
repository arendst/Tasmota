/*
  MockClient_test.cpp - Unit checks for the MockClient core (task 6.1).

  These verify the scriptable mock transport itself (not the library under
  test): inbound read/consume, exhaustion semantics, outbound capture, and
  connection / close tracking. They are tagged into the baseline suite so they
  run as part of the standard non-regression pass.
*/

#include <cstdint>
#include <vector>

#include "doctest.h"

#include "Client.h"
#include "IPAddress.h"
#include "MockClient.h"
#include "TestClock.h"

TEST_SUITE("baseline") {

    TEST_CASE("MockClient reads scripted inbound bytes in order and consumes them") {
        MockClient c;
        c.pushInbound({0x10, 0x20, 0x30});

        CHECK(c.available() == 3);
        CHECK(c.peek() == 0x10);   // peek does not consume
        CHECK(c.available() == 3);

        CHECK(c.read() == 0x10);
        CHECK(c.read() == 0x20);
        CHECK(c.available() == 1);
        CHECK(c.read() == 0x30);

        // Exhausted queue.
        CHECK(c.available() == 0);
        CHECK(c.read() == -1);
        CHECK(c.peek() == -1);
    }

    TEST_CASE("MockClient read(buf,size) consumes up to size and reports the count") {
        MockClient c;
        c.pushInbound({1, 2, 3, 4, 5});

        uint8_t buf[3] = {0, 0, 0};
        int n = c.read(buf, 3);
        CHECK(n == 3);
        CHECK(buf[0] == 1);
        CHECK(buf[1] == 2);
        CHECK(buf[2] == 3);
        CHECK(c.available() == 2);

        // Requesting more than remains returns only what is left.
        uint8_t rest[8] = {0};
        int m = c.read(rest, 8);
        CHECK(m == 2);
        CHECK(rest[0] == 4);
        CHECK(rest[1] == 5);
        CHECK(c.available() == 0);

        // Further reads on an empty queue yield nothing.
        CHECK(c.read(rest, 8) == 0);
    }

    TEST_CASE("MockClient pushInbound appends across multiple calls") {
        MockClient c;
        c.pushInbound({0xAA});
        c.pushInbound({0xBB, 0xCC});
        CHECK(c.available() == 3);
        CHECK(c.read() == 0xAA);
        CHECK(c.read() == 0xBB);
        CHECK(c.read() == 0xCC);
        CHECK(c.available() == 0);
    }

    TEST_CASE("MockClient clearInbound drops unconsumed bytes") {
        MockClient c;
        c.pushInbound({1, 2, 3});
        CHECK(c.read() == 1);
        c.clearInbound();
        CHECK(c.available() == 0);
        CHECK(c.read() == -1);
    }

    TEST_CASE("MockClient captures outbound writes (single and buffer)") {
        MockClient c;
        CHECK(c.write(uint8_t(0x30)) == 1);

        const uint8_t payload[] = {0x00, 0x03, 'a', 'b', 'c'};
        CHECK(c.write(payload, sizeof(payload)) == sizeof(payload));

        const std::vector<uint8_t> expected = {0x30, 0x00, 0x03, 'a', 'b', 'c'};
        CHECK(c.outbound() == expected);

        c.clearOutbound();
        CHECK(c.outbound().empty());
    }

    TEST_CASE("MockClient write(nullptr) records nothing") {
        MockClient c;
        CHECK(c.write(static_cast<const uint8_t*>(nullptr), 4) == 0);
        CHECK(c.outbound().empty());
    }

    TEST_CASE("MockClient connection state is settable and connect() records the endpoint") {
        MockClient c;
        CHECK(c.connected() == 0);
        CHECK(static_cast<bool>(c) == false);

        c.setConnected(true);
        CHECK(c.connected() == 1);
        CHECK(static_cast<bool>(c) == true);

        // Default connect result is success (1) and brings the socket up.
        MockClient d;
        CHECK(d.connectCalled() == false);
        CHECK(d.connect("broker.example", 1883) == 1);
        CHECK(d.connectCalled() == true);
        CHECK(d.lastHost() == "broker.example");
        CHECK(d.lastPort() == 1883);
        CHECK(d.connected() == 1);

        // A scripted failure result does not bring the socket up.
        MockClient e;
        e.setConnectResult(-2);
        IPAddress ip(192, 168, 1, 50);
        CHECK(e.connect(ip, 8883) == -2);
        CHECK(e.lastPort() == 8883);
        CHECK(e.lastIp() == ip);
        CHECK(e.connected() == 0);
    }

    TEST_CASE("MockClient tracks stop() and flush()") {
        MockClient c;
        c.setConnected(true);
        CHECK(c.stopCalled() == false);
        CHECK(c.flushCalled() == false);

        c.flush();
        CHECK(c.flushCalled() == true);
        CHECK(c.flushCount() == 1);

        c.stop();
        CHECK(c.stopCalled() == true);
        CHECK(c.stopCount() == 1);
        CHECK(c.connected() == 0);        // stop() clears the connection
        CHECK(static_cast<bool>(c) == false);

        c.stop();
        CHECK(c.stopCount() == 2);
    }

    TEST_CASE("MockClient is usable through a Client* base pointer") {
        MockClient c;
        Client* base = &c;
        c.pushInbound({0x42});
        CHECK(base->available() == 1);
        CHECK(base->read() == 0x42);
        CHECK(base->write(uint8_t(0x99)) == 1);
        CHECK(c.outbound().size() == 1);
        CHECK(c.outbound()[0] == 0x99);
    }

    // --- Fault injection (task 6.2) ----------------------------------------

    TEST_CASE("MockClient setWriteLimit truncates a single write to the limit") {
        MockClient c;
        c.setWriteLimit(3);

        const uint8_t payload[] = {'a', 'b', 'c', 'd', 'e', 'f'};
        // A single write is truncated: fewer than size, but >= 1.
        size_t n = c.write(payload, sizeof(payload));
        CHECK(n == 3);
        CHECK(n < sizeof(payload));
        CHECK(n >= 1);

        // Only the accepted prefix is recorded.
        const std::vector<uint8_t> expected = {'a', 'b', 'c'};
        CHECK(c.outbound() == expected);
    }

    TEST_CASE("MockClient write limit makes progress across repeated writes") {
        MockClient c;
        c.setWriteLimit(2);

        const uint8_t payload[] = {1, 2, 3, 4, 5};
        // Emulate a caller resending the unaccepted tail until fully written.
        size_t sent = 0;
        int guard = 0;
        while (sent < sizeof(payload) && guard++ < 100) {
            size_t n = c.write(payload + sent, sizeof(payload) - sent);
            CHECK(n >= 1);          // always makes progress
            CHECK(n <= 2);          // never exceeds the limit
            sent += n;
        }
        CHECK(sent == sizeof(payload));

        const std::vector<uint8_t> expected = {1, 2, 3, 4, 5};
        CHECK(c.outbound() == expected);
    }

    TEST_CASE("MockClient a write shorter than the limit is accepted whole") {
        MockClient c;
        c.setWriteLimit(8);
        const uint8_t payload[] = {0xDE, 0xAD};
        CHECK(c.write(payload, sizeof(payload)) == sizeof(payload));
        const std::vector<uint8_t> expected = {0xDE, 0xAD};
        CHECK(c.outbound() == expected);
    }

    TEST_CASE("MockClient trickle reveals bytes only as the virtual clock advances") {
        TestClock::instance().reset(0);

        MockClient c;
        c.pushInbound({10, 20, 30, 40});
        // Reveal one byte immediately, then one more per 100 virtual ms.
        c.setTrickle(1, 100);

        // Only the first byte is visible at t0.
        CHECK(c.available() == 1);
        CHECK(c.peek() == 10);

        // Time has not advanced: still just one byte.
        CHECK(c.available() == 1);

        // Advancing by less than msPerReveal does not reveal more.
        TestClock::instance().advance(50);
        CHECK(c.available() == 1);

        // Crossing a full step reveals the next byte.
        TestClock::instance().advance(50);   // total elapsed 100
        CHECK(c.available() == 2);

        TestClock::instance().advance(100);   // total elapsed 200
        CHECK(c.available() == 3);

        // Enough time reveals all scripted bytes and no more.
        TestClock::instance().advance(1000);
        CHECK(c.available() == 4);

        // Reads consume from the revealed prefix in order.
        CHECK(c.read() == 10);
        CHECK(c.read() == 20);
        CHECK(c.available() == 2);
    }

    TEST_CASE("MockClient trickle bytesPerReveal>1 reveals in chunks") {
        TestClock::instance().reset(0);

        MockClient c;
        c.pushInbound({1, 2, 3, 4, 5});
        c.setTrickle(2, 100);   // two bytes now, two more each 100 ms

        CHECK(c.available() == 2);
        TestClock::instance().advance(100);
        CHECK(c.available() == 4);
        TestClock::instance().advance(100);
        CHECK(c.available() == 5);   // saturates at the scripted total
    }
}
