/**
 * esp-knx-ip library for KNX/IP communication on an ESP8266
 * Author: Nico Weichbrodt <envy>
 * License: MIT
 */

#include "esp-knx-ip.h"

void ESPKNXIP::__handle_root()
{
  String m = F("<html><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'>");
#if USE_BOOTSTRAP
  m += F("<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css' integrity='sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm' crossorigin='anonymous'>");
  m += F("<style>.input-group-insert > .input-group-text { border-radius: 0; }</style>");
#endif
  m += F("</head><body><div class='container-fluid'>");
  m += F("<h2>ESP KNX</h2>");

  // Feedback

  if (registered_feedbacks > 0)
  {
    m += F("<h4>Feedback</h4>");
    for (feedback_id_t i = 0; i < registered_feedbacks; ++i)
    {
      if (feedbacks[i].cond && !feedbacks[i].cond())
      {
        continue;
      }
      m += F("<form action='" __FEEDBACK_PATH "' method='POST'>");
      m += F("<div class='row'><div class='col-auto'><div class='input-group'>");
      m += F("<div class='input-group-prepend'><span class='input-group-text'>");
      m += feedbacks[i].name;
      m += F("</span></div>");
      switch (feedbacks[i].type)
      {
        case FEEDBACK_TYPE_INT:
          m += F("<span class='input-group-text'>");
          m += String(*(int32_t *)feedbacks[i].data);
          m += F("</span>");
          break;
        case FEEDBACK_TYPE_FLOAT:
          m += F("<span class='input-group-text'>");
          m += feedbacks[i].options.float_options.prefix;
          m += String(*(float *)feedbacks[i].data, (unsigned int)feedbacks[i].options.float_options.precision);
          m += feedbacks[i].options.float_options.suffix;
          m += F("</span>");
          break;
        case FEEDBACK_TYPE_BOOL:
          m += F("<span class='input-group-text'>");
          m += (*(bool *)feedbacks[i].data) ? F("True") : F("False");
          m += F("</span>");
          break;
        case FEEDBACK_TYPE_ACTION:
          m += F("<input class='form-control' type='hidden' name='id' value='");
          m += i;
          m += F("' /><div class='input-group-append'><button type='submit' class='btn btn-primary'>");
          m += feedbacks[i].options.action_options.btn_text;
          m += F("</button></div>");
          break;
      }
      m += F("</div></div></div>");
      m += F("</form>");
    }
  }

  if (registered_callbacks > 0)
    m += F("<h4>Callbacks</h4>");

  if (registered_callback_assignments > 0)
  {
    for (uint8_t i = 0; i < registered_callback_assignments; ++i)
    {
      // Skip empty slots
      if ((callback_assignments[i].slot_flags & SLOT_FLAGS_USED) == 0)
      {
        continue;
      }
      // Skip disabled callbacks
      if (callbacks[callback_assignments[i].callback_id].cond && !callbacks[callback_assignments[i].callback_id].cond())
      {
        continue;
      }
      address_t &addr = callback_assignments[i].address;
      m += F("<form action='" __DELETE_PATH "' method='POST'>");
      m += F("<div class='row'><div class='col-auto'><div class='input-group'>");
      m += F("<div class='input-group-prepend'><span class='input-group-text'>");
      m += addr.ga.area;
      m += F("/");
      m += addr.ga.line;
      m += F("/");
      m += addr.ga.member;
      m += F("</span>");
      m += F("<span class='input-group-text'>");
      m += callbacks[callback_assignments[i].callback_id].name;
      m += F("</span></div>");
      m += F("<input class='form-control' type='hidden' name='id' value='");
      m += i;
      m += F("' /><div class='input-group-append'><button type='submit' class='btn btn-danger'>Delete</button></div>");
      m += F("</div></div></div>");
      m += F("</form>");
    }
  }

  if (registered_callbacks > 0)
  {
    m += F("<form action='" __REGISTER_PATH "' method='POST'>");
    m += F("<div class='row'><div class='col-auto'><div class='input-group'>");
    m += F("<input class='form-control' type='number' name='area' min='0' max='31'/>");
    m += F("<div class='input-group-insert'><span class='input-group-text'>/</span></div>");
    m += F("<input class='form-control' type='number' name='line' min='0' max='7'/>");
    m += F("<div class='input-group-insert'><span class='input-group-text'>/</span></div>");
    m += F("<input class='form-control' type='number' name='member' min='0' max='255'/>");
    m += F("<div class='input-group-insert'><span class='input-group-text'>-&gt;</span></div>");
    m += F("<select class='form-control' name='cb'>");
    for (callback_id_t i = 0; i < registered_callbacks; ++i)
    {
      // Skip empty slots
      if ((callbacks[i].slot_flags & SLOT_FLAGS_USED) == 0)
      {
        continue;
      }
      // Skip disabled callbacks
      if (callbacks[i].cond && !callbacks[i].cond())
      {
        continue;
      }
      m += F("<option value=\"");
      m += i;
      m += F("\">");
      m += callbacks[i].name;
      m += F("</option>");
    }
    m += F("</select>");
    m += F("<div class='input-group-append'><button type='submit' class='btn btn-primary'>Set</button></div>");
    m += F("</div></div></div>");
    m += F("</form>");
  }

  m += F("<h4>Configuration</h4>");

  // Physical address
  m += F("<form action='" __PHYS_PATH "' method='POST'>");
  m += F("<div class='row'><div class='col-auto'><div class='input-group'>");
  m += F("<div class='input-group-prepend'><span class='input-group-text'>Physical address</span></div>");
  m += F("<input class='form-control' type='number' name='area' min='0' max='15' value='");
  m += physaddr.pa.area;
  m += F("'/>");
  m += F("<div class='input-group-insert'><span class='input-group-text'>.</span></div>");
  m += F("<input class='form-control' type='number' name='line' min='0' max='15' value='");
  m += physaddr.pa.line;
  m += F("'/>");
  m += F("<div class='input-group-insert'><span class='input-group-text'>.</span></div>");
  m += F("<input class='form-control' type='number' name='member' min='0' max='255' value='");
  m += physaddr.pa.member;
  m += F("'/>");
  m += F("<div class='input-group-append'><button type='submit' class='btn btn-primary'>Set</button></div>");
  m += F("</div></div></div>");
  m += F("</form>");

  if (registered_configs > 0)
  {
    for (config_id_t i = 0; i < registered_configs; ++i)
    {
      // Check if this config option has a enable condition and if so check that condition
      if (custom_configs[i].cond && !custom_configs[i].cond())
        continue;

      m += F("<form action='" __CONFIG_PATH "' method='POST'>");
      m += F("<div class='row'><div class='col-auto'><div class='input-group'>");
      m += F("<div class='input-group-prepend'><span class='input-group-text'>");
      m += custom_configs[i].name;
      m += F("</span></div>");

      switch (custom_configs[i].type)
      {
        case CONFIG_TYPE_STRING:
          m += F("<input class='form-control' type='text' name='value' value='");
          m += config_get_string(i);
          m += F("' maxlength='");
          m += custom_configs[i].len - 1; // Subtract \0 byte
          m += F("'/>");
          break;
        case CONFIG_TYPE_INT:
          m += F("<input class='form-control' type='number' name='value' value='");
          m += config_get_int(i);
          m += F("'/>");
          break;
        case CONFIG_TYPE_BOOL:
          m += F("<div class='input-group-insert'><span class='input-group-text'>");
          m += F("<input type='checkbox' name='value' ");
          if (config_get_bool(i))
            m += F("checked ");
          m += F("/>");
          m += F("</span></div>");
          break;
        case CONFIG_TYPE_OPTIONS:
        {
          m += F("<select class='custom-select' name='value'>");
          option_entry_t *cur = custom_configs[i].data.options;
          while (cur->name != nullptr)
          {
            if (config_get_options(i) == cur->value)
            {
              m += F("<option selected value='");
            }
            else
            {
              m += F("<option value='");
            }
            m += cur->value;
            m += F("'>");
            m += String(cur->name);
            m += F("</option>");
            cur++;
          }
          m += F("");
          m += F("</select>");
          break;
        }
        case CONFIG_TYPE_GA:
          address_t a = config_get_ga(i);
          m += F("<input class='form-control' type='number' name='area' min='0' max='31' value='");
          m += a.ga.area;
          m += F("'/>");
          m += F("<div class='input-group-insert'><span class='input-group-text'>/</span></div>");
          m += F("<input class='form-control' type='number' name='line' min='0' max='7' value='");
          m += a.ga.line;
          m += F("'/>");
          m += F("<div class='input-group-insert'><span class='input-group-text'>/</span></div>");
          m += F("<input class='form-control' type='number' name='member' min='0' max='255' value='");
          m += a.ga.member;
          m += F("'/>");
          break;
      }
      m += F("<input type='hidden' name='id' value='");
      m += i;
      m += F("'/>");
      m += F("<div class='input-group-append'><button type='submit' class='btn btn-primary'>Set</button></div>");
      m += F("</div></div></div>");
      m += F("</form>");
    }
  }

#if !(DISABLE_EEPROM_BUTTONS && DISABLE_RESTORE_BUTTON && DISABLE_REBOOT_BUTTON)
  // EEPROM save and restore
  m += F("<div class='row'>");
  // Save to EEPROM
#if !DISABLE_EEPROM_BUTTONS
  m += F("<div class='col-auto'>");
  m += F("<form action='" __EEPROM_PATH "' method='POST'>");
  m += F("<input type='hidden' name='mode' value='1'>");
  m += F("<button type='submit' class='btn btn-success'>Save to EEPROM</button>");
  m += F("</form>");
  m += F("</div>");
  // Restore from EEPROM
  m += F("<div class='col-auto'>");
  m += F("<form action='" __EEPROM_PATH "' method='POST'>");
  m += F("<input type='hidden' name='mode' value='2'>");
  m += F("<button type='submit' class='btn btn-info'>Restore from EEPROM</button>");
  m += F("</form>");
  m += F("</div>");
#endif
#if !DISABLE_RESTORE_BUTTON
  // Load Defaults
  m += F("<div class='col-auto'>");
  m += F("<form action='" __RESTORE_PATH "' method='POST'>");
  m += F("<button type='submit' class='btn btn-warning'>Restore defaults</button>");
  m += F("</form>");
  m += F("</div>");
#endif
#if !DISABLE_REBOOT_BUTTON
  // Reboot
  m += F("<div class='col-auto'>");
  m += F("<form action='" __REBOOT_PATH "' method='POST'>");
  m += F("<button type='submit' class='btn btn-danger'>Reboot</button>");
  m += F("</form>");
  m += F("</div>");
#endif
  m += F("</div>"); // row
#endif

  // End of page
  m += F("</div></body></html>");
  server->send(200, F("text/html"), m);
}

void ESPKNXIP::__handle_register()
{
  DEBUG_PRINTLN(F("Register called"));
  if (server->hasArg(F("area")) && server->hasArg(F("line")) && server->hasArg(F("member")) && server->hasArg(F("cb")))
  {
    uint8_t area = server->arg(F("area")).toInt();
    uint8_t line = server->arg(F("line")).toInt();
    uint8_t member = server->arg(F("member")).toInt();
    callback_id_t cb = (callback_id_t)server->arg(F("cb")).toInt();

    DEBUG_PRINT(F("Got args: "));
    DEBUG_PRINT(area);
    DEBUG_PRINT(F("/"));
    DEBUG_PRINT(line);
    DEBUG_PRINT(F("/"));
    DEBUG_PRINT(member);
    DEBUG_PRINT(F("/"));
    DEBUG_PRINT(cb);
    DEBUG_PRINTLN(F(""));

    if (area > 31 || line > 7)
    {
      DEBUG_PRINTLN(F("Area or Line wrong"));
      goto end;
    }

    if (!__callback_is_id_valid(cb))
    {
      DEBUG_PRINTLN(F("Invalid callback id"));
      goto end;
    }
    address_t ga = {.ga={line, area, member}};
    __callback_register_assignment(ga, cb);
  }
end:
  server->sendHeader(F("Location"),F(__ROOT_PATH));
  server->send(302);
}

void ESPKNXIP::__handle_delete()
{
  DEBUG_PRINTLN(F("Delete called"));
  if (server->hasArg(F("id")))
  {
    callback_assignment_id_t id = (callback_assignment_id_t)server->arg(F("id")).toInt();

    DEBUG_PRINT(F("Got args: "));
    DEBUG_PRINT(id);
    DEBUG_PRINTLN(F(""));

    if (id >= registered_callback_assignments || (callback_assignments[id].slot_flags & SLOT_FLAGS_USED) == 0)
    {
      DEBUG_PRINTLN(F("ID wrong"));
      goto end;
    }

    __callback_delete_assignment(id);
  }
end:
  server->sendHeader(F("Location"),F(__ROOT_PATH));
  server->send(302);
}

void ESPKNXIP::__handle_set()
{
  DEBUG_PRINTLN(F("Set called"));
  if (server->hasArg(F("area")) && server->hasArg(F("line")) && server->hasArg(F("member")))
  {
    uint8_t area = server->arg(F("area")).toInt();
    uint8_t line = server->arg(F("line")).toInt();
    uint8_t member = server->arg(F("member")).toInt();

    DEBUG_PRINT(F("Got args: "));
    DEBUG_PRINT(area);
    DEBUG_PRINT(F("."));
    DEBUG_PRINT(line);
    DEBUG_PRINT(F("."));
    DEBUG_PRINT(member);
    DEBUG_PRINTLN(F(""));

    if (area > 31 || line > 7)
    {
      DEBUG_PRINTLN(F("Area or Line wrong"));
      goto end;
    }

    physaddr.bytes.high = (area << 4) | line;
    physaddr.bytes.low = member;
  }
end:
  server->sendHeader(F("Location"),F(__ROOT_PATH));
  server->send(302);
}

void ESPKNXIP::__handle_config()
{
  DEBUG_PRINTLN(F("Config called"));
  if (server->hasArg(F("id")))
  {
    config_id_t id = server->arg(F("id")).toInt();

    DEBUG_PRINT(F("Got args: "));
    DEBUG_PRINT(id);
    DEBUG_PRINTLN(F(""));

    if (id < 0 || id >= registered_configs)
    {
      DEBUG_PRINTLN(F("ID wrong"));
      goto end;
    }

    switch (custom_configs[id].type)
    {
      case CONFIG_TYPE_STRING:
      {
        String v = server->arg(F("value"));
        if (v.length() >= custom_configs[id].len)
          goto end;
        __config_set_flags(id, CONFIG_FLAGS_VALUE_SET);
        __config_set_string(id, v);
        break;
      }
      case CONFIG_TYPE_INT:
      {
        __config_set_flags(id, CONFIG_FLAGS_VALUE_SET);
        __config_set_int(id, server->arg(F("value")).toInt());
        break;
      }
      case CONFIG_TYPE_BOOL:
      {
        __config_set_flags(id, CONFIG_FLAGS_VALUE_SET);
        __config_set_bool(id, server->arg(F("value")).compareTo(F("on")) == 0);
        break;
      }
      case CONFIG_TYPE_OPTIONS:
      {
        uint8_t val = (uint8_t)server->arg(F("value")).toInt();
        DEBUG_PRINT(F("Value: "));
        DEBUG_PRINTLN(val);
        config_set_options(id, val);
        break;
      }
      case CONFIG_TYPE_GA:
      {
        uint8_t area = server->arg(F("area")).toInt();
        uint8_t line = server->arg(F("line")).toInt();
        uint8_t member = server->arg(F("member")).toInt();
        if (area > 31 || line > 7)
        {
          DEBUG_PRINTLN(F("Area or Line wrong"));
          goto end;
        }
        address_t tmp;
        tmp.bytes.high = (area << 3) | line;
        tmp.bytes.low = member;
        __config_set_flags(id, CONFIG_FLAGS_VALUE_SET);
        __config_set_ga(id, tmp);
        break;
      }
    }
  }
end:
  server->sendHeader(F("Location"),F(__ROOT_PATH));
  server->send(302);
}

void ESPKNXIP::__handle_feedback()
{
DEBUG_PRINTLN(F("Feedback called"));
  if (server->hasArg(F("id")))
  {
    config_id_t id = server->arg(F("id")).toInt();

    DEBUG_PRINT(F("Got args: "));
    DEBUG_PRINT(id);
    DEBUG_PRINTLN(F(""));

    if (id < 0 || id >= registered_feedbacks)
    {
      DEBUG_PRINTLN(F("ID wrong"));
      goto end;
    }

    switch (feedbacks[id].type)
    {
      case FEEDBACK_TYPE_ACTION:
      {
        feedback_action_fptr_t func = (feedback_action_fptr_t)feedbacks[id].data;
        void *arg = feedbacks[id].options.action_options.arg;
        func(arg);
        break;
      }
      default:
        DEBUG_PRINTLN(F("Feedback has no action"));
        break;
    }
  }
end:
  server->sendHeader(F("Location"),F(__ROOT_PATH));
  server->send(302);
}

#if !DISABLE_RESTORE_BUTTONS
void ESPKNXIP::__handle_restore()
{
  DEBUG_PRINTLN(F("Restore called"));
  memcpy(custom_config_data, custom_config_default_data, MAX_CONFIG_SPACE);
end:
  server->sendHeader(F("Location"),F(__ROOT_PATH));
  server->send(302);
}
#endif

#if !DISABLE_REBOOT_BUTTONS
void ESPKNXIP::__handle_reboot()
{
  DEBUG_PRINTLN(F("Rebooting!"));
  server->sendHeader(F("Location"),F(__ROOT_PATH));
  server->send(302);
  delay(1000);
  ESP.restart();
  //while(1);
}
#endif

#if !DISABLE_EEPROM_BUTTONS
void ESPKNXIP::__handle_eeprom()
{
  DEBUG_PRINTLN(F("EEPROM called"));
  if (server->hasArg(F("mode")))
  {
    uint8_t mode = server->arg(F("mode")).toInt();

    DEBUG_PRINT(F("Got args: "));
    DEBUG_PRINT(mode);
    DEBUG_PRINTLN(F(""));

    if (mode == 1)
    {
      // save
      save_to_eeprom();
    }
    else if (mode == 2)
    {
      // restore
      restore_from_eeprom();
    }
  }
end:
  server->sendHeader(F("Location"),F(__ROOT_PATH));
  server->send(302);
}
#endif
