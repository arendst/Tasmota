/** @file
 *
 * IGNORE THIS FILE IF USING ESP-IDF, USE MENUCONFIG TO SET NIMBLE OPTIONS.
 *
 * The config options here are for doxygen documentation only.
 */

#pragma once

#include "sdkconfig.h"
#include "nimconfig_rename.h"

#if defined(CONFIG_BT_ENABLED)

// Allows cpp wrapper to select the correct include paths when using esp-idf
#define CONFIG_NIMBLE_CPP_IDF

/* Cannot use client without scan */
#if defined(CONFIG_BT_NIMBLE_ROLE_CENTRAL) && !defined(CONFIG_BT_NIMBLE_ROLE_OBSERVER)
#define CONFIG_BT_NIMBLE_ROLE_OBSERVER
#endif

/* Cannot use server without advertise */
#if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL) && !defined(CONFIG_BT_NIMBLE_ROLE_BROADCASTER)
#define CONFIG_BT_NIMBLE_ROLE_BROADCASTER
#endif

/* Enables the use of Arduino String class for attribute values */
#if defined __has_include
#  if __has_include (<Arduino.h>)
#    define NIMBLE_CPP_ARDUINO_STRING_AVAILABLE
#  endif
#endif

#endif /* CONFIG_BT_ENABLED */

#ifdef _DOXYGEN_

/** @brief Un-comment to change the number of simultaneous connections (esp controller max is 9) */
#define CONFIG_BT_NIMBLE_MAX_CONNECTIONS 3

/** @brief Un-comment to enable storing the timestamp when an attribute value is updated\n
 *  This allows for checking the last update time using getTimeStamp() or getValue(time_t*)\n
 *  If disabled, the timestamp returned from these functions will be 0.\n
 *  Disabling timestamps will reduce the memory used for each value.\n
 *  1 = Enabled, 0 = Disabled; Default = Disabled
 */
#define CONFIG_NIMBLE_CPP_ATT_VALUE_TIMESTAMP_ENABLED 0

/** @brief Uncomment to set the default allocation size (bytes) for each attribute if\n
 *  not specified when the constructor is called. This is also the size used when a remote\n
 *  characteristic or descriptor is constructed before a value is read/notifed.\n
 *  Increasing this will reduce reallocations but increase memory footprint.\n
 *  Default value is 20. Range: 1 : 512 (BLE_ATT_ATTR_MAX_LEN)
 */
#define CONFIG_NIMBLE_CPP_ATT_VALUE_INIT_LENGTH 20

/** @brief Un-comment to change the default MTU size */
#define CONFIG_BT_NIMBLE_ATT_PREFERRED_MTU 255

/** @brief Un-comment to change default device name */
#define CONFIG_BT_NIMBLE_SVC_GAP_DEVICE_NAME "nimble"

/** @brief Un-comment to set the debug log messages level from the NimBLE host stack.\n
 *  Values: 0 = DEBUG, 1 = INFO, 2 = WARNING, 3 = ERROR, 4 = CRITICAL, 5+ = NONE\n
 *  Uses approx. 32kB of flash memory.
 */
#define CONFIG_BT_NIMBLE_LOG_LEVEL 5

/** @brief Un-comment to set the debug log messages level from the NimBLE CPP Wrapper.\n
 *  Values: 0 = NONE, 1 = ERROR, 2 = WARNING, 3 = INFO, 4+ = DEBUG\n
 *  Uses approx. 32kB of flash memory.
 */
#define CONFIG_NIMBLE_CPP_LOG_LEVEL 0

/** @brief Un-comment to see NimBLE host return codes as text debug log messages.
 *  Uses approx. 7kB of flash memory.
 */
#define CONFIG_NIMBLE_CPP_ENABLE_RETURN_CODE_TEXT

/** @brief Un-comment to see GAP event codes as text in debug log messages.
 *  Uses approx. 1kB of flash memory.
 */
#define CONFIG_NIMBLE_CPP_ENABLE_GAP_EVENT_CODE_TEXT

/** @brief Un-comment to see advertisment types as text while scanning in debug log messages.
 *  Uses approx. 250 bytes of flash memory.
 */
#define CONFIG_NIMBLE_CPP_ENABLE_ADVERTISMENT_TYPE_TEXT

/** @brief Un-comment to change the default GAP appearance */
#define CONFIG_BT_NIMBLE_SVC_GAP_APPEARANCE 0x0

 /** @brief Un-comment if not using NimBLE Client functions \n
 *  Reduces flash size by approx. 7kB.
 */
#define CONFIG_BT_NIMBLE_ROLE_CENTRAL_DISABLED

/** @brief Un-comment if not using NimBLE Scan functions \n
 *  Reduces flash size by approx. 26kB.
 */
#define CONFIG_BT_NIMBLE_ROLE_OBSERVER_DISABLED

/** @brief Un-comment if not using NimBLE Server functions \n
 *  Reduces flash size by approx. 16kB.
 */
#define CONFIG_BT_NIMBLE_ROLE_PERIPHERAL_DISABLED

/** @brief Un-comment if not using NimBLE Advertising functions \n
 *  Reduces flash size by approx. 5kB.
 */
#define CONFIG_BT_NIMBLE_ROLE_BROADCASTER_DISABLED

/** @brief Un-comment to change the number of devices allowed to store/bond with */
#define CONFIG_BT_NIMBLE_MAX_BONDS 3

/** @brief Un-comment to change the maximum number of CCCD subscriptions to store */
#define CONFIG_BT_NIMBLE_MAX_CCCDS 8

/** @brief Un-comment to change the random address refresh time (in seconds) */
#define CONFIG_BT_NIMBLE_RPA_TIMEOUT 900

/**
 * @brief Un-comment to change the number of MSYS buffers available.
 * @details MSYS is a system level mbuf registry. For prepare write & prepare \n
 * responses MBUFs are allocated out of msys_1 pool. This may need to be increased if\n
 * you are sending large blocks of data with a low MTU. E.g: 512 bytes with 23 MTU will fail.
 */
#define CONFIG_BT_NIMBLE_MSYS1_BLOCK_COUNT 12

/** @brief Un-comment to use external PSRAM for the NimBLE host */
#define CONFIG_BT_NIMBLE_MEM_ALLOC_MODE_EXTERNAL 1

/** @brief Un-comment to change the core NimBLE host runs on */
#define CONFIG_BT_NIMBLE_PINNED_TO_CORE 0

/** @brief Un-comment to change the stack size for the NimBLE host task */
#define CONFIG_BT_NIMBLE_TASK_STACK_SIZE 4096

#endif // _DOXYGEN_
