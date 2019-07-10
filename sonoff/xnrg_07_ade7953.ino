/*
  xnrg_07_ade7953.ino - ADE7953 energy sensor support for Sonoff-Tasmota

  Copyright (C) 2019  Theo Arends

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

#define ADE7953_PREF            1540
#define ADE7953_UREF            26000
#define ADE7953_IREF            10000

#define ADE7953_ADDR            0x38

uint32_t ade7953_active_power = 0;
uint32_t ade7953_active_power1 = 0;
uint32_t ade7953_active_power2 = 0;
uint32_t ade7953_current_rms = 0;
uint32_t ade7953_current_rms1 = 0;
uint32_t ade7953_current_rms2 = 0;
uint32_t ade7953_voltage_rms = 0;
uint8_t ade7953_init = 0;

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

uint32_t Ade7953Read(uint16_t reg)
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
  int32_t active_power;

  ade7953_voltage_rms = Ade7953Read(0x31C);      // Both relays
  ade7953_current_rms1 = Ade7953Read(0x31B);     // Relay 1
  if (ade7953_current_rms1 < 2000) {             // No load threshold (20mA)
    ade7953_current_rms1 = 0;
    ade7953_active_power1 = 0;
  } else {
    active_power = (int32_t)Ade7953Read(0x313) * -1;  // Relay 1
    ade7953_active_power1 = (active_power > 0) ? active_power : 0;
  }
  ade7953_current_rms2 = Ade7953Read(0x31A);     // Relay 2
  if (ade7953_current_rms2 < 2000) {             // No load threshold (20mA)
    ade7953_current_rms2 = 0;
    ade7953_active_power2 = 0;
  } else {
    active_power = (int32_t)Ade7953Read(0x312);  // Relay 2
    ade7953_active_power2 = (active_power > 0) ? active_power : 0;
  }
  // First phase only supports accumulated Current and Power
  ade7953_current_rms = ade7953_current_rms1 + ade7953_current_rms2;
  ade7953_active_power = ade7953_active_power1 + ade7953_active_power2;

  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ADE: U %d, I %d + %d = %d, P %d + %d = %d"),
    ade7953_voltage_rms, ade7953_current_rms1, ade7953_current_rms2, ade7953_current_rms, ade7953_active_power1, ade7953_active_power2, ade7953_active_power);

  if (energy_power_on) {  // Powered on
    energy_voltage = (float)ade7953_voltage_rms / Settings.energy_voltage_calibration;
    energy_active_power = (float)ade7953_active_power / (Settings.energy_power_calibration / 10);
    if (0 == energy_active_power) {
      energy_current = 0;
    } else {
      energy_current = (float)ade7953_current_rms / (Settings.energy_current_calibration * 10);
    }
  } else {  // Powered off
    energy_voltage = 0;
    energy_active_power = 0;
    energy_current = 0;
  }

  if (ade7953_active_power) {
    energy_kWhtoday_delta += ((ade7953_active_power * (100000 / (Settings.energy_power_calibration / 10))) / 3600);
    EnergyUpdateToday();
  }
}

void Ade7953EnergyEverySecond()
{
	if (ade7953_init) {
    if (1 == ade7953_init) {
      Ade7953Init();
	  }
    ade7953_init--;
	}	else {
		Ade7953GetData();
	}
}

void Ade7953DrvInit(void)
{
  if (!energy_flg) {
    if (i2c_flg && (pin[GPIO_ADE7953_IRQ] < 99)) {  // Irq on GPIO16 is not supported...
			delay(100);                                   // Need 100mS to init ADE7953
      if (I2cDevice(ADE7953_ADDR)) {
        if (HLW_PREF_PULSE == Settings.energy_power_calibration) {
          Settings.energy_power_calibration = ADE7953_PREF;
          Settings.energy_voltage_calibration = ADE7953_UREF;
          Settings.energy_current_calibration = ADE7953_IREF;
        }
        AddLog_P2(LOG_LEVEL_DEBUG, S_LOG_I2C_FOUND_AT, "ADE7953", ADE7953_ADDR);
				ade7953_init = 2;
        energy_flg = XNRG_07;
			}
    }
  }
}

bool Ade7953Command(void)
{
  bool serviced = true;

  uint32_t value = (uint32_t)(CharToFloat(XdrvMailbox.data) * 100);  // 1.23 = 123

  if (CMND_POWERCAL == energy_command_code) {
    if (1 == XdrvMailbox.payload) { XdrvMailbox.payload = ADE7953_PREF; }
    // Service in xdrv_03_energy.ino
  }
  else if (CMND_VOLTAGECAL == energy_command_code) {
    if (1 == XdrvMailbox.payload) { XdrvMailbox.payload = ADE7953_UREF; }
    // Service in xdrv_03_energy.ino
  }
  else if (CMND_CURRENTCAL == energy_command_code) {
    if (1 == XdrvMailbox.payload) { XdrvMailbox.payload = ADE7953_IREF; }
    // Service in xdrv_03_energy.ino
  }
  else if (CMND_POWERSET == energy_command_code) {
    if (XdrvMailbox.data_len && ade7953_active_power) {
      if ((value > 100) && (value < 200000)) {  // Between 1W and 2000W
        Settings.energy_power_calibration = (ade7953_active_power * 1000) / value;  // 0.00 W
      }
    }
  }
  else if (CMND_VOLTAGESET == energy_command_code) {
    if (XdrvMailbox.data_len && ade7953_voltage_rms) {
      if ((value > 10000) && (value < 26000)) {  // Between 100V and 260V
        Settings.energy_voltage_calibration = (ade7953_voltage_rms * 100) / value;  // 0.00 V
      }
    }
  }
  else if (CMND_CURRENTSET == energy_command_code) {
    if (XdrvMailbox.data_len && ade7953_current_rms) {
      if ((value > 2000) && (value < 1000000)) {  // Between 20mA and 10A
        Settings.energy_current_calibration = ((ade7953_current_rms * 100) / value) * 100;  // 0.00 mA
      }
    }
  }
  else serviced = false;  // Unknown command

  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

int Xnrg07(uint8_t function)
{
  int result = 0;

  if (FUNC_PRE_INIT == function) {
    Ade7953DrvInit();
  }
  else if (XNRG_07 == energy_flg) {
    switch (function) {
      case FUNC_ENERGY_EVERY_SECOND:
        Ade7953EnergyEverySecond();
        break;
      case FUNC_COMMAND:
        result = Ade7953Command();
        break;
		}
  }
  return result;
}

#endif  // USE_ADE7953
#endif  // USE_ENERGY_SENSOR
#endif  // USE_I2C
