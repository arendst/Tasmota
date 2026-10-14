#
# Matter_Fabric.be - Support for Matter Fabric
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

# for solidification only
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
  var deleted                     # timestamp when the deletion of fabric was requested, and deferred
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
  # Fabric-scoped Group Key Management data.
  # Keep these as JSON-safe maps/lists so they are persisted with the fabric.
  var group_key_map
  var group_table
  var group_key_sets

  #############################################################
  def init(store)
    import crypto
    self._store = store
    self._sessions = matter.Expirable_list()
    self.fabric_label = ""
    self.created = tasmota.rtc_utc()
    # self.deleted = nil      # no need to actually set to nil
    # init group counters
    self._counter_group_data_snd_impl = matter.Counter()
    self._counter_group_ctrl_snd_impl  = matter.Counter()
    self.counter_group_data_snd = self._counter_group_data_snd_impl.next() + self._GROUP_SND_INCR
    self.counter_group_ctrl_snd = self._counter_group_data_snd_impl.next() + self._GROUP_SND_INCR
    self.group_key_map = []
    self.group_table = []
    self.group_key_sets = []
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

  def get_fabric_id_as_int64()
    return int64.frombytes(self.fabric_id)
  end
  def get_device_id_as_int64()
    return int64.frombytes(self.device_id)
  end

  def get_admin_vendor_name()
    var vnd = self.admin_vendor
    if vnd == nil   return ""  end
    var name = matter.get_vendor_name(vnd)
    if name != nil
      return name
    else
      return f"0x{vnd:04X}"
    end
  end

  def set_fabric_index(v)     self.fabric_index = v         end
  def set_ca(ca)
    self.root_ca_certificate = ca
  end
  def set_noc_icac(noc, icac)
    self.noc = noc
    self.icac = icac
  end
  def set_ipk_epoch_key(ipk_epoch_key)
    self.ipk_epoch_key = ipk_epoch_key
  end
  def set_admin_subject_vendor(admin_subject, admin_vendor)
    self.admin_subject = admin_subject
    self.admin_vendor = admin_vendor
  end

  def set_fabric_device(fabric_id, device_id, fc, fabric_parent)
    self.fabric_id = fabric_id
    self.device_id = device_id
    self.fabric_compressed = fc
    self.fabric_parent = (fabric_parent != nil) ? fabric_parent.get_fabric_index() : nil
  end

  #############################################################
  # Generate a private key (or retrieve it)
  #
  # PK is generated by a commissioning session
  def get_pk()
    return self.no_private_key
  end
  def set_pk(pk)
    self.no_private_key = pk
  end

  #############################################################
  # Register the frabric as complete (end of commissioning)
  def fabric_candidate()
    self.set_expire_in_seconds(120)       # expire in 2 minutes
    self.assign_fabric_index()
    self._store.add_fabric(self)
  end

  #############################################################
  # Assign a new fabric index
  def assign_fabric_index()
    if (self.get_fabric_index() == nil)
      self.set_fabric_index(self._store.next_fabric_idx())
    end
  end

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
    # Migrate fabrics persisted before group storage was introduced.
    if self.group_key_map == nil       self.group_key_map = []       end
    if self.group_table == nil         self.group_table = []         end
    if self.group_key_sets == nil      self.group_key_sets = []      end
    # Berry's JSON encoder serializes int64 values as decimal strings.
    # Restore GroupKeySet epoch fields, which are Matter uint64 values.
    for key_set : self.group_key_sets
      for time_name : ["time0", "time1", "time2"]
        var persisted_time = key_set.find(time_name)
        if type(persisted_time) == 'string'
          var restored_time = self.uint64_from_json(persisted_time)
          if restored_time != nil   key_set[time_name] = restored_time end
        end
      end
    end
    # GroupKeySetID 0 is the operational IPK and is not a valid GroupKeyMap
    # target. Remove entries accepted by older builds.
    var map_idx = size(self.group_key_map) - 1
    while map_idx >= 0
      if self.group_key_map[map_idx].find("key_set_id") == 0
        self.group_key_map.remove(map_idx)
      end
      map_idx -= 1
    end
  end

  #############################################################
  # Persist this fabric and all associated sessions.
  def save()
    if self._store != nil
      self._store.save_fabrics()
    end
  end

  # Decode the decimal representation emitted by json.dump(int64). Requiring
  # an exact round-trip prevents malformed epoch times from becoming zero.
  static def uint64_from_json(value)
    if isinstance(value, int64)   return value end
    if type(value) == 'int'       return int64.fromu32(value) end
    if type(value) != 'string'    return nil end
    try
      var restored = int64.fromstring(value)
      return restored.tostring() == value ? restored : nil
    except ..
      # A damaged or hand-edited epoch value must not abort fabric loading.
      return nil
    end
  end

  # Expose GroupID-to-key-set associations for GKM reads and validation.
  def get_group_key_map()       return self.group_key_map       end
  # Expose persisted endpoint memberships used by Groups and GKM GroupTable.
  def get_group_table()         return self.group_table         end
  # Expose persisted key-set records to the mandatory GKM command handlers.
  def get_group_key_sets()      return self.group_key_sets      end

  # Find one fabric-scoped Groups record by GroupID for membership commands.
  def find_group(group_id)
    for entry : self.group_table
      if entry.find("group_id") == group_id
        return entry
      end
    end
    return nil
  end

  # Atomically replace and persist GroupKeyMap after a validated list write.
  def replace_group_key_map(entries)
    self.group_key_map = entries
    self.save()
  end

  #############################################################
  # Group Key Management helpers.
  # Locate a stored key set without exposing or copying its epoch key material.
  def find_group_key_set(id)
    for key_set : self.group_key_sets
      if key_set.find("id") == id
        return key_set
      end
    end
    return nil
  end

  # Insert or replace a key set by ID and persist the fabric immediately.
  # KeySetWrite calls this only after the Root plugin validates the full set.
  def put_group_key_set(key_set)
    var id = key_set.find("id")
    var idx = 0
    while idx < size(self.group_key_sets)
      if self.group_key_sets[idx].find("id") == id
        self.group_key_sets[idx] = key_set
        self.save()
        return
      end
      idx += 1
    end
    self.group_key_sets.push(key_set)
    self.save()
  end

  # KeySetRemove also removes every GroupKeyMap association referencing the
  # key set. Commit both changes with one fabric save.
  def remove_group_key_set_and_mappings(id)
    var found = false
    var idx = 0
    while idx < size(self.group_key_sets)
      if self.group_key_sets[idx].find("id") == id
        self.group_key_sets.remove(idx)
        found = true
        break
      end
      idx += 1
    end
    if !found   return false end
    idx = size(self.group_key_map) - 1
    while idx >= 0
      if self.group_key_map[idx].find("key_set_id") == id
        self.group_key_map.remove(idx)
      end
      idx -= 1
    end
    self.save()
    return true
  end

  #############################################################
  # Maintain the fabric's endpoint membership table for the Groups cluster.
  # Add an endpoint idempotently, creating the fabric-scoped group if needed.
  def add_group_endpoint(group_id, endpoint, name)
    var entry
    for candidate : self.group_table
      if candidate.find("group_id") == group_id
        entry = candidate
        break
      end
    end
    if entry == nil
      entry = {"group_id": group_id, "endpoints": [], "name": name ? name : ""}
      self.group_table.push(entry)
    elif name != nil
      entry["name"] = name
    end
    var endpoints = entry.find("endpoints")
    if endpoints.find(endpoint) == nil
      endpoints.push(endpoint)
    end
    self.save()
  end

  # Remove one endpoint membership and discard an empty group record. The
  # boolean result lets RemoveGroup distinguish success from NOT_FOUND.
  def remove_group_endpoint(group_id, endpoint)
    var idx = 0
    while idx < size(self.group_table)
      var entry = self.group_table[idx]
      if entry.find("group_id") == group_id
        var endpoints = entry.find("endpoints")
        var endpoint_idx = endpoints.find(endpoint)
        if endpoint_idx == nil   return false end
        endpoints.remove(endpoint_idx)
        if size(endpoints) == 0
          self.group_table.remove(idx)
        end
        self.save()
        return true
      end
      idx += 1
    end
    return false
  end

  # Remove an endpoint from every group, as required by RemoveAllGroups, and
  # persist once after all memberships have been updated.
  def remove_all_groups_for_endpoint(endpoint)
    var changed = false
    var idx = size(self.group_table) - 1
    while idx >= 0
      var endpoints = self.group_table[idx].find("endpoints")
      var endpoint_idx = endpoints.find(endpoint)
      if endpoint_idx != nil
        endpoints.remove(endpoint_idx)
        changed = true
      end
      if size(endpoints) == 0
        self.group_table.remove(idx)
      end
      idx -= 1
    end
    if changed
      self.save()
    end
  end

  #############################################################
  # Register the fabric as complete (end of commissioning)
  def fabric_completed()
    self.set_no_expiration()
    self.set_persist(true)
    self.assign_fabric_index()
    self._store.add_fabric(self)
  end

  #############################################################
  # Management of security counters
  #############################################################
  # Provide the next counter value, and update the last know persisted if needed
  #
  def counter_group_data_snd_next()
    var next = self._counter_group_data_snd_impl.next()
    log(f"MTR: .          Counter_group_data_snd={next:i}", 3)
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
    var next = self._counter_group_ctrl_snd_impl.next()
    log(f"MTR: .          Counter_group_ctrl_snd={next:i}", 3)
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
    log(format("MTR: +Fabric    fab='%s' vendorid=%s", self.get_fabric_id().copy().reverse().tohex(), self.get_admin_vendor_name()), 3)
  end

  #############################################################
  # Called before removal
  def before_remove()
    log(format("MTR: -Fabric    fab='%s' (removed)", self.get_fabric_id().copy().reverse().tohex()), 3)
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
        var session_deleted = self.get_oldest_session()
        self._sessions.remove(self._sessions.find(session_deleted))
        self._store.remove_session(session_deleted)
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
  # Mark for deleteion in the near future
  #
  def mark_for_deletion()
    self.deleted = tasmota.rtc_utc()
  end
  def is_marked_for_deletion()
    return self.deleted != nil
  end

  #############################################################
  # Fabric::tojson()
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
      if  isinstance(v, bytes)      v = "$$" + v.tob64() end    # bytes
      r.push(format("%s:%s", json.dump(str(k)), json.dump(v)))
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
  # Fabric::writejson(f)
  #
  # convert a single entry as json
  # write to file
  #############################################################
  def writejson(f)
    import json
    import introspect

    f.write("{")

    self.persist_pre()
    var keys = []
    for k : introspect.members(self)
      var v = introspect.get(self, k)
      if type(v) != 'function' && k[0] != '_'   keys.push(k) end
    end
    keys = matter.sort(keys)

    var first = true
    for k : keys
      var v = introspect.get(self, k)
      if v == nil     continue end
      if  isinstance(v, bytes)      v = "$$" + v.tob64() end    # bytes
      if (!first) f.write(",")  end
      f.write(format("%s:%s", json.dump(str(k)), json.dump(v)))
      first = false
    end

    # add sessions
    var first_session = true
    for sess : self._sessions.persistables()
      if first_session
        f.write(',"_sessions":[')
      else
        f.write(",")
      end
      f.write(sess.tojson())
      first_session = false
    end
    if !first_session
      f.write("]")
    end

    self.persist_post()
    f.write("}")
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
