BLE Environmental Sensing Service
=================================

This app uses BLE to provide the
[environmental sensing service](https://www.bluetooth.com/specifications/assigned-numbers/environmental-sensing-service-characteristics)
as an IPC service. This app configures the ESS profile for BLE, and then accepts
updates for the sensor values as IPC messages.

Example IPC Setup
-----------------

The service is identified as `org.tockos.services.ble-ess`. As an example, in C
you might do:

```c
char buf[64] __attribute__((aligned(64)));

static void ipc_callback(__attribute__ ((unused)) int pid,
                         __attribute__ ((unused)) int len,
                         __attribute__ ((unused)) int arg2,
                         __attribute__ ((unused)) void* ud) {
  printf("Updated BLE characteristic.\n");
}

int main() {
  int svc_num = ipc_discover("org.tockos.services.ble-ess");
  if (svc_num < 0) {
    printf("No BLE ESS service installed.\n");
    return -1;
  }

  ipc_register_client_cb(svc_num, ipc_callback, NULL);
  ipc_share(svc_num, buf, 64);

  // Tell this service to use the contants of the buffer to update the ESS state.
  ipc_notify_svc(svc_num);
}
```

ESS IPC Protocol
----------------

To update the ESS values, this service expects two four-byte, little-endian
integers at the start of the buffer shared between the process as the IPC
message. The first integer is the sensor type that the app is trying to update.
The second integer is the value of the sensor reading. This maps to this struct:

```c
typedef struct {
  int type;  // sensor type
  int value; // sensor reading
} sensor_update_t;
```

The currently supported types:

| Sensor Type | Type Identifier | Value Description                             |
|-------------|-----------------|-----------------------------------------------|
| Temperature | `0x00`          | Temperature in hundredths of degrees Celsius. |
| Irradiance  | `0x01`          | Light irradiance in 0.1 W/m^2.                |
| Humidity    | `0x02`          | Humidity in hundredths of a percent.          |


nRF Serialization Note
----------------------

This app uses Nordic's BLE Serialization format where BLE commands can be
communicated over UART to the nRF51822 radio. This serialization protocol allows
for otherwise normal nRF51822 BLE apps to be executed on a different
microcontroller. All function calls that would have hit the BLE stack are
instead shuttled over UART to the nRF51822 radio.
