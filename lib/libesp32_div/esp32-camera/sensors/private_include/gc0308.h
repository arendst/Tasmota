#pragma once

#include "sensor.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Detect sensor pid
 *
 * @param slv_addr SCCB address
 * @param id Detection result
 * @return
 *     0:       Can't detect this sensor
 *     Nonzero: This sensor has been detected
 */
int gc0308_detect(int slv_addr, sensor_id_t *id);

/**
 * @brief initialize sensor function pointers
 *
 * @param sensor pointer of sensor
 * @return
 *      Always 0
 */
int gc0308_init(sensor_t *sensor);

#ifdef __cplusplus
}
#endif
