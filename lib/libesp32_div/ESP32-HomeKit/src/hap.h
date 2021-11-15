/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2020 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS products only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef _HAP_H_
#define _HAP_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <esp_event.h>
#ifdef __cplusplus
extern "C" {
#endif

#define HAP_CHAR_STRING_MAX_LEN        256
#define HAP_CHAR_STRING_MAX_DEF_LEN    64

#define HAP_SUCCESS     0
#define HAP_FAIL        -1

typedef enum  {
    HAP_MFI_AUTH_NONE = 0,
    HAP_MFI_AUTH_HW,
    HAP_MFI_AUTH_SW
} hap_mfi_auth_type_t;

/** HomeKit Transports to be enabled */
typedef enum {
    /** Unknown. Default uninitialized value */
    HAP_TRANSPORT_UNKNOWN          = 0x00,
    /** HAP over Wi-Fi */
    HAP_TRANSPORT_WIFI             = 0x01,
    /** HAP Over Ethernet (Wi-Fi disabled).
     * To enable both Wi-Fi and Ethernet, use (HAP_TRANSPORT_WIFI | HAP_TRANSPORT_ETHERNET) for hap_init()
     */
    HAP_TRANSPORT_ETHERNET         = 0x02,
} hap_transport_t;

/**
 * HAP characteristic value formats
 */
typedef enum {
    /** No Type/Invalid type */
    HAP_CHAR_FORMAT_NONE = 0,
    /** Boolean (true or false)*/
	HAP_CHAR_FORMAT_BOOL,
    /** Unsigned 8-bit integer */
	HAP_CHAR_FORMAT_UINT8,
    /** Unsigned 16-bit integer */
	HAP_CHAR_FORMAT_UINT16,
    /** Unsigned 32-bit integer */
	HAP_CHAR_FORMAT_UINT32,
    /** Unsigned 64-bit integer */
    HAP_CHAR_FORMAT_UINT64,
    /** Signed 32-bit integer */
	HAP_CHAR_FORMAT_INT,
    /** Signed 32-bit floating point number */
	HAP_CHAR_FORMAT_FLOAT,
    /** Unicode string */
	HAP_CHAR_FORMAT_STRING,
    /** Base64 encoded set of one or more TLV8s */
    HAP_CHAR_FORMAT_TLV8,
    /** base64 encoded data blob */
    HAP_CHAR_FORMAT_DATA,
} hap_char_format_t;

/** Data value */
typedef struct {
    /** Pointer to an allocated buffer holding the data. This should remain valid
     * throughout the lifetime of the characteristic.
     */
    uint8_t *buf;
    /** Length of the data */
    uint32_t buflen;
} hap_data_val_t;

/** TLV8 value (Same as \ref hap_data_val_t) */
typedef hap_data_val_t hap_tlv8_val_t;

/** HAP Value */
typedef union {
    /** Boolean */
	bool b;
    /** Unsigned integer (uint8, uint16, uint32) */
    uint32_t u;
    /** Signed integer */
	int i;
    /** 64 bit Unsigned integer */
    uint64_t i64;
    /** Floating point number */
	float f;
    /** Pointer to string */
	char *s;
    /** Structure for Data */
    hap_data_val_t d;
    /** Structure for TLV8 */
    hap_tlv8_val_t t;
} hap_val_t;

/** Information about the Provisioned Network to which the accessory will connect */
typedef struct {
    /** SSID for the network */
    uint8_t ssid[64];
    /** Length of the SSID */
    size_t ssid_len;
    /** Password for the network. Can be NULL for Open networks */
    uint8_t password[64];
    /** Length of the Password. Can be 0 for Open networks */
    size_t password_len;
} hap_provisioned_nw_t;

/**
 * HAP characteristics unit types
 */
#define HAP_CHAR_UNIT_CELSIUS       "celsius"
#define HAP_CHAR_UNIT_PERCENTAGE    "percentage"
#define HAP_CHAR_UNIT_ARCDEGREES    "arcdegrees"
#define HAP_CHAR_UNIT_LUX           "lux"
#define HAP_CHAR_UNIT_SECONDS       "seconds"

/** HAP Characteristic properties/permissions */
/** Characteristic supports Paired Read */
#define HAP_CHAR_PERM_PR        (1 << 0)
/** Characteristic supports Paired Write */
#define HAP_CHAR_PERM_PW        (1 << 1)
/** Characteristic supports Event Notifications */
#define HAP_CHAR_PERM_EV        (1 << 2)
/** Characteristic supports Additional Authorization Data */
#define HAP_CHAR_PERM_AA        (1 << 3)
/** Characteristic supports Timed Writes */
#define HAP_CHAR_PERM_TW        (1 << 4)
/** Characteristic is hidden from the user */
#define HAP_CHAR_PERM_HD        (1 << 5)
/** Characterictic is of special read only type.
 * Valid for characteristics like "Programmable Switch Event" wherein the value
 * needs to be reported as NULL for a read, even when it is a uint8. Actual value
 * is reported only using event notifications.
 */
#define HAP_CHAR_PERM_SPECIAL_READ    (1 << 6)

/** HAP object handle */
typedef size_t                  hap_handle_t;

/** HAP Characteristic handle */
typedef hap_handle_t            hap_char_t;
/** HAP Service handle */
typedef hap_handle_t            hap_serv_t;
/** HAP Accessory handle */
typedef hap_handle_t            hap_acc_t;

/** HAP Status codes as per the HAP Specifications */
typedef enum {
    /** This specifies a success for the request. */
    HAP_STATUS_SUCCESS = 0,
    /** Request denied due to insufficient privileges. */
	HAP_STATUS_NO_PRIVILEGE = -70401,
    /** Unable to communicate with requested service. */
	HAP_STATUS_COMM_ERR = -70402,
    /** Resource is busy, try again. */
	HAP_STATUS_RES_BUSY = -70403,
    /** Cannot write to read only characteristic. */
	HAP_STATUS_WR_ON_RDONLY = -70404,
    /** Cannot read from a write only characteristic. */
	HAP_STATUS_RD_ON_WRONLY = -70405,
    /** Notification is not supported for characteristic */
	HAP_STATUS_NO_NOTIF = -70406,
    /** Out of resources to process request. */
	HAP_STATUS_OO_RES = -70407,
    /** Operation timed out. */
	HAP_STATUS_TIMEOUT = -70408,
    /** Resource does not exist. */
	HAP_STATUS_RES_ABSENT = -70409,
    /** Accessory received an invalid value in a write request. */
	HAP_STATUS_VAL_INVALID = -70410,
    /** Insufficient Authorization. */
	HAP_STATUS_INSUFFICIENT_AUTH = -70411,
} hap_status_t;

typedef enum {
    HAP_CID_NONE = 0,
    HAP_CID_OTHER,
    HAP_CID_BRIDGE,
    HAP_CID_FAN,
    HAP_CID_GARAGE_DOOR_OPENER,
    HAP_CID_LIGHTING,
    HAP_CID_LOCK,
    HAP_CID_OUTLET,
    HAP_CID_SWITCH,
    HAP_CID_THERMOSTAT,
    HAP_CID_SENSOR,
    HAP_CID_SECURITY_SYSTEM,
    HAP_CID_DOOR,
    HAP_CID_WINDOW,
    HAP_CID_WINDOW_COVERING,
    HAP_CID_PROGRAMMABLE_SWITCH,
    HAP_CID_RESERVED,
    HAP_CID_IP_CAMERA,
    HAP_CID_VIDEO_DOORBELL,
    HAP_CID_AIR_PURIFIER,
    HAP_CID_HEATER,
    HAP_CID_AIR_CONDITIONER,
    HAP_CID_HUMIDIFIER,
    HAP_CID_DEHUMIDIFIER,
    HAP_CID_MAX,
} hap_cid_t;

/** Prototype for Identify routine for the accessory
 *
 * @param[in] acc Handle of the accessory object
 */
typedef int (*hap_identify_routine_t) (hap_acc_t *acc);

/** HAP Accessory configuration */
typedef struct {
    /** Name (Mandatory) */
	const char *name;
    /** Model (Mandatory) */
	const char *model;
    /** Manufacturer (Mandatory) */
	const char *manufacturer;
    /** Serial Number (Mandatory) */
	const char *serial_num;
    /** Firmware Revision number in format x[.y[.z]] (Mandatory) */
	const char *fw_rev;
    /** Hardware revision number in format x[.y[.z]] (Optional. Can be NULL )*/
	const char *hw_rev;
    /** HAP Protocol version supported by the accessory. Should be set to "1.1"
     * @note The value set here will currently be ignored and assumed to be 1.1
     * as it is the only protocol version currently supported.
     * This is valid only for the Primary accessory.
     */
    const char *pv;
    /** Category Identifier for the Accessory. This is valid only for the
     * primary accessory
     */
    hap_cid_t cid;
    /** Identify routine for the accessory (Mandatory) */
	hap_identify_routine_t identify_routine;
} hap_acc_cfg_t;

/** HomeKit Debug prints level
 */
typedef enum {
    /** Basic information indicating what's happening in the HomeKit core */
    HAP_DEBUG_LEVEL_INFO = 0,
    /** Warnings for incidents that can affect behavior, but not necessarily cause a failure*/
    HAP_DEBUG_LEVEL_WARN,
    /** Errors which will affect the working, and may even halt the process */
    HAP_DEBUG_LEVEL_ERR,
    /** Asserts for failures which will definitely cause system to halt */
    HAP_DEBUG_LEVEL_ASSERT
} hap_debug_level_t;

/**
 * @brief Set the HomeKit Debug prints level.
 *
 * This indicates the maximum level for which the debug messages
 * will be printed. Default value is \ref HAP_DEBUG_LEVEL_INFO which means
 * that all information, warnings, errors and asserts will be printed, as
 * this is the lowest level. You can change it to any suitable higher level
 * to reduce the prints. For Eg. Setting this to HAP_DEBUG_LEVEL_WARN will
 * print all warnings, errors and asserts, but not the basic information.
 *
 * @param[in] level Maximum debug print level as per \ref hap_debug_level_t
 */
void hap_set_debug_level(hap_debug_level_t level);

/**
 *
 * @brief Get the HAP SDK Version from the hap core
 *
 * @return Pointer to a NULL terminated Version string (Eg. 2.0.r1-0136161)
 */
const char *hap_get_version();

/** Unique Paramaters for HomeKit */
typedef enum {
    /** Nothing unique.
     * Application code should ensure uniqueness of name and ssid.
     * HAP Core will not change anything.
     */
    UNIQUE_NONE = 0,
    /** Unique SSID (default configuration).
     * 3 bytes of MAC will be added to accessory name to create unique WAC SSID (Eg. Name-xxyyzz) */
    UNIQUE_SSID,
    /** Unique Name.
     * Name will be changed by adding 3 bytes of MAC to original name.
     * Thi swill automatically make the SSID unique as the accessory name will be used as SSID.
     */
    UNIQUE_NAME,
} hap_unique_param_t;

/** HomeKit Configuration.
 * Please do not change unless you understand the purpose of these */
typedef struct {
    /** Internal HomeKit Task's stack size */
    uint32_t task_stack_size;
    /** Internal HomeKit Task's priority */
    uint8_t task_priority;
    /** Maximum characteristics to which event notifications can be sent simultaneously.
     * Default value is enough for standalone accessories. Change may be required only
     * for bridges.
     */
    uint8_t max_event_notif_chars;
    /** Indicates what paramaters will be made unique by the HAP Core */
    hap_unique_param_t unique_param;
    /** Timeout (in seconds) after which a socket data receive call will return for a pair verified
     * socket, in case the call blocks. This has been added to avoid the webserver getting stalled on a read,
     * especially in case wherein some garbage data is received on a pair verified connection.
     */
    uint8_t recv_timeout;
    /** Timeout (in seconds) after which a socket data send call will return for a pair verified
     * socket, in case the call blocks. This has been added to avoid the webserver getting stalled on a write.
     */
    uint8_t send_timeout;
    /** Max length of the buffer to be used for handling SW Token Requests. The Default value has been kept
     * as 1200 to help pass HCA Tests. This can be changed to 512 for practical purposes as the actual Apple
     * tokens never really need more than that.
     */
    size_t sw_token_max_len;
    /** By default, config number (c#) incremenents on addition/removal of every bridged accessory after
     * hap_start(). Setting this flag to true will disable this. Use hap_update_config_number()
     * to increment c#. Note thar c# will still increment on a firmware upgrade though.
     */
    bool disable_config_num_update;
} hap_cfg_t;

/** Get HomeKit Configuration
 *
 * Gets the configuration that will be used by the HomeKit core.
 *
 * @param[out] cfg Pointer to an allocated \ref hap_cfg_t structure which will be populated
 * with the configuration information.
 *
 * @return HAP_SUCCESS on success
 * @return HAP_FAIL on error
 */
int hap_get_config(hap_cfg_t *cfg);

/** Set HomeKit Configuration
 *
 * This sets the HomeKit configuration. Please use hap_get_config() to get the old
 * configuration first, before modifying any values. Eg.
 *
 * @code
 *     hap_cfg_t hap_cfg;
 *     hap_get_config(&hap_cfg);
 *     hap_cfg.unique_param = UNIQUE_NAME;
 *     hap_set_config(&hap_cfg);
 * @endcode
 *
 * @note This should be called before hap_init().
 *
 * @param[in] cfg Pointer to an allocated \ref hap_cfg_t structure which has the new values
 * to set.
 *
 * @return HAP_SUCCESS on success
 * @return HAP_FAIL on error
 */
int hap_set_config(const hap_cfg_t *cfg);

/* Update config number (c#)
 *
 * This increments the config number (c#) by 1
 *
 * @return HAP_SUCCESS on success
 * @return HAP_FAIL on error
 */
int hap_update_config_number();

/**
 * @brief Initialize HAP Framework
 *
 * This internally initializes the MFi auth chip, TCP-IP stack, HomeKit Key Store,
 * HomeKit database, Wi-Fi and mDNS.
 *
 * @return HAP_SUCCESS on success
 * @return others on error
 */
int hap_init(hap_transport_t transport);

/**
 * @brief De-initialize HAP Framework.
 *
 * @note This currently does not do anything
 *
 * @return HAP_SUCCESS on success
 * @return others on error
 */
int hap_deinit();

/**
 * @brief Start the HAP framework
 *
 * This starts the webserver and also initializes WAC or HomeKit services
 * as per the state of the accessory.
 *
 * @return HAP_SUCCESS on success
 * @return others on error
 */
int hap_start(void);

/**
 * @brief Stop HAP framework and free all relevant resources
 *
 * @note This currently does not do anything
 *
 * @return HAP_SUCCESS on success
 * @return others on error
 */
int hap_stop(void);

/**
 * @brief Create a HAP accessory object
 *
 * @param[in] acc_cfg Pointer to the accessory configuration \ref hap_acc_cfg_t
 *
 * @return Handle for the accessory object pre-populated with
 * the Accessory Information Service (and Protocol Information Service
 * for the first accessory created)
 *
 * @return NULL on error
 */
hap_acc_t *hap_acc_create(hap_acc_cfg_t *acc_cfg);

/**
 * @brief Add Accessory flags to the Accessory Info service
 *
 * This adds the optional Accessory Information Flag to the given accessory,
 * with the value set to the one specified. Typically, the flags will be
 * initialised with a value of 1, meaning that additional manufacturer specific
 * setup is required. Once the setup is done, the flags should be reset to 0
 * using hap_acc_update_accessory_flags()
 *
 * @note Use of these flags requires explicit approval from Apple.
 *
 * @param[in] ha HAP Accessory object handle
 * @param[in] flags Initial value of flags. Check the specs for valid values
 *
 * @return HAP_SUCCESS on success
 * @return HAP_FAIL on failure
 */
int hap_acc_add_accessory_flags(hap_acc_t *ha, uint32_t flags);

/**
 * @brief Update the Accessory flags
 *
 * This updates the Accessory flags characteristic of the Accessory Info service
 * of the given accessory, if it has been added using hap_acc_add_accessory_flags().
 *
 * @param[in] ha HAP Accessory object handle
 * @param[in] flags New value of flags. Check the specs for valid values
 *
 * @return HAP_SUCCESS on success
 * @return HAP_FAIL on failure
 */
int hap_acc_update_accessory_flags(hap_acc_t *ha, uint32_t flags);

/**
 * @brief Add Product data
 *
 * As per HAP specs R15, this is the 8 byte product data assigned to the Product Plan.
 *
 * @param[in] ha HAP Accessory object handle
 * @param[in] product_data Pointer to the product data value buffer
 * @param[in] data_size Size of the buffer (should be 8 as per HAP Spec R15)
 *
 * @return HAP_SUCCESS on success
 * @return HAP_FAIL on failure
 */
int hap_acc_add_product_data(hap_acc_t *ha, uint8_t *product_data, size_t data_size);

/**
 * @brief Add accessory to the HAP Object database
 *
 * @param[in] ha HAP Accessory object handle
 */
void hap_add_accessory(hap_acc_t *ha);

/**
 * @brief Add a bridged accessory to the HAP Object database
 *
 * This adds a bridged accessory to the Homekit Object database and assigns
 * it the given AID. HomeKit requires that each accessory gets assigned a unique aid.
 * To ensure this, a helper API hap_get_unique_aid() has been provided. This API
 * requires some unique identifier of the accessory, say mac address, serial number, etc.
 *
 * Example: hap_add_bridged_accessory(ha, hap_get_unique_aid(mac_addr));
 *
 * @param[in] aid The AID desired for the bridged accessory. If a 0 is passed, the
 * next available AID will be assigned. However, this should be used only for
 * evaluation, not production.
 *
 * @param[in] ha HAP Accessory object handle
 */
void hap_add_bridged_accessory(hap_acc_t *ha, int aid);

/**
 * @brief Remove a bridged accessory from the HAP Object database
 *
 * @param[in] ha HAP Accessory object handle
 */
void hap_remove_bridged_accessory(hap_acc_t *ha);

/**
 * @brief Delete HAP Accessory Object
 *
 * @note Primary accessory cannot be deleted
 *
 * @param[in] ha HAP Accessory object handle
 *
 */
void hap_acc_delete(hap_acc_t *ha);

/**
 * @brief Delete all accessories
 */
void hap_delete_all_accessories(void);

/**
 * Get unique AID for the given identifier
 *
 * This API can be used to ensure that the same AID gets assigned to an accessory
 * every time, even across reboots and irrespective of when it is added to the
 * HomeKit database. It requires a unique identifier of the accessory, say a
 * mac addtess, serial number, etc. If it is a new accessory, a new AID gets
 * assigned and the id:aid combination gets stored in NVS (Non Volatile Storage).
 * If the accessory was already added some time later, the original AID is fetched
 * from the NVS and returned. This API should be used as the "aid" argument for
 * hap_add_bridged_accessory()
 *
 * @param[in] id Unique identifier for the accessory
 *
 * @return Unique AID for the accessory
 * @return -1 on error (NULL id)
 */
int hap_get_unique_aid(const char *id);

/**
 * @brief Get Accessory using AID
 *
 * @param[in] aid Accessory ID of the required accessory
 *
 * @return Handle for the accessory with given aid
 * @return NULL if accessory not found
 */
hap_acc_t *hap_acc_get_by_aid(int32_t aid);

/**
 * @brief Get first accessory in the database
 *
 * @return Handle for the first accessory
 */
hap_acc_t *hap_get_first_acc();

/**
 * @brief Get next accessory in the database
 *
 * @param[in] ha Current HAP Accessory object handle
 *
 * @return Pointer to the next accessory
 * @return NULL if the current accessory is the last
 */
hap_acc_t *hap_acc_get_next(hap_acc_t *ha);

/**
 * @brief Get Service using IID
 *
 * @param[in] ha HAP Accessory object handle in which the service should be searched
 * @param[in] iid Instance ID of the required service
 *
 * @return Handle for the service with given iid
 * @return NULL if service not found
 */
hap_serv_t *hap_acc_get_serv_by_iid(hap_acc_t *ha, int32_t iid);

/**
 * @brief Get Service using Type UUID
 *
 * @param[in] ha HAP Accessory object handle in which the service should be searched
 * @param[in] type_uuid UUID of the required service
 *
 * @return Handle for the service with given type_uuid
 * @return NULL if service not found
 */
hap_serv_t *hap_acc_get_serv_by_uuid(hap_acc_t *ha, const char *type_uuid);

/**
 * @brief Get characteristic using IID
 *
 * @param[in] ha HAP Accessory object handle in which the characteristic should be searched
 * @param[in] iid Instance ID of the required characteristic
 *
 * @return Handle for the characteristic with given iid
 * @return NULL if characteristic not found
 */
hap_char_t *hap_acc_get_char_by_iid(hap_acc_t *ha, int32_t iid);

/**
 * @brief Get first service in a given accessory object
 *
 * This is useful for iterating over all services of an accessory.
 * The subsequent services of the accessory can be iterated by using
 * hap_serv_get_next().
 *
 * @param[in] ha HAP Accessory object handle
 *
 * @return Handle for the first service in the accessory object
 */
hap_serv_t *hap_acc_get_first_serv(hap_acc_t *ha);

/**
 * @brief Create a Boolean Characteristic Object
 *
 * @param[in] type_uuid UUID for the characteristic as per the HAP Specs
 * @param[in] perms Logically OR of the various permissions supported by the characteristic
 * @param[in] val Initial value of the characteristic
 *
 * @return Handle for the characteristic object created
 * @return NULL on error
 */
hap_char_t *hap_char_bool_create(const char *type_uuid, uint16_t perms, bool val);

/**
 * @brief Create an 8-bit unsigned integer Characteristic Object
 *
 * @param[in] type_uuid UUID for the characteristic as per the HAP Specs
 * @param[in] perms Logically OR of the various permissions supported by the characteristic
 * @param[in] val Initial value of the characteristic
 *
 * @return Handle for the characteristic object created
 * @return NULL on error
 */
hap_char_t *hap_char_uint8_create(const char *type_uuid, uint16_t perms, uint8_t val);

/**
 * @brief Create a 16-bit unsigned integer Characteristic Object
 *
 * @param[in] type_uuid UUID for the characteristic as per the HAP Specs
 * @param[in] perms Logically OR of the various permissions supported by the characteristic
 * @param[in] val Initial value of the characteristic
 *
 * @return Handle for the characteristic object created
 * @return NULL on error
 */
hap_char_t *hap_char_uint16_create(const char *type_uuid, uint16_t perms, uint16_t val);

/**
 * @brief Create a 32-bit unsigned integer Characteristic Object
 *
 * @param[in] type_uuid UUID for the characteristic as per the HAP Specs
 * @param[in] perms Logically OR of the various permissions supported by the characteristic
 * @param[in] val Initial value of the characteristic
 *
 * @return Handle for the characteristic object created
 * @return NULL on error
 */
hap_char_t *hap_char_uint32_create(const char *type_uuid, uint16_t perms, uint32_t val);

/**
 * @brief Create a 64-bit unsigned integer Characteristic Object
 *
 * @param[in] type_uuid UUID for the characteristic as per the HAP Specs
 * @param[in] perms Logically OR of the various permissions supported by the characteristic
 * @param[in] val Initial value of the characteristic
 *
 * @return Handle for the characteristic object created
 * @return NULL on error
 */
hap_char_t *hap_char_uint64_create(const char *type_uuid, uint16_t perms, uint64_t val);

/**
 * @brief Create a 32-bit signed integer Characteristic Object
 *
 * @param[in] type_uuid UUID for the characteristic as per the HAP Specs
 * @param[in] perms Logically OR of the various permissions supported by the characteristic
 * @param[in] val Initial value of the characteristic
 *
 * @return Handle for the characteristic object created
 * @return NULL on error
 */
hap_char_t *hap_char_int_create(const char *type_uuid, uint16_t perms, int val);

/**
 * @brief Create a Floating point Characteristic Object
 *
 * @param[in] type_uuid UUID for the characteristic as per the HAP Specs
 * @param[in] perms Logically OR of the various permissions supported by the characteristic
 * @param[in] val Initial value of the characteristic
 *
 * @return Handle for the characteristic object created
 * @return NULL on error
 */
hap_char_t *hap_char_float_create(const char *type_uuid, uint16_t perms, float val);

/**
 * @brief Create a String Characteristic Object
 *
 * @param[in] type_uuid UUID for the characteristic as per the HAP Specs
 * @param[in] perms Logically OR of the various permissions supported by the characteristic
 * @param[in] val Initial value of the characteristic
 *
 * @return Handle for the characteristic object created
 * @return NULL on error
 */
hap_char_t *hap_char_string_create(const char *type_uuid, uint16_t perms, const char *val);

/**
 * @brief Create a Data Characteristic Object
 *
 * @param[in] type_uuid UUID for the characteristic as per the HAP Specs
 * @param[in] perms Logically OR of the various permissions supported by the characteristic
 * @param[in] val Pointer to initial value of the characteristic
 *
 * @return Handle for the characteristic object created
 * @return NULL on error
 */
hap_char_t *hap_char_data_create(const char *type_uuid, uint16_t perms, hap_data_val_t *val);

/**
 * @brief Create a TLV8 Characteristic Object
 *
 * @param[in] type_uuid UUID for the characteristic as per the HAP Specs
 * @param[in] perms Logically OR of the various permissions supported by the characteristic
 * @param[in] val Pointer to initial value of the characteristic
 *
 * @return Handle for the characteristic object created
 * @return NULL on error
 */
hap_char_t *hap_char_tlv8_create(const char *type_uuid, uint16_t perms, hap_tlv8_val_t *val);

/**
 * @brief Delete a characteristic object
 *
 * @param[in] hc HAP Characteristic Object handle
 */
void hap_char_delete(hap_char_t *hc);

/**
 * @brief Create a HAP Service Object
 *
 * @param[in] type_uuid UUID for the service as per the HAP Specs
 *
 * @return Handle for the service object created
 * @return NULL on error
 */
hap_serv_t *hap_serv_create(const char *type_uuid);

/**
 * @brief Delete a service object
 *
 * @param[in] hs HAP Service Object handle
 */
void hap_serv_delete(hap_serv_t *hs);

/**
 * @brief Get the AID for the given accessory
 *
 * @param[in] ha HAP Accessory Object handle
 *
 * @return aid for the accessory
 */
uint32_t hap_acc_get_aid(hap_acc_t *ha);

/**
 * @brief Get Characteristic using Type UUID
 *
 * @param[in] hs HAP Service object handle in which the characteristic should be searched
 * @param[in] type_uuid UUID of the required characteristic
 *
 * @return Handle for the characteristic with given type_uuid
 * @return NULL if characteristic not found
 */
hap_char_t *hap_serv_get_char_by_uuid(hap_serv_t *hs, const char *type_uuid);

/**
 * @brief Get parent Accessory for given Service
 *
 * @param[in] hs HAP Service object handle
 *
 * @return Handle for the parent accessory
 * @return NULL if the service is not added in any accessory
 */
hap_acc_t *hap_serv_get_parent(hap_serv_t *hs);

/**
 * @brief Get the next service in a given accessory object
 *
 * This is useful for iterating over all services of an accessory.
 *
 * @param[in] Current service object handle
 *
 * @return Handle for next service in the accessory object
 * @return NULL if the current service was the last one in the accessory
 */
hap_serv_t *hap_serv_get_next(hap_serv_t *hs);

/**
 * @brief Get first characteristic in a given service object
 *
 * This is useful for iterating over all characteristics of a service.
 * The subsequent characteristics of the service can be
 * iterated by using hap_char_get_next().
 *
 * @param[in] hs HAP Service object handle
 *
 * @return Handle for the first chatracteristic in the service object
 */
hap_char_t *hap_serv_get_first_char(hap_serv_t *hs);

/**
 * @brief Get the IID for the given characteristic
 *
 * @param[in] hc HAP Characteristic Object handle
 *
 * @return iid for the characteristic
 */
uint32_t hap_char_get_iid(hap_char_t *hc);

/**
 * @brief Get the type UUID for the given characteristic
 *
 * @param[in] hc HAP Characteristic Object handle
 *
 * @return Type UUID for the characteristic
 */
const char * hap_char_get_type_uuid(hap_char_t *hc);


/**
 * @brief Get the Permissions for the given characteristic
 *
 * @param[in] hc HAP Characteristic Object handle
 *
 * @return The permissions for the characteristics, the same as the ones passed while creating it.
 * It will be a logically OR of the various permissions supported by the characteristic.
 */
uint16_t hap_char_get_perm(hap_char_t *hc);


/**
 * @brief Get the format for the given characteristic
 *
 * @param[in] hc HAP Characteristic Object handle
 *
 * @return Format of the characteristic value
 */
hap_char_format_t hap_char_get_format(hap_char_t *hc);

/**
 * @brief Get the IID for the given service
 *
 * @param[in] hs HAP Service Object handle
 *
 * @return iid for the service
 */
uint32_t hap_serv_get_iid(hap_serv_t *hs);

/**
 * @brief Get the type UUID for the given service
 *
 * @param[in] hs HAP Service Object handle
 *
 * @return Type UUID for the service
 */
const char *hap_serv_get_type_uuid(hap_serv_t *hs);
/**
 * @brief Get parent Service for given Characteristic
 *
 * @param[in] hc HAP Characteristic object handle
 *
 * @return Handle for the parent service
 * @return NULL if the characteristic is not added in any service
 */
hap_serv_t *hap_char_get_parent(hap_char_t *hc);

/**
 * @brief Get the next characteristic in a given service object
 *
 * This is useful for iterating over all characteristics of a service.
 *
 * @param[in] Current characteristic object handle
 *
 * @return Handle for next characteristic in the service object
 * @return NULL if the current characteristic was the last one in the service
 */
hap_char_t *hap_char_get_next(hap_char_t *hc);

/**
 * @brief Add Integer characteristic constraints
 *
 * @param[in] hc HAP Characteristic Object handle
 * @param[in] min Minimum Value
 * @param[in] max Maximum Value
 * @param[in] step Step Value
 */
void hap_char_int_set_constraints(hap_char_t *hc, int min, int max, int step);

/**
 * @brief Add Integer characteristic constraints
 *
 * @param[in] hc HAP Characteristic Object handle
 * @param[in] min Minimum Value
 * @param[in] max Maximum Value
 * @param[in] step Step Value
 */
void hap_char_float_set_constraints(hap_char_t *hc, float min, float max, float step);

/**
 * @brief Add String characteristic constraints
 *
 * @param[in] hc HAP Characteristic Object handle
 * @param[in] maxlen Maximum Length of the string (default: 64, max: 256)
 */
void hap_char_string_set_maxlen(hap_char_t *hc, int maxlen);

/**
 * @brief Add Characteristic Description
 *
 * @param[in] hc HAP Characteristic Object handle
 * @param[in] description Manufacturer defined String description for the characteristic
 */
void hap_char_add_description(hap_char_t *hc, const char *description);

/**
 * @brief Add Characteristic Unit
 *
 * @param[in] hc HAP Characteristic Object handle
 * @param[in] unit Unit for the characteristic. Please see specs for valid strings.
 */
void hap_char_add_unit(hap_char_t *hc, const char *unit);

/**
 * @brief Add Valid Values for Characteristic
 *
 * Using this API will just add the valid-values metadata for a characteristic
 *
 * @param[in] hc HAP Characteristic Object handle
 * @param[in] valid_vals Pointer to an array of valid values
 * @param[in] valid_val_cnt Number of entries in the array
 */
void hap_char_add_valid_vals(hap_char_t *hc, const uint8_t *valid_vals, size_t valid_val_cnt);

/**
 * @brief Add Valid Values Range for Characteristic
 *
 * Using this API will just add the valid-values-range metadata for a characteristic
 *
 * @param[in] hc HAP Characteristic Object handle
 * @param[in] start_val Start value of the range
 * @param[in] end_val End value of the range
 */
void hap_char_add_valid_vals_range(hap_char_t *hc, uint8_t start_val, uint8_t end_val);
/**
 * @brief Set IID for a given characteristic
 *
 * HomeKit specifications require that the IID for a given characteristic should remain the same,
 * even after a firmware upgrade. Since the HomeKit core assigns the IIDs internally,
 * it is possible that a different IID is getting assigned because of removal of
 * some old service/characteristic or addition of a newer one in between. In such a case,
 * in order to maintain the same IID, this API can be used.
 *
 * @note This must be used only if actually required, and that too after adding all services/
 * characteristics to an accessory, but before hap_start().
 *
 * @param[in] hc HAP Characteristic Object Handle
 * @param[in] iid Desired IID
 */
void hap_char_set_iid(hap_char_t *hc, int32_t iid);

/**
 * @brief Add a characteristic to a service
 *
 * @param[in] hs HAP service object handle
 * @param[in] hc HAP characteristic object handle
 *
 * @return 0 on success
 * @return other on error
 */
int hap_serv_add_char(hap_serv_t *hs, hap_char_t *hc);

/**
 * @brief HAP add a service to an accessory
 *
 * @param[in] ha HAP Accessory object handle
 * @param[in] hs HAP Service object handle
 *
 * @return 0 on success
 * @return other on error
 */
int hap_acc_add_serv(hap_acc_t *ha, hap_serv_t *hs);

/**
 * @brief Update characteristic value
 *
 * This should be called within service read/write callbacks to update the
 * value of characteristic maintained by the HAP Core. This can also be called
 * from some other thread, for accessories like sensors that periodically
 * monitor some paramters.
 *
 * @param[in] hc HAP characteristic object handle
 * @param[in] val Pointer to new value
 *
 * @return 0 on success
 * @return other on error
 */
int hap_char_update_val(hap_char_t *hc, hap_val_t *val);

/**
 * @brief Get the current value of characteristic
 *
 * @param[in] hc HAP characteristic object handle
 *
 * @return Pointer to the current value
 */
const hap_val_t *hap_char_get_val(hap_char_t *hc);

/** Authorization Data received in a write reqest
 */
typedef struct {
    /** Pointer to the data. Will be NULL if no auth data was present */
    uint8_t *data;
    /** Length of the data. Will be 0 if no auth data was present */
    int len;
} hap_auth_data_t;

/** Write data for a characteristics received over HAP
 */
typedef struct {
    /** Pointer to the characteristic object */
    hap_char_t *hc;
    /** Value received in the write request.
     * Appropriate value in the \ref hap_val_t union will be set as per the format.
     */
    hap_val_t val;
    /** Authorization data \ref hap_auth_data_t id any, received in the write request.
     * It is the application's responsibility to handle and validate this data and
     * report the status accordingly.
     */
    hap_auth_data_t auth_data;
    /** Indicates if the received request was a remote write
     */
    bool remote;
    /** This is an output parameter, which should be set in the service write
     * routine as per the status of the write.
     */
    hap_status_t *status;
} hap_write_data_t;

/** Read data for a characteristic
 */
typedef struct {
    /** Pointer to the characteristic object */
    hap_char_t *hc;
    /** This is an output parameter, which should be set in the service read
     * routine as per the status of the read
     */
    hap_status_t *status;
} hap_read_data_t;


/** Service Write Function Prototype
 *
 * A function with this prototype must be registered with the HAP framework to handle
 * writes for the characteristics in the given service
 *
 * @note For writes, an array is sent instead of individual writes to avoid cases wherein
 * writes to multiple properties of a single accessory can cause unexpected behavior.
 * Eg. For a light which is off, a write could ask it to be turned on at 50% brightness,
 * with blue color. If 3 writes are issued for this, the light could first turn on to
 * 100% with white color, then dim down to 50% and then change color. A single write will
 * ensure that the hardware control takes care of all these at the same time.
 *
 * @param[in] write_data Array of characteristic write objects of type \ref hap_write_data_t
 * which has relevant information like the characteristic object handle, new value received, etc.
 * @param[in] count Number of entries in the write_data array.
 * @param[in] serv_priv The private data for the service set using hap_serv_set_priv()
 * @param[in] write_priv Can be used with hap_is_req_admin()
 *
 * @return HAP_SUCCESS on success
 * @return HAP_FAIL if an error is encountered even for a single characteristic. Actual error
 * value must be reported in the status under \ref hap_write_data_t.
 */
typedef int (*hap_serv_write_t) (hap_write_data_t write_data[], int count,
		void *serv_priv, void *write_priv);

/** Service Read Function Prototype
 *
 * A function with this prototype must be registered with the HAP framework to handle
 * reads for the characteristics in the given service. The new value must be set
 * by invoking hap_char_update_val().
 *
 * @param[in] hc HAP Characteristic Object Handle.
 * @param[out] status_code Status of the read, to be populated by the callback with
 *  values from \ref hap_status_t.
 * @param[in] serv_priv The private data for the service set using hap_serv_set_priv()
 * @param[in] read_priv Can be used with hap_is_req_admin()
 *
 * @return HAP_SUCCESS on success
 * @return HAP_FAIL if an error is encountered while reading. Actual error
 * value must be reported in the status_code
 */
typedef int (*hap_serv_read_t) (hap_char_t *hc, hap_status_t *status_code,
		void *serv_priv, void *read_priv);

/** Service Bulk Read Function Prototype
 *
 * A function with this prototype can be registered with the HAP framework to handle
 * bulk reads for the characteristics in the given service. The new values must be set
 * by invoking hap_char_update_val() for each characteristic.
 *
 * @note This should be used only under special circumstances for cases like
 * bridges, wherein fetching individual values from bridged accessories could
 * be time consuming. If this is used, it will override the handler of
 * type \ref hap_serv_read_t registered for the same service.
 *
 * @param[in] read_data Array of characteristic write objects of type \ref hap_read_data_t
 * which has characteristic object handle and read status pointer.
 * @param[in] count Number of entries in the read_data array.
 * @param[in] serv_priv The private data for the service set using hap_serv_set_priv()
 * @param[in] read_priv Can be used with hap_is_req_admin()
 *
 * @return HAP_SUCCESS on success
 * @return HAP_FAIL if an error is encountered even for a single characteristic.
 * Actual error value must be reported in the status under \ref hap_read_data_t.
 */
typedef int (*hap_serv_bulk_read_t) (hap_read_data_t read_data[], int count,
        void *serv_priv, void *read_priv);

/**
 * @brief Register Service Write callback
 *
 * @param[in] hs HAP Service Object Handle
 * @param[in] write Callback of type \ref hap_serv_write_t
 */
void hap_serv_set_write_cb(hap_serv_t *hs, hap_serv_write_t write);

/**
 * @brief Register Service Read callback
 *
 * @param[in] hs HAP Service Object Handle
 * @param[in] read Callback of type \ref hap_serv_read_t
 */
void hap_serv_set_read_cb(hap_serv_t *hs, hap_serv_read_t read);

/**
 * @brief Register Service Bulk read callback
 *
 * @note This should be used only under special circumstances for cases like
 * bridges, wherein fetching individual values from bridged accessories could
 * be time consuming. If this is used, the callback registered using
 * @ref hap_serv_set_read_cb() will be overriden.
 *
 * @param[in] hs HAP Service Object Handle
 * @param[in] read Callback of type \ref hap_serv_bulk_read_t
 */
void hap_serv_set_bulk_read_cb(hap_serv_t *hs, hap_serv_bulk_read_t read);

/**
 * @brief Set service private data
 *
 * This will be available in the read/write callbacks
 *
 * @param[in] hs HAP Service Object Handle
 * @param[in] priv Private data for the service
 */
void hap_serv_set_priv(hap_serv_t *hs, void *priv);

/**
 * @brief Get Service private
 *
 * This will get the private data associated with the service, which
 * was set using hap_serv_set_priv().
 *
 * @param[in] hs HAP Service Object Handle
 *
 * @return Pointer to the private data (can be NULL)
 */
void *hap_serv_get_priv(hap_serv_t *hs);

/**
 * @brief Mark service as primary
 *
 * @param[in] hs HAP Service Object Handle
 */
void hap_serv_mark_primary(hap_serv_t *hs);

/**
 * @brief Mark service as hidden
 *
 * @param[in] hs HAP Service Object Handle
 */
void hap_serv_mark_hidden(hap_serv_t *hs);

/**
 * @brief Link a HomeKit service to other service
 *
 * @param[in] hs HAP Service Object Handle
 * @param[in] linked_serv HAP Service Object Handle of the service to be linked
 *
 * @return HAP_SUCCESS on success
 * @return HAP_FAIL on failure
 */
int hap_serv_link_serv(hap_serv_t *hs, hap_serv_t *linked_serv);

/**
 * @brief Set IID for a given sService
 *
 * HomeKit specifications require that the IID for a given service should remain the same,
 * even after a firmware upgrade. Since the HomeKit core assigns the IIDs internally,
 * it is possible that a different IID is getting assigned because of removal of
 * some old service/characteristic or addition of a newer one in between. In such a case,
 * in order to maintain the same IID, this API can be used.
 *
 * @note This must be used only if actually required, and that too after adding all services/
 * characteristics to a service, but before hap_start().
 *
 * @param[in] hs HAP Service Object Handle
 * @param[in] iid Desired IID
 */
void hap_serv_set_iid(hap_serv_t *hs, int32_t iid);

/** HomeKit Setup Information */
typedef struct {
    /** SRP Salt */
    uint8_t salt[16];
    /** SRP Verifier */
    uint8_t verifier[384];
} hap_setup_info_t;

/**
 * @brief Set the Setup info for HomeKit Pairing
 *
 * HomeKit Pairing/Accessory Setup uses a Setup Code (Also called Pairing PIN).
 * However, the specs recommend that, instead of storing the PIN on the accessory,
 * the salt and verifier for the PIN should be stored.
 *
 * This API should be used to provide this information.
 *
 * @param[in] setup_info Pointer to the Setup Information structure which has
 * the Salt and Verifier.
 *
 * @return HAP_SUCCESS on success
 * @return HAP_FAIL on failure (like setup_info being NULL);
 */
int hap_set_setup_info(const hap_setup_info_t *setup_info);

/**
 * @brief Set the setup code (i.e. pairing pin)
 *
 * @note Ideally, the PIN should not be present in the firmware. Instead,
 * the salt and verifier should be stored on the accessory and provided using
 * hap_set_setup_info(). This API should be used only for testing purposes.
 *
 * Using this API overrides the setup information provided by hap_set_setup_info().
 *
 * @param[in] setup_code NULL terminated Setup code of the format xxx-xx-xxx where each
 * x is a number
 */
void hap_set_setup_code(const char *setup_code);

/**
 * @brief Set the setup ID
 *
 * @param[in] setup_id NULL terminated Setup ID, which will be a 4 character
 * long alpha numeric string (with capital alphabets)
 *
 * @return HAP_SUCCESS on success
 * @return HAP_FAIL on failure
 */
int hap_set_setup_id(const char *setup_id);

/**
 * Check if the MFi authentication co-processor is connected
 *
 * @note This should be called before hap_init()
 *
 * @return HAP_SUCCESS if an MFi co-processor is detected
 * @return HAP_FAIL if no MFi co-processor is detected
 */
int hap_check_mfi_chip();

/**
 * @brief Reboot the accessory
 *
 * This closes all the active HomeKit sessions and reboots the accessory.
 * It is recommended to use this API instead of the standard platform reboot/restart
 * APIs, for cleaner operation.
 *
 * @note This is an asynchronous API (actual action executed in HomeKit core thread context)
 * and can be invoked even from interrupt context. The \ref HAP_EVENT_ACC_REBOOTING
 * event will be invoked after this.
 *
 * @return HAP_SUCCESS on success
 * @return HAP_FAIL on failure
 */
int hap_reboot_accessory();

/**
 * @brief Reset to Factory Settings
 *
 * This resets the accessory to factory settings and reboots it.
 * All pairing information, network information and accessory specific information
 * (like accessory ID, keys, config number, etc.) is erased. Even all application
 * data written to NVS partition is erased.
 *
 * @note This is an asynchronous API (actual action executed in HomeKit core thread context)
 * and can be invoked even from interrupt context. The \ref HAP_EVENT_ACC_REBOOTING
 * event will be invoked after this.
 *
 * @return HAP_SUCCESS on success
 * @return HAP_FAIL on failure
 */
int hap_reset_to_factory();

/**
 * @brief Reset the Network Credentials
 *
 * This resets only the network credntials (keeping the pairing and any other information
 * intact) and reboots the accessory.
 *
 * @note This is an asynchronous API (actual action executed in HomeKit core thread context)
 * and can be invoked even from interrupt context. The \ref HAP_EVENT_ACC_REBOOTING
 * event will be invoked after this.
 *
 * @return HAP_SUCCESS on success
 * @return HAP_FAIL on failure
 */
int hap_reset_network();

/**
 * @brief Reset HomeKit Data
 *
 * This resets all the data stored by the HomeKit Framework, which includes
 * all pairing information, network information and accessory specific information
 * (like accessory ID, keys, config number, etc.)
 *
 * @note This is an asynchronous API (actual action executed in HomeKit core thread context)
 * and can be invoked even from interrupt context. The \ref HAP_EVENT_ACC_REBOOTING
 * event will be invoked after this.
 *
 * @return HAP_SUCCESS on success
 * @return HAP_FAIL on failure
 */
int hap_reset_homekit_data();

/**
 * @brief Reset HomeKit Pairings
 *
 * This API resets the HomeKit Pairing Information and also the accessory specific
 * information (like accessory ID, keys, config number, etc.). The accessory id is
 * reset because else, the paired controllers would consider the accessory as paired,
 * as they would still have the keys for the given accessory id.
 *
 * @note This is an asynchronous API (actual action executed in HomeKit core thread context)
 * and can be invoked even from interrupt context. The \ref HAP_EVENT_ACC_REBOOTING
 * event will be invoked after this.
 *
 * @return HAP_SUCCESS on success
 * @return HAP_FAIL on failure
 */
int hap_reset_pairings();

/**
 * @brief Check if the request is from Admin Controller
 *
 * This API can be used inside service read (\ref hap_serv_read_t) and service write
 * (\ref hap_serv_write_t) callbacks to check if the request was from an admin controller.
 * Pass the write_priv or read_priv pointer to this API
 *
 * @return true if request is from an admin controller
 * @return false if request is from a non-admin controller
 */
bool hap_is_req_admin(void *priv);

/**
 * @brief Get the ID of the controller who sent the request
 *
 * This API can be used inside service read (\ref hap_serv_read_t) and service write
 * (\ref hap_serv_write_t) callbacks to check the id of the controller who sent the request.
 * Pass the write_priv or read_priv pointer to this API
 *
 * @return pointer to a null terminated controller id string on success.
 * @return NULL on failure.
 */
char *hap_req_get_ctrl_id(void *priv);

/**
 *
 * @brief Get a value from factory NVS keystore
 *
 * This API can be used to fetch information from the factory_nvs flash partition.
 * This is treated as a Read-Only partition and so, no "set" API has been provided.
 *
 * @param[in] name_space NVS name space from which the value needs to be read.
 * @param[in] key The NVS key for the value
 * @param[out] val Buffer which will be used to hold the value, if found
 * @param[in,out] val_size Holds the size of the val buffer. Will be populated with the length of
 * the value by this function.
 *
 * @return HAP_SUCCESS on success
 * @return HAP_FAIL on failure
 */
int hap_factory_keystore_get(const char *name_space, const char *key, uint8_t *val, size_t *val_size);

/**
 * Enable MFi authentication
 *
 * @param[in] auth_type The authentication type desired.
 *
 * @return HAP_SUCCESS on success
 * @return HAP_FAIL on failure
 */
int hap_enable_mfi_auth(hap_mfi_auth_type_t auth_type);

/**
 * @brief Enable Software Token based authentication.
 *
 * This is valid for HomeKit Accessory Protocol Spec R12 (or later) and works with iOS 11.3 (or later) only.
 *
 * This API enables the Software Token based authentication for HomeKit pairing. It requires the accessory
 * UUID and Token to be programmed in the factory_nvs partition. Please check the README for more details.
 *
 * Please refer HomeKit Software Authentication Server Specification to understand the server side
 * implications of using this method for HomeKit Pairing. The tokens fetched from Apple server need to
 * be programmed on the accessory
 *
 * @note Software Authentication is not recommended for new accessories. It should be used only if HomeKit
 * functionality needs to be enabled on accessories which are already in field, or if an existing hardware
 * design needs to be re-used for cost or other considerations. For any other use cases, please get in touch
 * with your Apple contact.
 *
 * @return HAP_SUCCESS on success
 * @return HAP_FAIL if the correct UUID or Token is not provided in the factory_nvs partition
 */
static inline int hap_enable_software_auth()
{
    return hap_enable_mfi_auth(HAP_MFI_AUTH_SW);
}

#define HAP_REBOOT_REASON_UNKNOWN               "unknown"
#define HAP_REBOOT_REASON_RESET_TO_FACTORY      "reset_to_factory"
#define HAP_REBOOT_REASON_REBOOT_ACC            "reboot_acc"
#define HAP_REBOOT_REASON_RESET_NETWORK         "reset_network"
#define HAP_REBOOT_REASON_RESET_PAIRINGS        "reset_pairings"
#define HAP_REBOOT_REASON_RESET_HOMEKIT_DATA    "reset_homekit_data"

/** HomeKit Event Base */
ESP_EVENT_DECLARE_BASE(HAP_EVENT);
/** HomeKit Events */
typedef enum {
    /** A new controller was paired/added/modified.
     * Associated data is a NULL terminated controller identifier string.
     */
    HAP_EVENT_CTRL_PAIRED = 1,
    /** A controller was removed
     * Associated data is a NULL terminated controller identifier string.
     */
    HAP_EVENT_CTRL_UNPAIRED,
    /** A paired controller connected to the accessory (extablished a pair verified session).
     * Associated data is a NULL terminated controller identifier string.
     */
    HAP_EVENT_CTRL_CONNECTED,
    /** A controller disconnected from the accessory. This event is reported before the
     * actual disconnection, because for cases like pair-remove, the controller information
     * gets erased before the disconnection, and so the controller id is not available
     * after disconnection.
     * Associated data is a NULL terminated controller identifier string.
     */
    HAP_EVENT_CTRL_DISCONNECTED,
    /** A Pair Setup attempt has started. Waiting for Setup Code */
    HAP_EVENT_PAIRING_STARTED,
    /** Pair Setup was aborted because of inactivity or a wrong setup code */
    HAP_EVENT_PAIRING_ABORTED,
    /** A GET on /accessories was successfully completed */
    HAP_EVENT_GET_ACC_COMPLETED,
    /** A GET on /characteristics was successfully completed */
    HAP_EVENT_GET_CHAR_COMPLETED,
    /** A PUT (Set value) on /characteristics was successfully completed.
     * This event can also mean that notifications were enabled for some
     * characteristics as the same is also done in PUT /characteristics */
    HAP_EVENT_SET_CHAR_COMPLETED,
    /* Accessory is about to reboot. Will be triggered for operations like hap_reset_to_factory(),
     * hap_reboot_accessory(), hap_reset_network(), hap_reset_pairings() and hap_reset_homekit_data()
     * just before rebooting. Associated data is a pointer to a string indicating the reboot reason.
     * Reefer the HAP_REBOOT_REASON_* macros for possible values.
     */
    HAP_EVENT_ACC_REBOOTING,
} hap_event_t;

/** Prototype for HomeKit Event handler
 *
 * @param[in] event The event id of type \ref hap_event_t
 * @param[in] data Data associated with the event (if applicable).
 * Please refer \ref hap_event_t documentation for information regarding
 * data for each event.
 */
typedef void (*hap_event_handler_t) (hap_event_t event, void *data);

/** Register HomeKit Event Handler
 *
 * If applications are interested in HomeKit specific events, they can
 * write their own event handler and register with HomeKit core using
 * this API.
 *
 * @param[in] handler Application specific HomeKit event handler.
 */
void hap_register_event_handler(hap_event_handler_t handler);

/** Get Paired controller count
 *
 * This API can be used to get a count of number of paired controllers.
 * Thi can be used only after hap_init().
 *
 * @return Number of paired controllers
 */
int hap_get_paired_controller_count();

/*
 * Enable Simple HTTP Debugging
 *
 * Calling this API will enable HTTP Debugging which will show some HomeKit HTTP Debug
 * info like the URL and data.
 */
void hap_http_debug_enable();

/**
 * Disable Simple HTTP Debugging
 *
 * Calling this API will disable HTTP Debugging which was enabled by hap_enable_http_debug()
 */
void hap_http_debug_disable();

/** Get Setup payload
 *
 * This gives the setup payload for the given information
 *
 * @param[in] setup_code NULL terminated setup code. Eg. "111-22-333"
 * @param[in] setup_id NULL terminated setup id. Eg. "ES32"
 * @param[in] wac_support Boolean indicating if WAC provisioning is supported.
 * @param[in] cid Accessory category identifier.
 *
 * @return On success, an allocated NULL terminal setup paylod string. Eg. "X-HM://003363Z4TES32". Should be freed by the caller.
 * @return NULL on failure.
 */
char *esp_hap_get_setup_payload(char *setup_code, char *setup_id, bool wac_support, hap_cid_t cid);
#ifdef __cplusplus
}
#endif

#endif /* _ESP_HAP_H_ */
