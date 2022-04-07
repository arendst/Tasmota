#!/bin/bash
python3 tools/pycoc/main.py -o generate src default ../berry_tasmota/src ../berry_mapping/src ../berry_int64/src ../../libesp32_lvgl/lv_binding_berry/src ../../libesp32_lvgl/lv_binding_berry/generate -c default/berry_conf.h
