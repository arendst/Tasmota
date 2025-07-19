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

#include "ScioSense_ENS16x.h"
#include "math.h"

ScioSense_ENS16x::ScioSense_ENS16x(uint8_t slaveaddr) {
	this->_slaveaddr = slaveaddr;
	
	this->_ADDR = 0; 
	this->_nINT = 0; 
	this->_nCS = 0;
}

#ifndef ENS16x_DISABLE_ENHANCED_FEATURES
ScioSense_ENS16x::ScioSense_ENS16x(uint8_t ADDR, uint8_t nCS, uint8_t nINT) {
	this->_slaveaddr = ENS16x_I2CADDR_0;

	this->_ADDR = ADDR; 
	this->_nINT = nINT; 
	this->_nCS = nCS;
}

ScioSense_ENS16x::ScioSense_ENS16x(uint8_t slaveaddr, uint8_t ADDR, uint8_t nCS, uint8_t nINT) {
	this->_slaveaddr = slaveaddr;

	this->_ADDR = ADDR; 
	this->_nINT = nINT; 
	this->_nCS = nCS;
}

// Function to redefine I2C pins
void ScioSense_ENS16x::setI2C(uint8_t sda, uint8_t scl) {
	this->_sdaPin = sda;
	this->_sclPin = scl;	
}	
#endif

// Init I2C communication, resets ENS16x and checks its PART_ID. Returns false on I2C problems or wrong PART_ID.
bool ScioSense_ENS16x::begin(bool debug) 
{
#ifndef ENS16x_DISABLE_DEBUG
	debugENS16x = debug;

	//Set pin levels
	if (this->_ADDR > 0) {
		pinMode(this->_ADDR, OUTPUT);
		digitalWrite(this->_ADDR, LOW);
	}
	if (this->_nINT > 0) pinMode(this->_nINT, INPUT_PULLUP);
	if (this->_nCS > 0) {
		pinMode(this->_nCS, OUTPUT);
		digitalWrite(this->_nCS, HIGH);
	}
#endif

	//init I2C
	_i2c_init();
#ifndef ENS16x_DISABLE_DEBUG
	if (debugENS16x) {
		Serial.println("begin() - I2C init done");
	}
#endif
	delay(ENS16x_BOOTING);                   // Wait to boot after reset
  
	this->_available = false;
	this->_available = this->reset(); 
	
	this->_available = this->checkPartID();

	if (this->_available) {
		this->_available = this->setMode(ENS16x_OPMODE_IDLE);	
		this->_available = this->clearCommand();
		this->_available = this->getFirmware();
	}
#ifndef ENS16x_DISABLE_DEBUG
	if (debugENS16x) {
		Serial.println("ENS16x in idle mode");	
	}
#endif
	return this->_available;
}

// Sends a reset to the ENS16x. Returns false on I2C problems.
bool ScioSense_ENS16x::reset(void) 
{
	uint8_t result = this->write8(_slaveaddr, ENS16x_REG_OPMODE, ENS16x_OPMODE_RESET);

#ifndef ENS16x_DISABLE_DEBUG
	if (debugENS16x) {
		Serial.print("reset() result: ");
		Serial.println(result == 0 ? "ok" : "nok");
	}
#endif
	delay(ENS16x_BOOTING);                   // Wait to boot after reset

	return result == 0;
}

// Reads the part ID and confirms valid sensor
bool ScioSense_ENS16x::checkPartID(void) {
	uint8_t i2cbuf[2];
	uint16_t part_id;
	bool result = false;
	
	this->read(_slaveaddr, ENS16x_REG_PART_ID, i2cbuf, 2);
	part_id = i2cbuf[0] | ((uint16_t)i2cbuf[1] << 8);
	
#ifndef ENS16x_DISABLE_DEBUG
	if (debugENS16x) {
		Serial.print("checkPartID() result: ");
		if (part_id == ENS160_PARTID) Serial.println("ENS160 ok");
		else if (part_id == ENS161_PARTID) Serial.println("ENS161 ok");
		else Serial.println("nok");
	}	
#endif
	delay(ENS16x_BOOTING);                   // Wait to boot after reset

	if (part_id == ENS160_PARTID) { this->_revENS16x = 0; result = true; }
	else if (part_id == ENS161_PARTID) { this->_revENS16x = 1; result = true; }
	
	return result;
}

// Initialize idle mode and confirms 
bool ScioSense_ENS16x::clearCommand(void) {
	uint8_t status;
	uint8_t result;
	
	result = this->write8(_slaveaddr, ENS16x_REG_COMMAND, ENS16x_COMMAND_NOP);
	result = this->write8(_slaveaddr, ENS16x_REG_COMMAND, ENS16x_COMMAND_CLRGPR);
#ifndef ENS16x_DISABLE_DEBUG
	if (debugENS16x) {
		Serial.print("clearCommand() result: ");
		Serial.println(result == 0 ? "ok" : "nok");
	}
#endif
	delay(ENS16x_BOOTING);                   // Wait to boot after reset
	
	status = this->read8(_slaveaddr, ENS16x_REG_DATA_STATUS);
#ifndef ENS16x_DISABLE_DEBUG
 	if (debugENS16x) {
		Serial.print("clearCommand() status: 0x");
		Serial.println(status, HEX);
	}
#endif
	delay(ENS16x_BOOTING);                   // Wait to boot after reset
		
	return result == 0;
}

// Read firmware revisions
bool ScioSense_ENS16x::getFirmware() {
	uint8_t i2cbuf[3];
	uint8_t result;
	
	this->clearCommand();
	
	delay(ENS16x_BOOTING);                   // Wait to boot after reset
	
	result = this->write8(_slaveaddr, ENS16x_REG_COMMAND, ENS16x_COMMAND_GET_APPVER);
	result = this->read(_slaveaddr, ENS16x_REG_GPR_READ_4, i2cbuf, 3);	

	this->_fw_ver_major = i2cbuf[0];
	this->_fw_ver_minor = i2cbuf[1];
	this->_fw_ver_build = i2cbuf[2];
	
	if (this->_fw_ver_major > 6) this->_revENS16x = 1;
	else this->_revENS16x = 0;

#ifndef ENS16x_DISABLE_DEBUG
	if (debugENS16x) {
		Serial.println(this->_fw_ver_major);
		Serial.println(this->_fw_ver_minor);
		Serial.println(this->_fw_ver_build);
		Serial.print("getFirmware() result: ");
		Serial.println(result == 0 ? "ok" : "nok");
	}
#endif
	delay(ENS16x_BOOTING);                   // Wait to boot after reset
	
	return result == 0;
}

// Set operation mode of sensor
bool ScioSense_ENS16x::setMode(uint8_t mode) {
	uint8_t result;
	
	//LP only valid for rev>0
	if ((mode == ENS161_OPMODE_LP) and (_revENS16x == 0)) result = 1;
	else result = this->write8(_slaveaddr, ENS16x_REG_OPMODE, mode);

#ifndef ENS16x_DISABLE_DEBUG
	//if (debugENS16x) {
		Serial.print("setMode() activate result: ");
		Serial.println(result == 0 ? "ok" : "nok");
	//}
#endif
	delay(ENS16x_BOOTING);                   // Wait to boot after reset
	
	return result == 0;
}

#ifndef ENS16x_DISABLE_ENHANCED_FEATURES
// Initialize definition of custom mode with <n> steps
bool ScioSense_ENS16x::initCustomMode(uint16_t stepNum) {
	uint8_t result;
	
	if (stepNum > 0) {
		this->_stepCount = stepNum;
		
		result = this->setMode(ENS16x_OPMODE_IDLE);
		result = this->clearCommand();

		result = this->write8(_slaveaddr, ENS16x_REG_COMMAND, ENS16x_COMMAND_SETSEQ);
	} else {
		result = 1;
	}
	delay(ENS16x_BOOTING);                   // Wait to boot after reset
	
	return result == 0;
}
#endif

#ifndef ENS16x_DISABLE_ENHANCED_FEATURES
// Add a step to custom measurement profile with definition of duration, enabled data acquisition and temperature for each hotplate
bool ScioSense_ENS16x::addCustomStep(uint16_t time, bool measureHP0, bool measureHP1, bool measureHP2, bool measureHP3, uint16_t tempHP0, uint16_t tempHP1, uint16_t tempHP2, uint16_t tempHP3) {
	uint8_t seq_ack;
	uint8_t temp;

#ifndef ENS16x_DISABLE_DEBUG
	if (debugENS16x) {
		Serial.print("setCustomMode() write step ");
		Serial.println(this->_stepCount);
	}
#endif
	delay(ENS16x_BOOTING);                   // Wait to boot after reset

	temp = (uint8_t)(((time / 24)-1) << 6); 
	if (measureHP0) temp = temp | 0x20;
	if (measureHP1) temp = temp | 0x10;
	if (measureHP2) temp = temp | 0x8;
	if (measureHP3) temp = temp | 0x4;
	this->write8(_slaveaddr, ENS16x_REG_GPR_WRITE_0, temp);

	temp = (uint8_t)(((time / 24)-1) >> 2); 
	this->write8(_slaveaddr, ENS16x_REG_GPR_WRITE_1, temp);

	this->write8(_slaveaddr, ENS16x_REG_GPR_WRITE_2, (uint8_t)(tempHP0/2));
	this->write8(_slaveaddr, ENS16x_REG_GPR_WRITE_3, (uint8_t)(tempHP1/2));
	this->write8(_slaveaddr, ENS16x_REG_GPR_WRITE_4, (uint8_t)(tempHP2/2));
	this->write8(_slaveaddr, ENS16x_REG_GPR_WRITE_5, (uint8_t)(tempHP3/2));

	this->write8(_slaveaddr, ENS16x_REG_GPR_WRITE_6, (uint8_t)(this->_stepCount - 1));

    if (this->_stepCount == 1) {
        this->write8(_slaveaddr, ENS16x_REG_GPR_WRITE_7, 128);
    } else {
        this->write8(_slaveaddr, ENS16x_REG_GPR_WRITE_7, 0);
    }
    delay(ENS16x_BOOTING);

	seq_ack = this->read8(_slaveaddr, ENS16x_REG_GPR_READ_7);
	delay(ENS16x_BOOTING);                   // Wait to boot after reset		
	
	if ((ENS16x_SEQ_ACK_COMPLETE | this->_stepCount) != seq_ack) {
		this->_stepCount = this->_stepCount - 1;
		return 0;
	} else {
		return 1;
	}
	
}
#endif

// Perform prediction measurement and stores result in internal variables
bool ScioSense_ENS16x::measure(bool waitForNew) {
	uint8_t i2cbuf[8];
	uint8_t status;
	bool newData = false;

	// Set default status for early bail out
#ifndef ENS16x_DISABLE_DEBUG
	if (debugENS16x) Serial.println("Start measurement");
#endif

	if (waitForNew) {
		do {
			delay(10);
			status = this->read8(_slaveaddr, ENS16x_REG_DATA_STATUS);
			
#ifndef ENS16x_DISABLE_DEBUG
			if (debugENS16x) {
				Serial.print("Status: ");
				Serial.println(status);
			}
#endif			
		} while (!IS_NEWDAT(status));
	} else {
		status = this->read8(_slaveaddr, ENS16x_REG_DATA_STATUS);	
	}
	
	// Read predictions
	if (IS_NEWDAT(status)) {
		newData = true;
		this->read(_slaveaddr, ENS16x_REG_DATA_AQI, i2cbuf, 7);
		_data_aqi = i2cbuf[0];
		_data_tvoc = i2cbuf[1] | ((uint16_t)i2cbuf[2] << 8);
		_data_eco2 = i2cbuf[3] | ((uint16_t)i2cbuf[4] << 8);
		if (_revENS16x > 0) _data_aqis = ((uint16_t)i2cbuf[5]) | ((uint16_t)i2cbuf[6] << 8);
		else _data_aqis = 0;
	}
	
	return newData;
}

#ifndef ENS16x_DISABLE_ENHANCED_FEATURES
// Perfrom raw measurement and stores result in internal variables
bool ScioSense_ENS16x::measureRaw(bool waitForNew) {
	uint8_t i2cbuf[8];
	uint8_t status;
	bool newData = false;

	// Set default status for early bail out
#ifndef ENS16x_DISABLE_DEBUG
	if (debugENS16x) Serial.println("Start measurement");
#endif

	if (waitForNew) {
		do {
			delay(10);
			status = this->read8(_slaveaddr, ENS16x_REG_DATA_STATUS);
			
#ifndef ENS16x_DISABLE_DEBUG
			if (debugENS16x) {
				Serial.print("Status: ");
				Serial.println(status);
			}
#endif
		} while (!IS_NEWGPR(status));
	} else {
		status = this->read8(_slaveaddr, ENS16x_REG_DATA_STATUS);	
	}
	
	if (IS_NEWGPR(status)) {
		newData = true;		
		
		// Read raw resistance values
		this->read(_slaveaddr, ENS16x_REG_GPR_READ_0, i2cbuf, 8);
		_hp0_rs = CONVERT_RS_RAW2OHMS_F((uint32_t)(i2cbuf[0] | ((uint16_t)i2cbuf[1] << 8)));
		_hp1_rs = CONVERT_RS_RAW2OHMS_F((uint32_t)(i2cbuf[2] | ((uint16_t)i2cbuf[3] << 8)));
		_hp2_rs = CONVERT_RS_RAW2OHMS_F((uint32_t)(i2cbuf[4] | ((uint16_t)i2cbuf[5] << 8)));
		_hp3_rs = CONVERT_RS_RAW2OHMS_F((uint32_t)(i2cbuf[6] | ((uint16_t)i2cbuf[7] << 8)));
	
		// Read baselines
		this->read(_slaveaddr, ENS16x_REG_DATA_BL, i2cbuf, 8);
		_hp0_bl = CONVERT_RS_RAW2OHMS_F((uint32_t)(i2cbuf[0] | ((uint16_t)i2cbuf[1] << 8)));
		_hp1_bl = CONVERT_RS_RAW2OHMS_F((uint32_t)(i2cbuf[2] | ((uint16_t)i2cbuf[3] << 8)));
		_hp2_bl = CONVERT_RS_RAW2OHMS_F((uint32_t)(i2cbuf[4] | ((uint16_t)i2cbuf[5] << 8)));
		_hp3_bl = CONVERT_RS_RAW2OHMS_F((uint32_t)(i2cbuf[6] | ((uint16_t)i2cbuf[7] << 8)));

		this->read(_slaveaddr, ENS16x_REG_DATA_MISR, i2cbuf, 1);
		_misr = i2cbuf[0];
	}
	
	return newData;
}

// Writes t (degC) and h (%rh) to ENV_DATA. Returns false on I2C problems.
bool ScioSense_ENS16x::set_envdata(float t, float h) {
	
	uint16_t t_data = (uint16_t)((t + 273.15f) * 64.0f);
	
	uint16_t rh_data = (uint16_t)(h * 512.0f);
	
	return this->set_envdata210(t_data, rh_data);
}

// Writes t and h (in ENS210 format) to ENV_DATA. Returns false on I2C problems.
bool ScioSense_ENS16x::set_envdata210(uint16_t t, uint16_t h) {
	//uint16_t temp;
	uint8_t trh_in[4];
	
	//temp = (uint16_t)((t + 273.15f) * 64.0f);
	trh_in[0] = t & 0xff;
	trh_in[1] = (t >> 8) & 0xff;
	
	//temp = (uint16_t)(h * 512.0f);
	trh_in[2] = h & 0xff;
	trh_in[3] = (h >> 8) & 0xff;
	
	uint8_t result = this->write(_slaveaddr, ENS16x_REG_TEMP_IN, trh_in, 4);
	
	return result;
}
#endif

/**************************************************************************/

void ScioSense_ENS16x::_i2c_init() {
#ifndef ENS16x_DISABLE_ENHANCED_FEATURES
	if (this->_sdaPin != this->_sclPin) 
		Wire.begin(this->_sdaPin, this->_sclPin);
	else 
#endif
		Wire.begin();
}

/**************************************************************************/

uint8_t ScioSense_ENS16x::read8(uint8_t addr, byte reg) {
	uint8_t ret;
	this->read(addr, reg, &ret, 1);
	
	return ret;
}

uint8_t ScioSense_ENS16x::read(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t num) {
	uint8_t pos = 0;
	uint8_t result = 0;
	
#ifndef ENS16x_DISABLE_DEBUG
	if (debugENS16x) {
		Serial.print("I2C read address: 0x");
		Serial.print(addr, HEX);
		Serial.print(" - register: 0x");
		Serial.println(reg, HEX);
	}
#endif
	
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

uint8_t ScioSense_ENS16x::write8(uint8_t addr, byte reg, byte value) {
	uint8_t result = this->write(addr, reg, &value, 1);
	return result;
}

uint8_t ScioSense_ENS16x::write(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t num) {
#ifndef ENS16x_DISABLE_DEBUG
	if (debugENS16x) {
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
#endif
	
	Wire.beginTransmission((uint8_t)addr);
	Wire.write((uint8_t)reg);
	Wire.write((uint8_t *)buf, num);
	uint8_t result = Wire.endTransmission();
	return result;
}

/**************************************************************************/