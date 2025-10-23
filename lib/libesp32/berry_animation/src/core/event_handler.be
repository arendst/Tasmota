# Event Handler System for Berry Animation Framework
# Manages event callbacks and execution

class EventHandler
  var event_name      # Name of the event (e.g., "button_press", "timer")
  var callback_func   # Function to call when event occurs
  var condition       # Optional condition function (returns true/false)
  var priority        # Handler priority (higher = executed first)
  var is_active       # Whether this handler is currently active
  var metadata        # Additional event metadata (e.g., timer interval)
  
  def init(event_name, callback_func, priority, condition, metadata)
    self.event_name = event_name
    self.callback_func = callback_func
    self.priority = priority != nil ? priority : 0
    self.condition = condition
    self.is_active = true
    self.metadata = metadata != nil ? metadata : {}
  end
  
  # Execute the event handler if conditions are met
  def execute(event_data)
    if !self.is_active
      return false
    end
    
    # Check condition if provided
    if self.condition != nil
      if !self.condition(event_data)
        return false
      end
    end
    
    # Execute callback
    if self.callback_func != nil
      self.callback_func(event_data)
      return true
    end
    
    return false
  end
  
  # Enable/disable the handler
  def set_active(active)
    self.is_active = active
  end
  
  # Get handler info for debugging
  # def get_info()
  #   return {
  #     "event_name": self.event_name,
  #     "priority": self.priority,
  #     "is_active": self.is_active,
  #     "has_condition": self.condition != nil,
  #     "metadata": self.metadata
  #   }
  # end
end

#@ solidify:EventManager,weak
class EventManager
  var handlers        # Map of event_name -> list of handlers
  var global_handlers # Handlers that respond to all events
  var event_queue     # Simple event queue for deferred processing
  var is_processing   # Flag to prevent recursive event processing
  
  def init()
    self.handlers = {}
    self.global_handlers = []
    self.event_queue = []
    self.is_processing = false
  end
  
  # Register an event handler
  def register_handler(event_name, callback_func, priority, condition, metadata)
    var handler = animation.event_handler(event_name, callback_func, priority, condition, metadata)
    
    if event_name == "*"
      # Global handler for all events
      self.global_handlers.push(handler)
      self._sort_handlers(self.global_handlers)
    else
      # Specific event handler
      if !self.handlers.contains(event_name)
        self.handlers[event_name] = []
      end
      self.handlers[event_name].push(handler)
      self._sort_handlers(self.handlers[event_name])
    end
    
    return handler
  end
  
  # Remove an event handler
  def unregister_handler(handler)
    if handler.event_name == "*"
      var idx = self.global_handlers.find(handler)
      if idx != nil
        self.global_handlers.remove(idx)
      end
    else
      var event_handlers = self.handlers.find(handler.event_name)
      if event_handlers != nil
        var idx = event_handlers.find(handler)
        if idx != nil
          event_handlers.remove(idx)
        end
      end
    end
  end
  
  # Trigger an event immediately
  def trigger_event(event_name, event_data)
    if self.is_processing
      # Queue event to prevent recursion
      self.event_queue.push({"name": event_name, "data": event_data})
      return
    end
    
    self.is_processing = true
    
    try
      # Execute global handlers first
      for handler : self.global_handlers
        if handler.is_active
          handler.execute({"event_name": event_name, "data": event_data})
        end
      end
      
      # Execute specific event handlers
      var event_handlers = self.handlers.find(event_name)
      if event_handlers != nil
        for handler : event_handlers
          if handler.is_active
            handler.execute(event_data)
          end
        end
      end
      
    except .. as e, msg
      print("Event processing error:", e, msg)
    end
    
    self.is_processing = false
    
    # Process queued events
    self._process_queued_events()
  end
  
  # Process any queued events
  def _process_queued_events()
    while self.event_queue.size() > 0
      var queued_event = self.event_queue.pop(0)
      self.trigger_event(queued_event["name"], queued_event["data"])
    end
  end
  
  # Sort handlers by priority (higher priority first)
  def _sort_handlers(handler_list)
    # Insertion sort for small lists (embedded-friendly and efficient)
    for i : 1..size(handler_list)-1
      var k = handler_list[i]
      var j = i
      while (j > 0) && (handler_list[j-1].priority < k.priority)
        handler_list[j] = handler_list[j-1]
        j -= 1
      end
      handler_list[j] = k
    end
  end
  
  # Get all registered events
  def get_registered_events()
    var events = []
    for event_name : self.handlers.keys()
      events.push(event_name)
    end
    return events
  end
  
  # Get handlers for a specific event
  def get_handlers(event_name)
    var result = []
    
    # Add global handlers
    for handler : self.global_handlers
      result.push(handler.get_info())
    end
    
    # Add specific handlers
    var event_handlers = self.handlers.find(event_name)
    if event_handlers != nil
      for handler : event_handlers
        result.push(handler.get_info())
      end
    end
    
    return result
  end
  
  # Clear all handlers
  def clear_all_handlers()
    self.handlers.clear()
    self.global_handlers.clear()
    self.event_queue.clear()
  end
  
  # Enable/disable all handlers for an event
  def set_event_active(event_name, active)
    var event_handlers = self.handlers.find(event_name)
    if event_handlers != nil
      for handler : event_handlers
        handler.set_active(active)
      end
    end
  end
end

# Event system functions to monad
def register_event_handler(event_name, callback_func, priority, condition, metadata)
  return animation.event_manager.register_handler(event_name, callback_func, priority, condition, metadata)
end

def unregister_event_handler(handler)
  animation.event_manager.unregister_handler(handler)
end

def trigger_event(event_name, event_data)
  animation.event_manager.trigger_event(event_name, event_data)
end

def get_registered_events()
  return animation.event_manager.get_registered_events()
end

def get_event_handlers(event_name)
  return animation.event_manager.get_handlers(event_name)
end

def clear_all_event_handlers()
  animation.event_manager.clear_all_handlers()
end

def set_event_active(event_name, active)
  animation.event_manager.set_event_active(event_name, active)
end

# Export classes
return {
  "event_handler": EventHandler,
  "EventManager": EventManager,
  'register_event_handler': register_event_handler,
  'unregister_event_handler': unregister_event_handler,
  'trigger_event': trigger_event,
  'get_registered_events': get_registered_events,
  'get_event_handlers': get_event_handlers,
  'clear_all_event_handlers': clear_all_event_handlers,
  'set_event_active': set_event_active,
}