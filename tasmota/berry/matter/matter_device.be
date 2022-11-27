class matter_device
  var udp_socket
  var packet

  def init()
    if tasmota.eth().find('ip') != nil || tasmota.wifi().find('ip') != nil
      self.start_mdns()
    else
      tasmota.add_rule("Wifi#Connected", def () self.start_mdns() end)
    end
    tasmota.add_driver(self)
  end

  def stop()
    tasmota.remove_driver(self)
  end

  def every_50ms()
    if self.udp_socket == nil  return end
    var packet = self.udp_socket.read(self.packet)
    while packet != nil
      self.packet = packet
      print("Packet=",packet.to_hex())
    end
  end

  def start_mdns()
    print("MTR: starting mDSN")

    self.udp_socket = udp()
    self.udp_socket.begin("", 5540)

    import mdns
    mdns.start()
    mdns.add_service("_matterc","_udp", 5540, {"VP":"65521+32768", "SII":5000, "SAI":300, "T":1, "D":3840, "CM":1, "PH":33, "PI":""})
  end
end

return matter_device
