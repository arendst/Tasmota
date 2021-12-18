/*
 * This file is part of the OpenMV project.
 * Copyright (c) 2013/2014 Ibrahim Abdelkader <i.abdalkader@gmail.com>
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 * OV7725 driver.
 *
 */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sccb.h"
#include "xclk.h"
#include "ov7725.h"
#include "ov7725_regs.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#if defined(ARDUINO_ARCH_ESP32) && defined(CONFIG_ARDUHAL_ESP_LOG)
#include "esp32-hal-log.h"
#else
#include "esp_log.h"
static const char* TAG = "ov7725";
#endif


static const uint8_t default_regs[][2] = {
    {COM3,          COM3_SWAP_YUV},
    {COM7,          COM7_RES_QVGA | COM7_FMT_YUV},

    {COM4,          0x01 | 0x00}, /* bypass PLL (0x00:off, 0x40:4x, 0x80:6x, 0xC0:8x) */
    {CLKRC,         0x80 | 0x03}, /* Res/Bypass pre-scalar (0x40:bypass, 0x00-0x3F:prescaler PCLK=XCLK/(prescaler + 1)/2 ) */

    // QVGA Window Size
    {HSTART,        0x3F},
    {HSIZE,         0x50},
    {VSTART,        0x03},
    {VSIZE,         0x78},
    {HREF,          0x00},

    // Scale down to QVGA Resolution
    {HOUTSIZE,      0x50},
    {VOUTSIZE,      0x78},
    {EXHCH,         0x00},

    {COM12,         0x03},
    {TGT_B,         0x7F},
    {FIXGAIN,       0x09},
    {AWB_CTRL0,     0xE0},
    {DSP_CTRL1,     0xFF},

    {DSP_CTRL2,     DSP_CTRL2_VDCW_EN | DSP_CTRL2_HDCW_EN | DSP_CTRL2_HZOOM_EN | DSP_CTRL2_VZOOM_EN},

    {DSP_CTRL3,     0x00},
    {DSP_CTRL4,     0x00},
    {DSPAUTO,       0xFF},

    {COM8,          0xF0},
    {COM6,          0xC5},
    {COM9,          0x11},
    {COM10,         COM10_VSYNC_NEG | COM10_PCLK_MASK}, //Invert VSYNC and MASK PCLK
    {BDBASE,        0x7F},
    {DBSTEP,        0x03},
    {AEW,           0x96},
    {AEB,           0x64},
    {VPT,           0xA1},
    {EXHCL,         0x00},
    {AWB_CTRL3,     0xAA},
    {COM8,          0xFF},

    //Gamma
    {GAM1,          0x0C},
    {GAM2,          0x16},
    {GAM3,          0x2A},
    {GAM4,          0x4E},
    {GAM5,          0x61},
    {GAM6,          0x6F},
    {GAM7,          0x7B},
    {GAM8,          0x86},
    {GAM9,          0x8E},
    {GAM10,         0x97},
    {GAM11,         0xA4},
    {GAM12,         0xAF},
    {GAM13,         0xC5},
    {GAM14,         0xD7},
    {GAM15,         0xE8},

    {SLOP,          0x20},
    {EDGE1,         0x05},
    {EDGE2,         0x03},
    {EDGE3,         0x00},
    {DNSOFF,        0x01},

    {MTX1,          0xB0},
    {MTX2,          0x9D},
    {MTX3,          0x13},
    {MTX4,          0x16},
    {MTX5,          0x7B},
    {MTX6,          0x91},
    {MTX_CTRL,      0x1E},

    {BRIGHTNESS,    0x08},
    {CONTRAST,      0x30},
    {UVADJ0,        0x81},
    {SDE,           (SDE_CONT_BRIGHT_EN | SDE_SATURATION_EN)},

    // For 30 fps/60Hz
    {DM_LNL,        0x00},
    {DM_LNH,        0x00},
    {BDBASE,        0x7F},
    {DBSTEP,        0x03},

    // Lens Correction, should be tuned with real camera module
    {LC_RADI,       0x10},
    {LC_COEF,       0x10},
    {LC_COEFB,      0x14},
    {LC_COEFR,      0x17},
    {LC_CTR,        0x05},
    {COM5,          0xF5}, //0x65

    {0x00,          0x00},
};

static int get_reg(sensor_t *sensor, int reg, int mask)
{
    int ret = SCCB_Read(sensor->slv_addr, reg & 0xFF);
    if(ret > 0){
        ret &= mask;
    }
    return ret;
}

static int set_reg(sensor_t *sensor, int reg, int mask, int value)
{
    int ret = 0;
    ret = SCCB_Read(sensor->slv_addr, reg & 0xFF);
    if(ret < 0){
        return ret;
    }
    value = (ret & ~mask) | (value & mask);
    ret = SCCB_Write(sensor->slv_addr, reg & 0xFF, value);
    return ret;
}

static int set_reg_bits(sensor_t *sensor, uint8_t reg, uint8_t offset, uint8_t length, uint8_t value)
{
    int ret = 0;
    ret = SCCB_Read(sensor->slv_addr, reg);
    if(ret < 0){
        return ret;
    }
    uint8_t mask = ((1 << length) - 1) << offset;
    value = (ret & ~mask) | ((value << offset) & mask);
    ret = SCCB_Write(sensor->slv_addr, reg & 0xFF, value);
    return ret;
}

static int get_reg_bits(sensor_t *sensor, uint8_t reg, uint8_t offset, uint8_t length)
{
    int ret = 0;
    ret = SCCB_Read(sensor->slv_addr, reg);
    if(ret < 0){
        return ret;
    }
    uint8_t mask = ((1 << length) - 1) << offset;
    return (ret & mask) >> offset;
}


static int reset(sensor_t *sensor)
{
    int i=0;
    const uint8_t (*regs)[2];

    // Reset all registers
    SCCB_Write(sensor->slv_addr, COM7, COM7_RESET);

    // Delay 10 ms
    vTaskDelay(10 / portTICK_PERIOD_MS);

    // Write default regsiters
    for (i=0, regs = default_regs; regs[i][0]; i++) {
        SCCB_Write(sensor->slv_addr, regs[i][0], regs[i][1]);
    }

    // Delay
    vTaskDelay(30 / portTICK_PERIOD_MS);

    return 0;
}


static int set_pixformat(sensor_t *sensor, pixformat_t pixformat)
{
    int ret=0;
    sensor->pixformat = pixformat;
    // Read register COM7
    uint8_t reg = SCCB_Read(sensor->slv_addr, COM7);

    switch (pixformat) {
    case PIXFORMAT_RGB565:
        reg =  COM7_SET_RGB(reg, COM7_FMT_RGB565);
        break;
    case PIXFORMAT_YUV422:
    case PIXFORMAT_GRAYSCALE:
        reg =  COM7_SET_FMT(reg, COM7_FMT_YUV);
        break;
    default:
        return -1;
    }

    // Write back register COM7
    ret = SCCB_Write(sensor->slv_addr, COM7, reg);

    // Delay
    vTaskDelay(30 / portTICK_PERIOD_MS);

    return ret;
}

static int set_framesize(sensor_t *sensor, framesize_t framesize)
{
    int ret=0;
    if (framesize > FRAMESIZE_VGA) {
        return -1;
    }
    uint16_t w = resolution[framesize].width;
    uint16_t h = resolution[framesize].height;
    uint8_t reg = SCCB_Read(sensor->slv_addr, COM7);

    sensor->status.framesize = framesize;

    // Write MSBs
    ret |= SCCB_Write(sensor->slv_addr, HOUTSIZE, w>>2);
    ret |= SCCB_Write(sensor->slv_addr, VOUTSIZE, h>>1);

    ret |= SCCB_Write(sensor->slv_addr, HSIZE, w>>2);
    ret |= SCCB_Write(sensor->slv_addr, VSIZE, h>>1);

    // Write LSBs
    ret |= SCCB_Write(sensor->slv_addr, HREF, ((w&0x3) | ((h&0x1) << 2)));

    if (framesize < FRAMESIZE_VGA) {
        // Enable auto-scaling/zooming factors
        ret |= SCCB_Write(sensor->slv_addr, DSPAUTO, 0xFF);

        ret |= SCCB_Write(sensor->slv_addr, HSTART, 0x3F);
        ret |= SCCB_Write(sensor->slv_addr, VSTART, 0x03);

        ret |= SCCB_Write(sensor->slv_addr, COM7, reg | COM7_RES_QVGA);

        ret |= SCCB_Write(sensor->slv_addr, CLKRC, 0x80 | 0x01);

    } else {
        // Disable auto-scaling/zooming factors
        ret |= SCCB_Write(sensor->slv_addr, DSPAUTO, 0xF3);

        // Clear auto-scaling/zooming factors
        ret |= SCCB_Write(sensor->slv_addr, SCAL0, 0x00);
        ret |= SCCB_Write(sensor->slv_addr, SCAL1, 0x00);
        ret |= SCCB_Write(sensor->slv_addr, SCAL2, 0x00);

        ret |= SCCB_Write(sensor->slv_addr, HSTART, 0x23);
        ret |= SCCB_Write(sensor->slv_addr, VSTART, 0x07);

        ret |= SCCB_Write(sensor->slv_addr, COM7, reg & ~COM7_RES_QVGA);

        ret |= SCCB_Write(sensor->slv_addr, CLKRC, 0x80 | 0x03);
    }

    // Delay
    vTaskDelay(30 / portTICK_PERIOD_MS);

    return ret;
}

static int set_colorbar(sensor_t *sensor, int enable)
{
    int ret=0;
    uint8_t reg;
    sensor->status.colorbar = enable;

    // Read reg COM3
    reg = SCCB_Read(sensor->slv_addr, COM3);
    // Enable colorbar test pattern output
    reg = COM3_SET_CBAR(reg, enable);
    // Write back COM3
    ret |= SCCB_Write(sensor->slv_addr, COM3, reg);

    // Read reg DSP_CTRL3
    reg = SCCB_Read(sensor->slv_addr, DSP_CTRL3);
    // Enable DSP colorbar output
    reg = DSP_CTRL3_SET_CBAR(reg, enable);
    // Write back DSP_CTRL3
    ret |= SCCB_Write(sensor->slv_addr, DSP_CTRL3, reg);

    return ret;
}

static int set_whitebal(sensor_t *sensor, int enable)
{
    if(set_reg_bits(sensor, COM8, 1, 1, enable) >= 0){
        sensor->status.awb = !!enable;
    }
    return sensor->status.awb;
}

static int set_gain_ctrl(sensor_t *sensor, int enable)
{
    if(set_reg_bits(sensor, COM8, 2, 1, enable) >= 0){
        sensor->status.agc = !!enable;
    }
    return sensor->status.agc;
}

static int set_exposure_ctrl(sensor_t *sensor, int enable)
{
    if(set_reg_bits(sensor, COM8, 0, 1, enable) >= 0){
        sensor->status.aec = !!enable;
    }
    return sensor->status.aec;
}

static int set_hmirror(sensor_t *sensor, int enable)
{
    if(set_reg_bits(sensor, COM3, 6, 1, enable) >= 0){
        sensor->status.hmirror = !!enable;
    }
    return sensor->status.hmirror;
}

static int set_vflip(sensor_t *sensor, int enable)
{
    if(set_reg_bits(sensor, COM3, 7, 1, enable) >= 0){
        sensor->status.vflip = !!enable;
    }
    return sensor->status.vflip;
}

static int set_dcw_dsp(sensor_t *sensor, int enable)
{
    int ret = 0;
    ret = set_reg_bits(sensor, 0x65, 2, 1, !enable);
    if (ret == 0) {
        ESP_LOGD(TAG, "Set dcw to: %d", enable);
        sensor->status.dcw = enable;
    }
    return ret;
}

static int set_aec2(sensor_t *sensor, int enable)
{
    int ret = 0;
    ret = set_reg_bits(sensor, COM8, 7, 1, enable);
    if (ret == 0) {
        ESP_LOGD(TAG, "Set aec2 to: %d", enable);
        sensor->status.aec2 = enable;
    }
    return ret;
}

static int set_bpc_dsp(sensor_t *sensor, int enable)
{
    int ret = 0;
    ret = set_reg_bits(sensor, 0x64, 1, 1, enable);
    if (ret == 0) {
        ESP_LOGD(TAG, "Set bpc to: %d", enable);
        sensor->status.bpc = enable;
    }
    return ret;
}

static int set_wpc_dsp(sensor_t *sensor, int enable)
{
    int ret = 0;
    ret = set_reg_bits(sensor, 0x64, 0, 1, enable);
    if (ret == 0) {
        ESP_LOGD(TAG, "Set wpc to: %d", enable);
        sensor->status.wpc = enable;
    }
    return ret;
}

static int set_raw_gma_dsp(sensor_t *sensor, int enable)
{
    int ret = 0;
    ret = set_reg_bits(sensor, 0x64, 2, 1, enable);
    if (ret == 0) {
        ESP_LOGD(TAG, "Set raw_gma to: %d", enable);
        sensor->status.raw_gma = enable;
    }
    return ret;
}

static int set_lenc_dsp(sensor_t *sensor, int enable)
{
    int ret = 0;
    ret = set_reg_bits(sensor, LC_CTR, 0, 1, enable);
    if (ret == 0) {
        ESP_LOGD(TAG, "Set lenc to: %d", enable);
        sensor->status.lenc = enable;
    }
    return ret;
}

//real gain
static int set_agc_gain(sensor_t *sensor, int gain)
{
    int ret = 0;
    ret = set_reg_bits(sensor, COM9, 4, 3, gain % 5);
    if (ret == 0) {
        ESP_LOGD(TAG, "Set gain to: %d", gain);
        sensor->status.agc_gain = gain;
    }
    return ret;
}

static int set_aec_value(sensor_t *sensor, int value)
{
    int ret = 0;
    ret =  SCCB_Write(sensor->slv_addr, AEC, value & 0xff) | SCCB_Write(sensor->slv_addr, AECH, value >> 8);
    if (ret == 0) {
        ESP_LOGD(TAG, "Set aec_value to: %d", value);
        sensor->status.aec_value = value;
    }
    return ret;
}

static int set_awb_gain_dsp(sensor_t *sensor, int enable)
{
    int ret = 0;
    ret = set_reg_bits(sensor, 0x63, 7, 1, enable);
    if (ret == 0) {
        ESP_LOGD(TAG, "Set awb_gain to: %d", enable);
        sensor->status.awb_gain = enable;
    }
    return ret;
}

static int set_brightness(sensor_t *sensor, int level)
{
    int ret = 0;
    ret = SCCB_Write(sensor->slv_addr, 0x9B, level);
    if (ret == 0) {
        ESP_LOGD(TAG, "Set brightness to: %d", level);
        sensor->status.brightness = level;
    }
    return ret;
}

static int set_contrast(sensor_t *sensor, int level)
{
    int ret = 0;
    ret = SCCB_Write(sensor->slv_addr, 0x9C, level);
    if (ret == 0) {
        ESP_LOGD(TAG, "Set contrast to: %d", level);
        sensor->status.contrast = level;
    }
    return ret;
}

static int init_status(sensor_t *sensor)
{
    sensor->status.brightness = SCCB_Read(sensor->slv_addr, 0x9B);
    sensor->status.contrast = SCCB_Read(sensor->slv_addr, 0x9C);
    sensor->status.saturation = 0;
    sensor->status.ae_level = 0;
    sensor->status.special_effect = get_reg_bits(sensor, 0x64, 5, 1);
    sensor->status.wb_mode = get_reg_bits(sensor, 0x6B, 7, 1);
    sensor->status.agc_gain = get_reg_bits(sensor, COM9, 4, 3);
    sensor->status.aec_value = SCCB_Read(sensor->slv_addr, AEC) | (SCCB_Read(sensor->slv_addr, AECH) << 8);
    sensor->status.gainceiling = SCCB_Read(sensor->slv_addr, 0x00);
    sensor->status.awb = get_reg_bits(sensor, COM8, 1, 1);
    sensor->status.awb_gain = get_reg_bits(sensor, 0x63, 7, 1);
    sensor->status.aec = get_reg_bits(sensor, COM8, 0, 1);
    sensor->status.aec2 = get_reg_bits(sensor, COM8, 7, 1);
    sensor->status.agc = get_reg_bits(sensor, COM8, 2, 1);
    sensor->status.bpc = get_reg_bits(sensor, 0x64, 1, 1);
    sensor->status.wpc = get_reg_bits(sensor, 0x64, 0, 1);
    sensor->status.raw_gma = get_reg_bits(sensor, 0x64, 2, 1);
    sensor->status.lenc = get_reg_bits(sensor, LC_CTR, 0, 1);
    sensor->status.hmirror = get_reg_bits(sensor, COM3, 6, 1);
    sensor->status.vflip = get_reg_bits(sensor, COM3, 7, 1);
    sensor->status.dcw = get_reg_bits(sensor, 0x65, 2, 1);
    sensor->status.colorbar = get_reg_bits(sensor, COM3, 0, 1);
    sensor->status.sharpness = get_reg_bits(sensor, EDGE0, 0, 5);
    sensor->status.denoise = SCCB_Read(sensor->slv_addr, 0x8E);
    return 0;
}

static int set_dummy(sensor_t *sensor, int val){ return -1; }
static int set_gainceiling_dummy(sensor_t *sensor, gainceiling_t val){ return -1; }
static int set_res_raw(sensor_t *sensor, int startX, int startY, int endX, int endY, int offsetX, int offsetY, int totalX, int totalY, int outputX, int outputY, bool scale, bool binning){return -1;}
static int _set_pll(sensor_t *sensor, int bypass, int multiplier, int sys_div, int root_2x, int pre_div, int seld5, int pclk_manual, int pclk_div){return -1;}

static int set_xclk(sensor_t *sensor, int timer, int xclk)
{
    int ret = 0;
    sensor->xclk_freq_hz = xclk * 1000000U;
    ret = xclk_timer_conf(timer, sensor->xclk_freq_hz);
    return ret;
}

int ov7725_detect(int slv_addr, sensor_id_t *id)
{
    if (OV7725_SCCB_ADDR == slv_addr) {
        SCCB_Write(slv_addr, 0xFF, 0x01);//bank sensor
        uint16_t PID = SCCB_Read(slv_addr, 0x0A);
        if (OV7725_PID == PID) {
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

int ov7725_init(sensor_t *sensor)
{
    // Set function pointers
    sensor->reset = reset;
    sensor->init_status = init_status;
    sensor->set_pixformat = set_pixformat;
    sensor->set_framesize = set_framesize;
    sensor->set_colorbar = set_colorbar;
    sensor->set_whitebal = set_whitebal;
    sensor->set_gain_ctrl = set_gain_ctrl;
    sensor->set_exposure_ctrl = set_exposure_ctrl;
    sensor->set_hmirror = set_hmirror;
    sensor->set_vflip = set_vflip;

    sensor->set_brightness = set_brightness;
    sensor->set_contrast = set_contrast;
    sensor->set_aec2 = set_aec2;
    sensor->set_aec_value = set_aec_value;
    sensor->set_awb_gain = set_awb_gain_dsp;
    sensor->set_agc_gain = set_agc_gain;
    sensor->set_dcw = set_dcw_dsp;
    sensor->set_bpc = set_bpc_dsp;
    sensor->set_wpc = set_wpc_dsp;
    sensor->set_raw_gma = set_raw_gma_dsp;
    sensor->set_lenc = set_lenc_dsp;

    //not supported
    sensor->set_saturation= set_dummy;
    sensor->set_sharpness = set_dummy;
    sensor->set_denoise = set_dummy;
    sensor->set_quality = set_dummy;
    sensor->set_special_effect = set_dummy;
    sensor->set_wb_mode = set_dummy;
    sensor->set_ae_level = set_dummy;
    sensor->set_gainceiling = set_gainceiling_dummy;


    sensor->get_reg = get_reg;
    sensor->set_reg = set_reg;
    sensor->set_res_raw = set_res_raw;
    sensor->set_pll = _set_pll;
    sensor->set_xclk = set_xclk;
    
    // Retrieve sensor's signature
    sensor->id.MIDH = SCCB_Read(sensor->slv_addr, REG_MIDH);
    sensor->id.MIDL = SCCB_Read(sensor->slv_addr, REG_MIDL);
    sensor->id.PID = SCCB_Read(sensor->slv_addr, REG_PID);
    sensor->id.VER = SCCB_Read(sensor->slv_addr, REG_VER);
    
    ESP_LOGD(TAG, "OV7725 Attached");

    return 0;
}
