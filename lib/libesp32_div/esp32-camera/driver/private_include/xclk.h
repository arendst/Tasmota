#pragma once

#include "esp_system.h"

esp_err_t xclk_timer_conf(int ledc_timer, int xclk_freq_hz);

esp_err_t camera_enable_out_clock();

void camera_disable_out_clock();
