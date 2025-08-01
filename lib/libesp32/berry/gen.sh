#!/bin/bash
#
# generate all precompiled Berry structures from multiple modules
#
# Included in the Platformio build process with `pio-tools/gen-berry-structures.py
#
rm -Rf ./generate/be_*.h
python3 tools/coc/coc -o generate src default ../berry_tasmota/src ../berry_mapping/src ../berry_int64/src ../../libesp32_lvgl/lv_binding_berry/src ../../libesp32_lvgl/lv_haspmota/src/solidify ../berry_matter/src/solidify ../berry_matter/src ../berry_animate/src/solidify ../berry_animate/src ../berry_animation/src/solidify ../berry_animation/src ../../libesp32_lvgl/lv_binding_berry/src/solidify ../../libesp32_lvgl/lv_binding_berry/generate -c default/berry_conf.h
