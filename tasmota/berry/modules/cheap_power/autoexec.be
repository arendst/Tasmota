var wd = tasmota.wd
tasmota.add_cmd("CheapPower",
  def (cmd, idx)
    import sys
    var path = sys.path()
    path.push(wd)
    import cheap_power
    path.pop()
    cheap_power.start(idx)
  end
)
