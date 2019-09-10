# Docker container for tasmota builds
This Container will setup a proper build environment for [Sonoff-Tasmota](https://github.com/arendst/Sonoff-Tasmota)

## Create container
`docker build -t mytasmota:latest .`

## Use a ready container from docker hub
Use instead of the container `mytasmota:latest` the published container `eddyhub/docker-tasmota:latest` from docker hub.

## Build all development binaries
`git clone https://github.com/arendst/Sonoff-Tasmota.git`
`docker run -ti --rm -v $(pwd)/Sonoff-Tasmota:/tasmota -u $UID:$GID mytasmota:latest`

## Build a specific binary with custom options
Checkout Sonoff-Tasmota: `git clone https://github.com/arendst/Sonoff-Tasmota.git`
Mount the source as volume in `/tasmota`. **Prefix** any parameter available in `Sonoff-Tasmota/sonoff/my_user_config.h` with `TASMOTA_` as a environment variable for the container. **Also don't forget to escape what needs to be escaped in your shell.** **Strings** should be in **double quotes**. My config example:
`docker run -ti --rm -v $(pwd)/Sonoff-Tasmota:/tasmota -e TASMOTA_STA_SSID1='"my-wifi"' -e TASMOTA_STA_PASS1='"my-wifi-password"' -e TASMOTA_MQTT_HOST='my-mqtt-host' -e TASMOTA_MQTT_USER='"my-mqtt-user"' -e TASMOTA_MQTT_PASS='"my-mqtt-password"' -e TASMOTA_WEB_PASSWORD='"my-web-password"' -u $UID:$GID mytasmota:latest --environment sonoff-DE

Now you should have the file Sonoff-Tasmota/.pioenvs/sonoff-DE/firmware.bin which can be flashed on your device.

## Build a specific version of tasmota
Checkout out the needed version before using the build instructions above:
- `git clone https://github.com/arendst/Sonoff-Tasmota.git`
- `git -C Sonoff-Tasmota checkout v6.6.0`
Build it:
- `docker run -ti --rm -v $(pwd)/Sonoff-Tasmota:/tasmota -u $UID:$GID mytasmota:latest`
