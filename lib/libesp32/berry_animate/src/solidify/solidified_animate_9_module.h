/* Solidification of animate_9_module.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Animate_animator' ktab size: 10, total: 14 (saved 32 bytes)
static const bvalue be_ktab_class_Animate_animator[10] = {
  /* K0   */  be_nested_str_weak(running),
  /* K1   */  be_nested_str_weak(global),
  /* K2   */  be_nested_str_weak(_cur_anim),
  /* K3   */  be_nested_str_weak(add_animator),
  /* K4   */  be_nested_str_weak(origin),
  /* K5   */  be_nested_str_weak(duration_ms),
  /* K6   */  be_nested_str_weak(obj),
  /* K7   */  be_nested_str_weak(mth),
  /* K8   */  be_nested_str_weak(tasmota),
  /* K9   */  be_nested_str_weak(millis),
};


extern const bclass be_class_Animate_animator;

/********************************************************************
** Solidified function: is_running
********************************************************************/
be_local_closure(class_Animate_animator_is_running,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_animator,     /* shared constants */
    be_str_weak(is_running),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x60040017,  //  0000  GETGBL	R1	G23
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: beat
********************************************************************/
be_local_closure(class_Animate_animator_beat,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_animator,     /* shared constants */
    be_str_weak(beat),
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
be_local_closure(class_Animate_animator_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_animator,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0xB8060200,  //  0000  GETNGBL	R1	K1
      0x88040302,  //  0001  GETMBR	R1	R1	K2
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0002,  //  0004  JMPF	R2	#0008
      0x8C080303,  //  0005  GETMET	R2	R1	K3
      0x5C100000,  //  0006  MOVE	R4	R0
      0x7C080400,  //  0007  CALL	R2	2
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: stop
********************************************************************/
be_local_closure(class_Animate_animator_stop,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_animator,     /* shared constants */
    be_str_weak(stop),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x4C040000,  //  0000  LDNIL	R1
      0x90020801,  //  0001  SETMBR	R0	K4	R1
      0x50040000,  //  0002  LDBOOL	R1	0	0
      0x90020001,  //  0003  SETMBR	R0	K0	R1
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_duration_ms
********************************************************************/
be_local_closure(class_Animate_animator_set_duration_ms,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_animator,     /* shared constants */
    be_str_weak(set_duration_ms),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020A01,  //  0000  SETMBR	R0	K5	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_cb
********************************************************************/
be_local_closure(class_Animate_animator_set_cb,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_animator,     /* shared constants */
    be_str_weak(set_cb),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x90020C01,  //  0000  SETMBR	R0	K6	R1
      0x90020E02,  //  0001  SETMBR	R0	K7	R2
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(class_Animate_animator_start,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_animator,     /* shared constants */
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x88080105,  //  0000  GETMBR	R2	R0	K5
      0x4C0C0000,  //  0001  LDNIL	R3
      0x1C080403,  //  0002  EQ	R2	R2	R3
      0x780A0000,  //  0003  JMPF	R2	#0005
      0x80000400,  //  0004  RET	0
      0x4C080000,  //  0005  LDNIL	R2
      0x1C080202,  //  0006  EQ	R2	R1	R2
      0x780A0003,  //  0007  JMPF	R2	#000C
      0xB80A1000,  //  0008  GETNGBL	R2	K8
      0x8C080509,  //  0009  GETMET	R2	R2	K9
      0x7C080200,  //  000A  CALL	R2	1
      0x5C040400,  //  000B  MOVE	R1	R2
      0x90020801,  //  000C  SETMBR	R0	K4	R1
      0x50080200,  //  000D  LDBOOL	R2	1	0
      0x90020002,  //  000E  SETMBR	R0	K0	R2
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Animate_animator
********************************************************************/
be_local_class(Animate_animator,
    5,
    NULL,
    be_nested_map(12,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(running, 4), be_const_var(0) },
        { be_const_key_weak(is_running, 2), be_const_closure(class_Animate_animator_is_running_closure) },
        { be_const_key_weak(beat, -1), be_const_closure(class_Animate_animator_beat_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_Animate_animator_init_closure) },
        { be_const_key_weak(mth, -1), be_const_var(4) },
        { be_const_key_weak(stop, -1), be_const_closure(class_Animate_animator_stop_closure) },
        { be_const_key_weak(duration_ms, -1), be_const_var(1) },
        { be_const_key_weak(origin, -1), be_const_var(2) },
        { be_const_key_weak(set_cb, -1), be_const_closure(class_Animate_animator_set_cb_closure) },
        { be_const_key_weak(set_duration_ms, 8), be_const_closure(class_Animate_animator_set_duration_ms_closure) },
        { be_const_key_weak(obj, -1), be_const_var(3) },
        { be_const_key_weak(start, -1), be_const_closure(class_Animate_animator_start_closure) },
    })),
    be_str_weak(Animate_animator)
);
extern const bclass be_class_Animate_palette;
// compact class 'Animate_palette' ktab size: 43, total: 87 (saved 352 bytes)
static const bvalue be_ktab_class_Animate_palette[43] = {
  /* K0   */  be_const_class(be_class_Animate_palette),
  /* K1   */  be_nested_str_weak(ptr),
  /* K2   */  be_const_int(1),
  /* K3   */  be_const_int(0),
  /* K4   */  be_nested_str_weak(duration_ms),
  /* K5   */  be_nested_str_weak(tasmota),
  /* K6   */  be_nested_str_weak(millis),
  /* K7   */  be_nested_str_weak(origin),
  /* K8   */  be_nested_str_weak(scale_uint),
  /* K9   */  be_nested_str_weak(slots),
  /* K10  */  be_const_int(2),
  /* K11  */  be_nested_str_weak(slots_arr),
  /* K12  */  be_nested_str_weak(palette),
  /* K13  */  be_nested_str_weak(get),
  /* K14  */  be_nested_str_weak(color),
  /* K15  */  be_nested_str_weak(set_rgb),
  /* K16  */  be_nested_str_weak(bri),
  /* K17  */  be_nested_str_weak(set_bri),
  /* K18  */  be_nested_str_weak(r),
  /* K19  */  be_nested_str_weak(g),
  /* K20  */  be_nested_str_weak(b),
  /* K21  */  be_nested_str_weak(obj),
  /* K22  */  be_nested_str_weak(mth),
  /* K23  */  be_nested_str_weak(ptr_to_palette),
  /* K24  */  be_nested_str_weak(set_duration),
  /* K25  */  be_nested_str_weak(range_min),
  /* K26  */  be_nested_str_weak(range_max),
  /* K27  */  be_nested_str_weak(set_range),
  /* K28  */  be_nested_str_weak(parse_palette),
  /* K29  */  be_nested_str_weak(background_X3Alinear_X2Dgradient_X28to_X20right),
  /* K30  */  be_nested_str_weak(_X2C_X23_X2502X_X2502X_X2502X_X20_X25_X2E1f_X25_X25),
  /* K31  */  be_const_real_hex(0x41200000),
  /* K32  */  be_nested_str_weak(_X29_X3B),
  /* K33  */  be_nested_str_weak(resize),
  /* K34  */  be_nested_str_weak(scale_int),
  /* K35  */  be_nested_str_weak(value_error),
  /* K36  */  be_nested_str_weak(min_X20must_X20be_X20lower_X20than_X20mex),
  /* K37  */  be_nested_str_weak(duration_ms_X20must_X20be_X20positive),
  /* K38  */  be_nested_str_weak(init),
  /* K39  */  be_nested_str_weak(running),
  /* K40  */  be_nested_str_weak(light_state),
  /* K41  */  be_nested_str_weak(RGB),
  /* K42  */  be_nested_str_weak(set_palette),
};


extern const bclass be_class_Animate_palette;

/********************************************************************
** Solidified function: ptr_to_palette
********************************************************************/
be_local_closure(class_Animate_palette_ptr_to_palette,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_palette,     /* shared constants */
    be_str_weak(ptr_to_palette),
    &be_const_str_solidified,
    ( &(const binstruction[45]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x60080004,  //  0001  GETGBL	R2	G4
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C080200,  //  0003  CALL	R2	1
      0x1C080501,  //  0004  EQ	R2	R2	K1
      0x780A0025,  //  0005  JMPF	R2	#002C
      0x60080015,  //  0006  GETGBL	R2	G21
      0x5C0C0000,  //  0007  MOVE	R3	R0
      0x541207CF,  //  0008  LDINT	R4	2000
      0x7C080400,  //  0009  CALL	R2	2
      0x580C0002,  //  000A  LDCONST	R3	K2
      0x94100503,  //  000B  GETIDX	R4	R2	K3
      0x20100903,  //  000C  NE	R4	R4	K3
      0x7812000A,  //  000D  JMPF	R4	#0019
      0x50100200,  //  000E  LDBOOL	R4	1	0
      0x78120007,  //  000F  JMPF	R4	#0018
      0x54120003,  //  0010  LDINT	R4	4
      0x08100604,  //  0011  MUL	R4	R3	R4
      0x94100404,  //  0012  GETIDX	R4	R2	R4
      0x1C100903,  //  0013  EQ	R4	R4	K3
      0x78120000,  //  0014  JMPF	R4	#0016
      0x70020001,  //  0015  JMP		#0018
      0x000C0702,  //  0016  ADD	R3	R3	K2
      0x7001FFF5,  //  0017  JMP		#000E
      0x7002000A,  //  0018  JMP		#0024
      0x50100200,  //  0019  LDBOOL	R4	1	0
      0x78120008,  //  001A  JMPF	R4	#0024
      0x54120003,  //  001B  LDINT	R4	4
      0x08100604,  //  001C  MUL	R4	R3	R4
      0x94100404,  //  001D  GETIDX	R4	R2	R4
      0x541600FE,  //  001E  LDINT	R5	255
      0x1C100805,  //  001F  EQ	R4	R4	R5
      0x78120000,  //  0020  JMPF	R4	#0022
      0x70020001,  //  0021  JMP		#0024
      0x000C0702,  //  0022  ADD	R3	R3	K2
      0x7001FFF4,  //  0023  JMP		#0019
      0x00100702,  //  0024  ADD	R4	R3	K2
      0x54160003,  //  0025  LDINT	R5	4
      0x08100805,  //  0026  MUL	R4	R4	R5
      0x60140015,  //  0027  GETGBL	R5	G21
      0x5C180000,  //  0028  MOVE	R6	R0
      0x5C1C0800,  //  0029  MOVE	R7	R4
      0x7C140400,  //  002A  CALL	R5	2
      0x80040A00,  //  002B  RET	1	R5
      0x80000000,  //  002C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: animate
********************************************************************/
be_local_closure(class_Animate_palette_animate,   /* name */
  be_nested_proto(
    26,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_palette,     /* shared constants */
    be_str_weak(animate),
    &be_const_str_solidified,
    ( &(const binstruction[193]) {  /* code */
      0x88080104,  //  0000  GETMBR	R2	R0	K4
      0x4C0C0000,  //  0001  LDNIL	R3
      0x1C080403,  //  0002  EQ	R2	R2	R3
      0x780A0000,  //  0003  JMPF	R2	#0005
      0x80000400,  //  0004  RET	0
      0x4C080000,  //  0005  LDNIL	R2
      0x1C080202,  //  0006  EQ	R2	R1	R2
      0x780A0003,  //  0007  JMPF	R2	#000C
      0xB80A0A00,  //  0008  GETNGBL	R2	K5
      0x8C080506,  //  0009  GETMET	R2	R2	K6
      0x7C080200,  //  000A  CALL	R2	1
      0x5C040400,  //  000B  MOVE	R1	R2
      0x88080107,  //  000C  GETMBR	R2	R0	K7
      0x04080202,  //  000D  SUB	R2	R1	R2
      0x140C0503,  //  000E  LT	R3	R2	K3
      0x780E0001,  //  000F  JMPF	R3	#0012
      0x58080003,  //  0010  LDCONST	R2	K3
      0x88040107,  //  0011  GETMBR	R1	R0	K7
      0x880C0104,  //  0012  GETMBR	R3	R0	K4
      0xB8120A00,  //  0013  GETNGBL	R4	K5
      0x88100908,  //  0014  GETMBR	R4	R4	K8
      0x28140403,  //  0015  GE	R5	R2	R3
      0x78160005,  //  0016  JMPF	R5	#001D
      0x0C180403,  //  0017  DIV	R6	R2	R3
      0x08180C03,  //  0018  MUL	R6	R6	R3
      0x88140107,  //  0019  GETMBR	R5	R0	K7
      0x00140A06,  //  001A  ADD	R5	R5	R6
      0x90020E05,  //  001B  SETMBR	R0	K7	R5
      0x10080403,  //  001C  MOD	R2	R2	R3
      0x88140109,  //  001D  GETMBR	R5	R0	K9
      0x04180B0A,  //  001E  SUB	R6	R5	K10
      0x241C0D03,  //  001F  GT	R7	R6	K3
      0x781E0006,  //  0020  JMPF	R7	#0028
      0x881C010B,  //  0021  GETMBR	R7	R0	K11
      0x941C0E06,  //  0022  GETIDX	R7	R7	R6
      0x281C0407,  //  0023  GE	R7	R2	R7
      0x781E0000,  //  0024  JMPF	R7	#0026
      0x70020001,  //  0025  JMP		#0028
      0x04180D02,  //  0026  SUB	R6	R6	K2
      0x7001FFF6,  //  0027  JMP		#001F
      0x881C010C,  //  0028  GETMBR	R7	R0	K12
      0x8C1C0F0D,  //  0029  GETMET	R7	R7	K13
      0x54260003,  //  002A  LDINT	R9	4
      0x08240C09,  //  002B  MUL	R9	R6	R9
      0x542A0003,  //  002C  LDINT	R10	4
      0x7C1C0600,  //  002D  CALL	R7	3
      0x8820010C,  //  002E  GETMBR	R8	R0	K12
      0x8C20110D,  //  002F  GETMET	R8	R8	K13
      0x00280D02,  //  0030  ADD	R10	R6	K2
      0x542E0003,  //  0031  LDINT	R11	4
      0x0828140B,  //  0032  MUL	R10	R10	R11
      0x542E0003,  //  0033  LDINT	R11	4
      0x7C200600,  //  0034  CALL	R8	3
      0x8824010B,  //  0035  GETMBR	R9	R0	K11
      0x94241206,  //  0036  GETIDX	R9	R9	R6
      0x00280D02,  //  0037  ADD	R10	R6	K2
      0x882C010B,  //  0038  GETMBR	R11	R0	K11
      0x9428160A,  //  0039  GETIDX	R10	R11	R10
      0x5C300800,  //  003A  MOVE	R12	R4
      0x5C340400,  //  003B  MOVE	R13	R2
      0x5C381200,  //  003C  MOVE	R14	R9
      0x5C3C1400,  //  003D  MOVE	R15	R10
      0x54420007,  //  003E  LDINT	R16	8
      0x3C400E10,  //  003F  SHR	R16	R7	R16
      0x544600FE,  //  0040  LDINT	R17	255
      0x2C402011,  //  0041  AND	R16	R16	R17
      0x54460007,  //  0042  LDINT	R17	8
      0x3C441011,  //  0043  SHR	R17	R8	R17
      0x544A00FE,  //  0044  LDINT	R18	255
      0x2C442212,  //  0045  AND	R17	R17	R18
      0x7C300A00,  //  0046  CALL	R12	5
      0x5C2C1800,  //  0047  MOVE	R11	R12
      0x5C300800,  //  0048  MOVE	R12	R4
      0x5C340400,  //  0049  MOVE	R13	R2
      0x5C381200,  //  004A  MOVE	R14	R9
      0x5C3C1400,  //  004B  MOVE	R15	R10
      0x5442000F,  //  004C  LDINT	R16	16
      0x3C400E10,  //  004D  SHR	R16	R7	R16
      0x544600FE,  //  004E  LDINT	R17	255
      0x2C402011,  //  004F  AND	R16	R16	R17
      0x5446000F,  //  0050  LDINT	R17	16
      0x3C441011,  //  0051  SHR	R17	R8	R17
      0x544A00FE,  //  0052  LDINT	R18	255
      0x2C442212,  //  0053  AND	R17	R17	R18
      0x7C300A00,  //  0054  CALL	R12	5
      0x5C340800,  //  0055  MOVE	R13	R4
      0x5C380400,  //  0056  MOVE	R14	R2
      0x5C3C1200,  //  0057  MOVE	R15	R9
      0x5C401400,  //  0058  MOVE	R16	R10
      0x54460017,  //  0059  LDINT	R17	24
      0x3C440E11,  //  005A  SHR	R17	R7	R17
      0x544A00FE,  //  005B  LDINT	R18	255
      0x2C442212,  //  005C  AND	R17	R17	R18
      0x544A0017,  //  005D  LDINT	R18	24
      0x3C481012,  //  005E  SHR	R18	R8	R18
      0x544E00FE,  //  005F  LDINT	R19	255
      0x2C482413,  //  0060  AND	R18	R18	R19
      0x7C340A00,  //  0061  CALL	R13	5
      0x8838010E,  //  0062  GETMBR	R14	R0	K14
      0x8C3C1D0F,  //  0063  GETMET	R15	R14	K15
      0x54460007,  //  0064  LDINT	R17	8
      0x3C440E11,  //  0065  SHR	R17	R7	R17
      0x544A00FE,  //  0066  LDINT	R18	255
      0x2C442212,  //  0067  AND	R17	R17	R18
      0x544A000F,  //  0068  LDINT	R18	16
      0x3C480E12,  //  0069  SHR	R18	R7	R18
      0x544E00FE,  //  006A  LDINT	R19	255
      0x2C482413,  //  006B  AND	R18	R18	R19
      0x544E0017,  //  006C  LDINT	R19	24
      0x3C4C0E13,  //  006D  SHR	R19	R7	R19
      0x545200FE,  //  006E  LDINT	R20	255
      0x2C4C2614,  //  006F  AND	R19	R19	R20
      0x7C3C0800,  //  0070  CALL	R15	4
      0x883C1D10,  //  0071  GETMBR	R15	R14	K16
      0x8C401D0F,  //  0072  GETMET	R16	R14	K15
      0x544A0007,  //  0073  LDINT	R18	8
      0x3C481012,  //  0074  SHR	R18	R8	R18
      0x544E00FE,  //  0075  LDINT	R19	255
      0x2C482413,  //  0076  AND	R18	R18	R19
      0x544E000F,  //  0077  LDINT	R19	16
      0x3C4C1013,  //  0078  SHR	R19	R8	R19
      0x545200FE,  //  0079  LDINT	R20	255
      0x2C4C2614,  //  007A  AND	R19	R19	R20
      0x54520017,  //  007B  LDINT	R20	24
      0x3C501014,  //  007C  SHR	R20	R8	R20
      0x545600FE,  //  007D  LDINT	R21	255
      0x2C502815,  //  007E  AND	R20	R20	R21
      0x7C400800,  //  007F  CALL	R16	4
      0x88401D10,  //  0080  GETMBR	R16	R14	K16
      0x5C440800,  //  0081  MOVE	R17	R4
      0x5C480400,  //  0082  MOVE	R18	R2
      0x5C4C1200,  //  0083  MOVE	R19	R9
      0x5C501400,  //  0084  MOVE	R20	R10
      0x5C541E00,  //  0085  MOVE	R21	R15
      0x5C582000,  //  0086  MOVE	R22	R16
      0x7C440A00,  //  0087  CALL	R17	5
      0x8C481D0F,  //  0088  GETMET	R18	R14	K15
      0x5C501600,  //  0089  MOVE	R20	R11
      0x5C541800,  //  008A  MOVE	R21	R12
      0x5C581A00,  //  008B  MOVE	R22	R13
      0x7C480800,  //  008C  CALL	R18	4
      0x8C481D11,  //  008D  GETMET	R18	R14	K17
      0x5C502200,  //  008E  MOVE	R20	R17
      0x7C480400,  //  008F  CALL	R18	2
      0x882C1D12,  //  0090  GETMBR	R11	R14	K18
      0x88301D13,  //  0091  GETMBR	R12	R14	K19
      0x88341D14,  //  0092  GETMBR	R13	R14	K20
      0x88480110,  //  0093  GETMBR	R18	R0	K16
      0x544E0063,  //  0094  LDINT	R19	100
      0x204C2413,  //  0095  NE	R19	R18	R19
      0x784E001A,  //  0096  JMPF	R19	#00B2
      0xB84E0A00,  //  0097  GETNGBL	R19	K5
      0x8C4C2708,  //  0098  GETMET	R19	R19	K8
      0x5C541600,  //  0099  MOVE	R21	R11
      0x58580003,  //  009A  LDCONST	R22	K3
      0x545E0063,  //  009B  LDINT	R23	100
      0x58600003,  //  009C  LDCONST	R24	K3
      0x5C642400,  //  009D  MOVE	R25	R18
      0x7C4C0C00,  //  009E  CALL	R19	6
      0x5C2C2600,  //  009F  MOVE	R11	R19
      0xB84E0A00,  //  00A0  GETNGBL	R19	K5
      0x8C4C2708,  //  00A1  GETMET	R19	R19	K8
      0x5C541800,  //  00A2  MOVE	R21	R12
      0x58580003,  //  00A3  LDCONST	R22	K3
      0x545E0063,  //  00A4  LDINT	R23	100
      0x58600003,  //  00A5  LDCONST	R24	K3
      0x5C642400,  //  00A6  MOVE	R25	R18
      0x7C4C0C00,  //  00A7  CALL	R19	6
      0x5C302600,  //  00A8  MOVE	R12	R19
      0xB84E0A00,  //  00A9  GETNGBL	R19	K5
      0x8C4C2708,  //  00AA  GETMET	R19	R19	K8
      0x5C541A00,  //  00AB  MOVE	R21	R13
      0x58580003,  //  00AC  LDCONST	R22	K3
      0x545E0063,  //  00AD  LDINT	R23	100
      0x58600003,  //  00AE  LDCONST	R24	K3
      0x5C642400,  //  00AF  MOVE	R25	R18
      0x7C4C0C00,  //  00B0  CALL	R19	6
      0x5C342600,  //  00B1  MOVE	R13	R19
      0x544E000F,  //  00B2  LDINT	R19	16
      0x384C1613,  //  00B3  SHL	R19	R11	R19
      0x54520007,  //  00B4  LDINT	R20	8
      0x38501814,  //  00B5  SHL	R20	R12	R20
      0x304C2614,  //  00B6  OR	R19	R19	R20
      0x304C260D,  //  00B7  OR	R19	R19	R13
      0x88500115,  //  00B8  GETMBR	R20	R0	K21
      0x88540116,  //  00B9  GETMBR	R21	R0	K22
      0x78520004,  //  00BA  JMPF	R20	#00C0
      0x78560003,  //  00BB  JMPF	R21	#00C0
      0x5C582A00,  //  00BC  MOVE	R22	R21
      0x5C5C2800,  //  00BD  MOVE	R23	R20
      0x5C602600,  //  00BE  MOVE	R24	R19
      0x7C580400,  //  00BF  CALL	R22	2
      0x80042600,  //  00C0  RET	1	R19
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_palette
********************************************************************/
be_local_closure(class_Animate_palette_set_palette,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_palette,     /* shared constants */
    be_str_weak(set_palette),
    &be_const_str_solidified,
    ( &(const binstruction[37]) {  /* code */
      0x60080004,  //  0000  GETGBL	R2	G4
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x1C080501,  //  0003  EQ	R2	R2	K1
      0x780A0003,  //  0004  JMPF	R2	#0009
      0x8C080117,  //  0005  GETMET	R2	R0	K23
      0x5C100200,  //  0006  MOVE	R4	R1
      0x7C080400,  //  0007  CALL	R2	2
      0x5C040400,  //  0008  MOVE	R1	R2
      0x90021801,  //  0009  SETMBR	R0	K12	R1
      0x6008000C,  //  000A  GETGBL	R2	G12
      0x5C0C0200,  //  000B  MOVE	R3	R1
      0x7C080200,  //  000C  CALL	R2	1
      0x540E0003,  //  000D  LDINT	R3	4
      0x0C080403,  //  000E  DIV	R2	R2	R3
      0x90021202,  //  000F  SETMBR	R0	K9	R2
      0x88080104,  //  0010  GETMBR	R2	R0	K4
      0x4C0C0000,  //  0011  LDNIL	R3
      0x20080403,  //  0012  NE	R2	R2	R3
      0x780A0003,  //  0013  JMPF	R2	#0018
      0x8C080118,  //  0014  GETMET	R2	R0	K24
      0x88100104,  //  0015  GETMBR	R4	R0	K4
      0x7C080400,  //  0016  CALL	R2	2
      0x7002000B,  //  0017  JMP		#0024
      0x88080119,  //  0018  GETMBR	R2	R0	K25
      0x4C0C0000,  //  0019  LDNIL	R3
      0x20080403,  //  001A  NE	R2	R2	R3
      0x780A0007,  //  001B  JMPF	R2	#0024
      0x8808011A,  //  001C  GETMBR	R2	R0	K26
      0x4C0C0000,  //  001D  LDNIL	R3
      0x20080403,  //  001E  NE	R2	R2	R3
      0x780A0003,  //  001F  JMPF	R2	#0024
      0x8C08011B,  //  0020  GETMET	R2	R0	K27
      0x88100119,  //  0021  GETMBR	R4	R0	K25
      0x8814011A,  //  0022  GETMBR	R5	R0	K26
      0x7C080600,  //  0023  CALL	R2	3
      0x80000000,  //  0024  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: to_css_gradient
********************************************************************/
be_local_closure(class_Animate_palette_to_css_gradient,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    1,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_palette,     /* shared constants */
    be_str_weak(to_css_gradient),
    &be_const_str_solidified,
    ( &(const binstruction[46]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x5C080200,  //  0001  MOVE	R2	R1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C080200,  //  0003  CALL	R2	1
      0x8C0C051C,  //  0004  GETMET	R3	R2	K28
      0x58140003,  //  0005  LDCONST	R5	K3
      0x541A03E7,  //  0006  LDINT	R6	1000
      0x7C0C0600,  //  0007  CALL	R3	3
      0x5810001D,  //  0008  LDCONST	R4	K29
      0x58140003,  //  0009  LDCONST	R5	K3
      0x6018000C,  //  000A  GETGBL	R6	G12
      0x5C1C0600,  //  000B  MOVE	R7	R3
      0x7C180200,  //  000C  CALL	R6	1
      0x14180A06,  //  000D  LT	R6	R5	R6
      0x781A001C,  //  000E  JMPF	R6	#002C
      0x94180605,  //  000F  GETIDX	R6	R3	R5
      0x881C050C,  //  0010  GETMBR	R7	R2	K12
      0x8C1C0F0D,  //  0011  GETMET	R7	R7	K13
      0x54260003,  //  0012  LDINT	R9	4
      0x08240A09,  //  0013  MUL	R9	R5	R9
      0x542A0003,  //  0014  LDINT	R10	4
      0x7C1C0600,  //  0015  CALL	R7	3
      0x54220007,  //  0016  LDINT	R8	8
      0x3C200E08,  //  0017  SHR	R8	R7	R8
      0x542600FE,  //  0018  LDINT	R9	255
      0x2C201009,  //  0019  AND	R8	R8	R9
      0x5426000F,  //  001A  LDINT	R9	16
      0x3C240E09,  //  001B  SHR	R9	R7	R9
      0x542A00FE,  //  001C  LDINT	R10	255
      0x2C24120A,  //  001D  AND	R9	R9	R10
      0x542A0017,  //  001E  LDINT	R10	24
      0x3C280E0A,  //  001F  SHR	R10	R7	R10
      0x542E00FE,  //  0020  LDINT	R11	255
      0x2C28140B,  //  0021  AND	R10	R10	R11
      0x602C0018,  //  0022  GETGBL	R11	G24
      0x5830001E,  //  0023  LDCONST	R12	K30
      0x5C341000,  //  0024  MOVE	R13	R8
      0x5C381200,  //  0025  MOVE	R14	R9
      0x5C3C1400,  //  0026  MOVE	R15	R10
      0x0C400D1F,  //  0027  DIV	R16	R6	K31
      0x7C2C0A00,  //  0028  CALL	R11	5
      0x0010080B,  //  0029  ADD	R4	R4	R11
      0x00140B02,  //  002A  ADD	R5	R5	K2
      0x7001FFDD,  //  002B  JMP		#000A
      0x00100920,  //  002C  ADD	R4	R4	K32
      0x80040800,  //  002D  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_bri
********************************************************************/
be_local_closure(class_Animate_palette_set_bri,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_palette,     /* shared constants */
    be_str_weak(set_bri),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x60080009,  //  0000  GETGBL	R2	G9
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x90022002,  //  0003  SETMBR	R0	K16	R2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_palette
********************************************************************/
be_local_closure(class_Animate_palette_parse_palette,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_palette,     /* shared constants */
    be_str_weak(parse_palette),
    &be_const_str_solidified,
    ( &(const binstruction[71]) {  /* code */
      0x600C0012,  //  0000  GETGBL	R3	G18
      0x7C0C0000,  //  0001  CALL	R3	0
      0x88100109,  //  0002  GETMBR	R4	R0	K9
      0x8C140721,  //  0003  GETMET	R5	R3	K33
      0x5C1C0800,  //  0004  MOVE	R7	R4
      0x7C140400,  //  0005  CALL	R5	2
      0x8814010C,  //  0006  GETMBR	R5	R0	K12
      0x8C140B0D,  //  0007  GETMET	R5	R5	K13
      0x581C0003,  //  0008  LDCONST	R7	K3
      0x58200002,  //  0009  LDCONST	R8	K2
      0x7C140600,  //  000A  CALL	R5	3
      0x20140B03,  //  000B  NE	R5	R5	K3
      0x78160024,  //  000C  JMPF	R5	#0032
      0x58140003,  //  000D  LDCONST	R5	K3
      0x58180003,  //  000E  LDCONST	R6	K3
      0x041C0902,  //  000F  SUB	R7	R4	K2
      0x141C0C07,  //  0010  LT	R7	R6	R7
      0x781E0008,  //  0011  JMPF	R7	#001B
      0x881C010C,  //  0012  GETMBR	R7	R0	K12
      0x8C1C0F0D,  //  0013  GETMET	R7	R7	K13
      0x54260003,  //  0014  LDINT	R9	4
      0x08240C09,  //  0015  MUL	R9	R6	R9
      0x58280002,  //  0016  LDCONST	R10	K2
      0x7C1C0600,  //  0017  CALL	R7	3
      0x00140A07,  //  0018  ADD	R5	R5	R7
      0x00180D02,  //  0019  ADD	R6	R6	K2
      0x7001FFF3,  //  001A  JMP		#000F
      0x581C0003,  //  001B  LDCONST	R7	K3
      0x58180003,  //  001C  LDCONST	R6	K3
      0x14200C04,  //  001D  LT	R8	R6	R4
      0x78220011,  //  001E  JMPF	R8	#0031
      0xB8220A00,  //  001F  GETNGBL	R8	K5
      0x8C201122,  //  0020  GETMET	R8	R8	K34
      0x5C280E00,  //  0021  MOVE	R10	R7
      0x582C0003,  //  0022  LDCONST	R11	K3
      0x5C300A00,  //  0023  MOVE	R12	R5
      0x5C340200,  //  0024  MOVE	R13	R1
      0x5C380400,  //  0025  MOVE	R14	R2
      0x7C200C00,  //  0026  CALL	R8	6
      0x980C0C08,  //  0027  SETIDX	R3	R6	R8
      0x8820010C,  //  0028  GETMBR	R8	R0	K12
      0x8C20110D,  //  0029  GETMET	R8	R8	K13
      0x542A0003,  //  002A  LDINT	R10	4
      0x08280C0A,  //  002B  MUL	R10	R6	R10
      0x582C0002,  //  002C  LDCONST	R11	K2
      0x7C200600,  //  002D  CALL	R8	3
      0x001C0E08,  //  002E  ADD	R7	R7	R8
      0x00180D02,  //  002F  ADD	R6	R6	K2
      0x7001FFEB,  //  0030  JMP		#001D
      0x70020013,  //  0031  JMP		#0046
      0x58140003,  //  0032  LDCONST	R5	K3
      0x14180A04,  //  0033  LT	R6	R5	R4
      0x781A0010,  //  0034  JMPF	R6	#0046
      0x8818010C,  //  0035  GETMBR	R6	R0	K12
      0x8C180D0D,  //  0036  GETMET	R6	R6	K13
      0x54220003,  //  0037  LDINT	R8	4
      0x08200A08,  //  0038  MUL	R8	R5	R8
      0x58240002,  //  0039  LDCONST	R9	K2
      0x7C180600,  //  003A  CALL	R6	3
      0xB81E0A00,  //  003B  GETNGBL	R7	K5
      0x8C1C0F22,  //  003C  GETMET	R7	R7	K34
      0x5C240C00,  //  003D  MOVE	R9	R6
      0x58280003,  //  003E  LDCONST	R10	K3
      0x542E00FE,  //  003F  LDINT	R11	255
      0x5C300200,  //  0040  MOVE	R12	R1
      0x5C340400,  //  0041  MOVE	R13	R2
      0x7C1C0C00,  //  0042  CALL	R7	6
      0x980C0A07,  //  0043  SETIDX	R3	R5	R7
      0x00140B02,  //  0044  ADD	R5	R5	K2
      0x7001FFEC,  //  0045  JMP		#0033
      0x80040600,  //  0046  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_range
********************************************************************/
be_local_closure(class_Animate_palette_set_range,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_palette,     /* shared constants */
    be_str_weak(set_range),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x280C0202,  //  0000  GE	R3	R1	R2
      0x780E0000,  //  0001  JMPF	R3	#0003
      0xB0064724,  //  0002  RAISE	1	K35	K36
      0x90023201,  //  0003  SETMBR	R0	K25	R1
      0x90023402,  //  0004  SETMBR	R0	K26	R2
      0x8C0C011C,  //  0005  GETMET	R3	R0	K28
      0x5C140200,  //  0006  MOVE	R5	R1
      0x5C180400,  //  0007  MOVE	R6	R2
      0x7C0C0600,  //  0008  CALL	R3	3
      0x90021603,  //  0009  SETMBR	R0	K11	R3
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_value
********************************************************************/
be_local_closure(class_Animate_palette_set_value,   /* name */
  be_nested_proto(
    18,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_palette,     /* shared constants */
    be_str_weak(set_value),
    &be_const_str_solidified,
    ( &(const binstruction[96]) {  /* code */
      0x88080119,  //  0000  GETMBR	R2	R0	K25
      0x4C0C0000,  //  0001  LDNIL	R3
      0x1C080403,  //  0002  EQ	R2	R2	R3
      0x740A0003,  //  0003  JMPT	R2	#0008
      0x8808011A,  //  0004  GETMBR	R2	R0	K26
      0x4C0C0000,  //  0005  LDNIL	R3
      0x1C080403,  //  0006  EQ	R2	R2	R3
      0x780A0001,  //  0007  JMPF	R2	#000A
      0x4C080000,  //  0008  LDNIL	R2
      0x80040400,  //  0009  RET	1	R2
      0xB80A0A00,  //  000A  GETNGBL	R2	K5
      0x88080522,  //  000B  GETMBR	R2	R2	K34
      0x880C0109,  //  000C  GETMBR	R3	R0	K9
      0x0410070A,  //  000D  SUB	R4	R3	K10
      0x24140903,  //  000E  GT	R5	R4	K3
      0x78160006,  //  000F  JMPF	R5	#0017
      0x8814010B,  //  0010  GETMBR	R5	R0	K11
      0x94140A04,  //  0011  GETIDX	R5	R5	R4
      0x28140205,  //  0012  GE	R5	R1	R5
      0x78160000,  //  0013  JMPF	R5	#0015
      0x70020001,  //  0014  JMP		#0017
      0x04100902,  //  0015  SUB	R4	R4	K2
      0x7001FFF6,  //  0016  JMP		#000E
      0x8814010C,  //  0017  GETMBR	R5	R0	K12
      0x8C140B0D,  //  0018  GETMET	R5	R5	K13
      0x541E0003,  //  0019  LDINT	R7	4
      0x081C0807,  //  001A  MUL	R7	R4	R7
      0x54220003,  //  001B  LDINT	R8	4
      0x7C140600,  //  001C  CALL	R5	3
      0x8818010C,  //  001D  GETMBR	R6	R0	K12
      0x8C180D0D,  //  001E  GETMET	R6	R6	K13
      0x00200902,  //  001F  ADD	R8	R4	K2
      0x54260003,  //  0020  LDINT	R9	4
      0x08201009,  //  0021  MUL	R8	R8	R9
      0x54260003,  //  0022  LDINT	R9	4
      0x7C180600,  //  0023  CALL	R6	3
      0x881C010B,  //  0024  GETMBR	R7	R0	K11
      0x941C0E04,  //  0025  GETIDX	R7	R7	R4
      0x00200902,  //  0026  ADD	R8	R4	K2
      0x8824010B,  //  0027  GETMBR	R9	R0	K11
      0x94201208,  //  0028  GETIDX	R8	R9	R8
      0x5C280400,  //  0029  MOVE	R10	R2
      0x5C2C0200,  //  002A  MOVE	R11	R1
      0x5C300E00,  //  002B  MOVE	R12	R7
      0x5C341000,  //  002C  MOVE	R13	R8
      0x543A0007,  //  002D  LDINT	R14	8
      0x3C380A0E,  //  002E  SHR	R14	R5	R14
      0x543E00FE,  //  002F  LDINT	R15	255
      0x2C381C0F,  //  0030  AND	R14	R14	R15
      0x543E0007,  //  0031  LDINT	R15	8
      0x3C3C0C0F,  //  0032  SHR	R15	R6	R15
      0x544200FE,  //  0033  LDINT	R16	255
      0x2C3C1E10,  //  0034  AND	R15	R15	R16
      0x7C280A00,  //  0035  CALL	R10	5
      0x5C241400,  //  0036  MOVE	R9	R10
      0x5C280400,  //  0037  MOVE	R10	R2
      0x5C2C0200,  //  0038  MOVE	R11	R1
      0x5C300E00,  //  0039  MOVE	R12	R7
      0x5C341000,  //  003A  MOVE	R13	R8
      0x543A000F,  //  003B  LDINT	R14	16
      0x3C380A0E,  //  003C  SHR	R14	R5	R14
      0x543E00FE,  //  003D  LDINT	R15	255
      0x2C381C0F,  //  003E  AND	R14	R14	R15
      0x543E000F,  //  003F  LDINT	R15	16
      0x3C3C0C0F,  //  0040  SHR	R15	R6	R15
      0x544200FE,  //  0041  LDINT	R16	255
      0x2C3C1E10,  //  0042  AND	R15	R15	R16
      0x7C280A00,  //  0043  CALL	R10	5
      0x5C2C0400,  //  0044  MOVE	R11	R2
      0x5C300200,  //  0045  MOVE	R12	R1
      0x5C340E00,  //  0046  MOVE	R13	R7
      0x5C381000,  //  0047  MOVE	R14	R8
      0x543E0017,  //  0048  LDINT	R15	24
      0x3C3C0A0F,  //  0049  SHR	R15	R5	R15
      0x544200FE,  //  004A  LDINT	R16	255
      0x2C3C1E10,  //  004B  AND	R15	R15	R16
      0x54420017,  //  004C  LDINT	R16	24
      0x3C400C10,  //  004D  SHR	R16	R6	R16
      0x544600FE,  //  004E  LDINT	R17	255
      0x2C402011,  //  004F  AND	R16	R16	R17
      0x7C2C0A00,  //  0050  CALL	R11	5
      0x5432000F,  //  0051  LDINT	R12	16
      0x3830120C,  //  0052  SHL	R12	R9	R12
      0x54360007,  //  0053  LDINT	R13	8
      0x3834140D,  //  0054  SHL	R13	R10	R13
      0x3030180D,  //  0055  OR	R12	R12	R13
      0x3030180B,  //  0056  OR	R12	R12	R11
      0x88340115,  //  0057  GETMBR	R13	R0	K21
      0x88380116,  //  0058  GETMBR	R14	R0	K22
      0x78360004,  //  0059  JMPF	R13	#005F
      0x783A0003,  //  005A  JMPF	R14	#005F
      0x5C3C1C00,  //  005B  MOVE	R15	R14
      0x5C401A00,  //  005C  MOVE	R16	R13
      0x5C441800,  //  005D  MOVE	R17	R12
      0x7C3C0400,  //  005E  CALL	R15	2
      0x80041800,  //  005F  RET	1	R12
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_duration
********************************************************************/
be_local_closure(class_Animate_palette_set_duration,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_palette,     /* shared constants */
    be_str_weak(set_duration),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0000,  //  0002  JMPF	R2	#0004
      0x80000400,  //  0003  RET	0
      0x18080303,  //  0004  LE	R2	R1	K3
      0x780A0000,  //  0005  JMPF	R2	#0007
      0xB0064725,  //  0006  RAISE	1	K35	K37
      0x90020801,  //  0007  SETMBR	R0	K4	R1
      0x8C08011C,  //  0008  GETMET	R2	R0	K28
      0x58100003,  //  0009  LDCONST	R4	K3
      0x04140302,  //  000A  SUB	R5	R1	K2
      0x7C080600,  //  000B  CALL	R2	3
      0x90021602,  //  000C  SETMBR	R0	K11	R2
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Animate_palette_init,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_palette,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0726,  //  0003  GETMET	R3	R3	K38
      0x7C0C0200,  //  0004  CALL	R3	1
      0x90020802,  //  0005  SETMBR	R0	K4	R2
      0x500C0000,  //  0006  LDBOOL	R3	0	0
      0x90024E03,  //  0007  SETMBR	R0	K39	R3
      0x540E0063,  //  0008  LDINT	R3	100
      0x90022003,  //  0009  SETMBR	R0	K16	R3
      0xB80E5000,  //  000A  GETNGBL	R3	K40
      0xB8125000,  //  000B  GETNGBL	R4	K40
      0x88100929,  //  000C  GETMBR	R4	R4	K41
      0x7C0C0200,  //  000D  CALL	R3	1
      0x90021C03,  //  000E  SETMBR	R0	K14	R3
      0x8C0C012A,  //  000F  GETMET	R3	R0	K42
      0x5C140200,  //  0010  MOVE	R5	R1
      0x7C0C0400,  //  0011  CALL	R3	2
      0x80000000,  //  0012  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Animate_palette
********************************************************************/
extern const bclass be_class_Animate_animator;
be_local_class(Animate_palette,
    7,
    &be_class_Animate_animator,
    be_nested_map(17,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(color, 13), be_const_var(6) },
        { be_const_key_weak(slots, -1), be_const_var(2) },
        { be_const_key_weak(animate, 11), be_const_closure(class_Animate_palette_animate_closure) },
        { be_const_key_weak(range_max, 10), be_const_var(4) },
        { be_const_key_weak(set_palette, -1), be_const_closure(class_Animate_palette_set_palette_closure) },
        { be_const_key_weak(set_bri, -1), be_const_closure(class_Animate_palette_set_bri_closure) },
        { be_const_key_weak(bri, -1), be_const_var(5) },
        { be_const_key_weak(to_css_gradient, 12), be_const_static_closure(class_Animate_palette_to_css_gradient_closure) },
        { be_const_key_weak(slots_arr, 1), be_const_var(1) },
        { be_const_key_weak(range_min, 5), be_const_var(3) },
        { be_const_key_weak(set_value, -1), be_const_closure(class_Animate_palette_set_value_closure) },
        { be_const_key_weak(set_range, -1), be_const_closure(class_Animate_palette_set_range_closure) },
        { be_const_key_weak(parse_palette, -1), be_const_closure(class_Animate_palette_parse_palette_closure) },
        { be_const_key_weak(palette, -1), be_const_var(0) },
        { be_const_key_weak(set_duration, -1), be_const_closure(class_Animate_palette_set_duration_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_Animate_palette_init_closure) },
        { be_const_key_weak(ptr_to_palette, 0), be_const_static_closure(class_Animate_palette_ptr_to_palette_closure) },
    })),
    be_str_weak(Animate_palette)
);
// compact class 'Animate_oscillator' ktab size: 21, total: 38 (saved 136 bytes)
static const bvalue be_ktab_class_Animate_oscillator[21] = {
  /* K0   */  be_const_int(0),
  /* K1   */  be_nested_str_weak(duty_cycle),
  /* K2   */  be_nested_str_weak(a),
  /* K3   */  be_nested_str_weak(b),
  /* K4   */  be_const_int(1),
  /* K5   */  be_nested_str_weak(form),
  /* K6   */  be_nested_str_weak(phase),
  /* K7   */  be_nested_str_weak(init),
  /* K8   */  be_nested_str_weak(duration_ms),
  /* K9   */  be_nested_str_weak(value),
  /* K10  */  be_nested_str_weak(tasmota),
  /* K11  */  be_nested_str_weak(millis),
  /* K12  */  be_nested_str_weak(origin),
  /* K13  */  be_nested_str_weak(scale_uint),
  /* K14  */  be_nested_str_weak(beat),
  /* K15  */  be_nested_str_weak(scale_int),
  /* K16  */  be_const_int(2),
  /* K17  */  be_const_int(3),
  /* K18  */  be_nested_str_weak(sine_int),
  /* K19  */  be_nested_str_weak(obj),
  /* K20  */  be_nested_str_weak(mth),
};


extern const bclass be_class_Animate_oscillator;

/********************************************************************
** Solidified function: set_duty_cycle
********************************************************************/
be_local_closure(class_Animate_oscillator_set_duty_cycle,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_oscillator,     /* shared constants */
    be_str_weak(set_duty_cycle),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x14080300,  //  0000  LT	R2	R1	K0
      0x780A0000,  //  0001  JMPF	R2	#0003
      0x58040000,  //  0002  LDCONST	R1	K0
      0x540A0063,  //  0003  LDINT	R2	100
      0x24080202,  //  0004  GT	R2	R1	R2
      0x780A0000,  //  0005  JMPF	R2	#0007
      0x54060063,  //  0006  LDINT	R1	100
      0x90020201,  //  0007  SETMBR	R0	K1	R1
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_a
********************************************************************/
be_local_closure(class_Animate_oscillator_set_a,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_oscillator,     /* shared constants */
    be_str_weak(set_a),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020401,  //  0000  SETMBR	R0	K2	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_b
********************************************************************/
be_local_closure(class_Animate_oscillator_set_b,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_oscillator,     /* shared constants */
    be_str_weak(set_b),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020601,  //  0000  SETMBR	R0	K3	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_form
********************************************************************/
be_local_closure(class_Animate_oscillator_set_form,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_oscillator,     /* shared constants */
    be_str_weak(set_form),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0000,  //  0002  JMPF	R2	#0004
      0x58040004,  //  0003  LDCONST	R1	K4
      0x90020A01,  //  0004  SETMBR	R0	K5	R1
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_phase
********************************************************************/
be_local_closure(class_Animate_oscillator_set_phase,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_oscillator,     /* shared constants */
    be_str_weak(set_phase),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x14080300,  //  0000  LT	R2	R1	K0
      0x780A0000,  //  0001  JMPF	R2	#0003
      0x58040000,  //  0002  LDCONST	R1	K0
      0x540A0063,  //  0003  LDINT	R2	100
      0x24080202,  //  0004  GT	R2	R1	R2
      0x780A0000,  //  0005  JMPF	R2	#0007
      0x54060063,  //  0006  LDINT	R1	100
      0x90020C01,  //  0007  SETMBR	R0	K6	R1
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Animate_oscillator_init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_oscillator,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x60140003,  //  0000  GETGBL	R5	G3
      0x5C180000,  //  0001  MOVE	R6	R0
      0x7C140200,  //  0002  CALL	R5	1
      0x8C140B07,  //  0003  GETMET	R5	R5	K7
      0x7C140200,  //  0004  CALL	R5	1
      0x90020D00,  //  0005  SETMBR	R0	K6	K0
      0x54160031,  //  0006  LDINT	R5	50
      0x90020205,  //  0007  SETMBR	R0	K1	R5
      0x90020401,  //  0008  SETMBR	R0	K2	R1
      0x90020602,  //  0009  SETMBR	R0	K3	R2
      0x90021003,  //  000A  SETMBR	R0	K8	R3
      0x90021201,  //  000B  SETMBR	R0	K9	R1
      0x4C140000,  //  000C  LDNIL	R5
      0x1C140805,  //  000D  EQ	R5	R4	R5
      0x78160000,  //  000E  JMPF	R5	#0010
      0x58100004,  //  000F  LDCONST	R4	K4
      0x90020A04,  //  0010  SETMBR	R0	K5	R4
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: animate
********************************************************************/
be_local_closure(class_Animate_oscillator_animate,   /* name */
  be_nested_proto(
    18,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_oscillator,     /* shared constants */
    be_str_weak(animate),
    &be_const_str_solidified,
    ( &(const binstruction[141]) {  /* code */
      0x88080108,  //  0000  GETMBR	R2	R0	K8
      0x4C0C0000,  //  0001  LDNIL	R3
      0x1C080403,  //  0002  EQ	R2	R2	R3
      0x780A0000,  //  0003  JMPF	R2	#0005
      0x80000400,  //  0004  RET	0
      0x4C080000,  //  0005  LDNIL	R2
      0x1C080202,  //  0006  EQ	R2	R1	R2
      0x780A0003,  //  0007  JMPF	R2	#000C
      0xB80A1400,  //  0008  GETNGBL	R2	K10
      0x8C08050B,  //  0009  GETMET	R2	R2	K11
      0x7C080200,  //  000A  CALL	R2	1
      0x5C040400,  //  000B  MOVE	R1	R2
      0x8808010C,  //  000C  GETMBR	R2	R0	K12
      0x04080202,  //  000D  SUB	R2	R1	R2
      0x140C0500,  //  000E  LT	R3	R2	K0
      0x780E0001,  //  000F  JMPF	R3	#0012
      0x58080000,  //  0010  LDCONST	R2	K0
      0x8804010C,  //  0011  GETMBR	R1	R0	K12
      0x880C0108,  //  0012  GETMBR	R3	R0	K8
      0x4C100000,  //  0013  LDNIL	R4
      0xB8161400,  //  0014  GETNGBL	R5	K10
      0x8C140B0D,  //  0015  GETMET	R5	R5	K13
      0x881C0101,  //  0016  GETMBR	R7	R0	K1
      0x58200000,  //  0017  LDCONST	R8	K0
      0x54260063,  //  0018  LDINT	R9	100
      0x58280000,  //  0019  LDCONST	R10	K0
      0x5C2C0600,  //  001A  MOVE	R11	R3
      0x7C140C00,  //  001B  CALL	R5	6
      0x5C100A00,  //  001C  MOVE	R4	R5
      0x28140403,  //  001D  GE	R5	R2	R3
      0x78160007,  //  001E  JMPF	R5	#0027
      0x0C180403,  //  001F  DIV	R6	R2	R3
      0x08180C03,  //  0020  MUL	R6	R6	R3
      0x8814010C,  //  0021  GETMBR	R5	R0	K12
      0x00140A06,  //  0022  ADD	R5	R5	R6
      0x90021805,  //  0023  SETMBR	R0	K12	R5
      0x10080403,  //  0024  MOD	R2	R2	R3
      0x8C14010E,  //  0025  GETMET	R5	R0	K14
      0x7C140200,  //  0026  CALL	R5	1
      0x88140102,  //  0027  GETMBR	R5	R0	K2
      0x88180103,  //  0028  GETMBR	R6	R0	K3
      0x881C0109,  //  0029  GETMBR	R7	R0	K9
      0x5C200400,  //  002A  MOVE	R8	R2
      0x88240106,  //  002B  GETMBR	R9	R0	K6
      0x24241300,  //  002C  GT	R9	R9	K0
      0x7826000B,  //  002D  JMPF	R9	#003A
      0xB8261400,  //  002E  GETNGBL	R9	K10
      0x8C24130D,  //  002F  GETMET	R9	R9	K13
      0x882C0106,  //  0030  GETMBR	R11	R0	K6
      0x58300000,  //  0031  LDCONST	R12	K0
      0x54360063,  //  0032  LDINT	R13	100
      0x58380000,  //  0033  LDCONST	R14	K0
      0x5C3C0600,  //  0034  MOVE	R15	R3
      0x7C240C00,  //  0035  CALL	R9	6
      0x00201009,  //  0036  ADD	R8	R8	R9
      0x24241003,  //  0037  GT	R9	R8	R3
      0x78260000,  //  0038  JMPF	R9	#003A
      0x04201003,  //  0039  SUB	R8	R8	R3
      0x88240105,  //  003A  GETMBR	R9	R0	K5
      0x1C241304,  //  003B  EQ	R9	R9	K4
      0x78260009,  //  003C  JMPF	R9	#0047
      0xB8261400,  //  003D  GETNGBL	R9	K10
      0x8C24130F,  //  003E  GETMET	R9	R9	K15
      0x5C2C1000,  //  003F  MOVE	R11	R8
      0x58300000,  //  0040  LDCONST	R12	K0
      0x04340704,  //  0041  SUB	R13	R3	K4
      0x5C380A00,  //  0042  MOVE	R14	R5
      0x5C3C0C00,  //  0043  MOVE	R15	R6
      0x7C240C00,  //  0044  CALL	R9	6
      0x5C1C1200,  //  0045  MOVE	R7	R9
      0x7002003B,  //  0046  JMP		#0083
      0x88240105,  //  0047  GETMBR	R9	R0	K5
      0x1C241310,  //  0048  EQ	R9	R9	K16
      0x78260015,  //  0049  JMPF	R9	#0060
      0x14241004,  //  004A  LT	R9	R8	R4
      0x78260009,  //  004B  JMPF	R9	#0056
      0xB8261400,  //  004C  GETNGBL	R9	K10
      0x8C24130F,  //  004D  GETMET	R9	R9	K15
      0x5C2C1000,  //  004E  MOVE	R11	R8
      0x58300000,  //  004F  LDCONST	R12	K0
      0x04340904,  //  0050  SUB	R13	R4	K4
      0x5C380A00,  //  0051  MOVE	R14	R5
      0x5C3C0C00,  //  0052  MOVE	R15	R6
      0x7C240C00,  //  0053  CALL	R9	6
      0x5C1C1200,  //  0054  MOVE	R7	R9
      0x70020008,  //  0055  JMP		#005F
      0xB8261400,  //  0056  GETNGBL	R9	K10
      0x8C24130F,  //  0057  GETMET	R9	R9	K15
      0x5C2C1000,  //  0058  MOVE	R11	R8
      0x5C300800,  //  0059  MOVE	R12	R4
      0x04340704,  //  005A  SUB	R13	R3	K4
      0x5C380C00,  //  005B  MOVE	R14	R6
      0x5C3C0A00,  //  005C  MOVE	R15	R5
      0x7C240C00,  //  005D  CALL	R9	6
      0x5C1C1200,  //  005E  MOVE	R7	R9
      0x70020022,  //  005F  JMP		#0083
      0x88240105,  //  0060  GETMBR	R9	R0	K5
      0x1C241311,  //  0061  EQ	R9	R9	K17
      0x78260005,  //  0062  JMPF	R9	#0069
      0x14241004,  //  0063  LT	R9	R8	R4
      0x78260001,  //  0064  JMPF	R9	#0067
      0x5C1C0A00,  //  0065  MOVE	R7	R5
      0x70020000,  //  0066  JMP		#0068
      0x5C1C0C00,  //  0067  MOVE	R7	R6
      0x70020019,  //  0068  JMP		#0083
      0x88240105,  //  0069  GETMBR	R9	R0	K5
      0x542A0003,  //  006A  LDINT	R10	4
      0x1C24120A,  //  006B  EQ	R9	R9	R10
      0x78260015,  //  006C  JMPF	R9	#0083
      0xB8261400,  //  006D  GETNGBL	R9	K10
      0x8C24130F,  //  006E  GETMET	R9	R9	K15
      0x5C2C1000,  //  006F  MOVE	R11	R8
      0x58300000,  //  0070  LDCONST	R12	K0
      0x04340704,  //  0071  SUB	R13	R3	K4
      0x58380000,  //  0072  LDCONST	R14	K0
      0x543E7FFE,  //  0073  LDINT	R15	32767
      0x7C240C00,  //  0074  CALL	R9	6
      0xB82A1400,  //  0075  GETNGBL	R10	K10
      0x8C281512,  //  0076  GETMET	R10	R10	K18
      0x54321FFF,  //  0077  LDINT	R12	8192
      0x0430120C,  //  0078  SUB	R12	R9	R12
      0x7C280400,  //  0079  CALL	R10	2
      0xB82E1400,  //  007A  GETNGBL	R11	K10
      0x8C2C170F,  //  007B  GETMET	R11	R11	K15
      0x5C341400,  //  007C  MOVE	R13	R10
      0x5439EFFF,  //  007D  LDINT	R14	-4096
      0x543E0FFF,  //  007E  LDINT	R15	4096
      0x5C400A00,  //  007F  MOVE	R16	R5
      0x5C440C00,  //  0080  MOVE	R17	R6
      0x7C2C0C00,  //  0081  CALL	R11	6
      0x5C1C1600,  //  0082  MOVE	R7	R11
      0x90021207,  //  0083  SETMBR	R0	K9	R7
      0x88240113,  //  0084  GETMBR	R9	R0	K19
      0x88280114,  //  0085  GETMBR	R10	R0	K20
      0x78260004,  //  0086  JMPF	R9	#008C
      0x782A0003,  //  0087  JMPF	R10	#008C
      0x5C2C1400,  //  0088  MOVE	R11	R10
      0x5C301200,  //  0089  MOVE	R12	R9
      0x5C340E00,  //  008A  MOVE	R13	R7
      0x7C2C0400,  //  008B  CALL	R11	2
      0x80040E00,  //  008C  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Animate_oscillator
********************************************************************/
extern const bclass be_class_Animate_animator;
be_local_class(Animate_oscillator,
    6,
    &be_class_Animate_animator,
    be_nested_map(13,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(animate, -1), be_const_closure(class_Animate_oscillator_animate_closure) },
        { be_const_key_weak(a, -1), be_const_var(2) },
        { be_const_key_weak(init, 11), be_const_closure(class_Animate_oscillator_init_closure) },
        { be_const_key_weak(duty_cycle, 10), be_const_var(1) },
        { be_const_key_weak(b, -1), be_const_var(3) },
        { be_const_key_weak(value, -1), be_const_var(5) },
        { be_const_key_weak(set_duty_cycle, 2), be_const_closure(class_Animate_oscillator_set_duty_cycle_closure) },
        { be_const_key_weak(set_a, -1), be_const_closure(class_Animate_oscillator_set_a_closure) },
        { be_const_key_weak(set_b, -1), be_const_closure(class_Animate_oscillator_set_b_closure) },
        { be_const_key_weak(set_form, -1), be_const_closure(class_Animate_oscillator_set_form_closure) },
        { be_const_key_weak(phase, -1), be_const_var(0) },
        { be_const_key_weak(form, -1), be_const_var(4) },
        { be_const_key_weak(set_phase, 0), be_const_closure(class_Animate_oscillator_set_phase_closure) },
    })),
    be_str_weak(Animate_oscillator)
);
/********************************************************************/
/* End of solidification */
