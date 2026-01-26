#define UNIT_TEST

#include <QuickEspNow.h>
#include <unity.h>


PeerListClass PeerList;

uint8_t macs[ESP_NOW_MAX_TOTAL_PEER_NUM+1][6] = {
    {0x00,0x01,0x02,0x03,0x04,0x01},
    {0x00,0x01,0x02,0x03,0x04,0x02},
    {0x00,0x01,0x02,0x03,0x04,0x03},
    {0x00,0x01,0x02,0x03,0x04,0x04},
    {0x00,0x01,0x02,0x03,0x04,0x05},
    {0x00,0x01,0x02,0x03,0x04,0x06},
    {0x00,0x01,0x02,0x03,0x04,0x07},
    {0x00,0x01,0x02,0x03,0x04,0x08},
    {0x00,0x01,0x02,0x03,0x04,0x09},
    {0x00,0x01,0x02,0x03,0x04,0x10},
    {0x00,0x01,0x02,0x03,0x04,0x11},
    {0x00,0x01,0x02,0x03,0x04,0x12},
    {0x00,0x01,0x02,0x03,0x04,0x13},
    {0x00,0x01,0x02,0x03,0x04,0x14},
    {0x00,0x01,0x02,0x03,0x04,0x15},
    {0x00,0x01,0x02,0x03,0x04,0x16},
    {0x00,0x01,0x02,0x03,0x04,0x17},
    {0x00,0x01,0x02,0x03,0x04,0x18},
    {0x00,0x01,0x02,0x03,0x04,0x19},
    {0x00,0x01,0x02,0x03,0x04,0x20},
    {0x00,0x01,0x02,0x03,0x04,0x21}
};


void setUp (void) {
    // set stuff up here
    Serial.begin (115200);
}

void tearDown (void) {
    // clean stuff up here
}

void test_add_one_peer () {
    uint8_t mac[6] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 };
    TEST_ASSERT_FALSE (PeerList.peer_exists (mac));
    TEST_ASSERT_TRUE (PeerList.add_peer (mac));
    TEST_ASSERT_TRUE (PeerList.peer_exists (mac));
    TEST_ASSERT_EQUAL (1, PeerList.get_peer_number ());
    TEST_ASSERT_TRUE (PeerList.delete_peer (mac));
    TEST_ASSERT_EQUAL (0, PeerList.get_peer_number ());
}

void test_add_existing_peer () {
    // Serial.println ("test_add_existing_peer");
    uint8_t mac[6] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 };
    TEST_ASSERT_FALSE (PeerList.peer_exists (mac));
    TEST_ASSERT_TRUE(PeerList.add_peer(mac));
    TEST_ASSERT_TRUE (PeerList.peer_exists (mac));
    TEST_ASSERT_EQUAL (1, PeerList.get_peer_number ());
    TEST_ASSERT_FALSE (PeerList.add_peer (mac));
    TEST_ASSERT_EQUAL (1, PeerList.get_peer_number ());
    TEST_ASSERT_TRUE (PeerList.delete_peer (mac));
    TEST_ASSERT_EQUAL (0, PeerList.get_peer_number ());
}

void test_add_max_peers () {
    // Serial.println ("test_add_max_peers");
    for (uint8_t i = 0; i < ESP_NOW_MAX_TOTAL_PEER_NUM; i++) {
        TEST_ASSERT_FALSE (PeerList.peer_exists (macs[i]));
        TEST_ASSERT_TRUE(PeerList.add_peer(macs[i]));
        TEST_ASSERT_TRUE (PeerList.peer_exists (macs[i]));
        TEST_ASSERT_EQUAL (i + 1, PeerList.get_peer_number ());
    }
    // PeerList.dump_peer_list ();
    for (uint8_t i = 0; i < ESP_NOW_MAX_TOTAL_PEER_NUM; i++) {
        TEST_ASSERT_TRUE (PeerList.delete_peer (macs[i]));
        TEST_ASSERT_EQUAL (ESP_NOW_MAX_TOTAL_PEER_NUM - i - 1, PeerList.get_peer_number ());
    }
    // PeerList.dump_peer_list ();
}

void test_add_max_peers_plus_1 () {
    // Serial.println ("test_add_max_peers_plus_1");
    for (uint8_t i = 0; i < ESP_NOW_MAX_TOTAL_PEER_NUM; i++) {
        TEST_ASSERT_FALSE (PeerList.peer_exists (macs[i]));
        TEST_ASSERT_TRUE (PeerList.add_peer (macs[i]));
        TEST_ASSERT_TRUE (PeerList.peer_exists (macs[i]));
        TEST_ASSERT_EQUAL (i + 1, PeerList.get_peer_number ());
    }
    // PeerList.dump_peer_list ();
    TEST_ASSERT_FALSE (PeerList.peer_exists (macs[ESP_NOW_MAX_TOTAL_PEER_NUM]));
    TEST_ASSERT_FALSE (PeerList.add_peer (macs[ESP_NOW_MAX_TOTAL_PEER_NUM]));
    // PeerList.dump_peer_list ();
    TEST_ASSERT_FALSE (PeerList.peer_exists (macs[ESP_NOW_MAX_TOTAL_PEER_NUM]));
    TEST_ASSERT_EQUAL (ESP_NOW_MAX_TOTAL_PEER_NUM, PeerList.get_peer_number ());

    for (uint8_t i = 0; i < ESP_NOW_MAX_TOTAL_PEER_NUM; i++) {
        TEST_ASSERT_TRUE (PeerList.delete_peer ());
        TEST_ASSERT_EQUAL (ESP_NOW_MAX_TOTAL_PEER_NUM - i - 1, PeerList.get_peer_number ());
    }
    // PeerList.dump_peer_list ();
}

void process () {
    UNITY_BEGIN ();
    RUN_TEST (test_add_one_peer);
    RUN_TEST (test_add_existing_peer);
    RUN_TEST (test_add_max_peers);
    RUN_TEST (test_add_max_peers_plus_1);
    UNITY_END ();
}

#ifdef ARDUINO

#include <Arduino.h>
void setup () {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay (2000);

    process ();
}

void loop () {
    delay (1);
}

#else

int main (int argc, char** argv) {
    process ();
    return 0;
}

#endif