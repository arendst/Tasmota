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

#define XDRV_20             20

// Enable this for debug logging
//#define D_LOG_SM16716       "SM16716: "

enum SM16716_Commands {
  CMND_SM16716_COLOR };

const char k_SM16716_Commands[] PROGMEM =
  D_CMND_COLOR ;

uint8_t sm16716_pin_clk     = 100;
uint8_t sm16716_pin_dat     = 100;

boolean sm16716_is_selected = false;
boolean sm16716_is_on       = false;
uint8_t sm16716_color[3]    = {0, 0, 0};
int8_t sm16716_color_preset = 0;


/*********************************************************************************************/
void SM16716_SendBit(uint8_t v)
{
  /* NOTE:
   * According to the spec sheet, max freq is 30 MHz, that is 16.6 ns per high/low half of the
   * clk square wave. That is less than the overhead of 'digitalWrite' at this clock rate,
   * so no additional delays are needed yet. */

  digitalWrite(sm16716_pin_dat, (v != 0) ? HIGH : LOW);
  //delayMicroseconds(1);
  digitalWrite(sm16716_pin_clk, HIGH);
  //delayMicroseconds(1);
  digitalWrite(sm16716_pin_clk, LOW);
}


/*********************************************************************************************/
void SM16716_SendByte(uint8_t v)
{
  uint8_t mask;

  for (mask = 0x80; mask; mask >>= 1) {
    SM16716_SendBit(v & mask);
  }
}


/*********************************************************************************************/
void SM16716_Append_JSON(void)
{
  snprintf_P(mqtt_data, sizeof(mqtt_data),
      PSTR("%s,\"SM16716\":{\"Red\":%d,\"Green\":%d,\"Blue\":%d}"),
      mqtt_data, sm16716_color[0], sm16716_color[1], sm16716_color[2]);
}


/*********************************************************************************************/
boolean SM16716_Show_State(void)
{
  snprintf_P(mqtt_data, sizeof(mqtt_data),
      PSTR("{\"" D_CMND_COLOR "\":\"%02x%02x%02x\"}"),
      sm16716_color[0], sm16716_color[1], sm16716_color[2]);
  return true;
}


/*********************************************************************************************/
void SM16716_Update()
{
#ifdef D_LOG_SM16716
  snprintf_P(log_data, sizeof(log_data),
      PSTR(D_LOG_SM16716 "Update; pwr=%02x, rgb=%02x%02x%02x"),
      sm16716_is_on, sm16716_color[0], sm16716_color[1], sm16716_color[2]);
  AddLog(LOG_LEVEL_DEBUG);
#endif // D_LOG_SM16716

  // send start bit
  SM16716_SendBit(1);
  // send 24-bit rgb data
  if (sm16716_is_on) {
    SM16716_SendByte(sm16716_color[0]);
    SM16716_SendByte(sm16716_color[1]);
    SM16716_SendByte(sm16716_color[2]);
  }
  else {
    SM16716_SendByte(0);
    SM16716_SendByte(0);
    SM16716_SendByte(0);
  }
  // send a 'do it' pulse
  // (if multiple chips are chained, each one processes the 1st '1rgb' 25-bit block and
  // passes on the rest, right until the one starting with 0)
  SM16716_SendBit(0);
  SM16716_SendByte(0);
  SM16716_SendByte(0);
  SM16716_SendByte(0);
  SM16716_Show_State();
}


/*********************************************************************************************/
boolean SM16716_ModuleSelected(void)
{
  sm16716_pin_clk = pin[GPIO_SM16716_CLK];
  sm16716_pin_dat = pin[GPIO_SM16716_DAT];
#ifdef D_LOG_SM16716
  snprintf_P(log_data, sizeof(log_data),
      PSTR(D_LOG_SM16716 "ModuleSelected; clk_pin=%d, dat_pin=%d)"),
      sm16716_pin_clk, sm16716_pin_dat);
  AddLog(LOG_LEVEL_DEBUG);
#endif // D_LOG_SM16716
  sm16716_is_selected = (sm16716_pin_clk < 99) && (sm16716_pin_dat < 99);
  return sm16716_is_selected;
}


/*********************************************************************************************/
boolean SM16716_Init(void)
{
  uint8_t t_init;

  if (!SM16716_ModuleSelected())
    return false;

  pinMode(sm16716_pin_clk, OUTPUT);
  digitalWrite(sm16716_pin_clk, LOW);

  pinMode(sm16716_pin_dat, OUTPUT);
  digitalWrite(sm16716_pin_dat, LOW);

  for (t_init = 0; t_init < 50; ++t_init)
    SM16716_SendBit(0);

  return true;
}


/*********************************************************************************************/
boolean SM16716_Parse_RRGGBB(const char *data, int data_len) {
  char component[3];
  char *endptr = NULL;
  uint8_t candidate[3];
  int i;

#ifdef D_LOG_SM16716
  snprintf_P(log_data, sizeof(log_data),
      PSTR(D_LOG_SM16716 "Parse_RRGGBB; data='%s', data_len=%d"),
      data, data_len);
  AddLog(LOG_LEVEL_DEBUG);
#endif // D_LOG_SM16716

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

  sm16716_color[0] = candidate[0];
  sm16716_color[1] = candidate[1];
  sm16716_color[2] = candidate[2];
  return true;
}


/*********************************************************************************************/
boolean SM16716_Parse_Color(char *data, int data_len) {
  /* NOTE: Very similar to 'LightColorEntry', but can't reuse it here, because 
   * 'light_type' must be PWM, and then it won't parse the color components. */

#ifdef D_LOG_SM16716
  snprintf_P(log_data, sizeof(log_data),
      PSTR(D_LOG_SM16716 "Parse_Color; data='%s', data_len=%d"),
      data, data_len);
  AddLog(LOG_LEVEL_DEBUG);
#endif // D_LOG_SM16716

  if (data_len < 3) { // a color preset
    switch (data[0]) {
      case '+':
        ++sm16716_color_preset;
        if (sm16716_color_preset >= MAX_FIXED_COLOR)
          sm16716_color_preset = 0;
        break;

      case '-':
        --sm16716_color_preset;
        if (sm16716_color_preset < 0)
          sm16716_color_preset = MAX_FIXED_COLOR - 1;
        break;

      default:
        {
          char *endptr = NULL;
          uint8_t candidate = (uint8_t)strtoul(data, &endptr, 10);
          if (!endptr || *endptr || (candidate < 0) || (MAX_FIXED_COLOR <= candidate))
            return false;
          sm16716_color_preset = candidate;
        }
        break;
    }
#ifdef D_LOG_SM16716
    snprintf_P(log_data, sizeof(log_data),
        PSTR(D_LOG_SM16716 "Parse_Color; preset=%d"),
        sm16716_color_preset);
    AddLog(LOG_LEVEL_DEBUG);
#endif // D_LOG_SM16716
    memcpy_P(sm16716_color, &kFixedColor[sm16716_color_preset], 3);
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
    sm16716_color[0] = candidate[0];
    sm16716_color[1] = candidate[1];
    sm16716_color[2] = candidate[2];
  }
  return true;
}


/*********************************************************************************************/
bool SM16716_Command(void)
{
  char command [CMDSZ];
  int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic, k_SM16716_Commands);

#ifdef D_LOG_SM16716
  snprintf_P(log_data, sizeof(log_data),
      PSTR(D_LOG_SM16716 "Command; topic='%s', data_len=%d, data='%s', code=%d"),
      XdrvMailbox.topic, XdrvMailbox.data_len, XdrvMailbox.data, command_code);
  AddLog(LOG_LEVEL_DEBUG);
#endif // D_LOG_SM16716

  switch (command_code) {
    case CMND_SM16716_COLOR:
      if (XdrvMailbox.data_len == 0)
        return SM16716_Show_State();
      if (!SM16716_Parse_Color(XdrvMailbox.data, XdrvMailbox.data_len))
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
  if (function == FUNC_MODULE_INIT) {
      return SM16716_ModuleSelected();
  }

  if (!sm16716_is_selected)
    return false;

  switch (function) {
    case FUNC_INIT:
#ifdef D_LOG_SM16716
      snprintf_P(log_data, sizeof(log_data),
          PSTR(D_LOG_SM16716 "Entry; function=FUNC_INIT"));
      AddLog(LOG_LEVEL_DEBUG);
#endif
      return SM16716_Init();

    case FUNC_COMMAND:
#ifdef D_LOG_SM16716
      snprintf_P(log_data, sizeof(log_data),
          PSTR(D_LOG_SM16716 "Entry; function=FUNC_COMMAND"));
      AddLog(LOG_LEVEL_DEBUG);
      return SM16716_Command();
#endif

    //case FUNC_SET_POWER:
    case FUNC_SET_DEVICE_POWER:
#ifdef D_LOG_SM16716
      snprintf_P(log_data, sizeof(log_data),
          PSTR(D_LOG_SM16716 "Entry; function=FUNC_SET_DEVICE_POWER, index=%02x, payload=%02x"),
          XdrvMailbox.index, XdrvMailbox.payload);
      AddLog(LOG_LEVEL_DEBUG);
#endif
      sm16716_is_on = (XdrvMailbox.index != 0);
      SM16716_Update();
      return false; // don't catch the event

    case FUNC_JSON_APPEND:
#ifdef D_LOG_SM16716
      snprintf_P(log_data, sizeof(log_data),
          PSTR(D_LOG_SM16716 "Entry; function=FUNC_JSON_APPEND"));
      AddLog(LOG_LEVEL_DEBUG);
#endif
      SM16716_Append_JSON();
      break;
  }
  return false;
}

#endif // USE_SM16716
// vim: set ft=c sw=2 ts=2 et:
