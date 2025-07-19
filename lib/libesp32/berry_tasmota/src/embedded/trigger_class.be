#- Native code used for testing and code solidification -#
#- Do not use it directly -#

#@ solidify:Trigger

class Trigger
  var trig, f, id
  var o             # optional object, for Cron it contains the cron object, for rule, 'true' means run-once

  # trig: trigger of the event, either timestamp (int) or a rule matcher instance
  # f: function or closure to call
  # id: (any) identifier to allow removal of a specific trigger
  # o: (instance, optional) instance implementing `timer_reached(trig)`
  #    this is used to implement cron with a specific method for matching time reached
  def init(trig, f, id, o)
    self.trig = trig
    self.f = f
    self.id = id
    self.o = o
  end

  def tostring()
    return format("<instance: %s(%s, %s, %s)", str(classof(self)),
              str(self.trig), str(self.f), str(self.id))
  end

  def run_once()
    return self.o == true
  end

  ###########################################################################################
  # For cron triggers only
  ###########################################################################################
  # next() returns the next trigger, or 0 if rtc is invalid, or nil if no more
  def next()
    if self.o
      return self.o.next()
    end
  end
  
  # is the time of triggering reached?
  def time_reached()
    if self.o && self.trig > 0
      return self.o.time_reached(self.trig)
    end
    return false
  end
end
