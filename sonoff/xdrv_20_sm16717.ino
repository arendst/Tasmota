/*
  xdrv_20_sm16716.ino - SM16716 RGB led controller support for Sonoff-Tasmota

  Copyright (C) 2019  Gabor Simon

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

#ifdef USE_SM16716
/*********************************************************************************************\
 * SM16716 - Controlling RGB over a synchronous serial line
 *
 * Source: https://community.home-assistant.io/t/cheap-uk-wifi-bulbs-with-tasmota-teardown-help-tywe3s/40508/27
 *
\*********************************************************************************************/

#define D_LOG_SM16716       "SM16716: "
#define XDRV_20             20

#define CLK_USEC            10      // Clock interval in microseconds

enum SM16716_Commands {
  CMND_SM16716_POWER, CMND_SM16716_DIMMER, CMND_SM16716_COLOR };

const char k_SM16716_Commands[] PROGMEM =
  D_CMND_POWER "|" D_CMND_DIMMER "|" D_CMND_COLOR ;

uint8_t sm61716_pin_clk     = 100;
uint8_t sm61716_pin_dat     = 100;

uint8_t sm61716_colour[3]   = {0, 0, 0};

uint8_t sm61716_eff_red     = 0;
uint8_t sm61716_eff_green   = 0;
uint8_t sm61716_eff_blue    = 0;

int8_t sm61716_colour_preset = 0;

/*********************************************************************************************/

void SM16716_SendBit(uint8_t v)
{
  digitalWrite(sm61716_pin_dat, v ? HIGH : LOW);
  digitalWrite(sm61716_pin_clk, HIGH);
  delayMicroseconds(CLK_USEC);
  digitalWrite(sm61716_pin_clk, LOW);
  delayMicroseconds(CLK_USEC);
}

void SM16716_SendByte(uint8_t v)
{
  uint8_t mask;

  for (mask = 0x80; mask; mask >>= 1)
    SM16716_SendBit(v & mask);
}

/*********************************************************************************************/
void SM16716_Append_JSON(void)
{
  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_SM16716 "Append_JSON;"));
  AddLog(LOG_LEVEL_DEBUG);
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"SM16716\":{\"Red\":%d,\"Green\":%d,\"Blue\":%d}"), mqtt_data, sm61716_colour[0], sm61716_colour[1], sm61716_colour[2]);
}

/*********************************************************************************************/
boolean SM16716_Show_State(void)
{
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_COLOR "\":\"%02x%02x%02x\"}"),
      sm61716_colour[0], sm61716_colour[1], sm61716_colour[2]);
  return true;
}

/*********************************************************************************************/
void SM16716_Update()
{
  uint8_t eff_red, eff_green, eff_blue;

  if (Settings.power) { // any bit is ok for us
    eff_red   = (uint16_t)sm61716_colour[0] * Settings.light_dimmer / 100;
    eff_green = (uint16_t)sm61716_colour[1] * Settings.light_dimmer / 100;
    eff_blue  = (uint16_t)sm61716_colour[2] * Settings.light_dimmer / 100;
  }
  else {
    eff_red = eff_green = eff_blue = 0;
  }

  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_SM16716 "Update; pwr=%02x, rgb=%02x%02x%02x, dimmer=%d, eff=%02x%02x%02x"),
      Settings.power, sm61716_colour[0], sm61716_colour[1], sm61716_colour[2], Settings.light_dimmer, eff_red, eff_green, eff_blue);
  AddLog(LOG_LEVEL_DEBUG);

  if ((eff_red != sm61716_eff_red) || (eff_green != sm61716_eff_green) || (eff_blue != sm61716_eff_blue)) {
    sm61716_eff_red = eff_red;
    sm61716_eff_green = eff_green;
    sm61716_eff_blue = eff_blue;

    SM16716_SendBit(1);
    SM16716_SendByte(eff_red);
    SM16716_SendByte(eff_green);
    SM16716_SendByte(eff_blue);
  }
  SM16716_Show_State();
}

/*********************************************************************************************/
boolean SM16716_ModuleSelected(void)
{
  sm61716_pin_clk = pin[GPIO_SM16716_CLK];
  sm61716_pin_dat = pin[GPIO_SM16716_DAT];
  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_SM16716 "ModuleSelected; clk_pin=%d, dat_pin=%d)"), sm61716_pin_clk, sm61716_pin_dat);
  AddLog(LOG_LEVEL_DEBUG);
  return (sm61716_pin_clk < 99) && (sm61716_pin_dat < 99);
}

/*********************************************************************************************/
boolean SM16716_Init(void)
{
  uint8_t t_init;
  if (!SM16716_ModuleSelected())
    return false;

  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_SM16716 "Init;"));

  pinMode(sm61716_pin_clk, OUTPUT);
  digitalWrite(sm61716_pin_clk, LOW);

  pinMode(sm61716_pin_dat, OUTPUT);
  digitalWrite(sm61716_pin_dat, LOW);

  AddLog(LOG_LEVEL_DEBUG);
  for (t_init = 0; t_init < 50; ++t_init)
    SM16716_SendBit(0);

  return true;
}

boolean SM16716_Parse_RRGGBB(const char *data, int data_len) {
  char component[3];
  char *endptr = NULL;
  uint8_t candidate[3];
  int i;

  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_SM16716 "Parse_RRGGBB; data='%s', data_len=%d"), data, data_len);
  AddLog(LOG_LEVEL_DEBUG);

  if (data_len != 6)
    return false;

  component[2] = '\0';

  for (i = 0; i < 3; ++i) {
    component[0] = data[0];
    component[1] = data[1];
    candidate[i] = (uint8_t)strtoul(component, &endptr, 16);
    if (!endptr || *endptr)
      return false;
    data += 2;
  }

  sm61716_colour[0] = candidate[0];
  sm61716_colour[1] = candidate[1];
  sm61716_colour[2] = candidate[2];
  return true;
}

boolean SM16716_Parse_Colour(char *data, int data_len) {
  /* NOTE: Very similar to 'LightColorEntry', but it's not reusable, because 
   * 'light_type' must be PWM, but then it won't parse the colour components.
   * It's indeed more effective to not do the parsing when the current light
   * type couldn't handle it, but enum LightTypes is not to be changed, so
   * I can't implement new light types.
   *
   * If the parsing/handling of light attributes were separated from the
   * technical details of actually sending it to the device, then only the
   * device-specific parts should've been adapted, but unfortunately this is
   * not the case here, so, here we go */

  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_SM16716 "Parse_Colour; data='%s', data_len=%d"), data, data_len);
  AddLog(LOG_LEVEL_DEBUG);

  if (data_len < 3) { // Colour preset
    switch (data[0]) {
      case '+':
        ++sm61716_colour_preset;
        if (sm61716_colour_preset >= MAX_FIXED_COLOR)
          sm61716_colour_preset = 0;
        break;

      case '-':
        --sm61716_colour_preset;
        if (sm61716_colour_preset < 0)
          sm61716_colour_preset = MAX_FIXED_COLOR - 1;
        break;

      default:
        {
          char *endptr = NULL;
          uint8_t candidate = (uint8_t)strtoul(data, &endptr, 10);
          if (!endptr || *endptr || (candidate < 0) || (MAX_FIXED_COLOR <= candidate))
            return false;
          sm61716_colour_preset = candidate;
        }
        break;
    }
    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_SM16716 "Parse_Colour; preset=%d"), sm61716_colour_preset);
    AddLog(LOG_LEVEL_DEBUG);
    memcpy_P(sm61716_colour, &kFixedColor[sm61716_colour_preset], 3);
  }
  else if (data[0] == '#') { // #RRGGBB
    if (!SM16716_Parse_RRGGBB(data + 1, data_len - 1))
      return false;
  }
  else if (SM16716_Parse_RRGGBB(data, data_len)) { // RRGGBB
  }
  else { // rrr,g,bb
    uint8_t candidate[3];
    char *tok, *last, *endptr = NULL;
    int i;

    for (i = 0; i < 3; ++i) {
      tok = strtok_r(data, ",", &last);
      if (!tok)
        return false;
      candidate[i] = (uint8_t)strtoul(tok, &endptr, 0);
      if (!endptr || *endptr)
        return false;
      data = NULL;
    }
    tok = strtok_r(NULL, ",", &last);
    if (tok)
      return false; // junk at the end
    sm61716_colour[0] = candidate[0];
    sm61716_colour[1] = candidate[1];
    sm61716_colour[2] = candidate[2];
  }
  return true;
}

/*********************************************************************************************/
bool SM16716_Command(void)
{
  char command [CMDSZ];
  int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic, k_SM16716_Commands);

  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_SM16716 "Command; topic='%s', data_len=%d, data='%s', code=%d"),
      XdrvMailbox.topic, XdrvMailbox.data_len, XdrvMailbox.data, command_code);
  AddLog(LOG_LEVEL_DEBUG);

  switch (command_code) {
    case CMND_SM16716_POWER:
      SM16716_Update();
      return false; // Don't catch the event, only handle the change

    case CMND_SM16716_DIMMER:
      SM16716_Update();
      return false; // Don't catch the event, only handle the change

    case CMND_SM16716_COLOR:
      if (XdrvMailbox.data_len == 0)
        return SM16716_Show_State();
      if (!SM16716_Parse_Colour(XdrvMailbox.data, XdrvMailbox.data_len))
        return false;
      SM16716_Update();
      return true;
  }
  return false; // Unknown command
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
boolean Xdrv20(byte function)
{
  //snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_SM16716 "Xdrv20; function=%d, index=%d"), function, XdrvMailbox.index);
  //AddLog(LOG_LEVEL_DEBUG);

  switch (function) {
    case FUNC_MODULE_INIT:
      snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_SM16716 "Entry; function=FUNC_MODULE_INIT"));
      AddLog(LOG_LEVEL_DEBUG);
      return SM16716_ModuleSelected();

    case FUNC_INIT:
      snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_SM16716 "Entry; function=FUNC_INIT"));
      AddLog(LOG_LEVEL_DEBUG);
      return SM16716_Init();

    case FUNC_COMMAND:
      snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_SM16716 "Entry; function=FUNC_COMMAND"));
      AddLog(LOG_LEVEL_DEBUG);
      return SM16716_Command();

    case FUNC_SET_POWER:
      snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_SM16716 "Entry; function=FUNC_SET_POWER, index=%02x, payload=%02x"), XdrvMailbox.index, XdrvMailbox.payload);
      AddLog(LOG_LEVEL_DEBUG);
      return false;

    case FUNC_SET_DEVICE_POWER:
      snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_SM16716 "Entry; function=FUNC_SET_DEVICE_POWER, index=%02x, payload=%02x"), XdrvMailbox.index, XdrvMailbox.payload);
      AddLog(LOG_LEVEL_DEBUG);
      return false;

    case FUNC_JSON_APPEND:
      snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_SM16716 "Entry; function=FUNC_JSON_APPEND"));
      AddLog(LOG_LEVEL_DEBUG);
      SM16716_Append_JSON();
      break;
  }
  return false;
}

#endif // USE_SM16716

// vim: set ft=c sw=2 ts=2 et:
