/*
  ScioSense_ENS160.h - Library for the ENS160 sensor with I2C interface from ScioSense
  2020 Apr 06	v3	Christoph Friese	Changed nomenclature to ScioSense as product shifted from ams
  2020 Feb 15	v2	Giuseppe Pasetti	Corrected firmware flash option
  2019 May 05	v1	Christoph Friese	Created
  based on application note "ENS160 Software Integration.pdf" rev 0.01
*/

#include "ScioSense_ENS160.h"
#include "math.h"

ScioSense_ENS160::ScioSense_ENS160(uint8_t slaveaddr) {
	this->_slaveaddr = slaveaddr;
	
	this->_ADDR = -1; 
	this->_nINT = -1; 
	this->_nCS = -1;
}

ScioSense_ENS160::ScioSense_ENS160(uint8_t ADDR, uint8_t nCS, uint8_t nINT) {
	this->_slaveaddr = ENS160_I2CADDR_0;

	this->_ADDR = ADDR; 
	this->_nINT = nINT; 
	this->_nCS = nCS;
}

ScioSense_ENS160::ScioSense_ENS160(uint8_t slaveaddr, uint8_t ADDR, uint8_t nCS, uint8_t nINT) {
	this->_slaveaddr = slaveaddr;

	this->_ADDR = ADDR; 
	this->_nINT = nINT; 
	this->_nCS = nCS;
}

// Init I2C communication, resets ENS160 and checks its PART_ID. Returns false on I2C problems or wrong PART_ID.
bool ScioSense_ENS160::begin(bool debug, bool bootloader) 
{
	bool result;
	//uint16_t partid;
	uint8_t i2cbuf[2];
  
	debugENS160 = debug;

	//Set pin levels
	if (this->_ADDR > -1) {
		pinMode(this->_ADDR, OUTPUT);
		digitalWrite(this->_ADDR, LOW);
	}
	if (this->_nINT > -1) pinMode(this->_nINT, INPUT_PULLUP);
	if (this->_nCS > -1) {
		pinMode(this->_nCS, OUTPUT);
		digitalWrite(this->_nCS, HIGH);
	}
  
	//init I2C
	_i2c_init();
	if (debugENS160) {
		Serial.println("begin() - I2C init done");
	}
	delay(ENS160_BOOTING);                   // Wait to boot after reset
  
	this->_available = false;
	result = this->reset(); 
	
	this->_available = this->checkPartID();

	if (this->_available) {
		//sciosenseEither select bootloader or idle mode
		if (bootloader) {
			result = this->setMode(ENS160_OPMODE_BOOTLOADER); 
		} else {
			result = this->setMode(ENS160_OPMODE_IDLE);	
		}
		
		result = this->clearCommand();
		result = this->getFirmware();
	}
	if (debugENS160) {
		if (bootloader) {
			Serial.println("ENS160 in bootloader mode"); 
		} else {
			Serial.println("ENS160 in idle mode");	
		}
	}
	return this->_available;
}


// Sends a reset to the ENS160. Returns false on I2C problems.
bool ScioSense_ENS160::reset(void) 
{
	uint8_t result = this->write8(_slaveaddr, ENS160_REG_OPMODE, ENS160_OPMODE_RESET);

	if (debugENS160) {
		Serial.print("reset() result: ");
		Serial.println(result == 0 ? "ok" : "nok");
	}
	delay(ENS160_BOOTING);                   // Wait to boot after reset

	return result == 0;
}

//Reads the part ID and confirms valid sensor
bool ScioSense_ENS160::checkPartID(void) {
	uint8_t i2cbuf[2];
	uint16_t part_id;
	
	uint8_t result = this->read(_slaveaddr, ENS160_REG_PART_ID, i2cbuf, 2);
	part_id = i2cbuf[1] | ((uint16_t)i2cbuf[0] << 8);
	
	if (debugENS160) {
		Serial.print("checkPartID() result: ");
		Serial.println(part_id == ENS160_PARTID ? "ok" : "nok");
	}	
	delay(ENS160_BOOTING);                   // Wait to boot after reset
	
	return part_id == ENS160_PARTID;
}

//Initialize idle mode and confirms 
bool ScioSense_ENS160::clearCommand(void) {
	uint8_t status;
	uint8_t result;
	
	result = this->write8(_slaveaddr, ENS160_REG_COMMAND, ENS160_COMMAND_NOP);
	result = this->write8(_slaveaddr, ENS160_REG_COMMAND, ENS160_COMMAND_CLRGPR);
	if (debugENS160) {
		Serial.print("clearCommand() result: ");
		Serial.println(result == 0 ? "ok" : "nok");
	}
	delay(ENS160_BOOTING);                   // Wait to boot after reset
	
	status = this->read8(_slaveaddr, ENS160_REG_DATA_STATUS);
 	if (debugENS160) {
		Serial.print("clearCommand() status: 0x");
		Serial.println(status, HEX);
	}
	delay(ENS160_BOOTING);                   // Wait to boot after reset
		
	return result == 0;
}

bool ScioSense_ENS160::getFirmware() {
	uint8_t i2cbuf[3];
	uint8_t result;
	
	this->clearCommand();
	
	delay(ENS160_BOOTING);                   // Wait to boot after reset
	
	result = this->write8(_slaveaddr, ENS160_REG_COMMAND, ENS160_COMMAND_GET_APPVER);
	result = this->read(_slaveaddr, ENS160_REG_GPR_READ_4, i2cbuf, 3);	

	this->_fw_ver_major = i2cbuf[0];
	this->_fw_ver_minor = i2cbuf[1];
	this->_fw_ver_build = i2cbuf[2];

	if (debugENS160) {
		Serial.println(this->_fw_ver_major);
		Serial.println(this->_fw_ver_minor);
		Serial.println(this->_fw_ver_build);
		Serial.print("getFirmware() result: ");
		Serial.println(result == 0 ? "ok" : "nok");
	}
	delay(ENS160_BOOTING);                   // Wait to boot after reset
	
	return result == 0;
}

bool ScioSense_ENS160::setMode(uint8_t mode) {
	uint8_t result;
	
	result = this->write8(_slaveaddr, ENS160_REG_OPMODE, mode);

	if (debugENS160) {
		Serial.print("setMode() activate result: ");
		Serial.println(result == 0 ? "ok" : "nok");
	}

	delay(ENS160_BOOTING);                   // Wait to boot after reset
	
	return result == 0;
}


// Define custom mode with number of steps
bool ScioSense_ENS160::initCustomMode(uint16_t stepNum) {
	uint8_t result;
	
	if (stepNum > 0) {
		this->_stepCount = stepNum;
		
		result = this->setMode(ENS160_OPMODE_IDLE);
		result = this->clearCommand();

		result = this->write8(_slaveaddr, ENS160_REG_COMMAND, ENS160_COMMAND_SETSEQ);
	} else {
		result = 1;
	}
	delay(ENS160_BOOTING);                   // Wait to boot after reset
	
	return result == 0;
}

// Add custom mode step with definition of temperatures, duration and measurement 
bool ScioSense_ENS160::addCustomStep(uint16_t time, bool measureHP0, bool measureHP1, bool measureHP2, bool measureHP3, uint16_t tempHP0, uint16_t tempHP1, uint16_t tempHP2, uint16_t tempHP3) {
	uint8_t result;
	uint8_t seq_ack;
	uint8_t temp;

	if (debugENS160) {
		Serial.print("setCustomMode() write step ");
		Serial.println(this->_stepCount);
	}
	delay(ENS160_BOOTING);                   // Wait to boot after reset

	temp = (uint8_t)((time / 24) << 6); 
	if (measureHP0) temp = temp | 0x20;
	if (measureHP1) temp = temp | 0x10;
	if (measureHP2) temp = temp | 0x8;
	if (measureHP3) temp = temp | 0x4;
	result = this->write8(_slaveaddr, ENS160_REG_GPR_WRITE_0, temp);

	temp = (uint8_t)((time / 24) >> 2); 
	result = this->write8(_slaveaddr, ENS160_REG_GPR_WRITE_1, temp);

	result = this->write8(_slaveaddr, ENS160_REG_GPR_WRITE_2, (uint8_t)(tempHP0/2));
	result = this->write8(_slaveaddr, ENS160_REG_GPR_WRITE_3, (uint8_t)(tempHP1/2));
	result = this->write8(_slaveaddr, ENS160_REG_GPR_WRITE_4, (uint8_t)(tempHP2/2));
	result = this->write8(_slaveaddr, ENS160_REG_GPR_WRITE_5, (uint8_t)(tempHP3/2));

	result = this->write8(_slaveaddr, ENS160_REG_GPR_WRITE_6, (uint8_t)(this->_stepCount - 1));

	if (this->_stepCount == 0) {
		result = this->write8(_slaveaddr, ENS160_REG_GPR_WRITE_7, 0);
	} else {
		result = this->write8(_slaveaddr, ENS160_REG_GPR_WRITE_7, 128);
	}
	delay(ENS160_BOOTING);

	seq_ack = this->read8(_slaveaddr, ENS160_REG_GPR_READ_7);
	delay(ENS160_BOOTING);                   // Wait to boot after reset		
	
	if ((ENS160_SEQ_ACK_COMPLETE | this->_stepCount) != seq_ack) {
		this->_stepCount = this->_stepCount - 1;
		return 0;
	} else {
		return 1;
	}
	
}

// Performs one single shot temperature and relative humidity measurement.
bool ScioSense_ENS160::measure(bool waitForNew) 
{
	bool ok;
	uint8_t i2cbuf[8];
	uint8_t status;
	uint8_t result;

	// Set default status for early bail out
	if (debugENS160) Serial.println("Start measurement");
	
	// Either wait that new data is available (might take up to 1sec) or proceed even with old data
	if (waitForNew) {
		do {
			delay(ENS160_BOOTING);
			status = this->read8(_slaveaddr, ENS160_REG_DATA_STATUS);

			if (debugENS160) {
				Serial.print("Status: ");
				Serial.println(status);
			}

		} while (!IS_NEW_DATA_AVAILABLE(status));
	} else {
		status = this->read8(_slaveaddr, ENS160_REG_DATA_STATUS);
	}
	
	// Read predictions
	if (IS_NEWDAT(status)) {
		result = this->read(_slaveaddr, ENS160_REG_DATA_AQI, i2cbuf, 7);
		_data_tvoc = i2cbuf[1] | ((uint16_t)i2cbuf[2] << 8);
		_data_eco2 = i2cbuf[3] | ((uint16_t)i2cbuf[4] << 8);

	}
	
	// Read raw resistance values
	if (IS_NEWGPR(status)) {
		result = this->read(_slaveaddr, ENS160_REG_GPR_READ_0, i2cbuf, 8);
		_hp0_rs = CONVERT_RS_RAW2OHMS_F((uint32_t)(i2cbuf[0] | ((uint16_t)i2cbuf[1] << 8)));
		_hp1_rs = CONVERT_RS_RAW2OHMS_F((uint32_t)(i2cbuf[2] | ((uint16_t)i2cbuf[3] << 8)));
		_hp2_rs = CONVERT_RS_RAW2OHMS_F((uint32_t)(i2cbuf[4] | ((uint16_t)i2cbuf[5] << 8)));
		_hp3_rs = CONVERT_RS_RAW2OHMS_F((uint32_t)(i2cbuf[6] | ((uint16_t)i2cbuf[7] << 8)));
		
	}

	// Read baselines
	if ((IS_NEWGPR(status)) or (IS_NEWDAT(status))) {
		result = this->read(_slaveaddr, ENS160_REG_DATA_BL, i2cbuf, 8);
		_hp0_bl = CONVERT_RS_RAW2OHMS_F((uint32_t)(i2cbuf[0] | ((uint16_t)i2cbuf[1] << 8)));
		_hp1_bl = CONVERT_RS_RAW2OHMS_F((uint32_t)(i2cbuf[2] | ((uint16_t)i2cbuf[3] << 8)));
		_hp2_bl = CONVERT_RS_RAW2OHMS_F((uint32_t)(i2cbuf[4] | ((uint16_t)i2cbuf[5] << 8)));
		_hp3_bl = CONVERT_RS_RAW2OHMS_F((uint32_t)(i2cbuf[6] | ((uint16_t)i2cbuf[7] << 8)));

		result = this->read(_slaveaddr, ENS160_REG_DATA_MISR, i2cbuf, 1);
		_misr = i2cbuf[0];
	}
	
	return ok==0;
}

// Writes t and h (in ENS210 format) to ENV_DATA. Returns false on I2C problems.
bool ScioSense_ENS160::set_envdata210(uint16_t t, uint16_t h) {
	uint16_t trh;
	uint8_t trh_in[4];
	
	trh = (uint16_t)((t + 273.15f) * 64.0f);
	trh_in[0] = trh & 0xff;
	trh_in[1] = (trh >> 8) & 0xff;
	
	trh = (uint16_t)(h * 512.0f);
	trh_in[2] = trh & 0xff;
	trh_in[3] = (trh >> 8) & 0xff;
	
	uint8_t result = this->write(_slaveaddr, ENS160_REG_TEMP_IN, trh_in, 4);
	
	return result;
}

// A helper function to read back and verify status of command
int	ScioSense_ENS160::blCMDWriteVerify(uint8_t command) {
	uint8_t status[2];
	
	uint8_t result = this->write8(_slaveaddr, ENS160_REG_COMMAND, ENS160_COMMAND_NOP);
	Serial.print("1. Write 0x");Serial.print(ENS160_COMMAND_NOP,HEX);
	Serial.print(" with result 0x");Serial.println(result,HEX);
	
	result = this->write8(_slaveaddr, ENS160_REG_COMMAND, command);
	Serial.print("2. Write 0x");Serial.print(command,HEX);
	Serial.print(" with result 0x");Serial.println(result,HEX);
	
	do {
		if (command == ENS160_BL_CMD_WRITE) delay(100);
		else delay(2000);
		result = this->read(_slaveaddr, ENS160_REG_DATA_STATUS, status, 1);
		Serial.print("3. Read DATA_STATUS with 0x");
		Serial.print(status[0],HEX);
		Serial.print("\t0x");
		Serial.println(status[0] & ENS160_DATA_STATUS_NEWGPR);
	} while (!(status[0] & ENS160_DATA_STATUS_NEWGPR));
	
	result = this->read(_slaveaddr, ENS160_REG_GPR_READ_6, status, 2);
	Serial.print("4. Read GPR_READ_6 with 0x");
	Serial.print(status[0],HEX);
	Serial.print("\t0x");
	Serial.println(status[1],HEX);
	
	if (status[1] != command + 1) {
		// status[0] is the MSB 8bit of AMS_ERR_BL_XXX error code
		// documented in the ‘ENS160 Bootloader User Guide’.
		return status[0];
	}
	return 0;
}

// Flashes the firmware of the ENS160 with size bytes from image - image _must_ be in PROGMEM
/*bool ScioSense_ENS160::flash(const uint8_t * app_img, int size) {
	int error = 0;
	int idx = 0;	
	uint8_t result = this->write8(_slaveaddr,ENS160_REG_OPMODE,ENS160_OPMODE_BOOTLOADER);
	delay(ENS160_BOOTING);                   // Wait to boot after reset
	// Write magic word and start bootloader
	result = this->write(_slaveaddr, ENS160_REG_GPR_WRITE_0, ENS160_BL_MAGIC, 4);
	error = blCMDWriteVerify(ENS160_BL_CMD_START);
	if (error) {
	  Serial.print("Write failed ENS160_BL_CMD_START. Error code: ");
	  Serial.println(error);
	  return false;
	} else {
	  Serial.print("Write success ENS160_BL_CMD_START.");
	}
	
	// Erase stored baseline
	error = blCMDWriteVerify(ENS160_BL_CMD_ERASE_BLINE);
	if (error) {
	  Serial.print("Write failed ENS160_BL_CMD_ERASE_BLINE. Error code: ");
	  Serial.println(error);
	  return false;
	} else {
	  Serial.print("Write success ENS160_BL_CMD_ERASE_BLINE.");
	}
	
	// Erase application FLASH memory
	error = blCMDWriteVerify(ENS160_BL_CMD_ERASE_APP);
	if (error) {
	  Serial.print("Write failed ENS160_BL_CMD_ERASE_APP. Error code: ");
	  Serial.println(error);
	  return false;
	} else {
	  Serial.print("Write success ENS160_BL_CMD_ERASE_APP.");
	}
	
	// Write application binary in pieces of 8 bytes
	uint8_t len = 0;
	for(idx = 0; idx < size; idx+=8) {
		int len = (size - idx) < 8 ? (size - idx) : 8;
        Serial.println(size);
		Serial.println(len);
		Serial.println(idx);
        uint8_t ram[8];
        memcpy_P(ram, app_img, len);	// Copy up to 8 bytes from PROGMEM to RAM
		
		result = this->write(_slaveaddr, ENS160_REG_GPR_WRITE_0, ram, len);

		error = blCMDWriteVerify(ENS160_BL_CMD_WRITE);
		if (error) {
			Serial.print("Write failed ENS160_BL_CMD_WRITE. Error code: ");
			Serial.println(error);
			return false;
		}
		app_img += len;
	} 

	// idx should match sizeof(app_img);
	if(idx != size) {
		Serial.println("Failed to complete application image update.");
		return false;
	} else {
	  Serial.println("Write success of complete application image.");
	}
	
	// Verify application FLASH memory
	error = blCMDWriteVerify(ENS160_BL_CMD_VERIFY);
	if (error) {
	  Serial.print("Write failed ENS160_BL_CMD_VERIFY. Error code: ");
	  Serial.println(error);
	  return false;
	} else {
	  Serial.print("Write success ENS160_BL_CMD_VERIFY.");
	}
	
	// Verify application FLASH memory
	error = blCMDWriteVerify(ENS160_BL_CMD_EXITBL);
	if (error) {
	  Serial.print("Write failed ENS160_BL_CMD_EXITBL. Error code: ");
	  Serial.println(error);
	  return false;
	} else {
	  Serial.print("Write success ENS160_BL_CMD_EXITBL.");
	}	
	
	this->reset();

	result = this->clearCommand();
		
	result = this->getFirmware();
	
	return true;

}

*/
/****************************************************************************/
/*	General functions														*/
/****************************************************************************/

void ScioSense_ENS160::_i2c_init()
{
	Wire.begin();
}

/**************************************************************************/

uint8_t ScioSense_ENS160::read8(uint8_t addr, byte reg)
{
	uint8_t ret;
	this->read(addr, reg, &ret, 1);
	
	return ret;
}

uint8_t ScioSense_ENS160::read(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t num)
{
	uint8_t pos = 0;
	uint8_t result = 0;
	
	if (debugENS160) {
		Serial.print("I2C read address: 0x");
		Serial.print(addr, HEX);
		Serial.print(" - register: 0x");
		Serial.println(reg, HEX);
	}
	
	//on arduino we need to read in 32 byte chunks
	while(pos < num){
		
		uint8_t read_now = min((uint8_t)32, (uint8_t)(num - pos));
		Wire.beginTransmission((uint8_t)addr);
		
		Wire.write((uint8_t)reg + pos);
		result = Wire.endTransmission();
		Wire.requestFrom((uint8_t)addr, read_now);
		
		for(int i=0; i<read_now; i++){
			buf[pos] = Wire.read();
			pos++;
		}
	}
	return result;
}

/**************************************************************************/

uint8_t ScioSense_ENS160::write8(uint8_t addr, byte reg, byte value)
{
	uint8_t result = this->write(addr, reg, &value, 1);
	return result;
}

uint8_t ScioSense_ENS160::write(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t num)
{
	if (debugENS160) {
		Serial.print("I2C write address: 0x");
		Serial.print(addr, HEX);
		Serial.print(" - register: 0x");
		Serial.print(reg, HEX);
		Serial.print(" -  value:");
		for (int i = 0; i<num; i++) {
			Serial.print(" 0x");
			Serial.print(buf[i], HEX);
		}
		Serial.println();
	}
	
	Wire.beginTransmission((uint8_t)addr);
	Wire.write((uint8_t)reg);
	Wire.write((uint8_t *)buf, num);
	uint8_t result = Wire.endTransmission();
	return result;
}

/**************************************************************************/



