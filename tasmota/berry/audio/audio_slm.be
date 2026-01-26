# This is sample code for audio SLM (Sound Level Meter)
#
# It uses typical cheap mems microphone in PDM mode,
# and uses a DC block filter (high pass), a fixed gain
# and a low pass filter (currently cutting at 3KHz)
#
# add `import audio_slm` in `autoexec.be``
#
class Audio_SLM
  var audio_input, fast_loop_closure
  var buffer
  var rms
  var peak
  var noise_gate                                            # ignore below this level
  def init()
  #   tasmota.cmd("SaveData 0")
    self.buffer = bytes(1024)                               # resuse same buffer at each iteration
    self.audio_input = AudioInputI2S()
    self.audio_input.set_gain(30)
    self.rms = 0
    self.peak = 0
    self.noise_gate = 150

    self.audio_input.begin()
    tasmota.delay(300)                                      # wait for 300 ms for mems microphone to stabilize
    self.fast_loop_closure = def () self.fast_loop() end
    #tasmota.add_fast_loop(self.fast_loop_closure)
    tasmota.add_driver(self)
    tasmota.set_timer(100, /-> tasmota.add_fast_loop(self.fast_loop_closure))   # delay start by 1 second
  end

  def stop()
    self.audio_input.stop()
    tasmota.remove_fast_loop(self.fast_loop_closure)
    tasmota.remove_driver(self)
  end

  def fast_loop()
    var b = self.audio_input.read_bytes(self.buffer)
    if b != nil
      import light
      var rms = AudioInputI2S.rms_bytes(b)
      if rms < self.noise_gate
        rms = 0
      end
      if rms > self.rms
        self.rms = rms
      end

      var new_peak = self.audio_input.peak
      if new_peak == nil    new_peak = 0    end     # failsafe
      if new_peak > self.peak   self.peak = new_peak end
      # light.set_bri(rms / 80 #-range 0..255-#, true #-no save-#)
    end
  end

  #- display sensor value in the web UI -#
  def web_sensor()
    var msg = format(
            "{s}SLM Gain %ix{m}RMS: %i (peak %i%%){e}",
            self.audio_input.get_gain(),
            self.rms,
            self.peak / 32767)
    tasmota.web_send_decimal(msg)
    self.peak = 0
    self.rms = 0
  end
end

var audio_slm = Audio_SLM()
return audio_slm
