# TasmotaPubSub

Tasmota fork of Nick O'Leary's [PubSubClient](https://github.com/knolleary/pubsubclient) MQTT library,
hardened and adapted for use within Tasmota on ESP8266 and ESP32.

The public API and the `PubSubClient` class name are kept for compatibility, so Tasmota code
continues to use `#include <PubSubClient.h>`. This is now a standalone library: divergences
from the original are no longer tracked with per-hunk markers, they are documented inline
where the behaviour is non-obvious.

A host-based test suite lives in `tests/` (see `tests/README.md`).

Original library by Nick O'Leary — https://pubsubclient.knolleary.net

## License

Released under the MIT License. See `LICENSE.txt`.
