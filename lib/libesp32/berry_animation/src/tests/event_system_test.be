# Event System Test Suite
# Tests the event handler system and DSL integration

import string
import introspect
import animation
import animation_dsl

# Test counter for tracking test results
var test_count = 0
var passed_count = 0

def run_test(test_name, test_func)
  test_count += 1
  print(f"Running test {test_count}: {test_name}")
  
  try
    var result = test_func()
    if result
      passed_count += 1
      print(f"  ✓ PASSED")
    else
      print(f"  ✗ FAILED")
    end
  except .. as e, msg
    print(f"  ✗ ERROR: {e} - {msg}")
  end
  print()
end

# Test 1: Basic Event Handler Creation
def test_event_handler_creation()
  var handler = animation.event_handler("test_event", def(data) print("Event triggered") end, 10, nil, {})
  
  return handler.event_name == "test_event" &&
         handler.priority == 10 &&
         handler.is_active == true &&
         handler.condition == nil
end

# Test 2: Event Manager Registration
def test_event_manager_registration()
  var manager = animation.event_manager
  var callback_called = false
  
  var handler = manager.register_handler("button_press", def(data) callback_called = true end, 0, nil, nil)
  
  # Trigger the event
  manager.trigger_event("button_press", {"button": "main"})
  
  return callback_called == true
end

# Test 3: Event Priority Ordering
def test_event_priority_ordering()
  var manager = animation.event_manager
  var execution_order = []
  
  # Register handlers with different priorities
  manager.register_handler("test", def(data) execution_order.push("low") end, 1, nil, nil)
  manager.register_handler("test", def(data) execution_order.push("high") end, 10, nil, nil)
  manager.register_handler("test", def(data) execution_order.push("medium") end, 5, nil, nil)
  
  # Trigger event
  manager.trigger_event("test", {})
  
  # Check execution order (high priority first)
  return size(execution_order) == 3 &&
         execution_order[0] == "high" &&
         execution_order[1] == "medium" &&
         execution_order[2] == "low"
end

# Test 4: Event Conditions
def test_event_conditions()
  var manager = animation.event_manager
  var callback_called = false
  
  # Register handler with condition
  var condition = def(data) return data.find("allowed") == true end
  manager.register_handler("conditional", def(data) callback_called = true end, 0, condition, nil)
  
  # Trigger with condition false
  manager.trigger_event("conditional", {"allowed": false})
  if callback_called
    return false
  end
  
  # Trigger with condition true
  manager.trigger_event("conditional", {"allowed": true})
  return callback_called == true
end

# Test 5: Global Event Handlers
def test_global_event_handlers()
  var manager = animation.event_manager
  var global_events = []
  
  # Register global handler
  manager.register_handler("*", def(data) global_events.push(data["event_name"]) end, 0, nil, nil)
  
  # Trigger different events
  manager.trigger_event("event1", {})
  manager.trigger_event("event2", {})
  
  return size(global_events) == 2 &&
         global_events[0] == "event1" &&
         global_events[1] == "event2"
end

# Test 6: Animation Module Event Registration
def test_animation_module_integration()
  var callback_called = false
  
  # Register event through animation module
  var handler = animation.register_event_handler("module_test", def(data) callback_called = true end, 0, nil, nil)
  
  # Trigger event
  animation.trigger_event("module_test", {})
  
  # Clean up
  animation.unregister_event_handler(handler)
  
  return callback_called == true
end

# Test 7: DSL Event Handler Compilation
def test_dsl_event_compilation()
  var dsl_code = 
    "# strip length 30  # TEMPORARILY DISABLED\n"
    "color custom_red = 0xFF0000\n"
    "on button_press: solid(custom_red)\n"
    "animation anim = solid(color=custom_red)"
    "run anim"
  
  var compiled_code = animation_dsl.compile(dsl_code)
  
  # Check that compiled code contains event handler registration
  return compiled_code != nil &&
         string.find(compiled_code, "register_event_handler") >= 0 &&
         string.find(compiled_code, "button_press") >= 0
end

# Test 8: DSL Event with Parameters
def test_dsl_event_with_parameters()
  var dsl_code = 
    "# strip length 30  # TEMPORARILY DISABLED\n"
    "color custom_blue = 0x0000FF\n"
    "color custom_red = 0xFF0000\n"
    "on timer(5s): solid(custom_blue)\n"
    "animation anim = solid(color=custom_red)"
    "run anim"
  
  var compiled_code = animation_dsl.compile(dsl_code)
  
  # Check that compiled code contains timer parameters
  return compiled_code != nil &&
         string.find(compiled_code, "timer") >= 0 &&
         string.find(compiled_code, "5000") >= 0  # 5s converted to ms
end

# Test 9: Event Handler Deactivation
def test_event_handler_deactivation()
  var manager = animation.event_manager
  var callback_called = false
  
  var handler = manager.register_handler("deactivation_test", def(data) callback_called = true end, 0, nil, nil)
  
  # Deactivate handler
  handler.set_active(false)
  
  # Trigger event
  manager.trigger_event("deactivation_test", {})
  
  return callback_called == false
end

# Test 10: Event Queue Processing
def test_event_queue_processing()
  var manager = animation.event_manager
  var events_processed = []
  
  # Register handler that triggers another event (tests queue)
  manager.register_handler("trigger_chain", def(data) 
    events_processed.push("first")
    manager.trigger_event("chained_event", {})
  end, 0, nil, nil)
  
  manager.register_handler("chained_event", def(data)
    events_processed.push("second")
  end, 0, nil, nil)
  
  # Trigger initial event
  manager.trigger_event("trigger_chain", {})
  
  return size(events_processed) == 2 &&
         events_processed[0] == "first" &&
         events_processed[1] == "second"
end

# Test 11: Animation Engine Event Integration
def test_animation_engine_event_integration()
  # Create a real LED strip using global.Leds
  var strip = global.Leds(30)
  var engine = animation.create_engine(strip)
  
  # Test interrupt methods exist
  return introspect.contains(engine, "interrupt_current") &&
         introspect.contains(engine, "interrupt_all") &&
         introspect.contains(engine, "resume")
end

# Run all tests
def run_all_tests()
  print("=== Event System Test Suite ===")
  print()
  
  run_test("Event Handler Creation", test_event_handler_creation)
  run_test("Event Manager Registration", test_event_manager_registration)
  run_test("Event Priority Ordering", test_event_priority_ordering)
  run_test("Event Conditions", test_event_conditions)
  run_test("Global Event Handlers", test_global_event_handlers)
  run_test("Animation Module Integration", test_animation_module_integration)
  run_test("DSL Event Handler Compilation", test_dsl_event_compilation)
  run_test("DSL Event with Parameters", test_dsl_event_with_parameters)
  run_test("Event Handler Deactivation", test_event_handler_deactivation)
  run_test("Event Queue Processing", test_event_queue_processing)
  run_test("Animation Engine Event Integration", test_animation_engine_event_integration)
  
  print("=== Test Results ===")
  print(f"Total tests: {test_count}")
  print(f"Passed: {passed_count}")
  print(f"Failed: {test_count - passed_count}")
  print(f"Success rate: {int((passed_count * 100) / test_count)}%")
  
  return passed_count == test_count
end

if !run_all_tests()
  raise "test_failed"
end

# Export test function
return {
  "run_all_tests": run_all_tests
}