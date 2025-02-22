var cheap_power = module("cheap_power")

cheap_power.init = def (m)
import re
import string
import cheap_power_base

class CheapPower
  static URLTIME = '%Y-%m-%dT%H:00:00.000Z'
  var zone

  def start_args(idx, payload)
    var zone = string.tolower(payload)
    if !zone
      tasmota.log(f"CheapPower{idx}: a price zone name is expected")
    elif re.match('^(ee|fi|l[tv])$', zone)
      self.zone = '&fields=' + zone
      return nil
    else
      tasmota.log(f"CheapPower{idx} {payload}: unrecognized price zone")
    end
    return true
  end

  def url(rtc)
    var now = rtc['utc']
    return 'https://dashboard.elering.ee/api/nps/price/csv?start=' +
      tasmota.strftime(self.URLTIME, now) + '&end=' +
      tasmota.strftime(self.URLTIME, now + 172800) + self.zone
  end

  def parse(data, prices, times)
    try
      for d: string.split(data,'\n')[1..]
        var c = string.split(string.tr(d,'\042\r',''),';')
        prices.push(real(string.tr(c[2],',','.'))/10)
        times.push(int(c[0]))
      end
    except .. end
    return nil
  end
end
return cheap_power_base
('<a href="https://dashboard.elering.ee/assets/api-doc.html">¢/kWh</a>',
 CheapPower())
end
return cheap_power
