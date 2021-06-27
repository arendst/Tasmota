/*
  xnrg_07_ade7953.ino - ADE7953 energy sensor support for Tasmota

  Copyright (C) 2021  Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef USE_I2C
#ifdef USE_ENERGY_SENSOR
#ifdef USE_ADE7953
/*********************************************************************************************\
 * ADE7953 - Energy (Shelly 2.5)
 *
 * Based on datasheet from https://www.analog.com/en/products/ade7953.html
 *
 * I2C Address: 0x38
\*********************************************************************************************/

#define XNRG_07                 7
#define XI2C_07                 7  // See I2CDEVICES.md

#define ADE7953_PREF            1540
#define ADE7953_UREF            26000
#define ADE7953_IREF            10000

#define ADE7953_ADDR            0x38

const uint16_t Ade7953Registers[] {
  0x31B,  // RMS current channel B (Relay 1)
  0x313,  // Active power channel B
  0x311,  // Apparent power channel B
  0x315,  // Reactive power channel B
  0x31A,  // RMS current channel A (Relay 2)
  0x312,  // Active power channel A
  0x310,  // Apparent power channel A
  0x314,  // Reactive power channel A
  0x31C,  // RMS voltage (Both relays)
  0x10E   // 16-bit unsigned period register
};

struct Ade7953 {
  uint32_t voltage_rms = 0;
  uint32_t period = 0;
  uint32_t current_rms[2] = { 0, 0 };
  uint32_t active_power[2] = { 0, 0 };
  uint8_t init_step = 0;
} Ade7953;

int Ade7953RegSize(uint16_t reg)
{
  int size = 0;
  switch ((reg >> 8) & 0x0F) {
    case 0x03:
      size++;
    case 0x02:
      size++;
    case 0x01:
      size++;
    case 0x00:
    case 0x07:
    case 0x08:
      size++;
  }
  return size;
}

void Ade7953Write(uint16_t reg, uint32_t val)
{
  int size = Ade7953RegSize(reg);
  if (size) {
    Wire.beginTransmission(ADE7953_ADDR);
    Wire.write((reg >> 8) & 0xFF);
    Wire.write(reg & 0xFF);
    while (size--) {
      Wire.write((val >> (8 * size)) & 0xFF);  // Write data, MSB first
    }
    Wire.endTransmission();
    delayMicroseconds(5);    // Bus-free time minimum 4.7us
  }
}

int32_t Ade7953Read(uint16_t reg)
{
	uint32_t response = 0;

  int size = Ade7953RegSize(reg);
  if (size) {
    Wire.beginTransmission(ADE7953_ADDR);
    Wire.write((reg >> 8) & 0xFF);
    Wire.write(reg & 0xFF);
    Wire.endTransmission(0);
    Wire.requestFrom(ADE7953_ADDR, size);
    if (size <= Wire.available()) {
      for (uint32_t i = 0; i < size; i++) {
        response = response << 8 | Wire.read();   // receive DATA (MSB first)
      }
    }
  }
	return response;
}

void Ade7953Init(void)
{
  Ade7953Write(0x102, 0x0004);    // Locking the communication interface (Clear bit COMM_LOCK), Enable HPF
  Ade7953Write(0x0FE, 0x00AD);    // Unlock register 0x120
  Ade7953Write(0x120, 0x0030);    // Configure optimum setting
}

void Ade7953GetData(void)
{
  int32_t reg[2][4];
  for (uint32_t i = 0; i < sizeof(Ade7953Registers)/sizeof(uint16_t); i++) {
    int32_t value = Ade7953Read(Ade7953Registers[i]);
    if (8 == i) {
      Ade7953.voltage_rms = value;  // RMS voltage (Both relays)
    } else if (9 == i) {
      Ade7953.period = value;  // period
    } else {
      reg[i >> 2][i &3] = value;
    }
  }
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("ADE: %d, %d, [%d, %d, %d, %d], [%d, %d, %d, %d]"),
    Ade7953.voltage_rms, Ade7953.period,
    reg[0][0], reg[0][1], reg[0][2], reg[0][3],
    reg[1][0], reg[1][1], reg[1][2], reg[1][3]);

  uint32_t apparent_power[2] = { 0, 0 };
  uint32_t reactive_power[2] = { 0, 0 };

  for (uint32_t channel = 0; channel < 2; channel++) {
    Ade7953.current_rms[channel] = reg[channel][0];
    if (Ade7953.current_rms[channel] < 2000) {           // No load threshold (20mA)
      Ade7953.current_rms[channel] = 0;
      Ade7953.active_power[channel] = 0;
    } else {
      Ade7953.active_power[channel] = abs(reg[channel][1]);
      apparent_power[channel] = abs(reg[channel][2]);
      reactive_power[channel] = abs(reg[channel][3]);
    }
  }

  uint32_t current_rms_sum = Ade7953.current_rms[0] + Ade7953.current_rms[1];
  uint32_t active_power_sum = Ade7953.active_power[0] + Ade7953.active_power[1];

  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("ADE: U %d, C %d, I %d + %d = %d, P %d + %d = %d"),
    Ade7953.voltage_rms, Ade7953.period,
    Ade7953.current_rms[0], Ade7953.current_rms[1], current_rms_sum,
    Ade7953.active_power[0], Ade7953.active_power[1], active_power_sum);

  if (Energy.power_on) {  // Powered on
    Energy.voltage[0] = (float)Ade7953.voltage_rms / Settings->energy_voltage_calibration;
    Energy.frequency[0] = 223750.0f / ( (float)Ade7953.period + 1);

    for (uint32_t channel = 0; channel < 2; channel++) {
      Energy.data_valid[channel] = 0;
      Energy.active_power[channel] = (float)Ade7953.active_power[channel] / (Settings->energy_power_calibration / 10);
      Energy.reactive_power[channel] = (float)reactive_power[channel] / (Settings->energy_power_calibration / 10);
      Energy.apparent_power[channel] = (float)apparent_power[channel] / (Settings->energy_power_calibration / 10);
      if (0 == Energy.active_power[channel]) {
        Energy.current[channel] = 0;
      } else {
        Energy.current[channel] = (float)Ade7953.current_rms[channel] / (Settings->energy_current_calibration * 10);
      }
    }
/*
  } else {  // Powered off
    Energy.data_valid[0] = ENERGY_WATCHDOG;
    Energy.data_valid[1] = ENERGY_WATCHDOG;
*/
  }

  if (active_power_sum) {
    Energy.kWhtoday_delta += ((active_power_sum * (100000 / (Settings->energy_power_calibration / 10))) / 3600);
    EnergyUpdateToday();
  }
}

void Ade7953EnergyEverySecond(void)
{
	if (Ade7953.init_step) {
    if (1 == Ade7953.init_step) {
      Ade7953Init();
	  }
    Ade7953.init_step--;
	}	else {
		Ade7953GetData();
	}
}

void Ade7953DrvInit(void)
{
  if (PinUsed(GPIO_ADE7953_IRQ)) {                // Irq on GPIO16 is not supported...
    pinMode(Pin(GPIO_ADE7953_IRQ), INPUT);        // Related to resetPins() - Must be set to input
    delay(100);                                   // Need 100mS to init ADE7953
    if (I2cSetDevice(ADE7953_ADDR)) {
      if (HLW_PREF_PULSE == Settings->energy_power_calibration) {
        Settings->energy_power_calibration = ADE7953_PREF;
        Settings->energy_voltage_calibration = ADE7953_UREF;
        Settings->energy_current_calibration = ADE7953_IREF;
      }
      I2cSetActiveFound(ADE7953_ADDR, "ADE7953");
      Ade7953.init_step = 2;
      Energy.phase_count = 2;                     // Handle two channels as two phases
      Energy.voltage_common = true;               // Use common voltage
      Energy.frequency_common = true;             // Use common frequency
      Energy.use_overtemp = true;                 // Use global temperature for overtemp detection
      TasmotaGlobal.energy_driver = XNRG_07;
    }
  }
}

bool Ade7953Command(void)
{
  bool serviced = true;

  uint32_t channel = (2 == XdrvMailbox.index) ? 1 : 0;
  uint32_t value = (uint32_t)(CharToFloat(XdrvMailbox.data) * 100);  // 1.23 = 123

  if (CMND_POWERCAL == Energy.command_code) {
    if (1 == XdrvMailbox.payload) { XdrvMailbox.payload = ADE7953_PREF; }
    // Service in xdrv_03_energy.ino
  }
  else if (CMND_VOLTAGECAL == Energy.command_code) {
    if (1 == XdrvMailbox.payload) { XdrvMailbox.payload = ADE7953_UREF; }
    // Service in xdrv_03_energy.ino
  }
  else if (CMND_CURRENTCAL == Energy.command_code) {
    if (1 == XdrvMailbox.payload) { XdrvMailbox.payload = ADE7953_IREF; }
    // Service in xdrv_03_energy.ino
  }
  else if (CMND_POWERSET == Energy.command_code) {
    if (XdrvMailbox.data_len && Ade7953.active_power[channel]) {
      if ((value > 100) && (value < 200000)) {  // Between 1W and 2000W
        Settings->energy_power_calibration = (Ade7953.active_power[channel] * 1000) / value;  // 0.00 W
      }
    }
  }
  else if (CMND_VOLTAGESET == Energy.command_code) {
    if (XdrvMailbox.data_len && Ade7953.voltage_rms) {
      if ((value > 10000) && (value < 26000)) {  // Between 100V and 260V
        Settings->energy_voltage_calibration = (Ade7953.voltage_rms * 100) / value;  // 0.00 V
      }
    }
  }
  else if (CMND_CURRENTSET == Energy.command_code) {
    if (XdrvMailbox.data_len && Ade7953.current_rms[channel]) {
      if ((value > 2000) && (value < 1000000)) {  // Between 20mA and 10A
        Settings->energy_current_calibration = ((Ade7953.current_rms[channel] * 100) / value) * 100;  // 0.00 mA
      }
    }
  }
  else serviced = false;  // Unknown command

  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg07(uint8_t function)
{
  if (!I2cEnabled(XI2C_07)) { return false; }

  bool result = false;

  switch (function) {
    case FUNC_ENERGY_EVERY_SECOND:
      Ade7953EnergyEverySecond();
      break;
    case FUNC_COMMAND:
      result = Ade7953Command();
      break;
    case FUNC_PRE_INIT:
      Ade7953DrvInit();
      break;
  }
  return result;
}

#endif  // USE_ADE7953
#endif  // USE_ENERGY_SENSOR
#endif  // USE_I2C
