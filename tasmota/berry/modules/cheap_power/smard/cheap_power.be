var cheap_power = module("cheap_power")

cheap_power.init = def (m)
import string
import cheap_power_base

class CheapPower
  var zone, tz
  static zones={'AT':8004170,'BE':8004996,'CH':8000259,'CZ':8000261,
                'DE':8004169,'FR':8000254,'HU':8000262,'IT':8000255,
                'LU':8004169,'NL':8000256,'PL':8000257,'SI':8000260}
  def start_args(idx, payload)
    var zone = string.toupper(payload)
    if !zone
      tasmota.log(f"CheapPower{idx}: a price zone name is expected")
    else
      try
        self.zone = self.zones[zone]
        return nil
      except .. end
      tasmota.log(f"CheapPower{idx} {payload}: unrecognized price zone")
    end
    return true
  end

  def url(rtc,out)
    var now = rtc['utc'], hour = tasmota.time_dump(now)
    self.tz = rtc['timezone'] * 60
    hour = now - hour['min'] * 60 - hour['sec']
    out.push(format('{"request_form":[{"format":"CSV","moduleIds":[%d],'
                     '"region":"DE",'
                     '"timestamp_from":%u000,"timestamp_to":%u000,'
                     '"type":"discrete","language":"de"}]}',
                     self.zone, hour, hour + 172800))
    return 'https://www.smard.de/nip-download-manager/nip/download/market-data'
  end

  def parse(data, prices, times)
    try
      for d: string.split(data,'\n')[1..]
        var c = string.split(string.tr(d,'\042\r',''),';')
        if c[2] != '-'
          prices.push(real(string.tr(c[2],',','.'))/10)
          times.push(tasmota.strptime(c[0],'%d.%m.%Y %H:%M')['epoch']-self.tz)
        end
      end
    except .. end
    return nil
  end
end
return cheap_power_base
('<a href="https://www.smard.de/">¢/kWh</a>', CheapPower())
end
return cheap_power
