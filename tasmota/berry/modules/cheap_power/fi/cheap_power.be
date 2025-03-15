var cheap_power = module("cheap_power")

cheap_power.init = def (m)
import json
import cheap_power_base

class CheapPower
  static URLTIME = '%Y-%m-%dT%H:00:00.000Z'
  static MULT = .1255 # conversion to ¢/kWh including 25.5% VAT

  def start_args() end

  def url(rtc)
    var now = rtc['utc']
    return 'https://sahkotin.fi/prices?start=' +
      tasmota.strftime(self.URLTIME, now) + '&end=' +
      tasmota.strftime(self.URLTIME, now + 172800)
  end

  def parse(data, prices, times)
    data = json.load(data)
    if data == nil return nil end
    var d = data.find('prices')
    if d
      for i: d.keys()
        var datum = d[i]
        prices.push(self.MULT * datum['value'])
        times.push(tasmota.strptime(datum['date'],
                                    '%Y-%m-%dT%H:%M:%S.000Z')['epoch'])
      end
    end
    return nil
  end
end
return cheap_power_base
('<a href="https://sahkotin.fi">¢/kWh</a>', CheapPower())
end
return cheap_power
