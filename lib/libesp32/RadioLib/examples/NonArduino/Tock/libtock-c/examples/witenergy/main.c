#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <ble_conn_params.h>
#include <ble_db_discovery.h>
#include <ble_stack_handler_types.h>
#include <nordic_common.h>
#include <nrf.h>
#include <nrf_error.h>
#include <nrf_sdm.h>
#include <softdevice_handler.h>

#include <simple_ble.h>

#include <nrf51_serialization.h>

#include <button.h>
#include <console.h>
#include <led.h>
#include <timer.h>
#include <tock.h>


/*******************************************************************************
 * Function Prototypes
 ******************************************************************************/
void setup_oort (void);
void toggle_relay (void);


/*******************************************************************************
 * Global State
 ******************************************************************************/

// Intervals for advertising and connections
simple_ble_config_t _ble_config = {
  .device_id         = DEVICE_ID_DEFAULT,
  .adv_name          = "tock-http",
  .adv_interval      = MSEC_TO_UNITS(500, UNIT_0_625_MS),
  .min_conn_interval = MSEC_TO_UNITS(10, UNIT_1_25_MS),
  .max_conn_interval = MSEC_TO_UNITS(1250, UNIT_1_25_MS)
};


#define OORT_BASE_UUID {0x00, 0x00, 0x48, 0x43, 0x45, 0x54, 0x43, 0x49, \
    0x47, 0x4f, 0x4c, 0x49, 0xe0, 0xfe, 0x00, 0x00}
#define BLE_UUID_OORT_SERVICE_SENSOR  0xfee0
#define BLE_UUID_OORT_CHAR_SENSOR     0xfee1
#define BLE_UUID_OORT_CHAR_CLOCK      0xfee3

#define BLE_UUID_OORT_SERVICE_INFO    0x180a
#define BLE_UUID_OORT_CHAR_SYSTEMID   0x2a23


#define MIN_CONNECTION_INTERVAL MSEC_TO_UNITS(10, UNIT_1_25_MS)
#define MAX_CONNECTION_INTERVAL MSEC_TO_UNITS(10, UNIT_1_25_MS)
#define SLAVE_LATENCY           0
#define SUPERVISION_TIMEOUT     MSEC_TO_UNITS(4000, UNIT_10_MS)


static const ble_gap_conn_params_t _connection_param = {
  (uint16_t) MIN_CONNECTION_INTERVAL,  // Minimum connection
  (uint16_t) MAX_CONNECTION_INTERVAL,  // Maximum connection
  (uint16_t) SLAVE_LATENCY,            // Slave latency
  (uint16_t) SUPERVISION_TIMEOUT       // Supervision time-out
};

static const ble_gap_scan_params_t _scan_param = {
  .active      = 0,    // Active scanning not set.
  .selective   = 0,    // Selective scanning not set.
  .p_whitelist = NULL, // No whitelist provided.
  .interval    = 0x00A0,
  .window      = 0x0050,
  .timeout     = 0x0000
};


// Override. Don't need for serialization.
void ble_address_set (void) {
  // nop
}



uint16_t _conn_handle          = BLE_CONN_HANDLE_INVALID;
uint16_t _char_handle_sensor   = 0;
uint16_t _char_handle_systemid = 0;
uint16_t _char_handle_clock    = 0;


// Need to keep track of where we are in the state machine.
typedef enum {
  OORT_STATE_NONE,
  OORT_STATE_SETUP,
  OORT_STATE_SETUP_SEARCHING,            // Looking for meter advertisement
  OORT_STATE_SETUP_CONNECTING,           // Connecting to oort meter
  OORT_STATE_SETUP_DISCOVERING_SERVICES, // Getting all characteristics for services
  OORT_STATE_SETUP_READING_SYSTEMID,     // Reading System ID characteristic
  OORT_STATE_SETUP_WRITING_CLOCK,        // Write time to oort
  OORT_STATE_SETUP_ENABLING_NOTIFICATIONS,
  OORT_STATE_RELAY_TOGGLE_START,
  OORT_STATE_RELAY_TOGGLE_WAITING_NOTIFICATION,
  OORT_STATE_RELAY_TOGGLE_WRITING,
} oort_state_e;

oort_state_e _state = OORT_STATE_NONE;
// What to move to once the current operation has finished.
oort_state_e _next_state = OORT_STATE_NONE;


// State for the DB discovery tool.
static ble_db_discovery_t _ble_db_discovery;

ble_uuid_t _oort_info_service_uuid = {
  .uuid = BLE_UUID_OORT_SERVICE_INFO,
  .type = BLE_UUID_TYPE_BLE,
};

ble_uuid_t _oort_sensor_service_uuid = {
  .uuid = BLE_UUID_OORT_SERVICE_SENSOR,
  .type = BLE_UUID_TYPE_VENDOR_BEGIN,
};

ble_uuid_t _oort_info_systemid_characteristic_uuid = {
  .uuid = BLE_UUID_OORT_CHAR_SYSTEMID,
  .type = BLE_UUID_TYPE_VENDOR_BEGIN,
};

// Whether or not we are connected with the clock set.
bool _setup = false;

// Read from the meter.
uint8_t _system_id[8] = {0};

// Address of the meter we are connected to.
ble_gap_addr_t _oort_address;

// Temp buffer
uint8_t _read_buffer[22];
int _read_len = 0;

/*******************************************************************************
 * BLE Code
 ******************************************************************************/

void db_disc_handler (ble_db_discovery_evt_t* p_evt);
static int convert_oort_to_p1milliunits (const uint8_t* oort);

// Check that the gateway is advertising the correct service UUID.
static bool __is_oort_service_present(const ble_gap_evt_adv_report_t *p_adv_report) {
  uint8_t service_id[16] = OORT_BASE_UUID;
  uint32_t index         = 0;
  uint8_t *p_data        = (uint8_t *)p_adv_report->data;

  while (index < p_adv_report->dlen) {
    uint8_t field_length = p_data[index];
    uint8_t field_type   = p_data[index + 1];

    if ((field_type == BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_MORE_AVAILABLE ||
         field_type == BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_COMPLETE) &&
        (field_length - 1 == 16 &&
         memcmp(service_id, &p_data[index + 2], 16) == 0)) {
      return true;
    }
    index += field_length + 1;
  }
  return false;
}

// Do the next operation.
static void __next (void) {
  uint32_t err_code;

  switch (_state) {
    case OORT_STATE_NONE: break;

    case OORT_STATE_SETUP: {
      // Do the initial setup with the oort
      _state = OORT_STATE_SETUP_SEARCHING;

      // Scan for advertisements method.
      err_code = sd_ble_gap_scan_stop();
      err_code = sd_ble_gap_scan_start(&_scan_param);
      // It is okay to ignore this error since we are stopping the scan anyway.
      if (err_code != NRF_ERROR_INVALID_STATE) {
        APP_ERROR_CHECK(err_code);
      }

      break;
    }

    case OORT_STATE_SETUP_SEARCHING: {
      _state = OORT_STATE_SETUP_CONNECTING;

      // Go ahead and connect to the meter.
      err_code = sd_ble_gap_connect(&_oort_address,
                                    &_scan_param,
                                    &_connection_param);

      if (err_code != NRF_SUCCESS) {
        printf("error connecting to device %li.\n", err_code);
      }

      break;
    }

    case OORT_STATE_SETUP_CONNECTING: {
      // Move to trying to discover info service.
      _state = OORT_STATE_SETUP_DISCOVERING_SERVICES;

      err_code = ble_db_discovery_start(&_ble_db_discovery, _conn_handle);
      if (err_code != NRF_SUCCESS) {
        printf("error db discovery 0x%lx\n", err_code);
      }

      break;
    }

    case OORT_STATE_SETUP_DISCOVERING_SERVICES: {
      _state = OORT_STATE_SETUP_READING_SYSTEMID;

      err_code = sd_ble_gattc_read(_conn_handle, _char_handle_systemid, 0);
      if (err_code != NRF_SUCCESS) {
        printf("error doing read %li\n", err_code);
      }

      break;
    }

    case OORT_STATE_SETUP_READING_SYSTEMID: {
      _state = OORT_STATE_SETUP_WRITING_CLOCK;

      if (_read_len == 8) {
        memcpy(_system_id, _read_buffer, 8);
      } else {
        printf("systemid wrong length?? %i\n", _read_len);
      }

      // Next step is to write to the clock characteristic. This requires
      // calculating the correct message to send and then writing it.
      uint8_t to_send[10];
      to_send[0] = 0x3;
      to_send[1] = 0xe2; // lower 8 bits of year (2018)
      to_send[2] = 0x07; // upper 8 bits of year (2018)
      to_send[3] = 3; // month
      to_send[4] = 28; // day
      to_send[5] = 1; // hour
      to_send[6] = 13; // minute
      to_send[7] = 0; // seconds

      // Now we need to calculate a checksum to append to the end of the message.
      uint16_t chksum =
        ('i' ^ _system_id[0]) +
        ('L' ^ _system_id[1]) +
        ('o' ^ _system_id[2]) +
        ('g' ^ _system_id[5]) +
        ('i' ^ _system_id[6]) +
        ('c' ^ _system_id[7]);

      to_send[8] = chksum & 0xFF;
      to_send[9] = (chksum >> 8) & 0xFF;

      const ble_gattc_write_params_t write_params = {
        .write_op = BLE_GATT_OP_WRITE_REQ,
        .flags    = BLE_GATT_EXEC_WRITE_FLAG_PREPARED_WRITE,
        .handle   = _char_handle_clock,
        .offset   = 0,
        .len      = 10,
        .p_value  = to_send
      };

      err_code = sd_ble_gattc_write(_conn_handle, &write_params);
      if (err_code != NRF_SUCCESS) {
        printf("error doing write %li\n", err_code);
      }

      break;
    }

    case OORT_STATE_SETUP_WRITING_CLOCK: {
      _state = OORT_STATE_SETUP_ENABLING_NOTIFICATIONS;

      uint8_t to_send[2];
      to_send[0] = BLE_GATT_HVX_NOTIFICATION;
      to_send[1] = 0;

      ble_gattc_write_params_t write_params;
      memset(&write_params, 0, sizeof(write_params));
      write_params.write_op = BLE_GATT_OP_WRITE_REQ;
      write_params.handle   = _char_handle_sensor + 1;
      write_params.offset   = 0;
      write_params.len      = sizeof(to_send);
      write_params.p_value  = to_send;

      err_code = sd_ble_gattc_write(_conn_handle, &write_params);
      if (err_code != NRF_SUCCESS) {
        printf("error writing Characteristic 0x%lx\n", err_code);
      }
      break;

    }

    case OORT_STATE_SETUP_ENABLING_NOTIFICATIONS: {
      _setup = true;
      if (_next_state != OORT_STATE_NONE) {
        _state      = _next_state;
        _next_state = OORT_STATE_NONE;
        __next();
      } else {
        _state = OORT_STATE_NONE;
      }

      break;
    }

    case OORT_STATE_RELAY_TOGGLE_START: {
      _state = OORT_STATE_RELAY_TOGGLE_WAITING_NOTIFICATION;
      break;
    }

    case OORT_STATE_RELAY_TOGGLE_WAITING_NOTIFICATION: {
      _state = OORT_STATE_RELAY_TOGGLE_WRITING;
      uint8_t onoff      = _read_buffer[0] ^ 0x1;
      uint8_t to_send[2] = {0x4, onoff};

      const ble_gattc_write_params_t write_params = {
        .write_op = BLE_GATT_OP_WRITE_REQ,
        .flags    = BLE_GATT_EXEC_WRITE_FLAG_PREPARED_WRITE,
        .handle   = _char_handle_clock,
        .offset   = 0,
        .len      = 2,
        .p_value  = to_send
      };

      err_code = sd_ble_gattc_write(_conn_handle, &write_params);
      if (err_code != NRF_SUCCESS) {
        printf("error doing write %li\n", err_code);
      }

      break;
    }

    case OORT_STATE_RELAY_TOGGLE_WRITING: {
      if (_next_state != OORT_STATE_NONE) {
        _state      = _next_state;
        _next_state = OORT_STATE_NONE;
        __next();
      } else {
        _state = OORT_STATE_NONE;
      }

      break;
    }
  }
}

// Convert a u8 buffer like so:
// [0x01, 0x09, 0x99] -> 0.999
//
// Returns value in 10^-4 of whatever the value actually is. So, if
// the value is say 120.3, this will return 1203000.
static int convert_oort_to_p1milliunits (const uint8_t* oort) {

  // First byte is the decimal shift.
  uint8_t decimal_point_shift = oort[0];
  // Start by getting the original value in a u16.
  uint16_t hex_value = (((uint16_t) oort[1]) << 8) | ((uint16_t) oort[2]);

  // Now iterate through the 4 bit chunks to convert the number to decimal.
  unsigned multiplier = 1;
  unsigned out_value  = 0;
  unsigned shifter    = 0;
  for (unsigned i = 0; i < 4; i++) {
    out_value += (((hex_value >> (i * 4)) & 0xF) * multiplier);

    // Leverage the fact that we are already doing powers of 10 to generate
    // the value we need to multiply at the end to get into 0.1 milliunits.
    if (decimal_point_shift == i) {
      shifter = multiplier;
    }

    multiplier *= 10;
  }

  // Now take into account where the decimal place should be
  out_value *= shifter;
  return out_value;
}


static void __on_ble_evt (ble_evt_t* p_ble_evt) {
  switch (p_ble_evt->header.evt_id) {
    case BLE_GAP_EVT_CONN_PARAM_UPDATE: {
      // just update them right now
      ble_gap_conn_params_t conn_params;
      memset(&conn_params, 0, sizeof(conn_params));
      conn_params.min_conn_interval = _ble_config.min_conn_interval;
      conn_params.max_conn_interval = _ble_config.max_conn_interval;
      conn_params.slave_latency     = SLAVE_LATENCY;
      conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

      sd_ble_gap_conn_param_update(_conn_handle, &conn_params);
      break;
    }

    case BLE_GAP_EVT_CONNECTED: {
      _conn_handle = p_ble_evt->evt.gap_evt.conn_handle;

      __next();
      break;
    }

    case BLE_EVT_TX_COMPLETE: {
      break;
    }

    case BLE_GATTC_EVT_PRIM_SRVC_DISC_RSP: {
      break;
    }

    case BLE_GATTC_EVT_CHAR_DISC_RSP: {
      break;
    }

    case BLE_GATTC_EVT_WRITE_RSP: {
      __next();
      break;
    }

    case BLE_GATTC_EVT_HVX: {
      // Got notification
      const ble_gattc_evt_hvx_t* hvx = &p_ble_evt->evt.gattc_evt.params.hvx;

      memcpy(_read_buffer, hvx->data, hvx->len);
      _read_len = hvx->len;

      // Print the power data we just got notified about (since that is the
      // only characteristic we have subscribed to).
      bool relay_status = (hvx->data[0] == 0x1);
      int voltage       = convert_oort_to_p1milliunits(hvx->data + 1);
      int current       = convert_oort_to_p1milliunits(hvx->data + 4);
      int watts         = convert_oort_to_p1milliunits(hvx->data + 7);
      int pf   = convert_oort_to_p1milliunits(hvx->data + 10);
      int freq = convert_oort_to_p1milliunits(hvx->data + 13);
      printf("relay:        %4s\n", (relay_status) ? "on" : "off");
      printf("voltage:      %4i.%04i V\n", voltage / 10000, voltage % 10000);
      printf("current:      %4i.%04i A\n", current / 10000, current % 10000);
      printf("watts:        %4i.%04i W\n", watts / 10000, watts % 10000);
      printf("power factor: %4i.%04i\n", pf / 10000, pf % 10000);
      printf("frequency:    %4i.%04i Hz\n\n", freq / 10000, freq % 10000);

      __next();
      break;
    }

    case BLE_GATTC_EVT_READ_RSP: {
      if (p_ble_evt->evt.gattc_evt.gatt_status != BLE_GATT_STATUS_SUCCESS) {
        printf("read bad???\n");
      } else {
        const ble_gattc_evt_read_rsp_t* p_read_rsp;
        p_read_rsp = &(p_ble_evt->evt.gattc_evt.params.read_rsp);

        memcpy(_read_buffer, p_read_rsp->data, p_read_rsp->len);
        _read_len = p_read_rsp->len;

        __next();
      }
      break;
    }

    case BLE_GAP_EVT_ADV_REPORT: {
      const ble_gap_evt_t* p_gap_evt = &p_ble_evt->evt.gap_evt;
      const ble_gap_evt_adv_report_t* p_adv_report = &p_gap_evt->params.adv_report;

      if (__is_oort_service_present(p_adv_report)) {
        sd_ble_gap_scan_stop();

        // Save the peer address of the oort.
        _oort_address = p_adv_report->peer_addr;

        __next();
      }
      break;
    }

    case BLE_GAP_EVT_DISCONNECTED: {
      _conn_handle = BLE_CONN_HANDLE_INVALID;
      _char_handle_systemid = 0;
      _char_handle_clock    = 0;
      _char_handle_sensor   = 0;
      _setup      = false;
      _state      = OORT_STATE_NONE;
      _next_state = OORT_STATE_NONE;

      printf("Disconnected! Attempting to reconnect in 2s\n");
      delay_ms(2000);
      setup_oort();
      break;
    }

    case BLE_GATTC_EVT_DESC_DISC_RSP: {
      break;
    }

    default:
      printf("event 0x%x\n", p_ble_evt->header.evt_id);
  }
}

void db_disc_handler (ble_db_discovery_evt_t* p_evt) {
  if (p_evt->evt_type == BLE_DB_DISCOVERY_COMPLETE) {
    // We have discovered a service. Loop through the characteristics until
    // we found the ones we care about.

    // Iterate characteristics until we find the ones that we want.
    for (int i = 0; i < p_evt->params.discovered_db.char_count; i++) {
      if (p_evt->params.discovered_db.charateristics[i].characteristic.uuid.uuid == BLE_UUID_OORT_CHAR_SYSTEMID) {
        _char_handle_systemid = p_evt->params.discovered_db.charateristics[i].characteristic.handle_value;
      }
      if (p_evt->params.discovered_db.charateristics[i].characteristic.uuid.uuid == BLE_UUID_OORT_CHAR_CLOCK) {
        _char_handle_clock = p_evt->params.discovered_db.charateristics[i].characteristic.handle_value;
      }
      if (p_evt->params.discovered_db.charateristics[i].characteristic.uuid.uuid == BLE_UUID_OORT_CHAR_SENSOR) {
        _char_handle_sensor = p_evt->params.discovered_db.charateristics[i].characteristic.handle_value;
      }
    }

    // If we have everything we need, we can continue. Next up is to get the
    // system ID.
    if (_char_handle_systemid != 0 && _char_handle_clock != 0 && _char_handle_sensor != 0) {
      __next();
    }

  } else if (p_evt->evt_type == BLE_DB_DISCOVERY_ERROR) {
    printf("BLE_DB_DISCOVERY_ERROR\n");
  } else if (p_evt->evt_type == BLE_DB_DISCOVERY_SRV_NOT_FOUND) {
    printf("BLE_DB_DISCOVERY_SRV_NOT_FOUND\n");
  } else if (p_evt->evt_type == BLE_DB_DISCOVERY_AVAILABLE) {
    printf("BLE_DB_DISCOVERY_AVAILABLE\n");
  } else {
    printf("UNKNOWN BLE_DB_DISCVOERY Event: %d\n", p_evt->evt_type);
  }
}

void ble_error (uint32_t error_code) {
  printf("BLE ERROR: Code = 0x%x\n", (int) error_code);
}

// Called by the softdevice (via serialization) when a BLE event occurs.
static void __ble_evt_dispatch (ble_evt_t* p_ble_evt) {
  __on_ble_evt(p_ble_evt);
  ble_db_discovery_on_ble_evt(&_ble_db_discovery, p_ble_evt);
  ble_conn_params_on_ble_evt(p_ble_evt);
}


/*******************************************************************************
 * MAIN
 ******************************************************************************/

void setup_oort (void) {
  _state = OORT_STATE_SETUP;
  __next();
}

void toggle_relay (void) {
  if (!_setup) {
    _next_state = OORT_STATE_RELAY_TOGGLE_START;
    setup_oort();
  } else {
    _state      = OORT_STATE_RELAY_TOGGLE_START;
    _next_state = OORT_STATE_NONE;
    __next();
  }
}

static void button_callback(__attribute__ ((unused)) int   btn_num,
                            int                            val,
                            __attribute__ ((unused)) int   arg2,
                            __attribute__ ((unused)) void *ud) {
  if (val == 1 && _state == OORT_STATE_NONE) {
    printf("Button press! Toggle the relay!\n");
    toggle_relay();
  }
}

int main (void) {
  uint32_t err_code;

  printf("[Wit Energy]\n");
  printf("Press the user button to toggle the relay.\n");

  // Button press toggles meter relay.
  button_subscribe(button_callback, NULL);
  button_enable_interrupt(0);

  // Setup simple BLE. This does most of the nordic setup.
  simple_ble_init(&_ble_config);

  // Re-register the callback so that we use our handler and not simple ble's.
  err_code = softdevice_ble_evt_handler_set(__ble_evt_dispatch);
  if (err_code != NRF_SUCCESS) return err_code;

  // Set the UUID in the soft device so it can use it.
  ble_uuid128_t base_uuid = {OORT_BASE_UUID};
  uint8_t base_uuid_type  = BLE_UUID_TYPE_VENDOR_BEGIN;
  err_code = sd_ble_uuid_vs_add(&base_uuid, &base_uuid_type);
  if (err_code != NRF_SUCCESS) return err_code;

  err_code = ble_db_discovery_init(db_disc_handler);
  ble_db_discovery_evt_register(&_oort_info_service_uuid);
  ble_db_discovery_evt_register(&_oort_sensor_service_uuid);
}
