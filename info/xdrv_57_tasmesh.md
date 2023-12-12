# TASMESH

This driver provides the ability to move TASMOTA-devices out of the WLAN by using ESP-NOW to communicate bidirectional with an internal protocol.

Thus the workload for the WLAN-router is reduced and with the reduced overhead the local 2.4-GHz-band will be freed of some traffic. Power consumption of the nodes will be reduced significantly allowing better battery powered projects with TASMOTA.
Automatic payload encryption is applied using the WiFi-password1 as the key. A maximum of 32 bytes of this password is used for the ChaCha20Poly1305 authenticated encryption as the key.

As ACK/NACK messages seem to be not reliable on both ESP-platforms, the method "send-and-pray" is used.


## Working principle

An ESP32 is needed as gateway/broker to connect the nodes (typically an ESP8266) to the WLAN. The ESP32 will receive the MQTT-topic of every node and subscribe to it as a proxy.
If a MQTT-message in the form of 'cmnd/node_topic/...' is received, the broker will automatically send this to the referring node via ESP-NOW.
The broker will automatically send time messages to all nodes.

The nodes will send their MQTT-messages back to the broker via ESP-NOW.

## Enabling the driver

Add ``#define USE_TASMESH`` to your file ``user_config_override.h`` before compilation.

## Commands

**WARNING:  The MAC address used for ESP-NOW on the broker is the *Soft AP MAC*, not the WiFi MAC.**

*NOTE:  The colons in the mac addresses of the commands are optional.*

``MeshBroker``                  - starts the broker on the ESP32, printing out the MAC and used WiFi-channel to the log. Must be called after WiFi is initialized!! Example 'Rule1 on system#boot do meshbroker endon'

``MeshChannel 1..13``           - changes the WiFi-channel (on the node) to n (1-13) according to the channel of the (ESP32-)broker.

``MeshNode AA:BB:CC:DD:EE:FF``  - starts a node and connects the the broker with the given MAC-address, will automatically send MQTT-topic to the broker

``MeshPeer AA:BB:CC:DD:EE:FF``  - usable to add a known node to another node to be able to send data via the mesh to the broker, that may be out of reach

``MeshInterval 2..200``         - changes the interval between mesh messages default set to 50 ms

## Rules

Rules examples:

- The broker must be started after wifi is up!!
  - To start as ESP32 as broker after wifi and mqtt connection, use</br>``rule1 on system#boot do meshbroker endon``
- The node may be started as soon as possible. Once started wifi and webserver are disabled by design.
  - To start the node immediately use</br>``rule1 on system#init do meshnode FA:KE:AD:DR:ES:S1 endon``
    - To use mesh in combination with deep sleep, you must set a rule to re-initialize the mesh on wake-up.
The mesh status and parameters are **NOT** (yet) saved to flash and the mesh is not restarted automatically.
    - **WARNING**:  In case of a system-wide power outage, nodes will be unable to reconnect until after the broker is ready!
If all devices power up at the same time, a broker starting after `system#boot` will likely not be ready until *after* a node attempting to join at `system#init`.
This will cause the node to fail to mesh and *no retrying is implemented at this time*.
To account for this, instead of (or in addition to) using a rule on the nodes, assign all nodes to a common group topic (`GroupTopic2 tasnodes`) and have the broker send a command on that topic after it is ready:</br>`rule2 on mesh#broker=1 do publish cmnd/tasnodes/meshnode FA:KE:AD:DR:ES:S1`
- Add a known peer (another node in the mesh) after the node has initialized</br>``rule3 on mesh#node=1 do meshpeer FA:KE:AD:DR:ES:S1 endon``

## Limitations

The following limitations apply:
- An ESP32 is only supported as a broker
- An ESP8266 is only supported as a node
- No command persistence is implemented so use rules to start a broker or a node after start up or deep sleep
- Although node send queues are implemented there is no node receive queue so MQTT commands send to the node need to be as small as possible limited to a maximum of around 160 characters including the topic
- Although broker receive queues are implemented there is no broker send queue so MQTT commands send to the node need to be as small as possible limited to a maximum of around 160 characters including the topic
- As there is no direct connection from the node to the MQTT broker it will signal the node as LWT Offline
