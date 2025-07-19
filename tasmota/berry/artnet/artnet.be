# Art-Net driver

class ArtNet
  var matrix                    # the led matrix
  var port                      # port number for listening for incoming packets
  var udp_server                # instance of `udp` class
  var universe_start            # base universe number
  var universe_end              # last universe number allowed (excluded)
  # static var artnet_sig = bytes().fromstring("Art-Net\x00")   # 8 bytes               # signature of packet
  static var artnet_sig_0 = 0x4172742D    # "Art-"
  static var artnet_sig_4 = 0x4E657400    # "Net\x00"

  var packet                    # try reusing the same packer bytes() object for performance

  # local copy of led matrix attributes for faster access
  var alternate                 # field from matrix, alternate lines (reversed). Contains 0 if not alternate, or the number of bytes per pixel

  def init(matrix, universe_start, port, ip_addr)
    self.matrix = matrix
    self.alternate = matrix.alternate ? matrix.pixel_size() : 0
    # self.v = self.matrix.v
    if universe_start == nil   universe_start = 0 end
    self.universe_start = universe_start
    self.universe_end = universe_start + matrix.h

    if port == nil  port = 6454 end
    self.port = int(port)
    if ip_addr == nil  ip_addr = "" end

    self.packet = bytes()     # instanciate a single bytes() buffer that will be used for all received packets

    self.udp_server = udp()
    self.udp_server.begin(ip_addr, self.port)

    # register as fast_loop
    tasmota.add_fast_loop(/-> self.fast_loop())
    # set sleep to 5 for a smooth animation
    tasmota.global.sleep = 5
  end

  def stop()
    import introspect
    # if usd_server has a stop() method, call it
    if introspect.get(self.udp_server, "stop")
      self.udp_server.stop()
    end
    self.matrix.clear()
  end

  def fast_loop()
    var universe_start = self.universe_start
    var universe_end = self.universe_end
    var artnet_sig_0 = self.artnet_sig_0
    var artnet_sig_4 = self.artnet_sig_4
    var dirty = false
    var packet = self.udp_server.read(self.packet)
    while (packet != nil)
      if size(packet) >= 18 &&
          packet.get(0, -4) == artnet_sig_0 && packet.get(4, -4) == artnet_sig_4
        var opcode = packet.get(8, 2)       # should be 0x5000
        var protocol = packet.get(10, -2)   # big endian, should be 14
        var universe = packet.get(14, 2)

        if opcode == 0x5000 && protocol == 14 && universe >= universe_start && universe < universe_end
          # tasmota.log("DMX: received Art-Net packet :" + packet.tohex())
          # var seq = packet.get(12, 1)
          # var phy = packet.get(13, 1)
          var data_len = packet.get(16, -2)
          # data starts at offset 18
          if size(packet) >= 18 + data_len    # check size
            if self.alternate > 0 && (universe - self.universe_start) % 2
              packet.reverse(18, self.alternate, -1)
            end
            self.matrix.set_bytes(universe, packet, 18, data_len)
            dirty = true
          end
          # tasmota.log(format("DMX: opcode=0x%04X protocol=%i seq=%i phy=%i universe=%i data_len=%i data=%s",
          #             opcode, protocol, seq, phy, universe, data_len, packet[18..-1].tohex()))
        end
      end
      packet = self.udp_server.read(self.packet)
      if packet == nil
        tasmota.delay_microseconds(20)   # wait 20 us just in case
        packet = self.udp_server.read(self.packet)
      end
    end

    if dirty
      self.matrix.dirty()
      self.matrix.show()
    end
  end

  static def read_persist()
    import persist
    var conf = dyn()

    conf.gpio = persist.find("artnet_gpio", 0)      # gpio number from template
    conf.rows = persist.find("artnet_rows", 5)      # number of rows (min: 1)
    conf.cols = persist.find("artnet_cols", 5)      # number of columns (min: 1)
    conf.offs = persist.find("artnet_offs", 0)      # offset in the led strip where the matrix starts (min: 0)
    conf.alt  = persist.find("artnet_alt", false)   # are the rows in alternate directions

    conf.univ = persist.find("artnet_univ", 0)      # start universe

    # conf.addr = persist.find("artnet_addr", "uni")  # listening mode, either 'uni' or 'multi' for multicast
    conf.port = persist.find("artnet_port", 6454)   # UDP port number

    conf.auto = persist.find("artnet_auto", true)  # autorun at startup
    return conf
  end

  static def run_from_conf()
    import persist

    var conf = ArtNet.read_persist()
    var r = conf.rows
    var c = conf.cols

    var strip = Leds(r * c, gpio.pin(gpio.WS2812, conf.gpio))
    var matrix = strip.create_matrix(r, c, conf.offs)
    if conf.alt  matrix.set_alternate(true)
    end
    var dmx = ArtNet(matrix, conf.univ, conf.port)

    global._artnet = dmx
  end

  static def stop_global()
    var dmx = global._artnet
    if type(dmx) == 'instance'
      dmx.stop()
      global._artnet = nil  # dereference
      tasmota.gc()          # force gc
    end
  end
end

return ArtNet

#-
# Example for M5Stack ATOM Matrix (5x5 matrix without alternate)
import artnet
# var artnet = ArtNet
var strip = Leds(25, gpio.pin(gpio.WS2812, 0))
var m = strip.create_matrix(5, 5, 0)
var dmx = artnet(m)
-#