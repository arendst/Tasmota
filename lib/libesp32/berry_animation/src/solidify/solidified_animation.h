/* Solidification of animation.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

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
    /* K3   */  be_const_int(3),
    }),
    be_str_weak(square),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x90060503,  //  0004  SETMBR	R1	K2	K3
      0x80040200,  //  0005  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: encode_constraints
********************************************************************/
be_local_closure(encode_constraints,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        13,                          /* nstack */
        1,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        1,                          /* has sup protos */
        ( &(const struct bproto*[ 3]) {
          be_nested_proto(
            5,                          /* nstack */
            1,                          /* argc */
            0,                          /* varg */
            0,                          /* has upvals */
            NULL,                       /* no upvals */
            0,                          /* has sup protos */
            NULL,                       /* no sub protos */
            1,                          /* has constants */
            ( &(const bvalue[ 8]) {     /* constants */
            /* K0   */  be_nested_str_weak(bool),
            /* K1   */  be_nested_str_weak(string),
            /* K2   */  be_const_int(3),
            /* K3   */  be_nested_str_weak(instance),
            /* K4   */  be_nested_str_weak(int),
            /* K5   */  be_const_int(0),
            /* K6   */  be_const_int(1),
            /* K7   */  be_const_int(2),
            }),
            be_str_weak(get_type_code),
            &be_const_str_solidified,
            ( &(const binstruction[50]) {  /* code */
              0x60040004,  //  0000  GETGBL	R1	G4
              0x5C080000,  //  0001  MOVE	R2	R0
              0x7C040200,  //  0002  CALL	R1	1
              0x4C080000,  //  0003  LDNIL	R2
              0x1C080002,  //  0004  EQ	R2	R0	R2
              0x780A0002,  //  0005  JMPF	R2	#0009
              0x540A0005,  //  0006  LDINT	R2	6
              0x80040400,  //  0007  RET	1	R2
              0x70020027,  //  0008  JMP		#0031
              0x1C080300,  //  0009  EQ	R2	R1	K0
              0x780A0002,  //  000A  JMPF	R2	#000E
              0x540A0004,  //  000B  LDINT	R2	5
              0x80040400,  //  000C  RET	1	R2
              0x70020022,  //  000D  JMP		#0031
              0x1C080301,  //  000E  EQ	R2	R1	K1
              0x780A0001,  //  000F  JMPF	R2	#0012
              0x80060400,  //  0010  RET	1	K2
              0x7002001E,  //  0011  JMP		#0031
              0x1C080303,  //  0012  EQ	R2	R1	K3
              0x780A0007,  //  0013  JMPF	R2	#001C
              0x6008000F,  //  0014  GETGBL	R2	G15
              0x5C0C0000,  //  0015  MOVE	R3	R0
              0x60100015,  //  0016  GETGBL	R4	G21
              0x7C080400,  //  0017  CALL	R2	2
              0x780A0002,  //  0018  JMPF	R2	#001C
              0x540A0003,  //  0019  LDINT	R2	4
              0x80040400,  //  001A  RET	1	R2
              0x70020014,  //  001B  JMP		#0031
              0x1C080304,  //  001C  EQ	R2	R1	K4
              0x780A0011,  //  001D  JMPF	R2	#0030
              0x5409FF7F,  //  001E  LDINT	R2	-128
              0x28080002,  //  001F  GE	R2	R0	R2
              0x780A0004,  //  0020  JMPF	R2	#0026
              0x540A007E,  //  0021  LDINT	R2	127
              0x18080002,  //  0022  LE	R2	R0	R2
              0x780A0001,  //  0023  JMPF	R2	#0026
              0x80060A00,  //  0024  RET	1	K5
              0x70020008,  //  0025  JMP		#002F
              0x54097FFF,  //  0026  LDINT	R2	-32768
              0x28080002,  //  0027  GE	R2	R0	R2
              0x780A0004,  //  0028  JMPF	R2	#002E
              0x540A7FFE,  //  0029  LDINT	R2	32767
              0x18080002,  //  002A  LE	R2	R0	R2
              0x780A0001,  //  002B  JMPF	R2	#002E
              0x80060C00,  //  002C  RET	1	K6
              0x70020000,  //  002D  JMP		#002F
              0x80060E00,  //  002E  RET	1	K7
              0x70020000,  //  002F  JMP		#0031
              0x80060E00,  //  0030  RET	1	K7
              0x80000000,  //  0031  RET	0
            })
          ),
          be_nested_proto(
            8,                          /* nstack */
            2,                          /* argc */
            0,                          /* varg */
            1,                          /* has upvals */
            ( &(const bupvaldesc[ 1]) {  /* upvals */
              be_local_const_upval(1, 1),
            }),
            0,                          /* has sup protos */
            NULL,                       /* no sub protos */
            1,                          /* has constants */
            ( &(const bvalue[ 6]) {     /* constants */
            /* K0   */  be_nested_str_weak(add),
            /* K1   */  be_const_int(1),
            /* K2   */  be_const_int(0),
            /* K3   */  be_const_int(2),
            /* K4   */  be_const_int(3),
            /* K5   */  be_nested_str_weak(fromstring),
            }),
            be_str_weak(encode_value_with_type),
            &be_const_str_solidified,
            ( &(const binstruction[72]) {  /* code */
              0x68080000,  //  0000  GETUPV	R2	U0
              0x5C0C0000,  //  0001  MOVE	R3	R0
              0x7C080200,  //  0002  CALL	R2	1
              0x8C0C0300,  //  0003  GETMET	R3	R1	K0
              0x5C140400,  //  0004  MOVE	R5	R2
              0x58180001,  //  0005  LDCONST	R6	K1
              0x7C0C0600,  //  0006  CALL	R3	3
              0x540E0005,  //  0007  LDINT	R3	6
              0x1C0C0403,  //  0008  EQ	R3	R2	R3
              0x780E0001,  //  0009  JMPF	R3	#000C
              0x80000600,  //  000A  RET	0
              0x7002003A,  //  000B  JMP		#0047
              0x540E0004,  //  000C  LDINT	R3	5
              0x1C0C0403,  //  000D  EQ	R3	R2	R3
              0x780E0007,  //  000E  JMPF	R3	#0017
              0x8C0C0300,  //  000F  GETMET	R3	R1	K0
              0x78020001,  //  0010  JMPF	R0	#0013
              0x58140001,  //  0011  LDCONST	R5	K1
              0x70020000,  //  0012  JMP		#0014
              0x58140002,  //  0013  LDCONST	R5	K2
              0x58180001,  //  0014  LDCONST	R6	K1
              0x7C0C0600,  //  0015  CALL	R3	3
              0x7002002F,  //  0016  JMP		#0047
              0x1C0C0502,  //  0017  EQ	R3	R2	K2
              0x780E0005,  //  0018  JMPF	R3	#001F
              0x8C0C0300,  //  0019  GETMET	R3	R1	K0
              0x541600FE,  //  001A  LDINT	R5	255
              0x2C140005,  //  001B  AND	R5	R0	R5
              0x58180001,  //  001C  LDCONST	R6	K1
              0x7C0C0600,  //  001D  CALL	R3	3
              0x70020027,  //  001E  JMP		#0047
              0x1C0C0501,  //  001F  EQ	R3	R2	K1
              0x780E0005,  //  0020  JMPF	R3	#0027
              0x8C0C0300,  //  0021  GETMET	R3	R1	K0
              0x5416FFFE,  //  0022  LDINT	R5	65535
              0x2C140005,  //  0023  AND	R5	R0	R5
              0x58180003,  //  0024  LDCONST	R6	K3
              0x7C0C0600,  //  0025  CALL	R3	3
              0x7002001F,  //  0026  JMP		#0047
              0x1C0C0503,  //  0027  EQ	R3	R2	K3
              0x780E0004,  //  0028  JMPF	R3	#002E
              0x8C0C0300,  //  0029  GETMET	R3	R1	K0
              0x5C140000,  //  002A  MOVE	R5	R0
              0x541A0003,  //  002B  LDINT	R6	4
              0x7C0C0600,  //  002C  CALL	R3	3
              0x70020018,  //  002D  JMP		#0047
              0x1C0C0504,  //  002E  EQ	R3	R2	K4
              0x780E000C,  //  002F  JMPF	R3	#003D
              0x600C0015,  //  0030  GETGBL	R3	G21
              0x7C0C0000,  //  0031  CALL	R3	0
              0x8C0C0705,  //  0032  GETMET	R3	R3	K5
              0x5C140000,  //  0033  MOVE	R5	R0
              0x7C0C0400,  //  0034  CALL	R3	2
              0x8C100300,  //  0035  GETMET	R4	R1	K0
              0x6018000C,  //  0036  GETGBL	R6	G12
              0x5C1C0600,  //  0037  MOVE	R7	R3
              0x7C180200,  //  0038  CALL	R6	1
              0x581C0001,  //  0039  LDCONST	R7	K1
              0x7C100600,  //  003A  CALL	R4	3
              0x40100203,  //  003B  CONNECT	R4	R1	R3
              0x70020009,  //  003C  JMP		#0047
              0x540E0003,  //  003D  LDINT	R3	4
              0x1C0C0403,  //  003E  EQ	R3	R2	R3
              0x780E0006,  //  003F  JMPF	R3	#0047
              0x8C0C0300,  //  0040  GETMET	R3	R1	K0
              0x6014000C,  //  0041  GETGBL	R5	G12
              0x5C180000,  //  0042  MOVE	R6	R0
              0x7C140200,  //  0043  CALL	R5	1
              0x58180003,  //  0044  LDCONST	R6	K3
              0x7C0C0600,  //  0045  CALL	R3	3
              0x400C0200,  //  0046  CONNECT	R3	R1	R0
              0x80000000,  //  0047  RET	0
            })
          ),
          be_nested_proto(
            2,                          /* nstack */
            1,                          /* argc */
            0,                          /* varg */
            0,                          /* has upvals */
            NULL,                       /* no upvals */
            0,                          /* has sup protos */
            NULL,                       /* no sub protos */
            1,                          /* has constants */
            ( &(const bvalue[11]) {     /* constants */
            /* K0   */  be_nested_str_weak(int),
            /* K1   */  be_const_int(0),
            /* K2   */  be_nested_str_weak(string),
            /* K3   */  be_const_int(1),
            /* K4   */  be_nested_str_weak(bytes),
            /* K5   */  be_const_int(2),
            /* K6   */  be_nested_str_weak(bool),
            /* K7   */  be_const_int(3),
            /* K8   */  be_nested_str_weak(any),
            /* K9   */  be_nested_str_weak(instance),
            /* K10  */  be_nested_str_weak(function),
            }),
            be_str_weak(get_explicit_type_code),
            &be_const_str_solidified,
            ( &(const binstruction[32]) {  /* code */
              0x1C040100,  //  0000  EQ	R1	R0	K0
              0x78060001,  //  0001  JMPF	R1	#0004
              0x80060200,  //  0002  RET	1	K1
              0x70020019,  //  0003  JMP		#001E
              0x1C040102,  //  0004  EQ	R1	R0	K2
              0x78060001,  //  0005  JMPF	R1	#0008
              0x80060600,  //  0006  RET	1	K3
              0x70020015,  //  0007  JMP		#001E
              0x1C040104,  //  0008  EQ	R1	R0	K4
              0x78060001,  //  0009  JMPF	R1	#000C
              0x80060A00,  //  000A  RET	1	K5
              0x70020011,  //  000B  JMP		#001E
              0x1C040106,  //  000C  EQ	R1	R0	K6
              0x78060001,  //  000D  JMPF	R1	#0010
              0x80060E00,  //  000E  RET	1	K7
              0x7002000D,  //  000F  JMP		#001E
              0x1C040108,  //  0010  EQ	R1	R0	K8
              0x78060002,  //  0011  JMPF	R1	#0015
              0x54060003,  //  0012  LDINT	R1	4
              0x80040200,  //  0013  RET	1	R1
              0x70020008,  //  0014  JMP		#001E
              0x1C040109,  //  0015  EQ	R1	R0	K9
              0x78060002,  //  0016  JMPF	R1	#001A
              0x54060004,  //  0017  LDINT	R1	5
              0x80040200,  //  0018  RET	1	R1
              0x70020003,  //  0019  JMP		#001E
              0x1C04010A,  //  001A  EQ	R1	R0	K10
              0x78060001,  //  001B  JMPF	R1	#001E
              0x54060005,  //  001C  LDINT	R1	6
              0x80040200,  //  001D  RET	1	R1
              0x54060003,  //  001E  LDINT	R1	4
              0x80040200,  //  001F  RET	1	R1
            })
          ),
        }),
        1,                          /* has constants */
        ( &(const bvalue[14]) {     /* constants */
        /* K0   */  be_const_int(0),
        /* K1   */  be_nested_str_weak(resize),
        /* K2   */  be_const_int(1),
        /* K3   */  be_nested_str_weak(contains),
        /* K4   */  be_nested_str_weak(type),
        /* K5   */  be_nested_str_weak(min),
        /* K6   */  be_nested_str_weak(max),
        /* K7   */  be_const_int(2),
        /* K8   */  be_nested_str_weak(default),
        /* K9   */  be_nested_str_weak(add),
        /* K10  */  be_nested_str_weak(enum),
        /* K11  */  be_nested_str_weak(stop_iteration),
        /* K12  */  be_nested_str_weak(nillable),
        /* K13  */  be_nested_str_weak(set),
        }),
        be_str_weak(encode_single_constraint),
        &be_const_str_solidified,
        ( &(const binstruction[97]) {  /* code */
          0x84040000,  //  0000  CLOSURE	R1	P0
          0x84080001,  //  0001  CLOSURE	R2	P1
          0x580C0000,  //  0002  LDCONST	R3	K0
          0x60100015,  //  0003  GETGBL	R4	G21
          0x7C100000,  //  0004  CALL	R4	0
          0x8C140901,  //  0005  GETMET	R5	R4	K1
          0x581C0002,  //  0006  LDCONST	R7	K2
          0x7C140400,  //  0007  CALL	R5	2
          0x84140002,  //  0008  CLOSURE	R5	P2
          0x4C180000,  //  0009  LDNIL	R6
          0x8C1C0103,  //  000A  GETMET	R7	R0	K3
          0x58240004,  //  000B  LDCONST	R9	K4
          0x7C1C0400,  //  000C  CALL	R7	2
          0x781E0003,  //  000D  JMPF	R7	#0012
          0x5C1C0A00,  //  000E  MOVE	R7	R5
          0x94200104,  //  000F  GETIDX	R8	R0	K4
          0x7C1C0200,  //  0010  CALL	R7	1
          0x5C180E00,  //  0011  MOVE	R6	R7
          0x8C1C0103,  //  0012  GETMET	R7	R0	K3
          0x58240005,  //  0013  LDCONST	R9	K5
          0x7C1C0400,  //  0014  CALL	R7	2
          0x781E0004,  //  0015  JMPF	R7	#001B
          0x300C0702,  //  0016  OR	R3	R3	K2
          0x5C1C0400,  //  0017  MOVE	R7	R2
          0x94200105,  //  0018  GETIDX	R8	R0	K5
          0x5C240800,  //  0019  MOVE	R9	R4
          0x7C1C0400,  //  001A  CALL	R7	2
          0x8C1C0103,  //  001B  GETMET	R7	R0	K3
          0x58240006,  //  001C  LDCONST	R9	K6
          0x7C1C0400,  //  001D  CALL	R7	2
          0x781E0004,  //  001E  JMPF	R7	#0024
          0x300C0707,  //  001F  OR	R3	R3	K7
          0x5C1C0400,  //  0020  MOVE	R7	R2
          0x94200106,  //  0021  GETIDX	R8	R0	K6
          0x5C240800,  //  0022  MOVE	R9	R4
          0x7C1C0400,  //  0023  CALL	R7	2
          0x8C1C0103,  //  0024  GETMET	R7	R0	K3
          0x58240008,  //  0025  LDCONST	R9	K8
          0x7C1C0400,  //  0026  CALL	R7	2
          0x781E0005,  //  0027  JMPF	R7	#002E
          0x541E0003,  //  0028  LDINT	R7	4
          0x300C0607,  //  0029  OR	R3	R3	R7
          0x5C1C0400,  //  002A  MOVE	R7	R2
          0x94200108,  //  002B  GETIDX	R8	R0	K8
          0x5C240800,  //  002C  MOVE	R9	R4
          0x7C1C0400,  //  002D  CALL	R7	2
          0x4C1C0000,  //  002E  LDNIL	R7
          0x201C0C07,  //  002F  NE	R7	R6	R7
          0x781E0005,  //  0030  JMPF	R7	#0037
          0x541E0007,  //  0031  LDINT	R7	8
          0x300C0607,  //  0032  OR	R3	R3	R7
          0x8C1C0909,  //  0033  GETMET	R7	R4	K9
          0x5C240C00,  //  0034  MOVE	R9	R6
          0x58280002,  //  0035  LDCONST	R10	K2
          0x7C1C0600,  //  0036  CALL	R7	3
          0x8C1C0103,  //  0037  GETMET	R7	R0	K3
          0x5824000A,  //  0038  LDCONST	R9	K10
          0x7C1C0400,  //  0039  CALL	R7	2
          0x781E0016,  //  003A  JMPF	R7	#0052
          0x541E000F,  //  003B  LDINT	R7	16
          0x300C0607,  //  003C  OR	R3	R3	R7
          0x941C010A,  //  003D  GETIDX	R7	R0	K10
          0x8C200909,  //  003E  GETMET	R8	R4	K9
          0x6028000C,  //  003F  GETGBL	R10	G12
          0x5C2C0E00,  //  0040  MOVE	R11	R7
          0x7C280200,  //  0041  CALL	R10	1
          0x582C0002,  //  0042  LDCONST	R11	K2
          0x7C200600,  //  0043  CALL	R8	3
          0x60200010,  //  0044  GETGBL	R8	G16
          0x5C240E00,  //  0045  MOVE	R9	R7
          0x7C200200,  //  0046  CALL	R8	1
          0xA8020006,  //  0047  EXBLK	0	#004F
          0x5C241000,  //  0048  MOVE	R9	R8
          0x7C240000,  //  0049  CALL	R9	0
          0x5C280400,  //  004A  MOVE	R10	R2
          0x5C2C1200,  //  004B  MOVE	R11	R9
          0x5C300800,  //  004C  MOVE	R12	R4
          0x7C280400,  //  004D  CALL	R10	2
          0x7001FFF8,  //  004E  JMP		#0048
          0x5820000B,  //  004F  LDCONST	R8	K11
          0xAC200200,  //  0050  CATCH	R8	1	0
          0xB0080000,  //  0051  RAISE	2	R0	R0
          0x8C1C0103,  //  0052  GETMET	R7	R0	K3
          0x5824000C,  //  0053  LDCONST	R9	K12
          0x7C1C0400,  //  0054  CALL	R7	2
          0x781E0003,  //  0055  JMPF	R7	#005A
          0x941C010C,  //  0056  GETIDX	R7	R0	K12
          0x781E0001,  //  0057  JMPF	R7	#005A
          0x541E001F,  //  0058  LDINT	R7	32
          0x300C0607,  //  0059  OR	R3	R3	R7
          0x8C1C090D,  //  005A  GETMET	R7	R4	K13
          0x58240000,  //  005B  LDCONST	R9	K0
          0x5C280600,  //  005C  MOVE	R10	R3
          0x582C0002,  //  005D  LDCONST	R11	K2
          0x7C1C0800,  //  005E  CALL	R7	4
          0xA0000000,  //  005F  CLOSE	R0
          0x80040800,  //  0060  RET	1	R4
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(keys),
    /* K1   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(encode_constraints),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x84040000,  //  0000  CLOSURE	R1	P0
      0x60080013,  //  0001  GETGBL	R2	G19
      0x7C080000,  //  0002  CALL	R2	0
      0x600C0010,  //  0003  GETGBL	R3	G16
      0x8C100100,  //  0004  GETMET	R4	R0	K0
      0x7C100200,  //  0005  CALL	R4	1
      0x7C0C0200,  //  0006  CALL	R3	1
      0xA8020006,  //  0007  EXBLK	0	#000F
      0x5C100600,  //  0008  MOVE	R4	R3
      0x7C100000,  //  0009  CALL	R4	0
      0x5C140200,  //  000A  MOVE	R5	R1
      0x94180004,  //  000B  GETIDX	R6	R0	R4
      0x7C140200,  //  000C  CALL	R5	1
      0x98080805,  //  000D  SETIDX	R2	R4	R5
      0x7001FFF8,  //  000E  JMP		#0008
      0x580C0001,  //  000F  LDCONST	R3	K1
      0xAC0C0200,  //  0010  CATCH	R3	1	0
      0xB0080000,  //  0011  RAISE	2	R0	R0
      0x80040400,  //  0012  RET	1	R2
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


extern const bclass be_class_strip_length;

/********************************************************************
** Solidified function: produce_value
********************************************************************/
be_local_closure(class_strip_length_produce_value,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(engine),
    /* K1   */  be_nested_str_weak(strip_length),
    }),
    be_str_weak(produce_value),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x880C0701,  //  0001  GETMBR	R3	R3	K1
      0x80040600,  //  0002  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: strip_length
********************************************************************/
extern const bclass be_class_parameterized_object;
be_local_class(strip_length,
    0,
    &be_class_parameterized_object,
    be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(VALUE_PROVIDER, -1), be_const_bool(1) },
        { be_const_key_weak(produce_value, -1), be_const_closure(class_strip_length_produce_value_closure) },
    })),
    be_str_weak(strip_length)
);

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

// compact class 'frame_buffer' ktab size: 20, total: 40 (saved 160 bytes)
static const bvalue be_ktab_class_frame_buffer[20] = {
  /* K0   */  be_nested_str_weak(pixels),
  /* K1   */  be_nested_str_weak(tohex),
  /* K2   */  be_nested_str_weak(int),
  /* K3   */  be_const_int(0),
  /* K4   */  be_nested_str_weak(value_error),
  /* K5   */  be_nested_str_weak(width_X20must_X20be_X20positive),
  /* K6   */  be_nested_str_weak(width),
  /* K7   */  be_nested_str_weak(resize),
  /* K8   */  be_nested_str_weak(clear),
  /* K9   */  be_nested_str_weak(instance),
  /* K10  */  be_nested_str_weak(copy),
  /* K11  */  be_nested_str_weak(argument_X20must_X20be_X20either_X20int_X20or_X20instance),
  /* K12  */  be_nested_str_weak(animation),
  /* K13  */  be_nested_str_weak(frame_buffer),
  /* K14  */  be_nested_str_weak(index_error),
  /* K15  */  be_nested_str_weak(pixel_X20index_X20out_X20of_X20range),
  /* K16  */  be_nested_str_weak(get),
  /* K17  */  be_nested_str_weak(set_pixel_color),
  /* K18  */  be_nested_str_weak(set),
  /* K19  */  be_nested_str_weak(get_pixel_color),
};


extern const bclass be_class_frame_buffer;

/********************************************************************
** Solidified function: tohex
********************************************************************/
be_local_closure(class_frame_buffer_tohex,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_frame_buffer,     /* shared constants */
    be_str_weak(tohex),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_frame_buffer_init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_frame_buffer,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[36]) {  /* code */
      0x60080004,  //  0000  GETGBL	R2	G4
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x1C080502,  //  0003  EQ	R2	R2	K2
      0x780A0010,  //  0004  JMPF	R2	#0016
      0x5C080200,  //  0005  MOVE	R2	R1
      0x180C0503,  //  0006  LE	R3	R2	K3
      0x780E0000,  //  0007  JMPF	R3	#0009
      0xB0060905,  //  0008  RAISE	1	K4	K5
      0x90020C02,  //  0009  SETMBR	R0	K6	R2
      0x600C0015,  //  000A  GETGBL	R3	G21
      0x54120003,  //  000B  LDINT	R4	4
      0x08100404,  //  000C  MUL	R4	R2	R4
      0x7C0C0200,  //  000D  CALL	R3	1
      0x8C100707,  //  000E  GETMET	R4	R3	K7
      0x541A0003,  //  000F  LDINT	R6	4
      0x08180406,  //  0010  MUL	R6	R2	R6
      0x7C100400,  //  0011  CALL	R4	2
      0x90020003,  //  0012  SETMBR	R0	K0	R3
      0x8C100108,  //  0013  GETMET	R4	R0	K8
      0x7C100200,  //  0014  CALL	R4	1
      0x7002000C,  //  0015  JMP		#0023
      0x60080004,  //  0016  GETGBL	R2	G4
      0x5C0C0200,  //  0017  MOVE	R3	R1
      0x7C080200,  //  0018  CALL	R2	1
      0x1C080509,  //  0019  EQ	R2	R2	K9
      0x780A0006,  //  001A  JMPF	R2	#0022
      0x88080306,  //  001B  GETMBR	R2	R1	K6
      0x90020C02,  //  001C  SETMBR	R0	K6	R2
      0x88080300,  //  001D  GETMBR	R2	R1	K0
      0x8C08050A,  //  001E  GETMET	R2	R2	K10
      0x7C080200,  //  001F  CALL	R2	1
      0x90020002,  //  0020  SETMBR	R0	K0	R2
      0x70020000,  //  0021  JMP		#0023
      0xB006090B,  //  0022  RAISE	1	K4	K11
      0x80000000,  //  0023  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: copy
********************************************************************/
be_local_closure(class_frame_buffer_copy,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_frame_buffer,     /* shared constants */
    be_str_weak(copy),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0xB8061800,  //  0000  GETNGBL	R1	K12
      0x8C04030D,  //  0001  GETMET	R1	R1	K13
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
be_local_closure(class_frame_buffer_get_pixel_color,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_frame_buffer,     /* shared constants */
    be_str_weak(get_pixel_color),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x14080303,  //  0000  LT	R2	R1	K3
      0x740A0002,  //  0001  JMPT	R2	#0005
      0x88080106,  //  0002  GETMBR	R2	R0	K6
      0x28080202,  //  0003  GE	R2	R1	R2
      0x780A0000,  //  0004  JMPF	R2	#0006
      0xB0061D0F,  //  0005  RAISE	1	K14	K15
      0x88080100,  //  0006  GETMBR	R2	R0	K0
      0x8C080510,  //  0007  GETMET	R2	R2	K16
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
** Solidified function: resize
********************************************************************/
be_local_closure(class_frame_buffer_resize,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_frame_buffer,     /* shared constants */
    be_str_weak(resize),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x18080303,  //  0000  LE	R2	R1	K3
      0x780A0000,  //  0001  JMPF	R2	#0003
      0xB0060905,  //  0002  RAISE	1	K4	K5
      0x88080106,  //  0003  GETMBR	R2	R0	K6
      0x1C080202,  //  0004  EQ	R2	R1	R2
      0x780A0000,  //  0005  JMPF	R2	#0007
      0x80000400,  //  0006  RET	0
      0x90020C01,  //  0007  SETMBR	R0	K6	R1
      0x88080100,  //  0008  GETMBR	R2	R0	K0
      0x8C080507,  //  0009  GETMET	R2	R2	K7
      0x88100106,  //  000A  GETMBR	R4	R0	K6
      0x54160003,  //  000B  LDINT	R5	4
      0x08100805,  //  000C  MUL	R4	R4	R5
      0x7C080400,  //  000D  CALL	R2	2
      0x8C080108,  //  000E  GETMET	R2	R0	K8
      0x7C080200,  //  000F  CALL	R2	1
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: setitem
********************************************************************/
be_local_closure(class_frame_buffer_setitem,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_frame_buffer,     /* shared constants */
    be_str_weak(setitem),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C0C0111,  //  0000  GETMET	R3	R0	K17
      0x5C140200,  //  0001  MOVE	R5	R1
      0x5C180400,  //  0002  MOVE	R6	R2
      0x7C0C0600,  //  0003  CALL	R3	3
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_pixel_color
********************************************************************/
be_local_closure(class_frame_buffer_set_pixel_color,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_frame_buffer,     /* shared constants */
    be_str_weak(set_pixel_color),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x140C0303,  //  0000  LT	R3	R1	K3
      0x740E0002,  //  0001  JMPT	R3	#0005
      0x880C0106,  //  0002  GETMBR	R3	R0	K6
      0x280C0203,  //  0003  GE	R3	R1	R3
      0x780E0000,  //  0004  JMPF	R3	#0006
      0xB0061D0F,  //  0005  RAISE	1	K14	K15
      0x880C0100,  //  0006  GETMBR	R3	R0	K0
      0x8C0C0712,  //  0007  GETMET	R3	R3	K18
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
** Solidified function: clear
********************************************************************/
be_local_closure(class_frame_buffer_clear,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_frame_buffer,     /* shared constants */
    be_str_weak(clear),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040308,  //  0001  GETMET	R1	R1	K8
      0x7C040200,  //  0002  CALL	R1	1
      0x6004000C,  //  0003  GETGBL	R1	G12
      0x88080100,  //  0004  GETMBR	R2	R0	K0
      0x7C040200,  //  0005  CALL	R1	1
      0x88080106,  //  0006  GETMBR	R2	R0	K6
      0x540E0003,  //  0007  LDINT	R3	4
      0x08080403,  //  0008  MUL	R2	R2	R3
      0x20040202,  //  0009  NE	R1	R1	R2
      0x78060005,  //  000A  JMPF	R1	#0011
      0x88040100,  //  000B  GETMBR	R1	R0	K0
      0x8C040307,  //  000C  GETMET	R1	R1	K7
      0x880C0106,  //  000D  GETMBR	R3	R0	K6
      0x54120003,  //  000E  LDINT	R4	4
      0x080C0604,  //  000F  MUL	R3	R3	R4
      0x7C040400,  //  0010  CALL	R1	2
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: item
********************************************************************/
be_local_closure(class_frame_buffer_item,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_frame_buffer,     /* shared constants */
    be_str_weak(item),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C080113,  //  0000  GETMET	R2	R0	K19
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x80040400,  //  0003  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: frame_buffer
********************************************************************/
extern const bclass be_class_FrameBufferNtv;
be_local_class(frame_buffer,
    2,
    &be_class_FrameBufferNtv,
    be_nested_map(11,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(tohex, 1), be_const_closure(class_frame_buffer_tohex_closure) },
        { be_const_key_weak(width, -1), be_const_var(1) },
        { be_const_key_weak(clear, -1), be_const_closure(class_frame_buffer_clear_closure) },
        { be_const_key_weak(copy, -1), be_const_closure(class_frame_buffer_copy_closure) },
        { be_const_key_weak(pixels, -1), be_const_var(0) },
        { be_const_key_weak(get_pixel_color, -1), be_const_closure(class_frame_buffer_get_pixel_color_closure) },
        { be_const_key_weak(resize, -1), be_const_closure(class_frame_buffer_resize_closure) },
        { be_const_key_weak(setitem, -1), be_const_closure(class_frame_buffer_setitem_closure) },
        { be_const_key_weak(set_pixel_color, -1), be_const_closure(class_frame_buffer_set_pixel_color_closure) },
        { be_const_key_weak(init, 2), be_const_closure(class_frame_buffer_init_closure) },
        { be_const_key_weak(item, -1), be_const_closure(class_frame_buffer_item_closure) },
    })),
    be_str_weak(frame_buffer)
);

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

// compact class 'breathe_color' ktab size: 21, total: 25 (saved 32 bytes)
static const bvalue be_ktab_class_breathe_color[21] = {
  /* K0   */  be_nested_str_weak(init),
  /* K1   */  be_nested_str_weak(_oscillator),
  /* K2   */  be_nested_str_weak(animation),
  /* K3   */  be_nested_str_weak(oscillator_value),
  /* K4   */  be_nested_str_weak(form),
  /* K5   */  be_nested_str_weak(min_value),
  /* K6   */  be_const_int(0),
  /* K7   */  be_nested_str_weak(max_value),
  /* K8   */  be_nested_str_weak(duration),
  /* K9   */  be_nested_str_weak(add),
  /* K10  */  be_nested_str_weak(produce_value),
  /* K11  */  be_nested_str_weak(curve_factor),
  /* K12  */  be_const_int(1),
  /* K13  */  be_nested_str_weak(tasmota),
  /* K14  */  be_nested_str_weak(scale_uint),
  /* K15  */  be_nested_str_weak(min_brightness),
  /* K16  */  be_nested_str_weak(max_brightness),
  /* K17  */  be_nested_str_weak(color),
  /* K18  */  be_nested_str_weak(apply_brightness),
  /* K19  */  be_nested_str_weak(on_param_changed),
  /* K20  */  be_nested_str_weak(period),
};


extern const bclass be_class_breathe_color;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_breathe_color_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_breathe_color,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0xB80A0400,  //  0006  GETNGBL	R2	K2
      0x8C080503,  //  0007  GETMET	R2	R2	K3
      0x5C100200,  //  0008  MOVE	R4	R1
      0x7C080400,  //  0009  CALL	R2	2
      0x90020202,  //  000A  SETMBR	R0	K1	R2
      0x88080101,  //  000B  GETMBR	R2	R0	K1
      0x540E0003,  //  000C  LDINT	R3	4
      0x900A0803,  //  000D  SETMBR	R2	K4	R3
      0x88080101,  //  000E  GETMBR	R2	R0	K1
      0x900A0B06,  //  000F  SETMBR	R2	K5	K6
      0x88080101,  //  0010  GETMBR	R2	R0	K1
      0x540E00FE,  //  0011  LDINT	R3	255
      0x900A0E03,  //  0012  SETMBR	R2	K7	R3
      0x88080101,  //  0013  GETMBR	R2	R0	K1
      0x540E0BB7,  //  0014  LDINT	R3	3000
      0x900A1003,  //  0015  SETMBR	R2	K8	R3
      0x8C080309,  //  0016  GETMET	R2	R1	K9
      0x88100101,  //  0017  GETMBR	R4	R0	K1
      0x7C080400,  //  0018  CALL	R2	2
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: produce_value
********************************************************************/
be_local_closure(class_breathe_color_produce_value,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_breathe_color,     /* shared constants */
    be_str_weak(produce_value),
    &be_const_str_solidified,
    ( &(const binstruction[49]) {  /* code */
      0x880C0101,  //  0000  GETMBR	R3	R0	K1
      0x8C0C070A,  //  0001  GETMET	R3	R3	K10
      0x5C140200,  //  0002  MOVE	R5	R1
      0x5C180400,  //  0003  MOVE	R6	R2
      0x7C0C0600,  //  0004  CALL	R3	3
      0x8810010B,  //  0005  GETMBR	R4	R0	K11
      0x5C140600,  //  0006  MOVE	R5	R3
      0x2418090C,  //  0007  GT	R6	R4	K12
      0x781A0019,  //  0008  JMPF	R6	#0023
      0xB81A1A00,  //  0009  GETNGBL	R6	K13
      0x8C180D0E,  //  000A  GETMET	R6	R6	K14
      0x5C200600,  //  000B  MOVE	R8	R3
      0x58240006,  //  000C  LDCONST	R9	K6
      0x542A00FE,  //  000D  LDINT	R10	255
      0x582C0006,  //  000E  LDCONST	R11	K6
      0x54321FFF,  //  000F  LDINT	R12	8192
      0x7C180C00,  //  0010  CALL	R6	6
      0x5C1C0800,  //  0011  MOVE	R7	R4
      0x24200F0C,  //  0012  GT	R8	R7	K12
      0x78220005,  //  0013  JMPF	R8	#001A
      0x08200C06,  //  0014  MUL	R8	R6	R6
      0x54261FFF,  //  0015  LDINT	R9	8192
      0x0C201009,  //  0016  DIV	R8	R8	R9
      0x5C181000,  //  0017  MOVE	R6	R8
      0x041C0F0C,  //  0018  SUB	R7	R7	K12
      0x7001FFF7,  //  0019  JMP		#0012
      0xB8221A00,  //  001A  GETNGBL	R8	K13
      0x8C20110E,  //  001B  GETMET	R8	R8	K14
      0x5C280C00,  //  001C  MOVE	R10	R6
      0x582C0006,  //  001D  LDCONST	R11	K6
      0x54321FFF,  //  001E  LDINT	R12	8192
      0x58340006,  //  001F  LDCONST	R13	K6
      0x543A00FE,  //  0020  LDINT	R14	255
      0x7C200C00,  //  0021  CALL	R8	6
      0x5C141000,  //  0022  MOVE	R5	R8
      0xB81A1A00,  //  0023  GETNGBL	R6	K13
      0x8C180D0E,  //  0024  GETMET	R6	R6	K14
      0x5C200A00,  //  0025  MOVE	R8	R5
      0x58240006,  //  0026  LDCONST	R9	K6
      0x542A00FE,  //  0027  LDINT	R10	255
      0x882C010F,  //  0028  GETMBR	R11	R0	K15
      0x88300110,  //  0029  GETMBR	R12	R0	K16
      0x7C180C00,  //  002A  CALL	R6	6
      0x881C0111,  //  002B  GETMBR	R7	R0	K17
      0x8C200112,  //  002C  GETMET	R8	R0	K18
      0x5C280E00,  //  002D  MOVE	R10	R7
      0x5C2C0C00,  //  002E  MOVE	R11	R6
      0x7C200600,  //  002F  CALL	R8	3
      0x80041000,  //  0030  RET	1	R8
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: on_param_changed
********************************************************************/
be_local_closure(class_breathe_color_on_param_changed,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_breathe_color,     /* shared constants */
    be_str_weak(on_param_changed),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0713,  //  0003  GETMET	R3	R3	K19
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x1C0C0314,  //  0007  EQ	R3	R1	K20
      0x780E0001,  //  0008  JMPF	R3	#000B
      0x880C0101,  //  0009  GETMBR	R3	R0	K1
      0x900E1002,  //  000A  SETMBR	R3	K8	R2
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: breathe_color
********************************************************************/
extern const bclass be_class_color_provider;
be_local_class(breathe_color,
    1,
    &be_class_color_provider,
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, -1), be_const_closure(class_breathe_color_init_closure) },
        { be_const_key_weak(on_param_changed, -1), be_const_closure(class_breathe_color_on_param_changed_closure) },
        { be_const_key_weak(_oscillator, 1), be_const_var(0) },
        { be_const_key_weak(PARAMS, 4), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(period, -1), be_const_bytes_instance(05000101B80B) },
        { be_const_key_weak(max_brightness, -1), be_const_bytes_instance(07000001FF0001FF00) },
        { be_const_key_weak(curve_factor, -1), be_const_bytes_instance(07000100050002) },
        { be_const_key_weak(min_brightness, -1), be_const_bytes_instance(07000001FF000000) },
    }))    ) } )) },
        { be_const_key_weak(produce_value, -1), be_const_closure(class_breathe_color_produce_value_closure) },
    })),
    be_str_weak(breathe_color)
);
extern const bclass be_class_color_provider;
// compact class 'color_provider' ktab size: 11, total: 13 (saved 16 bytes)
static const bvalue be_ktab_class_color_provider[11] = {
  /* K0   */  be_const_class(be_class_color_provider),
  /* K1   */  be_nested_str_weak(tasmota),
  /* K2   */  be_nested_str_weak(scale_uint),
  /* K3   */  be_const_int(0),
  /* K4   */  be_nested_str_weak(color),
  /* K5   */  be_nested_str_weak(brightness),
  /* K6   */  be_nested_str_weak(apply_brightness),
  /* K7   */  be_nested_str_weak(init),
  /* K8   */  be_nested_str_weak(_color_lut),
  /* K9   */  be_nested_str_weak(_lut_dirty),
  /* K10  */  be_nested_str_weak(produce_value),
};


extern const bclass be_class_color_provider;

/********************************************************************
** Solidified function: apply_brightness
********************************************************************/
be_local_closure(class_color_provider_apply_brightness,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    2,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_color_provider,     /* shared constants */
    be_str_weak(apply_brightness),
    &be_const_str_solidified,
    ( &(const binstruction[56]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0x540E00FE,  //  0001  LDINT	R3	255
      0x1C0C0203,  //  0002  EQ	R3	R1	R3
      0x780E0000,  //  0003  JMPF	R3	#0005
      0x80040000,  //  0004  RET	1	R0
      0x540E0017,  //  0005  LDINT	R3	24
      0x3C0C0003,  //  0006  SHR	R3	R0	R3
      0x541200FE,  //  0007  LDINT	R4	255
      0x2C0C0604,  //  0008  AND	R3	R3	R4
      0x5412000F,  //  0009  LDINT	R4	16
      0x3C100004,  //  000A  SHR	R4	R0	R4
      0x541600FE,  //  000B  LDINT	R5	255
      0x2C100805,  //  000C  AND	R4	R4	R5
      0x54160007,  //  000D  LDINT	R5	8
      0x3C140005,  //  000E  SHR	R5	R0	R5
      0x541A00FE,  //  000F  LDINT	R6	255
      0x2C140A06,  //  0010  AND	R5	R5	R6
      0x541A00FE,  //  0011  LDINT	R6	255
      0x2C180006,  //  0012  AND	R6	R0	R6
      0xB81E0200,  //  0013  GETNGBL	R7	K1
      0x8C1C0F02,  //  0014  GETMET	R7	R7	K2
      0x5C240800,  //  0015  MOVE	R9	R4
      0x58280003,  //  0016  LDCONST	R10	K3
      0x542E00FE,  //  0017  LDINT	R11	255
      0x58300003,  //  0018  LDCONST	R12	K3
      0x5C340200,  //  0019  MOVE	R13	R1
      0x7C1C0C00,  //  001A  CALL	R7	6
      0x5C100E00,  //  001B  MOVE	R4	R7
      0xB81E0200,  //  001C  GETNGBL	R7	K1
      0x8C1C0F02,  //  001D  GETMET	R7	R7	K2
      0x5C240A00,  //  001E  MOVE	R9	R5
      0x58280003,  //  001F  LDCONST	R10	K3
      0x542E00FE,  //  0020  LDINT	R11	255
      0x58300003,  //  0021  LDCONST	R12	K3
      0x5C340200,  //  0022  MOVE	R13	R1
      0x7C1C0C00,  //  0023  CALL	R7	6
      0x5C140E00,  //  0024  MOVE	R5	R7
      0xB81E0200,  //  0025  GETNGBL	R7	K1
      0x8C1C0F02,  //  0026  GETMET	R7	R7	K2
      0x5C240C00,  //  0027  MOVE	R9	R6
      0x58280003,  //  0028  LDCONST	R10	K3
      0x542E00FE,  //  0029  LDINT	R11	255
      0x58300003,  //  002A  LDCONST	R12	K3
      0x5C340200,  //  002B  MOVE	R13	R1
      0x7C1C0C00,  //  002C  CALL	R7	6
      0x5C180E00,  //  002D  MOVE	R6	R7
      0x541E0017,  //  002E  LDINT	R7	24
      0x381C0607,  //  002F  SHL	R7	R3	R7
      0x5422000F,  //  0030  LDINT	R8	16
      0x38200808,  //  0031  SHL	R8	R4	R8
      0x301C0E08,  //  0032  OR	R7	R7	R8
      0x54220007,  //  0033  LDINT	R8	8
      0x38200A08,  //  0034  SHL	R8	R5	R8
      0x301C0E08,  //  0035  OR	R7	R7	R8
      0x301C0E06,  //  0036  OR	R7	R7	R6
      0x80040E00,  //  0037  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: produce_value
********************************************************************/
be_local_closure(class_color_provider_produce_value,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_color_provider,     /* shared constants */
    be_str_weak(produce_value),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x880C0104,  //  0000  GETMBR	R3	R0	K4
      0x88100105,  //  0001  GETMBR	R4	R0	K5
      0x541600FE,  //  0002  LDINT	R5	255
      0x20140805,  //  0003  NE	R5	R4	R5
      0x78160004,  //  0004  JMPF	R5	#000A
      0x8C140106,  //  0005  GETMET	R5	R0	K6
      0x5C1C0600,  //  0006  MOVE	R7	R3
      0x5C200800,  //  0007  MOVE	R8	R4
      0x7C140600,  //  0008  CALL	R5	3
      0x80040A00,  //  0009  RET	1	R5
      0x80040600,  //  000A  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_color_provider_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_color_provider,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080507,  //  0003  GETMET	R2	R2	K7
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x4C080000,  //  0006  LDNIL	R2
      0x90021002,  //  0007  SETMBR	R0	K8	R2
      0x50080200,  //  0008  LDBOOL	R2	1	0
      0x90021202,  //  0009  SETMBR	R0	K9	R2
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_lut
********************************************************************/
be_local_closure(class_color_provider_get_lut,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_color_provider,     /* shared constants */
    be_str_weak(get_lut),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040108,  //  0000  GETMBR	R1	R0	K8
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_color_for_value
********************************************************************/
be_local_closure(class_color_provider_get_color_for_value,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_color_provider,     /* shared constants */
    be_str_weak(get_color_for_value),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C0C010A,  //  0000  GETMET	R3	R0	K10
      0x58140004,  //  0001  LDCONST	R5	K4
      0x5C180400,  //  0002  MOVE	R6	R2
      0x7C0C0600,  //  0003  CALL	R3	3
      0x80040600,  //  0004  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: color_provider
********************************************************************/
extern const bclass be_class_parameterized_object;
be_local_class(color_provider,
    2,
    &be_class_parameterized_object,
    be_nested_map(10,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(_lut_dirty, -1), be_const_var(1) },
        { be_const_key_weak(apply_brightness, -1), be_const_static_closure(class_color_provider_apply_brightness_closure) },
        { be_const_key_weak(LUT_FACTOR, 7), be_const_int(1) },
        { be_const_key_weak(produce_value, 6), be_const_closure(class_color_provider_produce_value_closure) },
        { be_const_key_weak(get_color_for_value, -1), be_const_closure(class_color_provider_get_color_for_value_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_color_provider_init_closure) },
        { be_const_key_weak(PARAMS, 8), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(color, 1), be_const_bytes_instance(0400FF) },
        { be_const_key_weak(brightness, -1), be_const_bytes_instance(07000001FF0001FF00) },
    }))    ) } )) },
        { be_const_key_weak(VALUE_PROVIDER, -1), be_const_bool(1) },
        { be_const_key_weak(get_lut, -1), be_const_closure(class_color_provider_get_lut_closure) },
        { be_const_key_weak(_color_lut, 4), be_const_var(0) },
    })),
    be_str_weak(color_provider)
);
// compact class 'rich_palette_color' ktab size: 43, total: 108 (saved 520 bytes)
static const bvalue be_ktab_class_rich_palette_color[43] = {
  /* K0   */  be_nested_str_weak(period),
  /* K1   */  be_nested_str_weak(_get_palette_bytes),
  /* K2   */  be_nested_str_weak(_slots),
  /* K3   */  be_const_int(0),
  /* K4   */  be_nested_str_weak(_slots_arr),
  /* K5   */  be_nested_str_weak(_parse_palette),
  /* K6   */  be_const_int(1),
  /* K7   */  be_nested_str_weak(_value_arr),
  /* K8   */  be_nested_str_weak(resize),
  /* K9   */  be_nested_str_weak(get),
  /* K10  */  be_nested_str_weak(tasmota),
  /* K11  */  be_nested_str_weak(scale_int),
  /* K12  */  be_const_int(-16777216),
  /* K13  */  be_nested_str_weak(_lut_dirty),
  /* K14  */  be_nested_str_weak(_color_lut),
  /* K15  */  be_nested_str_weak(_rebuild_color_lut),
  /* K16  */  be_nested_str_weak(_brightness),
  /* K17  */  be_nested_str_weak(member),
  /* K18  */  be_nested_str_weak(brightness),
  /* K19  */  be_nested_str_weak(background_X3Alinear_X2Dgradient_X28to_X20right_X2C_X20_X23000000_X29_X3B),
  /* K20  */  be_nested_str_weak(background_X3Alinear_X2Dgradient_X28to_X20right),
  /* K21  */  be_nested_str_weak(_X2C_X23_X2502X_X2502X_X2502X_X20_X25_X2E1f_X25_X25),
  /* K22  */  be_const_real_hex(0x41200000),
  /* K23  */  be_nested_str_weak(_X29_X3B),
  /* K24  */  be_nested_str_weak(colors),
  /* K25  */  be_nested_str_weak(_DEFAULT_PALETTE),
  /* K26  */  be_nested_str_weak(_recompute_palette),
  /* K27  */  be_nested_str_weak(LUT_FACTOR),
  /* K28  */  be_nested_str_weak(_get_color_for_value_uncached),
  /* K29  */  be_nested_str_weak(set),
  /* K30  */  be_const_int(2),
  /* K31  */  be_nested_str_weak(scale_uint),
  /* K32  */  be_nested_str_weak(transition_type),
  /* K33  */  be_nested_str_weak(sine_int),
  /* K34  */  be_nested_str_weak(start),
  /* K35  */  be_nested_str_weak(_fix_time_ms),
  /* K36  */  be_nested_str_weak(start_time),
  /* K37  */  be_nested_str_weak(_interpolate),
  /* K38  */  be_nested_str_weak(init),
  /* K39  */  be_nested_str_weak(animation),
  /* K40  */  be_nested_str_weak(PALETTE_RAINBOW),
  /* K41  */  be_nested_str_weak(add),
  /* K42  */  be_nested_str_weak(on_param_changed),
};


extern const bclass be_class_rich_palette_color;

/********************************************************************
** Solidified function: _recompute_palette
********************************************************************/
be_local_closure(class_rich_palette_color__recompute_palette,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_rich_palette_color,     /* shared constants */
    be_str_weak(_recompute_palette),
    &be_const_str_solidified,
    ( &(const binstruction[36]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C080101,  //  0001  GETMET	R2	R0	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x600C000C,  //  0003  GETGBL	R3	G12
      0x5C100400,  //  0004  MOVE	R4	R2
      0x7C0C0200,  //  0005  CALL	R3	1
      0x54120003,  //  0006  LDINT	R4	4
      0x0C0C0604,  //  0007  DIV	R3	R3	R4
      0x90020403,  //  0008  SETMBR	R0	K2	R3
      0x240C0303,  //  0009  GT	R3	R1	K3
      0x780E0008,  //  000A  JMPF	R3	#0014
      0x4C0C0000,  //  000B  LDNIL	R3
      0x200C0403,  //  000C  NE	R3	R2	R3
      0x780E0005,  //  000D  JMPF	R3	#0014
      0x8C0C0105,  //  000E  GETMET	R3	R0	K5
      0x58140003,  //  000F  LDCONST	R5	K3
      0x04180306,  //  0010  SUB	R6	R1	K6
      0x7C0C0600,  //  0011  CALL	R3	3
      0x90020803,  //  0012  SETMBR	R0	K4	R3
      0x70020001,  //  0013  JMP		#0016
      0x4C0C0000,  //  0014  LDNIL	R3
      0x90020803,  //  0015  SETMBR	R0	K4	R3
      0x8C0C0101,  //  0016  GETMET	R3	R0	K1
      0x7C0C0200,  //  0017  CALL	R3	1
      0x4C100000,  //  0018  LDNIL	R4
      0x200C0604,  //  0019  NE	R3	R3	R4
      0x780E0005,  //  001A  JMPF	R3	#0021
      0x8C0C0105,  //  001B  GETMET	R3	R0	K5
      0x58140003,  //  001C  LDCONST	R5	K3
      0x541A00FE,  //  001D  LDINT	R6	255
      0x7C0C0600,  //  001E  CALL	R3	3
      0x90020E03,  //  001F  SETMBR	R0	K7	R3
      0x70020001,  //  0020  JMP		#0023
      0x4C0C0000,  //  0021  LDNIL	R3
      0x90020E03,  //  0022  SETMBR	R0	K7	R3
      0x80040000,  //  0023  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _parse_palette
********************************************************************/
be_local_closure(class_rich_palette_color__parse_palette,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_rich_palette_color,     /* shared constants */
    be_str_weak(_parse_palette),
    &be_const_str_solidified,
    ( &(const binstruction[69]) {  /* code */
      0x8C0C0101,  //  0000  GETMET	R3	R0	K1
      0x7C0C0200,  //  0001  CALL	R3	1
      0x60100012,  //  0002  GETGBL	R4	G18
      0x7C100000,  //  0003  CALL	R4	0
      0x88140102,  //  0004  GETMBR	R5	R0	K2
      0x8C180908,  //  0005  GETMET	R6	R4	K8
      0x5C200A00,  //  0006  MOVE	R8	R5
      0x7C180400,  //  0007  CALL	R6	2
      0x8C180709,  //  0008  GETMET	R6	R3	K9
      0x58200003,  //  0009  LDCONST	R8	K3
      0x58240006,  //  000A  LDCONST	R9	K6
      0x7C180600,  //  000B  CALL	R6	3
      0x20180D03,  //  000C  NE	R6	R6	K3
      0x781A0022,  //  000D  JMPF	R6	#0031
      0x58180003,  //  000E  LDCONST	R6	K3
      0x581C0003,  //  000F  LDCONST	R7	K3
      0x04200B06,  //  0010  SUB	R8	R5	K6
      0x14200E08,  //  0011  LT	R8	R7	R8
      0x78220007,  //  0012  JMPF	R8	#001B
      0x8C200709,  //  0013  GETMET	R8	R3	K9
      0x542A0003,  //  0014  LDINT	R10	4
      0x08280E0A,  //  0015  MUL	R10	R7	R10
      0x582C0006,  //  0016  LDCONST	R11	K6
      0x7C200600,  //  0017  CALL	R8	3
      0x00180C08,  //  0018  ADD	R6	R6	R8
      0x001C0F06,  //  0019  ADD	R7	R7	K6
      0x7001FFF4,  //  001A  JMP		#0010
      0x58200003,  //  001B  LDCONST	R8	K3
      0x581C0003,  //  001C  LDCONST	R7	K3
      0x14240E05,  //  001D  LT	R9	R7	R5
      0x78260010,  //  001E  JMPF	R9	#0030
      0xB8261400,  //  001F  GETNGBL	R9	K10
      0x8C24130B,  //  0020  GETMET	R9	R9	K11
      0x5C2C1000,  //  0021  MOVE	R11	R8
      0x58300003,  //  0022  LDCONST	R12	K3
      0x5C340C00,  //  0023  MOVE	R13	R6
      0x5C380200,  //  0024  MOVE	R14	R1
      0x5C3C0400,  //  0025  MOVE	R15	R2
      0x7C240C00,  //  0026  CALL	R9	6
      0x98100E09,  //  0027  SETIDX	R4	R7	R9
      0x8C240709,  //  0028  GETMET	R9	R3	K9
      0x542E0003,  //  0029  LDINT	R11	4
      0x082C0E0B,  //  002A  MUL	R11	R7	R11
      0x58300006,  //  002B  LDCONST	R12	K6
      0x7C240600,  //  002C  CALL	R9	3
      0x00201009,  //  002D  ADD	R8	R8	R9
      0x001C0F06,  //  002E  ADD	R7	R7	K6
      0x7001FFEC,  //  002F  JMP		#001D
      0x70020012,  //  0030  JMP		#0044
      0x58180003,  //  0031  LDCONST	R6	K3
      0x141C0C05,  //  0032  LT	R7	R6	R5
      0x781E000F,  //  0033  JMPF	R7	#0044
      0x8C1C0709,  //  0034  GETMET	R7	R3	K9
      0x54260003,  //  0035  LDINT	R9	4
      0x08240C09,  //  0036  MUL	R9	R6	R9
      0x58280006,  //  0037  LDCONST	R10	K6
      0x7C1C0600,  //  0038  CALL	R7	3
      0xB8221400,  //  0039  GETNGBL	R8	K10
      0x8C20110B,  //  003A  GETMET	R8	R8	K11
      0x5C280E00,  //  003B  MOVE	R10	R7
      0x582C0003,  //  003C  LDCONST	R11	K3
      0x543200FE,  //  003D  LDINT	R12	255
      0x5C340200,  //  003E  MOVE	R13	R1
      0x5C380400,  //  003F  MOVE	R14	R2
      0x7C200C00,  //  0040  CALL	R8	6
      0x98100C08,  //  0041  SETIDX	R4	R6	R8
      0x00180D06,  //  0042  ADD	R6	R6	K6
      0x7001FFED,  //  0043  JMP		#0032
      0x80040800,  //  0044  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _get_color_at_index
********************************************************************/
be_local_closure(class_rich_palette_color__get_color_at_index,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_rich_palette_color,     /* shared constants */
    be_str_weak(_get_color_at_index),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x14080303,  //  0000  LT	R2	R1	K3
      0x740A0002,  //  0001  JMPT	R2	#0005
      0x88080102,  //  0002  GETMBR	R2	R0	K2
      0x28080202,  //  0003  GE	R2	R1	R2
      0x780A0001,  //  0004  JMPF	R2	#0007
      0x5409FFFE,  //  0005  LDINT	R2	-1
      0x80040400,  //  0006  RET	1	R2
      0x8C080101,  //  0007  GETMET	R2	R0	K1
      0x7C080200,  //  0008  CALL	R2	1
      0x8C0C0509,  //  0009  GETMET	R3	R2	K9
      0x54160003,  //  000A  LDINT	R5	4
      0x08140205,  //  000B  MUL	R5	R1	R5
      0x5419FFFB,  //  000C  LDINT	R6	-4
      0x7C0C0600,  //  000D  CALL	R3	3
      0x300C070C,  //  000E  OR	R3	R3	K12
      0x80040600,  //  000F  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update
********************************************************************/
be_local_closure(class_rich_palette_color_update,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_rich_palette_color,     /* shared constants */
    be_str_weak(update),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x8808010D,  //  0000  GETMBR	R2	R0	K13
      0x740A0003,  //  0001  JMPT	R2	#0006
      0x8808010E,  //  0002  GETMBR	R2	R0	K14
      0x4C0C0000,  //  0003  LDNIL	R3
      0x1C080403,  //  0004  EQ	R2	R2	R3
      0x780A0001,  //  0005  JMPF	R2	#0008
      0x8C08010F,  //  0006  GETMET	R2	R0	K15
      0x7C080200,  //  0007  CALL	R2	1
      0x8C080111,  //  0008  GETMET	R2	R0	K17
      0x58100012,  //  0009  LDCONST	R4	K18
      0x7C080400,  //  000A  CALL	R2	2
      0x90022002,  //  000B  SETMBR	R0	K16	R2
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: to_css_gradient
********************************************************************/
be_local_closure(class_rich_palette_color_to_css_gradient,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_rich_palette_color,     /* shared constants */
    be_str_weak(to_css_gradient),
    &be_const_str_solidified,
    ( &(const binstruction[47]) {  /* code */
      0x8C040101,  //  0000  GETMET	R1	R0	K1
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x1C080202,  //  0003  EQ	R2	R1	R2
      0x780A0000,  //  0004  JMPF	R2	#0006
      0x80062600,  //  0005  RET	1	K19
      0x8C080105,  //  0006  GETMET	R2	R0	K5
      0x58100003,  //  0007  LDCONST	R4	K3
      0x541603E7,  //  0008  LDINT	R5	1000
      0x7C080600,  //  0009  CALL	R2	3
      0x580C0014,  //  000A  LDCONST	R3	K20
      0x58100003,  //  000B  LDCONST	R4	K3
      0x6014000C,  //  000C  GETGBL	R5	G12
      0x5C180400,  //  000D  MOVE	R6	R2
      0x7C140200,  //  000E  CALL	R5	1
      0x14140805,  //  000F  LT	R5	R4	R5
      0x7816001B,  //  0010  JMPF	R5	#002D
      0x94140404,  //  0011  GETIDX	R5	R2	R4
      0x8C180309,  //  0012  GETMET	R6	R1	K9
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
      0x582C0015,  //  0024  LDCONST	R11	K21
      0x5C300E00,  //  0025  MOVE	R12	R7
      0x5C341000,  //  0026  MOVE	R13	R8
      0x5C381200,  //  0027  MOVE	R14	R9
      0x0C3C0B16,  //  0028  DIV	R15	R5	K22
      0x7C280A00,  //  0029  CALL	R10	5
      0x000C060A,  //  002A  ADD	R3	R3	R10
      0x00100906,  //  002B  ADD	R4	R4	K6
      0x7001FFDE,  //  002C  JMP		#000C
      0x000C0717,  //  002D  ADD	R3	R3	K23
      0x80040600,  //  002E  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _get_palette_bytes
********************************************************************/
be_local_closure(class_rich_palette_color__get_palette_bytes,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_rich_palette_color,     /* shared constants */
    be_str_weak(_get_palette_bytes),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88040118,  //  0000  GETMBR	R1	R0	K24
      0x4C080000,  //  0001  LDNIL	R2
      0x20080202,  //  0002  NE	R2	R1	R2
      0x780A0001,  //  0003  JMPF	R2	#0006
      0x5C080200,  //  0004  MOVE	R2	R1
      0x70020000,  //  0005  JMP		#0007
      0x88080119,  //  0006  GETMBR	R2	R0	K25
      0x80040400,  //  0007  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _rebuild_color_lut
********************************************************************/
be_local_closure(class_rich_palette_color__rebuild_color_lut,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_rich_palette_color,     /* shared constants */
    be_str_weak(_rebuild_color_lut),
    &be_const_str_solidified,
    ( &(const binstruction[51]) {  /* code */
      0x88040107,  //  0000  GETMBR	R1	R0	K7
      0x4C080000,  //  0001  LDNIL	R2
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060001,  //  0003  JMPF	R1	#0006
      0x8C04011A,  //  0004  GETMET	R1	R0	K26
      0x7C040200,  //  0005  CALL	R1	1
      0x8804010E,  //  0006  GETMBR	R1	R0	K14
      0x4C080000,  //  0007  LDNIL	R2
      0x1C040202,  //  0008  EQ	R1	R1	R2
      0x78060008,  //  0009  JMPF	R1	#0013
      0x60040015,  //  000A  GETGBL	R1	G21
      0x7C040000,  //  000B  CALL	R1	0
      0x90021C01,  //  000C  SETMBR	R0	K14	R1
      0x8804010E,  //  000D  GETMBR	R1	R0	K14
      0x8C040308,  //  000E  GETMET	R1	R1	K8
      0x540E0080,  //  000F  LDINT	R3	129
      0x54120003,  //  0010  LDINT	R4	4
      0x080C0604,  //  0011  MUL	R3	R3	R4
      0x7C040400,  //  0012  CALL	R1	2
      0x8804011B,  //  0013  GETMBR	R1	R0	K27
      0x58080003,  //  0014  LDCONST	R2	K3
      0x540E00FF,  //  0015  LDINT	R3	256
      0x3C0C0601,  //  0016  SHR	R3	R3	R1
      0x14100403,  //  0017  LT	R4	R2	R3
      0x7812000C,  //  0018  JMPF	R4	#0026
      0x38100401,  //  0019  SHL	R4	R2	R1
      0x8C14011C,  //  001A  GETMET	R5	R0	K28
      0x5C1C0800,  //  001B  MOVE	R7	R4
      0x58200003,  //  001C  LDCONST	R8	K3
      0x7C140600,  //  001D  CALL	R5	3
      0x8818010E,  //  001E  GETMBR	R6	R0	K14
      0x8C180D1D,  //  001F  GETMET	R6	R6	K29
      0x3820051E,  //  0020  SHL	R8	R2	K30
      0x5C240A00,  //  0021  MOVE	R9	R5
      0x542A0003,  //  0022  LDINT	R10	4
      0x7C180800,  //  0023  CALL	R6	4
      0x00080506,  //  0024  ADD	R2	R2	K6
      0x7001FFF0,  //  0025  JMP		#0017
      0x8C10011C,  //  0026  GETMET	R4	R0	K28
      0x541A00FE,  //  0027  LDINT	R6	255
      0x581C0003,  //  0028  LDCONST	R7	K3
      0x7C100600,  //  0029  CALL	R4	3
      0x8814010E,  //  002A  GETMBR	R5	R0	K14
      0x8C140B1D,  //  002B  GETMET	R5	R5	K29
      0x381C071E,  //  002C  SHL	R7	R3	K30
      0x5C200800,  //  002D  MOVE	R8	R4
      0x54260003,  //  002E  LDINT	R9	4
      0x7C140800,  //  002F  CALL	R5	4
      0x50140000,  //  0030  LDBOOL	R5	0	0
      0x90021A05,  //  0031  SETMBR	R0	K13	R5
      0x80000000,  //  0032  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_color_for_value
********************************************************************/
be_local_closure(class_rich_palette_color_get_color_for_value,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_rich_palette_color,     /* shared constants */
    be_str_weak(get_color_for_value),
    &be_const_str_solidified,
    ( &(const binstruction[65]) {  /* code */
      0x880C011B,  //  0000  GETMBR	R3	R0	K27
      0x3C0C0203,  //  0001  SHR	R3	R1	R3
      0x541200FE,  //  0002  LDINT	R4	255
      0x28100204,  //  0003  GE	R4	R1	R4
      0x78120000,  //  0004  JMPF	R4	#0006
      0x540E007F,  //  0005  LDINT	R3	128
      0x8810010E,  //  0006  GETMBR	R4	R0	K14
      0x8C100909,  //  0007  GETMET	R4	R4	K9
      0x541A0003,  //  0008  LDINT	R6	4
      0x08180606,  //  0009  MUL	R6	R3	R6
      0x541E0003,  //  000A  LDINT	R7	4
      0x7C100600,  //  000B  CALL	R4	3
      0x88140110,  //  000C  GETMBR	R5	R0	K16
      0x541A00FE,  //  000D  LDINT	R6	255
      0x20180A06,  //  000E  NE	R6	R5	R6
      0x781A002F,  //  000F  JMPF	R6	#0040
      0x541A000F,  //  0010  LDINT	R6	16
      0x3C180806,  //  0011  SHR	R6	R4	R6
      0x541E00FE,  //  0012  LDINT	R7	255
      0x2C180C07,  //  0013  AND	R6	R6	R7
      0x541E0007,  //  0014  LDINT	R7	8
      0x3C1C0807,  //  0015  SHR	R7	R4	R7
      0x542200FE,  //  0016  LDINT	R8	255
      0x2C1C0E08,  //  0017  AND	R7	R7	R8
      0x542200FE,  //  0018  LDINT	R8	255
      0x2C200808,  //  0019  AND	R8	R4	R8
      0xB8261400,  //  001A  GETNGBL	R9	K10
      0x8C24131F,  //  001B  GETMET	R9	R9	K31
      0x5C2C0C00,  //  001C  MOVE	R11	R6
      0x58300003,  //  001D  LDCONST	R12	K3
      0x543600FE,  //  001E  LDINT	R13	255
      0x58380003,  //  001F  LDCONST	R14	K3
      0x5C3C0A00,  //  0020  MOVE	R15	R5
      0x7C240C00,  //  0021  CALL	R9	6
      0x5C181200,  //  0022  MOVE	R6	R9
      0xB8261400,  //  0023  GETNGBL	R9	K10
      0x8C24131F,  //  0024  GETMET	R9	R9	K31
      0x5C2C0E00,  //  0025  MOVE	R11	R7
      0x58300003,  //  0026  LDCONST	R12	K3
      0x543600FE,  //  0027  LDINT	R13	255
      0x58380003,  //  0028  LDCONST	R14	K3
      0x5C3C0A00,  //  0029  MOVE	R15	R5
      0x7C240C00,  //  002A  CALL	R9	6
      0x5C1C1200,  //  002B  MOVE	R7	R9
      0xB8261400,  //  002C  GETNGBL	R9	K10
      0x8C24131F,  //  002D  GETMET	R9	R9	K31
      0x5C2C1000,  //  002E  MOVE	R11	R8
      0x58300003,  //  002F  LDCONST	R12	K3
      0x543600FE,  //  0030  LDINT	R13	255
      0x58380003,  //  0031  LDCONST	R14	K3
      0x5C3C0A00,  //  0032  MOVE	R15	R5
      0x7C240C00,  //  0033  CALL	R9	6
      0x5C201200,  //  0034  MOVE	R8	R9
      0x542600FE,  //  0035  LDINT	R9	255
      0x542A0017,  //  0036  LDINT	R10	24
      0x3824120A,  //  0037  SHL	R9	R9	R10
      0x542A000F,  //  0038  LDINT	R10	16
      0x38280C0A,  //  0039  SHL	R10	R6	R10
      0x3024120A,  //  003A  OR	R9	R9	R10
      0x542A0007,  //  003B  LDINT	R10	8
      0x38280E0A,  //  003C  SHL	R10	R7	R10
      0x3024120A,  //  003D  OR	R9	R9	R10
      0x30241208,  //  003E  OR	R9	R9	R8
      0x5C101200,  //  003F  MOVE	R4	R9
      0x80040800,  //  0040  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _interpolate
********************************************************************/
be_local_closure(class_rich_palette_color__interpolate,   /* name */
  be_nested_proto(
    18,                          /* nstack */
    6,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_rich_palette_color,     /* shared constants */
    be_str_weak(_interpolate),
    &be_const_str_solidified,
    ( &(const binstruction[53]) {  /* code */
      0x88180120,  //  0000  GETMBR	R6	R0	K32
      0x541E0004,  //  0001  LDINT	R7	5
      0x1C1C0C07,  //  0002  EQ	R7	R6	R7
      0x781E0026,  //  0003  JMPF	R7	#002B
      0xB81E1400,  //  0004  GETNGBL	R7	K10
      0x8C1C0F1F,  //  0005  GETMET	R7	R7	K31
      0x5C240200,  //  0006  MOVE	R9	R1
      0x5C280400,  //  0007  MOVE	R10	R2
      0x5C2C0600,  //  0008  MOVE	R11	R3
      0x58300003,  //  0009  LDCONST	R12	K3
      0x543600FE,  //  000A  LDINT	R13	255
      0x7C1C0C00,  //  000B  CALL	R7	6
      0xB8221400,  //  000C  GETNGBL	R8	K10
      0x8C20111F,  //  000D  GETMET	R8	R8	K31
      0x5C280E00,  //  000E  MOVE	R10	R7
      0x582C0003,  //  000F  LDCONST	R11	K3
      0x543200FE,  //  0010  LDINT	R12	255
      0x54363FFF,  //  0011  LDINT	R13	16384
      0x58380003,  //  0012  LDCONST	R14	K3
      0x7C200C00,  //  0013  CALL	R8	6
      0xB8261400,  //  0014  GETNGBL	R9	K10
      0x8C241321,  //  0015  GETMET	R9	R9	K33
      0x542E1FFF,  //  0016  LDINT	R11	8192
      0x002C100B,  //  0017  ADD	R11	R8	R11
      0x7C240400,  //  0018  CALL	R9	2
      0xB82A1400,  //  0019  GETNGBL	R10	K10
      0x8C28150B,  //  001A  GETMET	R10	R10	K11
      0x5C301200,  //  001B  MOVE	R12	R9
      0x5435EFFF,  //  001C  LDINT	R13	-4096
      0x543A0FFF,  //  001D  LDINT	R14	4096
      0x583C0003,  //  001E  LDCONST	R15	K3
      0x544200FE,  //  001F  LDINT	R16	255
      0x7C280C00,  //  0020  CALL	R10	6
      0xB82E1400,  //  0021  GETNGBL	R11	K10
      0x8C2C170B,  //  0022  GETMET	R11	R11	K11
      0x5C341400,  //  0023  MOVE	R13	R10
      0x58380003,  //  0024  LDCONST	R14	K3
      0x543E00FE,  //  0025  LDINT	R15	255
      0x5C400800,  //  0026  MOVE	R16	R4
      0x5C440A00,  //  0027  MOVE	R17	R5
      0x7C2C0C00,  //  0028  CALL	R11	6
      0x80041600,  //  0029  RET	1	R11
      0x70020008,  //  002A  JMP		#0034
      0xB81E1400,  //  002B  GETNGBL	R7	K10
      0x8C1C0F1F,  //  002C  GETMET	R7	R7	K31
      0x5C240200,  //  002D  MOVE	R9	R1
      0x5C280400,  //  002E  MOVE	R10	R2
      0x5C2C0600,  //  002F  MOVE	R11	R3
      0x5C300800,  //  0030  MOVE	R12	R4
      0x5C340A00,  //  0031  MOVE	R13	R5
      0x7C1C0C00,  //  0032  CALL	R7	6
      0x80040E00,  //  0033  RET	1	R7
      0x80000000,  //  0034  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(class_rich_palette_color_start,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_rich_palette_color,     /* shared constants */
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x88080104,  //  0000  GETMBR	R2	R0	K4
      0x4C0C0000,  //  0001  LDNIL	R3
      0x1C080403,  //  0002  EQ	R2	R2	R3
      0x780A0005,  //  0003  JMPF	R2	#000A
      0x88080107,  //  0004  GETMBR	R2	R0	K7
      0x4C0C0000,  //  0005  LDNIL	R3
      0x1C080403,  //  0006  EQ	R2	R2	R3
      0x780A0001,  //  0007  JMPF	R2	#000A
      0x8C08011A,  //  0008  GETMET	R2	R0	K26
      0x7C080200,  //  0009  CALL	R2	1
      0x60080003,  //  000A  GETGBL	R2	G3
      0x5C0C0000,  //  000B  MOVE	R3	R0
      0x7C080200,  //  000C  CALL	R2	1
      0x8C080522,  //  000D  GETMET	R2	R2	K34
      0x5C100200,  //  000E  MOVE	R4	R1
      0x7C080400,  //  000F  CALL	R2	2
      0x80040000,  //  0010  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: produce_value
********************************************************************/
be_local_closure(class_rich_palette_color_produce_value,   /* name */
  be_nested_proto(
    24,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_rich_palette_color,     /* shared constants */
    be_str_weak(produce_value),
    &be_const_str_solidified,
    ( &(const binstruction[195]) {  /* code */
      0x8C0C0123,  //  0000  GETMET	R3	R0	K35
      0x5C140400,  //  0001  MOVE	R5	R2
      0x7C0C0400,  //  0002  CALL	R3	2
      0x5C080600,  //  0003  MOVE	R2	R3
      0x880C0104,  //  0004  GETMBR	R3	R0	K4
      0x4C100000,  //  0005  LDNIL	R4
      0x1C0C0604,  //  0006  EQ	R3	R3	R4
      0x780E0005,  //  0007  JMPF	R3	#000E
      0x880C0107,  //  0008  GETMBR	R3	R0	K7
      0x4C100000,  //  0009  LDNIL	R4
      0x1C0C0604,  //  000A  EQ	R3	R3	R4
      0x780E0001,  //  000B  JMPF	R3	#000E
      0x8C0C011A,  //  000C  GETMET	R3	R0	K26
      0x7C0C0200,  //  000D  CALL	R3	1
      0x8C0C0101,  //  000E  GETMET	R3	R0	K1
      0x7C0C0200,  //  000F  CALL	R3	1
      0x4C100000,  //  0010  LDNIL	R4
      0x1C100604,  //  0011  EQ	R4	R3	R4
      0x74120002,  //  0012  JMPT	R4	#0016
      0x88100102,  //  0013  GETMBR	R4	R0	K2
      0x1410091E,  //  0014  LT	R4	R4	K30
      0x78120001,  //  0015  JMPF	R4	#0018
      0x5411FFFE,  //  0016  LDINT	R4	-1
      0x80040800,  //  0017  RET	1	R4
      0x88100100,  //  0018  GETMBR	R4	R0	K0
      0x88140112,  //  0019  GETMBR	R5	R0	K18
      0x1C180903,  //  001A  EQ	R6	R4	K3
      0x781A0038,  //  001B  JMPF	R6	#0055
      0x8C180709,  //  001C  GETMET	R6	R3	K9
      0x58200003,  //  001D  LDCONST	R8	K3
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
      0xB82A1400,  //  002F  GETNGBL	R10	K10
      0x8C28151F,  //  0030  GETMET	R10	R10	K31
      0x5C300E00,  //  0031  MOVE	R12	R7
      0x58340003,  //  0032  LDCONST	R13	K3
      0x543A00FE,  //  0033  LDINT	R14	255
      0x583C0003,  //  0034  LDCONST	R15	K3
      0x5C400A00,  //  0035  MOVE	R16	R5
      0x7C280C00,  //  0036  CALL	R10	6
      0x5C1C1400,  //  0037  MOVE	R7	R10
      0xB82A1400,  //  0038  GETNGBL	R10	K10
      0x8C28151F,  //  0039  GETMET	R10	R10	K31
      0x5C301000,  //  003A  MOVE	R12	R8
      0x58340003,  //  003B  LDCONST	R13	K3
      0x543A00FE,  //  003C  LDINT	R14	255
      0x583C0003,  //  003D  LDCONST	R15	K3
      0x5C400A00,  //  003E  MOVE	R16	R5
      0x7C280C00,  //  003F  CALL	R10	6
      0x5C201400,  //  0040  MOVE	R8	R10
      0xB82A1400,  //  0041  GETNGBL	R10	K10
      0x8C28151F,  //  0042  GETMET	R10	R10	K31
      0x5C301200,  //  0043  MOVE	R12	R9
      0x58340003,  //  0044  LDCONST	R13	K3
      0x543A00FE,  //  0045  LDINT	R14	255
      0x583C0003,  //  0046  LDCONST	R15	K3
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
      0x80041400,  //  0054  RET	1	R10
      0x88180124,  //  0055  GETMBR	R6	R0	K36
      0x04180406,  //  0056  SUB	R6	R2	R6
      0x101C0C04,  //  0057  MOD	R7	R6	R4
      0x88200102,  //  0058  GETMBR	R8	R0	K2
      0x0424111E,  //  0059  SUB	R9	R8	K30
      0x24281303,  //  005A  GT	R10	R9	K3
      0x782A0006,  //  005B  JMPF	R10	#0063
      0x88280104,  //  005C  GETMBR	R10	R0	K4
      0x94281409,  //  005D  GETIDX	R10	R10	R9
      0x28280E0A,  //  005E  GE	R10	R7	R10
      0x782A0000,  //  005F  JMPF	R10	#0061
      0x70020001,  //  0060  JMP		#0063
      0x04241306,  //  0061  SUB	R9	R9	K6
      0x7001FFF6,  //  0062  JMP		#005A
      0x8C280709,  //  0063  GETMET	R10	R3	K9
      0x54320003,  //  0064  LDINT	R12	4
      0x0830120C,  //  0065  MUL	R12	R9	R12
      0x54360003,  //  0066  LDINT	R13	4
      0x7C280600,  //  0067  CALL	R10	3
      0x8C2C0709,  //  0068  GETMET	R11	R3	K9
      0x00341306,  //  0069  ADD	R13	R9	K6
      0x543A0003,  //  006A  LDINT	R14	4
      0x08341A0E,  //  006B  MUL	R13	R13	R14
      0x543A0003,  //  006C  LDINT	R14	4
      0x7C2C0600,  //  006D  CALL	R11	3
      0x88300104,  //  006E  GETMBR	R12	R0	K4
      0x94301809,  //  006F  GETIDX	R12	R12	R9
      0x00341306,  //  0070  ADD	R13	R9	K6
      0x88380104,  //  0071  GETMBR	R14	R0	K4
      0x94341C0D,  //  0072  GETIDX	R13	R14	R13
      0x8C380125,  //  0073  GETMET	R14	R0	K37
      0x5C400E00,  //  0074  MOVE	R16	R7
      0x5C441800,  //  0075  MOVE	R17	R12
      0x5C481A00,  //  0076  MOVE	R18	R13
      0x544E0007,  //  0077  LDINT	R19	8
      0x3C4C1413,  //  0078  SHR	R19	R10	R19
      0x545200FE,  //  0079  LDINT	R20	255
      0x2C4C2614,  //  007A  AND	R19	R19	R20
      0x54520007,  //  007B  LDINT	R20	8
      0x3C501614,  //  007C  SHR	R20	R11	R20
      0x545600FE,  //  007D  LDINT	R21	255
      0x2C502815,  //  007E  AND	R20	R20	R21
      0x7C380C00,  //  007F  CALL	R14	6
      0x8C3C0125,  //  0080  GETMET	R15	R0	K37
      0x5C440E00,  //  0081  MOVE	R17	R7
      0x5C481800,  //  0082  MOVE	R18	R12
      0x5C4C1A00,  //  0083  MOVE	R19	R13
      0x5452000F,  //  0084  LDINT	R20	16
      0x3C501414,  //  0085  SHR	R20	R10	R20
      0x545600FE,  //  0086  LDINT	R21	255
      0x2C502815,  //  0087  AND	R20	R20	R21
      0x5456000F,  //  0088  LDINT	R21	16
      0x3C541615,  //  0089  SHR	R21	R11	R21
      0x545A00FE,  //  008A  LDINT	R22	255
      0x2C542A16,  //  008B  AND	R21	R21	R22
      0x7C3C0C00,  //  008C  CALL	R15	6
      0x8C400125,  //  008D  GETMET	R16	R0	K37
      0x5C480E00,  //  008E  MOVE	R18	R7
      0x5C4C1800,  //  008F  MOVE	R19	R12
      0x5C501A00,  //  0090  MOVE	R20	R13
      0x54560017,  //  0091  LDINT	R21	24
      0x3C541415,  //  0092  SHR	R21	R10	R21
      0x545A00FE,  //  0093  LDINT	R22	255
      0x2C542A16,  //  0094  AND	R21	R21	R22
      0x545A0017,  //  0095  LDINT	R22	24
      0x3C581616,  //  0096  SHR	R22	R11	R22
      0x545E00FE,  //  0097  LDINT	R23	255
      0x2C582C17,  //  0098  AND	R22	R22	R23
      0x7C400C00,  //  0099  CALL	R16	6
      0x544600FE,  //  009A  LDINT	R17	255
      0x20440A11,  //  009B  NE	R17	R5	R17
      0x7846001A,  //  009C  JMPF	R17	#00B8
      0xB8461400,  //  009D  GETNGBL	R17	K10
      0x8C44231F,  //  009E  GETMET	R17	R17	K31
      0x5C4C1C00,  //  009F  MOVE	R19	R14
      0x58500003,  //  00A0  LDCONST	R20	K3
      0x545600FE,  //  00A1  LDINT	R21	255
      0x58580003,  //  00A2  LDCONST	R22	K3
      0x5C5C0A00,  //  00A3  MOVE	R23	R5
      0x7C440C00,  //  00A4  CALL	R17	6
      0x5C382200,  //  00A5  MOVE	R14	R17
      0xB8461400,  //  00A6  GETNGBL	R17	K10
      0x8C44231F,  //  00A7  GETMET	R17	R17	K31
      0x5C4C1E00,  //  00A8  MOVE	R19	R15
      0x58500003,  //  00A9  LDCONST	R20	K3
      0x545600FE,  //  00AA  LDINT	R21	255
      0x58580003,  //  00AB  LDCONST	R22	K3
      0x5C5C0A00,  //  00AC  MOVE	R23	R5
      0x7C440C00,  //  00AD  CALL	R17	6
      0x5C3C2200,  //  00AE  MOVE	R15	R17
      0xB8461400,  //  00AF  GETNGBL	R17	K10
      0x8C44231F,  //  00B0  GETMET	R17	R17	K31
      0x5C4C2000,  //  00B1  MOVE	R19	R16
      0x58500003,  //  00B2  LDCONST	R20	K3
      0x545600FE,  //  00B3  LDINT	R21	255
      0x58580003,  //  00B4  LDCONST	R22	K3
      0x5C5C0A00,  //  00B5  MOVE	R23	R5
      0x7C440C00,  //  00B6  CALL	R17	6
      0x5C402200,  //  00B7  MOVE	R16	R17
      0x544600FE,  //  00B8  LDINT	R17	255
      0x544A0017,  //  00B9  LDINT	R18	24
      0x38442212,  //  00BA  SHL	R17	R17	R18
      0x544A000F,  //  00BB  LDINT	R18	16
      0x38481C12,  //  00BC  SHL	R18	R14	R18
      0x30442212,  //  00BD  OR	R17	R17	R18
      0x544A0007,  //  00BE  LDINT	R18	8
      0x38481E12,  //  00BF  SHL	R18	R15	R18
      0x30442212,  //  00C0  OR	R17	R17	R18
      0x30442210,  //  00C1  OR	R17	R17	R16
      0x80042200,  //  00C2  RET	1	R17
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_rich_palette_color_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_rich_palette_color,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080526,  //  0003  GETMET	R2	R2	K38
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x90020503,  //  0006  SETMBR	R0	K2	K3
      0xB80A4E00,  //  0007  GETNGBL	R2	K39
      0x88080528,  //  0008  GETMBR	R2	R2	K40
      0x90023002,  //  0009  SETMBR	R0	K24	R2
      0x8C080329,  //  000A  GETMET	R2	R1	K41
      0x5C100000,  //  000B  MOVE	R4	R0
      0x7C080400,  //  000C  CALL	R2	2
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _get_color_for_value_uncached
********************************************************************/
be_local_closure(class_rich_palette_color__get_color_for_value_uncached,   /* name */
  be_nested_proto(
    20,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_rich_palette_color,     /* shared constants */
    be_str_weak(_get_color_for_value_uncached),
    &be_const_str_solidified,
    ( &(const binstruction[89]) {  /* code */
      0x880C0104,  //  0000  GETMBR	R3	R0	K4
      0x4C100000,  //  0001  LDNIL	R4
      0x1C0C0604,  //  0002  EQ	R3	R3	R4
      0x780E0005,  //  0003  JMPF	R3	#000A
      0x880C0107,  //  0004  GETMBR	R3	R0	K7
      0x4C100000,  //  0005  LDNIL	R4
      0x1C0C0604,  //  0006  EQ	R3	R3	R4
      0x780E0001,  //  0007  JMPF	R3	#000A
      0x8C0C011A,  //  0008  GETMET	R3	R0	K26
      0x7C0C0200,  //  0009  CALL	R3	1
      0x8C0C0101,  //  000A  GETMET	R3	R0	K1
      0x7C0C0200,  //  000B  CALL	R3	1
      0x88100102,  //  000C  GETMBR	R4	R0	K2
      0x0414091E,  //  000D  SUB	R5	R4	K30
      0x24180B03,  //  000E  GT	R6	R5	K3
      0x781A0006,  //  000F  JMPF	R6	#0017
      0x88180107,  //  0010  GETMBR	R6	R0	K7
      0x94180C05,  //  0011  GETIDX	R6	R6	R5
      0x28180206,  //  0012  GE	R6	R1	R6
      0x781A0000,  //  0013  JMPF	R6	#0015
      0x70020001,  //  0014  JMP		#0017
      0x04140B06,  //  0015  SUB	R5	R5	K6
      0x7001FFF6,  //  0016  JMP		#000E
      0x8C180709,  //  0017  GETMET	R6	R3	K9
      0x54220003,  //  0018  LDINT	R8	4
      0x08200A08,  //  0019  MUL	R8	R5	R8
      0x54260003,  //  001A  LDINT	R9	4
      0x7C180600,  //  001B  CALL	R6	3
      0x8C1C0709,  //  001C  GETMET	R7	R3	K9
      0x00240B06,  //  001D  ADD	R9	R5	K6
      0x542A0003,  //  001E  LDINT	R10	4
      0x0824120A,  //  001F  MUL	R9	R9	R10
      0x542A0003,  //  0020  LDINT	R10	4
      0x7C1C0600,  //  0021  CALL	R7	3
      0x88200107,  //  0022  GETMBR	R8	R0	K7
      0x94201005,  //  0023  GETIDX	R8	R8	R5
      0x00240B06,  //  0024  ADD	R9	R5	K6
      0x88280107,  //  0025  GETMBR	R10	R0	K7
      0x94241409,  //  0026  GETIDX	R9	R10	R9
      0x8C280125,  //  0027  GETMET	R10	R0	K37
      0x5C300200,  //  0028  MOVE	R12	R1
      0x5C341000,  //  0029  MOVE	R13	R8
      0x5C381200,  //  002A  MOVE	R14	R9
      0x543E0007,  //  002B  LDINT	R15	8
      0x3C3C0C0F,  //  002C  SHR	R15	R6	R15
      0x544200FE,  //  002D  LDINT	R16	255
      0x2C3C1E10,  //  002E  AND	R15	R15	R16
      0x54420007,  //  002F  LDINT	R16	8
      0x3C400E10,  //  0030  SHR	R16	R7	R16
      0x544600FE,  //  0031  LDINT	R17	255
      0x2C402011,  //  0032  AND	R16	R16	R17
      0x7C280C00,  //  0033  CALL	R10	6
      0x8C2C0125,  //  0034  GETMET	R11	R0	K37
      0x5C340200,  //  0035  MOVE	R13	R1
      0x5C381000,  //  0036  MOVE	R14	R8
      0x5C3C1200,  //  0037  MOVE	R15	R9
      0x5442000F,  //  0038  LDINT	R16	16
      0x3C400C10,  //  0039  SHR	R16	R6	R16
      0x544600FE,  //  003A  LDINT	R17	255
      0x2C402011,  //  003B  AND	R16	R16	R17
      0x5446000F,  //  003C  LDINT	R17	16
      0x3C440E11,  //  003D  SHR	R17	R7	R17
      0x544A00FE,  //  003E  LDINT	R18	255
      0x2C442212,  //  003F  AND	R17	R17	R18
      0x7C2C0C00,  //  0040  CALL	R11	6
      0x8C300125,  //  0041  GETMET	R12	R0	K37
      0x5C380200,  //  0042  MOVE	R14	R1
      0x5C3C1000,  //  0043  MOVE	R15	R8
      0x5C401200,  //  0044  MOVE	R16	R9
      0x54460017,  //  0045  LDINT	R17	24
      0x3C440C11,  //  0046  SHR	R17	R6	R17
      0x544A00FE,  //  0047  LDINT	R18	255
      0x2C442212,  //  0048  AND	R17	R17	R18
      0x544A0017,  //  0049  LDINT	R18	24
      0x3C480E12,  //  004A  SHR	R18	R7	R18
      0x544E00FE,  //  004B  LDINT	R19	255
      0x2C482413,  //  004C  AND	R18	R18	R19
      0x7C300C00,  //  004D  CALL	R12	6
      0x543600FE,  //  004E  LDINT	R13	255
      0x543A0017,  //  004F  LDINT	R14	24
      0x38341A0E,  //  0050  SHL	R13	R13	R14
      0x543A000F,  //  0051  LDINT	R14	16
      0x3838140E,  //  0052  SHL	R14	R10	R14
      0x30341A0E,  //  0053  OR	R13	R13	R14
      0x543A0007,  //  0054  LDINT	R14	8
      0x3838160E,  //  0055  SHL	R14	R11	R14
      0x30341A0E,  //  0056  OR	R13	R13	R14
      0x30341A0C,  //  0057  OR	R13	R13	R12
      0x80041A00,  //  0058  RET	1	R13
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: on_param_changed
********************************************************************/
be_local_closure(class_rich_palette_color_on_param_changed,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_rich_palette_color,     /* shared constants */
    be_str_weak(on_param_changed),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C072A,  //  0003  GETMET	R3	R3	K42
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x1C0C0300,  //  0007  EQ	R3	R1	K0
      0x740E0001,  //  0008  JMPT	R3	#000B
      0x1C0C0318,  //  0009  EQ	R3	R1	K24
      0x780E0009,  //  000A  JMPF	R3	#0015
      0x880C0104,  //  000B  GETMBR	R3	R0	K4
      0x4C100000,  //  000C  LDNIL	R4
      0x200C0604,  //  000D  NE	R3	R3	R4
      0x740E0003,  //  000E  JMPT	R3	#0013
      0x880C0107,  //  000F  GETMBR	R3	R0	K7
      0x4C100000,  //  0010  LDNIL	R4
      0x200C0604,  //  0011  NE	R3	R3	R4
      0x780E0001,  //  0012  JMPF	R3	#0015
      0x8C0C011A,  //  0013  GETMET	R3	R0	K26
      0x7C0C0200,  //  0014  CALL	R3	1
      0x1C0C0318,  //  0015  EQ	R3	R1	K24
      0x740E0001,  //  0016  JMPT	R3	#0019
      0x1C0C0320,  //  0017  EQ	R3	R1	K32
      0x780E0001,  //  0018  JMPF	R3	#001B
      0x500C0200,  //  0019  LDBOOL	R3	1	0
      0x90021A03,  //  001A  SETMBR	R0	K13	R3
      0x80000000,  //  001B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: rich_palette_color
********************************************************************/
extern const bclass be_class_color_provider;
be_local_class(rich_palette_color,
    4,
    &be_class_color_provider,
    be_nested_map(20,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(_recompute_palette, -1), be_const_closure(class_rich_palette_color__recompute_palette_closure) },
        { be_const_key_weak(to_css_gradient, -1), be_const_closure(class_rich_palette_color_to_css_gradient_closure) },
        { be_const_key_weak(_brightness, -1), be_const_var(3) },
        { be_const_key_weak(PARAMS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(period, 1), be_const_bytes_instance(050000018813) },
        { be_const_key_weak(transition_type, -1), be_const_bytes_instance(1400010200010005) },
        { be_const_key_weak(colors, -1), be_const_bytes_instance(0C0602) },
    }))    ) } )) },
        { be_const_key_weak(update, -1), be_const_closure(class_rich_palette_color_update_closure) },
        { be_const_key_weak(_parse_palette, 18), be_const_closure(class_rich_palette_color__parse_palette_closure) },
        { be_const_key_weak(_get_palette_bytes, -1), be_const_closure(class_rich_palette_color__get_palette_bytes_closure) },
        { be_const_key_weak(_rebuild_color_lut, -1), be_const_closure(class_rich_palette_color__rebuild_color_lut_closure) },
        { be_const_key_weak(start, -1), be_const_closure(class_rich_palette_color_start_closure) },
        { be_const_key_weak(get_color_for_value, 2), be_const_closure(class_rich_palette_color_get_color_for_value_closure) },
        { be_const_key_weak(produce_value, -1), be_const_closure(class_rich_palette_color_produce_value_closure) },
        { be_const_key_weak(_slots, 8), be_const_var(2) },
        { be_const_key_weak(_slots_arr, 14), be_const_var(0) },
        { be_const_key_weak(_get_color_at_index, 10), be_const_closure(class_rich_palette_color__get_color_at_index_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_rich_palette_color_init_closure) },
        { be_const_key_weak(_value_arr, 12), be_const_var(1) },
        { be_const_key_weak(_get_color_for_value_uncached, -1), be_const_closure(class_rich_palette_color__get_color_for_value_uncached_closure) },
        { be_const_key_weak(on_param_changed, -1), be_const_closure(class_rich_palette_color_on_param_changed_closure) },
        { be_const_key_weak(_interpolate, 1), be_const_closure(class_rich_palette_color__interpolate_closure) },
        { be_const_key_weak(_DEFAULT_PALETTE, -1), be_const_bytes_instance(00FF000024FFA50049FFFF006E00FF00920000FFB74B0082DBEE82EEFFFF0000) },
    })),
    be_str_weak(rich_palette_color)
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


extern const bclass be_class_static_value;

/********************************************************************
** Solidified function: produce_value
********************************************************************/
be_local_closure(class_static_value_produce_value,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(value),
    }),
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
** Solidified class: static_value
********************************************************************/
extern const bclass be_class_parameterized_object;
be_local_class(static_value,
    0,
    &be_class_parameterized_object,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(produce_value, 1), be_const_closure(class_static_value_produce_value_closure) },
        { be_const_key_weak(VALUE_PROVIDER, 2), be_const_bool(1) },
        { be_const_key_weak(PARAMS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(value, -1), be_const_bytes_instance(0C0604) },
    }))    ) } )) },
    })),
    be_str_weak(static_value)
);
// compact class 'rich_palette' ktab size: 13, total: 15 (saved 16 bytes)
static const bvalue be_ktab_class_rich_palette[13] = {
  /* K0   */  be_nested_str_weak(init),
  /* K1   */  be_nested_str_weak(color_provider),
  /* K2   */  be_nested_str_weak(animation),
  /* K3   */  be_nested_str_weak(rich_palette_color),
  /* K4   */  be_nested_str_weak(values),
  /* K5   */  be_nested_str_weak(color),
  /* K6   */  be_nested_str_weak(start),
  /* K7   */  be_nested_str_weak(on_param_changed),
  /* K8   */  be_nested_str_weak(colors),
  /* K9   */  be_nested_str_weak(period),
  /* K10  */  be_nested_str_weak(transition_type),
  /* K11  */  be_nested_str_weak(brightness),
  /* K12  */  be_nested_str_weak(set_param),
};


extern const bclass be_class_rich_palette;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_rich_palette_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_rich_palette,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0xB80A0400,  //  0006  GETNGBL	R2	K2
      0x8C080503,  //  0007  GETMET	R2	R2	K3
      0x5C100200,  //  0008  MOVE	R4	R1
      0x7C080400,  //  0009  CALL	R2	2
      0x90020202,  //  000A  SETMBR	R0	K1	R2
      0x88080104,  //  000B  GETMBR	R2	R0	K4
      0x880C0101,  //  000C  GETMBR	R3	R0	K1
      0x980A0A03,  //  000D  SETIDX	R2	K5	R3
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(class_rich_palette_start,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_rich_palette,     /* shared constants */
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080506,  //  0003  GETMET	R2	R2	K6
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x88080101,  //  0006  GETMBR	R2	R0	K1
      0x8C080506,  //  0007  GETMET	R2	R2	K6
      0x5C100200,  //  0008  MOVE	R4	R1
      0x7C080400,  //  0009  CALL	R2	2
      0x80040000,  //  000A  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: on_param_changed
********************************************************************/
be_local_closure(class_rich_palette_on_param_changed,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_rich_palette,     /* shared constants */
    be_str_weak(on_param_changed),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0707,  //  0003  GETMET	R3	R3	K7
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x1C0C0308,  //  0007  EQ	R3	R1	K8
      0x740E0005,  //  0008  JMPT	R3	#000F
      0x1C0C0309,  //  0009  EQ	R3	R1	K9
      0x740E0003,  //  000A  JMPT	R3	#000F
      0x1C0C030A,  //  000B  EQ	R3	R1	K10
      0x740E0001,  //  000C  JMPT	R3	#000F
      0x1C0C030B,  //  000D  EQ	R3	R1	K11
      0x780E0004,  //  000E  JMPF	R3	#0014
      0x880C0101,  //  000F  GETMBR	R3	R0	K1
      0x8C0C070C,  //  0010  GETMET	R3	R3	K12
      0x5C140200,  //  0011  MOVE	R5	R1
      0x5C180400,  //  0012  MOVE	R6	R2
      0x7C0C0600,  //  0013  CALL	R3	3
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: rich_palette
********************************************************************/
extern const bclass be_class_Animation;
be_local_class(rich_palette,
    1,
    &be_class_Animation,
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, -1), be_const_closure(class_rich_palette_init_closure) },
        { be_const_key_weak(start, -1), be_const_closure(class_rich_palette_start_closure) },
        { be_const_key_weak(on_param_changed, 4), be_const_closure(class_rich_palette_on_param_changed_closure) },
        { be_const_key_weak(PARAMS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(period, -1), be_const_bytes_instance(050000018813) },
        { be_const_key_weak(colors, -1), be_const_bytes_instance(0C0605) },
        { be_const_key_weak(brightness, -1), be_const_bytes_instance(07000001FF0001FF00) },
        { be_const_key_weak(transition_type, -1), be_const_bytes_instance(1400050200010005) },
    }))    ) } )) },
        { be_const_key_weak(color_provider, -1), be_const_var(0) },
    })),
    be_str_weak(rich_palette)
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
    /* K3   */  be_const_int(1),
    }),
    be_str_weak(ramp),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x90060503,  //  0004  SETMBR	R1	K2	K3
      0x80040200,  //  0005  RET	1	R1
    })
  )
);
/*******************************************************************/

// compact class 'twinkle' ktab size: 33, total: 54 (saved 168 bytes)
static const bvalue be_ktab_class_twinkle[33] = {
  /* K0   */  be_const_int(0),
  /* K1   */  be_nested_str_weak(_random),
  /* K2   */  be_nested_str_weak(current_colors),
  /* K3   */  be_nested_str_weak(size),
  /* K4   */  be_nested_str_weak(_initialize_arrays),
  /* K5   */  be_nested_str_weak(width),
  /* K6   */  be_nested_str_weak(get),
  /* K7   */  be_nested_str_weak(set_pixel_color),
  /* K8   */  be_const_int(1),
  /* K9   */  be_nested_str_weak(twinkle_speed),
  /* K10  */  be_nested_str_weak(last_update),
  /* K11  */  be_nested_str_weak(_update_twinkle_simulation),
  /* K12  */  be_nested_str_weak(random_seed),
  /* K13  */  be_const_int(1103515245),
  /* K14  */  be_const_int(2147483647),
  /* K15  */  be_nested_str_weak(engine),
  /* K16  */  be_nested_str_weak(strip_length),
  /* K17  */  be_nested_str_weak(clear),
  /* K18  */  be_nested_str_weak(resize),
  /* K19  */  be_nested_str_weak(set),
  /* K20  */  be_nested_str_weak(on_param_changed),
  /* K21  */  be_nested_str_weak(set_param),
  /* K22  */  be_nested_str_weak(init),
  /* K23  */  be_nested_str_weak(time_ms),
  /* K24  */  be_nested_str_weak(fade_speed),
  /* K25  */  be_nested_str_weak(density),
  /* K26  */  be_nested_str_weak(min_brightness),
  /* K27  */  be_nested_str_weak(max_brightness),
  /* K28  */  be_nested_str_weak(color),
  /* K29  */  be_nested_str_weak(tasmota),
  /* K30  */  be_nested_str_weak(scale_uint),
  /* K31  */  be_const_int(16777215),
  /* K32  */  be_nested_str_weak(_random_range),
};


extern const bclass be_class_twinkle;

/********************************************************************
** Solidified function: _random_range
********************************************************************/
be_local_closure(class_twinkle__random_range,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_twinkle,     /* shared constants */
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
be_local_closure(class_twinkle_render,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_twinkle,     /* shared constants */
    be_str_weak(render),
    &be_const_str_solidified,
    ( &(const binstruction[36]) {  /* code */
      0x88100102,  //  0000  GETMBR	R4	R0	K2
      0x8C100903,  //  0001  GETMET	R4	R4	K3
      0x7C100200,  //  0002  CALL	R4	1
      0x54160003,  //  0003  LDINT	R5	4
      0x08140605,  //  0004  MUL	R5	R3	R5
      0x20100805,  //  0005  NE	R4	R4	R5
      0x78120001,  //  0006  JMPF	R4	#0009
      0x8C100104,  //  0007  GETMET	R4	R0	K4
      0x7C100200,  //  0008  CALL	R4	1
      0x50100000,  //  0009  LDBOOL	R4	0	0
      0x58140000,  //  000A  LDCONST	R5	K0
      0x14180A03,  //  000B  LT	R6	R5	R3
      0x781A0015,  //  000C  JMPF	R6	#0023
      0x88180305,  //  000D  GETMBR	R6	R1	K5
      0x14180A06,  //  000E  LT	R6	R5	R6
      0x781A0010,  //  000F  JMPF	R6	#0021
      0x88180102,  //  0010  GETMBR	R6	R0	K2
      0x8C180D06,  //  0011  GETMET	R6	R6	K6
      0x54220003,  //  0012  LDINT	R8	4
      0x08200A08,  //  0013  MUL	R8	R5	R8
      0x5425FFFB,  //  0014  LDINT	R9	-4
      0x7C180600,  //  0015  CALL	R6	3
      0x541E0017,  //  0016  LDINT	R7	24
      0x3C1C0C07,  //  0017  SHR	R7	R6	R7
      0x542200FE,  //  0018  LDINT	R8	255
      0x2C1C0E08,  //  0019  AND	R7	R7	R8
      0x241C0F00,  //  001A  GT	R7	R7	K0
      0x781E0004,  //  001B  JMPF	R7	#0021
      0x8C1C0307,  //  001C  GETMET	R7	R1	K7
      0x5C240A00,  //  001D  MOVE	R9	R5
      0x5C280C00,  //  001E  MOVE	R10	R6
      0x7C1C0600,  //  001F  CALL	R7	3
      0x50100200,  //  0020  LDBOOL	R4	1	0
      0x00140B08,  //  0021  ADD	R5	R5	K8
      0x7001FFE7,  //  0022  JMP		#000B
      0x80040800,  //  0023  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update
********************************************************************/
be_local_closure(class_twinkle_update,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_twinkle,     /* shared constants */
    be_str_weak(update),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x88080109,  //  0000  GETMBR	R2	R0	K9
      0x540E03E7,  //  0001  LDINT	R3	1000
      0x0C0C0602,  //  0002  DIV	R3	R3	R2
      0x8810010A,  //  0003  GETMBR	R4	R0	K10
      0x04100204,  //  0004  SUB	R4	R1	R4
      0x28100803,  //  0005  GE	R4	R4	R3
      0x78120003,  //  0006  JMPF	R4	#000B
      0x90021401,  //  0007  SETMBR	R0	K10	R1
      0x8C10010B,  //  0008  GETMET	R4	R0	K11
      0x5C180200,  //  0009  MOVE	R6	R1
      0x7C100400,  //  000A  CALL	R4	2
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _random
********************************************************************/
be_local_closure(class_twinkle__random,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_twinkle,     /* shared constants */
    be_str_weak(_random),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x8804010C,  //  0000  GETMBR	R1	R0	K12
      0x0804030D,  //  0001  MUL	R1	R1	K13
      0x540A3038,  //  0002  LDINT	R2	12345
      0x00040202,  //  0003  ADD	R1	R1	R2
      0x2C04030E,  //  0004  AND	R1	R1	K14
      0x90021801,  //  0005  SETMBR	R0	K12	R1
      0x8804010C,  //  0006  GETMBR	R1	R0	K12
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _initialize_arrays
********************************************************************/
be_local_closure(class_twinkle__initialize_arrays,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_twinkle,     /* shared constants */
    be_str_weak(_initialize_arrays),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x8804010F,  //  0000  GETMBR	R1	R0	K15
      0x88040310,  //  0001  GETMBR	R1	R1	K16
      0x88080102,  //  0002  GETMBR	R2	R0	K2
      0x8C080511,  //  0003  GETMET	R2	R2	K17
      0x7C080200,  //  0004  CALL	R2	1
      0x88080102,  //  0005  GETMBR	R2	R0	K2
      0x8C080512,  //  0006  GETMET	R2	R2	K18
      0x54120003,  //  0007  LDINT	R4	4
      0x08100204,  //  0008  MUL	R4	R1	R4
      0x7C080400,  //  0009  CALL	R2	2
      0x58080000,  //  000A  LDCONST	R2	K0
      0x140C0401,  //  000B  LT	R3	R2	R1
      0x780E0008,  //  000C  JMPF	R3	#0016
      0x880C0102,  //  000D  GETMBR	R3	R0	K2
      0x8C0C0713,  //  000E  GETMET	R3	R3	K19
      0x54160003,  //  000F  LDINT	R5	4
      0x08140405,  //  0010  MUL	R5	R2	R5
      0x58180000,  //  0011  LDCONST	R6	K0
      0x541DFFFB,  //  0012  LDINT	R7	-4
      0x7C0C0800,  //  0013  CALL	R3	4
      0x00080508,  //  0014  ADD	R2	R2	K8
      0x7001FFF4,  //  0015  JMP		#000B
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: on_param_changed
********************************************************************/
be_local_closure(class_twinkle_on_param_changed,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_twinkle,     /* shared constants */
    be_str_weak(on_param_changed),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0714,  //  0003  GETMET	R3	R3	K20
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x1C0C0309,  //  0007  EQ	R3	R1	K9
      0x780E0010,  //  0008  JMPF	R3	#001A
      0x540E0031,  //  0009  LDINT	R3	50
      0x280C0403,  //  000A  GE	R3	R2	R3
      0x780E000D,  //  000B  JMPF	R3	#001A
      0x540E03E7,  //  000C  LDINT	R3	1000
      0x0C0C0602,  //  000D  DIV	R3	R3	R2
      0x14100708,  //  000E  LT	R4	R3	K8
      0x78120001,  //  000F  JMPF	R4	#0012
      0x580C0008,  //  0010  LDCONST	R3	K8
      0x70020003,  //  0011  JMP		#0016
      0x54120013,  //  0012  LDINT	R4	20
      0x24100604,  //  0013  GT	R4	R3	R4
      0x78120000,  //  0014  JMPF	R4	#0016
      0x540E0013,  //  0015  LDINT	R3	20
      0x8C100115,  //  0016  GETMET	R4	R0	K21
      0x58180009,  //  0017  LDCONST	R6	K9
      0x5C1C0600,  //  0018  MOVE	R7	R3
      0x7C100600,  //  0019  CALL	R4	3
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_twinkle_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_twinkle,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080516,  //  0003  GETMET	R2	R2	K22
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x60080015,  //  0006  GETGBL	R2	G21
      0x7C080000,  //  0007  CALL	R2	0
      0x90020402,  //  0008  SETMBR	R0	K2	R2
      0x90021500,  //  0009  SETMBR	R0	K10	K0
      0x8808010F,  //  000A  GETMBR	R2	R0	K15
      0x88080517,  //  000B  GETMBR	R2	R2	K23
      0x540EFFFF,  //  000C  LDINT	R3	65536
      0x10080403,  //  000D  MOD	R2	R2	R3
      0x90021802,  //  000E  SETMBR	R0	K12	R2
      0x8C080104,  //  000F  GETMET	R2	R0	K4
      0x7C080200,  //  0010  CALL	R2	1
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _update_twinkle_simulation
********************************************************************/
be_local_closure(class_twinkle__update_twinkle_simulation,   /* name */
  be_nested_proto(
    22,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_twinkle,     /* shared constants */
    be_str_weak(_update_twinkle_simulation),
    &be_const_str_solidified,
    ( &(const binstruction[116]) {  /* code */
      0x88080118,  //  0000  GETMBR	R2	R0	K24
      0x880C0119,  //  0001  GETMBR	R3	R0	K25
      0x8810011A,  //  0002  GETMBR	R4	R0	K26
      0x8814011B,  //  0003  GETMBR	R5	R0	K27
      0x8818011C,  //  0004  GETMBR	R6	R0	K28
      0x881C010F,  //  0005  GETMBR	R7	R0	K15
      0x881C0F10,  //  0006  GETMBR	R7	R7	K16
      0x88200102,  //  0007  GETMBR	R8	R0	K2
      0x8C201103,  //  0008  GETMET	R8	R8	K3
      0x7C200200,  //  0009  CALL	R8	1
      0x54260003,  //  000A  LDINT	R9	4
      0x08240E09,  //  000B  MUL	R9	R7	R9
      0x20201009,  //  000C  NE	R8	R8	R9
      0x78220001,  //  000D  JMPF	R8	#0010
      0x8C200104,  //  000E  GETMET	R8	R0	K4
      0x7C200200,  //  000F  CALL	R8	1
      0x58200000,  //  0010  LDCONST	R8	K0
      0x14241007,  //  0011  LT	R9	R8	R7
      0x7826002A,  //  0012  JMPF	R9	#003E
      0x88240102,  //  0013  GETMBR	R9	R0	K2
      0x8C241306,  //  0014  GETMET	R9	R9	K6
      0x542E0003,  //  0015  LDINT	R11	4
      0x082C100B,  //  0016  MUL	R11	R8	R11
      0x5431FFFB,  //  0017  LDINT	R12	-4
      0x7C240600,  //  0018  CALL	R9	3
      0x542A0017,  //  0019  LDINT	R10	24
      0x3C28120A,  //  001A  SHR	R10	R9	R10
      0x542E00FE,  //  001B  LDINT	R11	255
      0x2C28140B,  //  001C  AND	R10	R10	R11
      0x242C1500,  //  001D  GT	R11	R10	K0
      0x782E001C,  //  001E  JMPF	R11	#003C
      0xB82E3A00,  //  001F  GETNGBL	R11	K29
      0x8C2C171E,  //  0020  GETMET	R11	R11	K30
      0x5C340400,  //  0021  MOVE	R13	R2
      0x58380000,  //  0022  LDCONST	R14	K0
      0x543E00FE,  //  0023  LDINT	R15	255
      0x58400008,  //  0024  LDCONST	R16	K8
      0x54460013,  //  0025  LDINT	R17	20
      0x7C2C0C00,  //  0026  CALL	R11	6
      0x1830140B,  //  0027  LE	R12	R10	R11
      0x78320007,  //  0028  JMPF	R12	#0031
      0x88300102,  //  0029  GETMBR	R12	R0	K2
      0x8C301913,  //  002A  GETMET	R12	R12	K19
      0x543A0003,  //  002B  LDINT	R14	4
      0x0838100E,  //  002C  MUL	R14	R8	R14
      0x583C0000,  //  002D  LDCONST	R15	K0
      0x5441FFFB,  //  002E  LDINT	R16	-4
      0x7C300800,  //  002F  CALL	R12	4
      0x7002000A,  //  0030  JMP		#003C
      0x0430140B,  //  0031  SUB	R12	R10	R11
      0x2C34131F,  //  0032  AND	R13	R9	K31
      0x88380102,  //  0033  GETMBR	R14	R0	K2
      0x8C381D13,  //  0034  GETMET	R14	R14	K19
      0x54420003,  //  0035  LDINT	R16	4
      0x08401010,  //  0036  MUL	R16	R8	R16
      0x54460017,  //  0037  LDINT	R17	24
      0x38441811,  //  0038  SHL	R17	R12	R17
      0x3044220D,  //  0039  OR	R17	R17	R13
      0x5449FFFB,  //  003A  LDINT	R18	-4
      0x7C380800,  //  003B  CALL	R14	4
      0x00201108,  //  003C  ADD	R8	R8	K8
      0x7001FFD2,  //  003D  JMP		#0011
      0x58240000,  //  003E  LDCONST	R9	K0
      0x14281207,  //  003F  LT	R10	R9	R7
      0x782A0031,  //  0040  JMPF	R10	#0073
      0x88280102,  //  0041  GETMBR	R10	R0	K2
      0x8C281506,  //  0042  GETMET	R10	R10	K6
      0x54320003,  //  0043  LDINT	R12	4
      0x0830120C,  //  0044  MUL	R12	R9	R12
      0x5435FFFB,  //  0045  LDINT	R13	-4
      0x7C280600,  //  0046  CALL	R10	3
      0x542E0017,  //  0047  LDINT	R11	24
      0x3C2C140B,  //  0048  SHR	R11	R10	R11
      0x543200FE,  //  0049  LDINT	R12	255
      0x2C2C160C,  //  004A  AND	R11	R11	R12
      0x1C301700,  //  004B  EQ	R12	R11	K0
      0x78320023,  //  004C  JMPF	R12	#0071
      0x8C300120,  //  004D  GETMET	R12	R0	K32
      0x543A00FE,  //  004E  LDINT	R14	255
      0x7C300400,  //  004F  CALL	R12	2
      0x14301803,  //  0050  LT	R12	R12	R3
      0x7832001E,  //  0051  JMPF	R12	#0071
      0x8C300120,  //  0052  GETMET	R12	R0	K32
      0x04380A04,  //  0053  SUB	R14	R5	R4
      0x00381D08,  //  0054  ADD	R14	R14	K8
      0x7C300400,  //  0055  CALL	R12	2
      0x0030080C,  //  0056  ADD	R12	R4	R12
      0x5C340C00,  //  0057  MOVE	R13	R6
      0x543A000F,  //  0058  LDINT	R14	16
      0x3C381A0E,  //  0059  SHR	R14	R13	R14
      0x543E00FE,  //  005A  LDINT	R15	255
      0x2C381C0F,  //  005B  AND	R14	R14	R15
      0x543E0007,  //  005C  LDINT	R15	8
      0x3C3C1A0F,  //  005D  SHR	R15	R13	R15
      0x544200FE,  //  005E  LDINT	R16	255
      0x2C3C1E10,  //  005F  AND	R15	R15	R16
      0x544200FE,  //  0060  LDINT	R16	255
      0x2C401A10,  //  0061  AND	R16	R13	R16
      0x88440102,  //  0062  GETMBR	R17	R0	K2
      0x8C442313,  //  0063  GETMET	R17	R17	K19
      0x544E0003,  //  0064  LDINT	R19	4
      0x084C1213,  //  0065  MUL	R19	R9	R19
      0x54520017,  //  0066  LDINT	R20	24
      0x38501814,  //  0067  SHL	R20	R12	R20
      0x5456000F,  //  0068  LDINT	R21	16
      0x38541C15,  //  0069  SHL	R21	R14	R21
      0x30502815,  //  006A  OR	R20	R20	R21
      0x54560007,  //  006B  LDINT	R21	8
      0x38541E15,  //  006C  SHL	R21	R15	R21
      0x30502815,  //  006D  OR	R20	R20	R21
      0x30502810,  //  006E  OR	R20	R20	R16
      0x5455FFFB,  //  006F  LDINT	R21	-4
      0x7C440800,  //  0070  CALL	R17	4
      0x00241308,  //  0071  ADD	R9	R9	K8
      0x7001FFCB,  //  0072  JMP		#003F
      0x80000000,  //  0073  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: twinkle
********************************************************************/
extern const bclass be_class_Animation;
be_local_class(twinkle,
    3,
    &be_class_Animation,
    be_nested_map(12,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(last_update, -1), be_const_var(1) },
        { be_const_key_weak(render, 8), be_const_closure(class_twinkle_render_closure) },
        { be_const_key_weak(_update_twinkle_simulation, -1), be_const_closure(class_twinkle__update_twinkle_simulation_closure) },
        { be_const_key_weak(PARAMS, 6), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(twinkle_speed, 1), be_const_bytes_instance(0700010188130064) },
        { be_const_key_weak(min_brightness, -1), be_const_bytes_instance(07000001FF000020) },
        { be_const_key_weak(density, -1), be_const_bytes_instance(07000001FF000040) },
        { be_const_key_weak(max_brightness, 2), be_const_bytes_instance(07000001FF0001FF00) },
        { be_const_key_weak(color, -1), be_const_bytes_instance(0400BB) },
        { be_const_key_weak(fade_speed, 0), be_const_bytes_instance(07000001FF0001B400) },
    }))    ) } )) },
        { be_const_key_weak(update, -1), be_const_closure(class_twinkle_update_closure) },
        { be_const_key_weak(_random, -1), be_const_closure(class_twinkle__random_closure) },
        { be_const_key_weak(init, 9), be_const_closure(class_twinkle_init_closure) },
        { be_const_key_weak(_initialize_arrays, -1), be_const_closure(class_twinkle__initialize_arrays_closure) },
        { be_const_key_weak(on_param_changed, -1), be_const_closure(class_twinkle_on_param_changed_closure) },
        { be_const_key_weak(current_colors, -1), be_const_var(0) },
        { be_const_key_weak(random_seed, 2), be_const_var(2) },
        { be_const_key_weak(_random_range, 0), be_const_closure(class_twinkle__random_range_closure) },
    })),
    be_str_weak(twinkle)
);

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
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    }),
    be_str_weak(solid),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040300,  //  0001  GETMET	R1	R1	K0
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x80040200,  //  0004  RET	1	R1
    })
  )
);
/*******************************************************************/

// compact class 'palette_meter' ktab size: 26, total: 34 (saved 64 bytes)
static const bvalue be_ktab_class_palette_meter[26] = {
  /* K0   */  be_nested_str_weak(peak_hold),
  /* K1   */  be_const_int(0),
  /* K2   */  be_nested_str_weak(level),
  /* K3   */  be_nested_str_weak(_level),
  /* K4   */  be_nested_str_weak(peak_level),
  /* K5   */  be_nested_str_weak(peak_time),
  /* K6   */  be_nested_str_weak(update),
  /* K7   */  be_nested_str_weak(get_param),
  /* K8   */  be_nested_str_weak(color_source),
  /* K9   */  be_nested_str_weak(start_time),
  /* K10  */  be_nested_str_weak(tasmota),
  /* K11  */  be_nested_str_weak(scale_uint),
  /* K12  */  be_const_int(1),
  /* K13  */  be_nested_str_weak(animation),
  /* K14  */  be_nested_str_weak(color_provider),
  /* K15  */  be_nested_str_weak(get_lut),
  /* K16  */  be_nested_str_weak(LUT_FACTOR),
  /* K17  */  be_nested_str_weak(pixels),
  /* K18  */  be_nested_str_weak(_buffer),
  /* K19  */  be_nested_str_weak(value_buffer),
  /* K20  */  be_const_int(2),
  /* K21  */  be_const_int(3),
  /* K22  */  be_nested_str_weak(get_color_for_value),
  /* K23  */  be_nested_str_weak(set_pixel_color),
  /* K24  */  be_nested_str_weak(init),
  /* K25  */  be_nested_str_weak(shift_period),
};


extern const bclass be_class_palette_meter;

/********************************************************************
** Solidified function: update
********************************************************************/
be_local_closure(class_palette_meter_update,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_palette_meter,     /* shared constants */
    be_str_weak(update),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x240C0501,  //  0001  GT	R3	R2	K1
      0x780E000F,  //  0002  JMPF	R3	#0013
      0x880C0102,  //  0003  GETMBR	R3	R0	K2
      0x90020603,  //  0004  SETMBR	R0	K3	R3
      0x88100104,  //  0005  GETMBR	R4	R0	K4
      0x28140604,  //  0006  GE	R5	R3	R4
      0x78160002,  //  0007  JMPF	R5	#000B
      0x90020803,  //  0008  SETMBR	R0	K4	R3
      0x90020A01,  //  0009  SETMBR	R0	K5	R1
      0x70020007,  //  000A  JMP		#0013
      0x24140901,  //  000B  GT	R5	R4	K1
      0x78160005,  //  000C  JMPF	R5	#0013
      0x88140105,  //  000D  GETMBR	R5	R0	K5
      0x04140205,  //  000E  SUB	R5	R1	R5
      0x24180A02,  //  000F  GT	R6	R5	R2
      0x781A0001,  //  0010  JMPF	R6	#0013
      0x90020803,  //  0011  SETMBR	R0	K4	R3
      0x90020A01,  //  0012  SETMBR	R0	K5	R1
      0x600C0003,  //  0013  GETGBL	R3	G3
      0x5C100000,  //  0014  MOVE	R4	R0
      0x7C0C0200,  //  0015  CALL	R3	1
      0x8C0C0706,  //  0016  GETMET	R3	R3	K6
      0x5C140200,  //  0017  MOVE	R5	R1
      0x7C0C0400,  //  0018  CALL	R3	2
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: render
********************************************************************/
be_local_closure(class_palette_meter_render,   /* name */
  be_nested_proto(
    21,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_palette_meter,     /* shared constants */
    be_str_weak(render),
    &be_const_str_solidified,
    ( &(const binstruction[113]) {  /* code */
      0x8C100107,  //  0000  GETMET	R4	R0	K7
      0x58180008,  //  0001  LDCONST	R6	K8
      0x7C100400,  //  0002  CALL	R4	2
      0x4C140000,  //  0003  LDNIL	R5
      0x1C140805,  //  0004  EQ	R5	R4	R5
      0x78160001,  //  0005  JMPF	R5	#0008
      0x50140000,  //  0006  LDBOOL	R5	0	0
      0x80040A00,  //  0007  RET	1	R5
      0x88140109,  //  0008  GETMBR	R5	R0	K9
      0x04140405,  //  0009  SUB	R5	R2	R5
      0x88180103,  //  000A  GETMBR	R6	R0	K3
      0x881C0100,  //  000B  GETMBR	R7	R0	K0
      0xB8221400,  //  000C  GETNGBL	R8	K10
      0x8C20110B,  //  000D  GETMET	R8	R8	K11
      0x5C280C00,  //  000E  MOVE	R10	R6
      0x582C0001,  //  000F  LDCONST	R11	K1
      0x543200FE,  //  0010  LDINT	R12	255
      0x58340001,  //  0011  LDCONST	R13	K1
      0x5C380600,  //  0012  MOVE	R14	R3
      0x7C200C00,  //  0013  CALL	R8	6
      0x5425FFFE,  //  0014  LDINT	R9	-1
      0x24280F01,  //  0015  GT	R10	R7	K1
      0x782A000C,  //  0016  JMPF	R10	#0024
      0x88280104,  //  0017  GETMBR	R10	R0	K4
      0x24281406,  //  0018  GT	R10	R10	R6
      0x782A0009,  //  0019  JMPF	R10	#0024
      0xB82A1400,  //  001A  GETNGBL	R10	K10
      0x8C28150B,  //  001B  GETMET	R10	R10	K11
      0x88300104,  //  001C  GETMBR	R12	R0	K4
      0x58340001,  //  001D  LDCONST	R13	K1
      0x543A00FE,  //  001E  LDINT	R14	255
      0x583C0001,  //  001F  LDCONST	R15	K1
      0x5C400600,  //  0020  MOVE	R16	R3
      0x7C280C00,  //  0021  CALL	R10	6
      0x0428150C,  //  0022  SUB	R10	R10	K12
      0x5C241400,  //  0023  MOVE	R9	R10
      0x4C280000,  //  0024  LDNIL	R10
      0x602C000F,  //  0025  GETGBL	R11	G15
      0x5C300800,  //  0026  MOVE	R12	R4
      0xB8361A00,  //  0027  GETNGBL	R13	K13
      0x88341B0E,  //  0028  GETMBR	R13	R13	K14
      0x7C2C0400,  //  0029  CALL	R11	2
      0x782E0028,  //  002A  JMPF	R11	#0054
      0x8C2C090F,  //  002B  GETMET	R11	R4	K15
      0x7C2C0200,  //  002C  CALL	R11	1
      0x5C281600,  //  002D  MOVE	R10	R11
      0x4C300000,  //  002E  LDNIL	R12
      0x202C160C,  //  002F  NE	R11	R11	R12
      0x782E0022,  //  0030  JMPF	R11	#0054
      0x882C0910,  //  0031  GETMBR	R11	R4	K16
      0x543200FF,  //  0032  LDINT	R12	256
      0x3C30180B,  //  0033  SHR	R12	R12	R11
      0x58340001,  //  0034  LDCONST	R13	K1
      0x88380311,  //  0035  GETMBR	R14	R1	K17
      0x8C381D12,  //  0036  GETMET	R14	R14	K18
      0x7C380200,  //  0037  CALL	R14	1
      0x8C3C1512,  //  0038  GETMET	R15	R10	K18
      0x7C3C0200,  //  0039  CALL	R15	1
      0x88400113,  //  003A  GETMBR	R16	R0	K19
      0x8C402112,  //  003B  GETMET	R16	R16	K18
      0x7C400200,  //  003C  CALL	R16	1
      0x14441A08,  //  003D  LT	R17	R13	R8
      0x78460013,  //  003E  JMPF	R17	#0053
      0x9444200D,  //  003F  GETIDX	R17	R16	R13
      0x3C48220B,  //  0040  SHR	R18	R17	R11
      0x544E00FE,  //  0041  LDINT	R19	255
      0x1C4C2213,  //  0042  EQ	R19	R17	R19
      0x784E0000,  //  0043  JMPF	R19	#0045
      0x5C481800,  //  0044  MOVE	R18	R12
      0x384C2514,  //  0045  SHL	R19	R18	K20
      0x004C1E13,  //  0046  ADD	R19	R15	R19
      0x94502701,  //  0047  GETIDX	R20	R19	K1
      0x983A0214,  //  0048  SETIDX	R14	K1	R20
      0x9450270C,  //  0049  GETIDX	R20	R19	K12
      0x983A1814,  //  004A  SETIDX	R14	K12	R20
      0x94502714,  //  004B  GETIDX	R20	R19	K20
      0x983A2814,  //  004C  SETIDX	R14	K20	R20
      0x94502715,  //  004D  GETIDX	R20	R19	K21
      0x983A2A14,  //  004E  SETIDX	R14	K21	R20
      0x00341B0C,  //  004F  ADD	R13	R13	K12
      0x54520003,  //  0050  LDINT	R20	4
      0x00381C14,  //  0051  ADD	R14	R14	R20
      0x7001FFE9,  //  0052  JMP		#003D
      0x7002000E,  //  0053  JMP		#0063
      0x582C0001,  //  0054  LDCONST	R11	K1
      0x14301608,  //  0055  LT	R12	R11	R8
      0x7832000B,  //  0056  JMPF	R12	#0063
      0x88300113,  //  0057  GETMBR	R12	R0	K19
      0x9430180B,  //  0058  GETIDX	R12	R12	R11
      0x8C340916,  //  0059  GETMET	R13	R4	K22
      0x5C3C1800,  //  005A  MOVE	R15	R12
      0x5C400A00,  //  005B  MOVE	R16	R5
      0x7C340600,  //  005C  CALL	R13	3
      0x8C380317,  //  005D  GETMET	R14	R1	K23
      0x5C401600,  //  005E  MOVE	R16	R11
      0x5C441A00,  //  005F  MOVE	R17	R13
      0x7C380600,  //  0060  CALL	R14	3
      0x002C170C,  //  0061  ADD	R11	R11	K12
      0x7001FFF1,  //  0062  JMP		#0055
      0x282C1208,  //  0063  GE	R11	R9	R8
      0x782E0009,  //  0064  JMPF	R11	#006F
      0x882C0113,  //  0065  GETMBR	R11	R0	K19
      0x942C1609,  //  0066  GETIDX	R11	R11	R9
      0x8C300916,  //  0067  GETMET	R12	R4	K22
      0x5C381600,  //  0068  MOVE	R14	R11
      0x5C3C0A00,  //  0069  MOVE	R15	R5
      0x7C300600,  //  006A  CALL	R12	3
      0x8C340317,  //  006B  GETMET	R13	R1	K23
      0x5C3C1200,  //  006C  MOVE	R15	R9
      0x5C401800,  //  006D  MOVE	R16	R12
      0x7C340600,  //  006E  CALL	R13	3
      0x502C0200,  //  006F  LDBOOL	R11	1	0
      0x80041600,  //  0070  RET	1	R11
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_palette_meter_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_palette_meter,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080518,  //  0003  GETMET	R2	R2	K24
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x90020901,  //  0006  SETMBR	R0	K4	K1
      0x90020B01,  //  0007  SETMBR	R0	K5	K1
      0x90020701,  //  0008  SETMBR	R0	K3	K1
      0x90023301,  //  0009  SETMBR	R0	K25	K1
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: palette_meter
********************************************************************/
extern const bclass be_class_palette_gradient;
be_local_class(palette_meter,
    3,
    &be_class_palette_gradient,
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(peak_time, -1), be_const_var(1) },
        { be_const_key_weak(_level, 6), be_const_var(2) },
        { be_const_key_weak(init, 3), be_const_closure(class_palette_meter_init_closure) },
        { be_const_key_weak(PARAMS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(peak_hold, -1), be_const_bytes_instance(05000001E803) },
        { be_const_key_weak(level, -1), be_const_bytes_instance(07000001FF0001FF00) },
    }))    ) } )) },
        { be_const_key_weak(peak_level, -1), be_const_var(0) },
        { be_const_key_weak(render, 2), be_const_closure(class_palette_meter_render_closure) },
        { be_const_key_weak(update, -1), be_const_closure(class_palette_meter_update_closure) },
    })),
    be_str_weak(palette_meter)
);

extern const bclass be_class_gradient;

/********************************************************************
** Solidified function: render
********************************************************************/
be_local_closure(class_gradient_render,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(color1),
    /* K1   */  be_nested_str_weak(color2),
    /* K2   */  be_nested_str_weak(direction),
    /* K3   */  be_nested_str_weak(gradient_type),
    /* K4   */  be_nested_str_weak(color),
    /* K5   */  be_nested_str_weak(back_color),
    /* K6   */  be_const_int(1),
    /* K7   */  be_const_int(2),
    /* K8   */  be_nested_str_weak(pos),
    /* K9   */  be_nested_str_weak(beacon_size),
    /* K10  */  be_nested_str_weak(slew_size),
    /* K11  */  be_const_int(0),
    /* K12  */  be_nested_str_weak(right_edge),
    /* K13  */  be_nested_str_weak(render),
    }),
    be_str_weak(render),
    &be_const_str_solidified,
    ( &(const binstruction[45]) {  /* code */
      0x88100100,  //  0000  GETMBR	R4	R0	K0
      0x88140101,  //  0001  GETMBR	R5	R0	K1
      0x88180102,  //  0002  GETMBR	R6	R0	K2
      0x881C0103,  //  0003  GETMBR	R7	R0	K3
      0x781A0002,  //  0004  JMPF	R6	#0008
      0x90020805,  //  0005  SETMBR	R0	K4	R5
      0x90020A04,  //  0006  SETMBR	R0	K5	R4
      0x70020001,  //  0007  JMP		#000A
      0x90020804,  //  0008  SETMBR	R0	K4	R4
      0x90020A05,  //  0009  SETMBR	R0	K5	R5
      0x781E000E,  //  000A  JMPF	R7	#001A
      0x04200706,  //  000B  SUB	R8	R3	K6
      0x0C201107,  //  000C  DIV	R8	R8	K7
      0x90021008,  //  000D  SETMBR	R0	K8	R8
      0x04260C03,  //  000E  SUB	R9	K6	R3
      0x2C241306,  //  000F  AND	R9	R9	K6
      0x00260C09,  //  0010  ADD	R9	K6	R9
      0x90021209,  //  0011  SETMBR	R0	K9	R9
      0x2424110B,  //  0012  GT	R9	R8	K11
      0x78260001,  //  0013  JMPF	R9	#0016
      0x04241106,  //  0014  SUB	R9	R8	K6
      0x70020000,  //  0015  JMP		#0017
      0x5824000B,  //  0016  LDCONST	R9	K11
      0x90021409,  //  0017  SETMBR	R0	K10	R9
      0x9002190B,  //  0018  SETMBR	R0	K12	K11
      0x70020009,  //  0019  JMP		#0024
      0x9002110B,  //  001A  SETMBR	R0	K8	K11
      0x542203E7,  //  001B  LDINT	R8	1000
      0x90021208,  //  001C  SETMBR	R0	K9	R8
      0x24200706,  //  001D  GT	R8	R3	K6
      0x78220001,  //  001E  JMPF	R8	#0021
      0x04200707,  //  001F  SUB	R8	R3	K7
      0x70020000,  //  0020  JMP		#0022
      0x5820000B,  //  0021  LDCONST	R8	K11
      0x90021408,  //  0022  SETMBR	R0	K10	R8
      0x90021906,  //  0023  SETMBR	R0	K12	K6
      0x60200003,  //  0024  GETGBL	R8	G3
      0x5C240000,  //  0025  MOVE	R9	R0
      0x7C200200,  //  0026  CALL	R8	1
      0x8C20110D,  //  0027  GETMET	R8	R8	K13
      0x5C280200,  //  0028  MOVE	R10	R1
      0x5C2C0400,  //  0029  MOVE	R11	R2
      0x5C300600,  //  002A  MOVE	R12	R3
      0x7C200800,  //  002B  CALL	R8	4
      0x80041000,  //  002C  RET	1	R8
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: gradient
********************************************************************/
extern const bclass be_class_beacon;
be_local_class(gradient,
    0,
    &be_class_beacon,
    be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(render, -1), be_const_closure(class_gradient_render_closure) },
        { be_const_key_weak(PARAMS, 0), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(color2, -1), be_const_bytes_instance(0402FF0000FF) },
        { be_const_key_weak(gradient_type, 0), be_const_bytes_instance(1400000200000001) },
        { be_const_key_weak(direction, 1), be_const_bytes_instance(1400000200000001) },
        { be_const_key_weak(color1, -1), be_const_bytes_instance(04020000FFFF) },
    }))    ) } )) },
    })),
    be_str_weak(gradient)
);
// compact class 'oscillator_value' ktab size: 23, total: 25 (saved 16 bytes)
static const bvalue be_ktab_class_oscillator_value[23] = {
  /* K0   */  be_nested_str_weak(member),
  /* K1   */  be_nested_str_weak(duration),
  /* K2   */  be_nested_str_weak(min_value),
  /* K3   */  be_nested_str_weak(max_value),
  /* K4   */  be_nested_str_weak(form),
  /* K5   */  be_nested_str_weak(phase),
  /* K6   */  be_nested_str_weak(duty_cycle),
  /* K7   */  be_nested_str_weak(tasmota),
  /* K8   */  be_nested_str_weak(scale_uint),
  /* K9   */  be_nested_str_weak(scale_int),
  /* K10  */  be_nested_str_weak(_fix_time_ms),
  /* K11  */  be_const_int(0),
  /* K12  */  be_nested_str_weak(start_time),
  /* K13  */  be_const_int(3),
  /* K14  */  be_const_int(2),
  /* K15  */  be_const_int(1),
  /* K16  */  be_nested_str_weak(sine_int),
  /* K17  */  be_nested_str_weak(value),
  /* K18  */  be_const_int(196602),
  /* K19  */  be_const_int(-1044480),
  /* K20  */  be_const_int(1044480),
  /* K21  */  be_nested_str_weak(init),
  /* K22  */  be_nested_str_weak(start),
};


extern const bclass be_class_oscillator_value;

/********************************************************************
** Solidified function: produce_value
********************************************************************/
be_local_closure(class_oscillator_value_produce_value,   /* name */
  be_nested_proto(
    26,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_oscillator_value,     /* shared constants */
    be_str_weak(produce_value),
    &be_const_str_solidified,
    ( &(const binstruction[335]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x5C100600,  //  0001  MOVE	R4	R3
      0x5C140000,  //  0002  MOVE	R5	R0
      0x58180001,  //  0003  LDCONST	R6	K1
      0x7C100400,  //  0004  CALL	R4	2
      0x5C140600,  //  0005  MOVE	R5	R3
      0x5C180000,  //  0006  MOVE	R6	R0
      0x581C0002,  //  0007  LDCONST	R7	K2
      0x7C140400,  //  0008  CALL	R5	2
      0x5C180600,  //  0009  MOVE	R6	R3
      0x5C1C0000,  //  000A  MOVE	R7	R0
      0x58200003,  //  000B  LDCONST	R8	K3
      0x7C180400,  //  000C  CALL	R6	2
      0x5C1C0600,  //  000D  MOVE	R7	R3
      0x5C200000,  //  000E  MOVE	R8	R0
      0x58240004,  //  000F  LDCONST	R9	K4
      0x7C1C0400,  //  0010  CALL	R7	2
      0x5C200600,  //  0011  MOVE	R8	R3
      0x5C240000,  //  0012  MOVE	R9	R0
      0x58280005,  //  0013  LDCONST	R10	K5
      0x7C200400,  //  0014  CALL	R8	2
      0x5C240600,  //  0015  MOVE	R9	R3
      0x5C280000,  //  0016  MOVE	R10	R0
      0x582C0006,  //  0017  LDCONST	R11	K6
      0x7C240400,  //  0018  CALL	R9	2
      0xB82A0E00,  //  0019  GETNGBL	R10	K7
      0x88281508,  //  001A  GETMBR	R10	R10	K8
      0xB82E0E00,  //  001B  GETNGBL	R11	K7
      0x882C1709,  //  001C  GETMBR	R11	R11	K9
      0x8C30010A,  //  001D  GETMET	R12	R0	K10
      0x5C380400,  //  001E  MOVE	R14	R2
      0x7C300400,  //  001F  CALL	R12	2
      0x5C081800,  //  0020  MOVE	R2	R12
      0x4C300000,  //  0021  LDNIL	R12
      0x1C30080C,  //  0022  EQ	R12	R4	R12
      0x74320001,  //  0023  JMPT	R12	#0026
      0x1830090B,  //  0024  LE	R12	R4	K11
      0x78320000,  //  0025  JMPF	R12	#0027
      0x80040A00,  //  0026  RET	1	R5
      0x8830010C,  //  0027  GETMBR	R12	R0	K12
      0x0430040C,  //  0028  SUB	R12	R2	R12
      0x1434190B,  //  0029  LT	R13	R12	K11
      0x78360000,  //  002A  JMPF	R13	#002C
      0x5830000B,  //  002B  LDCONST	R12	K11
      0x28341804,  //  002C  GE	R13	R12	R4
      0x78360005,  //  002D  JMPF	R13	#0034
      0x0C381804,  //  002E  DIV	R14	R12	R4
      0x08381C04,  //  002F  MUL	R14	R14	R4
      0x8834010C,  //  0030  GETMBR	R13	R0	K12
      0x00341A0E,  //  0031  ADD	R13	R13	R14
      0x9002180D,  //  0032  SETMBR	R0	K12	R13
      0x10301804,  //  0033  MOD	R12	R12	R4
      0x2434110B,  //  0034  GT	R13	R8	K11
      0x7836000A,  //  0035  JMPF	R13	#0041
      0x5C341400,  //  0036  MOVE	R13	R10
      0x5C381000,  //  0037  MOVE	R14	R8
      0x583C000B,  //  0038  LDCONST	R15	K11
      0x544200FE,  //  0039  LDINT	R16	255
      0x5844000B,  //  003A  LDCONST	R17	K11
      0x5C480800,  //  003B  MOVE	R18	R4
      0x7C340A00,  //  003C  CALL	R13	5
      0x0030180D,  //  003D  ADD	R12	R12	R13
      0x28341804,  //  003E  GE	R13	R12	R4
      0x78360000,  //  003F  JMPF	R13	#0041
      0x04301804,  //  0040  SUB	R12	R12	R4
      0x4C340000,  //  0041  LDNIL	R13
      0x5C381400,  //  0042  MOVE	R14	R10
      0x5C3C1200,  //  0043  MOVE	R15	R9
      0x5840000B,  //  0044  LDCONST	R16	K11
      0x544600FE,  //  0045  LDINT	R17	255
      0x5848000B,  //  0046  LDCONST	R18	K11
      0x5C4C0800,  //  0047  MOVE	R19	R4
      0x7C380A00,  //  0048  CALL	R14	5
      0x1C3C0F0D,  //  0049  EQ	R15	R7	K13
      0x783E0006,  //  004A  JMPF	R15	#0052
      0x143C180E,  //  004B  LT	R15	R12	R14
      0x783E0001,  //  004C  JMPF	R15	#004F
      0x5C3C0A00,  //  004D  MOVE	R15	R5
      0x70020000,  //  004E  JMP		#0050
      0x5C3C0C00,  //  004F  MOVE	R15	R6
      0x5C341E00,  //  0050  MOVE	R13	R15
      0x700200FA,  //  0051  JMP		#014D
      0x1C3C0F0E,  //  0052  EQ	R15	R7	K14
      0x783E0013,  //  0053  JMPF	R15	#0068
      0x143C180E,  //  0054  LT	R15	R12	R14
      0x783E0008,  //  0055  JMPF	R15	#005F
      0x5C3C1400,  //  0056  MOVE	R15	R10
      0x5C401800,  //  0057  MOVE	R16	R12
      0x5844000B,  //  0058  LDCONST	R17	K11
      0x04481D0F,  //  0059  SUB	R18	R14	K15
      0x5C4C0A00,  //  005A  MOVE	R19	R5
      0x5C500C00,  //  005B  MOVE	R20	R6
      0x7C3C0A00,  //  005C  CALL	R15	5
      0x5C341E00,  //  005D  MOVE	R13	R15
      0x70020007,  //  005E  JMP		#0067
      0x5C3C1400,  //  005F  MOVE	R15	R10
      0x5C401800,  //  0060  MOVE	R16	R12
      0x5C441C00,  //  0061  MOVE	R17	R14
      0x0448090F,  //  0062  SUB	R18	R4	K15
      0x5C4C0C00,  //  0063  MOVE	R19	R6
      0x5C500A00,  //  0064  MOVE	R20	R5
      0x7C3C0A00,  //  0065  CALL	R15	5
      0x5C341E00,  //  0066  MOVE	R13	R15
      0x700200E4,  //  0067  JMP		#014D
      0x543E0003,  //  0068  LDINT	R15	4
      0x1C3C0E0F,  //  0069  EQ	R15	R7	R15
      0x743E0002,  //  006A  JMPT	R15	#006E
      0x543E0004,  //  006B  LDINT	R15	5
      0x1C3C0E0F,  //  006C  EQ	R15	R7	R15
      0x783E0017,  //  006D  JMPF	R15	#0086
      0x5C3C1400,  //  006E  MOVE	R15	R10
      0x5C401800,  //  006F  MOVE	R16	R12
      0x5844000B,  //  0070  LDCONST	R17	K11
      0x0448090F,  //  0071  SUB	R18	R4	K15
      0x584C000B,  //  0072  LDCONST	R19	K11
      0x54527FFE,  //  0073  LDINT	R20	32767
      0x7C3C0A00,  //  0074  CALL	R15	5
      0x54420003,  //  0075  LDINT	R16	4
      0x1C400E10,  //  0076  EQ	R16	R7	R16
      0x78420001,  //  0077  JMPF	R16	#007A
      0x54421FFF,  //  0078  LDINT	R16	8192
      0x043C1E10,  //  0079  SUB	R15	R15	R16
      0x5C401600,  //  007A  MOVE	R16	R11
      0xB8460E00,  //  007B  GETNGBL	R17	K7
      0x8C442310,  //  007C  GETMET	R17	R17	K16
      0x5C4C1E00,  //  007D  MOVE	R19	R15
      0x7C440400,  //  007E  CALL	R17	2
      0x5449EFFF,  //  007F  LDINT	R18	-4096
      0x544E0FFF,  //  0080  LDINT	R19	4096
      0x5C500A00,  //  0081  MOVE	R20	R5
      0x5C540C00,  //  0082  MOVE	R21	R6
      0x7C400A00,  //  0083  CALL	R16	5
      0x5C342000,  //  0084  MOVE	R13	R16
      0x700200C6,  //  0085  JMP		#014D
      0x543E0005,  //  0086  LDINT	R15	6
      0x1C3C0E0F,  //  0087  EQ	R15	R7	R15
      0x743E0002,  //  0088  JMPT	R15	#008C
      0x543E0006,  //  0089  LDINT	R15	7
      0x1C3C0E0F,  //  008A  EQ	R15	R7	R15
      0x783E001F,  //  008B  JMPF	R15	#00AC
      0x5C3C1400,  //  008C  MOVE	R15	R10
      0x5C401800,  //  008D  MOVE	R16	R12
      0x5844000B,  //  008E  LDCONST	R17	K11
      0x0448090F,  //  008F  SUB	R18	R4	K15
      0x584C000B,  //  0090  LDCONST	R19	K11
      0x545200FE,  //  0091  LDINT	R20	255
      0x7C3C0A00,  //  0092  CALL	R15	5
      0x54420005,  //  0093  LDINT	R16	6
      0x1C400E10,  //  0094  EQ	R16	R7	R16
      0x78420008,  //  0095  JMPF	R16	#009F
      0x5C401600,  //  0096  MOVE	R16	R11
      0x08441E0F,  //  0097  MUL	R17	R15	R15
      0x5848000B,  //  0098  LDCONST	R18	K11
      0x544EFE00,  //  0099  LDINT	R19	65025
      0x5C500A00,  //  009A  MOVE	R20	R5
      0x5C540C00,  //  009B  MOVE	R21	R6
      0x7C400A00,  //  009C  CALL	R16	5
      0x5C342000,  //  009D  MOVE	R13	R16
      0x7002000B,  //  009E  JMP		#00AB
      0x544200FE,  //  009F  LDINT	R16	255
      0x0440200F,  //  00A0  SUB	R16	R16	R15
      0x5C441600,  //  00A1  MOVE	R17	R11
      0x544AFE00,  //  00A2  LDINT	R18	65025
      0x084C2010,  //  00A3  MUL	R19	R16	R16
      0x04482413,  //  00A4  SUB	R18	R18	R19
      0x584C000B,  //  00A5  LDCONST	R19	K11
      0x5452FE00,  //  00A6  LDINT	R20	65025
      0x5C540A00,  //  00A7  MOVE	R21	R5
      0x5C580C00,  //  00A8  MOVE	R22	R6
      0x7C440A00,  //  00A9  CALL	R17	5
      0x5C342200,  //  00AA  MOVE	R13	R17
      0x700200A0,  //  00AB  JMP		#014D
      0x543E0007,  //  00AC  LDINT	R15	8
      0x1C3C0E0F,  //  00AD  EQ	R15	R7	R15
      0x783E0042,  //  00AE  JMPF	R15	#00F2
      0x5C3C1400,  //  00AF  MOVE	R15	R10
      0x5C401800,  //  00B0  MOVE	R16	R12
      0x5844000B,  //  00B1  LDCONST	R17	K11
      0x0448090F,  //  00B2  SUB	R18	R4	K15
      0x584C000B,  //  00B3  LDCONST	R19	K11
      0x545200FE,  //  00B4  LDINT	R20	255
      0x7C3C0A00,  //  00B5  CALL	R15	5
      0x1C401F0B,  //  00B6  EQ	R16	R15	K11
      0x78420001,  //  00B7  JMPF	R16	#00BA
      0x90022205,  //  00B8  SETMBR	R0	K17	R5
      0x80040A00,  //  00B9  RET	1	R5
      0x544200FE,  //  00BA  LDINT	R16	255
      0x1C401E10,  //  00BB  EQ	R16	R15	R16
      0x78420001,  //  00BC  JMPF	R16	#00BF
      0x90022206,  //  00BD  SETMBR	R0	K17	R6
      0x80040C00,  //  00BE  RET	1	R6
      0x5C401400,  //  00BF  MOVE	R16	R10
      0x544600FE,  //  00C0  LDINT	R17	255
      0x0444220F,  //  00C1  SUB	R17	R17	R15
      0x5848000B,  //  00C2  LDCONST	R18	K11
      0x544E00FE,  //  00C3  LDINT	R19	255
      0x545200FE,  //  00C4  LDINT	R20	255
      0x5456001F,  //  00C5  LDINT	R21	32
      0x7C400A00,  //  00C6  CALL	R16	5
      0xB8460E00,  //  00C7  GETNGBL	R17	K7
      0x8C442310,  //  00C8  GETMET	R17	R17	K16
      0x5C4C1400,  //  00C9  MOVE	R19	R10
      0x5C501E00,  //  00CA  MOVE	R20	R15
      0x5854000B,  //  00CB  LDCONST	R21	K11
      0x545A00FE,  //  00CC  LDINT	R22	255
      0x585C000B,  //  00CD  LDCONST	R23	K11
      0x58600012,  //  00CE  LDCONST	R24	K18
      0x7C4C0A00,  //  00CF  CALL	R19	5
      0x54527FFE,  //  00D0  LDINT	R20	32767
      0x104C2614,  //  00D1  MOD	R19	R19	R20
      0x7C440400,  //  00D2  CALL	R17	2
      0x04480C05,  //  00D3  SUB	R18	R6	R5
      0x5C4C1600,  //  00D4  MOVE	R19	R11
      0x08502210,  //  00D5  MUL	R20	R17	R16
      0x58540013,  //  00D6  LDCONST	R21	K19
      0x58580014,  //  00D7  LDCONST	R22	K20
      0x445C2400,  //  00D8  NEG	R23	R18
      0x5C602400,  //  00D9  MOVE	R24	R18
      0x7C4C0A00,  //  00DA  CALL	R19	5
      0x5C501600,  //  00DB  MOVE	R20	R11
      0x5C541E00,  //  00DC  MOVE	R21	R15
      0x5858000B,  //  00DD  LDCONST	R22	K11
      0x545E00FE,  //  00DE  LDINT	R23	255
      0x5860000B,  //  00DF  LDCONST	R24	K11
      0x5C642400,  //  00E0  MOVE	R25	R18
      0x7C500A00,  //  00E1  CALL	R20	5
      0x00500A14,  //  00E2  ADD	R20	R5	R20
      0x00502813,  //  00E3  ADD	R20	R20	R19
      0x5C342800,  //  00E4  MOVE	R13	R20
      0x54520003,  //  00E5  LDINT	R20	4
      0x0C502414,  //  00E6  DIV	R20	R18	R20
      0x00540C14,  //  00E7  ADD	R21	R6	R20
      0x24541A15,  //  00E8  GT	R21	R13	R21
      0x78560001,  //  00E9  JMPF	R21	#00EC
      0x00540C14,  //  00EA  ADD	R21	R6	R20
      0x5C342A00,  //  00EB  MOVE	R13	R21
      0x04540A14,  //  00EC  SUB	R21	R5	R20
      0x14541A15,  //  00ED  LT	R21	R13	R21
      0x78560001,  //  00EE  JMPF	R21	#00F1
      0x04540A14,  //  00EF  SUB	R21	R5	R20
      0x5C342A00,  //  00F0  MOVE	R13	R21
      0x7002005A,  //  00F1  JMP		#014D
      0x543E0008,  //  00F2  LDINT	R15	9
      0x1C3C0E0F,  //  00F3  EQ	R15	R7	R15
      0x783E004F,  //  00F4  JMPF	R15	#0145
      0x5C3C1400,  //  00F5  MOVE	R15	R10
      0x5C401800,  //  00F6  MOVE	R16	R12
      0x5844000B,  //  00F7  LDCONST	R17	K11
      0x0448090F,  //  00F8  SUB	R18	R4	K15
      0x584C000B,  //  00F9  LDCONST	R19	K11
      0x545200FE,  //  00FA  LDINT	R20	255
      0x7C3C0A00,  //  00FB  CALL	R15	5
      0x04400C05,  //  00FC  SUB	R16	R6	R5
      0x5446007F,  //  00FD  LDINT	R17	128
      0x14441E11,  //  00FE  LT	R17	R15	R17
      0x78460014,  //  00FF  JMPF	R17	#0115
      0x5C441400,  //  0100  MOVE	R17	R10
      0x5C481E00,  //  0101  MOVE	R18	R15
      0x584C000B,  //  0102  LDCONST	R19	K11
      0x5452007E,  //  0103  LDINT	R20	127
      0x5854000B,  //  0104  LDCONST	R21	K11
      0x545A00FE,  //  0105  LDINT	R22	255
      0x7C440A00,  //  0106  CALL	R17	5
      0x5C481600,  //  0107  MOVE	R18	R11
      0x544E00FE,  //  0108  LDINT	R19	255
      0x044C2611,  //  0109  SUB	R19	R19	R17
      0x545200FE,  //  010A  LDINT	R20	255
      0x04502811,  //  010B  SUB	R20	R20	R17
      0x084C2614,  //  010C  MUL	R19	R19	R20
      0x5850000B,  //  010D  LDCONST	R20	K11
      0x5456FE00,  //  010E  LDINT	R21	65025
      0x5858000B,  //  010F  LDCONST	R22	K11
      0x5C5C2000,  //  0110  MOVE	R23	R16
      0x7C480A00,  //  0111  CALL	R18	5
      0x04480C12,  //  0112  SUB	R18	R6	R18
      0x5C342400,  //  0113  MOVE	R13	R18
      0x7002002E,  //  0114  JMP		#0144
      0x544600BF,  //  0115  LDINT	R17	192
      0x14441E11,  //  0116  LT	R17	R15	R17
      0x78460015,  //  0117  JMPF	R17	#012E
      0x5C441400,  //  0118  MOVE	R17	R10
      0x544A007F,  //  0119  LDINT	R18	128
      0x04481E12,  //  011A  SUB	R18	R15	R18
      0x584C000B,  //  011B  LDCONST	R19	K11
      0x5452003E,  //  011C  LDINT	R20	63
      0x5854000B,  //  011D  LDCONST	R21	K11
      0x545A00FE,  //  011E  LDINT	R22	255
      0x7C440A00,  //  011F  CALL	R17	5
      0x5C481600,  //  0120  MOVE	R18	R11
      0x544E00FE,  //  0121  LDINT	R19	255
      0x044C2611,  //  0122  SUB	R19	R19	R17
      0x545200FE,  //  0123  LDINT	R20	255
      0x04502811,  //  0124  SUB	R20	R20	R17
      0x084C2614,  //  0125  MUL	R19	R19	R20
      0x5850000B,  //  0126  LDCONST	R20	K11
      0x5456FE00,  //  0127  LDINT	R21	65025
      0x5858000B,  //  0128  LDCONST	R22	K11
      0x0C5C210E,  //  0129  DIV	R23	R16	K14
      0x7C480A00,  //  012A  CALL	R18	5
      0x04480C12,  //  012B  SUB	R18	R6	R18
      0x5C342400,  //  012C  MOVE	R13	R18
      0x70020015,  //  012D  JMP		#0144
      0x5C441400,  //  012E  MOVE	R17	R10
      0x544A00BF,  //  012F  LDINT	R18	192
      0x04481E12,  //  0130  SUB	R18	R15	R18
      0x584C000B,  //  0131  LDCONST	R19	K11
      0x5452003E,  //  0132  LDINT	R20	63
      0x5854000B,  //  0133  LDCONST	R21	K11
      0x545A00FE,  //  0134  LDINT	R22	255
      0x7C440A00,  //  0135  CALL	R17	5
      0x5C481600,  //  0136  MOVE	R18	R11
      0x544E00FE,  //  0137  LDINT	R19	255
      0x044C2611,  //  0138  SUB	R19	R19	R17
      0x545200FE,  //  0139  LDINT	R20	255
      0x04502811,  //  013A  SUB	R20	R20	R17
      0x084C2614,  //  013B  MUL	R19	R19	R20
      0x5850000B,  //  013C  LDCONST	R20	K11
      0x5456FE00,  //  013D  LDINT	R21	65025
      0x5858000B,  //  013E  LDCONST	R22	K11
      0x545E0003,  //  013F  LDINT	R23	4
      0x0C5C2017,  //  0140  DIV	R23	R16	R23
      0x7C480A00,  //  0141  CALL	R18	5
      0x04480C12,  //  0142  SUB	R18	R6	R18
      0x5C342400,  //  0143  MOVE	R13	R18
      0x70020007,  //  0144  JMP		#014D
      0x5C3C1400,  //  0145  MOVE	R15	R10
      0x5C401800,  //  0146  MOVE	R16	R12
      0x5844000B,  //  0147  LDCONST	R17	K11
      0x0448090F,  //  0148  SUB	R18	R4	K15
      0x5C4C0A00,  //  0149  MOVE	R19	R5
      0x5C500C00,  //  014A  MOVE	R20	R6
      0x7C3C0A00,  //  014B  CALL	R15	5
      0x5C341E00,  //  014C  MOVE	R13	R15
      0x9002220D,  //  014D  SETMBR	R0	K17	R13
      0x80041A00,  //  014E  RET	1	R13
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_oscillator_value_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_oscillator_value,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080515,  //  0003  GETMET	R2	R2	K21
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x9002230B,  //  0006  SETMBR	R0	K17	K11
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(class_oscillator_value_start,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_oscillator_value,     /* shared constants */
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080516,  //  0003  GETMET	R2	R2	K22
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x80040000,  //  0006  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: oscillator_value
********************************************************************/
extern const bclass be_class_parameterized_object;
be_local_class(oscillator_value,
    1,
    &be_class_parameterized_object,
    be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(value, 1), be_const_var(0) },
        { be_const_key_weak(VALUE_PROVIDER, -1), be_const_bool(1) },
        { be_const_key_weak(init, 4), be_const_closure(class_oscillator_value_init_closure) },
        { be_const_key_weak(PARAMS, 2), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(phase, -1), be_const_bytes_instance(07000001FF000000) },
        { be_const_key_weak(max_value, 4), be_const_bytes_instance(0401FF00) },
        { be_const_key_weak(duty_cycle, -1), be_const_bytes_instance(07000001FF00007F) },
        { be_const_key_weak(min_value, -1), be_const_bytes_instance(040000) },
        { be_const_key_weak(duration, -1), be_const_bytes_instance(05000101E803) },
        { be_const_key_weak(form, 1), be_const_bytes_instance(14000109000100020003000400050006000700080009) },
    }))    ) } )) },
        { be_const_key_weak(produce_value, -1), be_const_closure(class_oscillator_value_produce_value_closure) },
        { be_const_key_weak(start, -1), be_const_closure(class_oscillator_value_start_closure) },
    })),
    be_str_weak(oscillator_value)
);

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
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(oscillator_value),
    /* K2   */  be_nested_str_weak(form),
    }),
    be_str_weak(bounce),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x540A0008,  //  0004  LDINT	R2	9
      0x90060402,  //  0005  SETMBR	R1	K2	R2
      0x80040200,  //  0006  RET	1	R1
    })
  )
);
/*******************************************************************/

// compact class 'sequence_manager' ktab size: 34, total: 122 (saved 704 bytes)
static const bvalue be_ktab_class_sequence_manager[34] = {
  /* K0   */  be_nested_str_weak(step_durations),
  /* K1   */  be_nested_str_weak(push),
  /* K2   */  be_nested_str_weak(step_refs),
  /* K3   */  be_nested_str_weak(repeat_count),
  /* K4   */  be_nested_str_weak(function),
  /* K5   */  be_nested_str_weak(engine),
  /* K6   */  be_nested_str_weak(init),
  /* K7   */  be_nested_str_weak(active_sequence),
  /* K8   */  be_nested_str_weak(sequence_state),
  /* K9   */  be_nested_str_weak(step_index),
  /* K10  */  be_const_int(0),
  /* K11  */  be_nested_str_weak(step_start_time),
  /* K12  */  be_const_int(1),
  /* K13  */  be_nested_str_weak(current_iteration),
  /* K14  */  be_nested_str_weak(is_repeat_sequence),
  /* K15  */  be_nested_str_weak(update_current_iteration),
  /* K16  */  be_nested_str_weak(get_resolved_repeat_count),
  /* K17  */  be_nested_str_weak(execute_current_step),
  /* K18  */  be_nested_str_weak(is_running),
  /* K19  */  be_nested_str_weak(pop_iteration_context),
  /* K20  */  be_nested_str_weak(update),
  /* K21  */  be_nested_str_weak(advance_to_next_step),
  /* K22  */  be_nested_str_weak(execute_closure_steps_batch),
  /* K23  */  be_nested_str_weak(stop),
  /* K24  */  be_nested_str_weak(stop_all_subsequences),
  /* K25  */  be_nested_str_weak(start_time),
  /* K26  */  be_nested_str_weak(push_iteration_context),
  /* K27  */  be_nested_str_weak(remove),
  /* K28  */  be_nested_str_weak(complete_iteration),
  /* K29  */  be_nested_str_weak(execute_closure_steps_batch_atomic),
  /* K30  */  be_nested_str_weak(start),
  /* K31  */  be_nested_str_weak(get_animations),
  /* K32  */  be_nested_str_weak(stop_iteration),
  /* K33  */  be_nested_str_weak(add),
};


extern const bclass be_class_sequence_manager;

/********************************************************************
** Solidified function: push_wait_step
********************************************************************/
be_local_closure(class_sequence_manager_push_wait_step,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_sequence_manager,     /* shared constants */
    be_str_weak(push_wait_step),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x88080102,  //  0004  GETMBR	R2	R0	K2
      0x8C080501,  //  0005  GETMET	R2	R2	K1
      0x4C100000,  //  0006  LDNIL	R4
      0x7C080400,  //  0007  CALL	R2	2
      0x80040000,  //  0008  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_resolved_repeat_count
********************************************************************/
be_local_closure(class_sequence_manager_get_resolved_repeat_count,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_sequence_manager,     /* shared constants */
    be_str_weak(get_resolved_repeat_count),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x4C040000,  //  0000  LDNIL	R1
      0x60080004,  //  0001  GETGBL	R2	G4
      0x880C0103,  //  0002  GETMBR	R3	R0	K3
      0x7C080200,  //  0003  CALL	R2	1
      0x1C080504,  //  0004  EQ	R2	R2	K4
      0x780A0004,  //  0005  JMPF	R2	#000B
      0x8C080103,  //  0006  GETMET	R2	R0	K3
      0x88100105,  //  0007  GETMBR	R4	R0	K5
      0x7C080400,  //  0008  CALL	R2	2
      0x5C040400,  //  0009  MOVE	R1	R2
      0x70020000,  //  000A  JMP		#000C
      0x88040103,  //  000B  GETMBR	R1	R0	K3
      0x60080009,  //  000C  GETGBL	R2	G9
      0x5C0C0200,  //  000D  MOVE	R3	R1
      0x7C080200,  //  000E  CALL	R2	1
      0x80040400,  //  000F  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_sequence_manager_init,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_sequence_manager,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[36]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0706,  //  0003  GETMET	R3	R3	K6
      0x5C140200,  //  0004  MOVE	R5	R1
      0x7C0C0400,  //  0005  CALL	R3	2
      0x4C0C0000,  //  0006  LDNIL	R3
      0x90020E03,  //  0007  SETMBR	R0	K7	R3
      0x600C0013,  //  0008  GETGBL	R3	G19
      0x7C0C0000,  //  0009  CALL	R3	0
      0x90021003,  //  000A  SETMBR	R0	K8	R3
      0x9002130A,  //  000B  SETMBR	R0	K9	K10
      0x9002170A,  //  000C  SETMBR	R0	K11	K10
      0x600C0012,  //  000D  GETGBL	R3	G18
      0x7C0C0000,  //  000E  CALL	R3	0
      0x90020003,  //  000F  SETMBR	R0	K0	R3
      0x600C0012,  //  0010  GETGBL	R3	G18
      0x7C0C0000,  //  0011  CALL	R3	0
      0x90020403,  //  0012  SETMBR	R0	K2	R3
      0x4C0C0000,  //  0013  LDNIL	R3
      0x200C0403,  //  0014  NE	R3	R2	R3
      0x780E0001,  //  0015  JMPF	R3	#0018
      0x5C0C0400,  //  0016  MOVE	R3	R2
      0x70020000,  //  0017  JMP		#0019
      0x580C000C,  //  0018  LDCONST	R3	K12
      0x90020603,  //  0019  SETMBR	R0	K3	R3
      0x90021B0A,  //  001A  SETMBR	R0	K13	K10
      0x4C0C0000,  //  001B  LDNIL	R3
      0x200C0403,  //  001C  NE	R3	R2	R3
      0x780E0001,  //  001D  JMPF	R3	#0020
      0x200C050C,  //  001E  NE	R3	R2	K12
      0x740E0000,  //  001F  JMPT	R3	#0021
      0x500C0001,  //  0020  LDBOOL	R3	0	1
      0x500C0200,  //  0021  LDBOOL	R3	1	0
      0x90021C03,  //  0022  SETMBR	R0	K14	R3
      0x80000000,  //  0023  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: push_repeat_subsequence
********************************************************************/
be_local_closure(class_sequence_manager_push_repeat_subsequence,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_sequence_manager,     /* shared constants */
    be_str_weak(push_repeat_subsequence),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5411FFFC,  //  0002  LDINT	R4	-3
      0x7C080400,  //  0003  CALL	R2	2
      0x88080102,  //  0004  GETMBR	R2	R0	K2
      0x8C080501,  //  0005  GETMET	R2	R2	K1
      0x5C100200,  //  0006  MOVE	R4	R1
      0x7C080400,  //  0007  CALL	R2	2
      0x80040000,  //  0008  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: complete_iteration
********************************************************************/
be_local_closure(class_sequence_manager_complete_iteration,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_sequence_manager,     /* shared constants */
    be_str_weak(complete_iteration),
    &be_const_str_solidified,
    ( &(const binstruction[71]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x1C0C0403,  //  0001  EQ	R3	R2	R3
      0x780E0004,  //  0002  JMPF	R3	#0008
      0x600C000C,  //  0003  GETGBL	R3	G12
      0x88100100,  //  0004  GETMBR	R4	R0	K0
      0x7C0C0200,  //  0005  CALL	R3	1
      0x000C070C,  //  0006  ADD	R3	R3	K12
      0x5C080600,  //  0007  MOVE	R2	R3
      0x880C010D,  //  0008  GETMBR	R3	R0	K13
      0x000C070C,  //  0009  ADD	R3	R3	K12
      0x90021A03,  //  000A  SETMBR	R0	K13	R3
      0x880C010E,  //  000B  GETMBR	R3	R0	K14
      0x780E0003,  //  000C  JMPF	R3	#0011
      0x880C0105,  //  000D  GETMBR	R3	R0	K5
      0x8C0C070F,  //  000E  GETMET	R3	R3	K15
      0x8814010D,  //  000F  GETMBR	R5	R0	K13
      0x7C0C0400,  //  0010  CALL	R3	2
      0x8C0C0110,  //  0011  GETMET	R3	R0	K16
      0x7C0C0200,  //  0012  CALL	R3	1
      0x5411FFFE,  //  0013  LDINT	R4	-1
      0x1C100604,  //  0014  EQ	R4	R3	R4
      0x74120002,  //  0015  JMPT	R4	#0019
      0x8810010D,  //  0016  GETMBR	R4	R0	K13
      0x14100803,  //  0017  LT	R4	R4	R3
      0x78120025,  //  0018  JMPF	R4	#003F
      0x1810050A,  //  0019  LE	R4	R2	K10
      0x78120001,  //  001A  JMPF	R4	#001D
      0x9002130A,  //  001B  SETMBR	R0	K9	K10
      0x80000800,  //  001C  RET	0
      0x9002130A,  //  001D  SETMBR	R0	K9	K10
      0x6010000C,  //  001E  GETGBL	R4	G12
      0x88140100,  //  001F  GETMBR	R5	R0	K0
      0x7C100200,  //  0020  CALL	R4	1
      0x88140109,  //  0021  GETMBR	R5	R0	K9
      0x14140A04,  //  0022  LT	R5	R5	R4
      0x78160012,  //  0023  JMPF	R5	#0037
      0x88140100,  //  0024  GETMBR	R5	R0	K0
      0x88180109,  //  0025  GETMBR	R6	R0	K9
      0x94140A06,  //  0026  GETIDX	R5	R5	R6
      0x5419FFFD,  //  0027  LDINT	R6	-2
      0x1C140A06,  //  0028  EQ	R5	R5	R6
      0x7816000C,  //  0029  JMPF	R5	#0037
      0x88140102,  //  002A  GETMBR	R5	R0	K2
      0x88180109,  //  002B  GETMBR	R6	R0	K9
      0x94140A06,  //  002C  GETIDX	R5	R5	R6
      0x4C180000,  //  002D  LDNIL	R6
      0x20180A06,  //  002E  NE	R6	R5	R6
      0x781A0002,  //  002F  JMPF	R6	#0033
      0x5C180A00,  //  0030  MOVE	R6	R5
      0x881C0105,  //  0031  GETMBR	R7	R0	K5
      0x7C180200,  //  0032  CALL	R6	1
      0x88180109,  //  0033  GETMBR	R6	R0	K9
      0x00180D0C,  //  0034  ADD	R6	R6	K12
      0x90021206,  //  0035  SETMBR	R0	K9	R6
      0x7001FFE9,  //  0036  JMP		#0021
      0x88140109,  //  0037  GETMBR	R5	R0	K9
      0x14140A04,  //  0038  LT	R5	R5	R4
      0x78160003,  //  0039  JMPF	R5	#003E
      0x8C140111,  //  003A  GETMET	R5	R0	K17
      0x5C1C0200,  //  003B  MOVE	R7	R1
      0x0420050C,  //  003C  SUB	R8	R2	K12
      0x7C140600,  //  003D  CALL	R5	3
      0x70020006,  //  003E  JMP		#0046
      0x50100000,  //  003F  LDBOOL	R4	0	0
      0x90022404,  //  0040  SETMBR	R0	K18	R4
      0x8810010E,  //  0041  GETMBR	R4	R0	K14
      0x78120002,  //  0042  JMPF	R4	#0046
      0x88100105,  //  0043  GETMBR	R4	R0	K5
      0x8C100913,  //  0044  GETMET	R4	R4	K19
      0x7C100200,  //  0045  CALL	R4	1
      0x80000000,  //  0046  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update
********************************************************************/
be_local_closure(class_sequence_manager_update,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_sequence_manager,     /* shared constants */
    be_str_weak(update),
    &be_const_str_solidified,
    ( &(const binstruction[64]) {  /* code */
      0x6008000C,  //  0000  GETGBL	R2	G12
      0x880C0100,  //  0001  GETMBR	R3	R0	K0
      0x7C080200,  //  0002  CALL	R2	1
      0x880C0112,  //  0003  GETMBR	R3	R0	K18
      0x780E0004,  //  0004  JMPF	R3	#000A
      0x1C0C050A,  //  0005  EQ	R3	R2	K10
      0x740E0002,  //  0006  JMPT	R3	#000A
      0x880C0109,  //  0007  GETMBR	R3	R0	K9
      0x280C0602,  //  0008  GE	R3	R3	R2
      0x780E0000,  //  0009  JMPF	R3	#000B
      0x80000600,  //  000A  RET	0
      0x880C0100,  //  000B  GETMBR	R3	R0	K0
      0x88100109,  //  000C  GETMBR	R4	R0	K9
      0x940C0604,  //  000D  GETIDX	R3	R3	R4
      0x5411FFFC,  //  000E  LDINT	R4	-3
      0x1C100604,  //  000F  EQ	R4	R3	R4
      0x7812000B,  //  0010  JMPF	R4	#001D
      0x88100102,  //  0011  GETMBR	R4	R0	K2
      0x88140109,  //  0012  GETMBR	R5	R0	K9
      0x94100805,  //  0013  GETIDX	R4	R4	R5
      0x8C140914,  //  0014  GETMET	R5	R4	K20
      0x5C1C0200,  //  0015  MOVE	R7	R1
      0x7C140400,  //  0016  CALL	R5	2
      0x88140912,  //  0017  GETMBR	R5	R4	K18
      0x74160002,  //  0018  JMPT	R5	#001C
      0x8C140115,  //  0019  GETMET	R5	R0	K21
      0x5C1C0200,  //  001A  MOVE	R7	R1
      0x7C140400,  //  001B  CALL	R5	2
      0x70020021,  //  001C  JMP		#003F
      0x5411FFFD,  //  001D  LDINT	R4	-2
      0x1C100604,  //  001E  EQ	R4	R3	R4
      0x78120003,  //  001F  JMPF	R4	#0024
      0x8C100116,  //  0020  GETMET	R4	R0	K22
      0x5C180200,  //  0021  MOVE	R6	R1
      0x7C100400,  //  0022  CALL	R4	2
      0x7002001A,  //  0023  JMP		#003F
      0x5C100600,  //  0024  MOVE	R4	R3
      0x60140004,  //  0025  GETGBL	R5	G4
      0x5C180600,  //  0026  MOVE	R6	R3
      0x7C140200,  //  0027  CALL	R5	1
      0x1C140B04,  //  0028  EQ	R5	R5	K4
      0x78160003,  //  0029  JMPF	R5	#002E
      0x5C140600,  //  002A  MOVE	R5	R3
      0x88180105,  //  002B  GETMBR	R6	R0	K5
      0x7C140200,  //  002C  CALL	R5	1
      0x5C100A00,  //  002D  MOVE	R4	R5
      0x60140009,  //  002E  GETGBL	R5	G9
      0x5C180800,  //  002F  MOVE	R6	R4
      0x7C140200,  //  0030  CALL	R5	1
      0x5C100A00,  //  0031  MOVE	R4	R5
      0x2414090A,  //  0032  GT	R5	R4	K10
      0x78160007,  //  0033  JMPF	R5	#003C
      0x8814010B,  //  0034  GETMBR	R5	R0	K11
      0x04140205,  //  0035  SUB	R5	R1	R5
      0x28140A04,  //  0036  GE	R5	R5	R4
      0x78160002,  //  0037  JMPF	R5	#003B
      0x8C140115,  //  0038  GETMET	R5	R0	K21
      0x5C1C0200,  //  0039  MOVE	R7	R1
      0x7C140400,  //  003A  CALL	R5	2
      0x70020002,  //  003B  JMP		#003F
      0x8C140115,  //  003C  GETMET	R5	R0	K21
      0x5C1C0200,  //  003D  MOVE	R7	R1
      0x7C140400,  //  003E  CALL	R5	2
      0x80000000,  //  003F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: stop_all_subsequences
********************************************************************/
be_local_closure(class_sequence_manager_stop_all_subsequences,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_sequence_manager,     /* shared constants */
    be_str_weak(stop_all_subsequences),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x6004000C,  //  0000  GETGBL	R1	G12
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x7C040200,  //  0002  CALL	R1	1
      0x5808000A,  //  0003  LDCONST	R2	K10
      0x140C0401,  //  0004  LT	R3	R2	R1
      0x780E000A,  //  0005  JMPF	R3	#0011
      0x880C0100,  //  0006  GETMBR	R3	R0	K0
      0x940C0602,  //  0007  GETIDX	R3	R3	R2
      0x5411FFFC,  //  0008  LDINT	R4	-3
      0x1C0C0604,  //  0009  EQ	R3	R3	R4
      0x780E0003,  //  000A  JMPF	R3	#000F
      0x880C0102,  //  000B  GETMBR	R3	R0	K2
      0x940C0602,  //  000C  GETIDX	R3	R3	R2
      0x8C0C0717,  //  000D  GETMET	R3	R3	K23
      0x7C0C0200,  //  000E  CALL	R3	1
      0x0008050C,  //  000F  ADD	R2	R2	K12
      0x7001FFF2,  //  0010  JMP		#0004
      0x80040000,  //  0011  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(class_sequence_manager_start,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_sequence_manager,     /* shared constants */
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[59]) {  /* code */
      0x88080112,  //  0000  GETMBR	R2	R0	K18
      0x780A0003,  //  0001  JMPF	R2	#0006
      0x50080000,  //  0002  LDBOOL	R2	0	0
      0x90022402,  //  0003  SETMBR	R0	K18	R2
      0x8C080118,  //  0004  GETMET	R2	R0	K24
      0x7C080200,  //  0005  CALL	R2	1
      0x9002130A,  //  0006  SETMBR	R0	K9	K10
      0x90021601,  //  0007  SETMBR	R0	K11	R1
      0x90021B0A,  //  0008  SETMBR	R0	K13	K10
      0x50080200,  //  0009  LDBOOL	R2	1	0
      0x90022402,  //  000A  SETMBR	R0	K18	R2
      0x90023201,  //  000B  SETMBR	R0	K25	R1
      0x8C080110,  //  000C  GETMET	R2	R0	K16
      0x7C080200,  //  000D  CALL	R2	1
      0x1C0C050A,  //  000E  EQ	R3	R2	K10
      0x780E0002,  //  000F  JMPF	R3	#0013
      0x500C0000,  //  0010  LDBOOL	R3	0	0
      0x90022403,  //  0011  SETMBR	R0	K18	R3
      0x80040000,  //  0012  RET	1	R0
      0x880C010E,  //  0013  GETMBR	R3	R0	K14
      0x780E0003,  //  0014  JMPF	R3	#0019
      0x880C0105,  //  0015  GETMBR	R3	R0	K5
      0x8C0C071A,  //  0016  GETMET	R3	R3	K26
      0x8814010D,  //  0017  GETMBR	R5	R0	K13
      0x7C0C0400,  //  0018  CALL	R3	2
      0x600C000C,  //  0019  GETGBL	R3	G12
      0x88100100,  //  001A  GETMBR	R4	R0	K0
      0x7C0C0200,  //  001B  CALL	R3	1
      0x2410070A,  //  001C  GT	R4	R3	K10
      0x7812001B,  //  001D  JMPF	R4	#003A
      0x88100109,  //  001E  GETMBR	R4	R0	K9
      0x14100803,  //  001F  LT	R4	R4	R3
      0x78120012,  //  0020  JMPF	R4	#0034
      0x88100100,  //  0021  GETMBR	R4	R0	K0
      0x88140109,  //  0022  GETMBR	R5	R0	K9
      0x94100805,  //  0023  GETIDX	R4	R4	R5
      0x5415FFFD,  //  0024  LDINT	R5	-2
      0x1C100805,  //  0025  EQ	R4	R4	R5
      0x7812000C,  //  0026  JMPF	R4	#0034
      0x88100102,  //  0027  GETMBR	R4	R0	K2
      0x88140109,  //  0028  GETMBR	R5	R0	K9
      0x94100805,  //  0029  GETIDX	R4	R4	R5
      0x4C140000,  //  002A  LDNIL	R5
      0x20140805,  //  002B  NE	R5	R4	R5
      0x78160002,  //  002C  JMPF	R5	#0030
      0x5C140800,  //  002D  MOVE	R5	R4
      0x88180105,  //  002E  GETMBR	R6	R0	K5
      0x7C140200,  //  002F  CALL	R5	1
      0x88140109,  //  0030  GETMBR	R5	R0	K9
      0x00140B0C,  //  0031  ADD	R5	R5	K12
      0x90021205,  //  0032  SETMBR	R0	K9	R5
      0x7001FFE9,  //  0033  JMP		#001E
      0x88100109,  //  0034  GETMBR	R4	R0	K9
      0x14100803,  //  0035  LT	R4	R4	R3
      0x78120002,  //  0036  JMPF	R4	#003A
      0x8C100111,  //  0037  GETMET	R4	R0	K17
      0x5C180200,  //  0038  MOVE	R6	R1
      0x7C100400,  //  0039  CALL	R4	2
      0x80040000,  //  003A  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: advance_to_next_step
********************************************************************/
be_local_closure(class_sequence_manager_advance_to_next_step,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_sequence_manager,     /* shared constants */
    be_str_weak(advance_to_next_step),
    &be_const_str_solidified,
    ( &(const binstruction[49]) {  /* code */
      0x600C000C,  //  0000  GETGBL	R3	G12
      0x88100100,  //  0001  GETMBR	R4	R0	K0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x4C100000,  //  0003  LDNIL	R4
      0x1C100404,  //  0004  EQ	R4	R2	R4
      0x78120001,  //  0005  JMPF	R4	#0008
      0x0010070C,  //  0006  ADD	R4	R3	K12
      0x5C080800,  //  0007  MOVE	R2	R4
      0x88100100,  //  0008  GETMBR	R4	R0	K0
      0x88140109,  //  0009  GETMBR	R5	R0	K9
      0x94100805,  //  000A  GETIDX	R4	R4	R5
      0x88140102,  //  000B  GETMBR	R5	R0	K2
      0x88180109,  //  000C  GETMBR	R6	R0	K9
      0x94140A06,  //  000D  GETIDX	R5	R5	R6
      0x4C180000,  //  000E  LDNIL	R6
      0x541DFFFD,  //  000F  LDINT	R7	-2
      0x201C0807,  //  0010  NE	R7	R4	R7
      0x781E0006,  //  0011  JMPF	R7	#0019
      0x541DFFFC,  //  0012  LDINT	R7	-3
      0x201C0807,  //  0013  NE	R7	R4	R7
      0x781E0003,  //  0014  JMPF	R7	#0019
      0x4C1C0000,  //  0015  LDNIL	R7
      0x201C0A07,  //  0016  NE	R7	R5	R7
      0x781E0000,  //  0017  JMPF	R7	#0019
      0x5C180A00,  //  0018  MOVE	R6	R5
      0x881C0109,  //  0019  GETMBR	R7	R0	K9
      0x001C0F0C,  //  001A  ADD	R7	R7	K12
      0x90021207,  //  001B  SETMBR	R0	K9	R7
      0x881C0109,  //  001C  GETMBR	R7	R0	K9
      0x281C0E03,  //  001D  GE	R7	R7	R3
      0x781E000B,  //  001E  JMPF	R7	#002B
      0x4C1C0000,  //  001F  LDNIL	R7
      0x201C0C07,  //  0020  NE	R7	R6	R7
      0x781E0003,  //  0021  JMPF	R7	#0026
      0x881C0105,  //  0022  GETMBR	R7	R0	K5
      0x8C1C0F1B,  //  0023  GETMET	R7	R7	K27
      0x5C240C00,  //  0024  MOVE	R9	R6
      0x7C1C0400,  //  0025  CALL	R7	2
      0x8C1C011C,  //  0026  GETMET	R7	R0	K28
      0x5C240200,  //  0027  MOVE	R9	R1
      0x5C280400,  //  0028  MOVE	R10	R2
      0x7C1C0600,  //  0029  CALL	R7	3
      0x70020004,  //  002A  JMP		#0030
      0x8C1C011D,  //  002B  GETMET	R7	R0	K29
      0x5C240200,  //  002C  MOVE	R9	R1
      0x5C280C00,  //  002D  MOVE	R10	R6
      0x5C2C0400,  //  002E  MOVE	R11	R2
      0x7C1C0800,  //  002F  CALL	R7	4
      0x80000000,  //  0030  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_sequence_running
********************************************************************/
be_local_closure(class_sequence_manager_is_sequence_running,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_sequence_manager,     /* shared constants */
    be_str_weak(is_sequence_running),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040112,  //  0000  GETMBR	R1	R0	K18
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: execute_closure_steps_batch_atomic
********************************************************************/
be_local_closure(class_sequence_manager_execute_closure_steps_batch_atomic,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_sequence_manager,     /* shared constants */
    be_str_weak(execute_closure_steps_batch_atomic),
    &be_const_str_solidified,
    ( &(const binstruction[83]) {  /* code */
      0x4C100000,  //  0000  LDNIL	R4
      0x1C100604,  //  0001  EQ	R4	R3	R4
      0x78120004,  //  0002  JMPF	R4	#0008
      0x6010000C,  //  0003  GETGBL	R4	G12
      0x88140100,  //  0004  GETMBR	R5	R0	K0
      0x7C100200,  //  0005  CALL	R4	1
      0x0010090C,  //  0006  ADD	R4	R4	K12
      0x5C0C0800,  //  0007  MOVE	R3	R4
      0x6010000C,  //  0008  GETGBL	R4	G12
      0x88140100,  //  0009  GETMBR	R5	R0	K0
      0x7C100200,  //  000A  CALL	R4	1
      0x88140109,  //  000B  GETMBR	R5	R0	K9
      0x14140A04,  //  000C  LT	R5	R5	R4
      0x78160012,  //  000D  JMPF	R5	#0021
      0x88140100,  //  000E  GETMBR	R5	R0	K0
      0x88180109,  //  000F  GETMBR	R6	R0	K9
      0x94140A06,  //  0010  GETIDX	R5	R5	R6
      0x5419FFFD,  //  0011  LDINT	R6	-2
      0x1C140A06,  //  0012  EQ	R5	R5	R6
      0x7816000C,  //  0013  JMPF	R5	#0021
      0x88140102,  //  0014  GETMBR	R5	R0	K2
      0x88180109,  //  0015  GETMBR	R6	R0	K9
      0x94140A06,  //  0016  GETIDX	R5	R5	R6
      0x4C180000,  //  0017  LDNIL	R6
      0x20180A06,  //  0018  NE	R6	R5	R6
      0x781A0002,  //  0019  JMPF	R6	#001D
      0x5C180A00,  //  001A  MOVE	R6	R5
      0x881C0105,  //  001B  GETMBR	R7	R0	K5
      0x7C180200,  //  001C  CALL	R6	1
      0x88180109,  //  001D  GETMBR	R6	R0	K9
      0x00180D0C,  //  001E  ADD	R6	R6	K12
      0x90021206,  //  001F  SETMBR	R0	K9	R6
      0x7001FFE9,  //  0020  JMP		#000B
      0x50140000,  //  0021  LDBOOL	R5	0	0
      0x88180109,  //  0022  GETMBR	R6	R0	K9
      0x14180C04,  //  0023  LT	R6	R6	R4
      0x781A0011,  //  0024  JMPF	R6	#0037
      0x4C180000,  //  0025  LDNIL	R6
      0x20180406,  //  0026  NE	R6	R2	R6
      0x781A000E,  //  0027  JMPF	R6	#0037
      0x88180100,  //  0028  GETMBR	R6	R0	K0
      0x881C0109,  //  0029  GETMBR	R7	R0	K9
      0x94180C07,  //  002A  GETIDX	R6	R6	R7
      0x881C0102,  //  002B  GETMBR	R7	R0	K2
      0x88200109,  //  002C  GETMBR	R8	R0	K9
      0x941C0E08,  //  002D  GETIDX	R7	R7	R8
      0x5421FFFD,  //  002E  LDINT	R8	-2
      0x20200C08,  //  002F  NE	R8	R6	R8
      0x78220005,  //  0030  JMPF	R8	#0037
      0x5421FFFC,  //  0031  LDINT	R8	-3
      0x20200C08,  //  0032  NE	R8	R6	R8
      0x78220002,  //  0033  JMPF	R8	#0037
      0x1C200E02,  //  0034  EQ	R8	R7	R2
      0x78220000,  //  0035  JMPF	R8	#0037
      0x50140200,  //  0036  LDBOOL	R5	1	0
      0x78160004,  //  0037  JMPF	R5	#003D
      0x90021601,  //  0038  SETMBR	R0	K11	R1
      0x8C18051E,  //  0039  GETMET	R6	R2	K30
      0x5C200200,  //  003A  MOVE	R8	R1
      0x7C180400,  //  003B  CALL	R6	2
      0x7002000D,  //  003C  JMP		#004B
      0x88180109,  //  003D  GETMBR	R6	R0	K9
      0x14180C04,  //  003E  LT	R6	R6	R4
      0x781A0003,  //  003F  JMPF	R6	#0044
      0x8C180111,  //  0040  GETMET	R6	R0	K17
      0x5C200200,  //  0041  MOVE	R8	R1
      0x5C240600,  //  0042  MOVE	R9	R3
      0x7C180600,  //  0043  CALL	R6	3
      0x4C180000,  //  0044  LDNIL	R6
      0x20180406,  //  0045  NE	R6	R2	R6
      0x781A0003,  //  0046  JMPF	R6	#004B
      0x88180105,  //  0047  GETMBR	R6	R0	K5
      0x8C180D1B,  //  0048  GETMET	R6	R6	K27
      0x5C200400,  //  0049  MOVE	R8	R2
      0x7C180400,  //  004A  CALL	R6	2
      0x88180109,  //  004B  GETMBR	R6	R0	K9
      0x28180C04,  //  004C  GE	R6	R6	R4
      0x781A0003,  //  004D  JMPF	R6	#0052
      0x8C18011C,  //  004E  GETMET	R6	R0	K28
      0x5C200200,  //  004F  MOVE	R8	R1
      0x5C240600,  //  0050  MOVE	R9	R3
      0x7C180600,  //  0051  CALL	R6	3
      0x80000000,  //  0052  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: stop
********************************************************************/
be_local_closure(class_sequence_manager_stop,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_sequence_manager,     /* shared constants */
    be_str_weak(stop),
    &be_const_str_solidified,
    ( &(const binstruction[40]) {  /* code */
      0x88040112,  //  0000  GETMBR	R1	R0	K18
      0x78060024,  //  0001  JMPF	R1	#0027
      0x50040000,  //  0002  LDBOOL	R1	0	0
      0x90022401,  //  0003  SETMBR	R0	K18	R1
      0x8804010E,  //  0004  GETMBR	R1	R0	K14
      0x78060002,  //  0005  JMPF	R1	#0009
      0x88040105,  //  0006  GETMBR	R1	R0	K5
      0x8C040313,  //  0007  GETMET	R1	R1	K19
      0x7C040200,  //  0008  CALL	R1	1
      0x6004000C,  //  0009  GETGBL	R1	G12
      0x88080100,  //  000A  GETMBR	R2	R0	K0
      0x7C040200,  //  000B  CALL	R1	1
      0x88080109,  //  000C  GETMBR	R2	R0	K9
      0x14080401,  //  000D  LT	R2	R2	R1
      0x780A0015,  //  000E  JMPF	R2	#0025
      0x88080100,  //  000F  GETMBR	R2	R0	K0
      0x880C0109,  //  0010  GETMBR	R3	R0	K9
      0x94080403,  //  0011  GETIDX	R2	R2	R3
      0x880C0102,  //  0012  GETMBR	R3	R0	K2
      0x88100109,  //  0013  GETMBR	R4	R0	K9
      0x940C0604,  //  0014  GETIDX	R3	R3	R4
      0x5411FFFC,  //  0015  LDINT	R4	-3
      0x1C100404,  //  0016  EQ	R4	R2	R4
      0x78120002,  //  0017  JMPF	R4	#001B
      0x8C100717,  //  0018  GETMET	R4	R3	K23
      0x7C100200,  //  0019  CALL	R4	1
      0x70020009,  //  001A  JMP		#0025
      0x5411FFFD,  //  001B  LDINT	R4	-2
      0x20100404,  //  001C  NE	R4	R2	R4
      0x78120006,  //  001D  JMPF	R4	#0025
      0x4C100000,  //  001E  LDNIL	R4
      0x20100604,  //  001F  NE	R4	R3	R4
      0x78120003,  //  0020  JMPF	R4	#0025
      0x88100105,  //  0021  GETMBR	R4	R0	K5
      0x8C10091B,  //  0022  GETMET	R4	R4	K27
      0x5C180600,  //  0023  MOVE	R6	R3
      0x7C100400,  //  0024  CALL	R4	2
      0x8C080118,  //  0025  GETMET	R2	R0	K24
      0x7C080200,  //  0026  CALL	R2	1
      0x80040000,  //  0027  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: execute_current_step
********************************************************************/
be_local_closure(class_sequence_manager_execute_current_step,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_sequence_manager,     /* shared constants */
    be_str_weak(execute_current_step),
    &be_const_str_solidified,
    ( &(const binstruction[85]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x1C0C0403,  //  0001  EQ	R3	R2	R3
      0x780E0004,  //  0002  JMPF	R3	#0008
      0x600C000C,  //  0003  GETGBL	R3	G12
      0x88100100,  //  0004  GETMBR	R4	R0	K0
      0x7C0C0200,  //  0005  CALL	R3	1
      0x000C070C,  //  0006  ADD	R3	R3	K12
      0x5C080600,  //  0007  MOVE	R2	R3
      0x600C000C,  //  0008  GETGBL	R3	G12
      0x88100100,  //  0009  GETMBR	R4	R0	K0
      0x7C0C0200,  //  000A  CALL	R3	1
      0x88100109,  //  000B  GETMBR	R4	R0	K9
      0x28100803,  //  000C  GE	R4	R4	R3
      0x78120004,  //  000D  JMPF	R4	#0013
      0x8C10011C,  //  000E  GETMET	R4	R0	K28
      0x5C180200,  //  000F  MOVE	R6	R1
      0x5C1C0400,  //  0010  MOVE	R7	R2
      0x7C100600,  //  0011  CALL	R4	3
      0x80000800,  //  0012  RET	0
      0x88100100,  //  0013  GETMBR	R4	R0	K0
      0x88140109,  //  0014  GETMBR	R5	R0	K9
      0x94100805,  //  0015  GETIDX	R4	R4	R5
      0x88140102,  //  0016  GETMBR	R5	R0	K2
      0x88180109,  //  0017  GETMBR	R6	R0	K9
      0x94140A06,  //  0018  GETIDX	R5	R5	R6
      0x5419FFFC,  //  0019  LDINT	R6	-3
      0x1C180806,  //  001A  EQ	R6	R4	R6
      0x781A000C,  //  001B  JMPF	R6	#0029
      0x8C180B1E,  //  001C  GETMET	R6	R5	K30
      0x5C200200,  //  001D  MOVE	R8	R1
      0x7C180400,  //  001E  CALL	R6	2
      0x88180B12,  //  001F  GETMBR	R6	R5	K18
      0x741A0006,  //  0020  JMPT	R6	#0028
      0x2418050A,  //  0021  GT	R6	R2	K10
      0x781A0004,  //  0022  JMPF	R6	#0028
      0x8C180115,  //  0023  GETMET	R6	R0	K21
      0x5C200200,  //  0024  MOVE	R8	R1
      0x0424050C,  //  0025  SUB	R9	R2	K12
      0x7C180600,  //  0026  CALL	R6	3
      0x80000C00,  //  0027  RET	0
      0x70020029,  //  0028  JMP		#0053
      0x5419FFFD,  //  0029  LDINT	R6	-2
      0x1C180806,  //  002A  EQ	R6	R4	R6
      0x781A0006,  //  002B  JMPF	R6	#0033
      0x4C180000,  //  002C  LDNIL	R6
      0x20180A06,  //  002D  NE	R6	R5	R6
      0x781A0002,  //  002E  JMPF	R6	#0032
      0x5C180A00,  //  002F  MOVE	R6	R5
      0x881C0105,  //  0030  GETMBR	R7	R0	K5
      0x7C180200,  //  0031  CALL	R6	1
      0x7002001F,  //  0032  JMP		#0053
      0x4C180000,  //  0033  LDNIL	R6
      0x20180A06,  //  0034  NE	R6	R5	R6
      0x781A001C,  //  0035  JMPF	R6	#0053
      0x88180105,  //  0036  GETMBR	R6	R0	K5
      0x8C180D1F,  //  0037  GETMET	R6	R6	K31
      0x7C180200,  //  0038  CALL	R6	1
      0x501C0000,  //  0039  LDBOOL	R7	0	0
      0x60200010,  //  003A  GETGBL	R8	G16
      0x5C240C00,  //  003B  MOVE	R9	R6
      0x7C200200,  //  003C  CALL	R8	1
      0xA8020008,  //  003D  EXBLK	0	#0047
      0x5C241000,  //  003E  MOVE	R9	R8
      0x7C240000,  //  003F  CALL	R9	0
      0x1C281205,  //  0040  EQ	R10	R9	R5
      0x782A0001,  //  0041  JMPF	R10	#0044
      0x501C0200,  //  0042  LDBOOL	R7	1	0
      0x70020000,  //  0043  JMP		#0045
      0x7001FFF8,  //  0044  JMP		#003E
      0xA8040001,  //  0045  EXBLK	1	1
      0x70020002,  //  0046  JMP		#004A
      0x58200020,  //  0047  LDCONST	R8	K32
      0xAC200200,  //  0048  CATCH	R8	1	0
      0xB0080000,  //  0049  RAISE	2	R0	R0
      0x5C200E00,  //  004A  MOVE	R8	R7
      0x74220003,  //  004B  JMPT	R8	#0050
      0x88200105,  //  004C  GETMBR	R8	R0	K5
      0x8C201121,  //  004D  GETMET	R8	R8	K33
      0x5C280A00,  //  004E  MOVE	R10	R5
      0x7C200400,  //  004F  CALL	R8	2
      0x8C200B1E,  //  0050  GETMET	R8	R5	K30
      0x5C280200,  //  0051  MOVE	R10	R1
      0x7C200400,  //  0052  CALL	R8	2
      0x90021601,  //  0053  SETMBR	R0	K11	R1
      0x80000000,  //  0054  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: push_play_step
********************************************************************/
be_local_closure(class_sequence_manager_push_play_step,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_sequence_manager,     /* shared constants */
    be_str_weak(push_play_step),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x4C140000,  //  0002  LDNIL	R5
      0x20140405,  //  0003  NE	R5	R2	R5
      0x78160001,  //  0004  JMPF	R5	#0007
      0x5C140400,  //  0005  MOVE	R5	R2
      0x70020000,  //  0006  JMP		#0008
      0x5814000A,  //  0007  LDCONST	R5	K10
      0x7C0C0400,  //  0008  CALL	R3	2
      0x880C0102,  //  0009  GETMBR	R3	R0	K2
      0x8C0C0701,  //  000A  GETMET	R3	R3	K1
      0x5C140200,  //  000B  MOVE	R5	R1
      0x7C0C0400,  //  000C  CALL	R3	2
      0x80040000,  //  000D  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: push_closure_step
********************************************************************/
be_local_closure(class_sequence_manager_push_closure_step,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_sequence_manager,     /* shared constants */
    be_str_weak(push_closure_step),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5411FFFD,  //  0002  LDINT	R4	-2
      0x7C080400,  //  0003  CALL	R2	2
      0x88080102,  //  0004  GETMBR	R2	R0	K2
      0x8C080501,  //  0005  GETMET	R2	R2	K1
      0x5C100200,  //  0006  MOVE	R4	R1
      0x7C080400,  //  0007  CALL	R2	2
      0x80040000,  //  0008  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: execute_closure_steps_batch
********************************************************************/
be_local_closure(class_sequence_manager_execute_closure_steps_batch,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_sequence_manager,     /* shared constants */
    be_str_weak(execute_closure_steps_batch),
    &be_const_str_solidified,
    ( &(const binstruction[36]) {  /* code */
      0x6008000C,  //  0000  GETGBL	R2	G12
      0x880C0100,  //  0001  GETMBR	R3	R0	K0
      0x7C080200,  //  0002  CALL	R2	1
      0x880C0109,  //  0003  GETMBR	R3	R0	K9
      0x140C0602,  //  0004  LT	R3	R3	R2
      0x780E0012,  //  0005  JMPF	R3	#0019
      0x880C0100,  //  0006  GETMBR	R3	R0	K0
      0x88100109,  //  0007  GETMBR	R4	R0	K9
      0x940C0604,  //  0008  GETIDX	R3	R3	R4
      0x5411FFFD,  //  0009  LDINT	R4	-2
      0x1C0C0604,  //  000A  EQ	R3	R3	R4
      0x780E000C,  //  000B  JMPF	R3	#0019
      0x880C0102,  //  000C  GETMBR	R3	R0	K2
      0x88100109,  //  000D  GETMBR	R4	R0	K9
      0x940C0604,  //  000E  GETIDX	R3	R3	R4
      0x4C100000,  //  000F  LDNIL	R4
      0x20100604,  //  0010  NE	R4	R3	R4
      0x78120002,  //  0011  JMPF	R4	#0015
      0x5C100600,  //  0012  MOVE	R4	R3
      0x88140105,  //  0013  GETMBR	R5	R0	K5
      0x7C100200,  //  0014  CALL	R4	1
      0x88100109,  //  0015  GETMBR	R4	R0	K9
      0x0010090C,  //  0016  ADD	R4	R4	K12
      0x90021204,  //  0017  SETMBR	R0	K9	R4
      0x7001FFE9,  //  0018  JMP		#0003
      0x880C0109,  //  0019  GETMBR	R3	R0	K9
      0x140C0602,  //  001A  LT	R3	R3	R2
      0x780E0003,  //  001B  JMPF	R3	#0020
      0x8C0C0111,  //  001C  GETMET	R3	R0	K17
      0x5C140200,  //  001D  MOVE	R5	R1
      0x7C0C0400,  //  001E  CALL	R3	2
      0x70020002,  //  001F  JMP		#0023
      0x8C0C011C,  //  0020  GETMET	R3	R0	K28
      0x5C140200,  //  0021  MOVE	R5	R1
      0x7C0C0400,  //  0022  CALL	R3	2
      0x80000000,  //  0023  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: sequence_manager
********************************************************************/
extern const bclass be_class_parameterized_object;
be_local_class(sequence_manager,
    9,
    &be_class_parameterized_object,
    be_nested_map(25,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(is_repeat_sequence, -1), be_const_var(8) },
        { be_const_key_weak(current_iteration, -1), be_const_var(7) },
        { be_const_key_weak(step_refs, -1), be_const_var(5) },
        { be_const_key_weak(push_wait_step, -1), be_const_closure(class_sequence_manager_push_wait_step_closure) },
        { be_const_key_weak(advance_to_next_step, -1), be_const_closure(class_sequence_manager_advance_to_next_step_closure) },
        { be_const_key_weak(init, 4), be_const_closure(class_sequence_manager_init_closure) },
        { be_const_key_weak(push_repeat_subsequence, -1), be_const_closure(class_sequence_manager_push_repeat_subsequence_closure) },
        { be_const_key_weak(is_sequence_running, -1), be_const_closure(class_sequence_manager_is_sequence_running_closure) },
        { be_const_key_weak(step_start_time, 24), be_const_var(3) },
        { be_const_key_weak(update, -1), be_const_closure(class_sequence_manager_update_closure) },
        { be_const_key_weak(stop_all_subsequences, -1), be_const_closure(class_sequence_manager_stop_all_subsequences_closure) },
        { be_const_key_weak(start, 1), be_const_closure(class_sequence_manager_start_closure) },
        { be_const_key_weak(sequence_state, -1), be_const_var(1) },
        { be_const_key_weak(complete_iteration, 7), be_const_closure(class_sequence_manager_complete_iteration_closure) },
        { be_const_key_weak(execute_closure_steps_batch_atomic, 12), be_const_closure(class_sequence_manager_execute_closure_steps_batch_atomic_closure) },
        { be_const_key_weak(execute_current_step, -1), be_const_closure(class_sequence_manager_execute_current_step_closure) },
        { be_const_key_weak(step_durations, -1), be_const_var(4) },
        { be_const_key_weak(stop, -1), be_const_closure(class_sequence_manager_stop_closure) },
        { be_const_key_weak(get_resolved_repeat_count, 15), be_const_closure(class_sequence_manager_get_resolved_repeat_count_closure) },
        { be_const_key_weak(push_play_step, -1), be_const_closure(class_sequence_manager_push_play_step_closure) },
        { be_const_key_weak(repeat_count, -1), be_const_var(6) },
        { be_const_key_weak(active_sequence, -1), be_const_var(0) },
        { be_const_key_weak(push_closure_step, -1), be_const_closure(class_sequence_manager_push_closure_step_closure) },
        { be_const_key_weak(execute_closure_steps_batch, -1), be_const_closure(class_sequence_manager_execute_closure_steps_batch_closure) },
        { be_const_key_weak(step_index, -1), be_const_var(2) },
    })),
    be_str_weak(sequence_manager)
);
// compact class 'Animation' ktab size: 24, total: 32 (saved 64 bytes)
static const bvalue be_ktab_class_Animation[24] = {
  /* K0   */  be_nested_str_weak(get_color_at),
  /* K1   */  be_const_int(0),
  /* K2   */  be_nested_str_weak(init),
  /* K3   */  be_nested_str_weak(member),
  /* K4   */  be_nested_str_weak(color),
  /* K5   */  be_nested_str_weak(fill_pixels),
  /* K6   */  be_nested_str_weak(pixels),
  /* K7   */  be_nested_str_weak(animation),
  /* K8   */  be_nested_str_weak(opacity_frame),
  /* K9   */  be_nested_str_weak(width),
  /* K10  */  be_nested_str_weak(frame_buffer),
  /* K11  */  be_nested_str_weak(clear),
  /* K12  */  be_nested_str_weak(is_running),
  /* K13  */  be_nested_str_weak(start),
  /* K14  */  be_nested_str_weak(start_time),
  /* K15  */  be_nested_str_weak(update),
  /* K16  */  be_nested_str_weak(render),
  /* K17  */  be_nested_str_weak(apply_opacity),
  /* K18  */  be_nested_str_weak(opacity),
  /* K19  */  be_nested_str_weak(int),
  /* K20  */  be_nested_str_weak(_apply_opacity),
  /* K21  */  be_nested_str_weak(get_param_value),
  /* K22  */  be_nested_str_weak(duration),
  /* K23  */  be_nested_str_weak(loop),
};


extern const bclass be_class_Animation;

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
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x58100001,  //  0001  LDCONST	R4	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C080600,  //  0003  CALL	R2	3
      0x80040400,  //  0004  RET	1	R2
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
      0x8C080502,  //  0003  GETMET	R2	R2	K2
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x80000000,  //  0006  RET	0
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
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animation,     /* shared constants */
    be_str_weak(render),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x8C100103,  //  0000  GETMET	R4	R0	K3
      0x58180004,  //  0001  LDCONST	R6	K4
      0x7C100400,  //  0002  CALL	R4	2
      0x20140901,  //  0003  NE	R5	R4	K1
      0x78160003,  //  0004  JMPF	R5	#0009
      0x8C140305,  //  0005  GETMET	R5	R1	K5
      0x881C0306,  //  0006  GETMBR	R7	R1	K6
      0x5C200800,  //  0007  MOVE	R8	R4
      0x7C140600,  //  0008  CALL	R5	3
      0x50140200,  //  0009  LDBOOL	R5	1	0
      0x80040A00,  //  000A  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _apply_opacity
********************************************************************/
be_local_closure(class_Animation__apply_opacity,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animation,     /* shared constants */
    be_str_weak(_apply_opacity),
    &be_const_str_solidified,
    ( &(const binstruction[43]) {  /* code */
      0x6014000F,  //  0000  GETGBL	R5	G15
      0x5C180400,  //  0001  MOVE	R6	R2
      0xB81E0E00,  //  0002  GETNGBL	R7	K7
      0x881C0F07,  //  0003  GETMBR	R7	R7	K7
      0x7C140400,  //  0004  CALL	R5	2
      0x78160023,  //  0005  JMPF	R5	#002A
      0x5C140400,  //  0006  MOVE	R5	R2
      0x88180108,  //  0007  GETMBR	R6	R0	K8
      0x4C1C0000,  //  0008  LDNIL	R7
      0x1C180C07,  //  0009  EQ	R6	R6	R7
      0x741A0004,  //  000A  JMPT	R6	#0010
      0x88180108,  //  000B  GETMBR	R6	R0	K8
      0x88180D09,  //  000C  GETMBR	R6	R6	K9
      0x881C0309,  //  000D  GETMBR	R7	R1	K9
      0x20180C07,  //  000E  NE	R6	R6	R7
      0x781A0004,  //  000F  JMPF	R6	#0015
      0xB81A0E00,  //  0010  GETNGBL	R6	K7
      0x8C180D0A,  //  0011  GETMET	R6	R6	K10
      0x88200309,  //  0012  GETMBR	R8	R1	K9
      0x7C180400,  //  0013  CALL	R6	2
      0x90021006,  //  0014  SETMBR	R0	K8	R6
      0x88180108,  //  0015  GETMBR	R6	R0	K8
      0x8C180D0B,  //  0016  GETMET	R6	R6	K11
      0x7C180200,  //  0017  CALL	R6	1
      0x88180B0C,  //  0018  GETMBR	R6	R5	K12
      0x741A0002,  //  0019  JMPT	R6	#001D
      0x8C180B0D,  //  001A  GETMET	R6	R5	K13
      0x8820010E,  //  001B  GETMBR	R8	R0	K14
      0x7C180400,  //  001C  CALL	R6	2
      0x8C180B0F,  //  001D  GETMET	R6	R5	K15
      0x5C200600,  //  001E  MOVE	R8	R3
      0x7C180400,  //  001F  CALL	R6	2
      0x8C180B10,  //  0020  GETMET	R6	R5	K16
      0x88200108,  //  0021  GETMBR	R8	R0	K8
      0x5C240600,  //  0022  MOVE	R9	R3
      0x5C280800,  //  0023  MOVE	R10	R4
      0x7C180800,  //  0024  CALL	R6	4
      0x8C180311,  //  0025  GETMET	R6	R1	K17
      0x88200306,  //  0026  GETMBR	R8	R1	K6
      0x88240108,  //  0027  GETMBR	R9	R0	K8
      0x88241306,  //  0028  GETMBR	R9	R9	K6
      0x7C180600,  //  0029  CALL	R6	3
      0x80000000,  //  002A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: post_render
********************************************************************/
be_local_closure(class_Animation_post_render,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animation,     /* shared constants */
    be_str_weak(post_render),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x88100112,  //  0000  GETMBR	R4	R0	K18
      0x541600FE,  //  0001  LDINT	R5	255
      0x1C140805,  //  0002  EQ	R5	R4	R5
      0x78160001,  //  0003  JMPF	R5	#0006
      0x80000A00,  //  0004  RET	0
      0x7002000F,  //  0005  JMP		#0016
      0x60140004,  //  0006  GETGBL	R5	G4
      0x5C180800,  //  0007  MOVE	R6	R4
      0x7C140200,  //  0008  CALL	R5	1
      0x1C140B13,  //  0009  EQ	R5	R5	K19
      0x78160004,  //  000A  JMPF	R5	#0010
      0x8C140311,  //  000B  GETMET	R5	R1	K17
      0x881C0306,  //  000C  GETMBR	R7	R1	K6
      0x5C200800,  //  000D  MOVE	R8	R4
      0x7C140600,  //  000E  CALL	R5	3
      0x70020005,  //  000F  JMP		#0016
      0x8C140114,  //  0010  GETMET	R5	R0	K20
      0x5C1C0200,  //  0011  MOVE	R7	R1
      0x5C200800,  //  0012  MOVE	R8	R4
      0x5C240400,  //  0013  MOVE	R9	R2
      0x5C280600,  //  0014  MOVE	R10	R3
      0x7C140A00,  //  0015  CALL	R5	5
      0x80000000,  //  0016  RET	0
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
      0x8C0C0115,  //  0000  GETMET	R3	R0	K21
      0x58140004,  //  0001  LDCONST	R5	K4
      0x5C180400,  //  0002  MOVE	R6	R2
      0x7C0C0600,  //  0003  CALL	R3	3
      0x80040600,  //  0004  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update
********************************************************************/
be_local_closure(class_Animation_update,   /* name */
  be_nested_proto(
    8,                          /* nstack */
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
    ( &(const binstruction[18]) {  /* code */
      0x88080116,  //  0000  GETMBR	R2	R0	K22
      0x240C0501,  //  0001  GT	R3	R2	K1
      0x780E000D,  //  0002  JMPF	R3	#0011
      0x880C010E,  //  0003  GETMBR	R3	R0	K14
      0x040C0203,  //  0004  SUB	R3	R1	R3
      0x28100602,  //  0005  GE	R4	R3	R2
      0x78120009,  //  0006  JMPF	R4	#0011
      0x88100117,  //  0007  GETMBR	R4	R0	K23
      0x78120005,  //  0008  JMPF	R4	#000F
      0x0C140602,  //  0009  DIV	R5	R3	R2
      0x8818010E,  //  000A  GETMBR	R6	R0	K14
      0x081C0A02,  //  000B  MUL	R7	R5	R2
      0x00180C07,  //  000C  ADD	R6	R6	R7
      0x90021C06,  //  000D  SETMBR	R0	K14	R6
      0x70020001,  //  000E  JMP		#0011
      0x50140000,  //  000F  LDBOOL	R5	0	0
      0x90021805,  //  0010  SETMBR	R0	K12	R5
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Animation
********************************************************************/
extern const bclass be_class_parameterized_object;
be_local_class(Animation,
    1,
    &be_class_parameterized_object,
    be_nested_map(9,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(update, 1), be_const_closure(class_Animation_update_closure) },
        { be_const_key_weak(get_color_at, -1), be_const_closure(class_Animation_get_color_at_closure) },
        { be_const_key_weak(PARAMS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(id, -1), be_const_bytes_instance(0C030001) },
        { be_const_key_weak(priority, -1), be_const_bytes_instance(050000000A) },
        { be_const_key_weak(color, -1), be_const_bytes_instance(040000) },
        { be_const_key_weak(loop, 1), be_const_bytes_instance(0C050003) },
        { be_const_key_weak(opacity, 2), be_const_bytes_instance(0C01FF0004) },
        { be_const_key_weak(duration, -1), be_const_bytes_instance(0500000000) },
    }))    ) } )) },
        { be_const_key_weak(opacity_frame, -1), be_const_var(0) },
        { be_const_key_weak(_apply_opacity, -1), be_const_closure(class_Animation__apply_opacity_closure) },
        { be_const_key_weak(get_color, 8), be_const_closure(class_Animation_get_color_closure) },
        { be_const_key_weak(init, 2), be_const_closure(class_Animation_init_closure) },
        { be_const_key_weak(render, 0), be_const_closure(class_Animation_render_closure) },
        { be_const_key_weak(post_render, -1), be_const_closure(class_Animation_post_render_closure) },
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
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(oscillator_value),
    /* K2   */  be_nested_str_weak(form),
    }),
    be_str_weak(ease_in),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x540A0005,  //  0004  LDINT	R2	6
      0x90060402,  //  0005  SETMBR	R1	K2	R2
      0x80040200,  //  0006  RET	1	R1
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
    /* K3   */  be_const_int(2),
    }),
    be_str_weak(linear),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x90060503,  //  0004  SETMBR	R1	K2	K3
      0x80040200,  //  0005  RET	1	R1
    })
  )
);
/*******************************************************************/


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
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(oscillator_value),
    /* K2   */  be_nested_str_weak(form),
    }),
    be_str_weak(sine_osc),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x540A0004,  //  0004  LDINT	R2	5
      0x90060402,  //  0005  SETMBR	R1	K2	R2
      0x80040200,  //  0006  RET	1	R1
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

// compact class 'AnimationEngine' ktab size: 90, total: 211 (saved 968 bytes)
static const bvalue be_ktab_class_AnimationEngine[90] = {
  /* K0   */  be_nested_str_weak(is_running),
  /* K1   */  be_nested_str_weak(fast_loop_closure),
  /* K2   */  be_nested_str_weak(tasmota),
  /* K3   */  be_nested_str_weak(remove_fast_loop),
  /* K4   */  be_nested_str_weak(root_animation),
  /* K5   */  be_nested_str_weak(get_animations),
  /* K6   */  be_nested_str_weak(push_iteration_context),
  /* K7   */  be_nested_str_weak(millis),
  /* K8   */  be_nested_str_weak(last_update),
  /* K9   */  be_nested_str_weak(tick_ms),
  /* K10  */  be_nested_str_weak(ts_start),
  /* K11  */  be_nested_str_weak(check_strip_length),
  /* K12  */  be_nested_str_weak(time_ms),
  /* K13  */  be_nested_str_weak(strip),
  /* K14  */  be_nested_str_weak(can_show),
  /* K15  */  be_nested_str_weak(_process_events),
  /* K16  */  be_nested_str_weak(_update_and_render),
  /* K17  */  be_nested_str_weak(ts_end),
  /* K18  */  be_nested_str_weak(_record_tick_metrics),
  /* K19  */  be_nested_str_weak(global),
  /* K20  */  be_nested_str_weak(debug_animation),
  /* K21  */  be_nested_str_weak(remove),
  /* K22  */  be_nested_str_weak(render_needed),
  /* K23  */  be_nested_str_weak(tick_count),
  /* K24  */  be_const_int(0),
  /* K25  */  be_nested_str_weak(tick_time_sum),
  /* K26  */  be_nested_str_weak(anim_time_sum),
  /* K27  */  be_nested_str_weak(hw_time_sum),
  /* K28  */  be_nested_str_weak(phase1_time_sum),
  /* K29  */  be_nested_str_weak(phase2_time_sum),
  /* K30  */  be_nested_str_weak(phase3_time_sum),
  /* K31  */  be_nested_str_weak(ANI_X3A_X20ticks_X3D_X25s_X20total_X3D_X25_X2E2fms_X28_X25s_X2D_X25s_X29_X20events_X3D_X25_X2E2fms_X28_X25s_X2D_X25s_X29_X20update_X3D_X25_X2E2fms_X28_X25s_X2D_X25s_X29_X20anim_X3D_X25_X2E2fms_X28_X25s_X2D_X25s_X29_X20hw_X3D_X25_X2E2fms_X28_X25s_X2D_X25s_X29),
  /* K32  */  be_nested_str_weak(tick_time_min),
  /* K33  */  be_nested_str_weak(tick_time_max),
  /* K34  */  be_nested_str_weak(phase1_time_min),
  /* K35  */  be_nested_str_weak(phase1_time_max),
  /* K36  */  be_nested_str_weak(phase2_time_min),
  /* K37  */  be_nested_str_weak(phase2_time_max),
  /* K38  */  be_nested_str_weak(anim_time_min),
  /* K39  */  be_nested_str_weak(anim_time_max),
  /* K40  */  be_nested_str_weak(hw_time_min),
  /* K41  */  be_nested_str_weak(hw_time_max),
  /* K42  */  be_nested_str_weak(log),
  /* K43  */  be_const_int(3),
  /* K44  */  be_nested_str_weak(children),
  /* K45  */  be_nested_str_weak(animation),
  /* K46  */  be_nested_str_weak(id),
  /* K47  */  be_nested_str_weak(stop),
  /* K48  */  be_const_int(1),
  /* K49  */  be_nested_str_weak(push_pixels_buffer_argb),
  /* K50  */  be_nested_str_weak(frame_buffer),
  /* K51  */  be_nested_str_weak(pixels),
  /* K52  */  be_nested_str_weak(show),
  /* K53  */  be_nested_str_weak(sequences),
  /* K54  */  be_nested_str_weak(clear),
  /* K55  */  be_nested_str_weak(strip_length),
  /* K56  */  be_nested_str_weak(update_current_iteration),
  /* K57  */  be_nested_str_weak(event_manager),
  /* K58  */  be_nested_str_weak(_process_queued_events),
  /* K59  */  be_nested_str_weak(start),
  /* K60  */  be_nested_str_weak(ts_2),
  /* K61  */  be_nested_str_weak(ts_3),
  /* K62  */  be_nested_str_weak(ts_hw),
  /* K63  */  be_nested_str_weak(ts_1),
  /* K64  */  be_nested_str_weak(last_stats_time),
  /* K65  */  be_nested_str_weak(phase3_time_min),
  /* K66  */  be_nested_str_weak(phase3_time_max),
  /* K67  */  be_nested_str_weak(stats_period),
  /* K68  */  be_nested_str_weak(_print_stats),
  /* K69  */  be_const_int(999999),
  /* K70  */  be_nested_str_weak(AnimationEngine_X28running_X3D_X25s_X29),
  /* K71  */  be_nested_str_weak(add_fast_loop),
  /* K72  */  be_nested_str_weak(update),
  /* K73  */  be_nested_str_weak(is_empty),
  /* K74  */  be_nested_str_weak(_clear_strip),
  /* K75  */  be_nested_str_weak(render),
  /* K76  */  be_nested_str_weak(_output_to_strip),
  /* K77  */  be_nested_str_weak(temp_buffer),
  /* K78  */  be_nested_str_weak(length),
  /* K79  */  be_nested_str_weak(_handle_strip_length_change),
  /* K80  */  be_nested_str_weak(get_current_iteration_number),
  /* K81  */  be_nested_str_weak(size_animations),
  /* K82  */  be_nested_str_weak(set_timer),
  /* K83  */  be_nested_str_weak(add),
  /* K84  */  be_nested_str_weak(value_error),
  /* K85  */  be_nested_str_weak(strip_X20cannot_X20be_X20nil),
  /* K86  */  be_nested_str_weak(engine_proxy),
  /* K87  */  be_nested_str_weak(TICK_MS),
  /* K88  */  be_nested_str_weak(resize),
  /* K89  */  be_nested_str_weak(pop_iteration_context),
};


extern const bclass be_class_AnimationEngine;

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
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060009,  //  0001  JMPF	R1	#000C
      0x50040000,  //  0002  LDBOOL	R1	0	0
      0x90020001,  //  0003  SETMBR	R0	K0	R1
      0x88040101,  //  0004  GETMBR	R1	R0	K1
      0x4C080000,  //  0005  LDNIL	R2
      0x20040202,  //  0006  NE	R1	R1	R2
      0x78060003,  //  0007  JMPF	R1	#000C
      0xB8060400,  //  0008  GETNGBL	R1	K2
      0x8C040303,  //  0009  GETMET	R1	R1	K3
      0x880C0101,  //  000A  GETMBR	R3	R0	K1
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
    3,                          /* nstack */
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
    ( &(const binstruction[ 4]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x8C040305,  //  0001  GETMET	R1	R1	K5
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
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
      0x88080104,  //  0000  GETMBR	R2	R0	K4
      0x8C080506,  //  0001  GETMET	R2	R2	K6
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: animations
********************************************************************/
be_local_closure(class_AnimationEngine_animations,   /* name */
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
    be_str_weak(animations),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x8C040105,  //  0000  GETMET	R1	R0	K5
      0x7C040200,  //  0001  CALL	R1	1
      0x80040200,  //  0002  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: on_tick
********************************************************************/
be_local_closure(class_AnimationEngine_on_tick,   /* name */
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
    be_str_weak(on_tick),
    &be_const_str_solidified,
    ( &(const binstruction[55]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x740A0001,  //  0001  JMPT	R2	#0004
      0x50080000,  //  0002  LDBOOL	R2	0	0
      0x80040400,  //  0003  RET	1	R2
      0x4C080000,  //  0004  LDNIL	R2
      0x1C080202,  //  0005  EQ	R2	R1	R2
      0x780A0003,  //  0006  JMPF	R2	#000B
      0xB80A0400,  //  0007  GETNGBL	R2	K2
      0x8C080507,  //  0008  GETMET	R2	R2	K7
      0x7C080200,  //  0009  CALL	R2	1
      0x5C040400,  //  000A  MOVE	R1	R2
      0x88080108,  //  000B  GETMBR	R2	R0	K8
      0x04080202,  //  000C  SUB	R2	R1	R2
      0x880C0109,  //  000D  GETMBR	R3	R0	K9
      0x140C0403,  //  000E  LT	R3	R2	R3
      0x780E0001,  //  000F  JMPF	R3	#0012
      0x500C0200,  //  0010  LDBOOL	R3	1	0
      0x80040600,  //  0011  RET	1	R3
      0xB80E0400,  //  0012  GETNGBL	R3	K2
      0x8C0C0707,  //  0013  GETMET	R3	R3	K7
      0x7C0C0200,  //  0014  CALL	R3	1
      0x90021403,  //  0015  SETMBR	R0	K10	R3
      0x8C0C010B,  //  0016  GETMET	R3	R0	K11
      0x7C0C0200,  //  0017  CALL	R3	1
      0x90021801,  //  0018  SETMBR	R0	K12	R1
      0x90021001,  //  0019  SETMBR	R0	K8	R1
      0x880C010D,  //  001A  GETMBR	R3	R0	K13
      0x880C070E,  //  001B  GETMBR	R3	R3	K14
      0x4C100000,  //  001C  LDNIL	R4
      0x200C0604,  //  001D  NE	R3	R3	R4
      0x780E0005,  //  001E  JMPF	R3	#0025
      0x880C010D,  //  001F  GETMBR	R3	R0	K13
      0x8C0C070E,  //  0020  GETMET	R3	R3	K14
      0x7C0C0200,  //  0021  CALL	R3	1
      0x740E0001,  //  0022  JMPT	R3	#0025
      0x500C0200,  //  0023  LDBOOL	R3	1	0
      0x80040600,  //  0024  RET	1	R3
      0x8C0C010F,  //  0025  GETMET	R3	R0	K15
      0x5C140200,  //  0026  MOVE	R5	R1
      0x7C0C0400,  //  0027  CALL	R3	2
      0x8C0C0110,  //  0028  GETMET	R3	R0	K16
      0x5C140200,  //  0029  MOVE	R5	R1
      0x7C0C0400,  //  002A  CALL	R3	2
      0xB80E0400,  //  002B  GETNGBL	R3	K2
      0x8C0C0707,  //  002C  GETMET	R3	R3	K7
      0x7C0C0200,  //  002D  CALL	R3	1
      0x90022203,  //  002E  SETMBR	R0	K17	R3
      0x8C0C0112,  //  002F  GETMET	R3	R0	K18
      0x5C140200,  //  0030  MOVE	R5	R1
      0x7C0C0400,  //  0031  CALL	R3	2
      0xB80E2600,  //  0032  GETNGBL	R3	K19
      0x50100000,  //  0033  LDBOOL	R4	0	0
      0x900E2804,  //  0034  SETMBR	R3	K20	R4
      0x500C0200,  //  0035  LDBOOL	R3	1	0
      0x80040600,  //  0036  RET	1	R3
    })
  )
);
/*******************************************************************/


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
    ( &(const binstruction[ 8]) {  /* code */
      0x88080104,  //  0000  GETMBR	R2	R0	K4
      0x8C080515,  //  0001  GETMET	R2	R2	K21
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x780A0001,  //  0004  JMPF	R2	#0007
      0x500C0200,  //  0005  LDBOOL	R3	1	0
      0x90022C03,  //  0006  SETMBR	R0	K22	R3
      0x80040400,  //  0007  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _print_stats
********************************************************************/
be_local_closure(class_AnimationEngine__print_stats,   /* name */
  be_nested_proto(
    26,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationEngine,     /* shared constants */
    be_str_weak(_print_stats),
    &be_const_str_solidified,
    ( &(const binstruction[47]) {  /* code */
      0x88080117,  //  0000  GETMBR	R2	R0	K23
      0x1C080518,  //  0001  EQ	R2	R2	K24
      0x780A0000,  //  0002  JMPF	R2	#0004
      0x80000400,  //  0003  RET	0
      0x88080119,  //  0004  GETMBR	R2	R0	K25
      0x880C0117,  //  0005  GETMBR	R3	R0	K23
      0x0C080403,  //  0006  DIV	R2	R2	R3
      0x880C011A,  //  0007  GETMBR	R3	R0	K26
      0x88100117,  //  0008  GETMBR	R4	R0	K23
      0x0C0C0604,  //  0009  DIV	R3	R3	R4
      0x8810011B,  //  000A  GETMBR	R4	R0	K27
      0x88140117,  //  000B  GETMBR	R5	R0	K23
      0x0C100805,  //  000C  DIV	R4	R4	R5
      0x8814011C,  //  000D  GETMBR	R5	R0	K28
      0x88180117,  //  000E  GETMBR	R6	R0	K23
      0x0C140A06,  //  000F  DIV	R5	R5	R6
      0x8818011D,  //  0010  GETMBR	R6	R0	K29
      0x881C0117,  //  0011  GETMBR	R7	R0	K23
      0x0C180C07,  //  0012  DIV	R6	R6	R7
      0x881C011E,  //  0013  GETMBR	R7	R0	K30
      0x88200117,  //  0014  GETMBR	R8	R0	K23
      0x0C1C0E08,  //  0015  DIV	R7	R7	R8
      0x60200018,  //  0016  GETGBL	R8	G24
      0x5824001F,  //  0017  LDCONST	R9	K31
      0x88280117,  //  0018  GETMBR	R10	R0	K23
      0x5C2C0400,  //  0019  MOVE	R11	R2
      0x88300120,  //  001A  GETMBR	R12	R0	K32
      0x88340121,  //  001B  GETMBR	R13	R0	K33
      0x5C380A00,  //  001C  MOVE	R14	R5
      0x883C0122,  //  001D  GETMBR	R15	R0	K34
      0x88400123,  //  001E  GETMBR	R16	R0	K35
      0x5C440C00,  //  001F  MOVE	R17	R6
      0x88480124,  //  0020  GETMBR	R18	R0	K36
      0x884C0125,  //  0021  GETMBR	R19	R0	K37
      0x5C500600,  //  0022  MOVE	R20	R3
      0x88540126,  //  0023  GETMBR	R21	R0	K38
      0x88580127,  //  0024  GETMBR	R22	R0	K39
      0x5C5C0800,  //  0025  MOVE	R23	R4
      0x88600128,  //  0026  GETMBR	R24	R0	K40
      0x88640129,  //  0027  GETMBR	R25	R0	K41
      0x7C202200,  //  0028  CALL	R8	17
      0xB8260400,  //  0029  GETNGBL	R9	K2
      0x8C24132A,  //  002A  GETMET	R9	R9	K42
      0x5C2C1000,  //  002B  MOVE	R11	R8
      0x5830002B,  //  002C  LDCONST	R12	K43
      0x7C240600,  //  002D  CALL	R9	3
      0x80000000,  //  002E  RET	0
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
    ( &(const binstruction[30]) {  /* code */
      0x58080018,  //  0000  LDCONST	R2	K24
      0x600C000C,  //  0001  GETGBL	R3	G12
      0x88100104,  //  0002  GETMBR	R4	R0	K4
      0x8810092C,  //  0003  GETMBR	R4	R4	K44
      0x7C0C0200,  //  0004  CALL	R3	1
      0x140C0403,  //  0005  LT	R3	R2	R3
      0x780E0015,  //  0006  JMPF	R3	#001D
      0x880C0104,  //  0007  GETMBR	R3	R0	K4
      0x880C072C,  //  0008  GETMBR	R3	R3	K44
      0x940C0602,  //  0009  GETIDX	R3	R3	R2
      0x6010000F,  //  000A  GETGBL	R4	G15
      0x5C140600,  //  000B  MOVE	R5	R3
      0xB81A5A00,  //  000C  GETNGBL	R6	K45
      0x88180D2D,  //  000D  GETMBR	R6	R6	K45
      0x7C100400,  //  000E  CALL	R4	2
      0x7812000A,  //  000F  JMPF	R4	#001B
      0x8810072E,  //  0010  GETMBR	R4	R3	K46
      0x1C100801,  //  0011  EQ	R4	R4	R1
      0x78120007,  //  0012  JMPF	R4	#001B
      0x8C10072F,  //  0013  GETMET	R4	R3	K47
      0x7C100200,  //  0014  CALL	R4	1
      0x88100104,  //  0015  GETMBR	R4	R0	K4
      0x8810092C,  //  0016  GETMBR	R4	R4	K44
      0x8C100915,  //  0017  GETMET	R4	R4	K21
      0x5C180400,  //  0018  MOVE	R6	R2
      0x7C100400,  //  0019  CALL	R4	2
      0x80000800,  //  001A  RET	0
      0x00080530,  //  001B  ADD	R2	R2	K48
      0x7001FFE3,  //  001C  JMP		#0001
      0x80000000,  //  001D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _output_to_strip
********************************************************************/
be_local_closure(class_AnimationEngine__output_to_strip,   /* name */
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
    be_str_weak(_output_to_strip),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x8804010D,  //  0000  GETMBR	R1	R0	K13
      0x8C040331,  //  0001  GETMET	R1	R1	K49
      0x880C0132,  //  0002  GETMBR	R3	R0	K50
      0x880C0733,  //  0003  GETMBR	R3	R3	K51
      0x7C040400,  //  0004  CALL	R1	2
      0x8804010D,  //  0005  GETMBR	R1	R0	K13
      0x8C040334,  //  0006  GETMET	R1	R1	K52
      0x7C040200,  //  0007  CALL	R1	1
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: sequence_managers
********************************************************************/
be_local_closure(class_AnimationEngine_sequence_managers,   /* name */
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
    be_str_weak(sequence_managers),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x88040335,  //  0001  GETMBR	R1	R1	K53
      0x80040200,  //  0002  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: clear
********************************************************************/
be_local_closure(class_AnimationEngine_clear,   /* name */
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
    be_str_weak(clear),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x8C040336,  //  0001  GETMET	R1	R1	K54
      0x7C040200,  //  0002  CALL	R1	1
      0x50040200,  //  0003  LDBOOL	R1	1	0
      0x90022C01,  //  0004  SETMBR	R0	K22	R1
      0x80040000,  //  0005  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_strip_length
********************************************************************/
be_local_closure(class_AnimationEngine_get_strip_length,   /* name */
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
    be_str_weak(get_strip_length),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040137,  //  0000  GETMBR	R1	R0	K55
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_current_iteration
********************************************************************/
be_local_closure(class_AnimationEngine_update_current_iteration,   /* name */
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
    be_str_weak(update_current_iteration),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080104,  //  0000  GETMBR	R2	R0	K4
      0x8C080538,  //  0001  GETMET	R2	R2	K56
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80040400,  //  0004  RET	1	R2
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
      0xB80A5A00,  //  0000  GETNGBL	R2	K45
      0x88080539,  //  0001  GETMBR	R2	R2	K57
      0x4C0C0000,  //  0002  LDNIL	R3
      0x20080403,  //  0003  NE	R2	R2	R3
      0x780A0003,  //  0004  JMPF	R2	#0009
      0xB80A5A00,  //  0005  GETNGBL	R2	K45
      0x88080539,  //  0006  GETMBR	R2	R2	K57
      0x8C08053A,  //  0007  GETMET	R2	R2	K58
      0x7C080200,  //  0008  CALL	R2	1
      0x80000000,  //  0009  RET	0
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
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x74060001,  //  0001  JMPT	R1	#0004
      0x8C04013B,  //  0002  GETMET	R1	R0	K59
      0x7C040200,  //  0003  CALL	R1	1
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _record_tick_metrics
********************************************************************/
be_local_closure(class_AnimationEngine__record_tick_metrics,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationEngine,     /* shared constants */
    be_str_weak(_record_tick_metrics),
    &be_const_str_solidified,
    ( &(const binstruction[198]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x4C0C0000,  //  0001  LDNIL	R3
      0x4C100000,  //  0002  LDNIL	R4
      0x4C140000,  //  0003  LDNIL	R5
      0x4C180000,  //  0004  LDNIL	R6
      0x4C1C0000,  //  0005  LDNIL	R7
      0x8820010A,  //  0006  GETMBR	R8	R0	K10
      0x4C240000,  //  0007  LDNIL	R9
      0x20201009,  //  0008  NE	R8	R8	R9
      0x78220007,  //  0009  JMPF	R8	#0012
      0x88200111,  //  000A  GETMBR	R8	R0	K17
      0x4C240000,  //  000B  LDNIL	R9
      0x20201009,  //  000C  NE	R8	R8	R9
      0x78220003,  //  000D  JMPF	R8	#0012
      0x88200111,  //  000E  GETMBR	R8	R0	K17
      0x8824010A,  //  000F  GETMBR	R9	R0	K10
      0x04201009,  //  0010  SUB	R8	R8	R9
      0x5C081000,  //  0011  MOVE	R2	R8
      0x8820013C,  //  0012  GETMBR	R8	R0	K60
      0x4C240000,  //  0013  LDNIL	R9
      0x20201009,  //  0014  NE	R8	R8	R9
      0x78220007,  //  0015  JMPF	R8	#001E
      0x8820013D,  //  0016  GETMBR	R8	R0	K61
      0x4C240000,  //  0017  LDNIL	R9
      0x20201009,  //  0018  NE	R8	R8	R9
      0x78220003,  //  0019  JMPF	R8	#001E
      0x8820013D,  //  001A  GETMBR	R8	R0	K61
      0x8824013C,  //  001B  GETMBR	R9	R0	K60
      0x04201009,  //  001C  SUB	R8	R8	R9
      0x5C0C1000,  //  001D  MOVE	R3	R8
      0x8820013D,  //  001E  GETMBR	R8	R0	K61
      0x4C240000,  //  001F  LDNIL	R9
      0x20201009,  //  0020  NE	R8	R8	R9
      0x78220007,  //  0021  JMPF	R8	#002A
      0x8820013E,  //  0022  GETMBR	R8	R0	K62
      0x4C240000,  //  0023  LDNIL	R9
      0x20201009,  //  0024  NE	R8	R8	R9
      0x78220003,  //  0025  JMPF	R8	#002A
      0x8820013E,  //  0026  GETMBR	R8	R0	K62
      0x8824013D,  //  0027  GETMBR	R9	R0	K61
      0x04201009,  //  0028  SUB	R8	R8	R9
      0x5C101000,  //  0029  MOVE	R4	R8
      0x8820010A,  //  002A  GETMBR	R8	R0	K10
      0x4C240000,  //  002B  LDNIL	R9
      0x20201009,  //  002C  NE	R8	R8	R9
      0x78220007,  //  002D  JMPF	R8	#0036
      0x8820013F,  //  002E  GETMBR	R8	R0	K63
      0x4C240000,  //  002F  LDNIL	R9
      0x20201009,  //  0030  NE	R8	R8	R9
      0x78220003,  //  0031  JMPF	R8	#0036
      0x8820013F,  //  0032  GETMBR	R8	R0	K63
      0x8824010A,  //  0033  GETMBR	R9	R0	K10
      0x04201009,  //  0034  SUB	R8	R8	R9
      0x5C141000,  //  0035  MOVE	R5	R8
      0x8820013F,  //  0036  GETMBR	R8	R0	K63
      0x4C240000,  //  0037  LDNIL	R9
      0x20201009,  //  0038  NE	R8	R8	R9
      0x78220007,  //  0039  JMPF	R8	#0042
      0x8820013C,  //  003A  GETMBR	R8	R0	K60
      0x4C240000,  //  003B  LDNIL	R9
      0x20201009,  //  003C  NE	R8	R8	R9
      0x78220003,  //  003D  JMPF	R8	#0042
      0x8820013C,  //  003E  GETMBR	R8	R0	K60
      0x8824013F,  //  003F  GETMBR	R9	R0	K63
      0x04201009,  //  0040  SUB	R8	R8	R9
      0x5C181000,  //  0041  MOVE	R6	R8
      0x8820013C,  //  0042  GETMBR	R8	R0	K60
      0x4C240000,  //  0043  LDNIL	R9
      0x20201009,  //  0044  NE	R8	R8	R9
      0x78220007,  //  0045  JMPF	R8	#004E
      0x8820013D,  //  0046  GETMBR	R8	R0	K61
      0x4C240000,  //  0047  LDNIL	R9
      0x20201009,  //  0048  NE	R8	R8	R9
      0x78220003,  //  0049  JMPF	R8	#004E
      0x8820013D,  //  004A  GETMBR	R8	R0	K61
      0x8824013C,  //  004B  GETMBR	R9	R0	K60
      0x04201009,  //  004C  SUB	R8	R8	R9
      0x5C1C1000,  //  004D  MOVE	R7	R8
      0x88200140,  //  004E  GETMBR	R8	R0	K64
      0x1C201118,  //  004F  EQ	R8	R8	K24
      0x78220000,  //  0050  JMPF	R8	#0052
      0x90028001,  //  0051  SETMBR	R0	K64	R1
      0x88200117,  //  0052  GETMBR	R8	R0	K23
      0x00201130,  //  0053  ADD	R8	R8	K48
      0x90022E08,  //  0054  SETMBR	R0	K23	R8
      0x4C200000,  //  0055  LDNIL	R8
      0x20200408,  //  0056  NE	R8	R2	R8
      0x7822000A,  //  0057  JMPF	R8	#0063
      0x88200119,  //  0058  GETMBR	R8	R0	K25
      0x00201002,  //  0059  ADD	R8	R8	R2
      0x90023208,  //  005A  SETMBR	R0	K25	R8
      0x88200120,  //  005B  GETMBR	R8	R0	K32
      0x14200408,  //  005C  LT	R8	R2	R8
      0x78220000,  //  005D  JMPF	R8	#005F
      0x90024002,  //  005E  SETMBR	R0	K32	R2
      0x88200121,  //  005F  GETMBR	R8	R0	K33
      0x24200408,  //  0060  GT	R8	R2	R8
      0x78220000,  //  0061  JMPF	R8	#0063
      0x90024202,  //  0062  SETMBR	R0	K33	R2
      0x4C200000,  //  0063  LDNIL	R8
      0x20200608,  //  0064  NE	R8	R3	R8
      0x7822000A,  //  0065  JMPF	R8	#0071
      0x8820011A,  //  0066  GETMBR	R8	R0	K26
      0x00201003,  //  0067  ADD	R8	R8	R3
      0x90023408,  //  0068  SETMBR	R0	K26	R8
      0x88200126,  //  0069  GETMBR	R8	R0	K38
      0x14200608,  //  006A  LT	R8	R3	R8
      0x78220000,  //  006B  JMPF	R8	#006D
      0x90024C03,  //  006C  SETMBR	R0	K38	R3
      0x88200127,  //  006D  GETMBR	R8	R0	K39
      0x24200608,  //  006E  GT	R8	R3	R8
      0x78220000,  //  006F  JMPF	R8	#0071
      0x90024E03,  //  0070  SETMBR	R0	K39	R3
      0x4C200000,  //  0071  LDNIL	R8
      0x20200808,  //  0072  NE	R8	R4	R8
      0x7822000A,  //  0073  JMPF	R8	#007F
      0x8820011B,  //  0074  GETMBR	R8	R0	K27
      0x00201004,  //  0075  ADD	R8	R8	R4
      0x90023608,  //  0076  SETMBR	R0	K27	R8
      0x88200128,  //  0077  GETMBR	R8	R0	K40
      0x14200808,  //  0078  LT	R8	R4	R8
      0x78220000,  //  0079  JMPF	R8	#007B
      0x90025004,  //  007A  SETMBR	R0	K40	R4
      0x88200129,  //  007B  GETMBR	R8	R0	K41
      0x24200808,  //  007C  GT	R8	R4	R8
      0x78220000,  //  007D  JMPF	R8	#007F
      0x90025204,  //  007E  SETMBR	R0	K41	R4
      0x4C200000,  //  007F  LDNIL	R8
      0x20200A08,  //  0080  NE	R8	R5	R8
      0x7822000A,  //  0081  JMPF	R8	#008D
      0x8820011C,  //  0082  GETMBR	R8	R0	K28
      0x00201005,  //  0083  ADD	R8	R8	R5
      0x90023808,  //  0084  SETMBR	R0	K28	R8
      0x88200122,  //  0085  GETMBR	R8	R0	K34
      0x14200A08,  //  0086  LT	R8	R5	R8
      0x78220000,  //  0087  JMPF	R8	#0089
      0x90024405,  //  0088  SETMBR	R0	K34	R5
      0x88200123,  //  0089  GETMBR	R8	R0	K35
      0x24200A08,  //  008A  GT	R8	R5	R8
      0x78220000,  //  008B  JMPF	R8	#008D
      0x90024605,  //  008C  SETMBR	R0	K35	R5
      0x4C200000,  //  008D  LDNIL	R8
      0x20200C08,  //  008E  NE	R8	R6	R8
      0x7822000A,  //  008F  JMPF	R8	#009B
      0x8820011D,  //  0090  GETMBR	R8	R0	K29
      0x00201006,  //  0091  ADD	R8	R8	R6
      0x90023A08,  //  0092  SETMBR	R0	K29	R8
      0x88200124,  //  0093  GETMBR	R8	R0	K36
      0x14200C08,  //  0094  LT	R8	R6	R8
      0x78220000,  //  0095  JMPF	R8	#0097
      0x90024806,  //  0096  SETMBR	R0	K36	R6
      0x88200125,  //  0097  GETMBR	R8	R0	K37
      0x24200C08,  //  0098  GT	R8	R6	R8
      0x78220000,  //  0099  JMPF	R8	#009B
      0x90024A06,  //  009A  SETMBR	R0	K37	R6
      0x4C200000,  //  009B  LDNIL	R8
      0x20200E08,  //  009C  NE	R8	R7	R8
      0x7822000A,  //  009D  JMPF	R8	#00A9
      0x8820011E,  //  009E  GETMBR	R8	R0	K30
      0x00201007,  //  009F  ADD	R8	R8	R7
      0x90023C08,  //  00A0  SETMBR	R0	K30	R8
      0x88200141,  //  00A1  GETMBR	R8	R0	K65
      0x14200E08,  //  00A2  LT	R8	R7	R8
      0x78220000,  //  00A3  JMPF	R8	#00A5
      0x90028207,  //  00A4  SETMBR	R0	K65	R7
      0x88200142,  //  00A5  GETMBR	R8	R0	K66
      0x24200E08,  //  00A6  GT	R8	R7	R8
      0x78220000,  //  00A7  JMPF	R8	#00A9
      0x90028407,  //  00A8  SETMBR	R0	K66	R7
      0x88200140,  //  00A9  GETMBR	R8	R0	K64
      0x04200208,  //  00AA  SUB	R8	R1	R8
      0x88240143,  //  00AB  GETMBR	R9	R0	K67
      0x28241009,  //  00AC  GE	R9	R8	R9
      0x78260016,  //  00AD  JMPF	R9	#00C5
      0x8C240144,  //  00AE  GETMET	R9	R0	K68
      0x5C2C1000,  //  00AF  MOVE	R11	R8
      0x7C240400,  //  00B0  CALL	R9	2
      0x90022F18,  //  00B1  SETMBR	R0	K23	K24
      0x90023318,  //  00B2  SETMBR	R0	K25	K24
      0x90024145,  //  00B3  SETMBR	R0	K32	K69
      0x90024318,  //  00B4  SETMBR	R0	K33	K24
      0x90023518,  //  00B5  SETMBR	R0	K26	K24
      0x90024D45,  //  00B6  SETMBR	R0	K38	K69
      0x90024F18,  //  00B7  SETMBR	R0	K39	K24
      0x90023718,  //  00B8  SETMBR	R0	K27	K24
      0x90025145,  //  00B9  SETMBR	R0	K40	K69
      0x90025318,  //  00BA  SETMBR	R0	K41	K24
      0x90023918,  //  00BB  SETMBR	R0	K28	K24
      0x90024545,  //  00BC  SETMBR	R0	K34	K69
      0x90024718,  //  00BD  SETMBR	R0	K35	K24
      0x90023B18,  //  00BE  SETMBR	R0	K29	K24
      0x90024945,  //  00BF  SETMBR	R0	K36	K69
      0x90024B18,  //  00C0  SETMBR	R0	K37	K24
      0x90023D18,  //  00C1  SETMBR	R0	K30	K24
      0x90028345,  //  00C2  SETMBR	R0	K65	K69
      0x90028518,  //  00C3  SETMBR	R0	K66	K24
      0x90028001,  //  00C4  SETMBR	R0	K64	R1
      0x80000000,  //  00C5  RET	0
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
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
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
      0x58080046,  //  0001  LDCONST	R2	K70
      0x880C0100,  //  0002  GETMBR	R3	R0	K0
      0x7C040400,  //  0003  CALL	R1	2
      0x80040200,  //  0004  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: run
********************************************************************/
be_local_closure(class_AnimationEngine_run,   /* name */
  be_nested_proto(
    5,                          /* nstack */
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
    ( &(const binstruction[26]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x74060015,  //  0001  JMPT	R1	#0018
      0xB8060400,  //  0002  GETNGBL	R1	K2
      0x8C040307,  //  0003  GETMET	R1	R1	K7
      0x7C040200,  //  0004  CALL	R1	1
      0x50080200,  //  0005  LDBOOL	R2	1	0
      0x90020002,  //  0006  SETMBR	R0	K0	R2
      0x540A0009,  //  0007  LDINT	R2	10
      0x04080202,  //  0008  SUB	R2	R1	R2
      0x90021002,  //  0009  SETMBR	R0	K8	R2
      0x88080101,  //  000A  GETMBR	R2	R0	K1
      0x4C0C0000,  //  000B  LDNIL	R3
      0x1C080403,  //  000C  EQ	R2	R2	R3
      0x780A0001,  //  000D  JMPF	R2	#0010
      0x84080000,  //  000E  CLOSURE	R2	P0
      0x90020202,  //  000F  SETMBR	R0	K1	R2
      0x88080104,  //  0010  GETMBR	R2	R0	K4
      0x8C08053B,  //  0011  GETMET	R2	R2	K59
      0x5C100200,  //  0012  MOVE	R4	R1
      0x7C080400,  //  0013  CALL	R2	2
      0xB80A0400,  //  0014  GETNGBL	R2	K2
      0x8C080547,  //  0015  GETMET	R2	R2	K71
      0x88100101,  //  0016  GETMBR	R4	R0	K1
      0x7C080400,  //  0017  CALL	R2	2
      0xA0000000,  //  0018  CLOSE	R0
      0x80040000,  //  0019  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _update_and_render
********************************************************************/
be_local_closure(class_AnimationEngine__update_and_render,   /* name */
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
    be_str_weak(_update_and_render),
    &be_const_str_solidified,
    ( &(const binstruction[44]) {  /* code */
      0xB80A0400,  //  0000  GETNGBL	R2	K2
      0x8C080507,  //  0001  GETMET	R2	R2	K7
      0x7C080200,  //  0002  CALL	R2	1
      0x90027E02,  //  0003  SETMBR	R0	K63	R2
      0x88080104,  //  0004  GETMBR	R2	R0	K4
      0x8C080548,  //  0005  GETMET	R2	R2	K72
      0x5C100200,  //  0006  MOVE	R4	R1
      0x7C080400,  //  0007  CALL	R2	2
      0xB80A0400,  //  0008  GETNGBL	R2	K2
      0x8C080507,  //  0009  GETMET	R2	R2	K7
      0x7C080200,  //  000A  CALL	R2	1
      0x90027802,  //  000B  SETMBR	R0	K60	R2
      0x88080104,  //  000C  GETMBR	R2	R0	K4
      0x8C080549,  //  000D  GETMET	R2	R2	K73
      0x7C080200,  //  000E  CALL	R2	1
      0x780A0006,  //  000F  JMPF	R2	#0017
      0x88080116,  //  0010  GETMBR	R2	R0	K22
      0x780A0003,  //  0011  JMPF	R2	#0016
      0x8C08014A,  //  0012  GETMET	R2	R0	K74
      0x7C080200,  //  0013  CALL	R2	1
      0x50080000,  //  0014  LDBOOL	R2	0	0
      0x90022C02,  //  0015  SETMBR	R0	K22	R2
      0x80000400,  //  0016  RET	0
      0x88080132,  //  0017  GETMBR	R2	R0	K50
      0x8C080536,  //  0018  GETMET	R2	R2	K54
      0x7C080200,  //  0019  CALL	R2	1
      0x88080104,  //  001A  GETMBR	R2	R0	K4
      0x8C08054B,  //  001B  GETMET	R2	R2	K75
      0x88100132,  //  001C  GETMBR	R4	R0	K50
      0x5C140200,  //  001D  MOVE	R5	R1
      0x7C080600,  //  001E  CALL	R2	3
      0xB80E0400,  //  001F  GETNGBL	R3	K2
      0x8C0C0707,  //  0020  GETMET	R3	R3	K7
      0x7C0C0200,  //  0021  CALL	R3	1
      0x90027A03,  //  0022  SETMBR	R0	K61	R3
      0x8C0C014C,  //  0023  GETMET	R3	R0	K76
      0x7C0C0200,  //  0024  CALL	R3	1
      0xB80E0400,  //  0025  GETNGBL	R3	K2
      0x8C0C0707,  //  0026  GETMET	R3	R3	K7
      0x7C0C0200,  //  0027  CALL	R3	1
      0x90027C03,  //  0028  SETMBR	R0	K62	R3
      0x500C0000,  //  0029  LDBOOL	R3	0	0
      0x90022C03,  //  002A  SETMBR	R0	K22	R3
      0x80000000,  //  002B  RET	0
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
      0x8C04012F,  //  0000  GETMET	R1	R0	K47
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040136,  //  0002  GETMET	R1	R0	K54
      0x7C040200,  //  0003  CALL	R1	1
      0x4C040000,  //  0004  LDNIL	R1
      0x90026401,  //  0005  SETMBR	R0	K50	R1
      0x4C040000,  //  0006  LDNIL	R1
      0x90029A01,  //  0007  SETMBR	R0	K77	R1
      0x4C040000,  //  0008  LDNIL	R1
      0x90021A01,  //  0009  SETMBR	R0	K13	R1
      0x80000000,  //  000A  RET	0
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
      0x8804010D,  //  0000  GETMBR	R1	R0	K13
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
    3,                          /* nstack */
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
    ( &(const binstruction[ 4]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x8C04032F,  //  0001  GETMET	R1	R1	K47
      0x7C040200,  //  0002  CALL	R1	1
      0x80000000,  //  0003  RET	0
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
      0x8804010D,  //  0000  GETMBR	R1	R0	K13
      0x8C04034E,  //  0001  GETMET	R1	R1	K78
      0x7C040200,  //  0002  CALL	R1	1
      0x88080137,  //  0003  GETMBR	R2	R0	K55
      0x20080202,  //  0004  NE	R2	R1	R2
      0x780A0004,  //  0005  JMPF	R2	#000B
      0x8C08014F,  //  0006  GETMET	R2	R0	K79
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
    ( &(const binstruction[ 4]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x8C040350,  //  0001  GETMET	R1	R1	K80
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
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
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x8C040351,  //  0001  GETMET	R1	R1	K81
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
    6,                          /* nstack */
    2,                          /* argc */
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
        /* K0   */  be_nested_str_weak(resume),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[ 4]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0x80000000,  //  0003  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    &be_ktab_class_AnimationEngine,     /* shared constants */
    be_str_weak(resume_after),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0xB80A0400,  //  0000  GETNGBL	R2	K2
      0x8C080552,  //  0001  GETMET	R2	R2	K82
      0x5C100200,  //  0002  MOVE	R4	R1
      0x84140000,  //  0003  CLOSURE	R5	P0
      0x7C080600,  //  0004  CALL	R2	3
      0xA0000000,  //  0005  CLOSE	R0
      0x80000000,  //  0006  RET	0
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
    ( &(const binstruction[ 8]) {  /* code */
      0x88080104,  //  0000  GETMBR	R2	R0	K4
      0x8C080553,  //  0001  GETMET	R2	R2	K83
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x780A0001,  //  0004  JMPF	R2	#0007
      0x500C0200,  //  0005  LDBOOL	R3	1	0
      0x90022C03,  //  0006  SETMBR	R0	K22	R3
      0x80040400,  //  0007  RET	1	R2
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
    ( &(const binstruction[68]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0000,  //  0002  JMPF	R2	#0004
      0xB006A955,  //  0003  RAISE	1	K84	K85
      0x90021A01,  //  0004  SETMBR	R0	K13	R1
      0x8C08034E,  //  0005  GETMET	R2	R1	K78
      0x7C080200,  //  0006  CALL	R2	1
      0x90026E02,  //  0007  SETMBR	R0	K55	R2
      0xB80A5A00,  //  0008  GETNGBL	R2	K45
      0x8C080532,  //  0009  GETMET	R2	R2	K50
      0x88100137,  //  000A  GETMBR	R4	R0	K55
      0x7C080400,  //  000B  CALL	R2	2
      0x90026402,  //  000C  SETMBR	R0	K50	R2
      0xB80A5A00,  //  000D  GETNGBL	R2	K45
      0x8C080532,  //  000E  GETMET	R2	R2	K50
      0x88100137,  //  000F  GETMBR	R4	R0	K55
      0x7C080400,  //  0010  CALL	R2	2
      0x90029A02,  //  0011  SETMBR	R0	K77	R2
      0xB80A5A00,  //  0012  GETNGBL	R2	K45
      0x8C080556,  //  0013  GETMET	R2	R2	K86
      0x5C100000,  //  0014  MOVE	R4	R0
      0x7C080400,  //  0015  CALL	R2	2
      0x90020802,  //  0016  SETMBR	R0	K4	R2
      0x50080000,  //  0017  LDBOOL	R2	0	0
      0x90020002,  //  0018  SETMBR	R0	K0	R2
      0x90021118,  //  0019  SETMBR	R0	K8	K24
      0x90021918,  //  001A  SETMBR	R0	K12	K24
      0x4C080000,  //  001B  LDNIL	R2
      0x90020202,  //  001C  SETMBR	R0	K1	R2
      0x88080157,  //  001D  GETMBR	R2	R0	K87
      0x90021202,  //  001E  SETMBR	R0	K9	R2
      0x50080000,  //  001F  LDBOOL	R2	0	0
      0x90022C02,  //  0020  SETMBR	R0	K22	R2
      0x90022F18,  //  0021  SETMBR	R0	K23	K24
      0x90023318,  //  0022  SETMBR	R0	K25	K24
      0x90024145,  //  0023  SETMBR	R0	K32	K69
      0x90024318,  //  0024  SETMBR	R0	K33	K24
      0x90023518,  //  0025  SETMBR	R0	K26	K24
      0x90024D45,  //  0026  SETMBR	R0	K38	K69
      0x90024F18,  //  0027  SETMBR	R0	K39	K24
      0x90023718,  //  0028  SETMBR	R0	K27	K24
      0x90025145,  //  0029  SETMBR	R0	K40	K69
      0x90025318,  //  002A  SETMBR	R0	K41	K24
      0x90023918,  //  002B  SETMBR	R0	K28	K24
      0x90024545,  //  002C  SETMBR	R0	K34	K69
      0x90024718,  //  002D  SETMBR	R0	K35	K24
      0x90023B18,  //  002E  SETMBR	R0	K29	K24
      0x90024945,  //  002F  SETMBR	R0	K36	K69
      0x90024B18,  //  0030  SETMBR	R0	K37	K24
      0x90023D18,  //  0031  SETMBR	R0	K30	K24
      0x90028345,  //  0032  SETMBR	R0	K65	K69
      0x90028518,  //  0033  SETMBR	R0	K66	K24
      0x90028118,  //  0034  SETMBR	R0	K64	K24
      0x540A1387,  //  0035  LDINT	R2	5000
      0x90028602,  //  0036  SETMBR	R0	K67	R2
      0x4C080000,  //  0037  LDNIL	R2
      0x90021402,  //  0038  SETMBR	R0	K10	R2
      0x4C080000,  //  0039  LDNIL	R2
      0x90027E02,  //  003A  SETMBR	R0	K63	R2
      0x4C080000,  //  003B  LDNIL	R2
      0x90027802,  //  003C  SETMBR	R0	K60	R2
      0x4C080000,  //  003D  LDNIL	R2
      0x90027A02,  //  003E  SETMBR	R0	K61	R2
      0x4C080000,  //  003F  LDNIL	R2
      0x90027C02,  //  0040  SETMBR	R0	K62	R2
      0x4C080000,  //  0041  LDNIL	R2
      0x90022202,  //  0042  SETMBR	R0	K17	R2
      0x80000000,  //  0043  RET	0
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
      0x8804010D,  //  0000  GETMBR	R1	R0	K13
      0x8C040336,  //  0001  GETMET	R1	R1	K54
      0x7C040200,  //  0002  CALL	R1	1
      0x8804010D,  //  0003  GETMBR	R1	R0	K13
      0x8C040334,  //  0004  GETMET	R1	R1	K52
      0x7C040200,  //  0005  CALL	R1	1
      0x80000000,  //  0006  RET	0
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
      0x18080318,  //  0000  LE	R2	R1	K24
      0x780A0000,  //  0001  JMPF	R2	#0003
      0x80000400,  //  0002  RET	0
      0x90026E01,  //  0003  SETMBR	R0	K55	R1
      0x88080132,  //  0004  GETMBR	R2	R0	K50
      0x8C080558,  //  0005  GETMET	R2	R2	K88
      0x5C100200,  //  0006  MOVE	R4	R1
      0x7C080400,  //  0007  CALL	R2	2
      0x8808014D,  //  0008  GETMBR	R2	R0	K77
      0x8C080558,  //  0009  GETMET	R2	R2	K88
      0x5C100200,  //  000A  MOVE	R4	R1
      0x7C080400,  //  000B  CALL	R2	2
      0x50080200,  //  000C  LDBOOL	R2	1	0
      0x90022C02,  //  000D  SETMBR	R0	K22	R2
      0x80000000,  //  000E  RET	0
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
    ( &(const binstruction[ 4]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x8C040359,  //  0001  GETMET	R1	R1	K89
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: AnimationEngine
********************************************************************/
be_local_class(AnimationEngine,
    38,
    NULL,
    be_nested_map(71,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(stop, 9), be_const_closure(class_AnimationEngine_stop_closure) },
        { be_const_key_weak(frame_buffer, 65), be_const_var(3) },
        { be_const_key_weak(pop_iteration_context, -1), be_const_closure(class_AnimationEngine_pop_iteration_context_closure) },
        { be_const_key_weak(animations, -1), be_const_closure(class_AnimationEngine_animations_closure) },
        { be_const_key_weak(phase2_time_max, 50), be_const_var(26) },
        { be_const_key_weak(strip_length, 45), be_const_var(1) },
        { be_const_key_weak(on_tick, 37), be_const_closure(class_AnimationEngine_on_tick_closure) },
        { be_const_key_weak(ts_1, -1), be_const_var(33) },
        { be_const_key_weak(phase2_time_min, 49), be_const_var(25) },
        { be_const_key_weak(tick_ms, -1), be_const_var(9) },
        { be_const_key_weak(tick_time_sum, 34), be_const_var(12) },
        { be_const_key_weak(phase1_time_max, 62), be_const_var(23) },
        { be_const_key_weak(_print_stats, -1), be_const_closure(class_AnimationEngine__print_stats_closure) },
        { be_const_key_weak(last_update, 22), be_const_var(6) },
        { be_const_key_weak(interrupt_animation, -1), be_const_closure(class_AnimationEngine_interrupt_animation_closure) },
        { be_const_key_weak(is_running, -1), be_const_var(5) },
        { be_const_key_weak(push_iteration_context, 1), be_const_closure(class_AnimationEngine_push_iteration_context_closure) },
        { be_const_key_weak(anim_time_min, 33), be_const_var(16) },
        { be_const_key_weak(_output_to_strip, -1), be_const_closure(class_AnimationEngine__output_to_strip_closure) },
        { be_const_key_weak(hw_time_min, -1), be_const_var(19) },
        { be_const_key_weak(TICK_MS, -1), be_const_int(50) },
        { be_const_key_weak(strip, -1), be_const_var(0) },
        { be_const_key_weak(clear, -1), be_const_closure(class_AnimationEngine_clear_closure) },
        { be_const_key_weak(sequence_managers, 19), be_const_closure(class_AnimationEngine_sequence_managers_closure) },
        { be_const_key_weak(_clear_strip, 30), be_const_closure(class_AnimationEngine__clear_strip_closure) },
        { be_const_key_weak(remove, 20), be_const_closure(class_AnimationEngine_remove_closure) },
        { be_const_key_weak(phase3_time_max, -1), be_const_var(29) },
        { be_const_key_weak(phase1_time_min, -1), be_const_var(22) },
        { be_const_key_weak(fast_loop_closure, -1), be_const_var(8) },
        { be_const_key_weak(get_strip_length, -1), be_const_closure(class_AnimationEngine_get_strip_length_closure) },
        { be_const_key_weak(phase2_time_sum, -1), be_const_var(24) },
        { be_const_key_weak(ts_2, -1), be_const_var(34) },
        { be_const_key_weak(tick_time_min, -1), be_const_var(13) },
        { be_const_key_weak(anim_time_max, 69), be_const_var(17) },
        { be_const_key_weak(add, 26), be_const_closure(class_AnimationEngine_add_closure) },
        { be_const_key_weak(ts_hw, -1), be_const_var(36) },
        { be_const_key_weak(tick_time_max, -1), be_const_var(14) },
        { be_const_key_weak(phase1_time_sum, -1), be_const_var(21) },
        { be_const_key_weak(run, -1), be_const_closure(class_AnimationEngine_run_closure) },
        { be_const_key_weak(hw_time_max, -1), be_const_var(20) },
        { be_const_key_weak(_record_tick_metrics, 2), be_const_closure(class_AnimationEngine__record_tick_metrics_closure) },
        { be_const_key_weak(render_needed, -1), be_const_var(10) },
        { be_const_key_weak(update_current_iteration, 58), be_const_closure(class_AnimationEngine_update_current_iteration_closure) },
        { be_const_key_weak(root_animation, 57), be_const_var(2) },
        { be_const_key_weak(ts_end, -1), be_const_var(37) },
        { be_const_key_weak(size, -1), be_const_closure(class_AnimationEngine_size_closure) },
        { be_const_key_weak(_update_and_render, -1), be_const_closure(class_AnimationEngine__update_and_render_closure) },
        { be_const_key_weak(stats_period, 55), be_const_var(31) },
        { be_const_key_weak(cleanup, -1), be_const_closure(class_AnimationEngine_cleanup_closure) },
        { be_const_key_weak(temp_buffer, 54), be_const_var(4) },
        { be_const_key_weak(get_current_iteration_number, 38), be_const_closure(class_AnimationEngine_get_current_iteration_number_closure) },
        { be_const_key_weak(interrupt_current, -1), be_const_closure(class_AnimationEngine_interrupt_current_closure) },
        { be_const_key_weak(get_animations, 53), be_const_closure(class_AnimationEngine_get_animations_closure) },
        { be_const_key_weak(check_strip_length, -1), be_const_closure(class_AnimationEngine_check_strip_length_closure) },
        { be_const_key_weak(_process_events, -1), be_const_closure(class_AnimationEngine__process_events_closure) },
        { be_const_key_weak(get_strip, -1), be_const_closure(class_AnimationEngine_get_strip_closure) },
        { be_const_key_weak(resume_after, -1), be_const_closure(class_AnimationEngine_resume_after_closure) },
        { be_const_key_weak(tostring, -1), be_const_closure(class_AnimationEngine_tostring_closure) },
        { be_const_key_weak(is_active, -1), be_const_closure(class_AnimationEngine_is_active_closure) },
        { be_const_key_weak(tick_count, -1), be_const_var(11) },
        { be_const_key_weak(hw_time_sum, 24), be_const_var(18) },
        { be_const_key_weak(init, -1), be_const_closure(class_AnimationEngine_init_closure) },
        { be_const_key_weak(anim_time_sum, -1), be_const_var(15) },
        { be_const_key_weak(time_ms, -1), be_const_var(7) },
        { be_const_key_weak(_handle_strip_length_change, -1), be_const_closure(class_AnimationEngine__handle_strip_length_change_closure) },
        { be_const_key_weak(ts_start, 70), be_const_var(32) },
        { be_const_key_weak(resume, 15), be_const_closure(class_AnimationEngine_resume_closure) },
        { be_const_key_weak(ts_3, -1), be_const_var(35) },
        { be_const_key_weak(phase3_time_sum, 7), be_const_var(27) },
        { be_const_key_weak(last_stats_time, -1), be_const_var(30) },
        { be_const_key_weak(phase3_time_min, -1), be_const_var(28) },
    })),
    be_str_weak(AnimationEngine)
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
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(oscillator_value),
    /* K2   */  be_nested_str_weak(form),
    }),
    be_str_weak(elastic),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x540A0007,  //  0004  LDINT	R2	8
      0x90060402,  //  0005  SETMBR	R1	K2	R2
      0x80040200,  //  0006  RET	1	R1
    })
  )
);
/*******************************************************************/

extern const bclass be_class_parameterized_object;
// compact class 'parameterized_object' ktab size: 60, total: 124 (saved 512 bytes)
static const bvalue be_ktab_class_parameterized_object[60] = {
  /* K0   */  be_nested_str_weak(engine),
  /* K1   */  be_nested_str_weak(time_ms),
  /* K2   */  be_nested_str_weak(start_time),
  /* K3   */  be_nested_str_weak(undefined),
  /* K4   */  be_nested_str_weak(values),
  /* K5   */  be_nested_str_weak(contains),
  /* K6   */  be_nested_str_weak(_get_param_def),
  /* K7   */  be_nested_str_weak(constraint_mask),
  /* K8   */  be_nested_str_weak(default),
  /* K9   */  be_nested_str_weak(constraint_find),
  /* K10  */  be_nested_str_weak(resolve_value),
  /* K11  */  be_const_class(be_class_parameterized_object),
  /* K12  */  be_const_int(1),
  /* K13  */  be_const_int(0),
  /* K14  */  be_nested_str_weak(_MASK),
  /* K15  */  be_nested_str_weak(find),
  /* K16  */  be_const_int(2),
  /* K17  */  be_nested_str_weak(_TYPES),
  /* K18  */  be_nested_str_weak(push),
  /* K19  */  be_nested_str_weak(_X3Cinstance_X3A_X20_X25s_X3E),
  /* K20  */  be_nested_str_weak(instance),
  /* K21  */  be_nested_str_weak(_X27_X25s_X27_X20object_X20has_X20no_X20attribute_X20_X27_X25s_X27),
  /* K22  */  be_nested_str_weak(attribute_error),
  /* K23  */  be_nested_str_weak(member),
  /* K24  */  be_nested_str_weak(animation),
  /* K25  */  be_nested_str_weak(is_value_provider),
  /* K26  */  be_nested_str_weak(_validate_param),
  /* K27  */  be_nested_str_weak(on_param_changed),
  /* K28  */  be_nested_str_weak(produce_value),
  /* K29  */  be_nested_str_weak(nillable),
  /* K30  */  be_nested_str_weak(introspect),
  /* K31  */  be_nested_str_weak(toptr),
  /* K32  */  be_nested_str_weak(PARAMS),
  /* K33  */  be_nested_str_weak(is_running),
  /* K34  */  be_nested_str_weak(value_error),
  /* K35  */  be_nested_str_weak(missing_X20engine_X20parameter),
  /* K36  */  be_nested_str_weak(_init_parameter_values),
  /* K37  */  be_nested_str_weak(has_param),
  /* K38  */  be_nested_str_weak(_set_parameter_value),
  /* K39  */  be_nested_str_weak(keys),
  /* K40  */  be_nested_str_weak(stop_iteration),
  /* K41  */  be_nested_str_weak(_X27_X25s_X27_X20does_X20not_X20accept_X20nil_X20values),
  /* K42  */  be_nested_str_weak(type),
  /* K43  */  be_nested_str_weak(int),
  /* K44  */  be_nested_str_weak(time),
  /* K45  */  be_nested_str_weak(percentage),
  /* K46  */  be_nested_str_weak(color),
  /* K47  */  be_nested_str_weak(palette),
  /* K48  */  be_nested_str_weak(bytes),
  /* K49  */  be_nested_str_weak(any),
  /* K50  */  be_nested_str_weak(real),
  /* K51  */  be_nested_str_weak(math),
  /* K52  */  be_nested_str_weak(round),
  /* K53  */  be_nested_str_weak(_X27_X25s_X27_X20expects_X20type_X20_X27_X25s_X27_X20but_X20got_X20_X27_X25s_X27_X20_X28value_X3A_X20_X25s_X29),
  /* K54  */  be_nested_str_weak(min),
  /* K55  */  be_nested_str_weak(_X27_X25s_X27_X20value_X20_X25s_X20is_X20below_X20minimum_X20_X25s),
  /* K56  */  be_nested_str_weak(max),
  /* K57  */  be_nested_str_weak(_X27_X25s_X27_X20value_X20_X25s_X20is_X20above_X20maximum_X20_X25s),
  /* K58  */  be_nested_str_weak(enum),
  /* K59  */  be_nested_str_weak(_X27_X25s_X27_X20value_X20_X25s_X20is_X20not_X20in_X20allowed_X20values_X20_X25s),
};


extern const bclass be_class_parameterized_object;

/********************************************************************
** Solidified function: _fix_time_ms
********************************************************************/
be_local_closure(class_parameterized_object__fix_time_ms,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_parameterized_object,     /* shared constants */
    be_str_weak(_fix_time_ms),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x88080100,  //  0003  GETMBR	R2	R0	K0
      0x88040501,  //  0004  GETMBR	R1	R2	K1
      0x88080102,  //  0005  GETMBR	R2	R0	K2
      0x4C0C0000,  //  0006  LDNIL	R3
      0x1C080403,  //  0007  EQ	R2	R2	R3
      0x780A0000,  //  0008  JMPF	R2	#000A
      0x90020401,  //  0009  SETMBR	R0	K2	R1
      0x80040200,  //  000A  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: produce_value
********************************************************************/
be_local_closure(class_parameterized_object_produce_value,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_parameterized_object,     /* shared constants */
    be_str_weak(produce_value),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x600C000B,  //  0000  GETGBL	R3	G11
      0x58100003,  //  0001  LDCONST	R4	K3
      0x7C0C0200,  //  0002  CALL	R3	1
      0x80040600,  //  0003  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _resolve_parameter_value
********************************************************************/
be_local_closure(class_parameterized_object__resolve_parameter_value,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_parameterized_object,     /* shared constants */
    be_str_weak(_resolve_parameter_value),
    &be_const_str_solidified,
    ( &(const binstruction[31]) {  /* code */
      0x880C0104,  //  0000  GETMBR	R3	R0	K4
      0x8C0C0705,  //  0001  GETMET	R3	R3	K5
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x740E0011,  //  0004  JMPT	R3	#0017
      0x8C0C0106,  //  0005  GETMET	R3	R0	K6
      0x5C140200,  //  0006  MOVE	R5	R1
      0x7C0C0400,  //  0007  CALL	R3	2
      0x4C100000,  //  0008  LDNIL	R4
      0x20100604,  //  0009  NE	R4	R3	R4
      0x78120009,  //  000A  JMPF	R4	#0015
      0x8C100107,  //  000B  GETMET	R4	R0	K7
      0x5C180600,  //  000C  MOVE	R6	R3
      0x581C0008,  //  000D  LDCONST	R7	K8
      0x7C100600,  //  000E  CALL	R4	3
      0x78120004,  //  000F  JMPF	R4	#0015
      0x8C100109,  //  0010  GETMET	R4	R0	K9
      0x5C180600,  //  0011  MOVE	R6	R3
      0x581C0008,  //  0012  LDCONST	R7	K8
      0x7C100600,  //  0013  CALL	R4	3
      0x80040800,  //  0014  RET	1	R4
      0x4C100000,  //  0015  LDNIL	R4
      0x80040800,  //  0016  RET	1	R4
      0x880C0104,  //  0017  GETMBR	R3	R0	K4
      0x940C0601,  //  0018  GETIDX	R3	R3	R1
      0x8C10010A,  //  0019  GETMET	R4	R0	K10
      0x5C180600,  //  001A  MOVE	R6	R3
      0x5C1C0200,  //  001B  MOVE	R7	R1
      0x5C200400,  //  001C  MOVE	R8	R2
      0x7C100800,  //  001D  CALL	R4	4
      0x80040800,  //  001E  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: !=
********************************************************************/
be_local_closure(class_parameterized_object__X21_X3D,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_parameterized_object,     /* shared constants */
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
** Solidified function: constraint_find
********************************************************************/
be_local_closure(class_parameterized_object_constraint_find,   /* name */
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
    &be_ktab_class_parameterized_object,     /* shared constants */
    be_str_weak(constraint_find),
    &be_const_str_solidified,
    ( &(const binstruction[112]) {  /* code */
      0x580C000B,  //  0000  LDCONST	R3	K11
      0x84100000,  //  0001  CLOSURE	R4	P0
      0x84140001,  //  0002  CLOSURE	R5	P1
      0x6018000C,  //  0003  GETGBL	R6	G12
      0x5C1C0000,  //  0004  MOVE	R7	R0
      0x7C180200,  //  0005  CALL	R6	1
      0x14180D0C,  //  0006  LT	R6	R6	K12
      0x781A0000,  //  0007  JMPF	R6	#0009
      0x80040400,  //  0008  RET	1	R2
      0x9418010D,  //  0009  GETIDX	R6	R0	K13
      0x581C000C,  //  000A  LDCONST	R7	K12
      0x8820070E,  //  000B  GETMBR	R8	R3	K14
      0x8C20110F,  //  000C  GETMET	R8	R8	K15
      0x5C280200,  //  000D  MOVE	R10	R1
      0x7C200400,  //  000E  CALL	R8	2
      0x4C240000,  //  000F  LDNIL	R9
      0x1C241009,  //  0010  EQ	R9	R8	R9
      0x78260000,  //  0011  JMPF	R9	#0013
      0x80040400,  //  0012  RET	1	R2
      0x38221808,  //  0013  SHL	R8	K12	R8
      0x2C240C08,  //  0014  AND	R9	R6	R8
      0x74260000,  //  0015  JMPT	R9	#0017
      0x80040400,  //  0016  RET	1	R2
      0x5426001F,  //  0017  LDINT	R9	32
      0x1C241009,  //  0018  EQ	R9	R8	R9
      0x78260001,  //  0019  JMPF	R9	#001C
      0x50240200,  //  001A  LDBOOL	R9	1	0
      0x80041200,  //  001B  RET	1	R9
      0x2424110C,  //  001C  GT	R9	R8	K12
      0x78260006,  //  001D  JMPF	R9	#0025
      0x2C240D0C,  //  001E  AND	R9	R6	K12
      0x78260004,  //  001F  JMPF	R9	#0025
      0x5C240800,  //  0020  MOVE	R9	R4
      0x5C280000,  //  0021  MOVE	R10	R0
      0x5C2C0E00,  //  0022  MOVE	R11	R7
      0x7C240400,  //  0023  CALL	R9	2
      0x001C0E09,  //  0024  ADD	R7	R7	R9
      0x24241110,  //  0025  GT	R9	R8	K16
      0x78260006,  //  0026  JMPF	R9	#002E
      0x2C240D10,  //  0027  AND	R9	R6	K16
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
      0x001C0F0C,  //  003F  ADD	R7	R7	K12
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
      0x882C0711,  //  004B  GETMBR	R11	R3	K17
      0x7C280200,  //  004C  CALL	R10	1
      0x1428120A,  //  004D  LT	R10	R9	R10
      0x782A0002,  //  004E  JMPF	R10	#0052
      0x88280711,  //  004F  GETMBR	R10	R3	K17
      0x94281409,  //  0050  GETIDX	R10	R10	R9
      0x80041400,  //  0051  RET	1	R10
      0x80040400,  //  0052  RET	1	R2
      0x5426000F,  //  0053  LDINT	R9	16
      0x1C241009,  //  0054  EQ	R9	R8	R9
      0x78260014,  //  0055  JMPF	R9	#006B
      0x94240007,  //  0056  GETIDX	R9	R0	R7
      0x001C0F0C,  //  0057  ADD	R7	R7	K12
      0x60280012,  //  0058  GETGBL	R10	G18
      0x7C280000,  //  0059  CALL	R10	0
      0x582C000D,  //  005A  LDCONST	R11	K13
      0x14301609,  //  005B  LT	R12	R11	R9
      0x7832000C,  //  005C  JMPF	R12	#006A
      0x8C301512,  //  005D  GETMET	R12	R10	K18
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
      0x002C170C,  //  0068  ADD	R11	R11	K12
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
** Solidified function: tostring
********************************************************************/
be_local_closure(class_parameterized_object_tostring,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_parameterized_object,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x60040018,  //  0000  GETGBL	R1	G24
      0x58080013,  //  0001  LDCONST	R2	K19
      0x600C0005,  //  0002  GETGBL	R3	G5
      0x5C100000,  //  0003  MOVE	R4	R0
      0x7C0C0200,  //  0004  CALL	R3	1
      0x7C040400,  //  0005  CALL	R1	2
      0x80040200,  //  0006  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: on_param_changed
********************************************************************/
be_local_closure(class_parameterized_object_on_param_changed,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_parameterized_object,     /* shared constants */
    be_str_weak(on_param_changed),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_param
********************************************************************/
be_local_closure(class_parameterized_object_get_param,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_parameterized_object,     /* shared constants */
    be_str_weak(get_param),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x880C0104,  //  0000  GETMBR	R3	R0	K4
      0x8C0C0705,  //  0001  GETMET	R3	R3	K5
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x780E0002,  //  0004  JMPF	R3	#0008
      0x880C0104,  //  0005  GETMBR	R3	R0	K4
      0x940C0601,  //  0006  GETIDX	R3	R3	R1
      0x80040600,  //  0007  RET	1	R3
      0x8C0C0106,  //  0008  GETMET	R3	R0	K6
      0x5C140200,  //  0009  MOVE	R5	R1
      0x7C0C0400,  //  000A  CALL	R3	2
      0x4C100000,  //  000B  LDNIL	R4
      0x20100604,  //  000C  NE	R4	R3	R4
      0x7812000A,  //  000D  JMPF	R4	#0019
      0x8C100107,  //  000E  GETMET	R4	R0	K7
      0x5C180600,  //  000F  MOVE	R6	R3
      0x581C0008,  //  0010  LDCONST	R7	K8
      0x7C100600,  //  0011  CALL	R4	3
      0x78120005,  //  0012  JMPF	R4	#0019
      0x8C100109,  //  0013  GETMET	R4	R0	K9
      0x5C180600,  //  0014  MOVE	R6	R3
      0x581C0008,  //  0015  LDCONST	R7	K8
      0x5C200400,  //  0016  MOVE	R8	R2
      0x7C100800,  //  0017  CALL	R4	4
      0x80040800,  //  0018  RET	1	R4
      0x80040400,  //  0019  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: member
********************************************************************/
be_local_closure(class_parameterized_object_member,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_parameterized_object,     /* shared constants */
    be_str_weak(member),
    &be_const_str_solidified,
    ( &(const binstruction[58]) {  /* code */
      0x88080104,  //  0000  GETMBR	R2	R0	K4
      0x8C08050F,  //  0001  GETMET	R2	R2	K15
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x4C0C0000,  //  0004  LDNIL	R3
      0x200C0403,  //  0005  NE	R3	R2	R3
      0x780E000D,  //  0006  JMPF	R3	#0015
      0x600C0004,  //  0007  GETGBL	R3	G4
      0x5C100400,  //  0008  MOVE	R4	R2
      0x7C0C0200,  //  0009  CALL	R3	1
      0x200C0714,  //  000A  NE	R3	R3	K20
      0x780E0000,  //  000B  JMPF	R3	#000D
      0x80040400,  //  000C  RET	1	R2
      0x8C0C010A,  //  000D  GETMET	R3	R0	K10
      0x5C140400,  //  000E  MOVE	R5	R2
      0x5C180200,  //  000F  MOVE	R6	R1
      0x881C0100,  //  0010  GETMBR	R7	R0	K0
      0x881C0F01,  //  0011  GETMBR	R7	R7	K1
      0x7C0C0800,  //  0012  CALL	R3	4
      0x80040600,  //  0013  RET	1	R3
      0x70020023,  //  0014  JMP		#0039
      0x880C0104,  //  0015  GETMBR	R3	R0	K4
      0x8C0C0705,  //  0016  GETMET	R3	R3	K5
      0x5C140200,  //  0017  MOVE	R5	R1
      0x7C0C0400,  //  0018  CALL	R3	2
      0x780E0002,  //  0019  JMPF	R3	#001D
      0x4C0C0000,  //  001A  LDNIL	R3
      0x80040600,  //  001B  RET	1	R3
      0x7002001B,  //  001C  JMP		#0039
      0x8C0C0106,  //  001D  GETMET	R3	R0	K6
      0x5C140200,  //  001E  MOVE	R5	R1
      0x7C0C0400,  //  001F  CALL	R3	2
      0x4C100000,  //  0020  LDNIL	R4
      0x20100604,  //  0021  NE	R4	R3	R4
      0x7812000D,  //  0022  JMPF	R4	#0031
      0x8C100107,  //  0023  GETMET	R4	R0	K7
      0x5C180600,  //  0024  MOVE	R6	R3
      0x581C0008,  //  0025  LDCONST	R7	K8
      0x7C100600,  //  0026  CALL	R4	3
      0x78120005,  //  0027  JMPF	R4	#002E
      0x8C100109,  //  0028  GETMET	R4	R0	K9
      0x5C180600,  //  0029  MOVE	R6	R3
      0x581C0008,  //  002A  LDCONST	R7	K8
      0x7C100600,  //  002B  CALL	R4	3
      0x80040800,  //  002C  RET	1	R4
      0x70020001,  //  002D  JMP		#0030
      0x4C100000,  //  002E  LDNIL	R4
      0x80040800,  //  002F  RET	1	R4
      0x70020007,  //  0030  JMP		#0039
      0x60100018,  //  0031  GETGBL	R4	G24
      0x58140015,  //  0032  LDCONST	R5	K21
      0x60180005,  //  0033  GETGBL	R6	G5
      0x5C1C0000,  //  0034  MOVE	R7	R0
      0x7C180200,  //  0035  CALL	R6	1
      0x5C1C0200,  //  0036  MOVE	R7	R1
      0x7C100600,  //  0037  CALL	R4	3
      0xB0062C04,  //  0038  RAISE	1	K22	R4
      0x80000000,  //  0039  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_param_value
********************************************************************/
be_local_closure(class_parameterized_object_get_param_value,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_parameterized_object,     /* shared constants */
    be_str_weak(get_param_value),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C080117,  //  0000  GETMET	R2	R0	K23
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x80040400,  //  0003  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _set_parameter_value
********************************************************************/
be_local_closure(class_parameterized_object__set_parameter_value,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_parameterized_object,     /* shared constants */
    be_str_weak(_set_parameter_value),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0xB80E3000,  //  0000  GETNGBL	R3	K24
      0x8C0C0719,  //  0001  GETMET	R3	R3	K25
      0x5C140400,  //  0002  MOVE	R5	R2
      0x7C0C0400,  //  0003  CALL	R3	2
      0x740E0004,  //  0004  JMPT	R3	#000A
      0x8C0C011A,  //  0005  GETMET	R3	R0	K26
      0x5C140200,  //  0006  MOVE	R5	R1
      0x5C180400,  //  0007  MOVE	R6	R2
      0x7C0C0600,  //  0008  CALL	R3	3
      0x5C080600,  //  0009  MOVE	R2	R3
      0x880C0104,  //  000A  GETMBR	R3	R0	K4
      0x980C0202,  //  000B  SETIDX	R3	R1	R2
      0x8C0C011B,  //  000C  GETMET	R3	R0	K27
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
be_local_closure(class_parameterized_object_resolve_value,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_parameterized_object,     /* shared constants */
    be_str_weak(resolve_value),
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
      0xB8123000,  //  0000  GETNGBL	R4	K24
      0x8C100919,  //  0001  GETMET	R4	R4	K25
      0x5C180200,  //  0002  MOVE	R6	R1
      0x7C100400,  //  0003  CALL	R4	2
      0x7812001A,  //  0004  JMPF	R4	#0020
      0x8C10031C,  //  0005  GETMET	R4	R1	K28
      0x5C180400,  //  0006  MOVE	R6	R2
      0x5C1C0600,  //  0007  MOVE	R7	R3
      0x7C100600,  //  0008  CALL	R4	3
      0x4C140000,  //  0009  LDNIL	R5
      0x1C140805,  //  000A  EQ	R5	R4	R5
      0x78160011,  //  000B  JMPF	R5	#001E
      0x8C140106,  //  000C  GETMET	R5	R0	K6
      0x5C1C0400,  //  000D  MOVE	R7	R2
      0x7C140400,  //  000E  CALL	R5	2
      0x8C180107,  //  000F  GETMET	R6	R0	K7
      0x5C200A00,  //  0010  MOVE	R8	R5
      0x5824001D,  //  0011  LDCONST	R9	K29
      0x7C180600,  //  0012  CALL	R6	3
      0x741A0009,  //  0013  JMPT	R6	#001E
      0x8C180107,  //  0014  GETMET	R6	R0	K7
      0x5C200A00,  //  0015  MOVE	R8	R5
      0x58240008,  //  0016  LDCONST	R9	K8
      0x7C180600,  //  0017  CALL	R6	3
      0x781A0004,  //  0018  JMPF	R6	#001E
      0x8C180109,  //  0019  GETMET	R6	R0	K9
      0x5C200A00,  //  001A  MOVE	R8	R5
      0x58240008,  //  001B  LDCONST	R9	K8
      0x7C180600,  //  001C  CALL	R6	3
      0x5C100C00,  //  001D  MOVE	R4	R6
      0x80040800,  //  001E  RET	1	R4
      0x70020000,  //  001F  JMP		#0021
      0x80040200,  //  0020  RET	1	R1
      0x80000000,  //  0021  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: ==
********************************************************************/
be_local_closure(class_parameterized_object__X3D_X3D,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_parameterized_object,     /* shared constants */
    be_str_weak(_X3D_X3D),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0xA40A3C00,  //  0000  IMPORT	R2	K30
      0x8C0C051F,  //  0001  GETMET	R3	R2	K31
      0x5C140000,  //  0002  MOVE	R5	R0
      0x7C0C0400,  //  0003  CALL	R3	2
      0x8C10051F,  //  0004  GETMET	R4	R2	K31
      0x5C180200,  //  0005  MOVE	R6	R1
      0x7C100400,  //  0006  CALL	R4	2
      0x1C0C0604,  //  0007  EQ	R3	R3	R4
      0x80040600,  //  0008  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _get_param_def
********************************************************************/
be_local_closure(class_parameterized_object__get_param_def,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_parameterized_object,     /* shared constants */
    be_str_weak(_get_param_def),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0xA40A3C00,  //  0000  IMPORT	R2	K30
      0x600C0006,  //  0001  GETGBL	R3	G6
      0x5C100000,  //  0002  MOVE	R4	R0
      0x7C0C0200,  //  0003  CALL	R3	1
      0x4C100000,  //  0004  LDNIL	R4
      0x20100604,  //  0005  NE	R4	R3	R4
      0x78120010,  //  0006  JMPF	R4	#0018
      0x8C100505,  //  0007  GETMET	R4	R2	K5
      0x5C180600,  //  0008  MOVE	R6	R3
      0x581C0020,  //  0009  LDCONST	R7	K32
      0x7C100600,  //  000A  CALL	R4	3
      0x78120006,  //  000B  JMPF	R4	#0013
      0x88100720,  //  000C  GETMBR	R4	R3	K32
      0x8C140905,  //  000D  GETMET	R5	R4	K5
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
** Solidified function: stop
********************************************************************/
be_local_closure(class_parameterized_object_stop,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_parameterized_object,     /* shared constants */
    be_str_weak(stop),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x50040000,  //  0000  LDBOOL	R1	0	0
      0x90024201,  //  0001  SETMBR	R0	K33	R1
      0x80040000,  //  0002  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_parameterized_object_init,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_parameterized_object,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x740A0004,  //  0002  JMPT	R2	#0008
      0x60080004,  //  0003  GETGBL	R2	G4
      0x5C0C0200,  //  0004  MOVE	R3	R1
      0x7C080200,  //  0005  CALL	R2	1
      0x20080514,  //  0006  NE	R2	R2	K20
      0x780A0000,  //  0007  JMPF	R2	#0009
      0xB0064523,  //  0008  RAISE	1	K34	K35
      0x90020001,  //  0009  SETMBR	R0	K0	R1
      0x60080013,  //  000A  GETGBL	R2	G19
      0x7C080000,  //  000B  CALL	R2	0
      0x90020802,  //  000C  SETMBR	R0	K4	R2
      0x50080000,  //  000D  LDBOOL	R2	0	0
      0x90024202,  //  000E  SETMBR	R0	K33	R2
      0x8C080124,  //  000F  GETMET	R2	R0	K36
      0x7C080200,  //  0010  CALL	R2	1
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update
********************************************************************/
be_local_closure(class_parameterized_object_update,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_parameterized_object,     /* shared constants */
    be_str_weak(update),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_param
********************************************************************/
be_local_closure(class_parameterized_object_set_param,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_parameterized_object,     /* shared constants */
    be_str_weak(set_param),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x8C0C0125,  //  0000  GETMET	R3	R0	K37
      0x5C140200,  //  0001  MOVE	R5	R1
      0x7C0C0400,  //  0002  CALL	R3	2
      0x740E0001,  //  0003  JMPT	R3	#0006
      0x500C0000,  //  0004  LDBOOL	R3	0	0
      0x80040600,  //  0005  RET	1	R3
      0xA8020008,  //  0006  EXBLK	0	#0010
      0x8C0C0126,  //  0007  GETMET	R3	R0	K38
      0x5C140200,  //  0008  MOVE	R5	R1
      0x5C180400,  //  0009  MOVE	R6	R2
      0x7C0C0600,  //  000A  CALL	R3	3
      0x500C0200,  //  000B  LDBOOL	R3	1	0
      0xA8040001,  //  000C  EXBLK	1	1
      0x80040600,  //  000D  RET	1	R3
      0xA8040001,  //  000E  EXBLK	1	1
      0x70020006,  //  000F  JMP		#0017
      0x580C0022,  //  0010  LDCONST	R3	K34
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
** Solidified function: has_param
********************************************************************/
be_local_closure(class_parameterized_object_has_param,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_parameterized_object,     /* shared constants */
    be_str_weak(has_param),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C080106,  //  0000  GETMET	R2	R0	K6
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x20080403,  //  0004  NE	R2	R2	R3
      0x80040400,  //  0005  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _init_parameter_values
********************************************************************/
be_local_closure(class_parameterized_object__init_parameter_values,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_parameterized_object,     /* shared constants */
    be_str_weak(_init_parameter_values),
    &be_const_str_solidified,
    ( &(const binstruction[47]) {  /* code */
      0xA4063C00,  //  0000  IMPORT	R1	K30
      0x60080006,  //  0001  GETGBL	R2	G6
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C080200,  //  0003  CALL	R2	1
      0x4C0C0000,  //  0004  LDNIL	R3
      0x200C0403,  //  0005  NE	R3	R2	R3
      0x780E0026,  //  0006  JMPF	R3	#002E
      0x8C0C0305,  //  0007  GETMET	R3	R1	K5
      0x5C140400,  //  0008  MOVE	R5	R2
      0x58180020,  //  0009  LDCONST	R6	K32
      0x7C0C0600,  //  000A  CALL	R3	3
      0x780E001C,  //  000B  JMPF	R3	#0029
      0x880C0520,  //  000C  GETMBR	R3	R2	K32
      0x60100010,  //  000D  GETGBL	R4	G16
      0x8C140727,  //  000E  GETMET	R5	R3	K39
      0x7C140200,  //  000F  CALL	R5	1
      0x7C100200,  //  0010  CALL	R4	1
      0xA8020013,  //  0011  EXBLK	0	#0026
      0x5C140800,  //  0012  MOVE	R5	R4
      0x7C140000,  //  0013  CALL	R5	0
      0x88180104,  //  0014  GETMBR	R6	R0	K4
      0x8C180D05,  //  0015  GETMET	R6	R6	K5
      0x5C200A00,  //  0016  MOVE	R8	R5
      0x7C180400,  //  0017  CALL	R6	2
      0x741A000B,  //  0018  JMPT	R6	#0025
      0x94180605,  //  0019  GETIDX	R6	R3	R5
      0x8C1C0107,  //  001A  GETMET	R7	R0	K7
      0x5C240C00,  //  001B  MOVE	R9	R6
      0x58280008,  //  001C  LDCONST	R10	K8
      0x7C1C0600,  //  001D  CALL	R7	3
      0x781E0005,  //  001E  JMPF	R7	#0025
      0x881C0104,  //  001F  GETMBR	R7	R0	K4
      0x8C200109,  //  0020  GETMET	R8	R0	K9
      0x5C280C00,  //  0021  MOVE	R10	R6
      0x582C0008,  //  0022  LDCONST	R11	K8
      0x7C200600,  //  0023  CALL	R8	3
      0x981C0A08,  //  0024  SETIDX	R7	R5	R8
      0x7001FFEB,  //  0025  JMP		#0012
      0x58100028,  //  0026  LDCONST	R4	K40
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
** Solidified function: setmember
********************************************************************/
be_local_closure(class_parameterized_object_setmember,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_parameterized_object,     /* shared constants */
    be_str_weak(setmember),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C0C0125,  //  0000  GETMET	R3	R0	K37
      0x5C140200,  //  0001  MOVE	R5	R1
      0x7C0C0400,  //  0002  CALL	R3	2
      0x780E0004,  //  0003  JMPF	R3	#0009
      0x8C0C0126,  //  0004  GETMET	R3	R0	K38
      0x5C140200,  //  0005  MOVE	R5	R1
      0x5C180400,  //  0006  MOVE	R6	R2
      0x7C0C0600,  //  0007  CALL	R3	3
      0x70020007,  //  0008  JMP		#0011
      0x600C0018,  //  0009  GETGBL	R3	G24
      0x58100015,  //  000A  LDCONST	R4	K21
      0x60140005,  //  000B  GETGBL	R5	G5
      0x5C180000,  //  000C  MOVE	R6	R0
      0x7C140200,  //  000D  CALL	R5	1
      0x5C180200,  //  000E  MOVE	R6	R1
      0x7C0C0600,  //  000F  CALL	R3	3
      0xB0062C03,  //  0010  RAISE	1	K22	R3
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _validate_param
********************************************************************/
be_local_closure(class_parameterized_object__validate_param,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_parameterized_object,     /* shared constants */
    be_str_weak(_validate_param),
    &be_const_str_solidified,
    ( &(const binstruction[186]) {  /* code */
      0x8C0C0106,  //  0000  GETMET	R3	R0	K6
      0x5C140200,  //  0001  MOVE	R5	R1
      0x7C0C0400,  //  0002  CALL	R3	2
      0x4C100000,  //  0003  LDNIL	R4
      0x1C100604,  //  0004  EQ	R4	R3	R4
      0x78120007,  //  0005  JMPF	R4	#000E
      0x60100018,  //  0006  GETGBL	R4	G24
      0x58140015,  //  0007  LDCONST	R5	K21
      0x60180005,  //  0008  GETGBL	R6	G5
      0x5C1C0000,  //  0009  MOVE	R7	R0
      0x7C180200,  //  000A  CALL	R6	1
      0x5C1C0200,  //  000B  MOVE	R7	R1
      0x7C100600,  //  000C  CALL	R4	3
      0xB0062C04,  //  000D  RAISE	1	K22	R4
      0xB8123000,  //  000E  GETNGBL	R4	K24
      0x8C100919,  //  000F  GETMET	R4	R4	K25
      0x5C180400,  //  0010  MOVE	R6	R2
      0x7C100400,  //  0011  CALL	R4	2
      0x78120000,  //  0012  JMPF	R4	#0014
      0x80040400,  //  0013  RET	1	R2
      0x4C100000,  //  0014  LDNIL	R4
      0x1C100404,  //  0015  EQ	R4	R2	R4
      0x78120014,  //  0016  JMPF	R4	#002C
      0x8C100107,  //  0017  GETMET	R4	R0	K7
      0x5C180600,  //  0018  MOVE	R6	R3
      0x581C001D,  //  0019  LDCONST	R7	K29
      0x7C100600,  //  001A  CALL	R4	3
      0x78120000,  //  001B  JMPF	R4	#001D
      0x80040400,  //  001C  RET	1	R2
      0x8C100107,  //  001D  GETMET	R4	R0	K7
      0x5C180600,  //  001E  MOVE	R6	R3
      0x581C0008,  //  001F  LDCONST	R7	K8
      0x7C100600,  //  0020  CALL	R4	3
      0x78120004,  //  0021  JMPF	R4	#0027
      0x8C100109,  //  0022  GETMET	R4	R0	K9
      0x5C180600,  //  0023  MOVE	R6	R3
      0x581C0008,  //  0024  LDCONST	R7	K8
      0x7C100600,  //  0025  CALL	R4	3
      0x80040800,  //  0026  RET	1	R4
      0x60100018,  //  0027  GETGBL	R4	G24
      0x58140029,  //  0028  LDCONST	R5	K41
      0x5C180200,  //  0029  MOVE	R6	R1
      0x7C100400,  //  002A  CALL	R4	2
      0xB0064404,  //  002B  RAISE	1	K34	R4
      0x8C100109,  //  002C  GETMET	R4	R0	K9
      0x5C180600,  //  002D  MOVE	R6	R3
      0x581C002A,  //  002E  LDCONST	R7	K42
      0x5820002B,  //  002F  LDCONST	R8	K43
      0x7C100800,  //  0030  CALL	R4	4
      0x1C14092C,  //  0031  EQ	R5	R4	K44
      0x74160003,  //  0032  JMPT	R5	#0037
      0x1C14092D,  //  0033  EQ	R5	R4	K45
      0x74160001,  //  0034  JMPT	R5	#0037
      0x1C14092E,  //  0035  EQ	R5	R4	K46
      0x78160001,  //  0036  JMPF	R5	#0039
      0x5810002B,  //  0037  LDCONST	R4	K43
      0x70020002,  //  0038  JMP		#003C
      0x1C14092F,  //  0039  EQ	R5	R4	K47
      0x78160000,  //  003A  JMPF	R5	#003C
      0x58100030,  //  003B  LDCONST	R4	K48
      0x60140004,  //  003C  GETGBL	R5	G4
      0x5C180400,  //  003D  MOVE	R6	R2
      0x7C140200,  //  003E  CALL	R5	1
      0x20180931,  //  003F  NE	R6	R4	K49
      0x781A0031,  //  0040  JMPF	R6	#0073
      0x1C18092B,  //  0041  EQ	R6	R4	K43
      0x781A000A,  //  0042  JMPF	R6	#004E
      0x1C180B32,  //  0043  EQ	R6	R5	K50
      0x781A0008,  //  0044  JMPF	R6	#004E
      0xA41A6600,  //  0045  IMPORT	R6	K51
      0x601C0009,  //  0046  GETGBL	R7	G9
      0x8C200D34,  //  0047  GETMET	R8	R6	K52
      0x5C280400,  //  0048  MOVE	R10	R2
      0x7C200400,  //  0049  CALL	R8	2
      0x7C1C0200,  //  004A  CALL	R7	1
      0x5C080E00,  //  004B  MOVE	R2	R7
      0x5814002B,  //  004C  LDCONST	R5	K43
      0x70020024,  //  004D  JMP		#0073
      0x1C180930,  //  004E  EQ	R6	R4	K48
      0x781A0018,  //  004F  JMPF	R6	#0069
      0x1C180B14,  //  0050  EQ	R6	R5	K20
      0x781A0006,  //  0051  JMPF	R6	#0059
      0x6018000F,  //  0052  GETGBL	R6	G15
      0x5C1C0400,  //  0053  MOVE	R7	R2
      0x60200015,  //  0054  GETGBL	R8	G21
      0x7C180400,  //  0055  CALL	R6	2
      0x781A0001,  //  0056  JMPF	R6	#0059
      0x58140030,  //  0057  LDCONST	R5	K48
      0x7002000E,  //  0058  JMP		#0068
      0x20180B14,  //  0059  NE	R6	R5	K20
      0x741A0004,  //  005A  JMPT	R6	#0060
      0x6018000F,  //  005B  GETGBL	R6	G15
      0x5C1C0400,  //  005C  MOVE	R7	R2
      0x60200015,  //  005D  GETGBL	R8	G21
      0x7C180400,  //  005E  CALL	R6	2
      0x741A0007,  //  005F  JMPT	R6	#0068
      0x60180018,  //  0060  GETGBL	R6	G24
      0x581C0035,  //  0061  LDCONST	R7	K53
      0x5C200200,  //  0062  MOVE	R8	R1
      0x5C240800,  //  0063  MOVE	R9	R4
      0x5C280A00,  //  0064  MOVE	R10	R5
      0x5C2C0400,  //  0065  MOVE	R11	R2
      0x7C180A00,  //  0066  CALL	R6	5
      0xB0064406,  //  0067  RAISE	1	K34	R6
      0x70020009,  //  0068  JMP		#0073
      0x20180805,  //  0069  NE	R6	R4	R5
      0x781A0007,  //  006A  JMPF	R6	#0073
      0x60180018,  //  006B  GETGBL	R6	G24
      0x581C0035,  //  006C  LDCONST	R7	K53
      0x5C200200,  //  006D  MOVE	R8	R1
      0x5C240800,  //  006E  MOVE	R9	R4
      0x5C280A00,  //  006F  MOVE	R10	R5
      0x5C2C0400,  //  0070  MOVE	R11	R2
      0x7C180A00,  //  0071  CALL	R6	5
      0xB0064406,  //  0072  RAISE	1	K34	R6
      0x1C180B2B,  //  0073  EQ	R6	R5	K43
      0x781A0023,  //  0074  JMPF	R6	#0099
      0x8C180107,  //  0075  GETMET	R6	R0	K7
      0x5C200600,  //  0076  MOVE	R8	R3
      0x58240036,  //  0077  LDCONST	R9	K54
      0x7C180600,  //  0078  CALL	R6	3
      0x781A000C,  //  0079  JMPF	R6	#0087
      0x8C180109,  //  007A  GETMET	R6	R0	K9
      0x5C200600,  //  007B  MOVE	R8	R3
      0x58240036,  //  007C  LDCONST	R9	K54
      0x7C180600,  //  007D  CALL	R6	3
      0x141C0406,  //  007E  LT	R7	R2	R6
      0x781E0006,  //  007F  JMPF	R7	#0087
      0x601C0018,  //  0080  GETGBL	R7	G24
      0x58200037,  //  0081  LDCONST	R8	K55
      0x5C240200,  //  0082  MOVE	R9	R1
      0x5C280400,  //  0083  MOVE	R10	R2
      0x5C2C0C00,  //  0084  MOVE	R11	R6
      0x7C1C0800,  //  0085  CALL	R7	4
      0xB0064407,  //  0086  RAISE	1	K34	R7
      0x8C180107,  //  0087  GETMET	R6	R0	K7
      0x5C200600,  //  0088  MOVE	R8	R3
      0x58240038,  //  0089  LDCONST	R9	K56
      0x7C180600,  //  008A  CALL	R6	3
      0x781A000C,  //  008B  JMPF	R6	#0099
      0x8C180109,  //  008C  GETMET	R6	R0	K9
      0x5C200600,  //  008D  MOVE	R8	R3
      0x58240038,  //  008E  LDCONST	R9	K56
      0x7C180600,  //  008F  CALL	R6	3
      0x241C0406,  //  0090  GT	R7	R2	R6
      0x781E0006,  //  0091  JMPF	R7	#0099
      0x601C0018,  //  0092  GETGBL	R7	G24
      0x58200039,  //  0093  LDCONST	R8	K57
      0x5C240200,  //  0094  MOVE	R9	R1
      0x5C280400,  //  0095  MOVE	R10	R2
      0x5C2C0C00,  //  0096  MOVE	R11	R6
      0x7C1C0800,  //  0097  CALL	R7	4
      0xB0064407,  //  0098  RAISE	1	K34	R7
      0x8C180107,  //  0099  GETMET	R6	R0	K7
      0x5C200600,  //  009A  MOVE	R8	R3
      0x5824003A,  //  009B  LDCONST	R9	K58
      0x7C180600,  //  009C  CALL	R6	3
      0x781A001A,  //  009D  JMPF	R6	#00B9
      0x50180000,  //  009E  LDBOOL	R6	0	0
      0x8C1C0109,  //  009F  GETMET	R7	R0	K9
      0x5C240600,  //  00A0  MOVE	R9	R3
      0x5828003A,  //  00A1  LDCONST	R10	K58
      0x7C1C0600,  //  00A2  CALL	R7	3
      0x6020000C,  //  00A3  GETGBL	R8	G12
      0x5C240E00,  //  00A4  MOVE	R9	R7
      0x7C200200,  //  00A5  CALL	R8	1
      0x5824000D,  //  00A6  LDCONST	R9	K13
      0x14281208,  //  00A7  LT	R10	R9	R8
      0x782A0006,  //  00A8  JMPF	R10	#00B0
      0x94280E09,  //  00A9  GETIDX	R10	R7	R9
      0x1C2C040A,  //  00AA  EQ	R11	R2	R10
      0x782E0001,  //  00AB  JMPF	R11	#00AE
      0x50180200,  //  00AC  LDBOOL	R6	1	0
      0x70020001,  //  00AD  JMP		#00B0
      0x0024130C,  //  00AE  ADD	R9	R9	K12
      0x7001FFF6,  //  00AF  JMP		#00A7
      0x5C280C00,  //  00B0  MOVE	R10	R6
      0x742A0006,  //  00B1  JMPT	R10	#00B9
      0x60280018,  //  00B2  GETGBL	R10	G24
      0x582C003B,  //  00B3  LDCONST	R11	K59
      0x5C300200,  //  00B4  MOVE	R12	R1
      0x5C340400,  //  00B5  MOVE	R13	R2
      0x5C380E00,  //  00B6  MOVE	R14	R7
      0x7C280800,  //  00B7  CALL	R10	4
      0xB006440A,  //  00B8  RAISE	1	K34	R10
      0x80040400,  //  00B9  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: constraint_mask
********************************************************************/
be_local_closure(class_parameterized_object_constraint_mask,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_parameterized_object,     /* shared constants */
    be_str_weak(constraint_mask),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x5808000B,  //  0000  LDCONST	R2	K11
      0x4C0C0000,  //  0001  LDNIL	R3
      0x200C0003,  //  0002  NE	R3	R0	R3
      0x780E000F,  //  0003  JMPF	R3	#0014
      0x600C000C,  //  0004  GETGBL	R3	G12
      0x5C100000,  //  0005  MOVE	R4	R0
      0x7C0C0200,  //  0006  CALL	R3	1
      0x240C070D,  //  0007  GT	R3	R3	K13
      0x780E000A,  //  0008  JMPF	R3	#0014
      0x880C050E,  //  0009  GETMBR	R3	R2	K14
      0x8C0C070F,  //  000A  GETMET	R3	R3	K15
      0x5C140200,  //  000B  MOVE	R5	R1
      0x7C0C0400,  //  000C  CALL	R3	2
      0x4C100000,  //  000D  LDNIL	R4
      0x20100604,  //  000E  NE	R4	R3	R4
      0x78120003,  //  000F  JMPF	R4	#0014
      0x9410010D,  //  0010  GETIDX	R4	R0	K13
      0x38161803,  //  0011  SHL	R5	K12	R3
      0x2C100805,  //  0012  AND	R4	R4	R5
      0x80040800,  //  0013  RET	1	R4
      0x80061A00,  //  0014  RET	1	K13
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tobool
********************************************************************/
be_local_closure(class_parameterized_object_tobool,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_parameterized_object,     /* shared constants */
    be_str_weak(tobool),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x50040200,  //  0000  LDBOOL	R1	1	0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(class_parameterized_object_start,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_parameterized_object,     /* shared constants */
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x88080100,  //  0003  GETMBR	R2	R0	K0
      0x88040501,  //  0004  GETMBR	R1	R2	K1
      0x50080200,  //  0005  LDBOOL	R2	1	0
      0x90024202,  //  0006  SETMBR	R0	K33	R2
      0x88080102,  //  0007  GETMBR	R2	R0	K2
      0x4C0C0000,  //  0008  LDNIL	R3
      0x20080403,  //  0009  NE	R2	R2	R3
      0x780A0000,  //  000A  JMPF	R2	#000C
      0x90020401,  //  000B  SETMBR	R0	K2	R1
      0x80040000,  //  000C  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: parameterized_object
********************************************************************/
be_local_class(parameterized_object,
    4,
    NULL,
    be_nested_map(32,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(_fix_time_ms, -1), be_const_closure(class_parameterized_object__fix_time_ms_closure) },
        { be_const_key_weak(produce_value, 22), be_const_closure(class_parameterized_object_produce_value_closure) },
        { be_const_key_weak(constraint_find, -1), be_const_static_closure(class_parameterized_object_constraint_find_closure) },
        { be_const_key_weak(_X21_X3D, 10), be_const_closure(class_parameterized_object__X21_X3D_closure) },
        { be_const_key_weak(_resolve_parameter_value, 2), be_const_closure(class_parameterized_object__resolve_parameter_value_closure) },
        { be_const_key_weak(tostring, 17), be_const_closure(class_parameterized_object_tostring_closure) },
        { be_const_key_weak(_MASK, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(6,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(min),
        be_nested_str_weak(max),
        be_nested_str_weak(default),
        be_nested_str_weak(type),
        be_nested_str_weak(enum),
        be_nested_str_weak(nillable),
    }))    ) } )) },
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
        { be_const_key_weak(is_running, -1), be_const_var(3) },
        { be_const_key_weak(get_param, -1), be_const_closure(class_parameterized_object_get_param_closure) },
        { be_const_key_weak(member, -1), be_const_closure(class_parameterized_object_member_closure) },
        { be_const_key_weak(get_param_value, -1), be_const_closure(class_parameterized_object_get_param_value_closure) },
        { be_const_key_weak(_set_parameter_value, -1), be_const_closure(class_parameterized_object__set_parameter_value_closure) },
        { be_const_key_weak(resolve_value, -1), be_const_closure(class_parameterized_object_resolve_value_closure) },
        { be_const_key_weak(constraint_mask, 18), be_const_static_closure(class_parameterized_object_constraint_mask_closure) },
        { be_const_key_weak(_X3D_X3D, -1), be_const_closure(class_parameterized_object__X3D_X3D_closure) },
        { be_const_key_weak(_get_param_def, -1), be_const_closure(class_parameterized_object__get_param_def_closure) },
        { be_const_key_weak(stop, -1), be_const_closure(class_parameterized_object_stop_closure) },
        { be_const_key_weak(engine, -1), be_const_var(1) },
        { be_const_key_weak(init, -1), be_const_closure(class_parameterized_object_init_closure) },
        { be_const_key_weak(update, -1), be_const_closure(class_parameterized_object_update_closure) },
        { be_const_key_weak(set_param, 24), be_const_closure(class_parameterized_object_set_param_closure) },
        { be_const_key_weak(setmember, -1), be_const_closure(class_parameterized_object_setmember_closure) },
        { be_const_key_weak(_init_parameter_values, -1), be_const_closure(class_parameterized_object__init_parameter_values_closure) },
        { be_const_key_weak(has_param, -1), be_const_closure(class_parameterized_object_has_param_closure) },
        { be_const_key_weak(start_time, -1), be_const_var(2) },
        { be_const_key_weak(_validate_param, -1), be_const_closure(class_parameterized_object__validate_param_closure) },
        { be_const_key_weak(values, 14), be_const_var(0) },
        { be_const_key_weak(tobool, -1), be_const_closure(class_parameterized_object_tobool_closure) },
        { be_const_key_weak(on_param_changed, 8), be_const_closure(class_parameterized_object_on_param_changed_closure) },
        { be_const_key_weak(VALUE_PROVIDER, 6), be_const_bool(0) },
        { be_const_key_weak(start, -1), be_const_closure(class_parameterized_object_start_closure) },
    })),
    be_str_weak(parameterized_object)
);
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
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(oscillator_value),
    /* K2   */  be_nested_str_weak(form),
    }),
    be_str_weak(smooth),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x540A0003,  //  0004  LDINT	R2	4
      0x90060402,  //  0005  SETMBR	R1	K2	R2
      0x80040200,  //  0006  RET	1	R1
    })
  )
);
/*******************************************************************/

// compact class 'palette_gradient' ktab size: 32, total: 47 (saved 120 bytes)
static const bvalue be_ktab_class_palette_gradient[32] = {
  /* K0   */  be_nested_str_weak(get_param),
  /* K1   */  be_nested_str_weak(color_source),
  /* K2   */  be_nested_str_weak(animation),
  /* K3   */  be_nested_str_weak(color_provider),
  /* K4   */  be_nested_str_weak(get_lut),
  /* K5   */  be_nested_str_weak(LUT_FACTOR),
  /* K6   */  be_const_int(0),
  /* K7   */  be_nested_str_weak(pixels),
  /* K8   */  be_nested_str_weak(_buffer),
  /* K9   */  be_nested_str_weak(value_buffer),
  /* K10  */  be_const_int(2),
  /* K11  */  be_const_int(1),
  /* K12  */  be_const_int(3),
  /* K13  */  be_nested_str_weak(start_time),
  /* K14  */  be_nested_str_weak(get_color_for_value),
  /* K15  */  be_nested_str_weak(set_pixel_color),
  /* K16  */  be_nested_str_weak(engine),
  /* K17  */  be_nested_str_weak(strip_length),
  /* K18  */  be_nested_str_weak(resize),
  /* K19  */  be_nested_str_weak(_update_value_buffer),
  /* K20  */  be_nested_str_weak(init),
  /* K21  */  be_nested_str_weak(_initialize_value_buffer),
  /* K22  */  be_nested_str_weak(member),
  /* K23  */  be_nested_str_weak(shift_period),
  /* K24  */  be_nested_str_weak(spatial_period),
  /* K25  */  be_nested_str_weak(phase_shift),
  /* K26  */  be_nested_str_weak(_spatial_period),
  /* K27  */  be_nested_str_weak(_phase_shift),
  /* K28  */  be_nested_str_weak(tasmota),
  /* K29  */  be_nested_str_weak(scale_uint),
  /* K30  */  be_const_int(522241),
  /* K31  */  be_nested_str_weak(on_param_changed),
};


extern const bclass be_class_palette_gradient;

/********************************************************************
** Solidified function: render
********************************************************************/
be_local_closure(class_palette_gradient_render,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_palette_gradient,     /* shared constants */
    be_str_weak(render),
    &be_const_str_solidified,
    ( &(const binstruction[75]) {  /* code */
      0x8C100100,  //  0000  GETMET	R4	R0	K0
      0x58180001,  //  0001  LDCONST	R6	K1
      0x7C100400,  //  0002  CALL	R4	2
      0x4C140000,  //  0003  LDNIL	R5
      0x1C140805,  //  0004  EQ	R5	R4	R5
      0x78160001,  //  0005  JMPF	R5	#0008
      0x50140000,  //  0006  LDBOOL	R5	0	0
      0x80040A00,  //  0007  RET	1	R5
      0x4C140000,  //  0008  LDNIL	R5
      0x6018000F,  //  0009  GETGBL	R6	G15
      0x5C1C0800,  //  000A  MOVE	R7	R4
      0xB8220400,  //  000B  GETNGBL	R8	K2
      0x88201103,  //  000C  GETMBR	R8	R8	K3
      0x7C180400,  //  000D  CALL	R6	2
      0x781A0028,  //  000E  JMPF	R6	#0038
      0x8C180904,  //  000F  GETMET	R6	R4	K4
      0x7C180200,  //  0010  CALL	R6	1
      0x5C140C00,  //  0011  MOVE	R5	R6
      0x4C1C0000,  //  0012  LDNIL	R7
      0x20180C07,  //  0013  NE	R6	R6	R7
      0x781A0022,  //  0014  JMPF	R6	#0038
      0x88180905,  //  0015  GETMBR	R6	R4	K5
      0x541E00FF,  //  0016  LDINT	R7	256
      0x3C1C0E06,  //  0017  SHR	R7	R7	R6
      0x58200006,  //  0018  LDCONST	R8	K6
      0x88240307,  //  0019  GETMBR	R9	R1	K7
      0x8C241308,  //  001A  GETMET	R9	R9	K8
      0x7C240200,  //  001B  CALL	R9	1
      0x8C280B08,  //  001C  GETMET	R10	R5	K8
      0x7C280200,  //  001D  CALL	R10	1
      0x882C0109,  //  001E  GETMBR	R11	R0	K9
      0x8C2C1708,  //  001F  GETMET	R11	R11	K8
      0x7C2C0200,  //  0020  CALL	R11	1
      0x14301003,  //  0021  LT	R12	R8	R3
      0x78320013,  //  0022  JMPF	R12	#0037
      0x94301608,  //  0023  GETIDX	R12	R11	R8
      0x3C341806,  //  0024  SHR	R13	R12	R6
      0x543A00FE,  //  0025  LDINT	R14	255
      0x1C38180E,  //  0026  EQ	R14	R12	R14
      0x783A0000,  //  0027  JMPF	R14	#0029
      0x5C340E00,  //  0028  MOVE	R13	R7
      0x38381B0A,  //  0029  SHL	R14	R13	K10
      0x0038140E,  //  002A  ADD	R14	R10	R14
      0x943C1D06,  //  002B  GETIDX	R15	R14	K6
      0x98260C0F,  //  002C  SETIDX	R9	K6	R15
      0x943C1D0B,  //  002D  GETIDX	R15	R14	K11
      0x9826160F,  //  002E  SETIDX	R9	K11	R15
      0x943C1D0A,  //  002F  GETIDX	R15	R14	K10
      0x9826140F,  //  0030  SETIDX	R9	K10	R15
      0x943C1D0C,  //  0031  GETIDX	R15	R14	K12
      0x9826180F,  //  0032  SETIDX	R9	K12	R15
      0x0020110B,  //  0033  ADD	R8	R8	K11
      0x543E0003,  //  0034  LDINT	R15	4
      0x0024120F,  //  0035  ADD	R9	R9	R15
      0x7001FFE9,  //  0036  JMP		#0021
      0x70020010,  //  0037  JMP		#0049
      0x8818010D,  //  0038  GETMBR	R6	R0	K13
      0x04180406,  //  0039  SUB	R6	R2	R6
      0x581C0006,  //  003A  LDCONST	R7	K6
      0x14200E03,  //  003B  LT	R8	R7	R3
      0x7822000B,  //  003C  JMPF	R8	#0049
      0x88200109,  //  003D  GETMBR	R8	R0	K9
      0x94201007,  //  003E  GETIDX	R8	R8	R7
      0x8C24090E,  //  003F  GETMET	R9	R4	K14
      0x5C2C1000,  //  0040  MOVE	R11	R8
      0x5C300C00,  //  0041  MOVE	R12	R6
      0x7C240600,  //  0042  CALL	R9	3
      0x8C28030F,  //  0043  GETMET	R10	R1	K15
      0x5C300E00,  //  0044  MOVE	R12	R7
      0x5C341200,  //  0045  MOVE	R13	R9
      0x7C280600,  //  0046  CALL	R10	3
      0x001C0F0B,  //  0047  ADD	R7	R7	K11
      0x7001FFF1,  //  0048  JMP		#003B
      0x50180200,  //  0049  LDBOOL	R6	1	0
      0x80040C00,  //  004A  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update
********************************************************************/
be_local_closure(class_palette_gradient_update,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_palette_gradient,     /* shared constants */
    be_str_weak(update),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8808010D,  //  0000  GETMBR	R2	R0	K13
      0x04080202,  //  0001  SUB	R2	R1	R2
      0x880C0110,  //  0002  GETMBR	R3	R0	K16
      0x880C0711,  //  0003  GETMBR	R3	R3	K17
      0x6010000C,  //  0004  GETGBL	R4	G12
      0x88140109,  //  0005  GETMBR	R5	R0	K9
      0x7C100200,  //  0006  CALL	R4	1
      0x20100803,  //  0007  NE	R4	R4	R3
      0x78120003,  //  0008  JMPF	R4	#000D
      0x88100109,  //  0009  GETMBR	R4	R0	K9
      0x8C100912,  //  000A  GETMET	R4	R4	K18
      0x5C180600,  //  000B  MOVE	R6	R3
      0x7C100400,  //  000C  CALL	R4	2
      0x8C100113,  //  000D  GETMET	R4	R0	K19
      0x5C180400,  //  000E  MOVE	R6	R2
      0x5C1C0600,  //  000F  MOVE	R7	R3
      0x7C100600,  //  0010  CALL	R4	3
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_palette_gradient_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_palette_gradient,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080514,  //  0003  GETMET	R2	R2	K20
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x60080015,  //  0006  GETGBL	R2	G21
      0x7C080000,  //  0007  CALL	R2	0
      0x90021202,  //  0008  SETMBR	R0	K9	R2
      0x8C080115,  //  0009  GETMET	R2	R0	K21
      0x7C080200,  //  000A  CALL	R2	1
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _update_value_buffer
********************************************************************/
be_local_closure(class_palette_gradient__update_value_buffer,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_palette_gradient,     /* shared constants */
    be_str_weak(_update_value_buffer),
    &be_const_str_solidified,
    ( &(const binstruction[72]) {  /* code */
      0x8C0C0116,  //  0000  GETMET	R3	R0	K22
      0x58140017,  //  0001  LDCONST	R5	K23
      0x7C0C0400,  //  0002  CALL	R3	2
      0x8C100116,  //  0003  GETMET	R4	R0	K22
      0x58180018,  //  0004  LDCONST	R6	K24
      0x7C100400,  //  0005  CALL	R4	2
      0x8C140116,  //  0006  GETMET	R5	R0	K22
      0x581C0019,  //  0007  LDCONST	R7	K25
      0x7C140400,  //  0008  CALL	R5	2
      0x1C180706,  //  0009  EQ	R6	R3	K6
      0x781A0011,  //  000A  JMPF	R6	#001D
      0x8818011A,  //  000B  GETMBR	R6	R0	K26
      0x4C1C0000,  //  000C  LDNIL	R7
      0x20180C07,  //  000D  NE	R6	R6	R7
      0x781A000B,  //  000E  JMPF	R6	#001B
      0x8818011A,  //  000F  GETMBR	R6	R0	K26
      0x1C180C04,  //  0010  EQ	R6	R6	R4
      0x781A0008,  //  0011  JMPF	R6	#001B
      0x8818011B,  //  0012  GETMBR	R6	R0	K27
      0x1C180C05,  //  0013  EQ	R6	R6	R5
      0x781A0005,  //  0014  JMPF	R6	#001B
      0x6018000C,  //  0015  GETGBL	R6	G12
      0x881C0109,  //  0016  GETMBR	R7	R0	K9
      0x7C180200,  //  0017  CALL	R6	1
      0x1C180C02,  //  0018  EQ	R6	R6	R2
      0x781A0000,  //  0019  JMPF	R6	#001B
      0x80000C00,  //  001A  RET	0
      0x90023404,  //  001B  SETMBR	R0	K26	R4
      0x90023605,  //  001C  SETMBR	R0	K27	R5
      0x24180906,  //  001D  GT	R6	R4	K6
      0x781A0001,  //  001E  JMPF	R6	#0021
      0x5C180800,  //  001F  MOVE	R6	R4
      0x70020000,  //  0020  JMP		#0022
      0x5C180400,  //  0021  MOVE	R6	R2
      0x581C0006,  //  0022  LDCONST	R7	K6
      0x24200706,  //  0023  GT	R8	R3	K6
      0x78220008,  //  0024  JMPF	R8	#002E
      0xB8223800,  //  0025  GETNGBL	R8	K28
      0x8C20111D,  //  0026  GETMET	R8	R8	K29
      0x10280203,  //  0027  MOD	R10	R1	R3
      0x582C0006,  //  0028  LDCONST	R11	K6
      0x5C300600,  //  0029  MOVE	R12	R3
      0x58340006,  //  002A  LDCONST	R13	K6
      0x5C380C00,  //  002B  MOVE	R14	R6
      0x7C200C00,  //  002C  CALL	R8	6
      0x5C1C1000,  //  002D  MOVE	R7	R8
      0xB8223800,  //  002E  GETNGBL	R8	K28
      0x8C20111D,  //  002F  GETMET	R8	R8	K29
      0x5C280A00,  //  0030  MOVE	R10	R5
      0x582C0006,  //  0031  LDCONST	R11	K6
      0x543200FE,  //  0032  LDINT	R12	255
      0x58340006,  //  0033  LDCONST	R13	K6
      0x5C380C00,  //  0034  MOVE	R14	R6
      0x7C200C00,  //  0035  CALL	R8	6
      0x58240006,  //  0036  LDCONST	R9	K6
      0x00280E08,  //  0037  ADD	R10	R7	R8
      0x10281406,  //  0038  MOD	R10	R10	R6
      0x0C2E3C06,  //  0039  DIV	R11	K30	R6
      0x3C2C170B,  //  003A  SHR	R11	R11	K11
      0x0830140B,  //  003B  MUL	R12	R10	R11
      0x88340109,  //  003C  GETMBR	R13	R0	K9
      0x8C341B08,  //  003D  GETMET	R13	R13	K8
      0x7C340200,  //  003E  CALL	R13	1
      0x14381202,  //  003F  LT	R14	R9	R2
      0x783A0005,  //  0040  JMPF	R14	#0047
      0x543A0009,  //  0041  LDINT	R14	10
      0x3C38180E,  //  0042  SHR	R14	R12	R14
      0x9834120E,  //  0043  SETIDX	R13	R9	R14
      0x0030180B,  //  0044  ADD	R12	R12	R11
      0x0024130B,  //  0045  ADD	R9	R9	K11
      0x7001FFF7,  //  0046  JMP		#003F
      0x80000000,  //  0047  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: on_param_changed
********************************************************************/
be_local_closure(class_palette_gradient_on_param_changed,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_palette_gradient,     /* shared constants */
    be_str_weak(on_param_changed),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C071F,  //  0003  GETMET	R3	R3	K31
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x1C0C0301,  //  0007  EQ	R3	R1	K1
      0x780E0001,  //  0008  JMPF	R3	#000B
      0x8C0C0115,  //  0009  GETMET	R3	R0	K21
      0x7C0C0200,  //  000A  CALL	R3	1
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _initialize_value_buffer
********************************************************************/
be_local_closure(class_palette_gradient__initialize_value_buffer,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_palette_gradient,     /* shared constants */
    be_str_weak(_initialize_value_buffer),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x88040110,  //  0000  GETMBR	R1	R0	K16
      0x88040311,  //  0001  GETMBR	R1	R1	K17
      0x88080109,  //  0002  GETMBR	R2	R0	K9
      0x8C080512,  //  0003  GETMET	R2	R2	K18
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x58080006,  //  0006  LDCONST	R2	K6
      0x140C0401,  //  0007  LT	R3	R2	R1
      0x780E0003,  //  0008  JMPF	R3	#000D
      0x880C0109,  //  0009  GETMBR	R3	R0	K9
      0x980C0506,  //  000A  SETIDX	R3	R2	K6
      0x0008050B,  //  000B  ADD	R2	R2	K11
      0x7001FFF9,  //  000C  JMP		#0007
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: palette_gradient
********************************************************************/
extern const bclass be_class_Animation;
be_local_class(palette_gradient,
    3,
    &be_class_Animation,
    be_nested_map(10,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(_spatial_period, -1), be_const_var(1) },
        { be_const_key_weak(value_buffer, -1), be_const_var(0) },
        { be_const_key_weak(on_param_changed, -1), be_const_closure(class_palette_gradient_on_param_changed_closure) },
        { be_const_key_weak(PARAMS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(color_source, -1), be_const_bytes_instance(0C0605) },
        { be_const_key_weak(shift_period, 2), be_const_bytes_instance(0500000000) },
        { be_const_key_weak(spatial_period, -1), be_const_bytes_instance(0500000000) },
        { be_const_key_weak(phase_shift, -1), be_const_bytes_instance(07000001FF000000) },
    }))    ) } )) },
        { be_const_key_weak(update, -1), be_const_closure(class_palette_gradient_update_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_palette_gradient_init_closure) },
        { be_const_key_weak(_update_value_buffer, -1), be_const_closure(class_palette_gradient__update_value_buffer_closure) },
        { be_const_key_weak(render, 2), be_const_closure(class_palette_gradient_render_closure) },
        { be_const_key_weak(_phase_shift, -1), be_const_var(2) },
        { be_const_key_weak(_initialize_value_buffer, -1), be_const_closure(class_palette_gradient__initialize_value_buffer_closure) },
    })),
    be_str_weak(palette_gradient)
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

// compact class 'comet' ktab size: 19, total: 32 (saved 104 bytes)
static const bvalue be_ktab_class_comet[19] = {
  /* K0   */  be_nested_str_weak(head_position),
  /* K1   */  be_nested_str_weak(color),
  /* K2   */  be_nested_str_weak(tail_length),
  /* K3   */  be_nested_str_weak(direction),
  /* K4   */  be_nested_str_weak(wrap_around),
  /* K5   */  be_nested_str_weak(fade_factor),
  /* K6   */  be_const_int(0),
  /* K7   */  be_const_int(1),
  /* K8   */  be_nested_str_weak(tasmota),
  /* K9   */  be_nested_str_weak(scale_uint),
  /* K10  */  be_nested_str_weak(width),
  /* K11  */  be_nested_str_weak(set_pixel_color),
  /* K12  */  be_nested_str_weak(init),
  /* K13  */  be_nested_str_weak(speed),
  /* K14  */  be_nested_str_weak(engine),
  /* K15  */  be_nested_str_weak(strip_length),
  /* K16  */  be_nested_str_weak(start_time),
  /* K17  */  be_nested_str_weak(on_param_changed),
  /* K18  */  be_nested_str_weak(int),
};


extern const bclass be_class_comet;

/********************************************************************
** Solidified function: render
********************************************************************/
be_local_closure(class_comet_render,   /* name */
  be_nested_proto(
    25,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_comet,     /* shared constants */
    be_str_weak(render),
    &be_const_str_solidified,
    ( &(const binstruction[83]) {  /* code */
      0x88100100,  //  0000  GETMBR	R4	R0	K0
      0x541600FF,  //  0001  LDINT	R5	256
      0x0C100805,  //  0002  DIV	R4	R4	R5
      0x88140101,  //  0003  GETMBR	R5	R0	K1
      0x88180102,  //  0004  GETMBR	R6	R0	K2
      0x881C0103,  //  0005  GETMBR	R7	R0	K3
      0x88200104,  //  0006  GETMBR	R8	R0	K4
      0x88240105,  //  0007  GETMBR	R9	R0	K5
      0x542A0017,  //  0008  LDINT	R10	24
      0x3C280A0A,  //  0009  SHR	R10	R5	R10
      0x542E00FE,  //  000A  LDINT	R11	255
      0x2C28140B,  //  000B  AND	R10	R10	R11
      0x542E000F,  //  000C  LDINT	R11	16
      0x3C2C0A0B,  //  000D  SHR	R11	R5	R11
      0x543200FE,  //  000E  LDINT	R12	255
      0x2C2C160C,  //  000F  AND	R11	R11	R12
      0x54320007,  //  0010  LDINT	R12	8
      0x3C300A0C,  //  0011  SHR	R12	R5	R12
      0x543600FE,  //  0012  LDINT	R13	255
      0x2C30180D,  //  0013  AND	R12	R12	R13
      0x543600FE,  //  0014  LDINT	R13	255
      0x2C340A0D,  //  0015  AND	R13	R5	R13
      0x58380006,  //  0016  LDCONST	R14	K6
      0x143C1C06,  //  0017  LT	R15	R14	R6
      0x783E0037,  //  0018  JMPF	R15	#0051
      0x083C1C07,  //  0019  MUL	R15	R14	R7
      0x043C080F,  //  001A  SUB	R15	R4	R15
      0x20401106,  //  001B  NE	R16	R8	K6
      0x78420008,  //  001C  JMPF	R16	#0026
      0x28401E03,  //  001D  GE	R16	R15	R3
      0x78420001,  //  001E  JMPF	R16	#0021
      0x043C1E03,  //  001F  SUB	R15	R15	R3
      0x7001FFFB,  //  0020  JMP		#001D
      0x14401F06,  //  0021  LT	R16	R15	K6
      0x78420001,  //  0022  JMPF	R16	#0025
      0x003C1E03,  //  0023  ADD	R15	R15	R3
      0x7001FFFB,  //  0024  JMP		#0021
      0x70020005,  //  0025  JMP		#002C
      0x14401F06,  //  0026  LT	R16	R15	K6
      0x74420001,  //  0027  JMPT	R16	#002A
      0x28401E03,  //  0028  GE	R16	R15	R3
      0x78420001,  //  0029  JMPF	R16	#002C
      0x00381D07,  //  002A  ADD	R14	R14	K7
      0x7001FFEA,  //  002B  JMP		#0017
      0x544200FE,  //  002C  LDINT	R16	255
      0x24441D06,  //  002D  GT	R17	R14	K6
      0x7846000D,  //  002E  JMPF	R17	#003D
      0x58440006,  //  002F  LDCONST	R17	K6
      0x1448220E,  //  0030  LT	R18	R17	R14
      0x784A000A,  //  0031  JMPF	R18	#003D
      0xB84A1000,  //  0032  GETNGBL	R18	K8
      0x8C482509,  //  0033  GETMET	R18	R18	K9
      0x5C502000,  //  0034  MOVE	R20	R16
      0x58540006,  //  0035  LDCONST	R21	K6
      0x545A00FE,  //  0036  LDINT	R22	255
      0x585C0006,  //  0037  LDCONST	R23	K6
      0x5C601200,  //  0038  MOVE	R24	R9
      0x7C480C00,  //  0039  CALL	R18	6
      0x5C402400,  //  003A  MOVE	R16	R18
      0x00442307,  //  003B  ADD	R17	R17	K7
      0x7001FFF2,  //  003C  JMP		#0030
      0x54460017,  //  003D  LDINT	R17	24
      0x38442011,  //  003E  SHL	R17	R16	R17
      0x544A000F,  //  003F  LDINT	R18	16
      0x38481612,  //  0040  SHL	R18	R11	R18
      0x30442212,  //  0041  OR	R17	R17	R18
      0x544A0007,  //  0042  LDINT	R18	8
      0x38481812,  //  0043  SHL	R18	R12	R18
      0x30442212,  //  0044  OR	R17	R17	R18
      0x3044220D,  //  0045  OR	R17	R17	R13
      0x28481F06,  //  0046  GE	R18	R15	K6
      0x784A0006,  //  0047  JMPF	R18	#004F
      0x8848030A,  //  0048  GETMBR	R18	R1	K10
      0x14481E12,  //  0049  LT	R18	R15	R18
      0x784A0003,  //  004A  JMPF	R18	#004F
      0x8C48030B,  //  004B  GETMET	R18	R1	K11
      0x5C501E00,  //  004C  MOVE	R20	R15
      0x5C542200,  //  004D  MOVE	R21	R17
      0x7C480600,  //  004E  CALL	R18	3
      0x00381D07,  //  004F  ADD	R14	R14	K7
      0x7001FFC5,  //  0050  JMP		#0017
      0x503C0200,  //  0051  LDBOOL	R15	1	0
      0x80041E00,  //  0052  RET	1	R15
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_comet_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_comet,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C08050C,  //  0003  GETMET	R2	R2	K12
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x90020106,  //  0006  SETMBR	R0	K0	K6
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update
********************************************************************/
be_local_closure(class_comet_update,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_comet,     /* shared constants */
    be_str_weak(update),
    &be_const_str_solidified,
    ( &(const binstruction[56]) {  /* code */
      0x8808010D,  //  0000  GETMBR	R2	R0	K13
      0x880C0103,  //  0001  GETMBR	R3	R0	K3
      0x88100104,  //  0002  GETMBR	R4	R0	K4
      0x8814010E,  //  0003  GETMBR	R5	R0	K14
      0x88140B0F,  //  0004  GETMBR	R5	R5	K15
      0x88180110,  //  0005  GETMBR	R6	R0	K16
      0x04180206,  //  0006  SUB	R6	R1	R6
      0x081C0406,  //  0007  MUL	R7	R2	R6
      0x081C0E03,  //  0008  MUL	R7	R7	R3
      0x542203E7,  //  0009  LDINT	R8	1000
      0x0C1C0E08,  //  000A  DIV	R7	R7	R8
      0x24200706,  //  000B  GT	R8	R3	K6
      0x78220001,  //  000C  JMPF	R8	#000F
      0x90020007,  //  000D  SETMBR	R0	K0	R7
      0x70020004,  //  000E  JMP		#0014
      0x04200B07,  //  000F  SUB	R8	R5	K7
      0x542600FF,  //  0010  LDINT	R9	256
      0x08201009,  //  0011  MUL	R8	R8	R9
      0x00201007,  //  0012  ADD	R8	R8	R7
      0x90020008,  //  0013  SETMBR	R0	K0	R8
      0x542200FF,  //  0014  LDINT	R8	256
      0x08200A08,  //  0015  MUL	R8	R5	R8
      0x20240906,  //  0016  NE	R9	R4	K6
      0x7826000E,  //  0017  JMPF	R9	#0027
      0x88240100,  //  0018  GETMBR	R9	R0	K0
      0x28241208,  //  0019  GE	R9	R9	R8
      0x78260003,  //  001A  JMPF	R9	#001F
      0x88240100,  //  001B  GETMBR	R9	R0	K0
      0x04241208,  //  001C  SUB	R9	R9	R8
      0x90020009,  //  001D  SETMBR	R0	K0	R9
      0x7001FFF8,  //  001E  JMP		#0018
      0x88240100,  //  001F  GETMBR	R9	R0	K0
      0x14241306,  //  0020  LT	R9	R9	K6
      0x78260003,  //  0021  JMPF	R9	#0026
      0x88240100,  //  0022  GETMBR	R9	R0	K0
      0x00241208,  //  0023  ADD	R9	R9	R8
      0x90020009,  //  0024  SETMBR	R0	K0	R9
      0x7001FFF8,  //  0025  JMP		#001F
      0x7002000F,  //  0026  JMP		#0037
      0x88240100,  //  0027  GETMBR	R9	R0	K0
      0x28241208,  //  0028  GE	R9	R9	R8
      0x78260006,  //  0029  JMPF	R9	#0031
      0x04240B07,  //  002A  SUB	R9	R5	K7
      0x542A00FF,  //  002B  LDINT	R10	256
      0x0824120A,  //  002C  MUL	R9	R9	R10
      0x90020009,  //  002D  SETMBR	R0	K0	R9
      0x44240600,  //  002E  NEG	R9	R3
      0x90020609,  //  002F  SETMBR	R0	K3	R9
      0x70020005,  //  0030  JMP		#0037
      0x88240100,  //  0031  GETMBR	R9	R0	K0
      0x14241306,  //  0032  LT	R9	R9	K6
      0x78260002,  //  0033  JMPF	R9	#0037
      0x90020106,  //  0034  SETMBR	R0	K0	K6
      0x44240600,  //  0035  NEG	R9	R3
      0x90020609,  //  0036  SETMBR	R0	K3	R9
      0x80000000,  //  0037  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: on_param_changed
********************************************************************/
be_local_closure(class_comet_on_param_changed,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_comet,     /* shared constants */
    be_str_weak(on_param_changed),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0711,  //  0003  GETMET	R3	R3	K17
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x1C0C0303,  //  0007  EQ	R3	R1	K3
      0x780E000E,  //  0008  JMPF	R3	#0018
      0x880C010E,  //  0009  GETMBR	R3	R0	K14
      0x880C070F,  //  000A  GETMBR	R3	R3	K15
      0x60100004,  //  000B  GETGBL	R4	G4
      0x5C140400,  //  000C  MOVE	R5	R2
      0x7C100200,  //  000D  CALL	R4	1
      0x1C100912,  //  000E  EQ	R4	R4	K18
      0x78120007,  //  000F  JMPF	R4	#0018
      0x24100506,  //  0010  GT	R4	R2	K6
      0x78120001,  //  0011  JMPF	R4	#0014
      0x90020106,  //  0012  SETMBR	R0	K0	K6
      0x70020003,  //  0013  JMP		#0018
      0x04100707,  //  0014  SUB	R4	R3	K7
      0x541600FF,  //  0015  LDINT	R5	256
      0x08100805,  //  0016  MUL	R4	R4	R5
      0x90020004,  //  0017  SETMBR	R0	K0	R4
      0x80000000,  //  0018  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: comet
********************************************************************/
extern const bclass be_class_Animation;
be_local_class(comet,
    1,
    &be_class_Animation,
    be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(on_param_changed, -1), be_const_closure(class_comet_on_param_changed_closure) },
        { be_const_key_weak(render, 0), be_const_closure(class_comet_render_closure) },
        { be_const_key_weak(PARAMS, 5), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(fade_factor, -1), be_const_bytes_instance(07000001FF0001B300) },
        { be_const_key_weak(wrap_around, -1), be_const_bytes_instance(07000000010001) },
        { be_const_key_weak(direction, -1), be_const_bytes_instance(1400010200FF0001) },
        { be_const_key_weak(speed, 0), be_const_bytes_instance(07000101006401000A) },
        { be_const_key_weak(tail_length, -1), be_const_bytes_instance(07000100320005) },
    }))    ) } )) },
        { be_const_key_weak(head_position, 2), be_const_var(0) },
        { be_const_key_weak(update, -1), be_const_closure(class_comet_update_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_comet_init_closure) },
    })),
    be_str_weak(comet)
);

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
    /* K3   */  be_const_int(2),
    }),
    be_str_weak(triangle),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x90060503,  //  0004  SETMBR	R1	K2	K3
      0x80040200,  //  0005  RET	1	R1
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
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(instance),
    /* K1   */  be_nested_str_weak(animation),
    /* K2   */  be_nested_str_weak(parameterized_object),
    /* K3   */  be_nested_str_weak(VALUE_PROVIDER),
    }),
    be_str_weak(is_value_provider),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x4C040000,  //  0000  LDNIL	R1
      0x20040001,  //  0001  NE	R1	R0	R1
      0x7806000C,  //  0002  JMPF	R1	#0010
      0x60040004,  //  0003  GETGBL	R1	G4
      0x5C080000,  //  0004  MOVE	R2	R0
      0x7C040200,  //  0005  CALL	R1	1
      0x1C040300,  //  0006  EQ	R1	R1	K0
      0x78060007,  //  0007  JMPF	R1	#0010
      0x6004000F,  //  0008  GETGBL	R1	G15
      0x5C080000,  //  0009  MOVE	R2	R0
      0xB80E0200,  //  000A  GETNGBL	R3	K1
      0x880C0702,  //  000B  GETMBR	R3	R3	K2
      0x7C040400,  //  000C  CALL	R1	2
      0x78060001,  //  000D  JMPF	R1	#0010
      0x88040103,  //  000E  GETMBR	R1	R0	K3
      0x74060000,  //  000F  JMPT	R1	#0011
      0x50040001,  //  0010  LDBOOL	R1	0	1
      0x50040200,  //  0011  LDBOOL	R1	1	0
      0x80040200,  //  0012  RET	1	R1
    })
  )
);
/*******************************************************************/

// compact class 'engine_proxy' ktab size: 40, total: 109 (saved 552 bytes)
static const bvalue be_ktab_class_engine_proxy[40] = {
  /* K0   */  be_nested_str_weak(engine),
  /* K1   */  be_nested_str_weak(strip_length),
  /* K2   */  be_nested_str_weak(animation),
  /* K3   */  be_nested_str_weak(sequence_manager),
  /* K4   */  be_nested_str_weak(_remove_sequence_manager),
  /* K5   */  be_nested_str_weak(is_value_provider),
  /* K6   */  be_nested_str_weak(_remove_value_provider),
  /* K7   */  be_nested_str_weak(_remove_animation),
  /* K8   */  be_nested_str_weak(animations),
  /* K9   */  be_nested_str_weak(find),
  /* K10  */  be_nested_str_weak(push),
  /* K11  */  be_nested_str_weak(_sort_animations_by_priority),
  /* K12  */  be_nested_str_weak(is_running),
  /* K13  */  be_nested_str_weak(start),
  /* K14  */  be_nested_str_weak(time_ms),
  /* K15  */  be_const_int(0),
  /* K16  */  be_nested_str_weak(sequences),
  /* K17  */  be_const_int(1),
  /* K18  */  be_nested_str_weak(value_providers),
  /* K19  */  be_nested_str_weak(remove),
  /* K20  */  be_nested_str_weak(iteration_stack),
  /* K21  */  be_nested_str_weak(stop),
  /* K22  */  be_nested_str_weak(_add_sequence_manager),
  /* K23  */  be_nested_str_weak(_add_value_provider),
  /* K24  */  be_nested_str_weak(_add_animation),
  /* K25  */  be_nested_str_weak(type_error),
  /* K26  */  be_nested_str_weak(only_X20Animation_X2C_X20sequence_manager_X2C_X20or_X20value_provider),
  /* K27  */  be_nested_str_weak(update),
  /* K28  */  be_nested_str_weak(start_time),
  /* K29  */  be_nested_str_weak(pop),
  /* K30  */  be_nested_str_weak(priority),
  /* K31  */  be_nested_str_weak(temp_buffer),
  /* K32  */  be_nested_str_weak(clear),
  /* K33  */  be_nested_str_weak(render),
  /* K34  */  be_nested_str_weak(post_render),
  /* K35  */  be_nested_str_weak(blend_pixels),
  /* K36  */  be_nested_str_weak(pixels),
  /* K37  */  be_nested_str_weak(stop_iteration),
  /* K38  */  be_nested_str_weak(init),
  /* K39  */  be_nested_str_weak(setup_template),
};


extern const bclass be_class_engine_proxy;

/********************************************************************
** Solidified function: get_strip_length
********************************************************************/
be_local_closure(class_engine_proxy_get_strip_length,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_engine_proxy,     /* shared constants */
    be_str_weak(get_strip_length),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x80040200,  //  0002  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove
********************************************************************/
be_local_closure(class_engine_proxy_remove,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_engine_proxy,     /* shared constants */
    be_str_weak(remove),
    &be_const_str_solidified,
    ( &(const binstruction[33]) {  /* code */
      0x6008000F,  //  0000  GETGBL	R2	G15
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0xB8120400,  //  0002  GETNGBL	R4	K2
      0x88100903,  //  0003  GETMBR	R4	R4	K3
      0x7C080400,  //  0004  CALL	R2	2
      0x780A0004,  //  0005  JMPF	R2	#000B
      0x8C080104,  //  0006  GETMET	R2	R0	K4
      0x5C100200,  //  0007  MOVE	R4	R1
      0x7C080400,  //  0008  CALL	R2	2
      0x80040400,  //  0009  RET	1	R2
      0x70020014,  //  000A  JMP		#0020
      0xB80A0400,  //  000B  GETNGBL	R2	K2
      0x8C080505,  //  000C  GETMET	R2	R2	K5
      0x5C100200,  //  000D  MOVE	R4	R1
      0x7C080400,  //  000E  CALL	R2	2
      0x780A0004,  //  000F  JMPF	R2	#0015
      0x8C080106,  //  0010  GETMET	R2	R0	K6
      0x5C100200,  //  0011  MOVE	R4	R1
      0x7C080400,  //  0012  CALL	R2	2
      0x80040400,  //  0013  RET	1	R2
      0x7002000A,  //  0014  JMP		#0020
      0x6008000F,  //  0015  GETGBL	R2	G15
      0x5C0C0200,  //  0016  MOVE	R3	R1
      0xB8120400,  //  0017  GETNGBL	R4	K2
      0x88100902,  //  0018  GETMBR	R4	R4	K2
      0x7C080400,  //  0019  CALL	R2	2
      0x780A0004,  //  001A  JMPF	R2	#0020
      0x8C080107,  //  001B  GETMET	R2	R0	K7
      0x5C100200,  //  001C  MOVE	R4	R1
      0x7C080400,  //  001D  CALL	R2	2
      0x80040400,  //  001E  RET	1	R2
      0x7001FFFF,  //  001F  JMP		#0020
      0x80000000,  //  0020  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: size_animations
********************************************************************/
be_local_closure(class_engine_proxy_size_animations,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_engine_proxy,     /* shared constants */
    be_str_weak(size_animations),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x6004000C,  //  0000  GETGBL	R1	G12
      0x88080108,  //  0001  GETMBR	R2	R0	K8
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _add_animation
********************************************************************/
be_local_closure(class_engine_proxy__add_animation,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_engine_proxy,     /* shared constants */
    be_str_weak(_add_animation),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x88080108,  //  0000  GETMBR	R2	R0	K8
      0x8C080509,  //  0001  GETMET	R2	R2	K9
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x4C0C0000,  //  0004  LDNIL	R3
      0x1C080403,  //  0005  EQ	R2	R2	R3
      0x780A000E,  //  0006  JMPF	R2	#0016
      0x88080108,  //  0007  GETMBR	R2	R0	K8
      0x8C08050A,  //  0008  GETMET	R2	R2	K10
      0x5C100200,  //  0009  MOVE	R4	R1
      0x7C080400,  //  000A  CALL	R2	2
      0x8C08010B,  //  000B  GETMET	R2	R0	K11
      0x7C080200,  //  000C  CALL	R2	1
      0x8808010C,  //  000D  GETMBR	R2	R0	K12
      0x780A0003,  //  000E  JMPF	R2	#0013
      0x8C08030D,  //  000F  GETMET	R2	R1	K13
      0x88100100,  //  0010  GETMBR	R4	R0	K0
      0x8810090E,  //  0011  GETMBR	R4	R4	K14
      0x7C080400,  //  0012  CALL	R2	2
      0x50080200,  //  0013  LDBOOL	R2	1	0
      0x80040400,  //  0014  RET	1	R2
      0x70020001,  //  0015  JMP		#0018
      0x50080000,  //  0016  LDBOOL	R2	0	0
      0x80040400,  //  0017  RET	1	R2
      0x80000000,  //  0018  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(class_engine_proxy_start,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_engine_proxy,     /* shared constants */
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[46]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C08050D,  //  0003  GETMET	R2	R2	K13
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x5808000F,  //  0006  LDCONST	R2	K15
      0x600C000C,  //  0007  GETGBL	R3	G12
      0x88100110,  //  0008  GETMBR	R4	R0	K16
      0x7C0C0200,  //  0009  CALL	R3	1
      0x140C0403,  //  000A  LT	R3	R2	R3
      0x780E0006,  //  000B  JMPF	R3	#0013
      0x880C0110,  //  000C  GETMBR	R3	R0	K16
      0x940C0602,  //  000D  GETIDX	R3	R3	R2
      0x8C0C070D,  //  000E  GETMET	R3	R3	K13
      0x5C140200,  //  000F  MOVE	R5	R1
      0x7C0C0400,  //  0010  CALL	R3	2
      0x00080511,  //  0011  ADD	R2	R2	K17
      0x7001FFF3,  //  0012  JMP		#0007
      0x5808000F,  //  0013  LDCONST	R2	K15
      0x600C000C,  //  0014  GETGBL	R3	G12
      0x88100112,  //  0015  GETMBR	R4	R0	K18
      0x7C0C0200,  //  0016  CALL	R3	1
      0x140C0403,  //  0017  LT	R3	R2	R3
      0x780E0006,  //  0018  JMPF	R3	#0020
      0x880C0112,  //  0019  GETMBR	R3	R0	K18
      0x940C0602,  //  001A  GETIDX	R3	R3	R2
      0x8C0C070D,  //  001B  GETMET	R3	R3	K13
      0x5C140200,  //  001C  MOVE	R5	R1
      0x7C0C0400,  //  001D  CALL	R3	2
      0x00080511,  //  001E  ADD	R2	R2	K17
      0x7001FFF3,  //  001F  JMP		#0014
      0x5808000F,  //  0020  LDCONST	R2	K15
      0x600C000C,  //  0021  GETGBL	R3	G12
      0x88100108,  //  0022  GETMBR	R4	R0	K8
      0x7C0C0200,  //  0023  CALL	R3	1
      0x140C0403,  //  0024  LT	R3	R2	R3
      0x780E0006,  //  0025  JMPF	R3	#002D
      0x880C0108,  //  0026  GETMBR	R3	R0	K8
      0x940C0602,  //  0027  GETIDX	R3	R3	R2
      0x8C0C070D,  //  0028  GETMET	R3	R3	K13
      0x5C140200,  //  0029  MOVE	R5	R1
      0x7C0C0400,  //  002A  CALL	R3	2
      0x00080511,  //  002B  ADD	R2	R2	K17
      0x7001FFF3,  //  002C  JMP		#0021
      0x80040000,  //  002D  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _remove_value_provider
********************************************************************/
be_local_closure(class_engine_proxy__remove_value_provider,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_engine_proxy,     /* shared constants */
    be_str_weak(_remove_value_provider),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x88080112,  //  0000  GETMBR	R2	R0	K18
      0x8C080509,  //  0001  GETMET	R2	R2	K9
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x4C0C0000,  //  0004  LDNIL	R3
      0x200C0403,  //  0005  NE	R3	R2	R3
      0x780E0006,  //  0006  JMPF	R3	#000E
      0x880C0112,  //  0007  GETMBR	R3	R0	K18
      0x8C0C0713,  //  0008  GETMET	R3	R3	K19
      0x5C140400,  //  0009  MOVE	R5	R2
      0x7C0C0400,  //  000A  CALL	R3	2
      0x500C0200,  //  000B  LDBOOL	R3	1	0
      0x80040600,  //  000C  RET	1	R3
      0x70020001,  //  000D  JMP		#0010
      0x500C0000,  //  000E  LDBOOL	R3	0	0
      0x80040600,  //  000F  RET	1	R3
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _remove_animation
********************************************************************/
be_local_closure(class_engine_proxy__remove_animation,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_engine_proxy,     /* shared constants */
    be_str_weak(_remove_animation),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x88080108,  //  0000  GETMBR	R2	R0	K8
      0x8C080509,  //  0001  GETMET	R2	R2	K9
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x4C0C0000,  //  0004  LDNIL	R3
      0x200C0403,  //  0005  NE	R3	R2	R3
      0x780E0006,  //  0006  JMPF	R3	#000E
      0x880C0108,  //  0007  GETMBR	R3	R0	K8
      0x8C0C0713,  //  0008  GETMET	R3	R3	K19
      0x5C140400,  //  0009  MOVE	R5	R2
      0x7C0C0400,  //  000A  CALL	R3	2
      0x500C0200,  //  000B  LDBOOL	R3	1	0
      0x80040600,  //  000C  RET	1	R3
      0x70020001,  //  000D  JMP		#0010
      0x500C0000,  //  000E  LDBOOL	R3	0	0
      0x80040600,  //  000F  RET	1	R3
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: setup_template
********************************************************************/
be_local_closure(class_engine_proxy_setup_template,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_engine_proxy,     /* shared constants */
    be_str_weak(setup_template),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_current_iteration
********************************************************************/
be_local_closure(class_engine_proxy_update_current_iteration,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_engine_proxy,     /* shared constants */
    be_str_weak(update_current_iteration),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x6008000C,  //  0000  GETGBL	R2	G12
      0x880C0114,  //  0001  GETMBR	R3	R0	K20
      0x7C080200,  //  0002  CALL	R2	1
      0x2408050F,  //  0003  GT	R2	R2	K15
      0x780A0002,  //  0004  JMPF	R2	#0008
      0x88080114,  //  0005  GETMBR	R2	R0	K20
      0x540DFFFE,  //  0006  LDINT	R3	-1
      0x98080601,  //  0007  SETIDX	R2	R3	R1
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: clear
********************************************************************/
be_local_closure(class_engine_proxy_clear,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_engine_proxy,     /* shared constants */
    be_str_weak(clear),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x8C040115,  //  0000  GETMET	R1	R0	K21
      0x7C040200,  //  0001  CALL	R1	1
      0x60040012,  //  0002  GETGBL	R1	G18
      0x7C040000,  //  0003  CALL	R1	0
      0x90021001,  //  0004  SETMBR	R0	K8	R1
      0x60040012,  //  0005  GETGBL	R1	G18
      0x7C040000,  //  0006  CALL	R1	0
      0x90022001,  //  0007  SETMBR	R0	K16	R1
      0x60040012,  //  0008  GETGBL	R1	G18
      0x7C040000,  //  0009  CALL	R1	0
      0x90022401,  //  000A  SETMBR	R0	K18	R1
      0x80040000,  //  000B  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _add_value_provider
********************************************************************/
be_local_closure(class_engine_proxy__add_value_provider,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_engine_proxy,     /* shared constants */
    be_str_weak(_add_value_provider),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x88080112,  //  0000  GETMBR	R2	R0	K18
      0x8C080509,  //  0001  GETMET	R2	R2	K9
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x4C0C0000,  //  0004  LDNIL	R3
      0x1C080403,  //  0005  EQ	R2	R2	R3
      0x780A0006,  //  0006  JMPF	R2	#000E
      0x88080112,  //  0007  GETMBR	R2	R0	K18
      0x8C08050A,  //  0008  GETMET	R2	R2	K10
      0x5C100200,  //  0009  MOVE	R4	R1
      0x7C080400,  //  000A  CALL	R2	2
      0x50080200,  //  000B  LDBOOL	R2	1	0
      0x80040400,  //  000C  RET	1	R2
      0x70020001,  //  000D  JMP		#0010
      0x50080000,  //  000E  LDBOOL	R2	0	0
      0x80040400,  //  000F  RET	1	R2
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _add_sequence_manager
********************************************************************/
be_local_closure(class_engine_proxy__add_sequence_manager,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_engine_proxy,     /* shared constants */
    be_str_weak(_add_sequence_manager),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x88080110,  //  0000  GETMBR	R2	R0	K16
      0x8C080509,  //  0001  GETMET	R2	R2	K9
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x4C0C0000,  //  0004  LDNIL	R3
      0x1C080403,  //  0005  EQ	R2	R2	R3
      0x780A0006,  //  0006  JMPF	R2	#000E
      0x88080110,  //  0007  GETMBR	R2	R0	K16
      0x8C08050A,  //  0008  GETMET	R2	R2	K10
      0x5C100200,  //  0009  MOVE	R4	R1
      0x7C080400,  //  000A  CALL	R2	2
      0x50080200,  //  000B  LDBOOL	R2	1	0
      0x80040400,  //  000C  RET	1	R2
      0x70020001,  //  000D  JMP		#0010
      0x50080000,  //  000E  LDBOOL	R2	0	0
      0x80040400,  //  000F  RET	1	R2
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: stop
********************************************************************/
be_local_closure(class_engine_proxy_stop,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_engine_proxy,     /* shared constants */
    be_str_weak(stop),
    &be_const_str_solidified,
    ( &(const binstruction[30]) {  /* code */
      0x5804000F,  //  0000  LDCONST	R1	K15
      0x6008000C,  //  0001  GETGBL	R2	G12
      0x880C0108,  //  0002  GETMBR	R3	R0	K8
      0x7C080200,  //  0003  CALL	R2	1
      0x14080202,  //  0004  LT	R2	R1	R2
      0x780A0005,  //  0005  JMPF	R2	#000C
      0x88080108,  //  0006  GETMBR	R2	R0	K8
      0x94080401,  //  0007  GETIDX	R2	R2	R1
      0x8C080515,  //  0008  GETMET	R2	R2	K21
      0x7C080200,  //  0009  CALL	R2	1
      0x00040311,  //  000A  ADD	R1	R1	K17
      0x7001FFF4,  //  000B  JMP		#0001
      0x5804000F,  //  000C  LDCONST	R1	K15
      0x6008000C,  //  000D  GETGBL	R2	G12
      0x880C0110,  //  000E  GETMBR	R3	R0	K16
      0x7C080200,  //  000F  CALL	R2	1
      0x14080202,  //  0010  LT	R2	R1	R2
      0x780A0005,  //  0011  JMPF	R2	#0018
      0x88080110,  //  0012  GETMBR	R2	R0	K16
      0x94080401,  //  0013  GETIDX	R2	R2	R1
      0x8C080515,  //  0014  GETMET	R2	R2	K21
      0x7C080200,  //  0015  CALL	R2	1
      0x00040311,  //  0016  ADD	R1	R1	K17
      0x7001FFF4,  //  0017  JMP		#000D
      0x60080003,  //  0018  GETGBL	R2	G3
      0x5C0C0000,  //  0019  MOVE	R3	R0
      0x7C080200,  //  001A  CALL	R2	1
      0x8C080515,  //  001B  GETMET	R2	R2	K21
      0x7C080200,  //  001C  CALL	R2	1
      0x80040000,  //  001D  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add
********************************************************************/
be_local_closure(class_engine_proxy_add,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_engine_proxy,     /* shared constants */
    be_str_weak(add),
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
      0x6008000F,  //  0000  GETGBL	R2	G15
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0xB8120400,  //  0002  GETNGBL	R4	K2
      0x88100903,  //  0003  GETMBR	R4	R4	K3
      0x7C080400,  //  0004  CALL	R2	2
      0x780A0004,  //  0005  JMPF	R2	#000B
      0x8C080116,  //  0006  GETMET	R2	R0	K22
      0x5C100200,  //  0007  MOVE	R4	R1
      0x7C080400,  //  0008  CALL	R2	2
      0x80040400,  //  0009  RET	1	R2
      0x70020015,  //  000A  JMP		#0021
      0xB80A0400,  //  000B  GETNGBL	R2	K2
      0x8C080505,  //  000C  GETMET	R2	R2	K5
      0x5C100200,  //  000D  MOVE	R4	R1
      0x7C080400,  //  000E  CALL	R2	2
      0x780A0004,  //  000F  JMPF	R2	#0015
      0x8C080117,  //  0010  GETMET	R2	R0	K23
      0x5C100200,  //  0011  MOVE	R4	R1
      0x7C080400,  //  0012  CALL	R2	2
      0x80040400,  //  0013  RET	1	R2
      0x7002000B,  //  0014  JMP		#0021
      0x6008000F,  //  0015  GETGBL	R2	G15
      0x5C0C0200,  //  0016  MOVE	R3	R1
      0xB8120400,  //  0017  GETNGBL	R4	K2
      0x88100902,  //  0018  GETMBR	R4	R4	K2
      0x7C080400,  //  0019  CALL	R2	2
      0x780A0004,  //  001A  JMPF	R2	#0020
      0x8C080118,  //  001B  GETMET	R2	R0	K24
      0x5C100200,  //  001C  MOVE	R4	R1
      0x7C080400,  //  001D  CALL	R2	2
      0x80040400,  //  001E  RET	1	R2
      0x70020000,  //  001F  JMP		#0021
      0xB006331A,  //  0020  RAISE	1	K25	K26
      0x80000000,  //  0021  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update
********************************************************************/
be_local_closure(class_engine_proxy_update,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_engine_proxy,     /* shared constants */
    be_str_weak(update),
    &be_const_str_solidified,
    ( &(const binstruction[73]) {  /* code */
      0x90021C01,  //  0000  SETMBR	R0	K14	R1
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x88080501,  //  0002  GETMBR	R2	R2	K1
      0x90020202,  //  0003  SETMBR	R0	K1	R2
      0x60080003,  //  0004  GETGBL	R2	G3
      0x5C0C0000,  //  0005  MOVE	R3	R0
      0x7C080200,  //  0006  CALL	R2	1
      0x8C08051B,  //  0007  GETMET	R2	R2	K27
      0x5C100200,  //  0008  MOVE	R4	R1
      0x7C080400,  //  0009  CALL	R2	2
      0x5808000F,  //  000A  LDCONST	R2	K15
      0x600C000C,  //  000B  GETGBL	R3	G12
      0x88100112,  //  000C  GETMBR	R4	R0	K18
      0x7C0C0200,  //  000D  CALL	R3	1
      0x14100403,  //  000E  LT	R4	R2	R3
      0x7812000D,  //  000F  JMPF	R4	#001E
      0x88100112,  //  0010  GETMBR	R4	R0	K18
      0x94100802,  //  0011  GETIDX	R4	R4	R2
      0x8814090C,  //  0012  GETMBR	R5	R4	K12
      0x78160007,  //  0013  JMPF	R5	#001C
      0x8814091C,  //  0014  GETMBR	R5	R4	K28
      0x4C180000,  //  0015  LDNIL	R6
      0x1C140A06,  //  0016  EQ	R5	R5	R6
      0x78160000,  //  0017  JMPF	R5	#0019
      0x90123801,  //  0018  SETMBR	R4	K28	R1
      0x8C14091B,  //  0019  GETMET	R5	R4	K27
      0x5C1C0200,  //  001A  MOVE	R7	R1
      0x7C140400,  //  001B  CALL	R5	2
      0x00080511,  //  001C  ADD	R2	R2	K17
      0x7001FFEF,  //  001D  JMP		#000E
      0x5808000F,  //  001E  LDCONST	R2	K15
      0x6010000C,  //  001F  GETGBL	R4	G12
      0x88140110,  //  0020  GETMBR	R5	R0	K16
      0x7C100200,  //  0021  CALL	R4	1
      0x5C0C0800,  //  0022  MOVE	R3	R4
      0x14100403,  //  0023  LT	R4	R2	R3
      0x7812000D,  //  0024  JMPF	R4	#0033
      0x88100110,  //  0025  GETMBR	R4	R0	K16
      0x94100802,  //  0026  GETIDX	R4	R4	R2
      0x8814090C,  //  0027  GETMBR	R5	R4	K12
      0x78160007,  //  0028  JMPF	R5	#0031
      0x8814091C,  //  0029  GETMBR	R5	R4	K28
      0x4C180000,  //  002A  LDNIL	R6
      0x1C140A06,  //  002B  EQ	R5	R5	R6
      0x78160000,  //  002C  JMPF	R5	#002E
      0x90123801,  //  002D  SETMBR	R4	K28	R1
      0x8C14091B,  //  002E  GETMET	R5	R4	K27
      0x5C1C0200,  //  002F  MOVE	R7	R1
      0x7C140400,  //  0030  CALL	R5	2
      0x00080511,  //  0031  ADD	R2	R2	K17
      0x7001FFEF,  //  0032  JMP		#0023
      0x5808000F,  //  0033  LDCONST	R2	K15
      0x6010000C,  //  0034  GETGBL	R4	G12
      0x88140108,  //  0035  GETMBR	R5	R0	K8
      0x7C100200,  //  0036  CALL	R4	1
      0x5C0C0800,  //  0037  MOVE	R3	R4
      0x14100403,  //  0038  LT	R4	R2	R3
      0x7812000D,  //  0039  JMPF	R4	#0048
      0x88100108,  //  003A  GETMBR	R4	R0	K8
      0x94100802,  //  003B  GETIDX	R4	R4	R2
      0x8814090C,  //  003C  GETMBR	R5	R4	K12
      0x78160007,  //  003D  JMPF	R5	#0046
      0x8814091C,  //  003E  GETMBR	R5	R4	K28
      0x4C180000,  //  003F  LDNIL	R6
      0x1C140A06,  //  0040  EQ	R5	R5	R6
      0x78160000,  //  0041  JMPF	R5	#0043
      0x90123801,  //  0042  SETMBR	R4	K28	R1
      0x8C14091B,  //  0043  GETMET	R5	R4	K27
      0x5C1C0200,  //  0044  MOVE	R7	R1
      0x7C140400,  //  0045  CALL	R5	2
      0x00080511,  //  0046  ADD	R2	R2	K17
      0x7001FFEF,  //  0047  JMP		#0038
      0x80000000,  //  0048  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: pop_iteration_context
********************************************************************/
be_local_closure(class_engine_proxy_pop_iteration_context,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_engine_proxy,     /* shared constants */
    be_str_weak(pop_iteration_context),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x6004000C,  //  0000  GETGBL	R1	G12
      0x88080114,  //  0001  GETMBR	R2	R0	K20
      0x7C040200,  //  0002  CALL	R1	1
      0x2404030F,  //  0003  GT	R1	R1	K15
      0x78060003,  //  0004  JMPF	R1	#0009
      0x88040114,  //  0005  GETMBR	R1	R0	K20
      0x8C04031D,  //  0006  GETMET	R1	R1	K29
      0x7C040200,  //  0007  CALL	R1	1
      0x80040200,  //  0008  RET	1	R1
      0x4C040000,  //  0009  LDNIL	R1
      0x80040200,  //  000A  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _remove_sequence_manager
********************************************************************/
be_local_closure(class_engine_proxy__remove_sequence_manager,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_engine_proxy,     /* shared constants */
    be_str_weak(_remove_sequence_manager),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x88080110,  //  0000  GETMBR	R2	R0	K16
      0x8C080509,  //  0001  GETMET	R2	R2	K9
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x4C0C0000,  //  0004  LDNIL	R3
      0x200C0403,  //  0005  NE	R3	R2	R3
      0x780E0006,  //  0006  JMPF	R3	#000E
      0x880C0110,  //  0007  GETMBR	R3	R0	K16
      0x8C0C0713,  //  0008  GETMET	R3	R3	K19
      0x5C140400,  //  0009  MOVE	R5	R2
      0x7C0C0400,  //  000A  CALL	R3	2
      0x500C0200,  //  000B  LDBOOL	R3	1	0
      0x80040600,  //  000C  RET	1	R3
      0x70020001,  //  000D  JMP		#0010
      0x500C0000,  //  000E  LDBOOL	R3	0	0
      0x80040600,  //  000F  RET	1	R3
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_empty
********************************************************************/
be_local_closure(class_engine_proxy_is_empty,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_engine_proxy,     /* shared constants */
    be_str_weak(is_empty),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x6004000C,  //  0000  GETGBL	R1	G12
      0x88080108,  //  0001  GETMBR	R2	R0	K8
      0x7C040200,  //  0002  CALL	R1	1
      0x1C04030F,  //  0003  EQ	R1	R1	K15
      0x78060009,  //  0004  JMPF	R1	#000F
      0x6004000C,  //  0005  GETGBL	R1	G12
      0x88080110,  //  0006  GETMBR	R2	R0	K16
      0x7C040200,  //  0007  CALL	R1	1
      0x1C04030F,  //  0008  EQ	R1	R1	K15
      0x78060004,  //  0009  JMPF	R1	#000F
      0x6004000C,  //  000A  GETGBL	R1	G12
      0x88080112,  //  000B  GETMBR	R2	R0	K18
      0x7C040200,  //  000C  CALL	R1	1
      0x1C04030F,  //  000D  EQ	R1	R1	K15
      0x74060000,  //  000E  JMPT	R1	#0010
      0x50040001,  //  000F  LDBOOL	R1	0	1
      0x50040200,  //  0010  LDBOOL	R1	1	0
      0x80040200,  //  0011  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _sort_animations_by_priority
********************************************************************/
be_local_closure(class_engine_proxy__sort_animations_by_priority,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_engine_proxy,     /* shared constants */
    be_str_weak(_sort_animations_by_priority),
    &be_const_str_solidified,
    ( &(const binstruction[48]) {  /* code */
      0x6004000C,  //  0000  GETGBL	R1	G12
      0x88080108,  //  0001  GETMBR	R2	R0	K8
      0x7C040200,  //  0002  CALL	R1	1
      0x18080311,  //  0003  LE	R2	R1	K17
      0x780A0000,  //  0004  JMPF	R2	#0006
      0x80000400,  //  0005  RET	0
      0x58080011,  //  0006  LDCONST	R2	K17
      0x140C0401,  //  0007  LT	R3	R2	R1
      0x780E0025,  //  0008  JMPF	R3	#002F
      0x880C0108,  //  0009  GETMBR	R3	R0	K8
      0x940C0602,  //  000A  GETIDX	R3	R3	R2
      0x6010000F,  //  000B  GETGBL	R4	G15
      0x5C140600,  //  000C  MOVE	R5	R3
      0xB81A0400,  //  000D  GETNGBL	R6	K2
      0x88180D02,  //  000E  GETMBR	R6	R6	K2
      0x7C100400,  //  000F  CALL	R4	2
      0x74120001,  //  0010  JMPT	R4	#0013
      0x00080511,  //  0011  ADD	R2	R2	K17
      0x7001FFF3,  //  0012  JMP		#0007
      0x5C100400,  //  0013  MOVE	R4	R2
      0x2414090F,  //  0014  GT	R5	R4	K15
      0x78160014,  //  0015  JMPF	R5	#002B
      0x04140911,  //  0016  SUB	R5	R4	K17
      0x88180108,  //  0017  GETMBR	R6	R0	K8
      0x94140C05,  //  0018  GETIDX	R5	R6	R5
      0x6018000F,  //  0019  GETGBL	R6	G15
      0x5C1C0A00,  //  001A  MOVE	R7	R5
      0xB8220400,  //  001B  GETNGBL	R8	K2
      0x88201102,  //  001C  GETMBR	R8	R8	K2
      0x7C180400,  //  001D  CALL	R6	2
      0x781A0003,  //  001E  JMPF	R6	#0023
      0x88180B1E,  //  001F  GETMBR	R6	R5	K30
      0x881C071E,  //  0020  GETMBR	R7	R3	K30
      0x28180C07,  //  0021  GE	R6	R6	R7
      0x781A0000,  //  0022  JMPF	R6	#0024
      0x70020006,  //  0023  JMP		#002B
      0x88180108,  //  0024  GETMBR	R6	R0	K8
      0x041C0911,  //  0025  SUB	R7	R4	K17
      0x88200108,  //  0026  GETMBR	R8	R0	K8
      0x941C1007,  //  0027  GETIDX	R7	R8	R7
      0x98180807,  //  0028  SETIDX	R6	R4	R7
      0x04100911,  //  0029  SUB	R4	R4	K17
      0x7001FFE8,  //  002A  JMP		#0014
      0x88140108,  //  002B  GETMBR	R5	R0	K8
      0x98140803,  //  002C  SETIDX	R5	R4	R3
      0x00080511,  //  002D  ADD	R2	R2	K17
      0x7001FFD7,  //  002E  JMP		#0007
      0x80000000,  //  002F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: render
********************************************************************/
be_local_closure(class_engine_proxy_render,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_engine_proxy,     /* shared constants */
    be_str_weak(render),
    &be_const_str_solidified,
    ( &(const binstruction[45]) {  /* code */
      0x8810010C,  //  0000  GETMBR	R4	R0	K12
      0x78120002,  //  0001  JMPF	R4	#0005
      0x4C100000,  //  0002  LDNIL	R4
      0x1C100204,  //  0003  EQ	R4	R1	R4
      0x78120001,  //  0004  JMPF	R4	#0007
      0x50100000,  //  0005  LDBOOL	R4	0	0
      0x80040800,  //  0006  RET	1	R4
      0x4C100000,  //  0007  LDNIL	R4
      0x1C100604,  //  0008  EQ	R4	R3	R4
      0x78120000,  //  0009  JMPF	R4	#000B
      0x880C0101,  //  000A  GETMBR	R3	R0	K1
      0x50100000,  //  000B  LDBOOL	R4	0	0
      0x5814000F,  //  000C  LDCONST	R5	K15
      0x6018000C,  //  000D  GETGBL	R6	G12
      0x881C0108,  //  000E  GETMBR	R7	R0	K8
      0x7C180200,  //  000F  CALL	R6	1
      0x141C0A06,  //  0010  LT	R7	R5	R6
      0x781E0019,  //  0011  JMPF	R7	#002C
      0x881C0108,  //  0012  GETMBR	R7	R0	K8
      0x941C0E05,  //  0013  GETIDX	R7	R7	R5
      0x88200F0C,  //  0014  GETMBR	R8	R7	K12
      0x78220013,  //  0015  JMPF	R8	#002A
      0x8820011F,  //  0016  GETMBR	R8	R0	K31
      0x8C201120,  //  0017  GETMET	R8	R8	K32
      0x7C200200,  //  0018  CALL	R8	1
      0x8C200F21,  //  0019  GETMET	R8	R7	K33
      0x8828011F,  //  001A  GETMBR	R10	R0	K31
      0x5C2C0400,  //  001B  MOVE	R11	R2
      0x5C300600,  //  001C  MOVE	R12	R3
      0x7C200800,  //  001D  CALL	R8	4
      0x7822000A,  //  001E  JMPF	R8	#002A
      0x8C240F22,  //  001F  GETMET	R9	R7	K34
      0x882C011F,  //  0020  GETMBR	R11	R0	K31
      0x5C300400,  //  0021  MOVE	R12	R2
      0x5C340600,  //  0022  MOVE	R13	R3
      0x7C240800,  //  0023  CALL	R9	4
      0x8C240323,  //  0024  GETMET	R9	R1	K35
      0x882C0324,  //  0025  GETMBR	R11	R1	K36
      0x8830011F,  //  0026  GETMBR	R12	R0	K31
      0x88301924,  //  0027  GETMBR	R12	R12	K36
      0x7C240600,  //  0028  CALL	R9	3
      0x50100200,  //  0029  LDBOOL	R4	1	0
      0x00140B11,  //  002A  ADD	R5	R5	K17
      0x7001FFE3,  //  002B  JMP		#0010
      0x80040800,  //  002C  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_current_iteration_number
********************************************************************/
be_local_closure(class_engine_proxy_get_current_iteration_number,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_engine_proxy,     /* shared constants */
    be_str_weak(get_current_iteration_number),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x6004000C,  //  0000  GETGBL	R1	G12
      0x88080114,  //  0001  GETMBR	R2	R0	K20
      0x7C040200,  //  0002  CALL	R1	1
      0x2404030F,  //  0003  GT	R1	R1	K15
      0x78060003,  //  0004  JMPF	R1	#0009
      0x88040114,  //  0005  GETMBR	R1	R0	K20
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
** Solidified function: push_iteration_context
********************************************************************/
be_local_closure(class_engine_proxy_push_iteration_context,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_engine_proxy,     /* shared constants */
    be_str_weak(push_iteration_context),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080114,  //  0000  GETMBR	R2	R0	K20
      0x8C08050A,  //  0001  GETMET	R2	R2	K10
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_animations
********************************************************************/
be_local_closure(class_engine_proxy_get_animations,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_engine_proxy,     /* shared constants */
    be_str_weak(get_animations),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x60040012,  //  0000  GETGBL	R1	G18
      0x7C040000,  //  0001  CALL	R1	0
      0x60080010,  //  0002  GETGBL	R2	G16
      0x880C0108,  //  0003  GETMBR	R3	R0	K8
      0x7C080200,  //  0004  CALL	R2	1
      0xA802000B,  //  0005  EXBLK	0	#0012
      0x5C0C0400,  //  0006  MOVE	R3	R2
      0x7C0C0000,  //  0007  CALL	R3	0
      0x6010000F,  //  0008  GETGBL	R4	G15
      0x5C140600,  //  0009  MOVE	R5	R3
      0xB81A0400,  //  000A  GETNGBL	R6	K2
      0x88180D02,  //  000B  GETMBR	R6	R6	K2
      0x7C100400,  //  000C  CALL	R4	2
      0x78120002,  //  000D  JMPF	R4	#0011
      0x8C10030A,  //  000E  GETMET	R4	R1	K10
      0x5C180600,  //  000F  MOVE	R6	R3
      0x7C100400,  //  0010  CALL	R4	2
      0x7001FFF3,  //  0011  JMP		#0006
      0x58080025,  //  0012  LDCONST	R2	K37
      0xAC080200,  //  0013  CATCH	R2	1	0
      0xB0080000,  //  0014  RAISE	2	R0	R0
      0x80040200,  //  0015  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_engine_proxy_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_engine_proxy,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080526,  //  0003  GETMET	R2	R2	K38
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x88080100,  //  0006  GETMBR	R2	R0	K0
      0x8808051F,  //  0007  GETMBR	R2	R2	K31
      0x90023E02,  //  0008  SETMBR	R0	K31	R2
      0x60080012,  //  0009  GETGBL	R2	G18
      0x7C080000,  //  000A  CALL	R2	0
      0x90021002,  //  000B  SETMBR	R0	K8	R2
      0x60080012,  //  000C  GETGBL	R2	G18
      0x7C080000,  //  000D  CALL	R2	0
      0x90022002,  //  000E  SETMBR	R0	K16	R2
      0x60080012,  //  000F  GETGBL	R2	G18
      0x7C080000,  //  0010  CALL	R2	0
      0x90022402,  //  0011  SETMBR	R0	K18	R2
      0x60080012,  //  0012  GETGBL	R2	G18
      0x7C080000,  //  0013  CALL	R2	0
      0x90022802,  //  0014  SETMBR	R0	K20	R2
      0x90021D0F,  //  0015  SETMBR	R0	K14	K15
      0x8C080127,  //  0016  GETMET	R2	R0	K39
      0x7C080200,  //  0017  CALL	R2	1
      0x80000000,  //  0018  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: engine_proxy
********************************************************************/
extern const bclass be_class_Animation;
be_local_class(engine_proxy,
    7,
    &be_class_Animation,
    be_nested_map(31,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(get_strip_length, -1), be_const_closure(class_engine_proxy_get_strip_length_closure) },
        { be_const_key_weak(remove, -1), be_const_closure(class_engine_proxy_remove_closure) },
        { be_const_key_weak(size_animations, 23), be_const_closure(class_engine_proxy_size_animations_closure) },
        { be_const_key_weak(_add_animation, 24), be_const_closure(class_engine_proxy__add_animation_closure) },
        { be_const_key_weak(start, -1), be_const_closure(class_engine_proxy_start_closure) },
        { be_const_key_weak(value_providers, 19), be_const_var(2) },
        { be_const_key_weak(setup_template, -1), be_const_closure(class_engine_proxy_setup_template_closure) },
        { be_const_key_weak(_remove_animation, -1), be_const_closure(class_engine_proxy__remove_animation_closure) },
        { be_const_key_weak(animations, 6), be_const_var(0) },
        { be_const_key_weak(update_current_iteration, -1), be_const_closure(class_engine_proxy_update_current_iteration_closure) },
        { be_const_key_weak(clear, -1), be_const_closure(class_engine_proxy_clear_closure) },
        { be_const_key_weak(_add_value_provider, 12), be_const_closure(class_engine_proxy__add_value_provider_closure) },
        { be_const_key_weak(stop, -1), be_const_closure(class_engine_proxy_stop_closure) },
        { be_const_key_weak(strip_length, -1), be_const_var(3) },
        { be_const_key_weak(_add_sequence_manager, -1), be_const_closure(class_engine_proxy__add_sequence_manager_closure) },
        { be_const_key_weak(push_iteration_context, 22), be_const_closure(class_engine_proxy_push_iteration_context_closure) },
        { be_const_key_weak(add, -1), be_const_closure(class_engine_proxy_add_closure) },
        { be_const_key_weak(update, -1), be_const_closure(class_engine_proxy_update_closure) },
        { be_const_key_weak(_remove_value_provider, 27), be_const_closure(class_engine_proxy__remove_value_provider_closure) },
        { be_const_key_weak(pop_iteration_context, -1), be_const_closure(class_engine_proxy_pop_iteration_context_closure) },
        { be_const_key_weak(_remove_sequence_manager, -1), be_const_closure(class_engine_proxy__remove_sequence_manager_closure) },
        { be_const_key_weak(is_empty, -1), be_const_closure(class_engine_proxy_is_empty_closure) },
        { be_const_key_weak(sequences, -1), be_const_var(1) },
        { be_const_key_weak(time_ms, 8), be_const_var(6) },
        { be_const_key_weak(_sort_animations_by_priority, -1), be_const_closure(class_engine_proxy__sort_animations_by_priority_closure) },
        { be_const_key_weak(render, 15), be_const_closure(class_engine_proxy_render_closure) },
        { be_const_key_weak(get_current_iteration_number, -1), be_const_closure(class_engine_proxy_get_current_iteration_number_closure) },
        { be_const_key_weak(iteration_stack, -1), be_const_var(5) },
        { be_const_key_weak(get_animations, -1), be_const_closure(class_engine_proxy_get_animations_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_engine_proxy_init_closure) },
        { be_const_key_weak(temp_buffer, -1), be_const_var(4) },
    })),
    be_str_weak(engine_proxy)
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
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(oscillator_value),
    /* K2   */  be_nested_str_weak(form),
    }),
    be_str_weak(ease_out),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x540A0006,  //  0004  LDINT	R2	7
      0x90060402,  //  0005  SETMBR	R1	K2	R2
      0x80040200,  //  0006  RET	1	R1
    })
  )
);
/*******************************************************************/

// compact class 'breathe' ktab size: 13, total: 17 (saved 32 bytes)
static const bvalue be_ktab_class_breathe[13] = {
  /* K0   */  be_nested_str_weak(init),
  /* K1   */  be_nested_str_weak(_breathe),
  /* K2   */  be_nested_str_weak(animation),
  /* K3   */  be_nested_str_weak(breathe_color),
  /* K4   */  be_nested_str_weak(values),
  /* K5   */  be_nested_str_weak(color),
  /* K6   */  be_nested_str_weak(start),
  /* K7   */  be_nested_str_weak(on_param_changed),
  /* K8   */  be_nested_str_weak(min_brightness),
  /* K9   */  be_nested_str_weak(max_brightness),
  /* K10  */  be_nested_str_weak(period),
  /* K11  */  be_nested_str_weak(curve_factor),
  /* K12  */  be_nested_str_weak(set_param),
};


extern const bclass be_class_breathe;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_breathe_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_breathe,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0xB80A0400,  //  0006  GETNGBL	R2	K2
      0x8C080503,  //  0007  GETMET	R2	R2	K3
      0x5C100200,  //  0008  MOVE	R4	R1
      0x7C080400,  //  0009  CALL	R2	2
      0x90020202,  //  000A  SETMBR	R0	K1	R2
      0x88080104,  //  000B  GETMBR	R2	R0	K4
      0x880C0101,  //  000C  GETMBR	R3	R0	K1
      0x980A0A03,  //  000D  SETIDX	R2	K5	R3
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(class_breathe_start,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_breathe,     /* shared constants */
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080506,  //  0003  GETMET	R2	R2	K6
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x88080101,  //  0006  GETMBR	R2	R0	K1
      0x8C080506,  //  0007  GETMET	R2	R2	K6
      0x5C100200,  //  0008  MOVE	R4	R1
      0x7C080400,  //  0009  CALL	R2	2
      0x80040000,  //  000A  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: on_param_changed
********************************************************************/
be_local_closure(class_breathe_on_param_changed,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_breathe,     /* shared constants */
    be_str_weak(on_param_changed),
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0707,  //  0003  GETMET	R3	R3	K7
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x1C0C0305,  //  0007  EQ	R3	R1	K5
      0x780E0005,  //  0008  JMPF	R3	#000F
      0x880C0101,  //  0009  GETMBR	R3	R0	K1
      0x900E0A02,  //  000A  SETMBR	R3	K5	R2
      0x880C0104,  //  000B  GETMBR	R3	R0	K4
      0x88100101,  //  000C  GETMBR	R4	R0	K1
      0x980E0A04,  //  000D  SETIDX	R3	K5	R4
      0x7002000C,  //  000E  JMP		#001C
      0x1C0C0308,  //  000F  EQ	R3	R1	K8
      0x740E0005,  //  0010  JMPT	R3	#0017
      0x1C0C0309,  //  0011  EQ	R3	R1	K9
      0x740E0003,  //  0012  JMPT	R3	#0017
      0x1C0C030A,  //  0013  EQ	R3	R1	K10
      0x740E0001,  //  0014  JMPT	R3	#0017
      0x1C0C030B,  //  0015  EQ	R3	R1	K11
      0x780E0004,  //  0016  JMPF	R3	#001C
      0x880C0101,  //  0017  GETMBR	R3	R0	K1
      0x8C0C070C,  //  0018  GETMET	R3	R3	K12
      0x5C140200,  //  0019  MOVE	R5	R1
      0x5C180400,  //  001A  MOVE	R6	R2
      0x7C0C0600,  //  001B  CALL	R3	3
      0x80000000,  //  001C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: breathe
********************************************************************/
extern const bclass be_class_Animation;
be_local_class(breathe,
    1,
    &be_class_Animation,
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, -1), be_const_closure(class_breathe_init_closure) },
        { be_const_key_weak(start, -1), be_const_closure(class_breathe_start_closure) },
        { be_const_key_weak(on_param_changed, 4), be_const_closure(class_breathe_on_param_changed_closure) },
        { be_const_key_weak(PARAMS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(period, -1), be_const_bytes_instance(05006401B80B) },
        { be_const_key_weak(max_brightness, -1), be_const_bytes_instance(07000001FF0001FF00) },
        { be_const_key_weak(curve_factor, -1), be_const_bytes_instance(07000100050002) },
        { be_const_key_weak(min_brightness, -1), be_const_bytes_instance(07000001FF000000) },
    }))    ) } )) },
        { be_const_key_weak(_breathe, -1), be_const_var(0) },
    })),
    be_str_weak(breathe)
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
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(oscillator_value),
    /* K2   */  be_nested_str_weak(form),
    }),
    be_str_weak(cosine_osc),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x540A0003,  //  0004  LDINT	R2	4
      0x90060402,  //  0005  SETMBR	R1	K2	R2
      0x80040200,  //  0006  RET	1	R1
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


extern const bclass be_class_iteration_number;

/********************************************************************
** Solidified function: produce_value
********************************************************************/
be_local_closure(class_iteration_number_produce_value,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(engine),
    /* K1   */  be_nested_str_weak(get_current_iteration_number),
    }),
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
** Solidified class: iteration_number
********************************************************************/
extern const bclass be_class_parameterized_object;
be_local_class(iteration_number,
    0,
    &be_class_parameterized_object,
    be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(VALUE_PROVIDER, -1), be_const_bool(1) },
        { be_const_key_weak(produce_value, -1), be_const_closure(class_iteration_number_produce_value_closure) },
    })),
    be_str_weak(iteration_number)
);

extern const bclass be_class_beacon;

/********************************************************************
** Solidified function: render
********************************************************************/
be_local_closure(class_beacon_render,   /* name */
  be_nested_proto(
    26,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(member),
    /* K1   */  be_nested_str_weak(back_color),
    /* K2   */  be_nested_str_weak(pos),
    /* K3   */  be_nested_str_weak(slew_size),
    /* K4   */  be_nested_str_weak(beacon_size),
    /* K5   */  be_nested_str_weak(color),
    /* K6   */  be_nested_str_weak(right_edge),
    /* K7   */  be_const_int(-16777216),
    /* K8   */  be_const_int(0),
    /* K9   */  be_nested_str_weak(fill_pixels),
    /* K10  */  be_nested_str_weak(pixels),
    /* K11  */  be_const_int(1),
    /* K12  */  be_nested_str_weak(tasmota),
    /* K13  */  be_nested_str_weak(scale_int),
    /* K14  */  be_nested_str_weak(blend_linear),
    /* K15  */  be_nested_str_weak(set_pixel_color),
    }),
    be_str_weak(render),
    &be_const_str_solidified,
    ( &(const binstruction[125]) {  /* code */
      0x88100100,  //  0000  GETMBR	R4	R0	K0
      0x5C140800,  //  0001  MOVE	R5	R4
      0x5C180000,  //  0002  MOVE	R6	R0
      0x581C0001,  //  0003  LDCONST	R7	K1
      0x7C140400,  //  0004  CALL	R5	2
      0x5C180800,  //  0005  MOVE	R6	R4
      0x5C1C0000,  //  0006  MOVE	R7	R0
      0x58200002,  //  0007  LDCONST	R8	K2
      0x7C180400,  //  0008  CALL	R6	2
      0x5C1C0800,  //  0009  MOVE	R7	R4
      0x5C200000,  //  000A  MOVE	R8	R0
      0x58240003,  //  000B  LDCONST	R9	K3
      0x7C1C0400,  //  000C  CALL	R7	2
      0x5C200800,  //  000D  MOVE	R8	R4
      0x5C240000,  //  000E  MOVE	R9	R0
      0x58280004,  //  000F  LDCONST	R10	K4
      0x7C200400,  //  0010  CALL	R8	2
      0x5C240800,  //  0011  MOVE	R9	R4
      0x5C280000,  //  0012  MOVE	R10	R0
      0x582C0005,  //  0013  LDCONST	R11	K5
      0x7C240400,  //  0014  CALL	R9	2
      0x5C280800,  //  0015  MOVE	R10	R4
      0x5C2C0000,  //  0016  MOVE	R11	R0
      0x58300006,  //  0017  LDCONST	R12	K6
      0x7C280400,  //  0018  CALL	R10	2
      0x202C0B07,  //  0019  NE	R11	R5	K7
      0x782E0006,  //  001A  JMPF	R11	#0022
      0x2C2C0B07,  //  001B  AND	R11	R5	K7
      0x202C1708,  //  001C  NE	R11	R11	K8
      0x782E0003,  //  001D  JMPF	R11	#0022
      0x8C2C0309,  //  001E  GETMET	R11	R1	K9
      0x8834030A,  //  001F  GETMBR	R13	R1	K10
      0x5C380A00,  //  0020  MOVE	R14	R5
      0x7C2C0600,  //  0021  CALL	R11	3
      0x4C2C0000,  //  0022  LDNIL	R11
      0x1C30150B,  //  0023  EQ	R12	R10	K11
      0x78320003,  //  0024  JMPF	R12	#0029
      0x04300C08,  //  0025  SUB	R12	R6	R8
      0x0030190B,  //  0026  ADD	R12	R12	K11
      0x5C2C1800,  //  0027  MOVE	R11	R12
      0x70020000,  //  0028  JMP		#002A
      0x5C2C0C00,  //  0029  MOVE	R11	R6
      0x5C301600,  //  002A  MOVE	R12	R11
      0x00341608,  //  002B  ADD	R13	R11	R8
      0x14381908,  //  002C  LT	R14	R12	K8
      0x783A0000,  //  002D  JMPF	R14	#002F
      0x58300008,  //  002E  LDCONST	R12	K8
      0x28381A03,  //  002F  GE	R14	R13	R3
      0x783A0000,  //  0030  JMPF	R14	#0032
      0x5C340600,  //  0031  MOVE	R13	R3
      0x8C380309,  //  0032  GETMET	R14	R1	K9
      0x8840030A,  //  0033  GETMBR	R16	R1	K10
      0x5C441200,  //  0034  MOVE	R17	R9
      0x5C481800,  //  0035  MOVE	R18	R12
      0x5C4C1A00,  //  0036  MOVE	R19	R13
      0x7C380A00,  //  0037  CALL	R14	5
      0x4C380000,  //  0038  LDNIL	R14
      0x243C0F08,  //  0039  GT	R15	R7	K8
      0x783E003F,  //  003A  JMPF	R15	#007B
      0x043C1607,  //  003B  SUB	R15	R11	R7
      0x5C401600,  //  003C  MOVE	R16	R11
      0x14441F08,  //  003D  LT	R17	R15	K8
      0x78460000,  //  003E  JMPF	R17	#0040
      0x583C0008,  //  003F  LDCONST	R15	K8
      0x28442003,  //  0040  GE	R17	R16	R3
      0x78460000,  //  0041  JMPF	R17	#0043
      0x5C400600,  //  0042  MOVE	R16	R3
      0x5C381E00,  //  0043  MOVE	R14	R15
      0x14441C10,  //  0044  LT	R17	R14	R16
      0x78460013,  //  0045  JMPF	R17	#005A
      0xB8461800,  //  0046  GETNGBL	R17	K12
      0x8C44230D,  //  0047  GETMET	R17	R17	K13
      0x5C4C1C00,  //  0048  MOVE	R19	R14
      0x04501607,  //  0049  SUB	R20	R11	R7
      0x0450290B,  //  004A  SUB	R20	R20	K11
      0x5C541600,  //  004B  MOVE	R21	R11
      0x545A00FE,  //  004C  LDINT	R22	255
      0x585C0008,  //  004D  LDCONST	R23	K8
      0x7C440C00,  //  004E  CALL	R17	6
      0x8C48030E,  //  004F  GETMET	R18	R1	K14
      0x5C500A00,  //  0050  MOVE	R20	R5
      0x5C541200,  //  0051  MOVE	R21	R9
      0x5C582200,  //  0052  MOVE	R22	R17
      0x7C480800,  //  0053  CALL	R18	4
      0x8C4C030F,  //  0054  GETMET	R19	R1	K15
      0x5C541C00,  //  0055  MOVE	R21	R14
      0x5C582400,  //  0056  MOVE	R22	R18
      0x7C4C0600,  //  0057  CALL	R19	3
      0x00381D0B,  //  0058  ADD	R14	R14	K11
      0x7001FFE9,  //  0059  JMP		#0044
      0x00441608,  //  005A  ADD	R17	R11	R8
      0x00481608,  //  005B  ADD	R18	R11	R8
      0x00482407,  //  005C  ADD	R18	R18	R7
      0x144C2308,  //  005D  LT	R19	R17	K8
      0x784E0000,  //  005E  JMPF	R19	#0060
      0x58440008,  //  005F  LDCONST	R17	K8
      0x284C2403,  //  0060  GE	R19	R18	R3
      0x784E0000,  //  0061  JMPF	R19	#0063
      0x5C480600,  //  0062  MOVE	R18	R3
      0x5C382200,  //  0063  MOVE	R14	R17
      0x144C1C12,  //  0064  LT	R19	R14	R18
      0x784E0014,  //  0065  JMPF	R19	#007B
      0xB84E1800,  //  0066  GETNGBL	R19	K12
      0x8C4C270D,  //  0067  GETMET	R19	R19	K13
      0x5C541C00,  //  0068  MOVE	R21	R14
      0x00581608,  //  0069  ADD	R22	R11	R8
      0x04582D0B,  //  006A  SUB	R22	R22	K11
      0x005C1608,  //  006B  ADD	R23	R11	R8
      0x005C2E07,  //  006C  ADD	R23	R23	R7
      0x58600008,  //  006D  LDCONST	R24	K8
      0x546600FE,  //  006E  LDINT	R25	255
      0x7C4C0C00,  //  006F  CALL	R19	6
      0x8C50030E,  //  0070  GETMET	R20	R1	K14
      0x5C580A00,  //  0071  MOVE	R22	R5
      0x5C5C1200,  //  0072  MOVE	R23	R9
      0x5C602600,  //  0073  MOVE	R24	R19
      0x7C500800,  //  0074  CALL	R20	4
      0x8C54030F,  //  0075  GETMET	R21	R1	K15
      0x5C5C1C00,  //  0076  MOVE	R23	R14
      0x5C602800,  //  0077  MOVE	R24	R20
      0x7C540600,  //  0078  CALL	R21	3
      0x00381D0B,  //  0079  ADD	R14	R14	K11
      0x7001FFE8,  //  007A  JMP		#0064
      0x503C0200,  //  007B  LDBOOL	R15	1	0
      0x80041E00,  //  007C  RET	1	R15
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: beacon
********************************************************************/
extern const bclass be_class_Animation;
be_local_class(beacon,
    0,
    &be_class_Animation,
    be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(render, -1), be_const_closure(class_beacon_render_closure) },
        { be_const_key_weak(PARAMS, 0), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(pos, 1), be_const_bytes_instance(040000) },
        { be_const_key_weak(slew_size, 4), be_const_bytes_instance(0500000000) },
        { be_const_key_weak(back_color, 0), be_const_bytes_instance(0402000000FF) },
        { be_const_key_weak(beacon_size, -1), be_const_bytes_instance(0500000001) },
        { be_const_key_weak(right_edge, -1), be_const_bytes_instance(1400000200000001) },
    }))    ) } )) },
    })),
    be_str_weak(beacon)
);

extern const bclass be_class_crenel;

/********************************************************************
** Solidified function: render
********************************************************************/
be_local_closure(class_crenel_render,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(back_color),
    /* K1   */  be_nested_str_weak(pos),
    /* K2   */  be_nested_str_weak(pulse_size),
    /* K3   */  be_nested_str_weak(low_size),
    /* K4   */  be_nested_str_weak(nb_pulse),
    /* K5   */  be_nested_str_weak(color),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(fill_pixels),
    /* K8   */  be_nested_str_weak(pixels),
    /* K9   */  be_const_int(1),
    /* K10  */  be_nested_str_weak(set_pixel_color),
    }),
    be_str_weak(render),
    &be_const_str_solidified,
    ( &(const binstruction[64]) {  /* code */
      0x88100100,  //  0000  GETMBR	R4	R0	K0
      0x88140101,  //  0001  GETMBR	R5	R0	K1
      0x88180102,  //  0002  GETMBR	R6	R0	K2
      0x881C0103,  //  0003  GETMBR	R7	R0	K3
      0x88200104,  //  0004  GETMBR	R8	R0	K4
      0x88240105,  //  0005  GETMBR	R9	R0	K5
      0x60280009,  //  0006  GETGBL	R10	G9
      0x002C0C07,  //  0007  ADD	R11	R6	R7
      0x7C280200,  //  0008  CALL	R10	1
      0x202C0906,  //  0009  NE	R11	R4	K6
      0x782E0003,  //  000A  JMPF	R11	#000F
      0x8C2C0307,  //  000B  GETMET	R11	R1	K7
      0x88340308,  //  000C  GETMBR	R13	R1	K8
      0x5C380800,  //  000D  MOVE	R14	R4
      0x7C2C0600,  //  000E  CALL	R11	3
      0x182C1506,  //  000F  LE	R11	R10	K6
      0x782E0000,  //  0010  JMPF	R11	#0012
      0x58280009,  //  0011  LDCONST	R10	K9
      0x1C2C1106,  //  0012  EQ	R11	R8	K6
      0x782E0001,  //  0013  JMPF	R11	#0016
      0x502C0200,  //  0014  LDBOOL	R11	1	0
      0x80041600,  //  0015  RET	1	R11
      0x142C1106,  //  0016  LT	R11	R8	K6
      0x782E0006,  //  0017  JMPF	R11	#001F
      0x002C0A06,  //  0018  ADD	R11	R5	R6
      0x042C1709,  //  0019  SUB	R11	R11	K9
      0x102C160A,  //  001A  MOD	R11	R11	R10
      0x042C1606,  //  001B  SUB	R11	R11	R6
      0x002C1709,  //  001C  ADD	R11	R11	K9
      0x5C141600,  //  001D  MOVE	R5	R11
      0x70020007,  //  001E  JMP		#0027
      0x442C1400,  //  001F  NEG	R11	R10
      0x142C0A0B,  //  0020  LT	R11	R5	R11
      0x782E0004,  //  0021  JMPF	R11	#0027
      0x202C1106,  //  0022  NE	R11	R8	K6
      0x782E0002,  //  0023  JMPF	R11	#0027
      0x00140A0A,  //  0024  ADD	R5	R5	R10
      0x04201109,  //  0025  SUB	R8	R8	K9
      0x7001FFF7,  //  0026  JMP		#001F
      0x142C0A03,  //  0027  LT	R11	R5	R3
      0x782E0014,  //  0028  JMPF	R11	#003E
      0x202C1106,  //  0029  NE	R11	R8	K6
      0x782E0012,  //  002A  JMPF	R11	#003E
      0x582C0006,  //  002B  LDCONST	R11	K6
      0x14300B06,  //  002C  LT	R12	R5	K6
      0x78320001,  //  002D  JMPF	R12	#0030
      0x44300A00,  //  002E  NEG	R12	R5
      0x5C2C1800,  //  002F  MOVE	R11	R12
      0x14301606,  //  0030  LT	R12	R11	R6
      0x78320008,  //  0031  JMPF	R12	#003B
      0x00300A0B,  //  0032  ADD	R12	R5	R11
      0x14301803,  //  0033  LT	R12	R12	R3
      0x78320005,  //  0034  JMPF	R12	#003B
      0x8C30030A,  //  0035  GETMET	R12	R1	K10
      0x00380A0B,  //  0036  ADD	R14	R5	R11
      0x5C3C1200,  //  0037  MOVE	R15	R9
      0x7C300600,  //  0038  CALL	R12	3
      0x002C1709,  //  0039  ADD	R11	R11	K9
      0x7001FFF4,  //  003A  JMP		#0030
      0x00140A0A,  //  003B  ADD	R5	R5	R10
      0x04201109,  //  003C  SUB	R8	R8	K9
      0x7001FFE8,  //  003D  JMP		#0027
      0x502C0200,  //  003E  LDBOOL	R11	1	0
      0x80041600,  //  003F  RET	1	R11
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: crenel
********************************************************************/
extern const bclass be_class_Animation;
be_local_class(crenel,
    0,
    &be_class_Animation,
    be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(render, -1), be_const_closure(class_crenel_render_closure) },
        { be_const_key_weak(PARAMS, 0), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(nb_pulse, -1), be_const_bytes_instance(0400FF) },
        { be_const_key_weak(low_size, 4), be_const_bytes_instance(0500000003) },
        { be_const_key_weak(pos, 1), be_const_bytes_instance(040000) },
        { be_const_key_weak(pulse_size, -1), be_const_bytes_instance(0500000001) },
        { be_const_key_weak(back_color, -1), be_const_bytes_instance(040000) },
    }))    ) } )) },
    })),
    be_str_weak(crenel)
);
// compact class 'closure_value' ktab size: 4, total: 5 (saved 8 bytes)
static const bvalue be_ktab_class_closure_value[4] = {
  /* K0   */  be_nested_str_weak(_closure),
  /* K1   */  be_nested_str_weak(engine),
  /* K2   */  be_nested_str_weak(on_param_changed),
  /* K3   */  be_nested_str_weak(closure),
};


extern const bclass be_class_closure_value;

/********************************************************************
** Solidified function: produce_value
********************************************************************/
be_local_closure(class_closure_value_produce_value,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_closure_value,     /* shared constants */
    be_str_weak(produce_value),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x4C100000,  //  0001  LDNIL	R4
      0x1C100604,  //  0002  EQ	R4	R3	R4
      0x78120001,  //  0003  JMPF	R4	#0006
      0x4C100000,  //  0004  LDNIL	R4
      0x80040800,  //  0005  RET	1	R4
      0x5C100600,  //  0006  MOVE	R4	R3
      0x88140101,  //  0007  GETMBR	R5	R0	K1
      0x5C180200,  //  0008  MOVE	R6	R1
      0x5C1C0400,  //  0009  MOVE	R7	R2
      0x7C100600,  //  000A  CALL	R4	3
      0x80040800,  //  000B  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: on_param_changed
********************************************************************/
be_local_closure(class_closure_value_on_param_changed,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_closure_value,     /* shared constants */
    be_str_weak(on_param_changed),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0702,  //  0003  GETMET	R3	R3	K2
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x1C0C0303,  //  0007  EQ	R3	R1	K3
      0x780E0000,  //  0008  JMPF	R3	#000A
      0x90020002,  //  0009  SETMBR	R0	K0	R2
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: closure_value
********************************************************************/
extern const bclass be_class_parameterized_object;
be_local_class(closure_value,
    1,
    &be_class_parameterized_object,
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(on_param_changed, 4), be_const_closure(class_closure_value_on_param_changed_closure) },
        { be_const_key_weak(PARAMS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(closure, -1), be_const_bytes_instance(0C0606) },
    }))    ) } )) },
        { be_const_key_weak(_closure, 0), be_const_var(0) },
        { be_const_key_weak(produce_value, 1), be_const_closure(class_closure_value_produce_value_closure) },
        { be_const_key_weak(VALUE_PROVIDER, -1), be_const_bool(1) },
    })),
    be_str_weak(closure_value)
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
    /* K3   */  be_const_int(1),
    }),
    be_str_weak(sawtooth),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x90060503,  //  0004  SETMBR	R1	K2	K3
      0x80040200,  //  0005  RET	1	R1
    })
  )
);
/*******************************************************************/

// compact class 'color_cycle' ktab size: 24, total: 48 (saved 192 bytes)
static const bvalue be_ktab_class_color_cycle[24] = {
  /* K0   */  be_nested_str_weak(colors),
  /* K1   */  be_const_int(0),
  /* K2   */  be_nested_str_weak(get),
  /* K3   */  be_const_int(-16777216),
  /* K4   */  be_nested_str_weak(palette_size),
  /* K5   */  be_nested_str_weak(_get_palette_size),
  /* K6   */  be_nested_str_weak(member),
  /* K7   */  be_nested_str_weak(animation),
  /* K8   */  be_nested_str_weak(PALETTE_RAINBOW),
  /* K9   */  be_nested_str_weak(period),
  /* K10  */  be_const_int(1),
  /* K11  */  be_nested_str_weak(current_index),
  /* K12  */  be_nested_str_weak(_get_color_at_index),
  /* K13  */  be_nested_str_weak(brightness),
  /* K14  */  be_nested_str_weak(apply_brightness),
  /* K15  */  be_nested_str_weak(tasmota),
  /* K16  */  be_nested_str_weak(scale_uint),
  /* K17  */  be_nested_str_weak(init),
  /* K18  */  be_nested_str_weak(values),
  /* K19  */  be_nested_str_weak(on_param_changed),
  /* K20  */  be_nested_str_weak(value_error),
  /* K21  */  be_nested_str_weak(Parameter_X20_X27palette_size_X27_X20is_X20read_X2Donly),
  /* K22  */  be_nested_str_weak(next),
  /* K23  */  be_nested_str_weak(_adjust_index),
};


extern const bclass be_class_color_cycle;

/********************************************************************
** Solidified function: _get_color_at_index
********************************************************************/
be_local_closure(class_color_cycle__get_color_at_index,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_color_cycle,     /* shared constants */
    be_str_weak(_get_color_at_index),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x600C000C,  //  0001  GETGBL	R3	G12
      0x5C100400,  //  0002  MOVE	R4	R2
      0x7C0C0200,  //  0003  CALL	R3	1
      0x54120003,  //  0004  LDINT	R4	4
      0x0C0C0604,  //  0005  DIV	R3	R3	R4
      0x1C100701,  //  0006  EQ	R4	R3	K1
      0x74120003,  //  0007  JMPT	R4	#000C
      0x28100203,  //  0008  GE	R4	R1	R3
      0x74120001,  //  0009  JMPT	R4	#000C
      0x14100301,  //  000A  LT	R4	R1	K1
      0x78120000,  //  000B  JMPF	R4	#000D
      0x80060200,  //  000C  RET	1	K1
      0x8C100502,  //  000D  GETMET	R4	R2	K2
      0x541A0003,  //  000E  LDINT	R6	4
      0x08180206,  //  000F  MUL	R6	R1	R6
      0x541DFFFB,  //  0010  LDINT	R7	-4
      0x7C100600,  //  0011  CALL	R4	3
      0x30100903,  //  0012  OR	R4	R4	K3
      0x80040800,  //  0013  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: member
********************************************************************/
be_local_closure(class_color_cycle_member,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_color_cycle,     /* shared constants */
    be_str_weak(member),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x1C080304,  //  0000  EQ	R2	R1	K4
      0x780A0003,  //  0001  JMPF	R2	#0006
      0x8C080105,  //  0002  GETMET	R2	R0	K5
      0x7C080200,  //  0003  CALL	R2	1
      0x80040400,  //  0004  RET	1	R2
      0x7002000D,  //  0005  JMP		#0014
      0x60080003,  //  0006  GETGBL	R2	G3
      0x5C0C0000,  //  0007  MOVE	R3	R0
      0x7C080200,  //  0008  CALL	R2	1
      0x8C080506,  //  0009  GETMET	R2	R2	K6
      0x5C100200,  //  000A  MOVE	R4	R1
      0x7C080400,  //  000B  CALL	R2	2
      0x1C0C0300,  //  000C  EQ	R3	R1	K0
      0x780E0004,  //  000D  JMPF	R3	#0013
      0x4C0C0000,  //  000E  LDNIL	R3
      0x1C0C0403,  //  000F  EQ	R3	R2	R3
      0x780E0001,  //  0010  JMPF	R3	#0013
      0xB80E0E00,  //  0011  GETNGBL	R3	K7
      0x88080708,  //  0012  GETMBR	R2	R3	K8
      0x80040400,  //  0013  RET	1	R2
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: produce_value
********************************************************************/
be_local_closure(class_color_cycle_produce_value,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_color_cycle,     /* shared constants */
    be_str_weak(produce_value),
    &be_const_str_solidified,
    ( &(const binstruction[56]) {  /* code */
      0x880C0109,  //  0000  GETMBR	R3	R0	K9
      0x8C100105,  //  0001  GETMET	R4	R0	K5
      0x7C100200,  //  0002  CALL	R4	1
      0x1814090A,  //  0003  LE	R5	R4	K10
      0x74160001,  //  0004  JMPT	R5	#0007
      0x1C140701,  //  0005  EQ	R5	R3	K1
      0x78160015,  //  0006  JMPF	R5	#001D
      0x8814010B,  //  0007  GETMBR	R5	R0	K11
      0x28180A04,  //  0008  GE	R6	R5	R4
      0x781A0001,  //  0009  JMPF	R6	#000C
      0x0418090A,  //  000A  SUB	R6	R4	K10
      0x5C140C00,  //  000B  MOVE	R5	R6
      0x14180B01,  //  000C  LT	R6	R5	K1
      0x781A0000,  //  000D  JMPF	R6	#000F
      0x58140001,  //  000E  LDCONST	R5	K1
      0x90021605,  //  000F  SETMBR	R0	K11	R5
      0x8C18010C,  //  0010  GETMET	R6	R0	K12
      0x8820010B,  //  0011  GETMBR	R8	R0	K11
      0x7C180400,  //  0012  CALL	R6	2
      0x881C010D,  //  0013  GETMBR	R7	R0	K13
      0x542200FE,  //  0014  LDINT	R8	255
      0x20200E08,  //  0015  NE	R8	R7	R8
      0x78220004,  //  0016  JMPF	R8	#001C
      0x8C20010E,  //  0017  GETMET	R8	R0	K14
      0x5C280C00,  //  0018  MOVE	R10	R6
      0x5C2C0E00,  //  0019  MOVE	R11	R7
      0x7C200600,  //  001A  CALL	R8	3
      0x80041000,  //  001B  RET	1	R8
      0x80040C00,  //  001C  RET	1	R6
      0x10140403,  //  001D  MOD	R5	R2	R3
      0xB81A1E00,  //  001E  GETNGBL	R6	K15
      0x8C180D10,  //  001F  GETMET	R6	R6	K16
      0x5C200A00,  //  0020  MOVE	R8	R5
      0x58240001,  //  0021  LDCONST	R9	K1
      0x0428070A,  //  0022  SUB	R10	R3	K10
      0x582C0001,  //  0023  LDCONST	R11	K1
      0x0430090A,  //  0024  SUB	R12	R4	K10
      0x7C180C00,  //  0025  CALL	R6	6
      0x281C0C04,  //  0026  GE	R7	R6	R4
      0x781E0001,  //  0027  JMPF	R7	#002A
      0x041C090A,  //  0028  SUB	R7	R4	K10
      0x5C180E00,  //  0029  MOVE	R6	R7
      0x90021606,  //  002A  SETMBR	R0	K11	R6
      0x8C1C010C,  //  002B  GETMET	R7	R0	K12
      0x5C240C00,  //  002C  MOVE	R9	R6
      0x7C1C0400,  //  002D  CALL	R7	2
      0x8820010D,  //  002E  GETMBR	R8	R0	K13
      0x542600FE,  //  002F  LDINT	R9	255
      0x20241009,  //  0030  NE	R9	R8	R9
      0x78260004,  //  0031  JMPF	R9	#0037
      0x8C24010E,  //  0032  GETMET	R9	R0	K14
      0x5C2C0E00,  //  0033  MOVE	R11	R7
      0x5C301000,  //  0034  MOVE	R12	R8
      0x7C240600,  //  0035  CALL	R9	3
      0x80041200,  //  0036  RET	1	R9
      0x80040E00,  //  0037  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _adjust_index
********************************************************************/
be_local_closure(class_color_cycle__adjust_index,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_color_cycle,     /* shared constants */
    be_str_weak(_adjust_index),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x8C040105,  //  0000  GETMET	R1	R0	K5
      0x7C040200,  //  0001  CALL	R1	1
      0x24080301,  //  0002  GT	R2	R1	K1
      0x780A0009,  //  0003  JMPF	R2	#000E
      0x8808010B,  //  0004  GETMBR	R2	R0	K11
      0x10080401,  //  0005  MOD	R2	R2	R1
      0x140C0501,  //  0006  LT	R3	R2	K1
      0x780E0000,  //  0007  JMPF	R3	#0009
      0x00080401,  //  0008  ADD	R2	R2	R1
      0x880C010B,  //  0009  GETMBR	R3	R0	K11
      0x200C0602,  //  000A  NE	R3	R3	R2
      0x780E0000,  //  000B  JMPF	R3	#000D
      0x90021602,  //  000C  SETMBR	R0	K11	R2
      0x70020000,  //  000D  JMP		#000F
      0x90021701,  //  000E  SETMBR	R0	K11	K1
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_color_cycle_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_color_cycle,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080511,  //  0003  GETMET	R2	R2	K17
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x90021701,  //  0006  SETMBR	R0	K11	K1
      0x88080112,  //  0007  GETMBR	R2	R0	K18
      0x8C0C0105,  //  0008  GETMET	R3	R0	K5
      0x7C0C0200,  //  0009  CALL	R3	1
      0x980A0803,  //  000A  SETIDX	R2	K4	R3
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: on_param_changed
********************************************************************/
be_local_closure(class_color_cycle_on_param_changed,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_color_cycle,     /* shared constants */
    be_str_weak(on_param_changed),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0713,  //  0003  GETMET	R3	R3	K19
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x1C0C0304,  //  0007  EQ	R3	R1	K4
      0x780E0005,  //  0008  JMPF	R3	#000F
      0x880C0112,  //  0009  GETMBR	R3	R0	K18
      0x8C100105,  //  000A  GETMET	R4	R0	K5
      0x7C100200,  //  000B  CALL	R4	1
      0x980E0804,  //  000C  SETIDX	R3	K4	R4
      0xB0062915,  //  000D  RAISE	1	K20	K21
      0x7002000A,  //  000E  JMP		#001A
      0x1C0C0316,  //  000F  EQ	R3	R1	K22
      0x780E0008,  //  0010  JMPF	R3	#001A
      0x200C0501,  //  0011  NE	R3	R2	K1
      0x780E0006,  //  0012  JMPF	R3	#001A
      0x880C010B,  //  0013  GETMBR	R3	R0	K11
      0x000C0602,  //  0014  ADD	R3	R3	R2
      0x90021603,  //  0015  SETMBR	R0	K11	R3
      0x8C0C0117,  //  0016  GETMET	R3	R0	K23
      0x7C0C0200,  //  0017  CALL	R3	1
      0x880C0112,  //  0018  GETMBR	R3	R0	K18
      0x980E2D01,  //  0019  SETIDX	R3	K22	K1
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _get_palette_size
********************************************************************/
be_local_closure(class_color_cycle__get_palette_size,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_color_cycle,     /* shared constants */
    be_str_weak(_get_palette_size),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x6004000C,  //  0000  GETGBL	R1	G12
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x7C040200,  //  0002  CALL	R1	1
      0x540A0003,  //  0003  LDINT	R2	4
      0x0C040202,  //  0004  DIV	R1	R1	R2
      0x80040200,  //  0005  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_color_for_value
********************************************************************/
be_local_closure(class_color_cycle_get_color_for_value,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_color_cycle,     /* shared constants */
    be_str_weak(get_color_for_value),
    &be_const_str_solidified,
    ( &(const binstruction[53]) {  /* code */
      0x8C0C0105,  //  0000  GETMET	R3	R0	K5
      0x7C0C0200,  //  0001  CALL	R3	1
      0x1C100701,  //  0002  EQ	R4	R3	K1
      0x78120000,  //  0003  JMPF	R4	#0005
      0x80060200,  //  0004  RET	1	K1
      0x1C10070A,  //  0005  EQ	R4	R3	K10
      0x7812000C,  //  0006  JMPF	R4	#0014
      0x8C10010C,  //  0007  GETMET	R4	R0	K12
      0x58180001,  //  0008  LDCONST	R6	K1
      0x7C100400,  //  0009  CALL	R4	2
      0x8814010D,  //  000A  GETMBR	R5	R0	K13
      0x541A00FE,  //  000B  LDINT	R6	255
      0x20180A06,  //  000C  NE	R6	R5	R6
      0x781A0004,  //  000D  JMPF	R6	#0013
      0x8C18010E,  //  000E  GETMET	R6	R0	K14
      0x5C200800,  //  000F  MOVE	R8	R4
      0x5C240A00,  //  0010  MOVE	R9	R5
      0x7C180600,  //  0011  CALL	R6	3
      0x80040C00,  //  0012  RET	1	R6
      0x80040800,  //  0013  RET	1	R4
      0x14100301,  //  0014  LT	R4	R1	K1
      0x78120001,  //  0015  JMPF	R4	#0018
      0x58040001,  //  0016  LDCONST	R1	K1
      0x70020003,  //  0017  JMP		#001C
      0x541200FE,  //  0018  LDINT	R4	255
      0x24100204,  //  0019  GT	R4	R1	R4
      0x78120000,  //  001A  JMPF	R4	#001C
      0x540600FE,  //  001B  LDINT	R1	255
      0xB8121E00,  //  001C  GETNGBL	R4	K15
      0x8C100910,  //  001D  GETMET	R4	R4	K16
      0x5C180200,  //  001E  MOVE	R6	R1
      0x581C0001,  //  001F  LDCONST	R7	K1
      0x542200FE,  //  0020  LDINT	R8	255
      0x58240001,  //  0021  LDCONST	R9	K1
      0x0428070A,  //  0022  SUB	R10	R3	K10
      0x7C100C00,  //  0023  CALL	R4	6
      0x28140803,  //  0024  GE	R5	R4	R3
      0x78160001,  //  0025  JMPF	R5	#0028
      0x0414070A,  //  0026  SUB	R5	R3	K10
      0x5C100A00,  //  0027  MOVE	R4	R5
      0x8C14010C,  //  0028  GETMET	R5	R0	K12
      0x5C1C0800,  //  0029  MOVE	R7	R4
      0x7C140400,  //  002A  CALL	R5	2
      0x8818010D,  //  002B  GETMBR	R6	R0	K13
      0x541E00FE,  //  002C  LDINT	R7	255
      0x201C0C07,  //  002D  NE	R7	R6	R7
      0x781E0004,  //  002E  JMPF	R7	#0034
      0x8C1C010E,  //  002F  GETMET	R7	R0	K14
      0x5C240A00,  //  0030  MOVE	R9	R5
      0x5C280C00,  //  0031  MOVE	R10	R6
      0x7C1C0600,  //  0032  CALL	R7	3
      0x80040E00,  //  0033  RET	1	R7
      0x80040A00,  //  0034  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: color_cycle
********************************************************************/
extern const bclass be_class_color_provider;
be_local_class(color_cycle,
    1,
    &be_class_color_provider,
    be_nested_map(10,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(produce_value, -1), be_const_closure(class_color_cycle_produce_value_closure) },
        { be_const_key_weak(member, -1), be_const_closure(class_color_cycle_member_closure) },
        { be_const_key_weak(_get_palette_size, -1), be_const_closure(class_color_cycle__get_palette_size_closure) },
        { be_const_key_weak(_get_color_at_index, 8), be_const_closure(class_color_cycle__get_color_at_index_closure) },
        { be_const_key_weak(_adjust_index, 2), be_const_closure(class_color_cycle__adjust_index_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_color_cycle_init_closure) },
        { be_const_key_weak(on_param_changed, -1), be_const_closure(class_color_cycle_on_param_changed_closure) },
        { be_const_key_weak(current_index, 6), be_const_var(0) },
        { be_const_key_weak(PARAMS, 0), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(period, 2), be_const_bytes_instance(050000018813) },
        { be_const_key_weak(colors, -1), be_const_bytes_instance(0C0602) },
        { be_const_key_weak(palette_size, 3), be_const_bytes_instance(0C000300) },
        { be_const_key_weak(next, -1), be_const_bytes_instance(040000) },
    }))    ) } )) },
        { be_const_key_weak(get_color_for_value, -1), be_const_closure(class_color_cycle_get_color_for_value_closure) },
    })),
    be_str_weak(color_cycle)
);

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


/********************************************************************
** Solidified module: animation
********************************************************************/
be_local_module(animation,
    "animation",
    be_nested_map(75,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(crenel, -1), be_const_class(be_class_crenel) },
        { be_const_key_weak(VERSION, -1), be_const_int(65536) },
        { be_const_key_weak(engine_proxy, -1), be_const_class(be_class_engine_proxy) },
        { be_const_key_weak(sine_osc, -1), be_const_closure(sine_osc_closure) },
        { be_const_key_weak(get_registered_events, -1), be_const_closure(get_registered_events_closure) },
        { be_const_key_weak(register_user_function, -1), be_const_closure(register_user_function_closure) },
        { be_const_key_weak(SAWTOOTH, 10), be_const_int(1) },
        { be_const_key_weak(PALETTE_RAINBOW_W, 68), be_const_bytes_instance(FFFC0000FFFF8000FFFFFF00FF00FF00FF00FFFFFF0080FFFF8000FFFFCCCCCC) },
        { be_const_key_weak(EventManager, -1), be_const_class(be_class_EventManager) },
        { be_const_key_weak(ramp, -1), be_const_closure(ramp_closure) },
        { be_const_key_weak(BOUNCE, -1), be_const_int(9) },
        { be_const_key_weak(sawtooth, -1), be_const_closure(sawtooth_closure) },
        { be_const_key_weak(init_strip, 15), be_const_closure(animation_init_strip_closure) },
        { be_const_key_weak(linear, -1), be_const_closure(linear_closure) },
        { be_const_key_weak(create_engine, 53), be_const_class(be_class_AnimationEngine) },
        { be_const_key_weak(sequence_manager, -1), be_const_class(be_class_sequence_manager) },
        { be_const_key_weak(ease_in, -1), be_const_closure(ease_in_closure) },
        { be_const_key_weak(clear_all_event_handlers, -1), be_const_closure(clear_all_event_handlers_closure) },
        { be_const_key_weak(oscillator_value, -1), be_const_class(be_class_oscillator_value) },
        { be_const_key_weak(gradient, -1), be_const_class(be_class_gradient) },
        { be_const_key_weak(cosine_osc, -1), be_const_closure(cosine_osc_closure) },
        { be_const_key_weak(PALETTE_RGB, -1), be_const_bytes_instance(FFFF0000FF00FF00FF0000FF) },
        { be_const_key_weak(comet, -1), be_const_class(be_class_comet) },
        { be_const_key_weak(breathe_color, 9), be_const_class(be_class_breathe_color) },
        { be_const_key_weak(ease_out, 16), be_const_closure(ease_out_closure) },
        { be_const_key_weak(is_value_provider, 51), be_const_closure(is_value_provider_closure) },
        { be_const_key_weak(is_color_provider, 70), be_const_closure(is_color_provider_closure) },
        { be_const_key_weak(animation, -1), be_const_class(be_class_Animation) },
        { be_const_key_weak(enc_params, 25), be_const_closure(encode_constraints_closure) },
        { be_const_key_weak(parameterized_object, -1), be_const_class(be_class_parameterized_object) },
        { be_const_key_weak(init, -1), be_const_closure(animation_init_closure) },
        { be_const_key_weak(static_value, 52), be_const_class(be_class_static_value) },
        { be_const_key_weak(color_cycle, 5), be_const_class(be_class_color_cycle) },
        { be_const_key_weak(EASE_IN, 36), be_const_int(6) },
        { be_const_key_weak(list_user_functions, 21), be_const_closure(list_user_functions_closure) },
        { be_const_key_weak(palette_gradient, -1), be_const_class(be_class_palette_gradient) },
        { be_const_key_weak(TRIANGLE, -1), be_const_int(2) },
        { be_const_key_weak(PALETTE_RAINBOW_W2, 14), be_const_bytes_instance(FFFC0000FFFF8000FFFFFF00FF00FF00FF00FFFFFF0080FFFF8000FFFFCCCCCCFFFC0000) },
        { be_const_key_weak(PALETTE_RAINBOW2, 35), be_const_bytes_instance(FFFC0000FFFF8000FFFFFF00FF00FF00FF00FFFFFF0080FFFF8000FFFFFC0000) },
        { be_const_key_weak(COSINE, -1), be_const_int(4) },
        { be_const_key_weak(triangle, -1), be_const_closure(triangle_closure) },
        { be_const_key_weak(elastic, -1), be_const_closure(elastic_closure) },
        { be_const_key_weak(closure_value, -1), be_const_class(be_class_closure_value) },
        { be_const_key_weak(smooth, -1), be_const_closure(smooth_closure) },
        { be_const_key_weak(beacon, 11), be_const_class(be_class_beacon) },
        { be_const_key_weak(SINE, 41), be_const_int(5) },
        { be_const_key_weak(iteration_number, -1), be_const_class(be_class_iteration_number) },
        { be_const_key_weak(is_user_function, -1), be_const_closure(is_user_function_closure) },
        { be_const_key_weak(PALETTE_RAINBOW, -1), be_const_bytes_instance(FFFC0000FFFF8000FFFFFF00FF00FF00FF00FFFFFF0080FFFF8000FF) },
        { be_const_key_weak(resolve, -1), be_const_closure(animation_resolve_closure) },
        { be_const_key_weak(frame_buffer, 20), be_const_class(be_class_frame_buffer) },
        { be_const_key_weak(bounce, -1), be_const_closure(bounce_closure) },
        { be_const_key_weak(palette_meter, -1), be_const_class(be_class_palette_meter) },
        { be_const_key_weak(solid, -1), be_const_closure(solid_closure) },
        { be_const_key_weak(register_event_handler, -1), be_const_closure(register_event_handler_closure) },
        { be_const_key_weak(EASE_OUT, 27), be_const_int(7) },
        { be_const_key_weak(strip_length, -1), be_const_class(be_class_strip_length) },
        { be_const_key_weak(trigger_event, 17), be_const_closure(trigger_event_closure) },
        { be_const_key_weak(create_closure_value, -1), be_const_closure(create_closure_value_closure) },
        { be_const_key_weak(_math, -1), be_const_class(be_class_AnimationMath) },
        { be_const_key_weak(square, 24), be_const_closure(square_closure) },
        { be_const_key_weak(rich_palette_color, 34), be_const_class(be_class_rich_palette_color) },
        { be_const_key_weak(get_user_function, -1), be_const_closure(get_user_function_closure) },
        { be_const_key_weak(breathe, -1), be_const_class(be_class_breathe) },
        { be_const_key_weak(get_event_handlers, 74), be_const_closure(get_event_handlers_closure) },
        { be_const_key_weak(set_event_active, -1), be_const_closure(set_event_active_closure) },
        { be_const_key_weak(event_handler, -1), be_const_class(be_class_EventHandler) },
        { be_const_key_weak(twinkle, -1), be_const_class(be_class_twinkle) },
        { be_const_key_weak(ELASTIC, 71), be_const_int(8) },
        { be_const_key_weak(rich_palette, 64), be_const_class(be_class_rich_palette) },
        { be_const_key_weak(SQUARE, -1), be_const_int(3) },
        { be_const_key_weak(color_provider, -1), be_const_class(be_class_color_provider) },
        { be_const_key_weak(LINEAR, 69), be_const_int(1) },
        { be_const_key_weak(PALETTE_FIRE, -1), be_const_bytes_instance(FF000000FF800000FFFF0000FFFF8000FFFFFF00) },
        { be_const_key_weak(unregister_event_handler, -1), be_const_closure(unregister_event_handler_closure) },
    }))
);
BE_EXPORT_VARIABLE be_define_const_native_module(animation);
/********************************************************************/
/********************************************************************/
/* End of solidification */
