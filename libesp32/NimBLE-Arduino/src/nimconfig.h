/** @file 
 * 
 * IGNORE THIS FILE IF USING ESP-IDF, USE MENUCONFIG TO SET NIMBLE OPTIONS.
 *
 * The config options here are for Arduino use only.
 */
 
#pragma once

#include "sdkconfig.h"

/*
 * For ESP-IDF compatibility
 * Some versions of ESP-IDF used the config name format "CONFIG_NIMBLE_".
 * This converts them to "CONFIG_BT_NIMBLE_" format used in the latest IDF.
 */

/* Detect if using ESP-IDF or Arduino (Arduino won't have these defines in sdkconfig) */
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
 * Arduino config options start here
 **********************************************/

/** @brief Comment out if not using NimBLE Client functions \n
 *  Reduces flash size by approx. 7kB.
 */
#define CONFIG_BT_NIMBLE_ROLE_CENTRAL

/** @brief Comment out if not using NimBLE Scan functions \n
 *  Reduces flash size by approx. 26kB.
 */
#define CONFIG_BT_NIMBLE_ROLE_OBSERVER

/** @brief Comment out if not using NimBLE Server functions \n
 *  Reduces flash size by approx. 16kB.
 */
// #define CONFIG_BT_NIMBLE_ROLE_PERIPHERAL

/** @brief Comment out if not using NimBLE Advertising functions \n
 *  Reduces flash size by approx. 5kB.
 */
// #define CONFIG_BT_NIMBLE_ROLE_BROADCASTER

/*  Uncomment to see debug log messages from the NimBLE host
 *  Uses approx. 32kB of flash memory.
 */
// #define CONFIG_BT_NIMBLE_DEBUG

/*  Uncomment to see NimBLE host return codes as text debug log messages.
 *  Uses approx. 7kB of flash memory.
 */
// #define CONFIG_NIMBLE_CPP_ENABLE_RETURN_CODE_TEXT

/*  Uncomment to see GAP event codes as text in debug log messages.
 *  Uses approx. 1kB of flash memory.
 */
// #define CONFIG_NIMBLE_CPP_ENABLE_GAP_EVENT_CODE_TEXT

/*  Uncomment to see advertisment types as text while scanning in debug log messages.
 *  Uses approx. 250 bytes of flash memory.
 */
// #define CONFIG_NIMBLE_CPP_ENABLE_ADVERTISMENT_TYPE_TEXT

/** @brief Sets the core NimBLE host runs on */
#define CONFIG_BT_NIMBLE_PINNED_TO_CORE 0

/** @brief Sets the stack size for the NimBLE host task */
#define CONFIG_BT_NIMBLE_TASK_STACK_SIZE 4096

/**
 * @brief Sets the memory pool where NimBLE will be loaded
 * @details By default NimBLE is loaded in internal ram.\n
 * To use external PSRAM you must change this to `#define CONFIG_BT_NIMBLE_MEM_ALLOC_MODE_EXTERNAL 1`
 */
#define CONFIG_BT_NIMBLE_MEM_ALLOC_MODE_INTERNAL 1

/** @brief Sets the number of simultaneous connections (esp controller max is 9) */
#define CONFIG_BT_NIMBLE_MAX_CONNECTIONS 3

/** @brief Sets the number of devices allowed to store/bond with */
#define CONFIG_BT_NIMBLE_MAX_BONDS 3

/** @brief Sets the maximum number of CCCD subscriptions to store */
#define CONFIG_BT_NIMBLE_MAX_CCCDS 8

/** @brief Set if CCCD's and bond data should be stored in NVS */
#define CONFIG_BT_NIMBLE_NVS_PERSIST 0

/** @brief Allow legacy paring */
#define CONFIG_BT_NIMBLE_SM_LEGACY 1

/** @brief Allow BLE secure connections */
#define CONFIG_BT_NIMBLE_SM_SC 1

/** @brief Default device name */
#define CONFIG_BT_NIMBLE_SVC_GAP_DEVICE_NAME "nimble"

/** @brief Max device name length (bytes) */
#define CONFIG_BT_NIMBLE_GAP_DEVICE_NAME_MAX_LEN 31

/** @brief Default MTU size */
#define CONFIG_BT_NIMBLE_ATT_PREFERRED_MTU 256

/** @brief Default GAP appearance */
#define CONFIG_BT_NIMBLE_SVC_GAP_APPEARANCE 0x0

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

/**
 * @brief Sets the number of MSYS buffers available.
 * @details MSYS is a system level mbuf registry. For prepare write & prepare \n
 * responses MBUFs are allocated out of msys_1 pool. This may need to be increased if\n 
 * you are sending large blocks of data with a low MTU. E.g: 512 bytes with 23 MTU will fail. 
 */
#define CONFIG_BT_NIMBLE_MSYS1_BLOCK_COUNT 12


/** @brief Random address refresh time in seconds */
#define CONFIG_BT_NIMBLE_RPA_TIMEOUT 900

/** @brief Maximum number of connection oriented channels */
#define CONFIG_BT_NIMBLE_L2CAP_COC_MAX_NUM 0

/* These should not be altered */
#define CONFIG_BT_NIMBLE_HS_FLOW_CTRL 1
#define CONFIG_BT_NIMBLE_HS_FLOW_CTRL_ITVL 1000
#define CONFIG_BT_NIMBLE_HS_FLOW_CTRL_THRESH 2
#define CONFIG_BT_NIMBLE_HS_FLOW_CTRL_TX_ON_DISCONNECT 1

#ifndef CONFIG_BT_ENABLED
#define CONFIG_BT_ENABLED
#endif

#define CONFIG_BTDM_CONTROLLER_MODE_BLE_ONLY

#endif // #if defined(CONFIG_BT_NIMBLE_TASK_STACK_SIZE) || defined(CONFIG_NIMBLE_TASK_STACK_SIZE)

/**********************************
 End Arduino config
**********************************/


/* Cannot use client without scan */
#if defined(CONFIG_BT_NIMBLE_ROLE_CENTRAL) && !defined(CONFIG_BT_NIMBLE_ROLE_OBSERVER)
#define CONFIG_BT_NIMBLE_ROLE_OBSERVER
#endif

/* Cannot use server without advertise */
#if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL) && !defined(CONFIG_BT_NIMBLE_ROLE_BROADCASTER)
#define CONFIG_BT_NIMBLE_ROLE_BROADCASTER
#endif



#ifdef _DOXYGEN_
/** @brief Uncomment to see debug log messages from the NimBLE host \n
 *  Uses approx. 32kB of flash memory.
 */
#define CONFIG_BT_NIMBLE_DEBUG

/** @brief Uncomment to see NimBLE host return codes as text debug log messages. \n
 *  Uses approx. 7kB of flash memory.
 */
#define CONFIG_NIMBLE_CPP_ENABLE_RETURN_CODE_TEXT

/** @brief Uncomment to see GAP event codes as text in debug log messages. \n
 *  Uses approx. 1kB of flash memory.
 */
#define CONFIG_NIMBLE_CPP_ENABLE_GAP_EVENT_CODE_TEXT

/** @brief Uncomment to see advertisment types as text while scanning in debug log messages. \n
 *  Uses approx. 250 bytes of flash memory.
 */
#define CONFIG_NIMBLE_CPP_ENABLE_ADVERTISMENT_TYPE_TEXT
#endif // _DOXYGEN_
