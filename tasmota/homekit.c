/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2018 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
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
/* HomeKit Smart Outlet Example
*/

//#define USE_HOMEKIT


#ifdef USE_HOMEKIT
#ifdef ESP32

#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <esp_log.h>
#include <driver/gpio.h>
#include <lwip/sockets.h>

#include <hap.h>

#include <hap_apple_servs.h>
#include <hap_apple_chars.h>
#include <hap_platform_keystore.h>

//#include <app_wifi.h>
//#include <app_hap_setup_payload.h>

static const char *TAG = "HAP outlet";
char *hk_desc;
char hk_code[12];
uint8_t hk_services;

extern void Ext_Replace_Cmd_Vars(char *srcbuf, uint32_t srcsize, char *dstbuf, uint32_t dstsize);
extern uint32_t Ext_UpdVar(char *vname, float *fvar, uint32_t mode);
extern void Ext_toLog(char *str);

#define MAX_HAP_DEFS 16
struct HAP_DESC {
  char hap_name[24];
  char var_name[12];
  char var2_name[12];
  char var3_name[12];
  char var4_name[12];
  char var5_name[12];
  uint8_t hap_cid;
  uint8_t type;
  hap_acc_t *accessory;
  hap_serv_t *service;
} hap_devs[MAX_HAP_DEFS];

#define HK_SRCBSIZE 256


#define SMART_OUTLET_TASK_PRIORITY  1
#define SMART_OUTLET_TASK_STACKSIZE 4 * 1024
#define SMART_OUTLET_TASK_NAME      "hap_outlet"

//#define OUTLET_IN_USE_GPIO GPIO_NUM_0
#define OUTLET_IN_USE_GPIO -1

#define ESP_INTR_FLAG_DEFAULT 0

static xQueueHandle s_esp_evt_queue = NULL;
/**
 * @brief the recover outlet in use gpio interrupt function
 */
static void IRAM_ATTR outlet_in_use_isr(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(s_esp_evt_queue, &gpio_num, NULL);
}

/**
 * Enable a GPIO Pin for Outlet in Use Detection
 */
static void outlet_in_use_key_init(uint32_t key_gpio_pin)
{
    gpio_config_t io_conf;
    /* Interrupt for both the edges  */
    io_conf.intr_type = GPIO_INTR_ANYEDGE;
    /* Bit mask of the pins */
    io_conf.pin_bit_mask = 1 << key_gpio_pin;
    /* Set as input mode */
    io_conf.mode = GPIO_MODE_INPUT;
    /* Enable internal pull-up */
    io_conf.pull_up_en = 1;
    /* Disable internal pull-down */
    io_conf.pull_down_en = 0;
    /* Set the GPIO configuration */
    gpio_config(&io_conf);

    /* Install gpio isr service */
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    /* Hook isr handler for specified gpio pin */
    gpio_isr_handler_add(key_gpio_pin, outlet_in_use_isr, (void*)key_gpio_pin);
}

/**
 * Initialize the Smart Outlet Hardware.Here, we just enebale the Outlet-In-Use detection.
 */
void smart_outlet_hardware_init(int32_t gpio_num) {
    s_esp_evt_queue = xQueueCreate(2, sizeof(uint32_t));

    if (gpio_num < 0) return;

    if (s_esp_evt_queue != NULL) {
        outlet_in_use_key_init(gpio_num);
    }
}


static int bridge_identify(hap_acc_t *ha)
{
    ESP_LOGI(TAG, "Bridge identified");
    return HAP_SUCCESS;
}

static int accessory_identify(hap_acc_t *ha)
{
    hap_serv_t *hs = hap_acc_get_serv_by_uuid(ha, HAP_SERV_UUID_ACCESSORY_INFORMATION);
    hap_char_t *hc = hap_serv_get_char_by_uuid(hs, HAP_CHAR_UUID_NAME);
    const hap_val_t *val = hap_char_get_val(hc);
    char *name = val->s;

    ESP_LOGI(TAG, "Bridged Accessory %s identified", name);
    return HAP_SUCCESS;
}

const struct HAP_CHAR_TABLE {
  char stype[4];
  char ntype;
  int8_t index;
} hap_rtab[] = {
  {HAP_CHAR_UUID_CURRENT_TEMPERATURE,'f',0},
  {HAP_CHAR_UUID_CURRENT_RELATIVE_HUMIDITY,'f',0},
  {HAP_CHAR_UUID_CURRENT_AMBIENT_LIGHT_LEVEL,'f',0},
  {HAP_CHAR_UUID_BATTERY_LEVEL,'u',0},
  {HAP_CHAR_UUID_STATUS_LOW_BATTERY,'u',1},
  {HAP_CHAR_UUID_CHARGING_STATE,'u',2},
  {HAP_CHAR_UUID_ON,'b',0},
  {HAP_CHAR_UUID_HUE,'f',1},
  {HAP_CHAR_UUID_SATURATION,'f',2},
  {HAP_CHAR_UUID_BRIGHTNESS,'u',3},
  {HAP_CHAR_UUID_COLOR_TEMPERATURE,'u',0},
  {HAP_CHAR_UUID_CONTACT_SENSOR_STATE,'u',0},
  {HAP_CHAR_UUID_WATTAGE,'f',0}
};

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

/* A dummy callback for handling a write on the "On" characteristic of Outlet.
 * In an actual accessory, this should control the hardware
 */
static int sensor_write(hap_write_data_t write_data[], int count, void *serv_priv, void *write_priv, uint32_t index) {
    int i, ret = HAP_SUCCESS;
    float fvar;
    hap_write_data_t *write;
    for (i = 0; i < count; i++) {
        write = &write_data[i];
        bool found = false;
        const char *hcp = hap_char_get_type_uuid(write->hc);
        for (uint32_t cnt = 0; cnt < ARRAY_SIZE(hap_rtab); cnt++ ) {
          if (!strcmp(hcp, hap_rtab[cnt].stype)) {
            hap_char_update_val(write->hc, &(write->val));
            switch (hap_rtab[cnt].ntype) {
              case 'f':  fvar = write->val.f; break;
              case 'u':  fvar = write->val.u; break;
              case 'b':  fvar = write->val.b; break;
            }
            switch (hap_rtab[cnt].index) {
              case 0: Ext_UpdVar(hap_devs[index].var_name, &fvar, 1);break;
              case 1: Ext_UpdVar(hap_devs[index].var2_name, &fvar, 1);break;
              case 2: Ext_UpdVar(hap_devs[index].var3_name, &fvar, 1);break;
              case 3: Ext_UpdVar(hap_devs[index].var4_name, &fvar, 1);break;
            }
            *(write->status) = HAP_STATUS_SUCCESS;
            found = true;
            break;
          }
        }
        if (!found) {
          *(write->status) = HAP_STATUS_RES_ABSENT;
        }

    }
    return ret;
}

// common read routine
static int sensor_read(hap_char_t *hc, hap_status_t *status_code, void *serv_priv, void *read_priv, uint32_t index) {
  hap_val_t new_val;
  float fvar = 0;

    if (hap_req_get_ctrl_id(read_priv)) {
        ESP_LOGI(TAG, "Received read from %s", hap_req_get_ctrl_id(read_priv));
    }

    const char *hcp = hap_char_get_type_uuid(hc);

    printf("read values %s\n", hcp );

    for (uint32_t cnt = 0; cnt < ARRAY_SIZE(hap_rtab); cnt++ ) {
      if (!strcmp(hcp, hap_rtab[cnt].stype)) {
        switch (hap_rtab[cnt].index) {
          case 0: Ext_UpdVar(hap_devs[index].var_name, &fvar, 0);break;
          case 1: Ext_UpdVar(hap_devs[index].var2_name, &fvar, 0);break;
          case 2: Ext_UpdVar(hap_devs[index].var3_name, &fvar, 0);break;
          case 3: Ext_UpdVar(hap_devs[index].var4_name, &fvar, 0);break;
        }
        switch (hap_rtab[cnt].ntype) {
          case 'f':  new_val.f = fvar; break;
          case 'u':  new_val.u = fvar; break;
          case 'b':  new_val.b = fvar; break;
        }
        hap_char_update_val(hc, &new_val);
        *status_code = HAP_STATUS_SUCCESS;
      }
    }
    return HAP_SUCCESS;
}

// update values every 500 ms
void hap_update_from_vars(void) {
    float fvar;
    hap_char_t *hc;
    hap_val_t new_val;
    for (uint32_t cnt = 0; cnt < hk_services; cnt++) {
      switch (hap_devs[cnt].hap_cid) {
        case HAP_CID_SENSOR:
          switch (hap_devs[cnt].type) {
            case 0:
              hc = hap_serv_get_char_by_uuid(hap_devs[cnt].service, HAP_CHAR_UUID_CURRENT_TEMPERATURE);
              if (Ext_UpdVar(hap_devs[cnt].var_name, &fvar, 0)) {
                new_val.f = fvar;
                hap_char_update_val(hc, &new_val);
              }
              break;
            case 1:
              hc = hap_serv_get_char_by_uuid(hap_devs[cnt].service, HAP_CHAR_UUID_CURRENT_RELATIVE_HUMIDITY);
              if (Ext_UpdVar(hap_devs[cnt].var_name, &fvar, 0)) {
                new_val.f = fvar;
                hap_char_update_val(hc, &new_val);
              }
              break;
            case 2:
              hc = hap_serv_get_char_by_uuid(hap_devs[cnt].service, HAP_CHAR_UUID_CURRENT_AMBIENT_LIGHT_LEVEL);
              if (Ext_UpdVar(hap_devs[cnt].var_name, &fvar, 0)) {
                new_val.f = fvar;
                hap_char_update_val(hc, &new_val);
              }
              break;
            case 3:
              hc = hap_serv_get_char_by_uuid(hap_devs[cnt].service, HAP_CHAR_UUID_BATTERY_LEVEL);
              if (Ext_UpdVar(hap_devs[cnt].var_name, &fvar, 0)) {
                new_val.u = fvar;
                hap_char_update_val(hc, &new_val);
              }
              hc = hap_serv_get_char_by_uuid(hap_devs[cnt].service, HAP_CHAR_UUID_STATUS_LOW_BATTERY);
              if (Ext_UpdVar(hap_devs[cnt].var2_name, &fvar, 0)) {
                new_val.u = fvar;
                hap_char_update_val(hc, &new_val);
              }
              hc = hap_serv_get_char_by_uuid(hap_devs[cnt].service, HAP_CHAR_UUID_CHARGING_STATE);
              if (Ext_UpdVar(hap_devs[cnt].var3_name, &fvar, 0)) {
                new_val.u = fvar;
                hap_char_update_val(hc, &new_val);
              }
              break;
            case 4:
              hc = hap_serv_get_char_by_uuid(hap_devs[cnt].service, HAP_CHAR_UUID_CURRENT_AMBIENT_LIGHT_LEVEL);
              if (Ext_UpdVar(hap_devs[cnt].var_name, &fvar, 0)) {
                new_val.f = fvar;
                hap_char_update_val(hc, &new_val);
              }
              break;
            case 5:
              hc = hap_serv_get_char_by_uuid(hap_devs[cnt].service, HAP_CHAR_UUID_CONTACT_SENSOR_STATE);
              if (Ext_UpdVar(hap_devs[cnt].var_name, &fvar, 0)) {
                new_val.u = fvar;
                hap_char_update_val(hc, &new_val);
              }
              break;
          }
          break;
        case HAP_CID_OUTLET:
          hc = hap_serv_get_char_by_uuid(hap_devs[cnt].service, HAP_CHAR_UUID_ON);
          if (Ext_UpdVar(hap_devs[cnt].var_name, &fvar, 0)) {
            new_val.b = fvar;
            hap_char_update_val(hc, &new_val);
          }
          break;
        case HAP_CID_LIGHTING:
          hc = hap_serv_get_char_by_uuid(hap_devs[cnt].service, HAP_CHAR_UUID_ON);
          if (Ext_UpdVar(hap_devs[cnt].var_name, &fvar, 0)) {
            new_val.b = fvar;
            hap_char_update_val(hc, &new_val);
          }
          hc = hap_serv_get_char_by_uuid(hap_devs[cnt].service, HAP_CHAR_UUID_HUE);
          if (Ext_UpdVar(hap_devs[cnt].var2_name, &fvar, 0)) {
            new_val.f = fvar;
            hap_char_update_val(hc, &new_val);
          }
          hc = hap_serv_get_char_by_uuid(hap_devs[cnt].service, HAP_CHAR_UUID_SATURATION);
          if (Ext_UpdVar(hap_devs[cnt].var3_name, &fvar, 0)) {
            new_val.f = fvar;
            hap_char_update_val(hc, &new_val);
          }
          hc = hap_serv_get_char_by_uuid(hap_devs[cnt].service, HAP_CHAR_UUID_BRIGHTNESS);
          if (Ext_UpdVar(hap_devs[cnt].var4_name, &fvar, 0)) {
            new_val.u = fvar;
            hap_char_update_val(hc, &new_val);
          }
          if (hap_devs[cnt].var5_name[0]) {
            hc = hap_serv_get_char_by_uuid(hap_devs[cnt].service, HAP_CHAR_UUID_COLOR_TEMPERATURE);
            if (Ext_UpdVar(hap_devs[cnt].var5_name, &fvar, 0)) {
              new_val.u = fvar;
              hap_char_update_val(hc, &new_val);
            }
          }
          break;
        }
      }

}

#define HAP_READ hap_char_t *hc, hap_status_t *status_code, void *serv_priv, void *read_priv) {  return sensor_read(hc, status_code, serv_priv, read_priv

static int sensor_read1(HAP_READ, 0);}
static int sensor_read2(HAP_READ, 1);}
static int sensor_read3(HAP_READ, 2);}
static int sensor_read4(HAP_READ, 3);}
static int sensor_read5(HAP_READ, 4);}
static int sensor_read6(HAP_READ, 5);}
static int sensor_read7(HAP_READ, 6);}
static int sensor_read8(HAP_READ, 7);}
static int sensor_read9(HAP_READ, 8);}
static int sensor_read10(HAP_READ, 9);}
static int sensor_read11(HAP_READ, 10);}
static int sensor_read12(HAP_READ, 11);}
static int sensor_read13(HAP_READ, 12);}
static int sensor_read14(HAP_READ, 13);}
static int sensor_read15(HAP_READ, 14);}
static int sensor_read16(HAP_READ, 15);}

void hap_set_read(hap_serv_t *service, uint32_t index) {
  switch (index) {
    case 0: hap_serv_set_read_cb(service, sensor_read1);break;
    case 1: hap_serv_set_read_cb(service, sensor_read2);break;
    case 2: hap_serv_set_read_cb(service, sensor_read3);break;
    case 3: hap_serv_set_read_cb(service, sensor_read4);break;
    case 4: hap_serv_set_read_cb(service, sensor_read5);break;
    case 5: hap_serv_set_read_cb(service, sensor_read6);break;
    case 6: hap_serv_set_read_cb(service, sensor_read7);break;
    case 7: hap_serv_set_read_cb(service, sensor_read8);break;
    case 8: hap_serv_set_read_cb(service, sensor_read9);break;
    case 9: hap_serv_set_read_cb(service, sensor_read10);break;
    case 10: hap_serv_set_read_cb(service, sensor_read11);break;
    case 11: hap_serv_set_read_cb(service, sensor_read12);break;
    case 12: hap_serv_set_read_cb(service, sensor_read13);break;
    case 13: hap_serv_set_read_cb(service, sensor_read14);break;
    case 14: hap_serv_set_read_cb(service, sensor_read15);break;
    case 15: hap_serv_set_read_cb(service, sensor_read16);break;
  }
}


#define HAP_WRITE hap_write_data_t write_data[], int count, void *serv_priv, void *write_priv) {  return sensor_write(write_data, count, serv_priv, write_priv

static int sensor_write1(HAP_WRITE, 0);}
static int sensor_write2(HAP_WRITE, 1);}
static int sensor_write3(HAP_WRITE, 2);}
static int sensor_write4(HAP_WRITE, 3);}
static int sensor_write5(HAP_WRITE, 4);}
static int sensor_write6(HAP_WRITE, 5);}
static int sensor_write7(HAP_WRITE, 6);}
static int sensor_write8(HAP_WRITE, 7);}
static int sensor_write9(HAP_WRITE, 8);}
static int sensor_write10(HAP_WRITE, 9);}
static int sensor_write11(HAP_WRITE, 10);}
static int sensor_write12(HAP_WRITE, 11);}
static int sensor_write13(HAP_WRITE, 12);}
static int sensor_write14(HAP_WRITE, 13);}
static int sensor_write15(HAP_WRITE, 14);}
static int sensor_write16(HAP_WRITE, 15);}


void hap_set_write(hap_serv_t *service, uint32_t index) {
  switch (index) {
    case 0: hap_serv_set_write_cb(service, sensor_write1);break;
    case 1: hap_serv_set_write_cb(service, sensor_write2);break;
    case 2: hap_serv_set_write_cb(service, sensor_write3);break;
    case 3: hap_serv_set_write_cb(service, sensor_write4);break;
    case 4: hap_serv_set_write_cb(service, sensor_write5);break;
    case 5: hap_serv_set_write_cb(service, sensor_write6);break;
    case 6: hap_serv_set_write_cb(service, sensor_write7);break;
    case 7: hap_serv_set_write_cb(service, sensor_write8);break;
    case 8: hap_serv_set_write_cb(service, sensor_write9);break;
    case 9: hap_serv_set_write_cb(service, sensor_write10);break;
    case 10: hap_serv_set_write_cb(service, sensor_write11);break;
    case 11: hap_serv_set_write_cb(service, sensor_write12);break;
    case 12: hap_serv_set_write_cb(service, sensor_write13);break;
    case 13: hap_serv_set_write_cb(service, sensor_write14);break;
    case 14: hap_serv_set_write_cb(service, sensor_write15);break;
    case 15: hap_serv_set_write_cb(service, sensor_write16);break;
  }
}


uint32_t HK_getlinelen(char *lp) {
uint32_t cnt;
  for (cnt=0; cnt<HK_SRCBSIZE-1; cnt++) {
    if (lp[cnt]=='\n') {
      break;
    }
  }
  return cnt;
}

float tsim = 20;

uint32_t str2c(char **sp, char *vp, uint32_t len) {
    char *lp = *sp;
    if (len) len--;
    char *cp = strchr(lp, ',');
    if (cp) {
        while (1) {
            if (*lp == ',') {
                *vp = 0;
                *sp = lp + 1;
                return 0;
            }
            if (len) {
                *vp++ = *lp++;
                len--;
            } else {
                lp++;
            }
        }
    } else {
      if (strlen(*sp)) {
        strlcpy(vp, *sp, len);
        *sp = lp + strlen(*sp);
        return 0;
      }
    }
    return 1;
}

extern char *GetFName();

/*The main thread for handling the Smart Outlet Accessory */
static void smart_outlet_thread_entry(void *p) {
    /* Initialize the HAP core */
    hap_init(HAP_TRANSPORT_WIFI);

    hap_acc_t *accessory;

    hap_acc_cfg_t cfg = {
        //.name = "Tasmota-Bridge",
        .name = GetFName(),
        .manufacturer = "Tasmota",
        .model = "Bridge",
        .serial_num = "001122334455",
        .fw_rev = "0.9.0",
        .hw_rev = NULL,
        .pv = "1.1.0",
        .identify_routine = bridge_identify,
        .cid = HAP_CID_BRIDGE,
    };
    /* Create accessory object */
    accessory = hap_acc_create(&cfg);

    /* Add a dummy Product Data */
    uint8_t product_data[] = {'E','S','P','3','2','H','A','P'};
    hap_acc_add_product_data(accessory, product_data, sizeof(product_data));

    /* Add the Accessory to the HomeKit Database */
    hap_add_accessory(accessory);

    /* Initialise the mandatory parameters for Accessory which will be added as
     * the mandatory services internally
     */
     hap_char_t *outlet_in_use;
    // get values from descriptor, line per line
    // name, cid, var
    char *lp = hk_desc;
    uint8_t index = 0;
    while (*lp) {
      if (*lp == '#') break;
      if (*lp == '\n') lp++;
      if (*lp == ' ') lp++;
      if (*lp == ';') goto nextline;

      char dstbuf[HK_SRCBSIZE*2];
      Ext_Replace_Cmd_Vars(lp, 1, dstbuf, sizeof(dstbuf));
      lp += HK_getlinelen(lp);

      char *lp1 = dstbuf;
      if (str2c(&lp1, hap_devs[index].hap_name, sizeof(hap_devs[index].hap_name))) {
        goto nextline;
      }
      hap_devs[index].hap_cid = strtol(lp1, &lp1, 10);
      lp1++;
      hap_devs[index].type = strtol(lp1, &lp1, 10);
      lp1++;
      if (str2c(&lp1, hap_devs[index].var_name, sizeof(hap_devs[index].var_name))) {
        goto nextline;
      }

      hap_devs[index].var2_name[0] = 0;
      hap_devs[index].var3_name[0] = 0;
      hap_devs[index].var4_name[0] = 0;
      hap_devs[index].var5_name[0] = 0;

      str2c(&lp1, hap_devs[index].var2_name, sizeof(hap_devs[index].var2_name));
      str2c(&lp1, hap_devs[index].var3_name, sizeof(hap_devs[index].var3_name));
      str2c(&lp1, hap_devs[index].var4_name, sizeof(hap_devs[index].var4_name));
      str2c(&lp1, hap_devs[index].var5_name, sizeof(hap_devs[index].var5_name));

      hap_acc_cfg_t hap_cfg;
      hap_cfg.name = hap_devs[index].hap_name;
      hap_cfg.manufacturer = "Tasmota";
      hap_cfg.model = "Tasmota Device";
      hap_cfg.serial_num = "001122334455";
      hap_cfg.fw_rev = "0.9.0";
      hap_cfg.hw_rev = NULL;
      hap_cfg.pv = "1.1.0";
      hap_cfg.identify_routine = accessory_identify;
      hap_cfg.cid =  hap_devs[index].hap_cid;

      /* Create accessory object */
      hap_devs[index].accessory = hap_acc_create(&hap_cfg);
      /* Add a dummy Product Data */
      hap_acc_add_product_data(hap_devs[index].accessory, product_data, sizeof(product_data));

      int ret;

      switch (hap_cfg.cid) {
        case HAP_CID_LIGHTING:
          { float fvar = 0;
            Ext_UpdVar(hap_devs[index].var_name, &fvar, 0);
            hap_devs[index].service = hap_serv_lightbulb_create(fvar);
            if (hap_devs[index].var2_name[0]) {
              Ext_UpdVar(hap_devs[index].var2_name, &fvar, 0);
              ret |= hap_serv_add_char(hap_devs[index].service, hap_char_hue_create(fvar));
            }
            if (hap_devs[index].var3_name[0]) {
              Ext_UpdVar(hap_devs[index].var3_name, &fvar, 0);
              ret |= hap_serv_add_char(hap_devs[index].service, hap_char_saturation_create(fvar));
            }
            Ext_UpdVar(hap_devs[index].var4_name, &fvar, 0);
            ret |= hap_serv_add_char(hap_devs[index].service, hap_char_brightness_create(fvar));
            if (hap_devs[index].var5_name[0]) {
              Ext_UpdVar(hap_devs[index].var5_name, &fvar, 0);
              ret |= hap_serv_add_char(hap_devs[index].service, hap_char_color_temperature_create(fvar));
            }
          }
          break;
        case HAP_CID_OUTLET:
          { float fvar = 0;
            Ext_UpdVar(hap_devs[index].var_name, &fvar, 0);
            hap_devs[index].service = hap_serv_outlet_create(fvar, true);
          }
          break;
        case HAP_CID_SENSOR:
          { float fvar = 22;
            Ext_UpdVar(hap_devs[index].var_name, &fvar, 0);
            switch (hap_devs[index].type) {
              case 0: hap_devs[index].service = hap_serv_temperature_sensor_create(fvar); break;
              case 1: hap_devs[index].service = hap_serv_humidity_sensor_create(fvar); break;
              case 2: hap_devs[index].service = hap_serv_light_sensor_create(fvar); break;
              case 3:
                { float fvar1 = 0, fvar2 = 0;
                  Ext_UpdVar(hap_devs[index].var2_name, &fvar1, 0);
                  Ext_UpdVar(hap_devs[index].var3_name, &fvar2, 0);
                  hap_devs[index].service = hap_serv_battery_service_create(fvar, fvar1, fvar2);
                }
                break;
              case 4: hap_devs[index].service = hap_serv_wattage_create(fvar); break;
              case 5: hap_devs[index].service = hap_serv_contact_sensor_create(fvar); break;
            }
          }
          break;


        default:
          hap_devs[index].service = hap_serv_outlet_create(true, true);
      }
      hap_serv_add_char(hap_devs[index].service, hap_char_name_create(hap_devs[index].hap_name));
      hap_set_read(hap_devs[index].service, index);
      hap_set_write(hap_devs[index].service, index);

      /* Get pointer to the outlet in use characteristic which we need to monitor for state changes */
      outlet_in_use = hap_serv_get_char_by_uuid(hap_devs[index].service, HAP_CHAR_UUID_OUTLET_IN_USE);

      /* Add the Outlet Service to the Accessory Object */
      hap_acc_add_serv(hap_devs[index].accessory, hap_devs[index].service);

      /* Add the Accessory to the HomeKit Database */
      hap_add_bridged_accessory(hap_devs[index].accessory, hap_get_unique_aid(hap_devs[index].hap_name));

      index++;

nextline:
      if (*lp=='\n') {
        lp++;
      } else {
        lp = strchr(lp, '\n');
        if (!lp) break;
        lp++;
      }
    }
    hk_services = index;

    /* Initialize the appliance specific hardware. This enables out-in-use detection */
    smart_outlet_hardware_init(OUTLET_IN_USE_GPIO);

    /* For production accessories, the setup code shouldn't be programmed on to
     * the device. Instead, the setup info, derived from the setup code must
     * be used. Use the factory_nvs_gen utility to generate this data and then
     * flash it into the factory NVS partition.
     *
     * By default, the setup ID and setup info will be read from the factory_nvs
     * Flash partition and so, is not required to set here explicitly.
     *
     * However, for testing purpose, this can be overridden by using hap_set_setup_code()
     * and hap_set_setup_id() APIs, as has been done here.
     */
    hap_set_setup_code(hk_code);
    hap_set_setup_id("ES32");
#ifdef CONFIG_EXAMPLE_USE_HARDCODED_SETUP_CODE
    /* Unique Setup code of the format xxx-xx-xxx. Default: 111-22-333 */
    hap_set_setup_code(CONFIG_EXAMPLE_SETUP_CODE);
    /* Unique four character Setup Id. Default: ES32 */
    hap_set_setup_id(CONFIG_EXAMPLE_SETUP_ID);
#ifdef CONFIG_APP_WIFI_USE_WAC_PROVISIONING
    app_hap_setup_payload(CONFIG_EXAMPLE_SETUP_CODE, CONFIG_EXAMPLE_SETUP_ID, true, hap_devs[0].hap_cid);
#else
    app_hap_setup_payload(CONFIG_EXAMPLE_SETUP_CODE, CONFIG_EXAMPLE_SETUP_ID, false, hap_devs[0].hap_cid);
#endif
#endif // CONFIG_EXAMPLE_USE_HARDCODED_SETUP_CODE

    /* Enable Hardware MFi authentication (applicable only for MFi variant of SDK) */
    hap_enable_mfi_auth(HAP_MFI_AUTH_HW);

    /* After all the initializations are done, start the HAP core */
    hap_start();

    int32_t io_num = OUTLET_IN_USE_GPIO;
    if (io_num >= 0) {
        hap_val_t appliance_value = {
          .b = true,
        };

        /* Listen for Outlet-In-Use state change events. Other read/write functionality will be handled
        * by the HAP Core.
        * When the Outlet in Use GPIO goes low, it means Outlet is not in use.
        * When the Outlet in Use GPIO goes high, it means Outlet is in use.
        * Applications can define own logic as per their hardware.
        */
        while (1) {
          if (xQueueReceive(s_esp_evt_queue, &io_num, portMAX_DELAY) == pdFALSE) {
            ESP_LOGI(TAG, "Outlet-In-Use trigger FAIL");
          } else {
            appliance_value.b = gpio_get_level(io_num);
            /* If any state change is detected, update the Outlet In Use characteristic value */
            hap_char_update_val(outlet_in_use, &appliance_value);
            ESP_LOGI(TAG, "Outlet-In-Use triggered [%d]", appliance_value.b);
          }
        }
    } else {
    //  vTaskDelete(NULL);
      while (1) {
        delay(500);
        hap_update_from_vars();
      }
    }
}

#define HK_PASSCODE "111-11-111"
int hap_loop_stop(void);
int32_t homekit_pars(uint32_t sel);

int32_t homekit_main(char *desc, uint32_t flag ) {
  if (desc) {
    char *cp = desc;
    cp += 2;
    while (*cp == ' ') cp++;
    // "111-11-111"

    if (*cp == '*') {
      strlcpy(hk_code, HK_PASSCODE, 10);
      cp++;
    } else {
      uint32_t cnt;
      for (cnt = 0; cnt < 10; cnt++) {
        hk_code[cnt] = *cp++;
      }
      hk_code[cnt] = 0;
    }
    if (*cp != '\n') {
      printf("init error\n");
      return -1;
    }
    cp++;
    hk_desc = cp;
  } else {
    if (flag == 99) {
      hap_loop_stop();
      hap_reset_to_factory();
    } else if (flag == 98) {
      hap_loop_stop();
      // is just the folder in wrapper
      hap_platfrom_keystore_erase_partition(hap_platform_keystore_get_nvs_partition_name());
    } else if (flag < 3) {
      return homekit_pars(flag);
    } else {
      hap_loop_stop();
    }

    return 0;
  }

  if (!hk_desc) return -2;

  /* Create the application thread */
  xTaskCreate(smart_outlet_thread_entry, SMART_OUTLET_TASK_NAME, SMART_OUTLET_TASK_STACKSIZE, NULL, SMART_OUTLET_TASK_PRIORITY, NULL);

  return 0;
}

#include <lwip/sockets.h>

int32_t homekit_pars(uint32_t sel) {

  if (sel == 0) {
  //  return CONFIG_LWIP_MAX_SOCKETS;
    return MEMP_NUM_NETCONN;
  } else if (sel == 1) {
    return LWIP_SOCKET_OFFSET;
  } else {
    struct sockaddr name;
    socklen_t len = sizeof(name);
    uint8_t open_socs = 0;
    for (uint32_t cnt = 0; cnt < CONFIG_LWIP_MAX_SOCKETS; cnt++) {
      //if (!getsockname(cnt, &name, &len)) {
      if (!getpeername(LWIP_SOCKET_OFFSET + cnt, &name, &len)) {
        open_socs++;
      }
    }
    return open_socs;
  }
}

#endif // ESP32
#endif // USE_HOMEKIT
