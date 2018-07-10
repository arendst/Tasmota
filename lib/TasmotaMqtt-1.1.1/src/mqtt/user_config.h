#ifndef __MQTT_CONFIG_H__
#define __MQTT_CONFIG_H__

//#define MQTT_SSL_ENABLE

#define MQTT_RECONNECT_TIMEOUT  5 /*second*/

//#define MQTT_BUF_SIZE           1024
#define MQTT_BUF_SIZE           512
#define QUEUE_BUFFER_SIZE       2048

//#define PROTOCOL_NAMEv31  /*MQTT version 3.1 compatible with Mosquitto v0.15*/
#define PROTOCOL_NAMEv311 /*MQTT version 3.11 compatible with https://eclipse.org/paho/clients/testing/*/

#endif // __MQTT_CONFIG_H__