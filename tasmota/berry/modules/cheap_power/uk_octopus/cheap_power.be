var cheap_power = module("cheap_power")

cheap_power.init = def (m)
import string
import json
import cheap_power_base

class CheapPower
  var tariff, zone

  def start_args(idx, payload)
    if !payload
      tasmota.log(f"CheapPower{idx}: expected tariff zone")
    else
      var s = string.split(payload, ' ')
      if size(s) != 2
        tasmota.log(f"CheapPower{idx} {payload}: expected tariff zone")
      else
        self.tariff = s[0]
        self.zone = s[1]
        return nil
      end
    end
    return true
  end

  def url(rtc)
    return format('https://api.octopus.energy/v1/products/%s/electricity-tariffs/E-1R-%s-%s/standard-unit-rates/',
      self.tariff, self.tariff, self.zone)
  end

  def parse(data, prices, times)
    data = json.load(data)
    if data == nil return nil end
    var d = data.find('results')
    if d
      var now = tasmota.rtc()['utc']
      for i: d.keys()
        var datum = d[i]
        var date = tasmota.strptime(datum['valid_from'],
                                    '%Y-%m-%dT%H:%M:%SZ')['epoch']
        if date < now
          break
        else
          times.insert(0, date)
          prices.insert(0, datum['value_inc_vat'])
        end
      end
    end
    return nil
  end
end
return cheap_power_base
('<a href="https://octopus.energy/blog/agile-smart-home-diy/">p/kWh</a>',
 CheapPower())
end
return cheap_power
