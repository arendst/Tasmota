/* Solidification of lvgl_glob.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_LVGL_glob;

/********************************************************************
** Solidified function: get_object_from_ptr
********************************************************************/
be_local_closure(LVGL_glob_get_object_from_ptr,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(cb_obj),
    /* K1   */  be_nested_str_weak(find),
    }),
    be_str_weak(get_object_from_ptr),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x20080403,  //  0002  NE	R2	R2	R3
      0x780A0004,  //  0003  JMPF	R2	#0009
      0x88080100,  //  0004  GETMBR	R2	R0	K0
      0x8C080501,  //  0005  GETMET	R2	R2	K1
      0x5C100200,  //  0006  MOVE	R4	R1
      0x7C080400,  //  0007  CALL	R2	2
      0x80040400,  //  0008  RET	1	R2
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_custom_widget
********************************************************************/
be_local_closure(LVGL_glob_create_custom_widget,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[27]) {     /* constants */
    /* K0   */  be_nested_str_weak(introspect),
    /* K1   */  be_nested_str_weak(lv),
    /* K2   */  be_nested_str_weak(lv_obj),
    /* K3   */  be_nested_str_weak(value_error),
    /* K4   */  be_nested_str_weak(arg_X20must_X20be_X20a_X20subclass_X20of_X20lv_obj),
    /* K5   */  be_nested_str_weak(widget_struct_by_class),
    /* K6   */  be_nested_str_weak(find),
    /* K7   */  be_nested_str_weak(widget_cb),
    /* K8   */  be_nested_str_weak(widget_struct_default),
    /* K9   */  be_nested_str_weak(copy),
    /* K10  */  be_nested_str_weak(base_class),
    /* K11  */  be_nested_str_weak(_class),
    /* K12  */  be_nested_str_weak(get),
    /* K13  */  be_nested_str_weak(widget_width_def),
    /* K14  */  be_nested_str_weak(width_def),
    /* K15  */  be_nested_str_weak(widget_height_def),
    /* K16  */  be_nested_str_weak(height_def),
    /* K17  */  be_nested_str_weak(widget_editable),
    /* K18  */  be_nested_str_weak(editable),
    /* K19  */  be_nested_str_weak(widget_group_def),
    /* K20  */  be_nested_str_weak(group_def),
    /* K21  */  be_nested_str_weak(widget_instance_size),
    /* K22  */  be_nested_str_weak(instance_size),
    /* K23  */  be_nested_str_weak(obj_class_create_obj),
    /* K24  */  be_nested_str_weak(_p),
    /* K25  */  be_nested_str_weak(register_obj),
    /* K26  */  be_nested_str_weak(class_init_obj),
    }),
    be_str_weak(create_custom_widget),
    &be_const_str_solidified,
    ( &(const binstruction[86]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0x6010000F,  //  0001  GETGBL	R4	G15
      0x5C140200,  //  0002  MOVE	R5	R1
      0xB81A0200,  //  0003  GETNGBL	R6	K1
      0x88180D02,  //  0004  GETMBR	R6	R6	K2
      0x7C100400,  //  0005  CALL	R4	2
      0x74120000,  //  0006  JMPT	R4	#0008
      0xB0060704,  //  0007  RAISE	1	K3	K4
      0x88100105,  //  0008  GETMBR	R4	R0	K5
      0x4C140000,  //  0009  LDNIL	R5
      0x1C100805,  //  000A  EQ	R4	R4	R5
      0x78120002,  //  000B  JMPF	R4	#000F
      0x60100013,  //  000C  GETGBL	R4	G19
      0x7C100000,  //  000D  CALL	R4	0
      0x90020A04,  //  000E  SETMBR	R0	K5	R4
      0x60100005,  //  000F  GETGBL	R4	G5
      0x5C140200,  //  0010  MOVE	R5	R1
      0x7C100200,  //  0011  CALL	R4	1
      0x88140105,  //  0012  GETMBR	R5	R0	K5
      0x8C140B06,  //  0013  GETMET	R5	R5	K6
      0x5C1C0800,  //  0014  MOVE	R7	R4
      0x7C140400,  //  0015  CALL	R5	2
      0x4C180000,  //  0016  LDNIL	R6
      0x1C180A06,  //  0017  EQ	R6	R5	R6
      0x781A002F,  //  0018  JMPF	R6	#0049
      0x8C180107,  //  0019  GETMET	R6	R0	K7
      0x7C180200,  //  001A  CALL	R6	1
      0x88180108,  //  001B  GETMBR	R6	R0	K8
      0x8C180D09,  //  001C  GETMET	R6	R6	K9
      0x7C180200,  //  001D  CALL	R6	1
      0x5C140C00,  //  001E  MOVE	R5	R6
      0x60180003,  //  001F  GETGBL	R6	G3
      0x5C1C0200,  //  0020  MOVE	R7	R1
      0x7C180200,  //  0021  CALL	R6	1
      0x88180D0B,  //  0022  GETMBR	R6	R6	K11
      0x90161406,  //  0023  SETMBR	R5	K10	R6
      0x8C18070C,  //  0024  GETMET	R6	R3	K12
      0x5C200200,  //  0025  MOVE	R8	R1
      0x5824000D,  //  0026  LDCONST	R9	K13
      0x7C180600,  //  0027  CALL	R6	3
      0x781A0001,  //  0028  JMPF	R6	#002B
      0x8818030D,  //  0029  GETMBR	R6	R1	K13
      0x90161C06,  //  002A  SETMBR	R5	K14	R6
      0x8C18070C,  //  002B  GETMET	R6	R3	K12
      0x5C200200,  //  002C  MOVE	R8	R1
      0x5824000F,  //  002D  LDCONST	R9	K15
      0x7C180600,  //  002E  CALL	R6	3
      0x781A0001,  //  002F  JMPF	R6	#0032
      0x8818030F,  //  0030  GETMBR	R6	R1	K15
      0x90162006,  //  0031  SETMBR	R5	K16	R6
      0x8C18070C,  //  0032  GETMET	R6	R3	K12
      0x5C200200,  //  0033  MOVE	R8	R1
      0x58240011,  //  0034  LDCONST	R9	K17
      0x7C180600,  //  0035  CALL	R6	3
      0x781A0001,  //  0036  JMPF	R6	#0039
      0x88180311,  //  0037  GETMBR	R6	R1	K17
      0x90162406,  //  0038  SETMBR	R5	K18	R6
      0x8C18070C,  //  0039  GETMET	R6	R3	K12
      0x5C200200,  //  003A  MOVE	R8	R1
      0x58240013,  //  003B  LDCONST	R9	K19
      0x7C180600,  //  003C  CALL	R6	3
      0x781A0001,  //  003D  JMPF	R6	#0040
      0x88180313,  //  003E  GETMBR	R6	R1	K19
      0x90162806,  //  003F  SETMBR	R5	K20	R6
      0x8C18070C,  //  0040  GETMET	R6	R3	K12
      0x5C200200,  //  0041  MOVE	R8	R1
      0x58240015,  //  0042  LDCONST	R9	K21
      0x7C180600,  //  0043  CALL	R6	3
      0x781A0001,  //  0044  JMPF	R6	#0047
      0x88180315,  //  0045  GETMBR	R6	R1	K21
      0x90162C06,  //  0046  SETMBR	R5	K22	R6
      0x88180105,  //  0047  GETMBR	R6	R0	K5
      0x98180805,  //  0048  SETIDX	R6	R4	R5
      0xB81A0200,  //  0049  GETNGBL	R6	K1
      0x8C180D17,  //  004A  GETMET	R6	R6	K23
      0x5C200A00,  //  004B  MOVE	R8	R5
      0x5C240400,  //  004C  MOVE	R9	R2
      0x7C180600,  //  004D  CALL	R6	3
      0x881C0D18,  //  004E  GETMBR	R7	R6	K24
      0x90063007,  //  004F  SETMBR	R1	K24	R7
      0x8C1C0119,  //  0050  GETMET	R7	R0	K25
      0x5C240200,  //  0051  MOVE	R9	R1
      0x7C1C0400,  //  0052  CALL	R7	2
      0x8C1C031A,  //  0053  GETMET	R7	R1	K26
      0x7C1C0200,  //  0054  CALL	R7	1
      0x80000000,  //  0055  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_cb_event_closure
********************************************************************/
be_local_closure(LVGL_glob_add_cb_event_closure,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(cb_event_closure),
    /* K1   */  be_nested_str_weak(contains),
    /* K2   */  be_nested_str_weak(function),
    /* K3   */  be_const_int(1),
    /* K4   */  be_nested_str_weak(push),
    /* K5   */  be_const_int(0),
    }),
    be_str_weak(add_cb_event_closure),
    &be_const_str_solidified,
    ( &(const binstruction[43]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x4C100000,  //  0001  LDNIL	R4
      0x1C0C0604,  //  0002  EQ	R3	R3	R4
      0x780E0002,  //  0003  JMPF	R3	#0007
      0x600C0013,  //  0004  GETGBL	R3	G19
      0x7C0C0000,  //  0005  CALL	R3	0
      0x90020003,  //  0006  SETMBR	R0	K0	R3
      0x880C0100,  //  0007  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0008  GETMET	R3	R3	K1
      0x5C140200,  //  0009  MOVE	R5	R1
      0x7C0C0400,  //  000A  CALL	R3	2
      0x780E001A,  //  000B  JMPF	R3	#0027
      0x880C0100,  //  000C  GETMBR	R3	R0	K0
      0x940C0601,  //  000D  GETIDX	R3	R3	R1
      0x60100004,  //  000E  GETGBL	R4	G4
      0x5C140600,  //  000F  MOVE	R5	R3
      0x7C100200,  //  0010  CALL	R4	1
      0x1C100902,  //  0011  EQ	R4	R4	K2
      0x78120007,  //  0012  JMPF	R4	#001B
      0x88100100,  //  0013  GETMBR	R4	R0	K0
      0x60140012,  //  0014  GETGBL	R5	G18
      0x7C140000,  //  0015  CALL	R5	0
      0x40180A03,  //  0016  CONNECT	R6	R5	R3
      0x40180A02,  //  0017  CONNECT	R6	R5	R2
      0x98100205,  //  0018  SETIDX	R4	R1	R5
      0x80060600,  //  0019  RET	1	K3
      0x7002000A,  //  001A  JMP		#0026
      0x88100100,  //  001B  GETMBR	R4	R0	K0
      0x94100801,  //  001C  GETIDX	R4	R4	R1
      0x8C100904,  //  001D  GETMET	R4	R4	K4
      0x5C180400,  //  001E  MOVE	R6	R2
      0x7C100400,  //  001F  CALL	R4	2
      0x6010000C,  //  0020  GETGBL	R4	G12
      0x88140100,  //  0021  GETMBR	R5	R0	K0
      0x94140A01,  //  0022  GETIDX	R5	R5	R1
      0x7C100200,  //  0023  CALL	R4	1
      0x04100903,  //  0024  SUB	R4	R4	K3
      0x80040800,  //  0025  RET	1	R4
      0x70020002,  //  0026  JMP		#002A
      0x880C0100,  //  0027  GETMBR	R3	R0	K0
      0x980C0202,  //  0028  SETIDX	R3	R1	R2
      0x80060A00,  //  0029  RET	1	K5
      0x80000000,  //  002A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _anonymous_
********************************************************************/
be_local_closure(LVGL_glob__anonymous_,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    0,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(LVG_X3A_X20call_X20to_X20unsupported_X20callback),
    }),
    be_str_weak(_anonymous_),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x60000001,  //  0000  GETGBL	R0	G1
      0x58040000,  //  0001  LDCONST	R1	K0
      0x7C000200,  //  0002  CALL	R0	1
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: widget_ctor_impl
********************************************************************/
be_local_closure(LVGL_glob_widget_ctor_impl,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(introspect),
    /* K1   */  be_nested_str_weak(lv),
    /* K2   */  be_nested_str_weak(lv_obj_class),
    /* K3   */  be_nested_str_weak(get_object_from_ptr),
    /* K4   */  be_nested_str_weak(cb_obj),
    /* K5   */  be_nested_str_weak(find),
    /* K6   */  be_nested_str_weak(instance),
    /* K7   */  be_nested_str_weak(get),
    /* K8   */  be_nested_str_weak(widget_constructor),
    }),
    be_str_weak(widget_ctor_impl),
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x5C180200,  //  0003  MOVE	R6	R1
      0x7C100400,  //  0004  CALL	R4	2
      0x8C140103,  //  0005  GETMET	R5	R0	K3
      0x5C1C0400,  //  0006  MOVE	R7	R2
      0x7C140400,  //  0007  CALL	R5	2
      0x88180104,  //  0008  GETMBR	R6	R0	K4
      0x8C180D05,  //  0009  GETMET	R6	R6	K5
      0x5C200A00,  //  000A  MOVE	R8	R5
      0x7C180400,  //  000B  CALL	R6	2
      0x781A0001,  //  000C  JMPF	R6	#000F
      0x88180104,  //  000D  GETMBR	R6	R0	K4
      0x94140C05,  //  000E  GETIDX	R5	R6	R5
      0x60180004,  //  000F  GETGBL	R6	G4
      0x5C1C0A00,  //  0010  MOVE	R7	R5
      0x7C180200,  //  0011  CALL	R6	1
      0x1C180D06,  //  0012  EQ	R6	R6	K6
      0x781A0007,  //  0013  JMPF	R6	#001C
      0x8C180707,  //  0014  GETMET	R6	R3	K7
      0x5C200A00,  //  0015  MOVE	R8	R5
      0x58240008,  //  0016  LDCONST	R9	K8
      0x7C180600,  //  0017  CALL	R6	3
      0x781A0002,  //  0018  JMPF	R6	#001C
      0x8C180B08,  //  0019  GETMET	R6	R5	K8
      0x5C200800,  //  001A  MOVE	R8	R4
      0x7C180400,  //  001B  CALL	R6	2
      0x80000000,  //  001C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: deregister_obj
********************************************************************/
be_local_closure(LVGL_glob_deregister_obj,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(cb_obj),
    /* K1   */  be_nested_str_weak(remove),
    /* K2   */  be_nested_str_weak(cb_event_closure),
    }),
    be_str_weak(deregister_obj),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x20080403,  //  0002  NE	R2	R2	R3
      0x780A0003,  //  0003  JMPF	R2	#0008
      0x88080100,  //  0004  GETMBR	R2	R0	K0
      0x8C080501,  //  0005  GETMET	R2	R2	K1
      0x5C100200,  //  0006  MOVE	R4	R1
      0x7C080400,  //  0007  CALL	R2	2
      0x88080102,  //  0008  GETMBR	R2	R0	K2
      0x4C0C0000,  //  0009  LDNIL	R3
      0x20080403,  //  000A  NE	R2	R2	R3
      0x780A0003,  //  000B  JMPF	R2	#0010
      0x88080102,  //  000C  GETMBR	R2	R0	K2
      0x8C080501,  //  000D  GETMET	R2	R2	K1
      0x5C100200,  //  000E  MOVE	R4	R1
      0x7C080400,  //  000F  CALL	R2	2
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: widget_event_impl
********************************************************************/
be_local_closure(LVGL_glob_widget_event_impl,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(introspect),
    /* K1   */  be_nested_str_weak(lv),
    /* K2   */  be_nested_str_weak(lv_obj_class),
    /* K3   */  be_nested_str_weak(lv_event),
    /* K4   */  be_nested_str_weak(target),
    /* K5   */  be_nested_str_weak(get_object_from_ptr),
    /* K6   */  be_nested_str_weak(instance),
    /* K7   */  be_nested_str_weak(code),
    /* K8   */  be_nested_str_weak(EVENT_DELETE),
    /* K9   */  be_nested_str_weak(get),
    /* K10  */  be_nested_str_weak(before_del),
    /* K11  */  be_nested_str_weak(widget_event),
    }),
    be_str_weak(widget_event_impl),
    &be_const_str_solidified,
    ( &(const binstruction[43]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x5C180200,  //  0003  MOVE	R6	R1
      0x7C100400,  //  0004  CALL	R4	2
      0xB8160200,  //  0005  GETNGBL	R5	K1
      0x8C140B03,  //  0006  GETMET	R5	R5	K3
      0x5C1C0400,  //  0007  MOVE	R7	R2
      0x7C140400,  //  0008  CALL	R5	2
      0x88180B04,  //  0009  GETMBR	R6	R5	K4
      0x8C1C0105,  //  000A  GETMET	R7	R0	K5
      0x5C240C00,  //  000B  MOVE	R9	R6
      0x7C1C0400,  //  000C  CALL	R7	2
      0x60200004,  //  000D  GETGBL	R8	G4
      0x5C240E00,  //  000E  MOVE	R9	R7
      0x7C200200,  //  000F  CALL	R8	1
      0x1C201106,  //  0010  EQ	R8	R8	K6
      0x78220017,  //  0011  JMPF	R8	#002A
      0x88200B07,  //  0012  GETMBR	R8	R5	K7
      0xB8260200,  //  0013  GETNGBL	R9	K1
      0x88241308,  //  0014  GETMBR	R9	R9	K8
      0x1C201009,  //  0015  EQ	R8	R8	R9
      0x78220009,  //  0016  JMPF	R8	#0021
      0x8C200709,  //  0017  GETMET	R8	R3	K9
      0x5C280E00,  //  0018  MOVE	R10	R7
      0x582C000A,  //  0019  LDCONST	R11	K10
      0x7C200600,  //  001A  CALL	R8	3
      0x78220004,  //  001B  JMPF	R8	#0021
      0x8C200F0A,  //  001C  GETMET	R8	R7	K10
      0x5C280800,  //  001D  MOVE	R10	R4
      0x5C2C0A00,  //  001E  MOVE	R11	R5
      0x7C200600,  //  001F  CALL	R8	3
      0x70020008,  //  0020  JMP		#002A
      0x8C200709,  //  0021  GETMET	R8	R3	K9
      0x5C280E00,  //  0022  MOVE	R10	R7
      0x582C000B,  //  0023  LDCONST	R11	K11
      0x7C200600,  //  0024  CALL	R8	3
      0x78220003,  //  0025  JMPF	R8	#002A
      0x8C200F0B,  //  0026  GETMET	R8	R7	K11
      0x5C280800,  //  0027  MOVE	R10	R4
      0x5C2C0A00,  //  0028  MOVE	R11	R5
      0x7C200600,  //  0029  CALL	R8	3
      0x80000000,  //  002A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_cb
********************************************************************/
be_local_closure(LVGL_glob_remove_cb,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(introspect),
    /* K1   */  be_nested_str_weak(_p),
    /* K2   */  be_nested_str_weak(deregister_obj),
    }),
    be_str_weak(remove_cb),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x880C0301,  //  0001  GETMBR	R3	R1	K1
      0x8C100102,  //  0002  GETMET	R4	R0	K2
      0x5C180600,  //  0003  MOVE	R6	R3
      0x7C100400,  //  0004  CALL	R4	2
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: lvgl_timer_dispatch
********************************************************************/
be_local_closure(LVGL_glob_lvgl_timer_dispatch,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(introspect),
    /* K1   */  be_nested_str_weak(toptr),
    /* K2   */  be_nested_str_weak(cb_event_closure),
    /* K3   */  be_nested_str_weak(function),
    /* K4   */  be_const_int(0),
    /* K5   */  be_const_int(1),
    }),
    be_str_weak(lvgl_timer_dispatch),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0501,  //  0001  GETMET	R3	R2	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x88100102,  //  0004  GETMBR	R4	R0	K2
      0x94100803,  //  0005  GETIDX	R4	R4	R3
      0x60140004,  //  0006  GETGBL	R5	G4
      0x5C180800,  //  0007  MOVE	R6	R4
      0x7C140200,  //  0008  CALL	R5	1
      0x1C140B03,  //  0009  EQ	R5	R5	K3
      0x78160003,  //  000A  JMPF	R5	#000F
      0x5C140800,  //  000B  MOVE	R5	R4
      0x5C180600,  //  000C  MOVE	R6	R3
      0x7C140200,  //  000D  CALL	R5	1
      0x7002000A,  //  000E  JMP		#001A
      0x58140004,  //  000F  LDCONST	R5	K4
      0x6018000C,  //  0010  GETGBL	R6	G12
      0x5C1C0800,  //  0011  MOVE	R7	R4
      0x7C180200,  //  0012  CALL	R6	1
      0x14180A06,  //  0013  LT	R6	R5	R6
      0x781A0004,  //  0014  JMPF	R6	#001A
      0x94180805,  //  0015  GETIDX	R6	R4	R5
      0x5C1C0600,  //  0016  MOVE	R7	R3
      0x7C180200,  //  0017  CALL	R6	1
      0x00140B05,  //  0018  ADD	R5	R5	K5
      0x7001FFF5,  //  0019  JMP		#0010
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: make_cb
********************************************************************/
be_local_closure(LVGL_glob_make_cb,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        4,                          /* nstack */
        1,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(remove_cb),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 5]) {  /* code */
          0x68040000,  //  0000  GETUPV	R1	U0
          0x8C040300,  //  0001  GETMET	R1	R1	K0
          0x5C0C0000,  //  0002  MOVE	R3	R0
          0x7C040400,  //  0003  CALL	R1	2
          0x80040200,  //  0004  RET	1	R1
        })
      ),
      be_nested_proto(
        4,                          /* nstack */
        1,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(lvgl_timer_dispatch),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 5]) {  /* code */
          0x68040000,  //  0000  GETUPV	R1	U0
          0x8C040300,  //  0001  GETMET	R1	R1	K0
          0x5C0C0000,  //  0002  MOVE	R3	R0
          0x7C040400,  //  0003  CALL	R1	2
          0x80040200,  //  0004  RET	1	R1
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(cb),
    /* K1   */  be_nested_str_weak(lv_event_cb),
    /* K2   */  be_nested_str_weak(register_obj),
    /* K3   */  be_nested_str_weak(add_event_cb),
    /* K4   */  be_nested_str_weak(lv),
    /* K5   */  be_nested_str_weak(EVENT_DELETE),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(add_cb_event_closure),
    /* K8   */  be_nested_str_weak(_p),
    /* K9   */  be_nested_str_weak(get_event_cb),
    /* K10  */  be_nested_str_weak(lv_timer_cb),
    /* K11  */  be_nested_str_weak(timer_cb),
    /* K12  */  be_nested_str_weak(gen_cb),
    /* K13  */  be_const_int(2),
    /* K14  */  be_nested_str_weak(lv_),
    /* K15  */  be_nested_str_weak(null_cb),
    /* K16  */  be_nested_str_weak(cb_do_nothing),
    }),
    be_str_weak(make_cb),
    &be_const_str_solidified,
    ( &(const binstruction[70]) {  /* code */
      0x4C100000,  //  0000  LDNIL	R4
      0x1C100604,  //  0001  EQ	R4	R3	R4
      0x78120001,  //  0002  JMPF	R4	#0005
      0x4C100000,  //  0003  LDNIL	R4
      0x80040800,  //  0004  RET	1	R4
      0xA4120000,  //  0005  IMPORT	R4	K0
      0x1C140701,  //  0006  EQ	R5	R3	K1
      0x78160016,  //  0007  JMPF	R5	#001F
      0x8C140102,  //  0008  GETMET	R5	R0	K2
      0x5C1C0400,  //  0009  MOVE	R7	R2
      0x7C140400,  //  000A  CALL	R5	2
      0x78160000,  //  000B  JMPF	R5	#000D
      0x50140001,  //  000C  LDBOOL	R5	0	1
      0x50140200,  //  000D  LDBOOL	R5	1	0
      0x78160005,  //  000E  JMPF	R5	#0015
      0x8C180503,  //  000F  GETMET	R6	R2	K3
      0x84200000,  //  0010  CLOSURE	R8	P0
      0xB8260800,  //  0011  GETNGBL	R9	K4
      0x88241305,  //  0012  GETMBR	R9	R9	K5
      0x58280006,  //  0013  LDCONST	R10	K6
      0x7C180800,  //  0014  CALL	R6	4
      0x8C180107,  //  0015  GETMET	R6	R0	K7
      0x88200508,  //  0016  GETMBR	R8	R2	K8
      0x5C240200,  //  0017  MOVE	R9	R1
      0x7C180600,  //  0018  CALL	R6	3
      0x8C1C0109,  //  0019  GETMET	R7	R0	K9
      0x5C240C00,  //  001A  MOVE	R9	R6
      0x7C1C0400,  //  001B  CALL	R7	2
      0xA0000000,  //  001C  CLOSE	R0
      0x80040E00,  //  001D  RET	1	R7
      0x70020024,  //  001E  JMP		#0044
      0x1C14070A,  //  001F  EQ	R5	R3	K10
      0x7816000F,  //  0020  JMPF	R5	#0031
      0x8814010B,  //  0021  GETMBR	R5	R0	K11
      0x4C180000,  //  0022  LDNIL	R6
      0x1C140A06,  //  0023  EQ	R5	R5	R6
      0x78160003,  //  0024  JMPF	R5	#0029
      0x8C14090C,  //  0025  GETMET	R5	R4	K12
      0x841C0001,  //  0026  CLOSURE	R7	P1
      0x7C140400,  //  0027  CALL	R5	2
      0x90021605,  //  0028  SETMBR	R0	K11	R5
      0x8C140107,  //  0029  GETMET	R5	R0	K7
      0x881C0508,  //  002A  GETMBR	R7	R2	K8
      0x5C200200,  //  002B  MOVE	R8	R1
      0x7C140600,  //  002C  CALL	R5	3
      0x8814010B,  //  002D  GETMBR	R5	R0	K11
      0xA0000000,  //  002E  CLOSE	R0
      0x80040A00,  //  002F  RET	1	R5
      0x70020012,  //  0030  JMP		#0044
      0x40160D0D,  //  0031  CONNECT	R5	K6	K13
      0x94140605,  //  0032  GETIDX	R5	R3	R5
      0x1C140B0E,  //  0033  EQ	R5	R5	K14
      0x7816000B,  //  0034  JMPF	R5	#0041
      0x8814010F,  //  0035  GETMBR	R5	R0	K15
      0x4C180000,  //  0036  LDNIL	R6
      0x1C140A06,  //  0037  EQ	R5	R5	R6
      0x78160003,  //  0038  JMPF	R5	#003D
      0x8C14090C,  //  0039  GETMET	R5	R4	K12
      0x881C0110,  //  003A  GETMBR	R7	R0	K16
      0x7C140400,  //  003B  CALL	R5	2
      0x90021E05,  //  003C  SETMBR	R0	K15	R5
      0x8814010F,  //  003D  GETMBR	R5	R0	K15
      0xA0000000,  //  003E  CLOSE	R0
      0x80040A00,  //  003F  RET	1	R5
      0x70020002,  //  0040  JMP		#0044
      0x4C140000,  //  0041  LDNIL	R5
      0xA0000000,  //  0042  CLOSE	R0
      0x80040A00,  //  0043  RET	1	R5
      0xA0000000,  //  0044  CLOSE	R0
      0x80000000,  //  0045  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_event_cb
********************************************************************/
be_local_closure(LVGL_glob_get_event_cb,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        5,                          /* nstack */
        1,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 2]) {  /* upvals */
          be_local_const_upval(1, 0),
          be_local_const_upval(1, 4),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(lvgl_event_dispatch),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 6]) {  /* code */
          0x68040000,  //  0000  GETUPV	R1	U0
          0x8C040300,  //  0001  GETMET	R1	R1	K0
          0x680C0001,  //  0002  GETUPV	R3	U1
          0x5C100000,  //  0003  MOVE	R4	R0
          0x7C040600,  //  0004  CALL	R1	3
          0x80040200,  //  0005  RET	1	R1
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(event_cb),
    /* K1   */  be_nested_str_weak(cb),
    /* K2   */  be_nested_str_weak(gen_cb),
    /* K3   */  be_nested_str_weak(push),
    /* K4   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(get_event_cb),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x1C080403,  //  0002  EQ	R2	R2	R3
      0x780A0002,  //  0003  JMPF	R2	#0007
      0x60080012,  //  0004  GETGBL	R2	G18
      0x7C080000,  //  0005  CALL	R2	0
      0x90020002,  //  0006  SETMBR	R0	K0	R2
      0x88080100,  //  0007  GETMBR	R2	R0	K0
      0x600C0010,  //  0008  GETGBL	R3	G16
      0x6010000C,  //  0009  GETGBL	R4	G12
      0x5C140400,  //  000A  MOVE	R5	R2
      0x7C100200,  //  000B  CALL	R4	1
      0x40100801,  //  000C  CONNECT	R4	R4	R1
      0x7C0C0200,  //  000D  CALL	R3	1
      0xA802000A,  //  000E  EXBLK	0	#001A
      0x5C100600,  //  000F  MOVE	R4	R3
      0x7C100000,  //  0010  CALL	R4	0
      0xB8160200,  //  0011  GETNGBL	R5	K1
      0x8C140B02,  //  0012  GETMET	R5	R5	K2
      0x841C0000,  //  0013  CLOSURE	R7	P0
      0x7C140400,  //  0014  CALL	R5	2
      0x8C180503,  //  0015  GETMET	R6	R2	K3
      0x5C200A00,  //  0016  MOVE	R8	R5
      0x7C180400,  //  0017  CALL	R6	2
      0xA00C0000,  //  0018  CLOSE	R3
      0x7001FFF4,  //  0019  JMP		#000F
      0x580C0004,  //  001A  LDCONST	R3	K4
      0xAC0C0200,  //  001B  CATCH	R3	1	0
      0xB0080000,  //  001C  RAISE	2	R0	R0
      0x940C0401,  //  001D  GETIDX	R3	R2	R1
      0xA0000000,  //  001E  CLOSE	R0
      0x80040600,  //  001F  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: lvgl_event_dispatch
********************************************************************/
be_local_closure(LVGL_glob_lvgl_event_dispatch,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(introspect),
    /* K1   */  be_nested_str_weak(toptr),
    /* K2   */  be_nested_str_weak(event),
    /* K3   */  be_nested_str_weak(_change_buffer),
    /* K4   */  be_nested_str_weak(lv),
    /* K5   */  be_nested_str_weak(lv_event),
    /* K6   */  be_nested_str_weak(target),
    /* K7   */  be_nested_str_weak(get_object_from_ptr),
    /* K8   */  be_nested_str_weak(cb_event_closure),
    /* K9   */  be_nested_str_weak(function),
    }),
    be_str_weak(lvgl_event_dispatch),
    &be_const_str_solidified,
    ( &(const binstruction[43]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0x8C100701,  //  0001  GETMET	R4	R3	K1
      0x5C180400,  //  0002  MOVE	R6	R2
      0x7C100400,  //  0003  CALL	R4	2
      0x88140102,  //  0004  GETMBR	R5	R0	K2
      0x78160004,  //  0005  JMPF	R5	#000B
      0x88140102,  //  0006  GETMBR	R5	R0	K2
      0x8C140B03,  //  0007  GETMET	R5	R5	K3
      0x5C1C0800,  //  0008  MOVE	R7	R4
      0x7C140400,  //  0009  CALL	R5	2
      0x70020004,  //  000A  JMP		#0010
      0xB8160800,  //  000B  GETNGBL	R5	K4
      0x8C140B05,  //  000C  GETMET	R5	R5	K5
      0x5C1C0800,  //  000D  MOVE	R7	R4
      0x7C140400,  //  000E  CALL	R5	2
      0x90020405,  //  000F  SETMBR	R0	K2	R5
      0x88140102,  //  0010  GETMBR	R5	R0	K2
      0x88140B06,  //  0011  GETMBR	R5	R5	K6
      0x8C180107,  //  0012  GETMET	R6	R0	K7
      0x5C200A00,  //  0013  MOVE	R8	R5
      0x7C180400,  //  0014  CALL	R6	2
      0x881C0108,  //  0015  GETMBR	R7	R0	K8
      0x941C0E05,  //  0016  GETIDX	R7	R7	R5
      0x60200004,  //  0017  GETGBL	R8	G4
      0x5C240E00,  //  0018  MOVE	R9	R7
      0x7C200200,  //  0019  CALL	R8	1
      0x1C201109,  //  001A  EQ	R8	R8	K9
      0x78220004,  //  001B  JMPF	R8	#0021
      0x5C200E00,  //  001C  MOVE	R8	R7
      0x5C240C00,  //  001D  MOVE	R9	R6
      0x88280102,  //  001E  GETMBR	R10	R0	K2
      0x7C200400,  //  001F  CALL	R8	2
      0x70020008,  //  0020  JMP		#002A
      0x6020000C,  //  0021  GETGBL	R8	G12
      0x5C240E00,  //  0022  MOVE	R9	R7
      0x7C200200,  //  0023  CALL	R8	1
      0x14200208,  //  0024  LT	R8	R1	R8
      0x78220003,  //  0025  JMPF	R8	#002A
      0x94200E01,  //  0026  GETIDX	R8	R7	R1
      0x5C240C00,  //  0027  MOVE	R9	R6
      0x88280102,  //  0028  GETMBR	R10	R0	K2
      0x7C200400,  //  0029  CALL	R8	2
      0x80000000,  //  002A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: register_obj
********************************************************************/
be_local_closure(LVGL_glob_register_obj,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(cb_obj),
    /* K1   */  be_nested_str_weak(contains),
    /* K2   */  be_nested_str_weak(_p),
    }),
    be_str_weak(register_obj),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x1C080403,  //  0002  EQ	R2	R2	R3
      0x780A0002,  //  0003  JMPF	R2	#0007
      0x60080013,  //  0004  GETGBL	R2	G19
      0x7C080000,  //  0005  CALL	R2	0
      0x90020002,  //  0006  SETMBR	R0	K0	R2
      0x88080100,  //  0007  GETMBR	R2	R0	K0
      0x8C080501,  //  0008  GETMET	R2	R2	K1
      0x88100302,  //  0009  GETMBR	R4	R1	K2
      0x7C080400,  //  000A  CALL	R2	2
      0x780A0002,  //  000B  JMPF	R2	#000F
      0x50080200,  //  000C  LDBOOL	R2	1	0
      0x80040400,  //  000D  RET	1	R2
      0x70020004,  //  000E  JMP		#0014
      0x88080100,  //  000F  GETMBR	R2	R0	K0
      0x880C0302,  //  0010  GETMBR	R3	R1	K2
      0x98080601,  //  0011  SETIDX	R2	R3	R1
      0x50080000,  //  0012  LDBOOL	R2	0	0
      0x80040400,  //  0013  RET	1	R2
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: widget_dtor_impl
********************************************************************/
be_local_closure(LVGL_glob_widget_dtor_impl,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(introspect),
    /* K1   */  be_nested_str_weak(lv),
    /* K2   */  be_nested_str_weak(lv_obj_class),
    /* K3   */  be_nested_str_weak(get_object_from_ptr),
    /* K4   */  be_nested_str_weak(instance),
    /* K5   */  be_nested_str_weak(get),
    /* K6   */  be_nested_str_weak(widget_destructor),
    }),
    be_str_weak(widget_dtor_impl),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x5C180200,  //  0003  MOVE	R6	R1
      0x7C100400,  //  0004  CALL	R4	2
      0x8C140103,  //  0005  GETMET	R5	R0	K3
      0x5C1C0400,  //  0006  MOVE	R7	R2
      0x7C140400,  //  0007  CALL	R5	2
      0x60180004,  //  0008  GETGBL	R6	G4
      0x5C1C0A00,  //  0009  MOVE	R7	R5
      0x7C180200,  //  000A  CALL	R6	1
      0x1C180D04,  //  000B  EQ	R6	R6	K4
      0x781A0007,  //  000C  JMPF	R6	#0015
      0x8C180705,  //  000D  GETMET	R6	R3	K5
      0x5C200A00,  //  000E  MOVE	R8	R5
      0x58240006,  //  000F  LDCONST	R9	K6
      0x7C180600,  //  0010  CALL	R6	3
      0x781A0002,  //  0011  JMPF	R6	#0015
      0x8C180B06,  //  0012  GETMET	R6	R5	K6
      0x5C200800,  //  0013  MOVE	R8	R4
      0x7C180400,  //  0014  CALL	R6	2
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(LVGL_glob_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        8,                          /* nstack */
        3,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(make_cb),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 7]) {  /* code */
          0x680C0000,  //  0000  GETUPV	R3	U0
          0x8C0C0700,  //  0001  GETMET	R3	R3	K0
          0x5C140000,  //  0002  MOVE	R5	R0
          0x5C180200,  //  0003  MOVE	R6	R1
          0x5C1C0400,  //  0004  MOVE	R7	R2
          0x7C0C0800,  //  0005  CALL	R3	4
          0x80040600,  //  0006  RET	1	R3
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(cb),
    /* K1   */  be_nested_str_weak(add_handler),
    /* K2   */  be_nested_str_weak(lv_extra),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x84100000,  //  0002  CLOSURE	R4	P0
      0x7C080400,  //  0003  CALL	R2	2
      0xA40A0400,  //  0004  IMPORT	R2	K2
      0xA0000000,  //  0005  CLOSE	R0
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: widget_cb
********************************************************************/
be_local_closure(LVGL_glob_widget_cb,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 3]) {
      be_nested_proto(
        6,                          /* nstack */
        2,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(widget_ctor_impl),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 6]) {  /* code */
          0x68080000,  //  0000  GETUPV	R2	U0
          0x8C080500,  //  0001  GETMET	R2	R2	K0
          0x5C100000,  //  0002  MOVE	R4	R0
          0x5C140200,  //  0003  MOVE	R5	R1
          0x7C080600,  //  0004  CALL	R2	3
          0x80040400,  //  0005  RET	1	R2
        })
      ),
      be_nested_proto(
        6,                          /* nstack */
        2,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(widget_dtor_impl),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 6]) {  /* code */
          0x68080000,  //  0000  GETUPV	R2	U0
          0x8C080500,  //  0001  GETMET	R2	R2	K0
          0x5C100000,  //  0002  MOVE	R4	R0
          0x5C140200,  //  0003  MOVE	R5	R1
          0x7C080600,  //  0004  CALL	R2	3
          0x80040400,  //  0005  RET	1	R2
        })
      ),
      be_nested_proto(
        6,                          /* nstack */
        2,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(widget_event_impl),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 6]) {  /* code */
          0x68080000,  //  0000  GETUPV	R2	U0
          0x8C080500,  //  0001  GETMET	R2	R2	K0
          0x5C100000,  //  0002  MOVE	R4	R0
          0x5C140200,  //  0003  MOVE	R5	R1
          0x7C080600,  //  0004  CALL	R2	3
          0x80040400,  //  0005  RET	1	R2
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_str_weak(cb),
    /* K1   */  be_nested_str_weak(widget_ctor_cb),
    /* K2   */  be_nested_str_weak(gen_cb),
    /* K3   */  be_nested_str_weak(widget_dtor_cb),
    /* K4   */  be_nested_str_weak(widget_event_cb),
    /* K5   */  be_nested_str_weak(widget_struct_default),
    /* K6   */  be_nested_str_weak(lv),
    /* K7   */  be_nested_str_weak(lv_obj_class),
    /* K8   */  be_nested_str_weak(lv_obj),
    /* K9   */  be_nested_str_weak(_class),
    /* K10  */  be_nested_str_weak(copy),
    /* K11  */  be_nested_str_weak(base_class),
    /* K12  */  be_nested_str_weak(constructor_cb),
    /* K13  */  be_nested_str_weak(destructor_cb),
    /* K14  */  be_nested_str_weak(event_cb),
    }),
    be_str_weak(widget_cb),
    &be_const_str_solidified,
    ( &(const binstruction[54]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x4C0C0000,  //  0002  LDNIL	R3
      0x1C080403,  //  0003  EQ	R2	R2	R3
      0x780A0003,  //  0004  JMPF	R2	#0009
      0x8C080302,  //  0005  GETMET	R2	R1	K2
      0x84100000,  //  0006  CLOSURE	R4	P0
      0x7C080400,  //  0007  CALL	R2	2
      0x90020202,  //  0008  SETMBR	R0	K1	R2
      0x88080103,  //  0009  GETMBR	R2	R0	K3
      0x4C0C0000,  //  000A  LDNIL	R3
      0x1C080403,  //  000B  EQ	R2	R2	R3
      0x780A0003,  //  000C  JMPF	R2	#0011
      0x8C080302,  //  000D  GETMET	R2	R1	K2
      0x84100001,  //  000E  CLOSURE	R4	P1
      0x7C080400,  //  000F  CALL	R2	2
      0x90020602,  //  0010  SETMBR	R0	K3	R2
      0x88080104,  //  0011  GETMBR	R2	R0	K4
      0x4C0C0000,  //  0012  LDNIL	R3
      0x1C080403,  //  0013  EQ	R2	R2	R3
      0x780A0003,  //  0014  JMPF	R2	#0019
      0x8C080302,  //  0015  GETMET	R2	R1	K2
      0x84100002,  //  0016  CLOSURE	R4	P2
      0x7C080400,  //  0017  CALL	R2	2
      0x90020802,  //  0018  SETMBR	R0	K4	R2
      0x88080105,  //  0019  GETMBR	R2	R0	K5
      0x4C0C0000,  //  001A  LDNIL	R3
      0x1C080403,  //  001B  EQ	R2	R2	R3
      0x780A0016,  //  001C  JMPF	R2	#0034
      0xB80A0C00,  //  001D  GETNGBL	R2	K6
      0x8C080507,  //  001E  GETMET	R2	R2	K7
      0xB8120C00,  //  001F  GETNGBL	R4	K6
      0x88100908,  //  0020  GETMBR	R4	R4	K8
      0x88100909,  //  0021  GETMBR	R4	R4	K9
      0x7C080400,  //  0022  CALL	R2	2
      0x8C08050A,  //  0023  GETMET	R2	R2	K10
      0x7C080200,  //  0024  CALL	R2	1
      0x90020A02,  //  0025  SETMBR	R0	K5	R2
      0x88080105,  //  0026  GETMBR	R2	R0	K5
      0xB80E0C00,  //  0027  GETNGBL	R3	K6
      0x880C0708,  //  0028  GETMBR	R3	R3	K8
      0x880C0709,  //  0029  GETMBR	R3	R3	K9
      0x900A1603,  //  002A  SETMBR	R2	K11	R3
      0x88080105,  //  002B  GETMBR	R2	R0	K5
      0x880C0101,  //  002C  GETMBR	R3	R0	K1
      0x900A1803,  //  002D  SETMBR	R2	K12	R3
      0x88080105,  //  002E  GETMBR	R2	R0	K5
      0x880C0103,  //  002F  GETMBR	R3	R0	K3
      0x900A1A03,  //  0030  SETMBR	R2	K13	R3
      0x88080105,  //  0031  GETMBR	R2	R0	K5
      0x880C0104,  //  0032  GETMBR	R3	R0	K4
      0x900A1C03,  //  0033  SETMBR	R2	K14	R3
      0xA0000000,  //  0034  CLOSE	R0
      0x80000000,  //  0035  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: LVGL_glob
********************************************************************/
be_local_class(LVGL_glob,
    11,
    NULL,
    be_nested_map(27,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(get_object_from_ptr, -1), be_const_closure(LVGL_glob_get_object_from_ptr_closure) },
        { be_const_key_weak(create_custom_widget, 18), be_const_closure(LVGL_glob_create_custom_widget_closure) },
        { be_const_key_weak(widget_cb, -1), be_const_closure(LVGL_glob_widget_cb_closure) },
        { be_const_key_weak(add_cb_event_closure, -1), be_const_closure(LVGL_glob_add_cb_event_closure_closure) },
        { be_const_key_weak(cb_do_nothing, -1), be_const_static_closure(LVGL_glob__anonymous__closure) },
        { be_const_key_weak(widget_dtor_cb, -1), be_const_var(7) },
        { be_const_key_weak(event_cb, -1), be_const_var(2) },
        { be_const_key_weak(widget_ctor_impl, -1), be_const_closure(LVGL_glob_widget_ctor_impl_closure) },
        { be_const_key_weak(null_cb, 25), be_const_var(5) },
        { be_const_key_weak(deregister_obj, 6), be_const_closure(LVGL_glob_deregister_obj_closure) },
        { be_const_key_weak(widget_event_impl, -1), be_const_closure(LVGL_glob_widget_event_impl_closure) },
        { be_const_key_weak(remove_cb, -1), be_const_closure(LVGL_glob_remove_cb_closure) },
        { be_const_key_weak(widget_ctor_cb, -1), be_const_var(6) },
        { be_const_key_weak(lvgl_timer_dispatch, -1), be_const_closure(LVGL_glob_lvgl_timer_dispatch_closure) },
        { be_const_key_weak(init, 22), be_const_closure(LVGL_glob_init_closure) },
        { be_const_key_weak(event, -1), be_const_var(4) },
        { be_const_key_weak(get_event_cb, -1), be_const_closure(LVGL_glob_get_event_cb_closure) },
        { be_const_key_weak(lvgl_event_dispatch, -1), be_const_closure(LVGL_glob_lvgl_event_dispatch_closure) },
        { be_const_key_weak(widget_event_cb, 23), be_const_var(8) },
        { be_const_key_weak(cb_event_closure, -1), be_const_var(1) },
        { be_const_key_weak(register_obj, -1), be_const_closure(LVGL_glob_register_obj_closure) },
        { be_const_key_weak(widget_dtor_impl, -1), be_const_closure(LVGL_glob_widget_dtor_impl_closure) },
        { be_const_key_weak(timer_cb, -1), be_const_var(3) },
        { be_const_key_weak(make_cb, -1), be_const_closure(LVGL_glob_make_cb_closure) },
        { be_const_key_weak(cb_obj, 14), be_const_var(0) },
        { be_const_key_weak(widget_struct_default, -1), be_const_var(9) },
        { be_const_key_weak(widget_struct_by_class, 2), be_const_var(10) },
    })),
    be_str_weak(LVGL_glob)
);
/*******************************************************************/

void be_load_LVGL_glob_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_LVGL_glob);
    be_setglobal(vm, "LVGL_glob");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
