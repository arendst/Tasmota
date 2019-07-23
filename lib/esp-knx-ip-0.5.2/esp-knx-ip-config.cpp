/**
 * esp-knx-ip library for KNX/IP communication on an ESP8266
 * Author: Nico Weichbrodt <envy>
 * License: MIT
 */

#include "esp-knx-ip.h"

/**
 * Physical address functions
 */

void ESPKNXIP::physical_address_set(address_t const &addr)
{
  physaddr = addr;
}

address_t ESPKNXIP::physical_address_get()
{
  return physaddr;
}

/**
 * Configuration functions start here
 */
config_id_t ESPKNXIP::config_register_string(String name, uint8_t len, String _default, enable_condition_t cond)
{
  if (registered_configs >= MAX_CONFIGS)
    return -1;

  if (_default.length() >= len)
    return -1;

  config_id_t id = registered_configs;

  custom_configs[id].name = name;
  custom_configs[id].type = CONFIG_TYPE_STRING;
  custom_configs[id].len = sizeof(uint8_t) + len;
  custom_configs[id].cond = cond;
  if (id == 0)
    custom_configs[id].offset = 0;
  else
    custom_configs[id].offset = custom_configs[id - 1].offset + custom_configs[id - 1].len;

  __config_set_string(id, _default);

  registered_configs++;

  DEBUG_PRINT("Registered config >");
  DEBUG_PRINT(name);
  DEBUG_PRINT("< @ ");
  DEBUG_PRINT(id);
  DEBUG_PRINT("/string[");
  DEBUG_PRINT(custom_configs[id].offset);
  DEBUG_PRINT("+");
  DEBUG_PRINT(custom_configs[id].len);
  DEBUG_PRINTLN("]");

  return id;
}

config_id_t ESPKNXIP::config_register_int(String name, int32_t _default, enable_condition_t cond)
{
  if (registered_configs >= MAX_CONFIGS)
    return -1;

  config_id_t id = registered_configs;

  custom_configs[id].name = name;
  custom_configs[id].type = CONFIG_TYPE_INT;
  custom_configs[id].len = sizeof(uint8_t) + sizeof(int32_t);
  custom_configs[id].cond = cond;
  if (id == 0)
    custom_configs[id].offset = 0;
  else
    custom_configs[id].offset = custom_configs[id - 1].offset + custom_configs[id - 1].len;

  __config_set_int(id, _default);

  registered_configs++;

  DEBUG_PRINT("Registered config >");
  DEBUG_PRINT(name);
  DEBUG_PRINT("< @ ");
  DEBUG_PRINT(id);
  DEBUG_PRINT("/int[");
  DEBUG_PRINT(custom_configs[id].offset);
  DEBUG_PRINT("+");
  DEBUG_PRINT(custom_configs[id].len);
  DEBUG_PRINTLN("]");

  return id;
}

config_id_t ESPKNXIP::config_register_bool(String name, bool _default, enable_condition_t cond)
{
  if (registered_configs >= MAX_CONFIGS)
    return -1;

  config_id_t id = registered_configs;

  custom_configs[id].name = name;
  custom_configs[id].type = CONFIG_TYPE_BOOL;
  custom_configs[id].len = sizeof(uint8_t) + sizeof(uint8_t);
  custom_configs[id].cond = cond;
  if (id == 0)
    custom_configs[id].offset = 0;
  else
    custom_configs[id].offset = custom_configs[id - 1].offset + custom_configs[id - 1].len;

  __config_set_bool(id, _default);

  registered_configs++;

  DEBUG_PRINT("Registered config >");
  DEBUG_PRINT(name);
  DEBUG_PRINT("< @ ");
  DEBUG_PRINT(id);
  DEBUG_PRINT("/bool[");
  DEBUG_PRINT(custom_configs[id].offset);
  DEBUG_PRINT("+");
  DEBUG_PRINT(custom_configs[id].len);
  DEBUG_PRINTLN("]");

  return id;
}

config_id_t ESPKNXIP::config_register_options(String name, option_entry_t *options, uint8_t _default, enable_condition_t cond)
{
  if (registered_configs >= MAX_CONFIGS)
    return -1;

  if (options == nullptr || options->name == nullptr)
    return -1;

  config_id_t id = registered_configs;

  custom_configs[id].name = name;
  custom_configs[id].type = CONFIG_TYPE_OPTIONS;
  custom_configs[id].len = sizeof(uint8_t) + sizeof(uint8_t);
  custom_configs[id].cond = cond;
  if (id == 0)
    custom_configs[id].offset = 0;
  else
    custom_configs[id].offset = custom_configs[id - 1].offset + custom_configs[id - 1].len;

  custom_configs[id].data.options = options;

  __config_set_options(id, _default);

  registered_configs++;

  DEBUG_PRINT("Registered config >");
  DEBUG_PRINT(name);
  DEBUG_PRINT("< @ ");
  DEBUG_PRINT(id);
  DEBUG_PRINT("/opt[");
  DEBUG_PRINT(custom_configs[id].offset);
  DEBUG_PRINT("+");
  DEBUG_PRINT(custom_configs[id].len);
  DEBUG_PRINTLN("]");

  return id;
}

config_id_t ESPKNXIP::config_register_ga(String name, enable_condition_t cond)
{
  if (registered_configs >= MAX_CONFIGS)
    return -1;

  config_id_t id = registered_configs;

  custom_configs[id].name = name;
  custom_configs[id].type = CONFIG_TYPE_GA;
  custom_configs[id].len = sizeof(uint8_t) + sizeof(address_t);
  custom_configs[id].cond = cond;
  if (id == 0)
    custom_configs[id].offset = 0;
  else
    custom_configs[id].offset = custom_configs[id - 1].offset + custom_configs[id - 1].len;

  address_t t;
  t.value = 0;
  __config_set_ga(id, t);

  registered_configs++;

  DEBUG_PRINT("Registered config >");
  DEBUG_PRINT(name);
  DEBUG_PRINT("< @ ");
  DEBUG_PRINT(id);
  DEBUG_PRINT("/ga[");
  DEBUG_PRINT(custom_configs[id].offset);
  DEBUG_PRINT("+");
  DEBUG_PRINT(custom_configs[id].len);
  DEBUG_PRINTLN("]");

  return id;
}

void ESPKNXIP::__config_set_flags(config_id_t id, config_flags_t flags)
{
  DEBUG_PRINT("Setting flag @ ");
  DEBUG_PRINT(custom_configs[id].offset);
  DEBUG_PRINT(" to ");
  DEBUG_PRINT(custom_config_data[custom_configs[id].offset], BIN);
  DEBUG_PRINT(" | ");
  DEBUG_PRINT(flags, BIN);
  custom_config_data[custom_configs[id].offset] |= (uint8_t)flags;
  DEBUG_PRINT(" = ");
  DEBUG_PRINTLN(custom_config_data[custom_configs[id].offset], BIN);
}

void ESPKNXIP::config_set_string(config_id_t id, String val)
{
  if (id >= registered_configs)
    return;
  if (custom_configs[id].type != CONFIG_TYPE_STRING)
    return;
  if (val.length() >= custom_configs[id].len)
    return;
  __config_set_flags(id, CONFIG_FLAGS_VALUE_SET);
  __config_set_string(id, val);
}

void ESPKNXIP::__config_set_string(config_id_t id, String &val)
{
  memcpy(&custom_config_data[custom_configs[id].offset + sizeof(uint8_t)], val.c_str(), val.length()+1);
}

void ESPKNXIP::config_set_int(config_id_t id, int32_t val)
{
  if (id >= registered_configs)
    return;
  if (custom_configs[id].type != CONFIG_TYPE_INT)
    return;
  __config_set_flags(id, CONFIG_FLAGS_VALUE_SET);
  __config_set_int(id, val);
}

void ESPKNXIP::__config_set_int(config_id_t id, int32_t val)
{
  // This does not work for some reason:
  // Could be due to pointer alignment
  //int32_t *v = (int32_t *)(custom_config_data + custom_configs[id].offset);
  //*v = val;
  custom_config_data[custom_configs[id].offset + sizeof(uint8_t) + 0] = (uint8_t)((val & 0xFF000000) >> 24);
  custom_config_data[custom_configs[id].offset + sizeof(uint8_t) + 1] = (uint8_t)((val & 0x00FF0000) >> 16);
  custom_config_data[custom_configs[id].offset + sizeof(uint8_t) + 2] = (uint8_t)((val & 0x0000FF00) >>  8);
  custom_config_data[custom_configs[id].offset + sizeof(uint8_t) + 3] = (uint8_t)((val & 0x000000FF) >>  0);
}

void ESPKNXIP::config_set_bool(config_id_t id, bool val)
{
  if (id >= registered_configs)
    return;
  if (custom_configs[id].type != CONFIG_TYPE_BOOL)
    return;
  __config_set_flags(id, CONFIG_FLAGS_VALUE_SET);
  __config_set_bool(id, val);
}

void ESPKNXIP::__config_set_bool(config_id_t id, bool val)
{
  custom_config_data[custom_configs[id].offset + sizeof(uint8_t)] = val ? 1 : 0;
}

void ESPKNXIP::config_set_options(config_id_t id, uint8_t val)
{
  if (id >= registered_configs)
    return;
  if (custom_configs[id].type != CONFIG_TYPE_OPTIONS)
    return;

  option_entry_t *cur = custom_configs[id].data.options;
  while (cur->name != nullptr)
  {
    if (cur->value == val)
    {
      __config_set_flags(id, CONFIG_FLAGS_VALUE_SET);
      __config_set_options(id, val);
      break;
    }
    cur++;
  }
}

void ESPKNXIP::__config_set_options(config_id_t id, uint8_t val)
{
  custom_config_data[custom_configs[id].offset + sizeof(uint8_t)] = val;
}

void ESPKNXIP::config_set_ga(config_id_t id, address_t const &val)
{
  if (id >= registered_configs)
    return;
  if (custom_configs[id].type != CONFIG_TYPE_GA)
    return;
  __config_set_flags(id, CONFIG_FLAGS_VALUE_SET);
  __config_set_ga(id, val);
}

void ESPKNXIP::__config_set_ga(config_id_t id, address_t const &val)
{
  custom_config_data[custom_configs[id].offset + sizeof(uint8_t) + 0] = val.bytes.high;
  custom_config_data[custom_configs[id].offset + sizeof(uint8_t) + 1] = val.bytes.low;
}

String ESPKNXIP::config_get_string(config_id_t id)
{
  if (id >= registered_configs)
    return String("");

  return String((char *)&custom_config_data[custom_configs[id].offset + sizeof(uint8_t)]);
}

int32_t ESPKNXIP::config_get_int(config_id_t id)
{
  if (id >= registered_configs)
    return 0;

  int32_t v = (custom_config_data[custom_configs[id].offset + sizeof(uint8_t) + 0] << 24) +
              (custom_config_data[custom_configs[id].offset + sizeof(uint8_t) + 1] << 16) +
              (custom_config_data[custom_configs[id].offset + sizeof(uint8_t) + 2] <<  8) +
              (custom_config_data[custom_configs[id].offset + sizeof(uint8_t) + 3] <<  0);
  return v;
}

bool ESPKNXIP::config_get_bool(config_id_t id)
{
  if (id >= registered_configs)
    return false;

  return custom_config_data[custom_configs[id].offset + sizeof(uint8_t)] != 0;
}

uint8_t ESPKNXIP::config_get_options(config_id_t id)
{
  if (id >= registered_configs)
    return false;

  return custom_config_data[custom_configs[id].offset + sizeof(uint8_t)];
}

address_t ESPKNXIP::config_get_ga(config_id_t id)
{
  address_t t;
  if (id >= registered_configs)
  {
    t.value = 0;
    return t;
  }

  t.bytes.high = custom_config_data[custom_configs[id].offset + sizeof(uint8_t) + 0];
  t.bytes.low = custom_config_data[custom_configs[id].offset + sizeof(uint8_t) + 1];

  return t;
}
