#include "Arduino.h"

void SPL_init(uint8_t spl_address=0x76);

uint8_t get_spl_id();		// Get ID Register 		0x0D
uint8_t get_spl_prs_cfg();	// Get PRS_CFG Register	0x06
uint8_t get_spl_tmp_cfg();	// Get TMP_CFG Register	0x07
uint8_t get_spl_meas_cfg();	// Get MEAS_CFG Register	0x08
uint8_t get_spl_cfg_reg();	// Get CFG_REG Register	0x09
uint8_t get_spl_int_sts();	// Get INT_STS Register	0x0A
uint8_t get_spl_fifo_sts();	// Get FIFO_STS Register	0x0B

double get_altitude(double pressure, double seaLevelhPa);	// get altitude in meters
double get_altitude_f(double pressure, double seaLevelhPa);	// get altitude in feet

int32_t get_traw();
double get_traw_sc();
double get_temp_c();
double get_temp_f();
double get_temperature_scale_factor();

int32_t get_praw();
double get_praw_sc();
double get_pcomp();
double get_pressure_scale_factor();
double get_pressure();

int16_t get_c0();
int16_t get_c1();
int32_t get_c00();
int32_t get_c10();
int16_t get_c01();
int16_t get_c11();
int16_t get_c20();
int16_t get_c21();
int16_t get_c30();

void i2c_eeprom_write_uint8_t(  uint8_t deviceaddress, uint8_t eeaddress, uint8_t data );
uint8_t i2c_eeprom_read_uint8_t(  uint8_t deviceaddress, uint8_t eeaddress );

