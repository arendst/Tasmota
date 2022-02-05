/*
 * NimBLEUtils.h
 *
 *  Created: on Jan 25 2020
 *      Author H2zero
 *
 */

#ifndef COMPONENTS_NIMBLEUTILS_H_
#define COMPONENTS_NIMBLEUTILS_H_

#include "nimconfig.h"
#if defined(CONFIG_BT_ENABLED)

#if defined(CONFIG_NIMBLE_CPP_IDF)
#include "host/ble_gap.h"
#else
#include "nimble/nimble/host/include/host/ble_gap.h"
#endif

/****  FIX COMPILATION ****/
#undef min
#undef max
/**************************/

#include <string>

typedef struct {
    void *pATT;
    TaskHandle_t task;
    int rc;
    void *buf;
} ble_task_data_t;


/**
 * @brief A BLE Utility class with methods for debugging and general purpose use.
 */
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
