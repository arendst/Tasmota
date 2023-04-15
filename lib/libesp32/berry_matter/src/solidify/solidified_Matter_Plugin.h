/* Solidification of Matter_Plugin.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin;

/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_read_attribute,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(attribute),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(Matter_TLV_array),
    /* K6   */  be_nested_str_weak(TYPES),
    /* K7   */  be_nested_str_weak(keys),
    /* K8   */  be_nested_str_weak(add_struct),
    /* K9   */  be_nested_str_weak(add_TLV),
    /* K10  */  be_nested_str_weak(U2),
    /* K11  */  be_const_int(1),
    /* K12  */  be_nested_str_weak(stop_iteration),
    /* K13  */  be_nested_str_weak(get_cluster_list),
    /* K14  */  be_nested_str_weak(U4),
    /* K15  */  be_const_int(2),
    /* K16  */  be_const_int(3),
    /* K17  */  be_nested_str_weak(create_TLV),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[93]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x880C0701,  //  0001  GETMBR	R3	R3	K1
      0x88100502,  //  0002  GETMBR	R4	R2	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x541A001C,  //  0004  LDINT	R6	29
      0x1C180806,  //  0005  EQ	R6	R4	R6
      0x781A0052,  //  0006  JMPF	R6	#005A
      0x1C180B04,  //  0007  EQ	R6	R5	K4
      0x781A001C,  //  0008  JMPF	R6	#0026
      0x8C180705,  //  0009  GETMET	R6	R3	K5
      0x7C180200,  //  000A  CALL	R6	1
      0x601C0010,  //  000B  GETGBL	R7	G16
      0x88200106,  //  000C  GETMBR	R8	R0	K6
      0x8C201107,  //  000D  GETMET	R8	R8	K7
      0x7C200200,  //  000E  CALL	R8	1
      0x7C1C0200,  //  000F  CALL	R7	1
      0xA802000F,  //  0010  EXBLK	0	#0021
      0x5C200E00,  //  0011  MOVE	R8	R7
      0x7C200000,  //  0012  CALL	R8	0
      0x8C240D08,  //  0013  GETMET	R9	R6	K8
      0x7C240200,  //  0014  CALL	R9	1
      0x8C281309,  //  0015  GETMET	R10	R9	K9
      0x58300004,  //  0016  LDCONST	R12	K4
      0x8834070A,  //  0017  GETMBR	R13	R3	K10
      0x5C381000,  //  0018  MOVE	R14	R8
      0x7C280800,  //  0019  CALL	R10	4
      0x8C281309,  //  001A  GETMET	R10	R9	K9
      0x5830000B,  //  001B  LDCONST	R12	K11
      0x8834070A,  //  001C  GETMBR	R13	R3	K10
      0x88380106,  //  001D  GETMBR	R14	R0	K6
      0x94381C08,  //  001E  GETIDX	R14	R14	R8
      0x7C280800,  //  001F  CALL	R10	4
      0x7001FFEF,  //  0020  JMP		#0011
      0x581C000C,  //  0021  LDCONST	R7	K12
      0xAC1C0200,  //  0022  CATCH	R7	1	0
      0xB0080000,  //  0023  RAISE	2	R0	R0
      0x80040C00,  //  0024  RET	1	R6
      0x70020032,  //  0025  JMP		#0059
      0x1C180B0B,  //  0026  EQ	R6	R5	K11
      0x781A0013,  //  0027  JMPF	R6	#003C
      0x8C180705,  //  0028  GETMET	R6	R3	K5
      0x7C180200,  //  0029  CALL	R6	1
      0x601C0010,  //  002A  GETGBL	R7	G16
      0x8C20010D,  //  002B  GETMET	R8	R0	K13
      0x7C200200,  //  002C  CALL	R8	1
      0x7C1C0200,  //  002D  CALL	R7	1
      0xA8020007,  //  002E  EXBLK	0	#0037
      0x5C200E00,  //  002F  MOVE	R8	R7
      0x7C200000,  //  0030  CALL	R8	0
      0x8C240D09,  //  0031  GETMET	R9	R6	K9
      0x4C2C0000,  //  0032  LDNIL	R11
      0x8830070E,  //  0033  GETMBR	R12	R3	K14
      0x5C341000,  //  0034  MOVE	R13	R8
      0x7C240800,  //  0035  CALL	R9	4
      0x7001FFF7,  //  0036  JMP		#002F
      0x581C000C,  //  0037  LDCONST	R7	K12
      0xAC1C0200,  //  0038  CATCH	R7	1	0
      0xB0080000,  //  0039  RAISE	2	R0	R0
      0x80040C00,  //  003A  RET	1	R6
      0x7002001C,  //  003B  JMP		#0059
      0x1C180B0F,  //  003C  EQ	R6	R5	K15
      0x781A0003,  //  003D  JMPF	R6	#0042
      0x8C180705,  //  003E  GETMET	R6	R3	K5
      0x7C180200,  //  003F  CALL	R6	1
      0x80040C00,  //  0040  RET	1	R6
      0x70020016,  //  0041  JMP		#0059
      0x1C180B10,  //  0042  EQ	R6	R5	K16
      0x781A0003,  //  0043  JMPF	R6	#0048
      0x8C180705,  //  0044  GETMET	R6	R3	K5
      0x7C180200,  //  0045  CALL	R6	1
      0x80040C00,  //  0046  RET	1	R6
      0x70020010,  //  0047  JMP		#0059
      0x541AFFFB,  //  0048  LDINT	R6	65532
      0x1C180A06,  //  0049  EQ	R6	R5	R6
      0x781A0005,  //  004A  JMPF	R6	#0051
      0x8C180711,  //  004B  GETMET	R6	R3	K17
      0x8820070E,  //  004C  GETMBR	R8	R3	K14
      0x58240004,  //  004D  LDCONST	R9	K4
      0x7C180600,  //  004E  CALL	R6	3
      0x80040C00,  //  004F  RET	1	R6
      0x70020007,  //  0050  JMP		#0059
      0x541AFFFC,  //  0051  LDINT	R6	65533
      0x1C180A06,  //  0052  EQ	R6	R5	R6
      0x781A0004,  //  0053  JMPF	R6	#0059
      0x8C180711,  //  0054  GETMET	R6	R3	K17
      0x8820070E,  //  0055  GETMBR	R8	R3	K14
      0x5824000B,  //  0056  LDCONST	R9	K11
      0x7C180600,  //  0057  CALL	R6	3
      0x80040C00,  //  0058  RET	1	R6
      0x70020001,  //  0059  JMP		#005C
      0x4C180000,  //  005A  LDNIL	R6
      0x80040C00,  //  005B  RET	1	R6
      0x80000000,  //  005C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: has
********************************************************************/
be_local_closure(Matter_Plugin_has,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(clusters),
    /* K1   */  be_nested_str_weak(contains),
    /* K2   */  be_nested_str_weak(endpoints),
    /* K3   */  be_nested_str_weak(find),
    }),
    be_str_weak(has),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x780E0006,  //  0004  JMPF	R3	#000C
      0x880C0102,  //  0005  GETMBR	R3	R0	K2
      0x8C0C0703,  //  0006  GETMET	R3	R3	K3
      0x5C140400,  //  0007  MOVE	R5	R2
      0x7C0C0400,  //  0008  CALL	R3	2
      0x4C100000,  //  0009  LDNIL	R4
      0x200C0604,  //  000A  NE	R3	R3	R4
      0x740E0000,  //  000B  JMPT	R3	#000D
      0x500C0001,  //  000C  LDBOOL	R3	0	1
      0x500C0200,  //  000D  LDBOOL	R3	1	0
      0x80040600,  //  000E  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: consolidate_clusters
********************************************************************/
be_local_closure(Matter_Plugin_consolidate_clusters,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        3,                          /* nstack */
        1,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        0,                          /* has constants */
        NULL,                       /* no const */
        be_str_weak(real_super),
        &be_const_str_solidified,
        ( &(const binstruction[ 4]) {  /* code */
          0x60040003,  //  0000  GETGBL	R1	G3
          0x5C080000,  //  0001  MOVE	R2	R0
          0x7C040200,  //  0002  CALL	R1	1
          0x80040200,  //  0003  RET	1	R1
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(CLUSTERS),
    /* K1   */  be_nested_str_weak(keys),
    /* K2   */  be_nested_str_weak(contains),
    /* K3   */  be_nested_str_weak(find),
    /* K4   */  be_nested_str_weak(push),
    /* K5   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(consolidate_clusters),
    &be_const_str_solidified,
    ( &(const binstruction[53]) {  /* code */
      0x84040000,  //  0000  CLOSURE	R1	P0
      0x60080013,  //  0001  GETGBL	R2	G19
      0x7C080000,  //  0002  CALL	R2	0
      0x5C0C0000,  //  0003  MOVE	R3	R0
      0x4C100000,  //  0004  LDNIL	R4
      0x20100604,  //  0005  NE	R4	R3	R4
      0x7812002C,  //  0006  JMPF	R4	#0034
      0x88100700,  //  0007  GETMBR	R4	R3	K0
      0x60140010,  //  0008  GETGBL	R5	G16
      0x8C180901,  //  0009  GETMET	R6	R4	K1
      0x7C180200,  //  000A  CALL	R6	1
      0x7C140200,  //  000B  CALL	R5	1
      0xA802001E,  //  000C  EXBLK	0	#002C
      0x5C180A00,  //  000D  MOVE	R6	R5
      0x7C180000,  //  000E  CALL	R6	0
      0x8C1C0502,  //  000F  GETMET	R7	R2	K2
      0x5C240C00,  //  0010  MOVE	R9	R6
      0x7C1C0400,  //  0011  CALL	R7	2
      0x741E0002,  //  0012  JMPT	R7	#0016
      0x601C0012,  //  0013  GETGBL	R7	G18
      0x7C1C0000,  //  0014  CALL	R7	0
      0x98080C07,  //  0015  SETIDX	R2	R6	R7
      0x601C0010,  //  0016  GETGBL	R7	G16
      0x94200806,  //  0017  GETIDX	R8	R4	R6
      0x7C1C0200,  //  0018  CALL	R7	1
      0xA802000D,  //  0019  EXBLK	0	#0028
      0x5C200E00,  //  001A  MOVE	R8	R7
      0x7C200000,  //  001B  CALL	R8	0
      0x94240406,  //  001C  GETIDX	R9	R2	R6
      0x8C241303,  //  001D  GETMET	R9	R9	K3
      0x5C2C1000,  //  001E  MOVE	R11	R8
      0x7C240400,  //  001F  CALL	R9	2
      0x4C280000,  //  0020  LDNIL	R10
      0x1C24120A,  //  0021  EQ	R9	R9	R10
      0x78260003,  //  0022  JMPF	R9	#0027
      0x94240406,  //  0023  GETIDX	R9	R2	R6
      0x8C241304,  //  0024  GETMET	R9	R9	K4
      0x5C2C1000,  //  0025  MOVE	R11	R8
      0x7C240400,  //  0026  CALL	R9	2
      0x7001FFF1,  //  0027  JMP		#001A
      0x581C0005,  //  0028  LDCONST	R7	K5
      0xAC1C0200,  //  0029  CATCH	R7	1	0
      0xB0080000,  //  002A  RAISE	2	R0	R0
      0x7001FFE0,  //  002B  JMP		#000D
      0x58140005,  //  002C  LDCONST	R5	K5
      0xAC140200,  //  002D  CATCH	R5	1	0
      0xB0080000,  //  002E  RAISE	2	R0	R0
      0x5C140200,  //  002F  MOVE	R5	R1
      0x5C180600,  //  0030  MOVE	R6	R3
      0x7C140200,  //  0031  CALL	R5	1
      0x5C0C0A00,  //  0032  MOVE	R3	R5
      0x7001FFCF,  //  0033  JMP		#0004
      0x80040400,  //  0034  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(Matter_Plugin_every_second,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(update_shadow),
    }),
    be_str_weak(every_second),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: subscribe_attribute
********************************************************************/
be_local_closure(Matter_Plugin_subscribe_attribute,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(subscribe_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x4C140000,  //  0000  LDNIL	R5
      0x80040A00,  //  0001  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: subscribe_event
********************************************************************/
be_local_closure(Matter_Plugin_subscribe_event,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(subscribe_event),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x4C140000,  //  0000  LDNIL	R5
      0x80040A00,  //  0001  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_shadow
********************************************************************/
be_local_closure(Matter_Plugin_update_shadow,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(update_shadow),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_event
********************************************************************/
be_local_closure(Matter_Plugin_read_event,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(read_event),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x4C140000,  //  0000  LDNIL	R5
      0x80040A00,  //  0001  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_endpoint
********************************************************************/
be_local_closure(Matter_Plugin_get_endpoint,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(endpoint),
    }),
    be_str_weak(get_endpoint),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: write_attribute
********************************************************************/
be_local_closure(Matter_Plugin_write_attribute,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(write_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x4C100000,  //  0000  LDNIL	R4
      0x80040800,  //  0001  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(endpoint),
    /* K2   */  be_nested_str_weak(clusters),
    /* K3   */  be_nested_str_weak(consolidate_clusters),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x90020202,  //  0001  SETMBR	R0	K1	R2
      0x8C0C0103,  //  0002  GETMET	R3	R0	K3
      0x7C0C0200,  //  0003  CALL	R3	1
      0x90020403,  //  0004  SETMBR	R0	K2	R3
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: timed_request
********************************************************************/
be_local_closure(Matter_Plugin_timed_request,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(timed_request),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x4C100000,  //  0000  LDNIL	R4
      0x80040800,  //  0001  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_attribute_list
********************************************************************/
be_local_closure(Matter_Plugin_get_attribute_list,   /* name */
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
    /* K0   */  be_nested_str_weak(clusters),
    /* K1   */  be_nested_str_weak(find),
    }),
    be_str_weak(get_attribute_list),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x5C140400,  //  0002  MOVE	R5	R2
      0x60180012,  //  0003  GETGBL	R6	G18
      0x7C180000,  //  0004  CALL	R6	0
      0x7C0C0600,  //  0005  CALL	R3	3
      0x80040600,  //  0006  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_cluster_list
********************************************************************/
be_local_closure(Matter_Plugin_get_cluster_list,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(clusters),
    /* K1   */  be_nested_str_weak(keys),
    /* K2   */  be_nested_str_weak(push),
    /* K3   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(get_cluster_list),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x60080012,  //  0000  GETGBL	R2	G18
      0x7C080000,  //  0001  CALL	R2	0
      0x600C0010,  //  0002  GETGBL	R3	G16
      0x88100100,  //  0003  GETMBR	R4	R0	K0
      0x8C100901,  //  0004  GETMET	R4	R4	K1
      0x7C100200,  //  0005  CALL	R4	1
      0x7C0C0200,  //  0006  CALL	R3	1
      0xA8020005,  //  0007  EXBLK	0	#000E
      0x5C100600,  //  0008  MOVE	R4	R3
      0x7C100000,  //  0009  CALL	R4	0
      0x8C140502,  //  000A  GETMET	R5	R2	K2
      0x5C1C0800,  //  000B  MOVE	R7	R4
      0x7C140400,  //  000C  CALL	R5	2
      0x7001FFF9,  //  000D  JMP		#0008
      0x580C0003,  //  000E  LDCONST	R3	K3
      0xAC0C0200,  //  000F  CATCH	R3	1	0
      0xB0080000,  //  0010  RAISE	2	R0	R0
      0x80040400,  //  0011  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_sensors
********************************************************************/
be_local_closure(Matter_Plugin_parse_sensors,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(parse_sensors),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Plugin_invoke_request,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x4C100000,  //  0000  LDNIL	R4
      0x80040800,  //  0001  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: attribute_updated
********************************************************************/
be_local_closure(Matter_Plugin_attribute_updated,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(endpoint),
    /* K1   */  be_nested_str_weak(device),
    /* K2   */  be_nested_str_weak(attribute_updated),
    }),
    be_str_weak(attribute_updated),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x4C140000,  //  0000  LDNIL	R5
      0x1C140205,  //  0001  EQ	R5	R1	R5
      0x78160000,  //  0002  JMPF	R5	#0004
      0x88040100,  //  0003  GETMBR	R1	R0	K0
      0x88140101,  //  0004  GETMBR	R5	R0	K1
      0x8C140B02,  //  0005  GETMET	R5	R5	K2
      0x5C1C0200,  //  0006  MOVE	R7	R1
      0x5C200400,  //  0007  MOVE	R8	R2
      0x5C240600,  //  0008  MOVE	R9	R3
      0x5C280800,  //  0009  MOVE	R10	R4
      0x7C140A00,  //  000A  CALL	R5	5
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin
********************************************************************/
be_local_class(Matter_Plugin,
    3,
    NULL,
    be_nested_map(21,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(read_attribute, -1), be_const_closure(Matter_Plugin_read_attribute_closure) },
        { be_const_key_weak(attribute_updated, 18), be_const_closure(Matter_Plugin_attribute_updated_closure) },
        { be_const_key_weak(consolidate_clusters, -1), be_const_closure(Matter_Plugin_consolidate_clusters_closure) },
        { be_const_key_weak(every_second, -1), be_const_closure(Matter_Plugin_every_second_closure) },
        { be_const_key_weak(subscribe_attribute, 1), be_const_closure(Matter_Plugin_subscribe_attribute_closure) },
        { be_const_key_weak(subscribe_event, -1), be_const_closure(Matter_Plugin_subscribe_event_closure) },
        { be_const_key_weak(device, -1), be_const_var(0) },
        { be_const_key_weak(update_shadow, 17), be_const_closure(Matter_Plugin_update_shadow_closure) },
        { be_const_key_weak(get_endpoint, -1), be_const_closure(Matter_Plugin_get_endpoint_closure) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(Matter_Plugin_invoke_request_closure) },
        { be_const_key_weak(has, 8), be_const_closure(Matter_Plugin_has_closure) },
        { be_const_key_weak(parse_sensors, 13), be_const_closure(Matter_Plugin_parse_sensors_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_init_closure) },
        { be_const_key_weak(get_attribute_list, -1), be_const_closure(Matter_Plugin_get_attribute_list_closure) },
        { be_const_key_weak(timed_request, 9), be_const_closure(Matter_Plugin_timed_request_closure) },
        { be_const_key_weak(read_event, 11), be_const_closure(Matter_Plugin_read_event_closure) },
        { be_const_key_weak(get_cluster_list, -1), be_const_closure(Matter_Plugin_get_cluster_list_closure) },
        { be_const_key_weak(CLUSTERS, 20), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(29, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(6,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(write_attribute, -1), be_const_closure(Matter_Plugin_write_attribute_closure) },
        { be_const_key_weak(clusters, -1), be_const_var(2) },
        { be_const_key_weak(endpoint, -1), be_const_var(1) },
    })),
    be_str_weak(Matter_Plugin)
);
/*******************************************************************/

void be_load_Matter_Plugin_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin);
    be_setglobal(vm, "Matter_Plugin");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
