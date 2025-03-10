#ifdef USE_TIMEPROP

#define XDRV_48 48

// #include "Timeprop.h"

const char kTimepropCommands[] PROGMEM = "|" D_CMND_TIMEPROP_ENABLE "|" D_CMND_TIMEPROP_CYCLE_LENGTH "|" D_CMND_TIMEPROP_NUM_PROPS "|" D_CMND_TIMEPROP_LOAD_TYPE "|" D_CMND_TIMEPROP_FALLBACK_AFTER "|" D_CMND_TIMEPROP_FALLBACK_VALUE;

void (*const TimepropCommand[])(void) PROGMEM = {
    &CmndTimePropEnable,
    &CmndTimePropCycleLength,
    &CmndTimePropNumProps,
    &CmndTimePropLoadType,
    &CmndTimePropFallbackAfter,
    &CmndTimePropFallbackValue,
};

struct TIMEPROP
{
  bool enabled = true;
  uint8_t cycle_length = 2;
  uint8_t num_timeprops = 3;
  bool load_type = false;
  uint8_t fallback_time = 30;
  uint8_t fallback_value = 11;
} Timeprop;

/*********************************************************************************************\
 * WebUI
\*********************************************************************************************/
#define WEB_HANDLE_TIMEPROP "s48"

const char HTTP_BTN_MENU_TIMEPROP[] PROGMEM = "<p><form action='" WEB_HANDLE_TIMEPROP "' method='get'><button>" D_CONFIGURE_TIMEPROP "</button></form></p>";

const char HTTP_FORM_TIMEPROPSTRT[] PROGMEM =
    "<fieldset><legend><b>&nbsp; " D_TIMEPROP_PARAMETERS " &nbsp;</b></legend>"
    "<form method='get' action='" WEB_HANDLE_TIMEPROP "'>";

const char HTTP_FORM_TIMEPROP_ENABLE[] PROGMEM =
    "<p><label><input id='tp_enable' type='checkbox' %s><b>" D_TIMEPROP_ENABLE "</b></label><br>";

const char HTTP_FORM_TIMEPROP_CYCLELENGTH[] PROGMEM =
    "<p><b>" D_TIMEPROP_CYCLELENGTH "</b> (15)<br>"
    "<select id=\"tp_cycle_length\" name=\"tp_cycle_Length\">"
    "<option %s value=\"0\">5 " D_UNIT_MINUTE "</option>"
    "<option %s value=\"1\">10 " D_UNIT_MINUTE " </option>"
    "<option %s value=\"2\">15 " D_UNIT_MINUTE " </option>"
    "<option %s value=\"3\">20 " D_UNIT_MINUTE " </option>"
    "</select>"
    "</p>";

const char HTTP_FORM_TIMEPROP_NUMPROPS[] PROGMEM =
    "<p><b>" D_TIMEPROP_NUMPROPS "</b> (5)<br>"
    "<select id=\"tp_num_props\" name=\"tp_num_props\">"
    "<option %s value=\"0\">1</option>"
    "<option %s value=\"1\">2</option>"
    "<option %s value=\"2\">3</option>"
    "<option %s value=\"3\">4</option>"
    "<option %s value=\"4\">5</option>"
    "<option %s value=\"5\">6</option>"
    "<option %s value=\"6\">7</option>"
    "<option %s value=\"7\">8</option>"
    "</select>"
    "</p>";

const char HTTP_FORM_TIMEPROP_LOADTYPE[] PROGMEM =
    "<p><b>" D_TIMEPROP_LOADTYPE "</b> (" D_TIMEPROP_LOADTYPE_DISTRIBUTION ")<br>"
    "<select id=\"tp_load_type\" name=\"tp_load_type\">"
    "<option %s value=\"0\">" D_TIMEPROP_LOADTYPE_DISTRIBUTION "</option>"
    "<option %s value=\"1\">" D_TIMEPROP_LOADTYPE_COLLECTION " </option>"
    "</select>"
    "</p>";

const char HTTP_FORM_TIMEPROP_FALLBACK[] PROGMEM =
    "<p><b>" D_TIMEPROP_FALLBACK_AFTER "</b> (0) " D_UNIT_HOUR "<br><input id='tp_fallback_after' placeholder='0' value='%d'></p>"
    "<p><b>" D_TIMEPROP_FALLBACK_VALUE "</b> (0) (0-100)<br><input id='tp_fallback_value' placeholder='0' value='%d'></p>";

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
    // WebRestart(1); TODO: Enable
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
  WSContentSend_P(HTTP_FORM_TIMEPROP_NUMPROPS,
                  Timeprop.num_timeprops == 0 ? PSTR("selected=\"\"") : "",
                  Timeprop.num_timeprops == 1 ? PSTR("selected=\"\"") : "",
                  Timeprop.num_timeprops == 2 ? PSTR("selected=\"\"") : "",
                  Timeprop.num_timeprops == 3 ? PSTR("selected=\"\"") : "",
                  Timeprop.num_timeprops == 4 ? PSTR("selected=\"\"") : "",
                  Timeprop.num_timeprops == 5 ? PSTR("selected=\"\"") : "",
                  Timeprop.num_timeprops == 6 ? PSTR("selected=\"\"") : "",
                  Timeprop.num_timeprops == 7 ? PSTR("selected=\"\"") : "");
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
  cmnd += AddWebCommand(PSTR(D_CMND_TIMEPROP_ENABLE), PSTR("tp_enable"), PSTR("1"));
  cmnd += AddWebCommand(PSTR(D_CMND_TIMEPROP_CYCLE_LENGTH), PSTR("tp_cycle_length"), PSTR("2"));
  cmnd += AddWebCommand(PSTR(D_CMND_TIMEPROP_NUM_PROPS), PSTR("tp_num_props"), PSTR("5"));
  cmnd += AddWebCommand(PSTR(D_CMND_TIMEPROP_LOAD_TYPE), PSTR("tp_load_type"), PSTR("0"));
  cmnd += AddWebCommand(PSTR(D_CMND_TIMEPROP_FALLBACK_AFTER), PSTR("tp_fallback_after"), PSTR("0"));
  cmnd += AddWebCommand(PSTR(D_CMND_TIMEPROP_FALLBACK_VALUE), PSTR("tp_fallback_value"), PSTR("0"));
  ExecuteWebCommand((char *)cmnd.c_str());
}

void CmndTimePropEnable(void)
{
  AddLog(LOG_LEVEL_INFO, PSTR("CmndTimePropEnable"));
}
void CmndTimePropCycleLength(void)
{
  AddLog(LOG_LEVEL_INFO, PSTR("CmndTimePropCycleLength"));
}
void CmndTimePropNumProps(void)
{
  AddLog(LOG_LEVEL_INFO, PSTR("CmndTimePropNumProps"));
}
void CmndTimePropLoadType(void)
{
  AddLog(LOG_LEVEL_INFO, PSTR("CmndTimePropLoadType"));
}
void CmndTimePropFallbackAfter(void)
{
  AddLog(LOG_LEVEL_INFO, PSTR("CmndTimePropFallbackAfter"));
}
void CmndTimePropFallbackValue(void)
{
  AddLog(LOG_LEVEL_INFO, PSTR("CmndTimePropFallbackValue"));
}

bool Xdrv48(uint32_t function)
{
  bool result = false;

  switch (function)
  {
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
