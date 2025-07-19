#-
 - WAV recorder driver  Berry - mono only
 - used from command `wavrec<duration in seconds> <file_name_without_wav_suffix>`
-#

class WAVREC
  var in, f
  var fast
  var targetSize

  def init(seconds, name)
    self.in=AudioInputI2S()
    self.in.begin()
    self.in.set_gain(60)
    var r = self.in.get_rate()
    var bps = self.in.get_bits_per_sample()
    log(f"WAV: sample rate {r} with {bps} bits")
    self.f = open(f"{name}.wav", "w")
    self.targetSize = r * seconds * (bps/8)
    self.writeHeader(self.targetSize,r, bps)
    log(f"WAV: will record {seconds} seconds to {name}.wav")
    self.fast = /->self.record()
    tasmota.add_fast_loop(self.fast)
    tasmota.add_driver(self)
  end

  def record()
    var b = self.in.read_bytes()
    if b == nil
      return
    end
    self.targetSize -= size(b)
    if self.targetSize > 0
      self.f.write(b)
    else
      tasmota.remove_fast_loop(self.fast)
      self.f.close()
      log("WAV: closing file")
      tasmota.remove_driver(self)
    end
  end

  def writeHeader(fsize, rate, bps)
    var header = bytes(44)
    header.."RIFF"
    header.add(fsize + 44,4)
    header.."WAVE"
    header.."fmt "
    header.add(bps,4) #sample_bits
    header..1 # PCM
    header..0
    header..1 # mono
    header..0
    header.add(rate,4) #sample_bits
    header.add(rate * 2,4) #(Sample Rate * BitsPerSample * Channels) / 8
    header..2 # BlockAlign
    header..0
    header..bps # sample bits
    header..0
    header.."data"
    header.add(fsize,4) #sample_bits
    self.f.write(header)
  end

end

def wav_recorder(cmd, idx, payload, payload_json)
  tasmota.resp_cmnd_done()
  return WAVREC(idx, payload)
end

tasmota.add_cmd('wavrec', wav_recorder)
# use like: waverec5 /test - > record 5 seconds to /test.wav
