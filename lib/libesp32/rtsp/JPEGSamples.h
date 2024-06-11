#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3

#pragma once

#ifndef ARDUINO_ARCH_ESP32
#define INCLUDE_SIMDATA
#endif

#ifdef INCLUDE_SIMDATA
extern unsigned const char capture_jpg[];
extern unsigned const char octo_jpg[];
extern unsigned int octo_jpg_len, capture_jpg_len;
#endif

#endif  // CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3