/*
 * This file is part of the OpenMV project.
 * Copyright (c) 2013/2014 Ibrahim Abdelkader <i.abdalkader@gmail.com>
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 * OV3660 driver.
 *
 */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "sccb.h"
#include "xclk.h"
#include "ov5640.h"
#include "ov5640_regs.h"
#include "ov5640_settings.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#if defined(ARDUINO_ARCH_ESP32) && defined(CONFIG_ARDUHAL_ESP_LOG)
#include "esp32-hal-log.h"
#else
#include "esp_log.h"
static const char *TAG = "ov5640";
#endif

//#define REG_DEBUG_ON

static int read_reg(uint8_t slv_addr, const uint16_t reg){
    int ret = SCCB_Read16(slv_addr, reg);
#ifdef REG_DEBUG_ON
    if (ret < 0) {
        ESP_LOGE(TAG, "READ REG 0x%04x FAILED: %d", reg, ret);
    }
#endif
    return ret;
}

static int check_reg_mask(uint8_t slv_addr, uint16_t reg, uint8_t mask){
    return (read_reg(slv_addr, reg) & mask) == mask;
}

static int read_reg16(uint8_t slv_addr, const uint16_t reg){
    int ret = 0, ret2 = 0;
    ret = read_reg(slv_addr, reg);
    if (ret >= 0) {
        ret = (ret & 0xFF) << 8;
        ret2 = read_reg(slv_addr, reg+1);
        if (ret2 < 0) {
            ret = ret2;
        } else {
            ret |= ret2 & 0xFF;
        }
    }
    return ret;
}

//static void dump_reg(sensor_t *sensor, const uint16_t reg){
//    int v = SCCB_Read16(sensor->slv_addr, reg);
//    if(v < 0){
//        ets_printf("  0x%04x: FAIL[%d]\n", reg, v);
//    } else {
//        ets_printf("  0x%04x: 0x%02X\n", reg, v);
//    }
//}
//
//static void dump_range(sensor_t *sensor, const char * name, const uint16_t start_reg, const uint16_t end_reg){
//    ets_printf("%s: 0x%04x - 0x%04X\n", name, start_reg, end_reg);
//    for(uint16_t reg = start_reg; reg <= end_reg; reg++){
//        dump_reg(sensor, reg);
//    }
//}
//
//static void dump_regs(sensor_t *sensor){
////    dump_range(sensor, "All Regs", 0x3000, 0x6100);
////    dump_range(sensor, "system and IO pad control", 0x3000, 0x3052);
////    dump_range(sensor, "SCCB control", 0x3100, 0x3108);
////    dump_range(sensor, "SRB control", 0x3200, 0x3211);
////    dump_range(sensor, "AWB gain control", 0x3400, 0x3406);
////    dump_range(sensor, "AEC/AGC control", 0x3500, 0x350D);
////    dump_range(sensor, "VCM control", 0x3600, 0x3606);
////    dump_range(sensor, "timing control", 0x3800, 0x3821);
////    dump_range(sensor, "AEC/AGC power down domain control", 0x3A00, 0x3A25);
////    dump_range(sensor, "strobe control", 0x3B00, 0x3B0C);
////    dump_range(sensor, "50/60Hz detector control", 0x3C00, 0x3C1E);
////    dump_range(sensor, "OTP control", 0x3D00, 0x3D21);
////    dump_range(sensor, "MC control", 0x3F00, 0x3F0D);
////    dump_range(sensor, "BLC control", 0x4000, 0x4033);
////    dump_range(sensor, "frame control", 0x4201, 0x4202);
////    dump_range(sensor, "format control", 0x4300, 0x430D);
////    dump_range(sensor, "JPEG control", 0x4400, 0x4431);
////    dump_range(sensor, "VFIFO control", 0x4600, 0x460D);
////    dump_range(sensor, "DVP control", 0x4709, 0x4745);
////    dump_range(sensor, "MIPI control", 0x4800, 0x4837);
////    dump_range(sensor, "ISP frame control", 0x4901, 0x4902);
////    dump_range(sensor, "ISP top control", 0x5000, 0x5063);
////    dump_range(sensor, "AWB control", 0x5180, 0x51D0);
////    dump_range(sensor, "CIP control", 0x5300, 0x530F);
////    dump_range(sensor, "CMX control", 0x5380, 0x538B);
////    dump_range(sensor, "gamma control", 0x5480, 0x5490);
////    dump_range(sensor, "SDE control", 0x5580, 0x558C);
////    dump_range(sensor, "scale control", 0x5600, 0x5606);
////    dump_range(sensor, "AVG control", 0x5680, 0x56A2);
////    dump_range(sensor, "LENC control", 0x5800, 0x5849);
////    dump_range(sensor, "AFC control", 0x6000, 0x603F);
//}

static int write_reg(uint8_t slv_addr, const uint16_t reg, uint8_t value){
    int ret = 0;
#ifndef REG_DEBUG_ON
    ret = SCCB_Write16(slv_addr, reg, value);
#else
    int old_value = read_reg(slv_addr, reg);
    if (old_value < 0) {
        return old_value;
    }
    if ((uint8_t)old_value != value) {
        ESP_LOGI(TAG, "NEW REG 0x%04x: 0x%02x to 0x%02x", reg, (uint8_t)old_value, value);
        ret = SCCB_Write16(slv_addr, reg, value);
    } else {
        ESP_LOGD(TAG, "OLD REG 0x%04x: 0x%02x", reg, (uint8_t)old_value);
        ret = SCCB_Write16(slv_addr, reg, value);//maybe not?
    }
    if (ret < 0) {
        ESP_LOGE(TAG, "WRITE REG 0x%04x FAILED: %d", reg, ret);
    }
#endif
    return ret;
}

static int set_reg_bits(uint8_t slv_addr, uint16_t reg, uint8_t offset, uint8_t mask, uint8_t value)
{
    int ret = 0;
    uint8_t c_value, new_value;
    ret = read_reg(slv_addr, reg);
    if(ret < 0) {
        return ret;
    }
    c_value = ret;
    new_value = (c_value & ~(mask << offset)) | ((value & mask) << offset);
    ret = write_reg(slv_addr, reg, new_value);
    return ret;
}

static int write_regs(uint8_t slv_addr, const uint16_t (*regs)[2])
{
    int i = 0, ret = 0;
    while (!ret && regs[i][0] != REGLIST_TAIL) {
        if (regs[i][0] == REG_DLY) {
            vTaskDelay(regs[i][1] / portTICK_PERIOD_MS);
        } else {
            ret = write_reg(slv_addr, regs[i][0], regs[i][1]);
        }
        i++;
    }
    return ret;
}

static int write_reg16(uint8_t slv_addr, const uint16_t reg, uint16_t value)
{
    if (write_reg(slv_addr, reg, value >> 8) || write_reg(slv_addr, reg + 1, value)) {
        return -1;
    }
    return 0;
}

static int write_addr_reg(uint8_t slv_addr, const uint16_t reg, uint16_t x_value, uint16_t y_value)
{
    if (write_reg16(slv_addr, reg, x_value) || write_reg16(slv_addr, reg + 2, y_value)) {
        return -1;
    }
    return 0;
}

#define write_reg_bits(slv_addr, reg, mask, enable) set_reg_bits(slv_addr, reg, 0, mask, (enable)?(mask):0)

static int calc_sysclk(int xclk, bool pll_bypass, int pll_multiplier, int pll_sys_div, int pre_div, bool root_2x, int pclk_root_div, bool pclk_manual, int pclk_div)
{
    const float pll_pre_div2x_map[] = { 1, 1, 2, 3, 4, 1.5, 6, 2.5, 8};
    const int pll_pclk_root_div_map[] = { 1, 2, 4, 8 };

    if(!pll_sys_div) {
        pll_sys_div = 1;
    }

    float pll_pre_div = pll_pre_div2x_map[pre_div];
    unsigned int root_2x_div = root_2x?2:1;
    unsigned int pll_pclk_root_div = pll_pclk_root_div_map[pclk_root_div];

    unsigned int REFIN = xclk / pll_pre_div;

    unsigned int VCO = REFIN * pll_multiplier / root_2x_div;

    unsigned int PLL_CLK = pll_bypass?(xclk):(VCO / pll_sys_div * 2 / 5);//5 here is 10bit mode / 2, for 8bit it should be 4 (reg 0x3034)

    unsigned int PCLK = PLL_CLK / pll_pclk_root_div / ((pclk_manual && pclk_div)?pclk_div:2);

    unsigned int SYSCLK = PLL_CLK / 4;

    ESP_LOGI(TAG, "Calculated XVCLK: %d Hz, REFIN: %u Hz, VCO: %u Hz, PLL_CLK: %u Hz, SYSCLK: %u Hz, PCLK: %u Hz", xclk, REFIN, VCO, PLL_CLK, SYSCLK, PCLK);
    return SYSCLK;
}

static int set_pll(sensor_t *sensor, bool bypass, uint8_t multiplier, uint8_t sys_div, uint8_t pre_div, bool root_2x, uint8_t pclk_root_div, bool pclk_manual, uint8_t pclk_div){
    int ret = 0;
    if(multiplier > 252 || multiplier < 4 || sys_div > 15 || pre_div > 8 || pclk_div > 31 || pclk_root_div > 3){
        ESP_LOGE(TAG, "Invalid arguments");
        return -1;
    }
    if(multiplier > 127){
        multiplier &= 0xFE;//only even integers above 127
    }
    ESP_LOGI(TAG, "Set PLL: bypass: %u, multiplier: %u, sys_div: %u, pre_div: %u, root_2x: %u, pclk_root_div: %u, pclk_manual: %u, pclk_div: %u", bypass, multiplier, sys_div, pre_div, root_2x, pclk_root_div, pclk_manual, pclk_div);

    calc_sysclk(sensor->xclk_freq_hz, bypass, multiplier, sys_div, pre_div, root_2x, pclk_root_div, pclk_manual, pclk_div);

    ret = write_reg(sensor->slv_addr, 0x3039, bypass?0x80:0x00);
    if (ret == 0) {
        ret = write_reg(sensor->slv_addr, 0x3034, 0x1A);//10bit mode
    }
    if (ret == 0) {
        ret = write_reg(sensor->slv_addr, 0x3035, 0x01 | ((sys_div & 0x0f) << 4));
    }
    if (ret == 0) {
        ret = write_reg(sensor->slv_addr, 0x3036, multiplier & 0xff);
    }
    if (ret == 0) {
        ret = write_reg(sensor->slv_addr, 0x3037, (pre_div & 0xf) | (root_2x?0x10:0x00));
    }
    if (ret == 0) {
        ret = write_reg(sensor->slv_addr, 0x3108, (pclk_root_div & 0x3) << 4 | 0x06);
    }
    if (ret == 0) {
        ret = write_reg(sensor->slv_addr, 0x3824, pclk_div & 0x1f);
    }
    if (ret == 0) {
        ret = write_reg(sensor->slv_addr, 0x460C, pclk_manual?0x22:0x20);
    }
    if (ret == 0) {
        ret = write_reg(sensor->slv_addr, 0x3103, 0x13);// system clock from pll, bit[1]
    }
    if(ret){
        ESP_LOGE(TAG, "set_sensor_pll FAILED!");
    }
    return ret;
}

static int set_ae_level(sensor_t *sensor, int level);

static int reset(sensor_t *sensor)
{
    //dump_regs(sensor);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    int ret = 0;
    // Software Reset: clear all registers and reset them to their default values
    ret = write_reg(sensor->slv_addr, SYSTEM_CTROL0, 0x82);
    if(ret){
        ESP_LOGE(TAG, "Software Reset FAILED!");
        return ret;
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
    ret = write_regs(sensor->slv_addr, sensor_default_regs);
    if (ret == 0) {
        ESP_LOGD(TAG, "Camera defaults loaded");
        vTaskDelay(100 / portTICK_PERIOD_MS);
        //write_regs(sensor->slv_addr, sensor_regs_awb0);
        //write_regs(sensor->slv_addr, sensor_regs_gamma1);
    }
    return ret;
}

static int set_pixformat(sensor_t *sensor, pixformat_t pixformat)
{
    int ret = 0;
    const uint16_t (*regs)[2];

    switch (pixformat) {
    case PIXFORMAT_YUV422:
        regs = sensor_fmt_yuv422;
        break;

    case PIXFORMAT_GRAYSCALE:
        regs = sensor_fmt_grayscale;
        break;

    case PIXFORMAT_RGB565:
    case PIXFORMAT_RGB888:
        regs = sensor_fmt_rgb565;
        break;

    case PIXFORMAT_JPEG:
        regs = sensor_fmt_jpeg;
        break;

    case PIXFORMAT_RAW:
        regs = sensor_fmt_raw;
        break;

    default:
        ESP_LOGE(TAG, "Unsupported pixformat: %u", pixformat);
        return -1;
    }

    ret = write_regs(sensor->slv_addr, regs);
    if(ret == 0) {
        sensor->pixformat = pixformat;
        ESP_LOGD(TAG, "Set pixformat to: %u", pixformat);
    }
    return ret;
}

static int set_image_options(sensor_t *sensor)
{
    int ret = 0;
    uint8_t reg20 = 0;
    uint8_t reg21 = 0;
    uint8_t reg4514 = 0;
    uint8_t reg4514_test = 0;

    // compression
    if (sensor->pixformat == PIXFORMAT_JPEG) {
        reg21 |= 0x20;
    }

    // binning
    if (!sensor->status.binning) {
        reg20 |= 0x40;
    } else {
        reg20 |= 0x01;
        reg21 |= 0x01;
        reg4514_test |= 4;
    }

    // V-Flip
    if (sensor->status.vflip) {
        reg20 |= 0x06;
        reg4514_test |= 1;
    }

    // H-Mirror
    if (sensor->status.hmirror) {
        reg21 |= 0x06;
        reg4514_test |= 2;
    }

    switch (reg4514_test) {
        //no binning
        case 0: reg4514 = 0x88; break;//normal
        case 1: reg4514 = 0x00; break;//v-flip
        case 2: reg4514 = 0xbb; break;//h-mirror
        case 3: reg4514 = 0x00; break;//v-flip+h-mirror
        //binning
        case 4: reg4514 = 0xaa; break;//normal
        case 5: reg4514 = 0xbb; break;//v-flip
        case 6: reg4514 = 0xbb; break;//h-mirror
        case 7: reg4514 = 0xaa; break;//v-flip+h-mirror
    }

    if(write_reg(sensor->slv_addr, TIMING_TC_REG20, reg20)
        || write_reg(sensor->slv_addr, TIMING_TC_REG21, reg21)
        || write_reg(sensor->slv_addr, 0x4514, reg4514)){
        ESP_LOGE(TAG, "Setting Image Options Failed");
        return -1;
    }

    if (!sensor->status.binning) {
        ret  = write_reg(sensor->slv_addr, 0x4520, 0x10)
            || write_reg(sensor->slv_addr, X_INCREMENT, 0x11)//odd:1, even: 1
            || write_reg(sensor->slv_addr, Y_INCREMENT, 0x11);//odd:1, even: 1
    } else {
        ret  = write_reg(sensor->slv_addr, 0x4520, 0x0b)
            || write_reg(sensor->slv_addr, X_INCREMENT, 0x31)//odd:3, even: 1
            || write_reg(sensor->slv_addr, Y_INCREMENT, 0x31);//odd:3, even: 1
    }

    ESP_LOGD(TAG, "Set Image Options: Compression: %u, Binning: %u, V-Flip: %u, H-Mirror: %u, Reg-4514: 0x%02x",
        sensor->pixformat == PIXFORMAT_JPEG, sensor->status.binning, sensor->status.vflip, sensor->status.hmirror, reg4514);
    return ret;
}

static int set_framesize(sensor_t *sensor, framesize_t framesize)
{
    int ret = 0;
    framesize_t old_framesize = sensor->status.framesize;
    sensor->status.framesize = framesize;

    if(framesize > FRAMESIZE_QSXGA){
        ESP_LOGE(TAG, "Invalid framesize: %u", framesize);
        return -1;
    }
    uint16_t w = resolution[framesize].width;
    uint16_t h = resolution[framesize].height;
    aspect_ratio_t ratio = resolution[framesize].aspect_ratio;
    ratio_settings_t settings = ratio_table[ratio];

    sensor->status.binning = (w <= (settings.max_width / 2) && h <= (settings.max_height / 2));
    sensor->status.scale = !((w == settings.max_width && h == settings.max_height)
        || (w == (settings.max_width / 2) && h == (settings.max_height / 2)));

    ret  = write_addr_reg(sensor->slv_addr, X_ADDR_ST_H, settings.start_x, settings.start_y)
        || write_addr_reg(sensor->slv_addr, X_ADDR_END_H, settings.end_x, settings.end_y)
        || write_addr_reg(sensor->slv_addr, X_OUTPUT_SIZE_H, w, h);

    if (ret) {
        goto fail;
    }

    if (!sensor->status.binning) {
        ret  = write_addr_reg(sensor->slv_addr, X_TOTAL_SIZE_H, settings.total_x, settings.total_y)
            || write_addr_reg(sensor->slv_addr, X_OFFSET_H, settings.offset_x, settings.offset_y);
    } else {
        if (w > 920) {
            ret = write_addr_reg(sensor->slv_addr, X_TOTAL_SIZE_H, settings.total_x - 200, settings.total_y / 2);
        } else {
            ret = write_addr_reg(sensor->slv_addr, X_TOTAL_SIZE_H, 2060, settings.total_y / 2);
        }
        if (ret == 0) {
            ret = write_addr_reg(sensor->slv_addr, X_OFFSET_H, settings.offset_x / 2, settings.offset_y / 2);
        }
    }

    if (ret == 0) {
        ret = write_reg_bits(sensor->slv_addr, ISP_CONTROL_01, 0x20, sensor->status.scale);
    }

    if (ret == 0) {
        ret = set_image_options(sensor);
    }

    if (ret) {
        goto fail;
    }

    if (sensor->pixformat == PIXFORMAT_JPEG) {
        //10MHz PCLK
        uint8_t sys_mul = 200;
        if(framesize < FRAMESIZE_QVGA || sensor->xclk_freq_hz == 16000000){
            sys_mul = 160;
        } else if(framesize < FRAMESIZE_XGA){
            sys_mul = 180;
        }
        ret = set_pll(sensor, false, sys_mul, 4, 2, false, 2, true, 4);
        //Set PLL: bypass: 0, multiplier: sys_mul, sys_div: 4, pre_div: 2, root_2x: 0, pclk_root_div: 2, pclk_manual: 1, pclk_div: 4
    } else {
        //ret = set_pll(sensor, false, 8, 1, 1, false, 1, true, 4);
        if (framesize > FRAMESIZE_HVGA) {
            ret = set_pll(sensor, false, 10, 1, 2, false, 1, true, 2);
        } else if (framesize >= FRAMESIZE_QVGA) {
            ret = set_pll(sensor, false, 8, 1, 1, false, 1, true, 4);
        } else {
            ret = set_pll(sensor, false, 20, 1, 1, false, 1, true, 8);
        }
    }

    if (ret == 0) {
        ESP_LOGD(TAG, "Set framesize to: %ux%u", w, h);
    }
    return ret;

fail:
    sensor->status.framesize = old_framesize;
    ESP_LOGE(TAG, "Setting framesize to: %ux%u failed", w, h);
    return ret;
}

static int set_hmirror(sensor_t *sensor, int enable)
{
    int ret = 0;
    sensor->status.hmirror = enable;
    ret = set_image_options(sensor);
    if (ret == 0) {
        ESP_LOGD(TAG, "Set h-mirror to: %d", enable);
    }
    return ret;
}

static int set_vflip(sensor_t *sensor, int enable)
{
    int ret = 0;
    sensor->status.vflip = enable;
    ret = set_image_options(sensor);
    if (ret == 0) {
        ESP_LOGD(TAG, "Set v-flip to: %d", enable);
    }
    return ret;
}

static int set_quality(sensor_t *sensor, int qs)
{
    int ret = 0;
    ret = write_reg(sensor->slv_addr, COMPRESSION_CTRL07, qs & 0x3f);
    if (ret == 0) {
        sensor->status.quality = qs;
        ESP_LOGD(TAG, "Set quality to: %d", qs);
    }
    return ret;
}

static int set_colorbar(sensor_t *sensor, int enable)
{
    int ret = 0;
    ret = write_reg_bits(sensor->slv_addr, PRE_ISP_TEST_SETTING_1, TEST_COLOR_BAR, enable);
    if (ret == 0) {
        sensor->status.colorbar = enable;
        ESP_LOGD(TAG, "Set colorbar to: %d", enable);
    }
    return ret;
}

static int set_gain_ctrl(sensor_t *sensor, int enable)
{
    int ret = 0;
    ret = write_reg_bits(sensor->slv_addr, AEC_PK_MANUAL, AEC_PK_MANUAL_AGC_MANUALEN, !enable);
    if (ret == 0) {
        ESP_LOGD(TAG, "Set gain_ctrl to: %d", enable);
        sensor->status.agc = enable;
    }
    return ret;
}

static int set_exposure_ctrl(sensor_t *sensor, int enable)
{
    int ret = 0;
    ret = write_reg_bits(sensor->slv_addr, AEC_PK_MANUAL, AEC_PK_MANUAL_AEC_MANUALEN, !enable);
    if (ret == 0) {
        ESP_LOGD(TAG, "Set exposure_ctrl to: %d", enable);
        sensor->status.aec = enable;
    }
    return ret;
}

static int set_whitebal(sensor_t *sensor, int enable)
{
    int ret = 0;
    ret = write_reg_bits(sensor->slv_addr, ISP_CONTROL_01, 0x01, enable);
    if (ret == 0) {
        ESP_LOGD(TAG, "Set awb to: %d", enable);
        sensor->status.awb = enable;
    }
    return ret;
}

//Advanced AWB
static int set_dcw_dsp(sensor_t *sensor, int enable)
{
    int ret = 0;
    ret = write_reg_bits(sensor->slv_addr, 0x5183, 0x80, !enable);
    if (ret == 0) {
        ESP_LOGD(TAG, "Set dcw to: %d", enable);
        sensor->status.dcw = enable;
    }
    return ret;
}

//night mode enable
static int set_aec2(sensor_t *sensor, int enable)
{
    int ret = 0;
    ret = write_reg_bits(sensor->slv_addr, 0x3a00, 0x04, enable);
    if (ret == 0) {
        ESP_LOGD(TAG, "Set aec2 to: %d", enable);
        sensor->status.aec2 = enable;
    }
    return ret;
}

static int set_bpc_dsp(sensor_t *sensor, int enable)
{
    int ret = 0;
    ret = write_reg_bits(sensor->slv_addr, 0x5000, 0x04, enable);
    if (ret == 0) {
        ESP_LOGD(TAG, "Set bpc to: %d", enable);
        sensor->status.bpc = enable;
    }
    return ret;
}

static int set_wpc_dsp(sensor_t *sensor, int enable)
{
    int ret = 0;
    ret = write_reg_bits(sensor->slv_addr, 0x5000, 0x02, enable);
    if (ret == 0) {
        ESP_LOGD(TAG, "Set wpc to: %d", enable);
        sensor->status.wpc = enable;
    }
    return ret;
}

//Gamma enable
static int set_raw_gma_dsp(sensor_t *sensor, int enable)
{
    int ret = 0;
    ret = write_reg_bits(sensor->slv_addr, 0x5000, 0x20, enable);
    if (ret == 0) {
        ESP_LOGD(TAG, "Set raw_gma to: %d", enable);
        sensor->status.raw_gma = enable;
    }
    return ret;
}

static int set_lenc_dsp(sensor_t *sensor, int enable)
{
    int ret = 0;
    ret = write_reg_bits(sensor->slv_addr, 0x5000, 0x80, enable);
    if (ret == 0) {
        ESP_LOGD(TAG, "Set lenc to: %d", enable);
        sensor->status.lenc = enable;
    }
    return ret;
}

static int get_agc_gain(sensor_t *sensor)
{
    int ra = read_reg(sensor->slv_addr, 0x350a);
    if (ra < 0) {
        return 0;
    }
    int rb = read_reg(sensor->slv_addr, 0x350b);
    if (rb < 0) {
        return 0;
    }
    int res = (rb & 0xF0) >> 4 | (ra & 0x03) << 4;
    if (rb & 0x0F) {
        res += 1;
    }
    return res;
}

//real gain
static int set_agc_gain(sensor_t *sensor, int gain)
{
    int ret = 0;
    if(gain < 0) {
        gain = 0;
    } else if(gain > 64) {
        gain = 64;
    }

    //gain value is 6.4 bits float
    //in order to use the max range, we deduct 1/16
    int gainv = gain << 4;
    if(gainv){
        gainv -= 1;
    }

    ret = write_reg(sensor->slv_addr, 0x350a, gainv >> 8) || write_reg(sensor->slv_addr, 0x350b, gainv & 0xff);
    if (ret == 0) {
        ESP_LOGD(TAG, "Set agc_gain to: %d", gain);
        sensor->status.agc_gain = gain;
    }
    return ret;
}

static int get_aec_value(sensor_t *sensor)
{
    int ra = read_reg(sensor->slv_addr, 0x3500);
    if (ra < 0) {
        return 0;
    }
    int rb = read_reg(sensor->slv_addr, 0x3501);
    if (rb < 0) {
        return 0;
    }
    int rc = read_reg(sensor->slv_addr, 0x3502);
    if (rc < 0) {
        return 0;
    }
    int res = (ra & 0x0F) << 12 | (rb & 0xFF) << 4 | (rc & 0xF0) >> 4;
    return res;
}

static int set_aec_value(sensor_t *sensor, int value)
{
    int ret = 0, max_val = 0;
    max_val = read_reg16(sensor->slv_addr, 0x380e);
    if (max_val < 0) {
        ESP_LOGE(TAG, "Could not read max aec_value");
        return -1;
    }
    if (value > max_val) {
        value =max_val;
    }

    ret =  write_reg(sensor->slv_addr, 0x3500, (value >> 12) & 0x0F)
        || write_reg(sensor->slv_addr, 0x3501, (value >> 4) & 0xFF)
        || write_reg(sensor->slv_addr, 0x3502, (value << 4) & 0xF0);

    if (ret == 0) {
        ESP_LOGD(TAG, "Set aec_value to: %d / %d", value, max_val);
        sensor->status.aec_value = value;
    }
    return ret;
}

static int set_ae_level(sensor_t *sensor, int level)
{
    int ret = 0;
    if (level < -5 || level > 5) {
        return -1;
    }
    //good targets are between 5 and 115
    int target_level = ((level + 5) * 10) + 5;

    int level_high, level_low;
    int fast_high, fast_low;

    level_low = target_level * 23 / 25; //0.92 (0.46)
    level_high = target_level * 27 / 25; //1.08 (2.08)

    fast_low = level_low >> 1;
    fast_high = level_high << 1;

    if(fast_high>255) {
        fast_high = 255;
    }

    ret =  write_reg(sensor->slv_addr, 0x3a0f, level_high)
        || write_reg(sensor->slv_addr, 0x3a10, level_low)
        || write_reg(sensor->slv_addr, 0x3a1b, level_high)
        || write_reg(sensor->slv_addr, 0x3a1e, level_low)
        || write_reg(sensor->slv_addr, 0x3a11, fast_high)
        || write_reg(sensor->slv_addr, 0x3a1f, fast_low);

    if (ret == 0) {
        ESP_LOGD(TAG, "Set ae_level to: %d", level);
        sensor->status.ae_level = level;
    }
    return ret;
}

static int set_wb_mode(sensor_t *sensor, int mode)
{
    int ret = 0;
    if (mode < 0 || mode > 4) {
        return -1;
    }

    ret = write_reg(sensor->slv_addr, 0x3406, (mode != 0));
    if (ret) {
        return ret;
    }
    switch (mode) {
        case 1://Sunny
            ret  = write_reg16(sensor->slv_addr, 0x3400, 0x5e0) //AWB R GAIN
                || write_reg16(sensor->slv_addr, 0x3402, 0x410) //AWB G GAIN
                || write_reg16(sensor->slv_addr, 0x3404, 0x540);//AWB B GAIN
            break;
        case 2://Cloudy
            ret  = write_reg16(sensor->slv_addr, 0x3400, 0x650) //AWB R GAIN
                || write_reg16(sensor->slv_addr, 0x3402, 0x410) //AWB G GAIN
                || write_reg16(sensor->slv_addr, 0x3404, 0x4f0);//AWB B GAIN
            break;
        case 3://Office
            ret  = write_reg16(sensor->slv_addr, 0x3400, 0x520) //AWB R GAIN
                || write_reg16(sensor->slv_addr, 0x3402, 0x410) //AWB G GAIN
                || write_reg16(sensor->slv_addr, 0x3404, 0x660);//AWB B GAIN
            break;
        case 4://HOME
            ret  = write_reg16(sensor->slv_addr, 0x3400, 0x420) //AWB R GAIN
                || write_reg16(sensor->slv_addr, 0x3402, 0x3f0) //AWB G GAIN
                || write_reg16(sensor->slv_addr, 0x3404, 0x710);//AWB B GAIN
            break;
        default://AUTO
            break;
    }

    if (ret == 0) {
        ESP_LOGD(TAG, "Set wb_mode to: %d", mode);
        sensor->status.wb_mode = mode;
    }
    return ret;
}

static int set_awb_gain_dsp(sensor_t *sensor, int enable)
{
    int ret = 0;
    int old_mode = sensor->status.wb_mode;
    int mode = enable?old_mode:0;

    ret = set_wb_mode(sensor, mode);

    if (ret == 0) {
        sensor->status.wb_mode = old_mode;
        ESP_LOGD(TAG, "Set awb_gain to: %d", enable);
        sensor->status.awb_gain = enable;
    }
    return ret;
}

static int set_special_effect(sensor_t *sensor, int effect)
{
    int ret=0;
    if (effect < 0 || effect > 6) {
        return -1;
    }

    uint8_t * regs = (uint8_t *)sensor_special_effects[effect];
    ret =  write_reg(sensor->slv_addr, 0x5580, regs[0])
        || write_reg(sensor->slv_addr, 0x5583, regs[1])
        || write_reg(sensor->slv_addr, 0x5584, regs[2])
        || write_reg(sensor->slv_addr, 0x5003, regs[3]);

    if (ret == 0) {
        ESP_LOGD(TAG, "Set special_effect to: %d", effect);
        sensor->status.special_effect = effect;
    }
    return ret;
}

static int set_brightness(sensor_t *sensor, int level)
{
    int ret = 0;
    uint8_t value = 0;
    bool negative = false;

    switch (level) {
        case 3:
            value = 0x30;
            break;
        case 2:
            value = 0x20;
            break;
        case 1:
            value = 0x10;
            break;
        case -1:
            value = 0x10;
            negative = true;
            break;
        case -2:
            value = 0x20;
            negative = true;
            break;
        case -3:
            value = 0x30;
            negative = true;
            break;
        default: // 0
            break;
    }

    ret = write_reg(sensor->slv_addr, 0x5587, value);
    if (ret == 0) {
        ret = write_reg_bits(sensor->slv_addr, 0x5588, 0x08, negative);
    }

    if (ret == 0) {
        ESP_LOGD(TAG, "Set brightness to: %d", level);
        sensor->status.brightness = level;
    }
    return ret;
}

static int set_contrast(sensor_t *sensor, int level)
{
    int ret = 0;
    if(level > 3 || level < -3) {
        return -1;
    }
    ret = write_reg(sensor->slv_addr, 0x5586, (level + 4) << 3);

    if (ret == 0) {
        ESP_LOGD(TAG, "Set contrast to: %d", level);
        sensor->status.contrast = level;
    }
    return ret;
}

static int set_saturation(sensor_t *sensor, int level)
{
    int ret = 0;
    if(level > 4 || level < -4) {
        return -1;
    }

    uint8_t * regs = (uint8_t *)sensor_saturation_levels[level+4];
    for(int i=0; i<11; i++) {
        ret = write_reg(sensor->slv_addr, 0x5381 + i, regs[i]);
        if (ret) {
            break;
        }
    }

    if (ret == 0) {
        ESP_LOGD(TAG, "Set saturation to: %d", level);
        sensor->status.saturation = level;
    }
    return ret;
}

static int set_sharpness(sensor_t *sensor, int level)
{
    int ret = 0;
    if(level > 3 || level < -3) {
        return -1;
    }

    uint8_t mt_offset_2 = (level + 3) * 8;
    uint8_t mt_offset_1 = mt_offset_2 + 1;

    ret = write_reg_bits(sensor->slv_addr, 0x5308, 0x40, false)//0x40 means auto
        || write_reg(sensor->slv_addr, 0x5300, 0x10)
        || write_reg(sensor->slv_addr, 0x5301, 0x10)
        || write_reg(sensor->slv_addr, 0x5302, mt_offset_1)
        || write_reg(sensor->slv_addr, 0x5303, mt_offset_2)
        || write_reg(sensor->slv_addr, 0x5309, 0x10)
        || write_reg(sensor->slv_addr, 0x530a, 0x10)
        || write_reg(sensor->slv_addr, 0x530b, 0x04)
        || write_reg(sensor->slv_addr, 0x530c, 0x06);

    if (ret == 0) {
        ESP_LOGD(TAG, "Set sharpness to: %d", level);
        sensor->status.sharpness = level;
    }
    return ret;
}

static int set_gainceiling(sensor_t *sensor, gainceiling_t level)
{
    int ret = 0, l = (int)level;

    ret = write_reg(sensor->slv_addr, 0x3A18, (l >> 8) & 3)
       || write_reg(sensor->slv_addr, 0x3A19, l & 0xFF);

    if (ret == 0) {
        ESP_LOGD(TAG, "Set gainceiling to: %d", l);
        sensor->status.gainceiling = l;
    }
    return ret;
}

static int get_denoise(sensor_t *sensor)
{
    if (!check_reg_mask(sensor->slv_addr, 0x5308, 0x10)) {
        return 0;
    }
    return (read_reg(sensor->slv_addr, 0x5306) / 4) + 1;
}

static int set_denoise(sensor_t *sensor, int level)
{
    int ret = 0;
    if (level < 0 || level > 8) {
        return -1;
    }

    ret = write_reg_bits(sensor->slv_addr, 0x5308, 0x10, level > 0);
    if (ret == 0 && level > 0) {
        ret = write_reg(sensor->slv_addr, 0x5306, (level - 1) * 4);
    }

    if (ret == 0) {
        ESP_LOGD(TAG, "Set denoise to: %d", level);
        sensor->status.denoise = level;
    }
    return ret;
}

static int get_reg(sensor_t *sensor, int reg, int mask)
{
    int ret = 0, ret2 = 0;
    if(mask > 0xFF){
        ret = read_reg16(sensor->slv_addr, reg);
        if(ret >= 0 && mask > 0xFFFF){
            ret2 = read_reg(sensor->slv_addr, reg+2);
            if(ret2 >= 0){
                ret = (ret << 8) | ret2 ;
            } else {
                ret = ret2;
            }
        }
    } else {
        ret = read_reg(sensor->slv_addr, reg);
    }
    if(ret > 0){
        ret &= mask;
    }
    return ret;
}

static int set_reg(sensor_t *sensor, int reg, int mask, int value)
{
    int ret = 0, ret2 = 0;
    if(mask > 0xFF){
        ret = read_reg16(sensor->slv_addr, reg);
        if(ret >= 0 && mask > 0xFFFF){
            ret2 = read_reg(sensor->slv_addr, reg+2);
            if(ret2 >= 0){
                ret = (ret << 8) | ret2 ;
            } else {
                ret = ret2;
            }
        }
    } else {
        ret = read_reg(sensor->slv_addr, reg);
    }
    if(ret < 0){
        return ret;
    }
    value = (ret & ~mask) | (value & mask);
    if(mask > 0xFFFF){
        ret = write_reg16(sensor->slv_addr, reg, value >> 8);
        if(ret >= 0){
            ret = write_reg(sensor->slv_addr, reg+2, value & 0xFF);
        }
    } else if(mask > 0xFF){
        ret = write_reg16(sensor->slv_addr, reg, value);
    } else {
        ret = write_reg(sensor->slv_addr, reg, value);
    }
    return ret;
}

static int set_res_raw(sensor_t *sensor, int startX, int startY, int endX, int endY, int offsetX, int offsetY, int totalX, int totalY, int outputX, int outputY, bool scale, bool binning)
{
    int ret = 0;
    ret  = write_addr_reg(sensor->slv_addr, X_ADDR_ST_H, startX, startY)
        || write_addr_reg(sensor->slv_addr, X_ADDR_END_H, endX, endY)
        || write_addr_reg(sensor->slv_addr, X_OFFSET_H, offsetX, offsetY)
        || write_addr_reg(sensor->slv_addr, X_TOTAL_SIZE_H, totalX, totalY)
        || write_addr_reg(sensor->slv_addr, X_OUTPUT_SIZE_H, outputX, outputY)
        || write_reg_bits(sensor->slv_addr, ISP_CONTROL_01, 0x20, scale);
    if(!ret){
        sensor->status.scale = scale;
        sensor->status.binning = binning;
        ret = set_image_options(sensor);
    }
    return ret;
}

static int _set_pll(sensor_t *sensor, int bypass, int multiplier, int sys_div, int root_2x, int pre_div, int seld5, int pclk_manual, int pclk_div)
{
    int ret = 0;
    ret = set_pll(sensor, bypass > 0, multiplier, sys_div, pre_div, root_2x > 0, seld5, pclk_manual > 0, pclk_div);
    return ret;
}

static int set_xclk(sensor_t *sensor, int timer, int xclk)
{
    int ret = 0;
    sensor->xclk_freq_hz = xclk * 1000000U;
    ret = xclk_timer_conf(timer, sensor->xclk_freq_hz);
    return ret;
}

static int init_status(sensor_t *sensor)
{
    sensor->status.brightness = 0;
    sensor->status.contrast = 0;
    sensor->status.saturation = 0;
    sensor->status.sharpness = (read_reg(sensor->slv_addr, 0x5303) / 8) - 3;
    sensor->status.denoise = get_denoise(sensor);
    sensor->status.ae_level = 0;
    sensor->status.gainceiling = read_reg16(sensor->slv_addr, 0x3A18) & 0x3FF;
    sensor->status.awb = check_reg_mask(sensor->slv_addr, ISP_CONTROL_01, 0x01);
    sensor->status.dcw = !check_reg_mask(sensor->slv_addr, 0x5183, 0x80);
    sensor->status.agc = !check_reg_mask(sensor->slv_addr, AEC_PK_MANUAL, AEC_PK_MANUAL_AGC_MANUALEN);
    sensor->status.aec = !check_reg_mask(sensor->slv_addr, AEC_PK_MANUAL, AEC_PK_MANUAL_AEC_MANUALEN);
    sensor->status.hmirror = check_reg_mask(sensor->slv_addr, TIMING_TC_REG21, TIMING_TC_REG21_HMIRROR);
    sensor->status.vflip = check_reg_mask(sensor->slv_addr, TIMING_TC_REG20, TIMING_TC_REG20_VFLIP);
    sensor->status.colorbar = check_reg_mask(sensor->slv_addr, PRE_ISP_TEST_SETTING_1, TEST_COLOR_BAR);
    sensor->status.bpc = check_reg_mask(sensor->slv_addr, 0x5000, 0x04);
    sensor->status.wpc = check_reg_mask(sensor->slv_addr, 0x5000, 0x02);
    sensor->status.raw_gma = check_reg_mask(sensor->slv_addr, 0x5000, 0x20);
    sensor->status.lenc = check_reg_mask(sensor->slv_addr, 0x5000, 0x80);
    sensor->status.quality = read_reg(sensor->slv_addr, COMPRESSION_CTRL07) & 0x3f;
    sensor->status.special_effect = 0;
    sensor->status.wb_mode = 0;
    sensor->status.awb_gain = check_reg_mask(sensor->slv_addr, 0x3406, 0x01);
    sensor->status.agc_gain = get_agc_gain(sensor);
    sensor->status.aec_value = get_aec_value(sensor);
    sensor->status.aec2 = check_reg_mask(sensor->slv_addr, 0x3a00, 0x04);
    return 0;
}

int ov5640_detect(int slv_addr, sensor_id_t *id)
{
    if (OV5640_SCCB_ADDR == slv_addr) {
        uint8_t h = SCCB_Read16(slv_addr, 0x300A);
        uint8_t l = SCCB_Read16(slv_addr, 0x300B);
        uint16_t PID = (h<<8) | l;
        if (OV5640_PID == PID) {
            id->PID = PID;
            return PID;
        } else {
            ESP_LOGI(TAG, "Mismatch PID=0x%x", PID);
        }
    }
    return 0;
}

int ov5640_init(sensor_t *sensor)
{
    sensor->reset = reset;
    sensor->set_pixformat = set_pixformat;
    sensor->set_framesize = set_framesize;
    sensor->set_contrast = set_contrast;
    sensor->set_brightness = set_brightness;
    sensor->set_saturation = set_saturation;
    sensor->set_sharpness = set_sharpness;
    sensor->set_gainceiling = set_gainceiling;
    sensor->set_quality = set_quality;
    sensor->set_colorbar = set_colorbar;
    sensor->set_gain_ctrl = set_gain_ctrl;
    sensor->set_exposure_ctrl = set_exposure_ctrl;
    sensor->set_whitebal = set_whitebal;
    sensor->set_hmirror = set_hmirror;
    sensor->set_vflip = set_vflip;
    sensor->init_status = init_status;
    sensor->set_aec2 = set_aec2;
    sensor->set_aec_value = set_aec_value;
    sensor->set_special_effect = set_special_effect;
    sensor->set_wb_mode = set_wb_mode;
    sensor->set_ae_level = set_ae_level;
    sensor->set_dcw = set_dcw_dsp;
    sensor->set_bpc = set_bpc_dsp;
    sensor->set_wpc = set_wpc_dsp;
    sensor->set_awb_gain = set_awb_gain_dsp;
    sensor->set_agc_gain = set_agc_gain;
    sensor->set_raw_gma = set_raw_gma_dsp;
    sensor->set_lenc = set_lenc_dsp;
    sensor->set_denoise = set_denoise;

    sensor->get_reg = get_reg;
    sensor->set_reg = set_reg;
    sensor->set_res_raw = set_res_raw;
    sensor->set_pll = _set_pll;
    sensor->set_xclk = set_xclk;
    return 0;
}
