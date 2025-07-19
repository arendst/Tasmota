/*
  ScioSense_ENS210.h - Library for the ENS210 relative humidity and temperature sensor with I2C interface from ScioSense
  2020 Apr 06	v3	Chrsitoph Friese	Changed nomenclature to ScioSense as product shifted from ams
  2018 Aug 28	v2	Christoph Friese	Adjusted I2C communication 
  2017 Aug 01	v1	Maarten Pennings	Created
*/

#ifndef __SCIOSENSE_ENS210_H_
#define __SCIOSENSE_ENS210_H_

#define ENS210_DISABLE_DEBUG
#define ENS210_DISABLE_ENHANCED_FEATURES

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h>

#define ENS210_I2CADDR          	0x43		//ADDR low

// Chip constants
#define ENS210_PARTID          0x0210 		// The expected part id of the ENS210
#define ENS210_BOOTING              2 		// Booting time in ms (also after reset, or going to high power)
#define ENS210_THCONV_SINGLE_MS 130 // Conversion time in ms for single shot T/H measurement
#define ENS210_THCONV_CONT_MS   238 // Conversion time in ms for continuous T/H measurement

// Addresses of the ENS210 registers
#define ENS210_REG_PART_ID       0x00
#define ENS210_REG_REV		     0x02
#define ENS210_REG_UID           0x04
#define ENS210_REG_SYS_CTRL      0x10
#define ENS210_REG_SYS_STAT      0x11
#define ENS210_REG_SENS_RUN      0x21
#define ENS210_REG_SENS_START    0x22
#define ENS210_REG_SENS_STOP     0x23
#define ENS210_REG_SENS_STAT     0x24
#define ENS210_REG_T_VAL         0x30
#define ENS210_REG_H_VAL         0x33

// Division macro (used in conversion functions), implementing integer division with rounding.
// It supports both positive and negative dividends (n), but ONLY positive divisors (d).
//#define IDIV(n,d)                ((n)>0 ? ((n)+(d)/2)/(d) : ((n)-(d)/2)/(d))

//               7654 3210
// Polynomial 0b 1000 1001 ~ x^7+x^3+x^0
//            0x    8    9
#define CRC7WIDTH  7    					// A 7 bits CRC has polynomial of 7th order, which has 8 terms
#define CRC7POLY   0x89 					// The 8 coefficients of the polynomial
#define CRC7IVEC   0x7F 					// Initial vector has all 7 bits high
// Payload data
#define DATA7WIDTH 17
#define DATA7MASK  ((1UL<<DATA7WIDTH)-1) 	// 0b 0 1111 1111 1111 1111
#define DATA7MSB   (1UL<<(DATA7WIDTH-1)) 	// 0b 1 0000 0000 0000 0000

// Measurement status as output by `measure()` and `extract()`.
// Note that the ENS210 provides a "value" (`t_val` or `h_val` each 24 bit).
// A "value" consists of a payload (17 bit) and a CRC (7 bit) over that payload.
// The payload consists of a valid flag (1 bit) and the actual measurement "data" (`t_data` or `h_data`, 16 bit)
#define ENS210_STATUS_I2CERROR  4 // There was an I2C communication error, `read`ing the value.
#define ENS210_STATUS_CRCERROR  3 // The value was read, but the CRC over the payload (valid and data) does not match.
#define ENS210_STATUS_INVALID   2 // The value was read, the CRC matches, but the data is invalid (e.g. the measurement was not yet finished).
#define ENS210_STATUS_OK        1 // The value was read, the CRC matches, and data is valid.

class ScioSense_ENS210 {
		
	public:
		ScioSense_ENS210(uint8_t slaveaddr = ENS210_I2CADDR);
		
#ifndef ENS210_DISABLE_ENHANCED_FEATURES
		void 				setI2C(uint8_t sda, uint8_t scl);
		void				changeAddr(uint8_t oldAddr,  uint8_t newAddr);
#endif

		bool 				begin(bool debug=false);				// init i2c interface, get partID und uID. Returns false on I2C problems or wrong PART_ID.
		bool 				setSingleMode(bool enable);				// false for continuous mode / true for single shot measurement. Returns false on I2C problems.
		
		bool				available()	{ return this->_available; }
		uint16_t 			getPartID() { return this->_partID; }
		uint16_t 			getRev() { return this->_rev; }
		uint32_t 			getHighUID(bool high) { uint32_t result  = high ? this->_uIDhi : this->_uIDlo; return result; }
		void 				measure(); 									// perfrom measurement and stores result in internal variables
#ifndef ENS210_DISABLE_ENHANCED_FEATURES
		float 				getTempKelvin     ();    					// Converts and returns data (from `measure`) in Kelvin
		float 				getTempFahrenheit ();    					// Converts and returns data (from `measure`) in  Fahrenheit
#endif
		float 				getTempCelsius    ();    					// Converts and returns data (from `measure`) in Celsius
		float 				getHumidityPercent();    					// Converts and returns data (from `measure`) in %RH
#ifndef ENS210_DISABLE_ENHANCED_FEATURES
		float 				getAbsoluteHumidityPercent();    			// Converts and returns data (from `measure`) in %aH
#endif
		char  				getStatusT() { return this->_t_status; }    // Converts a status (ENS210_STATUS_XXX) to a human readable string.
		uint32_t  			getDataT() { return this->_t_data; }    		
		char  				getStatusH() { return this->_h_status; }	// Converts a status (ENS210_STATUS_XXX) to a human readable string.
		uint32_t			getDataH() { return this->_h_data; }		
#ifndef ENS210_DISABLE_ENHANCED_FEATURES
		static const char * status_str( int status );          			// Converts a status (ENS210_STATUS_XXX) to a human readable string.
#endif

		// Optionally set a solder `correction` (units: 1/64K, default from `begin` is 0).
		// See "Effect of Soldering on Temperature Readout" in "Design-Guidelines" from
		// https://download.ams.com/ENVIRONMENTAL-SENSORS/ENS210/Documentation
#ifndef ENS210_DISABLE_ENHANCED_FEATURES
		void correction_set(int correction=50*64/1000);       			// Sets the solder correction (default is 50mK) - only used by the `toXxx()` functions.
		int  correction_get() {return this->_soldercorrection;}			// Gets the solder correction.
#endif

	private:
#ifndef ENS210_DISABLE_DEBUG
		bool				debugENS210 = false;
#endif
		bool 				reset(void);                                // Sends a reset to the ENS210. Returns false on I2C problems.
		bool 				lowpower(bool enable);                      // Sets ENS210 to low (true) or high (false) power. Returns false on I2C problems.
		bool 				getversion();								// Reads PART_ID and UID of ENS210. Returns false on I2C problems.
		bool 				readValue(); 								// Reads measurement data from the ENS210. Returns false on I2C problems.
		
		bool				_available = false;							// ENS210 available
		uint16_t 			_partID;									// Part ID of ENS210, should be 0x210
		uint16_t 			_rev;										// Revision 0 MRA2.6 / 1 MRA2.12
		uint64_t 			_uID;										// Unique ID of this specific ENS210
		uint32_t 			_uIDhi;										// First 32bit of unique ID of this specific ENS210
		uint32_t 			_uIDlo;										// Second 32bit of unique ID of this specific ENS210
		bool				_singleMode = true;							// Measurement mode: true single shot / false continuous
		uint32_t			_t_data;
		uint8_t				_t_status;
		uint32_t 			_h_data;
		uint8_t 			_h_status; 
		uint8_t  			_slaveaddress = 0x43;      					// Slave address of ENS210
		uint8_t  			_soldercorrection;							// Correction due to soldering (in 1/64K); subtracted from `t_data` by conversion functions.

		uint8_t				_sdaPin = 0;
		uint8_t				_sclPin = 0;	

	/****************************************************************************/
	/*	General functions														*/
    /****************************************************************************/
		
		uint8_t 			write8(uint8_t addr, byte reg, byte value);
		uint8_t				read8(uint8_t addr, byte reg);
			
		uint8_t				read(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t num);
		uint8_t				write(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t num);
		void 				_i2c_init();
};


#endif
