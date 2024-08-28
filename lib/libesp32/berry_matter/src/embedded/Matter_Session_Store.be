#
# Matter_Session_Store.be - Support for Matter Session Store
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

#@ solidify:Matter_Session_Store,weak

#################################################################################
#################################################################################
#################################################################################
# Matter_Session_Store class
#################################################################################
#################################################################################
#################################################################################
class Matter_Session_Store
  var device                      # device root object
  var sessions
  var fabrics                     # list of provisioned fabrics
  static var _FABRICS  = "/_matter_fabrics.json"
  static var _FABRICS_TEMP  = "/_matter_fabrics.tmp"   # temporary saved file before renaming to _FABRICS

  #############################################################
  def init(device)
    self.device = device
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
    if (self.sessions != nil)
      var idx = 0
      while idx < size(self.sessions)
        if self.sessions[idx]._fabric == fabric
          self.sessions.remove(idx)
        else
          idx += 1
        end
      end
    end
    if (self.fabrics != nil)
      self.fabrics.remove(self.fabrics.find(fabric))     # fail safe
    end
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
    if self.fabrics != nil
      return self.fabrics.count_persistables()
    else
      return 0
    end
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
    if !resumption_id  return nil end
    var i = 0
    var sessions = self.sessions
    while i < size(sessions)
      var session = sessions[i]
      log(format("MTR: session.resumption_id=%s vs %s", str(session.resumption_id), str(resumption_id)), 4)
      if session.resumption_id == resumption_id && session.shared_secret != nil
        # log(format("MTR: session.shared_secret=%s", str(session.shared_secret)), 4)
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
    import path
    try
      self.remove_expired()      # clean before saving
      var sessions_saved = 0
      var fabrics_saved = 0

      var f = open(self._FABRICS_TEMP, "w")

      f.write("[")
      for fab : self.fabrics.persistables()
        for _ : fab._sessions.persistables()    sessions_saved += 1   end   # count persitable sessions
        if fabrics_saved > 0
          f.write(",")
        end
        fab.writejson(f)
        fabrics_saved += 1
      end
      f.write("]")

      f.close()
      # saving went well, now remove previous version and rename
      path.remove(self._FABRICS)
      if (path.rename(self._FABRICS_TEMP, self._FABRICS))
        log(f"MTR: =Saved     {fabrics_saved} fabric(s) and {sessions_saved} session(s)", 2)
        self.device.event_fabrics_saved()     # signal event
      else
        log(f"MTR: Saving Fabrics failed", 2)
      end
    except .. as e, m
      log("MTR: Session_Store::save Exception:" + str(e) + "|" + str(m), 2)
    end
  end

  #############################################################
  # load fabrics and associated sessions
  def load_fabrics()
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

      log(format("MTR: Loaded %i fabric(s)", size(self.fabrics)), 2)
    except .. as e, m
      if e != "io_error"
        log("MTR: Session_Store::load Exception:" + str(e) + "|" + str(m), 2)
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
