#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <console.h>
#include <max17205.h>
#include <tock.h>

int main (void) {
  printf("[MAX17205] Test App\n");

  uint64_t rom_id;
  int rc = max17205_read_rom_id_sync(&rom_id);
  if (rc == RETURNCODE_SUCCESS) {
    printf("Found ROM ID: 0x%llX\n", rom_id);
  } else {
    printf("Got error: %s\n", tock_strrcode(rc));
  }

  printf("\n");

  uint16_t status;
  rc = max17205_read_status_sync(&status);
  if (rc == RETURNCODE_SUCCESS) {
    printf("Status: 0x%04X\n", status);
  } else {
    printf("Got error: %d - %s\n", rc, tock_strrcode(rc));
  }

  printf("\n");

  uint16_t percent, soc_mah, soc_mah_full;
  rc = max17205_read_soc_sync(&percent, &soc_mah, &soc_mah_full);
  if (rc == RETURNCODE_SUCCESS) {
    printf("Percent (.001%%): %ld\n", lrint(max17205_get_percentage_mP(percent)));
    printf("State of charge in uAh: %ld\n", lrint(max17205_get_capacity_uAh(soc_mah)));
    printf("Full charge in uAh: %ld\n", lrint(max17205_get_capacity_uAh(soc_mah_full)));
  } else {
    printf("Got error: %d - %s\n", rc, tock_strrcode(rc));
  }

  printf("\n");

  uint16_t voltage;
  int16_t current;
  rc = max17205_read_voltage_current_sync(&voltage, &current);
  if (rc == RETURNCODE_SUCCESS) {
    printf("Voltage (mV): %ld\n", lrint(max17205_get_voltage_mV(voltage)));
    printf("Current (uA): %ld\n", lrint(max17205_get_current_uA(current)));
  } else {
    printf("Got error: %d - %s\n", rc, tock_strrcode(rc));
  }
}
