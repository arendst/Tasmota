/**
 * @file "epd_board_specific.h"
 * @brief Board-specific functions that are only conditionally defined.
 */

#pragma once

#include <stdint.h>

/** This is a Lilygo47 specific function

  This is a work around a hardware issue with the Lilygo47 epd_poweroff() turns off the epaper
 completely however the hardware of the Lilygo47 is different than the official boards. Which means
 that on the Lilygo47 this disables power to the touchscreen.

  This is a workaround to allow to disable display power but not the touch screen.
  On the Lilygo the epd power flag was re-purposed as power enable
  for everything. This is a hardware thing.
 \warning This workaround may still leave power on to epd and as such may cause other problems such
 as grey screen.

  Please use epd_poweroff() and epd_deinit() whenever you sleep the system.
  The following code can be used to sleep the lilygo and power down the peripherals and wake the
 unit on touch. However is should be noted that the touch controller is not powered and as such the
 touch coordinates will not be captured. Arduino specific code: \code{.c} epd_poweroff();
  epd_deinit();
  esp_sleep_enable_ext1_wakeup(GPIO_SEL_13, ESP_EXT1_WAKEUP_ANY_HIGH);
  esp_deep_sleep_start();
  \endcode
*/
void epd_powerdown_lilygo_t5_47();
void epd_powerdown() __attribute__((deprecated));
