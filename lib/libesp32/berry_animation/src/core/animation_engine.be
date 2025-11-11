# Unified Animation Engine
#
# Uses composition pattern: contains a root EngineProxy that manages all children.
# The engine provides infrastructure (strip output, fast_loop) while delegating
# child management and rendering to the root animation.

class AnimationEngine
  # Core properties
  var strip                 # LED strip object
  var strip_length          # Strip length (cached for performance)
  var root_animation        # Root EngineProxy that holds all children
  var frame_buffer          # Main frame buffer
  var temp_buffer           # Temporary buffer for blending
  
  # State management
  var is_running            # Whether engine is active
  var last_update           # Last update time in milliseconds
  var time_ms               # Current time in milliseconds (updated each frame)
  var fast_loop_closure     # Stored closure for fast_loop registration
  
  # Performance optimization
  var render_needed         # Whether a render pass is needed
  
  # CPU metrics tracking (streaming stats - no array storage)
  var tick_count            # Number of ticks in current period
  var tick_time_sum         # Sum of all tick times (for mean calculation)
  var tick_time_min         # Minimum tick time in period
  var tick_time_max         # Maximum tick time in period
  var anim_time_sum         # Sum of animation calculation times
  var anim_time_min         # Minimum animation calculation time
  var anim_time_max         # Maximum animation calculation time
  var hw_time_sum           # Sum of hardware output times
  var hw_time_min           # Minimum hardware output time
  var hw_time_max           # Maximum hardware output time
  
  # Intermediate measurement point metrics
  var phase1_time_sum       # Sum of phase 1 times (ts_start to ts_1)
  var phase1_time_min       # Minimum phase 1 time
  var phase1_time_max       # Maximum phase 1 time
  var phase2_time_sum       # Sum of phase 2 times (ts_1 to ts_2)
  var phase2_time_min       # Minimum phase 2 time
  var phase2_time_max       # Maximum phase 2 time
  var phase3_time_sum       # Sum of phase 3 times (ts_2 to ts_3)
  var phase3_time_min       # Minimum phase 3 time
  var phase3_time_max       # Maximum phase 3 time
  
  var last_stats_time       # Last time stats were printed
  var stats_period          # Stats reporting period (5000ms)
  
  # Profiling timestamps (only store timestamps, compute durations in _record_tick_metrics)
  var ts_start              # Timestamp: tick start
  var ts_1                  # Timestamp: intermediate measure point 1 (optional)
  var ts_2                  # Timestamp: intermediate measure point 2 (optional)
  var ts_3                  # Timestamp: intermediate measure point 3 (optional)
  var ts_hw                 # Timestamp: hardware output complete
  var ts_end                # Timestamp: tick end
  
  # Initialize the animation engine for a specific LED strip
  def init(strip)
    if strip == nil
      raise "value_error", "strip cannot be nil"
    end
    
    self.strip = strip
    self.strip_length = strip.length()
    
    # Create frame buffers
    self.frame_buffer = animation.frame_buffer(self.strip_length)
    self.temp_buffer = animation.frame_buffer(self.strip_length)
    
    # Create root EngineProxy to manage all children
    self.root_animation = animation.engine_proxy(self)
    self.root_animation.name = "root"
    
    # Initialize state
    self.is_running = false
    self.last_update = 0
    self.time_ms = 0
    self.fast_loop_closure = nil
    self.render_needed = false
    
    # Initialize CPU metrics
    self.tick_count = 0
    self.tick_time_sum = 0
    self.tick_time_min = 999999
    self.tick_time_max = 0
    self.anim_time_sum = 0
    self.anim_time_min = 999999
    self.anim_time_max = 0
    self.hw_time_sum = 0
    self.hw_time_min = 999999
    self.hw_time_max = 0
    
    # Initialize intermediate phase metrics
    self.phase1_time_sum = 0
    self.phase1_time_min = 999999
    self.phase1_time_max = 0
    self.phase2_time_sum = 0
    self.phase2_time_min = 999999
    self.phase2_time_max = 0
    self.phase3_time_sum = 0
    self.phase3_time_min = 999999
    self.phase3_time_max = 0
    
    self.last_stats_time = 0
    self.stats_period = 5000
    
    # Initialize profiling timestamps
    self.ts_start = nil
    self.ts_1 = nil
    self.ts_2 = nil
    self.ts_3 = nil
    self.ts_hw = nil
    self.ts_end = nil
  end
  
  # Run the animation engine
  # 
  # @return self for method chaining
  def run()
    if !self.is_running
      var now = tasmota.millis()
      self.is_running = true
      self.last_update = now - 10
      
      if self.fast_loop_closure == nil
        self.fast_loop_closure = / -> self.on_tick()
      end

      # Start the root animation (which starts all children)
      self.root_animation.start(now)
      
      tasmota.add_fast_loop(self.fast_loop_closure)
    end
    return self
  end
  
  # Stop the animation engine
  # 
  # @return self for method chaining
  def stop()
    if self.is_running
      self.is_running = false
      
      if self.fast_loop_closure != nil
        tasmota.remove_fast_loop(self.fast_loop_closure)
      end
    end
    return self
  end
  
  # Add an animation or sequence to the root animation
  # 
  # @param obj: Animation|SequenceManager - The object to add
  # @return bool - True if added, false if already exists
  def add(obj)
    var ret = self.root_animation.add(obj)
    if ret
      self.render_needed = true
    end
    return ret
  end
  
  # Remove an animation or sequence from the root animation
  # 
  # @param obj: Animation|SequenceManager - The object to remove
  # @return bool - True if removed, false if not found
  def remove(obj)
    var ret = self.root_animation.remove(obj)
    if ret
      self.render_needed = true
    end
    return ret
  end
  
  # Clear all animations and sequences
  def clear()
    # Stop and clear all children in root animation
    self.root_animation.clear()
    self.render_needed = true
    return self
  end
  
  # Main tick function called by fast_loop
  def on_tick(current_time)
    if !self.is_running
      return false
    end
    
    # Start timing this tick
    self.ts_start = tasmota.millis()
    
    if current_time == nil
      current_time = self.ts_start
    end
    
    # Check if strip length changed since last time
    self.check_strip_length()
    
    # Update engine time
    self.time_ms = current_time
    
    # Throttle updates to ~5ms intervals
    var delta_time = current_time - self.last_update
    if delta_time < 5
      return true
    end
    
    self.last_update = current_time
    
    # Check if strip can accept updates
    if self.strip.can_show != nil && !self.strip.can_show()
      return true
    end
    
    # Process any queued events (non-blocking)
    self._process_events(current_time)
    
    # Update and render root animation (which updates all children)
    self._update_and_render(current_time)
    
    # End timing and record metrics
    self.ts_end = tasmota.millis()
    self._record_tick_metrics(current_time)
    
    global.debug_animation = false
    return true
  end
  
  # Unified update and render process
  def _update_and_render(time_ms)
    self.ts_1 = tasmota.millis()
    # Update root animation (which updates all children)
    self.root_animation.update(time_ms)
    
    self.ts_2 = tasmota.millis()
    # Skip rendering if no children
    if self.root_animation.is_empty()
      if self.render_needed
        self._clear_strip()
        self.render_needed = false
      end
      return
    end
    
    # Clear main buffer
    self.frame_buffer.clear()
    
    # self.ts_2 = tasmota.millis()
    # Render root animation (which renders all children with blending)
    var rendered = self.root_animation.render(self.frame_buffer, time_ms)
    
    self.ts_3 = tasmota.millis()
    # Output to hardware and measure time
    self._output_to_strip()
    self.ts_hw = tasmota.millis()
    
    self.render_needed = false
  end
  
  # Output frame buffer to LED strip
  def _output_to_strip()
    self.strip.push_pixels_buffer_argb(self.frame_buffer.pixels)
    self.strip.show()
  end
  
  # Clear the LED strip
  def _clear_strip()
    self.strip.clear()
    self.strip.show()
  end
  
  # Event processing methods
  def _process_events(current_time)
    # Process any queued events from the animation event manager
    # This is called during fast_loop to handle events asynchronously
    if animation.event_manager != nil
      animation.event_manager._process_queued_events()
    end
  end
  
  # Record tick metrics and print stats periodically
  def _record_tick_metrics(current_time)
    # Compute durations from timestamps (only if timestamps are not nil)
    var tick_duration = nil
    var anim_duration = nil
    var hw_duration = nil
    var phase1_duration = nil
    var phase2_duration = nil
    var phase3_duration = nil
    
    # Total tick duration: from start to end
    if self.ts_start != nil && self.ts_end != nil
      tick_duration = self.ts_end - self.ts_start
    end
    
    # Animation duration: from ts_2 (after event processing) to ts_3 (before hardware)
    if self.ts_2 != nil && self.ts_3 != nil
      anim_duration = self.ts_3 - self.ts_2
    end
    
    # Hardware duration: from ts_3 (before hardware) to ts_hw (after hardware)
    if self.ts_3 != nil && self.ts_hw != nil
      hw_duration = self.ts_hw - self.ts_3
    end
    
    # Phase 1: from ts_start to ts_1 (initial checks)
    if self.ts_start != nil && self.ts_1 != nil
      phase1_duration = self.ts_1 - self.ts_start
    end
    
    # Phase 2: from ts_1 to ts_2 (event processing)
    if self.ts_1 != nil && self.ts_2 != nil
      phase2_duration = self.ts_2 - self.ts_1
    end
    
    # Phase 3: from ts_2 to ts_3 (animation update/render)
    if self.ts_2 != nil && self.ts_3 != nil
      phase3_duration = self.ts_3 - self.ts_2
    end
    
    # Initialize stats time on first tick
    if self.last_stats_time == 0
      self.last_stats_time = current_time
    end
    
    # Update streaming statistics (only if durations are valid)
    self.tick_count += 1
    
    if tick_duration != nil
      self.tick_time_sum += tick_duration
      if tick_duration < self.tick_time_min
        self.tick_time_min = tick_duration
      end
      if tick_duration > self.tick_time_max
        self.tick_time_max = tick_duration
      end
    end
    
    if anim_duration != nil
      self.anim_time_sum += anim_duration
      if anim_duration < self.anim_time_min
        self.anim_time_min = anim_duration
      end
      if anim_duration > self.anim_time_max
        self.anim_time_max = anim_duration
      end
    end
    
    if hw_duration != nil
      self.hw_time_sum += hw_duration
      if hw_duration < self.hw_time_min
        self.hw_time_min = hw_duration
      end
      if hw_duration > self.hw_time_max
        self.hw_time_max = hw_duration
      end
    end
    
    # Update phase metrics
    if phase1_duration != nil
      self.phase1_time_sum += phase1_duration
      if phase1_duration < self.phase1_time_min
        self.phase1_time_min = phase1_duration
      end
      if phase1_duration > self.phase1_time_max
        self.phase1_time_max = phase1_duration
      end
    end
    
    if phase2_duration != nil
      self.phase2_time_sum += phase2_duration
      if phase2_duration < self.phase2_time_min
        self.phase2_time_min = phase2_duration
      end
      if phase2_duration > self.phase2_time_max
        self.phase2_time_max = phase2_duration
      end
    end
    
    if phase3_duration != nil
      self.phase3_time_sum += phase3_duration
      if phase3_duration < self.phase3_time_min
        self.phase3_time_min = phase3_duration
      end
      if phase3_duration > self.phase3_time_max
        self.phase3_time_max = phase3_duration
      end
    end
    
    # Check if it's time to print stats (every 5 seconds)
    var time_since_stats = current_time - self.last_stats_time
    if time_since_stats >= self.stats_period
      self._print_stats(time_since_stats)
      
      # Reset for next period
      self.tick_count = 0
      self.tick_time_sum = 0
      self.tick_time_min = 999999
      self.tick_time_max = 0
      self.anim_time_sum = 0
      self.anim_time_min = 999999
      self.anim_time_max = 0
      self.hw_time_sum = 0
      self.hw_time_min = 999999
      self.hw_time_max = 0
      self.phase1_time_sum = 0
      self.phase1_time_min = 999999
      self.phase1_time_max = 0
      self.phase2_time_sum = 0
      self.phase2_time_min = 999999
      self.phase2_time_max = 0
      self.phase3_time_sum = 0
      self.phase3_time_min = 999999
      self.phase3_time_max = 0
      self.last_stats_time = current_time
    end
  end
  
  # Print CPU statistics
  def _print_stats(period_ms)
    if self.tick_count == 0
      return
    end
    
    # # Calculate statistics
    # var expected_ticks = period_ms / 5  # Expected ticks at 5ms intervals
    # var missed_ticks = expected_ticks - self.tick_count
    
    # Calculate means from sums
    var mean_time = self.tick_time_sum / self.tick_count
    var mean_anim = self.anim_time_sum / self.tick_count
    var mean_hw = self.hw_time_sum / self.tick_count

      var mean_phase1 = self.phase1_time_sum / self.tick_count
      var mean_phase2 = self.phase2_time_sum / self.tick_count
      var mean_phase3 = self.phase3_time_sum / self.tick_count
    
    # # Calculate CPU usage percentage
    # var cpu_percent = (self.tick_time_sum * 100) / period_ms
    
    # Format and log stats - split into animation calc vs hardware output
    var stats_msg = f"AnimEngine: ticks={self.tick_count} total={mean_time:.2f}ms({self.tick_time_min}-{self.tick_time_max}) events={mean_phase1:.2f}ms({self.phase1_time_min}-{self.phase1_time_max}) update={mean_phase2:.2f}ms({self.phase2_time_min}-{self.phase2_time_max}) anim={mean_anim:.2f}ms({self.anim_time_min}-{self.anim_time_max}) hw={mean_hw:.2f}ms({self.hw_time_min}-{self.hw_time_max})"
    tasmota.log(stats_msg, 3)  # Log level 3 (DEBUG)
  end
  
  # Interrupt current animations
  def interrupt_current()
    self.root_animation.stop()
  end
  
  # Interrupt specific animation by name
  def interrupt_animation(name)
    var i = 0
    while i < size(self.root_animation.children)
      var child = self.root_animation.children[i]
      if isinstance(child, animation.animation) && child.name != nil && child.name == name
        child.stop()
        self.root_animation.children.remove(i)
        return
      end
      i += 1
    end
  end
  
  # Resume animations (placeholder for future state management)
  def resume()
    # For now, just ensure engine is running
    if !self.is_running
      self.start()
    end
  end
  
  # Resume after a delay (placeholder for future implementation)
  def resume_after(delay_ms)
    tasmota.set_timer(delay_ms, def () self.resume() end)
  end
  
  # Utility methods for compatibility
  def get_strip()
    return self.strip
  end
  
  def get_strip_length()
    return self.strip_length
  end
  
  def is_active()
    return self.is_running
  end
  
  def size()
    # Count only animations, not sequences (for backward compatibility)
    return self.root_animation.size_animations()
  end
  
  def get_animations()
    return self.root_animation.get_animations()
  end
  
  # Backward compatibility: get sequence managers
  def sequence_managers()
    return self.root_animation.sequences
  end
  
  # Backward compatibility: get animations list
  def animations()
    return self.get_animations()
  end
  
  # Check if the length of the strip changes
  #
  # @return bool - True if strip lengtj was changed, false otherwise
  def check_strip_length()
    var current_length = self.strip.length()
    if current_length != self.strip_length
      self._handle_strip_length_change(current_length)
      return true  # Length changed
    end
    return false  # No change
  end
  
  # Handle strip length changes by resizing buffers
  def _handle_strip_length_change(new_length)
    if new_length <= 0
      return  # Invalid length, ignore
    end
    
    self.strip_length = new_length
    
    # Resize existing frame buffers instead of creating new ones
    self.frame_buffer.resize(new_length)
    self.temp_buffer.resize(new_length)
    
    # Force a render to clear any stale pixels
    self.render_needed = true
  end
  
  # Cleanup method for proper resource management
  def cleanup()
    self.stop()
    self.clear()
    self.frame_buffer = nil
    self.temp_buffer = nil
    self.strip = nil
  end
  
  # Sequence iteration tracking methods, delegate to EngineProxy
  
  # Push a new iteration context onto the stack
  # Called when a sequence starts repeating
  #
  # @param iteration_number: int - The current iteration number (0-based)
  def push_iteration_context(iteration_number)
    return self.root_animation.push_iteration_context(iteration_number)
  end
  
  # Pop the current iteration context from the stack
  # Called when a sequence finishes repeating
  def pop_iteration_context()
    return self.root_animation.pop_iteration_context()
  end
  
  # Update the current iteration number in the top context
  # Called when a sequence advances to the next iteration
  #
  # @param iteration_number: int - The new iteration number (0-based)
  def update_current_iteration(iteration_number)
    return self.root_animation.update_current_iteration(iteration_number)
  end
  
  # Get the current iteration number from the innermost sequence context
  # Used by IterationNumberProvider to return the current iteration
  #
  # @return int|nil - Current iteration number (0-based) or nil if not in sequence
  def get_current_iteration_number()
    return self.root_animation.get_current_iteration_number()
  end
  
  # String representation
  def tostring()
    return f"AnimationEngine(running={self.is_running})"
  end
end

return {'create_engine': AnimationEngine}