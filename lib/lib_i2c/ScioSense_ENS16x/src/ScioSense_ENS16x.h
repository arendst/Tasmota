/*
  ScioSense_ENS16x.h - Library for the ENS160 & ENS161 sensor with I2C interface from ScioSense
  2023 Jul 03	v8	Christoph Friese	Update to cover ENS160 and ENS161
  2023 Mar 23	v7	Christoph Friese	Bugfix measurement routine, prepare next release
  2021 Nov 25   v6	Martin Herold		Custom mode timing fixed
  2021 July 29	v5	Christoph Friese	Changed nomenclature to ScioSense as product shifted from ams
  2021 Feb 04	v4	Giuseppe de Pinto	Custom mode fixed
  2020 Apr 06	v3	Christoph Friese	Changed nomenclature to ScioSense as product shifted from ams
  2020 Feb 15	v2	Giuseppe Pasetti	Corrected firmware flash option
  2019 May 05	v1	Christoph Friese	Created
  
  based on application note "ENS160 Software Integration.pdf" rev 0.01
*/

#ifndef __SCIOSENSE_ENS16x_H_
#define __SCIOSENSE_ENS16x_H_

#define ENS16x_DISABLE_DEBUG
#define ENS16x_DISABLE_ENHANCED_FEATURES

#if (ARDUINO >= 100)
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Wire.h>

// Chip constants
#define ENS160_PARTID				0x0160
#define ENS161_PARTID				0x0161
#define ENS16x_BOOTING				10

// 7-bit I2C slave address of the ENS16x
#define ENS16x_I2CADDR_0          	0x52		//ADDR low
#define ENS16x_I2CADDR_1          	0x53		//ADDR high

// ENS16x registers for version V0
#define ENS16x_REG_PART_ID          	0x00 		// 2 byte register
#define ENS16x_REG_OPMODE		0x10
#define ENS16x_REG_CONFIG		0x11
#define ENS16x_REG_COMMAND		0x12
#define ENS16x_REG_TEMP_IN		0x13
#define ENS16x_REG_RH_IN		0x15
#define ENS16x_REG_DATA_STATUS		0x20
#define ENS16x_REG_DATA_AQI		0x21
#define ENS16x_REG_DATA_TVOC		0x22
#define ENS16x_REG_DATA_ECO2		0x24			
#define ENS16x_REG_DATA_BL		0x28
#define ENS16x_REG_DATA_T		0x30
#define ENS16x_REG_DATA_RH		0x32
#define ENS16x_REG_DATA_MISR		0x38
#define ENS16x_REG_GPR_WRITE_0		0x40
#define ENS16x_REG_GPR_WRITE_1		ENS16x_REG_GPR_WRITE_0 + 1
#define ENS16x_REG_GPR_WRITE_2		ENS16x_REG_GPR_WRITE_0 + 2
#define ENS16x_REG_GPR_WRITE_3		ENS16x_REG_GPR_WRITE_0 + 3
#define ENS16x_REG_GPR_WRITE_4		ENS16x_REG_GPR_WRITE_0 + 4
#define ENS16x_REG_GPR_WRITE_5		ENS16x_REG_GPR_WRITE_0 + 5
#define ENS16x_REG_GPR_WRITE_6		ENS16x_REG_GPR_WRITE_0 + 6
#define ENS16x_REG_GPR_WRITE_7		ENS16x_REG_GPR_WRITE_0 + 7
#define ENS16x_REG_GPR_READ_0		0x48
#define ENS16x_REG_GPR_READ_4		ENS16x_REG_GPR_READ_0 + 4
#define ENS16x_REG_GPR_READ_6		ENS16x_REG_GPR_READ_0 + 6
#define ENS16x_REG_GPR_READ_7		ENS16x_REG_GPR_READ_0 + 7

//ENS16x data register fields
#define ENS16x_COMMAND_NOP		0x00
#define ENS16x_COMMAND_CLRGPR		0xCC
#define ENS16x_COMMAND_GET_APPVER	0x0E 
#define ENS16x_COMMAND_SETTH		0x02
#define ENS16x_COMMAND_SETSEQ		0xC2

#define ENS16x_OPMODE_RESET		0xF0
#define ENS16x_OPMODE_DEP_SLEEP		0x00
#define ENS16x_OPMODE_IDLE		0x01
#define ENS16x_OPMODE_STD		0x02
#define ENS161_OPMODE_LP		0x03	
#define ENS16x_OPMODE_CUSTOM		0xC0

#define ENS16x_BL_CMD_START		0x02
#define ENS16x_BL_CMD_ERASE_APP		0x04
#define ENS16x_BL_CMD_ERASE_BLINE	0x06
#define ENS16x_BL_CMD_WRITE		0x08
#define ENS16x_BL_CMD_VERIFY		0x0A
#define ENS16x_BL_CMD_GET_BLVER		0x0C
#define ENS16x_BL_CMD_GET_APPVER	0x0E
#define ENS16x_BL_CMD_EXITBL		0x12

#define ENS16x_SEQ_ACK_NOTCOMPLETE	0x80
#define ENS16x_SEQ_ACK_COMPLETE		0xC0

#define IS_ENS16x_SEQ_ACK_NOT_COMPLETE(x) 	(ENS16x_SEQ_ACK_NOTCOMPLETE == (ENS16x_SEQ_ACK_NOTCOMPLETE & (x)))
#define IS_ENS16x_SEQ_ACK_COMPLETE(x) 		(ENS16x_SEQ_ACK_COMPLETE == (ENS16x_SEQ_ACK_COMPLETE & (x)))

#define ENS16x_DATA_STATUS_NEWDAT	0x02
#define ENS16x_DATA_STATUS_NEWGPR	0x01

#define IS_NEWDAT(x) 			(ENS16x_DATA_STATUS_NEWDAT == (ENS16x_DATA_STATUS_NEWDAT & (x)))
#define IS_NEWGPR(x) 			(ENS16x_DATA_STATUS_NEWGPR == (ENS16x_DATA_STATUS_NEWGPR & (x)))
#define IS_NEW_DATA_AVAILABLE(x) 	(0 != ((ENS16x_DATA_STATUS_NEWDAT | ENS16x_DATA_STATUS_NEWGPR ) & (x)))

#define CONVERT_RS_RAW2OHMS_I(x) 	(1 << ((x) >> 11))
#define CONVERT_RS_RAW2OHMS_F(x) 	(pow (2, (float)(x) / 2048))

class ScioSense_ENS16x {
		
	public:
	    ScioSense_ENS16x(uint8_t slaveaddr = ENS16x_I2CADDR_0);               				// Constructor using slave address (5A or 5B)
#ifndef ENS16x_DISABLE_ENHANCED_FEATURES
		ScioSense_ENS16x(uint8_t ADDR, uint8_t nCS, uint8_t nINT);       				// Constructor with pin definition
		ScioSense_ENS16x(uint8_t slaveaddr, uint8_t ADDR, uint8_t nCS, uint8_t nINT);  			// Constructor with slave address and pin definition
#endif

		void 				setI2C(uint8_t sda, uint8_t scl);				// Function to redefine I2C pins
		
		bool 				begin(bool debug=false);					// Init I2C communication, resets ENS16x and checks its PART_ID. Returns false on I2C problems or wrong PART_ID.
		bool				available() 	{ return this->_available; }			// Report availability of sensor
		uint8_t				revENS16x() 	{ return this->_revENS16x; }			// Report version of sensor (0: ENS16x, 1: ENS161)
		bool 				setMode(uint8_t mode);						// Set operation mode of sensor

#ifndef ENS16x_DISABLE_ENHANCED_FEATURES
		bool 				initCustomMode(uint16_t stepNum);				// Initialize definition of custom mode with <n> steps
		bool 				addCustomStep(uint16_t time, bool measureHP0, bool measureHP1, bool measureHP2, bool measureHP3, uint16_t tempHP0, uint16_t tempHP1, uint16_t tempHP2, uint16_t tempHP3);
																							// Add a step to custom measurement profile with definition of duration, enabled data acquisition and temperature for each hotplate
#endif

		bool 				measure(bool waitForNew = true); 				// Perform measurement and stores result in internal variables
#ifndef ENS16x_DISABLE_ENHANCED_FEATURES
		bool 				measureRaw(bool waitForNew = true); 				// Perform raw measurement and stores result in internal variables
		bool 				set_envdata(float t, float h);					// Writes t (degC) and h (%rh) to ENV_DATA. Returns "0" if I2C transmission is successful
		bool 				set_envdata210(uint16_t t, uint16_t h);				// Writes t and h (in ENS210 format) to ENV_DATA. Returns "0" if I2C transmission is successful
#endif
		uint8_t				getMajorRev() 	{ return this->_fw_ver_major; }			// Get major revision number of used firmware
		uint8_t				getMinorRev() 	{ return this->_fw_ver_minor; }			// Get minor revision number of used firmware
		uint8_t				getBuild() 		{ return this->_fw_ver_build; }		// Get build revision number of used firmware

		uint8_t				getAQI() 		{ return this->_data_aqi; }		// Get AQI value of last measurement 
		uint16_t			getTVOC() 		{ return this->_data_tvoc; }		// Get TVOC value of last measurement 
		uint16_t			geteCO2()		{ return this->_data_eco2; }		// Get eCO2 value of last measurement 
		uint16_t			getAQIS() 		{ return this->_data_aqis; }		// Get AQI500 value of last measurement 
		uint32_t			getHP0() 		{ return this->_hp0_rs; }		// Get resistance of HP0 of last measurement
		uint32_t			getHP1() 		{ return this->_hp1_rs; }		// Get resistance of HP1 of last measurement
		uint32_t			getHP2() 		{ return this->_hp2_rs; }		// Get resistance of HP2 of last measurement
		uint32_t			getHP3() 		{ return this->_hp3_rs; }		// Get resistance of HP3 of last measurement
		uint32_t			getHP0BL() 		{ return this->_hp0_bl; }		// Get baseline resistance of HP0 of last measurement
		uint32_t			getHP1BL() 		{ return this->_hp1_bl; }		// Get baseline resistance of HP1 of last measurement
		uint32_t			getHP2BL() 		{ return this->_hp2_bl; }		// Get baseline resistance of HP2 of last measurement
		uint32_t			getHP3BL()		{ return this->_hp3_bl; }		// Get baseline resistance of HP3 of last measurement
		uint8_t				getMISR() 		{ return this->_misr; }			// Return status code of sensor

	private:
		uint8_t				_ADDR; 
		uint8_t				_nINT; 
		uint8_t				_nCS;
		uint8_t				_sdaPin = 0;
		uint8_t				_sclPin = 0;	
				
#ifndef ENS16x_DISABLE_DEBUG
		bool 				debugENS16x = false;
#endif
		
		bool 				reset(); 		                               		// Sends a reset to the ENS16x. Returns false on I2C problems.
		bool 				checkPartID();							// Reads the part ID and confirms valid sensor
		bool 				clearCommand();							// Initialize idle mode and confirms 
		bool				getFirmware();							// Read firmware revisions
		
		bool				_available = false;						// ENS16x available
		uint8_t				_revENS16x = 0;							// ENS160 or ENS161 connected? (FW >7)
	
		uint8_t				_fw_ver_major;
		uint8_t 			_fw_ver_minor;
		uint8_t				_fw_ver_build;

		uint16_t			_stepCount;							// Counter for custom sequence

		uint8_t				_data_aqi;
		uint16_t			_data_tvoc;
		uint16_t			_data_eco2;
		uint16_t			_data_aqis;
		uint32_t			_hp0_rs;
		uint32_t			_hp0_bl;
		uint32_t			_hp1_rs;
		uint32_t			_hp1_bl;
		uint32_t			_hp2_rs;
		uint32_t			_hp2_bl;
		uint32_t			_hp3_rs;
		uint32_t			_hp3_bl;
		uint16_t			_temp;
		int  				_slaveaddr;
		uint8_t				_misr;
		
		//Isotherm, HP0 252°C / HP1 350°C / HP2 250°C / HP3 324°C / measure every 1008ms
		uint8_t _seq_steps[1][8] = {
			{ 0x7C, 0x0A, 0x7E, 0xAF, 0xAF, 0xA2, 0x00, 0x80 },
		};


/****************************************************************************/
/* General functions														*/
/****************************************************************************/
		void 				_i2c_init();
				
		uint8_t				read8(uint8_t addr, byte reg);	
		uint8_t				read(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t num);
		
		uint8_t 			write8(uint8_t addr, byte reg, byte value);
		uint8_t				write(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t num);
		
};


#endif