#pragma once

#include "sdkconfig.h"
/** For ESP-IDF compatibility
 *
 *  Some versions of ESP-IDF used the config name format "CONFIG_NIMBLE_".
 *  This converts them to "CONFIG_BT_NIMBLE_" format used in the latest IDF.
 */
/* Detect if using ESP-IDF or Arduino (Arduino won't have these defines in sdkconfig)*/
#if defined(CONFIG_BT_NIMBLE_TASK_STACK_SIZE) || defined(CONFIG_NIMBLE_TASK_STACK_SIZE)

#if defined(CONFIG_NIMBLE_ENABLED) && !defined(CONFIG_BT_NIMBLE_ENABLED)
#define CONFIG_BT_NIMBLE_ENABLED
#endif

#if defined(CONFIG_NIMBLE_ROLE_OBSERVER) && !defined(CONFIG_BT_NIMBLE_ROLE_OBSERVER)
#define CONFIG_BT_NIMBLE_ROLE_OBSERVER
#endif

#if defined(CONFIG_NIMBLE_ROLE_BROADCASTER) && !defined(CONFIG_BT_NIMBLE_ROLE_BROADCASTER)
#define CONFIG_BT_NIMBLE_ROLE_BROADCASTER
#endif

#if defined(CONFIG_NIMBLE_ROLE_CENTRAL) && !defined(CONFIG_BT_NIMBLE_ROLE_CENTRAL)
#define CONFIG_BT_NIMBLE_ROLE_CENTRAL
#endif

#if defined(CONFIG_NIMBLE_ROLE_PERIPHERAL) && !defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)
#define CONFIG_BT_NIMBLE_ROLE_PERIPHERAL
#endif

#if defined(CONFIG_NIMBLE_DEBUG) && !defined(CONFIG_BT_NIMBLE_DEBUG)
#define CONFIG_BT_NIMBLE_DEBUG
#endif

#else // Using Arduino

/***********************************************
 * Arduino config options
 **********************************************/

/** Comment out if not using NimBLE Client functions
 *  Reduces flash size by approx. 7kB.
 */
#define CONFIG_BT_NIMBLE_ROLE_CENTRAL

/** Comment out if not using NimBLE Scan functions
 *  Reduces flash size by approx. 26kB.
 */
#define CONFIG_BT_NIMBLE_ROLE_OBSERVER

/** Comment out if not using NimBLE Server functions
 *  Reduces flash size by approx. 16kB.
 */
// #define CONFIG_BT_NIMBLE_ROLE_PERIPHERAL

/** Comment out if not using NimBLE Advertising functions
 *  Reduces flash size by approx. 5kB.
 */
// #define CONFIG_BT_NIMBLE_ROLE_BROADCASTER

/** Uncomment to see debug log messages from the NimBLE host
 *  Uses approx. 32kB of flash memory.
 */
// #define CONFIG_BT_NIMBLE_DEBUG

/** Uncomment to see NimBLE host return codes as text debug log messages.
 *  Uses approx. 7kB of flash memory.
 */
// #define CONFIG_NIMBLE_CPP_ENABLE_RETURN_CODE_TEXT

/** Uncomment to see GAP event codes as text in debug log messages.
 *  Uses approx. 1kB of flash memory.
 */
// #define CONFIG_NIMBLE_CPP_ENABLE_GAP_EVENT_CODE_TEXT

/** Uncomment to see advertisment types as text while scanning in debug log messages.
 *  Uses approx. 250 bytes of flash memory.
 */
// #define CONFIG_NIMBLE_CPP_ENABLE_ADVERTISMENT_TYPE_TEXT

/** Sets the core NimBLE host runs on */
#define CONFIG_BT_NIMBLE_PINNED_TO_CORE 0

/** Sets the stack size for the NimBLE host task */
#define CONFIG_BT_NIMBLE_TASK_STACK_SIZE 4096

/** Sets the number of simultaneous connections (esp controller max is 9) */
#define CONFIG_BT_NIMBLE_MAX_CONNECTIONS 3

/** Sets the number of devices allowed to store/bond with */
#define CONFIG_BT_NIMBLE_MAX_BONDS 3

/** Sets the number of CCCD's to store per bonded device */
#define CONFIG_BT_NIMBLE_MAX_CCCDS 8

#define CONFIG_BT_NIMBLE_NVS_PERSIST 0
#define CONFIG_BT_NIMBLE_SM_LEGACY 1
#define CONFIG_BT_NIMBLE_SM_SC 1
#define CONFIG_BT_NIMBLE_SVC_GAP_DEVICE_NAME "nimble"
#define CONFIG_BT_NIMBLE_GAP_DEVICE_NAME_MAX_LEN 31
#define CONFIG_BT_NIMBLE_ATT_PREFERRED_MTU 256
#define CONFIG_BT_NIMBLE_SVC_GAP_APPEARANCE 0x0
#define CONFIG_BT_NIMBLE_ACL_BUF_COUNT 12
#define CONFIG_BT_NIMBLE_ACL_BUF_SIZE 255
#define CONFIG_BT_NIMBLE_HCI_EVT_BUF_SIZE 70
#define CONFIG_BT_NIMBLE_HCI_EVT_HI_BUF_COUNT 30
#define CONFIG_BT_NIMBLE_HCI_EVT_LO_BUF_COUNT 8
#define CONFIG_BT_NIMBLE_MSYS1_BLOCK_COUNT 12
#define CONFIG_BT_NIMBLE_HS_FLOW_CTRL 1
#define CONFIG_BT_NIMBLE_HS_FLOW_CTRL_ITVL 1000
#define CONFIG_BT_NIMBLE_HS_FLOW_CTRL_THRESH 2
#define CONFIG_BT_NIMBLE_HS_FLOW_CTRL_TX_ON_DISCONNECT 1
#define CONFIG_BT_NIMBLE_RPA_TIMEOUT 900
#define CONFIG_BT_NIMBLE_L2CAP_COC_MAX_NUM 0

/** Do not comment out */
#ifndef CONFIG_BT_ENABLED
#define CONFIG_BT_ENABLED
#endif
#define CONFIG_BTDM_CONTROLLER_MODE_BLE_ONLY
#define CONFIG_BT_NIMBLE_MEM_ALLOC_MODE_INTERNAL

#endif // #if defined(CONFIG_BT_NIMBLE_TASK_STACK_SIZE) || defined(CONFIG_NIMBLE_TASK_STACK_SIZE)

/** Cannot use client without scan */
#if defined(CONFIG_BT_NIMBLE_ROLE_CENTRAL) && !defined(CONFIG_BT_NIMBLE_ROLE_OBSERVER)
#define CONFIG_BT_NIMBLE_ROLE_OBSERVER
#endif

/** Cannot use server without advertise */
#if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL) && !defined(CONFIG_BT_NIMBLE_ROLE_BROADCASTER)
#define CONFIG_BT_NIMBLE_ROLE_BROADCASTER
#endif
