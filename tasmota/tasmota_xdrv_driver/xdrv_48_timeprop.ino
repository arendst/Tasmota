#ifdef USE_TIMEPROP

#define XDRV_48 48

#ifndef TIMEPROP_FALLBACKTIME_MULTIPLIER
#define TIMEPROP_FALLBACKTIME_MULTIPLIER 2
#endif

const char kTimepropCommands[] PROGMEM = "|" D_CMND_TIMEPROP_SET "|" D_CMND_TIMEPROP_ENABLE "|" D_CMND_TIMEPROP_CYCLE_LENGTH "|" D_CMND_TIMEPROP_COUNT "|" D_CMND_TIMEPROP_LOAD_TYPE "|" D_CMND_TIMEPROP_FALLBACK_AFTER "|" D_CMND_TIMEPROP_FALLBACK_VALUE;

void (*const TimepropCommand[])(void) PROGMEM = {
    &CmndTimepropSet,
    &CmndTimePropEnable,
    &CmndTimePropCycleLength,
    &CmndTimePropCount,
    &CmndTimePropLoadType,
    &CmndTimePropFallbackAfter,
    &CmndTimePropFallbackValue,
};

// local "copy" of configuration
struct TIMEPROP
{
  bool enabled = false;
  uint8_t cycle_length = 0;
  uint8_t count = 1;
  bool load_type = false;
  uint8_t fallback_time = 0;
  uint8_t fallback_value = 0;
} Timeprop;

uint8_t *TimepropValues;
uint16_t *TimepropStartTimes;
uint16_t *TimepropSecondsLeft;

uint16_t cycle_position = 0;
uint16_t cycle_length_seconds = 0;

uint32_t seconds_since_set = 0;

/*********************************************************************************************\
 * WebUI
\*********************************************************************************************/
#define WEB_HANDLE_TIMEPROP "s48"

const char HTTP_BTN_MENU_TIMEPROP[] PROGMEM = "<p><form action='" WEB_HANDLE_TIMEPROP "' method='get'><button>" D_CONFIGURE_TIMEPROP "</button></form></p>";

const char HTTP_FORM_TIMEPROPSTRT[] PROGMEM =
    "<fieldset><legend><b>&nbsp; " D_TIMEPROP_PARAMETERS " &nbsp;</b></legend>"
    "<form method='get' action='" WEB_HANDLE_TIMEPROP "'>";

const char HTTP_FORM_TIMEPROP_ENABLE[] PROGMEM =
    "<p><label><input id='tpe' type='checkbox' %s><b>" D_TIMEPROP_ENABLE "</b></label><br>";

const char HTTP_FORM_TIMEPROP_CYCLELENGTH[] PROGMEM =
    "<p><b>" D_TIMEPROP_CYCLELENGTH "</b> (15)<br>"
    "<select id=\"tpl\" name=\"tpl\">"
    "<option %s value=\"0\">5 " D_UNIT_MINUTE "</option>"
    "<option %s value=\"1\">10 " D_UNIT_MINUTE " </option>"
    "<option %s value=\"2\">15 " D_UNIT_MINUTE " </option>"
    "<option %s value=\"3\">20 " D_UNIT_MINUTE " </option>"
    "</select>"
    "</p>";

const char HTTP_FORM_TIMEPROP_COUNT_BEGIN[] PROGMEM =
    "<p><b>" D_TIMEPROP_COUNT "</b> (5)<br>"
    "<select id=\"tpc\" name=\"tpc\">";

const char HTTP_FORM_TIMEPROP_COUNT_LINE[] PROGMEM =
    "<option %s value=\"%d\">%d</option>";

const char HTTP_FORM_TIMEPROP_COUNT_END[] PROGMEM =
    "</select>"
    "</p>";

const char HTTP_FORM_TIMEPROP_LOADTYPE[] PROGMEM =
    "<p><b>" D_TIMEPROP_LOADTYPE "</b> (" D_TIMEPROP_LOADTYPE_DISTRIBUTION ")<br>"
    "<select id=\"tpt\" name=\"tpt\">"
    "<option %s value=\"0\">" D_TIMEPROP_LOADTYPE_DISTRIBUTION "</option>"
    "<option %s value=\"1\">" D_TIMEPROP_LOADTYPE_COLLECTION " </option>"
    "</select>"
    "</p>";

const char HTTP_FORM_TIMEPROP_FALLBACK[] PROGMEM =
    "<p><b>" D_TIMEPROP_FALLBACK_AFTER "</b> (0) " D_UNIT_HOUR "<br><input id='tpf' placeholder='0' value='%d'></p>"
    "<p><b>" D_TIMEPROP_FALLBACK_VALUE "</b> (0) (0-100)<br><input id='tpv' placeholder='0' value='%d'></p>";

void HandleTimepropConfiguration(void)
{
  if (!HttpCheckPriviledgedAccess())
  {
    return;
  }

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_CONFIGURE_TIMEPROP));

  if (Webserver->hasArg(F("save")))
  {
    TimePropSaveSettings();
    WebRestart(1);
    return;
  }

  WSContentStart_P(PSTR(D_CONFIGURE_TIMEPROP));

  WSContentSendStyle();

  WSContentSend_P(HTTP_FORM_TIMEPROPSTRT);

  WSContentSend_P(HTTP_FORM_TIMEPROP_ENABLE, Timeprop.enabled ? PSTR(" checked") : "");
  WSContentSend_P(HTTP_FORM_TIMEPROP_CYCLELENGTH,
                  Timeprop.cycle_length == 0 ? PSTR("selected=\"\"") : "",
                  Timeprop.cycle_length == 1 ? PSTR("selected=\"\"") : "",
                  Timeprop.cycle_length == 2 ? PSTR("selected=\"\"") : "",
                  Timeprop.cycle_length == 3 ? PSTR("selected=\"\"") : "");

  WSContentSend_P(HTTP_FORM_TIMEPROP_COUNT_BEGIN);
  for (uint8_t i = 0; i < MAX_RELAYS; i++)
  {
    WSContentSend_P(HTTP_FORM_TIMEPROP_COUNT_LINE, Timeprop.count == i + 1 ? PSTR("selected=\"\"") : "", i + 1, i + 1);
  }
  WSContentSend_P(HTTP_FORM_TIMEPROP_COUNT_END);

  WSContentSend_P(HTTP_FORM_TIMEPROP_LOADTYPE,
                  !Timeprop.load_type ? PSTR("selected=\"\"") : "",
                  Timeprop.load_type ? PSTR("selected=\"\"") : "");

  WSContentSend_P(HTTP_FORM_TIMEPROP_FALLBACK, Timeprop.fallback_time, Timeprop.fallback_value);

  WSContentSend_P(HTTP_FORM_END);
  WSContentSpaceButton(BUTTON_CONFIGURATION);

  WSContentStop();
}

void TimePropSaveSettings(void)
{
  String cmnd = F(D_CMND_BACKLOG "0 ");
  if (Webserver->hasArg("tpe"))
  {
    cmnd += AddWebCommand(PSTR(D_CMND_TIMEPROP_ENABLE), PSTR("1"), PSTR("1"));
  }
  else
  {
    cmnd += AddWebCommand(PSTR(D_CMND_TIMEPROP_ENABLE), PSTR("0"), PSTR("0"));
  }
  cmnd += AddWebCommand(PSTR(D_CMND_TIMEPROP_CYCLE_LENGTH), PSTR("tpl"), PSTR("2"));
  cmnd += AddWebCommand(PSTR(D_CMND_TIMEPROP_COUNT), PSTR("tpc"), PSTR("5"));
  cmnd += AddWebCommand(PSTR(D_CMND_TIMEPROP_LOAD_TYPE), PSTR("tpt"), PSTR("0"));
  cmnd += AddWebCommand(PSTR(D_CMND_TIMEPROP_FALLBACK_AFTER), PSTR("tpf"), PSTR("0"));
  cmnd += AddWebCommand(PSTR(D_CMND_TIMEPROP_FALLBACK_VALUE), PSTR("tpv"), PSTR("0"));
  ExecuteWebCommand((char *)cmnd.c_str());
}

/*********************************************************************************************\
 * Internal
\*********************************************************************************************/
// takes percent value and returns the 3 bit value we use to store in config
uint8_t reduceFallbackValue(uint8_t percentValue)
{
  float fourBitValue = (float)percentValue * 7.0f / 100.0f;
  return round(fourBitValue);
}

// takes 4bit value and returns the percent value
uint8_t expandFallbackValue(uint8_t fourBitValue)
{
  float percentValue = (float)fourBitValue * 100.0f / 7.0f;
  return round(percentValue);
}

// takes percent value and returns the 3 bit value we use to store in config
uint8_t reduceFallbackTime(uint8_t hourValue)
{
  uint8_t fourBitValue = hourValue / TIMEPROP_FALLBACKTIME_MULTIPLIER;

  if (fourBitValue > 15)
  {
    fourBitValue = 15;
  }

  return fourBitValue;
}

// takes 4bit value and returns the percent value
uint8_t expandFallbackTime(uint8_t fourBitValue)
{
  uint8_t hourValue = fourBitValue * TIMEPROP_FALLBACKTIME_MULTIPLIER;
  return hourValue;
}

/*********************************************************************************************\
 * Init
\*********************************************************************************************/
void TimepropInit(void)
{
  LoadPersistentSettings();

  AllocateTimepropValues();
}

void LoadPersistentSettings(void)
{
  Timeprop.enabled = Settings->timeprop_cfg.enable;
  Timeprop.cycle_length = Settings->timeprop_cfg.cycle_length;
  Timeprop.count = Settings->timeprop_cfg.count + 1;
  Timeprop.load_type = Settings->timeprop_cfg.load_type;
  Timeprop.fallback_time = expandFallbackTime(Settings->timeprop_cfg.fallback_time);
  Timeprop.fallback_value = expandFallbackValue(Settings->timeprop_cfg.fallback_value);
}

void AllocateTimepropValues(void)
{
  if (Timeprop.enabled)
  {
    free(TimepropValues);
    TimepropValues = (uint8_t *)malloc(Timeprop.count * sizeof *TimepropValues);

    free(TimepropStartTimes);
    TimepropStartTimes = (uint16_t *)malloc(Timeprop.count * sizeof *TimepropStartTimes);

    free(TimepropSecondsLeft);
    TimepropSecondsLeft = (uint16_t *)malloc(Timeprop.count * sizeof *TimepropSecondsLeft);

    cycle_length_seconds = (Timeprop.cycle_length + 1) * 5 * 60;

    for (uint8_t i = 0; i < Timeprop.count; i++)
    {
      TimepropValues[i] = 0;
      TimepropSecondsLeft[i] = 0;

      if (Timeprop.load_type)
      {
        // load collection
        TimepropStartTimes[i] = 0;
      }
      else
      {
        // load distribution
        TimepropStartTimes[i] = (cycle_length_seconds / Timeprop.count) * i;
      }
    }
  }
}
/*********************************************************************************************\
 * Commands
\*********************************************************************************************/
void CmndTimepropSet(void)
{
  if (!Timeprop.enabled)
  {
    AddLog(LOG_LEVEL_ERROR, PSTR("TPR: Not enabled"));
    return;
  }

  if (XdrvMailbox.index < 1 || XdrvMailbox.index > Timeprop.count)
  {
    return;
  }

  if (XdrvMailbox.data_len > 0)
  {
    char sub_string[XdrvMailbox.data_len];

    uint32_t incoming_value = atoi(subStr(sub_string, XdrvMailbox.data, ",", 1));
    if (incoming_value < 0 || incoming_value > 100)
    {
      return;
    }

    seconds_since_set = 0; // reset fallback counter

    TimepropValues[XdrvMailbox.index - 1] = incoming_value;
  }

  ResponseCmndIdxNumber(TimepropValues[XdrvMailbox.index - 1]);
}

void CmndTimePropEnable(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1))
  {
    Timeprop.enabled = XdrvMailbox.payload;

    Settings->timeprop_cfg.enable = Timeprop.enabled;
    SettingsSave(0);
  }

  AllocateTimepropValues();

  ResponseCmndNumber(Timeprop.enabled);
}

void CmndTimePropCycleLength(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3))
  {
    Timeprop.cycle_length = XdrvMailbox.payload;

    Settings->timeprop_cfg.cycle_length = Timeprop.cycle_length;
    SettingsSave(0);
  }
  ResponseCmndNumber(Timeprop.cycle_length);
}

void CmndTimePropCount(void)
{
  if ((XdrvMailbox.payload >= 1) && (XdrvMailbox.payload <= MAX_RELAYS))
  {
    Timeprop.count = XdrvMailbox.payload;

    Settings->timeprop_cfg.count = Timeprop.count - 1;
    SettingsSave(0);
  }

  AllocateTimepropValues();

  ResponseCmndNumber(Timeprop.count);
}

void CmndTimePropLoadType(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1))
  {
    Timeprop.load_type = XdrvMailbox.payload;

    Settings->timeprop_cfg.load_type = Timeprop.load_type;
    SettingsSave(0);
  }

  AllocateTimepropValues();

  ResponseCmndNumber(Timeprop.load_type);
}

void CmndTimePropFallbackAfter(void)
{
  if (XdrvMailbox.payload >= 0)
  {

    Settings->timeprop_cfg.fallback_time = reduceFallbackTime(XdrvMailbox.payload);
    Timeprop.fallback_time = expandFallbackTime(Settings->timeprop_cfg.fallback_time);

    SettingsSave(0);
  }
  ResponseCmndNumber(Timeprop.fallback_time);
}

void CmndTimePropFallbackValue(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100))
  {

    Settings->timeprop_cfg.fallback_value = reduceFallbackValue(XdrvMailbox.payload);
    Timeprop.fallback_value = expandFallbackValue(Settings->timeprop_cfg.fallback_value);

    SettingsSave(0);
  }
  ResponseCmndNumber(Timeprop.fallback_value);
}

/*********************************************************************************************\
 * Periodic
\*********************************************************************************************/
// TODO wenn fallback aufgeloest wird bleiben die anderen alle auf fallback value stehen.
// wollen wir das? aktuell fühlt sich das richtig an
void TimepropEverySecond(void)
{
  if (!Timeprop.enabled)
  {
    return;
  }

  if (Timeprop.fallback_time > 0)
  {
    if (seconds_since_set == Timeprop.fallback_time * 60 * 60)
    {
      AddLog(LOG_LEVEL_INFO, PSTR("TPR: fallback reached %d"), Timeprop.fallback_value);
      for (uint8_t i = 0; i < Timeprop.count; i++)
      {
        TimepropValues[i] = Timeprop.fallback_value;
      }
      seconds_since_set++; // increase so that we do it only once
    }
    else
    {
      seconds_since_set++; // increase fallback check
    }
  }

  for (uint8_t i = 0; i < Timeprop.count; i++)
  {
    if (TimepropValues[i] == 0)
    {
      continue;
    }

    if (TimepropStartTimes[i] == cycle_position)
    {
      AddLog(LOG_LEVEL_DEBUG, PSTR("TPR: On %d. STart at: %d for %d"), i, TimepropStartTimes[i], GetOpenSeconds(i, cycle_length_seconds));
      TimepropSecondsLeft[i] = GetOpenSeconds(i, cycle_length_seconds);
      AddLog(LOG_LEVEL_INFO, PSTR("TPR: Trigger Power On %d"), i + 1);
      ExecuteCommandPower(i + 1, POWER_ON, SRC_IGNORE);
    }

    if (TimepropSecondsLeft[i] == 1)
    {
      AddLog(LOG_LEVEL_INFO, PSTR("TPR: Trigger Power Off %d"), i + 1);
      ExecuteCommandPower(i + 1, POWER_OFF, SRC_IGNORE);
    }

    if (TimepropSecondsLeft[i] > 0)
    {
      TimepropSecondsLeft[i]--;
    }
  }

  // maintain cycle
  if (cycle_position >= cycle_length_seconds - 1)
  {
    cycle_position = 0;
  }
  else
  {
    cycle_position++;
  }
}

uint16_t GetOpenSeconds(uint8_t i, uint16_t cycle_length_seconds)
{
  if (TimepropValues[i] == 100)
  {
    // special for 100%. return a slightly longer value to prevent a slight off/on.
    // that will make no difference but reduce part lifetime
    return cycle_length_seconds + 2;
  }
  return cycle_length_seconds * TimepropValues[i] / 100;
}

bool Xdrv48(uint32_t function)
{
  bool result = false;

  switch (function)
  {
  case FUNC_INIT:
    TimepropInit();
    break;
  case FUNC_EVERY_SECOND:
    TimepropEverySecond();
    break;
#ifdef USE_WEBSERVER
  case FUNC_WEB_ADD_BUTTON:
    WSContentSend_P(HTTP_BTN_MENU_TIMEPROP);
    break;
  case FUNC_WEB_ADD_HANDLER:
    WebServer_on(PSTR("/" WEB_HANDLE_TIMEPROP), HandleTimepropConfiguration);
    break;
#endif // USE_WEBSERVER
  case FUNC_COMMAND:
    result = DecodeCommand(kTimepropCommands, TimepropCommand);
    break;
  }

  return result;
}

#endif // USE_TIMEPROP
