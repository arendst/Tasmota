/*
  ScioSense_ENS160.h - Library for the ENS160 sensor with I2C interface from ScioSense
  2020 Apr 06	v2	Christoph Friese	Changed nomenclature to ScioSense as product shifted from ams
  2020 Feb 15	v2	Giuseppe Pasetti	Corrected firmware flash option
  2019 May 05	v1	Christoph Friese	Created
  based on application note "ENS160 Software Integration.pdf" rev 0.01
*/

#ifndef __SCIOSENSE_ENS160_H_
#define __SCIOSENSE_ENS160_H_

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h>

// Chip constants
#define ENS160_PARTID			0x6001
#define ENS160_BOOTING			50

// 7-bit I2C slave address of the ENS160
#define ENS160_I2CADDR_0          	0x52		//ADDR low
#define ENS160_I2CADDR_1          	0x53		//ADDR high

// ENS160 registers for version V0
#define ENS160_REG_PART_ID          	0x00 		// 2 byte register
#define ENS160_REG_OPMODE		0x10
#define ENS160_REG_CONFIG		0x11
#define ENS160_REG_COMMAND		0x12
#define ENS160_REG_TEMP_IN		0x13
#define ENS160_REG_RH_IN		0x15
#define ENS160_REG_DATA_STATUS		0x20
#define ENS160_REG_DATA_AQI		0x21
#define ENS160_REG_DATA_TVOC		0x22
#define ENS160_REG_DATA_ECO2		0x24			
#define ENS160_REG_DATA_BL		0x28
#define ENS160_REG_DATA_T		0x30
#define ENS160_REG_DATA_RH		0x32
#define ENS160_REG_DATA_MISR		0x38
#define ENS160_REG_GPR_WRITE_0		0x40
#define ENS160_REG_GPR_WRITE_1		ENS160_REG_GPR_WRITE_0 + 1
#define ENS160_REG_GPR_WRITE_2		ENS160_REG_GPR_WRITE_0 + 2
#define ENS160_REG_GPR_WRITE_3		ENS160_REG_GPR_WRITE_0 + 3
#define ENS160_REG_GPR_WRITE_4		ENS160_REG_GPR_WRITE_0 + 4
#define ENS160_REG_GPR_WRITE_5		ENS160_REG_GPR_WRITE_0 + 5
#define ENS160_REG_GPR_WRITE_6		ENS160_REG_GPR_WRITE_0 + 6
#define ENS160_REG_GPR_WRITE_7		ENS160_REG_GPR_WRITE_0 + 7
#define ENS160_REG_GPR_READ_0		0x48
#define ENS160_REG_GPR_READ_4		ENS160_REG_GPR_READ_0 + 4
#define ENS160_REG_GPR_READ_6		ENS160_REG_GPR_READ_0 + 6
#define ENS160_REG_GPR_READ_7		ENS160_REG_GPR_READ_0 + 7

//ENS160 data register fields
#define ENS160_COMMAND_NOP		0x00
#define ENS160_COMMAND_CLRGPR		0xCC
#define ENS160_COMMAND_GET_APPVER	0x0E 
#define ENS160_COMMAND_SETTH		0x02
#define ENS160_COMMAND_SETSEQ		0xC2

#define ENS160_OPMODE_RESET		0xF0
#define ENS160_OPMODE_DEP_SLEEP		0x00
#define ENS160_OPMODE_IDLE		0x01
#define ENS160_OPMODE_STD		0x02
#define ENS160_OPMODE_INTERMEDIATE	0x03	
#define ENS160_OPMODE_CUSTOM		0xC0
#define ENS160_OPMODE_D0		0xD0
#define ENS160_OPMODE_D1		0xD1
#define ENS160_OPMODE_BOOTLOADER	0xB0

#define ENS160_BL_CMD_START		0x02
#define ENS160_BL_CMD_ERASE_APP		0x04
#define ENS160_BL_CMD_ERASE_BLINE	0x06
#define ENS160_BL_CMD_WRITE		0x08
#define ENS160_BL_CMD_VERIFY		0x0A
#define ENS160_BL_CMD_GET_BLVER		0x0C
#define ENS160_BL_CMD_GET_APPVER	0x0E
#define ENS160_BL_CMD_EXITBL		0x12

#define ENS160_SEQ_ACK_NOTCOMPLETE	0x80
#define ENS160_SEQ_ACK_COMPLETE		0xC0

#define IS_ENS160_SEQ_ACK_NOT_COMPLETE(x) 	(ENS160_SEQ_ACK_NOTCOMPLETE == (ENS160_SEQ_ACK_NOTCOMPLETE & (x)))
#define IS_ENS160_SEQ_ACK_COMPLETE(x) 		(ENS160_SEQ_ACK_COMPLETE == (ENS160_SEQ_ACK_COMPLETE & (x)))

#define ENS160_DATA_STATUS_NEWDAT	0x02
#define ENS160_DATA_STATUS_NEWGPR	0x01

#define IS_NEWDAT(x) 			(ENS160_DATA_STATUS_NEWDAT == (ENS160_DATA_STATUS_NEWDAT & (x)))
#define IS_NEWGPR(x) 			(ENS160_DATA_STATUS_NEWGPR == (ENS160_DATA_STATUS_NEWGPR & (x)))
#define IS_NEW_DATA_AVAILABLE(x) 	(0 != ((ENS160_DATA_STATUS_NEWDAT | ENS160_DATA_STATUS_NEWGPR ) & (x)))

#define CONVERT_RS_RAW2OHMS_I(x) 	(1 << ((x) >> 11))
#define CONVERT_RS_RAW2OHMS_F(x) 	(pow (2, (float)(x) / 2048))

static uint8_t ENS160_BL_MAGIC[4] = {0x53, 0xCE, 0x1A, 0xBF};

class ScioSense_ENS160 {
		
	public:
	    ScioSense_ENS160(uint8_t slaveaddr = ENS160_I2CADDR_0);               			// Constructor using slave address (5A or 5B)
		ScioSense_ENS160(uint8_t ADDR, uint8_t nCS, uint8_t nINT);       					// Constructor with pin definition
		ScioSense_ENS160(uint8_t slaveaddr, uint8_t ADDR, uint8_t nCS, uint8_t nINT);     // Constructor with slave address and pin definition
		
		bool 				begin(bool debug=false, bool bootloader=false);								// init i2c interface, get partID und uID. Returns false on I2C problems or wrong PART_ID.
		bool				available() { return this->_available; }

		bool 				setMode(uint8_t mode);

		bool 				initCustomMode(uint16_t stepNum);
		bool 				addCustomStep(uint16_t time, bool measureHP0, bool measureHP1, bool measureHP2, bool measureHP3, uint16_t tempHP0, uint16_t tempHP1, uint16_t tempHP2, uint16_t tempHP3);


		bool 				measure(bool waitForNew = true); 												// perfrom measurement and stores result in internal variables
		bool 				set_envdata210(uint16_t t, uint16_t h);					// Writes t and h (in ENS210 format) to ENV_DATA. Returns false on I2C problems.
		uint8_t				getMajorRev() {return this->_fw_ver_major; }
		uint8_t				getMinorRev() {return this->_fw_ver_minor; }
		uint8_t				getBuild() {return this->_fw_ver_build; }

		uint8_t				getAQI() {return this->_data_aqi; }
		uint16_t			getTVOC() {return this->_data_tvoc; }
		uint16_t			geteCO2() {return this->_data_eco2; }
		uint32_t			getHP0() {return this->_hp0_rs; }
		uint32_t			getHP1() {return this->_hp1_rs; }
		uint32_t			getHP2() {return this->_hp2_rs; }
		uint32_t			getHP3() {return this->_hp3_rs; }
		uint32_t			getHP0BL() {return this->_hp0_bl; }
		uint32_t			getHP1BL() {return this->_hp1_bl; }
		uint32_t			getHP2BL() {return this->_hp2_bl; }
		uint32_t			getHP3BL() {return this->_hp3_bl; }
		uint8_t				getMISR() {return this->_misr; }
		
//		bool 				flash(const uint8_t * app_img, int size);                              // Flashes the firmware of the CCS811 with size bytes from image - image _must_ be in PROGMEM

	private:
		uint8_t				_ADDR; 
		uint8_t				_nINT; 
		uint8_t				_nCS;
		bool 				debugENS160 = false;
		
		bool 				reset(void);                                // Sends a reset to the ENS160. Returns false on I2C problems.
		bool 				checkPartID();
		bool 				clearCommand(void);
		bool				getFirmware();
		
		int				blCMDWriteVerify(uint8_t command);
		
		
		bool				_available = false;							// ENS160 available

		uint8_t				_fw_ver_major;
		uint8_t 			_fw_ver_minor;
		uint8_t				_fw_ver_build;

		uint16_t			_stepCount;									// Counter for custom sequence

		uint8_t				_data_aqi;
		uint16_t			_data_tvoc;
		uint16_t			_data_eco2;
		uint32_t			_hp0_rs;
		uint32_t			_hp0_bl;
		uint32_t			_hp1_rs;
		uint32_t			_hp1_bl;
		uint32_t			_hp2_rs;
		uint32_t			_hp2_bl;
		uint32_t			_hp3_rs;
		uint32_t			_hp3_bl;
		uint16_t			_temp;
		int  				_slaveaddr;										// Slave address of the ENS160
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
