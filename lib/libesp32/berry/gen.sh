#!/bin/bash
#
# generate all precompiled Berry structures from multiple modules
#
# Should be eventually included in the build process
#
rm generate/be_*.h
python3 tools/coc/coc -o generate src default ../berry_tasmota/src ../berry_mapping/src ../berry_int64/src ../../libesp32_lvgl/lv_binding_berry/src ../../libesp32_lvgl/lv_binding_berry/generate -c default/berry_conf.h
