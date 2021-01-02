/*
  xsns_69_opentherm_protocol.ino - OpenTherm protocol support for Tasmota

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

#include "OpenTherm.h"

// Temperature tolerance. If temperature setpoint difference is less than the value,
// OT (1)(Control setpoint) command will be skipped
#define OPENTHERM_BOILER_SETPOINT_TOLERANCE 1.0

typedef union {
    uint8_t m_flags;
    struct
    {
        uint8_t notSupported : 1; // If set, boiler does not support this command
        uint8_t supported : 1;    // Set if at least one response were successfull
        uint8_t retryCount : 2;   // Retry counter before notSupported flag being set
    };
} OpenThermParamFlags;

typedef union {
    float m_float;
    uint8_t m_u8;
    uint16_t m_u16;
    unsigned long m_ul;
    bool m_bool;
} ResponseStorage;

typedef struct OpenThermCommandT
{
    const char *m_command_name;
    uint8_t m_command_code;
    OpenThermParamFlags m_flags;
    ResponseStorage m_results[2];
    unsigned long (*m_ot_make_request)(OpenThermCommandT *self, OT_BOILER_STATUS_T *boilerStatus);
    void (*m_ot_parse_response)(OpenThermCommandT *self, OT_BOILER_STATUS_T *boilerStatus, unsigned long response);
    void (*m_ot_appent_telemetry)(OpenThermCommandT *self);
} OpenThermCommand;

OpenThermCommand sns_opentherm_commands[] = {
    {// Get/Set Slave Status Flags
     .m_command_name = "SLAVE",
     .m_command_code = 0,
     // OpenTherm ID(0) should never go into the notSupported state due to some connectivity issues
     // otherwice it may lose boiler control
     .m_flags = {.supported = 1},
     .m_results = {{.m_u8 = 0}, {.m_u8 = 0}},
     .m_ot_make_request = sns_opentherm_set_slave_flags,
     .m_ot_parse_response = sns_opentherm_parse_slave_flags,
     .m_ot_appent_telemetry = sns_opentherm_tele_slave_flags},
    {// Set boiler temperature
     .m_command_name = "BTMP",
     .m_command_code = 0,
     // OpenTherm ID(1) also should never go into the notSupported state due to some connectivity issues
     .m_flags = {.supported = 1},
     .m_results = {{.m_u8 = 0}, {.m_u8 = 0}},
     .m_ot_make_request = sns_opentherm_set_boiler_temperature,
     .m_ot_parse_response = sns_opentherm_parse_set_boiler_temperature,
     .m_ot_appent_telemetry = sns_opentherm_tele_boiler_temperature},
    {// Set Hot Water temperature
     .m_command_name = "HWTMP",
     .m_command_code = 0,
     // OpenTherm ID(56) may not be supported
     .m_flags = 0,
     .m_results = {{.m_u8 = 0}, {.m_u8 = 0}},
     .m_ot_make_request = sns_opentherm_set_boiler_dhw_temperature,
     .m_ot_parse_response = sns_opentherm_parse_boiler_dhw_temperature,
     .m_ot_appent_telemetry = sns_opentherm_tele_boiler_dhw_temperature},
    {// Read Application-specific fault flags and OEM fault code
     .m_command_name = "ASFF",
     .m_command_code = 0,
     .m_flags = 0,
     .m_results = {{.m_u8 = 0}, {.m_u8 = 0}},
     .m_ot_make_request = sns_opentherm_get_flags,
     .m_ot_parse_response = sns_opentherm_parse_flags,
     .m_ot_appent_telemetry = sns_opentherm_tele_flags},
    {// Read An OEM-specific diagnostic/service code
     .m_command_name = "OEMD",
     .m_command_code = 0,
     .m_flags = 0,
     .m_results = {{.m_u8 = 0}, {.m_u8 = 0}},
     .m_ot_make_request = sns_opentherm_get_oem_diag,
     .m_ot_parse_response = sns_opentherm_parse_oem_diag,
     .m_ot_appent_telemetry = sns_opentherm_tele_oem_diag},
    {// Read Flame modulation
     .m_command_name = "FLM",
     .m_command_code = (uint8_t)OpenThermMessageID::RelModLevel,
     .m_flags = 0,
     .m_results = {{.m_u8 = 0}, {.m_u8 = 0}},
     .m_ot_make_request = sns_opentherm_get_generic_float,
     .m_ot_parse_response = sns_opentherm_parse_flame_modulation,
     .m_ot_appent_telemetry = sns_opentherm_tele_generic_float},
    {// Read Boiler Temperature
     .m_command_name = "TB",
     .m_command_code = (uint8_t)OpenThermMessageID::Tboiler,
     .m_flags = 0,
     .m_results = {{.m_u8 = 0}, {.m_u8 = 0}},
     .m_ot_make_request = sns_opentherm_get_generic_float,
     .m_ot_parse_response = sns_opentherm_parse_boiler_temperature,
     .m_ot_appent_telemetry = sns_opentherm_tele_generic_float},
    {// Read DHW temperature
     .m_command_name = "TDHW",
     .m_command_code = (uint8_t)OpenThermMessageID::Tdhw,
     .m_flags = 0,
     .m_results = {{.m_u8 = 0}, {.m_u8 = 0}},
     .m_ot_make_request = sns_opentherm_get_generic_float,
     .m_ot_parse_response = sns_opentherm_parse_generic_float,
     .m_ot_appent_telemetry = sns_opentherm_tele_generic_float},
    {// Read Outside temperature
     .m_command_name = "TOUT",
     .m_command_code = (uint8_t)OpenThermMessageID::Toutside,
     .m_flags = 0,
     .m_results = {{.m_u8 = 0}, {.m_u8 = 0}},
     .m_ot_make_request = sns_opentherm_get_generic_float,
     .m_ot_parse_response = sns_opentherm_parse_generic_float,
     .m_ot_appent_telemetry = sns_opentherm_tele_generic_float},
    {// Read Return water temperature
     .m_command_name = "TRET",
     .m_command_code = (uint8_t)OpenThermMessageID::Tret,
     .m_flags = 0,
     .m_results = {{.m_u8 = 0}, {.m_u8 = 0}},
     .m_ot_make_request = sns_opentherm_get_generic_float,
     .m_ot_parse_response = sns_opentherm_parse_generic_float,
     .m_ot_appent_telemetry = sns_opentherm_tele_generic_float},
    {// Read DHW setpoint
     .m_command_name = "DHWS",
     .m_command_code = (uint8_t)OpenThermMessageID::TdhwSet,
     .m_flags = 0,
     .m_results = {{.m_u8 = 0}, {.m_u8 = 0}},
     .m_ot_make_request = sns_opentherm_get_generic_float,
     .m_ot_parse_response = sns_opentherm_parse_dhw_setpoint,
     .m_ot_appent_telemetry = sns_opentherm_tele_generic_float},
    {// Read max CH water setpoint
     .m_command_name = "TMAX",
     .m_command_code = (uint8_t)OpenThermMessageID::MaxTSet,
     .m_flags = 0,
     .m_results = {{.m_u8 = 0}, {.m_u8 = 0}},
     .m_ot_make_request = sns_opentherm_get_generic_float,
     .m_ot_parse_response = sns_opentherm_parse_generic_float,
     .m_ot_appent_telemetry = sns_opentherm_tele_generic_float},

};

/////////////////////////////////// Process Slave Status Flags & Control //////////////////////////////////////////////////
unsigned long sns_opentherm_set_slave_flags(struct OpenThermCommandT *self, struct OT_BOILER_STATUS_T *status)
{
    bool centralHeatingIsOn = status->m_enableCentralHeating;

    if (status->m_useDiagnosticIndicationAsHeatRequest) {
        centralHeatingIsOn |= OpenTherm::isDiagnostic(status->m_slave_raw_status);
    }

    if (self->m_results[1].m_bool != centralHeatingIsOn) {
        AddLog_P(LOG_LEVEL_INFO,
            PSTR("[OTH]: Central Heating transitioning from %s to %s"),
            self->m_results[1].m_bool ? "on" : "off",
            status->m_enableCentralHeating ? "on" : "off");
    }
    self->m_results[1].m_bool = centralHeatingIsOn;

    unsigned int data = centralHeatingIsOn |
                        (status->m_enableHotWater << 1) |
                        (status->m_enableCooling << 2) |
                        (status->m_enableOutsideTemperatureCompensation << 3) |
                        (status->m_enableCentralHeating2 << 4);

    data <<= 8;

    return OpenTherm::buildRequest(OpenThermRequestType::READ, OpenThermMessageID::Status, data);
}

void sns_opentherm_parse_slave_flags(struct OpenThermCommandT *self, struct OT_BOILER_STATUS_T *boilerStatus, unsigned long response)
{
    boilerStatus->m_slave_raw_status = response;
    self->m_results[0].m_ul = response;
}

#define OT_FLAG_TO_ON_OFF(status, flag) ((((status) & (flag)) != 0) ? 1 : 0)
void sns_opentherm_tele_slave_flags(struct OpenThermCommandT *self)
{
    unsigned long st = self->m_results[0].m_ul;
    ResponseAppend_P(PSTR("{\"FAULT\":%d,\"CH\":%d,\"DHW\":%d,\"FL\":%d,\"COOL\":%d,\"CH2\":%d,\"DIAG\":%d,\"RAW\":%lu}"),
                     OT_FLAG_TO_ON_OFF(st, 0x01),
                     OT_FLAG_TO_ON_OFF(st, 0x02),
                     OT_FLAG_TO_ON_OFF(st, 0x04),
                     OT_FLAG_TO_ON_OFF(st, 0x08),
                     OT_FLAG_TO_ON_OFF(st, 0x10),
                     OT_FLAG_TO_ON_OFF(st, 0x20),
                     OT_FLAG_TO_ON_OFF(st, 0x40),
                     st);
}

/////////////////////////////////// Set Boiler Temperature //////////////////////////////////////////////////
unsigned long sns_opentherm_set_boiler_temperature(struct OpenThermCommandT *self, struct OT_BOILER_STATUS_T *status)
{
    // Assuming some boilers might write setpoint temperature into the Flash memory
    // Having PID controlled appliance may produce a lot of small fluctuations in the setpoint value
    // wearing out Boiler flash memory.
    float diff = abs(status->m_boilerSetpoint - self->m_results[0].m_float);
    // Ignore small changes in the boiler setpoint temperature
    if (diff < OPENTHERM_BOILER_SETPOINT_TOLERANCE)
    {
        return -1;
    }
    AddLog_P(LOG_LEVEL_INFO,
              PSTR("[OTH]: Setting Boiler Temp. Old: %d, New: %d"),
              (int)self->m_results[0].m_float,
              (int)status->m_boilerSetpoint);
    self->m_results[0].m_float = status->m_boilerSetpoint;

    unsigned int data = OpenTherm::temperatureToData(status->m_boilerSetpoint);
    return OpenTherm::buildRequest(OpenThermMessageType::WRITE_DATA, OpenThermMessageID::TSet, data);
}
void sns_opentherm_parse_set_boiler_temperature(struct OpenThermCommandT *self, struct OT_BOILER_STATUS_T *boilerStatus, unsigned long response)
{
    self->m_results[1].m_float = OpenTherm::getFloat(response);
}
void sns_opentherm_tele_boiler_temperature(struct OpenThermCommandT *self)
{
    char requested[FLOATSZ];
    dtostrfd(self->m_results[0].m_float, Settings.flag2.temperature_resolution, requested);
    char actual[FLOATSZ];
    dtostrfd(self->m_results[1].m_float, Settings.flag2.temperature_resolution, actual);

    // indicate fault if tepmerature demand and actual setpoint are greater then tolerance
    bool isFault = abs(self->m_results[1].m_float - self->m_results[0].m_float) > OPENTHERM_BOILER_SETPOINT_TOLERANCE;

    ResponseAppend_P(PSTR("{\"FAULT\":%d,\"REQ\":%s,\"ACT\": %s}"),
                     (int)isFault,
                     requested,
                     actual);
}

/////////////////////////////////// Set Domestic Hot Water Temperature //////////////////////////////////////////////////
unsigned long sns_opentherm_set_boiler_dhw_temperature(struct OpenThermCommandT *self, struct OT_BOILER_STATUS_T *status)
{
    // The same consideration as for the boiler temperature
    float diff = abs(status->m_hotWaterSetpoint - self->m_results[0].m_float);
    // Ignore small changes in the boiler setpoint temperature
    if (diff < OPENTHERM_BOILER_SETPOINT_TOLERANCE)
    {
        return -1;
    }
    AddLog_P(LOG_LEVEL_INFO,
              PSTR("[OTH]: Setting Hot Water Temp. Old: %d, New: %d"),
              (int)self->m_results[0].m_float,
              (int)status->m_hotWaterSetpoint);

    self->m_results[0].m_float = status->m_hotWaterSetpoint;

    unsigned int data = OpenTherm::temperatureToData(status->m_hotWaterSetpoint);
    return OpenTherm::buildRequest(OpenThermMessageType::WRITE_DATA, OpenThermMessageID::TdhwSet, data);
}
void sns_opentherm_parse_boiler_dhw_temperature(struct OpenThermCommandT *self, struct OT_BOILER_STATUS_T *boilerStatus, unsigned long response)
{
    self->m_results[1].m_float = OpenTherm::getFloat(response);
}
void sns_opentherm_tele_boiler_dhw_temperature(struct OpenThermCommandT *self)
{
    char requested[FLOATSZ];
    dtostrfd(self->m_results[0].m_float, Settings.flag2.temperature_resolution, requested);
    char actual[FLOATSZ];
    dtostrfd(self->m_results[1].m_float, Settings.flag2.temperature_resolution, actual);

    ResponseAppend_P(PSTR("{\"REQ\":%s,\"ACT\": %s}"),
                     requested,
                     actual);
}

/////////////////////////////////// App Specific Fault Flags //////////////////////////////////////////////////
unsigned long sns_opentherm_get_flags(struct OpenThermCommandT *self, struct OT_BOILER_STATUS_T *)
{
    return OpenTherm::buildRequest(OpenThermRequestType::READ, OpenThermMessageID::ASFflags, 0);
}

void sns_opentherm_parse_flags(struct OpenThermCommandT *self, struct OT_BOILER_STATUS_T *boilerStatus, unsigned long response)
{
    uint8_t fault_code = (response >> 8) & 0xFF;
    uint8_t oem_fault_code = response & 0xFF;
    boilerStatus->m_fault_code = fault_code;
    boilerStatus->m_oem_fault_code = fault_code;
    self->m_results[0].m_u8 = fault_code;
    self->m_results[1].m_u8 = oem_fault_code;
}

void sns_opentherm_tele_flags(struct OpenThermCommandT *self)
{
    ResponseAppend_P(PSTR("{\"FC\":%d,\"OFC\":%d}"),
                     (int)self->m_results[0].m_u8,
                     (int)self->m_results[1].m_u8);
}

/////////////////////////////////// OEM Diag Code //////////////////////////////////////////////////
unsigned long sns_opentherm_get_oem_diag(struct OpenThermCommandT *self, struct OT_BOILER_STATUS_T *)
{
    return OpenTherm::buildRequest(OpenThermRequestType::READ, OpenThermMessageID::OEMDiagnosticCode, 0);
}

void sns_opentherm_parse_oem_diag(struct OpenThermCommandT *self, struct OT_BOILER_STATUS_T *boilerStatus, unsigned long response)
{
    uint16_t diag_code = (uint16_t)response & 0xFFFF;
    boilerStatus->m_oem_diag_code = diag_code;
    self->m_results[0].m_u16 = diag_code;
}

void sns_opentherm_tele_oem_diag(struct OpenThermCommandT *self)
{
    ResponseAppend_P(PSTR("%d"), (int)self->m_results[0].m_u16);
}

/////////////////////////////////// Generic Single Float /////////////////////////////////////////////////
unsigned long sns_opentherm_get_generic_float(struct OpenThermCommandT *self, struct OT_BOILER_STATUS_T *)
{
    return OpenTherm::buildRequest(OpenThermRequestType::READ, (OpenThermMessageID)self->m_command_code, 0);
}

void sns_opentherm_parse_generic_float(struct OpenThermCommandT *self, struct OT_BOILER_STATUS_T *boilerStatus, unsigned long response)
{
    self->m_results[0].m_float = OpenTherm::getFloat(response);
}

void sns_opentherm_tele_generic_float(struct OpenThermCommandT *self)
{
    char str[FLOATSZ];
    dtostrfd(self->m_results[0].m_float, Settings.flag2.temperature_resolution, str);
    ResponseAppend_P(PSTR("%s"), str);
}

/////////////////////////////////// Specific Floats Rerports to the  /////////////////////////////////////////////////
void sns_opentherm_parse_dhw_setpoint(struct OpenThermCommandT *self, struct OT_BOILER_STATUS_T *boilerStatus, unsigned long response)
{
    self->m_results[0].m_float = OpenTherm::getFloat(response);
    boilerStatus->m_hotWaterSetpoint_read = self->m_results[0].m_float;
}

void sns_opentherm_parse_flame_modulation(struct OpenThermCommandT *self, struct OT_BOILER_STATUS_T *boilerStatus, unsigned long response)
{
    self->m_results[0].m_float = OpenTherm::getFloat(response);
    boilerStatus->m_flame_modulation_read = self->m_results[0].m_float;
}

void sns_opentherm_parse_boiler_temperature(struct OpenThermCommandT *self, struct OT_BOILER_STATUS_T *boilerStatus, unsigned long response)
{
    self->m_results[0].m_float = OpenTherm::getFloat(response);
    boilerStatus->m_boiler_temperature_read = self->m_results[0].m_float;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define SNS_OT_COMMANDS_COUNT (sizeof(sns_opentherm_commands) / sizeof(OpenThermCommand))
int sns_opentherm_current_command = SNS_OT_COMMANDS_COUNT;

unsigned long sns_opentherm_get_next_request(struct OT_BOILER_STATUS_T *boilerStatus)
{
    // get next and loop the command
    if (++sns_opentherm_current_command >= SNS_OT_COMMANDS_COUNT)
    {
        sns_opentherm_current_command = 0;
    }

    struct OpenThermCommandT *cmd = &sns_opentherm_commands[sns_opentherm_current_command];
    // Return error if command known as not supported
    if (cmd->m_flags.notSupported)
    {
        return -1;
    }
    // Retrurn OT compatible request
    return cmd->m_ot_make_request(cmd, boilerStatus);
}

void sns_opentherm_check_retry_request()
{
    if (sns_opentherm_current_command >= SNS_OT_COMMANDS_COUNT)
    {
        return;
    }
    struct OpenThermCommandT *cmd = &sns_opentherm_commands[sns_opentherm_current_command];

    bool canRetry = ++cmd->m_flags.retryCount < 3;
    // In case of last retry and if this command never respond successfully, set notSupported flag
    if (!canRetry && !cmd->m_flags.supported)
    {
        cmd->m_flags.notSupported = true;
        AddLog_P(LOG_LEVEL_ERROR,
                  PSTR("[OTH]: command %s is not supported by the boiler. Last status: %s"),
                  cmd->m_command_name,
                  sns_ot_master->statusToString(sns_ot_master->getLastResponseStatus()));
    }
}

void sns_opentherm_process_success_response(struct OT_BOILER_STATUS_T *boilerStatus, unsigned long response)
{
    if (sns_opentherm_current_command >= SNS_OT_COMMANDS_COUNT)
    {
        return;
    }
    struct OpenThermCommandT *cmd = &sns_opentherm_commands[sns_opentherm_current_command];
    // mark command as supported
    cmd->m_flags.supported = true;

    cmd->m_ot_parse_response(cmd, boilerStatus, response);
}

void sns_opentherm_dump_telemetry()
{
    bool add_coma = false;
    for (int i = 0; i < SNS_OT_COMMANDS_COUNT; ++i)
    {
        struct OpenThermCommandT *cmd = &sns_opentherm_commands[i];
        if (!cmd->m_flags.supported)
        {
            continue;
        }

        ResponseAppend_P(PSTR("%s\"%s\":"), add_coma ? "," : "", cmd->m_command_name);

        cmd->m_ot_appent_telemetry(cmd);

        add_coma = true;
    }
}
#endif