#
# Matter_Plugin_1_Root.be - implements the core features that a Matter device must implemment
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

# Matter plug-in for root behavior

#@ solidify:Matter_Plugin_Root,weak

class Matter_Plugin_Root : Matter_Plugin
  static var TYPE = "root"            # name of the plug-in in json
  static var DISPLAY_NAME = "Root node"       # display name of the plug-in
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    # 0x001D: inherited               # Descriptor Cluster 9.5 p.453
    0x001F: [0,2,3,4],                # Access Control Cluster, p.461
    0x0028: [0,1,2,3,4,5,6,7,8,9,0x0A,0x0F,0x11,0x12,0x13],# Basic Information Cluster cluster 11.1 p.565
    # 0x002A: [0,1,2,3],                # OTA Software Update Requestor Cluster Definition 11.19.7 p.762
    0x002B: [0,1],                    # Localization Configuration Cluster 11.3 p.580
    0x002C: [0,1,2],                  # Time Format Localization Cluster 11.4 p.581
    0x0030: [0,1,2,3,4],              # GeneralCommissioning cluster 11.9 p.627
    0x0031: [3,4],                    # Network Commissioning Cluster cluster 11.8 p.606
    0x0032: [],                       # Diagnostic Logs Cluster 11.10 p.637
    0x0033: [0,1,2,8],                # General Diagnostics Cluster 11.11 p.642
    0x0034: [],                       # Software Diagnostics Cluster 11.12 p.654
    0x0038: [0,1,7],                  # Time Synchronization 11.16 p.689
    0x003C: [0,1,2],                  # Administrator Commissioning Cluster 11.18 p.725
    0x003E: [0,1,2,3,4,5],            # Node Operational Credentials Cluster 11.17 p.704
    0x003F: []                        # Group Key Management Cluster 11.2 p.572
  })
  # static var CLUSTERS_ICD = matter.consolidate_clusters(_class.CLUSTERS, {
  #   0x0046: [0,1,2]                   # ICD Management Cluster
  # })
  static var TYPES = { 0x0016: 1 }       # Root node

  # # ICD options
  # var icd_idle, icd_active, icd_threshold

  #############################################################
  # Constructor
  def init(device, endpoint, config)
    super(self).init(device, endpoint, config)
    # # check if we have ICD parameters
    # self.icd_idle = config.find("icd_idle", nil)
    # if (self.icd_idle != nil)
    #   self.icd_active = config.find("icd_active", 5000)       # default 5s
    #   self.icd_threshold = config.find("icd_threshold", 300)     # default 300ms
    # end
    # publish mandatory events
    self.publish_event(0x0028, 0x00, matter.EVENT_CRITICAL, matter.TLV.Matter_TLV_item().set(matter.TLV.U4, tasmota.version()))   # Event StartUp - Software Version
    self.publish_event(0x0033, 0x03, matter.EVENT_CRITICAL, matter.TLV.Matter_TLV_item().set(matter.TLV.U1, 1))   # Event BootReason - PowerOnReboot - TODO if we need to refine
  end

  #############################################################
  # consolidate_clusters
  #
  # Build a consolidated map of all the `CLUSTERS` static vars
  # from the inheritance hierarchy
  # def get_clusters()
  #   if (self.icd_idle == nil)
  #     return self.CLUSTERS
  #   else
  #     return self.CLUSTERS_ICD
  #   end
  # end

  #############################################################
  # read an attribute
  #
  def read_attribute(session, ctx, tlv_solo)
    import string
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    if   cluster == 0x0030              # ========== GeneralCommissioning cluster 11.9 p.627 ==========

      if   attribute == 0x0000          # ---------- Breadcrumb ----------
        return tlv_solo.set(TLV.U8, session._breadcrumb)
      elif attribute == 0x0001          # ---------- BasicCommissioningInfo / BasicCommissioningInfo----------
        var bci = TLV.Matter_TLV_struct()
        bci.add_TLV(0, TLV.U2, 60)      # FailSafeExpiryLengthSeconds
        bci.add_TLV(1, TLV.U2, 900)     # MaxCumulativeFailsafeSeconds
        return bci
      elif attribute == 0x0002          # ---------- RegulatoryConfig / RegulatoryLocationType ----------
        return tlv_solo.set(TLV.U1, 2)    # 2 = IndoorOutdoor | esp-matter = 0
      elif attribute == 0x0003          # ---------- LocationCapability / RegulatoryLocationType----------
        return tlv_solo.set(TLV.U1, 2)    # 2 = IndoorOutdoor
      elif attribute == 0x0004          # ---------- SupportsConcurrentConnection / bool ----------
        return tlv_solo.set(TLV.BOOL, false)    # false - maybe can set to true
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
          eth.add_TLV(4, TLV.B1, mac)               # HardwareAddress
          var ip4 = eth.add_array(5)                # IPv4Addresses
          ip4.add_TLV(nil, TLV.B1, matter.get_ip_bytes(tas_eth.find("ip", "")))
          var ip6 = eth.add_array(6)                # IPv6Addresses
          ip6.add_TLV(nil, TLV.B1, matter.get_ip_bytes(tas_eth.find("ip6local", "")))
          ip6.add_TLV(nil, TLV.B1, matter.get_ip_bytes(tas_eth.find("ip6", "")))
          eth.add_TLV(7, TLV.U1, 2)                 # InterfaceType, p646
        end

        var tas_wif = tasmota.wifi()
        if (tas_wif['up'])
          var wif = nwi.add_struct(nil)
          wif.add_TLV(0, TLV.UTF1, 'wifi')          # Name
          wif.add_TLV(1, TLV.BOOL, 1)               # IsOperational
          wif.add_TLV(2, TLV.BOOL, 1)               # OffPremiseServicesReachableIPv4
          wif.add_TLV(3, TLV.NULL, nil)             # OffPremiseServicesReachableIPv6
          var mac = bytes().fromhex(string.replace(tas_wif.find("mac", ""), ":", ""))
          wif.add_TLV(4, TLV.B1, mac)               # HardwareAddress
          var ip4 = wif.add_array(5)                # IPv4Addresses
          ip4.add_TLV(nil, TLV.B1, matter.get_ip_bytes(tas_wif.find("ip", "")))
          var ip6 = wif.add_array(6)                # IPv6Addresses
          ip6.add_TLV(nil, TLV.B1, matter.get_ip_bytes(tas_wif.find("ip6local", "")))
          ip6.add_TLV(nil, TLV.B1, matter.get_ip_bytes(tas_wif.find("ip6", "")))
          wif.add_TLV(7, TLV.U1, 1)                 # InterfaceType, p646
        end
        return nwi
      elif attribute == 0x0001          #  ---------- RebootCount u16 ----------
        return tlv_solo.set(TLV.U2, tasmota.cmd("Status 1", true)['StatusPRM']['BootCount'])
      elif attribute == 0x0002          #  ---------- UpTime u16 ----------
        return tlv_solo.set(TLV.U4, tasmota.cmd("Status 11", true)['StatusSTS']['UptimeSec'])
      # TODO add later other attributes
      elif attribute == 0x0008          #  ---------- TestEventTriggersEnabled bool ----------
        return tlv_solo.set(TLV.BOOL, false)    # false - maybe can set to true
      end
        
    # ====================================================================================================
    elif cluster == 0x0034              # ========== Software Diagnostics Cluster 11.12 p.654 ==========
      # no mandatory attributes - to be added later (maybe)

    # ====================================================================================================
    elif cluster == 0x0038              # ========== Time Synchronization 11.16 p.689 ==========
      if   attribute == 0x0000          #  ---------- UTCTime / epoch_us ----------
        var epoch_us = int64(tasmota.rtc_utc()) * int64(1000000)
        return tlv_solo.set(TLV.U8, epoch_us)     # TODO test the conversion of int64()
      elif attribute == 0x0001          #  ---------- Granularity / enum ----------
        return tlv_solo.set(TLV.U1, 3)     # MillisecondsGranularity (NTP every hour, i.e. 36ms max drift)
      # TODO add some missing args
      elif attribute == 0x0007          #  ---------- LocalTime / epoch_us ----------
        var epoch_us = int64(tasmota.rtc('local')) * int64(1000000)
        return tlv_solo.set(TLV.U8, epoch_us)     # TODO test the conversion of int64()
      end

    # ====================================================================================================
    elif cluster == 0x003E              # ========== Node Operational Credentials Cluster 11.17 p.704 ==========
      self.ack_request(ctx)             # long operation, send Ack first

      if   attribute == 0x0000          #  ---------- NOCs / list[NOCStruct] ----------
        var nocl = TLV.Matter_TLV_array() # NOCs, p.711
        var fabs = ctx.fabric_filtered ? [session.get_fabric()] : self.device.sessions.active_fabrics()
        for loc_fabric: fabs
          if loc_fabric.is_marked_for_deletion()    continue end    # fabric is scheduled for deletion
          if loc_fabric == nil    continue  end
          var nocs = nocl.add_struct(nil)
          nocs.add_TLV(1, TLV.B2, loc_fabric.get_noc())      # NOC
          nocs.add_TLV(2, TLV.B2, loc_fabric.get_icac())     # ICAC
          nocs.add_TLV(0xFE, TLV.U2, loc_fabric.get_fabric_index())    # Label
        end
        return nocl
      elif attribute == 0x0001          #  ---------- Fabrics / list[FabricDescriptorStruct] ----------
        var fabrics = TLV.Matter_TLV_array() # Fabrics, p.711
        var fabs = ctx.fabric_filtered ? [session.get_fabric()] : self.device.sessions.active_fabrics()
        for loc_fabric: fabs
          if loc_fabric == nil    continue  end
          if loc_fabric.is_marked_for_deletion()    continue end    # fabric is scheduled for deletion
          var root_ca_tlv = TLV.parse(loc_fabric.get_ca())
          var fab = fabrics.add_struct(nil)            # encoding see p.303
          fab.add_TLV(1, TLV.B2, root_ca_tlv.findsubval(9)) # RootPublicKey
          fab.add_TLV(2, TLV.U2, loc_fabric.get_admin_vendor())      # VendorID
          fab.add_TLV(3, TLV.U8, loc_fabric.get_fabric_id_as_int64())            # FabricID
          fab.add_TLV(4, TLV.U8, loc_fabric.get_device_id_as_int64())          # NodeID
          fab.add_TLV(5, TLV.UTF1, loc_fabric.get_fabric_label())    # Label
          fab.add_TLV(0xFE, TLV.U2, loc_fabric.get_fabric_index())    # idx
        end
        return fabrics
      elif attribute == 0x0002          #  ---------- SupportedFabrics / u1 ----------
        return tlv_solo.set(TLV.U1, matter.Fabric._MAX_CASE)     # Max 5 fabrics
      elif attribute == 0x0003          #  ---------- CommissionedFabrics / u1 ----------
        var fabric_actice = self.device.sessions.count_active_fabrics()
        return tlv_solo.set(TLV.U1, fabric_actice)  # number of active fabrics
      elif attribute == 0x0004          #  ---------- TrustedRootCertificates / list[octstr] ----------
        # TODO
      elif attribute == 0x0005          #  ---------- Current FabricIndex / u1 ----------
        var fab_index = session._fabric.get_fabric_index()
        if fab_index == nil   fab_index = 0   end     # if PASE session, then the fabric index should be zero
        return tlv_solo.set(TLV.U1, fab_index)      # number of active sessions
      end

    # ====================================================================================================
    elif cluster == 0x003C              # ========== Administrator Commissioning Cluster 11.18 p.725 ==========
      if   attribute == 0x0000          #  ---------- WindowStatus / u8 ----------
        var commissioning_open = self.device.commissioning.is_commissioning_open()
        var basic_commissioning = self.device.commissioning.is_root_commissioning_open()
        var val = commissioning_open ? (basic_commissioning ? 2 #-BasicWindowOpen-# : 1 #-EnhancedWindowOpen-#) : 0 #-WindowNotOpen-#
        return tlv_solo.set(TLV.U1, val)
      elif attribute == 0x0001          #  ---------- AdminFabricIndex / u16 ----------
        var admin_fabric = self.device.commissioning.commissioning_admin_fabric
        if admin_fabric != nil
          return tlv_solo.set_or_nil(TLV.U2, admin_fabric.get_fabric_index())
        end
      elif attribute == 0x0002          #  ---------- AdminVendorId / u16 ----------
        var admin_fabric = self.device.commissioning.commissioning_admin_fabric
        if admin_fabric != nil
          return tlv_solo.set_or_nil(TLV.U2, admin_fabric.get_admin_vendor())
        end
      end
        
    # ====================================================================================================
    elif cluster == 0x0028              # ========== Basic Information Cluster cluster 11.1 p.565 ==========
      self.ack_request(ctx)             # long operation, send Ack first

      if   attribute == 0x0000          #  ---------- DataModelRevision / CommissioningWindowStatus ----------
        return tlv_solo.set(TLV.U2, 1)
      elif attribute == 0x0001          #  ---------- VendorName / string ----------
        return tlv_solo.set(TLV.UTF1, "Tasmota")
      elif attribute == 0x0002          #  ---------- VendorID / vendor-id ----------
        return tlv_solo.set(TLV.U2, self.device.VENDOR_ID)    # Vendor ID reserved for development
      elif attribute == 0x0003          #  ---------- ProductName / string ----------
        return tlv_solo.set(TLV.UTF1, tasmota.cmd("DeviceName", true)['DeviceName'])
      elif attribute == 0x0004          #  ---------- ProductID / u16 (opt) ----------
        return tlv_solo.set(TLV.U2, 32768)    # taken from esp-matter example
      elif attribute == 0x0005          #  ---------- NodeLabel / string ----------
        return tlv_solo.set(TLV.UTF1, tasmota.cmd("FriendlyName", true)['FriendlyName1'])
      elif attribute == 0x0006          #  ---------- Location / string ----------
        return tlv_solo.set(TLV.UTF1, "XX")   # no location
      elif attribute == 0x0007          #  ---------- HardwareVersion / u16 ----------
        return tlv_solo.set(TLV.U2, 0)
      elif attribute == 0x0008          #  ---------- HardwareVersionString / string ----------
        return tlv_solo.set(TLV.UTF1, tasmota.cmd("Status 2", true)['StatusFWR']['Hardware'])
      elif attribute == 0x0009          #  ---------- SoftwareVersion / u32 ----------
        return tlv_solo.set(TLV.U4, tasmota.version())
      elif attribute == 0x000A          #  ---------- SoftwareVersionString / string ----------
        var version_full = tasmota.cmd("Status 2", true)['StatusFWR']['Version']
        var version_end = string.find(version_full, '(')
        if version_end > 0    version_full = version_full[0..version_end - 1]   end
        return tlv_solo.set(TLV.UTF1, version_full)
      elif attribute == 0x000F          #  ---------- SerialNumber / string ----------
        return tlv_solo.set(TLV.UTF1, tasmota.wifi().find("mac", ""))
      elif attribute == 0x0011          #  ---------- Reachable / bool ----------
        return tlv_solo.set(TLV.BOOL, 1)     # by default we are reachable
      elif attribute == 0x0012          #  ---------- UniqueID / string 32 max ----------
        return tlv_solo.set(TLV.UTF1, tasmota.wifi().find("mac", ""))
      elif attribute == 0x0013          #  ---------- CapabilityMinima / CapabilityMinimaStruct ----------
        var cps = TLV.Matter_TLV_struct()
        cps.add_TLV(0, TLV.U2, 3)       # CaseSessionsPerFabric = 3
        cps.add_TLV(1, TLV.U2, 3)       # SubscriptionsPerFabric = 5
        return cps
      end

    # ====================================================================================================
    elif cluster == 0x003F              # ========== Group Key Management Cluster 11.2 p.572 ==========
      # TODO

    # ====================================================================================================
    elif cluster == 0x002A              # ========== OTA Software Update Requestor Cluster Definition 11.19.7 p.762 ==========

      if   attribute == 0x0000          #  ---------- DefaultOTAProviders / list[ProviderLocationStruct] ----------
        return TLV.Matter_TLV_array()   # empty list for now TODO
      elif attribute == 0x0001          #  ---------- UpdatePossible / bool ----------
        return tlv_solo.set(TLV.BOOL, 0)  # we claim that update is not possible, would require to go to Tasmota UI
      elif attribute == 0x0002          #  ---------- UpdateState / UpdateStateEnum ----------
        return tlv_solo.set(TLV.U1, 1)  # Idle
      elif attribute == 0x0003          #  ---------- UpdateStateProgress / uint8 ----------
        return tlv_solo.set(TLV.NULL, nil)  # null, nothing in process
      end

    # ====================================================================================================
    elif cluster == 0x002B              # ========== Localization Configuration Cluster 11.3 p.580 ==========

      if   attribute == 0x0000          #  ---------- ActiveLocale / string ----------
        return tlv_solo.set(TLV.UTF1, tasmota.locale())
      elif attribute == 0x0001          #  ---------- SupportedLocales / list[string] ----------
        var locl = TLV.Matter_TLV_array()
        locl.add_TLV(nil, TLV.UTF1, tasmota.locale())
        return locl
      end

    # ====================================================================================================
    elif cluster == 0x002C              # ========== Time Format Localization Cluster 11.4 p.581 ==========

      if   attribute == 0x0000          #  ---------- HourFormat / HourFormat ----------
        return tlv_solo.set(TLV.U1, 1)  # 1 = 24hr
      elif attribute == 0x0001          #  ---------- ActiveCalendarType / CalendarType ----------
        return tlv_solo.set(TLV.U1, 4)  # 4 = Gregorian
      elif attribute == 0x0002          #  ---------- SupportedCalendarTypes / list[CalendarType] ----------
        var callist = TLV.Matter_TLV_array()
        callist.add_TLV(nil, TLV.create_TLV(TLV.U1, 4))
        return callist
      end

    # ====================================================================================================
    elif cluster == 0x0031              # ========== Network Commissioning Cluster cluster 11.8 p.606 ==========
      if   attribute == 0x0003          #  ---------- ConnectMaxTimeSeconds / uint8 ----------
        return tlv_solo.set(TLV.U1, 30)    # 30 - value taking from example in esp-matter
      end

    elif cluster == 0x001D              # ========== Descriptor Cluster 9.5 p.453 ==========

      # overwrite ClientList
      if   attribute == 0x0002          # ---------- ClientList / list[cluster-id] ----------
        var pl = TLV.Matter_TLV_array()
        # from connectedhome reference implementation
        pl.add_TLV(nil, TLV.U2, 0x001F)     # Access Control Cluster
        return pl        
      # overwrite PartsList
      elif attribute == 0x0003          # ---------- PartsList / list[endpoint-no]----------
        var pl = TLV.Matter_TLV_array()
        var eps = self.device.get_active_endpoints(true)
        var disable_bridge_mode = self.device.disable_bridge_mode
        for ep: eps
          # if bridge mode is disabled, don't announce Aggregatore (above 0xFF00)
          if !disable_bridge_mode || ep != matter.AGGREGATOR_ENDPOINT
            pl.add_TLV(nil, TLV.U2, ep)     # add each endpoint
          end
        end
        return pl
      end

    # ====================================================================================================
    # elif cluster == 0x0046              # ========== ICD Management Cluster ==========

    #   if   attribute == 0x0000          #  ---------- IdleModeDuration / uint32 (seconds) ----------
    #     return tlv_solo.set_or_nil(TLV.U4, self.icd_idle)
    #   elif attribute == 0x0001          #  ---------- ActiveModeDuration / uint32 (milliseconds) ----------
    #     return tlv_solo.set_or_nil(TLV.U4, self.icd_active)
    #   elif attribute == 0x0002          #  ---------- ActiveModeThreshold / uint16 (milliseconds) ----------
    #     return tlv_solo.set_or_nil(TLV.U2, self.icd_threshold)
    #   end

    end
    return super(self).read_attribute(session, ctx, tlv_solo)
  end

  #############################################################
  # Invoke a command
  #
  # returns a TLV object if successful, contains the response
  #   or an `int` to indicate a status
  def invoke_request(session, val, ctx)
    import crypto
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var command = ctx.command
    if   cluster == 0x0030              # ========== GeneralCommissioning cluster 11.9 p.627 ==========

      if   command == 0x0000            # ---------- ArmFailSafe ----------
        # create ArmFailSafeResponse
        # ID=1
        #  0=ErrorCode (OK=0)
        #  1=DebugText
        var ExpiryLengthSeconds = val.findsubval(0, 900)
        var Breadcrumb = val.findsubval(1, 0)
        session._breadcrumb = Breadcrumb

        var afsr = TLV.Matter_TLV_struct()
        afsr.add_TLV(0, TLV.U1, 0)      # ErrorCode = OK
        afsr.add_TLV(1, TLV.UTF1, "")   # DebugText = ""
        ctx.command = 0x01              # ArmFailSafeResponse
        return afsr

      elif command == 0x0002            # ---------- SetRegulatoryConfig ----------
        var NewRegulatoryConfig = val.findsubval(0)     # RegulatoryLocationType Enum
        var CountryCode = val.findsubval(1, "XX")
        var Breadcrumb = val.findsubval(2, 0)
        session._breadcrumb = Breadcrumb
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
        self.ack_request(ctx)           # long operation, send Ack first
        # no data
        if session._fabric
          session._breadcrumb = 0          # clear breadcrumb
          session._fabric.fabric_completed()      # fabric information is complete, persist
          session.set_no_expiration()
          session.save()

          # create CommissioningCompleteResponse
          # ID=1
          #  0=ErrorCode (OK=0)
          #  1=DebugText
          var ccr = TLV.Matter_TLV_struct()
          ccr.add_TLV(0, TLV.U1, 0)      # ErrorCode = OK
          ccr.add_TLV(1, TLV.UTF1, "")   # DebugText = ""
          ctx.command = 0x05              # CommissioningCompleteResponse

          self.device.commissioning.start_commissioning_complete_deferred(session)
          return ccr
        else
          raise "context_error", "CommissioningComplete: no fabric attached"
        end
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
        att_elts.add_TLV(3, TLV.U4, tasmota.rtc_utc())     # timestamp in epoch-s
        var attestation_message = att_elts.tlv2raw()

        var ac = session.get_ac()
        var attestation_tbs = attestation_message + ac
        # log("MTR: attestation_tbs=" + attestation_tbs.tohex(), 4)

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
        self.ack_request(ctx)           # long operation, send Ack first
        var CSRNonce = val.findsubval(0)     # octstr 32
        if size(CSRNonce) != 32   return nil end    # check size on nonce
        var IsForUpdateNOC = val.findsubval(1, false)     # bool
        # log(format("MTR: CSRRequest CSRNonce=%s IsForUpdateNOC=%s", str(CSRNonce), str(IsForUpdateNOC)), 4)

        var csr = session.gen_CSR()

        var nocsr_elements = TLV.Matter_TLV_struct()
        nocsr_elements.add_TLV(1, TLV.B2, csr)
        nocsr_elements.add_TLV(2, TLV.B1, CSRNonce)
        var nocsr_elements_message = nocsr_elements.tlv2raw()
        # sign with attestation challenge
        var nocsr_tbs = nocsr_elements_message + session.get_ac()
        # log("MTR: nocsr_tbs=" + nocsr_tbs.tohex(), 4)
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
        # TODO - additional tests are expected according to 11.17.7.13. AddTrustedRootCertificate Command
        session.set_temp_ca(RootCACertificate)
        # log("MTR: received ca_root="+RootCACertificate.tohex(), 4)
        ctx.status = matter.SUCCESS                  # OK
        return nil                      # trigger a standalone ack

      elif command == 0x0006            # ---------- AddNOC ----------
        log("MTR: AddNoc Args=" + str(val), 4)
        var NOCValue = val.findsubval(0)        # octstr max 400
        var ICACValue = val.findsubval(1)       # octstr max 400
        # Apple sends an empty ICAC instead of a missing attribute, fix this
        if size(ICACValue) == 0   ICACValue = nil   end
        var IpkValue = val.findsubval(2)        # octstr max 16
        var CaseAdminSubject = val.findsubval(3)
        var AdminVendorId = val.findsubval(4)
        # log("MTR: AddNoc NOCValue=" + (NOCValue ? NOCValue.tohex() : ""), 3)
        # log("MTR: AddNoc ICACValue=" + (ICACValue ? ICACValue.tohex() : ""), 3)
        # log("MTR: AddNoc IpkValue=" + str(IpkValue), 3)
        # log("MTR: AddNoc CaseAdminSubject=" + str(CaseAdminSubject), 3)
        # log("MTR: AddNoc AdminVendorId=" + str(AdminVendorId), 3)

        if session.get_temp_ca() == nil
          log("MTR: Error: AdNOC without CA", 2)
          return nil
        end

        var new_fabric = self.device.sessions.create_fabric()
        new_fabric.set_ca(session.get_temp_ca())          # copy temporary CA to fabric
        new_fabric.set_noc_icac(NOCValue, ICACValue)
        new_fabric.set_ipk_epoch_key(IpkValue)
        new_fabric.set_admin_subject_vendor(CaseAdminSubject, AdminVendorId)
        new_fabric.set_pk(session.get_pk())               # copy the temporary commissioning PK to the fabric

        # extract important information from NOC
        var noc_cert = matter.TLV.parse(NOCValue)
        var dnlist = noc_cert.findsub(6)
        var fabric_id = dnlist.findsubval(21)
        var deviceid = dnlist.findsubval(17)
        # log("MTR: AddNoc noc_cert=" + str(noc_cert), 3)
        # log("MTR: AddNoc dnlist=" + str(dnlist), 3)

        if !fabric_id || !deviceid
          log("MTR: Error: no fabricid nor deviceid in NOC certificate", 2)
          return false
        end
        # convert fo bytes(8)
        if type(fabric_id) == 'int' fabric_id = int64.fromu32(fabric_id).tobytes()  else fabric_id = fabric_id.tobytes() end
        if type(deviceid) == 'int'  deviceid = int64.fromu32(deviceid).tobytes()    else deviceid = deviceid.tobytes() end

        # log("MTR: AddNoc fabric_id=" + str(fabric_id), 3)
        # log("MTR: AddNoc deviceid=" + str(deviceid), 3)

        var root_ca_pub = session.get_temp_ca_pub()
        # log("MTR: AddNoc root_ca_pub=" + str(root_ca_pub), 3)
        # log("MTR: AddNoc root_ca_pub=" + root_ca_pub.tohex(), 3)
        root_ca_pub = root_ca_pub[1..]            # remove first byte as per Matter specification
        var info = bytes().fromstring("CompressedFabric")   # as per spec, 4.3.2.2 p.99
        var hk = crypto.HKDF_SHA256()
        var fabric_rev = fabric_id.copy().reverse()
        var k_fabric = hk.derive(root_ca_pub, fabric_rev, info, 8)
        var parent_fabric = session._fabric ? session._fabric : self.device.commissioning.commissioning_admin_fabric    # get parent fabric whether CASE or PASE
        new_fabric.set_fabric_device(fabric_id, deviceid, k_fabric, parent_fabric)

        # log("MTR: AddNoc k_fabric=" + str(k_fabric), 3)
        # We have a candidate fabric, add it as expirable for 2 minutes
        new_fabric.fabric_candidate()

        # move to next step
        self.device.commissioning.start_operational_discovery_deferred(new_fabric)

        # we keep the PASE session for 1 minute
        if session.is_PASE()
          session.set_expire_in_seconds(60)
        end

        # log("MTR: ------------------------------------------", 3)
        # log("MTR: session=" + matter.inspect(session), 3)
        # log("MTR: fabric=" + matter.inspect(session._fabric), 3)
        # log("MTR: ------------------------------------------", 3)
        new_fabric.log_new_fabric()        # log that we registered a new fabric
        new_fabric.assign_fabric_index()
        # create NOCResponse
        # 0=StatusCode
        # 1=FabricIndex (1-254) (opt)
        # 2=DebugText (opt)
        var nocr = TLV.Matter_TLV_struct()
        nocr.add_TLV(0, TLV.U1, matter.SUCCESS)   # Status
        nocr.add_TLV(1, TLV.U1, new_fabric.get_fabric_index())   # fabric-index
        ctx.command = 0x08              # NOCResponse
        return nocr

      elif command == 0x0009            # ---------- UpdateFabricLabel ----------
        var label = val.findsubval(0)     # Label string max 32
        session.set_fabric_label(label)
        log(format("MTR: .          Update fabric '%s' label='%s'", session._fabric.get_fabric_id().copy().reverse().tohex(), str(label)), 3)
        
        # create NOCResponse
        # 0=StatusCode
        # 1=FabricIndex (1-254) (opt)
        # 2=DebugText (opt)
        var nocr = TLV.Matter_TLV_struct()
        nocr.add_TLV(0, TLV.U1, matter.SUCCESS)   # Status
        nocr.add_TLV(1, TLV.U1, session.get_fabric().get_fabric_index())   # fabric-index
        ctx.command = 0x08              # NOCResponse
        return nocr

      elif command == 0x000A            # ---------- RemoveFabric ----------
        var index = val.findsubval(0)     # FabricIndex
        ctx.log = "fabric_index:"+str(index)

        for fab: self.device.sessions.active_fabrics()
          if fab.get_fabric_index() == index
            # log("MTR: removing fabric " + fab.get_fabric_id().copy().reverse().tohex(), 2)
            # defer actual removal to send a response
            fab.mark_for_deletion()       # this should not appear anymore in the list
            tasmota.set_timer(2000, def () self.device.remove_fabric(fab) end)

            # create NOCResponse
            # 0=StatusCode
            # 1=FabricIndex (1-254) (opt)
            # 2=DebugText (opt)
            var nocr = TLV.Matter_TLV_struct()
            nocr.add_TLV(0, TLV.U1, matter.SUCCESS)   # Status
            nocr.add_TLV(1, TLV.U1, index)   # fabric-index
            ctx.command = 0x08              # NOCResponse
            return nocr
          end
        end
        log("MTR: RemoveFabric fabric("+str(index)+") not found", 2)
        ctx.status = matter.INVALID_ACTION
        return nil                      # trigger a standalone ack

      end

    # ====================================================================================================
    elif cluster == 0x003C              # ========== Administrator Commissioning Cluster 11.18 p.725 ==========

      if   command == 0x0000          #  ---------- OpenCommissioningWindow  ----------
        var timeout = val.findsubval(0)             # CommissioningTimeout u2
        var passcode_verifier = val.findsubval(1)   # PAKEPasscodeVerifier octstr
        var discriminator = val.findsubval(2)       # Discriminator u2
        var iterations = val.findsubval(3)          # Iterations u4
        var salt = val.findsubval(4)                # Salt octstr

        log(format("MTR: OpenCommissioningWindow(timeout=%i, passcode=%s, discriminator=%i, iterations=%i, salt=%s)",
                                  timeout, passcode_verifier.tohex(), discriminator, iterations, salt.tohex()), 4)

        # check values
        if timeout == nil || passcode_verifier == nil || discriminator == nil || iterations == nil || salt == nil
          ctx.status = matter.INVALID_DATA_TYPE
          return nil                      # trigger a standalone ack
        end
        if size(passcode_verifier) != 32+65 || size(salt) < 16 || size(salt) > 32
          log("MTR: wrong size for PAKE parameters", 2)
          ctx.status = matter.CONSTRAINT_ERROR
          return nil                      # trigger a standalone ack
        end

        var w0 = passcode_verifier[0..31]
        var L  = passcode_verifier[32..]

        self.device.commissioning.start_basic_commissioning(timeout, iterations, discriminator, salt, w0, #-w1,-# L, session.get_fabric())
        # TODO announce in MDNS
        return true                   # OK
      elif command == 0x0001          #  ---------- OpenBasicCommissioningWindow  ----------
        var commissioning_timeout = val.findsubval(0)     # CommissioningTimeout
        log("MTR: OpenBasicCommissioningWindow commissioning_timeout="+str(commissioning_timeout), 3)
        self.device.commissioning.start_root_basic_commissioning(commissioning_timeout)
        return true
      elif command == 0x0002          #  ---------- RevokeCommissioning  ----------
        # TODO add checks that the commissioning window was opened by the same fabric
        self.device.commissioning.stop_basic_commissioning()
        return true
      end

    # ====================================================================================================
    elif cluster == 0x002A              # ========== OTA Software Update Requestor Cluster Definition 11.19.7 p.762 ==========

      if   command == 0x0000          #  ---------- DefaultOTAProviders  ----------
        return true                   # OK
      end
    
    else
      return super(self).invoke_request(session, val, ctx)
    end

  end

  #############################################################
  # write an attribute
  #
  def write_attribute(session, ctx, write_data)
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute
    
    # 0x001D no writable attributes
    # 0x0032 no attributes
    # 0x0033 no writable attributes
    # 0x0034 no writable attributes
    # 0x0038 no mandatory writable attributes
    # 0x003C no writable attributes
    # 0x003E no writable attributes

    if   cluster == 0x0030              # ========== GeneralCommissioning cluster 11.9 p.627 ==========

      if   attribute == 0x0000          # ---------- Breadcrumb ----------
        if type(write_data) == 'int' || isinstance(write_data, int64)
          session._breadcrumb = write_data
          self.attribute_updated(ctx.cluster, ctx.attribute)    # TODO should we have a more generalized way each time a write_attribute is triggered, declare the attribute as changed?
          return true
        else
          ctx.status = matter.CONSTRAINT_ERROR
          return false
        end
      end

    # ====================================================================================================
    elif cluster == 0x001F              # ========== Access Control Cluster 9.10 p.461 ==========
      if   attribute == 0x0000          # ACL - list[AccessControlEntryStruct]
        return true
      end

    # ====================================================================================================
    elif cluster == 0x0028              # ========== Basic Information Cluster cluster 11.1 p.565 ==========

      if   attribute == 0x0005          #  ---------- NodeLabel / string ----------
        # TODO
        return true
      elif attribute == 0x0006          #  ---------- Location / string ----------
        # TODO
        return true
      end
    # ====================================================================================================
    elif cluster == 0x002A              # ========== OTA Software Update Requestor Cluster Definition 11.19.7 p.762 ==========

      if   attribute == 0x0000          #  ---------- DefaultOTAProviders / list[ProviderLocationStruct] ----------
        return true                     # silently ignore
      end
    # ====================================================================================================
    elif cluster == 0x002B              # ========== Localization Configuration Cluster 11.3 p.580 ==========

      if   attribute == 0x0000          #  ---------- ActiveLocale / string ----------
        ctx.status = matter.CONSTRAINT_ERROR    # changing locale is not possible
        return false
      end
    # ====================================================================================================
    elif cluster == 0x002C              # ========== Time Format Localization Cluster 11.4 p.581 ==========

      if   attribute == 0x0000          #  ---------- HourFormat / HourFormat ----------
        # TODO
        return true
      elif attribute == 0x0001          #  ---------- ActiveCalendarType / CalendarType ----------
        # TODO
        return true
      end
    # ====================================================================================================
    elif cluster == 0x0031              # ========== Network Commissioning Cluster cluster 11.8 p.606 ==========
      if   attribute == 0x0004          #  ---------- InterfaceEnabled / bool ----------
        ctx.status = matter.INVALID_ACTION
        return false
      end


    end
  end
end
matter.Plugin_Root = Matter_Plugin_Root
  
