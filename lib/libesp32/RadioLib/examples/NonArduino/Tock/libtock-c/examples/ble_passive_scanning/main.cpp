#include "advertisement.h"
#include "advertisement_list.h"
#include <ble.h>
#include <stdio.h>

/*
 * BLE Demo Application
 * Passive scanner for Bluetooth Low Energy advertisements
 */

const int BUF_SIZE = 39;
static unsigned char scan[BUF_SIZE];
AdvertisementList list;

static void callback(int result, int len, __attribute__((unused)) int unused2,
                     __attribute__((unused)) void* ud)
{
  if (result == RETURNCODE_SUCCESS) {
    if (Advertisement::checkScanResult(scan, len)) {
      Advertisement advertisement(scan, len);

      if (list.tryAdd(advertisement)) {
        list.printList();
      }
      // FIXME: add this to get dynamic behavior i.e, update every time new advertisement is detected
      // but might it fill the print buffer, use at your own risk
      // else if (list.tryUpdateData(advertisement)) {
      //   list.printList();
      // }
    }
  }
}

int main(void)
{
  printf("[Tutorial] BLE Passive Scanner\r\n");

  // using the pre-configured advertisement interval
  int err = ble_start_passive_scan(scan, BUF_SIZE, callback);

  if (err < RETURNCODE_SUCCESS) {
    printf("ble_start_passive_scan, error: %s\r\n", tock_strrcode(static_cast<returncode_t>(err)));
  }
  return 0;
}
