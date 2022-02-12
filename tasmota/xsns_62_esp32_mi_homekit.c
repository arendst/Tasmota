#if(USE_MI_HOMEKIT==1)

#include <stdio.h>
#include <string.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>

#include <hap.h>
#include <hap_apple_servs.h>
#include <hap_apple_chars.h>
#include <hap_platform_keystore.h>

//Homekit
static int MI32_bridge_identify(hap_acc_t *ha);
static int MI32_accessory_identify(hap_acc_t *ha);
static void MI32_bridge_thread_entry(void *p);

extern uint32_t MI32numberOfDevices();
extern const char *MI32getDeviceName(uint32_t slot);
extern uint32_t MI32getDeviceType(uint32_t slot);
extern void MI32saveHAPhandles(uint32_t slot, uint32_t type, void* handle);
extern void MI32passHapEvent(uint32_t event);
extern void MI32didStartHAP();
extern const char * MI32getSetupCode();
extern uint32_t MI32numOfRelays();
extern void MI32setRelayFromHK(uint32_t relay, bool onOff);

// static const char *TAG = "Mi Bridge";
static bool MIBridgeWasNeverConnected = true;

#define CONFIG_EXAMPLE_SETUP_ID "MI32"

#define FLORA       1
#define MJ_HT_V1    2
#define LYWSD02     3
#define LYWSD03MMC  4
#define CGG1        5
#define CGD1        6
#define NLIGHT      7
#define MJYD2S      8
#define YLYK01       9
#define MHOC401     10
#define MHOC303     11
#define ATC         12
#define MCCGQ02     13
#define SJWS01L     14
#define PVVX        15
#define YLKG08      16

/*********************************************************************************************\
 * Homekit
\*********************************************************************************************/
/* Mandatory identify routine for the bridge.
 * In a real accessory, something like LED blink should be implemented
 * got visual identification
 */
static int MI32_bridge_identify(hap_acc_t *ha)
{
    return HAP_SUCCESS;
}

void mi_hap_event_handler(hap_event_t event, void *data)
{
    MI32passHapEvent((uint32_t)event);
    if(event == HAP_EVENT_CTRL_CONNECTED) MIBridgeWasNeverConnected = false;
}

static int MI32_bridge_read_callback(hap_read_data_t read_data[], int count,
        void *serv_priv, void *read_priv)
{
    return HAP_SUCCESS;
}

static int MI32_outlets_write_callback(hap_write_data_t write_data[], int count,
        void *serv_priv, void *write_priv)
{
    uint8_t _relay = ((uint8_t*)serv_priv)[0];
    int i, ret = HAP_SUCCESS;
    hap_write_data_t *write;
    for (i = 0; i < count; i++) {
        write = &write_data[i];
        if (!strcmp(hap_char_get_type_uuid(write->hc), HAP_CHAR_UUID_ON)) {
            MI32setRelayFromHK(_relay-48, write->val.b);
            hap_char_update_val(write->hc, &(write->val));
            *(write->status) = HAP_STATUS_SUCCESS;
        } else {
            *(write->status) = HAP_STATUS_RES_ABSENT;
        }
    }
    return ret;
}

/* Mandatory identify routine for the bridged accessory
 * In a real bridge, the actual accessory must be sent some request to
 * identify itself visually
 */
static int MI32_accessory_identify(hap_acc_t *ha)
{
    return HAP_SUCCESS;
}

/*The main thread for handling the Smart Outlet Accessory */
static void MI32_bridge_thread_entry(void *p)
{
    // esp_log_level_set("*", ESP_LOG_NONE);
    hap_acc_t *accessory;
    hap_serv_t *service;

    /* Initialize the HAP core */
    hap_init(HAP_TRANSPORT_WIFI);

    /* Initialise the mandatory parameters for Accessory which will be added as
     * the mandatory services internally
     */
    hap_acc_cfg_t cfg = {
        .name = "Mi-Home-Bridge",
        .manufacturer = "Tasmota",
        .model = "ESP32",
        .serial_num = "9600",
        .fw_rev = "0.9.5",
        .hw_rev = NULL,
        .pv = "1.1.0",
        .cid = HAP_CID_BRIDGE,
        .identify_routine = MI32_bridge_identify
    };
    /* Create accessory object */
    accessory = hap_acc_create(&cfg);

    /* Add a dummy Product Data */
    uint8_t product_data[] = {'T','M','H'};
    hap_acc_add_product_data(accessory, product_data, sizeof(product_data));

    /* Add the Accessory to the HomeKit Database */
    hap_add_accessory(accessory);

#define NUM_BRIDGED_ACCESSORIES 1
    /* Create and add the Accessory to the Bridge object*/
    uint32_t _numDevices = MI32numberOfDevices();
    for (uint32_t i = 0; i < _numDevices; i++) {
        char *accessory_name = (char*)MI32getDeviceName(i);
        char _serialNum[4] = {0};
        snprintf(_serialNum,sizeof(_serialNum),"%u", i);

        hap_acc_cfg_t bridge_cfg = {
            .name = accessory_name,
            .manufacturer = "Xiaomi",
            .model = accessory_name,
            .serial_num = _serialNum,
            .fw_rev = "0.9.1",
            .hw_rev = NULL,
            .pv = "1.1.0",
            .cid = HAP_CID_SENSOR,
            .identify_routine = MI32_accessory_identify,
        };

        /* Create accessory object */
        accessory = hap_acc_create(&bridge_cfg);

        switch (MI32getDeviceType(i)){
            case LYWSD02: case LYWSD03MMC: case CGG1: case CGD1: case MHOC303: case MHOC401: case ATC: case PVVX:
            {
                service = hap_serv_humidity_sensor_create(50.0f);
                hap_serv_set_bulk_read_cb(service, MI32_bridge_read_callback);
                hap_acc_add_serv(accessory, service);
                MI32saveHAPhandles(i,0x06,(void *)hap_serv_get_char_by_uuid(service, HAP_CHAR_UUID_CURRENT_RELATIVE_HUMIDITY));

                service = hap_serv_temperature_sensor_create(22.5f);
                hap_serv_set_bulk_read_cb(service, MI32_bridge_read_callback);
                hap_acc_add_serv(accessory, service);
                MI32saveHAPhandles(i,0x04,hap_serv_get_char_by_uuid(service, HAP_CHAR_UUID_CURRENT_TEMPERATURE));

                service = hap_serv_battery_service_create(99,0,0);
                hap_serv_set_bulk_read_cb(service, MI32_bridge_read_callback);
                hap_acc_add_serv(accessory, service);
                MI32saveHAPhandles(i,0x0a,hap_serv_get_char_by_uuid(service, HAP_CHAR_UUID_BATTERY_LEVEL));
            }
                break;
            case FLORA: case MJYD2S:
            {
                service = hap_serv_light_sensor_create(100.0f);
                hap_serv_set_bulk_read_cb(service, MI32_bridge_read_callback);
                hap_acc_add_serv(accessory, service);
                MI32saveHAPhandles(i,0x07,hap_serv_get_char_by_uuid(service, HAP_CHAR_UUID_CURRENT_AMBIENT_LIGHT_LEVEL));
                service = hap_serv_battery_service_create(50,0,0);
                hap_serv_set_bulk_read_cb(service, MI32_bridge_read_callback);
                hap_acc_add_serv(accessory, service);
                MI32saveHAPhandles(i,0x0a,hap_serv_get_char_by_uuid(service, HAP_CHAR_UUID_BATTERY_LEVEL));
                if(MI32getDeviceType(i) == MJYD2S){
                    service = hap_serv_motion_sensor_create(false);
                    hap_serv_set_bulk_read_cb(service, MI32_bridge_read_callback);
                    hap_acc_add_serv(accessory, service);
                    MI32saveHAPhandles(i,0x0f,hap_serv_get_char_by_uuid(service, HAP_CHAR_UUID_MOTION_DETECTED));
                }
                break;
            }
            case NLIGHT:
            {
                service = hap_serv_motion_sensor_create(false);
                hap_serv_set_bulk_read_cb(service, MI32_bridge_read_callback);
                hap_acc_add_serv(accessory, service);
                MI32saveHAPhandles(i,0x0f,hap_serv_get_char_by_uuid(service, HAP_CHAR_UUID_MOTION_DETECTED));
                
                service = hap_serv_battery_service_create(50,0,0);
                hap_serv_set_bulk_read_cb(service, MI32_bridge_read_callback);
                hap_acc_add_serv(accessory, service);
                MI32saveHAPhandles(i,0x0a,hap_serv_get_char_by_uuid(service, HAP_CHAR_UUID_BATTERY_LEVEL));
                break;
                //motion 0x0f
            }
            case MCCGQ02:
            {
                service = hap_serv_contact_sensor_create(0);
                hap_serv_set_bulk_read_cb(service, MI32_bridge_read_callback);
                hap_acc_add_serv(accessory, service);
                MI32saveHAPhandles(i,0x19,hap_serv_get_char_by_uuid(service, HAP_CHAR_UUID_CONTACT_SENSOR_STATE));
                service = hap_serv_battery_service_create(50,0,0);
                hap_serv_set_bulk_read_cb(service, MI32_bridge_read_callback);
                hap_acc_add_serv(accessory, service);
                MI32saveHAPhandles(i,0x0a,hap_serv_get_char_by_uuid(service, HAP_CHAR_UUID_BATTERY_LEVEL));
                break;
            }
            case YLYK01:
                {
                    bridge_cfg.cid = HAP_CID_PROGRAMMABLE_SWITCH;
                    hap_serv_t * _label = hap_serv_service_label_create(1);
                    hap_acc_add_serv(accessory, _label);
                    for(uint8_t _but=0;_but<6;_but++){
                        hap_serv_t * _newSwitch = hap_serv_stateless_programmable_switch_create(0);
                        const uint8_t _validVals[] = {0,2};
                        hap_char_add_valid_vals(hap_serv_get_char_by_uuid(_newSwitch, HAP_CHAR_UUID_PROGRAMMABLE_SWITCH_EVENT), _validVals, 2);
                        hap_char_t *_index = hap_char_service_label_index_create(_but+1);
                        hap_serv_add_char(_newSwitch,_index);
                        hap_acc_add_serv(accessory, _newSwitch);
                        MI32saveHAPhandles(i,_but+1000,hap_serv_get_char_by_uuid(_newSwitch, HAP_CHAR_UUID_PROGRAMMABLE_SWITCH_EVENT));
                    }
                }
                break;
            case YLKG08: //without the dimmer function due to lack of HomeKit support
                {
                    bridge_cfg.cid = HAP_CID_PROGRAMMABLE_SWITCH;
                    hap_serv_t * _label = hap_serv_service_label_create(1);
                    hap_acc_add_serv(accessory, _label);
                    hap_serv_t * _newSwitch = hap_serv_stateless_programmable_switch_create(0);
                    const uint8_t _validVals[] = {0,1,2};
                    hap_char_add_valid_vals(hap_serv_get_char_by_uuid(_newSwitch, HAP_CHAR_UUID_PROGRAMMABLE_SWITCH_EVENT), _validVals, 3);
                    hap_char_t *_index = hap_char_service_label_index_create(1);
                    hap_serv_add_char(_newSwitch,_index);
                    hap_acc_add_serv(accessory, _newSwitch);
                    MI32saveHAPhandles(i,1000,hap_serv_get_char_by_uuid(_newSwitch, HAP_CHAR_UUID_PROGRAMMABLE_SWITCH_EVENT));
                }
                break;
            case SJWS01L:
                service = hap_serv_leak_sensor_create(0);
                hap_serv_set_bulk_read_cb(service, MI32_bridge_read_callback);
                hap_acc_add_serv(accessory, service);
                MI32saveHAPhandles(i,0x14,hap_serv_get_char_by_uuid(service, HAP_CHAR_UUID_LEAK_DETECTED));
                hap_serv_t * _newSwitch = hap_serv_stateless_programmable_switch_create(0);
                const uint8_t _validVals[] = {0,2};
                hap_char_add_valid_vals(hap_serv_get_char_by_uuid(_newSwitch, HAP_CHAR_UUID_PROGRAMMABLE_SWITCH_EVENT), _validVals, 2);
                hap_acc_add_serv(accessory, _newSwitch);
                MI32saveHAPhandles(i,1000,hap_serv_get_char_by_uuid(_newSwitch, HAP_CHAR_UUID_PROGRAMMABLE_SWITCH_EVENT));
                service = hap_serv_battery_service_create(50,0,0);
                hap_serv_set_bulk_read_cb(service, MI32_bridge_read_callback);
                hap_acc_add_serv(accessory, service);
                MI32saveHAPhandles(i,0x0a,hap_serv_get_char_by_uuid(service, HAP_CHAR_UUID_BATTERY_LEVEL));
                break;
            default:
                break;
        }
        /* Add the Accessory to the HomeKit Database */
        hap_add_bridged_accessory(accessory, hap_get_unique_aid(accessory_name));
    }
    // add internal Tasmota devices
    for(uint32_t i = 0;i<MI32numOfRelays();i++){
        char _serialNum[2] = {i+49,0};

        hap_acc_cfg_t bridge_cfg = {
            .name = _serialNum,
            .manufacturer = "Xiaomi",
            .model = "Tasmota",
            .serial_num = _serialNum,
            .fw_rev = "0.9.1",
            .hw_rev = NULL,
            .pv = "1.1.0",
            .cid = HAP_CID_OUTLET,
            .identify_routine = MI32_accessory_identify,
        };
        /* Create accessory object */
        accessory = hap_acc_create(&bridge_cfg);
        service = hap_serv_outlet_create(false,true);
        hap_serv_set_bulk_read_cb(service, MI32_bridge_read_callback);
        hap_serv_set_write_cb(service, MI32_outlets_write_callback);
        hap_serv_set_priv(service, strdup(_serialNum));
        hap_acc_add_serv(accessory, service);

        MI32saveHAPhandles(i,0xf0,(void *)hap_serv_get_char_by_uuid(service, HAP_CHAR_UUID_ON));
        hap_add_bridged_accessory(accessory, hap_get_unique_aid(_serialNum));
    }

    hap_register_event_handler(mi_hap_event_handler);

    hap_set_setup_code(MI32getSetupCode());
    hap_set_setup_id(CONFIG_EXAMPLE_SETUP_ID);

    if (hap_start() == HAP_SUCCESS){
        MI32didStartHAP(true);
    }
    else MI32didStartHAP(false);
    /* The task ends here. The read/write callbacks will be invoked by the HAP Framework */
    vTaskDelete(NULL);
}

void mi_homekit_main(void){
    xTaskCreate(MI32_bridge_thread_entry, "MIBRIDGE", 8192, NULL, 5, NULL);
}

void mi_homekit_update_value(void* handle, float value, uint32_t type){
    if(handle == NULL) return;
    if(MIBridgeWasNeverConnected) return;
    hap_val_t new_val;
    switch(type){
        case 0x01: case 0x19: case 0x0a: case 0x14: case 0xf0:
            if(type == 0x19){
                value = 1-(uint8_t)value;
            }
            new_val.i = (uint8_t)value;
            break;
        case 0x0f:
            new_val.b = (value > 0.0f);
            break;
        default:
          new_val.f = value;  
    }
	int ret = hap_char_update_val((hap_char_t *)handle, &new_val);
    // if(ret!= HAP_SUCCESS){
        // ESP_LOGE(TAG,"error:",ret);
    // }
}

void mi_homekit_stop(){
    hap_stop();
}

#endif //USE_MI_ESP32

