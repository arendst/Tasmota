#
# Matter_Plugin_core.be - implements the core features that a Matter device must implemment
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

# Matter plug-in for core behavior

# dummy declaration for solidification
class Matter_Plugin end

#@ solidify:Matter_Plugin_core,weak

class Matter_Plugin_core : Matter_Plugin
  static var ENDPOINTS = [ 0 ]
  static var CLUSTERS  = {
    0x001D: [0,1,2,3],
    0x0028: [0,1,2,3,4,5,6,7,8,9],
    0x002B: [0,1],
    0x002C: [0,1,2],
    0x0030: [0,1,2,3,4],
    0x0031: [3,0xFFFC],
    0x0032: [],
    0x0033: [0,1,2,8],
    0x0034: [],
    0x0038: [0,1,7],
    0x003E: [0,1,2,3,4,5],
    0x003C: [],
    0x003F: []
  }

  #############################################################
  # Constructor
  def init(device)
    super(self).init(device)
    self.endpoints = self.ENDPOINTS
    self.clusters = self.CLUSTERS
  end

  #############################################################
  # read an attribute
  #
  def read_attribute(msg, ctx)
    import string
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    if   cluster == 0x0030              # ========== GeneralCommissioning cluster 11.9 p.627 ==========

      if   attribute == 0x0000          # ---------- Breadcrumb ----------
        return TLV.create_TLV(TLV.U8, msg.session.breadcrumb)
      elif attribute == 0x0001          # ---------- BasicCommissioningInfo / BasicCommissioningInfo----------
        var bci = TLV.Matter_TLV_struct()
        bci.add_TLV(0, TLV.U2, 60)      # FailSafeExpiryLengthSeconds
        bci.add_TLV(1, TLV.U2, 900)     # MaxCumulativeFailsafeSeconds
        return bci
      elif attribute == 0x0002          # ---------- RegulatoryConfig / RegulatoryLocationType ----------
        return TLV.create_TLV(TLV.U1, 2)    # 2 = IndoorOutdoor | esp-matter = 0
      elif attribute == 0x0003          # ---------- LocationCapability / RegulatoryLocationType----------
        return TLV.create_TLV(TLV.U1, 2)    # 2 = IndoorOutdoor
      elif attribute == 0x0004          # ---------- SupportsConcurrentConnection / bool ----------
        return TLV.create_TLV(TLV.BOOL, false)    # false - maybe can set to true
      end
    # ====================================================================================================
    elif cluster == 0x0032              # ========== Diagnostic Logs Cluster 11.10 p.637 ==========
      # no attributes

    # ====================================================================================================
    elif cluster == 0x0033              # ========== General Diagnostics Cluster 11.11 p.642 ==========

      if   attribute == 0x0000          #  ---------- NetworkInterfaces ----------
        var nwi = TLV.Matter_TLV_array() # list network interfaces, empty list for now, p.647

        var tas_eth = tasmota.eth()
        if (tas_eth['up'])
          var eth = nwi.add_struct(nil)
          eth.add_TLV(0, TLV.UTF1, 'ethernet')      # Name
          eth.add_TLV(1, TLV.BOOL, 1)               # IsOperational
          eth.add_TLV(2, TLV.BOOL, 1)               # OffPremiseServicesReachableIPv4
          eth.add_TLV(3, TLV.NULL, nil)             # OffPremiseServicesReachableIPv6
          var mac = bytes().fromhex(string.replace(tas_eth.find("mac", ""), ":", ""))
          eth.add_TLV(4, TLV.B1, mac) # HardwareAddress
          var ip4 = eth.add_array(5)                 # IPv4Addresses
          ip4.add_TLV(nil, TLV.B1, matter.get_ip_bytes(tas_eth.find("ip", "")))
          var ip6 = eth.add_array(6)                 # IPv6Addresses
          ip6.add_TLV(nil, TLV.B1, matter.get_ip_bytes(tas_eth.find("ip6local", "")))
          ip6.add_TLV(nil, TLV.B1, matter.get_ip_bytes(tas_eth.find("ip6", "")))
          eth.add_TLV(7, TLV.U1, 2)                # InterfaceType, p646
        end

        var tas_wif = tasmota.wifi()
        if (tas_wif['up'])
          var wif = nwi.add_struct(nil)
          wif.add_TLV(0, TLV.UTF1, 'wifi')          # Name
          wif.add_TLV(1, TLV.BOOL, 1)               # IsOperational
          wif.add_TLV(2, TLV.BOOL, 1)               # OffPremiseServicesReachableIPv4
          wif.add_TLV(3, TLV.NULL, nil)             # OffPremiseServicesReachableIPv6
          var mac = bytes().fromhex(string.replace(tas_wif.find("mac", ""), ":", ""))
          wif.add_TLV(4, TLV.B1, mac) # HardwareAddress
          var ip4 = wif.add_array(5)                 # IPv4Addresses
          ip4.add_TLV(nil, TLV.B1, matter.get_ip_bytes(tas_wif.find("ip", "")))
          var ip6 = wif.add_array(6)                 # IPv6Addresses
          ip6.add_TLV(nil, TLV.B1, matter.get_ip_bytes(tas_wif.find("ip6local", "")))
          ip6.add_TLV(nil, TLV.B1, matter.get_ip_bytes(tas_wif.find("ip6", "")))
          wif.add_TLV(7, TLV.U1, 1)                # InterfaceType, p646
        end
        return nwi
      elif attribute == 0x0001          #  ---------- RebootCount u16 ----------
        return TLV.create_TLV(TLV.U2, tasmota.cmd("Status 1")['StatusPRM']['BootCount'])
      elif attribute == 0x0002          #  ---------- UpTime u16 ----------
        return TLV.create_TLV(TLV.U4, tasmota.cmd("Status 11")['StatusSTS']['UptimeSec'])
      # TODO add later other attributes
      elif attribute == 0x0008          #  ---------- TestEventTriggersEnabled bool ----------
        return TLV.create_TLV(TLV.BOOL, false)    # false - maybe can set to true
      end
        
    # ====================================================================================================
    elif cluster == 0x0034              # ========== Software Diagnostics Cluster 11.12 p.654 ==========
      # no mandatory attributes - to be added later (maybe)

    # ====================================================================================================
    elif cluster == 0x0038              # ========== Time Synchronization 11.16 p.689 ==========
      if   attribute == 0x0000          #  ---------- UTCTime / epoch_us ----------
        var epoch_us = int64(tasmota.rtc()['utc']) * int64(1000000)
        return TLV.create_TLV(TLV.U8, epoch_us)     # TODO test the conversion of int64()
      elif attribute == 0x0001          #  ---------- Granularity / enum ----------
        return TLV.create_TLV(TLV.U1, 3)     # MillisecondsGranularity (NTP every hour, i.e. 36ms max drift)
      # TODO add some missing args
      elif attribute == 0x0007          #  ---------- LocalTime / epoch_us ----------
        var epoch_us = int64(tasmota.rtc()['local']) * int64(1000000)
        return TLV.create_TLV(TLV.U8, epoch_us)     # TODO test the conversion of int64()
      end

    # ====================================================================================================
    elif cluster == 0x003E              # ========== Node Operational Credentials Cluster 11.17 p.704 ==========

      if   attribute == 0x0000          #  ---------- NOCs / list[NOCStruct] ----------
        var nocl = TLV.Matter_TLV_array() # NOCs, p.711
        for session: self.device.sessions.sessions_active()
          var nocs = nocl.add_struct(nil)
          nocs.add_TLV(1, TLV.B2, session.noc)      # NOC
          nocs.add_TLV(2, TLV.B2, session.icac)     # ICAC
        end
        return nocl
      elif attribute == 0x0001          #  ---------- Fabrics / list[FabricDescriptorStruct] ----------
        var fabrics = TLV.Matter_TLV_array() # Fabrics, p.711
        for session: self.device.sessions.sessions_active()
          var root_ca_tlv = TLV.parse(session.get_ca())
          var fab = fabrics.add_struct(nil)            # encoding see p.303
          fab.add_TLV(1, TLV.B2, root_ca_tlv.findsubval(9)) # RootPublicKey
          fab.add_TLV(2, TLV.U2, session.admin_vendor)      # VendorID
          fab.add_TLV(3, TLV.U8, session.fabric)            # FabricID
          fab.add_TLV(4, TLV.U8, session.deviceid)          # NodeID
          fab.add_TLV(5, TLV.UTF1, session.fabric_label)    # Label
        end
        return fabrics
      elif attribute == 0x0002          #  ---------- SupportedFabrics / u1 ----------
        return TLV.create_TLV(TLV.U1, 5)     # Max 5 fabrics
      elif attribute == 0x0003          #  ---------- CommissionedFabrics / u1 ----------
        var sessions_active = self.device.sessions.sessions_active()
        return TLV.create_TLV(TLV.U1, size(sessions_active))  # number of active sessions
      elif attribute == 0x0004          #  ---------- TrustedRootCertificates / list[octstr] ----------
        # TODO
      elif attribute == 0x0005          #  ---------- Current­ FabricIndex / u1 ----------
        var sessions_active = self.device.sessions.sessions_active()
        var fabric_index = sessions_active.find(msg.session)
        if fabric_index == nil    fabric_index = 0 end
        return TLV.create_TLV(TLV.U1, fabric_index)  # number of active sessions
      end

    # ====================================================================================================
    elif cluster == 0x003C              # ========== Administrator Commissioning Cluster 11.18 p.725 ==========
      # TODO

    # ====================================================================================================
    elif cluster == 0x0028              # ========== Basic Information Cluster cluster 11.1 p.565 ==========

      if   attribute == 0x0000          #  ---------- DataModelRevision / u16 ----------
        return TLV.create_TLV(TLV.U2, 0)
      elif attribute == 0x0001          #  ---------- VendorName / string ----------
        return TLV.create_TLV(TLV.UTF1, "Tasmota")
      elif attribute == 0x0002          #  ---------- VendorID / vendor-id ----------
        return TLV.create_TLV(TLV.U2, self.device.vendorid)    # Vendor ID reserved for development
      elif attribute == 0x0003          #  ---------- ProductName / string ----------
        return TLV.create_TLV(TLV.UTF1, tasmota.cmd("DeviceName")['DeviceName'])
      elif attribute == 0x0004          #  ---------- ProductID / u16 (opt) ----------
        return TLV.create_TLV(TLV.U2, 32768)    # taken from esp-matter example
      elif attribute == 0x0005          #  ---------- NodeLabel / string ----------
        return TLV.create_TLV(TLV.UTF1, tasmota.cmd("FriendlyName")['FriendlyName1'])
      elif attribute == 0x0006          #  ---------- Location / string ----------
        return TLV.create_TLV(TLV.UTF1, "XX")   # no location
      elif attribute == 0x0007          #  ---------- HardwareVersion / u16 ----------
        return TLV.create_TLV(TLV.U2, 0)
      elif attribute == 0x0008          #  ---------- HardwareVersionString / string ----------
        return TLV.create_TLV(TLV.UTF1, tasmota.cmd("Status 2")['StatusFWR']['Hardware'])
      elif attribute == 0x0009          #  ---------- SoftwareVersion / u32 ----------
        return TLV.create_TLV(TLV.U2, 0)
      elif attribute == 0x000A          #  ---------- SoftwareVersionString / string ----------
        return TLV.create_TLV(TLV.UTF1, tasmota.cmd("Status 2")['StatusFWR']['Version'])
      end

    # ====================================================================================================
    elif cluster == 0x003F              # ========== Group Key Management Cluster 11.2 p.572 ==========
      # TODO

    # ====================================================================================================
    elif cluster == 0x002B              # ========== Localization Configuration Cluster 11.3 p.580 ==========

      if   attribute == 0x0000          #  ---------- ActiveLocale / string ----------
        return TLV.create_TLV(TLV.UTF1, tasmota.locale())
      elif attribute == 0x0001          #  ---------- SupportedLocales / list[string] ----------
        var locl = TLV.Matter_TLV_list()
        locl.add_TLV(nil, TLV.UTF1, tasmota.locale())
        return locl
      end

    # ====================================================================================================
    elif cluster == 0x002C              # ========== Time Format Localization Cluster 11.4 p.581 ==========

      if   attribute == 0x0000          #  ---------- HourFormat / HourFormat ----------
        return TLV.create_TLV(TLV.U1, 1)  # 1 = 24hr
      elif attribute == 0x0001          #  ---------- ActiveCalendarType / CalendarType ----------
        return TLV.create_TLV(TLV.U1, 4)  # 4 = Gregorian
      elif attribute == 0x0002          #  ---------- SupportedCalendarTypes / list[CalendarType] ----------
        var callist = TLV.Matter_TLV_list()
        callist.add_TLV(nil, TLV.create_TLV(TLV.U1, 4))
        return callist
      end

    # ====================================================================================================
    elif cluster == 0x0031              # ========== Network Commissioning Cluster cluster 11.8 p.606 ==========
      if   attribute == 0x0003          #  ---------- ConnectMaxTimeSeconds / uint8 ----------
        return TLV.create_TLV(TLV.U1, 30)    # 30 - value taking from example in esp-matter
      elif attribute == 0xFFFC          #  ---------- FeatureMap / map32 ----------
        return TLV.create_TLV(TLV.U4, 0)    # 15s ??? TOOD what should we put here?
      end

    # ====================================================================================================
    elif   cluster == 0x001D              # ========== Descriptor Cluster 9.5 p.453 ==========
      if   attribute == 0x0000          # ---------- DeviceTypeList / list[DeviceTypeStruct] ----------
      elif attribute == 0x0001          # ---------- ServerList / list[cluster-id] ----------
        var sl = TLV.Matter_TLV_array()
        for cl: self.get_cluster_list()
          sl.add_TLV(nil, TLV.U4, cl)
        end
        return sl
      elif attribute == 0x0002          # ---------- ClientList / list[cluster-id] ----------
        var cl = TLV.Matter_TLV_array()
        return cl
      elif attribute == 0x0003          # ---------- PartsList / list[endpoint-no]----------
        var eps = self.device.get_active_endpoints(true)
        var pl = TLV.Matter_TLV_array()
        for ep: eps
          pl.add_TLV(nil, TLV.U2, ep)     # add each endpoint
        end
        return pl
      end

    else
      ctx.status = matter.UNSUPPORTED_CLUSTER

    end
    # no match found, return that the attribute is unsupported
  end

  #############################################################
  # Invoke a command
  #
  # returns a TLV object if successful, contains the response
  #   or an `int` to indicate a status
  def invoke_request(msg, val, ctx)
    import crypto
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var command = ctx.command
    var session = msg.session
    if   cluster == 0x0030              # ========== GeneralCommissioning cluster 11.9 p.627 ==========

      if   command == 0x0000            # ---------- ArmFailSafe ----------
        # create ArmFailSafeResponse
        # ID=1
        #  0=ErrorCode (OK=0)
        #  1=DebugText
        var ExpiryLengthSeconds = val.findsubval(0, 900)
        var Breadcrumb = val.findsubval(1, 0)
        session.breadcrumb = Breadcrumb

        var afsr = TLV.Matter_TLV_struct()
        afsr.add_TLV(0, TLV.U1, 0)      # ErrorCode = OK
        afsr.add_TLV(1, TLV.UTF1, "")   # DebugText = ""
        ctx.command = 0x01              # ArmFailSafeResponse
        return afsr

      elif command == 0x0002            # ---------- SetRegulatoryConfig ----------
        var NewRegulatoryConfig = val.findsubval(0)     # RegulatoryLocationType Enum
        var CountryCode = val.findsubval(1, "XX")
        var Breadcrumb = val.findsubval(2, 0)
        session.breadcrumb = Breadcrumb
        # create SetRegulatoryConfigResponse
        # ID=1
        #  0=ErrorCode (OK=0)
        #  1=DebugText
        var srcr = TLV.Matter_TLV_struct()
        srcr.add_TLV(0, TLV.U1, 0)      # ErrorCode = OK
        srcr.add_TLV(1, TLV.UTF1, "")   # DebugText = ""
        ctx.command = 0x03              # SetRegulatoryConfigResponse
        return srcr

      elif command == 0x0004            # ---------- CommissioningComplete p.636 ----------
        # no data
        session.breadcrumb = 0          # clear breadcrumb
        session.set_no_expiration()

        # create CommissioningCompleteResponse
        # ID=1
        #  0=ErrorCode (OK=0)
        #  1=DebugText
        var ccr = TLV.Matter_TLV_struct()
        ccr.add_TLV(0, TLV.U1, 0)      # ErrorCode = OK
        ccr.add_TLV(1, TLV.UTF1, "")   # DebugText = ""
        ctx.command = 0x05              # CommissioningCompleteResponse

        self.device.start_commissioning_complete_deferred(session)
        return ccr                      # trigger a standalone ack
      end

    elif cluster == 0x003E              # ========== Node Operational Credentials Cluster 11.17 p.704 ==========

      if   command == 0x0002            # ---------- CertificateChainRequest ----------
        var CertificateType = val.findsubval(0)     # CertificateChainType Enum 1=DACCertificate 2=PAICertificate
        if CertificateType != 1 && CertificateType != 2
          ctx.status = matter.UNSUPPORTED_COMMAND
          return nil
        end
        # create CertificateChainResponse
        # ID=1
        #  0=Certificate (octstr)
        var ccr = TLV.Matter_TLV_struct()
        ccr.add_TLV(0, TLV.B2, CertificateType == 1 ? matter.DAC_Cert_FFF1_8000() : matter.PAI_Cert_FFF1())      # send DAC_Cert_FFF1_8000 or PAI_Cert_FFF1
        ctx.command = 0x03              # CertificateChainResponse
        return ccr

      elif command == 0x0000            # ---------- AttestationRequest ----------
        var AttestationNonce = val.findsubval(0)     # octstr
        if size(AttestationNonce) != 32   return nil end    # check size on nonce
        ctx.command = 0x01              # AttestationResponse

        # build Attestation Elements 11.17.5.4 p.707
        var att_elts = TLV.Matter_TLV_struct()
        att_elts.add_TLV(1, TLV.B2, matter.CD_FFF1_8000())    # certification_declaration
        att_elts.add_TLV(2, TLV.B1, AttestationNonce)         # attestation_nonce
        att_elts.add_TLV(3, TLV.U4, tasmota.rtc()['utc'])     # timestamp in epoch-s
        var attestation_message = att_elts.encode()

        var ac = session.get_ac()
        var attestation_tbs = attestation_message + ac
        tasmota.log("MTR: attestation_tbs=" + attestation_tbs.tohex(), 3)

        var attestation_signature = crypto.EC_P256().ecdsa_sign_sha256(matter.DAC_Priv_FFF1_8000(), attestation_tbs)

        # create AttestationResponse
        # 0=AttestationElements (octstr max 900 bytes)
        # 1=AttestationSignature (octstr 64)
        var ar = TLV.Matter_TLV_struct()
        ar.add_TLV(0, TLV.B2, attestation_message)      # AttestationElements
        ar.add_TLV(1, TLV.B1, attestation_signature)    # AttestationElements
        ctx.command = 0x01              # AttestationResponse
        return ar

      elif command == 0x0004            # ---------- CSRRequest ----------
        var CSRNonce = val.findsubval(0)     # octstr 32
        if size(CSRNonce) != 32   return nil end    # check size on nonce
        var IsForUpdateNOC = val.findsubval(1, false)     # bool

        var csr = session.gen_CSR()

        var nocsr_elements = TLV.Matter_TLV_struct()
        nocsr_elements.add_TLV(1, TLV.B2, csr)
        nocsr_elements.add_TLV(2, TLV.B1, CSRNonce)
        var nocsr_elements_message = nocsr_elements.encode()
        # sign with attestation challenge
        var nocsr_tbs = nocsr_elements_message + session.get_ac()
        tasmota.log("MTR: nocsr_tbs=" + nocsr_tbs.tohex(), 3)
        var attestation_signature = crypto.EC_P256().ecdsa_sign_sha256(matter.DAC_Priv_FFF1_8000(), nocsr_tbs)
        
        # create CSRResponse
        # 0=NOCSRElements (octstr max 900 bytes)
        # 1=AttestationSignature (octstr 64)
        var csrr = TLV.Matter_TLV_struct()
        csrr.add_TLV(0, TLV.B2, nocsr_elements_message)   # AttestationElements
        csrr.add_TLV(1, TLV.B1, attestation_signature)    # AttestationElements
        ctx.command = 0x05              # CSRResponse
        return csrr

      elif command == 0x000B            # ---------- AddTrustedRootCertificate ----------
        var RootCACertificate = val.findsubval(0)     # octstr 400 max
        session.set_ca(RootCACertificate)
        tasmota.log("MTR: received ca_root="+RootCACertificate.tohex(), 3)
        ctx.status = matter.SUCCESS                  # OK
        return nil                      # trigger a standalone ack

      elif command == 0x0006            # ---------- AddNOC ----------
        var NOCValue = val.findsubval(0)        # octstr max 400
        var ICACValue = val.findsubval(1)       # octstr max 400
        var IpkValue = val.findsubval(2)        # octstr max 16
        var CaseAdminSubject = val.findsubval(3)
        var AdminVendorId = val.findsubval(4)

        if session.get_ca() == nil
          tasmota.log("MTR: Error: AdNOC without CA", 2)
          return nil
        end

        session.set_noc(NOCValue, ICACValue)
        session.set_ipk_epoch_key(IpkValue)
        session.admin_subject = CaseAdminSubject
        session.admin_vendor = AdminVendorId

        # extract important information from NOC
        var noc_cert = matter.TLV.parse(NOCValue)
        var dnlist = noc_cert.findsub(6)
        var fabric = dnlist.findsubval(21)
        var deviceid = dnlist.findsubval(17)
        if !fabric || !deviceid
          tasmota.log("MTR: Error: no fabricid nor deviceid in NOC certificate", 2)
          return false
        end
        # convert fo bytes(8)
        if type(fabric) == 'int'    fabric = int64(fabric).tobytes()      else fabric = fabric.tobytes()     end
        if type(deviceid) == 'int'  deviceid = int64(deviceid).tobytes()  else deviceid = deviceid.tobytes() end

        var root_ca = matter.TLV.parse(session.get_ca()).findsubval(9)    # extract public key from ca
        root_ca = root_ca[1..]            # remove first byte as per Matter specification
        var info = bytes().fromstring("CompressedFabric")   # as per spec, 4.3.2.2 p.99
        var hk = crypto.HKDF_SHA256()
        var fabric_rev = fabric.copy().reverse()
        var k_fabric = hk.derive(root_ca, fabric_rev, info, 8)
        session.set_fabric_device(fabric, deviceid, k_fabric)

        # move to next step
        self.device.start_operational_dicovery_deferred(session)
        # create NOCResponse
        # 0=StatusCode
        # 1=FabricIndex (1-254) (opt)
        # 2=DebugText (opt)
        var nocr = TLV.Matter_TLV_struct()
        nocr.add_TLV(0, TLV.U1, matter.SUCCESS)   # Status
        nocr.add_TLV(1, TLV.U1, 1)   # fabric-index
        ctx.command = 0x08              # NOCResponse
        return nocr

      elif command == 0x0009            # ---------- UpdateFabricLabel ----------
        var label = val.findsubval(0)     # Label string max 32
        session.set_fabric_label(label)
        ctx.status = matter.SUCCESS                  # OK
        return nil                      # trigger a standalone ack

      elif command == 0x000A            # ---------- RemoveFabric ----------
        var index = val.findsubval(0)     # FabricIndex
        var sessions_act = self.device.sessions.sessions_active()
        if index >= 1 && index <= size(sessions_act)
          var session_deleted = sessions_act[index - 1]
          tasmota.log("MTR: removing fabric " + session.fabric.copy().reverse().tohex())
          self.device.sessions.remove_session()
          self.device.sessions.save()
        else
          # TODO return error 11 InvalidFabricIndex
        end
        ctx.status = matter.SUCCESS                  # OK
        return nil                      # trigger a standalone ack

      end
    end

  end
end
matter.Plugin_core = Matter_Plugin_core
  