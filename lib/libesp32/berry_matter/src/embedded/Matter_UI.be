#
# Matter_UI.be - WebUI for Matter configuration in Tasmota
#
# Copyright (C) 2023  Stephan Hadinger & Theo Arends
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

#######################################################################
# Matter Web UI
#
#######################################################################

import matter

#@ solidify:Matter_UI,weak

#################################################################################
# Partition_wizard_UI
#
# WebUI for the partition manager
#################################################################################
class Matter_UI
  static var _ADD_ENDPOINT_JS =
  "<script type='text/javascript'>"
  # Parse compact schema string into object
  "function parseSchema(s){"
  "var r={t:'t',r:0};"
  "var parts=s.split('|');"
  "if(parts.length>0&&parts[0].indexOf(':')===-1){"
  "r.a=parts[0];parts=parts.slice(1);}"
  "parts.forEach(function(p){"
  "var i=p.indexOf(':');"
  "if(i<0)return;"
  "var k=p.substring(0,i),v=p.substring(i+1);"
  "if(k==='l')r.l=v;"
  "else if(k==='t')r.t=v;"
  "else if(k==='h')r.h=v;"
  "else if(k==='d')r.d=v;"
  "else if(k==='r')r.r=(v!=='0');"
  "else if(k==='o'){"
  "r.o=[];v.split(',').forEach(function(opt){"
  "var ov=opt.split(':');r.o.push({v:ov[0],l:ov[1]||ov[0]});});}"
  "});return r;};"
  # Generate HTML input for a single field
  "function genFieldEp(prefix,key,def,val){"
  "var fn=prefix+'_'+(def.a||key);"
  "var req=def.r?' required':'';"
  "var ti=def.h?' title=\"'+def.h+'\"':'';"
  "var dv=(val!=null&&val!=='')?val:(def.d||'');"
  "if(def.t==='i')"
  "return '<input type=\"number\" id=\"'+fn+'\" name=\"'+fn+'\" value=\"'+dv+'\" size=\"1\"'+ti+req+'>';"
  "if(def.t==='s'){"
  "var h='<select id=\"'+fn+'\" name=\"'+fn+'\"'+req+'>';"
  "(def.o||[]).forEach(function(o){"
  "var sel=(String(o.v)==String(dv))?' selected':'';"
  "h+='<option value=\"'+o.v+'\"'+sel+'>'+o.l+'</option>';});"
  "return h+'</select>';}"
  "if(def.t==='c'){"
  "var chk=(String(dv)==='1'||dv==='true'||dv===true)?' checked':'';"
  "return '<input type=\"checkbox\" id=\"'+fn+'\" name=\"'+fn+'\"'+chk+' style=\"width:auto\">';}"
  "return '<input type=\"text\" id=\"'+fn+'\" name=\"'+fn+'\" value=\"'+dv+'\" size=\"1\"'+ti+req+'>';"
  "};"
  # Build type <select> options from dn map
  "function typeOpts(sel){"
  "var h='<option value=\"\"></option>';"
  "for(var t in dn){"
  "if(t==='-virtual'){h+='<option disabled>--- Virtual ---</option>';continue;}"
  "if(t==='-zigbee'){h+='<option disabled>--- Zigbee ---</option>';continue;}"
  "h+='<option value=\"'+t+'\"'+(t===sel?' selected':'')+'>'+dn[t]+'</option>';}"
  "return h;};"
  # Generate parameter rows as table rows (indented under the header row)
  # Returns HTML string of <tr> rows for each schema param
  "function epParamRows(ep,typ,conf){"
  "var schema=ps?ps[typ]:null;"
  "if(!schema||Object.keys(schema).length===0)return '';"
  "var h='';"
  "for(var key in schema){"
  "var def=parseSchema(schema[key]);"
  "var lbl=def.l||def.a||key;"
  "var akey=def.a||key;"
  "var val=conf[akey];"
  "if(val==null)val='';"
  "h+='<tr><td></td>"  # empty cell under #
  "<td colspan=\"2\" style=\"font-size:smaller;padding:1px 0\">"
  "<span style=\"color:#aaa\">'+lbl+': </span>"
  "'+genFieldEp('ep'+ep,key,def,val)+"
  "'</td><td></td></tr>';}"
  "return h;};"
  # Generate all rows for one endpoint: header row + param rows
  # Returns HTML string to insert inside the table
  "function genEpRows(ep,conf,isNew){"
  "var typ=conf.type||'';"
  "var nam=conf.name||'';"
  "var h='';"
  # Header row: # | Name | Type | delete
  "h+='<tr id=\"epr'+ep+'\">"
  "<td style=\"font-size:smaller\"><b>'+ep+'</b></td>"
  "<td style=\"font-size:smaller\"><input type=\"text\" id=\"epNam'+ep+'\" size=\"1\" value=\"'+nam+'\" placeholder=\"(optional)\"></td>';"
  "if(isNew){"
  "h+='<td style=\"font-size:smaller\"><select id=\"epTyp'+ep+'\" onchange=\"chgType('+ep+',this.value)\">'+typeOpts(typ)+'</select></td>';"
  "}else{"
  "var dnam=dn[typ]||(typ.indexOf('http_')===0?'&#x1F517; '+(dn[typ.substring(5)]||typ.substring(5)):typ);"
  "h+='<td style=\"font-size:smaller\"><b>'+dnam+'</b></td>';}"
  "h+='<td style=\"text-align:center\"><button type=\"button\" title=\"Delete\" "
  "style=\"background:none;border:none;line-height:1;cursor:pointer\" "
  "onclick=\"delEp('+ep+')\">&#128293;</button></td></tr>';"
  # Parameter rows below — no vertical gap from header
  "h+='<tr id=\"epp'+ep+'\"><td></td><td colspan=\"2\" style=\"padding:0\"><div id=\"epPrm'+ep+'\" style=\"margin:0;padding:0\">';"
  "var schema=ps?ps[typ]:null;"
  "if(schema&&Object.keys(schema).length>0){"
  "var tbl='<table style=\"width:100%;border-spacing:0\">';"
  "for(var key in schema){"
  "var def=parseSchema(schema[key]);"
  "var lbl=def.l||def.a||key;"
  "var akey=def.a||key;"
  "var val=conf[akey];if(val==null)val='';"
  "tbl+='<tr><td style=\"font-size:smaller;color:#aaa;white-space:nowrap;padding:0 4px 0 0;width:50%\">'+lbl+'</td>"
  "<td style=\"font-size:smaller;padding:0\">'+genFieldEp('ep'+String(ep),key,def,val)+'</td></tr>';}"
  "tbl+='</table>';"
  "h+=tbl;}"
  "h+='</div></td><td></td></tr>';"
  "return h;};"
  # When type changes on a new endpoint, regenerate param fields
  "function chgType(ep,typ){"
  "var el=eb('epPrm'+ep);"
  "if(!el)return;"
  "var schema=ps?ps[typ]:null;"
  "if(!schema||Object.keys(schema).length===0){el.innerHTML='';return;}"
  "var tbl='<table style=\"width:100%;border-spacing:0\">';"
  "for(var key in schema){"
  "var def=parseSchema(schema[key]);"
  "var lbl=def.l||def.a||key;"
  "tbl+='<tr><td style=\"font-size:smaller;color:#aaa;white-space:nowrap;padding:0 4px 0 0;width:50%\">'+lbl+'</td>"
  "<td style=\"font-size:smaller;padding:0\">'+genFieldEp('ep'+ep,key,def,'')+'</td></tr>';}"
  "tbl+='</table>';el.innerHTML=tbl;};"
  # Delete endpoint: remove from cfg, remove rows from DOM
  "function delEp(ep){"
  "if(!confirm('Remove endpoint '+ep+'?'))return;"
  "delete cfg[String(ep)];"
  "var r=eb('epr'+ep);if(r)r.remove();"
  "var p=eb('epp'+ep);if(p)p.remove();};"
  # Add new endpoint client-side
  "function addEp(){"
  "var ep=nextep;nextep=ep+1;"
  "cfg[String(ep)]={type:''};"
  "var tbl=eb('epTbl');"
  "if(!tbl)return;"
  "var none=eb('epNone');if(none)none.remove();"
  "tbl.insertAdjacentHTML('beforeend',genEpRows(ep,{type:''},true));};"
  # submitConfig: collect all endpoint data, build cfg, submit
  "function submitConfig(f){"
  "for(var k in cfg){"
  "var ne=eb('epNam'+k);"
  "if(ne){var v=ne.value.trim();if(v)cfg[k].name=v;else delete cfg[k].name;}"
  "var te=eb('epTyp'+k);"
  "if(te)cfg[k].type=te.value;"
  "var typ=cfg[k].type;"
  "if(!typ)continue;"
  "var schema=ps?ps[typ]:null;"
  "if(schema){"
  "for(var key in schema){"
  "var def=parseSchema(schema[key]);"
  "var akey=def.a||key;"
  "var fe=eb('ep'+k+'_'+key);"
  "if(fe){"
  "var v=fe.type==='checkbox'?(fe.checked?1:0):fe.value;"
  "if(v!==''&&v!==null&&v!==undefined){"
  "cfg[k][akey]=(def.t==='i')?parseInt(v,10):v;"
  "}else{delete cfg[k][akey];}}"
  "}}}"
  "for(var k in cfg){if(!cfg[k].type){delete cfg[k];}}"
  "f.elements['config_json'].value=JSON.stringify(cfg);"
  "return true;};"
  "</script>"

  static var _CLASSES_TYPES_STD =
                              "|relay|light0|light1|light2|light3|shutter|shutter+tilt"
                              "|gensw_btn"
                              "|temperature|pressure|illuminance|humidity|occupancy|onoff|contact|flow|rain|waterleak"
                              "|airquality"
  static var _CLASSES_TYPES_VIRTUAL = 
                              "-virtual|v_relay|v_light0|v_light1|v_light2|v_light3"
                              "|v_fan"
                              "|v_temp|v_pressure|v_illuminance|v_humidity|v_occupancy|v_contact|v_flow|v_rain|v_waterleak"
                              "|v_airquality"
  static var _CLASSES_TYPES2= "|http_relay|http_light0|http_light1|http_light2|http_light3"
                              "|http_temperature|http_pressure|http_illuminance|http_humidity"
                              "|http_occupancy|http_contact|http_flow|http_rain|http_waterleak"
                              "|http_airquality"
  var device
  var matter_enabled

  #----------------------------------------------------------------------- -#
  # Parse compact parameter definition string
  # Format: "relay|l:Relay|t:i|h:1-8|d:1"
  # First element is the arg name (no prefix), followed by short identifiers:
  #   l: - label (display name, defaults to arg name if omitted)
  #   t: - type (t=text, i=int, s=select, c=checkbox)
  #   h: - hint
  #   d: - default
  #   r: - required (1=required, omit or 0=optional)
  #   o: - options (select only, format: C:Celsius,F:Fahrenheit)
  # Returns a map with parsed values including 'arg' for the arg name
  #----------------------------------------------------------------------- -#
  def parse_param_def(param_str)
    import string
    var param_def = {}
    var parts = string.split(param_str, '|')
    
    # First element is the arg name (no prefix)
    if size(parts) > 0
      param_def['arg'] = parts[0]
    end
    
    # Parse remaining elements with key:value format
    for i: 1 .. size(parts) - 1
      var part = parts[i]
      var idx = string.find(part, ':')
      if idx >= 0
        var key = part[0..idx-1]
        var value = part[idx+1..]
        
        if key == 'l'
          param_def['label'] = value
        elif key == 't'
          param_def['type'] = value
        elif key == 'h'
          param_def['hint'] = value
        elif key == 'd'
          param_def['default'] = value
        elif key == 'r'
          param_def['required'] = (value != '0')
        elif key == 'o'
          param_def['options'] = self.parse_options(value)
        end
      end
    end
    
    # Set defaults
    if !param_def.contains('type')     param_def['type'] = 't'       end
    if !param_def.contains('required') param_def['required'] = false end
    # Label defaults to arg name if not specified
    if !param_def.contains('label') && param_def.contains('arg')
      param_def['label'] = param_def['arg']
    end
    
    return param_def
  end

  #----------------------------------------------------------------------- -#
  # Parse options string for select type
  # Format: "C:Celsius,F:Fahrenheit" or "opt1,opt2,opt3"
  # Returns a list of maps: [{"value": "C", "label": "Celsius"}, ...]
  #----------------------------------------------------------------------- -#
  def parse_options(opts_str)
    import string
    var options = []
    var pairs = string.split(opts_str, ',')
    
    for pair: pairs
      var idx = string.find(pair, ':')
      if idx >= 0
        options.push({"value": pair[0..idx-1], "label": pair[idx+1..]})
      elif size(pair) > 0
        options.push({"value": pair, "label": pair})
      end
    end
    
    return options
  end

  #----------------------------------------------------------------------- -#
  # Extract the arg name from a schema string (first element before '|')
  # Format: "relay|t:i|h:1-8|d:1"
  # Returns the arg name string or nil if not found
  #----------------------------------------------------------------------- -#
  def extract_arg_name(schema_str)
    import string
    if schema_str == nil || schema_str == ""
      return nil
    end
    var idx = string.find(schema_str, '|')
    if idx > 0
      return schema_str[0..idx-1]
    elif idx < 0
      # No '|' found, entire string is the arg name
      return schema_str
    end
    return nil
  end

  #----------------------------------------------------------------------- -#
  # Collect all schemas from a plugin class (SCHEMA, SCHEMA2, SCHEMA3...)
  # Uses introspection to iterate through numbered SCHEMA variables
  # Stops when nil or empty string is encountered
  # Returns a map: {arg_name: schema_string, ...}
  # Requirements: 1.7, 1.8, 5.8
  #----------------------------------------------------------------------- -#
  def collect_plugin_schemas(cl)
    import introspect
    var schemas = {}
    var i = 1
    
    while true
      # Build schema variable name: SCHEMA, SCHEMA2, SCHEMA3...
      var schema_name = (i == 1) ? "SCHEMA" : "SCHEMA" + str(i)
      var schema = introspect.get(cl, schema_name)
      
      # Stop when nil or empty string is encountered
      if schema == nil || schema == ""
        break
      end
      
      # Extract arg name from schema (first element before '|')
      var arg_name = self.extract_arg_name(schema)
      if arg_name != nil && arg_name != ""
        schemas[arg_name] = schema
      end
      
      i += 1
    end
    
    return schemas
  end



  # ####################################################################################################
  # Static function to compare two maps (shallow compare)
  # return true if equal
  static def equal_map(a, b)
    # all items of a are in b
    for k: a.keys()
      if !b.contains(k)   return false  end
      if b[k] != a[k]     return false  end
    end
    for k: b.keys()
      if !a.contains(k)   return false  end
      if b[k] != a[k]     return false  end
    end
    return true
  end

  #----------------------------------------------------------------------- -#
  # Handle config_json POST parameter
  # The browser sends the entire "config" section as a JSON string
  # Server parses, validates, replaces config, recomputes nextep,
  # reconciles live plugins, and saves.
  # Requirements: 4.6, 4.7, 4.8, 4.9
  #----------------------------------------------------------------------- -#
  def handle_config_json()
    import webserver
    import json
    
    var config_json_str = webserver.arg("config_json")
    if config_json_str == nil || config_json_str == ""
      raise "value_error", "Empty configuration"
    end
    
    var new_config = json.load(config_json_str)
    if new_config == nil
      raise "value_error", "Invalid JSON configuration"
    end
    
    # Validate all endpoint types exist
    for ep_str: new_config.keys()
      var conf = new_config[ep_str]
      var typ = conf.find('type')
      if typ == nil
        raise "value_error", "Endpoint " + ep_str + " missing type"
      end
      if self.device.plugins_classes.find(typ) == nil
        raise "value_error", "Unknown type '" + typ + "' for endpoint " + ep_str
      end
    end
    
    # Capture old config for reconciliation
    var old_config = self.device.plugins_config
    
    # Replace config wholesale
    self.device.plugins_config = new_config
    
    # Recompute nextep from config keys
    self.device.adjust_next_ep()
    
    # Reconcile live plugin instances
    self.reconcile_plugins(old_config, new_config)
    
    # Save
    self.device.plugins_persist = true
    self.device.save_param()
    
    log("MTR: config updated via config_json", 2)
  end

  #----------------------------------------------------------------------- -#
  # Reconcile live plugin instances with new config
  # - Remove plugins for deleted endpoints (but not root/aggregator ep<=1)
  # - Add plugins for new endpoints
  # - Update plugins for changed endpoints (name, params)
  # Parameters:
  #   old_config: the previous plugins_config map
  #   new_config: the new plugins_config map
  # Requirements: 4.8
  #----------------------------------------------------------------------- -#
  def reconcile_plugins(old_config, new_config)
    # Build set of new endpoint numbers
    var new_eps = {}
    for ep_str: new_config.keys()
      new_eps[int(ep_str)] = true
    end
    
    # Remove plugins for endpoints no longer in config (skip ep 0 and 1 = root/aggregator)
    var idx = 0
    while idx < size(self.device.plugins)
      var pl = self.device.plugins[idx]
      var ep = pl.get_endpoint()
      if ep > 1 && !new_eps.contains(ep)
        self.device.plugins.remove(idx)
      else
        idx += 1
      end
    end
    
    # Build map of remaining plugin instances by endpoint
    var current_eps = {}
    for pl: self.device.plugins
      current_eps[pl.get_endpoint()] = pl
    end
    
    # Add/update plugins for endpoints in new config
    for ep_str: new_config.keys()
      var ep = int(ep_str)
      var conf = new_config[ep_str]
      var typ = conf.find('type')
      
      if current_eps.contains(ep)
        # Endpoint exists - check if type changed
        var old_conf = old_config.find(ep_str)
        var old_type = old_conf ? old_conf.find('type') : nil
        
        if old_type != typ
          # Type changed - remove old, create new
          var rm_idx = 0
          while rm_idx < size(self.device.plugins)
            if self.device.plugins[rm_idx].get_endpoint() == ep
              self.device.plugins.remove(rm_idx)
              break
            end
            rm_idx += 1
          end
          # Create new plugin
          var pi_class = self.device.plugins_classes.find(typ)
          if pi_class != nil
            var pi = pi_class(self.device, ep, conf)
            self.device.plugins.push(pi)
          end
        else
          # Same type - update configuration (name, params)
          var pl = current_eps[ep]
          pl.parse_configuration(conf)
          var nam = conf.find('name')
          if nam != nil
            pl.set_name(nam)
          end
        end
      else
        # New endpoint - instantiate plugin
        var pi_class = self.device.plugins_classes.find(typ)
        if pi_class != nil
          var pi = pi_class(self.device, ep, conf)
          self.device.plugins.push(pi)
          log(format("MTR: adding endpoint = %i type:%s", ep, typ), 2)
        end
      end
    end
    
    # Clean any orphan remotes
    self.device.clean_remotes()
    self.device.signal_endpoints_changed()
  end

  # ####################################################################################################
  # Constructor
  def init(device, matter_enabled)
    self.device = device
    self.matter_enabled = matter_enabled
    tasmota.add_driver(self)
  end

  # ####################################################################################################
  # Init web handlers
  # ####################################################################################################
  # Displays the Configure Matter button on the configuration page
  def web_add_config_button()
    import webserver
    # webserver.content_send("<p></p><form id=ac action='matterc' style='display: block;' method='get'><button>Configure Matter</button></form>")
    webserver.content_send("<p></p><form id=ac action='matterc' style='display: block;' method='get'><button>")
    # send Matter logo in SVG
    webserver.content_send(
      "<svg style='vertical-align:middle;' width='24' height='24' xmlns='http://www.w3.org/2000/svg' viewBox='100 100 240 240'>"
      "<defs><style>.cls-1{fill:none}.cls-2{fill:#FFFFFF;}</style></defs><rect class='cls-1' "
      "width='420' height='420'/><path class='cls-2' d='"
      "M167,156.88a71,71,0,0,0,32.1,14.73v-62.8l12.79-7.38,12.78,7.38v62.8a71.09,71.09,0,0,0,32.11-14.73"
      "L280,170.31a96.92,96.92,0,0,1-136.33,0Zm28.22,160.37A96.92,96.92,0,0,0,127,199.19v26.87a71.06,"
      "71.06,0,0,1,28.82,20.43l-54.39,31.4v14.77L114.22,300l54.38-31.4a71,71,0,0,1,3.29,35.17Zm101.5-"
      "118.06a96.93,96.93,0,0,0-68.16,118.06l23.27-13.44a71.1,71.1,0,0,1,3.29-35.17L309.46,300l12.78-"
      "7.38V277.89l-54.39-31.4a71.13,71.13,0,0,1,28.82-20.43Z'/></svg>"
    )
    webserver.content_send(" Matter</button></form>")
  end

  #- ---------------------------------------------------------------------- -#
  #- Show commissioning information and QR Code
  #
  # Returns true if Matter is enabled
  #- ---------------------------------------------------------------------- -#
  def show_enable()
    import webserver
    var matter_enabled = self.matter_enabled

    webserver.content_send("<fieldset><legend><b>&nbsp;Matter &nbsp;</b></legend>"
                           "<p style='width:320px;'>Check the <a href='https://tasmota.github.io/docs/Matter/' target='_blank'>Matter documentation</a>.</p>"
                           "<form action='/matterc' method='post'>")

    # checkbox for Matter enable
    var matter_enabled_checked = matter_enabled ? 'checked' : ''
    webserver.content_send(f"<p><input id='menable' type='checkbox' name='menable' {matter_enabled_checked}>")
    webserver.content_send("<label for='menable'><b>Matter enable</b></label></p>")

    if matter_enabled
      # checkbox for Matter commissioning
      var commissioning_open_checked = self.device.commissioning.commissioning_open != nil ? "checked" : ""
      webserver.content_send(f"<p><input id='comm' type='checkbox' name='comm' {commissioning_open_checked}>")
      webserver.content_send("<label for='comm'><b>Commissioning open</b></label></p>")
      var disable_bridge_mode_checked = self.device.disable_bridge_mode ? " checked" : ""
      webserver.content_send(f"<p><input type='checkbox' name='nobridge'{disable_bridge_mode_checked}><b>Force Static endpoints</b> (non-bridge)</p>")  
    end

    webserver.content_send("<p></p><button name='save' class='button bgrn'>Save</button></form></p>"
                           "</fieldset><p></p>")
  end

  #- ---------------------------------------------------------------------- -#
  #- Show QR Code
  #- ---------------------------------------------------------------------- -#
  def show_qrcode(qr_text)
    import webserver
    # QRCode via UTF8
    var empty = " "
    var lowhalf = "\342\226\204"
    var uphalf = "\342\226\200"
    var full = "\342\226\210"

    var qr = matter.QRCode.encode_str(qr_text)
    var bitmap = qr['bitmap']
    var sz = qr['size']

    webserver.content_send('<style>.qr{font-family:monospace; margin:0; padding:0; white-space:pre; font-size:18px; color:#fff; line-height:100%;}</style>')


    webserver.content_send("<div style='transform:scale(.8,1); display:inline-block;'>")

    var s = "<div class='qr'>"
    webserver.content_send(s)
    s = ""
    for i: 0 .. sz + 1    s += lowhalf  end
    s += "</div>"
    webserver.content_send(s)
    for i: 0 .. (sz+1)/2 - 1
      s = "<div class='qr' style='background-color:#000;'>" + full
      for j: 0 .. sz - 1
        var high = (bitmap[i*2][j] == " ")
        var low = (i*2+1 < sz) ? (bitmap[i*2+1][j] == " ") : true     # default to true for bottom margin if size is odd
        s += high ? (low ? full : uphalf) : (low ? lowhalf : empty)
      end
      s += full
      s += "</div>"
      webserver.content_send(s)
    end
    # webserver.content_send("</div>")
    if sz % 2 == 0
      s = "<div class='qr' style='background-color:#000;'>"
      for i: 0 .. sz + 1    s += uphalf  end
      s += "/<div>"
      webserver.content_send(s)
    end

    webserver.content_send("</div>")
  end

  #- ---------------------------------------------------------------------- -#
  #- Show commissioning information and QR Code
  #- ---------------------------------------------------------------------- -#
  def show_commissioning_info()
    import webserver

    var seconds_left = (self.device.commissioning.commissioning_open - tasmota.millis()) / 1000
    if seconds_left < 0   seconds_left = 0 end
    var min_left = (seconds_left + 30) / 60

    webserver.content_send(f"<fieldset><legend><b>&nbsp;Commissioning open for {min_left:i} min&nbsp;</b></legend><p></p>")

    var pairing_code = self.device.commissioning.compute_manual_pairing_code()
    webserver.content_send(f"<p>Manual pairing code:<br><b>{pairing_code[0..3]}-{pairing_code[4..6]}-{pairing_code[7..]}</b></p><hr>")

    webserver.content_send("<div><center>")
    var qr_text = self.device.commissioning.compute_qrcode_content()
    self.show_qrcode(qr_text)
    webserver.content_send(f"<p> {qr_text}</p>")
    webserver.content_send("</div><p></p></fieldset><p></p>")

  end

  #- ---------------------------------------------------------------------- -#
  #- Show Passcode / discriminator form
  #- ---------------------------------------------------------------------- -#
  def show_passcode_form()
    import webserver

    webserver.content_send("<fieldset><legend><b>&nbsp;Matter Advanced Configuration&nbsp;</b></legend><p></p>"
    
                           "<form action='/matterc' method='post' onsubmit='return confirm(\"This will cause a restart.\");'>"
                           "<p>Passcode:</p>")
    webserver.content_send(f"<input type='number' min='1' max='99999998' name='passcode' value='{self.device.root_passcode:i}'>")
    webserver.content_send("<p>Distinguish id:</p>")
    webserver.content_send(f"<input type='number' min='0' max='4095' name='discriminator' value='{self.device.root_discriminator:i}'>")
    var ipv4only_checked = self.device.ipv4only ? " checked" : ""
    webserver.content_send(f"<p><input type='checkbox' name='ipv4'{ipv4only_checked}>IPv4 only</p>")
    webserver.content_send("<p></p><button name='passcode' class='button bgrn'>Change</button></form></p>"
                           "<p></p></fieldset><p></p>")

  end

  #- ---------------------------------------------------------------------- -#
  #- Show commissioning information and QR Code
  #- ---------------------------------------------------------------------- -#
  def show_fabric_info()
    import webserver

    webserver.content_send("<fieldset><legend><b>&nbsp;Fabrics&nbsp;</b></legend><p></p>"
                           "<p>Associated fabrics:</p>")

    if size(self.device.sessions.sessions) == 0
      webserver.content_send("<p><b>None</b></p>")
    else
      var first = true
      for f : self.device.sessions.fabrics.persistables()
        if !first     webserver.content_send("<hr>") end
        first = false

        var label = f.fabric_label
        if !label   label = "<No label>"    end
        label = webserver.html_escape(label)      # protect against HTML injection
        
        webserver.content_send(f"<fieldset><legend><b>&nbsp;#{f.get_fabric_index():i} {label}</b> ({f.get_admin_vendor_name()})&nbsp;</legend><p></p>")

        var fabric_rev = f.get_fabric_id().copy().reverse()
        var deviceid_rev = f.get_device_id().copy().reverse()
        webserver.content_send(f"Fabric: {fabric_rev.tohex()}<br>")
        webserver.content_send(f"Device: {deviceid_rev.tohex()}<br>&nbsp;")

        webserver.content_send("<form action='/matterc' method='post' onsubmit='return confirm(\"Are you sure?\");'>")
        webserver.content_send(f"<input name='del_fabric' type='hidden' value='{f.get_fabric_index():i}'>")
        webserver.content_send("<button name='del' class='button bgrn'>Delete Fabric</button></form></p>"
        
                               "<p></p></fieldset><p></p>")
      end
    end

    webserver.content_send("<p></p></fieldset><p></p>")

  end

  #----------------------------------------------------------------------- -#
  #- Generate JavaScript with Schema_Map
  #- Streams schemas directly to webserver without intermediate string to minimize memory usage
  #- Uses collect_plugin_schemas() to get all schemas for each plugin
  #- Output format: var ps={"type":{"arg1":"schema1","arg2":"schema2"},...};
  #- Requirements: 3.1, 3.2, 3.3
  #----------------------------------------------------------------------- -#
  def generate_schema_js()
    import string
    import webserver
    
    webserver.content_send("var ps={")
    var first_type = true
    
    for typ: self.device.plugins_classes.keys()
      var cl = self.device.plugins_classes[typ]
      # Use collect_plugin_schemas() to get all schemas (SCHEMA, SCHEMA2, SCHEMA3...)
      var schemas = self.collect_plugin_schemas(cl)
      
      # Only output if at least one schema is defined
      if size(schemas) > 0
        if !first_type
          webserver.content_send(",")
        end
        first_type = false
        
        webserver.content_send(format('"%s":{', typ))
        var first_schema = true
        
        for arg_name: schemas.keys()
          if !first_schema
            webserver.content_send(",")
          end
          first_schema = false
          
          # Escape quotes in schema string for JavaScript
          var schema_escaped = string.replace(schemas[arg_name], '"', '\\"')
          webserver.content_send(format('"%s":"%s"', arg_name, schema_escaped))
        end
        
        webserver.content_send("}")
      end
    end
    
    webserver.content_send("};")
  end

  #----------------------------------------------------------------------- -#
  #- Generate JavaScript with current config and nextep
  #- Sends current plugins_config as `var cfg={...};`
  #- Sends next_ep as `var nextep=N;`
  #- Requirements: 4.7
  #----------------------------------------------------------------------- -#
  def generate_config_js()
    import json
    import webserver
    
    webserver.content_send("var cfg=")
    webserver.content_send(json.dump(self.device.plugins_config))
    webserver.content_send(";")
    webserver.content_send(format("var nextep=%i;", self.device.next_ep))
  end

  #----------------------------------------------------------------------- -#
  #- Generate JavaScript with display names map
  #- Output: var dn={"relay":"Relay","light0":"Light 0 On",...,"-virtual":true};
  #- The "-virtual" key is a separator marker for the type dropdown
  #----------------------------------------------------------------------- -#
  def generate_display_names_js(class_list)
    import webserver
    import json
    import string

    var class_types = []
    for cl: class_list
      class_types += string.split(cl, '|')
    end

    webserver.content_send("var dn={")
    var first = true
    for typ: class_types
      if typ == ''    continue  end
      if !first   webserver.content_send(",")   end
      first = false
      if typ == '-virtual'
        webserver.content_send('"-virtual":true')
      elif typ == '-zigbee'
        webserver.content_send('"-zigbee":true')
      else
        var nam = self.device.get_plugin_class_displayname(typ)
        webserver.content_send(format('"%s":"%s"', typ, nam))
      end
    end
    webserver.content_send("};")
  end

  def show_plugins_hints_js(*class_list)
    import webserver

    webserver.content_send("<script type='text/javascript'>")
    # Generate schema map for dynamic form generation
    self.generate_schema_js()
    # Generate current config and nextep for browser-driven config submission
    self.generate_config_js()
    # Generate display names map for type dropdown
    self.generate_display_names_js(class_list)
    webserver.content_send("</script>")

    webserver.content_send(self._ADD_ENDPOINT_JS)

  end

  #----------------------------------------------------------------------- -#
  #- Show plugins configuration
  #----------------------------------------------------------------------- -#
  def show_plugins_configuration()
    import webserver
    import string
    import introspect
    import json

    # Emit JavaScript data and functions first (before the HTML that uses them)
    if self.device.zigbee
      self.show_plugins_hints_js(self._CLASSES_TYPES_STD, self.device.zigbee._CLASSES_TYPES, self._CLASSES_TYPES_VIRTUAL)
    else
      self.show_plugins_hints_js(self._CLASSES_TYPES_STD, self._CLASSES_TYPES_VIRTUAL)
    end

    webserver.content_send("<fieldset><legend><b>&nbsp;Configuration&nbsp;</b></legend><p></p>")
    webserver.content_send("<form action='/matterc' method='post'>")

    # --- Local sensors and devices ---
    webserver.content_send("<p><b>Local sensors and devices</b></p>")
    webserver.content_send("<table id='epTbl' style='width:100%'>")
    webserver.content_send("<tr>"
                           "<td width='25' style='font-size:smaller;'>#</td>"
                           "<td width='78' style='font-size:smaller;'>Name</td>"
                           "<td width='115' style='font-size:smaller;'>Type</td>"
                           "<td width='15'></td>"
                           "</tr>")

    self.device.plugins_config.remove("0")      # remove any leftover from ancient configuration
    var endpoints = self.device.k2l_num(self.device.plugins_config)
    var i = 0
    var found = false

    while i < size(endpoints)
      var ep = endpoints[i]
      var conf = self.device.plugins_config.find(str(ep))
      var typ = conf.find('type')
      if !typ   i += 1   continue    end

      # skip any remote class
      if string.find(typ, "http_") == 0   i += 1   continue    end

      found = true
      webserver.content_send(f"<script>document.write(genEpRows({ep:i},cfg['{ep:i}'],false))</script>")
      i += 1
    end

    webserver.content_send("</table>")

    if !found
      webserver.content_send("<p id='epNone'>&lt;none&gt;</p>")
    end

    # "+ Add endpoint" link, right-aligned, subtle
    webserver.content_send("<p style='text-align:right'>"
                           "<a href='#' onclick='addEp();return false' style='color:var(--c_btn)'>+ Add endpoint</a>"
                           "</p>")

    # --- Remote devices ---
    var remotes = []
    for conf: self.device.plugins_config
      var url = conf.find("url")
      if url != nil
        remotes.push(url)
      end
    end
    self.device.sort_distinct(remotes)

    for remote: remotes
      var remote_html = webserver.html_escape(remote)
      var host_device_name = webserver.html_escape( self.device.get_plugin_remote_info(remote).find('name', remote) )
      webserver.content_send(f"&#x1F517; <a target='_blank' title='http://{remote_html}/' href=\"http://{remote_html}/?\">{host_device_name}</a>")
      webserver.content_send("<table style='width:100%'>"
                             "<tr>"
                             "<td width='25'></td>"
                             "<td width='78'></td>"
                             "<td width='115'></td>"
                             "<td width='15'></td>"
                             "</tr>")

      found = false
      i = 0
      while i < size(endpoints)
        var ep = endpoints[i]
        var conf = self.device.plugins_config.find(str(ep))
        var typ = conf.find('type')
        if !typ   i += 1   continue    end
        if string.find(typ, "http_") != 0   i += 1   continue    end
        if conf.find("url") != remote   i += 1   continue    end

        found = true
        webserver.content_send(f"<script>document.write(genEpRows({ep:i},cfg['{ep:i}'],false))</script>")
        i += 1
      end

      webserver.content_send("</table><p></p>")

      if !found
        webserver.content_send("<p>&lt;none&gt;</p>")
      end
    end

    # Save configuration button
    webserver.content_send("<button name='config' class='button bgrn'"
                           " onclick='return submitConfig(this.form)'>"
                           "Save configuration</button>"
                           "<input type='hidden' name='config_json' id='config_json'>"
                           "</form>")

    # Add remote endpoint (separate form)
    webserver.content_send("<hr><p><b>Add Remote Tasmota or OpenBK</b></p>"
                           "<form action='/matteradd' method='get'>"
                           "<table style='width:100%'>"
                           "<tr><td width='30' style='font-size:smaller;'><b>http://</b></td>"
                           "<td><input type='text' name='url' size='8' value='' required placeholder='IP or domain'></td>"
                           "<td width='10' style='font-size:smaller;'><b>/</b></td></tr>"
                           "</table>"
                           "<div style='display: block;'></div>"
                           "<button class='button bgrn'>Auto-configure remote Tasmota</button></form><hr>")

    # button "Reset and Auto-discover"
    webserver.content_send("<form action='/matterc' method='post'"
                           " onsubmit='return confirm(\"This will RESET the configuration to the default. You will need to associate again.\");'>"
                           "<button name='auto' class='button bred'>Reset all and Auto-discover</button><p></p></form>"
                           "<p></p></fieldset>")

  end

  #- ---------------------------------------------------------------------- -#
  #- Show pretty name for plugin class
  #- ---------------------------------------------------------------------- -#
  def plugin_name(cur, *class_list)
    if cur == ''  return ''  end
    return self.device.get_plugin_class_displayname(cur)
  end

  #- ---------------------------------------------------------------------- -#
  #- Show all possible classes for plugin
  #- ---------------------------------------------------------------------- -#
  def plugin_option(cur, *class_list)
    import webserver
    import string
    var class_types = []
    for cl: class_list
      class_types += string.split(cl, '|')
    end
    
    var i = 0
    while i < size(class_types)
      var typ = class_types[i]
      if typ == ''
        webserver.content_send("<option value=''></option>")
      elif typ == '-virtual'
        webserver.content_send("<option value='' disabled>--- Virtual Devices ---</option>")
      elif typ == '-zigbee'
        webserver.content_send("<option value='' disabled>--- Zigbee Devices ---</option>")
      else
        var nam = self.device.get_plugin_class_displayname(typ)
        webserver.content_send(format("<option value='%s'%s>%s</option>", typ, (typ == cur) ? " selected" : "", nam))
      end
      i += 1
    end
  end


  #######################################################################
  # Display the advanced configuration page
  #######################################################################
  def page_part_mgr_adv()
    import webserver

    if !webserver.check_privileged_access() return nil end

    webserver.content_start("Matter Advanced Configuration")           #- title of the web page -#
    webserver.content_send_style()                  #- send standard Tasmota styles -#

    if self.matter_enabled
      self.show_passcode_form()
      self.show_fabric_info()
    end
    self.web_add_config_button()
    #webserver.content_button(webserver.BUTTON_CONFIGURATION)
    webserver.content_stop()                        #- end of web page -#
  end


  #######################################################################
  # Display the complete page
  #######################################################################
  def page_part_mgr()
    import webserver

    if !webserver.check_privileged_access() return nil end

    webserver.content_start("Matter")           #- title of the web page -#
    webserver.content_send_style()                  #- send standard Tasmota styles -#

    self.show_enable()
    if self.matter_enabled
      self.show_plugins_configuration()
    end

    webserver.content_send("<div style='display: block;'></div>"
                           "<p></p><form id='butmat' style='display: block;' action='mattera' method='get'><button name=''>Advanced Configuration</button></form>")

    webserver.content_button(webserver.BUTTON_CONFIGURATION)
    webserver.content_stop()                        #- end of web page -#
  end

  #---------------------------------------------------------------------- -#
  # Generate configuration map from Status 10 and Status 11
  #
  # Returns a list of maps: [ {"type":"temperature", "filter":"ESP32#Temperature"} ]
  #---------------------------------------------------------------------- -#
  def generate_config_from_status(status10, status11)
    var config_list = []

    # count `Power` and `Power<x>`
    var power_cnt = 0
    if status11.contains("POWER")
      power_cnt = 1
    else
      var idx = 1
      while true
        if status11.contains("POWER" + str(idx))
          power_cnt = idx
          idx += 1
        else
          break
        end
      end
    end
    # Now `power_cnt` contains the number of Relays including light

    # detect lights
    var light1, light2, light3    # contains a relay number of nil
    if status11.contains("HSBColor")
      light3 = power_cnt
      power_cnt -= 1
    elif status11.contains("CT")
      light2 =  power_cnt
      power_cnt -= 1
    elif status11.contains("Dimmer")
      light1 =  power_cnt
      power_cnt -= 1
    end

    # rest is relays
    for i: 1..power_cnt
      config_list.push({'type': 'light0', 'relay': i})
    end

    # show lights
    if light1 != nil
      config_list.push({'type': 'light1', 'relay': light1})
    end
    if light2 != nil
      config_list.push({'type': 'light2', 'relay': light2})
    end
    if light3 != nil
      config_list.push({'type': 'light3', 'relay': light3})
    end


    # detect sensors
    config_list += self.device.autoconf.autoconf_sensors_list(status10)

    return config_list
  end

  #- ---------------------------------------------------------------------- -#
  #- Probe remote device
  #- ---------------------------------------------------------------------- -#
  def show_remote_autoconf(url)
    import webserver
    import json

    if url == ''  return end
    var timeout = matter.Plugin_Device.PROBE_TIMEOUT
    var http_remote = matter.HTTP_remote(nil, url, timeout)
    # Status 10
    var status10 = http_remote.call_sync('Status 10', timeout)
    if status10 != nil   status10 = json.load(status10)                end
    if status10 != nil   status10 = status10.find('StatusSNS')         end
    # Status 11
    var status11
    if status10 != nil
      status11 = http_remote.call_sync('Status 11', timeout)
      if status11 != nil   status11 = json.load(status11)           end
      if status11 != nil   status11 = status11.find('StatusSTS')     end
    end
    
    if status10 != nil && status11 != nil
      log(format("MTR: probed '%s' status10=%s satus11=%s", url, str(status10), str(status11)), 3)

      var config_list = self.generate_config_from_status(status10, status11)

      # Emit JS data: schemas (ps), display names (dn) for remote types
      webserver.content_send("<script type='text/javascript'>")
      self.generate_schema_js()
      self.generate_display_names_js([self._CLASSES_TYPES2])

      # Emit the detected config as a JS array for client-side rendering
      # Each entry has 'type' prefixed with 'http_' and parameter values
      var rem_configs = []
      for config: config_list
        var entry = {}
        for k: config.keys()
          if k == 'type'
            entry['type'] = 'http_' + config['type']
          else
            entry[k] = config[k]
          end
        end
        rem_configs.push(entry)
      end
      webserver.content_send(format("var remcfg=%s;", json.dump(rem_configs)))
      webserver.content_send(format("var remurl=%s;", json.dump(url)))
      webserver.content_send("</script>")
      webserver.content_send(self._ADD_ENDPOINT_JS)

      webserver.content_send("<fieldset><legend><b>&nbsp;Matter Remote Device&nbsp;</b></legend><p></p>"
                             "<p><b>Add Remote sensor or device</b></p>")

      var remote_html = webserver.html_escape(url)
      webserver.content_send(f"<p>&#x1F517; <a target='_blank' href=\"http://{remote_html}/?\">{remote_html}</a></p>")

      # Form with hidden JSON field, rendered by JS
      webserver.content_send("<form action='/matterc' method='post' onsubmit='return submitRemote(this)'>"
                             "<input name='url' type='hidden' value='" + webserver.html_escape(url) + "'>"
                             "<input name='rem_json' type='hidden' value=''>"
                             "<table id='remTbl' style='width:100%'>"
                             "<tr>"
                             "<td width='25' style='font-size:smaller;'>#</td>"
                             "<td width='78' style='font-size:smaller;'>Name</td>"
                             "<td width='115' style='font-size:smaller;'>Type</td>"
                             "<td width='15'></td>"
                             "</tr>"
                             "</table>"
                             "<div style='display: block;'></div>"
                             "<button name='addrem' class='button bgrn'>Add endpoints</button>"
                             "</form>")

      # JS to populate the table from remcfg and handle submission
      webserver.content_send(
        "<script type='text/javascript'>"
        "var ri=0;"
        # Render detected endpoints
        "remcfg.forEach(function(c){"
        "var tbl=eb('remTbl');"
        "if(tbl)tbl.insertAdjacentHTML('beforeend',genEpRows('r'+ri,c,false));"
        "ri++;});"
        # Add one empty row for new endpoint
        "(function(){"
        "var tbl=eb('remTbl');"
        "if(tbl)tbl.insertAdjacentHTML('beforeend',genEpRows('r'+ri,{type:''},true));"
        "ri++;})();"
        # submitRemote: collect all remote endpoint data into JSON
        "function submitRemote(f){"
        "var eps=[];"
        "for(var i=0;i<ri;i++){"
        "var ne=eb('epNamr'+i);"
        "var te=eb('epTypr'+i);"
        "var typ=te?te.value:remcfg[i]?remcfg[i].type:'';"
        "if(!typ)continue;"
        "var ep={type:typ};"
        "if(ne){var v=ne.value.trim();if(v)ep.name=v;}"
        "var schema=ps?ps[typ]:null;"
        "if(schema){"
        "for(var key in schema){"
        "var def=parseSchema(schema[key]);"
        "var akey=def.a||key;"
        "var fe=eb('epr'+i+'_'+key);"
        "if(fe){"
        "var v=fe.type==='checkbox'?(fe.checked?1:0):fe.value;"
        "if(v!==''&&v!==null&&v!==undefined){"
        "ep[akey]=(def.t==='i')?parseInt(v,10):v;"
        "}}}}"
        "eps.push(ep);}"
        "f.elements['rem_json'].value=JSON.stringify(eps);"
        "return true;}"
        "</script>")

      webserver.content_send("</fieldset>")

    else
      webserver.content_send(format("<p><b>Unable to connect to '%s'</b></p>", webserver.html_escape(url)))
    end


  end

  #######################################################################
  # Display the page for adding a new endpoint
  #######################################################################
  def page_part_mgr_add()
    import webserver

    if !webserver.check_privileged_access() return nil end

    webserver.content_start("Matter Create new endpoint")           #- title of the web page -#
    webserver.content_send_style()                  #- send standard Tasmota styles -#

    var url = webserver.arg("url")
    if self.matter_enabled
      self.show_remote_autoconf(url)
    end
    webserver.content_button(webserver.BUTTON_CONFIGURATION)
    webserver.content_stop()                        #- end of web page -#
  end


  #######################################################################
  # Web Controller, called by POST to `/matterc`
  #######################################################################
  def page_part_ctl()
    import webserver
    if !webserver.check_privileged_access() return nil end

    import string
    import partition_core
    import persist

    var error

    try

      # debug information about parameters
      # for i:0..webserver.arg_size()-1
      #   log(format("MTR: Arg%i '%s' = '%s'", i, webserver.arg_name(i), webserver.arg(i)))
      # end

      #---------------------------------------------------------------------#
      # Change Passcode and/or Passcode
      #---------------------------------------------------------------------#
      if webserver.has_arg("passcode") || webserver.has_arg("discriminator")
        log(format("MTR: /matterc received '%s' command", 'passcode'), 3)
        if webserver.has_arg("passcode")
          self.device.root_passcode = int(webserver.arg("passcode"))
        end
        if webserver.has_arg("discriminator")
          self.device.root_discriminator = int(webserver.arg("discriminator"))
        end
        self.device.ipv4only = webserver.arg("ipv4") == 'on'
        self.device.save_param()

        #- and force restart -#
        webserver.redirect("/?rst=")

      elif webserver.has_arg("save")
        var matter_enabled_requested = webserver.has_arg("menable")
        var matter_commissioning_requested = webserver.has_arg("comm")
        var matter_disable_bridge_mode_requested = (webserver.arg("nobridge") == 'on')
        if self.device.disable_bridge_mode != matter_disable_bridge_mode_requested
          self.device.disable_bridge_mode = matter_disable_bridge_mode_requested
          self.device.save_param()
        end

        if matter_enabled_requested != self.matter_enabled
          if matter_enabled_requested
            log(format("MTR: /matterc received '%s' command", 'enable'), 3)
            tasmota.cmd("SetOption" + str(matter.MATTER_OPTION) + " 1")
          else
            log(format("MTR: /matterc received '%s' command", 'disable'), 3)
            tasmota.cmd("SetOption" + str(matter.MATTER_OPTION) + " 0")
          end
          #- and force restart -#
          webserver.redirect("/?rst=")
        elif matter_commissioning_requested != (self.device.commissioning.commissioning_open != nil)
          if matter_commissioning_requested
            self.device.commissioning.start_root_basic_commissioning()
          else
            self.device.commissioning.stop_basic_commissioning()
          end
        
          #- and force restart -#
          webserver.redirect("/")
        else
          webserver.redirect("/")
        end

      #---------------------------------------------------------------------#
      # Delete Fabric
      #---------------------------------------------------------------------#
      elif webserver.has_arg("del_fabric")
        log(format("MTR: /matterc received '%s' command", 'del_fabric'), 3)
        var del_fabric = int(webserver.arg("del_fabric"))
        var idx = 0
        var fabrics = self.device.sessions.fabrics
        while idx < size(fabrics)
          if fabrics[idx].get_fabric_index() == del_fabric
            self.device.remove_fabric(fabrics[idx])
            break
          else
            idx += 1
          end
        end
        #- reload advanced page -#
        webserver.redirect("/mattera?")

      #---------------------------------------------------------------------#
      # Reset to default auto-configuration
      #---------------------------------------------------------------------#
      elif webserver.has_arg("auto")
        log(format("MTR: /matterc received '%s' command", 'auto'), 3)
        self.device.reset_param()
        #- and force restart -#
        webserver.redirect("/?rst=")

      #---------------------------------------------------------------------#
      # Apply new configuration from full JSON (browser-computed)
      #---------------------------------------------------------------------#
      elif webserver.has_arg("config_json")
        log(format("MTR: /matterc received '%s' command", 'config_json'), 3)
        self.handle_config_json()
        #- and reload -#
        webserver.redirect("/matterc?")

      #---------------------------------------------------------------------#
      # Add new endpoint for remote sensor or device
      #---------------------------------------------------------------------#
      elif webserver.has_arg("addrem")
        var url = webserver.arg('url')
        if url == nil || url == ''    raise "value_error", "url shouldn't be null"  end

        var rem_json_str = webserver.arg('rem_json')
        if rem_json_str != nil && rem_json_str != ''
          import json
          var endpoints = json.load(rem_json_str)
          if endpoints != nil
            for ep_conf: endpoints
              var typ = ep_conf.find('type', '')
              if typ == ''    continue    end
              var typ_class = self.device.plugins_classes.find(typ)
              if typ_class == nil   continue   end

              var config = {'url': url, 'type': typ}
              var nam = ep_conf.find('name')
              if nam    config['name'] = nam    end
              # copy all schema parameters from the submitted config
              for k: ep_conf.keys()
                if k != 'type' && k != 'name'
                  config[k] = ep_conf[k]
                end
              end

              # check if configuration is already present
              var duplicate = false
              for c: self.device.plugins_config   # iterate on values, not on keys()
                if self.equal_map(c, config)   duplicate = true  break   end
              end
              # not a duplicate, add it
              if !duplicate
                log(format("MTR: remote add url='%s' type='%s'", url, typ), 3)
                self.device.bridge_add_endpoint(typ, config)
              end
            end
          end
        end
        #- and go back to Matter configuration -#
        webserver.redirect("/matterc?")

      end

      if error
        webserver.content_start("Parameter error")           #- title of the web page -#
        webserver.content_send_style()                  #- send standard Tasmota styles -#
        webserver.content_send(format("<p style='width:340px;'><b>Error:</b>%s</p>", webserver.html_escape(error)))
        webserver.content_button(webserver.BUTTON_CONFIGURATION) #- button back to configuration page -#
        webserver.content_stop()                        #- end of web page -#
      end

    except .. as e, m
      log(format("BRY: Exception> '%s' - %s", e, m), 2)
      #- display error page -#
      webserver.content_start("Parameter error")           #- title of the web page -#
      webserver.content_send_style()                  #- send standard Tasmota styles -#

      webserver.content_send(format("<p style='width:340px;'><b>Exception:</b><br>'%s'<br>%s</p>", e, m))

      webserver.content_button(webserver.BUTTON_CONFIGURATION) #- button back to configuration page -#
      webserver.content_stop()                        #- end of web page -#
    end
  end

  #######################################################################
  # Show bridge status
  #######################################################################
  def show_bridge_status()
    if (self.device.plugins == nil)   return  end
    import webserver
    var bridge_plugin_by_host
    
    var idx = 0
    while idx < size(self.device.plugins)
      var plg = self.device.plugins[idx]

      if plg.BRIDGE
        if bridge_plugin_by_host == nil     bridge_plugin_by_host = {}   end
        var host = plg.http_remote.addr

        if !bridge_plugin_by_host.contains(host)    bridge_plugin_by_host[host] = []    end
        bridge_plugin_by_host[host].push(plg)

      end
      idx += 1
    end

    if bridge_plugin_by_host == nil     return    end         # no remote device, abort

    # set specific styles
    webserver.content_send("<hr>")
    webserver.content_send("<table style='width:100%'>")
    # MATTER_STYLESHEET
    webserver.content_send(
        "<style>"
        ".bxm{height:14px;width:14px;display:inline-block;border:1px solid currentColor;background-color:var(--cl,#fff)}"
        ".ztdm td:not(:first-child){width:20px;font-size:70%}"
        ".ztdm td:last-child{width:45px}"
        ".ztdm .bt{margin-right:10px;}"
        ".htrm{line-height:20px}"
        "</style>"
    )

    for host: self.device.k2l(bridge_plugin_by_host)
      var host_html = webserver.html_escape(host)
      var host_device_name = webserver.html_escape( self.device.get_plugin_remote_info(host).find('name', host) )
      webserver.content_send(f"<tr class='ztdm htrm'><td>&#x1F517; <a target='_blank' title='http://{host_html}/' href=\"http://{host_html}/?\"'>{host_device_name}</a></td>")
      var http_remote = bridge_plugin_by_host[host][0].http_remote    # get the http_remote object from the first in list
      webserver.content_send(http_remote.web_last_seen())

      for plg: bridge_plugin_by_host[host]
        webserver.content_send("<tr class='htrm'><td colspan='2'>")
        plg.web_values()                                      # show values
        webserver.content_send("</td></tr>")
      end
    end


    webserver.content_send("</table><hr>")

  end

  #- display sensor value in the web UI -#
  def web_sensor()
    import webserver

    if self.matter_enabled

      # mtc0 = close, mtc1 = open commissioning
      var fabrics_count = (self.device.sessions != nil) ? self.device.sessions.count_active_fabrics() : 0
      if fabrics_count == 0
        webserver.content_send(format("<div style='text-align:right;font-size:11px;color:#aaa;padding:0px;'>%s</div>", "Matter: No active association"))
      else
        var plural = fabrics_count > 1
        webserver.content_send(format("<div style='text-align:right;font-size:11px;color:#aaa;padding:0px;'>%s</div>", "Matter: " + str(fabrics_count) + " active association" + (plural ? "s" : "")))
      end

      self.show_bridge_status()

      if self.device.commissioning.is_root_commissioning_open()
        self.show_commissioning_info()
      end

    end
  end

  def web_get_arg()
    import webserver
    if   webserver.has_arg("mtc0")    # Close Commissioning
      self.device.commissioning.stop_basic_commissioning()
    elif webserver.has_arg("mtc1")    # Open Commissioning
      self.device.commissioning.start_root_basic_commissioning()
    end
  end

  #- ---------------------------------------------------------------------- -#
  # respond to web_add_handler() event to register web listeners
  #- ---------------------------------------------------------------------- -#
  #- this is called at Tasmota start-up, as soon as Wifi/Eth is up and web server running -#
  def web_add_handler()
    import webserver
    #- we need to register a closure, not just a function, that captures the current instance -#
    webserver.on("/matterc", / -> self.page_part_mgr(), webserver.HTTP_GET)
    webserver.on("/matterc", / -> self.page_part_ctl(), webserver.HTTP_POST)
    webserver.on("/mattera", / -> self.page_part_mgr_adv(), webserver.HTTP_GET)   # advanced
    webserver.on("/matteradd", / -> self.page_part_mgr_add(), webserver.HTTP_GET)   # add endpoint
  end
end
matter.UI = Matter_UI

# optimization of constants
import solidify
solidify.nocompact(Matter_UI.page_part_ctl)