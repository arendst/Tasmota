/*
 * NimBLEUtils.h
 *
 *  Created: on Jan 25 2020
 *      Author H2zero
 *
 */

#ifndef COMPONENTS_NIMBLEUTILS_H_
#define COMPONENTS_NIMBLEUTILS_H_
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "host/ble_gap.h"

extern "C"{
char *addr_str(const void *addr);
void print_conn_desc(const struct ble_gap_conn_desc *desc);
void print_adv_fields(const struct ble_hs_adv_fields *fields);
void print_addr(const void *addr);
void print_bytes(const uint8_t *bytes, int len);
}

class NimBLEUtils {
public:
    static void                 dumpGapEvent(ble_gap_event *event, void *arg);
    static const char*          gapEventToString(uint8_t eventType);
    static char*                buildHexData(uint8_t* target, const uint8_t* source, uint8_t length);
    static const char*          advTypeToString(uint8_t advType);
    static const char*          returnCodeToString(int rc);
    static int                  checkConnParams(ble_gap_conn_params* params);
};


#endif // CONFIG_BT_ENABLED
#endif // COMPONENTS_NIMBLEUTILS_H_
