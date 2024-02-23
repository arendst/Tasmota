#include <stdio.h>
#include <string.h>

#include <ipc.h>
#include <timer.h>

size_t _svc_num = 0;

char buf[64] __attribute__((aligned(64)));

typedef enum {
  SENSOR_TEMPERATURE = 0,
  SENSOR_IRRADIANCE = 1,
  SENSOR_HUMIDITY = 2,
} sensor_type_e;

typedef struct {
  int type;  // sensor type
  int value; // sensor reading
} sensor_update_t;

static void ipc_callback(__attribute__ ((unused)) int   pid,
                         __attribute__ ((unused)) int   len,
                         __attribute__ ((unused)) int   arg2,
                         __attribute__ ((unused)) void* ud) {
  printf("Updated BLE characteristic.\n");
}

int main(void) {
  int err = ipc_discover("org.tockos.services.ble-ess", &_svc_num);
  if (err < 0) {
    printf("No BLE ESS service installed.\n");
    return -1;
  }

  printf("Found BLE ESS service (%u)\n", _svc_num);

  delay_ms(1500);

  sensor_update_t *update = (sensor_update_t*) buf;
  ipc_register_client_callback(_svc_num, ipc_callback, update);

  update->type  = SENSOR_HUMIDITY;
  update->value = 185;
  ipc_share(_svc_num, buf, 64);

  ipc_notify_service(_svc_num);
  return 0;
}
