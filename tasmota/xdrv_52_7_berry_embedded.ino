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
  //"def func(x) for i:1..x print('a') end end "
  //"def testreal() return str(1.2+1) end "
  //"def noop() log('noop before'); yield(); log('middle after'); yield(); log('noop after'); end "
  //"log(\"foobar\") "

  // auto-import modules
  // // import alias
  "import wire "

  // Phase 1
  // Prepare the super class that will be eventually in Flash
  "class Tasmota_ntv "
    "var _op, _operators, _rules, _timers, _cmd "

    // Map all native functions to methods
    // Again, this will be eventually pre-compiled
    "var getfreeheap, publish, cmd, getoption, millis, timereached, yield "
    "var respcmnd, respcmndstr, respcmnd_done, respcmnd_error, respcmnd_failed, resolvecmnd "
    "def init_ntv() "
      "import tasmota_ntv "
      "self.getfreeheap = tasmota_ntv.getfreeheap "
      "self.publish = tasmota_ntv.publish "
      "self.cmd = tasmota_ntv.cmd "
      "self.getoption = tasmota_ntv.getoption "
      "self.millis = tasmota_ntv.millis "
      "self.timereached = tasmota_ntv.timereached "
      "self.yield = tasmota_ntv.yield "
      "self._operators = tasmota_ntv._operators "

      "self.respcmnd = tasmota_ntv.respcmnd "
      "self.respcmndstr = tasmota_ntv.respcmndstr "
      "self.respcmnd_done = tasmota_ntv.respcmnd_done "
      "self.respcmnd_error = tasmota_ntv.respcmnd_error "
      "self.respcmnd_failed = tasmota_ntv.respcmnd_failed "
      "self.resolvecmnd = tasmota_ntv.resolvecmnd "
    "end "

    "def init() "
      "self._op = [ "
        "['==', /s1,s2-> str(s1)  == str(s2)],"
        "['!==',/s1,s2-> str(s1)  != str(s2)],"
        "['=',  /f1,f2-> real(f1) == real(f2)],"
        "['!=', /f1,f2-> real(f1) != real(f2)],"
        "['>=', /f1,f2-> real(f1) >= real(f2)],"
        "['<=', /f1,f2-> real(f1) <= real(f2)],"
        "['>',  /f1,f2-> real(f1) >  real(f2)],"
        "['<',  /f1,f2-> real(f1) <  real(f2)],"
      "] "
      "self._rules = {} "
      "self._timers = [] "
      "self._cmd = {} "
      "self.init_ntv() "
    "end "
  "end "

  "class Tasmota: Tasmota_ntv "
    // add `charsinstring(s:string,c:string) -> int``
    // looks for any char in c, and return the position of the first chat
    // or -1 if not found
    "def charsinstring(s,c) "
      "for i:0..size(s)-1 "
        "for j:0..size(c)-1 "
          "if s[i] == c[j] return i end "
        "end "
      "end "
      "return -1 "
    "end "

    // find a key in map, case insensitive, return actual key or nil if not found
    "def findkeyi(m,keyi) "
      "import string "
      "var keyu = string.toupper(keyi) "
      "if classof(m) == map "
        "for k:m.keys() "
          "if string.toupper(k)==keyu || keyi=='?' "
            "return k "
          "end "
        "end "
      "end "
    "end "

    // Rules
    "def addrule(pat,f) self._rules[pat] = f end "

    // # split the item when there is an operator, returns a list of (left,op,right)
    // # ex: "Dimmer>50" -> ["Dimmer",tasmota_gt,"50"]
    "def find_op(item) "
      "import string "
      "var pos = self.charsinstring(item, self._operators) "
      "if pos>=0 "
        "var op_split = string.split(item,pos) "
        // #print(op_split)
        "var op_left = op_split[0] "
        "var op_rest = op_split[1] "
        // # iterate through operators
        "for op: self._op "
          "if string.find(op_rest,op[0]) == 0 "
            "var op_func = op[1] "
            "var op_right = string.split(op_rest,size(op[0]))[1] "
            "return [op_left,op_func,op_right] "
          "end "
        "end "
      "end "
      "return [item, nil, nil] "
    "end "
  
    // Rules trigger if match. return true if match, false if not
    // Note: condition is not yet managed
    "def try_rule(ev, rule, f) "
      "import string "
      "var rl_list = self.find_op(rule) "
      "var e=ev "
      "var rl=string.split(rl_list[0],'#') "
      "for it:rl "
        "found=self.findkeyi(e,it) "
        "if found == nil "
          "return false "
        "end "
        "e=e[found] "
      "end "
      // # check if condition is true
      "if rl_list[1] "
        // # did we find a function
        "if !rl_list[1](e,rl_list[2]) "
          // # condition is not met
          "return false "
        "end "
      "end "
      "f(e,ev) "
      "return true "
    "end "

    // Run rules, i.e. check each individual rule
    // Returns true if at least one rule matched, false if none
    "def exec_rules(ev_json) "
      "import json "
      "var ev = json.load(ev_json) "
      "var ret = false "
      "if ev == nil "
        "print('BRY: ERROR, bad json: '+ev_json, 3) "
      "else "
        "for r: self._rules.keys() "
          "ret = self.try_rule(ev,r,self._rules[r]) || ret "
        "end "
      "end "
      "return ret "
    "end "
  
    "def settimer(delay,f) self._timers.push([self.millis(delay),f]) end "

    "def run_deferred() "
      "var i=0 "
      "while i<self._timers.size() "
        "if self.timereached(self._timers[i][0]) "
          "f=self._timers[i][1] "
          "self._timers.remove(i) "
          "f() "
        "else "
          "i=i+1 "
        "end "
      "end "
    "end "

    // Delay function, internally calls yield() every 10ms to avoid WDT
    "def delay(ms) "
      "var tend = self.millis(ms) "
      "while !self.timereached(tend) "
        "self.yield() "
      "end "
    "end "

    // Add command to list
    "def addcommand(c,f) "
      "self._cmd[c]=f "
    "end "

    "def exec_cmd(cmd, idx, payload) "
      "import json "
      "var payload_json = json.load(payload) "
      "var cmd_found = self.findkeyi(self._cmd, cmd) "
      "if cmd_found != nil "
        "self.resolvecmnd(cmd_found) "  // set the command name in XdrvMailbox.command
        "self._cmd[cmd_found](cmd_found, idx, payload, payload_json) "
        "return true "
      "end "
      "return false "
    "end "

    // Force gc and return allocated memory
    "def gc() "
      "import gc "
      "gc.collect() "
      "return gc.allocated() "
    "end "

  "end "

  // Instantiate tasmota object
  "tasmota = Tasmota() "

  // Not sure how to run call methods from C
  "def _exec_rules(e) return tasmota.exec_rules(e) end "
  "def _run_deferred() return tasmota.run_deferred() end "
  "def _exec_cmd(cmd, idx, payload) return tasmota.exec_cmd(cmd, idx, payload) end "
  "def _gc() return tasmota.gc() end "

  // simple wrapper to load a file
  // prefixes '/' if needed, and simpler to use than `compile()`
  "def load(f) "
    "import string "
    "try "
      // check that the file ends with '.be' of '.bec'
      "var fl = string.split(f,'.') "
      "if (size(fl) <= 1 || (fl[-1] != 'be' && fl[-1] != 'bec')) "
        "raise \"file extension is not '.be' or '.bec'\" "
      "end "
      "var native = f[size(f)-1] == 'c' "
      // add prefix if needed
      "if f[0] != '/' f = '/' + f end "
      // load - works the same for .be and .bec
      "var c = compile(f,'file') "
      // save the compiled bytecode
      "if !native "
        "save(f+'c', c) "
      "end "
      // call the compiled code
      "c() "
    "except .. as e "
      "log(string.format(\"BRY: could not load file '%s' - %s\",f,e)) "
    "end "
  "end "

  // try to load "/autoexec.be"
  // "try compile('/autoexec.be','file')() except .. log('BRY: no /autoexec.bat file') end "

  // Wire
  ;

#endif  // USE_BERRY
