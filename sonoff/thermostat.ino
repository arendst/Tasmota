/*
  thermostat.ino - Sonoff TH Standalone Thermostat Module support for Sonoff-Tasmota

  Copyright (C) 2017  Theo Arends

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

/*********************************************************************************************\
 * Sonoff TH Standalone Thermostat Module support for Sonoff-Tasmota
 * 
 * Based on Hard Thermostat by nambuco / Paulo H F Alves
 *
 * Extended functionality with web interface heating/cooling select by IceStuff / Paul Butland
\*********************************************************************************************/

#define TEMPERATURECHECK 10    // Only check temperature this many seconds to reduce load

enum ThermoCommands {
    CMND_TEMP_CONTROL, CMND_TEMP_SETPOINT, CMND_TEMP_HYSTERESIS, CMND_TEMP_OFFTIMEMIN };
const char kThermoCommands[] PROGMEM =
    D_CMND_TEMP_CONTROL "|" D_CMND_TEMP_SETPOINT "|" D_CMND_TEMP_HYSTERESIS "|" D_CMND_TEMP_OFFTIMEMIN ;

enum ThermoStates {
    STATE_DISABLED, STATE_OFF, STATE_OFF_DELAY, STATE_ON };

byte thermo_state = STATE_DISABLED;
uint16_t thermo_timer = 0;
uint16_t temperature_check = 0;


// #define OFF_DELAY 90  // minimal off time in seconds
// #define HYSTERESIS 2  // Hysteresys in 0.1 Degrees

  
/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

boolean ThermoCommand(char *type, uint16_t index, char *dataBuf, uint16_t data_len, int16_t payload)
{
    char command[CMDSZ];
    char sunit[CMDSZ];
    boolean serviced = true;

    int command_code = GetCommandCode(command, sizeof(command), type, kThermoCommands);
    if (CMND_TEMP_CONTROL == command_code) 
    {
        snprintf_P(log_data, sizeof(log_data), PSTR("Thermostat Control Command %d"), payload);
        AddLog(LOG_LEVEL_DEBUG);
        if ((payload == 0) || (payload == 1) || (payload == 2)) 
        {
            Settings.temp_control_mode = payload;
            snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, command, Settings.temp_control_mode);
        }
    }
    else if (CMND_TEMP_SETPOINT == command_code) 
    {
        snprintf_P(log_data, sizeof(log_data), PSTR("Thermostat Setpoint Command %d"), payload);
        AddLog(LOG_LEVEL_DEBUG);
        if ((payload != -99) && (payload >= -10000) && (payload <= +30000)) 
        {
            Settings.temp_control_setpoint = payload;
            snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, command,  Settings.temp_control_setpoint);
        }
    }
    else if (CMND_TEMP_HYSTERESIS == command_code) 
    {
        snprintf_P(log_data, sizeof(log_data), PSTR("Thermostat Hysteresis Command %d"), payload);
        AddLog(LOG_LEVEL_DEBUG);
        if ((payload >= 0) && (payload <= +30000)) 
        {
            Settings.temp_control_hysteresis = payload;
            snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, command,  Settings.temp_control_hysteresis);
        }
    }
    else if (CMND_TEMP_OFFTIMEMIN == command_code) 
    {
        snprintf_P(log_data, sizeof(log_data), PSTR("Thermostat Min Off Time Command %d"), payload);
        AddLog(LOG_LEVEL_DEBUG);
        if ((payload >= 0) && (payload <= +60000)) 
        {
            Settings.temp_control_offtimemin = payload;
            snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, command,  Settings.temp_control_offtimemin);
        }
    }
    else 
    {
        serviced = false;
    }
    return serviced;
}

// Standalone Thermostat Main Function
void ThermoFunction (int tele_period)
{
    if ((Settings.module == SONOFF_TH) && ((Settings.my_gp.io[14] == GPIO_DHT11) || (Settings.my_gp.io[14] == GPIO_DHT22) || (Settings.my_gp.io[14] == GPIO_SI7021) || (Settings.my_gp.io[14] == GPIO_DSB)))
    {
        float t = NAN;
        float h = NAN;
        int16_t temperature = -30000;
        int16_t humidity = -30000;
        bool readsuccess = false;
        bool controlserviced = false;
        thermo_timer++;
        if (temperature_check >= TEMPERATURECHECK)
        {
            if ((Settings.my_gp.io[14] == GPIO_DHT11) || (Settings.my_gp.io[14] == GPIO_DHT22) || (Settings.my_gp.io[14] == GPIO_SI7021))
            {
                if (DhtReadTempHum(0, t, h))   // Check if read failed
                {     // Read temperature
                    temperature = t * 100;
                    humidity = h * 100;
                    readsuccess = true;
                }
            }
            else if (Settings.my_gp.io[14] == GPIO_DSB)
            {
                if (Ds18b20Read(t))     // Check if read failed
                {                
                    temperature = (int)t * 100;
                    readsuccess = true;
                }
            }
            if (readsuccess)
            {
                if (thermo_state == STATE_ON) 
                {
                    if (Settings.temp_control_mode == TEMP_CONTROL_DISABLED) 
                    {
                        ExecuteCommandPower(1, POWER_OFF);
                        thermo_state = STATE_DISABLED;
                        controlserviced = true;
                    }
                }
                else if (thermo_state == STATE_OFF) 
                {
                    if (Settings.temp_control_mode == TEMP_CONTROL_DISABLED) 
                    {
                        ExecuteCommandPower(1, POWER_OFF);
                        thermo_state = STATE_DISABLED;
                        controlserviced = true;
                    }
                }
                else if (thermo_state == STATE_OFF_DELAY) 
                {
                    if (thermo_timer > Settings.temp_control_offtimemin) 
                    {
                        thermo_state = STATE_OFF;
                        thermo_timer = 0;
                        controlserviced = true;
                    }
                }
                else if (thermo_state == STATE_DISABLED) 
                {
                    if ((Settings.temp_control_mode == TEMP_CONTROL_COOLING) || (Settings.temp_control_mode == TEMP_CONTROL_HEATING))
                    {
                        ExecuteCommandPower(1, POWER_OFF);
                        thermo_state = STATE_OFF;
                        controlserviced = true;
                    }
                }
                if (!controlserviced)
                {
                    if (Settings.temp_control_mode == TEMP_CONTROL_COOLING)
                    {
                        if (thermo_state == STATE_ON) 
                        {
                            if (temperature <= (Settings.temp_control_setpoint)) 
                            {
                                ExecuteCommandPower(1, POWER_OFF);
                                thermo_timer = 0;
                                thermo_state = STATE_OFF_DELAY;
                            }
                        }
                        else if (thermo_state == STATE_OFF) 
                        {
                            if (temperature > (Settings.temp_control_setpoint + Settings.temp_control_hysteresis)) 
                            {
                                ExecuteCommandPower(1, POWER_ON);
                                thermo_state = STATE_ON;
                            }
                        }
                    }
                    else if (Settings.temp_control_mode == TEMP_CONTROL_HEATING)
                    {
                        if (thermo_state == STATE_ON) 
                        {
                            if (temperature >= (Settings.temp_control_setpoint)) 
                            {
                                ExecuteCommandPower(1, POWER_OFF);
                                thermo_timer = 0;
                                thermo_state = STATE_OFF_DELAY;
                            }
                        }
                        else if (thermo_state == STATE_OFF) 
                        {
                            if (temperature < (Settings.temp_control_setpoint - Settings.temp_control_hysteresis)) 
                            {
                                ExecuteCommandPower(1, POWER_ON);
                                thermo_state = STATE_ON;
                            }
                        }
                    }
                }
            }
            else
            {
                if (bitRead(power, 0))
                {
                    ExecuteCommandPower(1, POWER_OFF);
                    thermo_state = STATE_OFF_DELAY;
                    thermo_timer = 0;
                }
                else if (thermo_timer > Settings.temp_control_offtimemin) 
                {
                    thermo_state = STATE_OFF;
                    thermo_timer = 0;
                }
            }
            snprintf_P(log_data, sizeof(log_data), PSTR("Thermo Cntr %d | State %d | SP %d | Temp %d"), Settings.temp_control_mode, thermo_state, Settings.temp_control_setpoint, temperature);
            AddLog(LOG_LEVEL_DEBUG);
            temperature_check = 0;
        }
        else
        {
            temperature_check++;
        }
    }
}
