
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "unity.h"
#include <mbedtls/base64.h>
#include "esp_log.h"

#include "esp_camera.h"

#ifdef CONFIG_IDF_TARGET_ESP32
#define BOARD_WROVER_KIT 1
#elif defined CONFIG_IDF_TARGET_ESP32S2
#define BOARD_CAMERA_MODEL_ESP32S2 1
#elif defined CONFIG_IDF_TARGET_ESP32S3
#define BOARD_CAMERA_MODEL_ESP32_S3_EYE 1
#endif

// WROVER-KIT PIN Map
#if BOARD_WROVER_KIT

#define PWDN_GPIO_NUM -1  //power down is not used
#define RESET_GPIO_NUM -1 //software reset will be performed
#define XCLK_GPIO_NUM 21
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27

#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 19
#define Y4_GPIO_NUM 18
#define Y3_GPIO_NUM 5
#define Y2_GPIO_NUM 4
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

// ESP32Cam (AiThinker) PIN Map
#elif BOARD_ESP32CAM_AITHINKER

#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1 //software reset will be performed
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27

#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

#elif BOARD_CAMERA_MODEL_ESP32S2

#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    -1

#define VSYNC_GPIO_NUM    21
#define HREF_GPIO_NUM     38
#define PCLK_GPIO_NUM     11
#define XCLK_GPIO_NUM     40

#define SIOD_GPIO_NUM     17
#define SIOC_GPIO_NUM     18

#define Y9_GPIO_NUM       39
#define Y8_GPIO_NUM       41
#define Y7_GPIO_NUM       42
#define Y6_GPIO_NUM       12
#define Y5_GPIO_NUM       3
#define Y4_GPIO_NUM       14
#define Y3_GPIO_NUM       37
#define Y2_GPIO_NUM       13

#elif BOARD_CAMERA_MODEL_ESP32_S3_EYE

#define PWDN_GPIO_NUM     43
#define RESET_GPIO_NUM    44

#define VSYNC_GPIO_NUM    6
#define HREF_GPIO_NUM     7
#define PCLK_GPIO_NUM     13
#define XCLK_GPIO_NUM     15

#define SIOD_GPIO_NUM     4
#define SIOC_GPIO_NUM     5

#define Y9_GPIO_NUM       16
#define Y8_GPIO_NUM       17
#define Y7_GPIO_NUM       18
#define Y6_GPIO_NUM       12
#define Y5_GPIO_NUM       11
#define Y4_GPIO_NUM       10
#define Y3_GPIO_NUM       9
#define Y2_GPIO_NUM       8

#endif

static const char *TAG = "test camera";

typedef void (*decode_func_t)(uint8_t *jpegbuffer, uint32_t size, uint8_t *outbuffer);

static esp_err_t init_camera(uint32_t xclk_freq_hz, pixformat_t pixel_format, framesize_t frame_size, uint8_t fb_count)
{
    framesize_t size_bak = frame_size;
    if (PIXFORMAT_JPEG == pixel_format && FRAMESIZE_SVGA > frame_size) {
        frame_size = FRAMESIZE_HD;
    }
    camera_config_t camera_config = {
        .pin_pwdn = PWDN_GPIO_NUM,
        .pin_reset = RESET_GPIO_NUM,
        .pin_xclk = XCLK_GPIO_NUM,
        .pin_sscb_sda = SIOD_GPIO_NUM,
        .pin_sscb_scl = SIOC_GPIO_NUM,

        .pin_d7 = Y9_GPIO_NUM,
        .pin_d6 = Y8_GPIO_NUM,
        .pin_d5 = Y7_GPIO_NUM,
        .pin_d4 = Y6_GPIO_NUM,
        .pin_d3 = Y5_GPIO_NUM,
        .pin_d2 = Y4_GPIO_NUM,
        .pin_d1 = Y3_GPIO_NUM,
        .pin_d0 = Y2_GPIO_NUM,
        .pin_vsync = VSYNC_GPIO_NUM,
        .pin_href = HREF_GPIO_NUM,
        .pin_pclk = PCLK_GPIO_NUM,

        //EXPERIMENTAL: Set to 16MHz on ESP32-S2 or ESP32-S3 to enable EDMA mode
        .xclk_freq_hz = xclk_freq_hz,
        .ledc_timer = LEDC_TIMER_0,
        .ledc_channel = LEDC_CHANNEL_0,

        .pixel_format = pixel_format, //YUV422,GRAYSCALE,RGB565,JPEG
        .frame_size = frame_size,    //QQVGA-UXGA Do not use sizes above QVGA when not JPEG

        .jpeg_quality = 12, //0-63 lower number means higher quality
        .fb_count = fb_count,       //if more than one, i2s runs in continuous mode. Use only with JPEG
        .grab_mode = CAMERA_GRAB_WHEN_EMPTY
    };

    //initialize the camera
    esp_err_t ret = esp_camera_init(&camera_config);

    if (ESP_OK == ret && PIXFORMAT_JPEG == pixel_format && FRAMESIZE_SVGA > size_bak) {
        sensor_t *s = esp_camera_sensor_get();
        s->set_framesize(s, size_bak);
    }

    return ret;
}

static bool camera_test_fps(uint16_t times, float *fps, uint32_t *size)
{
    *fps = 0.0f;
    *size = 0;
    uint32_t s = 0;
    uint32_t num = 0;
    uint64_t total_time = esp_timer_get_time();
    for (size_t i = 0; i < times; i++) {
        camera_fb_t *pic = esp_camera_fb_get();
        if (NULL == pic) {
            ESP_LOGW(TAG, "fb get failed");
            return 0;
        } else {
            s += pic->len;
            num++;
        }
        esp_camera_fb_return(pic);
    }
    total_time = esp_timer_get_time() - total_time;
    if (num) {
        *fps = num * 1000000.0f / total_time ;
        *size = s / num;
    }
    return 1;
}

static const char *get_cam_format_name(pixformat_t pixel_format)
{
    switch (pixel_format) {
    case PIXFORMAT_JPEG: return "JPEG";
    case PIXFORMAT_RGB565: return "RGB565";
    case PIXFORMAT_RGB888: return "RGB888";
    case PIXFORMAT_YUV422: return "YUV422";
    default:
        break;
    }
    return "UNKNOW";
}

static void printf_img_base64(const camera_fb_t *pic)
{
    uint8_t *outbuffer = NULL;
    size_t outsize = 0;
    if (PIXFORMAT_JPEG != pic->format) {
        fmt2jpg(pic->buf, pic->width * pic->height * 2, pic->width, pic->height, pic->format, 50, &outbuffer, &outsize);
    } else {
        outbuffer = pic->buf;
        outsize = pic->len;
    }

    uint8_t *base64_buf = calloc(1, outsize * 4);
    if (NULL != base64_buf) {
        size_t out_len = 0;
        mbedtls_base64_encode(base64_buf, outsize * 4, &out_len, outbuffer, outsize);
        printf("%s\n", base64_buf);
        free(base64_buf);
        if (PIXFORMAT_JPEG != pic->format) {
            free(outbuffer);
        }
    } else {
        ESP_LOGE(TAG, "malloc for base64 buffer failed");
    }
}

static void camera_performance_test(uint32_t xclk_freq, uint32_t pic_num)
{
    esp_err_t ret = ESP_OK;
    //detect sensor information
    TEST_ESP_OK(init_camera(20000000, PIXFORMAT_RGB565, FRAMESIZE_QVGA, 2));
    sensor_t *s = esp_camera_sensor_get();
    camera_sensor_info_t *info = esp_camera_sensor_get_info(&s->id);
    TEST_ASSERT_NOT_NULL(info);
    TEST_ESP_OK(esp_camera_deinit());
    vTaskDelay(500 / portTICK_RATE_MS);
    framesize_t max_size = info->max_size;
    pixformat_t all_format[] = {PIXFORMAT_JPEG, PIXFORMAT_RGB565, PIXFORMAT_YUV422, };
    pixformat_t *format_s = &all_format[0];
    pixformat_t *format_e = &all_format[2];
    if (false == info->support_jpeg) {
        format_s++; // skip jpeg
    }

    struct fps_result {
        float fps[FRAMESIZE_INVALID];
        uint32_t size[FRAMESIZE_INVALID];
    };
    struct fps_result results[3] = {0};

    for (; format_s <= format_e; format_s++) {
        for (size_t i = 0; i <= max_size; i++) {
            ESP_LOGI(TAG, "\n\n===> Testing format:%s resolution: %d x %d <===", get_cam_format_name(*format_s), resolution[i].width, resolution[i].height);
            ret = init_camera(xclk_freq, *format_s, i, 2);
            vTaskDelay(100 / portTICK_RATE_MS);
            if (ESP_OK != ret) {
                ESP_LOGW(TAG, "Testing init failed :-(, skip this item");
                vTaskDelay(500 / portTICK_RATE_MS);
                continue;
            }
            camera_test_fps(pic_num, &results[format_s - all_format].fps[i], &results[format_s - all_format].size[i]);
            TEST_ESP_OK(esp_camera_deinit());
        }
    }

    printf("FPS Result\n");
    printf("resolution  ,  JPEG fps,  JPEG size, RGB565 fps, RGB565 size, YUV422 fps, YUV422 size \n");
    for (size_t i = 0; i <= max_size; i++) {
        printf("%4d x %4d ,     %5.2f,     %6d,      %5.2f,     %7d,      %5.2f,     %7d \n",
               resolution[i].width, resolution[i].height,
               results[0].fps[i], results[0].size[i],
               results[1].fps[i], results[1].size[i],
               results[2].fps[i], results[2].size[i]);
    }
    printf("----------------------------------------------------------------------------------------\n");
}

TEST_CASE("Camera driver init, deinit test", "[camera]")
{
    uint64_t t1 = esp_timer_get_time();
    TEST_ESP_OK(init_camera(20000000, PIXFORMAT_RGB565, FRAMESIZE_QVGA, 2));
    uint64_t t2 = esp_timer_get_time();
    ESP_LOGI(TAG, "Camera init time %llu ms", (t2 - t1) / 1000);

    TEST_ESP_OK(esp_camera_deinit());
}

TEST_CASE("Camera driver take RGB565 picture test", "[camera]")
{
    TEST_ESP_OK(init_camera(10000000, PIXFORMAT_RGB565, FRAMESIZE_QVGA, 2));
    vTaskDelay(500 / portTICK_RATE_MS);
    ESP_LOGI(TAG, "Taking picture...");
    camera_fb_t *pic = esp_camera_fb_get();
    if (pic) {
        ESP_LOGI(TAG, "picture: %d x %d, size: %u", pic->width, pic->height, pic->len);
        printf_img_base64(pic);
        esp_camera_fb_return(pic);
    }

    TEST_ESP_OK(esp_camera_deinit());
    TEST_ASSERT_NOT_NULL(pic);
}

TEST_CASE("Camera driver take YUV422 picture test", "[camera]")
{
    TEST_ESP_OK(init_camera(10000000, PIXFORMAT_YUV422, FRAMESIZE_QVGA, 2));
    vTaskDelay(500 / portTICK_RATE_MS);
    ESP_LOGI(TAG, "Taking picture...");
    camera_fb_t *pic = esp_camera_fb_get();
    if (pic) {
        ESP_LOGI(TAG, "picture: %d x %d, size: %u", pic->width, pic->height, pic->len);
        printf_img_base64(pic);
        esp_camera_fb_return(pic);
    }

    TEST_ESP_OK(esp_camera_deinit());
    TEST_ASSERT_NOT_NULL(pic);
}

TEST_CASE("Camera driver take JPEG picture test", "[camera]")
{
    TEST_ESP_OK(init_camera(20000000, PIXFORMAT_JPEG, FRAMESIZE_QVGA, 2));
    vTaskDelay(500 / portTICK_RATE_MS);
    ESP_LOGI(TAG, "Taking picture...");
    camera_fb_t *pic = esp_camera_fb_get();
    if (pic) {
        ESP_LOGI(TAG, "picture: %d x %d, size: %u", pic->width, pic->height, pic->len);
        printf_img_base64(pic);
        esp_camera_fb_return(pic);
    }

    TEST_ESP_OK(esp_camera_deinit());
    TEST_ASSERT_NOT_NULL(pic);
}

TEST_CASE("Camera driver performance test", "[camera]")
{
    camera_performance_test(20 * 1000000, 16);
}


static void print_rgb565_img(uint8_t *img, int width, int height)
{
    uint16_t *p = (uint16_t *)img;
    const char temp2char[17] = "@MNHQ&#UJ*x7^i;.";
    for (size_t j = 0; j < height; j++) {
        for (size_t i = 0; i < width; i++) {
            uint32_t c = p[j * width + i];
            uint8_t r = c >> 11;
            uint8_t g = (c >> 6) & 0x1f;
            uint8_t b = c & 0x1f;
            c = (r + g + b) / 3;
            c >>= 1;
            printf("%c", temp2char[15 - c]);
        }
        printf("\n");
    }
}

static void print_rgb888_img(uint8_t *img, int width, int height)
{
    uint8_t *p = (uint8_t *)img;
    const char temp2char[17] = "@MNHQ&#UJ*x7^i;.";
    for (size_t j = 0; j < height; j++) {
        for (size_t i = 0; i < width; i++) {
            uint8_t *c = p + 3 * (j * width + i);
            uint8_t r = *c++;
            uint8_t g = *c++;
            uint8_t b = *c;
            uint32_t v = (r + g + b) / 3;
            v >>= 4;
            printf("%c", temp2char[15 - v]);
        }
        printf("\n");
    }
}

static void tjpgd_decode_rgb565(uint8_t *mjpegbuffer, uint32_t size, uint8_t *outbuffer)
{
    jpg2rgb565(mjpegbuffer, size, outbuffer, JPG_SCALE_NONE);
}

static void tjpgd_decode_rgb888(uint8_t *mjpegbuffer, uint32_t size, uint8_t *outbuffer)
{
    fmt2rgb888(mjpegbuffer, size, PIXFORMAT_JPEG, outbuffer);
}

typedef enum {
    DECODE_RGB565,
    DECODE_RGB888,
} decode_type_t;

static const decode_func_t g_decode_func[2][2] = {
    {tjpgd_decode_rgb565,},
    {tjpgd_decode_rgb888,},
};


static float jpg_decode_test(uint8_t decoder_index, decode_type_t type, const uint8_t *jpg, uint32_t length, uint32_t img_w, uint32_t img_h, uint32_t times)
{
    uint8_t *jpg_buf = malloc(length);
    if (NULL == jpg_buf) {
        ESP_LOGE(TAG, "malloc for jpg buffer failed");
        return 0;
    }
    memcpy(jpg_buf, jpg, length);

    uint8_t *rgb_buf = heap_caps_malloc(img_w * img_h * 3, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (NULL == rgb_buf) {
        free(jpg_buf);
        ESP_LOGE(TAG, "malloc for rgb buffer failed");
        return 0;
    }
    decode_func_t decode = g_decode_func[type][decoder_index];
    decode(jpg_buf, length, rgb_buf);
    if (DECODE_RGB565 == type) {
        ESP_LOGI(TAG, "jpeg decode to rgb565");
        print_rgb565_img(rgb_buf, img_w, img_h);
    } else {
        ESP_LOGI(TAG, "jpeg decode to rgb888");
        print_rgb888_img(rgb_buf, img_w, img_h);
    }

    uint64_t t_decode[times];
    for (size_t i = 0; i < times; i++) {
        uint64_t t1 = esp_timer_get_time();
        decode(jpg_buf, length, rgb_buf);
        t_decode[i] = esp_timer_get_time() - t1;
    }

    printf("resolution  ,  t \n");
    uint64_t t_total = 0;
    for (size_t i = 0; i < times; i++) {
        t_total += t_decode[i];
        float t = t_decode[i] / 1000.0f;
        printf("%4d x %4d ,  %5.2f ms \n", img_w, img_h, t);
    }

    float fps = times / (t_total / 1000000.0f);
    printf("Decode FPS Result\n");
    printf("resolution  , fps \n");
    printf("%4d x %4d , %5.2f  \n", img_w, img_h, fps);

    free(jpg_buf);
    heap_caps_free(rgb_buf);
    return fps;
}

static void img_jpeg_decode_test(uint16_t pic_index, uint16_t lib_index)
{
    extern const uint8_t img1_start[] asm("_binary_testimg_jpeg_start");
    extern const uint8_t img1_end[]   asm("_binary_testimg_jpeg_end");
    extern const uint8_t img2_start[] asm("_binary_test_inside_jpeg_start");
    extern const uint8_t img2_end[]   asm("_binary_test_inside_jpeg_end");
    extern const uint8_t img3_start[] asm("_binary_test_outside_jpeg_start");
    extern const uint8_t img3_end[]   asm("_binary_test_outside_jpeg_end");

    struct img_t {
        const uint8_t *buf;
        uint32_t length;
        uint16_t w, h;
    };
    struct img_t imgs[3] = {
        {
            .buf = img1_start,
            .length = img1_end - img1_start,
            .w = 227,
            .h = 149,
        },
        {
            .buf = img2_start,
            .length = img2_end - img2_start,
            .w = 320,
            .h = 240,
        },
        {
            .buf = img3_start,
            .length = img3_end - img3_start,
            .w = 480,
            .h = 320,
        },
    };

    ESP_LOGI(TAG, "pic_index:%d", pic_index);
    ESP_LOGI(TAG, "lib_index:%d", lib_index);
    jpg_decode_test(lib_index, DECODE_RGB565, imgs[pic_index].buf, imgs[pic_index].length, imgs[pic_index].w, imgs[pic_index].h, 16);
}

TEST_CASE("Conversions image 227x149 jpeg decode test", "[camera]")
{
    img_jpeg_decode_test(0, 0);
}

TEST_CASE("Conversions image 320x240 jpeg decode test", "[camera]")
{
    img_jpeg_decode_test(1, 0);
}

TEST_CASE("Conversions image 480x320 jpeg decode test", "[camera]")
{
    img_jpeg_decode_test(2, 0);
}
