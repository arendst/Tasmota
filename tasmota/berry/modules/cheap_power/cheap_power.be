import webserver
import json

var cheap_power = module("cheap_power")

cheap_power.init = def (m)
class CheapPower
  var prices # future prices for up to 48 hours
  var times  # start times of the prices
  var timeout# timeout until retrying to update prices
  var chosen # the chosen time slot
  var channel# the channel to control
  var tz     # the current time zone offset from UTC
  static var PAST = -3600 # minimum timer start age
  static var MULT = .1255 # conversion to ¢/kWh including 25.5% VAT
  static var PREV = 0, PAUSE = 1, UPDATE = 2, NEXT= 3
  static var UI = "<table style='width:100%'><tr>"
    "<td style='width:25%'><button onclick='la(\"&op=0\");'>⏮</button></td>"
    "<td style='width:25%'><button onclick='la(\"&op=1\");'>⏯</button></td>"
    "<td style='width:25%'><button onclick='la(\"&op=2\");'>🔄</button></td>"
    "<td style='width:25%'><button onclick='la(\"&op=3\");'>⏭</button></td>"
    "</tr></table>"
  static var URL0 = 'http://sahkotin.fi/prices?start=', URL1 = '&end='
  static var URLTIME = '%Y-%m-%dT%H:00:00.000Z'

  def init()
    self.prices = []
    self.times = []
  end

  def start(idx)
    if idx == nil || idx < 1 || idx > tasmota.global.devices_present
      tasmota.log(f"CheapPower{idx} is not a valid Power output")
      tasmota.resp_cmnd_failed()
    else
      self.channel = idx - 1
      tasmota.add_driver(self)
      self.update()
      tasmota.resp_cmnd_done()
    end
  end

  def power(on) tasmota.set_power(self.channel, on) end

  # fetch the prices for the next 24 to 48 hours
  def update()
    var wc = webclient()
    var rtc = tasmota.rtc()
    self.tz = rtc['timezone'] * 60
    var now = rtc['utc']
    var url = self.URL0 +
      tasmota.strftime(self.URLTIME, now) + self.URL1 +
      tasmota.strftime(self.URLTIME, now + 172800)
    wc.begin(url)
    var rc = wc.GET()
    if rc == 200
      var data = json.load(wc.get_string())
      wc.close()
      if data data = data.find('prices') end
      var prices = [], times = []
      if data
        for i: data.keys()
          var datum = data[i]
          prices.push(self.MULT * datum['value'])
          times.push(tasmota.strptime(datum['date'],
                                      '%Y-%m-%dT%H:%M:%S.000Z')['epoch'])
        end
        self.timeout = nil
        self.prices = prices
        self.times = times
        self.schedule_chosen(self.find_cheapest(), now, self.PAST)
        return
      end
    else
      wc.close()
      print(f'error {rc} for {url}')
    end
    # We failed to update the prices. Retry in 1, 2, 4, 8, …, 64 minutes.
    if !self.timeout
      self.timeout = 60000
    elif self.timeout < 3840000
      self.timeout = self.timeout * 2
    end
    tasmota.set_timer(self.timeout, /->self.update())
  end

  # determine the cheapest slot
  def find_cheapest()
    var cheapest, N = size(self.prices)
    if N
      cheapest = 0
      for i: 1..N-1
        if self.prices[i] < self.prices[cheapest] cheapest = i end
      end
    end
    return cheapest
  end

  def date_from_now(chosen, now) return self.times[chosen] - now end

  # trigger the timer at the chosen hour
  def schedule_chosen(chosen, now, old)
    tasmota.remove_timer('power_on')
    var d = chosen == nil ? self.PAST : self.date_from_now(chosen, now)
    if d != old self.power(d > self.PAST && d <= 0) end
    if d > 0
      tasmota.set_timer(d * 1000, def() self.power(true) end, 'power_on')
    elif d <= self.PAST
      chosen = nil
    end
    self.chosen = chosen
  end

  def web_add_main_button() webserver.content_send(self.UI) end

  def web_sensor()
    var ch, old = self.PAST, now = tasmota.rtc()['utc']
    var N = size(self.prices)
    if N
      ch = self.chosen
      if ch != nil && ch < N old = self.date_from_now(ch, now) end
      while N
        if self.date_from_now(0, now) > self.PAST break end
        ch = ch ? ch - 1 : nil
        self.prices.pop(0)
        self.times.pop(0)
        N -= 1
      end
    end
    var op = webserver.has_arg('op') ? int(webserver.arg('op')) : nil
    if op == self.UPDATE
      self.update()
      ch = self.chosen
    end
    if !N
    elif op == self.PAUSE
      ch = ch == nil ? self.find_cheapest() : nil
    elif op == self.PREV
      ch = (!ch ? N : ch) - 1
    elif op == self.NEXT
      ch = ch != nil && ch + 1 < N ? ch + 1 : 0
    end
    self.schedule_chosen(ch, now, old)
    var status = ch == nil
      ? '{s}⭘{m}{e}'
      : format('{s}⭙ %s{m}%.3g ¢{e}',
               tasmota.strftime('%Y-%m-%d %H:%M', self.tz + self.times[ch]),
               self.prices[ch])
    tasmota.web_send_decimal(status)
  end
end
return CheapPower()
end
return cheap_power
