/*
  xsns_69_opentherm.ino - OpenTherm protocol support for Tasmota

  Copyright (C) 2021  Yuriy Sannikov

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

#ifdef USE_OPENTHERM

#define XSNS_69 69

#include <OpenTherm.h>

// Hot water and boiler parameter ranges
#ifndef OT_HOT_WATER_MIN
#define OT_HOT_WATER_MIN 23
#endif
#ifndef OT_HOT_WATER_MAX
#define OT_HOT_WATER_MAX 55
#endif
#ifndef OT_BOILER_MIN
#define OT_BOILER_MIN 40
#endif
#ifndef OT_BOILER_MAX
#define OT_BOILER_MAX 85
#endif

#ifndef OT_HOT_WATER_DEFAULT
#define OT_HOT_WATER_DEFAULT 36;
#endif
#ifndef OT_BOILER_DEFAULT
#define OT_BOILER_DEFAULT 85;
#endif

// Seconds before OT will make an attempt to connect to the boiler after connection error
#define SNS_OT_DISCONNECT_COOLDOWN_SECONDS 4

// Number of consecutive timeouts which are accepted before entering disconnect state
#define SNS_OT_MAX_TIMEOUTS_BEFORE_DISCONNECT 3

// Count of the OpenThermSettingsFlags
#define OT_FLAGS_COUNT 6
enum OpenThermSettingsFlags
{
    // If set, central heating on/off state follows diagnostic indication bit(6), however
    // EnableCentralHeating flag has a priority over it
    EnableCentralHeatingOnDiagnostics = 0x01,
    // If set, DHW is on after restart.
    EnableHotWater = 0x02,
    // If set, keep CH always on after restart. If off, follows the EnableCentralHeatingOnDiagnostics rule
    EnableCentralHeating = 0x04,
    EnableCooling = 0x08,
    EnableTemperatureCompensation = 0x10,
    EnableCentralHeating2 = 0x20,
};

enum OpenThermConnectionStatus
{
    OTC_NONE,         // OT not initialized
    OTC_DISCONNECTED, // OT communication timed out
    OTC_CONNECTING,   // Connecting after start or from DISCONNECTED state
    OTC_HANDSHAKE,    // Wait for the handshake response
    OTC_READY,        // Last Known Good response state is SUCCESS and no requests are in flight
    OTC_INFLIGHT      // Request sent, waiting from the response
};

OpenThermConnectionStatus sns_ot_connection_status = OpenThermConnectionStatus::OTC_NONE;
uint8_t sns_ot_disconnect_cooldown = 0;
uint8_t sns_ot_timeout_before_disconnect = 0;

OpenTherm *sns_ot_master = NULL;

// Has valid values if connection status is READY or INFLIGHT
typedef struct OT_BOILER_STATUS_T
{
    // Boiler fault code
    uint8_t m_fault_code;
    // Boiler OEM fault code
    uint8_t m_oem_fault_code;
    // Boilder OEM Diagnostics code
    uint16_t m_oem_diag_code;
    // OpenTherm ID(3) response.
    uint8_t m_slave_flags;
    // OpenTherm ID(1) codes. Should be used to display state
    unsigned long m_slave_raw_status;
    // Desired boiler states
    bool m_enableCentralHeating;
    bool m_enableHotWater;
    bool m_enableCooling;
    bool m_enableOutsideTemperatureCompensation;
    bool m_enableCentralHeating2;

    // Some boilers has an input for the heat request. When short, heat is requested
    // OT ID(0) bit 6 may indicate state of the Heat Request input
    // By enabling this bit we will set m_enableCentralHeating to true when OT ID(0) bit 6 is set.
    // This enables to use external mechanical thermostat to enable heating.
    // Some of the use cases might be setting an emergency temperature to prevent freezing
    // in case of the software thermostat failure.
    bool m_useDiagnosticIndicationAsHeatRequest;

    // Hot Water temperature
    float m_hotWaterSetpoint_read;
    // Flame Modulation
    float m_flame_modulation_read;
    // Boiler Temperature
    float m_boiler_temperature_read;

    // Boiler desired values
    float m_boilerSetpoint;
    float m_hotWaterSetpoint;
    // This flag is set when Master require a CH to be on
    // and forces the OpenThermMessageID::TSet to be sent to the boiler
    bool m_forceSetpointSet;

} OT_BOILER_STATUS;

OT_BOILER_STATUS sns_ot_boiler_status;

const char *sns_opentherm_connection_stat_to_str(int status)
{
    switch (status)
    {
    case OpenThermConnectionStatus::OTC_NONE:
        return "NONE";
    case OpenThermConnectionStatus::OTC_DISCONNECTED:
        return "FAULT";
    case OpenThermConnectionStatus::OTC_CONNECTING:
        return "CONNECTING";
    case OpenThermConnectionStatus::OTC_HANDSHAKE:
        return "HANDSHAKE";
    case OpenThermConnectionStatus::OTC_READY:
        return "READY";
    case OpenThermConnectionStatus::OTC_INFLIGHT:
        return "BUSY";
    default:
        return "UNKNOWN";
    }
}

void sns_opentherm_init_boiler_status()
{
    memset(&sns_ot_boiler_status, 0, sizeof(OT_BOILER_STATUS));

    // Settings
    sns_ot_boiler_status.m_useDiagnosticIndicationAsHeatRequest = Settings->ot_flags & (uint8_t)OpenThermSettingsFlags::EnableCentralHeatingOnDiagnostics;
    sns_ot_boiler_status.m_enableHotWater = Settings->ot_flags & (uint8_t)OpenThermSettingsFlags::EnableHotWater;
    sns_ot_boiler_status.m_enableCentralHeating = Settings->ot_flags & (uint8_t)OpenThermSettingsFlags::EnableCentralHeating;
    sns_ot_boiler_status.m_enableCooling = Settings->ot_flags & (uint8_t)OpenThermSettingsFlags::EnableCooling;
    sns_ot_boiler_status.m_enableOutsideTemperatureCompensation = Settings->ot_flags & (uint8_t)OpenThermSettingsFlags::EnableTemperatureCompensation;
    sns_ot_boiler_status.m_enableCentralHeating2 = Settings->ot_flags & (uint8_t)OpenThermSettingsFlags::EnableCentralHeating2;

    sns_ot_boiler_status.m_boilerSetpoint = (float)Settings->ot_boiler_setpoint;
    sns_ot_boiler_status.m_hotWaterSetpoint = (float)Settings->ot_hot_water_setpoint;

    sns_ot_boiler_status.m_fault_code = 0;
    sns_ot_boiler_status.m_oem_fault_code = 0;
    sns_ot_boiler_status.m_oem_diag_code = 0;
    sns_ot_boiler_status.m_hotWaterSetpoint_read = 0;
    sns_ot_boiler_status.m_flame_modulation_read = 0;
    sns_ot_boiler_status.m_boiler_temperature_read = 0;
}

void IRAM_ATTR sns_opentherm_handleInterrupt()
{
    sns_ot_master->handleInterrupt();
}

void sns_opentherm_processResponseCallback(unsigned long response, int st)
{
    OpenThermResponseStatus status = (OpenThermResponseStatus)st;
    AddLog(LOG_LEVEL_DEBUG_MORE,
              PSTR("[OTH]: Processing response. Status=%s, Response=0x%lX"),
              sns_ot_master->statusToString(status), response);

    if (sns_ot_connection_status == OpenThermConnectionStatus::OTC_HANDSHAKE)
    {
        return sns_ot_process_handshake(response, st);
    }

    switch (status)
    {
    case OpenThermResponseStatus::OPTH_SUCCESS:
        if (sns_ot_master->isValidResponse(response))
        {
            sns_opentherm_process_success_response(&sns_ot_boiler_status, response);
        }
        sns_ot_connection_status = OpenThermConnectionStatus::OTC_READY;
        sns_ot_timeout_before_disconnect = SNS_OT_MAX_TIMEOUTS_BEFORE_DISCONNECT;
        break;

    case OpenThermResponseStatus::OPTH_INVALID:
        sns_opentherm_check_retry_request();
        sns_ot_connection_status = OpenThermConnectionStatus::OTC_READY;
        sns_ot_timeout_before_disconnect = SNS_OT_MAX_TIMEOUTS_BEFORE_DISCONNECT;
        break;

    // Timeout may indicate not valid/supported command or connection error
    // In this case we do reconnect.
    // If this command will timeout multiple times, it will be excluded from the rotation later on
    // after couple of failed attempts. See sns_opentherm_check_retry_request logic
    case OpenThermResponseStatus::OPTH_TIMEOUT:
        sns_opentherm_check_retry_request();
        if (--sns_ot_timeout_before_disconnect == 0)
        {
            sns_ot_connection_status = OpenThermConnectionStatus::OTC_DISCONNECTED;
        }
        else
        {
            sns_ot_connection_status = OpenThermConnectionStatus::OTC_READY;
        }
        break;
    }
}

bool sns_opentherm_Init()
{
    if (PinUsed(GPIO_BOILER_OT_RX) && PinUsed(GPIO_BOILER_OT_TX))
    {
        sns_ot_master = new OpenTherm(Pin(GPIO_BOILER_OT_RX), Pin(GPIO_BOILER_OT_TX));
        sns_ot_master->begin(sns_opentherm_handleInterrupt, sns_opentherm_processResponseCallback);
        sns_ot_connection_status = OpenThermConnectionStatus::OTC_CONNECTING;
        return true;
    }
    return false;
    // !warning, sns_opentherm settings are not ready at this point
}

void sns_opentherm_stat(bool json)
{
    if (!sns_ot_master)
    {
        return;
    }
    const char *statusStr = sns_opentherm_connection_stat_to_str(sns_ot_connection_status);

    if (json)
    {
        ResponseAppend_P(PSTR(",\"OPENTHERM\":{\"conn\":\"%s\",\"settings\":%d"), statusStr, Settings->ot_flags);
        sns_opentherm_dump_telemetry();
        ResponseJsonEnd();
#ifdef USE_WEBSERVER
    }
    else
    {
        WSContentSend_P(PSTR("{s}OpenTherm status{m}%s (0x%X){e}"), statusStr, (int)sns_ot_boiler_status.m_slave_flags);
        if (sns_ot_connection_status < OpenThermConnectionStatus::OTC_READY)
        {
            return;
        }
        WSContentSend_P(PSTR("{s}Std/OEM Fault Codes{m}%d / %d{e}"),
                        (int)sns_ot_boiler_status.m_fault_code,
                        (int)sns_ot_boiler_status.m_oem_fault_code);

        WSContentSend_P(PSTR("{s}OEM Diagnostic Code{m}%d{e}"),
                        (int)sns_ot_boiler_status.m_oem_diag_code);

        WSContentSend_P(PSTR("{s}Hot Water Setpoint{m}%d{e}"),
                        (int)sns_ot_boiler_status.m_hotWaterSetpoint_read);

        WSContentSend_P(PSTR("{s}Flame Modulation{m}%d{e}"),
                        (int)sns_ot_boiler_status.m_flame_modulation_read);

        WSContentSend_P(PSTR("{s}Boiler Temp/Setpnt{m}%d / %d{e}"),
                        (int)sns_ot_boiler_status.m_boiler_temperature_read,
                        (int)sns_ot_boiler_status.m_boilerSetpoint);

        if (OpenTherm::isCentralHeatingActive(sns_ot_boiler_status.m_slave_raw_status))
        {
            WSContentSend_P(PSTR("{s}Central Heating is ACTIVE{m}{e}"));
        }

        if (sns_ot_boiler_status.m_enableHotWater)
        {
            WSContentSend_P(PSTR("{s}Hot Water is Enabled{m}{e}"));
        }

        if (OpenTherm::isHotWaterActive(sns_ot_boiler_status.m_slave_raw_status))
        {
            WSContentSend_P(PSTR("{s}Hot Water is ACTIVE{m}{e}"));
        }

        if (OpenTherm::isFlameOn(sns_ot_boiler_status.m_slave_raw_status))
        {
            WSContentSend_P(PSTR("{s}Flame is ACTIVE{m}{e}"));
        }

        if (sns_ot_boiler_status.m_enableCooling)
        {
            WSContentSend_P(PSTR("{s}Cooling is Enabled{m}{e}"));
        }

        if (OpenTherm::isCoolingActive(sns_ot_boiler_status.m_slave_raw_status))
        {
            WSContentSend_P(PSTR("{s}Cooling is ACTIVE{m}{e}"));
        }

        if (OpenTherm::isDiagnostic(sns_ot_boiler_status.m_slave_raw_status))
        {
            WSContentSend_P(PSTR("{s}Diagnostic Indication{m}{e}"));
        }

#endif // USE_WEBSERVER
    }
}

void sns_ot_start_handshake()
{
    if (!sns_ot_master)
    {
        return;
    }

    AddLog(LOG_LEVEL_DEBUG, PSTR("[OTH]: perform handshake"));

    sns_opentherm_protocol_reset();

    sns_ot_master->sendRequestAync(
        OpenTherm::buildRequest(OpenThermMessageType::OPTH_READ_DATA, OpenThermMessageID::SConfigSMemberIDcode, 0));

    sns_ot_connection_status = OpenThermConnectionStatus::OTC_HANDSHAKE;
}

void sns_ot_process_handshake(unsigned long response, int st)
{
    OpenThermResponseStatus status = (OpenThermResponseStatus)st;

    if (status != OpenThermResponseStatus::OPTH_SUCCESS || !sns_ot_master->isValidResponse(response))
    {
        AddLog(LOG_LEVEL_ERROR,
                  PSTR("[OTH]: getSlaveConfiguration failed. Status=%s"),
                  sns_ot_master->statusToString(status));
        sns_ot_connection_status = OpenThermConnectionStatus::OTC_DISCONNECTED;
        return;
    }

    AddLog(LOG_LEVEL_DEBUG, PSTR("[OTH]: getLastResponseStatus SUCCESS. Slave Cfg: %lX"), response);

    sns_ot_boiler_status.m_slave_flags = (response & 0xFF00) >> 8;

    sns_ot_connection_status = OpenThermConnectionStatus::OTC_READY;
}

void sns_opentherm_check_settings(void)
{
    bool settingsValid = true;

    settingsValid &= Settings->ot_hot_water_setpoint >= OT_HOT_WATER_MIN;
    settingsValid &= Settings->ot_hot_water_setpoint <= OT_HOT_WATER_MAX;
    settingsValid &= Settings->ot_boiler_setpoint >= OT_BOILER_MIN;
    settingsValid &= Settings->ot_boiler_setpoint <= OT_BOILER_MAX;

    if (!settingsValid)
    {
        Settings->ot_hot_water_setpoint = OT_HOT_WATER_DEFAULT;
        Settings->ot_boiler_setpoint = OT_BOILER_DEFAULT;
        Settings->ot_flags =
            OpenThermSettingsFlags::EnableCentralHeatingOnDiagnostics |
            OpenThermSettingsFlags::EnableHotWater;
    }
}
/*********************************************************************************************\
 * Command Processing
\*********************************************************************************************/
const char *sns_opentherm_flag_text(uint8_t mode)
{
    switch ((OpenThermSettingsFlags)mode)
    {
    case OpenThermSettingsFlags::EnableCentralHeatingOnDiagnostics:
        return "CHOD";
    case OpenThermSettingsFlags::EnableHotWater:
        return "DHW";
    case OpenThermSettingsFlags::EnableCentralHeating:
        return "CH";
    case OpenThermSettingsFlags::EnableCooling:
        return "COOL";
    case OpenThermSettingsFlags::EnableTemperatureCompensation:
        return "OTC";
    case OpenThermSettingsFlags::EnableCentralHeating2:
        return "CH2";
    default:
        return "?";
    }
}

uint8_t sns_opentherm_parse_flag(char *flag)
{
    if (!strncmp(flag, "CHOD", 4))
    {
        return OpenThermSettingsFlags::EnableCentralHeatingOnDiagnostics;
    }
    else if (!strncmp(flag, "COOL", 4))
    {
        return OpenThermSettingsFlags::EnableCooling;
    }
    else if (!strncmp(flag, "DHW", 3))
    {
        return OpenThermSettingsFlags::EnableHotWater;
    }
    else if (!strncmp(flag, "OTC", 3))
    {
        return OpenThermSettingsFlags::EnableTemperatureCompensation;
    }
    else if (!strncmp(flag, "CH2", 3))
    {
        return OpenThermSettingsFlags::EnableCentralHeating2;
    }
    else if (!strncmp(flag, "CH", 2))
    {
        return OpenThermSettingsFlags::EnableCentralHeating;
    }
    return 0;
}

uint8_t sns_opentherm_read_flags(char *data, uint32_t len)
{
    uint8_t tokens = 1;
    for (int i = 0; i < len; ++i)
    {
        if (data[i] == ',')
        {
            ++tokens;
        }
    }
    uint8_t result = 0;
    char sub_string[XdrvMailbox.data_len + 1];
    for (int i = 1; i <= tokens; ++i)
    {
        char *flag = subStr(sub_string, data, ",", i);
        if (!flag)
        {
            break;
        }
        result |= sns_opentherm_parse_flag(flag);
    }
    return result;
}
#define D_PRFX_OTHERM "ot_"
// set the boiler temperature (CH). Sutable for the PID app.
// After restart will use the defaults from the settings
#define D_CMND_OTHERM_BOILER_SETPOINT "tboiler"
// set hot water (DHW) temperature. Do not write it in the flash memory.
// suitable for the temporary changes
#define D_CMND_OTHERM_DHW_SETPOINT "twater"
// This command will save CH and DHW setpoints into the Settings-> Those values will be used after system restart
// The reason to separate set and save is to reduce flash memory write count, especially if boiler temperature is controlled
// by the PID thermostat
#define D_CMND_OTHERM_SAVE_SETTINGS "save_setpoints"
// Get or set flags

// EnableCentralHeatingOnDiagnostics -> CHOD
// EnableHotWater                    -> DHW
// EnableCentralHeating              -> CH
// EnableCooling                     -> COOL
// EnableTemperatureCompensation     -> OTC
// EnableCentralHeating2             -> CH2
#define D_CMND_OTHERM_FLAGS "flags"

// Get/Set boiler status m_enableCentralHeating value. It's equivalent of the EnableCentralHeating settings
// flag value, however, this command does not update the Settings->
// Usefull to buld automations
// Please note, if you set it to "0" and EnableCentralHeatingOnDiagnostics is set
// boiler will follow the Diagnostics bit and won't turn CH off. When Diagnostics bit cleared,
// and "ot_ch" is "1", boiler will keep heating
#define D_CMND_SET_CENTRAL_HEATING_ENABLED "ch"

// Get/Set boiler status m_enableHotWater value. It's equivalent of the EnableHotWater settings
// flag value, however, this command does not update the settings.
#define D_CMND_SET_HOT_WATER_ENABLED "dhw"

// BLOR - Reset boiler 
#define D_CMND_BLLOR "blor"

const char kOpenThermCommands[] PROGMEM = D_PRFX_OTHERM "|" D_CMND_OTHERM_BOILER_SETPOINT "|" D_CMND_OTHERM_DHW_SETPOINT
	    "|" D_CMND_OTHERM_SAVE_SETTINGS "|" D_CMND_OTHERM_FLAGS "|" D_CMND_SET_CENTRAL_HEATING_ENABLED "|" D_CMND_SET_HOT_WATER_ENABLED "|" D_CMND_BLLOR;

void (*const OpenThermCommands[])(void) PROGMEM = {
    &sns_opentherm_boiler_setpoint_cmd,
    &sns_opentherm_hot_water_setpoint_cmd,
    &sns_opentherm_save_settings_cmd,
    &sns_opentherm_flags_cmd,
    &sns_opentherm_set_central_heating_cmd,
    &sns_opentherm_set_hot_water_cmd,
    &sns_opentherm_blor_cmd,};

void sns_opentherm_cmd(void) { }
void sns_opentherm_boiler_setpoint_cmd(void)
{
    bool query = strlen(XdrvMailbox.data) == 0;
    if (!query)
    {
        sns_ot_boiler_status.m_boilerSetpoint = CharToFloat(XdrvMailbox.data);
    }
    ResponseCmndFloat(sns_ot_boiler_status.m_boilerSetpoint, Settings->flag2.temperature_resolution);
}

void sns_opentherm_hot_water_setpoint_cmd(void)
{
    bool query = strlen(XdrvMailbox.data) == 0;
    if (!query)
    {
        sns_ot_boiler_status.m_hotWaterSetpoint = CharToFloat(XdrvMailbox.data);
    }
    ResponseCmndFloat(sns_ot_boiler_status.m_hotWaterSetpoint, Settings->flag2.temperature_resolution);
}

void sns_opentherm_save_settings_cmd(void)
{
    Settings->ot_hot_water_setpoint = (uint8_t)sns_ot_boiler_status.m_hotWaterSetpoint;
    Settings->ot_boiler_setpoint = (uint8_t)sns_ot_boiler_status.m_boilerSetpoint;
    ResponseCmndDone();
}

void sns_opentherm_flags_cmd(void)
{
    bool query = strlen(XdrvMailbox.data) == 0;
    if (!query)
    {
        // Set flags value
        Settings->ot_flags = sns_opentherm_read_flags(XdrvMailbox.data, XdrvMailbox.data_len);
        // Reset boiler status to apply settings
        sns_opentherm_init_boiler_status();
    }
    bool addComma = false;
    ResponseClear();
    for (int pos = 0; pos < OT_FLAGS_COUNT; ++pos)
    {
        int mask = 1 << pos;
        int mode = Settings->ot_flags & (uint8_t)mask;
        if (mode > 0)
        {
            ResponseAppend_P(PSTR("%s%s"), (addComma)?",":"", sns_opentherm_flag_text(mode));
            addComma = true;
        }
    }
}

void sns_opentherm_set_central_heating_cmd(void)
{
    bool query = strlen(XdrvMailbox.data) == 0;
    if (!query)
    {
        sns_ot_boiler_status.m_enableCentralHeating = atoi(XdrvMailbox.data);
    }
    ResponseCmndNumber(sns_ot_boiler_status.m_enableCentralHeating ? 1 : 0);
}

void sns_opentherm_set_hot_water_cmd(void)
{
    bool query = strlen(XdrvMailbox.data) == 0;
    if (!query)
    {
        sns_ot_boiler_status.m_enableHotWater = atoi(XdrvMailbox.data);
    }
    ResponseCmndNumber(sns_ot_boiler_status.m_enableHotWater ? 1 : 0);
}

void sns_opentherm_blor_cmd(void)
{
    bool query = strlen(XdrvMailbox.data) == 0;
    bool retval = false;
    if (!query)
    {
        if (atoi(XdrvMailbox.data)) retval = sns_opentherm_call_blor();
    }
    ResponseCmndNumber(retval);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns69(uint32_t function)
{
    bool result = false;
    if (FUNC_INIT == function)
    {
        if (sns_opentherm_Init())
        {
            sns_opentherm_check_settings();
            sns_opentherm_init_boiler_status();
        }
    }

    if (!sns_ot_master)
    {
        return result;
    }

    switch (function)
    {
    case FUNC_LOOP:
        sns_ot_master->process();
        break;
    case FUNC_EVERY_100_MSECOND:
        if (sns_ot_connection_status == OpenThermConnectionStatus::OTC_READY && sns_ot_master->isReady())
        {
            unsigned long request = sns_opentherm_get_next_request(&sns_ot_boiler_status);
            if (-1 != request)
            {
                sns_ot_master->sendRequestAync(request);
                sns_ot_connection_status = OpenThermConnectionStatus::OTC_INFLIGHT;
            }
        }
        break;
    case FUNC_EVERY_SECOND:
        if (sns_ot_connection_status == OpenThermConnectionStatus::OTC_DISCONNECTED)
        {
            // If disconnected, wait for the SNS_OT_DISCONNECT_COOLDOWN_SECONDS before the handshake
            if (sns_ot_disconnect_cooldown == 0)
            {
                sns_ot_disconnect_cooldown = SNS_OT_DISCONNECT_COOLDOWN_SECONDS;
            }
            else if (--sns_ot_disconnect_cooldown == 0)
            {
                sns_ot_connection_status = OpenThermConnectionStatus::OTC_CONNECTING;
            }
        }
        else if (sns_ot_connection_status == OpenThermConnectionStatus::OTC_CONNECTING)
        {
            sns_ot_start_handshake();
        }
        break;
    case FUNC_COMMAND:
        result = DecodeCommand(kOpenThermCommands, OpenThermCommands);
        break;
    case FUNC_JSON_APPEND:
        sns_opentherm_stat(1);
        break;
    case FUNC_SAVE_AT_MIDNIGHT:
        sns_opentherm_protocol_reset();
        break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
        sns_opentherm_stat(0);
        break;
#endif // USE_WEBSERVER
    }

    return result;
}

#endif // USE_OPENTHERM
