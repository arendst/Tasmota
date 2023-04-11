#
# Matter_Session.be - Support for Matter Sessions and Session Store
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

#@ solidify:Matter_Fabric,weak
#@ solidify:Matter_Session,weak
#@ solidify:Matter_Session_Store,weak

# for compilation
class Matter_Expirable end

#################################################################################
# Matter_Fabric class
#
# Record all information for a fabric that has provisioned
#
# By convetion:
#   attributes with a normal name are persisted (unless they are instances)
#   attributes starting with '_' are not persisted
#   attributes starting with '__' are cleared when a new session is created
#################################################################################
class Matter_Fabric : Matter_Expirable
  static var _MAX_CASE = 5        # maximum number of concurrent CASE sessions per fabric
  static var _GROUP_SND_INCR = 32  # counter increased when persisting
  # Group Key Derivation
  static var _GROUP_KEY = "GroupKey v1.0"  # starting with double `_` means it's not writable

  var _store                      # reference back to session store
  # timestamp
  var created
  # fabric-index
  var fabric_index                # index number for fabrics, starts with `1`
  var fabric_parent               # index of the parent fabric, i.e. the fabric that triggered the provisioning (if nested)
  # list of active sessions
  var _sessions                   # only active CASE sessions that need to be persisted
  # our own private key
  var no_private_key              # private key of the device certificate (generated at commissioning)
  # NOC information
  var root_ca_certificate         # root certificate of the initiator
  var noc                         # Node Operational Certificate in TLV Matter Certificate
  var icac                        # Initiator CA Certificate in TLV Matter Certificate
  var ipk_epoch_key               # timestamp
  # Information extracted from `noc`
  var fabric_id                   # fabric identifier as bytes(8) little endian
  var fabric_compressed           # comrpessed fabric_id identifier, hashed with root_ca public key
  var device_id                   # our own device id bytes(8) little endian
  var fabric_label                # set by UpdateFabricLabel
  # global group counters (send)
  var counter_group_data_snd      # counter for group data
  var counter_group_ctrl_snd      # counter for group command
  var _counter_group_data_snd_impl# implementation of counter_group_data_snd by matter.Counter()
  var _counter_group_ctrl_snd_impl# implementation of counter_group_ctrl_snd by matter.Counter()
  # Admin info extracted from NOC/ICAC
  var admin_subject
  var admin_vendor

  #############################################################
  def init(store)
    import crypto
    self._store = store
    self._sessions = matter.Expirable_list()
    self.fabric_label = ""
    self.created = tasmota.rtc()['utc']
    # init group counters
    self._counter_group_data_snd_impl = matter.Counter()
    self._counter_group_ctrl_snd_impl  = matter.Counter()
    self.counter_group_data_snd = self._counter_group_data_snd_impl.next() + self._GROUP_SND_INCR
    self.counter_group_ctrl_snd = self._counter_group_data_snd_impl.next() + self._GROUP_SND_INCR
  end

  def get_noc()               return self.noc               end
  def get_icac()              return self.icac              end
  def get_ipk_epoch_key()     return self.ipk_epoch_key     end
  def get_fabric_id()         return self.fabric_id         end
  def get_device_id()         return self.device_id         end
  def get_fabric_compressed() return self.fabric_compressed end
  def get_fabric_label()      return self.fabric_label      end
  def get_admin_subject()     return self.admin_subject     end
  def get_admin_vendor()      return self.admin_vendor      end
  def get_ca()                return self.root_ca_certificate end
  def get_fabric_index()      return self.fabric_index      end

  def set_fabric_index(v)     self.fabric_index = v         end

  #############################################################
  # When hydrating from persistance, update counters
  def hydrate_post()
    # reset counter_snd to highest known.
    # We advance it only in case it is actually used
    # This avoids updaing counters on dead sessions
    self._counter_group_data_snd_impl.reset(self.counter_group_data_snd)
    self._counter_group_ctrl_snd_impl.reset(self.counter_group_ctrl_snd)
    self.counter_group_data_snd = self._counter_group_data_snd_impl.val()
    self.counter_group_ctrl_snd = self._counter_group_ctrl_snd_impl.val()
  end

  #############################################################
  # Management of security counters
  #############################################################
  # Provide the next counter value, and update the last know persisted if needed
  #
  def counter_group_data_snd_next()
    import string
    var next = self._counter_group_data_snd_impl.next()
    tasmota.log(string.format("MTR: .          Counter_group_data_snd=%i", next), 3)
    if matter.Counter.is_greater(next, self.counter_group_data_snd)
      self.counter_group_data_snd = next + self._GROUP_SND_INCR
      if self.does_persist()
        # the persisted counter is behind the actual counter
        self.save()
      end
    end
    return next
  end
  #############################################################
  # Provide the next counter value, and update the last know persisted if needed
  #
  def counter_group_ctrl_snd_next()
    import string
    var next = self._counter_group_ctrl_snd_impl.next()
    tasmota.log(string.format("MTR: .          Counter_group_ctrl_snd=%i", next), 3)
    if matter.Counter.is_greater(next, self.counter_group_ctrl_snd)
      self.counter_group_ctrl_snd = next + self._GROUP_SND_INCR
      if self.does_persist()
        # the persisted counter is behind the actual counter
        self.save()
      end
    end
    return next
  end
  
  #############################################################
  # Called before removal
  def log_new_fabric()
    import string
    tasmota.log(string.format("MTR: +Fabric    fab='%s'", self.get_fabric_id().copy().reverse().tohex()), 2)
  end

  #############################################################
  # Called before removal
  def before_remove()
    import string
    tasmota.log(string.format("MTR: -Fabric    fab='%s' (removed)", self.get_fabric_id().copy().reverse().tohex()), 2)
  end

  #############################################################
  # Operational Group Key Derivation, 4.15.2, p.182
  def get_ipk_group_key()
    if self.ipk_epoch_key == nil || self.fabric_compressed == nil   return nil end
    import crypto
    var hk = crypto.HKDF_SHA256()
    var info = bytes().fromstring(self._GROUP_KEY)
    var hash = hk.derive(self.ipk_epoch_key, self.fabric_compressed, info, 16)
    return hash
  end

  def get_ca_pub()
    var ca = self.root_ca_certificate
    if ca
      var m = matter.TLV.parse(ca)
      return m.findsubval(9)
    end
  end

  #############################################################
  # add session to list of persisted sessions
  # check for duplicates
  def add_session(s)
    if self._sessions.find(s) == nil
      while size(self._sessions) >= self._MAX_CASE
        self._sessions.remove(self._sessions.find(self.get_oldest_session()))
      end
      self._sessions.push(s)
    end
  end

  def get_oldest_session()  return self.get_old_recent_session(true)  end
  def get_newest_session()  return self.get_old_recent_session(false)  end

  # get the oldest or most recent session (oldest indicates direction)
  def get_old_recent_session(oldest)
    if size(self._sessions) == 0    return nil    end
    var session = self._sessions[0]
    var timestamp = session.last_used

    var idx = 1
    while idx < size(self._sessions)
      var time2 = self._sessions[idx].last_used
      if (oldest ? time2 < timestamp : time2 > timestamp)
        session = self._sessions[idx]
        timestamp = time2
      end
      idx += 1
    end
    return session
  end

  #############################################################
  # Fabric::tojson()
  #
  # convert a single entry as json
  # returns a JSON string
  #############################################################
  def tojson()
    import json
    import string
    import introspect

    self.persist_pre()
    var keys = []
    for k : introspect.members(self)
      var v = introspect.get(self, k)
      if type(v) != 'function' && k[0] != '_'   keys.push(k) end
    end
    keys = matter.sort(keys)

    var r = []
    for k : keys
      var v = introspect.get(self, k)
      if v == nil     continue end
      if  isinstance(v, bytes)      v = "$$" + v.tob64() end    # bytes
      r.push(string.format("%s:%s", json.dump(str(k)), json.dump(v)))
    end

    # add sessions
    var s = []
    for sess : self._sessions.persistables()
      s.push(sess.tojson())
    end
    if size(s) > 0
      var s_list = "[" + s.concat(",") + "]"
      r.push('"_sessions":' + s_list)
    end

    self.persist_post()
    return "{" + r.concat(",") + "}"
  end

  #############################################################
  # fromjson()
  #
  # reads a map and load arguments
  # returns an new instance of fabric
  # don't load embedded session, this is done by store
  # i.e. ignore any key starting with '_'
  #############################################################
  static def fromjson(store, values)
    import string
    import introspect
    var self = matter.Fabric(store)

    for k : values.keys()
      if k[0] == '_'    continue  end   # ignore if key starts with '_'
      var v = values[k]
      # standard values
      if type(v) == 'string'
        if string.find(v, "0x") == 0  # treat as bytes
          introspect.set(self, k, bytes().fromhex(v[2..]))
        elif string.find(v, "$$") == 0  # treat as bytes
          introspect.set(self, k, bytes().fromb64(v[2..]))
        else
          introspect.set(self, k, v)
        end
      else
        introspect.set(self, k, v)
      end
    end
    self.hydrate_post()
    return self
  end

end
matter.Fabric = Matter_Fabric

#################################################################################
# Matter_Session class
#
# Session is refered by remote_session_id once a session is established
# It can also be retrived by `source_node_id` when `local_session_id` is 0
#
# By convention, names starting with `_` are not persisted
#################################################################################
class Matter_Session : Matter_Expirable
  static var _PASE = 1           # PASE authentication in progress
  static var _CASE = 2           # CASE authentication in progress
  static var _COUNTER_SND_INCR = 1024  # counter increased when persisting
  var _store                     # reference back to session store
  # mode for Session. Can be PASE=1, CASE=2, Established=10 none=0
  var mode
  # link to a fabric object, temporary and in construction for PASE, persistent for CASE
  var _fabric
  # sesions
  var local_session_id            # id for the current local session, starts with 1
  var initiator_session_id        # id used to respond to the initiator
  var created                     # timestamp (UTC) when the session was created
  var last_used                   # timestamp (UTC) when the session was last used
  var _source_node_id             # source node if bytes(8) (opt, used only when session is not established)
  # session_ids when the session will be active
  var __future_initiator_session_id
  var __future_local_session_id
  # counters
  var counter_rcv                 # counter for incoming messages
  var counter_snd                 # persisted last highest known counter_snd (it is in advance or equal to the actual last used counter_snd)
  var _counter_rcv_impl           # implementation of counter_rcv by matter.Counter()
  var _counter_snd_impl           # implementation of counter_snd by matter.Counter()
  var _exchange_id                # exchange id for locally initiated transaction, non-persistent
  # keep track of last known IP/Port of the fabric
  var _ip                         # IP of the last received packet
  var _port                       # port of the last received packet
  var _message_handler            # pointer to the message handler for this session
  # non-session counters
  var _counter_insecure_rcv      # counter for incoming messages
  var _counter_insecure_snd      # counter for outgoing messages
  # encryption keys and challenges
  var i2rkey                      # key initiator to receiver (incoming)
  var r2ikey                      # key receiver to initiator (outgoing)
  var _i2r_privacy                # cache for the i2r privacy key
  var attestation_challenge       # Attestation challenge
  var peer_node_id
  # breadcrumb
  var _breadcrumb                 # breadcrumb attribute for this session, prefix `__` so that it is not persisted and untouched
  # CASE
  var resumption_id               # bytes(16)
  var shared_secret               # ECDH shared secret used in CASE
  # Previous CASE messages for Transcript hash
  var __Msg1, __Msg2

  # below are placeholders for ongoing transactions or chunked responses
  var __chunked_attr_reports      # if not `nil` holds a container for the current _chuked_attr_reports

  # Group Key Derivation
  static var _GROUP_KEY = "GroupKey v1.0"  # starting with double `_` means it's not writable

  #############################################################
  def init(store, local_session_id, initiator_session_id, fabric)
    import crypto
    self._store = store
    self.mode = 0
    self.local_session_id = local_session_id
    self.initiator_session_id = initiator_session_id
    # self.counter_rcv = matter.Counter()
    # self.counter_snd = matter.Counter()
    self._counter_snd_impl = matter.Counter()
    self._counter_rcv_impl = matter.Counter()
    self.counter_rcv = 0      # avoid nil values
    self.counter_snd = self._counter_snd_impl.next() + self._COUNTER_SND_INCR
    #
    self._counter_insecure_rcv = matter.Counter()
    self._counter_insecure_snd = matter.Counter()
    self._breadcrumb = 0
    self._exchange_id = crypto.random(2).get(0,2)      # generate a random 16 bits number, then increment with rollover

    self._fabric = fabric ? fabric : self._store.create_fabric()
    self.update()
  end

  #############################################################
  # Called before removal
  def before_remove()
    import string
    tasmota.log(string.format("MTR: -Session   (%6i) (removed)", self.local_session_id), 3)
  end

  #############################################################
  # Management of security counters
  #############################################################
  # Provide the next counter value, and update the last know persisted if needed
  #
  def counter_snd_next()
    import string
    var next = self._counter_snd_impl.next()
    tasmota.log(string.format("MTR: .          Counter_snd=%i", next), 3)
    # print(">>> NEXT counter_snd=", self.counter_snd, "_impl=", self._counter_snd_impl.val(), 4)
    if matter.Counter.is_greater(next, self.counter_snd)
      self.counter_snd = next + self._COUNTER_SND_INCR
      if self.does_persist()
        # the persisted counter is behind the actual counter
        self.save()
      end
    end
    return next
  end
  # #############################################################
  # # Before savind
  # def persist_pre()
  # end

  #############################################################
  # When hydrating from persistance, update counters
  def hydrate_post()
    # reset counter_snd to highest known.
    # We advance it only in case it is actually used
    # This avoids updaing counters on dead sessions
    self._counter_snd_impl.reset(self.counter_snd)
    self._counter_rcv_impl.reset(self.counter_rcv)
    self.counter_snd = self._counter_snd_impl.val()
    self.counter_rcv = self._counter_rcv_impl.val()
  end
  #############################################################
  # Validate received counter
  def counter_rcv_validate(v, t)
    var ret = self._counter_rcv_impl.validate(v, t)
    if ret    self.counter_rcv = self._counter_rcv_impl.val()    end           # update the validated counter
    return ret
  end

  #############################################################
  # Update the timestamp or any other information
  def update()
    self.last_used = tasmota.rtc()['utc']
  end

  def set_mode_PASE()   self.set_mode(self._PASE)   end
  def set_mode_CASE()   self.set_mode(self._CASE)   end
  def is_PASE()   return self.mode == self._PASE    end
  def is_CASE()   return self.mode == self._CASE    end
  
  #############################################################
  # Assign a new fabric index
  def assign_fabric_index()
    if (self._fabric.get_fabric_index() == nil)
      self._fabric.set_fabric_index(self._store.next_fabric_idx())
    end
  end

  #############################################################
  # Register the fabric as complete (end of commissioning)
  def fabric_completed()
    self._fabric.set_no_expiration()
    self._fabric.set_persist(true)
    self.assign_fabric_index()
    self._store.add_fabric(self._fabric)
  end

  #############################################################
  # Register the frabric as complete (end of commissioning)
  def fabric_candidate()
    self._fabric.set_expire_in_seconds(120)       # expire in 2 minutes
    self.assign_fabric_index()
    self._store.add_fabric(self._fabric)
  end

  #############################################################
  # Persist to fabric
  # Add self session to the persisted established CASE session of the fabric
  def persist_to_fabric()
    self._fabric.add_session(self)
  end

  #############################################################
  # Close the current PASE/CASE session to be re-negociated
  #
  def close()
    # close the PASE session, it will be re-opened with a CASE session
    self.local_session_id = self.__future_local_session_id
    self.initiator_session_id = self.__future_initiator_session_id
    self._counter_rcv_impl.reset()
    self._counter_snd_impl.reset()
    self.counter_rcv = 0
    self.counter_snd = self._counter_snd_impl.next()
    self.i2rkey = nil
    self._i2r_privacy = nil
    self.r2ikey = nil
    self.attestation_challenge = nil
    # clear any attribute starting with `__`
    import introspect
    for k : introspect.members(self)
      var v = introspect.get(self, k)
      if type(v) != 'function' && type(v) != 'instance' && k[0] == '_' && k[1] == '_'
        self.(k) = nil
      end
    end
  end

  #############################################################
  def set_mode(mode)
    self.mode = mode
  end
  def set_keys(i2r, r2i, ac, st)
    self.i2rkey = i2r
    self._i2r_privacy = nil   # clear cache
    self.r2ikey = r2i
    self.attestation_challenge = ac
    self.created = st
  end
  def set_ca(ca)
    self._fabric.root_ca_certificate = ca
  end
  def set_noc(noc, icac)
    self._fabric.noc = noc
    self._fabric.icac = icac
  end
  def set_ipk_epoch_key(ipk_epoch_key)
    self._fabric.ipk_epoch_key = ipk_epoch_key
  end
  def set_admin_subject_vendor(admin_subject, admin_vendor)
    self._fabric.admin_subject = admin_subject
    self._fabric.admin_vendor = admin_vendor
  end

  def set_fabric_device(fabric_id, device_id, fc, fabric_parent)
    self._fabric.fabric_id = fabric_id
    self._fabric.device_id = device_id
    self._fabric.fabric_compressed = fc
    self._fabric.fabric_parent = (fabric_parent != nil) ? fabric_parent.get_fabric_index() : nil
  end
  def set_fabric_label(s)
    if type(s) == 'string'
      self._fabric.fabric_label = s
    end
  end

  #############################################################
  def get_mode()
    return self.mode
  end
  def get_i2r()
    return self.i2rkey
  end
  def get_i2r_privacy()       # get and cache privacy key
    if self._i2r_privacy == nil
      import crypto
      # compute privacy key according to p.127
      self._i2r_privacy = crypto.HKDF_SHA256().derive(self.get_i2r(), bytes(), bytes().fromstring("PrivacyKey"), 16)
    end
    return self._i2r_privacy
  end
  def get_r2i()
    return self.r2ikey
  end
  def get_ac()
    return self.attestation_challenge
  end
  def get_ca()
    return self._fabric.root_ca_certificate
  end
  def get_ca_pub()
    return self._fabric.get_ca_pub()
  end
  def get_fabric()            return self._fabric                   end
  def get_noc()               return self._fabric.noc               end
  def get_icac()              return self._fabric.icac              end
  def get_ipk_epoch_key()     return self._fabric.ipk_epoch_key     end
  def get_fabric_id()         return self._fabric.fabric_id         end
  def get_device_id()         return self._fabric.device_id         end
  def get_fabric_compressed() return self._fabric.fabric_compressed end
  def get_fabric_label()      return self._fabric.fabric_label      end
  def get_admin_subject()     return self._fabric.admin_subject     end
  def get_admin_vendor()      return self._fabric.admin_vendor      end

  #############################################################
  # Generate a private key (or retrieve it)
  def get_pk()
    if !self._fabric.no_private_key
      import crypto
      self._fabric.no_private_key = crypto.random(32)
    end
    return self._fabric.no_private_key
  end

  #############################################################
  # Operational Group Key Derivation, 4.15.2, p.182
  def get_ipk_group_key()
    if self.get_ipk_epoch_key() == nil || self.get_fabric_compressed() == nil   return nil end
    import crypto
    var hk = crypto.HKDF_SHA256()
    var info = bytes().fromstring(self._GROUP_KEY)
    var hash = hk.derive(self.get_ipk_epoch_key(), self.get_fabric_compressed(), info, 16)
    return hash
  end

  #############################################################
  # Session::to_json()
  #
  # convert a single entry as json
  # returns a JSON string
  #############################################################
  def tojson()
    import json
    import string
    import introspect

    self.persist_pre()
    var keys = []
    for k : introspect.members(self)
      var v = introspect.get(self, k)
      if type(v) != 'function' && k[0] != '_'   keys.push(k) end
    end
    keys = matter.sort(keys)

    var r = []
    for k : keys
      var v = introspect.get(self, k)
      if v == nil     continue end

      if   isinstance(v, bytes)     v = "$$" + v.tob64()        # bytes
      elif type(v) == 'instance'    continue                    # skip any other instance
      end
      
      r.push(string.format("%s:%s", json.dump(str(k)), json.dump(v)))
    end
    self.persist_post()
    return "{" + r.concat(",") + "}"
  end

  #############################################################
  # Session::fromjson()
  #
  # reads a map and load arguments
  # returns an new instance of session
  #############################################################
  static def fromjson(store, values, fabric)
    import string
    import introspect
    var self = matter.Session(store, nil, nil, fabric)

    for k:values.keys()
      var v = values[k]
      # standard values
      if type(v) == 'string'
        if string.find(v, "0x") == 0  # treat as bytes
          introspect.set(self, k, bytes().fromhex(v[2..]))
        elif string.find(v, "$$") == 0  # treat as bytes
          introspect.set(self, k, bytes().fromb64(v[2..]))
        else
          introspect.set(self, k, v)
        end
      else
        introspect.set(self, k, v)
      end
    end
    self.hydrate_post()
    return self
  end

  #############################################################
  # Callback to Session store
  def save()
    self._store.save_fabrics()
  end

  #############################################################
  def gen_CSR()
    # Create CSR
    # See 6.4.7. Node Operational Certificate Signing Request (NOCSR) p.302
    var pk = self.get_pk()                   # private key of device

    # Example
    # 3081CA
    #   3070020100300E310C300A060355040A0C034353523059301306072A8648CE3D020106082A8648CE3D030107
    #     PubKey=034200.043AEFB8D1F25813BE355920577971814827B24F2784A729297F88FBD998A59D29D439604678C42D2FA5DE4E9402C30376015E05DF0AD4A8737DCD4E6D03B11CF5
    #     A000
    #   300C06082A8648CE3D0403020500
    #   034800
    #     ECDSA=3045022054907C4F096B30EFEB56190E0F2AFAEE447991C927003185AD044D1A971BDEDD02210088FB7E44116FBD7DE5277890A3F3BC26ACC35441DF04FD0BBF02A369E751241D

    import crypto
    var ec256 = crypto.EC_P256()
    var pub = ec256.public_key(pk)

    var seg1 = bytes("3070020100300E310C300A060355040A0C034353523059301306072A8648CE3D020106082A8648CE3D030107034200")
    seg1 .. pub
    seg1 .. bytes("A000")
    var seg2 = bytes("300C06082A8648CE3D0403020500")    # not mutable
    # compute signature in ECDSA format
    var sig = ec256.ecdsa_sign_sha256_asn1(pk, seg1)
    var seg3 = bytes(128)
    seg3.add(0x03, 1)
    seg3.add(size(sig)+1, 1)
    seg3.add(0x00, 1)
    seg3 .. sig
    # construct the global csr
    var seg_123_size = size(seg1) + size(seg2) + size(seg3)
    var csr = bytes(208)
    csr.add(0x30, 1)
    csr.add(0x81, 1)
    csr.add(seg_123_size & 0xFF, 1)
    csr .. seg1
    csr .. seg2
    csr .. seg3
    return csr
  end

end
matter.Session = Matter_Session


#################################################################################
#################################################################################
#################################################################################
# Matter_Session_Store class
#################################################################################
#################################################################################
#################################################################################
class Matter_Session_Store
  var sessions
  var fabrics                     # list of provisioned fabrics
  static var _FABRICS  = "_matter_fabrics.json"

  #############################################################
  def init()
    self.sessions = matter.Expirable_list()
    self.fabrics = matter.Expirable_list()
  end

  #############################################################
  # add provisioned fabric
  def add_fabric(fabric)
    if !isinstance(fabric, matter.Fabric)   raise "value_error", "must be of class matter.Fabric" end
    if self.fabrics.find(fabric) == nil
      self.remove_redundant_fabric(fabric)
      self.fabrics.push(fabric)
    end
  end

  #############################################################
  # remove fabric
  def remove_fabric(fabric)
    var idx = 0
    while idx < size(self.sessions)
      if self.sessions[idx]._fabric == fabric
        self.sessions.remove(idx)
      else
        idx += 1
      end
    end
    self.fabrics.remove(self.fabrics.find(fabric))     # fail safe
  end

  #############################################################
  # Remove redudant fabric
  #
  # remove all other fabrics that have the same:
  #  fabric_id / device_id
  def remove_redundant_fabric(f)
    var i = 0
    while i < size(self.fabrics)
      var fabric = self.fabrics[i]
      if fabric != f && fabric.fabric_id == f.fabric_id && fabric.device_id == f.device_id
        self.fabrics.remove(i)
      else
        i += 1
      end
    end
  end

  #############################################################
  # Returns an iterator on active fabrics
  def active_fabrics()
    self.remove_expired()      # clean before
    return self.fabrics.persistables()
  end

  #############################################################
  # Count active fabrics
  #
  # Count the number of commissionned fabrics, i.e. persisted
  def count_active_fabrics()
    self.remove_expired()      # clean before
    return self.fabrics.count_persistables()
  end

  #############################################################
  # Find fabric by index number
  #
  def find_fabric_by_index(fabric_index)
    for fab : self.active_fabrics()
      if fab.get_fabric_index() == fabric_index
        return fab
      end
    end
    return nil
  end

  #############################################################
  # Find children fabrics
  #
  # Find all children fabrics recursively and collate in array
  # includes the parent fabric as first element
  #
  # Ex:
  # matter_device.sessions.fabrics[1].fabric_parent = 1
  # matter_device.sessions.find_children_fabrics(1)
  # 
  def find_children_fabrics(parent_index)
    if parent_index == nil  return []   end
    var ret = [ parent_index ]

    def find_children_fabrics_inner(index)
      for fab: self.active_fabrics()
        if fab.fabric_parent == index
          # protect against infinite loops
          if ret.find() == nil
            var sub_index = fab.fabric_index
            ret.push(sub_index)
            find_children_fabrics_inner(sub_index)
          end
        end
      end
    end

    find_children_fabrics_inner(parent_index)

    # ret contains a list of indices
    return ret
  end

  #############################################################
  # Next fabric-idx
  #
  # starts at `1`, computes the next available fabric-idx
  def next_fabric_idx()
    self.remove_expired()      # clean before
    var next_idx = 1
    for fab: self.active_fabrics()
      var fab_idx = fab.fabric_index
      if type(fab_idx) == 'int' && fab_idx >= next_idx
        next_idx = fab_idx + 1
      end
    end
    return next_idx
  end

  #############################################################
  # add session 
  def create_session(local_session_id, initiator_session_id)
    var session = self.get_session_by_local_session_id(local_session_id)
    if session != nil     self.remove_session(session) end
    session = matter.Session(self, local_session_id, initiator_session_id)
    self.sessions.push(session)
    return session
  end

  #############################################################
  # add session 
  def add_session(s, expires_in_seconds)
    if expires_in_seconds != nil
      s.set_expire_in_seconds(expires_in_seconds)
    end
    self.sessions.push(s)
  end

  #############################################################
  def get_session_by_local_session_id(id)
    if id == nil  return nil end
    var sz = size(self.sessions)
    var i = 0
    var sessions = self.sessions
    while i < sz
      var session = sessions[i]
      if session.local_session_id == id
        session.update()
        return session
      end
      i += 1
    end
  end

  #############################################################
  def get_session_by_source_node_id(nodeid)
    if nodeid == nil  return nil end
    var sz = size(self.sessions)
    var i = 0
    var sessions = self.sessions
    while i < sz
      var session = sessions[i]
      if session._source_node_id == nodeid
        session.update()
        return session
      end
      i += 1
    end
  end

  #############################################################
  # Remove session by reference
  #
  def remove_session(s)
    var i = 0
    var sessions = self.sessions
    while i < size(self.sessions)
      if sessions[i] == s
        sessions.remove(i)
      else
        i += 1
      end
    end
  end

  #############################################################
  # Generate a new local_session_id
  def gen_local_session_id()
    import crypto
    while true
      var candidate_local_session_id = crypto.random(2).get(0, 2)

      if self.get_session_by_local_session_id(candidate_local_session_id) == nil
        return candidate_local_session_id
      end

    end
  end

  #############################################################
  # remove_expired
  #
  def remove_expired()
    self.sessions.every_second()
    self.fabrics.every_second()
  end

  #############################################################
  # call remove_expired every second
  #
  def every_second()
    self.remove_expired()
  end

  #############################################################
  # find or create a session for unencrypted traffic
  # expires in `expire` seconds
  def find_session_source_id_unsecure(source_node_id, expire)
    var session = self.get_session_by_source_node_id(source_node_id)
    if session == nil
      session = matter.Session(self, 0, 0)
      session._source_node_id = source_node_id
      self.sessions.push(session)
      session.set_expire_in_seconds(expire)
    end
    session.update()
    return session
  end

  #############################################################
  # find session by resumption id
  def find_session_by_resumption_id(resumption_id)
    import string
    if !resumption_id  return nil end
    var i = 0
    var sessions = self.sessions
    while i < size(sessions)
      var session = sessions[i]
      tasmota.log(string.format("MTR: session.resumption_id=%s vs %s", str(session.resumption_id), str(resumption_id)))
      if session.resumption_id == resumption_id && session.shared_secret != nil
        tasmota.log(string.format("MTR: session.shared_secret=%s", str(session.shared_secret)))
        session.update()
        return session
      end
      i += 1
    end
  end

  #############################################################
  # list of sessions that are active, i.e. have been
  # successfully commissioned
  #
  def sessions_active()
    var ret = []
    var idx = 0
    while idx < size(self.sessions)
      var session = self.sessions[idx]
      if session.get_device_id() && session.get_fabric_id()
        ret.push(session)
      end
      idx += 1
    end
    return ret
  end

  #############################################################
  def save_fabrics()
    import json
    self.remove_expired()      # clean before saving
    var sessions_saved = 0

    var fabs = []
    for f : self.fabrics.persistables()
      for _ : f._sessions.persistables()    sessions_saved += 1   end   # count persitable sessions
      fabs.push(f.tojson())
    end
    var fabs_size = size(fabs)
    fabs = "[" + fabs.concat(",") + "]"

    try
      import string

      var f = open(self._FABRICS, "w")
      f.write(fabs)
      f.close()
      tasmota.log(string.format("MTR: =Saved     %i fabric(s) and %i session(s)", fabs_size, sessions_saved), 2)
    except .. as e, m
      tasmota.log("MTR: Session_Store::save Exception:" + str(e) + "|" + str(m), 2)
    end
  end

  #############################################################
  # load fabrics and associated sessions
  def load_fabrics()
    import string
    try
      self.sessions = matter.Expirable_list()        # remove any left-over
      self.fabrics = matter.Expirable_list()         # remove any left-over
      var f = open(self._FABRICS)
      var file_content = f.read()
      f.close()

      import json
      var file_json = json.load(file_content)
      file_content = nil
      tasmota.gc()      # clean-up a potential long string

      for v : file_json         # iterate on values
        # read fabric
        var fabric = matter.Fabric.fromjson(self, v)
        fabric.set_no_expiration()
        fabric.set_persist(true)

        # iterate on sessions
        var sessions_json = v.find("_sessions", [])

        for sess_json : sessions_json
          var session = matter.Session.fromjson(self, sess_json, fabric)
          if session != nil
            session.set_no_expiration()
            session.set_persist(true)
            self.add_session(session)
            fabric.add_session(session)
          end
        end

        self.fabrics.push(fabric)
      end

      tasmota.log(string.format("MTR: Loaded %i fabric(s)", size(self.fabrics)), 2)
    except .. as e, m
      if e != "io_error"
        tasmota.log("MTR: Session_Store::load Exception:" + str(e) + "|" + str(m), 2)
      end
    end
    # persistables are normally not expiring
    # if self.remove_expired()      # clean after load
    #   self.save_fabrics()
    # end
  end

  #############################################################
  def create_fabric()
    var fabric = matter.Fabric(self)
    return fabric
  end
end
matter.Session_Store = Matter_Session_Store

#-

# Unit test
var s = Matter_Session(1,2)
s.counter_rcv.validate(0x100, false)
s.counter_snd.validate(0x1000, false)
s.source_node_id = bytes("1122334455667788")
assert(s.tojson() == '{"breadcrumb":"0","counter_rcv":256,"counter_snd":4352,"initiator_session_id":2,"local_session_id":1,"source_node_id":"0x1122334455667788"}')

var ss = Matter_Session_Store()
ss.add_session(s)
var j = ss.save()
assert(j == '{"1":{"breadcrumb":"0","counter_rcv":256,"counter_snd":4352,"initiator_session_id":2,"local_session_id":1,"source_node_id":"0x1122334455667788"}}')


var ss = Matter_Session_Store()
ss.load()

-#
