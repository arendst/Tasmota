#
# Matter_IM_Subscription.be - suppport for Matter Interaction Model subscriptions
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

#@ solidify:Matter_IM_Subscription,weak
#@ solidify:Matter_IM_Subscription_Shop,weak

#################################################################################
# Matter_IM_Subscription
#
# Individual subscription instance
#################################################################################
class Matter_IM_Subscription
  static var MAX_INTERVAL_MARGIN = 5              # we always keep 5s margin
  var subs_shop                                   # pointer to sub shop
  # parameters of the subscription
  var subscription_id                             # id of the subcription as known by requester
  var session                                     # the session it belongs to
  var path_list                                   # list of path subscibed to
  var min_interval                                # never send data more often than every `min_interval` seconds
  var max_interval                                # always send data before `max_interal` seconds or the subscription is lost
  var fabric_filtered
  # manage time
  var not_before                                  # rate-limiting
  var expiration                                  # expiration epoch, we need to respond before
  var wait_status                                 # if `true` wait for Status Response before sending anything new
  var is_keep_alive                               # was the last message sent an empty keep-alive
  # updates
  var updates

  # req: SubscribeRequestMessage
  def init(subs_shop, id, session, req)
    self.subs_shop = subs_shop
    self.subscription_id = id
    self.session = session
    # check values for min_interval
    var min_interval = req.min_interval_floor
    if min_interval < 0                     min_interval = 0 end
    if min_interval > 60                    min_interval = 60 end
    self.min_interval = min_interval
    # check values for max_interval
    var max_interval = req.max_interval_ceiling
    if max_interval < 60                    max_interval = 60 end
    if max_interval > 3600                  max_interval = 3600 end
    max_interval = 60
    self.max_interval = max_interval
    self.wait_status = false
    
    self.fabric_filtered = req.fabric_filtered

    # get list of path from 
    self.path_list = []

    for q: req.attributes_requests
      var ctx = matter.Path()
      ctx.endpoint = q.endpoint
      ctx.cluster = q.cluster
      ctx.attribute = q.attribute
      self.path_list.push(ctx)
    end
    
    # update next time interval
    self.updates = []
    self.clear_before_arm()
    self.is_keep_alive = false

    # tasmota.log("MTR: new subsctiption " + matter.inspect(self), 3)
  end
  
  # remove self from subs_shop list
  def remove_self()
    tasmota.log("MTR: -Sub_Del   (      ) sub=" + str(self.subscription_id), 3)
    self.subs_shop.remove_sub(self)
  end

  # clear log after it was sent, and re-arm next expiration
  def clear_before_arm()
    self.updates.clear()
    self.wait_status = true
  end

  # we received a complete ack for previous message, rearm
  def re_arm()
    self.wait_status = false
    var now = tasmota.millis()
    self.expiration = now + (self.max_interval - self.MAX_INTERVAL_MARGIN) * 1000
    self.not_before = now + self.min_interval * 1000 - 1
    if !self.is_keep_alive
      tasmota.log(format("MTR: .Sub_Done  (      ) sub=%i", self.subscription_id), 3)
    end
  end

  # signal that an attribute was updated, to add to the list of reportable
  def attribute_updated_ctx(ctx, fabric_specific)
    var idx = 0
    while idx < size(self.path_list)
      var filter = self.path_list[idx]
      if (filter.endpoint == nil  || filter.endpoint  == ctx.endpoint) &&
         (filter.cluster == nil   || filter.cluster   == ctx.cluster)  &&
         (filter.attribute == nil || filter.attribute == ctx.attribute)

         # ready to push the new attribute, check that it 
         self._add_attribute_unique_path(ctx)
      end
      idx += 1
    end
  end

  # add an attribute path for an updated attribute, remove any duplicate
  def _add_attribute_unique_path(ctx)
    var idx = 0
    while idx < size(self.updates)
      var path = self.updates[idx]
      if   path.endpoint  == ctx.endpoint  &&
           path.cluster   == ctx.cluster   &&
           path.attribute == ctx.attribute
        return      # already exists in the list, abort
      end
      idx += 1
    end
    self.updates.push(ctx)
  end

end
matter.IM_Subscription = Matter_IM_Subscription

#################################################################################
# Matter_IM_Subscription_Shop (monad)
#
# Handles all subscriptions
#################################################################################
class Matter_IM_Subscription_Shop
  var subs                                      # list of subscriptions
  var im                                        # pointer to parent `im` object

  def init(im)
    self.im = im
    self.subs = []
  end

  #############################################################
  # create a new subscription
  #
  # session object
  # SubscribeRequestMessage request
  # returns: new subscription
  def new_subscription(session, req)
    import crypto
    var id = crypto.random(2).get(0,2)
    while self.get_by_id(id)
      id = crypto.random(2).get(0,2)
    end

    var sub = matter.IM_Subscription(self, id, session, req)
    self.subs.push(sub)

    return sub
  end

  def remove_sub(sub)
    var idx = 0
    while idx < size(self.subs)
      if self.subs[idx] == sub
        self.subs.remove(idx)
      else
        idx += 1
      end
    end
  end

  def get_by_id(id)
    var idx = 0
    while idx < size(self.subs)
      if self.subs[idx].subscription_id == id
        return self.subs[idx]
      end
      idx += 1
    end
  end

  def remove_by_session(session)
    var idx = 0
    while idx < size(self.subs)
      if self.subs[idx].session == session
        self.subs.remove(idx)
      else
        idx += 1
      end
    end
  end

  def remove_by_fabric(fabric)
    for session: fabric._sessions
      self.remove_by_session(session)
    end
  end

  #############################################################
  # dispatch every 250ms click to sub-objects that need it
  def every_250ms()
    # any data ready to send?
    var idx = 0
    while idx < size(self.subs)
      var sub = self.subs[idx]
      if !sub.wait_status && size(sub.updates) > 0 && tasmota.time_reached(sub.not_before)
        self.im.send_subscribe_update(sub)
        sub.clear_before_arm()
      end
      idx += 1
    end

    # any heartbeat needing to be sent
    idx = 0
    while idx < size(self.subs)
      var sub = self.subs[idx]
      if !sub.wait_status && tasmota.time_reached(sub.expiration)
        self.im.send_subscribe_heartbeat(sub)
        sub.clear_before_arm()
        sub.re_arm()                 # signal that we can proceed to next sub report
      end
      idx += 1
    end
  end

  # signal that an attribute was updated, to add to the list of reportable
  def attribute_updated_ctx(ctx, fabric_specific)
    # signal any relevant subscription
    var idx = 0
    while idx < size(self.subs)
      self.subs[idx].attribute_updated_ctx(ctx, fabric_specific)
      idx += 1
    end
  end
  
end
matter.IM_Subscription_Shop = Matter_IM_Subscription_Shop
