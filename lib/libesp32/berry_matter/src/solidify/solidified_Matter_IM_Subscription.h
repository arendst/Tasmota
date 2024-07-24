/* Solidification of Matter_IM_Subscription.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_IM_Subscription;

/********************************************************************
** Solidified function: event_published
********************************************************************/
be_local_closure(class_Matter_IM_Subscription_event_published,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(update_event_no),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_weak(event_generators),
    /* K3   */  be_nested_str_weak(event_is_match),
    /* K4   */  be_nested_str_weak(event_no),
    /* K5   */  be_nested_str_weak(add),
    /* K6   */  be_const_int(1),
    }),
    be_str_weak(event_published),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x1C080403,  //  0002  EQ	R2	R2	R3
      0x780A0012,  //  0003  JMPF	R2	#0017
      0x58080001,  //  0004  LDCONST	R2	K1
      0x600C000C,  //  0005  GETGBL	R3	G12
      0x88100102,  //  0006  GETMBR	R4	R0	K2
      0x7C0C0200,  //  0007  CALL	R3	1
      0x140C0403,  //  0008  LT	R3	R2	R3
      0x780E000C,  //  0009  JMPF	R3	#0017
      0x880C0102,  //  000A  GETMBR	R3	R0	K2
      0x940C0602,  //  000B  GETIDX	R3	R3	R2
      0x8C0C0703,  //  000C  GETMET	R3	R3	K3
      0x5C140200,  //  000D  MOVE	R5	R1
      0x7C0C0400,  //  000E  CALL	R3	2
      0x780E0004,  //  000F  JMPF	R3	#0015
      0x880C0304,  //  0010  GETMBR	R3	R1	K4
      0x8C0C0705,  //  0011  GETMET	R3	R3	K5
      0x5415FFFE,  //  0012  LDINT	R5	-1
      0x7C0C0400,  //  0013  CALL	R3	2
      0x90020003,  //  0014  SETMBR	R0	K0	R3
      0x00080506,  //  0015  ADD	R2	R2	K6
      0x7001FFED,  //  0016  JMP		#0005
      0x80000000,  //  0017  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: attribute_updated_ctx
********************************************************************/
be_local_closure(class_Matter_IM_Subscription_attribute_updated_ctx,   /* name */
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
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(path_list),
    /* K2   */  be_nested_str_weak(endpoint),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(attribute),
    /* K5   */  be_nested_str_weak(_add_attribute_unique_path),
    /* K6   */  be_const_int(1),
    }),
    be_str_weak(attribute_updated_ctx),
    &be_const_str_solidified,
    ( &(const binstruction[38]) {  /* code */
      0x580C0000,  //  0000  LDCONST	R3	K0
      0x6010000C,  //  0001  GETGBL	R4	G12
      0x88140101,  //  0002  GETMBR	R5	R0	K1
      0x7C100200,  //  0003  CALL	R4	1
      0x14100604,  //  0004  LT	R4	R3	R4
      0x7812001E,  //  0005  JMPF	R4	#0025
      0x88100101,  //  0006  GETMBR	R4	R0	K1
      0x94100803,  //  0007  GETIDX	R4	R4	R3
      0x88140902,  //  0008  GETMBR	R5	R4	K2
      0x4C180000,  //  0009  LDNIL	R6
      0x1C140A06,  //  000A  EQ	R5	R5	R6
      0x74160003,  //  000B  JMPT	R5	#0010
      0x88140902,  //  000C  GETMBR	R5	R4	K2
      0x88180302,  //  000D  GETMBR	R6	R1	K2
      0x1C140A06,  //  000E  EQ	R5	R5	R6
      0x78160012,  //  000F  JMPF	R5	#0023
      0x88140903,  //  0010  GETMBR	R5	R4	K3
      0x4C180000,  //  0011  LDNIL	R6
      0x1C140A06,  //  0012  EQ	R5	R5	R6
      0x74160003,  //  0013  JMPT	R5	#0018
      0x88140903,  //  0014  GETMBR	R5	R4	K3
      0x88180303,  //  0015  GETMBR	R6	R1	K3
      0x1C140A06,  //  0016  EQ	R5	R5	R6
      0x7816000A,  //  0017  JMPF	R5	#0023
      0x88140904,  //  0018  GETMBR	R5	R4	K4
      0x4C180000,  //  0019  LDNIL	R6
      0x1C140A06,  //  001A  EQ	R5	R5	R6
      0x74160003,  //  001B  JMPT	R5	#0020
      0x88140904,  //  001C  GETMBR	R5	R4	K4
      0x88180304,  //  001D  GETMBR	R6	R1	K4
      0x1C140A06,  //  001E  EQ	R5	R5	R6
      0x78160002,  //  001F  JMPF	R5	#0023
      0x8C140105,  //  0020  GETMET	R5	R0	K5
      0x5C1C0200,  //  0021  MOVE	R7	R1
      0x7C140400,  //  0022  CALL	R5	2
      0x000C0706,  //  0023  ADD	R3	R3	K6
      0x7001FFDB,  //  0024  JMP		#0001
      0x80000000,  //  0025  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: clear_before_arm
********************************************************************/
be_local_closure(class_Matter_IM_Subscription_clear_before_arm,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(updates),
    /* K1   */  be_nested_str_weak(clear),
    /* K2   */  be_nested_str_weak(wait_status),
    /* K3   */  be_nested_str_weak(update_event_no),
    }),
    be_str_weak(clear_before_arm),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x50040200,  //  0003  LDBOOL	R1	1	0
      0x90020401,  //  0004  SETMBR	R0	K2	R1
      0x4C040000,  //  0005  LDNIL	R1
      0x90020601,  //  0006  SETMBR	R0	K3	R1
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_IM_Subscription_init,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[27]) {     /* constants */
    /* K0   */  be_nested_str_weak(subs_shop),
    /* K1   */  be_nested_str_weak(subscription_id),
    /* K2   */  be_nested_str_weak(session),
    /* K3   */  be_nested_str_weak(min_interval_floor),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(min_interval),
    /* K6   */  be_nested_str_weak(max_interval_ceiling),
    /* K7   */  be_nested_str_weak(max_interval),
    /* K8   */  be_nested_str_weak(wait_status),
    /* K9   */  be_nested_str_weak(fabric_filtered),
    /* K10  */  be_nested_str_weak(im),
    /* K11  */  be_nested_str_weak(device),
    /* K12  */  be_nested_str_weak(path_list),
    /* K13  */  be_nested_str_weak(attributes_requests),
    /* K14  */  be_nested_str_weak(push),
    /* K15  */  be_nested_str_weak(matter),
    /* K16  */  be_nested_str_weak(Path),
    /* K17  */  be_nested_str_weak(endpoint),
    /* K18  */  be_nested_str_weak(cluster),
    /* K19  */  be_nested_str_weak(attribute),
    /* K20  */  be_nested_str_weak(stop_iteration),
    /* K21  */  be_nested_str_weak(IM),
    /* K22  */  be_nested_str_weak(parse_event_filters_min_no),
    /* K23  */  be_nested_str_weak(event_filters),
    /* K24  */  be_nested_str_weak(updates),
    /* K25  */  be_nested_str_weak(clear_before_arm),
    /* K26  */  be_nested_str_weak(is_keep_alive),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[69]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x90020202,  //  0001  SETMBR	R0	K1	R2
      0x90020403,  //  0002  SETMBR	R0	K2	R3
      0x88140903,  //  0003  GETMBR	R5	R4	K3
      0x14180B04,  //  0004  LT	R6	R5	K4
      0x781A0000,  //  0005  JMPF	R6	#0007
      0x58140004,  //  0006  LDCONST	R5	K4
      0x541A003B,  //  0007  LDINT	R6	60
      0x24180A06,  //  0008  GT	R6	R5	R6
      0x781A0000,  //  0009  JMPF	R6	#000B
      0x5416003B,  //  000A  LDINT	R5	60
      0x90020A05,  //  000B  SETMBR	R0	K5	R5
      0x88180906,  //  000C  GETMBR	R6	R4	K6
      0x541E003B,  //  000D  LDINT	R7	60
      0x141C0C07,  //  000E  LT	R7	R6	R7
      0x781E0000,  //  000F  JMPF	R7	#0011
      0x541A003B,  //  0010  LDINT	R6	60
      0x541E0E0F,  //  0011  LDINT	R7	3600
      0x241C0C07,  //  0012  GT	R7	R6	R7
      0x781E0000,  //  0013  JMPF	R7	#0015
      0x541A0E0F,  //  0014  LDINT	R6	3600
      0x541A003B,  //  0015  LDINT	R6	60
      0x90020E06,  //  0016  SETMBR	R0	K7	R6
      0x501C0000,  //  0017  LDBOOL	R7	0	0
      0x90021007,  //  0018  SETMBR	R0	K8	R7
      0x881C0909,  //  0019  GETMBR	R7	R4	K9
      0x90021207,  //  001A  SETMBR	R0	K9	R7
      0x881C030A,  //  001B  GETMBR	R7	R1	K10
      0x881C0F0B,  //  001C  GETMBR	R7	R7	K11
      0x60200012,  //  001D  GETGBL	R8	G18
      0x7C200000,  //  001E  CALL	R8	0
      0x90021808,  //  001F  SETMBR	R0	K12	R8
      0x8820090D,  //  0020  GETMBR	R8	R4	K13
      0x4C240000,  //  0021  LDNIL	R9
      0x20201009,  //  0022  NE	R8	R8	R9
      0x78220012,  //  0023  JMPF	R8	#0037
      0x60200010,  //  0024  GETGBL	R8	G16
      0x8824090D,  //  0025  GETMBR	R9	R4	K13
      0x7C200200,  //  0026  CALL	R8	1
      0xA802000B,  //  0027  EXBLK	0	#0034
      0x5C241000,  //  0028  MOVE	R9	R8
      0x7C240000,  //  0029  CALL	R9	0
      0x8828010C,  //  002A  GETMBR	R10	R0	K12
      0x8C28150E,  //  002B  GETMET	R10	R10	K14
      0xB8321E00,  //  002C  GETNGBL	R12	K15
      0x8C301910,  //  002D  GETMET	R12	R12	K16
      0x88381311,  //  002E  GETMBR	R14	R9	K17
      0x883C1312,  //  002F  GETMBR	R15	R9	K18
      0x88401313,  //  0030  GETMBR	R16	R9	K19
      0x7C300800,  //  0031  CALL	R12	4
      0x7C280400,  //  0032  CALL	R10	2
      0x7001FFF3,  //  0033  JMP		#0028
      0x58200014,  //  0034  LDCONST	R8	K20
      0xAC200200,  //  0035  CATCH	R8	1	0
      0xB0080000,  //  0036  RAISE	2	R0	R0
      0xB8221E00,  //  0037  GETNGBL	R8	K15
      0x88201115,  //  0038  GETMBR	R8	R8	K21
      0x8C201116,  //  0039  GETMET	R8	R8	K22
      0x88280917,  //  003A  GETMBR	R10	R4	K23
      0x4C2C0000,  //  003B  LDNIL	R11
      0x7C200600,  //  003C  CALL	R8	3
      0x60240012,  //  003D  GETGBL	R9	G18
      0x7C240000,  //  003E  CALL	R9	0
      0x90023009,  //  003F  SETMBR	R0	K24	R9
      0x8C240119,  //  0040  GETMET	R9	R0	K25
      0x7C240200,  //  0041  CALL	R9	1
      0x50240000,  //  0042  LDBOOL	R9	0	0
      0x90023409,  //  0043  SETMBR	R0	K26	R9
      0x80000000,  //  0044  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _add_attribute_unique_path
********************************************************************/
be_local_closure(class_Matter_IM_Subscription__add_attribute_unique_path,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(updates),
    /* K2   */  be_nested_str_weak(endpoint),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(attribute),
    /* K5   */  be_const_int(1),
    /* K6   */  be_nested_str_weak(push),
    }),
    be_str_weak(_add_attribute_unique_path),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0x600C000C,  //  0001  GETGBL	R3	G12
      0x88100101,  //  0002  GETMBR	R4	R0	K1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x140C0403,  //  0004  LT	R3	R2	R3
      0x780E0010,  //  0005  JMPF	R3	#0017
      0x880C0101,  //  0006  GETMBR	R3	R0	K1
      0x940C0602,  //  0007  GETIDX	R3	R3	R2
      0x88100702,  //  0008  GETMBR	R4	R3	K2
      0x88140302,  //  0009  GETMBR	R5	R1	K2
      0x1C100805,  //  000A  EQ	R4	R4	R5
      0x78120008,  //  000B  JMPF	R4	#0015
      0x88100703,  //  000C  GETMBR	R4	R3	K3
      0x88140303,  //  000D  GETMBR	R5	R1	K3
      0x1C100805,  //  000E  EQ	R4	R4	R5
      0x78120004,  //  000F  JMPF	R4	#0015
      0x88100704,  //  0010  GETMBR	R4	R3	K4
      0x88140304,  //  0011  GETMBR	R5	R1	K4
      0x1C100805,  //  0012  EQ	R4	R4	R5
      0x78120000,  //  0013  JMPF	R4	#0015
      0x80000800,  //  0014  RET	0
      0x00080505,  //  0015  ADD	R2	R2	K5
      0x7001FFE9,  //  0016  JMP		#0001
      0x880C0101,  //  0017  GETMBR	R3	R0	K1
      0x8C0C0706,  //  0018  GETMET	R3	R3	K6
      0x5C140200,  //  0019  MOVE	R5	R1
      0x7C0C0400,  //  001A  CALL	R3	2
      0x80000000,  //  001B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_event_generator_array
********************************************************************/
be_local_closure(class_Matter_IM_Subscription_update_event_generator_array,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(event_generators),
    /* K1   */  be_const_int(0),
    /* K2   */  be_const_int(1),
    /* K3   */  be_nested_str_weak(restart_from),
    /* K4   */  be_nested_str_weak(update_event_no),
    /* K5   */  be_nested_str_weak(resize),
    }),
    be_str_weak(update_event_generator_array),
    &be_const_str_solidified,
    ( &(const binstruction[46]) {  /* code */
      0x6004000C,  //  0000  GETGBL	R1	G12
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x7C040200,  //  0002  CALL	R1	1
      0x1C040301,  //  0003  EQ	R1	R1	K1
      0x78060002,  //  0004  JMPF	R1	#0008
      0x4C040000,  //  0005  LDNIL	R1
      0x80040200,  //  0006  RET	1	R1
      0x70020024,  //  0007  JMP		#002D
      0x6004000C,  //  0008  GETGBL	R1	G12
      0x88080100,  //  0009  GETMBR	R2	R0	K0
      0x7C040200,  //  000A  CALL	R1	1
      0x1C040302,  //  000B  EQ	R1	R1	K2
      0x78060008,  //  000C  JMPF	R1	#0016
      0x88040100,  //  000D  GETMBR	R1	R0	K0
      0x94040301,  //  000E  GETIDX	R1	R1	K1
      0x8C040303,  //  000F  GETMET	R1	R1	K3
      0x880C0104,  //  0010  GETMBR	R3	R0	K4
      0x7C040400,  //  0011  CALL	R1	2
      0x88040100,  //  0012  GETMBR	R1	R0	K0
      0x94040301,  //  0013  GETIDX	R1	R1	K1
      0x80040200,  //  0014  RET	1	R1
      0x70020016,  //  0015  JMP		#002D
      0x60040012,  //  0016  GETGBL	R1	G18
      0x7C040000,  //  0017  CALL	R1	0
      0x8C080305,  //  0018  GETMET	R2	R1	K5
      0x6010000C,  //  0019  GETGBL	R4	G12
      0x88140100,  //  001A  GETMBR	R5	R0	K0
      0x7C100200,  //  001B  CALL	R4	1
      0x7C080400,  //  001C  CALL	R2	2
      0x58080001,  //  001D  LDCONST	R2	K1
      0x600C000C,  //  001E  GETGBL	R3	G12
      0x88100100,  //  001F  GETMBR	R4	R0	K0
      0x7C0C0200,  //  0020  CALL	R3	1
      0x140C0403,  //  0021  LT	R3	R2	R3
      0x780E0009,  //  0022  JMPF	R3	#002D
      0x880C0100,  //  0023  GETMBR	R3	R0	K0
      0x940C0602,  //  0024  GETIDX	R3	R3	R2
      0x8C0C0703,  //  0025  GETMET	R3	R3	K3
      0x88140104,  //  0026  GETMBR	R5	R0	K4
      0x7C0C0400,  //  0027  CALL	R3	2
      0x880C0100,  //  0028  GETMBR	R3	R0	K0
      0x940C0602,  //  0029  GETIDX	R3	R3	R2
      0x98040403,  //  002A  SETIDX	R1	R2	R3
      0x00080502,  //  002B  ADD	R2	R2	K2
      0x7001FFF0,  //  002C  JMP		#001E
      0x80000000,  //  002D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: re_arm
********************************************************************/
be_local_closure(class_Matter_IM_Subscription_re_arm,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(wait_status),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(millis),
    /* K3   */  be_nested_str_weak(expiration),
    /* K4   */  be_nested_str_weak(max_interval),
    /* K5   */  be_nested_str_weak(MAX_INTERVAL_MARGIN),
    /* K6   */  be_nested_str_weak(not_before),
    /* K7   */  be_nested_str_weak(min_interval),
    /* K8   */  be_const_int(1),
    /* K9   */  be_nested_str_weak(is_keep_alive),
    /* K10  */  be_nested_str_weak(log),
    /* K11  */  be_nested_str_weak(MTR_X3A_X20_X2ESub_Done_X20_X20_X28_X20_X20_X20_X20_X20_X20_X29_X20sub_X3D_X25i),
    /* K12  */  be_nested_str_weak(subscription_id),
    /* K13  */  be_const_int(3),
    }),
    be_str_weak(re_arm),
    &be_const_str_solidified,
    ( &(const binstruction[31]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x74060000,  //  0001  JMPT	R1	#0003
      0x80000200,  //  0002  RET	0
      0x50040000,  //  0003  LDBOOL	R1	0	0
      0x90020001,  //  0004  SETMBR	R0	K0	R1
      0xB8060200,  //  0005  GETNGBL	R1	K1
      0x8C040302,  //  0006  GETMET	R1	R1	K2
      0x7C040200,  //  0007  CALL	R1	1
      0x88080104,  //  0008  GETMBR	R2	R0	K4
      0x880C0105,  //  0009  GETMBR	R3	R0	K5
      0x04080403,  //  000A  SUB	R2	R2	R3
      0x540E03E7,  //  000B  LDINT	R3	1000
      0x08080403,  //  000C  MUL	R2	R2	R3
      0x00080202,  //  000D  ADD	R2	R1	R2
      0x90020602,  //  000E  SETMBR	R0	K3	R2
      0x88080107,  //  000F  GETMBR	R2	R0	K7
      0x540E03E7,  //  0010  LDINT	R3	1000
      0x08080403,  //  0011  MUL	R2	R2	R3
      0x00080202,  //  0012  ADD	R2	R1	R2
      0x04080508,  //  0013  SUB	R2	R2	K8
      0x90020C02,  //  0014  SETMBR	R0	K6	R2
      0x88080109,  //  0015  GETMBR	R2	R0	K9
      0x740A0006,  //  0016  JMPT	R2	#001E
      0xB80A1400,  //  0017  GETNGBL	R2	K10
      0x600C0018,  //  0018  GETGBL	R3	G24
      0x5810000B,  //  0019  LDCONST	R4	K11
      0x8814010C,  //  001A  GETMBR	R5	R0	K12
      0x7C0C0400,  //  001B  CALL	R3	2
      0x5810000D,  //  001C  LDCONST	R4	K13
      0x7C080400,  //  001D  CALL	R2	2
      0x80000000,  //  001E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_event_generator_or_arr
********************************************************************/
be_local_closure(class_Matter_IM_Subscription_set_event_generator_or_arr,   /* name */
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
    /* K0   */  be_nested_str_weak(event_generators),
    }),
    be_str_weak(set_event_generator_or_arr),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0003,  //  0002  JMPF	R2	#0007
      0x60080012,  //  0003  GETGBL	R2	G18
      0x7C080000,  //  0004  CALL	R2	0
      0x90020002,  //  0005  SETMBR	R0	K0	R2
      0x7002000A,  //  0006  JMP		#0012
      0x6008000F,  //  0007  GETGBL	R2	G15
      0x5C0C0200,  //  0008  MOVE	R3	R1
      0x60100012,  //  0009  GETGBL	R4	G18
      0x7C080400,  //  000A  CALL	R2	2
      0x780A0001,  //  000B  JMPF	R2	#000E
      0x90020001,  //  000C  SETMBR	R0	K0	R1
      0x70020003,  //  000D  JMP		#0012
      0x60080012,  //  000E  GETGBL	R2	G18
      0x7C080000,  //  000F  CALL	R2	0
      0x400C0401,  //  0010  CONNECT	R3	R2	R1
      0x90020002,  //  0011  SETMBR	R0	K0	R2
      0x80000000,  //  0012  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_self
********************************************************************/
be_local_closure(class_Matter_IM_Subscription_remove_self,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(log),
    /* K1   */  be_nested_str_weak(MTR_X3A_X20_X2DSub_Del_X20_X20_X20_X28_X20_X20_X20_X20_X20_X20_X29_X20sub_X3D),
    /* K2   */  be_nested_str_weak(subscription_id),
    /* K3   */  be_const_int(3),
    /* K4   */  be_nested_str_weak(subs_shop),
    /* K5   */  be_nested_str_weak(remove_sub),
    }),
    be_str_weak(remove_self),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x60080008,  //  0001  GETGBL	R2	G8
      0x880C0102,  //  0002  GETMBR	R3	R0	K2
      0x7C080200,  //  0003  CALL	R2	1
      0x000A0202,  //  0004  ADD	R2	K1	R2
      0x580C0003,  //  0005  LDCONST	R3	K3
      0x7C040400,  //  0006  CALL	R1	2
      0x88040104,  //  0007  GETMBR	R1	R0	K4
      0x8C040305,  //  0008  GETMET	R1	R1	K5
      0x5C0C0000,  //  0009  MOVE	R3	R0
      0x7C040400,  //  000A  CALL	R1	2
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_IM_Subscription
********************************************************************/
be_local_class(Matter_IM_Subscription,
    14,
    NULL,
    be_nested_map(24,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(expiration, 1), be_const_var(9) },
        { be_const_key_weak(remove_self, -1), be_const_closure(class_Matter_IM_Subscription_remove_self_closure) },
        { be_const_key_weak(fabric_filtered, 23), be_const_var(7) },
        { be_const_key_weak(subs_shop, 16), be_const_var(0) },
        { be_const_key_weak(attribute_updated_ctx, -1), be_const_closure(class_Matter_IM_Subscription_attribute_updated_ctx_closure) },
        { be_const_key_weak(update_event_no, -1), be_const_var(13) },
        { be_const_key_weak(MAX_INTERVAL_MARGIN, -1), be_const_int(5) },
        { be_const_key_weak(session, 14), be_const_var(2) },
        { be_const_key_weak(not_before, -1), be_const_var(8) },
        { be_const_key_weak(path_list, 5), be_const_var(3) },
        { be_const_key_weak(event_generators, -1), be_const_var(4) },
        { be_const_key_weak(clear_before_arm, 4), be_const_closure(class_Matter_IM_Subscription_clear_before_arm_closure) },
        { be_const_key_weak(is_keep_alive, 11), be_const_var(11) },
        { be_const_key_weak(updates, 12), be_const_var(12) },
        { be_const_key_weak(wait_status, -1), be_const_var(10) },
        { be_const_key_weak(max_interval, -1), be_const_var(6) },
        { be_const_key_weak(init, 9), be_const_closure(class_Matter_IM_Subscription_init_closure) },
        { be_const_key_weak(min_interval, -1), be_const_var(5) },
        { be_const_key_weak(_add_attribute_unique_path, -1), be_const_closure(class_Matter_IM_Subscription__add_attribute_unique_path_closure) },
        { be_const_key_weak(update_event_generator_array, -1), be_const_closure(class_Matter_IM_Subscription_update_event_generator_array_closure) },
        { be_const_key_weak(subscription_id, -1), be_const_var(1) },
        { be_const_key_weak(re_arm, -1), be_const_closure(class_Matter_IM_Subscription_re_arm_closure) },
        { be_const_key_weak(set_event_generator_or_arr, -1), be_const_closure(class_Matter_IM_Subscription_set_event_generator_or_arr_closure) },
        { be_const_key_weak(event_published, -1), be_const_closure(class_Matter_IM_Subscription_event_published_closure) },
    })),
    be_str_weak(Matter_IM_Subscription)
);

extern const bclass be_class_Matter_IM_Subscription_Shop;

/********************************************************************
** Solidified function: every_50ms
********************************************************************/
be_local_closure(class_Matter_IM_Subscription_Shop_every_50ms,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(subs),
    /* K2   */  be_nested_str_weak(wait_status),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(time_reached),
    /* K5   */  be_nested_str_weak(not_before),
    /* K6   */  be_nested_str_weak(updates),
    /* K7   */  be_nested_str_weak(update_event_no),
    /* K8   */  be_nested_str_weak(im),
    /* K9   */  be_nested_str_weak(send_subscribe_update),
    /* K10  */  be_nested_str_weak(clear_before_arm),
    /* K11  */  be_const_int(1),
    /* K12  */  be_nested_str_weak(expiration),
    /* K13  */  be_nested_str_weak(send_subscribe_heartbeat),
    /* K14  */  be_nested_str_weak(re_arm),
    }),
    be_str_weak(every_50ms),
    &be_const_str_solidified,
    ( &(const binstruction[58]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x6008000C,  //  0001  GETGBL	R2	G12
      0x880C0101,  //  0002  GETMBR	R3	R0	K1
      0x7C080200,  //  0003  CALL	R2	1
      0x14080202,  //  0004  LT	R2	R1	R2
      0x780A0019,  //  0005  JMPF	R2	#0020
      0x88080101,  //  0006  GETMBR	R2	R0	K1
      0x94080401,  //  0007  GETIDX	R2	R2	R1
      0x880C0502,  //  0008  GETMBR	R3	R2	K2
      0x740E0013,  //  0009  JMPT	R3	#001E
      0xB80E0600,  //  000A  GETNGBL	R3	K3
      0x8C0C0704,  //  000B  GETMET	R3	R3	K4
      0x88140505,  //  000C  GETMBR	R5	R2	K5
      0x7C0C0400,  //  000D  CALL	R3	2
      0x780E000E,  //  000E  JMPF	R3	#001E
      0x600C000C,  //  000F  GETGBL	R3	G12
      0x88100506,  //  0010  GETMBR	R4	R2	K6
      0x7C0C0200,  //  0011  CALL	R3	1
      0x240C0700,  //  0012  GT	R3	R3	K0
      0x740E0003,  //  0013  JMPT	R3	#0018
      0x880C0507,  //  0014  GETMBR	R3	R2	K7
      0x4C100000,  //  0015  LDNIL	R4
      0x200C0604,  //  0016  NE	R3	R3	R4
      0x780E0005,  //  0017  JMPF	R3	#001E
      0x880C0108,  //  0018  GETMBR	R3	R0	K8
      0x8C0C0709,  //  0019  GETMET	R3	R3	K9
      0x5C140400,  //  001A  MOVE	R5	R2
      0x7C0C0400,  //  001B  CALL	R3	2
      0x8C0C050A,  //  001C  GETMET	R3	R2	K10
      0x7C0C0200,  //  001D  CALL	R3	1
      0x0004030B,  //  001E  ADD	R1	R1	K11
      0x7001FFE0,  //  001F  JMP		#0001
      0x58040000,  //  0020  LDCONST	R1	K0
      0x6008000C,  //  0021  GETGBL	R2	G12
      0x880C0101,  //  0022  GETMBR	R3	R0	K1
      0x7C080200,  //  0023  CALL	R2	1
      0x14080202,  //  0024  LT	R2	R1	R2
      0x780A0012,  //  0025  JMPF	R2	#0039
      0x88080101,  //  0026  GETMBR	R2	R0	K1
      0x94080401,  //  0027  GETIDX	R2	R2	R1
      0x880C0502,  //  0028  GETMBR	R3	R2	K2
      0x740E000C,  //  0029  JMPT	R3	#0037
      0xB80E0600,  //  002A  GETNGBL	R3	K3
      0x8C0C0704,  //  002B  GETMET	R3	R3	K4
      0x8814050C,  //  002C  GETMBR	R5	R2	K12
      0x7C0C0400,  //  002D  CALL	R3	2
      0x780E0007,  //  002E  JMPF	R3	#0037
      0x880C0108,  //  002F  GETMBR	R3	R0	K8
      0x8C0C070D,  //  0030  GETMET	R3	R3	K13
      0x5C140400,  //  0031  MOVE	R5	R2
      0x7C0C0400,  //  0032  CALL	R3	2
      0x8C0C050A,  //  0033  GETMET	R3	R2	K10
      0x7C0C0200,  //  0034  CALL	R3	1
      0x8C0C050E,  //  0035  GETMET	R3	R2	K14
      0x7C0C0200,  //  0036  CALL	R3	1
      0x0004030B,  //  0037  ADD	R1	R1	K11
      0x7001FFE7,  //  0038  JMP		#0021
      0x80000000,  //  0039  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_IM_Subscription_Shop_init,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(im),
    /* K1   */  be_nested_str_weak(subs),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x60080012,  //  0001  GETGBL	R2	G18
      0x7C080000,  //  0002  CALL	R2	0
      0x90020202,  //  0003  SETMBR	R0	K1	R2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: attribute_updated_ctx
********************************************************************/
be_local_closure(class_Matter_IM_Subscription_Shop_attribute_updated_ctx,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(subs),
    /* K2   */  be_nested_str_weak(attribute_updated_ctx),
    /* K3   */  be_const_int(1),
    }),
    be_str_weak(attribute_updated_ctx),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x580C0000,  //  0000  LDCONST	R3	K0
      0x6010000C,  //  0001  GETGBL	R4	G12
      0x88140101,  //  0002  GETMBR	R5	R0	K1
      0x7C100200,  //  0003  CALL	R4	1
      0x14100604,  //  0004  LT	R4	R3	R4
      0x78120007,  //  0005  JMPF	R4	#000E
      0x88100101,  //  0006  GETMBR	R4	R0	K1
      0x94100803,  //  0007  GETIDX	R4	R4	R3
      0x8C100902,  //  0008  GETMET	R4	R4	K2
      0x5C180200,  //  0009  MOVE	R6	R1
      0x5C1C0400,  //  000A  MOVE	R7	R2
      0x7C100600,  //  000B  CALL	R4	3
      0x000C0703,  //  000C  ADD	R3	R3	K3
      0x7001FFF2,  //  000D  JMP		#0001
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_sub
********************************************************************/
be_local_closure(class_Matter_IM_Subscription_Shop_remove_sub,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(subs),
    /* K2   */  be_nested_str_weak(remove),
    /* K3   */  be_const_int(1),
    }),
    be_str_weak(remove_sub),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0x600C000C,  //  0001  GETGBL	R3	G12
      0x88100101,  //  0002  GETMBR	R4	R0	K1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x140C0403,  //  0004  LT	R3	R2	R3
      0x780E000A,  //  0005  JMPF	R3	#0011
      0x880C0101,  //  0006  GETMBR	R3	R0	K1
      0x940C0602,  //  0007  GETIDX	R3	R3	R2
      0x1C0C0601,  //  0008  EQ	R3	R3	R1
      0x780E0004,  //  0009  JMPF	R3	#000F
      0x880C0101,  //  000A  GETMBR	R3	R0	K1
      0x8C0C0702,  //  000B  GETMET	R3	R3	K2
      0x5C140400,  //  000C  MOVE	R5	R2
      0x7C0C0400,  //  000D  CALL	R3	2
      0x70020000,  //  000E  JMP		#0010
      0x00080503,  //  000F  ADD	R2	R2	K3
      0x7001FFEF,  //  0010  JMP		#0001
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: new_subscription
********************************************************************/
be_local_closure(class_Matter_IM_Subscription_Shop_new_subscription,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(random),
    /* K2   */  be_const_int(2),
    /* K3   */  be_nested_str_weak(get),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(get_by_id),
    /* K6   */  be_nested_str_weak(matter),
    /* K7   */  be_nested_str_weak(IM_Subscription),
    /* K8   */  be_nested_str_weak(subs),
    /* K9   */  be_nested_str_weak(push),
    }),
    be_str_weak(new_subscription),
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0x8C140901,  //  0001  GETMET	R5	R4	K1
      0x581C0002,  //  0002  LDCONST	R7	K2
      0x7C140400,  //  0003  CALL	R5	2
      0x8C140B03,  //  0004  GETMET	R5	R5	K3
      0x581C0004,  //  0005  LDCONST	R7	K4
      0x58200002,  //  0006  LDCONST	R8	K2
      0x7C140600,  //  0007  CALL	R5	3
      0x8C180105,  //  0008  GETMET	R6	R0	K5
      0x5C200A00,  //  0009  MOVE	R8	R5
      0x7C180400,  //  000A  CALL	R6	2
      0x781A0008,  //  000B  JMPF	R6	#0015
      0x8C180901,  //  000C  GETMET	R6	R4	K1
      0x58200002,  //  000D  LDCONST	R8	K2
      0x7C180400,  //  000E  CALL	R6	2
      0x8C180D03,  //  000F  GETMET	R6	R6	K3
      0x58200004,  //  0010  LDCONST	R8	K4
      0x58240002,  //  0011  LDCONST	R9	K2
      0x7C180600,  //  0012  CALL	R6	3
      0x5C140C00,  //  0013  MOVE	R5	R6
      0x7001FFF2,  //  0014  JMP		#0008
      0xB81A0C00,  //  0015  GETNGBL	R6	K6
      0x8C180D07,  //  0016  GETMET	R6	R6	K7
      0x5C200000,  //  0017  MOVE	R8	R0
      0x5C240A00,  //  0018  MOVE	R9	R5
      0x5C280200,  //  0019  MOVE	R10	R1
      0x5C2C0400,  //  001A  MOVE	R11	R2
      0x5C300600,  //  001B  MOVE	R12	R3
      0x7C180C00,  //  001C  CALL	R6	6
      0x881C0108,  //  001D  GETMBR	R7	R0	K8
      0x8C1C0F09,  //  001E  GETMET	R7	R7	K9
      0x5C240C00,  //  001F  MOVE	R9	R6
      0x7C1C0400,  //  0020  CALL	R7	2
      0x80040C00,  //  0021  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_published
********************************************************************/
be_local_closure(class_Matter_IM_Subscription_Shop_event_published,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(subs),
    /* K2   */  be_nested_str_weak(event_published),
    /* K3   */  be_const_int(1),
    }),
    be_str_weak(event_published),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0x600C000C,  //  0001  GETGBL	R3	G12
      0x88100101,  //  0002  GETMBR	R4	R0	K1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x140C0403,  //  0004  LT	R3	R2	R3
      0x780E0006,  //  0005  JMPF	R3	#000D
      0x880C0101,  //  0006  GETMBR	R3	R0	K1
      0x940C0602,  //  0007  GETIDX	R3	R3	R2
      0x8C0C0702,  //  0008  GETMET	R3	R3	K2
      0x5C140200,  //  0009  MOVE	R5	R1
      0x7C0C0400,  //  000A  CALL	R3	2
      0x00080503,  //  000B  ADD	R2	R2	K3
      0x7001FFF3,  //  000C  JMP		#0001
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_by_session
********************************************************************/
be_local_closure(class_Matter_IM_Subscription_Shop_remove_by_session,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(subs),
    /* K2   */  be_nested_str_weak(session),
    /* K3   */  be_nested_str_weak(remove),
    /* K4   */  be_const_int(1),
    }),
    be_str_weak(remove_by_session),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0x600C000C,  //  0001  GETGBL	R3	G12
      0x88100101,  //  0002  GETMBR	R4	R0	K1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x140C0403,  //  0004  LT	R3	R2	R3
      0x780E000B,  //  0005  JMPF	R3	#0012
      0x880C0101,  //  0006  GETMBR	R3	R0	K1
      0x940C0602,  //  0007  GETIDX	R3	R3	R2
      0x880C0702,  //  0008  GETMBR	R3	R3	K2
      0x1C0C0601,  //  0009  EQ	R3	R3	R1
      0x780E0004,  //  000A  JMPF	R3	#0010
      0x880C0101,  //  000B  GETMBR	R3	R0	K1
      0x8C0C0703,  //  000C  GETMET	R3	R3	K3
      0x5C140400,  //  000D  MOVE	R5	R2
      0x7C0C0400,  //  000E  CALL	R3	2
      0x70020000,  //  000F  JMP		#0011
      0x00080504,  //  0010  ADD	R2	R2	K4
      0x7001FFEE,  //  0011  JMP		#0001
      0x80000000,  //  0012  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_by_fabric
********************************************************************/
be_local_closure(class_Matter_IM_Subscription_Shop_remove_by_fabric,   /* name */
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
    /* K0   */  be_nested_str_weak(_sessions),
    /* K1   */  be_nested_str_weak(remove_by_session),
    /* K2   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(remove_by_fabric),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x60080010,  //  0000  GETGBL	R2	G16
      0x880C0300,  //  0001  GETMBR	R3	R1	K0
      0x7C080200,  //  0002  CALL	R2	1
      0xA8020005,  //  0003  EXBLK	0	#000A
      0x5C0C0400,  //  0004  MOVE	R3	R2
      0x7C0C0000,  //  0005  CALL	R3	0
      0x8C100101,  //  0006  GETMET	R4	R0	K1
      0x5C180600,  //  0007  MOVE	R6	R3
      0x7C100400,  //  0008  CALL	R4	2
      0x7001FFF9,  //  0009  JMP		#0004
      0x58080002,  //  000A  LDCONST	R2	K2
      0xAC080200,  //  000B  CATCH	R2	1	0
      0xB0080000,  //  000C  RAISE	2	R0	R0
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_by_id
********************************************************************/
be_local_closure(class_Matter_IM_Subscription_Shop_get_by_id,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(subs),
    /* K2   */  be_nested_str_weak(subscription_id),
    /* K3   */  be_const_int(1),
    }),
    be_str_weak(get_by_id),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0x600C000C,  //  0001  GETGBL	R3	G12
      0x88100101,  //  0002  GETMBR	R4	R0	K1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x140C0403,  //  0004  LT	R3	R2	R3
      0x780E0009,  //  0005  JMPF	R3	#0010
      0x880C0101,  //  0006  GETMBR	R3	R0	K1
      0x940C0602,  //  0007  GETIDX	R3	R3	R2
      0x880C0702,  //  0008  GETMBR	R3	R3	K2
      0x1C0C0601,  //  0009  EQ	R3	R3	R1
      0x780E0002,  //  000A  JMPF	R3	#000E
      0x880C0101,  //  000B  GETMBR	R3	R0	K1
      0x940C0602,  //  000C  GETIDX	R3	R3	R2
      0x80040600,  //  000D  RET	1	R3
      0x00080503,  //  000E  ADD	R2	R2	K3
      0x7001FFF0,  //  000F  JMP		#0001
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_IM_Subscription_Shop
********************************************************************/
be_local_class(Matter_IM_Subscription_Shop,
    2,
    NULL,
    be_nested_map(11,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(every_50ms, -1), be_const_closure(class_Matter_IM_Subscription_Shop_every_50ms_closure) },
        { be_const_key_weak(get_by_id, -1), be_const_closure(class_Matter_IM_Subscription_Shop_get_by_id_closure) },
        { be_const_key_weak(attribute_updated_ctx, -1), be_const_closure(class_Matter_IM_Subscription_Shop_attribute_updated_ctx_closure) },
        { be_const_key_weak(remove_sub, -1), be_const_closure(class_Matter_IM_Subscription_Shop_remove_sub_closure) },
        { be_const_key_weak(remove_by_fabric, -1), be_const_closure(class_Matter_IM_Subscription_Shop_remove_by_fabric_closure) },
        { be_const_key_weak(new_subscription, -1), be_const_closure(class_Matter_IM_Subscription_Shop_new_subscription_closure) },
        { be_const_key_weak(event_published, -1), be_const_closure(class_Matter_IM_Subscription_Shop_event_published_closure) },
        { be_const_key_weak(subs, -1), be_const_var(0) },
        { be_const_key_weak(remove_by_session, -1), be_const_closure(class_Matter_IM_Subscription_Shop_remove_by_session_closure) },
        { be_const_key_weak(init, 4), be_const_closure(class_Matter_IM_Subscription_Shop_init_closure) },
        { be_const_key_weak(im, 1), be_const_var(1) },
    })),
    be_str_weak(Matter_IM_Subscription_Shop)
);
/********************************************************************/
/* End of solidification */
