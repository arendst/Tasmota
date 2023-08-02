# class containing all registered Alexa/Hue lights
#
# simulates a Hue bridge containing multiple lights

#@ solidify:hue_bridge
var hue_bridge = module("hue_bridge")

hue_bridge.init = def (m)
  class hue_bridge_monad
    var lights

    def init()
      self.lights = {}
    end

    # add a new light by id
    def add_light(id, light, name, model, manuf)
      if type(id) != 'int'                raise "value_error", "id must be of type 'int'" end
      if !isinstance(light, light_state)  raise "value_error", "light must be of class 'light_state'" end
      name = str(name)    # force string type
      if size(name) == 0                  raise "value_error", "missing name" end
      if !model     model = "Unknown" end
      if !manuf     manuf = "Tasmota" end

      self.lights[id] = { 'light': light, 'name':name, 'model': str(model), 'manuf':str(manuf) }
      return light
    end

    def remove_light(id)
      self.lights.remove(id)
    end

    # get id from light object
    def light_to_id(l)
      for id: self.lights.keys()
        if l == self.lights[id]['light']
          return id
        end
      end
    end

    # return the status of a single light by id
    def hue_status(id)
      import hue_ntv
      if self.lights.contains(id)
        return hue_ntv.light_state(self.lights[id]['light'])
      end
    end

    # hue_status takes an id, and return a string with status, or `nil` if not found
    def full_status(id)
      import hue_ntv
      if self.lights.contains(id)
        var l = self.lights[id]
        return hue_ntv.full_state(id, l['light'], l['name'], l['model'], l['manuf'])
      end
    end

    # discovery of new devices, returns a partial json string, or `nil` if no device
    def discover()
      import hue_ntv
      import json

      var fragments = []          # store json elements
      for id: self.lights.keys()
        var fragment = self.full_status(id)
        if fragment
          fragment = '"' + str(id) + '":' + fragment;
          fragments.push(fragment)
        end
      end
      if size(fragments) > 0
        return fragments.concat(",")
      end
    end

    # receive a command from Alexa
    def cmd(id, arg)
      import json

      if !self.lights.contains(id) return end   # not our light, ignore
      var l = self.lights[id]['light']
      var resp = {}     # response

      # parse input json
      # ex: '{"on":true,"bri":117}'
      var hue_cmd = json.load(arg)
      if arg == nil
        tasmota.log("BRY: invalid hue payload: " + str(arg), 3)
        return
      end

      # 'on'
      if hue_cmd.contains('on')
        var onoff = bool(hue_cmd['on'])
        resp['on'] = onoff    # ack command
        l.set_power(onoff)
      end

      # handle xy before Hue/Sat
      # If the request contains both XY and HS, we wan't to give priority to HS
      if hue_cmd.contains('xy')
        var xy_arr = hue_cmd['xy']
        var x = real(xy_arr[0])
        var y = real(xy_arr[1])
        resp['xy'] = [x, y]
        x *= 65536
        y *= 65536
        l.set_xy(x, y)
      end

      # hue
      var huesat_changed = false
      var hue = l.hue
      var sat = l.sat
      if hue_cmd.contains('hue')
        hue = int(hue_cmd['hue'])
        resp['hue'] = hue
        if hue > 65535   hue = 65535 end
        huesat_changed = true
      end

      # sat
      if hue_cmd.contains('sat')
        sat = int(hue_cmd['sat'])
        resp['sat'] = sat
        if sat >= 254  sat = 255 end
        if l.type >= 3
          huesat_changed = true
        end
      end
      if huesat_changed
        l.set_hue16sat(hue, sat)
      end

      # ct
      if hue_cmd.contains('ct')
        var ct = int(hue_cmd['ct'])
        resp['ct'] = ct
        l.set_ct(ct)
      end

      # 'bri' after ct/hue to have the right mode
      if hue_cmd.contains('bri')
        var bri = int(hue_cmd['bri'])
        resp['bri'] = bri
        if bri >= 254   bri = 255 end   #  extend bri value if set to max
        l.set_bri(bri)
      end

      # compute response
      # Ex: [{"success":{"/lights/10/state/on":true}},{"success":{"/lights/10/state/bri":117}}]
      var resp_arr = []
      for r: resp.keys()
        var path = "/lights/" + str(id) + "/state/" + r
        var fragment = {'success': { path: resp[r]}}
        resp_arr.push(fragment)
      end
      if size(resp_arr) > 0   l.signal_change() end
      return json.dump(resp_arr)
    end

    # list group ids, ex: "11,23"
    def groups()
      var resp = []
      for r: self.lights.keys()
        resp.push('"' + str(r) + '"')
      end
      if size(resp) > 0
        return resp.concat(',')
      end
    end
  end

  return hue_bridge_monad()
end

#hue_bridge = hue_bridge.init(hue_bridge) # for non-solidified

return hue_bridge
