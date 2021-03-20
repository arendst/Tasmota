#include "PubSubClient.h"
#include "ShimClient.h"
#include "Buffer.h"
#include "BDDTest.h"
#include "trace.h"


byte server[] = { 172, 16, 0, 2 };

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}

int test_publish() {
    IT("publishes a null-terminated string");
    ShimClient shimClient;
    shimClient.setAllowConnect(true);

    byte connack[] = { 0x20, 0x02, 0x00, 0x00 };
    shimClient.respond(connack,4);

    PubSubClient client(server, 1883, callback, shimClient);
    int rc = client.connect((char*)"client_test1");
    IS_TRUE(rc);

    byte publish[] = {0x30,0xe,0x0,0x5,0x74,0x6f,0x70,0x69,0x63,0x70,0x61,0x79,0x6c,0x6f,0x61,0x64};
    shimClient.expect(publish,16);

    rc = client.publish((char*)"topic",(char*)"payload");
    IS_TRUE(rc);

    IS_FALSE(shimClient.error());

    END_IT
}


int test_publish_bytes() {
    IT("publishes a byte array");
    ShimClient shimClient;
    shimClient.setAllowConnect(true);

    byte payload[] = { 0x01,0x02,0x03,0x0,0x05 };
    int length = 5;

    byte connack[] = { 0x20, 0x02, 0x00, 0x00 };
    shimClient.respond(connack,4);

    PubSubClient client(server, 1883, callback, shimClient);
    int rc = client.connect((char*)"client_test1");
    IS_TRUE(rc);

    byte publish[] = {0x30,0xc,0x0,0x5,0x74,0x6f,0x70,0x69,0x63,0x1,0x2,0x3,0x0,0x5};
    shimClient.expect(publish,14);

    rc = client.publish((char*)"topic",payload,length);
    IS_TRUE(rc);

    IS_FALSE(shimClient.error());

    END_IT
}


int test_publish_retained() {
    IT("publishes retained - 1");
    ShimClient shimClient;
    shimClient.setAllowConnect(true);

    byte payload[] = { 0x01,0x02,0x03,0x0,0x05 };
    int length = 5;

    byte connack[] = { 0x20, 0x02, 0x00, 0x00 };
    shimClient.respond(connack,4);

    PubSubClient client(server, 1883, callback, shimClient);
    int rc = client.connect((char*)"client_test1");
    IS_TRUE(rc);

    byte publish[] = {0x31,0xc,0x0,0x5,0x74,0x6f,0x70,0x69,0x63,0x1,0x2,0x3,0x0,0x5};
    shimClient.expect(publish,14);

    rc = client.publish((char*)"topic",payload,length,true);
    IS_TRUE(rc);

    IS_FALSE(shimClient.error());

    END_IT
}

int test_publish_retained_2() {
    IT("publishes retained - 2");
    ShimClient shimClient;
    shimClient.setAllowConnect(true);

    byte connack[] = { 0x20, 0x02, 0x00, 0x00 };
    shimClient.respond(connack,4);

    PubSubClient client(server, 1883, callback, shimClient);
    int rc = client.connect((char*)"client_test1");
    IS_TRUE(rc);

    byte publish[] = {0x31,0xc,0x0,0x5,0x74,0x6f,0x70,0x69,0x63,'A','B','C','D','E'};
    shimClient.expect(publish,14);

    rc = client.publish((char*)"topic",(char*)"ABCDE",true);
    IS_TRUE(rc);

    IS_FALSE(shimClient.error());

    END_IT
}

int test_publish_not_connected() {
    IT("publish fails when not connected");
    ShimClient shimClient;

    PubSubClient client(server, 1883, callback, shimClient);

    int rc = client.publish((char*)"topic",(char*)"payload");
    IS_FALSE(rc);

    IS_FALSE(shimClient.error());

    END_IT
}

int test_publish_too_long() {
    IT("publish fails when topic/payload are too long");
    ShimClient shimClient;
    shimClient.setAllowConnect(true);

    byte connack[] = { 0x20, 0x02, 0x00, 0x00 };
    shimClient.respond(connack,4);

    PubSubClient client(server, 1883, callback, shimClient);
    client.setBufferSize(128);
    int rc = client.connect((char*)"client_test1");
    IS_TRUE(rc);

    //                                         0        1         2         3         4         5         6         7         8         9         0         1         2
    rc = client.publish((char*)"topic",(char*)"123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
    IS_FALSE(rc);

    IS_FALSE(shimClient.error());

    END_IT
}

int test_publish_P() {
    IT("publishes using PROGMEM");
    ShimClient shimClient;
    shimClient.setAllowConnect(true);

    byte payload[] = { 0x01,0x02,0x03,0x0,0x05 };
    int length = 5;

    byte connack[] = { 0x20, 0x02, 0x00, 0x00 };
    shimClient.respond(connack,4);

    PubSubClient client(server, 1883, callback, shimClient);
    int rc = client.connect((char*)"client_test1");
    IS_TRUE(rc);

    byte publish[] = {0x31,0xc,0x0,0x5,0x74,0x6f,0x70,0x69,0x63,0x1,0x2,0x3,0x0,0x5};
    shimClient.expect(publish,14);

    rc = client.publish_P((char*)"topic",payload,length,true);
    IS_TRUE(rc);

    IS_FALSE(shimClient.error());

    END_IT
}




int main()
{
    SUITE("Publish");
    test_publish();
    test_publish_bytes();
    test_publish_retained();
    test_publish_retained_2();
    test_publish_not_connected();
    test_publish_too_long();
    test_publish_P();

    FINISH
}
