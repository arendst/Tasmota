/*
  xdrv_52_3_berry_embedded.ino - Berry scripting language, embedded code

  Copyright (C) 2021 Stephan Hadinger, Berry language by Guan Wenliang https://github.com/Skiars/berry

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


#ifdef USE_BERRY

/*********************************************************************************************\
 * Handlers for Berry calls and async
 * 
\*********************************************************************************************/

const char berry_prog[] =
  ""
  // create a 'ntv' module to allow functions to be registered in a safe namespace
  // "ntv = module('ntv') "

  // auto-import modules
  // // import alias
  "import energy "

  // Phase 1
  "class Tasmota: Tasmota_ntv "
    // for now the variables are built, need to find a way to push in Flash
    // "def init() "
    // "end "
    // // add `chars_in_string(s:string,c:string) -> int``
    // // looks for any char in c, and return the position of the first char
    // // or -1 if not found
    // // inv is optional and inverses the behavior, i.e. look for chars not in the list
    // "def chars_in_string(s,c,inv) "
    //   "var inverted = inv ? true : false "
    //   "for i:0..size(s)-1 "
    //     "var found = false "
    //     "for j:0..size(c)-1 "
    //       "if s[i] == c[j] found = true end "
    //     "end "
    //     "if inverted != found return i end "
    //   "end "
    //   "return -1 "
    // "end "

    // // find a key in map, case insensitive, return actual key or nil if not found
    // "def find_key_i(m,keyi) "
    //   "import string "
    //   "var keyu = string.toupper(keyi) "
    //   "if classof(m) == map "
    //     "for k:m.keys() "
    //       "if string.toupper(k)==keyu || keyi=='?' "
    //         "return k "
    //       "end "
    //     "end "
    //   "end "
    // "end "

    // # split the item when there is an operator, returns a list of (left,op,right)
    // // # ex: "Dimmer>50" -> ["Dimmer",tasmota_gt,"50"]
    // "def find_op(item) "
    //   "import string "
    //   "var op_chars = '=<>!' "
    //   "var pos = self.chars_in_string(item, op_chars) "
    //   "if pos >= 0 "
    //     "var op_split = string.split(item,pos) "
    //     "var op_left = op_split[0] "
    //     "var op_rest = op_split[1] "
    //     "pos = self.chars_in_string(op_rest, op_chars, true) "
    //     "if pos >= 0 "
    //       "var op_split2 = string.split(op_rest,pos) "
    //       "var op_middle = op_split2[0] "
    //       "var op_right = op_split2[1] "
    //       "return [op_left,op_middle,op_right] "
    //     "end "
    //   "end "
    //   "return [item, nil, nil] "
    // "end "

    // // Rules
    // "def add_rule(pat,f) "
    //   "if !self._rules "
    //     "self._rules={} "
    //   "end "
    //   "self._rules[pat] = f "
    // "end "
  
    // // Rules trigger if match. return true if match, false if not
    // "def try_rule(ev, rule, f) "
    //   "import string "
    //   "var rl_list = self.find_op(rule) "
    //   "var e=ev "
    //   "var rl=string.split(rl_list[0],'#') "
    //   "for it:rl "
    //     "found=self.find_key_i(e,it) "
    //     "if found == nil return false end "
    //     "e=e[found] "
    //   "end "
    //   "var op=rl_list[1]"
    //   "var op2=rl_list[2]"
    //   "if op "
    //     "if   op=='==' "
    //       "if str(e) != str(op2)   return false end "
    //     "elif op=='!==' "
    //       "if str(e) == str(op2)   return false end "
    //     "elif op=='=' "
    //       "if real(e) != real(op2) return false end "
    //     "elif op=='!=' "
    //       "if real(e) == real(op2) return false end "
    //     "elif op=='>' "
    //       "if real(e) <= real(op2) return false end "
    //     "elif op=='>=' "
    //       "if real(e) < real(op2)  return false end "
    //     "elif op=='<' "
    //       "if real(e) >= real(op2) return false end "
    //     "elif op=='<=' "
    //       "if real(e) > real(op2)  return false end "
    //     "end "
    //   "end "
    //   "f(e,ev) "
    //   "return true "
    // "end "

    // // Run rules, i.e. check each individual rule
    // // Returns true if at least one rule matched, false if none
    // "def exec_rules(ev_json) "
    //   "if self._rules "
    //     "import json "
    //     "var ev = json.load(ev_json) "
    //     "var ret = false "
    //     "if ev == nil "
    //       "print('BRY: ERROR, bad json: '+ev_json, 3) "
    //     "else "
    //       "for r: self._rules.keys() "
    //         "ret = self.try_rule(ev,r,self._rules[r]) || ret "
    //       "end "
    //     "end "
    //     "return ret "
    //   "end "
    //   "return false "
    // "end "
  
    // "def set_timer(delay,f) "
    //   "if !self._timers self._timers=[] end "
    //   "self._timers.push([self.millis(delay),f]) "
    // "end "

    // // run every 50ms tick
    // "def run_deferred() "
    //   "if self._timers "
    //     "var i=0 "
    //     "while i<self._timers.size() "
    //       "if self.time_reached(self._timers[i][0]) "
    //         "f=self._timers[i][1] "
    //         "self._timers.remove(i) "
    //         "f() "
    //       "else "
    //         "i=i+1 "
    //       "end "
    //     "end "
    //   "end "
    // "end "

    // // Delay function, internally calls yield() every 10ms to avoid WDT
    // "def delay(ms) "
    //   "var tend = self.millis(ms) "
    //   "while !self.time_reached(tend) "
    //     "self.yield() "
    //   "end "
    // "end "

    // // Add command to list
    // "def add_cmd(c,f) "
    //   "if !self._cmd "
    //     "self._cmd={} "
    //   "end "
    //   "self._cmd[c]=f "
    // "end "

    // "def exec_cmd(cmd, idx, payload) "
    //   "if self._cmd "
    //     "import json "
    //     "var payload_json = json.load(payload) "
    //     "var cmd_found = self.find_key_i(self._cmd, cmd) "
    //     "if cmd_found != nil "
    //       "self.resolvecmnd(cmd_found) "  // set the command name in XdrvMailbox.command
    //       "self._cmd[cmd_found](cmd_found, idx, payload, payload_json) "
    //       "return true "
    //     "end "
    //   "end "
    //   "return false "
    // "end "

    // // Force gc and return allocated memory
    // "def gc() "
    //   "import gc "
    //   "gc.collect() "
    //   "return gc.allocated() "
    // // "end "
    // // simple wrapper to load a file
    // // prefixes '/' if needed, and simpler to use than `compile()`
    // "def load(f) "
    //   "import string "
    //   "try "
    //     // check that the file ends with '.be' of '.bec'
    //     "var fl = string.split(f,'.') "
    //     "if (size(fl) <= 1 || (fl[-1] != 'be' && fl[-1] != 'bec')) "
    //       "raise \"file extension is not '.be' or '.bec'\" "
    //     "end "
    //     "var native = f[size(f)-1] == 'c' "
    //     // add prefix if needed
    //     "if f[0] != '/' f = '/' + f end "
    //     // load - works the same for .be and .bec
    //     "var c = compile(f,'file') "
    //     // save the compiled bytecode
    //     "if !native "
    //       "try "
    //         "self.save(f+'c', c) "
    //       "except .. as e "
    //         "self.log(string.format('BRY: could not save compiled file %s (%s)',f+'c',e)) "
    //       "end "
    //     "end "
    //     // call the compiled code
    //     "c() "
    //     "self.log(string.format(\"BRY: sucessfully loaded '%s'\",f)) "
    //   "except .. as e "
    //     "raise \"io_error\",string.format(\"Could not load file '%s'\",f) " 
    //   "end "

    // "end "

    // //
    // // Event from Tasmota is:
    // // 1. event:string        -- type of event (cmd, rule, ...)
    // // 2. cmd:string          -- name of the command to process
    // // 3. index:int           -- index number
    // // 4. payload:string      -- payload as text, analyzed as json
    // // //
    // "def event(type, cmd, idx, payload) "
    //   "if type=='cmd' return self.exec_cmd(cmd, idx, payload) "
    //   "elif type=='rule' return self.exec_rules(payload) "
    //   "elif type=='mqtt_data' return nil "    // not yet implemented
    //   "elif type=='gc' return self.gc() "
    //   "elif type=='every_50ms' return self.run_deferred() "
    //   "elif self._drivers "
    //     "for d:self._drivers "
    //       "try "
    //         "if   type=='every_second' && d.every_second return d.every_second() "
    //         "elif type=='every_100ms' && d.every_100ms return d.every_100ms() "
    //         "elif type=='web_add_button' && d.web_add_button return d.web_add_button() "
    //         "elif type=='web_add_main_button' && d.web_add_main_button return d.web_add_main_button() "
    //         "elif type=='save_before_restart' && d.save_before_restart return d.save_before_restart() "
    //         "elif type=='web_sensor' && d.web_sensor return d.web_sensor() "
    //         "elif type=='json_append' && d.json_append return d.json_append() "
    //         "elif type=='button_pressed' && d.button_pressed return d.button_pressed() "
    //         "end "
    //       "except .. as e,m "
    //         "import string "
    //         "self.log(string.format('BRY: exception %s - %m',3)) "
    //       "end "
    //     "end "
    //   "end "
    // "end "
    //
    // add driver to the queue of event dispatching
    //
    // "def add_driver(d) "
    //   "if self._drivers "
    //     "self._drivers.push(d) "
    //   "else "
    //     "self._drivers = [d]"
    //   "end "
    // "end "

  "end "

  // Instantiate tasmota object
  "tasmota = Tasmota() "
  "def log(m,l) tasmota.log(m,l) end "
  "def load(f) tasmota.load(f) end "

  // Wire class
  // "class Wire : Wire_ntv "
  //   // read bytes as `bytes()` object
  //   "def read_bytes(addr,reg,size) "
  //     "self._begin_transmission(addr) "
  //     "self._write(reg) "
  //     "self._end_transmission(false) "
  //     "self._request_from(addr,size) "
  //     "var ret=bytes(size) "
  //     "while (self._available()) "
  //       "ret..self._read() "
  //     "end "
  //     "return ret "
  //   "end "
  //   // write bytes from `bytes` object
  //   "def write_bytes(addr,reg,b) "
  //     "self._begin_transmission(addr) "
  //     "self._write(reg) "
  //     "self._write(b) "
  //     "self._end_transmission() "
  //   "end "
  // "end "

  "wire = Wire(0) "
  "wire1 = wire "
  "wire2 = Wire(1) "

  // try to load "/autoexec.be"
  // "try compile('/autoexec.be','file')() except .. log('BRY: no /autoexec.bat file') end "
  ;

const char berry_autoexec[] =
  // load "autoexec.be" using import, which loads either .be or .bec file
  "try "
    "load('autoexec.be') "
  "except .. "
    "log(\"BRY: No 'autoexec.be' file\") " 
  "end "
  ;
#endif  // USE_BERRY
