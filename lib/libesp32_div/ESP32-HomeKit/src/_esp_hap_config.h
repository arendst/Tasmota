/*
 * _esp_hap_config.h
 *
 *  Created on: 2020-11-23
 *      Author: Mixiaoxiao
 */

#ifndef ESP_HAP_CONFIG_H_
#define ESP_HAP_CONFIG_H_

#define HAP_SDK_VER         "4.0"
#define MFI_VER       HAP_SDK_VER

#define CONFIG_HAP_HTTP_STACK_SIZE     12288
#define CONFIG_HAP_HTTP_SERVER_PORT     5556 // 80 for normal webserver
#define CONFIG_HAP_HTTP_CONTROL_PORT   32859
#define CONFIG_HAP_HTTP_MAX_OPEN_SOCKETS   5  // 6
#define CONFIG_HAP_HTTP_MAX_URI_HANDLERS  16

#endif /* ESP_HAP_CONFIG_H_ */
