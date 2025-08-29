/* Solidification of animation.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'BreatheAnimation' ktab size: 17, total: 24 (saved 56 bytes)
static const bvalue be_ktab_class_BreatheAnimation[17] = {
  /* K0   */  be_nested_str_weak(base_color),
  /* K1   */  be_nested_str_weak(breathe_provider),
  /* K2   */  be_nested_str_weak(min_brightness),
  /* K3   */  be_nested_str_weak(max_brightness),
  /* K4   */  be_nested_str_weak(period),
  /* K5   */  be_nested_str_weak(duration),
  /* K6   */  be_nested_str_weak(curve_factor),
  /* K7   */  be_nested_str_weak(BreatheAnimation_X28base_color_X3D0x_X2508x_X2C_X20min_brightness_X3D_X25s_X2C_X20max_brightness_X3D_X25s_X2C_X20period_X3D_X25s_X2C_X20curve_factor_X3D_X25s_X2C_X20priority_X3D_X25s_X2C_X20running_X3D_X25s_X29),
  /* K8   */  be_nested_str_weak(priority),
  /* K9   */  be_nested_str_weak(is_running),
  /* K10  */  be_nested_str_weak(init),
  /* K11  */  be_nested_str_weak(animation),
  /* K12  */  be_nested_str_weak(breathe_color),
  /* K13  */  be_nested_str_weak(color),
  /* K14  */  be_nested_str_weak(start),
  /* K15  */  be_nested_str_weak(engine),
  /* K16  */  be_nested_str_weak(time_ms),
};


extern const bclass be_class_BreatheAnimation;

/********************************************************************
** Solidified function: on_param_changed
********************************************************************/
be_local_closure(class_BreatheAnimation_on_param_changed,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_BreatheAnimation,     /* shared constants */
    be_str_weak(on_param_changed),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x1C0C0300,  //  0000  EQ	R3	R1	K0
      0x780E0002,  //  0001  JMPF	R3	#0005
      0x880C0101,  //  0002  GETMBR	R3	R0	K1
      0x900E0002,  //  0003  SETMBR	R3	K0	R2
      0x70020012,  //  0004  JMP		#0018
      0x1C0C0302,  //  0005  EQ	R3	R1	K2
      0x780E0002,  //  0006  JMPF	R3	#000A
      0x880C0101,  //  0007  GETMBR	R3	R0	K1
      0x900E0402,  //  0008  SETMBR	R3	K2	R2
      0x7002000D,  //  0009  JMP		#0018
      0x1C0C0303,  //  000A  EQ	R3	R1	K3
      0x780E0002,  //  000B  JMPF	R3	#000F
      0x880C0101,  //  000C  GETMBR	R3	R0	K1
      0x900E0602,  //  000D  SETMBR	R3	K3	R2
      0x70020008,  //  000E  JMP		#0018
      0x1C0C0304,  //  000F  EQ	R3	R1	K4
      0x780E0002,  //  0010  JMPF	R3	#0014
      0x880C0101,  //  0011  GETMBR	R3	R0	K1
      0x900E0A02,  //  0012  SETMBR	R3	K5	R2
      0x70020003,  //  0013  JMP		#0018
      0x1C0C0306,  //  0014  EQ	R3	R1	K6
      0x780E0001,  //  0015  JMPF	R3	#0018
      0x880C0101,  //  0016  GETMBR	R3	R0	K1
      0x900E0C02,  //  0017  SETMBR	R3	K6	R2
      0x80000000,  //  0018  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_BreatheAnimation_tostring,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_BreatheAnimation,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x60040018,  //  0000  GETGBL	R1	G24
      0x58080007,  //  0001  LDCONST	R2	K7
      0x880C0100,  //  0002  GETMBR	R3	R0	K0
      0x88100102,  //  0003  GETMBR	R4	R0	K2
      0x88140103,  //  0004  GETMBR	R5	R0	K3
      0x88180104,  //  0005  GETMBR	R6	R0	K4
      0x881C0106,  //  0006  GETMBR	R7	R0	K6
      0x88200108,  //  0007  GETMBR	R8	R0	K8
      0x88240109,  //  0008  GETMBR	R9	R0	K9
      0x7C041000,  //  0009  CALL	R1	8
      0x80040200,  //  000A  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_BreatheAnimation_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_BreatheAnimation,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C08050A,  //  0003  GETMET	R2	R2	K10
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0xB80A1600,  //  0006  GETNGBL	R2	K11
      0x8C08050C,  //  0007  GETMET	R2	R2	K12
      0x5C100200,  //  0008  MOVE	R4	R1
      0x7C080400,  //  0009  CALL	R2	2
      0x90020202,  //  000A  SETMBR	R0	K1	R2
      0x88080101,  //  000B  GETMBR	R2	R0	K1
      0x90021A02,  //  000C  SETMBR	R0	K13	R2
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(class_BreatheAnimation_start,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_BreatheAnimation,     /* shared constants */
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C08050E,  //  0003  GETMET	R2	R2	K14
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x4C080000,  //  0006  LDNIL	R2
      0x20080202,  //  0007  NE	R2	R1	R2
      0x780A0001,  //  0008  JMPF	R2	#000B
      0x5C080200,  //  0009  MOVE	R2	R1
      0x70020001,  //  000A  JMP		#000D
      0x8808010F,  //  000B  GETMBR	R2	R0	K15
      0x88080510,  //  000C  GETMBR	R2	R2	K16
      0x880C0101,  //  000D  GETMBR	R3	R0	K1
      0x8C0C070E,  //  000E  GETMET	R3	R3	K14
      0x5C140400,  //  000F  MOVE	R5	R2
      0x7C0C0400,  //  0010  CALL	R3	2
      0x80040000,  //  0011  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: BreatheAnimation
********************************************************************/
extern const bclass be_class_Animation;
be_local_class(BreatheAnimation,
    1,
    &be_class_Animation,
    be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(tostring, -1), be_const_closure(class_BreatheAnimation_tostring_closure) },
        { be_const_key_weak(on_param_changed, 0), be_const_closure(class_BreatheAnimation_on_param_changed_closure) },
        { be_const_key_weak(PARAMS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(base_color, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(-1) },
    }))    ) } )) },
        { be_const_key_weak(curve_factor, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(2) },
        { be_const_key_weak(min, -1), be_const_int(1) },
        { be_const_key_weak(max, 1), be_const_int(5) },
    }))    ) } )) },
        { be_const_key_weak(min_brightness, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(0) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(max_brightness, 0), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(255) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(period, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(3000) },
        { be_const_key_weak(min, -1), be_const_int(100) },
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(init, 2), be_const_closure(class_BreatheAnimation_init_closure) },
        { be_const_key_weak(breathe_provider, -1), be_const_var(0) },
        { be_const_key_weak(start, -1), be_const_closure(class_BreatheAnimation_start_closure) },
    })),
    be_str_weak(BreatheAnimation)
);

/********************************************************************
** Solidified function: ease_out
********************************************************************/
be_local_closure(ease_out,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(oscillator_value),
    /* K2   */  be_nested_str_weak(form),
    /* K3   */  be_nested_str_weak(EASE_OUT),
    }),
    be_str_weak(ease_out),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0xB80A0000,  //  0004  GETNGBL	R2	K0
      0x88080503,  //  0005  GETMBR	R2	R2	K3
      0x90060402,  //  0006  SETMBR	R1	K2	R2
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: triangle
********************************************************************/
be_local_closure(triangle,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(oscillator_value),
    /* K2   */  be_nested_str_weak(form),
    /* K3   */  be_nested_str_weak(TRIANGLE),
    }),
    be_str_weak(triangle),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0xB80A0000,  //  0004  GETNGBL	R2	K0
      0x88080503,  //  0005  GETMBR	R2	R2	K3
      0x90060402,  //  0006  SETMBR	R1	K2	R2
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_event_active
********************************************************************/
be_local_closure(set_event_active,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(event_manager),
    /* K2   */  be_nested_str_weak(set_event_active),
    }),
    be_str_weak(set_event_active),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x88080501,  //  0001  GETMBR	R2	R2	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0x5C100000,  //  0003  MOVE	R4	R0
      0x5C140200,  //  0004  MOVE	R5	R1
      0x7C080600,  //  0005  CALL	R2	3
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/

// compact class 'SparkleAnimation' ktab size: 42, total: 76 (saved 272 bytes)
static const bvalue be_ktab_class_SparkleAnimation[42] = {
  /* K0   */  be_const_int(0),
  /* K1   */  be_nested_str_weak(_random),
  /* K2   */  be_nested_str_weak(init),
  /* K3   */  be_nested_str_weak(random_seed),
  /* K4   */  be_nested_str_weak(engine),
  /* K5   */  be_nested_str_weak(time_ms),
  /* K6   */  be_nested_str_weak(current_colors),
  /* K7   */  be_nested_str_weak(sparkle_states),
  /* K8   */  be_nested_str_weak(sparkle_ages),
  /* K9   */  be_nested_str_weak(last_update),
  /* K10  */  be_nested_str_weak(_initialize_buffers),
  /* K11  */  be_nested_str_weak(update),
  /* K12  */  be_nested_str_weak(_update_sparkles),
  /* K13  */  be_nested_str_weak(get_param),
  /* K14  */  be_nested_str_weak(color),
  /* K15  */  be_nested_str_weak(animation),
  /* K16  */  be_nested_str_weak(is_value_provider),
  /* K17  */  be_nested_str_weak(0x_X2508x),
  /* K18  */  be_nested_str_weak(SparkleAnimation_X28color_X3D_X25s_X2C_X20density_X3D_X25s_X2C_X20fade_speed_X3D_X25s_X2C_X20priority_X3D_X25s_X2C_X20running_X3D_X25s_X29),
  /* K19  */  be_nested_str_weak(density),
  /* K20  */  be_nested_str_weak(fade_speed),
  /* K21  */  be_nested_str_weak(priority),
  /* K22  */  be_nested_str_weak(is_running),
  /* K23  */  be_nested_str_weak(get_strip_length),
  /* K24  */  be_nested_str_weak(sparkle_duration),
  /* K25  */  be_nested_str_weak(min_brightness),
  /* K26  */  be_nested_str_weak(max_brightness),
  /* K27  */  be_nested_str_weak(back_color),
  /* K28  */  be_const_int(1),
  /* K29  */  be_nested_str_weak(tasmota),
  /* K30  */  be_nested_str_weak(scale_uint),
  /* K31  */  be_nested_str_weak(_update_sparkle_color),
  /* K32  */  be_nested_str_weak(_random_range),
  /* K33  */  be_const_int(1103515245),
  /* K34  */  be_const_int(2147483647),
  /* K35  */  be_nested_str_weak(resize),
  /* K36  */  be_nested_str_weak(width),
  /* K37  */  be_nested_str_weak(set_pixel_color),
  /* K38  */  be_nested_str_weak(is_color_provider),
  /* K39  */  be_nested_str_weak(get_color_for_value),
  /* K40  */  be_nested_str_weak(get_param_value),
  /* K41  */  be_nested_str_weak(start),
};


extern const bclass be_class_SparkleAnimation;

/********************************************************************
** Solidified function: _random_range
********************************************************************/
be_local_closure(class_SparkleAnimation__random_range,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SparkleAnimation,     /* shared constants */
    be_str_weak(_random_range),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x18080300,  //  0000  LE	R2	R1	K0
      0x780A0000,  //  0001  JMPF	R2	#0003
      0x80060000,  //  0002  RET	1	K0
      0x8C080101,  //  0003  GETMET	R2	R0	K1
      0x7C080200,  //  0004  CALL	R2	1
      0x10080401,  //  0005  MOD	R2	R2	R1
      0x80040400,  //  0006  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_SparkleAnimation_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SparkleAnimation,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080502,  //  0003  GETMET	R2	R2	K2
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x88080104,  //  0006  GETMBR	R2	R0	K4
      0x88080505,  //  0007  GETMBR	R2	R2	K5
      0x540EFFFF,  //  0008  LDINT	R3	65536
      0x10080403,  //  0009  MOD	R2	R2	R3
      0x90020602,  //  000A  SETMBR	R0	K3	R2
      0x60080012,  //  000B  GETGBL	R2	G18
      0x7C080000,  //  000C  CALL	R2	0
      0x90020C02,  //  000D  SETMBR	R0	K6	R2
      0x60080012,  //  000E  GETGBL	R2	G18
      0x7C080000,  //  000F  CALL	R2	0
      0x90020E02,  //  0010  SETMBR	R0	K7	R2
      0x60080012,  //  0011  GETGBL	R2	G18
      0x7C080000,  //  0012  CALL	R2	0
      0x90021002,  //  0013  SETMBR	R0	K8	R2
      0x90021300,  //  0014  SETMBR	R0	K9	K0
      0x8C08010A,  //  0015  GETMET	R2	R0	K10
      0x7C080200,  //  0016  CALL	R2	1
      0x80000000,  //  0017  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update
********************************************************************/
be_local_closure(class_SparkleAnimation_update,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SparkleAnimation,     /* shared constants */
    be_str_weak(update),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C08050B,  //  0003  GETMET	R2	R2	K11
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x540A0020,  //  0006  LDINT	R2	33
      0x880C0109,  //  0007  GETMBR	R3	R0	K9
      0x040C0203,  //  0008  SUB	R3	R1	R3
      0x140C0602,  //  0009  LT	R3	R3	R2
      0x780E0001,  //  000A  JMPF	R3	#000D
      0x500C0200,  //  000B  LDBOOL	R3	1	0
      0x80040600,  //  000C  RET	1	R3
      0x90021201,  //  000D  SETMBR	R0	K9	R1
      0x8C0C010C,  //  000E  GETMET	R3	R0	K12
      0x5C140200,  //  000F  MOVE	R5	R1
      0x7C0C0400,  //  0010  CALL	R3	2
      0x500C0200,  //  0011  LDBOOL	R3	1	0
      0x80040600,  //  0012  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_SparkleAnimation_tostring,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SparkleAnimation,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x8C04010D,  //  0000  GETMET	R1	R0	K13
      0x580C000E,  //  0001  LDCONST	R3	K14
      0x7C040400,  //  0002  CALL	R1	2
      0x4C080000,  //  0003  LDNIL	R2
      0xB80E1E00,  //  0004  GETNGBL	R3	K15
      0x8C0C0710,  //  0005  GETMET	R3	R3	K16
      0x5C140200,  //  0006  MOVE	R5	R1
      0x7C0C0400,  //  0007  CALL	R3	2
      0x780E0004,  //  0008  JMPF	R3	#000E
      0x600C0008,  //  0009  GETGBL	R3	G8
      0x5C100200,  //  000A  MOVE	R4	R1
      0x7C0C0200,  //  000B  CALL	R3	1
      0x5C080600,  //  000C  MOVE	R2	R3
      0x70020004,  //  000D  JMP		#0013
      0x600C0018,  //  000E  GETGBL	R3	G24
      0x58100011,  //  000F  LDCONST	R4	K17
      0x8814010E,  //  0010  GETMBR	R5	R0	K14
      0x7C0C0400,  //  0011  CALL	R3	2
      0x5C080600,  //  0012  MOVE	R2	R3
      0x600C0018,  //  0013  GETGBL	R3	G24
      0x58100012,  //  0014  LDCONST	R4	K18
      0x5C140400,  //  0015  MOVE	R5	R2
      0x88180113,  //  0016  GETMBR	R6	R0	K19
      0x881C0114,  //  0017  GETMBR	R7	R0	K20
      0x88200115,  //  0018  GETMBR	R8	R0	K21
      0x88240116,  //  0019  GETMBR	R9	R0	K22
      0x7C0C0C00,  //  001A  CALL	R3	6
      0x80040600,  //  001B  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _update_sparkles
********************************************************************/
be_local_closure(class_SparkleAnimation__update_sparkles,   /* name */
  be_nested_proto(
    19,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SparkleAnimation,     /* shared constants */
    be_str_weak(_update_sparkles),
    &be_const_str_solidified,
    ( &(const binstruction[100]) {  /* code */
      0x88080104,  //  0000  GETMBR	R2	R0	K4
      0x8C080517,  //  0001  GETMET	R2	R2	K23
      0x7C080200,  //  0002  CALL	R2	1
      0x880C0118,  //  0003  GETMBR	R3	R0	K24
      0x88100114,  //  0004  GETMBR	R4	R0	K20
      0x88140113,  //  0005  GETMBR	R5	R0	K19
      0x88180119,  //  0006  GETMBR	R6	R0	K25
      0x881C011A,  //  0007  GETMBR	R7	R0	K26
      0x8820011B,  //  0008  GETMBR	R8	R0	K27
      0x58240000,  //  0009  LDCONST	R9	K0
      0x14281202,  //  000A  LT	R10	R9	R2
      0x782A0056,  //  000B  JMPF	R10	#0063
      0x88280107,  //  000C  GETMBR	R10	R0	K7
      0x94281409,  //  000D  GETIDX	R10	R10	R9
      0x24281500,  //  000E  GT	R10	R10	K0
      0x782A003A,  //  000F  JMPF	R10	#004B
      0x88280108,  //  0010  GETMBR	R10	R0	K8
      0x942C1409,  //  0011  GETIDX	R11	R10	R9
      0x002C171C,  //  0012  ADD	R11	R11	K28
      0x9828120B,  //  0013  SETIDX	R10	R9	R11
      0x88280108,  //  0014  GETMBR	R10	R0	K8
      0x94281409,  //  0015  GETIDX	R10	R10	R9
      0x28281403,  //  0016  GE	R10	R10	R3
      0x782A0006,  //  0017  JMPF	R10	#001F
      0x88280107,  //  0018  GETMBR	R10	R0	K7
      0x98281300,  //  0019  SETIDX	R10	R9	K0
      0x88280108,  //  001A  GETMBR	R10	R0	K8
      0x98281300,  //  001B  SETIDX	R10	R9	K0
      0x88280106,  //  001C  GETMBR	R10	R0	K6
      0x98281208,  //  001D  SETIDX	R10	R9	R8
      0x7002002A,  //  001E  JMP		#004A
      0xB82A3A00,  //  001F  GETNGBL	R10	K29
      0x8C28151E,  //  0020  GETMET	R10	R10	K30
      0x88300108,  //  0021  GETMBR	R12	R0	K8
      0x94301809,  //  0022  GETIDX	R12	R12	R9
      0x58340000,  //  0023  LDCONST	R13	K0
      0x5C380600,  //  0024  MOVE	R14	R3
      0x583C0000,  //  0025  LDCONST	R15	K0
      0x544200FE,  //  0026  LDINT	R16	255
      0x7C280C00,  //  0027  CALL	R10	6
      0x542E00FE,  //  0028  LDINT	R11	255
      0xB8323A00,  //  0029  GETNGBL	R12	K29
      0x8C30191E,  //  002A  GETMET	R12	R12	K30
      0x5C381400,  //  002B  MOVE	R14	R10
      0x583C0000,  //  002C  LDCONST	R15	K0
      0x544200FE,  //  002D  LDINT	R16	255
      0x58440000,  //  002E  LDCONST	R17	K0
      0x5C480800,  //  002F  MOVE	R18	R4
      0x7C300C00,  //  0030  CALL	R12	6
      0x042C160C,  //  0031  SUB	R11	R11	R12
      0xB8323A00,  //  0032  GETNGBL	R12	K29
      0x8C30191E,  //  0033  GETMET	R12	R12	K30
      0x88380107,  //  0034  GETMBR	R14	R0	K7
      0x94381C09,  //  0035  GETIDX	R14	R14	R9
      0x583C0000,  //  0036  LDCONST	R15	K0
      0x544200FE,  //  0037  LDINT	R16	255
      0x58440000,  //  0038  LDCONST	R17	K0
      0x5C481600,  //  0039  MOVE	R18	R11
      0x7C300C00,  //  003A  CALL	R12	6
      0x54360009,  //  003B  LDINT	R13	10
      0x1434180D,  //  003C  LT	R13	R12	R13
      0x78360006,  //  003D  JMPF	R13	#0045
      0x88340107,  //  003E  GETMBR	R13	R0	K7
      0x98341300,  //  003F  SETIDX	R13	R9	K0
      0x88340108,  //  0040  GETMBR	R13	R0	K8
      0x98341300,  //  0041  SETIDX	R13	R9	K0
      0x88340106,  //  0042  GETMBR	R13	R0	K6
      0x98341208,  //  0043  SETIDX	R13	R9	R8
      0x70020004,  //  0044  JMP		#004A
      0x8C34011F,  //  0045  GETMET	R13	R0	K31
      0x5C3C1200,  //  0046  MOVE	R15	R9
      0x5C401800,  //  0047  MOVE	R16	R12
      0x5C440200,  //  0048  MOVE	R17	R1
      0x7C340800,  //  0049  CALL	R13	4
      0x70020015,  //  004A  JMP		#0061
      0x8C280120,  //  004B  GETMET	R10	R0	K32
      0x543200FF,  //  004C  LDINT	R12	256
      0x7C280400,  //  004D  CALL	R10	2
      0x14281405,  //  004E  LT	R10	R10	R5
      0x782A000E,  //  004F  JMPF	R10	#005F
      0x8C280120,  //  0050  GETMET	R10	R0	K32
      0x04300E06,  //  0051  SUB	R12	R7	R6
      0x0030191C,  //  0052  ADD	R12	R12	K28
      0x7C280400,  //  0053  CALL	R10	2
      0x00280C0A,  //  0054  ADD	R10	R6	R10
      0x882C0107,  //  0055  GETMBR	R11	R0	K7
      0x982C120A,  //  0056  SETIDX	R11	R9	R10
      0x882C0108,  //  0057  GETMBR	R11	R0	K8
      0x982C1300,  //  0058  SETIDX	R11	R9	K0
      0x8C2C011F,  //  0059  GETMET	R11	R0	K31
      0x5C341200,  //  005A  MOVE	R13	R9
      0x5C381400,  //  005B  MOVE	R14	R10
      0x5C3C0200,  //  005C  MOVE	R15	R1
      0x7C2C0800,  //  005D  CALL	R11	4
      0x70020001,  //  005E  JMP		#0061
      0x88280106,  //  005F  GETMBR	R10	R0	K6
      0x98281208,  //  0060  SETIDX	R10	R9	R8
      0x0024131C,  //  0061  ADD	R9	R9	K28
      0x7001FFA6,  //  0062  JMP		#000A
      0x80000000,  //  0063  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _random
********************************************************************/
be_local_closure(class_SparkleAnimation__random,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SparkleAnimation,     /* shared constants */
    be_str_weak(_random),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88040103,  //  0000  GETMBR	R1	R0	K3
      0x08040321,  //  0001  MUL	R1	R1	K33
      0x540A3038,  //  0002  LDINT	R2	12345
      0x00040202,  //  0003  ADD	R1	R1	R2
      0x2C040322,  //  0004  AND	R1	R1	K34
      0x90020601,  //  0005  SETMBR	R0	K3	R1
      0x88040103,  //  0006  GETMBR	R1	R0	K3
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _initialize_buffers
********************************************************************/
be_local_closure(class_SparkleAnimation__initialize_buffers,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SparkleAnimation,     /* shared constants */
    be_str_weak(_initialize_buffers),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x8C040317,  //  0001  GETMET	R1	R1	K23
      0x7C040200,  //  0002  CALL	R1	1
      0x88080106,  //  0003  GETMBR	R2	R0	K6
      0x8C080523,  //  0004  GETMET	R2	R2	K35
      0x5C100200,  //  0005  MOVE	R4	R1
      0x7C080400,  //  0006  CALL	R2	2
      0x88080107,  //  0007  GETMBR	R2	R0	K7
      0x8C080523,  //  0008  GETMET	R2	R2	K35
      0x5C100200,  //  0009  MOVE	R4	R1
      0x7C080400,  //  000A  CALL	R2	2
      0x88080108,  //  000B  GETMBR	R2	R0	K8
      0x8C080523,  //  000C  GETMET	R2	R2	K35
      0x5C100200,  //  000D  MOVE	R4	R1
      0x7C080400,  //  000E  CALL	R2	2
      0x8808011B,  //  000F  GETMBR	R2	R0	K27
      0x580C0000,  //  0010  LDCONST	R3	K0
      0x14100601,  //  0011  LT	R4	R3	R1
      0x78120007,  //  0012  JMPF	R4	#001B
      0x88100106,  //  0013  GETMBR	R4	R0	K6
      0x98100602,  //  0014  SETIDX	R4	R3	R2
      0x88100107,  //  0015  GETMBR	R4	R0	K7
      0x98100700,  //  0016  SETIDX	R4	R3	K0
      0x88100108,  //  0017  GETMBR	R4	R0	K8
      0x98100700,  //  0018  SETIDX	R4	R3	K0
      0x000C071C,  //  0019  ADD	R3	R3	K28
      0x7001FFF5,  //  001A  JMP		#0011
      0x80000000,  //  001B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: render
********************************************************************/
be_local_closure(class_SparkleAnimation_render,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SparkleAnimation,     /* shared constants */
    be_str_weak(render),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x880C0116,  //  0000  GETMBR	R3	R0	K22
      0x780E0002,  //  0001  JMPF	R3	#0005
      0x4C0C0000,  //  0002  LDNIL	R3
      0x1C0C0203,  //  0003  EQ	R3	R1	R3
      0x780E0001,  //  0004  JMPF	R3	#0007
      0x500C0000,  //  0005  LDBOOL	R3	0	0
      0x80040600,  //  0006  RET	1	R3
      0x880C0104,  //  0007  GETMBR	R3	R0	K4
      0x8C0C0717,  //  0008  GETMET	R3	R3	K23
      0x7C0C0200,  //  0009  CALL	R3	1
      0x58100000,  //  000A  LDCONST	R4	K0
      0x14140803,  //  000B  LT	R5	R4	R3
      0x78160009,  //  000C  JMPF	R5	#0017
      0x88140324,  //  000D  GETMBR	R5	R1	K36
      0x14140805,  //  000E  LT	R5	R4	R5
      0x78160004,  //  000F  JMPF	R5	#0015
      0x8C140325,  //  0010  GETMET	R5	R1	K37
      0x5C1C0800,  //  0011  MOVE	R7	R4
      0x88200106,  //  0012  GETMBR	R8	R0	K6
      0x94201004,  //  0013  GETIDX	R8	R8	R4
      0x7C140600,  //  0014  CALL	R5	3
      0x0010091C,  //  0015  ADD	R4	R4	K28
      0x7001FFF3,  //  0016  JMP		#000B
      0x50140200,  //  0017  LDBOOL	R5	1	0
      0x80040A00,  //  0018  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _update_sparkle_color
********************************************************************/
be_local_closure(class_SparkleAnimation__update_sparkle_color,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SparkleAnimation,     /* shared constants */
    be_str_weak(_update_sparkle_color),
    &be_const_str_solidified,
    ( &(const binstruction[77]) {  /* code */
      0x5411FFFE,  //  0000  LDINT	R4	-1
      0x8814010E,  //  0001  GETMBR	R5	R0	K14
      0xB81A1E00,  //  0002  GETNGBL	R6	K15
      0x8C180D26,  //  0003  GETMET	R6	R6	K38
      0x5C200A00,  //  0004  MOVE	R8	R5
      0x7C180400,  //  0005  CALL	R6	2
      0x781A0009,  //  0006  JMPF	R6	#0011
      0x88180B27,  //  0007  GETMBR	R6	R5	K39
      0x4C1C0000,  //  0008  LDNIL	R7
      0x20180C07,  //  0009  NE	R6	R6	R7
      0x781A0005,  //  000A  JMPF	R6	#0011
      0x8C180B27,  //  000B  GETMET	R6	R5	K39
      0x5C200400,  //  000C  MOVE	R8	R2
      0x58240000,  //  000D  LDCONST	R9	K0
      0x7C180600,  //  000E  CALL	R6	3
      0x5C100C00,  //  000F  MOVE	R4	R6
      0x70020006,  //  0010  JMP		#0018
      0x8C180128,  //  0011  GETMET	R6	R0	K40
      0x5820000E,  //  0012  LDCONST	R8	K14
      0x54260009,  //  0013  LDINT	R9	10
      0x08240209,  //  0014  MUL	R9	R1	R9
      0x00240609,  //  0015  ADD	R9	R3	R9
      0x7C180600,  //  0016  CALL	R6	3
      0x5C100C00,  //  0017  MOVE	R4	R6
      0x541A0017,  //  0018  LDINT	R6	24
      0x3C180806,  //  0019  SHR	R6	R4	R6
      0x541E00FE,  //  001A  LDINT	R7	255
      0x2C180C07,  //  001B  AND	R6	R6	R7
      0x541E000F,  //  001C  LDINT	R7	16
      0x3C1C0807,  //  001D  SHR	R7	R4	R7
      0x542200FE,  //  001E  LDINT	R8	255
      0x2C1C0E08,  //  001F  AND	R7	R7	R8
      0x54220007,  //  0020  LDINT	R8	8
      0x3C200808,  //  0021  SHR	R8	R4	R8
      0x542600FE,  //  0022  LDINT	R9	255
      0x2C201009,  //  0023  AND	R8	R8	R9
      0x542600FE,  //  0024  LDINT	R9	255
      0x2C240809,  //  0025  AND	R9	R4	R9
      0xB82A3A00,  //  0026  GETNGBL	R10	K29
      0x8C28151E,  //  0027  GETMET	R10	R10	K30
      0x5C300400,  //  0028  MOVE	R12	R2
      0x58340000,  //  0029  LDCONST	R13	K0
      0x543A00FE,  //  002A  LDINT	R14	255
      0x583C0000,  //  002B  LDCONST	R15	K0
      0x5C400E00,  //  002C  MOVE	R16	R7
      0x7C280C00,  //  002D  CALL	R10	6
      0x5C1C1400,  //  002E  MOVE	R7	R10
      0xB82A3A00,  //  002F  GETNGBL	R10	K29
      0x8C28151E,  //  0030  GETMET	R10	R10	K30
      0x5C300400,  //  0031  MOVE	R12	R2
      0x58340000,  //  0032  LDCONST	R13	K0
      0x543A00FE,  //  0033  LDINT	R14	255
      0x583C0000,  //  0034  LDCONST	R15	K0
      0x5C401000,  //  0035  MOVE	R16	R8
      0x7C280C00,  //  0036  CALL	R10	6
      0x5C201400,  //  0037  MOVE	R8	R10
      0xB82A3A00,  //  0038  GETNGBL	R10	K29
      0x8C28151E,  //  0039  GETMET	R10	R10	K30
      0x5C300400,  //  003A  MOVE	R12	R2
      0x58340000,  //  003B  LDCONST	R13	K0
      0x543A00FE,  //  003C  LDINT	R14	255
      0x583C0000,  //  003D  LDCONST	R15	K0
      0x5C401200,  //  003E  MOVE	R16	R9
      0x7C280C00,  //  003F  CALL	R10	6
      0x5C241400,  //  0040  MOVE	R9	R10
      0x88280106,  //  0041  GETMBR	R10	R0	K6
      0x542E0017,  //  0042  LDINT	R11	24
      0x382C0C0B,  //  0043  SHL	R11	R6	R11
      0x5432000F,  //  0044  LDINT	R12	16
      0x38300E0C,  //  0045  SHL	R12	R7	R12
      0x302C160C,  //  0046  OR	R11	R11	R12
      0x54320007,  //  0047  LDINT	R12	8
      0x3830100C,  //  0048  SHL	R12	R8	R12
      0x302C160C,  //  0049  OR	R11	R11	R12
      0x302C1609,  //  004A  OR	R11	R11	R9
      0x9828020B,  //  004B  SETIDX	R10	R1	R11
      0x80000000,  //  004C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(class_SparkleAnimation_start,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SparkleAnimation,     /* shared constants */
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080529,  //  0003  GETMET	R2	R2	K41
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x88080104,  //  0006  GETMBR	R2	R0	K4
      0x88080505,  //  0007  GETMBR	R2	R2	K5
      0x540EFFFF,  //  0008  LDINT	R3	65536
      0x10080403,  //  0009  MOD	R2	R2	R3
      0x90020602,  //  000A  SETMBR	R0	K3	R2
      0x8C08010A,  //  000B  GETMET	R2	R0	K10
      0x7C080200,  //  000C  CALL	R2	1
      0x80040000,  //  000D  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: SparkleAnimation
********************************************************************/
extern const bclass be_class_Animation;
be_local_class(SparkleAnimation,
    5,
    &be_class_Animation,
    be_nested_map(16,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(sparkle_states, -1), be_const_var(1) },
        { be_const_key_weak(start, -1), be_const_closure(class_SparkleAnimation_start_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_SparkleAnimation_init_closure) },
        { be_const_key_weak(PARAMS, 2), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(fade_speed, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(50) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(density, 5), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(30) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(color, 4), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(-1) },
    }))    ) } )) },
        { be_const_key_weak(max_brightness, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(255) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(sparkle_duration, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(60) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(back_color, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(-16777216) },
    }))    ) } )) },
        { be_const_key_weak(min_brightness, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(100) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(update, -1), be_const_closure(class_SparkleAnimation_update_closure) },
        { be_const_key_weak(tostring, -1), be_const_closure(class_SparkleAnimation_tostring_closure) },
        { be_const_key_weak(sparkle_ages, -1), be_const_var(2) },
        { be_const_key_weak(current_colors, -1), be_const_var(0) },
        { be_const_key_weak(last_update, -1), be_const_var(4) },
        { be_const_key_weak(_random, -1), be_const_closure(class_SparkleAnimation__random_closure) },
        { be_const_key_weak(random_seed, -1), be_const_var(3) },
        { be_const_key_weak(_update_sparkles, 8), be_const_closure(class_SparkleAnimation__update_sparkles_closure) },
        { be_const_key_weak(_initialize_buffers, -1), be_const_closure(class_SparkleAnimation__initialize_buffers_closure) },
        { be_const_key_weak(render, -1), be_const_closure(class_SparkleAnimation_render_closure) },
        { be_const_key_weak(_update_sparkle_color, -1), be_const_closure(class_SparkleAnimation__update_sparkle_color_closure) },
        { be_const_key_weak(_random_range, 1), be_const_closure(class_SparkleAnimation__random_range_closure) },
    })),
    be_str_weak(SparkleAnimation)
);

/********************************************************************
** Solidified function: shift_fast_scroll
********************************************************************/
be_local_closure(shift_fast_scroll,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(shift_animation),
    /* K2   */  be_nested_str_weak(direction),
    /* K3   */  be_const_int(1),
    /* K4   */  be_nested_str_weak(shift_speed),
    /* K5   */  be_nested_str_weak(wrap_around),
    }),
    be_str_weak(shift_fast_scroll),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x90060503,  //  0004  SETMBR	R1	K2	K3
      0x540A00C7,  //  0005  LDINT	R2	200
      0x90060802,  //  0006  SETMBR	R1	K4	R2
      0x50080200,  //  0007  LDBOOL	R2	1	0
      0x90060A02,  //  0008  SETMBR	R1	K5	R2
      0x80040200,  //  0009  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: bounce_basic
********************************************************************/
be_local_closure(bounce_basic,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(bounce_animation),
    /* K2   */  be_nested_str_weak(bounce_speed),
    /* K3   */  be_nested_str_weak(bounce_range),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(damping),
    /* K6   */  be_nested_str_weak(gravity),
    /* K7   */  be_nested_str_weak(name),
    /* K8   */  be_nested_str_weak(bounce_basic),
    }),
    be_str_weak(bounce_basic),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x540A007F,  //  0004  LDINT	R2	128
      0x90060402,  //  0005  SETMBR	R1	K2	R2
      0x90060704,  //  0006  SETMBR	R1	K3	K4
      0x540A00F9,  //  0007  LDINT	R2	250
      0x90060A02,  //  0008  SETMBR	R1	K5	R2
      0x90060D04,  //  0009  SETMBR	R1	K6	K4
      0x90060F08,  //  000A  SETMBR	R1	K7	K8
      0x80040200,  //  000B  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: animation_init_strip
********************************************************************/
be_local_closure(animation_init_strip,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    1,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(global),
    /* K1   */  be_nested_str_weak(animation),
    /* K2   */  be_nested_str_weak(introspect),
    /* K3   */  be_nested_str_weak(contains),
    /* K4   */  be_nested_str_weak(_engines),
    /* K5   */  be_nested_str_weak(find),
    /* K6   */  be_nested_str_weak(stop),
    /* K7   */  be_nested_str_weak(clear),
    /* K8   */  be_nested_str_weak(Leds),
    /* K9   */  be_nested_str_weak(create_engine),
    }),
    be_str_weak(animation_init_strip),
    &be_const_str_solidified,
    ( &(const binstruction[37]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0xA40E0400,  //  0002  IMPORT	R3	K2
      0x8C100703,  //  0003  GETMET	R4	R3	K3
      0x5C180400,  //  0004  MOVE	R6	R2
      0x581C0004,  //  0005  LDCONST	R7	K4
      0x7C100600,  //  0006  CALL	R4	3
      0x74120002,  //  0007  JMPT	R4	#000B
      0x60100013,  //  0008  GETGBL	R4	G19
      0x7C100000,  //  0009  CALL	R4	0
      0x900A0804,  //  000A  SETMBR	R2	K4	R4
      0x60100008,  //  000B  GETGBL	R4	G8
      0x5C140000,  //  000C  MOVE	R5	R0
      0x7C100200,  //  000D  CALL	R4	1
      0x88140504,  //  000E  GETMBR	R5	R2	K4
      0x8C140B05,  //  000F  GETMET	R5	R5	K5
      0x5C1C0800,  //  0010  MOVE	R7	R4
      0x7C140400,  //  0011  CALL	R5	2
      0x4C180000,  //  0012  LDNIL	R6
      0x20180A06,  //  0013  NE	R6	R5	R6
      0x781A0004,  //  0014  JMPF	R6	#001A
      0x8C180B06,  //  0015  GETMET	R6	R5	K6
      0x7C180200,  //  0016  CALL	R6	1
      0x8C180B07,  //  0017  GETMET	R6	R5	K7
      0x7C180200,  //  0018  CALL	R6	1
      0x70020009,  //  0019  JMP		#0024
      0x60180016,  //  001A  GETGBL	R6	G22
      0x881C0308,  //  001B  GETMBR	R7	R1	K8
      0x5C200000,  //  001C  MOVE	R8	R0
      0x7C180400,  //  001D  CALL	R6	2
      0x8C1C0509,  //  001E  GETMET	R7	R2	K9
      0x5C240C00,  //  001F  MOVE	R9	R6
      0x7C1C0400,  //  0020  CALL	R7	2
      0x5C140E00,  //  0021  MOVE	R5	R7
      0x881C0504,  //  0022  GETMBR	R7	R2	K4
      0x981C0805,  //  0023  SETIDX	R7	R4	R5
      0x80040A00,  //  0024  RET	1	R5
    })
  )
);
/*******************************************************************/

// compact class 'RichPaletteAnimation' ktab size: 18, total: 23 (saved 40 bytes)
static const bvalue be_ktab_class_RichPaletteAnimation[18] = {
  /* K0   */  be_nested_str_weak(palette),
  /* K1   */  be_nested_str_weak(cycle_period),
  /* K2   */  be_nested_str_weak(transition_type),
  /* K3   */  be_nested_str_weak(brightness),
  /* K4   */  be_nested_str_weak(range_min),
  /* K5   */  be_nested_str_weak(range_max),
  /* K6   */  be_nested_str_weak(color_provider),
  /* K7   */  be_nested_str_weak(set_param),
  /* K8   */  be_nested_str_weak(on_param_changed),
  /* K9   */  be_nested_str_weak(RichPaletteAnimation_X28_X25s_X2C_X20cycle_period_X3D_X25s_X2C_X20brightness_X3D_X25s_X29),
  /* K10  */  be_nested_str_weak(name),
  /* K11  */  be_nested_str_weak(RichPaletteAnimation_X28uninitialized_X29),
  /* K12  */  be_nested_str_weak(init),
  /* K13  */  be_nested_str_weak(rich_palette),
  /* K14  */  be_nested_str_weak(animation),
  /* K15  */  be_nested_str_weak(values),
  /* K16  */  be_nested_str_weak(color),
  /* K17  */  be_nested_str_weak(start),
};


extern const bclass be_class_RichPaletteAnimation;

/********************************************************************
** Solidified function: on_param_changed
********************************************************************/
be_local_closure(class_RichPaletteAnimation_on_param_changed,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_RichPaletteAnimation,     /* shared constants */
    be_str_weak(on_param_changed),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x1C0C0300,  //  0000  EQ	R3	R1	K0
      0x740E0009,  //  0001  JMPT	R3	#000C
      0x1C0C0301,  //  0002  EQ	R3	R1	K1
      0x740E0007,  //  0003  JMPT	R3	#000C
      0x1C0C0302,  //  0004  EQ	R3	R1	K2
      0x740E0005,  //  0005  JMPT	R3	#000C
      0x1C0C0303,  //  0006  EQ	R3	R1	K3
      0x740E0003,  //  0007  JMPT	R3	#000C
      0x1C0C0304,  //  0008  EQ	R3	R1	K4
      0x740E0001,  //  0009  JMPT	R3	#000C
      0x1C0C0305,  //  000A  EQ	R3	R1	K5
      0x780E0005,  //  000B  JMPF	R3	#0012
      0x880C0106,  //  000C  GETMBR	R3	R0	K6
      0x8C0C0707,  //  000D  GETMET	R3	R3	K7
      0x5C140200,  //  000E  MOVE	R5	R1
      0x5C180400,  //  000F  MOVE	R6	R2
      0x7C0C0600,  //  0010  CALL	R3	3
      0x70020006,  //  0011  JMP		#0019
      0x600C0003,  //  0012  GETGBL	R3	G3
      0x5C100000,  //  0013  MOVE	R4	R0
      0x7C0C0200,  //  0014  CALL	R3	1
      0x8C0C0708,  //  0015  GETMET	R3	R3	K8
      0x5C140200,  //  0016  MOVE	R5	R1
      0x5C180400,  //  0017  MOVE	R6	R2
      0x7C0C0600,  //  0018  CALL	R3	3
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_RichPaletteAnimation_tostring,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_RichPaletteAnimation,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0xA8020009,  //  0000  EXBLK	0	#000B
      0x60040018,  //  0001  GETGBL	R1	G24
      0x58080009,  //  0002  LDCONST	R2	K9
      0x880C010A,  //  0003  GETMBR	R3	R0	K10
      0x88100101,  //  0004  GETMBR	R4	R0	K1
      0x88140103,  //  0005  GETMBR	R5	R0	K3
      0x7C040800,  //  0006  CALL	R1	4
      0xA8040001,  //  0007  EXBLK	1	1
      0x80040200,  //  0008  RET	1	R1
      0xA8040001,  //  0009  EXBLK	1	1
      0x70020004,  //  000A  JMP		#0010
      0xAC040000,  //  000B  CATCH	R1	0	0
      0x70020001,  //  000C  JMP		#000F
      0x80061600,  //  000D  RET	1	K11
      0x70020000,  //  000E  JMP		#0010
      0xB0080000,  //  000F  RAISE	2	R0	R0
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_RichPaletteAnimation_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_RichPaletteAnimation,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C08050C,  //  0003  GETMET	R2	R2	K12
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x9002150D,  //  0006  SETMBR	R0	K10	K13
      0xB80A1C00,  //  0007  GETNGBL	R2	K14
      0x8C08050D,  //  0008  GETMET	R2	R2	K13
      0x5C100200,  //  0009  MOVE	R4	R1
      0x7C080400,  //  000A  CALL	R2	2
      0x90020C02,  //  000B  SETMBR	R0	K6	R2
      0x8808010F,  //  000C  GETMBR	R2	R0	K15
      0x880C0106,  //  000D  GETMBR	R3	R0	K6
      0x980A2003,  //  000E  SETIDX	R2	K16	R3
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(class_RichPaletteAnimation_start,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_RichPaletteAnimation,     /* shared constants */
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080511,  //  0003  GETMET	R2	R2	K17
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x88080106,  //  0006  GETMBR	R2	R0	K6
      0x8C080511,  //  0007  GETMET	R2	R2	K17
      0x5C100200,  //  0008  MOVE	R4	R1
      0x7C080400,  //  0009  CALL	R2	2
      0x80040000,  //  000A  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: RichPaletteAnimation
********************************************************************/
extern const bclass be_class_Animation;
be_local_class(RichPaletteAnimation,
    1,
    &be_class_Animation,
    be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(tostring, -1), be_const_closure(class_RichPaletteAnimation_tostring_closure) },
        { be_const_key_weak(on_param_changed, 0), be_const_closure(class_RichPaletteAnimation_on_param_changed_closure) },
        { be_const_key_weak(PARAMS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(range_min, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(0) },
    }))    ) } )) },
        { be_const_key_weak(transition_type, 0), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(enum, 1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(2,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(1),
        be_const_int(5),
    }))    ) } )) },
        { be_const_key_weak(default, -1), be_const_int(5) },
    }))    ) } )) },
        { be_const_key_weak(palette, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_nil() },
        { be_const_key_weak(type, -1), be_nested_str_weak(instance) },
    }))    ) } )) },
        { be_const_key_weak(cycle_period, 1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(5000) },
        { be_const_key_weak(min, -1), be_const_int(0) },
    }))    ) } )) },
        { be_const_key_weak(brightness, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(255) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(range_max, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(100) },
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(init, 2), be_const_closure(class_RichPaletteAnimation_init_closure) },
        { be_const_key_weak(color_provider, -1), be_const_var(0) },
        { be_const_key_weak(start, -1), be_const_closure(class_RichPaletteAnimation_start_closure) },
    })),
    be_str_weak(RichPaletteAnimation)
);

/********************************************************************
** Solidified function: jitter_brightness
********************************************************************/
be_local_closure(jitter_brightness,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(jitter_animation),
    /* K2   */  be_nested_str_weak(jitter_type),
    /* K3   */  be_const_int(2),
    /* K4   */  be_nested_str_weak(brightness_range),
    }),
    be_str_weak(jitter_brightness),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x90060503,  //  0004  SETMBR	R1	K2	K3
      0x540A0027,  //  0005  LDINT	R2	40
      0x90060802,  //  0006  SETMBR	R1	K4	R2
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: twinkle_solid
********************************************************************/
be_local_closure(twinkle_solid,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(twinkle_animation),
    /* K2   */  be_nested_str_weak(color),
    /* K3   */  be_const_int(-16744193),
    /* K4   */  be_nested_str_weak(density),
    /* K5   */  be_nested_str_weak(twinkle_speed),
    /* K6   */  be_nested_str_weak(fade_speed),
    /* K7   */  be_nested_str_weak(min_brightness),
    /* K8   */  be_nested_str_weak(max_brightness),
    }),
    be_str_weak(twinkle_solid),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x90060503,  //  0004  SETMBR	R1	K2	K3
      0x540A0063,  //  0005  LDINT	R2	100
      0x90060802,  //  0006  SETMBR	R1	K4	R2
      0x540A0005,  //  0007  LDINT	R2	6
      0x90060A02,  //  0008  SETMBR	R1	K5	R2
      0x540A00B3,  //  0009  LDINT	R2	180
      0x90060C02,  //  000A  SETMBR	R1	K6	R2
      0x540A001F,  //  000B  LDINT	R2	32
      0x90060E02,  //  000C  SETMBR	R1	K7	R2
      0x540A00FE,  //  000D  LDINT	R2	255
      0x90061002,  //  000E  SETMBR	R1	K8	R2
      0x80040200,  //  000F  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_user_function
********************************************************************/
be_local_closure(get_user_function,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(global),
    /* K1   */  be_nested_str_weak(_animation_user_functions),
    /* K2   */  be_nested_str_weak(find),
    }),
    be_str_weak(get_user_function),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x88080301,  //  0001  GETMBR	R2	R1	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0x5C100000,  //  0003  MOVE	R4	R0
      0x7C080400,  //  0004  CALL	R2	2
      0x80040400,  //  0005  RET	1	R2
    })
  )
);
/*******************************************************************/

// compact class 'EventManager' ktab size: 30, total: 61 (saved 248 bytes)
static const bvalue be_ktab_class_EventManager[30] = {
  /* K0   */  be_nested_str_weak(event_name),
  /* K1   */  be_nested_str_weak(_X2A),
  /* K2   */  be_nested_str_weak(global_handlers),
  /* K3   */  be_nested_str_weak(find),
  /* K4   */  be_nested_str_weak(remove),
  /* K5   */  be_nested_str_weak(handlers),
  /* K6   */  be_nested_str_weak(set_active),
  /* K7   */  be_nested_str_weak(stop_iteration),
  /* K8   */  be_nested_str_weak(push),
  /* K9   */  be_nested_str_weak(get_info),
  /* K10  */  be_nested_str_weak(keys),
  /* K11  */  be_nested_str_weak(event_queue),
  /* K12  */  be_nested_str_weak(is_processing),
  /* K13  */  be_nested_str_weak(clear),
  /* K14  */  be_const_int(1),
  /* K15  */  be_const_int(0),
  /* K16  */  be_nested_str_weak(priority),
  /* K17  */  be_nested_str_weak(animation),
  /* K18  */  be_nested_str_weak(event_handler),
  /* K19  */  be_nested_str_weak(_sort_handlers),
  /* K20  */  be_nested_str_weak(contains),
  /* K21  */  be_nested_str_weak(name),
  /* K22  */  be_nested_str_weak(data),
  /* K23  */  be_nested_str_weak(is_active),
  /* K24  */  be_nested_str_weak(execute),
  /* K25  */  be_nested_str_weak(Event_X20processing_X20error_X3A),
  /* K26  */  be_nested_str_weak(_process_queued_events),
  /* K27  */  be_nested_str_weak(size),
  /* K28  */  be_nested_str_weak(pop),
  /* K29  */  be_nested_str_weak(trigger_event),
};


extern const bclass be_class_EventManager;

/********************************************************************
** Solidified function: unregister_handler
********************************************************************/
be_local_closure(class_EventManager_unregister_handler,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_EventManager,     /* shared constants */
    be_str_weak(unregister_handler),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0x88080300,  //  0000  GETMBR	R2	R1	K0
      0x1C080501,  //  0001  EQ	R2	R2	K1
      0x780A000B,  //  0002  JMPF	R2	#000F
      0x88080102,  //  0003  GETMBR	R2	R0	K2
      0x8C080503,  //  0004  GETMET	R2	R2	K3
      0x5C100200,  //  0005  MOVE	R4	R1
      0x7C080400,  //  0006  CALL	R2	2
      0x4C0C0000,  //  0007  LDNIL	R3
      0x200C0403,  //  0008  NE	R3	R2	R3
      0x780E0003,  //  0009  JMPF	R3	#000E
      0x880C0102,  //  000A  GETMBR	R3	R0	K2
      0x8C0C0704,  //  000B  GETMET	R3	R3	K4
      0x5C140400,  //  000C  MOVE	R5	R2
      0x7C0C0400,  //  000D  CALL	R3	2
      0x7002000F,  //  000E  JMP		#001F
      0x88080105,  //  000F  GETMBR	R2	R0	K5
      0x8C080503,  //  0010  GETMET	R2	R2	K3
      0x88100300,  //  0011  GETMBR	R4	R1	K0
      0x7C080400,  //  0012  CALL	R2	2
      0x4C0C0000,  //  0013  LDNIL	R3
      0x200C0403,  //  0014  NE	R3	R2	R3
      0x780E0008,  //  0015  JMPF	R3	#001F
      0x8C0C0503,  //  0016  GETMET	R3	R2	K3
      0x5C140200,  //  0017  MOVE	R5	R1
      0x7C0C0400,  //  0018  CALL	R3	2
      0x4C100000,  //  0019  LDNIL	R4
      0x20100604,  //  001A  NE	R4	R3	R4
      0x78120002,  //  001B  JMPF	R4	#001F
      0x8C100504,  //  001C  GETMET	R4	R2	K4
      0x5C180600,  //  001D  MOVE	R6	R3
      0x7C100400,  //  001E  CALL	R4	2
      0x80000000,  //  001F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_event_active
********************************************************************/
be_local_closure(class_EventManager_set_event_active,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_EventManager,     /* shared constants */
    be_str_weak(set_event_active),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x880C0105,  //  0000  GETMBR	R3	R0	K5
      0x8C0C0703,  //  0001  GETMET	R3	R3	K3
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x4C100000,  //  0004  LDNIL	R4
      0x20100604,  //  0005  NE	R4	R3	R4
      0x7812000C,  //  0006  JMPF	R4	#0014
      0x60100010,  //  0007  GETGBL	R4	G16
      0x5C140600,  //  0008  MOVE	R5	R3
      0x7C100200,  //  0009  CALL	R4	1
      0xA8020005,  //  000A  EXBLK	0	#0011
      0x5C140800,  //  000B  MOVE	R5	R4
      0x7C140000,  //  000C  CALL	R5	0
      0x8C180B06,  //  000D  GETMET	R6	R5	K6
      0x5C200400,  //  000E  MOVE	R8	R2
      0x7C180400,  //  000F  CALL	R6	2
      0x7001FFF9,  //  0010  JMP		#000B
      0x58100007,  //  0011  LDCONST	R4	K7
      0xAC100200,  //  0012  CATCH	R4	1	0
      0xB0080000,  //  0013  RAISE	2	R0	R0
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_handlers
********************************************************************/
be_local_closure(class_EventManager_get_handlers,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_EventManager,     /* shared constants */
    be_str_weak(get_handlers),
    &be_const_str_solidified,
    ( &(const binstruction[38]) {  /* code */
      0x60080012,  //  0000  GETGBL	R2	G18
      0x7C080000,  //  0001  CALL	R2	0
      0x600C0010,  //  0002  GETGBL	R3	G16
      0x88100102,  //  0003  GETMBR	R4	R0	K2
      0x7C0C0200,  //  0004  CALL	R3	1
      0xA8020006,  //  0005  EXBLK	0	#000D
      0x5C100600,  //  0006  MOVE	R4	R3
      0x7C100000,  //  0007  CALL	R4	0
      0x8C140508,  //  0008  GETMET	R5	R2	K8
      0x8C1C0909,  //  0009  GETMET	R7	R4	K9
      0x7C1C0200,  //  000A  CALL	R7	1
      0x7C140400,  //  000B  CALL	R5	2
      0x7001FFF8,  //  000C  JMP		#0006
      0x580C0007,  //  000D  LDCONST	R3	K7
      0xAC0C0200,  //  000E  CATCH	R3	1	0
      0xB0080000,  //  000F  RAISE	2	R0	R0
      0x880C0105,  //  0010  GETMBR	R3	R0	K5
      0x8C0C0703,  //  0011  GETMET	R3	R3	K3
      0x5C140200,  //  0012  MOVE	R5	R1
      0x7C0C0400,  //  0013  CALL	R3	2
      0x4C100000,  //  0014  LDNIL	R4
      0x20100604,  //  0015  NE	R4	R3	R4
      0x7812000D,  //  0016  JMPF	R4	#0025
      0x60100010,  //  0017  GETGBL	R4	G16
      0x5C140600,  //  0018  MOVE	R5	R3
      0x7C100200,  //  0019  CALL	R4	1
      0xA8020006,  //  001A  EXBLK	0	#0022
      0x5C140800,  //  001B  MOVE	R5	R4
      0x7C140000,  //  001C  CALL	R5	0
      0x8C180508,  //  001D  GETMET	R6	R2	K8
      0x8C200B09,  //  001E  GETMET	R8	R5	K9
      0x7C200200,  //  001F  CALL	R8	1
      0x7C180400,  //  0020  CALL	R6	2
      0x7001FFF8,  //  0021  JMP		#001B
      0x58100007,  //  0022  LDCONST	R4	K7
      0xAC100200,  //  0023  CATCH	R4	1	0
      0xB0080000,  //  0024  RAISE	2	R0	R0
      0x80040400,  //  0025  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_registered_events
********************************************************************/
be_local_closure(class_EventManager_get_registered_events,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_EventManager,     /* shared constants */
    be_str_weak(get_registered_events),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x60040012,  //  0000  GETGBL	R1	G18
      0x7C040000,  //  0001  CALL	R1	0
      0x60080010,  //  0002  GETGBL	R2	G16
      0x880C0105,  //  0003  GETMBR	R3	R0	K5
      0x8C0C070A,  //  0004  GETMET	R3	R3	K10
      0x7C0C0200,  //  0005  CALL	R3	1
      0x7C080200,  //  0006  CALL	R2	1
      0xA8020005,  //  0007  EXBLK	0	#000E
      0x5C0C0400,  //  0008  MOVE	R3	R2
      0x7C0C0000,  //  0009  CALL	R3	0
      0x8C100308,  //  000A  GETMET	R4	R1	K8
      0x5C180600,  //  000B  MOVE	R6	R3
      0x7C100400,  //  000C  CALL	R4	2
      0x7001FFF9,  //  000D  JMP		#0008
      0x58080007,  //  000E  LDCONST	R2	K7
      0xAC080200,  //  000F  CATCH	R2	1	0
      0xB0080000,  //  0010  RAISE	2	R0	R0
      0x80040200,  //  0011  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_EventManager_init,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_EventManager,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x60040013,  //  0000  GETGBL	R1	G19
      0x7C040000,  //  0001  CALL	R1	0
      0x90020A01,  //  0002  SETMBR	R0	K5	R1
      0x60040012,  //  0003  GETGBL	R1	G18
      0x7C040000,  //  0004  CALL	R1	0
      0x90020401,  //  0005  SETMBR	R0	K2	R1
      0x60040012,  //  0006  GETGBL	R1	G18
      0x7C040000,  //  0007  CALL	R1	0
      0x90021601,  //  0008  SETMBR	R0	K11	R1
      0x50040000,  //  0009  LDBOOL	R1	0	0
      0x90021801,  //  000A  SETMBR	R0	K12	R1
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: clear_all_handlers
********************************************************************/
be_local_closure(class_EventManager_clear_all_handlers,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_EventManager,     /* shared constants */
    be_str_weak(clear_all_handlers),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x88040105,  //  0000  GETMBR	R1	R0	K5
      0x8C04030D,  //  0001  GETMET	R1	R1	K13
      0x7C040200,  //  0002  CALL	R1	1
      0x88040102,  //  0003  GETMBR	R1	R0	K2
      0x8C04030D,  //  0004  GETMET	R1	R1	K13
      0x7C040200,  //  0005  CALL	R1	1
      0x8804010B,  //  0006  GETMBR	R1	R0	K11
      0x8C04030D,  //  0007  GETMET	R1	R1	K13
      0x7C040200,  //  0008  CALL	R1	1
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _sort_handlers
********************************************************************/
be_local_closure(class_EventManager__sort_handlers,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_EventManager,     /* shared constants */
    be_str_weak(_sort_handlers),
    &be_const_str_solidified,
    ( &(const binstruction[31]) {  /* code */
      0x60080010,  //  0000  GETGBL	R2	G16
      0x600C000C,  //  0001  GETGBL	R3	G12
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x040C070E,  //  0004  SUB	R3	R3	K14
      0x400E1C03,  //  0005  CONNECT	R3	K14	R3
      0x7C080200,  //  0006  CALL	R2	1
      0xA8020012,  //  0007  EXBLK	0	#001B
      0x5C0C0400,  //  0008  MOVE	R3	R2
      0x7C0C0000,  //  0009  CALL	R3	0
      0x94100203,  //  000A  GETIDX	R4	R1	R3
      0x5C140600,  //  000B  MOVE	R5	R3
      0x24180B0F,  //  000C  GT	R6	R5	K15
      0x781A000A,  //  000D  JMPF	R6	#0019
      0x04180B0E,  //  000E  SUB	R6	R5	K14
      0x94180206,  //  000F  GETIDX	R6	R1	R6
      0x88180D10,  //  0010  GETMBR	R6	R6	K16
      0x881C0910,  //  0011  GETMBR	R7	R4	K16
      0x14180C07,  //  0012  LT	R6	R6	R7
      0x781A0004,  //  0013  JMPF	R6	#0019
      0x04180B0E,  //  0014  SUB	R6	R5	K14
      0x94180206,  //  0015  GETIDX	R6	R1	R6
      0x98040A06,  //  0016  SETIDX	R1	R5	R6
      0x04140B0E,  //  0017  SUB	R5	R5	K14
      0x7001FFF2,  //  0018  JMP		#000C
      0x98040A04,  //  0019  SETIDX	R1	R5	R4
      0x7001FFEC,  //  001A  JMP		#0008
      0x58080007,  //  001B  LDCONST	R2	K7
      0xAC080200,  //  001C  CATCH	R2	1	0
      0xB0080000,  //  001D  RAISE	2	R0	R0
      0x80000000,  //  001E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: register_handler
********************************************************************/
be_local_closure(class_EventManager_register_handler,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    6,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_EventManager,     /* shared constants */
    be_str_weak(register_handler),
    &be_const_str_solidified,
    ( &(const binstruction[37]) {  /* code */
      0xB81A2200,  //  0000  GETNGBL	R6	K17
      0x8C180D12,  //  0001  GETMET	R6	R6	K18
      0x5C200200,  //  0002  MOVE	R8	R1
      0x5C240400,  //  0003  MOVE	R9	R2
      0x5C280600,  //  0004  MOVE	R10	R3
      0x5C2C0800,  //  0005  MOVE	R11	R4
      0x5C300A00,  //  0006  MOVE	R12	R5
      0x7C180C00,  //  0007  CALL	R6	6
      0x1C1C0301,  //  0008  EQ	R7	R1	K1
      0x781E0007,  //  0009  JMPF	R7	#0012
      0x881C0102,  //  000A  GETMBR	R7	R0	K2
      0x8C1C0F08,  //  000B  GETMET	R7	R7	K8
      0x5C240C00,  //  000C  MOVE	R9	R6
      0x7C1C0400,  //  000D  CALL	R7	2
      0x8C1C0113,  //  000E  GETMET	R7	R0	K19
      0x88240102,  //  000F  GETMBR	R9	R0	K2
      0x7C1C0400,  //  0010  CALL	R7	2
      0x70020011,  //  0011  JMP		#0024
      0x881C0105,  //  0012  GETMBR	R7	R0	K5
      0x8C1C0F14,  //  0013  GETMET	R7	R7	K20
      0x5C240200,  //  0014  MOVE	R9	R1
      0x7C1C0400,  //  0015  CALL	R7	2
      0x741E0003,  //  0016  JMPT	R7	#001B
      0x881C0105,  //  0017  GETMBR	R7	R0	K5
      0x60200012,  //  0018  GETGBL	R8	G18
      0x7C200000,  //  0019  CALL	R8	0
      0x981C0208,  //  001A  SETIDX	R7	R1	R8
      0x881C0105,  //  001B  GETMBR	R7	R0	K5
      0x941C0E01,  //  001C  GETIDX	R7	R7	R1
      0x8C1C0F08,  //  001D  GETMET	R7	R7	K8
      0x5C240C00,  //  001E  MOVE	R9	R6
      0x7C1C0400,  //  001F  CALL	R7	2
      0x8C1C0113,  //  0020  GETMET	R7	R0	K19
      0x88240105,  //  0021  GETMBR	R9	R0	K5
      0x94241201,  //  0022  GETIDX	R9	R9	R1
      0x7C1C0400,  //  0023  CALL	R7	2
      0x80040C00,  //  0024  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: trigger_event
********************************************************************/
be_local_closure(class_EventManager_trigger_event,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_EventManager,     /* shared constants */
    be_str_weak(trigger_event),
    &be_const_str_solidified,
    ( &(const binstruction[69]) {  /* code */
      0x880C010C,  //  0000  GETMBR	R3	R0	K12
      0x780E0007,  //  0001  JMPF	R3	#000A
      0x880C010B,  //  0002  GETMBR	R3	R0	K11
      0x8C0C0708,  //  0003  GETMET	R3	R3	K8
      0x60140013,  //  0004  GETGBL	R5	G19
      0x7C140000,  //  0005  CALL	R5	0
      0x98162A01,  //  0006  SETIDX	R5	K21	R1
      0x98162C02,  //  0007  SETIDX	R5	K22	R2
      0x7C0C0400,  //  0008  CALL	R3	2
      0x80000600,  //  0009  RET	0
      0x500C0200,  //  000A  LDBOOL	R3	1	0
      0x90021803,  //  000B  SETMBR	R0	K12	R3
      0xA8020029,  //  000C  EXBLK	0	#0037
      0x600C0010,  //  000D  GETGBL	R3	G16
      0x88100102,  //  000E  GETMBR	R4	R0	K2
      0x7C0C0200,  //  000F  CALL	R3	1
      0xA802000A,  //  0010  EXBLK	0	#001C
      0x5C100600,  //  0011  MOVE	R4	R3
      0x7C100000,  //  0012  CALL	R4	0
      0x88140917,  //  0013  GETMBR	R5	R4	K23
      0x78160005,  //  0014  JMPF	R5	#001B
      0x8C140918,  //  0015  GETMET	R5	R4	K24
      0x601C0013,  //  0016  GETGBL	R7	G19
      0x7C1C0000,  //  0017  CALL	R7	0
      0x981E0001,  //  0018  SETIDX	R7	K0	R1
      0x981E2C02,  //  0019  SETIDX	R7	K22	R2
      0x7C140400,  //  001A  CALL	R5	2
      0x7001FFF4,  //  001B  JMP		#0011
      0x580C0007,  //  001C  LDCONST	R3	K7
      0xAC0C0200,  //  001D  CATCH	R3	1	0
      0xB0080000,  //  001E  RAISE	2	R0	R0
      0x880C0105,  //  001F  GETMBR	R3	R0	K5
      0x8C0C0703,  //  0020  GETMET	R3	R3	K3
      0x5C140200,  //  0021  MOVE	R5	R1
      0x7C0C0400,  //  0022  CALL	R3	2
      0x4C100000,  //  0023  LDNIL	R4
      0x20100604,  //  0024  NE	R4	R3	R4
      0x7812000E,  //  0025  JMPF	R4	#0035
      0x60100010,  //  0026  GETGBL	R4	G16
      0x5C140600,  //  0027  MOVE	R5	R3
      0x7C100200,  //  0028  CALL	R4	1
      0xA8020007,  //  0029  EXBLK	0	#0032
      0x5C140800,  //  002A  MOVE	R5	R4
      0x7C140000,  //  002B  CALL	R5	0
      0x88180B17,  //  002C  GETMBR	R6	R5	K23
      0x781A0002,  //  002D  JMPF	R6	#0031
      0x8C180B18,  //  002E  GETMET	R6	R5	K24
      0x5C200400,  //  002F  MOVE	R8	R2
      0x7C180400,  //  0030  CALL	R6	2
      0x7001FFF7,  //  0031  JMP		#002A
      0x58100007,  //  0032  LDCONST	R4	K7
      0xAC100200,  //  0033  CATCH	R4	1	0
      0xB0080000,  //  0034  RAISE	2	R0	R0
      0xA8040001,  //  0035  EXBLK	1	1
      0x70020008,  //  0036  JMP		#0040
      0xAC0C0002,  //  0037  CATCH	R3	0	2
      0x70020005,  //  0038  JMP		#003F
      0x60140001,  //  0039  GETGBL	R5	G1
      0x58180019,  //  003A  LDCONST	R6	K25
      0x5C1C0600,  //  003B  MOVE	R7	R3
      0x5C200800,  //  003C  MOVE	R8	R4
      0x7C140600,  //  003D  CALL	R5	3
      0x70020000,  //  003E  JMP		#0040
      0xB0080000,  //  003F  RAISE	2	R0	R0
      0x500C0000,  //  0040  LDBOOL	R3	0	0
      0x90021803,  //  0041  SETMBR	R0	K12	R3
      0x8C0C011A,  //  0042  GETMET	R3	R0	K26
      0x7C0C0200,  //  0043  CALL	R3	1
      0x80000000,  //  0044  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _process_queued_events
********************************************************************/
be_local_closure(class_EventManager__process_queued_events,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_EventManager,     /* shared constants */
    be_str_weak(_process_queued_events),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x8804010B,  //  0000  GETMBR	R1	R0	K11
      0x8C04031B,  //  0001  GETMET	R1	R1	K27
      0x7C040200,  //  0002  CALL	R1	1
      0x2404030F,  //  0003  GT	R1	R1	K15
      0x78060008,  //  0004  JMPF	R1	#000E
      0x8804010B,  //  0005  GETMBR	R1	R0	K11
      0x8C04031C,  //  0006  GETMET	R1	R1	K28
      0x580C000F,  //  0007  LDCONST	R3	K15
      0x7C040400,  //  0008  CALL	R1	2
      0x8C08011D,  //  0009  GETMET	R2	R0	K29
      0x94100315,  //  000A  GETIDX	R4	R1	K21
      0x94140316,  //  000B  GETIDX	R5	R1	K22
      0x7C080600,  //  000C  CALL	R2	3
      0x7001FFF1,  //  000D  JMP		#0000
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: EventManager
********************************************************************/
be_local_class(EventManager,
    4,
    NULL,
    be_nested_map(14,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(unregister_handler, -1), be_const_closure(class_EventManager_unregister_handler_closure) },
        { be_const_key_weak(set_event_active, -1), be_const_closure(class_EventManager_set_event_active_closure) },
        { be_const_key_weak(handlers, -1), be_const_var(0) },
        { be_const_key_weak(init, -1), be_const_closure(class_EventManager_init_closure) },
        { be_const_key_weak(trigger_event, -1), be_const_closure(class_EventManager_trigger_event_closure) },
        { be_const_key_weak(get_handlers, 3), be_const_closure(class_EventManager_get_handlers_closure) },
        { be_const_key_weak(clear_all_handlers, -1), be_const_closure(class_EventManager_clear_all_handlers_closure) },
        { be_const_key_weak(event_queue, -1), be_const_var(2) },
        { be_const_key_weak(_sort_handlers, -1), be_const_closure(class_EventManager__sort_handlers_closure) },
        { be_const_key_weak(is_processing, 7), be_const_var(3) },
        { be_const_key_weak(global_handlers, -1), be_const_var(1) },
        { be_const_key_weak(register_handler, -1), be_const_closure(class_EventManager_register_handler_closure) },
        { be_const_key_weak(get_registered_events, 4), be_const_closure(class_EventManager_get_registered_events_closure) },
        { be_const_key_weak(_process_queued_events, -1), be_const_closure(class_EventManager__process_queued_events_closure) },
    })),
    be_str_weak(EventManager)
);
// compact class 'SequenceManager' ktab size: 39, total: 137 (saved 784 bytes)
static const bvalue be_ktab_class_SequenceManager[39] = {
  /* K0   */  be_nested_str_weak(steps),
  /* K1   */  be_nested_str_weak(push),
  /* K2   */  be_nested_str_weak(type),
  /* K3   */  be_nested_str_weak(play),
  /* K4   */  be_nested_str_weak(animation),
  /* K5   */  be_nested_str_weak(duration),
  /* K6   */  be_const_int(0),
  /* K7   */  be_nested_str_weak(is_running),
  /* K8   */  be_nested_str_weak(current_iteration),
  /* K9   */  be_const_int(1),
  /* K10  */  be_nested_str_weak(repeat_count),
  /* K11  */  be_nested_str_weak(step_index),
  /* K12  */  be_nested_str_weak(execute_current_step),
  /* K13  */  be_nested_str_weak(assign),
  /* K14  */  be_nested_str_weak(closure),
  /* K15  */  be_nested_str_weak(engine),
  /* K16  */  be_nested_str_weak(complete_iteration),
  /* K17  */  be_nested_str_weak(subsequence),
  /* K18  */  be_nested_str_weak(sequence_manager),
  /* K19  */  be_nested_str_weak(stop),
  /* K20  */  be_nested_str_weak(stop_iteration),
  /* K21  */  be_nested_str_weak(wait),
  /* K22  */  be_nested_str_weak(active_sequence),
  /* K23  */  be_nested_str_weak(sequence_state),
  /* K24  */  be_nested_str_weak(step_start_time),
  /* K25  */  be_nested_str_weak(is_repeat_sequence),
  /* K26  */  be_nested_str_weak(update),
  /* K27  */  be_nested_str_weak(advance_to_next_step),
  /* K28  */  be_nested_str_weak(execute_assign_steps_batch),
  /* K29  */  be_nested_str_weak(contains),
  /* K30  */  be_nested_str_weak(remove_animation),
  /* K31  */  be_nested_str_weak(clear),
  /* K32  */  be_nested_str_weak(stop_all_subsequences),
  /* K33  */  be_nested_str_weak(total_steps),
  /* K34  */  be_nested_str_weak(current_step),
  /* K35  */  be_nested_str_weak(elapsed_ms),
  /* K36  */  be_nested_str_weak(time_ms),
  /* K37  */  be_nested_str_weak(add_animation),
  /* K38  */  be_nested_str_weak(start),
};


extern const bclass be_class_SequenceManager;

/********************************************************************
** Solidified function: push_play_step
********************************************************************/
be_local_closure(class_SequenceManager_push_play_step,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SequenceManager,     /* shared constants */
    be_str_weak(push_play_step),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x60140013,  //  0002  GETGBL	R5	G19
      0x7C140000,  //  0003  CALL	R5	0
      0x98160503,  //  0004  SETIDX	R5	K2	K3
      0x98160801,  //  0005  SETIDX	R5	K4	R1
      0x4C180000,  //  0006  LDNIL	R6
      0x20180406,  //  0007  NE	R6	R2	R6
      0x781A0001,  //  0008  JMPF	R6	#000B
      0x5C180400,  //  0009  MOVE	R6	R2
      0x70020000,  //  000A  JMP		#000C
      0x58180006,  //  000B  LDCONST	R6	K6
      0x98160A06,  //  000C  SETIDX	R5	K5	R6
      0x7C0C0400,  //  000D  CALL	R3	2
      0x80040000,  //  000E  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_sequence_running
********************************************************************/
be_local_closure(class_SequenceManager_is_sequence_running,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SequenceManager,     /* shared constants */
    be_str_weak(is_sequence_running),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040107,  //  0000  GETMBR	R1	R0	K7
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: complete_iteration
********************************************************************/
be_local_closure(class_SequenceManager_complete_iteration,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SequenceManager,     /* shared constants */
    be_str_weak(complete_iteration),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x88080108,  //  0000  GETMBR	R2	R0	K8
      0x00080509,  //  0001  ADD	R2	R2	K9
      0x90021002,  //  0002  SETMBR	R0	K8	R2
      0x8808010A,  //  0003  GETMBR	R2	R0	K10
      0x540DFFFE,  //  0004  LDINT	R3	-1
      0x1C080403,  //  0005  EQ	R2	R2	R3
      0x740A0003,  //  0006  JMPT	R2	#000B
      0x88080108,  //  0007  GETMBR	R2	R0	K8
      0x880C010A,  //  0008  GETMBR	R3	R0	K10
      0x14080403,  //  0009  LT	R2	R2	R3
      0x780A0004,  //  000A  JMPF	R2	#0010
      0x90021706,  //  000B  SETMBR	R0	K11	K6
      0x8C08010C,  //  000C  GETMET	R2	R0	K12
      0x5C100200,  //  000D  MOVE	R4	R1
      0x7C080400,  //  000E  CALL	R2	2
      0x70020001,  //  000F  JMP		#0012
      0x50080000,  //  0010  LDBOOL	R2	0	0
      0x90020E02,  //  0011  SETMBR	R0	K7	R2
      0x80000000,  //  0012  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: execute_assign_steps_batch
********************************************************************/
be_local_closure(class_SequenceManager_execute_assign_steps_batch,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SequenceManager,     /* shared constants */
    be_str_weak(execute_assign_steps_batch),
    &be_const_str_solidified,
    ( &(const binstruction[39]) {  /* code */
      0x8808010B,  //  0000  GETMBR	R2	R0	K11
      0x600C000C,  //  0001  GETGBL	R3	G12
      0x88100100,  //  0002  GETMBR	R4	R0	K0
      0x7C0C0200,  //  0003  CALL	R3	1
      0x14080403,  //  0004  LT	R2	R2	R3
      0x780A0012,  //  0005  JMPF	R2	#0019
      0x88080100,  //  0006  GETMBR	R2	R0	K0
      0x880C010B,  //  0007  GETMBR	R3	R0	K11
      0x94080403,  //  0008  GETIDX	R2	R2	R3
      0x940C0502,  //  0009  GETIDX	R3	R2	K2
      0x1C0C070D,  //  000A  EQ	R3	R3	K13
      0x780E000A,  //  000B  JMPF	R3	#0017
      0x940C050E,  //  000C  GETIDX	R3	R2	K14
      0x4C100000,  //  000D  LDNIL	R4
      0x20100604,  //  000E  NE	R4	R3	R4
      0x78120002,  //  000F  JMPF	R4	#0013
      0x5C100600,  //  0010  MOVE	R4	R3
      0x8814010F,  //  0011  GETMBR	R5	R0	K15
      0x7C100200,  //  0012  CALL	R4	1
      0x8810010B,  //  0013  GETMBR	R4	R0	K11
      0x00100909,  //  0014  ADD	R4	R4	K9
      0x90021604,  //  0015  SETMBR	R0	K11	R4
      0x70020000,  //  0016  JMP		#0018
      0x70020000,  //  0017  JMP		#0019
      0x7001FFE6,  //  0018  JMP		#0000
      0x8808010B,  //  0019  GETMBR	R2	R0	K11
      0x600C000C,  //  001A  GETGBL	R3	G12
      0x88100100,  //  001B  GETMBR	R4	R0	K0
      0x7C0C0200,  //  001C  CALL	R3	1
      0x14080403,  //  001D  LT	R2	R2	R3
      0x780A0003,  //  001E  JMPF	R2	#0023
      0x8C08010C,  //  001F  GETMET	R2	R0	K12
      0x5C100200,  //  0020  MOVE	R4	R1
      0x7C080400,  //  0021  CALL	R2	2
      0x70020002,  //  0022  JMP		#0026
      0x8C080110,  //  0023  GETMET	R2	R0	K16
      0x5C100200,  //  0024  MOVE	R4	R1
      0x7C080400,  //  0025  CALL	R2	2
      0x80000000,  //  0026  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: stop_all_subsequences
********************************************************************/
be_local_closure(class_SequenceManager_stop_all_subsequences,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SequenceManager,     /* shared constants */
    be_str_weak(stop_all_subsequences),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x60040010,  //  0000  GETGBL	R1	G16
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x7C040200,  //  0002  CALL	R1	1
      0xA8020008,  //  0003  EXBLK	0	#000D
      0x5C080200,  //  0004  MOVE	R2	R1
      0x7C080000,  //  0005  CALL	R2	0
      0x940C0502,  //  0006  GETIDX	R3	R2	K2
      0x1C0C0711,  //  0007  EQ	R3	R3	K17
      0x780E0002,  //  0008  JMPF	R3	#000C
      0x940C0512,  //  0009  GETIDX	R3	R2	K18
      0x8C100713,  //  000A  GETMET	R4	R3	K19
      0x7C100200,  //  000B  CALL	R4	1
      0x7001FFF6,  //  000C  JMP		#0004
      0x58040014,  //  000D  LDCONST	R1	K20
      0xAC040200,  //  000E  CATCH	R1	1	0
      0xB0080000,  //  000F  RAISE	2	R0	R0
      0x80040000,  //  0010  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: push_wait_step
********************************************************************/
be_local_closure(class_SequenceManager_push_wait_step,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SequenceManager,     /* shared constants */
    be_str_weak(push_wait_step),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x60100013,  //  0002  GETGBL	R4	G19
      0x7C100000,  //  0003  CALL	R4	0
      0x98120515,  //  0004  SETIDX	R4	K2	K21
      0x98120A01,  //  0005  SETIDX	R4	K5	R1
      0x7C080400,  //  0006  CALL	R2	2
      0x80040000,  //  0007  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_SequenceManager_init,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SequenceManager,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[30]) {  /* code */
      0x90021E01,  //  0000  SETMBR	R0	K15	R1
      0x4C0C0000,  //  0001  LDNIL	R3
      0x90022C03,  //  0002  SETMBR	R0	K22	R3
      0x600C0013,  //  0003  GETGBL	R3	G19
      0x7C0C0000,  //  0004  CALL	R3	0
      0x90022E03,  //  0005  SETMBR	R0	K23	R3
      0x90021706,  //  0006  SETMBR	R0	K11	K6
      0x90023106,  //  0007  SETMBR	R0	K24	K6
      0x600C0012,  //  0008  GETGBL	R3	G18
      0x7C0C0000,  //  0009  CALL	R3	0
      0x90020003,  //  000A  SETMBR	R0	K0	R3
      0x500C0000,  //  000B  LDBOOL	R3	0	0
      0x90020E03,  //  000C  SETMBR	R0	K7	R3
      0x4C0C0000,  //  000D  LDNIL	R3
      0x200C0403,  //  000E  NE	R3	R2	R3
      0x780E0001,  //  000F  JMPF	R3	#0012
      0x5C0C0400,  //  0010  MOVE	R3	R2
      0x70020000,  //  0011  JMP		#0013
      0x580C0009,  //  0012  LDCONST	R3	K9
      0x90021403,  //  0013  SETMBR	R0	K10	R3
      0x90021106,  //  0014  SETMBR	R0	K8	K6
      0x4C0C0000,  //  0015  LDNIL	R3
      0x200C0403,  //  0016  NE	R3	R2	R3
      0x780E0001,  //  0017  JMPF	R3	#001A
      0x200C0509,  //  0018  NE	R3	R2	K9
      0x740E0000,  //  0019  JMPT	R3	#001B
      0x500C0001,  //  001A  LDBOOL	R3	0	1
      0x500C0200,  //  001B  LDBOOL	R3	1	0
      0x90023203,  //  001C  SETMBR	R0	K25	R3
      0x80000000,  //  001D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update
********************************************************************/
be_local_closure(class_SequenceManager_update,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SequenceManager,     /* shared constants */
    be_str_weak(update),
    &be_const_str_solidified,
    ( &(const binstruction[52]) {  /* code */
      0x88080107,  //  0000  GETMBR	R2	R0	K7
      0x780A0004,  //  0001  JMPF	R2	#0007
      0x6008000C,  //  0002  GETGBL	R2	G12
      0x880C0100,  //  0003  GETMBR	R3	R0	K0
      0x7C080200,  //  0004  CALL	R2	1
      0x1C080506,  //  0005  EQ	R2	R2	K6
      0x780A0001,  //  0006  JMPF	R2	#0009
      0x50080000,  //  0007  LDBOOL	R2	0	0
      0x80040400,  //  0008  RET	1	R2
      0x88080100,  //  0009  GETMBR	R2	R0	K0
      0x880C010B,  //  000A  GETMBR	R3	R0	K11
      0x94080403,  //  000B  GETIDX	R2	R2	R3
      0x940C0502,  //  000C  GETIDX	R3	R2	K2
      0x1C0C0711,  //  000D  EQ	R3	R3	K17
      0x780E0008,  //  000E  JMPF	R3	#0018
      0x940C0512,  //  000F  GETIDX	R3	R2	K18
      0x8C10071A,  //  0010  GETMET	R4	R3	K26
      0x5C180200,  //  0011  MOVE	R6	R1
      0x7C100400,  //  0012  CALL	R4	2
      0x74120002,  //  0013  JMPT	R4	#0017
      0x8C10011B,  //  0014  GETMET	R4	R0	K27
      0x5C180200,  //  0015  MOVE	R6	R1
      0x7C100400,  //  0016  CALL	R4	2
      0x70020019,  //  0017  JMP		#0032
      0x940C0502,  //  0018  GETIDX	R3	R2	K2
      0x1C0C070D,  //  0019  EQ	R3	R3	K13
      0x780E0003,  //  001A  JMPF	R3	#001F
      0x8C0C011C,  //  001B  GETMET	R3	R0	K28
      0x5C140200,  //  001C  MOVE	R5	R1
      0x7C0C0400,  //  001D  CALL	R3	2
      0x70020012,  //  001E  JMP		#0032
      0x8C0C051D,  //  001F  GETMET	R3	R2	K29
      0x58140005,  //  0020  LDCONST	R5	K5
      0x7C0C0400,  //  0021  CALL	R3	2
      0x780E000B,  //  0022  JMPF	R3	#002F
      0x940C0505,  //  0023  GETIDX	R3	R2	K5
      0x240C0706,  //  0024  GT	R3	R3	K6
      0x780E0008,  //  0025  JMPF	R3	#002F
      0x880C0118,  //  0026  GETMBR	R3	R0	K24
      0x040C0203,  //  0027  SUB	R3	R1	R3
      0x94100505,  //  0028  GETIDX	R4	R2	K5
      0x28100604,  //  0029  GE	R4	R3	R4
      0x78120002,  //  002A  JMPF	R4	#002E
      0x8C10011B,  //  002B  GETMET	R4	R0	K27
      0x5C180200,  //  002C  MOVE	R6	R1
      0x7C100400,  //  002D  CALL	R4	2
      0x70020002,  //  002E  JMP		#0032
      0x8C0C011B,  //  002F  GETMET	R3	R0	K27
      0x5C140200,  //  0030  MOVE	R5	R1
      0x7C0C0400,  //  0031  CALL	R3	2
      0x880C0107,  //  0032  GETMBR	R3	R0	K7
      0x80040600,  //  0033  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: push_repeat_subsequence
********************************************************************/
be_local_closure(class_SequenceManager_push_repeat_subsequence,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SequenceManager,     /* shared constants */
    be_str_weak(push_repeat_subsequence),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x60100013,  //  0002  GETGBL	R4	G19
      0x7C100000,  //  0003  CALL	R4	0
      0x98120511,  //  0004  SETIDX	R4	K2	K17
      0x98122401,  //  0005  SETIDX	R4	K18	R1
      0x7C080400,  //  0006  CALL	R2	2
      0x80040000,  //  0007  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: push_assign_step
********************************************************************/
be_local_closure(class_SequenceManager_push_assign_step,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SequenceManager,     /* shared constants */
    be_str_weak(push_assign_step),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x60100013,  //  0002  GETGBL	R4	G19
      0x7C100000,  //  0003  CALL	R4	0
      0x9812050D,  //  0004  SETIDX	R4	K2	K13
      0x98121C01,  //  0005  SETIDX	R4	K14	R1
      0x7C080400,  //  0006  CALL	R2	2
      0x80040000,  //  0007  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: push_step
********************************************************************/
be_local_closure(class_SequenceManager_push_step,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SequenceManager,     /* shared constants */
    be_str_weak(push_step),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80040000,  //  0004  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: stop
********************************************************************/
be_local_closure(class_SequenceManager_stop,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SequenceManager,     /* shared constants */
    be_str_weak(stop),
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
      0x88040107,  //  0000  GETMBR	R1	R0	K7
      0x7806001E,  //  0001  JMPF	R1	#0021
      0x50040000,  //  0002  LDBOOL	R1	0	0
      0x90020E01,  //  0003  SETMBR	R0	K7	R1
      0x8804010B,  //  0004  GETMBR	R1	R0	K11
      0x6008000C,  //  0005  GETGBL	R2	G12
      0x880C0100,  //  0006  GETMBR	R3	R0	K0
      0x7C080200,  //  0007  CALL	R2	1
      0x14040202,  //  0008  LT	R1	R1	R2
      0x78060011,  //  0009  JMPF	R1	#001C
      0x88040100,  //  000A  GETMBR	R1	R0	K0
      0x8808010B,  //  000B  GETMBR	R2	R0	K11
      0x94040202,  //  000C  GETIDX	R1	R1	R2
      0x94080302,  //  000D  GETIDX	R2	R1	K2
      0x1C080503,  //  000E  EQ	R2	R2	K3
      0x780A0005,  //  000F  JMPF	R2	#0016
      0x94080304,  //  0010  GETIDX	R2	R1	K4
      0x880C010F,  //  0011  GETMBR	R3	R0	K15
      0x8C0C071E,  //  0012  GETMET	R3	R3	K30
      0x5C140400,  //  0013  MOVE	R5	R2
      0x7C0C0400,  //  0014  CALL	R3	2
      0x70020005,  //  0015  JMP		#001C
      0x94080302,  //  0016  GETIDX	R2	R1	K2
      0x1C080511,  //  0017  EQ	R2	R2	K17
      0x780A0002,  //  0018  JMPF	R2	#001C
      0x94080312,  //  0019  GETIDX	R2	R1	K18
      0x8C0C0513,  //  001A  GETMET	R3	R2	K19
      0x7C0C0200,  //  001B  CALL	R3	1
      0x8804010F,  //  001C  GETMBR	R1	R0	K15
      0x8C04031F,  //  001D  GETMET	R1	R1	K31
      0x7C040200,  //  001E  CALL	R1	1
      0x8C040120,  //  001F  GETMET	R1	R0	K32
      0x7C040200,  //  0020  CALL	R1	1
      0x80040000,  //  0021  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: advance_to_next_step
********************************************************************/
be_local_closure(class_SequenceManager_advance_to_next_step,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SequenceManager,     /* shared constants */
    be_str_weak(advance_to_next_step),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x880C010B,  //  0001  GETMBR	R3	R0	K11
      0x94080403,  //  0002  GETIDX	R2	R2	R3
      0x940C0502,  //  0003  GETIDX	R3	R2	K2
      0x1C0C0703,  //  0004  EQ	R3	R3	K3
      0x780E0008,  //  0005  JMPF	R3	#000F
      0x8C0C051D,  //  0006  GETMET	R3	R2	K29
      0x58140005,  //  0007  LDCONST	R5	K5
      0x7C0C0400,  //  0008  CALL	R3	2
      0x780E0004,  //  0009  JMPF	R3	#000F
      0x940C0504,  //  000A  GETIDX	R3	R2	K4
      0x8810010F,  //  000B  GETMBR	R4	R0	K15
      0x8C10091E,  //  000C  GETMET	R4	R4	K30
      0x5C180600,  //  000D  MOVE	R6	R3
      0x7C100400,  //  000E  CALL	R4	2
      0x880C010B,  //  000F  GETMBR	R3	R0	K11
      0x000C0709,  //  0010  ADD	R3	R3	K9
      0x90021603,  //  0011  SETMBR	R0	K11	R3
      0x880C010B,  //  0012  GETMBR	R3	R0	K11
      0x6010000C,  //  0013  GETGBL	R4	G12
      0x88140100,  //  0014  GETMBR	R5	R0	K0
      0x7C100200,  //  0015  CALL	R4	1
      0x280C0604,  //  0016  GE	R3	R3	R4
      0x780E0003,  //  0017  JMPF	R3	#001C
      0x8C0C0110,  //  0018  GETMET	R3	R0	K16
      0x5C140200,  //  0019  MOVE	R5	R1
      0x7C0C0400,  //  001A  CALL	R3	2
      0x70020002,  //  001B  JMP		#001F
      0x8C0C011C,  //  001C  GETMET	R3	R0	K28
      0x5C140200,  //  001D  MOVE	R5	R1
      0x7C0C0400,  //  001E  CALL	R3	2
      0x80000000,  //  001F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(class_SequenceManager_start,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SequenceManager,     /* shared constants */
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x88080107,  //  0000  GETMBR	R2	R0	K7
      0x780A0006,  //  0001  JMPF	R2	#0009
      0x50080000,  //  0002  LDBOOL	R2	0	0
      0x90020E02,  //  0003  SETMBR	R0	K7	R2
      0x8808010F,  //  0004  GETMBR	R2	R0	K15
      0x8C08051F,  //  0005  GETMET	R2	R2	K31
      0x7C080200,  //  0006  CALL	R2	1
      0x8C080120,  //  0007  GETMET	R2	R0	K32
      0x7C080200,  //  0008  CALL	R2	1
      0x90021706,  //  0009  SETMBR	R0	K11	K6
      0x90023001,  //  000A  SETMBR	R0	K24	R1
      0x90021106,  //  000B  SETMBR	R0	K8	K6
      0x50080200,  //  000C  LDBOOL	R2	1	0
      0x90020E02,  //  000D  SETMBR	R0	K7	R2
      0x6008000C,  //  000E  GETGBL	R2	G12
      0x880C0100,  //  000F  GETMBR	R3	R0	K0
      0x7C080200,  //  0010  CALL	R2	1
      0x24080506,  //  0011  GT	R2	R2	K6
      0x780A0002,  //  0012  JMPF	R2	#0016
      0x8C08010C,  //  0013  GETMET	R2	R0	K12
      0x5C100200,  //  0014  MOVE	R4	R1
      0x7C080400,  //  0015  CALL	R2	2
      0x80040000,  //  0016  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_current_step_info
********************************************************************/
be_local_closure(class_SequenceManager_get_current_step_info,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SequenceManager,     /* shared constants */
    be_str_weak(get_current_step_info),
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
      0x88040107,  //  0000  GETMBR	R1	R0	K7
      0x78060005,  //  0001  JMPF	R1	#0008
      0x8804010B,  //  0002  GETMBR	R1	R0	K11
      0x6008000C,  //  0003  GETGBL	R2	G12
      0x880C0100,  //  0004  GETMBR	R3	R0	K0
      0x7C080200,  //  0005  CALL	R2	1
      0x28040202,  //  0006  GE	R1	R1	R2
      0x78060001,  //  0007  JMPF	R1	#000A
      0x4C040000,  //  0008  LDNIL	R1
      0x80040200,  //  0009  RET	1	R1
      0x60040013,  //  000A  GETGBL	R1	G19
      0x7C040000,  //  000B  CALL	R1	0
      0x8808010B,  //  000C  GETMBR	R2	R0	K11
      0x98061602,  //  000D  SETIDX	R1	K11	R2
      0x6008000C,  //  000E  GETGBL	R2	G12
      0x880C0100,  //  000F  GETMBR	R3	R0	K0
      0x7C080200,  //  0010  CALL	R2	1
      0x98064202,  //  0011  SETIDX	R1	K33	R2
      0x88080100,  //  0012  GETMBR	R2	R0	K0
      0x880C010B,  //  0013  GETMBR	R3	R0	K11
      0x94080403,  //  0014  GETIDX	R2	R2	R3
      0x98064402,  //  0015  SETIDX	R1	K34	R2
      0x8808010F,  //  0016  GETMBR	R2	R0	K15
      0x88080524,  //  0017  GETMBR	R2	R2	K36
      0x880C0118,  //  0018  GETMBR	R3	R0	K24
      0x04080403,  //  0019  SUB	R2	R2	R3
      0x98064602,  //  001A  SETIDX	R1	K35	R2
      0x8808010A,  //  001B  GETMBR	R2	R0	K10
      0x98061402,  //  001C  SETIDX	R1	K10	R2
      0x88080108,  //  001D  GETMBR	R2	R0	K8
      0x98061002,  //  001E  SETIDX	R1	K8	R2
      0x88080119,  //  001F  GETMBR	R2	R0	K25
      0x98063202,  //  0020  SETIDX	R1	K25	R2
      0x80040200,  //  0021  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: execute_current_step
********************************************************************/
be_local_closure(class_SequenceManager_execute_current_step,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SequenceManager,     /* shared constants */
    be_str_weak(execute_current_step),
    &be_const_str_solidified,
    ( &(const binstruction[57]) {  /* code */
      0x8808010B,  //  0000  GETMBR	R2	R0	K11
      0x600C000C,  //  0001  GETGBL	R3	G12
      0x88100100,  //  0002  GETMBR	R4	R0	K0
      0x7C0C0200,  //  0003  CALL	R3	1
      0x28080403,  //  0004  GE	R2	R2	R3
      0x780A0003,  //  0005  JMPF	R2	#000A
      0x8C080110,  //  0006  GETMET	R2	R0	K16
      0x5C100200,  //  0007  MOVE	R4	R1
      0x7C080400,  //  0008  CALL	R2	2
      0x80000400,  //  0009  RET	0
      0x88080100,  //  000A  GETMBR	R2	R0	K0
      0x880C010B,  //  000B  GETMBR	R3	R0	K11
      0x94080403,  //  000C  GETIDX	R2	R2	R3
      0x940C0502,  //  000D  GETIDX	R3	R2	K2
      0x1C0C0703,  //  000E  EQ	R3	R3	K3
      0x780E0008,  //  000F  JMPF	R3	#0019
      0x940C0504,  //  0010  GETIDX	R3	R2	K4
      0x8810010F,  //  0011  GETMBR	R4	R0	K15
      0x8C100925,  //  0012  GETMET	R4	R4	K37
      0x5C180600,  //  0013  MOVE	R6	R3
      0x7C100400,  //  0014  CALL	R4	2
      0x8C100726,  //  0015  GETMET	R4	R3	K38
      0x5C180200,  //  0016  MOVE	R6	R1
      0x7C100400,  //  0017  CALL	R4	2
      0x7002001D,  //  0018  JMP		#0037
      0x940C0502,  //  0019  GETIDX	R3	R2	K2
      0x1C0C0715,  //  001A  EQ	R3	R3	K21
      0x780E0000,  //  001B  JMPF	R3	#001D
      0x70020019,  //  001C  JMP		#0037
      0x940C0502,  //  001D  GETIDX	R3	R2	K2
      0x1C0C0713,  //  001E  EQ	R3	R3	K19
      0x780E0005,  //  001F  JMPF	R3	#0026
      0x940C0504,  //  0020  GETIDX	R3	R2	K4
      0x8810010F,  //  0021  GETMBR	R4	R0	K15
      0x8C10091E,  //  0022  GETMET	R4	R4	K30
      0x5C180600,  //  0023  MOVE	R6	R3
      0x7C100400,  //  0024  CALL	R4	2
      0x70020010,  //  0025  JMP		#0037
      0x940C0502,  //  0026  GETIDX	R3	R2	K2
      0x1C0C070D,  //  0027  EQ	R3	R3	K13
      0x780E0007,  //  0028  JMPF	R3	#0031
      0x940C050E,  //  0029  GETIDX	R3	R2	K14
      0x4C100000,  //  002A  LDNIL	R4
      0x20100604,  //  002B  NE	R4	R3	R4
      0x78120002,  //  002C  JMPF	R4	#0030
      0x5C100600,  //  002D  MOVE	R4	R3
      0x8814010F,  //  002E  GETMBR	R5	R0	K15
      0x7C100200,  //  002F  CALL	R4	1
      0x70020005,  //  0030  JMP		#0037
      0x940C0502,  //  0031  GETIDX	R3	R2	K2
      0x1C0C0711,  //  0032  EQ	R3	R3	K17
      0x780E0002,  //  0033  JMPF	R3	#0037
      0x940C0512,  //  0034  GETIDX	R3	R2	K18
      0x8C100726,  //  0035  GETMET	R4	R3	K38
      0x7C100200,  //  0036  CALL	R4	1
      0x90023001,  //  0037  SETMBR	R0	K24	R1
      0x80000000,  //  0038  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: SequenceManager
********************************************************************/
be_local_class(SequenceManager,
    10,
    NULL,
    be_nested_map(26,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(stop_all_subsequences, -1), be_const_closure(class_SequenceManager_stop_all_subsequences_closure) },
        { be_const_key_weak(engine, -1), be_const_var(0) },
        { be_const_key_weak(steps, -1), be_const_var(5) },
        { be_const_key_weak(sequence_state, -1), be_const_var(2) },
        { be_const_key_weak(update, -1), be_const_closure(class_SequenceManager_update_closure) },
        { be_const_key_weak(execute_assign_steps_batch, -1), be_const_closure(class_SequenceManager_execute_assign_steps_batch_closure) },
        { be_const_key_weak(current_iteration, 0), be_const_var(8) },
        { be_const_key_weak(push_play_step, 1), be_const_closure(class_SequenceManager_push_play_step_closure) },
        { be_const_key_weak(push_assign_step, 3), be_const_closure(class_SequenceManager_push_assign_step_closure) },
        { be_const_key_weak(is_repeat_sequence, -1), be_const_var(9) },
        { be_const_key_weak(is_running, -1), be_const_var(6) },
        { be_const_key_weak(stop, -1), be_const_closure(class_SequenceManager_stop_closure) },
        { be_const_key_weak(push_wait_step, 4), be_const_closure(class_SequenceManager_push_wait_step_closure) },
        { be_const_key_weak(push_repeat_subsequence, -1), be_const_closure(class_SequenceManager_push_repeat_subsequence_closure) },
        { be_const_key_weak(complete_iteration, 8), be_const_closure(class_SequenceManager_complete_iteration_closure) },
        { be_const_key_weak(is_sequence_running, 10), be_const_closure(class_SequenceManager_is_sequence_running_closure) },
        { be_const_key_weak(push_step, -1), be_const_closure(class_SequenceManager_push_step_closure) },
        { be_const_key_weak(active_sequence, -1), be_const_var(1) },
        { be_const_key_weak(step_start_time, -1), be_const_var(4) },
        { be_const_key_weak(init, 11), be_const_closure(class_SequenceManager_init_closure) },
        { be_const_key_weak(advance_to_next_step, -1), be_const_closure(class_SequenceManager_advance_to_next_step_closure) },
        { be_const_key_weak(start, -1), be_const_closure(class_SequenceManager_start_closure) },
        { be_const_key_weak(step_index, -1), be_const_var(3) },
        { be_const_key_weak(get_current_step_info, -1), be_const_closure(class_SequenceManager_get_current_step_info_closure) },
        { be_const_key_weak(repeat_count, -1), be_const_var(7) },
        { be_const_key_weak(execute_current_step, -1), be_const_closure(class_SequenceManager_execute_current_step_closure) },
    })),
    be_str_weak(SequenceManager)
);

/********************************************************************
** Solidified function: scale_static
********************************************************************/
be_local_closure(scale_static,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(scale_animation),
    /* K2   */  be_nested_str_weak(scale_mode),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(scale_speed),
    }),
    be_str_weak(scale_static),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x90060503,  //  0004  SETMBR	R1	K2	K3
      0x90060903,  //  0005  SETMBR	R1	K4	K3
      0x80040200,  //  0006  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: twinkle_intense
********************************************************************/
be_local_closure(twinkle_intense,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(twinkle_animation),
    /* K2   */  be_nested_str_weak(color),
    /* K3   */  be_nested_str_weak(density),
    /* K4   */  be_nested_str_weak(twinkle_speed),
    /* K5   */  be_nested_str_weak(fade_speed),
    /* K6   */  be_nested_str_weak(min_brightness),
    /* K7   */  be_nested_str_weak(max_brightness),
    }),
    be_str_weak(twinkle_intense),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x5408FFFF,  //  0004  LDINT	R2	-65536
      0x90060402,  //  0005  SETMBR	R1	K2	R2
      0x540A00C7,  //  0006  LDINT	R2	200
      0x90060602,  //  0007  SETMBR	R1	K3	R2
      0x540A000B,  //  0008  LDINT	R2	12
      0x90060802,  //  0009  SETMBR	R1	K4	R2
      0x540A00DB,  //  000A  LDINT	R2	220
      0x90060A02,  //  000B  SETMBR	R1	K5	R2
      0x540A003F,  //  000C  LDINT	R2	64
      0x90060C02,  //  000D  SETMBR	R1	K6	R2
      0x540A00FE,  //  000E  LDINT	R2	255
      0x90060E02,  //  000F  SETMBR	R1	K7	R2
      0x80040200,  //  0010  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: wave_single_sine
********************************************************************/
be_local_closure(wave_single_sine,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(wave_animation),
    /* K2   */  be_nested_str_weak(color),
    /* K3   */  be_nested_str_weak(wave_type),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(frequency),
    /* K6   */  be_nested_str_weak(wave_speed),
    }),
    be_str_weak(wave_single_sine),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x5408FFFF,  //  0004  LDINT	R2	-65536
      0x90060402,  //  0005  SETMBR	R1	K2	R2
      0x90060704,  //  0006  SETMBR	R1	K3	K4
      0x540A001F,  //  0007  LDINT	R2	32
      0x90060A02,  //  0008  SETMBR	R1	K5	R2
      0x540A0031,  //  0009  LDINT	R2	50
      0x90060C02,  //  000A  SETMBR	R1	K6	R2
      0x80040200,  //  000B  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: gradient_two_color_linear
********************************************************************/
be_local_closure(gradient_two_color_linear,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(gradient_animation),
    /* K2   */  be_nested_str_weak(color),
    /* K3   */  be_nested_str_weak(gradient_type),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(direction),
    /* K6   */  be_nested_str_weak(movement_speed),
    }),
    be_str_weak(gradient_two_color_linear),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x5408FFFF,  //  0004  LDINT	R2	-65536
      0x90060402,  //  0005  SETMBR	R1	K2	R2
      0x90060704,  //  0006  SETMBR	R1	K3	K4
      0x90060B04,  //  0007  SETMBR	R1	K5	K4
      0x90060D04,  //  0008  SETMBR	R1	K6	K4
      0x80040200,  //  0009  RET	1	R1
    })
  )
);
/*******************************************************************/

// compact class 'CometAnimation' ktab size: 24, total: 45 (saved 168 bytes)
static const bvalue be_ktab_class_CometAnimation[24] = {
  /* K0   */  be_nested_str_weak(update),
  /* K1   */  be_nested_str_weak(speed),
  /* K2   */  be_nested_str_weak(direction),
  /* K3   */  be_nested_str_weak(wrap_around),
  /* K4   */  be_nested_str_weak(engine),
  /* K5   */  be_nested_str_weak(get_strip_length),
  /* K6   */  be_nested_str_weak(start_time),
  /* K7   */  be_const_int(0),
  /* K8   */  be_nested_str_weak(head_position),
  /* K9   */  be_const_int(1),
  /* K10  */  be_nested_str_weak(init),
  /* K11  */  be_nested_str_weak(animation),
  /* K12  */  be_nested_str_weak(is_value_provider),
  /* K13  */  be_nested_str_weak(color),
  /* K14  */  be_nested_str_weak(0x_X2508x),
  /* K15  */  be_nested_str_weak(CometAnimation_X28color_X3D_X25s_X2C_X20head_pos_X3D_X25_X2E1f_X2C_X20tail_length_X3D_X25s_X2C_X20speed_X3D_X25s_X2C_X20direction_X3D_X25s_X2C_X20priority_X3D_X25s_X2C_X20running_X3D_X25s_X29),
  /* K16  */  be_nested_str_weak(tail_length),
  /* K17  */  be_nested_str_weak(priority),
  /* K18  */  be_nested_str_weak(is_running),
  /* K19  */  be_nested_str_weak(fade_factor),
  /* K20  */  be_nested_str_weak(tasmota),
  /* K21  */  be_nested_str_weak(scale_uint),
  /* K22  */  be_nested_str_weak(width),
  /* K23  */  be_nested_str_weak(set_pixel_color),
};


extern const bclass be_class_CometAnimation;

/********************************************************************
** Solidified function: update
********************************************************************/
be_local_closure(class_CometAnimation_update,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_CometAnimation,     /* shared constants */
    be_str_weak(update),
    &be_const_str_solidified,
    ( &(const binstruction[67]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x740A0001,  //  0006  JMPT	R2	#0009
      0x50080000,  //  0007  LDBOOL	R2	0	0
      0x80040400,  //  0008  RET	1	R2
      0x88080101,  //  0009  GETMBR	R2	R0	K1
      0x880C0102,  //  000A  GETMBR	R3	R0	K2
      0x88100103,  //  000B  GETMBR	R4	R0	K3
      0x88140104,  //  000C  GETMBR	R5	R0	K4
      0x8C140B05,  //  000D  GETMET	R5	R5	K5
      0x7C140200,  //  000E  CALL	R5	1
      0x88180106,  //  000F  GETMBR	R6	R0	K6
      0x04180206,  //  0010  SUB	R6	R1	R6
      0x081C0406,  //  0011  MUL	R7	R2	R6
      0x081C0E03,  //  0012  MUL	R7	R7	R3
      0x542203E7,  //  0013  LDINT	R8	1000
      0x0C1C0E08,  //  0014  DIV	R7	R7	R8
      0x24200707,  //  0015  GT	R8	R3	K7
      0x78220001,  //  0016  JMPF	R8	#0019
      0x90021007,  //  0017  SETMBR	R0	K8	R7
      0x70020004,  //  0018  JMP		#001E
      0x04200B09,  //  0019  SUB	R8	R5	K9
      0x542600FF,  //  001A  LDINT	R9	256
      0x08201009,  //  001B  MUL	R8	R8	R9
      0x00201007,  //  001C  ADD	R8	R8	R7
      0x90021008,  //  001D  SETMBR	R0	K8	R8
      0x542200FF,  //  001E  LDINT	R8	256
      0x08200A08,  //  001F  MUL	R8	R5	R8
      0x20240907,  //  0020  NE	R9	R4	K7
      0x7826000E,  //  0021  JMPF	R9	#0031
      0x88240108,  //  0022  GETMBR	R9	R0	K8
      0x28241208,  //  0023  GE	R9	R9	R8
      0x78260003,  //  0024  JMPF	R9	#0029
      0x88240108,  //  0025  GETMBR	R9	R0	K8
      0x04241208,  //  0026  SUB	R9	R9	R8
      0x90021009,  //  0027  SETMBR	R0	K8	R9
      0x7001FFF8,  //  0028  JMP		#0022
      0x88240108,  //  0029  GETMBR	R9	R0	K8
      0x14241307,  //  002A  LT	R9	R9	K7
      0x78260003,  //  002B  JMPF	R9	#0030
      0x88240108,  //  002C  GETMBR	R9	R0	K8
      0x00241208,  //  002D  ADD	R9	R9	R8
      0x90021009,  //  002E  SETMBR	R0	K8	R9
      0x7001FFF8,  //  002F  JMP		#0029
      0x7002000F,  //  0030  JMP		#0041
      0x88240108,  //  0031  GETMBR	R9	R0	K8
      0x28241208,  //  0032  GE	R9	R9	R8
      0x78260006,  //  0033  JMPF	R9	#003B
      0x04240B09,  //  0034  SUB	R9	R5	K9
      0x542A00FF,  //  0035  LDINT	R10	256
      0x0824120A,  //  0036  MUL	R9	R9	R10
      0x90021009,  //  0037  SETMBR	R0	K8	R9
      0x44240600,  //  0038  NEG	R9	R3
      0x90020409,  //  0039  SETMBR	R0	K2	R9
      0x70020005,  //  003A  JMP		#0041
      0x88240108,  //  003B  GETMBR	R9	R0	K8
      0x14241307,  //  003C  LT	R9	R9	K7
      0x78260002,  //  003D  JMPF	R9	#0041
      0x90021107,  //  003E  SETMBR	R0	K8	K7
      0x44240600,  //  003F  NEG	R9	R3
      0x90020409,  //  0040  SETMBR	R0	K2	R9
      0x50240200,  //  0041  LDBOOL	R9	1	0
      0x80041200,  //  0042  RET	1	R9
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_CometAnimation_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_CometAnimation,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C08050A,  //  0003  GETMET	R2	R2	K10
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x90021107,  //  0006  SETMBR	R0	K8	K7
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_CometAnimation_tostring,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_CometAnimation,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0x4C040000,  //  0000  LDNIL	R1
      0xB80A1600,  //  0001  GETNGBL	R2	K11
      0x8C08050C,  //  0002  GETMET	R2	R2	K12
      0x8810010D,  //  0003  GETMBR	R4	R0	K13
      0x7C080400,  //  0004  CALL	R2	2
      0x780A0004,  //  0005  JMPF	R2	#000B
      0x60080008,  //  0006  GETGBL	R2	G8
      0x880C010D,  //  0007  GETMBR	R3	R0	K13
      0x7C080200,  //  0008  CALL	R2	1
      0x5C040400,  //  0009  MOVE	R1	R2
      0x70020004,  //  000A  JMP		#0010
      0x60080018,  //  000B  GETGBL	R2	G24
      0x580C000E,  //  000C  LDCONST	R3	K14
      0x8810010D,  //  000D  GETMBR	R4	R0	K13
      0x7C080400,  //  000E  CALL	R2	2
      0x5C040400,  //  000F  MOVE	R1	R2
      0x60080018,  //  0010  GETGBL	R2	G24
      0x580C000F,  //  0011  LDCONST	R3	K15
      0x5C100200,  //  0012  MOVE	R4	R1
      0x88140108,  //  0013  GETMBR	R5	R0	K8
      0x541A00FF,  //  0014  LDINT	R6	256
      0x0C140A06,  //  0015  DIV	R5	R5	R6
      0x88180110,  //  0016  GETMBR	R6	R0	K16
      0x881C0101,  //  0017  GETMBR	R7	R0	K1
      0x88200102,  //  0018  GETMBR	R8	R0	K2
      0x88240111,  //  0019  GETMBR	R9	R0	K17
      0x88280112,  //  001A  GETMBR	R10	R0	K18
      0x7C081000,  //  001B  CALL	R2	8
      0x80040400,  //  001C  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: render
********************************************************************/
be_local_closure(class_CometAnimation_render,   /* name */
  be_nested_proto(
    25,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_CometAnimation,     /* shared constants */
    be_str_weak(render),
    &be_const_str_solidified,
    ( &(const binstruction[93]) {  /* code */
      0x880C0112,  //  0000  GETMBR	R3	R0	K18
      0x780E0002,  //  0001  JMPF	R3	#0005
      0x4C0C0000,  //  0002  LDNIL	R3
      0x1C0C0203,  //  0003  EQ	R3	R1	R3
      0x780E0001,  //  0004  JMPF	R3	#0007
      0x500C0000,  //  0005  LDBOOL	R3	0	0
      0x80040600,  //  0006  RET	1	R3
      0x880C0108,  //  0007  GETMBR	R3	R0	K8
      0x541200FF,  //  0008  LDINT	R4	256
      0x0C0C0604,  //  0009  DIV	R3	R3	R4
      0x8810010D,  //  000A  GETMBR	R4	R0	K13
      0x88140110,  //  000B  GETMBR	R5	R0	K16
      0x88180102,  //  000C  GETMBR	R6	R0	K2
      0x881C0103,  //  000D  GETMBR	R7	R0	K3
      0x88200113,  //  000E  GETMBR	R8	R0	K19
      0x88240104,  //  000F  GETMBR	R9	R0	K4
      0x8C241305,  //  0010  GETMET	R9	R9	K5
      0x7C240200,  //  0011  CALL	R9	1
      0x542A0017,  //  0012  LDINT	R10	24
      0x3C28080A,  //  0013  SHR	R10	R4	R10
      0x542E00FE,  //  0014  LDINT	R11	255
      0x2C28140B,  //  0015  AND	R10	R10	R11
      0x542E000F,  //  0016  LDINT	R11	16
      0x3C2C080B,  //  0017  SHR	R11	R4	R11
      0x543200FE,  //  0018  LDINT	R12	255
      0x2C2C160C,  //  0019  AND	R11	R11	R12
      0x54320007,  //  001A  LDINT	R12	8
      0x3C30080C,  //  001B  SHR	R12	R4	R12
      0x543600FE,  //  001C  LDINT	R13	255
      0x2C30180D,  //  001D  AND	R12	R12	R13
      0x543600FE,  //  001E  LDINT	R13	255
      0x2C34080D,  //  001F  AND	R13	R4	R13
      0x58380007,  //  0020  LDCONST	R14	K7
      0x143C1C05,  //  0021  LT	R15	R14	R5
      0x783E0037,  //  0022  JMPF	R15	#005B
      0x083C1C06,  //  0023  MUL	R15	R14	R6
      0x043C060F,  //  0024  SUB	R15	R3	R15
      0x20400F07,  //  0025  NE	R16	R7	K7
      0x78420008,  //  0026  JMPF	R16	#0030
      0x28401E09,  //  0027  GE	R16	R15	R9
      0x78420001,  //  0028  JMPF	R16	#002B
      0x043C1E09,  //  0029  SUB	R15	R15	R9
      0x7001FFFB,  //  002A  JMP		#0027
      0x14401F07,  //  002B  LT	R16	R15	K7
      0x78420001,  //  002C  JMPF	R16	#002F
      0x003C1E09,  //  002D  ADD	R15	R15	R9
      0x7001FFFB,  //  002E  JMP		#002B
      0x70020005,  //  002F  JMP		#0036
      0x14401F07,  //  0030  LT	R16	R15	K7
      0x74420001,  //  0031  JMPT	R16	#0034
      0x28401E09,  //  0032  GE	R16	R15	R9
      0x78420001,  //  0033  JMPF	R16	#0036
      0x00381D09,  //  0034  ADD	R14	R14	K9
      0x7001FFEA,  //  0035  JMP		#0021
      0x544200FE,  //  0036  LDINT	R16	255
      0x24441D07,  //  0037  GT	R17	R14	K7
      0x7846000D,  //  0038  JMPF	R17	#0047
      0x58440007,  //  0039  LDCONST	R17	K7
      0x1448220E,  //  003A  LT	R18	R17	R14
      0x784A000A,  //  003B  JMPF	R18	#0047
      0xB84A2800,  //  003C  GETNGBL	R18	K20
      0x8C482515,  //  003D  GETMET	R18	R18	K21
      0x5C502000,  //  003E  MOVE	R20	R16
      0x58540007,  //  003F  LDCONST	R21	K7
      0x545A00FE,  //  0040  LDINT	R22	255
      0x585C0007,  //  0041  LDCONST	R23	K7
      0x5C601000,  //  0042  MOVE	R24	R8
      0x7C480C00,  //  0043  CALL	R18	6
      0x5C402400,  //  0044  MOVE	R16	R18
      0x00442309,  //  0045  ADD	R17	R17	K9
      0x7001FFF2,  //  0046  JMP		#003A
      0x54460017,  //  0047  LDINT	R17	24
      0x38442011,  //  0048  SHL	R17	R16	R17
      0x544A000F,  //  0049  LDINT	R18	16
      0x38481612,  //  004A  SHL	R18	R11	R18
      0x30442212,  //  004B  OR	R17	R17	R18
      0x544A0007,  //  004C  LDINT	R18	8
      0x38481812,  //  004D  SHL	R18	R12	R18
      0x30442212,  //  004E  OR	R17	R17	R18
      0x3044220D,  //  004F  OR	R17	R17	R13
      0x28481F07,  //  0050  GE	R18	R15	K7
      0x784A0006,  //  0051  JMPF	R18	#0059
      0x88480316,  //  0052  GETMBR	R18	R1	K22
      0x14481E12,  //  0053  LT	R18	R15	R18
      0x784A0003,  //  0054  JMPF	R18	#0059
      0x8C480317,  //  0055  GETMET	R18	R1	K23
      0x5C501E00,  //  0056  MOVE	R20	R15
      0x5C542200,  //  0057  MOVE	R21	R17
      0x7C480600,  //  0058  CALL	R18	3
      0x00381D09,  //  0059  ADD	R14	R14	K9
      0x7001FFC5,  //  005A  JMP		#0021
      0x503C0200,  //  005B  LDBOOL	R15	1	0
      0x80041E00,  //  005C  RET	1	R15
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: on_param_changed
********************************************************************/
be_local_closure(class_CometAnimation_on_param_changed,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_CometAnimation,     /* shared constants */
    be_str_weak(on_param_changed),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x1C0C0302,  //  0000  EQ	R3	R1	K2
      0x780E000A,  //  0001  JMPF	R3	#000D
      0x880C0104,  //  0002  GETMBR	R3	R0	K4
      0x8C0C0705,  //  0003  GETMET	R3	R3	K5
      0x7C0C0200,  //  0004  CALL	R3	1
      0x24100507,  //  0005  GT	R4	R2	K7
      0x78120001,  //  0006  JMPF	R4	#0009
      0x90021107,  //  0007  SETMBR	R0	K8	K7
      0x70020003,  //  0008  JMP		#000D
      0x04100709,  //  0009  SUB	R4	R3	K9
      0x541600FF,  //  000A  LDINT	R5	256
      0x08100805,  //  000B  MUL	R4	R4	R5
      0x90021004,  //  000C  SETMBR	R0	K8	R4
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: CometAnimation
********************************************************************/
extern const bclass be_class_Animation;
be_local_class(CometAnimation,
    1,
    &be_class_Animation,
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, -1), be_const_closure(class_CometAnimation_init_closure) },
        { be_const_key_weak(update, -1), be_const_closure(class_CometAnimation_update_closure) },
        { be_const_key_weak(PARAMS, 4), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(tail_length, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(5) },
        { be_const_key_weak(min, -1), be_const_int(1) },
        { be_const_key_weak(max, 1), be_const_int(50) },
    }))    ) } )) },
        { be_const_key_weak(wrap_around, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(1) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(1) },
    }))    ) } )) },
        { be_const_key_weak(speed, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(2560) },
        { be_const_key_weak(min, -1), be_const_int(1) },
        { be_const_key_weak(max, 1), be_const_int(25600) },
    }))    ) } )) },
        { be_const_key_weak(color, 0), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(-1) },
    }))    ) } )) },
        { be_const_key_weak(direction, 3), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(enum, 1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(2,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(-1),
        be_const_int(1),
    }))    ) } )) },
        { be_const_key_weak(default, -1), be_const_int(1) },
    }))    ) } )) },
        { be_const_key_weak(fade_factor, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(179) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(tostring, -1), be_const_closure(class_CometAnimation_tostring_closure) },
        { be_const_key_weak(render, 0), be_const_closure(class_CometAnimation_render_closure) },
        { be_const_key_weak(head_position, 2), be_const_var(0) },
        { be_const_key_weak(on_param_changed, -1), be_const_closure(class_CometAnimation_on_param_changed_closure) },
    })),
    be_str_weak(CometAnimation)
);
// compact class 'FireAnimation' ktab size: 45, total: 73 (saved 224 bytes)
static const bvalue be_ktab_class_FireAnimation[45] = {
  /* K0   */  be_nested_str_weak(init),
  /* K1   */  be_nested_str_weak(heat_map),
  /* K2   */  be_nested_str_weak(current_colors),
  /* K3   */  be_nested_str_weak(last_update),
  /* K4   */  be_const_int(0),
  /* K5   */  be_nested_str_weak(random_seed),
  /* K6   */  be_nested_str_weak(engine),
  /* K7   */  be_nested_str_weak(time_ms),
  /* K8   */  be_nested_str_weak(cooling_rate),
  /* K9   */  be_nested_str_weak(sparking_rate),
  /* K10  */  be_nested_str_weak(intensity),
  /* K11  */  be_nested_str_weak(flicker_amount),
  /* K12  */  be_nested_str_weak(color),
  /* K13  */  be_nested_str_weak(get_strip_length),
  /* K14  */  be_nested_str_weak(_initialize_buffers),
  /* K15  */  be_nested_str_weak(_random_range),
  /* K16  */  be_nested_str_weak(tasmota),
  /* K17  */  be_nested_str_weak(scale_uint),
  /* K18  */  be_const_int(2),
  /* K19  */  be_const_int(1),
  /* K20  */  be_const_int(3),
  /* K21  */  be_const_int(-16777216),
  /* K22  */  be_nested_str_weak(animation),
  /* K23  */  be_nested_str_weak(rich_palette),
  /* K24  */  be_nested_str_weak(palette),
  /* K25  */  be_nested_str_weak(PALETTE_FIRE),
  /* K26  */  be_nested_str_weak(cycle_period),
  /* K27  */  be_nested_str_weak(transition_type),
  /* K28  */  be_nested_str_weak(brightness),
  /* K29  */  be_nested_str_weak(set_range),
  /* K30  */  be_nested_str_weak(is_color_provider),
  /* K31  */  be_nested_str_weak(get_color_for_value),
  /* K32  */  be_const_int(1103515245),
  /* K33  */  be_const_int(2147483647),
  /* K34  */  be_nested_str_weak(start),
  /* K35  */  be_nested_str_weak(resize),
  /* K36  */  be_nested_str_weak(is_running),
  /* K37  */  be_nested_str_weak(width),
  /* K38  */  be_nested_str_weak(set_pixel_color),
  /* K39  */  be_nested_str_weak(FireAnimation_X28intensity_X3D_X25s_X2C_X20flicker_speed_X3D_X25s_X2C_X20priority_X3D_X25s_X2C_X20running_X3D_X25s_X29),
  /* K40  */  be_nested_str_weak(flicker_speed),
  /* K41  */  be_nested_str_weak(priority),
  /* K42  */  be_nested_str_weak(_random),
  /* K43  */  be_nested_str_weak(update),
  /* K44  */  be_nested_str_weak(_update_fire_simulation),
};


extern const bclass be_class_FireAnimation;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_FireAnimation_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_FireAnimation,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x60080012,  //  0006  GETGBL	R2	G18
      0x7C080000,  //  0007  CALL	R2	0
      0x90020202,  //  0008  SETMBR	R0	K1	R2
      0x60080012,  //  0009  GETGBL	R2	G18
      0x7C080000,  //  000A  CALL	R2	0
      0x90020402,  //  000B  SETMBR	R0	K2	R2
      0x90020704,  //  000C  SETMBR	R0	K3	K4
      0x88080106,  //  000D  GETMBR	R2	R0	K6
      0x88080507,  //  000E  GETMBR	R2	R2	K7
      0x540EFFFF,  //  000F  LDINT	R3	65536
      0x10080403,  //  0010  MOD	R2	R2	R3
      0x90020A02,  //  0011  SETMBR	R0	K5	R2
      0x80000000,  //  0012  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _update_fire_simulation
********************************************************************/
be_local_closure(class_FireAnimation__update_fire_simulation,   /* name */
  be_nested_proto(
    23,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_FireAnimation,     /* shared constants */
    be_str_weak(_update_fire_simulation),
    &be_const_str_solidified,
    ( &(const binstruction[210]) {  /* code */
      0x88080108,  //  0000  GETMBR	R2	R0	K8
      0x880C0109,  //  0001  GETMBR	R3	R0	K9
      0x8810010A,  //  0002  GETMBR	R4	R0	K10
      0x8814010B,  //  0003  GETMBR	R5	R0	K11
      0x8818010C,  //  0004  GETMBR	R6	R0	K12
      0x881C0106,  //  0005  GETMBR	R7	R0	K6
      0x8C1C0F0D,  //  0006  GETMET	R7	R7	K13
      0x7C1C0200,  //  0007  CALL	R7	1
      0x6020000C,  //  0008  GETGBL	R8	G12
      0x88240101,  //  0009  GETMBR	R9	R0	K1
      0x7C200200,  //  000A  CALL	R8	1
      0x20201007,  //  000B  NE	R8	R8	R7
      0x78220001,  //  000C  JMPF	R8	#000F
      0x8C20010E,  //  000D  GETMET	R8	R0	K14
      0x7C200200,  //  000E  CALL	R8	1
      0x58200004,  //  000F  LDCONST	R8	K4
      0x14241007,  //  0010  LT	R9	R8	R7
      0x78260017,  //  0011  JMPF	R9	#002A
      0x8C24010F,  //  0012  GETMET	R9	R0	K15
      0xB82E2000,  //  0013  GETNGBL	R11	K16
      0x8C2C1711,  //  0014  GETMET	R11	R11	K17
      0x5C340400,  //  0015  MOVE	R13	R2
      0x58380004,  //  0016  LDCONST	R14	K4
      0x543E00FE,  //  0017  LDINT	R15	255
      0x58400004,  //  0018  LDCONST	R16	K4
      0x54460009,  //  0019  LDINT	R17	10
      0x7C2C0C00,  //  001A  CALL	R11	6
      0x002C1712,  //  001B  ADD	R11	R11	K18
      0x7C240400,  //  001C  CALL	R9	2
      0x88280101,  //  001D  GETMBR	R10	R0	K1
      0x94281408,  //  001E  GETIDX	R10	R10	R8
      0x2828120A,  //  001F  GE	R10	R9	R10
      0x782A0002,  //  0020  JMPF	R10	#0024
      0x88280101,  //  0021  GETMBR	R10	R0	K1
      0x98281104,  //  0022  SETIDX	R10	R8	K4
      0x70020003,  //  0023  JMP		#0028
      0x88280101,  //  0024  GETMBR	R10	R0	K1
      0x942C1408,  //  0025  GETIDX	R11	R10	R8
      0x042C1609,  //  0026  SUB	R11	R11	R9
      0x9828100B,  //  0027  SETIDX	R10	R8	R11
      0x00201113,  //  0028  ADD	R8	R8	K19
      0x7001FFE5,  //  0029  JMP		#0010
      0x28240F14,  //  002A  GE	R9	R7	K20
      0x78260012,  //  002B  JMPF	R9	#003F
      0x04240F13,  //  002C  SUB	R9	R7	K19
      0x28281312,  //  002D  GE	R10	R9	K18
      0x782A000F,  //  002E  JMPF	R10	#003F
      0x04281313,  //  002F  SUB	R10	R9	K19
      0x882C0101,  //  0030  GETMBR	R11	R0	K1
      0x9428160A,  //  0031  GETIDX	R10	R11	R10
      0x042C1312,  //  0032  SUB	R11	R9	K18
      0x88300101,  //  0033  GETMBR	R12	R0	K1
      0x942C180B,  //  0034  GETIDX	R11	R12	R11
      0x0028140B,  //  0035  ADD	R10	R10	R11
      0x042C1312,  //  0036  SUB	R11	R9	K18
      0x88300101,  //  0037  GETMBR	R12	R0	K1
      0x942C180B,  //  0038  GETIDX	R11	R12	R11
      0x0028140B,  //  0039  ADD	R10	R10	R11
      0x0C281514,  //  003A  DIV	R10	R10	K20
      0x882C0101,  //  003B  GETMBR	R11	R0	K1
      0x982C120A,  //  003C  SETIDX	R11	R9	R10
      0x04241313,  //  003D  SUB	R9	R9	K19
      0x7001FFED,  //  003E  JMP		#002D
      0x8C24010F,  //  003F  GETMET	R9	R0	K15
      0x542E00FE,  //  0040  LDINT	R11	255
      0x7C240400,  //  0041  CALL	R9	2
      0x14241203,  //  0042  LT	R9	R9	R3
      0x7826000B,  //  0043  JMPF	R9	#0050
      0x8C24010F,  //  0044  GETMET	R9	R0	K15
      0x542E0006,  //  0045  LDINT	R11	7
      0x7C240400,  //  0046  CALL	R9	2
      0x8C28010F,  //  0047  GETMET	R10	R0	K15
      0x5432005E,  //  0048  LDINT	R12	95
      0x7C280400,  //  0049  CALL	R10	2
      0x542E009F,  //  004A  LDINT	R11	160
      0x0028140B,  //  004B  ADD	R10	R10	R11
      0x142C1207,  //  004C  LT	R11	R9	R7
      0x782E0001,  //  004D  JMPF	R11	#0050
      0x882C0101,  //  004E  GETMBR	R11	R0	K1
      0x982C120A,  //  004F  SETIDX	R11	R9	R10
      0x58200004,  //  0050  LDCONST	R8	K4
      0x14241007,  //  0051  LT	R9	R8	R7
      0x7826007D,  //  0052  JMPF	R9	#00D1
      0x88240101,  //  0053  GETMBR	R9	R0	K1
      0x94241208,  //  0054  GETIDX	R9	R9	R8
      0xB82A2000,  //  0055  GETNGBL	R10	K16
      0x8C281511,  //  0056  GETMET	R10	R10	K17
      0x5C301200,  //  0057  MOVE	R12	R9
      0x58340004,  //  0058  LDCONST	R13	K4
      0x543A00FE,  //  0059  LDINT	R14	255
      0x583C0004,  //  005A  LDCONST	R15	K4
      0x5C400800,  //  005B  MOVE	R16	R4
      0x7C280C00,  //  005C  CALL	R10	6
      0x5C241400,  //  005D  MOVE	R9	R10
      0x24280B04,  //  005E  GT	R10	R5	K4
      0x782A0012,  //  005F  JMPF	R10	#0073
      0x8C28010F,  //  0060  GETMET	R10	R0	K15
      0x5C300A00,  //  0061  MOVE	R12	R5
      0x7C280400,  //  0062  CALL	R10	2
      0x8C2C010F,  //  0063  GETMET	R11	R0	K15
      0x58340012,  //  0064  LDCONST	R13	K18
      0x7C2C0400,  //  0065  CALL	R11	2
      0x1C2C1704,  //  0066  EQ	R11	R11	K4
      0x782E0001,  //  0067  JMPF	R11	#006A
      0x0024120A,  //  0068  ADD	R9	R9	R10
      0x70020004,  //  0069  JMP		#006F
      0x242C120A,  //  006A  GT	R11	R9	R10
      0x782E0001,  //  006B  JMPF	R11	#006E
      0x0424120A,  //  006C  SUB	R9	R9	R10
      0x70020000,  //  006D  JMP		#006F
      0x58240004,  //  006E  LDCONST	R9	K4
      0x542E00FE,  //  006F  LDINT	R11	255
      0x242C120B,  //  0070  GT	R11	R9	R11
      0x782E0000,  //  0071  JMPF	R11	#0073
      0x542600FE,  //  0072  LDINT	R9	255
      0x58280015,  //  0073  LDCONST	R10	K21
      0x242C1304,  //  0074  GT	R11	R9	K4
      0x782E0056,  //  0075  JMPF	R11	#00CD
      0x5C2C0C00,  //  0076  MOVE	R11	R6
      0x4C300000,  //  0077  LDNIL	R12
      0x1C30160C,  //  0078  EQ	R12	R11	R12
      0x7832000F,  //  0079  JMPF	R12	#008A
      0xB8322C00,  //  007A  GETNGBL	R12	K22
      0x8C301917,  //  007B  GETMET	R12	R12	K23
      0x88380106,  //  007C  GETMBR	R14	R0	K6
      0x7C300400,  //  007D  CALL	R12	2
      0xB8362C00,  //  007E  GETNGBL	R13	K22
      0x88341B19,  //  007F  GETMBR	R13	R13	K25
      0x9032300D,  //  0080  SETMBR	R12	K24	R13
      0x90323504,  //  0081  SETMBR	R12	K26	K4
      0x90323713,  //  0082  SETMBR	R12	K27	K19
      0x543600FE,  //  0083  LDINT	R13	255
      0x9032380D,  //  0084  SETMBR	R12	K28	R13
      0x8C34191D,  //  0085  GETMET	R13	R12	K29
      0x583C0004,  //  0086  LDCONST	R15	K4
      0x544200FE,  //  0087  LDINT	R16	255
      0x7C340600,  //  0088  CALL	R13	3
      0x5C2C1800,  //  0089  MOVE	R11	R12
      0xB8322C00,  //  008A  GETNGBL	R12	K22
      0x8C30191E,  //  008B  GETMET	R12	R12	K30
      0x5C381600,  //  008C  MOVE	R14	R11
      0x7C300400,  //  008D  CALL	R12	2
      0x78320009,  //  008E  JMPF	R12	#0099
      0x8830171F,  //  008F  GETMBR	R12	R11	K31
      0x4C340000,  //  0090  LDNIL	R13
      0x2030180D,  //  0091  NE	R12	R12	R13
      0x78320005,  //  0092  JMPF	R12	#0099
      0x8C30171F,  //  0093  GETMET	R12	R11	K31
      0x5C381200,  //  0094  MOVE	R14	R9
      0x583C0004,  //  0095  LDCONST	R15	K4
      0x7C300600,  //  0096  CALL	R12	3
      0x5C281800,  //  0097  MOVE	R10	R12
      0x70020033,  //  0098  JMP		#00CD
      0x5C281600,  //  0099  MOVE	R10	R11
      0x54320017,  //  009A  LDINT	R12	24
      0x3C30140C,  //  009B  SHR	R12	R10	R12
      0x543600FE,  //  009C  LDINT	R13	255
      0x2C30180D,  //  009D  AND	R12	R12	R13
      0x5436000F,  //  009E  LDINT	R13	16
      0x3C34140D,  //  009F  SHR	R13	R10	R13
      0x543A00FE,  //  00A0  LDINT	R14	255
      0x2C341A0E,  //  00A1  AND	R13	R13	R14
      0x543A0007,  //  00A2  LDINT	R14	8
      0x3C38140E,  //  00A3  SHR	R14	R10	R14
      0x543E00FE,  //  00A4  LDINT	R15	255
      0x2C381C0F,  //  00A5  AND	R14	R14	R15
      0x543E00FE,  //  00A6  LDINT	R15	255
      0x2C3C140F,  //  00A7  AND	R15	R10	R15
      0xB8422000,  //  00A8  GETNGBL	R16	K16
      0x8C402111,  //  00A9  GETMET	R16	R16	K17
      0x5C481200,  //  00AA  MOVE	R18	R9
      0x584C0004,  //  00AB  LDCONST	R19	K4
      0x545200FE,  //  00AC  LDINT	R20	255
      0x58540004,  //  00AD  LDCONST	R21	K4
      0x5C581A00,  //  00AE  MOVE	R22	R13
      0x7C400C00,  //  00AF  CALL	R16	6
      0x5C342000,  //  00B0  MOVE	R13	R16
      0xB8422000,  //  00B1  GETNGBL	R16	K16
      0x8C402111,  //  00B2  GETMET	R16	R16	K17
      0x5C481200,  //  00B3  MOVE	R18	R9
      0x584C0004,  //  00B4  LDCONST	R19	K4
      0x545200FE,  //  00B5  LDINT	R20	255
      0x58540004,  //  00B6  LDCONST	R21	K4
      0x5C581C00,  //  00B7  MOVE	R22	R14
      0x7C400C00,  //  00B8  CALL	R16	6
      0x5C382000,  //  00B9  MOVE	R14	R16
      0xB8422000,  //  00BA  GETNGBL	R16	K16
      0x8C402111,  //  00BB  GETMET	R16	R16	K17
      0x5C481200,  //  00BC  MOVE	R18	R9
      0x584C0004,  //  00BD  LDCONST	R19	K4
      0x545200FE,  //  00BE  LDINT	R20	255
      0x58540004,  //  00BF  LDCONST	R21	K4
      0x5C581E00,  //  00C0  MOVE	R22	R15
      0x7C400C00,  //  00C1  CALL	R16	6
      0x5C3C2000,  //  00C2  MOVE	R15	R16
      0x54420017,  //  00C3  LDINT	R16	24
      0x38401810,  //  00C4  SHL	R16	R12	R16
      0x5446000F,  //  00C5  LDINT	R17	16
      0x38441A11,  //  00C6  SHL	R17	R13	R17
      0x30402011,  //  00C7  OR	R16	R16	R17
      0x54460007,  //  00C8  LDINT	R17	8
      0x38441C11,  //  00C9  SHL	R17	R14	R17
      0x30402011,  //  00CA  OR	R16	R16	R17
      0x3040200F,  //  00CB  OR	R16	R16	R15
      0x5C282000,  //  00CC  MOVE	R10	R16
      0x882C0102,  //  00CD  GETMBR	R11	R0	K2
      0x982C100A,  //  00CE  SETIDX	R11	R8	R10
      0x00201113,  //  00CF  ADD	R8	R8	K19
      0x7001FF7F,  //  00D0  JMP		#0051
      0x80000000,  //  00D1  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _random
********************************************************************/
be_local_closure(class_FireAnimation__random,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_FireAnimation,     /* shared constants */
    be_str_weak(_random),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88040105,  //  0000  GETMBR	R1	R0	K5
      0x08040320,  //  0001  MUL	R1	R1	K32
      0x540A3038,  //  0002  LDINT	R2	12345
      0x00040202,  //  0003  ADD	R1	R1	R2
      0x2C040321,  //  0004  AND	R1	R1	K33
      0x90020A01,  //  0005  SETMBR	R0	K5	R1
      0x88040105,  //  0006  GETMBR	R1	R0	K5
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(class_FireAnimation_start,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_FireAnimation,     /* shared constants */
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080522,  //  0003  GETMET	R2	R2	K34
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x90020704,  //  0006  SETMBR	R0	K3	K4
      0x8C08010E,  //  0007  GETMET	R2	R0	K14
      0x7C080200,  //  0008  CALL	R2	1
      0x88080106,  //  0009  GETMBR	R2	R0	K6
      0x88080507,  //  000A  GETMBR	R2	R2	K7
      0x540EFFFF,  //  000B  LDINT	R3	65536
      0x10080403,  //  000C  MOD	R2	R2	R3
      0x90020A02,  //  000D  SETMBR	R0	K5	R2
      0x80040000,  //  000E  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: on_param_changed
********************************************************************/
be_local_closure(class_FireAnimation_on_param_changed,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_FireAnimation,     /* shared constants */
    be_str_weak(on_param_changed),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _initialize_buffers
********************************************************************/
be_local_closure(class_FireAnimation__initialize_buffers,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_FireAnimation,     /* shared constants */
    be_str_weak(_initialize_buffers),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x88040106,  //  0000  GETMBR	R1	R0	K6
      0x8C04030D,  //  0001  GETMET	R1	R1	K13
      0x7C040200,  //  0002  CALL	R1	1
      0x88080101,  //  0003  GETMBR	R2	R0	K1
      0x8C080523,  //  0004  GETMET	R2	R2	K35
      0x5C100200,  //  0005  MOVE	R4	R1
      0x7C080400,  //  0006  CALL	R2	2
      0x88080102,  //  0007  GETMBR	R2	R0	K2
      0x8C080523,  //  0008  GETMET	R2	R2	K35
      0x5C100200,  //  0009  MOVE	R4	R1
      0x7C080400,  //  000A  CALL	R2	2
      0x58080004,  //  000B  LDCONST	R2	K4
      0x140C0401,  //  000C  LT	R3	R2	R1
      0x780E0005,  //  000D  JMPF	R3	#0014
      0x880C0101,  //  000E  GETMBR	R3	R0	K1
      0x980C0504,  //  000F  SETIDX	R3	R2	K4
      0x880C0102,  //  0010  GETMBR	R3	R0	K2
      0x980C0515,  //  0011  SETIDX	R3	R2	K21
      0x00080513,  //  0012  ADD	R2	R2	K19
      0x7001FFF7,  //  0013  JMP		#000C
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: render
********************************************************************/
be_local_closure(class_FireAnimation_render,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_FireAnimation,     /* shared constants */
    be_str_weak(render),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x880C0124,  //  0000  GETMBR	R3	R0	K36
      0x780E0002,  //  0001  JMPF	R3	#0005
      0x4C0C0000,  //  0002  LDNIL	R3
      0x1C0C0203,  //  0003  EQ	R3	R1	R3
      0x780E0001,  //  0004  JMPF	R3	#0007
      0x500C0000,  //  0005  LDBOOL	R3	0	0
      0x80040600,  //  0006  RET	1	R3
      0x880C0106,  //  0007  GETMBR	R3	R0	K6
      0x8C0C070D,  //  0008  GETMET	R3	R3	K13
      0x7C0C0200,  //  0009  CALL	R3	1
      0x58100004,  //  000A  LDCONST	R4	K4
      0x14140803,  //  000B  LT	R5	R4	R3
      0x78160009,  //  000C  JMPF	R5	#0017
      0x88140325,  //  000D  GETMBR	R5	R1	K37
      0x14140805,  //  000E  LT	R5	R4	R5
      0x78160004,  //  000F  JMPF	R5	#0015
      0x8C140326,  //  0010  GETMET	R5	R1	K38
      0x5C1C0800,  //  0011  MOVE	R7	R4
      0x88200102,  //  0012  GETMBR	R8	R0	K2
      0x94201004,  //  0013  GETIDX	R8	R8	R4
      0x7C140600,  //  0014  CALL	R5	3
      0x00100913,  //  0015  ADD	R4	R4	K19
      0x7001FFF3,  //  0016  JMP		#000B
      0x50140200,  //  0017  LDBOOL	R5	1	0
      0x80040A00,  //  0018  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_FireAnimation_tostring,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_FireAnimation,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x60040018,  //  0000  GETGBL	R1	G24
      0x58080027,  //  0001  LDCONST	R2	K39
      0x880C010A,  //  0002  GETMBR	R3	R0	K10
      0x88100128,  //  0003  GETMBR	R4	R0	K40
      0x88140129,  //  0004  GETMBR	R5	R0	K41
      0x88180124,  //  0005  GETMBR	R6	R0	K36
      0x7C040A00,  //  0006  CALL	R1	5
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _random_range
********************************************************************/
be_local_closure(class_FireAnimation__random_range,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_FireAnimation,     /* shared constants */
    be_str_weak(_random_range),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x18080304,  //  0000  LE	R2	R1	K4
      0x780A0000,  //  0001  JMPF	R2	#0003
      0x80060800,  //  0002  RET	1	K4
      0x8C08012A,  //  0003  GETMET	R2	R0	K42
      0x7C080200,  //  0004  CALL	R2	1
      0x10080401,  //  0005  MOD	R2	R2	R1
      0x80040400,  //  0006  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update
********************************************************************/
be_local_closure(class_FireAnimation_update,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_FireAnimation,     /* shared constants */
    be_str_weak(update),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C08052B,  //  0003  GETMET	R2	R2	K43
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x740A0001,  //  0006  JMPT	R2	#0009
      0x50080000,  //  0007  LDBOOL	R2	0	0
      0x80040400,  //  0008  RET	1	R2
      0x88080128,  //  0009  GETMBR	R2	R0	K40
      0x540E03E7,  //  000A  LDINT	R3	1000
      0x0C0C0602,  //  000B  DIV	R3	R3	R2
      0x88100103,  //  000C  GETMBR	R4	R0	K3
      0x04100204,  //  000D  SUB	R4	R1	R4
      0x28100803,  //  000E  GE	R4	R4	R3
      0x78120003,  //  000F  JMPF	R4	#0014
      0x90020601,  //  0010  SETMBR	R0	K3	R1
      0x8C10012C,  //  0011  GETMET	R4	R0	K44
      0x5C180200,  //  0012  MOVE	R6	R1
      0x7C100400,  //  0013  CALL	R4	2
      0x50100200,  //  0014  LDBOOL	R4	1	0
      0x80040800,  //  0015  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: FireAnimation
********************************************************************/
extern const bclass be_class_Animation;
be_local_class(FireAnimation,
    4,
    &be_class_Animation,
    be_nested_map(15,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, -1), be_const_closure(class_FireAnimation_init_closure) },
        { be_const_key_weak(heat_map, -1), be_const_var(0) },
        { be_const_key_weak(last_update, -1), be_const_var(2) },
        { be_const_key_weak(PARAMS, 9), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(intensity, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(180) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(cooling_rate, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(55) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(flicker_amount, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(100) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(flicker_speed, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(8) },
        { be_const_key_weak(min, -1), be_const_int(1) },
        { be_const_key_weak(max, 1), be_const_int(20) },
    }))    ) } )) },
        { be_const_key_weak(color, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_nil() },
    }))    ) } )) },
        { be_const_key_weak(sparking_rate, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(120) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(_update_fire_simulation, 6), be_const_closure(class_FireAnimation__update_fire_simulation_closure) },
        { be_const_key_weak(_random, -1), be_const_closure(class_FireAnimation__random_closure) },
        { be_const_key_weak(update, -1), be_const_closure(class_FireAnimation_update_closure) },
        { be_const_key_weak(on_param_changed, 14), be_const_closure(class_FireAnimation_on_param_changed_closure) },
        { be_const_key_weak(random_seed, 13), be_const_var(3) },
        { be_const_key_weak(current_colors, -1), be_const_var(1) },
        { be_const_key_weak(_initialize_buffers, 8), be_const_closure(class_FireAnimation__initialize_buffers_closure) },
        { be_const_key_weak(start, 12), be_const_closure(class_FireAnimation_start_closure) },
        { be_const_key_weak(_random_range, -1), be_const_closure(class_FireAnimation__random_range_closure) },
        { be_const_key_weak(tostring, -1), be_const_closure(class_FireAnimation_tostring_closure) },
        { be_const_key_weak(render, -1), be_const_closure(class_FireAnimation_render_closure) },
    })),
    be_str_weak(FireAnimation)
);

/********************************************************************
** Solidified function: list_user_functions
********************************************************************/
be_local_closure(list_user_functions,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    0,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(global),
    /* K1   */  be_nested_str_weak(_animation_user_functions),
    /* K2   */  be_nested_str_weak(keys),
    /* K3   */  be_nested_str_weak(push),
    /* K4   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(list_user_functions),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0xA4020000,  //  0000  IMPORT	R0	K0
      0x60040012,  //  0001  GETGBL	R1	G18
      0x7C040000,  //  0002  CALL	R1	0
      0x60080010,  //  0003  GETGBL	R2	G16
      0x880C0101,  //  0004  GETMBR	R3	R0	K1
      0x8C0C0702,  //  0005  GETMET	R3	R3	K2
      0x7C0C0200,  //  0006  CALL	R3	1
      0x7C080200,  //  0007  CALL	R2	1
      0xA8020005,  //  0008  EXBLK	0	#000F
      0x5C0C0400,  //  0009  MOVE	R3	R2
      0x7C0C0000,  //  000A  CALL	R3	0
      0x8C100303,  //  000B  GETMET	R4	R1	K3
      0x5C180600,  //  000C  MOVE	R6	R3
      0x7C100400,  //  000D  CALL	R4	2
      0x7001FFF9,  //  000E  JMP		#0009
      0x58080004,  //  000F  LDCONST	R2	K4
      0xAC080200,  //  0010  CATCH	R2	1	0
      0xB0080000,  //  0011  RAISE	2	R0	R0
      0x80040200,  //  0012  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: gradient_rainbow_linear
********************************************************************/
be_local_closure(gradient_rainbow_linear,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(gradient_animation),
    /* K2   */  be_nested_str_weak(color),
    /* K3   */  be_nested_str_weak(gradient_type),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(direction),
    /* K6   */  be_nested_str_weak(movement_speed),
    }),
    be_str_weak(gradient_rainbow_linear),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x4C080000,  //  0004  LDNIL	R2
      0x90060402,  //  0005  SETMBR	R1	K2	R2
      0x90060704,  //  0006  SETMBR	R1	K3	K4
      0x90060B04,  //  0007  SETMBR	R1	K5	K4
      0x540A0031,  //  0008  LDINT	R2	50
      0x90060C02,  //  0009  SETMBR	R1	K6	R2
      0x80040200,  //  000A  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: jitter_position
********************************************************************/
be_local_closure(jitter_position,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(jitter_animation),
    /* K2   */  be_nested_str_weak(jitter_type),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(position_range),
    }),
    be_str_weak(jitter_position),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x90060503,  //  0004  SETMBR	R1	K2	K3
      0x540A0031,  //  0005  LDINT	R2	50
      0x90060802,  //  0006  SETMBR	R1	K4	R2
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: sparkle_white
********************************************************************/
be_local_closure(sparkle_white,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(sparkle_animation),
    /* K2   */  be_nested_str_weak(color),
    /* K3   */  be_nested_str_weak(name),
    /* K4   */  be_nested_str_weak(sparkle_white),
    }),
    be_str_weak(sparkle_white),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x5409FFFE,  //  0004  LDINT	R2	-1
      0x90060402,  //  0005  SETMBR	R1	K2	R2
      0x90060704,  //  0006  SETMBR	R1	K3	K4
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: scale_grow
********************************************************************/
be_local_closure(scale_grow,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(scale_animation),
    /* K2   */  be_nested_str_weak(scale_mode),
    /* K3   */  be_const_int(2),
    /* K4   */  be_nested_str_weak(scale_speed),
    }),
    be_str_weak(scale_grow),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x90060503,  //  0004  SETMBR	R1	K2	K3
      0x540A007F,  //  0005  LDINT	R2	128
      0x90060802,  //  0006  SETMBR	R1	K4	R2
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/

// compact class 'JitterAnimation' ktab size: 47, total: 93 (saved 368 bytes)
static const bvalue be_ktab_class_JitterAnimation[47] = {
  /* K0   */  be_nested_str_weak(jitter_type),
  /* K1   */  be_nested_str_weak(color_range),
  /* K2   */  be_nested_str_weak(brightness_range),
  /* K3   */  be_const_int(1),
  /* K4   */  be_const_int(3),
  /* K5   */  be_nested_str_weak(tasmota),
  /* K6   */  be_nested_str_weak(scale_uint),
  /* K7   */  be_const_int(0),
  /* K8   */  be_nested_str_weak(_random_range),
  /* K9   */  be_const_int(2),
  /* K10  */  be_nested_str_weak(position),
  /* K11  */  be_nested_str_weak(color),
  /* K12  */  be_nested_str_weak(brightness),
  /* K13  */  be_nested_str_weak(all),
  /* K14  */  be_nested_str_weak(unknown),
  /* K15  */  be_nested_str_weak(JitterAnimation_X28_X25s_X2C_X20intensity_X3D_X25s_X2C_X20frequency_X3D_X25s_X29),
  /* K16  */  be_nested_str_weak(jitter_intensity),
  /* K17  */  be_nested_str_weak(jitter_frequency),
  /* K18  */  be_nested_str_weak(_random),
  /* K19  */  be_nested_str_weak(init),
  /* K20  */  be_nested_str_weak(random_seed),
  /* K21  */  be_nested_str_weak(engine),
  /* K22  */  be_nested_str_weak(time_ms),
  /* K23  */  be_nested_str_weak(last_jitter_time),
  /* K24  */  be_nested_str_weak(_initialize_buffers),
  /* K25  */  be_nested_str_weak(get_strip_length),
  /* K26  */  be_nested_str_weak(jitter_offsets),
  /* K27  */  be_nested_str_weak(source_animation),
  /* K28  */  be_nested_str_weak(position_range),
  /* K29  */  be_nested_str_weak(source_frame),
  /* K30  */  be_nested_str_weak(clear),
  /* K31  */  be_nested_str_weak(render),
  /* K32  */  be_const_int(-16777216),
  /* K33  */  be_nested_str_weak(get_pixel_color),
  /* K34  */  be_nested_str_weak(_apply_color_jitter),
  /* K35  */  be_nested_str_weak(current_colors),
  /* K36  */  be_nested_str_weak(width),
  /* K37  */  be_nested_str_weak(set_pixel_color),
  /* K38  */  be_const_int(1103515245),
  /* K39  */  be_const_int(2147483647),
  /* K40  */  be_nested_str_weak(_update_jitter),
  /* K41  */  be_nested_str_weak(update),
  /* K42  */  be_nested_str_weak(_calculate_jitter),
  /* K43  */  be_nested_str_weak(start),
  /* K44  */  be_nested_str_weak(resize),
  /* K45  */  be_nested_str_weak(animation),
  /* K46  */  be_nested_str_weak(frame_buffer),
};


extern const bclass be_class_JitterAnimation;

/********************************************************************
** Solidified function: _apply_color_jitter
********************************************************************/
be_local_closure(class_JitterAnimation__apply_color_jitter,   /* name */
  be_nested_proto(
    19,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_JitterAnimation,     /* shared constants */
    be_str_weak(_apply_color_jitter),
    &be_const_str_solidified,
    ( &(const binstruction[127]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x88100101,  //  0001  GETMBR	R4	R0	K1
      0x88140102,  //  0002  GETMBR	R5	R0	K2
      0x541A0017,  //  0003  LDINT	R6	24
      0x3C180206,  //  0004  SHR	R6	R1	R6
      0x541E00FE,  //  0005  LDINT	R7	255
      0x2C180C07,  //  0006  AND	R6	R6	R7
      0x541E000F,  //  0007  LDINT	R7	16
      0x3C1C0207,  //  0008  SHR	R7	R1	R7
      0x542200FE,  //  0009  LDINT	R8	255
      0x2C1C0E08,  //  000A  AND	R7	R7	R8
      0x54220007,  //  000B  LDINT	R8	8
      0x3C200208,  //  000C  SHR	R8	R1	R8
      0x542600FE,  //  000D  LDINT	R9	255
      0x2C201009,  //  000E  AND	R8	R8	R9
      0x542600FE,  //  000F  LDINT	R9	255
      0x2C240209,  //  0010  AND	R9	R1	R9
      0x1C280703,  //  0011  EQ	R10	R3	K3
      0x742A0001,  //  0012  JMPT	R10	#0015
      0x1C280704,  //  0013  EQ	R10	R3	K4
      0x782A0013,  //  0014  JMPF	R10	#0029
      0xB82A0A00,  //  0015  GETNGBL	R10	K5
      0x8C281506,  //  0016  GETMET	R10	R10	K6
      0x5C300800,  //  0017  MOVE	R12	R4
      0x58340007,  //  0018  LDCONST	R13	K7
      0x543A00FE,  //  0019  LDINT	R14	255
      0x583C0007,  //  001A  LDCONST	R15	K7
      0x5442001D,  //  001B  LDINT	R16	30
      0x7C280C00,  //  001C  CALL	R10	6
      0x8C2C0108,  //  001D  GETMET	R11	R0	K8
      0x5C341400,  //  001E  MOVE	R13	R10
      0x7C2C0400,  //  001F  CALL	R11	2
      0x001C0E0B,  //  0020  ADD	R7	R7	R11
      0x8C2C0108,  //  0021  GETMET	R11	R0	K8
      0x5C341400,  //  0022  MOVE	R13	R10
      0x7C2C0400,  //  0023  CALL	R11	2
      0x0020100B,  //  0024  ADD	R8	R8	R11
      0x8C2C0108,  //  0025  GETMET	R11	R0	K8
      0x5C341400,  //  0026  MOVE	R13	R10
      0x7C2C0400,  //  0027  CALL	R11	2
      0x0024120B,  //  0028  ADD	R9	R9	R11
      0x1C280709,  //  0029  EQ	R10	R3	K9
      0x742A0001,  //  002A  JMPT	R10	#002D
      0x1C280704,  //  002B  EQ	R10	R3	K4
      0x782A002F,  //  002C  JMPF	R10	#005D
      0xB82A0A00,  //  002D  GETNGBL	R10	K5
      0x8C281506,  //  002E  GETMET	R10	R10	K6
      0x5C300A00,  //  002F  MOVE	R12	R5
      0x58340007,  //  0030  LDCONST	R13	K7
      0x543A00FE,  //  0031  LDINT	R14	255
      0x583C0007,  //  0032  LDCONST	R15	K7
      0x54420031,  //  0033  LDINT	R16	50
      0x7C280C00,  //  0034  CALL	R10	6
      0x542E007F,  //  0035  LDINT	R11	128
      0x8C300108,  //  0036  GETMET	R12	R0	K8
      0x5C381400,  //  0037  MOVE	R14	R10
      0x7C300400,  //  0038  CALL	R12	2
      0x002C160C,  //  0039  ADD	R11	R11	R12
      0x14301707,  //  003A  LT	R12	R11	K7
      0x78320001,  //  003B  JMPF	R12	#003E
      0x582C0007,  //  003C  LDCONST	R11	K7
      0x70020003,  //  003D  JMP		#0042
      0x543200FE,  //  003E  LDINT	R12	255
      0x2430160C,  //  003F  GT	R12	R11	R12
      0x78320000,  //  0040  JMPF	R12	#0042
      0x542E00FE,  //  0041  LDINT	R11	255
      0xB8320A00,  //  0042  GETNGBL	R12	K5
      0x8C301906,  //  0043  GETMET	R12	R12	K6
      0x5C380E00,  //  0044  MOVE	R14	R7
      0x583C0007,  //  0045  LDCONST	R15	K7
      0x544200FE,  //  0046  LDINT	R16	255
      0x58440007,  //  0047  LDCONST	R17	K7
      0x5C481600,  //  0048  MOVE	R18	R11
      0x7C300C00,  //  0049  CALL	R12	6
      0x5C1C1800,  //  004A  MOVE	R7	R12
      0xB8320A00,  //  004B  GETNGBL	R12	K5
      0x8C301906,  //  004C  GETMET	R12	R12	K6
      0x5C381000,  //  004D  MOVE	R14	R8
      0x583C0007,  //  004E  LDCONST	R15	K7
      0x544200FE,  //  004F  LDINT	R16	255
      0x58440007,  //  0050  LDCONST	R17	K7
      0x5C481600,  //  0051  MOVE	R18	R11
      0x7C300C00,  //  0052  CALL	R12	6
      0x5C201800,  //  0053  MOVE	R8	R12
      0xB8320A00,  //  0054  GETNGBL	R12	K5
      0x8C301906,  //  0055  GETMET	R12	R12	K6
      0x5C381200,  //  0056  MOVE	R14	R9
      0x583C0007,  //  0057  LDCONST	R15	K7
      0x544200FE,  //  0058  LDINT	R16	255
      0x58440007,  //  0059  LDCONST	R17	K7
      0x5C481600,  //  005A  MOVE	R18	R11
      0x7C300C00,  //  005B  CALL	R12	6
      0x5C241800,  //  005C  MOVE	R9	R12
      0x542A00FE,  //  005D  LDINT	R10	255
      0x24280E0A,  //  005E  GT	R10	R7	R10
      0x782A0001,  //  005F  JMPF	R10	#0062
      0x541E00FE,  //  0060  LDINT	R7	255
      0x70020002,  //  0061  JMP		#0065
      0x14280F07,  //  0062  LT	R10	R7	K7
      0x782A0000,  //  0063  JMPF	R10	#0065
      0x581C0007,  //  0064  LDCONST	R7	K7
      0x542A00FE,  //  0065  LDINT	R10	255
      0x2428100A,  //  0066  GT	R10	R8	R10
      0x782A0001,  //  0067  JMPF	R10	#006A
      0x542200FE,  //  0068  LDINT	R8	255
      0x70020002,  //  0069  JMP		#006D
      0x14281107,  //  006A  LT	R10	R8	K7
      0x782A0000,  //  006B  JMPF	R10	#006D
      0x58200007,  //  006C  LDCONST	R8	K7
      0x542A00FE,  //  006D  LDINT	R10	255
      0x2428120A,  //  006E  GT	R10	R9	R10
      0x782A0001,  //  006F  JMPF	R10	#0072
      0x542600FE,  //  0070  LDINT	R9	255
      0x70020002,  //  0071  JMP		#0075
      0x14281307,  //  0072  LT	R10	R9	K7
      0x782A0000,  //  0073  JMPF	R10	#0075
      0x58240007,  //  0074  LDCONST	R9	K7
      0x542A0017,  //  0075  LDINT	R10	24
      0x38280C0A,  //  0076  SHL	R10	R6	R10
      0x542E000F,  //  0077  LDINT	R11	16
      0x382C0E0B,  //  0078  SHL	R11	R7	R11
      0x3028140B,  //  0079  OR	R10	R10	R11
      0x542E0007,  //  007A  LDINT	R11	8
      0x382C100B,  //  007B  SHL	R11	R8	R11
      0x3028140B,  //  007C  OR	R10	R10	R11
      0x30281409,  //  007D  OR	R10	R10	R9
      0x80041400,  //  007E  RET	1	R10
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_JitterAnimation_tostring,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_JitterAnimation,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x60040012,  //  0000  GETGBL	R1	G18
      0x7C040000,  //  0001  CALL	R1	0
      0x4008030A,  //  0002  CONNECT	R2	R1	K10
      0x4008030B,  //  0003  CONNECT	R2	R1	K11
      0x4008030C,  //  0004  CONNECT	R2	R1	K12
      0x4008030D,  //  0005  CONNECT	R2	R1	K13
      0x88080100,  //  0006  GETMBR	R2	R0	K0
      0x940C0202,  //  0007  GETIDX	R3	R1	R2
      0x4C100000,  //  0008  LDNIL	R4
      0x200C0604,  //  0009  NE	R3	R3	R4
      0x780E0001,  //  000A  JMPF	R3	#000D
      0x940C0202,  //  000B  GETIDX	R3	R1	R2
      0x70020000,  //  000C  JMP		#000E
      0x580C000E,  //  000D  LDCONST	R3	K14
      0x60100018,  //  000E  GETGBL	R4	G24
      0x5814000F,  //  000F  LDCONST	R5	K15
      0x5C180600,  //  0010  MOVE	R6	R3
      0x881C0110,  //  0011  GETMBR	R7	R0	K16
      0x88200111,  //  0012  GETMBR	R8	R0	K17
      0x7C100800,  //  0013  CALL	R4	4
      0x80040800,  //  0014  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _random_range
********************************************************************/
be_local_closure(class_JitterAnimation__random_range,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_JitterAnimation,     /* shared constants */
    be_str_weak(_random_range),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x18080307,  //  0000  LE	R2	R1	K7
      0x780A0000,  //  0001  JMPF	R2	#0003
      0x80060E00,  //  0002  RET	1	K7
      0x8C080112,  //  0003  GETMET	R2	R0	K18
      0x7C080200,  //  0004  CALL	R2	1
      0x080C0309,  //  0005  MUL	R3	R1	K9
      0x000C0703,  //  0006  ADD	R3	R3	K3
      0x10080403,  //  0007  MOD	R2	R2	R3
      0x040C0401,  //  0008  SUB	R3	R2	R1
      0x80040600,  //  0009  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_JitterAnimation_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_JitterAnimation,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080513,  //  0003  GETMET	R2	R2	K19
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x88080115,  //  0006  GETMBR	R2	R0	K21
      0x88080516,  //  0007  GETMBR	R2	R2	K22
      0x540EFFFF,  //  0008  LDINT	R3	65536
      0x10080403,  //  0009  MOD	R2	R2	R3
      0x90022802,  //  000A  SETMBR	R0	K20	R2
      0x90022F07,  //  000B  SETMBR	R0	K23	K7
      0x8C080118,  //  000C  GETMET	R2	R0	K24
      0x7C080200,  //  000D  CALL	R2	1
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _update_jitter
********************************************************************/
be_local_closure(class_JitterAnimation__update_jitter,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_JitterAnimation,     /* shared constants */
    be_str_weak(_update_jitter),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x88040115,  //  0000  GETMBR	R1	R0	K21
      0x8C040319,  //  0001  GETMET	R1	R1	K25
      0x7C040200,  //  0002  CALL	R1	1
      0x88080110,  //  0003  GETMBR	R2	R0	K16
      0xB80E0A00,  //  0004  GETNGBL	R3	K5
      0x8C0C0706,  //  0005  GETMET	R3	R3	K6
      0x5C140400,  //  0006  MOVE	R5	R2
      0x58180007,  //  0007  LDCONST	R6	K7
      0x541E00FE,  //  0008  LDINT	R7	255
      0x58200007,  //  0009  LDCONST	R8	K7
      0x54260009,  //  000A  LDINT	R9	10
      0x7C0C0C00,  //  000B  CALL	R3	6
      0x58100007,  //  000C  LDCONST	R4	K7
      0x14140801,  //  000D  LT	R5	R4	R1
      0x78160006,  //  000E  JMPF	R5	#0016
      0x8814011A,  //  000F  GETMBR	R5	R0	K26
      0x8C180108,  //  0010  GETMET	R6	R0	K8
      0x5C200600,  //  0011  MOVE	R8	R3
      0x7C180400,  //  0012  CALL	R6	2
      0x98140806,  //  0013  SETIDX	R5	R4	R6
      0x00100903,  //  0014  ADD	R4	R4	K3
      0x7001FFF6,  //  0015  JMP		#000D
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _calculate_jitter
********************************************************************/
be_local_closure(class_JitterAnimation__calculate_jitter,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_JitterAnimation,     /* shared constants */
    be_str_weak(_calculate_jitter),
    &be_const_str_solidified,
    ( &(const binstruction[72]) {  /* code */
      0x88040115,  //  0000  GETMBR	R1	R0	K21
      0x8C040319,  //  0001  GETMET	R1	R1	K25
      0x7C040200,  //  0002  CALL	R1	1
      0x8808011B,  //  0003  GETMBR	R2	R0	K27
      0x880C0100,  //  0004  GETMBR	R3	R0	K0
      0x8810011C,  //  0005  GETMBR	R4	R0	K28
      0x8814011D,  //  0006  GETMBR	R5	R0	K29
      0x8C140B1E,  //  0007  GETMET	R5	R5	K30
      0x7C140200,  //  0008  CALL	R5	1
      0x4C140000,  //  0009  LDNIL	R5
      0x20140405,  //  000A  NE	R5	R2	R5
      0x78160003,  //  000B  JMPF	R5	#0010
      0x8C14051F,  //  000C  GETMET	R5	R2	K31
      0x881C011D,  //  000D  GETMBR	R7	R0	K29
      0x58200007,  //  000E  LDCONST	R8	K7
      0x7C140600,  //  000F  CALL	R5	3
      0x58140007,  //  0010  LDCONST	R5	K7
      0x14180A01,  //  0011  LT	R6	R5	R1
      0x781A0033,  //  0012  JMPF	R6	#0047
      0x58180020,  //  0013  LDCONST	R6	K32
      0x1C1C0707,  //  0014  EQ	R7	R3	K7
      0x741E0001,  //  0015  JMPT	R7	#0018
      0x1C1C0704,  //  0016  EQ	R7	R3	K4
      0x781E0018,  //  0017  JMPF	R7	#0031
      0xB81E0A00,  //  0018  GETNGBL	R7	K5
      0x8C1C0F06,  //  0019  GETMET	R7	R7	K6
      0x8824011A,  //  001A  GETMBR	R9	R0	K26
      0x94241205,  //  001B  GETIDX	R9	R9	R5
      0x5429FFF5,  //  001C  LDINT	R10	-10
      0x542E0009,  //  001D  LDINT	R11	10
      0x44300800,  //  001E  NEG	R12	R4
      0x54360009,  //  001F  LDINT	R13	10
      0x0C30180D,  //  0020  DIV	R12	R12	R13
      0x54360009,  //  0021  LDINT	R13	10
      0x0C34080D,  //  0022  DIV	R13	R4	R13
      0x7C1C0C00,  //  0023  CALL	R7	6
      0x00200A07,  //  0024  ADD	R8	R5	R7
      0x28241107,  //  0025  GE	R9	R8	K7
      0x78260007,  //  0026  JMPF	R9	#002F
      0x14241001,  //  0027  LT	R9	R8	R1
      0x78260005,  //  0028  JMPF	R9	#002F
      0x8824011D,  //  0029  GETMBR	R9	R0	K29
      0x8C241321,  //  002A  GETMET	R9	R9	K33
      0x5C2C1000,  //  002B  MOVE	R11	R8
      0x7C240400,  //  002C  CALL	R9	2
      0x5C181200,  //  002D  MOVE	R6	R9
      0x70020000,  //  002E  JMP		#0030
      0x58180020,  //  002F  LDCONST	R6	K32
      0x70020004,  //  0030  JMP		#0036
      0x881C011D,  //  0031  GETMBR	R7	R0	K29
      0x8C1C0F21,  //  0032  GETMET	R7	R7	K33
      0x5C240A00,  //  0033  MOVE	R9	R5
      0x7C1C0400,  //  0034  CALL	R7	2
      0x5C180E00,  //  0035  MOVE	R6	R7
      0x1C1C0703,  //  0036  EQ	R7	R3	K3
      0x741E0003,  //  0037  JMPT	R7	#003C
      0x1C1C0709,  //  0038  EQ	R7	R3	K9
      0x741E0001,  //  0039  JMPT	R7	#003C
      0x1C1C0704,  //  003A  EQ	R7	R3	K4
      0x781E0006,  //  003B  JMPF	R7	#0043
      0x201C0D20,  //  003C  NE	R7	R6	K32
      0x781E0004,  //  003D  JMPF	R7	#0043
      0x8C1C0122,  //  003E  GETMET	R7	R0	K34
      0x5C240C00,  //  003F  MOVE	R9	R6
      0x5C280A00,  //  0040  MOVE	R10	R5
      0x7C1C0600,  //  0041  CALL	R7	3
      0x5C180E00,  //  0042  MOVE	R6	R7
      0x881C0123,  //  0043  GETMBR	R7	R0	K35
      0x981C0A06,  //  0044  SETIDX	R7	R5	R6
      0x00140B03,  //  0045  ADD	R5	R5	K3
      0x7001FFC9,  //  0046  JMP		#0011
      0x80000000,  //  0047  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: render
********************************************************************/
be_local_closure(class_JitterAnimation_render,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_JitterAnimation,     /* shared constants */
    be_str_weak(render),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x1C0C0203,  //  0001  EQ	R3	R1	R3
      0x780E0001,  //  0002  JMPF	R3	#0005
      0x500C0000,  //  0003  LDBOOL	R3	0	0
      0x80040600,  //  0004  RET	1	R3
      0x880C0115,  //  0005  GETMBR	R3	R0	K21
      0x8C0C0719,  //  0006  GETMET	R3	R3	K25
      0x7C0C0200,  //  0007  CALL	R3	1
      0x58100007,  //  0008  LDCONST	R4	K7
      0x14140803,  //  0009  LT	R5	R4	R3
      0x78160009,  //  000A  JMPF	R5	#0015
      0x88140324,  //  000B  GETMBR	R5	R1	K36
      0x14140805,  //  000C  LT	R5	R4	R5
      0x78160004,  //  000D  JMPF	R5	#0013
      0x8C140325,  //  000E  GETMET	R5	R1	K37
      0x5C1C0800,  //  000F  MOVE	R7	R4
      0x88200123,  //  0010  GETMBR	R8	R0	K35
      0x94201004,  //  0011  GETIDX	R8	R8	R4
      0x7C140600,  //  0012  CALL	R5	3
      0x00100903,  //  0013  ADD	R4	R4	K3
      0x7001FFF3,  //  0014  JMP		#0009
      0x50140200,  //  0015  LDBOOL	R5	1	0
      0x80040A00,  //  0016  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _random
********************************************************************/
be_local_closure(class_JitterAnimation__random,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_JitterAnimation,     /* shared constants */
    be_str_weak(_random),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88040114,  //  0000  GETMBR	R1	R0	K20
      0x08040326,  //  0001  MUL	R1	R1	K38
      0x540A3038,  //  0002  LDINT	R2	12345
      0x00040202,  //  0003  ADD	R1	R1	R2
      0x2C040327,  //  0004  AND	R1	R1	K39
      0x90022801,  //  0005  SETMBR	R0	K20	R1
      0x88040114,  //  0006  GETMBR	R1	R0	K20
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update
********************************************************************/
be_local_closure(class_JitterAnimation_update,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_JitterAnimation,     /* shared constants */
    be_str_weak(update),
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0x88080111,  //  0000  GETMBR	R2	R0	K17
      0x880C011B,  //  0001  GETMBR	R3	R0	K27
      0x24100507,  //  0002  GT	R4	R2	K7
      0x78120014,  //  0003  JMPF	R4	#0019
      0xB8120A00,  //  0004  GETNGBL	R4	K5
      0x8C100906,  //  0005  GETMET	R4	R4	K6
      0x5C180400,  //  0006  MOVE	R6	R2
      0x581C0007,  //  0007  LDCONST	R7	K7
      0x542200FE,  //  0008  LDINT	R8	255
      0x58240007,  //  0009  LDCONST	R9	K7
      0x542A001D,  //  000A  LDINT	R10	30
      0x7C100C00,  //  000B  CALL	R4	6
      0x24140907,  //  000C  GT	R5	R4	K7
      0x78160002,  //  000D  JMPF	R5	#0011
      0x541603E7,  //  000E  LDINT	R5	1000
      0x0C140A04,  //  000F  DIV	R5	R5	R4
      0x70020000,  //  0010  JMP		#0012
      0x541603E7,  //  0011  LDINT	R5	1000
      0x88180117,  //  0012  GETMBR	R6	R0	K23
      0x04180206,  //  0013  SUB	R6	R1	R6
      0x28180C05,  //  0014  GE	R6	R6	R5
      0x781A0002,  //  0015  JMPF	R6	#0019
      0x90022E01,  //  0016  SETMBR	R0	K23	R1
      0x8C180128,  //  0017  GETMET	R6	R0	K40
      0x7C180200,  //  0018  CALL	R6	1
      0x4C100000,  //  0019  LDNIL	R4
      0x20100604,  //  001A  NE	R4	R3	R4
      0x78120002,  //  001B  JMPF	R4	#001F
      0x8C100729,  //  001C  GETMET	R4	R3	K41
      0x5C180200,  //  001D  MOVE	R6	R1
      0x7C100400,  //  001E  CALL	R4	2
      0x8C10012A,  //  001F  GETMET	R4	R0	K42
      0x7C100200,  //  0020  CALL	R4	1
      0x50100200,  //  0021  LDBOOL	R4	1	0
      0x80040800,  //  0022  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(class_JitterAnimation_start,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_JitterAnimation,     /* shared constants */
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C08052B,  //  0003  GETMET	R2	R2	K43
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x4C080000,  //  0006  LDNIL	R2
      0x20080202,  //  0007  NE	R2	R1	R2
      0x780A0001,  //  0008  JMPF	R2	#000B
      0x5C080200,  //  0009  MOVE	R2	R1
      0x70020001,  //  000A  JMP		#000D
      0x88080115,  //  000B  GETMBR	R2	R0	K21
      0x88080516,  //  000C  GETMBR	R2	R2	K22
      0x90022E02,  //  000D  SETMBR	R0	K23	R2
      0x8C080118,  //  000E  GETMET	R2	R0	K24
      0x7C080200,  //  000F  CALL	R2	1
      0x80040000,  //  0010  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _initialize_buffers
********************************************************************/
be_local_closure(class_JitterAnimation__initialize_buffers,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_JitterAnimation,     /* shared constants */
    be_str_weak(_initialize_buffers),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0x88040115,  //  0000  GETMBR	R1	R0	K21
      0x8C040319,  //  0001  GETMET	R1	R1	K25
      0x7C040200,  //  0002  CALL	R1	1
      0x60080012,  //  0003  GETGBL	R2	G18
      0x7C080000,  //  0004  CALL	R2	0
      0x90023402,  //  0005  SETMBR	R0	K26	R2
      0x8808011A,  //  0006  GETMBR	R2	R0	K26
      0x8C08052C,  //  0007  GETMET	R2	R2	K44
      0x5C100200,  //  0008  MOVE	R4	R1
      0x7C080400,  //  0009  CALL	R2	2
      0xB80A5A00,  //  000A  GETNGBL	R2	K45
      0x8C08052E,  //  000B  GETMET	R2	R2	K46
      0x5C100200,  //  000C  MOVE	R4	R1
      0x7C080400,  //  000D  CALL	R2	2
      0x90023A02,  //  000E  SETMBR	R0	K29	R2
      0x60080012,  //  000F  GETGBL	R2	G18
      0x7C080000,  //  0010  CALL	R2	0
      0x90024602,  //  0011  SETMBR	R0	K35	R2
      0x88080123,  //  0012  GETMBR	R2	R0	K35
      0x8C08052C,  //  0013  GETMET	R2	R2	K44
      0x5C100200,  //  0014  MOVE	R4	R1
      0x7C080400,  //  0015  CALL	R2	2
      0x58080007,  //  0016  LDCONST	R2	K7
      0x140C0401,  //  0017  LT	R3	R2	R1
      0x780E0005,  //  0018  JMPF	R3	#001F
      0x880C011A,  //  0019  GETMBR	R3	R0	K26
      0x980C0507,  //  001A  SETIDX	R3	R2	K7
      0x880C0123,  //  001B  GETMBR	R3	R0	K35
      0x980C0520,  //  001C  SETIDX	R3	R2	K32
      0x00080503,  //  001D  ADD	R2	R2	K3
      0x7001FFF7,  //  001E  JMP		#0017
      0x80000000,  //  001F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: JitterAnimation
********************************************************************/
extern const bclass be_class_Animation;
be_local_class(JitterAnimation,
    5,
    &be_class_Animation,
    be_nested_map(17,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(_apply_color_jitter, 8), be_const_closure(class_JitterAnimation__apply_color_jitter_closure) },
        { be_const_key_weak(tostring, -1), be_const_closure(class_JitterAnimation_tostring_closure) },
        { be_const_key_weak(jitter_offsets, 7), be_const_var(2) },
        { be_const_key_weak(_initialize_buffers, 9), be_const_closure(class_JitterAnimation__initialize_buffers_closure) },
        { be_const_key_weak(last_jitter_time, -1), be_const_var(1) },
        { be_const_key_weak(start, -1), be_const_closure(class_JitterAnimation_start_closure) },
        { be_const_key_weak(update, -1), be_const_closure(class_JitterAnimation_update_closure) },
        { be_const_key_weak(_update_jitter, 1), be_const_closure(class_JitterAnimation__update_jitter_closure) },
        { be_const_key_weak(_random, 11), be_const_closure(class_JitterAnimation__random_closure) },
        { be_const_key_weak(source_frame, -1), be_const_var(3) },
        { be_const_key_weak(render, -1), be_const_closure(class_JitterAnimation_render_closure) },
        { be_const_key_weak(current_colors, -1), be_const_var(4) },
        { be_const_key_weak(random_seed, -1), be_const_var(0) },
        { be_const_key_weak(_random_range, 6), be_const_closure(class_JitterAnimation__random_range_closure) },
        { be_const_key_weak(_calculate_jitter, 5), be_const_closure(class_JitterAnimation__calculate_jitter_closure) },
        { be_const_key_weak(init, 3), be_const_closure(class_JitterAnimation_init_closure) },
        { be_const_key_weak(PARAMS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(brightness_range, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(40) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(jitter_intensity, 3), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(100) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(jitter_type, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(0) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(3) },
    }))    ) } )) },
        { be_const_key_weak(color_range, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(30) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(source_animation, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_nil() },
        { be_const_key_weak(type, -1), be_nested_str_weak(instance) },
    }))    ) } )) },
        { be_const_key_weak(position_range, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(50) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(jitter_frequency, 2), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(60) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
    }))    ) } )) },
    })),
    be_str_weak(JitterAnimation)
);
// compact class 'RichPaletteColorProvider' ktab size: 46, total: 105 (saved 472 bytes)
static const bvalue be_ktab_class_RichPaletteColorProvider[46] = {
  /* K0   */  be_nested_str_weak(value_error),
  /* K1   */  be_nested_str_weak(min_X20must_X20be_X20lower_X20than_X20max),
  /* K2   */  be_nested_str_weak(range_min),
  /* K3   */  be_nested_str_weak(range_max),
  /* K4   */  be_nested_str_weak(slots_arr),
  /* K5   */  be_nested_str_weak(_parse_palette),
  /* K6   */  be_nested_str_weak(RichPaletteColorProvider_X28slots_X3D_X25s_X2C_X20cycle_period_X3D_X25s_X29),
  /* K7   */  be_nested_str_weak(slots),
  /* K8   */  be_nested_str_weak(cycle_period),
  /* K9   */  be_nested_str_weak(RichPaletteColorProvider_X28uninitialized_X29),
  /* K10  */  be_nested_str_weak(_get_palette_bytes),
  /* K11  */  be_nested_str_weak(resize),
  /* K12  */  be_nested_str_weak(get),
  /* K13  */  be_const_int(0),
  /* K14  */  be_const_int(1),
  /* K15  */  be_nested_str_weak(tasmota),
  /* K16  */  be_nested_str_weak(scale_int),
  /* K17  */  be_nested_str_weak(palette),
  /* K18  */  be_nested_str_weak(_recompute_palette),
  /* K19  */  be_nested_str_weak(cycle_period_X20must_X20be_X20non_X2Dnegative),
  /* K20  */  be_nested_str_weak(range_min_X20must_X20be_X20lower_X20than_X20range_max),
  /* K21  */  be_nested_str_weak(brightness),
  /* K22  */  be_const_int(2),
  /* K23  */  be_nested_str_weak(scale_uint),
  /* K24  */  be_nested_str_weak(current_color),
  /* K25  */  be_nested_str_weak(_get_color_at_index),
  /* K26  */  be_nested_str_weak(init),
  /* K27  */  be_nested_str_weak(cycle_start),
  /* K28  */  be_nested_str_weak(engine),
  /* K29  */  be_nested_str_weak(time_ms),
  /* K30  */  be_nested_str_weak(global),
  /* K31  */  be_nested_str_weak(light_state),
  /* K32  */  be_nested_str_weak(RGB),
  /* K33  */  be_nested_str_weak(background_X3Alinear_X2Dgradient_X28to_X20right_X2C_X20_X23000000_X29_X3B),
  /* K34  */  be_nested_str_weak(background_X3Alinear_X2Dgradient_X28to_X20right),
  /* K35  */  be_nested_str_weak(_X2C_X23_X2502X_X2502X_X2502X_X20_X25_X2E1f_X25_X25),
  /* K36  */  be_const_real_hex(0x41200000),
  /* K37  */  be_nested_str_weak(_X29_X3B),
  /* K38  */  be_const_int(-16777216),
  /* K39  */  be_nested_str_weak(_DEFAULT_PALETTE),
  /* K40  */  be_nested_str_weak(set_rgb),
  /* K41  */  be_nested_str_weak(bri),
  /* K42  */  be_nested_str_weak(set_bri),
  /* K43  */  be_nested_str_weak(r),
  /* K44  */  be_nested_str_weak(g),
  /* K45  */  be_nested_str_weak(b),
};


extern const bclass be_class_RichPaletteColorProvider;

/********************************************************************
** Solidified function: set_range
********************************************************************/
be_local_closure(class_RichPaletteColorProvider_set_range,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_RichPaletteColorProvider,     /* shared constants */
    be_str_weak(set_range),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x280C0202,  //  0000  GE	R3	R1	R2
      0x780E0000,  //  0001  JMPF	R3	#0003
      0xB0060101,  //  0002  RAISE	1	K0	K1
      0x90020401,  //  0003  SETMBR	R0	K2	R1
      0x90020602,  //  0004  SETMBR	R0	K3	R2
      0x8C0C0105,  //  0005  GETMET	R3	R0	K5
      0x5C140200,  //  0006  MOVE	R5	R1
      0x5C180400,  //  0007  MOVE	R6	R2
      0x7C0C0600,  //  0008  CALL	R3	3
      0x90020803,  //  0009  SETMBR	R0	K4	R3
      0x80040000,  //  000A  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_RichPaletteColorProvider_tostring,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_RichPaletteColorProvider,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0xA8020008,  //  0000  EXBLK	0	#000A
      0x60040018,  //  0001  GETGBL	R1	G24
      0x58080006,  //  0002  LDCONST	R2	K6
      0x880C0107,  //  0003  GETMBR	R3	R0	K7
      0x88100108,  //  0004  GETMBR	R4	R0	K8
      0x7C040600,  //  0005  CALL	R1	3
      0xA8040001,  //  0006  EXBLK	1	1
      0x80040200,  //  0007  RET	1	R1
      0xA8040001,  //  0008  EXBLK	1	1
      0x70020004,  //  0009  JMP		#000F
      0xAC040000,  //  000A  CATCH	R1	0	0
      0x70020001,  //  000B  JMP		#000E
      0x80061200,  //  000C  RET	1	K9
      0x70020000,  //  000D  JMP		#000F
      0xB0080000,  //  000E  RAISE	2	R0	R0
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _parse_palette
********************************************************************/
be_local_closure(class_RichPaletteColorProvider__parse_palette,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_RichPaletteColorProvider,     /* shared constants */
    be_str_weak(_parse_palette),
    &be_const_str_solidified,
    ( &(const binstruction[69]) {  /* code */
      0x8C0C010A,  //  0000  GETMET	R3	R0	K10
      0x7C0C0200,  //  0001  CALL	R3	1
      0x60100012,  //  0002  GETGBL	R4	G18
      0x7C100000,  //  0003  CALL	R4	0
      0x88140107,  //  0004  GETMBR	R5	R0	K7
      0x8C18090B,  //  0005  GETMET	R6	R4	K11
      0x5C200A00,  //  0006  MOVE	R8	R5
      0x7C180400,  //  0007  CALL	R6	2
      0x8C18070C,  //  0008  GETMET	R6	R3	K12
      0x5820000D,  //  0009  LDCONST	R8	K13
      0x5824000E,  //  000A  LDCONST	R9	K14
      0x7C180600,  //  000B  CALL	R6	3
      0x20180D0D,  //  000C  NE	R6	R6	K13
      0x781A0022,  //  000D  JMPF	R6	#0031
      0x5818000D,  //  000E  LDCONST	R6	K13
      0x581C000D,  //  000F  LDCONST	R7	K13
      0x04200B0E,  //  0010  SUB	R8	R5	K14
      0x14200E08,  //  0011  LT	R8	R7	R8
      0x78220007,  //  0012  JMPF	R8	#001B
      0x8C20070C,  //  0013  GETMET	R8	R3	K12
      0x542A0003,  //  0014  LDINT	R10	4
      0x08280E0A,  //  0015  MUL	R10	R7	R10
      0x582C000E,  //  0016  LDCONST	R11	K14
      0x7C200600,  //  0017  CALL	R8	3
      0x00180C08,  //  0018  ADD	R6	R6	R8
      0x001C0F0E,  //  0019  ADD	R7	R7	K14
      0x7001FFF4,  //  001A  JMP		#0010
      0x5820000D,  //  001B  LDCONST	R8	K13
      0x581C000D,  //  001C  LDCONST	R7	K13
      0x14240E05,  //  001D  LT	R9	R7	R5
      0x78260010,  //  001E  JMPF	R9	#0030
      0xB8261E00,  //  001F  GETNGBL	R9	K15
      0x8C241310,  //  0020  GETMET	R9	R9	K16
      0x5C2C1000,  //  0021  MOVE	R11	R8
      0x5830000D,  //  0022  LDCONST	R12	K13
      0x5C340C00,  //  0023  MOVE	R13	R6
      0x5C380200,  //  0024  MOVE	R14	R1
      0x5C3C0400,  //  0025  MOVE	R15	R2
      0x7C240C00,  //  0026  CALL	R9	6
      0x98100E09,  //  0027  SETIDX	R4	R7	R9
      0x8C24070C,  //  0028  GETMET	R9	R3	K12
      0x542E0003,  //  0029  LDINT	R11	4
      0x082C0E0B,  //  002A  MUL	R11	R7	R11
      0x5830000E,  //  002B  LDCONST	R12	K14
      0x7C240600,  //  002C  CALL	R9	3
      0x00201009,  //  002D  ADD	R8	R8	R9
      0x001C0F0E,  //  002E  ADD	R7	R7	K14
      0x7001FFEC,  //  002F  JMP		#001D
      0x70020012,  //  0030  JMP		#0044
      0x5818000D,  //  0031  LDCONST	R6	K13
      0x141C0C05,  //  0032  LT	R7	R6	R5
      0x781E000F,  //  0033  JMPF	R7	#0044
      0x8C1C070C,  //  0034  GETMET	R7	R3	K12
      0x54260003,  //  0035  LDINT	R9	4
      0x08240C09,  //  0036  MUL	R9	R6	R9
      0x5828000E,  //  0037  LDCONST	R10	K14
      0x7C1C0600,  //  0038  CALL	R7	3
      0xB8221E00,  //  0039  GETNGBL	R8	K15
      0x8C201110,  //  003A  GETMET	R8	R8	K16
      0x5C280E00,  //  003B  MOVE	R10	R7
      0x582C000D,  //  003C  LDCONST	R11	K13
      0x543200FE,  //  003D  LDINT	R12	255
      0x5C340200,  //  003E  MOVE	R13	R1
      0x5C380400,  //  003F  MOVE	R14	R2
      0x7C200C00,  //  0040  CALL	R8	6
      0x98100C08,  //  0041  SETIDX	R4	R6	R8
      0x00180D0E,  //  0042  ADD	R6	R6	K14
      0x7001FFED,  //  0043  JMP		#0032
      0x80040800,  //  0044  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: on_param_changed
********************************************************************/
be_local_closure(class_RichPaletteColorProvider_on_param_changed,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_RichPaletteColorProvider,     /* shared constants */
    be_str_weak(on_param_changed),
    &be_const_str_solidified,
    ( &(const binstruction[53]) {  /* code */
      0x1C0C0311,  //  0000  EQ	R3	R1	K17
      0x780E0002,  //  0001  JMPF	R3	#0005
      0x8C0C0112,  //  0002  GETMET	R3	R0	K18
      0x7C0C0200,  //  0003  CALL	R3	1
      0x7002002E,  //  0004  JMP		#0034
      0x1C0C0308,  //  0005  EQ	R3	R1	K8
      0x780E0013,  //  0006  JMPF	R3	#001B
      0x4C0C0000,  //  0007  LDNIL	R3
      0x1C0C0403,  //  0008  EQ	R3	R2	R3
      0x780E0000,  //  0009  JMPF	R3	#000B
      0x80000600,  //  000A  RET	0
      0x140C050D,  //  000B  LT	R3	R2	K13
      0x780E0000,  //  000C  JMPF	R3	#000E
      0xB0060113,  //  000D  RAISE	1	K0	K19
      0x240C050D,  //  000E  GT	R3	R2	K13
      0x780E0009,  //  000F  JMPF	R3	#001A
      0x8C0C010A,  //  0010  GETMET	R3	R0	K10
      0x7C0C0200,  //  0011  CALL	R3	1
      0x4C100000,  //  0012  LDNIL	R4
      0x200C0604,  //  0013  NE	R3	R3	R4
      0x780E0004,  //  0014  JMPF	R3	#001A
      0x8C0C0105,  //  0015  GETMET	R3	R0	K5
      0x5814000D,  //  0016  LDCONST	R5	K13
      0x0418050E,  //  0017  SUB	R6	R2	K14
      0x7C0C0600,  //  0018  CALL	R3	3
      0x90020803,  //  0019  SETMBR	R0	K4	R3
      0x70020018,  //  001A  JMP		#0034
      0x1C0C0302,  //  001B  EQ	R3	R1	K2
      0x740E0001,  //  001C  JMPT	R3	#001F
      0x1C0C0303,  //  001D  EQ	R3	R1	K3
      0x780E0014,  //  001E  JMPF	R3	#0034
      0x880C0102,  //  001F  GETMBR	R3	R0	K2
      0x88100103,  //  0020  GETMBR	R4	R0	K3
      0x4C140000,  //  0021  LDNIL	R5
      0x20140605,  //  0022  NE	R5	R3	R5
      0x7816000F,  //  0023  JMPF	R5	#0034
      0x4C140000,  //  0024  LDNIL	R5
      0x20140805,  //  0025  NE	R5	R4	R5
      0x7816000C,  //  0026  JMPF	R5	#0034
      0x28140604,  //  0027  GE	R5	R3	R4
      0x78160000,  //  0028  JMPF	R5	#002A
      0xB0060114,  //  0029  RAISE	1	K0	K20
      0x8C14010A,  //  002A  GETMET	R5	R0	K10
      0x7C140200,  //  002B  CALL	R5	1
      0x4C180000,  //  002C  LDNIL	R6
      0x20140A06,  //  002D  NE	R5	R5	R6
      0x78160004,  //  002E  JMPF	R5	#0034
      0x8C140105,  //  002F  GETMET	R5	R0	K5
      0x5C1C0600,  //  0030  MOVE	R7	R3
      0x5C200800,  //  0031  MOVE	R8	R4
      0x7C140600,  //  0032  CALL	R5	3
      0x90020805,  //  0033  SETMBR	R0	K4	R5
      0x80000000,  //  0034  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_color_for_value
********************************************************************/
be_local_closure(class_RichPaletteColorProvider_get_color_for_value,   /* name */
  be_nested_proto(
    23,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_RichPaletteColorProvider,     /* shared constants */
    be_str_weak(get_color_for_value),
    &be_const_str_solidified,
    ( &(const binstruction[123]) {  /* code */
      0x8C0C010A,  //  0000  GETMET	R3	R0	K10
      0x7C0C0200,  //  0001  CALL	R3	1
      0x88100102,  //  0002  GETMBR	R4	R0	K2
      0x88140103,  //  0003  GETMBR	R5	R0	K3
      0x88180115,  //  0004  GETMBR	R6	R0	K21
      0x4C1C0000,  //  0005  LDNIL	R7
      0x1C1C0807,  //  0006  EQ	R7	R4	R7
      0x741E0002,  //  0007  JMPT	R7	#000B
      0x4C1C0000,  //  0008  LDNIL	R7
      0x1C1C0A07,  //  0009  EQ	R7	R5	R7
      0x781E0001,  //  000A  JMPF	R7	#000D
      0x4C1C0000,  //  000B  LDNIL	R7
      0x80040E00,  //  000C  RET	1	R7
      0x881C0107,  //  000D  GETMBR	R7	R0	K7
      0x04200F16,  //  000E  SUB	R8	R7	K22
      0x2424110D,  //  000F  GT	R9	R8	K13
      0x78260006,  //  0010  JMPF	R9	#0018
      0x88240104,  //  0011  GETMBR	R9	R0	K4
      0x94241208,  //  0012  GETIDX	R9	R9	R8
      0x28240209,  //  0013  GE	R9	R1	R9
      0x78260000,  //  0014  JMPF	R9	#0016
      0x70020001,  //  0015  JMP		#0018
      0x0420110E,  //  0016  SUB	R8	R8	K14
      0x7001FFF6,  //  0017  JMP		#000F
      0x8C24070C,  //  0018  GETMET	R9	R3	K12
      0x542E0003,  //  0019  LDINT	R11	4
      0x082C100B,  //  001A  MUL	R11	R8	R11
      0x54320003,  //  001B  LDINT	R12	4
      0x7C240600,  //  001C  CALL	R9	3
      0x8C28070C,  //  001D  GETMET	R10	R3	K12
      0x0030110E,  //  001E  ADD	R12	R8	K14
      0x54360003,  //  001F  LDINT	R13	4
      0x0830180D,  //  0020  MUL	R12	R12	R13
      0x54360003,  //  0021  LDINT	R13	4
      0x7C280600,  //  0022  CALL	R10	3
      0x882C0104,  //  0023  GETMBR	R11	R0	K4
      0x942C1608,  //  0024  GETIDX	R11	R11	R8
      0x0030110E,  //  0025  ADD	R12	R8	K14
      0x88340104,  //  0026  GETMBR	R13	R0	K4
      0x94301A0C,  //  0027  GETIDX	R12	R13	R12
      0xB8361E00,  //  0028  GETNGBL	R13	K15
      0x8C341B17,  //  0029  GETMET	R13	R13	K23
      0x5C3C0200,  //  002A  MOVE	R15	R1
      0x5C401600,  //  002B  MOVE	R16	R11
      0x5C441800,  //  002C  MOVE	R17	R12
      0x544A0007,  //  002D  LDINT	R18	8
      0x3C481212,  //  002E  SHR	R18	R9	R18
      0x544E00FE,  //  002F  LDINT	R19	255
      0x2C482413,  //  0030  AND	R18	R18	R19
      0x544E0007,  //  0031  LDINT	R19	8
      0x3C4C1413,  //  0032  SHR	R19	R10	R19
      0x545200FE,  //  0033  LDINT	R20	255
      0x2C4C2614,  //  0034  AND	R19	R19	R20
      0x7C340C00,  //  0035  CALL	R13	6
      0xB83A1E00,  //  0036  GETNGBL	R14	K15
      0x8C381D17,  //  0037  GETMET	R14	R14	K23
      0x5C400200,  //  0038  MOVE	R16	R1
      0x5C441600,  //  0039  MOVE	R17	R11
      0x5C481800,  //  003A  MOVE	R18	R12
      0x544E000F,  //  003B  LDINT	R19	16
      0x3C4C1213,  //  003C  SHR	R19	R9	R19
      0x545200FE,  //  003D  LDINT	R20	255
      0x2C4C2614,  //  003E  AND	R19	R19	R20
      0x5452000F,  //  003F  LDINT	R20	16
      0x3C501414,  //  0040  SHR	R20	R10	R20
      0x545600FE,  //  0041  LDINT	R21	255
      0x2C502815,  //  0042  AND	R20	R20	R21
      0x7C380C00,  //  0043  CALL	R14	6
      0xB83E1E00,  //  0044  GETNGBL	R15	K15
      0x8C3C1F17,  //  0045  GETMET	R15	R15	K23
      0x5C440200,  //  0046  MOVE	R17	R1
      0x5C481600,  //  0047  MOVE	R18	R11
      0x5C4C1800,  //  0048  MOVE	R19	R12
      0x54520017,  //  0049  LDINT	R20	24
      0x3C501214,  //  004A  SHR	R20	R9	R20
      0x545600FE,  //  004B  LDINT	R21	255
      0x2C502815,  //  004C  AND	R20	R20	R21
      0x54560017,  //  004D  LDINT	R21	24
      0x3C541415,  //  004E  SHR	R21	R10	R21
      0x545A00FE,  //  004F  LDINT	R22	255
      0x2C542A16,  //  0050  AND	R21	R21	R22
      0x7C3C0C00,  //  0051  CALL	R15	6
      0x544200FE,  //  0052  LDINT	R16	255
      0x20400C10,  //  0053  NE	R16	R6	R16
      0x7842001A,  //  0054  JMPF	R16	#0070
      0xB8421E00,  //  0055  GETNGBL	R16	K15
      0x8C402117,  //  0056  GETMET	R16	R16	K23
      0x5C481A00,  //  0057  MOVE	R18	R13
      0x584C000D,  //  0058  LDCONST	R19	K13
      0x545200FE,  //  0059  LDINT	R20	255
      0x5854000D,  //  005A  LDCONST	R21	K13
      0x5C580C00,  //  005B  MOVE	R22	R6
      0x7C400C00,  //  005C  CALL	R16	6
      0x5C342000,  //  005D  MOVE	R13	R16
      0xB8421E00,  //  005E  GETNGBL	R16	K15
      0x8C402117,  //  005F  GETMET	R16	R16	K23
      0x5C481C00,  //  0060  MOVE	R18	R14
      0x584C000D,  //  0061  LDCONST	R19	K13
      0x545200FE,  //  0062  LDINT	R20	255
      0x5854000D,  //  0063  LDCONST	R21	K13
      0x5C580C00,  //  0064  MOVE	R22	R6
      0x7C400C00,  //  0065  CALL	R16	6
      0x5C382000,  //  0066  MOVE	R14	R16
      0xB8421E00,  //  0067  GETNGBL	R16	K15
      0x8C402117,  //  0068  GETMET	R16	R16	K23
      0x5C481E00,  //  0069  MOVE	R18	R15
      0x584C000D,  //  006A  LDCONST	R19	K13
      0x545200FE,  //  006B  LDINT	R20	255
      0x5854000D,  //  006C  LDCONST	R21	K13
      0x5C580C00,  //  006D  MOVE	R22	R6
      0x7C400C00,  //  006E  CALL	R16	6
      0x5C3C2000,  //  006F  MOVE	R15	R16
      0x544200FE,  //  0070  LDINT	R16	255
      0x54460017,  //  0071  LDINT	R17	24
      0x38402011,  //  0072  SHL	R16	R16	R17
      0x5446000F,  //  0073  LDINT	R17	16
      0x38441A11,  //  0074  SHL	R17	R13	R17
      0x30402011,  //  0075  OR	R16	R16	R17
      0x54460007,  //  0076  LDINT	R17	8
      0x38441C11,  //  0077  SHL	R17	R14	R17
      0x30402011,  //  0078  OR	R16	R16	R17
      0x3040200F,  //  0079  OR	R16	R16	R15
      0x80042000,  //  007A  RET	1	R16
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _recompute_palette
********************************************************************/
be_local_closure(class_RichPaletteColorProvider__recompute_palette,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_RichPaletteColorProvider,     /* shared constants */
    be_str_weak(_recompute_palette),
    &be_const_str_solidified,
    ( &(const binstruction[41]) {  /* code */
      0x8C04010A,  //  0000  GETMET	R1	R0	K10
      0x7C040200,  //  0001  CALL	R1	1
      0x6008000C,  //  0002  GETGBL	R2	G12
      0x5C0C0200,  //  0003  MOVE	R3	R1
      0x7C080200,  //  0004  CALL	R2	1
      0x540E0003,  //  0005  LDINT	R3	4
      0x0C080403,  //  0006  DIV	R2	R2	R3
      0x90020E02,  //  0007  SETMBR	R0	K7	R2
      0x88080108,  //  0008  GETMBR	R2	R0	K8
      0x880C0102,  //  0009  GETMBR	R3	R0	K2
      0x88100103,  //  000A  GETMBR	R4	R0	K3
      0x4C140000,  //  000B  LDNIL	R5
      0x20140405,  //  000C  NE	R5	R2	R5
      0x78160007,  //  000D  JMPF	R5	#0016
      0x2414050D,  //  000E  GT	R5	R2	K13
      0x78160005,  //  000F  JMPF	R5	#0016
      0x8C140105,  //  0010  GETMET	R5	R0	K5
      0x581C000D,  //  0011  LDCONST	R7	K13
      0x0420050E,  //  0012  SUB	R8	R2	K14
      0x7C140600,  //  0013  CALL	R5	3
      0x90020805,  //  0014  SETMBR	R0	K4	R5
      0x7002000A,  //  0015  JMP		#0021
      0x4C140000,  //  0016  LDNIL	R5
      0x20140605,  //  0017  NE	R5	R3	R5
      0x78160007,  //  0018  JMPF	R5	#0021
      0x4C140000,  //  0019  LDNIL	R5
      0x20140805,  //  001A  NE	R5	R4	R5
      0x78160004,  //  001B  JMPF	R5	#0021
      0x8C140105,  //  001C  GETMET	R5	R0	K5
      0x5C1C0600,  //  001D  MOVE	R7	R3
      0x5C200800,  //  001E  MOVE	R8	R4
      0x7C140600,  //  001F  CALL	R5	3
      0x90020805,  //  0020  SETMBR	R0	K4	R5
      0x88140107,  //  0021  GETMBR	R5	R0	K7
      0x24140B0D,  //  0022  GT	R5	R5	K13
      0x78160003,  //  0023  JMPF	R5	#0028
      0x8C140119,  //  0024  GETMET	R5	R0	K25
      0x581C000D,  //  0025  LDCONST	R7	K13
      0x7C140400,  //  0026  CALL	R5	2
      0x90023005,  //  0027  SETMBR	R0	K24	R5
      0x80040000,  //  0028  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_RichPaletteColorProvider_init,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_RichPaletteColorProvider,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C08051A,  //  0003  GETMET	R2	R2	K26
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x5409FFFE,  //  0006  LDINT	R2	-1
      0x90023002,  //  0007  SETMBR	R0	K24	R2
      0x8808011C,  //  0008  GETMBR	R2	R0	K28
      0x8808051D,  //  0009  GETMBR	R2	R2	K29
      0x90023602,  //  000A  SETMBR	R0	K27	R2
      0x4C080000,  //  000B  LDNIL	R2
      0x90020802,  //  000C  SETMBR	R0	K4	R2
      0x90020F0D,  //  000D  SETMBR	R0	K7	K13
      0xA40A3C00,  //  000E  IMPORT	R2	K30
      0x8C0C051F,  //  000F  GETMET	R3	R2	K31
      0x8814051F,  //  0010  GETMBR	R5	R2	K31
      0x88140B20,  //  0011  GETMBR	R5	R5	K32
      0x7C0C0400,  //  0012  CALL	R3	2
      0x90023E03,  //  0013  SETMBR	R0	K31	R3
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: to_css_gradient
********************************************************************/
be_local_closure(class_RichPaletteColorProvider_to_css_gradient,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_RichPaletteColorProvider,     /* shared constants */
    be_str_weak(to_css_gradient),
    &be_const_str_solidified,
    ( &(const binstruction[47]) {  /* code */
      0x8C04010A,  //  0000  GETMET	R1	R0	K10
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x1C080202,  //  0003  EQ	R2	R1	R2
      0x780A0000,  //  0004  JMPF	R2	#0006
      0x80064200,  //  0005  RET	1	K33
      0x8C080105,  //  0006  GETMET	R2	R0	K5
      0x5810000D,  //  0007  LDCONST	R4	K13
      0x541603E7,  //  0008  LDINT	R5	1000
      0x7C080600,  //  0009  CALL	R2	3
      0x580C0022,  //  000A  LDCONST	R3	K34
      0x5810000D,  //  000B  LDCONST	R4	K13
      0x6014000C,  //  000C  GETGBL	R5	G12
      0x5C180400,  //  000D  MOVE	R6	R2
      0x7C140200,  //  000E  CALL	R5	1
      0x14140805,  //  000F  LT	R5	R4	R5
      0x7816001B,  //  0010  JMPF	R5	#002D
      0x94140404,  //  0011  GETIDX	R5	R2	R4
      0x8C18030C,  //  0012  GETMET	R6	R1	K12
      0x54220003,  //  0013  LDINT	R8	4
      0x08200808,  //  0014  MUL	R8	R4	R8
      0x54260003,  //  0015  LDINT	R9	4
      0x7C180600,  //  0016  CALL	R6	3
      0x541E0007,  //  0017  LDINT	R7	8
      0x3C1C0C07,  //  0018  SHR	R7	R6	R7
      0x542200FE,  //  0019  LDINT	R8	255
      0x2C1C0E08,  //  001A  AND	R7	R7	R8
      0x5422000F,  //  001B  LDINT	R8	16
      0x3C200C08,  //  001C  SHR	R8	R6	R8
      0x542600FE,  //  001D  LDINT	R9	255
      0x2C201009,  //  001E  AND	R8	R8	R9
      0x54260017,  //  001F  LDINT	R9	24
      0x3C240C09,  //  0020  SHR	R9	R6	R9
      0x542A00FE,  //  0021  LDINT	R10	255
      0x2C24120A,  //  0022  AND	R9	R9	R10
      0x60280018,  //  0023  GETGBL	R10	G24
      0x582C0023,  //  0024  LDCONST	R11	K35
      0x5C300E00,  //  0025  MOVE	R12	R7
      0x5C341000,  //  0026  MOVE	R13	R8
      0x5C381200,  //  0027  MOVE	R14	R9
      0x0C3C0B24,  //  0028  DIV	R15	R5	K36
      0x7C280A00,  //  0029  CALL	R10	5
      0x000C060A,  //  002A  ADD	R3	R3	R10
      0x0010090E,  //  002B  ADD	R4	R4	K14
      0x7001FFDE,  //  002C  JMP		#000C
      0x000C0725,  //  002D  ADD	R3	R3	K37
      0x80040600,  //  002E  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(class_RichPaletteColorProvider_start,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_RichPaletteColorProvider,     /* shared constants */
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x8808011C,  //  0003  GETMBR	R2	R0	K28
      0x8804051D,  //  0004  GETMBR	R1	R2	K29
      0x90023601,  //  0005  SETMBR	R0	K27	R1
      0x80040000,  //  0006  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _get_color_at_index
********************************************************************/
be_local_closure(class_RichPaletteColorProvider__get_color_at_index,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_RichPaletteColorProvider,     /* shared constants */
    be_str_weak(_get_color_at_index),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x1408030D,  //  0000  LT	R2	R1	K13
      0x740A0002,  //  0001  JMPT	R2	#0005
      0x88080107,  //  0002  GETMBR	R2	R0	K7
      0x28080202,  //  0003  GE	R2	R1	R2
      0x780A0001,  //  0004  JMPF	R2	#0007
      0x5409FFFE,  //  0005  LDINT	R2	-1
      0x80040400,  //  0006  RET	1	R2
      0x8C08010A,  //  0007  GETMET	R2	R0	K10
      0x7C080200,  //  0008  CALL	R2	1
      0x8C0C050C,  //  0009  GETMET	R3	R2	K12
      0x54160003,  //  000A  LDINT	R5	4
      0x08140205,  //  000B  MUL	R5	R1	R5
      0x5419FFFB,  //  000C  LDINT	R6	-4
      0x7C0C0600,  //  000D  CALL	R3	3
      0x300C0726,  //  000E  OR	R3	R3	K38
      0x80040600,  //  000F  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _get_palette_bytes
********************************************************************/
be_local_closure(class_RichPaletteColorProvider__get_palette_bytes,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_RichPaletteColorProvider,     /* shared constants */
    be_str_weak(_get_palette_bytes),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88040111,  //  0000  GETMBR	R1	R0	K17
      0x4C080000,  //  0001  LDNIL	R2
      0x20080202,  //  0002  NE	R2	R1	R2
      0x780A0001,  //  0003  JMPF	R2	#0006
      0x5C080200,  //  0004  MOVE	R2	R1
      0x70020000,  //  0005  JMP		#0007
      0x88080127,  //  0006  GETMBR	R2	R0	K39
      0x80040400,  //  0007  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: produce_value
********************************************************************/
be_local_closure(class_RichPaletteColorProvider_produce_value,   /* name */
  be_nested_proto(
    28,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_RichPaletteColorProvider,     /* shared constants */
    be_str_weak(produce_value),
    &be_const_str_solidified,
    ( &(const binstruction[236]) {  /* code */
      0x8C0C010A,  //  0000  GETMET	R3	R0	K10
      0x7C0C0200,  //  0001  CALL	R3	1
      0x4C100000,  //  0002  LDNIL	R4
      0x1C100604,  //  0003  EQ	R4	R3	R4
      0x74120002,  //  0004  JMPT	R4	#0008
      0x88100107,  //  0005  GETMBR	R4	R0	K7
      0x14100916,  //  0006  LT	R4	R4	K22
      0x78120001,  //  0007  JMPF	R4	#000A
      0x5411FFFE,  //  0008  LDINT	R4	-1
      0x80040800,  //  0009  RET	1	R4
      0x88100108,  //  000A  GETMBR	R4	R0	K8
      0x88140115,  //  000B  GETMBR	R5	R0	K21
      0x1C18090D,  //  000C  EQ	R6	R4	K13
      0x781A0039,  //  000D  JMPF	R6	#0048
      0x8C18070C,  //  000E  GETMET	R6	R3	K12
      0x5820000D,  //  000F  LDCONST	R8	K13
      0x54260003,  //  0010  LDINT	R9	4
      0x7C180600,  //  0011  CALL	R6	3
      0x541E0007,  //  0012  LDINT	R7	8
      0x3C1C0C07,  //  0013  SHR	R7	R6	R7
      0x542200FE,  //  0014  LDINT	R8	255
      0x2C1C0E08,  //  0015  AND	R7	R7	R8
      0x5422000F,  //  0016  LDINT	R8	16
      0x3C200C08,  //  0017  SHR	R8	R6	R8
      0x542600FE,  //  0018  LDINT	R9	255
      0x2C201009,  //  0019  AND	R8	R8	R9
      0x54260017,  //  001A  LDINT	R9	24
      0x3C240C09,  //  001B  SHR	R9	R6	R9
      0x542A00FE,  //  001C  LDINT	R10	255
      0x2C24120A,  //  001D  AND	R9	R9	R10
      0x542A00FE,  //  001E  LDINT	R10	255
      0x20280A0A,  //  001F  NE	R10	R5	R10
      0x782A001A,  //  0020  JMPF	R10	#003C
      0xB82A1E00,  //  0021  GETNGBL	R10	K15
      0x8C281517,  //  0022  GETMET	R10	R10	K23
      0x5C300E00,  //  0023  MOVE	R12	R7
      0x5834000D,  //  0024  LDCONST	R13	K13
      0x543A00FE,  //  0025  LDINT	R14	255
      0x583C000D,  //  0026  LDCONST	R15	K13
      0x5C400A00,  //  0027  MOVE	R16	R5
      0x7C280C00,  //  0028  CALL	R10	6
      0x5C1C1400,  //  0029  MOVE	R7	R10
      0xB82A1E00,  //  002A  GETNGBL	R10	K15
      0x8C281517,  //  002B  GETMET	R10	R10	K23
      0x5C301000,  //  002C  MOVE	R12	R8
      0x5834000D,  //  002D  LDCONST	R13	K13
      0x543A00FE,  //  002E  LDINT	R14	255
      0x583C000D,  //  002F  LDCONST	R15	K13
      0x5C400A00,  //  0030  MOVE	R16	R5
      0x7C280C00,  //  0031  CALL	R10	6
      0x5C201400,  //  0032  MOVE	R8	R10
      0xB82A1E00,  //  0033  GETNGBL	R10	K15
      0x8C281517,  //  0034  GETMET	R10	R10	K23
      0x5C301200,  //  0035  MOVE	R12	R9
      0x5834000D,  //  0036  LDCONST	R13	K13
      0x543A00FE,  //  0037  LDINT	R14	255
      0x583C000D,  //  0038  LDCONST	R15	K13
      0x5C400A00,  //  0039  MOVE	R16	R5
      0x7C280C00,  //  003A  CALL	R10	6
      0x5C241400,  //  003B  MOVE	R9	R10
      0x542A00FE,  //  003C  LDINT	R10	255
      0x542E0017,  //  003D  LDINT	R11	24
      0x3828140B,  //  003E  SHL	R10	R10	R11
      0x542E000F,  //  003F  LDINT	R11	16
      0x382C0E0B,  //  0040  SHL	R11	R7	R11
      0x3028140B,  //  0041  OR	R10	R10	R11
      0x542E0007,  //  0042  LDINT	R11	8
      0x382C100B,  //  0043  SHL	R11	R8	R11
      0x3028140B,  //  0044  OR	R10	R10	R11
      0x30281409,  //  0045  OR	R10	R10	R9
      0x9002300A,  //  0046  SETMBR	R0	K24	R10
      0x80041400,  //  0047  RET	1	R10
      0x8818011B,  //  0048  GETMBR	R6	R0	K27
      0x04180406,  //  0049  SUB	R6	R2	R6
      0x101C0C04,  //  004A  MOD	R7	R6	R4
      0x88200107,  //  004B  GETMBR	R8	R0	K7
      0x04241116,  //  004C  SUB	R9	R8	K22
      0x2428130D,  //  004D  GT	R10	R9	K13
      0x782A0006,  //  004E  JMPF	R10	#0056
      0x88280104,  //  004F  GETMBR	R10	R0	K4
      0x94281409,  //  0050  GETIDX	R10	R10	R9
      0x28280E0A,  //  0051  GE	R10	R7	R10
      0x782A0000,  //  0052  JMPF	R10	#0054
      0x70020001,  //  0053  JMP		#0056
      0x0424130E,  //  0054  SUB	R9	R9	K14
      0x7001FFF6,  //  0055  JMP		#004D
      0x8C28070C,  //  0056  GETMET	R10	R3	K12
      0x54320003,  //  0057  LDINT	R12	4
      0x0830120C,  //  0058  MUL	R12	R9	R12
      0x54360003,  //  0059  LDINT	R13	4
      0x7C280600,  //  005A  CALL	R10	3
      0x8C2C070C,  //  005B  GETMET	R11	R3	K12
      0x0034130E,  //  005C  ADD	R13	R9	K14
      0x543A0003,  //  005D  LDINT	R14	4
      0x08341A0E,  //  005E  MUL	R13	R13	R14
      0x543A0003,  //  005F  LDINT	R14	4
      0x7C2C0600,  //  0060  CALL	R11	3
      0x88300104,  //  0061  GETMBR	R12	R0	K4
      0x94301809,  //  0062  GETIDX	R12	R12	R9
      0x0034130E,  //  0063  ADD	R13	R9	K14
      0x88380104,  //  0064  GETMBR	R14	R0	K4
      0x94341C0D,  //  0065  GETIDX	R13	R14	R13
      0xB83A1E00,  //  0066  GETNGBL	R14	K15
      0x8C381D17,  //  0067  GETMET	R14	R14	K23
      0x5C400E00,  //  0068  MOVE	R16	R7
      0x5C441800,  //  0069  MOVE	R17	R12
      0x5C481A00,  //  006A  MOVE	R18	R13
      0x544E0007,  //  006B  LDINT	R19	8
      0x3C4C1413,  //  006C  SHR	R19	R10	R19
      0x545200FE,  //  006D  LDINT	R20	255
      0x2C4C2614,  //  006E  AND	R19	R19	R20
      0x54520007,  //  006F  LDINT	R20	8
      0x3C501614,  //  0070  SHR	R20	R11	R20
      0x545600FE,  //  0071  LDINT	R21	255
      0x2C502815,  //  0072  AND	R20	R20	R21
      0x7C380C00,  //  0073  CALL	R14	6
      0xB83E1E00,  //  0074  GETNGBL	R15	K15
      0x8C3C1F17,  //  0075  GETMET	R15	R15	K23
      0x5C440E00,  //  0076  MOVE	R17	R7
      0x5C481800,  //  0077  MOVE	R18	R12
      0x5C4C1A00,  //  0078  MOVE	R19	R13
      0x5452000F,  //  0079  LDINT	R20	16
      0x3C501414,  //  007A  SHR	R20	R10	R20
      0x545600FE,  //  007B  LDINT	R21	255
      0x2C502815,  //  007C  AND	R20	R20	R21
      0x5456000F,  //  007D  LDINT	R21	16
      0x3C541615,  //  007E  SHR	R21	R11	R21
      0x545A00FE,  //  007F  LDINT	R22	255
      0x2C542A16,  //  0080  AND	R21	R21	R22
      0x7C3C0C00,  //  0081  CALL	R15	6
      0xB8421E00,  //  0082  GETNGBL	R16	K15
      0x8C402117,  //  0083  GETMET	R16	R16	K23
      0x5C480E00,  //  0084  MOVE	R18	R7
      0x5C4C1800,  //  0085  MOVE	R19	R12
      0x5C501A00,  //  0086  MOVE	R20	R13
      0x54560017,  //  0087  LDINT	R21	24
      0x3C541415,  //  0088  SHR	R21	R10	R21
      0x545A00FE,  //  0089  LDINT	R22	255
      0x2C542A16,  //  008A  AND	R21	R21	R22
      0x545A0017,  //  008B  LDINT	R22	24
      0x3C581616,  //  008C  SHR	R22	R11	R22
      0x545E00FE,  //  008D  LDINT	R23	255
      0x2C582C17,  //  008E  AND	R22	R22	R23
      0x7C400C00,  //  008F  CALL	R16	6
      0x8844011F,  //  0090  GETMBR	R17	R0	K31
      0x8C482328,  //  0091  GETMET	R18	R17	K40
      0x54520007,  //  0092  LDINT	R20	8
      0x3C501414,  //  0093  SHR	R20	R10	R20
      0x545600FE,  //  0094  LDINT	R21	255
      0x2C502815,  //  0095  AND	R20	R20	R21
      0x5456000F,  //  0096  LDINT	R21	16
      0x3C541415,  //  0097  SHR	R21	R10	R21
      0x545A00FE,  //  0098  LDINT	R22	255
      0x2C542A16,  //  0099  AND	R21	R21	R22
      0x545A0017,  //  009A  LDINT	R22	24
      0x3C581416,  //  009B  SHR	R22	R10	R22
      0x545E00FE,  //  009C  LDINT	R23	255
      0x2C582C17,  //  009D  AND	R22	R22	R23
      0x7C480800,  //  009E  CALL	R18	4
      0x88482329,  //  009F  GETMBR	R18	R17	K41
      0x8C4C2328,  //  00A0  GETMET	R19	R17	K40
      0x54560007,  //  00A1  LDINT	R21	8
      0x3C541615,  //  00A2  SHR	R21	R11	R21
      0x545A00FE,  //  00A3  LDINT	R22	255
      0x2C542A16,  //  00A4  AND	R21	R21	R22
      0x545A000F,  //  00A5  LDINT	R22	16
      0x3C581616,  //  00A6  SHR	R22	R11	R22
      0x545E00FE,  //  00A7  LDINT	R23	255
      0x2C582C17,  //  00A8  AND	R22	R22	R23
      0x545E0017,  //  00A9  LDINT	R23	24
      0x3C5C1617,  //  00AA  SHR	R23	R11	R23
      0x546200FE,  //  00AB  LDINT	R24	255
      0x2C5C2E18,  //  00AC  AND	R23	R23	R24
      0x7C4C0800,  //  00AD  CALL	R19	4
      0x884C2329,  //  00AE  GETMBR	R19	R17	K41
      0xB8521E00,  //  00AF  GETNGBL	R20	K15
      0x8C502917,  //  00B0  GETMET	R20	R20	K23
      0x5C580E00,  //  00B1  MOVE	R22	R7
      0x5C5C1800,  //  00B2  MOVE	R23	R12
      0x5C601A00,  //  00B3  MOVE	R24	R13
      0x5C642400,  //  00B4  MOVE	R25	R18
      0x5C682600,  //  00B5  MOVE	R26	R19
      0x7C500C00,  //  00B6  CALL	R20	6
      0x8C542328,  //  00B7  GETMET	R21	R17	K40
      0x5C5C1C00,  //  00B8  MOVE	R23	R14
      0x5C601E00,  //  00B9  MOVE	R24	R15
      0x5C642000,  //  00BA  MOVE	R25	R16
      0x7C540800,  //  00BB  CALL	R21	4
      0x8C54232A,  //  00BC  GETMET	R21	R17	K42
      0x5C5C2800,  //  00BD  MOVE	R23	R20
      0x7C540400,  //  00BE  CALL	R21	2
      0x8838232B,  //  00BF  GETMBR	R14	R17	K43
      0x883C232C,  //  00C0  GETMBR	R15	R17	K44
      0x8840232D,  //  00C1  GETMBR	R16	R17	K45
      0x545600FE,  //  00C2  LDINT	R21	255
      0x20540A15,  //  00C3  NE	R21	R5	R21
      0x7856001A,  //  00C4  JMPF	R21	#00E0
      0xB8561E00,  //  00C5  GETNGBL	R21	K15
      0x8C542B17,  //  00C6  GETMET	R21	R21	K23
      0x5C5C1C00,  //  00C7  MOVE	R23	R14
      0x5860000D,  //  00C8  LDCONST	R24	K13
      0x546600FE,  //  00C9  LDINT	R25	255
      0x5868000D,  //  00CA  LDCONST	R26	K13
      0x5C6C0A00,  //  00CB  MOVE	R27	R5
      0x7C540C00,  //  00CC  CALL	R21	6
      0x5C382A00,  //  00CD  MOVE	R14	R21
      0xB8561E00,  //  00CE  GETNGBL	R21	K15
      0x8C542B17,  //  00CF  GETMET	R21	R21	K23
      0x5C5C1E00,  //  00D0  MOVE	R23	R15
      0x5860000D,  //  00D1  LDCONST	R24	K13
      0x546600FE,  //  00D2  LDINT	R25	255
      0x5868000D,  //  00D3  LDCONST	R26	K13
      0x5C6C0A00,  //  00D4  MOVE	R27	R5
      0x7C540C00,  //  00D5  CALL	R21	6
      0x5C3C2A00,  //  00D6  MOVE	R15	R21
      0xB8561E00,  //  00D7  GETNGBL	R21	K15
      0x8C542B17,  //  00D8  GETMET	R21	R21	K23
      0x5C5C2000,  //  00D9  MOVE	R23	R16
      0x5860000D,  //  00DA  LDCONST	R24	K13
      0x546600FE,  //  00DB  LDINT	R25	255
      0x5868000D,  //  00DC  LDCONST	R26	K13
      0x5C6C0A00,  //  00DD  MOVE	R27	R5
      0x7C540C00,  //  00DE  CALL	R21	6
      0x5C402A00,  //  00DF  MOVE	R16	R21
      0x545600FE,  //  00E0  LDINT	R21	255
      0x545A0017,  //  00E1  LDINT	R22	24
      0x38542A16,  //  00E2  SHL	R21	R21	R22
      0x545A000F,  //  00E3  LDINT	R22	16
      0x38581C16,  //  00E4  SHL	R22	R14	R22
      0x30542A16,  //  00E5  OR	R21	R21	R22
      0x545A0007,  //  00E6  LDINT	R22	8
      0x38581E16,  //  00E7  SHL	R22	R15	R22
      0x30542A16,  //  00E8  OR	R21	R21	R22
      0x30542A10,  //  00E9  OR	R21	R21	R16
      0x90023015,  //  00EA  SETMBR	R0	K24	R21
      0x80042A00,  //  00EB  RET	1	R21
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: RichPaletteColorProvider
********************************************************************/
extern const bclass be_class_ColorProvider;
be_local_class(RichPaletteColorProvider,
    5,
    &be_class_ColorProvider,
    be_nested_map(19,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(light_state, -1), be_const_var(3) },
        { be_const_key_weak(_parse_palette, -1), be_const_closure(class_RichPaletteColorProvider__parse_palette_closure) },
        { be_const_key_weak(tostring, -1), be_const_closure(class_RichPaletteColorProvider_tostring_closure) },
        { be_const_key_weak(PARAMS, 15), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(range_min, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(0) },
    }))    ) } )) },
        { be_const_key_weak(transition_type, 0), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(enum, 1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(2,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(1),
        be_const_int(5),
    }))    ) } )) },
        { be_const_key_weak(default, -1), be_const_int(5) },
    }))    ) } )) },
        { be_const_key_weak(palette, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_nil() },
        { be_const_key_weak(type, -1), be_nested_str_weak(bytes) },
    }))    ) } )) },
        { be_const_key_weak(cycle_period, 1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(5000) },
        { be_const_key_weak(min, -1), be_const_int(0) },
    }))    ) } )) },
        { be_const_key_weak(brightness, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(255) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(range_max, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(100) },
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(slots_arr, 1), be_const_var(0) },
        { be_const_key_weak(_get_palette_bytes, -1), be_const_closure(class_RichPaletteColorProvider__get_palette_bytes_closure) },
        { be_const_key_weak(get_color_for_value, 9), be_const_closure(class_RichPaletteColorProvider_get_color_for_value_closure) },
        { be_const_key_weak(_DEFAULT_PALETTE, -1), be_const_bytes_instance(00FF000024FFA50049FFFF006E00FF00920000FFB74B0082DBEE82EEFFFF0000) },
        { be_const_key_weak(_recompute_palette, -1), be_const_closure(class_RichPaletteColorProvider__recompute_palette_closure) },
        { be_const_key_weak(current_color, -1), be_const_var(2) },
        { be_const_key_weak(_get_color_at_index, -1), be_const_closure(class_RichPaletteColorProvider__get_color_at_index_closure) },
        { be_const_key_weak(to_css_gradient, -1), be_const_closure(class_RichPaletteColorProvider_to_css_gradient_closure) },
        { be_const_key_weak(slots, -1), be_const_var(1) },
        { be_const_key_weak(init, 12), be_const_closure(class_RichPaletteColorProvider_init_closure) },
        { be_const_key_weak(cycle_start, 10), be_const_var(4) },
        { be_const_key_weak(start, -1), be_const_closure(class_RichPaletteColorProvider_start_closure) },
        { be_const_key_weak(set_range, 5), be_const_closure(class_RichPaletteColorProvider_set_range_closure) },
        { be_const_key_weak(produce_value, -1), be_const_closure(class_RichPaletteColorProvider_produce_value_closure) },
        { be_const_key_weak(on_param_changed, 0), be_const_closure(class_RichPaletteColorProvider_on_param_changed_closure) },
    })),
    be_str_weak(RichPaletteColorProvider)
);

/********************************************************************
** Solidified function: scale_oscillate
********************************************************************/
be_local_closure(scale_oscillate,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(scale_animation),
    /* K2   */  be_nested_str_weak(scale_mode),
    /* K3   */  be_const_int(1),
    /* K4   */  be_nested_str_weak(scale_speed),
    }),
    be_str_weak(scale_oscillate),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x90060503,  //  0004  SETMBR	R1	K2	K3
      0x540A007F,  //  0005  LDINT	R2	128
      0x90060802,  //  0006  SETMBR	R1	K4	R2
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: wave_rainbow_sine
********************************************************************/
be_local_closure(wave_rainbow_sine,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(wave_animation),
    /* K2   */  be_nested_str_weak(rich_palette),
    /* K3   */  be_nested_str_weak(palette),
    /* K4   */  be_nested_str_weak(PALETTE_RAINBOW),
    /* K5   */  be_nested_str_weak(cycle_period),
    /* K6   */  be_nested_str_weak(transition_type),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str_weak(brightness),
    /* K9   */  be_nested_str_weak(set_range),
    /* K10  */  be_const_int(0),
    /* K11  */  be_nested_str_weak(color),
    /* K12  */  be_nested_str_weak(wave_type),
    /* K13  */  be_nested_str_weak(frequency),
    /* K14  */  be_nested_str_weak(wave_speed),
    }),
    be_str_weak(wave_rainbow_sine),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0xB80A0000,  //  0004  GETNGBL	R2	K0
      0x8C080502,  //  0005  GETMET	R2	R2	K2
      0x5C100000,  //  0006  MOVE	R4	R0
      0x7C080400,  //  0007  CALL	R2	2
      0xB80E0000,  //  0008  GETNGBL	R3	K0
      0x880C0704,  //  0009  GETMBR	R3	R3	K4
      0x900A0603,  //  000A  SETMBR	R2	K3	R3
      0x540E1387,  //  000B  LDINT	R3	5000
      0x900A0A03,  //  000C  SETMBR	R2	K5	R3
      0x900A0D07,  //  000D  SETMBR	R2	K6	K7
      0x540E00FE,  //  000E  LDINT	R3	255
      0x900A1003,  //  000F  SETMBR	R2	K8	R3
      0x8C0C0509,  //  0010  GETMET	R3	R2	K9
      0x5814000A,  //  0011  LDCONST	R5	K10
      0x541A00FE,  //  0012  LDINT	R6	255
      0x7C0C0600,  //  0013  CALL	R3	3
      0x90061602,  //  0014  SETMBR	R1	K11	R2
      0x9006190A,  //  0015  SETMBR	R1	K12	K10
      0x540E001F,  //  0016  LDINT	R3	32
      0x90061A03,  //  0017  SETMBR	R1	K13	R3
      0x540E0031,  //  0018  LDINT	R3	50
      0x90061C03,  //  0019  SETMBR	R1	K14	R3
      0x80040200,  //  001A  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: trigger_event
********************************************************************/
be_local_closure(trigger_event,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(event_manager),
    /* K2   */  be_nested_str_weak(trigger_event),
    }),
    be_str_weak(trigger_event),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x88080501,  //  0001  GETMBR	R2	R2	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0x5C100000,  //  0003  MOVE	R4	R0
      0x5C140200,  //  0004  MOVE	R5	R1
      0x7C080600,  //  0005  CALL	R2	3
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/

// compact class 'ScaleAnimation' ktab size: 42, total: 83 (saved 328 bytes)
static const bvalue be_ktab_class_ScaleAnimation[42] = {
  /* K0   */  be_nested_str_weak(scale_mode),
  /* K1   */  be_nested_str_weak(scale_factor),
  /* K2   */  be_const_int(0),
  /* K3   */  be_const_int(1),
  /* K4   */  be_nested_str_weak(_sine),
  /* K5   */  be_nested_str_weak(scale_phase),
  /* K6   */  be_nested_str_weak(tasmota),
  /* K7   */  be_nested_str_weak(scale_uint),
  /* K8   */  be_const_int(2),
  /* K9   */  be_nested_str_weak(engine),
  /* K10  */  be_nested_str_weak(get_strip_length),
  /* K11  */  be_nested_str_weak(current_colors),
  /* K12  */  be_nested_str_weak(_initialize_buffers),
  /* K13  */  be_nested_str_weak(source_animation),
  /* K14  */  be_nested_str_weak(scale_center),
  /* K15  */  be_nested_str_weak(interpolation),
  /* K16  */  be_nested_str_weak(source_frame),
  /* K17  */  be_nested_str_weak(clear),
  /* K18  */  be_nested_str_weak(render),
  /* K19  */  be_nested_str_weak(_get_current_scale_factor),
  /* K20  */  be_nested_str_weak(get_pixel_color),
  /* K21  */  be_const_int(-16777216),
  /* K22  */  be_nested_str_weak(_interpolate_colors),
  /* K23  */  be_nested_str_weak(scale_speed),
  /* K24  */  be_nested_str_weak(start_time),
  /* K25  */  be_nested_str_weak(is_running),
  /* K26  */  be_nested_str_weak(start),
  /* K27  */  be_nested_str_weak(update),
  /* K28  */  be_nested_str_weak(_calculate_scale),
  /* K29  */  be_nested_str_weak(static),
  /* K30  */  be_nested_str_weak(oscillate),
  /* K31  */  be_nested_str_weak(grow),
  /* K32  */  be_nested_str_weak(shrink),
  /* K33  */  be_nested_str_weak(unknown),
  /* K34  */  be_nested_str_weak(ScaleAnimation_X28_X25s_X2C_X20factor_X3D_X25s_X2C_X20speed_X3D_X25s_X29),
  /* K35  */  be_nested_str_weak(init),
  /* K36  */  be_nested_str_weak(time_ms),
  /* K37  */  be_nested_str_weak(animation),
  /* K38  */  be_nested_str_weak(frame_buffer),
  /* K39  */  be_nested_str_weak(resize),
  /* K40  */  be_nested_str_weak(width),
  /* K41  */  be_nested_str_weak(set_pixel_color),
};


extern const bclass be_class_ScaleAnimation;

/********************************************************************
** Solidified function: _get_current_scale_factor
********************************************************************/
be_local_closure(class_ScaleAnimation__get_current_scale_factor,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ScaleAnimation,     /* shared constants */
    be_str_weak(_get_current_scale_factor),
    &be_const_str_solidified,
    ( &(const binstruction[45]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x1C0C0302,  //  0002  EQ	R3	R1	K2
      0x780E0001,  //  0003  JMPF	R3	#0006
      0x80040400,  //  0004  RET	1	R2
      0x70020025,  //  0005  JMP		#002C
      0x1C0C0303,  //  0006  EQ	R3	R1	K3
      0x780E000C,  //  0007  JMPF	R3	#0015
      0x8C0C0104,  //  0008  GETMET	R3	R0	K4
      0x88140105,  //  0009  GETMBR	R5	R0	K5
      0x7C0C0400,  //  000A  CALL	R3	2
      0xB8120C00,  //  000B  GETNGBL	R4	K6
      0x8C100907,  //  000C  GETMET	R4	R4	K7
      0x5C180600,  //  000D  MOVE	R6	R3
      0x581C0002,  //  000E  LDCONST	R7	K2
      0x542200FE,  //  000F  LDINT	R8	255
      0x5426003F,  //  0010  LDINT	R9	64
      0x542A00FE,  //  0011  LDINT	R10	255
      0x7C100C00,  //  0012  CALL	R4	6
      0x80040800,  //  0013  RET	1	R4
      0x70020016,  //  0014  JMP		#002C
      0x1C0C0308,  //  0015  EQ	R3	R1	K8
      0x780E0009,  //  0016  JMPF	R3	#0021
      0xB80E0C00,  //  0017  GETNGBL	R3	K6
      0x8C0C0707,  //  0018  GETMET	R3	R3	K7
      0x88140105,  //  0019  GETMBR	R5	R0	K5
      0x58180002,  //  001A  LDCONST	R6	K2
      0x541E00FE,  //  001B  LDINT	R7	255
      0x5422003F,  //  001C  LDINT	R8	64
      0x542600FE,  //  001D  LDINT	R9	255
      0x7C0C0C00,  //  001E  CALL	R3	6
      0x80040600,  //  001F  RET	1	R3
      0x7002000A,  //  0020  JMP		#002C
      0xB80E0C00,  //  0021  GETNGBL	R3	K6
      0x8C0C0707,  //  0022  GETMET	R3	R3	K7
      0x541600FE,  //  0023  LDINT	R5	255
      0x88180105,  //  0024  GETMBR	R6	R0	K5
      0x04140A06,  //  0025  SUB	R5	R5	R6
      0x58180002,  //  0026  LDCONST	R6	K2
      0x541E00FE,  //  0027  LDINT	R7	255
      0x5422003F,  //  0028  LDINT	R8	64
      0x542600FE,  //  0029  LDINT	R9	255
      0x7C0C0C00,  //  002A  CALL	R3	6
      0x80040600,  //  002B  RET	1	R3
      0x80000000,  //  002C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _calculate_scale
********************************************************************/
be_local_closure(class_ScaleAnimation__calculate_scale,   /* name */
  be_nested_proto(
    21,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ScaleAnimation,     /* shared constants */
    be_str_weak(_calculate_scale),
    &be_const_str_solidified,
    ( &(const binstruction[110]) {  /* code */
      0x88040109,  //  0000  GETMBR	R1	R0	K9
      0x8C04030A,  //  0001  GETMET	R1	R1	K10
      0x7C040200,  //  0002  CALL	R1	1
      0x6008000C,  //  0003  GETGBL	R2	G12
      0x880C010B,  //  0004  GETMBR	R3	R0	K11
      0x7C080200,  //  0005  CALL	R2	1
      0x20080401,  //  0006  NE	R2	R2	R1
      0x780A0001,  //  0007  JMPF	R2	#000A
      0x8C08010C,  //  0008  GETMET	R2	R0	K12
      0x7C080200,  //  0009  CALL	R2	1
      0x8808010D,  //  000A  GETMBR	R2	R0	K13
      0x880C010E,  //  000B  GETMBR	R3	R0	K14
      0x8810010F,  //  000C  GETMBR	R4	R0	K15
      0x88140110,  //  000D  GETMBR	R5	R0	K16
      0x8C140B11,  //  000E  GETMET	R5	R5	K17
      0x7C140200,  //  000F  CALL	R5	1
      0x4C140000,  //  0010  LDNIL	R5
      0x20140405,  //  0011  NE	R5	R2	R5
      0x78160003,  //  0012  JMPF	R5	#0017
      0x8C140512,  //  0013  GETMET	R5	R2	K18
      0x881C0110,  //  0014  GETMBR	R7	R0	K16
      0x58200002,  //  0015  LDCONST	R8	K2
      0x7C140600,  //  0016  CALL	R5	3
      0x8C140113,  //  0017  GETMET	R5	R0	K19
      0x7C140200,  //  0018  CALL	R5	1
      0xB81A0C00,  //  0019  GETNGBL	R6	K6
      0x8C180D07,  //  001A  GETMET	R6	R6	K7
      0x5C200600,  //  001B  MOVE	R8	R3
      0x58240002,  //  001C  LDCONST	R9	K2
      0x542A00FE,  //  001D  LDINT	R10	255
      0x582C0002,  //  001E  LDCONST	R11	K2
      0x04300303,  //  001F  SUB	R12	R1	K3
      0x7C180C00,  //  0020  CALL	R6	6
      0x581C0002,  //  0021  LDCONST	R7	K2
      0x14200E01,  //  0022  LT	R8	R7	R1
      0x78220048,  //  0023  JMPF	R8	#006D
      0x04200E06,  //  0024  SUB	R8	R7	R6
      0xB8260C00,  //  0025  GETNGBL	R9	K6
      0x8C241307,  //  0026  GETMET	R9	R9	K7
      0x542E007F,  //  0027  LDINT	R11	128
      0x082C100B,  //  0028  MUL	R11	R8	R11
      0x58300002,  //  0029  LDCONST	R12	K2
      0x5436007F,  //  002A  LDINT	R13	128
      0x543A007F,  //  002B  LDINT	R14	128
      0x08341A0E,  //  002C  MUL	R13	R13	R14
      0x58380002,  //  002D  LDCONST	R14	K2
      0x543E007F,  //  002E  LDINT	R15	128
      0x083C0A0F,  //  002F  MUL	R15	R5	R15
      0x7C240C00,  //  0030  CALL	R9	6
      0x542A007F,  //  0031  LDINT	R10	128
      0x0C24120A,  //  0032  DIV	R9	R9	R10
      0x00280C09,  //  0033  ADD	R10	R6	R9
      0x1C2C0902,  //  0034  EQ	R11	R4	K2
      0x782E000D,  //  0035  JMPF	R11	#0044
      0x282C1502,  //  0036  GE	R11	R10	K2
      0x782E0008,  //  0037  JMPF	R11	#0041
      0x142C1401,  //  0038  LT	R11	R10	R1
      0x782E0006,  //  0039  JMPF	R11	#0041
      0x882C010B,  //  003A  GETMBR	R11	R0	K11
      0x88300110,  //  003B  GETMBR	R12	R0	K16
      0x8C301914,  //  003C  GETMET	R12	R12	K20
      0x5C381400,  //  003D  MOVE	R14	R10
      0x7C300400,  //  003E  CALL	R12	2
      0x982C0E0C,  //  003F  SETIDX	R11	R7	R12
      0x70020001,  //  0040  JMP		#0043
      0x882C010B,  //  0041  GETMBR	R11	R0	K11
      0x982C0F15,  //  0042  SETIDX	R11	R7	K21
      0x70020026,  //  0043  JMP		#006B
      0x282C1502,  //  0044  GE	R11	R10	K2
      0x782E0022,  //  0045  JMPF	R11	#0069
      0x042C0303,  //  0046  SUB	R11	R1	K3
      0x142C140B,  //  0047  LT	R11	R10	R11
      0x782E001F,  //  0048  JMPF	R11	#0069
      0x602C0009,  //  0049  GETGBL	R11	G9
      0x5C301400,  //  004A  MOVE	R12	R10
      0x7C2C0200,  //  004B  CALL	R11	1
      0x60300009,  //  004C  GETGBL	R12	G9
      0x0434140B,  //  004D  SUB	R13	R10	R11
      0x543A00FF,  //  004E  LDINT	R14	256
      0x08341A0E,  //  004F  MUL	R13	R13	R14
      0x7C300200,  //  0050  CALL	R12	1
      0x28341702,  //  0051  GE	R13	R11	K2
      0x78360012,  //  0052  JMPF	R13	#0066
      0x04340303,  //  0053  SUB	R13	R1	K3
      0x1434160D,  //  0054  LT	R13	R11	R13
      0x7836000F,  //  0055  JMPF	R13	#0066
      0x88340110,  //  0056  GETMBR	R13	R0	K16
      0x8C341B14,  //  0057  GETMET	R13	R13	K20
      0x5C3C1600,  //  0058  MOVE	R15	R11
      0x7C340400,  //  0059  CALL	R13	2
      0x88380110,  //  005A  GETMBR	R14	R0	K16
      0x8C381D14,  //  005B  GETMET	R14	R14	K20
      0x00401703,  //  005C  ADD	R16	R11	K3
      0x7C380400,  //  005D  CALL	R14	2
      0x883C010B,  //  005E  GETMBR	R15	R0	K11
      0x8C400116,  //  005F  GETMET	R16	R0	K22
      0x5C481A00,  //  0060  MOVE	R18	R13
      0x5C4C1C00,  //  0061  MOVE	R19	R14
      0x5C501800,  //  0062  MOVE	R20	R12
      0x7C400800,  //  0063  CALL	R16	4
      0x983C0E10,  //  0064  SETIDX	R15	R7	R16
      0x70020001,  //  0065  JMP		#0068
      0x8834010B,  //  0066  GETMBR	R13	R0	K11
      0x98340F15,  //  0067  SETIDX	R13	R7	K21
      0x70020001,  //  0068  JMP		#006B
      0x882C010B,  //  0069  GETMBR	R11	R0	K11
      0x982C0F15,  //  006A  SETIDX	R11	R7	K21
      0x001C0F03,  //  006B  ADD	R7	R7	K3
      0x7001FFB4,  //  006C  JMP		#0022
      0x80000000,  //  006D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update
********************************************************************/
be_local_closure(class_ScaleAnimation_update,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ScaleAnimation,     /* shared constants */
    be_str_weak(update),
    &be_const_str_solidified,
    ( &(const binstruction[40]) {  /* code */
      0x88080117,  //  0000  GETMBR	R2	R0	K23
      0x880C0100,  //  0001  GETMBR	R3	R0	K0
      0x8810010D,  //  0002  GETMBR	R4	R0	K13
      0x24140502,  //  0003  GT	R5	R2	K2
      0x78160013,  //  0004  JMPF	R5	#0019
      0x24140702,  //  0005  GT	R5	R3	K2
      0x78160011,  //  0006  JMPF	R5	#0019
      0x88140118,  //  0007  GETMBR	R5	R0	K24
      0x04140205,  //  0008  SUB	R5	R1	R5
      0xB81A0C00,  //  0009  GETNGBL	R6	K6
      0x8C180D07,  //  000A  GETMET	R6	R6	K7
      0x5C200400,  //  000B  MOVE	R8	R2
      0x58240002,  //  000C  LDCONST	R9	K2
      0x542A00FE,  //  000D  LDINT	R10	255
      0x582C0002,  //  000E  LDCONST	R11	K2
      0x58300008,  //  000F  LDCONST	R12	K8
      0x7C180C00,  //  0010  CALL	R6	6
      0x241C0D02,  //  0011  GT	R7	R6	K2
      0x781E0005,  //  0012  JMPF	R7	#0019
      0x081C0A06,  //  0013  MUL	R7	R5	R6
      0x542203E7,  //  0014  LDINT	R8	1000
      0x0C1C0E08,  //  0015  DIV	R7	R7	R8
      0x542200FF,  //  0016  LDINT	R8	256
      0x101C0E08,  //  0017  MOD	R7	R7	R8
      0x90020A07,  //  0018  SETMBR	R0	K5	R7
      0x4C140000,  //  0019  LDNIL	R5
      0x20140805,  //  001A  NE	R5	R4	R5
      0x78160007,  //  001B  JMPF	R5	#0024
      0x88140919,  //  001C  GETMBR	R5	R4	K25
      0x74160002,  //  001D  JMPT	R5	#0021
      0x8C14091A,  //  001E  GETMET	R5	R4	K26
      0x881C0118,  //  001F  GETMBR	R7	R0	K24
      0x7C140400,  //  0020  CALL	R5	2
      0x8C14091B,  //  0021  GETMET	R5	R4	K27
      0x5C1C0200,  //  0022  MOVE	R7	R1
      0x7C140400,  //  0023  CALL	R5	2
      0x8C14011C,  //  0024  GETMET	R5	R0	K28
      0x7C140200,  //  0025  CALL	R5	1
      0x50140200,  //  0026  LDBOOL	R5	1	0
      0x80040A00,  //  0027  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_ScaleAnimation_tostring,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ScaleAnimation,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x60040012,  //  0000  GETGBL	R1	G18
      0x7C040000,  //  0001  CALL	R1	0
      0x4008031D,  //  0002  CONNECT	R2	R1	K29
      0x4008031E,  //  0003  CONNECT	R2	R1	K30
      0x4008031F,  //  0004  CONNECT	R2	R1	K31
      0x40080320,  //  0005  CONNECT	R2	R1	K32
      0x88080100,  //  0006  GETMBR	R2	R0	K0
      0x880C0101,  //  0007  GETMBR	R3	R0	K1
      0x88100117,  //  0008  GETMBR	R4	R0	K23
      0x94140202,  //  0009  GETIDX	R5	R1	R2
      0x4C180000,  //  000A  LDNIL	R6
      0x20140A06,  //  000B  NE	R5	R5	R6
      0x78160001,  //  000C  JMPF	R5	#000F
      0x94140202,  //  000D  GETIDX	R5	R1	R2
      0x70020000,  //  000E  JMP		#0010
      0x58140021,  //  000F  LDCONST	R5	K33
      0x60180018,  //  0010  GETGBL	R6	G24
      0x581C0022,  //  0011  LDCONST	R7	K34
      0x5C200A00,  //  0012  MOVE	R8	R5
      0x5C240600,  //  0013  MOVE	R9	R3
      0x5C280800,  //  0014  MOVE	R10	R4
      0x7C180800,  //  0015  CALL	R6	4
      0x80040C00,  //  0016  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: on_param_changed
********************************************************************/
be_local_closure(class_ScaleAnimation_on_param_changed,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ScaleAnimation,     /* shared constants */
    be_str_weak(on_param_changed),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_ScaleAnimation_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ScaleAnimation,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080523,  //  0003  GETMET	R2	R2	K35
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x90020B02,  //  0006  SETMBR	R0	K5	K2
      0x88080109,  //  0007  GETMBR	R2	R0	K9
      0x88080524,  //  0008  GETMBR	R2	R2	K36
      0x90023002,  //  0009  SETMBR	R0	K24	R2
      0x8C08010C,  //  000A  GETMET	R2	R0	K12
      0x7C080200,  //  000B  CALL	R2	1
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _sine
********************************************************************/
be_local_closure(class_ScaleAnimation__sine,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ScaleAnimation,     /* shared constants */
    be_str_weak(_sine),
    &be_const_str_solidified,
    ( &(const binstruction[54]) {  /* code */
      0x540A003F,  //  0000  LDINT	R2	64
      0x10080202,  //  0001  MOD	R2	R1	R2
      0x540E003F,  //  0002  LDINT	R3	64
      0x140C0203,  //  0003  LT	R3	R1	R3
      0x780E0009,  //  0004  JMPF	R3	#000F
      0xB80E0C00,  //  0005  GETNGBL	R3	K6
      0x8C0C0707,  //  0006  GETMET	R3	R3	K7
      0x5C140400,  //  0007  MOVE	R5	R2
      0x58180002,  //  0008  LDCONST	R6	K2
      0x541E003F,  //  0009  LDINT	R7	64
      0x5422007F,  //  000A  LDINT	R8	128
      0x542600FE,  //  000B  LDINT	R9	255
      0x7C0C0C00,  //  000C  CALL	R3	6
      0x80040600,  //  000D  RET	1	R3
      0x70020025,  //  000E  JMP		#0035
      0x540E007F,  //  000F  LDINT	R3	128
      0x140C0203,  //  0010  LT	R3	R1	R3
      0x780E000A,  //  0011  JMPF	R3	#001D
      0xB80E0C00,  //  0012  GETNGBL	R3	K6
      0x8C0C0707,  //  0013  GETMET	R3	R3	K7
      0x5416007F,  //  0014  LDINT	R5	128
      0x04140A01,  //  0015  SUB	R5	R5	R1
      0x58180002,  //  0016  LDCONST	R6	K2
      0x541E003F,  //  0017  LDINT	R7	64
      0x5422007F,  //  0018  LDINT	R8	128
      0x542600FE,  //  0019  LDINT	R9	255
      0x7C0C0C00,  //  001A  CALL	R3	6
      0x80040600,  //  001B  RET	1	R3
      0x70020017,  //  001C  JMP		#0035
      0x540E00BF,  //  001D  LDINT	R3	192
      0x140C0203,  //  001E  LT	R3	R1	R3
      0x780E000A,  //  001F  JMPF	R3	#002B
      0xB80E0C00,  //  0020  GETNGBL	R3	K6
      0x8C0C0707,  //  0021  GETMET	R3	R3	K7
      0x5416007F,  //  0022  LDINT	R5	128
      0x04140205,  //  0023  SUB	R5	R1	R5
      0x58180002,  //  0024  LDCONST	R6	K2
      0x541E003F,  //  0025  LDINT	R7	64
      0x5422007F,  //  0026  LDINT	R8	128
      0x58240002,  //  0027  LDCONST	R9	K2
      0x7C0C0C00,  //  0028  CALL	R3	6
      0x80040600,  //  0029  RET	1	R3
      0x70020009,  //  002A  JMP		#0035
      0xB80E0C00,  //  002B  GETNGBL	R3	K6
      0x8C0C0707,  //  002C  GETMET	R3	R3	K7
      0x541600FF,  //  002D  LDINT	R5	256
      0x04140A01,  //  002E  SUB	R5	R5	R1
      0x58180002,  //  002F  LDCONST	R6	K2
      0x541E003F,  //  0030  LDINT	R7	64
      0x5422007F,  //  0031  LDINT	R8	128
      0x58240002,  //  0032  LDCONST	R9	K2
      0x7C0C0C00,  //  0033  CALL	R3	6
      0x80040600,  //  0034  RET	1	R3
      0x80000000,  //  0035  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _initialize_buffers
********************************************************************/
be_local_closure(class_ScaleAnimation__initialize_buffers,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ScaleAnimation,     /* shared constants */
    be_str_weak(_initialize_buffers),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x88040109,  //  0000  GETMBR	R1	R0	K9
      0x8C04030A,  //  0001  GETMET	R1	R1	K10
      0x7C040200,  //  0002  CALL	R1	1
      0xB80A4A00,  //  0003  GETNGBL	R2	K37
      0x8C080526,  //  0004  GETMET	R2	R2	K38
      0x5C100200,  //  0005  MOVE	R4	R1
      0x7C080400,  //  0006  CALL	R2	2
      0x90022002,  //  0007  SETMBR	R0	K16	R2
      0x60080012,  //  0008  GETGBL	R2	G18
      0x7C080000,  //  0009  CALL	R2	0
      0x90021602,  //  000A  SETMBR	R0	K11	R2
      0x8808010B,  //  000B  GETMBR	R2	R0	K11
      0x8C080527,  //  000C  GETMET	R2	R2	K39
      0x5C100200,  //  000D  MOVE	R4	R1
      0x7C080400,  //  000E  CALL	R2	2
      0x58080002,  //  000F  LDCONST	R2	K2
      0x140C0401,  //  0010  LT	R3	R2	R1
      0x780E0003,  //  0011  JMPF	R3	#0016
      0x880C010B,  //  0012  GETMBR	R3	R0	K11
      0x980C0515,  //  0013  SETIDX	R3	R2	K21
      0x00080503,  //  0014  ADD	R2	R2	K3
      0x7001FFF9,  //  0015  JMP		#0010
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: render
********************************************************************/
be_local_closure(class_ScaleAnimation_render,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ScaleAnimation,     /* shared constants */
    be_str_weak(render),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x1C0C0203,  //  0001  EQ	R3	R1	R3
      0x780E0001,  //  0002  JMPF	R3	#0005
      0x500C0000,  //  0003  LDBOOL	R3	0	0
      0x80040600,  //  0004  RET	1	R3
      0x880C0109,  //  0005  GETMBR	R3	R0	K9
      0x8C0C070A,  //  0006  GETMET	R3	R3	K10
      0x7C0C0200,  //  0007  CALL	R3	1
      0x58100002,  //  0008  LDCONST	R4	K2
      0x14140803,  //  0009  LT	R5	R4	R3
      0x78160009,  //  000A  JMPF	R5	#0015
      0x88140328,  //  000B  GETMBR	R5	R1	K40
      0x14140805,  //  000C  LT	R5	R4	R5
      0x78160004,  //  000D  JMPF	R5	#0013
      0x8C140329,  //  000E  GETMET	R5	R1	K41
      0x5C1C0800,  //  000F  MOVE	R7	R4
      0x8820010B,  //  0010  GETMBR	R8	R0	K11
      0x94201004,  //  0011  GETIDX	R8	R8	R4
      0x7C140600,  //  0012  CALL	R5	3
      0x00100903,  //  0013  ADD	R4	R4	K3
      0x7001FFF3,  //  0014  JMP		#0009
      0x50140200,  //  0015  LDBOOL	R5	1	0
      0x80040A00,  //  0016  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _interpolate_colors
********************************************************************/
be_local_closure(class_ScaleAnimation__interpolate_colors,   /* name */
  be_nested_proto(
    18,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ScaleAnimation,     /* shared constants */
    be_str_weak(_interpolate_colors),
    &be_const_str_solidified,
    ( &(const binstruction[66]) {  /* code */
      0x18100702,  //  0000  LE	R4	R3	K2
      0x78120001,  //  0001  JMPF	R4	#0004
      0x80040200,  //  0002  RET	1	R1
      0x70020003,  //  0003  JMP		#0008
      0x541200FF,  //  0004  LDINT	R4	256
      0x28100604,  //  0005  GE	R4	R3	R4
      0x78120000,  //  0006  JMPF	R4	#0008
      0x80040400,  //  0007  RET	1	R2
      0x54120017,  //  0008  LDINT	R4	24
      0x3C100204,  //  0009  SHR	R4	R1	R4
      0x541600FE,  //  000A  LDINT	R5	255
      0x2C100805,  //  000B  AND	R4	R4	R5
      0x5416000F,  //  000C  LDINT	R5	16
      0x3C140205,  //  000D  SHR	R5	R1	R5
      0x541A00FE,  //  000E  LDINT	R6	255
      0x2C140A06,  //  000F  AND	R5	R5	R6
      0x541A0007,  //  0010  LDINT	R6	8
      0x3C180206,  //  0011  SHR	R6	R1	R6
      0x541E00FE,  //  0012  LDINT	R7	255
      0x2C180C07,  //  0013  AND	R6	R6	R7
      0x541E00FE,  //  0014  LDINT	R7	255
      0x2C1C0207,  //  0015  AND	R7	R1	R7
      0x54220017,  //  0016  LDINT	R8	24
      0x3C200408,  //  0017  SHR	R8	R2	R8
      0x542600FE,  //  0018  LDINT	R9	255
      0x2C201009,  //  0019  AND	R8	R8	R9
      0x5426000F,  //  001A  LDINT	R9	16
      0x3C240409,  //  001B  SHR	R9	R2	R9
      0x542A00FE,  //  001C  LDINT	R10	255
      0x2C24120A,  //  001D  AND	R9	R9	R10
      0x542A0007,  //  001E  LDINT	R10	8
      0x3C28040A,  //  001F  SHR	R10	R2	R10
      0x542E00FE,  //  0020  LDINT	R11	255
      0x2C28140B,  //  0021  AND	R10	R10	R11
      0x542E00FE,  //  0022  LDINT	R11	255
      0x2C2C040B,  //  0023  AND	R11	R2	R11
      0x04301004,  //  0024  SUB	R12	R8	R4
      0x08301803,  //  0025  MUL	R12	R12	R3
      0x543600FF,  //  0026  LDINT	R13	256
      0x0C30180D,  //  0027  DIV	R12	R12	R13
      0x0030080C,  //  0028  ADD	R12	R4	R12
      0x04341205,  //  0029  SUB	R13	R9	R5
      0x08341A03,  //  002A  MUL	R13	R13	R3
      0x543A00FF,  //  002B  LDINT	R14	256
      0x0C341A0E,  //  002C  DIV	R13	R13	R14
      0x00340A0D,  //  002D  ADD	R13	R5	R13
      0x04381406,  //  002E  SUB	R14	R10	R6
      0x08381C03,  //  002F  MUL	R14	R14	R3
      0x543E00FF,  //  0030  LDINT	R15	256
      0x0C381C0F,  //  0031  DIV	R14	R14	R15
      0x00380C0E,  //  0032  ADD	R14	R6	R14
      0x043C1607,  //  0033  SUB	R15	R11	R7
      0x083C1E03,  //  0034  MUL	R15	R15	R3
      0x544200FF,  //  0035  LDINT	R16	256
      0x0C3C1E10,  //  0036  DIV	R15	R15	R16
      0x003C0E0F,  //  0037  ADD	R15	R7	R15
      0x54420017,  //  0038  LDINT	R16	24
      0x38401810,  //  0039  SHL	R16	R12	R16
      0x5446000F,  //  003A  LDINT	R17	16
      0x38441A11,  //  003B  SHL	R17	R13	R17
      0x30402011,  //  003C  OR	R16	R16	R17
      0x54460007,  //  003D  LDINT	R17	8
      0x38441C11,  //  003E  SHL	R17	R14	R17
      0x30402011,  //  003F  OR	R16	R16	R17
      0x3040200F,  //  0040  OR	R16	R16	R15
      0x80042000,  //  0041  RET	1	R16
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(class_ScaleAnimation_start,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ScaleAnimation,     /* shared constants */
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C08051A,  //  0003  GETMET	R2	R2	K26
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x90020B02,  //  0006  SETMBR	R0	K5	K2
      0x4C080000,  //  0007  LDNIL	R2
      0x1C080202,  //  0008  EQ	R2	R1	R2
      0x780A0001,  //  0009  JMPF	R2	#000C
      0x88080109,  //  000A  GETMBR	R2	R0	K9
      0x88040524,  //  000B  GETMBR	R1	R2	K36
      0x90023001,  //  000C  SETMBR	R0	K24	R1
      0x80040000,  //  000D  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: ScaleAnimation
********************************************************************/
extern const bclass be_class_Animation;
be_local_class(ScaleAnimation,
    4,
    &be_class_Animation,
    be_nested_map(16,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(_calculate_scale, -1), be_const_closure(class_ScaleAnimation__calculate_scale_closure) },
        { be_const_key_weak(scale_phase, -1), be_const_var(0) },
        { be_const_key_weak(tostring, -1), be_const_closure(class_ScaleAnimation_tostring_closure) },
        { be_const_key_weak(PARAMS, 10), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(scale_center, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(128) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(scale_speed, 0), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(0) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(source_animation, 4), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_nil() },
        { be_const_key_weak(type, -1), be_nested_str_weak(instance) },
    }))    ) } )) },
        { be_const_key_weak(scale_factor, 2), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(128) },
        { be_const_key_weak(min, -1), be_const_int(1) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(scale_mode, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(0) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(3) },
    }))    ) } )) },
        { be_const_key_weak(interpolation, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(1) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(1) },
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(update, -1), be_const_closure(class_ScaleAnimation_update_closure) },
        { be_const_key_weak(_get_current_scale_factor, 2), be_const_closure(class_ScaleAnimation__get_current_scale_factor_closure) },
        { be_const_key_weak(source_frame, -1), be_const_var(1) },
        { be_const_key_weak(current_colors, -1), be_const_var(2) },
        { be_const_key_weak(render, -1), be_const_closure(class_ScaleAnimation_render_closure) },
        { be_const_key_weak(start_time, -1), be_const_var(3) },
        { be_const_key_weak(init, 0), be_const_closure(class_ScaleAnimation_init_closure) },
        { be_const_key_weak(_sine, -1), be_const_closure(class_ScaleAnimation__sine_closure) },
        { be_const_key_weak(_initialize_buffers, -1), be_const_closure(class_ScaleAnimation__initialize_buffers_closure) },
        { be_const_key_weak(on_param_changed, 8), be_const_closure(class_ScaleAnimation_on_param_changed_closure) },
        { be_const_key_weak(_interpolate_colors, -1), be_const_closure(class_ScaleAnimation__interpolate_colors_closure) },
        { be_const_key_weak(start, -1), be_const_closure(class_ScaleAnimation_start_closure) },
    })),
    be_str_weak(ScaleAnimation)
);

/********************************************************************
** Solidified function: animation_global
********************************************************************/
be_local_closure(animation_global,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(global),
    /* K1   */  be_nested_str_weak(introspect),
    /* K2   */  be_nested_str_weak(animation),
    /* K3   */  be_nested_str_weak(contains),
    /* K4   */  be_nested_str_weak(_X27_X25s_X27_X20undeclared),
    /* K5   */  be_nested_str_weak(syntax_error),
    }),
    be_str_weak(animation_global),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0xA4120400,  //  0002  IMPORT	R4	K2
      0x4C140000,  //  0003  LDNIL	R5
      0x20140205,  //  0004  NE	R5	R1	R5
      0x78160006,  //  0005  JMPF	R5	#000D
      0x8C140703,  //  0006  GETMET	R5	R3	K3
      0x5C1C0800,  //  0007  MOVE	R7	R4
      0x5C200200,  //  0008  MOVE	R8	R1
      0x7C140600,  //  0009  CALL	R5	3
      0x78160001,  //  000A  JMPF	R5	#000D
      0x88140801,  //  000B  GETMBR	R5	R4	R1
      0x80040A00,  //  000C  RET	1	R5
      0x8C140503,  //  000D  GETMET	R5	R2	K3
      0x5C1C0000,  //  000E  MOVE	R7	R0
      0x7C140400,  //  000F  CALL	R5	2
      0x78160002,  //  0010  JMPF	R5	#0014
      0x88140400,  //  0011  GETMBR	R5	R2	R0
      0x80040A00,  //  0012  RET	1	R5
      0x70020004,  //  0013  JMP		#0019
      0x60140018,  //  0014  GETGBL	R5	G24
      0x58180004,  //  0015  LDCONST	R6	K4
      0x5C1C0000,  //  0016  MOVE	R7	R0
      0x7C140400,  //  0017  CALL	R5	2
      0xB0060A05,  //  0018  RAISE	1	K5	R5
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: unregister_event_handler
********************************************************************/
be_local_closure(unregister_event_handler,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(event_manager),
    /* K2   */  be_nested_str_weak(unregister_handler),
    }),
    be_str_weak(unregister_event_handler),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x8C040302,  //  0002  GETMET	R1	R1	K2
      0x5C0C0000,  //  0003  MOVE	R3	R0
      0x7C040400,  //  0004  CALL	R1	2
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: bounce_constrained
********************************************************************/
be_local_closure(bounce_constrained,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(bounce_animation),
    /* K2   */  be_nested_str_weak(bounce_speed),
    /* K3   */  be_nested_str_weak(bounce_range),
    /* K4   */  be_nested_str_weak(damping),
    /* K5   */  be_nested_str_weak(gravity),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(name),
    /* K8   */  be_nested_str_weak(bounce_constrained),
    }),
    be_str_weak(bounce_constrained),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x540A0095,  //  0004  LDINT	R2	150
      0x90060402,  //  0005  SETMBR	R1	K2	R2
      0x540A000E,  //  0006  LDINT	R2	15
      0x90060602,  //  0007  SETMBR	R1	K3	R2
      0x540A00F9,  //  0008  LDINT	R2	250
      0x90060802,  //  0009  SETMBR	R1	K4	R2
      0x90060B06,  //  000A  SETMBR	R1	K5	K6
      0x90060F08,  //  000B  SETMBR	R1	K7	K8
      0x80040200,  //  000C  RET	1	R1
    })
  )
);
/*******************************************************************/


extern const bclass be_class_PaletteWaveAnimation;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_PaletteWaveAnimation_init,   /* name */
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
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(name),
    /* K2   */  be_nested_str_weak(palette_wave),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x90020302,  //  0006  SETMBR	R0	K1	K2
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _update_value_buffer
********************************************************************/
be_local_closure(class_PaletteWaveAnimation__update_value_buffer,   /* name */
  be_nested_proto(
    19,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(wave_period),
    /* K1   */  be_nested_str_weak(wave_length),
    /* K2   */  be_nested_str_weak(engine),
    /* K3   */  be_nested_str_weak(get_strip_length),
    /* K4   */  be_nested_str_weak(value_buffer),
    /* K5   */  be_nested_str_weak(resize),
    /* K6   */  be_nested_str_weak(tasmota),
    /* K7   */  be_nested_str_weak(scale_uint),
    /* K8   */  be_const_int(0),
    /* K9   */  be_const_real_hex(0x447A0000),
    /* K10  */  be_nested_str_weak(sine_int),
    /* K11  */  be_nested_str_weak(scale_int),
    /* K12  */  be_const_int(1),
    }),
    be_str_weak(_update_value_buffer),
    &be_const_str_solidified,
    ( &(const binstruction[56]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x880C0101,  //  0001  GETMBR	R3	R0	K1
      0x88100102,  //  0002  GETMBR	R4	R0	K2
      0x8C100903,  //  0003  GETMET	R4	R4	K3
      0x7C100200,  //  0004  CALL	R4	1
      0x6014000C,  //  0005  GETGBL	R5	G12
      0x88180104,  //  0006  GETMBR	R6	R0	K4
      0x7C140200,  //  0007  CALL	R5	1
      0x20140A04,  //  0008  NE	R5	R5	R4
      0x78160003,  //  0009  JMPF	R5	#000E
      0x88140104,  //  000A  GETMBR	R5	R0	K4
      0x8C140B05,  //  000B  GETMET	R5	R5	K5
      0x5C1C0800,  //  000C  MOVE	R7	R4
      0x7C140400,  //  000D  CALL	R5	2
      0xB8160C00,  //  000E  GETNGBL	R5	K6
      0x8C140B07,  //  000F  GETMET	R5	R5	K7
      0x101C0202,  //  0010  MOD	R7	R1	R2
      0x58200008,  //  0011  LDCONST	R8	K8
      0x5C240400,  //  0012  MOVE	R9	R2
      0x58280008,  //  0013  LDCONST	R10	K8
      0x542E03E7,  //  0014  LDINT	R11	1000
      0x7C140C00,  //  0015  CALL	R5	6
      0x0C140B09,  //  0016  DIV	R5	R5	K9
      0x60180009,  //  0017  GETGBL	R6	G9
      0x081C0A03,  //  0018  MUL	R7	R5	R3
      0x7C180200,  //  0019  CALL	R6	1
      0x581C0008,  //  001A  LDCONST	R7	K8
      0x14200E04,  //  001B  LT	R8	R7	R4
      0x78220019,  //  001C  JMPF	R8	#0037
      0x00200E06,  //  001D  ADD	R8	R7	R6
      0x10201003,  //  001E  MOD	R8	R8	R3
      0xB8260C00,  //  001F  GETNGBL	R9	K6
      0x8C241307,  //  0020  GETMET	R9	R9	K7
      0x5C2C1000,  //  0021  MOVE	R11	R8
      0x58300008,  //  0022  LDCONST	R12	K8
      0x5C340600,  //  0023  MOVE	R13	R3
      0x58380008,  //  0024  LDCONST	R14	K8
      0x543E7FFE,  //  0025  LDINT	R15	32767
      0x7C240C00,  //  0026  CALL	R9	6
      0xB82A0C00,  //  0027  GETNGBL	R10	K6
      0x8C28150A,  //  0028  GETMET	R10	R10	K10
      0x5C301200,  //  0029  MOVE	R12	R9
      0x7C280400,  //  002A  CALL	R10	2
      0x882C0104,  //  002B  GETMBR	R11	R0	K4
      0xB8320C00,  //  002C  GETNGBL	R12	K6
      0x8C30190B,  //  002D  GETMET	R12	R12	K11
      0x5C381400,  //  002E  MOVE	R14	R10
      0x543DEFFF,  //  002F  LDINT	R15	-4096
      0x54420FFF,  //  0030  LDINT	R16	4096
      0x58440008,  //  0031  LDCONST	R17	K8
      0x544A0063,  //  0032  LDINT	R18	100
      0x7C300C00,  //  0033  CALL	R12	6
      0x982C0E0C,  //  0034  SETIDX	R11	R7	R12
      0x001C0F0C,  //  0035  ADD	R7	R7	K12
      0x7001FFE3,  //  0036  JMP		#001B
      0x80000000,  //  0037  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: PaletteWaveAnimation
********************************************************************/
extern const bclass be_class_PalettePatternAnimation;
be_local_class(PaletteWaveAnimation,
    0,
    &be_class_PalettePatternAnimation,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(PARAMS, 1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(wave_period, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(5000) },
        { be_const_key_weak(min, -1), be_const_int(1) },
    }))    ) } )) },
        { be_const_key_weak(wave_length, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(10) },
        { be_const_key_weak(min, -1), be_const_int(1) },
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(init, -1), be_const_closure(class_PaletteWaveAnimation_init_closure) },
        { be_const_key_weak(_update_value_buffer, -1), be_const_closure(class_PaletteWaveAnimation__update_value_buffer_closure) },
    })),
    be_str_weak(PaletteWaveAnimation)
);

/********************************************************************
** Solidified function: bounce_gravity
********************************************************************/
be_local_closure(bounce_gravity,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(bounce_animation),
    /* K2   */  be_nested_str_weak(bounce_speed),
    /* K3   */  be_nested_str_weak(bounce_range),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(damping),
    /* K6   */  be_nested_str_weak(gravity),
    /* K7   */  be_nested_str_weak(name),
    /* K8   */  be_nested_str_weak(bounce_gravity),
    }),
    be_str_weak(bounce_gravity),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x540A0063,  //  0004  LDINT	R2	100
      0x90060402,  //  0005  SETMBR	R1	K2	R2
      0x90060704,  //  0006  SETMBR	R1	K3	K4
      0x540A00EF,  //  0007  LDINT	R2	240
      0x90060A02,  //  0008  SETMBR	R1	K5	R2
      0x540A007F,  //  0009  LDINT	R2	128
      0x90060C02,  //  000A  SETMBR	R1	K6	R2
      0x90060F08,  //  000B  SETMBR	R1	K7	K8
      0x80040200,  //  000C  RET	1	R1
    })
  )
);
/*******************************************************************/

// compact class 'StaticColorProvider' ktab size: 3, total: 5 (saved 16 bytes)
static const bvalue be_ktab_class_StaticColorProvider[3] = {
  /* K0   */  be_nested_str_weak(color),
  /* K1   */  be_nested_str_weak(StaticColorProvider_X28color_X3D0x_X2508X_X29),
  /* K2   */  be_nested_str_weak(StaticColorProvider_X28color_X3Dunset_X29),
};


extern const bclass be_class_StaticColorProvider;

/********************************************************************
** Solidified function: produce_value
********************************************************************/
be_local_closure(class_StaticColorProvider_produce_value,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_StaticColorProvider,     /* shared constants */
    be_str_weak(produce_value),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x80040600,  //  0001  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_StaticColorProvider_tostring,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_StaticColorProvider,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0xA8020007,  //  0000  EXBLK	0	#0009
      0x60040018,  //  0001  GETGBL	R1	G24
      0x58080001,  //  0002  LDCONST	R2	K1
      0x880C0100,  //  0003  GETMBR	R3	R0	K0
      0x7C040400,  //  0004  CALL	R1	2
      0xA8040001,  //  0005  EXBLK	1	1
      0x80040200,  //  0006  RET	1	R1
      0xA8040001,  //  0007  EXBLK	1	1
      0x70020004,  //  0008  JMP		#000E
      0xAC040000,  //  0009  CATCH	R1	0	0
      0x70020001,  //  000A  JMP		#000D
      0x80060400,  //  000B  RET	1	K2
      0x70020000,  //  000C  JMP		#000E
      0xB0080000,  //  000D  RAISE	2	R0	R0
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_color_for_value
********************************************************************/
be_local_closure(class_StaticColorProvider_get_color_for_value,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_StaticColorProvider,     /* shared constants */
    be_str_weak(get_color_for_value),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x80040600,  //  0001  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: StaticColorProvider
********************************************************************/
extern const bclass be_class_ColorProvider;
be_local_class(StaticColorProvider,
    0,
    &be_class_ColorProvider,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(tostring, -1), be_const_closure(class_StaticColorProvider_tostring_closure) },
        { be_const_key_weak(produce_value, 2), be_const_closure(class_StaticColorProvider_produce_value_closure) },
        { be_const_key_weak(get_color_for_value, 0), be_const_closure(class_StaticColorProvider_get_color_for_value_closure) },
        { be_const_key_weak(PARAMS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(color, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(-1) },
    }))    ) } )) },
    }))    ) } )) },
    })),
    be_str_weak(StaticColorProvider)
);

/********************************************************************
** Solidified function: get_registered_events
********************************************************************/
be_local_closure(get_registered_events,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    0,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(event_manager),
    /* K2   */  be_nested_str_weak(get_registered_events),
    }),
    be_str_weak(get_registered_events),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0xB8020000,  //  0000  GETNGBL	R0	K0
      0x88000101,  //  0001  GETMBR	R0	R0	K1
      0x8C000102,  //  0002  GETMET	R0	R0	K2
      0x7C000200,  //  0003  CALL	R0	1
      0x80040000,  //  0004  RET	1	R0
    })
  )
);
/*******************************************************************/

// compact class 'BounceAnimation' ktab size: 38, total: 85 (saved 376 bytes)
static const bvalue be_ktab_class_BounceAnimation[38] = {
  /* K0   */  be_nested_str_weak(bounce_speed),
  /* K1   */  be_nested_str_weak(tasmota),
  /* K2   */  be_nested_str_weak(scale_uint),
  /* K3   */  be_const_int(0),
  /* K4   */  be_nested_str_weak(current_velocity),
  /* K5   */  be_nested_str_weak(init),
  /* K6   */  be_nested_str_weak(current_position),
  /* K7   */  be_nested_str_weak(bounce_center),
  /* K8   */  be_nested_str_weak(source_frame),
  /* K9   */  be_nested_str_weak(current_colors),
  /* K10  */  be_nested_str_weak(last_update_time),
  /* K11  */  be_nested_str_weak(_initialize_buffers),
  /* K12  */  be_nested_str_weak(update),
  /* K13  */  be_nested_str_weak(_update_physics),
  /* K14  */  be_nested_str_weak(source_animation),
  /* K15  */  be_nested_str_weak(is_running),
  /* K16  */  be_nested_str_weak(start),
  /* K17  */  be_nested_str_weak(start_time),
  /* K18  */  be_nested_str_weak(_calculate_bounce),
  /* K19  */  be_nested_str_weak(BounceAnimation_X28speed_X3D_X25s_X2C_X20damping_X3D_X25s_X2C_X20gravity_X3D_X25s_X2C_X20priority_X3D_X25s_X2C_X20running_X3D_X25s_X29),
  /* K20  */  be_nested_str_weak(damping),
  /* K21  */  be_nested_str_weak(gravity),
  /* K22  */  be_nested_str_weak(priority),
  /* K23  */  be_nested_str_weak(bounce_range),
  /* K24  */  be_nested_str_weak(engine),
  /* K25  */  be_nested_str_weak(get_strip_length),
  /* K26  */  be_const_int(2),
  /* K27  */  be_nested_str_weak(animation),
  /* K28  */  be_nested_str_weak(frame_buffer),
  /* K29  */  be_nested_str_weak(resize),
  /* K30  */  be_const_int(-16777216),
  /* K31  */  be_const_int(1),
  /* K32  */  be_nested_str_weak(width),
  /* K33  */  be_nested_str_weak(set_pixel_color),
  /* K34  */  be_nested_str_weak(clear),
  /* K35  */  be_nested_str_weak(render),
  /* K36  */  be_nested_str_weak(get_pixel_color),
  /* K37  */  be_nested_str_weak(time_ms),
};


extern const bclass be_class_BounceAnimation;

/********************************************************************
** Solidified function: on_param_changed
********************************************************************/
be_local_closure(class_BounceAnimation_on_param_changed,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_BounceAnimation,     /* shared constants */
    be_str_weak(on_param_changed),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x1C0C0300,  //  0000  EQ	R3	R1	K0
      0x780E0010,  //  0001  JMPF	R3	#0013
      0xB80E0200,  //  0002  GETNGBL	R3	K1
      0x8C0C0702,  //  0003  GETMET	R3	R3	K2
      0x5C140400,  //  0004  MOVE	R5	R2
      0x58180003,  //  0005  LDCONST	R6	K3
      0x541E00FE,  //  0006  LDINT	R7	255
      0x58200003,  //  0007  LDCONST	R8	K3
      0x54260013,  //  0008  LDINT	R9	20
      0x7C0C0C00,  //  0009  CALL	R3	6
      0x541200FF,  //  000A  LDINT	R4	256
      0x08100604,  //  000B  MUL	R4	R3	R4
      0x88140104,  //  000C  GETMBR	R5	R0	K4
      0x14140B03,  //  000D  LT	R5	R5	K3
      0x78160002,  //  000E  JMPF	R5	#0012
      0x44140800,  //  000F  NEG	R5	R4
      0x90020805,  //  0010  SETMBR	R0	K4	R5
      0x70020000,  //  0011  JMP		#0013
      0x90020804,  //  0012  SETMBR	R0	K4	R4
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_BounceAnimation_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_BounceAnimation,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080505,  //  0003  GETMET	R2	R2	K5
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x90020D03,  //  0006  SETMBR	R0	K6	K3
      0x90020903,  //  0007  SETMBR	R0	K4	K3
      0x90020F03,  //  0008  SETMBR	R0	K7	K3
      0x4C080000,  //  0009  LDNIL	R2
      0x90021002,  //  000A  SETMBR	R0	K8	R2
      0x60080012,  //  000B  GETGBL	R2	G18
      0x7C080000,  //  000C  CALL	R2	0
      0x90021202,  //  000D  SETMBR	R0	K9	R2
      0x90021503,  //  000E  SETMBR	R0	K10	K3
      0x8C08010B,  //  000F  GETMET	R2	R0	K11
      0x7C080200,  //  0010  CALL	R2	1
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update
********************************************************************/
be_local_closure(class_BounceAnimation_update,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_BounceAnimation,     /* shared constants */
    be_str_weak(update),
    &be_const_str_solidified,
    ( &(const binstruction[39]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C08050C,  //  0003  GETMET	R2	R2	K12
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x740A0001,  //  0006  JMPT	R2	#0009
      0x50080000,  //  0007  LDBOOL	R2	0	0
      0x80040400,  //  0008  RET	1	R2
      0x8808010A,  //  0009  GETMBR	R2	R0	K10
      0x1C080503,  //  000A  EQ	R2	R2	K3
      0x780A0000,  //  000B  JMPF	R2	#000D
      0x90021401,  //  000C  SETMBR	R0	K10	R1
      0x8808010A,  //  000D  GETMBR	R2	R0	K10
      0x04080202,  //  000E  SUB	R2	R1	R2
      0x180C0503,  //  000F  LE	R3	R2	K3
      0x780E0001,  //  0010  JMPF	R3	#0013
      0x500C0200,  //  0011  LDBOOL	R3	1	0
      0x80040600,  //  0012  RET	1	R3
      0x90021401,  //  0013  SETMBR	R0	K10	R1
      0x8C0C010D,  //  0014  GETMET	R3	R0	K13
      0x5C140400,  //  0015  MOVE	R5	R2
      0x7C0C0400,  //  0016  CALL	R3	2
      0x880C010E,  //  0017  GETMBR	R3	R0	K14
      0x4C100000,  //  0018  LDNIL	R4
      0x20100604,  //  0019  NE	R4	R3	R4
      0x78120007,  //  001A  JMPF	R4	#0023
      0x8810070F,  //  001B  GETMBR	R4	R3	K15
      0x74120002,  //  001C  JMPT	R4	#0020
      0x8C100710,  //  001D  GETMET	R4	R3	K16
      0x88180111,  //  001E  GETMBR	R6	R0	K17
      0x7C100400,  //  001F  CALL	R4	2
      0x8C10070C,  //  0020  GETMET	R4	R3	K12
      0x5C180200,  //  0021  MOVE	R6	R1
      0x7C100400,  //  0022  CALL	R4	2
      0x8C100112,  //  0023  GETMET	R4	R0	K18
      0x7C100200,  //  0024  CALL	R4	1
      0x50100200,  //  0025  LDBOOL	R4	1	0
      0x80040800,  //  0026  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_BounceAnimation_tostring,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_BounceAnimation,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x60040018,  //  0000  GETGBL	R1	G24
      0x58080013,  //  0001  LDCONST	R2	K19
      0x880C0100,  //  0002  GETMBR	R3	R0	K0
      0x88100114,  //  0003  GETMBR	R4	R0	K20
      0x88140115,  //  0004  GETMBR	R5	R0	K21
      0x88180116,  //  0005  GETMBR	R6	R0	K22
      0x881C010F,  //  0006  GETMBR	R7	R0	K15
      0x7C040C00,  //  0007  CALL	R1	6
      0x80040200,  //  0008  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _update_physics
********************************************************************/
be_local_closure(class_BounceAnimation__update_physics,   /* name */
  be_nested_proto(
    19,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_BounceAnimation,     /* shared constants */
    be_str_weak(_update_physics),
    &be_const_str_solidified,
    ( &(const binstruction[95]) {  /* code */
      0x88080115,  //  0000  GETMBR	R2	R0	K21
      0x880C0117,  //  0001  GETMBR	R3	R0	K23
      0x88100118,  //  0002  GETMBR	R4	R0	K24
      0x8C100919,  //  0003  GETMET	R4	R4	K25
      0x7C100200,  //  0004  CALL	R4	1
      0x88140114,  //  0005  GETMBR	R5	R0	K20
      0x24180503,  //  0006  GT	R6	R2	K3
      0x781A000D,  //  0007  JMPF	R6	#0016
      0xB81A0200,  //  0008  GETNGBL	R6	K1
      0x8C180D02,  //  0009  GETMET	R6	R6	K2
      0x5C200400,  //  000A  MOVE	R8	R2
      0x58240003,  //  000B  LDCONST	R9	K3
      0x542A00FE,  //  000C  LDINT	R10	255
      0x582C0003,  //  000D  LDCONST	R11	K3
      0x543203E7,  //  000E  LDINT	R12	1000
      0x7C180C00,  //  000F  CALL	R6	6
      0x081C0C01,  //  0010  MUL	R7	R6	R1
      0x542203E7,  //  0011  LDINT	R8	1000
      0x0C1C0E08,  //  0012  DIV	R7	R7	R8
      0x88200104,  //  0013  GETMBR	R8	R0	K4
      0x00201007,  //  0014  ADD	R8	R8	R7
      0x90020808,  //  0015  SETMBR	R0	K4	R8
      0x881C0104,  //  0016  GETMBR	R7	R0	K4
      0x081C0E01,  //  0017  MUL	R7	R7	R1
      0x542203E7,  //  0018  LDINT	R8	1000
      0x0C1C0E08,  //  0019  DIV	R7	R7	R8
      0x88180106,  //  001A  GETMBR	R6	R0	K6
      0x00180C07,  //  001B  ADD	R6	R6	R7
      0x90020C06,  //  001C  SETMBR	R0	K6	R6
      0x24180703,  //  001D  GT	R6	R3	K3
      0x781A0001,  //  001E  JMPF	R6	#0021
      0x5C180600,  //  001F  MOVE	R6	R3
      0x70020000,  //  0020  JMP		#0022
      0x5C180800,  //  0021  MOVE	R6	R4
      0x541E00FF,  //  0022  LDINT	R7	256
      0x081C0C07,  //  0023  MUL	R7	R6	R7
      0x0C1C0F1A,  //  0024  DIV	R7	R7	K26
      0x88200107,  //  0025  GETMBR	R8	R0	K7
      0x04201007,  //  0026  SUB	R8	R8	R7
      0x88240107,  //  0027  GETMBR	R9	R0	K7
      0x00241207,  //  0028  ADD	R9	R9	R7
      0x50280000,  //  0029  LDBOOL	R10	0	0
      0x882C0106,  //  002A  GETMBR	R11	R0	K6
      0x182C1608,  //  002B  LE	R11	R11	R8
      0x782E0005,  //  002C  JMPF	R11	#0033
      0x90020C08,  //  002D  SETMBR	R0	K6	R8
      0x882C0104,  //  002E  GETMBR	R11	R0	K4
      0x442C1600,  //  002F  NEG	R11	R11
      0x9002080B,  //  0030  SETMBR	R0	K4	R11
      0x50280200,  //  0031  LDBOOL	R10	1	0
      0x70020007,  //  0032  JMP		#003B
      0x882C0106,  //  0033  GETMBR	R11	R0	K6
      0x282C1609,  //  0034  GE	R11	R11	R9
      0x782E0004,  //  0035  JMPF	R11	#003B
      0x90020C09,  //  0036  SETMBR	R0	K6	R9
      0x882C0104,  //  0037  GETMBR	R11	R0	K4
      0x442C1600,  //  0038  NEG	R11	R11
      0x9002080B,  //  0039  SETMBR	R0	K4	R11
      0x50280200,  //  003A  LDBOOL	R10	1	0
      0x782A0021,  //  003B  JMPF	R10	#005E
      0x542E00FE,  //  003C  LDINT	R11	255
      0x142C0A0B,  //  003D  LT	R11	R5	R11
      0x782E001E,  //  003E  JMPF	R11	#005E
      0xB82E0200,  //  003F  GETNGBL	R11	K1
      0x8C2C1702,  //  0040  GETMET	R11	R11	K2
      0x5C340A00,  //  0041  MOVE	R13	R5
      0x58380003,  //  0042  LDCONST	R14	K3
      0x543E00FE,  //  0043  LDINT	R15	255
      0x58400003,  //  0044  LDCONST	R16	K3
      0x544600FE,  //  0045  LDINT	R17	255
      0x7C2C0C00,  //  0046  CALL	R11	6
      0xB8320200,  //  0047  GETNGBL	R12	K1
      0x8C301902,  //  0048  GETMET	R12	R12	K2
      0x88380104,  //  0049  GETMBR	R14	R0	K4
      0x583C0003,  //  004A  LDCONST	R15	K3
      0x544200FE,  //  004B  LDINT	R16	255
      0x58440003,  //  004C  LDCONST	R17	K3
      0x5C481600,  //  004D  MOVE	R18	R11
      0x7C300C00,  //  004E  CALL	R12	6
      0x9002080C,  //  004F  SETMBR	R0	K4	R12
      0x88300104,  //  0050  GETMBR	R12	R0	K4
      0x14301903,  //  0051  LT	R12	R12	K3
      0x7832000A,  //  0052  JMPF	R12	#005E
      0xB8320200,  //  0053  GETNGBL	R12	K1
      0x8C301902,  //  0054  GETMET	R12	R12	K2
      0x88380104,  //  0055  GETMBR	R14	R0	K4
      0x44381C00,  //  0056  NEG	R14	R14
      0x583C0003,  //  0057  LDCONST	R15	K3
      0x544200FE,  //  0058  LDINT	R16	255
      0x58440003,  //  0059  LDCONST	R17	K3
      0x5C481600,  //  005A  MOVE	R18	R11
      0x7C300C00,  //  005B  CALL	R12	6
      0x44301800,  //  005C  NEG	R12	R12
      0x9002080C,  //  005D  SETMBR	R0	K4	R12
      0x80000000,  //  005E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _initialize_buffers
********************************************************************/
be_local_closure(class_BounceAnimation__initialize_buffers,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_BounceAnimation,     /* shared constants */
    be_str_weak(_initialize_buffers),
    &be_const_str_solidified,
    ( &(const binstruction[37]) {  /* code */
      0x88040118,  //  0000  GETMBR	R1	R0	K24
      0x8C040319,  //  0001  GETMET	R1	R1	K25
      0x7C040200,  //  0002  CALL	R1	1
      0x540A00FF,  //  0003  LDINT	R2	256
      0x08080202,  //  0004  MUL	R2	R1	R2
      0x0C08051A,  //  0005  DIV	R2	R2	K26
      0x90020E02,  //  0006  SETMBR	R0	K7	R2
      0x88080107,  //  0007  GETMBR	R2	R0	K7
      0x90020C02,  //  0008  SETMBR	R0	K6	R2
      0xB80A0200,  //  0009  GETNGBL	R2	K1
      0x8C080502,  //  000A  GETMET	R2	R2	K2
      0x88100100,  //  000B  GETMBR	R4	R0	K0
      0x58140003,  //  000C  LDCONST	R5	K3
      0x541A00FE,  //  000D  LDINT	R6	255
      0x581C0003,  //  000E  LDCONST	R7	K3
      0x54220013,  //  000F  LDINT	R8	20
      0x7C080C00,  //  0010  CALL	R2	6
      0x540E00FF,  //  0011  LDINT	R3	256
      0x080C0403,  //  0012  MUL	R3	R2	R3
      0x90020803,  //  0013  SETMBR	R0	K4	R3
      0xB80E3600,  //  0014  GETNGBL	R3	K27
      0x8C0C071C,  //  0015  GETMET	R3	R3	K28
      0x5C140200,  //  0016  MOVE	R5	R1
      0x7C0C0400,  //  0017  CALL	R3	2
      0x90021003,  //  0018  SETMBR	R0	K8	R3
      0x880C0109,  //  0019  GETMBR	R3	R0	K9
      0x8C0C071D,  //  001A  GETMET	R3	R3	K29
      0x5C140200,  //  001B  MOVE	R5	R1
      0x7C0C0400,  //  001C  CALL	R3	2
      0x580C0003,  //  001D  LDCONST	R3	K3
      0x14100601,  //  001E  LT	R4	R3	R1
      0x78120003,  //  001F  JMPF	R4	#0024
      0x88100109,  //  0020  GETMBR	R4	R0	K9
      0x9810071E,  //  0021  SETIDX	R4	R3	K30
      0x000C071F,  //  0022  ADD	R3	R3	K31
      0x7001FFF9,  //  0023  JMP		#001E
      0x80000000,  //  0024  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: render
********************************************************************/
be_local_closure(class_BounceAnimation_render,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_BounceAnimation,     /* shared constants */
    be_str_weak(render),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x880C010F,  //  0000  GETMBR	R3	R0	K15
      0x780E0002,  //  0001  JMPF	R3	#0005
      0x4C0C0000,  //  0002  LDNIL	R3
      0x1C0C0203,  //  0003  EQ	R3	R1	R3
      0x780E0001,  //  0004  JMPF	R3	#0007
      0x500C0000,  //  0005  LDBOOL	R3	0	0
      0x80040600,  //  0006  RET	1	R3
      0x880C0118,  //  0007  GETMBR	R3	R0	K24
      0x8C0C0719,  //  0008  GETMET	R3	R3	K25
      0x7C0C0200,  //  0009  CALL	R3	1
      0x58100003,  //  000A  LDCONST	R4	K3
      0x14140803,  //  000B  LT	R5	R4	R3
      0x78160009,  //  000C  JMPF	R5	#0017
      0x88140320,  //  000D  GETMBR	R5	R1	K32
      0x14140805,  //  000E  LT	R5	R4	R5
      0x78160004,  //  000F  JMPF	R5	#0015
      0x8C140321,  //  0010  GETMET	R5	R1	K33
      0x5C1C0800,  //  0011  MOVE	R7	R4
      0x88200109,  //  0012  GETMBR	R8	R0	K9
      0x94201004,  //  0013  GETIDX	R8	R8	R4
      0x7C140600,  //  0014  CALL	R5	3
      0x0010091F,  //  0015  ADD	R4	R4	K31
      0x7001FFF3,  //  0016  JMP		#000B
      0x50140200,  //  0017  LDBOOL	R5	1	0
      0x80040A00,  //  0018  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _calculate_bounce
********************************************************************/
be_local_closure(class_BounceAnimation__calculate_bounce,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_BounceAnimation,     /* shared constants */
    be_str_weak(_calculate_bounce),
    &be_const_str_solidified,
    ( &(const binstruction[39]) {  /* code */
      0x88040108,  //  0000  GETMBR	R1	R0	K8
      0x8C040322,  //  0001  GETMET	R1	R1	K34
      0x7C040200,  //  0002  CALL	R1	1
      0x8804010E,  //  0003  GETMBR	R1	R0	K14
      0x4C080000,  //  0004  LDNIL	R2
      0x20080202,  //  0005  NE	R2	R1	R2
      0x780A0003,  //  0006  JMPF	R2	#000B
      0x8C080323,  //  0007  GETMET	R2	R1	K35
      0x88100108,  //  0008  GETMBR	R4	R0	K8
      0x58140003,  //  0009  LDCONST	R5	K3
      0x7C080600,  //  000A  CALL	R2	3
      0x88080118,  //  000B  GETMBR	R2	R0	K24
      0x8C080519,  //  000C  GETMET	R2	R2	K25
      0x7C080200,  //  000D  CALL	R2	1
      0x880C0106,  //  000E  GETMBR	R3	R0	K6
      0x541200FF,  //  000F  LDINT	R4	256
      0x0C0C0604,  //  0010  DIV	R3	R3	R4
      0x0C10051A,  //  0011  DIV	R4	R2	K26
      0x04100604,  //  0012  SUB	R4	R3	R4
      0x58140003,  //  0013  LDCONST	R5	K3
      0x14180A02,  //  0014  LT	R6	R5	R2
      0x781A000F,  //  0015  JMPF	R6	#0026
      0x04180A04,  //  0016  SUB	R6	R5	R4
      0x281C0D03,  //  0017  GE	R7	R6	K3
      0x781E0008,  //  0018  JMPF	R7	#0022
      0x141C0C02,  //  0019  LT	R7	R6	R2
      0x781E0006,  //  001A  JMPF	R7	#0022
      0x881C0109,  //  001B  GETMBR	R7	R0	K9
      0x88200108,  //  001C  GETMBR	R8	R0	K8
      0x8C201124,  //  001D  GETMET	R8	R8	K36
      0x5C280C00,  //  001E  MOVE	R10	R6
      0x7C200400,  //  001F  CALL	R8	2
      0x981C0A08,  //  0020  SETIDX	R7	R5	R8
      0x70020001,  //  0021  JMP		#0024
      0x881C0109,  //  0022  GETMBR	R7	R0	K9
      0x981C0B1E,  //  0023  SETIDX	R7	R5	K30
      0x00140B1F,  //  0024  ADD	R5	R5	K31
      0x7001FFED,  //  0025  JMP		#0014
      0x80000000,  //  0026  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(class_BounceAnimation_start,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_BounceAnimation,     /* shared constants */
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080510,  //  0003  GETMET	R2	R2	K16
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x4C080000,  //  0006  LDNIL	R2
      0x20080202,  //  0007  NE	R2	R1	R2
      0x780A0001,  //  0008  JMPF	R2	#000B
      0x5C080200,  //  0009  MOVE	R2	R1
      0x70020001,  //  000A  JMP		#000D
      0x88080118,  //  000B  GETMBR	R2	R0	K24
      0x88080525,  //  000C  GETMBR	R2	R2	K37
      0x90021402,  //  000D  SETMBR	R0	K10	R2
      0x8C0C010B,  //  000E  GETMET	R3	R0	K11
      0x7C0C0200,  //  000F  CALL	R3	1
      0x880C010E,  //  0010  GETMBR	R3	R0	K14
      0x4C100000,  //  0011  LDNIL	R4
      0x20100604,  //  0012  NE	R4	R3	R4
      0x78120002,  //  0013  JMPF	R4	#0017
      0x8C100710,  //  0014  GETMET	R4	R3	K16
      0x5C180400,  //  0015  MOVE	R6	R2
      0x7C100400,  //  0016  CALL	R4	2
      0x80040000,  //  0017  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: BounceAnimation
********************************************************************/
extern const bclass be_class_Animation;
be_local_class(BounceAnimation,
    6,
    &be_class_Animation,
    be_nested_map(16,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(last_update_time, -1), be_const_var(5) },
        { be_const_key_weak(_calculate_bounce, 14), be_const_closure(class_BounceAnimation__calculate_bounce_closure) },
        { be_const_key_weak(render, 0), be_const_closure(class_BounceAnimation_render_closure) },
        { be_const_key_weak(init, 1), be_const_closure(class_BounceAnimation_init_closure) },
        { be_const_key_weak(update, -1), be_const_closure(class_BounceAnimation_update_closure) },
        { be_const_key_weak(tostring, -1), be_const_closure(class_BounceAnimation_tostring_closure) },
        { be_const_key_weak(source_frame, -1), be_const_var(3) },
        { be_const_key_weak(current_colors, -1), be_const_var(4) },
        { be_const_key_weak(current_velocity, -1), be_const_var(1) },
        { be_const_key_weak(_update_physics, -1), be_const_closure(class_BounceAnimation__update_physics_closure) },
        { be_const_key_weak(current_position, -1), be_const_var(0) },
        { be_const_key_weak(bounce_center, -1), be_const_var(2) },
        { be_const_key_weak(_initialize_buffers, 10), be_const_closure(class_BounceAnimation__initialize_buffers_closure) },
        { be_const_key_weak(on_param_changed, 2), be_const_closure(class_BounceAnimation_on_param_changed_closure) },
        { be_const_key_weak(PARAMS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(damping, 4), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(250) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(bounce_range, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(0) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(1000) },
    }))    ) } )) },
        { be_const_key_weak(bounce_speed, 0), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(128) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(gravity, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(0) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(source_animation, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_nil() },
        { be_const_key_weak(type, -1), be_nested_str_weak(instance) },
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(start, -1), be_const_closure(class_BounceAnimation_start_closure) },
    })),
    be_str_weak(BounceAnimation)
);

/********************************************************************
** Solidified function: gradient_rainbow_radial
********************************************************************/
be_local_closure(gradient_rainbow_radial,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(gradient_animation),
    /* K2   */  be_nested_str_weak(color),
    /* K3   */  be_nested_str_weak(gradient_type),
    /* K4   */  be_const_int(1),
    /* K5   */  be_nested_str_weak(center_pos),
    /* K6   */  be_nested_str_weak(movement_speed),
    }),
    be_str_weak(gradient_rainbow_radial),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x4C080000,  //  0004  LDNIL	R2
      0x90060402,  //  0005  SETMBR	R1	K2	R2
      0x90060704,  //  0006  SETMBR	R1	K3	K4
      0x540A007F,  //  0007  LDINT	R2	128
      0x90060A02,  //  0008  SETMBR	R1	K5	R2
      0x540A001D,  //  0009  LDINT	R2	30
      0x90060C02,  //  000A  SETMBR	R1	K6	R2
      0x80040200,  //  000B  RET	1	R1
    })
  )
);
/*******************************************************************/

// compact class 'OscillatorValueProvider' ktab size: 30, total: 39 (saved 72 bytes)
static const bvalue be_ktab_class_OscillatorValueProvider[30] = {
  /* K0   */  be_nested_str_weak(duration),
  /* K1   */  be_nested_str_weak(min_value),
  /* K2   */  be_nested_str_weak(max_value),
  /* K3   */  be_nested_str_weak(form),
  /* K4   */  be_nested_str_weak(phase),
  /* K5   */  be_nested_str_weak(duty_cycle),
  /* K6   */  be_const_int(0),
  /* K7   */  be_nested_str_weak(origin),
  /* K8   */  be_nested_str_weak(tasmota),
  /* K9   */  be_nested_str_weak(scale_uint),
  /* K10  */  be_nested_str_weak(animation),
  /* K11  */  be_nested_str_weak(SAWTOOTH),
  /* K12  */  be_nested_str_weak(value),
  /* K13  */  be_nested_str_weak(scale_int),
  /* K14  */  be_const_int(1),
  /* K15  */  be_nested_str_weak(TRIANGLE),
  /* K16  */  be_nested_str_weak(SQUARE),
  /* K17  */  be_nested_str_weak(COSINE),
  /* K18  */  be_nested_str_weak(sine_int),
  /* K19  */  be_nested_str_weak(SINE),
  /* K20  */  be_nested_str_weak(EASE_IN),
  /* K21  */  be_nested_str_weak(EASE_OUT),
  /* K22  */  be_nested_str_weak(ELASTIC),
  /* K23  */  be_nested_str_weak(BOUNCE),
  /* K24  */  be_nested_str_weak(form_names),
  /* K25  */  be_nested_str_weak(UNKNOWN),
  /* K26  */  be_nested_str_weak(OscillatorValueProvider_X28min_value_X3D_X25s_X2C_X20max_value_X3D_X25s_X2C_X20duration_X3D_X25sms_X2C_X20form_X3D_X25s_X29),
  /* K27  */  be_nested_str_weak(init),
  /* K28  */  be_nested_str_weak(engine),
  /* K29  */  be_nested_str_weak(time_ms),
};


extern const bclass be_class_OscillatorValueProvider;

/********************************************************************
** Solidified function: produce_value
********************************************************************/
be_local_closure(class_OscillatorValueProvider_produce_value,   /* name */
  be_nested_proto(
    24,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_OscillatorValueProvider,     /* shared constants */
    be_str_weak(produce_value),
    &be_const_str_solidified,
    ( &(const binstruction[382]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x88100101,  //  0001  GETMBR	R4	R0	K1
      0x88140102,  //  0002  GETMBR	R5	R0	K2
      0x88180103,  //  0003  GETMBR	R6	R0	K3
      0x881C0104,  //  0004  GETMBR	R7	R0	K4
      0x88200105,  //  0005  GETMBR	R8	R0	K5
      0x4C240000,  //  0006  LDNIL	R9
      0x1C240609,  //  0007  EQ	R9	R3	R9
      0x74260001,  //  0008  JMPT	R9	#000B
      0x18240706,  //  0009  LE	R9	R3	K6
      0x78260000,  //  000A  JMPF	R9	#000C
      0x80040800,  //  000B  RET	1	R4
      0x88240107,  //  000C  GETMBR	R9	R0	K7
      0x04240409,  //  000D  SUB	R9	R2	R9
      0x14281306,  //  000E  LT	R10	R9	K6
      0x782A0000,  //  000F  JMPF	R10	#0011
      0x58240006,  //  0010  LDCONST	R9	K6
      0xB82A1000,  //  0011  GETNGBL	R10	K8
      0x8C281509,  //  0012  GETMET	R10	R10	K9
      0x5C301000,  //  0013  MOVE	R12	R8
      0x58340006,  //  0014  LDCONST	R13	K6
      0x543A0063,  //  0015  LDINT	R14	100
      0x583C0006,  //  0016  LDCONST	R15	K6
      0x5C400600,  //  0017  MOVE	R16	R3
      0x7C280C00,  //  0018  CALL	R10	6
      0x282C1203,  //  0019  GE	R11	R9	R3
      0x782E0005,  //  001A  JMPF	R11	#0021
      0x0C2C1203,  //  001B  DIV	R11	R9	R3
      0x08341603,  //  001C  MUL	R13	R11	R3
      0x88300107,  //  001D  GETMBR	R12	R0	K7
      0x0030180D,  //  001E  ADD	R12	R12	R13
      0x90020E0C,  //  001F  SETMBR	R0	K7	R12
      0x10241203,  //  0020  MOD	R9	R9	R3
      0x5C2C1200,  //  0021  MOVE	R11	R9
      0x24300F06,  //  0022  GT	R12	R7	K6
      0x7832000B,  //  0023  JMPF	R12	#0030
      0xB8321000,  //  0024  GETNGBL	R12	K8
      0x8C301909,  //  0025  GETMET	R12	R12	K9
      0x5C380E00,  //  0026  MOVE	R14	R7
      0x583C0006,  //  0027  LDCONST	R15	K6
      0x54420063,  //  0028  LDINT	R16	100
      0x58440006,  //  0029  LDCONST	R17	K6
      0x5C480600,  //  002A  MOVE	R18	R3
      0x7C300C00,  //  002B  CALL	R12	6
      0x002C160C,  //  002C  ADD	R11	R11	R12
      0x28301603,  //  002D  GE	R12	R11	R3
      0x78320000,  //  002E  JMPF	R12	#0030
      0x042C1603,  //  002F  SUB	R11	R11	R3
      0xB8321400,  //  0030  GETNGBL	R12	K10
      0x8830190B,  //  0031  GETMBR	R12	R12	K11
      0x1C300C0C,  //  0032  EQ	R12	R6	R12
      0x78320009,  //  0033  JMPF	R12	#003E
      0xB8321000,  //  0034  GETNGBL	R12	K8
      0x8C30190D,  //  0035  GETMET	R12	R12	K13
      0x5C381600,  //  0036  MOVE	R14	R11
      0x583C0006,  //  0037  LDCONST	R15	K6
      0x0440070E,  //  0038  SUB	R16	R3	K14
      0x5C440800,  //  0039  MOVE	R17	R4
      0x5C480A00,  //  003A  MOVE	R18	R5
      0x7C300C00,  //  003B  CALL	R12	6
      0x9002180C,  //  003C  SETMBR	R0	K12	R12
      0x7002013D,  //  003D  JMP		#017C
      0xB8321400,  //  003E  GETNGBL	R12	K10
      0x8830190F,  //  003F  GETMBR	R12	R12	K15
      0x1C300C0C,  //  0040  EQ	R12	R6	R12
      0x78320015,  //  0041  JMPF	R12	#0058
      0x1430160A,  //  0042  LT	R12	R11	R10
      0x78320009,  //  0043  JMPF	R12	#004E
      0xB8321000,  //  0044  GETNGBL	R12	K8
      0x8C30190D,  //  0045  GETMET	R12	R12	K13
      0x5C381600,  //  0046  MOVE	R14	R11
      0x583C0006,  //  0047  LDCONST	R15	K6
      0x0440150E,  //  0048  SUB	R16	R10	K14
      0x5C440800,  //  0049  MOVE	R17	R4
      0x5C480A00,  //  004A  MOVE	R18	R5
      0x7C300C00,  //  004B  CALL	R12	6
      0x9002180C,  //  004C  SETMBR	R0	K12	R12
      0x70020008,  //  004D  JMP		#0057
      0xB8321000,  //  004E  GETNGBL	R12	K8
      0x8C30190D,  //  004F  GETMET	R12	R12	K13
      0x5C381600,  //  0050  MOVE	R14	R11
      0x5C3C1400,  //  0051  MOVE	R15	R10
      0x0440070E,  //  0052  SUB	R16	R3	K14
      0x5C440A00,  //  0053  MOVE	R17	R5
      0x5C480800,  //  0054  MOVE	R18	R4
      0x7C300C00,  //  0055  CALL	R12	6
      0x9002180C,  //  0056  SETMBR	R0	K12	R12
      0x70020123,  //  0057  JMP		#017C
      0xB8321400,  //  0058  GETNGBL	R12	K10
      0x88301910,  //  0059  GETMBR	R12	R12	K16
      0x1C300C0C,  //  005A  EQ	R12	R6	R12
      0x78320005,  //  005B  JMPF	R12	#0062
      0x1430160A,  //  005C  LT	R12	R11	R10
      0x78320001,  //  005D  JMPF	R12	#0060
      0x90021804,  //  005E  SETMBR	R0	K12	R4
      0x70020000,  //  005F  JMP		#0061
      0x90021805,  //  0060  SETMBR	R0	K12	R5
      0x70020119,  //  0061  JMP		#017C
      0xB8321400,  //  0062  GETNGBL	R12	K10
      0x88301911,  //  0063  GETMBR	R12	R12	K17
      0x1C300C0C,  //  0064  EQ	R12	R6	R12
      0x78320016,  //  0065  JMPF	R12	#007D
      0xB8321000,  //  0066  GETNGBL	R12	K8
      0x8C301909,  //  0067  GETMET	R12	R12	K9
      0x5C381600,  //  0068  MOVE	R14	R11
      0x583C0006,  //  0069  LDCONST	R15	K6
      0x0440070E,  //  006A  SUB	R16	R3	K14
      0x58440006,  //  006B  LDCONST	R17	K6
      0x544A7FFE,  //  006C  LDINT	R18	32767
      0x7C300C00,  //  006D  CALL	R12	6
      0xB8361000,  //  006E  GETNGBL	R13	K8
      0x8C341B12,  //  006F  GETMET	R13	R13	K18
      0x543E1FFF,  //  0070  LDINT	R15	8192
      0x043C180F,  //  0071  SUB	R15	R12	R15
      0x7C340400,  //  0072  CALL	R13	2
      0xB83A1000,  //  0073  GETNGBL	R14	K8
      0x8C381D0D,  //  0074  GETMET	R14	R14	K13
      0x5C401A00,  //  0075  MOVE	R16	R13
      0x5445EFFF,  //  0076  LDINT	R17	-4096
      0x544A0FFF,  //  0077  LDINT	R18	4096
      0x5C4C0800,  //  0078  MOVE	R19	R4
      0x5C500A00,  //  0079  MOVE	R20	R5
      0x7C380C00,  //  007A  CALL	R14	6
      0x9002180E,  //  007B  SETMBR	R0	K12	R14
      0x700200FE,  //  007C  JMP		#017C
      0xB8321400,  //  007D  GETNGBL	R12	K10
      0x88301913,  //  007E  GETMBR	R12	R12	K19
      0x1C300C0C,  //  007F  EQ	R12	R6	R12
      0x78320015,  //  0080  JMPF	R12	#0097
      0xB8321000,  //  0081  GETNGBL	R12	K8
      0x8C301909,  //  0082  GETMET	R12	R12	K9
      0x5C381600,  //  0083  MOVE	R14	R11
      0x583C0006,  //  0084  LDCONST	R15	K6
      0x0440070E,  //  0085  SUB	R16	R3	K14
      0x58440006,  //  0086  LDCONST	R17	K6
      0x544A7FFE,  //  0087  LDINT	R18	32767
      0x7C300C00,  //  0088  CALL	R12	6
      0xB8361000,  //  0089  GETNGBL	R13	K8
      0x8C341B12,  //  008A  GETMET	R13	R13	K18
      0x5C3C1800,  //  008B  MOVE	R15	R12
      0x7C340400,  //  008C  CALL	R13	2
      0xB83A1000,  //  008D  GETNGBL	R14	K8
      0x8C381D0D,  //  008E  GETMET	R14	R14	K13
      0x5C401A00,  //  008F  MOVE	R16	R13
      0x5445EFFF,  //  0090  LDINT	R17	-4096
      0x544A0FFF,  //  0091  LDINT	R18	4096
      0x5C4C0800,  //  0092  MOVE	R19	R4
      0x5C500A00,  //  0093  MOVE	R20	R5
      0x7C380C00,  //  0094  CALL	R14	6
      0x9002180E,  //  0095  SETMBR	R0	K12	R14
      0x700200E4,  //  0096  JMP		#017C
      0xB8321400,  //  0097  GETNGBL	R12	K10
      0x88301914,  //  0098  GETMBR	R12	R12	K20
      0x1C300C0C,  //  0099  EQ	R12	R6	R12
      0x78320014,  //  009A  JMPF	R12	#00B0
      0xB8321000,  //  009B  GETNGBL	R12	K8
      0x8C301909,  //  009C  GETMET	R12	R12	K9
      0x5C381600,  //  009D  MOVE	R14	R11
      0x583C0006,  //  009E  LDCONST	R15	K6
      0x0440070E,  //  009F  SUB	R16	R3	K14
      0x58440006,  //  00A0  LDCONST	R17	K6
      0x544A00FE,  //  00A1  LDINT	R18	255
      0x7C300C00,  //  00A2  CALL	R12	6
      0x0834180C,  //  00A3  MUL	R13	R12	R12
      0x543A00FE,  //  00A4  LDINT	R14	255
      0x0C341A0E,  //  00A5  DIV	R13	R13	R14
      0xB83A1000,  //  00A6  GETNGBL	R14	K8
      0x8C381D0D,  //  00A7  GETMET	R14	R14	K13
      0x5C401A00,  //  00A8  MOVE	R16	R13
      0x58440006,  //  00A9  LDCONST	R17	K6
      0x544A00FE,  //  00AA  LDINT	R18	255
      0x5C4C0800,  //  00AB  MOVE	R19	R4
      0x5C500A00,  //  00AC  MOVE	R20	R5
      0x7C380C00,  //  00AD  CALL	R14	6
      0x9002180E,  //  00AE  SETMBR	R0	K12	R14
      0x700200CB,  //  00AF  JMP		#017C
      0xB8321400,  //  00B0  GETNGBL	R12	K10
      0x88301915,  //  00B1  GETMBR	R12	R12	K21
      0x1C300C0C,  //  00B2  EQ	R12	R6	R12
      0x7832001A,  //  00B3  JMPF	R12	#00CF
      0xB8321000,  //  00B4  GETNGBL	R12	K8
      0x8C301909,  //  00B5  GETMET	R12	R12	K9
      0x5C381600,  //  00B6  MOVE	R14	R11
      0x583C0006,  //  00B7  LDCONST	R15	K6
      0x0440070E,  //  00B8  SUB	R16	R3	K14
      0x58440006,  //  00B9  LDCONST	R17	K6
      0x544A00FE,  //  00BA  LDINT	R18	255
      0x7C300C00,  //  00BB  CALL	R12	6
      0x543600FE,  //  00BC  LDINT	R13	255
      0x543A00FE,  //  00BD  LDINT	R14	255
      0x04381C0C,  //  00BE  SUB	R14	R14	R12
      0x543E00FE,  //  00BF  LDINT	R15	255
      0x043C1E0C,  //  00C0  SUB	R15	R15	R12
      0x08381C0F,  //  00C1  MUL	R14	R14	R15
      0x543E00FE,  //  00C2  LDINT	R15	255
      0x0C381C0F,  //  00C3  DIV	R14	R14	R15
      0x04341A0E,  //  00C4  SUB	R13	R13	R14
      0xB83A1000,  //  00C5  GETNGBL	R14	K8
      0x8C381D0D,  //  00C6  GETMET	R14	R14	K13
      0x5C401A00,  //  00C7  MOVE	R16	R13
      0x58440006,  //  00C8  LDCONST	R17	K6
      0x544A00FE,  //  00C9  LDINT	R18	255
      0x5C4C0800,  //  00CA  MOVE	R19	R4
      0x5C500A00,  //  00CB  MOVE	R20	R5
      0x7C380C00,  //  00CC  CALL	R14	6
      0x9002180E,  //  00CD  SETMBR	R0	K12	R14
      0x700200AC,  //  00CE  JMP		#017C
      0xB8321400,  //  00CF  GETNGBL	R12	K10
      0x88301916,  //  00D0  GETMBR	R12	R12	K22
      0x1C300C0C,  //  00D1  EQ	R12	R6	R12
      0x78320046,  //  00D2  JMPF	R12	#011A
      0xB8321000,  //  00D3  GETNGBL	R12	K8
      0x8C301909,  //  00D4  GETMET	R12	R12	K9
      0x5C381600,  //  00D5  MOVE	R14	R11
      0x583C0006,  //  00D6  LDCONST	R15	K6
      0x0440070E,  //  00D7  SUB	R16	R3	K14
      0x58440006,  //  00D8  LDCONST	R17	K6
      0x544A00FE,  //  00D9  LDINT	R18	255
      0x7C300C00,  //  00DA  CALL	R12	6
      0x1C341906,  //  00DB  EQ	R13	R12	K6
      0x78360001,  //  00DC  JMPF	R13	#00DF
      0x90021804,  //  00DD  SETMBR	R0	K12	R4
      0x70020039,  //  00DE  JMP		#0119
      0x543600FE,  //  00DF  LDINT	R13	255
      0x1C34180D,  //  00E0  EQ	R13	R12	R13
      0x78360001,  //  00E1  JMPF	R13	#00E4
      0x90021805,  //  00E2  SETMBR	R0	K12	R5
      0x70020034,  //  00E3  JMP		#0119
      0xB8361000,  //  00E4  GETNGBL	R13	K8
      0x8C341B09,  //  00E5  GETMET	R13	R13	K9
      0x543E00FE,  //  00E6  LDINT	R15	255
      0x043C1E0C,  //  00E7  SUB	R15	R15	R12
      0x58400006,  //  00E8  LDCONST	R16	K6
      0x544600FE,  //  00E9  LDINT	R17	255
      0x544A00FE,  //  00EA  LDINT	R18	255
      0x544E001F,  //  00EB  LDINT	R19	32
      0x7C340C00,  //  00EC  CALL	R13	6
      0xB83A1000,  //  00ED  GETNGBL	R14	K8
      0x8C381D09,  //  00EE  GETMET	R14	R14	K9
      0x5C401800,  //  00EF  MOVE	R16	R12
      0x58440006,  //  00F0  LDCONST	R17	K6
      0x544A00FE,  //  00F1  LDINT	R18	255
      0x584C0006,  //  00F2  LDCONST	R19	K6
      0x54527FFE,  //  00F3  LDINT	R20	32767
      0x54560005,  //  00F4  LDINT	R21	6
      0x08502815,  //  00F5  MUL	R20	R20	R21
      0x7C380C00,  //  00F6  CALL	R14	6
      0xB83E1000,  //  00F7  GETNGBL	R15	K8
      0x8C3C1F12,  //  00F8  GETMET	R15	R15	K18
      0x54467FFE,  //  00F9  LDINT	R17	32767
      0x10441C11,  //  00FA  MOD	R17	R14	R17
      0x7C3C0400,  //  00FB  CALL	R15	2
      0x08401E0D,  //  00FC  MUL	R16	R15	R13
      0x54460FFF,  //  00FD  LDINT	R17	4096
      0x0C402011,  //  00FE  DIV	R16	R16	R17
      0xB8461000,  //  00FF  GETNGBL	R17	K8
      0x8C44230D,  //  0100  GETMET	R17	R17	K13
      0x5C4C1800,  //  0101  MOVE	R19	R12
      0x58500006,  //  0102  LDCONST	R20	K6
      0x545600FE,  //  0103  LDINT	R21	255
      0x58580006,  //  0104  LDCONST	R22	K6
      0x045C0A04,  //  0105  SUB	R23	R5	R4
      0x7C440C00,  //  0106  CALL	R17	6
      0x00480811,  //  0107  ADD	R18	R4	R17
      0x00482410,  //  0108  ADD	R18	R18	R16
      0x90021812,  //  0109  SETMBR	R0	K12	R18
      0x04480A04,  //  010A  SUB	R18	R5	R4
      0x544E0003,  //  010B  LDINT	R19	4
      0x0C4C2413,  //  010C  DIV	R19	R18	R19
      0x8850010C,  //  010D  GETMBR	R20	R0	K12
      0x00540A13,  //  010E  ADD	R21	R5	R19
      0x24502815,  //  010F  GT	R20	R20	R21
      0x78520001,  //  0110  JMPF	R20	#0113
      0x00500A13,  //  0111  ADD	R20	R5	R19
      0x90021814,  //  0112  SETMBR	R0	K12	R20
      0x8850010C,  //  0113  GETMBR	R20	R0	K12
      0x04540813,  //  0114  SUB	R21	R4	R19
      0x14502815,  //  0115  LT	R20	R20	R21
      0x78520001,  //  0116  JMPF	R20	#0119
      0x04500813,  //  0117  SUB	R20	R4	R19
      0x90021814,  //  0118  SETMBR	R0	K12	R20
      0x70020061,  //  0119  JMP		#017C
      0xB8321400,  //  011A  GETNGBL	R12	K10
      0x88301917,  //  011B  GETMBR	R12	R12	K23
      0x1C300C0C,  //  011C  EQ	R12	R6	R12
      0x7832005D,  //  011D  JMPF	R12	#017C
      0xB8321000,  //  011E  GETNGBL	R12	K8
      0x8C301909,  //  011F  GETMET	R12	R12	K9
      0x5C381600,  //  0120  MOVE	R14	R11
      0x583C0006,  //  0121  LDCONST	R15	K6
      0x0440070E,  //  0122  SUB	R16	R3	K14
      0x58440006,  //  0123  LDCONST	R17	K6
      0x544A00FE,  //  0124  LDINT	R18	255
      0x7C300C00,  //  0125  CALL	R12	6
      0x58340006,  //  0126  LDCONST	R13	K6
      0x543A007F,  //  0127  LDINT	R14	128
      0x1438180E,  //  0128  LT	R14	R12	R14
      0x783A0012,  //  0129  JMPF	R14	#013D
      0xB83A1000,  //  012A  GETNGBL	R14	K8
      0x8C381D09,  //  012B  GETMET	R14	R14	K9
      0x5C401800,  //  012C  MOVE	R16	R12
      0x58440006,  //  012D  LDCONST	R17	K6
      0x544A007E,  //  012E  LDINT	R18	127
      0x584C0006,  //  012F  LDCONST	R19	K6
      0x545200FE,  //  0130  LDINT	R20	255
      0x7C380C00,  //  0131  CALL	R14	6
      0x543E00FE,  //  0132  LDINT	R15	255
      0x544200FE,  //  0133  LDINT	R16	255
      0x0440200E,  //  0134  SUB	R16	R16	R14
      0x544600FE,  //  0135  LDINT	R17	255
      0x0444220E,  //  0136  SUB	R17	R17	R14
      0x08402011,  //  0137  MUL	R16	R16	R17
      0x544600FE,  //  0138  LDINT	R17	255
      0x0C402011,  //  0139  DIV	R16	R16	R17
      0x043C1E10,  //  013A  SUB	R15	R15	R16
      0x5C341E00,  //  013B  MOVE	R13	R15
      0x70020035,  //  013C  JMP		#0173
      0x543A00BF,  //  013D  LDINT	R14	192
      0x1438180E,  //  013E  LT	R14	R12	R14
      0x783A0017,  //  013F  JMPF	R14	#0158
      0xB83A1000,  //  0140  GETNGBL	R14	K8
      0x8C381D09,  //  0141  GETMET	R14	R14	K9
      0x5442007F,  //  0142  LDINT	R16	128
      0x04401810,  //  0143  SUB	R16	R12	R16
      0x58440006,  //  0144  LDCONST	R17	K6
      0x544A003E,  //  0145  LDINT	R18	63
      0x584C0006,  //  0146  LDCONST	R19	K6
      0x545200FE,  //  0147  LDINT	R20	255
      0x7C380C00,  //  0148  CALL	R14	6
      0x543E00FE,  //  0149  LDINT	R15	255
      0x544200FE,  //  014A  LDINT	R16	255
      0x0440200E,  //  014B  SUB	R16	R16	R14
      0x544600FE,  //  014C  LDINT	R17	255
      0x0444220E,  //  014D  SUB	R17	R17	R14
      0x08402011,  //  014E  MUL	R16	R16	R17
      0x544600FE,  //  014F  LDINT	R17	255
      0x0C402011,  //  0150  DIV	R16	R16	R17
      0x043C1E10,  //  0151  SUB	R15	R15	R16
      0x5442007F,  //  0152  LDINT	R16	128
      0x08401E10,  //  0153  MUL	R16	R15	R16
      0x544600FE,  //  0154  LDINT	R17	255
      0x0C402011,  //  0155  DIV	R16	R16	R17
      0x5C342000,  //  0156  MOVE	R13	R16
      0x7002001A,  //  0157  JMP		#0173
      0xB83A1000,  //  0158  GETNGBL	R14	K8
      0x8C381D09,  //  0159  GETMET	R14	R14	K9
      0x544200BF,  //  015A  LDINT	R16	192
      0x04401810,  //  015B  SUB	R16	R12	R16
      0x58440006,  //  015C  LDCONST	R17	K6
      0x544A003E,  //  015D  LDINT	R18	63
      0x584C0006,  //  015E  LDCONST	R19	K6
      0x545200FE,  //  015F  LDINT	R20	255
      0x7C380C00,  //  0160  CALL	R14	6
      0x543E00FE,  //  0161  LDINT	R15	255
      0x544200FE,  //  0162  LDINT	R16	255
      0x0440200E,  //  0163  SUB	R16	R16	R14
      0x544600FE,  //  0164  LDINT	R17	255
      0x0444220E,  //  0165  SUB	R17	R17	R14
      0x08402011,  //  0166  MUL	R16	R16	R17
      0x544600FE,  //  0167  LDINT	R17	255
      0x0C402011,  //  0168  DIV	R16	R16	R17
      0x043C1E10,  //  0169  SUB	R15	R15	R16
      0x544200FE,  //  016A  LDINT	R16	255
      0x544600FE,  //  016B  LDINT	R17	255
      0x0444220F,  //  016C  SUB	R17	R17	R15
      0x544A003F,  //  016D  LDINT	R18	64
      0x08442212,  //  016E  MUL	R17	R17	R18
      0x544A00FE,  //  016F  LDINT	R18	255
      0x0C442212,  //  0170  DIV	R17	R17	R18
      0x04402011,  //  0171  SUB	R16	R16	R17
      0x5C342000,  //  0172  MOVE	R13	R16
      0xB83A1000,  //  0173  GETNGBL	R14	K8
      0x8C381D0D,  //  0174  GETMET	R14	R14	K13
      0x5C401A00,  //  0175  MOVE	R16	R13
      0x58440006,  //  0176  LDCONST	R17	K6
      0x544A00FE,  //  0177  LDINT	R18	255
      0x5C4C0800,  //  0178  MOVE	R19	R4
      0x5C500A00,  //  0179  MOVE	R20	R5
      0x7C380C00,  //  017A  CALL	R14	6
      0x9002180E,  //  017B  SETMBR	R0	K12	R14
      0x8830010C,  //  017C  GETMBR	R12	R0	K12
      0x80041800,  //  017D  RET	1	R12
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_OscillatorValueProvider_tostring,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_OscillatorValueProvider,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x88040103,  //  0000  GETMBR	R1	R0	K3
      0x2804030E,  //  0001  GE	R1	R1	K14
      0x78060007,  //  0002  JMPF	R1	#000B
      0x88040103,  //  0003  GETMBR	R1	R0	K3
      0x540A0008,  //  0004  LDINT	R2	9
      0x18040202,  //  0005  LE	R1	R1	R2
      0x78060003,  //  0006  JMPF	R1	#000B
      0x88040118,  //  0007  GETMBR	R1	R0	K24
      0x88080103,  //  0008  GETMBR	R2	R0	K3
      0x94040202,  //  0009  GETIDX	R1	R1	R2
      0x70020000,  //  000A  JMP		#000C
      0x58040019,  //  000B  LDCONST	R1	K25
      0x60080018,  //  000C  GETGBL	R2	G24
      0x580C001A,  //  000D  LDCONST	R3	K26
      0x88100101,  //  000E  GETMBR	R4	R0	K1
      0x88140102,  //  000F  GETMBR	R5	R0	K2
      0x88180100,  //  0010  GETMBR	R6	R0	K0
      0x5C1C0200,  //  0011  MOVE	R7	R1
      0x7C080A00,  //  0012  CALL	R2	5
      0x80040400,  //  0013  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_OscillatorValueProvider_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_OscillatorValueProvider,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C08051B,  //  0003  GETMET	R2	R2	K27
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x90020F06,  //  0006  SETMBR	R0	K7	K6
      0x90021906,  //  0007  SETMBR	R0	K12	K6
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(class_OscillatorValueProvider_start,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_OscillatorValueProvider,     /* shared constants */
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x8808011C,  //  0003  GETMBR	R2	R0	K28
      0x8804051D,  //  0004  GETMBR	R1	R2	K29
      0x90020E01,  //  0005  SETMBR	R0	K7	R1
      0x80040000,  //  0006  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: OscillatorValueProvider
********************************************************************/
extern const bclass be_class_ValueProvider;
be_local_class(OscillatorValueProvider,
    2,
    &be_class_ValueProvider,
    be_nested_map(8,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(form_names, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(10,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(),
        be_nested_str_weak(SAWTOOTH),
        be_nested_str_weak(TRIANGLE),
        be_nested_str_weak(SQUARE),
        be_nested_str_weak(COSINE),
        be_nested_str_weak(SINE),
        be_nested_str_weak(EASE_IN),
        be_nested_str_weak(EASE_OUT),
        be_nested_str_weak(ELASTIC),
        be_nested_str_weak(BOUNCE),
    }))    ) } )) },
        { be_const_key_weak(produce_value, -1), be_const_closure(class_OscillatorValueProvider_produce_value_closure) },
        { be_const_key_weak(value, -1), be_const_var(1) },
        { be_const_key_weak(PARAMS, 6), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(phase, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(0) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(100) },
    }))    ) } )) },
        { be_const_key_weak(form, 4), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(enum, 1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(9,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
        be_const_int(4),
        be_const_int(5),
        be_const_int(6),
        be_const_int(7),
        be_const_int(8),
        be_const_int(9),
    }))    ) } )) },
        { be_const_key_weak(default, -1), be_const_int(1) },
    }))    ) } )) },
        { be_const_key_weak(duty_cycle, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(50) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(100) },
    }))    ) } )) },
        { be_const_key_weak(min_value, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(0) },
    }))    ) } )) },
        { be_const_key_weak(duration, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(1000) },
        { be_const_key_weak(min, -1), be_const_int(1) },
    }))    ) } )) },
        { be_const_key_weak(max_value, 1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(100) },
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(start, -1), be_const_closure(class_OscillatorValueProvider_start_closure) },
        { be_const_key_weak(tostring, -1), be_const_closure(class_OscillatorValueProvider_tostring_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_OscillatorValueProvider_init_closure) },
        { be_const_key_weak(origin, 4), be_const_var(0) },
    })),
    be_str_weak(OscillatorValueProvider)
);

/********************************************************************
** Solidified function: rich_palette_rainbow
********************************************************************/
be_local_closure(rich_palette_rainbow,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(00FF000024FFA50049FFFF006E00FF00920000FFB74B0082DBEE82EEFFFF0000),
    /* K1   */  be_nested_str_weak(animation),
    /* K2   */  be_nested_str_weak(rich_palette),
    /* K3   */  be_nested_str_weak(palette),
    }),
    be_str_weak(rich_palette_rainbow),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x60040015,  //  0000  GETGBL	R1	G21
      0x58080000,  //  0001  LDCONST	R2	K0
      0x7C040200,  //  0002  CALL	R1	1
      0xB80A0200,  //  0003  GETNGBL	R2	K1
      0x8C080502,  //  0004  GETMET	R2	R2	K2
      0x5C100000,  //  0005  MOVE	R4	R0
      0x7C080400,  //  0006  CALL	R2	2
      0x900A0601,  //  0007  SETMBR	R2	K3	R1
      0x80040400,  //  0008  RET	1	R2
    })
  )
);
/*******************************************************************/

// compact class 'PalettePatternAnimation' ktab size: 21, total: 40 (saved 152 bytes)
static const bvalue be_ktab_class_PalettePatternAnimation[21] = {
  /* K0   */  be_nested_str_weak(pattern_func),
  /* K1   */  be_nested_str_weak(color_source),
  /* K2   */  be_nested_str_weak(_initialize_value_buffer),
  /* K3   */  be_nested_str_weak(is_running),
  /* K4   */  be_nested_str_weak(engine),
  /* K5   */  be_nested_str_weak(time_ms),
  /* K6   */  be_nested_str_weak(start_time),
  /* K7   */  be_nested_str_weak(get_strip_length),
  /* K8   */  be_const_int(0),
  /* K9   */  be_nested_str_weak(width),
  /* K10  */  be_nested_str_weak(value_buffer),
  /* K11  */  be_nested_str_weak(get_color_for_value),
  /* K12  */  be_nested_str_weak(current_color),
  /* K13  */  be_nested_str_weak(set_pixel_color),
  /* K14  */  be_const_int(1),
  /* K15  */  be_nested_str_weak(resize),
  /* K16  */  be_nested_str_weak(init),
  /* K17  */  be_nested_str_weak(PalettePatternAnimation_X28strip_length_X3D_X25s_X2C_X20priority_X3D_X25s_X2C_X20running_X3D_X25s_X29),
  /* K18  */  be_nested_str_weak(priority),
  /* K19  */  be_nested_str_weak(update),
  /* K20  */  be_nested_str_weak(_update_value_buffer),
};


extern const bclass be_class_PalettePatternAnimation;

/********************************************************************
** Solidified function: on_param_changed
********************************************************************/
be_local_closure(class_PalettePatternAnimation_on_param_changed,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_PalettePatternAnimation,     /* shared constants */
    be_str_weak(on_param_changed),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x1C0C0300,  //  0000  EQ	R3	R1	K0
      0x740E0001,  //  0001  JMPT	R3	#0004
      0x1C0C0301,  //  0002  EQ	R3	R1	K1
      0x780E0001,  //  0003  JMPF	R3	#0006
      0x8C0C0102,  //  0004  GETMET	R3	R0	K2
      0x7C0C0200,  //  0005  CALL	R3	1
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: render
********************************************************************/
be_local_closure(class_PalettePatternAnimation_render,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_PalettePatternAnimation,     /* shared constants */
    be_str_weak(render),
    &be_const_str_solidified,
    ( &(const binstruction[51]) {  /* code */
      0x880C0103,  //  0000  GETMBR	R3	R0	K3
      0x780E0002,  //  0001  JMPF	R3	#0005
      0x4C0C0000,  //  0002  LDNIL	R3
      0x1C0C0203,  //  0003  EQ	R3	R1	R3
      0x780E0001,  //  0004  JMPF	R3	#0007
      0x500C0000,  //  0005  LDBOOL	R3	0	0
      0x80040600,  //  0006  RET	1	R3
      0x4C0C0000,  //  0007  LDNIL	R3
      0x1C0C0403,  //  0008  EQ	R3	R2	R3
      0x780E0001,  //  0009  JMPF	R3	#000C
      0x880C0104,  //  000A  GETMBR	R3	R0	K4
      0x88080705,  //  000B  GETMBR	R2	R3	K5
      0x880C0101,  //  000C  GETMBR	R3	R0	K1
      0x4C100000,  //  000D  LDNIL	R4
      0x1C100604,  //  000E  EQ	R4	R3	R4
      0x78120001,  //  000F  JMPF	R4	#0012
      0x50100000,  //  0010  LDBOOL	R4	0	0
      0x80040800,  //  0011  RET	1	R4
      0x88100106,  //  0012  GETMBR	R4	R0	K6
      0x04100404,  //  0013  SUB	R4	R2	R4
      0x88140104,  //  0014  GETMBR	R5	R0	K4
      0x8C140B07,  //  0015  GETMET	R5	R5	K7
      0x7C140200,  //  0016  CALL	R5	1
      0x58180008,  //  0017  LDCONST	R6	K8
      0x141C0C05,  //  0018  LT	R7	R6	R5
      0x781E0016,  //  0019  JMPF	R7	#0031
      0x881C0309,  //  001A  GETMBR	R7	R1	K9
      0x141C0C07,  //  001B  LT	R7	R6	R7
      0x781E0013,  //  001C  JMPF	R7	#0031
      0x881C010A,  //  001D  GETMBR	R7	R0	K10
      0x941C0E06,  //  001E  GETIDX	R7	R7	R6
      0x4C200000,  //  001F  LDNIL	R8
      0x8824070B,  //  0020  GETMBR	R9	R3	K11
      0x4C280000,  //  0021  LDNIL	R10
      0x2024120A,  //  0022  NE	R9	R9	R10
      0x78260005,  //  0023  JMPF	R9	#002A
      0x8C24070B,  //  0024  GETMET	R9	R3	K11
      0x5C2C0E00,  //  0025  MOVE	R11	R7
      0x5C300800,  //  0026  MOVE	R12	R4
      0x7C240600,  //  0027  CALL	R9	3
      0x5C201200,  //  0028  MOVE	R8	R9
      0x70020000,  //  0029  JMP		#002B
      0x8820070C,  //  002A  GETMBR	R8	R3	K12
      0x8C24030D,  //  002B  GETMET	R9	R1	K13
      0x5C2C0C00,  //  002C  MOVE	R11	R6
      0x5C301000,  //  002D  MOVE	R12	R8
      0x7C240600,  //  002E  CALL	R9	3
      0x00180D0E,  //  002F  ADD	R6	R6	K14
      0x7001FFE6,  //  0030  JMP		#0018
      0x501C0200,  //  0031  LDBOOL	R7	1	0
      0x80040E00,  //  0032  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _update_value_buffer
********************************************************************/
be_local_closure(class_PalettePatternAnimation__update_value_buffer,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_PalettePatternAnimation,     /* shared constants */
    be_str_weak(_update_value_buffer),
    &be_const_str_solidified,
    ( &(const binstruction[30]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x1C0C0403,  //  0002  EQ	R3	R2	R3
      0x780E0000,  //  0003  JMPF	R3	#0005
      0x80000600,  //  0004  RET	0
      0x880C0104,  //  0005  GETMBR	R3	R0	K4
      0x8C0C0707,  //  0006  GETMET	R3	R3	K7
      0x7C0C0200,  //  0007  CALL	R3	1
      0x6010000C,  //  0008  GETGBL	R4	G12
      0x8814010A,  //  0009  GETMBR	R5	R0	K10
      0x7C100200,  //  000A  CALL	R4	1
      0x20100803,  //  000B  NE	R4	R4	R3
      0x78120003,  //  000C  JMPF	R4	#0011
      0x8810010A,  //  000D  GETMBR	R4	R0	K10
      0x8C10090F,  //  000E  GETMET	R4	R4	K15
      0x5C180600,  //  000F  MOVE	R6	R3
      0x7C100400,  //  0010  CALL	R4	2
      0x58100008,  //  0011  LDCONST	R4	K8
      0x14140803,  //  0012  LT	R5	R4	R3
      0x78160008,  //  0013  JMPF	R5	#001D
      0x8814010A,  //  0014  GETMBR	R5	R0	K10
      0x5C180400,  //  0015  MOVE	R6	R2
      0x5C1C0800,  //  0016  MOVE	R7	R4
      0x5C200200,  //  0017  MOVE	R8	R1
      0x5C240000,  //  0018  MOVE	R9	R0
      0x7C180600,  //  0019  CALL	R6	3
      0x98140806,  //  001A  SETIDX	R5	R4	R6
      0x0010090E,  //  001B  ADD	R4	R4	K14
      0x7001FFF4,  //  001C  JMP		#0012
      0x80000000,  //  001D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _initialize_value_buffer
********************************************************************/
be_local_closure(class_PalettePatternAnimation__initialize_value_buffer,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_PalettePatternAnimation,     /* shared constants */
    be_str_weak(_initialize_value_buffer),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x8C040307,  //  0001  GETMET	R1	R1	K7
      0x7C040200,  //  0002  CALL	R1	1
      0x8808010A,  //  0003  GETMBR	R2	R0	K10
      0x8C08050F,  //  0004  GETMET	R2	R2	K15
      0x5C100200,  //  0005  MOVE	R4	R1
      0x7C080400,  //  0006  CALL	R2	2
      0x58080008,  //  0007  LDCONST	R2	K8
      0x140C0401,  //  0008  LT	R3	R2	R1
      0x780E0003,  //  0009  JMPF	R3	#000E
      0x880C010A,  //  000A  GETMBR	R3	R0	K10
      0x980C0508,  //  000B  SETIDX	R3	R2	K8
      0x0008050E,  //  000C  ADD	R2	R2	K14
      0x7001FFF9,  //  000D  JMP		#0008
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_PalettePatternAnimation_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_PalettePatternAnimation,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080510,  //  0003  GETMET	R2	R2	K16
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x60080012,  //  0006  GETGBL	R2	G18
      0x7C080000,  //  0007  CALL	R2	0
      0x90021402,  //  0008  SETMBR	R0	K10	R2
      0x8C080102,  //  0009  GETMET	R2	R0	K2
      0x7C080200,  //  000A  CALL	R2	1
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_PalettePatternAnimation_tostring,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_PalettePatternAnimation,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x8C040307,  //  0001  GETMET	R1	R1	K7
      0x7C040200,  //  0002  CALL	R1	1
      0x60080018,  //  0003  GETGBL	R2	G24
      0x580C0011,  //  0004  LDCONST	R3	K17
      0x5C100200,  //  0005  MOVE	R4	R1
      0x88140112,  //  0006  GETMBR	R5	R0	K18
      0x88180103,  //  0007  GETMBR	R6	R0	K3
      0x7C080800,  //  0008  CALL	R2	4
      0x80040400,  //  0009  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update
********************************************************************/
be_local_closure(class_PalettePatternAnimation_update,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_PalettePatternAnimation,     /* shared constants */
    be_str_weak(update),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080513,  //  0003  GETMET	R2	R2	K19
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x740A0001,  //  0006  JMPT	R2	#0009
      0x50080000,  //  0007  LDBOOL	R2	0	0
      0x80040400,  //  0008  RET	1	R2
      0x88080106,  //  0009  GETMBR	R2	R0	K6
      0x04080202,  //  000A  SUB	R2	R1	R2
      0x8C0C0114,  //  000B  GETMET	R3	R0	K20
      0x5C140400,  //  000C  MOVE	R5	R2
      0x7C0C0400,  //  000D  CALL	R3	2
      0x500C0200,  //  000E  LDBOOL	R3	1	0
      0x80040600,  //  000F  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: PalettePatternAnimation
********************************************************************/
extern const bclass be_class_Animation;
be_local_class(PalettePatternAnimation,
    1,
    &be_class_Animation,
    be_nested_map(9,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(update, 1), be_const_closure(class_PalettePatternAnimation_update_closure) },
        { be_const_key_weak(tostring, -1), be_const_closure(class_PalettePatternAnimation_tostring_closure) },
        { be_const_key_weak(_update_value_buffer, -1), be_const_closure(class_PalettePatternAnimation__update_value_buffer_closure) },
        { be_const_key_weak(_initialize_value_buffer, -1), be_const_closure(class_PalettePatternAnimation__initialize_value_buffer_closure) },
        { be_const_key_weak(on_param_changed, 8), be_const_closure(class_PalettePatternAnimation_on_param_changed_closure) },
        { be_const_key_weak(PARAMS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(color_source, 1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_nil() },
        { be_const_key_weak(type, -1), be_nested_str_weak(instance) },
    }))    ) } )) },
        { be_const_key_weak(pattern_func, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_nil() },
        { be_const_key_weak(type, -1), be_nested_str_weak(function) },
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(init, 5), be_const_closure(class_PalettePatternAnimation_init_closure) },
        { be_const_key_weak(render, 0), be_const_closure(class_PalettePatternAnimation_render_closure) },
        { be_const_key_weak(value_buffer, -1), be_const_var(0) },
    })),
    be_str_weak(PalettePatternAnimation)
);

/********************************************************************
** Solidified function: sine_osc
********************************************************************/
be_local_closure(sine_osc,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(oscillator_value),
    /* K2   */  be_nested_str_weak(form),
    /* K3   */  be_nested_str_weak(SINE),
    }),
    be_str_weak(sine_osc),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0xB80A0000,  //  0004  GETNGBL	R2	K0
      0x88080503,  //  0005  GETMBR	R2	R2	K3
      0x90060402,  //  0006  SETMBR	R1	K2	R2
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: bounce
********************************************************************/
be_local_closure(bounce,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(oscillator_value),
    /* K2   */  be_nested_str_weak(form),
    /* K3   */  be_nested_str_weak(BOUNCE),
    }),
    be_str_weak(bounce),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0xB80A0000,  //  0004  GETNGBL	R2	K0
      0x88080503,  //  0005  GETMBR	R2	R2	K3
      0x90060402,  //  0006  SETMBR	R1	K2	R2
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/

// compact class 'ShiftAnimation' ktab size: 33, total: 62 (saved 232 bytes)
static const bvalue be_ktab_class_ShiftAnimation[33] = {
  /* K0   */  be_nested_str_weak(engine),
  /* K1   */  be_nested_str_weak(get_strip_length),
  /* K2   */  be_nested_str_weak(source_frame),
  /* K3   */  be_nested_str_weak(animation),
  /* K4   */  be_nested_str_weak(frame_buffer),
  /* K5   */  be_nested_str_weak(current_colors),
  /* K6   */  be_nested_str_weak(resize),
  /* K7   */  be_const_int(0),
  /* K8   */  be_const_int(-16777216),
  /* K9   */  be_const_int(1),
  /* K10  */  be_nested_str_weak(update),
  /* K11  */  be_nested_str_weak(shift_speed),
  /* K12  */  be_nested_str_weak(direction),
  /* K13  */  be_nested_str_weak(wrap_around),
  /* K14  */  be_nested_str_weak(source_animation),
  /* K15  */  be_nested_str_weak(start_time),
  /* K16  */  be_nested_str_weak(tasmota),
  /* K17  */  be_nested_str_weak(scale_uint),
  /* K18  */  be_nested_str_weak(current_offset),
  /* K19  */  be_nested_str_weak(is_running),
  /* K20  */  be_nested_str_weak(start),
  /* K21  */  be_nested_str_weak(_calculate_shift),
  /* K22  */  be_nested_str_weak(priority),
  /* K23  */  be_nested_str_weak(right),
  /* K24  */  be_nested_str_weak(left),
  /* K25  */  be_nested_str_weak(ShiftAnimation_X28_X25s_X2C_X20speed_X3D_X25s_X2C_X20wrap_X3D_X25s_X2C_X20priority_X3D_X25s_X2C_X20running_X3D_X25s_X29),
  /* K26  */  be_nested_str_weak(width),
  /* K27  */  be_nested_str_weak(set_pixel_color),
  /* K28  */  be_nested_str_weak(_initialize_buffers),
  /* K29  */  be_nested_str_weak(clear),
  /* K30  */  be_nested_str_weak(render),
  /* K31  */  be_nested_str_weak(get_pixel_color),
  /* K32  */  be_nested_str_weak(init),
};


extern const bclass be_class_ShiftAnimation;

/********************************************************************
** Solidified function: _initialize_buffers
********************************************************************/
be_local_closure(class_ShiftAnimation__initialize_buffers,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ShiftAnimation,     /* shared constants */
    be_str_weak(_initialize_buffers),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0xB80A0600,  //  0003  GETNGBL	R2	K3
      0x8C080504,  //  0004  GETMET	R2	R2	K4
      0x5C100200,  //  0005  MOVE	R4	R1
      0x7C080400,  //  0006  CALL	R2	2
      0x90020402,  //  0007  SETMBR	R0	K2	R2
      0x60080012,  //  0008  GETGBL	R2	G18
      0x7C080000,  //  0009  CALL	R2	0
      0x90020A02,  //  000A  SETMBR	R0	K5	R2
      0x88080105,  //  000B  GETMBR	R2	R0	K5
      0x8C080506,  //  000C  GETMET	R2	R2	K6
      0x5C100200,  //  000D  MOVE	R4	R1
      0x7C080400,  //  000E  CALL	R2	2
      0x58080007,  //  000F  LDCONST	R2	K7
      0x140C0401,  //  0010  LT	R3	R2	R1
      0x780E0003,  //  0011  JMPF	R3	#0016
      0x880C0105,  //  0012  GETMBR	R3	R0	K5
      0x980C0508,  //  0013  SETIDX	R3	R2	K8
      0x00080509,  //  0014  ADD	R2	R2	K9
      0x7001FFF9,  //  0015  JMP		#0010
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update
********************************************************************/
be_local_closure(class_ShiftAnimation_update,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ShiftAnimation,     /* shared constants */
    be_str_weak(update),
    &be_const_str_solidified,
    ( &(const binstruction[63]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C08050A,  //  0003  GETMET	R2	R2	K10
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x8808010B,  //  0006  GETMBR	R2	R0	K11
      0x880C010C,  //  0007  GETMBR	R3	R0	K12
      0x8810010D,  //  0008  GETMBR	R4	R0	K13
      0x8814010E,  //  0009  GETMBR	R5	R0	K14
      0x88180100,  //  000A  GETMBR	R6	R0	K0
      0x8C180D01,  //  000B  GETMET	R6	R6	K1
      0x7C180200,  //  000C  CALL	R6	1
      0x241C0507,  //  000D  GT	R7	R2	K7
      0x781E0020,  //  000E  JMPF	R7	#0030
      0x881C010F,  //  000F  GETMBR	R7	R0	K15
      0x041C0207,  //  0010  SUB	R7	R1	R7
      0xB8222000,  //  0011  GETNGBL	R8	K16
      0x8C201111,  //  0012  GETMET	R8	R8	K17
      0x5C280400,  //  0013  MOVE	R10	R2
      0x582C0007,  //  0014  LDCONST	R11	K7
      0x543200FE,  //  0015  LDINT	R12	255
      0x58340007,  //  0016  LDCONST	R13	K7
      0x543A0009,  //  0017  LDINT	R14	10
      0x543E00FF,  //  0018  LDINT	R15	256
      0x08381C0F,  //  0019  MUL	R14	R14	R15
      0x7C200C00,  //  001A  CALL	R8	6
      0x24241107,  //  001B  GT	R9	R8	K7
      0x78260012,  //  001C  JMPF	R9	#0030
      0x08240E08,  //  001D  MUL	R9	R7	R8
      0x542A03E7,  //  001E  LDINT	R10	1000
      0x0C24120A,  //  001F  DIV	R9	R9	R10
      0x08241203,  //  0020  MUL	R9	R9	R3
      0x7812000C,  //  0021  JMPF	R4	#002F
      0x542A00FF,  //  0022  LDINT	R10	256
      0x08280C0A,  //  0023  MUL	R10	R6	R10
      0x1028120A,  //  0024  MOD	R10	R9	R10
      0x9002240A,  //  0025  SETMBR	R0	K18	R10
      0x88280112,  //  0026  GETMBR	R10	R0	K18
      0x14281507,  //  0027  LT	R10	R10	K7
      0x782A0004,  //  0028  JMPF	R10	#002E
      0x542E00FF,  //  0029  LDINT	R11	256
      0x082C0C0B,  //  002A  MUL	R11	R6	R11
      0x88280112,  //  002B  GETMBR	R10	R0	K18
      0x0028140B,  //  002C  ADD	R10	R10	R11
      0x9002240A,  //  002D  SETMBR	R0	K18	R10
      0x70020000,  //  002E  JMP		#0030
      0x90022409,  //  002F  SETMBR	R0	K18	R9
      0x4C1C0000,  //  0030  LDNIL	R7
      0x201C0A07,  //  0031  NE	R7	R5	R7
      0x781E0007,  //  0032  JMPF	R7	#003B
      0x881C0B13,  //  0033  GETMBR	R7	R5	K19
      0x741E0002,  //  0034  JMPT	R7	#0038
      0x8C1C0B14,  //  0035  GETMET	R7	R5	K20
      0x8824010F,  //  0036  GETMBR	R9	R0	K15
      0x7C1C0400,  //  0037  CALL	R7	2
      0x8C1C0B0A,  //  0038  GETMET	R7	R5	K10
      0x5C240200,  //  0039  MOVE	R9	R1
      0x7C1C0400,  //  003A  CALL	R7	2
      0x8C1C0115,  //  003B  GETMET	R7	R0	K21
      0x7C1C0200,  //  003C  CALL	R7	1
      0x501C0200,  //  003D  LDBOOL	R7	1	0
      0x80040E00,  //  003E  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_ShiftAnimation_tostring,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ShiftAnimation,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8804010C,  //  0000  GETMBR	R1	R0	K12
      0x8808010B,  //  0001  GETMBR	R2	R0	K11
      0x880C010D,  //  0002  GETMBR	R3	R0	K13
      0x88100116,  //  0003  GETMBR	R4	R0	K22
      0x24140307,  //  0004  GT	R5	R1	K7
      0x78160001,  //  0005  JMPF	R5	#0008
      0x58140017,  //  0006  LDCONST	R5	K23
      0x70020000,  //  0007  JMP		#0009
      0x58140018,  //  0008  LDCONST	R5	K24
      0x60180018,  //  0009  GETGBL	R6	G24
      0x581C0019,  //  000A  LDCONST	R7	K25
      0x5C200A00,  //  000B  MOVE	R8	R5
      0x5C240400,  //  000C  MOVE	R9	R2
      0x5C280600,  //  000D  MOVE	R10	R3
      0x5C2C0800,  //  000E  MOVE	R11	R4
      0x88300113,  //  000F  GETMBR	R12	R0	K19
      0x7C180C00,  //  0010  CALL	R6	6
      0x80040C00,  //  0011  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: render
********************************************************************/
be_local_closure(class_ShiftAnimation_render,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ShiftAnimation,     /* shared constants */
    be_str_weak(render),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x880C0113,  //  0000  GETMBR	R3	R0	K19
      0x780E0002,  //  0001  JMPF	R3	#0005
      0x4C0C0000,  //  0002  LDNIL	R3
      0x1C0C0203,  //  0003  EQ	R3	R1	R3
      0x780E0001,  //  0004  JMPF	R3	#0007
      0x500C0000,  //  0005  LDBOOL	R3	0	0
      0x80040600,  //  0006  RET	1	R3
      0x880C0100,  //  0007  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0008  GETMET	R3	R3	K1
      0x7C0C0200,  //  0009  CALL	R3	1
      0x58100007,  //  000A  LDCONST	R4	K7
      0x14140803,  //  000B  LT	R5	R4	R3
      0x78160009,  //  000C  JMPF	R5	#0017
      0x8814031A,  //  000D  GETMBR	R5	R1	K26
      0x14140805,  //  000E  LT	R5	R4	R5
      0x78160004,  //  000F  JMPF	R5	#0015
      0x8C14031B,  //  0010  GETMET	R5	R1	K27
      0x5C1C0800,  //  0011  MOVE	R7	R4
      0x88200105,  //  0012  GETMBR	R8	R0	K5
      0x94201004,  //  0013  GETIDX	R8	R8	R4
      0x7C140600,  //  0014  CALL	R5	3
      0x00100909,  //  0015  ADD	R4	R4	K9
      0x7001FFF3,  //  0016  JMP		#000B
      0x50140200,  //  0017  LDBOOL	R5	1	0
      0x80040A00,  //  0018  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: on_param_changed
********************************************************************/
be_local_closure(class_ShiftAnimation_on_param_changed,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ShiftAnimation,     /* shared constants */
    be_str_weak(on_param_changed),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x1C0C030E,  //  0000  EQ	R3	R1	K14
      0x780E0001,  //  0001  JMPF	R3	#0004
      0x8C0C011C,  //  0002  GETMET	R3	R0	K28
      0x7C0C0200,  //  0003  CALL	R3	1
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _calculate_shift
********************************************************************/
be_local_closure(class_ShiftAnimation__calculate_shift,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ShiftAnimation,     /* shared constants */
    be_str_weak(_calculate_shift),
    &be_const_str_solidified,
    ( &(const binstruction[64]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x6008000C,  //  0003  GETGBL	R2	G12
      0x880C0105,  //  0004  GETMBR	R3	R0	K5
      0x7C080200,  //  0005  CALL	R2	1
      0x20080401,  //  0006  NE	R2	R2	R1
      0x780A0001,  //  0007  JMPF	R2	#000A
      0x8C08011C,  //  0008  GETMET	R2	R0	K28
      0x7C080200,  //  0009  CALL	R2	1
      0x8808010E,  //  000A  GETMBR	R2	R0	K14
      0x880C010D,  //  000B  GETMBR	R3	R0	K13
      0x88100102,  //  000C  GETMBR	R4	R0	K2
      0x8C10091D,  //  000D  GETMET	R4	R4	K29
      0x7C100200,  //  000E  CALL	R4	1
      0x4C100000,  //  000F  LDNIL	R4
      0x20100404,  //  0010  NE	R4	R2	R4
      0x78120003,  //  0011  JMPF	R4	#0016
      0x8C10051E,  //  0012  GETMET	R4	R2	K30
      0x88180102,  //  0013  GETMBR	R6	R0	K2
      0x581C0007,  //  0014  LDCONST	R7	K7
      0x7C100600,  //  0015  CALL	R4	3
      0x88100112,  //  0016  GETMBR	R4	R0	K18
      0x541600FF,  //  0017  LDINT	R5	256
      0x0C100805,  //  0018  DIV	R4	R4	R5
      0x88140112,  //  0019  GETMBR	R5	R0	K18
      0x541A00FF,  //  001A  LDINT	R6	256
      0x10140A06,  //  001B  MOD	R5	R5	R6
      0x58180007,  //  001C  LDCONST	R6	K7
      0x141C0C01,  //  001D  LT	R7	R6	R1
      0x781E001F,  //  001E  JMPF	R7	#003F
      0x041C0C04,  //  001F  SUB	R7	R6	R4
      0x780E000E,  //  0020  JMPF	R3	#0030
      0x14200F07,  //  0021  LT	R8	R7	K7
      0x78220001,  //  0022  JMPF	R8	#0025
      0x001C0E01,  //  0023  ADD	R7	R7	R1
      0x7001FFFB,  //  0024  JMP		#0021
      0x28200E01,  //  0025  GE	R8	R7	R1
      0x78220001,  //  0026  JMPF	R8	#0029
      0x041C0E01,  //  0027  SUB	R7	R7	R1
      0x7001FFFB,  //  0028  JMP		#0025
      0x88200105,  //  0029  GETMBR	R8	R0	K5
      0x88240102,  //  002A  GETMBR	R9	R0	K2
      0x8C24131F,  //  002B  GETMET	R9	R9	K31
      0x5C2C0E00,  //  002C  MOVE	R11	R7
      0x7C240400,  //  002D  CALL	R9	2
      0x98200C09,  //  002E  SETIDX	R8	R6	R9
      0x7002000C,  //  002F  JMP		#003D
      0x28200F07,  //  0030  GE	R8	R7	K7
      0x78220008,  //  0031  JMPF	R8	#003B
      0x14200E01,  //  0032  LT	R8	R7	R1
      0x78220006,  //  0033  JMPF	R8	#003B
      0x88200105,  //  0034  GETMBR	R8	R0	K5
      0x88240102,  //  0035  GETMBR	R9	R0	K2
      0x8C24131F,  //  0036  GETMET	R9	R9	K31
      0x5C2C0E00,  //  0037  MOVE	R11	R7
      0x7C240400,  //  0038  CALL	R9	2
      0x98200C09,  //  0039  SETIDX	R8	R6	R9
      0x70020001,  //  003A  JMP		#003D
      0x88200105,  //  003B  GETMBR	R8	R0	K5
      0x98200D08,  //  003C  SETIDX	R8	R6	K8
      0x00180D09,  //  003D  ADD	R6	R6	K9
      0x7001FFDD,  //  003E  JMP		#001D
      0x80000000,  //  003F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_ShiftAnimation_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ShiftAnimation,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080520,  //  0003  GETMET	R2	R2	K32
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x90022507,  //  0006  SETMBR	R0	K18	K7
      0x8C08011C,  //  0007  GETMET	R2	R0	K28
      0x7C080200,  //  0008  CALL	R2	1
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: ShiftAnimation
********************************************************************/
extern const bclass be_class_Animation;
be_local_class(ShiftAnimation,
    3,
    &be_class_Animation,
    be_nested_map(11,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(current_colors, -1), be_const_var(2) },
        { be_const_key_weak(_initialize_buffers, -1), be_const_closure(class_ShiftAnimation__initialize_buffers_closure) },
        { be_const_key_weak(current_offset, 6), be_const_var(0) },
        { be_const_key_weak(tostring, 2), be_const_closure(class_ShiftAnimation_tostring_closure) },
        { be_const_key_weak(update, 10), be_const_closure(class_ShiftAnimation_update_closure) },
        { be_const_key_weak(on_param_changed, -1), be_const_closure(class_ShiftAnimation_on_param_changed_closure) },
        { be_const_key_weak(source_frame, -1), be_const_var(1) },
        { be_const_key_weak(_calculate_shift, -1), be_const_closure(class_ShiftAnimation__calculate_shift_closure) },
        { be_const_key_weak(PARAMS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(shift_speed, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(128) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(source_animation, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_nil() },
        { be_const_key_weak(type, -1), be_nested_str_weak(instance) },
    }))    ) } )) },
        { be_const_key_weak(direction, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(1) },
        { be_const_key_weak(min, -1), be_const_int(-1) },
        { be_const_key_weak(max, 1), be_const_int(1) },
    }))    ) } )) },
        { be_const_key_weak(wrap_around, 0), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_bool(1) },
        { be_const_key_weak(type, -1), be_nested_str_weak(bool) },
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(init, -1), be_const_closure(class_ShiftAnimation_init_closure) },
        { be_const_key_weak(render, -1), be_const_closure(class_ShiftAnimation_render_closure) },
    })),
    be_str_weak(ShiftAnimation)
);

/********************************************************************
** Solidified function: jitter_all
********************************************************************/
be_local_closure(jitter_all,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(jitter_animation),
    /* K2   */  be_nested_str_weak(jitter_type),
    /* K3   */  be_const_int(3),
    /* K4   */  be_nested_str_weak(position_range),
    /* K5   */  be_nested_str_weak(color_range),
    /* K6   */  be_nested_str_weak(brightness_range),
    }),
    be_str_weak(jitter_all),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x90060503,  //  0004  SETMBR	R1	K2	K3
      0x540A0031,  //  0005  LDINT	R2	50
      0x90060802,  //  0006  SETMBR	R1	K4	R2
      0x540A001D,  //  0007  LDINT	R2	30
      0x90060A02,  //  0008  SETMBR	R1	K5	R2
      0x540A0027,  //  0009  LDINT	R2	40
      0x90060C02,  //  000A  SETMBR	R1	K6	R2
      0x80040200,  //  000B  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: linear
********************************************************************/
be_local_closure(linear,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(oscillator_value),
    /* K2   */  be_nested_str_weak(form),
    /* K3   */  be_nested_str_weak(TRIANGLE),
    }),
    be_str_weak(linear),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0xB80A0000,  //  0004  GETNGBL	R2	K0
      0x88080503,  //  0005  GETMBR	R2	R2	K3
      0x90060402,  //  0006  SETMBR	R1	K2	R2
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/

// compact class 'PlasmaAnimation' ktab size: 46, total: 93 (saved 376 bytes)
static const bvalue be_ktab_class_PlasmaAnimation[46] = {
  /* K0   */  be_nested_str_weak(start),
  /* K1   */  be_nested_str_weak(color),
  /* K2   */  be_nested_str_weak(animation),
  /* K3   */  be_nested_str_weak(rich_palette),
  /* K4   */  be_nested_str_weak(engine),
  /* K5   */  be_nested_str_weak(palette),
  /* K6   */  be_nested_str_weak(PALETTE_RAINBOW),
  /* K7   */  be_nested_str_weak(cycle_period),
  /* K8   */  be_nested_str_weak(transition_type),
  /* K9   */  be_const_int(1),
  /* K10  */  be_nested_str_weak(brightness),
  /* K11  */  be_nested_str_weak(range_min),
  /* K12  */  be_const_int(0),
  /* K13  */  be_nested_str_weak(range_max),
  /* K14  */  be_nested_str_weak(time_phase),
  /* K15  */  be_nested_str_weak(is_value_provider),
  /* K16  */  be_nested_str_weak(0x_X2508x),
  /* K17  */  be_nested_str_weak(PlasmaAnimation_X28color_X3D_X25s_X2C_X20freq_x_X3D_X25s_X2C_X20freq_y_X3D_X25s_X2C_X20time_speed_X3D_X25s_X2C_X20priority_X3D_X25s_X2C_X20running_X3D_X25s_X29),
  /* K18  */  be_nested_str_weak(freq_x),
  /* K19  */  be_nested_str_weak(freq_y),
  /* K20  */  be_nested_str_weak(time_speed),
  /* K21  */  be_nested_str_weak(priority),
  /* K22  */  be_nested_str_weak(is_running),
  /* K23  */  be_nested_str_weak(init),
  /* K24  */  be_nested_str_weak(current_colors),
  /* K25  */  be_nested_str_weak(_initialize_colors),
  /* K26  */  be_nested_str_weak(update),
  /* K27  */  be_nested_str_weak(start_time),
  /* K28  */  be_nested_str_weak(tasmota),
  /* K29  */  be_nested_str_weak(scale_uint),
  /* K30  */  be_nested_str_weak(_calculate_plasma),
  /* K31  */  be_nested_str_weak(get_strip_length),
  /* K32  */  be_nested_str_weak(resize),
  /* K33  */  be_const_int(-16777216),
  /* K34  */  be_nested_str_weak(set_param),
  /* K35  */  be_nested_str_weak(phase_x),
  /* K36  */  be_nested_str_weak(phase_y),
  /* K37  */  be_nested_str_weak(blend_mode),
  /* K38  */  be_nested_str_weak(_sine),
  /* K39  */  be_const_int(2),
  /* K40  */  be_nested_str_weak(is_color_provider),
  /* K41  */  be_nested_str_weak(get_color_for_value),
  /* K42  */  be_nested_str_weak(resolve_value),
  /* K43  */  be_nested_str_weak(width),
  /* K44  */  be_nested_str_weak(set_pixel_color),
  /* K45  */  be_nested_str_weak(sine_int),
};


extern const bclass be_class_PlasmaAnimation;

/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(class_PlasmaAnimation_start,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_PlasmaAnimation,     /* shared constants */
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x88080101,  //  0006  GETMBR	R2	R0	K1
      0x4C0C0000,  //  0007  LDNIL	R3
      0x1C080403,  //  0008  EQ	R2	R2	R3
      0x780A000F,  //  0009  JMPF	R2	#001A
      0xB80A0400,  //  000A  GETNGBL	R2	K2
      0x8C080503,  //  000B  GETMET	R2	R2	K3
      0x88100104,  //  000C  GETMBR	R4	R0	K4
      0x7C080400,  //  000D  CALL	R2	2
      0xB80E0400,  //  000E  GETNGBL	R3	K2
      0x880C0706,  //  000F  GETMBR	R3	R3	K6
      0x900A0A03,  //  0010  SETMBR	R2	K5	R3
      0x540E1387,  //  0011  LDINT	R3	5000
      0x900A0E03,  //  0012  SETMBR	R2	K7	R3
      0x900A1109,  //  0013  SETMBR	R2	K8	K9
      0x540E00FE,  //  0014  LDINT	R3	255
      0x900A1403,  //  0015  SETMBR	R2	K10	R3
      0x900A170C,  //  0016  SETMBR	R2	K11	K12
      0x540E00FE,  //  0017  LDINT	R3	255
      0x900A1A03,  //  0018  SETMBR	R2	K13	R3
      0x90020202,  //  0019  SETMBR	R0	K1	R2
      0x90021D0C,  //  001A  SETMBR	R0	K14	K12
      0x80040000,  //  001B  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_PlasmaAnimation_tostring,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_PlasmaAnimation,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x4C040000,  //  0000  LDNIL	R1
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0xB80E0400,  //  0002  GETNGBL	R3	K2
      0x8C0C070F,  //  0003  GETMET	R3	R3	K15
      0x5C140400,  //  0004  MOVE	R5	R2
      0x7C0C0400,  //  0005  CALL	R3	2
      0x780E0004,  //  0006  JMPF	R3	#000C
      0x600C0008,  //  0007  GETGBL	R3	G8
      0x5C100400,  //  0008  MOVE	R4	R2
      0x7C0C0200,  //  0009  CALL	R3	1
      0x5C040600,  //  000A  MOVE	R1	R3
      0x70020004,  //  000B  JMP		#0011
      0x600C0018,  //  000C  GETGBL	R3	G24
      0x58100010,  //  000D  LDCONST	R4	K16
      0x5C140400,  //  000E  MOVE	R5	R2
      0x7C0C0400,  //  000F  CALL	R3	2
      0x5C040600,  //  0010  MOVE	R1	R3
      0x600C0018,  //  0011  GETGBL	R3	G24
      0x58100011,  //  0012  LDCONST	R4	K17
      0x5C140200,  //  0013  MOVE	R5	R1
      0x88180112,  //  0014  GETMBR	R6	R0	K18
      0x881C0113,  //  0015  GETMBR	R7	R0	K19
      0x88200114,  //  0016  GETMBR	R8	R0	K20
      0x88240115,  //  0017  GETMBR	R9	R0	K21
      0x88280116,  //  0018  GETMBR	R10	R0	K22
      0x7C0C0E00,  //  0019  CALL	R3	7
      0x80040600,  //  001A  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_PlasmaAnimation_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_PlasmaAnimation,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080517,  //  0003  GETMET	R2	R2	K23
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x90021D0C,  //  0006  SETMBR	R0	K14	K12
      0x60080012,  //  0007  GETGBL	R2	G18
      0x7C080000,  //  0008  CALL	R2	0
      0x90023002,  //  0009  SETMBR	R0	K24	R2
      0x8C080119,  //  000A  GETMET	R2	R0	K25
      0x7C080200,  //  000B  CALL	R2	1
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update
********************************************************************/
be_local_closure(class_PlasmaAnimation_update,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_PlasmaAnimation,     /* shared constants */
    be_str_weak(update),
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C08051A,  //  0003  GETMET	R2	R2	K26
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x740A0001,  //  0006  JMPT	R2	#0009
      0x50080000,  //  0007  LDBOOL	R2	0	0
      0x80040400,  //  0008  RET	1	R2
      0x88080114,  //  0009  GETMBR	R2	R0	K20
      0x240C050C,  //  000A  GT	R3	R2	K12
      0x780E0011,  //  000B  JMPF	R3	#001E
      0x880C011B,  //  000C  GETMBR	R3	R0	K27
      0x040C0203,  //  000D  SUB	R3	R1	R3
      0xB8123800,  //  000E  GETNGBL	R4	K28
      0x8C10091D,  //  000F  GETMET	R4	R4	K29
      0x5C180400,  //  0010  MOVE	R6	R2
      0x581C000C,  //  0011  LDCONST	R7	K12
      0x542200FE,  //  0012  LDINT	R8	255
      0x5824000C,  //  0013  LDCONST	R9	K12
      0x542A0007,  //  0014  LDINT	R10	8
      0x7C100C00,  //  0015  CALL	R4	6
      0x2414090C,  //  0016  GT	R5	R4	K12
      0x78160005,  //  0017  JMPF	R5	#001E
      0x08140604,  //  0018  MUL	R5	R3	R4
      0x541A03E7,  //  0019  LDINT	R6	1000
      0x0C140A06,  //  001A  DIV	R5	R5	R6
      0x541A00FF,  //  001B  LDINT	R6	256
      0x10140A06,  //  001C  MOD	R5	R5	R6
      0x90021C05,  //  001D  SETMBR	R0	K14	R5
      0x8C0C011E,  //  001E  GETMET	R3	R0	K30
      0x5C140200,  //  001F  MOVE	R5	R1
      0x7C0C0400,  //  0020  CALL	R3	2
      0x500C0200,  //  0021  LDBOOL	R3	1	0
      0x80040600,  //  0022  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _initialize_colors
********************************************************************/
be_local_closure(class_PlasmaAnimation__initialize_colors,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_PlasmaAnimation,     /* shared constants */
    be_str_weak(_initialize_colors),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x8C04031F,  //  0001  GETMET	R1	R1	K31
      0x7C040200,  //  0002  CALL	R1	1
      0x88080118,  //  0003  GETMBR	R2	R0	K24
      0x8C080520,  //  0004  GETMET	R2	R2	K32
      0x5C100200,  //  0005  MOVE	R4	R1
      0x7C080400,  //  0006  CALL	R2	2
      0x5808000C,  //  0007  LDCONST	R2	K12
      0x140C0401,  //  0008  LT	R3	R2	R1
      0x780E0003,  //  0009  JMPF	R3	#000E
      0x880C0118,  //  000A  GETMBR	R3	R0	K24
      0x980C0521,  //  000B  SETIDX	R3	R2	K33
      0x00080509,  //  000C  ADD	R2	R2	K9
      0x7001FFF9,  //  000D  JMP		#0008
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: on_param_changed
********************************************************************/
be_local_closure(class_PlasmaAnimation_on_param_changed,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_PlasmaAnimation,     /* shared constants */
    be_str_weak(on_param_changed),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x1C0C0301,  //  0000  EQ	R3	R1	K1
      0x780E0015,  //  0001  JMPF	R3	#0018
      0x4C0C0000,  //  0002  LDNIL	R3
      0x1C0C0403,  //  0003  EQ	R3	R2	R3
      0x780E0012,  //  0004  JMPF	R3	#0018
      0xB80E0400,  //  0005  GETNGBL	R3	K2
      0x8C0C0703,  //  0006  GETMET	R3	R3	K3
      0x88140104,  //  0007  GETMBR	R5	R0	K4
      0x7C0C0400,  //  0008  CALL	R3	2
      0xB8120400,  //  0009  GETNGBL	R4	K2
      0x88100906,  //  000A  GETMBR	R4	R4	K6
      0x900E0A04,  //  000B  SETMBR	R3	K5	R4
      0x54121387,  //  000C  LDINT	R4	5000
      0x900E0E04,  //  000D  SETMBR	R3	K7	R4
      0x900E1109,  //  000E  SETMBR	R3	K8	K9
      0x541200FE,  //  000F  LDINT	R4	255
      0x900E1404,  //  0010  SETMBR	R3	K10	R4
      0x900E170C,  //  0011  SETMBR	R3	K11	K12
      0x541200FE,  //  0012  LDINT	R4	255
      0x900E1A04,  //  0013  SETMBR	R3	K13	R4
      0x8C100122,  //  0014  GETMET	R4	R0	K34
      0x58180001,  //  0015  LDCONST	R6	K1
      0x5C1C0600,  //  0016  MOVE	R7	R3
      0x7C100600,  //  0017  CALL	R4	3
      0x80000000,  //  0018  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _calculate_plasma
********************************************************************/
be_local_closure(class_PlasmaAnimation__calculate_plasma,   /* name */
  be_nested_proto(
    21,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_PlasmaAnimation,     /* shared constants */
    be_str_weak(_calculate_plasma),
    &be_const_str_solidified,
    ( &(const binstruction[103]) {  /* code */
      0x88080104,  //  0000  GETMBR	R2	R0	K4
      0x8C08051F,  //  0001  GETMET	R2	R2	K31
      0x7C080200,  //  0002  CALL	R2	1
      0x600C000C,  //  0003  GETGBL	R3	G12
      0x88100118,  //  0004  GETMBR	R4	R0	K24
      0x7C0C0200,  //  0005  CALL	R3	1
      0x200C0602,  //  0006  NE	R3	R3	R2
      0x780E0001,  //  0007  JMPF	R3	#000A
      0x8C0C0119,  //  0008  GETMET	R3	R0	K25
      0x7C0C0200,  //  0009  CALL	R3	1
      0x880C0112,  //  000A  GETMBR	R3	R0	K18
      0x88100113,  //  000B  GETMBR	R4	R0	K19
      0x88140123,  //  000C  GETMBR	R5	R0	K35
      0x88180124,  //  000D  GETMBR	R6	R0	K36
      0x881C0125,  //  000E  GETMBR	R7	R0	K37
      0x88200101,  //  000F  GETMBR	R8	R0	K1
      0x5824000C,  //  0010  LDCONST	R9	K12
      0x14281202,  //  0011  LT	R10	R9	R2
      0x782A0052,  //  0012  JMPF	R10	#0066
      0xB82A3800,  //  0013  GETNGBL	R10	K28
      0x8C28151D,  //  0014  GETMET	R10	R10	K29
      0x5C301200,  //  0015  MOVE	R12	R9
      0x5834000C,  //  0016  LDCONST	R13	K12
      0x04380509,  //  0017  SUB	R14	R2	K9
      0x583C000C,  //  0018  LDCONST	R15	K12
      0x544200FE,  //  0019  LDINT	R16	255
      0x7C280C00,  //  001A  CALL	R10	6
      0x8C2C0126,  //  001B  GETMET	R11	R0	K38
      0x08341403,  //  001C  MUL	R13	R10	R3
      0x543A001F,  //  001D  LDINT	R14	32
      0x0C341A0E,  //  001E  DIV	R13	R13	R14
      0x00341A05,  //  001F  ADD	R13	R13	R5
      0x8838010E,  //  0020  GETMBR	R14	R0	K14
      0x00341A0E,  //  0021  ADD	R13	R13	R14
      0x7C2C0400,  //  0022  CALL	R11	2
      0x8C300126,  //  0023  GETMET	R12	R0	K38
      0x08381404,  //  0024  MUL	R14	R10	R4
      0x543E001F,  //  0025  LDINT	R15	32
      0x0C381C0F,  //  0026  DIV	R14	R14	R15
      0x00381C06,  //  0027  ADD	R14	R14	R6
      0x883C010E,  //  0028  GETMBR	R15	R0	K14
      0x083C1F27,  //  0029  MUL	R15	R15	K39
      0x00381C0F,  //  002A  ADD	R14	R14	R15
      0x7C300400,  //  002B  CALL	R12	2
      0x5834000C,  //  002C  LDCONST	R13	K12
      0x1C380F0C,  //  002D  EQ	R14	R7	K12
      0x783A0003,  //  002E  JMPF	R14	#0033
      0x0038160C,  //  002F  ADD	R14	R11	R12
      0x0C381D27,  //  0030  DIV	R14	R14	K39
      0x5C341C00,  //  0031  MOVE	R13	R14
      0x7002000E,  //  0032  JMP		#0042
      0x1C380F09,  //  0033  EQ	R14	R7	K9
      0x783A0009,  //  0034  JMPF	R14	#003F
      0xB83A3800,  //  0035  GETNGBL	R14	K28
      0x8C381D1D,  //  0036  GETMET	R14	R14	K29
      0x5C401600,  //  0037  MOVE	R16	R11
      0x5844000C,  //  0038  LDCONST	R17	K12
      0x544A00FE,  //  0039  LDINT	R18	255
      0x584C000C,  //  003A  LDCONST	R19	K12
      0x5C501800,  //  003B  MOVE	R20	R12
      0x7C380C00,  //  003C  CALL	R14	6
      0x5C341C00,  //  003D  MOVE	R13	R14
      0x70020002,  //  003E  JMP		#0042
      0x0038160C,  //  003F  ADD	R14	R11	R12
      0x0C381D27,  //  0040  DIV	R14	R14	K39
      0x5C341C00,  //  0041  MOVE	R13	R14
      0x543A00FE,  //  0042  LDINT	R14	255
      0x24381A0E,  //  0043  GT	R14	R13	R14
      0x783A0001,  //  0044  JMPF	R14	#0047
      0x543600FE,  //  0045  LDINT	R13	255
      0x70020002,  //  0046  JMP		#004A
      0x14381B0C,  //  0047  LT	R14	R13	K12
      0x783A0000,  //  0048  JMPF	R14	#004A
      0x5834000C,  //  0049  LDCONST	R13	K12
      0x58380021,  //  004A  LDCONST	R14	K33
      0xB83E0400,  //  004B  GETNGBL	R15	K2
      0x8C3C1F28,  //  004C  GETMET	R15	R15	K40
      0x5C441000,  //  004D  MOVE	R17	R8
      0x7C3C0400,  //  004E  CALL	R15	2
      0x783E0009,  //  004F  JMPF	R15	#005A
      0x883C1129,  //  0050  GETMBR	R15	R8	K41
      0x4C400000,  //  0051  LDNIL	R16
      0x203C1E10,  //  0052  NE	R15	R15	R16
      0x783E0005,  //  0053  JMPF	R15	#005A
      0x8C3C1129,  //  0054  GETMET	R15	R8	K41
      0x5C441A00,  //  0055  MOVE	R17	R13
      0x5848000C,  //  0056  LDCONST	R18	K12
      0x7C3C0600,  //  0057  CALL	R15	3
      0x5C381E00,  //  0058  MOVE	R14	R15
      0x70020007,  //  0059  JMP		#0062
      0x8C3C012A,  //  005A  GETMET	R15	R0	K42
      0x5C441000,  //  005B  MOVE	R17	R8
      0x58480001,  //  005C  LDCONST	R18	K1
      0x544E0009,  //  005D  LDINT	R19	10
      0x084C1A13,  //  005E  MUL	R19	R13	R19
      0x004C0213,  //  005F  ADD	R19	R1	R19
      0x7C3C0800,  //  0060  CALL	R15	4
      0x5C381E00,  //  0061  MOVE	R14	R15
      0x883C0118,  //  0062  GETMBR	R15	R0	K24
      0x983C120E,  //  0063  SETIDX	R15	R9	R14
      0x00241309,  //  0064  ADD	R9	R9	K9
      0x7001FFAA,  //  0065  JMP		#0011
      0x80000000,  //  0066  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: render
********************************************************************/
be_local_closure(class_PlasmaAnimation_render,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_PlasmaAnimation,     /* shared constants */
    be_str_weak(render),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x880C0116,  //  0000  GETMBR	R3	R0	K22
      0x780E0002,  //  0001  JMPF	R3	#0005
      0x4C0C0000,  //  0002  LDNIL	R3
      0x1C0C0203,  //  0003  EQ	R3	R1	R3
      0x780E0001,  //  0004  JMPF	R3	#0007
      0x500C0000,  //  0005  LDBOOL	R3	0	0
      0x80040600,  //  0006  RET	1	R3
      0x880C0104,  //  0007  GETMBR	R3	R0	K4
      0x8C0C071F,  //  0008  GETMET	R3	R3	K31
      0x7C0C0200,  //  0009  CALL	R3	1
      0x5810000C,  //  000A  LDCONST	R4	K12
      0x14140803,  //  000B  LT	R5	R4	R3
      0x78160009,  //  000C  JMPF	R5	#0017
      0x8814032B,  //  000D  GETMBR	R5	R1	K43
      0x14140805,  //  000E  LT	R5	R4	R5
      0x78160004,  //  000F  JMPF	R5	#0015
      0x8C14032C,  //  0010  GETMET	R5	R1	K44
      0x5C1C0800,  //  0011  MOVE	R7	R4
      0x88200118,  //  0012  GETMBR	R8	R0	K24
      0x94201004,  //  0013  GETIDX	R8	R8	R4
      0x7C140600,  //  0014  CALL	R5	3
      0x00100909,  //  0015  ADD	R4	R4	K9
      0x7001FFF3,  //  0016  JMP		#000B
      0x50140200,  //  0017  LDBOOL	R5	1	0
      0x80040A00,  //  0018  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _sine
********************************************************************/
be_local_closure(class_PlasmaAnimation__sine,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_PlasmaAnimation,     /* shared constants */
    be_str_weak(_sine),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0xB80A3800,  //  0000  GETNGBL	R2	K28
      0x8C08051D,  //  0001  GETMET	R2	R2	K29
      0x5C100200,  //  0002  MOVE	R4	R1
      0x5814000C,  //  0003  LDCONST	R5	K12
      0x541A00FE,  //  0004  LDINT	R6	255
      0x581C000C,  //  0005  LDCONST	R7	K12
      0x54227FFE,  //  0006  LDINT	R8	32767
      0x7C080C00,  //  0007  CALL	R2	6
      0xB80E3800,  //  0008  GETNGBL	R3	K28
      0x8C0C072D,  //  0009  GETMET	R3	R3	K45
      0x5C140400,  //  000A  MOVE	R5	R2
      0x7C0C0400,  //  000B  CALL	R3	2
      0xB8123800,  //  000C  GETNGBL	R4	K28
      0x8C10091D,  //  000D  GETMET	R4	R4	K29
      0x5C180600,  //  000E  MOVE	R6	R3
      0x541DEFFF,  //  000F  LDINT	R7	-4096
      0x54220FFF,  //  0010  LDINT	R8	4096
      0x5824000C,  //  0011  LDCONST	R9	K12
      0x542A00FE,  //  0012  LDINT	R10	255
      0x7C100C00,  //  0013  CALL	R4	6
      0x80040800,  //  0014  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: PlasmaAnimation
********************************************************************/
extern const bclass be_class_Animation;
be_local_class(PlasmaAnimation,
    2,
    &be_class_Animation,
    be_nested_map(12,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(_sine, -1), be_const_closure(class_PlasmaAnimation__sine_closure) },
        { be_const_key_weak(tostring, 2), be_const_closure(class_PlasmaAnimation_tostring_closure) },
        { be_const_key_weak(render, 5), be_const_closure(class_PlasmaAnimation_render_closure) },
        { be_const_key_weak(init, 9), be_const_closure(class_PlasmaAnimation_init_closure) },
        { be_const_key_weak(update, -1), be_const_closure(class_PlasmaAnimation_update_closure) },
        { be_const_key_weak(_calculate_plasma, 7), be_const_closure(class_PlasmaAnimation__calculate_plasma_closure) },
        { be_const_key_weak(_initialize_colors, -1), be_const_closure(class_PlasmaAnimation__initialize_colors_closure) },
        { be_const_key_weak(on_param_changed, 6), be_const_closure(class_PlasmaAnimation_on_param_changed_closure) },
        { be_const_key_weak(time_phase, -1), be_const_var(1) },
        { be_const_key_weak(current_colors, 10), be_const_var(0) },
        { be_const_key_weak(PARAMS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(phase_y, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(64) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(freq_x, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(32) },
        { be_const_key_weak(min, -1), be_const_int(1) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(blend_mode, 4), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(0) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(time_speed, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(50) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(color, 3), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_nil() },
    }))    ) } )) },
        { be_const_key_weak(phase_x, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(0) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(freq_y, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(23) },
        { be_const_key_weak(min, -1), be_const_int(1) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(start, 0), be_const_closure(class_PlasmaAnimation_start_closure) },
    })),
    be_str_weak(PlasmaAnimation)
);

extern const bclass be_class_ColorProvider;

/********************************************************************
** Solidified function: produce_value
********************************************************************/
be_local_closure(class_ColorProvider_produce_value,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(produce_value),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x540DFFFE,  //  0000  LDINT	R3	-1
      0x80040600,  //  0001  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_color_for_value
********************************************************************/
be_local_closure(class_ColorProvider_get_color_for_value,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(produce_value),
    /* K1   */  be_nested_str_weak(color),
    }),
    be_str_weak(get_color_for_value),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C0C0100,  //  0000  GETMET	R3	R0	K0
      0x58140001,  //  0001  LDCONST	R5	K1
      0x5C180400,  //  0002  MOVE	R6	R2
      0x7C0C0600,  //  0003  CALL	R3	3
      0x80040600,  //  0004  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: ColorProvider
********************************************************************/
extern const bclass be_class_ValueProvider;
be_local_class(ColorProvider,
    0,
    &be_class_ValueProvider,
    be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(get_color_for_value, -1), be_const_closure(class_ColorProvider_get_color_for_value_closure) },
        { be_const_key_weak(produce_value, 0), be_const_closure(class_ColorProvider_produce_value_closure) },
    })),
    be_str_weak(ColorProvider)
);

/********************************************************************
** Solidified function: register_event_handler
********************************************************************/
be_local_closure(register_event_handler,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    5,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(event_manager),
    /* K2   */  be_nested_str_weak(register_handler),
    }),
    be_str_weak(register_event_handler),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0xB8160000,  //  0000  GETNGBL	R5	K0
      0x88140B01,  //  0001  GETMBR	R5	R5	K1
      0x8C140B02,  //  0002  GETMET	R5	R5	K2
      0x5C1C0000,  //  0003  MOVE	R7	R0
      0x5C200200,  //  0004  MOVE	R8	R1
      0x5C240400,  //  0005  MOVE	R9	R2
      0x5C280600,  //  0006  MOVE	R10	R3
      0x5C2C0800,  //  0007  MOVE	R11	R4
      0x7C140C00,  //  0008  CALL	R5	6
      0x80040A00,  //  0009  RET	1	R5
    })
  )
);
/*******************************************************************/

// compact class 'StaticValueProvider' ktab size: 3, total: 10 (saved 56 bytes)
static const bvalue be_ktab_class_StaticValueProvider[3] = {
  /* K0   */  be_nested_str_weak(value),
  /* K1   */  be_nested_str_weak(StaticValueProvider_X28value_X3D_X25s_X29),
  /* K2   */  be_nested_str_weak(StaticValueProvider_X28value_X3Dunset_X29),
};


extern const bclass be_class_StaticValueProvider;

/********************************************************************
** Solidified function: <=
********************************************************************/
be_local_closure(class_StaticValueProvider__X3C_X3D,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_StaticValueProvider,     /* shared constants */
    be_str_weak(_X3C_X3D),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x600C0009,  //  0001  GETGBL	R3	G9
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x18080403,  //  0004  LE	R2	R2	R3
      0x80040400,  //  0005  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: >
********************************************************************/
be_local_closure(class_StaticValueProvider__X3E,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_StaticValueProvider,     /* shared constants */
    be_str_weak(_X3E),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x600C0009,  //  0001  GETGBL	R3	G9
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x24080403,  //  0004  GT	R2	R2	R3
      0x80040400,  //  0005  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: >=
********************************************************************/
be_local_closure(class_StaticValueProvider__X3E_X3D,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_StaticValueProvider,     /* shared constants */
    be_str_weak(_X3E_X3D),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x600C0009,  //  0001  GETGBL	R3	G9
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x28080403,  //  0004  GE	R2	R2	R3
      0x80040400,  //  0005  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: produce_value
********************************************************************/
be_local_closure(class_StaticValueProvider_produce_value,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_StaticValueProvider,     /* shared constants */
    be_str_weak(produce_value),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x80040600,  //  0001  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: !=
********************************************************************/
be_local_closure(class_StaticValueProvider__X21_X3D,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_StaticValueProvider,     /* shared constants */
    be_str_weak(_X21_X3D),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x600C0009,  //  0001  GETGBL	R3	G9
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x20080403,  //  0004  NE	R2	R2	R3
      0x80040400,  //  0005  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: ==
********************************************************************/
be_local_closure(class_StaticValueProvider__X3D_X3D,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_StaticValueProvider,     /* shared constants */
    be_str_weak(_X3D_X3D),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x600C0009,  //  0001  GETGBL	R3	G9
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x1C080403,  //  0004  EQ	R2	R2	R3
      0x80040400,  //  0005  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_StaticValueProvider_tostring,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_StaticValueProvider,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0xA8020007,  //  0000  EXBLK	0	#0009
      0x60040018,  //  0001  GETGBL	R1	G24
      0x58080001,  //  0002  LDCONST	R2	K1
      0x880C0100,  //  0003  GETMBR	R3	R0	K0
      0x7C040400,  //  0004  CALL	R1	2
      0xA8040001,  //  0005  EXBLK	1	1
      0x80040200,  //  0006  RET	1	R1
      0xA8040001,  //  0007  EXBLK	1	1
      0x70020004,  //  0008  JMP		#000E
      0xAC040000,  //  0009  CATCH	R1	0	0
      0x70020001,  //  000A  JMP		#000D
      0x80060400,  //  000B  RET	1	K2
      0x70020000,  //  000C  JMP		#000E
      0xB0080000,  //  000D  RAISE	2	R0	R0
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: <
********************************************************************/
be_local_closure(class_StaticValueProvider__X3C,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_StaticValueProvider,     /* shared constants */
    be_str_weak(_X3C),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x600C0009,  //  0001  GETGBL	R3	G9
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x14080403,  //  0004  LT	R2	R2	R3
      0x80040400,  //  0005  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: StaticValueProvider
********************************************************************/
extern const bclass be_class_ValueProvider;
be_local_class(StaticValueProvider,
    0,
    &be_class_ValueProvider,
    be_nested_map(9,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(_X3C_X3D, -1), be_const_closure(class_StaticValueProvider__X3C_X3D_closure) },
        { be_const_key_weak(_X3D_X3D, -1), be_const_closure(class_StaticValueProvider__X3D_X3D_closure) },
        { be_const_key_weak(PARAMS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(value, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_nil() },
        { be_const_key_weak(type, -1), be_nested_str_weak(any) },
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(produce_value, -1), be_const_closure(class_StaticValueProvider_produce_value_closure) },
        { be_const_key_weak(_X21_X3D, -1), be_const_closure(class_StaticValueProvider__X21_X3D_closure) },
        { be_const_key_weak(_X3E_X3D, 1), be_const_closure(class_StaticValueProvider__X3E_X3D_closure) },
        { be_const_key_weak(_X3E, 2), be_const_closure(class_StaticValueProvider__X3E_closure) },
        { be_const_key_weak(tostring, -1), be_const_closure(class_StaticValueProvider_tostring_closure) },
        { be_const_key_weak(_X3C, -1), be_const_closure(class_StaticValueProvider__X3C_closure) },
    })),
    be_str_weak(StaticValueProvider)
);

/********************************************************************
** Solidified function: square
********************************************************************/
be_local_closure(square,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(oscillator_value),
    /* K2   */  be_nested_str_weak(form),
    /* K3   */  be_nested_str_weak(SQUARE),
    }),
    be_str_weak(square),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0xB80A0000,  //  0004  GETNGBL	R2	K0
      0x88080503,  //  0005  GETMBR	R2	R2	K3
      0x90060402,  //  0006  SETMBR	R1	K2	R2
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: sawtooth
********************************************************************/
be_local_closure(sawtooth,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(oscillator_value),
    /* K2   */  be_nested_str_weak(form),
    /* K3   */  be_nested_str_weak(SAWTOOTH),
    }),
    be_str_weak(sawtooth),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0xB80A0000,  //  0004  GETNGBL	R2	K0
      0x88080503,  //  0005  GETMBR	R2	R2	K3
      0x90060402,  //  0006  SETMBR	R1	K2	R2
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/

// compact class 'NoiseAnimation' ktab size: 50, total: 107 (saved 456 bytes)
static const bvalue be_ktab_class_NoiseAnimation[50] = {
  /* K0   */  be_nested_str_weak(init),
  /* K1   */  be_nested_str_weak(engine),
  /* K2   */  be_nested_str_weak(get_strip_length),
  /* K3   */  be_nested_str_weak(current_colors),
  /* K4   */  be_nested_str_weak(resize),
  /* K5   */  be_nested_str_weak(time_offset),
  /* K6   */  be_const_int(0),
  /* K7   */  be_const_int(-16777216),
  /* K8   */  be_const_int(1),
  /* K9   */  be_nested_str_weak(noise_table),
  /* K10  */  be_nested_str_weak(color),
  /* K11  */  be_nested_str_weak(animation),
  /* K12  */  be_nested_str_weak(rich_palette),
  /* K13  */  be_nested_str_weak(palette),
  /* K14  */  be_nested_str_weak(PALETTE_RAINBOW),
  /* K15  */  be_nested_str_weak(cycle_period),
  /* K16  */  be_nested_str_weak(transition_type),
  /* K17  */  be_nested_str_weak(brightness),
  /* K18  */  be_nested_str_weak(range_min),
  /* K19  */  be_nested_str_weak(range_max),
  /* K20  */  be_nested_str_weak(int),
  /* K21  */  be_nested_str_weak(add),
  /* K22  */  be_nested_str_weak(setmember),
  /* K23  */  be_nested_str_weak(seed),
  /* K24  */  be_const_int(1103515245),
  /* K25  */  be_const_int(2147483647),
  /* K26  */  be_nested_str_weak(update),
  /* K27  */  be_nested_str_weak(speed),
  /* K28  */  be_nested_str_weak(start_time),
  /* K29  */  be_nested_str_weak(tasmota),
  /* K30  */  be_nested_str_weak(scale_uint),
  /* K31  */  be_nested_str_weak(_calculate_noise),
  /* K32  */  be_nested_str_weak(_fractal_noise),
  /* K33  */  be_nested_str_weak(is_color_provider),
  /* K34  */  be_nested_str_weak(get_color_for_value),
  /* K35  */  be_nested_str_weak(resolve_value),
  /* K36  */  be_nested_str_weak(_init_noise_table),
  /* K37  */  be_nested_str_weak(is_running),
  /* K38  */  be_nested_str_weak(width),
  /* K39  */  be_nested_str_weak(set_pixel_color),
  /* K40  */  be_nested_str_weak(is_value_provider),
  /* K41  */  be_nested_str_weak(0x_X2508x),
  /* K42  */  be_nested_str_weak(NoiseAnimation_X28color_X3D_X25s_X2C_X20scale_X3D_X25s_X2C_X20speed_X3D_X25s_X2C_X20octaves_X3D_X25s_X2C_X20priority_X3D_X25s_X2C_X20running_X3D_X25s_X29),
  /* K43  */  be_nested_str_weak(scale),
  /* K44  */  be_nested_str_weak(octaves),
  /* K45  */  be_nested_str_weak(priority),
  /* K46  */  be_nested_str_weak(start),
  /* K47  */  be_nested_str_weak(persistence),
  /* K48  */  be_nested_str_weak(_noise_1d),
  /* K49  */  be_const_int(2),
};


extern const bclass be_class_NoiseAnimation;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_NoiseAnimation_init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_NoiseAnimation,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[48]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x88080101,  //  0006  GETMBR	R2	R0	K1
      0x8C080502,  //  0007  GETMET	R2	R2	K2
      0x7C080200,  //  0008  CALL	R2	1
      0x600C0012,  //  0009  GETGBL	R3	G18
      0x7C0C0000,  //  000A  CALL	R3	0
      0x90020603,  //  000B  SETMBR	R0	K3	R3
      0x880C0103,  //  000C  GETMBR	R3	R0	K3
      0x8C0C0704,  //  000D  GETMET	R3	R3	K4
      0x5C140400,  //  000E  MOVE	R5	R2
      0x7C0C0400,  //  000F  CALL	R3	2
      0x90020B06,  //  0010  SETMBR	R0	K5	K6
      0x580C0006,  //  0011  LDCONST	R3	K6
      0x14100602,  //  0012  LT	R4	R3	R2
      0x78120003,  //  0013  JMPF	R4	#0018
      0x88100103,  //  0014  GETMBR	R4	R0	K3
      0x98100707,  //  0015  SETIDX	R4	R3	K7
      0x000C0708,  //  0016  ADD	R3	R3	K8
      0x7001FFF9,  //  0017  JMP		#0012
      0x60100012,  //  0018  GETGBL	R4	G18
      0x7C100000,  //  0019  CALL	R4	0
      0x90021204,  //  001A  SETMBR	R0	K9	R4
      0x8810010A,  //  001B  GETMBR	R4	R0	K10
      0x4C140000,  //  001C  LDNIL	R5
      0x1C100805,  //  001D  EQ	R4	R4	R5
      0x7812000F,  //  001E  JMPF	R4	#002F
      0xB8121600,  //  001F  GETNGBL	R4	K11
      0x8C10090C,  //  0020  GETMET	R4	R4	K12
      0x5C180200,  //  0021  MOVE	R6	R1
      0x7C100400,  //  0022  CALL	R4	2
      0xB8161600,  //  0023  GETNGBL	R5	K11
      0x88140B0E,  //  0024  GETMBR	R5	R5	K14
      0x90121A05,  //  0025  SETMBR	R4	K13	R5
      0x54161387,  //  0026  LDINT	R5	5000
      0x90121E05,  //  0027  SETMBR	R4	K15	R5
      0x90122108,  //  0028  SETMBR	R4	K16	K8
      0x541600FE,  //  0029  LDINT	R5	255
      0x90122205,  //  002A  SETMBR	R4	K17	R5
      0x90122506,  //  002B  SETMBR	R4	K18	K6
      0x541600FE,  //  002C  LDINT	R5	255
      0x90122605,  //  002D  SETMBR	R4	K19	R5
      0x90021404,  //  002E  SETMBR	R0	K10	R4
      0x80000000,  //  002F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: setmember
********************************************************************/
be_local_closure(class_NoiseAnimation_setmember,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_NoiseAnimation,     /* shared constants */
    be_str_weak(setmember),
    &be_const_str_solidified,
    ( &(const binstruction[77]) {  /* code */
      0x1C0C030A,  //  0000  EQ	R3	R1	K10
      0x780E0042,  //  0001  JMPF	R3	#0045
      0x600C0004,  //  0002  GETGBL	R3	G4
      0x5C100400,  //  0003  MOVE	R4	R2
      0x7C0C0200,  //  0004  CALL	R3	1
      0x1C0C0714,  //  0005  EQ	R3	R3	K20
      0x780E003D,  //  0006  JMPF	R3	#0045
      0x600C0015,  //  0007  GETGBL	R3	G21
      0x7C0C0000,  //  0008  CALL	R3	0
      0x8C100715,  //  0009  GETMET	R4	R3	K21
      0x58180006,  //  000A  LDCONST	R6	K6
      0x581C0008,  //  000B  LDCONST	R7	K8
      0x7C100600,  //  000C  CALL	R4	3
      0x8C100715,  //  000D  GETMET	R4	R3	K21
      0x58180006,  //  000E  LDCONST	R6	K6
      0x581C0008,  //  000F  LDCONST	R7	K8
      0x7C100600,  //  0010  CALL	R4	3
      0x8C100715,  //  0011  GETMET	R4	R3	K21
      0x58180006,  //  0012  LDCONST	R6	K6
      0x581C0008,  //  0013  LDCONST	R7	K8
      0x7C100600,  //  0014  CALL	R4	3
      0x8C100715,  //  0015  GETMET	R4	R3	K21
      0x58180006,  //  0016  LDCONST	R6	K6
      0x581C0008,  //  0017  LDCONST	R7	K8
      0x7C100600,  //  0018  CALL	R4	3
      0x8C100715,  //  0019  GETMET	R4	R3	K21
      0x541A00FE,  //  001A  LDINT	R6	255
      0x581C0008,  //  001B  LDCONST	R7	K8
      0x7C100600,  //  001C  CALL	R4	3
      0x8C100715,  //  001D  GETMET	R4	R3	K21
      0x541A000F,  //  001E  LDINT	R6	16
      0x3C180406,  //  001F  SHR	R6	R2	R6
      0x541E00FE,  //  0020  LDINT	R7	255
      0x2C180C07,  //  0021  AND	R6	R6	R7
      0x581C0008,  //  0022  LDCONST	R7	K8
      0x7C100600,  //  0023  CALL	R4	3
      0x8C100715,  //  0024  GETMET	R4	R3	K21
      0x541A0007,  //  0025  LDINT	R6	8
      0x3C180406,  //  0026  SHR	R6	R2	R6
      0x541E00FE,  //  0027  LDINT	R7	255
      0x2C180C07,  //  0028  AND	R6	R6	R7
      0x581C0008,  //  0029  LDCONST	R7	K8
      0x7C100600,  //  002A  CALL	R4	3
      0x8C100715,  //  002B  GETMET	R4	R3	K21
      0x541A00FE,  //  002C  LDINT	R6	255
      0x2C180406,  //  002D  AND	R6	R2	R6
      0x581C0008,  //  002E  LDCONST	R7	K8
      0x7C100600,  //  002F  CALL	R4	3
      0xB8121600,  //  0030  GETNGBL	R4	K11
      0x8C10090C,  //  0031  GETMET	R4	R4	K12
      0x88180101,  //  0032  GETMBR	R6	R0	K1
      0x7C100400,  //  0033  CALL	R4	2
      0x90121A03,  //  0034  SETMBR	R4	K13	R3
      0x54161387,  //  0035  LDINT	R5	5000
      0x90121E05,  //  0036  SETMBR	R4	K15	R5
      0x90122108,  //  0037  SETMBR	R4	K16	K8
      0x541600FE,  //  0038  LDINT	R5	255
      0x90122205,  //  0039  SETMBR	R4	K17	R5
      0x90122506,  //  003A  SETMBR	R4	K18	K6
      0x541600FE,  //  003B  LDINT	R5	255
      0x90122605,  //  003C  SETMBR	R4	K19	R5
      0x60140003,  //  003D  GETGBL	R5	G3
      0x5C180000,  //  003E  MOVE	R6	R0
      0x7C140200,  //  003F  CALL	R5	1
      0x8C140B16,  //  0040  GETMET	R5	R5	K22
      0x5C1C0200,  //  0041  MOVE	R7	R1
      0x5C200800,  //  0042  MOVE	R8	R4
      0x7C140600,  //  0043  CALL	R5	3
      0x70020006,  //  0044  JMP		#004C
      0x600C0003,  //  0045  GETGBL	R3	G3
      0x5C100000,  //  0046  MOVE	R4	R0
      0x7C0C0200,  //  0047  CALL	R3	1
      0x8C0C0716,  //  0048  GETMET	R3	R3	K22
      0x5C140200,  //  0049  MOVE	R5	R1
      0x5C180400,  //  004A  MOVE	R6	R2
      0x7C0C0600,  //  004B  CALL	R3	3
      0x80000000,  //  004C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _init_noise_table
********************************************************************/
be_local_closure(class_NoiseAnimation__init_noise_table,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_NoiseAnimation,     /* shared constants */
    be_str_weak(_init_noise_table),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x60040012,  //  0000  GETGBL	R1	G18
      0x7C040000,  //  0001  CALL	R1	0
      0x90021201,  //  0002  SETMBR	R0	K9	R1
      0x88040109,  //  0003  GETMBR	R1	R0	K9
      0x8C040304,  //  0004  GETMET	R1	R1	K4
      0x540E00FF,  //  0005  LDINT	R3	256
      0x7C040400,  //  0006  CALL	R1	2
      0x88040117,  //  0007  GETMBR	R1	R0	K23
      0x5C080200,  //  0008  MOVE	R2	R1
      0x580C0006,  //  0009  LDCONST	R3	K6
      0x541200FF,  //  000A  LDINT	R4	256
      0x14100604,  //  000B  LT	R4	R3	R4
      0x7812000A,  //  000C  JMPF	R4	#0018
      0x08100518,  //  000D  MUL	R4	R2	K24
      0x54163038,  //  000E  LDINT	R5	12345
      0x00100805,  //  000F  ADD	R4	R4	R5
      0x2C100919,  //  0010  AND	R4	R4	K25
      0x5C080800,  //  0011  MOVE	R2	R4
      0x88100109,  //  0012  GETMBR	R4	R0	K9
      0x541600FF,  //  0013  LDINT	R5	256
      0x10140405,  //  0014  MOD	R5	R2	R5
      0x98100605,  //  0015  SETIDX	R4	R3	R5
      0x000C0708,  //  0016  ADD	R3	R3	K8
      0x7001FFF1,  //  0017  JMP		#000A
      0x80000000,  //  0018  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update
********************************************************************/
be_local_closure(class_NoiseAnimation_update,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_NoiseAnimation,     /* shared constants */
    be_str_weak(update),
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C08051A,  //  0003  GETMET	R2	R2	K26
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x740A0001,  //  0006  JMPT	R2	#0009
      0x50080000,  //  0007  LDBOOL	R2	0	0
      0x80040400,  //  0008  RET	1	R2
      0x8808011B,  //  0009  GETMBR	R2	R0	K27
      0x240C0506,  //  000A  GT	R3	R2	K6
      0x780E0011,  //  000B  JMPF	R3	#001E
      0x880C011C,  //  000C  GETMBR	R3	R0	K28
      0x040C0203,  //  000D  SUB	R3	R1	R3
      0xB8123A00,  //  000E  GETNGBL	R4	K29
      0x8C10091E,  //  000F  GETMET	R4	R4	K30
      0x5C180400,  //  0010  MOVE	R6	R2
      0x581C0006,  //  0011  LDCONST	R7	K6
      0x542200FE,  //  0012  LDINT	R8	255
      0x58240006,  //  0013  LDCONST	R9	K6
      0x542A0004,  //  0014  LDINT	R10	5
      0x7C100C00,  //  0015  CALL	R4	6
      0x24140906,  //  0016  GT	R5	R4	K6
      0x78160005,  //  0017  JMPF	R5	#001E
      0x08140604,  //  0018  MUL	R5	R3	R4
      0x541A03E7,  //  0019  LDINT	R6	1000
      0x0C140A06,  //  001A  DIV	R5	R5	R6
      0x541A00FF,  //  001B  LDINT	R6	256
      0x10140A06,  //  001C  MOD	R5	R5	R6
      0x90020A05,  //  001D  SETMBR	R0	K5	R5
      0x8C0C011F,  //  001E  GETMET	R3	R0	K31
      0x5C140200,  //  001F  MOVE	R5	R1
      0x7C0C0400,  //  0020  CALL	R3	2
      0x500C0200,  //  0021  LDBOOL	R3	1	0
      0x80040600,  //  0022  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _calculate_noise
********************************************************************/
be_local_closure(class_NoiseAnimation__calculate_noise,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_NoiseAnimation,     /* shared constants */
    be_str_weak(_calculate_noise),
    &be_const_str_solidified,
    ( &(const binstruction[40]) {  /* code */
      0x88080101,  //  0000  GETMBR	R2	R0	K1
      0x8C080502,  //  0001  GETMET	R2	R2	K2
      0x7C080200,  //  0002  CALL	R2	1
      0x880C010A,  //  0003  GETMBR	R3	R0	K10
      0x58100006,  //  0004  LDCONST	R4	K6
      0x14140802,  //  0005  LT	R5	R4	R2
      0x7816001F,  //  0006  JMPF	R5	#0027
      0x8C140120,  //  0007  GETMET	R5	R0	K32
      0x5C1C0800,  //  0008  MOVE	R7	R4
      0x88200105,  //  0009  GETMBR	R8	R0	K5
      0x7C140600,  //  000A  CALL	R5	3
      0x58180007,  //  000B  LDCONST	R6	K7
      0xB81E1600,  //  000C  GETNGBL	R7	K11
      0x8C1C0F21,  //  000D  GETMET	R7	R7	K33
      0x5C240600,  //  000E  MOVE	R9	R3
      0x7C1C0400,  //  000F  CALL	R7	2
      0x781E0009,  //  0010  JMPF	R7	#001B
      0x881C0722,  //  0011  GETMBR	R7	R3	K34
      0x4C200000,  //  0012  LDNIL	R8
      0x201C0E08,  //  0013  NE	R7	R7	R8
      0x781E0005,  //  0014  JMPF	R7	#001B
      0x8C1C0722,  //  0015  GETMET	R7	R3	K34
      0x5C240A00,  //  0016  MOVE	R9	R5
      0x58280006,  //  0017  LDCONST	R10	K6
      0x7C1C0600,  //  0018  CALL	R7	3
      0x5C180E00,  //  0019  MOVE	R6	R7
      0x70020007,  //  001A  JMP		#0023
      0x8C1C0123,  //  001B  GETMET	R7	R0	K35
      0x5C240600,  //  001C  MOVE	R9	R3
      0x5828000A,  //  001D  LDCONST	R10	K10
      0x542E0009,  //  001E  LDINT	R11	10
      0x082C0A0B,  //  001F  MUL	R11	R5	R11
      0x002C020B,  //  0020  ADD	R11	R1	R11
      0x7C1C0800,  //  0021  CALL	R7	4
      0x5C180E00,  //  0022  MOVE	R6	R7
      0x881C0103,  //  0023  GETMBR	R7	R0	K3
      0x981C0806,  //  0024  SETIDX	R7	R4	R6
      0x00100908,  //  0025  ADD	R4	R4	K8
      0x7001FFDD,  //  0026  JMP		#0005
      0x80000000,  //  0027  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: on_param_changed
********************************************************************/
be_local_closure(class_NoiseAnimation_on_param_changed,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_NoiseAnimation,     /* shared constants */
    be_str_weak(on_param_changed),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x1C0C0317,  //  0000  EQ	R3	R1	K23
      0x780E0001,  //  0001  JMPF	R3	#0004
      0x8C0C0124,  //  0002  GETMET	R3	R0	K36
      0x7C0C0200,  //  0003  CALL	R3	1
      0x880C0101,  //  0004  GETMBR	R3	R0	K1
      0x8C0C0702,  //  0005  GETMET	R3	R3	K2
      0x7C0C0200,  //  0006  CALL	R3	1
      0x6010000C,  //  0007  GETGBL	R4	G12
      0x88140103,  //  0008  GETMBR	R5	R0	K3
      0x7C100200,  //  0009  CALL	R4	1
      0x20100803,  //  000A  NE	R4	R4	R3
      0x7812000C,  //  000B  JMPF	R4	#0019
      0x88100103,  //  000C  GETMBR	R4	R0	K3
      0x8C100904,  //  000D  GETMET	R4	R4	K4
      0x5C180600,  //  000E  MOVE	R6	R3
      0x7C100400,  //  000F  CALL	R4	2
      0x6010000C,  //  0010  GETGBL	R4	G12
      0x88140103,  //  0011  GETMBR	R5	R0	K3
      0x7C100200,  //  0012  CALL	R4	1
      0x14140803,  //  0013  LT	R5	R4	R3
      0x78160003,  //  0014  JMPF	R5	#0019
      0x88140103,  //  0015  GETMBR	R5	R0	K3
      0x98140907,  //  0016  SETIDX	R5	R4	K7
      0x00100908,  //  0017  ADD	R4	R4	K8
      0x7001FFF9,  //  0018  JMP		#0013
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: render
********************************************************************/
be_local_closure(class_NoiseAnimation_render,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_NoiseAnimation,     /* shared constants */
    be_str_weak(render),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x880C0125,  //  0000  GETMBR	R3	R0	K37
      0x780E0002,  //  0001  JMPF	R3	#0005
      0x4C0C0000,  //  0002  LDNIL	R3
      0x1C0C0203,  //  0003  EQ	R3	R1	R3
      0x780E0001,  //  0004  JMPF	R3	#0007
      0x500C0000,  //  0005  LDBOOL	R3	0	0
      0x80040600,  //  0006  RET	1	R3
      0x880C0101,  //  0007  GETMBR	R3	R0	K1
      0x8C0C0702,  //  0008  GETMET	R3	R3	K2
      0x7C0C0200,  //  0009  CALL	R3	1
      0x58100006,  //  000A  LDCONST	R4	K6
      0x14140803,  //  000B  LT	R5	R4	R3
      0x78160009,  //  000C  JMPF	R5	#0017
      0x88140326,  //  000D  GETMBR	R5	R1	K38
      0x14140805,  //  000E  LT	R5	R4	R5
      0x78160004,  //  000F  JMPF	R5	#0015
      0x8C140327,  //  0010  GETMET	R5	R1	K39
      0x5C1C0800,  //  0011  MOVE	R7	R4
      0x88200103,  //  0012  GETMBR	R8	R0	K3
      0x94201004,  //  0013  GETIDX	R8	R8	R4
      0x7C140600,  //  0014  CALL	R5	3
      0x00100908,  //  0015  ADD	R4	R4	K8
      0x7001FFF3,  //  0016  JMP		#000B
      0x50140200,  //  0017  LDBOOL	R5	1	0
      0x80040A00,  //  0018  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_NoiseAnimation_tostring,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_NoiseAnimation,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x8804010A,  //  0000  GETMBR	R1	R0	K10
      0x4C080000,  //  0001  LDNIL	R2
      0xB80E1600,  //  0002  GETNGBL	R3	K11
      0x8C0C0728,  //  0003  GETMET	R3	R3	K40
      0x5C140200,  //  0004  MOVE	R5	R1
      0x7C0C0400,  //  0005  CALL	R3	2
      0x780E0004,  //  0006  JMPF	R3	#000C
      0x600C0008,  //  0007  GETGBL	R3	G8
      0x5C100200,  //  0008  MOVE	R4	R1
      0x7C0C0200,  //  0009  CALL	R3	1
      0x5C080600,  //  000A  MOVE	R2	R3
      0x70020004,  //  000B  JMP		#0011
      0x600C0018,  //  000C  GETGBL	R3	G24
      0x58100029,  //  000D  LDCONST	R4	K41
      0x5C140200,  //  000E  MOVE	R5	R1
      0x7C0C0400,  //  000F  CALL	R3	2
      0x5C080600,  //  0010  MOVE	R2	R3
      0x600C0018,  //  0011  GETGBL	R3	G24
      0x5810002A,  //  0012  LDCONST	R4	K42
      0x5C140400,  //  0013  MOVE	R5	R2
      0x8818012B,  //  0014  GETMBR	R6	R0	K43
      0x881C011B,  //  0015  GETMBR	R7	R0	K27
      0x8820012C,  //  0016  GETMBR	R8	R0	K44
      0x8824012D,  //  0017  GETMBR	R9	R0	K45
      0x88280125,  //  0018  GETMBR	R10	R0	K37
      0x7C0C0E00,  //  0019  CALL	R3	7
      0x80040600,  //  001A  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(class_NoiseAnimation_start,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_NoiseAnimation,     /* shared constants */
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C08052E,  //  0003  GETMET	R2	R2	K46
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x8C080124,  //  0006  GETMET	R2	R0	K36
      0x7C080200,  //  0007  CALL	R2	1
      0x90020B06,  //  0008  SETMBR	R0	K5	K6
      0x80040000,  //  0009  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _noise_1d
********************************************************************/
be_local_closure(class_NoiseAnimation__noise_1d,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_NoiseAnimation,     /* shared constants */
    be_str_weak(_noise_1d),
    &be_const_str_solidified,
    ( &(const binstruction[36]) {  /* code */
      0x60080009,  //  0000  GETGBL	R2	G9
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x540E00FE,  //  0003  LDINT	R3	255
      0x2C080403,  //  0004  AND	R2	R2	R3
      0x600C0009,  //  0005  GETGBL	R3	G9
      0x5C100200,  //  0006  MOVE	R4	R1
      0x7C0C0200,  //  0007  CALL	R3	1
      0x040C0203,  //  0008  SUB	R3	R1	R3
      0x88100109,  //  0009  GETMBR	R4	R0	K9
      0x94100802,  //  000A  GETIDX	R4	R4	R2
      0x00140508,  //  000B  ADD	R5	R2	K8
      0x541A00FE,  //  000C  LDINT	R6	255
      0x2C140A06,  //  000D  AND	R5	R5	R6
      0x88180109,  //  000E  GETMBR	R6	R0	K9
      0x94140C05,  //  000F  GETIDX	R5	R6	R5
      0xB81A3A00,  //  0010  GETNGBL	R6	K29
      0x8C180D1E,  //  0011  GETMET	R6	R6	K30
      0x60200009,  //  0012  GETGBL	R8	G9
      0x542600FF,  //  0013  LDINT	R9	256
      0x08240609,  //  0014  MUL	R9	R3	R9
      0x7C200200,  //  0015  CALL	R8	1
      0x58240006,  //  0016  LDCONST	R9	K6
      0x542A00FF,  //  0017  LDINT	R10	256
      0x582C0006,  //  0018  LDCONST	R11	K6
      0x543200FE,  //  0019  LDINT	R12	255
      0x7C180C00,  //  001A  CALL	R6	6
      0xB81E3A00,  //  001B  GETNGBL	R7	K29
      0x8C1C0F1E,  //  001C  GETMET	R7	R7	K30
      0x5C240C00,  //  001D  MOVE	R9	R6
      0x58280006,  //  001E  LDCONST	R10	K6
      0x542E00FE,  //  001F  LDINT	R11	255
      0x5C300800,  //  0020  MOVE	R12	R4
      0x5C340A00,  //  0021  MOVE	R13	R5
      0x7C1C0C00,  //  0022  CALL	R7	6
      0x80040E00,  //  0023  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _fractal_noise
********************************************************************/
be_local_closure(class_NoiseAnimation__fractal_noise,   /* name */
  be_nested_proto(
    20,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_NoiseAnimation,     /* shared constants */
    be_str_weak(_fractal_noise),
    &be_const_str_solidified,
    ( &(const binstruction[62]) {  /* code */
      0x580C0006,  //  0000  LDCONST	R3	K6
      0x541200FE,  //  0001  LDINT	R4	255
      0x8814012B,  //  0002  GETMBR	R5	R0	K43
      0x8818012C,  //  0003  GETMBR	R6	R0	K44
      0x881C012F,  //  0004  GETMBR	R7	R0	K47
      0x5C200A00,  //  0005  MOVE	R8	R5
      0x58240006,  //  0006  LDCONST	R9	K6
      0x58280006,  //  0007  LDCONST	R10	K6
      0x142C1406,  //  0008  LT	R11	R10	R6
      0x782E0027,  //  0009  JMPF	R11	#0032
      0xB82E3A00,  //  000A  GETNGBL	R11	K29
      0x8C2C171E,  //  000B  GETMET	R11	R11	K30
      0x08340208,  //  000C  MUL	R13	R1	R8
      0x58380006,  //  000D  LDCONST	R14	K6
      0x543E00FE,  //  000E  LDINT	R15	255
      0x544200FE,  //  000F  LDINT	R16	255
      0x083C1E10,  //  0010  MUL	R15	R15	R16
      0x58400006,  //  0011  LDCONST	R16	K6
      0x544600FE,  //  0012  LDINT	R17	255
      0x7C2C0C00,  //  0013  CALL	R11	6
      0x002C1602,  //  0014  ADD	R11	R11	R2
      0x8C300130,  //  0015  GETMET	R12	R0	K48
      0x5C381600,  //  0016  MOVE	R14	R11
      0x7C300400,  //  0017  CALL	R12	2
      0xB8363A00,  //  0018  GETNGBL	R13	K29
      0x8C341B1E,  //  0019  GETMET	R13	R13	K30
      0x5C3C1800,  //  001A  MOVE	R15	R12
      0x58400006,  //  001B  LDCONST	R16	K6
      0x544600FE,  //  001C  LDINT	R17	255
      0x58480006,  //  001D  LDCONST	R18	K6
      0x5C4C0800,  //  001E  MOVE	R19	R4
      0x7C340C00,  //  001F  CALL	R13	6
      0x000C060D,  //  0020  ADD	R3	R3	R13
      0x00241204,  //  0021  ADD	R9	R9	R4
      0xB8363A00,  //  0022  GETNGBL	R13	K29
      0x8C341B1E,  //  0023  GETMET	R13	R13	K30
      0x5C3C0800,  //  0024  MOVE	R15	R4
      0x58400006,  //  0025  LDCONST	R16	K6
      0x544600FE,  //  0026  LDINT	R17	255
      0x58480006,  //  0027  LDCONST	R18	K6
      0x5C4C0E00,  //  0028  MOVE	R19	R7
      0x7C340C00,  //  0029  CALL	R13	6
      0x5C101A00,  //  002A  MOVE	R4	R13
      0x08201131,  //  002B  MUL	R8	R8	K49
      0x543600FE,  //  002C  LDINT	R13	255
      0x2434100D,  //  002D  GT	R13	R8	R13
      0x78360000,  //  002E  JMPF	R13	#0030
      0x542200FE,  //  002F  LDINT	R8	255
      0x00281508,  //  0030  ADD	R10	R10	K8
      0x7001FFD5,  //  0031  JMP		#0008
      0x242C1306,  //  0032  GT	R11	R9	K6
      0x782E0008,  //  0033  JMPF	R11	#003D
      0xB82E3A00,  //  0034  GETNGBL	R11	K29
      0x8C2C171E,  //  0035  GETMET	R11	R11	K30
      0x5C340600,  //  0036  MOVE	R13	R3
      0x58380006,  //  0037  LDCONST	R14	K6
      0x5C3C1200,  //  0038  MOVE	R15	R9
      0x58400006,  //  0039  LDCONST	R16	K6
      0x544600FE,  //  003A  LDINT	R17	255
      0x7C2C0C00,  //  003B  CALL	R11	6
      0x5C0C1600,  //  003C  MOVE	R3	R11
      0x80040600,  //  003D  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: NoiseAnimation
********************************************************************/
extern const bclass be_class_Animation;
be_local_class(NoiseAnimation,
    3,
    &be_class_Animation,
    be_nested_map(15,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, -1), be_const_closure(class_NoiseAnimation_init_closure) },
        { be_const_key_weak(setmember, -1), be_const_closure(class_NoiseAnimation_setmember_closure) },
        { be_const_key_weak(_init_noise_table, -1), be_const_closure(class_NoiseAnimation__init_noise_table_closure) },
        { be_const_key_weak(PARAMS, 12), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(persistence, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(128) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(octaves, 0), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(1) },
        { be_const_key_weak(min, -1), be_const_int(1) },
        { be_const_key_weak(max, 1), be_const_int(4) },
    }))    ) } )) },
        { be_const_key_weak(seed, 3), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(12345) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(65535) },
    }))    ) } )) },
        { be_const_key_weak(speed, 1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(30) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(color, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_nil() },
    }))    ) } )) },
        { be_const_key_weak(scale, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(50) },
        { be_const_key_weak(min, -1), be_const_int(1) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(update, 6), be_const_closure(class_NoiseAnimation_update_closure) },
        { be_const_key_weak(time_offset, -1), be_const_var(1) },
        { be_const_key_weak(_calculate_noise, -1), be_const_closure(class_NoiseAnimation__calculate_noise_closure) },
        { be_const_key_weak(on_param_changed, 13), be_const_closure(class_NoiseAnimation_on_param_changed_closure) },
        { be_const_key_weak(_noise_1d, -1), be_const_closure(class_NoiseAnimation__noise_1d_closure) },
        { be_const_key_weak(noise_table, -1), be_const_var(2) },
        { be_const_key_weak(tostring, 8), be_const_closure(class_NoiseAnimation_tostring_closure) },
        { be_const_key_weak(start, -1), be_const_closure(class_NoiseAnimation_start_closure) },
        { be_const_key_weak(current_colors, -1), be_const_var(0) },
        { be_const_key_weak(render, -1), be_const_closure(class_NoiseAnimation_render_closure) },
        { be_const_key_weak(_fractal_noise, -1), be_const_closure(class_NoiseAnimation__fractal_noise_closure) },
    })),
    be_str_weak(NoiseAnimation)
);
// compact class 'ColorCycleColorProvider' ktab size: 23, total: 46 (saved 184 bytes)
static const bvalue be_ktab_class_ColorCycleColorProvider[23] = {
  /* K0   */  be_nested_str_weak(palette),
  /* K1   */  be_nested_str_weak(_get_param_def),
  /* K2   */  be_nested_str_weak(contains),
  /* K3   */  be_nested_str_weak(default),
  /* K4   */  be_nested_str_weak(cycle_period),
  /* K5   */  be_const_int(0),
  /* K6   */  be_nested_str_weak(manual),
  /* K7   */  be_nested_str_weak(auto),
  /* K8   */  be_nested_str_weak(_get_palette_size),
  /* K9   */  be_nested_str_weak(ColorCycleColorProvider_X28palette_size_X3D_X25s_X2C_X20cycle_period_X3D_X25s_X2C_X20mode_X3D_X25s_X2C_X20current_index_X3D_X25s_X29),
  /* K10  */  be_nested_str_weak(current_index),
  /* K11  */  be_nested_str_weak(ColorCycleColorProvider_X28uninitialized_X29),
  /* K12  */  be_nested_str_weak(_get_palette_bytes),
  /* K13  */  be_nested_str_weak(current_color),
  /* K14  */  be_nested_str_weak(_get_color_at_index),
  /* K15  */  be_nested_str_weak(next),
  /* K16  */  be_nested_str_weak(set_param),
  /* K17  */  be_const_int(1),
  /* K18  */  be_nested_str_weak(tasmota),
  /* K19  */  be_nested_str_weak(scale_uint),
  /* K20  */  be_nested_str_weak(init),
  /* K21  */  be_nested_str_weak(get),
  /* K22  */  be_const_int(-16777216),
};


extern const bclass be_class_ColorCycleColorProvider;

/********************************************************************
** Solidified function: _get_palette_bytes
********************************************************************/
be_local_closure(class_ColorCycleColorProvider__get_palette_bytes,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ColorCycleColorProvider,     /* shared constants */
    be_str_weak(_get_palette_bytes),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x1C080202,  //  0002  EQ	R2	R1	R2
      0x780A000A,  //  0003  JMPF	R2	#000F
      0x8C080101,  //  0004  GETMET	R2	R0	K1
      0x58100000,  //  0005  LDCONST	R4	K0
      0x7C080400,  //  0006  CALL	R2	2
      0x4C0C0000,  //  0007  LDNIL	R3
      0x200C0403,  //  0008  NE	R3	R2	R3
      0x780E0004,  //  0009  JMPF	R3	#000F
      0x8C0C0502,  //  000A  GETMET	R3	R2	K2
      0x58140003,  //  000B  LDCONST	R5	K3
      0x7C0C0400,  //  000C  CALL	R3	2
      0x780E0000,  //  000D  JMPF	R3	#000F
      0x94040503,  //  000E  GETIDX	R1	R2	K3
      0x80040200,  //  000F  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_ColorCycleColorProvider_tostring,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ColorCycleColorProvider,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0xA8020012,  //  0000  EXBLK	0	#0014
      0x88040104,  //  0001  GETMBR	R1	R0	K4
      0x1C040305,  //  0002  EQ	R1	R1	K5
      0x78060001,  //  0003  JMPF	R1	#0006
      0x58040006,  //  0004  LDCONST	R1	K6
      0x70020000,  //  0005  JMP		#0007
      0x58040007,  //  0006  LDCONST	R1	K7
      0x8C080108,  //  0007  GETMET	R2	R0	K8
      0x7C080200,  //  0008  CALL	R2	1
      0x600C0018,  //  0009  GETGBL	R3	G24
      0x58100009,  //  000A  LDCONST	R4	K9
      0x5C140400,  //  000B  MOVE	R5	R2
      0x88180104,  //  000C  GETMBR	R6	R0	K4
      0x5C1C0200,  //  000D  MOVE	R7	R1
      0x8820010A,  //  000E  GETMBR	R8	R0	K10
      0x7C0C0A00,  //  000F  CALL	R3	5
      0xA8040001,  //  0010  EXBLK	1	1
      0x80040600,  //  0011  RET	1	R3
      0xA8040001,  //  0012  EXBLK	1	1
      0x70020004,  //  0013  JMP		#0019
      0xAC040000,  //  0014  CATCH	R1	0	0
      0x70020001,  //  0015  JMP		#0018
      0x80061600,  //  0016  RET	1	K11
      0x70020000,  //  0017  JMP		#0019
      0xB0080000,  //  0018  RAISE	2	R0	R0
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _get_palette_size
********************************************************************/
be_local_closure(class_ColorCycleColorProvider__get_palette_size,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ColorCycleColorProvider,     /* shared constants */
    be_str_weak(_get_palette_size),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x8C04010C,  //  0000  GETMET	R1	R0	K12
      0x7C040200,  //  0001  CALL	R1	1
      0x6008000C,  //  0002  GETGBL	R2	G12
      0x5C0C0200,  //  0003  MOVE	R3	R1
      0x7C080200,  //  0004  CALL	R2	1
      0x540E0003,  //  0005  LDINT	R3	4
      0x0C080403,  //  0006  DIV	R2	R2	R3
      0x80040400,  //  0007  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: on_param_changed
********************************************************************/
be_local_closure(class_ColorCycleColorProvider_on_param_changed,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ColorCycleColorProvider,     /* shared constants */
    be_str_weak(on_param_changed),
    &be_const_str_solidified,
    ( &(const binstruction[39]) {  /* code */
      0x1C0C0300,  //  0000  EQ	R3	R1	K0
      0x780E000C,  //  0001  JMPF	R3	#000F
      0x8C0C0108,  //  0002  GETMET	R3	R0	K8
      0x7C0C0200,  //  0003  CALL	R3	1
      0x24100705,  //  0004  GT	R4	R3	K5
      0x78120007,  //  0005  JMPF	R4	#000E
      0x8810010A,  //  0006  GETMBR	R4	R0	K10
      0x28100803,  //  0007  GE	R4	R4	R3
      0x78120000,  //  0008  JMPF	R4	#000A
      0x90021505,  //  0009  SETMBR	R0	K10	K5
      0x8C10010E,  //  000A  GETMET	R4	R0	K14
      0x8818010A,  //  000B  GETMBR	R6	R0	K10
      0x7C100400,  //  000C  CALL	R4	2
      0x90021A04,  //  000D  SETMBR	R0	K13	R4
      0x70020016,  //  000E  JMP		#0026
      0x1C0C030F,  //  000F  EQ	R3	R1	K15
      0x780E0014,  //  0010  JMPF	R3	#0026
      0x200C0505,  //  0011  NE	R3	R2	K5
      0x780E0012,  //  0012  JMPF	R3	#0026
      0x8C0C0108,  //  0013  GETMET	R3	R0	K8
      0x7C0C0200,  //  0014  CALL	R3	1
      0x24100705,  //  0015  GT	R4	R3	K5
      0x7812000A,  //  0016  JMPF	R4	#0022
      0x8810010A,  //  0017  GETMBR	R4	R0	K10
      0x00100802,  //  0018  ADD	R4	R4	R2
      0x10100803,  //  0019  MOD	R4	R4	R3
      0x14140905,  //  001A  LT	R5	R4	K5
      0x78160000,  //  001B  JMPF	R5	#001D
      0x00100803,  //  001C  ADD	R4	R4	R3
      0x90021404,  //  001D  SETMBR	R0	K10	R4
      0x8C14010E,  //  001E  GETMET	R5	R0	K14
      0x881C010A,  //  001F  GETMBR	R7	R0	K10
      0x7C140400,  //  0020  CALL	R5	2
      0x90021A05,  //  0021  SETMBR	R0	K13	R5
      0x8C100110,  //  0022  GETMET	R4	R0	K16
      0x5818000F,  //  0023  LDCONST	R6	K15
      0x581C0005,  //  0024  LDCONST	R7	K5
      0x7C100600,  //  0025  CALL	R4	3
      0x80000000,  //  0026  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: produce_value
********************************************************************/
be_local_closure(class_ColorCycleColorProvider_produce_value,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ColorCycleColorProvider,     /* shared constants */
    be_str_weak(produce_value),
    &be_const_str_solidified,
    ( &(const binstruction[39]) {  /* code */
      0x880C0104,  //  0000  GETMBR	R3	R0	K4
      0x8C100108,  //  0001  GETMET	R4	R0	K8
      0x7C100200,  //  0002  CALL	R4	1
      0x1C140905,  //  0003  EQ	R5	R4	K5
      0x78160001,  //  0004  JMPF	R5	#0007
      0x5415FFFE,  //  0005  LDINT	R5	-1
      0x80040A00,  //  0006  RET	1	R5
      0x1C140911,  //  0007  EQ	R5	R4	K17
      0x78160005,  //  0008  JMPF	R5	#000F
      0x8C14010E,  //  0009  GETMET	R5	R0	K14
      0x581C0005,  //  000A  LDCONST	R7	K5
      0x7C140400,  //  000B  CALL	R5	2
      0x90021A05,  //  000C  SETMBR	R0	K13	R5
      0x8814010D,  //  000D  GETMBR	R5	R0	K13
      0x80040A00,  //  000E  RET	1	R5
      0x1C140705,  //  000F  EQ	R5	R3	K5
      0x78160001,  //  0010  JMPF	R5	#0013
      0x8814010D,  //  0011  GETMBR	R5	R0	K13
      0x80040A00,  //  0012  RET	1	R5
      0x10140403,  //  0013  MOD	R5	R2	R3
      0xB81A2400,  //  0014  GETNGBL	R6	K18
      0x8C180D13,  //  0015  GETMET	R6	R6	K19
      0x5C200A00,  //  0016  MOVE	R8	R5
      0x58240005,  //  0017  LDCONST	R9	K5
      0x04280711,  //  0018  SUB	R10	R3	K17
      0x582C0005,  //  0019  LDCONST	R11	K5
      0x04300911,  //  001A  SUB	R12	R4	K17
      0x7C180C00,  //  001B  CALL	R6	6
      0x281C0C04,  //  001C  GE	R7	R6	R4
      0x781E0001,  //  001D  JMPF	R7	#0020
      0x041C0911,  //  001E  SUB	R7	R4	K17
      0x5C180E00,  //  001F  MOVE	R6	R7
      0x90021406,  //  0020  SETMBR	R0	K10	R6
      0x8C1C010E,  //  0021  GETMET	R7	R0	K14
      0x5C240C00,  //  0022  MOVE	R9	R6
      0x7C1C0400,  //  0023  CALL	R7	2
      0x90021A07,  //  0024  SETMBR	R0	K13	R7
      0x881C010D,  //  0025  GETMBR	R7	R0	K13
      0x80040E00,  //  0026  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_color_for_value
********************************************************************/
be_local_closure(class_ColorCycleColorProvider_get_color_for_value,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ColorCycleColorProvider,     /* shared constants */
    be_str_weak(get_color_for_value),
    &be_const_str_solidified,
    ( &(const binstruction[36]) {  /* code */
      0x8C0C0108,  //  0000  GETMET	R3	R0	K8
      0x7C0C0200,  //  0001  CALL	R3	1
      0x1C100705,  //  0002  EQ	R4	R3	K5
      0x78120001,  //  0003  JMPF	R4	#0006
      0x5411FFFE,  //  0004  LDINT	R4	-1
      0x80040800,  //  0005  RET	1	R4
      0x1C100711,  //  0006  EQ	R4	R3	K17
      0x78120003,  //  0007  JMPF	R4	#000C
      0x8C10010E,  //  0008  GETMET	R4	R0	K14
      0x58180005,  //  0009  LDCONST	R6	K5
      0x7C100400,  //  000A  CALL	R4	2
      0x80040800,  //  000B  RET	1	R4
      0x14100305,  //  000C  LT	R4	R1	K5
      0x78120001,  //  000D  JMPF	R4	#0010
      0x58040005,  //  000E  LDCONST	R1	K5
      0x70020003,  //  000F  JMP		#0014
      0x54120063,  //  0010  LDINT	R4	100
      0x24100204,  //  0011  GT	R4	R1	R4
      0x78120000,  //  0012  JMPF	R4	#0014
      0x54060063,  //  0013  LDINT	R1	100
      0xB8122400,  //  0014  GETNGBL	R4	K18
      0x8C100913,  //  0015  GETMET	R4	R4	K19
      0x5C180200,  //  0016  MOVE	R6	R1
      0x581C0005,  //  0017  LDCONST	R7	K5
      0x54220063,  //  0018  LDINT	R8	100
      0x58240005,  //  0019  LDCONST	R9	K5
      0x04280711,  //  001A  SUB	R10	R3	K17
      0x7C100C00,  //  001B  CALL	R4	6
      0x28140803,  //  001C  GE	R5	R4	R3
      0x78160001,  //  001D  JMPF	R5	#0020
      0x04140711,  //  001E  SUB	R5	R3	K17
      0x5C100A00,  //  001F  MOVE	R4	R5
      0x8C14010E,  //  0020  GETMET	R5	R0	K14
      0x5C1C0800,  //  0021  MOVE	R7	R4
      0x7C140400,  //  0022  CALL	R5	2
      0x80040A00,  //  0023  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_ColorCycleColorProvider_init,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ColorCycleColorProvider,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080514,  //  0003  GETMET	R2	R2	K20
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x8C08010C,  //  0006  GETMET	R2	R0	K12
      0x7C080200,  //  0007  CALL	R2	1
      0x8C0C010E,  //  0008  GETMET	R3	R0	K14
      0x58140005,  //  0009  LDCONST	R5	K5
      0x7C0C0400,  //  000A  CALL	R3	2
      0x90021A03,  //  000B  SETMBR	R0	K13	R3
      0x90021505,  //  000C  SETMBR	R0	K10	K5
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _get_color_at_index
********************************************************************/
be_local_closure(class_ColorCycleColorProvider__get_color_at_index,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ColorCycleColorProvider,     /* shared constants */
    be_str_weak(_get_color_at_index),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x8C08010C,  //  0000  GETMET	R2	R0	K12
      0x7C080200,  //  0001  CALL	R2	1
      0x600C000C,  //  0002  GETGBL	R3	G12
      0x5C100400,  //  0003  MOVE	R4	R2
      0x7C0C0200,  //  0004  CALL	R3	1
      0x54120003,  //  0005  LDINT	R4	4
      0x0C0C0604,  //  0006  DIV	R3	R3	R4
      0x1C100705,  //  0007  EQ	R4	R3	K5
      0x74120003,  //  0008  JMPT	R4	#000D
      0x14100305,  //  0009  LT	R4	R1	K5
      0x74120001,  //  000A  JMPT	R4	#000D
      0x28100203,  //  000B  GE	R4	R1	R3
      0x78120001,  //  000C  JMPF	R4	#000F
      0x5411FFFE,  //  000D  LDINT	R4	-1
      0x80040800,  //  000E  RET	1	R4
      0x8C100515,  //  000F  GETMET	R4	R2	K21
      0x541A0003,  //  0010  LDINT	R6	4
      0x08180206,  //  0011  MUL	R6	R1	R6
      0x541DFFFB,  //  0012  LDINT	R7	-4
      0x7C100600,  //  0013  CALL	R4	3
      0x30100916,  //  0014  OR	R4	R4	K22
      0x80040800,  //  0015  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: ColorCycleColorProvider
********************************************************************/
extern const bclass be_class_ColorProvider;
be_local_class(ColorCycleColorProvider,
    2,
    &be_class_ColorProvider,
    be_nested_map(11,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(_get_color_at_index, -1), be_const_closure(class_ColorCycleColorProvider__get_color_at_index_closure) },
        { be_const_key_weak(_get_palette_size, -1), be_const_closure(class_ColorCycleColorProvider__get_palette_size_closure) },
        { be_const_key_weak(current_color, 6), be_const_var(0) },
        { be_const_key_weak(tostring, -1), be_const_closure(class_ColorCycleColorProvider_tostring_closure) },
        { be_const_key_weak(_get_palette_bytes, 1), be_const_closure(class_ColorCycleColorProvider__get_palette_bytes_closure) },
        { be_const_key_weak(on_param_changed, -1), be_const_closure(class_ColorCycleColorProvider_on_param_changed_closure) },
        { be_const_key_weak(current_index, -1), be_const_var(1) },
        { be_const_key_weak(get_color_for_value, -1), be_const_closure(class_ColorCycleColorProvider_get_color_for_value_closure) },
        { be_const_key_weak(PARAMS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(cycle_period, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(5000) },
        { be_const_key_weak(min, -1), be_const_int(0) },
    }))    ) } )) },
        { be_const_key_weak(next, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(0) },
    }))    ) } )) },
        { be_const_key_weak(palette, 1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_bytes_instance(FF0000FFFF00FF00FFFF0000) },
        { be_const_key_weak(type, -1), be_nested_str_weak(bytes) },
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(init, -1), be_const_closure(class_ColorCycleColorProvider_init_closure) },
        { be_const_key_weak(produce_value, 0), be_const_closure(class_ColorCycleColorProvider_produce_value_closure) },
    })),
    be_str_weak(ColorCycleColorProvider)
);

/********************************************************************
** Solidified function: twinkle_rainbow
********************************************************************/
be_local_closure(twinkle_rainbow,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(twinkle_animation),
    /* K2   */  be_nested_str_weak(color),
    /* K3   */  be_nested_str_weak(density),
    /* K4   */  be_nested_str_weak(twinkle_speed),
    /* K5   */  be_nested_str_weak(fade_speed),
    /* K6   */  be_nested_str_weak(min_brightness),
    /* K7   */  be_nested_str_weak(max_brightness),
    }),
    be_str_weak(twinkle_rainbow),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x5409FFFE,  //  0004  LDINT	R2	-1
      0x90060402,  //  0005  SETMBR	R1	K2	R2
      0x540A0077,  //  0006  LDINT	R2	120
      0x90060602,  //  0007  SETMBR	R1	K3	R2
      0x540A0005,  //  0008  LDINT	R2	6
      0x90060802,  //  0009  SETMBR	R1	K4	R2
      0x540A00B3,  //  000A  LDINT	R2	180
      0x90060A02,  //  000B  SETMBR	R1	K5	R2
      0x540A001F,  //  000C  LDINT	R2	32
      0x90060C02,  //  000D  SETMBR	R1	K6	R2
      0x540A00FE,  //  000E  LDINT	R2	255
      0x90060E02,  //  000F  SETMBR	R1	K7	R2
      0x80040200,  //  0010  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: jitter_color
********************************************************************/
be_local_closure(jitter_color,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(jitter_animation),
    /* K2   */  be_nested_str_weak(jitter_type),
    /* K3   */  be_const_int(1),
    /* K4   */  be_nested_str_weak(color_range),
    }),
    be_str_weak(jitter_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x90060503,  //  0004  SETMBR	R1	K2	K3
      0x540A001D,  //  0005  LDINT	R2	30
      0x90060802,  //  0006  SETMBR	R1	K4	R2
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: clear_all_event_handlers
********************************************************************/
be_local_closure(clear_all_event_handlers,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    0,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(event_manager),
    /* K2   */  be_nested_str_weak(clear_all_handlers),
    }),
    be_str_weak(clear_all_event_handlers),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0xB8020000,  //  0000  GETNGBL	R0	K0
      0x88000101,  //  0001  GETMBR	R0	R0	K1
      0x8C000102,  //  0002  GETMET	R0	R0	K2
      0x7C000200,  //  0003  CALL	R0	1
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/

// compact class 'ClosureValueProvider' ktab size: 26, total: 39 (saved 104 bytes)
static const bvalue be_ktab_class_ClosureValueProvider[26] = {
  /* K0   */  be_nested_str_weak(closure),
  /* K1   */  be_nested_str_weak(_closure),
  /* K2   */  be_nested_str_weak(math),
  /* K3   */  be_nested_str_weak(abs),
  /* K4   */  be_nested_str_weak(int),
  /* K5   */  be_const_int(0),
  /* K6   */  be_const_real_hex(0x437F0000),
  /* K7   */  be_nested_str_weak(sqrt),
  /* K8   */  be_nested_str_weak(ClosureValueProvider_X28_X25s_X29),
  /* K9   */  be_nested_str_weak(closure_X20set),
  /* K10  */  be_nested_str_weak(no_X20closure),
  /* K11  */  be_nested_str_weak(tasmota),
  /* K12  */  be_nested_str_weak(scale_int),
  /* K13  */  be_nested_str_weak(sine_int),
  /* K14  */  be_nested_str_weak(min),
  /* K15  */  be_nested_str_weak(round),
  /* K16  */  be_nested_str_weak(max),
  /* K17  */  be_nested_str_weak(animation),
  /* K18  */  be_nested_str_weak(is_value_provider),
  /* K19  */  be_nested_str_weak(produce_value),
  /* K20  */  be_nested_str_weak(engine),
  /* K21  */  be_nested_str_weak(time_ms),
  /* K22  */  be_nested_str_weak(parameterized_object),
  /* K23  */  be_nested_str_weak(value_error),
  /* K24  */  be_nested_str_weak(Parameter_X20name_X20cannot_X20be_X20nil_X20when_X20resolving_X20object_X20parameter),
  /* K25  */  be_nested_str_weak(get_param_value),
};


extern const bclass be_class_ClosureValueProvider;

/********************************************************************
** Solidified function: on_param_changed
********************************************************************/
be_local_closure(class_ClosureValueProvider_on_param_changed,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ClosureValueProvider,     /* shared constants */
    be_str_weak(on_param_changed),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x1C0C0300,  //  0000  EQ	R3	R1	K0
      0x780E0000,  //  0001  JMPF	R3	#0003
      0x90020202,  //  0002  SETMBR	R0	K1	R2
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: abs
********************************************************************/
be_local_closure(class_ClosureValueProvider_abs,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ClosureValueProvider,     /* shared constants */
    be_str_weak(abs),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0xA40A0400,  //  0000  IMPORT	R2	K2
      0x8C0C0503,  //  0001  GETMET	R3	R2	K3
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x80040600,  //  0004  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: sqrt
********************************************************************/
be_local_closure(class_ClosureValueProvider_sqrt,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ClosureValueProvider,     /* shared constants */
    be_str_weak(sqrt),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0xA40A0400,  //  0000  IMPORT	R2	K2
      0x600C0004,  //  0001  GETGBL	R3	G4
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x1C0C0704,  //  0004  EQ	R3	R3	K4
      0x780E000E,  //  0005  JMPF	R3	#0015
      0x280C0305,  //  0006  GE	R3	R1	K5
      0x780E000C,  //  0007  JMPF	R3	#0015
      0x540E00FE,  //  0008  LDINT	R3	255
      0x180C0203,  //  0009  LE	R3	R1	R3
      0x780E0009,  //  000A  JMPF	R3	#0015
      0x0C0C0306,  //  000B  DIV	R3	R1	K6
      0x60100009,  //  000C  GETGBL	R4	G9
      0x8C140507,  //  000D  GETMET	R5	R2	K7
      0x5C1C0600,  //  000E  MOVE	R7	R3
      0x7C140400,  //  000F  CALL	R5	2
      0x541A00FE,  //  0010  LDINT	R6	255
      0x08140A06,  //  0011  MUL	R5	R5	R6
      0x7C100200,  //  0012  CALL	R4	1
      0x80040800,  //  0013  RET	1	R4
      0x70020003,  //  0014  JMP		#0019
      0x8C0C0507,  //  0015  GETMET	R3	R2	K7
      0x5C140200,  //  0016  MOVE	R5	R1
      0x7C0C0400,  //  0017  CALL	R3	2
      0x80040600,  //  0018  RET	1	R3
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_ClosureValueProvider_tostring,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ClosureValueProvider,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x60040018,  //  0000  GETGBL	R1	G24
      0x58080008,  //  0001  LDCONST	R2	K8
      0x880C0101,  //  0002  GETMBR	R3	R0	K1
      0x780E0001,  //  0003  JMPF	R3	#0006
      0x580C0009,  //  0004  LDCONST	R3	K9
      0x70020000,  //  0005  JMP		#0007
      0x580C000A,  //  0006  LDCONST	R3	K10
      0x7C040400,  //  0007  CALL	R1	2
      0x80040200,  //  0008  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: cos
********************************************************************/
be_local_closure(class_ClosureValueProvider_cos,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ClosureValueProvider,     /* shared constants */
    be_str_weak(cos),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0xB80A1600,  //  0000  GETNGBL	R2	K11
      0x8C08050C,  //  0001  GETMET	R2	R2	K12
      0x5C100200,  //  0002  MOVE	R4	R1
      0x58140005,  //  0003  LDCONST	R5	K5
      0x541A00FE,  //  0004  LDINT	R6	255
      0x581C0005,  //  0005  LDCONST	R7	K5
      0x54227FFE,  //  0006  LDINT	R8	32767
      0x7C080C00,  //  0007  CALL	R2	6
      0xB80E1600,  //  0008  GETNGBL	R3	K11
      0x8C0C070D,  //  0009  GETMET	R3	R3	K13
      0x54161FFF,  //  000A  LDINT	R5	8192
      0x04140405,  //  000B  SUB	R5	R2	R5
      0x7C0C0400,  //  000C  CALL	R3	2
      0xB8121600,  //  000D  GETNGBL	R4	K11
      0x8C10090C,  //  000E  GETMET	R4	R4	K12
      0x5C180600,  //  000F  MOVE	R6	R3
      0x541DEFFF,  //  0010  LDINT	R7	-4096
      0x54220FFF,  //  0011  LDINT	R8	4096
      0x5425FF00,  //  0012  LDINT	R9	-255
      0x542A00FE,  //  0013  LDINT	R10	255
      0x7C100C00,  //  0014  CALL	R4	6
      0x80040800,  //  0015  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: sin
********************************************************************/
be_local_closure(class_ClosureValueProvider_sin,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ClosureValueProvider,     /* shared constants */
    be_str_weak(sin),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0xB80A1600,  //  0000  GETNGBL	R2	K11
      0x8C08050C,  //  0001  GETMET	R2	R2	K12
      0x5C100200,  //  0002  MOVE	R4	R1
      0x58140005,  //  0003  LDCONST	R5	K5
      0x541A00FE,  //  0004  LDINT	R6	255
      0x581C0005,  //  0005  LDCONST	R7	K5
      0x54227FFE,  //  0006  LDINT	R8	32767
      0x7C080C00,  //  0007  CALL	R2	6
      0xB80E1600,  //  0008  GETNGBL	R3	K11
      0x8C0C070D,  //  0009  GETMET	R3	R3	K13
      0x5C140400,  //  000A  MOVE	R5	R2
      0x7C0C0400,  //  000B  CALL	R3	2
      0xB8121600,  //  000C  GETNGBL	R4	K11
      0x8C10090C,  //  000D  GETMET	R4	R4	K12
      0x5C180600,  //  000E  MOVE	R6	R3
      0x541DEFFF,  //  000F  LDINT	R7	-4096
      0x54220FFF,  //  0010  LDINT	R8	4096
      0x5425FF00,  //  0011  LDINT	R9	-255
      0x542A00FE,  //  0012  LDINT	R10	255
      0x7C100C00,  //  0013  CALL	R4	6
      0x80040800,  //  0014  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: min
********************************************************************/
be_local_closure(class_ClosureValueProvider_min,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    11,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ClosureValueProvider,     /* shared constants */
    be_str_weak(min),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0xA40A0400,  //  0000  IMPORT	R2	K2
      0x600C0016,  //  0001  GETGBL	R3	G22
      0x8810050E,  //  0002  GETMBR	R4	R2	K14
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C0C0400,  //  0004  CALL	R3	2
      0x80040600,  //  0005  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: round
********************************************************************/
be_local_closure(class_ClosureValueProvider_round,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ClosureValueProvider,     /* shared constants */
    be_str_weak(round),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0xA40A0400,  //  0000  IMPORT	R2	K2
      0x600C0009,  //  0001  GETGBL	R3	G9
      0x8C10050F,  //  0002  GETMET	R4	R2	K15
      0x5C180200,  //  0003  MOVE	R6	R1
      0x7C100400,  //  0004  CALL	R4	2
      0x7C0C0200,  //  0005  CALL	R3	1
      0x80040600,  //  0006  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: max
********************************************************************/
be_local_closure(class_ClosureValueProvider_max,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    11,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ClosureValueProvider,     /* shared constants */
    be_str_weak(max),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0xA40A0400,  //  0000  IMPORT	R2	K2
      0x600C0016,  //  0001  GETGBL	R3	G22
      0x88100510,  //  0002  GETMBR	R4	R2	K16
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C0C0400,  //  0004  CALL	R3	2
      0x80040600,  //  0005  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: produce_value
********************************************************************/
be_local_closure(class_ClosureValueProvider_produce_value,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ClosureValueProvider,     /* shared constants */
    be_str_weak(produce_value),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x880C0101,  //  0000  GETMBR	R3	R0	K1
      0x4C100000,  //  0001  LDNIL	R4
      0x1C100604,  //  0002  EQ	R4	R3	R4
      0x78120001,  //  0003  JMPF	R4	#0006
      0x4C100000,  //  0004  LDNIL	R4
      0x80040800,  //  0005  RET	1	R4
      0x5C100600,  //  0006  MOVE	R4	R3
      0x5C140000,  //  0007  MOVE	R5	R0
      0x5C180200,  //  0008  MOVE	R6	R1
      0x5C1C0400,  //  0009  MOVE	R7	R2
      0x7C100600,  //  000A  CALL	R4	3
      0x80040800,  //  000B  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: resolve
********************************************************************/
be_local_closure(class_ClosureValueProvider_resolve,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ClosureValueProvider,     /* shared constants */
    be_str_weak(resolve),
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
      0xB80E2200,  //  0000  GETNGBL	R3	K17
      0x8C0C0712,  //  0001  GETMET	R3	R3	K18
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x780E0006,  //  0004  JMPF	R3	#000C
      0x8C0C0313,  //  0005  GETMET	R3	R1	K19
      0x5C140400,  //  0006  MOVE	R5	R2
      0x88180114,  //  0007  GETMBR	R6	R0	K20
      0x88180D15,  //  0008  GETMBR	R6	R6	K21
      0x7C0C0600,  //  0009  CALL	R3	3
      0x80040600,  //  000A  RET	1	R3
      0x70020014,  //  000B  JMP		#0021
      0x4C0C0000,  //  000C  LDNIL	R3
      0x200C0203,  //  000D  NE	R3	R1	R3
      0x780E0010,  //  000E  JMPF	R3	#0020
      0x600C000F,  //  000F  GETGBL	R3	G15
      0x5C100200,  //  0010  MOVE	R4	R1
      0xB8162200,  //  0011  GETNGBL	R5	K17
      0x88140B16,  //  0012  GETMBR	R5	R5	K22
      0x7C0C0400,  //  0013  CALL	R3	2
      0x780E000A,  //  0014  JMPF	R3	#0020
      0x4C0C0000,  //  0015  LDNIL	R3
      0x1C0C0403,  //  0016  EQ	R3	R2	R3
      0x780E0000,  //  0017  JMPF	R3	#0019
      0xB0062F18,  //  0018  RAISE	1	K23	K24
      0x8C0C0319,  //  0019  GETMET	R3	R1	K25
      0x5C140400,  //  001A  MOVE	R5	R2
      0x88180114,  //  001B  GETMBR	R6	R0	K20
      0x88180D15,  //  001C  GETMBR	R6	R6	K21
      0x7C0C0600,  //  001D  CALL	R3	3
      0x80040600,  //  001E  RET	1	R3
      0x70020000,  //  001F  JMP		#0021
      0x80040200,  //  0020  RET	1	R1
      0x80000000,  //  0021  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: scale
********************************************************************/
be_local_closure(class_ClosureValueProvider_scale,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    6,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ClosureValueProvider,     /* shared constants */
    be_str_weak(scale),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0xB81A1600,  //  0000  GETNGBL	R6	K11
      0x8C180D0C,  //  0001  GETMET	R6	R6	K12
      0x5C200200,  //  0002  MOVE	R8	R1
      0x5C240400,  //  0003  MOVE	R9	R2
      0x5C280600,  //  0004  MOVE	R10	R3
      0x5C2C0800,  //  0005  MOVE	R11	R4
      0x5C300A00,  //  0006  MOVE	R12	R5
      0x7C180C00,  //  0007  CALL	R6	6
      0x80040C00,  //  0008  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: ClosureValueProvider
********************************************************************/
extern const bclass be_class_ValueProvider;
be_local_class(ClosureValueProvider,
    1,
    &be_class_ValueProvider,
    be_nested_map(14,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(scale, -1), be_const_closure(class_ClosureValueProvider_scale_closure) },
        { be_const_key_weak(abs, 8), be_const_closure(class_ClosureValueProvider_abs_closure) },
        { be_const_key_weak(resolve, 6), be_const_closure(class_ClosureValueProvider_resolve_closure) },
        { be_const_key_weak(tostring, 2), be_const_closure(class_ClosureValueProvider_tostring_closure) },
        { be_const_key_weak(cos, -1), be_const_closure(class_ClosureValueProvider_cos_closure) },
        { be_const_key_weak(sin, 10), be_const_closure(class_ClosureValueProvider_sin_closure) },
        { be_const_key_weak(min, -1), be_const_closure(class_ClosureValueProvider_min_closure) },
        { be_const_key_weak(round, -1), be_const_closure(class_ClosureValueProvider_round_closure) },
        { be_const_key_weak(produce_value, 12), be_const_closure(class_ClosureValueProvider_produce_value_closure) },
        { be_const_key_weak(max, -1), be_const_closure(class_ClosureValueProvider_max_closure) },
        { be_const_key_weak(PARAMS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(closure, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_nil() },
        { be_const_key_weak(type, -1), be_nested_str_weak(function) },
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(_closure, -1), be_const_var(0) },
        { be_const_key_weak(sqrt, -1), be_const_closure(class_ClosureValueProvider_sqrt_closure) },
        { be_const_key_weak(on_param_changed, 0), be_const_closure(class_ClosureValueProvider_on_param_changed_closure) },
    })),
    be_str_weak(ClosureValueProvider)
);

/********************************************************************
** Solidified function: plasma_fast
********************************************************************/
be_local_closure(plasma_fast,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(plasma_animation),
    /* K2   */  be_nested_str_weak(color),
    /* K3   */  be_nested_str_weak(time_speed),
    /* K4   */  be_nested_str_weak(freq_x),
    /* K5   */  be_nested_str_weak(freq_y),
    /* K6   */  be_nested_str_weak(name),
    /* K7   */  be_nested_str_weak(plasma_fast),
    }),
    be_str_weak(plasma_fast),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x4C080000,  //  0004  LDNIL	R2
      0x90060402,  //  0005  SETMBR	R1	K2	R2
      0x540A0095,  //  0006  LDINT	R2	150
      0x90060602,  //  0007  SETMBR	R1	K3	R2
      0x540A002F,  //  0008  LDINT	R2	48
      0x90060802,  //  0009  SETMBR	R1	K4	R2
      0x540A0022,  //  000A  LDINT	R2	35
      0x90060A02,  //  000B  SETMBR	R1	K5	R2
      0x90060D07,  //  000C  SETMBR	R1	K6	K7
      0x80040200,  //  000D  RET	1	R1
    })
  )
);
/*******************************************************************/


extern const bclass be_class_PaletteGradientAnimation;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_PaletteGradientAnimation_init,   /* name */
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
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(name),
    /* K2   */  be_nested_str_weak(palette_gradient),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x90020302,  //  0006  SETMBR	R0	K1	K2
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _update_value_buffer
********************************************************************/
be_local_closure(class_PaletteGradientAnimation__update_value_buffer,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(shift_period),
    /* K1   */  be_nested_str_weak(engine),
    /* K2   */  be_nested_str_weak(get_strip_length),
    /* K3   */  be_nested_str_weak(value_buffer),
    /* K4   */  be_nested_str_weak(resize),
    /* K5   */  be_nested_str_weak(tasmota),
    /* K6   */  be_nested_str_weak(scale_uint),
    /* K7   */  be_const_int(0),
    /* K8   */  be_const_real_hex(0x447A0000),
    /* K9   */  be_const_int(1),
    }),
    be_str_weak(_update_value_buffer),
    &be_const_str_solidified,
    ( &(const binstruction[43]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x880C0101,  //  0001  GETMBR	R3	R0	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x7C0C0200,  //  0003  CALL	R3	1
      0x6010000C,  //  0004  GETGBL	R4	G12
      0x88140103,  //  0005  GETMBR	R5	R0	K3
      0x7C100200,  //  0006  CALL	R4	1
      0x20100803,  //  0007  NE	R4	R4	R3
      0x78120003,  //  0008  JMPF	R4	#000D
      0x88100103,  //  0009  GETMBR	R4	R0	K3
      0x8C100904,  //  000A  GETMET	R4	R4	K4
      0x5C180600,  //  000B  MOVE	R6	R3
      0x7C100400,  //  000C  CALL	R4	2
      0xB8120A00,  //  000D  GETNGBL	R4	K5
      0x8C100906,  //  000E  GETMET	R4	R4	K6
      0x10180202,  //  000F  MOD	R6	R1	R2
      0x581C0007,  //  0010  LDCONST	R7	K7
      0x5C200400,  //  0011  MOVE	R8	R2
      0x58240007,  //  0012  LDCONST	R9	K7
      0x542A03E7,  //  0013  LDINT	R10	1000
      0x7C100C00,  //  0014  CALL	R4	6
      0x0C100908,  //  0015  DIV	R4	R4	K8
      0x60140009,  //  0016  GETGBL	R5	G9
      0x08180803,  //  0017  MUL	R6	R4	R3
      0x7C140200,  //  0018  CALL	R5	1
      0x58180007,  //  0019  LDCONST	R6	K7
      0x141C0C03,  //  001A  LT	R7	R6	R3
      0x781E000D,  //  001B  JMPF	R7	#002A
      0x001C0C05,  //  001C  ADD	R7	R6	R5
      0x101C0E03,  //  001D  MOD	R7	R7	R3
      0x88200103,  //  001E  GETMBR	R8	R0	K3
      0xB8260A00,  //  001F  GETNGBL	R9	K5
      0x8C241306,  //  0020  GETMET	R9	R9	K6
      0x5C2C0E00,  //  0021  MOVE	R11	R7
      0x58300007,  //  0022  LDCONST	R12	K7
      0x04340709,  //  0023  SUB	R13	R3	K9
      0x58380007,  //  0024  LDCONST	R14	K7
      0x543E0063,  //  0025  LDINT	R15	100
      0x7C240C00,  //  0026  CALL	R9	6
      0x98200C09,  //  0027  SETIDX	R8	R6	R9
      0x00180D09,  //  0028  ADD	R6	R6	K9
      0x7001FFEF,  //  0029  JMP		#001A
      0x80000000,  //  002A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: PaletteGradientAnimation
********************************************************************/
extern const bclass be_class_PalettePatternAnimation;
be_local_class(PaletteGradientAnimation,
    0,
    &be_class_PalettePatternAnimation,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(PARAMS, 1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(shift_period, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(10000) },
        { be_const_key_weak(min, -1), be_const_int(1) },
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(init, -1), be_const_closure(class_PaletteGradientAnimation_init_closure) },
        { be_const_key_weak(_update_value_buffer, -1), be_const_closure(class_PaletteGradientAnimation__update_value_buffer_closure) },
    })),
    be_str_weak(PaletteGradientAnimation)
);
extern const bclass be_class_FrameBuffer;
// compact class 'FrameBuffer' ktab size: 34, total: 122 (saved 704 bytes)
static const bvalue be_ktab_class_FrameBuffer[34] = {
  /* K0   */  be_const_int(0),
  /* K1   */  be_nested_str_weak(value_error),
  /* K2   */  be_nested_str_weak(width_X20must_X20be_X20positive),
  /* K3   */  be_nested_str_weak(width),
  /* K4   */  be_nested_str_weak(pixels),
  /* K5   */  be_nested_str_weak(resize),
  /* K6   */  be_nested_str_weak(clear),
  /* K7   */  be_nested_str_weak(int),
  /* K8   */  be_nested_str_weak(instance),
  /* K9   */  be_nested_str_weak(copy),
  /* K10  */  be_nested_str_weak(argument_X20must_X20be_X20either_X20int_X20or_X20instance),
  /* K11  */  be_nested_str_weak(frame_X20buffers_X20must_X20have_X20the_X20same_X20width),
  /* K12  */  be_nested_str_weak(get_pixel_color),
  /* K13  */  be_nested_str_weak(tasmota),
  /* K14  */  be_nested_str_weak(scale_uint),
  /* K15  */  be_nested_str_weak(set_pixel_color),
  /* K16  */  be_const_int(1),
  /* K17  */  be_nested_str_weak(index_error),
  /* K18  */  be_nested_str_weak(pixel_X20index_X20out_X20of_X20range),
  /* K19  */  be_nested_str_weak(set),
  /* K20  */  be_nested_str_weak(BLEND_MODE_NORMAL),
  /* K21  */  be_nested_str_weak(animation),
  /* K22  */  be_nested_str_weak(frame_buffer),
  /* K23  */  be_const_class(be_class_FrameBuffer),
  /* K24  */  be_nested_str_weak(start_pos_X20out_X20of_X20range),
  /* K25  */  be_nested_str_weak(end_pos_X20out_X20of_X20range),
  /* K26  */  be_nested_str_weak(FrameBuffer_X28width_X3D_X25s_X2C_X20pixels_X3D_X25s_X29),
  /* K27  */  be_nested_str_weak(get),
  /* K28  */  be_nested_str_weak(tohex),
  /* K29  */  be_nested_str_weak(blend),
  /* K30  */  be_nested_str_weak(region_start_X20out_X20of_X20range),
  /* K31  */  be_nested_str_weak(region_end_X20out_X20of_X20range),
  /* K32  */  be_nested_str_weak(),
  /* K33  */  be_nested_str_weak(_X2502X_X2502X_X2502X_X2502X_X7C),
};


extern const bclass be_class_FrameBuffer;

/********************************************************************
** Solidified function: blend_linear
********************************************************************/
be_local_closure(class_FrameBuffer_blend_linear,   /* name */
  be_nested_proto(
    19,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_FrameBuffer,     /* shared constants */
    be_str_weak(blend_linear),
    &be_const_str_solidified,
    ( &(const binstruction[114]) {  /* code */
      0x54120017,  //  0000  LDINT	R4	24
      0x3C100204,  //  0001  SHR	R4	R1	R4
      0x541600FE,  //  0002  LDINT	R5	255
      0x2C100805,  //  0003  AND	R4	R4	R5
      0x5416000F,  //  0004  LDINT	R5	16
      0x3C140205,  //  0005  SHR	R5	R1	R5
      0x541A00FE,  //  0006  LDINT	R6	255
      0x2C140A06,  //  0007  AND	R5	R5	R6
      0x541A0007,  //  0008  LDINT	R6	8
      0x3C180206,  //  0009  SHR	R6	R1	R6
      0x541E00FE,  //  000A  LDINT	R7	255
      0x2C180C07,  //  000B  AND	R6	R6	R7
      0x541E00FE,  //  000C  LDINT	R7	255
      0x2C1C0207,  //  000D  AND	R7	R1	R7
      0x54220017,  //  000E  LDINT	R8	24
      0x3C200408,  //  000F  SHR	R8	R2	R8
      0x542600FE,  //  0010  LDINT	R9	255
      0x2C201009,  //  0011  AND	R8	R8	R9
      0x5426000F,  //  0012  LDINT	R9	16
      0x3C240409,  //  0013  SHR	R9	R2	R9
      0x542A00FE,  //  0014  LDINT	R10	255
      0x2C24120A,  //  0015  AND	R9	R9	R10
      0x542A0007,  //  0016  LDINT	R10	8
      0x3C28040A,  //  0017  SHR	R10	R2	R10
      0x542E00FE,  //  0018  LDINT	R11	255
      0x2C28140B,  //  0019  AND	R10	R10	R11
      0x542E00FE,  //  001A  LDINT	R11	255
      0x2C2C040B,  //  001B  AND	R11	R2	R11
      0x04300808,  //  001C  SUB	R12	R4	R8
      0x08301803,  //  001D  MUL	R12	R12	R3
      0x543600FE,  //  001E  LDINT	R13	255
      0x0C30180D,  //  001F  DIV	R12	R12	R13
      0x0030100C,  //  0020  ADD	R12	R8	R12
      0x04340A09,  //  0021  SUB	R13	R5	R9
      0x08341A03,  //  0022  MUL	R13	R13	R3
      0x543A00FE,  //  0023  LDINT	R14	255
      0x0C341A0E,  //  0024  DIV	R13	R13	R14
      0x0034120D,  //  0025  ADD	R13	R9	R13
      0x04380C0A,  //  0026  SUB	R14	R6	R10
      0x08381C03,  //  0027  MUL	R14	R14	R3
      0x543E00FE,  //  0028  LDINT	R15	255
      0x0C381C0F,  //  0029  DIV	R14	R14	R15
      0x0038140E,  //  002A  ADD	R14	R10	R14
      0x043C0E0B,  //  002B  SUB	R15	R7	R11
      0x083C1E03,  //  002C  MUL	R15	R15	R3
      0x544200FE,  //  002D  LDINT	R16	255
      0x0C3C1E10,  //  002E  DIV	R15	R15	R16
      0x003C160F,  //  002F  ADD	R15	R11	R15
      0x14401900,  //  0030  LT	R16	R12	K0
      0x78420001,  //  0031  JMPF	R16	#0034
      0x58400000,  //  0032  LDCONST	R16	K0
      0x70020005,  //  0033  JMP		#003A
      0x544200FE,  //  0034  LDINT	R16	255
      0x24401810,  //  0035  GT	R16	R12	R16
      0x78420001,  //  0036  JMPF	R16	#0039
      0x544200FE,  //  0037  LDINT	R16	255
      0x70020000,  //  0038  JMP		#003A
      0x5C401800,  //  0039  MOVE	R16	R12
      0x5C302000,  //  003A  MOVE	R12	R16
      0x14401B00,  //  003B  LT	R16	R13	K0
      0x78420001,  //  003C  JMPF	R16	#003F
      0x58400000,  //  003D  LDCONST	R16	K0
      0x70020005,  //  003E  JMP		#0045
      0x544200FE,  //  003F  LDINT	R16	255
      0x24401A10,  //  0040  GT	R16	R13	R16
      0x78420001,  //  0041  JMPF	R16	#0044
      0x544200FE,  //  0042  LDINT	R16	255
      0x70020000,  //  0043  JMP		#0045
      0x5C401A00,  //  0044  MOVE	R16	R13
      0x5C342000,  //  0045  MOVE	R13	R16
      0x14401D00,  //  0046  LT	R16	R14	K0
      0x78420001,  //  0047  JMPF	R16	#004A
      0x58400000,  //  0048  LDCONST	R16	K0
      0x70020005,  //  0049  JMP		#0050
      0x544200FE,  //  004A  LDINT	R16	255
      0x24401C10,  //  004B  GT	R16	R14	R16
      0x78420001,  //  004C  JMPF	R16	#004F
      0x544200FE,  //  004D  LDINT	R16	255
      0x70020000,  //  004E  JMP		#0050
      0x5C401C00,  //  004F  MOVE	R16	R14
      0x5C382000,  //  0050  MOVE	R14	R16
      0x14401F00,  //  0051  LT	R16	R15	K0
      0x78420001,  //  0052  JMPF	R16	#0055
      0x58400000,  //  0053  LDCONST	R16	K0
      0x70020005,  //  0054  JMP		#005B
      0x544200FE,  //  0055  LDINT	R16	255
      0x24401E10,  //  0056  GT	R16	R15	R16
      0x78420001,  //  0057  JMPF	R16	#005A
      0x544200FE,  //  0058  LDINT	R16	255
      0x70020000,  //  0059  JMP		#005B
      0x5C401E00,  //  005A  MOVE	R16	R15
      0x5C3C2000,  //  005B  MOVE	R15	R16
      0x60400009,  //  005C  GETGBL	R16	G9
      0x5C441800,  //  005D  MOVE	R17	R12
      0x7C400200,  //  005E  CALL	R16	1
      0x54460017,  //  005F  LDINT	R17	24
      0x38402011,  //  0060  SHL	R16	R16	R17
      0x60440009,  //  0061  GETGBL	R17	G9
      0x5C481A00,  //  0062  MOVE	R18	R13
      0x7C440200,  //  0063  CALL	R17	1
      0x544A000F,  //  0064  LDINT	R18	16
      0x38442212,  //  0065  SHL	R17	R17	R18
      0x30402011,  //  0066  OR	R16	R16	R17
      0x60440009,  //  0067  GETGBL	R17	G9
      0x5C481C00,  //  0068  MOVE	R18	R14
      0x7C440200,  //  0069  CALL	R17	1
      0x544A0007,  //  006A  LDINT	R18	8
      0x38442212,  //  006B  SHL	R17	R17	R18
      0x30402011,  //  006C  OR	R16	R16	R17
      0x60440009,  //  006D  GETGBL	R17	G9
      0x5C481E00,  //  006E  MOVE	R18	R15
      0x7C440200,  //  006F  CALL	R17	1
      0x30402011,  //  0070  OR	R16	R16	R17
      0x80042000,  //  0071  RET	1	R16
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: resize
********************************************************************/
be_local_closure(class_FrameBuffer_resize,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_FrameBuffer,     /* shared constants */
    be_str_weak(resize),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x18080300,  //  0000  LE	R2	R1	K0
      0x780A0000,  //  0001  JMPF	R2	#0003
      0xB0060302,  //  0002  RAISE	1	K1	K2
      0x88080103,  //  0003  GETMBR	R2	R0	K3
      0x1C080202,  //  0004  EQ	R2	R1	R2
      0x780A0000,  //  0005  JMPF	R2	#0007
      0x80000400,  //  0006  RET	0
      0x90020601,  //  0007  SETMBR	R0	K3	R1
      0x88080104,  //  0008  GETMBR	R2	R0	K4
      0x8C080505,  //  0009  GETMET	R2	R2	K5
      0x88100103,  //  000A  GETMBR	R4	R0	K3
      0x54160003,  //  000B  LDINT	R5	4
      0x08100805,  //  000C  MUL	R4	R4	R5
      0x7C080400,  //  000D  CALL	R2	2
      0x8C080106,  //  000E  GETMET	R2	R0	K6
      0x7C080200,  //  000F  CALL	R2	1
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: clear
********************************************************************/
be_local_closure(class_FrameBuffer_clear,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_FrameBuffer,     /* shared constants */
    be_str_weak(clear),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x8C040306,  //  0001  GETMET	R1	R1	K6
      0x7C040200,  //  0002  CALL	R1	1
      0x88040104,  //  0003  GETMBR	R1	R0	K4
      0x8C040305,  //  0004  GETMET	R1	R1	K5
      0x880C0103,  //  0005  GETMBR	R3	R0	K3
      0x54120003,  //  0006  LDINT	R4	4
      0x080C0604,  //  0007  MUL	R3	R3	R4
      0x7C040400,  //  0008  CALL	R1	2
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_FrameBuffer_init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_FrameBuffer,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[36]) {  /* code */
      0x60080004,  //  0000  GETGBL	R2	G4
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x1C080507,  //  0003  EQ	R2	R2	K7
      0x780A0010,  //  0004  JMPF	R2	#0016
      0x5C080200,  //  0005  MOVE	R2	R1
      0x180C0500,  //  0006  LE	R3	R2	K0
      0x780E0000,  //  0007  JMPF	R3	#0009
      0xB0060302,  //  0008  RAISE	1	K1	K2
      0x90020602,  //  0009  SETMBR	R0	K3	R2
      0x600C0015,  //  000A  GETGBL	R3	G21
      0x54120003,  //  000B  LDINT	R4	4
      0x08100404,  //  000C  MUL	R4	R2	R4
      0x7C0C0200,  //  000D  CALL	R3	1
      0x8C100705,  //  000E  GETMET	R4	R3	K5
      0x541A0003,  //  000F  LDINT	R6	4
      0x08180406,  //  0010  MUL	R6	R2	R6
      0x7C100400,  //  0011  CALL	R4	2
      0x90020803,  //  0012  SETMBR	R0	K4	R3
      0x8C100106,  //  0013  GETMET	R4	R0	K6
      0x7C100200,  //  0014  CALL	R4	1
      0x7002000C,  //  0015  JMP		#0023
      0x60080004,  //  0016  GETGBL	R2	G4
      0x5C0C0200,  //  0017  MOVE	R3	R1
      0x7C080200,  //  0018  CALL	R2	1
      0x1C080508,  //  0019  EQ	R2	R2	K8
      0x780A0006,  //  001A  JMPF	R2	#0022
      0x88080303,  //  001B  GETMBR	R2	R1	K3
      0x90020602,  //  001C  SETMBR	R0	K3	R2
      0x88080304,  //  001D  GETMBR	R2	R1	K4
      0x8C080509,  //  001E  GETMET	R2	R2	K9
      0x7C080200,  //  001F  CALL	R2	1
      0x90020802,  //  0020  SETMBR	R0	K4	R2
      0x70020000,  //  0021  JMP		#0023
      0xB006030A,  //  0022  RAISE	1	K1	K10
      0x80000000,  //  0023  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: apply_mask
********************************************************************/
be_local_closure(class_FrameBuffer_apply_mask,   /* name */
  be_nested_proto(
    18,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_FrameBuffer,     /* shared constants */
    be_str_weak(apply_mask),
    &be_const_str_solidified,
    ( &(const binstruction[65]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x1C0C0403,  //  0001  EQ	R3	R2	R3
      0x780E0000,  //  0002  JMPF	R3	#0004
      0x50080000,  //  0003  LDBOOL	R2	0	0
      0x880C0103,  //  0004  GETMBR	R3	R0	K3
      0x88100303,  //  0005  GETMBR	R4	R1	K3
      0x200C0604,  //  0006  NE	R3	R3	R4
      0x780E0000,  //  0007  JMPF	R3	#0009
      0xB006030B,  //  0008  RAISE	1	K1	K11
      0x580C0000,  //  0009  LDCONST	R3	K0
      0x88100103,  //  000A  GETMBR	R4	R0	K3
      0x14100604,  //  000B  LT	R4	R3	R4
      0x78120032,  //  000C  JMPF	R4	#0040
      0x8C10010C,  //  000D  GETMET	R4	R0	K12
      0x5C180600,  //  000E  MOVE	R6	R3
      0x7C100400,  //  000F  CALL	R4	2
      0x8C14030C,  //  0010  GETMET	R5	R1	K12
      0x5C1C0600,  //  0011  MOVE	R7	R3
      0x7C140400,  //  0012  CALL	R5	2
      0x541A0017,  //  0013  LDINT	R6	24
      0x3C180A06,  //  0014  SHR	R6	R5	R6
      0x541E00FE,  //  0015  LDINT	R7	255
      0x2C180C07,  //  0016  AND	R6	R6	R7
      0x780A0001,  //  0017  JMPF	R2	#001A
      0x541E00FE,  //  0018  LDINT	R7	255
      0x04180E06,  //  0019  SUB	R6	R7	R6
      0x541E0017,  //  001A  LDINT	R7	24
      0x3C1C0807,  //  001B  SHR	R7	R4	R7
      0x542200FE,  //  001C  LDINT	R8	255
      0x2C1C0E08,  //  001D  AND	R7	R7	R8
      0x5422000F,  //  001E  LDINT	R8	16
      0x3C200808,  //  001F  SHR	R8	R4	R8
      0x542600FE,  //  0020  LDINT	R9	255
      0x2C201009,  //  0021  AND	R8	R8	R9
      0x54260007,  //  0022  LDINT	R9	8
      0x3C240809,  //  0023  SHR	R9	R4	R9
      0x542A00FE,  //  0024  LDINT	R10	255
      0x2C24120A,  //  0025  AND	R9	R9	R10
      0x542A00FE,  //  0026  LDINT	R10	255
      0x2C28080A,  //  0027  AND	R10	R4	R10
      0xB82E1A00,  //  0028  GETNGBL	R11	K13
      0x8C2C170E,  //  0029  GETMET	R11	R11	K14
      0x5C340C00,  //  002A  MOVE	R13	R6
      0x58380000,  //  002B  LDCONST	R14	K0
      0x543E00FE,  //  002C  LDINT	R15	255
      0x58400000,  //  002D  LDCONST	R16	K0
      0x5C440E00,  //  002E  MOVE	R17	R7
      0x7C2C0C00,  //  002F  CALL	R11	6
      0x5C1C1600,  //  0030  MOVE	R7	R11
      0x542E0017,  //  0031  LDINT	R11	24
      0x382C0E0B,  //  0032  SHL	R11	R7	R11
      0x5432000F,  //  0033  LDINT	R12	16
      0x3830100C,  //  0034  SHL	R12	R8	R12
      0x302C160C,  //  0035  OR	R11	R11	R12
      0x54320007,  //  0036  LDINT	R12	8
      0x3830120C,  //  0037  SHL	R12	R9	R12
      0x302C160C,  //  0038  OR	R11	R11	R12
      0x302C160A,  //  0039  OR	R11	R11	R10
      0x8C30010F,  //  003A  GETMET	R12	R0	K15
      0x5C380600,  //  003B  MOVE	R14	R3
      0x5C3C1600,  //  003C  MOVE	R15	R11
      0x7C300600,  //  003D  CALL	R12	3
      0x000C0710,  //  003E  ADD	R3	R3	K16
      0x7001FFC9,  //  003F  JMP		#000A
      0x80000000,  //  0040  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_pixel_color
********************************************************************/
be_local_closure(class_FrameBuffer_set_pixel_color,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_FrameBuffer,     /* shared constants */
    be_str_weak(set_pixel_color),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x140C0300,  //  0000  LT	R3	R1	K0
      0x740E0002,  //  0001  JMPT	R3	#0005
      0x880C0103,  //  0002  GETMBR	R3	R0	K3
      0x280C0203,  //  0003  GE	R3	R1	R3
      0x780E0000,  //  0004  JMPF	R3	#0006
      0xB0062312,  //  0005  RAISE	1	K17	K18
      0x880C0104,  //  0006  GETMBR	R3	R0	K4
      0x8C0C0713,  //  0007  GETMET	R3	R3	K19
      0x54160003,  //  0008  LDINT	R5	4
      0x08140205,  //  0009  MUL	R5	R1	R5
      0x5C180400,  //  000A  MOVE	R6	R2
      0x541E0003,  //  000B  LDINT	R7	4
      0x7C0C0800,  //  000C  CALL	R3	4
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: setitem
********************************************************************/
be_local_closure(class_FrameBuffer_setitem,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_FrameBuffer,     /* shared constants */
    be_str_weak(setitem),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C0C010F,  //  0000  GETMET	R3	R0	K15
      0x5C140200,  //  0001  MOVE	R5	R1
      0x5C180400,  //  0002  MOVE	R6	R2
      0x7C0C0600,  //  0003  CALL	R3	3
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: blend
********************************************************************/
be_local_closure(class_FrameBuffer_blend,   /* name */
  be_nested_proto(
    23,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_FrameBuffer,     /* shared constants */
    be_str_weak(blend),
    &be_const_str_solidified,
    ( &(const binstruction[169]) {  /* code */
      0x4C100000,  //  0000  LDNIL	R4
      0x1C100604,  //  0001  EQ	R4	R3	R4
      0x78120000,  //  0002  JMPF	R4	#0004
      0x880C0114,  //  0003  GETMBR	R3	R0	K20
      0x54120017,  //  0004  LDINT	R4	24
      0x3C100204,  //  0005  SHR	R4	R1	R4
      0x541600FE,  //  0006  LDINT	R5	255
      0x2C100805,  //  0007  AND	R4	R4	R5
      0x5416000F,  //  0008  LDINT	R5	16
      0x3C140205,  //  0009  SHR	R5	R1	R5
      0x541A00FE,  //  000A  LDINT	R6	255
      0x2C140A06,  //  000B  AND	R5	R5	R6
      0x541A0007,  //  000C  LDINT	R6	8
      0x3C180206,  //  000D  SHR	R6	R1	R6
      0x541E00FE,  //  000E  LDINT	R7	255
      0x2C180C07,  //  000F  AND	R6	R6	R7
      0x541E00FE,  //  0010  LDINT	R7	255
      0x2C1C0207,  //  0011  AND	R7	R1	R7
      0x54220017,  //  0012  LDINT	R8	24
      0x3C200408,  //  0013  SHR	R8	R2	R8
      0x542600FE,  //  0014  LDINT	R9	255
      0x2C201009,  //  0015  AND	R8	R8	R9
      0x5426000F,  //  0016  LDINT	R9	16
      0x3C240409,  //  0017  SHR	R9	R2	R9
      0x542A00FE,  //  0018  LDINT	R10	255
      0x2C24120A,  //  0019  AND	R9	R9	R10
      0x542A0007,  //  001A  LDINT	R10	8
      0x3C28040A,  //  001B  SHR	R10	R2	R10
      0x542E00FE,  //  001C  LDINT	R11	255
      0x2C28140B,  //  001D  AND	R10	R10	R11
      0x542E00FE,  //  001E  LDINT	R11	255
      0x2C2C040B,  //  001F  AND	R11	R2	R11
      0x1C301100,  //  0020  EQ	R12	R8	K0
      0x78320000,  //  0021  JMPF	R12	#0023
      0x80040200,  //  0022  RET	1	R1
      0x5C301000,  //  0023  MOVE	R12	R8
      0xB8361A00,  //  0024  GETNGBL	R13	K13
      0x8C341B0E,  //  0025  GETMET	R13	R13	K14
      0x543E00FE,  //  0026  LDINT	R15	255
      0x043C1E0C,  //  0027  SUB	R15	R15	R12
      0x58400000,  //  0028  LDCONST	R16	K0
      0x544600FE,  //  0029  LDINT	R17	255
      0x58480000,  //  002A  LDCONST	R18	K0
      0x5C4C0A00,  //  002B  MOVE	R19	R5
      0x7C340C00,  //  002C  CALL	R13	6
      0xB83A1A00,  //  002D  GETNGBL	R14	K13
      0x8C381D0E,  //  002E  GETMET	R14	R14	K14
      0x5C401800,  //  002F  MOVE	R16	R12
      0x58440000,  //  0030  LDCONST	R17	K0
      0x544A00FE,  //  0031  LDINT	R18	255
      0x584C0000,  //  0032  LDCONST	R19	K0
      0x5C501200,  //  0033  MOVE	R20	R9
      0x7C380C00,  //  0034  CALL	R14	6
      0x00341A0E,  //  0035  ADD	R13	R13	R14
      0xB83A1A00,  //  0036  GETNGBL	R14	K13
      0x8C381D0E,  //  0037  GETMET	R14	R14	K14
      0x544200FE,  //  0038  LDINT	R16	255
      0x0440200C,  //  0039  SUB	R16	R16	R12
      0x58440000,  //  003A  LDCONST	R17	K0
      0x544A00FE,  //  003B  LDINT	R18	255
      0x584C0000,  //  003C  LDCONST	R19	K0
      0x5C500C00,  //  003D  MOVE	R20	R6
      0x7C380C00,  //  003E  CALL	R14	6
      0xB83E1A00,  //  003F  GETNGBL	R15	K13
      0x8C3C1F0E,  //  0040  GETMET	R15	R15	K14
      0x5C441800,  //  0041  MOVE	R17	R12
      0x58480000,  //  0042  LDCONST	R18	K0
      0x544E00FE,  //  0043  LDINT	R19	255
      0x58500000,  //  0044  LDCONST	R20	K0
      0x5C541400,  //  0045  MOVE	R21	R10
      0x7C3C0C00,  //  0046  CALL	R15	6
      0x00381C0F,  //  0047  ADD	R14	R14	R15
      0xB83E1A00,  //  0048  GETNGBL	R15	K13
      0x8C3C1F0E,  //  0049  GETMET	R15	R15	K14
      0x544600FE,  //  004A  LDINT	R17	255
      0x0444220C,  //  004B  SUB	R17	R17	R12
      0x58480000,  //  004C  LDCONST	R18	K0
      0x544E00FE,  //  004D  LDINT	R19	255
      0x58500000,  //  004E  LDCONST	R20	K0
      0x5C540E00,  //  004F  MOVE	R21	R7
      0x7C3C0C00,  //  0050  CALL	R15	6
      0xB8421A00,  //  0051  GETNGBL	R16	K13
      0x8C40210E,  //  0052  GETMET	R16	R16	K14
      0x5C481800,  //  0053  MOVE	R18	R12
      0x584C0000,  //  0054  LDCONST	R19	K0
      0x545200FE,  //  0055  LDINT	R20	255
      0x58540000,  //  0056  LDCONST	R21	K0
      0x5C581600,  //  0057  MOVE	R22	R11
      0x7C400C00,  //  0058  CALL	R16	6
      0x003C1E10,  //  0059  ADD	R15	R15	R16
      0xB8421A00,  //  005A  GETNGBL	R16	K13
      0x8C40210E,  //  005B  GETMET	R16	R16	K14
      0x544A00FE,  //  005C  LDINT	R18	255
      0x04482404,  //  005D  SUB	R18	R18	R4
      0x08482408,  //  005E  MUL	R18	R18	R8
      0x584C0000,  //  005F  LDCONST	R19	K0
      0x545200FE,  //  0060  LDINT	R20	255
      0x545600FE,  //  0061  LDINT	R21	255
      0x08502815,  //  0062  MUL	R20	R20	R21
      0x58540000,  //  0063  LDCONST	R21	K0
      0x545A00FE,  //  0064  LDINT	R22	255
      0x7C400C00,  //  0065  CALL	R16	6
      0x00400810,  //  0066  ADD	R16	R4	R16
      0x14441B00,  //  0067  LT	R17	R13	K0
      0x78460001,  //  0068  JMPF	R17	#006B
      0x58440000,  //  0069  LDCONST	R17	K0
      0x70020005,  //  006A  JMP		#0071
      0x544600FE,  //  006B  LDINT	R17	255
      0x24441A11,  //  006C  GT	R17	R13	R17
      0x78460001,  //  006D  JMPF	R17	#0070
      0x544600FE,  //  006E  LDINT	R17	255
      0x70020000,  //  006F  JMP		#0071
      0x5C441A00,  //  0070  MOVE	R17	R13
      0x5C342200,  //  0071  MOVE	R13	R17
      0x14441D00,  //  0072  LT	R17	R14	K0
      0x78460001,  //  0073  JMPF	R17	#0076
      0x58440000,  //  0074  LDCONST	R17	K0
      0x70020005,  //  0075  JMP		#007C
      0x544600FE,  //  0076  LDINT	R17	255
      0x24441C11,  //  0077  GT	R17	R14	R17
      0x78460001,  //  0078  JMPF	R17	#007B
      0x544600FE,  //  0079  LDINT	R17	255
      0x70020000,  //  007A  JMP		#007C
      0x5C441C00,  //  007B  MOVE	R17	R14
      0x5C382200,  //  007C  MOVE	R14	R17
      0x14441F00,  //  007D  LT	R17	R15	K0
      0x78460001,  //  007E  JMPF	R17	#0081
      0x58440000,  //  007F  LDCONST	R17	K0
      0x70020005,  //  0080  JMP		#0087
      0x544600FE,  //  0081  LDINT	R17	255
      0x24441E11,  //  0082  GT	R17	R15	R17
      0x78460001,  //  0083  JMPF	R17	#0086
      0x544600FE,  //  0084  LDINT	R17	255
      0x70020000,  //  0085  JMP		#0087
      0x5C441E00,  //  0086  MOVE	R17	R15
      0x5C3C2200,  //  0087  MOVE	R15	R17
      0x14442100,  //  0088  LT	R17	R16	K0
      0x78460001,  //  0089  JMPF	R17	#008C
      0x58440000,  //  008A  LDCONST	R17	K0
      0x70020005,  //  008B  JMP		#0092
      0x544600FE,  //  008C  LDINT	R17	255
      0x24442011,  //  008D  GT	R17	R16	R17
      0x78460001,  //  008E  JMPF	R17	#0091
      0x544600FE,  //  008F  LDINT	R17	255
      0x70020000,  //  0090  JMP		#0092
      0x5C442000,  //  0091  MOVE	R17	R16
      0x5C402200,  //  0092  MOVE	R16	R17
      0x60440009,  //  0093  GETGBL	R17	G9
      0x5C482000,  //  0094  MOVE	R18	R16
      0x7C440200,  //  0095  CALL	R17	1
      0x544A0017,  //  0096  LDINT	R18	24
      0x38442212,  //  0097  SHL	R17	R17	R18
      0x60480009,  //  0098  GETGBL	R18	G9
      0x5C4C1A00,  //  0099  MOVE	R19	R13
      0x7C480200,  //  009A  CALL	R18	1
      0x544E000F,  //  009B  LDINT	R19	16
      0x38482413,  //  009C  SHL	R18	R18	R19
      0x30442212,  //  009D  OR	R17	R17	R18
      0x60480009,  //  009E  GETGBL	R18	G9
      0x5C4C1C00,  //  009F  MOVE	R19	R14
      0x7C480200,  //  00A0  CALL	R18	1
      0x544E0007,  //  00A1  LDINT	R19	8
      0x38482413,  //  00A2  SHL	R18	R18	R19
      0x30442212,  //  00A3  OR	R17	R17	R18
      0x60480009,  //  00A4  GETGBL	R18	G9
      0x5C4C1E00,  //  00A5  MOVE	R19	R15
      0x7C480200,  //  00A6  CALL	R18	1
      0x30442212,  //  00A7  OR	R17	R17	R18
      0x80042200,  //  00A8  RET	1	R17
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: copy
********************************************************************/
be_local_closure(class_FrameBuffer_copy,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_FrameBuffer,     /* shared constants */
    be_str_weak(copy),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0xB8062A00,  //  0000  GETNGBL	R1	K21
      0x8C040316,  //  0001  GETMET	R1	R1	K22
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x80040200,  //  0004  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: to_color
********************************************************************/
be_local_closure(class_FrameBuffer_to_color,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    4,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_FrameBuffer,     /* shared constants */
    be_str_weak(to_color),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x58100017,  //  0000  LDCONST	R4	K23
      0x4C140000,  //  0001  LDNIL	R5
      0x1C140605,  //  0002  EQ	R5	R3	R5
      0x78160000,  //  0003  JMPF	R5	#0005
      0x540E00FE,  //  0004  LDINT	R3	255
      0x541600FE,  //  0005  LDINT	R5	255
      0x2C000005,  //  0006  AND	R0	R0	R5
      0x541600FE,  //  0007  LDINT	R5	255
      0x2C040205,  //  0008  AND	R1	R1	R5
      0x541600FE,  //  0009  LDINT	R5	255
      0x2C080405,  //  000A  AND	R2	R2	R5
      0x541600FE,  //  000B  LDINT	R5	255
      0x2C0C0605,  //  000C  AND	R3	R3	R5
      0x54160017,  //  000D  LDINT	R5	24
      0x38140605,  //  000E  SHL	R5	R3	R5
      0x541A000F,  //  000F  LDINT	R6	16
      0x38180006,  //  0010  SHL	R6	R0	R6
      0x30140A06,  //  0011  OR	R5	R5	R6
      0x541A0007,  //  0012  LDINT	R6	8
      0x38180206,  //  0013  SHL	R6	R1	R6
      0x30140A06,  //  0014  OR	R5	R5	R6
      0x30140A02,  //  0015  OR	R5	R5	R2
      0x80040A00,  //  0016  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: apply_brightness
********************************************************************/
be_local_closure(class_FrameBuffer_apply_brightness,   /* name */
  be_nested_proto(
    18,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_FrameBuffer,     /* shared constants */
    be_str_weak(apply_brightness),
    &be_const_str_solidified,
    ( &(const binstruction[161]) {  /* code */
      0x4C100000,  //  0000  LDNIL	R4
      0x1C100204,  //  0001  EQ	R4	R1	R4
      0x78120000,  //  0002  JMPF	R4	#0004
      0x540600FE,  //  0003  LDINT	R1	255
      0x4C100000,  //  0004  LDNIL	R4
      0x1C100404,  //  0005  EQ	R4	R2	R4
      0x78120000,  //  0006  JMPF	R4	#0008
      0x58080000,  //  0007  LDCONST	R2	K0
      0x4C100000,  //  0008  LDNIL	R4
      0x1C100604,  //  0009  EQ	R4	R3	R4
      0x78120002,  //  000A  JMPF	R4	#000E
      0x88100103,  //  000B  GETMBR	R4	R0	K3
      0x04100910,  //  000C  SUB	R4	R4	K16
      0x5C0C0800,  //  000D  MOVE	R3	R4
      0x14100500,  //  000E  LT	R4	R2	K0
      0x74120002,  //  000F  JMPT	R4	#0013
      0x88100103,  //  0010  GETMBR	R4	R0	K3
      0x28100404,  //  0011  GE	R4	R2	R4
      0x78120000,  //  0012  JMPF	R4	#0014
      0xB0062318,  //  0013  RAISE	1	K17	K24
      0x14100602,  //  0014  LT	R4	R3	R2
      0x74120002,  //  0015  JMPT	R4	#0019
      0x88100103,  //  0016  GETMBR	R4	R0	K3
      0x28100604,  //  0017  GE	R4	R3	R4
      0x78120000,  //  0018  JMPF	R4	#001A
      0xB0062319,  //  0019  RAISE	1	K17	K25
      0x14100300,  //  001A  LT	R4	R1	K0
      0x78120001,  //  001B  JMPF	R4	#001E
      0x58100000,  //  001C  LDCONST	R4	K0
      0x70020005,  //  001D  JMP		#0024
      0x541201FE,  //  001E  LDINT	R4	511
      0x24100204,  //  001F  GT	R4	R1	R4
      0x78120001,  //  0020  JMPF	R4	#0023
      0x541201FE,  //  0021  LDINT	R4	511
      0x70020000,  //  0022  JMP		#0024
      0x5C100200,  //  0023  MOVE	R4	R1
      0x5C040800,  //  0024  MOVE	R1	R4
      0x5C100400,  //  0025  MOVE	R4	R2
      0x18140803,  //  0026  LE	R5	R4	R3
      0x78160077,  //  0027  JMPF	R5	#00A0
      0x8C14010C,  //  0028  GETMET	R5	R0	K12
      0x5C1C0800,  //  0029  MOVE	R7	R4
      0x7C140400,  //  002A  CALL	R5	2
      0x541A0017,  //  002B  LDINT	R6	24
      0x3C180A06,  //  002C  SHR	R6	R5	R6
      0x541E00FE,  //  002D  LDINT	R7	255
      0x2C180C07,  //  002E  AND	R6	R6	R7
      0x541E000F,  //  002F  LDINT	R7	16
      0x3C1C0A07,  //  0030  SHR	R7	R5	R7
      0x542200FE,  //  0031  LDINT	R8	255
      0x2C1C0E08,  //  0032  AND	R7	R7	R8
      0x54220007,  //  0033  LDINT	R8	8
      0x3C200A08,  //  0034  SHR	R8	R5	R8
      0x542600FE,  //  0035  LDINT	R9	255
      0x2C201009,  //  0036  AND	R8	R8	R9
      0x542600FE,  //  0037  LDINT	R9	255
      0x2C240A09,  //  0038  AND	R9	R5	R9
      0x542A00FE,  //  0039  LDINT	R10	255
      0x1828020A,  //  003A  LE	R10	R1	R10
      0x782A001B,  //  003B  JMPF	R10	#0058
      0xB82A1A00,  //  003C  GETNGBL	R10	K13
      0x8C28150E,  //  003D  GETMET	R10	R10	K14
      0x5C300E00,  //  003E  MOVE	R12	R7
      0x58340000,  //  003F  LDCONST	R13	K0
      0x543A00FE,  //  0040  LDINT	R14	255
      0x583C0000,  //  0041  LDCONST	R15	K0
      0x5C400200,  //  0042  MOVE	R16	R1
      0x7C280C00,  //  0043  CALL	R10	6
      0x5C1C1400,  //  0044  MOVE	R7	R10
      0xB82A1A00,  //  0045  GETNGBL	R10	K13
      0x8C28150E,  //  0046  GETMET	R10	R10	K14
      0x5C301000,  //  0047  MOVE	R12	R8
      0x58340000,  //  0048  LDCONST	R13	K0
      0x543A00FE,  //  0049  LDINT	R14	255
      0x583C0000,  //  004A  LDCONST	R15	K0
      0x5C400200,  //  004B  MOVE	R16	R1
      0x7C280C00,  //  004C  CALL	R10	6
      0x5C201400,  //  004D  MOVE	R8	R10
      0xB82A1A00,  //  004E  GETNGBL	R10	K13
      0x8C28150E,  //  004F  GETMET	R10	R10	K14
      0x5C301200,  //  0050  MOVE	R12	R9
      0x58340000,  //  0051  LDCONST	R13	K0
      0x543A00FE,  //  0052  LDINT	R14	255
      0x583C0000,  //  0053  LDCONST	R15	K0
      0x5C400200,  //  0054  MOVE	R16	R1
      0x7C280C00,  //  0055  CALL	R10	6
      0x5C241400,  //  0056  MOVE	R9	R10
      0x70020037,  //  0057  JMP		#0090
      0x542A00FE,  //  0058  LDINT	R10	255
      0x0428020A,  //  0059  SUB	R10	R1	R10
      0xB82E1A00,  //  005A  GETNGBL	R11	K13
      0x8C2C170E,  //  005B  GETMET	R11	R11	K14
      0x08340E0A,  //  005C  MUL	R13	R7	R10
      0x58380000,  //  005D  LDCONST	R14	K0
      0x543E00FE,  //  005E  LDINT	R15	255
      0x544200FF,  //  005F  LDINT	R16	256
      0x083C1E10,  //  0060  MUL	R15	R15	R16
      0x58400000,  //  0061  LDCONST	R16	K0
      0x544600FE,  //  0062  LDINT	R17	255
      0x7C2C0C00,  //  0063  CALL	R11	6
      0x001C0E0B,  //  0064  ADD	R7	R7	R11
      0xB82E1A00,  //  0065  GETNGBL	R11	K13
      0x8C2C170E,  //  0066  GETMET	R11	R11	K14
      0x0834100A,  //  0067  MUL	R13	R8	R10
      0x58380000,  //  0068  LDCONST	R14	K0
      0x543E00FE,  //  0069  LDINT	R15	255
      0x544200FF,  //  006A  LDINT	R16	256
      0x083C1E10,  //  006B  MUL	R15	R15	R16
      0x58400000,  //  006C  LDCONST	R16	K0
      0x544600FE,  //  006D  LDINT	R17	255
      0x7C2C0C00,  //  006E  CALL	R11	6
      0x0020100B,  //  006F  ADD	R8	R8	R11
      0xB82E1A00,  //  0070  GETNGBL	R11	K13
      0x8C2C170E,  //  0071  GETMET	R11	R11	K14
      0x0834120A,  //  0072  MUL	R13	R9	R10
      0x58380000,  //  0073  LDCONST	R14	K0
      0x543E00FE,  //  0074  LDINT	R15	255
      0x544200FF,  //  0075  LDINT	R16	256
      0x083C1E10,  //  0076  MUL	R15	R15	R16
      0x58400000,  //  0077  LDCONST	R16	K0
      0x544600FE,  //  0078  LDINT	R17	255
      0x7C2C0C00,  //  0079  CALL	R11	6
      0x0024120B,  //  007A  ADD	R9	R9	R11
      0x542E00FE,  //  007B  LDINT	R11	255
      0x242C0E0B,  //  007C  GT	R11	R7	R11
      0x782E0001,  //  007D  JMPF	R11	#0080
      0x542E00FE,  //  007E  LDINT	R11	255
      0x70020000,  //  007F  JMP		#0081
      0x5C2C0E00,  //  0080  MOVE	R11	R7
      0x5C1C1600,  //  0081  MOVE	R7	R11
      0x542E00FE,  //  0082  LDINT	R11	255
      0x242C100B,  //  0083  GT	R11	R8	R11
      0x782E0001,  //  0084  JMPF	R11	#0087
      0x542E00FE,  //  0085  LDINT	R11	255
      0x70020000,  //  0086  JMP		#0088
      0x5C2C1000,  //  0087  MOVE	R11	R8
      0x5C201600,  //  0088  MOVE	R8	R11
      0x542E00FE,  //  0089  LDINT	R11	255
      0x242C120B,  //  008A  GT	R11	R9	R11
      0x782E0001,  //  008B  JMPF	R11	#008E
      0x542E00FE,  //  008C  LDINT	R11	255
      0x70020000,  //  008D  JMP		#008F
      0x5C2C1200,  //  008E  MOVE	R11	R9
      0x5C241600,  //  008F  MOVE	R9	R11
      0x542A0017,  //  0090  LDINT	R10	24
      0x38280C0A,  //  0091  SHL	R10	R6	R10
      0x542E000F,  //  0092  LDINT	R11	16
      0x382C0E0B,  //  0093  SHL	R11	R7	R11
      0x3028140B,  //  0094  OR	R10	R10	R11
      0x542E0007,  //  0095  LDINT	R11	8
      0x382C100B,  //  0096  SHL	R11	R8	R11
      0x3028140B,  //  0097  OR	R10	R10	R11
      0x30281409,  //  0098  OR	R10	R10	R9
      0x5C141400,  //  0099  MOVE	R5	R10
      0x8C28010F,  //  009A  GETMET	R10	R0	K15
      0x5C300800,  //  009B  MOVE	R12	R4
      0x5C340A00,  //  009C  MOVE	R13	R5
      0x7C280600,  //  009D  CALL	R10	3
      0x00100910,  //  009E  ADD	R4	R4	K16
      0x7001FF85,  //  009F  JMP		#0026
      0x80000000,  //  00A0  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_FrameBuffer_tostring,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_FrameBuffer,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x60040018,  //  0000  GETGBL	R1	G24
      0x5808001A,  //  0001  LDCONST	R2	K26
      0x880C0103,  //  0002  GETMBR	R3	R0	K3
      0x88100104,  //  0003  GETMBR	R4	R0	K4
      0x7C040600,  //  0004  CALL	R1	3
      0x80040200,  //  0005  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_pixel_color
********************************************************************/
be_local_closure(class_FrameBuffer_get_pixel_color,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_FrameBuffer,     /* shared constants */
    be_str_weak(get_pixel_color),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x14080300,  //  0000  LT	R2	R1	K0
      0x740A0002,  //  0001  JMPT	R2	#0005
      0x88080103,  //  0002  GETMBR	R2	R0	K3
      0x28080202,  //  0003  GE	R2	R1	R2
      0x780A0000,  //  0004  JMPF	R2	#0006
      0xB0062312,  //  0005  RAISE	1	K17	K18
      0x88080104,  //  0006  GETMBR	R2	R0	K4
      0x8C08051B,  //  0007  GETMET	R2	R2	K27
      0x54120003,  //  0008  LDINT	R4	4
      0x08100204,  //  0009  MUL	R4	R1	R4
      0x54160003,  //  000A  LDINT	R5	4
      0x7C080600,  //  000B  CALL	R2	3
      0x80040400,  //  000C  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tohex
********************************************************************/
be_local_closure(class_FrameBuffer_tohex,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_FrameBuffer,     /* shared constants */
    be_str_weak(tohex),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x8C04031C,  //  0001  GETMET	R1	R1	K28
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: blend_color
********************************************************************/
be_local_closure(class_FrameBuffer_blend_color,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_FrameBuffer,     /* shared constants */
    be_str_weak(blend_color),
    &be_const_str_solidified,
    ( &(const binstruction[63]) {  /* code */
      0x4C140000,  //  0000  LDNIL	R5
      0x1C140405,  //  0001  EQ	R5	R2	R5
      0x78160000,  //  0002  JMPF	R5	#0004
      0x88080114,  //  0003  GETMBR	R2	R0	K20
      0x4C140000,  //  0004  LDNIL	R5
      0x1C140605,  //  0005  EQ	R5	R3	R5
      0x78160000,  //  0006  JMPF	R5	#0008
      0x580C0000,  //  0007  LDCONST	R3	K0
      0x4C140000,  //  0008  LDNIL	R5
      0x1C140805,  //  0009  EQ	R5	R4	R5
      0x78160002,  //  000A  JMPF	R5	#000E
      0x88140103,  //  000B  GETMBR	R5	R0	K3
      0x04140B10,  //  000C  SUB	R5	R5	K16
      0x5C100A00,  //  000D  MOVE	R4	R5
      0x14140700,  //  000E  LT	R5	R3	K0
      0x74160002,  //  000F  JMPT	R5	#0013
      0x88140103,  //  0010  GETMBR	R5	R0	K3
      0x28140605,  //  0011  GE	R5	R3	R5
      0x78160000,  //  0012  JMPF	R5	#0014
      0xB0062318,  //  0013  RAISE	1	K17	K24
      0x14140803,  //  0014  LT	R5	R4	R3
      0x74160002,  //  0015  JMPT	R5	#0019
      0x88140103,  //  0016  GETMBR	R5	R0	K3
      0x28140805,  //  0017  GE	R5	R4	R5
      0x78160000,  //  0018  JMPF	R5	#001A
      0xB0062319,  //  0019  RAISE	1	K17	K25
      0x54160017,  //  001A  LDINT	R5	24
      0x3C140205,  //  001B  SHR	R5	R1	R5
      0x541A00FE,  //  001C  LDINT	R6	255
      0x2C140A06,  //  001D  AND	R5	R5	R6
      0x541A000F,  //  001E  LDINT	R6	16
      0x3C180206,  //  001F  SHR	R6	R1	R6
      0x541E00FE,  //  0020  LDINT	R7	255
      0x2C180C07,  //  0021  AND	R6	R6	R7
      0x541E0007,  //  0022  LDINT	R7	8
      0x3C1C0207,  //  0023  SHR	R7	R1	R7
      0x542200FE,  //  0024  LDINT	R8	255
      0x2C1C0E08,  //  0025  AND	R7	R7	R8
      0x542200FE,  //  0026  LDINT	R8	255
      0x2C200208,  //  0027  AND	R8	R1	R8
      0x5C240600,  //  0028  MOVE	R9	R3
      0x18281204,  //  0029  LE	R10	R9	R4
      0x782A0012,  //  002A  JMPF	R10	#003E
      0x8C28010C,  //  002B  GETMET	R10	R0	K12
      0x5C301200,  //  002C  MOVE	R12	R9
      0x7C280400,  //  002D  CALL	R10	2
      0x242C0B00,  //  002E  GT	R11	R5	K0
      0x782E000B,  //  002F  JMPF	R11	#003C
      0x8C2C011D,  //  0030  GETMET	R11	R0	K29
      0x5C341400,  //  0031  MOVE	R13	R10
      0x5C380200,  //  0032  MOVE	R14	R1
      0x5C3C0400,  //  0033  MOVE	R15	R2
      0x7C2C0800,  //  0034  CALL	R11	4
      0x88300104,  //  0035  GETMBR	R12	R0	K4
      0x8C301913,  //  0036  GETMET	R12	R12	K19
      0x543A0003,  //  0037  LDINT	R14	4
      0x0838120E,  //  0038  MUL	R14	R9	R14
      0x5C3C1600,  //  0039  MOVE	R15	R11
      0x54420003,  //  003A  LDINT	R16	4
      0x7C300800,  //  003B  CALL	R12	4
      0x00241310,  //  003C  ADD	R9	R9	K16
      0x7001FFEA,  //  003D  JMP		#0029
      0x80000000,  //  003E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: apply_opacity
********************************************************************/
be_local_closure(class_FrameBuffer_apply_opacity,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_FrameBuffer,     /* shared constants */
    be_str_weak(apply_opacity),
    &be_const_str_solidified,
    ( &(const binstruction[105]) {  /* code */
      0x4C100000,  //  0000  LDNIL	R4
      0x1C100204,  //  0001  EQ	R4	R1	R4
      0x78120000,  //  0002  JMPF	R4	#0004
      0x540600FE,  //  0003  LDINT	R1	255
      0x4C100000,  //  0004  LDNIL	R4
      0x1C100404,  //  0005  EQ	R4	R2	R4
      0x78120000,  //  0006  JMPF	R4	#0008
      0x58080000,  //  0007  LDCONST	R2	K0
      0x4C100000,  //  0008  LDNIL	R4
      0x1C100604,  //  0009  EQ	R4	R3	R4
      0x78120002,  //  000A  JMPF	R4	#000E
      0x88100103,  //  000B  GETMBR	R4	R0	K3
      0x04100910,  //  000C  SUB	R4	R4	K16
      0x5C0C0800,  //  000D  MOVE	R3	R4
      0x14100500,  //  000E  LT	R4	R2	K0
      0x74120002,  //  000F  JMPT	R4	#0013
      0x88100103,  //  0010  GETMBR	R4	R0	K3
      0x28100404,  //  0011  GE	R4	R2	R4
      0x78120000,  //  0012  JMPF	R4	#0014
      0xB0062318,  //  0013  RAISE	1	K17	K24
      0x14100602,  //  0014  LT	R4	R3	R2
      0x74120002,  //  0015  JMPT	R4	#0019
      0x88100103,  //  0016  GETMBR	R4	R0	K3
      0x28100604,  //  0017  GE	R4	R3	R4
      0x78120000,  //  0018  JMPF	R4	#001A
      0xB0062319,  //  0019  RAISE	1	K17	K25
      0x14100300,  //  001A  LT	R4	R1	K0
      0x78120001,  //  001B  JMPF	R4	#001E
      0x58100000,  //  001C  LDCONST	R4	K0
      0x70020005,  //  001D  JMP		#0024
      0x541201FE,  //  001E  LDINT	R4	511
      0x24100204,  //  001F  GT	R4	R1	R4
      0x78120001,  //  0020  JMPF	R4	#0023
      0x541201FE,  //  0021  LDINT	R4	511
      0x70020000,  //  0022  JMP		#0024
      0x5C100200,  //  0023  MOVE	R4	R1
      0x5C040800,  //  0024  MOVE	R1	R4
      0x5C100400,  //  0025  MOVE	R4	R2
      0x18140803,  //  0026  LE	R5	R4	R3
      0x7816003F,  //  0027  JMPF	R5	#0068
      0x8C14010C,  //  0028  GETMET	R5	R0	K12
      0x5C1C0800,  //  0029  MOVE	R7	R4
      0x7C140400,  //  002A  CALL	R5	2
      0x541A0017,  //  002B  LDINT	R6	24
      0x3C180A06,  //  002C  SHR	R6	R5	R6
      0x541E00FE,  //  002D  LDINT	R7	255
      0x2C180C07,  //  002E  AND	R6	R6	R7
      0x541E000F,  //  002F  LDINT	R7	16
      0x3C1C0A07,  //  0030  SHR	R7	R5	R7
      0x542200FE,  //  0031  LDINT	R8	255
      0x2C1C0E08,  //  0032  AND	R7	R7	R8
      0x54220007,  //  0033  LDINT	R8	8
      0x3C200A08,  //  0034  SHR	R8	R5	R8
      0x542600FE,  //  0035  LDINT	R9	255
      0x2C201009,  //  0036  AND	R8	R8	R9
      0x542600FE,  //  0037  LDINT	R9	255
      0x2C240A09,  //  0038  AND	R9	R5	R9
      0x542A00FE,  //  0039  LDINT	R10	255
      0x1828020A,  //  003A  LE	R10	R1	R10
      0x782A0009,  //  003B  JMPF	R10	#0046
      0xB82A1A00,  //  003C  GETNGBL	R10	K13
      0x8C28150E,  //  003D  GETMET	R10	R10	K14
      0x5C300200,  //  003E  MOVE	R12	R1
      0x58340000,  //  003F  LDCONST	R13	K0
      0x543A00FE,  //  0040  LDINT	R14	255
      0x583C0000,  //  0041  LDCONST	R15	K0
      0x5C400C00,  //  0042  MOVE	R16	R6
      0x7C280C00,  //  0043  CALL	R10	6
      0x5C181400,  //  0044  MOVE	R6	R10
      0x70020011,  //  0045  JMP		#0058
      0xB82A1A00,  //  0046  GETNGBL	R10	K13
      0x8C28150E,  //  0047  GETMET	R10	R10	K14
      0x08300C01,  //  0048  MUL	R12	R6	R1
      0x58340000,  //  0049  LDCONST	R13	K0
      0x543A00FE,  //  004A  LDINT	R14	255
      0x543E00FE,  //  004B  LDINT	R15	255
      0x08381C0F,  //  004C  MUL	R14	R14	R15
      0x583C0000,  //  004D  LDCONST	R15	K0
      0x544200FE,  //  004E  LDINT	R16	255
      0x7C280C00,  //  004F  CALL	R10	6
      0x5C181400,  //  0050  MOVE	R6	R10
      0x542A00FE,  //  0051  LDINT	R10	255
      0x24280C0A,  //  0052  GT	R10	R6	R10
      0x782A0001,  //  0053  JMPF	R10	#0056
      0x542A00FE,  //  0054  LDINT	R10	255
      0x70020000,  //  0055  JMP		#0057
      0x5C280C00,  //  0056  MOVE	R10	R6
      0x5C181400,  //  0057  MOVE	R6	R10
      0x542A0017,  //  0058  LDINT	R10	24
      0x38280C0A,  //  0059  SHL	R10	R6	R10
      0x542E000F,  //  005A  LDINT	R11	16
      0x382C0E0B,  //  005B  SHL	R11	R7	R11
      0x3028140B,  //  005C  OR	R10	R10	R11
      0x542E0007,  //  005D  LDINT	R11	8
      0x382C100B,  //  005E  SHL	R11	R8	R11
      0x3028140B,  //  005F  OR	R10	R10	R11
      0x30281409,  //  0060  OR	R10	R10	R9
      0x5C141400,  //  0061  MOVE	R5	R10
      0x8C28010F,  //  0062  GETMET	R10	R0	K15
      0x5C300800,  //  0063  MOVE	R12	R4
      0x5C340A00,  //  0064  MOVE	R13	R5
      0x7C280600,  //  0065  CALL	R10	3
      0x00100910,  //  0066  ADD	R4	R4	K16
      0x7001FFBD,  //  0067  JMP		#0026
      0x80000000,  //  0068  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: fill_pixels
********************************************************************/
be_local_closure(class_FrameBuffer_fill_pixels,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_FrameBuffer,     /* shared constants */
    be_str_weak(fill_pixels),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0x880C0103,  //  0001  GETMBR	R3	R0	K3
      0x140C0403,  //  0002  LT	R3	R2	R3
      0x780E0008,  //  0003  JMPF	R3	#000D
      0x880C0104,  //  0004  GETMBR	R3	R0	K4
      0x8C0C0713,  //  0005  GETMET	R3	R3	K19
      0x54160003,  //  0006  LDINT	R5	4
      0x08140405,  //  0007  MUL	R5	R2	R5
      0x5C180200,  //  0008  MOVE	R6	R1
      0x541E0003,  //  0009  LDINT	R7	4
      0x7C0C0800,  //  000A  CALL	R3	4
      0x00080510,  //  000B  ADD	R2	R2	K16
      0x7001FFF3,  //  000C  JMP		#0001
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: blend_pixels
********************************************************************/
be_local_closure(class_FrameBuffer_blend_pixels,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_FrameBuffer,     /* shared constants */
    be_str_weak(blend_pixels),
    &be_const_str_solidified,
    ( &(const binstruction[105]) {  /* code */
      0x4C140000,  //  0000  LDNIL	R5
      0x1C140405,  //  0001  EQ	R5	R2	R5
      0x78160000,  //  0002  JMPF	R5	#0004
      0x88080114,  //  0003  GETMBR	R2	R0	K20
      0x4C140000,  //  0004  LDNIL	R5
      0x1C140605,  //  0005  EQ	R5	R3	R5
      0x78160000,  //  0006  JMPF	R5	#0008
      0x580C0000,  //  0007  LDCONST	R3	K0
      0x4C140000,  //  0008  LDNIL	R5
      0x1C140805,  //  0009  EQ	R5	R4	R5
      0x78160002,  //  000A  JMPF	R5	#000E
      0x88140103,  //  000B  GETMBR	R5	R0	K3
      0x04140B10,  //  000C  SUB	R5	R5	K16
      0x5C100A00,  //  000D  MOVE	R4	R5
      0x88140103,  //  000E  GETMBR	R5	R0	K3
      0x88180303,  //  000F  GETMBR	R6	R1	K3
      0x20140A06,  //  0010  NE	R5	R5	R6
      0x78160000,  //  0011  JMPF	R5	#0013
      0xB006030B,  //  0012  RAISE	1	K1	K11
      0x14140700,  //  0013  LT	R5	R3	K0
      0x74160002,  //  0014  JMPT	R5	#0018
      0x88140103,  //  0015  GETMBR	R5	R0	K3
      0x28140605,  //  0016  GE	R5	R3	R5
      0x78160000,  //  0017  JMPF	R5	#0019
      0xB006231E,  //  0018  RAISE	1	K17	K30
      0x14140803,  //  0019  LT	R5	R4	R3
      0x74160002,  //  001A  JMPT	R5	#001E
      0x88140103,  //  001B  GETMBR	R5	R0	K3
      0x28140805,  //  001C  GE	R5	R4	R5
      0x78160000,  //  001D  JMPF	R5	#001F
      0xB006231F,  //  001E  RAISE	1	K17	K31
      0x88140114,  //  001F  GETMBR	R5	R0	K20
      0x1C140405,  //  0020  EQ	R5	R2	R5
      0x78160028,  //  0021  JMPF	R5	#004B
      0x5C140600,  //  0022  MOVE	R5	R3
      0x18180A04,  //  0023  LE	R6	R5	R4
      0x781A0024,  //  0024  JMPF	R6	#004A
      0x8C18030C,  //  0025  GETMET	R6	R1	K12
      0x5C200A00,  //  0026  MOVE	R8	R5
      0x7C180400,  //  0027  CALL	R6	2
      0x541E0017,  //  0028  LDINT	R7	24
      0x3C1C0C07,  //  0029  SHR	R7	R6	R7
      0x542200FE,  //  002A  LDINT	R8	255
      0x2C1C0E08,  //  002B  AND	R7	R7	R8
      0x24200F00,  //  002C  GT	R8	R7	K0
      0x78220019,  //  002D  JMPF	R8	#0048
      0x542200FE,  //  002E  LDINT	R8	255
      0x1C200E08,  //  002F  EQ	R8	R7	R8
      0x78220007,  //  0030  JMPF	R8	#0039
      0x88200104,  //  0031  GETMBR	R8	R0	K4
      0x8C201113,  //  0032  GETMET	R8	R8	K19
      0x542A0003,  //  0033  LDINT	R10	4
      0x08280A0A,  //  0034  MUL	R10	R5	R10
      0x5C2C0C00,  //  0035  MOVE	R11	R6
      0x54320003,  //  0036  LDINT	R12	4
      0x7C200800,  //  0037  CALL	R8	4
      0x7002000E,  //  0038  JMP		#0048
      0x8C20010C,  //  0039  GETMET	R8	R0	K12
      0x5C280A00,  //  003A  MOVE	R10	R5
      0x7C200400,  //  003B  CALL	R8	2
      0x8C24011D,  //  003C  GETMET	R9	R0	K29
      0x5C2C1000,  //  003D  MOVE	R11	R8
      0x5C300C00,  //  003E  MOVE	R12	R6
      0x5C340400,  //  003F  MOVE	R13	R2
      0x7C240800,  //  0040  CALL	R9	4
      0x88280104,  //  0041  GETMBR	R10	R0	K4
      0x8C281513,  //  0042  GETMET	R10	R10	K19
      0x54320003,  //  0043  LDINT	R12	4
      0x08300A0C,  //  0044  MUL	R12	R5	R12
      0x5C341200,  //  0045  MOVE	R13	R9
      0x543A0003,  //  0046  LDINT	R14	4
      0x7C280800,  //  0047  CALL	R10	4
      0x00140B10,  //  0048  ADD	R5	R5	K16
      0x7001FFD8,  //  0049  JMP		#0023
      0x80000C00,  //  004A  RET	0
      0x5C140600,  //  004B  MOVE	R5	R3
      0x18180A04,  //  004C  LE	R6	R5	R4
      0x781A0019,  //  004D  JMPF	R6	#0068
      0x8C18010C,  //  004E  GETMET	R6	R0	K12
      0x5C200A00,  //  004F  MOVE	R8	R5
      0x7C180400,  //  0050  CALL	R6	2
      0x8C1C030C,  //  0051  GETMET	R7	R1	K12
      0x5C240A00,  //  0052  MOVE	R9	R5
      0x7C1C0400,  //  0053  CALL	R7	2
      0x54220017,  //  0054  LDINT	R8	24
      0x3C200E08,  //  0055  SHR	R8	R7	R8
      0x542600FE,  //  0056  LDINT	R9	255
      0x2C201009,  //  0057  AND	R8	R8	R9
      0x24241100,  //  0058  GT	R9	R8	K0
      0x7826000B,  //  0059  JMPF	R9	#0066
      0x8C24011D,  //  005A  GETMET	R9	R0	K29
      0x5C2C0C00,  //  005B  MOVE	R11	R6
      0x5C300E00,  //  005C  MOVE	R12	R7
      0x5C340400,  //  005D  MOVE	R13	R2
      0x7C240800,  //  005E  CALL	R9	4
      0x88280104,  //  005F  GETMBR	R10	R0	K4
      0x8C281513,  //  0060  GETMET	R10	R10	K19
      0x54320003,  //  0061  LDINT	R12	4
      0x08300A0C,  //  0062  MUL	R12	R5	R12
      0x5C341200,  //  0063  MOVE	R13	R9
      0x543A0003,  //  0064  LDINT	R14	4
      0x7C280800,  //  0065  CALL	R10	4
      0x00140B10,  //  0066  ADD	R5	R5	K16
      0x7001FFE3,  //  0067  JMP		#004C
      0x80000000,  //  0068  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: gradient_fill
********************************************************************/
be_local_closure(class_FrameBuffer_gradient_fill,   /* name */
  be_nested_proto(
    26,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_FrameBuffer,     /* shared constants */
    be_str_weak(gradient_fill),
    &be_const_str_solidified,
    ( &(const binstruction[162]) {  /* code */
      0x4C140000,  //  0000  LDNIL	R5
      0x1C140605,  //  0001  EQ	R5	R3	R5
      0x78160000,  //  0002  JMPF	R5	#0004
      0x580C0000,  //  0003  LDCONST	R3	K0
      0x4C140000,  //  0004  LDNIL	R5
      0x1C140805,  //  0005  EQ	R5	R4	R5
      0x78160002,  //  0006  JMPF	R5	#000A
      0x88140103,  //  0007  GETMBR	R5	R0	K3
      0x04140B10,  //  0008  SUB	R5	R5	K16
      0x5C100A00,  //  0009  MOVE	R4	R5
      0x14140700,  //  000A  LT	R5	R3	K0
      0x74160002,  //  000B  JMPT	R5	#000F
      0x88140103,  //  000C  GETMBR	R5	R0	K3
      0x28140605,  //  000D  GE	R5	R3	R5
      0x78160000,  //  000E  JMPF	R5	#0010
      0xB0062318,  //  000F  RAISE	1	K17	K24
      0x14140803,  //  0010  LT	R5	R4	R3
      0x74160002,  //  0011  JMPT	R5	#0015
      0x88140103,  //  0012  GETMBR	R5	R0	K3
      0x28140805,  //  0013  GE	R5	R4	R5
      0x78160000,  //  0014  JMPF	R5	#0016
      0xB0062319,  //  0015  RAISE	1	K17	K25
      0x8C14010F,  //  0016  GETMET	R5	R0	K15
      0x5C1C0600,  //  0017  MOVE	R7	R3
      0x5C200200,  //  0018  MOVE	R8	R1
      0x7C140600,  //  0019  CALL	R5	3
      0x1C140604,  //  001A  EQ	R5	R3	R4
      0x78160000,  //  001B  JMPF	R5	#001D
      0x80000A00,  //  001C  RET	0
      0x8C14010F,  //  001D  GETMET	R5	R0	K15
      0x5C1C0800,  //  001E  MOVE	R7	R4
      0x5C200400,  //  001F  MOVE	R8	R2
      0x7C140600,  //  0020  CALL	R5	3
      0x04140803,  //  0021  SUB	R5	R4	R3
      0x18140B10,  //  0022  LE	R5	R5	K16
      0x78160000,  //  0023  JMPF	R5	#0025
      0x80000A00,  //  0024  RET	0
      0x54160017,  //  0025  LDINT	R5	24
      0x3C140205,  //  0026  SHR	R5	R1	R5
      0x541A00FE,  //  0027  LDINT	R6	255
      0x2C140A06,  //  0028  AND	R5	R5	R6
      0x541A000F,  //  0029  LDINT	R6	16
      0x3C180206,  //  002A  SHR	R6	R1	R6
      0x541E00FE,  //  002B  LDINT	R7	255
      0x2C180C07,  //  002C  AND	R6	R6	R7
      0x541E0007,  //  002D  LDINT	R7	8
      0x3C1C0207,  //  002E  SHR	R7	R1	R7
      0x542200FE,  //  002F  LDINT	R8	255
      0x2C1C0E08,  //  0030  AND	R7	R7	R8
      0x542200FE,  //  0031  LDINT	R8	255
      0x2C200208,  //  0032  AND	R8	R1	R8
      0x54260017,  //  0033  LDINT	R9	24
      0x3C240409,  //  0034  SHR	R9	R2	R9
      0x542A00FE,  //  0035  LDINT	R10	255
      0x2C24120A,  //  0036  AND	R9	R9	R10
      0x542A000F,  //  0037  LDINT	R10	16
      0x3C28040A,  //  0038  SHR	R10	R2	R10
      0x542E00FE,  //  0039  LDINT	R11	255
      0x2C28140B,  //  003A  AND	R10	R10	R11
      0x542E0007,  //  003B  LDINT	R11	8
      0x3C2C040B,  //  003C  SHR	R11	R2	R11
      0x543200FE,  //  003D  LDINT	R12	255
      0x2C2C160C,  //  003E  AND	R11	R11	R12
      0x543200FE,  //  003F  LDINT	R12	255
      0x2C30040C,  //  0040  AND	R12	R2	R12
      0x04340803,  //  0041  SUB	R13	R4	R3
      0x00380710,  //  0042  ADD	R14	R3	K16
      0x143C1C04,  //  0043  LT	R15	R14	R4
      0x783E005B,  //  0044  JMPF	R15	#00A1
      0x043C1C03,  //  0045  SUB	R15	R14	R3
      0xB8421A00,  //  0046  GETNGBL	R16	K13
      0x8C40210E,  //  0047  GETMET	R16	R16	K14
      0x5C481E00,  //  0048  MOVE	R18	R15
      0x584C0000,  //  0049  LDCONST	R19	K0
      0x5C501A00,  //  004A  MOVE	R20	R13
      0x5C540C00,  //  004B  MOVE	R21	R6
      0x5C581400,  //  004C  MOVE	R22	R10
      0x7C400C00,  //  004D  CALL	R16	6
      0xB8461A00,  //  004E  GETNGBL	R17	K13
      0x8C44230E,  //  004F  GETMET	R17	R17	K14
      0x5C4C1E00,  //  0050  MOVE	R19	R15
      0x58500000,  //  0051  LDCONST	R20	K0
      0x5C541A00,  //  0052  MOVE	R21	R13
      0x5C580E00,  //  0053  MOVE	R22	R7
      0x5C5C1600,  //  0054  MOVE	R23	R11
      0x7C440C00,  //  0055  CALL	R17	6
      0xB84A1A00,  //  0056  GETNGBL	R18	K13
      0x8C48250E,  //  0057  GETMET	R18	R18	K14
      0x5C501E00,  //  0058  MOVE	R20	R15
      0x58540000,  //  0059  LDCONST	R21	K0
      0x5C581A00,  //  005A  MOVE	R22	R13
      0x5C5C1000,  //  005B  MOVE	R23	R8
      0x5C601800,  //  005C  MOVE	R24	R12
      0x7C480C00,  //  005D  CALL	R18	6
      0xB84E1A00,  //  005E  GETNGBL	R19	K13
      0x8C4C270E,  //  005F  GETMET	R19	R19	K14
      0x5C541E00,  //  0060  MOVE	R21	R15
      0x58580000,  //  0061  LDCONST	R22	K0
      0x5C5C1A00,  //  0062  MOVE	R23	R13
      0x5C600A00,  //  0063  MOVE	R24	R5
      0x5C641200,  //  0064  MOVE	R25	R9
      0x7C4C0C00,  //  0065  CALL	R19	6
      0x14502100,  //  0066  LT	R20	R16	K0
      0x78520001,  //  0067  JMPF	R20	#006A
      0x58500000,  //  0068  LDCONST	R20	K0
      0x70020005,  //  0069  JMP		#0070
      0x545200FE,  //  006A  LDINT	R20	255
      0x24502014,  //  006B  GT	R20	R16	R20
      0x78520001,  //  006C  JMPF	R20	#006F
      0x545200FE,  //  006D  LDINT	R20	255
      0x70020000,  //  006E  JMP		#0070
      0x5C502000,  //  006F  MOVE	R20	R16
      0x5C402800,  //  0070  MOVE	R16	R20
      0x14502300,  //  0071  LT	R20	R17	K0
      0x78520001,  //  0072  JMPF	R20	#0075
      0x58500000,  //  0073  LDCONST	R20	K0
      0x70020005,  //  0074  JMP		#007B
      0x545200FE,  //  0075  LDINT	R20	255
      0x24502214,  //  0076  GT	R20	R17	R20
      0x78520001,  //  0077  JMPF	R20	#007A
      0x545200FE,  //  0078  LDINT	R20	255
      0x70020000,  //  0079  JMP		#007B
      0x5C502200,  //  007A  MOVE	R20	R17
      0x5C442800,  //  007B  MOVE	R17	R20
      0x14502500,  //  007C  LT	R20	R18	K0
      0x78520001,  //  007D  JMPF	R20	#0080
      0x58500000,  //  007E  LDCONST	R20	K0
      0x70020005,  //  007F  JMP		#0086
      0x545200FE,  //  0080  LDINT	R20	255
      0x24502414,  //  0081  GT	R20	R18	R20
      0x78520001,  //  0082  JMPF	R20	#0085
      0x545200FE,  //  0083  LDINT	R20	255
      0x70020000,  //  0084  JMP		#0086
      0x5C502400,  //  0085  MOVE	R20	R18
      0x5C482800,  //  0086  MOVE	R18	R20
      0x14502700,  //  0087  LT	R20	R19	K0
      0x78520001,  //  0088  JMPF	R20	#008B
      0x58500000,  //  0089  LDCONST	R20	K0
      0x70020005,  //  008A  JMP		#0091
      0x545200FE,  //  008B  LDINT	R20	255
      0x24502614,  //  008C  GT	R20	R19	R20
      0x78520001,  //  008D  JMPF	R20	#0090
      0x545200FE,  //  008E  LDINT	R20	255
      0x70020000,  //  008F  JMP		#0091
      0x5C502600,  //  0090  MOVE	R20	R19
      0x5C4C2800,  //  0091  MOVE	R19	R20
      0x54520017,  //  0092  LDINT	R20	24
      0x38502614,  //  0093  SHL	R20	R19	R20
      0x5456000F,  //  0094  LDINT	R21	16
      0x38542015,  //  0095  SHL	R21	R16	R21
      0x30502815,  //  0096  OR	R20	R20	R21
      0x54560007,  //  0097  LDINT	R21	8
      0x38542215,  //  0098  SHL	R21	R17	R21
      0x30502815,  //  0099  OR	R20	R20	R21
      0x30502812,  //  009A  OR	R20	R20	R18
      0x8C54010F,  //  009B  GETMET	R21	R0	K15
      0x5C5C1C00,  //  009C  MOVE	R23	R14
      0x5C602800,  //  009D  MOVE	R24	R20
      0x7C540600,  //  009E  CALL	R21	3
      0x00381D10,  //  009F  ADD	R14	R14	K16
      0x7001FFA1,  //  00A0  JMP		#0043
      0x80000000,  //  00A1  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: item
********************************************************************/
be_local_closure(class_FrameBuffer_item,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_FrameBuffer,     /* shared constants */
    be_str_weak(item),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C08010C,  //  0000  GETMET	R2	R0	K12
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x80040400,  //  0003  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: dump
********************************************************************/
be_local_closure(class_FrameBuffer_dump,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_FrameBuffer,     /* shared constants */
    be_str_weak(dump),
    &be_const_str_solidified,
    ( &(const binstruction[36]) {  /* code */
      0x58040020,  //  0000  LDCONST	R1	K32
      0x58080000,  //  0001  LDCONST	R2	K0
      0x880C0103,  //  0002  GETMBR	R3	R0	K3
      0x140C0403,  //  0003  LT	R3	R2	R3
      0x780E001A,  //  0004  JMPF	R3	#0020
      0x8C0C010C,  //  0005  GETMET	R3	R0	K12
      0x5C140400,  //  0006  MOVE	R5	R2
      0x7C0C0400,  //  0007  CALL	R3	2
      0x54120017,  //  0008  LDINT	R4	24
      0x3C100604,  //  0009  SHR	R4	R3	R4
      0x541600FE,  //  000A  LDINT	R5	255
      0x2C100805,  //  000B  AND	R4	R4	R5
      0x5416000F,  //  000C  LDINT	R5	16
      0x3C140605,  //  000D  SHR	R5	R3	R5
      0x541A00FE,  //  000E  LDINT	R6	255
      0x2C140A06,  //  000F  AND	R5	R5	R6
      0x541A0007,  //  0010  LDINT	R6	8
      0x3C180606,  //  0011  SHR	R6	R3	R6
      0x541E00FE,  //  0012  LDINT	R7	255
      0x2C180C07,  //  0013  AND	R6	R6	R7
      0x541E00FE,  //  0014  LDINT	R7	255
      0x2C1C0607,  //  0015  AND	R7	R3	R7
      0x60200018,  //  0016  GETGBL	R8	G24
      0x58240021,  //  0017  LDCONST	R9	K33
      0x5C280800,  //  0018  MOVE	R10	R4
      0x5C2C0A00,  //  0019  MOVE	R11	R5
      0x5C300C00,  //  001A  MOVE	R12	R6
      0x5C340E00,  //  001B  MOVE	R13	R7
      0x7C200A00,  //  001C  CALL	R8	5
      0x00040208,  //  001D  ADD	R1	R1	R8
      0x00080510,  //  001E  ADD	R2	R2	K16
      0x7001FFE1,  //  001F  JMP		#0002
      0x540DFFFD,  //  0020  LDINT	R3	-2
      0x400E0003,  //  0021  CONNECT	R3	K0	R3
      0x94040203,  //  0022  GETIDX	R1	R1	R3
      0x80040200,  //  0023  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: FrameBuffer
********************************************************************/
be_local_class(FrameBuffer,
    2,
    NULL,
    be_nested_map(24,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(blend_linear, 14), be_const_closure(class_FrameBuffer_blend_linear_closure) },
        { be_const_key_weak(resize, -1), be_const_closure(class_FrameBuffer_resize_closure) },
        { be_const_key_weak(clear, -1), be_const_closure(class_FrameBuffer_clear_closure) },
        { be_const_key_weak(init, 7), be_const_closure(class_FrameBuffer_init_closure) },
        { be_const_key_weak(apply_mask, 6), be_const_closure(class_FrameBuffer_apply_mask_closure) },
        { be_const_key_weak(dump, -1), be_const_closure(class_FrameBuffer_dump_closure) },
        { be_const_key_weak(set_pixel_color, -1), be_const_closure(class_FrameBuffer_set_pixel_color_closure) },
        { be_const_key_weak(gradient_fill, -1), be_const_closure(class_FrameBuffer_gradient_fill_closure) },
        { be_const_key_weak(blend, 18), be_const_closure(class_FrameBuffer_blend_closure) },
        { be_const_key_weak(blend_pixels, 10), be_const_closure(class_FrameBuffer_blend_pixels_closure) },
        { be_const_key_weak(fill_pixels, 19), be_const_closure(class_FrameBuffer_fill_pixels_closure) },
        { be_const_key_weak(apply_brightness, -1), be_const_closure(class_FrameBuffer_apply_brightness_closure) },
        { be_const_key_weak(pixels, -1), be_const_var(0) },
        { be_const_key_weak(tostring, -1), be_const_closure(class_FrameBuffer_tostring_closure) },
        { be_const_key_weak(BLEND_MODE_NORMAL, -1), be_const_int(0) },
        { be_const_key_weak(apply_opacity, -1), be_const_closure(class_FrameBuffer_apply_opacity_closure) },
        { be_const_key_weak(blend_color, -1), be_const_closure(class_FrameBuffer_blend_color_closure) },
        { be_const_key_weak(tohex, 15), be_const_closure(class_FrameBuffer_tohex_closure) },
        { be_const_key_weak(get_pixel_color, -1), be_const_closure(class_FrameBuffer_get_pixel_color_closure) },
        { be_const_key_weak(to_color, 21), be_const_static_closure(class_FrameBuffer_to_color_closure) },
        { be_const_key_weak(setitem, 9), be_const_closure(class_FrameBuffer_setitem_closure) },
        { be_const_key_weak(copy, -1), be_const_closure(class_FrameBuffer_copy_closure) },
        { be_const_key_weak(item, -1), be_const_closure(class_FrameBuffer_item_closure) },
        { be_const_key_weak(width, 5), be_const_var(1) },
    })),
    be_str_weak(FrameBuffer)
);

/********************************************************************
** Solidified function: twinkle_gentle
********************************************************************/
be_local_closure(twinkle_gentle,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(twinkle_animation),
    /* K2   */  be_nested_str_weak(color),
    /* K3   */  be_nested_str_weak(density),
    /* K4   */  be_nested_str_weak(twinkle_speed),
    /* K5   */  be_const_int(3),
    /* K6   */  be_nested_str_weak(fade_speed),
    /* K7   */  be_nested_str_weak(min_brightness),
    /* K8   */  be_nested_str_weak(max_brightness),
    }),
    be_str_weak(twinkle_gentle),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x5409D6FF,  //  0004  LDINT	R2	-10496
      0x90060402,  //  0005  SETMBR	R1	K2	R2
      0x540A003F,  //  0006  LDINT	R2	64
      0x90060602,  //  0007  SETMBR	R1	K3	R2
      0x90060905,  //  0008  SETMBR	R1	K4	K5
      0x540A0077,  //  0009  LDINT	R2	120
      0x90060C02,  //  000A  SETMBR	R1	K6	R2
      0x540A000F,  //  000B  LDINT	R2	16
      0x90060E02,  //  000C  SETMBR	R1	K7	R2
      0x540A00B3,  //  000D  LDINT	R2	180
      0x90061002,  //  000E  SETMBR	R1	K8	R2
      0x80040200,  //  000F  RET	1	R1
    })
  )
);
/*******************************************************************/

// compact class 'CompositeColorProvider' ktab size: 15, total: 25 (saved 80 bytes)
static const bvalue be_ktab_class_CompositeColorProvider[15] = {
  /* K0   */  be_nested_str_weak(providers),
  /* K1   */  be_nested_str_weak(push),
  /* K2   */  be_const_int(0),
  /* K3   */  be_const_int(1),
  /* K4   */  be_nested_str_weak(get_color_for_value),
  /* K5   */  be_nested_str_weak(_blend_colors),
  /* K6   */  be_nested_str_weak(produce_value),
  /* K7   */  be_nested_str_weak(CompositeColorProvider_X28providers_X3D_X25s_X2C_X20blend_mode_X3D_X25s_X29),
  /* K8   */  be_nested_str_weak(blend_mode),
  /* K9   */  be_nested_str_weak(CompositeColorProvider_X28uninitialized_X29),
  /* K10  */  be_const_real_hex(0x437F0000),
  /* K11  */  be_const_int(2),
  /* K12  */  be_nested_str_weak(tasmota),
  /* K13  */  be_nested_str_weak(scale_uint),
  /* K14  */  be_nested_str_weak(init),
};


extern const bclass be_class_CompositeColorProvider;

/********************************************************************
** Solidified function: add_provider
********************************************************************/
be_local_closure(class_CompositeColorProvider_add_provider,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_CompositeColorProvider,     /* shared constants */
    be_str_weak(add_provider),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80040000,  //  0004  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_color_for_value
********************************************************************/
be_local_closure(class_CompositeColorProvider_get_color_for_value,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_CompositeColorProvider,     /* shared constants */
    be_str_weak(get_color_for_value),
    &be_const_str_solidified,
    ( &(const binstruction[45]) {  /* code */
      0x600C000C,  //  0000  GETGBL	R3	G12
      0x88100100,  //  0001  GETMBR	R4	R0	K0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x1C0C0702,  //  0003  EQ	R3	R3	K2
      0x780E0001,  //  0004  JMPF	R3	#0007
      0x540DFFFE,  //  0005  LDINT	R3	-1
      0x80040600,  //  0006  RET	1	R3
      0x600C000C,  //  0007  GETGBL	R3	G12
      0x88100100,  //  0008  GETMBR	R4	R0	K0
      0x7C0C0200,  //  0009  CALL	R3	1
      0x1C0C0703,  //  000A  EQ	R3	R3	K3
      0x780E0006,  //  000B  JMPF	R3	#0013
      0x880C0100,  //  000C  GETMBR	R3	R0	K0
      0x940C0702,  //  000D  GETIDX	R3	R3	K2
      0x8C0C0704,  //  000E  GETMET	R3	R3	K4
      0x5C140200,  //  000F  MOVE	R5	R1
      0x5C180400,  //  0010  MOVE	R6	R2
      0x7C0C0600,  //  0011  CALL	R3	3
      0x80040600,  //  0012  RET	1	R3
      0x880C0100,  //  0013  GETMBR	R3	R0	K0
      0x940C0702,  //  0014  GETIDX	R3	R3	K2
      0x8C0C0704,  //  0015  GETMET	R3	R3	K4
      0x5C140200,  //  0016  MOVE	R5	R1
      0x5C180400,  //  0017  MOVE	R6	R2
      0x7C0C0600,  //  0018  CALL	R3	3
      0x58100003,  //  0019  LDCONST	R4	K3
      0x6014000C,  //  001A  GETGBL	R5	G12
      0x88180100,  //  001B  GETMBR	R6	R0	K0
      0x7C140200,  //  001C  CALL	R5	1
      0x14140805,  //  001D  LT	R5	R4	R5
      0x7816000C,  //  001E  JMPF	R5	#002C
      0x88140100,  //  001F  GETMBR	R5	R0	K0
      0x94140A04,  //  0020  GETIDX	R5	R5	R4
      0x8C140B04,  //  0021  GETMET	R5	R5	K4
      0x5C1C0200,  //  0022  MOVE	R7	R1
      0x5C200400,  //  0023  MOVE	R8	R2
      0x7C140600,  //  0024  CALL	R5	3
      0x8C180105,  //  0025  GETMET	R6	R0	K5
      0x5C200600,  //  0026  MOVE	R8	R3
      0x5C240A00,  //  0027  MOVE	R9	R5
      0x7C180600,  //  0028  CALL	R6	3
      0x5C0C0C00,  //  0029  MOVE	R3	R6
      0x00100903,  //  002A  ADD	R4	R4	K3
      0x7001FFED,  //  002B  JMP		#001A
      0x80040600,  //  002C  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: produce_value
********************************************************************/
be_local_closure(class_CompositeColorProvider_produce_value,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_CompositeColorProvider,     /* shared constants */
    be_str_weak(produce_value),
    &be_const_str_solidified,
    ( &(const binstruction[45]) {  /* code */
      0x600C000C,  //  0000  GETGBL	R3	G12
      0x88100100,  //  0001  GETMBR	R4	R0	K0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x1C0C0702,  //  0003  EQ	R3	R3	K2
      0x780E0001,  //  0004  JMPF	R3	#0007
      0x540DFFFE,  //  0005  LDINT	R3	-1
      0x80040600,  //  0006  RET	1	R3
      0x600C000C,  //  0007  GETGBL	R3	G12
      0x88100100,  //  0008  GETMBR	R4	R0	K0
      0x7C0C0200,  //  0009  CALL	R3	1
      0x1C0C0703,  //  000A  EQ	R3	R3	K3
      0x780E0006,  //  000B  JMPF	R3	#0013
      0x880C0100,  //  000C  GETMBR	R3	R0	K0
      0x940C0702,  //  000D  GETIDX	R3	R3	K2
      0x8C0C0706,  //  000E  GETMET	R3	R3	K6
      0x5C140200,  //  000F  MOVE	R5	R1
      0x5C180400,  //  0010  MOVE	R6	R2
      0x7C0C0600,  //  0011  CALL	R3	3
      0x80040600,  //  0012  RET	1	R3
      0x880C0100,  //  0013  GETMBR	R3	R0	K0
      0x940C0702,  //  0014  GETIDX	R3	R3	K2
      0x8C0C0706,  //  0015  GETMET	R3	R3	K6
      0x5C140200,  //  0016  MOVE	R5	R1
      0x5C180400,  //  0017  MOVE	R6	R2
      0x7C0C0600,  //  0018  CALL	R3	3
      0x58100003,  //  0019  LDCONST	R4	K3
      0x6014000C,  //  001A  GETGBL	R5	G12
      0x88180100,  //  001B  GETMBR	R6	R0	K0
      0x7C140200,  //  001C  CALL	R5	1
      0x14140805,  //  001D  LT	R5	R4	R5
      0x7816000C,  //  001E  JMPF	R5	#002C
      0x88140100,  //  001F  GETMBR	R5	R0	K0
      0x94140A04,  //  0020  GETIDX	R5	R5	R4
      0x8C140B06,  //  0021  GETMET	R5	R5	K6
      0x5C1C0200,  //  0022  MOVE	R7	R1
      0x5C200400,  //  0023  MOVE	R8	R2
      0x7C140600,  //  0024  CALL	R5	3
      0x8C180105,  //  0025  GETMET	R6	R0	K5
      0x5C200600,  //  0026  MOVE	R8	R3
      0x5C240A00,  //  0027  MOVE	R9	R5
      0x7C180600,  //  0028  CALL	R6	3
      0x5C0C0C00,  //  0029  MOVE	R3	R6
      0x00100903,  //  002A  ADD	R4	R4	K3
      0x7001FFED,  //  002B  JMP		#001A
      0x80040600,  //  002C  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_CompositeColorProvider_tostring,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_CompositeColorProvider,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0xA802000A,  //  0000  EXBLK	0	#000C
      0x60040018,  //  0001  GETGBL	R1	G24
      0x58080007,  //  0002  LDCONST	R2	K7
      0x600C000C,  //  0003  GETGBL	R3	G12
      0x88100100,  //  0004  GETMBR	R4	R0	K0
      0x7C0C0200,  //  0005  CALL	R3	1
      0x88100108,  //  0006  GETMBR	R4	R0	K8
      0x7C040600,  //  0007  CALL	R1	3
      0xA8040001,  //  0008  EXBLK	1	1
      0x80040200,  //  0009  RET	1	R1
      0xA8040001,  //  000A  EXBLK	1	1
      0x70020004,  //  000B  JMP		#0011
      0xAC040000,  //  000C  CATCH	R1	0	0
      0x70020001,  //  000D  JMP		#0010
      0x80061200,  //  000E  RET	1	K9
      0x70020000,  //  000F  JMP		#0011
      0xB0080000,  //  0010  RAISE	2	R0	R0
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _blend_colors
********************************************************************/
be_local_closure(class_CompositeColorProvider__blend_colors,   /* name */
  be_nested_proto(
    23,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_CompositeColorProvider,     /* shared constants */
    be_str_weak(_blend_colors),
    &be_const_str_solidified,
    ( &(const binstruction[151]) {  /* code */
      0x880C0108,  //  0000  GETMBR	R3	R0	K8
      0x54120017,  //  0001  LDINT	R4	24
      0x3C100204,  //  0002  SHR	R4	R1	R4
      0x541600FE,  //  0003  LDINT	R5	255
      0x2C100805,  //  0004  AND	R4	R4	R5
      0x5416000F,  //  0005  LDINT	R5	16
      0x3C140205,  //  0006  SHR	R5	R1	R5
      0x541A00FE,  //  0007  LDINT	R6	255
      0x2C140A06,  //  0008  AND	R5	R5	R6
      0x541A0007,  //  0009  LDINT	R6	8
      0x3C180206,  //  000A  SHR	R6	R1	R6
      0x541E00FE,  //  000B  LDINT	R7	255
      0x2C180C07,  //  000C  AND	R6	R6	R7
      0x541E00FE,  //  000D  LDINT	R7	255
      0x2C1C0207,  //  000E  AND	R7	R1	R7
      0x54220017,  //  000F  LDINT	R8	24
      0x3C200408,  //  0010  SHR	R8	R2	R8
      0x542600FE,  //  0011  LDINT	R9	255
      0x2C201009,  //  0012  AND	R8	R8	R9
      0x5426000F,  //  0013  LDINT	R9	16
      0x3C240409,  //  0014  SHR	R9	R2	R9
      0x542A00FE,  //  0015  LDINT	R10	255
      0x2C24120A,  //  0016  AND	R9	R9	R10
      0x542A0007,  //  0017  LDINT	R10	8
      0x3C28040A,  //  0018  SHR	R10	R2	R10
      0x542E00FE,  //  0019  LDINT	R11	255
      0x2C28140B,  //  001A  AND	R10	R10	R11
      0x542E00FE,  //  001B  LDINT	R11	255
      0x2C2C040B,  //  001C  AND	R11	R2	R11
      0x4C300000,  //  001D  LDNIL	R12
      0x4C340000,  //  001E  LDNIL	R13
      0x4C380000,  //  001F  LDNIL	R14
      0x4C3C0000,  //  0020  LDNIL	R15
      0x1C400702,  //  0021  EQ	R16	R3	K2
      0x7842001C,  //  0022  JMPF	R16	#0040
      0x0C40110A,  //  0023  DIV	R16	R8	K10
      0x60440009,  //  0024  GETGBL	R17	G9
      0x044A0610,  //  0025  SUB	R18	K3	R16
      0x08480E12,  //  0026  MUL	R18	R7	R18
      0x084C1610,  //  0027  MUL	R19	R11	R16
      0x00482413,  //  0028  ADD	R18	R18	R19
      0x7C440200,  //  0029  CALL	R17	1
      0x5C342200,  //  002A  MOVE	R13	R17
      0x60440009,  //  002B  GETGBL	R17	G9
      0x044A0610,  //  002C  SUB	R18	K3	R16
      0x08480C12,  //  002D  MUL	R18	R6	R18
      0x084C1410,  //  002E  MUL	R19	R10	R16
      0x00482413,  //  002F  ADD	R18	R18	R19
      0x7C440200,  //  0030  CALL	R17	1
      0x5C382200,  //  0031  MOVE	R14	R17
      0x60440009,  //  0032  GETGBL	R17	G9
      0x044A0610,  //  0033  SUB	R18	K3	R16
      0x08480A12,  //  0034  MUL	R18	R5	R18
      0x084C1210,  //  0035  MUL	R19	R9	R16
      0x00482413,  //  0036  ADD	R18	R18	R19
      0x7C440200,  //  0037  CALL	R17	1
      0x5C3C2200,  //  0038  MOVE	R15	R17
      0x24440808,  //  0039  GT	R17	R4	R8
      0x78460001,  //  003A  JMPF	R17	#003D
      0x5C440800,  //  003B  MOVE	R17	R4
      0x70020000,  //  003C  JMP		#003E
      0x5C441000,  //  003D  MOVE	R17	R8
      0x5C302200,  //  003E  MOVE	R12	R17
      0x7002004C,  //  003F  JMP		#008D
      0x1C400703,  //  0040  EQ	R16	R3	K3
      0x78420021,  //  0041  JMPF	R16	#0064
      0x00400E0B,  //  0042  ADD	R16	R7	R11
      0x5C342000,  //  0043  MOVE	R13	R16
      0x00400C0A,  //  0044  ADD	R16	R6	R10
      0x5C382000,  //  0045  MOVE	R14	R16
      0x00400A09,  //  0046  ADD	R16	R5	R9
      0x5C3C2000,  //  0047  MOVE	R15	R16
      0x24400808,  //  0048  GT	R16	R4	R8
      0x78420001,  //  0049  JMPF	R16	#004C
      0x5C400800,  //  004A  MOVE	R16	R4
      0x70020000,  //  004B  JMP		#004D
      0x5C401000,  //  004C  MOVE	R16	R8
      0x5C302000,  //  004D  MOVE	R12	R16
      0x544200FE,  //  004E  LDINT	R16	255
      0x24401A10,  //  004F  GT	R16	R13	R16
      0x78420001,  //  0050  JMPF	R16	#0053
      0x544200FE,  //  0051  LDINT	R16	255
      0x70020000,  //  0052  JMP		#0054
      0x5C401A00,  //  0053  MOVE	R16	R13
      0x5C342000,  //  0054  MOVE	R13	R16
      0x544200FE,  //  0055  LDINT	R16	255
      0x24401C10,  //  0056  GT	R16	R14	R16
      0x78420001,  //  0057  JMPF	R16	#005A
      0x544200FE,  //  0058  LDINT	R16	255
      0x70020000,  //  0059  JMP		#005B
      0x5C401C00,  //  005A  MOVE	R16	R14
      0x5C382000,  //  005B  MOVE	R14	R16
      0x544200FE,  //  005C  LDINT	R16	255
      0x24401E10,  //  005D  GT	R16	R15	R16
      0x78420001,  //  005E  JMPF	R16	#0061
      0x544200FE,  //  005F  LDINT	R16	255
      0x70020000,  //  0060  JMP		#0062
      0x5C401E00,  //  0061  MOVE	R16	R15
      0x5C3C2000,  //  0062  MOVE	R15	R16
      0x70020028,  //  0063  JMP		#008D
      0x1C40070B,  //  0064  EQ	R16	R3	K11
      0x78420026,  //  0065  JMPF	R16	#008D
      0xB8421800,  //  0066  GETNGBL	R16	K12
      0x8C40210D,  //  0067  GETMET	R16	R16	K13
      0x08480E0B,  //  0068  MUL	R18	R7	R11
      0x584C0002,  //  0069  LDCONST	R19	K2
      0x545200FE,  //  006A  LDINT	R20	255
      0x545600FE,  //  006B  LDINT	R21	255
      0x08502815,  //  006C  MUL	R20	R20	R21
      0x58540002,  //  006D  LDCONST	R21	K2
      0x545A00FE,  //  006E  LDINT	R22	255
      0x7C400C00,  //  006F  CALL	R16	6
      0x5C342000,  //  0070  MOVE	R13	R16
      0xB8421800,  //  0071  GETNGBL	R16	K12
      0x8C40210D,  //  0072  GETMET	R16	R16	K13
      0x08480C0A,  //  0073  MUL	R18	R6	R10
      0x584C0002,  //  0074  LDCONST	R19	K2
      0x545200FE,  //  0075  LDINT	R20	255
      0x545600FE,  //  0076  LDINT	R21	255
      0x08502815,  //  0077  MUL	R20	R20	R21
      0x58540002,  //  0078  LDCONST	R21	K2
      0x545A00FE,  //  0079  LDINT	R22	255
      0x7C400C00,  //  007A  CALL	R16	6
      0x5C382000,  //  007B  MOVE	R14	R16
      0xB8421800,  //  007C  GETNGBL	R16	K12
      0x8C40210D,  //  007D  GETMET	R16	R16	K13
      0x08480A09,  //  007E  MUL	R18	R5	R9
      0x584C0002,  //  007F  LDCONST	R19	K2
      0x545200FE,  //  0080  LDINT	R20	255
      0x545600FE,  //  0081  LDINT	R21	255
      0x08502815,  //  0082  MUL	R20	R20	R21
      0x58540002,  //  0083  LDCONST	R21	K2
      0x545A00FE,  //  0084  LDINT	R22	255
      0x7C400C00,  //  0085  CALL	R16	6
      0x5C3C2000,  //  0086  MOVE	R15	R16
      0x24400808,  //  0087  GT	R16	R4	R8
      0x78420001,  //  0088  JMPF	R16	#008B
      0x5C400800,  //  0089  MOVE	R16	R4
      0x70020000,  //  008A  JMP		#008C
      0x5C401000,  //  008B  MOVE	R16	R8
      0x5C302000,  //  008C  MOVE	R12	R16
      0x54420017,  //  008D  LDINT	R16	24
      0x38401810,  //  008E  SHL	R16	R12	R16
      0x5446000F,  //  008F  LDINT	R17	16
      0x38441E11,  //  0090  SHL	R17	R15	R17
      0x30402011,  //  0091  OR	R16	R16	R17
      0x54460007,  //  0092  LDINT	R17	8
      0x38441C11,  //  0093  SHL	R17	R14	R17
      0x30402011,  //  0094  OR	R16	R16	R17
      0x3040200D,  //  0095  OR	R16	R16	R13
      0x80042000,  //  0096  RET	1	R16
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_CompositeColorProvider_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_CompositeColorProvider,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C08050E,  //  0003  GETMET	R2	R2	K14
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x60080012,  //  0006  GETGBL	R2	G18
      0x7C080000,  //  0007  CALL	R2	0
      0x90020002,  //  0008  SETMBR	R0	K0	R2
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: CompositeColorProvider
********************************************************************/
extern const bclass be_class_ColorProvider;
be_local_class(CompositeColorProvider,
    1,
    &be_class_ColorProvider,
    be_nested_map(8,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(add_provider, -1), be_const_closure(class_CompositeColorProvider_add_provider_closure) },
        { be_const_key_weak(get_color_for_value, 7), be_const_closure(class_CompositeColorProvider_get_color_for_value_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_CompositeColorProvider_init_closure) },
        { be_const_key_weak(PARAMS, 2), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(blend_mode, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(enum, 1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
    }))    ) } )) },
        { be_const_key_weak(default, -1), be_const_int(0) },
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(tostring, -1), be_const_closure(class_CompositeColorProvider_tostring_closure) },
        { be_const_key_weak(providers, 4), be_const_var(0) },
        { be_const_key_weak(_blend_colors, -1), be_const_closure(class_CompositeColorProvider__blend_colors_closure) },
        { be_const_key_weak(produce_value, -1), be_const_closure(class_CompositeColorProvider_produce_value_closure) },
    })),
    be_str_weak(CompositeColorProvider)
);

/********************************************************************
** Solidified function: solid
********************************************************************/
be_local_closure(solid,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(name),
    /* K2   */  be_nested_str_weak(solid),
    }),
    be_str_weak(solid),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040300,  //  0001  GETMET	R1	R1	K0
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x90060302,  //  0004  SETMBR	R1	K1	K2
      0x80040200,  //  0005  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_engine
********************************************************************/
be_local_closure(create_engine,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(animation_engine),
    }),
    be_str_weak(create_engine),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x80040200,  //  0004  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: smooth
********************************************************************/
be_local_closure(smooth,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(oscillator_value),
    /* K2   */  be_nested_str_weak(form),
    /* K3   */  be_nested_str_weak(COSINE),
    }),
    be_str_weak(smooth),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0xB80A0000,  //  0004  GETNGBL	R2	K0
      0x88080503,  //  0005  GETMBR	R2	R2	K3
      0x90060402,  //  0006  SETMBR	R1	K2	R2
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: cosine_osc
********************************************************************/
be_local_closure(cosine_osc,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(oscillator_value),
    /* K2   */  be_nested_str_weak(form),
    /* K3   */  be_nested_str_weak(COSINE),
    }),
    be_str_weak(cosine_osc),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0xB80A0000,  //  0004  GETNGBL	R2	K0
      0x88080503,  //  0005  GETMBR	R2	R2	K3
      0x90060402,  //  0006  SETMBR	R1	K2	R2
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_color_provider
********************************************************************/
be_local_closure(is_color_provider,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(color_provider),
    }),
    be_str_weak(is_color_provider),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x6004000F,  //  0000  GETGBL	R1	G15
      0x5C080000,  //  0001  MOVE	R2	R0
      0xB80E0000,  //  0002  GETNGBL	R3	K0
      0x880C0701,  //  0003  GETMBR	R3	R3	K1
      0x7C040400,  //  0004  CALL	R1	2
      0x80040200,  //  0005  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_event_handlers
********************************************************************/
be_local_closure(get_event_handlers,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(event_manager),
    /* K2   */  be_nested_str_weak(get_handlers),
    }),
    be_str_weak(get_event_handlers),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x8C040302,  //  0002  GETMET	R1	R1	K2
      0x5C0C0000,  //  0003  MOVE	R3	R0
      0x7C040400,  //  0004  CALL	R1	2
      0x80040200,  //  0005  RET	1	R1
    })
  )
);
/*******************************************************************/

// compact class 'WaveAnimation' ktab size: 41, total: 69 (saved 224 bytes)
static const bvalue be_ktab_class_WaveAnimation[41] = {
  /* K0   */  be_nested_str_weak(wave_table),
  /* K1   */  be_nested_str_weak(resize),
  /* K2   */  be_nested_str_weak(wave_type),
  /* K3   */  be_const_int(0),
  /* K4   */  be_nested_str_weak(tasmota),
  /* K5   */  be_nested_str_weak(scale_uint),
  /* K6   */  be_const_int(1),
  /* K7   */  be_const_int(2),
  /* K8   */  be_nested_str_weak(sine),
  /* K9   */  be_nested_str_weak(triangle),
  /* K10  */  be_nested_str_weak(square),
  /* K11  */  be_nested_str_weak(sawtooth),
  /* K12  */  be_nested_str_weak(unknown),
  /* K13  */  be_nested_str_weak(color),
  /* K14  */  be_nested_str_weak(animation),
  /* K15  */  be_nested_str_weak(is_value_provider),
  /* K16  */  be_nested_str_weak(0x_X2508x),
  /* K17  */  be_nested_str_weak(WaveAnimation_X28_X25s_X2C_X20color_X3D_X25s_X2C_X20freq_X3D_X25s_X2C_X20speed_X3D_X25s_X2C_X20priority_X3D_X25s_X2C_X20running_X3D_X25s_X29),
  /* K18  */  be_nested_str_weak(frequency),
  /* K19  */  be_nested_str_weak(wave_speed),
  /* K20  */  be_nested_str_weak(priority),
  /* K21  */  be_nested_str_weak(is_running),
  /* K22  */  be_nested_str_weak(engine),
  /* K23  */  be_nested_str_weak(get_strip_length),
  /* K24  */  be_nested_str_weak(width),
  /* K25  */  be_nested_str_weak(current_colors),
  /* K26  */  be_nested_str_weak(size),
  /* K27  */  be_nested_str_weak(set_pixel_color),
  /* K28  */  be_nested_str_weak(_init_wave_table),
  /* K29  */  be_nested_str_weak(update),
  /* K30  */  be_nested_str_weak(start_time),
  /* K31  */  be_nested_str_weak(time_offset),
  /* K32  */  be_nested_str_weak(_calculate_wave),
  /* K33  */  be_nested_str_weak(init),
  /* K34  */  be_nested_str_weak(phase),
  /* K35  */  be_nested_str_weak(amplitude),
  /* K36  */  be_nested_str_weak(center_level),
  /* K37  */  be_nested_str_weak(back_color),
  /* K38  */  be_nested_str_weak(is_color_provider),
  /* K39  */  be_nested_str_weak(get_color_for_value),
  /* K40  */  be_nested_str_weak(resolve_value),
};


extern const bclass be_class_WaveAnimation;

/********************************************************************
** Solidified function: _init_wave_table
********************************************************************/
be_local_closure(class_WaveAnimation__init_wave_table,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_WaveAnimation,     /* shared constants */
    be_str_weak(_init_wave_table),
    &be_const_str_solidified,
    ( &(const binstruction[108]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x540E00FF,  //  0002  LDINT	R3	256
      0x7C040400,  //  0003  CALL	R1	2
      0x88040102,  //  0004  GETMBR	R1	R0	K2
      0x58080003,  //  0005  LDCONST	R2	K3
      0x540E00FF,  //  0006  LDINT	R3	256
      0x140C0403,  //  0007  LT	R3	R2	R3
      0x780E0061,  //  0008  JMPF	R3	#006B
      0x580C0003,  //  0009  LDCONST	R3	K3
      0x1C100303,  //  000A  EQ	R4	R1	K3
      0x78120035,  //  000B  JMPF	R4	#0042
      0x5412003F,  //  000C  LDINT	R4	64
      0x10100404,  //  000D  MOD	R4	R2	R4
      0x5416003F,  //  000E  LDINT	R5	64
      0x14140405,  //  000F  LT	R5	R2	R5
      0x78160009,  //  0010  JMPF	R5	#001B
      0xB8160800,  //  0011  GETNGBL	R5	K4
      0x8C140B05,  //  0012  GETMET	R5	R5	K5
      0x5C1C0800,  //  0013  MOVE	R7	R4
      0x58200003,  //  0014  LDCONST	R8	K3
      0x5426003F,  //  0015  LDINT	R9	64
      0x542A007F,  //  0016  LDINT	R10	128
      0x542E00FE,  //  0017  LDINT	R11	255
      0x7C140C00,  //  0018  CALL	R5	6
      0x5C0C0A00,  //  0019  MOVE	R3	R5
      0x70020025,  //  001A  JMP		#0041
      0x5416007F,  //  001B  LDINT	R5	128
      0x14140405,  //  001C  LT	R5	R2	R5
      0x7816000A,  //  001D  JMPF	R5	#0029
      0xB8160800,  //  001E  GETNGBL	R5	K4
      0x8C140B05,  //  001F  GETMET	R5	R5	K5
      0x541E007F,  //  0020  LDINT	R7	128
      0x041C0E02,  //  0021  SUB	R7	R7	R2
      0x58200003,  //  0022  LDCONST	R8	K3
      0x5426003F,  //  0023  LDINT	R9	64
      0x542A007F,  //  0024  LDINT	R10	128
      0x542E00FE,  //  0025  LDINT	R11	255
      0x7C140C00,  //  0026  CALL	R5	6
      0x5C0C0A00,  //  0027  MOVE	R3	R5
      0x70020017,  //  0028  JMP		#0041
      0x541600BF,  //  0029  LDINT	R5	192
      0x14140405,  //  002A  LT	R5	R2	R5
      0x7816000A,  //  002B  JMPF	R5	#0037
      0xB8160800,  //  002C  GETNGBL	R5	K4
      0x8C140B05,  //  002D  GETMET	R5	R5	K5
      0x541E007F,  //  002E  LDINT	R7	128
      0x041C0407,  //  002F  SUB	R7	R2	R7
      0x58200003,  //  0030  LDCONST	R8	K3
      0x5426003F,  //  0031  LDINT	R9	64
      0x542A007F,  //  0032  LDINT	R10	128
      0x582C0003,  //  0033  LDCONST	R11	K3
      0x7C140C00,  //  0034  CALL	R5	6
      0x5C0C0A00,  //  0035  MOVE	R3	R5
      0x70020009,  //  0036  JMP		#0041
      0xB8160800,  //  0037  GETNGBL	R5	K4
      0x8C140B05,  //  0038  GETMET	R5	R5	K5
      0x541E00FF,  //  0039  LDINT	R7	256
      0x041C0E02,  //  003A  SUB	R7	R7	R2
      0x58200003,  //  003B  LDCONST	R8	K3
      0x5426003F,  //  003C  LDINT	R9	64
      0x542A007F,  //  003D  LDINT	R10	128
      0x582C0003,  //  003E  LDCONST	R11	K3
      0x7C140C00,  //  003F  CALL	R5	6
      0x5C0C0A00,  //  0040  MOVE	R3	R5
      0x70020024,  //  0041  JMP		#0067
      0x1C100306,  //  0042  EQ	R4	R1	K6
      0x78120017,  //  0043  JMPF	R4	#005C
      0x5412007F,  //  0044  LDINT	R4	128
      0x14100404,  //  0045  LT	R4	R2	R4
      0x78120009,  //  0046  JMPF	R4	#0051
      0xB8120800,  //  0047  GETNGBL	R4	K4
      0x8C100905,  //  0048  GETMET	R4	R4	K5
      0x5C180400,  //  0049  MOVE	R6	R2
      0x581C0003,  //  004A  LDCONST	R7	K3
      0x5422007F,  //  004B  LDINT	R8	128
      0x58240003,  //  004C  LDCONST	R9	K3
      0x542A00FE,  //  004D  LDINT	R10	255
      0x7C100C00,  //  004E  CALL	R4	6
      0x5C0C0800,  //  004F  MOVE	R3	R4
      0x70020009,  //  0050  JMP		#005B
      0xB8120800,  //  0051  GETNGBL	R4	K4
      0x8C100905,  //  0052  GETMET	R4	R4	K5
      0x541A00FF,  //  0053  LDINT	R6	256
      0x04180C02,  //  0054  SUB	R6	R6	R2
      0x581C0003,  //  0055  LDCONST	R7	K3
      0x5422007F,  //  0056  LDINT	R8	128
      0x58240003,  //  0057  LDCONST	R9	K3
      0x542A00FE,  //  0058  LDINT	R10	255
      0x7C100C00,  //  0059  CALL	R4	6
      0x5C0C0800,  //  005A  MOVE	R3	R4
      0x7002000A,  //  005B  JMP		#0067
      0x1C100307,  //  005C  EQ	R4	R1	K7
      0x78120007,  //  005D  JMPF	R4	#0066
      0x5412007F,  //  005E  LDINT	R4	128
      0x14100404,  //  005F  LT	R4	R2	R4
      0x78120001,  //  0060  JMPF	R4	#0063
      0x541200FE,  //  0061  LDINT	R4	255
      0x70020000,  //  0062  JMP		#0064
      0x58100003,  //  0063  LDCONST	R4	K3
      0x5C0C0800,  //  0064  MOVE	R3	R4
      0x70020000,  //  0065  JMP		#0067
      0x5C0C0400,  //  0066  MOVE	R3	R2
      0x88100100,  //  0067  GETMBR	R4	R0	K0
      0x98100403,  //  0068  SETIDX	R4	R2	R3
      0x00080506,  //  0069  ADD	R2	R2	K6
      0x7001FF9A,  //  006A  JMP		#0006
      0x80000000,  //  006B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_WaveAnimation_tostring,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_WaveAnimation,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[41]) {  /* code */
      0x60040012,  //  0000  GETGBL	R1	G18
      0x7C040000,  //  0001  CALL	R1	0
      0x40080308,  //  0002  CONNECT	R2	R1	K8
      0x40080309,  //  0003  CONNECT	R2	R1	K9
      0x4008030A,  //  0004  CONNECT	R2	R1	K10
      0x4008030B,  //  0005  CONNECT	R2	R1	K11
      0x88080102,  //  0006  GETMBR	R2	R0	K2
      0x940C0202,  //  0007  GETIDX	R3	R1	R2
      0x4C100000,  //  0008  LDNIL	R4
      0x200C0604,  //  0009  NE	R3	R3	R4
      0x780E0001,  //  000A  JMPF	R3	#000D
      0x940C0202,  //  000B  GETIDX	R3	R1	R2
      0x70020000,  //  000C  JMP		#000E
      0x580C000C,  //  000D  LDCONST	R3	K12
      0x8810010D,  //  000E  GETMBR	R4	R0	K13
      0x4C140000,  //  000F  LDNIL	R5
      0xB81A1C00,  //  0010  GETNGBL	R6	K14
      0x8C180D0F,  //  0011  GETMET	R6	R6	K15
      0x5C200800,  //  0012  MOVE	R8	R4
      0x7C180400,  //  0013  CALL	R6	2
      0x781A0004,  //  0014  JMPF	R6	#001A
      0x60180008,  //  0015  GETGBL	R6	G8
      0x5C1C0800,  //  0016  MOVE	R7	R4
      0x7C180200,  //  0017  CALL	R6	1
      0x5C140C00,  //  0018  MOVE	R5	R6
      0x70020004,  //  0019  JMP		#001F
      0x60180018,  //  001A  GETGBL	R6	G24
      0x581C0010,  //  001B  LDCONST	R7	K16
      0x5C200800,  //  001C  MOVE	R8	R4
      0x7C180400,  //  001D  CALL	R6	2
      0x5C140C00,  //  001E  MOVE	R5	R6
      0x60180018,  //  001F  GETGBL	R6	G24
      0x581C0011,  //  0020  LDCONST	R7	K17
      0x5C200600,  //  0021  MOVE	R8	R3
      0x5C240A00,  //  0022  MOVE	R9	R5
      0x88280112,  //  0023  GETMBR	R10	R0	K18
      0x882C0113,  //  0024  GETMBR	R11	R0	K19
      0x88300114,  //  0025  GETMBR	R12	R0	K20
      0x88340115,  //  0026  GETMBR	R13	R0	K21
      0x7C180E00,  //  0027  CALL	R6	7
      0x80040C00,  //  0028  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: render
********************************************************************/
be_local_closure(class_WaveAnimation_render,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_WaveAnimation,     /* shared constants */
    be_str_weak(render),
    &be_const_str_solidified,
    ( &(const binstruction[30]) {  /* code */
      0x880C0115,  //  0000  GETMBR	R3	R0	K21
      0x780E0002,  //  0001  JMPF	R3	#0005
      0x4C0C0000,  //  0002  LDNIL	R3
      0x1C0C0203,  //  0003  EQ	R3	R1	R3
      0x780E0001,  //  0004  JMPF	R3	#0007
      0x500C0000,  //  0005  LDBOOL	R3	0	0
      0x80040600,  //  0006  RET	1	R3
      0x880C0116,  //  0007  GETMBR	R3	R0	K22
      0x8C0C0717,  //  0008  GETMET	R3	R3	K23
      0x7C0C0200,  //  0009  CALL	R3	1
      0x58100003,  //  000A  LDCONST	R4	K3
      0x14140803,  //  000B  LT	R5	R4	R3
      0x7816000E,  //  000C  JMPF	R5	#001C
      0x88140318,  //  000D  GETMBR	R5	R1	K24
      0x14140805,  //  000E  LT	R5	R4	R5
      0x78160009,  //  000F  JMPF	R5	#001A
      0x88140119,  //  0010  GETMBR	R5	R0	K25
      0x8C140B1A,  //  0011  GETMET	R5	R5	K26
      0x7C140200,  //  0012  CALL	R5	1
      0x14140805,  //  0013  LT	R5	R4	R5
      0x78160004,  //  0014  JMPF	R5	#001A
      0x8C14031B,  //  0015  GETMET	R5	R1	K27
      0x5C1C0800,  //  0016  MOVE	R7	R4
      0x88200119,  //  0017  GETMBR	R8	R0	K25
      0x94201004,  //  0018  GETIDX	R8	R8	R4
      0x7C140600,  //  0019  CALL	R5	3
      0x00100906,  //  001A  ADD	R4	R4	K6
      0x7001FFEE,  //  001B  JMP		#000B
      0x50140200,  //  001C  LDBOOL	R5	1	0
      0x80040A00,  //  001D  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: on_param_changed
********************************************************************/
be_local_closure(class_WaveAnimation_on_param_changed,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_WaveAnimation,     /* shared constants */
    be_str_weak(on_param_changed),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x1C0C0302,  //  0000  EQ	R3	R1	K2
      0x780E0001,  //  0001  JMPF	R3	#0004
      0x8C0C011C,  //  0002  GETMET	R3	R0	K28
      0x7C0C0200,  //  0003  CALL	R3	1
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update
********************************************************************/
be_local_closure(class_WaveAnimation_update,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_WaveAnimation,     /* shared constants */
    be_str_weak(update),
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C08051D,  //  0003  GETMET	R2	R2	K29
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x740A0001,  //  0006  JMPT	R2	#0009
      0x50080000,  //  0007  LDBOOL	R2	0	0
      0x80040400,  //  0008  RET	1	R2
      0x88080113,  //  0009  GETMBR	R2	R0	K19
      0x240C0503,  //  000A  GT	R3	R2	K3
      0x780E0011,  //  000B  JMPF	R3	#001E
      0x880C011E,  //  000C  GETMBR	R3	R0	K30
      0x040C0203,  //  000D  SUB	R3	R1	R3
      0xB8120800,  //  000E  GETNGBL	R4	K4
      0x8C100905,  //  000F  GETMET	R4	R4	K5
      0x5C180400,  //  0010  MOVE	R6	R2
      0x581C0003,  //  0011  LDCONST	R7	K3
      0x542200FE,  //  0012  LDINT	R8	255
      0x58240003,  //  0013  LDCONST	R9	K3
      0x542A0009,  //  0014  LDINT	R10	10
      0x7C100C00,  //  0015  CALL	R4	6
      0x24140903,  //  0016  GT	R5	R4	K3
      0x78160005,  //  0017  JMPF	R5	#001E
      0x08140604,  //  0018  MUL	R5	R3	R4
      0x541A03E7,  //  0019  LDINT	R6	1000
      0x0C140A06,  //  001A  DIV	R5	R5	R6
      0x541A00FF,  //  001B  LDINT	R6	256
      0x10140A06,  //  001C  MOD	R5	R5	R6
      0x90023E05,  //  001D  SETMBR	R0	K31	R5
      0x8C0C0120,  //  001E  GETMET	R3	R0	K32
      0x5C140200,  //  001F  MOVE	R5	R1
      0x7C0C0400,  //  0020  CALL	R3	2
      0x500C0200,  //  0021  LDBOOL	R3	1	0
      0x80040600,  //  0022  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_WaveAnimation_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_WaveAnimation,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080521,  //  0003  GETMET	R2	R2	K33
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x60080012,  //  0006  GETGBL	R2	G18
      0x7C080000,  //  0007  CALL	R2	0
      0x90023202,  //  0008  SETMBR	R0	K25	R2
      0x90023F03,  //  0009  SETMBR	R0	K31	K3
      0x60080012,  //  000A  GETGBL	R2	G18
      0x7C080000,  //  000B  CALL	R2	0
      0x90020002,  //  000C  SETMBR	R0	K0	R2
      0x8C08011C,  //  000D  GETMET	R2	R0	K28
      0x7C080200,  //  000E  CALL	R2	1
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _calculate_wave
********************************************************************/
be_local_closure(class_WaveAnimation__calculate_wave,   /* name */
  be_nested_proto(
    27,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_WaveAnimation,     /* shared constants */
    be_str_weak(_calculate_wave),
    &be_const_str_solidified,
    ( &(const binstruction[169]) {  /* code */
      0x88080116,  //  0000  GETMBR	R2	R0	K22
      0x8C080517,  //  0001  GETMET	R2	R2	K23
      0x7C080200,  //  0002  CALL	R2	1
      0x880C0112,  //  0003  GETMBR	R3	R0	K18
      0x88100122,  //  0004  GETMBR	R4	R0	K34
      0x88140123,  //  0005  GETMBR	R5	R0	K35
      0x88180124,  //  0006  GETMBR	R6	R0	K36
      0x881C0125,  //  0007  GETMBR	R7	R0	K37
      0x8820010D,  //  0008  GETMBR	R8	R0	K13
      0x88240119,  //  0009  GETMBR	R9	R0	K25
      0x8C24131A,  //  000A  GETMET	R9	R9	K26
      0x7C240200,  //  000B  CALL	R9	1
      0x20241202,  //  000C  NE	R9	R9	R2
      0x78260003,  //  000D  JMPF	R9	#0012
      0x88240119,  //  000E  GETMBR	R9	R0	K25
      0x8C241301,  //  000F  GETMET	R9	R9	K1
      0x5C2C0400,  //  0010  MOVE	R11	R2
      0x7C240400,  //  0011  CALL	R9	2
      0x58240003,  //  0012  LDCONST	R9	K3
      0x14281202,  //  0013  LT	R10	R9	R2
      0x782A0092,  //  0014  JMPF	R10	#00A8
      0xB82A0800,  //  0015  GETNGBL	R10	K4
      0x8C281505,  //  0016  GETMET	R10	R10	K5
      0x5C301200,  //  0017  MOVE	R12	R9
      0x58340003,  //  0018  LDCONST	R13	K3
      0x04380506,  //  0019  SUB	R14	R2	K6
      0x583C0003,  //  001A  LDCONST	R15	K3
      0x544200FE,  //  001B  LDINT	R16	255
      0x7C280C00,  //  001C  CALL	R10	6
      0x082C1403,  //  001D  MUL	R11	R10	R3
      0x5432001F,  //  001E  LDINT	R12	32
      0x0C2C160C,  //  001F  DIV	R11	R11	R12
      0x002C1604,  //  0020  ADD	R11	R11	R4
      0x8830011F,  //  0021  GETMBR	R12	R0	K31
      0x002C160C,  //  0022  ADD	R11	R11	R12
      0x543200FE,  //  0023  LDINT	R12	255
      0x2C2C160C,  //  0024  AND	R11	R11	R12
      0x88300100,  //  0025  GETMBR	R12	R0	K0
      0x9430180B,  //  0026  GETIDX	R12	R12	R11
      0xB8360800,  //  0027  GETNGBL	R13	K4
      0x8C341B05,  //  0028  GETMET	R13	R13	K5
      0x5C3C0A00,  //  0029  MOVE	R15	R5
      0x58400003,  //  002A  LDCONST	R16	K3
      0x544600FE,  //  002B  LDINT	R17	255
      0x58480003,  //  002C  LDCONST	R18	K3
      0x544E007F,  //  002D  LDINT	R19	128
      0x7C340C00,  //  002E  CALL	R13	6
      0x58380003,  //  002F  LDCONST	R14	K3
      0x543E007F,  //  0030  LDINT	R15	128
      0x283C180F,  //  0031  GE	R15	R12	R15
      0x783E000D,  //  0032  JMPF	R15	#0041
      0x543E007F,  //  0033  LDINT	R15	128
      0x043C180F,  //  0034  SUB	R15	R12	R15
      0xB8420800,  //  0035  GETNGBL	R16	K4
      0x8C402105,  //  0036  GETMET	R16	R16	K5
      0x5C481E00,  //  0037  MOVE	R18	R15
      0x584C0003,  //  0038  LDCONST	R19	K3
      0x5452007E,  //  0039  LDINT	R20	127
      0x58540003,  //  003A  LDCONST	R21	K3
      0x5C581A00,  //  003B  MOVE	R22	R13
      0x7C400C00,  //  003C  CALL	R16	6
      0x5C3C2000,  //  003D  MOVE	R15	R16
      0x00400C0F,  //  003E  ADD	R16	R6	R15
      0x5C382000,  //  003F  MOVE	R14	R16
      0x7002000C,  //  0040  JMP		#004E
      0x543E007F,  //  0041  LDINT	R15	128
      0x043C1E0C,  //  0042  SUB	R15	R15	R12
      0xB8420800,  //  0043  GETNGBL	R16	K4
      0x8C402105,  //  0044  GETMET	R16	R16	K5
      0x5C481E00,  //  0045  MOVE	R18	R15
      0x584C0003,  //  0046  LDCONST	R19	K3
      0x5452007F,  //  0047  LDINT	R20	128
      0x58540003,  //  0048  LDCONST	R21	K3
      0x5C581A00,  //  0049  MOVE	R22	R13
      0x7C400C00,  //  004A  CALL	R16	6
      0x5C3C2000,  //  004B  MOVE	R15	R16
      0x04400C0F,  //  004C  SUB	R16	R6	R15
      0x5C382000,  //  004D  MOVE	R14	R16
      0x543E00FE,  //  004E  LDINT	R15	255
      0x243C1C0F,  //  004F  GT	R15	R14	R15
      0x783E0001,  //  0050  JMPF	R15	#0053
      0x543A00FE,  //  0051  LDINT	R14	255
      0x70020002,  //  0052  JMP		#0056
      0x143C1D03,  //  0053  LT	R15	R14	K3
      0x783E0000,  //  0054  JMPF	R15	#0056
      0x58380003,  //  0055  LDCONST	R14	K3
      0x5C3C0E00,  //  0056  MOVE	R15	R7
      0x54420009,  //  0057  LDINT	R16	10
      0x24401C10,  //  0058  GT	R16	R14	R16
      0x78420049,  //  0059  JMPF	R16	#00A4
      0xB8421C00,  //  005A  GETNGBL	R16	K14
      0x8C402126,  //  005B  GETMET	R16	R16	K38
      0x5C481000,  //  005C  MOVE	R18	R8
      0x7C400400,  //  005D  CALL	R16	2
      0x78420009,  //  005E  JMPF	R16	#0069
      0x88401127,  //  005F  GETMBR	R16	R8	K39
      0x4C440000,  //  0060  LDNIL	R17
      0x20402011,  //  0061  NE	R16	R16	R17
      0x78420005,  //  0062  JMPF	R16	#0069
      0x8C401127,  //  0063  GETMET	R16	R8	K39
      0x5C481C00,  //  0064  MOVE	R18	R14
      0x584C0003,  //  0065  LDCONST	R19	K3
      0x7C400600,  //  0066  CALL	R16	3
      0x5C3C2000,  //  0067  MOVE	R15	R16
      0x7002003A,  //  0068  JMP		#00A4
      0x8C400128,  //  0069  GETMET	R16	R0	K40
      0x5C481000,  //  006A  MOVE	R18	R8
      0x584C000D,  //  006B  LDCONST	R19	K13
      0x54520009,  //  006C  LDINT	R20	10
      0x08501C14,  //  006D  MUL	R20	R14	R20
      0x00500214,  //  006E  ADD	R20	R1	R20
      0x7C400800,  //  006F  CALL	R16	4
      0x5C3C2000,  //  0070  MOVE	R15	R16
      0x54420017,  //  0071  LDINT	R16	24
      0x3C401E10,  //  0072  SHR	R16	R15	R16
      0x544600FE,  //  0073  LDINT	R17	255
      0x2C402011,  //  0074  AND	R16	R16	R17
      0x5446000F,  //  0075  LDINT	R17	16
      0x3C441E11,  //  0076  SHR	R17	R15	R17
      0x544A00FE,  //  0077  LDINT	R18	255
      0x2C442212,  //  0078  AND	R17	R17	R18
      0x544A0007,  //  0079  LDINT	R18	8
      0x3C481E12,  //  007A  SHR	R18	R15	R18
      0x544E00FE,  //  007B  LDINT	R19	255
      0x2C482413,  //  007C  AND	R18	R18	R19
      0x544E00FE,  //  007D  LDINT	R19	255
      0x2C4C1E13,  //  007E  AND	R19	R15	R19
      0xB8520800,  //  007F  GETNGBL	R20	K4
      0x8C502905,  //  0080  GETMET	R20	R20	K5
      0x5C581C00,  //  0081  MOVE	R22	R14
      0x585C0003,  //  0082  LDCONST	R23	K3
      0x546200FE,  //  0083  LDINT	R24	255
      0x58640003,  //  0084  LDCONST	R25	K3
      0x5C682200,  //  0085  MOVE	R26	R17
      0x7C500C00,  //  0086  CALL	R20	6
      0x5C442800,  //  0087  MOVE	R17	R20
      0xB8520800,  //  0088  GETNGBL	R20	K4
      0x8C502905,  //  0089  GETMET	R20	R20	K5
      0x5C581C00,  //  008A  MOVE	R22	R14
      0x585C0003,  //  008B  LDCONST	R23	K3
      0x546200FE,  //  008C  LDINT	R24	255
      0x58640003,  //  008D  LDCONST	R25	K3
      0x5C682400,  //  008E  MOVE	R26	R18
      0x7C500C00,  //  008F  CALL	R20	6
      0x5C482800,  //  0090  MOVE	R18	R20
      0xB8520800,  //  0091  GETNGBL	R20	K4
      0x8C502905,  //  0092  GETMET	R20	R20	K5
      0x5C581C00,  //  0093  MOVE	R22	R14
      0x585C0003,  //  0094  LDCONST	R23	K3
      0x546200FE,  //  0095  LDINT	R24	255
      0x58640003,  //  0096  LDCONST	R25	K3
      0x5C682600,  //  0097  MOVE	R26	R19
      0x7C500C00,  //  0098  CALL	R20	6
      0x5C4C2800,  //  0099  MOVE	R19	R20
      0x54520017,  //  009A  LDINT	R20	24
      0x38502014,  //  009B  SHL	R20	R16	R20
      0x5456000F,  //  009C  LDINT	R21	16
      0x38542215,  //  009D  SHL	R21	R17	R21
      0x30502815,  //  009E  OR	R20	R20	R21
      0x54560007,  //  009F  LDINT	R21	8
      0x38542415,  //  00A0  SHL	R21	R18	R21
      0x30502815,  //  00A1  OR	R20	R20	R21
      0x30502813,  //  00A2  OR	R20	R20	R19
      0x5C3C2800,  //  00A3  MOVE	R15	R20
      0x88400119,  //  00A4  GETMBR	R16	R0	K25
      0x9840120F,  //  00A5  SETIDX	R16	R9	R15
      0x00241306,  //  00A6  ADD	R9	R9	K6
      0x7001FF6A,  //  00A7  JMP		#0013
      0x80000000,  //  00A8  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: WaveAnimation
********************************************************************/
extern const bclass be_class_Animation;
be_local_class(WaveAnimation,
    3,
    &be_class_Animation,
    be_nested_map(11,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(current_colors, -1), be_const_var(0) },
        { be_const_key_weak(update, -1), be_const_closure(class_WaveAnimation_update_closure) },
        { be_const_key_weak(wave_table, -1), be_const_var(2) },
        { be_const_key_weak(tostring, -1), be_const_closure(class_WaveAnimation_tostring_closure) },
        { be_const_key_weak(render, 7), be_const_closure(class_WaveAnimation_render_closure) },
        { be_const_key_weak(on_param_changed, -1), be_const_closure(class_WaveAnimation_on_param_changed_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_WaveAnimation_init_closure) },
        { be_const_key_weak(time_offset, 1), be_const_var(1) },
        { be_const_key_weak(PARAMS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(8,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(phase, 3), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(0) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(frequency, 5), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(32) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(amplitude, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(128) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(color, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(-65536) },
    }))    ) } )) },
        { be_const_key_weak(wave_speed, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(50) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(wave_type, 7), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(0) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(3) },
    }))    ) } )) },
        { be_const_key_weak(back_color, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(-16777216) },
    }))    ) } )) },
        { be_const_key_weak(center_level, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(128) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(_init_wave_table, 6), be_const_closure(class_WaveAnimation__init_wave_table_closure) },
        { be_const_key_weak(_calculate_wave, -1), be_const_closure(class_WaveAnimation__calculate_wave_closure) },
    })),
    be_str_weak(WaveAnimation)
);
// compact class 'BreatheColorProvider' ktab size: 18, total: 29 (saved 88 bytes)
static const bvalue be_ktab_class_BreatheColorProvider[18] = {
  /* K0   */  be_nested_str_weak(BreatheColorProvider_X28base_color_X3D0x_X2508x_X2C_X20min_brightness_X3D_X25s_X2C_X20max_brightness_X3D_X25s_X2C_X20duration_X3D_X25s_X2C_X20curve_factor_X3D_X25s_X29),
  /* K1   */  be_nested_str_weak(base_color),
  /* K2   */  be_nested_str_weak(min_brightness),
  /* K3   */  be_nested_str_weak(max_brightness),
  /* K4   */  be_nested_str_weak(duration),
  /* K5   */  be_nested_str_weak(curve_factor),
  /* K6   */  be_const_int(1),
  /* K7   */  be_nested_str_weak(form),
  /* K8   */  be_nested_str_weak(animation),
  /* K9   */  be_nested_str_weak(COSINE),
  /* K10  */  be_nested_str_weak(on_param_changed),
  /* K11  */  be_nested_str_weak(produce_value),
  /* K12  */  be_nested_str_weak(tasmota),
  /* K13  */  be_nested_str_weak(scale_uint),
  /* K14  */  be_const_int(0),
  /* K15  */  be_nested_str_weak(init),
  /* K16  */  be_nested_str_weak(min_value),
  /* K17  */  be_nested_str_weak(max_value),
};


extern const bclass be_class_BreatheColorProvider;

/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_BreatheColorProvider_tostring,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_BreatheColorProvider,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x60040018,  //  0000  GETGBL	R1	G24
      0x58080000,  //  0001  LDCONST	R2	K0
      0x880C0101,  //  0002  GETMBR	R3	R0	K1
      0x88100102,  //  0003  GETMBR	R4	R0	K2
      0x88140103,  //  0004  GETMBR	R5	R0	K3
      0x88180104,  //  0005  GETMBR	R6	R0	K4
      0x881C0105,  //  0006  GETMBR	R7	R0	K5
      0x7C040C00,  //  0007  CALL	R1	6
      0x80040200,  //  0008  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: on_param_changed
********************************************************************/
be_local_closure(class_BreatheColorProvider_on_param_changed,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_BreatheColorProvider,     /* shared constants */
    be_str_weak(on_param_changed),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x1C0C0305,  //  0000  EQ	R3	R1	K5
      0x780E0008,  //  0001  JMPF	R3	#000B
      0x1C0C0506,  //  0002  EQ	R3	R2	K6
      0x780E0003,  //  0003  JMPF	R3	#0008
      0xB80E1000,  //  0004  GETNGBL	R3	K8
      0x880C0709,  //  0005  GETMBR	R3	R3	K9
      0x90020E03,  //  0006  SETMBR	R0	K7	R3
      0x70020002,  //  0007  JMP		#000B
      0xB80E1000,  //  0008  GETNGBL	R3	K8
      0x880C0709,  //  0009  GETMBR	R3	R3	K9
      0x90020E03,  //  000A  SETMBR	R0	K7	R3
      0x600C0003,  //  000B  GETGBL	R3	G3
      0x5C100000,  //  000C  MOVE	R4	R0
      0x7C0C0200,  //  000D  CALL	R3	1
      0x8C0C070A,  //  000E  GETMET	R3	R3	K10
      0x5C140200,  //  000F  MOVE	R5	R1
      0x5C180400,  //  0010  MOVE	R6	R2
      0x7C0C0600,  //  0011  CALL	R3	3
      0x80000000,  //  0012  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: produce_value
********************************************************************/
be_local_closure(class_BreatheColorProvider_produce_value,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_BreatheColorProvider,     /* shared constants */
    be_str_weak(produce_value),
    &be_const_str_solidified,
    ( &(const binstruction[82]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C070B,  //  0003  GETMET	R3	R3	K11
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x88100105,  //  0007  GETMBR	R4	R0	K5
      0x5C140600,  //  0008  MOVE	R5	R3
      0x24180906,  //  0009  GT	R6	R4	K6
      0x781A0019,  //  000A  JMPF	R6	#0025
      0xB81A1800,  //  000B  GETNGBL	R6	K12
      0x8C180D0D,  //  000C  GETMET	R6	R6	K13
      0x5C200600,  //  000D  MOVE	R8	R3
      0x5824000E,  //  000E  LDCONST	R9	K14
      0x542A00FE,  //  000F  LDINT	R10	255
      0x582C000E,  //  0010  LDCONST	R11	K14
      0x54321FFF,  //  0011  LDINT	R12	8192
      0x7C180C00,  //  0012  CALL	R6	6
      0x5C1C0800,  //  0013  MOVE	R7	R4
      0x24200F06,  //  0014  GT	R8	R7	K6
      0x78220005,  //  0015  JMPF	R8	#001C
      0x08200C06,  //  0016  MUL	R8	R6	R6
      0x54261FFF,  //  0017  LDINT	R9	8192
      0x0C201009,  //  0018  DIV	R8	R8	R9
      0x5C181000,  //  0019  MOVE	R6	R8
      0x041C0F06,  //  001A  SUB	R7	R7	K6
      0x7001FFF7,  //  001B  JMP		#0014
      0xB8221800,  //  001C  GETNGBL	R8	K12
      0x8C20110D,  //  001D  GETMET	R8	R8	K13
      0x5C280C00,  //  001E  MOVE	R10	R6
      0x582C000E,  //  001F  LDCONST	R11	K14
      0x54321FFF,  //  0020  LDINT	R12	8192
      0x5834000E,  //  0021  LDCONST	R13	K14
      0x543A00FE,  //  0022  LDINT	R14	255
      0x7C200C00,  //  0023  CALL	R8	6
      0x5C141000,  //  0024  MOVE	R5	R8
      0xB81A1800,  //  0025  GETNGBL	R6	K12
      0x8C180D0D,  //  0026  GETMET	R6	R6	K13
      0x5C200A00,  //  0027  MOVE	R8	R5
      0x5824000E,  //  0028  LDCONST	R9	K14
      0x542A00FE,  //  0029  LDINT	R10	255
      0x882C0102,  //  002A  GETMBR	R11	R0	K2
      0x88300103,  //  002B  GETMBR	R12	R0	K3
      0x7C180C00,  //  002C  CALL	R6	6
      0x881C0101,  //  002D  GETMBR	R7	R0	K1
      0x54220017,  //  002E  LDINT	R8	24
      0x3C200E08,  //  002F  SHR	R8	R7	R8
      0x542600FE,  //  0030  LDINT	R9	255
      0x2C201009,  //  0031  AND	R8	R8	R9
      0x5426000F,  //  0032  LDINT	R9	16
      0x3C240E09,  //  0033  SHR	R9	R7	R9
      0x542A00FE,  //  0034  LDINT	R10	255
      0x2C24120A,  //  0035  AND	R9	R9	R10
      0x542A0007,  //  0036  LDINT	R10	8
      0x3C280E0A,  //  0037  SHR	R10	R7	R10
      0x542E00FE,  //  0038  LDINT	R11	255
      0x2C28140B,  //  0039  AND	R10	R10	R11
      0x542E00FE,  //  003A  LDINT	R11	255
      0x2C2C0E0B,  //  003B  AND	R11	R7	R11
      0x08301206,  //  003C  MUL	R12	R9	R6
      0x543600FE,  //  003D  LDINT	R13	255
      0x0C30180D,  //  003E  DIV	R12	R12	R13
      0x5C241800,  //  003F  MOVE	R9	R12
      0x08301406,  //  0040  MUL	R12	R10	R6
      0x543600FE,  //  0041  LDINT	R13	255
      0x0C30180D,  //  0042  DIV	R12	R12	R13
      0x5C281800,  //  0043  MOVE	R10	R12
      0x08301606,  //  0044  MUL	R12	R11	R6
      0x543600FE,  //  0045  LDINT	R13	255
      0x0C30180D,  //  0046  DIV	R12	R12	R13
      0x5C2C1800,  //  0047  MOVE	R11	R12
      0x54320017,  //  0048  LDINT	R12	24
      0x3830100C,  //  0049  SHL	R12	R8	R12
      0x5436000F,  //  004A  LDINT	R13	16
      0x3834120D,  //  004B  SHL	R13	R9	R13
      0x3030180D,  //  004C  OR	R12	R12	R13
      0x54360007,  //  004D  LDINT	R13	8
      0x3834140D,  //  004E  SHL	R13	R10	R13
      0x3030180D,  //  004F  OR	R12	R12	R13
      0x3030180B,  //  0050  OR	R12	R12	R11
      0x80041800,  //  0051  RET	1	R12
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_BreatheColorProvider_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_BreatheColorProvider,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C08050F,  //  0003  GETMET	R2	R2	K15
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0xB80A1000,  //  0006  GETNGBL	R2	K8
      0x88080509,  //  0007  GETMBR	R2	R2	K9
      0x90020E02,  //  0008  SETMBR	R0	K7	R2
      0x9002210E,  //  0009  SETMBR	R0	K16	K14
      0x540A00FE,  //  000A  LDINT	R2	255
      0x90022202,  //  000B  SETMBR	R0	K17	R2
      0x540A0BB7,  //  000C  LDINT	R2	3000
      0x90020802,  //  000D  SETMBR	R0	K4	R2
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: BreatheColorProvider
********************************************************************/
extern const bclass be_class_OscillatorValueProvider;
be_local_class(BreatheColorProvider,
    0,
    &be_class_OscillatorValueProvider,
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(tostring, 1), be_const_closure(class_BreatheColorProvider_tostring_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_BreatheColorProvider_init_closure) },
        { be_const_key_weak(on_param_changed, -1), be_const_closure(class_BreatheColorProvider_on_param_changed_closure) },
        { be_const_key_weak(PARAMS, 4), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(base_color, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(-1) },
    }))    ) } )) },
        { be_const_key_weak(max_brightness, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(255) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(curve_factor, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(2) },
        { be_const_key_weak(min, -1), be_const_int(1) },
        { be_const_key_weak(max, 1), be_const_int(5) },
    }))    ) } )) },
        { be_const_key_weak(min_brightness, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(0) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(produce_value, -1), be_const_closure(class_BreatheColorProvider_produce_value_closure) },
    })),
    be_str_weak(BreatheColorProvider)
);

/********************************************************************
** Solidified function: noise_rainbow
********************************************************************/
be_local_closure(noise_rainbow,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(noise_animation),
    /* K2   */  be_nested_str_weak(rich_palette),
    /* K3   */  be_nested_str_weak(palette),
    /* K4   */  be_nested_str_weak(PALETTE_RAINBOW),
    /* K5   */  be_nested_str_weak(cycle_period),
    /* K6   */  be_nested_str_weak(transition_type),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str_weak(brightness),
    /* K9   */  be_nested_str_weak(range_min),
    /* K10  */  be_const_int(0),
    /* K11  */  be_nested_str_weak(range_max),
    /* K12  */  be_nested_str_weak(color),
    /* K13  */  be_nested_str_weak(scale),
    /* K14  */  be_nested_str_weak(speed),
    /* K15  */  be_nested_str_weak(octaves),
    }),
    be_str_weak(noise_rainbow),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0xB80A0000,  //  0004  GETNGBL	R2	K0
      0x8C080502,  //  0005  GETMET	R2	R2	K2
      0x5C100000,  //  0006  MOVE	R4	R0
      0x7C080400,  //  0007  CALL	R2	2
      0xB80E0000,  //  0008  GETNGBL	R3	K0
      0x880C0704,  //  0009  GETMBR	R3	R3	K4
      0x900A0603,  //  000A  SETMBR	R2	K3	R3
      0x540E1387,  //  000B  LDINT	R3	5000
      0x900A0A03,  //  000C  SETMBR	R2	K5	R3
      0x900A0D07,  //  000D  SETMBR	R2	K6	K7
      0x540E00FE,  //  000E  LDINT	R3	255
      0x900A1003,  //  000F  SETMBR	R2	K8	R3
      0x900A130A,  //  0010  SETMBR	R2	K9	K10
      0x540E00FE,  //  0011  LDINT	R3	255
      0x900A1603,  //  0012  SETMBR	R2	K11	R3
      0x90061802,  //  0013  SETMBR	R1	K12	R2
      0x540E0031,  //  0014  LDINT	R3	50
      0x90061A03,  //  0015  SETMBR	R1	K13	R3
      0x540E001D,  //  0016  LDINT	R3	30
      0x90061C03,  //  0017  SETMBR	R1	K14	R3
      0x90061F07,  //  0018  SETMBR	R1	K15	K7
      0x80040200,  //  0019  RET	1	R1
    })
  )
);
/*******************************************************************/

// compact class 'EventHandler' ktab size: 8, total: 17 (saved 72 bytes)
static const bvalue be_ktab_class_EventHandler[8] = {
  /* K0   */  be_nested_str_weak(is_active),
  /* K1   */  be_nested_str_weak(condition),
  /* K2   */  be_nested_str_weak(callback_func),
  /* K3   */  be_nested_str_weak(event_name),
  /* K4   */  be_nested_str_weak(priority),
  /* K5   */  be_const_int(0),
  /* K6   */  be_nested_str_weak(metadata),
  /* K7   */  be_nested_str_weak(has_condition),
};


extern const bclass be_class_EventHandler;

/********************************************************************
** Solidified function: execute
********************************************************************/
be_local_closure(class_EventHandler_execute,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_EventHandler,     /* shared constants */
    be_str_weak(execute),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x740A0001,  //  0001  JMPT	R2	#0004
      0x50080000,  //  0002  LDBOOL	R2	0	0
      0x80040400,  //  0003  RET	1	R2
      0x88080101,  //  0004  GETMBR	R2	R0	K1
      0x4C0C0000,  //  0005  LDNIL	R3
      0x20080403,  //  0006  NE	R2	R2	R3
      0x780A0005,  //  0007  JMPF	R2	#000E
      0x8C080101,  //  0008  GETMET	R2	R0	K1
      0x5C100200,  //  0009  MOVE	R4	R1
      0x7C080400,  //  000A  CALL	R2	2
      0x740A0001,  //  000B  JMPT	R2	#000E
      0x50080000,  //  000C  LDBOOL	R2	0	0
      0x80040400,  //  000D  RET	1	R2
      0x88080102,  //  000E  GETMBR	R2	R0	K2
      0x4C0C0000,  //  000F  LDNIL	R3
      0x20080403,  //  0010  NE	R2	R2	R3
      0x780A0004,  //  0011  JMPF	R2	#0017
      0x8C080102,  //  0012  GETMET	R2	R0	K2
      0x5C100200,  //  0013  MOVE	R4	R1
      0x7C080400,  //  0014  CALL	R2	2
      0x50080200,  //  0015  LDBOOL	R2	1	0
      0x80040400,  //  0016  RET	1	R2
      0x50080000,  //  0017  LDBOOL	R2	0	0
      0x80040400,  //  0018  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_active
********************************************************************/
be_local_closure(class_EventHandler_set_active,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_EventHandler,     /* shared constants */
    be_str_weak(set_active),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_EventHandler_init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    6,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_EventHandler,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x90020601,  //  0000  SETMBR	R0	K3	R1
      0x90020402,  //  0001  SETMBR	R0	K2	R2
      0x4C180000,  //  0002  LDNIL	R6
      0x20180606,  //  0003  NE	R6	R3	R6
      0x781A0001,  //  0004  JMPF	R6	#0007
      0x5C180600,  //  0005  MOVE	R6	R3
      0x70020000,  //  0006  JMP		#0008
      0x58180005,  //  0007  LDCONST	R6	K5
      0x90020806,  //  0008  SETMBR	R0	K4	R6
      0x90020204,  //  0009  SETMBR	R0	K1	R4
      0x50180200,  //  000A  LDBOOL	R6	1	0
      0x90020006,  //  000B  SETMBR	R0	K0	R6
      0x4C180000,  //  000C  LDNIL	R6
      0x20180A06,  //  000D  NE	R6	R5	R6
      0x781A0001,  //  000E  JMPF	R6	#0011
      0x5C180A00,  //  000F  MOVE	R6	R5
      0x70020001,  //  0010  JMP		#0013
      0x60180013,  //  0011  GETGBL	R6	G19
      0x7C180000,  //  0012  CALL	R6	0
      0x90020C06,  //  0013  SETMBR	R0	K6	R6
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_info
********************************************************************/
be_local_closure(class_EventHandler_get_info,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_EventHandler,     /* shared constants */
    be_str_weak(get_info),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x60040013,  //  0000  GETGBL	R1	G19
      0x7C040000,  //  0001  CALL	R1	0
      0x88080103,  //  0002  GETMBR	R2	R0	K3
      0x98060602,  //  0003  SETIDX	R1	K3	R2
      0x88080104,  //  0004  GETMBR	R2	R0	K4
      0x98060802,  //  0005  SETIDX	R1	K4	R2
      0x88080100,  //  0006  GETMBR	R2	R0	K0
      0x98060002,  //  0007  SETIDX	R1	K0	R2
      0x88080101,  //  0008  GETMBR	R2	R0	K1
      0x4C0C0000,  //  0009  LDNIL	R3
      0x20080403,  //  000A  NE	R2	R2	R3
      0x98060E02,  //  000B  SETIDX	R1	K7	R2
      0x88080106,  //  000C  GETMBR	R2	R0	K6
      0x98060C02,  //  000D  SETIDX	R1	K6	R2
      0x80040200,  //  000E  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: EventHandler
********************************************************************/
be_local_class(EventHandler,
    6,
    NULL,
    be_nested_map(10,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(set_active, -1), be_const_closure(class_EventHandler_set_active_closure) },
        { be_const_key_weak(metadata, 3), be_const_var(5) },
        { be_const_key_weak(is_active, 0), be_const_var(4) },
        { be_const_key_weak(condition, -1), be_const_var(2) },
        { be_const_key_weak(init, -1), be_const_closure(class_EventHandler_init_closure) },
        { be_const_key_weak(event_name, 4), be_const_var(0) },
        { be_const_key_weak(get_info, -1), be_const_closure(class_EventHandler_get_info_closure) },
        { be_const_key_weak(priority, -1), be_const_var(3) },
        { be_const_key_weak(execute, 1), be_const_closure(class_EventHandler_execute_closure) },
        { be_const_key_weak(callback_func, -1), be_const_var(1) },
    })),
    be_str_weak(EventHandler)
);
// compact class 'Animation' ktab size: 27, total: 52 (saved 200 bytes)
static const bvalue be_ktab_class_Animation[27] = {
  /* K0   */  be_nested_str_weak(Animation_X28_X25s_X2C_X20priority_X3D_X25s_X2C_X20duration_X3D_X25s_X2C_X20loop_X3D_X25s_X2C_X20running_X3D_X25s_X29),
  /* K1   */  be_nested_str_weak(name),
  /* K2   */  be_nested_str_weak(priority),
  /* K3   */  be_nested_str_weak(duration),
  /* K4   */  be_nested_str_weak(loop),
  /* K5   */  be_nested_str_weak(is_running),
  /* K6   */  be_nested_str_weak(get_color_at),
  /* K7   */  be_const_int(0),
  /* K8   */  be_nested_str_weak(get_param_value),
  /* K9   */  be_nested_str_weak(color),
  /* K10  */  be_nested_str_weak(update),
  /* K11  */  be_nested_str_weak(opacity),
  /* K12  */  be_nested_str_weak(fill_pixels),
  /* K13  */  be_nested_str_weak(apply_brightness),
  /* K14  */  be_nested_str_weak(current_time),
  /* K15  */  be_nested_str_weak(start_time),
  /* K16  */  be_nested_str_weak(tasmota),
  /* K17  */  be_nested_str_weak(scale_uint),
  /* K18  */  be_nested_str_weak(engine),
  /* K19  */  be_nested_str_weak(time_ms),
  /* K20  */  be_nested_str_weak(_start_value_providers),
  /* K21  */  be_nested_str_weak(init),
  /* K22  */  be_nested_str_weak(values),
  /* K23  */  be_nested_str_weak(animation),
  /* K24  */  be_nested_str_weak(is_value_provider),
  /* K25  */  be_nested_str_weak(start),
  /* K26  */  be_nested_str_weak(stop_iteration),
};


extern const bclass be_class_Animation;

/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_Animation_tostring,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animation,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x60040018,  //  0000  GETGBL	R1	G24
      0x58080000,  //  0001  LDCONST	R2	K0
      0x880C0101,  //  0002  GETMBR	R3	R0	K1
      0x88100102,  //  0003  GETMBR	R4	R0	K2
      0x88140103,  //  0004  GETMBR	R5	R0	K3
      0x88180104,  //  0005  GETMBR	R6	R0	K4
      0x881C0105,  //  0006  GETMBR	R7	R0	K5
      0x7C040C00,  //  0007  CALL	R1	6
      0x80040200,  //  0008  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_color
********************************************************************/
be_local_closure(class_Animation_get_color,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animation,     /* shared constants */
    be_str_weak(get_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C080106,  //  0000  GETMET	R2	R0	K6
      0x58100007,  //  0001  LDCONST	R4	K7
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C080600,  //  0003  CALL	R2	3
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_color_at
********************************************************************/
be_local_closure(class_Animation_get_color_at,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animation,     /* shared constants */
    be_str_weak(get_color_at),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C0C0108,  //  0000  GETMET	R3	R0	K8
      0x58140009,  //  0001  LDCONST	R5	K9
      0x5C180400,  //  0002  MOVE	R6	R2
      0x7C0C0600,  //  0003  CALL	R3	3
      0x80040600,  //  0004  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: render
********************************************************************/
be_local_closure(class_Animation_render,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animation,     /* shared constants */
    be_str_weak(render),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x880C0105,  //  0000  GETMBR	R3	R0	K5
      0x5C100600,  //  0001  MOVE	R4	R3
      0x78120002,  //  0002  JMPF	R4	#0006
      0x4C100000,  //  0003  LDNIL	R4
      0x1C100204,  //  0004  EQ	R4	R1	R4
      0x78120001,  //  0005  JMPF	R4	#0008
      0x50100000,  //  0006  LDBOOL	R4	0	0
      0x80040800,  //  0007  RET	1	R4
      0x8C10010A,  //  0008  GETMET	R4	R0	K10
      0x5C180400,  //  0009  MOVE	R6	R2
      0x7C100400,  //  000A  CALL	R4	2
      0x88100109,  //  000B  GETMBR	R4	R0	K9
      0x8814010B,  //  000C  GETMBR	R5	R0	K11
      0x8C18030C,  //  000D  GETMET	R6	R1	K12
      0x5C200800,  //  000E  MOVE	R8	R4
      0x7C180400,  //  000F  CALL	R6	2
      0x541A00FE,  //  0010  LDINT	R6	255
      0x14180A06,  //  0011  LT	R6	R5	R6
      0x781A0002,  //  0012  JMPF	R6	#0016
      0x8C18030D,  //  0013  GETMET	R6	R1	K13
      0x5C200A00,  //  0014  MOVE	R8	R5
      0x7C180400,  //  0015  CALL	R6	2
      0x50180200,  //  0016  LDBOOL	R6	1	0
      0x80040C00,  //  0017  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_progress
********************************************************************/
be_local_closure(class_Animation_get_progress,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animation,     /* shared constants */
    be_str_weak(get_progress),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x88040103,  //  0000  GETMBR	R1	R0	K3
      0x18080307,  //  0001  LE	R2	R1	K7
      0x780A0000,  //  0002  JMPF	R2	#0004
      0x80060E00,  //  0003  RET	1	K7
      0x8808010E,  //  0004  GETMBR	R2	R0	K14
      0x880C010F,  //  0005  GETMBR	R3	R0	K15
      0x04080403,  //  0006  SUB	R2	R2	R3
      0x100C0401,  //  0007  MOD	R3	R2	R1
      0x88100104,  //  0008  GETMBR	R4	R0	K4
      0x5C140800,  //  0009  MOVE	R5	R4
      0x74160003,  //  000A  JMPT	R5	#000F
      0x28140401,  //  000B  GE	R5	R2	R1
      0x78160001,  //  000C  JMPF	R5	#000F
      0x541600FE,  //  000D  LDINT	R5	255
      0x80040A00,  //  000E  RET	1	R5
      0xB8162000,  //  000F  GETNGBL	R5	K16
      0x8C140B11,  //  0010  GETMET	R5	R5	K17
      0x5C1C0600,  //  0011  MOVE	R7	R3
      0x58200007,  //  0012  LDCONST	R8	K7
      0x5C240200,  //  0013  MOVE	R9	R1
      0x58280007,  //  0014  LDCONST	R10	K7
      0x542E00FE,  //  0015  LDINT	R11	255
      0x7C140C00,  //  0016  CALL	R5	6
      0x80040A00,  //  0017  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: on_param_changed
********************************************************************/
be_local_closure(class_Animation_on_param_changed,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animation,     /* shared constants */
    be_str_weak(on_param_changed),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x1C0C0305,  //  0000  EQ	R3	R1	K5
      0x780E000E,  //  0001  JMPF	R3	#0011
      0x500C0200,  //  0002  LDBOOL	R3	1	0
      0x1C0C0403,  //  0003  EQ	R3	R2	R3
      0x780E0008,  //  0004  JMPF	R3	#000E
      0x880C0112,  //  0005  GETMBR	R3	R0	K18
      0x880C0713,  //  0006  GETMBR	R3	R3	K19
      0x90021E03,  //  0007  SETMBR	R0	K15	R3
      0x8810010F,  //  0008  GETMBR	R4	R0	K15
      0x90021C04,  //  0009  SETMBR	R0	K14	R4
      0x8C100114,  //  000A  GETMET	R4	R0	K20
      0x5C180600,  //  000B  MOVE	R6	R3
      0x7C100400,  //  000C  CALL	R4	2
      0x70020002,  //  000D  JMP		#0011
      0x500C0000,  //  000E  LDBOOL	R3	0	0
      0x1C0C0403,  //  000F  EQ	R3	R2	R3
      0x780DFFFF,  //  0010  JMPF	R3	#0011
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Animation_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animation,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080515,  //  0003  GETMET	R2	R2	K21
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x90021F07,  //  0006  SETMBR	R0	K15	K7
      0x90021D07,  //  0007  SETMBR	R0	K14	K7
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(class_Animation_start,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animation,     /* shared constants */
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x88080116,  //  0000  GETMBR	R2	R0	K22
      0x500C0200,  //  0001  LDBOOL	R3	1	0
      0x980A0A03,  //  0002  SETIDX	R2	K5	R3
      0x4C080000,  //  0003  LDNIL	R2
      0x20080202,  //  0004  NE	R2	R1	R2
      0x780A0001,  //  0005  JMPF	R2	#0008
      0x5C080200,  //  0006  MOVE	R2	R1
      0x70020001,  //  0007  JMP		#000A
      0x88080112,  //  0008  GETMBR	R2	R0	K18
      0x88080513,  //  0009  GETMBR	R2	R2	K19
      0x90021E02,  //  000A  SETMBR	R0	K15	R2
      0x880C010F,  //  000B  GETMBR	R3	R0	K15
      0x90021C03,  //  000C  SETMBR	R0	K14	R3
      0x8C0C0114,  //  000D  GETMET	R3	R0	K20
      0x5C140400,  //  000E  MOVE	R5	R2
      0x7C0C0400,  //  000F  CALL	R3	2
      0x80040000,  //  0010  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _start_value_providers
********************************************************************/
be_local_closure(class_Animation__start_value_providers,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animation,     /* shared constants */
    be_str_weak(_start_value_providers),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x60080010,  //  0000  GETGBL	R2	G16
      0x880C0116,  //  0001  GETMBR	R3	R0	K22
      0x7C080200,  //  0002  CALL	R2	1
      0xA8020011,  //  0003  EXBLK	0	#0016
      0x5C0C0400,  //  0004  MOVE	R3	R2
      0x7C0C0000,  //  0005  CALL	R3	0
      0xB8122E00,  //  0006  GETNGBL	R4	K23
      0x8C100918,  //  0007  GETMET	R4	R4	K24
      0x5C180600,  //  0008  MOVE	R6	R3
      0x7C100400,  //  0009  CALL	R4	2
      0x78120009,  //  000A  JMPF	R4	#0015
      0xA8020004,  //  000B  EXBLK	0	#0011
      0x8C100719,  //  000C  GETMET	R4	R3	K25
      0x5C180200,  //  000D  MOVE	R6	R1
      0x7C100400,  //  000E  CALL	R4	2
      0xA8040001,  //  000F  EXBLK	1	1
      0x70020003,  //  0010  JMP		#0015
      0xAC100001,  //  0011  CATCH	R4	0	1
      0x70020000,  //  0012  JMP		#0014
      0x70020000,  //  0013  JMP		#0015
      0xB0080000,  //  0014  RAISE	2	R0	R0
      0x7001FFED,  //  0015  JMP		#0004
      0x5808001A,  //  0016  LDCONST	R2	K26
      0xAC080200,  //  0017  CATCH	R2	1	0
      0xB0080000,  //  0018  RAISE	2	R0	R0
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update
********************************************************************/
be_local_closure(class_Animation_update,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animation,     /* shared constants */
    be_str_weak(update),
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0x88080105,  //  0000  GETMBR	R2	R0	K5
      0x5C0C0400,  //  0001  MOVE	R3	R2
      0x740E0001,  //  0002  JMPT	R3	#0005
      0x500C0000,  //  0003  LDBOOL	R3	0	0
      0x80040600,  //  0004  RET	1	R3
      0x90021C01,  //  0005  SETMBR	R0	K14	R1
      0x880C010E,  //  0006  GETMBR	R3	R0	K14
      0x8810010F,  //  0007  GETMBR	R4	R0	K15
      0x040C0604,  //  0008  SUB	R3	R3	R4
      0x88100103,  //  0009  GETMBR	R4	R0	K3
      0x88140104,  //  000A  GETMBR	R5	R0	K4
      0x24180907,  //  000B  GT	R6	R4	K7
      0x781A000D,  //  000C  JMPF	R6	#001B
      0x28180604,  //  000D  GE	R6	R3	R4
      0x781A000B,  //  000E  JMPF	R6	#001B
      0x78160005,  //  000F  JMPF	R5	#0016
      0x0C180604,  //  0010  DIV	R6	R3	R4
      0x881C010F,  //  0011  GETMBR	R7	R0	K15
      0x08200C04,  //  0012  MUL	R8	R6	R4
      0x001C0E08,  //  0013  ADD	R7	R7	R8
      0x90021E07,  //  0014  SETMBR	R0	K15	R7
      0x70020004,  //  0015  JMP		#001B
      0x88180116,  //  0016  GETMBR	R6	R0	K22
      0x501C0000,  //  0017  LDBOOL	R7	0	0
      0x981A0A07,  //  0018  SETIDX	R6	K5	R7
      0x50180000,  //  0019  LDBOOL	R6	0	0
      0x80040C00,  //  001A  RET	1	R6
      0x50180200,  //  001B  LDBOOL	R6	1	0
      0x80040C00,  //  001C  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Animation
********************************************************************/
extern const bclass be_class_ParameterizedObject;
be_local_class(Animation,
    2,
    &be_class_ParameterizedObject,
    be_nested_map(13,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(tostring, -1), be_const_closure(class_Animation_tostring_closure) },
        { be_const_key_weak(get_color, -1), be_const_closure(class_Animation_get_color_closure) },
        { be_const_key_weak(update, -1), be_const_closure(class_Animation_update_closure) },
        { be_const_key_weak(render, -1), be_const_closure(class_Animation_render_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_Animation_init_closure) },
        { be_const_key_weak(current_time, -1), be_const_var(1) },
        { be_const_key_weak(on_param_changed, 4), be_const_closure(class_Animation_on_param_changed_closure) },
        { be_const_key_weak(get_progress, 5), be_const_closure(class_Animation_get_progress_closure) },
        { be_const_key_weak(start, -1), be_const_closure(class_Animation_start_closure) },
        { be_const_key_weak(_start_value_providers, -1), be_const_closure(class_Animation__start_value_providers_closure) },
        { be_const_key_weak(start_time, -1), be_const_var(0) },
        { be_const_key_weak(PARAMS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(priority, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(10) },
        { be_const_key_weak(min, -1), be_const_int(0) },
    }))    ) } )) },
        { be_const_key_weak(name, 3), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_nested_str_weak(animation) },
        { be_const_key_weak(type, -1), be_nested_str_weak(string) },
    }))    ) } )) },
        { be_const_key_weak(color, 1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(-1) },
    }))    ) } )) },
        { be_const_key_weak(is_running, 5), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_bool(0) },
        { be_const_key_weak(type, -1), be_nested_str_weak(bool) },
    }))    ) } )) },
        { be_const_key_weak(opacity, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(255) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(loop, 0), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_bool(1) },
        { be_const_key_weak(type, -1), be_nested_str_weak(bool) },
    }))    ) } )) },
        { be_const_key_weak(duration, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(0) },
        { be_const_key_weak(min, -1), be_const_int(0) },
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(get_color_at, 2), be_const_closure(class_Animation_get_color_at_closure) },
    })),
    be_str_weak(Animation)
);

/********************************************************************
** Solidified function: is_user_function
********************************************************************/
be_local_closure(is_user_function,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(global),
    /* K1   */  be_nested_str_weak(_animation_user_functions),
    /* K2   */  be_nested_str_weak(contains),
    }),
    be_str_weak(is_user_function),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x88080301,  //  0001  GETMBR	R2	R1	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0x5C100000,  //  0003  MOVE	R4	R0
      0x7C080400,  //  0004  CALL	R2	2
      0x80040400,  //  0005  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: register_user_function
********************************************************************/
be_local_closure(register_user_function,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(global),
    /* K1   */  be_nested_str_weak(_animation_user_functions),
    }),
    be_str_weak(register_user_function),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x880C0501,  //  0001  GETMBR	R3	R2	K1
      0x980C0001,  //  0002  SETIDX	R3	R0	R1
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: shift_scroll_right
********************************************************************/
be_local_closure(shift_scroll_right,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(shift_animation),
    /* K2   */  be_nested_str_weak(direction),
    /* K3   */  be_const_int(1),
    /* K4   */  be_nested_str_weak(shift_speed),
    /* K5   */  be_nested_str_weak(wrap_around),
    }),
    be_str_weak(shift_scroll_right),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x90060503,  //  0004  SETMBR	R1	K2	K3
      0x540A007F,  //  0005  LDINT	R2	128
      0x90060802,  //  0006  SETMBR	R1	K4	R2
      0x50080200,  //  0007  LDBOOL	R2	1	0
      0x90060A02,  //  0008  SETMBR	R1	K5	R2
      0x80040200,  //  0009  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: animation_version_string
********************************************************************/
be_local_closure(animation_version_string,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(VERSION),
    /* K2   */  be_nested_str_weak(_X25s_X2E_X25s_X2E_X25s),
    }),
    be_str_weak(animation_version_string),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x4C040000,  //  0000  LDNIL	R1
      0x1C040001,  //  0001  EQ	R1	R0	R1
      0x78060001,  //  0002  JMPF	R1	#0005
      0xB8060000,  //  0003  GETNGBL	R1	K0
      0x88000301,  //  0004  GETMBR	R0	R1	K1
      0x54060017,  //  0005  LDINT	R1	24
      0x3C040001,  //  0006  SHR	R1	R0	R1
      0x540A00FE,  //  0007  LDINT	R2	255
      0x2C040202,  //  0008  AND	R1	R1	R2
      0x540A000F,  //  0009  LDINT	R2	16
      0x3C080002,  //  000A  SHR	R2	R0	R2
      0x540E00FE,  //  000B  LDINT	R3	255
      0x2C080403,  //  000C  AND	R2	R2	R3
      0x540E0007,  //  000D  LDINT	R3	8
      0x3C0C0003,  //  000E  SHR	R3	R0	R3
      0x541200FE,  //  000F  LDINT	R4	255
      0x2C0C0604,  //  0010  AND	R3	R3	R4
      0x60100018,  //  0011  GETGBL	R4	G24
      0x58140002,  //  0012  LDCONST	R5	K2
      0x5C180200,  //  0013  MOVE	R6	R1
      0x5C1C0400,  //  0014  MOVE	R7	R2
      0x5C200600,  //  0015  MOVE	R8	R3
      0x7C100800,  //  0016  CALL	R4	4
      0x80040800,  //  0017  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_value_provider
********************************************************************/
be_local_closure(is_value_provider,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(value_provider),
    }),
    be_str_weak(is_value_provider),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x6004000F,  //  0000  GETGBL	R1	G15
      0x5C080000,  //  0001  MOVE	R2	R0
      0xB80E0000,  //  0002  GETNGBL	R3	K0
      0x880C0701,  //  0003  GETMBR	R3	R3	K1
      0x7C040400,  //  0004  CALL	R1	2
      0x80040200,  //  0005  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: elastic
********************************************************************/
be_local_closure(elastic,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(oscillator_value),
    /* K2   */  be_nested_str_weak(form),
    /* K3   */  be_nested_str_weak(ELASTIC),
    }),
    be_str_weak(elastic),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0xB80A0000,  //  0004  GETNGBL	R2	K0
      0x88080503,  //  0005  GETMBR	R2	R2	K3
      0x90060402,  //  0006  SETMBR	R1	K2	R2
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: noise_single_color
********************************************************************/
be_local_closure(noise_single_color,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(noise_animation),
    /* K2   */  be_nested_str_weak(color),
    /* K3   */  be_nested_str_weak(scale),
    /* K4   */  be_nested_str_weak(speed),
    /* K5   */  be_nested_str_weak(octaves),
    /* K6   */  be_const_int(1),
    }),
    be_str_weak(noise_single_color),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x5409FFFE,  //  0004  LDINT	R2	-1
      0x90060402,  //  0005  SETMBR	R1	K2	R2
      0x540A0031,  //  0006  LDINT	R2	50
      0x90060602,  //  0007  SETMBR	R1	K3	R2
      0x540A001D,  //  0008  LDINT	R2	30
      0x90060802,  //  0009  SETMBR	R1	K4	R2
      0x90060B06,  //  000A  SETMBR	R1	K5	K6
      0x80040200,  //  000B  RET	1	R1
    })
  )
);
/*******************************************************************/


extern const bclass be_class_PaletteMeterAnimation;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_PaletteMeterAnimation_init,   /* name */
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
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(name),
    /* K2   */  be_nested_str_weak(palette_meter),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x90020302,  //  0006  SETMBR	R0	K1	K2
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _update_value_buffer
********************************************************************/
be_local_closure(class_PaletteMeterAnimation__update_value_buffer,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(value_func),
    /* K1   */  be_nested_str_weak(engine),
    /* K2   */  be_nested_str_weak(get_strip_length),
    /* K3   */  be_nested_str_weak(value_buffer),
    /* K4   */  be_nested_str_weak(resize),
    /* K5   */  be_nested_str_weak(tasmota),
    /* K6   */  be_nested_str_weak(scale_uint),
    /* K7   */  be_const_int(0),
    /* K8   */  be_const_int(1),
    }),
    be_str_weak(_update_value_buffer),
    &be_const_str_solidified,
    ( &(const binstruction[42]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x1C0C0403,  //  0002  EQ	R3	R2	R3
      0x780E0000,  //  0003  JMPF	R3	#0005
      0x80000600,  //  0004  RET	0
      0x880C0101,  //  0005  GETMBR	R3	R0	K1
      0x8C0C0702,  //  0006  GETMET	R3	R3	K2
      0x7C0C0200,  //  0007  CALL	R3	1
      0x6010000C,  //  0008  GETGBL	R4	G12
      0x88140103,  //  0009  GETMBR	R5	R0	K3
      0x7C100200,  //  000A  CALL	R4	1
      0x20100803,  //  000B  NE	R4	R4	R3
      0x78120003,  //  000C  JMPF	R4	#0011
      0x88100103,  //  000D  GETMBR	R4	R0	K3
      0x8C100904,  //  000E  GETMET	R4	R4	K4
      0x5C180600,  //  000F  MOVE	R6	R3
      0x7C100400,  //  0010  CALL	R4	2
      0x5C100400,  //  0011  MOVE	R4	R2
      0x5C140200,  //  0012  MOVE	R5	R1
      0x5C180000,  //  0013  MOVE	R6	R0
      0x7C100400,  //  0014  CALL	R4	2
      0xB8160A00,  //  0015  GETNGBL	R5	K5
      0x8C140B06,  //  0016  GETMET	R5	R5	K6
      0x5C1C0800,  //  0017  MOVE	R7	R4
      0x58200007,  //  0018  LDCONST	R8	K7
      0x54260063,  //  0019  LDINT	R9	100
      0x58280007,  //  001A  LDCONST	R10	K7
      0x5C2C0600,  //  001B  MOVE	R11	R3
      0x7C140C00,  //  001C  CALL	R5	6
      0x58180007,  //  001D  LDCONST	R6	K7
      0x141C0C03,  //  001E  LT	R7	R6	R3
      0x781E0008,  //  001F  JMPF	R7	#0029
      0x881C0103,  //  0020  GETMBR	R7	R0	K3
      0x14200C05,  //  0021  LT	R8	R6	R5
      0x78220001,  //  0022  JMPF	R8	#0025
      0x54220063,  //  0023  LDINT	R8	100
      0x70020000,  //  0024  JMP		#0026
      0x58200007,  //  0025  LDCONST	R8	K7
      0x981C0C08,  //  0026  SETIDX	R7	R6	R8
      0x00180D08,  //  0027  ADD	R6	R6	K8
      0x7001FFF4,  //  0028  JMP		#001E
      0x80000000,  //  0029  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: PaletteMeterAnimation
********************************************************************/
extern const bclass be_class_PalettePatternAnimation;
be_local_class(PaletteMeterAnimation,
    0,
    &be_class_PalettePatternAnimation,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(PARAMS, 1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(value_func, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_nil() },
        { be_const_key_weak(type, -1), be_nested_str_weak(function) },
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(init, -1), be_const_closure(class_PaletteMeterAnimation_init_closure) },
        { be_const_key_weak(_update_value_buffer, -1), be_const_closure(class_PaletteMeterAnimation__update_value_buffer_closure) },
    })),
    be_str_weak(PaletteMeterAnimation)
);

/********************************************************************
** Solidified function: pulsating_animation
********************************************************************/
be_local_closure(pulsating_animation,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(breathe_animation),
    /* K2   */  be_nested_str_weak(curve_factor),
    /* K3   */  be_const_int(1),
    /* K4   */  be_nested_str_weak(period),
    }),
    be_str_weak(pulsating_animation),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x90060503,  //  0004  SETMBR	R1	K2	K3
      0x540A03E7,  //  0005  LDINT	R2	1000
      0x90060802,  //  0006  SETMBR	R1	K4	R2
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: ramp
********************************************************************/
be_local_closure(ramp,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(oscillator_value),
    /* K2   */  be_nested_str_weak(form),
    /* K3   */  be_nested_str_weak(SAWTOOTH),
    }),
    be_str_weak(ramp),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0xB80A0000,  //  0004  GETNGBL	R2	K0
      0x88080503,  //  0005  GETMBR	R2	R2	K3
      0x90060402,  //  0006  SETMBR	R1	K2	R2
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: wave_custom
********************************************************************/
be_local_closure(wave_custom,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(wave_animation),
    /* K2   */  be_nested_str_weak(color),
    /* K3   */  be_nested_str_weak(wave_type),
    /* K4   */  be_const_int(2),
    /* K5   */  be_nested_str_weak(frequency),
    /* K6   */  be_nested_str_weak(wave_speed),
    }),
    be_str_weak(wave_custom),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x5409FEFF,  //  0004  LDINT	R2	-256
      0x90060402,  //  0005  SETMBR	R1	K2	R2
      0x90060704,  //  0006  SETMBR	R1	K3	K4
      0x540A0027,  //  0007  LDINT	R2	40
      0x90060A02,  //  0008  SETMBR	R1	K5	R2
      0x540A001D,  //  0009  LDINT	R2	30
      0x90060C02,  //  000A  SETMBR	R1	K6	R2
      0x80040200,  //  000B  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: ease_in
********************************************************************/
be_local_closure(ease_in,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(oscillator_value),
    /* K2   */  be_nested_str_weak(form),
    /* K3   */  be_nested_str_weak(EASE_IN),
    }),
    be_str_weak(ease_in),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0xB80A0000,  //  0004  GETNGBL	R2	K0
      0x88080503,  //  0005  GETMBR	R2	R2	K3
      0x90060402,  //  0006  SETMBR	R1	K2	R2
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/

// compact class 'AnimationEngine' ktab size: 49, total: 144 (saved 760 bytes)
static const bvalue be_ktab_class_AnimationEngine[49] = {
  /* K0   */  be_nested_str_weak(animations),
  /* K1   */  be_const_int(1),
  /* K2   */  be_const_int(0),
  /* K3   */  be_nested_str_weak(priority),
  /* K4   */  be_nested_str_weak(strip),
  /* K5   */  be_nested_str_weak(clear),
  /* K6   */  be_nested_str_weak(show),
  /* K7   */  be_nested_str_weak(AnimationEngine_X28running_X3D_X25s_X2C_X20animations_X3D_X25s_X2C_X20width_X3D_X25s_X29),
  /* K8   */  be_nested_str_weak(is_running),
  /* K9   */  be_nested_str_weak(width),
  /* K10  */  be_nested_str_weak(sequence_managers),
  /* K11  */  be_nested_str_weak(remove),
  /* K12  */  be_nested_str_weak(push),
  /* K13  */  be_nested_str_weak(set_pixel_color),
  /* K14  */  be_nested_str_weak(frame_buffer),
  /* K15  */  be_nested_str_weak(get_pixel_color),
  /* K16  */  be_nested_str_weak(value_error),
  /* K17  */  be_nested_str_weak(strip_X20cannot_X20be_X20nil),
  /* K18  */  be_nested_str_weak(length),
  /* K19  */  be_nested_str_weak(animation),
  /* K20  */  be_nested_str_weak(temp_buffer),
  /* K21  */  be_nested_str_weak(last_update),
  /* K22  */  be_nested_str_weak(time_ms),
  /* K23  */  be_nested_str_weak(fast_loop_closure),
  /* K24  */  be_nested_str_weak(render_needed),
  /* K25  */  be_nested_str_weak(stop),
  /* K26  */  be_nested_str_weak(render),
  /* K27  */  be_nested_str_weak(blend_pixels),
  /* K28  */  be_nested_str_weak(_output_to_strip),
  /* K29  */  be_nested_str_weak(name),
  /* K30  */  be_nested_str_weak(update),
  /* K31  */  be_nested_str_weak(_clear_strip),
  /* K32  */  be_nested_str_weak(_render_animations),
  /* K33  */  be_nested_str_weak(resume),
  /* K34  */  be_nested_str_weak(tasmota),
  /* K35  */  be_nested_str_weak(millis),
  /* K36  */  be_nested_str_weak(start),
  /* K37  */  be_nested_str_weak(add_fast_loop),
  /* K38  */  be_nested_str_weak(_sort_animations),
  /* K39  */  be_nested_str_weak(_handle_strip_length_change),
  /* K40  */  be_nested_str_weak(check_strip_length),
  /* K41  */  be_nested_str_weak(resize),
  /* K42  */  be_nested_str_weak(stop_iteration),
  /* K43  */  be_nested_str_weak(can_show),
  /* K44  */  be_nested_str_weak(_process_events),
  /* K45  */  be_nested_str_weak(_update_and_render),
  /* K46  */  be_nested_str_weak(remove_fast_loop),
  /* K47  */  be_nested_str_weak(event_manager),
  /* K48  */  be_nested_str_weak(_process_queued_events),
};


extern const bclass be_class_AnimationEngine;

/********************************************************************
** Solidified function: _sort_animations
********************************************************************/
be_local_closure(class_AnimationEngine__sort_animations,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationEngine,     /* shared constants */
    be_str_weak(_sort_animations),
    &be_const_str_solidified,
    ( &(const binstruction[33]) {  /* code */
      0x6004000C,  //  0000  GETGBL	R1	G12
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x7C040200,  //  0002  CALL	R1	1
      0x18080301,  //  0003  LE	R2	R1	K1
      0x780A0000,  //  0004  JMPF	R2	#0006
      0x80000400,  //  0005  RET	0
      0x58080001,  //  0006  LDCONST	R2	K1
      0x140C0401,  //  0007  LT	R3	R2	R1
      0x780E0016,  //  0008  JMPF	R3	#0020
      0x880C0100,  //  0009  GETMBR	R3	R0	K0
      0x940C0602,  //  000A  GETIDX	R3	R3	R2
      0x5C100400,  //  000B  MOVE	R4	R2
      0x24140902,  //  000C  GT	R5	R4	K2
      0x7816000D,  //  000D  JMPF	R5	#001C
      0x04140901,  //  000E  SUB	R5	R4	K1
      0x88180100,  //  000F  GETMBR	R6	R0	K0
      0x94140C05,  //  0010  GETIDX	R5	R6	R5
      0x88140B03,  //  0011  GETMBR	R5	R5	K3
      0x88180703,  //  0012  GETMBR	R6	R3	K3
      0x14140A06,  //  0013  LT	R5	R5	R6
      0x78160006,  //  0014  JMPF	R5	#001C
      0x88140100,  //  0015  GETMBR	R5	R0	K0
      0x04180901,  //  0016  SUB	R6	R4	K1
      0x881C0100,  //  0017  GETMBR	R7	R0	K0
      0x94180E06,  //  0018  GETIDX	R6	R7	R6
      0x98140806,  //  0019  SETIDX	R5	R4	R6
      0x04100901,  //  001A  SUB	R4	R4	K1
      0x7001FFEF,  //  001B  JMP		#000C
      0x88140100,  //  001C  GETMBR	R5	R0	K0
      0x98140803,  //  001D  SETIDX	R5	R4	R3
      0x00080501,  //  001E  ADD	R2	R2	K1
      0x7001FFE6,  //  001F  JMP		#0007
      0x80000000,  //  0020  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _clear_strip
********************************************************************/
be_local_closure(class_AnimationEngine__clear_strip,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationEngine,     /* shared constants */
    be_str_weak(_clear_strip),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x8C040305,  //  0001  GETMET	R1	R1	K5
      0x7C040200,  //  0002  CALL	R1	1
      0x88040104,  //  0003  GETMBR	R1	R0	K4
      0x8C040306,  //  0004  GETMET	R1	R1	K6
      0x7C040200,  //  0005  CALL	R1	1
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: size
********************************************************************/
be_local_closure(class_AnimationEngine_size,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationEngine,     /* shared constants */
    be_str_weak(size),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x6004000C,  //  0000  GETGBL	R1	G12
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: interrupt_all
********************************************************************/
be_local_closure(class_AnimationEngine_interrupt_all,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationEngine,     /* shared constants */
    be_str_weak(interrupt_all),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x8C040105,  //  0000  GETMET	R1	R0	K5
      0x7C040200,  //  0001  CALL	R1	1
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_AnimationEngine_tostring,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationEngine,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x60040018,  //  0000  GETGBL	R1	G24
      0x58080007,  //  0001  LDCONST	R2	K7
      0x880C0108,  //  0002  GETMBR	R3	R0	K8
      0x6010000C,  //  0003  GETGBL	R4	G12
      0x88140100,  //  0004  GETMBR	R5	R0	K0
      0x7C100200,  //  0005  CALL	R4	1
      0x88140109,  //  0006  GETMBR	R5	R0	K9
      0x7C040800,  //  0007  CALL	R1	4
      0x80040200,  //  0008  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_strip
********************************************************************/
be_local_closure(class_AnimationEngine_get_strip,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationEngine,     /* shared constants */
    be_str_weak(get_strip),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_sequence_manager
********************************************************************/
be_local_closure(class_AnimationEngine_remove_sequence_manager,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationEngine,     /* shared constants */
    be_str_weak(remove_sequence_manager),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x5409FFFE,  //  0000  LDINT	R2	-1
      0x580C0002,  //  0001  LDCONST	R3	K2
      0x6010000C,  //  0002  GETGBL	R4	G12
      0x8814010A,  //  0003  GETMBR	R5	R0	K10
      0x7C100200,  //  0004  CALL	R4	1
      0x14100604,  //  0005  LT	R4	R3	R4
      0x78120007,  //  0006  JMPF	R4	#000F
      0x8810010A,  //  0007  GETMBR	R4	R0	K10
      0x94100803,  //  0008  GETIDX	R4	R4	R3
      0x1C100801,  //  0009  EQ	R4	R4	R1
      0x78120001,  //  000A  JMPF	R4	#000D
      0x5C080600,  //  000B  MOVE	R2	R3
      0x70020001,  //  000C  JMP		#000F
      0x000C0701,  //  000D  ADD	R3	R3	K1
      0x7001FFF2,  //  000E  JMP		#0002
      0x28100502,  //  000F  GE	R4	R2	K2
      0x78120005,  //  0010  JMPF	R4	#0017
      0x8810010A,  //  0011  GETMBR	R4	R0	K10
      0x8C10090B,  //  0012  GETMET	R4	R4	K11
      0x5C180400,  //  0013  MOVE	R6	R2
      0x7C100400,  //  0014  CALL	R4	2
      0x50100200,  //  0015  LDBOOL	R4	1	0
      0x80040800,  //  0016  RET	1	R4
      0x50100000,  //  0017  LDBOOL	R4	0	0
      0x80040800,  //  0018  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_sequence_manager
********************************************************************/
be_local_closure(class_AnimationEngine_add_sequence_manager,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationEngine,     /* shared constants */
    be_str_weak(add_sequence_manager),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8808010A,  //  0000  GETMBR	R2	R0	K10
      0x8C08050C,  //  0001  GETMET	R2	R2	K12
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80040000,  //  0004  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _output_to_strip
********************************************************************/
be_local_closure(class_AnimationEngine__output_to_strip,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationEngine,     /* shared constants */
    be_str_weak(_output_to_strip),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x58040002,  //  0000  LDCONST	R1	K2
      0x88080109,  //  0001  GETMBR	R2	R0	K9
      0x14080202,  //  0002  LT	R2	R1	R2
      0x780A0009,  //  0003  JMPF	R2	#000E
      0x88080104,  //  0004  GETMBR	R2	R0	K4
      0x8C08050D,  //  0005  GETMET	R2	R2	K13
      0x5C100200,  //  0006  MOVE	R4	R1
      0x8814010E,  //  0007  GETMBR	R5	R0	K14
      0x8C140B0F,  //  0008  GETMET	R5	R5	K15
      0x5C1C0200,  //  0009  MOVE	R7	R1
      0x7C140400,  //  000A  CALL	R5	2
      0x7C080600,  //  000B  CALL	R2	3
      0x00040301,  //  000C  ADD	R1	R1	K1
      0x7001FFF2,  //  000D  JMP		#0001
      0x88080104,  //  000E  GETMBR	R2	R0	K4
      0x8C080506,  //  000F  GETMET	R2	R2	K6
      0x7C080200,  //  0010  CALL	R2	1
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_AnimationEngine_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationEngine,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[33]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0000,  //  0002  JMPF	R2	#0004
      0xB0062111,  //  0003  RAISE	1	K16	K17
      0x90020801,  //  0004  SETMBR	R0	K4	R1
      0x8C080312,  //  0005  GETMET	R2	R1	K18
      0x7C080200,  //  0006  CALL	R2	1
      0x90021202,  //  0007  SETMBR	R0	K9	R2
      0x60080012,  //  0008  GETGBL	R2	G18
      0x7C080000,  //  0009  CALL	R2	0
      0x90020002,  //  000A  SETMBR	R0	K0	R2
      0x60080012,  //  000B  GETGBL	R2	G18
      0x7C080000,  //  000C  CALL	R2	0
      0x90021402,  //  000D  SETMBR	R0	K10	R2
      0xB80A2600,  //  000E  GETNGBL	R2	K19
      0x8C08050E,  //  000F  GETMET	R2	R2	K14
      0x88100109,  //  0010  GETMBR	R4	R0	K9
      0x7C080400,  //  0011  CALL	R2	2
      0x90021C02,  //  0012  SETMBR	R0	K14	R2
      0xB80A2600,  //  0013  GETNGBL	R2	K19
      0x8C08050E,  //  0014  GETMET	R2	R2	K14
      0x88100109,  //  0015  GETMBR	R4	R0	K9
      0x7C080400,  //  0016  CALL	R2	2
      0x90022802,  //  0017  SETMBR	R0	K20	R2
      0x50080000,  //  0018  LDBOOL	R2	0	0
      0x90021002,  //  0019  SETMBR	R0	K8	R2
      0x90022B02,  //  001A  SETMBR	R0	K21	K2
      0x90022D02,  //  001B  SETMBR	R0	K22	K2
      0x4C080000,  //  001C  LDNIL	R2
      0x90022E02,  //  001D  SETMBR	R0	K23	R2
      0x50080000,  //  001E  LDBOOL	R2	0	0
      0x90023002,  //  001F  SETMBR	R0	K24	R2
      0x80000000,  //  0020  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: clear
********************************************************************/
be_local_closure(class_AnimationEngine_clear,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationEngine,     /* shared constants */
    be_str_weak(clear),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x60040012,  //  0000  GETGBL	R1	G18
      0x7C040000,  //  0001  CALL	R1	0
      0x90020001,  //  0002  SETMBR	R0	K0	R1
      0x58040002,  //  0003  LDCONST	R1	K2
      0x6008000C,  //  0004  GETGBL	R2	G12
      0x880C010A,  //  0005  GETMBR	R3	R0	K10
      0x7C080200,  //  0006  CALL	R2	1
      0x14080202,  //  0007  LT	R2	R1	R2
      0x780A0005,  //  0008  JMPF	R2	#000F
      0x8808010A,  //  0009  GETMBR	R2	R0	K10
      0x94080401,  //  000A  GETIDX	R2	R2	R1
      0x8C080519,  //  000B  GETMET	R2	R2	K25
      0x7C080200,  //  000C  CALL	R2	1
      0x00040301,  //  000D  ADD	R1	R1	K1
      0x7001FFF4,  //  000E  JMP		#0004
      0x60080012,  //  000F  GETGBL	R2	G18
      0x7C080000,  //  0010  CALL	R2	0
      0x90021402,  //  0011  SETMBR	R0	K10	R2
      0x50080200,  //  0012  LDBOOL	R2	1	0
      0x90023002,  //  0013  SETMBR	R0	K24	R2
      0x80040000,  //  0014  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _render_animations
********************************************************************/
be_local_closure(class_AnimationEngine__render_animations,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationEngine,     /* shared constants */
    be_str_weak(_render_animations),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x880C010E,  //  0000  GETMBR	R3	R0	K14
      0x8C0C0705,  //  0001  GETMET	R3	R3	K5
      0x7C0C0200,  //  0002  CALL	R3	1
      0x580C0002,  //  0003  LDCONST	R3	K2
      0x6010000C,  //  0004  GETGBL	R4	G12
      0x5C140200,  //  0005  MOVE	R5	R1
      0x7C100200,  //  0006  CALL	R4	1
      0x14100604,  //  0007  LT	R4	R3	R4
      0x7812000E,  //  0008  JMPF	R4	#0018
      0x94100203,  //  0009  GETIDX	R4	R1	R3
      0x88140114,  //  000A  GETMBR	R5	R0	K20
      0x8C140B05,  //  000B  GETMET	R5	R5	K5
      0x7C140200,  //  000C  CALL	R5	1
      0x8C14091A,  //  000D  GETMET	R5	R4	K26
      0x881C0114,  //  000E  GETMBR	R7	R0	K20
      0x5C200400,  //  000F  MOVE	R8	R2
      0x7C140600,  //  0010  CALL	R5	3
      0x78160003,  //  0011  JMPF	R5	#0016
      0x8818010E,  //  0012  GETMBR	R6	R0	K14
      0x8C180D1B,  //  0013  GETMET	R6	R6	K27
      0x88200114,  //  0014  GETMBR	R8	R0	K20
      0x7C180400,  //  0015  CALL	R6	2
      0x000C0701,  //  0016  ADD	R3	R3	K1
      0x7001FFEB,  //  0017  JMP		#0004
      0x8C10011C,  //  0018  GETMET	R4	R0	K28
      0x7C100200,  //  0019  CALL	R4	1
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: interrupt_animation
********************************************************************/
be_local_closure(class_AnimationEngine_interrupt_animation,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationEngine,     /* shared constants */
    be_str_weak(interrupt_animation),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x58080002,  //  0000  LDCONST	R2	K2
      0x600C000C,  //  0001  GETGBL	R3	G12
      0x88100100,  //  0002  GETMBR	R4	R0	K0
      0x7C0C0200,  //  0003  CALL	R3	1
      0x140C0403,  //  0004  LT	R3	R2	R3
      0x780E0012,  //  0005  JMPF	R3	#0019
      0x880C0100,  //  0006  GETMBR	R3	R0	K0
      0x940C0602,  //  0007  GETIDX	R3	R3	R2
      0x8810071D,  //  0008  GETMBR	R4	R3	K29
      0x4C140000,  //  0009  LDNIL	R5
      0x20100805,  //  000A  NE	R4	R4	R5
      0x7812000A,  //  000B  JMPF	R4	#0017
      0x8810071D,  //  000C  GETMBR	R4	R3	K29
      0x1C100801,  //  000D  EQ	R4	R4	R1
      0x78120007,  //  000E  JMPF	R4	#0017
      0x8C100719,  //  000F  GETMET	R4	R3	K25
      0x5C180600,  //  0010  MOVE	R6	R3
      0x7C100400,  //  0011  CALL	R4	2
      0x88100100,  //  0012  GETMBR	R4	R0	K0
      0x8C10090B,  //  0013  GETMET	R4	R4	K11
      0x5C180400,  //  0014  MOVE	R6	R2
      0x7C100400,  //  0015  CALL	R4	2
      0x80000800,  //  0016  RET	0
      0x00080501,  //  0017  ADD	R2	R2	K1
      0x7001FFE7,  //  0018  JMP		#0001
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _update_and_render
********************************************************************/
be_local_closure(class_AnimationEngine__update_and_render,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationEngine,     /* shared constants */
    be_str_weak(_update_and_render),
    &be_const_str_solidified,
    ( &(const binstruction[41]) {  /* code */
      0x58080002,  //  0000  LDCONST	R2	K2
      0x580C0002,  //  0001  LDCONST	R3	K2
      0x6010000C,  //  0002  GETGBL	R4	G12
      0x88140100,  //  0003  GETMBR	R5	R0	K0
      0x7C100200,  //  0004  CALL	R4	1
      0x14100604,  //  0005  LT	R4	R3	R4
      0x78120011,  //  0006  JMPF	R4	#0019
      0x88100100,  //  0007  GETMBR	R4	R0	K0
      0x94100803,  //  0008  GETIDX	R4	R4	R3
      0x8C14091E,  //  0009  GETMET	R5	R4	K30
      0x5C1C0200,  //  000A  MOVE	R7	R1
      0x7C140400,  //  000B  CALL	R5	2
      0x78160004,  //  000C  JMPF	R5	#0012
      0x88180908,  //  000D  GETMBR	R6	R4	K8
      0x781A0002,  //  000E  JMPF	R6	#0012
      0x00080501,  //  000F  ADD	R2	R2	K1
      0x000C0701,  //  0010  ADD	R3	R3	K1
      0x70020005,  //  0011  JMP		#0018
      0x88180100,  //  0012  GETMBR	R6	R0	K0
      0x8C180D0B,  //  0013  GETMET	R6	R6	K11
      0x5C200600,  //  0014  MOVE	R8	R3
      0x7C180400,  //  0015  CALL	R6	2
      0x50180200,  //  0016  LDBOOL	R6	1	0
      0x90023006,  //  0017  SETMBR	R0	K24	R6
      0x7001FFE8,  //  0018  JMP		#0002
      0x1C100502,  //  0019  EQ	R4	R2	K2
      0x78120006,  //  001A  JMPF	R4	#0022
      0x88100118,  //  001B  GETMBR	R4	R0	K24
      0x78120003,  //  001C  JMPF	R4	#0021
      0x8C10011F,  //  001D  GETMET	R4	R0	K31
      0x7C100200,  //  001E  CALL	R4	1
      0x50100000,  //  001F  LDBOOL	R4	0	0
      0x90023004,  //  0020  SETMBR	R0	K24	R4
      0x80000800,  //  0021  RET	0
      0x8C100120,  //  0022  GETMET	R4	R0	K32
      0x88180100,  //  0023  GETMBR	R6	R0	K0
      0x5C1C0200,  //  0024  MOVE	R7	R1
      0x7C100600,  //  0025  CALL	R4	3
      0x50100000,  //  0026  LDBOOL	R4	0	0
      0x90023004,  //  0027  SETMBR	R0	K24	R4
      0x80000000,  //  0028  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: resume_after
********************************************************************/
be_local_closure(class_AnimationEngine_resume_after,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationEngine,     /* shared constants */
    be_str_weak(resume_after),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x8C080121,  //  0000  GETMET	R2	R0	K33
      0x7C080200,  //  0001  CALL	R2	1
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(class_AnimationEngine_start,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        2,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(on_tick),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 4]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0x80040000,  //  0003  RET	1	R0
        })
      ),
    }),
    1,                          /* has constants */
    &be_ktab_class_AnimationEngine,     /* shared constants */
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[51]) {  /* code */
      0x88040108,  //  0000  GETMBR	R1	R0	K8
      0x7406002E,  //  0001  JMPT	R1	#0031
      0x50040200,  //  0002  LDBOOL	R1	1	0
      0x90021001,  //  0003  SETMBR	R0	K8	R1
      0xB8064400,  //  0004  GETNGBL	R1	K34
      0x8C040323,  //  0005  GETMET	R1	R1	K35
      0x7C040200,  //  0006  CALL	R1	1
      0x540A0009,  //  0007  LDINT	R2	10
      0x04040202,  //  0008  SUB	R1	R1	R2
      0x90022A01,  //  0009  SETMBR	R0	K21	R1
      0x88040117,  //  000A  GETMBR	R1	R0	K23
      0x4C080000,  //  000B  LDNIL	R2
      0x1C040202,  //  000C  EQ	R1	R1	R2
      0x78060001,  //  000D  JMPF	R1	#0010
      0x84040000,  //  000E  CLOSURE	R1	P0
      0x90022E01,  //  000F  SETMBR	R0	K23	R1
      0x58040002,  //  0010  LDCONST	R1	K2
      0xB80A4400,  //  0011  GETNGBL	R2	K34
      0x8C080523,  //  0012  GETMET	R2	R2	K35
      0x7C080200,  //  0013  CALL	R2	1
      0x600C000C,  //  0014  GETGBL	R3	G12
      0x88100100,  //  0015  GETMBR	R4	R0	K0
      0x7C0C0200,  //  0016  CALL	R3	1
      0x140C0203,  //  0017  LT	R3	R1	R3
      0x780E0006,  //  0018  JMPF	R3	#0020
      0x880C0100,  //  0019  GETMBR	R3	R0	K0
      0x940C0601,  //  001A  GETIDX	R3	R3	R1
      0x8C0C0724,  //  001B  GETMET	R3	R3	K36
      0x5C140400,  //  001C  MOVE	R5	R2
      0x7C0C0400,  //  001D  CALL	R3	2
      0x00040301,  //  001E  ADD	R1	R1	K1
      0x7001FFF3,  //  001F  JMP		#0014
      0x58040002,  //  0020  LDCONST	R1	K2
      0x600C000C,  //  0021  GETGBL	R3	G12
      0x8810010A,  //  0022  GETMBR	R4	R0	K10
      0x7C0C0200,  //  0023  CALL	R3	1
      0x140C0203,  //  0024  LT	R3	R1	R3
      0x780E0006,  //  0025  JMPF	R3	#002D
      0x880C010A,  //  0026  GETMBR	R3	R0	K10
      0x940C0601,  //  0027  GETIDX	R3	R3	R1
      0x8C0C0724,  //  0028  GETMET	R3	R3	K36
      0x5C140400,  //  0029  MOVE	R5	R2
      0x7C0C0400,  //  002A  CALL	R3	2
      0x00040301,  //  002B  ADD	R1	R1	K1
      0x7001FFF3,  //  002C  JMP		#0021
      0xB80E4400,  //  002D  GETNGBL	R3	K34
      0x8C0C0725,  //  002E  GETMET	R3	R3	K37
      0x88140117,  //  002F  GETMBR	R5	R0	K23
      0x7C0C0400,  //  0030  CALL	R3	2
      0xA0000000,  //  0031  CLOSE	R0
      0x80040000,  //  0032  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_animation
********************************************************************/
be_local_closure(class_AnimationEngine_remove_animation,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationEngine,     /* shared constants */
    be_str_weak(remove_animation),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x5409FFFE,  //  0000  LDINT	R2	-1
      0x580C0002,  //  0001  LDCONST	R3	K2
      0x6010000C,  //  0002  GETGBL	R4	G12
      0x88140100,  //  0003  GETMBR	R5	R0	K0
      0x7C100200,  //  0004  CALL	R4	1
      0x14100604,  //  0005  LT	R4	R3	R4
      0x78120007,  //  0006  JMPF	R4	#000F
      0x88100100,  //  0007  GETMBR	R4	R0	K0
      0x94100803,  //  0008  GETIDX	R4	R4	R3
      0x1C100801,  //  0009  EQ	R4	R4	R1
      0x78120001,  //  000A  JMPF	R4	#000D
      0x5C080600,  //  000B  MOVE	R2	R3
      0x70020001,  //  000C  JMP		#000F
      0x000C0701,  //  000D  ADD	R3	R3	K1
      0x7001FFF2,  //  000E  JMP		#0002
      0x28100502,  //  000F  GE	R4	R2	K2
      0x78120007,  //  0010  JMPF	R4	#0019
      0x88100100,  //  0011  GETMBR	R4	R0	K0
      0x8C10090B,  //  0012  GETMET	R4	R4	K11
      0x5C180400,  //  0013  MOVE	R6	R2
      0x7C100400,  //  0014  CALL	R4	2
      0x50100200,  //  0015  LDBOOL	R4	1	0
      0x90023004,  //  0016  SETMBR	R0	K24	R4
      0x50100200,  //  0017  LDBOOL	R4	1	0
      0x80040800,  //  0018  RET	1	R4
      0x50100000,  //  0019  LDBOOL	R4	0	0
      0x80040800,  //  001A  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_animation
********************************************************************/
be_local_closure(class_AnimationEngine_add_animation,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationEngine,     /* shared constants */
    be_str_weak(add_animation),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x58080002,  //  0000  LDCONST	R2	K2
      0x600C000C,  //  0001  GETGBL	R3	G12
      0x88100100,  //  0002  GETMBR	R4	R0	K0
      0x7C0C0200,  //  0003  CALL	R3	1
      0x140C0403,  //  0004  LT	R3	R2	R3
      0x780E0007,  //  0005  JMPF	R3	#000E
      0x880C0100,  //  0006  GETMBR	R3	R0	K0
      0x940C0602,  //  0007  GETIDX	R3	R3	R2
      0x1C0C0601,  //  0008  EQ	R3	R3	R1
      0x780E0001,  //  0009  JMPF	R3	#000C
      0x500C0000,  //  000A  LDBOOL	R3	0	0
      0x80040600,  //  000B  RET	1	R3
      0x00080501,  //  000C  ADD	R2	R2	K1
      0x7001FFF2,  //  000D  JMP		#0001
      0x880C0100,  //  000E  GETMBR	R3	R0	K0
      0x8C0C070C,  //  000F  GETMET	R3	R3	K12
      0x5C140200,  //  0010  MOVE	R5	R1
      0x7C0C0400,  //  0011  CALL	R3	2
      0x8C0C0126,  //  0012  GETMET	R3	R0	K38
      0x7C0C0200,  //  0013  CALL	R3	1
      0x880C0108,  //  0014  GETMBR	R3	R0	K8
      0x780E0001,  //  0015  JMPF	R3	#0018
      0x8C0C0324,  //  0016  GETMET	R3	R1	K36
      0x7C0C0200,  //  0017  CALL	R3	1
      0x500C0200,  //  0018  LDBOOL	R3	1	0
      0x90023003,  //  0019  SETMBR	R0	K24	R3
      0x500C0200,  //  001A  LDBOOL	R3	1	0
      0x80040600,  //  001B  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: resume
********************************************************************/
be_local_closure(class_AnimationEngine_resume,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationEngine,     /* shared constants */
    be_str_weak(resume),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88040108,  //  0000  GETMBR	R1	R0	K8
      0x74060001,  //  0001  JMPT	R1	#0004
      0x8C040124,  //  0002  GETMET	R1	R0	K36
      0x7C040200,  //  0003  CALL	R1	1
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: check_strip_length
********************************************************************/
be_local_closure(class_AnimationEngine_check_strip_length,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationEngine,     /* shared constants */
    be_str_weak(check_strip_length),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x8C040312,  //  0001  GETMET	R1	R1	K18
      0x7C040200,  //  0002  CALL	R1	1
      0x88080109,  //  0003  GETMBR	R2	R0	K9
      0x20080202,  //  0004  NE	R2	R1	R2
      0x780A0004,  //  0005  JMPF	R2	#000B
      0x8C080127,  //  0006  GETMET	R2	R0	K39
      0x5C100200,  //  0007  MOVE	R4	R1
      0x7C080400,  //  0008  CALL	R2	2
      0x50080200,  //  0009  LDBOOL	R2	1	0
      0x80040400,  //  000A  RET	1	R2
      0x50080000,  //  000B  LDBOOL	R2	0	0
      0x80040400,  //  000C  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_strip_length
********************************************************************/
be_local_closure(class_AnimationEngine_get_strip_length,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationEngine,     /* shared constants */
    be_str_weak(get_strip_length),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C040128,  //  0000  GETMET	R1	R0	K40
      0x7C040200,  //  0001  CALL	R1	1
      0x88040109,  //  0002  GETMBR	R1	R0	K9
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _handle_strip_length_change
********************************************************************/
be_local_closure(class_AnimationEngine__handle_strip_length_change,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationEngine,     /* shared constants */
    be_str_weak(_handle_strip_length_change),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x18080302,  //  0000  LE	R2	R1	K2
      0x780A0000,  //  0001  JMPF	R2	#0003
      0x80000400,  //  0002  RET	0
      0x90021201,  //  0003  SETMBR	R0	K9	R1
      0x8808010E,  //  0004  GETMBR	R2	R0	K14
      0x8C080529,  //  0005  GETMET	R2	R2	K41
      0x5C100200,  //  0006  MOVE	R4	R1
      0x7C080400,  //  0007  CALL	R2	2
      0x88080114,  //  0008  GETMBR	R2	R0	K20
      0x8C080529,  //  0009  GETMET	R2	R2	K41
      0x5C100200,  //  000A  MOVE	R4	R1
      0x7C080400,  //  000B  CALL	R2	2
      0x50080200,  //  000C  LDBOOL	R2	1	0
      0x90023002,  //  000D  SETMBR	R0	K24	R2
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_active
********************************************************************/
be_local_closure(class_AnimationEngine_is_active,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationEngine,     /* shared constants */
    be_str_weak(is_active),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040108,  //  0000  GETMBR	R1	R0	K8
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: interrupt_current
********************************************************************/
be_local_closure(class_AnimationEngine_interrupt_current,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationEngine,     /* shared constants */
    be_str_weak(interrupt_current),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x60040010,  //  0000  GETGBL	R1	G16
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x7C040200,  //  0002  CALL	R1	1
      0xA8020006,  //  0003  EXBLK	0	#000B
      0x5C080200,  //  0004  MOVE	R2	R1
      0x7C080000,  //  0005  CALL	R2	0
      0x880C0508,  //  0006  GETMBR	R3	R2	K8
      0x780E0001,  //  0007  JMPF	R3	#000A
      0x8C0C0519,  //  0008  GETMET	R3	R2	K25
      0x7C0C0200,  //  0009  CALL	R3	1
      0x7001FFF8,  //  000A  JMP		#0004
      0x5804002A,  //  000B  LDCONST	R1	K42
      0xAC040200,  //  000C  CATCH	R1	1	0
      0xB0080000,  //  000D  RAISE	2	R0	R0
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: cleanup
********************************************************************/
be_local_closure(class_AnimationEngine_cleanup,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationEngine,     /* shared constants */
    be_str_weak(cleanup),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x8C040119,  //  0000  GETMET	R1	R0	K25
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040105,  //  0002  GETMET	R1	R0	K5
      0x7C040200,  //  0003  CALL	R1	1
      0x4C040000,  //  0004  LDNIL	R1
      0x90021C01,  //  0005  SETMBR	R0	K14	R1
      0x4C040000,  //  0006  LDNIL	R1
      0x90022801,  //  0007  SETMBR	R0	K20	R1
      0x4C040000,  //  0008  LDNIL	R1
      0x90020801,  //  0009  SETMBR	R0	K4	R1
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: on_tick
********************************************************************/
be_local_closure(class_AnimationEngine_on_tick,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationEngine,     /* shared constants */
    be_str_weak(on_tick),
    &be_const_str_solidified,
    ( &(const binstruction[54]) {  /* code */
      0x88080108,  //  0000  GETMBR	R2	R0	K8
      0x740A0001,  //  0001  JMPT	R2	#0004
      0x50080000,  //  0002  LDBOOL	R2	0	0
      0x80040400,  //  0003  RET	1	R2
      0x4C080000,  //  0004  LDNIL	R2
      0x1C080202,  //  0005  EQ	R2	R1	R2
      0x780A0003,  //  0006  JMPF	R2	#000B
      0xB80A4400,  //  0007  GETNGBL	R2	K34
      0x8C080523,  //  0008  GETMET	R2	R2	K35
      0x7C080200,  //  0009  CALL	R2	1
      0x5C040400,  //  000A  MOVE	R1	R2
      0x8C080128,  //  000B  GETMET	R2	R0	K40
      0x7C080200,  //  000C  CALL	R2	1
      0x90022C01,  //  000D  SETMBR	R0	K22	R1
      0x88080115,  //  000E  GETMBR	R2	R0	K21
      0x04080202,  //  000F  SUB	R2	R1	R2
      0x540E0004,  //  0010  LDINT	R3	5
      0x140C0403,  //  0011  LT	R3	R2	R3
      0x780E0001,  //  0012  JMPF	R3	#0015
      0x500C0200,  //  0013  LDBOOL	R3	1	0
      0x80040600,  //  0014  RET	1	R3
      0x90022A01,  //  0015  SETMBR	R0	K21	R1
      0x880C0104,  //  0016  GETMBR	R3	R0	K4
      0x880C072B,  //  0017  GETMBR	R3	R3	K43
      0x4C100000,  //  0018  LDNIL	R4
      0x200C0604,  //  0019  NE	R3	R3	R4
      0x780E0005,  //  001A  JMPF	R3	#0021
      0x880C0104,  //  001B  GETMBR	R3	R0	K4
      0x8C0C072B,  //  001C  GETMET	R3	R3	K43
      0x7C0C0200,  //  001D  CALL	R3	1
      0x740E0001,  //  001E  JMPT	R3	#0021
      0x500C0200,  //  001F  LDBOOL	R3	1	0
      0x80040600,  //  0020  RET	1	R3
      0x580C0002,  //  0021  LDCONST	R3	K2
      0x6010000C,  //  0022  GETGBL	R4	G12
      0x8814010A,  //  0023  GETMBR	R5	R0	K10
      0x7C100200,  //  0024  CALL	R4	1
      0x14100604,  //  0025  LT	R4	R3	R4
      0x78120006,  //  0026  JMPF	R4	#002E
      0x8810010A,  //  0027  GETMBR	R4	R0	K10
      0x94100803,  //  0028  GETIDX	R4	R4	R3
      0x8C10091E,  //  0029  GETMET	R4	R4	K30
      0x5C180200,  //  002A  MOVE	R6	R1
      0x7C100400,  //  002B  CALL	R4	2
      0x000C0701,  //  002C  ADD	R3	R3	K1
      0x7001FFF3,  //  002D  JMP		#0022
      0x8C10012C,  //  002E  GETMET	R4	R0	K44
      0x5C180200,  //  002F  MOVE	R6	R1
      0x7C100400,  //  0030  CALL	R4	2
      0x8C10012D,  //  0031  GETMET	R4	R0	K45
      0x5C180200,  //  0032  MOVE	R6	R1
      0x7C100400,  //  0033  CALL	R4	2
      0x50100200,  //  0034  LDBOOL	R4	1	0
      0x80040800,  //  0035  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: stop
********************************************************************/
be_local_closure(class_AnimationEngine_stop,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationEngine,     /* shared constants */
    be_str_weak(stop),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x88040108,  //  0000  GETMBR	R1	R0	K8
      0x78060009,  //  0001  JMPF	R1	#000C
      0x50040000,  //  0002  LDBOOL	R1	0	0
      0x90021001,  //  0003  SETMBR	R0	K8	R1
      0x88040117,  //  0004  GETMBR	R1	R0	K23
      0x4C080000,  //  0005  LDNIL	R2
      0x20040202,  //  0006  NE	R1	R1	R2
      0x78060003,  //  0007  JMPF	R1	#000C
      0xB8064400,  //  0008  GETNGBL	R1	K34
      0x8C04032E,  //  0009  GETMET	R1	R1	K46
      0x880C0117,  //  000A  GETMBR	R3	R0	K23
      0x7C040400,  //  000B  CALL	R1	2
      0x80040000,  //  000C  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_animations
********************************************************************/
be_local_closure(class_AnimationEngine_get_animations,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationEngine,     /* shared constants */
    be_str_weak(get_animations),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _process_events
********************************************************************/
be_local_closure(class_AnimationEngine__process_events,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationEngine,     /* shared constants */
    be_str_weak(_process_events),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0xB80A2600,  //  0000  GETNGBL	R2	K19
      0x8808052F,  //  0001  GETMBR	R2	R2	K47
      0x4C0C0000,  //  0002  LDNIL	R3
      0x20080403,  //  0003  NE	R2	R2	R3
      0x780A0003,  //  0004  JMPF	R2	#0009
      0xB80A2600,  //  0005  GETNGBL	R2	K19
      0x8808052F,  //  0006  GETMBR	R2	R2	K47
      0x8C080530,  //  0007  GETMET	R2	R2	K48
      0x7C080200,  //  0008  CALL	R2	1
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: AnimationEngine
********************************************************************/
be_local_class(AnimationEngine,
    11,
    NULL,
    be_nested_map(40,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(_sort_animations, -1), be_const_closure(class_AnimationEngine__sort_animations_closure) },
        { be_const_key_weak(_process_events, -1), be_const_closure(class_AnimationEngine__process_events_closure) },
        { be_const_key_weak(get_animations, -1), be_const_closure(class_AnimationEngine_get_animations_closure) },
        { be_const_key_weak(width, -1), be_const_var(1) },
        { be_const_key_weak(size, 39), be_const_closure(class_AnimationEngine_size_closure) },
        { be_const_key_weak(tostring, 1), be_const_closure(class_AnimationEngine_tostring_closure) },
        { be_const_key_weak(_clear_strip, 38), be_const_closure(class_AnimationEngine__clear_strip_closure) },
        { be_const_key_weak(remove_sequence_manager, 2), be_const_closure(class_AnimationEngine_remove_sequence_manager_closure) },
        { be_const_key_weak(add_sequence_manager, -1), be_const_closure(class_AnimationEngine_add_sequence_manager_closure) },
        { be_const_key_weak(on_tick, -1), be_const_closure(class_AnimationEngine_on_tick_closure) },
        { be_const_key_weak(cleanup, -1), be_const_closure(class_AnimationEngine_cleanup_closure) },
        { be_const_key_weak(interrupt_current, -1), be_const_closure(class_AnimationEngine_interrupt_current_closure) },
        { be_const_key_weak(_render_animations, -1), be_const_closure(class_AnimationEngine__render_animations_closure) },
        { be_const_key_weak(interrupt_animation, 17), be_const_closure(class_AnimationEngine_interrupt_animation_closure) },
        { be_const_key_weak(_update_and_render, -1), be_const_closure(class_AnimationEngine__update_and_render_closure) },
        { be_const_key_weak(resume_after, 3), be_const_closure(class_AnimationEngine_resume_after_closure) },
        { be_const_key_weak(sequence_managers, -1), be_const_var(3) },
        { be_const_key_weak(time_ms, -1), be_const_var(8) },
        { be_const_key_weak(temp_buffer, -1), be_const_var(5) },
        { be_const_key_weak(_handle_strip_length_change, -1), be_const_closure(class_AnimationEngine__handle_strip_length_change_closure) },
        { be_const_key_weak(remove_animation, 28), be_const_closure(class_AnimationEngine_remove_animation_closure) },
        { be_const_key_weak(is_running, 16), be_const_var(6) },
        { be_const_key_weak(get_strip_length, 24), be_const_closure(class_AnimationEngine_get_strip_length_closure) },
        { be_const_key_weak(render_needed, -1), be_const_var(10) },
        { be_const_key_weak(last_update, -1), be_const_var(7) },
        { be_const_key_weak(add_animation, 30), be_const_closure(class_AnimationEngine_add_animation_closure) },
        { be_const_key_weak(animations, 29), be_const_var(2) },
        { be_const_key_weak(check_strip_length, 22), be_const_closure(class_AnimationEngine_check_strip_length_closure) },
        { be_const_key_weak(fast_loop_closure, -1), be_const_var(9) },
        { be_const_key_weak(resume, -1), be_const_closure(class_AnimationEngine_resume_closure) },
        { be_const_key_weak(frame_buffer, -1), be_const_var(4) },
        { be_const_key_weak(start, 19), be_const_closure(class_AnimationEngine_start_closure) },
        { be_const_key_weak(is_active, -1), be_const_closure(class_AnimationEngine_is_active_closure) },
        { be_const_key_weak(strip, -1), be_const_var(0) },
        { be_const_key_weak(clear, 11), be_const_closure(class_AnimationEngine_clear_closure) },
        { be_const_key_weak(init, 10), be_const_closure(class_AnimationEngine_init_closure) },
        { be_const_key_weak(_output_to_strip, 9), be_const_closure(class_AnimationEngine__output_to_strip_closure) },
        { be_const_key_weak(stop, -1), be_const_closure(class_AnimationEngine_stop_closure) },
        { be_const_key_weak(get_strip, -1), be_const_closure(class_AnimationEngine_get_strip_closure) },
        { be_const_key_weak(interrupt_all, -1), be_const_closure(class_AnimationEngine_interrupt_all_closure) },
    })),
    be_str_weak(AnimationEngine)
);
// compact class 'StripLengthProvider' ktab size: 5, total: 8 (saved 24 bytes)
static const bvalue be_ktab_class_StripLengthProvider[5] = {
  /* K0   */  be_nested_str_weak(engine),
  /* K1   */  be_const_int(0),
  /* K2   */  be_nested_str_weak(width),
  /* K3   */  be_nested_str_weak(StripLengthProvider_X28length_X3D_X25s_X29),
  /* K4   */  be_nested_str_weak(StripLengthProvider_X28length_X3Dunknown_X29),
};


extern const bclass be_class_StripLengthProvider;

/********************************************************************
** Solidified function: produce_value
********************************************************************/
be_local_closure(class_StripLengthProvider_produce_value,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_StripLengthProvider,     /* shared constants */
    be_str_weak(produce_value),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x4C100000,  //  0001  LDNIL	R4
      0x1C0C0604,  //  0002  EQ	R3	R3	R4
      0x780E0000,  //  0003  JMPF	R3	#0005
      0x80060200,  //  0004  RET	1	K1
      0x880C0100,  //  0005  GETMBR	R3	R0	K0
      0x880C0702,  //  0006  GETMBR	R3	R3	K2
      0x80040600,  //  0007  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_StripLengthProvider_tostring,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_StripLengthProvider,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0xA802000F,  //  0000  EXBLK	0	#0011
      0x88040100,  //  0001  GETMBR	R1	R0	K0
      0x4C080000,  //  0002  LDNIL	R2
      0x20040202,  //  0003  NE	R1	R1	R2
      0x78060002,  //  0004  JMPF	R1	#0008
      0x88040100,  //  0005  GETMBR	R1	R0	K0
      0x88040302,  //  0006  GETMBR	R1	R1	K2
      0x70020000,  //  0007  JMP		#0009
      0x58040001,  //  0008  LDCONST	R1	K1
      0x60080018,  //  0009  GETGBL	R2	G24
      0x580C0003,  //  000A  LDCONST	R3	K3
      0x5C100200,  //  000B  MOVE	R4	R1
      0x7C080400,  //  000C  CALL	R2	2
      0xA8040001,  //  000D  EXBLK	1	1
      0x80040400,  //  000E  RET	1	R2
      0xA8040001,  //  000F  EXBLK	1	1
      0x70020004,  //  0010  JMP		#0016
      0xAC040000,  //  0011  CATCH	R1	0	0
      0x70020001,  //  0012  JMP		#0015
      0x80060800,  //  0013  RET	1	K4
      0x70020000,  //  0014  JMP		#0016
      0xB0080000,  //  0015  RAISE	2	R0	R0
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: StripLengthProvider
********************************************************************/
extern const bclass be_class_ValueProvider;
be_local_class(StripLengthProvider,
    0,
    &be_class_ValueProvider,
    be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(tostring, -1), be_const_closure(class_StripLengthProvider_tostring_closure) },
        { be_const_key_weak(produce_value, 0), be_const_closure(class_StripLengthProvider_produce_value_closure) },
    })),
    be_str_weak(StripLengthProvider)
);

/********************************************************************
** Solidified function: shift_scroll_left
********************************************************************/
be_local_closure(shift_scroll_left,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(shift_animation),
    /* K2   */  be_nested_str_weak(direction),
    /* K3   */  be_nested_str_weak(shift_speed),
    /* K4   */  be_nested_str_weak(wrap_around),
    }),
    be_str_weak(shift_scroll_left),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x5409FFFE,  //  0004  LDINT	R2	-1
      0x90060402,  //  0005  SETMBR	R1	K2	R2
      0x540A007F,  //  0006  LDINT	R2	128
      0x90060602,  //  0007  SETMBR	R1	K3	R2
      0x50080200,  //  0008  LDBOOL	R2	1	0
      0x90060802,  //  0009  SETMBR	R1	K4	R2
      0x80040200,  //  000A  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: sparkle_rainbow
********************************************************************/
be_local_closure(sparkle_rainbow,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(rich_palette),
    /* K2   */  be_nested_str_weak(palette),
    /* K3   */  be_nested_str_weak(PALETTE_RAINBOW),
    /* K4   */  be_nested_str_weak(cycle_period),
    /* K5   */  be_nested_str_weak(transition_type),
    /* K6   */  be_const_int(1),
    /* K7   */  be_nested_str_weak(sparkle_animation),
    /* K8   */  be_nested_str_weak(color),
    /* K9   */  be_nested_str_weak(name),
    /* K10  */  be_nested_str_weak(sparkle_rainbow),
    }),
    be_str_weak(sparkle_rainbow),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0xB80A0000,  //  0004  GETNGBL	R2	K0
      0x88080503,  //  0005  GETMBR	R2	R2	K3
      0x90060402,  //  0006  SETMBR	R1	K2	R2
      0x540A1387,  //  0007  LDINT	R2	5000
      0x90060802,  //  0008  SETMBR	R1	K4	R2
      0x90060B06,  //  0009  SETMBR	R1	K5	K6
      0xB80A0000,  //  000A  GETNGBL	R2	K0
      0x8C080507,  //  000B  GETMET	R2	R2	K7
      0x5C100000,  //  000C  MOVE	R4	R0
      0x7C080400,  //  000D  CALL	R2	2
      0x900A1001,  //  000E  SETMBR	R2	K8	R1
      0x900A130A,  //  000F  SETMBR	R2	K9	K10
      0x80040400,  //  0010  RET	1	R2
    })
  )
);
/*******************************************************************/

// compact class 'CrenelPositionAnimation' ktab size: 21, total: 27 (saved 48 bytes)
static const bvalue be_ktab_class_CrenelPositionAnimation[21] = {
  /* K0   */  be_nested_str_weak(is_running),
  /* K1   */  be_nested_str_weak(engine),
  /* K2   */  be_nested_str_weak(time_ms),
  /* K3   */  be_nested_str_weak(width),
  /* K4   */  be_nested_str_weak(back_color),
  /* K5   */  be_nested_str_weak(pos),
  /* K6   */  be_nested_str_weak(pulse_size),
  /* K7   */  be_nested_str_weak(low_size),
  /* K8   */  be_nested_str_weak(nb_pulse),
  /* K9   */  be_nested_str_weak(color),
  /* K10  */  be_const_int(-16777216),
  /* K11  */  be_nested_str_weak(fill_pixels),
  /* K12  */  be_const_int(0),
  /* K13  */  be_const_int(1),
  /* K14  */  be_nested_str_weak(set_pixel_color),
  /* K15  */  be_nested_str_weak(get_param),
  /* K16  */  be_nested_str_weak(animation),
  /* K17  */  be_nested_str_weak(is_value_provider),
  /* K18  */  be_nested_str_weak(0x_X2508x),
  /* K19  */  be_nested_str_weak(CrenelPositionAnimation_X28color_X3D_X25s_X2C_X20pos_X3D_X25s_X2C_X20pulse_size_X3D_X25s_X2C_X20low_size_X3D_X25s_X2C_X20nb_pulse_X3D_X25s_X2C_X20priority_X3D_X25s_X2C_X20running_X3D_X25s_X29),
  /* K20  */  be_nested_str_weak(priority),
};


extern const bclass be_class_CrenelPositionAnimation;

/********************************************************************
** Solidified function: render
********************************************************************/
be_local_closure(class_CrenelPositionAnimation_render,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_CrenelPositionAnimation,     /* shared constants */
    be_str_weak(render),
    &be_const_str_solidified,
    ( &(const binstruction[76]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x780E0002,  //  0001  JMPF	R3	#0005
      0x4C0C0000,  //  0002  LDNIL	R3
      0x1C0C0203,  //  0003  EQ	R3	R1	R3
      0x780E0001,  //  0004  JMPF	R3	#0007
      0x500C0000,  //  0005  LDBOOL	R3	0	0
      0x80040600,  //  0006  RET	1	R3
      0x4C0C0000,  //  0007  LDNIL	R3
      0x1C0C0403,  //  0008  EQ	R3	R2	R3
      0x780E0001,  //  0009  JMPF	R3	#000C
      0x880C0101,  //  000A  GETMBR	R3	R0	K1
      0x88080702,  //  000B  GETMBR	R2	R3	K2
      0x880C0303,  //  000C  GETMBR	R3	R1	K3
      0x88100104,  //  000D  GETMBR	R4	R0	K4
      0x88140105,  //  000E  GETMBR	R5	R0	K5
      0x88180106,  //  000F  GETMBR	R6	R0	K6
      0x881C0107,  //  0010  GETMBR	R7	R0	K7
      0x88200108,  //  0011  GETMBR	R8	R0	K8
      0x88240109,  //  0012  GETMBR	R9	R0	K9
      0x60280009,  //  0013  GETGBL	R10	G9
      0x002C0C07,  //  0014  ADD	R11	R6	R7
      0x7C280200,  //  0015  CALL	R10	1
      0x202C090A,  //  0016  NE	R11	R4	K10
      0x782E0002,  //  0017  JMPF	R11	#001B
      0x8C2C030B,  //  0018  GETMET	R11	R1	K11
      0x5C340800,  //  0019  MOVE	R13	R4
      0x7C2C0400,  //  001A  CALL	R11	2
      0x182C150C,  //  001B  LE	R11	R10	K12
      0x782E0000,  //  001C  JMPF	R11	#001E
      0x5828000D,  //  001D  LDCONST	R10	K13
      0x1C2C110C,  //  001E  EQ	R11	R8	K12
      0x782E0001,  //  001F  JMPF	R11	#0022
      0x502C0200,  //  0020  LDBOOL	R11	1	0
      0x80041600,  //  0021  RET	1	R11
      0x142C110C,  //  0022  LT	R11	R8	K12
      0x782E0006,  //  0023  JMPF	R11	#002B
      0x002C0A06,  //  0024  ADD	R11	R5	R6
      0x042C170D,  //  0025  SUB	R11	R11	K13
      0x102C160A,  //  0026  MOD	R11	R11	R10
      0x042C1606,  //  0027  SUB	R11	R11	R6
      0x002C170D,  //  0028  ADD	R11	R11	K13
      0x5C141600,  //  0029  MOVE	R5	R11
      0x70020007,  //  002A  JMP		#0033
      0x442C1400,  //  002B  NEG	R11	R10
      0x142C0A0B,  //  002C  LT	R11	R5	R11
      0x782E0004,  //  002D  JMPF	R11	#0033
      0x202C110C,  //  002E  NE	R11	R8	K12
      0x782E0002,  //  002F  JMPF	R11	#0033
      0x00140A0A,  //  0030  ADD	R5	R5	R10
      0x0420110D,  //  0031  SUB	R8	R8	K13
      0x7001FFF7,  //  0032  JMP		#002B
      0x142C0A03,  //  0033  LT	R11	R5	R3
      0x782E0014,  //  0034  JMPF	R11	#004A
      0x202C110C,  //  0035  NE	R11	R8	K12
      0x782E0012,  //  0036  JMPF	R11	#004A
      0x582C000C,  //  0037  LDCONST	R11	K12
      0x14300B0C,  //  0038  LT	R12	R5	K12
      0x78320001,  //  0039  JMPF	R12	#003C
      0x44300A00,  //  003A  NEG	R12	R5
      0x5C2C1800,  //  003B  MOVE	R11	R12
      0x14301606,  //  003C  LT	R12	R11	R6
      0x78320008,  //  003D  JMPF	R12	#0047
      0x00300A0B,  //  003E  ADD	R12	R5	R11
      0x14301803,  //  003F  LT	R12	R12	R3
      0x78320005,  //  0040  JMPF	R12	#0047
      0x8C30030E,  //  0041  GETMET	R12	R1	K14
      0x00380A0B,  //  0042  ADD	R14	R5	R11
      0x5C3C1200,  //  0043  MOVE	R15	R9
      0x7C300600,  //  0044  CALL	R12	3
      0x002C170D,  //  0045  ADD	R11	R11	K13
      0x7001FFF4,  //  0046  JMP		#003C
      0x00140A0A,  //  0047  ADD	R5	R5	R10
      0x0420110D,  //  0048  SUB	R8	R8	K13
      0x7001FFE8,  //  0049  JMP		#0033
      0x502C0200,  //  004A  LDBOOL	R11	1	0
      0x80041600,  //  004B  RET	1	R11
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_CrenelPositionAnimation_tostring,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_CrenelPositionAnimation,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[30]) {  /* code */
      0x4C040000,  //  0000  LDNIL	R1
      0x8C08010F,  //  0001  GETMET	R2	R0	K15
      0x58100009,  //  0002  LDCONST	R4	K9
      0x7C080400,  //  0003  CALL	R2	2
      0xB80E2000,  //  0004  GETNGBL	R3	K16
      0x8C0C0711,  //  0005  GETMET	R3	R3	K17
      0x5C140400,  //  0006  MOVE	R5	R2
      0x7C0C0400,  //  0007  CALL	R3	2
      0x780E0004,  //  0008  JMPF	R3	#000E
      0x600C0008,  //  0009  GETGBL	R3	G8
      0x5C100400,  //  000A  MOVE	R4	R2
      0x7C0C0200,  //  000B  CALL	R3	1
      0x5C040600,  //  000C  MOVE	R1	R3
      0x70020004,  //  000D  JMP		#0013
      0x600C0018,  //  000E  GETGBL	R3	G24
      0x58100012,  //  000F  LDCONST	R4	K18
      0x88140109,  //  0010  GETMBR	R5	R0	K9
      0x7C0C0400,  //  0011  CALL	R3	2
      0x5C040600,  //  0012  MOVE	R1	R3
      0x600C0018,  //  0013  GETGBL	R3	G24
      0x58100013,  //  0014  LDCONST	R4	K19
      0x5C140200,  //  0015  MOVE	R5	R1
      0x88180105,  //  0016  GETMBR	R6	R0	K5
      0x881C0106,  //  0017  GETMBR	R7	R0	K6
      0x88200107,  //  0018  GETMBR	R8	R0	K7
      0x88240108,  //  0019  GETMBR	R9	R0	K8
      0x88280114,  //  001A  GETMBR	R10	R0	K20
      0x882C0100,  //  001B  GETMBR	R11	R0	K0
      0x7C0C1000,  //  001C  CALL	R3	8
      0x80040600,  //  001D  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: CrenelPositionAnimation
********************************************************************/
extern const bclass be_class_Animation;
be_local_class(CrenelPositionAnimation,
    0,
    &be_class_Animation,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(PARAMS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(color, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(-1) },
    }))    ) } )) },
        { be_const_key_weak(nb_pulse, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(-1) },
    }))    ) } )) },
        { be_const_key_weak(pulse_size, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(1) },
        { be_const_key_weak(min, -1), be_const_int(0) },
    }))    ) } )) },
        { be_const_key_weak(pos, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(0) },
    }))    ) } )) },
        { be_const_key_weak(back_color, 0), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(-16777216) },
    }))    ) } )) },
        { be_const_key_weak(low_size, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(3) },
        { be_const_key_weak(min, -1), be_const_int(0) },
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(render, 2), be_const_closure(class_CrenelPositionAnimation_render_closure) },
        { be_const_key_weak(tostring, -1), be_const_closure(class_CrenelPositionAnimation_tostring_closure) },
    })),
    be_str_weak(CrenelPositionAnimation)
);

/********************************************************************
** Solidified function: plasma_rainbow
********************************************************************/
be_local_closure(plasma_rainbow,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(plasma_animation),
    /* K2   */  be_nested_str_weak(color),
    /* K3   */  be_nested_str_weak(time_speed),
    /* K4   */  be_nested_str_weak(name),
    /* K5   */  be_nested_str_weak(plasma_rainbow),
    }),
    be_str_weak(plasma_rainbow),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x4C080000,  //  0004  LDNIL	R2
      0x90060402,  //  0005  SETMBR	R1	K2	R2
      0x540A0031,  //  0006  LDINT	R2	50
      0x90060602,  //  0007  SETMBR	R1	K3	R2
      0x90060905,  //  0008  SETMBR	R1	K4	K5
      0x80040200,  //  0009  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: animation_init
********************************************************************/
be_local_closure(animation_init,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        7,                          /* nstack */
        1,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 5]) {     /* constants */
        /* K0   */  be_nested_str_weak(animation),
        /* K1   */  be_nested_str_weak(introspect),
        /* K2   */  be_nested_str_weak(contains),
        /* K3   */  be_nested_str_weak(_ntv),
        /* K4   */  be_nested_str_weak(undefined),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[16]) {  /* code */
          0xA4060000,  //  0000  IMPORT	R1	K0
          0xA40A0200,  //  0001  IMPORT	R2	K1
          0x8C0C0502,  //  0002  GETMET	R3	R2	K2
          0x88140303,  //  0003  GETMBR	R5	R1	K3
          0x5C180000,  //  0004  MOVE	R6	R0
          0x7C0C0600,  //  0005  CALL	R3	3
          0x780E0003,  //  0006  JMPF	R3	#000B
          0x880C0303,  //  0007  GETMBR	R3	R1	K3
          0x880C0600,  //  0008  GETMBR	R3	R3	R0
          0x80040600,  //  0009  RET	1	R3
          0x70020003,  //  000A  JMP		#000F
          0x600C000B,  //  000B  GETGBL	R3	G11
          0x58100004,  //  000C  LDCONST	R4	K4
          0x7C0C0200,  //  000D  CALL	R3	1
          0x80040600,  //  000E  RET	1	R3
          0x80000000,  //  000F  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(_ntv),
    /* K2   */  be_nested_str_weak(event_manager),
    /* K3   */  be_nested_str_weak(EventManager),
    /* K4   */  be_nested_str_weak(member),
    }),
    be_str_weak(animation_init),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x6004000B,  //  0000  GETGBL	R1	G11
      0x58080000,  //  0001  LDCONST	R2	K0
      0x7C040200,  //  0002  CALL	R1	1
      0x90060200,  //  0003  SETMBR	R1	K1	R0
      0x8C080103,  //  0004  GETMET	R2	R0	K3
      0x7C080200,  //  0005  CALL	R2	1
      0x90060402,  //  0006  SETMBR	R1	K2	R2
      0x84080000,  //  0007  CLOSURE	R2	P0
      0x90060802,  //  0008  SETMBR	R1	K4	R2
      0x80040200,  //  0009  RET	1	R1
    })
  )
);
/*******************************************************************/

// compact class 'ParameterizedObject' ktab size: 45, total: 90 (saved 360 bytes)
static const bvalue be_ktab_class_ParameterizedObject[45] = {
  /* K0   */  be_nested_str_weak(introspect),
  /* K1   */  be_nested_str_weak(contains),
  /* K2   */  be_nested_str_weak(PARAMS),
  /* K3   */  be_nested_str_weak(keys),
  /* K4   */  be_nested_str_weak(stop_iteration),
  /* K5   */  be_nested_str_weak(_get_param_def),
  /* K6   */  be_nested_str_weak(values),
  /* K7   */  be_nested_str_weak(_has_param),
  /* K8   */  be_nested_str_weak(_resolve_parameter_value),
  /* K9   */  be_nested_str_weak(engine),
  /* K10  */  be_nested_str_weak(time_ms),
  /* K11  */  be_nested_str_weak(_X27_X25s_X27_X20object_X20has_X20no_X20attribute_X20_X27_X25s_X27),
  /* K12  */  be_nested_str_weak(attribute_error),
  /* K13  */  be_nested_str_weak(_set_parameter_value),
  /* K14  */  be_nested_str_weak(instance),
  /* K15  */  be_nested_str_weak(value_error),
  /* K16  */  be_nested_str_weak(ParameterizedObject_X20requires_X20an_X20engine_X20parameter),
  /* K17  */  be_nested_str_weak(_init_parameter_values),
  /* K18  */  be_nested_str_weak(default),
  /* K19  */  be_nested_str_weak(Parameter_X20_X27_X25s_X27_X20is_X20not_X20defined_X20for_X20class_X20_X27_X25s_X27),
  /* K20  */  be_nested_str_weak(animation),
  /* K21  */  be_nested_str_weak(is_value_provider),
  /* K22  */  be_nested_str_weak(nillable),
  /* K23  */  be_nested_str_weak(Parameter_X20_X27_X25s_X27_X20does_X20not_X20accept_X20nil_X20values),
  /* K24  */  be_nested_str_weak(int),
  /* K25  */  be_nested_str_weak(type),
  /* K26  */  be_nested_str_weak(any),
  /* K27  */  be_nested_str_weak(real),
  /* K28  */  be_nested_str_weak(math),
  /* K29  */  be_nested_str_weak(round),
  /* K30  */  be_nested_str_weak(bytes),
  /* K31  */  be_nested_str_weak(Parameter_X20_X27_X25s_X27_X20expects_X20type_X20_X27_X25s_X27_X20but_X20got_X20_X27_X25s_X27_X20_X28value_X3A_X20_X25s_X29),
  /* K32  */  be_nested_str_weak(min),
  /* K33  */  be_nested_str_weak(Parameter_X20_X27_X25s_X27_X20value_X20_X25s_X20is_X20below_X20minimum_X20_X25s),
  /* K34  */  be_nested_str_weak(max),
  /* K35  */  be_nested_str_weak(Parameter_X20_X27_X25s_X27_X20value_X20_X25s_X20is_X20above_X20maximum_X20_X25s),
  /* K36  */  be_nested_str_weak(enum),
  /* K37  */  be_nested_str_weak(size),
  /* K38  */  be_const_int(0),
  /* K39  */  be_const_int(1),
  /* K40  */  be_nested_str_weak(Parameter_X20_X27_X25s_X27_X20value_X20_X25s_X20is_X20not_X20in_X20allowed_X20values_X20_X25s),
  /* K41  */  be_nested_str_weak(toptr),
  /* K42  */  be_nested_str_weak(produce_value),
  /* K43  */  be_nested_str_weak(_validate_param),
  /* K44  */  be_nested_str_weak(on_param_changed),
};


extern const bclass be_class_ParameterizedObject;

/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(class_ParameterizedObject_start,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ParameterizedObject,     /* shared constants */
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80040000,  //  0000  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_params_metadata
********************************************************************/
be_local_closure(class_ParameterizedObject_get_params_metadata,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ParameterizedObject,     /* shared constants */
    be_str_weak(get_params_metadata),
    &be_const_str_solidified,
    ( &(const binstruction[38]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x60080013,  //  0001  GETGBL	R2	G19
      0x7C080000,  //  0002  CALL	R2	0
      0x600C0006,  //  0003  GETGBL	R3	G6
      0x5C100000,  //  0004  MOVE	R4	R0
      0x7C0C0200,  //  0005  CALL	R3	1
      0x4C100000,  //  0006  LDNIL	R4
      0x20100604,  //  0007  NE	R4	R3	R4
      0x7812001B,  //  0008  JMPF	R4	#0025
      0x8C100301,  //  0009  GETMET	R4	R1	K1
      0x5C180600,  //  000A  MOVE	R6	R3
      0x581C0002,  //  000B  LDCONST	R7	K2
      0x7C100600,  //  000C  CALL	R4	3
      0x78120011,  //  000D  JMPF	R4	#0020
      0x88100702,  //  000E  GETMBR	R4	R3	K2
      0x60140010,  //  000F  GETGBL	R5	G16
      0x8C180903,  //  0010  GETMET	R6	R4	K3
      0x7C180200,  //  0011  CALL	R6	1
      0x7C140200,  //  0012  CALL	R5	1
      0xA8020008,  //  0013  EXBLK	0	#001D
      0x5C180A00,  //  0014  MOVE	R6	R5
      0x7C180000,  //  0015  CALL	R6	0
      0x8C1C0501,  //  0016  GETMET	R7	R2	K1
      0x5C240C00,  //  0017  MOVE	R9	R6
      0x7C1C0400,  //  0018  CALL	R7	2
      0x741E0001,  //  0019  JMPT	R7	#001C
      0x941C0806,  //  001A  GETIDX	R7	R4	R6
      0x98080C07,  //  001B  SETIDX	R2	R6	R7
      0x7001FFF6,  //  001C  JMP		#0014
      0x58140004,  //  001D  LDCONST	R5	K4
      0xAC140200,  //  001E  CATCH	R5	1	0
      0xB0080000,  //  001F  RAISE	2	R0	R0
      0x60100003,  //  0020  GETGBL	R4	G3
      0x5C140600,  //  0021  MOVE	R5	R3
      0x7C100200,  //  0022  CALL	R4	1
      0x5C0C0800,  //  0023  MOVE	R3	R4
      0x7001FFE0,  //  0024  JMP		#0006
      0x80040400,  //  0025  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_param_metadata
********************************************************************/
be_local_closure(class_ParameterizedObject_get_param_metadata,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ParameterizedObject,     /* shared constants */
    be_str_weak(get_param_metadata),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C080105,  //  0000  GETMET	R2	R0	K5
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x80040400,  //  0003  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: member
********************************************************************/
be_local_closure(class_ParameterizedObject_member,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ParameterizedObject,     /* shared constants */
    be_str_weak(member),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x88080106,  //  0000  GETMBR	R2	R0	K6
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x740A0003,  //  0004  JMPT	R2	#0009
      0x8C080107,  //  0005  GETMET	R2	R0	K7
      0x5C100200,  //  0006  MOVE	R4	R1
      0x7C080400,  //  0007  CALL	R2	2
      0x780A0005,  //  0008  JMPF	R2	#000F
      0x8C080108,  //  0009  GETMET	R2	R0	K8
      0x5C100200,  //  000A  MOVE	R4	R1
      0x88140109,  //  000B  GETMBR	R5	R0	K9
      0x88140B0A,  //  000C  GETMBR	R5	R5	K10
      0x7C080600,  //  000D  CALL	R2	3
      0x80040400,  //  000E  RET	1	R2
      0x60080018,  //  000F  GETGBL	R2	G24
      0x580C000B,  //  0010  LDCONST	R3	K11
      0x60100005,  //  0011  GETGBL	R4	G5
      0x5C140000,  //  0012  MOVE	R5	R0
      0x7C100200,  //  0013  CALL	R4	1
      0x5C140200,  //  0014  MOVE	R5	R1
      0x7C080600,  //  0015  CALL	R2	3
      0xB0061802,  //  0016  RAISE	1	K12	R2
      0x80000000,  //  0017  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: on_param_changed
********************************************************************/
be_local_closure(class_ParameterizedObject_on_param_changed,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ParameterizedObject,     /* shared constants */
    be_str_weak(on_param_changed),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _has_param
********************************************************************/
be_local_closure(class_ParameterizedObject__has_param,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ParameterizedObject,     /* shared constants */
    be_str_weak(_has_param),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x600C0006,  //  0001  GETGBL	R3	G6
      0x5C100000,  //  0002  MOVE	R4	R0
      0x7C0C0200,  //  0003  CALL	R3	1
      0x4C100000,  //  0004  LDNIL	R4
      0x20100604,  //  0005  NE	R4	R3	R4
      0x78120010,  //  0006  JMPF	R4	#0018
      0x8C100501,  //  0007  GETMET	R4	R2	K1
      0x5C180600,  //  0008  MOVE	R6	R3
      0x581C0002,  //  0009  LDCONST	R7	K2
      0x7C100600,  //  000A  CALL	R4	3
      0x78120006,  //  000B  JMPF	R4	#0013
      0x88100702,  //  000C  GETMBR	R4	R3	K2
      0x8C140901,  //  000D  GETMET	R5	R4	K1
      0x5C1C0200,  //  000E  MOVE	R7	R1
      0x7C140400,  //  000F  CALL	R5	2
      0x78160001,  //  0010  JMPF	R5	#0013
      0x50140200,  //  0011  LDBOOL	R5	1	0
      0x80040A00,  //  0012  RET	1	R5
      0x60100003,  //  0013  GETGBL	R4	G3
      0x5C140600,  //  0014  MOVE	R5	R3
      0x7C100200,  //  0015  CALL	R4	1
      0x5C0C0800,  //  0016  MOVE	R3	R4
      0x7001FFEB,  //  0017  JMP		#0004
      0x50100000,  //  0018  LDBOOL	R4	0	0
      0x80040800,  //  0019  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: setmember
********************************************************************/
be_local_closure(class_ParameterizedObject_setmember,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ParameterizedObject,     /* shared constants */
    be_str_weak(setmember),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C0C0107,  //  0000  GETMET	R3	R0	K7
      0x5C140200,  //  0001  MOVE	R5	R1
      0x7C0C0400,  //  0002  CALL	R3	2
      0x780E0004,  //  0003  JMPF	R3	#0009
      0x8C0C010D,  //  0004  GETMET	R3	R0	K13
      0x5C140200,  //  0005  MOVE	R5	R1
      0x5C180400,  //  0006  MOVE	R6	R2
      0x7C0C0600,  //  0007  CALL	R3	3
      0x70020007,  //  0008  JMP		#0011
      0x600C0018,  //  0009  GETGBL	R3	G24
      0x5810000B,  //  000A  LDCONST	R4	K11
      0x60140005,  //  000B  GETGBL	R5	G5
      0x5C180000,  //  000C  MOVE	R6	R0
      0x7C140200,  //  000D  CALL	R5	1
      0x5C180200,  //  000E  MOVE	R6	R1
      0x7C0C0600,  //  000F  CALL	R3	3
      0xB0061803,  //  0010  RAISE	1	K12	R3
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_param_value
********************************************************************/
be_local_closure(class_ParameterizedObject_get_param_value,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ParameterizedObject,     /* shared constants */
    be_str_weak(get_param_value),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C0C0108,  //  0000  GETMET	R3	R0	K8
      0x5C140200,  //  0001  MOVE	R5	R1
      0x5C180400,  //  0002  MOVE	R6	R2
      0x7C0C0600,  //  0003  CALL	R3	3
      0x80040600,  //  0004  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_ParameterizedObject_init,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ParameterizedObject,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x740A0004,  //  0002  JMPT	R2	#0008
      0x60080004,  //  0003  GETGBL	R2	G4
      0x5C0C0200,  //  0004  MOVE	R3	R1
      0x7C080200,  //  0005  CALL	R2	1
      0x2008050E,  //  0006  NE	R2	R2	K14
      0x780A0000,  //  0007  JMPF	R2	#0009
      0xB0061F10,  //  0008  RAISE	1	K15	K16
      0x90021201,  //  0009  SETMBR	R0	K9	R1
      0x60080013,  //  000A  GETGBL	R2	G19
      0x7C080000,  //  000B  CALL	R2	0
      0x90020C02,  //  000C  SETMBR	R0	K6	R2
      0x8C080111,  //  000D  GETMET	R2	R0	K17
      0x7C080200,  //  000E  CALL	R2	1
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_param
********************************************************************/
be_local_closure(class_ParameterizedObject_get_param,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ParameterizedObject,     /* shared constants */
    be_str_weak(get_param),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x880C0106,  //  0000  GETMBR	R3	R0	K6
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x780E0002,  //  0004  JMPF	R3	#0008
      0x880C0106,  //  0005  GETMBR	R3	R0	K6
      0x940C0601,  //  0006  GETIDX	R3	R3	R1
      0x80040600,  //  0007  RET	1	R3
      0x8C0C0105,  //  0008  GETMET	R3	R0	K5
      0x5C140200,  //  0009  MOVE	R5	R1
      0x7C0C0400,  //  000A  CALL	R3	2
      0x4C100000,  //  000B  LDNIL	R4
      0x20100604,  //  000C  NE	R4	R3	R4
      0x78120005,  //  000D  JMPF	R4	#0014
      0x8C100701,  //  000E  GETMET	R4	R3	K1
      0x58180012,  //  000F  LDCONST	R6	K18
      0x7C100400,  //  0010  CALL	R4	2
      0x78120001,  //  0011  JMPF	R4	#0014
      0x94100712,  //  0012  GETIDX	R4	R3	K18
      0x80040800,  //  0013  RET	1	R4
      0x80040400,  //  0014  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_param
********************************************************************/
be_local_closure(class_ParameterizedObject_set_param,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ParameterizedObject,     /* shared constants */
    be_str_weak(set_param),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x8C0C0107,  //  0000  GETMET	R3	R0	K7
      0x5C140200,  //  0001  MOVE	R5	R1
      0x7C0C0400,  //  0002  CALL	R3	2
      0x740E0001,  //  0003  JMPT	R3	#0006
      0x500C0000,  //  0004  LDBOOL	R3	0	0
      0x80040600,  //  0005  RET	1	R3
      0xA8020008,  //  0006  EXBLK	0	#0010
      0x8C0C010D,  //  0007  GETMET	R3	R0	K13
      0x5C140200,  //  0008  MOVE	R5	R1
      0x5C180400,  //  0009  MOVE	R6	R2
      0x7C0C0600,  //  000A  CALL	R3	3
      0x500C0200,  //  000B  LDBOOL	R3	1	0
      0xA8040001,  //  000C  EXBLK	1	1
      0x80040600,  //  000D  RET	1	R3
      0xA8040001,  //  000E  EXBLK	1	1
      0x70020006,  //  000F  JMP		#0017
      0x580C000F,  //  0010  LDCONST	R3	K15
      0xAC0C0201,  //  0011  CATCH	R3	1	1
      0x70020002,  //  0012  JMP		#0016
      0x50100000,  //  0013  LDBOOL	R4	0	0
      0x80040800,  //  0014  RET	1	R4
      0x70020000,  //  0015  JMP		#0017
      0xB0080000,  //  0016  RAISE	2	R0	R0
      0x80000000,  //  0017  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _validate_param
********************************************************************/
be_local_closure(class_ParameterizedObject__validate_param,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ParameterizedObject,     /* shared constants */
    be_str_weak(_validate_param),
    &be_const_str_solidified,
    ( &(const binstruction[162]) {  /* code */
      0x8C0C0105,  //  0000  GETMET	R3	R0	K5
      0x5C140200,  //  0001  MOVE	R5	R1
      0x7C0C0400,  //  0002  CALL	R3	2
      0x4C100000,  //  0003  LDNIL	R4
      0x1C100604,  //  0004  EQ	R4	R3	R4
      0x78120007,  //  0005  JMPF	R4	#000E
      0x60100018,  //  0006  GETGBL	R4	G24
      0x58140013,  //  0007  LDCONST	R5	K19
      0x5C180200,  //  0008  MOVE	R6	R1
      0x601C0005,  //  0009  GETGBL	R7	G5
      0x5C200000,  //  000A  MOVE	R8	R0
      0x7C1C0200,  //  000B  CALL	R7	1
      0x7C100600,  //  000C  CALL	R4	3
      0xB0061E04,  //  000D  RAISE	1	K15	R4
      0xB8122800,  //  000E  GETNGBL	R4	K20
      0x8C100915,  //  000F  GETMET	R4	R4	K21
      0x5C180400,  //  0010  MOVE	R6	R2
      0x7C100400,  //  0011  CALL	R4	2
      0x78120000,  //  0012  JMPF	R4	#0014
      0x80040400,  //  0013  RET	1	R2
      0x4C100000,  //  0014  LDNIL	R4
      0x1C100404,  //  0015  EQ	R4	R2	R4
      0x78120012,  //  0016  JMPF	R4	#002A
      0x8C100701,  //  0017  GETMET	R4	R3	K1
      0x58180016,  //  0018  LDCONST	R6	K22
      0x7C100400,  //  0019  CALL	R4	2
      0x78120004,  //  001A  JMPF	R4	#0020
      0x94100716,  //  001B  GETIDX	R4	R3	K22
      0x50140200,  //  001C  LDBOOL	R5	1	0
      0x1C100805,  //  001D  EQ	R4	R4	R5
      0x78120000,  //  001E  JMPF	R4	#0020
      0x80040400,  //  001F  RET	1	R2
      0x8C100701,  //  0020  GETMET	R4	R3	K1
      0x58180012,  //  0021  LDCONST	R6	K18
      0x7C100400,  //  0022  CALL	R4	2
      0x78120000,  //  0023  JMPF	R4	#0025
      0x80040400,  //  0024  RET	1	R2
      0x60100018,  //  0025  GETGBL	R4	G24
      0x58140017,  //  0026  LDCONST	R5	K23
      0x5C180200,  //  0027  MOVE	R6	R1
      0x7C100400,  //  0028  CALL	R4	2
      0xB0061E04,  //  0029  RAISE	1	K15	R4
      0x58100018,  //  002A  LDCONST	R4	K24
      0x8C140701,  //  002B  GETMET	R5	R3	K1
      0x581C0019,  //  002C  LDCONST	R7	K25
      0x7C140400,  //  002D  CALL	R5	2
      0x78160000,  //  002E  JMPF	R5	#0030
      0x94100719,  //  002F  GETIDX	R4	R3	K25
      0x60140004,  //  0030  GETGBL	R5	G4
      0x5C180400,  //  0031  MOVE	R6	R2
      0x7C140200,  //  0032  CALL	R5	1
      0x2018091A,  //  0033  NE	R6	R4	K26
      0x781A0031,  //  0034  JMPF	R6	#0067
      0x1C180918,  //  0035  EQ	R6	R4	K24
      0x781A000A,  //  0036  JMPF	R6	#0042
      0x1C180B1B,  //  0037  EQ	R6	R5	K27
      0x781A0008,  //  0038  JMPF	R6	#0042
      0xA41A3800,  //  0039  IMPORT	R6	K28
      0x601C0009,  //  003A  GETGBL	R7	G9
      0x8C200D1D,  //  003B  GETMET	R8	R6	K29
      0x5C280400,  //  003C  MOVE	R10	R2
      0x7C200400,  //  003D  CALL	R8	2
      0x7C1C0200,  //  003E  CALL	R7	1
      0x5C080E00,  //  003F  MOVE	R2	R7
      0x58140018,  //  0040  LDCONST	R5	K24
      0x70020024,  //  0041  JMP		#0067
      0x1C18091E,  //  0042  EQ	R6	R4	K30
      0x781A0018,  //  0043  JMPF	R6	#005D
      0x1C180B0E,  //  0044  EQ	R6	R5	K14
      0x781A0006,  //  0045  JMPF	R6	#004D
      0x6018000F,  //  0046  GETGBL	R6	G15
      0x5C1C0400,  //  0047  MOVE	R7	R2
      0x60200015,  //  0048  GETGBL	R8	G21
      0x7C180400,  //  0049  CALL	R6	2
      0x781A0001,  //  004A  JMPF	R6	#004D
      0x5814001E,  //  004B  LDCONST	R5	K30
      0x7002000E,  //  004C  JMP		#005C
      0x20180B0E,  //  004D  NE	R6	R5	K14
      0x741A0004,  //  004E  JMPT	R6	#0054
      0x6018000F,  //  004F  GETGBL	R6	G15
      0x5C1C0400,  //  0050  MOVE	R7	R2
      0x60200015,  //  0051  GETGBL	R8	G21
      0x7C180400,  //  0052  CALL	R6	2
      0x741A0007,  //  0053  JMPT	R6	#005C
      0x60180018,  //  0054  GETGBL	R6	G24
      0x581C001F,  //  0055  LDCONST	R7	K31
      0x5C200200,  //  0056  MOVE	R8	R1
      0x5C240800,  //  0057  MOVE	R9	R4
      0x5C280A00,  //  0058  MOVE	R10	R5
      0x5C2C0400,  //  0059  MOVE	R11	R2
      0x7C180A00,  //  005A  CALL	R6	5
      0xB0061E06,  //  005B  RAISE	1	K15	R6
      0x70020009,  //  005C  JMP		#0067
      0x20180805,  //  005D  NE	R6	R4	R5
      0x781A0007,  //  005E  JMPF	R6	#0067
      0x60180018,  //  005F  GETGBL	R6	G24
      0x581C001F,  //  0060  LDCONST	R7	K31
      0x5C200200,  //  0061  MOVE	R8	R1
      0x5C240800,  //  0062  MOVE	R9	R4
      0x5C280A00,  //  0063  MOVE	R10	R5
      0x5C2C0400,  //  0064  MOVE	R11	R2
      0x7C180A00,  //  0065  CALL	R6	5
      0xB0061E06,  //  0066  RAISE	1	K15	R6
      0x1C180B18,  //  0067  EQ	R6	R5	K24
      0x781A001B,  //  0068  JMPF	R6	#0085
      0x8C180701,  //  0069  GETMET	R6	R3	K1
      0x58200020,  //  006A  LDCONST	R8	K32
      0x7C180400,  //  006B  CALL	R6	2
      0x781A0009,  //  006C  JMPF	R6	#0077
      0x94180720,  //  006D  GETIDX	R6	R3	K32
      0x14180406,  //  006E  LT	R6	R2	R6
      0x781A0006,  //  006F  JMPF	R6	#0077
      0x60180018,  //  0070  GETGBL	R6	G24
      0x581C0021,  //  0071  LDCONST	R7	K33
      0x5C200200,  //  0072  MOVE	R8	R1
      0x5C240400,  //  0073  MOVE	R9	R2
      0x94280720,  //  0074  GETIDX	R10	R3	K32
      0x7C180800,  //  0075  CALL	R6	4
      0xB0061E06,  //  0076  RAISE	1	K15	R6
      0x8C180701,  //  0077  GETMET	R6	R3	K1
      0x58200022,  //  0078  LDCONST	R8	K34
      0x7C180400,  //  0079  CALL	R6	2
      0x781A0009,  //  007A  JMPF	R6	#0085
      0x94180722,  //  007B  GETIDX	R6	R3	K34
      0x24180406,  //  007C  GT	R6	R2	R6
      0x781A0006,  //  007D  JMPF	R6	#0085
      0x60180018,  //  007E  GETGBL	R6	G24
      0x581C0023,  //  007F  LDCONST	R7	K35
      0x5C200200,  //  0080  MOVE	R8	R1
      0x5C240400,  //  0081  MOVE	R9	R2
      0x94280722,  //  0082  GETIDX	R10	R3	K34
      0x7C180800,  //  0083  CALL	R6	4
      0xB0061E06,  //  0084  RAISE	1	K15	R6
      0x8C180701,  //  0085  GETMET	R6	R3	K1
      0x58200024,  //  0086  LDCONST	R8	K36
      0x7C180400,  //  0087  CALL	R6	2
      0x781A0017,  //  0088  JMPF	R6	#00A1
      0x50180000,  //  0089  LDBOOL	R6	0	0
      0xA41E0000,  //  008A  IMPORT	R7	K0
      0x94200724,  //  008B  GETIDX	R8	R3	K36
      0x8C241125,  //  008C  GETMET	R9	R8	K37
      0x7C240200,  //  008D  CALL	R9	1
      0x58280026,  //  008E  LDCONST	R10	K38
      0x142C1409,  //  008F  LT	R11	R10	R9
      0x782E0006,  //  0090  JMPF	R11	#0098
      0x942C100A,  //  0091  GETIDX	R11	R8	R10
      0x1C30040B,  //  0092  EQ	R12	R2	R11
      0x78320001,  //  0093  JMPF	R12	#0096
      0x50180200,  //  0094  LDBOOL	R6	1	0
      0x70020001,  //  0095  JMP		#0098
      0x00281527,  //  0096  ADD	R10	R10	K39
      0x7001FFF6,  //  0097  JMP		#008F
      0x5C2C0C00,  //  0098  MOVE	R11	R6
      0x742E0006,  //  0099  JMPT	R11	#00A1
      0x602C0018,  //  009A  GETGBL	R11	G24
      0x58300028,  //  009B  LDCONST	R12	K40
      0x5C340200,  //  009C  MOVE	R13	R1
      0x5C380400,  //  009D  MOVE	R14	R2
      0x5C3C1000,  //  009E  MOVE	R15	R8
      0x7C2C0800,  //  009F  CALL	R11	4
      0xB0061E0B,  //  00A0  RAISE	1	K15	R11
      0x80040400,  //  00A1  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _get_param_def
********************************************************************/
be_local_closure(class_ParameterizedObject__get_param_def,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ParameterizedObject,     /* shared constants */
    be_str_weak(_get_param_def),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x600C0006,  //  0001  GETGBL	R3	G6
      0x5C100000,  //  0002  MOVE	R4	R0
      0x7C0C0200,  //  0003  CALL	R3	1
      0x4C100000,  //  0004  LDNIL	R4
      0x20100604,  //  0005  NE	R4	R3	R4
      0x78120010,  //  0006  JMPF	R4	#0018
      0x8C100501,  //  0007  GETMET	R4	R2	K1
      0x5C180600,  //  0008  MOVE	R6	R3
      0x581C0002,  //  0009  LDCONST	R7	K2
      0x7C100600,  //  000A  CALL	R4	3
      0x78120006,  //  000B  JMPF	R4	#0013
      0x88100702,  //  000C  GETMBR	R4	R3	K2
      0x8C140901,  //  000D  GETMET	R5	R4	K1
      0x5C1C0200,  //  000E  MOVE	R7	R1
      0x7C140400,  //  000F  CALL	R5	2
      0x78160001,  //  0010  JMPF	R5	#0013
      0x94140801,  //  0011  GETIDX	R5	R4	R1
      0x80040A00,  //  0012  RET	1	R5
      0x60100003,  //  0013  GETGBL	R4	G3
      0x5C140600,  //  0014  MOVE	R5	R3
      0x7C100200,  //  0015  CALL	R4	1
      0x5C0C0800,  //  0016  MOVE	R3	R4
      0x7001FFEB,  //  0017  JMP		#0004
      0x4C100000,  //  0018  LDNIL	R4
      0x80040800,  //  0019  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _init_parameter_values
********************************************************************/
be_local_closure(class_ParameterizedObject__init_parameter_values,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ParameterizedObject,     /* shared constants */
    be_str_weak(_init_parameter_values),
    &be_const_str_solidified,
    ( &(const binstruction[43]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x60080006,  //  0001  GETGBL	R2	G6
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C080200,  //  0003  CALL	R2	1
      0x4C0C0000,  //  0004  LDNIL	R3
      0x200C0403,  //  0005  NE	R3	R2	R3
      0x780E0022,  //  0006  JMPF	R3	#002A
      0x8C0C0301,  //  0007  GETMET	R3	R1	K1
      0x5C140400,  //  0008  MOVE	R5	R2
      0x58180002,  //  0009  LDCONST	R6	K2
      0x7C0C0600,  //  000A  CALL	R3	3
      0x780E0018,  //  000B  JMPF	R3	#0025
      0x880C0502,  //  000C  GETMBR	R3	R2	K2
      0x60100010,  //  000D  GETGBL	R4	G16
      0x8C140703,  //  000E  GETMET	R5	R3	K3
      0x7C140200,  //  000F  CALL	R5	1
      0x7C100200,  //  0010  CALL	R4	1
      0xA802000F,  //  0011  EXBLK	0	#0022
      0x5C140800,  //  0012  MOVE	R5	R4
      0x7C140000,  //  0013  CALL	R5	0
      0x88180106,  //  0014  GETMBR	R6	R0	K6
      0x8C180D01,  //  0015  GETMET	R6	R6	K1
      0x5C200A00,  //  0016  MOVE	R8	R5
      0x7C180400,  //  0017  CALL	R6	2
      0x741A0007,  //  0018  JMPT	R6	#0021
      0x94180605,  //  0019  GETIDX	R6	R3	R5
      0x8C1C0D01,  //  001A  GETMET	R7	R6	K1
      0x58240012,  //  001B  LDCONST	R9	K18
      0x7C1C0400,  //  001C  CALL	R7	2
      0x781E0002,  //  001D  JMPF	R7	#0021
      0x881C0106,  //  001E  GETMBR	R7	R0	K6
      0x94200D12,  //  001F  GETIDX	R8	R6	K18
      0x981C0A08,  //  0020  SETIDX	R7	R5	R8
      0x7001FFEF,  //  0021  JMP		#0012
      0x58100004,  //  0022  LDCONST	R4	K4
      0xAC100200,  //  0023  CATCH	R4	1	0
      0xB0080000,  //  0024  RAISE	2	R0	R0
      0x600C0003,  //  0025  GETGBL	R3	G3
      0x5C100400,  //  0026  MOVE	R4	R2
      0x7C0C0200,  //  0027  CALL	R3	1
      0x5C080600,  //  0028  MOVE	R2	R3
      0x7001FFD9,  //  0029  JMP		#0004
      0x80000000,  //  002A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: ==
********************************************************************/
be_local_closure(class_ParameterizedObject__X3D_X3D,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ParameterizedObject,     /* shared constants */
    be_str_weak(_X3D_X3D),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0529,  //  0001  GETMET	R3	R2	K41
      0x5C140000,  //  0002  MOVE	R5	R0
      0x7C0C0400,  //  0003  CALL	R3	2
      0x8C100529,  //  0004  GETMET	R4	R2	K41
      0x5C180200,  //  0005  MOVE	R6	R1
      0x7C100400,  //  0006  CALL	R4	2
      0x1C0C0604,  //  0007  EQ	R3	R3	R4
      0x80040600,  //  0008  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _resolve_parameter_value
********************************************************************/
be_local_closure(class_ParameterizedObject__resolve_parameter_value,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ParameterizedObject,     /* shared constants */
    be_str_weak(_resolve_parameter_value),
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
      0x880C0106,  //  0000  GETMBR	R3	R0	K6
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x740E000D,  //  0004  JMPT	R3	#0013
      0x8C0C0105,  //  0005  GETMET	R3	R0	K5
      0x5C140200,  //  0006  MOVE	R5	R1
      0x7C0C0400,  //  0007  CALL	R3	2
      0x4C100000,  //  0008  LDNIL	R4
      0x20100604,  //  0009  NE	R4	R3	R4
      0x78120005,  //  000A  JMPF	R4	#0011
      0x8C100701,  //  000B  GETMET	R4	R3	K1
      0x58180012,  //  000C  LDCONST	R6	K18
      0x7C100400,  //  000D  CALL	R4	2
      0x78120001,  //  000E  JMPF	R4	#0011
      0x94100712,  //  000F  GETIDX	R4	R3	K18
      0x80040800,  //  0010  RET	1	R4
      0x4C100000,  //  0011  LDNIL	R4
      0x80040800,  //  0012  RET	1	R4
      0x880C0106,  //  0013  GETMBR	R3	R0	K6
      0x940C0601,  //  0014  GETIDX	R3	R3	R1
      0xB8122800,  //  0015  GETNGBL	R4	K20
      0x8C100915,  //  0016  GETMET	R4	R4	K21
      0x5C180600,  //  0017  MOVE	R6	R3
      0x7C100400,  //  0018  CALL	R4	2
      0x78120005,  //  0019  JMPF	R4	#0020
      0x8C10072A,  //  001A  GETMET	R4	R3	K42
      0x5C180200,  //  001B  MOVE	R6	R1
      0x5C1C0400,  //  001C  MOVE	R7	R2
      0x7C100600,  //  001D  CALL	R4	3
      0x80040800,  //  001E  RET	1	R4
      0x70020000,  //  001F  JMP		#0021
      0x80040600,  //  0020  RET	1	R3
      0x80000000,  //  0021  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: !=
********************************************************************/
be_local_closure(class_ParameterizedObject__X21_X3D,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ParameterizedObject,     /* shared constants */
    be_str_weak(_X21_X3D),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x1C080001,  //  0000  EQ	R2	R0	R1
      0x780A0000,  //  0001  JMPF	R2	#0003
      0x50080001,  //  0002  LDBOOL	R2	0	1
      0x50080200,  //  0003  LDBOOL	R2	1	0
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _set_parameter_value
********************************************************************/
be_local_closure(class_ParameterizedObject__set_parameter_value,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ParameterizedObject,     /* shared constants */
    be_str_weak(_set_parameter_value),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0xB80E2800,  //  0000  GETNGBL	R3	K20
      0x8C0C0715,  //  0001  GETMET	R3	R3	K21
      0x5C140400,  //  0002  MOVE	R5	R2
      0x7C0C0400,  //  0003  CALL	R3	2
      0x740E0004,  //  0004  JMPT	R3	#000A
      0x8C0C012B,  //  0005  GETMET	R3	R0	K43
      0x5C140200,  //  0006  MOVE	R5	R1
      0x5C180400,  //  0007  MOVE	R6	R2
      0x7C0C0600,  //  0008  CALL	R3	3
      0x5C080600,  //  0009  MOVE	R2	R3
      0x880C0106,  //  000A  GETMBR	R3	R0	K6
      0x980C0202,  //  000B  SETIDX	R3	R1	R2
      0x8C0C012C,  //  000C  GETMET	R3	R0	K44
      0x5C140200,  //  000D  MOVE	R5	R1
      0x5C180400,  //  000E  MOVE	R6	R2
      0x7C0C0600,  //  000F  CALL	R3	3
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: resolve_value
********************************************************************/
be_local_closure(class_ParameterizedObject_resolve_value,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ParameterizedObject,     /* shared constants */
    be_str_weak(resolve_value),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x4C100000,  //  0000  LDNIL	R4
      0x1C100204,  //  0001  EQ	R4	R1	R4
      0x78120001,  //  0002  JMPF	R4	#0005
      0x4C100000,  //  0003  LDNIL	R4
      0x80040800,  //  0004  RET	1	R4
      0xB8122800,  //  0005  GETNGBL	R4	K20
      0x8C100915,  //  0006  GETMET	R4	R4	K21
      0x5C180200,  //  0007  MOVE	R6	R1
      0x7C100400,  //  0008  CALL	R4	2
      0x78120005,  //  0009  JMPF	R4	#0010
      0x8C10032A,  //  000A  GETMET	R4	R1	K42
      0x5C180400,  //  000B  MOVE	R6	R2
      0x5C1C0600,  //  000C  MOVE	R7	R3
      0x7C100600,  //  000D  CALL	R4	3
      0x80040800,  //  000E  RET	1	R4
      0x70020000,  //  000F  JMP		#0011
      0x80040200,  //  0010  RET	1	R1
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: ParameterizedObject
********************************************************************/
be_local_class(ParameterizedObject,
    2,
    NULL,
    be_nested_map(22,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(resolve_value, 13), be_const_closure(class_ParameterizedObject_resolve_value_closure) },
        { be_const_key_weak(on_param_changed, -1), be_const_closure(class_ParameterizedObject_on_param_changed_closure) },
        { be_const_key_weak(get_params_metadata, -1), be_const_closure(class_ParameterizedObject_get_params_metadata_closure) },
        { be_const_key_weak(PARAMS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(0,
    ( (struct bmapnode*) &(const bmapnode[]) {
    }))    ) } )) },
        { be_const_key_weak(init, -1), be_const_closure(class_ParameterizedObject_init_closure) },
        { be_const_key_weak(start, 18), be_const_closure(class_ParameterizedObject_start_closure) },
        { be_const_key_weak(_has_param, -1), be_const_closure(class_ParameterizedObject__has_param_closure) },
        { be_const_key_weak(setmember, -1), be_const_closure(class_ParameterizedObject_setmember_closure) },
        { be_const_key_weak(engine, -1), be_const_var(1) },
        { be_const_key_weak(values, 4), be_const_var(0) },
        { be_const_key_weak(set_param, -1), be_const_closure(class_ParameterizedObject_set_param_closure) },
        { be_const_key_weak(get_param_metadata, 10), be_const_closure(class_ParameterizedObject_get_param_metadata_closure) },
        { be_const_key_weak(_validate_param, 15), be_const_closure(class_ParameterizedObject__validate_param_closure) },
        { be_const_key_weak(_X3D_X3D, 14), be_const_closure(class_ParameterizedObject__X3D_X3D_closure) },
        { be_const_key_weak(_init_parameter_values, -1), be_const_closure(class_ParameterizedObject__init_parameter_values_closure) },
        { be_const_key_weak(_get_param_def, -1), be_const_closure(class_ParameterizedObject__get_param_def_closure) },
        { be_const_key_weak(_resolve_parameter_value, -1), be_const_closure(class_ParameterizedObject__resolve_parameter_value_closure) },
        { be_const_key_weak(member, 8), be_const_closure(class_ParameterizedObject_member_closure) },
        { be_const_key_weak(get_param, 1), be_const_closure(class_ParameterizedObject_get_param_closure) },
        { be_const_key_weak(_X21_X3D, 3), be_const_closure(class_ParameterizedObject__X21_X3D_closure) },
        { be_const_key_weak(_set_parameter_value, -1), be_const_closure(class_ParameterizedObject__set_parameter_value_closure) },
        { be_const_key_weak(get_param_value, 0), be_const_closure(class_ParameterizedObject_get_param_value_closure) },
    })),
    be_str_weak(ParameterizedObject)
);

/********************************************************************
** Solidified function: create_closure_value
********************************************************************/
be_local_closure(create_closure_value,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(closure_value),
    /* K2   */  be_nested_str_weak(closure),
    }),
    be_str_weak(create_closure_value),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100000,  //  0002  MOVE	R4	R0
      0x7C080400,  //  0003  CALL	R2	2
      0x900A0401,  //  0004  SETMBR	R2	K2	R1
      0x80040400,  //  0005  RET	1	R2
    })
  )
);
/*******************************************************************/

// compact class 'TwinkleAnimation' ktab size: 37, total: 67 (saved 240 bytes)
static const bvalue be_ktab_class_TwinkleAnimation[37] = {
  /* K0   */  be_nested_str_weak(twinkle_speed),
  /* K1   */  be_const_int(1),
  /* K2   */  be_nested_str_weak(set_param),
  /* K3   */  be_nested_str_weak(fade_speed),
  /* K4   */  be_nested_str_weak(density),
  /* K5   */  be_nested_str_weak(min_brightness),
  /* K6   */  be_nested_str_weak(max_brightness),
  /* K7   */  be_nested_str_weak(color),
  /* K8   */  be_nested_str_weak(engine),
  /* K9   */  be_nested_str_weak(get_strip_length),
  /* K10  */  be_nested_str_weak(twinkle_states),
  /* K11  */  be_nested_str_weak(_initialize_arrays),
  /* K12  */  be_const_int(0),
  /* K13  */  be_nested_str_weak(current_colors),
  /* K14  */  be_nested_str_weak(tasmota),
  /* K15  */  be_nested_str_weak(scale_uint),
  /* K16  */  be_const_int(16777215),
  /* K17  */  be_nested_str_weak(_random_range),
  /* K18  */  be_nested_str_weak(get_param),
  /* K19  */  be_nested_str_weak(animation),
  /* K20  */  be_nested_str_weak(is_value_provider),
  /* K21  */  be_nested_str_weak(0x_X2508x),
  /* K22  */  be_nested_str_weak(TwinkleAnimation_X28color_X3D_X25s_X2C_X20density_X3D_X25s_X2C_X20twinkle_speed_X3D_X25s_X2C_X20priority_X3D_X25s_X2C_X20running_X3D_X25s_X29),
  /* K23  */  be_nested_str_weak(priority),
  /* K24  */  be_nested_str_weak(is_running),
  /* K25  */  be_nested_str_weak(init),
  /* K26  */  be_nested_str_weak(last_update),
  /* K27  */  be_nested_str_weak(random_seed),
  /* K28  */  be_nested_str_weak(time_ms),
  /* K29  */  be_nested_str_weak(width),
  /* K30  */  be_nested_str_weak(set_pixel_color),
  /* K31  */  be_nested_str_weak(update),
  /* K32  */  be_nested_str_weak(_update_twinkle_simulation),
  /* K33  */  be_nested_str_weak(resize),
  /* K34  */  be_const_int(1103515245),
  /* K35  */  be_const_int(2147483647),
  /* K36  */  be_nested_str_weak(_random),
};


extern const bclass be_class_TwinkleAnimation;

/********************************************************************
** Solidified function: on_param_changed
********************************************************************/
be_local_closure(class_TwinkleAnimation_on_param_changed,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_TwinkleAnimation,     /* shared constants */
    be_str_weak(on_param_changed),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x1C0C0300,  //  0000  EQ	R3	R1	K0
      0x780E0010,  //  0001  JMPF	R3	#0013
      0x540E0031,  //  0002  LDINT	R3	50
      0x280C0403,  //  0003  GE	R3	R2	R3
      0x780E000D,  //  0004  JMPF	R3	#0013
      0x540E03E7,  //  0005  LDINT	R3	1000
      0x0C0C0602,  //  0006  DIV	R3	R3	R2
      0x14100701,  //  0007  LT	R4	R3	K1
      0x78120001,  //  0008  JMPF	R4	#000B
      0x580C0001,  //  0009  LDCONST	R3	K1
      0x70020003,  //  000A  JMP		#000F
      0x54120013,  //  000B  LDINT	R4	20
      0x24100604,  //  000C  GT	R4	R3	R4
      0x78120000,  //  000D  JMPF	R4	#000F
      0x540E0013,  //  000E  LDINT	R3	20
      0x8C100102,  //  000F  GETMET	R4	R0	K2
      0x58180000,  //  0010  LDCONST	R6	K0
      0x5C1C0600,  //  0011  MOVE	R7	R3
      0x7C100600,  //  0012  CALL	R4	3
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _update_twinkle_simulation
********************************************************************/
be_local_closure(class_TwinkleAnimation__update_twinkle_simulation,   /* name */
  be_nested_proto(
    18,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_TwinkleAnimation,     /* shared constants */
    be_str_weak(_update_twinkle_simulation),
    &be_const_str_solidified,
    ( &(const binstruction[94]) {  /* code */
      0x88080103,  //  0000  GETMBR	R2	R0	K3
      0x880C0104,  //  0001  GETMBR	R3	R0	K4
      0x88100105,  //  0002  GETMBR	R4	R0	K5
      0x88140106,  //  0003  GETMBR	R5	R0	K6
      0x88180107,  //  0004  GETMBR	R6	R0	K7
      0x881C0108,  //  0005  GETMBR	R7	R0	K8
      0x8C1C0F09,  //  0006  GETMET	R7	R7	K9
      0x7C1C0200,  //  0007  CALL	R7	1
      0x6020000C,  //  0008  GETGBL	R8	G12
      0x8824010A,  //  0009  GETMBR	R9	R0	K10
      0x7C200200,  //  000A  CALL	R8	1
      0x20201007,  //  000B  NE	R8	R8	R7
      0x78220001,  //  000C  JMPF	R8	#000F
      0x8C20010B,  //  000D  GETMET	R8	R0	K11
      0x7C200200,  //  000E  CALL	R8	1
      0x5820000C,  //  000F  LDCONST	R8	K12
      0x14241007,  //  0010  LT	R9	R8	R7
      0x7826001F,  //  0011  JMPF	R9	#0032
      0x8824010D,  //  0012  GETMBR	R9	R0	K13
      0x94241208,  //  0013  GETIDX	R9	R9	R8
      0x542A0017,  //  0014  LDINT	R10	24
      0x3C28120A,  //  0015  SHR	R10	R9	R10
      0x542E00FE,  //  0016  LDINT	R11	255
      0x2C28140B,  //  0017  AND	R10	R10	R11
      0x242C150C,  //  0018  GT	R11	R10	K12
      0x782E0015,  //  0019  JMPF	R11	#0030
      0xB82E1C00,  //  001A  GETNGBL	R11	K14
      0x8C2C170F,  //  001B  GETMET	R11	R11	K15
      0x5C340400,  //  001C  MOVE	R13	R2
      0x5838000C,  //  001D  LDCONST	R14	K12
      0x543E00FE,  //  001E  LDINT	R15	255
      0x58400001,  //  001F  LDCONST	R16	K1
      0x54460013,  //  0020  LDINT	R17	20
      0x7C2C0C00,  //  0021  CALL	R11	6
      0x1830140B,  //  0022  LE	R12	R10	R11
      0x78320004,  //  0023  JMPF	R12	#0029
      0x8830010A,  //  0024  GETMBR	R12	R0	K10
      0x9830110C,  //  0025  SETIDX	R12	R8	K12
      0x8830010D,  //  0026  GETMBR	R12	R0	K13
      0x9830110C,  //  0027  SETIDX	R12	R8	K12
      0x70020006,  //  0028  JMP		#0030
      0x0430140B,  //  0029  SUB	R12	R10	R11
      0x2C341310,  //  002A  AND	R13	R9	K16
      0x8838010D,  //  002B  GETMBR	R14	R0	K13
      0x543E0017,  //  002C  LDINT	R15	24
      0x383C180F,  //  002D  SHL	R15	R12	R15
      0x303C1E0D,  //  002E  OR	R15	R15	R13
      0x9838100F,  //  002F  SETIDX	R14	R8	R15
      0x00201101,  //  0030  ADD	R8	R8	K1
      0x7001FFDD,  //  0031  JMP		#0010
      0x5824000C,  //  0032  LDCONST	R9	K12
      0x14281207,  //  0033  LT	R10	R9	R7
      0x782A0027,  //  0034  JMPF	R10	#005D
      0x8828010A,  //  0035  GETMBR	R10	R0	K10
      0x94281409,  //  0036  GETIDX	R10	R10	R9
      0x1C28150C,  //  0037  EQ	R10	R10	K12
      0x782A0021,  //  0038  JMPF	R10	#005B
      0x8C280111,  //  0039  GETMET	R10	R0	K17
      0x543200FE,  //  003A  LDINT	R12	255
      0x7C280400,  //  003B  CALL	R10	2
      0x14281403,  //  003C  LT	R10	R10	R3
      0x782A001C,  //  003D  JMPF	R10	#005B
      0x8C280111,  //  003E  GETMET	R10	R0	K17
      0x04300A04,  //  003F  SUB	R12	R5	R4
      0x00301901,  //  0040  ADD	R12	R12	K1
      0x7C280400,  //  0041  CALL	R10	2
      0x0028080A,  //  0042  ADD	R10	R4	R10
      0x5C2C0C00,  //  0043  MOVE	R11	R6
      0x5432000F,  //  0044  LDINT	R12	16
      0x3C30160C,  //  0045  SHR	R12	R11	R12
      0x543600FE,  //  0046  LDINT	R13	255
      0x2C30180D,  //  0047  AND	R12	R12	R13
      0x54360007,  //  0048  LDINT	R13	8
      0x3C34160D,  //  0049  SHR	R13	R11	R13
      0x543A00FE,  //  004A  LDINT	R14	255
      0x2C341A0E,  //  004B  AND	R13	R13	R14
      0x543A00FE,  //  004C  LDINT	R14	255
      0x2C38160E,  //  004D  AND	R14	R11	R14
      0x883C010A,  //  004E  GETMBR	R15	R0	K10
      0x983C1301,  //  004F  SETIDX	R15	R9	K1
      0x883C010D,  //  0050  GETMBR	R15	R0	K13
      0x54420017,  //  0051  LDINT	R16	24
      0x38401410,  //  0052  SHL	R16	R10	R16
      0x5446000F,  //  0053  LDINT	R17	16
      0x38441811,  //  0054  SHL	R17	R12	R17
      0x30402011,  //  0055  OR	R16	R16	R17
      0x54460007,  //  0056  LDINT	R17	8
      0x38441A11,  //  0057  SHL	R17	R13	R17
      0x30402011,  //  0058  OR	R16	R16	R17
      0x3040200E,  //  0059  OR	R16	R16	R14
      0x983C1210,  //  005A  SETIDX	R15	R9	R16
      0x00241301,  //  005B  ADD	R9	R9	K1
      0x7001FFD5,  //  005C  JMP		#0033
      0x80000000,  //  005D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_TwinkleAnimation_tostring,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_TwinkleAnimation,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x4C040000,  //  0000  LDNIL	R1
      0x8C080112,  //  0001  GETMET	R2	R0	K18
      0x58100007,  //  0002  LDCONST	R4	K7
      0x7C080400,  //  0003  CALL	R2	2
      0xB80E2600,  //  0004  GETNGBL	R3	K19
      0x8C0C0714,  //  0005  GETMET	R3	R3	K20
      0x5C140400,  //  0006  MOVE	R5	R2
      0x7C0C0400,  //  0007  CALL	R3	2
      0x780E0004,  //  0008  JMPF	R3	#000E
      0x600C0008,  //  0009  GETGBL	R3	G8
      0x5C100400,  //  000A  MOVE	R4	R2
      0x7C0C0200,  //  000B  CALL	R3	1
      0x5C040600,  //  000C  MOVE	R1	R3
      0x70020004,  //  000D  JMP		#0013
      0x600C0018,  //  000E  GETGBL	R3	G24
      0x58100015,  //  000F  LDCONST	R4	K21
      0x88140107,  //  0010  GETMBR	R5	R0	K7
      0x7C0C0400,  //  0011  CALL	R3	2
      0x5C040600,  //  0012  MOVE	R1	R3
      0x600C0018,  //  0013  GETGBL	R3	G24
      0x58100016,  //  0014  LDCONST	R4	K22
      0x5C140200,  //  0015  MOVE	R5	R1
      0x88180104,  //  0016  GETMBR	R6	R0	K4
      0x881C0100,  //  0017  GETMBR	R7	R0	K0
      0x88200117,  //  0018  GETMBR	R8	R0	K23
      0x88240118,  //  0019  GETMBR	R9	R0	K24
      0x7C0C0C00,  //  001A  CALL	R3	6
      0x80040600,  //  001B  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_TwinkleAnimation_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_TwinkleAnimation,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080519,  //  0003  GETMET	R2	R2	K25
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x60080012,  //  0006  GETGBL	R2	G18
      0x7C080000,  //  0007  CALL	R2	0
      0x90021402,  //  0008  SETMBR	R0	K10	R2
      0x60080012,  //  0009  GETGBL	R2	G18
      0x7C080000,  //  000A  CALL	R2	0
      0x90021A02,  //  000B  SETMBR	R0	K13	R2
      0x9002350C,  //  000C  SETMBR	R0	K26	K12
      0x88080108,  //  000D  GETMBR	R2	R0	K8
      0x8808051C,  //  000E  GETMBR	R2	R2	K28
      0x540EFFFF,  //  000F  LDINT	R3	65536
      0x10080403,  //  0010  MOD	R2	R2	R3
      0x90023602,  //  0011  SETMBR	R0	K27	R2
      0x8C08010B,  //  0012  GETMET	R2	R0	K11
      0x7C080200,  //  0013  CALL	R2	1
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: render
********************************************************************/
be_local_closure(class_TwinkleAnimation_render,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_TwinkleAnimation,     /* shared constants */
    be_str_weak(render),
    &be_const_str_solidified,
    ( &(const binstruction[45]) {  /* code */
      0x880C0118,  //  0000  GETMBR	R3	R0	K24
      0x780E0002,  //  0001  JMPF	R3	#0005
      0x4C0C0000,  //  0002  LDNIL	R3
      0x1C0C0203,  //  0003  EQ	R3	R1	R3
      0x780E0001,  //  0004  JMPF	R3	#0007
      0x500C0000,  //  0005  LDBOOL	R3	0	0
      0x80040600,  //  0006  RET	1	R3
      0x4C0C0000,  //  0007  LDNIL	R3
      0x1C0C0403,  //  0008  EQ	R3	R2	R3
      0x780E0001,  //  0009  JMPF	R3	#000C
      0x880C0108,  //  000A  GETMBR	R3	R0	K8
      0x8808071C,  //  000B  GETMBR	R2	R3	K28
      0x880C0108,  //  000C  GETMBR	R3	R0	K8
      0x8C0C0709,  //  000D  GETMET	R3	R3	K9
      0x7C0C0200,  //  000E  CALL	R3	1
      0x6010000C,  //  000F  GETGBL	R4	G12
      0x8814010A,  //  0010  GETMBR	R5	R0	K10
      0x7C100200,  //  0011  CALL	R4	1
      0x20100803,  //  0012  NE	R4	R4	R3
      0x78120001,  //  0013  JMPF	R4	#0016
      0x8C10010B,  //  0014  GETMET	R4	R0	K11
      0x7C100200,  //  0015  CALL	R4	1
      0x50100000,  //  0016  LDBOOL	R4	0	0
      0x5814000C,  //  0017  LDCONST	R5	K12
      0x14180A03,  //  0018  LT	R6	R5	R3
      0x781A0011,  //  0019  JMPF	R6	#002C
      0x8818031D,  //  001A  GETMBR	R6	R1	K29
      0x14180A06,  //  001B  LT	R6	R5	R6
      0x781A000C,  //  001C  JMPF	R6	#002A
      0x8818010D,  //  001D  GETMBR	R6	R0	K13
      0x94180C05,  //  001E  GETIDX	R6	R6	R5
      0x541E0017,  //  001F  LDINT	R7	24
      0x3C1C0C07,  //  0020  SHR	R7	R6	R7
      0x542200FE,  //  0021  LDINT	R8	255
      0x2C1C0E08,  //  0022  AND	R7	R7	R8
      0x241C0F0C,  //  0023  GT	R7	R7	K12
      0x781E0004,  //  0024  JMPF	R7	#002A
      0x8C1C031E,  //  0025  GETMET	R7	R1	K30
      0x5C240A00,  //  0026  MOVE	R9	R5
      0x5C280C00,  //  0027  MOVE	R10	R6
      0x7C1C0600,  //  0028  CALL	R7	3
      0x50100200,  //  0029  LDBOOL	R4	1	0
      0x00140B01,  //  002A  ADD	R5	R5	K1
      0x7001FFEB,  //  002B  JMP		#0018
      0x80040800,  //  002C  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update
********************************************************************/
be_local_closure(class_TwinkleAnimation_update,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_TwinkleAnimation,     /* shared constants */
    be_str_weak(update),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C08051F,  //  0003  GETMET	R2	R2	K31
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x740A0001,  //  0006  JMPT	R2	#0009
      0x50080000,  //  0007  LDBOOL	R2	0	0
      0x80040400,  //  0008  RET	1	R2
      0x4C080000,  //  0009  LDNIL	R2
      0x1C080202,  //  000A  EQ	R2	R1	R2
      0x780A0001,  //  000B  JMPF	R2	#000E
      0x88080108,  //  000C  GETMBR	R2	R0	K8
      0x8804051C,  //  000D  GETMBR	R1	R2	K28
      0x88080100,  //  000E  GETMBR	R2	R0	K0
      0x540E03E7,  //  000F  LDINT	R3	1000
      0x0C0C0602,  //  0010  DIV	R3	R3	R2
      0x8810011A,  //  0011  GETMBR	R4	R0	K26
      0x04100204,  //  0012  SUB	R4	R1	R4
      0x28100803,  //  0013  GE	R4	R4	R3
      0x78120003,  //  0014  JMPF	R4	#0019
      0x90023401,  //  0015  SETMBR	R0	K26	R1
      0x8C100120,  //  0016  GETMET	R4	R0	K32
      0x5C180200,  //  0017  MOVE	R6	R1
      0x7C100400,  //  0018  CALL	R4	2
      0x50100200,  //  0019  LDBOOL	R4	1	0
      0x80040800,  //  001A  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _initialize_arrays
********************************************************************/
be_local_closure(class_TwinkleAnimation__initialize_arrays,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_TwinkleAnimation,     /* shared constants */
    be_str_weak(_initialize_arrays),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x88040108,  //  0000  GETMBR	R1	R0	K8
      0x8C040309,  //  0001  GETMET	R1	R1	K9
      0x7C040200,  //  0002  CALL	R1	1
      0x8808010A,  //  0003  GETMBR	R2	R0	K10
      0x8C080521,  //  0004  GETMET	R2	R2	K33
      0x5C100200,  //  0005  MOVE	R4	R1
      0x7C080400,  //  0006  CALL	R2	2
      0x8808010D,  //  0007  GETMBR	R2	R0	K13
      0x8C080521,  //  0008  GETMET	R2	R2	K33
      0x5C100200,  //  0009  MOVE	R4	R1
      0x7C080400,  //  000A  CALL	R2	2
      0x5808000C,  //  000B  LDCONST	R2	K12
      0x140C0401,  //  000C  LT	R3	R2	R1
      0x780E0005,  //  000D  JMPF	R3	#0014
      0x880C010A,  //  000E  GETMBR	R3	R0	K10
      0x980C050C,  //  000F  SETIDX	R3	R2	K12
      0x880C010D,  //  0010  GETMBR	R3	R0	K13
      0x980C050C,  //  0011  SETIDX	R3	R2	K12
      0x00080501,  //  0012  ADD	R2	R2	K1
      0x7001FFF7,  //  0013  JMP		#000C
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _random
********************************************************************/
be_local_closure(class_TwinkleAnimation__random,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_TwinkleAnimation,     /* shared constants */
    be_str_weak(_random),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x8804011B,  //  0000  GETMBR	R1	R0	K27
      0x08040322,  //  0001  MUL	R1	R1	K34
      0x540A3038,  //  0002  LDINT	R2	12345
      0x00040202,  //  0003  ADD	R1	R1	R2
      0x2C040323,  //  0004  AND	R1	R1	K35
      0x90023601,  //  0005  SETMBR	R0	K27	R1
      0x8804011B,  //  0006  GETMBR	R1	R0	K27
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _random_range
********************************************************************/
be_local_closure(class_TwinkleAnimation__random_range,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_TwinkleAnimation,     /* shared constants */
    be_str_weak(_random_range),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x1808030C,  //  0000  LE	R2	R1	K12
      0x780A0000,  //  0001  JMPF	R2	#0003
      0x80061800,  //  0002  RET	1	K12
      0x8C080124,  //  0003  GETMET	R2	R0	K36
      0x7C080200,  //  0004  CALL	R2	1
      0x10080401,  //  0005  MOD	R2	R2	R1
      0x80040400,  //  0006  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: TwinkleAnimation
********************************************************************/
extern const bclass be_class_Animation;
be_local_class(TwinkleAnimation,
    4,
    &be_class_Animation,
    be_nested_map(14,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(_random_range, -1), be_const_closure(class_TwinkleAnimation__random_range_closure) },
        { be_const_key_weak(twinkle_states, -1), be_const_var(0) },
        { be_const_key_weak(init, -1), be_const_closure(class_TwinkleAnimation_init_closure) },
        { be_const_key_weak(tostring, -1), be_const_closure(class_TwinkleAnimation_tostring_closure) },
        { be_const_key_weak(random_seed, -1), be_const_var(3) },
        { be_const_key_weak(PARAMS, 6), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(fade_speed, 1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(180) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(twinkle_speed, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(6) },
        { be_const_key_weak(min, -1), be_const_int(1) },
        { be_const_key_weak(max, 1), be_const_int(5000) },
    }))    ) } )) },
        { be_const_key_weak(density, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(128) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(max_brightness, 2), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(255) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(color, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(-1) },
    }))    ) } )) },
        { be_const_key_weak(min_brightness, 0), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(32) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(_random, 9), be_const_closure(class_TwinkleAnimation__random_closure) },
        { be_const_key_weak(last_update, -1), be_const_var(2) },
        { be_const_key_weak(update, -1), be_const_closure(class_TwinkleAnimation_update_closure) },
        { be_const_key_weak(_initialize_arrays, 10), be_const_closure(class_TwinkleAnimation__initialize_arrays_closure) },
        { be_const_key_weak(render, 2), be_const_closure(class_TwinkleAnimation_render_closure) },
        { be_const_key_weak(current_colors, -1), be_const_var(1) },
        { be_const_key_weak(_update_twinkle_simulation, 1), be_const_closure(class_TwinkleAnimation__update_twinkle_simulation_closure) },
        { be_const_key_weak(on_param_changed, 0), be_const_closure(class_TwinkleAnimation_on_param_changed_closure) },
    })),
    be_str_weak(TwinkleAnimation)
);

/********************************************************************
** Solidified function: twinkle_classic
********************************************************************/
be_local_closure(twinkle_classic,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(twinkle_animation),
    /* K2   */  be_nested_str_weak(color),
    /* K3   */  be_nested_str_weak(density),
    /* K4   */  be_nested_str_weak(twinkle_speed),
    /* K5   */  be_nested_str_weak(fade_speed),
    /* K6   */  be_nested_str_weak(min_brightness),
    /* K7   */  be_nested_str_weak(max_brightness),
    }),
    be_str_weak(twinkle_classic),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x5409FFFE,  //  0004  LDINT	R2	-1
      0x90060402,  //  0005  SETMBR	R1	K2	R2
      0x540A0095,  //  0006  LDINT	R2	150
      0x90060602,  //  0007  SETMBR	R1	K3	R2
      0x540A0005,  //  0008  LDINT	R2	6
      0x90060802,  //  0009  SETMBR	R1	K4	R2
      0x540A00B3,  //  000A  LDINT	R2	180
      0x90060A02,  //  000B  SETMBR	R1	K5	R2
      0x540A001F,  //  000C  LDINT	R2	32
      0x90060C02,  //  000D  SETMBR	R1	K6	R2
      0x540A00FE,  //  000E  LDINT	R2	255
      0x90060E02,  //  000F  SETMBR	R1	K7	R2
      0x80040200,  //  0010  RET	1	R1
    })
  )
);
/*******************************************************************/

// compact class 'BeaconAnimation' ktab size: 17, total: 21 (saved 32 bytes)
static const bvalue be_ktab_class_BeaconAnimation[17] = {
  /* K0   */  be_nested_str_weak(engine),
  /* K1   */  be_nested_str_weak(time_ms),
  /* K2   */  be_nested_str_weak(width),
  /* K3   */  be_nested_str_weak(back_color),
  /* K4   */  be_nested_str_weak(pos),
  /* K5   */  be_nested_str_weak(slew_size),
  /* K6   */  be_nested_str_weak(beacon_size),
  /* K7   */  be_nested_str_weak(color),
  /* K8   */  be_const_int(-16777216),
  /* K9   */  be_nested_str_weak(fill_pixels),
  /* K10  */  be_const_int(0),
  /* K11  */  be_nested_str_weak(set_pixel_color),
  /* K12  */  be_const_int(1),
  /* K13  */  be_nested_str_weak(tasmota),
  /* K14  */  be_nested_str_weak(scale_int),
  /* K15  */  be_nested_str_weak(blend_linear),
  /* K16  */  be_nested_str_weak(BeaconAnimation_X28color_X3D0x_X2508x_X2C_X20pos_X3D_X25s_X2C_X20beacon_size_X3D_X25s_X2C_X20slew_size_X3D_X25s_X29),
};


extern const bclass be_class_BeaconAnimation;

/********************************************************************
** Solidified function: render
********************************************************************/
be_local_closure(class_BeaconAnimation_render,   /* name */
  be_nested_proto(
    23,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_BeaconAnimation,     /* shared constants */
    be_str_weak(render),
    &be_const_str_solidified,
    ( &(const binstruction[106]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x1C0C0203,  //  0001  EQ	R3	R1	R3
      0x780E0001,  //  0002  JMPF	R3	#0005
      0x500C0000,  //  0003  LDBOOL	R3	0	0
      0x80040600,  //  0004  RET	1	R3
      0x4C0C0000,  //  0005  LDNIL	R3
      0x1C0C0403,  //  0006  EQ	R3	R2	R3
      0x780E0001,  //  0007  JMPF	R3	#000A
      0x880C0100,  //  0008  GETMBR	R3	R0	K0
      0x88080701,  //  0009  GETMBR	R2	R3	K1
      0x880C0302,  //  000A  GETMBR	R3	R1	K2
      0x88100103,  //  000B  GETMBR	R4	R0	K3
      0x88140104,  //  000C  GETMBR	R5	R0	K4
      0x88180105,  //  000D  GETMBR	R6	R0	K5
      0x881C0106,  //  000E  GETMBR	R7	R0	K6
      0x88200107,  //  000F  GETMBR	R8	R0	K7
      0x20240908,  //  0010  NE	R9	R4	K8
      0x78260002,  //  0011  JMPF	R9	#0015
      0x8C240309,  //  0012  GETMET	R9	R1	K9
      0x5C2C0800,  //  0013  MOVE	R11	R4
      0x7C240400,  //  0014  CALL	R9	2
      0x5C240A00,  //  0015  MOVE	R9	R5
      0x00280A07,  //  0016  ADD	R10	R5	R7
      0x142C130A,  //  0017  LT	R11	R9	K10
      0x782E0000,  //  0018  JMPF	R11	#001A
      0x5824000A,  //  0019  LDCONST	R9	K10
      0x282C1403,  //  001A  GE	R11	R10	R3
      0x782E0000,  //  001B  JMPF	R11	#001D
      0x5C280600,  //  001C  MOVE	R10	R3
      0x5C2C1200,  //  001D  MOVE	R11	R9
      0x1430160A,  //  001E  LT	R12	R11	R10
      0x78320005,  //  001F  JMPF	R12	#0026
      0x8C30030B,  //  0020  GETMET	R12	R1	K11
      0x5C381600,  //  0021  MOVE	R14	R11
      0x5C3C1000,  //  0022  MOVE	R15	R8
      0x7C300600,  //  0023  CALL	R12	3
      0x002C170C,  //  0024  ADD	R11	R11	K12
      0x7001FFF7,  //  0025  JMP		#001E
      0x24300D0A,  //  0026  GT	R12	R6	K10
      0x7832003F,  //  0027  JMPF	R12	#0068
      0x04300A06,  //  0028  SUB	R12	R5	R6
      0x5C340A00,  //  0029  MOVE	R13	R5
      0x1438190A,  //  002A  LT	R14	R12	K10
      0x783A0000,  //  002B  JMPF	R14	#002D
      0x5830000A,  //  002C  LDCONST	R12	K10
      0x28381A03,  //  002D  GE	R14	R13	R3
      0x783A0000,  //  002E  JMPF	R14	#0030
      0x5C340600,  //  002F  MOVE	R13	R3
      0x5C2C1800,  //  0030  MOVE	R11	R12
      0x1438160D,  //  0031  LT	R14	R11	R13
      0x783A0013,  //  0032  JMPF	R14	#0047
      0xB83A1A00,  //  0033  GETNGBL	R14	K13
      0x8C381D0E,  //  0034  GETMET	R14	R14	K14
      0x5C401600,  //  0035  MOVE	R16	R11
      0x04440A06,  //  0036  SUB	R17	R5	R6
      0x0444230C,  //  0037  SUB	R17	R17	K12
      0x5C480A00,  //  0038  MOVE	R18	R5
      0x544E00FE,  //  0039  LDINT	R19	255
      0x5850000A,  //  003A  LDCONST	R20	K10
      0x7C380C00,  //  003B  CALL	R14	6
      0x8C3C030F,  //  003C  GETMET	R15	R1	K15
      0x5C440800,  //  003D  MOVE	R17	R4
      0x5C481000,  //  003E  MOVE	R18	R8
      0x5C4C1C00,  //  003F  MOVE	R19	R14
      0x7C3C0800,  //  0040  CALL	R15	4
      0x8C40030B,  //  0041  GETMET	R16	R1	K11
      0x5C481600,  //  0042  MOVE	R18	R11
      0x5C4C1E00,  //  0043  MOVE	R19	R15
      0x7C400600,  //  0044  CALL	R16	3
      0x002C170C,  //  0045  ADD	R11	R11	K12
      0x7001FFE9,  //  0046  JMP		#0031
      0x00380A07,  //  0047  ADD	R14	R5	R7
      0x003C0A07,  //  0048  ADD	R15	R5	R7
      0x003C1E06,  //  0049  ADD	R15	R15	R6
      0x14401D0A,  //  004A  LT	R16	R14	K10
      0x78420000,  //  004B  JMPF	R16	#004D
      0x5838000A,  //  004C  LDCONST	R14	K10
      0x28401E03,  //  004D  GE	R16	R15	R3
      0x78420000,  //  004E  JMPF	R16	#0050
      0x5C3C0600,  //  004F  MOVE	R15	R3
      0x5C2C1C00,  //  0050  MOVE	R11	R14
      0x1440160F,  //  0051  LT	R16	R11	R15
      0x78420014,  //  0052  JMPF	R16	#0068
      0xB8421A00,  //  0053  GETNGBL	R16	K13
      0x8C40210E,  //  0054  GETMET	R16	R16	K14
      0x5C481600,  //  0055  MOVE	R18	R11
      0x004C0A07,  //  0056  ADD	R19	R5	R7
      0x044C270C,  //  0057  SUB	R19	R19	K12
      0x00500A07,  //  0058  ADD	R20	R5	R7
      0x00502806,  //  0059  ADD	R20	R20	R6
      0x5854000A,  //  005A  LDCONST	R21	K10
      0x545A00FE,  //  005B  LDINT	R22	255
      0x7C400C00,  //  005C  CALL	R16	6
      0x8C44030F,  //  005D  GETMET	R17	R1	K15
      0x5C4C0800,  //  005E  MOVE	R19	R4
      0x5C501000,  //  005F  MOVE	R20	R8
      0x5C542000,  //  0060  MOVE	R21	R16
      0x7C440800,  //  0061  CALL	R17	4
      0x8C48030B,  //  0062  GETMET	R18	R1	K11
      0x5C501600,  //  0063  MOVE	R20	R11
      0x5C542200,  //  0064  MOVE	R21	R17
      0x7C480600,  //  0065  CALL	R18	3
      0x002C170C,  //  0066  ADD	R11	R11	K12
      0x7001FFE8,  //  0067  JMP		#0051
      0x50300200,  //  0068  LDBOOL	R12	1	0
      0x80041800,  //  0069  RET	1	R12
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_BeaconAnimation_tostring,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_BeaconAnimation,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x60040018,  //  0000  GETGBL	R1	G24
      0x58080010,  //  0001  LDCONST	R2	K16
      0x880C0107,  //  0002  GETMBR	R3	R0	K7
      0x88100104,  //  0003  GETMBR	R4	R0	K4
      0x88140106,  //  0004  GETMBR	R5	R0	K6
      0x88180105,  //  0005  GETMBR	R6	R0	K5
      0x7C040A00,  //  0006  CALL	R1	5
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: BeaconAnimation
********************************************************************/
extern const bclass be_class_Animation;
be_local_class(BeaconAnimation,
    0,
    &be_class_Animation,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(PARAMS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(back_color, 4), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(-16777216) },
    }))    ) } )) },
        { be_const_key_weak(color, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(-1) },
    }))    ) } )) },
        { be_const_key_weak(slew_size, 0), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(0) },
        { be_const_key_weak(min, -1), be_const_int(0) },
    }))    ) } )) },
        { be_const_key_weak(beacon_size, 1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(1) },
        { be_const_key_weak(min, -1), be_const_int(0) },
    }))    ) } )) },
        { be_const_key_weak(pos, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(0) },
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(render, 2), be_const_closure(class_BeaconAnimation_render_closure) },
        { be_const_key_weak(tostring, -1), be_const_closure(class_BeaconAnimation_tostring_closure) },
    })),
    be_str_weak(BeaconAnimation)
);

/********************************************************************
** Solidified function: pulsating_color_provider
********************************************************************/
be_local_closure(pulsating_color_provider,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(breathe_color),
    /* K2   */  be_nested_str_weak(curve_factor),
    /* K3   */  be_const_int(1),
    /* K4   */  be_nested_str_weak(duration),
    }),
    be_str_weak(pulsating_color_provider),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x90060503,  //  0004  SETMBR	R1	K2	K3
      0x540A03E7,  //  0005  LDINT	R2	1000
      0x90060802,  //  0006  SETMBR	R1	K4	R2
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: noise_fractal
********************************************************************/
be_local_closure(noise_fractal,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(noise_animation),
    /* K2   */  be_nested_str_weak(rich_palette),
    /* K3   */  be_nested_str_weak(palette),
    /* K4   */  be_nested_str_weak(PALETTE_RAINBOW),
    /* K5   */  be_nested_str_weak(cycle_period),
    /* K6   */  be_nested_str_weak(transition_type),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str_weak(brightness),
    /* K9   */  be_nested_str_weak(range_min),
    /* K10  */  be_const_int(0),
    /* K11  */  be_nested_str_weak(range_max),
    /* K12  */  be_nested_str_weak(color),
    /* K13  */  be_nested_str_weak(scale),
    /* K14  */  be_nested_str_weak(speed),
    /* K15  */  be_nested_str_weak(octaves),
    /* K16  */  be_const_int(3),
    /* K17  */  be_nested_str_weak(persistence),
    }),
    be_str_weak(noise_fractal),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0xB80A0000,  //  0004  GETNGBL	R2	K0
      0x8C080502,  //  0005  GETMET	R2	R2	K2
      0x5C100000,  //  0006  MOVE	R4	R0
      0x7C080400,  //  0007  CALL	R2	2
      0xB80E0000,  //  0008  GETNGBL	R3	K0
      0x880C0704,  //  0009  GETMBR	R3	R3	K4
      0x900A0603,  //  000A  SETMBR	R2	K3	R3
      0x540E1387,  //  000B  LDINT	R3	5000
      0x900A0A03,  //  000C  SETMBR	R2	K5	R3
      0x900A0D07,  //  000D  SETMBR	R2	K6	K7
      0x540E00FE,  //  000E  LDINT	R3	255
      0x900A1003,  //  000F  SETMBR	R2	K8	R3
      0x900A130A,  //  0010  SETMBR	R2	K9	K10
      0x540E00FE,  //  0011  LDINT	R3	255
      0x900A1603,  //  0012  SETMBR	R2	K11	R3
      0x90061802,  //  0013  SETMBR	R1	K12	R2
      0x540E001D,  //  0014  LDINT	R3	30
      0x90061A03,  //  0015  SETMBR	R1	K13	R3
      0x540E0013,  //  0016  LDINT	R3	20
      0x90061C03,  //  0017  SETMBR	R1	K14	R3
      0x90061F10,  //  0018  SETMBR	R1	K15	K16
      0x540E007F,  //  0019  LDINT	R3	128
      0x90062203,  //  001A  SETMBR	R1	K17	R3
      0x80040200,  //  001B  RET	1	R1
    })
  )
);
/*******************************************************************/

// compact class 'GradientAnimation' ktab size: 43, total: 82 (saved 312 bytes)
static const bvalue be_ktab_class_GradientAnimation[43] = {
  /* K0   */  be_nested_str_weak(update),
  /* K1   */  be_nested_str_weak(movement_speed),
  /* K2   */  be_const_int(0),
  /* K3   */  be_nested_str_weak(start_time),
  /* K4   */  be_nested_str_weak(tasmota),
  /* K5   */  be_nested_str_weak(scale_uint),
  /* K6   */  be_nested_str_weak(phase_offset),
  /* K7   */  be_nested_str_weak(_calculate_gradient),
  /* K8   */  be_const_int(1),
  /* K9   */  be_nested_str_weak(direction),
  /* K10  */  be_nested_str_weak(spread),
  /* K11  */  be_nested_str_weak(gradient_type),
  /* K12  */  be_nested_str_weak(color),
  /* K13  */  be_nested_str_weak(priority),
  /* K14  */  be_nested_str_weak(linear),
  /* K15  */  be_nested_str_weak(radial),
  /* K16  */  be_nested_str_weak(animation),
  /* K17  */  be_nested_str_weak(is_value_provider),
  /* K18  */  be_nested_str_weak(rainbow),
  /* K19  */  be_nested_str_weak(0x_X2508x),
  /* K20  */  be_nested_str_weak(GradientAnimation_X28_X25s_X2C_X20color_X3D_X25s_X2C_X20movement_X3D_X25s_X2C_X20priority_X3D_X25s_X2C_X20running_X3D_X25s_X29),
  /* K21  */  be_nested_str_weak(is_running),
  /* K22  */  be_nested_str_weak(engine),
  /* K23  */  be_nested_str_weak(get_strip_length),
  /* K24  */  be_nested_str_weak(width),
  /* K25  */  be_nested_str_weak(current_colors),
  /* K26  */  be_nested_str_weak(set_pixel_color),
  /* K27  */  be_nested_str_weak(resize),
  /* K28  */  be_const_int(-16777216),
  /* K29  */  be_nested_str_weak(_calculate_linear_position),
  /* K30  */  be_nested_str_weak(_calculate_radial_position),
  /* K31  */  be_nested_str_weak(light_state),
  /* K32  */  be_const_int(3),
  /* K33  */  be_nested_str_weak(HsToRgb),
  /* K34  */  be_nested_str_weak(r),
  /* K35  */  be_nested_str_weak(g),
  /* K36  */  be_nested_str_weak(b),
  /* K37  */  be_nested_str_weak(is_color_provider),
  /* K38  */  be_nested_str_weak(get_color_for_value),
  /* K39  */  be_nested_str_weak(resolve_value),
  /* K40  */  be_nested_str_weak(int),
  /* K41  */  be_nested_str_weak(init),
  /* K42  */  be_nested_str_weak(center_pos),
};


extern const bclass be_class_GradientAnimation;

/********************************************************************
** Solidified function: update
********************************************************************/
be_local_closure(class_GradientAnimation_update,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_GradientAnimation,     /* shared constants */
    be_str_weak(update),
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x740A0001,  //  0006  JMPT	R2	#0009
      0x50080000,  //  0007  LDBOOL	R2	0	0
      0x80040400,  //  0008  RET	1	R2
      0x88080101,  //  0009  GETMBR	R2	R0	K1
      0x240C0502,  //  000A  GT	R3	R2	K2
      0x780E0011,  //  000B  JMPF	R3	#001E
      0x880C0103,  //  000C  GETMBR	R3	R0	K3
      0x040C0203,  //  000D  SUB	R3	R1	R3
      0xB8120800,  //  000E  GETNGBL	R4	K4
      0x8C100905,  //  000F  GETMET	R4	R4	K5
      0x5C180400,  //  0010  MOVE	R6	R2
      0x581C0002,  //  0011  LDCONST	R7	K2
      0x542200FE,  //  0012  LDINT	R8	255
      0x58240002,  //  0013  LDCONST	R9	K2
      0x542A0009,  //  0014  LDINT	R10	10
      0x7C100C00,  //  0015  CALL	R4	6
      0x24140902,  //  0016  GT	R5	R4	K2
      0x78160005,  //  0017  JMPF	R5	#001E
      0x08140604,  //  0018  MUL	R5	R3	R4
      0x541A03E7,  //  0019  LDINT	R6	1000
      0x0C140A06,  //  001A  DIV	R5	R5	R6
      0x541A00FF,  //  001B  LDINT	R6	256
      0x10140A06,  //  001C  MOD	R5	R5	R6
      0x90020C05,  //  001D  SETMBR	R0	K6	R5
      0x8C0C0107,  //  001E  GETMET	R3	R0	K7
      0x5C140200,  //  001F  MOVE	R5	R1
      0x7C0C0400,  //  0020  CALL	R3	2
      0x500C0200,  //  0021  LDBOOL	R3	1	0
      0x80040600,  //  0022  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _calculate_linear_position
********************************************************************/
be_local_closure(class_GradientAnimation__calculate_linear_position,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_GradientAnimation,     /* shared constants */
    be_str_weak(_calculate_linear_position),
    &be_const_str_solidified,
    ( &(const binstruction[50]) {  /* code */
      0xB80E0800,  //  0000  GETNGBL	R3	K4
      0x8C0C0705,  //  0001  GETMET	R3	R3	K5
      0x5C140200,  //  0002  MOVE	R5	R1
      0x58180002,  //  0003  LDCONST	R6	K2
      0x041C0508,  //  0004  SUB	R7	R2	K8
      0x58200002,  //  0005  LDCONST	R8	K2
      0x542600FE,  //  0006  LDINT	R9	255
      0x7C0C0C00,  //  0007  CALL	R3	6
      0x88100109,  //  0008  GETMBR	R4	R0	K9
      0x8814010A,  //  0009  GETMBR	R5	R0	K10
      0x541A007F,  //  000A  LDINT	R6	128
      0x18180806,  //  000B  LE	R6	R4	R6
      0x781A000C,  //  000C  JMPF	R6	#001A
      0xB81A0800,  //  000D  GETNGBL	R6	K4
      0x8C180D05,  //  000E  GETMET	R6	R6	K5
      0x5C200800,  //  000F  MOVE	R8	R4
      0x58240002,  //  0010  LDCONST	R9	K2
      0x542A007F,  //  0011  LDINT	R10	128
      0x582C0002,  //  0012  LDCONST	R11	K2
      0x5432007F,  //  0013  LDINT	R12	128
      0x7C180C00,  //  0014  CALL	R6	6
      0x001C0606,  //  0015  ADD	R7	R3	R6
      0x542200FF,  //  0016  LDINT	R8	256
      0x101C0E08,  //  0017  MOD	R7	R7	R8
      0x5C0C0E00,  //  0018  MOVE	R3	R7
      0x7002000D,  //  0019  JMP		#0028
      0xB81A0800,  //  001A  GETNGBL	R6	K4
      0x8C180D05,  //  001B  GETMET	R6	R6	K5
      0x5C200800,  //  001C  MOVE	R8	R4
      0x5426007F,  //  001D  LDINT	R9	128
      0x542A00FE,  //  001E  LDINT	R10	255
      0x582C0002,  //  001F  LDCONST	R11	K2
      0x543200FE,  //  0020  LDINT	R12	255
      0x7C180C00,  //  0021  CALL	R6	6
      0x541E00FE,  //  0022  LDINT	R7	255
      0x00200606,  //  0023  ADD	R8	R3	R6
      0x542600FF,  //  0024  LDINT	R9	256
      0x10201009,  //  0025  MOD	R8	R8	R9
      0x041C0E08,  //  0026  SUB	R7	R7	R8
      0x5C0C0E00,  //  0027  MOVE	R3	R7
      0xB81A0800,  //  0028  GETNGBL	R6	K4
      0x8C180D05,  //  0029  GETMET	R6	R6	K5
      0x5C200600,  //  002A  MOVE	R8	R3
      0x58240002,  //  002B  LDCONST	R9	K2
      0x542A00FE,  //  002C  LDINT	R10	255
      0x582C0002,  //  002D  LDCONST	R11	K2
      0x5C300A00,  //  002E  MOVE	R12	R5
      0x7C180C00,  //  002F  CALL	R6	6
      0x5C0C0C00,  //  0030  MOVE	R3	R6
      0x80040600,  //  0031  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_GradientAnimation_tostring,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_GradientAnimation,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[39]) {  /* code */
      0x8804010B,  //  0000  GETMBR	R1	R0	K11
      0x8808010C,  //  0001  GETMBR	R2	R0	K12
      0x880C0101,  //  0002  GETMBR	R3	R0	K1
      0x8810010D,  //  0003  GETMBR	R4	R0	K13
      0x1C140302,  //  0004  EQ	R5	R1	K2
      0x78160001,  //  0005  JMPF	R5	#0008
      0x5814000E,  //  0006  LDCONST	R5	K14
      0x70020000,  //  0007  JMP		#0009
      0x5814000F,  //  0008  LDCONST	R5	K15
      0x4C180000,  //  0009  LDNIL	R6
      0xB81E2000,  //  000A  GETNGBL	R7	K16
      0x8C1C0F11,  //  000B  GETMET	R7	R7	K17
      0x5C240400,  //  000C  MOVE	R9	R2
      0x7C1C0400,  //  000D  CALL	R7	2
      0x781E0004,  //  000E  JMPF	R7	#0014
      0x601C0008,  //  000F  GETGBL	R7	G8
      0x5C200400,  //  0010  MOVE	R8	R2
      0x7C1C0200,  //  0011  CALL	R7	1
      0x5C180E00,  //  0012  MOVE	R6	R7
      0x70020009,  //  0013  JMP		#001E
      0x4C1C0000,  //  0014  LDNIL	R7
      0x1C1C0407,  //  0015  EQ	R7	R2	R7
      0x781E0001,  //  0016  JMPF	R7	#0019
      0x58180012,  //  0017  LDCONST	R6	K18
      0x70020004,  //  0018  JMP		#001E
      0x601C0018,  //  0019  GETGBL	R7	G24
      0x58200013,  //  001A  LDCONST	R8	K19
      0x5C240400,  //  001B  MOVE	R9	R2
      0x7C1C0400,  //  001C  CALL	R7	2
      0x5C180E00,  //  001D  MOVE	R6	R7
      0x601C0018,  //  001E  GETGBL	R7	G24
      0x58200014,  //  001F  LDCONST	R8	K20
      0x5C240A00,  //  0020  MOVE	R9	R5
      0x5C280C00,  //  0021  MOVE	R10	R6
      0x5C2C0600,  //  0022  MOVE	R11	R3
      0x5C300800,  //  0023  MOVE	R12	R4
      0x88340115,  //  0024  GETMBR	R13	R0	K21
      0x7C1C0C00,  //  0025  CALL	R7	6
      0x80040E00,  //  0026  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: render
********************************************************************/
be_local_closure(class_GradientAnimation_render,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_GradientAnimation,     /* shared constants */
    be_str_weak(render),
    &be_const_str_solidified,
    ( &(const binstruction[30]) {  /* code */
      0x880C0115,  //  0000  GETMBR	R3	R0	K21
      0x780E0002,  //  0001  JMPF	R3	#0005
      0x4C0C0000,  //  0002  LDNIL	R3
      0x1C0C0203,  //  0003  EQ	R3	R1	R3
      0x780E0001,  //  0004  JMPF	R3	#0007
      0x500C0000,  //  0005  LDBOOL	R3	0	0
      0x80040600,  //  0006  RET	1	R3
      0x880C0116,  //  0007  GETMBR	R3	R0	K22
      0x8C0C0717,  //  0008  GETMET	R3	R3	K23
      0x7C0C0200,  //  0009  CALL	R3	1
      0x58100002,  //  000A  LDCONST	R4	K2
      0x14140803,  //  000B  LT	R5	R4	R3
      0x7816000E,  //  000C  JMPF	R5	#001C
      0x88140318,  //  000D  GETMBR	R5	R1	K24
      0x14140805,  //  000E  LT	R5	R4	R5
      0x7816000B,  //  000F  JMPF	R5	#001C
      0x6014000C,  //  0010  GETGBL	R5	G12
      0x88180119,  //  0011  GETMBR	R6	R0	K25
      0x7C140200,  //  0012  CALL	R5	1
      0x14140805,  //  0013  LT	R5	R4	R5
      0x78160004,  //  0014  JMPF	R5	#001A
      0x8C14031A,  //  0015  GETMET	R5	R1	K26
      0x5C1C0800,  //  0016  MOVE	R7	R4
      0x88200119,  //  0017  GETMBR	R8	R0	K25
      0x94201004,  //  0018  GETIDX	R8	R8	R4
      0x7C140600,  //  0019  CALL	R5	3
      0x00100908,  //  001A  ADD	R4	R4	K8
      0x7001FFEE,  //  001B  JMP		#000B
      0x50140200,  //  001C  LDBOOL	R5	1	0
      0x80040A00,  //  001D  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: on_param_changed
********************************************************************/
be_local_closure(class_GradientAnimation_on_param_changed,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_GradientAnimation,     /* shared constants */
    be_str_weak(on_param_changed),
    &be_const_str_solidified,
    ( &(const binstruction[37]) {  /* code */
      0x880C0116,  //  0000  GETMBR	R3	R0	K22
      0x8C0C0717,  //  0001  GETMET	R3	R3	K23
      0x7C0C0200,  //  0002  CALL	R3	1
      0x6010000C,  //  0003  GETGBL	R4	G12
      0x88140119,  //  0004  GETMBR	R5	R0	K25
      0x7C100200,  //  0005  CALL	R4	1
      0x20100803,  //  0006  NE	R4	R4	R3
      0x7812001B,  //  0007  JMPF	R4	#0024
      0x88100119,  //  0008  GETMBR	R4	R0	K25
      0x8C10091B,  //  0009  GETMET	R4	R4	K27
      0x5C180600,  //  000A  MOVE	R6	R3
      0x7C100400,  //  000B  CALL	R4	2
      0x6010000C,  //  000C  GETGBL	R4	G12
      0x88140119,  //  000D  GETMBR	R5	R0	K25
      0x7C100200,  //  000E  CALL	R4	1
      0x14140803,  //  000F  LT	R5	R4	R3
      0x78160012,  //  0010  JMPF	R5	#0024
      0x6014000C,  //  0011  GETGBL	R5	G12
      0x88180119,  //  0012  GETMBR	R6	R0	K25
      0x7C140200,  //  0013  CALL	R5	1
      0x28140805,  //  0014  GE	R5	R4	R5
      0x74160004,  //  0015  JMPT	R5	#001B
      0x88140119,  //  0016  GETMBR	R5	R0	K25
      0x94140A04,  //  0017  GETIDX	R5	R5	R4
      0x4C180000,  //  0018  LDNIL	R6
      0x1C140A06,  //  0019  EQ	R5	R5	R6
      0x78160006,  //  001A  JMPF	R5	#0022
      0x6014000C,  //  001B  GETGBL	R5	G12
      0x88180119,  //  001C  GETMBR	R6	R0	K25
      0x7C140200,  //  001D  CALL	R5	1
      0x14140805,  //  001E  LT	R5	R4	R5
      0x78160001,  //  001F  JMPF	R5	#0022
      0x88140119,  //  0020  GETMBR	R5	R0	K25
      0x9814091C,  //  0021  SETIDX	R5	R4	K28
      0x00100908,  //  0022  ADD	R4	R4	K8
      0x7001FFEA,  //  0023  JMP		#000F
      0x80000000,  //  0024  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _calculate_gradient
********************************************************************/
be_local_closure(class_GradientAnimation__calculate_gradient,   /* name */
  be_nested_proto(
    18,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_GradientAnimation,     /* shared constants */
    be_str_weak(_calculate_gradient),
    &be_const_str_solidified,
    ( &(const binstruction[149]) {  /* code */
      0x8808010B,  //  0000  GETMBR	R2	R0	K11
      0x880C010C,  //  0001  GETMBR	R3	R0	K12
      0x88100116,  //  0002  GETMBR	R4	R0	K22
      0x8C100917,  //  0003  GETMET	R4	R4	K23
      0x7C100200,  //  0004  CALL	R4	1
      0x6014000C,  //  0005  GETGBL	R5	G12
      0x88180119,  //  0006  GETMBR	R6	R0	K25
      0x7C140200,  //  0007  CALL	R5	1
      0x20140A04,  //  0008  NE	R5	R5	R4
      0x78160003,  //  0009  JMPF	R5	#000E
      0x88140119,  //  000A  GETMBR	R5	R0	K25
      0x8C140B1B,  //  000B  GETMET	R5	R5	K27
      0x5C1C0800,  //  000C  MOVE	R7	R4
      0x7C140400,  //  000D  CALL	R5	2
      0x58140002,  //  000E  LDCONST	R5	K2
      0x14180A04,  //  000F  LT	R6	R5	R4
      0x781A0082,  //  0010  JMPF	R6	#0094
      0x58180002,  //  0011  LDCONST	R6	K2
      0x1C1C0502,  //  0012  EQ	R7	R2	K2
      0x781E0005,  //  0013  JMPF	R7	#001A
      0x8C1C011D,  //  0014  GETMET	R7	R0	K29
      0x5C240A00,  //  0015  MOVE	R9	R5
      0x5C280800,  //  0016  MOVE	R10	R4
      0x7C1C0600,  //  0017  CALL	R7	3
      0x5C180E00,  //  0018  MOVE	R6	R7
      0x70020004,  //  0019  JMP		#001F
      0x8C1C011E,  //  001A  GETMET	R7	R0	K30
      0x5C240A00,  //  001B  MOVE	R9	R5
      0x5C280800,  //  001C  MOVE	R10	R4
      0x7C1C0600,  //  001D  CALL	R7	3
      0x5C180E00,  //  001E  MOVE	R6	R7
      0x881C0106,  //  001F  GETMBR	R7	R0	K6
      0x001C0C07,  //  0020  ADD	R7	R6	R7
      0x542200FF,  //  0021  LDINT	R8	256
      0x101C0E08,  //  0022  MOD	R7	R7	R8
      0x5C180E00,  //  0023  MOVE	R6	R7
      0x581C001C,  //  0024  LDCONST	R7	K28
      0x4C200000,  //  0025  LDNIL	R8
      0x1C200608,  //  0026  EQ	R8	R3	R8
      0x7822001B,  //  0027  JMPF	R8	#0044
      0xB8220800,  //  0028  GETNGBL	R8	K4
      0x8C201105,  //  0029  GETMET	R8	R8	K5
      0x5C280C00,  //  002A  MOVE	R10	R6
      0x582C0002,  //  002B  LDCONST	R11	K2
      0x543200FE,  //  002C  LDINT	R12	255
      0x58340002,  //  002D  LDCONST	R13	K2
      0x543A0166,  //  002E  LDINT	R14	359
      0x7C200C00,  //  002F  CALL	R8	6
      0xA4263E00,  //  0030  IMPORT	R9	K31
      0x5C281200,  //  0031  MOVE	R10	R9
      0x582C0020,  //  0032  LDCONST	R11	K32
      0x7C280200,  //  0033  CALL	R10	1
      0x8C2C1521,  //  0034  GETMET	R11	R10	K33
      0x5C341000,  //  0035  MOVE	R13	R8
      0x543A00FE,  //  0036  LDINT	R14	255
      0x7C2C0600,  //  0037  CALL	R11	3
      0x882C1522,  //  0038  GETMBR	R11	R10	K34
      0x5432000F,  //  0039  LDINT	R12	16
      0x382C160C,  //  003A  SHL	R11	R11	R12
      0x302E380B,  //  003B  OR	R11	K28	R11
      0x88301523,  //  003C  GETMBR	R12	R10	K35
      0x54360007,  //  003D  LDINT	R13	8
      0x3830180D,  //  003E  SHL	R12	R12	R13
      0x302C160C,  //  003F  OR	R11	R11	R12
      0x88301524,  //  0040  GETMBR	R12	R10	K36
      0x302C160C,  //  0041  OR	R11	R11	R12
      0x5C1C1600,  //  0042  MOVE	R7	R11
      0x7002004B,  //  0043  JMP		#0090
      0xB8222000,  //  0044  GETNGBL	R8	K16
      0x8C201125,  //  0045  GETMET	R8	R8	K37
      0x5C280600,  //  0046  MOVE	R10	R3
      0x7C200400,  //  0047  CALL	R8	2
      0x78220009,  //  0048  JMPF	R8	#0053
      0x88200726,  //  0049  GETMBR	R8	R3	K38
      0x4C240000,  //  004A  LDNIL	R9
      0x20201009,  //  004B  NE	R8	R8	R9
      0x78220005,  //  004C  JMPF	R8	#0053
      0x8C200726,  //  004D  GETMET	R8	R3	K38
      0x5C280C00,  //  004E  MOVE	R10	R6
      0x582C0002,  //  004F  LDCONST	R11	K2
      0x7C200600,  //  0050  CALL	R8	3
      0x5C1C1000,  //  0051  MOVE	R7	R8
      0x7002003C,  //  0052  JMP		#0090
      0xB8222000,  //  0053  GETNGBL	R8	K16
      0x8C201111,  //  0054  GETMET	R8	R8	K17
      0x5C280600,  //  0055  MOVE	R10	R3
      0x7C200400,  //  0056  CALL	R8	2
      0x78220008,  //  0057  JMPF	R8	#0061
      0x8C200127,  //  0058  GETMET	R8	R0	K39
      0x5C280600,  //  0059  MOVE	R10	R3
      0x582C000C,  //  005A  LDCONST	R11	K12
      0x54320009,  //  005B  LDINT	R12	10
      0x08300C0C,  //  005C  MUL	R12	R6	R12
      0x0030020C,  //  005D  ADD	R12	R1	R12
      0x7C200800,  //  005E  CALL	R8	4
      0x5C1C1000,  //  005F  MOVE	R7	R8
      0x7002002E,  //  0060  JMP		#0090
      0x60200004,  //  0061  GETGBL	R8	G4
      0x5C240600,  //  0062  MOVE	R9	R3
      0x7C200200,  //  0063  CALL	R8	1
      0x1C201128,  //  0064  EQ	R8	R8	K40
      0x78220028,  //  0065  JMPF	R8	#008F
      0x5C200C00,  //  0066  MOVE	R8	R6
      0xB8260800,  //  0067  GETNGBL	R9	K4
      0x8C241305,  //  0068  GETMET	R9	R9	K5
      0x5C2C1000,  //  0069  MOVE	R11	R8
      0x58300002,  //  006A  LDCONST	R12	K2
      0x543600FE,  //  006B  LDINT	R13	255
      0x58380002,  //  006C  LDCONST	R14	K2
      0x543E000F,  //  006D  LDINT	R15	16
      0x3C3C060F,  //  006E  SHR	R15	R3	R15
      0x544200FE,  //  006F  LDINT	R16	255
      0x2C3C1E10,  //  0070  AND	R15	R15	R16
      0x7C240C00,  //  0071  CALL	R9	6
      0xB82A0800,  //  0072  GETNGBL	R10	K4
      0x8C281505,  //  0073  GETMET	R10	R10	K5
      0x5C301000,  //  0074  MOVE	R12	R8
      0x58340002,  //  0075  LDCONST	R13	K2
      0x543A00FE,  //  0076  LDINT	R14	255
      0x583C0002,  //  0077  LDCONST	R15	K2
      0x54420007,  //  0078  LDINT	R16	8
      0x3C400610,  //  0079  SHR	R16	R3	R16
      0x544600FE,  //  007A  LDINT	R17	255
      0x2C402011,  //  007B  AND	R16	R16	R17
      0x7C280C00,  //  007C  CALL	R10	6
      0xB82E0800,  //  007D  GETNGBL	R11	K4
      0x8C2C1705,  //  007E  GETMET	R11	R11	K5
      0x5C341000,  //  007F  MOVE	R13	R8
      0x58380002,  //  0080  LDCONST	R14	K2
      0x543E00FE,  //  0081  LDINT	R15	255
      0x58400002,  //  0082  LDCONST	R16	K2
      0x544600FE,  //  0083  LDINT	R17	255
      0x2C440611,  //  0084  AND	R17	R3	R17
      0x7C2C0C00,  //  0085  CALL	R11	6
      0x5432000F,  //  0086  LDINT	R12	16
      0x3830120C,  //  0087  SHL	R12	R9	R12
      0x3032380C,  //  0088  OR	R12	K28	R12
      0x54360007,  //  0089  LDINT	R13	8
      0x3834140D,  //  008A  SHL	R13	R10	R13
      0x3030180D,  //  008B  OR	R12	R12	R13
      0x3030180B,  //  008C  OR	R12	R12	R11
      0x5C1C1800,  //  008D  MOVE	R7	R12
      0x70020000,  //  008E  JMP		#0090
      0x5C1C0600,  //  008F  MOVE	R7	R3
      0x88200119,  //  0090  GETMBR	R8	R0	K25
      0x98200A07,  //  0091  SETIDX	R8	R5	R7
      0x00140B08,  //  0092  ADD	R5	R5	K8
      0x7001FF7A,  //  0093  JMP		#000F
      0x80000000,  //  0094  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_GradientAnimation_init,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_GradientAnimation,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080529,  //  0003  GETMET	R2	R2	K41
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x60080012,  //  0006  GETGBL	R2	G18
      0x7C080000,  //  0007  CALL	R2	0
      0x90023202,  //  0008  SETMBR	R0	K25	R2
      0x90020D02,  //  0009  SETMBR	R0	K6	K2
      0x88080116,  //  000A  GETMBR	R2	R0	K22
      0x8C080517,  //  000B  GETMET	R2	R2	K23
      0x7C080200,  //  000C  CALL	R2	1
      0x880C0119,  //  000D  GETMBR	R3	R0	K25
      0x8C0C071B,  //  000E  GETMET	R3	R3	K27
      0x5C140400,  //  000F  MOVE	R5	R2
      0x7C0C0400,  //  0010  CALL	R3	2
      0x580C0002,  //  0011  LDCONST	R3	K2
      0x14100602,  //  0012  LT	R4	R3	R2
      0x78120003,  //  0013  JMPF	R4	#0018
      0x88100119,  //  0014  GETMBR	R4	R0	K25
      0x9810071C,  //  0015  SETIDX	R4	R3	K28
      0x000C0708,  //  0016  ADD	R3	R3	K8
      0x7001FFF9,  //  0017  JMP		#0012
      0x80000000,  //  0018  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _calculate_radial_position
********************************************************************/
be_local_closure(class_GradientAnimation__calculate_radial_position,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_GradientAnimation,     /* shared constants */
    be_str_weak(_calculate_radial_position),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0xB80E0800,  //  0000  GETNGBL	R3	K4
      0x8C0C0705,  //  0001  GETMET	R3	R3	K5
      0x5C140200,  //  0002  MOVE	R5	R1
      0x58180002,  //  0003  LDCONST	R6	K2
      0x041C0508,  //  0004  SUB	R7	R2	K8
      0x58200002,  //  0005  LDCONST	R8	K2
      0x542600FE,  //  0006  LDINT	R9	255
      0x7C0C0C00,  //  0007  CALL	R3	6
      0x8810012A,  //  0008  GETMBR	R4	R0	K42
      0x8814010A,  //  0009  GETMBR	R5	R0	K10
      0x58180002,  //  000A  LDCONST	R6	K2
      0x281C0604,  //  000B  GE	R7	R3	R4
      0x781E0002,  //  000C  JMPF	R7	#0010
      0x041C0604,  //  000D  SUB	R7	R3	R4
      0x5C180E00,  //  000E  MOVE	R6	R7
      0x70020001,  //  000F  JMP		#0012
      0x041C0803,  //  0010  SUB	R7	R4	R3
      0x5C180E00,  //  0011  MOVE	R6	R7
      0xB81E0800,  //  0012  GETNGBL	R7	K4
      0x8C1C0F05,  //  0013  GETMET	R7	R7	K5
      0x5C240C00,  //  0014  MOVE	R9	R6
      0x58280002,  //  0015  LDCONST	R10	K2
      0x542E007F,  //  0016  LDINT	R11	128
      0x58300002,  //  0017  LDCONST	R12	K2
      0x5C340A00,  //  0018  MOVE	R13	R5
      0x7C1C0C00,  //  0019  CALL	R7	6
      0x5C180E00,  //  001A  MOVE	R6	R7
      0x541E00FE,  //  001B  LDINT	R7	255
      0x241C0C07,  //  001C  GT	R7	R6	R7
      0x781E0000,  //  001D  JMPF	R7	#001F
      0x541A00FE,  //  001E  LDINT	R6	255
      0x80040C00,  //  001F  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: GradientAnimation
********************************************************************/
extern const bclass be_class_Animation;
be_local_class(GradientAnimation,
    2,
    &be_class_Animation,
    be_nested_map(11,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(current_colors, -1), be_const_var(0) },
        { be_const_key_weak(render, -1), be_const_closure(class_GradientAnimation_render_closure) },
        { be_const_key_weak(_calculate_linear_position, -1), be_const_closure(class_GradientAnimation__calculate_linear_position_closure) },
        { be_const_key_weak(tostring, -1), be_const_closure(class_GradientAnimation_tostring_closure) },
        { be_const_key_weak(update, 1), be_const_closure(class_GradientAnimation_update_closure) },
        { be_const_key_weak(on_param_changed, -1), be_const_closure(class_GradientAnimation_on_param_changed_closure) },
        { be_const_key_weak(phase_offset, -1), be_const_var(1) },
        { be_const_key_weak(_calculate_gradient, -1), be_const_closure(class_GradientAnimation__calculate_gradient_closure) },
        { be_const_key_weak(PARAMS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(direction, 3), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(0) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(center_pos, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(128) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(gradient_type, 5), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(0) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(1) },
    }))    ) } )) },
        { be_const_key_weak(color, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, 1), be_const_nil() },
        { be_const_key_weak(nillable, -1), be_const_bool(1) },
    }))    ) } )) },
        { be_const_key_weak(movement_speed, 0), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(0) },
        { be_const_key_weak(min, -1), be_const_int(0) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
        { be_const_key_weak(spread, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(default, -1), be_const_int(255) },
        { be_const_key_weak(min, -1), be_const_int(1) },
        { be_const_key_weak(max, 1), be_const_int(255) },
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(init, -1), be_const_closure(class_GradientAnimation_init_closure) },
        { be_const_key_weak(_calculate_radial_position, -1), be_const_closure(class_GradientAnimation__calculate_radial_position_closure) },
    })),
    be_str_weak(GradientAnimation)
);

extern const bclass be_class_ValueProvider;

/********************************************************************
** Solidified function: produce_value
********************************************************************/
be_local_closure(class_ValueProvider_produce_value,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(undefined),
    }),
    be_str_weak(produce_value),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x600C000B,  //  0000  GETGBL	R3	G11
      0x58100000,  //  0001  LDCONST	R4	K0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x80040600,  //  0003  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_ValueProvider_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: ValueProvider
********************************************************************/
extern const bclass be_class_ParameterizedObject;
be_local_class(ValueProvider,
    0,
    &be_class_ParameterizedObject,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(produce_value, 1), be_const_closure(class_ValueProvider_produce_value_closure) },
        { be_const_key_weak(PARAMS, 2), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(0,
    ( (struct bmapnode*) &(const bmapnode[]) {
    }))    ) } )) },
        { be_const_key_weak(init, -1), be_const_closure(class_ValueProvider_init_closure) },
    })),
    be_str_weak(ValueProvider)
);

/********************************************************************
** Solidified module: animation
********************************************************************/
be_local_module(animation,
    "animation",
    be_nested_map(121,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(static_color, 21), be_const_class(be_class_StaticColorProvider) },
        { be_const_key_weak(rich_palette_animation, 79), be_const_class(be_class_RichPaletteAnimation) },
        { be_const_key_weak(scale_animation, 64), be_const_class(be_class_ScaleAnimation) },
        { be_const_key_weak(rich_palette, 15), be_const_class(be_class_RichPaletteColorProvider) },
        { be_const_key_weak(color_cycle, -1), be_const_class(be_class_ColorCycleColorProvider) },
        { be_const_key_weak(value_provider, -1), be_const_class(be_class_ValueProvider) },
        { be_const_key_weak(palette_gradient_animation, -1), be_const_class(be_class_PaletteGradientAnimation) },
        { be_const_key_weak(set_event_active, -1), be_const_closure(set_event_active_closure) },
        { be_const_key_weak(sparkle_rainbow, -1), be_const_closure(sparkle_rainbow_closure) },
        { be_const_key_weak(animation_engine, -1), be_const_class(be_class_AnimationEngine) },
        { be_const_key_weak(gradient_animation, 66), be_const_class(be_class_GradientAnimation) },
        { be_const_key_weak(twinkle_gentle, -1), be_const_closure(twinkle_gentle_closure) },
        { be_const_key_weak(pulsating_animation, -1), be_const_closure(pulsating_animation_closure) },
        { be_const_key_weak(TRIANGLE, -1), be_const_int(2) },
        { be_const_key_weak(sparkle_animation, -1), be_const_class(be_class_SparkleAnimation) },
        { be_const_key_weak(jitter_position, -1), be_const_closure(jitter_position_closure) },
        { be_const_key_weak(pulsating_color, -1), be_const_closure(pulsating_color_provider_closure) },
        { be_const_key_weak(sparkle_white, -1), be_const_closure(sparkle_white_closure) },
        { be_const_key_weak(twinkle_intense, 43), be_const_closure(twinkle_intense_closure) },
        { be_const_key_weak(twinkle_classic, 16), be_const_closure(twinkle_classic_closure) },
        { be_const_key_weak(beacon_animation, -1), be_const_class(be_class_BeaconAnimation) },
        { be_const_key_weak(PALETTE_FOREST, -1), be_const_bytes_instance(0000640040228B228032CD32C09AFF9AFF90EE90) },
        { be_const_key_weak(strip_length, -1), be_const_class(be_class_StripLengthProvider) },
        { be_const_key_weak(is_color_provider, 30), be_const_closure(is_color_provider_closure) },
        { be_const_key_weak(ease_out, -1), be_const_closure(ease_out_closure) },
        { be_const_key_weak(COSINE, -1), be_const_int(4) },
        { be_const_key_weak(twinkle_animation, -1), be_const_class(be_class_TwinkleAnimation) },
        { be_const_key_weak(cosine_osc, -1), be_const_closure(cosine_osc_closure) },
        { be_const_key_weak(comet_animation, 103), be_const_class(be_class_CometAnimation) },
        { be_const_key_weak(noise_single_color, -1), be_const_closure(noise_single_color_closure) },
        { be_const_key_weak(parameterized_object, -1), be_const_class(be_class_ParameterizedObject) },
        { be_const_key_weak(elastic, 36), be_const_closure(elastic_closure) },
        { be_const_key_weak(init, -1), be_const_closure(animation_init_closure) },
        { be_const_key_weak(bounce_constrained, -1), be_const_closure(bounce_constrained_closure) },
        { be_const_key_weak(jitter_animation, -1), be_const_class(be_class_JitterAnimation) },
        { be_const_key_weak(register_event_handler, 112), be_const_closure(register_event_handler_closure) },
        { be_const_key_weak(shift_scroll_left, -1), be_const_closure(shift_scroll_left_closure) },
        { be_const_key_weak(breathe_color, 78), be_const_class(be_class_BreatheColorProvider) },
        { be_const_key_weak(composite_color, -1), be_const_class(be_class_CompositeColorProvider) },
        { be_const_key_weak(BOUNCE, -1), be_const_int(9) },
        { be_const_key_weak(plasma_animation, -1), be_const_class(be_class_PlasmaAnimation) },
        { be_const_key_weak(EASE_OUT, 48), be_const_int(7) },
        { be_const_key_weak(SQUARE, 89), be_const_int(3) },
        { be_const_key_weak(ease_in, 40), be_const_closure(ease_in_closure) },
        { be_const_key_weak(jitter_brightness, -1), be_const_closure(jitter_brightness_closure) },
        { be_const_key_weak(sine_osc, -1), be_const_closure(sine_osc_closure) },
        { be_const_key_weak(twinkle_rainbow, 68), be_const_closure(twinkle_rainbow_closure) },
        { be_const_key_weak(clear_all_event_handlers, -1), be_const_closure(clear_all_event_handlers_closure) },
        { be_const_key_weak(ramp, -1), be_const_closure(ramp_closure) },
        { be_const_key_weak(breathe_animation, -1), be_const_class(be_class_BreatheAnimation) },
        { be_const_key_weak(palette_pattern_animation, 84), be_const_class(be_class_PalettePatternAnimation) },
        { be_const_key_weak(PALETTE_RGB, -1), be_const_bytes_instance(00FF00008000FF00FF0000FF) },
        { be_const_key_weak(crenel_position_animation, -1), be_const_class(be_class_CrenelPositionAnimation) },
        { be_const_key_weak(create_closure_value, -1), be_const_closure(create_closure_value_closure) },
        { be_const_key_weak(init_strip, 34), be_const_closure(animation_init_strip_closure) },
        { be_const_key_weak(square, -1), be_const_closure(square_closure) },
        { be_const_key_weak(bounce_gravity, -1), be_const_closure(bounce_gravity_closure) },
        { be_const_key_weak(shift_fast_scroll, 75), be_const_closure(shift_fast_scroll_closure) },
        { be_const_key_weak(gradient_rainbow_linear, -1), be_const_closure(gradient_rainbow_linear_closure) },
        { be_const_key_weak(smooth, 29), be_const_closure(smooth_closure) },
        { be_const_key_weak(bounce_animation, -1), be_const_class(be_class_BounceAnimation) },
        { be_const_key_weak(gradient_two_color_linear, -1), be_const_closure(gradient_two_color_linear_closure) },
        { be_const_key_weak(get_user_function, 5), be_const_closure(get_user_function_closure) },
        { be_const_key_weak(is_value_provider, 114), be_const_closure(is_value_provider_closure) },
        { be_const_key_weak(version_string, -1), be_const_closure(animation_version_string_closure) },
        { be_const_key_weak(ELASTIC, -1), be_const_int(8) },
        { be_const_key_weak(shift_scroll_right, -1), be_const_closure(shift_scroll_right_closure) },
        { be_const_key_weak(color_provider, 71), be_const_class(be_class_ColorProvider) },
        { be_const_key_weak(register_user_function, 81), be_const_closure(register_user_function_closure) },
        { be_const_key_weak(SequenceManager, -1), be_const_class(be_class_SequenceManager) },
        { be_const_key_weak(is_user_function, -1), be_const_closure(is_user_function_closure) },
        { be_const_key_weak(animation, -1), be_const_class(be_class_Animation) },
        { be_const_key_weak(gradient_rainbow_radial, -1), be_const_closure(gradient_rainbow_radial_closure) },
        { be_const_key_weak(palette_wave_animation, 47), be_const_class(be_class_PaletteWaveAnimation) },
        { be_const_key_weak(LINEAR, 107), be_const_int(1) },
        { be_const_key_weak(event_handler, -1), be_const_class(be_class_EventHandler) },
        { be_const_key_weak(PALETTE_FIRE, 111), be_const_bytes_instance(000000004080000080FF0000C0FF8000FFFFFF00) },
        { be_const_key_weak(EventManager, 63), be_const_class(be_class_EventManager) },
        { be_const_key_weak(noise_rainbow, -1), be_const_closure(noise_rainbow_closure) },
        { be_const_key_weak(wave_animation, 94), be_const_class(be_class_WaveAnimation) },
        { be_const_key_weak(get_event_handlers, -1), be_const_closure(get_event_handlers_closure) },
        { be_const_key_weak(create_engine, -1), be_const_closure(create_engine_closure) },
        { be_const_key_weak(scale_grow, 80), be_const_closure(scale_grow_closure) },
        { be_const_key_weak(wave_custom, -1), be_const_closure(wave_custom_closure) },
        { be_const_key_weak(shift_animation, -1), be_const_class(be_class_ShiftAnimation) },
        { be_const_key_weak(bounce_basic, -1), be_const_closure(bounce_basic_closure) },
        { be_const_key_weak(sawtooth, -1), be_const_closure(sawtooth_closure) },
        { be_const_key_weak(PALETTE_OCEAN, -1), be_const_bytes_instance(00000080400000FF8000FFFFC000FF80FF008000) },
        { be_const_key_weak(static_value, -1), be_const_class(be_class_StaticValueProvider) },
        { be_const_key_weak(plasma_fast, 120), be_const_closure(plasma_fast_closure) },
        { be_const_key_weak(plasma_rainbow, -1), be_const_closure(plasma_rainbow_closure) },
        { be_const_key_weak(solid, 26), be_const_closure(solid_closure) },
        { be_const_key_weak(twinkle_solid, 20), be_const_closure(twinkle_solid_closure) },
        { be_const_key_weak(linear, -1), be_const_closure(linear_closure) },
        { be_const_key_weak(jitter_color, -1), be_const_closure(jitter_color_closure) },
        { be_const_key_weak(noise_animation, -1), be_const_class(be_class_NoiseAnimation) },
        { be_const_key_weak(jitter_all, -1), be_const_closure(jitter_all_closure) },
        { be_const_key_weak(unregister_event_handler, 9), be_const_closure(unregister_event_handler_closure) },
        { be_const_key_weak(palette_meter_animation, -1), be_const_class(be_class_PaletteMeterAnimation) },
        { be_const_key_weak(trigger_event, 35), be_const_closure(trigger_event_closure) },
        { be_const_key_weak(SAWTOOTH, -1), be_const_int(1) },
        { be_const_key_weak(frame_buffer, -1), be_const_class(be_class_FrameBuffer) },
        { be_const_key_weak(global, -1), be_const_closure(animation_global_closure) },
        { be_const_key_weak(bounce, 118), be_const_closure(bounce_closure) },
        { be_const_key_weak(list_user_functions, 70), be_const_closure(list_user_functions_closure) },
        { be_const_key_weak(PALETTE_SUNSET_TICKS, -1), be_const_bytes_instance(28FF450028FF8C0028FFD70028FF69B4288000802819197000000080) },
        { be_const_key_weak(EASE_IN, -1), be_const_int(6) },
        { be_const_key_weak(oscillator_value, -1), be_const_class(be_class_OscillatorValueProvider) },
        { be_const_key_weak(scale_static, -1), be_const_closure(scale_static_closure) },
        { be_const_key_weak(wave_rainbow_sine, -1), be_const_closure(wave_rainbow_sine_closure) },
        { be_const_key_weak(wave_single_sine, 10), be_const_closure(wave_single_sine_closure) },
        { be_const_key_weak(SINE, -1), be_const_int(5) },
        { be_const_key_weak(VERSION, -1), be_const_int(65536) },
        { be_const_key_weak(closure_value, -1), be_const_class(be_class_ClosureValueProvider) },
        { be_const_key_weak(scale_oscillate, 3), be_const_closure(scale_oscillate_closure) },
        { be_const_key_weak(rich_palette_rainbow, 95), be_const_closure(rich_palette_rainbow_closure) },
        { be_const_key_weak(get_registered_events, -1), be_const_closure(get_registered_events_closure) },
        { be_const_key_weak(noise_fractal, -1), be_const_closure(noise_fractal_closure) },
        { be_const_key_weak(fire_animation, -1), be_const_class(be_class_FireAnimation) },
        { be_const_key_weak(triangle, 32), be_const_closure(triangle_closure) },
        { be_const_key_weak(PALETTE_RAINBOW, -1), be_const_bytes_instance(00FF000024FFA50049FFFF006E00FF00920000FFB74B0082DBEE82EEFFFF0000) },
    }))
);
BE_EXPORT_VARIABLE be_define_const_native_module(animation);
/********************************************************************/
/********************************************************************/
/* End of solidification */
