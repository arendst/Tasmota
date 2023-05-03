/* Solidification of Matter_Device.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Device;

/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(Matter_Device_start,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
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
        /* K0   */  be_nested_str_weak(_trigger_read_sensors),
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
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(started),
    /* K1   */  be_nested_str_weak(autoconf_device),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(add_cron),
    /* K4   */  be_nested_str_weak(_X2A_X2F30_X20_X2A_X20_X2A_X20_X2A_X20_X2A_X20_X2A),
    /* K5   */  be_nested_str_weak(matter_sensors_30s),
    /* K6   */  be_nested_str_weak(_start_udp),
    /* K7   */  be_nested_str_weak(UDP_PORT),
    /* K8   */  be_nested_str_weak(start_mdns_announce_hostnames),
    }),
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060000,  //  0001  JMPF	R1	#0003
      0x80000200,  //  0002  RET	0
      0x8C040101,  //  0003  GETMET	R1	R0	K1
      0x7C040200,  //  0004  CALL	R1	1
      0xB8060400,  //  0005  GETNGBL	R1	K2
      0x8C040303,  //  0006  GETMET	R1	R1	K3
      0x580C0004,  //  0007  LDCONST	R3	K4
      0x84100000,  //  0008  CLOSURE	R4	P0
      0x58140005,  //  0009  LDCONST	R5	K5
      0x7C040800,  //  000A  CALL	R1	4
      0x8C040106,  //  000B  GETMET	R1	R0	K6
      0x880C0107,  //  000C  GETMBR	R3	R0	K7
      0x7C040400,  //  000D  CALL	R1	2
      0x8C040108,  //  000E  GETMET	R1	R0	K8
      0x7C040200,  //  000F  CALL	R1	1
      0x50040200,  //  0010  LDBOOL	R1	1	0
      0x90020001,  //  0011  SETMBR	R0	K0	R1
      0xA0000000,  //  0012  CLOSE	R0
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: generate_random_passcode
********************************************************************/
be_local_closure(Matter_Device_generate_random_passcode,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(random),
    /* K2   */  be_nested_str_weak(get),
    /* K3   */  be_const_int(0),
    /* K4   */  be_const_int(134217727),
    /* K5   */  be_const_int(99999998),
    /* K6   */  be_nested_str_weak(PASSCODE_INVALID),
    /* K7   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(generate_random_passcode),
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x500C0200,  //  0002  LDBOOL	R3	1	0
      0x780E001D,  //  0003  JMPF	R3	#0022
      0x8C0C0301,  //  0004  GETMET	R3	R1	K1
      0x54160003,  //  0005  LDINT	R5	4
      0x7C0C0400,  //  0006  CALL	R3	2
      0x8C0C0702,  //  0007  GETMET	R3	R3	K2
      0x58140003,  //  0008  LDCONST	R5	K3
      0x541A0003,  //  0009  LDINT	R6	4
      0x7C0C0600,  //  000A  CALL	R3	3
      0x2C0C0704,  //  000B  AND	R3	R3	K4
      0x5C080600,  //  000C  MOVE	R2	R3
      0x240C0505,  //  000D  GT	R3	R2	K5
      0x780E0000,  //  000E  JMPF	R3	#0010
      0x7001FFF1,  //  000F  JMP		#0002
      0x600C0010,  //  0010  GETGBL	R3	G16
      0x88100106,  //  0011  GETMBR	R4	R0	K6
      0x7C0C0200,  //  0012  CALL	R3	1
      0xA8020005,  //  0013  EXBLK	0	#001A
      0x5C100600,  //  0014  MOVE	R4	R3
      0x7C100000,  //  0015  CALL	R4	0
      0x1C140404,  //  0016  EQ	R5	R2	R4
      0x78160000,  //  0017  JMPF	R5	#0019
      0x4C080000,  //  0018  LDNIL	R2
      0x7001FFF9,  //  0019  JMP		#0014
      0x580C0007,  //  001A  LDCONST	R3	K7
      0xAC0C0200,  //  001B  CATCH	R3	1	0
      0xB0080000,  //  001C  RAISE	2	R0	R0
      0x4C0C0000,  //  001D  LDNIL	R3
      0x200C0403,  //  001E  NE	R3	R2	R3
      0x780E0000,  //  001F  JMPF	R3	#0021
      0x80040400,  //  0020  RET	1	R2
      0x7001FFDF,  //  0021  JMP		#0002
      0x80000000,  //  0022  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: save_param
********************************************************************/
be_local_closure(Matter_Device_save_param,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[26]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(json),
    /* K2   */  be_nested_str_weak(format),
    /* K3   */  be_nested_str_weak(_X7B_X22distinguish_X22_X3A_X25i_X2C_X22passcode_X22_X3A_X25i_X2C_X22ipv4only_X22_X3A_X25s),
    /* K4   */  be_nested_str_weak(root_discriminator),
    /* K5   */  be_nested_str_weak(root_passcode),
    /* K6   */  be_nested_str_weak(ipv4only),
    /* K7   */  be_nested_str_weak(true),
    /* K8   */  be_nested_str_weak(false),
    /* K9   */  be_nested_str_weak(plugins_persist),
    /* K10  */  be_nested_str_weak(_X2C_X22config_X22_X3A),
    /* K11  */  be_nested_str_weak(dump),
    /* K12  */  be_nested_str_weak(plugins_config),
    /* K13  */  be_nested_str_weak(_X7D),
    /* K14  */  be_nested_str_weak(FILENAME),
    /* K15  */  be_nested_str_weak(w),
    /* K16  */  be_nested_str_weak(write),
    /* K17  */  be_nested_str_weak(close),
    /* K18  */  be_nested_str_weak(tasmota),
    /* K19  */  be_nested_str_weak(log),
    /* K20  */  be_nested_str_weak(MTR_X3A_X20_X3DSaved_X20_X20_X20_X20_X20parameters_X25s),
    /* K21  */  be_nested_str_weak(_X20and_X20condiguration),
    /* K22  */  be_nested_str_weak(),
    /* K23  */  be_const_int(2),
    /* K24  */  be_nested_str_weak(MTR_X3A_X20Session_Store_X3A_X3Asave_X20Exception_X3A),
    /* K25  */  be_nested_str_weak(_X7C),
    }),
    be_str_weak(save_param),
    &be_const_str_solidified,
    ( &(const binstruction[65]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x8C0C0302,  //  0002  GETMET	R3	R1	K2
      0x58140003,  //  0003  LDCONST	R5	K3
      0x88180104,  //  0004  GETMBR	R6	R0	K4
      0x881C0105,  //  0005  GETMBR	R7	R0	K5
      0x88200106,  //  0006  GETMBR	R8	R0	K6
      0x78220001,  //  0007  JMPF	R8	#000A
      0x58200007,  //  0008  LDCONST	R8	K7
      0x70020000,  //  0009  JMP		#000B
      0x58200008,  //  000A  LDCONST	R8	K8
      0x7C0C0A00,  //  000B  CALL	R3	5
      0x88100109,  //  000C  GETMBR	R4	R0	K9
      0x78120004,  //  000D  JMPF	R4	#0013
      0x000C070A,  //  000E  ADD	R3	R3	K10
      0x8C10050B,  //  000F  GETMET	R4	R2	K11
      0x8818010C,  //  0010  GETMBR	R6	R0	K12
      0x7C100400,  //  0011  CALL	R4	2
      0x000C0604,  //  0012  ADD	R3	R3	R4
      0x000C070D,  //  0013  ADD	R3	R3	K13
      0xA8020018,  //  0014  EXBLK	0	#002E
      0x60100011,  //  0015  GETGBL	R4	G17
      0x8814010E,  //  0016  GETMBR	R5	R0	K14
      0x5818000F,  //  0017  LDCONST	R6	K15
      0x7C100400,  //  0018  CALL	R4	2
      0x8C140910,  //  0019  GETMET	R5	R4	K16
      0x5C1C0600,  //  001A  MOVE	R7	R3
      0x7C140400,  //  001B  CALL	R5	2
      0x8C140911,  //  001C  GETMET	R5	R4	K17
      0x7C140200,  //  001D  CALL	R5	1
      0xB8162400,  //  001E  GETNGBL	R5	K18
      0x8C140B13,  //  001F  GETMET	R5	R5	K19
      0x8C1C0302,  //  0020  GETMET	R7	R1	K2
      0x58240014,  //  0021  LDCONST	R9	K20
      0x88280109,  //  0022  GETMBR	R10	R0	K9
      0x782A0001,  //  0023  JMPF	R10	#0026
      0x58280015,  //  0024  LDCONST	R10	K21
      0x70020000,  //  0025  JMP		#0027
      0x58280016,  //  0026  LDCONST	R10	K22
      0x7C1C0600,  //  0027  CALL	R7	3
      0x58200017,  //  0028  LDCONST	R8	K23
      0x7C140600,  //  0029  CALL	R5	3
      0xA8040001,  //  002A  EXBLK	1	1
      0x80040600,  //  002B  RET	1	R3
      0xA8040001,  //  002C  EXBLK	1	1
      0x70020011,  //  002D  JMP		#0040
      0xAC100002,  //  002E  CATCH	R4	0	2
      0x7002000E,  //  002F  JMP		#003F
      0xB81A2400,  //  0030  GETNGBL	R6	K18
      0x8C180D13,  //  0031  GETMET	R6	R6	K19
      0x60200008,  //  0032  GETGBL	R8	G8
      0x5C240800,  //  0033  MOVE	R9	R4
      0x7C200200,  //  0034  CALL	R8	1
      0x00223008,  //  0035  ADD	R8	K24	R8
      0x00201119,  //  0036  ADD	R8	R8	K25
      0x60240008,  //  0037  GETGBL	R9	G8
      0x5C280A00,  //  0038  MOVE	R10	R5
      0x7C240200,  //  0039  CALL	R9	1
      0x00201009,  //  003A  ADD	R8	R8	R9
      0x58240017,  //  003B  LDCONST	R9	K23
      0x7C180600,  //  003C  CALL	R6	3
      0x80040600,  //  003D  RET	1	R3
      0x70020000,  //  003E  JMP		#0040
      0xB0080000,  //  003F  RAISE	2	R0	R0
      0x80000000,  //  0040  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_fabric
********************************************************************/
be_local_closure(Matter_Device_remove_fabric,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(sessions),
    /* K1   */  be_nested_str_weak(find_children_fabrics),
    /* K2   */  be_nested_str_weak(get_fabric_index),
    /* K3   */  be_nested_str_weak(find_fabric_by_index),
    /* K4   */  be_nested_str_weak(message_handler),
    /* K5   */  be_nested_str_weak(im),
    /* K6   */  be_nested_str_weak(subs_shop),
    /* K7   */  be_nested_str_weak(remove_by_fabric),
    /* K8   */  be_nested_str_weak(mdns_remove_op_discovery),
    /* K9   */  be_nested_str_weak(remove_fabric),
    /* K10  */  be_nested_str_weak(stop_iteration),
    /* K11  */  be_nested_str_weak(save_fabrics),
    }),
    be_str_weak(remove_fabric),
    &be_const_str_solidified,
    ( &(const binstruction[43]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x8C100302,  //  0002  GETMET	R4	R1	K2
      0x7C100200,  //  0003  CALL	R4	1
      0x7C080400,  //  0004  CALL	R2	2
      0x4C0C0000,  //  0005  LDNIL	R3
      0x1C0C0403,  //  0006  EQ	R3	R2	R3
      0x780E0000,  //  0007  JMPF	R3	#0009
      0x80000600,  //  0008  RET	0
      0x600C0010,  //  0009  GETGBL	R3	G16
      0x5C100400,  //  000A  MOVE	R4	R2
      0x7C0C0200,  //  000B  CALL	R3	1
      0xA8020016,  //  000C  EXBLK	0	#0024
      0x5C100600,  //  000D  MOVE	R4	R3
      0x7C100000,  //  000E  CALL	R4	0
      0x88140100,  //  000F  GETMBR	R5	R0	K0
      0x8C140B03,  //  0010  GETMET	R5	R5	K3
      0x5C1C0800,  //  0011  MOVE	R7	R4
      0x7C140400,  //  0012  CALL	R5	2
      0x4C180000,  //  0013  LDNIL	R6
      0x20180A06,  //  0014  NE	R6	R5	R6
      0x781A000C,  //  0015  JMPF	R6	#0023
      0x88180104,  //  0016  GETMBR	R6	R0	K4
      0x88180D05,  //  0017  GETMBR	R6	R6	K5
      0x88180D06,  //  0018  GETMBR	R6	R6	K6
      0x8C180D07,  //  0019  GETMET	R6	R6	K7
      0x5C200A00,  //  001A  MOVE	R8	R5
      0x7C180400,  //  001B  CALL	R6	2
      0x8C180108,  //  001C  GETMET	R6	R0	K8
      0x5C200A00,  //  001D  MOVE	R8	R5
      0x7C180400,  //  001E  CALL	R6	2
      0x88180100,  //  001F  GETMBR	R6	R0	K0
      0x8C180D09,  //  0020  GETMET	R6	R6	K9
      0x5C200A00,  //  0021  MOVE	R8	R5
      0x7C180400,  //  0022  CALL	R6	2
      0x7001FFE8,  //  0023  JMP		#000D
      0x580C000A,  //  0024  LDCONST	R3	K10
      0xAC0C0200,  //  0025  CATCH	R3	1	0
      0xB0080000,  //  0026  RAISE	2	R0	R0
      0x880C0100,  //  0027  GETMBR	R3	R0	K0
      0x8C0C070B,  //  0028  GETMET	R3	R3	K11
      0x7C0C0200,  //  0029  CALL	R3	1
      0x80000000,  //  002A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _start_udp
********************************************************************/
be_local_closure(Matter_Device__start_udp,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
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
        /* K0   */  be_nested_str_weak(msg_received),
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
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(udp_server),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(MTR_X3A_X20starting_X20UDP_X20server_X20on_X20port_X3A_X20),
    /* K4   */  be_const_int(2),
    /* K5   */  be_nested_str_weak(matter),
    /* K6   */  be_nested_str_weak(UDPServer),
    /* K7   */  be_nested_str_weak(),
    /* K8   */  be_nested_str_weak(start),
    }),
    be_str_weak(_start_udp),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x780A0000,  //  0001  JMPF	R2	#0003
      0x80000400,  //  0002  RET	0
      0x4C080000,  //  0003  LDNIL	R2
      0x1C080202,  //  0004  EQ	R2	R1	R2
      0x780A0000,  //  0005  JMPF	R2	#0007
      0x540615A3,  //  0006  LDINT	R1	5540
      0xB80A0200,  //  0007  GETNGBL	R2	K1
      0x8C080502,  //  0008  GETMET	R2	R2	K2
      0x60100008,  //  0009  GETGBL	R4	G8
      0x5C140200,  //  000A  MOVE	R5	R1
      0x7C100200,  //  000B  CALL	R4	1
      0x00120604,  //  000C  ADD	R4	K3	R4
      0x58140004,  //  000D  LDCONST	R5	K4
      0x7C080600,  //  000E  CALL	R2	3
      0xB80A0A00,  //  000F  GETNGBL	R2	K5
      0x8C080506,  //  0010  GETMET	R2	R2	K6
      0x58100007,  //  0011  LDCONST	R4	K7
      0x5C140200,  //  0012  MOVE	R5	R1
      0x7C080600,  //  0013  CALL	R2	3
      0x90020002,  //  0014  SETMBR	R0	K0	R2
      0x88080100,  //  0015  GETMBR	R2	R0	K0
      0x8C080508,  //  0016  GETMET	R2	R2	K8
      0x84100000,  //  0017  CLOSURE	R4	P0
      0x7C080400,  //  0018  CALL	R2	2
      0xA0000000,  //  0019  CLOSE	R0
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_50ms
********************************************************************/
be_local_closure(Matter_Device_every_50ms,   /* name */
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
    /* K1   */  be_const_int(1),
    }),
    be_str_weak(every_50ms),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x00040301,  //  0001  ADD	R1	R1	K1
      0x90020001,  //  0002  SETMBR	R0	K0	R1
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: stop
********************************************************************/
be_local_closure(Matter_Device_stop,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(remove_driver),
    /* K2   */  be_nested_str_weak(udp_server),
    /* K3   */  be_nested_str_weak(stop),
    }),
    be_str_weak(stop),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x88040102,  //  0004  GETMBR	R1	R0	K2
      0x78060002,  //  0005  JMPF	R1	#0009
      0x88040102,  //  0006  GETMBR	R1	R0	K2
      0x8C040303,  //  0007  GETMET	R1	R1	K3
      0x7C040200,  //  0008  CALL	R1	1
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_commissioning_complete
********************************************************************/
be_local_closure(Matter_Device_start_commissioning_complete,   /* name */
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
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(log),
    /* K2   */  be_nested_str_weak(MTR_X3A_X20_X2A_X2A_X2A_X20Commissioning_X20complete_X20_X2A_X2A_X2A),
    /* K3   */  be_const_int(2),
    /* K4   */  be_nested_str_weak(stop_basic_commissioning),
    }),
    be_str_weak(start_commissioning_complete),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x58100002,  //  0002  LDCONST	R4	K2
      0x58140003,  //  0003  LDCONST	R5	K3
      0x7C080600,  //  0004  CALL	R2	3
      0x8C080104,  //  0005  GETMET	R2	R0	K4
      0x7C080200,  //  0006  CALL	R2	1
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: register_commands
********************************************************************/
be_local_closure(Matter_Device_register_commands,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        10,                          /* nstack */
        4,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(MtrJoin),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 8]) {  /* code */
          0x68100000,  //  0000  GETUPV	R4	U0
          0x8C100900,  //  0001  GETMET	R4	R4	K0
          0x5C180000,  //  0002  MOVE	R6	R0
          0x5C1C0200,  //  0003  MOVE	R7	R1
          0x5C200400,  //  0004  MOVE	R8	R2
          0x5C240600,  //  0005  MOVE	R9	R3
          0x7C100A00,  //  0006  CALL	R4	5
          0x80040800,  //  0007  RET	1	R4
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(add_cmd),
    /* K2   */  be_nested_str_weak(MtrJoin),
    }),
    be_str_weak(register_commands),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x580C0002,  //  0002  LDCONST	R3	K2
      0x84100000,  //  0003  CLOSURE	R4	P0
      0x7C040600,  //  0004  CALL	R1	3
      0xA0000000,  //  0005  CLOSE	R0
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_operational_discovery_deferred
********************************************************************/
be_local_closure(Matter_Device_start_operational_discovery_deferred,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        3,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 2]) {  /* upvals */
          be_local_const_upval(1, 0),
          be_local_const_upval(1, 1),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(start_operational_discovery),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 5]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x68080001,  //  0002  GETUPV	R2	U1
          0x7C000400,  //  0003  CALL	R0	2
          0x80040000,  //  0004  RET	1	R0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(set_timer),
    /* K2   */  be_const_int(0),
    }),
    be_str_weak(start_operational_discovery_deferred),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x58100002,  //  0002  LDCONST	R4	K2
      0x84140000,  //  0003  CLOSURE	R5	P0
      0x7C080600,  //  0004  CALL	R2	3
      0xA0000000,  //  0005  CLOSE	R0
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: register_native_classes
********************************************************************/
be_local_closure(Matter_Device_register_native_classes,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(introspect),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(members),
    /* K3   */  be_nested_str_weak(matter),
    /* K4   */  be_nested_str_weak(get),
    /* K5   */  be_nested_str_weak(class),
    /* K6   */  be_nested_str_weak(find),
    /* K7   */  be_nested_str_weak(Plugin_),
    /* K8   */  be_const_int(0),
    /* K9   */  be_nested_str_weak(register_plugin_class),
    /* K10  */  be_nested_str_weak(stop_iteration),
    /* K11  */  be_nested_str_weak(tasmota),
    /* K12  */  be_nested_str_weak(log),
    /* K13  */  be_nested_str_weak(MTR_X3A_X20registered_X20classes_X20),
    /* K14  */  be_nested_str_weak(k2l),
    /* K15  */  be_nested_str_weak(plugins_classes),
    /* K16  */  be_const_int(3),
    }),
    be_str_weak(register_native_classes),
    &be_const_str_solidified,
    ( &(const binstruction[43]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xA4120200,  //  0001  IMPORT	R4	K1
      0x60140010,  //  0002  GETGBL	R5	G16
      0x8C180702,  //  0003  GETMET	R6	R3	K2
      0xB8220600,  //  0004  GETNGBL	R8	K3
      0x7C180400,  //  0005  CALL	R6	2
      0x7C140200,  //  0006  CALL	R5	1
      0xA8020014,  //  0007  EXBLK	0	#001D
      0x5C180A00,  //  0008  MOVE	R6	R5
      0x7C180000,  //  0009  CALL	R6	0
      0x8C1C0704,  //  000A  GETMET	R7	R3	K4
      0xB8260600,  //  000B  GETNGBL	R9	K3
      0x5C280C00,  //  000C  MOVE	R10	R6
      0x7C1C0600,  //  000D  CALL	R7	3
      0x60200004,  //  000E  GETGBL	R8	G4
      0x5C240E00,  //  000F  MOVE	R9	R7
      0x7C200200,  //  0010  CALL	R8	1
      0x1C201105,  //  0011  EQ	R8	R8	K5
      0x78220008,  //  0012  JMPF	R8	#001C
      0x8C200906,  //  0013  GETMET	R8	R4	K6
      0x5C280C00,  //  0014  MOVE	R10	R6
      0x582C0007,  //  0015  LDCONST	R11	K7
      0x7C200600,  //  0016  CALL	R8	3
      0x1C201108,  //  0017  EQ	R8	R8	K8
      0x78220002,  //  0018  JMPF	R8	#001C
      0x8C200109,  //  0019  GETMET	R8	R0	K9
      0x5C280E00,  //  001A  MOVE	R10	R7
      0x7C200400,  //  001B  CALL	R8	2
      0x7001FFEA,  //  001C  JMP		#0008
      0x5814000A,  //  001D  LDCONST	R5	K10
      0xAC140200,  //  001E  CATCH	R5	1	0
      0xB0080000,  //  001F  RAISE	2	R0	R0
      0xB8161600,  //  0020  GETNGBL	R5	K11
      0x8C140B0C,  //  0021  GETMET	R5	R5	K12
      0x601C0008,  //  0022  GETGBL	R7	G8
      0x8C20010E,  //  0023  GETMET	R8	R0	K14
      0x8828010F,  //  0024  GETMBR	R10	R0	K15
      0x7C200400,  //  0025  CALL	R8	2
      0x7C1C0200,  //  0026  CALL	R7	1
      0x001E1A07,  //  0027  ADD	R7	K13	R7
      0x58200010,  //  0028  LDCONST	R8	K16
      0x7C140600,  //  0029  CALL	R5	3
      0x80000000,  //  002A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: stop_basic_commissioning
********************************************************************/
be_local_closure(Matter_Device_stop_basic_commissioning,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(is_root_commissioning_open),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(publish_result),
    /* K3   */  be_nested_str_weak(_X7B_X22Matter_X22_X3A_X7B_X22Commissioning_X22_X3A0_X7D_X7D),
    /* K4   */  be_nested_str_weak(Matter),
    /* K5   */  be_nested_str_weak(commissioning_open),
    /* K6   */  be_nested_str_weak(mdns_remove_PASE),
    /* K7   */  be_nested_str_weak(commissioning_iterations),
    /* K8   */  be_nested_str_weak(commissioning_discriminator),
    /* K9   */  be_nested_str_weak(commissioning_salt),
    /* K10  */  be_nested_str_weak(commissioning_w0),
    /* K11  */  be_nested_str_weak(commissioning_L),
    /* K12  */  be_nested_str_weak(commissioning_admin_fabric),
    }),
    be_str_weak(stop_basic_commissioning),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x78060004,  //  0002  JMPF	R1	#0008
      0xB8060200,  //  0003  GETNGBL	R1	K1
      0x8C040302,  //  0004  GETMET	R1	R1	K2
      0x580C0003,  //  0005  LDCONST	R3	K3
      0x58100004,  //  0006  LDCONST	R4	K4
      0x7C040600,  //  0007  CALL	R1	3
      0x4C040000,  //  0008  LDNIL	R1
      0x90020A01,  //  0009  SETMBR	R0	K5	R1
      0x8C040106,  //  000A  GETMET	R1	R0	K6
      0x7C040200,  //  000B  CALL	R1	1
      0x4C040000,  //  000C  LDNIL	R1
      0x90020E01,  //  000D  SETMBR	R0	K7	R1
      0x4C040000,  //  000E  LDNIL	R1
      0x90021001,  //  000F  SETMBR	R0	K8	R1
      0x4C040000,  //  0010  LDNIL	R1
      0x90021201,  //  0011  SETMBR	R0	K9	R1
      0x4C040000,  //  0012  LDNIL	R1
      0x90021401,  //  0013  SETMBR	R0	K10	R1
      0x4C040000,  //  0014  LDNIL	R1
      0x90021601,  //  0015  SETMBR	R0	K11	R1
      0x4C040000,  //  0016  LDNIL	R1
      0x90021801,  //  0017  SETMBR	R0	K12	R1
      0x80000000,  //  0018  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _trigger_read_sensors
********************************************************************/
be_local_closure(Matter_Device__trigger_read_sensors,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(json),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(read_sensors),
    /* K3   */  be_nested_str_weak(load),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(plugins),
    /* K6   */  be_nested_str_weak(parse_sensors),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str_weak(log),
    /* K9   */  be_nested_str_weak(MTR_X3A_X20unable_X20to_X20parse_X20read_sensors_X3A_X20),
    /* K10  */  be_const_int(3),
    }),
    be_str_weak(_trigger_read_sensors),
    &be_const_str_solidified,
    ( &(const binstruction[37]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xB80A0200,  //  0001  GETNGBL	R2	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0x7C080200,  //  0003  CALL	R2	1
      0x4C0C0000,  //  0004  LDNIL	R3
      0x1C0C0403,  //  0005  EQ	R3	R2	R3
      0x780E0000,  //  0006  JMPF	R3	#0008
      0x80000600,  //  0007  RET	0
      0x8C0C0303,  //  0008  GETMET	R3	R1	K3
      0x5C140400,  //  0009  MOVE	R5	R2
      0x7C0C0400,  //  000A  CALL	R3	2
      0x4C100000,  //  000B  LDNIL	R4
      0x20100604,  //  000C  NE	R4	R3	R4
      0x7812000D,  //  000D  JMPF	R4	#001C
      0x58100004,  //  000E  LDCONST	R4	K4
      0x6014000C,  //  000F  GETGBL	R5	G12
      0x88180105,  //  0010  GETMBR	R6	R0	K5
      0x7C140200,  //  0011  CALL	R5	1
      0x14140805,  //  0012  LT	R5	R4	R5
      0x78160006,  //  0013  JMPF	R5	#001B
      0x88140105,  //  0014  GETMBR	R5	R0	K5
      0x94140A04,  //  0015  GETIDX	R5	R5	R4
      0x8C140B06,  //  0016  GETMET	R5	R5	K6
      0x5C1C0600,  //  0017  MOVE	R7	R3
      0x7C140400,  //  0018  CALL	R5	2
      0x00100907,  //  0019  ADD	R4	R4	K7
      0x7001FFF3,  //  001A  JMP		#000F
      0x70020007,  //  001B  JMP		#0024
      0xB8120200,  //  001C  GETNGBL	R4	K1
      0x8C100908,  //  001D  GETMET	R4	R4	K8
      0x60180008,  //  001E  GETGBL	R6	G8
      0x5C1C0400,  //  001F  MOVE	R7	R2
      0x7C180200,  //  0020  CALL	R6	1
      0x001A1206,  //  0021  ADD	R6	K9	R6
      0x581C000A,  //  0022  LDCONST	R7	K10
      0x7C100600,  //  0023  CALL	R4	3
      0x80000000,  //  0024  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _init_basic_commissioning
********************************************************************/
be_local_closure(Matter_Device__init_basic_commissioning,   /* name */
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
    /* K0   */  be_nested_str_weak(sessions),
    /* K1   */  be_nested_str_weak(count_active_fabrics),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(start_root_basic_commissioning),
    }),
    be_str_weak(_init_basic_commissioning),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x1C040302,  //  0003  EQ	R1	R1	K2
      0x78060001,  //  0004  JMPF	R1	#0007
      0x8C040103,  //  0005  GETMET	R1	R0	K3
      0x7C040200,  //  0006  CALL	R1	1
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: compute_qrcode_content
********************************************************************/
be_local_closure(Matter_Device_compute_qrcode_content,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(resize),
    /* K1   */  be_nested_str_weak(setbits),
    /* K2   */  be_const_int(3),
    /* K3   */  be_nested_str_weak(vendorid),
    /* K4   */  be_nested_str_weak(productid),
    /* K5   */  be_nested_str_weak(root_discriminator),
    /* K6   */  be_nested_str_weak(root_passcode),
    /* K7   */  be_const_int(134217727),
    /* K8   */  be_nested_str_weak(MT_X3A),
    /* K9   */  be_nested_str_weak(matter),
    /* K10  */  be_nested_str_weak(Base38),
    /* K11  */  be_nested_str_weak(encode),
    }),
    be_str_weak(compute_qrcode_content),
    &be_const_str_solidified,
    ( &(const binstruction[40]) {  /* code */
      0x60040015,  //  0000  GETGBL	R1	G21
      0x7C040000,  //  0001  CALL	R1	0
      0x8C040300,  //  0002  GETMET	R1	R1	K0
      0x540E000A,  //  0003  LDINT	R3	11
      0x7C040400,  //  0004  CALL	R1	2
      0x8C080301,  //  0005  GETMET	R2	R1	K1
      0x58100002,  //  0006  LDCONST	R4	K2
      0x5416000F,  //  0007  LDINT	R5	16
      0x88180103,  //  0008  GETMBR	R6	R0	K3
      0x7C080800,  //  0009  CALL	R2	4
      0x8C080301,  //  000A  GETMET	R2	R1	K1
      0x54120012,  //  000B  LDINT	R4	19
      0x5416000F,  //  000C  LDINT	R5	16
      0x88180104,  //  000D  GETMBR	R6	R0	K4
      0x7C080800,  //  000E  CALL	R2	4
      0x8C080301,  //  000F  GETMET	R2	R1	K1
      0x54120024,  //  0010  LDINT	R4	37
      0x54160007,  //  0011  LDINT	R5	8
      0x541A0003,  //  0012  LDINT	R6	4
      0x7C080800,  //  0013  CALL	R2	4
      0x8C080301,  //  0014  GETMET	R2	R1	K1
      0x5412002C,  //  0015  LDINT	R4	45
      0x5416000B,  //  0016  LDINT	R5	12
      0x88180105,  //  0017  GETMBR	R6	R0	K5
      0x541E0FFE,  //  0018  LDINT	R7	4095
      0x2C180C07,  //  0019  AND	R6	R6	R7
      0x7C080800,  //  001A  CALL	R2	4
      0x8C080301,  //  001B  GETMET	R2	R1	K1
      0x54120038,  //  001C  LDINT	R4	57
      0x5416001A,  //  001D  LDINT	R5	27
      0x88180106,  //  001E  GETMBR	R6	R0	K6
      0x2C180D07,  //  001F  AND	R6	R6	K7
      0x7C080800,  //  0020  CALL	R2	4
      0xB80A1200,  //  0021  GETNGBL	R2	K9
      0x8808050A,  //  0022  GETMBR	R2	R2	K10
      0x8C08050B,  //  0023  GETMET	R2	R2	K11
      0x5C100200,  //  0024  MOVE	R4	R1
      0x7C080400,  //  0025  CALL	R2	2
      0x000A1002,  //  0026  ADD	R2	K8	R2
      0x80040400,  //  0027  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_active_endpoints
********************************************************************/
be_local_closure(Matter_Device_get_active_endpoints,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(plugins),
    /* K1   */  be_nested_str_weak(get_endpoint),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(find),
    /* K4   */  be_nested_str_weak(push),
    /* K5   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(get_active_endpoints),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x60080012,  //  0000  GETGBL	R2	G18
      0x7C080000,  //  0001  CALL	R2	0
      0x600C0010,  //  0002  GETGBL	R3	G16
      0x88100100,  //  0003  GETMBR	R4	R0	K0
      0x7C0C0200,  //  0004  CALL	R3	1
      0xA8020011,  //  0005  EXBLK	0	#0018
      0x5C100600,  //  0006  MOVE	R4	R3
      0x7C100000,  //  0007  CALL	R4	0
      0x8C140901,  //  0008  GETMET	R5	R4	K1
      0x7C140200,  //  0009  CALL	R5	1
      0x78060002,  //  000A  JMPF	R1	#000E
      0x1C180B02,  //  000B  EQ	R6	R5	K2
      0x781A0000,  //  000C  JMPF	R6	#000E
      0x7001FFF7,  //  000D  JMP		#0006
      0x8C180503,  //  000E  GETMET	R6	R2	K3
      0x5C200A00,  //  000F  MOVE	R8	R5
      0x7C180400,  //  0010  CALL	R6	2
      0x4C1C0000,  //  0011  LDNIL	R7
      0x1C180C07,  //  0012  EQ	R6	R6	R7
      0x781A0002,  //  0013  JMPF	R6	#0017
      0x8C180504,  //  0014  GETMET	R6	R2	K4
      0x5C200A00,  //  0015  MOVE	R8	R5
      0x7C180400,  //  0016  CALL	R6	2
      0x7001FFED,  //  0017  JMP		#0006
      0x580C0005,  //  0018  LDCONST	R3	K5
      0xAC0C0200,  //  0019  CATCH	R3	1	0
      0xB0080000,  //  001A  RAISE	2	R0	R0
      0x80040400,  //  001B  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: attribute_updated
********************************************************************/
be_local_closure(Matter_Device_attribute_updated,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(Path),
    /* K2   */  be_nested_str_weak(endpoint),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(attribute),
    /* K5   */  be_nested_str_weak(message_handler),
    /* K6   */  be_nested_str_weak(im),
    /* K7   */  be_nested_str_weak(subs_shop),
    /* K8   */  be_nested_str_weak(attribute_updated_ctx),
    }),
    be_str_weak(attribute_updated),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x4C140000,  //  0000  LDNIL	R5
      0x1C140805,  //  0001  EQ	R5	R4	R5
      0x78160000,  //  0002  JMPF	R5	#0004
      0x50100000,  //  0003  LDBOOL	R4	0	0
      0xB8160000,  //  0004  GETNGBL	R5	K0
      0x8C140B01,  //  0005  GETMET	R5	R5	K1
      0x7C140200,  //  0006  CALL	R5	1
      0x90160401,  //  0007  SETMBR	R5	K2	R1
      0x90160602,  //  0008  SETMBR	R5	K3	R2
      0x90160803,  //  0009  SETMBR	R5	K4	R3
      0x88180105,  //  000A  GETMBR	R6	R0	K5
      0x88180D06,  //  000B  GETMBR	R6	R6	K6
      0x88180D07,  //  000C  GETMBR	R6	R6	K7
      0x8C180D08,  //  000D  GETMET	R6	R6	K8
      0x5C200A00,  //  000E  MOVE	R8	R5
      0x5C240800,  //  000F  MOVE	R9	R4
      0x7C180600,  //  0010  CALL	R6	3
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Device_invoke_request,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(endpoint),
    /* K2   */  be_nested_str_weak(plugins),
    /* K3   */  be_nested_str_weak(invoke_request),
    /* K4   */  be_const_int(1),
    /* K5   */  be_nested_str_weak(status),
    /* K6   */  be_nested_str_weak(matter),
    /* K7   */  be_nested_str_weak(UNSUPPORTED_ENDPOINT),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x58100000,  //  0000  LDCONST	R4	K0
      0x88140701,  //  0001  GETMBR	R5	R3	K1
      0x6018000C,  //  0002  GETGBL	R6	G12
      0x881C0102,  //  0003  GETMBR	R7	R0	K2
      0x7C180200,  //  0004  CALL	R6	1
      0x14180806,  //  0005  LT	R6	R4	R6
      0x781A000C,  //  0006  JMPF	R6	#0014
      0x88180102,  //  0007  GETMBR	R6	R0	K2
      0x94180C04,  //  0008  GETIDX	R6	R6	R4
      0x881C0D01,  //  0009  GETMBR	R7	R6	K1
      0x1C1C0E05,  //  000A  EQ	R7	R7	R5
      0x781E0005,  //  000B  JMPF	R7	#0012
      0x8C1C0D03,  //  000C  GETMET	R7	R6	K3
      0x5C240200,  //  000D  MOVE	R9	R1
      0x5C280400,  //  000E  MOVE	R10	R2
      0x5C2C0600,  //  000F  MOVE	R11	R3
      0x7C1C0800,  //  0010  CALL	R7	4
      0x80040E00,  //  0011  RET	1	R7
      0x00100904,  //  0012  ADD	R4	R4	K4
      0x7001FFED,  //  0013  JMP		#0002
      0xB81A0C00,  //  0014  GETNGBL	R6	K6
      0x88180D07,  //  0015  GETMBR	R6	R6	K7
      0x900E0A06,  //  0016  SETMBR	R3	K5	R6
      0x80000000,  //  0017  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_fabrics_saved
********************************************************************/
be_local_closure(Matter_Device_event_fabrics_saved,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(sessions),
    /* K1   */  be_nested_str_weak(count_active_fabrics),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(plugins_persist),
    /* K4   */  be_nested_str_weak(save_param),
    }),
    be_str_weak(event_fabrics_saved),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x24040302,  //  0003  GT	R1	R1	K2
      0x78060005,  //  0004  JMPF	R1	#000B
      0x88040103,  //  0005  GETMBR	R1	R0	K3
      0x74060003,  //  0006  JMPT	R1	#000B
      0x50040200,  //  0007  LDBOOL	R1	1	0
      0x90020601,  //  0008  SETMBR	R0	K3	R1
      0x8C040104,  //  0009  GETMET	R1	R0	K4
      0x7C040200,  //  000A  CALL	R1	1
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: sort_distinct
********************************************************************/
be_local_closure(Matter_Device_sort_distinct,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_const_class(be_class_Matter_Device),
    /* K1   */  be_const_int(1),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(stop_iteration),
    /* K4   */  be_nested_str_weak(remove),
    }),
    be_str_weak(sort_distinct),
    &be_const_str_solidified,
    ( &(const binstruction[53]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x60080010,  //  0001  GETGBL	R2	G16
      0x600C000C,  //  0002  GETGBL	R3	G12
      0x5C100000,  //  0003  MOVE	R4	R0
      0x7C0C0200,  //  0004  CALL	R3	1
      0x040C0701,  //  0005  SUB	R3	R3	K1
      0x400E0203,  //  0006  CONNECT	R3	K1	R3
      0x7C080200,  //  0007  CALL	R2	1
      0xA8020010,  //  0008  EXBLK	0	#001A
      0x5C0C0400,  //  0009  MOVE	R3	R2
      0x7C0C0000,  //  000A  CALL	R3	0
      0x94100003,  //  000B  GETIDX	R4	R0	R3
      0x5C140600,  //  000C  MOVE	R5	R3
      0x24180B02,  //  000D  GT	R6	R5	K2
      0x781A0008,  //  000E  JMPF	R6	#0018
      0x04180B01,  //  000F  SUB	R6	R5	K1
      0x94180006,  //  0010  GETIDX	R6	R0	R6
      0x24180C04,  //  0011  GT	R6	R6	R4
      0x781A0004,  //  0012  JMPF	R6	#0018
      0x04180B01,  //  0013  SUB	R6	R5	K1
      0x94180006,  //  0014  GETIDX	R6	R0	R6
      0x98000A06,  //  0015  SETIDX	R0	R5	R6
      0x04140B01,  //  0016  SUB	R5	R5	K1
      0x7001FFF4,  //  0017  JMP		#000D
      0x98000A04,  //  0018  SETIDX	R0	R5	R4
      0x7001FFEE,  //  0019  JMP		#0009
      0x58080003,  //  001A  LDCONST	R2	K3
      0xAC080200,  //  001B  CATCH	R2	1	0
      0xB0080000,  //  001C  RAISE	2	R0	R0
      0x58080001,  //  001D  LDCONST	R2	K1
      0x600C000C,  //  001E  GETGBL	R3	G12
      0x5C100000,  //  001F  MOVE	R4	R0
      0x7C0C0200,  //  0020  CALL	R3	1
      0x180C0701,  //  0021  LE	R3	R3	K1
      0x780E0000,  //  0022  JMPF	R3	#0024
      0x80040000,  //  0023  RET	1	R0
      0x940C0102,  //  0024  GETIDX	R3	R0	K2
      0x6010000C,  //  0025  GETGBL	R4	G12
      0x5C140000,  //  0026  MOVE	R5	R0
      0x7C100200,  //  0027  CALL	R4	1
      0x14100404,  //  0028  LT	R4	R2	R4
      0x78120009,  //  0029  JMPF	R4	#0034
      0x94100002,  //  002A  GETIDX	R4	R0	R2
      0x1C100803,  //  002B  EQ	R4	R4	R3
      0x78120003,  //  002C  JMPF	R4	#0031
      0x8C100104,  //  002D  GETMET	R4	R0	K4
      0x5C180400,  //  002E  MOVE	R6	R2
      0x7C100400,  //  002F  CALL	R4	2
      0x70020001,  //  0030  JMP		#0033
      0x940C0002,  //  0031  GETIDX	R3	R0	R2
      0x00080501,  //  0032  ADD	R2	R2	K1
      0x7001FFF0,  //  0033  JMP		#0025
      0x80040000,  //  0034  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: k2l_num
********************************************************************/
be_local_closure(Matter_Device_k2l_num,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_const_class(be_class_Matter_Device),
    /* K1   */  be_nested_str_weak(keys),
    /* K2   */  be_nested_str_weak(push),
    /* K3   */  be_nested_str_weak(stop_iteration),
    /* K4   */  be_const_int(1),
    /* K5   */  be_const_int(0),
    }),
    be_str_weak(k2l_num),
    &be_const_str_solidified,
    ( &(const binstruction[52]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x60080012,  //  0001  GETGBL	R2	G18
      0x7C080000,  //  0002  CALL	R2	0
      0x4C0C0000,  //  0003  LDNIL	R3
      0x1C0C0003,  //  0004  EQ	R3	R0	R3
      0x780E0000,  //  0005  JMPF	R3	#0007
      0x80040400,  //  0006  RET	1	R2
      0x600C0010,  //  0007  GETGBL	R3	G16
      0x8C100101,  //  0008  GETMET	R4	R0	K1
      0x7C100200,  //  0009  CALL	R4	1
      0x7C0C0200,  //  000A  CALL	R3	1
      0xA8020007,  //  000B  EXBLK	0	#0014
      0x5C100600,  //  000C  MOVE	R4	R3
      0x7C100000,  //  000D  CALL	R4	0
      0x8C140502,  //  000E  GETMET	R5	R2	K2
      0x601C0009,  //  000F  GETGBL	R7	G9
      0x5C200800,  //  0010  MOVE	R8	R4
      0x7C1C0200,  //  0011  CALL	R7	1
      0x7C140400,  //  0012  CALL	R5	2
      0x7001FFF7,  //  0013  JMP		#000C
      0x580C0003,  //  0014  LDCONST	R3	K3
      0xAC0C0200,  //  0015  CATCH	R3	1	0
      0xB0080000,  //  0016  RAISE	2	R0	R0
      0x600C0010,  //  0017  GETGBL	R3	G16
      0x6010000C,  //  0018  GETGBL	R4	G12
      0x5C140400,  //  0019  MOVE	R5	R2
      0x7C100200,  //  001A  CALL	R4	1
      0x04100904,  //  001B  SUB	R4	R4	K4
      0x40120804,  //  001C  CONNECT	R4	K4	R4
      0x7C0C0200,  //  001D  CALL	R3	1
      0xA8020010,  //  001E  EXBLK	0	#0030
      0x5C100600,  //  001F  MOVE	R4	R3
      0x7C100000,  //  0020  CALL	R4	0
      0x94140404,  //  0021  GETIDX	R5	R2	R4
      0x5C180800,  //  0022  MOVE	R6	R4
      0x241C0D05,  //  0023  GT	R7	R6	K5
      0x781E0008,  //  0024  JMPF	R7	#002E
      0x041C0D04,  //  0025  SUB	R7	R6	K4
      0x941C0407,  //  0026  GETIDX	R7	R2	R7
      0x241C0E05,  //  0027  GT	R7	R7	R5
      0x781E0004,  //  0028  JMPF	R7	#002E
      0x041C0D04,  //  0029  SUB	R7	R6	K4
      0x941C0407,  //  002A  GETIDX	R7	R2	R7
      0x98080C07,  //  002B  SETIDX	R2	R6	R7
      0x04180D04,  //  002C  SUB	R6	R6	K4
      0x7001FFF4,  //  002D  JMP		#0023
      0x98080C05,  //  002E  SETIDX	R2	R6	R5
      0x7001FFEE,  //  002F  JMP		#001F
      0x580C0003,  //  0030  LDCONST	R3	K3
      0xAC0C0200,  //  0031  CATCH	R3	1	0
      0xB0080000,  //  0032  RAISE	2	R0	R0
      0x80040400,  //  0033  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: mdns_announce_op_discovery
********************************************************************/
be_local_closure(Matter_Device_mdns_announce_op_discovery,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[29]) {     /* constants */
    /* K0   */  be_nested_str_weak(mdns),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(get_device_id),
    /* K3   */  be_nested_str_weak(copy),
    /* K4   */  be_nested_str_weak(reverse),
    /* K5   */  be_nested_str_weak(get_fabric_compressed),
    /* K6   */  be_nested_str_weak(tohex),
    /* K7   */  be_nested_str_weak(_X2D),
    /* K8   */  be_nested_str_weak(tasmota),
    /* K9   */  be_nested_str_weak(log),
    /* K10  */  be_nested_str_weak(MTR_X3A_X20Operational_X20Discovery_X20node_X20_X3D_X20),
    /* K11  */  be_const_int(2),
    /* K12  */  be_nested_str_weak(eth),
    /* K13  */  be_nested_str_weak(find),
    /* K14  */  be_nested_str_weak(up),
    /* K15  */  be_nested_str_weak(format),
    /* K16  */  be_nested_str_weak(MTR_X3A_X20adding_X20mDNS_X20on_X20_X25s_X20_X27_X25s_X27_X20ptr_X20to_X20_X60_X25s_X2Elocal_X60),
    /* K17  */  be_nested_str_weak(hostname_eth),
    /* K18  */  be_const_int(3),
    /* K19  */  be_nested_str_weak(add_service),
    /* K20  */  be_nested_str_weak(_matter),
    /* K21  */  be_nested_str_weak(_tcp),
    /* K22  */  be_nested_str_weak(_I),
    /* K23  */  be_nested_str_weak(MTR_X3A_X20adding_X20subtype_X3A_X20),
    /* K24  */  be_nested_str_weak(add_subtype),
    /* K25  */  be_nested_str_weak(wifi),
    /* K26  */  be_nested_str_weak(hostname_wifi),
    /* K27  */  be_nested_str_weak(MTR_X3A_X20Exception),
    /* K28  */  be_nested_str_weak(_X7C),
    }),
    be_str_weak(mdns_announce_op_discovery),
    &be_const_str_solidified,
    ( &(const binstruction[122]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0xA8020064,  //  0002  EXBLK	0	#0068
      0x8C100302,  //  0003  GETMET	R4	R1	K2
      0x7C100200,  //  0004  CALL	R4	1
      0x8C100903,  //  0005  GETMET	R4	R4	K3
      0x7C100200,  //  0006  CALL	R4	1
      0x8C100904,  //  0007  GETMET	R4	R4	K4
      0x7C100200,  //  0008  CALL	R4	1
      0x8C140305,  //  0009  GETMET	R5	R1	K5
      0x7C140200,  //  000A  CALL	R5	1
      0x8C180B06,  //  000B  GETMET	R6	R5	K6
      0x7C180200,  //  000C  CALL	R6	1
      0x00180D07,  //  000D  ADD	R6	R6	K7
      0x8C1C0906,  //  000E  GETMET	R7	R4	K6
      0x7C1C0200,  //  000F  CALL	R7	1
      0x00180C07,  //  0010  ADD	R6	R6	R7
      0xB81E1000,  //  0011  GETNGBL	R7	K8
      0x8C1C0F09,  //  0012  GETMET	R7	R7	K9
      0x00261406,  //  0013  ADD	R9	K10	R6
      0x5828000B,  //  0014  LDCONST	R10	K11
      0x7C1C0600,  //  0015  CALL	R7	3
      0xB81E1000,  //  0016  GETNGBL	R7	K8
      0x8C1C0F0C,  //  0017  GETMET	R7	R7	K12
      0x7C1C0200,  //  0018  CALL	R7	1
      0x8C1C0F0D,  //  0019  GETMET	R7	R7	K13
      0x5824000E,  //  001A  LDCONST	R9	K14
      0x7C1C0400,  //  001B  CALL	R7	2
      0x781E0020,  //  001C  JMPF	R7	#003E
      0xB81E1000,  //  001D  GETNGBL	R7	K8
      0x8C1C0F09,  //  001E  GETMET	R7	R7	K9
      0x8C24070F,  //  001F  GETMET	R9	R3	K15
      0x582C0010,  //  0020  LDCONST	R11	K16
      0x5830000C,  //  0021  LDCONST	R12	K12
      0x5C340C00,  //  0022  MOVE	R13	R6
      0x88380111,  //  0023  GETMBR	R14	R0	K17
      0x7C240A00,  //  0024  CALL	R9	5
      0x58280012,  //  0025  LDCONST	R10	K18
      0x7C1C0600,  //  0026  CALL	R7	3
      0x8C1C0513,  //  0027  GETMET	R7	R2	K19
      0x58240014,  //  0028  LDCONST	R9	K20
      0x58280015,  //  0029  LDCONST	R10	K21
      0x542E15A3,  //  002A  LDINT	R11	5540
      0x4C300000,  //  002B  LDNIL	R12
      0x5C340C00,  //  002C  MOVE	R13	R6
      0x88380111,  //  002D  GETMBR	R14	R0	K17
      0x7C1C0E00,  //  002E  CALL	R7	7
      0x8C1C0B06,  //  002F  GETMET	R7	R5	K6
      0x7C1C0200,  //  0030  CALL	R7	1
      0x001E2C07,  //  0031  ADD	R7	K22	R7
      0xB8221000,  //  0032  GETNGBL	R8	K8
      0x8C201109,  //  0033  GETMET	R8	R8	K9
      0x002A2E07,  //  0034  ADD	R10	K23	R7
      0x582C0012,  //  0035  LDCONST	R11	K18
      0x7C200600,  //  0036  CALL	R8	3
      0x8C200518,  //  0037  GETMET	R8	R2	K24
      0x58280014,  //  0038  LDCONST	R10	K20
      0x582C0015,  //  0039  LDCONST	R11	K21
      0x5C300C00,  //  003A  MOVE	R12	R6
      0x88340111,  //  003B  GETMBR	R13	R0	K17
      0x5C380E00,  //  003C  MOVE	R14	R7
      0x7C200C00,  //  003D  CALL	R8	6
      0xB81E1000,  //  003E  GETNGBL	R7	K8
      0x8C1C0F19,  //  003F  GETMET	R7	R7	K25
      0x7C1C0200,  //  0040  CALL	R7	1
      0x8C1C0F0D,  //  0041  GETMET	R7	R7	K13
      0x5824000E,  //  0042  LDCONST	R9	K14
      0x7C1C0400,  //  0043  CALL	R7	2
      0x781E0020,  //  0044  JMPF	R7	#0066
      0xB81E1000,  //  0045  GETNGBL	R7	K8
      0x8C1C0F09,  //  0046  GETMET	R7	R7	K9
      0x8C24070F,  //  0047  GETMET	R9	R3	K15
      0x582C0010,  //  0048  LDCONST	R11	K16
      0x58300019,  //  0049  LDCONST	R12	K25
      0x5C340C00,  //  004A  MOVE	R13	R6
      0x8838011A,  //  004B  GETMBR	R14	R0	K26
      0x7C240A00,  //  004C  CALL	R9	5
      0x58280012,  //  004D  LDCONST	R10	K18
      0x7C1C0600,  //  004E  CALL	R7	3
      0x8C1C0513,  //  004F  GETMET	R7	R2	K19
      0x58240014,  //  0050  LDCONST	R9	K20
      0x58280015,  //  0051  LDCONST	R10	K21
      0x542E15A3,  //  0052  LDINT	R11	5540
      0x4C300000,  //  0053  LDNIL	R12
      0x5C340C00,  //  0054  MOVE	R13	R6
      0x8838011A,  //  0055  GETMBR	R14	R0	K26
      0x7C1C0E00,  //  0056  CALL	R7	7
      0x8C1C0B06,  //  0057  GETMET	R7	R5	K6
      0x7C1C0200,  //  0058  CALL	R7	1
      0x001E2C07,  //  0059  ADD	R7	K22	R7
      0xB8221000,  //  005A  GETNGBL	R8	K8
      0x8C201109,  //  005B  GETMET	R8	R8	K9
      0x002A2E07,  //  005C  ADD	R10	K23	R7
      0x582C0012,  //  005D  LDCONST	R11	K18
      0x7C200600,  //  005E  CALL	R8	3
      0x8C200518,  //  005F  GETMET	R8	R2	K24
      0x58280014,  //  0060  LDCONST	R10	K20
      0x582C0015,  //  0061  LDCONST	R11	K21
      0x5C300C00,  //  0062  MOVE	R12	R6
      0x8834011A,  //  0063  GETMBR	R13	R0	K26
      0x5C380E00,  //  0064  MOVE	R14	R7
      0x7C200C00,  //  0065  CALL	R8	6
      0xA8040001,  //  0066  EXBLK	1	1
      0x70020010,  //  0067  JMP		#0079
      0xAC100002,  //  0068  CATCH	R4	0	2
      0x7002000D,  //  0069  JMP		#0078
      0xB81A1000,  //  006A  GETNGBL	R6	K8
      0x8C180D09,  //  006B  GETMET	R6	R6	K9
      0x60200008,  //  006C  GETGBL	R8	G8
      0x5C240800,  //  006D  MOVE	R9	R4
      0x7C200200,  //  006E  CALL	R8	1
      0x00223608,  //  006F  ADD	R8	K27	R8
      0x0020111C,  //  0070  ADD	R8	R8	K28
      0x60240008,  //  0071  GETGBL	R9	G8
      0x5C280A00,  //  0072  MOVE	R10	R5
      0x7C240200,  //  0073  CALL	R9	1
      0x00201009,  //  0074  ADD	R8	R8	R9
      0x5824000B,  //  0075  LDCONST	R9	K11
      0x7C180600,  //  0076  CALL	R6	3
      0x70020000,  //  0077  JMP		#0079
      0xB0080000,  //  0078  RAISE	2	R0	R0
      0x80000000,  //  0079  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(Matter_Device_every_second,   /* name */
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
    /* K0   */  be_nested_str_weak(sessions),
    /* K1   */  be_nested_str_weak(every_second),
    /* K2   */  be_nested_str_weak(message_handler),
    /* K3   */  be_nested_str_weak(commissioning_open),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(time_reached),
    }),
    be_str_weak(every_second),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x88040102,  //  0003  GETMBR	R1	R0	K2
      0x8C040301,  //  0004  GETMET	R1	R1	K1
      0x7C040200,  //  0005  CALL	R1	1
      0x88040103,  //  0006  GETMBR	R1	R0	K3
      0x4C080000,  //  0007  LDNIL	R2
      0x20040202,  //  0008  NE	R1	R1	R2
      0x78060006,  //  0009  JMPF	R1	#0011
      0xB8060800,  //  000A  GETNGBL	R1	K4
      0x8C040305,  //  000B  GETMET	R1	R1	K5
      0x880C0103,  //  000C  GETMBR	R3	R0	K3
      0x7C040400,  //  000D  CALL	R1	2
      0x78060001,  //  000E  JMPF	R1	#0011
      0x4C040000,  //  000F  LDNIL	R1
      0x90020601,  //  0010  SETMBR	R0	K3	R1
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_plugin_class_arg
********************************************************************/
be_local_closure(Matter_Device_get_plugin_class_arg,   /* name */
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
    /* K0   */  be_nested_str_weak(plugins_classes),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(ARG),
    /* K3   */  be_nested_str_weak(),
    }),
    be_str_weak(get_plugin_class_arg),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x780A0001,  //  0004  JMPF	R2	#0007
      0x880C0502,  //  0005  GETMBR	R3	R2	K2
      0x70020000,  //  0006  JMP		#0008
      0x580C0003,  //  0007  LDCONST	R3	K3
      0x80040600,  //  0008  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_root_commissioning_open
********************************************************************/
be_local_closure(Matter_Device_is_root_commissioning_open,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(commissioning_open),
    /* K1   */  be_nested_str_weak(commissioning_admin_fabric),
    }),
    be_str_weak(is_root_commissioning_open),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060003,  //  0003  JMPF	R1	#0008
      0x88040101,  //  0004  GETMBR	R1	R0	K1
      0x4C080000,  //  0005  LDNIL	R2
      0x1C040202,  //  0006  EQ	R1	R1	R2
      0x74060000,  //  0007  JMPT	R1	#0009
      0x50040001,  //  0008  LDBOOL	R1	0	1
      0x50040200,  //  0009  LDBOOL	R1	1	0
      0x80040200,  //  000A  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: mdns_announce_PASE
********************************************************************/
be_local_closure(Matter_Device_mdns_announce_PASE,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[44]) {     /* constants */
    /* K0   */  be_nested_str_weak(mdns),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(crypto),
    /* K3   */  be_nested_str_weak(VP),
    /* K4   */  be_nested_str_weak(vendorid),
    /* K5   */  be_nested_str_weak(_X2B),
    /* K6   */  be_nested_str_weak(productid),
    /* K7   */  be_nested_str_weak(D),
    /* K8   */  be_nested_str_weak(commissioning_discriminator),
    /* K9   */  be_nested_str_weak(CM),
    /* K10  */  be_const_int(1),
    /* K11  */  be_nested_str_weak(T),
    /* K12  */  be_const_int(0),
    /* K13  */  be_nested_str_weak(SII),
    /* K14  */  be_nested_str_weak(SAI),
    /* K15  */  be_nested_str_weak(commissioning_instance_wifi),
    /* K16  */  be_nested_str_weak(random),
    /* K17  */  be_nested_str_weak(tohex),
    /* K18  */  be_nested_str_weak(commissioning_instance_eth),
    /* K19  */  be_nested_str_weak(hostname_eth),
    /* K20  */  be_nested_str_weak(tasmota),
    /* K21  */  be_nested_str_weak(log),
    /* K22  */  be_nested_str_weak(format),
    /* K23  */  be_nested_str_weak(MTR_X3A_X20calling_X20mdns_X2Eadd_service_X28_X25s_X2C_X20_X25s_X2C_X20_X25i_X2C_X20_X25s_X2C_X20_X25s_X2C_X20_X25s_X29),
    /* K24  */  be_nested_str_weak(_matterc),
    /* K25  */  be_nested_str_weak(_udp),
    /* K26  */  be_const_int(3),
    /* K27  */  be_nested_str_weak(add_service),
    /* K28  */  be_nested_str_weak(mdns_pase_eth),
    /* K29  */  be_nested_str_weak(MTR_X3A_X20announce_X20mDNS_X20on_X20_X25s_X20_X27_X25s_X27_X20ptr_X20to_X20_X60_X25s_X2Elocal_X60),
    /* K30  */  be_nested_str_weak(eth),
    /* K31  */  be_const_int(2),
    /* K32  */  be_nested_str_weak(_L),
    /* K33  */  be_nested_str_weak(MTR_X3A_X20adding_X20subtype_X3A_X20),
    /* K34  */  be_nested_str_weak(add_subtype),
    /* K35  */  be_nested_str_weak(_S),
    /* K36  */  be_nested_str_weak(_V),
    /* K37  */  be_nested_str_weak(_CM1),
    /* K38  */  be_nested_str_weak(hostname_wifi),
    /* K39  */  be_nested_str_weak(mdns_pase_wifi),
    /* K40  */  be_nested_str_weak(MTR_X3A_X20starting_X20mDNS_X20on_X20_X25s_X20_X27_X25s_X27_X20ptr_X20to_X20_X60_X25s_X2Elocal_X60),
    /* K41  */  be_nested_str_weak(wifi),
    /* K42  */  be_nested_str_weak(MTR_X3A_X20Exception),
    /* K43  */  be_nested_str_weak(_X7C),
    }),
    be_str_weak(mdns_announce_PASE),
    &be_const_str_solidified,
    ( &(const binstruction[267]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0xA40E0400,  //  0002  IMPORT	R3	K2
      0x60100013,  //  0003  GETGBL	R4	G19
      0x7C100000,  //  0004  CALL	R4	0
      0x60140008,  //  0005  GETGBL	R5	G8
      0x88180104,  //  0006  GETMBR	R6	R0	K4
      0x7C140200,  //  0007  CALL	R5	1
      0x00140B05,  //  0008  ADD	R5	R5	K5
      0x60180008,  //  0009  GETGBL	R6	G8
      0x881C0106,  //  000A  GETMBR	R7	R0	K6
      0x7C180200,  //  000B  CALL	R6	1
      0x00140A06,  //  000C  ADD	R5	R5	R6
      0x98120605,  //  000D  SETIDX	R4	K3	R5
      0x88140108,  //  000E  GETMBR	R5	R0	K8
      0x98120E05,  //  000F  SETIDX	R4	K7	R5
      0x9812130A,  //  0010  SETIDX	R4	K9	K10
      0x9812170C,  //  0011  SETIDX	R4	K11	K12
      0x54161387,  //  0012  LDINT	R5	5000
      0x98121A05,  //  0013  SETIDX	R4	K13	R5
      0x5416012B,  //  0014  LDINT	R5	300
      0x98121C05,  //  0015  SETIDX	R4	K14	R5
      0x8C140710,  //  0016  GETMET	R5	R3	K16
      0x541E0007,  //  0017  LDINT	R7	8
      0x7C140400,  //  0018  CALL	R5	2
      0x8C140B11,  //  0019  GETMET	R5	R5	K17
      0x7C140200,  //  001A  CALL	R5	1
      0x90021E05,  //  001B  SETMBR	R0	K15	R5
      0x8C140710,  //  001C  GETMET	R5	R3	K16
      0x541E0007,  //  001D  LDINT	R7	8
      0x7C140400,  //  001E  CALL	R5	2
      0x8C140B11,  //  001F  GETMET	R5	R5	K17
      0x7C140200,  //  0020  CALL	R5	1
      0x90022405,  //  0021  SETMBR	R0	K18	R5
      0xA80200D5,  //  0022  EXBLK	0	#00F9
      0x88140113,  //  0023  GETMBR	R5	R0	K19
      0x78160067,  //  0024  JMPF	R5	#008D
      0xB8162800,  //  0025  GETNGBL	R5	K20
      0x8C140B15,  //  0026  GETMET	R5	R5	K21
      0x8C1C0516,  //  0027  GETMET	R7	R2	K22
      0x58240017,  //  0028  LDCONST	R9	K23
      0x58280018,  //  0029  LDCONST	R10	K24
      0x582C0019,  //  002A  LDCONST	R11	K25
      0x543215A3,  //  002B  LDINT	R12	5540
      0x60340008,  //  002C  GETGBL	R13	G8
      0x5C380800,  //  002D  MOVE	R14	R4
      0x7C340200,  //  002E  CALL	R13	1
      0x88380112,  //  002F  GETMBR	R14	R0	K18
      0x883C0113,  //  0030  GETMBR	R15	R0	K19
      0x7C1C1000,  //  0031  CALL	R7	8
      0x5820001A,  //  0032  LDCONST	R8	K26
      0x7C140600,  //  0033  CALL	R5	3
      0x8C14031B,  //  0034  GETMET	R5	R1	K27
      0x581C0018,  //  0035  LDCONST	R7	K24
      0x58200019,  //  0036  LDCONST	R8	K25
      0x542615A3,  //  0037  LDINT	R9	5540
      0x5C280800,  //  0038  MOVE	R10	R4
      0x882C0112,  //  0039  GETMBR	R11	R0	K18
      0x88300113,  //  003A  GETMBR	R12	R0	K19
      0x7C140E00,  //  003B  CALL	R5	7
      0x50140200,  //  003C  LDBOOL	R5	1	0
      0x90023805,  //  003D  SETMBR	R0	K28	R5
      0xB8162800,  //  003E  GETNGBL	R5	K20
      0x8C140B15,  //  003F  GETMET	R5	R5	K21
      0x8C1C0516,  //  0040  GETMET	R7	R2	K22
      0x5824001D,  //  0041  LDCONST	R9	K29
      0x5828001E,  //  0042  LDCONST	R10	K30
      0x882C0112,  //  0043  GETMBR	R11	R0	K18
      0x88300113,  //  0044  GETMBR	R12	R0	K19
      0x7C1C0A00,  //  0045  CALL	R7	5
      0x5820001F,  //  0046  LDCONST	R8	K31
      0x7C140600,  //  0047  CALL	R5	3
      0x60140008,  //  0048  GETGBL	R5	G8
      0x88180108,  //  0049  GETMBR	R6	R0	K8
      0x541E0FFE,  //  004A  LDINT	R7	4095
      0x2C180C07,  //  004B  AND	R6	R6	R7
      0x7C140200,  //  004C  CALL	R5	1
      0x00164005,  //  004D  ADD	R5	K32	R5
      0xB81A2800,  //  004E  GETNGBL	R6	K20
      0x8C180D15,  //  004F  GETMET	R6	R6	K21
      0x00224205,  //  0050  ADD	R8	K33	R5
      0x5824001F,  //  0051  LDCONST	R9	K31
      0x7C180600,  //  0052  CALL	R6	3
      0x8C180322,  //  0053  GETMET	R6	R1	K34
      0x58200018,  //  0054  LDCONST	R8	K24
      0x58240019,  //  0055  LDCONST	R9	K25
      0x88280112,  //  0056  GETMBR	R10	R0	K18
      0x882C0113,  //  0057  GETMBR	R11	R0	K19
      0x5C300A00,  //  0058  MOVE	R12	R5
      0x7C180C00,  //  0059  CALL	R6	6
      0x60180008,  //  005A  GETGBL	R6	G8
      0x881C0108,  //  005B  GETMBR	R7	R0	K8
      0x54220EFF,  //  005C  LDINT	R8	3840
      0x2C1C0E08,  //  005D  AND	R7	R7	R8
      0x54220007,  //  005E  LDINT	R8	8
      0x3C1C0E08,  //  005F  SHR	R7	R7	R8
      0x7C180200,  //  0060  CALL	R6	1
      0x001A4606,  //  0061  ADD	R6	K35	R6
      0x5C140C00,  //  0062  MOVE	R5	R6
      0xB81A2800,  //  0063  GETNGBL	R6	K20
      0x8C180D15,  //  0064  GETMET	R6	R6	K21
      0x00224205,  //  0065  ADD	R8	K33	R5
      0x5824001F,  //  0066  LDCONST	R9	K31
      0x7C180600,  //  0067  CALL	R6	3
      0x8C180322,  //  0068  GETMET	R6	R1	K34
      0x58200018,  //  0069  LDCONST	R8	K24
      0x58240019,  //  006A  LDCONST	R9	K25
      0x88280112,  //  006B  GETMBR	R10	R0	K18
      0x882C0113,  //  006C  GETMBR	R11	R0	K19
      0x5C300A00,  //  006D  MOVE	R12	R5
      0x7C180C00,  //  006E  CALL	R6	6
      0x60180008,  //  006F  GETGBL	R6	G8
      0x881C0104,  //  0070  GETMBR	R7	R0	K4
      0x7C180200,  //  0071  CALL	R6	1
      0x001A4806,  //  0072  ADD	R6	K36	R6
      0x5C140C00,  //  0073  MOVE	R5	R6
      0xB81A2800,  //  0074  GETNGBL	R6	K20
      0x8C180D15,  //  0075  GETMET	R6	R6	K21
      0x00224205,  //  0076  ADD	R8	K33	R5
      0x5824001F,  //  0077  LDCONST	R9	K31
      0x7C180600,  //  0078  CALL	R6	3
      0x8C180322,  //  0079  GETMET	R6	R1	K34
      0x58200018,  //  007A  LDCONST	R8	K24
      0x58240019,  //  007B  LDCONST	R9	K25
      0x88280112,  //  007C  GETMBR	R10	R0	K18
      0x882C0113,  //  007D  GETMBR	R11	R0	K19
      0x5C300A00,  //  007E  MOVE	R12	R5
      0x7C180C00,  //  007F  CALL	R6	6
      0x58140025,  //  0080  LDCONST	R5	K37
      0xB81A2800,  //  0081  GETNGBL	R6	K20
      0x8C180D15,  //  0082  GETMET	R6	R6	K21
      0x00224205,  //  0083  ADD	R8	K33	R5
      0x5824001F,  //  0084  LDCONST	R9	K31
      0x7C180600,  //  0085  CALL	R6	3
      0x8C180322,  //  0086  GETMET	R6	R1	K34
      0x58200018,  //  0087  LDCONST	R8	K24
      0x58240019,  //  0088  LDCONST	R9	K25
      0x88280112,  //  0089  GETMBR	R10	R0	K18
      0x882C0113,  //  008A  GETMBR	R11	R0	K19
      0x5C300A00,  //  008B  MOVE	R12	R5
      0x7C180C00,  //  008C  CALL	R6	6
      0x88140126,  //  008D  GETMBR	R5	R0	K38
      0x78160067,  //  008E  JMPF	R5	#00F7
      0xB8162800,  //  008F  GETNGBL	R5	K20
      0x8C140B15,  //  0090  GETMET	R5	R5	K21
      0x8C1C0516,  //  0091  GETMET	R7	R2	K22
      0x58240017,  //  0092  LDCONST	R9	K23
      0x58280018,  //  0093  LDCONST	R10	K24
      0x582C0019,  //  0094  LDCONST	R11	K25
      0x543215A3,  //  0095  LDINT	R12	5540
      0x60340008,  //  0096  GETGBL	R13	G8
      0x5C380800,  //  0097  MOVE	R14	R4
      0x7C340200,  //  0098  CALL	R13	1
      0x8838010F,  //  0099  GETMBR	R14	R0	K15
      0x883C0126,  //  009A  GETMBR	R15	R0	K38
      0x7C1C1000,  //  009B  CALL	R7	8
      0x5820001A,  //  009C  LDCONST	R8	K26
      0x7C140600,  //  009D  CALL	R5	3
      0x8C14031B,  //  009E  GETMET	R5	R1	K27
      0x581C0018,  //  009F  LDCONST	R7	K24
      0x58200019,  //  00A0  LDCONST	R8	K25
      0x542615A3,  //  00A1  LDINT	R9	5540
      0x5C280800,  //  00A2  MOVE	R10	R4
      0x882C010F,  //  00A3  GETMBR	R11	R0	K15
      0x88300126,  //  00A4  GETMBR	R12	R0	K38
      0x7C140E00,  //  00A5  CALL	R5	7
      0x50140200,  //  00A6  LDBOOL	R5	1	0
      0x90024E05,  //  00A7  SETMBR	R0	K39	R5
      0xB8162800,  //  00A8  GETNGBL	R5	K20
      0x8C140B15,  //  00A9  GETMET	R5	R5	K21
      0x8C1C0516,  //  00AA  GETMET	R7	R2	K22
      0x58240028,  //  00AB  LDCONST	R9	K40
      0x58280029,  //  00AC  LDCONST	R10	K41
      0x882C010F,  //  00AD  GETMBR	R11	R0	K15
      0x88300126,  //  00AE  GETMBR	R12	R0	K38
      0x7C1C0A00,  //  00AF  CALL	R7	5
      0x5820001F,  //  00B0  LDCONST	R8	K31
      0x7C140600,  //  00B1  CALL	R5	3
      0x60140008,  //  00B2  GETGBL	R5	G8
      0x88180108,  //  00B3  GETMBR	R6	R0	K8
      0x541E0FFE,  //  00B4  LDINT	R7	4095
      0x2C180C07,  //  00B5  AND	R6	R6	R7
      0x7C140200,  //  00B6  CALL	R5	1
      0x00164005,  //  00B7  ADD	R5	K32	R5
      0xB81A2800,  //  00B8  GETNGBL	R6	K20
      0x8C180D15,  //  00B9  GETMET	R6	R6	K21
      0x00224205,  //  00BA  ADD	R8	K33	R5
      0x5824001F,  //  00BB  LDCONST	R9	K31
      0x7C180600,  //  00BC  CALL	R6	3
      0x8C180322,  //  00BD  GETMET	R6	R1	K34
      0x58200018,  //  00BE  LDCONST	R8	K24
      0x58240019,  //  00BF  LDCONST	R9	K25
      0x8828010F,  //  00C0  GETMBR	R10	R0	K15
      0x882C0126,  //  00C1  GETMBR	R11	R0	K38
      0x5C300A00,  //  00C2  MOVE	R12	R5
      0x7C180C00,  //  00C3  CALL	R6	6
      0x60180008,  //  00C4  GETGBL	R6	G8
      0x881C0108,  //  00C5  GETMBR	R7	R0	K8
      0x54220EFF,  //  00C6  LDINT	R8	3840
      0x2C1C0E08,  //  00C7  AND	R7	R7	R8
      0x54220007,  //  00C8  LDINT	R8	8
      0x3C1C0E08,  //  00C9  SHR	R7	R7	R8
      0x7C180200,  //  00CA  CALL	R6	1
      0x001A4606,  //  00CB  ADD	R6	K35	R6
      0x5C140C00,  //  00CC  MOVE	R5	R6
      0xB81A2800,  //  00CD  GETNGBL	R6	K20
      0x8C180D15,  //  00CE  GETMET	R6	R6	K21
      0x00224205,  //  00CF  ADD	R8	K33	R5
      0x5824001F,  //  00D0  LDCONST	R9	K31
      0x7C180600,  //  00D1  CALL	R6	3
      0x8C180322,  //  00D2  GETMET	R6	R1	K34
      0x58200018,  //  00D3  LDCONST	R8	K24
      0x58240019,  //  00D4  LDCONST	R9	K25
      0x8828010F,  //  00D5  GETMBR	R10	R0	K15
      0x882C0126,  //  00D6  GETMBR	R11	R0	K38
      0x5C300A00,  //  00D7  MOVE	R12	R5
      0x7C180C00,  //  00D8  CALL	R6	6
      0x60180008,  //  00D9  GETGBL	R6	G8
      0x881C0104,  //  00DA  GETMBR	R7	R0	K4
      0x7C180200,  //  00DB  CALL	R6	1
      0x001A4806,  //  00DC  ADD	R6	K36	R6
      0x5C140C00,  //  00DD  MOVE	R5	R6
      0xB81A2800,  //  00DE  GETNGBL	R6	K20
      0x8C180D15,  //  00DF  GETMET	R6	R6	K21
      0x00224205,  //  00E0  ADD	R8	K33	R5
      0x5824001F,  //  00E1  LDCONST	R9	K31
      0x7C180600,  //  00E2  CALL	R6	3
      0x8C180322,  //  00E3  GETMET	R6	R1	K34
      0x58200018,  //  00E4  LDCONST	R8	K24
      0x58240019,  //  00E5  LDCONST	R9	K25
      0x8828010F,  //  00E6  GETMBR	R10	R0	K15
      0x882C0126,  //  00E7  GETMBR	R11	R0	K38
      0x5C300A00,  //  00E8  MOVE	R12	R5
      0x7C180C00,  //  00E9  CALL	R6	6
      0x58140025,  //  00EA  LDCONST	R5	K37
      0xB81A2800,  //  00EB  GETNGBL	R6	K20
      0x8C180D15,  //  00EC  GETMET	R6	R6	K21
      0x00224205,  //  00ED  ADD	R8	K33	R5
      0x5824001F,  //  00EE  LDCONST	R9	K31
      0x7C180600,  //  00EF  CALL	R6	3
      0x8C180322,  //  00F0  GETMET	R6	R1	K34
      0x58200018,  //  00F1  LDCONST	R8	K24
      0x58240019,  //  00F2  LDCONST	R9	K25
      0x8828010F,  //  00F3  GETMBR	R10	R0	K15
      0x882C0126,  //  00F4  GETMBR	R11	R0	K38
      0x5C300A00,  //  00F5  MOVE	R12	R5
      0x7C180C00,  //  00F6  CALL	R6	6
      0xA8040001,  //  00F7  EXBLK	1	1
      0x70020010,  //  00F8  JMP		#010A
      0xAC140002,  //  00F9  CATCH	R5	0	2
      0x7002000D,  //  00FA  JMP		#0109
      0xB81E2800,  //  00FB  GETNGBL	R7	K20
      0x8C1C0F15,  //  00FC  GETMET	R7	R7	K21
      0x60240008,  //  00FD  GETGBL	R9	G8
      0x5C280A00,  //  00FE  MOVE	R10	R5
      0x7C240200,  //  00FF  CALL	R9	1
      0x00265409,  //  0100  ADD	R9	K42	R9
      0x0024132B,  //  0101  ADD	R9	R9	K43
      0x60280008,  //  0102  GETGBL	R10	G8
      0x5C2C0C00,  //  0103  MOVE	R11	R6
      0x7C280200,  //  0104  CALL	R10	1
      0x0024120A,  //  0105  ADD	R9	R9	R10
      0x5828001F,  //  0106  LDCONST	R10	K31
      0x7C1C0600,  //  0107  CALL	R7	3
      0x70020000,  //  0108  JMP		#010A
      0xB0080000,  //  0109  RAISE	2	R0	R0
      0x80000000,  //  010A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: autoconf_device_map
********************************************************************/
be_local_closure(Matter_Device_autoconf_device_map,   /* name */
  be_nested_proto(
    22,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[53]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(json),
    /* K2   */  be_nested_str_weak(0),
    /* K3   */  be_nested_str_weak(type),
    /* K4   */  be_nested_str_weak(root),
    /* K5   */  be_const_int(1),
    /* K6   */  be_nested_str_weak(light),
    /* K7   */  be_nested_str_weak(get),
    /* K8   */  be_nested_str_weak(find),
    /* K9   */  be_nested_str_weak(channels),
    /* K10  */  be_nested_str_weak(),
    /* K11  */  be_const_int(0),
    /* K12  */  be_nested_str_weak(light1),
    /* K13  */  be_const_int(2),
    /* K14  */  be_nested_str_weak(light2),
    /* K15  */  be_nested_str_weak(light3),
    /* K16  */  be_nested_str_weak(tasmota),
    /* K17  */  be_nested_str_weak(cmd),
    /* K18  */  be_nested_str_weak(Status_X2013),
    /* K19  */  be_nested_str_weak(log),
    /* K20  */  be_nested_str_weak(MTR_X3A_X20Status_X2013_X20_X3D_X20),
    /* K21  */  be_const_int(3),
    /* K22  */  be_nested_str_weak(contains),
    /* K23  */  be_nested_str_weak(StatusSHT),
    /* K24  */  be_nested_str_weak(SHT),
    /* K25  */  be_nested_str_weak(format),
    /* K26  */  be_nested_str_weak(MTR_X3A_X20_X27_X25s_X27_X20_X3D_X20_X25s),
    /* K27  */  be_nested_str_weak(Relay1),
    /* K28  */  be_nested_str_weak(Relay2),
    /* K29  */  be_nested_str_weak(push),
    /* K30  */  be_nested_str_weak(MTR_X3A_X20relay1_X20_X3D_X20_X25s_X2C_X20relay2_X20_X3D_X20_X25s),
    /* K31  */  be_nested_str_weak(TiltConfig),
    /* K32  */  be_nested_str_weak(shutter_X2Btilt),
    /* K33  */  be_nested_str_weak(shutter),
    /* K34  */  be_nested_str_weak(get_power),
    /* K35  */  be_nested_str_weak(relay),
    /* K36  */  be_nested_str_weak(load),
    /* K37  */  be_nested_str_weak(read_sensors),
    /* K38  */  be_nested_str_weak(k2l),
    /* K39  */  be_nested_str_weak(Temperature),
    /* K40  */  be_nested_str_weak(_X23Temperature),
    /* K41  */  be_nested_str_weak(temperature),
    /* K42  */  be_nested_str_weak(filter),
    /* K43  */  be_nested_str_weak(stop_iteration),
    /* K44  */  be_nested_str_weak(Pressure),
    /* K45  */  be_nested_str_weak(_X23Pressure),
    /* K46  */  be_nested_str_weak(pressure),
    /* K47  */  be_nested_str_weak(Illuminance),
    /* K48  */  be_nested_str_weak(_X23Illuminance),
    /* K49  */  be_nested_str_weak(illuminance),
    /* K50  */  be_nested_str_weak(Humidity),
    /* K51  */  be_nested_str_weak(_X23Humidity),
    /* K52  */  be_nested_str_weak(humidity),
    }),
    be_str_weak(autoconf_device_map),
    &be_const_str_solidified,
    ( &(const binstruction[339]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x600C0013,  //  0002  GETGBL	R3	G19
      0x7C0C0000,  //  0003  CALL	R3	0
      0x60100013,  //  0004  GETGBL	R4	G19
      0x7C100000,  //  0005  CALL	R4	0
      0x98120704,  //  0006  SETIDX	R4	K3	K4
      0x980E0404,  //  0007  SETIDX	R3	K2	R4
      0x58100005,  //  0008  LDCONST	R4	K5
      0x50140000,  //  0009  LDBOOL	R5	0	0
      0xA41A0C00,  //  000A  IMPORT	R6	K6
      0x8C1C0D07,  //  000B  GETMET	R7	R6	K7
      0x7C1C0200,  //  000C  CALL	R7	1
      0x4C200000,  //  000D  LDNIL	R8
      0x20200E08,  //  000E  NE	R8	R7	R8
      0x78220024,  //  000F  JMPF	R8	#0035
      0x6020000C,  //  0010  GETGBL	R8	G12
      0x8C240F08,  //  0011  GETMET	R9	R7	K8
      0x582C0009,  //  0012  LDCONST	R11	K9
      0x5830000A,  //  0013  LDCONST	R12	K10
      0x7C240600,  //  0014  CALL	R9	3
      0x7C200200,  //  0015  CALL	R8	1
      0x2424110B,  //  0016  GT	R9	R8	K11
      0x7826001C,  //  0017  JMPF	R9	#0035
      0x1C241105,  //  0018  EQ	R9	R8	K5
      0x78260007,  //  0019  JMPF	R9	#0022
      0x60240008,  //  001A  GETGBL	R9	G8
      0x5C280800,  //  001B  MOVE	R10	R4
      0x7C240200,  //  001C  CALL	R9	1
      0x60280013,  //  001D  GETGBL	R10	G19
      0x7C280000,  //  001E  CALL	R10	0
      0x982A070C,  //  001F  SETIDX	R10	K3	K12
      0x980C120A,  //  0020  SETIDX	R3	R9	R10
      0x70020010,  //  0021  JMP		#0033
      0x1C24110D,  //  0022  EQ	R9	R8	K13
      0x78260007,  //  0023  JMPF	R9	#002C
      0x60240008,  //  0024  GETGBL	R9	G8
      0x5C280800,  //  0025  MOVE	R10	R4
      0x7C240200,  //  0026  CALL	R9	1
      0x60280013,  //  0027  GETGBL	R10	G19
      0x7C280000,  //  0028  CALL	R10	0
      0x982A070E,  //  0029  SETIDX	R10	K3	K14
      0x980C120A,  //  002A  SETIDX	R3	R9	R10
      0x70020006,  //  002B  JMP		#0033
      0x60240008,  //  002C  GETGBL	R9	G8
      0x5C280800,  //  002D  MOVE	R10	R4
      0x7C240200,  //  002E  CALL	R9	1
      0x60280013,  //  002F  GETGBL	R10	G19
      0x7C280000,  //  0030  CALL	R10	0
      0x982A070F,  //  0031  SETIDX	R10	K3	K15
      0x980C120A,  //  0032  SETIDX	R3	R9	R10
      0x50140200,  //  0033  LDBOOL	R5	1	0
      0x00100905,  //  0034  ADD	R4	R4	K5
      0xB8222000,  //  0035  GETNGBL	R8	K16
      0x8C201111,  //  0036  GETMET	R8	R8	K17
      0x58280012,  //  0037  LDCONST	R10	K18
      0x502C0200,  //  0038  LDBOOL	R11	1	0
      0x7C200600,  //  0039  CALL	R8	3
      0x60240012,  //  003A  GETGBL	R9	G18
      0x7C240000,  //  003B  CALL	R9	0
      0xB82A2000,  //  003C  GETNGBL	R10	K16
      0x8C281513,  //  003D  GETMET	R10	R10	K19
      0x60300008,  //  003E  GETGBL	R12	G8
      0x5C341000,  //  003F  MOVE	R13	R8
      0x7C300200,  //  0040  CALL	R12	1
      0x0032280C,  //  0041  ADD	R12	K20	R12
      0x58340015,  //  0042  LDCONST	R13	K21
      0x7C280600,  //  0043  CALL	R10	3
      0x8C281116,  //  0044  GETMET	R10	R8	K22
      0x58300017,  //  0045  LDCONST	R12	K23
      0x7C280400,  //  0046  CALL	R10	2
      0x782A004D,  //  0047  JMPF	R10	#0096
      0x94201117,  //  0048  GETIDX	R8	R8	K23
      0x5828000B,  //  0049  LDCONST	R10	K11
      0x502C0200,  //  004A  LDBOOL	R11	1	0
      0x782E0049,  //  004B  JMPF	R11	#0096
      0x602C0008,  //  004C  GETGBL	R11	G8
      0x5C301400,  //  004D  MOVE	R12	R10
      0x7C2C0200,  //  004E  CALL	R11	1
      0x002E300B,  //  004F  ADD	R11	K24	R11
      0x8C301116,  //  0050  GETMET	R12	R8	K22
      0x5C381600,  //  0051  MOVE	R14	R11
      0x7C300400,  //  0052  CALL	R12	2
      0x74320000,  //  0053  JMPT	R12	#0055
      0x70020040,  //  0054  JMP		#0096
      0x9430100B,  //  0055  GETIDX	R12	R8	R11
      0xB8362000,  //  0056  GETNGBL	R13	K16
      0x8C341B13,  //  0057  GETMET	R13	R13	K19
      0x8C3C0319,  //  0058  GETMET	R15	R1	K25
      0x5844001A,  //  0059  LDCONST	R17	K26
      0x5C481600,  //  005A  MOVE	R18	R11
      0x604C0008,  //  005B  GETGBL	R19	G8
      0x5C501800,  //  005C  MOVE	R20	R12
      0x7C4C0200,  //  005D  CALL	R19	1
      0x7C3C0800,  //  005E  CALL	R15	4
      0x58400015,  //  005F  LDCONST	R16	K21
      0x7C340600,  //  0060  CALL	R13	3
      0x8C341908,  //  0061  GETMET	R13	R12	K8
      0x583C001B,  //  0062  LDCONST	R15	K27
      0x5840000B,  //  0063  LDCONST	R16	K11
      0x7C340600,  //  0064  CALL	R13	3
      0x04341B05,  //  0065  SUB	R13	R13	K5
      0x8C381908,  //  0066  GETMET	R14	R12	K8
      0x5840001C,  //  0067  LDCONST	R16	K28
      0x5844000B,  //  0068  LDCONST	R17	K11
      0x7C380600,  //  0069  CALL	R14	3
      0x04381D05,  //  006A  SUB	R14	R14	K5
      0x283C1B0B,  //  006B  GE	R15	R13	K11
      0x783E0002,  //  006C  JMPF	R15	#0070
      0x8C3C131D,  //  006D  GETMET	R15	R9	K29
      0x5C441A00,  //  006E  MOVE	R17	R13
      0x7C3C0400,  //  006F  CALL	R15	2
      0x283C1D0B,  //  0070  GE	R15	R14	K11
      0x783E0002,  //  0071  JMPF	R15	#0075
      0x8C3C131D,  //  0072  GETMET	R15	R9	K29
      0x5C441C00,  //  0073  MOVE	R17	R14
      0x7C3C0400,  //  0074  CALL	R15	2
      0xB83E2000,  //  0075  GETNGBL	R15	K16
      0x8C3C1F13,  //  0076  GETMET	R15	R15	K19
      0x8C440319,  //  0077  GETMET	R17	R1	K25
      0x584C001E,  //  0078  LDCONST	R19	K30
      0x5C501A00,  //  0079  MOVE	R20	R13
      0x5C541C00,  //  007A  MOVE	R21	R14
      0x7C440800,  //  007B  CALL	R17	4
      0x58480015,  //  007C  LDCONST	R18	K21
      0x7C3C0600,  //  007D  CALL	R15	3
      0x8C3C1908,  //  007E  GETMET	R15	R12	K8
      0x5844001F,  //  007F  LDCONST	R17	K31
      0x7C3C0400,  //  0080  CALL	R15	2
      0x783E0002,  //  0081  JMPF	R15	#0085
      0x94401F0D,  //  0082  GETIDX	R16	R15	K13
      0x2440210B,  //  0083  GT	R16	R16	K11
      0x74420000,  //  0084  JMPT	R16	#0086
      0x50400001,  //  0085  LDBOOL	R16	0	1
      0x50400200,  //  0086  LDBOOL	R16	1	0
      0x60440008,  //  0087  GETGBL	R17	G8
      0x5C480800,  //  0088  MOVE	R18	R4
      0x7C440200,  //  0089  CALL	R17	1
      0x60480013,  //  008A  GETGBL	R18	G19
      0x7C480000,  //  008B  CALL	R18	0
      0x78420001,  //  008C  JMPF	R16	#008F
      0x584C0020,  //  008D  LDCONST	R19	K32
      0x70020000,  //  008E  JMP		#0090
      0x584C0021,  //  008F  LDCONST	R19	K33
      0x984A0613,  //  0090  SETIDX	R18	K3	R19
      0x984A420A,  //  0091  SETIDX	R18	K33	R10
      0x980C2212,  //  0092  SETIDX	R3	R17	R18
      0x00100905,  //  0093  ADD	R4	R4	K5
      0x00281505,  //  0094  ADD	R10	R10	K5
      0x7001FFB3,  //  0095  JMP		#004A
      0x6028000C,  //  0096  GETGBL	R10	G12
      0xB82E2000,  //  0097  GETNGBL	R11	K16
      0x8C2C1722,  //  0098  GETMET	R11	R11	K34
      0x7C2C0200,  //  0099  CALL	R11	1
      0x7C280200,  //  009A  CALL	R10	1
      0x582C000B,  //  009B  LDCONST	R11	K11
      0x78160000,  //  009C  JMPF	R5	#009E
      0x04281505,  //  009D  SUB	R10	R10	K5
      0x1430160A,  //  009E  LT	R12	R11	R10
      0x78320010,  //  009F  JMPF	R12	#00B1
      0x8C301308,  //  00A0  GETMET	R12	R9	K8
      0x5C381600,  //  00A1  MOVE	R14	R11
      0x7C300400,  //  00A2  CALL	R12	2
      0x4C340000,  //  00A3  LDNIL	R13
      0x1C30180D,  //  00A4  EQ	R12	R12	R13
      0x78320008,  //  00A5  JMPF	R12	#00AF
      0x60300008,  //  00A6  GETGBL	R12	G8
      0x5C340800,  //  00A7  MOVE	R13	R4
      0x7C300200,  //  00A8  CALL	R12	1
      0x60340013,  //  00A9  GETGBL	R13	G19
      0x7C340000,  //  00AA  CALL	R13	0
      0x98360723,  //  00AB  SETIDX	R13	K3	K35
      0x9836460B,  //  00AC  SETIDX	R13	K35	R11
      0x980C180D,  //  00AD  SETIDX	R3	R12	R13
      0x00100905,  //  00AE  ADD	R4	R4	K5
      0x002C1705,  //  00AF  ADD	R11	R11	K5
      0x7001FFEC,  //  00B0  JMP		#009E
      0x8C300524,  //  00B1  GETMET	R12	R2	K36
      0xB83A2000,  //  00B2  GETNGBL	R14	K16
      0x8C381D25,  //  00B3  GETMET	R14	R14	K37
      0x7C380200,  //  00B4  CALL	R14	1
      0x7C300400,  //  00B5  CALL	R12	2
      0x5412001F,  //  00B6  LDINT	R4	32
      0x60340010,  //  00B7  GETGBL	R13	G16
      0x8C380126,  //  00B8  GETMET	R14	R0	K38
      0x5C401800,  //  00B9  MOVE	R16	R12
      0x7C380400,  //  00BA  CALL	R14	2
      0x7C340200,  //  00BB  CALL	R13	1
      0xA802001C,  //  00BC  EXBLK	0	#00DA
      0x5C381A00,  //  00BD  MOVE	R14	R13
      0x7C380000,  //  00BE  CALL	R14	0
      0x943C180E,  //  00BF  GETIDX	R15	R12	R14
      0x6040000F,  //  00C0  GETGBL	R16	G15
      0x5C441E00,  //  00C1  MOVE	R17	R15
      0x60480013,  //  00C2  GETGBL	R18	G19
      0x7C400400,  //  00C3  CALL	R16	2
      0x7842000D,  //  00C4  JMPF	R16	#00D3
      0x8C401F16,  //  00C5  GETMET	R16	R15	K22
      0x58480027,  //  00C6  LDCONST	R18	K39
      0x7C400400,  //  00C7  CALL	R16	2
      0x78420009,  //  00C8  JMPF	R16	#00D3
      0x00401D28,  //  00C9  ADD	R16	R14	K40
      0x60440008,  //  00CA  GETGBL	R17	G8
      0x5C480800,  //  00CB  MOVE	R18	R4
      0x7C440200,  //  00CC  CALL	R17	1
      0x60480013,  //  00CD  GETGBL	R18	G19
      0x7C480000,  //  00CE  CALL	R18	0
      0x984A0729,  //  00CF  SETIDX	R18	K3	K41
      0x984A5410,  //  00D0  SETIDX	R18	K42	R16
      0x980C2212,  //  00D1  SETIDX	R3	R17	R18
      0x00100905,  //  00D2  ADD	R4	R4	K5
      0x54420027,  //  00D3  LDINT	R16	40
      0x24400810,  //  00D4  GT	R16	R4	R16
      0x78420000,  //  00D5  JMPF	R16	#00D7
      0x70020000,  //  00D6  JMP		#00D8
      0x7001FFE4,  //  00D7  JMP		#00BD
      0xA8040001,  //  00D8  EXBLK	1	1
      0x70020002,  //  00D9  JMP		#00DD
      0x5834002B,  //  00DA  LDCONST	R13	K43
      0xAC340200,  //  00DB  CATCH	R13	1	0
      0xB0080000,  //  00DC  RAISE	2	R0	R0
      0x54120027,  //  00DD  LDINT	R4	40
      0x60340010,  //  00DE  GETGBL	R13	G16
      0x8C380126,  //  00DF  GETMET	R14	R0	K38
      0x5C401800,  //  00E0  MOVE	R16	R12
      0x7C380400,  //  00E1  CALL	R14	2
      0x7C340200,  //  00E2  CALL	R13	1
      0xA802001C,  //  00E3  EXBLK	0	#0101
      0x5C381A00,  //  00E4  MOVE	R14	R13
      0x7C380000,  //  00E5  CALL	R14	0
      0x943C180E,  //  00E6  GETIDX	R15	R12	R14
      0x6040000F,  //  00E7  GETGBL	R16	G15
      0x5C441E00,  //  00E8  MOVE	R17	R15
      0x60480013,  //  00E9  GETGBL	R18	G19
      0x7C400400,  //  00EA  CALL	R16	2
      0x7842000D,  //  00EB  JMPF	R16	#00FA
      0x8C401F16,  //  00EC  GETMET	R16	R15	K22
      0x5848002C,  //  00ED  LDCONST	R18	K44
      0x7C400400,  //  00EE  CALL	R16	2
      0x78420009,  //  00EF  JMPF	R16	#00FA
      0x00401D2D,  //  00F0  ADD	R16	R14	K45
      0x60440008,  //  00F1  GETGBL	R17	G8
      0x5C480800,  //  00F2  MOVE	R18	R4
      0x7C440200,  //  00F3  CALL	R17	1
      0x60480013,  //  00F4  GETGBL	R18	G19
      0x7C480000,  //  00F5  CALL	R18	0
      0x984A072E,  //  00F6  SETIDX	R18	K3	K46
      0x984A5410,  //  00F7  SETIDX	R18	K42	R16
      0x980C2212,  //  00F8  SETIDX	R3	R17	R18
      0x00100905,  //  00F9  ADD	R4	R4	K5
      0x5442002E,  //  00FA  LDINT	R16	47
      0x24400810,  //  00FB  GT	R16	R4	R16
      0x78420000,  //  00FC  JMPF	R16	#00FE
      0x70020000,  //  00FD  JMP		#00FF
      0x7001FFE4,  //  00FE  JMP		#00E4
      0xA8040001,  //  00FF  EXBLK	1	1
      0x70020002,  //  0100  JMP		#0104
      0x5834002B,  //  0101  LDCONST	R13	K43
      0xAC340200,  //  0102  CATCH	R13	1	0
      0xB0080000,  //  0103  RAISE	2	R0	R0
      0x5412002F,  //  0104  LDINT	R4	48
      0x60340010,  //  0105  GETGBL	R13	G16
      0x8C380126,  //  0106  GETMET	R14	R0	K38
      0x5C401800,  //  0107  MOVE	R16	R12
      0x7C380400,  //  0108  CALL	R14	2
      0x7C340200,  //  0109  CALL	R13	1
      0xA802001C,  //  010A  EXBLK	0	#0128
      0x5C381A00,  //  010B  MOVE	R14	R13
      0x7C380000,  //  010C  CALL	R14	0
      0x943C180E,  //  010D  GETIDX	R15	R12	R14
      0x6040000F,  //  010E  GETGBL	R16	G15
      0x5C441E00,  //  010F  MOVE	R17	R15
      0x60480013,  //  0110  GETGBL	R18	G19
      0x7C400400,  //  0111  CALL	R16	2
      0x7842000D,  //  0112  JMPF	R16	#0121
      0x8C401F16,  //  0113  GETMET	R16	R15	K22
      0x5848002F,  //  0114  LDCONST	R18	K47
      0x7C400400,  //  0115  CALL	R16	2
      0x78420009,  //  0116  JMPF	R16	#0121
      0x00401D30,  //  0117  ADD	R16	R14	K48
      0x60440008,  //  0118  GETGBL	R17	G8
      0x5C480800,  //  0119  MOVE	R18	R4
      0x7C440200,  //  011A  CALL	R17	1
      0x60480013,  //  011B  GETGBL	R18	G19
      0x7C480000,  //  011C  CALL	R18	0
      0x984A0731,  //  011D  SETIDX	R18	K3	K49
      0x984A5410,  //  011E  SETIDX	R18	K42	R16
      0x980C2212,  //  011F  SETIDX	R3	R17	R18
      0x00100905,  //  0120  ADD	R4	R4	K5
      0x54420037,  //  0121  LDINT	R16	56
      0x24400810,  //  0122  GT	R16	R4	R16
      0x78420000,  //  0123  JMPF	R16	#0125
      0x70020000,  //  0124  JMP		#0126
      0x7001FFE4,  //  0125  JMP		#010B
      0xA8040001,  //  0126  EXBLK	1	1
      0x70020002,  //  0127  JMP		#012B
      0x5834002B,  //  0128  LDCONST	R13	K43
      0xAC340200,  //  0129  CATCH	R13	1	0
      0xB0080000,  //  012A  RAISE	2	R0	R0
      0x54120037,  //  012B  LDINT	R4	56
      0x60340010,  //  012C  GETGBL	R13	G16
      0x8C380126,  //  012D  GETMET	R14	R0	K38
      0x5C401800,  //  012E  MOVE	R16	R12
      0x7C380400,  //  012F  CALL	R14	2
      0x7C340200,  //  0130  CALL	R13	1
      0xA802001C,  //  0131  EXBLK	0	#014F
      0x5C381A00,  //  0132  MOVE	R14	R13
      0x7C380000,  //  0133  CALL	R14	0
      0x943C180E,  //  0134  GETIDX	R15	R12	R14
      0x6040000F,  //  0135  GETGBL	R16	G15
      0x5C441E00,  //  0136  MOVE	R17	R15
      0x60480013,  //  0137  GETGBL	R18	G19
      0x7C400400,  //  0138  CALL	R16	2
      0x7842000D,  //  0139  JMPF	R16	#0148
      0x8C401F16,  //  013A  GETMET	R16	R15	K22
      0x58480032,  //  013B  LDCONST	R18	K50
      0x7C400400,  //  013C  CALL	R16	2
      0x78420009,  //  013D  JMPF	R16	#0148
      0x00401D33,  //  013E  ADD	R16	R14	K51
      0x60440008,  //  013F  GETGBL	R17	G8
      0x5C480800,  //  0140  MOVE	R18	R4
      0x7C440200,  //  0141  CALL	R17	1
      0x60480013,  //  0142  GETGBL	R18	G19
      0x7C480000,  //  0143  CALL	R18	0
      0x984A0734,  //  0144  SETIDX	R18	K3	K52
      0x984A5410,  //  0145  SETIDX	R18	K42	R16
      0x980C2212,  //  0146  SETIDX	R3	R17	R18
      0x00100905,  //  0147  ADD	R4	R4	K5
      0x5442003F,  //  0148  LDINT	R16	64
      0x24400810,  //  0149  GT	R16	R4	R16
      0x78420000,  //  014A  JMPF	R16	#014C
      0x70020000,  //  014B  JMP		#014D
      0x7001FFE4,  //  014C  JMP		#0132
      0xA8040001,  //  014D  EXBLK	1	1
      0x70020002,  //  014E  JMP		#0152
      0x5834002B,  //  014F  LDCONST	R13	K43
      0xAC340200,  //  0150  CATCH	R13	1	0
      0xB0080000,  //  0151  RAISE	2	R0	R0
      0x80040600,  //  0152  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_commissioning_open
********************************************************************/
be_local_closure(Matter_Device_is_commissioning_open,   /* name */
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
    /* K0   */  be_nested_str_weak(commissioning_open),
    }),
    be_str_weak(is_commissioning_open),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Device_init,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        4,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 5]) {     /* constants */
        /* K0   */  be_nested_str_weak(start),
        /* K1   */  be_nested_str_weak(tasmota),
        /* K2   */  be_nested_str_weak(remove_rule),
        /* K3   */  be_nested_str_weak(Wifi_X23Connected),
        /* K4   */  be_nested_str_weak(matter_start),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[ 9]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0xB8020200,  //  0003  GETNGBL	R0	K1
          0x8C000102,  //  0004  GETMET	R0	R0	K2
          0x58080003,  //  0005  LDCONST	R2	K3
          0x580C0004,  //  0006  LDCONST	R3	K4
          0x7C000600,  //  0007  CALL	R0	3
          0x80000000,  //  0008  RET	0
        })
      ),
      be_nested_proto(
        4,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 5]) {     /* constants */
        /* K0   */  be_nested_str_weak(start),
        /* K1   */  be_nested_str_weak(tasmota),
        /* K2   */  be_nested_str_weak(remove_rule),
        /* K3   */  be_nested_str_weak(Eth_X23Connected),
        /* K4   */  be_nested_str_weak(matter_start),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[ 9]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0xB8020200,  //  0003  GETNGBL	R0	K1
          0x8C000102,  //  0004  GETMET	R0	R0	K2
          0x58080003,  //  0005  LDCONST	R2	K3
          0x580C0004,  //  0006  LDCONST	R3	K4
          0x7C000600,  //  0007  CALL	R0	3
          0x80000000,  //  0008  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[41]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(get_option),
    /* K4   */  be_nested_str_weak(matter),
    /* K5   */  be_nested_str_weak(MATTER_OPTION),
    /* K6   */  be_nested_str_weak(UI),
    /* K7   */  be_nested_str_weak(started),
    /* K8   */  be_nested_str_weak(tick),
    /* K9   */  be_const_int(0),
    /* K10  */  be_nested_str_weak(plugins),
    /* K11  */  be_nested_str_weak(plugins_persist),
    /* K12  */  be_nested_str_weak(plugins_classes),
    /* K13  */  be_nested_str_weak(register_native_classes),
    /* K14  */  be_nested_str_weak(vendorid),
    /* K15  */  be_nested_str_weak(VENDOR_ID),
    /* K16  */  be_nested_str_weak(productid),
    /* K17  */  be_nested_str_weak(PRODUCT_ID),
    /* K18  */  be_nested_str_weak(root_iterations),
    /* K19  */  be_nested_str_weak(PBKDF_ITERATIONS),
    /* K20  */  be_nested_str_weak(root_salt),
    /* K21  */  be_nested_str_weak(random),
    /* K22  */  be_nested_str_weak(ipv4only),
    /* K23  */  be_nested_str_weak(load_param),
    /* K24  */  be_nested_str_weak(sessions),
    /* K25  */  be_nested_str_weak(Session_Store),
    /* K26  */  be_nested_str_weak(load_fabrics),
    /* K27  */  be_nested_str_weak(message_handler),
    /* K28  */  be_nested_str_weak(MessageHandler),
    /* K29  */  be_nested_str_weak(ui),
    /* K30  */  be_nested_str_weak(wifi),
    /* K31  */  be_nested_str_weak(up),
    /* K32  */  be_nested_str_weak(eth),
    /* K33  */  be_nested_str_weak(start),
    /* K34  */  be_nested_str_weak(add_rule),
    /* K35  */  be_nested_str_weak(Wifi_X23Connected),
    /* K36  */  be_nested_str_weak(matter_start),
    /* K37  */  be_nested_str_weak(Eth_X23Connected),
    /* K38  */  be_nested_str_weak(_init_basic_commissioning),
    /* K39  */  be_nested_str_weak(add_driver),
    /* K40  */  be_nested_str_weak(register_commands),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[102]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0xB80E0400,  //  0002  GETNGBL	R3	K2
      0x8C0C0703,  //  0003  GETMET	R3	R3	K3
      0xB8160800,  //  0004  GETNGBL	R5	K4
      0x88140B05,  //  0005  GETMBR	R5	R5	K5
      0x7C0C0400,  //  0006  CALL	R3	2
      0x740E0004,  //  0007  JMPT	R3	#000D
      0xB80E0800,  //  0008  GETNGBL	R3	K4
      0x8C0C0706,  //  0009  GETMET	R3	R3	K6
      0x5C140000,  //  000A  MOVE	R5	R0
      0x7C0C0400,  //  000B  CALL	R3	2
      0x80000600,  //  000C  RET	0
      0x500C0000,  //  000D  LDBOOL	R3	0	0
      0x90020E03,  //  000E  SETMBR	R0	K7	R3
      0x90021109,  //  000F  SETMBR	R0	K8	K9
      0x600C0012,  //  0010  GETGBL	R3	G18
      0x7C0C0000,  //  0011  CALL	R3	0
      0x90021403,  //  0012  SETMBR	R0	K10	R3
      0x500C0000,  //  0013  LDBOOL	R3	0	0
      0x90021603,  //  0014  SETMBR	R0	K11	R3
      0x600C0013,  //  0015  GETGBL	R3	G19
      0x7C0C0000,  //  0016  CALL	R3	0
      0x90021803,  //  0017  SETMBR	R0	K12	R3
      0x8C0C010D,  //  0018  GETMET	R3	R0	K13
      0x7C0C0200,  //  0019  CALL	R3	1
      0x880C010F,  //  001A  GETMBR	R3	R0	K15
      0x90021C03,  //  001B  SETMBR	R0	K14	R3
      0x880C0111,  //  001C  GETMBR	R3	R0	K17
      0x90022003,  //  001D  SETMBR	R0	K16	R3
      0x880C0113,  //  001E  GETMBR	R3	R0	K19
      0x90022403,  //  001F  SETMBR	R0	K18	R3
      0x8C0C0315,  //  0020  GETMET	R3	R1	K21
      0x5416000F,  //  0021  LDINT	R5	16
      0x7C0C0400,  //  0022  CALL	R3	2
      0x90022803,  //  0023  SETMBR	R0	K20	R3
      0x500C0000,  //  0024  LDBOOL	R3	0	0
      0x90022C03,  //  0025  SETMBR	R0	K22	R3
      0x8C0C0117,  //  0026  GETMET	R3	R0	K23
      0x7C0C0200,  //  0027  CALL	R3	1
      0xB80E0800,  //  0028  GETNGBL	R3	K4
      0x8C0C0719,  //  0029  GETMET	R3	R3	K25
      0x5C140000,  //  002A  MOVE	R5	R0
      0x7C0C0400,  //  002B  CALL	R3	2
      0x90023003,  //  002C  SETMBR	R0	K24	R3
      0x880C0118,  //  002D  GETMBR	R3	R0	K24
      0x8C0C071A,  //  002E  GETMET	R3	R3	K26
      0x7C0C0200,  //  002F  CALL	R3	1
      0xB80E0800,  //  0030  GETNGBL	R3	K4
      0x8C0C071C,  //  0031  GETMET	R3	R3	K28
      0x5C140000,  //  0032  MOVE	R5	R0
      0x7C0C0400,  //  0033  CALL	R3	2
      0x90023603,  //  0034  SETMBR	R0	K27	R3
      0xB80E0800,  //  0035  GETNGBL	R3	K4
      0x8C0C0706,  //  0036  GETMET	R3	R3	K6
      0x5C140000,  //  0037  MOVE	R5	R0
      0x7C0C0400,  //  0038  CALL	R3	2
      0x90023A03,  //  0039  SETMBR	R0	K29	R3
      0xB80E0400,  //  003A  GETNGBL	R3	K2
      0x8C0C071E,  //  003B  GETMET	R3	R3	K30
      0x7C0C0200,  //  003C  CALL	R3	1
      0x940C071F,  //  003D  GETIDX	R3	R3	K31
      0x740E0004,  //  003E  JMPT	R3	#0044
      0xB80E0400,  //  003F  GETNGBL	R3	K2
      0x8C0C0720,  //  0040  GETMET	R3	R3	K32
      0x7C0C0200,  //  0041  CALL	R3	1
      0x940C071F,  //  0042  GETIDX	R3	R3	K31
      0x780E0001,  //  0043  JMPF	R3	#0046
      0x8C0C0121,  //  0044  GETMET	R3	R0	K33
      0x7C0C0200,  //  0045  CALL	R3	1
      0xB80E0400,  //  0046  GETNGBL	R3	K2
      0x8C0C071E,  //  0047  GETMET	R3	R3	K30
      0x7C0C0200,  //  0048  CALL	R3	1
      0x940C071F,  //  0049  GETIDX	R3	R3	K31
      0x740E0005,  //  004A  JMPT	R3	#0051
      0xB80E0400,  //  004B  GETNGBL	R3	K2
      0x8C0C0722,  //  004C  GETMET	R3	R3	K34
      0x58140023,  //  004D  LDCONST	R5	K35
      0x84180000,  //  004E  CLOSURE	R6	P0
      0x581C0024,  //  004F  LDCONST	R7	K36
      0x7C0C0800,  //  0050  CALL	R3	4
      0xB80E0400,  //  0051  GETNGBL	R3	K2
      0x8C0C0720,  //  0052  GETMET	R3	R3	K32
      0x7C0C0200,  //  0053  CALL	R3	1
      0x940C071F,  //  0054  GETIDX	R3	R3	K31
      0x740E0005,  //  0055  JMPT	R3	#005C
      0xB80E0400,  //  0056  GETNGBL	R3	K2
      0x8C0C0722,  //  0057  GETMET	R3	R3	K34
      0x58140025,  //  0058  LDCONST	R5	K37
      0x84180001,  //  0059  CLOSURE	R6	P1
      0x581C0024,  //  005A  LDCONST	R7	K36
      0x7C0C0800,  //  005B  CALL	R3	4
      0x8C0C0126,  //  005C  GETMET	R3	R0	K38
      0x7C0C0200,  //  005D  CALL	R3	1
      0xB80E0400,  //  005E  GETNGBL	R3	K2
      0x8C0C0727,  //  005F  GETMET	R3	R3	K39
      0x5C140000,  //  0060  MOVE	R5	R0
      0x7C0C0400,  //  0061  CALL	R3	2
      0x8C0C0128,  //  0062  GETMET	R3	R0	K40
      0x7C0C0200,  //  0063  CALL	R3	1
      0xA0000000,  //  0064  CLOSE	R0
      0x80000000,  //  0065  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: mdns_remove_op_discovery
********************************************************************/
be_local_closure(Matter_Device_mdns_remove_op_discovery,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[24]) {     /* constants */
    /* K0   */  be_nested_str_weak(mdns),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(get_device_id),
    /* K3   */  be_nested_str_weak(copy),
    /* K4   */  be_nested_str_weak(reverse),
    /* K5   */  be_nested_str_weak(get_fabric_compressed),
    /* K6   */  be_nested_str_weak(tohex),
    /* K7   */  be_nested_str_weak(_X2D),
    /* K8   */  be_nested_str_weak(tasmota),
    /* K9   */  be_nested_str_weak(eth),
    /* K10  */  be_nested_str_weak(find),
    /* K11  */  be_nested_str_weak(up),
    /* K12  */  be_nested_str_weak(log),
    /* K13  */  be_nested_str_weak(format),
    /* K14  */  be_nested_str_weak(MTR_X3A_X20remove_X20mDNS_X20on_X20_X25s_X20_X27_X25s_X27),
    /* K15  */  be_const_int(2),
    /* K16  */  be_nested_str_weak(remove_service),
    /* K17  */  be_nested_str_weak(_matter),
    /* K18  */  be_nested_str_weak(_tcp),
    /* K19  */  be_nested_str_weak(hostname_eth),
    /* K20  */  be_nested_str_weak(wifi),
    /* K21  */  be_nested_str_weak(hostname_wifi),
    /* K22  */  be_nested_str_weak(MTR_X3A_X20Exception),
    /* K23  */  be_nested_str_weak(_X7C),
    }),
    be_str_weak(mdns_remove_op_discovery),
    &be_const_str_solidified,
    ( &(const binstruction[81]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0xA802003B,  //  0002  EXBLK	0	#003F
      0x8C100302,  //  0003  GETMET	R4	R1	K2
      0x7C100200,  //  0004  CALL	R4	1
      0x8C100903,  //  0005  GETMET	R4	R4	K3
      0x7C100200,  //  0006  CALL	R4	1
      0x8C100904,  //  0007  GETMET	R4	R4	K4
      0x7C100200,  //  0008  CALL	R4	1
      0x8C140305,  //  0009  GETMET	R5	R1	K5
      0x7C140200,  //  000A  CALL	R5	1
      0x8C180B06,  //  000B  GETMET	R6	R5	K6
      0x7C180200,  //  000C  CALL	R6	1
      0x00180D07,  //  000D  ADD	R6	R6	K7
      0x8C1C0906,  //  000E  GETMET	R7	R4	K6
      0x7C1C0200,  //  000F  CALL	R7	1
      0x00180C07,  //  0010  ADD	R6	R6	R7
      0xB81E1000,  //  0011  GETNGBL	R7	K8
      0x8C1C0F09,  //  0012  GETMET	R7	R7	K9
      0x7C1C0200,  //  0013  CALL	R7	1
      0x8C1C0F0A,  //  0014  GETMET	R7	R7	K10
      0x5824000B,  //  0015  LDCONST	R9	K11
      0x7C1C0400,  //  0016  CALL	R7	2
      0x781E000E,  //  0017  JMPF	R7	#0027
      0xB81E1000,  //  0018  GETNGBL	R7	K8
      0x8C1C0F0C,  //  0019  GETMET	R7	R7	K12
      0x8C24070D,  //  001A  GETMET	R9	R3	K13
      0x582C000E,  //  001B  LDCONST	R11	K14
      0x58300009,  //  001C  LDCONST	R12	K9
      0x5C340C00,  //  001D  MOVE	R13	R6
      0x7C240800,  //  001E  CALL	R9	4
      0x5828000F,  //  001F  LDCONST	R10	K15
      0x7C1C0600,  //  0020  CALL	R7	3
      0x8C1C0510,  //  0021  GETMET	R7	R2	K16
      0x58240011,  //  0022  LDCONST	R9	K17
      0x58280012,  //  0023  LDCONST	R10	K18
      0x5C2C0C00,  //  0024  MOVE	R11	R6
      0x88300113,  //  0025  GETMBR	R12	R0	K19
      0x7C1C0A00,  //  0026  CALL	R7	5
      0xB81E1000,  //  0027  GETNGBL	R7	K8
      0x8C1C0F14,  //  0028  GETMET	R7	R7	K20
      0x7C1C0200,  //  0029  CALL	R7	1
      0x8C1C0F0A,  //  002A  GETMET	R7	R7	K10
      0x5824000B,  //  002B  LDCONST	R9	K11
      0x7C1C0400,  //  002C  CALL	R7	2
      0x781E000E,  //  002D  JMPF	R7	#003D
      0xB81E1000,  //  002E  GETNGBL	R7	K8
      0x8C1C0F0C,  //  002F  GETMET	R7	R7	K12
      0x8C24070D,  //  0030  GETMET	R9	R3	K13
      0x582C000E,  //  0031  LDCONST	R11	K14
      0x58300014,  //  0032  LDCONST	R12	K20
      0x5C340C00,  //  0033  MOVE	R13	R6
      0x7C240800,  //  0034  CALL	R9	4
      0x5828000F,  //  0035  LDCONST	R10	K15
      0x7C1C0600,  //  0036  CALL	R7	3
      0x8C1C0510,  //  0037  GETMET	R7	R2	K16
      0x58240011,  //  0038  LDCONST	R9	K17
      0x58280012,  //  0039  LDCONST	R10	K18
      0x5C2C0C00,  //  003A  MOVE	R11	R6
      0x88300115,  //  003B  GETMBR	R12	R0	K21
      0x7C1C0A00,  //  003C  CALL	R7	5
      0xA8040001,  //  003D  EXBLK	1	1
      0x70020010,  //  003E  JMP		#0050
      0xAC100002,  //  003F  CATCH	R4	0	2
      0x7002000D,  //  0040  JMP		#004F
      0xB81A1000,  //  0041  GETNGBL	R6	K8
      0x8C180D0C,  //  0042  GETMET	R6	R6	K12
      0x60200008,  //  0043  GETGBL	R8	G8
      0x5C240800,  //  0044  MOVE	R9	R4
      0x7C200200,  //  0045  CALL	R8	1
      0x00222C08,  //  0046  ADD	R8	K22	R8
      0x00201117,  //  0047  ADD	R8	R8	K23
      0x60240008,  //  0048  GETGBL	R9	G8
      0x5C280A00,  //  0049  MOVE	R10	R5
      0x7C240200,  //  004A  CALL	R9	1
      0x00201009,  //  004B  ADD	R8	R8	R9
      0x5824000F,  //  004C  LDCONST	R9	K15
      0x7C180600,  //  004D  CALL	R6	3
      0x70020000,  //  004E  JMP		#0050
      0xB0080000,  //  004F  RAISE	2	R0	R0
      0x80000000,  //  0050  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _mdns_announce_hostname
********************************************************************/
be_local_closure(Matter_Device__mdns_announce_hostname,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[28]) {     /* constants */
    /* K0   */  be_nested_str_weak(mdns),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(start),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(eth),
    /* K5   */  be_nested_str_weak(hostname_eth),
    /* K6   */  be_nested_str_weak(replace),
    /* K7   */  be_nested_str_weak(find),
    /* K8   */  be_nested_str_weak(mac),
    /* K9   */  be_nested_str_weak(_X3A),
    /* K10  */  be_nested_str_weak(),
    /* K11  */  be_nested_str_weak(ipv4only),
    /* K12  */  be_nested_str_weak(log),
    /* K13  */  be_nested_str_weak(format),
    /* K14  */  be_nested_str_weak(MTR_X3A_X20calling_X20mdns_X2Eadd_hostname_X28_X25s_X2C_X20_X25s_X2C_X20_X25s_X29),
    /* K15  */  be_nested_str_weak(ip6local),
    /* K16  */  be_nested_str_weak(ip),
    /* K17  */  be_const_int(3),
    /* K18  */  be_nested_str_weak(add_hostname),
    /* K19  */  be_nested_str_weak(ip6),
    /* K20  */  be_nested_str_weak(MTR_X3A_X20calling_X20mdns_X2Eadd_hostname_X28_X25s_X2C_X20_X25s_X29),
    /* K21  */  be_nested_str_weak(wifi),
    /* K22  */  be_nested_str_weak(hostname_wifi),
    /* K23  */  be_nested_str_weak(MTR_X3A_X20start_X20mDNS_X20on_X20_X25s_X20host_X20_X27_X25s_X2Elocal_X27),
    /* K24  */  be_const_int(2),
    /* K25  */  be_nested_str_weak(MTR_X3A_X20Exception),
    /* K26  */  be_nested_str_weak(_X7C),
    /* K27  */  be_nested_str_weak(mdns_announce_op_discovery_all_fabrics),
    }),
    be_str_weak(_mdns_announce_hostname),
    &be_const_str_solidified,
    ( &(const binstruction[172]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0x8C100502,  //  0002  GETMET	R4	R2	K2
      0x7C100200,  //  0003  CALL	R4	1
      0xA8020092,  //  0004  EXBLK	0	#0098
      0x78060040,  //  0005  JMPF	R1	#0047
      0xB8120600,  //  0006  GETNGBL	R4	K3
      0x8C100904,  //  0007  GETMET	R4	R4	K4
      0x7C100200,  //  0008  CALL	R4	1
      0x8C140706,  //  0009  GETMET	R5	R3	K6
      0x8C1C0907,  //  000A  GETMET	R7	R4	K7
      0x58240008,  //  000B  LDCONST	R9	K8
      0x7C1C0400,  //  000C  CALL	R7	2
      0x58200009,  //  000D  LDCONST	R8	K9
      0x5824000A,  //  000E  LDCONST	R9	K10
      0x7C140800,  //  000F  CALL	R5	4
      0x90020A05,  //  0010  SETMBR	R0	K5	R5
      0x8814010B,  //  0011  GETMBR	R5	R0	K11
      0x7416001F,  //  0012  JMPT	R5	#0033
      0xB8160600,  //  0013  GETNGBL	R5	K3
      0x8C140B0C,  //  0014  GETMET	R5	R5	K12
      0x8C1C070D,  //  0015  GETMET	R7	R3	K13
      0x5824000E,  //  0016  LDCONST	R9	K14
      0x88280105,  //  0017  GETMBR	R10	R0	K5
      0x8C2C0907,  //  0018  GETMET	R11	R4	K7
      0x5834000F,  //  0019  LDCONST	R13	K15
      0x5838000A,  //  001A  LDCONST	R14	K10
      0x7C2C0600,  //  001B  CALL	R11	3
      0x8C300907,  //  001C  GETMET	R12	R4	K7
      0x58380010,  //  001D  LDCONST	R14	K16
      0x583C000A,  //  001E  LDCONST	R15	K10
      0x7C300600,  //  001F  CALL	R12	3
      0x7C1C0A00,  //  0020  CALL	R7	5
      0x58200011,  //  0021  LDCONST	R8	K17
      0x7C140600,  //  0022  CALL	R5	3
      0x8C140512,  //  0023  GETMET	R5	R2	K18
      0x881C0105,  //  0024  GETMBR	R7	R0	K5
      0x8C200907,  //  0025  GETMET	R8	R4	K7
      0x5828000F,  //  0026  LDCONST	R10	K15
      0x582C000A,  //  0027  LDCONST	R11	K10
      0x7C200600,  //  0028  CALL	R8	3
      0x8C240907,  //  0029  GETMET	R9	R4	K7
      0x582C0010,  //  002A  LDCONST	R11	K16
      0x5830000A,  //  002B  LDCONST	R12	K10
      0x7C240600,  //  002C  CALL	R9	3
      0x8C280907,  //  002D  GETMET	R10	R4	K7
      0x58300013,  //  002E  LDCONST	R12	K19
      0x5834000A,  //  002F  LDCONST	R13	K10
      0x7C280600,  //  0030  CALL	R10	3
      0x7C140A00,  //  0031  CALL	R5	5
      0x70020012,  //  0032  JMP		#0046
      0xB8160600,  //  0033  GETNGBL	R5	K3
      0x8C140B0C,  //  0034  GETMET	R5	R5	K12
      0x8C1C070D,  //  0035  GETMET	R7	R3	K13
      0x58240014,  //  0036  LDCONST	R9	K20
      0x88280105,  //  0037  GETMBR	R10	R0	K5
      0x8C2C0907,  //  0038  GETMET	R11	R4	K7
      0x58340010,  //  0039  LDCONST	R13	K16
      0x5838000A,  //  003A  LDCONST	R14	K10
      0x7C2C0600,  //  003B  CALL	R11	3
      0x7C1C0800,  //  003C  CALL	R7	4
      0x58200011,  //  003D  LDCONST	R8	K17
      0x7C140600,  //  003E  CALL	R5	3
      0x8C140512,  //  003F  GETMET	R5	R2	K18
      0x881C0105,  //  0040  GETMBR	R7	R0	K5
      0x8C200907,  //  0041  GETMET	R8	R4	K7
      0x58280010,  //  0042  LDCONST	R10	K16
      0x582C000A,  //  0043  LDCONST	R11	K10
      0x7C200600,  //  0044  CALL	R8	3
      0x7C140600,  //  0045  CALL	R5	3
      0x7002003F,  //  0046  JMP		#0087
      0xB8120600,  //  0047  GETNGBL	R4	K3
      0x8C100915,  //  0048  GETMET	R4	R4	K21
      0x7C100200,  //  0049  CALL	R4	1
      0x8C140706,  //  004A  GETMET	R5	R3	K6
      0x8C1C0907,  //  004B  GETMET	R7	R4	K7
      0x58240008,  //  004C  LDCONST	R9	K8
      0x7C1C0400,  //  004D  CALL	R7	2
      0x58200009,  //  004E  LDCONST	R8	K9
      0x5824000A,  //  004F  LDCONST	R9	K10
      0x7C140800,  //  0050  CALL	R5	4
      0x90022C05,  //  0051  SETMBR	R0	K22	R5
      0x8814010B,  //  0052  GETMBR	R5	R0	K11
      0x7416001F,  //  0053  JMPT	R5	#0074
      0xB8160600,  //  0054  GETNGBL	R5	K3
      0x8C140B0C,  //  0055  GETMET	R5	R5	K12
      0x8C1C070D,  //  0056  GETMET	R7	R3	K13
      0x5824000E,  //  0057  LDCONST	R9	K14
      0x88280116,  //  0058  GETMBR	R10	R0	K22
      0x8C2C0907,  //  0059  GETMET	R11	R4	K7
      0x5834000F,  //  005A  LDCONST	R13	K15
      0x5838000A,  //  005B  LDCONST	R14	K10
      0x7C2C0600,  //  005C  CALL	R11	3
      0x8C300907,  //  005D  GETMET	R12	R4	K7
      0x58380010,  //  005E  LDCONST	R14	K16
      0x583C000A,  //  005F  LDCONST	R15	K10
      0x7C300600,  //  0060  CALL	R12	3
      0x7C1C0A00,  //  0061  CALL	R7	5
      0x58200011,  //  0062  LDCONST	R8	K17
      0x7C140600,  //  0063  CALL	R5	3
      0x8C140512,  //  0064  GETMET	R5	R2	K18
      0x881C0116,  //  0065  GETMBR	R7	R0	K22
      0x8C200907,  //  0066  GETMET	R8	R4	K7
      0x5828000F,  //  0067  LDCONST	R10	K15
      0x582C000A,  //  0068  LDCONST	R11	K10
      0x7C200600,  //  0069  CALL	R8	3
      0x8C240907,  //  006A  GETMET	R9	R4	K7
      0x582C0010,  //  006B  LDCONST	R11	K16
      0x5830000A,  //  006C  LDCONST	R12	K10
      0x7C240600,  //  006D  CALL	R9	3
      0x8C280907,  //  006E  GETMET	R10	R4	K7
      0x58300013,  //  006F  LDCONST	R12	K19
      0x5834000A,  //  0070  LDCONST	R13	K10
      0x7C280600,  //  0071  CALL	R10	3
      0x7C140A00,  //  0072  CALL	R5	5
      0x70020012,  //  0073  JMP		#0087
      0xB8160600,  //  0074  GETNGBL	R5	K3
      0x8C140B0C,  //  0075  GETMET	R5	R5	K12
      0x8C1C070D,  //  0076  GETMET	R7	R3	K13
      0x58240014,  //  0077  LDCONST	R9	K20
      0x88280105,  //  0078  GETMBR	R10	R0	K5
      0x8C2C0907,  //  0079  GETMET	R11	R4	K7
      0x58340010,  //  007A  LDCONST	R13	K16
      0x5838000A,  //  007B  LDCONST	R14	K10
      0x7C2C0600,  //  007C  CALL	R11	3
      0x7C1C0800,  //  007D  CALL	R7	4
      0x58200011,  //  007E  LDCONST	R8	K17
      0x7C140600,  //  007F  CALL	R5	3
      0x8C140512,  //  0080  GETMET	R5	R2	K18
      0x881C0116,  //  0081  GETMBR	R7	R0	K22
      0x8C200907,  //  0082  GETMET	R8	R4	K7
      0x58280010,  //  0083  LDCONST	R10	K16
      0x582C000A,  //  0084  LDCONST	R11	K10
      0x7C200600,  //  0085  CALL	R8	3
      0x7C140600,  //  0086  CALL	R5	3
      0xB8120600,  //  0087  GETNGBL	R4	K3
      0x8C10090C,  //  0088  GETMET	R4	R4	K12
      0x8C18070D,  //  0089  GETMET	R6	R3	K13
      0x58200017,  //  008A  LDCONST	R8	K23
      0x78060001,  //  008B  JMPF	R1	#008E
      0x58240004,  //  008C  LDCONST	R9	K4
      0x70020000,  //  008D  JMP		#008F
      0x58240015,  //  008E  LDCONST	R9	K21
      0x78060001,  //  008F  JMPF	R1	#0092
      0x88280105,  //  0090  GETMBR	R10	R0	K5
      0x70020000,  //  0091  JMP		#0093
      0x88280116,  //  0092  GETMBR	R10	R0	K22
      0x7C180800,  //  0093  CALL	R6	4
      0x581C0018,  //  0094  LDCONST	R7	K24
      0x7C100600,  //  0095  CALL	R4	3
      0xA8040001,  //  0096  EXBLK	1	1
      0x70020010,  //  0097  JMP		#00A9
      0xAC100002,  //  0098  CATCH	R4	0	2
      0x7002000D,  //  0099  JMP		#00A8
      0xB81A0600,  //  009A  GETNGBL	R6	K3
      0x8C180D0C,  //  009B  GETMET	R6	R6	K12
      0x60200008,  //  009C  GETGBL	R8	G8
      0x5C240800,  //  009D  MOVE	R9	R4
      0x7C200200,  //  009E  CALL	R8	1
      0x00223208,  //  009F  ADD	R8	K25	R8
      0x0020111A,  //  00A0  ADD	R8	R8	K26
      0x60240008,  //  00A1  GETGBL	R9	G8
      0x5C280A00,  //  00A2  MOVE	R10	R5
      0x7C240200,  //  00A3  CALL	R9	1
      0x00201009,  //  00A4  ADD	R8	R8	R9
      0x58240018,  //  00A5  LDCONST	R9	K24
      0x7C180600,  //  00A6  CALL	R6	3
      0x70020000,  //  00A7  JMP		#00A9
      0xB0080000,  //  00A8  RAISE	2	R0	R0
      0x8C10011B,  //  00A9  GETMET	R4	R0	K27
      0x7C100200,  //  00AA  CALL	R4	1
      0x80000000,  //  00AB  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_mdns_announce_hostnames
********************************************************************/
be_local_closure(Matter_Device_start_mdns_announce_hostnames,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        4,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 5]) {     /* constants */
        /* K0   */  be_nested_str_weak(_mdns_announce_hostname),
        /* K1   */  be_nested_str_weak(tasmota),
        /* K2   */  be_nested_str_weak(remove_rule),
        /* K3   */  be_nested_str_weak(Wifi_X23Connected),
        /* K4   */  be_nested_str_weak(matter_mdns_host),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[10]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x50080000,  //  0002  LDBOOL	R2	0	0
          0x7C000400,  //  0003  CALL	R0	2
          0xB8020200,  //  0004  GETNGBL	R0	K1
          0x8C000102,  //  0005  GETMET	R0	R0	K2
          0x58080003,  //  0006  LDCONST	R2	K3
          0x580C0004,  //  0007  LDCONST	R3	K4
          0x7C000600,  //  0008  CALL	R0	3
          0x80000000,  //  0009  RET	0
        })
      ),
      be_nested_proto(
        4,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 5]) {     /* constants */
        /* K0   */  be_nested_str_weak(_mdns_announce_hostname),
        /* K1   */  be_nested_str_weak(tasmota),
        /* K2   */  be_nested_str_weak(remove_rule),
        /* K3   */  be_nested_str_weak(Eth_X23Connected),
        /* K4   */  be_nested_str_weak(matter_mdns_host),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[10]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x50080200,  //  0002  LDBOOL	R2	1	0
          0x7C000400,  //  0003  CALL	R0	2
          0xB8020200,  //  0004  GETNGBL	R0	K1
          0x8C000102,  //  0005  GETMET	R0	R0	K2
          0x58080003,  //  0006  LDCONST	R2	K3
          0x580C0004,  //  0007  LDCONST	R3	K4
          0x7C000600,  //  0008  CALL	R0	3
          0x80000000,  //  0009  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(wifi),
    /* K2   */  be_nested_str_weak(up),
    /* K3   */  be_nested_str_weak(_mdns_announce_hostname),
    /* K4   */  be_nested_str_weak(add_rule),
    /* K5   */  be_nested_str_weak(Wifi_X23Connected),
    /* K6   */  be_nested_str_weak(matter_mdns_host),
    /* K7   */  be_nested_str_weak(eth),
    /* K8   */  be_nested_str_weak(Eth_X23Connected),
    }),
    be_str_weak(start_mdns_announce_hostnames),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x94040302,  //  0003  GETIDX	R1	R1	K2
      0x78060003,  //  0004  JMPF	R1	#0009
      0x8C040103,  //  0005  GETMET	R1	R0	K3
      0x500C0000,  //  0006  LDBOOL	R3	0	0
      0x7C040400,  //  0007  CALL	R1	2
      0x70020005,  //  0008  JMP		#000F
      0xB8060000,  //  0009  GETNGBL	R1	K0
      0x8C040304,  //  000A  GETMET	R1	R1	K4
      0x580C0005,  //  000B  LDCONST	R3	K5
      0x84100000,  //  000C  CLOSURE	R4	P0
      0x58140006,  //  000D  LDCONST	R5	K6
      0x7C040800,  //  000E  CALL	R1	4
      0xB8060000,  //  000F  GETNGBL	R1	K0
      0x8C040307,  //  0010  GETMET	R1	R1	K7
      0x7C040200,  //  0011  CALL	R1	1
      0x94040302,  //  0012  GETIDX	R1	R1	K2
      0x78060003,  //  0013  JMPF	R1	#0018
      0x8C040103,  //  0014  GETMET	R1	R0	K3
      0x500C0200,  //  0015  LDBOOL	R3	1	0
      0x7C040400,  //  0016  CALL	R1	2
      0x70020005,  //  0017  JMP		#001E
      0xB8060000,  //  0018  GETNGBL	R1	K0
      0x8C040304,  //  0019  GETMET	R1	R1	K4
      0x580C0008,  //  001A  LDCONST	R3	K8
      0x84100001,  //  001B  CLOSURE	R4	P1
      0x58140006,  //  001C  LDCONST	R5	K6
      0x7C040800,  //  001D  CALL	R1	4
      0xA0000000,  //  001E  CLOSE	R0
      0x80000000,  //  001F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: MtrJoin
********************************************************************/
be_local_closure(Matter_Device_MtrJoin,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(start_root_basic_commissioning),
    /* K1   */  be_nested_str_weak(stop_basic_commissioning),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(resp_cmnd_done),
    }),
    be_str_weak(MtrJoin),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x60140009,  //  0000  GETGBL	R5	G9
      0x5C180600,  //  0001  MOVE	R6	R3
      0x7C140200,  //  0002  CALL	R5	1
      0x78160002,  //  0003  JMPF	R5	#0007
      0x8C180100,  //  0004  GETMET	R6	R0	K0
      0x7C180200,  //  0005  CALL	R6	1
      0x70020001,  //  0006  JMP		#0009
      0x8C180101,  //  0007  GETMET	R6	R0	K1
      0x7C180200,  //  0008  CALL	R6	1
      0xB81A0400,  //  0009  GETNGBL	R6	K2
      0x8C180D03,  //  000A  GETMET	R6	R6	K3
      0x7C180200,  //  000B  CALL	R6	1
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _load_plugins_config
********************************************************************/
be_local_closure(Matter_Device__load_plugins_config,   /* name */
  be_nested_proto(
    19,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[27]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(k2l_num),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(log),
    /* K4   */  be_nested_str_weak(MTR_X3A_X20endpoints_X20to_X20be_X20configured_X20),
    /* K5   */  be_const_int(3),
    /* K6   */  be_nested_str_weak(format),
    /* K7   */  be_nested_str_weak(MTR_X3A_X20endpoint_X20_X25i_X20config_X20_X25s),
    /* K8   */  be_nested_str_weak(find),
    /* K9   */  be_nested_str_weak(type),
    /* K10  */  be_nested_str_weak(MTR_X3A_X20no_X20class_X20name_X2C_X20skipping),
    /* K11  */  be_nested_str_weak(plugins_classes),
    /* K12  */  be_nested_str_weak(MTR_X3A_X20unknown_X20class_X20name_X20_X27),
    /* K13  */  be_nested_str_weak(_X27_X20skipping),
    /* K14  */  be_const_int(2),
    /* K15  */  be_nested_str_weak(plugins),
    /* K16  */  be_nested_str_weak(push),
    /* K17  */  be_nested_str_weak(),
    /* K18  */  be_nested_str_weak(k2l),
    /* K19  */  be_nested_str_weak(_X20_X25s_X3A_X25s),
    /* K20  */  be_nested_str_weak(stop_iteration),
    /* K21  */  be_nested_str_weak(MTR_X3A_X20endpoint_X3A_X25i_X20type_X3A_X25s_X25s),
    /* K22  */  be_nested_str_weak(MTR_X3A_X20Exception),
    /* K23  */  be_nested_str_weak(_X7C),
    /* K24  */  be_nested_str_weak(publish_result),
    /* K25  */  be_nested_str_weak(_X7B_X22Matter_X22_X3A_X7B_X22Initialized_X22_X3A1_X7D_X7D),
    /* K26  */  be_nested_str_weak(Matter),
    }),
    be_str_weak(_load_plugins_config),
    &be_const_str_solidified,
    ( &(const binstruction[133]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0101,  //  0001  GETMET	R3	R0	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0xB8120400,  //  0004  GETNGBL	R4	K2
      0x8C100903,  //  0005  GETMET	R4	R4	K3
      0x60180008,  //  0006  GETGBL	R6	G8
      0x5C1C0600,  //  0007  MOVE	R7	R3
      0x7C180200,  //  0008  CALL	R6	1
      0x001A0806,  //  0009  ADD	R6	K4	R6
      0x581C0005,  //  000A  LDCONST	R7	K5
      0x7C100600,  //  000B  CALL	R4	3
      0x60100010,  //  000C  GETGBL	R4	G16
      0x5C140600,  //  000D  MOVE	R5	R3
      0x7C100200,  //  000E  CALL	R4	1
      0xA802006B,  //  000F  EXBLK	0	#007C
      0x5C140800,  //  0010  MOVE	R5	R4
      0x7C140000,  //  0011  CALL	R5	0
      0xA8020056,  //  0012  EXBLK	0	#006A
      0x60180008,  //  0013  GETGBL	R6	G8
      0x5C1C0A00,  //  0014  MOVE	R7	R5
      0x7C180200,  //  0015  CALL	R6	1
      0x94180206,  //  0016  GETIDX	R6	R1	R6
      0xB81E0400,  //  0017  GETNGBL	R7	K2
      0x8C1C0F03,  //  0018  GETMET	R7	R7	K3
      0x8C240506,  //  0019  GETMET	R9	R2	K6
      0x582C0007,  //  001A  LDCONST	R11	K7
      0x5C300A00,  //  001B  MOVE	R12	R5
      0x5C340C00,  //  001C  MOVE	R13	R6
      0x7C240800,  //  001D  CALL	R9	4
      0x58280005,  //  001E  LDCONST	R10	K5
      0x7C1C0600,  //  001F  CALL	R7	3
      0x8C1C0D08,  //  0020  GETMET	R7	R6	K8
      0x58240009,  //  0021  LDCONST	R9	K9
      0x7C1C0400,  //  0022  CALL	R7	2
      0x4C200000,  //  0023  LDNIL	R8
      0x1C200E08,  //  0024  EQ	R8	R7	R8
      0x78220006,  //  0025  JMPF	R8	#002D
      0xB8220400,  //  0026  GETNGBL	R8	K2
      0x8C201103,  //  0027  GETMET	R8	R8	K3
      0x5828000A,  //  0028  LDCONST	R10	K10
      0x582C0005,  //  0029  LDCONST	R11	K5
      0x7C200600,  //  002A  CALL	R8	3
      0xA8040001,  //  002B  EXBLK	1	1
      0x7001FFE2,  //  002C  JMP		#0010
      0x8820010B,  //  002D  GETMBR	R8	R0	K11
      0x8C201108,  //  002E  GETMET	R8	R8	K8
      0x5C280E00,  //  002F  MOVE	R10	R7
      0x7C200400,  //  0030  CALL	R8	2
      0x4C240000,  //  0031  LDNIL	R9
      0x1C241009,  //  0032  EQ	R9	R8	R9
      0x7826000A,  //  0033  JMPF	R9	#003F
      0xB8260400,  //  0034  GETNGBL	R9	K2
      0x8C241303,  //  0035  GETMET	R9	R9	K3
      0x602C0008,  //  0036  GETGBL	R11	G8
      0x5C300E00,  //  0037  MOVE	R12	R7
      0x7C2C0200,  //  0038  CALL	R11	1
      0x002E180B,  //  0039  ADD	R11	K12	R11
      0x002C170D,  //  003A  ADD	R11	R11	K13
      0x5830000E,  //  003B  LDCONST	R12	K14
      0x7C240600,  //  003C  CALL	R9	3
      0xA8040001,  //  003D  EXBLK	1	1
      0x7001FFD0,  //  003E  JMP		#0010
      0x5C241000,  //  003F  MOVE	R9	R8
      0x5C280000,  //  0040  MOVE	R10	R0
      0x5C2C0A00,  //  0041  MOVE	R11	R5
      0x5C300C00,  //  0042  MOVE	R12	R6
      0x7C240600,  //  0043  CALL	R9	3
      0x8828010F,  //  0044  GETMBR	R10	R0	K15
      0x8C281510,  //  0045  GETMET	R10	R10	K16
      0x5C301200,  //  0046  MOVE	R12	R9
      0x7C280400,  //  0047  CALL	R10	2
      0x58280011,  //  0048  LDCONST	R10	K17
      0x602C0010,  //  0049  GETGBL	R11	G16
      0x8C300112,  //  004A  GETMET	R12	R0	K18
      0x5C380C00,  //  004B  MOVE	R14	R6
      0x7C300400,  //  004C  CALL	R12	2
      0x7C2C0200,  //  004D  CALL	R11	1
      0xA802000B,  //  004E  EXBLK	0	#005B
      0x5C301600,  //  004F  MOVE	R12	R11
      0x7C300000,  //  0050  CALL	R12	0
      0x1C341909,  //  0051  EQ	R13	R12	K9
      0x78360000,  //  0052  JMPF	R13	#0054
      0x7001FFFA,  //  0053  JMP		#004F
      0x8C340506,  //  0054  GETMET	R13	R2	K6
      0x583C0013,  //  0055  LDCONST	R15	K19
      0x5C401800,  //  0056  MOVE	R16	R12
      0x94440C0C,  //  0057  GETIDX	R17	R6	R12
      0x7C340800,  //  0058  CALL	R13	4
      0x0028140D,  //  0059  ADD	R10	R10	R13
      0x7001FFF3,  //  005A  JMP		#004F
      0x582C0014,  //  005B  LDCONST	R11	K20
      0xAC2C0200,  //  005C  CATCH	R11	1	0
      0xB0080000,  //  005D  RAISE	2	R0	R0
      0xB82E0400,  //  005E  GETNGBL	R11	K2
      0x8C2C1703,  //  005F  GETMET	R11	R11	K3
      0x8C340506,  //  0060  GETMET	R13	R2	K6
      0x583C0015,  //  0061  LDCONST	R15	K21
      0x5C400A00,  //  0062  MOVE	R16	R5
      0x5C440E00,  //  0063  MOVE	R17	R7
      0x5C481400,  //  0064  MOVE	R18	R10
      0x7C340A00,  //  0065  CALL	R13	5
      0x5838000E,  //  0066  LDCONST	R14	K14
      0x7C2C0600,  //  0067  CALL	R11	3
      0xA8040001,  //  0068  EXBLK	1	1
      0x70020010,  //  0069  JMP		#007B
      0xAC180002,  //  006A  CATCH	R6	0	2
      0x7002000D,  //  006B  JMP		#007A
      0xB8220400,  //  006C  GETNGBL	R8	K2
      0x8C201103,  //  006D  GETMET	R8	R8	K3
      0x60280008,  //  006E  GETGBL	R10	G8
      0x5C2C0C00,  //  006F  MOVE	R11	R6
      0x7C280200,  //  0070  CALL	R10	1
      0x002A2C0A,  //  0071  ADD	R10	K22	R10
      0x00281517,  //  0072  ADD	R10	R10	K23
      0x602C0008,  //  0073  GETGBL	R11	G8
      0x5C300E00,  //  0074  MOVE	R12	R7
      0x7C2C0200,  //  0075  CALL	R11	1
      0x0028140B,  //  0076  ADD	R10	R10	R11
      0x582C000E,  //  0077  LDCONST	R11	K14
      0x7C200600,  //  0078  CALL	R8	3
      0x70020000,  //  0079  JMP		#007B
      0xB0080000,  //  007A  RAISE	2	R0	R0
      0x7001FF93,  //  007B  JMP		#0010
      0x58100014,  //  007C  LDCONST	R4	K20
      0xAC100200,  //  007D  CATCH	R4	1	0
      0xB0080000,  //  007E  RAISE	2	R0	R0
      0xB8120400,  //  007F  GETNGBL	R4	K2
      0x8C100918,  //  0080  GETMET	R4	R4	K24
      0x58180019,  //  0081  LDCONST	R6	K25
      0x581C001A,  //  0082  LDCONST	R7	K26
      0x7C100600,  //  0083  CALL	R4	3
      0x80000000,  //  0084  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: save_before_restart
********************************************************************/
be_local_closure(Matter_Device_save_before_restart,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(stop_basic_commissioning),
    /* K1   */  be_nested_str_weak(mdns_remove_op_discovery_all_fabrics),
    }),
    be_str_weak(save_before_restart),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x7C040200,  //  0003  CALL	R1	1
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_root_basic_commissioning
********************************************************************/
be_local_closure(Matter_Device_start_root_basic_commissioning,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[23]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(PASE_TIMEOUT),
    /* K2   */  be_nested_str_weak(compute_manual_pairing_code),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(log),
    /* K5   */  be_nested_str_weak(format),
    /* K6   */  be_nested_str_weak(MTR_X3A_X20Manual_X20pairing_X20code_X3A_X20_X25s_X2D_X25s_X2D_X25s),
    /* K7   */  be_const_int(0),
    /* K8   */  be_const_int(3),
    /* K9   */  be_const_int(2147483647),
    /* K10  */  be_const_int(2),
    /* K11  */  be_nested_str_weak(compute_qrcode_content),
    /* K12  */  be_nested_str_weak(publish_result),
    /* K13  */  be_nested_str_weak(_X7B_X22Matter_X22_X3A_X7B_X22Commissioning_X22_X3A1_X2C_X22PairingCode_X22_X3A_X22_X25s_X22_X2C_X22QRCode_X22_X3A_X22_X25s_X22_X7D_X7D),
    /* K14  */  be_nested_str_weak(Matter),
    /* K15  */  be_nested_str_weak(_compute_pbkdf),
    /* K16  */  be_nested_str_weak(root_passcode),
    /* K17  */  be_nested_str_weak(root_iterations),
    /* K18  */  be_nested_str_weak(root_salt),
    /* K19  */  be_nested_str_weak(start_basic_commissioning),
    /* K20  */  be_nested_str_weak(root_discriminator),
    /* K21  */  be_nested_str_weak(root_w0),
    /* K22  */  be_nested_str_weak(root_L),
    }),
    be_str_weak(start_root_basic_commissioning),
    &be_const_str_solidified,
    ( &(const binstruction[49]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x1C0C0203,  //  0002  EQ	R3	R1	R3
      0x780E0000,  //  0003  JMPF	R3	#0005
      0x88040101,  //  0004  GETMBR	R1	R0	K1
      0x8C0C0102,  //  0005  GETMET	R3	R0	K2
      0x7C0C0200,  //  0006  CALL	R3	1
      0xB8120600,  //  0007  GETNGBL	R4	K3
      0x8C100904,  //  0008  GETMET	R4	R4	K4
      0x8C180505,  //  0009  GETMET	R6	R2	K5
      0x58200006,  //  000A  LDCONST	R8	K6
      0x40260F08,  //  000B  CONNECT	R9	K7	K8
      0x94240609,  //  000C  GETIDX	R9	R3	R9
      0x542A0003,  //  000D  LDINT	R10	4
      0x542E0005,  //  000E  LDINT	R11	6
      0x4028140B,  //  000F  CONNECT	R10	R10	R11
      0x9428060A,  //  0010  GETIDX	R10	R3	R10
      0x542E0006,  //  0011  LDINT	R11	7
      0x402C1709,  //  0012  CONNECT	R11	R11	K9
      0x942C060B,  //  0013  GETIDX	R11	R3	R11
      0x7C180A00,  //  0014  CALL	R6	5
      0x581C000A,  //  0015  LDCONST	R7	K10
      0x7C100600,  //  0016  CALL	R4	3
      0x8C10010B,  //  0017  GETMET	R4	R0	K11
      0x7C100200,  //  0018  CALL	R4	1
      0xB8160600,  //  0019  GETNGBL	R5	K3
      0x8C140B0C,  //  001A  GETMET	R5	R5	K12
      0x8C1C0505,  //  001B  GETMET	R7	R2	K5
      0x5824000D,  //  001C  LDCONST	R9	K13
      0x5C280600,  //  001D  MOVE	R10	R3
      0x5C2C0800,  //  001E  MOVE	R11	R4
      0x7C1C0800,  //  001F  CALL	R7	4
      0x5820000E,  //  0020  LDCONST	R8	K14
      0x7C140600,  //  0021  CALL	R5	3
      0x8C14010F,  //  0022  GETMET	R5	R0	K15
      0x881C0110,  //  0023  GETMBR	R7	R0	K16
      0x88200111,  //  0024  GETMBR	R8	R0	K17
      0x88240112,  //  0025  GETMBR	R9	R0	K18
      0x7C140800,  //  0026  CALL	R5	4
      0x8C140113,  //  0027  GETMET	R5	R0	K19
      0x5C1C0200,  //  0028  MOVE	R7	R1
      0x88200111,  //  0029  GETMBR	R8	R0	K17
      0x88240114,  //  002A  GETMBR	R9	R0	K20
      0x88280112,  //  002B  GETMBR	R10	R0	K18
      0x882C0115,  //  002C  GETMBR	R11	R0	K21
      0x88300116,  //  002D  GETMBR	R12	R0	K22
      0x4C340000,  //  002E  LDNIL	R13
      0x7C141000,  //  002F  CALL	R5	8
      0x80000000,  //  0030  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: received_ack
********************************************************************/
be_local_closure(Matter_Device_received_ack,   /* name */
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
    /* K0   */  be_nested_str_weak(udp_server),
    /* K1   */  be_nested_str_weak(received_ack),
    }),
    be_str_weak(received_ack),
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
** Solidified function: _compute_pbkdf
********************************************************************/
be_local_closure(Matter_Device__compute_pbkdf,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(add),
    /* K3   */  be_nested_str_weak(PBKDF2_HMAC_SHA256),
    /* K4   */  be_nested_str_weak(derive),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(root_w0),
    /* K7   */  be_nested_str_weak(EC_P256),
    /* K8   */  be_nested_str_weak(mod),
    /* K9   */  be_nested_str_weak(root_L),
    /* K10  */  be_nested_str_weak(public_key),
    }),
    be_str_weak(_compute_pbkdf),
    &be_const_str_solidified,
    ( &(const binstruction[41]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0xA4160200,  //  0001  IMPORT	R5	K1
      0x60180015,  //  0002  GETGBL	R6	G21
      0x7C180000,  //  0003  CALL	R6	0
      0x8C180D02,  //  0004  GETMET	R6	R6	K2
      0x5C200200,  //  0005  MOVE	R8	R1
      0x54260003,  //  0006  LDINT	R9	4
      0x7C180600,  //  0007  CALL	R6	3
      0x8C1C0903,  //  0008  GETMET	R7	R4	K3
      0x7C1C0200,  //  0009  CALL	R7	1
      0x8C1C0F04,  //  000A  GETMET	R7	R7	K4
      0x5C240C00,  //  000B  MOVE	R9	R6
      0x5C280600,  //  000C  MOVE	R10	R3
      0x5C2C0400,  //  000D  MOVE	R11	R2
      0x5432004F,  //  000E  LDINT	R12	80
      0x7C1C0A00,  //  000F  CALL	R7	5
      0x54220026,  //  0010  LDINT	R8	39
      0x40220A08,  //  0011  CONNECT	R8	K5	R8
      0x94200E08,  //  0012  GETIDX	R8	R7	R8
      0x54260027,  //  0013  LDINT	R9	40
      0x542A004E,  //  0014  LDINT	R10	79
      0x4024120A,  //  0015  CONNECT	R9	R9	R10
      0x94240E09,  //  0016  GETIDX	R9	R7	R9
      0x8C280907,  //  0017  GETMET	R10	R4	K7
      0x7C280200,  //  0018  CALL	R10	1
      0x8C281508,  //  0019  GETMET	R10	R10	K8
      0x5C301000,  //  001A  MOVE	R12	R8
      0x7C280400,  //  001B  CALL	R10	2
      0x90020C0A,  //  001C  SETMBR	R0	K6	R10
      0x8C280907,  //  001D  GETMET	R10	R4	K7
      0x7C280200,  //  001E  CALL	R10	1
      0x8C281508,  //  001F  GETMET	R10	R10	K8
      0x5C301200,  //  0020  MOVE	R12	R9
      0x7C280400,  //  0021  CALL	R10	2
      0x8C2C0907,  //  0022  GETMET	R11	R4	K7
      0x7C2C0200,  //  0023  CALL	R11	1
      0x8C2C170A,  //  0024  GETMET	R11	R11	K10
      0x5C341400,  //  0025  MOVE	R13	R10
      0x7C2C0400,  //  0026  CALL	R11	2
      0x9002120B,  //  0027  SETMBR	R0	K9	R11
      0x80000000,  //  0028  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_commissioning_complete_deferred
********************************************************************/
be_local_closure(Matter_Device_start_commissioning_complete_deferred,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        3,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 2]) {  /* upvals */
          be_local_const_upval(1, 0),
          be_local_const_upval(1, 1),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(start_commissioning_complete),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 5]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x68080001,  //  0002  GETUPV	R2	U1
          0x7C000400,  //  0003  CALL	R0	2
          0x80040000,  //  0004  RET	1	R0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(set_timer),
    /* K2   */  be_const_int(0),
    }),
    be_str_weak(start_commissioning_complete_deferred),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x58100002,  //  0002  LDCONST	R4	K2
      0x84140000,  //  0003  CLOSURE	R5	P0
      0x7C080600,  //  0004  CALL	R2	3
      0xA0000000,  //  0005  CLOSE	R0
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_basic_commissioning
********************************************************************/
be_local_closure(Matter_Device_start_basic_commissioning,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    8,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        4,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 4]) {     /* constants */
        /* K0   */  be_nested_str_weak(mdns_announce_PASE),
        /* K1   */  be_nested_str_weak(tasmota),
        /* K2   */  be_nested_str_weak(remove_rule),
        /* K3   */  be_nested_str_weak(Wifi_X23Connected),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[ 9]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0xB8020200,  //  0003  GETNGBL	R0	K1
          0x8C000102,  //  0004  GETMET	R0	R0	K2
          0x58080003,  //  0005  LDCONST	R2	K3
          0x580C0000,  //  0006  LDCONST	R3	K0
          0x7C000600,  //  0007  CALL	R0	3
          0x80000000,  //  0008  RET	0
        })
      ),
      be_nested_proto(
        4,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 4]) {     /* constants */
        /* K0   */  be_nested_str_weak(mdns_announce_PASE),
        /* K1   */  be_nested_str_weak(tasmota),
        /* K2   */  be_nested_str_weak(remove_rule),
        /* K3   */  be_nested_str_weak(Eth_X23Connected),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[ 9]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0xB8020200,  //  0003  GETNGBL	R0	K1
          0x8C000102,  //  0004  GETMET	R0	R0	K2
          0x58080003,  //  0005  LDCONST	R2	K3
          0x580C0000,  //  0006  LDCONST	R3	K0
          0x7C000600,  //  0007  CALL	R0	3
          0x80000000,  //  0008  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(commissioning_open),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(millis),
    /* K3   */  be_nested_str_weak(commissioning_iterations),
    /* K4   */  be_nested_str_weak(commissioning_discriminator),
    /* K5   */  be_nested_str_weak(commissioning_salt),
    /* K6   */  be_nested_str_weak(commissioning_w0),
    /* K7   */  be_nested_str_weak(commissioning_L),
    /* K8   */  be_nested_str_weak(commissioning_admin_fabric),
    /* K9   */  be_nested_str_weak(wifi),
    /* K10  */  be_nested_str_weak(up),
    /* K11  */  be_nested_str_weak(eth),
    /* K12  */  be_nested_str_weak(mdns_announce_PASE),
    /* K13  */  be_nested_str_weak(add_rule),
    /* K14  */  be_nested_str_weak(Wifi_X23Connected),
    /* K15  */  be_nested_str_weak(Eth_X23Connected),
    }),
    be_str_weak(start_basic_commissioning),
    &be_const_str_solidified,
    ( &(const binstruction[40]) {  /* code */
      0xB8220200,  //  0000  GETNGBL	R8	K1
      0x8C201102,  //  0001  GETMET	R8	R8	K2
      0x7C200200,  //  0002  CALL	R8	1
      0x542603E7,  //  0003  LDINT	R9	1000
      0x08240209,  //  0004  MUL	R9	R1	R9
      0x00201009,  //  0005  ADD	R8	R8	R9
      0x90020008,  //  0006  SETMBR	R0	K0	R8
      0x90020602,  //  0007  SETMBR	R0	K3	R2
      0x90020803,  //  0008  SETMBR	R0	K4	R3
      0x90020A04,  //  0009  SETMBR	R0	K5	R4
      0x90020C05,  //  000A  SETMBR	R0	K6	R5
      0x90020E06,  //  000B  SETMBR	R0	K7	R6
      0x90021007,  //  000C  SETMBR	R0	K8	R7
      0xB8220200,  //  000D  GETNGBL	R8	K1
      0x8C201109,  //  000E  GETMET	R8	R8	K9
      0x7C200200,  //  000F  CALL	R8	1
      0x9420110A,  //  0010  GETIDX	R8	R8	K10
      0x74220004,  //  0011  JMPT	R8	#0017
      0xB8220200,  //  0012  GETNGBL	R8	K1
      0x8C20110B,  //  0013  GETMET	R8	R8	K11
      0x7C200200,  //  0014  CALL	R8	1
      0x9420110A,  //  0015  GETIDX	R8	R8	K10
      0x78220002,  //  0016  JMPF	R8	#001A
      0x8C20010C,  //  0017  GETMET	R8	R0	K12
      0x7C200200,  //  0018  CALL	R8	1
      0x7002000B,  //  0019  JMP		#0026
      0xB8220200,  //  001A  GETNGBL	R8	K1
      0x8C20110D,  //  001B  GETMET	R8	R8	K13
      0x5828000E,  //  001C  LDCONST	R10	K14
      0x842C0000,  //  001D  CLOSURE	R11	P0
      0x5830000C,  //  001E  LDCONST	R12	K12
      0x7C200800,  //  001F  CALL	R8	4
      0xB8220200,  //  0020  GETNGBL	R8	K1
      0x8C20110D,  //  0021  GETMET	R8	R8	K13
      0x5828000F,  //  0022  LDCONST	R10	K15
      0x842C0001,  //  0023  CLOSURE	R11	P1
      0x5830000C,  //  0024  LDCONST	R12	K12
      0x7C200800,  //  0025  CALL	R8	4
      0xA0000000,  //  0026  CLOSE	R0
      0x80000000,  //  0027  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: mdns_announce_op_discovery_all_fabrics
********************************************************************/
be_local_closure(Matter_Device_mdns_announce_op_discovery_all_fabrics,   /* name */
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
    /* K0   */  be_nested_str_weak(sessions),
    /* K1   */  be_nested_str_weak(active_fabrics),
    /* K2   */  be_nested_str_weak(get_device_id),
    /* K3   */  be_nested_str_weak(get_fabric_id),
    /* K4   */  be_nested_str_weak(mdns_announce_op_discovery),
    /* K5   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(mdns_announce_op_discovery_all_fabrics),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x60040010,  //  0000  GETGBL	R1	G16
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x8C080501,  //  0002  GETMET	R2	R2	K1
      0x7C080200,  //  0003  CALL	R2	1
      0x7C040200,  //  0004  CALL	R1	1
      0xA802000B,  //  0005  EXBLK	0	#0012
      0x5C080200,  //  0006  MOVE	R2	R1
      0x7C080000,  //  0007  CALL	R2	0
      0x8C0C0502,  //  0008  GETMET	R3	R2	K2
      0x7C0C0200,  //  0009  CALL	R3	1
      0x780E0005,  //  000A  JMPF	R3	#0011
      0x8C0C0503,  //  000B  GETMET	R3	R2	K3
      0x7C0C0200,  //  000C  CALL	R3	1
      0x780E0002,  //  000D  JMPF	R3	#0011
      0x8C0C0104,  //  000E  GETMET	R3	R0	K4
      0x5C140400,  //  000F  MOVE	R5	R2
      0x7C0C0400,  //  0010  CALL	R3	2
      0x7001FFF3,  //  0011  JMP		#0006
      0x58040005,  //  0012  LDCONST	R1	K5
      0xAC040200,  //  0013  CATCH	R1	1	0
      0xB0080000,  //  0014  RAISE	2	R0	R0
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: load_param
********************************************************************/
be_local_closure(Matter_Device_load_param,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[28]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(crypto),
    /* K2   */  be_nested_str_weak(FILENAME),
    /* K3   */  be_nested_str_weak(read),
    /* K4   */  be_nested_str_weak(close),
    /* K5   */  be_nested_str_weak(json),
    /* K6   */  be_nested_str_weak(load),
    /* K7   */  be_nested_str_weak(root_discriminator),
    /* K8   */  be_nested_str_weak(find),
    /* K9   */  be_nested_str_weak(distinguish),
    /* K10  */  be_nested_str_weak(root_passcode),
    /* K11  */  be_nested_str_weak(passcode),
    /* K12  */  be_nested_str_weak(ipv4only),
    /* K13  */  be_nested_str_weak(plugins_config),
    /* K14  */  be_nested_str_weak(config),
    /* K15  */  be_nested_str_weak(_load_plugins_config),
    /* K16  */  be_nested_str_weak(plugins_persist),
    /* K17  */  be_nested_str_weak(io_error),
    /* K18  */  be_nested_str_weak(tasmota),
    /* K19  */  be_nested_str_weak(log),
    /* K20  */  be_nested_str_weak(MTR_X3A_X20Session_Store_X3A_X3Aload_X20Exception_X3A),
    /* K21  */  be_nested_str_weak(_X7C),
    /* K22  */  be_const_int(2),
    /* K23  */  be_nested_str_weak(random),
    /* K24  */  be_nested_str_weak(get),
    /* K25  */  be_const_int(0),
    /* K26  */  be_nested_str_weak(generate_random_passcode),
    /* K27  */  be_nested_str_weak(save_param),
    }),
    be_str_weak(load_param),
    &be_const_str_solidified,
    ( &(const binstruction[91]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0xA8020028,  //  0002  EXBLK	0	#002C
      0x600C0011,  //  0003  GETGBL	R3	G17
      0x88100102,  //  0004  GETMBR	R4	R0	K2
      0x7C0C0200,  //  0005  CALL	R3	1
      0x8C100703,  //  0006  GETMET	R4	R3	K3
      0x7C100200,  //  0007  CALL	R4	1
      0x8C140704,  //  0008  GETMET	R5	R3	K4
      0x7C140200,  //  0009  CALL	R5	1
      0xA4160A00,  //  000A  IMPORT	R5	K5
      0x8C180B06,  //  000B  GETMET	R6	R5	K6
      0x5C200800,  //  000C  MOVE	R8	R4
      0x7C180400,  //  000D  CALL	R6	2
      0x8C1C0D08,  //  000E  GETMET	R7	R6	K8
      0x58240009,  //  000F  LDCONST	R9	K9
      0x88280107,  //  0010  GETMBR	R10	R0	K7
      0x7C1C0600,  //  0011  CALL	R7	3
      0x90020E07,  //  0012  SETMBR	R0	K7	R7
      0x8C1C0D08,  //  0013  GETMET	R7	R6	K8
      0x5824000B,  //  0014  LDCONST	R9	K11
      0x8828010A,  //  0015  GETMBR	R10	R0	K10
      0x7C1C0600,  //  0016  CALL	R7	3
      0x90021407,  //  0017  SETMBR	R0	K10	R7
      0x601C0017,  //  0018  GETGBL	R7	G23
      0x8C200D08,  //  0019  GETMET	R8	R6	K8
      0x5828000C,  //  001A  LDCONST	R10	K12
      0x502C0000,  //  001B  LDBOOL	R11	0	0
      0x7C200600,  //  001C  CALL	R8	3
      0x7C1C0200,  //  001D  CALL	R7	1
      0x90021807,  //  001E  SETMBR	R0	K12	R7
      0x8C1C0D08,  //  001F  GETMET	R7	R6	K8
      0x5824000E,  //  0020  LDCONST	R9	K14
      0x7C1C0400,  //  0021  CALL	R7	2
      0x90021A07,  //  0022  SETMBR	R0	K13	R7
      0x881C010D,  //  0023  GETMBR	R7	R0	K13
      0x781E0004,  //  0024  JMPF	R7	#002A
      0x8C1C010F,  //  0025  GETMET	R7	R0	K15
      0x8824010D,  //  0026  GETMBR	R9	R0	K13
      0x7C1C0400,  //  0027  CALL	R7	2
      0x501C0200,  //  0028  LDBOOL	R7	1	0
      0x90022007,  //  0029  SETMBR	R0	K16	R7
      0xA8040001,  //  002A  EXBLK	1	1
      0x70020012,  //  002B  JMP		#003F
      0xAC0C0002,  //  002C  CATCH	R3	0	2
      0x7002000F,  //  002D  JMP		#003E
      0x20140711,  //  002E  NE	R5	R3	K17
      0x7816000C,  //  002F  JMPF	R5	#003D
      0xB8162400,  //  0030  GETNGBL	R5	K18
      0x8C140B13,  //  0031  GETMET	R5	R5	K19
      0x601C0008,  //  0032  GETGBL	R7	G8
      0x5C200600,  //  0033  MOVE	R8	R3
      0x7C1C0200,  //  0034  CALL	R7	1
      0x001E2807,  //  0035  ADD	R7	K20	R7
      0x001C0F15,  //  0036  ADD	R7	R7	K21
      0x60200008,  //  0037  GETGBL	R8	G8
      0x5C240800,  //  0038  MOVE	R9	R4
      0x7C200200,  //  0039  CALL	R8	1
      0x001C0E08,  //  003A  ADD	R7	R7	R8
      0x58200016,  //  003B  LDCONST	R8	K22
      0x7C140600,  //  003C  CALL	R5	3
      0x70020000,  //  003D  JMP		#003F
      0xB0080000,  //  003E  RAISE	2	R0	R0
      0x500C0000,  //  003F  LDBOOL	R3	0	0
      0x88100107,  //  0040  GETMBR	R4	R0	K7
      0x4C140000,  //  0041  LDNIL	R5
      0x1C100805,  //  0042  EQ	R4	R4	R5
      0x7812000A,  //  0043  JMPF	R4	#004F
      0x8C100517,  //  0044  GETMET	R4	R2	K23
      0x58180016,  //  0045  LDCONST	R6	K22
      0x7C100400,  //  0046  CALL	R4	2
      0x8C100918,  //  0047  GETMET	R4	R4	K24
      0x58180019,  //  0048  LDCONST	R6	K25
      0x581C0016,  //  0049  LDCONST	R7	K22
      0x7C100600,  //  004A  CALL	R4	3
      0x54160FFE,  //  004B  LDINT	R5	4095
      0x2C100805,  //  004C  AND	R4	R4	R5
      0x90020E04,  //  004D  SETMBR	R0	K7	R4
      0x500C0200,  //  004E  LDBOOL	R3	1	0
      0x8810010A,  //  004F  GETMBR	R4	R0	K10
      0x4C140000,  //  0050  LDNIL	R5
      0x1C100805,  //  0051  EQ	R4	R4	R5
      0x78120003,  //  0052  JMPF	R4	#0057
      0x8C10011A,  //  0053  GETMET	R4	R0	K26
      0x7C100200,  //  0054  CALL	R4	1
      0x90021404,  //  0055  SETMBR	R0	K10	R4
      0x500C0200,  //  0056  LDBOOL	R3	1	0
      0x780E0001,  //  0057  JMPF	R3	#005A
      0x8C10011B,  //  0058  GETMET	R4	R0	K27
      0x7C100200,  //  0059  CALL	R4	1
      0x80000000,  //  005A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: k2l
********************************************************************/
be_local_closure(Matter_Device_k2l,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_const_class(be_class_Matter_Device),
    /* K1   */  be_nested_str_weak(keys),
    /* K2   */  be_nested_str_weak(push),
    /* K3   */  be_nested_str_weak(stop_iteration),
    /* K4   */  be_const_int(1),
    /* K5   */  be_const_int(0),
    }),
    be_str_weak(k2l),
    &be_const_str_solidified,
    ( &(const binstruction[50]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x60080012,  //  0001  GETGBL	R2	G18
      0x7C080000,  //  0002  CALL	R2	0
      0x4C0C0000,  //  0003  LDNIL	R3
      0x1C0C0003,  //  0004  EQ	R3	R0	R3
      0x780E0000,  //  0005  JMPF	R3	#0007
      0x80040400,  //  0006  RET	1	R2
      0x600C0010,  //  0007  GETGBL	R3	G16
      0x8C100101,  //  0008  GETMET	R4	R0	K1
      0x7C100200,  //  0009  CALL	R4	1
      0x7C0C0200,  //  000A  CALL	R3	1
      0xA8020005,  //  000B  EXBLK	0	#0012
      0x5C100600,  //  000C  MOVE	R4	R3
      0x7C100000,  //  000D  CALL	R4	0
      0x8C140502,  //  000E  GETMET	R5	R2	K2
      0x5C1C0800,  //  000F  MOVE	R7	R4
      0x7C140400,  //  0010  CALL	R5	2
      0x7001FFF9,  //  0011  JMP		#000C
      0x580C0003,  //  0012  LDCONST	R3	K3
      0xAC0C0200,  //  0013  CATCH	R3	1	0
      0xB0080000,  //  0014  RAISE	2	R0	R0
      0x600C0010,  //  0015  GETGBL	R3	G16
      0x6010000C,  //  0016  GETGBL	R4	G12
      0x5C140400,  //  0017  MOVE	R5	R2
      0x7C100200,  //  0018  CALL	R4	1
      0x04100904,  //  0019  SUB	R4	R4	K4
      0x40120804,  //  001A  CONNECT	R4	K4	R4
      0x7C0C0200,  //  001B  CALL	R3	1
      0xA8020010,  //  001C  EXBLK	0	#002E
      0x5C100600,  //  001D  MOVE	R4	R3
      0x7C100000,  //  001E  CALL	R4	0
      0x94140404,  //  001F  GETIDX	R5	R2	R4
      0x5C180800,  //  0020  MOVE	R6	R4
      0x241C0D05,  //  0021  GT	R7	R6	K5
      0x781E0008,  //  0022  JMPF	R7	#002C
      0x041C0D04,  //  0023  SUB	R7	R6	K4
      0x941C0407,  //  0024  GETIDX	R7	R2	R7
      0x241C0E05,  //  0025  GT	R7	R7	R5
      0x781E0004,  //  0026  JMPF	R7	#002C
      0x041C0D04,  //  0027  SUB	R7	R6	K4
      0x941C0407,  //  0028  GETIDX	R7	R2	R7
      0x98080C07,  //  0029  SETIDX	R2	R6	R7
      0x04180D04,  //  002A  SUB	R6	R6	K4
      0x7001FFF4,  //  002B  JMP		#0021
      0x98080C05,  //  002C  SETIDX	R2	R6	R5
      0x7001FFEE,  //  002D  JMP		#001D
      0x580C0003,  //  002E  LDCONST	R3	K3
      0xAC0C0200,  //  002F  CATCH	R3	1	0
      0xB0080000,  //  0030  RAISE	2	R0	R0
      0x80040400,  //  0031  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: autoconf_device
********************************************************************/
be_local_closure(Matter_Device_autoconf_device,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(json),
    /* K2   */  be_nested_str_weak(plugins),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(plugins_config),
    /* K5   */  be_nested_str_weak(autoconf_device_map),
    /* K6   */  be_nested_str_weak(tasmota),
    /* K7   */  be_nested_str_weak(log),
    /* K8   */  be_nested_str_weak(MTR_X3A_X20autoconfig_X20_X3D_X20),
    /* K9   */  be_const_int(3),
    /* K10  */  be_nested_str_weak(_load_plugins_config),
    /* K11  */  be_nested_str_weak(plugins_persist),
    /* K12  */  be_nested_str_weak(sessions),
    /* K13  */  be_nested_str_weak(count_active_fabrics),
    /* K14  */  be_nested_str_weak(save_param),
    }),
    be_str_weak(autoconf_device),
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x600C000C,  //  0002  GETGBL	R3	G12
      0x88100102,  //  0003  GETMBR	R4	R0	K2
      0x7C0C0200,  //  0004  CALL	R3	1
      0x240C0703,  //  0005  GT	R3	R3	K3
      0x780E0000,  //  0006  JMPF	R3	#0008
      0x80000600,  //  0007  RET	0
      0x8C0C0105,  //  0008  GETMET	R3	R0	K5
      0x7C0C0200,  //  0009  CALL	R3	1
      0x90020803,  //  000A  SETMBR	R0	K4	R3
      0xB80E0C00,  //  000B  GETNGBL	R3	K6
      0x8C0C0707,  //  000C  GETMET	R3	R3	K7
      0x60140008,  //  000D  GETGBL	R5	G8
      0x88180104,  //  000E  GETMBR	R6	R0	K4
      0x7C140200,  //  000F  CALL	R5	1
      0x00161005,  //  0010  ADD	R5	K8	R5
      0x58180009,  //  0011  LDCONST	R6	K9
      0x7C0C0600,  //  0012  CALL	R3	3
      0x8C0C010A,  //  0013  GETMET	R3	R0	K10
      0x88140104,  //  0014  GETMBR	R5	R0	K4
      0x7C0C0400,  //  0015  CALL	R3	2
      0x880C010B,  //  0016  GETMBR	R3	R0	K11
      0x740E0008,  //  0017  JMPT	R3	#0021
      0x880C010C,  //  0018  GETMBR	R3	R0	K12
      0x8C0C070D,  //  0019  GETMET	R3	R3	K13
      0x7C0C0200,  //  001A  CALL	R3	1
      0x240C0703,  //  001B  GT	R3	R3	K3
      0x780E0003,  //  001C  JMPF	R3	#0021
      0x500C0200,  //  001D  LDBOOL	R3	1	0
      0x90021603,  //  001E  SETMBR	R0	K11	R3
      0x8C0C010E,  //  001F  GETMET	R3	R0	K14
      0x7C0C0200,  //  0020  CALL	R3	1
      0x80000000,  //  0021  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_attribute_expansion
********************************************************************/
be_local_closure(Matter_Device_process_attribute_expansion,   /* name */
  be_nested_proto(
    29,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
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
        /* K0   */  be_nested_str_weak(keys),
        /* K1   */  be_nested_str_weak(push),
        /* K2   */  be_nested_str_weak(stop_iteration),
        /* K3   */  be_const_int(1),
        /* K4   */  be_const_int(0),
        }),
        be_str_weak(keys_sorted),
        &be_const_str_solidified,
        ( &(const binstruction[45]) {  /* code */
          0x60040012,  //  0000  GETGBL	R1	G18
          0x7C040000,  //  0001  CALL	R1	0
          0x60080010,  //  0002  GETGBL	R2	G16
          0x8C0C0100,  //  0003  GETMET	R3	R0	K0
          0x7C0C0200,  //  0004  CALL	R3	1
          0x7C080200,  //  0005  CALL	R2	1
          0xA8020005,  //  0006  EXBLK	0	#000D
          0x5C0C0400,  //  0007  MOVE	R3	R2
          0x7C0C0000,  //  0008  CALL	R3	0
          0x8C100301,  //  0009  GETMET	R4	R1	K1
          0x5C180600,  //  000A  MOVE	R6	R3
          0x7C100400,  //  000B  CALL	R4	2
          0x7001FFF9,  //  000C  JMP		#0007
          0x58080002,  //  000D  LDCONST	R2	K2
          0xAC080200,  //  000E  CATCH	R2	1	0
          0xB0080000,  //  000F  RAISE	2	R0	R0
          0x60080010,  //  0010  GETGBL	R2	G16
          0x600C000C,  //  0011  GETGBL	R3	G12
          0x5C100200,  //  0012  MOVE	R4	R1
          0x7C0C0200,  //  0013  CALL	R3	1
          0x040C0703,  //  0014  SUB	R3	R3	K3
          0x400E0603,  //  0015  CONNECT	R3	K3	R3
          0x7C080200,  //  0016  CALL	R2	1
          0xA8020010,  //  0017  EXBLK	0	#0029
          0x5C0C0400,  //  0018  MOVE	R3	R2
          0x7C0C0000,  //  0019  CALL	R3	0
          0x94100203,  //  001A  GETIDX	R4	R1	R3
          0x5C140600,  //  001B  MOVE	R5	R3
          0x24180B04,  //  001C  GT	R6	R5	K4
          0x781A0008,  //  001D  JMPF	R6	#0027
          0x04180B03,  //  001E  SUB	R6	R5	K3
          0x94180206,  //  001F  GETIDX	R6	R1	R6
          0x24180C04,  //  0020  GT	R6	R6	R4
          0x781A0004,  //  0021  JMPF	R6	#0027
          0x04180B03,  //  0022  SUB	R6	R5	K3
          0x94180206,  //  0023  GETIDX	R6	R1	R6
          0x98040A06,  //  0024  SETIDX	R1	R5	R6
          0x04140B03,  //  0025  SUB	R5	R5	K3
          0x7001FFF4,  //  0026  JMP		#001C
          0x98040A04,  //  0027  SETIDX	R1	R5	R4
          0x7001FFEE,  //  0028  JMP		#0018
          0x58080002,  //  0029  LDCONST	R2	K2
          0xAC080200,  //  002A  CATCH	R2	1	0
          0xB0080000,  //  002B  RAISE	2	R0	R0
          0x80040200,  //  002C  RET	1	R1
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[22]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(endpoint),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(attribute),
    /* K4   */  be_nested_str_weak(plugins),
    /* K5   */  be_nested_str_weak(get_endpoint),
    /* K6   */  be_nested_str_weak(contains),
    /* K7   */  be_nested_str_weak(get_cluster_list),
    /* K8   */  be_nested_str_weak(get_attribute_list),
    /* K9   */  be_nested_str_weak(push),
    /* K10  */  be_nested_str_weak(stop_iteration),
    /* K11  */  be_nested_str_weak(tasmota),
    /* K12  */  be_nested_str_weak(log),
    /* K13  */  be_nested_str_weak(format),
    /* K14  */  be_nested_str_weak(MTR_X3A_X20expansion_X20_X5B_X2502X_X5D_X2504X_X2F_X2504X),
    /* K15  */  be_const_int(3),
    /* K16  */  be_nested_str_weak(status),
    /* K17  */  be_nested_str_weak(matter),
    /* K18  */  be_nested_str_weak(UNSUPPORTED_ENDPOINT),
    /* K19  */  be_nested_str_weak(UNSUPPORTED_CLUSTER),
    /* K20  */  be_nested_str_weak(UNSUPPORTED_ATTRIBUTE),
    /* K21  */  be_nested_str_weak(UNREPORTABLE_ATTRIBUTE),
    }),
    be_str_weak(process_attribute_expansion),
    &be_const_str_solidified,
    ( &(const binstruction[216]) {  /* code */
      0x840C0000,  //  0000  CLOSURE	R3	P0
      0xA4120000,  //  0001  IMPORT	R4	K0
      0x88140301,  //  0002  GETMBR	R5	R1	K1
      0x50180000,  //  0003  LDBOOL	R6	0	0
      0x881C0302,  //  0004  GETMBR	R7	R1	K2
      0x50200000,  //  0005  LDBOOL	R8	0	0
      0x88240303,  //  0006  GETMBR	R9	R1	K3
      0x50280000,  //  0007  LDBOOL	R10	0	0
      0x882C0301,  //  0008  GETMBR	R11	R1	K1
      0x4C300000,  //  0009  LDNIL	R12
      0x202C160C,  //  000A  NE	R11	R11	R12
      0x782E0007,  //  000B  JMPF	R11	#0014
      0x882C0302,  //  000C  GETMBR	R11	R1	K2
      0x4C300000,  //  000D  LDNIL	R12
      0x202C160C,  //  000E  NE	R11	R11	R12
      0x782E0003,  //  000F  JMPF	R11	#0014
      0x882C0303,  //  0010  GETMBR	R11	R1	K3
      0x4C300000,  //  0011  LDNIL	R12
      0x202C160C,  //  0012  NE	R11	R11	R12
      0x742E0000,  //  0013  JMPT	R11	#0015
      0x502C0001,  //  0014  LDBOOL	R11	0	1
      0x502C0200,  //  0015  LDBOOL	R11	1	0
      0x60300013,  //  0016  GETGBL	R12	G19
      0x7C300000,  //  0017  CALL	R12	0
      0x60340010,  //  0018  GETGBL	R13	G16
      0x88380104,  //  0019  GETMBR	R14	R0	K4
      0x7C340200,  //  001A  CALL	R13	1
      0xA8020055,  //  001B  EXBLK	0	#0072
      0x5C381A00,  //  001C  MOVE	R14	R13
      0x7C380000,  //  001D  CALL	R14	0
      0x8C3C1D05,  //  001E  GETMET	R15	R14	K5
      0x7C3C0200,  //  001F  CALL	R15	1
      0x4C400000,  //  0020  LDNIL	R16
      0x20400A10,  //  0021  NE	R16	R5	R16
      0x78420002,  //  0022  JMPF	R16	#0026
      0x20401E05,  //  0023  NE	R16	R15	R5
      0x78420000,  //  0024  JMPF	R16	#0026
      0x7001FFF5,  //  0025  JMP		#001C
      0x8C401906,  //  0026  GETMET	R16	R12	K6
      0x5C481E00,  //  0027  MOVE	R18	R15
      0x7C400400,  //  0028  CALL	R16	2
      0x74420002,  //  0029  JMPT	R16	#002D
      0x60400013,  //  002A  GETGBL	R16	G19
      0x7C400000,  //  002B  CALL	R16	0
      0x98301E10,  //  002C  SETIDX	R12	R15	R16
      0x50180200,  //  002D  LDBOOL	R6	1	0
      0x8C401D07,  //  002E  GETMET	R16	R14	K7
      0x5C481E00,  //  002F  MOVE	R18	R15
      0x7C400400,  //  0030  CALL	R16	2
      0x60440010,  //  0031  GETGBL	R17	G16
      0x5C482000,  //  0032  MOVE	R18	R16
      0x7C440200,  //  0033  CALL	R17	1
      0xA8020038,  //  0034  EXBLK	0	#006E
      0x5C482200,  //  0035  MOVE	R18	R17
      0x7C480000,  //  0036  CALL	R18	0
      0x4C4C0000,  //  0037  LDNIL	R19
      0x204C0E13,  //  0038  NE	R19	R7	R19
      0x784E0002,  //  0039  JMPF	R19	#003D
      0x204C2407,  //  003A  NE	R19	R18	R7
      0x784E0000,  //  003B  JMPF	R19	#003D
      0x7001FFF7,  //  003C  JMP		#0035
      0x944C180F,  //  003D  GETIDX	R19	R12	R15
      0x8C4C2706,  //  003E  GETMET	R19	R19	K6
      0x5C542400,  //  003F  MOVE	R21	R18
      0x7C4C0400,  //  0040  CALL	R19	2
      0x744E0003,  //  0041  JMPT	R19	#0046
      0x944C180F,  //  0042  GETIDX	R19	R12	R15
      0x60500013,  //  0043  GETGBL	R20	G19
      0x7C500000,  //  0044  CALL	R20	0
      0x984C2414,  //  0045  SETIDX	R19	R18	R20
      0x50200200,  //  0046  LDBOOL	R8	1	0
      0x8C4C1D08,  //  0047  GETMET	R19	R14	K8
      0x5C541E00,  //  0048  MOVE	R21	R15
      0x5C582400,  //  0049  MOVE	R22	R18
      0x7C4C0600,  //  004A  CALL	R19	3
      0x60500010,  //  004B  GETGBL	R20	G16
      0x5C542600,  //  004C  MOVE	R21	R19
      0x7C500200,  //  004D  CALL	R20	1
      0xA802001A,  //  004E  EXBLK	0	#006A
      0x5C542800,  //  004F  MOVE	R21	R20
      0x7C540000,  //  0050  CALL	R21	0
      0x4C580000,  //  0051  LDNIL	R22
      0x20581216,  //  0052  NE	R22	R9	R22
      0x785A0002,  //  0053  JMPF	R22	#0057
      0x20582A09,  //  0054  NE	R22	R21	R9
      0x785A0000,  //  0055  JMPF	R22	#0057
      0x7001FFF7,  //  0056  JMP		#004F
      0x9458180F,  //  0057  GETIDX	R22	R12	R15
      0x94582C12,  //  0058  GETIDX	R22	R22	R18
      0x8C582D06,  //  0059  GETMET	R22	R22	K6
      0x5C602A00,  //  005A  MOVE	R24	R21
      0x7C580400,  //  005B  CALL	R22	2
      0x745A0004,  //  005C  JMPT	R22	#0062
      0x9458180F,  //  005D  GETIDX	R22	R12	R15
      0x94582C12,  //  005E  GETIDX	R22	R22	R18
      0x605C0012,  //  005F  GETGBL	R23	G18
      0x7C5C0000,  //  0060  CALL	R23	0
      0x98582A17,  //  0061  SETIDX	R22	R21	R23
      0x50280200,  //  0062  LDBOOL	R10	1	0
      0x9458180F,  //  0063  GETIDX	R22	R12	R15
      0x94582C12,  //  0064  GETIDX	R22	R22	R18
      0x94582C15,  //  0065  GETIDX	R22	R22	R21
      0x8C582D09,  //  0066  GETMET	R22	R22	K9
      0x5C601C00,  //  0067  MOVE	R24	R14
      0x7C580400,  //  0068  CALL	R22	2
      0x7001FFE4,  //  0069  JMP		#004F
      0x5850000A,  //  006A  LDCONST	R20	K10
      0xAC500200,  //  006B  CATCH	R20	1	0
      0xB0080000,  //  006C  RAISE	2	R0	R0
      0x7001FFC6,  //  006D  JMP		#0035
      0x5844000A,  //  006E  LDCONST	R17	K10
      0xAC440200,  //  006F  CATCH	R17	1	0
      0xB0080000,  //  0070  RAISE	2	R0	R0
      0x7001FFA9,  //  0071  JMP		#001C
      0x5834000A,  //  0072  LDCONST	R13	K10
      0xAC340200,  //  0073  CATCH	R13	1	0
      0xB0080000,  //  0074  RAISE	2	R0	R0
      0x60340010,  //  0075  GETGBL	R13	G16
      0x5C380600,  //  0076  MOVE	R14	R3
      0x5C3C1800,  //  0077  MOVE	R15	R12
      0x7C380200,  //  0078  CALL	R14	1
      0x7C340200,  //  0079  CALL	R13	1
      0xA802003D,  //  007A  EXBLK	0	#00B9
      0x5C381A00,  //  007B  MOVE	R14	R13
      0x7C380000,  //  007C  CALL	R14	0
      0x603C0010,  //  007D  GETGBL	R15	G16
      0x5C400600,  //  007E  MOVE	R16	R3
      0x9444180E,  //  007F  GETIDX	R17	R12	R14
      0x7C400200,  //  0080  CALL	R16	1
      0x7C3C0200,  //  0081  CALL	R15	1
      0xA8020031,  //  0082  EXBLK	0	#00B5
      0x5C401E00,  //  0083  MOVE	R16	R15
      0x7C400000,  //  0084  CALL	R16	0
      0x60440010,  //  0085  GETGBL	R17	G16
      0x5C480600,  //  0086  MOVE	R18	R3
      0x944C180E,  //  0087  GETIDX	R19	R12	R14
      0x944C2610,  //  0088  GETIDX	R19	R19	R16
      0x7C480200,  //  0089  CALL	R18	1
      0x7C440200,  //  008A  CALL	R17	1
      0xA8020024,  //  008B  EXBLK	0	#00B1
      0x5C482200,  //  008C  MOVE	R18	R17
      0x7C480000,  //  008D  CALL	R18	0
      0x604C0010,  //  008E  GETGBL	R19	G16
      0x9450180E,  //  008F  GETIDX	R20	R12	R14
      0x94502810,  //  0090  GETIDX	R20	R20	R16
      0x94502812,  //  0091  GETIDX	R20	R20	R18
      0x7C4C0200,  //  0092  CALL	R19	1
      0xA8020018,  //  0093  EXBLK	0	#00AD
      0x5C502600,  //  0094  MOVE	R20	R19
      0x7C500000,  //  0095  CALL	R20	0
      0xB8561600,  //  0096  GETNGBL	R21	K11
      0x8C542B0C,  //  0097  GETMET	R21	R21	K12
      0x8C5C090D,  //  0098  GETMET	R23	R4	K13
      0x5864000E,  //  0099  LDCONST	R25	K14
      0x5C681C00,  //  009A  MOVE	R26	R14
      0x5C6C2000,  //  009B  MOVE	R27	R16
      0x5C702400,  //  009C  MOVE	R28	R18
      0x7C5C0A00,  //  009D  CALL	R23	5
      0x5860000F,  //  009E  LDCONST	R24	K15
      0x7C540600,  //  009F  CALL	R21	3
      0x9006020E,  //  00A0  SETMBR	R1	K1	R14
      0x90060410,  //  00A1  SETMBR	R1	K2	R16
      0x90060612,  //  00A2  SETMBR	R1	K3	R18
      0x5C540400,  //  00A3  MOVE	R21	R2
      0x5C582800,  //  00A4  MOVE	R22	R20
      0x5C5C0200,  //  00A5  MOVE	R23	R1
      0x5C601600,  //  00A6  MOVE	R24	R11
      0x7C540600,  //  00A7  CALL	R21	3
      0x782E0002,  //  00A8  JMPF	R11	#00AC
      0x78560001,  //  00A9  JMPF	R21	#00AC
      0xA8040004,  //  00AA  EXBLK	1	4
      0x80002C00,  //  00AB  RET	0
      0x7001FFE6,  //  00AC  JMP		#0094
      0x584C000A,  //  00AD  LDCONST	R19	K10
      0xAC4C0200,  //  00AE  CATCH	R19	1	0
      0xB0080000,  //  00AF  RAISE	2	R0	R0
      0x7001FFDA,  //  00B0  JMP		#008C
      0x5844000A,  //  00B1  LDCONST	R17	K10
      0xAC440200,  //  00B2  CATCH	R17	1	0
      0xB0080000,  //  00B3  RAISE	2	R0	R0
      0x7001FFCD,  //  00B4  JMP		#0083
      0x583C000A,  //  00B5  LDCONST	R15	K10
      0xAC3C0200,  //  00B6  CATCH	R15	1	0
      0xB0080000,  //  00B7  RAISE	2	R0	R0
      0x7001FFC1,  //  00B8  JMP		#007B
      0x5834000A,  //  00B9  LDCONST	R13	K10
      0xAC340200,  //  00BA  CATCH	R13	1	0
      0xB0080000,  //  00BB  RAISE	2	R0	R0
      0x782E0019,  //  00BC  JMPF	R11	#00D7
      0x5C340C00,  //  00BD  MOVE	R13	R6
      0x74360003,  //  00BE  JMPT	R13	#00C3
      0xB8362200,  //  00BF  GETNGBL	R13	K17
      0x88341B12,  //  00C0  GETMBR	R13	R13	K18
      0x9006200D,  //  00C1  SETMBR	R1	K16	R13
      0x7002000E,  //  00C2  JMP		#00D2
      0x5C341000,  //  00C3  MOVE	R13	R8
      0x74360003,  //  00C4  JMPT	R13	#00C9
      0xB8362200,  //  00C5  GETNGBL	R13	K17
      0x88341B13,  //  00C6  GETMBR	R13	R13	K19
      0x9006200D,  //  00C7  SETMBR	R1	K16	R13
      0x70020008,  //  00C8  JMP		#00D2
      0x5C341400,  //  00C9  MOVE	R13	R10
      0x74360003,  //  00CA  JMPT	R13	#00CF
      0xB8362200,  //  00CB  GETNGBL	R13	K17
      0x88341B14,  //  00CC  GETMBR	R13	R13	K20
      0x9006200D,  //  00CD  SETMBR	R1	K16	R13
      0x70020002,  //  00CE  JMP		#00D2
      0xB8362200,  //  00CF  GETNGBL	R13	K17
      0x88341B15,  //  00D0  GETMBR	R13	R13	K21
      0x9006200D,  //  00D1  SETMBR	R1	K16	R13
      0x5C340400,  //  00D2  MOVE	R13	R2
      0x4C380000,  //  00D3  LDNIL	R14
      0x5C3C0200,  //  00D4  MOVE	R15	R1
      0x50400200,  //  00D5  LDBOOL	R16	1	0
      0x7C340600,  //  00D6  CALL	R13	3
      0x80000000,  //  00D7  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: msg_send
********************************************************************/
be_local_closure(Matter_Device_msg_send,   /* name */
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
    /* K0   */  be_nested_str_weak(udp_server),
    /* K1   */  be_nested_str_weak(send_UDP),
    }),
    be_str_weak(msg_send),
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
** Solidified function: get_plugin_class_displayname
********************************************************************/
be_local_closure(Matter_Device_get_plugin_class_displayname,   /* name */
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
    /* K0   */  be_nested_str_weak(plugins_classes),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(NAME),
    /* K3   */  be_nested_str_weak(),
    }),
    be_str_weak(get_plugin_class_displayname),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x780A0001,  //  0004  JMPF	R2	#0007
      0x880C0502,  //  0005  GETMBR	R3	R2	K2
      0x70020000,  //  0006  JMP		#0008
      0x580C0003,  //  0007  LDCONST	R3	K3
      0x80040600,  //  0008  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: mdns_remove_op_discovery_all_fabrics
********************************************************************/
be_local_closure(Matter_Device_mdns_remove_op_discovery_all_fabrics,   /* name */
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
    /* K0   */  be_nested_str_weak(sessions),
    /* K1   */  be_nested_str_weak(active_fabrics),
    /* K2   */  be_nested_str_weak(get_device_id),
    /* K3   */  be_nested_str_weak(get_fabric_id),
    /* K4   */  be_nested_str_weak(mdns_remove_op_discovery),
    /* K5   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(mdns_remove_op_discovery_all_fabrics),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x60040010,  //  0000  GETGBL	R1	G16
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x8C080501,  //  0002  GETMET	R2	R2	K1
      0x7C080200,  //  0003  CALL	R2	1
      0x7C040200,  //  0004  CALL	R1	1
      0xA802000B,  //  0005  EXBLK	0	#0012
      0x5C080200,  //  0006  MOVE	R2	R1
      0x7C080000,  //  0007  CALL	R2	0
      0x8C0C0502,  //  0008  GETMET	R3	R2	K2
      0x7C0C0200,  //  0009  CALL	R3	1
      0x780E0005,  //  000A  JMPF	R3	#0011
      0x8C0C0503,  //  000B  GETMET	R3	R2	K3
      0x7C0C0200,  //  000C  CALL	R3	1
      0x780E0002,  //  000D  JMPF	R3	#0011
      0x8C0C0104,  //  000E  GETMET	R3	R0	K4
      0x5C140400,  //  000F  MOVE	R5	R2
      0x7C0C0400,  //  0010  CALL	R3	2
      0x7001FFF3,  //  0011  JMP		#0006
      0x58040005,  //  0012  LDCONST	R1	K5
      0xAC040200,  //  0013  CATCH	R1	1	0
      0xB0080000,  //  0014  RAISE	2	R0	R0
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: msg_received
********************************************************************/
be_local_closure(Matter_Device_msg_received,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(message_handler),
    /* K1   */  be_nested_str_weak(msg_received),
    }),
    be_str_weak(msg_received),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88100100,  //  0000  GETMBR	R4	R0	K0
      0x8C100901,  //  0001  GETMET	R4	R4	K1
      0x5C180200,  //  0002  MOVE	R6	R1
      0x5C1C0400,  //  0003  MOVE	R7	R2
      0x5C200600,  //  0004  MOVE	R8	R3
      0x7C100800,  //  0005  CALL	R4	4
      0x80040800,  //  0006  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_operational_discovery
********************************************************************/
be_local_closure(Matter_Device_start_operational_discovery,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(mdns),
    /* K2   */  be_nested_str_weak(string),
    /* K3   */  be_nested_str_weak(stop_basic_commissioning),
    /* K4   */  be_nested_str_weak(root_w0),
    /* K5   */  be_nested_str_weak(root_L),
    /* K6   */  be_nested_str_weak(set_expire_in_seconds),
    /* K7   */  be_nested_str_weak(mdns_announce_op_discovery),
    /* K8   */  be_nested_str_weak(get_fabric),
    }),
    be_str_weak(start_operational_discovery),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0xA4120400,  //  0002  IMPORT	R4	K2
      0x8C140103,  //  0003  GETMET	R5	R0	K3
      0x7C140200,  //  0004  CALL	R5	1
      0x4C140000,  //  0005  LDNIL	R5
      0x90020805,  //  0006  SETMBR	R0	K4	R5
      0x4C140000,  //  0007  LDNIL	R5
      0x90020A05,  //  0008  SETMBR	R0	K5	R5
      0x8C140306,  //  0009  GETMET	R5	R1	K6
      0x541E003B,  //  000A  LDINT	R7	60
      0x7C140400,  //  000B  CALL	R5	2
      0x8C140107,  //  000C  GETMET	R5	R0	K7
      0x8C1C0308,  //  000D  GETMET	R7	R1	K8
      0x7C1C0200,  //  000E  CALL	R7	1
      0x7C140400,  //  000F  CALL	R5	2
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_250ms
********************************************************************/
be_local_closure(Matter_Device_every_250ms,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(message_handler),
    /* K1   */  be_nested_str_weak(every_250ms),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(plugins),
    /* K4   */  be_const_int(1),
    }),
    be_str_weak(every_250ms),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x58040002,  //  0003  LDCONST	R1	K2
      0x6008000C,  //  0004  GETGBL	R2	G12
      0x880C0103,  //  0005  GETMBR	R3	R0	K3
      0x7C080200,  //  0006  CALL	R2	1
      0x14080202,  //  0007  LT	R2	R1	R2
      0x780A0005,  //  0008  JMPF	R2	#000F
      0x88080103,  //  0009  GETMBR	R2	R0	K3
      0x94080401,  //  000A  GETIDX	R2	R2	R1
      0x8C080501,  //  000B  GETMET	R2	R2	K1
      0x7C080200,  //  000C  CALL	R2	1
      0x00040304,  //  000D  ADD	R1	R1	K4
      0x7001FFF4,  //  000E  JMP		#0004
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: register_plugin_class
********************************************************************/
be_local_closure(Matter_Device_register_plugin_class,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(introspect),
    /* K1   */  be_nested_str_weak(get),
    /* K2   */  be_nested_str_weak(TYPE),
    /* K3   */  be_nested_str_weak(plugins_classes),
    }),
    be_str_weak(register_plugin_class),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0501,  //  0001  GETMET	R3	R2	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x58180002,  //  0003  LDCONST	R6	K2
      0x7C0C0600,  //  0004  CALL	R3	3
      0x780E0001,  //  0005  JMPF	R3	#0008
      0x88100103,  //  0006  GETMBR	R4	R0	K3
      0x98100601,  //  0007  SETIDX	R4	R3	R1
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: mdns_remove_PASE
********************************************************************/
be_local_closure(Matter_Device_mdns_remove_PASE,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[22]) {     /* constants */
    /* K0   */  be_nested_str_weak(mdns),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(mdns_pase_eth),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(log),
    /* K5   */  be_nested_str_weak(format),
    /* K6   */  be_nested_str_weak(MTR_X3A_X20calling_X20mdns_X2Eremove_service_X28_X25s_X2C_X20_X25s_X2C_X20_X25s_X2C_X20_X25s_X29),
    /* K7   */  be_nested_str_weak(_matterc),
    /* K8   */  be_nested_str_weak(_udp),
    /* K9   */  be_nested_str_weak(commissioning_instance_eth),
    /* K10  */  be_nested_str_weak(hostname_eth),
    /* K11  */  be_const_int(3),
    /* K12  */  be_nested_str_weak(MTR_X3A_X20remove_X20mDNS_X20on_X20_X25s_X20_X27_X25s_X27),
    /* K13  */  be_nested_str_weak(eth),
    /* K14  */  be_const_int(2),
    /* K15  */  be_nested_str_weak(remove_service),
    /* K16  */  be_nested_str_weak(mdns_pase_wifi),
    /* K17  */  be_nested_str_weak(commissioning_instance_wifi),
    /* K18  */  be_nested_str_weak(hostname_wifi),
    /* K19  */  be_nested_str_weak(wifi),
    /* K20  */  be_nested_str_weak(MTR_X3A_X20Exception),
    /* K21  */  be_nested_str_weak(_X7C),
    }),
    be_str_weak(mdns_remove_PASE),
    &be_const_str_solidified,
    ( &(const binstruction[83]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0xA802003D,  //  0002  EXBLK	0	#0041
      0x880C0102,  //  0003  GETMBR	R3	R0	K2
      0x780E001B,  //  0004  JMPF	R3	#0021
      0xB80E0600,  //  0005  GETNGBL	R3	K3
      0x8C0C0704,  //  0006  GETMET	R3	R3	K4
      0x8C140505,  //  0007  GETMET	R5	R2	K5
      0x581C0006,  //  0008  LDCONST	R7	K6
      0x58200007,  //  0009  LDCONST	R8	K7
      0x58240008,  //  000A  LDCONST	R9	K8
      0x88280109,  //  000B  GETMBR	R10	R0	K9
      0x882C010A,  //  000C  GETMBR	R11	R0	K10
      0x7C140C00,  //  000D  CALL	R5	6
      0x5818000B,  //  000E  LDCONST	R6	K11
      0x7C0C0600,  //  000F  CALL	R3	3
      0xB80E0600,  //  0010  GETNGBL	R3	K3
      0x8C0C0704,  //  0011  GETMET	R3	R3	K4
      0x8C140505,  //  0012  GETMET	R5	R2	K5
      0x581C000C,  //  0013  LDCONST	R7	K12
      0x5820000D,  //  0014  LDCONST	R8	K13
      0x88240109,  //  0015  GETMBR	R9	R0	K9
      0x7C140800,  //  0016  CALL	R5	4
      0x5818000E,  //  0017  LDCONST	R6	K14
      0x7C0C0600,  //  0018  CALL	R3	3
      0x500C0000,  //  0019  LDBOOL	R3	0	0
      0x90020403,  //  001A  SETMBR	R0	K2	R3
      0x8C0C030F,  //  001B  GETMET	R3	R1	K15
      0x58140007,  //  001C  LDCONST	R5	K7
      0x58180008,  //  001D  LDCONST	R6	K8
      0x881C0109,  //  001E  GETMBR	R7	R0	K9
      0x8820010A,  //  001F  GETMBR	R8	R0	K10
      0x7C0C0A00,  //  0020  CALL	R3	5
      0x880C0110,  //  0021  GETMBR	R3	R0	K16
      0x780E001B,  //  0022  JMPF	R3	#003F
      0xB80E0600,  //  0023  GETNGBL	R3	K3
      0x8C0C0704,  //  0024  GETMET	R3	R3	K4
      0x8C140505,  //  0025  GETMET	R5	R2	K5
      0x581C0006,  //  0026  LDCONST	R7	K6
      0x58200007,  //  0027  LDCONST	R8	K7
      0x58240008,  //  0028  LDCONST	R9	K8
      0x88280111,  //  0029  GETMBR	R10	R0	K17
      0x882C0112,  //  002A  GETMBR	R11	R0	K18
      0x7C140C00,  //  002B  CALL	R5	6
      0x5818000B,  //  002C  LDCONST	R6	K11
      0x7C0C0600,  //  002D  CALL	R3	3
      0xB80E0600,  //  002E  GETNGBL	R3	K3
      0x8C0C0704,  //  002F  GETMET	R3	R3	K4
      0x8C140505,  //  0030  GETMET	R5	R2	K5
      0x581C000C,  //  0031  LDCONST	R7	K12
      0x58200013,  //  0032  LDCONST	R8	K19
      0x88240111,  //  0033  GETMBR	R9	R0	K17
      0x7C140800,  //  0034  CALL	R5	4
      0x5818000E,  //  0035  LDCONST	R6	K14
      0x7C0C0600,  //  0036  CALL	R3	3
      0x500C0000,  //  0037  LDBOOL	R3	0	0
      0x90022003,  //  0038  SETMBR	R0	K16	R3
      0x8C0C030F,  //  0039  GETMET	R3	R1	K15
      0x58140007,  //  003A  LDCONST	R5	K7
      0x58180008,  //  003B  LDCONST	R6	K8
      0x881C0111,  //  003C  GETMBR	R7	R0	K17
      0x88200112,  //  003D  GETMBR	R8	R0	K18
      0x7C0C0A00,  //  003E  CALL	R3	5
      0xA8040001,  //  003F  EXBLK	1	1
      0x70020010,  //  0040  JMP		#0052
      0xAC0C0002,  //  0041  CATCH	R3	0	2
      0x7002000D,  //  0042  JMP		#0051
      0xB8160600,  //  0043  GETNGBL	R5	K3
      0x8C140B04,  //  0044  GETMET	R5	R5	K4
      0x601C0008,  //  0045  GETGBL	R7	G8
      0x5C200600,  //  0046  MOVE	R8	R3
      0x7C1C0200,  //  0047  CALL	R7	1
      0x001E2807,  //  0048  ADD	R7	K20	R7
      0x001C0F15,  //  0049  ADD	R7	R7	K21
      0x60200008,  //  004A  GETGBL	R8	G8
      0x5C240800,  //  004B  MOVE	R9	R4
      0x7C200200,  //  004C  CALL	R8	1
      0x001C0E08,  //  004D  ADD	R7	R7	R8
      0x5820000E,  //  004E  LDCONST	R8	K14
      0x7C140600,  //  004F  CALL	R5	3
      0x70020000,  //  0050  JMP		#0052
      0xB0080000,  //  0051  RAISE	2	R0	R0
      0x80000000,  //  0052  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: compute_manual_pairing_code
********************************************************************/
be_local_closure(Matter_Device_compute_manual_pairing_code,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(root_discriminator),
    /* K2   */  be_nested_str_weak(root_passcode),
    /* K3   */  be_nested_str_weak(format),
    /* K4   */  be_nested_str_weak(_X251i_X2505i_X2504i),
    /* K5   */  be_nested_str_weak(matter),
    /* K6   */  be_nested_str_weak(Verhoeff),
    /* K7   */  be_nested_str_weak(checksum),
    }),
    be_str_weak(compute_manual_pairing_code),
    &be_const_str_solidified,
    ( &(const binstruction[31]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x540E0FFE,  //  0002  LDINT	R3	4095
      0x2C080403,  //  0003  AND	R2	R2	R3
      0x540E0009,  //  0004  LDINT	R3	10
      0x3C080403,  //  0005  SHR	R2	R2	R3
      0x880C0101,  //  0006  GETMBR	R3	R0	K1
      0x541202FF,  //  0007  LDINT	R4	768
      0x2C0C0604,  //  0008  AND	R3	R3	R4
      0x54120005,  //  0009  LDINT	R4	6
      0x380C0604,  //  000A  SHL	R3	R3	R4
      0x88100102,  //  000B  GETMBR	R4	R0	K2
      0x54163FFE,  //  000C  LDINT	R5	16383
      0x2C100805,  //  000D  AND	R4	R4	R5
      0x300C0604,  //  000E  OR	R3	R3	R4
      0x88100102,  //  000F  GETMBR	R4	R0	K2
      0x5416000D,  //  0010  LDINT	R5	14
      0x3C100805,  //  0011  SHR	R4	R4	R5
      0x8C140303,  //  0012  GETMET	R5	R1	K3
      0x581C0004,  //  0013  LDCONST	R7	K4
      0x5C200400,  //  0014  MOVE	R8	R2
      0x5C240600,  //  0015  MOVE	R9	R3
      0x5C280800,  //  0016  MOVE	R10	R4
      0x7C140A00,  //  0017  CALL	R5	5
      0xB81A0A00,  //  0018  GETNGBL	R6	K5
      0x88180D06,  //  0019  GETMBR	R6	R6	K6
      0x8C180D07,  //  001A  GETMET	R6	R6	K7
      0x5C200A00,  //  001B  MOVE	R8	R5
      0x7C180400,  //  001C  CALL	R6	2
      0x00140A06,  //  001D  ADD	R5	R5	R6
      0x80040A00,  //  001E  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Device
********************************************************************/
be_local_class(Matter_Device,
    32,
    NULL,
    be_nested_map(93,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(message_handler, -1), be_const_var(6) },
        { be_const_key_weak(generate_random_passcode, -1), be_const_closure(Matter_Device_generate_random_passcode_closure) },
        { be_const_key_weak(save_param, -1), be_const_closure(Matter_Device_save_param_closure) },
        { be_const_key_weak(mdns_pase_wifi, -1), be_const_var(24) },
        { be_const_key_weak(remove_fabric, 28), be_const_closure(Matter_Device_remove_fabric_closure) },
        { be_const_key_weak(_start_udp, -1), be_const_closure(Matter_Device__start_udp_closure) },
        { be_const_key_weak(root_iterations, -1), be_const_var(28) },
        { be_const_key_weak(commissioning_discriminator, -1), be_const_var(12) },
        { be_const_key_weak(commissioning_w0, -1), be_const_var(14) },
        { be_const_key_weak(compute_manual_pairing_code, 66), be_const_closure(Matter_Device_compute_manual_pairing_code_closure) },
        { be_const_key_weak(root_discriminator, -1), be_const_var(25) },
        { be_const_key_weak(stop, -1), be_const_closure(Matter_Device_stop_closure) },
        { be_const_key_weak(mdns_remove_PASE, 22), be_const_closure(Matter_Device_mdns_remove_PASE_closure) },
        { be_const_key_weak(plugins_config, -1), be_const_var(4) },
        { be_const_key_weak(start_commissioning_complete, -1), be_const_closure(Matter_Device_start_commissioning_complete_closure) },
        { be_const_key_weak(_trigger_read_sensors, 62), be_const_closure(Matter_Device__trigger_read_sensors_closure) },
        { be_const_key_weak(register_plugin_class, -1), be_const_closure(Matter_Device_register_plugin_class_closure) },
        { be_const_key_weak(root_passcode, 12), be_const_var(26) },
        { be_const_key_weak(every_250ms, 19), be_const_closure(Matter_Device_every_250ms_closure) },
        { be_const_key_weak(commissioning_instance_eth, -1), be_const_var(18) },
        { be_const_key_weak(vendorid, -1), be_const_var(21) },
        { be_const_key_weak(register_native_classes, 89), be_const_closure(Matter_Device_register_native_classes_closure) },
        { be_const_key_weak(start_operational_discovery, 15), be_const_closure(Matter_Device_start_operational_discovery_closure) },
        { be_const_key_weak(_init_basic_commissioning, -1), be_const_closure(Matter_Device__init_basic_commissioning_closure) },
        { be_const_key_weak(compute_qrcode_content, -1), be_const_closure(Matter_Device_compute_qrcode_content_closure) },
        { be_const_key_weak(is_root_commissioning_open, -1), be_const_closure(Matter_Device_is_root_commissioning_open_closure) },
        { be_const_key_weak(commissioning_open, -1), be_const_var(10) },
        { be_const_key_weak(commissioning_instance_wifi, -1), be_const_var(17) },
        { be_const_key_weak(msg_received, -1), be_const_closure(Matter_Device_msg_received_closure) },
        { be_const_key_weak(get_active_endpoints, -1), be_const_closure(Matter_Device_get_active_endpoints_closure) },
        { be_const_key_weak(MtrJoin, -1), be_const_closure(Matter_Device_MtrJoin_closure) },
        { be_const_key_weak(productid, 73), be_const_var(22) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(Matter_Device_invoke_request_closure) },
        { be_const_key_weak(get_plugin_class_displayname, 59), be_const_closure(Matter_Device_get_plugin_class_displayname_closure) },
        { be_const_key_weak(event_fabrics_saved, -1), be_const_closure(Matter_Device_event_fabrics_saved_closure) },
        { be_const_key_weak(start, 86), be_const_closure(Matter_Device_start_closure) },
        { be_const_key_weak(k2l_num, -1), be_const_static_closure(Matter_Device_k2l_num_closure) },
        { be_const_key_weak(mdns_announce_op_discovery, -1), be_const_closure(Matter_Device_mdns_announce_op_discovery_closure) },
        { be_const_key_weak(root_salt, -1), be_const_var(29) },
        { be_const_key_weak(every_second, 78), be_const_closure(Matter_Device_every_second_closure) },
        { be_const_key_weak(commissioning_L, -1), be_const_var(15) },
        { be_const_key_weak(hostname_wifi, 18), be_const_var(19) },
        { be_const_key_weak(get_plugin_class_arg, 61), be_const_closure(Matter_Device_get_plugin_class_arg_closure) },
        { be_const_key_weak(started, 84), be_const_var(0) },
        { be_const_key_weak(root_w0, 25), be_const_var(30) },
        { be_const_key_weak(mdns_announce_PASE, -1), be_const_closure(Matter_Device_mdns_announce_PASE_closure) },
        { be_const_key_weak(autoconf_device_map, 82), be_const_closure(Matter_Device_autoconf_device_map_closure) },
        { be_const_key_weak(every_50ms, -1), be_const_closure(Matter_Device_every_50ms_closure) },
        { be_const_key_weak(commissioning_admin_fabric, -1), be_const_var(16) },
        { be_const_key_weak(msg_send, 30), be_const_closure(Matter_Device_msg_send_closure) },
        { be_const_key_weak(commissioning_salt, -1), be_const_var(13) },
        { be_const_key_weak(plugins, -1), be_const_var(1) },
        { be_const_key_weak(PBKDF_ITERATIONS, -1), be_const_int(1000) },
        { be_const_key_weak(_mdns_announce_hostname, 50), be_const_closure(Matter_Device__mdns_announce_hostname_closure) },
        { be_const_key_weak(start_mdns_announce_hostnames, -1), be_const_closure(Matter_Device_start_mdns_announce_hostnames_closure) },
        { be_const_key_weak(attribute_updated, 49), be_const_closure(Matter_Device_attribute_updated_closure) },
        { be_const_key_weak(PRODUCT_ID, -1), be_const_int(32768) },
        { be_const_key_weak(root_L, -1), be_const_var(31) },
        { be_const_key_weak(register_commands, 77), be_const_closure(Matter_Device_register_commands_closure) },
        { be_const_key_weak(plugins_persist, -1), be_const_var(2) },
        { be_const_key_weak(init, 57), be_const_closure(Matter_Device_init_closure) },
        { be_const_key_weak(_compute_pbkdf, -1), be_const_closure(Matter_Device__compute_pbkdf_closure) },
        { be_const_key_weak(hostname_eth, -1), be_const_var(20) },
        { be_const_key_weak(received_ack, -1), be_const_closure(Matter_Device_received_ack_closure) },
        { be_const_key_weak(VENDOR_ID, -1), be_const_int(65521) },
        { be_const_key_weak(FILENAME, -1), be_nested_str_weak(_matter_device_X2Ejson) },
        { be_const_key_weak(load_param, -1), be_const_closure(Matter_Device_load_param_closure) },
        { be_const_key_weak(mdns_announce_op_discovery_all_fabrics, -1), be_const_closure(Matter_Device_mdns_announce_op_discovery_all_fabrics_closure) },
        { be_const_key_weak(tick, 70), be_const_var(9) },
        { be_const_key_weak(udp_server, 67), be_const_var(5) },
        { be_const_key_weak(start_basic_commissioning, -1), be_const_closure(Matter_Device_start_basic_commissioning_closure) },
        { be_const_key_weak(k2l, -1), be_const_static_closure(Matter_Device_k2l_closure) },
        { be_const_key_weak(PASSCODE_INVALID, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(12,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(11111111),
        be_const_int(22222222),
        be_const_int(33333333),
        be_const_int(44444444),
        be_const_int(55555555),
        be_const_int(66666666),
        be_const_int(77777777),
        be_const_int(88888888),
        be_const_int(99999999),
        be_const_int(12345678),
        be_const_int(87654321),
    }))    ) } )) },
        { be_const_key_weak(start_root_basic_commissioning, -1), be_const_closure(Matter_Device_start_root_basic_commissioning_closure) },
        { be_const_key_weak(autoconf_device, -1), be_const_closure(Matter_Device_autoconf_device_closure) },
        { be_const_key_weak(save_before_restart, 56), be_const_closure(Matter_Device_save_before_restart_closure) },
        { be_const_key_weak(process_attribute_expansion, -1), be_const_closure(Matter_Device_process_attribute_expansion_closure) },
        { be_const_key_weak(_load_plugins_config, -1), be_const_closure(Matter_Device__load_plugins_config_closure) },
        { be_const_key_weak(mdns_pase_eth, -1), be_const_var(23) },
        { be_const_key_weak(PASE_TIMEOUT, 33), be_const_int(600) },
        { be_const_key_weak(is_commissioning_open, 31), be_const_closure(Matter_Device_is_commissioning_open_closure) },
        { be_const_key_weak(mdns_remove_op_discovery_all_fabrics, -1), be_const_closure(Matter_Device_mdns_remove_op_discovery_all_fabrics_closure) },
        { be_const_key_weak(mdns_remove_op_discovery, -1), be_const_closure(Matter_Device_mdns_remove_op_discovery_closure) },
        { be_const_key_weak(ui, -1), be_const_var(8) },
        { be_const_key_weak(sessions, -1), be_const_var(7) },
        { be_const_key_weak(plugins_classes, -1), be_const_var(3) },
        { be_const_key_weak(sort_distinct, 47), be_const_static_closure(Matter_Device_sort_distinct_closure) },
        { be_const_key_weak(commissioning_iterations, 16), be_const_var(11) },
        { be_const_key_weak(UDP_PORT, -1), be_const_int(5540) },
        { be_const_key_weak(stop_basic_commissioning, -1), be_const_closure(Matter_Device_stop_basic_commissioning_closure) },
        { be_const_key_weak(start_commissioning_complete_deferred, 9), be_const_closure(Matter_Device_start_commissioning_complete_deferred_closure) },
        { be_const_key_weak(ipv4only, 3), be_const_var(27) },
        { be_const_key_weak(start_operational_discovery_deferred, 0), be_const_closure(Matter_Device_start_operational_discovery_deferred_closure) },
    })),
    be_str_weak(Matter_Device)
);
/*******************************************************************/

void be_load_Matter_Device_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Device);
    be_setglobal(vm, "Matter_Device");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
