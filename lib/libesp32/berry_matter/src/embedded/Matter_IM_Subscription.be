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
  var event_generators                            # list of EventGenerator filters for subscription
  var min_interval                                # never send data more often than every `min_interval` seconds
  var max_interval                                # always send data before `max_interal` seconds or the subscription is lost
  var fabric_filtered
  # manage time
  var not_before                                  # rate-limiting
  var expiration                                  # expiration epoch, we need to respond before
  var wait_status                                 # if `true` wait for Status Response before sending anything new
  var is_keep_alive                               # was the last message sent an empty keep-alive
  # updates
  var updates                                     # array of paths that will need to be updated at next window
  var update_event_no                             # int64 or nil, contains the first event number from which we should send an update, or nil if none

  #################################################################################
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
    var device = subs_shop.im.device              # keep a local copy of master device object

    # get list of attribute paths
    self.path_list = []
    if (req.attributes_requests != nil)
      for q: req.attributes_requests
        self.path_list.push( matter.Path(q.endpoint, q.cluster, q.attribute) )
      end
    end

    # scan event filters
    var event_no_min = matter.IM.parse_event_filters_min_no(req.event_filters, nil)

    # update next time interval
    self.updates = []
    # self.update_event_no = nil
    self.clear_before_arm()
    self.is_keep_alive = false

    # log("MTR: new subsctiption " + matter.inspect(self), 3)
  end
  
  #################################################################################
  # set the event generators
  #
  # Force to always be an actual array
  def set_event_generator_or_arr(event_generator_or_arr)
    # copy list of event paths, we will reuse each time we send events
    if event_generator_or_arr == nil
      self.event_generators = []
    elif isinstance(event_generator_or_arr, list)
      self.event_generators = event_generator_or_arr
    else
      self.event_generators = [ event_generator_or_arr ]
    end
  end

  #################################################################################
  # remove self from subs_shop list
  def remove_self()
    log("MTR: -Sub_Del   (      ) sub=" + str(self.subscription_id), 3)
    self.subs_shop.remove_sub(self)
  end

  #################################################################################
  # clear log after it was sent, and re-arm next expiration
  def clear_before_arm()
    self.updates.clear()
    self.wait_status = true
    self.update_event_no = nil                      # rearm for next events
  end

  #################################################################################
  # we received a complete ack for previous message, rearm
  def re_arm()
    if (!self.wait_status)  return    end   # make it reentrant
    self.wait_status = false
    var now = tasmota.millis()
    self.expiration = now + (self.max_interval - self.MAX_INTERVAL_MARGIN) * 1000
    self.not_before = now + self.min_interval * 1000 - 1
    if !self.is_keep_alive
      log(format("MTR: .Sub_Done  (      ) sub=%i", self.subscription_id), 3)
    end
  end

  #################################################################################
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

  #################################################################################
  # signal a new event, check if it matches any filter
  #
  # This is called whenever a new event is published.
  # We now that the new event number is above any event previously seen, so we can optimize
  #
  # Hence we store only the first event number that matches this generator.
  # We know that we don't need to go back in the past from this.
  # And if events are published after, we already have our starting number in `update_event_no`
  #
  # Note: since the generator skips from the last seen event, we need to substract 1 because
  # this event has not been sent in subscription updates
  #
  # Arg:
  #   - event: instance of Matter_EventQueued
  def event_published(new_event)
    if (self.update_event_no == nil)                              # if we have already an event to send updates, all events after this are potential candidates
      var idx = 0
      while idx < size(self.event_generators)
        if self.event_generators[idx].event_is_match(new_event)
          self.update_event_no = new_event.event_no.add(-1)       # we need to substract 1 becaus ewe haven't parse this one yet
        end
        idx += 1
      end
    end
  end

  #################################################################################
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

  #################################################################################
  # update_event_generator_array
  #
  # Returns an array, or single instance of nil, with generators
  def update_event_generator_array()
    if   size(self.event_generators) == 0
      return nil                                    # empty, return nil
    elif size(self.event_generators) == 1
      self.event_generators[0].restart_from(self.update_event_no)
      return self.event_generators[0]               # single element, return just the instance (most common case)
    else
      var res = []
      res.resize(size(self.event_generators))
      var idx = 0
      while idx < size(self.event_generators)
        self.event_generators[idx].restart_from(self.update_event_no)
        res[idx] = self.event_generators[idx]
        idx += 1
      end
    end
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
  def new_subscription(session, req, event_generator_or_arr)
    import crypto
    var id = crypto.random(2).get(0,2)
    while self.get_by_id(id)
      id = crypto.random(2).get(0,2)
    end

    var sub = matter.IM_Subscription(self, id, session, req, event_generator_or_arr)
    self.subs.push(sub)

    return sub
  end

  #################################################################################
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

  #################################################################################
  def get_by_id(id)
    var idx = 0
    while idx < size(self.subs)
      if self.subs[idx].subscription_id == id
        return self.subs[idx]
      end
      idx += 1
    end
  end

  #################################################################################
  # remove_by_session
  #
  # Cleaning: remove all subscriptions created by this sessions
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

  #################################################################################
  # remove_by_fabric
  #
  # Cleaning: remove all subscriptions created by this fabric
  def remove_by_fabric(fabric)
    for session: fabric._sessions
      self.remove_by_session(session)
    end
  end

  #############################################################
  # dispatch every 50ms click to sub-objects that need it
  #
  def every_50ms()
    # any data ready to send?
    var idx = 0
    while idx < size(self.subs)
      var sub = self.subs[idx]
      if (!sub.wait_status && tasmota.time_reached(sub.not_before))           # is the update window open (not in progress and minimum time has passed)
        if (size(sub.updates) > 0) || (sub.update_event_no != nil)            # we have updated attributes, or an event was published since last update
          self.im.send_subscribe_update(sub)
          sub.clear_before_arm()
        end
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

  #################################################################################
  # signal that an attribute was updated, to add to the list of reportable
  def attribute_updated_ctx(ctx, fabric_specific)
    # signal any relevant subscription
    var idx = 0
    while idx < size(self.subs)
      self.subs[idx].attribute_updated_ctx(ctx, fabric_specific)
      idx += 1
    end
  end

  #################################################################################
  # signal that a new event was published, see if any subscription is interested
  def event_published(new_event)
    var idx = 0
    while idx < size(self.subs)
      self.subs[idx].event_published(new_event)
      idx += 1
    end
  end
  
end
matter.IM_Subscription_Shop = Matter_IM_Subscription_Shop
