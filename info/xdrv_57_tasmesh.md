# TASMESH

This driver provides the ability to move TASMOTA-devices out of the WLAN by using ESP-NOW to communicate bidirectional with an internal protocol.

Thus the workload for the WLAN-router is reduced and with the reduced overhead the local 2.4-GHz-band will be freed of some traffic. Power consumption of the nodes will be reduced significantly allowing better battery powered projects with TASMOTA.
Automatic payload encryption is applied using the WiFi-password1 as the key. A maximum of 32 bytes of this password is used for the ChaCha20Poly1305 authenticated encryption as the key.

As ACK/NACK messages seem to be not reliable on both ESP-platforms, the method "send-and-pray" is used.


## Working priciple

An ESP32 is needed as gateway/broker to connect the nodes (typically an ESP8266) to the WLAN. The ESP32 will receive the MQTT-topic of every node and subscribe to it as a proxy.
If a MQTT-message in the form of 'cmnd/node_topic/...' is received, the broker will automatically send this to the referring node via ESP-NOW.
The broker will automatically send time messages to all nodes.

The nodes will send their MQTT-messages back to the broker via ESP-NOW.

## Enabling the driver

Add ``#define USE_TASMESH`` to your file ``user_config_override.h`` before compilation.

## Commands

``MeshBroker``                  - starts the broker on the ESP32, printing out the MAC and used WiFi-channel to the log. Must be called after WiFi is initialized!! Example 'Rule1 on system#boot do meshbroker endon'

``MeshChannel 1..13``           - changes the WiFi-channel (on the node) to n (1-13) according to the channel of the (ESP32-)broker.

``MeshNode AA:BB:CC:DD:EE:FF``  - starts a node and connects the the broker with the given MAC-address, will automatically send MQTT-topic to the broker

``MeshPeer AA:BB:CC:DD:EE:FF``  - usable to add a known node to another node to be able to send data via the mesh to the broker, that may be out of reach

``MeshInterval 2..200``         - changes the interval between mesh messages default set to 50 ms

## Rules

Rules examples:

- The broker must be started after wifi is up!!</br>``rule1 on system#boot do meshbroker endon``
- The node may be started as soon as possible. Once started wifi and webserver are disabled by design</br>``rule1 on system#init do meshnode 98:F4:AB:6D:2D:B5 endon``
- Add a known peer (another node in the mesh) after the node has initialized</br>``rule3 on mesh#node=1 do meshpeer 2cf4323cdb33 endon``

## Limitations

The following limitations apply:
- An ESP32 is only supported as a broker
- An ESP8266 is only supported as a node
- No command persistence is implemented so use rules to start a broker or a node
- Although node send queues are implemented there is no node receive queue so MQTT commands send to the node need to be as small as possible limited to a maximum of around 160 characters including the topic
- Although broker receive queues are implemented there is no broker send queue so MQTT commands send to the node need to be as small as possible limited to a maximum of around 160 characters including the topic
- As there is no direct connection from the node to the MQTT broker it will signal the node as LWT Offline
