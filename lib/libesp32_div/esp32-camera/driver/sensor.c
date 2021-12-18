#include <stdio.h>
#include "sensor.h"

const camera_sensor_info_t camera_sensor[CAMERA_MODEL_MAX] = {
    // The sequence must be consistent with camera_model_t
    {CAMERA_OV7725, "OV7725", OV7725_SCCB_ADDR, OV7725_PID, FRAMESIZE_VGA, false},
    {CAMERA_OV2640, "OV2640", OV2640_SCCB_ADDR, OV2640_PID, FRAMESIZE_UXGA, true},
    {CAMERA_OV3660, "OV3660", OV3660_SCCB_ADDR, OV3660_PID, FRAMESIZE_QXGA, true},
    {CAMERA_OV5640, "OV5640", OV5640_SCCB_ADDR, OV5640_PID, FRAMESIZE_QSXGA, true},
    {CAMERA_OV7670, "OV7670", OV7670_SCCB_ADDR, OV7670_PID, FRAMESIZE_VGA, false},
    {CAMERA_NT99141, "NT99141", NT99141_SCCB_ADDR, NT99141_PID, FRAMESIZE_HD, true},
    {CAMERA_GC2145, "GC2145", GC2145_SCCB_ADDR, GC2145_PID, FRAMESIZE_UXGA, false},
    {CAMERA_GC032A, "GC032A", GC032A_SCCB_ADDR, GC032A_PID, FRAMESIZE_VGA, false},
    {CAMERA_GC0308, "GC0308", GC0308_SCCB_ADDR, GC0308_PID, FRAMESIZE_VGA, false},
};

const resolution_info_t resolution[FRAMESIZE_INVALID] = {
    {   96,   96, ASPECT_RATIO_1X1   }, /* 96x96 */
    {  160,  120, ASPECT_RATIO_4X3   }, /* QQVGA */
    {  176,  144, ASPECT_RATIO_5X4   }, /* QCIF  */
    {  240,  176, ASPECT_RATIO_4X3   }, /* HQVGA */
    {  240,  240, ASPECT_RATIO_1X1   }, /* 240x240 */
    {  320,  240, ASPECT_RATIO_4X3   }, /* QVGA  */
    {  400,  296, ASPECT_RATIO_4X3   }, /* CIF   */
    {  480,  320, ASPECT_RATIO_3X2   }, /* HVGA  */
    {  640,  480, ASPECT_RATIO_4X3   }, /* VGA   */
    {  800,  600, ASPECT_RATIO_4X3   }, /* SVGA  */
    { 1024,  768, ASPECT_RATIO_4X3   }, /* XGA   */
    { 1280,  720, ASPECT_RATIO_16X9  }, /* HD    */
    { 1280, 1024, ASPECT_RATIO_5X4   }, /* SXGA  */
    { 1600, 1200, ASPECT_RATIO_4X3   }, /* UXGA  */
    // 3MP Sensors
    { 1920, 1080, ASPECT_RATIO_16X9  }, /* FHD   */
    {  720, 1280, ASPECT_RATIO_9X16  }, /* Portrait HD   */
    {  864, 1536, ASPECT_RATIO_9X16  }, /* Portrait 3MP   */
    { 2048, 1536, ASPECT_RATIO_4X3   }, /* QXGA  */
    // 5MP Sensors
    { 2560, 1440, ASPECT_RATIO_16X9  }, /* QHD    */
    { 2560, 1600, ASPECT_RATIO_16X10 }, /* WQXGA  */
    { 1088, 1920, ASPECT_RATIO_9X16  }, /* Portrait FHD   */
    { 2560, 1920, ASPECT_RATIO_4X3   }, /* QSXGA  */
};

camera_sensor_info_t *esp_camera_sensor_get_info(sensor_id_t *id)
{
    for (int i = 0; i < CAMERA_MODEL_MAX; i++) {
        if (id->PID == camera_sensor[i].pid) {
            return (camera_sensor_info_t *)&camera_sensor[i];
        }
    }
    return NULL;
}
