cmake_minimum_required(VERSION 3.16.0)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(tasmota)

if(CONFIG_IDF_TARGET_ESP32C2)
    include(relinker)
endif()
