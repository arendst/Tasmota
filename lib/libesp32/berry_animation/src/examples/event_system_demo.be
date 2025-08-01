# Event System Demo
# Demonstrates the event handling capabilities of the Berry Animation Framework

import string

# Import animation module
var animation = global.animation

print("=== Event System Demo ===")
print()

# Create a mock LED strip for demonstration
var mock_strip = {
  "length": def() return 30 end,
  "show": def() end,
  "can_show": def() return true end,
  "set_pixel": def(i, color) end
}

# Create animation engine
var engine = animation.create_engine(mock_strip)

print("1. Basic Event Handler Registration")
print("-----------------------------------")

# Register a simple event handler
var button_handler = animation.register_event_handler(
  "button_press",
  def(event_data)
    print(f"Button pressed! Data: {event_data}")
    # Add a flash animation
    var flash_anim = animation.solid(0xFFFFFFFF)  # White flash
    engine.add_animation(flash_anim)
  end,
  10,  # High priority
  nil, # No condition
  {}   # No metadata
)

print("✓ Registered button_press event handler")

# Trigger the event
print("Triggering button_press event...")
animation.trigger_event("button_press", {"button": "main", "timestamp": tasmota.millis()})
print()

print("2. Event Handler with Conditions")
print("--------------------------------")

# Register a conditional event handler
var brightness_handler = animation.register_event_handler(
  "brightness_change",
  def(event_data)
    var brightness = event_data["brightness"]
    if brightness > 80
      print("High brightness detected - switching to day mode")
    else
      print("Low brightness detected - switching to night mode")
    end
  end,
  5,   # Medium priority
  def(event_data) return event_data.contains("brightness") end,  # Condition
  {"type": "brightness_monitor"}  # Metadata
)

print("✓ Registered brightness_change event handler with condition")

# Trigger with valid data
print("Triggering brightness_change with brightness=90...")
animation.trigger_event("brightness_change", {"brightness": 90})

# Trigger with invalid data (should be ignored due to condition)
print("Triggering brightness_change without brightness data (should be ignored)...")
animation.trigger_event("brightness_change", {"other_data": "test"})
print()

print("3. Timer Event with Metadata")
print("----------------------------")

# Register a timer event handler
var timer_handler = animation.register_event_handler(
  "timer",
  def(event_data)
    print(f"Timer event triggered! Interval: {event_data['interval']}ms")
    # Add a pulse animation
    var pulse_anim = animation.pulse_animation(animation.solid(0xFF00FF00), 1000, 100, 255)  # Green pulse
    engine.add_animation(pulse_anim)
  end,
  0,   # Normal priority
  nil, # No condition
  {"interval": 5000, "repeat": true}  # 5-second timer
)

print("✓ Registered timer event handler")

# Simulate timer trigger
print("Simulating timer trigger...")
animation.trigger_event("timer", {"interval": 5000})
print()

print("4. Global Event Handler")
print("-----------------------")

# Register a global event handler that responds to all events
var global_handler = animation.register_event_handler(
  "*",  # Global event name
  def(event_data)
    var event_name = event_data["event_name"]
    print(f"Global handler: Event '{event_name}' was triggered")
  end,
  1,   # Low priority (runs after specific handlers)
  nil, # No condition
  {"type": "global_monitor"}
)

print("✓ Registered global event handler")

# Trigger various events to show global handler
print("Triggering various events to demonstrate global handler...")
animation.trigger_event("test_event_1", {"data": "test1"})
animation.trigger_event("test_event_2", {"data": "test2"})
print()

print("5. DSL Event Handler Compilation")
print("--------------------------------")

# Demonstrate DSL event handler compilation
var dsl_code = 
  "strip length 30\n"
  "color red = #FF0000\n"
  "color blue = #0000FF\n"
  "color white = #FFFFFF\n"
  "\n"
  "# Event handlers\n"
  "on button_press: solid(red)\n"
  "on timer(3s): solid(blue)\n"
  "on startup: solid(white)\n"
  "\n"
  "# Main sequence\n"
  "sequence main {\n"
  "  play solid(red) for 2s\n"
  "  play solid(blue) for 2s\n"
  "}\n"
  "\n"
  "run main"

print("Compiling DSL with event handlers...")
var compiled_code = animation.compile_dsl(dsl_code)

if compiled_code != nil
  print("✓ DSL compilation successful!")
  print("Generated Berry code contains:")
  if string.find(compiled_code, "register_event_handler") >= 0
    print("  - Event handler registrations")
  end
  if string.find(compiled_code, "button_press") >= 0
    print("  - Button press event")
  end
  if string.find(compiled_code, "timer") >= 0
    print("  - Timer event")
  end
  if string.find(compiled_code, "startup") >= 0
    print("  - Startup event")
  end
else
  print("✗ DSL compilation failed")
end
print()

print("6. Event System Status")
print("---------------------")

# Show registered events
var registered_events = animation.get_registered_events()
print(f"Registered events: {registered_events}")

# Show handlers for a specific event
var button_handlers = animation.get_event_handlers("button_press")
print(f"Button press handlers: {size(button_handlers)}")

for handler_info : button_handlers
  print(f"  - Priority: {handler_info['priority']}, Active: {handler_info['is_active']}")
end
print()

print("7. Animation Engine Integration")
print("-------------------------------")

# Demonstrate animation engine event integration
print("Testing animation engine interrupt methods...")

# Add some test animations
engine.add_animation(animation.solid(0xFF00FF00))  # Green
engine.add_animation(animation.solid(0xFF0000FF))  # Blue

print(f"Animations before interrupt: {engine.size()}")

# Test interrupt current
engine.interrupt_current()
print("✓ interrupt_current() executed")

# Add more animations
engine.add_animation(animation.solid(0xFFFF0000))  # Red
engine.add_animation(animation.solid(0xFFFFFF00))  # Yellow

print(f"Animations after adding more: {engine.size()}")

# Test interrupt all
engine.interrupt_all()
print("✓ interrupt_all() executed")
print(f"Animations after interrupt_all: {engine.size()}")

print()

print("8. Cleanup")
print("----------")

# Clean up event handlers
animation.unregister_event_handler(button_handler)
animation.unregister_event_handler(brightness_handler)
animation.unregister_event_handler(timer_handler)
animation.unregister_event_handler(global_handler)

print("✓ All event handlers unregistered")

# Verify cleanup
var remaining_events = animation.get_registered_events()
print(f"Remaining registered events: {size(remaining_events)}")

print()
print("=== Event System Demo Complete ===")
print("The event system is ready for use!")
print()
print("Key Features Demonstrated:")
print("- Basic event handler registration and triggering")
print("- Event priority ordering")
print("- Conditional event handlers")
print("- Global event handlers")
print("- Timer events with metadata")
print("- DSL event handler compilation")
print("- Animation engine integration")
print("- Event handler cleanup")