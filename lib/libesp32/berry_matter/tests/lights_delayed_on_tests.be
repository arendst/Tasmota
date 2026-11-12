#!/usr/bin/env -S ../../berry/berry -g
# "Lights delayed on" (global option, off by default): a dimmable light that is off delays
# a received On for up to DELAYED_ON_MS, so that a following MoveToLevel(WithOnOff) switches
# it on at its own level instead of first restoring the previous brightness.
# Run from this directory: ../../berry/berry -g lights_delayed_on_tests.be
# Optional first argument: directory holding baseline Matter_Plugin_2_Light0.be,
# Matter_Plugin_3_Light1.be, Matter_UI.be and/or Matter_zz_Device.be (missing ones are
# taken from ../src/embedded); unchanged upstream sources fail the first check.
# Production plugins, shadows, publishing, command dispatch and the web UI handler run;
# remote Tasmota lights, the local light driver, Zigbee, time and timers are simulated.
import global
import os
import string
import sys
sys.path().push("../src/embedded")
import matter
var globs = "path,ctypes_bytes_dyn,tasmota,ccronexpr,gpio,light,webclient,load,MD5,lv,light_state,udp,tcpclientasync,log,"
            "lv_clock,lv_clock_icon,lv_signal_arcs,lv_signal_bars,lv_wifi_arcs_icon,lv_wifi_arcs,"
            "lv_wifi_bars_icon,_lvgl,"
for name : string.split(globs, ",") global.(name) = nil end
var files = os.listdir("../src/embedded")
for i : 1..size(files)-1
  var value = files[i]
  var j = i
  while j > 0 && files[j-1] > value
    files[j] = files[j-1]
    j -= 1
  end
  files[j] = value
end
for name : files
  if name[0] == '.' continue end
  var directory = "../src/embedded/"
  if size(_argv) > 1 && (name == "Matter_Plugin_2_Light0.be" || name == "Matter_Plugin_3_Light1.be" ||
                         name == "Matter_UI.be" || name == "Matter_zz_Device.be") && os.path.exists(_argv[1] + "/" + name)
    directory = _argv[1] + "/"
  end
  var file = open(directory + name)
  var source = file.read()
  file.close()
  compile(source)()
end

class TestTasmota
  var now, utc, timers
  def init() self.now = 0 self.utc = 1700000000 self.timers = [] end
  def millis(offset) return self.now + (offset == nil ? 0 : offset) end
  def rtc_utc() return self.utc end
  def rtc(mode) return self.utc end
  def time_reached(deadline) return self.now >= deadline end
  def loglevel(level) return false end
  def get_option(index) return 0 end
  def set_timer(delay, f, id) self.timers.push([self.now + delay, f]) end
  def add_driver(d) end
  def cmd(c) assert(false, "unexpected cmd " + c) end
  # Tasmota changeUIntScale(), for the 0..254 <-> 0..100 conversions
  def scale_uint(num, from_min, from_max, to_min, to_max)
    if num > from_max  num = from_max  end
    if num < from_min  num = from_min  end
    if num == from_min  return to_min  end
    if num == from_max  return to_max  end
    var result
    if to_max - to_min > from_max - from_min
      result = (((num - from_min) * (to_max - to_min) * 2) / (from_max - from_min) + 1) / 2 + to_min
    else
      result = (((num - from_min) * 2 + 1) * (to_max - to_min + 1)) / ((from_max - from_min + 1) * 2) + to_min
    end
    return result > to_max ? to_max : result
  end
end
tasmota = TestTasmota()
import introspect
var crypto = module("crypto")
var next_id = 0
crypto.random = def (count)
  next_id += 1
  return bytes().add(next_id, count)
end
introspect.setmodule("crypto", crypto)
# Local light driver (Berry `light` module): power and bri 0..255; `seen` records the visible level
# after each call (0 = off). `light.set()` applies power before bri, as in xdrv_52_3_berry_light.ino.
class FakeLocal
  var power, bri, calls, seen
  def init(power, bri) self.power = power self.bri = bri self.calls = [] self.seen = [] end
end
var local = nil
var light_module = module("light")
light_module.get = def (index)
  if local == nil   assert(false, "bridged light used the local light driver")   end
  if index != 0   return nil   end
  return {'power': local.power, 'bri': local.bri}
end
light_module.set = def (values, index)
  if local == nil   assert(false, "bridged light used the local light driver")   end
  local.calls.push(values)
  if values.contains('power')   local.power = values['power']   local.seen.push(local.power ? local.bri : 0)   end
  if values.contains('bri')     local.bri = values['bri']       local.seen.push(local.power ? local.bri : 0)   end
end
introspect.setmodule("light", light_module)
var logs = []
log = def (msg, level) logs.push(msg) end
var published = []
matter.publish_command = def (prefix, endpoint, name, payload) published.push(payload) end

# Remote Tasmota 9.x light over HTTP: `Power` restores the stored Dimmer, `Dimmer`/`CT`
# switch it on unless SetOption20 is set. `seen` records every visible brightness (0 = off).
class FakeLamp
  var power, dimmer, ct, so20, commands, seen
  def init(power, dimmer) self.power = power self.dimmer = dimmer self.ct = 300 self.so20 = false self.commands = [] self.seen = [] end
  def onoff() return self.power ? "ON" : "OFF" end
  def state() return '"POWER":"' + self.onoff() + '","Dimmer":' + str(self.dimmer) + ',"CT":' + str(self.ct) end
  def run(cmd)
    self.commands.push(cmd)
    var parts = string.split(cmd, " ")
    var verb = parts[0]
    var arg = size(parts) > 1 ? int(parts[1]) : nil
    var reply
    if verb == "Status"
      return "{\"StatusSTS\":{" + self.state() + "}}"
    elif verb == "Power1"
      self.power = (arg == 1)
      reply = '{"POWER":"' + self.onoff() + '"}'
    elif verb == "Dimmer"
      if arg == 0   self.power = false   else   self.dimmer = arg   if !self.so20   self.power = true   end   end
      reply = "{" + self.state() + "}"
    elif verb == "CT"
      self.ct = arg
      if !self.so20   self.power = true   end
      reply = "{" + self.state() + "}"
    else
      assert(false, "unexpected remote command " + cmd)
    end
    self.seen.push(self.power ? self.dimmer : 0)
    return reply
  end
  def max_seen()
    var m = 0
    for v : self.seen   if v > m   m = v   end   end
    return m
  end
end

class MemoryRemote : matter.HTTP_remote
  var time_start, http_status, addr, lamp
  def init(device)
    self.device = device
    self.reachable = false
    self.probe_update_time_map = {}
    self.probe_next_timestamp_map = {}
    self.async_cb_map = {}
    self.info = {}
    self.time_start = 0
    self.http_status = 0
    self.addr = ""
  end
  def call_sync(cmd, timeout) return self.lamp.run(cmd) end
end
class MQTTBoundary
  var reachable, sent
  def init() self.reachable = true self.sent = [] end
  def get_info() return {} end
  def add_async_cb(cb, cmd) end
  def can_send() return true end
  def call_sync(cmd, timeout) self.sent.push(cmd) return nil end
end
class FakeMapper
  var shortaddr, sent
  def init() self.shortaddr = 0x1234 self.sent = [] end
  def parse_configuration(config) end
  def resolve_zb_device() return true end
  def zb_single_command(key, value)              # as Matter_Zigbee_Mapper: only Power, Bri (ZbSend Dimmer) and CT
    if key == 'Power' || key == 'Bri' || key == 'CT'   self.sent.push(f"{key} {value}")   end
  end
end
class MemoryDevice : matter.Device
  static var FILENAME = "_lights_delayed_on_test.json"
  var notifications
  def init()
    self.plugins = []
    self.http_remotes = {}
    self.plugins_config_remotes = {}
    self.notifications = []
    self.tick = 1
    self.debug = false
    self.disable_bridge_mode = false
    try   self.lights_delayed_on = true   except .. end   # option on (absent in a baseline); default checked below
  end
  def register_http_remote(addr, timeout)
    if !self.http_remotes.contains(addr)   self.http_remotes[addr] = MemoryRemote(self)   end
    return self.http_remotes[addr]
  end
  var mqtt_boundary
  def register_mqtt_remote(topic) self.mqtt_boundary = MQTTBoundary() return self.mqtt_boundary end
  def create_zb_mapper(pi) return FakeMapper() end
  def attribute_updated(endpoint, cluster, attribute, fabric_specific)
    self.notifications.push([endpoint, cluster, attribute])
  end
  def http_reachable_changed(remote) end
end

var TLV = matter.TLV
var device
def json_load(s) import json return json.load(s) end

def advance(ms)
  tasmota.now += ms
  device.tick += 1
  var i = 0
  while i < size(tasmota.timers)
    if tasmota.timers[i][0] <= tasmota.now
      var f = tasmota.timers[i][1]
      tasmota.timers.remove(i)
      f()
    else
      i += 1
    end
  end
end

def reset()
  tasmota.now = 1000
  tasmota.timers = []
  logs.clear()
  published.clear()
  local = nil
  device = MemoryDevice()
end

# A fresh bridged CT light (http_light2, as endpoint 2 on the bridge) with a lamp in the given state
def bridged(power, dimmer, plugin_class, config)
  reset()
  var plugin = (plugin_class != nil ? plugin_class : matter.Plugin_Bridge_Light2)(device, 2,
               config != nil ? config : {"url": "192.0.2.2", "relay": 1})
  if plugin.http_remote != nil
    plugin.http_remote.lamp = FakeLamp(power, dimmer)
    plugin.parse_status(json_load('{"POWER":"' + (power ? "ON" : "OFF") + '","Dimmer":' + str(dimmer) + '}'), 11)
  end
  device.notifications.clear()
  device.tick += 1
  return plugin
end

def invoke(plugin, cluster, command, fields)
  var ctx = matter.Path(plugin.endpoint, cluster, nil)
  ctx.command = command
  var val = TLV.Matter_TLV_struct()
  if fields != nil
    for i : 0..size(fields)-1   val.add_TLV(i, 0x05, fields[i])   end
  end
  var res = plugin.invoke_request(nil, val, ctx)
  device.tick += 1
  return [res, ctx]
end
def on(p)              return invoke(p, 0x0006, 0x01) end
def off(p)             return invoke(p, 0x0006, 0x00) end
def level_onoff(p, b)  return invoke(p, 0x0008, 0x04, [b]) end
def ct(p, mireds)      return invoke(p, 0x0300, 0x0A, [mireds]) end
def commands(p)        return p.http_remote.lamp.commands end
def lamp(p)            return p.http_remote.lamp end
def logged(text)
  for m : logs   if string.find(m, text) >= 0   return true   end   end
  return false
end
def mqtt(power, dimmer)
  var p = bridged(power, dimmer, matter.Plugin_Bridge_Light1, {"topic": "lamp", "relay": 1})
  p.parse_status(json_load('{"POWER":"' + (power ? "ON" : "OFF") + '","Dimmer":' + str(dimmer) + '}'), 11)
  device.notifications.clear()
  return p
end
def local_light(power, bri_255)
  reset()
  local = FakeLocal(power, bri_255)
  var p = matter.Plugin_Light1(device, 3, {})
  p.update_shadow()
  device.notifications.clear()
  device.tick += 1
  return p
end
def virtual_light(plugin_class, config)
  reset()
  var p = plugin_class(device, 5, config != nil ? config : {})
  device.tick += 1
  return p
end

var passed = 0
def check(name, f)
  f()
  passed += 1
  print("  " + name + ": OK")
end

print("Lights delayed on:")

check("HTTP: dim scene (On, then 5%) from off at 100%: lamp never visible above 5%", def ()
  var p = bridged(false, 100)
  on(p)
  advance(120)
  level_onoff(p, 14)
  advance(700)
  ct(p, 460)
  advance(1000)
  assert(lamp(p).max_seen() == 5, "the lamp was visible above 5%: " + str(lamp(p).seen))
end)

check("option off (default): On is sent at once, as before", def ()
  var p = bridged(false, 100)
  device.lights_delayed_on = nil                 # what load_param() gives without the key is checked below
  on(p)
  assert(p.on_hold == nil && commands(p) == ["Status 11", "Power1 1"] && size(tasmota.timers) == 0, str(commands(p)))
  level_onoff(p, 14)
  assert(lamp(p).seen == [100, 5], "default behaviour changed: " + str(lamp(p).seen))
  var l = local_light(false, 255)
  device.lights_delayed_on = false
  on(l)
  assert(l.on_hold == nil && str(local.calls) == "[{'power': true}]", str(local.calls))
end)

check("HTTP: command, shadow and publish details", def ()
  var p = bridged(false, 100)
  var r = on(p)
  assert(r[0] == true && r[1].log == "delayed", "On must succeed while delayed")
  assert(size(commands(p)) == 0, "a delayed On must not reach the remote")
  assert(p.shadow_onoff == true && p.on_hold != nil, "a delayed On is reported as on")
  assert(device.notifications.find([2, 0x0006, 0x0000]) != nil, "OnOff change must be reported")
  assert(size(published) == 0, "MtrReceived is published when the On is sent")
  advance(120)
  r = level_onoff(p, 14)
  assert(r[0] == true && r[1].log == "bri:14")
  assert(commands(p) == ["Dimmer 5"], "only the level goes to the remote: " + str(commands(p)))
  assert(published == ['"Power":1,"Bri":14,"Dimmer":5'], str(published))
  assert(lamp(p).power && lamp(p).dimmer == 5)
  assert(p.on_hold == nil && p.shadow_onoff && p.shadow_bri == 13)
  assert(logged("delayed On merged into level 14 after 120 ms"))
  advance(1000)
  assert(commands(p) == ["Dimmer 5"], "expired timer of a merged delay must do nothing")
  assert(size(tasmota.timers) == 0)
end)

check("HTTP: full scene (On, then 100%) from off at 5%: straight to full", def ()
  var p = bridged(false, 5)
  on(p)
  advance(50)
  level_onoff(p, 254)
  advance(1000)
  assert(commands(p) == ["Dimmer 100"], str(commands(p)))
  assert(lamp(p).seen == [100] && p.shadow_onoff && p.shadow_bri == 254)
end)

check("HTTP: plain On is sent when the delay expires (600 ms)", def ()
  var p = bridged(false, 60)
  on(p)
  advance(599)
  assert(size(commands(p)) == 0, "sent before the delay expired")
  advance(1)
  assert(commands(p) == ["Power1 1"], str(commands(p)))
  assert(published == ['"Power":1'], str(published))
  assert(lamp(p).power && lamp(p).dimmer == 60 && p.shadow_onoff && p.on_hold == nil)
  assert(logged("delayed On sent after 600 ms, no level command"))
end)

check("HTTP: On while already on is sent immediately", def ()
  var p = bridged(true, 80)
  on(p)
  assert(p.on_hold == nil && commands(p)[-1] == "Power1 1", str(commands(p)))
  assert(size(tasmota.timers) == 0)
end)

check("HTTP: Off, Toggle and OnWithTimedOff during a delay", def ()
  var p = bridged(false, 100)
  on(p)  advance(50)  off(p)  advance(1000)
  assert(commands(p) == ["Power1 0"] && !lamp(p).power && lamp(p).max_seen() == 0 && !p.shadow_onoff, str(commands(p)))
  p = bridged(false, 100)
  on(p)  advance(50)  invoke(p, 0x0006, 0x02)  advance(1000)
  assert(commands(p) == ["Power1 0"] && !lamp(p).power && !p.shadow_onoff, str(commands(p)))
  p = bridged(false, 70)
  on(p)  advance(50)  invoke(p, 0x0006, 0x42, [0, 0, 0])  advance(1000)
  assert(commands(p) == ["Power1 1"] && lamp(p).power && p.shadow_onoff, str(commands(p)))
end)

check("HTTP: polls and reads during a delay keep the reported On", def ()
  var p = bridged(false, 100)
  on(p)
  advance(50)
  p.parse_http_response(1, '{"StatusSTS":{"POWER":"OFF","Dimmer":100}}', "Status 11")
  assert(p.shadow_onoff == true, "scheduled poll undid the delayed On")
  advance(10)
  var tlv = p.read_attribute(nil, matter.Path(2, 0x0006, 0x0000), TLV.Matter_TLV_item())
  assert(commands(p)[-1] == "Status 11" && tlv.val == true, "OnOff read returned off during a delay")
  advance(20)
  level_onoff(p, 14)
  assert(lamp(p).max_seen() == 5 && commands(p)[-1] == "Dimmer 5")
end)

check("HTTP: SetOption20 remote, level 0, colour first, repeated On", def ()
  var p = bridged(false, 100)
  lamp(p).so20 = true
  on(p)  advance(80)  invoke(p, 0x0008, 0x00, [14])  advance(1000)
  assert(commands(p) == ["Dimmer 5", "Power1 1"] && lamp(p).power && lamp(p).max_seen() == 5, str(commands(p)))
  p = bridged(false, 100)
  on(p)  advance(40)  level_onoff(p, 0)  advance(1000)
  assert(commands(p) == ["Dimmer 0"] && !lamp(p).power && !p.shadow_onoff, str(commands(p)))
  p = bridged(false, 100)
  on(p)  advance(40)  ct(p, 460)
  assert(commands(p)[0] == "Power1 1" && commands(p)[-1] == "CT 460", str(commands(p)))
  assert(logged("delayed On sent after 40 ms, before another command"))
  p = bridged(false, 100)
  on(p)  advance(40)  invoke(p, 0x0008, 0x06, [0, 10, 0])  advance(1000)     # StepWithOnOff: accepted no-op
  assert(commands(p) == ["Power1 1", "Status 11"], "delayed On must be sent once, first: " + str(commands(p)))
  p = bridged(false, 100)
  on(p)  advance(100)  on(p)
  assert(size(tasmota.timers) == 1 && size(commands(p)) == 0, "repeated On must stay delayed: " + str(commands(p)))
  advance(500)
  assert(commands(p) == ["Power1 1"], str(commands(p)))
end)

check("on/off-only lights are never delayed", def ()
  var p0 = bridged(false, 100, matter.Plugin_Bridge_Light0)
  on(p0)
  assert(p0.on_hold == nil && commands(p0)[-1] == "Power1 1", "http_light0 must not delay")
  var v0 = virtual_light(matter.Plugin_Virt_Light0)
  on(v0)
  assert(v0.on_hold == nil && published[-1] == '"Power":1' && size(tasmota.timers) == 0)
end)

check("MQTT: level after On sends Dimmer then Power, never Power first", def ()
  var p = mqtt(false, 100)
  on(p)
  assert(size(device.mqtt_boundary.sent) == 0 && p.shadow_onoff, str(device.mqtt_boundary.sent))
  p.parse_status(json_load('{"POWER":"OFF","Dimmer":100}'), 11)     # late RESULT/STATE
  assert(p.shadow_onoff == true, "MQTT state undid the delayed On")
  advance(200)
  level_onoff(p, 14)
  assert(device.mqtt_boundary.sent == ["Dimmer 5", "Power1 1"], str(device.mqtt_boundary.sent))
  var q = mqtt(false, 100)
  on(q)  advance(600)
  assert(device.mqtt_boundary.sent == ["Power1 1"], str(device.mqtt_boundary.sent))
  var z = mqtt(false, 100)
  on(z)  advance(40)  level_onoff(z, 0)
  assert(device.mqtt_boundary.sent == ["Dimmer 0"] && !z.shadow_onoff, str(device.mqtt_boundary.sent))
end)

check("local light: level is set before power, previous level never shown", def ()
  var p = local_light(false, 255)
  on(p)
  assert(size(local.calls) == 0 && p.shadow_onoff, "On reached the light driver: " + str(local.calls))
  advance(100)
  assert(p.read_attribute(nil, matter.Path(3, 0x0006, 0x0000), TLV.Matter_TLV_item()).val == true)
  p.update_shadow()
  assert(p.shadow_onoff == true, "light.get() undid the delayed On")
  level_onoff(p, 14)
  assert(str(local.calls) == "[{'bri': 14}, {'power': true}]", str(local.calls))
  assert(local.seen == [0, 14] && local.power && p.shadow_onoff, str(local.seen))
  var q = local_light(false, 255)
  on(q)  advance(600)
  assert(str(local.calls) == "[{'power': true}]" && local.seen == [255])
end)

check("virtual light: one MtrReceived with power and level", def ()
  var p = virtual_light(matter.Plugin_Virt_Light1)
  on(p)
  assert(size(published) == 0 && p.shadow_onoff)
  p.update_virtual({"Power": 0})                  # stale MtrUpdate during the delay
  p.update_virtual({"Power": 0, "Bri": 254})
  assert(p.shadow_onoff)
  advance(300)
  invoke(p, 0x0008, 0x00, [14])                   # MoveToLevel (no OnOff) still switches on
  assert(published == ['"Power":1,"Bri":14,"Dimmer":5'], str(published))
  assert(p.shadow_onoff && p.shadow_bri == 14)
  var q = virtual_light(matter.Plugin_Virt_Light1)
  on(q)  advance(600)
  assert(published == ['"Power":1'], str(published))
end)

check("Zigbee light: only ZbSend Dimmer (MoveToLevelWithOnOff)", def ()
  var p = virtual_light(matter.Plugin_Zigbee_Light1, {"zigbee_device": "0x1234"})
  on(p)
  assert(size(p.zigbee_mapper.sent) == 0)
  advance(300)
  level_onoff(p, 14)
  assert(p.zigbee_mapper.sent == ["Bri 14"], str(p.zigbee_mapper.sent))
  var q = virtual_light(matter.Plugin_Zigbee_Light1, {"zigbee_device": "0x1234"})
  on(q)  advance(600)
  assert(q.zigbee_mapper.sent == ["Power 1"], str(q.zigbee_mapper.sent))
end)

check("setting is saved only when on, and defaults to off", def ()
  import os
  reset()
  device.root_discriminator = 1  device.root_passcode = 2  device.next_ep = 2
  device.configuration_version = 1  device.ipv4only = false  device.plugins_persist = false
  device.lights_delayed_on = false
  var j = device.save_param()                     # MemoryDevice.FILENAME: a temporary file
  assert(string.find(j, "lights_delayed_on") < 0, "off must not add a key: " + j)
  device.lights_delayed_on = nil
  device.load_param()
  assert(device.lights_delayed_on == false, "missing key must load as off")
  device.lights_delayed_on = true
  j = device.save_param()
  assert(string.find(j, ',"lights_delayed_on":true') >= 0, j)
  device.lights_delayed_on = false
  device.load_param()
  assert(device.lights_delayed_on == true, "saved setting not loaded")
  os.remove(device.FILENAME)
end)

var web_args = {}
var web_sent = []
var web_redirect = nil
var ws = module("webserver")
ws.check_privileged_access = def () return true end
ws.has_arg = def (n) return web_args.contains(n) end
ws.arg = def (n) return web_args.find(n) end
ws.redirect = def (u) web_redirect = u end
ws.content_send = def (s) web_sent.push(s) end
introspect.setmodule("webserver", ws)
introspect.setmodule("partition_core", module("partition_core"))
introspect.setmodule("persist", module("persist"))
check("web UI: checkbox shown, saved without restart, Force Static still restarts", def ()
  reset()
  device.root_discriminator = 1  device.root_passcode = 2  device.next_ep = 2
  device.configuration_version = 1  device.ipv4only = false  device.plugins_persist = false
  device.lights_delayed_on = false
  class FakeComm var commissioning_open end
  device.commissioning = FakeComm()
  def saved() var f = open(device.FILENAME) var r = f.read() f.close() return r end
  var ui = matter.UI(device, true)
  web_sent.clear()  ui.show_enable()
  assert(string.find(web_sent.concat(), "name='ldon'><b>Lights delayed on</b>") >= 0, web_sent.concat())
  web_args = {"save": "", "menable": "on", "ldon": "on"}
  ui.page_part_ctl()
  assert(device.lights_delayed_on == true && web_redirect == "/", str([device.lights_delayed_on, web_redirect]))
  assert(string.find(saved(), '"lights_delayed_on":true') >= 0, saved())
  web_sent.clear()  ui.show_enable()
  assert(string.find(web_sent.concat(), "name='ldon' checked>") >= 0)
  web_args = {"save": "", "menable": "on"}
  ui.page_part_ctl()
  assert(device.lights_delayed_on == false && web_redirect == "/" && string.find(saved(), "lights_delayed_on") < 0, saved())
  web_args = {"save": "", "menable": "on", "nobridge": "on", "ldon": "on"}
  ui.page_part_ctl()
  assert(device.lights_delayed_on == true && web_redirect == "/?rst=", "bridge change must still restart: " + str(web_redirect))
  assert(string.find(saved(), '"disable_bridge_mode":true') >= 0 && string.find(saved(), '"lights_delayed_on":true') >= 0, saved())
  assert(device.disable_bridge_mode == false, "live topology unchanged until restart")
  os.remove(device.FILENAME)
end)

print(f"Lights delayed on: {passed} passed, 0 failed")
