var cheap_power = module("cheap_power")

cheap_power.init = def (m)
import re
import json
import cheap_power_base

class CheapPower
  var daystart, zone

  def start_args(idx, payload)
    if !payload
      tasmota.log(f"CheapPower{idx}: a price zone name is expected")
    elif re.match('^SE[1-4]$', payload)
      self.zone = payload
      return nil
    else
      tasmota.log(f"CheapPower{idx} {payload}: unrecognized price zone")
    end
    return true
  end

  def url_string()
    return 'https://mgrey.se/espot?format=json&domain=' + self.zone +
      '&date=' + tasmota.strftime('%Y-%m-%d', self.daystart)
  end

  def url(rtc)
    var now = rtc['local']
    var daystart = tasmota.time_dump(now)
    daystart = rtc['utc'] -
      daystart['hour'] * 3600 - daystart['min'] * 60 - daystart['sec']
    self.daystart = daystart
    return self.url_string()
  end

  def parse(data, prices, times)
    data = json.load(data)
    if data == nil return nil end
    var d = data.find(self.zone)
    if d
      for i: d.keys()
        var datum = d[i]
        prices.push(datum['price_sek'])
        times.push(datum['hour'] * 3600 + self.daystart)
      end
    end
    if self.daystart < tasmota.rtc()['utc']
      self.daystart += 86400
      return self.url_string()
    end
    return nil
  end
end
return cheap_power_base
('<a href="https://mgrey.se/espot">öre/kWh</a>', CheapPower())
end
return cheap_power
