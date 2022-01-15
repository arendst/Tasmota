# register the command 'DisplayCalibrate'
#
# load the module in memory only when the command is first used
#
var wd = tasmota.wd                 # capture value of the first run
tasmota.add_cmd("DisplayCalibrate",
  def ()
    import sys
    var path = sys.path()

    path.push(wd)
    import ts_calibrate
    path.pop()

    tasmota.set_timer(0, /-> ts_calibrate.start())
    tasmota.resp_cmnd_done()
  end
)
