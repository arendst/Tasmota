/* Solidification of animation.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'BreatheAnimation' ktab size: 18, total: 25 (saved 56 bytes)
static const bvalue be_ktab_class_BreatheAnimation[18] = {
  /* K0   */  be_nested_str_weak(on_param_changed),
  /* K1   */  be_nested_str_weak(base_color),
  /* K2   */  be_nested_str_weak(breathe_provider),
  /* K3   */  be_nested_str_weak(min_brightness),
  /* K4   */  be_nested_str_weak(max_brightness),
  /* K5   */  be_nested_str_weak(period),
  /* K6   */  be_nested_str_weak(duration),
  /* K7   */  be_nested_str_weak(curve_factor),
  /* K8   */  be_nested_str_weak(BreatheAnimation_X28base_color_X3D0x_X2508x_X2C_X20min_brightness_X3D_X25s_X2C_X20max_brightness_X3D_X25s_X2C_X20period_X3D_X25s_X2C_X20curve_factor_X3D_X25s_X2C_X20priority_X3D_X25s_X2C_X20running_X3D_X25s_X29),
  /* K9   */  be_nested_str_weak(priority),
  /* K10  */  be_nested_str_weak(is_running),
  /* K11  */  be_nested_str_weak(init),
  /* K12  */  be_nested_str_weak(animation),
  /* K13  */  be_nested_str_weak(breathe_color),
  /* K14  */  be_nested_str_weak(color),
  /* K15  */  be_nested_str_weak(start),
  /* K16  */  be_nested_str_weak(engine),
  /* K17  */  be_nested_str_weak(time_ms),
};


extern const bclass be_class_BreatheAnimation;

/********************************************************************
** Solidified function: on_param_changed
********************************************************************/
be_local_closure(class_BreatheAnimation_on_param_changed,   /* name */
  be_nested_proto(
    7,                          /* nstack */
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
    ( &(const binstruction[32]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0700,  //  0003  GETMET	R3	R3	K0
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x1C0C0301,  //  0007  EQ	R3	R1	K1
      0x780E0002,  //  0008  JMPF	R3	#000C
      0x880C0102,  //  0009  GETMBR	R3	R0	K2
      0x900E0202,  //  000A  SETMBR	R3	K1	R2
      0x70020012,  //  000B  JMP		#001F
      0x1C0C0303,  //  000C  EQ	R3	R1	K3
      0x780E0002,  //  000D  JMPF	R3	#0011
      0x880C0102,  //  000E  GETMBR	R3	R0	K2
      0x900E0602,  //  000F  SETMBR	R3	K3	R2
      0x7002000D,  //  0010  JMP		#001F
      0x1C0C0304,  //  0011  EQ	R3	R1	K4
      0x780E0002,  //  0012  JMPF	R3	#0016
      0x880C0102,  //  0013  GETMBR	R3	R0	K2
      0x900E0802,  //  0014  SETMBR	R3	K4	R2
      0x70020008,  //  0015  JMP		#001F
      0x1C0C0305,  //  0016  EQ	R3	R1	K5
      0x780E0002,  //  0017  JMPF	R3	#001B
      0x880C0102,  //  0018  GETMBR	R3	R0	K2
      0x900E0C02,  //  0019  SETMBR	R3	K6	R2
      0x70020003,  //  001A  JMP		#001F
      0x1C0C0307,  //  001B  EQ	R3	R1	K7
      0x780E0001,  //  001C  JMPF	R3	#001F
      0x880C0102,  //  001D  GETMBR	R3	R0	K2
      0x900E0E02,  //  001E  SETMBR	R3	K7	R2
      0x80000000,  //  001F  RET	0
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
      0x58080008,  //  0001  LDCONST	R2	K8
      0x880C0101,  //  0002  GETMBR	R3	R0	K1
      0x88100103,  //  0003  GETMBR	R4	R0	K3
      0x88140104,  //  0004  GETMBR	R5	R0	K4
      0x88180105,  //  0005  GETMBR	R6	R0	K5
      0x881C0107,  //  0006  GETMBR	R7	R0	K7
      0x88200109,  //  0007  GETMBR	R8	R0	K9
      0x8824010A,  //  0008  GETMBR	R9	R0	K10
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
      0x8C08050B,  //  0003  GETMET	R2	R2	K11
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0xB80A1800,  //  0006  GETNGBL	R2	K12
      0x8C08050D,  //  0007  GETMET	R2	R2	K13
      0x5C100200,  //  0008  MOVE	R4	R1
      0x7C080400,  //  0009  CALL	R2	2
      0x90020402,  //  000A  SETMBR	R0	K2	R2
      0x88080102,  //  000B  GETMBR	R2	R0	K2
      0x90021C02,  //  000C  SETMBR	R0	K14	R2
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
      0x8C08050F,  //  0003  GETMET	R2	R2	K15
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x4C080000,  //  0006  LDNIL	R2
      0x20080202,  //  0007  NE	R2	R1	R2
      0x780A0001,  //  0008  JMPF	R2	#000B
      0x5C080200,  //  0009  MOVE	R2	R1
      0x70020001,  //  000A  JMP		#000D
      0x88080110,  //  000B  GETMBR	R2	R0	K16
      0x88080511,  //  000C  GETMBR	R2	R2	K17
      0x880C0102,  //  000D  GETMBR	R3	R0	K2
      0x8C0C070F,  //  000E  GETMET	R3	R3	K15
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
        { be_const_key_weak(max_brightness, -1), be_const_bytes_instance(07000001FF0001FF00) },
        { be_const_key_weak(curve_factor, -1), be_const_bytes_instance(07000100050002) },
        { be_const_key_weak(min_brightness, -1), be_const_bytes_instance(07000001FF000000) },
        { be_const_key_weak(base_color, 0), be_const_bytes_instance(0400FF) },
        { be_const_key_weak(period, -1), be_const_bytes_instance(05006401B80B) },
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
    /* K1   */  be_nested_str_weak(_user_functions),
    /* K2   */  be_nested_str_weak(find),
    }),
    be_str_weak(get_user_function),
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
// compact class 'SequenceManager' ktab size: 40, total: 152 (saved 896 bytes)
static const bvalue be_ktab_class_SequenceManager[40] = {
  /* K0   */  be_nested_str_weak(steps),
  /* K1   */  be_nested_str_weak(push),
  /* K2   */  be_nested_str_weak(type),
  /* K3   */  be_nested_str_weak(play),
  /* K4   */  be_nested_str_weak(animation),
  /* K5   */  be_nested_str_weak(duration),
  /* K6   */  be_const_int(0),
  /* K7   */  be_nested_str_weak(step_index),
  /* K8   */  be_nested_str_weak(complete_iteration),
  /* K9   */  be_nested_str_weak(engine),
  /* K10  */  be_nested_str_weak(get_animations),
  /* K11  */  be_nested_str_weak(stop_iteration),
  /* K12  */  be_nested_str_weak(add),
  /* K13  */  be_nested_str_weak(start),
  /* K14  */  be_nested_str_weak(wait),
  /* K15  */  be_nested_str_weak(stop),
  /* K16  */  be_nested_str_weak(remove),
  /* K17  */  be_nested_str_weak(closure),
  /* K18  */  be_nested_str_weak(subsequence),
  /* K19  */  be_nested_str_weak(sequence_manager),
  /* K20  */  be_nested_str_weak(step_start_time),
  /* K21  */  be_nested_str_weak(is_running),
  /* K22  */  be_nested_str_weak(update),
  /* K23  */  be_nested_str_weak(advance_to_next_step),
  /* K24  */  be_nested_str_weak(execute_closure_steps_batch),
  /* K25  */  be_nested_str_weak(contains),
  /* K26  */  be_nested_str_weak(active_sequence),
  /* K27  */  be_nested_str_weak(sequence_state),
  /* K28  */  be_nested_str_weak(repeat_count),
  /* K29  */  be_const_int(1),
  /* K30  */  be_nested_str_weak(current_iteration),
  /* K31  */  be_nested_str_weak(is_repeat_sequence),
  /* K32  */  be_nested_str_weak(execute_closure_steps_batch_atomic),
  /* K33  */  be_nested_str_weak(execute_current_step),
  /* K34  */  be_nested_str_weak(pop_iteration_context),
  /* K35  */  be_nested_str_weak(stop_all_subsequences),
  /* K36  */  be_nested_str_weak(push_iteration_context),
  /* K37  */  be_nested_str_weak(function),
  /* K38  */  be_nested_str_weak(update_current_iteration),
  /* K39  */  be_nested_str_weak(get_resolved_repeat_count),
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
** Solidified function: execute_current_step
********************************************************************/
be_local_closure(class_SequenceManager_execute_current_step,   /* name */
  be_nested_proto(
    9,                          /* nstack */
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
    ( &(const binstruction[80]) {  /* code */
      0x88080107,  //  0000  GETMBR	R2	R0	K7
      0x600C000C,  //  0001  GETGBL	R3	G12
      0x88100100,  //  0002  GETMBR	R4	R0	K0
      0x7C0C0200,  //  0003  CALL	R3	1
      0x28080403,  //  0004  GE	R2	R2	R3
      0x780A0003,  //  0005  JMPF	R2	#000A
      0x8C080108,  //  0006  GETMET	R2	R0	K8
      0x5C100200,  //  0007  MOVE	R4	R1
      0x7C080400,  //  0008  CALL	R2	2
      0x80000400,  //  0009  RET	0
      0x88080100,  //  000A  GETMBR	R2	R0	K0
      0x880C0107,  //  000B  GETMBR	R3	R0	K7
      0x94080403,  //  000C  GETIDX	R2	R2	R3
      0x940C0502,  //  000D  GETIDX	R3	R2	K2
      0x1C0C0703,  //  000E  EQ	R3	R3	K3
      0x780E001E,  //  000F  JMPF	R3	#002F
      0x940C0504,  //  0010  GETIDX	R3	R2	K4
      0x88100109,  //  0011  GETMBR	R4	R0	K9
      0x8C10090A,  //  0012  GETMET	R4	R4	K10
      0x7C100200,  //  0013  CALL	R4	1
      0x50140000,  //  0014  LDBOOL	R5	0	0
      0x60180010,  //  0015  GETGBL	R6	G16
      0x5C1C0800,  //  0016  MOVE	R7	R4
      0x7C180200,  //  0017  CALL	R6	1
      0xA8020008,  //  0018  EXBLK	0	#0022
      0x5C1C0C00,  //  0019  MOVE	R7	R6
      0x7C1C0000,  //  001A  CALL	R7	0
      0x1C200E03,  //  001B  EQ	R8	R7	R3
      0x78220001,  //  001C  JMPF	R8	#001F
      0x50140200,  //  001D  LDBOOL	R5	1	0
      0x70020000,  //  001E  JMP		#0020
      0x7001FFF8,  //  001F  JMP		#0019
      0xA8040001,  //  0020  EXBLK	1	1
      0x70020002,  //  0021  JMP		#0025
      0x5818000B,  //  0022  LDCONST	R6	K11
      0xAC180200,  //  0023  CATCH	R6	1	0
      0xB0080000,  //  0024  RAISE	2	R0	R0
      0x5C180A00,  //  0025  MOVE	R6	R5
      0x741A0003,  //  0026  JMPT	R6	#002B
      0x88180109,  //  0027  GETMBR	R6	R0	K9
      0x8C180D0C,  //  0028  GETMET	R6	R6	K12
      0x5C200600,  //  0029  MOVE	R8	R3
      0x7C180400,  //  002A  CALL	R6	2
      0x8C18070D,  //  002B  GETMET	R6	R3	K13
      0x5C200200,  //  002C  MOVE	R8	R1
      0x7C180400,  //  002D  CALL	R6	2
      0x7002001E,  //  002E  JMP		#004E
      0x940C0502,  //  002F  GETIDX	R3	R2	K2
      0x1C0C070E,  //  0030  EQ	R3	R3	K14
      0x780E0000,  //  0031  JMPF	R3	#0033
      0x7002001A,  //  0032  JMP		#004E
      0x940C0502,  //  0033  GETIDX	R3	R2	K2
      0x1C0C070F,  //  0034  EQ	R3	R3	K15
      0x780E0005,  //  0035  JMPF	R3	#003C
      0x940C0504,  //  0036  GETIDX	R3	R2	K4
      0x88100109,  //  0037  GETMBR	R4	R0	K9
      0x8C100910,  //  0038  GETMET	R4	R4	K16
      0x5C180600,  //  0039  MOVE	R6	R3
      0x7C100400,  //  003A  CALL	R4	2
      0x70020011,  //  003B  JMP		#004E
      0x940C0502,  //  003C  GETIDX	R3	R2	K2
      0x1C0C0711,  //  003D  EQ	R3	R3	K17
      0x780E0007,  //  003E  JMPF	R3	#0047
      0x940C0511,  //  003F  GETIDX	R3	R2	K17
      0x4C100000,  //  0040  LDNIL	R4
      0x20100604,  //  0041  NE	R4	R3	R4
      0x78120002,  //  0042  JMPF	R4	#0046
      0x5C100600,  //  0043  MOVE	R4	R3
      0x88140109,  //  0044  GETMBR	R5	R0	K9
      0x7C100200,  //  0045  CALL	R4	1
      0x70020006,  //  0046  JMP		#004E
      0x940C0502,  //  0047  GETIDX	R3	R2	K2
      0x1C0C0712,  //  0048  EQ	R3	R3	K18
      0x780E0003,  //  0049  JMPF	R3	#004E
      0x940C0513,  //  004A  GETIDX	R3	R2	K19
      0x8C10070D,  //  004B  GETMET	R4	R3	K13
      0x5C180200,  //  004C  MOVE	R6	R1
      0x7C100400,  //  004D  CALL	R4	2
      0x90022801,  //  004E  SETMBR	R0	K20	R1
      0x80000000,  //  004F  RET	0
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
      0x88080115,  //  0000  GETMBR	R2	R0	K21
      0x780A0004,  //  0001  JMPF	R2	#0007
      0x6008000C,  //  0002  GETGBL	R2	G12
      0x880C0100,  //  0003  GETMBR	R3	R0	K0
      0x7C080200,  //  0004  CALL	R2	1
      0x1C080506,  //  0005  EQ	R2	R2	K6
      0x780A0001,  //  0006  JMPF	R2	#0009
      0x50080000,  //  0007  LDBOOL	R2	0	0
      0x80040400,  //  0008  RET	1	R2
      0x88080100,  //  0009  GETMBR	R2	R0	K0
      0x880C0107,  //  000A  GETMBR	R3	R0	K7
      0x94080403,  //  000B  GETIDX	R2	R2	R3
      0x940C0502,  //  000C  GETIDX	R3	R2	K2
      0x1C0C0712,  //  000D  EQ	R3	R3	K18
      0x780E0008,  //  000E  JMPF	R3	#0018
      0x940C0513,  //  000F  GETIDX	R3	R2	K19
      0x8C100716,  //  0010  GETMET	R4	R3	K22
      0x5C180200,  //  0011  MOVE	R6	R1
      0x7C100400,  //  0012  CALL	R4	2
      0x74120002,  //  0013  JMPT	R4	#0017
      0x8C100117,  //  0014  GETMET	R4	R0	K23
      0x5C180200,  //  0015  MOVE	R6	R1
      0x7C100400,  //  0016  CALL	R4	2
      0x70020019,  //  0017  JMP		#0032
      0x940C0502,  //  0018  GETIDX	R3	R2	K2
      0x1C0C0711,  //  0019  EQ	R3	R3	K17
      0x780E0003,  //  001A  JMPF	R3	#001F
      0x8C0C0118,  //  001B  GETMET	R3	R0	K24
      0x5C140200,  //  001C  MOVE	R5	R1
      0x7C0C0400,  //  001D  CALL	R3	2
      0x70020012,  //  001E  JMP		#0032
      0x8C0C0519,  //  001F  GETMET	R3	R2	K25
      0x58140005,  //  0020  LDCONST	R5	K5
      0x7C0C0400,  //  0021  CALL	R3	2
      0x780E000B,  //  0022  JMPF	R3	#002F
      0x940C0505,  //  0023  GETIDX	R3	R2	K5
      0x240C0706,  //  0024  GT	R3	R3	K6
      0x780E0008,  //  0025  JMPF	R3	#002F
      0x880C0114,  //  0026  GETMBR	R3	R0	K20
      0x040C0203,  //  0027  SUB	R3	R1	R3
      0x94100505,  //  0028  GETIDX	R4	R2	K5
      0x28100604,  //  0029  GE	R4	R3	R4
      0x78120002,  //  002A  JMPF	R4	#002E
      0x8C100117,  //  002B  GETMET	R4	R0	K23
      0x5C180200,  //  002C  MOVE	R6	R1
      0x7C100400,  //  002D  CALL	R4	2
      0x70020002,  //  002E  JMP		#0032
      0x8C0C0117,  //  002F  GETMET	R3	R0	K23
      0x5C140200,  //  0030  MOVE	R5	R1
      0x7C0C0400,  //  0031  CALL	R3	2
      0x880C0115,  //  0032  GETMBR	R3	R0	K21
      0x80040600,  //  0033  RET	1	R3
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
      0x90021201,  //  0000  SETMBR	R0	K9	R1
      0x4C0C0000,  //  0001  LDNIL	R3
      0x90023403,  //  0002  SETMBR	R0	K26	R3
      0x600C0013,  //  0003  GETGBL	R3	G19
      0x7C0C0000,  //  0004  CALL	R3	0
      0x90023603,  //  0005  SETMBR	R0	K27	R3
      0x90020F06,  //  0006  SETMBR	R0	K7	K6
      0x90022906,  //  0007  SETMBR	R0	K20	K6
      0x600C0012,  //  0008  GETGBL	R3	G18
      0x7C0C0000,  //  0009  CALL	R3	0
      0x90020003,  //  000A  SETMBR	R0	K0	R3
      0x500C0000,  //  000B  LDBOOL	R3	0	0
      0x90022A03,  //  000C  SETMBR	R0	K21	R3
      0x4C0C0000,  //  000D  LDNIL	R3
      0x200C0403,  //  000E  NE	R3	R2	R3
      0x780E0001,  //  000F  JMPF	R3	#0012
      0x5C0C0400,  //  0010  MOVE	R3	R2
      0x70020000,  //  0011  JMP		#0013
      0x580C001D,  //  0012  LDCONST	R3	K29
      0x90023803,  //  0013  SETMBR	R0	K28	R3
      0x90023D06,  //  0014  SETMBR	R0	K30	K6
      0x4C0C0000,  //  0015  LDNIL	R3
      0x200C0403,  //  0016  NE	R3	R2	R3
      0x780E0001,  //  0017  JMPF	R3	#001A
      0x200C051D,  //  0018  NE	R3	R2	K29
      0x740E0000,  //  0019  JMPT	R3	#001B
      0x500C0001,  //  001A  LDBOOL	R3	0	1
      0x500C0200,  //  001B  LDBOOL	R3	1	0
      0x90023E03,  //  001C  SETMBR	R0	K31	R3
      0x80000000,  //  001D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: advance_to_next_step
********************************************************************/
be_local_closure(class_SequenceManager_advance_to_next_step,   /* name */
  be_nested_proto(
    8,                          /* nstack */
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
    ( &(const binstruction[37]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x880C0107,  //  0001  GETMBR	R3	R0	K7
      0x94080403,  //  0002  GETIDX	R2	R2	R3
      0x4C0C0000,  //  0003  LDNIL	R3
      0x94100502,  //  0004  GETIDX	R4	R2	K2
      0x1C100903,  //  0005  EQ	R4	R4	K3
      0x78120004,  //  0006  JMPF	R4	#000C
      0x8C100519,  //  0007  GETMET	R4	R2	K25
      0x58180005,  //  0008  LDCONST	R6	K5
      0x7C100400,  //  0009  CALL	R4	2
      0x78120000,  //  000A  JMPF	R4	#000C
      0x940C0504,  //  000B  GETIDX	R3	R2	K4
      0x88100107,  //  000C  GETMBR	R4	R0	K7
      0x0010091D,  //  000D  ADD	R4	R4	K29
      0x90020E04,  //  000E  SETMBR	R0	K7	R4
      0x88100107,  //  000F  GETMBR	R4	R0	K7
      0x6014000C,  //  0010  GETGBL	R5	G12
      0x88180100,  //  0011  GETMBR	R6	R0	K0
      0x7C140200,  //  0012  CALL	R5	1
      0x28100805,  //  0013  GE	R4	R4	R5
      0x7812000A,  //  0014  JMPF	R4	#0020
      0x4C100000,  //  0015  LDNIL	R4
      0x20100604,  //  0016  NE	R4	R3	R4
      0x78120003,  //  0017  JMPF	R4	#001C
      0x88100109,  //  0018  GETMBR	R4	R0	K9
      0x8C100910,  //  0019  GETMET	R4	R4	K16
      0x5C180600,  //  001A  MOVE	R6	R3
      0x7C100400,  //  001B  CALL	R4	2
      0x8C100108,  //  001C  GETMET	R4	R0	K8
      0x5C180200,  //  001D  MOVE	R6	R1
      0x7C100400,  //  001E  CALL	R4	2
      0x70020003,  //  001F  JMP		#0024
      0x8C100120,  //  0020  GETMET	R4	R0	K32
      0x5C180200,  //  0021  MOVE	R6	R1
      0x5C1C0600,  //  0022  MOVE	R7	R3
      0x7C100600,  //  0023  CALL	R4	3
      0x80000000,  //  0024  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: execute_closure_steps_batch
********************************************************************/
be_local_closure(class_SequenceManager_execute_closure_steps_batch,   /* name */
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
    be_str_weak(execute_closure_steps_batch),
    &be_const_str_solidified,
    ( &(const binstruction[39]) {  /* code */
      0x88080107,  //  0000  GETMBR	R2	R0	K7
      0x600C000C,  //  0001  GETGBL	R3	G12
      0x88100100,  //  0002  GETMBR	R4	R0	K0
      0x7C0C0200,  //  0003  CALL	R3	1
      0x14080403,  //  0004  LT	R2	R2	R3
      0x780A0012,  //  0005  JMPF	R2	#0019
      0x88080100,  //  0006  GETMBR	R2	R0	K0
      0x880C0107,  //  0007  GETMBR	R3	R0	K7
      0x94080403,  //  0008  GETIDX	R2	R2	R3
      0x940C0502,  //  0009  GETIDX	R3	R2	K2
      0x1C0C0711,  //  000A  EQ	R3	R3	K17
      0x780E000A,  //  000B  JMPF	R3	#0017
      0x940C0511,  //  000C  GETIDX	R3	R2	K17
      0x4C100000,  //  000D  LDNIL	R4
      0x20100604,  //  000E  NE	R4	R3	R4
      0x78120002,  //  000F  JMPF	R4	#0013
      0x5C100600,  //  0010  MOVE	R4	R3
      0x88140109,  //  0011  GETMBR	R5	R0	K9
      0x7C100200,  //  0012  CALL	R4	1
      0x88100107,  //  0013  GETMBR	R4	R0	K7
      0x0010091D,  //  0014  ADD	R4	R4	K29
      0x90020E04,  //  0015  SETMBR	R0	K7	R4
      0x70020000,  //  0016  JMP		#0018
      0x70020000,  //  0017  JMP		#0019
      0x7001FFE6,  //  0018  JMP		#0000
      0x88080107,  //  0019  GETMBR	R2	R0	K7
      0x600C000C,  //  001A  GETGBL	R3	G12
      0x88100100,  //  001B  GETMBR	R4	R0	K0
      0x7C0C0200,  //  001C  CALL	R3	1
      0x14080403,  //  001D  LT	R2	R2	R3
      0x780A0003,  //  001E  JMPF	R2	#0023
      0x8C080121,  //  001F  GETMET	R2	R0	K33
      0x5C100200,  //  0020  MOVE	R4	R1
      0x7C080400,  //  0021  CALL	R2	2
      0x70020002,  //  0022  JMP		#0026
      0x8C080108,  //  0023  GETMET	R2	R0	K8
      0x5C100200,  //  0024  MOVE	R4	R1
      0x7C080400,  //  0025  CALL	R2	2
      0x80000000,  //  0026  RET	0
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
    ( &(const binstruction[36]) {  /* code */
      0x88040115,  //  0000  GETMBR	R1	R0	K21
      0x78060020,  //  0001  JMPF	R1	#0023
      0x50040000,  //  0002  LDBOOL	R1	0	0
      0x90022A01,  //  0003  SETMBR	R0	K21	R1
      0x8804011F,  //  0004  GETMBR	R1	R0	K31
      0x78060002,  //  0005  JMPF	R1	#0009
      0x88040109,  //  0006  GETMBR	R1	R0	K9
      0x8C040322,  //  0007  GETMET	R1	R1	K34
      0x7C040200,  //  0008  CALL	R1	1
      0x88040107,  //  0009  GETMBR	R1	R0	K7
      0x6008000C,  //  000A  GETGBL	R2	G12
      0x880C0100,  //  000B  GETMBR	R3	R0	K0
      0x7C080200,  //  000C  CALL	R2	1
      0x14040202,  //  000D  LT	R1	R1	R2
      0x78060011,  //  000E  JMPF	R1	#0021
      0x88040100,  //  000F  GETMBR	R1	R0	K0
      0x88080107,  //  0010  GETMBR	R2	R0	K7
      0x94040202,  //  0011  GETIDX	R1	R1	R2
      0x94080302,  //  0012  GETIDX	R2	R1	K2
      0x1C080503,  //  0013  EQ	R2	R2	K3
      0x780A0005,  //  0014  JMPF	R2	#001B
      0x94080304,  //  0015  GETIDX	R2	R1	K4
      0x880C0109,  //  0016  GETMBR	R3	R0	K9
      0x8C0C0710,  //  0017  GETMET	R3	R3	K16
      0x5C140400,  //  0018  MOVE	R5	R2
      0x7C0C0400,  //  0019  CALL	R3	2
      0x70020005,  //  001A  JMP		#0021
      0x94080302,  //  001B  GETIDX	R2	R1	K2
      0x1C080512,  //  001C  EQ	R2	R2	K18
      0x780A0002,  //  001D  JMPF	R2	#0021
      0x94080313,  //  001E  GETIDX	R2	R1	K19
      0x8C0C050F,  //  001F  GETMET	R3	R2	K15
      0x7C0C0200,  //  0020  CALL	R3	1
      0x8C040123,  //  0021  GETMET	R1	R0	K35
      0x7C040200,  //  0022  CALL	R1	1
      0x80040000,  //  0023  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: execute_closure_steps_batch_atomic
********************************************************************/
be_local_closure(class_SequenceManager_execute_closure_steps_batch_atomic,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SequenceManager,     /* shared constants */
    be_str_weak(execute_closure_steps_batch_atomic),
    &be_const_str_solidified,
    ( &(const binstruction[77]) {  /* code */
      0x880C0107,  //  0000  GETMBR	R3	R0	K7
      0x6010000C,  //  0001  GETGBL	R4	G12
      0x88140100,  //  0002  GETMBR	R5	R0	K0
      0x7C100200,  //  0003  CALL	R4	1
      0x140C0604,  //  0004  LT	R3	R3	R4
      0x780E0012,  //  0005  JMPF	R3	#0019
      0x880C0100,  //  0006  GETMBR	R3	R0	K0
      0x88100107,  //  0007  GETMBR	R4	R0	K7
      0x940C0604,  //  0008  GETIDX	R3	R3	R4
      0x94100702,  //  0009  GETIDX	R4	R3	K2
      0x1C100911,  //  000A  EQ	R4	R4	K17
      0x7812000A,  //  000B  JMPF	R4	#0017
      0x94100711,  //  000C  GETIDX	R4	R3	K17
      0x4C140000,  //  000D  LDNIL	R5
      0x20140805,  //  000E  NE	R5	R4	R5
      0x78160002,  //  000F  JMPF	R5	#0013
      0x5C140800,  //  0010  MOVE	R5	R4
      0x88180109,  //  0011  GETMBR	R6	R0	K9
      0x7C140200,  //  0012  CALL	R5	1
      0x88140107,  //  0013  GETMBR	R5	R0	K7
      0x00140B1D,  //  0014  ADD	R5	R5	K29
      0x90020E05,  //  0015  SETMBR	R0	K7	R5
      0x70020000,  //  0016  JMP		#0018
      0x70020000,  //  0017  JMP		#0019
      0x7001FFE6,  //  0018  JMP		#0000
      0x4C0C0000,  //  0019  LDNIL	R3
      0x50100000,  //  001A  LDBOOL	R4	0	0
      0x88140107,  //  001B  GETMBR	R5	R0	K7
      0x6018000C,  //  001C  GETGBL	R6	G12
      0x881C0100,  //  001D  GETMBR	R7	R0	K0
      0x7C180200,  //  001E  CALL	R6	1
      0x14140A06,  //  001F  LT	R5	R5	R6
      0x7816000B,  //  0020  JMPF	R5	#002D
      0x88140100,  //  0021  GETMBR	R5	R0	K0
      0x88180107,  //  0022  GETMBR	R6	R0	K7
      0x940C0A06,  //  0023  GETIDX	R3	R5	R6
      0x94180702,  //  0024  GETIDX	R6	R3	K2
      0x1C180D03,  //  0025  EQ	R6	R6	K3
      0x781A0005,  //  0026  JMPF	R6	#002D
      0x4C180000,  //  0027  LDNIL	R6
      0x20180406,  //  0028  NE	R6	R2	R6
      0x781A0002,  //  0029  JMPF	R6	#002D
      0x94180704,  //  002A  GETIDX	R6	R3	K4
      0x1C180C02,  //  002B  EQ	R6	R6	R2
      0x5C100C00,  //  002C  MOVE	R4	R6
      0x78120004,  //  002D  JMPF	R4	#0033
      0x90022801,  //  002E  SETMBR	R0	K20	R1
      0x8C14050D,  //  002F  GETMET	R5	R2	K13
      0x5C1C0200,  //  0030  MOVE	R7	R1
      0x7C140400,  //  0031  CALL	R5	2
      0x7002000F,  //  0032  JMP		#0043
      0x88140107,  //  0033  GETMBR	R5	R0	K7
      0x6018000C,  //  0034  GETGBL	R6	G12
      0x881C0100,  //  0035  GETMBR	R7	R0	K0
      0x7C180200,  //  0036  CALL	R6	1
      0x14140A06,  //  0037  LT	R5	R5	R6
      0x78160002,  //  0038  JMPF	R5	#003C
      0x8C140121,  //  0039  GETMET	R5	R0	K33
      0x5C1C0200,  //  003A  MOVE	R7	R1
      0x7C140400,  //  003B  CALL	R5	2
      0x4C140000,  //  003C  LDNIL	R5
      0x20140405,  //  003D  NE	R5	R2	R5
      0x78160003,  //  003E  JMPF	R5	#0043
      0x88140109,  //  003F  GETMBR	R5	R0	K9
      0x8C140B10,  //  0040  GETMET	R5	R5	K16
      0x5C1C0400,  //  0041  MOVE	R7	R2
      0x7C140400,  //  0042  CALL	R5	2
      0x88140107,  //  0043  GETMBR	R5	R0	K7
      0x6018000C,  //  0044  GETGBL	R6	G12
      0x881C0100,  //  0045  GETMBR	R7	R0	K0
      0x7C180200,  //  0046  CALL	R6	1
      0x28140A06,  //  0047  GE	R5	R5	R6
      0x78160002,  //  0048  JMPF	R5	#004C
      0x8C140108,  //  0049  GETMET	R5	R0	K8
      0x5C1C0200,  //  004A  MOVE	R7	R1
      0x7C140400,  //  004B  CALL	R5	2
      0x80000000,  //  004C  RET	0
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
      0x9812050E,  //  0004  SETIDX	R4	K2	K14
      0x98120A01,  //  0005  SETIDX	R4	K5	R1
      0x7C080400,  //  0006  CALL	R2	2
      0x80040000,  //  0007  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: push_closure_step
********************************************************************/
be_local_closure(class_SequenceManager_push_closure_step,   /* name */
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
    be_str_weak(push_closure_step),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x60100013,  //  0002  GETGBL	R4	G19
      0x7C100000,  //  0003  CALL	R4	0
      0x98120511,  //  0004  SETIDX	R4	K2	K17
      0x98122201,  //  0005  SETIDX	R4	K17	R1
      0x7C080400,  //  0006  CALL	R2	2
      0x80040000,  //  0007  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(class_SequenceManager_start,   /* name */
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
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[57]) {  /* code */
      0x88080115,  //  0000  GETMBR	R2	R0	K21
      0x780A0003,  //  0001  JMPF	R2	#0006
      0x50080000,  //  0002  LDBOOL	R2	0	0
      0x90022A02,  //  0003  SETMBR	R0	K21	R2
      0x8C080123,  //  0004  GETMET	R2	R0	K35
      0x7C080200,  //  0005  CALL	R2	1
      0x90020F06,  //  0006  SETMBR	R0	K7	K6
      0x90022801,  //  0007  SETMBR	R0	K20	R1
      0x90023D06,  //  0008  SETMBR	R0	K30	K6
      0x50080200,  //  0009  LDBOOL	R2	1	0
      0x90022A02,  //  000A  SETMBR	R0	K21	R2
      0x8808011F,  //  000B  GETMBR	R2	R0	K31
      0x780A0003,  //  000C  JMPF	R2	#0011
      0x88080109,  //  000D  GETMBR	R2	R0	K9
      0x8C080524,  //  000E  GETMET	R2	R2	K36
      0x8810011E,  //  000F  GETMBR	R4	R0	K30
      0x7C080400,  //  0010  CALL	R2	2
      0x6008000C,  //  0011  GETGBL	R2	G12
      0x880C0100,  //  0012  GETMBR	R3	R0	K0
      0x7C080200,  //  0013  CALL	R2	1
      0x24080506,  //  0014  GT	R2	R2	K6
      0x780A0021,  //  0015  JMPF	R2	#0038
      0x88080107,  //  0016  GETMBR	R2	R0	K7
      0x600C000C,  //  0017  GETGBL	R3	G12
      0x88100100,  //  0018  GETMBR	R4	R0	K0
      0x7C0C0200,  //  0019  CALL	R3	1
      0x14080403,  //  001A  LT	R2	R2	R3
      0x780A0012,  //  001B  JMPF	R2	#002F
      0x88080100,  //  001C  GETMBR	R2	R0	K0
      0x880C0107,  //  001D  GETMBR	R3	R0	K7
      0x94080403,  //  001E  GETIDX	R2	R2	R3
      0x940C0502,  //  001F  GETIDX	R3	R2	K2
      0x1C0C0711,  //  0020  EQ	R3	R3	K17
      0x780E000A,  //  0021  JMPF	R3	#002D
      0x940C0511,  //  0022  GETIDX	R3	R2	K17
      0x4C100000,  //  0023  LDNIL	R4
      0x20100604,  //  0024  NE	R4	R3	R4
      0x78120002,  //  0025  JMPF	R4	#0029
      0x5C100600,  //  0026  MOVE	R4	R3
      0x88140109,  //  0027  GETMBR	R5	R0	K9
      0x7C100200,  //  0028  CALL	R4	1
      0x88100107,  //  0029  GETMBR	R4	R0	K7
      0x0010091D,  //  002A  ADD	R4	R4	K29
      0x90020E04,  //  002B  SETMBR	R0	K7	R4
      0x70020000,  //  002C  JMP		#002E
      0x70020000,  //  002D  JMP		#002F
      0x7001FFE6,  //  002E  JMP		#0016
      0x88080107,  //  002F  GETMBR	R2	R0	K7
      0x600C000C,  //  0030  GETGBL	R3	G12
      0x88100100,  //  0031  GETMBR	R4	R0	K0
      0x7C0C0200,  //  0032  CALL	R3	1
      0x14080403,  //  0033  LT	R2	R2	R3
      0x780A0002,  //  0034  JMPF	R2	#0038
      0x8C080121,  //  0035  GETMET	R2	R0	K33
      0x5C100200,  //  0036  MOVE	R4	R1
      0x7C080400,  //  0037  CALL	R2	2
      0x80040000,  //  0038  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_resolved_repeat_count
********************************************************************/
be_local_closure(class_SequenceManager_get_resolved_repeat_count,   /* name */
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
    be_str_weak(get_resolved_repeat_count),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x60040004,  //  0000  GETGBL	R1	G4
      0x8808011C,  //  0001  GETMBR	R2	R0	K28
      0x7C040200,  //  0002  CALL	R1	1
      0x1C040325,  //  0003  EQ	R1	R1	K37
      0x78060004,  //  0004  JMPF	R1	#000A
      0x8C04011C,  //  0005  GETMET	R1	R0	K28
      0x880C0109,  //  0006  GETMBR	R3	R0	K9
      0x7C040400,  //  0007  CALL	R1	2
      0x80040200,  //  0008  RET	1	R1
      0x70020001,  //  0009  JMP		#000C
      0x8804011C,  //  000A  GETMBR	R1	R0	K28
      0x80040200,  //  000B  RET	1	R1
      0x80000000,  //  000C  RET	0
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
      0x1C0C0712,  //  0007  EQ	R3	R3	K18
      0x780E0002,  //  0008  JMPF	R3	#000C
      0x940C0513,  //  0009  GETIDX	R3	R2	K19
      0x8C10070F,  //  000A  GETMET	R4	R3	K15
      0x7C100200,  //  000B  CALL	R4	1
      0x7001FFF6,  //  000C  JMP		#0004
      0x5804000B,  //  000D  LDCONST	R1	K11
      0xAC040200,  //  000E  CATCH	R1	1	0
      0xB0080000,  //  000F  RAISE	2	R0	R0
      0x80040000,  //  0010  RET	1	R0
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
      0x98120512,  //  0004  SETIDX	R4	K2	K18
      0x98122601,  //  0005  SETIDX	R4	K19	R1
      0x7C080400,  //  0006  CALL	R2	2
      0x80040000,  //  0007  RET	1	R0
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
      0x88040115,  //  0000  GETMBR	R1	R0	K21
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
    7,                          /* nstack */
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
    ( &(const binstruction[61]) {  /* code */
      0x8808011E,  //  0000  GETMBR	R2	R0	K30
      0x0008051D,  //  0001  ADD	R2	R2	K29
      0x90023C02,  //  0002  SETMBR	R0	K30	R2
      0x8808011F,  //  0003  GETMBR	R2	R0	K31
      0x780A0003,  //  0004  JMPF	R2	#0009
      0x88080109,  //  0005  GETMBR	R2	R0	K9
      0x8C080526,  //  0006  GETMET	R2	R2	K38
      0x8810011E,  //  0007  GETMBR	R4	R0	K30
      0x7C080400,  //  0008  CALL	R2	2
      0x8C080127,  //  0009  GETMET	R2	R0	K39
      0x7C080200,  //  000A  CALL	R2	1
      0x540DFFFE,  //  000B  LDINT	R3	-1
      0x1C0C0403,  //  000C  EQ	R3	R2	R3
      0x740E0002,  //  000D  JMPT	R3	#0011
      0x880C011E,  //  000E  GETMBR	R3	R0	K30
      0x140C0602,  //  000F  LT	R3	R3	R2
      0x780E0023,  //  0010  JMPF	R3	#0035
      0x90020F06,  //  0011  SETMBR	R0	K7	K6
      0x880C0107,  //  0012  GETMBR	R3	R0	K7
      0x6010000C,  //  0013  GETGBL	R4	G12
      0x88140100,  //  0014  GETMBR	R5	R0	K0
      0x7C100200,  //  0015  CALL	R4	1
      0x140C0604,  //  0016  LT	R3	R3	R4
      0x780E0012,  //  0017  JMPF	R3	#002B
      0x880C0100,  //  0018  GETMBR	R3	R0	K0
      0x88100107,  //  0019  GETMBR	R4	R0	K7
      0x940C0604,  //  001A  GETIDX	R3	R3	R4
      0x94100702,  //  001B  GETIDX	R4	R3	K2
      0x1C100911,  //  001C  EQ	R4	R4	K17
      0x7812000A,  //  001D  JMPF	R4	#0029
      0x94100711,  //  001E  GETIDX	R4	R3	K17
      0x4C140000,  //  001F  LDNIL	R5
      0x20140805,  //  0020  NE	R5	R4	R5
      0x78160002,  //  0021  JMPF	R5	#0025
      0x5C140800,  //  0022  MOVE	R5	R4
      0x88180109,  //  0023  GETMBR	R6	R0	K9
      0x7C140200,  //  0024  CALL	R5	1
      0x88140107,  //  0025  GETMBR	R5	R0	K7
      0x00140B1D,  //  0026  ADD	R5	R5	K29
      0x90020E05,  //  0027  SETMBR	R0	K7	R5
      0x70020000,  //  0028  JMP		#002A
      0x70020000,  //  0029  JMP		#002B
      0x7001FFE6,  //  002A  JMP		#0012
      0x880C0107,  //  002B  GETMBR	R3	R0	K7
      0x6010000C,  //  002C  GETGBL	R4	G12
      0x88140100,  //  002D  GETMBR	R5	R0	K0
      0x7C100200,  //  002E  CALL	R4	1
      0x140C0604,  //  002F  LT	R3	R3	R4
      0x780E0002,  //  0030  JMPF	R3	#0034
      0x8C0C0121,  //  0031  GETMET	R3	R0	K33
      0x5C140200,  //  0032  MOVE	R5	R1
      0x7C0C0400,  //  0033  CALL	R3	2
      0x70020006,  //  0034  JMP		#003C
      0x500C0000,  //  0035  LDBOOL	R3	0	0
      0x90022A03,  //  0036  SETMBR	R0	K21	R3
      0x880C011F,  //  0037  GETMBR	R3	R0	K31
      0x780E0002,  //  0038  JMPF	R3	#003C
      0x880C0109,  //  0039  GETMBR	R3	R0	K9
      0x8C0C0722,  //  003A  GETMET	R3	R3	K34
      0x7C0C0200,  //  003B  CALL	R3	1
      0x80000000,  //  003C  RET	0
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
    be_nested_map(27,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(active_sequence, 26), be_const_var(1) },
        { be_const_key_weak(step_start_time, -1), be_const_var(4) },
        { be_const_key_weak(repeat_count, -1), be_const_var(7) },
        { be_const_key_weak(is_repeat_sequence, -1), be_const_var(9) },
        { be_const_key_weak(push_step, 12), be_const_closure(class_SequenceManager_push_step_closure) },
        { be_const_key_weak(steps, -1), be_const_var(5) },
        { be_const_key_weak(execute_current_step, -1), be_const_closure(class_SequenceManager_execute_current_step_closure) },
        { be_const_key_weak(update, -1), be_const_closure(class_SequenceManager_update_closure) },
        { be_const_key_weak(complete_iteration, 9), be_const_closure(class_SequenceManager_complete_iteration_closure) },
        { be_const_key_weak(is_sequence_running, -1), be_const_closure(class_SequenceManager_is_sequence_running_closure) },
        { be_const_key_weak(execute_closure_steps_batch, -1), be_const_closure(class_SequenceManager_execute_closure_steps_batch_closure) },
        { be_const_key_weak(stop, -1), be_const_closure(class_SequenceManager_stop_closure) },
        { be_const_key_weak(push_wait_step, -1), be_const_closure(class_SequenceManager_push_wait_step_closure) },
        { be_const_key_weak(is_running, -1), be_const_var(6) },
        { be_const_key_weak(execute_closure_steps_batch_atomic, -1), be_const_closure(class_SequenceManager_execute_closure_steps_batch_atomic_closure) },
        { be_const_key_weak(push_closure_step, -1), be_const_closure(class_SequenceManager_push_closure_step_closure) },
        { be_const_key_weak(step_index, 25), be_const_var(3) },
        { be_const_key_weak(engine, 15), be_const_var(0) },
        { be_const_key_weak(advance_to_next_step, 16), be_const_closure(class_SequenceManager_advance_to_next_step_closure) },
        { be_const_key_weak(current_iteration, -1), be_const_var(8) },
        { be_const_key_weak(start, -1), be_const_closure(class_SequenceManager_start_closure) },
        { be_const_key_weak(get_resolved_repeat_count, -1), be_const_closure(class_SequenceManager_get_resolved_repeat_count_closure) },
        { be_const_key_weak(stop_all_subsequences, -1), be_const_closure(class_SequenceManager_stop_all_subsequences_closure) },
        { be_const_key_weak(push_repeat_subsequence, 3), be_const_closure(class_SequenceManager_push_repeat_subsequence_closure) },
        { be_const_key_weak(init, 8), be_const_closure(class_SequenceManager_init_closure) },
        { be_const_key_weak(sequence_state, -1), be_const_var(2) },
        { be_const_key_weak(push_play_step, -1), be_const_closure(class_SequenceManager_push_play_step_closure) },
    })),
    be_str_weak(SequenceManager)
);

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

// compact class 'CometAnimation' ktab size: 26, total: 47 (saved 168 bytes)
static const bvalue be_ktab_class_CometAnimation[26] = {
  /* K0   */  be_nested_str_weak(update),
  /* K1   */  be_nested_str_weak(_fix_time_ms),
  /* K2   */  be_nested_str_weak(speed),
  /* K3   */  be_nested_str_weak(direction),
  /* K4   */  be_nested_str_weak(wrap_around),
  /* K5   */  be_nested_str_weak(engine),
  /* K6   */  be_nested_str_weak(get_strip_length),
  /* K7   */  be_nested_str_weak(start_time),
  /* K8   */  be_const_int(0),
  /* K9   */  be_nested_str_weak(head_position),
  /* K10  */  be_const_int(1),
  /* K11  */  be_nested_str_weak(init),
  /* K12  */  be_nested_str_weak(animation),
  /* K13  */  be_nested_str_weak(is_value_provider),
  /* K14  */  be_nested_str_weak(color),
  /* K15  */  be_nested_str_weak(0x_X2508x),
  /* K16  */  be_nested_str_weak(CometAnimation_X28color_X3D_X25s_X2C_X20head_pos_X3D_X25_X2E1f_X2C_X20tail_length_X3D_X25s_X2C_X20speed_X3D_X25s_X2C_X20direction_X3D_X25s_X2C_X20priority_X3D_X25s_X2C_X20running_X3D_X25s_X29),
  /* K17  */  be_nested_str_weak(tail_length),
  /* K18  */  be_nested_str_weak(priority),
  /* K19  */  be_nested_str_weak(is_running),
  /* K20  */  be_nested_str_weak(fade_factor),
  /* K21  */  be_nested_str_weak(tasmota),
  /* K22  */  be_nested_str_weak(scale_uint),
  /* K23  */  be_nested_str_weak(width),
  /* K24  */  be_nested_str_weak(set_pixel_color),
  /* K25  */  be_nested_str_weak(on_param_changed),
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
    ( &(const binstruction[71]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x740A0001,  //  0006  JMPT	R2	#0009
      0x50080000,  //  0007  LDBOOL	R2	0	0
      0x80040400,  //  0008  RET	1	R2
      0x8C080101,  //  0009  GETMET	R2	R0	K1
      0x5C100200,  //  000A  MOVE	R4	R1
      0x7C080400,  //  000B  CALL	R2	2
      0x5C040400,  //  000C  MOVE	R1	R2
      0x88080102,  //  000D  GETMBR	R2	R0	K2
      0x880C0103,  //  000E  GETMBR	R3	R0	K3
      0x88100104,  //  000F  GETMBR	R4	R0	K4
      0x88140105,  //  0010  GETMBR	R5	R0	K5
      0x8C140B06,  //  0011  GETMET	R5	R5	K6
      0x7C140200,  //  0012  CALL	R5	1
      0x88180107,  //  0013  GETMBR	R6	R0	K7
      0x04180206,  //  0014  SUB	R6	R1	R6
      0x081C0406,  //  0015  MUL	R7	R2	R6
      0x081C0E03,  //  0016  MUL	R7	R7	R3
      0x542203E7,  //  0017  LDINT	R8	1000
      0x0C1C0E08,  //  0018  DIV	R7	R7	R8
      0x24200708,  //  0019  GT	R8	R3	K8
      0x78220001,  //  001A  JMPF	R8	#001D
      0x90021207,  //  001B  SETMBR	R0	K9	R7
      0x70020004,  //  001C  JMP		#0022
      0x04200B0A,  //  001D  SUB	R8	R5	K10
      0x542600FF,  //  001E  LDINT	R9	256
      0x08201009,  //  001F  MUL	R8	R8	R9
      0x00201007,  //  0020  ADD	R8	R8	R7
      0x90021208,  //  0021  SETMBR	R0	K9	R8
      0x542200FF,  //  0022  LDINT	R8	256
      0x08200A08,  //  0023  MUL	R8	R5	R8
      0x20240908,  //  0024  NE	R9	R4	K8
      0x7826000E,  //  0025  JMPF	R9	#0035
      0x88240109,  //  0026  GETMBR	R9	R0	K9
      0x28241208,  //  0027  GE	R9	R9	R8
      0x78260003,  //  0028  JMPF	R9	#002D
      0x88240109,  //  0029  GETMBR	R9	R0	K9
      0x04241208,  //  002A  SUB	R9	R9	R8
      0x90021209,  //  002B  SETMBR	R0	K9	R9
      0x7001FFF8,  //  002C  JMP		#0026
      0x88240109,  //  002D  GETMBR	R9	R0	K9
      0x14241308,  //  002E  LT	R9	R9	K8
      0x78260003,  //  002F  JMPF	R9	#0034
      0x88240109,  //  0030  GETMBR	R9	R0	K9
      0x00241208,  //  0031  ADD	R9	R9	R8
      0x90021209,  //  0032  SETMBR	R0	K9	R9
      0x7001FFF8,  //  0033  JMP		#002D
      0x7002000F,  //  0034  JMP		#0045
      0x88240109,  //  0035  GETMBR	R9	R0	K9
      0x28241208,  //  0036  GE	R9	R9	R8
      0x78260006,  //  0037  JMPF	R9	#003F
      0x04240B0A,  //  0038  SUB	R9	R5	K10
      0x542A00FF,  //  0039  LDINT	R10	256
      0x0824120A,  //  003A  MUL	R9	R9	R10
      0x90021209,  //  003B  SETMBR	R0	K9	R9
      0x44240600,  //  003C  NEG	R9	R3
      0x90020609,  //  003D  SETMBR	R0	K3	R9
      0x70020005,  //  003E  JMP		#0045
      0x88240109,  //  003F  GETMBR	R9	R0	K9
      0x14241308,  //  0040  LT	R9	R9	K8
      0x78260002,  //  0041  JMPF	R9	#0045
      0x90021308,  //  0042  SETMBR	R0	K9	K8
      0x44240600,  //  0043  NEG	R9	R3
      0x90020609,  //  0044  SETMBR	R0	K3	R9
      0x50240200,  //  0045  LDBOOL	R9	1	0
      0x80041200,  //  0046  RET	1	R9
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
      0x8C08050B,  //  0003  GETMET	R2	R2	K11
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x90021308,  //  0006  SETMBR	R0	K9	K8
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
      0xB80A1800,  //  0001  GETNGBL	R2	K12
      0x8C08050D,  //  0002  GETMET	R2	R2	K13
      0x8810010E,  //  0003  GETMBR	R4	R0	K14
      0x7C080400,  //  0004  CALL	R2	2
      0x780A0004,  //  0005  JMPF	R2	#000B
      0x60080008,  //  0006  GETGBL	R2	G8
      0x880C010E,  //  0007  GETMBR	R3	R0	K14
      0x7C080200,  //  0008  CALL	R2	1
      0x5C040400,  //  0009  MOVE	R1	R2
      0x70020004,  //  000A  JMP		#0010
      0x60080018,  //  000B  GETGBL	R2	G24
      0x580C000F,  //  000C  LDCONST	R3	K15
      0x8810010E,  //  000D  GETMBR	R4	R0	K14
      0x7C080400,  //  000E  CALL	R2	2
      0x5C040400,  //  000F  MOVE	R1	R2
      0x60080018,  //  0010  GETGBL	R2	G24
      0x580C0010,  //  0011  LDCONST	R3	K16
      0x5C100200,  //  0012  MOVE	R4	R1
      0x88140109,  //  0013  GETMBR	R5	R0	K9
      0x541A00FF,  //  0014  LDINT	R6	256
      0x0C140A06,  //  0015  DIV	R5	R5	R6
      0x88180111,  //  0016  GETMBR	R6	R0	K17
      0x881C0102,  //  0017  GETMBR	R7	R0	K2
      0x88200103,  //  0018  GETMBR	R8	R0	K3
      0x88240112,  //  0019  GETMBR	R9	R0	K18
      0x88280113,  //  001A  GETMBR	R10	R0	K19
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
      0x880C0113,  //  0000  GETMBR	R3	R0	K19
      0x780E0002,  //  0001  JMPF	R3	#0005
      0x4C0C0000,  //  0002  LDNIL	R3
      0x1C0C0203,  //  0003  EQ	R3	R1	R3
      0x780E0001,  //  0004  JMPF	R3	#0007
      0x500C0000,  //  0005  LDBOOL	R3	0	0
      0x80040600,  //  0006  RET	1	R3
      0x880C0109,  //  0007  GETMBR	R3	R0	K9
      0x541200FF,  //  0008  LDINT	R4	256
      0x0C0C0604,  //  0009  DIV	R3	R3	R4
      0x8810010E,  //  000A  GETMBR	R4	R0	K14
      0x88140111,  //  000B  GETMBR	R5	R0	K17
      0x88180103,  //  000C  GETMBR	R6	R0	K3
      0x881C0104,  //  000D  GETMBR	R7	R0	K4
      0x88200114,  //  000E  GETMBR	R8	R0	K20
      0x88240105,  //  000F  GETMBR	R9	R0	K5
      0x8C241306,  //  0010  GETMET	R9	R9	K6
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
      0x58380008,  //  0020  LDCONST	R14	K8
      0x143C1C05,  //  0021  LT	R15	R14	R5
      0x783E0037,  //  0022  JMPF	R15	#005B
      0x083C1C06,  //  0023  MUL	R15	R14	R6
      0x043C060F,  //  0024  SUB	R15	R3	R15
      0x20400F08,  //  0025  NE	R16	R7	K8
      0x78420008,  //  0026  JMPF	R16	#0030
      0x28401E09,  //  0027  GE	R16	R15	R9
      0x78420001,  //  0028  JMPF	R16	#002B
      0x043C1E09,  //  0029  SUB	R15	R15	R9
      0x7001FFFB,  //  002A  JMP		#0027
      0x14401F08,  //  002B  LT	R16	R15	K8
      0x78420001,  //  002C  JMPF	R16	#002F
      0x003C1E09,  //  002D  ADD	R15	R15	R9
      0x7001FFFB,  //  002E  JMP		#002B
      0x70020005,  //  002F  JMP		#0036
      0x14401F08,  //  0030  LT	R16	R15	K8
      0x74420001,  //  0031  JMPT	R16	#0034
      0x28401E09,  //  0032  GE	R16	R15	R9
      0x78420001,  //  0033  JMPF	R16	#0036
      0x00381D0A,  //  0034  ADD	R14	R14	K10
      0x7001FFEA,  //  0035  JMP		#0021
      0x544200FE,  //  0036  LDINT	R16	255
      0x24441D08,  //  0037  GT	R17	R14	K8
      0x7846000D,  //  0038  JMPF	R17	#0047
      0x58440008,  //  0039  LDCONST	R17	K8
      0x1448220E,  //  003A  LT	R18	R17	R14
      0x784A000A,  //  003B  JMPF	R18	#0047
      0xB84A2A00,  //  003C  GETNGBL	R18	K21
      0x8C482516,  //  003D  GETMET	R18	R18	K22
      0x5C502000,  //  003E  MOVE	R20	R16
      0x58540008,  //  003F  LDCONST	R21	K8
      0x545A00FE,  //  0040  LDINT	R22	255
      0x585C0008,  //  0041  LDCONST	R23	K8
      0x5C601000,  //  0042  MOVE	R24	R8
      0x7C480C00,  //  0043  CALL	R18	6
      0x5C402400,  //  0044  MOVE	R16	R18
      0x0044230A,  //  0045  ADD	R17	R17	K10
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
      0x28481F08,  //  0050  GE	R18	R15	K8
      0x784A0006,  //  0051  JMPF	R18	#0059
      0x88480317,  //  0052  GETMBR	R18	R1	K23
      0x14481E12,  //  0053  LT	R18	R15	R18
      0x784A0003,  //  0054  JMPF	R18	#0059
      0x8C480318,  //  0055  GETMET	R18	R1	K24
      0x5C501E00,  //  0056  MOVE	R20	R15
      0x5C542200,  //  0057  MOVE	R21	R17
      0x7C480600,  //  0058  CALL	R18	3
      0x00381D0A,  //  0059  ADD	R14	R14	K10
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
    7,                          /* nstack */
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
    ( &(const binstruction[21]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0719,  //  0003  GETMET	R3	R3	K25
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x1C0C0303,  //  0007  EQ	R3	R1	K3
      0x780E000A,  //  0008  JMPF	R3	#0014
      0x880C0105,  //  0009  GETMBR	R3	R0	K5
      0x8C0C0706,  //  000A  GETMET	R3	R3	K6
      0x7C0C0200,  //  000B  CALL	R3	1
      0x24100508,  //  000C  GT	R4	R2	K8
      0x78120001,  //  000D  JMPF	R4	#0010
      0x90021308,  //  000E  SETMBR	R0	K9	K8
      0x70020003,  //  000F  JMP		#0014
      0x0410070A,  //  0010  SUB	R4	R3	K10
      0x541600FF,  //  0011  LDINT	R5	256
      0x08100805,  //  0012  MUL	R4	R4	R5
      0x90021204,  //  0013  SETMBR	R0	K9	R4
      0x80000000,  //  0014  RET	0
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
        be_const_map( *     be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(fade_factor, -1), be_const_bytes_instance(07000001FF0001B300) },
        { be_const_key_weak(wrap_around, -1), be_const_bytes_instance(07000000010001) },
        { be_const_key_weak(direction, -1), be_const_bytes_instance(1400010200FF0001) },
        { be_const_key_weak(speed, 0), be_const_bytes_instance(07000101006401000A) },
        { be_const_key_weak(tail_length, -1), be_const_bytes_instance(07000100320005) },
    }))    ) } )) },
        { be_const_key_weak(tostring, -1), be_const_closure(class_CometAnimation_tostring_closure) },
        { be_const_key_weak(render, 0), be_const_closure(class_CometAnimation_render_closure) },
        { be_const_key_weak(head_position, 2), be_const_var(0) },
        { be_const_key_weak(on_param_changed, -1), be_const_closure(class_CometAnimation_on_param_changed_closure) },
    })),
    be_str_weak(CometAnimation)
);
// compact class 'FireAnimation' ktab size: 51, total: 81 (saved 240 bytes)
static const bvalue be_ktab_class_FireAnimation[51] = {
  /* K0   */  be_const_int(0),
  /* K1   */  be_nested_str_weak(_random),
  /* K2   */  be_nested_str_weak(is_running),
  /* K3   */  be_nested_str_weak(_fix_time_ms),
  /* K4   */  be_nested_str_weak(engine),
  /* K5   */  be_nested_str_weak(get_strip_length),
  /* K6   */  be_nested_str_weak(width),
  /* K7   */  be_nested_str_weak(set_pixel_color),
  /* K8   */  be_nested_str_weak(current_colors),
  /* K9   */  be_nested_str_weak(get),
  /* K10  */  be_const_int(1),
  /* K11  */  be_nested_str_weak(FireAnimation_X28intensity_X3D_X25s_X2C_X20flicker_speed_X3D_X25s_X2C_X20priority_X3D_X25s_X2C_X20running_X3D_X25s_X29),
  /* K12  */  be_nested_str_weak(intensity),
  /* K13  */  be_nested_str_weak(flicker_speed),
  /* K14  */  be_nested_str_weak(priority),
  /* K15  */  be_nested_str_weak(init),
  /* K16  */  be_nested_str_weak(heat_map),
  /* K17  */  be_nested_str_weak(last_update),
  /* K18  */  be_nested_str_weak(random_seed),
  /* K19  */  be_nested_str_weak(time_ms),
  /* K20  */  be_nested_str_weak(clear),
  /* K21  */  be_nested_str_weak(resize),
  /* K22  */  be_nested_str_weak(set),
  /* K23  */  be_const_int(-16777216),
  /* K24  */  be_nested_str_weak(update),
  /* K25  */  be_nested_str_weak(_update_fire_simulation),
  /* K26  */  be_const_int(1103515245),
  /* K27  */  be_const_int(2147483647),
  /* K28  */  be_nested_str_weak(cooling_rate),
  /* K29  */  be_nested_str_weak(sparking_rate),
  /* K30  */  be_nested_str_weak(flicker_amount),
  /* K31  */  be_nested_str_weak(color),
  /* K32  */  be_nested_str_weak(size),
  /* K33  */  be_nested_str_weak(_initialize_buffers),
  /* K34  */  be_nested_str_weak(_random_range),
  /* K35  */  be_nested_str_weak(tasmota),
  /* K36  */  be_nested_str_weak(scale_uint),
  /* K37  */  be_const_int(2),
  /* K38  */  be_const_int(3),
  /* K39  */  be_nested_str_weak(animation),
  /* K40  */  be_nested_str_weak(rich_palette),
  /* K41  */  be_nested_str_weak(palette),
  /* K42  */  be_nested_str_weak(PALETTE_FIRE),
  /* K43  */  be_nested_str_weak(cycle_period),
  /* K44  */  be_nested_str_weak(transition_type),
  /* K45  */  be_nested_str_weak(brightness),
  /* K46  */  be_nested_str_weak(range_min),
  /* K47  */  be_nested_str_weak(range_max),
  /* K48  */  be_nested_str_weak(is_color_provider),
  /* K49  */  be_nested_str_weak(get_color_for_value),
  /* K50  */  be_nested_str_weak(start),
};


extern const bclass be_class_FireAnimation;

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
** Solidified function: render
********************************************************************/
be_local_closure(class_FireAnimation_render,   /* name */
  be_nested_proto(
    12,                          /* nstack */
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
    ( &(const binstruction[33]) {  /* code */
      0x880C0102,  //  0000  GETMBR	R3	R0	K2
      0x780E0002,  //  0001  JMPF	R3	#0005
      0x4C0C0000,  //  0002  LDNIL	R3
      0x1C0C0203,  //  0003  EQ	R3	R1	R3
      0x780E0001,  //  0004  JMPF	R3	#0007
      0x500C0000,  //  0005  LDBOOL	R3	0	0
      0x80040600,  //  0006  RET	1	R3
      0x8C0C0103,  //  0007  GETMET	R3	R0	K3
      0x5C140400,  //  0008  MOVE	R5	R2
      0x7C0C0400,  //  0009  CALL	R3	2
      0x5C080600,  //  000A  MOVE	R2	R3
      0x880C0104,  //  000B  GETMBR	R3	R0	K4
      0x8C0C0705,  //  000C  GETMET	R3	R3	K5
      0x7C0C0200,  //  000D  CALL	R3	1
      0x58100000,  //  000E  LDCONST	R4	K0
      0x14140803,  //  000F  LT	R5	R4	R3
      0x7816000D,  //  0010  JMPF	R5	#001F
      0x88140306,  //  0011  GETMBR	R5	R1	K6
      0x14140805,  //  0012  LT	R5	R4	R5
      0x78160008,  //  0013  JMPF	R5	#001D
      0x8C140307,  //  0014  GETMET	R5	R1	K7
      0x5C1C0800,  //  0015  MOVE	R7	R4
      0x88200108,  //  0016  GETMBR	R8	R0	K8
      0x8C201109,  //  0017  GETMET	R8	R8	K9
      0x542A0003,  //  0018  LDINT	R10	4
      0x0828080A,  //  0019  MUL	R10	R4	R10
      0x542DFFFB,  //  001A  LDINT	R11	-4
      0x7C200600,  //  001B  CALL	R8	3
      0x7C140600,  //  001C  CALL	R5	3
      0x0010090A,  //  001D  ADD	R4	R4	K10
      0x7001FFEF,  //  001E  JMP		#000F
      0x50140200,  //  001F  LDBOOL	R5	1	0
      0x80040A00,  //  0020  RET	1	R5
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
      0x5808000B,  //  0001  LDCONST	R2	K11
      0x880C010C,  //  0002  GETMBR	R3	R0	K12
      0x8810010D,  //  0003  GETMBR	R4	R0	K13
      0x8814010E,  //  0004  GETMBR	R5	R0	K14
      0x88180102,  //  0005  GETMBR	R6	R0	K2
      0x7C040A00,  //  0006  CALL	R1	5
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


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
      0x8C08050F,  //  0003  GETMET	R2	R2	K15
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x60080015,  //  0006  GETGBL	R2	G21
      0x7C080000,  //  0007  CALL	R2	0
      0x90022002,  //  0008  SETMBR	R0	K16	R2
      0x60080015,  //  0009  GETGBL	R2	G21
      0x7C080000,  //  000A  CALL	R2	0
      0x90021002,  //  000B  SETMBR	R0	K8	R2
      0x90022300,  //  000C  SETMBR	R0	K17	K0
      0x88080104,  //  000D  GETMBR	R2	R0	K4
      0x88080513,  //  000E  GETMBR	R2	R2	K19
      0x540EFFFF,  //  000F  LDINT	R3	65536
      0x10080403,  //  0010  MOD	R2	R2	R3
      0x90022402,  //  0011  SETMBR	R0	K18	R2
      0x80000000,  //  0012  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _initialize_buffers
********************************************************************/
be_local_closure(class_FireAnimation__initialize_buffers,   /* name */
  be_nested_proto(
    8,                          /* nstack */
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
    ( &(const binstruction[31]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x8C040305,  //  0001  GETMET	R1	R1	K5
      0x7C040200,  //  0002  CALL	R1	1
      0x88080110,  //  0003  GETMBR	R2	R0	K16
      0x8C080514,  //  0004  GETMET	R2	R2	K20
      0x7C080200,  //  0005  CALL	R2	1
      0x88080110,  //  0006  GETMBR	R2	R0	K16
      0x8C080515,  //  0007  GETMET	R2	R2	K21
      0x5C100200,  //  0008  MOVE	R4	R1
      0x7C080400,  //  0009  CALL	R2	2
      0x88080108,  //  000A  GETMBR	R2	R0	K8
      0x8C080514,  //  000B  GETMET	R2	R2	K20
      0x7C080200,  //  000C  CALL	R2	1
      0x88080108,  //  000D  GETMBR	R2	R0	K8
      0x8C080515,  //  000E  GETMET	R2	R2	K21
      0x54120003,  //  000F  LDINT	R4	4
      0x08100204,  //  0010  MUL	R4	R1	R4
      0x7C080400,  //  0011  CALL	R2	2
      0x58080000,  //  0012  LDCONST	R2	K0
      0x140C0401,  //  0013  LT	R3	R2	R1
      0x780E0008,  //  0014  JMPF	R3	#001E
      0x880C0108,  //  0015  GETMBR	R3	R0	K8
      0x8C0C0716,  //  0016  GETMET	R3	R3	K22
      0x54160003,  //  0017  LDINT	R5	4
      0x08140405,  //  0018  MUL	R5	R2	R5
      0x58180017,  //  0019  LDCONST	R6	K23
      0x541DFFFB,  //  001A  LDINT	R7	-4
      0x7C0C0800,  //  001B  CALL	R3	4
      0x0008050A,  //  001C  ADD	R2	R2	K10
      0x7001FFF4,  //  001D  JMP		#0013
      0x80000000,  //  001E  RET	0
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
    ( &(const binstruction[26]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080518,  //  0003  GETMET	R2	R2	K24
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x740A0001,  //  0006  JMPT	R2	#0009
      0x50080000,  //  0007  LDBOOL	R2	0	0
      0x80040400,  //  0008  RET	1	R2
      0x8C080103,  //  0009  GETMET	R2	R0	K3
      0x5C100200,  //  000A  MOVE	R4	R1
      0x7C080400,  //  000B  CALL	R2	2
      0x5C040400,  //  000C  MOVE	R1	R2
      0x8808010D,  //  000D  GETMBR	R2	R0	K13
      0x540E03E7,  //  000E  LDINT	R3	1000
      0x0C0C0602,  //  000F  DIV	R3	R3	R2
      0x88100111,  //  0010  GETMBR	R4	R0	K17
      0x04100204,  //  0011  SUB	R4	R1	R4
      0x28100803,  //  0012  GE	R4	R4	R3
      0x78120003,  //  0013  JMPF	R4	#0018
      0x90022201,  //  0014  SETMBR	R0	K17	R1
      0x8C100119,  //  0015  GETMET	R4	R0	K25
      0x5C180200,  //  0016  MOVE	R6	R1
      0x7C100400,  //  0017  CALL	R4	2
      0x50100200,  //  0018  LDBOOL	R4	1	0
      0x80040800,  //  0019  RET	1	R4
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
      0x88040112,  //  0000  GETMBR	R1	R0	K18
      0x0804031A,  //  0001  MUL	R1	R1	K26
      0x540A3038,  //  0002  LDINT	R2	12345
      0x00040202,  //  0003  ADD	R1	R1	R2
      0x2C04031B,  //  0004  AND	R1	R1	K27
      0x90022401,  //  0005  SETMBR	R0	K18	R1
      0x88040112,  //  0006  GETMBR	R1	R0	K18
      0x80040200,  //  0007  RET	1	R1
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
    ( &(const binstruction[236]) {  /* code */
      0x8808011C,  //  0000  GETMBR	R2	R0	K28
      0x880C011D,  //  0001  GETMBR	R3	R0	K29
      0x8810010C,  //  0002  GETMBR	R4	R0	K12
      0x8814011E,  //  0003  GETMBR	R5	R0	K30
      0x8818011F,  //  0004  GETMBR	R6	R0	K31
      0x881C0104,  //  0005  GETMBR	R7	R0	K4
      0x8C1C0F05,  //  0006  GETMET	R7	R7	K5
      0x7C1C0200,  //  0007  CALL	R7	1
      0x88200110,  //  0008  GETMBR	R8	R0	K16
      0x8C201120,  //  0009  GETMET	R8	R8	K32
      0x7C200200,  //  000A  CALL	R8	1
      0x20201007,  //  000B  NE	R8	R8	R7
      0x74220006,  //  000C  JMPT	R8	#0014
      0x88200108,  //  000D  GETMBR	R8	R0	K8
      0x8C201120,  //  000E  GETMET	R8	R8	K32
      0x7C200200,  //  000F  CALL	R8	1
      0x54260003,  //  0010  LDINT	R9	4
      0x08240E09,  //  0011  MUL	R9	R7	R9
      0x20201009,  //  0012  NE	R8	R8	R9
      0x78220001,  //  0013  JMPF	R8	#0016
      0x8C200121,  //  0014  GETMET	R8	R0	K33
      0x7C200200,  //  0015  CALL	R8	1
      0x58200000,  //  0016  LDCONST	R8	K0
      0x14241007,  //  0017  LT	R9	R8	R7
      0x78260017,  //  0018  JMPF	R9	#0031
      0x8C240122,  //  0019  GETMET	R9	R0	K34
      0xB82E4600,  //  001A  GETNGBL	R11	K35
      0x8C2C1724,  //  001B  GETMET	R11	R11	K36
      0x5C340400,  //  001C  MOVE	R13	R2
      0x58380000,  //  001D  LDCONST	R14	K0
      0x543E00FE,  //  001E  LDINT	R15	255
      0x58400000,  //  001F  LDCONST	R16	K0
      0x54460009,  //  0020  LDINT	R17	10
      0x7C2C0C00,  //  0021  CALL	R11	6
      0x002C1725,  //  0022  ADD	R11	R11	K37
      0x7C240400,  //  0023  CALL	R9	2
      0x88280110,  //  0024  GETMBR	R10	R0	K16
      0x94281408,  //  0025  GETIDX	R10	R10	R8
      0x2828120A,  //  0026  GE	R10	R9	R10
      0x782A0002,  //  0027  JMPF	R10	#002B
      0x88280110,  //  0028  GETMBR	R10	R0	K16
      0x98281100,  //  0029  SETIDX	R10	R8	K0
      0x70020003,  //  002A  JMP		#002F
      0x88280110,  //  002B  GETMBR	R10	R0	K16
      0x942C1408,  //  002C  GETIDX	R11	R10	R8
      0x042C1609,  //  002D  SUB	R11	R11	R9
      0x9828100B,  //  002E  SETIDX	R10	R8	R11
      0x0020110A,  //  002F  ADD	R8	R8	K10
      0x7001FFE5,  //  0030  JMP		#0017
      0x28240F26,  //  0031  GE	R9	R7	K38
      0x7826001D,  //  0032  JMPF	R9	#0051
      0x04240F0A,  //  0033  SUB	R9	R7	K10
      0x28281325,  //  0034  GE	R10	R9	K37
      0x782A001A,  //  0035  JMPF	R10	#0051
      0x0428130A,  //  0036  SUB	R10	R9	K10
      0x882C0110,  //  0037  GETMBR	R11	R0	K16
      0x9428160A,  //  0038  GETIDX	R10	R11	R10
      0x042C1325,  //  0039  SUB	R11	R9	K37
      0x88300110,  //  003A  GETMBR	R12	R0	K16
      0x942C180B,  //  003B  GETIDX	R11	R12	R11
      0x0028140B,  //  003C  ADD	R10	R10	R11
      0x042C1325,  //  003D  SUB	R11	R9	K37
      0x88300110,  //  003E  GETMBR	R12	R0	K16
      0x942C180B,  //  003F  GETIDX	R11	R12	R11
      0x0028140B,  //  0040  ADD	R10	R10	R11
      0x0C281526,  //  0041  DIV	R10	R10	K38
      0x142C1500,  //  0042  LT	R11	R10	K0
      0x782E0001,  //  0043  JMPF	R11	#0046
      0x58280000,  //  0044  LDCONST	R10	K0
      0x70020003,  //  0045  JMP		#004A
      0x542E00FE,  //  0046  LDINT	R11	255
      0x242C140B,  //  0047  GT	R11	R10	R11
      0x782E0000,  //  0048  JMPF	R11	#004A
      0x542A00FE,  //  0049  LDINT	R10	255
      0x882C0110,  //  004A  GETMBR	R11	R0	K16
      0x60300009,  //  004B  GETGBL	R12	G9
      0x5C341400,  //  004C  MOVE	R13	R10
      0x7C300200,  //  004D  CALL	R12	1
      0x982C120C,  //  004E  SETIDX	R11	R9	R12
      0x0424130A,  //  004F  SUB	R9	R9	K10
      0x7001FFE2,  //  0050  JMP		#0034
      0x8C240122,  //  0051  GETMET	R9	R0	K34
      0x542E00FE,  //  0052  LDINT	R11	255
      0x7C240400,  //  0053  CALL	R9	2
      0x14241203,  //  0054  LT	R9	R9	R3
      0x7826000F,  //  0055  JMPF	R9	#0066
      0x8C240122,  //  0056  GETMET	R9	R0	K34
      0x542E0006,  //  0057  LDINT	R11	7
      0x7C240400,  //  0058  CALL	R9	2
      0x8C280122,  //  0059  GETMET	R10	R0	K34
      0x5432005E,  //  005A  LDINT	R12	95
      0x7C280400,  //  005B  CALL	R10	2
      0x542E009F,  //  005C  LDINT	R11	160
      0x0028140B,  //  005D  ADD	R10	R10	R11
      0x542E00FE,  //  005E  LDINT	R11	255
      0x242C140B,  //  005F  GT	R11	R10	R11
      0x782E0000,  //  0060  JMPF	R11	#0062
      0x542A00FE,  //  0061  LDINT	R10	255
      0x142C1207,  //  0062  LT	R11	R9	R7
      0x782E0001,  //  0063  JMPF	R11	#0066
      0x882C0110,  //  0064  GETMBR	R11	R0	K16
      0x982C120A,  //  0065  SETIDX	R11	R9	R10
      0x58200000,  //  0066  LDCONST	R8	K0
      0x14241007,  //  0067  LT	R9	R8	R7
      0x78260081,  //  0068  JMPF	R9	#00EB
      0x88240110,  //  0069  GETMBR	R9	R0	K16
      0x94241208,  //  006A  GETIDX	R9	R9	R8
      0xB82A4600,  //  006B  GETNGBL	R10	K35
      0x8C281524,  //  006C  GETMET	R10	R10	K36
      0x5C301200,  //  006D  MOVE	R12	R9
      0x58340000,  //  006E  LDCONST	R13	K0
      0x543A00FE,  //  006F  LDINT	R14	255
      0x583C0000,  //  0070  LDCONST	R15	K0
      0x5C400800,  //  0071  MOVE	R16	R4
      0x7C280C00,  //  0072  CALL	R10	6
      0x5C241400,  //  0073  MOVE	R9	R10
      0x24280B00,  //  0074  GT	R10	R5	K0
      0x782A0012,  //  0075  JMPF	R10	#0089
      0x8C280122,  //  0076  GETMET	R10	R0	K34
      0x5C300A00,  //  0077  MOVE	R12	R5
      0x7C280400,  //  0078  CALL	R10	2
      0x8C2C0122,  //  0079  GETMET	R11	R0	K34
      0x58340025,  //  007A  LDCONST	R13	K37
      0x7C2C0400,  //  007B  CALL	R11	2
      0x1C2C1700,  //  007C  EQ	R11	R11	K0
      0x782E0001,  //  007D  JMPF	R11	#0080
      0x0024120A,  //  007E  ADD	R9	R9	R10
      0x70020004,  //  007F  JMP		#0085
      0x242C120A,  //  0080  GT	R11	R9	R10
      0x782E0001,  //  0081  JMPF	R11	#0084
      0x0424120A,  //  0082  SUB	R9	R9	R10
      0x70020000,  //  0083  JMP		#0085
      0x58240000,  //  0084  LDCONST	R9	K0
      0x542E00FE,  //  0085  LDINT	R11	255
      0x242C120B,  //  0086  GT	R11	R9	R11
      0x782E0000,  //  0087  JMPF	R11	#0089
      0x542600FE,  //  0088  LDINT	R9	255
      0x58280017,  //  0089  LDCONST	R10	K23
      0x242C1300,  //  008A  GT	R11	R9	K0
      0x782E0055,  //  008B  JMPF	R11	#00E2
      0x5C2C0C00,  //  008C  MOVE	R11	R6
      0x4C300000,  //  008D  LDNIL	R12
      0x1C30160C,  //  008E  EQ	R12	R11	R12
      0x7832000E,  //  008F  JMPF	R12	#009F
      0xB8324E00,  //  0090  GETNGBL	R12	K39
      0x8C301928,  //  0091  GETMET	R12	R12	K40
      0x88380104,  //  0092  GETMBR	R14	R0	K4
      0x7C300400,  //  0093  CALL	R12	2
      0xB8364E00,  //  0094  GETNGBL	R13	K39
      0x88341B2A,  //  0095  GETMBR	R13	R13	K42
      0x9032520D,  //  0096  SETMBR	R12	K41	R13
      0x90325700,  //  0097  SETMBR	R12	K43	K0
      0x9032590A,  //  0098  SETMBR	R12	K44	K10
      0x543600FE,  //  0099  LDINT	R13	255
      0x90325A0D,  //  009A  SETMBR	R12	K45	R13
      0x90325D00,  //  009B  SETMBR	R12	K46	K0
      0x543600FE,  //  009C  LDINT	R13	255
      0x90325E0D,  //  009D  SETMBR	R12	K47	R13
      0x5C2C1800,  //  009E  MOVE	R11	R12
      0xB8324E00,  //  009F  GETNGBL	R12	K39
      0x8C301930,  //  00A0  GETMET	R12	R12	K48
      0x5C381600,  //  00A1  MOVE	R14	R11
      0x7C300400,  //  00A2  CALL	R12	2
      0x78320009,  //  00A3  JMPF	R12	#00AE
      0x88301731,  //  00A4  GETMBR	R12	R11	K49
      0x4C340000,  //  00A5  LDNIL	R13
      0x2030180D,  //  00A6  NE	R12	R12	R13
      0x78320005,  //  00A7  JMPF	R12	#00AE
      0x8C301731,  //  00A8  GETMET	R12	R11	K49
      0x5C381200,  //  00A9  MOVE	R14	R9
      0x583C0000,  //  00AA  LDCONST	R15	K0
      0x7C300600,  //  00AB  CALL	R12	3
      0x5C281800,  //  00AC  MOVE	R10	R12
      0x70020033,  //  00AD  JMP		#00E2
      0x5C281600,  //  00AE  MOVE	R10	R11
      0x54320017,  //  00AF  LDINT	R12	24
      0x3C30140C,  //  00B0  SHR	R12	R10	R12
      0x543600FE,  //  00B1  LDINT	R13	255
      0x2C30180D,  //  00B2  AND	R12	R12	R13
      0x5436000F,  //  00B3  LDINT	R13	16
      0x3C34140D,  //  00B4  SHR	R13	R10	R13
      0x543A00FE,  //  00B5  LDINT	R14	255
      0x2C341A0E,  //  00B6  AND	R13	R13	R14
      0x543A0007,  //  00B7  LDINT	R14	8
      0x3C38140E,  //  00B8  SHR	R14	R10	R14
      0x543E00FE,  //  00B9  LDINT	R15	255
      0x2C381C0F,  //  00BA  AND	R14	R14	R15
      0x543E00FE,  //  00BB  LDINT	R15	255
      0x2C3C140F,  //  00BC  AND	R15	R10	R15
      0xB8424600,  //  00BD  GETNGBL	R16	K35
      0x8C402124,  //  00BE  GETMET	R16	R16	K36
      0x5C481200,  //  00BF  MOVE	R18	R9
      0x584C0000,  //  00C0  LDCONST	R19	K0
      0x545200FE,  //  00C1  LDINT	R20	255
      0x58540000,  //  00C2  LDCONST	R21	K0
      0x5C581A00,  //  00C3  MOVE	R22	R13
      0x7C400C00,  //  00C4  CALL	R16	6
      0x5C342000,  //  00C5  MOVE	R13	R16
      0xB8424600,  //  00C6  GETNGBL	R16	K35
      0x8C402124,  //  00C7  GETMET	R16	R16	K36
      0x5C481200,  //  00C8  MOVE	R18	R9
      0x584C0000,  //  00C9  LDCONST	R19	K0
      0x545200FE,  //  00CA  LDINT	R20	255
      0x58540000,  //  00CB  LDCONST	R21	K0
      0x5C581C00,  //  00CC  MOVE	R22	R14
      0x7C400C00,  //  00CD  CALL	R16	6
      0x5C382000,  //  00CE  MOVE	R14	R16
      0xB8424600,  //  00CF  GETNGBL	R16	K35
      0x8C402124,  //  00D0  GETMET	R16	R16	K36
      0x5C481200,  //  00D1  MOVE	R18	R9
      0x584C0000,  //  00D2  LDCONST	R19	K0
      0x545200FE,  //  00D3  LDINT	R20	255
      0x58540000,  //  00D4  LDCONST	R21	K0
      0x5C581E00,  //  00D5  MOVE	R22	R15
      0x7C400C00,  //  00D6  CALL	R16	6
      0x5C3C2000,  //  00D7  MOVE	R15	R16
      0x54420017,  //  00D8  LDINT	R16	24
      0x38401810,  //  00D9  SHL	R16	R12	R16
      0x5446000F,  //  00DA  LDINT	R17	16
      0x38441A11,  //  00DB  SHL	R17	R13	R17
      0x30402011,  //  00DC  OR	R16	R16	R17
      0x54460007,  //  00DD  LDINT	R17	8
      0x38441C11,  //  00DE  SHL	R17	R14	R17
      0x30402011,  //  00DF  OR	R16	R16	R17
      0x3040200F,  //  00E0  OR	R16	R16	R15
      0x5C282000,  //  00E1  MOVE	R10	R16
      0x882C0108,  //  00E2  GETMBR	R11	R0	K8
      0x8C2C1716,  //  00E3  GETMET	R11	R11	K22
      0x54360003,  //  00E4  LDINT	R13	4
      0x0834100D,  //  00E5  MUL	R13	R8	R13
      0x5C381400,  //  00E6  MOVE	R14	R10
      0x543DFFFB,  //  00E7  LDINT	R15	-4
      0x7C2C0800,  //  00E8  CALL	R11	4
      0x0020110A,  //  00E9  ADD	R8	R8	K10
      0x7001FF7B,  //  00EA  JMP		#0067
      0x80000000,  //  00EB  RET	0
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
      0x8C080532,  //  0003  GETMET	R2	R2	K50
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x90022300,  //  0006  SETMBR	R0	K17	K0
      0x8C080121,  //  0007  GETMET	R2	R0	K33
      0x7C080200,  //  0008  CALL	R2	1
      0x88080104,  //  0009  GETMBR	R2	R0	K4
      0x88080513,  //  000A  GETMBR	R2	R2	K19
      0x540EFFFF,  //  000B  LDINT	R3	65536
      0x10080403,  //  000C  MOD	R2	R2	R3
      0x90022402,  //  000D  SETMBR	R0	K18	R2
      0x80040000,  //  000E  RET	1	R0
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
    be_nested_map(14,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(heat_map, -1), be_const_var(0) },
        { be_const_key_weak(start, -1), be_const_closure(class_FireAnimation_start_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_FireAnimation_init_closure) },
        { be_const_key_weak(tostring, -1), be_const_closure(class_FireAnimation_tostring_closure) },
        { be_const_key_weak(random_seed, -1), be_const_var(3) },
        { be_const_key_weak(render, 9), be_const_closure(class_FireAnimation_render_closure) },
        { be_const_key_weak(update, -1), be_const_closure(class_FireAnimation_update_closure) },
        { be_const_key_weak(last_update, -1), be_const_var(2) },
        { be_const_key_weak(_initialize_buffers, 6), be_const_closure(class_FireAnimation__initialize_buffers_closure) },
        { be_const_key_weak(PARAMS, 10), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(flicker_amount, 4), be_const_bytes_instance(07000001FF000064) },
        { be_const_key_weak(intensity, 0), be_const_bytes_instance(07000001FF0001B400) },
        { be_const_key_weak(flicker_speed, -1), be_const_bytes_instance(07000100140008) },
        { be_const_key_weak(sparking_rate, -1), be_const_bytes_instance(07000001FF000078) },
        { be_const_key_weak(cooling_rate, -1), be_const_bytes_instance(07000001FF000037) },
    }))    ) } )) },
        { be_const_key_weak(_random, 2), be_const_closure(class_FireAnimation__random_closure) },
        { be_const_key_weak(current_colors, -1), be_const_var(1) },
        { be_const_key_weak(_update_fire_simulation, -1), be_const_closure(class_FireAnimation__update_fire_simulation_closure) },
        { be_const_key_weak(_random_range, 1), be_const_closure(class_FireAnimation__random_range_closure) },
    })),
    be_str_weak(FireAnimation)
);
// compact class 'RichPaletteColorProvider' ktab size: 46, total: 99 (saved 424 bytes)
static const bvalue be_ktab_class_RichPaletteColorProvider[46] = {
  /* K0   */  be_nested_str_weak(on_param_changed),
  /* K1   */  be_nested_str_weak(range_min),
  /* K2   */  be_nested_str_weak(range_max),
  /* K3   */  be_nested_str_weak(cycle_period),
  /* K4   */  be_nested_str_weak(palette),
  /* K5   */  be_nested_str_weak(slots_arr),
  /* K6   */  be_nested_str_weak(value_arr),
  /* K7   */  be_nested_str_weak(_recompute_palette),
  /* K8   */  be_nested_str_weak(_fix_time_ms),
  /* K9   */  be_nested_str_weak(_get_palette_bytes),
  /* K10  */  be_nested_str_weak(slots),
  /* K11  */  be_const_int(2),
  /* K12  */  be_nested_str_weak(brightness),
  /* K13  */  be_const_int(0),
  /* K14  */  be_nested_str_weak(get),
  /* K15  */  be_nested_str_weak(tasmota),
  /* K16  */  be_nested_str_weak(scale_uint),
  /* K17  */  be_nested_str_weak(current_color),
  /* K18  */  be_nested_str_weak(start_time),
  /* K19  */  be_const_int(1),
  /* K20  */  be_nested_str_weak(light_state),
  /* K21  */  be_nested_str_weak(set_rgb),
  /* K22  */  be_nested_str_weak(bri),
  /* K23  */  be_nested_str_weak(set_bri),
  /* K24  */  be_nested_str_weak(r),
  /* K25  */  be_nested_str_weak(g),
  /* K26  */  be_nested_str_weak(b),
  /* K27  */  be_const_int(-16777216),
  /* K28  */  be_nested_str_weak(resize),
  /* K29  */  be_nested_str_weak(scale_int),
  /* K30  */  be_nested_str_weak(RichPaletteColorProvider_X28slots_X3D_X25s_X2C_X20cycle_period_X3D_X25s_X29),
  /* K31  */  be_nested_str_weak(RichPaletteColorProvider_X28uninitialized_X29),
  /* K32  */  be_nested_str_weak(init),
  /* K33  */  be_nested_str_weak(global),
  /* K34  */  be_nested_str_weak(RGB),
  /* K35  */  be_nested_str_weak(start),
  /* K36  */  be_nested_str_weak(_parse_palette),
  /* K37  */  be_nested_str_weak(value_error),
  /* K38  */  be_nested_str_weak(range_min_X20must_X20be_X20lower_X20than_X20range_max),
  /* K39  */  be_nested_str_weak(_get_color_at_index),
  /* K40  */  be_nested_str_weak(_DEFAULT_PALETTE),
  /* K41  */  be_nested_str_weak(background_X3Alinear_X2Dgradient_X28to_X20right_X2C_X20_X23000000_X29_X3B),
  /* K42  */  be_nested_str_weak(background_X3Alinear_X2Dgradient_X28to_X20right),
  /* K43  */  be_nested_str_weak(_X2C_X23_X2502X_X2502X_X2502X_X20_X25_X2E1f_X25_X25),
  /* K44  */  be_const_real_hex(0x41200000),
  /* K45  */  be_nested_str_weak(_X29_X3B),
};


extern const bclass be_class_RichPaletteColorProvider;

/********************************************************************
** Solidified function: on_param_changed
********************************************************************/
be_local_closure(class_RichPaletteColorProvider_on_param_changed,   /* name */
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
    be_str_weak(on_param_changed),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0700,  //  0003  GETMET	R3	R3	K0
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x1C0C0301,  //  0007  EQ	R3	R1	K1
      0x740E0005,  //  0008  JMPT	R3	#000F
      0x1C0C0302,  //  0009  EQ	R3	R1	K2
      0x740E0003,  //  000A  JMPT	R3	#000F
      0x1C0C0303,  //  000B  EQ	R3	R1	K3
      0x740E0001,  //  000C  JMPT	R3	#000F
      0x1C0C0304,  //  000D  EQ	R3	R1	K4
      0x780E0009,  //  000E  JMPF	R3	#0019
      0x880C0105,  //  000F  GETMBR	R3	R0	K5
      0x4C100000,  //  0010  LDNIL	R4
      0x200C0604,  //  0011  NE	R3	R3	R4
      0x740E0003,  //  0012  JMPT	R3	#0017
      0x880C0106,  //  0013  GETMBR	R3	R0	K6
      0x4C100000,  //  0014  LDNIL	R4
      0x200C0604,  //  0015  NE	R3	R3	R4
      0x780E0001,  //  0016  JMPF	R3	#0019
      0x8C0C0107,  //  0017  GETMET	R3	R0	K7
      0x7C0C0200,  //  0018  CALL	R3	1
      0x80000000,  //  0019  RET	0
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
    ( &(const binstruction[250]) {  /* code */
      0x8C0C0108,  //  0000  GETMET	R3	R0	K8
      0x5C140400,  //  0001  MOVE	R5	R2
      0x7C0C0400,  //  0002  CALL	R3	2
      0x5C080600,  //  0003  MOVE	R2	R3
      0x880C0105,  //  0004  GETMBR	R3	R0	K5
      0x4C100000,  //  0005  LDNIL	R4
      0x1C0C0604,  //  0006  EQ	R3	R3	R4
      0x780E0005,  //  0007  JMPF	R3	#000E
      0x880C0106,  //  0008  GETMBR	R3	R0	K6
      0x4C100000,  //  0009  LDNIL	R4
      0x1C0C0604,  //  000A  EQ	R3	R3	R4
      0x780E0001,  //  000B  JMPF	R3	#000E
      0x8C0C0107,  //  000C  GETMET	R3	R0	K7
      0x7C0C0200,  //  000D  CALL	R3	1
      0x8C0C0109,  //  000E  GETMET	R3	R0	K9
      0x7C0C0200,  //  000F  CALL	R3	1
      0x4C100000,  //  0010  LDNIL	R4
      0x1C100604,  //  0011  EQ	R4	R3	R4
      0x74120002,  //  0012  JMPT	R4	#0016
      0x8810010A,  //  0013  GETMBR	R4	R0	K10
      0x1410090B,  //  0014  LT	R4	R4	K11
      0x78120001,  //  0015  JMPF	R4	#0018
      0x5411FFFE,  //  0016  LDINT	R4	-1
      0x80040800,  //  0017  RET	1	R4
      0x88100103,  //  0018  GETMBR	R4	R0	K3
      0x8814010C,  //  0019  GETMBR	R5	R0	K12
      0x1C18090D,  //  001A  EQ	R6	R4	K13
      0x781A0039,  //  001B  JMPF	R6	#0056
      0x8C18070E,  //  001C  GETMET	R6	R3	K14
      0x5820000D,  //  001D  LDCONST	R8	K13
      0x54260003,  //  001E  LDINT	R9	4
      0x7C180600,  //  001F  CALL	R6	3
      0x541E0007,  //  0020  LDINT	R7	8
      0x3C1C0C07,  //  0021  SHR	R7	R6	R7
      0x542200FE,  //  0022  LDINT	R8	255
      0x2C1C0E08,  //  0023  AND	R7	R7	R8
      0x5422000F,  //  0024  LDINT	R8	16
      0x3C200C08,  //  0025  SHR	R8	R6	R8
      0x542600FE,  //  0026  LDINT	R9	255
      0x2C201009,  //  0027  AND	R8	R8	R9
      0x54260017,  //  0028  LDINT	R9	24
      0x3C240C09,  //  0029  SHR	R9	R6	R9
      0x542A00FE,  //  002A  LDINT	R10	255
      0x2C24120A,  //  002B  AND	R9	R9	R10
      0x542A00FE,  //  002C  LDINT	R10	255
      0x20280A0A,  //  002D  NE	R10	R5	R10
      0x782A001A,  //  002E  JMPF	R10	#004A
      0xB82A1E00,  //  002F  GETNGBL	R10	K15
      0x8C281510,  //  0030  GETMET	R10	R10	K16
      0x5C300E00,  //  0031  MOVE	R12	R7
      0x5834000D,  //  0032  LDCONST	R13	K13
      0x543A00FE,  //  0033  LDINT	R14	255
      0x583C000D,  //  0034  LDCONST	R15	K13
      0x5C400A00,  //  0035  MOVE	R16	R5
      0x7C280C00,  //  0036  CALL	R10	6
      0x5C1C1400,  //  0037  MOVE	R7	R10
      0xB82A1E00,  //  0038  GETNGBL	R10	K15
      0x8C281510,  //  0039  GETMET	R10	R10	K16
      0x5C301000,  //  003A  MOVE	R12	R8
      0x5834000D,  //  003B  LDCONST	R13	K13
      0x543A00FE,  //  003C  LDINT	R14	255
      0x583C000D,  //  003D  LDCONST	R15	K13
      0x5C400A00,  //  003E  MOVE	R16	R5
      0x7C280C00,  //  003F  CALL	R10	6
      0x5C201400,  //  0040  MOVE	R8	R10
      0xB82A1E00,  //  0041  GETNGBL	R10	K15
      0x8C281510,  //  0042  GETMET	R10	R10	K16
      0x5C301200,  //  0043  MOVE	R12	R9
      0x5834000D,  //  0044  LDCONST	R13	K13
      0x543A00FE,  //  0045  LDINT	R14	255
      0x583C000D,  //  0046  LDCONST	R15	K13
      0x5C400A00,  //  0047  MOVE	R16	R5
      0x7C280C00,  //  0048  CALL	R10	6
      0x5C241400,  //  0049  MOVE	R9	R10
      0x542A00FE,  //  004A  LDINT	R10	255
      0x542E0017,  //  004B  LDINT	R11	24
      0x3828140B,  //  004C  SHL	R10	R10	R11
      0x542E000F,  //  004D  LDINT	R11	16
      0x382C0E0B,  //  004E  SHL	R11	R7	R11
      0x3028140B,  //  004F  OR	R10	R10	R11
      0x542E0007,  //  0050  LDINT	R11	8
      0x382C100B,  //  0051  SHL	R11	R8	R11
      0x3028140B,  //  0052  OR	R10	R10	R11
      0x30281409,  //  0053  OR	R10	R10	R9
      0x9002220A,  //  0054  SETMBR	R0	K17	R10
      0x80041400,  //  0055  RET	1	R10
      0x88180112,  //  0056  GETMBR	R6	R0	K18
      0x04180406,  //  0057  SUB	R6	R2	R6
      0x101C0C04,  //  0058  MOD	R7	R6	R4
      0x8820010A,  //  0059  GETMBR	R8	R0	K10
      0x0424110B,  //  005A  SUB	R9	R8	K11
      0x2428130D,  //  005B  GT	R10	R9	K13
      0x782A0006,  //  005C  JMPF	R10	#0064
      0x88280105,  //  005D  GETMBR	R10	R0	K5
      0x94281409,  //  005E  GETIDX	R10	R10	R9
      0x28280E0A,  //  005F  GE	R10	R7	R10
      0x782A0000,  //  0060  JMPF	R10	#0062
      0x70020001,  //  0061  JMP		#0064
      0x04241313,  //  0062  SUB	R9	R9	K19
      0x7001FFF6,  //  0063  JMP		#005B
      0x8C28070E,  //  0064  GETMET	R10	R3	K14
      0x54320003,  //  0065  LDINT	R12	4
      0x0830120C,  //  0066  MUL	R12	R9	R12
      0x54360003,  //  0067  LDINT	R13	4
      0x7C280600,  //  0068  CALL	R10	3
      0x8C2C070E,  //  0069  GETMET	R11	R3	K14
      0x00341313,  //  006A  ADD	R13	R9	K19
      0x543A0003,  //  006B  LDINT	R14	4
      0x08341A0E,  //  006C  MUL	R13	R13	R14
      0x543A0003,  //  006D  LDINT	R14	4
      0x7C2C0600,  //  006E  CALL	R11	3
      0x88300105,  //  006F  GETMBR	R12	R0	K5
      0x94301809,  //  0070  GETIDX	R12	R12	R9
      0x00341313,  //  0071  ADD	R13	R9	K19
      0x88380105,  //  0072  GETMBR	R14	R0	K5
      0x94341C0D,  //  0073  GETIDX	R13	R14	R13
      0xB83A1E00,  //  0074  GETNGBL	R14	K15
      0x8C381D10,  //  0075  GETMET	R14	R14	K16
      0x5C400E00,  //  0076  MOVE	R16	R7
      0x5C441800,  //  0077  MOVE	R17	R12
      0x5C481A00,  //  0078  MOVE	R18	R13
      0x544E0007,  //  0079  LDINT	R19	8
      0x3C4C1413,  //  007A  SHR	R19	R10	R19
      0x545200FE,  //  007B  LDINT	R20	255
      0x2C4C2614,  //  007C  AND	R19	R19	R20
      0x54520007,  //  007D  LDINT	R20	8
      0x3C501614,  //  007E  SHR	R20	R11	R20
      0x545600FE,  //  007F  LDINT	R21	255
      0x2C502815,  //  0080  AND	R20	R20	R21
      0x7C380C00,  //  0081  CALL	R14	6
      0xB83E1E00,  //  0082  GETNGBL	R15	K15
      0x8C3C1F10,  //  0083  GETMET	R15	R15	K16
      0x5C440E00,  //  0084  MOVE	R17	R7
      0x5C481800,  //  0085  MOVE	R18	R12
      0x5C4C1A00,  //  0086  MOVE	R19	R13
      0x5452000F,  //  0087  LDINT	R20	16
      0x3C501414,  //  0088  SHR	R20	R10	R20
      0x545600FE,  //  0089  LDINT	R21	255
      0x2C502815,  //  008A  AND	R20	R20	R21
      0x5456000F,  //  008B  LDINT	R21	16
      0x3C541615,  //  008C  SHR	R21	R11	R21
      0x545A00FE,  //  008D  LDINT	R22	255
      0x2C542A16,  //  008E  AND	R21	R21	R22
      0x7C3C0C00,  //  008F  CALL	R15	6
      0xB8421E00,  //  0090  GETNGBL	R16	K15
      0x8C402110,  //  0091  GETMET	R16	R16	K16
      0x5C480E00,  //  0092  MOVE	R18	R7
      0x5C4C1800,  //  0093  MOVE	R19	R12
      0x5C501A00,  //  0094  MOVE	R20	R13
      0x54560017,  //  0095  LDINT	R21	24
      0x3C541415,  //  0096  SHR	R21	R10	R21
      0x545A00FE,  //  0097  LDINT	R22	255
      0x2C542A16,  //  0098  AND	R21	R21	R22
      0x545A0017,  //  0099  LDINT	R22	24
      0x3C581616,  //  009A  SHR	R22	R11	R22
      0x545E00FE,  //  009B  LDINT	R23	255
      0x2C582C17,  //  009C  AND	R22	R22	R23
      0x7C400C00,  //  009D  CALL	R16	6
      0x88440114,  //  009E  GETMBR	R17	R0	K20
      0x8C482315,  //  009F  GETMET	R18	R17	K21
      0x54520007,  //  00A0  LDINT	R20	8
      0x3C501414,  //  00A1  SHR	R20	R10	R20
      0x545600FE,  //  00A2  LDINT	R21	255
      0x2C502815,  //  00A3  AND	R20	R20	R21
      0x5456000F,  //  00A4  LDINT	R21	16
      0x3C541415,  //  00A5  SHR	R21	R10	R21
      0x545A00FE,  //  00A6  LDINT	R22	255
      0x2C542A16,  //  00A7  AND	R21	R21	R22
      0x545A0017,  //  00A8  LDINT	R22	24
      0x3C581416,  //  00A9  SHR	R22	R10	R22
      0x545E00FE,  //  00AA  LDINT	R23	255
      0x2C582C17,  //  00AB  AND	R22	R22	R23
      0x7C480800,  //  00AC  CALL	R18	4
      0x88482316,  //  00AD  GETMBR	R18	R17	K22
      0x8C4C2315,  //  00AE  GETMET	R19	R17	K21
      0x54560007,  //  00AF  LDINT	R21	8
      0x3C541615,  //  00B0  SHR	R21	R11	R21
      0x545A00FE,  //  00B1  LDINT	R22	255
      0x2C542A16,  //  00B2  AND	R21	R21	R22
      0x545A000F,  //  00B3  LDINT	R22	16
      0x3C581616,  //  00B4  SHR	R22	R11	R22
      0x545E00FE,  //  00B5  LDINT	R23	255
      0x2C582C17,  //  00B6  AND	R22	R22	R23
      0x545E0017,  //  00B7  LDINT	R23	24
      0x3C5C1617,  //  00B8  SHR	R23	R11	R23
      0x546200FE,  //  00B9  LDINT	R24	255
      0x2C5C2E18,  //  00BA  AND	R23	R23	R24
      0x7C4C0800,  //  00BB  CALL	R19	4
      0x884C2316,  //  00BC  GETMBR	R19	R17	K22
      0xB8521E00,  //  00BD  GETNGBL	R20	K15
      0x8C502910,  //  00BE  GETMET	R20	R20	K16
      0x5C580E00,  //  00BF  MOVE	R22	R7
      0x5C5C1800,  //  00C0  MOVE	R23	R12
      0x5C601A00,  //  00C1  MOVE	R24	R13
      0x5C642400,  //  00C2  MOVE	R25	R18
      0x5C682600,  //  00C3  MOVE	R26	R19
      0x7C500C00,  //  00C4  CALL	R20	6
      0x8C542315,  //  00C5  GETMET	R21	R17	K21
      0x5C5C1C00,  //  00C6  MOVE	R23	R14
      0x5C601E00,  //  00C7  MOVE	R24	R15
      0x5C642000,  //  00C8  MOVE	R25	R16
      0x7C540800,  //  00C9  CALL	R21	4
      0x8C542317,  //  00CA  GETMET	R21	R17	K23
      0x5C5C2800,  //  00CB  MOVE	R23	R20
      0x7C540400,  //  00CC  CALL	R21	2
      0x88382318,  //  00CD  GETMBR	R14	R17	K24
      0x883C2319,  //  00CE  GETMBR	R15	R17	K25
      0x8840231A,  //  00CF  GETMBR	R16	R17	K26
      0x545600FE,  //  00D0  LDINT	R21	255
      0x20540A15,  //  00D1  NE	R21	R5	R21
      0x7856001A,  //  00D2  JMPF	R21	#00EE
      0xB8561E00,  //  00D3  GETNGBL	R21	K15
      0x8C542B10,  //  00D4  GETMET	R21	R21	K16
      0x5C5C1C00,  //  00D5  MOVE	R23	R14
      0x5860000D,  //  00D6  LDCONST	R24	K13
      0x546600FE,  //  00D7  LDINT	R25	255
      0x5868000D,  //  00D8  LDCONST	R26	K13
      0x5C6C0A00,  //  00D9  MOVE	R27	R5
      0x7C540C00,  //  00DA  CALL	R21	6
      0x5C382A00,  //  00DB  MOVE	R14	R21
      0xB8561E00,  //  00DC  GETNGBL	R21	K15
      0x8C542B10,  //  00DD  GETMET	R21	R21	K16
      0x5C5C1E00,  //  00DE  MOVE	R23	R15
      0x5860000D,  //  00DF  LDCONST	R24	K13
      0x546600FE,  //  00E0  LDINT	R25	255
      0x5868000D,  //  00E1  LDCONST	R26	K13
      0x5C6C0A00,  //  00E2  MOVE	R27	R5
      0x7C540C00,  //  00E3  CALL	R21	6
      0x5C3C2A00,  //  00E4  MOVE	R15	R21
      0xB8561E00,  //  00E5  GETNGBL	R21	K15
      0x8C542B10,  //  00E6  GETMET	R21	R21	K16
      0x5C5C2000,  //  00E7  MOVE	R23	R16
      0x5860000D,  //  00E8  LDCONST	R24	K13
      0x546600FE,  //  00E9  LDINT	R25	255
      0x5868000D,  //  00EA  LDCONST	R26	K13
      0x5C6C0A00,  //  00EB  MOVE	R27	R5
      0x7C540C00,  //  00EC  CALL	R21	6
      0x5C402A00,  //  00ED  MOVE	R16	R21
      0x545600FE,  //  00EE  LDINT	R21	255
      0x545A0017,  //  00EF  LDINT	R22	24
      0x38542A16,  //  00F0  SHL	R21	R21	R22
      0x545A000F,  //  00F1  LDINT	R22	16
      0x38581C16,  //  00F2  SHL	R22	R14	R22
      0x30542A16,  //  00F3  OR	R21	R21	R22
      0x545A0007,  //  00F4  LDINT	R22	8
      0x38581E16,  //  00F5  SHL	R22	R15	R22
      0x30542A16,  //  00F6  OR	R21	R21	R22
      0x30542A10,  //  00F7  OR	R21	R21	R16
      0x90022215,  //  00F8  SETMBR	R0	K17	R21
      0x80042A00,  //  00F9  RET	1	R21
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
      0x8808010A,  //  0002  GETMBR	R2	R0	K10
      0x28080202,  //  0003  GE	R2	R1	R2
      0x780A0001,  //  0004  JMPF	R2	#0007
      0x5409FFFE,  //  0005  LDINT	R2	-1
      0x80040400,  //  0006  RET	1	R2
      0x8C080109,  //  0007  GETMET	R2	R0	K9
      0x7C080200,  //  0008  CALL	R2	1
      0x8C0C050E,  //  0009  GETMET	R3	R2	K14
      0x54160003,  //  000A  LDINT	R5	4
      0x08140205,  //  000B  MUL	R5	R1	R5
      0x5419FFFB,  //  000C  LDINT	R6	-4
      0x7C0C0600,  //  000D  CALL	R3	3
      0x300C071B,  //  000E  OR	R3	R3	K27
      0x80040600,  //  000F  RET	1	R3
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
      0x8C0C0109,  //  0000  GETMET	R3	R0	K9
      0x7C0C0200,  //  0001  CALL	R3	1
      0x60100012,  //  0002  GETGBL	R4	G18
      0x7C100000,  //  0003  CALL	R4	0
      0x8814010A,  //  0004  GETMBR	R5	R0	K10
      0x8C18091C,  //  0005  GETMET	R6	R4	K28
      0x5C200A00,  //  0006  MOVE	R8	R5
      0x7C180400,  //  0007  CALL	R6	2
      0x8C18070E,  //  0008  GETMET	R6	R3	K14
      0x5820000D,  //  0009  LDCONST	R8	K13
      0x58240013,  //  000A  LDCONST	R9	K19
      0x7C180600,  //  000B  CALL	R6	3
      0x20180D0D,  //  000C  NE	R6	R6	K13
      0x781A0022,  //  000D  JMPF	R6	#0031
      0x5818000D,  //  000E  LDCONST	R6	K13
      0x581C000D,  //  000F  LDCONST	R7	K13
      0x04200B13,  //  0010  SUB	R8	R5	K19
      0x14200E08,  //  0011  LT	R8	R7	R8
      0x78220007,  //  0012  JMPF	R8	#001B
      0x8C20070E,  //  0013  GETMET	R8	R3	K14
      0x542A0003,  //  0014  LDINT	R10	4
      0x08280E0A,  //  0015  MUL	R10	R7	R10
      0x582C0013,  //  0016  LDCONST	R11	K19
      0x7C200600,  //  0017  CALL	R8	3
      0x00180C08,  //  0018  ADD	R6	R6	R8
      0x001C0F13,  //  0019  ADD	R7	R7	K19
      0x7001FFF4,  //  001A  JMP		#0010
      0x5820000D,  //  001B  LDCONST	R8	K13
      0x581C000D,  //  001C  LDCONST	R7	K13
      0x14240E05,  //  001D  LT	R9	R7	R5
      0x78260010,  //  001E  JMPF	R9	#0030
      0xB8261E00,  //  001F  GETNGBL	R9	K15
      0x8C24131D,  //  0020  GETMET	R9	R9	K29
      0x5C2C1000,  //  0021  MOVE	R11	R8
      0x5830000D,  //  0022  LDCONST	R12	K13
      0x5C340C00,  //  0023  MOVE	R13	R6
      0x5C380200,  //  0024  MOVE	R14	R1
      0x5C3C0400,  //  0025  MOVE	R15	R2
      0x7C240C00,  //  0026  CALL	R9	6
      0x98100E09,  //  0027  SETIDX	R4	R7	R9
      0x8C24070E,  //  0028  GETMET	R9	R3	K14
      0x542E0003,  //  0029  LDINT	R11	4
      0x082C0E0B,  //  002A  MUL	R11	R7	R11
      0x58300013,  //  002B  LDCONST	R12	K19
      0x7C240600,  //  002C  CALL	R9	3
      0x00201009,  //  002D  ADD	R8	R8	R9
      0x001C0F13,  //  002E  ADD	R7	R7	K19
      0x7001FFEC,  //  002F  JMP		#001D
      0x70020012,  //  0030  JMP		#0044
      0x5818000D,  //  0031  LDCONST	R6	K13
      0x141C0C05,  //  0032  LT	R7	R6	R5
      0x781E000F,  //  0033  JMPF	R7	#0044
      0x8C1C070E,  //  0034  GETMET	R7	R3	K14
      0x54260003,  //  0035  LDINT	R9	4
      0x08240C09,  //  0036  MUL	R9	R6	R9
      0x58280013,  //  0037  LDCONST	R10	K19
      0x7C1C0600,  //  0038  CALL	R7	3
      0xB8221E00,  //  0039  GETNGBL	R8	K15
      0x8C20111D,  //  003A  GETMET	R8	R8	K29
      0x5C280E00,  //  003B  MOVE	R10	R7
      0x582C000D,  //  003C  LDCONST	R11	K13
      0x543200FE,  //  003D  LDINT	R12	255
      0x5C340200,  //  003E  MOVE	R13	R1
      0x5C380400,  //  003F  MOVE	R14	R2
      0x7C200C00,  //  0040  CALL	R8	6
      0x98100C08,  //  0041  SETIDX	R4	R6	R8
      0x00180D13,  //  0042  ADD	R6	R6	K19
      0x7001FFED,  //  0043  JMP		#0032
      0x80040800,  //  0044  RET	1	R4
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
      0x5808001E,  //  0002  LDCONST	R2	K30
      0x880C010A,  //  0003  GETMBR	R3	R0	K10
      0x88100103,  //  0004  GETMBR	R4	R0	K3
      0x7C040600,  //  0005  CALL	R1	3
      0xA8040001,  //  0006  EXBLK	1	1
      0x80040200,  //  0007  RET	1	R1
      0xA8040001,  //  0008  EXBLK	1	1
      0x70020004,  //  0009  JMP		#000F
      0xAC040000,  //  000A  CATCH	R1	0	0
      0x70020001,  //  000B  JMP		#000E
      0x80063E00,  //  000C  RET	1	K31
      0x70020000,  //  000D  JMP		#000F
      0xB0080000,  //  000E  RAISE	2	R0	R0
      0x80000000,  //  000F  RET	0
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
    ( &(const binstruction[16]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080520,  //  0003  GETMET	R2	R2	K32
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x5409FFFE,  //  0006  LDINT	R2	-1
      0x90022202,  //  0007  SETMBR	R0	K17	R2
      0x9002150D,  //  0008  SETMBR	R0	K10	K13
      0xA40A4200,  //  0009  IMPORT	R2	K33
      0x8C0C0514,  //  000A  GETMET	R3	R2	K20
      0x88140514,  //  000B  GETMBR	R5	R2	K20
      0x88140B22,  //  000C  GETMBR	R5	R5	K34
      0x7C0C0400,  //  000D  CALL	R3	2
      0x90022803,  //  000E  SETMBR	R0	K20	R3
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(class_RichPaletteColorProvider_start,   /* name */
  be_nested_proto(
    5,                          /* nstack */
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
    ( &(const binstruction[17]) {  /* code */
      0x88080105,  //  0000  GETMBR	R2	R0	K5
      0x4C0C0000,  //  0001  LDNIL	R3
      0x1C080403,  //  0002  EQ	R2	R2	R3
      0x780A0005,  //  0003  JMPF	R2	#000A
      0x88080106,  //  0004  GETMBR	R2	R0	K6
      0x4C0C0000,  //  0005  LDNIL	R3
      0x1C080403,  //  0006  EQ	R2	R2	R3
      0x780A0001,  //  0007  JMPF	R2	#000A
      0x8C080107,  //  0008  GETMET	R2	R0	K7
      0x7C080200,  //  0009  CALL	R2	1
      0x60080003,  //  000A  GETGBL	R2	G3
      0x5C0C0000,  //  000B  MOVE	R3	R0
      0x7C080200,  //  000C  CALL	R2	1
      0x8C080523,  //  000D  GETMET	R2	R2	K35
      0x5C100200,  //  000E  MOVE	R4	R1
      0x7C080400,  //  000F  CALL	R2	2
      0x80040000,  //  0010  RET	1	R0
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
    ( &(const binstruction[133]) {  /* code */
      0x880C0105,  //  0000  GETMBR	R3	R0	K5
      0x4C100000,  //  0001  LDNIL	R4
      0x1C0C0604,  //  0002  EQ	R3	R3	R4
      0x780E0005,  //  0003  JMPF	R3	#000A
      0x880C0106,  //  0004  GETMBR	R3	R0	K6
      0x4C100000,  //  0005  LDNIL	R4
      0x1C0C0604,  //  0006  EQ	R3	R3	R4
      0x780E0001,  //  0007  JMPF	R3	#000A
      0x8C0C0107,  //  0008  GETMET	R3	R0	K7
      0x7C0C0200,  //  0009  CALL	R3	1
      0x8C0C0109,  //  000A  GETMET	R3	R0	K9
      0x7C0C0200,  //  000B  CALL	R3	1
      0x88100101,  //  000C  GETMBR	R4	R0	K1
      0x88140102,  //  000D  GETMBR	R5	R0	K2
      0x8818010C,  //  000E  GETMBR	R6	R0	K12
      0x4C1C0000,  //  000F  LDNIL	R7
      0x1C1C0807,  //  0010  EQ	R7	R4	R7
      0x741E0002,  //  0011  JMPT	R7	#0015
      0x4C1C0000,  //  0012  LDNIL	R7
      0x1C1C0A07,  //  0013  EQ	R7	R5	R7
      0x781E0001,  //  0014  JMPF	R7	#0017
      0x4C1C0000,  //  0015  LDNIL	R7
      0x80040E00,  //  0016  RET	1	R7
      0x881C010A,  //  0017  GETMBR	R7	R0	K10
      0x04200F0B,  //  0018  SUB	R8	R7	K11
      0x2424110D,  //  0019  GT	R9	R8	K13
      0x78260006,  //  001A  JMPF	R9	#0022
      0x88240106,  //  001B  GETMBR	R9	R0	K6
      0x94241208,  //  001C  GETIDX	R9	R9	R8
      0x28240209,  //  001D  GE	R9	R1	R9
      0x78260000,  //  001E  JMPF	R9	#0020
      0x70020001,  //  001F  JMP		#0022
      0x04201113,  //  0020  SUB	R8	R8	K19
      0x7001FFF6,  //  0021  JMP		#0019
      0x8C24070E,  //  0022  GETMET	R9	R3	K14
      0x542E0003,  //  0023  LDINT	R11	4
      0x082C100B,  //  0024  MUL	R11	R8	R11
      0x54320003,  //  0025  LDINT	R12	4
      0x7C240600,  //  0026  CALL	R9	3
      0x8C28070E,  //  0027  GETMET	R10	R3	K14
      0x00301113,  //  0028  ADD	R12	R8	K19
      0x54360003,  //  0029  LDINT	R13	4
      0x0830180D,  //  002A  MUL	R12	R12	R13
      0x54360003,  //  002B  LDINT	R13	4
      0x7C280600,  //  002C  CALL	R10	3
      0x882C0106,  //  002D  GETMBR	R11	R0	K6
      0x942C1608,  //  002E  GETIDX	R11	R11	R8
      0x00301113,  //  002F  ADD	R12	R8	K19
      0x88340106,  //  0030  GETMBR	R13	R0	K6
      0x94301A0C,  //  0031  GETIDX	R12	R13	R12
      0xB8361E00,  //  0032  GETNGBL	R13	K15
      0x8C341B10,  //  0033  GETMET	R13	R13	K16
      0x5C3C0200,  //  0034  MOVE	R15	R1
      0x5C401600,  //  0035  MOVE	R16	R11
      0x5C441800,  //  0036  MOVE	R17	R12
      0x544A0007,  //  0037  LDINT	R18	8
      0x3C481212,  //  0038  SHR	R18	R9	R18
      0x544E00FE,  //  0039  LDINT	R19	255
      0x2C482413,  //  003A  AND	R18	R18	R19
      0x544E0007,  //  003B  LDINT	R19	8
      0x3C4C1413,  //  003C  SHR	R19	R10	R19
      0x545200FE,  //  003D  LDINT	R20	255
      0x2C4C2614,  //  003E  AND	R19	R19	R20
      0x7C340C00,  //  003F  CALL	R13	6
      0xB83A1E00,  //  0040  GETNGBL	R14	K15
      0x8C381D10,  //  0041  GETMET	R14	R14	K16
      0x5C400200,  //  0042  MOVE	R16	R1
      0x5C441600,  //  0043  MOVE	R17	R11
      0x5C481800,  //  0044  MOVE	R18	R12
      0x544E000F,  //  0045  LDINT	R19	16
      0x3C4C1213,  //  0046  SHR	R19	R9	R19
      0x545200FE,  //  0047  LDINT	R20	255
      0x2C4C2614,  //  0048  AND	R19	R19	R20
      0x5452000F,  //  0049  LDINT	R20	16
      0x3C501414,  //  004A  SHR	R20	R10	R20
      0x545600FE,  //  004B  LDINT	R21	255
      0x2C502815,  //  004C  AND	R20	R20	R21
      0x7C380C00,  //  004D  CALL	R14	6
      0xB83E1E00,  //  004E  GETNGBL	R15	K15
      0x8C3C1F10,  //  004F  GETMET	R15	R15	K16
      0x5C440200,  //  0050  MOVE	R17	R1
      0x5C481600,  //  0051  MOVE	R18	R11
      0x5C4C1800,  //  0052  MOVE	R19	R12
      0x54520017,  //  0053  LDINT	R20	24
      0x3C501214,  //  0054  SHR	R20	R9	R20
      0x545600FE,  //  0055  LDINT	R21	255
      0x2C502815,  //  0056  AND	R20	R20	R21
      0x54560017,  //  0057  LDINT	R21	24
      0x3C541415,  //  0058  SHR	R21	R10	R21
      0x545A00FE,  //  0059  LDINT	R22	255
      0x2C542A16,  //  005A  AND	R21	R21	R22
      0x7C3C0C00,  //  005B  CALL	R15	6
      0x544200FE,  //  005C  LDINT	R16	255
      0x20400C10,  //  005D  NE	R16	R6	R16
      0x7842001A,  //  005E  JMPF	R16	#007A
      0xB8421E00,  //  005F  GETNGBL	R16	K15
      0x8C402110,  //  0060  GETMET	R16	R16	K16
      0x5C481A00,  //  0061  MOVE	R18	R13
      0x584C000D,  //  0062  LDCONST	R19	K13
      0x545200FE,  //  0063  LDINT	R20	255
      0x5854000D,  //  0064  LDCONST	R21	K13
      0x5C580C00,  //  0065  MOVE	R22	R6
      0x7C400C00,  //  0066  CALL	R16	6
      0x5C342000,  //  0067  MOVE	R13	R16
      0xB8421E00,  //  0068  GETNGBL	R16	K15
      0x8C402110,  //  0069  GETMET	R16	R16	K16
      0x5C481C00,  //  006A  MOVE	R18	R14
      0x584C000D,  //  006B  LDCONST	R19	K13
      0x545200FE,  //  006C  LDINT	R20	255
      0x5854000D,  //  006D  LDCONST	R21	K13
      0x5C580C00,  //  006E  MOVE	R22	R6
      0x7C400C00,  //  006F  CALL	R16	6
      0x5C382000,  //  0070  MOVE	R14	R16
      0xB8421E00,  //  0071  GETNGBL	R16	K15
      0x8C402110,  //  0072  GETMET	R16	R16	K16
      0x5C481E00,  //  0073  MOVE	R18	R15
      0x584C000D,  //  0074  LDCONST	R19	K13
      0x545200FE,  //  0075  LDINT	R20	255
      0x5854000D,  //  0076  LDCONST	R21	K13
      0x5C580C00,  //  0077  MOVE	R22	R6
      0x7C400C00,  //  0078  CALL	R16	6
      0x5C3C2000,  //  0079  MOVE	R15	R16
      0x544200FE,  //  007A  LDINT	R16	255
      0x54460017,  //  007B  LDINT	R17	24
      0x38402011,  //  007C  SHL	R16	R16	R17
      0x5446000F,  //  007D  LDINT	R17	16
      0x38441A11,  //  007E  SHL	R17	R13	R17
      0x30402011,  //  007F  OR	R16	R16	R17
      0x54460007,  //  0080  LDINT	R17	8
      0x38441C11,  //  0081  SHL	R17	R14	R17
      0x30402011,  //  0082  OR	R16	R16	R17
      0x3040200F,  //  0083  OR	R16	R16	R15
      0x80042000,  //  0084  RET	1	R16
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
    ( &(const binstruction[48]) {  /* code */
      0x88040103,  //  0000  GETMBR	R1	R0	K3
      0x8C080109,  //  0001  GETMET	R2	R0	K9
      0x7C080200,  //  0002  CALL	R2	1
      0x600C000C,  //  0003  GETGBL	R3	G12
      0x5C100400,  //  0004  MOVE	R4	R2
      0x7C0C0200,  //  0005  CALL	R3	1
      0x54120003,  //  0006  LDINT	R4	4
      0x0C0C0604,  //  0007  DIV	R3	R3	R4
      0x90021403,  //  0008  SETMBR	R0	K10	R3
      0x240C030D,  //  0009  GT	R3	R1	K13
      0x780E0008,  //  000A  JMPF	R3	#0014
      0x4C0C0000,  //  000B  LDNIL	R3
      0x200C0403,  //  000C  NE	R3	R2	R3
      0x780E0005,  //  000D  JMPF	R3	#0014
      0x8C0C0124,  //  000E  GETMET	R3	R0	K36
      0x5814000D,  //  000F  LDCONST	R5	K13
      0x04180313,  //  0010  SUB	R6	R1	K19
      0x7C0C0600,  //  0011  CALL	R3	3
      0x90020A03,  //  0012  SETMBR	R0	K5	R3
      0x70020001,  //  0013  JMP		#0016
      0x4C0C0000,  //  0014  LDNIL	R3
      0x90020A03,  //  0015  SETMBR	R0	K5	R3
      0x880C0101,  //  0016  GETMBR	R3	R0	K1
      0x88100102,  //  0017  GETMBR	R4	R0	K2
      0x28140604,  //  0018  GE	R5	R3	R4
      0x78160000,  //  0019  JMPF	R5	#001B
      0xB0064B26,  //  001A  RAISE	1	K37	K38
      0x8C140109,  //  001B  GETMET	R5	R0	K9
      0x7C140200,  //  001C  CALL	R5	1
      0x4C180000,  //  001D  LDNIL	R6
      0x20140A06,  //  001E  NE	R5	R5	R6
      0x78160005,  //  001F  JMPF	R5	#0026
      0x8C140124,  //  0020  GETMET	R5	R0	K36
      0x5C1C0600,  //  0021  MOVE	R7	R3
      0x5C200800,  //  0022  MOVE	R8	R4
      0x7C140600,  //  0023  CALL	R5	3
      0x90020C05,  //  0024  SETMBR	R0	K6	R5
      0x70020001,  //  0025  JMP		#0028
      0x4C140000,  //  0026  LDNIL	R5
      0x90020C05,  //  0027  SETMBR	R0	K6	R5
      0x8814010A,  //  0028  GETMBR	R5	R0	K10
      0x24140B0D,  //  0029  GT	R5	R5	K13
      0x78160003,  //  002A  JMPF	R5	#002F
      0x8C140127,  //  002B  GETMET	R5	R0	K39
      0x581C000D,  //  002C  LDCONST	R7	K13
      0x7C140400,  //  002D  CALL	R5	2
      0x90022205,  //  002E  SETMBR	R0	K17	R5
      0x80040000,  //  002F  RET	1	R0
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
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x4C080000,  //  0001  LDNIL	R2
      0x20080202,  //  0002  NE	R2	R1	R2
      0x780A0001,  //  0003  JMPF	R2	#0006
      0x5C080200,  //  0004  MOVE	R2	R1
      0x70020000,  //  0005  JMP		#0007
      0x88080128,  //  0006  GETMBR	R2	R0	K40
      0x80040400,  //  0007  RET	1	R2
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
      0x8C040109,  //  0000  GETMET	R1	R0	K9
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x1C080202,  //  0003  EQ	R2	R1	R2
      0x780A0000,  //  0004  JMPF	R2	#0006
      0x80065200,  //  0005  RET	1	K41
      0x8C080124,  //  0006  GETMET	R2	R0	K36
      0x5810000D,  //  0007  LDCONST	R4	K13
      0x541603E7,  //  0008  LDINT	R5	1000
      0x7C080600,  //  0009  CALL	R2	3
      0x580C002A,  //  000A  LDCONST	R3	K42
      0x5810000D,  //  000B  LDCONST	R4	K13
      0x6014000C,  //  000C  GETGBL	R5	G12
      0x5C180400,  //  000D  MOVE	R6	R2
      0x7C140200,  //  000E  CALL	R5	1
      0x14140805,  //  000F  LT	R5	R4	R5
      0x7816001B,  //  0010  JMPF	R5	#002D
      0x94140404,  //  0011  GETIDX	R5	R2	R4
      0x8C18030E,  //  0012  GETMET	R6	R1	K14
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
      0x582C002B,  //  0024  LDCONST	R11	K43
      0x5C300E00,  //  0025  MOVE	R12	R7
      0x5C341000,  //  0026  MOVE	R13	R8
      0x5C381200,  //  0027  MOVE	R14	R9
      0x0C3C0B2C,  //  0028  DIV	R15	R5	K44
      0x7C280A00,  //  0029  CALL	R10	5
      0x000C060A,  //  002A  ADD	R3	R3	R10
      0x00100913,  //  002B  ADD	R4	R4	K19
      0x7001FFDE,  //  002C  JMP		#000C
      0x000C072D,  //  002D  ADD	R3	R3	K45
      0x80040600,  //  002E  RET	1	R3
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
    be_nested_map(18,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(value_arr, -1), be_const_var(1) },
        { be_const_key_weak(to_css_gradient, -1), be_const_closure(class_RichPaletteColorProvider_to_css_gradient_closure) },
        { be_const_key_weak(_parse_palette, -1), be_const_closure(class_RichPaletteColorProvider__parse_palette_closure) },
        { be_const_key_weak(_DEFAULT_PALETTE, 17), be_const_bytes_instance(00FF000024FFA50049FFFF006E00FF00920000FFB74B0082DBEE82EEFFFF0000) },
        { be_const_key_weak(slots_arr, -1), be_const_var(0) },
        { be_const_key_weak(_get_color_at_index, 2), be_const_closure(class_RichPaletteColorProvider__get_color_at_index_closure) },
        { be_const_key_weak(PARAMS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(cycle_period, -1), be_const_bytes_instance(050000018813) },
        { be_const_key_weak(range_min, 0), be_const_bytes_instance(040000) },
        { be_const_key_weak(palette, -1), be_const_bytes_instance(0C0602) },
        { be_const_key_weak(transition_type, 1), be_const_bytes_instance(1400050200010005) },
        { be_const_key_weak(brightness, -1), be_const_bytes_instance(07000001FF0001FF00) },
        { be_const_key_weak(range_max, -1), be_const_bytes_instance(0401FF00) },
    }))    ) } )) },
        { be_const_key_weak(tostring, 1), be_const_closure(class_RichPaletteColorProvider_tostring_closure) },
        { be_const_key_weak(get_color_for_value, -1), be_const_closure(class_RichPaletteColorProvider_get_color_for_value_closure) },
        { be_const_key_weak(light_state, -1), be_const_var(4) },
        { be_const_key_weak(current_color, -1), be_const_var(3) },
        { be_const_key_weak(start, 8), be_const_closure(class_RichPaletteColorProvider_start_closure) },
        { be_const_key_weak(slots, -1), be_const_var(2) },
        { be_const_key_weak(on_param_changed, 9), be_const_closure(class_RichPaletteColorProvider_on_param_changed_closure) },
        { be_const_key_weak(_recompute_palette, -1), be_const_closure(class_RichPaletteColorProvider__recompute_palette_closure) },
        { be_const_key_weak(init, 6), be_const_closure(class_RichPaletteColorProvider_init_closure) },
        { be_const_key_weak(_get_palette_bytes, -1), be_const_closure(class_RichPaletteColorProvider__get_palette_bytes_closure) },
        { be_const_key_weak(produce_value, -1), be_const_closure(class_RichPaletteColorProvider_produce_value_closure) },
    })),
    be_str_weak(RichPaletteColorProvider)
);

/********************************************************************
** Solidified function: list_user_functions
********************************************************************/
be_local_closure(list_user_functions,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    0,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(user_functions),
    /* K2   */  be_nested_str_weak(keys),
    /* K3   */  be_nested_str_weak(push),
    /* K4   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(list_user_functions),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x60000012,  //  0000  GETGBL	R0	G18
      0x7C000000,  //  0001  CALL	R0	0
      0x60040010,  //  0002  GETGBL	R1	G16
      0xB80A0000,  //  0003  GETNGBL	R2	K0
      0x88080501,  //  0004  GETMBR	R2	R2	K1
      0x8C080502,  //  0005  GETMET	R2	R2	K2
      0x7C080200,  //  0006  CALL	R2	1
      0x7C040200,  //  0007  CALL	R1	1
      0xA8020005,  //  0008  EXBLK	0	#000F
      0x5C080200,  //  0009  MOVE	R2	R1
      0x7C080000,  //  000A  CALL	R2	0
      0x8C0C0103,  //  000B  GETMET	R3	R0	K3
      0x5C140400,  //  000C  MOVE	R5	R2
      0x7C0C0400,  //  000D  CALL	R3	2
      0x7001FFF9,  //  000E  JMP		#0009
      0x58040004,  //  000F  LDCONST	R1	K4
      0xAC040200,  //  0010  CATCH	R1	1	0
      0xB0080000,  //  0011  RAISE	2	R0	R0
      0x80040000,  //  0012  RET	1	R0
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

// compact class 'StaticColorProvider' ktab size: 2, total: 4 (saved 16 bytes)
static const bvalue be_ktab_class_StaticColorProvider[2] = {
  /* K0   */  be_nested_str_weak(color),
  /* K1   */  be_nested_str_weak(StaticColorProvider_X28color_X3D0x_X2508X_X29),
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
    ( &(const binstruction[ 5]) {  /* code */
      0x60040018,  //  0000  GETGBL	R1	G24
      0x58080001,  //  0001  LDCONST	R2	K1
      0x880C0100,  //  0002  GETMBR	R3	R0	K0
      0x7C040400,  //  0003  CALL	R1	2
      0x80040200,  //  0004  RET	1	R1
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
        { be_const_key_weak(color, -1), be_const_bytes_instance(0400FF) },
    }))    ) } )) },
    })),
    be_str_weak(StaticColorProvider)
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

// compact class 'OscillatorValueProvider' ktab size: 30, total: 37 (saved 56 bytes)
static const bvalue be_ktab_class_OscillatorValueProvider[30] = {
  /* K0   */  be_nested_str_weak(duration),
  /* K1   */  be_nested_str_weak(min_value),
  /* K2   */  be_nested_str_weak(max_value),
  /* K3   */  be_nested_str_weak(form),
  /* K4   */  be_nested_str_weak(phase),
  /* K5   */  be_nested_str_weak(duty_cycle),
  /* K6   */  be_nested_str_weak(_fix_time_ms),
  /* K7   */  be_const_int(0),
  /* K8   */  be_nested_str_weak(start_time),
  /* K9   */  be_nested_str_weak(tasmota),
  /* K10  */  be_nested_str_weak(scale_uint),
  /* K11  */  be_nested_str_weak(animation),
  /* K12  */  be_nested_str_weak(SAWTOOTH),
  /* K13  */  be_nested_str_weak(value),
  /* K14  */  be_nested_str_weak(scale_int),
  /* K15  */  be_const_int(1),
  /* K16  */  be_nested_str_weak(TRIANGLE),
  /* K17  */  be_nested_str_weak(SQUARE),
  /* K18  */  be_nested_str_weak(COSINE),
  /* K19  */  be_nested_str_weak(sine_int),
  /* K20  */  be_nested_str_weak(SINE),
  /* K21  */  be_nested_str_weak(EASE_IN),
  /* K22  */  be_nested_str_weak(EASE_OUT),
  /* K23  */  be_nested_str_weak(ELASTIC),
  /* K24  */  be_nested_str_weak(BOUNCE),
  /* K25  */  be_nested_str_weak(form_names),
  /* K26  */  be_nested_str_weak(UNKNOWN),
  /* K27  */  be_nested_str_weak(OscillatorValueProvider_X28min_value_X3D_X25s_X2C_X20max_value_X3D_X25s_X2C_X20duration_X3D_X25sms_X2C_X20form_X3D_X25s_X29),
  /* K28  */  be_nested_str_weak(init),
  /* K29  */  be_nested_str_weak(start),
};


extern const bclass be_class_OscillatorValueProvider;

/********************************************************************
** Solidified function: produce_value
********************************************************************/
be_local_closure(class_OscillatorValueProvider_produce_value,   /* name */
  be_nested_proto(
    26,                          /* nstack */
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
    ( &(const binstruction[435]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x88100101,  //  0001  GETMBR	R4	R0	K1
      0x88140102,  //  0002  GETMBR	R5	R0	K2
      0x88180103,  //  0003  GETMBR	R6	R0	K3
      0x881C0104,  //  0004  GETMBR	R7	R0	K4
      0x88200105,  //  0005  GETMBR	R8	R0	K5
      0x8C240106,  //  0006  GETMET	R9	R0	K6
      0x5C2C0400,  //  0007  MOVE	R11	R2
      0x7C240400,  //  0008  CALL	R9	2
      0x5C081200,  //  0009  MOVE	R2	R9
      0x4C240000,  //  000A  LDNIL	R9
      0x1C240609,  //  000B  EQ	R9	R3	R9
      0x74260001,  //  000C  JMPT	R9	#000F
      0x18240707,  //  000D  LE	R9	R3	K7
      0x78260000,  //  000E  JMPF	R9	#0010
      0x80040800,  //  000F  RET	1	R4
      0x88240108,  //  0010  GETMBR	R9	R0	K8
      0x04240409,  //  0011  SUB	R9	R2	R9
      0x14281307,  //  0012  LT	R10	R9	K7
      0x782A0000,  //  0013  JMPF	R10	#0015
      0x58240007,  //  0014  LDCONST	R9	K7
      0xB82A1200,  //  0015  GETNGBL	R10	K9
      0x8C28150A,  //  0016  GETMET	R10	R10	K10
      0x5C301000,  //  0017  MOVE	R12	R8
      0x58340007,  //  0018  LDCONST	R13	K7
      0x543A0063,  //  0019  LDINT	R14	100
      0x583C0007,  //  001A  LDCONST	R15	K7
      0x5C400600,  //  001B  MOVE	R16	R3
      0x7C280C00,  //  001C  CALL	R10	6
      0x282C1203,  //  001D  GE	R11	R9	R3
      0x782E0005,  //  001E  JMPF	R11	#0025
      0x0C2C1203,  //  001F  DIV	R11	R9	R3
      0x08341603,  //  0020  MUL	R13	R11	R3
      0x88300108,  //  0021  GETMBR	R12	R0	K8
      0x0030180D,  //  0022  ADD	R12	R12	R13
      0x9002100C,  //  0023  SETMBR	R0	K8	R12
      0x10241203,  //  0024  MOD	R9	R9	R3
      0x5C2C1200,  //  0025  MOVE	R11	R9
      0x24300F07,  //  0026  GT	R12	R7	K7
      0x7832000B,  //  0027  JMPF	R12	#0034
      0xB8321200,  //  0028  GETNGBL	R12	K9
      0x8C30190A,  //  0029  GETMET	R12	R12	K10
      0x5C380E00,  //  002A  MOVE	R14	R7
      0x583C0007,  //  002B  LDCONST	R15	K7
      0x54420063,  //  002C  LDINT	R16	100
      0x58440007,  //  002D  LDCONST	R17	K7
      0x5C480600,  //  002E  MOVE	R18	R3
      0x7C300C00,  //  002F  CALL	R12	6
      0x002C160C,  //  0030  ADD	R11	R11	R12
      0x28301603,  //  0031  GE	R12	R11	R3
      0x78320000,  //  0032  JMPF	R12	#0034
      0x042C1603,  //  0033  SUB	R11	R11	R3
      0xB8321600,  //  0034  GETNGBL	R12	K11
      0x8830190C,  //  0035  GETMBR	R12	R12	K12
      0x1C300C0C,  //  0036  EQ	R12	R6	R12
      0x78320009,  //  0037  JMPF	R12	#0042
      0xB8321200,  //  0038  GETNGBL	R12	K9
      0x8C30190E,  //  0039  GETMET	R12	R12	K14
      0x5C381600,  //  003A  MOVE	R14	R11
      0x583C0007,  //  003B  LDCONST	R15	K7
      0x0440070F,  //  003C  SUB	R16	R3	K15
      0x5C440800,  //  003D  MOVE	R17	R4
      0x5C480A00,  //  003E  MOVE	R18	R5
      0x7C300C00,  //  003F  CALL	R12	6
      0x90021A0C,  //  0040  SETMBR	R0	K13	R12
      0x7002016E,  //  0041  JMP		#01B1
      0xB8321600,  //  0042  GETNGBL	R12	K11
      0x88301910,  //  0043  GETMBR	R12	R12	K16
      0x1C300C0C,  //  0044  EQ	R12	R6	R12
      0x78320015,  //  0045  JMPF	R12	#005C
      0x1430160A,  //  0046  LT	R12	R11	R10
      0x78320009,  //  0047  JMPF	R12	#0052
      0xB8321200,  //  0048  GETNGBL	R12	K9
      0x8C30190E,  //  0049  GETMET	R12	R12	K14
      0x5C381600,  //  004A  MOVE	R14	R11
      0x583C0007,  //  004B  LDCONST	R15	K7
      0x0440150F,  //  004C  SUB	R16	R10	K15
      0x5C440800,  //  004D  MOVE	R17	R4
      0x5C480A00,  //  004E  MOVE	R18	R5
      0x7C300C00,  //  004F  CALL	R12	6
      0x90021A0C,  //  0050  SETMBR	R0	K13	R12
      0x70020008,  //  0051  JMP		#005B
      0xB8321200,  //  0052  GETNGBL	R12	K9
      0x8C30190E,  //  0053  GETMET	R12	R12	K14
      0x5C381600,  //  0054  MOVE	R14	R11
      0x5C3C1400,  //  0055  MOVE	R15	R10
      0x0440070F,  //  0056  SUB	R16	R3	K15
      0x5C440A00,  //  0057  MOVE	R17	R5
      0x5C480800,  //  0058  MOVE	R18	R4
      0x7C300C00,  //  0059  CALL	R12	6
      0x90021A0C,  //  005A  SETMBR	R0	K13	R12
      0x70020154,  //  005B  JMP		#01B1
      0xB8321600,  //  005C  GETNGBL	R12	K11
      0x88301911,  //  005D  GETMBR	R12	R12	K17
      0x1C300C0C,  //  005E  EQ	R12	R6	R12
      0x78320005,  //  005F  JMPF	R12	#0066
      0x1430160A,  //  0060  LT	R12	R11	R10
      0x78320001,  //  0061  JMPF	R12	#0064
      0x90021A04,  //  0062  SETMBR	R0	K13	R4
      0x70020000,  //  0063  JMP		#0065
      0x90021A05,  //  0064  SETMBR	R0	K13	R5
      0x7002014A,  //  0065  JMP		#01B1
      0xB8321600,  //  0066  GETNGBL	R12	K11
      0x88301912,  //  0067  GETMBR	R12	R12	K18
      0x1C300C0C,  //  0068  EQ	R12	R6	R12
      0x78320016,  //  0069  JMPF	R12	#0081
      0xB8321200,  //  006A  GETNGBL	R12	K9
      0x8C30190A,  //  006B  GETMET	R12	R12	K10
      0x5C381600,  //  006C  MOVE	R14	R11
      0x583C0007,  //  006D  LDCONST	R15	K7
      0x0440070F,  //  006E  SUB	R16	R3	K15
      0x58440007,  //  006F  LDCONST	R17	K7
      0x544A7FFE,  //  0070  LDINT	R18	32767
      0x7C300C00,  //  0071  CALL	R12	6
      0xB8361200,  //  0072  GETNGBL	R13	K9
      0x8C341B13,  //  0073  GETMET	R13	R13	K19
      0x543E1FFF,  //  0074  LDINT	R15	8192
      0x043C180F,  //  0075  SUB	R15	R12	R15
      0x7C340400,  //  0076  CALL	R13	2
      0xB83A1200,  //  0077  GETNGBL	R14	K9
      0x8C381D0E,  //  0078  GETMET	R14	R14	K14
      0x5C401A00,  //  0079  MOVE	R16	R13
      0x5445EFFF,  //  007A  LDINT	R17	-4096
      0x544A0FFF,  //  007B  LDINT	R18	4096
      0x5C4C0800,  //  007C  MOVE	R19	R4
      0x5C500A00,  //  007D  MOVE	R20	R5
      0x7C380C00,  //  007E  CALL	R14	6
      0x90021A0E,  //  007F  SETMBR	R0	K13	R14
      0x7002012F,  //  0080  JMP		#01B1
      0xB8321600,  //  0081  GETNGBL	R12	K11
      0x88301914,  //  0082  GETMBR	R12	R12	K20
      0x1C300C0C,  //  0083  EQ	R12	R6	R12
      0x78320015,  //  0084  JMPF	R12	#009B
      0xB8321200,  //  0085  GETNGBL	R12	K9
      0x8C30190A,  //  0086  GETMET	R12	R12	K10
      0x5C381600,  //  0087  MOVE	R14	R11
      0x583C0007,  //  0088  LDCONST	R15	K7
      0x0440070F,  //  0089  SUB	R16	R3	K15
      0x58440007,  //  008A  LDCONST	R17	K7
      0x544A7FFE,  //  008B  LDINT	R18	32767
      0x7C300C00,  //  008C  CALL	R12	6
      0xB8361200,  //  008D  GETNGBL	R13	K9
      0x8C341B13,  //  008E  GETMET	R13	R13	K19
      0x5C3C1800,  //  008F  MOVE	R15	R12
      0x7C340400,  //  0090  CALL	R13	2
      0xB83A1200,  //  0091  GETNGBL	R14	K9
      0x8C381D0E,  //  0092  GETMET	R14	R14	K14
      0x5C401A00,  //  0093  MOVE	R16	R13
      0x5445EFFF,  //  0094  LDINT	R17	-4096
      0x544A0FFF,  //  0095  LDINT	R18	4096
      0x5C4C0800,  //  0096  MOVE	R19	R4
      0x5C500A00,  //  0097  MOVE	R20	R5
      0x7C380C00,  //  0098  CALL	R14	6
      0x90021A0E,  //  0099  SETMBR	R0	K13	R14
      0x70020115,  //  009A  JMP		#01B1
      0xB8321600,  //  009B  GETNGBL	R12	K11
      0x88301915,  //  009C  GETMBR	R12	R12	K21
      0x1C300C0C,  //  009D  EQ	R12	R6	R12
      0x7832001B,  //  009E  JMPF	R12	#00BB
      0xB8321200,  //  009F  GETNGBL	R12	K9
      0x8C30190A,  //  00A0  GETMET	R12	R12	K10
      0x5C381600,  //  00A1  MOVE	R14	R11
      0x583C0007,  //  00A2  LDCONST	R15	K7
      0x0440070F,  //  00A3  SUB	R16	R3	K15
      0x58440007,  //  00A4  LDCONST	R17	K7
      0x544A00FE,  //  00A5  LDINT	R18	255
      0x7C300C00,  //  00A6  CALL	R12	6
      0xB8361200,  //  00A7  GETNGBL	R13	K9
      0x8C341B0E,  //  00A8  GETMET	R13	R13	K14
      0x083C180C,  //  00A9  MUL	R15	R12	R12
      0x58400007,  //  00AA  LDCONST	R16	K7
      0x544600FE,  //  00AB  LDINT	R17	255
      0x544A00FE,  //  00AC  LDINT	R18	255
      0x08442212,  //  00AD  MUL	R17	R17	R18
      0x58480007,  //  00AE  LDCONST	R18	K7
      0x544E00FE,  //  00AF  LDINT	R19	255
      0x7C340C00,  //  00B0  CALL	R13	6
      0xB83A1200,  //  00B1  GETNGBL	R14	K9
      0x8C381D0E,  //  00B2  GETMET	R14	R14	K14
      0x5C401A00,  //  00B3  MOVE	R16	R13
      0x58440007,  //  00B4  LDCONST	R17	K7
      0x544A00FE,  //  00B5  LDINT	R18	255
      0x5C4C0800,  //  00B6  MOVE	R19	R4
      0x5C500A00,  //  00B7  MOVE	R20	R5
      0x7C380C00,  //  00B8  CALL	R14	6
      0x90021A0E,  //  00B9  SETMBR	R0	K13	R14
      0x700200F5,  //  00BA  JMP		#01B1
      0xB8321600,  //  00BB  GETNGBL	R12	K11
      0x88301916,  //  00BC  GETMBR	R12	R12	K22
      0x1C300C0C,  //  00BD  EQ	R12	R6	R12
      0x7832001F,  //  00BE  JMPF	R12	#00DF
      0xB8321200,  //  00BF  GETNGBL	R12	K9
      0x8C30190A,  //  00C0  GETMET	R12	R12	K10
      0x5C381600,  //  00C1  MOVE	R14	R11
      0x583C0007,  //  00C2  LDCONST	R15	K7
      0x0440070F,  //  00C3  SUB	R16	R3	K15
      0x58440007,  //  00C4  LDCONST	R17	K7
      0x544A00FE,  //  00C5  LDINT	R18	255
      0x7C300C00,  //  00C6  CALL	R12	6
      0x543600FE,  //  00C7  LDINT	R13	255
      0x04341A0C,  //  00C8  SUB	R13	R13	R12
      0x543A00FE,  //  00C9  LDINT	R14	255
      0xB83E1200,  //  00CA  GETNGBL	R15	K9
      0x8C3C1F0E,  //  00CB  GETMET	R15	R15	K14
      0x08441A0D,  //  00CC  MUL	R17	R13	R13
      0x58480007,  //  00CD  LDCONST	R18	K7
      0x544E00FE,  //  00CE  LDINT	R19	255
      0x545200FE,  //  00CF  LDINT	R20	255
      0x084C2614,  //  00D0  MUL	R19	R19	R20
      0x58500007,  //  00D1  LDCONST	R20	K7
      0x545600FE,  //  00D2  LDINT	R21	255
      0x7C3C0C00,  //  00D3  CALL	R15	6
      0x04381C0F,  //  00D4  SUB	R14	R14	R15
      0xB83E1200,  //  00D5  GETNGBL	R15	K9
      0x8C3C1F0E,  //  00D6  GETMET	R15	R15	K14
      0x5C441C00,  //  00D7  MOVE	R17	R14
      0x58480007,  //  00D8  LDCONST	R18	K7
      0x544E00FE,  //  00D9  LDINT	R19	255
      0x5C500800,  //  00DA  MOVE	R20	R4
      0x5C540A00,  //  00DB  MOVE	R21	R5
      0x7C3C0C00,  //  00DC  CALL	R15	6
      0x90021A0F,  //  00DD  SETMBR	R0	K13	R15
      0x700200D1,  //  00DE  JMP		#01B1
      0xB8321600,  //  00DF  GETNGBL	R12	K11
      0x88301917,  //  00E0  GETMBR	R12	R12	K23
      0x1C300C0C,  //  00E1  EQ	R12	R6	R12
      0x78320055,  //  00E2  JMPF	R12	#0139
      0xB8321200,  //  00E3  GETNGBL	R12	K9
      0x8C30190A,  //  00E4  GETMET	R12	R12	K10
      0x5C381600,  //  00E5  MOVE	R14	R11
      0x583C0007,  //  00E6  LDCONST	R15	K7
      0x0440070F,  //  00E7  SUB	R16	R3	K15
      0x58440007,  //  00E8  LDCONST	R17	K7
      0x544A00FE,  //  00E9  LDINT	R18	255
      0x7C300C00,  //  00EA  CALL	R12	6
      0x1C341907,  //  00EB  EQ	R13	R12	K7
      0x78360001,  //  00EC  JMPF	R13	#00EF
      0x90021A04,  //  00ED  SETMBR	R0	K13	R4
      0x70020048,  //  00EE  JMP		#0138
      0x543600FE,  //  00EF  LDINT	R13	255
      0x1C34180D,  //  00F0  EQ	R13	R12	R13
      0x78360001,  //  00F1  JMPF	R13	#00F4
      0x90021A05,  //  00F2  SETMBR	R0	K13	R5
      0x70020043,  //  00F3  JMP		#0138
      0xB8361200,  //  00F4  GETNGBL	R13	K9
      0x8C341B0A,  //  00F5  GETMET	R13	R13	K10
      0x543E00FE,  //  00F6  LDINT	R15	255
      0x043C1E0C,  //  00F7  SUB	R15	R15	R12
      0x58400007,  //  00F8  LDCONST	R16	K7
      0x544600FE,  //  00F9  LDINT	R17	255
      0x544A00FE,  //  00FA  LDINT	R18	255
      0x544E001F,  //  00FB  LDINT	R19	32
      0x7C340C00,  //  00FC  CALL	R13	6
      0xB83A1200,  //  00FD  GETNGBL	R14	K9
      0x8C381D0A,  //  00FE  GETMET	R14	R14	K10
      0x5C401800,  //  00FF  MOVE	R16	R12
      0x58440007,  //  0100  LDCONST	R17	K7
      0x544A00FE,  //  0101  LDINT	R18	255
      0x584C0007,  //  0102  LDCONST	R19	K7
      0x54527FFE,  //  0103  LDINT	R20	32767
      0x54560005,  //  0104  LDINT	R21	6
      0x08502815,  //  0105  MUL	R20	R20	R21
      0x7C380C00,  //  0106  CALL	R14	6
      0xB83E1200,  //  0107  GETNGBL	R15	K9
      0x8C3C1F13,  //  0108  GETMET	R15	R15	K19
      0x54467FFE,  //  0109  LDINT	R17	32767
      0x10441C11,  //  010A  MOD	R17	R14	R17
      0x7C3C0400,  //  010B  CALL	R15	2
      0xB8421200,  //  010C  GETNGBL	R16	K9
      0x8C40210E,  //  010D  GETMET	R16	R16	K14
      0x08481E0D,  //  010E  MUL	R18	R15	R13
      0x544DEFFF,  //  010F  LDINT	R19	-4096
      0x545200FE,  //  0110  LDINT	R20	255
      0x084C2614,  //  0111  MUL	R19	R19	R20
      0x54520FFF,  //  0112  LDINT	R20	4096
      0x545600FE,  //  0113  LDINT	R21	255
      0x08502815,  //  0114  MUL	R20	R20	R21
      0x5455FF00,  //  0115  LDINT	R21	-255
      0x545A00FE,  //  0116  LDINT	R22	255
      0x7C400C00,  //  0117  CALL	R16	6
      0xB8461200,  //  0118  GETNGBL	R17	K9
      0x8C44230E,  //  0119  GETMET	R17	R17	K14
      0x5C4C1800,  //  011A  MOVE	R19	R12
      0x58500007,  //  011B  LDCONST	R20	K7
      0x545600FE,  //  011C  LDINT	R21	255
      0x58580007,  //  011D  LDCONST	R22	K7
      0x045C0A04,  //  011E  SUB	R23	R5	R4
      0x7C440C00,  //  011F  CALL	R17	6
      0x00480811,  //  0120  ADD	R18	R4	R17
      0x00482410,  //  0121  ADD	R18	R18	R16
      0x90021A12,  //  0122  SETMBR	R0	K13	R18
      0x04480A04,  //  0123  SUB	R18	R5	R4
      0xB84E1200,  //  0124  GETNGBL	R19	K9
      0x8C4C270E,  //  0125  GETMET	R19	R19	K14
      0x5C542400,  //  0126  MOVE	R21	R18
      0x58580007,  //  0127  LDCONST	R22	K7
      0x545E0003,  //  0128  LDINT	R23	4
      0x58600007,  //  0129  LDCONST	R24	K7
      0x5864000F,  //  012A  LDCONST	R25	K15
      0x7C4C0C00,  //  012B  CALL	R19	6
      0x8850010D,  //  012C  GETMBR	R20	R0	K13
      0x00540A13,  //  012D  ADD	R21	R5	R19
      0x24502815,  //  012E  GT	R20	R20	R21
      0x78520001,  //  012F  JMPF	R20	#0132
      0x00500A13,  //  0130  ADD	R20	R5	R19
      0x90021A14,  //  0131  SETMBR	R0	K13	R20
      0x8850010D,  //  0132  GETMBR	R20	R0	K13
      0x04540813,  //  0133  SUB	R21	R4	R19
      0x14502815,  //  0134  LT	R20	R20	R21
      0x78520001,  //  0135  JMPF	R20	#0138
      0x04500813,  //  0136  SUB	R20	R4	R19
      0x90021A14,  //  0137  SETMBR	R0	K13	R20
      0x70020077,  //  0138  JMP		#01B1
      0xB8321600,  //  0139  GETNGBL	R12	K11
      0x88301918,  //  013A  GETMBR	R12	R12	K24
      0x1C300C0C,  //  013B  EQ	R12	R6	R12
      0x78320073,  //  013C  JMPF	R12	#01B1
      0xB8321200,  //  013D  GETNGBL	R12	K9
      0x8C30190A,  //  013E  GETMET	R12	R12	K10
      0x5C381600,  //  013F  MOVE	R14	R11
      0x583C0007,  //  0140  LDCONST	R15	K7
      0x0440070F,  //  0141  SUB	R16	R3	K15
      0x58440007,  //  0142  LDCONST	R17	K7
      0x544A00FE,  //  0143  LDINT	R18	255
      0x7C300C00,  //  0144  CALL	R12	6
      0x58340007,  //  0145  LDCONST	R13	K7
      0x543A007F,  //  0146  LDINT	R14	128
      0x1438180E,  //  0147  LT	R14	R12	R14
      0x783A0017,  //  0148  JMPF	R14	#0161
      0xB83A1200,  //  0149  GETNGBL	R14	K9
      0x8C381D0A,  //  014A  GETMET	R14	R14	K10
      0x5C401800,  //  014B  MOVE	R16	R12
      0x58440007,  //  014C  LDCONST	R17	K7
      0x544A007E,  //  014D  LDINT	R18	127
      0x584C0007,  //  014E  LDCONST	R19	K7
      0x545200FE,  //  014F  LDINT	R20	255
      0x7C380C00,  //  0150  CALL	R14	6
      0x543E00FE,  //  0151  LDINT	R15	255
      0x043C1E0E,  //  0152  SUB	R15	R15	R14
      0x544200FE,  //  0153  LDINT	R16	255
      0xB8461200,  //  0154  GETNGBL	R17	K9
      0x8C44230E,  //  0155  GETMET	R17	R17	K14
      0x084C1E0F,  //  0156  MUL	R19	R15	R15
      0x58500007,  //  0157  LDCONST	R20	K7
      0x545600FE,  //  0158  LDINT	R21	255
      0x545A00FE,  //  0159  LDINT	R22	255
      0x08542A16,  //  015A  MUL	R21	R21	R22
      0x58580007,  //  015B  LDCONST	R22	K7
      0x545E00FE,  //  015C  LDINT	R23	255
      0x7C440C00,  //  015D  CALL	R17	6
      0x04402011,  //  015E  SUB	R16	R16	R17
      0x5C342000,  //  015F  MOVE	R13	R16
      0x70020046,  //  0160  JMP		#01A8
      0x543A00BF,  //  0161  LDINT	R14	192
      0x1438180E,  //  0162  LT	R14	R12	R14
      0x783A0020,  //  0163  JMPF	R14	#0185
      0xB83A1200,  //  0164  GETNGBL	R14	K9
      0x8C381D0A,  //  0165  GETMET	R14	R14	K10
      0x5442007F,  //  0166  LDINT	R16	128
      0x04401810,  //  0167  SUB	R16	R12	R16
      0x58440007,  //  0168  LDCONST	R17	K7
      0x544A003E,  //  0169  LDINT	R18	63
      0x584C0007,  //  016A  LDCONST	R19	K7
      0x545200FE,  //  016B  LDINT	R20	255
      0x7C380C00,  //  016C  CALL	R14	6
      0x543E00FE,  //  016D  LDINT	R15	255
      0x043C1E0E,  //  016E  SUB	R15	R15	R14
      0x544200FE,  //  016F  LDINT	R16	255
      0xB8461200,  //  0170  GETNGBL	R17	K9
      0x8C44230E,  //  0171  GETMET	R17	R17	K14
      0x084C1E0F,  //  0172  MUL	R19	R15	R15
      0x58500007,  //  0173  LDCONST	R20	K7
      0x545600FE,  //  0174  LDINT	R21	255
      0x545A00FE,  //  0175  LDINT	R22	255
      0x08542A16,  //  0176  MUL	R21	R21	R22
      0x58580007,  //  0177  LDCONST	R22	K7
      0x545E00FE,  //  0178  LDINT	R23	255
      0x7C440C00,  //  0179  CALL	R17	6
      0x04402011,  //  017A  SUB	R16	R16	R17
      0xB8461200,  //  017B  GETNGBL	R17	K9
      0x8C44230E,  //  017C  GETMET	R17	R17	K14
      0x5C4C2000,  //  017D  MOVE	R19	R16
      0x58500007,  //  017E  LDCONST	R20	K7
      0x545600FE,  //  017F  LDINT	R21	255
      0x58580007,  //  0180  LDCONST	R22	K7
      0x545E007F,  //  0181  LDINT	R23	128
      0x7C440C00,  //  0182  CALL	R17	6
      0x5C342200,  //  0183  MOVE	R13	R17
      0x70020022,  //  0184  JMP		#01A8
      0xB83A1200,  //  0185  GETNGBL	R14	K9
      0x8C381D0A,  //  0186  GETMET	R14	R14	K10
      0x544200BF,  //  0187  LDINT	R16	192
      0x04401810,  //  0188  SUB	R16	R12	R16
      0x58440007,  //  0189  LDCONST	R17	K7
      0x544A003E,  //  018A  LDINT	R18	63
      0x584C0007,  //  018B  LDCONST	R19	K7
      0x545200FE,  //  018C  LDINT	R20	255
      0x7C380C00,  //  018D  CALL	R14	6
      0x543E00FE,  //  018E  LDINT	R15	255
      0x043C1E0E,  //  018F  SUB	R15	R15	R14
      0x544200FE,  //  0190  LDINT	R16	255
      0xB8461200,  //  0191  GETNGBL	R17	K9
      0x8C44230E,  //  0192  GETMET	R17	R17	K14
      0x084C1E0F,  //  0193  MUL	R19	R15	R15
      0x58500007,  //  0194  LDCONST	R20	K7
      0x545600FE,  //  0195  LDINT	R21	255
      0x545A00FE,  //  0196  LDINT	R22	255
      0x08542A16,  //  0197  MUL	R21	R21	R22
      0x58580007,  //  0198  LDCONST	R22	K7
      0x545E00FE,  //  0199  LDINT	R23	255
      0x7C440C00,  //  019A  CALL	R17	6
      0x04402011,  //  019B  SUB	R16	R16	R17
      0x544600FE,  //  019C  LDINT	R17	255
      0xB84A1200,  //  019D  GETNGBL	R18	K9
      0x8C48250E,  //  019E  GETMET	R18	R18	K14
      0x545200FE,  //  019F  LDINT	R20	255
      0x04502810,  //  01A0  SUB	R20	R20	R16
      0x58540007,  //  01A1  LDCONST	R21	K7
      0x545A00FE,  //  01A2  LDINT	R22	255
      0x585C0007,  //  01A3  LDCONST	R23	K7
      0x5462003F,  //  01A4  LDINT	R24	64
      0x7C480C00,  //  01A5  CALL	R18	6
      0x04442212,  //  01A6  SUB	R17	R17	R18
      0x5C342200,  //  01A7  MOVE	R13	R17
      0xB83A1200,  //  01A8  GETNGBL	R14	K9
      0x8C381D0E,  //  01A9  GETMET	R14	R14	K14
      0x5C401A00,  //  01AA  MOVE	R16	R13
      0x58440007,  //  01AB  LDCONST	R17	K7
      0x544A00FE,  //  01AC  LDINT	R18	255
      0x5C4C0800,  //  01AD  MOVE	R19	R4
      0x5C500A00,  //  01AE  MOVE	R20	R5
      0x7C380C00,  //  01AF  CALL	R14	6
      0x90021A0E,  //  01B0  SETMBR	R0	K13	R14
      0x8830010D,  //  01B1  GETMBR	R12	R0	K13
      0x80041800,  //  01B2  RET	1	R12
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
      0x2804030F,  //  0001  GE	R1	R1	K15
      0x78060007,  //  0002  JMPF	R1	#000B
      0x88040103,  //  0003  GETMBR	R1	R0	K3
      0x540A0008,  //  0004  LDINT	R2	9
      0x18040202,  //  0005  LE	R1	R1	R2
      0x78060003,  //  0006  JMPF	R1	#000B
      0x88040119,  //  0007  GETMBR	R1	R0	K25
      0x88080103,  //  0008  GETMBR	R2	R0	K3
      0x94040202,  //  0009  GETIDX	R1	R1	R2
      0x70020000,  //  000A  JMP		#000C
      0x5804001A,  //  000B  LDCONST	R1	K26
      0x60080018,  //  000C  GETGBL	R2	G24
      0x580C001B,  //  000D  LDCONST	R3	K27
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
    ( &(const binstruction[ 8]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C08051C,  //  0003  GETMET	R2	R2	K28
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x90021B07,  //  0006  SETMBR	R0	K13	K7
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(class_OscillatorValueProvider_start,   /* name */
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
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C08051D,  //  0003  GETMET	R2	R2	K29
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
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
    1,
    &be_class_ValueProvider,
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(PARAMS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(phase, -1), be_const_bytes_instance(07000000640000) },
        { be_const_key_weak(max_value, 4), be_const_bytes_instance(040064) },
        { be_const_key_weak(duty_cycle, -1), be_const_bytes_instance(07000000640032) },
        { be_const_key_weak(min_value, -1), be_const_bytes_instance(040000) },
        { be_const_key_weak(duration, -1), be_const_bytes_instance(05000101E803) },
        { be_const_key_weak(form, 1), be_const_bytes_instance(14000109000100020003000400050006000700080009) },
    }))    ) } )) },
        { be_const_key_weak(produce_value, -1), be_const_closure(class_OscillatorValueProvider_produce_value_closure) },
        { be_const_key_weak(form_names, 4), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
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
        { be_const_key_weak(tostring, -1), be_const_closure(class_OscillatorValueProvider_tostring_closure) },
        { be_const_key_weak(init, 0), be_const_closure(class_OscillatorValueProvider_init_closure) },
        { be_const_key_weak(value, 2), be_const_var(0) },
        { be_const_key_weak(start, -1), be_const_closure(class_OscillatorValueProvider_start_closure) },
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

// compact class 'TwinkleAnimation' ktab size: 43, total: 74 (saved 248 bytes)
static const bvalue be_ktab_class_TwinkleAnimation[43] = {
  /* K0   */  be_nested_str_weak(on_param_changed),
  /* K1   */  be_nested_str_weak(twinkle_speed),
  /* K2   */  be_const_int(1),
  /* K3   */  be_nested_str_weak(set_param),
  /* K4   */  be_nested_str_weak(fade_speed),
  /* K5   */  be_nested_str_weak(density),
  /* K6   */  be_nested_str_weak(min_brightness),
  /* K7   */  be_nested_str_weak(max_brightness),
  /* K8   */  be_nested_str_weak(color),
  /* K9   */  be_nested_str_weak(engine),
  /* K10  */  be_nested_str_weak(get_strip_length),
  /* K11  */  be_nested_str_weak(twinkle_states),
  /* K12  */  be_nested_str_weak(current_colors),
  /* K13  */  be_nested_str_weak(size),
  /* K14  */  be_nested_str_weak(_initialize_arrays),
  /* K15  */  be_const_int(0),
  /* K16  */  be_nested_str_weak(get),
  /* K17  */  be_nested_str_weak(tasmota),
  /* K18  */  be_nested_str_weak(scale_uint),
  /* K19  */  be_nested_str_weak(set),
  /* K20  */  be_const_int(16777215),
  /* K21  */  be_nested_str_weak(_random_range),
  /* K22  */  be_nested_str_weak(get_param),
  /* K23  */  be_nested_str_weak(animation),
  /* K24  */  be_nested_str_weak(is_value_provider),
  /* K25  */  be_nested_str_weak(0x_X2508x),
  /* K26  */  be_nested_str_weak(TwinkleAnimation_X28color_X3D_X25s_X2C_X20density_X3D_X25s_X2C_X20twinkle_speed_X3D_X25s_X2C_X20priority_X3D_X25s_X2C_X20running_X3D_X25s_X29),
  /* K27  */  be_nested_str_weak(priority),
  /* K28  */  be_nested_str_weak(is_running),
  /* K29  */  be_nested_str_weak(init),
  /* K30  */  be_nested_str_weak(last_update),
  /* K31  */  be_nested_str_weak(random_seed),
  /* K32  */  be_nested_str_weak(time_ms),
  /* K33  */  be_nested_str_weak(_fix_time_ms),
  /* K34  */  be_nested_str_weak(width),
  /* K35  */  be_nested_str_weak(set_pixel_color),
  /* K36  */  be_nested_str_weak(update),
  /* K37  */  be_nested_str_weak(_update_twinkle_simulation),
  /* K38  */  be_nested_str_weak(resize),
  /* K39  */  be_nested_str_weak(clear),
  /* K40  */  be_const_int(1103515245),
  /* K41  */  be_const_int(2147483647),
  /* K42  */  be_nested_str_weak(_random),
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
    ( &(const binstruction[27]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0700,  //  0003  GETMET	R3	R3	K0
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x1C0C0301,  //  0007  EQ	R3	R1	K1
      0x780E0010,  //  0008  JMPF	R3	#001A
      0x540E0031,  //  0009  LDINT	R3	50
      0x280C0403,  //  000A  GE	R3	R2	R3
      0x780E000D,  //  000B  JMPF	R3	#001A
      0x540E03E7,  //  000C  LDINT	R3	1000
      0x0C0C0602,  //  000D  DIV	R3	R3	R2
      0x14100702,  //  000E  LT	R4	R3	K2
      0x78120001,  //  000F  JMPF	R4	#0012
      0x580C0002,  //  0010  LDCONST	R3	K2
      0x70020003,  //  0011  JMP		#0016
      0x54120013,  //  0012  LDINT	R4	20
      0x24100604,  //  0013  GT	R4	R3	R4
      0x78120000,  //  0014  JMPF	R4	#0016
      0x540E0013,  //  0015  LDINT	R3	20
      0x8C100103,  //  0016  GETMET	R4	R0	K3
      0x58180001,  //  0017  LDCONST	R6	K1
      0x5C1C0600,  //  0018  MOVE	R7	R3
      0x7C100600,  //  0019  CALL	R4	3
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _update_twinkle_simulation
********************************************************************/
be_local_closure(class_TwinkleAnimation__update_twinkle_simulation,   /* name */
  be_nested_proto(
    20,                          /* nstack */
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
    ( &(const binstruction[118]) {  /* code */
      0x88080104,  //  0000  GETMBR	R2	R0	K4
      0x880C0105,  //  0001  GETMBR	R3	R0	K5
      0x88100106,  //  0002  GETMBR	R4	R0	K6
      0x88140107,  //  0003  GETMBR	R5	R0	K7
      0x88180108,  //  0004  GETMBR	R6	R0	K8
      0x881C0109,  //  0005  GETMBR	R7	R0	K9
      0x8C1C0F0A,  //  0006  GETMET	R7	R7	K10
      0x7C1C0200,  //  0007  CALL	R7	1
      0x6020000C,  //  0008  GETGBL	R8	G12
      0x8824010B,  //  0009  GETMBR	R9	R0	K11
      0x7C200200,  //  000A  CALL	R8	1
      0x20201007,  //  000B  NE	R8	R8	R7
      0x74220006,  //  000C  JMPT	R8	#0014
      0x8820010C,  //  000D  GETMBR	R8	R0	K12
      0x8C20110D,  //  000E  GETMET	R8	R8	K13
      0x7C200200,  //  000F  CALL	R8	1
      0x54260003,  //  0010  LDINT	R9	4
      0x08240E09,  //  0011  MUL	R9	R7	R9
      0x20201009,  //  0012  NE	R8	R8	R9
      0x78220001,  //  0013  JMPF	R8	#0016
      0x8C20010E,  //  0014  GETMET	R8	R0	K14
      0x7C200200,  //  0015  CALL	R8	1
      0x5820000F,  //  0016  LDCONST	R8	K15
      0x14241007,  //  0017  LT	R9	R8	R7
      0x7826002C,  //  0018  JMPF	R9	#0046
      0x8824010C,  //  0019  GETMBR	R9	R0	K12
      0x8C241310,  //  001A  GETMET	R9	R9	K16
      0x542E0003,  //  001B  LDINT	R11	4
      0x082C100B,  //  001C  MUL	R11	R8	R11
      0x5431FFFB,  //  001D  LDINT	R12	-4
      0x7C240600,  //  001E  CALL	R9	3
      0x542A0017,  //  001F  LDINT	R10	24
      0x3C28120A,  //  0020  SHR	R10	R9	R10
      0x542E00FE,  //  0021  LDINT	R11	255
      0x2C28140B,  //  0022  AND	R10	R10	R11
      0x242C150F,  //  0023  GT	R11	R10	K15
      0x782E001E,  //  0024  JMPF	R11	#0044
      0xB82E2200,  //  0025  GETNGBL	R11	K17
      0x8C2C1712,  //  0026  GETMET	R11	R11	K18
      0x5C340400,  //  0027  MOVE	R13	R2
      0x5838000F,  //  0028  LDCONST	R14	K15
      0x543E00FE,  //  0029  LDINT	R15	255
      0x58400002,  //  002A  LDCONST	R16	K2
      0x54460013,  //  002B  LDINT	R17	20
      0x7C2C0C00,  //  002C  CALL	R11	6
      0x1830140B,  //  002D  LE	R12	R10	R11
      0x78320009,  //  002E  JMPF	R12	#0039
      0x8830010B,  //  002F  GETMBR	R12	R0	K11
      0x9830110F,  //  0030  SETIDX	R12	R8	K15
      0x8830010C,  //  0031  GETMBR	R12	R0	K12
      0x8C301913,  //  0032  GETMET	R12	R12	K19
      0x543A0003,  //  0033  LDINT	R14	4
      0x0838100E,  //  0034  MUL	R14	R8	R14
      0x583C000F,  //  0035  LDCONST	R15	K15
      0x5441FFFB,  //  0036  LDINT	R16	-4
      0x7C300800,  //  0037  CALL	R12	4
      0x7002000A,  //  0038  JMP		#0044
      0x0430140B,  //  0039  SUB	R12	R10	R11
      0x2C341314,  //  003A  AND	R13	R9	K20
      0x8838010C,  //  003B  GETMBR	R14	R0	K12
      0x8C381D13,  //  003C  GETMET	R14	R14	K19
      0x54420003,  //  003D  LDINT	R16	4
      0x08401010,  //  003E  MUL	R16	R8	R16
      0x54460017,  //  003F  LDINT	R17	24
      0x38441811,  //  0040  SHL	R17	R12	R17
      0x3044220D,  //  0041  OR	R17	R17	R13
      0x5449FFFB,  //  0042  LDINT	R18	-4
      0x7C380800,  //  0043  CALL	R14	4
      0x00201102,  //  0044  ADD	R8	R8	K2
      0x7001FFD0,  //  0045  JMP		#0017
      0x5824000F,  //  0046  LDCONST	R9	K15
      0x14281207,  //  0047  LT	R10	R9	R7
      0x782A002B,  //  0048  JMPF	R10	#0075
      0x8828010B,  //  0049  GETMBR	R10	R0	K11
      0x94281409,  //  004A  GETIDX	R10	R10	R9
      0x1C28150F,  //  004B  EQ	R10	R10	K15
      0x782A0025,  //  004C  JMPF	R10	#0073
      0x8C280115,  //  004D  GETMET	R10	R0	K21
      0x543200FE,  //  004E  LDINT	R12	255
      0x7C280400,  //  004F  CALL	R10	2
      0x14281403,  //  0050  LT	R10	R10	R3
      0x782A0020,  //  0051  JMPF	R10	#0073
      0x8C280115,  //  0052  GETMET	R10	R0	K21
      0x04300A04,  //  0053  SUB	R12	R5	R4
      0x00301902,  //  0054  ADD	R12	R12	K2
      0x7C280400,  //  0055  CALL	R10	2
      0x0028080A,  //  0056  ADD	R10	R4	R10
      0x5C2C0C00,  //  0057  MOVE	R11	R6
      0x5432000F,  //  0058  LDINT	R12	16
      0x3C30160C,  //  0059  SHR	R12	R11	R12
      0x543600FE,  //  005A  LDINT	R13	255
      0x2C30180D,  //  005B  AND	R12	R12	R13
      0x54360007,  //  005C  LDINT	R13	8
      0x3C34160D,  //  005D  SHR	R13	R11	R13
      0x543A00FE,  //  005E  LDINT	R14	255
      0x2C341A0E,  //  005F  AND	R13	R13	R14
      0x543A00FE,  //  0060  LDINT	R14	255
      0x2C38160E,  //  0061  AND	R14	R11	R14
      0x883C010B,  //  0062  GETMBR	R15	R0	K11
      0x983C1302,  //  0063  SETIDX	R15	R9	K2
      0x883C010C,  //  0064  GETMBR	R15	R0	K12
      0x8C3C1F13,  //  0065  GETMET	R15	R15	K19
      0x54460003,  //  0066  LDINT	R17	4
      0x08441211,  //  0067  MUL	R17	R9	R17
      0x544A0017,  //  0068  LDINT	R18	24
      0x38481412,  //  0069  SHL	R18	R10	R18
      0x544E000F,  //  006A  LDINT	R19	16
      0x384C1813,  //  006B  SHL	R19	R12	R19
      0x30482413,  //  006C  OR	R18	R18	R19
      0x544E0007,  //  006D  LDINT	R19	8
      0x384C1A13,  //  006E  SHL	R19	R13	R19
      0x30482413,  //  006F  OR	R18	R18	R19
      0x3048240E,  //  0070  OR	R18	R18	R14
      0x544DFFFB,  //  0071  LDINT	R19	-4
      0x7C3C0800,  //  0072  CALL	R15	4
      0x00241302,  //  0073  ADD	R9	R9	K2
      0x7001FFD1,  //  0074  JMP		#0047
      0x80000000,  //  0075  RET	0
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
      0x8C080116,  //  0001  GETMET	R2	R0	K22
      0x58100008,  //  0002  LDCONST	R4	K8
      0x7C080400,  //  0003  CALL	R2	2
      0xB80E2E00,  //  0004  GETNGBL	R3	K23
      0x8C0C0718,  //  0005  GETMET	R3	R3	K24
      0x5C140400,  //  0006  MOVE	R5	R2
      0x7C0C0400,  //  0007  CALL	R3	2
      0x780E0004,  //  0008  JMPF	R3	#000E
      0x600C0008,  //  0009  GETGBL	R3	G8
      0x5C100400,  //  000A  MOVE	R4	R2
      0x7C0C0200,  //  000B  CALL	R3	1
      0x5C040600,  //  000C  MOVE	R1	R3
      0x70020004,  //  000D  JMP		#0013
      0x600C0018,  //  000E  GETGBL	R3	G24
      0x58100019,  //  000F  LDCONST	R4	K25
      0x88140108,  //  0010  GETMBR	R5	R0	K8
      0x7C0C0400,  //  0011  CALL	R3	2
      0x5C040600,  //  0012  MOVE	R1	R3
      0x600C0018,  //  0013  GETGBL	R3	G24
      0x5810001A,  //  0014  LDCONST	R4	K26
      0x5C140200,  //  0015  MOVE	R5	R1
      0x88180105,  //  0016  GETMBR	R6	R0	K5
      0x881C0101,  //  0017  GETMBR	R7	R0	K1
      0x8820011B,  //  0018  GETMBR	R8	R0	K27
      0x8824011C,  //  0019  GETMBR	R9	R0	K28
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
      0x8C08051D,  //  0003  GETMET	R2	R2	K29
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x60080012,  //  0006  GETGBL	R2	G18
      0x7C080000,  //  0007  CALL	R2	0
      0x90021602,  //  0008  SETMBR	R0	K11	R2
      0x60080015,  //  0009  GETGBL	R2	G21
      0x7C080000,  //  000A  CALL	R2	0
      0x90021802,  //  000B  SETMBR	R0	K12	R2
      0x90023D0F,  //  000C  SETMBR	R0	K30	K15
      0x88080109,  //  000D  GETMBR	R2	R0	K9
      0x88080520,  //  000E  GETMBR	R2	R2	K32
      0x540EFFFF,  //  000F  LDINT	R3	65536
      0x10080403,  //  0010  MOD	R2	R2	R3
      0x90023E02,  //  0011  SETMBR	R0	K31	R2
      0x8C08010E,  //  0012  GETMET	R2	R0	K14
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
    ( &(const binstruction[55]) {  /* code */
      0x880C011C,  //  0000  GETMBR	R3	R0	K28
      0x780E0002,  //  0001  JMPF	R3	#0005
      0x4C0C0000,  //  0002  LDNIL	R3
      0x1C0C0203,  //  0003  EQ	R3	R1	R3
      0x780E0001,  //  0004  JMPF	R3	#0007
      0x500C0000,  //  0005  LDBOOL	R3	0	0
      0x80040600,  //  0006  RET	1	R3
      0x8C0C0121,  //  0007  GETMET	R3	R0	K33
      0x5C140400,  //  0008  MOVE	R5	R2
      0x7C0C0400,  //  0009  CALL	R3	2
      0x5C080600,  //  000A  MOVE	R2	R3
      0x880C0109,  //  000B  GETMBR	R3	R0	K9
      0x8C0C070A,  //  000C  GETMET	R3	R3	K10
      0x7C0C0200,  //  000D  CALL	R3	1
      0x6010000C,  //  000E  GETGBL	R4	G12
      0x8814010B,  //  000F  GETMBR	R5	R0	K11
      0x7C100200,  //  0010  CALL	R4	1
      0x20100803,  //  0011  NE	R4	R4	R3
      0x74120006,  //  0012  JMPT	R4	#001A
      0x8810010C,  //  0013  GETMBR	R4	R0	K12
      0x8C10090D,  //  0014  GETMET	R4	R4	K13
      0x7C100200,  //  0015  CALL	R4	1
      0x54160003,  //  0016  LDINT	R5	4
      0x08140605,  //  0017  MUL	R5	R3	R5
      0x20100805,  //  0018  NE	R4	R4	R5
      0x78120001,  //  0019  JMPF	R4	#001C
      0x8C10010E,  //  001A  GETMET	R4	R0	K14
      0x7C100200,  //  001B  CALL	R4	1
      0x50100000,  //  001C  LDBOOL	R4	0	0
      0x5814000F,  //  001D  LDCONST	R5	K15
      0x14180A03,  //  001E  LT	R6	R5	R3
      0x781A0015,  //  001F  JMPF	R6	#0036
      0x88180322,  //  0020  GETMBR	R6	R1	K34
      0x14180A06,  //  0021  LT	R6	R5	R6
      0x781A0010,  //  0022  JMPF	R6	#0034
      0x8818010C,  //  0023  GETMBR	R6	R0	K12
      0x8C180D10,  //  0024  GETMET	R6	R6	K16
      0x54220003,  //  0025  LDINT	R8	4
      0x08200A08,  //  0026  MUL	R8	R5	R8
      0x5425FFFB,  //  0027  LDINT	R9	-4
      0x7C180600,  //  0028  CALL	R6	3
      0x541E0017,  //  0029  LDINT	R7	24
      0x3C1C0C07,  //  002A  SHR	R7	R6	R7
      0x542200FE,  //  002B  LDINT	R8	255
      0x2C1C0E08,  //  002C  AND	R7	R7	R8
      0x241C0F0F,  //  002D  GT	R7	R7	K15
      0x781E0004,  //  002E  JMPF	R7	#0034
      0x8C1C0323,  //  002F  GETMET	R7	R1	K35
      0x5C240A00,  //  0030  MOVE	R9	R5
      0x5C280C00,  //  0031  MOVE	R10	R6
      0x7C1C0600,  //  0032  CALL	R7	3
      0x50100200,  //  0033  LDBOOL	R4	1	0
      0x00140B02,  //  0034  ADD	R5	R5	K2
      0x7001FFE7,  //  0035  JMP		#001E
      0x80040800,  //  0036  RET	1	R4
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
    ( &(const binstruction[26]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080524,  //  0003  GETMET	R2	R2	K36
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x740A0001,  //  0006  JMPT	R2	#0009
      0x50080000,  //  0007  LDBOOL	R2	0	0
      0x80040400,  //  0008  RET	1	R2
      0x8C080121,  //  0009  GETMET	R2	R0	K33
      0x5C100200,  //  000A  MOVE	R4	R1
      0x7C080400,  //  000B  CALL	R2	2
      0x5C040400,  //  000C  MOVE	R1	R2
      0x88080101,  //  000D  GETMBR	R2	R0	K1
      0x540E03E7,  //  000E  LDINT	R3	1000
      0x0C0C0602,  //  000F  DIV	R3	R3	R2
      0x8810011E,  //  0010  GETMBR	R4	R0	K30
      0x04100204,  //  0011  SUB	R4	R1	R4
      0x28100803,  //  0012  GE	R4	R4	R3
      0x78120003,  //  0013  JMPF	R4	#0018
      0x90023C01,  //  0014  SETMBR	R0	K30	R1
      0x8C100125,  //  0015  GETMET	R4	R0	K37
      0x5C180200,  //  0016  MOVE	R6	R1
      0x7C100400,  //  0017  CALL	R4	2
      0x50100200,  //  0018  LDBOOL	R4	1	0
      0x80040800,  //  0019  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _initialize_arrays
********************************************************************/
be_local_closure(class_TwinkleAnimation__initialize_arrays,   /* name */
  be_nested_proto(
    8,                          /* nstack */
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
    ( &(const binstruction[30]) {  /* code */
      0x88040109,  //  0000  GETMBR	R1	R0	K9
      0x8C04030A,  //  0001  GETMET	R1	R1	K10
      0x7C040200,  //  0002  CALL	R1	1
      0x8808010B,  //  0003  GETMBR	R2	R0	K11
      0x8C080526,  //  0004  GETMET	R2	R2	K38
      0x5C100200,  //  0005  MOVE	R4	R1
      0x7C080400,  //  0006  CALL	R2	2
      0x8808010C,  //  0007  GETMBR	R2	R0	K12
      0x8C080527,  //  0008  GETMET	R2	R2	K39
      0x7C080200,  //  0009  CALL	R2	1
      0x8808010C,  //  000A  GETMBR	R2	R0	K12
      0x8C080526,  //  000B  GETMET	R2	R2	K38
      0x54120003,  //  000C  LDINT	R4	4
      0x08100204,  //  000D  MUL	R4	R1	R4
      0x7C080400,  //  000E  CALL	R2	2
      0x5808000F,  //  000F  LDCONST	R2	K15
      0x140C0401,  //  0010  LT	R3	R2	R1
      0x780E000A,  //  0011  JMPF	R3	#001D
      0x880C010B,  //  0012  GETMBR	R3	R0	K11
      0x980C050F,  //  0013  SETIDX	R3	R2	K15
      0x880C010C,  //  0014  GETMBR	R3	R0	K12
      0x8C0C0713,  //  0015  GETMET	R3	R3	K19
      0x54160003,  //  0016  LDINT	R5	4
      0x08140405,  //  0017  MUL	R5	R2	R5
      0x5818000F,  //  0018  LDCONST	R6	K15
      0x541DFFFB,  //  0019  LDINT	R7	-4
      0x7C0C0800,  //  001A  CALL	R3	4
      0x00080502,  //  001B  ADD	R2	R2	K2
      0x7001FFF2,  //  001C  JMP		#0010
      0x80000000,  //  001D  RET	0
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
      0x8804011F,  //  0000  GETMBR	R1	R0	K31
      0x08040328,  //  0001  MUL	R1	R1	K40
      0x540A3038,  //  0002  LDINT	R2	12345
      0x00040202,  //  0003  ADD	R1	R1	R2
      0x2C040329,  //  0004  AND	R1	R1	K41
      0x90023E01,  //  0005  SETMBR	R0	K31	R1
      0x8804011F,  //  0006  GETMBR	R1	R0	K31
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
      0x1808030F,  //  0000  LE	R2	R1	K15
      0x780A0000,  //  0001  JMPF	R2	#0003
      0x80061E00,  //  0002  RET	1	K15
      0x8C08012A,  //  0003  GETMET	R2	R0	K42
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
        { be_const_key_weak(twinkle_speed, 1), be_const_bytes_instance(0700010188130006) },
        { be_const_key_weak(min_brightness, -1), be_const_bytes_instance(07000001FF000020) },
        { be_const_key_weak(density, -1), be_const_bytes_instance(07000001FF00018000) },
        { be_const_key_weak(max_brightness, 2), be_const_bytes_instance(07000001FF0001FF00) },
        { be_const_key_weak(color, -1), be_const_bytes_instance(0400FF) },
        { be_const_key_weak(fade_speed, 0), be_const_bytes_instance(07000001FF0001B400) },
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
  /* K0   */  be_nested_str_weak(on_param_changed),
  /* K1   */  be_nested_str_weak(palette),
  /* K2   */  be_nested_str_weak(cycle_period),
  /* K3   */  be_nested_str_weak(transition_type),
  /* K4   */  be_nested_str_weak(brightness),
  /* K5   */  be_nested_str_weak(range_min),
  /* K6   */  be_nested_str_weak(range_max),
  /* K7   */  be_nested_str_weak(color_provider),
  /* K8   */  be_nested_str_weak(set_param),
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
    ( &(const binstruction[33]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0700,  //  0003  GETMET	R3	R3	K0
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x1C0C0301,  //  0007  EQ	R3	R1	K1
      0x740E0009,  //  0008  JMPT	R3	#0013
      0x1C0C0302,  //  0009  EQ	R3	R1	K2
      0x740E0007,  //  000A  JMPT	R3	#0013
      0x1C0C0303,  //  000B  EQ	R3	R1	K3
      0x740E0005,  //  000C  JMPT	R3	#0013
      0x1C0C0304,  //  000D  EQ	R3	R1	K4
      0x740E0003,  //  000E  JMPT	R3	#0013
      0x1C0C0305,  //  000F  EQ	R3	R1	K5
      0x740E0001,  //  0010  JMPT	R3	#0013
      0x1C0C0306,  //  0011  EQ	R3	R1	K6
      0x780E0005,  //  0012  JMPF	R3	#0019
      0x880C0107,  //  0013  GETMBR	R3	R0	K7
      0x8C0C0708,  //  0014  GETMET	R3	R3	K8
      0x5C140200,  //  0015  MOVE	R5	R1
      0x5C180400,  //  0016  MOVE	R6	R2
      0x7C0C0600,  //  0017  CALL	R3	3
      0x70020006,  //  0018  JMP		#0020
      0x600C0003,  //  0019  GETGBL	R3	G3
      0x5C100000,  //  001A  MOVE	R4	R0
      0x7C0C0200,  //  001B  CALL	R3	1
      0x8C0C0700,  //  001C  GETMET	R3	R3	K0
      0x5C140200,  //  001D  MOVE	R5	R1
      0x5C180400,  //  001E  MOVE	R6	R2
      0x7C0C0600,  //  001F  CALL	R3	3
      0x80000000,  //  0020  RET	0
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
      0x88100102,  //  0004  GETMBR	R4	R0	K2
      0x88140104,  //  0005  GETMBR	R5	R0	K4
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
      0x90020E02,  //  000B  SETMBR	R0	K7	R2
      0x8808010F,  //  000C  GETMBR	R2	R0	K15
      0x880C0107,  //  000D  GETMBR	R3	R0	K7
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
      0x88080107,  //  0006  GETMBR	R2	R0	K7
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
        { be_const_key_weak(cycle_period, -1), be_const_bytes_instance(050000018813) },
        { be_const_key_weak(range_min, 0), be_const_bytes_instance(040000) },
        { be_const_key_weak(palette, -1), be_const_bytes_instance(0C0605) },
        { be_const_key_weak(transition_type, 1), be_const_bytes_instance(1400050200010005) },
        { be_const_key_weak(brightness, -1), be_const_bytes_instance(07000001FF0001FF00) },
        { be_const_key_weak(range_max, -1), be_const_bytes_instance(0401FF00) },
    }))    ) } )) },
        { be_const_key_weak(init, 2), be_const_closure(class_RichPaletteAnimation_init_closure) },
        { be_const_key_weak(color_provider, -1), be_const_var(0) },
        { be_const_key_weak(start, -1), be_const_closure(class_RichPaletteAnimation_start_closure) },
    })),
    be_str_weak(RichPaletteAnimation)
);

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

// compact class 'StaticValueProvider' ktab size: 2, total: 9 (saved 56 bytes)
static const bvalue be_ktab_class_StaticValueProvider[2] = {
  /* K0   */  be_nested_str_weak(value),
  /* K1   */  be_nested_str_weak(StaticValueProvider_X28value_X3D_X25s_X29),
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
    ( &(const binstruction[ 5]) {  /* code */
      0x60040018,  //  0000  GETGBL	R1	G24
      0x58080001,  //  0001  LDCONST	R2	K1
      0x880C0100,  //  0002  GETMBR	R3	R0	K0
      0x7C040400,  //  0003  CALL	R1	2
      0x80040200,  //  0004  RET	1	R1
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
        { be_const_key_weak(value, -1), be_const_bytes_instance(0C0604) },
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

// compact class 'NoiseAnimation' ktab size: 52, total: 109 (saved 456 bytes)
static const bvalue be_ktab_class_NoiseAnimation[52] = {
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
  /* K36  */  be_nested_str_weak(on_param_changed),
  /* K37  */  be_nested_str_weak(_init_noise_table),
  /* K38  */  be_nested_str_weak(is_running),
  /* K39  */  be_nested_str_weak(_fix_time_ms),
  /* K40  */  be_nested_str_weak(width),
  /* K41  */  be_nested_str_weak(set_pixel_color),
  /* K42  */  be_nested_str_weak(is_value_provider),
  /* K43  */  be_nested_str_weak(0x_X2508x),
  /* K44  */  be_nested_str_weak(NoiseAnimation_X28color_X3D_X25s_X2C_X20scale_X3D_X25s_X2C_X20speed_X3D_X25s_X2C_X20octaves_X3D_X25s_X2C_X20priority_X3D_X25s_X2C_X20running_X3D_X25s_X29),
  /* K45  */  be_nested_str_weak(scale),
  /* K46  */  be_nested_str_weak(octaves),
  /* K47  */  be_nested_str_weak(priority),
  /* K48  */  be_nested_str_weak(start),
  /* K49  */  be_nested_str_weak(persistence),
  /* K50  */  be_nested_str_weak(_noise_1d),
  /* K51  */  be_const_int(2),
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
    ( &(const binstruction[33]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0724,  //  0003  GETMET	R3	R3	K36
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x1C0C0317,  //  0007  EQ	R3	R1	K23
      0x780E0001,  //  0008  JMPF	R3	#000B
      0x8C0C0125,  //  0009  GETMET	R3	R0	K37
      0x7C0C0200,  //  000A  CALL	R3	1
      0x880C0101,  //  000B  GETMBR	R3	R0	K1
      0x8C0C0702,  //  000C  GETMET	R3	R3	K2
      0x7C0C0200,  //  000D  CALL	R3	1
      0x6010000C,  //  000E  GETGBL	R4	G12
      0x88140103,  //  000F  GETMBR	R5	R0	K3
      0x7C100200,  //  0010  CALL	R4	1
      0x20100803,  //  0011  NE	R4	R4	R3
      0x7812000C,  //  0012  JMPF	R4	#0020
      0x88100103,  //  0013  GETMBR	R4	R0	K3
      0x8C100904,  //  0014  GETMET	R4	R4	K4
      0x5C180600,  //  0015  MOVE	R6	R3
      0x7C100400,  //  0016  CALL	R4	2
      0x6010000C,  //  0017  GETGBL	R4	G12
      0x88140103,  //  0018  GETMBR	R5	R0	K3
      0x7C100200,  //  0019  CALL	R4	1
      0x14140803,  //  001A  LT	R5	R4	R3
      0x78160003,  //  001B  JMPF	R5	#0020
      0x88140103,  //  001C  GETMBR	R5	R0	K3
      0x98140907,  //  001D  SETIDX	R5	R4	K7
      0x00100908,  //  001E  ADD	R4	R4	K8
      0x7001FFF9,  //  001F  JMP		#001A
      0x80000000,  //  0020  RET	0
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
    ( &(const binstruction[29]) {  /* code */
      0x880C0126,  //  0000  GETMBR	R3	R0	K38
      0x780E0002,  //  0001  JMPF	R3	#0005
      0x4C0C0000,  //  0002  LDNIL	R3
      0x1C0C0203,  //  0003  EQ	R3	R1	R3
      0x780E0001,  //  0004  JMPF	R3	#0007
      0x500C0000,  //  0005  LDBOOL	R3	0	0
      0x80040600,  //  0006  RET	1	R3
      0x8C0C0127,  //  0007  GETMET	R3	R0	K39
      0x5C140400,  //  0008  MOVE	R5	R2
      0x7C0C0400,  //  0009  CALL	R3	2
      0x5C080600,  //  000A  MOVE	R2	R3
      0x880C0101,  //  000B  GETMBR	R3	R0	K1
      0x8C0C0702,  //  000C  GETMET	R3	R3	K2
      0x7C0C0200,  //  000D  CALL	R3	1
      0x58100006,  //  000E  LDCONST	R4	K6
      0x14140803,  //  000F  LT	R5	R4	R3
      0x78160009,  //  0010  JMPF	R5	#001B
      0x88140328,  //  0011  GETMBR	R5	R1	K40
      0x14140805,  //  0012  LT	R5	R4	R5
      0x78160004,  //  0013  JMPF	R5	#0019
      0x8C140329,  //  0014  GETMET	R5	R1	K41
      0x5C1C0800,  //  0015  MOVE	R7	R4
      0x88200103,  //  0016  GETMBR	R8	R0	K3
      0x94201004,  //  0017  GETIDX	R8	R8	R4
      0x7C140600,  //  0018  CALL	R5	3
      0x00100908,  //  0019  ADD	R4	R4	K8
      0x7001FFF3,  //  001A  JMP		#000F
      0x50140200,  //  001B  LDBOOL	R5	1	0
      0x80040A00,  //  001C  RET	1	R5
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
      0x8C0C072A,  //  0003  GETMET	R3	R3	K42
      0x5C140200,  //  0004  MOVE	R5	R1
      0x7C0C0400,  //  0005  CALL	R3	2
      0x780E0004,  //  0006  JMPF	R3	#000C
      0x600C0008,  //  0007  GETGBL	R3	G8
      0x5C100200,  //  0008  MOVE	R4	R1
      0x7C0C0200,  //  0009  CALL	R3	1
      0x5C080600,  //  000A  MOVE	R2	R3
      0x70020004,  //  000B  JMP		#0011
      0x600C0018,  //  000C  GETGBL	R3	G24
      0x5810002B,  //  000D  LDCONST	R4	K43
      0x5C140200,  //  000E  MOVE	R5	R1
      0x7C0C0400,  //  000F  CALL	R3	2
      0x5C080600,  //  0010  MOVE	R2	R3
      0x600C0018,  //  0011  GETGBL	R3	G24
      0x5810002C,  //  0012  LDCONST	R4	K44
      0x5C140400,  //  0013  MOVE	R5	R2
      0x8818012D,  //  0014  GETMBR	R6	R0	K45
      0x881C011B,  //  0015  GETMBR	R7	R0	K27
      0x8820012E,  //  0016  GETMBR	R8	R0	K46
      0x8824012F,  //  0017  GETMBR	R9	R0	K47
      0x88280126,  //  0018  GETMBR	R10	R0	K38
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
      0x8C080530,  //  0003  GETMET	R2	R2	K48
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x8C080125,  //  0006  GETMET	R2	R0	K37
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
      0x8814012D,  //  0002  GETMBR	R5	R0	K45
      0x8818012E,  //  0003  GETMBR	R6	R0	K46
      0x881C0131,  //  0004  GETMBR	R7	R0	K49
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
      0x8C300132,  //  0015  GETMET	R12	R0	K50
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
      0x08201133,  //  002B  MUL	R8	R8	K51
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
        { be_const_key_weak(octaves, -1), be_const_bytes_instance(07000100040001) },
        { be_const_key_weak(seed, 0), be_const_bytes_instance(07000002FFFF0000013930) },
        { be_const_key_weak(speed, 3), be_const_bytes_instance(07000001FF00001E) },
        { be_const_key_weak(persistence, 1), be_const_bytes_instance(07000001FF00018000) },
        { be_const_key_weak(color, -1), be_const_bytes_instance(0406) },
        { be_const_key_weak(scale, -1), be_const_bytes_instance(07000101FF000032) },
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

// compact class 'FrameBuffer' ktab size: 21, total: 43 (saved 176 bytes)
static const bvalue be_ktab_class_FrameBuffer[21] = {
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
  /* K11  */  be_nested_str_weak(index_error),
  /* K12  */  be_nested_str_weak(pixel_X20index_X20out_X20of_X20range),
  /* K13  */  be_nested_str_weak(set),
  /* K14  */  be_nested_str_weak(tohex),
  /* K15  */  be_nested_str_weak(FrameBuffer_X28width_X3D_X25s_X2C_X20pixels_X3D_X25s_X29),
  /* K16  */  be_nested_str_weak(set_pixel_color),
  /* K17  */  be_nested_str_weak(animation),
  /* K18  */  be_nested_str_weak(frame_buffer),
  /* K19  */  be_nested_str_weak(get),
  /* K20  */  be_nested_str_weak(get_pixel_color),
};


extern const bclass be_class_FrameBuffer;

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
    ( &(const binstruction[18]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x8C040306,  //  0001  GETMET	R1	R1	K6
      0x7C040200,  //  0002  CALL	R1	1
      0x6004000C,  //  0003  GETGBL	R1	G12
      0x88080104,  //  0004  GETMBR	R2	R0	K4
      0x7C040200,  //  0005  CALL	R1	1
      0x88080103,  //  0006  GETMBR	R2	R0	K3
      0x540E0003,  //  0007  LDINT	R3	4
      0x08080403,  //  0008  MUL	R2	R2	R3
      0x20040202,  //  0009  NE	R1	R1	R2
      0x78060005,  //  000A  JMPF	R1	#0011
      0x88040104,  //  000B  GETMBR	R1	R0	K4
      0x8C040305,  //  000C  GETMET	R1	R1	K5
      0x880C0103,  //  000D  GETMBR	R3	R0	K3
      0x54120003,  //  000E  LDINT	R4	4
      0x080C0604,  //  000F  MUL	R3	R3	R4
      0x7C040400,  //  0010  CALL	R1	2
      0x80000000,  //  0011  RET	0
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
      0xB006170C,  //  0005  RAISE	1	K11	K12
      0x880C0104,  //  0006  GETMBR	R3	R0	K4
      0x8C0C070D,  //  0007  GETMET	R3	R3	K13
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
      0x8C04030E,  //  0001  GETMET	R1	R1	K14
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
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
      0x5808000F,  //  0001  LDCONST	R2	K15
      0x880C0103,  //  0002  GETMBR	R3	R0	K3
      0x88100104,  //  0003  GETMBR	R4	R0	K4
      0x7C040600,  //  0004  CALL	R1	3
      0x80040200,  //  0005  RET	1	R1
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
      0x8C0C0110,  //  0000  GETMET	R3	R0	K16
      0x5C140200,  //  0001  MOVE	R5	R1
      0x5C180400,  //  0002  MOVE	R6	R2
      0x7C0C0600,  //  0003  CALL	R3	3
      0x80000000,  //  0004  RET	0
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
      0xB8062200,  //  0000  GETNGBL	R1	K17
      0x8C040312,  //  0001  GETMET	R1	R1	K18
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x80040200,  //  0004  RET	1	R1
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
      0xB006170C,  //  0005  RAISE	1	K11	K12
      0x88080104,  //  0006  GETMBR	R2	R0	K4
      0x8C080513,  //  0007  GETMET	R2	R2	K19
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
      0x8C080114,  //  0000  GETMET	R2	R0	K20
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x80040400,  //  0003  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: FrameBuffer
********************************************************************/
extern const bclass be_class_FrameBufferNtv;
be_local_class(FrameBuffer,
    2,
    &be_class_FrameBufferNtv,
    be_nested_map(12,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(pixels, -1), be_const_var(0) },
        { be_const_key_weak(resize, 6), be_const_closure(class_FrameBuffer_resize_closure) },
        { be_const_key_weak(clear, -1), be_const_closure(class_FrameBuffer_clear_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_FrameBuffer_init_closure) },
        { be_const_key_weak(set_pixel_color, -1), be_const_closure(class_FrameBuffer_set_pixel_color_closure) },
        { be_const_key_weak(tohex, -1), be_const_closure(class_FrameBuffer_tohex_closure) },
        { be_const_key_weak(tostring, -1), be_const_closure(class_FrameBuffer_tostring_closure) },
        { be_const_key_weak(copy, -1), be_const_closure(class_FrameBuffer_copy_closure) },
        { be_const_key_weak(setitem, 9), be_const_closure(class_FrameBuffer_setitem_closure) },
        { be_const_key_weak(get_pixel_color, 7), be_const_closure(class_FrameBuffer_get_pixel_color_closure) },
        { be_const_key_weak(item, -1), be_const_closure(class_FrameBuffer_item_closure) },
        { be_const_key_weak(width, -1), be_const_var(1) },
    })),
    be_str_weak(FrameBuffer)
);

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
    18,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(shift_period),
    /* K1   */  be_nested_str_weak(spatial_period),
    /* K2   */  be_nested_str_weak(phase_shift),
    /* K3   */  be_nested_str_weak(engine),
    /* K4   */  be_nested_str_weak(get_strip_length),
    /* K5   */  be_nested_str_weak(value_buffer),
    /* K6   */  be_nested_str_weak(resize),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str_weak(tasmota),
    /* K9   */  be_nested_str_weak(scale_uint),
    /* K10  */  be_const_real_hex(0x447A0000),
    /* K11  */  be_const_int(1),
    }),
    be_str_weak(_update_value_buffer),
    &be_const_str_solidified,
    ( &(const binstruction[63]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x880C0101,  //  0001  GETMBR	R3	R0	K1
      0x88100102,  //  0002  GETMBR	R4	R0	K2
      0x88140103,  //  0003  GETMBR	R5	R0	K3
      0x8C140B04,  //  0004  GETMET	R5	R5	K4
      0x7C140200,  //  0005  CALL	R5	1
      0x6018000C,  //  0006  GETGBL	R6	G12
      0x881C0105,  //  0007  GETMBR	R7	R0	K5
      0x7C180200,  //  0008  CALL	R6	1
      0x20180C05,  //  0009  NE	R6	R6	R5
      0x781A0003,  //  000A  JMPF	R6	#000F
      0x88180105,  //  000B  GETMBR	R6	R0	K5
      0x8C180D06,  //  000C  GETMET	R6	R6	K6
      0x5C200A00,  //  000D  MOVE	R8	R5
      0x7C180400,  //  000E  CALL	R6	2
      0x24180707,  //  000F  GT	R6	R3	K7
      0x781A0001,  //  0010  JMPF	R6	#0013
      0x5C180600,  //  0011  MOVE	R6	R3
      0x70020000,  //  0012  JMP		#0014
      0x5C180A00,  //  0013  MOVE	R6	R5
      0x581C0007,  //  0014  LDCONST	R7	K7
      0x24200507,  //  0015  GT	R8	R2	K7
      0x7822000A,  //  0016  JMPF	R8	#0022
      0xB8221000,  //  0017  GETNGBL	R8	K8
      0x8C201109,  //  0018  GETMET	R8	R8	K9
      0x10280202,  //  0019  MOD	R10	R1	R2
      0x582C0007,  //  001A  LDCONST	R11	K7
      0x5C300400,  //  001B  MOVE	R12	R2
      0x58340007,  //  001C  LDCONST	R13	K7
      0x543A03E7,  //  001D  LDINT	R14	1000
      0x7C200C00,  //  001E  CALL	R8	6
      0x0C20110A,  //  001F  DIV	R8	R8	K10
      0x08241006,  //  0020  MUL	R9	R8	R6
      0x5C1C1200,  //  0021  MOVE	R7	R9
      0xB8221000,  //  0022  GETNGBL	R8	K8
      0x8C201109,  //  0023  GETMET	R8	R8	K9
      0x5C280800,  //  0024  MOVE	R10	R4
      0x582C0007,  //  0025  LDCONST	R11	K7
      0x54320063,  //  0026  LDINT	R12	100
      0x58340007,  //  0027  LDCONST	R13	K7
      0x5C380C00,  //  0028  MOVE	R14	R6
      0x7C200C00,  //  0029  CALL	R8	6
      0x58240007,  //  002A  LDCONST	R9	K7
      0x14281205,  //  002B  LT	R10	R9	R5
      0x782A0010,  //  002C  JMPF	R10	#003E
      0x00281207,  //  002D  ADD	R10	R9	R7
      0x00281408,  //  002E  ADD	R10	R10	R8
      0x10281406,  //  002F  MOD	R10	R10	R6
      0xB82E1000,  //  0030  GETNGBL	R11	K8
      0x8C2C1709,  //  0031  GETMET	R11	R11	K9
      0x60340009,  //  0032  GETGBL	R13	G9
      0x5C381400,  //  0033  MOVE	R14	R10
      0x7C340200,  //  0034  CALL	R13	1
      0x58380007,  //  0035  LDCONST	R14	K7
      0x043C0D0B,  //  0036  SUB	R15	R6	K11
      0x58400007,  //  0037  LDCONST	R16	K7
      0x544600FE,  //  0038  LDINT	R17	255
      0x7C2C0C00,  //  0039  CALL	R11	6
      0x88300105,  //  003A  GETMBR	R12	R0	K5
      0x9830120B,  //  003B  SETIDX	R12	R9	R11
      0x0024130B,  //  003C  ADD	R9	R9	K11
      0x7001FFEC,  //  003D  JMP		#002B
      0x80000000,  //  003E  RET	0
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
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(spatial_period, -1), be_const_bytes_instance(0500000000) },
        { be_const_key_weak(shift_period, 0), be_const_bytes_instance(0500000000) },
        { be_const_key_weak(phase_shift, -1), be_const_bytes_instance(07000000640000) },
    }))    ) } )) },
        { be_const_key_weak(init, -1), be_const_closure(class_PaletteGradientAnimation_init_closure) },
        { be_const_key_weak(_update_value_buffer, -1), be_const_closure(class_PaletteGradientAnimation__update_value_buffer_closure) },
    })),
    be_str_weak(PaletteGradientAnimation)
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

extern const bclass be_class_AnimationMath;
// compact class 'AnimationMath' ktab size: 13, total: 31 (saved 144 bytes)
static const bvalue be_ktab_class_AnimationMath[13] = {
  /* K0   */  be_const_class(be_class_AnimationMath),
  /* K1   */  be_nested_str_weak(math),
  /* K2   */  be_nested_str_weak(int),
  /* K3   */  be_const_int(0),
  /* K4   */  be_const_real_hex(0x437F0000),
  /* K5   */  be_nested_str_weak(sqrt),
  /* K6   */  be_nested_str_weak(max),
  /* K7   */  be_nested_str_weak(round),
  /* K8   */  be_nested_str_weak(abs),
  /* K9   */  be_nested_str_weak(tasmota),
  /* K10  */  be_nested_str_weak(scale_int),
  /* K11  */  be_nested_str_weak(sine_int),
  /* K12  */  be_nested_str_weak(min),
};


extern const bclass be_class_AnimationMath;

/********************************************************************
** Solidified function: sqrt
********************************************************************/
be_local_closure(class_AnimationMath_sqrt,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationMath,     /* shared constants */
    be_str_weak(sqrt),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x600C0004,  //  0002  GETGBL	R3	G4
      0x5C100000,  //  0003  MOVE	R4	R0
      0x7C0C0200,  //  0004  CALL	R3	1
      0x1C0C0702,  //  0005  EQ	R3	R3	K2
      0x780E000E,  //  0006  JMPF	R3	#0016
      0x280C0103,  //  0007  GE	R3	R0	K3
      0x780E000C,  //  0008  JMPF	R3	#0016
      0x540E00FE,  //  0009  LDINT	R3	255
      0x180C0003,  //  000A  LE	R3	R0	R3
      0x780E0009,  //  000B  JMPF	R3	#0016
      0x0C0C0104,  //  000C  DIV	R3	R0	K4
      0x60100009,  //  000D  GETGBL	R4	G9
      0x8C140505,  //  000E  GETMET	R5	R2	K5
      0x5C1C0600,  //  000F  MOVE	R7	R3
      0x7C140400,  //  0010  CALL	R5	2
      0x541A00FE,  //  0011  LDINT	R6	255
      0x08140A06,  //  0012  MUL	R5	R5	R6
      0x7C100200,  //  0013  CALL	R4	1
      0x80040800,  //  0014  RET	1	R4
      0x70020003,  //  0015  JMP		#001A
      0x8C0C0505,  //  0016  GETMET	R3	R2	K5
      0x5C140000,  //  0017  MOVE	R5	R0
      0x7C0C0400,  //  0018  CALL	R3	2
      0x80040600,  //  0019  RET	1	R3
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: max
********************************************************************/
be_local_closure(class_AnimationMath_max,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    13,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationMath,     /* shared constants */
    be_str_weak(max),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x600C0016,  //  0002  GETGBL	R3	G22
      0x88100506,  //  0003  GETMBR	R4	R2	K6
      0x5C140000,  //  0004  MOVE	R5	R0
      0x7C0C0400,  //  0005  CALL	R3	2
      0x80040600,  //  0006  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: round
********************************************************************/
be_local_closure(class_AnimationMath_round,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationMath,     /* shared constants */
    be_str_weak(round),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x600C0009,  //  0002  GETGBL	R3	G9
      0x8C100507,  //  0003  GETMET	R4	R2	K7
      0x5C180000,  //  0004  MOVE	R6	R0
      0x7C100400,  //  0005  CALL	R4	2
      0x7C0C0200,  //  0006  CALL	R3	1
      0x80040600,  //  0007  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: abs
********************************************************************/
be_local_closure(class_AnimationMath_abs,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationMath,     /* shared constants */
    be_str_weak(abs),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x8C0C0508,  //  0002  GETMET	R3	R2	K8
      0x5C140000,  //  0003  MOVE	R5	R0
      0x7C0C0400,  //  0004  CALL	R3	2
      0x80040600,  //  0005  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: cos
********************************************************************/
be_local_closure(class_AnimationMath_cos,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationMath,     /* shared constants */
    be_str_weak(cos),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0xB80A1200,  //  0001  GETNGBL	R2	K9
      0x8C08050A,  //  0002  GETMET	R2	R2	K10
      0x5C100000,  //  0003  MOVE	R4	R0
      0x58140003,  //  0004  LDCONST	R5	K3
      0x541A00FE,  //  0005  LDINT	R6	255
      0x581C0003,  //  0006  LDCONST	R7	K3
      0x54227FFE,  //  0007  LDINT	R8	32767
      0x7C080C00,  //  0008  CALL	R2	6
      0xB80E1200,  //  0009  GETNGBL	R3	K9
      0x8C0C070B,  //  000A  GETMET	R3	R3	K11
      0x54161FFF,  //  000B  LDINT	R5	8192
      0x04140405,  //  000C  SUB	R5	R2	R5
      0x7C0C0400,  //  000D  CALL	R3	2
      0xB8121200,  //  000E  GETNGBL	R4	K9
      0x8C10090A,  //  000F  GETMET	R4	R4	K10
      0x5C180600,  //  0010  MOVE	R6	R3
      0x541DEFFF,  //  0011  LDINT	R7	-4096
      0x54220FFF,  //  0012  LDINT	R8	4096
      0x5425FF00,  //  0013  LDINT	R9	-255
      0x542A00FE,  //  0014  LDINT	R10	255
      0x7C100C00,  //  0015  CALL	R4	6
      0x80040800,  //  0016  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: sin
********************************************************************/
be_local_closure(class_AnimationMath_sin,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationMath,     /* shared constants */
    be_str_weak(sin),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0xB80A1200,  //  0001  GETNGBL	R2	K9
      0x8C08050A,  //  0002  GETMET	R2	R2	K10
      0x5C100000,  //  0003  MOVE	R4	R0
      0x58140003,  //  0004  LDCONST	R5	K3
      0x541A00FE,  //  0005  LDINT	R6	255
      0x581C0003,  //  0006  LDCONST	R7	K3
      0x54227FFE,  //  0007  LDINT	R8	32767
      0x7C080C00,  //  0008  CALL	R2	6
      0xB80E1200,  //  0009  GETNGBL	R3	K9
      0x8C0C070B,  //  000A  GETMET	R3	R3	K11
      0x5C140400,  //  000B  MOVE	R5	R2
      0x7C0C0400,  //  000C  CALL	R3	2
      0xB8121200,  //  000D  GETNGBL	R4	K9
      0x8C10090A,  //  000E  GETMET	R4	R4	K10
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
** Solidified function: scale
********************************************************************/
be_local_closure(class_AnimationMath_scale,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    5,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationMath,     /* shared constants */
    be_str_weak(scale),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x58140000,  //  0000  LDCONST	R5	K0
      0xB81A1200,  //  0001  GETNGBL	R6	K9
      0x8C180D0A,  //  0002  GETMET	R6	R6	K10
      0x5C200000,  //  0003  MOVE	R8	R0
      0x5C240200,  //  0004  MOVE	R9	R1
      0x5C280400,  //  0005  MOVE	R10	R2
      0x5C2C0600,  //  0006  MOVE	R11	R3
      0x5C300800,  //  0007  MOVE	R12	R4
      0x7C180C00,  //  0008  CALL	R6	6
      0x80040C00,  //  0009  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: min
********************************************************************/
be_local_closure(class_AnimationMath_min,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    13,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationMath,     /* shared constants */
    be_str_weak(min),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x600C0016,  //  0002  GETGBL	R3	G22
      0x8810050C,  //  0003  GETMBR	R4	R2	K12
      0x5C140000,  //  0004  MOVE	R5	R0
      0x7C0C0400,  //  0005  CALL	R3	2
      0x80040600,  //  0006  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: AnimationMath
********************************************************************/
be_local_class(AnimationMath,
    0,
    NULL,
    be_nested_map(8,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(min, -1), be_const_static_closure(class_AnimationMath_min_closure) },
        { be_const_key_weak(max, 2), be_const_static_closure(class_AnimationMath_max_closure) },
        { be_const_key_weak(scale, -1), be_const_static_closure(class_AnimationMath_scale_closure) },
        { be_const_key_weak(round, 6), be_const_static_closure(class_AnimationMath_round_closure) },
        { be_const_key_weak(cos, -1), be_const_static_closure(class_AnimationMath_cos_closure) },
        { be_const_key_weak(sin, -1), be_const_static_closure(class_AnimationMath_sin_closure) },
        { be_const_key_weak(abs, -1), be_const_static_closure(class_AnimationMath_abs_closure) },
        { be_const_key_weak(sqrt, 0), be_const_static_closure(class_AnimationMath_sqrt_closure) },
    })),
    be_str_weak(AnimationMath)
);
// compact class 'CompositeColorProvider' ktab size: 14, total: 24 (saved 80 bytes)
static const bvalue be_ktab_class_CompositeColorProvider[14] = {
  /* K0   */  be_nested_str_weak(providers),
  /* K1   */  be_nested_str_weak(push),
  /* K2   */  be_const_int(0),
  /* K3   */  be_const_int(1),
  /* K4   */  be_nested_str_weak(get_color_for_value),
  /* K5   */  be_nested_str_weak(_blend_colors),
  /* K6   */  be_nested_str_weak(produce_value),
  /* K7   */  be_nested_str_weak(CompositeColorProvider_X28providers_X3D_X25s_X2C_X20blend_mode_X3D_X25s_X29),
  /* K8   */  be_nested_str_weak(blend_mode),
  /* K9   */  be_const_real_hex(0x437F0000),
  /* K10  */  be_const_int(2),
  /* K11  */  be_nested_str_weak(tasmota),
  /* K12  */  be_nested_str_weak(scale_uint),
  /* K13  */  be_nested_str_weak(init),
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
    ( &(const binstruction[ 8]) {  /* code */
      0x60040018,  //  0000  GETGBL	R1	G24
      0x58080007,  //  0001  LDCONST	R2	K7
      0x600C000C,  //  0002  GETGBL	R3	G12
      0x88100100,  //  0003  GETMBR	R4	R0	K0
      0x7C0C0200,  //  0004  CALL	R3	1
      0x88100108,  //  0005  GETMBR	R4	R0	K8
      0x7C040600,  //  0006  CALL	R1	3
      0x80040200,  //  0007  RET	1	R1
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
      0x0C401109,  //  0023  DIV	R16	R8	K9
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
      0x1C40070A,  //  0064  EQ	R16	R3	K10
      0x78420026,  //  0065  JMPF	R16	#008D
      0xB8421600,  //  0066  GETNGBL	R16	K11
      0x8C40210C,  //  0067  GETMET	R16	R16	K12
      0x08480E0B,  //  0068  MUL	R18	R7	R11
      0x584C0002,  //  0069  LDCONST	R19	K2
      0x545200FE,  //  006A  LDINT	R20	255
      0x545600FE,  //  006B  LDINT	R21	255
      0x08502815,  //  006C  MUL	R20	R20	R21
      0x58540002,  //  006D  LDCONST	R21	K2
      0x545A00FE,  //  006E  LDINT	R22	255
      0x7C400C00,  //  006F  CALL	R16	6
      0x5C342000,  //  0070  MOVE	R13	R16
      0xB8421600,  //  0071  GETNGBL	R16	K11
      0x8C40210C,  //  0072  GETMET	R16	R16	K12
      0x08480C0A,  //  0073  MUL	R18	R6	R10
      0x584C0002,  //  0074  LDCONST	R19	K2
      0x545200FE,  //  0075  LDINT	R20	255
      0x545600FE,  //  0076  LDINT	R21	255
      0x08502815,  //  0077  MUL	R20	R20	R21
      0x58540002,  //  0078  LDCONST	R21	K2
      0x545A00FE,  //  0079  LDINT	R22	255
      0x7C400C00,  //  007A  CALL	R16	6
      0x5C382000,  //  007B  MOVE	R14	R16
      0xB8421600,  //  007C  GETNGBL	R16	K11
      0x8C40210C,  //  007D  GETMET	R16	R16	K12
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
      0x8C08050D,  //  0003  GETMET	R2	R2	K13
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
        { be_const_key_weak(blend_mode, -1), be_const_bytes_instance(14000003000000010002) },
    }))    ) } )) },
        { be_const_key_weak(tostring, -1), be_const_closure(class_CompositeColorProvider_tostring_closure) },
        { be_const_key_weak(providers, 4), be_const_var(0) },
        { be_const_key_weak(_blend_colors, -1), be_const_closure(class_CompositeColorProvider__blend_colors_closure) },
        { be_const_key_weak(produce_value, -1), be_const_closure(class_CompositeColorProvider_produce_value_closure) },
    })),
    be_str_weak(CompositeColorProvider)
);

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

// compact class 'WaveAnimation' ktab size: 43, total: 71 (saved 224 bytes)
static const bvalue be_ktab_class_WaveAnimation[43] = {
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
  /* K22  */  be_nested_str_weak(_fix_time_ms),
  /* K23  */  be_nested_str_weak(engine),
  /* K24  */  be_nested_str_weak(get_strip_length),
  /* K25  */  be_nested_str_weak(width),
  /* K26  */  be_nested_str_weak(current_colors),
  /* K27  */  be_nested_str_weak(size),
  /* K28  */  be_nested_str_weak(set_pixel_color),
  /* K29  */  be_nested_str_weak(on_param_changed),
  /* K30  */  be_nested_str_weak(_init_wave_table),
  /* K31  */  be_nested_str_weak(update),
  /* K32  */  be_nested_str_weak(start_time),
  /* K33  */  be_nested_str_weak(time_offset),
  /* K34  */  be_nested_str_weak(_calculate_wave),
  /* K35  */  be_nested_str_weak(init),
  /* K36  */  be_nested_str_weak(phase),
  /* K37  */  be_nested_str_weak(amplitude),
  /* K38  */  be_nested_str_weak(center_level),
  /* K39  */  be_nested_str_weak(back_color),
  /* K40  */  be_nested_str_weak(is_color_provider),
  /* K41  */  be_nested_str_weak(get_color_for_value),
  /* K42  */  be_nested_str_weak(resolve_value),
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
    ( &(const binstruction[34]) {  /* code */
      0x880C0115,  //  0000  GETMBR	R3	R0	K21
      0x780E0002,  //  0001  JMPF	R3	#0005
      0x4C0C0000,  //  0002  LDNIL	R3
      0x1C0C0203,  //  0003  EQ	R3	R1	R3
      0x780E0001,  //  0004  JMPF	R3	#0007
      0x500C0000,  //  0005  LDBOOL	R3	0	0
      0x80040600,  //  0006  RET	1	R3
      0x8C0C0116,  //  0007  GETMET	R3	R0	K22
      0x5C140400,  //  0008  MOVE	R5	R2
      0x7C0C0400,  //  0009  CALL	R3	2
      0x5C080600,  //  000A  MOVE	R2	R3
      0x880C0117,  //  000B  GETMBR	R3	R0	K23
      0x8C0C0718,  //  000C  GETMET	R3	R3	K24
      0x7C0C0200,  //  000D  CALL	R3	1
      0x58100003,  //  000E  LDCONST	R4	K3
      0x14140803,  //  000F  LT	R5	R4	R3
      0x7816000E,  //  0010  JMPF	R5	#0020
      0x88140319,  //  0011  GETMBR	R5	R1	K25
      0x14140805,  //  0012  LT	R5	R4	R5
      0x78160009,  //  0013  JMPF	R5	#001E
      0x8814011A,  //  0014  GETMBR	R5	R0	K26
      0x8C140B1B,  //  0015  GETMET	R5	R5	K27
      0x7C140200,  //  0016  CALL	R5	1
      0x14140805,  //  0017  LT	R5	R4	R5
      0x78160004,  //  0018  JMPF	R5	#001E
      0x8C14031C,  //  0019  GETMET	R5	R1	K28
      0x5C1C0800,  //  001A  MOVE	R7	R4
      0x8820011A,  //  001B  GETMBR	R8	R0	K26
      0x94201004,  //  001C  GETIDX	R8	R8	R4
      0x7C140600,  //  001D  CALL	R5	3
      0x00100906,  //  001E  ADD	R4	R4	K6
      0x7001FFEE,  //  001F  JMP		#000F
      0x50140200,  //  0020  LDBOOL	R5	1	0
      0x80040A00,  //  0021  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: on_param_changed
********************************************************************/
be_local_closure(class_WaveAnimation_on_param_changed,   /* name */
  be_nested_proto(
    7,                          /* nstack */
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
    ( &(const binstruction[12]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C071D,  //  0003  GETMET	R3	R3	K29
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x1C0C0302,  //  0007  EQ	R3	R1	K2
      0x780E0001,  //  0008  JMPF	R3	#000B
      0x8C0C011E,  //  0009  GETMET	R3	R0	K30
      0x7C0C0200,  //  000A  CALL	R3	1
      0x80000000,  //  000B  RET	0
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
      0x8C08051F,  //  0003  GETMET	R2	R2	K31
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x740A0001,  //  0006  JMPT	R2	#0009
      0x50080000,  //  0007  LDBOOL	R2	0	0
      0x80040400,  //  0008  RET	1	R2
      0x88080113,  //  0009  GETMBR	R2	R0	K19
      0x240C0503,  //  000A  GT	R3	R2	K3
      0x780E0011,  //  000B  JMPF	R3	#001E
      0x880C0120,  //  000C  GETMBR	R3	R0	K32
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
      0x90024205,  //  001D  SETMBR	R0	K33	R5
      0x8C0C0122,  //  001E  GETMET	R3	R0	K34
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
      0x8C080523,  //  0003  GETMET	R2	R2	K35
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x60080012,  //  0006  GETGBL	R2	G18
      0x7C080000,  //  0007  CALL	R2	0
      0x90023402,  //  0008  SETMBR	R0	K26	R2
      0x90024303,  //  0009  SETMBR	R0	K33	K3
      0x60080012,  //  000A  GETGBL	R2	G18
      0x7C080000,  //  000B  CALL	R2	0
      0x90020002,  //  000C  SETMBR	R0	K0	R2
      0x8C08011E,  //  000D  GETMET	R2	R0	K30
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
      0x88080117,  //  0000  GETMBR	R2	R0	K23
      0x8C080518,  //  0001  GETMET	R2	R2	K24
      0x7C080200,  //  0002  CALL	R2	1
      0x880C0112,  //  0003  GETMBR	R3	R0	K18
      0x88100124,  //  0004  GETMBR	R4	R0	K36
      0x88140125,  //  0005  GETMBR	R5	R0	K37
      0x88180126,  //  0006  GETMBR	R6	R0	K38
      0x881C0127,  //  0007  GETMBR	R7	R0	K39
      0x8820010D,  //  0008  GETMBR	R8	R0	K13
      0x8824011A,  //  0009  GETMBR	R9	R0	K26
      0x8C24131B,  //  000A  GETMET	R9	R9	K27
      0x7C240200,  //  000B  CALL	R9	1
      0x20241202,  //  000C  NE	R9	R9	R2
      0x78260003,  //  000D  JMPF	R9	#0012
      0x8824011A,  //  000E  GETMBR	R9	R0	K26
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
      0x88300121,  //  0021  GETMBR	R12	R0	K33
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
      0x8C402128,  //  005B  GETMET	R16	R16	K40
      0x5C481000,  //  005C  MOVE	R18	R8
      0x7C400400,  //  005D  CALL	R16	2
      0x78420009,  //  005E  JMPF	R16	#0069
      0x88401129,  //  005F  GETMBR	R16	R8	K41
      0x4C440000,  //  0060  LDNIL	R17
      0x20402011,  //  0061  NE	R16	R16	R17
      0x78420005,  //  0062  JMPF	R16	#0069
      0x8C401129,  //  0063  GETMET	R16	R8	K41
      0x5C481C00,  //  0064  MOVE	R18	R14
      0x584C0003,  //  0065  LDCONST	R19	K3
      0x7C400600,  //  0066  CALL	R16	3
      0x5C3C2000,  //  0067  MOVE	R15	R16
      0x7002003A,  //  0068  JMP		#00A4
      0x8C40012A,  //  0069  GETMET	R16	R0	K42
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
      0x8840011A,  //  00A4  GETMBR	R16	R0	K26
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
        { be_const_key_weak(phase, 7), be_const_bytes_instance(07000001FF000000) },
        { be_const_key_weak(center_level, 3), be_const_bytes_instance(07000001FF00018000) },
        { be_const_key_weak(amplitude, -1), be_const_bytes_instance(07000001FF00018000) },
        { be_const_key_weak(frequency, 5), be_const_bytes_instance(07000001FF000020) },
        { be_const_key_weak(wave_speed, -1), be_const_bytes_instance(07000001FF000032) },
        { be_const_key_weak(wave_type, -1), be_const_bytes_instance(07000000030000) },
        { be_const_key_weak(back_color, -1), be_const_bytes_instance(0402000000FF) },
        { be_const_key_weak(color, -1), be_const_bytes_instance(04020000FFFF) },
    }))    ) } )) },
        { be_const_key_weak(_init_wave_table, 6), be_const_closure(class_WaveAnimation__init_wave_table_closure) },
        { be_const_key_weak(_calculate_wave, -1), be_const_closure(class_WaveAnimation__calculate_wave_closure) },
    })),
    be_str_weak(WaveAnimation)
);

/********************************************************************
** Solidified function: wave_rainbow_sine
********************************************************************/
be_local_closure(wave_rainbow_sine,   /* name */
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
    /* K1   */  be_nested_str_weak(wave_animation),
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
    /* K13  */  be_nested_str_weak(wave_type),
    /* K14  */  be_nested_str_weak(frequency),
    /* K15  */  be_nested_str_weak(wave_speed),
    }),
    be_str_weak(wave_rainbow_sine),
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
      0x90061B0A,  //  0014  SETMBR	R1	K13	K10
      0x540E001F,  //  0015  LDINT	R3	32
      0x90061C03,  //  0016  SETMBR	R1	K14	R3
      0x540E0031,  //  0017  LDINT	R3	50
      0x90061E03,  //  0018  SETMBR	R1	K15	R3
      0x80040200,  //  0019  RET	1	R1
    })
  )
);
/*******************************************************************/

// compact class 'EventHandler' ktab size: 7, total: 11 (saved 32 bytes)
static const bvalue be_ktab_class_EventHandler[7] = {
  /* K0   */  be_nested_str_weak(is_active),
  /* K1   */  be_nested_str_weak(condition),
  /* K2   */  be_nested_str_weak(callback_func),
  /* K3   */  be_nested_str_weak(event_name),
  /* K4   */  be_nested_str_weak(priority),
  /* K5   */  be_const_int(0),
  /* K6   */  be_nested_str_weak(metadata),
};


extern const bclass be_class_EventHandler;

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
** Solidified class: EventHandler
********************************************************************/
be_local_class(EventHandler,
    6,
    NULL,
    be_nested_map(9,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(set_active, -1), be_const_closure(class_EventHandler_set_active_closure) },
        { be_const_key_weak(execute, 2), be_const_closure(class_EventHandler_execute_closure) },
        { be_const_key_weak(callback_func, -1), be_const_var(1) },
        { be_const_key_weak(init, -1), be_const_closure(class_EventHandler_init_closure) },
        { be_const_key_weak(event_name, -1), be_const_var(0) },
        { be_const_key_weak(condition, -1), be_const_var(2) },
        { be_const_key_weak(priority, 3), be_const_var(3) },
        { be_const_key_weak(metadata, -1), be_const_var(5) },
        { be_const_key_weak(is_active, -1), be_const_var(4) },
    })),
    be_str_weak(EventHandler)
);
// compact class 'Animation' ktab size: 28, total: 40 (saved 96 bytes)
static const bvalue be_ktab_class_Animation[28] = {
  /* K0   */  be_nested_str_weak(get_param_value),
  /* K1   */  be_nested_str_weak(color),
  /* K2   */  be_nested_str_weak(animation),
  /* K3   */  be_nested_str_weak(opacity_frame),
  /* K4   */  be_nested_str_weak(width),
  /* K5   */  be_nested_str_weak(frame_buffer),
  /* K6   */  be_nested_str_weak(clear),
  /* K7   */  be_nested_str_weak(is_running),
  /* K8   */  be_nested_str_weak(start),
  /* K9   */  be_nested_str_weak(start_time),
  /* K10  */  be_nested_str_weak(update),
  /* K11  */  be_nested_str_weak(render),
  /* K12  */  be_nested_str_weak(apply_opacity),
  /* K13  */  be_nested_str_weak(pixels),
  /* K14  */  be_nested_str_weak(int),
  /* K15  */  be_nested_str_weak(_fix_time_ms),
  /* K16  */  be_nested_str_weak(duration),
  /* K17  */  be_nested_str_weak(loop),
  /* K18  */  be_const_int(0),
  /* K19  */  be_nested_str_weak(values),
  /* K20  */  be_nested_str_weak(init),
  /* K21  */  be_nested_str_weak(get_color_at),
  /* K22  */  be_nested_str_weak(fill_pixels),
  /* K23  */  be_nested_str_weak(opacity),
  /* K24  */  be_nested_str_weak(_apply_opacity),
  /* K25  */  be_nested_str_weak(Animation_X28_X25s_X2C_X20priority_X3D_X25s_X2C_X20duration_X3D_X25s_X2C_X20loop_X3D_X25s_X2C_X20running_X3D_X25s_X29),
  /* K26  */  be_nested_str_weak(name),
  /* K27  */  be_nested_str_weak(priority),
};


extern const bclass be_class_Animation;

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
** Solidified function: _apply_opacity
********************************************************************/
be_local_closure(class_Animation__apply_opacity,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animation,     /* shared constants */
    be_str_weak(_apply_opacity),
    &be_const_str_solidified,
    ( &(const binstruction[55]) {  /* code */
      0x6010000F,  //  0000  GETGBL	R4	G15
      0x5C140400,  //  0001  MOVE	R5	R2
      0xB81A0400,  //  0002  GETNGBL	R6	K2
      0x88180D02,  //  0003  GETMBR	R6	R6	K2
      0x7C100400,  //  0004  CALL	R4	2
      0x78120023,  //  0005  JMPF	R4	#002A
      0x5C100400,  //  0006  MOVE	R4	R2
      0x88140103,  //  0007  GETMBR	R5	R0	K3
      0x4C180000,  //  0008  LDNIL	R6
      0x1C140A06,  //  0009  EQ	R5	R5	R6
      0x74160004,  //  000A  JMPT	R5	#0010
      0x88140103,  //  000B  GETMBR	R5	R0	K3
      0x88140B04,  //  000C  GETMBR	R5	R5	K4
      0x88180304,  //  000D  GETMBR	R6	R1	K4
      0x20140A06,  //  000E  NE	R5	R5	R6
      0x78160004,  //  000F  JMPF	R5	#0015
      0xB8160400,  //  0010  GETNGBL	R5	K2
      0x8C140B05,  //  0011  GETMET	R5	R5	K5
      0x881C0304,  //  0012  GETMBR	R7	R1	K4
      0x7C140400,  //  0013  CALL	R5	2
      0x90020605,  //  0014  SETMBR	R0	K3	R5
      0x88140103,  //  0015  GETMBR	R5	R0	K3
      0x8C140B06,  //  0016  GETMET	R5	R5	K6
      0x7C140200,  //  0017  CALL	R5	1
      0x88140907,  //  0018  GETMBR	R5	R4	K7
      0x74160002,  //  0019  JMPT	R5	#001D
      0x8C140908,  //  001A  GETMET	R5	R4	K8
      0x881C0109,  //  001B  GETMBR	R7	R0	K9
      0x7C140400,  //  001C  CALL	R5	2
      0x8C14090A,  //  001D  GETMET	R5	R4	K10
      0x5C1C0600,  //  001E  MOVE	R7	R3
      0x7C140400,  //  001F  CALL	R5	2
      0x8C14090B,  //  0020  GETMET	R5	R4	K11
      0x881C0103,  //  0021  GETMBR	R7	R0	K3
      0x5C200600,  //  0022  MOVE	R8	R3
      0x7C140600,  //  0023  CALL	R5	3
      0x8C14030C,  //  0024  GETMET	R5	R1	K12
      0x881C030D,  //  0025  GETMBR	R7	R1	K13
      0x88200103,  //  0026  GETMBR	R8	R0	K3
      0x8820110D,  //  0027  GETMBR	R8	R8	K13
      0x7C140600,  //  0028  CALL	R5	3
      0x7002000B,  //  0029  JMP		#0036
      0x60100004,  //  002A  GETGBL	R4	G4
      0x5C140400,  //  002B  MOVE	R5	R2
      0x7C100200,  //  002C  CALL	R4	1
      0x1C10090E,  //  002D  EQ	R4	R4	K14
      0x78120006,  //  002E  JMPF	R4	#0036
      0x541200FE,  //  002F  LDINT	R4	255
      0x14100404,  //  0030  LT	R4	R2	R4
      0x78120003,  //  0031  JMPF	R4	#0036
      0x8C10030C,  //  0032  GETMET	R4	R1	K12
      0x8818030D,  //  0033  GETMBR	R6	R1	K13
      0x5C1C0400,  //  0034  MOVE	R7	R2
      0x7C100600,  //  0035  CALL	R4	3
      0x80000000,  //  0036  RET	0
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
    ( &(const binstruction[31]) {  /* code */
      0x8C08010F,  //  0000  GETMET	R2	R0	K15
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x5C040400,  //  0003  MOVE	R1	R2
      0x88080107,  //  0004  GETMBR	R2	R0	K7
      0x5C0C0400,  //  0005  MOVE	R3	R2
      0x740E0001,  //  0006  JMPT	R3	#0009
      0x500C0000,  //  0007  LDBOOL	R3	0	0
      0x80040600,  //  0008  RET	1	R3
      0x880C0109,  //  0009  GETMBR	R3	R0	K9
      0x040C0203,  //  000A  SUB	R3	R1	R3
      0x88100110,  //  000B  GETMBR	R4	R0	K16
      0x88140111,  //  000C  GETMBR	R5	R0	K17
      0x24180912,  //  000D  GT	R6	R4	K18
      0x781A000D,  //  000E  JMPF	R6	#001D
      0x28180604,  //  000F  GE	R6	R3	R4
      0x781A000B,  //  0010  JMPF	R6	#001D
      0x78160005,  //  0011  JMPF	R5	#0018
      0x0C180604,  //  0012  DIV	R6	R3	R4
      0x881C0109,  //  0013  GETMBR	R7	R0	K9
      0x08200C04,  //  0014  MUL	R8	R6	R4
      0x001C0E08,  //  0015  ADD	R7	R7	R8
      0x90021207,  //  0016  SETMBR	R0	K9	R7
      0x70020004,  //  0017  JMP		#001D
      0x88180113,  //  0018  GETMBR	R6	R0	K19
      0x501C0000,  //  0019  LDBOOL	R7	0	0
      0x981A0E07,  //  001A  SETIDX	R6	K7	R7
      0x50180000,  //  001B  LDBOOL	R6	0	0
      0x80040C00,  //  001C  RET	1	R6
      0x50180200,  //  001D  LDBOOL	R6	1	0
      0x80040C00,  //  001E  RET	1	R6
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
    ( &(const binstruction[ 7]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080514,  //  0003  GETMET	R2	R2	K20
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x80000000,  //  0006  RET	0
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
      0x8C080115,  //  0000  GETMET	R2	R0	K21
      0x58100012,  //  0001  LDCONST	R4	K18
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C080600,  //  0003  CALL	R2	3
      0x80040400,  //  0004  RET	1	R2
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
      0x8C0C010F,  //  0000  GETMET	R3	R0	K15
      0x5C140400,  //  0001  MOVE	R5	R2
      0x7C0C0400,  //  0002  CALL	R3	2
      0x5C080600,  //  0003  MOVE	R2	R3
      0x880C0107,  //  0004  GETMBR	R3	R0	K7
      0x5C100600,  //  0005  MOVE	R4	R3
      0x78120002,  //  0006  JMPF	R4	#000A
      0x4C100000,  //  0007  LDNIL	R4
      0x1C100204,  //  0008  EQ	R4	R1	R4
      0x78120001,  //  0009  JMPF	R4	#000C
      0x50100000,  //  000A  LDBOOL	R4	0	0
      0x80040800,  //  000B  RET	1	R4
      0x8C10010A,  //  000C  GETMET	R4	R0	K10
      0x5C180400,  //  000D  MOVE	R6	R2
      0x7C100400,  //  000E  CALL	R4	2
      0x88100101,  //  000F  GETMBR	R4	R0	K1
      0x20140912,  //  0010  NE	R5	R4	K18
      0x78160003,  //  0011  JMPF	R5	#0016
      0x8C140316,  //  0012  GETMET	R5	R1	K22
      0x881C030D,  //  0013  GETMBR	R7	R1	K13
      0x5C200800,  //  0014  MOVE	R8	R4
      0x7C140600,  //  0015  CALL	R5	3
      0x50140200,  //  0016  LDBOOL	R5	1	0
      0x80040A00,  //  0017  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: post_render
********************************************************************/
be_local_closure(class_Animation_post_render,   /* name */
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
    be_str_weak(post_render),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x880C0117,  //  0000  GETMBR	R3	R0	K23
      0x8C100118,  //  0001  GETMET	R4	R0	K24
      0x5C180200,  //  0002  MOVE	R6	R1
      0x5C1C0600,  //  0003  MOVE	R7	R3
      0x5C200400,  //  0004  MOVE	R8	R2
      0x7C100800,  //  0005  CALL	R4	4
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


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
      0x58080019,  //  0001  LDCONST	R2	K25
      0x880C011A,  //  0002  GETMBR	R3	R0	K26
      0x8810011B,  //  0003  GETMBR	R4	R0	K27
      0x88140110,  //  0004  GETMBR	R5	R0	K16
      0x88180111,  //  0005  GETMBR	R6	R0	K17
      0x881C0107,  //  0006  GETMBR	R7	R0	K7
      0x7C040C00,  //  0007  CALL	R1	6
      0x80040200,  //  0008  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Animation
********************************************************************/
extern const bclass be_class_ParameterizedObject;
be_local_class(Animation,
    1,
    &be_class_ParameterizedObject,
    be_nested_map(10,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(opacity_frame, -1), be_const_var(0) },
        { be_const_key_weak(get_color_at, 9), be_const_closure(class_Animation_get_color_at_closure) },
        { be_const_key_weak(_apply_opacity, -1), be_const_closure(class_Animation__apply_opacity_closure) },
        { be_const_key_weak(PARAMS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(priority, -1), be_const_bytes_instance(050000000A) },
        { be_const_key_weak(color, -1), be_const_bytes_instance(0400FF) },
        { be_const_key_weak(name, -1), be_const_bytes_instance(0C0309616E696D6174696F6E01) },
        { be_const_key_weak(loop, 0), be_const_bytes_instance(0C050003) },
        { be_const_key_weak(opacity, 1), be_const_bytes_instance(0C01FF0004) },
        { be_const_key_weak(duration, -1), be_const_bytes_instance(0500000000) },
    }))    ) } )) },
        { be_const_key_weak(update, -1), be_const_closure(class_Animation_update_closure) },
        { be_const_key_weak(init, 6), be_const_closure(class_Animation_init_closure) },
        { be_const_key_weak(tostring, -1), be_const_closure(class_Animation_tostring_closure) },
        { be_const_key_weak(render, -1), be_const_closure(class_Animation_render_closure) },
        { be_const_key_weak(post_render, -1), be_const_closure(class_Animation_post_render_closure) },
        { be_const_key_weak(get_color, -1), be_const_closure(class_Animation_get_color_closure) },
    })),
    be_str_weak(Animation)
);

/********************************************************************
** Solidified function: is_user_function
********************************************************************/
be_local_closure(is_user_function,   /* name */
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
    /* K1   */  be_nested_str_weak(_user_functions),
    /* K2   */  be_nested_str_weak(contains),
    }),
    be_str_weak(is_user_function),
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


/********************************************************************
** Solidified function: register_user_function
********************************************************************/
be_local_closure(register_user_function,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(_user_functions),
    }),
    be_str_weak(register_user_function),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x88080501,  //  0001  GETMBR	R2	R2	K1
      0x98080001,  //  0002  SETIDX	R2	R0	R1
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/

// compact class 'BeaconAnimation' ktab size: 17, total: 21 (saved 32 bytes)
static const bvalue be_ktab_class_BeaconAnimation[17] = {
  /* K0   */  be_nested_str_weak(_fix_time_ms),
  /* K1   */  be_nested_str_weak(width),
  /* K2   */  be_nested_str_weak(back_color),
  /* K3   */  be_nested_str_weak(pos),
  /* K4   */  be_nested_str_weak(slew_size),
  /* K5   */  be_nested_str_weak(beacon_size),
  /* K6   */  be_nested_str_weak(color),
  /* K7   */  be_const_int(-16777216),
  /* K8   */  be_nested_str_weak(fill_pixels),
  /* K9   */  be_nested_str_weak(pixels),
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
      0x8C0C0100,  //  0005  GETMET	R3	R0	K0
      0x5C140400,  //  0006  MOVE	R5	R2
      0x7C0C0400,  //  0007  CALL	R3	2
      0x5C080600,  //  0008  MOVE	R2	R3
      0x880C0301,  //  0009  GETMBR	R3	R1	K1
      0x88100102,  //  000A  GETMBR	R4	R0	K2
      0x88140103,  //  000B  GETMBR	R5	R0	K3
      0x88180104,  //  000C  GETMBR	R6	R0	K4
      0x881C0105,  //  000D  GETMBR	R7	R0	K5
      0x88200106,  //  000E  GETMBR	R8	R0	K6
      0x20240907,  //  000F  NE	R9	R4	K7
      0x78260003,  //  0010  JMPF	R9	#0015
      0x8C240308,  //  0011  GETMET	R9	R1	K8
      0x882C0309,  //  0012  GETMBR	R11	R1	K9
      0x5C300800,  //  0013  MOVE	R12	R4
      0x7C240600,  //  0014  CALL	R9	3
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
      0x880C0106,  //  0002  GETMBR	R3	R0	K6
      0x88100103,  //  0003  GETMBR	R4	R0	K3
      0x88140105,  //  0004  GETMBR	R5	R0	K5
      0x88180104,  //  0005  GETMBR	R6	R0	K4
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
        { be_const_key_weak(slew_size, 4), be_const_bytes_instance(0500000000) },
        { be_const_key_weak(color, -1), be_const_bytes_instance(0400FF) },
        { be_const_key_weak(back_color, 0), be_const_bytes_instance(0402000000FF) },
        { be_const_key_weak(beacon_size, 1), be_const_bytes_instance(0500000001) },
        { be_const_key_weak(pos, -1), be_const_bytes_instance(040000) },
    }))    ) } )) },
        { be_const_key_weak(render, 2), be_const_closure(class_BeaconAnimation_render_closure) },
        { be_const_key_weak(tostring, -1), be_const_closure(class_BeaconAnimation_tostring_closure) },
    })),
    be_str_weak(BeaconAnimation)
);

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


/********************************************************************
** Solidified function: animation_resolve
********************************************************************/
be_local_closure(animation_resolve,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(is_value_provider),
    /* K2   */  be_nested_str_weak(produce_value),
    /* K3   */  be_nested_str_weak(parameterized_object),
    /* K4   */  be_nested_str_weak(value_error),
    /* K5   */  be_nested_str_weak(Parameter_X20name_X20cannot_X20be_X20nil_X20when_X20resolving_X20object_X20parameter),
    /* K6   */  be_nested_str_weak(get_param_value),
    }),
    be_str_weak(animation_resolve),
    &be_const_str_solidified,
    ( &(const binstruction[31]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x5C140000,  //  0002  MOVE	R5	R0
      0x7C0C0400,  //  0003  CALL	R3	2
      0x780E0005,  //  0004  JMPF	R3	#000B
      0x8C0C0102,  //  0005  GETMET	R3	R0	K2
      0x5C140200,  //  0006  MOVE	R5	R1
      0x5C180400,  //  0007  MOVE	R6	R2
      0x7C0C0600,  //  0008  CALL	R3	3
      0x80040600,  //  0009  RET	1	R3
      0x70020012,  //  000A  JMP		#001E
      0x4C0C0000,  //  000B  LDNIL	R3
      0x200C0003,  //  000C  NE	R3	R0	R3
      0x780E000E,  //  000D  JMPF	R3	#001D
      0x600C000F,  //  000E  GETGBL	R3	G15
      0x5C100000,  //  000F  MOVE	R4	R0
      0xB8160000,  //  0010  GETNGBL	R5	K0
      0x88140B03,  //  0011  GETMBR	R5	R5	K3
      0x7C0C0400,  //  0012  CALL	R3	2
      0x780E0008,  //  0013  JMPF	R3	#001D
      0x4C0C0000,  //  0014  LDNIL	R3
      0x1C0C0203,  //  0015  EQ	R3	R1	R3
      0x780E0000,  //  0016  JMPF	R3	#0018
      0xB0060905,  //  0017  RAISE	1	K4	K5
      0x8C0C0106,  //  0018  GETMET	R3	R0	K6
      0x5C140200,  //  0019  MOVE	R5	R1
      0x7C0C0400,  //  001A  CALL	R3	2
      0x80040600,  //  001B  RET	1	R3
      0x70020000,  //  001C  JMP		#001E
      0x80040000,  //  001D  RET	1	R0
      0x80000000,  //  001E  RET	0
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
      0x542200FE,  //  0023  LDINT	R8	255
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
        { be_const_key_weak(value_func, -1), be_const_bytes_instance(0C0606) },
    }))    ) } )) },
        { be_const_key_weak(init, -1), be_const_closure(class_PaletteMeterAnimation_init_closure) },
        { be_const_key_weak(_update_value_buffer, -1), be_const_closure(class_PaletteMeterAnimation__update_value_buffer_closure) },
    })),
    be_str_weak(PaletteMeterAnimation)
);
// compact class 'AnimationEngine' ktab size: 61, total: 164 (saved 824 bytes)
static const bvalue be_ktab_class_AnimationEngine[61] = {
  /* K0   */  be_nested_str_weak(animation),
  /* K1   */  be_nested_str_weak(SequenceManager),
  /* K2   */  be_nested_str_weak(remove_sequence_manager),
  /* K3   */  be_nested_str_weak(remove_animation),
  /* K4   */  be_nested_str_weak(animations),
  /* K5   */  be_const_int(0),
  /* K6   */  be_nested_str_weak(update),
  /* K7   */  be_nested_str_weak(is_running),
  /* K8   */  be_const_int(1),
  /* K9   */  be_nested_str_weak(remove),
  /* K10  */  be_nested_str_weak(render_needed),
  /* K11  */  be_nested_str_weak(_clear_strip),
  /* K12  */  be_nested_str_weak(_render_animations),
  /* K13  */  be_nested_str_weak(fast_loop_closure),
  /* K14  */  be_nested_str_weak(tasmota),
  /* K15  */  be_nested_str_weak(remove_fast_loop),
  /* K16  */  be_nested_str_weak(stop),
  /* K17  */  be_nested_str_weak(stop_iteration),
  /* K18  */  be_nested_str_weak(find),
  /* K19  */  be_nested_str_weak(push),
  /* K20  */  be_nested_str_weak(_sort_animations),
  /* K21  */  be_nested_str_weak(start),
  /* K22  */  be_nested_str_weak(time_ms),
  /* K23  */  be_nested_str_weak(AnimationEngine_X28running_X3D_X25s_X29),
  /* K24  */  be_nested_str_weak(iteration_stack),
  /* K25  */  be_nested_str_weak(pop),
  /* K26  */  be_nested_str_weak(strip),
  /* K27  */  be_nested_str_weak(length),
  /* K28  */  be_nested_str_weak(width),
  /* K29  */  be_nested_str_weak(_handle_strip_length_change),
  /* K30  */  be_nested_str_weak(frame_buffer),
  /* K31  */  be_nested_str_weak(resize),
  /* K32  */  be_nested_str_weak(temp_buffer),
  /* K33  */  be_nested_str_weak(millis),
  /* K34  */  be_nested_str_weak(check_strip_length),
  /* K35  */  be_nested_str_weak(last_update),
  /* K36  */  be_nested_str_weak(can_show),
  /* K37  */  be_nested_str_weak(sequence_managers),
  /* K38  */  be_nested_str_weak(_process_events),
  /* K39  */  be_nested_str_weak(_update_and_render),
  /* K40  */  be_nested_str_weak(clear),
  /* K41  */  be_nested_str_weak(priority),
  /* K42  */  be_nested_str_weak(show),
  /* K43  */  be_nested_str_weak(value_error),
  /* K44  */  be_nested_str_weak(strip_X20cannot_X20be_X20nil),
  /* K45  */  be_nested_str_weak(name),
  /* K46  */  be_nested_str_weak(render),
  /* K47  */  be_nested_str_weak(post_render),
  /* K48  */  be_nested_str_weak(blend_pixels),
  /* K49  */  be_nested_str_weak(pixels),
  /* K50  */  be_nested_str_weak(_output_to_strip),
  /* K51  */  be_nested_str_weak(add_fast_loop),
  /* K52  */  be_nested_str_weak(event_manager),
  /* K53  */  be_nested_str_weak(_process_queued_events),
  /* K54  */  be_nested_str_weak(resume),
  /* K55  */  be_nested_str_weak(set_pixel_color),
  /* K56  */  be_nested_str_weak(get_pixel_color),
  /* K57  */  be_nested_str_weak(_add_sequence_manager),
  /* K58  */  be_nested_str_weak(_add_animation),
  /* K59  */  be_nested_str_weak(type_error),
  /* K60  */  be_nested_str_weak(only_X20Animation_X20or_X20SequenceManager),
};


extern const bclass be_class_AnimationEngine;

/********************************************************************
** Solidified function: remove
********************************************************************/
be_local_closure(class_AnimationEngine_remove,   /* name */
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
    be_str_weak(remove),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x6008000F,  //  0000  GETGBL	R2	G15
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0xB8120000,  //  0002  GETNGBL	R4	K0
      0x88100901,  //  0003  GETMBR	R4	R4	K1
      0x7C080400,  //  0004  CALL	R2	2
      0x780A0004,  //  0005  JMPF	R2	#000B
      0x8C080102,  //  0006  GETMET	R2	R0	K2
      0x5C100200,  //  0007  MOVE	R4	R1
      0x7C080400,  //  0008  CALL	R2	2
      0x80040400,  //  0009  RET	1	R2
      0x7002000A,  //  000A  JMP		#0016
      0x6008000F,  //  000B  GETGBL	R2	G15
      0x5C0C0200,  //  000C  MOVE	R3	R1
      0xB8120000,  //  000D  GETNGBL	R4	K0
      0x88100900,  //  000E  GETMBR	R4	R4	K0
      0x7C080400,  //  000F  CALL	R2	2
      0x780A0004,  //  0010  JMPF	R2	#0016
      0x8C080103,  //  0011  GETMET	R2	R0	K3
      0x5C100200,  //  0012  MOVE	R4	R1
      0x7C080400,  //  0013  CALL	R2	2
      0x80040400,  //  0014  RET	1	R2
      0x7001FFFF,  //  0015  JMP		#0016
      0x80000000,  //  0016  RET	0
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
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x80040200,  //  0001  RET	1	R1
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
      0x58080005,  //  0000  LDCONST	R2	K5
      0x580C0005,  //  0001  LDCONST	R3	K5
      0x6010000C,  //  0002  GETGBL	R4	G12
      0x88140104,  //  0003  GETMBR	R5	R0	K4
      0x7C100200,  //  0004  CALL	R4	1
      0x14100604,  //  0005  LT	R4	R3	R4
      0x78120011,  //  0006  JMPF	R4	#0019
      0x88100104,  //  0007  GETMBR	R4	R0	K4
      0x94100803,  //  0008  GETIDX	R4	R4	R3
      0x8C140906,  //  0009  GETMET	R5	R4	K6
      0x5C1C0200,  //  000A  MOVE	R7	R1
      0x7C140400,  //  000B  CALL	R5	2
      0x78160004,  //  000C  JMPF	R5	#0012
      0x88180907,  //  000D  GETMBR	R6	R4	K7
      0x781A0002,  //  000E  JMPF	R6	#0012
      0x00080508,  //  000F  ADD	R2	R2	K8
      0x000C0708,  //  0010  ADD	R3	R3	K8
      0x70020005,  //  0011  JMP		#0018
      0x88180104,  //  0012  GETMBR	R6	R0	K4
      0x8C180D09,  //  0013  GETMET	R6	R6	K9
      0x5C200600,  //  0014  MOVE	R8	R3
      0x7C180400,  //  0015  CALL	R6	2
      0x50180200,  //  0016  LDBOOL	R6	1	0
      0x90021406,  //  0017  SETMBR	R0	K10	R6
      0x7001FFE8,  //  0018  JMP		#0002
      0x1C100505,  //  0019  EQ	R4	R2	K5
      0x78120006,  //  001A  JMPF	R4	#0022
      0x8810010A,  //  001B  GETMBR	R4	R0	K10
      0x78120003,  //  001C  JMPF	R4	#0021
      0x8C10010B,  //  001D  GETMET	R4	R0	K11
      0x7C100200,  //  001E  CALL	R4	1
      0x50100000,  //  001F  LDBOOL	R4	0	0
      0x90021404,  //  0020  SETMBR	R0	K10	R4
      0x80000800,  //  0021  RET	0
      0x8C10010C,  //  0022  GETMET	R4	R0	K12
      0x88180104,  //  0023  GETMBR	R6	R0	K4
      0x5C1C0200,  //  0024  MOVE	R7	R1
      0x7C100600,  //  0025  CALL	R4	3
      0x50100000,  //  0026  LDBOOL	R4	0	0
      0x90021404,  //  0027  SETMBR	R0	K10	R4
      0x80000000,  //  0028  RET	0
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
      0x88040107,  //  0000  GETMBR	R1	R0	K7
      0x78060009,  //  0001  JMPF	R1	#000C
      0x50040000,  //  0002  LDBOOL	R1	0	0
      0x90020E01,  //  0003  SETMBR	R0	K7	R1
      0x8804010D,  //  0004  GETMBR	R1	R0	K13
      0x4C080000,  //  0005  LDNIL	R2
      0x20040202,  //  0006  NE	R1	R1	R2
      0x78060003,  //  0007  JMPF	R1	#000C
      0xB8061C00,  //  0008  GETNGBL	R1	K14
      0x8C04030F,  //  0009  GETMET	R1	R1	K15
      0x880C010D,  //  000A  GETMBR	R3	R0	K13
      0x7C040400,  //  000B  CALL	R1	2
      0x80040000,  //  000C  RET	1	R0
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
      0x88080104,  //  0001  GETMBR	R2	R0	K4
      0x7C040200,  //  0002  CALL	R1	1
      0xA8020006,  //  0003  EXBLK	0	#000B
      0x5C080200,  //  0004  MOVE	R2	R1
      0x7C080000,  //  0005  CALL	R2	0
      0x880C0507,  //  0006  GETMBR	R3	R2	K7
      0x780E0001,  //  0007  JMPF	R3	#000A
      0x8C0C0510,  //  0008  GETMET	R3	R2	K16
      0x7C0C0200,  //  0009  CALL	R3	1
      0x7001FFF8,  //  000A  JMP		#0004
      0x58040011,  //  000B  LDCONST	R1	K17
      0xAC040200,  //  000C  CATCH	R1	1	0
      0xB0080000,  //  000D  RAISE	2	R0	R0
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _add_animation
********************************************************************/
be_local_closure(class_AnimationEngine__add_animation,   /* name */
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
    be_str_weak(_add_animation),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x88080104,  //  0000  GETMBR	R2	R0	K4
      0x8C080512,  //  0001  GETMET	R2	R2	K18
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x4C0C0000,  //  0004  LDNIL	R3
      0x1C080403,  //  0005  EQ	R2	R2	R3
      0x780A000F,  //  0006  JMPF	R2	#0017
      0x88080104,  //  0007  GETMBR	R2	R0	K4
      0x8C080513,  //  0008  GETMET	R2	R2	K19
      0x5C100200,  //  0009  MOVE	R4	R1
      0x7C080400,  //  000A  CALL	R2	2
      0x8C080114,  //  000B  GETMET	R2	R0	K20
      0x7C080200,  //  000C  CALL	R2	1
      0x88080107,  //  000D  GETMBR	R2	R0	K7
      0x780A0002,  //  000E  JMPF	R2	#0012
      0x8C080315,  //  000F  GETMET	R2	R1	K21
      0x88100116,  //  0010  GETMBR	R4	R0	K22
      0x7C080400,  //  0011  CALL	R2	2
      0x50080200,  //  0012  LDBOOL	R2	1	0
      0x90021402,  //  0013  SETMBR	R0	K10	R2
      0x50080200,  //  0014  LDBOOL	R2	1	0
      0x80040400,  //  0015  RET	1	R2
      0x70020001,  //  0016  JMP		#0019
      0x50080000,  //  0017  LDBOOL	R2	0	0
      0x80040400,  //  0018  RET	1	R2
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_AnimationEngine_tostring,   /* name */
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
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x60040018,  //  0000  GETGBL	R1	G24
      0x58080017,  //  0001  LDCONST	R2	K23
      0x880C0107,  //  0002  GETMBR	R3	R0	K7
      0x7C040400,  //  0003  CALL	R1	2
      0x80040200,  //  0004  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: pop_iteration_context
********************************************************************/
be_local_closure(class_AnimationEngine_pop_iteration_context,   /* name */
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
    be_str_weak(pop_iteration_context),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x6004000C,  //  0000  GETGBL	R1	G12
      0x88080118,  //  0001  GETMBR	R2	R0	K24
      0x7C040200,  //  0002  CALL	R1	1
      0x24040305,  //  0003  GT	R1	R1	K5
      0x78060003,  //  0004  JMPF	R1	#0009
      0x88040118,  //  0005  GETMBR	R1	R0	K24
      0x8C040319,  //  0006  GETMET	R1	R1	K25
      0x7C040200,  //  0007  CALL	R1	1
      0x80040200,  //  0008  RET	1	R1
      0x4C040000,  //  0009  LDNIL	R1
      0x80040200,  //  000A  RET	1	R1
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
      0x8804011A,  //  0000  GETMBR	R1	R0	K26
      0x8C04031B,  //  0001  GETMET	R1	R1	K27
      0x7C040200,  //  0002  CALL	R1	1
      0x8808011C,  //  0003  GETMBR	R2	R0	K28
      0x20080202,  //  0004  NE	R2	R1	R2
      0x780A0004,  //  0005  JMPF	R2	#000B
      0x8C08011D,  //  0006  GETMET	R2	R0	K29
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
      0x18080305,  //  0000  LE	R2	R1	K5
      0x780A0000,  //  0001  JMPF	R2	#0003
      0x80000400,  //  0002  RET	0
      0x90023801,  //  0003  SETMBR	R0	K28	R1
      0x8808011E,  //  0004  GETMBR	R2	R0	K30
      0x8C08051F,  //  0005  GETMET	R2	R2	K31
      0x5C100200,  //  0006  MOVE	R4	R1
      0x7C080400,  //  0007  CALL	R2	2
      0x88080120,  //  0008  GETMBR	R2	R0	K32
      0x8C08051F,  //  0009  GETMET	R2	R2	K31
      0x5C100200,  //  000A  MOVE	R4	R1
      0x7C080400,  //  000B  CALL	R2	2
      0x50080200,  //  000C  LDBOOL	R2	1	0
      0x90021402,  //  000D  SETMBR	R0	K10	R2
      0x80000000,  //  000E  RET	0
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
      0x88080107,  //  0000  GETMBR	R2	R0	K7
      0x740A0001,  //  0001  JMPT	R2	#0004
      0x50080000,  //  0002  LDBOOL	R2	0	0
      0x80040400,  //  0003  RET	1	R2
      0x4C080000,  //  0004  LDNIL	R2
      0x1C080202,  //  0005  EQ	R2	R1	R2
      0x780A0003,  //  0006  JMPF	R2	#000B
      0xB80A1C00,  //  0007  GETNGBL	R2	K14
      0x8C080521,  //  0008  GETMET	R2	R2	K33
      0x7C080200,  //  0009  CALL	R2	1
      0x5C040400,  //  000A  MOVE	R1	R2
      0x8C080122,  //  000B  GETMET	R2	R0	K34
      0x7C080200,  //  000C  CALL	R2	1
      0x90022C01,  //  000D  SETMBR	R0	K22	R1
      0x88080123,  //  000E  GETMBR	R2	R0	K35
      0x04080202,  //  000F  SUB	R2	R1	R2
      0x540E0004,  //  0010  LDINT	R3	5
      0x140C0403,  //  0011  LT	R3	R2	R3
      0x780E0001,  //  0012  JMPF	R3	#0015
      0x500C0200,  //  0013  LDBOOL	R3	1	0
      0x80040600,  //  0014  RET	1	R3
      0x90024601,  //  0015  SETMBR	R0	K35	R1
      0x880C011A,  //  0016  GETMBR	R3	R0	K26
      0x880C0724,  //  0017  GETMBR	R3	R3	K36
      0x4C100000,  //  0018  LDNIL	R4
      0x200C0604,  //  0019  NE	R3	R3	R4
      0x780E0005,  //  001A  JMPF	R3	#0021
      0x880C011A,  //  001B  GETMBR	R3	R0	K26
      0x8C0C0724,  //  001C  GETMET	R3	R3	K36
      0x7C0C0200,  //  001D  CALL	R3	1
      0x740E0001,  //  001E  JMPT	R3	#0021
      0x500C0200,  //  001F  LDBOOL	R3	1	0
      0x80040600,  //  0020  RET	1	R3
      0x580C0005,  //  0021  LDCONST	R3	K5
      0x6010000C,  //  0022  GETGBL	R4	G12
      0x88140125,  //  0023  GETMBR	R5	R0	K37
      0x7C100200,  //  0024  CALL	R4	1
      0x14100604,  //  0025  LT	R4	R3	R4
      0x78120006,  //  0026  JMPF	R4	#002E
      0x88100125,  //  0027  GETMBR	R4	R0	K37
      0x94100803,  //  0028  GETIDX	R4	R4	R3
      0x8C100906,  //  0029  GETMET	R4	R4	K6
      0x5C180200,  //  002A  MOVE	R6	R1
      0x7C100400,  //  002B  CALL	R4	2
      0x000C0708,  //  002C  ADD	R3	R3	K8
      0x7001FFF3,  //  002D  JMP		#0022
      0x8C100126,  //  002E  GETMET	R4	R0	K38
      0x5C180200,  //  002F  MOVE	R6	R1
      0x7C100400,  //  0030  CALL	R4	2
      0x8C100127,  //  0031  GETMET	R4	R0	K39
      0x5C180200,  //  0032  MOVE	R6	R1
      0x7C100400,  //  0033  CALL	R4	2
      0x50100200,  //  0034  LDBOOL	R4	1	0
      0x80040800,  //  0035  RET	1	R4
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
      0x8C040110,  //  0000  GETMET	R1	R0	K16
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040128,  //  0002  GETMET	R1	R0	K40
      0x7C040200,  //  0003  CALL	R1	1
      0x4C040000,  //  0004  LDNIL	R1
      0x90023C01,  //  0005  SETMBR	R0	K30	R1
      0x4C040000,  //  0006  LDNIL	R1
      0x90024001,  //  0007  SETMBR	R0	K32	R1
      0x4C040000,  //  0008  LDNIL	R1
      0x90023401,  //  0009  SETMBR	R0	K26	R1
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


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
      0x88080104,  //  0001  GETMBR	R2	R0	K4
      0x7C040200,  //  0002  CALL	R1	1
      0x18080308,  //  0003  LE	R2	R1	K8
      0x780A0000,  //  0004  JMPF	R2	#0006
      0x80000400,  //  0005  RET	0
      0x58080008,  //  0006  LDCONST	R2	K8
      0x140C0401,  //  0007  LT	R3	R2	R1
      0x780E0016,  //  0008  JMPF	R3	#0020
      0x880C0104,  //  0009  GETMBR	R3	R0	K4
      0x940C0602,  //  000A  GETIDX	R3	R3	R2
      0x5C100400,  //  000B  MOVE	R4	R2
      0x24140905,  //  000C  GT	R5	R4	K5
      0x7816000D,  //  000D  JMPF	R5	#001C
      0x04140908,  //  000E  SUB	R5	R4	K8
      0x88180104,  //  000F  GETMBR	R6	R0	K4
      0x94140C05,  //  0010  GETIDX	R5	R6	R5
      0x88140B29,  //  0011  GETMBR	R5	R5	K41
      0x88180729,  //  0012  GETMBR	R6	R3	K41
      0x14140A06,  //  0013  LT	R5	R5	R6
      0x78160006,  //  0014  JMPF	R5	#001C
      0x88140104,  //  0015  GETMBR	R5	R0	K4
      0x04180908,  //  0016  SUB	R6	R4	K8
      0x881C0104,  //  0017  GETMBR	R7	R0	K4
      0x94180E06,  //  0018  GETIDX	R6	R7	R6
      0x98140806,  //  0019  SETIDX	R5	R4	R6
      0x04100908,  //  001A  SUB	R4	R4	K8
      0x7001FFEF,  //  001B  JMP		#000C
      0x88140104,  //  001C  GETMBR	R5	R0	K4
      0x98140803,  //  001D  SETIDX	R5	R4	R3
      0x00080508,  //  001E  ADD	R2	R2	K8
      0x7001FFE6,  //  001F  JMP		#0007
      0x80000000,  //  0020  RET	0
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
      0x8C040128,  //  0000  GETMET	R1	R0	K40
      0x7C040200,  //  0001  CALL	R1	1
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _add_sequence_manager
********************************************************************/
be_local_closure(class_AnimationEngine__add_sequence_manager,   /* name */
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
    be_str_weak(_add_sequence_manager),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080125,  //  0000  GETMBR	R2	R0	K37
      0x8C080513,  //  0001  GETMET	R2	R2	K19
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80040000,  //  0004  RET	1	R0
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
      0x8804011A,  //  0000  GETMBR	R1	R0	K26
      0x8C040328,  //  0001  GETMET	R1	R1	K40
      0x7C040200,  //  0002  CALL	R1	1
      0x8804011A,  //  0003  GETMBR	R1	R0	K26
      0x8C04032A,  //  0004  GETMET	R1	R1	K42
      0x7C040200,  //  0005  CALL	R1	1
      0x80000000,  //  0006  RET	0
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
    ( &(const binstruction[36]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0000,  //  0002  JMPF	R2	#0004
      0xB006572C,  //  0003  RAISE	1	K43	K44
      0x90023401,  //  0004  SETMBR	R0	K26	R1
      0x8C08031B,  //  0005  GETMET	R2	R1	K27
      0x7C080200,  //  0006  CALL	R2	1
      0x90023802,  //  0007  SETMBR	R0	K28	R2
      0x60080012,  //  0008  GETGBL	R2	G18
      0x7C080000,  //  0009  CALL	R2	0
      0x90020802,  //  000A  SETMBR	R0	K4	R2
      0x60080012,  //  000B  GETGBL	R2	G18
      0x7C080000,  //  000C  CALL	R2	0
      0x90024A02,  //  000D  SETMBR	R0	K37	R2
      0xB80A0000,  //  000E  GETNGBL	R2	K0
      0x8C08051E,  //  000F  GETMET	R2	R2	K30
      0x8810011C,  //  0010  GETMBR	R4	R0	K28
      0x7C080400,  //  0011  CALL	R2	2
      0x90023C02,  //  0012  SETMBR	R0	K30	R2
      0xB80A0000,  //  0013  GETNGBL	R2	K0
      0x8C08051E,  //  0014  GETMET	R2	R2	K30
      0x8810011C,  //  0015  GETMBR	R4	R0	K28
      0x7C080400,  //  0016  CALL	R2	2
      0x90024002,  //  0017  SETMBR	R0	K32	R2
      0x50080000,  //  0018  LDBOOL	R2	0	0
      0x90020E02,  //  0019  SETMBR	R0	K7	R2
      0x90024705,  //  001A  SETMBR	R0	K35	K5
      0x90022D05,  //  001B  SETMBR	R0	K22	K5
      0x4C080000,  //  001C  LDNIL	R2
      0x90021A02,  //  001D  SETMBR	R0	K13	R2
      0x50080000,  //  001E  LDBOOL	R2	0	0
      0x90021402,  //  001F  SETMBR	R0	K10	R2
      0x60080012,  //  0020  GETGBL	R2	G18
      0x7C080000,  //  0021  CALL	R2	0
      0x90023002,  //  0022  SETMBR	R0	K24	R2
      0x80000000,  //  0023  RET	0
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
      0x58080005,  //  0000  LDCONST	R2	K5
      0x600C000C,  //  0001  GETGBL	R3	G12
      0x88100104,  //  0002  GETMBR	R4	R0	K4
      0x7C0C0200,  //  0003  CALL	R3	1
      0x140C0403,  //  0004  LT	R3	R2	R3
      0x780E0012,  //  0005  JMPF	R3	#0019
      0x880C0104,  //  0006  GETMBR	R3	R0	K4
      0x940C0602,  //  0007  GETIDX	R3	R3	R2
      0x8810072D,  //  0008  GETMBR	R4	R3	K45
      0x4C140000,  //  0009  LDNIL	R5
      0x20100805,  //  000A  NE	R4	R4	R5
      0x7812000A,  //  000B  JMPF	R4	#0017
      0x8810072D,  //  000C  GETMBR	R4	R3	K45
      0x1C100801,  //  000D  EQ	R4	R4	R1
      0x78120007,  //  000E  JMPF	R4	#0017
      0x8C100710,  //  000F  GETMET	R4	R3	K16
      0x5C180600,  //  0010  MOVE	R6	R3
      0x7C100400,  //  0011  CALL	R4	2
      0x88100104,  //  0012  GETMBR	R4	R0	K4
      0x8C100909,  //  0013  GETMET	R4	R4	K9
      0x5C180400,  //  0014  MOVE	R6	R2
      0x7C100400,  //  0015  CALL	R4	2
      0x80000800,  //  0016  RET	0
      0x00080508,  //  0017  ADD	R2	R2	K8
      0x7001FFE7,  //  0018  JMP		#0001
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _render_animations
********************************************************************/
be_local_closure(class_AnimationEngine__render_animations,   /* name */
  be_nested_proto(
    10,                          /* nstack */
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
    ( &(const binstruction[34]) {  /* code */
      0x880C011E,  //  0000  GETMBR	R3	R0	K30
      0x8C0C0728,  //  0001  GETMET	R3	R3	K40
      0x7C0C0200,  //  0002  CALL	R3	1
      0x580C0005,  //  0003  LDCONST	R3	K5
      0x6010000C,  //  0004  GETGBL	R4	G12
      0x5C140200,  //  0005  MOVE	R5	R1
      0x7C100200,  //  0006  CALL	R4	1
      0x14100604,  //  0007  LT	R4	R3	R4
      0x78120015,  //  0008  JMPF	R4	#001F
      0x94100203,  //  0009  GETIDX	R4	R1	R3
      0x88140120,  //  000A  GETMBR	R5	R0	K32
      0x8C140B28,  //  000B  GETMET	R5	R5	K40
      0x7C140200,  //  000C  CALL	R5	1
      0x8C14092E,  //  000D  GETMET	R5	R4	K46
      0x881C0120,  //  000E  GETMBR	R7	R0	K32
      0x5C200400,  //  000F  MOVE	R8	R2
      0x7C140600,  //  0010  CALL	R5	3
      0x7816000A,  //  0011  JMPF	R5	#001D
      0x8C18092F,  //  0012  GETMET	R6	R4	K47
      0x88200120,  //  0013  GETMBR	R8	R0	K32
      0x5C240400,  //  0014  MOVE	R9	R2
      0x7C180600,  //  0015  CALL	R6	3
      0x8818011E,  //  0016  GETMBR	R6	R0	K30
      0x8C180D30,  //  0017  GETMET	R6	R6	K48
      0x8820011E,  //  0018  GETMBR	R8	R0	K30
      0x88201131,  //  0019  GETMBR	R8	R8	K49
      0x88240120,  //  001A  GETMBR	R9	R0	K32
      0x88241331,  //  001B  GETMBR	R9	R9	K49
      0x7C180600,  //  001C  CALL	R6	3
      0x000C0708,  //  001D  ADD	R3	R3	K8
      0x7001FFE4,  //  001E  JMP		#0004
      0x8C100132,  //  001F  GETMET	R4	R0	K50
      0x7C100200,  //  0020  CALL	R4	1
      0x80000000,  //  0021  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: run
********************************************************************/
be_local_closure(class_AnimationEngine_run,   /* name */
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
    be_str_weak(run),
    &be_const_str_solidified,
    ( &(const binstruction[48]) {  /* code */
      0x88040107,  //  0000  GETMBR	R1	R0	K7
      0x7406002B,  //  0001  JMPT	R1	#002E
      0xB8061C00,  //  0002  GETNGBL	R1	K14
      0x8C040321,  //  0003  GETMET	R1	R1	K33
      0x7C040200,  //  0004  CALL	R1	1
      0x50080200,  //  0005  LDBOOL	R2	1	0
      0x90020E02,  //  0006  SETMBR	R0	K7	R2
      0x540A0009,  //  0007  LDINT	R2	10
      0x04080202,  //  0008  SUB	R2	R1	R2
      0x90024602,  //  0009  SETMBR	R0	K35	R2
      0x8808010D,  //  000A  GETMBR	R2	R0	K13
      0x4C0C0000,  //  000B  LDNIL	R3
      0x1C080403,  //  000C  EQ	R2	R2	R3
      0x780A0001,  //  000D  JMPF	R2	#0010
      0x84080000,  //  000E  CLOSURE	R2	P0
      0x90021A02,  //  000F  SETMBR	R0	K13	R2
      0x58080005,  //  0010  LDCONST	R2	K5
      0x600C000C,  //  0011  GETGBL	R3	G12
      0x88100104,  //  0012  GETMBR	R4	R0	K4
      0x7C0C0200,  //  0013  CALL	R3	1
      0x140C0403,  //  0014  LT	R3	R2	R3
      0x780E0006,  //  0015  JMPF	R3	#001D
      0x880C0104,  //  0016  GETMBR	R3	R0	K4
      0x940C0602,  //  0017  GETIDX	R3	R3	R2
      0x8C0C0715,  //  0018  GETMET	R3	R3	K21
      0x5C140200,  //  0019  MOVE	R5	R1
      0x7C0C0400,  //  001A  CALL	R3	2
      0x00080508,  //  001B  ADD	R2	R2	K8
      0x7001FFF3,  //  001C  JMP		#0011
      0x58080005,  //  001D  LDCONST	R2	K5
      0x600C000C,  //  001E  GETGBL	R3	G12
      0x88100125,  //  001F  GETMBR	R4	R0	K37
      0x7C0C0200,  //  0020  CALL	R3	1
      0x140C0403,  //  0021  LT	R3	R2	R3
      0x780E0006,  //  0022  JMPF	R3	#002A
      0x880C0125,  //  0023  GETMBR	R3	R0	K37
      0x940C0602,  //  0024  GETIDX	R3	R3	R2
      0x8C0C0715,  //  0025  GETMET	R3	R3	K21
      0x5C140200,  //  0026  MOVE	R5	R1
      0x7C0C0400,  //  0027  CALL	R3	2
      0x00080508,  //  0028  ADD	R2	R2	K8
      0x7001FFF3,  //  0029  JMP		#001E
      0xB80E1C00,  //  002A  GETNGBL	R3	K14
      0x8C0C0733,  //  002B  GETMET	R3	R3	K51
      0x8814010D,  //  002C  GETMBR	R5	R0	K13
      0x7C0C0400,  //  002D  CALL	R3	2
      0xA0000000,  //  002E  CLOSE	R0
      0x80040000,  //  002F  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_current_iteration
********************************************************************/
be_local_closure(class_AnimationEngine_update_current_iteration,   /* name */
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
    be_str_weak(update_current_iteration),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x6008000C,  //  0000  GETGBL	R2	G12
      0x880C0118,  //  0001  GETMBR	R3	R0	K24
      0x7C080200,  //  0002  CALL	R2	1
      0x24080505,  //  0003  GT	R2	R2	K5
      0x780A0002,  //  0004  JMPF	R2	#0008
      0x88080118,  //  0005  GETMBR	R2	R0	K24
      0x540DFFFE,  //  0006  LDINT	R3	-1
      0x98080601,  //  0007  SETIDX	R2	R3	R1
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_current_iteration_number
********************************************************************/
be_local_closure(class_AnimationEngine_get_current_iteration_number,   /* name */
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
    be_str_weak(get_current_iteration_number),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x6004000C,  //  0000  GETGBL	R1	G12
      0x88080118,  //  0001  GETMBR	R2	R0	K24
      0x7C040200,  //  0002  CALL	R1	1
      0x24040305,  //  0003  GT	R1	R1	K5
      0x78060003,  //  0004  JMPF	R1	#0009
      0x88040118,  //  0005  GETMBR	R1	R0	K24
      0x5409FFFE,  //  0006  LDINT	R2	-1
      0x94040202,  //  0007  GETIDX	R1	R1	R2
      0x80040200,  //  0008  RET	1	R1
      0x4C040000,  //  0009  LDNIL	R1
      0x80040200,  //  000A  RET	1	R1
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
      0x88040107,  //  0000  GETMBR	R1	R0	K7
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
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x88080534,  //  0001  GETMBR	R2	R2	K52
      0x4C0C0000,  //  0002  LDNIL	R3
      0x20080403,  //  0003  NE	R2	R2	R3
      0x780A0003,  //  0004  JMPF	R2	#0009
      0xB80A0000,  //  0005  GETNGBL	R2	K0
      0x88080534,  //  0006  GETMBR	R2	R2	K52
      0x8C080535,  //  0007  GETMET	R2	R2	K53
      0x7C080200,  //  0008  CALL	R2	1
      0x80000000,  //  0009  RET	0
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
      0x8804011A,  //  0000  GETMBR	R1	R0	K26
      0x80040200,  //  0001  RET	1	R1
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
      0x88080104,  //  0001  GETMBR	R2	R0	K4
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
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
      0x8C080136,  //  0000  GETMET	R2	R0	K54
      0x7C080200,  //  0001  CALL	R2	1
      0x80000000,  //  0002  RET	0
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
      0x58040005,  //  0000  LDCONST	R1	K5
      0x8808011C,  //  0001  GETMBR	R2	R0	K28
      0x14080202,  //  0002  LT	R2	R1	R2
      0x780A0009,  //  0003  JMPF	R2	#000E
      0x8808011A,  //  0004  GETMBR	R2	R0	K26
      0x8C080537,  //  0005  GETMET	R2	R2	K55
      0x5C100200,  //  0006  MOVE	R4	R1
      0x8814011E,  //  0007  GETMBR	R5	R0	K30
      0x8C140B38,  //  0008  GETMET	R5	R5	K56
      0x5C1C0200,  //  0009  MOVE	R7	R1
      0x7C140400,  //  000A  CALL	R5	2
      0x7C080600,  //  000B  CALL	R2	3
      0x00040308,  //  000C  ADD	R1	R1	K8
      0x7001FFF2,  //  000D  JMP		#0001
      0x8808011A,  //  000E  GETMBR	R2	R0	K26
      0x8C08052A,  //  000F  GETMET	R2	R2	K42
      0x7C080200,  //  0010  CALL	R2	1
      0x80000000,  //  0011  RET	0
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
      0x88040107,  //  0000  GETMBR	R1	R0	K7
      0x74060001,  //  0001  JMPT	R1	#0004
      0x8C040115,  //  0002  GETMET	R1	R0	K21
      0x7C040200,  //  0003  CALL	R1	1
      0x80000000,  //  0004  RET	0
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
      0x580C0005,  //  0001  LDCONST	R3	K5
      0x6010000C,  //  0002  GETGBL	R4	G12
      0x88140125,  //  0003  GETMBR	R5	R0	K37
      0x7C100200,  //  0004  CALL	R4	1
      0x14100604,  //  0005  LT	R4	R3	R4
      0x78120007,  //  0006  JMPF	R4	#000F
      0x88100125,  //  0007  GETMBR	R4	R0	K37
      0x94100803,  //  0008  GETIDX	R4	R4	R3
      0x1C100801,  //  0009  EQ	R4	R4	R1
      0x78120001,  //  000A  JMPF	R4	#000D
      0x5C080600,  //  000B  MOVE	R2	R3
      0x70020001,  //  000C  JMP		#000F
      0x000C0708,  //  000D  ADD	R3	R3	K8
      0x7001FFF2,  //  000E  JMP		#0002
      0x28100505,  //  000F  GE	R4	R2	K5
      0x78120005,  //  0010  JMPF	R4	#0017
      0x88100125,  //  0011  GETMBR	R4	R0	K37
      0x8C100909,  //  0012  GETMET	R4	R4	K9
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
      0x90020801,  //  0002  SETMBR	R0	K4	R1
      0x58040005,  //  0003  LDCONST	R1	K5
      0x6008000C,  //  0004  GETGBL	R2	G12
      0x880C0125,  //  0005  GETMBR	R3	R0	K37
      0x7C080200,  //  0006  CALL	R2	1
      0x14080202,  //  0007  LT	R2	R1	R2
      0x780A0005,  //  0008  JMPF	R2	#000F
      0x88080125,  //  0009  GETMBR	R2	R0	K37
      0x94080401,  //  000A  GETIDX	R2	R2	R1
      0x8C080510,  //  000B  GETMET	R2	R2	K16
      0x7C080200,  //  000C  CALL	R2	1
      0x00040308,  //  000D  ADD	R1	R1	K8
      0x7001FFF4,  //  000E  JMP		#0004
      0x60080012,  //  000F  GETGBL	R2	G18
      0x7C080000,  //  0010  CALL	R2	0
      0x90024A02,  //  0011  SETMBR	R0	K37	R2
      0x50080200,  //  0012  LDBOOL	R2	1	0
      0x90021402,  //  0013  SETMBR	R0	K10	R2
      0x80040000,  //  0014  RET	1	R0
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
      0x580C0005,  //  0001  LDCONST	R3	K5
      0x6010000C,  //  0002  GETGBL	R4	G12
      0x88140104,  //  0003  GETMBR	R5	R0	K4
      0x7C100200,  //  0004  CALL	R4	1
      0x14100604,  //  0005  LT	R4	R3	R4
      0x78120007,  //  0006  JMPF	R4	#000F
      0x88100104,  //  0007  GETMBR	R4	R0	K4
      0x94100803,  //  0008  GETIDX	R4	R4	R3
      0x1C100801,  //  0009  EQ	R4	R4	R1
      0x78120001,  //  000A  JMPF	R4	#000D
      0x5C080600,  //  000B  MOVE	R2	R3
      0x70020001,  //  000C  JMP		#000F
      0x000C0708,  //  000D  ADD	R3	R3	K8
      0x7001FFF2,  //  000E  JMP		#0002
      0x28100505,  //  000F  GE	R4	R2	K5
      0x78120007,  //  0010  JMPF	R4	#0019
      0x88100104,  //  0011  GETMBR	R4	R0	K4
      0x8C100909,  //  0012  GETMET	R4	R4	K9
      0x5C180400,  //  0013  MOVE	R6	R2
      0x7C100400,  //  0014  CALL	R4	2
      0x50100200,  //  0015  LDBOOL	R4	1	0
      0x90021404,  //  0016  SETMBR	R0	K10	R4
      0x50100200,  //  0017  LDBOOL	R4	1	0
      0x80040800,  //  0018  RET	1	R4
      0x50100000,  //  0019  LDBOOL	R4	0	0
      0x80040800,  //  001A  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add
********************************************************************/
be_local_closure(class_AnimationEngine_add,   /* name */
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
    be_str_weak(add),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x6008000F,  //  0000  GETGBL	R2	G15
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0xB8120000,  //  0002  GETNGBL	R4	K0
      0x88100901,  //  0003  GETMBR	R4	R4	K1
      0x7C080400,  //  0004  CALL	R2	2
      0x780A0004,  //  0005  JMPF	R2	#000B
      0x8C080139,  //  0006  GETMET	R2	R0	K57
      0x5C100200,  //  0007  MOVE	R4	R1
      0x7C080400,  //  0008  CALL	R2	2
      0x80040400,  //  0009  RET	1	R2
      0x7002000B,  //  000A  JMP		#0017
      0x6008000F,  //  000B  GETGBL	R2	G15
      0x5C0C0200,  //  000C  MOVE	R3	R1
      0xB8120000,  //  000D  GETNGBL	R4	K0
      0x88100900,  //  000E  GETMBR	R4	R4	K0
      0x7C080400,  //  000F  CALL	R2	2
      0x780A0004,  //  0010  JMPF	R2	#0016
      0x8C08013A,  //  0011  GETMET	R2	R0	K58
      0x5C100200,  //  0012  MOVE	R4	R1
      0x7C080400,  //  0013  CALL	R2	2
      0x80040400,  //  0014  RET	1	R2
      0x70020000,  //  0015  JMP		#0017
      0xB006773C,  //  0016  RAISE	1	K59	K60
      0x80000000,  //  0017  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: push_iteration_context
********************************************************************/
be_local_closure(class_AnimationEngine_push_iteration_context,   /* name */
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
    be_str_weak(push_iteration_context),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080118,  //  0000  GETMBR	R2	R0	K24
      0x8C080513,  //  0001  GETMET	R2	R2	K19
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80000000,  //  0004  RET	0
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
      0x8C040122,  //  0000  GETMET	R1	R0	K34
      0x7C040200,  //  0001  CALL	R1	1
      0x8804011C,  //  0002  GETMBR	R1	R0	K28
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: AnimationEngine
********************************************************************/
be_local_class(AnimationEngine,
    12,
    NULL,
    be_nested_map(47,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(remove, 1), be_const_closure(class_AnimationEngine_remove_closure) },
        { be_const_key_weak(_update_and_render, -1), be_const_closure(class_AnimationEngine__update_and_render_closure) },
        { be_const_key_weak(is_running, -1), be_const_var(6) },
        { be_const_key_weak(stop, 19), be_const_closure(class_AnimationEngine_stop_closure) },
        { be_const_key_weak(sequence_managers, 22), be_const_var(3) },
        { be_const_key_weak(interrupt_current, 17), be_const_closure(class_AnimationEngine_interrupt_current_closure) },
        { be_const_key_weak(time_ms, -1), be_const_var(8) },
        { be_const_key_weak(_add_animation, -1), be_const_closure(class_AnimationEngine__add_animation_closure) },
        { be_const_key_weak(tostring, -1), be_const_closure(class_AnimationEngine_tostring_closure) },
        { be_const_key_weak(frame_buffer, -1), be_const_var(4) },
        { be_const_key_weak(push_iteration_context, -1), be_const_closure(class_AnimationEngine_push_iteration_context_closure) },
        { be_const_key_weak(animations, -1), be_const_var(2) },
        { be_const_key_weak(_handle_strip_length_change, -1), be_const_closure(class_AnimationEngine__handle_strip_length_change_closure) },
        { be_const_key_weak(strip, -1), be_const_var(0) },
        { be_const_key_weak(add, -1), be_const_closure(class_AnimationEngine_add_closure) },
        { be_const_key_weak(temp_buffer, -1), be_const_var(5) },
        { be_const_key_weak(cleanup, 27), be_const_closure(class_AnimationEngine_cleanup_closure) },
        { be_const_key_weak(pop_iteration_context, -1), be_const_closure(class_AnimationEngine_pop_iteration_context_closure) },
        { be_const_key_weak(_sort_animations, 42), be_const_closure(class_AnimationEngine__sort_animations_closure) },
        { be_const_key_weak(_clear_strip, -1), be_const_closure(class_AnimationEngine__clear_strip_closure) },
        { be_const_key_weak(fast_loop_closure, 35), be_const_var(9) },
        { be_const_key_weak(init, 24), be_const_closure(class_AnimationEngine_init_closure) },
        { be_const_key_weak(_render_animations, 44), be_const_closure(class_AnimationEngine__render_animations_closure) },
        { be_const_key_weak(remove_sequence_manager, -1), be_const_closure(class_AnimationEngine_remove_sequence_manager_closure) },
        { be_const_key_weak(run, -1), be_const_closure(class_AnimationEngine_run_closure) },
        { be_const_key_weak(update_current_iteration, -1), be_const_closure(class_AnimationEngine_update_current_iteration_closure) },
        { be_const_key_weak(get_current_iteration_number, -1), be_const_closure(class_AnimationEngine_get_current_iteration_number_closure) },
        { be_const_key_weak(render_needed, -1), be_const_var(10) },
        { be_const_key_weak(iteration_stack, -1), be_const_var(11) },
        { be_const_key_weak(is_active, 30), be_const_closure(class_AnimationEngine_is_active_closure) },
        { be_const_key_weak(_process_events, -1), be_const_closure(class_AnimationEngine__process_events_closure) },
        { be_const_key_weak(get_strip, -1), be_const_closure(class_AnimationEngine_get_strip_closure) },
        { be_const_key_weak(size, 9), be_const_closure(class_AnimationEngine_size_closure) },
        { be_const_key_weak(resume_after, -1), be_const_closure(class_AnimationEngine_resume_after_closure) },
        { be_const_key_weak(_output_to_strip, -1), be_const_closure(class_AnimationEngine__output_to_strip_closure) },
        { be_const_key_weak(last_update, -1), be_const_var(7) },
        { be_const_key_weak(resume, -1), be_const_closure(class_AnimationEngine_resume_closure) },
        { be_const_key_weak(get_animations, 23), be_const_closure(class_AnimationEngine_get_animations_closure) },
        { be_const_key_weak(clear, -1), be_const_closure(class_AnimationEngine_clear_closure) },
        { be_const_key_weak(interrupt_animation, 20), be_const_closure(class_AnimationEngine_interrupt_animation_closure) },
        { be_const_key_weak(width, 14), be_const_var(1) },
        { be_const_key_weak(remove_animation, -1), be_const_closure(class_AnimationEngine_remove_animation_closure) },
        { be_const_key_weak(interrupt_all, -1), be_const_closure(class_AnimationEngine_interrupt_all_closure) },
        { be_const_key_weak(on_tick, 10), be_const_closure(class_AnimationEngine_on_tick_closure) },
        { be_const_key_weak(check_strip_length, -1), be_const_closure(class_AnimationEngine_check_strip_length_closure) },
        { be_const_key_weak(_add_sequence_manager, 2), be_const_closure(class_AnimationEngine__add_sequence_manager_closure) },
        { be_const_key_weak(get_strip_length, -1), be_const_closure(class_AnimationEngine_get_strip_length_closure) },
    })),
    be_str_weak(AnimationEngine)
);

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

// compact class 'PalettePatternAnimation' ktab size: 22, total: 42 (saved 160 bytes)
static const bvalue be_ktab_class_PalettePatternAnimation[22] = {
  /* K0   */  be_nested_str_weak(on_param_changed),
  /* K1   */  be_nested_str_weak(pattern_func),
  /* K2   */  be_nested_str_weak(color_source),
  /* K3   */  be_nested_str_weak(_initialize_value_buffer),
  /* K4   */  be_nested_str_weak(is_running),
  /* K5   */  be_nested_str_weak(_fix_time_ms),
  /* K6   */  be_nested_str_weak(get_param),
  /* K7   */  be_nested_str_weak(get_color_for_value),
  /* K8   */  be_nested_str_weak(start_time),
  /* K9   */  be_nested_str_weak(engine),
  /* K10  */  be_nested_str_weak(get_strip_length),
  /* K11  */  be_const_int(0),
  /* K12  */  be_nested_str_weak(width),
  /* K13  */  be_nested_str_weak(value_buffer),
  /* K14  */  be_nested_str_weak(set_pixel_color),
  /* K15  */  be_const_int(1),
  /* K16  */  be_nested_str_weak(resize),
  /* K17  */  be_nested_str_weak(init),
  /* K18  */  be_nested_str_weak(PalettePatternAnimation_X28strip_length_X3D_X25s_X2C_X20priority_X3D_X25s_X2C_X20running_X3D_X25s_X29),
  /* K19  */  be_nested_str_weak(priority),
  /* K20  */  be_nested_str_weak(update),
  /* K21  */  be_nested_str_weak(_update_value_buffer),
};


extern const bclass be_class_PalettePatternAnimation;

/********************************************************************
** Solidified function: on_param_changed
********************************************************************/
be_local_closure(class_PalettePatternAnimation_on_param_changed,   /* name */
  be_nested_proto(
    7,                          /* nstack */
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
    ( &(const binstruction[14]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0700,  //  0003  GETMET	R3	R3	K0
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x1C0C0301,  //  0007  EQ	R3	R1	K1
      0x740E0001,  //  0008  JMPT	R3	#000B
      0x1C0C0302,  //  0009  EQ	R3	R1	K2
      0x780E0001,  //  000A  JMPF	R3	#000D
      0x8C0C0103,  //  000B  GETMET	R3	R0	K3
      0x7C0C0200,  //  000C  CALL	R3	1
      0x80000000,  //  000D  RET	0
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
    ( &(const binstruction[50]) {  /* code */
      0x880C0104,  //  0000  GETMBR	R3	R0	K4
      0x780E0002,  //  0001  JMPF	R3	#0005
      0x4C0C0000,  //  0002  LDNIL	R3
      0x1C0C0203,  //  0003  EQ	R3	R1	R3
      0x780E0001,  //  0004  JMPF	R3	#0007
      0x500C0000,  //  0005  LDBOOL	R3	0	0
      0x80040600,  //  0006  RET	1	R3
      0x8C0C0105,  //  0007  GETMET	R3	R0	K5
      0x5C140400,  //  0008  MOVE	R5	R2
      0x7C0C0400,  //  0009  CALL	R3	2
      0x5C080600,  //  000A  MOVE	R2	R3
      0x8C0C0106,  //  000B  GETMET	R3	R0	K6
      0x58140002,  //  000C  LDCONST	R5	K2
      0x7C0C0400,  //  000D  CALL	R3	2
      0x4C100000,  //  000E  LDNIL	R4
      0x1C100604,  //  000F  EQ	R4	R3	R4
      0x78120001,  //  0010  JMPF	R4	#0013
      0x50100000,  //  0011  LDBOOL	R4	0	0
      0x80040800,  //  0012  RET	1	R4
      0x88100707,  //  0013  GETMBR	R4	R3	K7
      0x4C140000,  //  0014  LDNIL	R5
      0x1C100805,  //  0015  EQ	R4	R4	R5
      0x78120001,  //  0016  JMPF	R4	#0019
      0x50100000,  //  0017  LDBOOL	R4	0	0
      0x80040800,  //  0018  RET	1	R4
      0x88100108,  //  0019  GETMBR	R4	R0	K8
      0x04100404,  //  001A  SUB	R4	R2	R4
      0x88140109,  //  001B  GETMBR	R5	R0	K9
      0x8C140B0A,  //  001C  GETMET	R5	R5	K10
      0x7C140200,  //  001D  CALL	R5	1
      0x5818000B,  //  001E  LDCONST	R6	K11
      0x141C0C05,  //  001F  LT	R7	R6	R5
      0x781E000E,  //  0020  JMPF	R7	#0030
      0x881C030C,  //  0021  GETMBR	R7	R1	K12
      0x141C0C07,  //  0022  LT	R7	R6	R7
      0x781E000B,  //  0023  JMPF	R7	#0030
      0x881C010D,  //  0024  GETMBR	R7	R0	K13
      0x941C0E06,  //  0025  GETIDX	R7	R7	R6
      0x8C200707,  //  0026  GETMET	R8	R3	K7
      0x5C280E00,  //  0027  MOVE	R10	R7
      0x5C2C0800,  //  0028  MOVE	R11	R4
      0x7C200600,  //  0029  CALL	R8	3
      0x8C24030E,  //  002A  GETMET	R9	R1	K14
      0x5C2C0C00,  //  002B  MOVE	R11	R6
      0x5C301000,  //  002C  MOVE	R12	R8
      0x7C240600,  //  002D  CALL	R9	3
      0x00180D0F,  //  002E  ADD	R6	R6	K15
      0x7001FFEE,  //  002F  JMP		#001F
      0x501C0200,  //  0030  LDBOOL	R7	1	0
      0x80040E00,  //  0031  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _update_value_buffer
********************************************************************/
be_local_closure(class_PalettePatternAnimation__update_value_buffer,   /* name */
  be_nested_proto(
    9,                          /* nstack */
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
    ( &(const binstruction[40]) {  /* code */
      0x88080101,  //  0000  GETMBR	R2	R0	K1
      0x4C0C0000,  //  0001  LDNIL	R3
      0x1C0C0403,  //  0002  EQ	R3	R2	R3
      0x780E0000,  //  0003  JMPF	R3	#0005
      0x80000600,  //  0004  RET	0
      0x880C0109,  //  0005  GETMBR	R3	R0	K9
      0x8C0C070A,  //  0006  GETMET	R3	R3	K10
      0x7C0C0200,  //  0007  CALL	R3	1
      0x6010000C,  //  0008  GETGBL	R4	G12
      0x8814010D,  //  0009  GETMBR	R5	R0	K13
      0x7C100200,  //  000A  CALL	R4	1
      0x20100803,  //  000B  NE	R4	R4	R3
      0x78120003,  //  000C  JMPF	R4	#0011
      0x8810010D,  //  000D  GETMBR	R4	R0	K13
      0x8C100910,  //  000E  GETMET	R4	R4	K16
      0x5C180600,  //  000F  MOVE	R6	R3
      0x7C100400,  //  0010  CALL	R4	2
      0x5810000B,  //  0011  LDCONST	R4	K11
      0x14140803,  //  0012  LT	R5	R4	R3
      0x78160012,  //  0013  JMPF	R5	#0027
      0x5C140400,  //  0014  MOVE	R5	R2
      0x5C180800,  //  0015  MOVE	R6	R4
      0x5C1C0200,  //  0016  MOVE	R7	R1
      0x5C200000,  //  0017  MOVE	R8	R0
      0x7C140600,  //  0018  CALL	R5	3
      0x60180009,  //  0019  GETGBL	R6	G9
      0x5C1C0A00,  //  001A  MOVE	R7	R5
      0x7C180200,  //  001B  CALL	R6	1
      0x141C0D0B,  //  001C  LT	R7	R6	K11
      0x781E0000,  //  001D  JMPF	R7	#001F
      0x5818000B,  //  001E  LDCONST	R6	K11
      0x541E00FE,  //  001F  LDINT	R7	255
      0x241C0C07,  //  0020  GT	R7	R6	R7
      0x781E0000,  //  0021  JMPF	R7	#0023
      0x541A00FE,  //  0022  LDINT	R6	255
      0x881C010D,  //  0023  GETMBR	R7	R0	K13
      0x981C0806,  //  0024  SETIDX	R7	R4	R6
      0x0010090F,  //  0025  ADD	R4	R4	K15
      0x7001FFEA,  //  0026  JMP		#0012
      0x80000000,  //  0027  RET	0
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
      0x88040109,  //  0000  GETMBR	R1	R0	K9
      0x8C04030A,  //  0001  GETMET	R1	R1	K10
      0x7C040200,  //  0002  CALL	R1	1
      0x8808010D,  //  0003  GETMBR	R2	R0	K13
      0x8C080510,  //  0004  GETMET	R2	R2	K16
      0x5C100200,  //  0005  MOVE	R4	R1
      0x7C080400,  //  0006  CALL	R2	2
      0x5808000B,  //  0007  LDCONST	R2	K11
      0x140C0401,  //  0008  LT	R3	R2	R1
      0x780E0003,  //  0009  JMPF	R3	#000E
      0x880C010D,  //  000A  GETMBR	R3	R0	K13
      0x980C050B,  //  000B  SETIDX	R3	R2	K11
      0x0008050F,  //  000C  ADD	R2	R2	K15
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
      0x8C080511,  //  0003  GETMET	R2	R2	K17
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x60080015,  //  0006  GETGBL	R2	G21
      0x7C080000,  //  0007  CALL	R2	0
      0x90021A02,  //  0008  SETMBR	R0	K13	R2
      0x8C080103,  //  0009  GETMET	R2	R0	K3
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
      0x88040109,  //  0000  GETMBR	R1	R0	K9
      0x8C04030A,  //  0001  GETMET	R1	R1	K10
      0x7C040200,  //  0002  CALL	R1	1
      0x60080018,  //  0003  GETGBL	R2	G24
      0x580C0012,  //  0004  LDCONST	R3	K18
      0x5C100200,  //  0005  MOVE	R4	R1
      0x88140113,  //  0006  GETMBR	R5	R0	K19
      0x88180104,  //  0007  GETMBR	R6	R0	K4
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
    ( &(const binstruction[20]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080514,  //  0003  GETMET	R2	R2	K20
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x740A0001,  //  0006  JMPT	R2	#0009
      0x50080000,  //  0007  LDBOOL	R2	0	0
      0x80040400,  //  0008  RET	1	R2
      0x8C080105,  //  0009  GETMET	R2	R0	K5
      0x5C100200,  //  000A  MOVE	R4	R1
      0x7C080400,  //  000B  CALL	R2	2
      0x5C040400,  //  000C  MOVE	R1	R2
      0x88080108,  //  000D  GETMBR	R2	R0	K8
      0x04080202,  //  000E  SUB	R2	R1	R2
      0x8C0C0115,  //  000F  GETMET	R3	R0	K21
      0x5C140400,  //  0010  MOVE	R5	R2
      0x7C0C0400,  //  0011  CALL	R3	2
      0x500C0200,  //  0012  LDBOOL	R3	1	0
      0x80040600,  //  0013  RET	1	R3
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
        { be_const_key_weak(color_source, 1), be_const_bytes_instance(0C0605) },
        { be_const_key_weak(pattern_func, -1), be_const_bytes_instance(0C0606) },
    }))    ) } )) },
        { be_const_key_weak(init, 5), be_const_closure(class_PalettePatternAnimation_init_closure) },
        { be_const_key_weak(render, 0), be_const_closure(class_PalettePatternAnimation_render_closure) },
        { be_const_key_weak(value_buffer, -1), be_const_var(0) },
    })),
    be_str_weak(PalettePatternAnimation)
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

// compact class 'StripLengthProvider' ktab size: 5, total: 7 (saved 16 bytes)
static const bvalue be_ktab_class_StripLengthProvider[5] = {
  /* K0   */  be_nested_str_weak(engine),
  /* K1   */  be_nested_str_weak(width),
  /* K2   */  be_const_int(0),
  /* K3   */  be_nested_str_weak(StripLengthProvider_X28length_X3D_X25s_X29),
  /* K4   */  be_nested_str_weak(unknown),
};


extern const bclass be_class_StripLengthProvider;

/********************************************************************
** Solidified function: produce_value
********************************************************************/
be_local_closure(class_StripLengthProvider_produce_value,   /* name */
  be_nested_proto(
    4,                          /* nstack */
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
    ( &(const binstruction[ 7]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x780E0002,  //  0001  JMPF	R3	#0005
      0x880C0100,  //  0002  GETMBR	R3	R0	K0
      0x880C0701,  //  0003  GETMBR	R3	R3	K1
      0x70020000,  //  0004  JMP		#0006
      0x580C0002,  //  0005  LDCONST	R3	K2
      0x80040600,  //  0006  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_StripLengthProvider_tostring,   /* name */
  be_nested_proto(
    4,                          /* nstack */
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
    ( &(const binstruction[10]) {  /* code */
      0x60040018,  //  0000  GETGBL	R1	G24
      0x58080003,  //  0001  LDCONST	R2	K3
      0x880C0100,  //  0002  GETMBR	R3	R0	K0
      0x780E0002,  //  0003  JMPF	R3	#0007
      0x880C0100,  //  0004  GETMBR	R3	R0	K0
      0x880C0701,  //  0005  GETMBR	R3	R3	K1
      0x70020000,  //  0006  JMP		#0008
      0x580C0004,  //  0007  LDCONST	R3	K4
      0x7C040400,  //  0008  CALL	R1	2
      0x80040200,  //  0009  RET	1	R1
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
    18,                          /* nstack */
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
      0xB82E0C00,  //  002B  GETNGBL	R11	K6
      0x8C2C170B,  //  002C  GETMET	R11	R11	K11
      0x5C341400,  //  002D  MOVE	R13	R10
      0x5439EFFF,  //  002E  LDINT	R14	-4096
      0x543E0FFF,  //  002F  LDINT	R15	4096
      0x58400008,  //  0030  LDCONST	R16	K8
      0x544600FE,  //  0031  LDINT	R17	255
      0x7C2C0C00,  //  0032  CALL	R11	6
      0x88300104,  //  0033  GETMBR	R12	R0	K4
      0x98300E0B,  //  0034  SETIDX	R12	R7	R11
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
        { be_const_key_weak(wave_period, -1), be_const_bytes_instance(050001018813) },
        { be_const_key_weak(wave_length, -1), be_const_bytes_instance(050001000A) },
    }))    ) } )) },
        { be_const_key_weak(init, -1), be_const_closure(class_PaletteWaveAnimation_init_closure) },
        { be_const_key_weak(_update_value_buffer, -1), be_const_closure(class_PaletteWaveAnimation__update_value_buffer_closure) },
    })),
    be_str_weak(PaletteWaveAnimation)
);
// compact class 'ClosureValueProvider' ktab size: 7, total: 9 (saved 16 bytes)
static const bvalue be_ktab_class_ClosureValueProvider[7] = {
  /* K0   */  be_nested_str_weak(ClosureValueProvider_X28_X25s_X29),
  /* K1   */  be_nested_str_weak(_closure),
  /* K2   */  be_nested_str_weak(closure_X20set),
  /* K3   */  be_nested_str_weak(no_X20closure),
  /* K4   */  be_nested_str_weak(on_param_changed),
  /* K5   */  be_nested_str_weak(closure),
  /* K6   */  be_nested_str_weak(engine),
};


extern const bclass be_class_ClosureValueProvider;

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
      0x58080000,  //  0001  LDCONST	R2	K0
      0x880C0101,  //  0002  GETMBR	R3	R0	K1
      0x780E0001,  //  0003  JMPF	R3	#0006
      0x580C0002,  //  0004  LDCONST	R3	K2
      0x70020000,  //  0005  JMP		#0007
      0x580C0003,  //  0006  LDCONST	R3	K3
      0x7C040400,  //  0007  CALL	R1	2
      0x80040200,  //  0008  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: on_param_changed
********************************************************************/
be_local_closure(class_ClosureValueProvider_on_param_changed,   /* name */
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
    be_str_weak(on_param_changed),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0704,  //  0003  GETMET	R3	R3	K4
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x1C0C0305,  //  0007  EQ	R3	R1	K5
      0x780E0000,  //  0008  JMPF	R3	#000A
      0x90020202,  //  0009  SETMBR	R0	K1	R2
      0x80000000,  //  000A  RET	0
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
      0x88140106,  //  0007  GETMBR	R5	R0	K6
      0x5C180200,  //  0008  MOVE	R6	R1
      0x5C1C0400,  //  0009  MOVE	R7	R2
      0x7C100600,  //  000A  CALL	R4	3
      0x80040800,  //  000B  RET	1	R4
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
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(tostring, -1), be_const_closure(class_ClosureValueProvider_tostring_closure) },
        { be_const_key_weak(PARAMS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(closure, -1), be_const_bytes_instance(0C0606) },
    }))    ) } )) },
        { be_const_key_weak(_closure, 4), be_const_var(0) },
        { be_const_key_weak(produce_value, 1), be_const_closure(class_ClosureValueProvider_produce_value_closure) },
        { be_const_key_weak(on_param_changed, -1), be_const_closure(class_ClosureValueProvider_on_param_changed_closure) },
    })),
    be_str_weak(ClosureValueProvider)
);
// compact class 'CrenelPositionAnimation' ktab size: 21, total: 27 (saved 48 bytes)
static const bvalue be_ktab_class_CrenelPositionAnimation[21] = {
  /* K0   */  be_nested_str_weak(is_running),
  /* K1   */  be_nested_str_weak(_fix_time_ms),
  /* K2   */  be_nested_str_weak(width),
  /* K3   */  be_nested_str_weak(back_color),
  /* K4   */  be_nested_str_weak(pos),
  /* K5   */  be_nested_str_weak(pulse_size),
  /* K6   */  be_nested_str_weak(low_size),
  /* K7   */  be_nested_str_weak(nb_pulse),
  /* K8   */  be_nested_str_weak(color),
  /* K9   */  be_const_int(-16777216),
  /* K10  */  be_nested_str_weak(fill_pixels),
  /* K11  */  be_nested_str_weak(pixels),
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
      0x8C0C0101,  //  0007  GETMET	R3	R0	K1
      0x5C140400,  //  0008  MOVE	R5	R2
      0x7C0C0400,  //  0009  CALL	R3	2
      0x5C080600,  //  000A  MOVE	R2	R3
      0x880C0302,  //  000B  GETMBR	R3	R1	K2
      0x88100103,  //  000C  GETMBR	R4	R0	K3
      0x88140104,  //  000D  GETMBR	R5	R0	K4
      0x88180105,  //  000E  GETMBR	R6	R0	K5
      0x881C0106,  //  000F  GETMBR	R7	R0	K6
      0x88200107,  //  0010  GETMBR	R8	R0	K7
      0x88240108,  //  0011  GETMBR	R9	R0	K8
      0x60280009,  //  0012  GETGBL	R10	G9
      0x002C0C07,  //  0013  ADD	R11	R6	R7
      0x7C280200,  //  0014  CALL	R10	1
      0x202C0909,  //  0015  NE	R11	R4	K9
      0x782E0003,  //  0016  JMPF	R11	#001B
      0x8C2C030A,  //  0017  GETMET	R11	R1	K10
      0x8834030B,  //  0018  GETMBR	R13	R1	K11
      0x5C380800,  //  0019  MOVE	R14	R4
      0x7C2C0600,  //  001A  CALL	R11	3
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
      0x58100008,  //  0002  LDCONST	R4	K8
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
      0x88140108,  //  0010  GETMBR	R5	R0	K8
      0x7C0C0400,  //  0011  CALL	R3	2
      0x5C040600,  //  0012  MOVE	R1	R3
      0x600C0018,  //  0013  GETGBL	R3	G24
      0x58100013,  //  0014  LDCONST	R4	K19
      0x5C140200,  //  0015  MOVE	R5	R1
      0x88180104,  //  0016  GETMBR	R6	R0	K4
      0x881C0105,  //  0017  GETMBR	R7	R0	K5
      0x88200106,  //  0018  GETMBR	R8	R0	K6
      0x88240107,  //  0019  GETMBR	R9	R0	K7
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
        be_const_map( *     be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(nb_pulse, -1), be_const_bytes_instance(0400FF) },
        { be_const_key_weak(low_size, 4), be_const_bytes_instance(0500000003) },
        { be_const_key_weak(pos, 1), be_const_bytes_instance(040000) },
        { be_const_key_weak(pulse_size, -1), be_const_bytes_instance(0500000001) },
        { be_const_key_weak(back_color, -1), be_const_bytes_instance(0402000000FF) },
    }))    ) } )) },
        { be_const_key_weak(render, 2), be_const_closure(class_CrenelPositionAnimation_render_closure) },
        { be_const_key_weak(tostring, -1), be_const_closure(class_CrenelPositionAnimation_tostring_closure) },
    })),
    be_str_weak(CrenelPositionAnimation)
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
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(_ntv),
    /* K2   */  be_nested_str_weak(event_manager),
    /* K3   */  be_nested_str_weak(EventManager),
    /* K4   */  be_nested_str_weak(member),
    /* K5   */  be_nested_str_weak(_user_functions),
    }),
    be_str_weak(animation_init),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x6004000B,  //  0000  GETGBL	R1	G11
      0x58080000,  //  0001  LDCONST	R2	K0
      0x7C040200,  //  0002  CALL	R1	1
      0x90060200,  //  0003  SETMBR	R1	K1	R0
      0x8C080103,  //  0004  GETMET	R2	R0	K3
      0x7C080200,  //  0005  CALL	R2	1
      0x90060402,  //  0006  SETMBR	R1	K2	R2
      0x84080000,  //  0007  CLOSURE	R2	P0
      0x90060802,  //  0008  SETMBR	R1	K4	R2
      0x60080013,  //  0009  GETGBL	R2	G19
      0x7C080000,  //  000A  CALL	R2	0
      0x90060A02,  //  000B  SETMBR	R1	K5	R2
      0x80040200,  //  000C  RET	1	R1
    })
  )
);
/*******************************************************************/

extern const bclass be_class_ParameterizedObject;
// compact class 'ParameterizedObject' ktab size: 54, total: 113 (saved 472 bytes)
static const bvalue be_ktab_class_ParameterizedObject[54] = {
  /* K0   */  be_nested_str_weak(values),
  /* K1   */  be_nested_str_weak(contains),
  /* K2   */  be_nested_str_weak(_has_param),
  /* K3   */  be_nested_str_weak(_resolve_parameter_value),
  /* K4   */  be_nested_str_weak(engine),
  /* K5   */  be_nested_str_weak(time_ms),
  /* K6   */  be_nested_str_weak(_X27_X25s_X27_X20object_X20has_X20no_X20attribute_X20_X27_X25s_X27),
  /* K7   */  be_nested_str_weak(attribute_error),
  /* K8   */  be_nested_str_weak(introspect),
  /* K9   */  be_nested_str_weak(toptr),
  /* K10  */  be_nested_str_weak(_set_parameter_value),
  /* K11  */  be_nested_str_weak(value_error),
  /* K12  */  be_nested_str_weak(PARAMS),
  /* K13  */  be_nested_str_weak(keys),
  /* K14  */  be_nested_str_weak(constraint_mask),
  /* K15  */  be_nested_str_weak(default),
  /* K16  */  be_nested_str_weak(constraint_find),
  /* K17  */  be_nested_str_weak(stop_iteration),
  /* K18  */  be_nested_str_weak(start_time),
  /* K19  */  be_nested_str_weak(animation),
  /* K20  */  be_nested_str_weak(is_value_provider),
  /* K21  */  be_nested_str_weak(produce_value),
  /* K22  */  be_nested_str_weak(_get_param_def),
  /* K23  */  be_nested_str_weak(nillable),
  /* K24  */  be_nested_str_weak(_X27_X25s_X27_X20does_X20not_X20accept_X20nil_X20values),
  /* K25  */  be_nested_str_weak(type),
  /* K26  */  be_nested_str_weak(int),
  /* K27  */  be_nested_str_weak(any),
  /* K28  */  be_nested_str_weak(real),
  /* K29  */  be_nested_str_weak(math),
  /* K30  */  be_nested_str_weak(round),
  /* K31  */  be_nested_str_weak(bytes),
  /* K32  */  be_nested_str_weak(instance),
  /* K33  */  be_nested_str_weak(_X27_X25s_X27_X20expects_X20type_X20_X27_X25s_X27_X20but_X20got_X20_X27_X25s_X27_X20_X28value_X3A_X20_X25s_X29),
  /* K34  */  be_nested_str_weak(min),
  /* K35  */  be_nested_str_weak(_X27_X25s_X27_X20value_X20_X25s_X20is_X20below_X20minimum_X20_X25s),
  /* K36  */  be_nested_str_weak(max),
  /* K37  */  be_nested_str_weak(_X27_X25s_X27_X20value_X20_X25s_X20is_X20above_X20maximum_X20_X25s),
  /* K38  */  be_nested_str_weak(enum),
  /* K39  */  be_const_int(0),
  /* K40  */  be_const_int(1),
  /* K41  */  be_nested_str_weak(_X27_X25s_X27_X20value_X20_X25s_X20is_X20not_X20in_X20allowed_X20values_X20_X25s),
  /* K42  */  be_nested_str_weak(_validate_param),
  /* K43  */  be_nested_str_weak(on_param_changed),
  /* K44  */  be_const_class(be_class_ParameterizedObject),
  /* K45  */  be_nested_str_weak(_MASK),
  /* K46  */  be_nested_str_weak(find),
  /* K47  */  be_const_int(2),
  /* K48  */  be_nested_str_weak(_TYPES),
  /* K49  */  be_nested_str_weak(push),
  /* K50  */  be_nested_str_weak(is_running),
  /* K51  */  be_nested_str_weak(start),
  /* K52  */  be_nested_str_weak(missing_X20engine_X20parameter),
  /* K53  */  be_nested_str_weak(_init_parameter_values),
};


extern const bclass be_class_ParameterizedObject;

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
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x740A0003,  //  0004  JMPT	R2	#0009
      0x8C080102,  //  0005  GETMET	R2	R0	K2
      0x5C100200,  //  0006  MOVE	R4	R1
      0x7C080400,  //  0007  CALL	R2	2
      0x780A0005,  //  0008  JMPF	R2	#000F
      0x8C080103,  //  0009  GETMET	R2	R0	K3
      0x5C100200,  //  000A  MOVE	R4	R1
      0x88140104,  //  000B  GETMBR	R5	R0	K4
      0x88140B05,  //  000C  GETMBR	R5	R5	K5
      0x7C080600,  //  000D  CALL	R2	3
      0x80040400,  //  000E  RET	1	R2
      0x60080018,  //  000F  GETGBL	R2	G24
      0x580C0006,  //  0010  LDCONST	R3	K6
      0x60100005,  //  0011  GETGBL	R4	G5
      0x5C140000,  //  0012  MOVE	R5	R0
      0x7C100200,  //  0013  CALL	R4	1
      0x5C140200,  //  0014  MOVE	R5	R1
      0x7C080600,  //  0015  CALL	R2	3
      0xB0060E02,  //  0016  RAISE	1	K7	R2
      0x80000000,  //  0017  RET	0
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
      0xA40A1000,  //  0000  IMPORT	R2	K8
      0x8C0C0509,  //  0001  GETMET	R3	R2	K9
      0x5C140000,  //  0002  MOVE	R5	R0
      0x7C0C0400,  //  0003  CALL	R3	2
      0x8C100509,  //  0004  GETMET	R4	R2	K9
      0x5C180200,  //  0005  MOVE	R6	R1
      0x7C100400,  //  0006  CALL	R4	2
      0x1C0C0604,  //  0007  EQ	R3	R3	R4
      0x80040600,  //  0008  RET	1	R3
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
      0x8C0C0103,  //  0000  GETMET	R3	R0	K3
      0x5C140200,  //  0001  MOVE	R5	R1
      0x5C180400,  //  0002  MOVE	R6	R2
      0x7C0C0600,  //  0003  CALL	R3	3
      0x80040600,  //  0004  RET	1	R3
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
      0x8C0C0102,  //  0000  GETMET	R3	R0	K2
      0x5C140200,  //  0001  MOVE	R5	R1
      0x7C0C0400,  //  0002  CALL	R3	2
      0x740E0001,  //  0003  JMPT	R3	#0006
      0x500C0000,  //  0004  LDBOOL	R3	0	0
      0x80040600,  //  0005  RET	1	R3
      0xA8020008,  //  0006  EXBLK	0	#0010
      0x8C0C010A,  //  0007  GETMET	R3	R0	K10
      0x5C140200,  //  0008  MOVE	R5	R1
      0x5C180400,  //  0009  MOVE	R6	R2
      0x7C0C0600,  //  000A  CALL	R3	3
      0x500C0200,  //  000B  LDBOOL	R3	1	0
      0xA8040001,  //  000C  EXBLK	1	1
      0x80040600,  //  000D  RET	1	R3
      0xA8040001,  //  000E  EXBLK	1	1
      0x70020006,  //  000F  JMP		#0017
      0x580C000B,  //  0010  LDCONST	R3	K11
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
      0xA40A1000,  //  0000  IMPORT	R2	K8
      0x600C0006,  //  0001  GETGBL	R3	G6
      0x5C100000,  //  0002  MOVE	R4	R0
      0x7C0C0200,  //  0003  CALL	R3	1
      0x4C100000,  //  0004  LDNIL	R4
      0x20100604,  //  0005  NE	R4	R3	R4
      0x78120010,  //  0006  JMPF	R4	#0018
      0x8C100501,  //  0007  GETMET	R4	R2	K1
      0x5C180600,  //  0008  MOVE	R6	R3
      0x581C000C,  //  0009  LDCONST	R7	K12
      0x7C100600,  //  000A  CALL	R4	3
      0x78120006,  //  000B  JMPF	R4	#0013
      0x8810070C,  //  000C  GETMBR	R4	R3	K12
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
    12,                          /* nstack */
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
    ( &(const binstruction[47]) {  /* code */
      0xA4061000,  //  0000  IMPORT	R1	K8
      0x60080006,  //  0001  GETGBL	R2	G6
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C080200,  //  0003  CALL	R2	1
      0x4C0C0000,  //  0004  LDNIL	R3
      0x200C0403,  //  0005  NE	R3	R2	R3
      0x780E0026,  //  0006  JMPF	R3	#002E
      0x8C0C0301,  //  0007  GETMET	R3	R1	K1
      0x5C140400,  //  0008  MOVE	R5	R2
      0x5818000C,  //  0009  LDCONST	R6	K12
      0x7C0C0600,  //  000A  CALL	R3	3
      0x780E001C,  //  000B  JMPF	R3	#0029
      0x880C050C,  //  000C  GETMBR	R3	R2	K12
      0x60100010,  //  000D  GETGBL	R4	G16
      0x8C14070D,  //  000E  GETMET	R5	R3	K13
      0x7C140200,  //  000F  CALL	R5	1
      0x7C100200,  //  0010  CALL	R4	1
      0xA8020013,  //  0011  EXBLK	0	#0026
      0x5C140800,  //  0012  MOVE	R5	R4
      0x7C140000,  //  0013  CALL	R5	0
      0x88180100,  //  0014  GETMBR	R6	R0	K0
      0x8C180D01,  //  0015  GETMET	R6	R6	K1
      0x5C200A00,  //  0016  MOVE	R8	R5
      0x7C180400,  //  0017  CALL	R6	2
      0x741A000B,  //  0018  JMPT	R6	#0025
      0x94180605,  //  0019  GETIDX	R6	R3	R5
      0x8C1C010E,  //  001A  GETMET	R7	R0	K14
      0x5C240C00,  //  001B  MOVE	R9	R6
      0x5828000F,  //  001C  LDCONST	R10	K15
      0x7C1C0600,  //  001D  CALL	R7	3
      0x781E0005,  //  001E  JMPF	R7	#0025
      0x881C0100,  //  001F  GETMBR	R7	R0	K0
      0x8C200110,  //  0020  GETMET	R8	R0	K16
      0x5C280C00,  //  0021  MOVE	R10	R6
      0x582C000F,  //  0022  LDCONST	R11	K15
      0x7C200600,  //  0023  CALL	R8	3
      0x981C0A08,  //  0024  SETIDX	R7	R5	R8
      0x7001FFEB,  //  0025  JMP		#0012
      0x58100011,  //  0026  LDCONST	R4	K17
      0xAC100200,  //  0027  CATCH	R4	1	0
      0xB0080000,  //  0028  RAISE	2	R0	R0
      0x600C0003,  //  0029  GETGBL	R3	G3
      0x5C100400,  //  002A  MOVE	R4	R2
      0x7C0C0200,  //  002B  CALL	R3	1
      0x5C080600,  //  002C  MOVE	R2	R3
      0x7001FFD5,  //  002D  JMP		#0004
      0x80000000,  //  002E  RET	0
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
      0xA40A1000,  //  0000  IMPORT	R2	K8
      0x600C0006,  //  0001  GETGBL	R3	G6
      0x5C100000,  //  0002  MOVE	R4	R0
      0x7C0C0200,  //  0003  CALL	R3	1
      0x4C100000,  //  0004  LDNIL	R4
      0x20100604,  //  0005  NE	R4	R3	R4
      0x78120010,  //  0006  JMPF	R4	#0018
      0x8C100501,  //  0007  GETMET	R4	R2	K1
      0x5C180600,  //  0008  MOVE	R6	R3
      0x581C000C,  //  0009  LDCONST	R7	K12
      0x7C100600,  //  000A  CALL	R4	3
      0x78120006,  //  000B  JMPF	R4	#0013
      0x8810070C,  //  000C  GETMBR	R4	R3	K12
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
** Solidified function: _fix_time_ms
********************************************************************/
be_local_closure(class_ParameterizedObject__fix_time_ms,   /* name */
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
    be_str_weak(_fix_time_ms),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x88080104,  //  0003  GETMBR	R2	R0	K4
      0x88040505,  //  0004  GETMBR	R1	R2	K5
      0x88080112,  //  0005  GETMBR	R2	R0	K18
      0x4C0C0000,  //  0006  LDNIL	R3
      0x1C080403,  //  0007  EQ	R2	R2	R3
      0x780A0000,  //  0008  JMPF	R2	#000A
      0x90022401,  //  0009  SETMBR	R0	K18	R1
      0x80040200,  //  000A  RET	1	R1
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
    ( &(const binstruction[13]) {  /* code */
      0xB8122600,  //  0000  GETNGBL	R4	K19
      0x8C100914,  //  0001  GETMET	R4	R4	K20
      0x5C180200,  //  0002  MOVE	R6	R1
      0x7C100400,  //  0003  CALL	R4	2
      0x78120005,  //  0004  JMPF	R4	#000B
      0x8C100315,  //  0005  GETMET	R4	R1	K21
      0x5C180400,  //  0006  MOVE	R6	R2
      0x5C1C0600,  //  0007  MOVE	R7	R3
      0x7C100600,  //  0008  CALL	R4	3
      0x80040800,  //  0009  RET	1	R4
      0x70020000,  //  000A  JMP		#000C
      0x80040200,  //  000B  RET	1	R1
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _validate_param
********************************************************************/
be_local_closure(class_ParameterizedObject__validate_param,   /* name */
  be_nested_proto(
    15,                          /* nstack */
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
    ( &(const binstruction[171]) {  /* code */
      0x8C0C0116,  //  0000  GETMET	R3	R0	K22
      0x5C140200,  //  0001  MOVE	R5	R1
      0x7C0C0400,  //  0002  CALL	R3	2
      0x4C100000,  //  0003  LDNIL	R4
      0x1C100604,  //  0004  EQ	R4	R3	R4
      0x78120007,  //  0005  JMPF	R4	#000E
      0x60100018,  //  0006  GETGBL	R4	G24
      0x58140006,  //  0007  LDCONST	R5	K6
      0x60180005,  //  0008  GETGBL	R6	G5
      0x5C1C0000,  //  0009  MOVE	R7	R0
      0x7C180200,  //  000A  CALL	R6	1
      0x5C1C0200,  //  000B  MOVE	R7	R1
      0x7C100600,  //  000C  CALL	R4	3
      0xB0060E04,  //  000D  RAISE	1	K7	R4
      0xB8122600,  //  000E  GETNGBL	R4	K19
      0x8C100914,  //  000F  GETMET	R4	R4	K20
      0x5C180400,  //  0010  MOVE	R6	R2
      0x7C100400,  //  0011  CALL	R4	2
      0x78120000,  //  0012  JMPF	R4	#0014
      0x80040400,  //  0013  RET	1	R2
      0x4C100000,  //  0014  LDNIL	R4
      0x1C100404,  //  0015  EQ	R4	R2	R4
      0x78120010,  //  0016  JMPF	R4	#0028
      0x8C10010E,  //  0017  GETMET	R4	R0	K14
      0x5C180600,  //  0018  MOVE	R6	R3
      0x581C0017,  //  0019  LDCONST	R7	K23
      0x7C100600,  //  001A  CALL	R4	3
      0x78120000,  //  001B  JMPF	R4	#001D
      0x80040400,  //  001C  RET	1	R2
      0x8C10010E,  //  001D  GETMET	R4	R0	K14
      0x5C180600,  //  001E  MOVE	R6	R3
      0x581C000F,  //  001F  LDCONST	R7	K15
      0x7C100600,  //  0020  CALL	R4	3
      0x78120000,  //  0021  JMPF	R4	#0023
      0x80040400,  //  0022  RET	1	R2
      0x60100018,  //  0023  GETGBL	R4	G24
      0x58140018,  //  0024  LDCONST	R5	K24
      0x5C180200,  //  0025  MOVE	R6	R1
      0x7C100400,  //  0026  CALL	R4	2
      0xB0061604,  //  0027  RAISE	1	K11	R4
      0x8C100110,  //  0028  GETMET	R4	R0	K16
      0x5C180600,  //  0029  MOVE	R6	R3
      0x581C0019,  //  002A  LDCONST	R7	K25
      0x5820001A,  //  002B  LDCONST	R8	K26
      0x7C100800,  //  002C  CALL	R4	4
      0x60140004,  //  002D  GETGBL	R5	G4
      0x5C180400,  //  002E  MOVE	R6	R2
      0x7C140200,  //  002F  CALL	R5	1
      0x2018091B,  //  0030  NE	R6	R4	K27
      0x781A0031,  //  0031  JMPF	R6	#0064
      0x1C18091A,  //  0032  EQ	R6	R4	K26
      0x781A000A,  //  0033  JMPF	R6	#003F
      0x1C180B1C,  //  0034  EQ	R6	R5	K28
      0x781A0008,  //  0035  JMPF	R6	#003F
      0xA41A3A00,  //  0036  IMPORT	R6	K29
      0x601C0009,  //  0037  GETGBL	R7	G9
      0x8C200D1E,  //  0038  GETMET	R8	R6	K30
      0x5C280400,  //  0039  MOVE	R10	R2
      0x7C200400,  //  003A  CALL	R8	2
      0x7C1C0200,  //  003B  CALL	R7	1
      0x5C080E00,  //  003C  MOVE	R2	R7
      0x5814001A,  //  003D  LDCONST	R5	K26
      0x70020024,  //  003E  JMP		#0064
      0x1C18091F,  //  003F  EQ	R6	R4	K31
      0x781A0018,  //  0040  JMPF	R6	#005A
      0x1C180B20,  //  0041  EQ	R6	R5	K32
      0x781A0006,  //  0042  JMPF	R6	#004A
      0x6018000F,  //  0043  GETGBL	R6	G15
      0x5C1C0400,  //  0044  MOVE	R7	R2
      0x60200015,  //  0045  GETGBL	R8	G21
      0x7C180400,  //  0046  CALL	R6	2
      0x781A0001,  //  0047  JMPF	R6	#004A
      0x5814001F,  //  0048  LDCONST	R5	K31
      0x7002000E,  //  0049  JMP		#0059
      0x20180B20,  //  004A  NE	R6	R5	K32
      0x741A0004,  //  004B  JMPT	R6	#0051
      0x6018000F,  //  004C  GETGBL	R6	G15
      0x5C1C0400,  //  004D  MOVE	R7	R2
      0x60200015,  //  004E  GETGBL	R8	G21
      0x7C180400,  //  004F  CALL	R6	2
      0x741A0007,  //  0050  JMPT	R6	#0059
      0x60180018,  //  0051  GETGBL	R6	G24
      0x581C0021,  //  0052  LDCONST	R7	K33
      0x5C200200,  //  0053  MOVE	R8	R1
      0x5C240800,  //  0054  MOVE	R9	R4
      0x5C280A00,  //  0055  MOVE	R10	R5
      0x5C2C0400,  //  0056  MOVE	R11	R2
      0x7C180A00,  //  0057  CALL	R6	5
      0xB0061606,  //  0058  RAISE	1	K11	R6
      0x70020009,  //  0059  JMP		#0064
      0x20180805,  //  005A  NE	R6	R4	R5
      0x781A0007,  //  005B  JMPF	R6	#0064
      0x60180018,  //  005C  GETGBL	R6	G24
      0x581C0021,  //  005D  LDCONST	R7	K33
      0x5C200200,  //  005E  MOVE	R8	R1
      0x5C240800,  //  005F  MOVE	R9	R4
      0x5C280A00,  //  0060  MOVE	R10	R5
      0x5C2C0400,  //  0061  MOVE	R11	R2
      0x7C180A00,  //  0062  CALL	R6	5
      0xB0061606,  //  0063  RAISE	1	K11	R6
      0x1C180B1A,  //  0064  EQ	R6	R5	K26
      0x781A0023,  //  0065  JMPF	R6	#008A
      0x8C18010E,  //  0066  GETMET	R6	R0	K14
      0x5C200600,  //  0067  MOVE	R8	R3
      0x58240022,  //  0068  LDCONST	R9	K34
      0x7C180600,  //  0069  CALL	R6	3
      0x781A000C,  //  006A  JMPF	R6	#0078
      0x8C180110,  //  006B  GETMET	R6	R0	K16
      0x5C200600,  //  006C  MOVE	R8	R3
      0x58240022,  //  006D  LDCONST	R9	K34
      0x7C180600,  //  006E  CALL	R6	3
      0x141C0406,  //  006F  LT	R7	R2	R6
      0x781E0006,  //  0070  JMPF	R7	#0078
      0x601C0018,  //  0071  GETGBL	R7	G24
      0x58200023,  //  0072  LDCONST	R8	K35
      0x5C240200,  //  0073  MOVE	R9	R1
      0x5C280400,  //  0074  MOVE	R10	R2
      0x5C2C0C00,  //  0075  MOVE	R11	R6
      0x7C1C0800,  //  0076  CALL	R7	4
      0xB0061607,  //  0077  RAISE	1	K11	R7
      0x8C18010E,  //  0078  GETMET	R6	R0	K14
      0x5C200600,  //  0079  MOVE	R8	R3
      0x58240024,  //  007A  LDCONST	R9	K36
      0x7C180600,  //  007B  CALL	R6	3
      0x781A000C,  //  007C  JMPF	R6	#008A
      0x8C180110,  //  007D  GETMET	R6	R0	K16
      0x5C200600,  //  007E  MOVE	R8	R3
      0x58240024,  //  007F  LDCONST	R9	K36
      0x7C180600,  //  0080  CALL	R6	3
      0x241C0406,  //  0081  GT	R7	R2	R6
      0x781E0006,  //  0082  JMPF	R7	#008A
      0x601C0018,  //  0083  GETGBL	R7	G24
      0x58200025,  //  0084  LDCONST	R8	K37
      0x5C240200,  //  0085  MOVE	R9	R1
      0x5C280400,  //  0086  MOVE	R10	R2
      0x5C2C0C00,  //  0087  MOVE	R11	R6
      0x7C1C0800,  //  0088  CALL	R7	4
      0xB0061607,  //  0089  RAISE	1	K11	R7
      0x8C18010E,  //  008A  GETMET	R6	R0	K14
      0x5C200600,  //  008B  MOVE	R8	R3
      0x58240026,  //  008C  LDCONST	R9	K38
      0x7C180600,  //  008D  CALL	R6	3
      0x781A001A,  //  008E  JMPF	R6	#00AA
      0x50180000,  //  008F  LDBOOL	R6	0	0
      0x8C1C0110,  //  0090  GETMET	R7	R0	K16
      0x5C240600,  //  0091  MOVE	R9	R3
      0x58280026,  //  0092  LDCONST	R10	K38
      0x7C1C0600,  //  0093  CALL	R7	3
      0x6020000C,  //  0094  GETGBL	R8	G12
      0x5C240E00,  //  0095  MOVE	R9	R7
      0x7C200200,  //  0096  CALL	R8	1
      0x58240027,  //  0097  LDCONST	R9	K39
      0x14281208,  //  0098  LT	R10	R9	R8
      0x782A0006,  //  0099  JMPF	R10	#00A1
      0x94280E09,  //  009A  GETIDX	R10	R7	R9
      0x1C2C040A,  //  009B  EQ	R11	R2	R10
      0x782E0001,  //  009C  JMPF	R11	#009F
      0x50180200,  //  009D  LDBOOL	R6	1	0
      0x70020001,  //  009E  JMP		#00A1
      0x00241328,  //  009F  ADD	R9	R9	K40
      0x7001FFF6,  //  00A0  JMP		#0098
      0x5C280C00,  //  00A1  MOVE	R10	R6
      0x742A0006,  //  00A2  JMPT	R10	#00AA
      0x60280018,  //  00A3  GETGBL	R10	G24
      0x582C0029,  //  00A4  LDCONST	R11	K41
      0x5C300200,  //  00A5  MOVE	R12	R1
      0x5C340400,  //  00A6  MOVE	R13	R2
      0x5C380E00,  //  00A7  MOVE	R14	R7
      0x7C280800,  //  00A8  CALL	R10	4
      0xB006160A,  //  00A9  RAISE	1	K11	R10
      0x80040400,  //  00AA  RET	1	R2
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
      0x8C0C0102,  //  0000  GETMET	R3	R0	K2
      0x5C140200,  //  0001  MOVE	R5	R1
      0x7C0C0400,  //  0002  CALL	R3	2
      0x780E0004,  //  0003  JMPF	R3	#0009
      0x8C0C010A,  //  0004  GETMET	R3	R0	K10
      0x5C140200,  //  0005  MOVE	R5	R1
      0x5C180400,  //  0006  MOVE	R6	R2
      0x7C0C0600,  //  0007  CALL	R3	3
      0x70020007,  //  0008  JMP		#0011
      0x600C0018,  //  0009  GETGBL	R3	G24
      0x58100006,  //  000A  LDCONST	R4	K6
      0x60140005,  //  000B  GETGBL	R5	G5
      0x5C180000,  //  000C  MOVE	R6	R0
      0x7C140200,  //  000D  CALL	R5	1
      0x5C180200,  //  000E  MOVE	R6	R1
      0x7C0C0600,  //  000F  CALL	R3	3
      0xB0060E03,  //  0010  RAISE	1	K7	R3
      0x80000000,  //  0011  RET	0
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
      0xB80E2600,  //  0000  GETNGBL	R3	K19
      0x8C0C0714,  //  0001  GETMET	R3	R3	K20
      0x5C140400,  //  0002  MOVE	R5	R2
      0x7C0C0400,  //  0003  CALL	R3	2
      0x740E0004,  //  0004  JMPT	R3	#000A
      0x8C0C012A,  //  0005  GETMET	R3	R0	K42
      0x5C140200,  //  0006  MOVE	R5	R1
      0x5C180400,  //  0007  MOVE	R6	R2
      0x7C0C0600,  //  0008  CALL	R3	3
      0x5C080600,  //  0009  MOVE	R2	R3
      0x880C0100,  //  000A  GETMBR	R3	R0	K0
      0x980C0202,  //  000B  SETIDX	R3	R1	R2
      0x8C0C012B,  //  000C  GETMET	R3	R0	K43
      0x5C140200,  //  000D  MOVE	R5	R1
      0x5C180400,  //  000E  MOVE	R6	R2
      0x7C0C0600,  //  000F  CALL	R3	3
      0x80000000,  //  0010  RET	0
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
    ( &(const binstruction[38]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x740E0011,  //  0004  JMPT	R3	#0017
      0x8C0C0116,  //  0005  GETMET	R3	R0	K22
      0x5C140200,  //  0006  MOVE	R5	R1
      0x7C0C0400,  //  0007  CALL	R3	2
      0x4C100000,  //  0008  LDNIL	R4
      0x20100604,  //  0009  NE	R4	R3	R4
      0x78120009,  //  000A  JMPF	R4	#0015
      0x8C10010E,  //  000B  GETMET	R4	R0	K14
      0x5C180600,  //  000C  MOVE	R6	R3
      0x581C000F,  //  000D  LDCONST	R7	K15
      0x7C100600,  //  000E  CALL	R4	3
      0x78120004,  //  000F  JMPF	R4	#0015
      0x8C100110,  //  0010  GETMET	R4	R0	K16
      0x5C180600,  //  0011  MOVE	R6	R3
      0x581C000F,  //  0012  LDCONST	R7	K15
      0x7C100600,  //  0013  CALL	R4	3
      0x80040800,  //  0014  RET	1	R4
      0x4C100000,  //  0015  LDNIL	R4
      0x80040800,  //  0016  RET	1	R4
      0x880C0100,  //  0017  GETMBR	R3	R0	K0
      0x940C0601,  //  0018  GETIDX	R3	R3	R1
      0xB8122600,  //  0019  GETNGBL	R4	K19
      0x8C100914,  //  001A  GETMET	R4	R4	K20
      0x5C180600,  //  001B  MOVE	R6	R3
      0x7C100400,  //  001C  CALL	R4	2
      0x78120005,  //  001D  JMPF	R4	#0024
      0x8C100715,  //  001E  GETMET	R4	R3	K21
      0x5C180200,  //  001F  MOVE	R6	R1
      0x5C1C0400,  //  0020  MOVE	R7	R2
      0x7C100600,  //  0021  CALL	R4	3
      0x80040800,  //  0022  RET	1	R4
      0x70020000,  //  0023  JMP		#0025
      0x80040600,  //  0024  RET	1	R3
      0x80000000,  //  0025  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: constraint_find
********************************************************************/
be_local_closure(class_ParameterizedObject_constraint_find,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    3,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        7,                          /* nstack */
        2,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 5]) {     /* constants */
        /* K0   */  be_const_int(0),
        /* K1   */  be_const_int(1),
        /* K2   */  be_const_int(2),
        /* K3   */  be_const_int(3),
        /* K4   */  be_nested_str_weak(get),
        }),
        be_str_weak(_skip_typed_value),
        &be_const_str_solidified,
        ( &(const binstruction[47]) {  /* code */
          0x6008000C,  //  0000  GETGBL	R2	G12
          0x5C0C0000,  //  0001  MOVE	R3	R0
          0x7C080200,  //  0002  CALL	R2	1
          0x28080202,  //  0003  GE	R2	R1	R2
          0x780A0000,  //  0004  JMPF	R2	#0006
          0x80060000,  //  0005  RET	1	K0
          0x94080001,  //  0006  GETIDX	R2	R0	R1
          0x540E0005,  //  0007  LDINT	R3	6
          0x1C0C0403,  //  0008  EQ	R3	R2	R3
          0x780E0001,  //  0009  JMPF	R3	#000C
          0x80060200,  //  000A  RET	1	K1
          0x70020021,  //  000B  JMP		#002E
          0x540E0004,  //  000C  LDINT	R3	5
          0x1C0C0403,  //  000D  EQ	R3	R2	R3
          0x780E0001,  //  000E  JMPF	R3	#0011
          0x80060400,  //  000F  RET	1	K2
          0x7002001C,  //  0010  JMP		#002E
          0x1C0C0500,  //  0011  EQ	R3	R2	K0
          0x780E0001,  //  0012  JMPF	R3	#0015
          0x80060400,  //  0013  RET	1	K2
          0x70020018,  //  0014  JMP		#002E
          0x1C0C0501,  //  0015  EQ	R3	R2	K1
          0x780E0001,  //  0016  JMPF	R3	#0019
          0x80060600,  //  0017  RET	1	K3
          0x70020014,  //  0018  JMP		#002E
          0x1C0C0502,  //  0019  EQ	R3	R2	K2
          0x780E0002,  //  001A  JMPF	R3	#001E
          0x540E0004,  //  001B  LDINT	R3	5
          0x80040600,  //  001C  RET	1	R3
          0x7002000F,  //  001D  JMP		#002E
          0x1C0C0503,  //  001E  EQ	R3	R2	K3
          0x780E0004,  //  001F  JMPF	R3	#0025
          0x000C0301,  //  0020  ADD	R3	R1	K1
          0x940C0003,  //  0021  GETIDX	R3	R0	R3
          0x000E0403,  //  0022  ADD	R3	K2	R3
          0x80040600,  //  0023  RET	1	R3
          0x70020008,  //  0024  JMP		#002E
          0x540E0003,  //  0025  LDINT	R3	4
          0x1C0C0403,  //  0026  EQ	R3	R2	R3
          0x780E0005,  //  0027  JMPF	R3	#002E
          0x8C0C0104,  //  0028  GETMET	R3	R0	K4
          0x00140301,  //  0029  ADD	R5	R1	K1
          0x58180002,  //  002A  LDCONST	R6	K2
          0x7C0C0600,  //  002B  CALL	R3	3
          0x000E0603,  //  002C  ADD	R3	K3	R3
          0x80040600,  //  002D  RET	1	R3
          0x80060000,  //  002E  RET	1	K0
        })
      ),
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
        /* K0   */  be_const_int(1),
        /* K1   */  be_const_int(0),
        /* K2   */  be_nested_str_weak(get),
        /* K3   */  be_const_int(2),
        /* K4   */  be_const_int(3),
        /* K5   */  be_nested_str_weak(asstring),
        }),
        be_str_weak(_read_typed_value),
        &be_const_str_solidified,
        ( &(const binstruction[83]) {  /* code */
          0x6008000C,  //  0000  GETGBL	R2	G12
          0x5C0C0000,  //  0001  MOVE	R3	R0
          0x7C080200,  //  0002  CALL	R2	1
          0x28080202,  //  0003  GE	R2	R1	R2
          0x780A0001,  //  0004  JMPF	R2	#0007
          0x4C080000,  //  0005  LDNIL	R2
          0x80040400,  //  0006  RET	1	R2
          0x94080001,  //  0007  GETIDX	R2	R0	R1
          0x00040300,  //  0008  ADD	R1	R1	K0
          0x540E0005,  //  0009  LDINT	R3	6
          0x1C0C0403,  //  000A  EQ	R3	R2	R3
          0x780E0002,  //  000B  JMPF	R3	#000F
          0x4C0C0000,  //  000C  LDNIL	R3
          0x80040600,  //  000D  RET	1	R3
          0x70020041,  //  000E  JMP		#0051
          0x540E0004,  //  000F  LDINT	R3	5
          0x1C0C0403,  //  0010  EQ	R3	R2	R3
          0x780E0003,  //  0011  JMPF	R3	#0016
          0x940C0001,  //  0012  GETIDX	R3	R0	R1
          0x200C0701,  //  0013  NE	R3	R3	K1
          0x80040600,  //  0014  RET	1	R3
          0x7002003A,  //  0015  JMP		#0051
          0x1C0C0501,  //  0016  EQ	R3	R2	K1
          0x780E0009,  //  0017  JMPF	R3	#0022
          0x940C0001,  //  0018  GETIDX	R3	R0	R1
          0x5412007E,  //  0019  LDINT	R4	127
          0x24100604,  //  001A  GT	R4	R3	R4
          0x78120002,  //  001B  JMPF	R4	#001F
          0x541200FF,  //  001C  LDINT	R4	256
          0x04100604,  //  001D  SUB	R4	R3	R4
          0x70020000,  //  001E  JMP		#0020
          0x5C100600,  //  001F  MOVE	R4	R3
          0x80040800,  //  0020  RET	1	R4
          0x7002002E,  //  0021  JMP		#0051
          0x1C0C0500,  //  0022  EQ	R3	R2	K0
          0x780E000C,  //  0023  JMPF	R3	#0031
          0x8C0C0102,  //  0024  GETMET	R3	R0	K2
          0x5C140200,  //  0025  MOVE	R5	R1
          0x58180003,  //  0026  LDCONST	R6	K3
          0x7C0C0600,  //  0027  CALL	R3	3
          0x54127FFE,  //  0028  LDINT	R4	32767
          0x24100604,  //  0029  GT	R4	R3	R4
          0x78120002,  //  002A  JMPF	R4	#002E
          0x5412FFFF,  //  002B  LDINT	R4	65536
          0x04100604,  //  002C  SUB	R4	R3	R4
          0x70020000,  //  002D  JMP		#002F
          0x5C100600,  //  002E  MOVE	R4	R3
          0x80040800,  //  002F  RET	1	R4
          0x7002001F,  //  0030  JMP		#0051
          0x1C0C0503,  //  0031  EQ	R3	R2	K3
          0x780E0005,  //  0032  JMPF	R3	#0039
          0x8C0C0102,  //  0033  GETMET	R3	R0	K2
          0x5C140200,  //  0034  MOVE	R5	R1
          0x541A0003,  //  0035  LDINT	R6	4
          0x7C0C0600,  //  0036  CALL	R3	3
          0x80040600,  //  0037  RET	1	R3
          0x70020017,  //  0038  JMP		#0051
          0x1C0C0504,  //  0039  EQ	R3	R2	K4
          0x780E0008,  //  003A  JMPF	R3	#0044
          0x940C0001,  //  003B  GETIDX	R3	R0	R1
          0x00100300,  //  003C  ADD	R4	R1	K0
          0x00140203,  //  003D  ADD	R5	R1	R3
          0x40100805,  //  003E  CONNECT	R4	R4	R5
          0x94100004,  //  003F  GETIDX	R4	R0	R4
          0x8C100905,  //  0040  GETMET	R4	R4	K5
          0x7C100200,  //  0041  CALL	R4	1
          0x80040800,  //  0042  RET	1	R4
          0x7002000C,  //  0043  JMP		#0051
          0x540E0003,  //  0044  LDINT	R3	4
          0x1C0C0403,  //  0045  EQ	R3	R2	R3
          0x780E0009,  //  0046  JMPF	R3	#0051
          0x8C0C0102,  //  0047  GETMET	R3	R0	K2
          0x5C140200,  //  0048  MOVE	R5	R1
          0x58180003,  //  0049  LDCONST	R6	K3
          0x7C0C0600,  //  004A  CALL	R3	3
          0x00100303,  //  004B  ADD	R4	R1	K3
          0x00140203,  //  004C  ADD	R5	R1	R3
          0x00140B00,  //  004D  ADD	R5	R5	K0
          0x40100805,  //  004E  CONNECT	R4	R4	R5
          0x94100004,  //  004F  GETIDX	R4	R0	R4
          0x80040800,  //  0050  RET	1	R4
          0x4C0C0000,  //  0051  LDNIL	R3
          0x80040600,  //  0052  RET	1	R3
        })
      ),
    }),
    1,                          /* has constants */
    &be_ktab_class_ParameterizedObject,     /* shared constants */
    be_str_weak(constraint_find),
    &be_const_str_solidified,
    ( &(const binstruction[112]) {  /* code */
      0x580C002C,  //  0000  LDCONST	R3	K44
      0x84100000,  //  0001  CLOSURE	R4	P0
      0x84140001,  //  0002  CLOSURE	R5	P1
      0x6018000C,  //  0003  GETGBL	R6	G12
      0x5C1C0000,  //  0004  MOVE	R7	R0
      0x7C180200,  //  0005  CALL	R6	1
      0x14180D28,  //  0006  LT	R6	R6	K40
      0x781A0000,  //  0007  JMPF	R6	#0009
      0x80040400,  //  0008  RET	1	R2
      0x94180127,  //  0009  GETIDX	R6	R0	K39
      0x581C0028,  //  000A  LDCONST	R7	K40
      0x8820072D,  //  000B  GETMBR	R8	R3	K45
      0x8C20112E,  //  000C  GETMET	R8	R8	K46
      0x5C280200,  //  000D  MOVE	R10	R1
      0x7C200400,  //  000E  CALL	R8	2
      0x4C240000,  //  000F  LDNIL	R9
      0x1C241009,  //  0010  EQ	R9	R8	R9
      0x78260000,  //  0011  JMPF	R9	#0013
      0x80040400,  //  0012  RET	1	R2
      0x38225008,  //  0013  SHL	R8	K40	R8
      0x2C240C08,  //  0014  AND	R9	R6	R8
      0x74260000,  //  0015  JMPT	R9	#0017
      0x80040400,  //  0016  RET	1	R2
      0x5426001F,  //  0017  LDINT	R9	32
      0x1C241009,  //  0018  EQ	R9	R8	R9
      0x78260001,  //  0019  JMPF	R9	#001C
      0x50240200,  //  001A  LDBOOL	R9	1	0
      0x80041200,  //  001B  RET	1	R9
      0x24241128,  //  001C  GT	R9	R8	K40
      0x78260006,  //  001D  JMPF	R9	#0025
      0x2C240D28,  //  001E  AND	R9	R6	K40
      0x78260004,  //  001F  JMPF	R9	#0025
      0x5C240800,  //  0020  MOVE	R9	R4
      0x5C280000,  //  0021  MOVE	R10	R0
      0x5C2C0E00,  //  0022  MOVE	R11	R7
      0x7C240400,  //  0023  CALL	R9	2
      0x001C0E09,  //  0024  ADD	R7	R7	R9
      0x2424112F,  //  0025  GT	R9	R8	K47
      0x78260006,  //  0026  JMPF	R9	#002E
      0x2C240D2F,  //  0027  AND	R9	R6	K47
      0x78260004,  //  0028  JMPF	R9	#002E
      0x5C240800,  //  0029  MOVE	R9	R4
      0x5C280000,  //  002A  MOVE	R10	R0
      0x5C2C0E00,  //  002B  MOVE	R11	R7
      0x7C240400,  //  002C  CALL	R9	2
      0x001C0E09,  //  002D  ADD	R7	R7	R9
      0x54260003,  //  002E  LDINT	R9	4
      0x24241009,  //  002F  GT	R9	R8	R9
      0x78260007,  //  0030  JMPF	R9	#0039
      0x54260003,  //  0031  LDINT	R9	4
      0x2C240C09,  //  0032  AND	R9	R6	R9
      0x78260004,  //  0033  JMPF	R9	#0039
      0x5C240800,  //  0034  MOVE	R9	R4
      0x5C280000,  //  0035  MOVE	R10	R0
      0x5C2C0E00,  //  0036  MOVE	R11	R7
      0x7C240400,  //  0037  CALL	R9	2
      0x001C0E09,  //  0038  ADD	R7	R7	R9
      0x54260007,  //  0039  LDINT	R9	8
      0x24241009,  //  003A  GT	R9	R8	R9
      0x78260003,  //  003B  JMPF	R9	#0040
      0x54260007,  //  003C  LDINT	R9	8
      0x2C240C09,  //  003D  AND	R9	R6	R9
      0x78260000,  //  003E  JMPF	R9	#0040
      0x001C0F28,  //  003F  ADD	R7	R7	K40
      0x6024000C,  //  0040  GETGBL	R9	G12
      0x5C280000,  //  0041  MOVE	R10	R0
      0x7C240200,  //  0042  CALL	R9	1
      0x28240E09,  //  0043  GE	R9	R7	R9
      0x78260000,  //  0044  JMPF	R9	#0046
      0x80040400,  //  0045  RET	1	R2
      0x54260007,  //  0046  LDINT	R9	8
      0x1C241009,  //  0047  EQ	R9	R8	R9
      0x78260009,  //  0048  JMPF	R9	#0053
      0x94240007,  //  0049  GETIDX	R9	R0	R7
      0x6028000C,  //  004A  GETGBL	R10	G12
      0x882C0730,  //  004B  GETMBR	R11	R3	K48
      0x7C280200,  //  004C  CALL	R10	1
      0x1428120A,  //  004D  LT	R10	R9	R10
      0x782A0002,  //  004E  JMPF	R10	#0052
      0x88280730,  //  004F  GETMBR	R10	R3	K48
      0x94281409,  //  0050  GETIDX	R10	R10	R9
      0x80041400,  //  0051  RET	1	R10
      0x80040400,  //  0052  RET	1	R2
      0x5426000F,  //  0053  LDINT	R9	16
      0x1C241009,  //  0054  EQ	R9	R8	R9
      0x78260014,  //  0055  JMPF	R9	#006B
      0x94240007,  //  0056  GETIDX	R9	R0	R7
      0x001C0F28,  //  0057  ADD	R7	R7	K40
      0x60280012,  //  0058  GETGBL	R10	G18
      0x7C280000,  //  0059  CALL	R10	0
      0x582C0027,  //  005A  LDCONST	R11	K39
      0x14301609,  //  005B  LT	R12	R11	R9
      0x7832000C,  //  005C  JMPF	R12	#006A
      0x8C301531,  //  005D  GETMET	R12	R10	K49
      0x5C380A00,  //  005E  MOVE	R14	R5
      0x5C3C0000,  //  005F  MOVE	R15	R0
      0x5C400E00,  //  0060  MOVE	R16	R7
      0x7C380400,  //  0061  CALL	R14	2
      0x7C300400,  //  0062  CALL	R12	2
      0x5C340800,  //  0063  MOVE	R13	R4
      0x5C380000,  //  0064  MOVE	R14	R0
      0x5C3C0E00,  //  0065  MOVE	R15	R7
      0x7C340400,  //  0066  CALL	R13	2
      0x001C0E0D,  //  0067  ADD	R7	R7	R13
      0x002C1728,  //  0068  ADD	R11	R11	K40
      0x7001FFF0,  //  0069  JMP		#005B
      0x80041400,  //  006A  RET	1	R10
      0x5C240A00,  //  006B  MOVE	R9	R5
      0x5C280000,  //  006C  MOVE	R10	R0
      0x5C2C0E00,  //  006D  MOVE	R11	R7
      0x7C240400,  //  006E  CALL	R9	2
      0x80041200,  //  006F  RET	1	R9
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: on_param_changed
********************************************************************/
be_local_closure(class_ParameterizedObject_on_param_changed,   /* name */
  be_nested_proto(
    6,                          /* nstack */
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
    ( &(const binstruction[13]) {  /* code */
      0x1C0C0332,  //  0000  EQ	R3	R1	K50
      0x780E0009,  //  0001  JMPF	R3	#000C
      0x500C0200,  //  0002  LDBOOL	R3	1	0
      0x1C0C0403,  //  0003  EQ	R3	R2	R3
      0x780E0003,  //  0004  JMPF	R3	#0009
      0x8C0C0133,  //  0005  GETMET	R3	R0	K51
      0x4C140000,  //  0006  LDNIL	R5
      0x7C0C0400,  //  0007  CALL	R3	2
      0x70020002,  //  0008  JMP		#000C
      0x500C0000,  //  0009  LDBOOL	R3	0	0
      0x1C0C0403,  //  000A  EQ	R3	R2	R3
      0x780DFFFF,  //  000B  JMPF	R3	#000C
      0x80000000,  //  000C  RET	0
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
      0x20080520,  //  0006  NE	R2	R2	K32
      0x780A0000,  //  0007  JMPF	R2	#0009
      0xB0061734,  //  0008  RAISE	1	K11	K52
      0x90020801,  //  0009  SETMBR	R0	K4	R1
      0x60080013,  //  000A  GETGBL	R2	G19
      0x7C080000,  //  000B  CALL	R2	0
      0x90020002,  //  000C  SETMBR	R0	K0	R2
      0x8C080135,  //  000D  GETMET	R2	R0	K53
      0x7C080200,  //  000E  CALL	R2	1
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(class_ParameterizedObject_start,   /* name */
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
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x88080104,  //  0003  GETMBR	R2	R0	K4
      0x88040505,  //  0004  GETMBR	R1	R2	K5
      0x88080112,  //  0005  GETMBR	R2	R0	K18
      0x4C0C0000,  //  0006  LDNIL	R3
      0x20080403,  //  0007  NE	R2	R2	R3
      0x780A0000,  //  0008  JMPF	R2	#000A
      0x90022401,  //  0009  SETMBR	R0	K18	R1
      0x88080100,  //  000A  GETMBR	R2	R0	K0
      0x500C0200,  //  000B  LDBOOL	R3	1	0
      0x980A6403,  //  000C  SETIDX	R2	K50	R3
      0x80040000,  //  000D  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_param
********************************************************************/
be_local_closure(class_ParameterizedObject_get_param,   /* name */
  be_nested_proto(
    9,                          /* nstack */
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
    ( &(const binstruction[26]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x780E0002,  //  0004  JMPF	R3	#0008
      0x880C0100,  //  0005  GETMBR	R3	R0	K0
      0x940C0601,  //  0006  GETIDX	R3	R3	R1
      0x80040600,  //  0007  RET	1	R3
      0x8C0C0116,  //  0008  GETMET	R3	R0	K22
      0x5C140200,  //  0009  MOVE	R5	R1
      0x7C0C0400,  //  000A  CALL	R3	2
      0x4C100000,  //  000B  LDNIL	R4
      0x20100604,  //  000C  NE	R4	R3	R4
      0x7812000A,  //  000D  JMPF	R4	#0019
      0x8C10010E,  //  000E  GETMET	R4	R0	K14
      0x5C180600,  //  000F  MOVE	R6	R3
      0x581C000F,  //  0010  LDCONST	R7	K15
      0x7C100600,  //  0011  CALL	R4	3
      0x78120005,  //  0012  JMPF	R4	#0019
      0x8C100110,  //  0013  GETMET	R4	R0	K16
      0x5C180600,  //  0014  MOVE	R6	R3
      0x581C000F,  //  0015  LDCONST	R7	K15
      0x5C200400,  //  0016  MOVE	R8	R2
      0x7C100800,  //  0017  CALL	R4	4
      0x80040800,  //  0018  RET	1	R4
      0x80040400,  //  0019  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: constraint_mask
********************************************************************/
be_local_closure(class_ParameterizedObject_constraint_mask,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ParameterizedObject,     /* shared constants */
    be_str_weak(constraint_mask),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x5808002C,  //  0000  LDCONST	R2	K44
      0x600C000C,  //  0001  GETGBL	R3	G12
      0x5C100000,  //  0002  MOVE	R4	R0
      0x7C0C0200,  //  0003  CALL	R3	1
      0x240C0727,  //  0004  GT	R3	R3	K39
      0x780E000A,  //  0005  JMPF	R3	#0011
      0x880C052D,  //  0006  GETMBR	R3	R2	K45
      0x8C0C072E,  //  0007  GETMET	R3	R3	K46
      0x5C140200,  //  0008  MOVE	R5	R1
      0x7C0C0400,  //  0009  CALL	R3	2
      0x4C100000,  //  000A  LDNIL	R4
      0x20100604,  //  000B  NE	R4	R3	R4
      0x78120003,  //  000C  JMPF	R4	#0011
      0x94100127,  //  000D  GETIDX	R4	R0	K39
      0x38165003,  //  000E  SHL	R5	K40	R3
      0x2C100805,  //  000F  AND	R4	R4	R5
      0x80040800,  //  0010  RET	1	R4
      0x80064E00,  //  0011  RET	1	K39
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: ParameterizedObject
********************************************************************/
be_local_class(ParameterizedObject,
    3,
    NULL,
    be_nested_map(26,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(get_param_value, -1), be_const_closure(class_ParameterizedObject_get_param_value_closure) },
        { be_const_key_weak(values, 2), be_const_var(0) },
        { be_const_key_weak(engine, -1), be_const_var(1) },
        { be_const_key_weak(set_param, -1), be_const_closure(class_ParameterizedObject_set_param_closure) },
        { be_const_key_weak(_get_param_def, -1), be_const_closure(class_ParameterizedObject__get_param_def_closure) },
        { be_const_key_weak(_TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(7,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(int),
        be_nested_str_weak(string),
        be_nested_str_weak(bytes),
        be_nested_str_weak(bool),
        be_nested_str_weak(any),
        be_nested_str_weak(instance),
        be_nested_str_weak(function),
    }))    ) } )) },
        { be_const_key_weak(_MASK, 17), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(6,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(min),
        be_nested_str_weak(max),
        be_nested_str_weak(default),
        be_nested_str_weak(type),
        be_nested_str_weak(enum),
        be_nested_str_weak(nillable),
    }))    ) } )) },
        { be_const_key_weak(_init_parameter_values, 1), be_const_closure(class_ParameterizedObject__init_parameter_values_closure) },
        { be_const_key_weak(get_param, 10), be_const_closure(class_ParameterizedObject_get_param_closure) },
        { be_const_key_weak(start, 24), be_const_closure(class_ParameterizedObject_start_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_ParameterizedObject_init_closure) },
        { be_const_key_weak(member, 22), be_const_closure(class_ParameterizedObject_member_closure) },
        { be_const_key_weak(_has_param, 20), be_const_closure(class_ParameterizedObject__has_param_closure) },
        { be_const_key_weak(resolve_value, -1), be_const_closure(class_ParameterizedObject_resolve_value_closure) },
        { be_const_key_weak(_validate_param, -1), be_const_closure(class_ParameterizedObject__validate_param_closure) },
        { be_const_key_weak(setmember, -1), be_const_closure(class_ParameterizedObject_setmember_closure) },
        { be_const_key_weak(_set_parameter_value, -1), be_const_closure(class_ParameterizedObject__set_parameter_value_closure) },
        { be_const_key_weak(_resolve_parameter_value, -1), be_const_closure(class_ParameterizedObject__resolve_parameter_value_closure) },
        { be_const_key_weak(constraint_find, -1), be_const_static_closure(class_ParameterizedObject_constraint_find_closure) },
        { be_const_key_weak(on_param_changed, 8), be_const_closure(class_ParameterizedObject_on_param_changed_closure) },
        { be_const_key_weak(_fix_time_ms, -1), be_const_closure(class_ParameterizedObject__fix_time_ms_closure) },
        { be_const_key_weak(_X3D_X3D, 9), be_const_closure(class_ParameterizedObject__X3D_X3D_closure) },
        { be_const_key_weak(PARAMS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(is_running, -1), be_const_bytes_instance(0C050003) },
    }))    ) } )) },
        { be_const_key_weak(start_time, -1), be_const_var(2) },
        { be_const_key_weak(_X21_X3D, 0), be_const_closure(class_ParameterizedObject__X21_X3D_closure) },
        { be_const_key_weak(constraint_mask, -1), be_const_static_closure(class_ParameterizedObject_constraint_mask_closure) },
    })),
    be_str_weak(ParameterizedObject)
);
// compact class 'ColorCycleColorProvider' ktab size: 28, total: 53 (saved 200 bytes)
static const bvalue be_ktab_class_ColorCycleColorProvider[28] = {
  /* K0   */  be_nested_str_weak(palette),
  /* K1   */  be_nested_str_weak(_get_param_def),
  /* K2   */  be_nested_str_weak(constraint_mask),
  /* K3   */  be_nested_str_weak(default),
  /* K4   */  be_nested_str_weak(constraint_find),
  /* K5   */  be_nested_str_weak(ColorCycleColorProvider_X28palette_size_X3D_X25s_X2C_X20cycle_period_X3D_X25s_X2C_X20mode_X3D_X25s_X2C_X20current_index_X3D_X25s_X29),
  /* K6   */  be_nested_str_weak(_get_palette_size),
  /* K7   */  be_nested_str_weak(cycle_period),
  /* K8   */  be_nested_str_weak(manual),
  /* K9   */  be_nested_str_weak(auto),
  /* K10  */  be_nested_str_weak(current_index),
  /* K11  */  be_nested_str_weak(_get_palette_bytes),
  /* K12  */  be_nested_str_weak(on_param_changed),
  /* K13  */  be_nested_str_weak(palette_size),
  /* K14  */  be_nested_str_weak(values),
  /* K15  */  be_nested_str_weak(value_error),
  /* K16  */  be_nested_str_weak(Parameter_X20_X27palette_size_X27_X20is_X20read_X2Donly),
  /* K17  */  be_const_int(0),
  /* K18  */  be_nested_str_weak(current_color),
  /* K19  */  be_nested_str_weak(_get_color_at_index),
  /* K20  */  be_nested_str_weak(next),
  /* K21  */  be_nested_str_weak(set_param),
  /* K22  */  be_const_int(1),
  /* K23  */  be_nested_str_weak(tasmota),
  /* K24  */  be_nested_str_weak(scale_uint),
  /* K25  */  be_nested_str_weak(init),
  /* K26  */  be_nested_str_weak(get),
  /* K27  */  be_const_int(-16777216),
};


extern const bclass be_class_ColorCycleColorProvider;

/********************************************************************
** Solidified function: _get_palette_bytes
********************************************************************/
be_local_closure(class_ColorCycleColorProvider__get_palette_bytes,   /* name */
  be_nested_proto(
    8,                          /* nstack */
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
    ( &(const binstruction[22]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x1C080202,  //  0002  EQ	R2	R1	R2
      0x780A0010,  //  0003  JMPF	R2	#0015
      0x8C080101,  //  0004  GETMET	R2	R0	K1
      0x58100000,  //  0005  LDCONST	R4	K0
      0x7C080400,  //  0006  CALL	R2	2
      0x4C0C0000,  //  0007  LDNIL	R3
      0x200C0403,  //  0008  NE	R3	R2	R3
      0x780E000A,  //  0009  JMPF	R3	#0015
      0x8C0C0102,  //  000A  GETMET	R3	R0	K2
      0x5C140400,  //  000B  MOVE	R5	R2
      0x58180003,  //  000C  LDCONST	R6	K3
      0x7C0C0600,  //  000D  CALL	R3	3
      0x780E0005,  //  000E  JMPF	R3	#0015
      0x8C0C0104,  //  000F  GETMET	R3	R0	K4
      0x5C140400,  //  0010  MOVE	R5	R2
      0x58180003,  //  0011  LDCONST	R6	K3
      0x4C1C0000,  //  0012  LDNIL	R7
      0x7C0C0800,  //  0013  CALL	R3	4
      0x5C040600,  //  0014  MOVE	R1	R3
      0x80040200,  //  0015  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_ColorCycleColorProvider_tostring,   /* name */
  be_nested_proto(
    7,                          /* nstack */
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
    ( &(const binstruction[13]) {  /* code */
      0x60040018,  //  0000  GETGBL	R1	G24
      0x58080005,  //  0001  LDCONST	R2	K5
      0x8C0C0106,  //  0002  GETMET	R3	R0	K6
      0x7C0C0200,  //  0003  CALL	R3	1
      0x88100107,  //  0004  GETMBR	R4	R0	K7
      0x88140107,  //  0005  GETMBR	R5	R0	K7
      0x78160001,  //  0006  JMPF	R5	#0009
      0x58140008,  //  0007  LDCONST	R5	K8
      0x70020000,  //  0008  JMP		#000A
      0x58140009,  //  0009  LDCONST	R5	K9
      0x8818010A,  //  000A  GETMBR	R6	R0	K10
      0x7C040A00,  //  000B  CALL	R1	5
      0x80040200,  //  000C  RET	1	R1
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
      0x8C04010B,  //  0000  GETMET	R1	R0	K11
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
    ( &(const binstruction[56]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C070C,  //  0003  GETMET	R3	R3	K12
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x1C0C030D,  //  0007  EQ	R3	R1	K13
      0x780E0005,  //  0008  JMPF	R3	#000F
      0x880C010E,  //  0009  GETMBR	R3	R0	K14
      0x8C100106,  //  000A  GETMET	R4	R0	K6
      0x7C100200,  //  000B  CALL	R4	1
      0x980E1A04,  //  000C  SETIDX	R3	K13	R4
      0xB0061F10,  //  000D  RAISE	1	K15	K16
      0x70020027,  //  000E  JMP		#0037
      0x1C0C0300,  //  000F  EQ	R3	R1	K0
      0x780E000E,  //  0010  JMPF	R3	#0020
      0x8C0C0106,  //  0011  GETMET	R3	R0	K6
      0x7C0C0200,  //  0012  CALL	R3	1
      0x24100711,  //  0013  GT	R4	R3	K17
      0x78120007,  //  0014  JMPF	R4	#001D
      0x8810010A,  //  0015  GETMBR	R4	R0	K10
      0x28100803,  //  0016  GE	R4	R4	R3
      0x78120000,  //  0017  JMPF	R4	#0019
      0x90021511,  //  0018  SETMBR	R0	K10	K17
      0x8C100113,  //  0019  GETMET	R4	R0	K19
      0x8818010A,  //  001A  GETMBR	R6	R0	K10
      0x7C100400,  //  001B  CALL	R4	2
      0x90022404,  //  001C  SETMBR	R0	K18	R4
      0x8810010E,  //  001D  GETMBR	R4	R0	K14
      0x98121A03,  //  001E  SETIDX	R4	K13	R3
      0x70020016,  //  001F  JMP		#0037
      0x1C0C0314,  //  0020  EQ	R3	R1	K20
      0x780E0014,  //  0021  JMPF	R3	#0037
      0x200C0511,  //  0022  NE	R3	R2	K17
      0x780E0012,  //  0023  JMPF	R3	#0037
      0x8C0C0106,  //  0024  GETMET	R3	R0	K6
      0x7C0C0200,  //  0025  CALL	R3	1
      0x24100711,  //  0026  GT	R4	R3	K17
      0x7812000A,  //  0027  JMPF	R4	#0033
      0x8810010A,  //  0028  GETMBR	R4	R0	K10
      0x00100802,  //  0029  ADD	R4	R4	R2
      0x10100803,  //  002A  MOD	R4	R4	R3
      0x14140911,  //  002B  LT	R5	R4	K17
      0x78160000,  //  002C  JMPF	R5	#002E
      0x00100803,  //  002D  ADD	R4	R4	R3
      0x90021404,  //  002E  SETMBR	R0	K10	R4
      0x8C140113,  //  002F  GETMET	R5	R0	K19
      0x881C010A,  //  0030  GETMBR	R7	R0	K10
      0x7C140400,  //  0031  CALL	R5	2
      0x90022405,  //  0032  SETMBR	R0	K18	R5
      0x8C100115,  //  0033  GETMET	R4	R0	K21
      0x58180014,  //  0034  LDCONST	R6	K20
      0x581C0011,  //  0035  LDCONST	R7	K17
      0x7C100600,  //  0036  CALL	R4	3
      0x80000000,  //  0037  RET	0
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
      0x880C0107,  //  0000  GETMBR	R3	R0	K7
      0x8C100106,  //  0001  GETMET	R4	R0	K6
      0x7C100200,  //  0002  CALL	R4	1
      0x1C140911,  //  0003  EQ	R5	R4	K17
      0x78160001,  //  0004  JMPF	R5	#0007
      0x5415FFFE,  //  0005  LDINT	R5	-1
      0x80040A00,  //  0006  RET	1	R5
      0x1C140916,  //  0007  EQ	R5	R4	K22
      0x78160005,  //  0008  JMPF	R5	#000F
      0x8C140113,  //  0009  GETMET	R5	R0	K19
      0x581C0011,  //  000A  LDCONST	R7	K17
      0x7C140400,  //  000B  CALL	R5	2
      0x90022405,  //  000C  SETMBR	R0	K18	R5
      0x88140112,  //  000D  GETMBR	R5	R0	K18
      0x80040A00,  //  000E  RET	1	R5
      0x1C140711,  //  000F  EQ	R5	R3	K17
      0x78160001,  //  0010  JMPF	R5	#0013
      0x88140112,  //  0011  GETMBR	R5	R0	K18
      0x80040A00,  //  0012  RET	1	R5
      0x10140403,  //  0013  MOD	R5	R2	R3
      0xB81A2E00,  //  0014  GETNGBL	R6	K23
      0x8C180D18,  //  0015  GETMET	R6	R6	K24
      0x5C200A00,  //  0016  MOVE	R8	R5
      0x58240011,  //  0017  LDCONST	R9	K17
      0x04280716,  //  0018  SUB	R10	R3	K22
      0x582C0011,  //  0019  LDCONST	R11	K17
      0x04300916,  //  001A  SUB	R12	R4	K22
      0x7C180C00,  //  001B  CALL	R6	6
      0x281C0C04,  //  001C  GE	R7	R6	R4
      0x781E0001,  //  001D  JMPF	R7	#0020
      0x041C0916,  //  001E  SUB	R7	R4	K22
      0x5C180E00,  //  001F  MOVE	R6	R7
      0x90021406,  //  0020  SETMBR	R0	K10	R6
      0x8C1C0113,  //  0021  GETMET	R7	R0	K19
      0x5C240C00,  //  0022  MOVE	R9	R6
      0x7C1C0400,  //  0023  CALL	R7	2
      0x90022407,  //  0024  SETMBR	R0	K18	R7
      0x881C0112,  //  0025  GETMBR	R7	R0	K18
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
      0x8C0C0106,  //  0000  GETMET	R3	R0	K6
      0x7C0C0200,  //  0001  CALL	R3	1
      0x1C100711,  //  0002  EQ	R4	R3	K17
      0x78120001,  //  0003  JMPF	R4	#0006
      0x5411FFFE,  //  0004  LDINT	R4	-1
      0x80040800,  //  0005  RET	1	R4
      0x1C100716,  //  0006  EQ	R4	R3	K22
      0x78120003,  //  0007  JMPF	R4	#000C
      0x8C100113,  //  0008  GETMET	R4	R0	K19
      0x58180011,  //  0009  LDCONST	R6	K17
      0x7C100400,  //  000A  CALL	R4	2
      0x80040800,  //  000B  RET	1	R4
      0x14100311,  //  000C  LT	R4	R1	K17
      0x78120001,  //  000D  JMPF	R4	#0010
      0x58040011,  //  000E  LDCONST	R1	K17
      0x70020003,  //  000F  JMP		#0014
      0x54120063,  //  0010  LDINT	R4	100
      0x24100204,  //  0011  GT	R4	R1	R4
      0x78120000,  //  0012  JMPF	R4	#0014
      0x54060063,  //  0013  LDINT	R1	100
      0xB8122E00,  //  0014  GETNGBL	R4	K23
      0x8C100918,  //  0015  GETMET	R4	R4	K24
      0x5C180200,  //  0016  MOVE	R6	R1
      0x581C0011,  //  0017  LDCONST	R7	K17
      0x54220063,  //  0018  LDINT	R8	100
      0x58240011,  //  0019  LDCONST	R9	K17
      0x04280716,  //  001A  SUB	R10	R3	K22
      0x7C100C00,  //  001B  CALL	R4	6
      0x28140803,  //  001C  GE	R5	R4	R3
      0x78160001,  //  001D  JMPF	R5	#0020
      0x04140716,  //  001E  SUB	R5	R3	K22
      0x5C100A00,  //  001F  MOVE	R4	R5
      0x8C140113,  //  0020  GETMET	R5	R0	K19
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
    ( &(const binstruction[18]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080519,  //  0003  GETMET	R2	R2	K25
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x8C08010B,  //  0006  GETMET	R2	R0	K11
      0x7C080200,  //  0007  CALL	R2	1
      0x8C0C0113,  //  0008  GETMET	R3	R0	K19
      0x58140011,  //  0009  LDCONST	R5	K17
      0x7C0C0400,  //  000A  CALL	R3	2
      0x90022403,  //  000B  SETMBR	R0	K18	R3
      0x90021511,  //  000C  SETMBR	R0	K10	K17
      0x880C010E,  //  000D  GETMBR	R3	R0	K14
      0x8C100106,  //  000E  GETMET	R4	R0	K6
      0x7C100200,  //  000F  CALL	R4	1
      0x980E1A04,  //  0010  SETIDX	R3	K13	R4
      0x80000000,  //  0011  RET	0
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
      0x8C08010B,  //  0000  GETMET	R2	R0	K11
      0x7C080200,  //  0001  CALL	R2	1
      0x600C000C,  //  0002  GETGBL	R3	G12
      0x5C100400,  //  0003  MOVE	R4	R2
      0x7C0C0200,  //  0004  CALL	R3	1
      0x54120003,  //  0005  LDINT	R4	4
      0x0C0C0604,  //  0006  DIV	R3	R3	R4
      0x1C100711,  //  0007  EQ	R4	R3	K17
      0x74120003,  //  0008  JMPT	R4	#000D
      0x14100311,  //  0009  LT	R4	R1	K17
      0x74120001,  //  000A  JMPT	R4	#000D
      0x28100203,  //  000B  GE	R4	R1	R3
      0x78120001,  //  000C  JMPF	R4	#000F
      0x5411FFFE,  //  000D  LDINT	R4	-1
      0x80040800,  //  000E  RET	1	R4
      0x8C10051A,  //  000F  GETMET	R4	R2	K26
      0x541A0003,  //  0010  LDINT	R6	4
      0x08180206,  //  0011  MUL	R6	R1	R6
      0x541DFFFB,  //  0012  LDINT	R7	-4
      0x7C100600,  //  0013  CALL	R4	3
      0x3010091B,  //  0014  OR	R4	R4	K27
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
        be_const_map( *     be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(palette, 2), be_const_bytes_instance(0C040C00FF0000FFFF00FF00FFFF000002) },
        { be_const_key_weak(palette_size, -1), be_const_bytes_instance(0C000300) },
        { be_const_key_weak(next, 1), be_const_bytes_instance(040000) },
        { be_const_key_weak(cycle_period, -1), be_const_bytes_instance(050000018813) },
    }))    ) } )) },
        { be_const_key_weak(init, -1), be_const_closure(class_ColorCycleColorProvider_init_closure) },
        { be_const_key_weak(produce_value, 0), be_const_closure(class_ColorCycleColorProvider_produce_value_closure) },
    })),
    be_str_weak(ColorCycleColorProvider)
);
// compact class 'GradientAnimation' ktab size: 45, total: 84 (saved 312 bytes)
static const bvalue be_ktab_class_GradientAnimation[45] = {
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
  /* K22  */  be_nested_str_weak(_fix_time_ms),
  /* K23  */  be_nested_str_weak(engine),
  /* K24  */  be_nested_str_weak(get_strip_length),
  /* K25  */  be_nested_str_weak(width),
  /* K26  */  be_nested_str_weak(current_colors),
  /* K27  */  be_nested_str_weak(set_pixel_color),
  /* K28  */  be_nested_str_weak(on_param_changed),
  /* K29  */  be_nested_str_weak(resize),
  /* K30  */  be_const_int(-16777216),
  /* K31  */  be_nested_str_weak(_calculate_linear_position),
  /* K32  */  be_nested_str_weak(_calculate_radial_position),
  /* K33  */  be_nested_str_weak(light_state),
  /* K34  */  be_const_int(3),
  /* K35  */  be_nested_str_weak(HsToRgb),
  /* K36  */  be_nested_str_weak(r),
  /* K37  */  be_nested_str_weak(g),
  /* K38  */  be_nested_str_weak(b),
  /* K39  */  be_nested_str_weak(is_color_provider),
  /* K40  */  be_nested_str_weak(get_color_for_value),
  /* K41  */  be_nested_str_weak(resolve_value),
  /* K42  */  be_nested_str_weak(int),
  /* K43  */  be_nested_str_weak(init),
  /* K44  */  be_nested_str_weak(center_pos),
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
    ( &(const binstruction[34]) {  /* code */
      0x880C0115,  //  0000  GETMBR	R3	R0	K21
      0x780E0002,  //  0001  JMPF	R3	#0005
      0x4C0C0000,  //  0002  LDNIL	R3
      0x1C0C0203,  //  0003  EQ	R3	R1	R3
      0x780E0001,  //  0004  JMPF	R3	#0007
      0x500C0000,  //  0005  LDBOOL	R3	0	0
      0x80040600,  //  0006  RET	1	R3
      0x8C0C0116,  //  0007  GETMET	R3	R0	K22
      0x5C140400,  //  0008  MOVE	R5	R2
      0x7C0C0400,  //  0009  CALL	R3	2
      0x5C080600,  //  000A  MOVE	R2	R3
      0x880C0117,  //  000B  GETMBR	R3	R0	K23
      0x8C0C0718,  //  000C  GETMET	R3	R3	K24
      0x7C0C0200,  //  000D  CALL	R3	1
      0x58100002,  //  000E  LDCONST	R4	K2
      0x14140803,  //  000F  LT	R5	R4	R3
      0x7816000E,  //  0010  JMPF	R5	#0020
      0x88140319,  //  0011  GETMBR	R5	R1	K25
      0x14140805,  //  0012  LT	R5	R4	R5
      0x7816000B,  //  0013  JMPF	R5	#0020
      0x6014000C,  //  0014  GETGBL	R5	G12
      0x8818011A,  //  0015  GETMBR	R6	R0	K26
      0x7C140200,  //  0016  CALL	R5	1
      0x14140805,  //  0017  LT	R5	R4	R5
      0x78160004,  //  0018  JMPF	R5	#001E
      0x8C14031B,  //  0019  GETMET	R5	R1	K27
      0x5C1C0800,  //  001A  MOVE	R7	R4
      0x8820011A,  //  001B  GETMBR	R8	R0	K26
      0x94201004,  //  001C  GETIDX	R8	R8	R4
      0x7C140600,  //  001D  CALL	R5	3
      0x00100908,  //  001E  ADD	R4	R4	K8
      0x7001FFEE,  //  001F  JMP		#000F
      0x50140200,  //  0020  LDBOOL	R5	1	0
      0x80040A00,  //  0021  RET	1	R5
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
    ( &(const binstruction[44]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C071C,  //  0003  GETMET	R3	R3	K28
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x880C0117,  //  0007  GETMBR	R3	R0	K23
      0x8C0C0718,  //  0008  GETMET	R3	R3	K24
      0x7C0C0200,  //  0009  CALL	R3	1
      0x6010000C,  //  000A  GETGBL	R4	G12
      0x8814011A,  //  000B  GETMBR	R5	R0	K26
      0x7C100200,  //  000C  CALL	R4	1
      0x20100803,  //  000D  NE	R4	R4	R3
      0x7812001B,  //  000E  JMPF	R4	#002B
      0x8810011A,  //  000F  GETMBR	R4	R0	K26
      0x8C10091D,  //  0010  GETMET	R4	R4	K29
      0x5C180600,  //  0011  MOVE	R6	R3
      0x7C100400,  //  0012  CALL	R4	2
      0x6010000C,  //  0013  GETGBL	R4	G12
      0x8814011A,  //  0014  GETMBR	R5	R0	K26
      0x7C100200,  //  0015  CALL	R4	1
      0x14140803,  //  0016  LT	R5	R4	R3
      0x78160012,  //  0017  JMPF	R5	#002B
      0x6014000C,  //  0018  GETGBL	R5	G12
      0x8818011A,  //  0019  GETMBR	R6	R0	K26
      0x7C140200,  //  001A  CALL	R5	1
      0x28140805,  //  001B  GE	R5	R4	R5
      0x74160004,  //  001C  JMPT	R5	#0022
      0x8814011A,  //  001D  GETMBR	R5	R0	K26
      0x94140A04,  //  001E  GETIDX	R5	R5	R4
      0x4C180000,  //  001F  LDNIL	R6
      0x1C140A06,  //  0020  EQ	R5	R5	R6
      0x78160006,  //  0021  JMPF	R5	#0029
      0x6014000C,  //  0022  GETGBL	R5	G12
      0x8818011A,  //  0023  GETMBR	R6	R0	K26
      0x7C140200,  //  0024  CALL	R5	1
      0x14140805,  //  0025  LT	R5	R4	R5
      0x78160001,  //  0026  JMPF	R5	#0029
      0x8814011A,  //  0027  GETMBR	R5	R0	K26
      0x9814091E,  //  0028  SETIDX	R5	R4	K30
      0x00100908,  //  0029  ADD	R4	R4	K8
      0x7001FFEA,  //  002A  JMP		#0016
      0x80000000,  //  002B  RET	0
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
      0x88100117,  //  0002  GETMBR	R4	R0	K23
      0x8C100918,  //  0003  GETMET	R4	R4	K24
      0x7C100200,  //  0004  CALL	R4	1
      0x6014000C,  //  0005  GETGBL	R5	G12
      0x8818011A,  //  0006  GETMBR	R6	R0	K26
      0x7C140200,  //  0007  CALL	R5	1
      0x20140A04,  //  0008  NE	R5	R5	R4
      0x78160003,  //  0009  JMPF	R5	#000E
      0x8814011A,  //  000A  GETMBR	R5	R0	K26
      0x8C140B1D,  //  000B  GETMET	R5	R5	K29
      0x5C1C0800,  //  000C  MOVE	R7	R4
      0x7C140400,  //  000D  CALL	R5	2
      0x58140002,  //  000E  LDCONST	R5	K2
      0x14180A04,  //  000F  LT	R6	R5	R4
      0x781A0082,  //  0010  JMPF	R6	#0094
      0x58180002,  //  0011  LDCONST	R6	K2
      0x1C1C0502,  //  0012  EQ	R7	R2	K2
      0x781E0005,  //  0013  JMPF	R7	#001A
      0x8C1C011F,  //  0014  GETMET	R7	R0	K31
      0x5C240A00,  //  0015  MOVE	R9	R5
      0x5C280800,  //  0016  MOVE	R10	R4
      0x7C1C0600,  //  0017  CALL	R7	3
      0x5C180E00,  //  0018  MOVE	R6	R7
      0x70020004,  //  0019  JMP		#001F
      0x8C1C0120,  //  001A  GETMET	R7	R0	K32
      0x5C240A00,  //  001B  MOVE	R9	R5
      0x5C280800,  //  001C  MOVE	R10	R4
      0x7C1C0600,  //  001D  CALL	R7	3
      0x5C180E00,  //  001E  MOVE	R6	R7
      0x881C0106,  //  001F  GETMBR	R7	R0	K6
      0x001C0C07,  //  0020  ADD	R7	R6	R7
      0x542200FF,  //  0021  LDINT	R8	256
      0x101C0E08,  //  0022  MOD	R7	R7	R8
      0x5C180E00,  //  0023  MOVE	R6	R7
      0x581C001E,  //  0024  LDCONST	R7	K30
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
      0xA4264200,  //  0030  IMPORT	R9	K33
      0x5C281200,  //  0031  MOVE	R10	R9
      0x582C0022,  //  0032  LDCONST	R11	K34
      0x7C280200,  //  0033  CALL	R10	1
      0x8C2C1523,  //  0034  GETMET	R11	R10	K35
      0x5C341000,  //  0035  MOVE	R13	R8
      0x543A00FE,  //  0036  LDINT	R14	255
      0x7C2C0600,  //  0037  CALL	R11	3
      0x882C1524,  //  0038  GETMBR	R11	R10	K36
      0x5432000F,  //  0039  LDINT	R12	16
      0x382C160C,  //  003A  SHL	R11	R11	R12
      0x302E3C0B,  //  003B  OR	R11	K30	R11
      0x88301525,  //  003C  GETMBR	R12	R10	K37
      0x54360007,  //  003D  LDINT	R13	8
      0x3830180D,  //  003E  SHL	R12	R12	R13
      0x302C160C,  //  003F  OR	R11	R11	R12
      0x88301526,  //  0040  GETMBR	R12	R10	K38
      0x302C160C,  //  0041  OR	R11	R11	R12
      0x5C1C1600,  //  0042  MOVE	R7	R11
      0x7002004B,  //  0043  JMP		#0090
      0xB8222000,  //  0044  GETNGBL	R8	K16
      0x8C201127,  //  0045  GETMET	R8	R8	K39
      0x5C280600,  //  0046  MOVE	R10	R3
      0x7C200400,  //  0047  CALL	R8	2
      0x78220009,  //  0048  JMPF	R8	#0053
      0x88200728,  //  0049  GETMBR	R8	R3	K40
      0x4C240000,  //  004A  LDNIL	R9
      0x20201009,  //  004B  NE	R8	R8	R9
      0x78220005,  //  004C  JMPF	R8	#0053
      0x8C200728,  //  004D  GETMET	R8	R3	K40
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
      0x8C200129,  //  0058  GETMET	R8	R0	K41
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
      0x1C20112A,  //  0064  EQ	R8	R8	K42
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
      0x30323C0C,  //  0088  OR	R12	K30	R12
      0x54360007,  //  0089  LDINT	R13	8
      0x3834140D,  //  008A  SHL	R13	R10	R13
      0x3030180D,  //  008B  OR	R12	R12	R13
      0x3030180B,  //  008C  OR	R12	R12	R11
      0x5C1C1800,  //  008D  MOVE	R7	R12
      0x70020000,  //  008E  JMP		#0090
      0x5C1C0600,  //  008F  MOVE	R7	R3
      0x8820011A,  //  0090  GETMBR	R8	R0	K26
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
      0x8C08052B,  //  0003  GETMET	R2	R2	K43
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x60080012,  //  0006  GETGBL	R2	G18
      0x7C080000,  //  0007  CALL	R2	0
      0x90023402,  //  0008  SETMBR	R0	K26	R2
      0x90020D02,  //  0009  SETMBR	R0	K6	K2
      0x88080117,  //  000A  GETMBR	R2	R0	K23
      0x8C080518,  //  000B  GETMET	R2	R2	K24
      0x7C080200,  //  000C  CALL	R2	1
      0x880C011A,  //  000D  GETMBR	R3	R0	K26
      0x8C0C071D,  //  000E  GETMET	R3	R3	K29
      0x5C140400,  //  000F  MOVE	R5	R2
      0x7C0C0400,  //  0010  CALL	R3	2
      0x580C0002,  //  0011  LDCONST	R3	K2
      0x14100602,  //  0012  LT	R4	R3	R2
      0x78120003,  //  0013  JMPF	R4	#0018
      0x8810011A,  //  0014  GETMBR	R4	R0	K26
      0x9810071E,  //  0015  SETIDX	R4	R3	K30
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
      0x8810012C,  //  0008  GETMBR	R4	R0	K44
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
        { be_const_key_weak(spread, -1), be_const_bytes_instance(07000101FF0001FF00) },
        { be_const_key_weak(center_pos, -1), be_const_bytes_instance(07000001FF00018000) },
        { be_const_key_weak(gradient_type, 0), be_const_bytes_instance(07000000010000) },
        { be_const_key_weak(movement_speed, 5), be_const_bytes_instance(07000001FF000000) },
        { be_const_key_weak(direction, 3), be_const_bytes_instance(07000001FF000000) },
        { be_const_key_weak(color, -1), be_const_bytes_instance(2406) },
    }))    ) } )) },
        { be_const_key_weak(init, -1), be_const_closure(class_GradientAnimation_init_closure) },
        { be_const_key_weak(_calculate_radial_position, -1), be_const_closure(class_GradientAnimation__calculate_radial_position_closure) },
    })),
    be_str_weak(GradientAnimation)
);

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
    19,                          /* nstack */
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
    ( &(const binstruction[97]) {  /* code */
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
      0xB8321800,  //  003C  GETNGBL	R12	K12
      0x8C30190D,  //  003D  GETMET	R12	R12	K13
      0x5C381200,  //  003E  MOVE	R14	R9
      0x583C000E,  //  003F  LDCONST	R15	K14
      0x544200FE,  //  0040  LDINT	R16	255
      0x5844000E,  //  0041  LDCONST	R17	K14
      0x5C480C00,  //  0042  MOVE	R18	R6
      0x7C300C00,  //  0043  CALL	R12	6
      0x5C241800,  //  0044  MOVE	R9	R12
      0xB8321800,  //  0045  GETNGBL	R12	K12
      0x8C30190D,  //  0046  GETMET	R12	R12	K13
      0x5C381400,  //  0047  MOVE	R14	R10
      0x583C000E,  //  0048  LDCONST	R15	K14
      0x544200FE,  //  0049  LDINT	R16	255
      0x5844000E,  //  004A  LDCONST	R17	K14
      0x5C480C00,  //  004B  MOVE	R18	R6
      0x7C300C00,  //  004C  CALL	R12	6
      0x5C281800,  //  004D  MOVE	R10	R12
      0xB8321800,  //  004E  GETNGBL	R12	K12
      0x8C30190D,  //  004F  GETMET	R12	R12	K13
      0x5C381600,  //  0050  MOVE	R14	R11
      0x583C000E,  //  0051  LDCONST	R15	K14
      0x544200FE,  //  0052  LDINT	R16	255
      0x5844000E,  //  0053  LDCONST	R17	K14
      0x5C480C00,  //  0054  MOVE	R18	R6
      0x7C300C00,  //  0055  CALL	R12	6
      0x5C2C1800,  //  0056  MOVE	R11	R12
      0x54320017,  //  0057  LDINT	R12	24
      0x3830100C,  //  0058  SHL	R12	R8	R12
      0x5436000F,  //  0059  LDINT	R13	16
      0x3834120D,  //  005A  SHL	R13	R9	R13
      0x3030180D,  //  005B  OR	R12	R12	R13
      0x54360007,  //  005C  LDINT	R13	8
      0x3834140D,  //  005D  SHL	R13	R10	R13
      0x3030180D,  //  005E  OR	R12	R12	R13
      0x3030180B,  //  005F  OR	R12	R12	R11
      0x80041800,  //  0060  RET	1	R12
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
        { be_const_key_weak(base_color, -1), be_const_bytes_instance(0400FF) },
        { be_const_key_weak(max_brightness, -1), be_const_bytes_instance(07000001FF0001FF00) },
        { be_const_key_weak(curve_factor, -1), be_const_bytes_instance(07000100050002) },
        { be_const_key_weak(min_brightness, -1), be_const_bytes_instance(07000001FF000000) },
    }))    ) } )) },
        { be_const_key_weak(produce_value, -1), be_const_closure(class_BreatheColorProvider_produce_value_closure) },
    })),
    be_str_weak(BreatheColorProvider)
);
// compact class 'IterationNumberProvider' ktab size: 4, total: 6 (saved 16 bytes)
static const bvalue be_ktab_class_IterationNumberProvider[4] = {
  /* K0   */  be_nested_str_weak(engine),
  /* K1   */  be_nested_str_weak(get_current_iteration_number),
  /* K2   */  be_nested_str_weak(IterationNumberProvider_X28current_X3A_X20_X25s_X29),
  /* K3   */  be_nested_str_weak(IterationNumberProvider_X28not_in_sequence_X29),
};


extern const bclass be_class_IterationNumberProvider;

/********************************************************************
** Solidified function: produce_value
********************************************************************/
be_local_closure(class_IterationNumberProvider_produce_value,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_IterationNumberProvider,     /* shared constants */
    be_str_weak(produce_value),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x7C0C0200,  //  0002  CALL	R3	1
      0x80040600,  //  0003  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_IterationNumberProvider_tostring,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_IterationNumberProvider,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x4C080000,  //  0003  LDNIL	R2
      0x20080202,  //  0004  NE	R2	R1	R2
      0x780A0005,  //  0005  JMPF	R2	#000C
      0x60080018,  //  0006  GETGBL	R2	G24
      0x580C0002,  //  0007  LDCONST	R3	K2
      0x5C100200,  //  0008  MOVE	R4	R1
      0x7C080400,  //  0009  CALL	R2	2
      0x80040400,  //  000A  RET	1	R2
      0x70020000,  //  000B  JMP		#000D
      0x80060600,  //  000C  RET	1	K3
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: IterationNumberProvider
********************************************************************/
extern const bclass be_class_ValueProvider;
be_local_class(IterationNumberProvider,
    0,
    &be_class_ValueProvider,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(produce_value, 2), be_const_closure(class_IterationNumberProvider_produce_value_closure) },
        { be_const_key_weak(tostring, -1), be_const_closure(class_IterationNumberProvider_tostring_closure) },
        { be_const_key_weak(PARAMS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(0,
    ( (struct bmapnode*) &(const bmapnode[]) {
    }))    ) } )) },
    })),
    be_str_weak(IterationNumberProvider)
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
    be_nested_map(99,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(rich_palette, 89), be_const_class(be_class_RichPaletteColorProvider) },
        { be_const_key_weak(pulsating_animation, -1), be_const_closure(pulsating_animation_closure) },
        { be_const_key_weak(register_user_function, -1), be_const_closure(register_user_function_closure) },
        { be_const_key_weak(SequenceManager, -1), be_const_class(be_class_SequenceManager) },
        { be_const_key_weak(noise_rainbow, -1), be_const_closure(noise_rainbow_closure) },
        { be_const_key_weak(is_user_function, -1), be_const_closure(is_user_function_closure) },
        { be_const_key_weak(palette_gradient_animation, 26), be_const_class(be_class_PaletteGradientAnimation) },
        { be_const_key_weak(PALETTE_RGB, 83), be_const_bytes_instance(00FF00008000FF00FF0000FF) },
        { be_const_key_weak(EASE_IN, 81), be_const_int(6) },
        { be_const_key_weak(elastic, 27), be_const_closure(elastic_closure) },
        { be_const_key_weak(ELASTIC, 51), be_const_int(8) },
        { be_const_key_weak(EventManager, 13), be_const_class(be_class_EventManager) },
        { be_const_key_weak(get_event_handlers, -1), be_const_closure(get_event_handlers_closure) },
        { be_const_key_weak(strip_length, 98), be_const_class(be_class_StripLengthProvider) },
        { be_const_key_weak(color_provider, -1), be_const_class(be_class_ColorProvider) },
        { be_const_key_weak(iteration_number, 76), be_const_class(be_class_IterationNumberProvider) },
        { be_const_key_weak(breathe_color, -1), be_const_class(be_class_BreatheColorProvider) },
        { be_const_key_weak(PALETTE_SUNSET_TICKS, -1), be_const_bytes_instance(28FF450028FF8C0028FFD70028FF69B4288000802819197000000080) },
        { be_const_key_weak(ease_in, -1), be_const_closure(ease_in_closure) },
        { be_const_key_weak(twinkle_classic, -1), be_const_closure(twinkle_classic_closure) },
        { be_const_key_weak(_math, -1), be_const_class(be_class_AnimationMath) },
        { be_const_key_weak(static_value, -1), be_const_class(be_class_StaticValueProvider) },
        { be_const_key_weak(VERSION, -1), be_const_int(65536) },
        { be_const_key_weak(is_color_provider, 64), be_const_closure(is_color_provider_closure) },
        { be_const_key_weak(TRIANGLE, -1), be_const_int(2) },
        { be_const_key_weak(solid, -1), be_const_closure(solid_closure) },
        { be_const_key_weak(gradient_two_color_linear, 48), be_const_closure(gradient_two_color_linear_closure) },
        { be_const_key_weak(PALETTE_RAINBOW, -1), be_const_bytes_instance(00FF000024FFA50049FFFF006E00FF00920000FFB74B0082DBEE82EEFFFF0000) },
        { be_const_key_weak(color_cycle, -1), be_const_class(be_class_ColorCycleColorProvider) },
        { be_const_key_weak(rich_palette_animation, -1), be_const_class(be_class_RichPaletteAnimation) },
        { be_const_key_weak(LINEAR, -1), be_const_int(1) },
        { be_const_key_weak(triangle, -1), be_const_closure(triangle_closure) },
        { be_const_key_weak(init, -1), be_const_closure(animation_init_closure) },
        { be_const_key_weak(fire_animation, -1), be_const_class(be_class_FireAnimation) },
        { be_const_key_weak(animation, 14), be_const_class(be_class_Animation) },
        { be_const_key_weak(frame_buffer, -1), be_const_class(be_class_FrameBuffer) },
        { be_const_key_weak(closure_value, -1), be_const_class(be_class_ClosureValueProvider) },
        { be_const_key_weak(smooth, -1), be_const_closure(smooth_closure) },
        { be_const_key_weak(crenel_position_animation, -1), be_const_class(be_class_CrenelPositionAnimation) },
        { be_const_key_weak(BOUNCE, -1), be_const_int(9) },
        { be_const_key_weak(palette_wave_animation, -1), be_const_class(be_class_PaletteWaveAnimation) },
        { be_const_key_weak(ramp, -1), be_const_closure(ramp_closure) },
        { be_const_key_weak(unregister_event_handler, 32), be_const_closure(unregister_event_handler_closure) },
        { be_const_key_weak(palette_pattern_animation, 46), be_const_class(be_class_PalettePatternAnimation) },
        { be_const_key_weak(static_color, 50), be_const_class(be_class_StaticColorProvider) },
        { be_const_key_weak(SAWTOOTH, -1), be_const_int(1) },
        { be_const_key_weak(bounce, 58), be_const_closure(bounce_closure) },
        { be_const_key_weak(twinkle_animation, -1), be_const_class(be_class_TwinkleAnimation) },
        { be_const_key_weak(wave_custom, -1), be_const_closure(wave_custom_closure) },
        { be_const_key_weak(list_user_functions, -1), be_const_closure(list_user_functions_closure) },
        { be_const_key_weak(twinkle_gentle, -1), be_const_closure(twinkle_gentle_closure) },
        { be_const_key_weak(palette_meter_animation, 93), be_const_class(be_class_PaletteMeterAnimation) },
        { be_const_key_weak(EASE_OUT, -1), be_const_int(7) },
        { be_const_key_weak(sawtooth, -1), be_const_closure(sawtooth_closure) },
        { be_const_key_weak(SINE, 67), be_const_int(5) },
        { be_const_key_weak(is_value_provider, 21), be_const_closure(is_value_provider_closure) },
        { be_const_key_weak(parameterized_object, -1), be_const_class(be_class_ParameterizedObject) },
        { be_const_key_weak(twinkle_rainbow, 65), be_const_closure(twinkle_rainbow_closure) },
        { be_const_key_weak(resolve, 33), be_const_closure(animation_resolve_closure) },
        { be_const_key_weak(beacon_animation, 16), be_const_class(be_class_BeaconAnimation) },
        { be_const_key_weak(breathe_animation, 12), be_const_class(be_class_BreatheAnimation) },
        { be_const_key_weak(comet_animation, 43), be_const_class(be_class_CometAnimation) },
        { be_const_key_weak(get_user_function, -1), be_const_closure(get_user_function_closure) },
        { be_const_key_weak(oscillator_value, -1), be_const_class(be_class_OscillatorValueProvider) },
        { be_const_key_weak(wave_animation, -1), be_const_class(be_class_WaveAnimation) },
        { be_const_key_weak(event_handler, -1), be_const_class(be_class_EventHandler) },
        { be_const_key_weak(square, -1), be_const_closure(square_closure) },
        { be_const_key_weak(wave_rainbow_sine, -1), be_const_closure(wave_rainbow_sine_closure) },
        { be_const_key_weak(version_string, -1), be_const_closure(animation_version_string_closure) },
        { be_const_key_weak(COSINE, -1), be_const_int(4) },
        { be_const_key_weak(twinkle_solid, -1), be_const_closure(twinkle_solid_closure) },
        { be_const_key_weak(rich_palette_rainbow, 75), be_const_closure(rich_palette_rainbow_closure) },
        { be_const_key_weak(gradient_rainbow_radial, -1), be_const_closure(gradient_rainbow_radial_closure) },
        { be_const_key_weak(value_provider, -1), be_const_class(be_class_ValueProvider) },
        { be_const_key_weak(pulsating_color, 38), be_const_closure(pulsating_color_provider_closure) },
        { be_const_key_weak(cosine_osc, 85), be_const_closure(cosine_osc_closure) },
        { be_const_key_weak(composite_color, -1), be_const_class(be_class_CompositeColorProvider) },
        { be_const_key_weak(gradient_animation, -1), be_const_class(be_class_GradientAnimation) },
        { be_const_key_weak(sine_osc, 29), be_const_closure(sine_osc_closure) },
        { be_const_key_weak(create_engine, -1), be_const_class(be_class_AnimationEngine) },
        { be_const_key_weak(trigger_event, -1), be_const_closure(trigger_event_closure) },
        { be_const_key_weak(clear_all_event_handlers, -1), be_const_closure(clear_all_event_handlers_closure) },
        { be_const_key_weak(linear, 15), be_const_closure(linear_closure) },
        { be_const_key_weak(noise_fractal, -1), be_const_closure(noise_fractal_closure) },
        { be_const_key_weak(twinkle_intense, 8), be_const_closure(twinkle_intense_closure) },
        { be_const_key_weak(noise_animation, -1), be_const_class(be_class_NoiseAnimation) },
        { be_const_key_weak(SQUARE, -1), be_const_int(3) },
        { be_const_key_weak(init_strip, -1), be_const_closure(animation_init_strip_closure) },
        { be_const_key_weak(PALETTE_FOREST, -1), be_const_bytes_instance(0000640040228B228032CD32C09AFF9AFF90EE90) },
        { be_const_key_weak(register_event_handler, 80), be_const_closure(register_event_handler_closure) },
        { be_const_key_weak(ease_out, -1), be_const_closure(ease_out_closure) },
        { be_const_key_weak(gradient_rainbow_linear, -1), be_const_closure(gradient_rainbow_linear_closure) },
        { be_const_key_weak(noise_single_color, 28), be_const_closure(noise_single_color_closure) },
        { be_const_key_weak(PALETTE_FIRE, 96), be_const_bytes_instance(000000004080000080FF0000C0FF8000FFFFFF00) },
        { be_const_key_weak(get_registered_events, -1), be_const_closure(get_registered_events_closure) },
        { be_const_key_weak(set_event_active, -1), be_const_closure(set_event_active_closure) },
        { be_const_key_weak(PALETTE_OCEAN, -1), be_const_bytes_instance(00000080400000FF8000FFFFC000FF80FF008000) },
        { be_const_key_weak(create_closure_value, -1), be_const_closure(create_closure_value_closure) },
        { be_const_key_weak(wave_single_sine, -1), be_const_closure(wave_single_sine_closure) },
    }))
);
BE_EXPORT_VARIABLE be_define_const_native_module(animation);
/********************************************************************/
/********************************************************************/
/* End of solidification */
