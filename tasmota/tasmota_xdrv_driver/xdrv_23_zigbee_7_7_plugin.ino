/*
  xdrv_23_zigbee.ino - zigbee support for Tasmota

  Copyright (C) 2021  Theo Arends and Stephan Hadinger

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

#ifdef USE_ZIGBEE

const char Z_MUL[] PROGMEM = "mul:";
const char Z_DIV[] PROGMEM = "div:";
const char Z_MANUF[] PROGMEM = "manuf:";
const char Z_ADD[] PROGMEM = "add:";

char * Z_subtoken(char * token, const char * prefix) {
  size_t prefix_len = strlen_P(prefix);
  if (!strncmp_P(token, prefix, prefix_len)) {
    return token + prefix_len;
  }
  return nullptr;
}

// global singleton
Z_plugin_templates g_plugin_templates;

class Z_attribute_synonym Z_plugin_matchAttributeSynonym(const char *model, const char *manufacturer, uint16_t cluster, uint16_t attribute) {
  const Z_attribute_synonym * attr_syn;
  attr_syn = g_plugin_templates.matchAttributeSynonym(model == nullptr ? "" : model,
                                            manufacturer == nullptr ? "" : manufacturer,
                                            cluster, attribute);
  
  Z_attribute_synonym syn;
  if (attr_syn != nullptr) {
    syn = *attr_syn;
  }
  return syn;
}

Z_attribute_match Z_plugin_matchAttributeById(const char *model, const char *manufacturer, uint16_t cluster, uint16_t attribute) {
  const Z_plugin_attribute * attr_tmpl;
  attr_tmpl =  g_plugin_templates.matchAttributeById(model == nullptr ? "" : model,
                                            manufacturer == nullptr ? "" : manufacturer,
                                            cluster, attribute);
  
  Z_attribute_match attr;
  if (attr_tmpl != nullptr) {
    attr.cluster = attr_tmpl->cluster;
    attr.attribute = attr_tmpl->attribute;
    attr.name = attr_tmpl->name;
    attr.zigbee_type = attr_tmpl->type;
    attr.multiplier = attr_tmpl->multiplier;
    attr.divider = attr_tmpl->divider;
    attr.base = attr_tmpl->base;
    attr.manuf = attr_tmpl->manuf;
  }
  return attr;
}

Z_attribute_match Z_plugin_matchAttributeByName(const char *model, const char *manufacturer, const char * name) {
  const Z_plugin_attribute * attr_tmpl;
  Z_attribute_match attr;

  if (name != nullptr) {
    attr_tmpl =  g_plugin_templates.matchAttributeByName(model == nullptr ? "" : model,
                                              manufacturer == nullptr ? "" : manufacturer,
                                              name);
    
    if (attr_tmpl != nullptr) {
      attr.cluster = attr_tmpl->cluster;
      attr.attribute = attr_tmpl->attribute;
      attr.name = attr_tmpl->name;
      attr.zigbee_type = attr_tmpl->type;
      attr.multiplier = attr_tmpl->multiplier;
      attr.divider = attr_tmpl->divider;
      attr.base = attr_tmpl->base;
      attr.manuf = attr_tmpl->manuf;
    }
  }
  return attr;
}

bool Zb_readline(class File *f, char* buf, size_t size) {
  bool eof = 0;
  while (1) {
    // read line
    bool comment = false;                   // did we encounter '#', if so ignore anything until '\n'
    char * p = buf;
    while (1) {
      int c = f->read();
      if (c == -1) { eof = true; break; }   // EOF reached
      if (c == '#') { comment = true; }     // rest of line is ignored
      if (c == '\n') { break; }             // end of line
      if (!comment) {
        if (p < buf + size - 1) {
          *p++ = c;                         // append character
        } else {
          AddLog(LOG_LEVEL_INFO, "ZIG: ZbLoad line exceeds 96 bytes, aborting");
          return false;
        }
      }
    }
    int32_t ret = p - buf;                  // len in bytes
    if (eof && ret == 0) { return false; }  // nothing more to read
    // found something, don't add the \n since we don't need it
    buf[ret] = 0;           // add string terminator
    RemoveSpace(buf);       // remove anything that looks like a space, tab, crlf...
    // AddLog(LOG_LEVEL_INFO, "ZIG: ZbRead>'%s'", buf);
    return true;
  }
}

#ifdef USE_UFILESYS
extern FS *ffsp;
#endif

bool ZbLoad_inner(const char *filename, File &fp) {
  char * filename_imported = nullptr;
  Z_plugin_template * tmpl = nullptr;   // current template with matchers and attributes
  bool new_matchers = false;            // indicates that we have finished the current matchers
  char buf_line[96];        // max line is 96 bytes (comments don't count)

  // read the first 6 chars
  bool invalid_header = false;
  static const char Z2T_HEADER_V1[] PROGMEM = "#Z2Tv1";
  for (uint32_t i = 0; i < 6; i++) {
    int c = fp.read();
    if (c < 0) {
      invalid_header = true;
      break;
    }
    buf_line[i] = c;
    buf_line[i+1] = 0;
  }
  if (!invalid_header) {
    if (strcmp_P(buf_line, Z2T_HEADER_V1) != 0) {
      invalid_header = true;
    }
  }

  if (invalid_header) {
    AddLog(LOG_LEVEL_INFO, "ZIG: ZbLoad '%s' invalid header", filename);
    return false;
  }

  // parse line by line
  while (1) {
    if (!Zb_readline(&fp, buf_line, sizeof(buf_line))) {
      // EOF
      break;
    }

    // at first valid line, we instanciate a new plug-in instance and assign a filemane
    if (filename_imported == nullptr) {
      // allocate only once the filename for multiple entries
      // freed only by `ZbUnload`
      filename_imported = (char*) malloc(strlen_P(filename)+1);
      strcpy_P(filename_imported, filename);
    }

    // there is a non-empty line, containing no space/tab/crlf
    // depending on the first char, parse either device name or cluster/attribute+name
    if (buf_line[0] == ':') {
      if (tmpl == nullptr || new_matchers) {
        tmpl = &g_plugin_templates.addToLast();
        tmpl->filename = filename_imported;
        new_matchers = false;
      }
      // parse device name
      char *rest = buf_line + 1;    // skip first char ':'
      char *token = strtok_r(rest, ",", &rest);
      Z_plugin_matcher & matcher = tmpl->matchers.addToLast();
      if (token != nullptr) {
        matcher.setModel(token);
      }
      token = strtok_r(rest, ",", &rest);
      if (token != nullptr) {
        matcher.setManuf(token);
      }
    } else {
      if (tmpl == nullptr) {
        continue;
      }
      new_matchers = true;
      char *rest = buf_line;
      char *token = strtok_r(rest, ",", &rest);
      if (token == nullptr) {
        continue;
      }

      // detect if token contains '=', if yes it is a synonym
      char * delimiter_equal = strchr(token, '=');

      if (delimiter_equal == nullptr) {
        // NORMAL ATTRIBUTE
        // token is of from '0000/0000' or '0000/0000%00'
        char * delimiter_slash = strchr(token, '/');
        char * delimiter_percent = strchr(token, '%');
        if (delimiter_slash == nullptr || (delimiter_percent != nullptr && delimiter_slash > delimiter_percent)) {
          AddLog(LOG_LEVEL_INFO, "ZIG: ZbLoad '%s' wrong delimiter '%s'", filename, token);
        }

        uint16_t attr_id = 0xFFFF;
        uint16_t cluster_id = 0xFFFF;
        uint8_t  type_id = Zunk;
        uint32_t multiplier = 1;
        uint32_t divider = 1;
        int32_t  base = 0;
        char *   name = nullptr;
        uint16_t manuf = 0;

        cluster_id = strtoul(token, &delimiter_slash, 16);
        if (!delimiter_percent) {
          attr_id = strtoul(delimiter_slash+1, nullptr, 16);
        } else {
          attr_id = strtoul(delimiter_slash+1, &delimiter_percent, 16);
          type_id = Z_getTypeByName(delimiter_percent+1);
        }
        // NAME of the attribute
        token = strtok_r(rest, ",", &rest);
        if (token == nullptr) {
          AddLog(LOG_LEVEL_INFO, "ZIG: ZbLoad '%s' ignore missing name '%s'", filename, buf_line);
          continue;
        }
        name = token;
        // ADDITIONAL ELEMENTS?
        // Ex: `manuf:1037`
        while (token = strtok_r(rest, ",", &rest)) {
          char * sub_token;
          // look for multiplier
          if (sub_token = Z_subtoken(token, Z_MUL))  {
            multiplier = strtol(sub_token, nullptr, 10);
          }
          // look for divider
          else if (sub_token = Z_subtoken(token, Z_DIV))  {
            divider = strtol(sub_token, nullptr, 10);    // negative to indicate divider
          }
          // look for offset (base)
          else if (sub_token = Z_subtoken(token, Z_ADD))  {
            base = strtol(sub_token, nullptr, 10);    // negative to indicate divider
          }
          // look for `manuf:HHHH`
          else if (sub_token = Z_subtoken(token, Z_MANUF))  {
            manuf = strtoul(sub_token, nullptr, 16);
          }
          else {
            AddLog(LOG_LEVEL_DEBUG, "ZIG: ZbLoad unrecognized modifier '%s'", token);
          }
        }
        
        // token contains the name of the attribute
        Z_plugin_attribute & plugin_attr = tmpl->attributes.addToLast();
        plugin_attr.cluster = cluster_id;
        plugin_attr.attribute = attr_id;
        plugin_attr.type = type_id;
        plugin_attr.setName(name);
        plugin_attr.multiplier = multiplier;
        plugin_attr.divider = divider;
        plugin_attr.base = base;
        plugin_attr.manuf = manuf;
      } else {
        // ATTRIBUTE SYNONYM
        // token is of from '0000/0000=0000/0000,1'
        char * rest2 = token;
        char * tok2 = strtok_r(rest2, "=", &rest2);
        char * delimiter_slash = strchr(tok2, '/');
        uint16_t cluster_id = strtoul(tok2, &delimiter_slash, 16);
        uint16_t attr_id = strtoul(delimiter_slash+1, nullptr, 16);
        tok2 = strtok_r(rest2, "=", &rest2);
        char * delimiter_slash2 = strchr(tok2, '/');
        uint16_t new_cluster_id = strtoul(tok2, &delimiter_slash2, 16);
        uint16_t new_attr_id = strtoul(delimiter_slash2+1, nullptr, 16);
        uint32_t multiplier = 1;
        uint32_t divider = 1;
        int32_t  base = 0;

        // ADDITIONAL ELEMENTS?
        while (token = strtok_r(rest, ",", &rest)) {
          char * sub_token;
          // look for multiplier
          if (sub_token = Z_subtoken(token, Z_MUL))  {
            multiplier = strtol(sub_token, nullptr, 10);
          }
          // look for divider
          else if (sub_token = Z_subtoken(token, Z_DIV))  {
            divider = strtol(sub_token, nullptr, 10);    // negative to indicate divider
          }
          // look for offset (base)
          else if (sub_token = Z_subtoken(token, Z_ADD))  {
            base = strtol(sub_token, nullptr, 10);    // negative to indicate divider
          }
          else {
            AddLog(LOG_LEVEL_DEBUG, "ZIG: ZbLoad unrecognized modifier '%s'", token);
          }
        }
        // create the synonym
        Z_attribute_synonym & syn = tmpl->synonyms.addToLast();
        syn.cluster = cluster_id;
        syn.attribute = attr_id;
        syn.new_cluster = new_cluster_id;
        syn.new_attribute = new_attr_id;
        syn.multiplier = multiplier;
        syn.divider = divider;
        syn.base = base;
      }
    }
  }
  return true;
}

// load a file from filesystem
// returns `true` if success
bool ZbLoad(const char *filename_raw) {

#ifdef USE_UFILESYS
  if (ffsp) {
    // first unload previsou definitions
    ZbUnload(filename_raw);

    String filename = filename_raw;
    if (filename_raw[0] != '/') {
      filename = "/";
      filename += filename_raw;
    }
    File fp;
    fp = ffsp->open(filename.c_str(), "r");

    if (fp <= 0) {
      AddLog(LOG_LEVEL_INFO, "ZIG: unable to load file '%s'", filename.c_str());
      return false;
    }

    return ZbLoad_inner(filename.c_str(), fp);
  } else {
    AddLog(LOG_LEVEL_ERROR, "ZIG: filesystem not enabled");
  }
#else
  AddLog(LOG_LEVEL_INFO, "ZIG: ZbLoad requires file system");
#endif
  return true;
}

// Unlaod previously loaded definitions
bool ZbUnload(const char *filename_raw) {
  String filename = filename_raw;
  if (filename_raw[0] != '/') {
    filename = "/";
    filename += filename_raw;
  }

  char * filename_registered = nullptr;     // internal allocation for filename
  for (const Z_plugin_template & tmpl : g_plugin_templates) {
    bool to_be_freed = false;
    if (filename_registered) {
      // if filename_registered is not NULL, compare pointers
      if (tmpl.filename == filename_registered) { to_be_freed = true; }
    } else {
      if (strcmp(tmpl.filename, filename.c_str()) == 0) {
        filename_registered = tmpl.filename;
        to_be_freed = true;
      }
    }
    // check if we remove this node
    if (to_be_freed) {
      g_plugin_templates.remove(&tmpl);
    }
  }
  // free memory for filename
  if (filename_registered) {
    free(filename_registered);
    AddLog(LOG_LEVEL_INFO, "ZIG: ZbUnload '%s' sucessful", filename_raw);
    return true;
  }
  return false;
}

// append modifiers like mul/div/manuf
void Z_AppendModifiers(char * buf, size_t buf_len, uint32_t multiplier, uint32_t divider, int32_t base, uint16_t manuf) {
  if (multiplier != 0 && multiplier != 1) {
    ext_snprintf_P(buf, buf_len, "%s,%s%i", buf, Z_MUL, multiplier);
  }
  if (divider != 0 && divider != 1) {
    ext_snprintf_P(buf, buf_len, "%s,%s%i", buf, Z_DIV, divider);
  }
  if (base != 0) {
    ext_snprintf_P(buf, buf_len, "%s,%s%i", buf, Z_ADD, base);
  }
  if (manuf) {
    ext_snprintf_P(buf, buf_len, "%s,%s%04X", buf, Z_MANUF, manuf);
  }
}

// Dump the ZbLoad structure in a format compatible with ZbLoad
void ZbLoadDump(void) {
  char buf[96];
  AddLog(LOG_LEVEL_INFO, "ZIG: ZbLoad dump all current information");
  AddLog(LOG_LEVEL_INFO, "====> START");

  for (const Z_plugin_template & tmpl : g_plugin_templates) {
    if (tmpl.filename != nullptr) {
      ext_snprintf_P(buf, sizeof(buf), "# imported from '%s'", tmpl.filename);
      AddLog(LOG_LEVEL_INFO, PSTR("%s"), buf);
    }
    // marchers
    if (tmpl.matchers.length() == 0) {
      ext_snprintf_P(buf, sizeof(buf), ":    # no matcher");
      AddLog(LOG_LEVEL_INFO, PSTR("%s"), buf);
    } else {
      for (const Z_plugin_matcher & matcher : tmpl.matchers) {
        ext_snprintf_P(buf, sizeof(buf), ":%s,%s", matcher.model ? matcher.model : "", matcher.manufacturer ? matcher.manufacturer : "");
        AddLog(LOG_LEVEL_INFO, PSTR("%s"), buf);
      }
    }
    // attributes
    if (tmpl.attributes.length() == 0 && tmpl.synonyms.length() == 0) {
      // no content, output an empty line
      AddLog(LOG_LEVEL_INFO, "");
    } else {
      for (const Z_plugin_attribute & attr : tmpl.attributes) {
        ext_snprintf_P(buf, sizeof(buf), "%04X/%04X", attr.cluster, attr.attribute);
        // add type if known
        if (attr.type != Zunk) {
          char type_str[16];
          Z_getTypeByNumber(type_str, sizeof(type_str), attr.type);
          ext_snprintf_P(buf, sizeof(buf), "%s%%%s", buf, type_str);
        }
        ext_snprintf_P(buf, sizeof(buf), "%s,%s", buf, attr.name);
        Z_AppendModifiers(buf, sizeof(buf), attr.multiplier, attr.divider, attr.base, attr.manuf);
        AddLog(LOG_LEVEL_INFO, PSTR("%s"), buf);
      }
      for (const Z_attribute_synonym & syn : tmpl.synonyms) {
        ext_snprintf_P(buf, sizeof(buf), "%04X/%04X=%04X/%04X", syn.cluster, syn.attribute, syn.new_cluster, syn.new_attribute);
        Z_AppendModifiers(buf, sizeof(buf), syn.multiplier, syn.divider, syn.base, 0);
        AddLog(LOG_LEVEL_INFO, PSTR("%s"), buf);
      }
    }
  }

  AddLog(LOG_LEVEL_INFO, "<==== END");
}

// Auto-load all files ending with '.zb'
void ZbAutoload(void) {
  ZbAutoLoadFromFlash();
#ifdef USE_UFILESYS
  if (ffsp) {
    File dir = ffsp->open("/", "r");
    if (dir) {
      dir.rewindDirectory();
      while (1) {
          File entry = dir.openNextFile();
          if (!entry) { break; }
          const char * fn = entry.name();
          if (strcmp(fn, ".") && strcmp(fn, "..")) {
            // check suffix
            size_t l = strlen(fn);
            if (l > 3) {
              if (fn[l-3] == '.' && fn[l-2] == 'z' && fn[l-1] == 'b') {
                bool ret = ZbLoad(fn);
                if (ret) {
                  AddLog(LOG_LEVEL_INFO, "ZIG: ZbLoad '%s' loaded successfully", fn);
                }
              }
            }
          }

      }

    }
  }
#endif // USE_UFILESYS
}

#endif // USE_ZIGBEE
