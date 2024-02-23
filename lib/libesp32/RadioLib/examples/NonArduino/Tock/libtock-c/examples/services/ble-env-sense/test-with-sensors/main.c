#include <stdio.h>
#include <string.h>

#include <ipc.h>
#include <timer.h>

#include <ambient_light.h>
#include <humidity.h>
#include <temperature.h>

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

bool _ipc_done = false;
tock_timer_t _timer;

static void ipc_callback(__attribute__ ((unused)) int   pid,
                         __attribute__ ((unused)) int   len,
                         __attribute__ ((unused)) int   arg2,
                         __attribute__ ((unused)) void* ud) {
  _ipc_done = true;
}

static void do_sensing_cb(__attribute__ ((unused)) int   now,
                          __attribute__ ((unused)) int   expiration,
                          __attribute__ ((unused)) int   unused,
                          __attribute__ ((unused)) void* ud) {

  printf("[BLE ESS Test] Sampling Sensors\n");

  sensor_update_t *update = (sensor_update_t*) buf;

  int light     = 0;
  int temp      = 0;
  unsigned humi = 0;

  if (driver_exists(DRIVER_NUM_AMBIENT_LIGHT)) {
    ambient_light_read_intensity_sync(&light);

    update->type  = SENSOR_IRRADIANCE;
    update->value = light;
    ipc_notify_service(_svc_num);
    _ipc_done = false;
    yield_for(&_ipc_done);
  }

  if (driver_exists(DRIVER_NUM_TEMPERATURE)) {
    temperature_read_sync(&temp);

    update->type  = SENSOR_TEMPERATURE;
    update->value = temp;
    ipc_notify_service(_svc_num);
    _ipc_done = false;
    yield_for(&_ipc_done);
  }

  if (driver_exists(DRIVER_NUM_HUMIDITY)) {
    humidity_read_sync(&humi);

    update->type  = SENSOR_HUMIDITY;
    update->value = humi;
    ipc_notify_service(_svc_num);
    _ipc_done = false;
    yield_for(&_ipc_done);
  }

  printf("Setting ESS to:\n");
  printf("  light: %i\n", light);
  printf("  temp:  %i\n", temp);
  printf("  humi:  %i\n", humi);

  timer_in(3000, do_sensing_cb, NULL, &_timer);

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
  ipc_share(_svc_num, buf, 64);

  timer_in(1000, do_sensing_cb, NULL, &_timer);

  return 0;
}
