/**
 * esp-knx-ip library for KNX/IP communication on an ESP8266
 * Author: Nico Weichbrodt <envy>
 * License: MIT
 */

#ifndef ESP_KNX_IP_H
#define ESP_KNX_IP_H

/**
 * CONFIG
 * All MAX_ values must not exceed 255 (1 byte, except MAC_CONFIG_SPACE which can go up to 2 bytes, so 0xffff in theory) and must not be negative!
 * Config space is restriced by EEPROM_SIZE (default 1024).
 * Required EEPROM size is 8 + MAX_GA_CALLBACKS * 3 + 2 + MAX_CONFIG_SPACE which is 552 by default
 */
#define EEPROM_SIZE               1024 // [Default 1024]
#define MAX_CALLBACK_ASSIGNMENTS  10 // [Default 10] Maximum number of group address callbacks that can be stored
#define MAX_CALLBACKS             10 // [Default 10] Maximum number of callbacks that can be stored
#define MAX_CONFIGS               20 // [Default 20] Maximum number of config items that can be stored
#define MAX_CONFIG_SPACE          0x0200 // [Default 0x0200] Maximum number of bytes that can be stored for custom config

#define MAX_FEEDBACKS             20 // [Default 20] Maximum number of feedbacks that can be shown

// Callbacks
#define ALLOW_MULTIPLE_CALLBACKS_PER_ADDRESS  1 // [Default 0] Set to 1 to always test all assigned callbacks. This allows for multiple callbacks being assigned to the same address. If disabled, only the first assigned will be called.

// Webserver related
#define USE_BOOTSTRAP             0 // [Default 1] Set to 1 to enable use of bootstrap CSS for nicer webconfig. CSS is loaded from bootstrapcdn.com. Set to 0 to disable
#define ROOT_PREFIX               "/knx"  // [Default ""] This gets prepended to all webserver paths, default is empty string "". Set this to "/knx" if you want the config to be available on http://<ip>/knx
#define DISABLE_EEPROM_BUTTONS    1 // [Default 0] Set to 1 to disable the EEPROM buttons in the web ui.
#define DISABLE_REBOOT_BUTTON     1 // [Default 0] Set to 1 to disable the reboot button in the web ui.
#define DISABLE_RESTORE_BUTTON    1 // [Default 0] Set to 1 to disable the "restore defaults" button in the web ui.

// These values normally don't need adjustment
#ifndef MULTICAST_IP
#define MULTICAST_IP              IPAddress(224, 0, 23, 12) // [Default IPAddress(224, 0, 23, 12)]
#else
#warning USING CUSTOM MULTICAST_IP
#endif

#ifndef MULTICAST_PORT
#define MULTICAST_PORT            3671 // [Default 3671]
#else
#warning USING CUSTOM MULTICAST_PORT
#endif

#define SEND_CHECKSUM             0

// Uncomment to enable printing out debug messages.
//#define ESP_KNX_DEBUG
/**
 * END CONFIG
 */

#include "Arduino.h"
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266WebServer.h>

#include "DPT.h"

#define EEPROM_MAGIC (0xDEADBEEF00000000 + (MAX_CONFIG_SPACE) + (MAX_CALLBACK_ASSIGNMENTS << 16) + (MAX_CALLBACKS << 8))

// Define where debug output will be printed.
#ifndef DEBUG_PRINTER
#define DEBUG_PRINTER Serial
#endif

// Setup debug printing macros.
#ifdef ESP_KNX_DEBUG
  #define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
  #define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
  #define DEBUG_PRINT(...) {}
  #define DEBUG_PRINTLN(...) {}
#endif

#define __ROOT_PATH       ROOT_PREFIX"/"
#define __REGISTER_PATH   ROOT_PREFIX"/register"
#define __DELETE_PATH     ROOT_PREFIX"/delete"
#define __PHYS_PATH       ROOT_PREFIX"/phys"
#define __EEPROM_PATH     ROOT_PREFIX"/eeprom"
#define __CONFIG_PATH     ROOT_PREFIX"/config"
#define __FEEDBACK_PATH   ROOT_PREFIX"/feedback"
#define __RESTORE_PATH    ROOT_PREFIX"/restore"
#define __REBOOT_PATH     ROOT_PREFIX"/reboot"

/**
 * Different service types, we are mainly interested in KNX_ST_ROUTING_INDICATION
 */
typedef enum __knx_service_type
{
  KNX_ST_SEARCH_REQUEST           = 0x0201,
  KNX_ST_SEARCH_RESPONSE          = 0x0202,
  KNX_ST_DESCRIPTION_REQUEST      = 0x0203,
  KNX_ST_DESCRIPTION_RESPONSE     = 0x0204,
  KNX_ST_CONNECT_REQUEST          = 0x0205,
  KNX_ST_CONNECT_RESPONSE         = 0x0206,
  KNX_ST_CONNECTIONSTATE_REQUEST  = 0x0207,
  KNX_ST_CONNECTIONSTATE_RESPONSE = 0x0208,
  KNX_ST_DISCONNECT_REQUEST       = 0x0209,
  KNX_ST_DISCONNECT_RESPONSE      = 0x020A,

  KNX_ST_DEVICE_CONFIGURATION_REQUEST = 0x0310,
  KNX_ST_DEVICE_CONFIGURATION_ACK     = 0x0311,

  KNX_ST_TUNNELING_REQUEST = 0x0420,
  KNX_ST_TUNNELING_ACK     = 0x0421,

  KNX_ST_ROUTING_INDICATION   = 0x0530,
  KNX_ST_ROUTING_LOST_MESSAGE = 0x0531,
  KNX_ST_ROUTING_BUSY         = 0x0532,

//  KNX_ST_RLOG_START = 0x0600,
//  KNX_ST_RLOG_END   = 0x06FF,

  KNX_ST_REMOTE_DIAGNOSTIC_REQUEST          = 0x0740,
  KNX_ST_REMOTE_DIAGNOSTIC_RESPONSE         = 0x0741,
  KNX_ST_REMOTE_BASIC_CONFIGURATION_REQUEST = 0x0742,
  KNX_ST_REMOTE_RESET_REQUEST               = 0x0743,

//  KNX_ST_OBJSRV_START = 0x0800,
//  KNX_ST_OBJSRV_END   = 0x08FF,
} knx_service_type_t;

/**
 * Differnt command types, first three are of main interest
 */
typedef enum __knx_command_type
{
  KNX_CT_READ                     = 0x00,
  KNX_CT_ANSWER                   = 0x01,
  KNX_CT_WRITE                    = 0x02,
  KNX_CT_INDIVIDUAL_ADDR_WRITE    = 0x03,
  KNX_CT_INDIVIDUAL_ADDR_REQUEST  = 0x04,
  KNX_CT_INDIVIDUAL_ADDR_RESPONSE = 0x05,
  KNX_CT_ADC_READ                 = 0x06,
  KNX_CT_ADC_ANSWER               = 0x07,
  KNX_CT_MEM_READ                 = 0x08,
  KNX_CT_MEM_ANSWER               = 0x09,
  KNX_CT_MEM_WRITE                = 0x0A,
//KNX_CT_UNKNOWN                  = 0x0B,
  KNX_CT_MASK_VERSION_READ        = 0x0C,
  KNX_CT_MASK_VERSION_RESPONSE    = 0x0D,
  KNX_CT_RESTART                  = 0x0E,
  KNX_CT_ESCAPE                   = 0x0F,
} knx_command_type_t;

/**
 * cEMI message types, mainly KNX_MT_L_DATA_IND is interesting
 */
typedef enum __knx_cemi_msg_type
{
  KNX_MT_L_DATA_REQ = 0x11,
  KNX_MT_L_DATA_IND = 0x29,
  KNX_MT_L_DATA_CON = 0x2E,
} knx_cemi_msg_type_t;

/**
 * TCPI communication type
 */
typedef enum __knx_communication_type {
  KNX_COT_UDP = 0x00, // Unnumbered Data Packet
  KNX_COT_NDP = 0x01, // Numbered Data Packet
  KNX_COT_UCD = 0x02, // Unnumbered Control Data
  KNX_COT_NCD = 0x03, // Numbered Control Data
} knx_communication_type_t;

/**
 * acpi for KNX_COT_NCD
 */
typedef enum __knx_cot_ncd_ack_type {
  KNX_COT_NCD_ACK = 0x10, // Inform positively reception of the Previouly received telegram
  KNX_COT_NCD_NACK = 0x11, // Inform negatively reception of the Previouly received telegram
} knx_cot_ncd_ack_type_t;

/**
 * KNX/IP header
 */
typedef struct __knx_ip_pkt
{
  uint8_t header_len; // Should always be 0x06
  uint8_t protocol_version; // Should be version 1.0, transmitted as 0x10
  uint16_t service_type; // See knx_service_type_t
  union
  {
    struct {
      uint8_t first_byte;
      uint8_t second_byte;
    } bytes;
    uint16_t len;
  } total_len; // header_len + rest of pkt. This is a bit weird as the spec says this:  If the total number of bytes transmitted is greater than 252 bytes, the first “Total Length” byte is set to FF (255). Only in this case the second byte includes additional length information
  uint8_t pkt_data[]; // This is of type cemi_msg_t
} knx_ip_pkt_t;

typedef struct __cemi_addi
{
  uint8_t type_id;
  uint8_t len;
  uint8_t data[];
} cemi_addi_t;

typedef union __address
{
  uint16_t value;
  struct
  {
    uint8_t high;
    uint8_t low;
  } bytes;
  struct __attribute__((packed))
  {
    uint8_t line:3;
    uint8_t area:5;
    uint8_t member;
  } ga;
  struct __attribute__((packed))
  {
    uint8_t line:4;
    uint8_t area:4;
    uint8_t member;
  } pa;
  uint8_t array[2];
} address_t;

typedef struct __cemi_service
{
  union
  {
    struct
    {
      // Struct is reversed due to bit order
      uint8_t confirm:1; // 0 = no error, 1 = error
      uint8_t ack:1; // 0 = no ack, 1 = ack
      uint8_t priority:2; // 0 = system, 1 = high, 2 = urgent/alarm, 3 = normal
      uint8_t system_broadcast:1; // 0 = system broadcast, 1 = broadcast
      uint8_t repeat:1; // 0 = repeated telegram, 1 = not repeated telegram
      uint8_t reserved:1; // always zero
      uint8_t frame_type:1; // 0 = extended, 1 = standard
    } bits;
    uint8_t byte;
  } control_1;
  union
  {
    struct
    {
      // Struct is reversed due to bit order
      uint8_t extended_frame_format:4;
      uint8_t hop_count:3;
      uint8_t dest_addr_type:1; // 0 = individual, 1 = group
    } bits;
    uint8_t byte;
  } control_2;
  address_t source;
  address_t destination;
  uint8_t data_len; // length of data, excluding the tpci byte
  struct
  {
    uint8_t apci:2; // If tpci.comm_type == KNX_COT_UCD or KNX_COT_NCD, then this is apparently control data?
    uint8_t tpci_seq_number:4;
    uint8_t tpci_comm_type:2; // See knx_communication_type_t
  } pci;
  uint8_t data[];
} cemi_service_t;

typedef struct __cemi_msg
{
  uint8_t message_code;
  uint8_t additional_info_len;
  union
  {
    cemi_addi_t additional_info[];
    cemi_service_t service_information;
  } data;
} cemi_msg_t;

typedef enum __config_type
{
  CONFIG_TYPE_UNKNOWN,
  CONFIG_TYPE_INT,
  CONFIG_TYPE_BOOL,
  CONFIG_TYPE_STRING,
  CONFIG_TYPE_OPTIONS,
  CONFIG_TYPE_GA,
} config_type_t;

typedef enum __feedback_type
{
  FEEDBACK_TYPE_UNKNOWN,
  FEEDBACK_TYPE_INT,
  FEEDBACK_TYPE_FLOAT,
  FEEDBACK_TYPE_BOOL,
  FEEDBACK_TYPE_ACTION,
} feedback_type_t;

typedef enum __config_flags
{
  CONFIG_FLAGS_NO_FLAGS = 0,
  CONFIG_FLAGS_VALUE_SET = 1,
} config_flags_t;

typedef enum __slot_flags
{
  SLOT_FLAGS_EMPTY = 0, // Empty slots have no flags
  SLOT_FLAGS_USED = 1,
} slot_flags_t;

typedef struct __message
{
  knx_command_type_t ct;
  address_t received_on;
  uint8_t data_len;
  uint8_t *data;
} message_t;

typedef bool (*enable_condition_t)(void);
typedef void (*callback_fptr_t)(message_t const &msg, void *arg);
typedef void (*feedback_action_fptr_t)(void *arg);

typedef uint8_t callback_id_t;
#define CALLBACK_ID_MAX UINT8_MAX
typedef uint8_t callback_assignment_id_t;
#define CALLBACK_ASSIGNMENT_ID_MAX UINT8_MAX
typedef uint8_t config_id_t;
typedef uint8_t feedback_id_t;

typedef struct __option_entry
{
  char const *name;
  uint8_t value;
} option_entry_t;

typedef struct __config
{
  config_type_t type;
  String name;
  uint8_t offset;
  uint8_t len;
  enable_condition_t cond;
  union {
    option_entry_t *options;
  } data;
} config_t;

extern char const *string_defaults[];
#define STRING_DEFAULT_DO_THIS (string_defaults[0])
#define STRING_DEFAULT_TRUE (string_defaults[1])
#define STRING_DEFAULT_FALSE (string_defaults[2])
#define STRING_DEFAULT_EMPTY (string_defaults[3])

typedef struct __feedback_float_options
{
  uint8_t precision;
  char const *prefix;
  char const *suffix;
} feedback_float_options_t;

typedef struct __feedback_bool_options
{
  char const *true_text;
  char const *false_text;
} feedback_bool_options_t;

typedef struct __feedback_action_options
{
  void *arg;
  char const *btn_text;
} feedback_action_options_t;

typedef struct __feedback
{
  feedback_type_t type;
  String name;
  enable_condition_t cond;
  void *data;
  union {
    feedback_bool_options_t bool_options;
    feedback_float_options_t float_options;
    feedback_action_options_t action_options;
  } options;
} feedback_t;

typedef struct __callback
{
  uint8_t slot_flags;
  callback_fptr_t fkt;
  enable_condition_t cond;
  void *arg;
  String name;
} callback_t;

typedef struct __callback_assignment
{
  uint8_t slot_flags;
  address_t address;
  callback_id_t callback_id;
} callback_assignment_t;

// FastPrecisePowf from tasmota/support_float.ino
extern float FastPrecisePowf(const float x, const float y);

class ESPKNXIP {
  public:
    ESPKNXIP();
    void load();
    void start();
    void start(ESP8266WebServer *srv);
    void loop();

    void save_to_eeprom();
    void restore_from_eeprom();

    callback_id_t            callback_register(String name, callback_fptr_t cb, void *arg = nullptr, enable_condition_t cond = nullptr);
    callback_assignment_id_t callback_assign(callback_id_t id, address_t val);
    void                     callback_deregister(callback_id_t id);
    void                     callback_unassign(callback_assignment_id_t id);

    void                     physical_address_set(address_t const &addr);
    address_t                physical_address_get();

    // Configuration functions
    config_id_t              config_register_string(String name, uint8_t len, String _default, enable_condition_t cond = nullptr);
    config_id_t              config_register_int(String name, int32_t _default, enable_condition_t cond = nullptr);
    config_id_t              config_register_bool(String name, bool _default, enable_condition_t cond = nullptr);
    config_id_t              config_register_options(String name, option_entry_t *options, uint8_t _default, enable_condition_t cond = nullptr);
    config_id_t              config_register_ga(String name, enable_condition_t cond = nullptr);

    String                   config_get_string(config_id_t id);
    int32_t                  config_get_int(config_id_t id);
    bool                     config_get_bool(config_id_t id);
    uint8_t                  config_get_options(config_id_t id);
    address_t                config_get_ga(config_id_t id);

    void                     config_set_string(config_id_t id, String val);
    void                     config_set_int(config_id_t id, int32_t val);
    void                     config_set_bool(config_id_t, bool val);
    void                     config_set_options(config_id_t id, uint8_t val);
    void                     config_set_ga(config_id_t id, address_t const &val);

    // Feedback functions
    feedback_id_t            feedback_register_int(String name, int32_t *value, enable_condition_t cond = nullptr);
    feedback_id_t            feedback_register_float(String name, float *value, uint8_t precision = 2, char const *prefix = nullptr, char const *suffix = nullptr, enable_condition_t cond = nullptr);
    feedback_id_t            feedback_register_bool(String name, bool *value, char const *true_text = nullptr, char const *false_text = nullptr, enable_condition_t cond = nullptr);
    feedback_id_t            feedback_register_action(String name, feedback_action_fptr_t value, char const *btn_text = nullptr, void *arg = nullptr, enable_condition_t = nullptr);

    // Send functions
    void send(address_t const &receiver, knx_command_type_t ct, uint8_t data_len, uint8_t *data);

    void send_1bit(address_t const &receiver, knx_command_type_t ct, uint8_t bit);
    void send_2bit(address_t const &receiver, knx_command_type_t ct, uint8_t twobit);
    void send_4bit(address_t const &receiver, knx_command_type_t ct, uint8_t fourbit);
    void send_1byte_int(address_t const &receiver, knx_command_type_t ct, int8_t val);
    void send_1byte_uint(address_t const &receiver, knx_command_type_t ct, uint8_t val);
    void send_2byte_int(address_t const &receiver, knx_command_type_t ct, int16_t val);
    void send_2byte_uint(address_t const &receiver, knx_command_type_t ct, uint16_t val);
    void send_2byte_float(address_t const &receiver, knx_command_type_t ct, float val);
    void send_3byte_time(address_t const &receiver, knx_command_type_t ct, uint8_t weekday, uint8_t hours, uint8_t minutes, uint8_t seconds);
    void send_3byte_time(address_t const &receiver, knx_command_type_t ct, time_of_day_t const &time) { send_3byte_time(receiver, ct, time.weekday, time.hours, time.minutes, time.seconds); }
    void send_3byte_date(address_t const &receiver, knx_command_type_t ct, uint8_t day, uint8_t month, uint8_t year);
    void send_3byte_date(address_t const &receiver, knx_command_type_t ct, date_t const &date) { send_3byte_date(receiver, ct, date.day, date.month, date.year); }
    void send_3byte_color(address_t const &receiver, knx_command_type_t ct, uint8_t red, uint8_t green, uint8_t blue);
    void send_3byte_color(address_t const &receiver, knx_command_type_t ct, color_t const &color) { send_3byte_color(receiver, ct, color.red, color.green, color.blue); }
    void send_4byte_int(address_t const &receiver, knx_command_type_t ct, int32_t val);
    void send_4byte_uint(address_t const &receiver, knx_command_type_t ct, uint32_t val);
    void send_4byte_float(address_t const &receiver, knx_command_type_t ct, float val);
    void send_14byte_string(address_t const &receiver, knx_command_type_t ct, const char *val);

    void write_1bit(address_t const &receiver, uint8_t bit) { send_1bit(receiver, KNX_CT_WRITE, bit); }
    void write_2bit(address_t const &receiver, uint8_t twobit) { send_2bit(receiver, KNX_CT_WRITE, twobit); }
    void write_4bit(address_t const &receiver, uint8_t fourbit) { send_4bit(receiver, KNX_CT_WRITE, fourbit); }
    void write_1byte_int(address_t const &receiver, int8_t val) { send_1byte_int(receiver, KNX_CT_WRITE, val); }
    void write_1byte_uint(address_t const &receiver, uint8_t val) { send_1byte_uint(receiver, KNX_CT_WRITE, val); }
    void write_2byte_int(address_t const &receiver, int16_t val) { send_2byte_int(receiver, KNX_CT_WRITE, val); }
    void write_2byte_uint(address_t const &receiver, uint16_t val) { send_2byte_uint(receiver, KNX_CT_WRITE, val); }
    void write_2byte_float(address_t const &receiver, float val) { send_2byte_float(receiver, KNX_CT_WRITE, val); }
    void write_3byte_time(address_t const &receiver, uint8_t weekday, uint8_t hours, uint8_t minutes, uint8_t seconds) { send_3byte_time(receiver, KNX_CT_WRITE, weekday, hours, minutes, seconds); }
    void write_3byte_time(address_t const &receiver, time_of_day_t const &time) { send_3byte_time(receiver, KNX_CT_WRITE, time.weekday, time.hours, time.minutes, time.seconds); }
    void write_3byte_date(address_t const &receiver, uint8_t day, uint8_t month, uint8_t year) { send_3byte_date(receiver, KNX_CT_WRITE, day, month, year); }
    void write_3byte_date(address_t const &receiver, date_t const &date) { send_3byte_date(receiver, KNX_CT_WRITE, date.day, date.month, date.year); }
    void write_3byte_color(address_t const &receiver, uint8_t red, uint8_t green, uint8_t blue) { send_3byte_color(receiver, KNX_CT_WRITE, red, green, blue); }
    void write_3byte_color(address_t const &receiver, color_t const &color) { send_3byte_color(receiver, KNX_CT_WRITE, color); }
    void write_4byte_int(address_t const &receiver, int32_t val) { send_4byte_int(receiver, KNX_CT_WRITE, val); }
    void write_4byte_uint(address_t const &receiver, uint32_t val) { send_4byte_uint(receiver, KNX_CT_WRITE, val); }
    void write_4byte_float(address_t const &receiver, float val) { send_4byte_float(receiver, KNX_CT_WRITE, val);}
    void write_14byte_string(address_t const &receiver, const char *val) { send_14byte_string(receiver, KNX_CT_WRITE, val); }

    void answer_1bit(address_t const &receiver, uint8_t bit) { send_1bit(receiver, KNX_CT_ANSWER, bit); }
    void answer_2bit(address_t const &receiver, uint8_t twobit) { send_2bit(receiver, KNX_CT_ANSWER, twobit); }
    void answer_4bit(address_t const &receiver, uint8_t fourbit) { send_4bit(receiver, KNX_CT_ANSWER, fourbit); }
    void answer_1byte_int(address_t const &receiver, int8_t val) { send_1byte_int(receiver, KNX_CT_ANSWER, val); }
    void answer_1byte_uint(address_t const &receiver, uint8_t val) { send_1byte_uint(receiver, KNX_CT_ANSWER, val); }
    void answer_2byte_int(address_t const &receiver, int16_t val) { send_2byte_int(receiver, KNX_CT_ANSWER, val); }
    void answer_2byte_uint(address_t const &receiver, uint16_t val) { send_2byte_uint(receiver, KNX_CT_ANSWER, val); }
    void answer_2byte_float(address_t const &receiver, float val) { send_2byte_float(receiver, KNX_CT_ANSWER, val); }
    void answer_3byte_time(address_t const &receiver, uint8_t weekday, uint8_t hours, uint8_t minutes, uint8_t seconds) { send_3byte_time(receiver, KNX_CT_ANSWER, weekday, hours, minutes, seconds); }
    void answer_3byte_time(address_t const &receiver, time_of_day_t const &time) { send_3byte_time(receiver, KNX_CT_ANSWER, time.weekday, time.hours, time.minutes, time.seconds); }
    void answer_3byte_date(address_t const &receiver, uint8_t day, uint8_t month, uint8_t year) { send_3byte_date(receiver, KNX_CT_ANSWER, day, month, year); }
    void answer_3byte_date(address_t const &receiver, date_t const &date) { send_3byte_date(receiver, KNX_CT_ANSWER, date.day, date.month, date.year); }
    void answer_3byte_color(address_t const &receiver, uint8_t red, uint8_t green, uint8_t blue) { send_3byte_color(receiver, KNX_CT_ANSWER, red, green, blue); }
    void answer_3byte_color(address_t const &receiver, color_t const &color) { send_3byte_color(receiver, KNX_CT_ANSWER, color); }
    void answer_4byte_int(address_t const &receiver, int32_t val) { send_4byte_int(receiver, KNX_CT_ANSWER, val); }
    void answer_4byte_uint(address_t const &receiver, uint32_t val) { send_4byte_uint(receiver, KNX_CT_ANSWER, val); }
    void answer_4byte_float(address_t const &receiver, float val) { send_4byte_float(receiver, KNX_CT_ANSWER, val);}
    void answer_14byte_string(address_t const &receiver, const char *val) { send_14byte_string(receiver, KNX_CT_ANSWER, val); }

    bool          data_to_bool(uint8_t *data);
    int8_t        data_to_1byte_int(uint8_t *data);
    uint8_t       data_to_1byte_uint(uint8_t *data);
    int16_t       data_to_2byte_int(uint8_t *data);
    uint16_t      data_to_2byte_uint(uint8_t *data);
    float         data_to_2byte_float(uint8_t *data);
    color_t       data_to_3byte_color(uint8_t *data);
    time_of_day_t data_to_3byte_time(uint8_t *data);
    date_t        data_to_3byte_data(uint8_t *data);
    int32_t       data_to_4byte_int(uint8_t *data);
    uint32_t      data_to_4byte_uint(uint8_t *data);
    float         data_to_4byte_float(uint8_t *data);

    static address_t GA_to_address(uint8_t area, uint8_t line, uint8_t member)
    {
      // Yes, the order is correct, see the struct definition above
      address_t tmp = {.ga={line, area, member}};
      return tmp;
    }

    static address_t PA_to_address(uint8_t area, uint8_t line, uint8_t member)
    {
      // Yes, the order is correct, see the struct definition above
      address_t tmp = {.pa={line, area, member}};
      return tmp;
    }

  private:
    void __start();

    void __loop_knx();

    // Webserver functions
    void __loop_webserver();
    void __handle_root();
    void __handle_register();
    void __handle_delete();
    void __handle_set();
#if !DISABLE_EEPROM_BUTTONS
    void __handle_eeprom();
#endif
    void __handle_config();
    void __handle_feedback();
#if !DISABLE_RESTORE_BUTTONS
    void __handle_restore();
#endif
#if !DISABLE_REBOOT_BUTTONS
    void __handle_reboot();
#endif

    void __config_set_flags(config_id_t id, config_flags_t flags);

    void __config_set_string(config_id_t id, String &val);
    void __config_set_int(config_id_t id, int32_t val);
    void __config_set_bool(config_id_t id, bool val);
    void __config_set_options(config_id_t id, uint8_t val);
    void __config_set_ga(config_id_t id, address_t const &val);

    bool __callback_is_id_valid(callback_id_t id);

    callback_assignment_id_t __callback_register_assignment(address_t address, callback_id_t id);
    void __callback_delete_assignment(callback_assignment_id_t id);

    static inline float pow(float a, float b) { return FastPrecisePowf(a, b); }

    ESP8266WebServer *server;
    address_t physaddr;

    WiFiUDP udp;

    callback_assignment_id_t registered_callback_assignments;
    callback_assignment_id_t free_callback_assignment_slots;
    callback_assignment_t callback_assignments[MAX_CALLBACK_ASSIGNMENTS];

    callback_id_t registered_callbacks;
    callback_id_t free_callback_slots;
    callback_t callbacks[MAX_CALLBACKS];

    config_id_t registered_configs;
    uint8_t custom_config_data[MAX_CONFIG_SPACE];
    uint8_t custom_config_default_data[MAX_CONFIG_SPACE];
    config_t custom_configs[MAX_CONFIGS];

    feedback_id_t registered_feedbacks;
    feedback_t feedbacks[MAX_FEEDBACKS];

    uint16_t __ntohs(uint16_t);
};

// Global "singleton" object
extern ESPKNXIP knx;

#endif
