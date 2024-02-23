#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// From the simple_ble library in https://github.com/lab11/nrf5x-base
// Included in the libnrfserialization.a library.
#include <simple_ble.h>

#include <led.h>

/*******************************************************************************
 * BLE
 ******************************************************************************/

// Intervals for advertising and connections
simple_ble_config_t ble_config = {
  .platform_id       = 0x00,              // used as 4th octect in device BLE address
  .device_id         = DEVICE_ID_DEFAULT,
  .adv_name          = "Tock",
  .adv_interval      = MSEC_TO_UNITS(500, UNIT_0_625_MS),
  .min_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS),
  .max_conn_interval = MSEC_TO_UNITS(1250, UNIT_1_25_MS)
};

void app_error_fault_handler(__attribute__ ((unused)) uint32_t error_code,
                             __attribute__ ((unused)) uint32_t line_num,
                             __attribute__ ((unused)) uint32_t info) {
  // Ignore errors. With serialization, these happen because a packet
  // was framed wrong or corrupted between the nRF and the main processor.
  // The application can continue.
}

void ble_address_set (void) {
  // Need to redefine this function so that we do not try to set the address
  // on the main processor.
}

// Called when each advertisement is received.
void ble_evt_adv_report (ble_evt_t* p_ble_evt) {
  // Follow the nRF SDK data structures to get to the advertising data.
  // More doc here:
  // http://infocenter.nordicsemi.com/topic/com.nordic.infocenter.s130.api.v2.0.0/structble__gap__evt__adv__report__t.html
  ble_gap_evt_adv_report_t* adv = (ble_gap_evt_adv_report_t*) &p_ble_evt->evt.gap_evt.params.adv_report;

  // Print some details about the discovered advertisement.
  printf("Recv Advertisement: [%02x:%02x:%02x:%02x:%02x:%02x] RSSI: %d, Len: %d\n",
         adv->peer_addr.addr[5], adv->peer_addr.addr[4], adv->peer_addr.addr[3],
         adv->peer_addr.addr[2], adv->peer_addr.addr[1], adv->peer_addr.addr[0],
         adv->rssi, adv->dlen);

  // Also toggle the first LED.
  led_toggle(0);
}

/*******************************************************************************
 * MAIN
 ******************************************************************************/

int main (void) {
  printf("[Tutorial] BLE Scanning\n");

  // Setup BLE. See the simple_ble library for more information:
  // https://github.com/lab11/nrf5x-base/tree/master/lib
  simple_ble_init(&ble_config);

  // Scan for advertisements.
  simple_ble_scan_start();
}
