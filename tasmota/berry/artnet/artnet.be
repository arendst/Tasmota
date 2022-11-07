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
          # import string
          # tasmota.log(string.format("DMX: opcode=0x%04X protocol=%i seq=%i phy=%i universe=%i data_len=%i data=%s",
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