#- ------------------------------------------------------------ -#
#  Module `lv_tasmota` - piggybacks on `lv` to extend it
#- ------------------------------------------------------------ -#

#-

import path 
path.remove("mqtt.bec")
import solidify
load('mqtt.be')
var f = open("mqtt.c", "w")
solidify.dump(MQTT, false, f)
f.close()
print("Ok")

-#

class MQTT_ntv end    # for solidification only

#@ solidify:MQTT
class MQTT : MQTT_ntv
  var topics

  # def init()
  # end
  def lazy_init()
    if self.topics == nil
      self.topics = []
      tasmota.add_driver(self)
      tasmota.add_rule("Mqtt#Connected", / -> self.mqtt_connect())    # call mqtt_connect() when the connection occurs
    end
  end

  def mqtt_listener_class()

    class mqtt_listener
      var topic           # topic as a list of subtopics
      var fulltopic       # fulltopic as string
      var closure

      def init(topic, closure)
        import string
        self.fulltopic = topic
        self.topic = string.split(topic, '/')
        self.closure = closure
      end

      def tostring()
        return format("<instance: %s('%s')>", classname(self), self.fulltopic)
      end

      def mqtt_data(topic, idx, payload_s, payload_b)
        # check if the topic matches the patter
        import string
        var topic_elts = string.split(topic, '/')
        var topic_sz = size(topic_elts)
        var pat = self.topic
        var pat_sz = size(pat)
        var i = 0
        while i < pat_sz
          var pat_elt = pat[i]

          if pat_elt == '#'
            # joker, munch whatever is left
            # '#' is supposed to be the last character of the topic (we don't check it)
            break
          elif i >= topic_sz
            # the topic is too short - no match
            return false
          elif pat_elt == '+'
            # pass
          elif pat_elt != topic_elts[i]
            # topic element are different - no match
            return false
          end

          i += 1
        end

        if i >= pat_sz && pat_sz != topic_sz
          # the topic is too long and the pattern did not finish with '#' - no match
          return false
        end

        var cl = self.closure
        var ret = cl(topic, idx, payload_s, payload_b)
        if ret == nil   ret = true end    # return true if the return value is forgotten
        return ret
      end
    end
    return mqtt_listener
  end

  def subscribe(topic, closure)
    self.lazy_init()
    var found = false
    for m : self.topics
      if m.fulltopic == topic && m.closure == closure
        return                                       # we have already the subscription, ignore
      end
    end
    var mqtt_listener = self.mqtt_listener_class()
    if type(closure) == 'function'
      tasmota.check_not_method(closure)
      self.topics.push(mqtt_listener(topic, closure))
    else
      self.topics.push(mqtt_listener(topic))
    end
    self._subscribe(topic)
  end

  # if topic == nil, unsuscribe to all
  def unsubscribe(topic)
    if self.topics == nil  return end                 # nothing to do
    
    var i = 0
    while i < size(self.topics)
      if topic == nil || self.topics[i].fulltopic == topic
        if topic == nil self._unsubscribe(self.topics[i].fulltopic) end
        self.topics.remove(i)   # remove and don't increment
      else
        i += 1
      end
    end

    if topic != nil  self._unsubscribe(topic) end
  end

  def mqtt_data(topic, idx, payload_s, payload_b)
    if self.topics == nil  return end
    var ret = false
    for m : self.topics
      if m.closure != nil                                         # if no closure attached, skip it
        var res = m.mqtt_data(topic, idx, payload_s, payload_b)   # stop propagation?
        ret = ret || res
      end
    end
    return ret    # return true to stop propagation as a Tasmota cmd
  end

  def mqtt_connect()    # called when MQTT connects or re-connects
    tasmota.log("BRY: mqtt subscribe all registered topics", 3)
    for m : self.topics
      var fulltopic = m.fulltopic
      self._subscribe(fulltopic)
    end
    return false
  end
end

#-
# example

import mqtt
def p1(a,b,c) print("mqtt1",a,b,c) end
def p2(a,b,c) print("mqtt2",a,b,c) end
def p3(a,b,c) print("mqtt3",a,b,c) end

mqtt.subscribe("/a/b", p1)
mqtt.subscribe("/a/b/c", p2)
#mqtt.subscribe("#", p3)

print(">unsub /a/b")
mqtt.unsubscribe("/a/b")
print(">unsub all")
mqtt.unsubscribe()


-#

