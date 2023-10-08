#pragma once

/**
 * Initialize the ADC for temperature measurement.
 */
void epd_temperature_init();

/**
 * Get the current ambient temperature in °C.
 */
float epd_ambient_temperature();
