#
# class Animate
#
# Animation framework
#

animate = module("animate")

# state-machine: from val a to b
class Animate_ins_ramp
  var a                 # starting value
  var b                 # end value
  var duration          # duration in milliseconds

  def init(a,b,duration)
    self.a = a
    self.b = b
    self.duration = duration
  end
end
animate.ins_ramp = Animate_ins_ramp

# state-machine: pause and goto
class Animate_ins_goto
  var pc_rel            # relative PC, -1 previous instruction, 1 next instruction, 0 means see pc_abs
  var pc_abs            # absolute PC, only if pc_rel == 0, address if next instruction
  var duration          # pause in milliseconds before goto, -1 means infinite (state-machine can be changed externally)

  def init(pc_rel, pc_abs, duration)
    self.pc_rel = pc_rel
    self.pc_abs = pc_abs
    self.duration = duration
  end
end
animate.ins_goto = Animate_ins_goto

class Animate_engine
  var code              # array of state-machine instructions
  var closure           # closure to call with the new value
  var pc                # program-counter
  var ins_time          # absolute time when the current instruction started
  var running           # is the animation running? allows fast return
  var value             # current value

  def init()
    self.code = []
    self.pc = 0         # start at instruction 0
    self.ins_time = 0
    self.running = false # not running by default
    #
  end

  # run but needs external calls to `animate()`
  # cur_time:int (opt) current timestamp in ms, defaults to `tasmota.millis()`
  # val:int (opt) starting value, default to `nil`
  def run(cur_time, val)
    if cur_time == nil   cur_time = tasmota.millis() end
    if (val != nil) self.value = val end
    self.ins_time = cur_time

    self.running = true
    tasmota.add_driver(self)
  end

  # runs autonomously in the Tasmota event loop
  def autorun(cur_time, val)
    self.run(cur_time, val)
    tasmota.add_driver(self)
  end

  def stop()
    self.running = false
    tasmota.remove_driver(self)
  end

  def is_running()
    return self.running
  end

  def every_50ms()
    self.animate()
  end

  def animate(cur_time)     # time in milliseconds, optional, defaults to `tasmota.millis()`
    if !self.running return end
    if cur_time == nil   cur_time = tasmota.millis() end
    # run through instructions
    while true
      var sub_index = cur_time - self.ins_time    # time since the beginning of current instruction
      #
      # make sure PC is valid
      if self.pc >= size(self.code)
        self.running = false
        break
      end
      #
      if self.pc < 0 raise "internal_error", "Animate pc is out of range" end
      var ins = self.code[self.pc]

      # Instruction Ramp
      if   isinstance(ins, animate.ins_ramp)
        var f = self.closure        # assign to a local variable to not call a method
        if sub_index < ins.duration
          # we're still in the ramp
          self.value = tasmota.scale_uint(sub_index, 0, ins.duration, ins.a, ins.b)
          # call closure
          if f   f(self.value) end  # call closure, need try? TODO
          break
        else
          self.value = ins.b
          if f   f(self.value) end  # set to last value
          self.pc += 1              # next instruction
          self.ins_time = cur_time - (sub_index - ins.duration)
        end

      # Instruction Goto
      elif isinstance(ins, animate.ins_goto)
        if sub_index < ins.duration
          break
        else
          if ins.pc_rel != 0
            self.pc += ins.pc_rel
          else
            self.pc = ins.pc_abs
          end
          self.ins_time = cur_time - (sub_index - ins.duration)
        end
      
      # Invalid
      else
        raise "internal_error", "unknown instruction"
      end
    end
    return self.value

  end
end
animate.engine = Animate_engine

class Animate_from_to : Animate_engine

  def init(closure, from, to, duration)
    super(self).init()
    self.closure = closure
    self.code.push(animate.ins_ramp(from, to, duration))
  end

end
animate.from_to = Animate_from_to

#-
a=Animate_from_to(nil, 0, 100, 5000)
a.autorun()
-#

class Animate_rotate : Animate_engine

  def init(closure, from, to, duration)
    super(self).init()
    self.closure = closure
    self.code.push(animate.ins_ramp(from, to, duration))
    self.code.push(animate.ins_goto(0, 0, 0))   # goto abs pc = 0 without any pause
  end

end
animate.rotate = Animate_rotate

#-
a=Animate_rotate(nil, 0, 100, 5000)
a.autorun()
-#

class Animate_back_forth : Animate_engine

  def init(closure, from, to, duration)
    super(self).init()
    self.closure = closure
    self.code.push(animate.ins_ramp(from, to, duration / 2))
    self.code.push(animate.ins_ramp(to, from, duration / 2))
    self.code.push(animate.ins_goto(0, 0, 0))   # goto abs pc = 0 without any pause
  end

end
animate.back_forth = Animate_back_forth

#-
a=Animate_back_forth(nil, 0, 100, 5000)
a.autorun()
-#
