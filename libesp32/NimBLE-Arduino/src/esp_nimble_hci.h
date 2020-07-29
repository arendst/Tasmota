/*
 * Copyright 2019 Espressif Systems (Shanghai) PTE LTD
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#ifndef __ESP_NIMBLE_HCI_H__
#define __ESP_NIMBLE_HCI_H__

#include "nimble/ble_hci_trans.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BLE_HCI_UART_H4_NONE        0x00
#define BLE_HCI_UART_H4_CMD         0x01
#define BLE_HCI_UART_H4_ACL         0x02
#define BLE_HCI_UART_H4_SCO         0x03
#define BLE_HCI_UART_H4_EVT         0x04

/**
 * @brief Initialize VHCI transport layer between NimBLE Host and
 * ESP Bluetooth controller
 *
 * This function initializes the transport buffers to be exchanged
 * between NimBLE host and ESP controller. It also registers required
 * host callbacks with the controller.
 *
 * @return
 *    - ESP_OK if the initialization is successful
 *    - Appropriate error code from esp_err_t in case of an error
 */
esp_err_t esp_nimble_hci_init(void);

/**
 * @brief Initialize ESP Bluetooth controller(link layer) and VHCI transport
 * layer between NimBLE Host and ESP Bluetooth controller
 *
 * This function initializes ESP controller in BLE only mode and the
 * transport buffers to be exchanged between NimBLE host and ESP controller.
 * It also registers required host callbacks with the controller.
 *
 * Below is the sequence of APIs to be called to init/enable NimBLE host and ESP controller:
 *
 * @code{c}
 * void ble_host_task(void *param)
 * {
 *      nimble_port_run(); //This function will return only when nimble_port_stop() is executed.
 *      nimble_port_freertos_deinit();
 * }
 *
 * int ret = esp_nimble_hci_and_controller_init();
 * if (ret != ESP_OK) {
        ESP_LOGE(TAG, "esp_nimble_hci_and_controller_init() failed with error: %d", ret);
 *      return;
 * }
 *
 * nimble_port_init();
 *
 * //Initialize the NimBLE Host configuration
 *
 * nimble_port_freertos_init(ble_host_task);
 * @endcode
 *
 * nimble_port_freertos_init() is an optional call that creates a new task in which the NimBLE
 * host will run. The task function should have a call to nimble_port_run(). If a separate task
 * is not required, calling nimble_port_run() will run the NimBLE host in the current task.
 *
 * @return
 *    - ESP_OK if the initialization is successful
 *    - Appropriate error code from esp_err_t in case of an error
 */
esp_err_t esp_nimble_hci_and_controller_init(void);

/**
 * @brief Deinitialize VHCI transport layer between NimBLE Host and
 * ESP Bluetooth controller
 *
 * @note This function should be called after the NimBLE host is deinitialized.
 *
 * @return
 *    - ESP_OK if the deinitialization is successful
 *    - Appropriate error codes from esp_err_t in case of an error
 */
esp_err_t esp_nimble_hci_deinit(void);

/**
 * @brief Deinitialize VHCI transport layer between NimBLE Host and
 * ESP Bluetooth controller and disable and deinitialize the controller
 *
 * @note This function should not be executed in the context of Bluetooth host task.
 *
 * @note This function should be called after the NimBLE host is deinitialized.
 *
 * Below is the sequence of APIs to be called to disable/deinit NimBLE host and ESP controller:
 *
 * @code{c}
 * int ret = nimble_port_stop();
 * if (ret == 0) {
 *      nimble_port_deinit();
 *
 *      ret = esp_nimble_hci_and_controller_deinit();
 *      if (ret != ESP_OK) {
            ESP_LOGE(TAG, "esp_nimble_hci_and_controller_deinit() failed with error: %d", ret);
 *      }
 * }
 * @endcode
 *
 * If nimble_port_freertos_init() is used during initialization, then
 * nimble_port_freertos_deinit() should be called in the host task after nimble_port_run().
 *
 * @return
 *    - ESP_OK if the deinitialization is successful
 *    - Appropriate error codes from esp_err_t in case of an error
 */
esp_err_t esp_nimble_hci_and_controller_deinit(void);

#ifdef __cplusplus
}
#endif

#endif /* __ESP_NIMBLE_HCI_H__ */
