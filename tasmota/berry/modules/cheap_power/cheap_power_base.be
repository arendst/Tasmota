var cheap_power_base = module("cheap_power_base")

cheap_power_base.init = def (m)

import binary_heap
var make_heap = binary_heap.make_heap, remove_heap = binary_heap.remove_heap
import webserver

class CheapPowerBase
  var plugin # the data source
  var prices # future prices for up to 48 hours
  var times  # start times of the prices
  var timeout# timeout until retrying to update prices
  var chosen # the chosen time slots
  var slots  # the maximum number of time slots to choose
  var channel# the channel to control
  var tz     # the current time zone offset from UTC
  var p_kWh  # currency unit/kWh
  var past   # minimum timer start age (one time slot duration)
  static PREV = 0, NEXT = 1, PAUSE = 2, UPDATE = 3, LESS = 4, MORE = 5
  static UI = "<table style='width:100%'><tr>"
    "<td style='width:12.5%'><button onclick='la(\"&op=0\");'>⏮</button></td>"
    "<td style='width:12.5%'><button onclick='la(\"&op=1\");'>⏭</button></td>"
    "<td style='width:25%'><button onclick='la(\"&op=2\");'>⏯</button></td>"
    "<td style='width:25%'><button onclick='la(\"&op=3\");'>🔄</button></td>"
    "<td style='width:12.5%'><button onclick='la(\"&op=4\");'>➖</button></td>"
    "<td style='width:12.5%'><button onclick='la(\"&op=5\");'>➕</button></td>"
    "</tr></table>"

  def init(p_kWh, plugin)
    self.p_kWh = p_kWh
    self.plugin = plugin
    self.past = -900
    self.slots = 1
    self.prices = []
    self.times = []
    self.tz = 0
  end

  def start(idx, payload)
    if self.start_args(idx) && !self.plugin.start_args(idx, payload)
      self.start_ok()
    end
  end

  def start_args(idx)
    if !idx || idx < 1 || idx > tasmota.global.devices_present
      tasmota.log(f"CheapPower{idx} is not a valid Power output")
      return self.start_failed()
    else
      self.channel = idx - 1
      return true
    end
  end

  def start_ok()
    tasmota.add_driver(self)
    tasmota.set_timer(0, /->self.update())
    tasmota.resp_cmnd_done()
  end

  def start_failed() tasmota.resp_cmnd_failed() return nil end

  def power(on) tasmota.set_power(self.channel, on) end

  # fetch the prices for the next 0 to 48 hours from now
  def update()
    var rtc=tasmota.rtc(),params=[],post,url=self.plugin.url(rtc, params)
    if !url return end
    self.tz = rtc['timezone'] * 60
    try
      post = params[0]
      self.p_kWh = params[1]
    except .. end
    var wc = webclient()
    var prices = [], times = []
    while true
      wc.begin(url)
      var rc = post == nil ? wc.GET() : wc.POST(post)
      var data = rc == 200 ? wc.get_string() : nil
      wc.close()
      if data != nil
        url = self.plugin.parse(data, prices, times)
        if url continue end
      end
      if size(prices)
        try self.past = times[0] - times[1] except .. end
        self.timeout = nil
        self.prices = prices
        self.times = times
        self.prune_old(rtc['utc'])
        self.schedule_chosen(self.find_cheapest(), rtc['utc'], self.past)
        return
      end
      if data == nil print(f'error {rc} for {url} {post=}') end
      break
    end
    # We failed to update the prices. Retry in 1, 2, 4, 8, …, 64 minutes.
    if !self.timeout
      self.timeout = 60000
    elif self.timeout < 3840000
      self.timeout = self.timeout * 2
    end
    tasmota.set_timer(self.timeout, /->self.update())
  end

  def prune_old(now, ch)
    var N = size(self.prices)
    while N
      if self.times[0] - now > self.past break end
      self.prices.pop(0)
      self.times.pop(0)
    end
    var M = size(self.prices)
    if M && ch
      N -= M
      var chS = size(ch)
      while chS && ch[0] < N ch.pop(0) chS -= 1 end
      if chS
        while chS chS -= 1 ch[chS] -= N end
      else
        ch = nil
      end
    else
      ch = nil
    end
    return M
  end

  def find_cheapest(first)
    var cheapest, N = size(self.prices)
    if N
      var heap = [], i = first == nil ? 0 : first
      while i<N heap.push(i) i += 1 end
      var cmp = / a b
      -> a == first || (b != first && self.prices[a] < self.prices[b])
      make_heap(heap, cmp)
      var slots = size(heap)
      if slots > self.slots slots = self.slots end
      cheapest = []
      while slots slots -= 1 cheapest.push(remove_heap(heap, cmp)) end
      binary_heap.sort(cheapest, / a b -> a < b)
    end
    return cheapest
  end

  # trigger the timer at the chosen hour
  def schedule_chosen(chosen, now, old)
    tasmota.remove_timer('power_on')
    var d = chosen && size(chosen) ? self.times[chosen[0]] - now : self.past
    if d != old self.power(d > self.past && d <= 0) end
    if d > 0
      tasmota.set_timer(d * 1000, def() self.power(true) end, 'power_on')
    elif d <= self.past
      if chosen==nil || size(chosen) < 2 chosen = nil else chosen.pop(0) end
    end
    self.chosen = chosen
  end

  def web_add_main_button() webserver.content_send(self.UI) end

  def web_sensor()
    var ch, old = self.past, now = tasmota.rtc()['utc']
    var N = size(self.prices)
    if N
      ch = []
      try
        for i: self.chosen ch.push(i) end
        old = self.times[ch[0]] - now
      except .. end
      N = self.prune_old(now, ch)
    end
    var op = webserver.has_arg('op') ? int(webserver.arg('op')) : nil
    if op == self.UPDATE
      self.update()
      ch = self.chosen
    end
    while N
      var first
      if op == self.MORE
        if self.slots < N self.slots += 1 end
      elif op == self.LESS
        if self.slots > 1 self.slots -= 1 end
      elif op == self.PAUSE
        if self.chosen != nil ch = nil break end
      elif op == self.PREV
        try first = (!ch[0] ? N : ch[0]) - 1 except .. end
      elif op == self.NEXT
        try first = ch[0] + 1 < N ? ch[0] + 1 : 0 except .. end
      else break end
      ch = self.find_cheapest(first)
      break
    end
    self.schedule_chosen(ch, now, old)
    var min = self.prices[0], max = min
    for p : self.prices if p < min min = p elif p > max max = p end end
    var status
    if size(ch)
      var delay = self.times[ch[0]] - now
      if delay <= 0 delay = ''
      else
        delay /= 60
        delay = format(" %d:%02d", delay / 60, delay % 60)
      end
      status = format("{s}(%d≤%d)%.3g‥%.3g %s<small> (%.3g)</small>{m}⭙%s{e}",
                      size(ch), self.slots, min, max,
                      self.p_kWh, self.prices[ch[0]], delay)
    else
      status = format("{s}(0≤%d)%.3g‥%.3g %s{m}⭘{e}",
                      self.slots, min, max, self.p_kWh)
    end
    status+="<tr><td colspan='2'>"
      "<svg width='100%' height='4ex' viewBox='-1 -1 1052 102'>"
    if N
      var w = 1050.0 / N
      var scale = 100.0 / (max - min)
      try
        for choice: ch
          status+=format("<rect x='%g' width='%g' height='100'"
                         " fill='red'></rect>", w * choice, w)
        end
      except .. end
      status+="<path d='"
      var fmt="M0 %gh%g"
      for p: self.prices
        status+=format(fmt, 100 - scale * (p - min), w)
        fmt="V%gh%g"
      end
      status+="' fill='transparent' stroke='white' stroke-width='2'></path>"
    end
    status+="</svg>{e}"
    tasmota.web_send_decimal(status)
  end
end
return CheapPowerBase
end
return cheap_power_base
