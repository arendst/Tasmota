 #pragma once

#include "sdkconfig.h"
#include "nimconfig_rename.h"

/***********************************************
 * Arduino user-config options start here
 **********************************************/

/** @brief Un-comment to change the number of simultaneous connections (esp controller max is 9) */
// #define CONFIG_BT_NIMBLE_MAX_CONNECTIONS 3

/** @brief Un-comment to change the default MTU size */
// #define CONFIG_BT_NIMBLE_ATT_PREFERRED_MTU 255

/** @brief Un-comment to change default device name */
// #define CONFIG_BT_NIMBLE_SVC_GAP_DEVICE_NAME "nimble"

/** @brief Un-comment to see debug log messages from the NimBLE host
 *  Uses approx. 32kB of flash memory.
 */
// #define CONFIG_BT_NIMBLE_DEBUG

/** @brief Un-comment to see NimBLE host return codes as text debug log messages.
 *  Uses approx. 7kB of flash memory.
 */
// #define CONFIG_NIMBLE_CPP_ENABLE_RETURN_CODE_TEXT

/** @brief Un-comment to see GAP event codes as text in debug log messages.
 *  Uses approx. 1kB of flash memory.
 */
// #define CONFIG_NIMBLE_CPP_ENABLE_GAP_EVENT_CODE_TEXT

/** @brief Un-comment to see advertisment types as text while scanning in debug log messages.
 *  Uses approx. 250 bytes of flash memory.
 */
// #define CONFIG_NIMBLE_CPP_ENABLE_ADVERTISMENT_TYPE_TEXT

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
// #define CONFIG_BT_NIMBLE_TASK_STACK_SIZE 4096

/**********************************
 End Arduino user-config
**********************************/

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

#ifndef CONFIG_BT_NIMBLE_TASK_STACK_SIZE
#define CONFIG_BT_NIMBLE_TASK_STACK_SIZE 4096
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


/** @brief Set if CCCD's and bond data should be stored in NVS */
#define CONFIG_BT_NIMBLE_NVS_PERSIST 1

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
#define CONFIG_BT_NIMBLE_HCI_EVT_BUF_SIZE 70

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

#ifndef CONFIG_BT_ENABLED
#define CONFIG_BT_ENABLED
#endif

#ifndef CONFIG_BTDM_CONTROLLER_MODE_BLE_ONLY
#define CONFIG_BTDM_CONTROLLER_MODE_BLE_ONLY
#endif

/* Cannot use client without scan */
#if defined(CONFIG_BT_NIMBLE_ROLE_CENTRAL) && !defined(CONFIG_BT_NIMBLE_ROLE_OBSERVER)
#define CONFIG_BT_NIMBLE_ROLE_OBSERVER
#endif

/* Cannot use server without advertise */
#if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL) && !defined(CONFIG_BT_NIMBLE_ROLE_BROADCASTER)
#define CONFIG_BT_NIMBLE_ROLE_BROADCASTER
#endif

