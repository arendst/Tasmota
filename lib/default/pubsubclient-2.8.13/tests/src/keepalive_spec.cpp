#include "PubSubClient.h"
#include "ShimClient.h"
#include "Buffer.h"
#include "BDDTest.h"
#include "trace.h"
#include <unistd.h>

byte server[] = { 172, 16, 0, 2 };

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}


int test_keepalive_pings_idle() {
    IT("keeps an idle connection alive (takes 1 minute)");

    ShimClient shimClient;
    shimClient.setAllowConnect(true);

    byte connack[] = { 0x20, 0x02, 0x00, 0x00 };
    shimClient.respond(connack,4);

    PubSubClient client(server, 1883, callback, shimClient);
    int rc = client.connect((char*)"client_test1");
    IS_TRUE(rc);

    byte pingreq[] = { 0xC0,0x0 };
    shimClient.expect(pingreq,2);
    byte pingresp[] = { 0xD0,0x0 };
    shimClient.respond(pingresp,2);

    for (int i = 0; i < 50; i++) {
        sleep(1);
        if ( i == 15 || i == 31 || i == 47) {
            shimClient.expect(pingreq,2);
            shimClient.respond(pingresp,2);
        }
        rc = client.loop();
        IS_TRUE(rc);
    }

    IS_FALSE(shimClient.error());

    END_IT
}

int test_keepalive_pings_with_outbound_qos0() {
    IT("keeps a connection alive that only sends qos0 (takes 1 minute)");

    ShimClient shimClient;
    shimClient.setAllowConnect(true);

    byte connack[] = { 0x20, 0x02, 0x00, 0x00 };
    shimClient.respond(connack,4);

    PubSubClient client(server, 1883, callback, shimClient);
    int rc = client.connect((char*)"client_test1");
    IS_TRUE(rc);

    byte publish[] = {0x30,0xe,0x0,0x5,0x74,0x6f,0x70,0x69,0x63,0x70,0x61,0x79,0x6c,0x6f,0x61,0x64};

    for (int i = 0; i < 50; i++) {
        TRACE(i<<":");
        shimClient.expect(publish,16);
        rc = client.publish((char*)"topic",(char*)"payload");
        IS_TRUE(rc);
        IS_FALSE(shimClient.error());
        sleep(1);
        if ( i == 15 || i == 31 || i == 47) {
            byte pingreq[] = { 0xC0,0x0 };
            shimClient.expect(pingreq,2);
            byte pingresp[] = { 0xD0,0x0 };
            shimClient.respond(pingresp,2);
        }
        rc = client.loop();
        IS_TRUE(rc);
        IS_FALSE(shimClient.error());
    }

    END_IT
}

int test_keepalive_pings_with_inbound_qos0() {
    IT("keeps a connection alive that only receives qos0 (takes 1 minute)");

    ShimClient shimClient;
    shimClient.setAllowConnect(true);

    byte connack[] = { 0x20, 0x02, 0x00, 0x00 };
    shimClient.respond(connack,4);

    PubSubClient client(server, 1883, callback, shimClient);
    int rc = client.connect((char*)"client_test1");
    IS_TRUE(rc);

    byte publish[] = {0x30,0xe,0x0,0x5,0x74,0x6f,0x70,0x69,0x63,0x70,0x61,0x79,0x6c,0x6f,0x61,0x64};

    for (int i = 0; i < 50; i++) {
        TRACE(i<<":");
        sleep(1);
        if ( i == 15 || i == 31 || i == 47) {
            byte pingreq[] = { 0xC0,0x0 };
            shimClient.expect(pingreq,2);
            byte pingresp[] = { 0xD0,0x0 };
            shimClient.respond(pingresp,2);
        }
        shimClient.respond(publish,16);
        rc = client.loop();
        IS_TRUE(rc);
        IS_FALSE(shimClient.error());
    }

    END_IT
}

int test_keepalive_no_pings_inbound_qos1() {
    IT("does not send pings for connections with inbound qos1 (takes 1 minute)");

    ShimClient shimClient;
    shimClient.setAllowConnect(true);

    byte connack[] = { 0x20, 0x02, 0x00, 0x00 };
    shimClient.respond(connack,4);

    PubSubClient client(server, 1883, callback, shimClient);
    int rc = client.connect((char*)"client_test1");
    IS_TRUE(rc);

    byte publish[] = {0x32,0x10,0x0,0x5,0x74,0x6f,0x70,0x69,0x63,0x12,0x34,0x70,0x61,0x79,0x6c,0x6f,0x61,0x64};
    byte puback[] = {0x40,0x2,0x12,0x34};

    for (int i = 0; i < 50; i++) {
        shimClient.respond(publish,18);
        shimClient.expect(puback,4);
        sleep(1);
        rc = client.loop();
        IS_TRUE(rc);
        IS_FALSE(shimClient.error());
    }

    END_IT
}

int test_keepalive_disconnects_hung() {
    IT("disconnects a hung connection (takes 30 seconds)");

    ShimClient shimClient;
    shimClient.setAllowConnect(true);

    byte connack[] = { 0x20, 0x02, 0x00, 0x00 };
    shimClient.respond(connack,4);

    PubSubClient client(server, 1883, callback, shimClient);
    int rc = client.connect((char*)"client_test1");
    IS_TRUE(rc);

    byte pingreq[] = { 0xC0,0x0 };
    shimClient.expect(pingreq,2);

    for (int i = 0; i < 32; i++) {
        sleep(1);
        rc = client.loop();
    }
    IS_FALSE(rc);

    int state = client.state();
    IS_TRUE(state == MQTT_CONNECTION_TIMEOUT);

    IS_FALSE(shimClient.error());

    END_IT
}

int main()
{
    SUITE("Keep-alive");
    test_keepalive_pings_idle();
    test_keepalive_pings_with_outbound_qos0();
    test_keepalive_pings_with_inbound_qos0();
    test_keepalive_no_pings_inbound_qos1();
    test_keepalive_disconnects_hung();

    FINISH
}
