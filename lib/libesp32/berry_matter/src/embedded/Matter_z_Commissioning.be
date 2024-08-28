#
# Matter_z_Commissioning.be - implements the commissioning and MDNS logic
#
# Copyright (C) 2023  Stephan Hadinger & Theo Arends
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

import matter

#@ solidify:Matter_Commissioning,weak

class Matter_Commissioning
  var device                          # reference to the main Device instance

  static var PBKDF_ITERATIONS = 1000  # I don't see any reason to choose a different number
  static var PASE_TIMEOUT = 10*60     # default open commissioning window (10 minutes)
  # Commissioning open
  var commissioning_open              # timestamp for timeout of commissioning (millis()) or `nil` if closed
  var commissioning_iterations        # current PBKDF number of iterations
  var commissioning_discriminator     # commissioning_discriminator
  var commissioning_salt              # current salt
  var commissioning_w0                # current w0 (SPAKE2+)
  var commissioning_L                 # current L (SPAKE2+)
  var commissioning_admin_fabric      # the fabric that opened the currint commissioning window, or `nil` for default
  # information about the device
  var commissioning_instance_wifi     # random instance name for commissioning (mDNS)
  var commissioning_instance_eth      # random instance name for commissioning (mDNS)
  var hostname_wifi                   # MAC-derived hostname for commissioning
  var hostname_eth                    # MAC-derived hostname for commissioning
  # mDNS active announces
  var mdns_pase_eth                   # do we have an active PASE mDNS announce for eth
  var mdns_pase_wifi                  # do we have an active PASE mDNS announce for wifi


  #############################################################
  def init(device)
    self.device = device
  end

  #############################################################
  # Start Basic Commissioning Window if needed at startup
  def init_basic_commissioning()
    # if no fabric is configured, automatically open commissioning at restart
    if self.device.sessions.count_active_fabrics() == 0
      self.start_root_basic_commissioning()
    end
  end

  #############################################################
  # dispatch every second click to sub-objects that need it
  def every_second()
    if self.commissioning_open != nil && tasmota.time_reached(self.commissioning_open)    # timeout reached, close provisioning
      self.commissioning_open = nil
    end
  end

  #############################################################
  # Start Basic Commissioning with root parameters
  #
  # Open window for `timeout_s` (default 10 minutes)
  def start_root_basic_commissioning(timeout_s)
    if timeout_s == nil   timeout_s = self.PASE_TIMEOUT end

    # show Manual pairing code in logs
    var pairing_code = self.compute_manual_pairing_code()
    log(format("MTR: Manual pairing code: %s", pairing_code), 2)
    
    # output MQTT
    var qr_code = self.compute_qrcode_content()
    tasmota.publish_result(format('{"Matter":{"Commissioning":1,"PairingCode":"%s","QRCode":"%s"}}', pairing_code, qr_code), 'Matter')

    # compute PBKDF
    import crypto
    var root_salt = crypto.random(16)

    # Compute the PBKDF parameters for SPAKE2+ from root parameters
    var passcode = bytes().add(self.device.root_passcode, 4)

    var tv = crypto.PBKDF2_HMAC_SHA256().derive(passcode, root_salt, self.PBKDF_ITERATIONS, 80)
    var w0s = tv[0..39]
    var w1s = tv[40..79]

    var root_w0 = crypto.EC_P256().mod(w0s)
    var w1 = crypto.EC_P256().mod(w1s)    # w1 is temporarily computed then discarded
    # self.root_w1 = crypto.EC_P256().mod(w1s)
    var root_L = crypto.EC_P256().public_key(w1)
    self.start_basic_commissioning(timeout_s, self.PBKDF_ITERATIONS, self.device.root_discriminator, root_salt, root_w0, #-self.root_w1,-# root_L, nil)
  end

  #############################################################
  # Start Basic Commissioning Window with custom parameters
  def start_basic_commissioning(timeout_s, iterations, discriminator, salt, w0, L, admin_fabric)
    self.commissioning_open = tasmota.millis() + timeout_s * 1000
    self.commissioning_iterations = iterations
    self.commissioning_discriminator = discriminator
    self.commissioning_salt = salt
    self.commissioning_w0 = w0
    self.commissioning_L  = L
    self.commissioning_admin_fabric = admin_fabric

    if tasmota.wifi()['up'] || tasmota.eth()['up']
      self.mdns_announce_PASE()
    else
      tasmota.add_rule("Wifi#Connected", def ()
          self.mdns_announce_PASE()
          tasmota.remove_rule("Wifi#Connected", "mdns_announce_PASE")
        end, "mdns_announce_PASE")
        tasmota.add_rule("Eth#Connected", def ()
            self.mdns_announce_PASE()
            tasmota.remove_rule("Eth#Connected", "mdns_announce_PASE")
          end, "mdns_announce_PASE")
    end
  end

  #############################################################
  # Is root commissioning currently open. Mostly for UI to know if QRCode needs to be shown.
  def is_root_commissioning_open()
    return self.commissioning_open != nil && self.commissioning_admin_fabric == nil
  end
  
  #############################################################
  # Stop PASE commissioning, mostly called when CASE is about to start
  def stop_basic_commissioning()
    var n = nil
    if self.is_root_commissioning_open()
      tasmota.publish_result('{"Matter":{"Commissioning":0}}', 'Matter')
    end
    self.commissioning_open = n

    self.mdns_remove_PASE()

    # clear any PBKDF information to free memory
    self.commissioning_iterations = n
    self.commissioning_discriminator = n
    self.commissioning_salt = n
    self.commissioning_w0 = n
    # self.commissioning_w1 = nil
    self.commissioning_L = n
    self.commissioning_admin_fabric = n
  end
  def is_commissioning_open()
    return self.commissioning_open != nil
  end
  
  #############################################################
  # Compute QR Code content - can be done only for root PASE
  def compute_qrcode_content()
    var raw = bytes().resize(11)    # we don't use TLV Data so it's only 88 bits or 11 bytes
    # version is `000` dont touch
    raw.setbits(3, 16, self.device.VENDOR_ID)
    raw.setbits(19, 16, self.device.PRODUCT_ID)
    # custom flow = 0 (offset=35, len=2)
    raw.setbits(37, 8, 0x04)        # already on IP network
    raw.setbits(45, 12, self.device.root_discriminator & 0xFFF)
    raw.setbits(57, 27, self.device.root_passcode & 0x7FFFFFF)
    # padding (offset=84 len=4)
    return "MT:" + matter.Base38.encode(raw)
  end


  #############################################################
  # Compute the 11 digits manual pairing code (wihout vendorid nor productid) p.223
  # <BR>
  # can be done only for root PASE (we need the passcode, but we don't get it with OpenCommissioningWindow command)
  def compute_manual_pairing_code()
    var digit_1 = (self.device.root_discriminator & 0x0FFF) >> 10
    var digit_2_6 = ((self.device.root_discriminator & 0x0300) << 6) | (self.device.root_passcode & 0x3FFF)
    var digit_7_10 = (self.device.root_passcode >> 14)

    var ret = format("%1i%05i%04i", digit_1, digit_2_6, digit_7_10)
    ret += matter.Verhoeff.checksum(ret)
    return ret
  end

  #############################################################
  # Start Operational Discovery for this session
  #
  # Deferred until next tick.
  def start_operational_discovery_deferred(fabric)
    # defer to next click
    tasmota.set_timer(0, /-> self.start_operational_discovery(fabric))
  end

  #############################################################
  # Start Commissioning Complete for this session
  #
  # Deferred until next tick.
  def start_commissioning_complete_deferred(session)
    # defer to next click
    tasmota.set_timer(0, /-> self.start_commissioning_complete(session))
  end

  #############################################################
  # Start Operational Discovery for this session
  #
  # Stop Basic Commissioning and clean PASE specific values (to save memory).
  # Announce fabric entry in mDNS.
  def start_operational_discovery(fabric)
    import crypto
    import mdns

    self.stop_basic_commissioning()    # close all PASE commissioning information

    self.mdns_announce_op_discovery(fabric)
  end

  #############################################################
  # Commissioning Complete
  #
  # Stop basic commissioning.
  def start_commissioning_complete(session)
    var fabric = session.get_fabric()
    var fabric_id = fabric.get_fabric_id().copy().reverse().tohex()
    var vendor_name = fabric.get_admin_vendor_name()
    log(f"MTR: --- Commissioning complete for Fabric '{fabric_id}' (Vendor {vendor_name}) ---", 2)
    self.stop_basic_commissioning()     # by default close commissioning when it's complete
  end

  #############################################################
  # mDNS Configuration
  #############################################################
  # Start mDNS and announce hostnames for Wifi and ETH from MAC
  #
  # When the announce is active, `hostname_wifi` and `hostname_eth`
  # are defined
  def start_mdns_announce_hostnames()
    if tasmota.wifi()['up']
      self._mdns_announce_hostname(false)
    else
      tasmota.add_rule("Wifi#Connected", def ()
          self._mdns_announce_hostname(false)
          tasmota.remove_rule("Wifi#Connected", "matter_mdns_host")
        end, "matter_mdns_host")
    end

    if tasmota.eth()['up']
      self._mdns_announce_hostname(true)
    else
      tasmota.add_rule("Eth#Connected", def ()
          self._mdns_announce_hostname(true)
          tasmota.remove_rule("Eth#Connected", "matter_mdns_host")
        end, "matter_mdns_host")
    end
  end

  #############################################################
  # Start UDP mDNS announcements hostname
  # This announcement is independant from commissioning windows
  #
  # eth is `true` if ethernet turned up, `false` is wifi turned up
  def _mdns_announce_hostname(is_eth)
    import mdns
    import string

    mdns.start()

    try
      if is_eth
        # Add Hostname (based on MAC) with IPv4/IPv6 addresses
        var eth = tasmota.eth()
        self.hostname_eth  = string.replace(eth.find("mac"), ':', '')
        if !self.device.ipv4only || !eth.contains('ip6local')
          # log(format("MTR: calling mdns.add_hostname(%s, %s, %s)", self.hostname_eth, eth.find('ip6local',''), eth.find('ip','')), 4)
          mdns.add_hostname(self.hostname_eth, eth.find('ip6local',''), eth.find('ip',''), eth.find('ip6',''))
        else
          log(format("MTR: calling mdns.add_hostname(%s, %s)", self.hostname_eth, eth.find('ip','')), 3)
          mdns.add_hostname(self.hostname_eth, eth.find('ip',''))
        end
      else
        var wifi = tasmota.wifi()
        self.hostname_wifi = string.replace(wifi.find("mac"), ':', '')
        if !self.device.ipv4only || !wifi.contains('ip6local')
          # log(format("MTR: calling mdns.add_hostname(%s, %s, %s)", self.hostname_wifi, wifi.find('ip6local',''), wifi.find('ip','')), 4)
          mdns.add_hostname(self.hostname_wifi, wifi.find('ip6local',''), wifi.find('ip',''), wifi.find('ip6',''))
        else
          log(format("MTR: calling mdns.add_hostname(%s, %s)", self.hostname_wifi, wifi.find('ip','')), 3)
          mdns.add_hostname(self.hostname_wifi, wifi.find('ip',''))
        end
      end
      log(format("MTR: start mDNS on %s host '%s.local'", is_eth ? "eth" : "wifi", is_eth ? self.hostname_eth : self.hostname_wifi), 3)
    except .. as e, m
      log("MTR: Exception" + str(e) + "|" + str(m), 2)
    end

    self.mdns_announce_op_discovery_all_fabrics()
  end

  #############################################################
  # Announce MDNS for PASE commissioning
  def mdns_announce_PASE()
    import mdns
    import crypto

    var services = {
      "VP": f"{self.device.VENDOR_ID}+{self.device.PRODUCT_ID}",
      "D": self.commissioning_discriminator,
      "CM":1,                           # requires passcode
      "T":0,                            # no support for TCP
      "SII":5000, "SAI":300
    }

    self.commissioning_instance_wifi = crypto.random(8).tohex()    # 16 characters random hostname
    self.commissioning_instance_eth = crypto.random(8).tohex()    # 16 characters random hostname

    try
      if self.hostname_eth
        # Add Matter `_matterc._udp` service
        # log(format("MTR: calling mdns.add_service(%s, %s, %i, %s, %s, %s)", "_matterc", "_udp", 5540, str(services), self.commissioning_instance_eth, self.hostname_eth), 4)
        mdns.add_service("_matterc", "_udp", 5540, services, self.commissioning_instance_eth, self.hostname_eth)
        self.mdns_pase_eth = true

        log(format("MTR: announce mDNS on %s '%s' ptr to `%s.local`", "eth", self.commissioning_instance_eth, self.hostname_eth), 2)

        # `mdns.add_subtype(service:string, proto:string, instance:string, hostname:string, subtype:string) -> nil`
        var subtype = "_L" + str(self.commissioning_discriminator & 0xFFF)
        log("MTR: adding subtype: "+subtype, 3)
        mdns.add_subtype("_matterc", "_udp", self.commissioning_instance_eth, self.hostname_eth, subtype)
        subtype = "_S" + str((self.commissioning_discriminator & 0xF00) >> 8)
        log("MTR: adding subtype: "+subtype, 3)
        mdns.add_subtype("_matterc", "_udp", self.commissioning_instance_eth, self.hostname_eth, subtype)
        subtype = "_V" + str(self.device.VENDOR_ID)
        log("MTR: adding subtype: "+subtype, 3)
        mdns.add_subtype("_matterc", "_udp", self.commissioning_instance_eth, self.hostname_eth, subtype)
        subtype = "_CM1"
        log("MTR: adding subtype: "+subtype, 3)
        mdns.add_subtype("_matterc", "_udp", self.commissioning_instance_eth, self.hostname_eth, subtype)
      end
      if self.hostname_wifi

        # log(format("MTR: calling mdns.add_service(%s, %s, %i, %s, %s, %s)", "_matterc", "_udp", 5540, str(services), self.commissioning_instance_wifi, self.hostname_wifi), 4)
        mdns.add_service("_matterc", "_udp", 5540, services, self.commissioning_instance_wifi, self.hostname_wifi)
        self.mdns_pase_wifi = true

        log(format("MTR: starting mDNS on %s '%s' ptr to `%s.local`", "wifi", self.commissioning_instance_wifi, self.hostname_wifi), 3)

        # `mdns.add_subtype(service:string, proto:string, instance:string, hostname:string, subtype:string) -> nil`
        var subtype = "_L" + str(self.commissioning_discriminator & 0xFFF)
        log("MTR: adding subtype: "+subtype, 3)
        mdns.add_subtype("_matterc", "_udp", self.commissioning_instance_wifi, self.hostname_wifi, subtype)
        subtype = "_S" + str((self.commissioning_discriminator & 0xF00) >> 8)
        log("MTR: adding subtype: "+subtype, 3)
        mdns.add_subtype("_matterc", "_udp", self.commissioning_instance_wifi, self.hostname_wifi, subtype)
        subtype = "_V" + str(self.device.VENDOR_ID)
        log("MTR: adding subtype: "+subtype, 3)
        mdns.add_subtype("_matterc", "_udp", self.commissioning_instance_wifi, self.hostname_wifi, subtype)
        subtype = "_CM1"
        log("MTR: adding subtype: "+subtype, 3)
        mdns.add_subtype("_matterc", "_udp", self.commissioning_instance_wifi, self.hostname_wifi, subtype)
      end
    except .. as e, m
      log("MTR: Exception" + str(e) + "|" + str(m), 2)
    end

  end

  #############################################################
  # MDNS remove any PASE announce
  def mdns_remove_PASE()
    import mdns

    try
      if self.mdns_pase_eth
        log(format("MTR: calling mdns.remove_service(%s, %s, %s, %s)", "_matterc", "_udp", self.commissioning_instance_eth, self.hostname_eth), 3)
        log(format("MTR: remove mDNS on %s '%s'", "eth", self.commissioning_instance_eth), 3)
        self.mdns_pase_eth = false
        mdns.remove_service("_matterc", "_udp", self.commissioning_instance_eth, self.hostname_eth)
      end
      if self.mdns_pase_wifi
        log(format("MTR: calling mdns.remove_service(%s, %s, %s, %s)", "_matterc", "_udp", self.commissioning_instance_wifi, self.hostname_wifi), 3)
        log(format("MTR: remove mDNS on %s '%s'", "wifi", self.commissioning_instance_wifi), 3)
        self.mdns_pase_wifi = false
        mdns.remove_service("_matterc", "_udp", self.commissioning_instance_wifi, self.hostname_wifi)
      end
    except .. as e, m
      log("MTR: Exception" + str(e) + "|" + str(m), 2)
    end
  end

  #############################################################
  # Start UDP mDNS announcements for commissioning for all persisted sessions
  def mdns_announce_op_discovery_all_fabrics()
    for fabric: self.device.sessions.active_fabrics()
      if fabric.get_device_id() && fabric.get_fabric_id()
        self.mdns_announce_op_discovery(fabric)
      end
    end
  end

  #############################################################
  # Start UDP mDNS announcements for commissioning
  def mdns_announce_op_discovery(fabric)
    import mdns
    try
      var device_id = fabric.get_device_id().copy().reverse()
      var k_fabric = fabric.get_fabric_compressed()
      var op_node = k_fabric.tohex() + "-" + device_id.tohex()
      log("MTR: Operational Discovery node = " + op_node, 3)

      # mdns
      if (tasmota.eth().find("up"))
        log(format("MTR: adding mDNS on %s '%s' ptr to `%s.local`", "eth", op_node, self.hostname_eth), 3)
        mdns.add_service("_matter","_tcp", 5540, nil, op_node, self.hostname_eth)
        var subtype = "_I" + k_fabric.tohex()
        log("MTR: adding subtype: "+subtype, 3)
        mdns.add_subtype("_matter", "_tcp", op_node, self.hostname_eth, subtype)
      end
      if (tasmota.wifi().find("up"))
        log(format("MTR: adding mDNS on %s '%s' ptr to `%s.local`", "wifi", op_node, self.hostname_wifi), 3)
        mdns.add_service("_matter","_tcp", 5540, nil, op_node, self.hostname_wifi)
        var subtype = "_I" + k_fabric.tohex()
        log("MTR: adding subtype: "+subtype, 3)
        mdns.add_subtype("_matter", "_tcp", op_node, self.hostname_wifi, subtype)
      end
    except .. as e, m
      log("MTR: Exception" + str(e) + "|" + str(m), 2)
    end
  end

  #############################################################
  # Remove all mDNS announces for all fabrics
  def mdns_remove_op_discovery_all_fabrics()
    for fabric: self.device.sessions.active_fabrics()
      if fabric.get_device_id() && fabric.get_fabric_id()
        self.mdns_remove_op_discovery(fabric)
      end
    end
  end

  #############################################################
  # Remove mDNS announce for fabric
  def mdns_remove_op_discovery(fabric)
    import mdns
    try
      var device_id = fabric.get_device_id().copy().reverse()
      var k_fabric = fabric.get_fabric_compressed()
      var op_node = k_fabric.tohex() + "-" + device_id.tohex()

      # mdns
      if (tasmota.eth().find("up"))
        log(format("MTR: remove mDNS on %s '%s'", "eth", op_node), 3)
        mdns.remove_service("_matter", "_tcp", op_node, self.hostname_eth)
      end
      if (tasmota.wifi().find("up"))
        log(format("MTR: remove mDNS on %s '%s'", "wifi", op_node), 3)
        mdns.remove_service("_matter", "_tcp", op_node, self.hostname_wifi)
      end
    except .. as e, m
      log("MTR: Exception" + str(e) + "|" + str(m), 2)
    end
  end

  #####################################################################
  # Generate random passcode
  #####################################################################
  static var PASSCODE_INVALID = [ 0, 11111111, 22222222, 33333333, 44444444, 55555555, 66666666, 77777777, 88888888, 99999999, 12345678, 87654321]
  def generate_random_passcode()
    import crypto
    var passcode
    while true
      passcode = crypto.random(4).get(0, 4) & 0x7FFFFFF
      if passcode > 0x5F5E0FE     continue      end         # larger than allowed
      for inv: self.PASSCODE_INVALID
        if passcode == inv    passcode = nil    end
      end
      if passcode != nil    return passcode     end
    end
  end

end
matter.Commissioning = Matter_Commissioning
