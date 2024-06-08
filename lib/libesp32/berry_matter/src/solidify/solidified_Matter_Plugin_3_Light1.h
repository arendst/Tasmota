/* Solidification of Matter_Plugin_3_Light1.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Light1;

/********************************************************************
** Solidified function: update_virtual
********************************************************************/
extern const bclass be_class_Matter_Plugin_Light1;
be_local_closure(class_Matter_Plugin_Light1_update_virtual,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Light1, 
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
** Solidified function: set_bri
********************************************************************/
extern const bclass be_class_Matter_Plugin_Light1;
be_local_closure(class_Matter_Plugin_Light1_set_bri,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Light1, 
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(BRIDGE),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(scale_uint),
    /* K4   */  be_nested_str_weak(call_remote_sync),
    /* K5   */  be_nested_str_weak(Dimmer),
    /* K6   */  be_nested_str_weak(parse_status),
    /* K7   */  be_nested_str_weak(VIRTUAL),
    /* K8   */  be_nested_str_weak(shadow_onoff),
    /* K9   */  be_nested_str_weak(attribute_updated),
    /* K10  */  be_nested_str_weak(shadow_bri),
    /* K11  */  be_nested_str_weak(light),
    /* K12  */  be_nested_str_weak(set),
    /* K13  */  be_nested_str_weak(bri),
    /* K14  */  be_nested_str_weak(power),
    /* K15  */  be_nested_str_weak(update_shadow),
    }),
    be_str_weak(set_bri),
    &be_const_str_solidified,
    ( &(const binstruction[89]) {  /* code */
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
      0x780E0015,  //  0011  JMPF	R3	#0028
      0xB80E0400,  //  0012  GETNGBL	R3	K2
      0x8C0C0703,  //  0013  GETMET	R3	R3	K3
      0x5C140200,  //  0014  MOVE	R5	R1
      0x58180000,  //  0015  LDCONST	R6	K0
      0x541E00FD,  //  0016  LDINT	R7	254
      0x58200000,  //  0017  LDCONST	R8	K0
      0x54260063,  //  0018  LDINT	R9	100
      0x7C0C0C00,  //  0019  CALL	R3	6
      0x8C100104,  //  001A  GETMET	R4	R0	K4
      0x58180005,  //  001B  LDCONST	R6	K5
      0x601C0008,  //  001C  GETGBL	R7	G8
      0x5C200600,  //  001D  MOVE	R8	R3
      0x7C1C0200,  //  001E  CALL	R7	1
      0x7C100600,  //  001F  CALL	R4	3
      0x4C140000,  //  0020  LDNIL	R5
      0x20140805,  //  0021  NE	R5	R4	R5
      0x78160003,  //  0022  JMPF	R5	#0027
      0x8C140106,  //  0023  GETMET	R5	R0	K6
      0x5C1C0800,  //  0024  MOVE	R7	R4
      0x5422000A,  //  0025  LDINT	R8	11
      0x7C140600,  //  0026  CALL	R5	3
      0x7002002F,  //  0027  JMP		#0058
      0x880C0107,  //  0028  GETMBR	R3	R0	K7
      0x780E0013,  //  0029  JMPF	R3	#003E
      0x4C0C0000,  //  002A  LDNIL	R3
      0x200C0403,  //  002B  NE	R3	R2	R3
      0x780E0007,  //  002C  JMPF	R3	#0035
      0x880C0108,  //  002D  GETMBR	R3	R0	K8
      0x200C0403,  //  002E  NE	R3	R2	R3
      0x780E0004,  //  002F  JMPF	R3	#0035
      0x8C0C0109,  //  0030  GETMET	R3	R0	K9
      0x54160005,  //  0031  LDINT	R5	6
      0x58180000,  //  0032  LDCONST	R6	K0
      0x7C0C0600,  //  0033  CALL	R3	3
      0x90021002,  //  0034  SETMBR	R0	K8	R2
      0x880C010A,  //  0035  GETMBR	R3	R0	K10
      0x200C0203,  //  0036  NE	R3	R1	R3
      0x780E0004,  //  0037  JMPF	R3	#003D
      0x8C0C0109,  //  0038  GETMET	R3	R0	K9
      0x54160007,  //  0039  LDINT	R5	8
      0x58180000,  //  003A  LDCONST	R6	K0
      0x7C0C0600,  //  003B  CALL	R3	3
      0x90021401,  //  003C  SETMBR	R0	K10	R1
      0x70020019,  //  003D  JMP		#0058
      0xA40E1600,  //  003E  IMPORT	R3	K11
      0xB8120400,  //  003F  GETNGBL	R4	K2
      0x8C100903,  //  0040  GETMET	R4	R4	K3
      0x5C180200,  //  0041  MOVE	R6	R1
      0x581C0000,  //  0042  LDCONST	R7	K0
      0x542200FD,  //  0043  LDINT	R8	254
      0x58240000,  //  0044  LDCONST	R9	K0
      0x542A00FE,  //  0045  LDINT	R10	255
      0x7C100C00,  //  0046  CALL	R4	6
      0x4C140000,  //  0047  LDNIL	R5
      0x1C140405,  //  0048  EQ	R5	R2	R5
      0x78160005,  //  0049  JMPF	R5	#0050
      0x8C14070C,  //  004A  GETMET	R5	R3	K12
      0x601C0013,  //  004B  GETGBL	R7	G19
      0x7C1C0000,  //  004C  CALL	R7	0
      0x981E1A04,  //  004D  SETIDX	R7	K13	R4
      0x7C140400,  //  004E  CALL	R5	2
      0x70020005,  //  004F  JMP		#0056
      0x8C14070C,  //  0050  GETMET	R5	R3	K12
      0x601C0013,  //  0051  GETGBL	R7	G19
      0x7C1C0000,  //  0052  CALL	R7	0
      0x981E1A04,  //  0053  SETIDX	R7	K13	R4
      0x981E1C02,  //  0054  SETIDX	R7	K14	R2
      0x7C140400,  //  0055  CALL	R5	2
      0x8C14010F,  //  0056  GETMET	R5	R0	K15
      0x7C140200,  //  0057  CALL	R5	1
      0x80000000,  //  0058  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
extern const bclass be_class_Matter_Plugin_Light1;
be_local_closure(class_Matter_Plugin_Light1_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Light1, 
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
extern const bclass be_class_Matter_Plugin_Light1;
be_local_closure(class_Matter_Plugin_Light1_update_shadow,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Light1, 
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(VIRTUAL),
    /* K1   */  be_nested_str_weak(BRIDGE),
    /* K2   */  be_nested_str_weak(light),
    /* K3   */  be_nested_str_weak(get),
    /* K4   */  be_nested_str_weak(find),
    /* K5   */  be_nested_str_weak(power),
    /* K6   */  be_nested_str_weak(shadow_onoff),
    /* K7   */  be_nested_str_weak(attribute_updated),
    /* K8   */  be_const_int(0),
    /* K9   */  be_nested_str_weak(bri),
    /* K10  */  be_nested_str_weak(tasmota),
    /* K11  */  be_nested_str_weak(scale_uint),
    /* K12  */  be_nested_str_weak(shadow_bri),
    /* K13  */  be_nested_str_weak(update_shadow),
    }),
    be_str_weak(update_shadow),
    &be_const_str_solidified,
    ( &(const binstruction[52]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x7406002B,  //  0001  JMPT	R1	#002E
      0x88040101,  //  0002  GETMBR	R1	R0	K1
      0x74060029,  //  0003  JMPT	R1	#002E
      0xA4060400,  //  0004  IMPORT	R1	K2
      0x8C080303,  //  0005  GETMET	R2	R1	K3
      0x7C080200,  //  0006  CALL	R2	1
      0x4C0C0000,  //  0007  LDNIL	R3
      0x200C0403,  //  0008  NE	R3	R2	R3
      0x780E0023,  //  0009  JMPF	R3	#002E
      0x8C0C0504,  //  000A  GETMET	R3	R2	K4
      0x58140005,  //  000B  LDCONST	R5	K5
      0x4C180000,  //  000C  LDNIL	R6
      0x7C0C0600,  //  000D  CALL	R3	3
      0x88100106,  //  000E  GETMBR	R4	R0	K6
      0x20100604,  //  000F  NE	R4	R3	R4
      0x78120004,  //  0010  JMPF	R4	#0016
      0x8C100107,  //  0011  GETMET	R4	R0	K7
      0x541A0005,  //  0012  LDINT	R6	6
      0x581C0008,  //  0013  LDCONST	R7	K8
      0x7C100600,  //  0014  CALL	R4	3
      0x90020C03,  //  0015  SETMBR	R0	K6	R3
      0x8C100504,  //  0016  GETMET	R4	R2	K4
      0x58180009,  //  0017  LDCONST	R6	K9
      0x4C1C0000,  //  0018  LDNIL	R7
      0x7C100600,  //  0019  CALL	R4	3
      0x4C140000,  //  001A  LDNIL	R5
      0x20140805,  //  001B  NE	R5	R4	R5
      0x78160010,  //  001C  JMPF	R5	#002E
      0xB8161400,  //  001D  GETNGBL	R5	K10
      0x8C140B0B,  //  001E  GETMET	R5	R5	K11
      0x5C1C0800,  //  001F  MOVE	R7	R4
      0x58200008,  //  0020  LDCONST	R8	K8
      0x542600FE,  //  0021  LDINT	R9	255
      0x58280008,  //  0022  LDCONST	R10	K8
      0x542E00FD,  //  0023  LDINT	R11	254
      0x7C140C00,  //  0024  CALL	R5	6
      0x5C100A00,  //  0025  MOVE	R4	R5
      0x8814010C,  //  0026  GETMBR	R5	R0	K12
      0x20140805,  //  0027  NE	R5	R4	R5
      0x78160004,  //  0028  JMPF	R5	#002E
      0x8C140107,  //  0029  GETMET	R5	R0	K7
      0x541E0007,  //  002A  LDINT	R7	8
      0x58200008,  //  002B  LDCONST	R8	K8
      0x7C140600,  //  002C  CALL	R5	3
      0x90021804,  //  002D  SETMBR	R0	K12	R4
      0x60040003,  //  002E  GETGBL	R1	G3
      0x5C080000,  //  002F  MOVE	R2	R0
      0x7C040200,  //  0030  CALL	R1	1
      0x8C04030D,  //  0031  GETMET	R1	R1	K13
      0x7C040200,  //  0032  CALL	R1	1
      0x80000000,  //  0033  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_configuration
********************************************************************/
extern const bclass be_class_Matter_Plugin_Light1;
be_local_closure(class_Matter_Plugin_Light1_parse_configuration,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Light1, 
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(BRIDGE),
    /* K1   */  be_nested_str_weak(tasmota_relay_index),
    /* K2   */  be_nested_str_weak(find),
    /* K3   */  be_nested_str_weak(ARG),
    /* K4   */  be_const_int(0),
    /* K5   */  be_const_int(1),
    }),
    be_str_weak(parse_configuration),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x780A000E,  //  0001  JMPF	R2	#0011
      0x60080009,  //  0002  GETGBL	R2	G9
      0x8C0C0302,  //  0003  GETMET	R3	R1	K2
      0x88140103,  //  0004  GETMBR	R5	R0	K3
      0x4C180000,  //  0005  LDNIL	R6
      0x7C0C0600,  //  0006  CALL	R3	3
      0x7C080200,  //  0007  CALL	R2	1
      0x90020202,  //  0008  SETMBR	R0	K1	R2
      0x88080101,  //  0009  GETMBR	R2	R0	K1
      0x4C0C0000,  //  000A  LDNIL	R3
      0x20080403,  //  000B  NE	R2	R2	R3
      0x780A0003,  //  000C  JMPF	R2	#0011
      0x88080101,  //  000D  GETMBR	R2	R0	K1
      0x18080504,  //  000E  LE	R2	R2	K4
      0x780A0000,  //  000F  JMPF	R2	#0011
      0x90020305,  //  0010  SETMBR	R0	K1	K5
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_status
********************************************************************/
extern const bclass be_class_Matter_Plugin_Light1;
be_local_closure(class_Matter_Plugin_Light1_parse_status,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Light1, 
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(parse_status),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(Dimmer),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(scale_uint),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(shadow_bri),
    /* K7   */  be_nested_str_weak(attribute_updated),
    }),
    be_str_weak(parse_status),
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0700,  //  0003  GETMET	R3	R3	K0
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x540E000A,  //  0007  LDINT	R3	11
      0x1C0C0403,  //  0008  EQ	R3	R2	R3
      0x780E0017,  //  0009  JMPF	R3	#0022
      0x600C0009,  //  000A  GETGBL	R3	G9
      0x8C100301,  //  000B  GETMET	R4	R1	K1
      0x58180002,  //  000C  LDCONST	R6	K2
      0x7C100400,  //  000D  CALL	R4	2
      0x7C0C0200,  //  000E  CALL	R3	1
      0x4C100000,  //  000F  LDNIL	R4
      0x20100604,  //  0010  NE	R4	R3	R4
      0x7812000F,  //  0011  JMPF	R4	#0022
      0xB8120600,  //  0012  GETNGBL	R4	K3
      0x8C100904,  //  0013  GETMET	R4	R4	K4
      0x5C180600,  //  0014  MOVE	R6	R3
      0x581C0005,  //  0015  LDCONST	R7	K5
      0x54220063,  //  0016  LDINT	R8	100
      0x58240005,  //  0017  LDCONST	R9	K5
      0x542A00FD,  //  0018  LDINT	R10	254
      0x7C100C00,  //  0019  CALL	R4	6
      0x88140106,  //  001A  GETMBR	R5	R0	K6
      0x20140805,  //  001B  NE	R5	R4	R5
      0x78160004,  //  001C  JMPF	R5	#0022
      0x8C140107,  //  001D  GETMET	R5	R0	K7
      0x541E0007,  //  001E  LDINT	R7	8
      0x58200005,  //  001F  LDCONST	R8	K5
      0x7C140600,  //  0020  CALL	R5	3
      0x90020C04,  //  0021  SETMBR	R0	K6	R4
      0x80000000,  //  0022  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
extern const bclass be_class_Matter_Plugin_Light1;
be_local_closure(class_Matter_Plugin_Light1_read_attribute,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Light1, 
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
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
    /* K11  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[59]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140502,  //  0002  GETMBR	R5	R2	K2
      0x88180503,  //  0003  GETMBR	R6	R2	K3
      0x541E0007,  //  0004  LDINT	R7	8
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E002A,  //  0006  JMPF	R7	#0032
      0x8C1C0104,  //  0007  GETMET	R7	R0	K4
      0x7C1C0200,  //  0008  CALL	R7	1
      0x1C1C0D05,  //  0009  EQ	R7	R6	K5
      0x781E0005,  //  000A  JMPF	R7	#0011
      0x8C1C0706,  //  000B  GETMET	R7	R3	K6
      0x88240907,  //  000C  GETMBR	R9	R4	K7
      0x88280108,  //  000D  GETMBR	R10	R0	K8
      0x7C1C0600,  //  000E  CALL	R7	3
      0x80040E00,  //  000F  RET	1	R7
      0x70020020,  //  0010  JMP		#0032
      0x1C1C0D09,  //  0011  EQ	R7	R6	K9
      0x781E0005,  //  0012  JMPF	R7	#0019
      0x8C1C0706,  //  0013  GETMET	R7	R3	K6
      0x88240907,  //  0014  GETMBR	R9	R4	K7
      0x58280005,  //  0015  LDCONST	R10	K5
      0x7C1C0600,  //  0016  CALL	R7	3
      0x80040E00,  //  0017  RET	1	R7
      0x70020018,  //  0018  JMP		#0032
      0x1C1C0D0A,  //  0019  EQ	R7	R6	K10
      0x781E0005,  //  001A  JMPF	R7	#0021
      0x8C1C0706,  //  001B  GETMET	R7	R3	K6
      0x88240907,  //  001C  GETMBR	R9	R4	K7
      0x542A00FD,  //  001D  LDINT	R10	254
      0x7C1C0600,  //  001E  CALL	R7	3
      0x80040E00,  //  001F  RET	1	R7
      0x70020010,  //  0020  JMP		#0032
      0x541E000E,  //  0021  LDINT	R7	15
      0x1C1C0C07,  //  0022  EQ	R7	R6	R7
      0x781E0005,  //  0023  JMPF	R7	#002A
      0x8C1C0706,  //  0024  GETMET	R7	R3	K6
      0x88240907,  //  0025  GETMBR	R9	R4	K7
      0x58280005,  //  0026  LDCONST	R10	K5
      0x7C1C0600,  //  0027  CALL	R7	3
      0x80040E00,  //  0028  RET	1	R7
      0x70020007,  //  0029  JMP		#0032
      0x541E0010,  //  002A  LDINT	R7	17
      0x1C1C0C07,  //  002B  EQ	R7	R6	R7
      0x781E0004,  //  002C  JMPF	R7	#0032
      0x8C1C0706,  //  002D  GETMET	R7	R3	K6
      0x88240907,  //  002E  GETMBR	R9	R4	K7
      0x88280108,  //  002F  GETMBR	R10	R0	K8
      0x7C1C0600,  //  0030  CALL	R7	3
      0x80040E00,  //  0031  RET	1	R7
      0x601C0003,  //  0032  GETGBL	R7	G3
      0x5C200000,  //  0033  MOVE	R8	R0
      0x7C1C0200,  //  0034  CALL	R7	1
      0x8C1C0F0B,  //  0035  GETMET	R7	R7	K11
      0x5C240200,  //  0036  MOVE	R9	R1
      0x5C280400,  //  0037  MOVE	R10	R2
      0x5C2C0600,  //  0038  MOVE	R11	R3
      0x7C1C0800,  //  0039  CALL	R7	4
      0x80040E00,  //  003A  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
extern const bclass be_class_Matter_Plugin_Light1;
be_local_closure(class_Matter_Plugin_Light1_invoke_request,   /* name */
  be_nested_proto(
    22,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Light1, 
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
** Solidified function: web_value_dimmer
********************************************************************/
extern const bclass be_class_Matter_Plugin_Light1;
be_local_closure(class_Matter_Plugin_Light1_web_value_dimmer,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Light1, 
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(),
    /* K1   */  be_nested_str_weak(shadow_bri),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(scale_uint),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(_X25i_X25_X25),
    /* K6   */  be_nested_str_weak(_X26_X23128261_X3B_X20),
    }),
    be_str_weak(web_value_dimmer),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x4C0C0000,  //  0002  LDNIL	R3
      0x20080403,  //  0003  NE	R2	R2	R3
      0x780A000C,  //  0004  JMPF	R2	#0012
      0xB80A0400,  //  0005  GETNGBL	R2	K2
      0x8C080503,  //  0006  GETMET	R2	R2	K3
      0x88100101,  //  0007  GETMBR	R4	R0	K1
      0x58140004,  //  0008  LDCONST	R5	K4
      0x541A00FD,  //  0009  LDINT	R6	254
      0x581C0004,  //  000A  LDCONST	R7	K4
      0x54220063,  //  000B  LDINT	R8	100
      0x7C080C00,  //  000C  CALL	R2	6
      0x600C0018,  //  000D  GETGBL	R3	G24
      0x58100005,  //  000E  LDCONST	R4	K5
      0x5C140400,  //  000F  MOVE	R5	R2
      0x7C0C0400,  //  0010  CALL	R3	2
      0x5C040600,  //  0011  MOVE	R1	R3
      0x000A0C01,  //  0012  ADD	R2	K6	R1
      0x80040400,  //  0013  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_values
********************************************************************/
extern const bclass be_class_Matter_Plugin_Light1;
be_local_closure(class_Matter_Plugin_Light1_web_values,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Light1, 
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(web_values_prefix),
    /* K2   */  be_nested_str_weak(content_send),
    /* K3   */  be_nested_str_weak(_X25s_X20_X25s),
    /* K4   */  be_nested_str_weak(web_value_onoff),
    /* K5   */  be_nested_str_weak(shadow_onoff),
    /* K6   */  be_nested_str_weak(web_value_dimmer),
    }),
    be_str_weak(web_values),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080101,  //  0001  GETMET	R2	R0	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080302,  //  0003  GETMET	R2	R1	K2
      0x60100018,  //  0004  GETGBL	R4	G24
      0x58140003,  //  0005  LDCONST	R5	K3
      0x8C180104,  //  0006  GETMET	R6	R0	K4
      0x88200105,  //  0007  GETMBR	R8	R0	K5
      0x7C180400,  //  0008  CALL	R6	2
      0x8C1C0106,  //  0009  GETMET	R7	R0	K6
      0x7C1C0200,  //  000A  CALL	R7	1
      0x7C100600,  //  000B  CALL	R4	3
      0x7C080400,  //  000C  CALL	R2	2
      0x80000000,  //  000D  RET	0
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
    be_nested_map(16,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(update_virtual, 1), be_const_closure(class_Matter_Plugin_Light1_update_virtual_closure) },
        { be_const_key_weak(web_values, -1), be_const_closure(class_Matter_Plugin_Light1_web_values_closure) },
        { be_const_key_weak(parse_status, -1), be_const_closure(class_Matter_Plugin_Light1_parse_status_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_Plugin_Light1_init_closure) },
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(Light_X201_X20Dimmer) },
        { be_const_key_weak(update_shadow, 6), be_const_closure(class_Matter_Plugin_Light1_update_shadow_closure) },
        { be_const_key_weak(shadow_bri, 15), be_const_var(0) },
        { be_const_key_weak(UPDATE_COMMANDS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(2,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(Power),
        be_nested_str_weak(Bri),
    }))    ) } )) },
        { be_const_key_weak(invoke_request, 2), be_const_closure(class_Matter_Plugin_Light1_invoke_request_closure) },
        { be_const_key_weak(TYPES, 12), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(257, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(CLUSTERS, 9), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(29, 2), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(10,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(8, 0), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(11,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(2),
        be_const_int(3),
        be_const_int(15),
        be_const_int(17),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(57, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(12,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(3),
        be_const_int(5),
        be_const_int(10),
        be_const_int(15),
        be_const_int(17),
        be_const_int(18),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(3, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(8,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(4, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(7,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(5, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(12,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
        be_const_int(4),
        be_const_int(5),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(6, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(7,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(set_bri, 8), be_const_closure(class_Matter_Plugin_Light1_set_bri_closure) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(class_Matter_Plugin_Light1_read_attribute_closure) },
        { be_const_key_weak(TYPE, 4), be_nested_str_weak(light1) },
        { be_const_key_weak(web_value_dimmer, -1), be_const_closure(class_Matter_Plugin_Light1_web_value_dimmer_closure) },
        { be_const_key_weak(parse_configuration, -1), be_const_closure(class_Matter_Plugin_Light1_parse_configuration_closure) },
    })),
    be_str_weak(Matter_Plugin_Light1)
);
/********************************************************************/
/* End of solidification */
