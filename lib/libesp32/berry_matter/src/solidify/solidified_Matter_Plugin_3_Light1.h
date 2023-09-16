/* Solidification of Matter_Plugin_3_Light1.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Light1;

/********************************************************************
** Solidified function: set_bri
********************************************************************/
be_local_closure(Matter_Plugin_Light1_set_bri,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(VIRTUAL),
    /* K2   */  be_nested_str_weak(light),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(scale_uint),
    /* K5   */  be_nested_str_weak(set),
    /* K6   */  be_nested_str_weak(bri),
    /* K7   */  be_nested_str_weak(power),
    /* K8   */  be_nested_str_weak(update_shadow),
    /* K9   */  be_nested_str_weak(shadow_onoff),
    /* K10  */  be_nested_str_weak(attribute_updated),
    /* K11  */  be_nested_str_weak(shadow_bri),
    }),
    be_str_weak(set_bri),
    &be_const_str_solidified,
    ( &(const binstruction[65]) {  /* code */
      0x140C0300,  //  0000  LT	R3	R1	K0
      0x780E0000,  //  0001  JMPF	R3	#0003
      0x58040000,  //  0002  LDCONST	R1	K0
      0x540E00FD,  //  0003  LDINT	R3	254
      0x240C0203,  //  0004  GT	R3	R1	R3
      0x780E0000,  //  0005  JMPF	R3	#0007
      0x540600FD,  //  0006  LDINT	R1	254
      0x4C0C0000,  //  0007  LDNIL	R3
      0x200C0403,  //  0008  NE	R3	R2	R3
      0x780E0003,  //  0009  JMPF	R3	#000E
      0x600C0017,  //  000A  GETGBL	R3	G23
      0x5C100400,  //  000B  MOVE	R4	R2
      0x7C0C0200,  //  000C  CALL	R3	1
      0x70020000,  //  000D  JMP		#000F
      0x4C0C0000,  //  000E  LDNIL	R3
      0x5C080600,  //  000F  MOVE	R2	R3
      0x880C0101,  //  0010  GETMBR	R3	R0	K1
      0x740E001A,  //  0011  JMPT	R3	#002D
      0xA40E0400,  //  0012  IMPORT	R3	K2
      0xB8120600,  //  0013  GETNGBL	R4	K3
      0x8C100904,  //  0014  GETMET	R4	R4	K4
      0x5C180200,  //  0015  MOVE	R6	R1
      0x581C0000,  //  0016  LDCONST	R7	K0
      0x542200FD,  //  0017  LDINT	R8	254
      0x58240000,  //  0018  LDCONST	R9	K0
      0x542A00FE,  //  0019  LDINT	R10	255
      0x7C100C00,  //  001A  CALL	R4	6
      0x4C140000,  //  001B  LDNIL	R5
      0x1C140405,  //  001C  EQ	R5	R2	R5
      0x78160005,  //  001D  JMPF	R5	#0024
      0x8C140705,  //  001E  GETMET	R5	R3	K5
      0x601C0013,  //  001F  GETGBL	R7	G19
      0x7C1C0000,  //  0020  CALL	R7	0
      0x981E0C04,  //  0021  SETIDX	R7	K6	R4
      0x7C140400,  //  0022  CALL	R5	2
      0x70020005,  //  0023  JMP		#002A
      0x8C140705,  //  0024  GETMET	R5	R3	K5
      0x601C0013,  //  0025  GETGBL	R7	G19
      0x7C1C0000,  //  0026  CALL	R7	0
      0x981E0C04,  //  0027  SETIDX	R7	K6	R4
      0x981E0E02,  //  0028  SETIDX	R7	K7	R2
      0x7C140400,  //  0029  CALL	R5	2
      0x8C140108,  //  002A  GETMET	R5	R0	K8
      0x7C140200,  //  002B  CALL	R5	1
      0x70020012,  //  002C  JMP		#0040
      0x4C0C0000,  //  002D  LDNIL	R3
      0x200C0403,  //  002E  NE	R3	R2	R3
      0x780E0007,  //  002F  JMPF	R3	#0038
      0x880C0109,  //  0030  GETMBR	R3	R0	K9
      0x200C0403,  //  0031  NE	R3	R2	R3
      0x780E0004,  //  0032  JMPF	R3	#0038
      0x8C0C010A,  //  0033  GETMET	R3	R0	K10
      0x54160005,  //  0034  LDINT	R5	6
      0x58180000,  //  0035  LDCONST	R6	K0
      0x7C0C0600,  //  0036  CALL	R3	3
      0x90021202,  //  0037  SETMBR	R0	K9	R2
      0x880C010B,  //  0038  GETMBR	R3	R0	K11
      0x200C0203,  //  0039  NE	R3	R1	R3
      0x780E0004,  //  003A  JMPF	R3	#0040
      0x8C0C010A,  //  003B  GETMET	R3	R0	K10
      0x54160007,  //  003C  LDINT	R5	8
      0x58180000,  //  003D  LDCONST	R6	K0
      0x7C0C0600,  //  003E  CALL	R3	3
      0x90021601,  //  003F  SETMBR	R0	K11	R1
      0x80000000,  //  0040  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_Light1_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(shadow_bri),
    /* K2   */  be_const_int(0),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x60100003,  //  0000  GETGBL	R4	G3
      0x5C140000,  //  0001  MOVE	R5	R0
      0x7C100200,  //  0002  CALL	R4	1
      0x8C100900,  //  0003  GETMET	R4	R4	K0
      0x5C180200,  //  0004  MOVE	R6	R1
      0x5C1C0400,  //  0005  MOVE	R7	R2
      0x5C200600,  //  0006  MOVE	R8	R3
      0x7C100800,  //  0007  CALL	R4	4
      0x90020302,  //  0008  SETMBR	R0	K1	K2
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_shadow
********************************************************************/
be_local_closure(Matter_Plugin_Light1_update_shadow,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(VIRTUAL),
    /* K1   */  be_nested_str_weak(light),
    /* K2   */  be_nested_str_weak(get),
    /* K3   */  be_nested_str_weak(find),
    /* K4   */  be_nested_str_weak(bri),
    /* K5   */  be_nested_str_weak(tasmota),
    /* K6   */  be_nested_str_weak(scale_uint),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str_weak(shadow_bri),
    /* K9   */  be_nested_str_weak(attribute_updated),
    /* K10  */  be_nested_str_weak(update_shadow),
    }),
    be_str_weak(update_shadow),
    &be_const_str_solidified,
    ( &(const binstruction[38]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x7406001D,  //  0001  JMPT	R1	#0020
      0xA4060200,  //  0002  IMPORT	R1	K1
      0x8C080302,  //  0003  GETMET	R2	R1	K2
      0x7C080200,  //  0004  CALL	R2	1
      0x4C0C0000,  //  0005  LDNIL	R3
      0x200C0403,  //  0006  NE	R3	R2	R3
      0x780E0017,  //  0007  JMPF	R3	#0020
      0x8C0C0503,  //  0008  GETMET	R3	R2	K3
      0x58140004,  //  0009  LDCONST	R5	K4
      0x4C180000,  //  000A  LDNIL	R6
      0x7C0C0600,  //  000B  CALL	R3	3
      0x4C100000,  //  000C  LDNIL	R4
      0x20100604,  //  000D  NE	R4	R3	R4
      0x78120010,  //  000E  JMPF	R4	#0020
      0xB8120A00,  //  000F  GETNGBL	R4	K5
      0x8C100906,  //  0010  GETMET	R4	R4	K6
      0x5C180600,  //  0011  MOVE	R6	R3
      0x581C0007,  //  0012  LDCONST	R7	K7
      0x542200FE,  //  0013  LDINT	R8	255
      0x58240007,  //  0014  LDCONST	R9	K7
      0x542A00FD,  //  0015  LDINT	R10	254
      0x7C100C00,  //  0016  CALL	R4	6
      0x5C0C0800,  //  0017  MOVE	R3	R4
      0x88100108,  //  0018  GETMBR	R4	R0	K8
      0x20100604,  //  0019  NE	R4	R3	R4
      0x78120004,  //  001A  JMPF	R4	#0020
      0x8C100109,  //  001B  GETMET	R4	R0	K9
      0x541A0007,  //  001C  LDINT	R6	8
      0x581C0007,  //  001D  LDCONST	R7	K7
      0x7C100600,  //  001E  CALL	R4	3
      0x90021003,  //  001F  SETMBR	R0	K8	R3
      0x60040003,  //  0020  GETGBL	R1	G3
      0x5C080000,  //  0021  MOVE	R2	R0
      0x7C040200,  //  0022  CALL	R1	1
      0x8C04030A,  //  0023  GETMET	R1	R1	K10
      0x7C040200,  //  0024  CALL	R1	1
      0x80000000,  //  0025  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Light1_read_attribute,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(attribute),
    /* K4   */  be_nested_str_weak(update_shadow_lazy),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(set),
    /* K7   */  be_nested_str_weak(U1),
    /* K8   */  be_nested_str_weak(shadow_bri),
    /* K9   */  be_const_int(2),
    /* K10  */  be_const_int(3),
    /* K11  */  be_nested_str_weak(U4),
    /* K12  */  be_const_int(1),
    /* K13  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[79]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140502,  //  0002  GETMBR	R5	R2	K2
      0x88180503,  //  0003  GETMBR	R6	R2	K3
      0x541E0007,  //  0004  LDINT	R7	8
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E003D,  //  0006  JMPF	R7	#0045
      0x8C1C0104,  //  0007  GETMET	R7	R0	K4
      0x7C1C0200,  //  0008  CALL	R7	1
      0x1C1C0D05,  //  0009  EQ	R7	R6	K5
      0x781E0005,  //  000A  JMPF	R7	#0011
      0x8C1C0706,  //  000B  GETMET	R7	R3	K6
      0x88240907,  //  000C  GETMBR	R9	R4	K7
      0x88280108,  //  000D  GETMBR	R10	R0	K8
      0x7C1C0600,  //  000E  CALL	R7	3
      0x80040E00,  //  000F  RET	1	R7
      0x70020032,  //  0010  JMP		#0044
      0x1C1C0D09,  //  0011  EQ	R7	R6	K9
      0x781E0005,  //  0012  JMPF	R7	#0019
      0x8C1C0706,  //  0013  GETMET	R7	R3	K6
      0x88240907,  //  0014  GETMBR	R9	R4	K7
      0x58280005,  //  0015  LDCONST	R10	K5
      0x7C1C0600,  //  0016  CALL	R7	3
      0x80040E00,  //  0017  RET	1	R7
      0x7002002A,  //  0018  JMP		#0044
      0x1C1C0D0A,  //  0019  EQ	R7	R6	K10
      0x781E0005,  //  001A  JMPF	R7	#0021
      0x8C1C0706,  //  001B  GETMET	R7	R3	K6
      0x88240907,  //  001C  GETMBR	R9	R4	K7
      0x542A00FD,  //  001D  LDINT	R10	254
      0x7C1C0600,  //  001E  CALL	R7	3
      0x80040E00,  //  001F  RET	1	R7
      0x70020022,  //  0020  JMP		#0044
      0x541E000E,  //  0021  LDINT	R7	15
      0x1C1C0C07,  //  0022  EQ	R7	R6	R7
      0x781E0005,  //  0023  JMPF	R7	#002A
      0x8C1C0706,  //  0024  GETMET	R7	R3	K6
      0x88240907,  //  0025  GETMBR	R9	R4	K7
      0x58280005,  //  0026  LDCONST	R10	K5
      0x7C1C0600,  //  0027  CALL	R7	3
      0x80040E00,  //  0028  RET	1	R7
      0x70020019,  //  0029  JMP		#0044
      0x541E0010,  //  002A  LDINT	R7	17
      0x1C1C0C07,  //  002B  EQ	R7	R6	R7
      0x781E0005,  //  002C  JMPF	R7	#0033
      0x8C1C0706,  //  002D  GETMET	R7	R3	K6
      0x88240907,  //  002E  GETMBR	R9	R4	K7
      0x88280108,  //  002F  GETMBR	R10	R0	K8
      0x7C1C0600,  //  0030  CALL	R7	3
      0x80040E00,  //  0031  RET	1	R7
      0x70020010,  //  0032  JMP		#0044
      0x541EFFFB,  //  0033  LDINT	R7	65532
      0x1C1C0C07,  //  0034  EQ	R7	R6	R7
      0x781E0005,  //  0035  JMPF	R7	#003C
      0x8C1C0706,  //  0036  GETMET	R7	R3	K6
      0x8824090B,  //  0037  GETMBR	R9	R4	K11
      0x5828000C,  //  0038  LDCONST	R10	K12
      0x7C1C0600,  //  0039  CALL	R7	3
      0x80040E00,  //  003A  RET	1	R7
      0x70020007,  //  003B  JMP		#0044
      0x541EFFFC,  //  003C  LDINT	R7	65533
      0x1C1C0C07,  //  003D  EQ	R7	R6	R7
      0x781E0004,  //  003E  JMPF	R7	#0044
      0x8C1C0706,  //  003F  GETMET	R7	R3	K6
      0x8824090B,  //  0040  GETMBR	R9	R4	K11
      0x542A0004,  //  0041  LDINT	R10	5
      0x7C1C0600,  //  0042  CALL	R7	3
      0x80040E00,  //  0043  RET	1	R7
      0x70020008,  //  0044  JMP		#004E
      0x601C0003,  //  0045  GETGBL	R7	G3
      0x5C200000,  //  0046  MOVE	R8	R0
      0x7C1C0200,  //  0047  CALL	R7	1
      0x8C1C0F0D,  //  0048  GETMET	R7	R7	K13
      0x5C240200,  //  0049  MOVE	R9	R1
      0x5C280400,  //  004A  MOVE	R10	R2
      0x5C2C0600,  //  004B  MOVE	R11	R3
      0x7C1C0800,  //  004C  CALL	R7	4
      0x80040E00,  //  004D  RET	1	R7
      0x80000000,  //  004E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_virtual
********************************************************************/
be_local_closure(Matter_Plugin_Light1_update_virtual,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(find),
    /* K1   */  be_nested_str_weak(Power),
    /* K2   */  be_nested_str_weak(Bri),
    /* K3   */  be_nested_str_weak(set_bri),
    /* K4   */  be_nested_str_weak(update_virtual),
    }),
    be_str_weak(update_virtual),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x8C080300,  //  0000  GETMET	R2	R1	K0
      0x58100001,  //  0001  LDCONST	R4	K1
      0x7C080400,  //  0002  CALL	R2	2
      0x8C0C0300,  //  0003  GETMET	R3	R1	K0
      0x58140002,  //  0004  LDCONST	R5	K2
      0x7C0C0400,  //  0005  CALL	R3	2
      0x4C100000,  //  0006  LDNIL	R4
      0x20100604,  //  0007  NE	R4	R3	R4
      0x78120006,  //  0008  JMPF	R4	#0010
      0x8C100103,  //  0009  GETMET	R4	R0	K3
      0x60180009,  //  000A  GETGBL	R6	G9
      0x5C1C0600,  //  000B  MOVE	R7	R3
      0x7C180200,  //  000C  CALL	R6	1
      0x5C1C0400,  //  000D  MOVE	R7	R2
      0x7C100600,  //  000E  CALL	R4	3
      0x80000800,  //  000F  RET	0
      0x60100003,  //  0010  GETGBL	R4	G3
      0x5C140000,  //  0011  MOVE	R5	R0
      0x7C100200,  //  0012  CALL	R4	1
      0x8C100904,  //  0013  GETMET	R4	R4	K4
      0x5C180200,  //  0014  MOVE	R6	R1
      0x7C100400,  //  0015  CALL	R4	2
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Plugin_Light1_invoke_request,   /* name */
  be_nested_proto(
    22,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[21]) {     /* constants */
    /* K0   */  be_nested_str_weak(light),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(command),
    /* K5   */  be_nested_str_weak(update_shadow_lazy),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(findsubval),
    /* K8   */  be_nested_str_weak(set_bri),
    /* K9   */  be_nested_str_weak(log),
    /* K10  */  be_nested_str_weak(bri_X3A),
    /* K11  */  be_nested_str_weak(publish_command),
    /* K12  */  be_nested_str_weak(Bri),
    /* K13  */  be_nested_str_weak(Dimmer),
    /* K14  */  be_nested_str_weak(tasmota),
    /* K15  */  be_nested_str_weak(scale_uint),
    /* K16  */  be_const_int(1),
    /* K17  */  be_const_int(2),
    /* K18  */  be_const_int(3),
    /* K19  */  be_nested_str_weak(Power),
    /* K20  */  be_nested_str_weak(invoke_request),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[119]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0xB8160200,  //  0001  GETNGBL	R5	K1
      0x88140B02,  //  0002  GETMBR	R5	R5	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x881C0704,  //  0004  GETMBR	R7	R3	K4
      0x54220007,  //  0005  LDINT	R8	8
      0x1C200C08,  //  0006  EQ	R8	R6	R8
      0x78220064,  //  0007  JMPF	R8	#006D
      0x8C200105,  //  0008  GETMET	R8	R0	K5
      0x7C200200,  //  0009  CALL	R8	1
      0x1C200F06,  //  000A  EQ	R8	R7	K6
      0x7822001A,  //  000B  JMPF	R8	#0027
      0x8C200507,  //  000C  GETMET	R8	R2	K7
      0x58280006,  //  000D  LDCONST	R10	K6
      0x7C200400,  //  000E  CALL	R8	2
      0x8C240108,  //  000F  GETMET	R9	R0	K8
      0x5C2C1000,  //  0010  MOVE	R11	R8
      0x7C240400,  //  0011  CALL	R9	2
      0x60240008,  //  0012  GETGBL	R9	G8
      0x5C281000,  //  0013  MOVE	R10	R8
      0x7C240200,  //  0014  CALL	R9	1
      0x00261409,  //  0015  ADD	R9	K10	R9
      0x900E1209,  //  0016  SETMBR	R3	K9	R9
      0x8C24010B,  //  0017  GETMET	R9	R0	K11
      0x582C000C,  //  0018  LDCONST	R11	K12
      0x5C301000,  //  0019  MOVE	R12	R8
      0x5834000D,  //  001A  LDCONST	R13	K13
      0xB83A1C00,  //  001B  GETNGBL	R14	K14
      0x8C381D0F,  //  001C  GETMET	R14	R14	K15
      0x5C401000,  //  001D  MOVE	R16	R8
      0x58440006,  //  001E  LDCONST	R17	K6
      0x544A00FD,  //  001F  LDINT	R18	254
      0x584C0006,  //  0020  LDCONST	R19	K6
      0x54520063,  //  0021  LDINT	R20	100
      0x7C380C00,  //  0022  CALL	R14	6
      0x7C240A00,  //  0023  CALL	R9	5
      0x50240200,  //  0024  LDBOOL	R9	1	0
      0x80041200,  //  0025  RET	1	R9
      0x70020044,  //  0026  JMP		#006C
      0x1C200F10,  //  0027  EQ	R8	R7	K16
      0x78220002,  //  0028  JMPF	R8	#002C
      0x50200200,  //  0029  LDBOOL	R8	1	0
      0x80041000,  //  002A  RET	1	R8
      0x7002003F,  //  002B  JMP		#006C
      0x1C200F11,  //  002C  EQ	R8	R7	K17
      0x78220002,  //  002D  JMPF	R8	#0031
      0x50200200,  //  002E  LDBOOL	R8	1	0
      0x80041000,  //  002F  RET	1	R8
      0x7002003A,  //  0030  JMP		#006C
      0x1C200F12,  //  0031  EQ	R8	R7	K18
      0x78220002,  //  0032  JMPF	R8	#0036
      0x50200200,  //  0033  LDBOOL	R8	1	0
      0x80041000,  //  0034  RET	1	R8
      0x70020035,  //  0035  JMP		#006C
      0x54220003,  //  0036  LDINT	R8	4
      0x1C200E08,  //  0037  EQ	R8	R7	R8
      0x78220021,  //  0038  JMPF	R8	#005B
      0x8C200507,  //  0039  GETMET	R8	R2	K7
      0x58280006,  //  003A  LDCONST	R10	K6
      0x7C200400,  //  003B  CALL	R8	2
      0x24241106,  //  003C  GT	R9	R8	K6
      0x8C280108,  //  003D  GETMET	R10	R0	K8
      0x5C301000,  //  003E  MOVE	R12	R8
      0x5C341200,  //  003F  MOVE	R13	R9
      0x7C280600,  //  0040  CALL	R10	3
      0x60280008,  //  0041  GETGBL	R10	G8
      0x5C2C1000,  //  0042  MOVE	R11	R8
      0x7C280200,  //  0043  CALL	R10	1
      0x002A140A,  //  0044  ADD	R10	K10	R10
      0x900E120A,  //  0045  SETMBR	R3	K9	R10
      0x8C28010B,  //  0046  GETMET	R10	R0	K11
      0x5830000C,  //  0047  LDCONST	R12	K12
      0x5C341000,  //  0048  MOVE	R13	R8
      0x5838000D,  //  0049  LDCONST	R14	K13
      0xB83E1C00,  //  004A  GETNGBL	R15	K14
      0x8C3C1F0F,  //  004B  GETMET	R15	R15	K15
      0x5C441000,  //  004C  MOVE	R17	R8
      0x58480006,  //  004D  LDCONST	R18	K6
      0x544E00FD,  //  004E  LDINT	R19	254
      0x58500006,  //  004F  LDCONST	R20	K6
      0x54560063,  //  0050  LDINT	R21	100
      0x7C3C0C00,  //  0051  CALL	R15	6
      0x58400013,  //  0052  LDCONST	R16	K19
      0x78260001,  //  0053  JMPF	R9	#0056
      0x58440010,  //  0054  LDCONST	R17	K16
      0x70020000,  //  0055  JMP		#0057
      0x58440006,  //  0056  LDCONST	R17	K6
      0x7C280E00,  //  0057  CALL	R10	7
      0x50280200,  //  0058  LDBOOL	R10	1	0
      0x80041400,  //  0059  RET	1	R10
      0x70020010,  //  005A  JMP		#006C
      0x54220004,  //  005B  LDINT	R8	5
      0x1C200E08,  //  005C  EQ	R8	R7	R8
      0x78220002,  //  005D  JMPF	R8	#0061
      0x50200200,  //  005E  LDBOOL	R8	1	0
      0x80041000,  //  005F  RET	1	R8
      0x7002000A,  //  0060  JMP		#006C
      0x54220005,  //  0061  LDINT	R8	6
      0x1C200E08,  //  0062  EQ	R8	R7	R8
      0x78220002,  //  0063  JMPF	R8	#0067
      0x50200200,  //  0064  LDBOOL	R8	1	0
      0x80041000,  //  0065  RET	1	R8
      0x70020004,  //  0066  JMP		#006C
      0x54220006,  //  0067  LDINT	R8	7
      0x1C200E08,  //  0068  EQ	R8	R7	R8
      0x78220001,  //  0069  JMPF	R8	#006C
      0x50200200,  //  006A  LDBOOL	R8	1	0
      0x80041000,  //  006B  RET	1	R8
      0x70020008,  //  006C  JMP		#0076
      0x60200003,  //  006D  GETGBL	R8	G3
      0x5C240000,  //  006E  MOVE	R9	R0
      0x7C200200,  //  006F  CALL	R8	1
      0x8C201114,  //  0070  GETMET	R8	R8	K20
      0x5C280200,  //  0071  MOVE	R10	R1
      0x5C2C0400,  //  0072  MOVE	R11	R2
      0x5C300600,  //  0073  MOVE	R12	R3
      0x7C200800,  //  0074  CALL	R8	4
      0x80041000,  //  0075  RET	1	R8
      0x80000000,  //  0076  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Light1
********************************************************************/
extern const bclass be_class_Matter_Plugin_Light0;
be_local_class(Matter_Plugin_Light1,
    1,
    &be_class_Matter_Plugin_Light0,
    be_nested_map(12,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(invoke_request, -1), be_const_closure(Matter_Plugin_Light1_invoke_request_closure) },
        { be_const_key_weak(shadow_bri, 7), be_const_var(0) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(257, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(UPDATE_COMMANDS, 4), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(2,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(Power),
        be_nested_str_weak(Bri),
    }))    ) } )) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_Light1_init_closure) },
        { be_const_key_weak(update_shadow, -1), be_const_closure(Matter_Plugin_Light1_update_shadow_closure) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(Matter_Plugin_Light1_read_attribute_closure) },
        { be_const_key_weak(TYPE, 5), be_nested_str_weak(light1) },
        { be_const_key_weak(update_virtual, -1), be_const_closure(Matter_Plugin_Light1_update_virtual_closure) },
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(Light_X201_X20Dimmer) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(29, 2), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(6,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(8, 0), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(7,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(2),
        be_const_int(3),
        be_const_int(15),
        be_const_int(17),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(57, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(7,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(17),
        be_const_int(3),
        be_const_int(5),
        be_const_int(10),
        be_const_int(15),
        be_const_int(17),
        be_const_int(18),
    }))    ) } )) },
        { be_const_key_int(3, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(4,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(4, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(5, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(8,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
        be_const_int(4),
        be_const_int(5),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(6, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(set_bri, 0), be_const_closure(Matter_Plugin_Light1_set_bri_closure) },
    })),
    be_str_weak(Matter_Plugin_Light1)
);
/*******************************************************************/

void be_load_Matter_Plugin_Light1_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Light1);
    be_setglobal(vm, "Matter_Plugin_Light1");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
