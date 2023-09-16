/* Solidification of Matter_Plugin_0.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin;

/********************************************************************
** Solidified function: ack_request
********************************************************************/
be_local_closure(Matter_Plugin_ack_request,   /* name */
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
    /* K0   */  be_nested_str_weak(msg),
    /* K1   */  be_nested_str_weak(device),
    /* K2   */  be_nested_str_weak(message_handler),
    /* K3   */  be_nested_str_weak(im),
    /* K4   */  be_nested_str_weak(send_ack_now),
    }),
    be_str_weak(ack_request),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x88080300,  //  0000  GETMBR	R2	R1	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x200C0403,  //  0002  NE	R3	R2	R3
      0x780E0005,  //  0003  JMPF	R3	#000A
      0x880C0101,  //  0004  GETMBR	R3	R0	K1
      0x880C0702,  //  0005  GETMBR	R3	R3	K2
      0x880C0703,  //  0006  GETMBR	R3	R3	K3
      0x8C0C0704,  //  0007  GETMET	R3	R3	K4
      0x5C140400,  //  0008  MOVE	R5	R2
      0x7C0C0400,  //  0009  CALL	R3	2
      0x4C0C0000,  //  000A  LDNIL	R3
      0x90060003,  //  000B  SETMBR	R1	K0	R3
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: consolidate_clusters
********************************************************************/
be_local_closure(Matter_Plugin_consolidate_clusters,   /* name */
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
    /* K0   */  be_nested_str_weak(CLUSTERS),
    }),
    be_str_weak(consolidate_clusters),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_shadow
********************************************************************/
be_local_closure(Matter_Plugin_update_shadow,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(tick),
    /* K1   */  be_nested_str_weak(device),
    }),
    be_str_weak(update_shadow),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040101,  //  0000  GETMBR	R1	R0	K1
      0x88040300,  //  0001  GETMBR	R1	R1	K0
      0x90020001,  //  0002  SETMBR	R0	K0	R1
      0x80000000,  //  0003  RET	0
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
** Solidified function: consolidate_update_commands
********************************************************************/
be_local_closure(Matter_Plugin_consolidate_update_commands,   /* name */
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
    /* K0   */  be_nested_str_weak(UPDATE_COMMANDS),
    }),
    be_str_weak(consolidate_update_commands),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: publish_command
********************************************************************/
be_local_closure(Matter_Plugin_publish_command,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    7,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(json),
    /* K1   */  be_nested_str_weak(_X25s_X3A_X25s),
    /* K2   */  be_nested_str_weak(dump),
    /* K3   */  be_nested_str_weak(_X25s_X2C_X25s_X3A_X25s),
    /* K4   */  be_nested_str_weak(matter),
    /* K5   */  be_nested_str_weak(publish_command),
    /* K6   */  be_nested_str_weak(MtrReceived),
    /* K7   */  be_nested_str_weak(endpoint),
    /* K8   */  be_nested_str_weak(node_label),
    }),
    be_str_weak(publish_command),
    &be_const_str_solidified,
    ( &(const binstruction[46]) {  /* code */
      0xA41E0000,  //  0000  IMPORT	R7	K0
      0x60200018,  //  0001  GETGBL	R8	G24
      0x58240001,  //  0002  LDCONST	R9	K1
      0x8C280F02,  //  0003  GETMET	R10	R7	K2
      0x5C300200,  //  0004  MOVE	R12	R1
      0x7C280400,  //  0005  CALL	R10	2
      0x8C2C0F02,  //  0006  GETMET	R11	R7	K2
      0x5C340400,  //  0007  MOVE	R13	R2
      0x7C2C0400,  //  0008  CALL	R11	2
      0x7C200600,  //  0009  CALL	R8	3
      0x4C240000,  //  000A  LDNIL	R9
      0x20240609,  //  000B  NE	R9	R3	R9
      0x7826000A,  //  000C  JMPF	R9	#0018
      0x60240018,  //  000D  GETGBL	R9	G24
      0x58280003,  //  000E  LDCONST	R10	K3
      0x5C2C1000,  //  000F  MOVE	R11	R8
      0x8C300F02,  //  0010  GETMET	R12	R7	K2
      0x5C380600,  //  0011  MOVE	R14	R3
      0x7C300400,  //  0012  CALL	R12	2
      0x8C340F02,  //  0013  GETMET	R13	R7	K2
      0x5C3C0800,  //  0014  MOVE	R15	R4
      0x7C340400,  //  0015  CALL	R13	2
      0x7C240800,  //  0016  CALL	R9	4
      0x5C201200,  //  0017  MOVE	R8	R9
      0x4C240000,  //  0018  LDNIL	R9
      0x20240A09,  //  0019  NE	R9	R5	R9
      0x7826000A,  //  001A  JMPF	R9	#0026
      0x60240018,  //  001B  GETGBL	R9	G24
      0x58280003,  //  001C  LDCONST	R10	K3
      0x5C2C1000,  //  001D  MOVE	R11	R8
      0x8C300F02,  //  001E  GETMET	R12	R7	K2
      0x5C380A00,  //  001F  MOVE	R14	R5
      0x7C300400,  //  0020  CALL	R12	2
      0x8C340F02,  //  0021  GETMET	R13	R7	K2
      0x5C3C0C00,  //  0022  MOVE	R15	R6
      0x7C340400,  //  0023  CALL	R13	2
      0x7C240800,  //  0024  CALL	R9	4
      0x5C201200,  //  0025  MOVE	R8	R9
      0xB8260800,  //  0026  GETNGBL	R9	K4
      0x8C241305,  //  0027  GETMET	R9	R9	K5
      0x582C0006,  //  0028  LDCONST	R11	K6
      0x88300107,  //  0029  GETMBR	R12	R0	K7
      0x88340108,  //  002A  GETMBR	R13	R0	K8
      0x5C381000,  //  002B  MOVE	R14	R8
      0x7C240A00,  //  002C  CALL	R9	5
      0x80000000,  //  002D  RET	0
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
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_read_attribute,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[20]) {     /* constants */
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
    /* K17  */  be_nested_str_weak(set),
    /* K18  */  be_nested_str_weak(BOOL),
    /* K19  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[114]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140502,  //  0002  GETMBR	R5	R2	K2
      0x88180503,  //  0003  GETMBR	R6	R2	K3
      0x541E001C,  //  0004  LDINT	R7	29
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E0051,  //  0006  JMPF	R7	#0059
      0x1C1C0D04,  //  0007  EQ	R7	R6	K4
      0x781E001B,  //  0008  JMPF	R7	#0025
      0x8C1C0905,  //  0009  GETMET	R7	R4	K5
      0x7C1C0200,  //  000A  CALL	R7	1
      0x88200106,  //  000B  GETMBR	R8	R0	K6
      0x60240010,  //  000C  GETGBL	R9	G16
      0x8C281107,  //  000D  GETMET	R10	R8	K7
      0x7C280200,  //  000E  CALL	R10	1
      0x7C240200,  //  000F  CALL	R9	1
      0xA802000E,  //  0010  EXBLK	0	#0020
      0x5C281200,  //  0011  MOVE	R10	R9
      0x7C280000,  //  0012  CALL	R10	0
      0x8C2C0F08,  //  0013  GETMET	R11	R7	K8
      0x7C2C0200,  //  0014  CALL	R11	1
      0x8C301709,  //  0015  GETMET	R12	R11	K9
      0x58380004,  //  0016  LDCONST	R14	K4
      0x883C090A,  //  0017  GETMBR	R15	R4	K10
      0x5C401400,  //  0018  MOVE	R16	R10
      0x7C300800,  //  0019  CALL	R12	4
      0x8C301709,  //  001A  GETMET	R12	R11	K9
      0x5838000B,  //  001B  LDCONST	R14	K11
      0x883C090A,  //  001C  GETMBR	R15	R4	K10
      0x9440100A,  //  001D  GETIDX	R16	R8	R10
      0x7C300800,  //  001E  CALL	R12	4
      0x7001FFF0,  //  001F  JMP		#0011
      0x5824000C,  //  0020  LDCONST	R9	K12
      0xAC240200,  //  0021  CATCH	R9	1	0
      0xB0080000,  //  0022  RAISE	2	R0	R0
      0x80040E00,  //  0023  RET	1	R7
      0x70020032,  //  0024  JMP		#0058
      0x1C1C0D0B,  //  0025  EQ	R7	R6	K11
      0x781E0013,  //  0026  JMPF	R7	#003B
      0x8C1C0905,  //  0027  GETMET	R7	R4	K5
      0x7C1C0200,  //  0028  CALL	R7	1
      0x60200010,  //  0029  GETGBL	R8	G16
      0x8C24010D,  //  002A  GETMET	R9	R0	K13
      0x7C240200,  //  002B  CALL	R9	1
      0x7C200200,  //  002C  CALL	R8	1
      0xA8020007,  //  002D  EXBLK	0	#0036
      0x5C241000,  //  002E  MOVE	R9	R8
      0x7C240000,  //  002F  CALL	R9	0
      0x8C280F09,  //  0030  GETMET	R10	R7	K9
      0x4C300000,  //  0031  LDNIL	R12
      0x8834090E,  //  0032  GETMBR	R13	R4	K14
      0x5C381200,  //  0033  MOVE	R14	R9
      0x7C280800,  //  0034  CALL	R10	4
      0x7001FFF7,  //  0035  JMP		#002E
      0x5820000C,  //  0036  LDCONST	R8	K12
      0xAC200200,  //  0037  CATCH	R8	1	0
      0xB0080000,  //  0038  RAISE	2	R0	R0
      0x80040E00,  //  0039  RET	1	R7
      0x7002001C,  //  003A  JMP		#0058
      0x1C1C0D0F,  //  003B  EQ	R7	R6	K15
      0x781E0003,  //  003C  JMPF	R7	#0041
      0x8C1C0905,  //  003D  GETMET	R7	R4	K5
      0x7C1C0200,  //  003E  CALL	R7	1
      0x80040E00,  //  003F  RET	1	R7
      0x70020016,  //  0040  JMP		#0058
      0x1C1C0D10,  //  0041  EQ	R7	R6	K16
      0x781E0003,  //  0042  JMPF	R7	#0047
      0x8C1C0905,  //  0043  GETMET	R7	R4	K5
      0x7C1C0200,  //  0044  CALL	R7	1
      0x80040E00,  //  0045  RET	1	R7
      0x70020010,  //  0046  JMP		#0058
      0x541EFFFB,  //  0047  LDINT	R7	65532
      0x1C1C0C07,  //  0048  EQ	R7	R6	R7
      0x781E0005,  //  0049  JMPF	R7	#0050
      0x8C1C0711,  //  004A  GETMET	R7	R3	K17
      0x8824090E,  //  004B  GETMBR	R9	R4	K14
      0x58280004,  //  004C  LDCONST	R10	K4
      0x7C1C0600,  //  004D  CALL	R7	3
      0x80040E00,  //  004E  RET	1	R7
      0x70020007,  //  004F  JMP		#0058
      0x541EFFFC,  //  0050  LDINT	R7	65533
      0x1C1C0C07,  //  0051  EQ	R7	R6	R7
      0x781E0004,  //  0052  JMPF	R7	#0058
      0x8C1C0711,  //  0053  GETMET	R7	R3	K17
      0x8824090E,  //  0054  GETMBR	R9	R4	K14
      0x5828000B,  //  0055  LDCONST	R10	K11
      0x7C1C0600,  //  0056  CALL	R7	3
      0x80040E00,  //  0057  RET	1	R7
      0x70020017,  //  0058  JMP		#0071
      0x541E0038,  //  0059  LDINT	R7	57
      0x1C1C0A07,  //  005A  EQ	R7	R5	R7
      0x781E0012,  //  005B  JMPF	R7	#006F
      0x541E0010,  //  005C  LDINT	R7	17
      0x1C1C0C07,  //  005D  EQ	R7	R6	R7
      0x781E0005,  //  005E  JMPF	R7	#0065
      0x8C1C0711,  //  005F  GETMET	R7	R3	K17
      0x88240912,  //  0060  GETMBR	R9	R4	K18
      0x5828000B,  //  0061  LDCONST	R10	K11
      0x7C1C0600,  //  0062  CALL	R7	3
      0x80040E00,  //  0063  RET	1	R7
      0x70020008,  //  0064  JMP		#006E
      0x601C0003,  //  0065  GETGBL	R7	G3
      0x5C200000,  //  0066  MOVE	R8	R0
      0x7C1C0200,  //  0067  CALL	R7	1
      0x8C1C0F13,  //  0068  GETMET	R7	R7	K19
      0x5C240200,  //  0069  MOVE	R9	R1
      0x5C280400,  //  006A  MOVE	R10	R2
      0x5C2C0600,  //  006B  MOVE	R11	R3
      0x7C1C0800,  //  006C  CALL	R7	4
      0x80040E00,  //  006D  RET	1	R7
      0x70020001,  //  006E  JMP		#0071
      0x4C1C0000,  //  006F  LDNIL	R7
      0x80040E00,  //  0070  RET	1	R7
      0x80000000,  //  0071  RET	0
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
** Solidified function: set_name
********************************************************************/
be_local_closure(Matter_Plugin_set_name,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(node_label),
    /* K1   */  be_nested_str_weak(attribute_updated),
    }),
    be_str_weak(set_name),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x20080202,  //  0001  NE	R2	R1	R2
      0x780A0003,  //  0002  JMPF	R2	#0007
      0x8C080101,  //  0003  GETMET	R2	R0	K1
      0x54120038,  //  0004  LDINT	R4	57
      0x54160004,  //  0005  LDINT	R5	5
      0x7C080600,  //  0006  CALL	R2	3
      0x90020001,  //  0007  SETMBR	R0	K0	R1
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_local_device
********************************************************************/
be_local_closure(Matter_Plugin_is_local_device,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(is_local_device),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x50040200,  //  0000  LDBOOL	R1	1	0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_name
********************************************************************/
be_local_closure(Matter_Plugin_get_name,   /* name */
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
    /* K0   */  be_nested_str_weak(node_label),
    }),
    be_str_weak(get_name),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_init,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(endpoint),
    /* K2   */  be_nested_str_weak(clusters),
    /* K3   */  be_nested_str_weak(consolidate_clusters),
    /* K4   */  be_nested_str_weak(parse_configuration),
    /* K5   */  be_nested_str_weak(node_label),
    /* K6   */  be_nested_str_weak(find),
    /* K7   */  be_nested_str_weak(name),
    /* K8   */  be_nested_str_weak(),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x90020202,  //  0001  SETMBR	R0	K1	R2
      0x8C100103,  //  0002  GETMET	R4	R0	K3
      0x7C100200,  //  0003  CALL	R4	1
      0x90020404,  //  0004  SETMBR	R0	K2	R4
      0x8C100104,  //  0005  GETMET	R4	R0	K4
      0x5C180600,  //  0006  MOVE	R6	R3
      0x7C100400,  //  0007  CALL	R4	2
      0x8C100706,  //  0008  GETMET	R4	R3	K6
      0x58180007,  //  0009  LDCONST	R6	K7
      0x581C0008,  //  000A  LDCONST	R7	K8
      0x7C100600,  //  000B  CALL	R4	3
      0x90020A04,  //  000C  SETMBR	R0	K5	R4
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_shadow_lazy
********************************************************************/
be_local_closure(Matter_Plugin_update_shadow_lazy,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(tick),
    /* K1   */  be_nested_str_weak(device),
    /* K2   */  be_nested_str_weak(update_shadow),
    }),
    be_str_weak(update_shadow_lazy),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x88080500,  //  0002  GETMBR	R2	R2	K0
      0x20040202,  //  0003  NE	R1	R1	R2
      0x78060004,  //  0004  JMPF	R1	#000A
      0x8C040102,  //  0005  GETMET	R1	R0	K2
      0x7C040200,  //  0006  CALL	R1	1
      0x88040101,  //  0007  GETMBR	R1	R0	K1
      0x88040300,  //  0008  GETMBR	R1	R1	K0
      0x90020001,  //  0009  SETMBR	R0	K0	R1
      0x80000000,  //  000A  RET	0
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
** Solidified function: ui_string_to_conf
********************************************************************/
be_local_closure(Matter_Plugin_ui_string_to_conf,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_const_class(be_class_Matter_Plugin),
    /* K1   */  be_nested_str_weak(ARG),
    /* K2   */  be_nested_str_weak(ARG_TYPE),
    }),
    be_str_weak(ui_string_to_conf),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x580C0000,  //  0000  LDCONST	R3	K0
      0x88100101,  //  0001  GETMBR	R4	R0	K1
      0x88140102,  //  0002  GETMBR	R5	R0	K2
      0x780A0004,  //  0003  JMPF	R2	#0009
      0x78120003,  //  0004  JMPF	R4	#0009
      0x5C180A00,  //  0005  MOVE	R6	R5
      0x5C1C0400,  //  0006  MOVE	R7	R2
      0x7C180200,  //  0007  CALL	R6	1
      0x98040806,  //  0008  SETIDX	R1	R4	R6
      0x80040200,  //  0009  RET	1	R1
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
** Solidified function: get_attribute_list
********************************************************************/
be_local_closure(Matter_Plugin_get_attribute_list,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
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
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x60140012,  //  0003  GETGBL	R5	G18
      0x7C140000,  //  0004  CALL	R5	0
      0x7C080600,  //  0005  CALL	R2	3
      0x80040400,  //  0006  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: append_state_json
********************************************************************/
be_local_closure(Matter_Plugin_append_state_json,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(),
    }),
    be_str_weak(append_state_json),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80060000,  //  0000  RET	1	K0
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
** Solidified function: parse_configuration
********************************************************************/
be_local_closure(Matter_Plugin_parse_configuration,   /* name */
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
    be_str_weak(parse_configuration),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: state_json
********************************************************************/
be_local_closure(Matter_Plugin_state_json,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(json),
    /* K1   */  be_nested_str_weak(node_label),
    /* K2   */  be_nested_str_weak(_X2C_X22Name_X22_X3A_X25s),
    /* K3   */  be_nested_str_weak(dump),
    /* K4   */  be_nested_str_weak(),
    /* K5   */  be_nested_str_weak(append_state_json),
    /* K6   */  be_nested_str_weak(_X7B_X22Ep_X22_X3A_X25i_X25s_X25s_X7D),
    /* K7   */  be_nested_str_weak(endpoint),
    }),
    be_str_weak(state_json),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x780A0006,  //  0002  JMPF	R2	#000A
      0x60080018,  //  0003  GETGBL	R2	G24
      0x580C0002,  //  0004  LDCONST	R3	K2
      0x8C100303,  //  0005  GETMET	R4	R1	K3
      0x88180101,  //  0006  GETMBR	R6	R0	K1
      0x7C100400,  //  0007  CALL	R4	2
      0x7C080400,  //  0008  CALL	R2	2
      0x70020000,  //  0009  JMP		#000B
      0x58080004,  //  000A  LDCONST	R2	K4
      0x8C0C0105,  //  000B  GETMET	R3	R0	K5
      0x7C0C0200,  //  000C  CALL	R3	1
      0x780E0007,  //  000D  JMPF	R3	#0016
      0x60100018,  //  000E  GETGBL	R4	G24
      0x58140006,  //  000F  LDCONST	R5	K6
      0x88180107,  //  0010  GETMBR	R6	R0	K7
      0x5C1C0400,  //  0011  MOVE	R7	R2
      0x5C200600,  //  0012  MOVE	R8	R3
      0x7C100800,  //  0013  CALL	R4	4
      0x80040800,  //  0014  RET	1	R4
      0x70020001,  //  0015  JMP		#0018
      0x4C100000,  //  0016  LDNIL	R4
      0x80040800,  //  0017  RET	1	R4
      0x80000000,  //  0018  RET	0
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
** Solidified function: every_250ms
********************************************************************/
be_local_closure(Matter_Plugin_every_250ms,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(update_next),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(jitter),
    /* K3   */  be_nested_str_weak(UPDATE_TIME),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(time_reached),
    /* K6   */  be_nested_str_weak(tick),
    /* K7   */  be_nested_str_weak(device),
    /* K8   */  be_nested_str_weak(update_shadow),
    /* K9   */  be_nested_str_weak(millis),
    }),
    be_str_weak(every_250ms),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060005,  //  0003  JMPF	R1	#000A
      0xB8060200,  //  0004  GETNGBL	R1	K1
      0x8C040302,  //  0005  GETMET	R1	R1	K2
      0x880C0103,  //  0006  GETMBR	R3	R0	K3
      0x7C040400,  //  0007  CALL	R1	2
      0x90020001,  //  0008  SETMBR	R0	K0	R1
      0x70020010,  //  0009  JMP		#001B
      0xB8060800,  //  000A  GETNGBL	R1	K4
      0x8C040305,  //  000B  GETMET	R1	R1	K5
      0x880C0100,  //  000C  GETMBR	R3	R0	K0
      0x7C040400,  //  000D  CALL	R1	2
      0x7806000B,  //  000E  JMPF	R1	#001B
      0x88040106,  //  000F  GETMBR	R1	R0	K6
      0x88080107,  //  0010  GETMBR	R2	R0	K7
      0x88080506,  //  0011  GETMBR	R2	R2	K6
      0x20040202,  //  0012  NE	R1	R1	R2
      0x78060001,  //  0013  JMPF	R1	#0016
      0x8C040108,  //  0014  GETMET	R1	R0	K8
      0x7C040200,  //  0015  CALL	R1	1
      0xB8060800,  //  0016  GETNGBL	R1	K4
      0x8C040309,  //  0017  GETMET	R1	R1	K9
      0x880C0103,  //  0018  GETMBR	R3	R0	K3
      0x7C040400,  //  0019  CALL	R1	2
      0x90020001,  //  001A  SETMBR	R0	K0	R1
      0x80000000,  //  001B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: contains_attribute
********************************************************************/
be_local_closure(Matter_Plugin_contains_attribute,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(clusters),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_const_int(0),
    /* K3   */  be_const_int(1),
    }),
    be_str_weak(contains_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x4C100000,  //  0004  LDNIL	R4
      0x20100604,  //  0005  NE	R4	R3	R4
      0x7812000C,  //  0006  JMPF	R4	#0014
      0x58100002,  //  0007  LDCONST	R4	K2
      0x6014000C,  //  0008  GETGBL	R5	G12
      0x5C180600,  //  0009  MOVE	R6	R3
      0x7C140200,  //  000A  CALL	R5	1
      0x14140805,  //  000B  LT	R5	R4	R5
      0x78160006,  //  000C  JMPF	R5	#0014
      0x94140604,  //  000D  GETIDX	R5	R3	R4
      0x1C140A02,  //  000E  EQ	R5	R5	R2
      0x78160001,  //  000F  JMPF	R5	#0012
      0x50140200,  //  0010  LDBOOL	R5	1	0
      0x80040A00,  //  0011  RET	1	R5
      0x00100903,  //  0012  ADD	R4	R4	K3
      0x7001FFF3,  //  0013  JMP		#0008
      0x50100000,  //  0014  LDBOOL	R4	0	0
      0x80040800,  //  0015  RET	1	R4
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
** Solidified function: attribute_updated
********************************************************************/
be_local_closure(Matter_Plugin_attribute_updated,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(attribute_updated),
    /* K2   */  be_nested_str_weak(endpoint),
    }),
    be_str_weak(attribute_updated),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88100100,  //  0000  GETMBR	R4	R0	K0
      0x8C100901,  //  0001  GETMET	R4	R4	K1
      0x88180102,  //  0002  GETMBR	R6	R0	K2
      0x5C1C0200,  //  0003  MOVE	R7	R1
      0x5C200400,  //  0004  MOVE	R8	R2
      0x5C240600,  //  0005  MOVE	R9	R3
      0x7C100A00,  //  0006  CALL	R4	5
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: <lambda>
********************************************************************/
be_local_closure(Matter_Plugin__X3Clambda_X3E,   /* name */
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
    be_str_weak(_X3Clambda_X3E),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x60040008,  //  0000  GETGBL	R1	G8
      0x5C080000,  //  0001  MOVE	R2	R0
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_virtual
********************************************************************/
be_local_closure(Matter_Plugin_update_virtual,   /* name */
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
    be_str_weak(update_virtual),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_cluster_list
********************************************************************/
be_local_closure(Matter_Plugin_get_cluster_list,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
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
      0x60040012,  //  0000  GETGBL	R1	G18
      0x7C040000,  //  0001  CALL	R1	0
      0x60080010,  //  0002  GETGBL	R2	G16
      0x880C0100,  //  0003  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0004  GETMET	R3	R3	K1
      0x7C0C0200,  //  0005  CALL	R3	1
      0x7C080200,  //  0006  CALL	R2	1
      0xA8020005,  //  0007  EXBLK	0	#000E
      0x5C0C0400,  //  0008  MOVE	R3	R2
      0x7C0C0000,  //  0009  CALL	R3	0
      0x8C100302,  //  000A  GETMET	R4	R1	K2
      0x5C180600,  //  000B  MOVE	R6	R3
      0x7C100400,  //  000C  CALL	R4	2
      0x7001FFF9,  //  000D  JMP		#0008
      0x58080003,  //  000E  LDCONST	R2	K3
      0xAC080200,  //  000F  CATCH	R2	1	0
      0xB0080000,  //  0010  RAISE	2	R0	R0
      0x80040200,  //  0011  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: ui_conf_to_string
********************************************************************/
be_local_closure(Matter_Plugin_ui_conf_to_string,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_const_class(be_class_Matter_Plugin),
    /* K1   */  be_nested_str_weak(ARG),
    /* K2   */  be_nested_str_weak(find),
    /* K3   */  be_nested_str_weak(),
    }),
    be_str_weak(ui_conf_to_string),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0x880C0101,  //  0001  GETMBR	R3	R0	K1
      0x780E0006,  //  0002  JMPF	R3	#000A
      0x60100008,  //  0003  GETGBL	R4	G8
      0x8C140302,  //  0004  GETMET	R5	R1	K2
      0x5C1C0600,  //  0005  MOVE	R7	R3
      0x58200003,  //  0006  LDCONST	R8	K3
      0x7C140600,  //  0007  CALL	R5	3
      0x7C100200,  //  0008  CALL	R4	1
      0x70020000,  //  0009  JMP		#000B
      0x58100003,  //  000A  LDCONST	R4	K3
      0x80040800,  //  000B  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: contains_cluster
********************************************************************/
be_local_closure(Matter_Plugin_contains_cluster,   /* name */
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
    /* K0   */  be_nested_str_weak(clusters),
    /* K1   */  be_nested_str_weak(contains),
    }),
    be_str_weak(contains_cluster),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin
********************************************************************/
be_local_class(Matter_Plugin,
    6,
    NULL,
    be_nested_map(47,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(ack_request, -1), be_const_closure(Matter_Plugin_ack_request_closure) },
        { be_const_key_weak(parse_sensors, -1), be_const_closure(Matter_Plugin_parse_sensors_closure) },
        { be_const_key_weak(ui_conf_to_string, 45), be_const_static_closure(Matter_Plugin_ui_conf_to_string_closure) },
        { be_const_key_weak(clusters, 27), be_const_var(3) },
        { be_const_key_weak(ARG_HINT, -1), be_nested_str_weak(_Not_X20used_) },
        { be_const_key_weak(subscribe_attribute, -1), be_const_closure(Matter_Plugin_subscribe_attribute_closure) },
        { be_const_key_weak(timed_request, -1), be_const_closure(Matter_Plugin_timed_request_closure) },
        { be_const_key_weak(endpoint, -1), be_const_var(2) },
        { be_const_key_weak(DISPLAY_NAME, 1), be_nested_str_weak() },
        { be_const_key_weak(update_shadow, 2), be_const_closure(Matter_Plugin_update_shadow_closure) },
        { be_const_key_weak(ARG_TYPE, -1), be_const_static_closure(Matter_Plugin__X3Clambda_X3E_closure) },
        { be_const_key_weak(publish_command, -1), be_const_closure(Matter_Plugin_publish_command_closure) },
        { be_const_key_weak(consolidate_clusters, 5), be_const_closure(Matter_Plugin_consolidate_clusters_closure) },
        { be_const_key_weak(set_name, 42), be_const_closure(Matter_Plugin_set_name_closure) },
        { be_const_key_weak(write_attribute, 23), be_const_closure(Matter_Plugin_write_attribute_closure) },
        { be_const_key_weak(VIRTUAL, 6), be_const_bool(0) },
        { be_const_key_weak(node_label, 34), be_const_var(5) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
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
        { be_const_key_int(57, 0), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(1,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(17),
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(update_shadow_lazy, -1), be_const_closure(Matter_Plugin_update_shadow_lazy_closure) },
        { be_const_key_weak(update_next, 40), be_const_var(0) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(Matter_Plugin_invoke_request_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_init_closure) },
        { be_const_key_weak(ARG, 14), be_nested_str_weak() },
        { be_const_key_weak(get_endpoint, 17), be_const_closure(Matter_Plugin_get_endpoint_closure) },
        { be_const_key_weak(get_name, 20), be_const_closure(Matter_Plugin_get_name_closure) },
        { be_const_key_weak(ui_string_to_conf, 10), be_const_static_closure(Matter_Plugin_ui_string_to_conf_closure) },
        { be_const_key_weak(tick, 18), be_const_var(4) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak() },
        { be_const_key_weak(append_state_json, -1), be_const_closure(Matter_Plugin_append_state_json_closure) },
        { be_const_key_weak(read_event, -1), be_const_closure(Matter_Plugin_read_event_closure) },
        { be_const_key_weak(device, -1), be_const_var(1) },
        { be_const_key_weak(parse_configuration, -1), be_const_closure(Matter_Plugin_parse_configuration_closure) },
        { be_const_key_weak(state_json, -1), be_const_closure(Matter_Plugin_state_json_closure) },
        { be_const_key_weak(subscribe_event, -1), be_const_closure(Matter_Plugin_subscribe_event_closure) },
        { be_const_key_weak(get_attribute_list, -1), be_const_closure(Matter_Plugin_get_attribute_list_closure) },
        { be_const_key_weak(read_attribute, 26), be_const_closure(Matter_Plugin_read_attribute_closure) },
        { be_const_key_weak(has, -1), be_const_closure(Matter_Plugin_has_closure) },
        { be_const_key_weak(UPDATE_TIME, -1), be_const_int(5000) },
        { be_const_key_weak(every_250ms, -1), be_const_closure(Matter_Plugin_every_250ms_closure) },
        { be_const_key_weak(contains_attribute, -1), be_const_closure(Matter_Plugin_contains_attribute_closure) },
        { be_const_key_weak(is_local_device, 13), be_const_closure(Matter_Plugin_is_local_device_closure) },
        { be_const_key_weak(attribute_updated, -1), be_const_closure(Matter_Plugin_attribute_updated_closure) },
        { be_const_key_weak(UPDATE_COMMANDS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(0,
    ( (struct bvalue*) &(const bvalue[]) {
    }))    ) } )) },
        { be_const_key_weak(update_virtual, -1), be_const_closure(Matter_Plugin_update_virtual_closure) },
        { be_const_key_weak(get_cluster_list, -1), be_const_closure(Matter_Plugin_get_cluster_list_closure) },
        { be_const_key_weak(consolidate_update_commands, -1), be_const_closure(Matter_Plugin_consolidate_update_commands_closure) },
        { be_const_key_weak(contains_cluster, -1), be_const_closure(Matter_Plugin_contains_cluster_closure) },
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
