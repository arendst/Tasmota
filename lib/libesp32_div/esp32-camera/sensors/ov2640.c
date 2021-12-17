/*
 * This file is part of the OpenMV project.
 * Copyright (c) 2013/2014 Ibrahim Abdelkader <i.abdalkader@gmail.com>
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 * OV2640 driver.
 *
 */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "sccb.h"
#include "xclk.h"
#include "ov2640.h"
#include "ov2640_regs.h"
#include "ov2640_settings.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#if defined(ARDUINO_ARCH_ESP32) && defined(CONFIG_ARDUHAL_ESP_LOG)
#include "esp32-hal-log.h"
#else
#include "esp_log.h"
static const char* TAG = "ov2640";
#endif

static volatile ov2640_bank_t reg_bank = BANK_MAX;
static int set_bank(sensor_t *sensor, ov2640_bank_t bank)
{
    int res = 0;
    if (bank != reg_bank) {
        reg_bank = bank;
        res = SCCB_Write(sensor->slv_addr, BANK_SEL, bank);
    }
    return res;
}

static int write_regs(sensor_t *sensor, const uint8_t (*regs)[2])
{
    int i=0, res = 0;
    while (regs[i][0]) {
        if (regs[i][0] == BANK_SEL) {
            res = set_bank(sensor, regs[i][1]);
        } else {
            res = SCCB_Write(sensor->slv_addr, regs[i][0], regs[i][1]);
        }
        if (res) {
            return res;
        }
        i++;
    }
    return res;
}

static int write_reg(sensor_t *sensor, ov2640_bank_t bank, uint8_t reg, uint8_t value)
{
    int ret = set_bank(sensor, bank);
    if(!ret) {
        ret = SCCB_Write(sensor->slv_addr, reg, value);
    }
    return ret;
}

static int set_reg_bits(sensor_t *sensor, uint8_t bank, uint8_t reg, uint8_t offset, uint8_t mask, uint8_t value)
{
    int ret = 0;
    uint8_t c_value, new_value;

    ret = set_bank(sensor, bank);
    if(ret) {
        return ret;
    }
    c_value = SCCB_Read(sensor->slv_addr, reg);
    new_value = (c_value & ~(mask << offset)) | ((value & mask) << offset);
    ret = SCCB_Write(sensor->slv_addr, reg, new_value);
    return ret;
}

static int read_reg(sensor_t *sensor, ov2640_bank_t bank, uint8_t reg)
{
    if(set_bank(sensor, bank)){
        return 0;
    }
    return SCCB_Read(sensor->slv_addr, reg);
}

static uint8_t get_reg_bits(sensor_t *sensor, uint8_t bank, uint8_t reg, uint8_t offset, uint8_t mask)
{
    return (read_reg(sensor, bank, reg) >> offset) & mask;
}

static int write_reg_bits(sensor_t *sensor, uint8_t bank, uint8_t reg, uint8_t mask, int enable)
{
    return set_reg_bits(sensor, bank, reg, 0, mask, enable?mask:0);
}

#define WRITE_REGS_OR_RETURN(regs) ret = write_regs(sensor, regs); if(ret){return ret;}
#define WRITE_REG_OR_RETURN(bank, reg, val) ret = write_reg(sensor, bank, reg, val); if(ret){return ret;}
#define SET_REG_BITS_OR_RETURN(bank, reg, offset, mask, val) ret = set_reg_bits(sensor, bank, reg, offset, mask, val); if(ret){return ret;}

static int reset(sensor_t *sensor)
{
    int ret = 0;
    WRITE_REG_OR_RETURN(BANK_SENSOR, COM7, COM7_SRST);
    vTaskDelay(10 / portTICK_PERIOD_MS);
    WRITE_REGS_OR_RETURN(ov2640_settings_cif);
    return ret;
}

static int set_pixformat(sensor_t *sensor, pixformat_t pixformat)
{
    int ret = 0;
    sensor->pixformat = pixformat;
    switch (pixformat) {
    case PIXFORMAT_RGB565:
    case PIXFORMAT_RGB888:
        WRITE_REGS_OR_RETURN(ov2640_settings_rgb565);
        break;
    case PIXFORMAT_YUV422:
    case PIXFORMAT_GRAYSCALE:
        WRITE_REGS_OR_RETURN(ov2640_settings_yuv422);
        break;
    case PIXFORMAT_JPEG:
        WRITE_REGS_OR_RETURN(ov2640_settings_jpeg3);
        break;
    default:
        ret = -1;
        break;
    }
    if(!ret) {
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    return ret;
}

static int set_window(sensor_t *sensor, ov2640_sensor_mode_t mode, int offset_x, int offset_y, int max_x, int max_y, int w, int h){
    int ret = 0;
    const uint8_t (*regs)[2];
    ov2640_clk_t c;
    c.reserved = 0;

    max_x /= 4;
    max_y /= 4;
    w /= 4;
    h /= 4;
    uint8_t win_regs[][2] = {
        {BANK_SEL, BANK_DSP},
        {HSIZE, max_x & 0xFF},
        {VSIZE, max_y & 0xFF},
        {XOFFL, offset_x & 0xFF},
        {YOFFL, offset_y & 0xFF},
        {VHYX, ((max_y >> 1) & 0X80) | ((offset_y >> 4) & 0X70) | ((max_x >> 5) & 0X08) | ((offset_x >> 8) & 0X07)},
        {TEST, (max_x >> 2) & 0X80},
        {ZMOW, (w)&0xFF},
        {ZMOH, (h)&0xFF},
        {ZMHH, ((h>>6)&0x04)|((w>>8)&0x03)},
        {0, 0}
    };

    if (sensor->pixformat == PIXFORMAT_JPEG) {
        c.clk_2x = 0;
        c.clk_div = 0;
        c.pclk_auto = 0;
        c.pclk_div = 8;
        if(mode == OV2640_MODE_UXGA) {
            c.pclk_div = 12;
        }
        // if (sensor->xclk_freq_hz == 16000000) {
        //     c.pclk_div = c.pclk_div / 2;
        // }
    } else {
#if CONFIG_IDF_TARGET_ESP32
        c.clk_2x = 0;
#else
        c.clk_2x = 1;
#endif
        c.clk_div = 7;
        c.pclk_auto = 1;
        c.pclk_div = 8;
        if (mode == OV2640_MODE_CIF) {
            c.clk_div = 3;
        } else if(mode == OV2640_MODE_UXGA) {
            c.pclk_div = 12;
        }
    }
    ESP_LOGI(TAG, "Set PLL: clk_2x: %u, clk_div: %u, pclk_auto: %u, pclk_div: %u", c.clk_2x, c.clk_div, c.pclk_auto, c.pclk_div);

    if (mode == OV2640_MODE_CIF) {
        regs = ov2640_settings_to_cif;
    } else if (mode == OV2640_MODE_SVGA) {
        regs = ov2640_settings_to_svga;
    } else {
        regs = ov2640_settings_to_uxga;
    }

    WRITE_REG_OR_RETURN(BANK_DSP, R_BYPASS, R_BYPASS_DSP_BYPAS);
    WRITE_REGS_OR_RETURN(regs);
    WRITE_REGS_OR_RETURN(win_regs);
    WRITE_REG_OR_RETURN(BANK_SENSOR, CLKRC, c.clk);
    WRITE_REG_OR_RETURN(BANK_DSP, R_DVP_SP, c.pclk);
    WRITE_REG_OR_RETURN(BANK_DSP, R_BYPASS, R_BYPASS_DSP_EN);

    vTaskDelay(10 / portTICK_PERIOD_MS);
    //required when changing resolution
    set_pixformat(sensor, sensor->pixformat);

    return ret;
}

static int set_framesize(sensor_t *sensor, framesize_t framesize)
{
    int ret = 0;
    uint16_t w = resolution[framesize].width;
    uint16_t h = resolution[framesize].height;
    aspect_ratio_t ratio = resolution[framesize].aspect_ratio;
    uint16_t max_x = ratio_table[ratio].max_x;
    uint16_t max_y = ratio_table[ratio].max_y;
    uint16_t offset_x = ratio_table[ratio].offset_x;
    uint16_t offset_y = ratio_table[ratio].offset_y;
    ov2640_sensor_mode_t mode = OV2640_MODE_UXGA;

    sensor->status.framesize = framesize;



    if (framesize <= FRAMESIZE_CIF) {
        mode = OV2640_MODE_CIF;
        max_x /= 4;
        max_y /= 4;
        offset_x /= 4;
        offset_y /= 4;
        if(max_y > 296){
            max_y = 296;
        }
    } else if (framesize <= FRAMESIZE_SVGA) {
        mode = OV2640_MODE_SVGA;
        max_x /= 2;
        max_y /= 2;
        offset_x /= 2;
        offset_y /= 2;
    }

    ret = set_window(sensor, mode, offset_x, offset_y, max_x, max_y, w, h);
    return ret;
}

static int set_contrast(sensor_t *sensor, int level)
{
    int ret=0;
    level += 3;
    if (level <= 0 || level > NUM_CONTRAST_LEVELS) {
        return -1;
    }
    sensor->status.contrast = level-3;
    for (int i=0; i<7; i++) {
        WRITE_REG_OR_RETURN(BANK_DSP, contrast_regs[0][i], contrast_regs[level][i]);
    }
    return ret;
}

static int set_brightness(sensor_t *sensor, int level)
{
    int ret=0;
    level += 3;
    if (level <= 0 || level > NUM_BRIGHTNESS_LEVELS) {
        return -1;
    }
    sensor->status.brightness = level-3;
    for (int i=0; i<5; i++) {
        WRITE_REG_OR_RETURN(BANK_DSP, brightness_regs[0][i], brightness_regs[level][i]);
    }
    return ret;
}

static int set_saturation(sensor_t *sensor, int level)
{
    int ret=0;
    level += 3;
    if (level <= 0 || level > NUM_SATURATION_LEVELS) {
        return -1;
    }
    sensor->status.saturation = level-3;
    for (int i=0; i<5; i++) {
        WRITE_REG_OR_RETURN(BANK_DSP, saturation_regs[0][i], saturation_regs[level][i]);
    }
    return ret;
}

static int set_special_effect(sensor_t *sensor, int effect)
{
    int ret=0;
    effect++;
    if (effect <= 0 || effect > NUM_SPECIAL_EFFECTS) {
        return -1;
    }
    sensor->status.special_effect = effect-1;
    for (int i=0; i<5; i++) {
        WRITE_REG_OR_RETURN(BANK_DSP, special_effects_regs[0][i], special_effects_regs[effect][i]);
    }
    return ret;
}

static int set_wb_mode(sensor_t *sensor, int mode)
{
    int ret=0;
    if (mode < 0 || mode > NUM_WB_MODES) {
        return -1;
    }
    sensor->status.wb_mode = mode;
    SET_REG_BITS_OR_RETURN(BANK_DSP, 0XC7, 6, 1, mode?1:0);
    if(mode) {
        for (int i=0; i<3; i++) {
            WRITE_REG_OR_RETURN(BANK_DSP, wb_modes_regs[0][i], wb_modes_regs[mode][i]);
        }
    }
    return ret;
}

static int set_ae_level(sensor_t *sensor, int level)
{
    int ret=0;
    level += 3;
    if (level <= 0 || level > NUM_AE_LEVELS) {
        return -1;
    }
    sensor->status.ae_level = level-3;
    for (int i=0; i<3; i++) {
        WRITE_REG_OR_RETURN(BANK_SENSOR, ae_levels_regs[0][i], ae_levels_regs[level][i]);
    }
    return ret;
}

static int set_quality(sensor_t *sensor, int quality)
{
    if(quality < 0) {
        quality = 0;
    } else if(quality > 63) {
        quality = 63;
    }
    sensor->status.quality = quality;
    return write_reg(sensor, BANK_DSP, QS, quality);
}

static int set_agc_gain(sensor_t *sensor, int gain)
{
    if(gain < 0) {
        gain = 0;
    } else if(gain > 30) {
        gain = 30;
    }
    sensor->status.agc_gain = gain;
    return write_reg(sensor, BANK_SENSOR, GAIN, agc_gain_tbl[gain]);
}

static int set_gainceiling_sensor(sensor_t *sensor, gainceiling_t gainceiling)
{
    sensor->status.gainceiling = gainceiling;
    //return write_reg(sensor, BANK_SENSOR, COM9, COM9_AGC_SET(gainceiling));
    return set_reg_bits(sensor, BANK_SENSOR, COM9, 5, 7, gainceiling);
}

static int set_aec_value(sensor_t *sensor, int value)
{
    if(value < 0) {
        value = 0;
    } else if(value > 1200) {
        value = 1200;
    }
    sensor->status.aec_value = value;
    return set_reg_bits(sensor, BANK_SENSOR, REG04, 0, 3, value & 0x3)
           || write_reg(sensor, BANK_SENSOR, AEC, (value >> 2) & 0xFF)
           || set_reg_bits(sensor, BANK_SENSOR, REG45, 0, 0x3F, value >> 10);
}

static int set_aec2(sensor_t *sensor, int enable)
{
    sensor->status.aec2 = enable;
    return set_reg_bits(sensor, BANK_DSP, CTRL0, 6, 1, enable?0:1);
}

static int set_colorbar(sensor_t *sensor, int enable)
{
    sensor->status.colorbar = enable;
    return write_reg_bits(sensor, BANK_SENSOR, COM7, COM7_COLOR_BAR, enable?1:0);
}

static int set_agc_sensor(sensor_t *sensor, int enable)
{
    sensor->status.agc = enable;
    return write_reg_bits(sensor, BANK_SENSOR, COM8, COM8_AGC_EN, enable?1:0);
}

static int set_aec_sensor(sensor_t *sensor, int enable)
{
    sensor->status.aec = enable;
    return write_reg_bits(sensor, BANK_SENSOR, COM8, COM8_AEC_EN, enable?1:0);
}

static int set_hmirror_sensor(sensor_t *sensor, int enable)
{
    sensor->status.hmirror = enable;
    return write_reg_bits(sensor, BANK_SENSOR, REG04, REG04_HFLIP_IMG, enable?1:0);
}

static int set_vflip_sensor(sensor_t *sensor, int enable)
{
    int ret = 0;
    sensor->status.vflip = enable;
    ret = write_reg_bits(sensor, BANK_SENSOR, REG04, REG04_VREF_EN, enable?1:0);
    return ret & write_reg_bits(sensor, BANK_SENSOR, REG04, REG04_VFLIP_IMG, enable?1:0);
}

static int set_raw_gma_dsp(sensor_t *sensor, int enable)
{
    sensor->status.raw_gma = enable;
    return set_reg_bits(sensor, BANK_DSP, CTRL1, 5, 1, enable?1:0);
}

static int set_awb_dsp(sensor_t *sensor, int enable)
{
    sensor->status.awb = enable;
    return set_reg_bits(sensor, BANK_DSP, CTRL1, 3, 1, enable?1:0);
}

static int set_awb_gain_dsp(sensor_t *sensor, int enable)
{
    sensor->status.awb_gain = enable;
    return set_reg_bits(sensor, BANK_DSP, CTRL1, 2, 1, enable?1:0);
}

static int set_lenc_dsp(sensor_t *sensor, int enable)
{
    sensor->status.lenc = enable;
    return set_reg_bits(sensor, BANK_DSP, CTRL1, 1, 1, enable?1:0);
}

static int set_dcw_dsp(sensor_t *sensor, int enable)
{
    sensor->status.dcw = enable;
    return set_reg_bits(sensor, BANK_DSP, CTRL2, 5, 1, enable?1:0);
}

static int set_bpc_dsp(sensor_t *sensor, int enable)
{
    sensor->status.bpc = enable;
    return set_reg_bits(sensor, BANK_DSP, CTRL3, 7, 1, enable?1:0);
}

static int set_wpc_dsp(sensor_t *sensor, int enable)
{
    sensor->status.wpc = enable;
    return set_reg_bits(sensor, BANK_DSP, CTRL3, 6, 1, enable?1:0);
}

//unsupported
static int set_sharpness(sensor_t *sensor, int level)
{
   return -1;
}

static int set_denoise(sensor_t *sensor, int level)
{
   return -1;
}

static int get_reg(sensor_t *sensor, int reg, int mask)
{
    int ret = read_reg(sensor, (reg >> 8) & 0x01, reg & 0xFF);
    if(ret > 0){
        ret &= mask;
    }
    return ret;
}

static int set_reg(sensor_t *sensor, int reg, int mask, int value)
{
    int ret = 0;
    ret = read_reg(sensor, (reg >> 8) & 0x01, reg & 0xFF);
    if(ret < 0){
        return ret;
    }
    value = (ret & ~mask) | (value & mask);
    ret = write_reg(sensor, (reg >> 8) & 0x01, reg & 0xFF, value);
    return ret;
}

static int set_res_raw(sensor_t *sensor, int startX, int startY, int endX, int endY, int offsetX, int offsetY, int totalX, int totalY, int outputX, int outputY, bool scale, bool binning)
{
    return set_window(sensor, (ov2640_sensor_mode_t)startX, offsetX, offsetY, totalX, totalY, outputX, outputY);
}

static int _set_pll(sensor_t *sensor, int bypass, int multiplier, int sys_div, int root_2x, int pre_div, int seld5, int pclk_manual, int pclk_div)
{
    return -1;
}

static int set_xclk(sensor_t *sensor, int timer, int xclk)
{
    int ret = 0;
    sensor->xclk_freq_hz = xclk * 1000000U;
    ret = xclk_timer_conf(timer, sensor->xclk_freq_hz);
    return ret;
}

static int init_status(sensor_t *sensor){
    sensor->status.brightness = 0;
    sensor->status.contrast = 0;
    sensor->status.saturation = 0;
    sensor->status.ae_level = 0;
    sensor->status.special_effect = 0;
    sensor->status.wb_mode = 0;

    sensor->status.agc_gain = 30;
    int agc_gain = read_reg(sensor, BANK_SENSOR, GAIN);
    for (int i=0; i<30; i++){
        if(agc_gain >= agc_gain_tbl[i] && agc_gain < agc_gain_tbl[i+1]){
            sensor->status.agc_gain = i;
            break;
        }
    }

    sensor->status.aec_value = ((uint16_t)get_reg_bits(sensor, BANK_SENSOR, REG45, 0, 0x3F) << 10)
                             | ((uint16_t)read_reg(sensor, BANK_SENSOR, AEC) << 2)
                             | get_reg_bits(sensor, BANK_SENSOR, REG04, 0, 3);//0 - 1200
    sensor->status.quality = read_reg(sensor, BANK_DSP, QS);
    sensor->status.gainceiling = get_reg_bits(sensor, BANK_SENSOR, COM9, 5, 7);

    sensor->status.awb = get_reg_bits(sensor, BANK_DSP, CTRL1, 3, 1);
    sensor->status.awb_gain = get_reg_bits(sensor, BANK_DSP, CTRL1, 2, 1);
    sensor->status.aec = get_reg_bits(sensor, BANK_SENSOR, COM8, 0, 1);
    sensor->status.aec2 = get_reg_bits(sensor, BANK_DSP, CTRL0, 6, 1);
    sensor->status.agc = get_reg_bits(sensor, BANK_SENSOR, COM8, 2, 1);
    sensor->status.bpc = get_reg_bits(sensor, BANK_DSP, CTRL3, 7, 1);
    sensor->status.wpc = get_reg_bits(sensor, BANK_DSP, CTRL3, 6, 1);
    sensor->status.raw_gma = get_reg_bits(sensor, BANK_DSP, CTRL1, 5, 1);
    sensor->status.lenc = get_reg_bits(sensor, BANK_DSP, CTRL1, 1, 1);
    sensor->status.hmirror = get_reg_bits(sensor, BANK_SENSOR, REG04, 7, 1);
    sensor->status.vflip = get_reg_bits(sensor, BANK_SENSOR, REG04, 6, 1);
    sensor->status.dcw = get_reg_bits(sensor, BANK_DSP, CTRL2, 5, 1);
    sensor->status.colorbar = get_reg_bits(sensor, BANK_SENSOR, COM7, 1, 1);

    sensor->status.sharpness = 0;//not supported
    sensor->status.denoise = 0;
    return 0;
}

int ov2640_detect(int slv_addr, sensor_id_t *id)
{
    if (OV2640_SCCB_ADDR == slv_addr) {
        SCCB_Write(slv_addr, 0xFF, 0x01);//bank sensor
        uint16_t PID = SCCB_Read(slv_addr, 0x0A);
        if (OV2640_PID == PID) {
            id->PID = PID;
            id->VER = SCCB_Read(slv_addr, REG_VER);
            id->MIDL = SCCB_Read(slv_addr, REG_MIDL);
            id->MIDH = SCCB_Read(slv_addr, REG_MIDH);
            return PID;
        } else {
            ESP_LOGI(TAG, "Mismatch PID=0x%x", PID);
        }
    }
    return 0;
}

int ov2640_init(sensor_t *sensor)
{
    sensor->reset = reset;
    sensor->init_status = init_status;
    sensor->set_pixformat = set_pixformat;
    sensor->set_framesize = set_framesize;
    sensor->set_contrast  = set_contrast;
    sensor->set_brightness= set_brightness;
    sensor->set_saturation= set_saturation;

    sensor->set_quality = set_quality;
    sensor->set_colorbar = set_colorbar;

    sensor->set_gainceiling = set_gainceiling_sensor;
    sensor->set_gain_ctrl = set_agc_sensor;
    sensor->set_exposure_ctrl = set_aec_sensor;
    sensor->set_hmirror = set_hmirror_sensor;
    sensor->set_vflip = set_vflip_sensor;

    sensor->set_whitebal = set_awb_dsp;
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

    //not supported
    sensor->set_sharpness = set_sharpness;
    sensor->set_denoise = set_denoise;

    sensor->get_reg = get_reg;
    sensor->set_reg = set_reg;
    sensor->set_res_raw = set_res_raw;
    sensor->set_pll = _set_pll;
    sensor->set_xclk = set_xclk;
    ESP_LOGD(TAG, "OV2640 Attached");
    return 0;
}
