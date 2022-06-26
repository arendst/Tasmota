/********************************************************************
 * Berry module `mqtt`
 * 
 *******************************************************************/

#include "be_constobj.h"
#include "be_mapping.h"

/* Embedded code */

/*

def _(action, topic, closure)   # inner class for subscribe and unsubscribe
  class mqtt_listener
    var topic           # topic as a list of subtopics
    var fulltopic       # fulltopic as string
    var closure

    def init(topic, closure)
      import string
      self.fulltopic = topic
      self.topic = string.split(topic, '/')
      self.closure = closure
      tasmota.add_driver(self)
    end

    def remove()
      tasmota.remove_driver(self)
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

  import mqtt
  if action
    # subscribe
    mqtt._subscribe(topic)
    if type(closure) == 'function'
      tasmota.check_not_method(closure)
      mqtt_listener(topic, closure)
    end
  else
    # unsubscribe
    if topic != nil
      mqtt._unsubscribe(topic)
    end

    # scan through drivers if any matches the topic
    for d: tasmota._drivers.copy()    # make a copy of the list since we might modify it on the fly
      if isinstance(d, mqtt_listener)
        # class of interest
        # topic was specified
        if topic != nil
          if d.fulltopic == topic
            d.remove()
          end
        else
          # remove all topics
          if d.fulltopic != topic       # don't call again unsubscribe if we just did
            mqtt._unsubscribe(d.fulltopic)
          end
          d.remove()
        end
      end
    end
  end
end

def subscribe(topic, closure)
  import mqtt
  mqtt._(true, topic, closure)
end

def unsubscribe(topic)
  import mqtt
  mqtt._(false, topic)
end

solidify.dump(_) solidify.dump(subscribe) solidify.dump(unsubscribe)

#-
# example

import mqtt
def p(a,b,c) print("mqtt",a,b,c) end

mqtt.subscribe("/a/b", p)
mqtt.subscribe("/a/b/c", p)
mqtt.subscribe("#", p)

print(">unsub /a/b")
mqtt.unsubscribe("/a/b")
print(">unsub all")
mqtt.unsubscribe()


-#


*/



/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(mqtt_listener_init,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str(string),
    /* K1   */  be_nested_str(fulltopic),
    /* K2   */  be_nested_str(topic),
    /* K3   */  be_nested_str(split),
    /* K4   */  be_nested_str(_X2F),
    /* K5   */  be_nested_str(closure),
    /* K6   */  be_nested_str(tasmota),
    /* K7   */  be_nested_str(add_driver),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0x90020201,  //  0001  SETMBR	R0	K1	R1
      0x8C100703,  //  0002  GETMET	R4	R3	K3
      0x5C180200,  //  0003  MOVE	R6	R1
      0x581C0004,  //  0004  LDCONST	R7	K4
      0x7C100600,  //  0005  CALL	R4	3
      0x90020404,  //  0006  SETMBR	R0	K2	R4
      0x90020A02,  //  0007  SETMBR	R0	K5	R2
      0xB8120C00,  //  0008  GETNGBL	R4	K6
      0x8C100907,  //  0009  GETMET	R4	R4	K7
      0x5C180000,  //  000A  MOVE	R6	R0
      0x7C100400,  //  000B  CALL	R4	2
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove
********************************************************************/
be_local_closure(mqtt_listener_remove,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(tasmota),
    /* K1   */  be_nested_str(remove_driver),
    }),
    &be_const_str_remove,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: mqtt_data
********************************************************************/
be_local_closure(mqtt_listener_mqtt_data,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str(string),
    /* K1   */  be_nested_str(split),
    /* K2   */  be_nested_str(_X2F),
    /* K3   */  be_nested_str(topic),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str(_X23),
    /* K6   */  be_nested_str(_X2B),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str(closure),
    }),
    &be_const_str_mqtt_data,
    &be_const_str_solidified,
    ( &(const binstruction[53]) {  /* code */
      0xA4160000,  //  0000  IMPORT	R5	K0
      0x8C180B01,  //  0001  GETMET	R6	R5	K1
      0x5C200200,  //  0002  MOVE	R8	R1
      0x58240002,  //  0003  LDCONST	R9	K2
      0x7C180600,  //  0004  CALL	R6	3
      0x601C000C,  //  0005  GETGBL	R7	G12
      0x5C200C00,  //  0006  MOVE	R8	R6
      0x7C1C0200,  //  0007  CALL	R7	1
      0x88200103,  //  0008  GETMBR	R8	R0	K3
      0x6024000C,  //  0009  GETGBL	R9	G12
      0x5C281000,  //  000A  MOVE	R10	R8
      0x7C240200,  //  000B  CALL	R9	1
      0x58280004,  //  000C  LDCONST	R10	K4
      0x142C1409,  //  000D  LT	R11	R10	R9
      0x782E0013,  //  000E  JMPF	R11	#0023
      0x942C100A,  //  000F  GETIDX	R11	R8	R10
      0x1C301705,  //  0010  EQ	R12	R11	K5
      0x78320001,  //  0011  JMPF	R12	#0014
      0x7002000F,  //  0012  JMP		#0023
      0x7002000C,  //  0013  JMP		#0021
      0x28301407,  //  0014  GE	R12	R10	R7
      0x78320002,  //  0015  JMPF	R12	#0019
      0x50300000,  //  0016  LDBOOL	R12	0	0
      0x80041800,  //  0017  RET	1	R12
      0x70020007,  //  0018  JMP		#0021
      0x1C301706,  //  0019  EQ	R12	R11	K6
      0x78320000,  //  001A  JMPF	R12	#001C
      0x70020004,  //  001B  JMP		#0021
      0x94300C0A,  //  001C  GETIDX	R12	R6	R10
      0x2030160C,  //  001D  NE	R12	R11	R12
      0x78320001,  //  001E  JMPF	R12	#0021
      0x50300000,  //  001F  LDBOOL	R12	0	0
      0x80041800,  //  0020  RET	1	R12
      0x00281507,  //  0021  ADD	R10	R10	K7
      0x7001FFE9,  //  0022  JMP		#000D
      0x282C1409,  //  0023  GE	R11	R10	R9
      0x782E0003,  //  0024  JMPF	R11	#0029
      0x202C1207,  //  0025  NE	R11	R9	R7
      0x782E0001,  //  0026  JMPF	R11	#0029
      0x502C0000,  //  0027  LDBOOL	R11	0	0
      0x80041600,  //  0028  RET	1	R11
      0x882C0108,  //  0029  GETMBR	R11	R0	K8
      0x5C301600,  //  002A  MOVE	R12	R11
      0x5C340200,  //  002B  MOVE	R13	R1
      0x5C380400,  //  002C  MOVE	R14	R2
      0x5C3C0600,  //  002D  MOVE	R15	R3
      0x5C400800,  //  002E  MOVE	R16	R4
      0x7C300800,  //  002F  CALL	R12	4
      0x4C340000,  //  0030  LDNIL	R13
      0x1C34180D,  //  0031  EQ	R13	R12	R13
      0x78360000,  //  0032  JMPF	R13	#0034
      0x50300200,  //  0033  LDBOOL	R12	1	0
      0x80041800,  //  0034  RET	1	R12
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: mqtt_listener
********************************************************************/
be_local_class(mqtt_listener,
    3,
    NULL,
    be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(remove, -1), be_const_closure(mqtt_listener_remove_closure) },
        { be_const_key(mqtt_data, -1), be_const_closure(mqtt_listener_mqtt_data_closure) },
        { be_const_key(topic, -1), be_const_var(0) },
        { be_const_key(init, 0), be_const_closure(mqtt_listener_init_closure) },
        { be_const_key(closure, 1), be_const_var(2) },
        { be_const_key(fulltopic, -1), be_const_var(1) },
    })),
    (bstring*) &be_const_str_mqtt_listener
);

/********************************************************************
** Solidified function: _
********************************************************************/
be_local_closure(_,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_const_class(be_class_mqtt_listener),
    /* K1   */  be_nested_str(mqtt),
    /* K2   */  be_nested_str(_subscribe),
    /* K3   */  be_nested_str(function),
    /* K4   */  be_nested_str(tasmota),
    /* K5   */  be_nested_str(check_not_method),
    /* K6   */  be_nested_str(_unsubscribe),
    /* K7   */  be_nested_str(_drivers),
    /* K8   */  be_nested_str(copy),
    /* K9   */  be_nested_str(fulltopic),
    /* K10  */  be_nested_str(remove),
    /* K11  */  be_nested_str(stop_iteration),
    }),
    &be_const_str__,
    &be_const_str_solidified,
    ( &(const binstruction[63]) {  /* code */
      0x580C0000,  //  0000  LDCONST	R3	K0
      0xB4000000,  //  0001  CLASS	K0
      0xA4120200,  //  0002  IMPORT	R4	K1
      0x78020010,  //  0003  JMPF	R0	#0015
      0x8C140902,  //  0004  GETMET	R5	R4	K2
      0x5C1C0200,  //  0005  MOVE	R7	R1
      0x7C140400,  //  0006  CALL	R5	2
      0x60140004,  //  0007  GETGBL	R5	G4
      0x5C180400,  //  0008  MOVE	R6	R2
      0x7C140200,  //  0009  CALL	R5	1
      0x1C140B03,  //  000A  EQ	R5	R5	K3
      0x78160007,  //  000B  JMPF	R5	#0014
      0xB8160800,  //  000C  GETNGBL	R5	K4
      0x8C140B05,  //  000D  GETMET	R5	R5	K5
      0x5C1C0400,  //  000E  MOVE	R7	R2
      0x7C140400,  //  000F  CALL	R5	2
      0x5C140600,  //  0010  MOVE	R5	R3
      0x5C180200,  //  0011  MOVE	R6	R1
      0x5C1C0400,  //  0012  MOVE	R7	R2
      0x7C140400,  //  0013  CALL	R5	2
      0x70020028,  //  0014  JMP		#003E
      0x4C140000,  //  0015  LDNIL	R5
      0x20140205,  //  0016  NE	R5	R1	R5
      0x78160002,  //  0017  JMPF	R5	#001B
      0x8C140906,  //  0018  GETMET	R5	R4	K6
      0x5C1C0200,  //  0019  MOVE	R7	R1
      0x7C140400,  //  001A  CALL	R5	2
      0x60140010,  //  001B  GETGBL	R5	G16
      0xB81A0800,  //  001C  GETNGBL	R6	K4
      0x88180D07,  //  001D  GETMBR	R6	R6	K7
      0x8C180D08,  //  001E  GETMET	R6	R6	K8
      0x7C180200,  //  001F  CALL	R6	1
      0x7C140200,  //  0020  CALL	R5	1
      0xA8020018,  //  0021  EXBLK	0	#003B
      0x5C180A00,  //  0022  MOVE	R6	R5
      0x7C180000,  //  0023  CALL	R6	0
      0x601C000F,  //  0024  GETGBL	R7	G15
      0x5C200C00,  //  0025  MOVE	R8	R6
      0x5C240600,  //  0026  MOVE	R9	R3
      0x7C1C0400,  //  0027  CALL	R7	2
      0x781E0010,  //  0028  JMPF	R7	#003A
      0x4C1C0000,  //  0029  LDNIL	R7
      0x201C0207,  //  002A  NE	R7	R1	R7
      0x781E0005,  //  002B  JMPF	R7	#0032
      0x881C0D09,  //  002C  GETMBR	R7	R6	K9
      0x1C1C0E01,  //  002D  EQ	R7	R7	R1
      0x781E0001,  //  002E  JMPF	R7	#0031
      0x8C1C0D0A,  //  002F  GETMET	R7	R6	K10
      0x7C1C0200,  //  0030  CALL	R7	1
      0x70020007,  //  0031  JMP		#003A
      0x881C0D09,  //  0032  GETMBR	R7	R6	K9
      0x201C0E01,  //  0033  NE	R7	R7	R1
      0x781E0002,  //  0034  JMPF	R7	#0038
      0x8C1C0906,  //  0035  GETMET	R7	R4	K6
      0x88240D09,  //  0036  GETMBR	R9	R6	K9
      0x7C1C0400,  //  0037  CALL	R7	2
      0x8C1C0D0A,  //  0038  GETMET	R7	R6	K10
      0x7C1C0200,  //  0039  CALL	R7	1
      0x7001FFE6,  //  003A  JMP		#0022
      0x5814000B,  //  003B  LDCONST	R5	K11
      0xAC140200,  //  003C  CATCH	R5	1	0
      0xB0080000,  //  003D  RAISE	2	R0	R0
      0x80000000,  //  003E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: subscribe
********************************************************************/
be_local_closure(subscribe,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(mqtt),
    /* K1   */  be_nested_str(_),
    }),
    &be_const_str_subscribe,
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0501,  //  0001  GETMET	R3	R2	K1
      0x50140200,  //  0002  LDBOOL	R5	1	0
      0x5C180000,  //  0003  MOVE	R6	R0
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x7C0C0800,  //  0005  CALL	R3	4
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: unsubscribe
********************************************************************/
be_local_closure(unsubscribe,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(mqtt),
    /* K1   */  be_nested_str(_),
    }),
    &be_const_str_unsubscribe,
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x50100000,  //  0002  LDBOOL	R4	0	0
      0x5C140000,  //  0003  MOVE	R5	R0
      0x7C080600,  //  0004  CALL	R2	3
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/



extern int be_mqtt_publish(bvm *vm);

extern void be_mqtt_subscribe(const char* topic);     BE_FUNC_CTYPE_DECLARE(be_mqtt_subscribe, "", "s")
extern void be_mqtt_unsubscribe(const char* topic);   BE_FUNC_CTYPE_DECLARE(be_mqtt_unsubscribe, "", "s")

/* @const_object_info_begin
module mqtt (scope: global) {
  publish, func(be_mqtt_publish)
  subscribe, closure(subscribe_closure)
  unsubscribe, closure(unsubscribe_closure)
  _, closure(__closure)
  _subscribe, ctype_func(be_mqtt_subscribe)
  _unsubscribe, ctype_func(be_mqtt_unsubscribe)
}
@const_object_info_end */
#include "be_fixed_mqtt.h"
