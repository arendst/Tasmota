import json import string
tasmota = module("tasmota")
def log(m) print(m) end
def save() end

#######
import string
import json
import gc
import tasmota
#// import alias
import tasmota as t

def charsinstring(s,c)
  for i:0..size(s)-1
    for j:0..size(c)-1
      if s[i] == c[j] return i end
    end
  end
  return -1
end

tasmota._eqstr=/s1,s2-> str(s1) == str(s2)
tasmota._neqstr=/s1,s2-> str(s1) != str(s2)
tasmota._eq=/f1,f2-> real(f1) == real(f2)
tasmota._neq=/f1,f2-> real(f1) != real(f2)
tasmota._gt=/f1,f2-> real(f1) > real(f2)
tasmota._lt=/f1,f2-> real(f1) < real(f2)
tasmota._ge=/f1,f2-> real(f1) >= real(f2)
tasmota._le=/f1,f2-> real(f1) <= real(f2)
tasmota._op=[
  ['==',tasmota._eqstr],
  ['!==',tasmota._neqstr],
  ['=',tasmota._eq],
  ['!=',tasmota._neq],
  ['>=',tasmota._ge],
  ['<=',tasmota._le],
  ['>',tasmota._gt],
  ['<',tasmota._lt],
]
tasmota._operators="=<>!|"

# split the item when there is an operator, returns a list of (left,op,right)
# ex: "Dimmer>50" -> ["Dimmer",tasmota_gt,"50"]
tasmota.find_op = def (item)
  var pos = charsinstring(item, tasmota._operators)
  if pos>=0
    var op_split = string.split(item,pos)
    #print(op_split)
    var op_left = op_split[0]
    var op_rest = op_split[1]
    # iterate through operators
    for op:tasmota._op
      if string.find(op_rest,op[0]) == 0
        var op_func = op[1]
        var op_right = string.split(op_rest,size(op[0]))[1]
        return [op_left,op_func,op_right]
      end
    end
  end
  return [item, nil, nil]
end


def findkeyi(m,keyi)
  var keyu = string.toupper(keyi)
  if classof(m) == map
    for k:m.keys()
      if string.toupper(k)==keyu || keyi=='?'
        return k
      end
    end
  end
end


tasmota.try_rule = def (ev, rule, f)
  var rl_list = tasmota.find_op(rule)
  var e=ev
  var rl=string.split(rl_list[0],'#')
  for it:rl
    found=findkeyi(e,it)
    if found == nil
      return false
    end
    e=e[found]
  end
  # check if condition is true
  if rl_list[1]
    # did we find a function
    if !rl_list[1](e,rl_list[2])
      # condition is not met
      return false
    end
  end
  f(e,ev)
  return true
end
tasmota_rules={}
tasmota.rule = def(pat,f) tasmota_rules[pat] = f end

tasmota.exec_rules = def (ev_json)
  var ev = json.load(ev_json)
  var ret = false
  if ev == nil
    log('BRY: ERROR, bad json: '+ev_json, 3)
  end
  for r:tasmota_rules.keys()
    ret = tasmota.try_rule(ev,r,tasmota_rules[r]) || ret
  end
  return ret
end

tasmota.delay = def(ms)
  tend = tasmota.millis(ms)
  while !tasmota.timereached(tend)
    tasmota.yield()
  end
end

def load(f)
  try
    if f[0] != '/' f = '/' + f end
    compile(f,'file')()
  except .. as e
    log(string.format("BRY: could not load file '%s' - %s",f,e))
  end
end

#- Test
#################################################################

def log(m) print(m) end
def my_rule(e,ev) log("e1="+str(e)+" e2="+str(ev)) end

tasmota.rule("ZBRECEIVED#?#LINKQUALITY", my_rule)
tasmota.rule("ZBRECEIVED#0x1234", my_rule)

tasmota.rule("ZBRECEIVED#?#LINKQUALITY<10", my_rule)

tasmota.rule("Dimmer>50", my_rule)
tasmota.rule("Dimmer=01", my_rule)


tasmota.rule("Color==022600", my_rule)

tasmota.exec_rules('{"Color":"022600"}')

tasmota.exec_rules('{"ZbReceived":{"0x1234":{"Device":"0x1234","LinkQuality":50}}}')

tasmota.exec_rules('{"Dimmer":10}')



# tasmota.rule("DIMMER", my_rule)
# tasmota.rule("DIMMER#DATA#DATA", my_rule)
# tasmota.exec_rules('{"Dimmer":{"Data":50}}')


-#

#-
tasmota.find_op("aaa")
tasmota.find_op("aaa>50")
-#

#-
# Example of backlog equivalent

def backlog(cmd_list)
  delay_backlog = tasmota.getoption(34)   # in milliseconds
  delay = 0
  for cmd:cmd_list
    tasmota.timer(delay, /-> tasmota.cmd(cmd))
    delay = delay + delay_backlog
  end
end


br def backlog(cmd_list) delay_backlog = tasmota.getoption(34) delay = 0 for cmd:cmd_list tasmota.timer(delay, /-> tasmota.cmd(cmd)) delay = delay + delay_backlog end end

br backlog( [ "Power 0", "Status 4", "Power 1" ] )

-#

#-

tasmota.delay = def(ms) tend = tasmota.millis(ms) log(str(tasmota.millis())) while !tasmota.timereached(tend) end log(str(tasmota.millis())) end
tasmota.delay = def(ms) a=0 tend = tasmota.millis(ms) log(str(tasmota.millis())) while !tasmota.timereached(tend) a=a+1 end log(str(tasmota.millis())) log(str(a)) end

-#