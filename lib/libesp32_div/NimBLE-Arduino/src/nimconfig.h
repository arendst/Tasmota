 #pragma once

#ifdef ESP_PLATFORM
#include "sdkconfig.h"
#else
#include "ext_nimble_config.h"
#endif

#include "nimconfig_rename.h"

/***********************************************
 * Arduino user-config options start here
 **********************************************/

/** @brief Un-comment to change the number of simultaneous connections (esp controller max is 9) */
// #define CONFIG_BT_NIMBLE_MAX_CONNECTIONS 3

/** @brief Un-comment to enable storing the timestamp when an attribute value is updated\n
 *  This allows for checking the last update time using getTimeStamp() or getValue(time_t*)\n
 *  If disabled, the timestamp returned from these functions will be 0.\n
 *  Disabling timestamps will reduce the memory used for each value.\n
 *  1 = Enabled, 0 = Disabled; Default = Disabled
 */
// #define CONFIG_NIMBLE_CPP_ATT_VALUE_TIMESTAMP_ENABLED 0

/** @brief Uncomment to set the default allocation size (bytes) for each attribute if\n
 *  not specified when the constructor is called. This is also the size used when a remote\n
 *  characteristic or descriptor is constructed before a value is read/notified.\n
 *  Increasing this will reduce reallocations but increase memory footprint.\n
 *  Default value is 20. Range: 1 : 512 (BLE_ATT_ATTR_MAX_LEN)
 */
// #define CONFIG_NIMBLE_CPP_ATT_VALUE_INIT_LENGTH 20


/****************************************************
 *         Extended advertising settings            *
 * For use with ESP32C3, ESP32S3, ESP32H2 ONLY!     *
 ***************************************************/

/** @brief Un-comment to enable extended advertising */
// #define CONFIG_BT_NIMBLE_EXT_ADV 1

/** @brief Un-comment to set the max number of extended advertising instances (Range: 0 - 4) */
// #define CONFIG_BT_NIMBLE_MAX_EXT_ADV_INSTANCES 1

/** @brief Un-comment to set the max extended advertising data size (Range: 31 - 1650) */
// #define CONFIG_BT_NIMBLE_MAX_EXT_ADV_DATA_LEN 251

/** @brief Un-comment to enable periodic advertising */
// #define CONFIG_BT_NIMBLE_ENABLE_PERIODIC_ADV 1

/** @brief Un-comment to change the maximum number of periodically synced devices */
// #define CONFIG_BT_NIMBLE_MAX_PERIODIC_SYNCS 1

/****************************************************
 * END For use with ESP32C3, ESP32S3, ESP32H2 ONLY! *
 ***************************************************/


/** @brief Un-comment to change the default MTU size */
// #define CONFIG_BT_NIMBLE_ATT_PREFERRED_MTU 255

/** @brief Un-comment to change default device name */
// #define CONFIG_BT_NIMBLE_SVC_GAP_DEVICE_NAME "nimble"

/** @brief Un-comment to set the debug log messages level from the NimBLE host stack.\n
 *  Values: 0 = DEBUG, 1 = INFO, 2 = WARNING, 3 = ERROR, 4 = CRITICAL, 5+ = NONE\n
 *  Uses approx. 32kB of flash memory.
 */
 // #define CONFIG_BT_NIMBLE_LOG_LEVEL 5

 /** @brief Un-comment to set the debug log messages level from the NimBLE CPP Wrapper.\n
 *  Values: 0 = NONE, 1 = ERROR, 2 = WARNING, 3 = INFO, 4+ = DEBUG\n
 *  Uses approx. 32kB of flash memory.
 */
 // #define CONFIG_NIMBLE_CPP_LOG_LEVEL 0

/** @brief Un-comment to see NimBLE host return codes as text debug log messages.
 *  Uses approx. 7kB of flash memory.
 */
// #define CONFIG_NIMBLE_CPP_ENABLE_RETURN_CODE_TEXT

/** @brief Un-comment to see GAP event codes as text in debug log messages.
 *  Uses approx. 1kB of flash memory.
 */
// #define CONFIG_NIMBLE_CPP_ENABLE_GAP_EVENT_CODE_TEXT

/** @brief Un-comment to see advertisement types as text while scanning in debug log messages.
 *  Uses approx. 250 bytes of flash memory.
 */
// #define CONFIG_NIMBLE_CPP_ENABLE_ADVERTISEMENT_TYPE_TEXT

/** @brief Un-comment to change the default GAP appearance */
// #define CONFIG_BT_NIMBLE_SVC_GAP_APPEARANCE 0x0

 /** @brief Un-comment if not using NimBLE Client functions \n
 *  Reduces flash size by approx. 7kB.
 */
// #define CONFIG_BT_NIMBLE_ROLE_CENTRAL_DISABLED

/** @brief Un-comment if not using NimBLE Scan functions \n
 *  Reduces flash size by approx. 26kB.
 */
// #define CONFIG_BT_NIMBLE_ROLE_OBSERVER_DISABLED

/** @brief Un-comment if not using NimBLE Server functions \n
 *  Reduces flash size by approx. 16kB.
 */
// #define CONFIG_BT_NIMBLE_ROLE_PERIPHERAL_DISABLED

/** @brief Un-comment if not using NimBLE Advertising functions \n
 *  Reduces flash size by approx. 5kB.
 */
// #define CONFIG_BT_NIMBLE_ROLE_BROADCASTER_DISABLED

/** @brief Un-comment to change the number of devices allowed to store/bond with */
// #define CONFIG_BT_NIMBLE_MAX_BONDS 3

/** @brief Un-comment to change the maximum number of CCCD subscriptions to store */
// #define CONFIG_BT_NIMBLE_MAX_CCCDS 8

/** @brief Un-comment to change the random address refresh time (in seconds) */
// #define CONFIG_BT_NIMBLE_RPA_TIMEOUT 900

/**
 * @brief Un-comment to change the number of MSYS buffers available.
 * @details MSYS is a system level mbuf registry. For prepare write & prepare \n
 * responses MBUFs are allocated out of msys_1 pool. This may need to be increased if\n
 * you are sending large blocks of data with a low MTU. E.g: 512 bytes with 23 MTU will fail.
 */
// #define CONFIG_BT_NIMBLE_MSYS1_BLOCK_COUNT 12

/** @brief Un-comment to use external PSRAM for the NimBLE host */
// #define CONFIG_BT_NIMBLE_MEM_ALLOC_MODE_EXTERNAL 1

/** @brief Un-comment to change the core NimBLE host runs on */
// #define CONFIG_BT_NIMBLE_PINNED_TO_CORE 0

/** @brief Un-comment to change the stack size for the NimBLE host task */
// #define CONFIG_BT_NIMBLE_HOST_TASK_STACK_SIZE 4096

/**
 * @brief Un-comment to use memory pools for stack operations
 * @details this will use slightly more RAM but may provide more stability.
 */
// #define CONFIG_NIMBLE_STACK_USE_MEM_POOLS 1

/**********************************
 End Arduino user-config
**********************************/

#ifndef CONFIG_BT_NIMBLE_HOST_TASK_STACK_SIZE
#define CONFIG_BT_NIMBLE_HOST_TASK_STACK_SIZE 4096
#endif

#ifndef CONFIG_BT_NIMBLE_ROLE_CENTRAL // means for Tasmota: nimble was already embedded into the Arduino framework

/* This section should not be altered */
#ifndef CONFIG_BT_NIMBLE_ROLE_CENTRAL_DISABLED
#define CONFIG_BT_NIMBLE_ROLE_CENTRAL
#endif

#ifndef CONFIG_BT_NIMBLE_ROLE_OBSERVER_DISABLED
#define CONFIG_BT_NIMBLE_ROLE_OBSERVER
#endif

#ifndef CONFIG_BT_NIMBLE_ROLE_PERIPHERAL_DISABLED
#define CONFIG_BT_NIMBLE_ROLE_PERIPHERAL
#endif

#ifndef CONFIG_BT_NIMBLE_ROLE_BROADCASTER_DISABLED
#define CONFIG_BT_NIMBLE_ROLE_BROADCASTER
#endif

#ifndef CONFIG_BT_NIMBLE_PINNED_TO_CORE
#define CONFIG_BT_NIMBLE_PINNED_TO_CORE 0
#endif

#ifndef CONFIG_BT_NIMBLE_MEM_ALLOC_MODE_EXTERNAL
#define CONFIG_BT_NIMBLE_MEM_ALLOC_MODE_INTERNAL 1
#endif

#ifndef CONFIG_BT_NIMBLE_MAX_CONNECTIONS
#define CONFIG_BT_NIMBLE_MAX_CONNECTIONS 3
#endif

#ifndef CONFIG_BT_NIMBLE_MAX_BONDS
#define CONFIG_BT_NIMBLE_MAX_BONDS 3
#endif

#ifndef CONFIG_BT_NIMBLE_MAX_CCCDS
#define CONFIG_BT_NIMBLE_MAX_CCCDS 8
#endif

#ifndef CONFIG_BT_NIMBLE_SVC_GAP_DEVICE_NAME
#define CONFIG_BT_NIMBLE_SVC_GAP_DEVICE_NAME "nimble"
#endif

#ifndef CONFIG_BT_NIMBLE_ATT_PREFERRED_MTU
#define CONFIG_BT_NIMBLE_ATT_PREFERRED_MTU 255
#endif

#ifndef CONFIG_BT_NIMBLE_SVC_GAP_APPEARANCE
#define CONFIG_BT_NIMBLE_SVC_GAP_APPEARANCE 0x0
#endif

#ifndef CONFIG_BT_NIMBLE_MSYS1_BLOCK_COUNT
#define CONFIG_BT_NIMBLE_MSYS1_BLOCK_COUNT 12
#endif

#ifndef CONFIG_BT_NIMBLE_RPA_TIMEOUT
#define CONFIG_BT_NIMBLE_RPA_TIMEOUT 900
#endif

#ifndef CONFIG_BT_NIMBLE_LOG_LEVEL
#define CONFIG_BT_NIMBLE_LOG_LEVEL 5
#endif

#ifndef CONFIG_NIMBLE_STACK_USE_MEM_POOLS
#define CONFIG_NIMBLE_STACK_USE_MEM_POOLS 0
#endif

/** @brief Set if CCCD's and bond data should be stored in NVS */
#define CONFIG_BT_NIMBLE_NVS_PERSIST 0

/** @brief Allow legacy paring */
#define CONFIG_BT_NIMBLE_SM_LEGACY 1

/** @brief Allow BLE secure connections */
#define CONFIG_BT_NIMBLE_SM_SC 1

/** @brief Max device name length (bytes) */
#define CONFIG_BT_NIMBLE_GAP_DEVICE_NAME_MAX_LEN 31

/** @brief ACL Buffer count */
#define CONFIG_BT_NIMBLE_ACL_BUF_COUNT 12

/** @brief ACL Buffer size */
#define CONFIG_BT_NIMBLE_ACL_BUF_SIZE 255

/** @brief HCI Event Buffer size */
#if CONFIG_BT_NIMBLE_EXT_ADV || CONFIG_BT_NIMBLE_ENABLE_PERIODIC_ADV
#  define CONFIG_BT_NIMBLE_HCI_EVT_BUF_SIZE 257
#else
#  define CONFIG_BT_NIMBLE_HCI_EVT_BUF_SIZE 70
#endif

/** @brief Number of high priority HCI event buffers */
#define CONFIG_BT_NIMBLE_HCI_EVT_HI_BUF_COUNT 30

/** @brief Number of low priority HCI event buffers */
#define CONFIG_BT_NIMBLE_HCI_EVT_LO_BUF_COUNT 8

/** @brief Maximum number of connection oriented channels */
#define CONFIG_BT_NIMBLE_L2CAP_COC_MAX_NUM 0

#define CONFIG_BT_NIMBLE_HS_FLOW_CTRL 1
#define CONFIG_BT_NIMBLE_HS_FLOW_CTRL_ITVL 1000
#define CONFIG_BT_NIMBLE_HS_FLOW_CTRL_THRESH 2
#define CONFIG_BT_NIMBLE_HS_FLOW_CTRL_TX_ON_DISCONNECT 1

#define CONFIG_BT_NIMBLE_HS_STOP_TIMEOUT_MS 2000

#ifndef CONFIG_BT_ENABLED
#define CONFIG_BT_ENABLED
#endif

#ifndef CONFIG_BT_NIMBLE_ENABLED
#define CONFIG_BT_NIMBLE_ENABLED 1
#endif

#ifdef ESP_PLATFORM
#ifndef CONFIG_BTDM_CONTROLLER_MODE_BLE_ONLY
#define CONFIG_BTDM_CONTROLLER_MODE_BLE_ONLY
#endif

#ifndef CONFIG_BTDM_SCAN_DUPL_TYPE_DEVICE
#define CONFIG_BTDM_SCAN_DUPL_TYPE_DEVICE 0
#endif

#ifndef CONFIG_BTDM_SCAN_DUPL_TYPE_DATA
#define CONFIG_BTDM_SCAN_DUPL_TYPE_DATA 1
#endif

#ifndef CONFIG_BTDM_SCAN_DUPL_TYPE_DATA_DEVICE
#define CONFIG_BTDM_SCAN_DUPL_TYPE_DATA_DEVICE 2
#endif

#if !defined(CONFIG_IDF_TARGET_ESP32) && !defined(CONFIG_IDF_TARGET_ESP32C3) && !defined(CONFIG_IDF_TARGET_ESP32C6) && !defined(CONFIG_IDF_TARGET_ESP32S3)
#define CONFIG_IDF_TARGET_ESP32 1
#endif

#if CONFIG_BT_NIMBLE_EXT_ADV || CONFIG_BT_NIMBLE_ENABLE_PERIODIC_ADV
#  if defined(CONFIG_IDF_TARGET_ESP32)
#    error Extended advertising is not supported on ESP32.
#  endif
#endif
#endif

#if CONFIG_BT_NIMBLE_ENABLE_PERIODIC_ADV && !CONFIG_BT_NIMBLE_EXT_ADV
#  error Extended advertising must be enabled to use periodic advertising.
#endif

/* Must have max instances and data length set if extended advertising is enabled */
#if CONFIG_BT_NIMBLE_EXT_ADV
#  if !defined(CONFIG_BT_NIMBLE_MAX_EXT_ADV_INSTANCES)
#    define CONFIG_BT_NIMBLE_MAX_EXT_ADV_INSTANCES 1
#  endif
#  if !defined(CONFIG_BT_NIMBLE_MAX_EXT_ADV_DATA_LEN)
#    define CONFIG_BT_NIMBLE_MAX_EXT_ADV_DATA_LEN 251
#  endif
#endif

/* Must set max number of syncs if periodic advertising is enabled */
#if CONFIG_BT_NIMBLE_ENABLE_PERIODIC_ADV && !defined(CONFIG_BT_NIMBLE_MAX_PERIODIC_SYNCS)
#  define CONFIG_BT_NIMBLE_MAX_PERIODIC_SYNCS 1
#endif

/* Cannot use client without scan */
#if defined(CONFIG_BT_NIMBLE_ROLE_CENTRAL) && !defined(CONFIG_BT_NIMBLE_ROLE_OBSERVER)
#define CONFIG_BT_NIMBLE_ROLE_OBSERVER
#endif

/* Cannot use server without advertise */
#if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL) && !defined(CONFIG_BT_NIMBLE_ROLE_BROADCASTER)
#define CONFIG_BT_NIMBLE_ROLE_BROADCASTER
#endif

#endif //CONFIG_BT_NIMBLE_ROLE_CENTRAL

/* Enables the use of Arduino String class for attribute values */
#if defined __has_include
#  if __has_include (<Arduino.h>)
#    define NIMBLE_CPP_ARDUINO_STRING_AVAILABLE
#  endif
#endif

