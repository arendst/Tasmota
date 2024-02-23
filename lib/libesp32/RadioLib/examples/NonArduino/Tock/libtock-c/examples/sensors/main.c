#include <stdbool.h>
#include <stdio.h>

#include <ambient_light.h>
#include <humidity.h>
#include <lps25hb.h>
#include <ninedof.h>
#include <proximity.h>
#include <sound_pressure.h>
#include <temperature.h>
#include <timer.h>
#include <tock.h>
#include <tsl2561.h>

static tock_timer_t timer;
static bool light          = false;
static bool tsl2561        = false;
static bool lps25hb        = false;
static bool temperature    = false;
static bool humidity       = false;
static bool ninedof        = false;
static bool ninedof_accel  = false;
static bool ninedof_mag    = false;
static bool ninedof_gyro   = false;
static bool proximity      = false;
static bool sound_pressure = false;
static void timer_fired(__attribute__ ((unused)) int   arg0,
                        __attribute__ ((unused)) int   arg1,
                        __attribute__ ((unused)) int   arg2,
                        __attribute__ ((unused)) void* ud) {
  int lite = 0;
  int tsl2561_lux = 0;
  int lps25hb_pressure = 0;
  int temp = 0;
  unsigned humi = 0;
  int ninedof_accel_x = 0, ninedof_accel_y = 0, ninedof_accel_z = 0;
  int ninedof_magneto_x = 0, ninedof_magneto_y = 0, ninedof_magneto_z = 0;
  int ninedof_gyro_x = 0, ninedof_gyro_y = 0, ninedof_gyro_z = 0;
  uint8_t prox_reading = 0;
  unsigned char sound_pressure_reading = 0;

  /* *INDENT-OFF* */
  if (light)          ambient_light_read_intensity_sync(&lite);
  if (tsl2561)        tsl2561_get_lux_sync(&tsl2561_lux);
  if (lps25hb)        lps25hb_get_pressure_sync(&lps25hb_pressure);
  if (temperature)    temperature_read_sync(&temp);
  if (humidity)       humidity_read_sync(&humi);
  if (ninedof_accel)  ninedof_read_acceleration_sync(&ninedof_accel_x, &ninedof_accel_y, &ninedof_accel_z);
  if (ninedof_mag)    ninedof_read_magnetometer_sync(&ninedof_magneto_x, &ninedof_magneto_y, &ninedof_magneto_z);
  if (ninedof_gyro)   ninedof_read_gyroscope_sync(&ninedof_gyro_x, &ninedof_gyro_y, &ninedof_gyro_z);
  if (proximity)      proximity_read_sync(&prox_reading);
  if (sound_pressure) sound_pressure_read_sync(&sound_pressure_reading);

  if (light)          printf("Amb. Light: Light Intensity: %d\n", lite);
  if (tsl2561)        printf("TSL2561:    Light:           %d lux\n", tsl2561_lux);
  if (lps25hb)        printf("LPS25HB:    Pressure:        %d\n", lps25hb_pressure);
  if (temperature)    printf("Temperature:                 %d deg C\n", temp/100);
  if (humidity)       printf("Humidity:                    %u%%\n", humi/100);
  if (ninedof_accel)  printf("Acceleration: X: %d Y: %d Z: %d\n", ninedof_accel_x, ninedof_accel_y, ninedof_accel_z);
  if (ninedof_mag)    printf("Magnetometer: X: %d Y: %d Z: %d\n", ninedof_magneto_x, ninedof_magneto_y, ninedof_magneto_z);
  if (ninedof_gyro)   printf("Gyro:         X: %d Y: %d Z: %d\n", ninedof_gyro_x, ninedof_gyro_y, ninedof_gyro_z);
  if (proximity)      printf("Proximity:                   %u\n", prox_reading);
  if (sound_pressure) printf("Sound Pressure:              %u\n", sound_pressure_reading);

  /* *INDENT-ON* */

  printf("\n");
  timer_in(1000, timer_fired, NULL, &timer);
}

int main(void) {
  printf("[Sensors] Starting Sensors App.\n");
  printf("[Sensors] All available sensors on the platform will be sampled.\n");

  /* *INDENT-OFF* */
  light          = driver_exists(DRIVER_NUM_AMBIENT_LIGHT);
  tsl2561        = driver_exists(DRIVER_NUM_TSL2561);
  lps25hb        = driver_exists(DRIVER_NUM_LPS25HB);
  temperature    = driver_exists(DRIVER_NUM_TEMPERATURE);
  humidity       = driver_exists(DRIVER_NUM_HUMIDITY);
  ninedof        = driver_exists(DRIVER_NUM_NINEDOF);
  proximity      = driver_exists(DRIVER_NUM_PROXIMITY);
  sound_pressure = driver_exists(DRIVER_NUM_SOUND_PRESSURE);
  /* *INDENT-ON* */

  if (ninedof) {
    int buffer;
    ninedof_accel = (ninedof_read_acceleration_sync(&buffer, &buffer, &buffer) == RETURNCODE_SUCCESS);
    ninedof_mag   = (ninedof_read_magnetometer_sync(&buffer, &buffer, &buffer) == RETURNCODE_SUCCESS);
    ninedof_gyro  = (ninedof_read_gyroscope_sync(&buffer, &buffer, &buffer) == RETURNCODE_SUCCESS);
  }

  /* *INDENT-OFF* */
  if (light)          printf("[Sensors]   Sampling Ambient Light sensor.\n");
  if (tsl2561)        printf("[Sensors]   Sampling TSL2561 Light sensor.\n");
  if (lps25hb)        printf("[Sensors]   Sampling LPS25HB pressure sensor.\n");
  if (temperature)    printf("[Sensors]   Sampling Temperature sensor.\n");
  if (humidity)       printf("[Sensors]   Sampling Humidity sensor.\n");
  if (ninedof_accel)  printf("[Sensors]   Sampling Accelerometer.\n");
  if (ninedof_mag)    printf("[Sensors]   Sampling Magnetometer.\n");
  if (ninedof_gyro)   printf("[Sensors]   Sampling Gyroscope.\n");
  if (proximity)      printf("[Sensors]   Sampling Proximity sensor.\n");
  if (sound_pressure) printf("[Sensors]   Sampling Sound Pressure sensor.\n");
  /* *INDENT-ON* */

  if (sound_pressure) {
    sound_pressure_enable();
  }

  // Setup periodic timer to sample the sensors.
  timer_in(1000, timer_fired, NULL, &timer);

  return 0;
}
