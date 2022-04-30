/********************************************************************
 * Berry module `mqtt`
 * 
 *******************************************************************/

#include "be_constobj.h"
#include "be_mapping.h"

/* Embedded code */

/*

def subscribe(topic, closure)
  class mqtt_listener
    var topic
    var closure

    def init(topic, closure)
      import string
      self.topic = string.split(topic, '/')
      self.closure = closure
      tasmota.add_driver(self)
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
  mqtt._subscribe(topic)
  if type(closure) == 'function'
    tasmota.check_not_method(closure)
    mqtt_listener(topic, closure)
  end
end


*/



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
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str(string),
    /* K1   */  be_nested_str(topic),
    /* K2   */  be_nested_str(split),
    /* K3   */  be_nested_str(_X2F),
    /* K4   */  be_nested_str(closure),
    /* K5   */  be_nested_str(tasmota),
    /* K6   */  be_nested_str(add_driver),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0x8C100702,  //  0001  GETMET	R4	R3	K2
      0x5C180200,  //  0002  MOVE	R6	R1
      0x581C0003,  //  0003  LDCONST	R7	K3
      0x7C100600,  //  0004  CALL	R4	3
      0x90020204,  //  0005  SETMBR	R0	K1	R4
      0x90020802,  //  0006  SETMBR	R0	K4	R2
      0xB8120A00,  //  0007  GETNGBL	R4	K5
      0x8C100906,  //  0008  GETMET	R4	R4	K6
      0x5C180000,  //  0009  MOVE	R6	R0
      0x7C100400,  //  000A  CALL	R4	2
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: mqtt_listener
********************************************************************/
be_local_class(mqtt_listener,
    2,
    NULL,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(topic, 1), be_const_var(0) },
        { be_const_key(mqtt_data, -1), be_const_closure(mqtt_listener_mqtt_data_closure) },
        { be_const_key(closure, -1), be_const_var(1) },
        { be_const_key(init, -1), be_const_closure(mqtt_listener_init_closure) },
    })),
    (bstring*) &be_const_str_mqtt_listener
);

/********************************************************************
** Solidified function: subscribe
********************************************************************/
be_local_closure(subscribe,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_const_class(be_class_mqtt_listener),
    /* K1   */  be_nested_str(mqtt),
    /* K2   */  be_nested_str(_subscribe),
    /* K3   */  be_nested_str(function),
    /* K4   */  be_nested_str(tasmota),
    /* K5   */  be_nested_str(check_not_method),
    }),
    &be_const_str_subscribe,
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0xB4000000,  //  0001  CLASS	K0
      0xA40E0200,  //  0002  IMPORT	R3	K1
      0x8C100702,  //  0003  GETMET	R4	R3	K2
      0x5C180000,  //  0004  MOVE	R6	R0
      0x7C100400,  //  0005  CALL	R4	2
      0x60100004,  //  0006  GETGBL	R4	G4
      0x5C140200,  //  0007  MOVE	R5	R1
      0x7C100200,  //  0008  CALL	R4	1
      0x1C100903,  //  0009  EQ	R4	R4	K3
      0x78120007,  //  000A  JMPF	R4	#0013
      0xB8120800,  //  000B  GETNGBL	R4	K4
      0x8C100905,  //  000C  GETMET	R4	R4	K5
      0x5C180200,  //  000D  MOVE	R6	R1
      0x7C100400,  //  000E  CALL	R4	2
      0x5C100400,  //  000F  MOVE	R4	R2
      0x5C140000,  //  0010  MOVE	R5	R0
      0x5C180200,  //  0011  MOVE	R6	R1
      0x7C100400,  //  0012  CALL	R4	2
      0x80000000,  //  0013  RET	0
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
  _subscribe, ctype_func(be_mqtt_subscribe)
  unsubscribe, ctype_func(be_mqtt_unsubscribe)
}
@const_object_info_end */
#include "be_fixed_mqtt.h"
