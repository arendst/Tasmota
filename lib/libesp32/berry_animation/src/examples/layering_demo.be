# Animation Layering Demo
#
# This example demonstrates how to use the AnimationManager to layer multiple animations
# with different priorities and opacities.
#
# Command to run:
#    ./berry -s -g -m lib/libesp32/berry_animation lib/libesp32/berry_animation/examples/layering_demo.be

import global
import tasmota
import animation

# Create a simple animation class that fills with a solid color
class SolidAnimation : animation.animation
  var color
  
  def init(priority, color, name)
    super(self).init(priority, 0, false, name)
    self.color = color
    
    # Register parameters
    self.register_param("color", {"default": 0xFF0000})
    
    # Set initial values
    self.set_param("color", color)
  end
  
  def render(frame)
    if !self.is_running || frame == nil
      return false
    end
    
    # Extract components from color
    var r = self.color & 0xFF
    var g = (self.color >> 8) & 0xFF
    var b = (self.color >> 16) & 0xFF
    
    # Fill the frame with the color
    frame.fill_pixels(animation.frame_buffer.to_color(r, g, b))
    
    return true
  end
  
  def on_param_changed(name, value)
    if name == "color"
      self.color = value
    end
  end
end

# Create a simple animation class that creates a gradient
class GradientAnimation : animation.animation
  var color_start
  var color_end
  
  def init(priority, color_start, color_end, name)
    super(self).init(priority, 0, false, name)
    self.color_start = color_start
    self.color_end = color_end
    
    # Register parameters
    self.register_param("color_start", {"default": 0xFF0000})
    self.register_param("color_end", {"default": 0x0000FF})
    
    # Set initial values
    self.set_param("color_start", color_start)
    self.set_param("color_end", color_end)
  end
  
  def render(frame)
    if !self.is_running || frame == nil
      return false
    end
    
    # Create a gradient from start to end color
    frame.gradient_fill(self.color_start, self.color_end)
    
    return true
  end
  
  def on_param_changed(name, value)
    if name == "color_start"
      self.color_start = value
    elif name == "color_end"
      self.color_end = value
    end
  end
end

# Create a simple animation class that creates a pattern
class PatternAnimation : animation.animation
  var pattern_width
  var color1
  var color2
  
  def init(priority, pattern_width, color1, color2, name)
    super(self).init(priority, 0, false, name)
    self.pattern_width = pattern_width
    self.color1 = color1
    self.color2 = color2
    
    # Register parameters
    self.register_param("pattern_width", {"min": 1, "default": 1})
    self.register_param("color1", {"default": 0xFFFFFF})
    self.register_param("color2", {"default": 0x000000})
    
    # Set initial values
    self.set_param("pattern_width", pattern_width)
    self.set_param("color1", color1)
    self.set_param("color2", color2)
  end
  
  def render(frame)
    if !self.is_running || frame == nil
      return false
    end
    
    # Create a pattern of alternating colors
    var i = 0
    while i < frame.width
      var color = (i / self.pattern_width) % 2 == 0 ? self.color1 : self.color2
      
      # Extract components from color
      var r = color & 0xFF
      var g = (color >> 8) & 0xFF
      var b = (color >> 16) & 0xFF
      
      # Set the pixel
      frame.set_pixel_color(i, animation.frame_buffer.to_color(r, g, b))
      
      i += 1
    end
    
    return true
  end
  
  def on_param_changed(name, value)
    if name == "pattern_width"
      self.pattern_width = value
    elif name == "color1"
      self.color1 = value
    elif name == "color2"
      self.color2 = value
    end
  end
end

# Create a frame buffer for rendering
var width = 20  # 20 pixels
var frame = animation.frame_buffer(width)

# Create a renderer
var renderer = animation.renderer(width)

# Create an animation manager with the renderer
var manager = animation.animation_manager(nil, renderer)

# Create some animations with different priorities
var background = SolidAnimation(10, 0x000080FF, "background")  # Dark blue background (fully opaque)
var gradient = GradientAnimation(20, 0xFF000080, 0x00FF0080, "gradient")  # Red to green gradient with 50% alpha
var pattern = PatternAnimation(30, 2, 0xFFFFFF40, 0x00000040, "pattern")  # White/black pattern with 25% alpha

# Add animations to the manager
manager.add(background)
manager.add(gradient)
manager.add(pattern)

print("Added animations to manager:", manager)
print("Number of animations:", manager.size())

# Start all animations
var start_time = tasmota.millis()
manager.start_all(start_time)

# Render the animations to the frame buffer
var rendered = manager.render_animations(frame, start_time)
print("Rendered:", rendered)

# Print the first few pixels to see the blending result
print("Pixel colors (first 5):")
var i = 0
while i < 5
  var color = frame.get_pixel_color(i)
  var r = color & 0xFF
  var g = (color >> 8) & 0xFF
  var b = (color >> 16) & 0xFF
  print("  Pixel", i, ":", format("R:%d G:%d B:%d", r, g, b))
  i += 1
end

# Demonstrate changing priorities
print("\nChanging priorities:")
background.set_priority(40)  # Move background to top
manager._sort_animations()  # Resort animations

# Render again
manager.render_animations(frame, start_time)

# Print the first few pixels again
print("Pixel colors after priority change (first 5):")
i = 0
while i < 5
  var color = frame.get_pixel_color(i)
  var r = color & 0xFF
  var g = (color >> 8) & 0xFF
  var b = (color >> 16) & 0xFF
  print("  Pixel", i, ":", format("R:%d G:%d B:%d", r, g, b))
  i += 1
end

# Note: Opacity is now controlled through per-pixel alpha channels
print("\nNote: Opacity is now controlled through per-pixel alpha channels")

# Render again
manager.render_animations(frame, start_time)

# Print the first few pixels again
print("Pixel colors with per-pixel alpha (first 5):")
i = 0
while i < 5
  var color = frame.get_pixel_color(i)
  var r = color & 0xFF
  var g = (color >> 8) & 0xFF
  var b = (color >> 16) & 0xFF
  print("  Pixel", i, ":", format("R:%d G:%d B:%d", r, g, b))
  i += 1
end

print("\nLayering demo complete!")