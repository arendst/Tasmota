var cheap_power = module("cheap_power")

cheap_power.init = def (m)
import re
import string
import json
import cheap_power_base

class CheapPower
  var daystart, zone, host, unit

  def start_args(idx, payload)
    if !payload
      tasmota.log(f"CheapPower{idx}: a price zone name is expected")
    elif re.match('^DK[12]$', payload)
      self.zone = payload
      self.host = 'https://www.elprisenligenu.dk'
      self.unit = 'DKK_per_kWh'
      return nil
    elif re.match('^NO[1-5]$', payload)
      self.zone = payload
      self.host = 'https://www.hvakosterstrommen.no'
      self.unit = 'NOK_per_kWh'
      return nil
    else
      tasmota.log(f"CheapPower{idx} {payload}: unrecognized price zone")
    end
    return true
  end

  def url_string()
    return self.host +
      tasmota.strftime('/api/v1/prices/%Y/%m-%d_', self.daystart) +
      self.zone + '.json'
  end

  def url(rtc,out)
    var now = rtc['local']
    var night = tasmota.time_dump(now)
    night = now - night['hour'] * 3600 - night['min'] * 60 - night['sec']
    self.daystart = night
    out.push(nil)
    out.push(f'<a href="{self.host}">øre/kWh</a>')
    return self.url_string()
  end

  def parse(data, prices, times)
    data = json.load(data)
    if data == nil return nil end
    for i: data.keys()
      var d = data[i]
      prices.push(100.0 * d[self.unit])
      var t = tasmota.strptime(d['time_start'], '%Y-%m-%dT%H:%M:%S')
      times.push(t['epoch'] - number(string.split(t['unparsed'],':')[0])*3600)
    end
    if self.daystart < tasmota.rtc()['utc']
      self.daystart += 86400
      return self.url_string()
    end
    return nil
  end
end
return cheap_power_base(nil,CheapPower())
end
return cheap_power
