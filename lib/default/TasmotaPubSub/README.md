# TasmotaPubSub

Tasmota fork of Nick O'Leary's [PubSubClient](https://github.com/knolleary/pubsubclient) MQTT library,
hardened and adapted for use within Tasmota on ESP8266 and ESP32.

The public API and the `PubSubClient` class name are kept for compatibility, so Tasmota code
continues to use `#include <PubSubClient.h>`. Tasmota-specific changes in the source are marked
with `// Start Tasmota patch` comments (see `CHANGES.txt`).

Original library by Nick O'Leary — https://pubsubclient.knolleary.net

## License

Released under the MIT License. See `LICENSE.txt`.
