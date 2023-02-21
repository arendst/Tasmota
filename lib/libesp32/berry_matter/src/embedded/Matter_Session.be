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

#@ solidify:Matter_Session,weak
#@ solidify:Matter_Session_Store,weak

#################################################################################
# Matter_Session class
#
# Session is refered by remote_session_id once a session is established
# It can also be retrived by `source_node_id` when `local_session_id` is 0
#
# By convention, names starting with `_` are not persisted
#################################################################################
class Matter_Session
  static var __PASE = 1           # PASE authentication in progress
  static var __CASE = 2           # CASE authentication in progress
  var __store                     # reference back to session store
  # mode for Session. Can be PASE=1, CASE=2, Established=10 none=0
  var mode
  # sesions
  var local_session_id            # id for the current local session, starts with 1
  var initiator_session_id        # id used to respond to the initiator
  var session_timestamp           # timestamp (UTC) when the session was created
  var source_node_id              # source node if bytes(8) (opt, used only when session is not established)
  # session_ids when the session will be active
  var _future_initiator_session_id
  var _future_local_session_id
  # counters
  var counter_rcv                 # counter for incoming messages
  var counter_snd                 # counter for outgoing messages
  var __exchange_id               # exchange id for locally initiated transaction, non-persistent
  # keep track of last known IP/Port of the fabric
  var __ip                        # IP of the last received packet
  var __port                      # port of the last received packet
  var __message_handler           # pointer to the message handler for this session
  # non-session counters
  var _counter_insecure_rcv       # counter for incoming messages
  var _counter_insecure_snd       # counter for outgoing messages
  # encryption keys and challenges
  var i2rkey                      # key initiator to receiver (incoming)
  var r2ikey                      # key receiver to initiator (outgoing)
  var _i2r_privacy                # cache for the i2r privacy key
  var attestation_challenge        # Attestation challenge
  var peer_node_id
  # breadcrumb
  var __breadcrumb                # breadcrumb attribute for this session, prefix `__` so that it is not persisted and untouched
  # our own private key
  var no_private_key              # private key of the device certificate (generated at commissioning)
  # NOC information
  var root_ca_certificate         # root certificate of the initiator
  var noc                         # Node Operational Certificate in TLV Matter Certificate
  var icac                        # Initiator CA Certificate in TLV Matter Certificate
  var ipk_epoch_key               # timestamp
  # CASE
  var resumption_id               # bytes(16)
  var shared_secret              # ECDH shared secret used in CASE
  # Information extracted from `noc`
  var fabric                      # fabric identifier as bytes(8) little endian
  var fabric_compressed           # comrpessed fabric identifier, hashed with root_ca public key
  var deviceid                    # our own device id bytes(8) little endian
  var fabric_label                # set by UpdateFabricLabel
  # Admin info extracted from NOC/ICAC
  var admin_subject
  var admin_vendor
  # Previous CASE messages for Transcript hash
  var _Msg1, _Msg2
  # Expiration
  var _persist                     # do we persist this sessions or is it remporary
  var expiration                   # if not `nil` the entry is removed after this timestamp

  # below are placeholders for ongoing transactions or chunked responses
  var _chunked_attr_reports        # if not `nil` holds a container for the current _chuked_attr_reports

  # Group Key Derivation
  static var __GROUP_KEY = "GroupKey v1.0"  # starting with double `_` means it's not writable

  #############################################################
  def init(store, local_session_id, initiator_session_id)
    import crypto
    self.__store = store
    self.mode = 0
    self.local_session_id = local_session_id
    self.initiator_session_id = initiator_session_id
    self.counter_rcv = matter.Counter()
    self.counter_snd = matter.Counter()
    self._counter_insecure_rcv = matter.Counter()
    self._counter_insecure_snd = matter.Counter()
    self.__breadcrumb = 0
    self.__exchange_id = crypto.random(2).get(0,2)      # generate a random 16 bits number, then increment with rollover
  end

  #############################################################
  # Close the current PASE/CASE session to be re-negociated
  #
  def close()
    # close the PASE session, it will be re-opened with a CASE session
    var persist_save = self._persist
    self.local_session_id = self._future_local_session_id
    self.initiator_session_id = self._future_initiator_session_id
    self.source_node_id = nil
    self.counter_rcv.reset()
    self.counter_snd.reset()
    self.i2rkey = nil
    self._i2r_privacy = nil
    self.r2ikey = nil
    self.attestation_challenge = nil
    self.fabric_label = ""
    # clear any attribute starting with `_`
    import introspect
    for k : introspect.members(self)
      var v = introspect.get(self, k)
      if type(v) != 'function' && type(v) != 'instance' && k[0] == '_' && k[1] != '_'
        self.(k) = nil
      end
    end
    self._persist = persist_save
    # self._future_initiator_session_id = nil
    # self._future_local_session_id = nil
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
    self.session_timestamp = st
  end
  def set_ca(ca)
    self.root_ca_certificate = ca
  end
  def set_noc(noc, icac)
    self.noc = noc
    self.icac = icac
  end
  def set_ipk_epoch_key(ipk_epoch_key)
    self.ipk_epoch_key = ipk_epoch_key
  end
  def set_fabric_device(fabric, deviceid, fc)
    self.fabric = fabric
    self.deviceid = deviceid
    self.fabric_compressed = fc
    self.__store.remove_redundant_session(self)
  end
  def set_persist(p)
    self._persist = bool(p)
  end
  def set_fabric_label(s)
    if type(s) == 'string'
      self.fabric_label = s
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
    return self.root_ca_certificate
  end
  def get_ca_pub()
    if self.root_ca_certificate
      var m = matter.TLV.parse(self.root_ca_certificate)
      return m.findsubval(9)
    end
  end
  def get_noc()       return self.noc       end
  def get_icac()      return self.icac      end
  def get_ipk_epoch_key() return self.ipk_epoch_key end
  def get_fabric()    return self.fabric    end
  def get_deviceid()  return self.deviceid  end
  def get_fabric_compressed() return self.fabric_compressed end

  #############################################################
  # Generate a private key (or retrieve it)
  def get_pk()
    if !self.no_private_key
      import crypto
      self.no_private_key = crypto.random(32)
    end
    return self.no_private_key
  end

  #############################################################
  # Operational Group Key Derivation, 4.15.2, p.182
  def get_ipk_group_key()
    if self.ipk_epoch_key == nil || self.fabric_compressed == nil   return nil end
    import crypto
    var hk = crypto.HKDF_SHA256()
    var info = bytes().fromstring(self.__GROUP_KEY)
    var hash = hk.derive(self.ipk_epoch_key, self.fabric_compressed, info, 16)
    return hash
  end

  #############################################################
  # set absolute time for expiration
  def set_no_expiration()
    self.expiration = nil
  end

  #############################################################
  # set absolute time for expiration
  def set_expire_time(t)
    self.expiration = int(t)
  end

  #############################################################
  # set relative time in the future for expiration (in seconds)
  def set_expire_in_seconds(s, now)
    if s == nil  return end
    if now == nil     now = tasmota.rtc()['utc']    end
    self.set_expire_time(now + s)
  end

  #############################################################
  # set relative time in the future for expiration (in seconds)
  # returns `true` if expiration date has been reached
  def has_expired(now)
    if now == nil     now = tasmota.rtc()['utc']    end
    if self.expiration != nil
      return now >= self.expiration
    end
    return false
  end

  #############################################################
  # to_json()
  #
  # convert a single entry as json
  # returns a JSON string
  #############################################################
  def tojson()
    import json
    import string
    import introspect

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

      if   k == "counter_rcv"     v = v.val()
      elif k == "counter_snd"     v = v.val() + 256     # take a margin to avoid reusing the same counter
      end

      if  isinstance(v, bytes)      v = "$$" + v.tob64() end    # bytes
      # if  isinstance(v, bytes)      v = "0x" + v.tohex() end
      
      # if type(v) == 'string'    v = string.escape(v, true) end
      r.push(string.format("%s:%s", json.dump(str(k)), json.dump(v)))
    end
    return "{" + r.concat(",") + "}"
  end

  #############################################################
  # fromjson()
  #
  # reads a map and load arguments
  # returns an new instance of session
  #############################################################
  static def fromjson(store, values)
    import string
    import introspect
    var self = matter.Session(store)

    for k:values.keys()
      var v = values[k]
      if   k == "counter_rcv"     self.counter_rcv.reset(int(v))
      elif k == "counter_snd"     self.counter_snd.reset(int(v))
      else
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
    end

    return self
  end

  #############################################################
  # Callback to Session store
  def save()
    self.__store.save()
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
# Matter_Session_Store class
#################################################################################
class Matter_Session_Store
  var sessions
  static var FILENAME = "_matter_sessions.json"

  #############################################################
  def init()
    self.sessions = []
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
      if sessions[i].local_session_id == id     return sessions[i] end
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
      if sessions[i].source_node_id == nodeid     return sessions[i] end
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
  # Remove session by reference
  #
  # remove all other sessions that have the same:
  #  fabric / deviceid / fc
  def remove_redundant_session(s)
    var i = 0
    var sessions = self.sessions
    while i < size(self.sessions)
      var session = sessions[i]
      if session != s && session.fabric == s.fabric && session.deviceid == s.deviceid #- && session.fabric_compressed == s.fabric_compressed -#
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
  # Check is any session has expired
  def remove_expired()
    var dirty = false
    var i = 0
    var sessions = self.sessions
    while i < size(self.sessions)
      if sessions[i].has_expired()
        if sessions[i]._persist    dirty = true end      # do we need to save
        sessions.remove(i)
      else
        i += 1
      end
    end
    if dirty   self.save() end
  end
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
      session.source_node_id = source_node_id
      self.sessions.push(session)
    end
    session.set_expire_in_seconds(expire)
    return session
  end

  #############################################################
  # find session by resumption id
  def find_session_by_resumption_id(resumption_id)
    if !resumption_id  return nil end
    var i = 0
    var sessions = self.sessions
    while i < size(sessions)
      if sessions[i].resumption_id == resumption_id
        return sessions[i]
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
      if session.get_deviceid() && session.get_fabric()
        ret.push(session)
      end
      idx += 1
    end
    return ret
  end

  #############################################################
  def save()
    import json
    self.remove_expired()      # clean before saving

    var j = []
    for v:self.sessions
      if v._persist
        j.push(v.tojson())
      end
    end
    var j_size = size(j)
    j = "[" + j.concat(",") + "]"

    try
      import string
      var f = open(self.FILENAME, "w")
      f.write(j)
      f.close()
      tasmota.log(string.format("MTR: Saved %i session(s)", j_size), 2)
      return j
    except .. as e, m
      tasmota.log("MTR: Session_Store::save Exception:" + str(e) + "|" + str(m), 2)
      return j
    end
  end

  #############################################################
  def load()
    import string
    try
      self.sessions = []        # remove any left-over
      var f = open(self.FILENAME)
      var s = f.read()
      f.close()

      import json
      var j = json.load(s)
      s = nil
      tasmota.gc()      # clean-up a potential long string

      for v:j           # iterate on values
        var session = matter.Session.fromjson(self, v)
        session._persist = true
        if session != nil
          self.add_session(session)
        end
      end

      tasmota.log(string.format("MTR: Loaded %i session(s)", size(self.sessions)), 2)
    except .. as e, m
      if e != "io_error"
        tasmota.log("MTR: Session_Store::load Exception:" + str(e) + "|" + str(m), 2)
      end
    end
    self.remove_expired()      # clean after load
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
