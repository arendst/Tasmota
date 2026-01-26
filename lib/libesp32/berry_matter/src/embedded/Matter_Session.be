#
# Matter_Session.be - Support for Matter Sessions
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

# for compilation
class Matter_Expirable end

#################################################################################
# Matter_Session class
#
# Session is refered by remote_session_id once a session is established
# It can also be retrived by `source_node_id` when `local_session_id` is 0
#
# By convention, names starting with `_` are not persisted
# Names starting with `__` are cleared when session is closed (transition from PASE to CASE or CASE finished)
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
  # temporary data for ArmFailSafe provisioning - before it is stored in a fabric
  var _temp_root_ca_certificate   # temporary root_ca_certificate added by `AddTrustedRootCertificate` before `AddNoc`
  var _temp_pk
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
  var _ip                         # IP of the last received packet (string)
  var _port                       # port of the last received packet (int)
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
  var _breadcrumb                 # breadcrumb attribute for this session, prefix `_` so that it is not persisted and untouched
  # CASE
  var resumption_id               # bytes(16)
  var shared_secret               # ECDH shared secret used in CASE
  var __responder_priv, __responder_pub
  var __initiator_pub
  # PASE
  var __spake_cA                  # crypto.SPAKE2P_Matter object, cA
  var __spake_Ke                  # crypto.SPAKE2P_Matter object, Ke
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

    self._fabric = fabric
    self.update()
  end

  #############################################################
  # Called before removal
  def before_remove()
    log(format("MTR: -Session   (%6i) (removed)", self.local_session_id), 3)
  end

  #############################################################
  # Management of security counters
  #############################################################
  # Provide the next counter value, and update the last know persisted if needed
  #
  def counter_snd_next()
    var next = self._counter_snd_impl.next()
    # log(format("MTR: .          Counter_snd=%i", next), 4)
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
    self.last_used = tasmota.rtc_utc()
  end

  def set_mode_PASE()   self.set_mode(self._PASE)   end
  def set_mode_CASE()   self.set_mode(self._CASE)   end
  def is_PASE()   return self.mode == self._PASE    end
  def is_CASE()   return self.mode == self._CASE    end

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
  def set_temp_ca(ca)
    self._temp_root_ca_certificate = ca
  end
  def get_temp_ca()           return self._temp_root_ca_certificate end
  def get_temp_ca_pub()
    var ca = self._temp_root_ca_certificate
    if ca
      var m = matter.TLV.parse(ca)
      return m.findsubval(9)
    end
  end
  def set_fabric_label(s)
    if type(s) == 'string'
      self._fabric.fabric_label = s
    end
  end

  #############################################################
  def get_mode()              return self.mode                    end
  def get_i2r()               return self.i2rkey                  end
  def get_i2r_privacy()       # get and cache privacy key
    if self._i2r_privacy == nil
      import crypto
      # compute privacy key according to p.127
      self._i2r_privacy = crypto.HKDF_SHA256().derive(self.get_i2r(), bytes(), bytes().fromstring("PrivacyKey"), 16)
    end
    return self._i2r_privacy
  end
  def get_r2i()               return self.r2ikey                   end
  def get_ac()                return self.attestation_challenge     end

  def get_ca()                return self._fabric.root_ca_certificate end
  def get_ca_pub()            return self._fabric.get_ca_pub()      end
  def get_fabric()            return self._fabric                   end
  def get_noc()               return self._fabric.noc               end
  def get_icac()              return self._fabric.icac              end
  def get_ipk_epoch_key()     return self._fabric.ipk_epoch_key     end
  def get_fabric_id()         return self._fabric.fabric_id         end
  def get_fabric_index()      return self._fabric ? self._fabric.fabric_index  : nil    end
  def get_device_id()         return self._fabric ? self._fabric.device_id : nil        end
  def get_fabric_compressed() return self._fabric ? self._fabric.fabric_compressed : nil end
  def get_fabric_label()      return self._fabric ? self._fabric.fabric_label : nil     end
  def get_admin_subject()     return self._fabric ? self._fabric.admin_subject : nil    end
  def get_admin_vendor()      return self._fabric ? self._fabric.admin_vendor : nil     end
  def get_node_id()           return self._fabric ? self._fabric.device_id : nil        end

  #############################################################
  # Get operational key pair (private key)
  #
  # If there is a fabric, retrieve the PK from the fabric
  # If there is no fabric, create a temporary PK in `_temp_pk`
  # to be stored later in the fabric
  def get_pk()
    if self._fabric
      return self._fabric.get_pk()
    else
      if !self._temp_pk
        import crypto
        self._temp_pk = crypto.random(32)
      end
      return self._temp_pk
    end
  end

  #############################################################
  # Operational Group Key Derivation, 4.15.2, p.182
  def get_ipk_group_key()
    return self._fabric.get_ipk_group_key()
  end

  #############################################################
  # Session::tojson()
  #
  # convert a single entry as json
  # returns a JSON string
  #############################################################
  def tojson()
    import json
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
      
      r.push(format("%s:%s", json.dump(str(k)), json.dump(v)))
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
