/********************************************************************
 * Berry module `webserver`
 * 
 * To use: `import webserver`
 * 
 * Allows to respond to HTTP request
 *******************************************************************/
#include "be_constobj.h"

/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(Partition_otadata_tostring,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str(string),
    /* K1   */  be_nested_str(format),
    /* K2   */  be_nested_str(_X3Cinstance_X3A_X20Partition_otadata_X28ota_active_X3A_X25d_X2C_X20ota_seq_X3D_X5B_X25d_X2C_X25d_X5D_X2C_X20ota_max_X3D_X25d_X29_X3E),
    /* K3   */  be_nested_str(active_otadata),
    /* K4   */  be_nested_str(seq0),
    /* K5   */  be_nested_str(seq1),
    /* K6   */  be_nested_str(maxota),
    }),
    &be_const_str_tostring,
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x58100002,  //  0002  LDCONST	R4	K2
      0x88140103,  //  0003  GETMBR	R5	R0	K3
      0x88180104,  //  0004  GETMBR	R6	R0	K4
      0x881C0105,  //  0005  GETMBR	R7	R0	K5
      0x88200106,  //  0006  GETMBR	R8	R0	K6
      0x7C080C00,  //  0007  CALL	R2	6
      0x80040400,  //  0008  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: load
********************************************************************/
be_local_closure(Partition_otadata_load,   /* name */
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
    /* K0   */  be_nested_str(flash),
    /* K1   */  be_nested_str(read),
    /* K2   */  be_nested_str(seq0),
    /* K3   */  be_nested_str(get),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str(seq1),
    /* K6   */  be_nested_str(crc32_ota_seq),
    /* K7   */  be_nested_str(_validate),
    }),
    &be_const_str_load,
    &be_const_str_solidified,
    ( &(const binstruction[46]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x5412DFFF,  //  0002  LDINT	R4	57344
      0x5416001F,  //  0003  LDINT	R5	32
      0x7C080600,  //  0004  CALL	R2	3
      0x8C0C0301,  //  0005  GETMET	R3	R1	K1
      0x5416EFFF,  //  0006  LDINT	R5	61440
      0x541A001F,  //  0007  LDINT	R6	32
      0x7C0C0600,  //  0008  CALL	R3	3
      0x8C100503,  //  0009  GETMET	R4	R2	K3
      0x58180004,  //  000A  LDCONST	R6	K4
      0x541E0003,  //  000B  LDINT	R7	4
      0x7C100600,  //  000C  CALL	R4	3
      0x90020404,  //  000D  SETMBR	R0	K2	R4
      0x8C100703,  //  000E  GETMET	R4	R3	K3
      0x58180004,  //  000F  LDCONST	R6	K4
      0x541E0003,  //  0010  LDINT	R7	4
      0x7C100600,  //  0011  CALL	R4	3
      0x90020A04,  //  0012  SETMBR	R0	K5	R4
      0x8C100503,  //  0013  GETMET	R4	R2	K3
      0x541A001B,  //  0014  LDINT	R6	28
      0x541E0003,  //  0015  LDINT	R7	4
      0x7C100600,  //  0016  CALL	R4	3
      0x8C140106,  //  0017  GETMET	R5	R0	K6
      0x881C0102,  //  0018  GETMBR	R7	R0	K2
      0x7C140400,  //  0019  CALL	R5	2
      0x1C100805,  //  001A  EQ	R4	R4	R5
      0x8C140703,  //  001B  GETMET	R5	R3	K3
      0x541E001B,  //  001C  LDINT	R7	28
      0x54220003,  //  001D  LDINT	R8	4
      0x7C140600,  //  001E  CALL	R5	3
      0x8C180106,  //  001F  GETMET	R6	R0	K6
      0x88200105,  //  0020  GETMBR	R8	R0	K5
      0x7C180400,  //  0021  CALL	R6	2
      0x1C140A06,  //  0022  EQ	R5	R5	R6
      0x5C180800,  //  0023  MOVE	R6	R4
      0x741A0001,  //  0024  JMPT	R6	#0027
      0x4C180000,  //  0025  LDNIL	R6
      0x90020406,  //  0026  SETMBR	R0	K2	R6
      0x5C180A00,  //  0027  MOVE	R6	R5
      0x741A0001,  //  0028  JMPT	R6	#002B
      0x4C180000,  //  0029  LDNIL	R6
      0x90020A06,  //  002A  SETMBR	R0	K5	R6
      0x8C180107,  //  002B  GETMET	R6	R0	K7
      0x7C180200,  //  002C  CALL	R6	1
      0x80000000,  //  002D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: save
********************************************************************/
be_local_closure(Partition_otadata_save,   /* name */
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
    /* K0   */  be_nested_str(flash),
    /* K1   */  be_nested_str(seq0),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str(seq1),
    /* K4   */  be_const_int(1),
    /* K5   */  be_nested_str(offset),
    /* K6   */  be_nested_str(add),
    /* K7   */  be_nested_str(FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF),
    /* K8   */  be_nested_str(crc32_ota_seq),
    /* K9   */  be_nested_str(erase),
    /* K10  */  be_nested_str(write),
    }),
    &be_const_str_save,
    &be_const_str_solidified,
    ( &(const binstruction[53]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x5409FFFE,  //  0001  LDINT	R2	-1
      0x540DFFFE,  //  0002  LDINT	R3	-1
      0x88100101,  //  0003  GETMBR	R4	R0	K1
      0x4C140000,  //  0004  LDNIL	R5
      0x20100805,  //  0005  NE	R4	R4	R5
      0x78120001,  //  0006  JMPF	R4	#0009
      0x880C0101,  //  0007  GETMBR	R3	R0	K1
      0x58080002,  //  0008  LDCONST	R2	K2
      0x88100103,  //  0009  GETMBR	R4	R0	K3
      0x4C140000,  //  000A  LDNIL	R5
      0x20100805,  //  000B  NE	R4	R4	R5
      0x78120004,  //  000C  JMPF	R4	#0012
      0x88100103,  //  000D  GETMBR	R4	R0	K3
      0x24100803,  //  000E  GT	R4	R4	R3
      0x78120001,  //  000F  JMPF	R4	#0012
      0x880C0103,  //  0010  GETMBR	R3	R0	K3
      0x58080004,  //  0011  LDCONST	R2	K4
      0x14100502,  //  0012  LT	R4	R2	K2
      0x78120000,  //  0013  JMPF	R4	#0015
      0x58080002,  //  0014  LDCONST	R2	K2
      0x14100702,  //  0015  LT	R4	R3	K2
      0x78120000,  //  0016  JMPF	R4	#0018
      0x580C0004,  //  0017  LDCONST	R3	K4
      0x88100105,  //  0018  GETMBR	R4	R0	K5
      0x54160FFF,  //  0019  LDINT	R5	4096
      0x08140A02,  //  001A  MUL	R5	R5	R2
      0x00100805,  //  001B  ADD	R4	R4	R5
      0x60140015,  //  001C  GETGBL	R5	G21
      0x7C140000,  //  001D  CALL	R5	0
      0x8C180B06,  //  001E  GETMET	R6	R5	K6
      0x5C200600,  //  001F  MOVE	R8	R3
      0x54260003,  //  0020  LDINT	R9	4
      0x7C180600,  //  0021  CALL	R6	3
      0x60180015,  //  0022  GETGBL	R6	G21
      0x581C0007,  //  0023  LDCONST	R7	K7
      0x7C180200,  //  0024  CALL	R6	1
      0x00140A06,  //  0025  ADD	R5	R5	R6
      0x8C180B06,  //  0026  GETMET	R6	R5	K6
      0x8C200108,  //  0027  GETMET	R8	R0	K8
      0x5C280600,  //  0028  MOVE	R10	R3
      0x7C200400,  //  0029  CALL	R8	2
      0x54260003,  //  002A  LDINT	R9	4
      0x7C180600,  //  002B  CALL	R6	3
      0x8C180309,  //  002C  GETMET	R6	R1	K9
      0x5C200800,  //  002D  MOVE	R8	R4
      0x54260FFF,  //  002E  LDINT	R9	4096
      0x7C180600,  //  002F  CALL	R6	3
      0x8C18030A,  //  0030  GETMET	R6	R1	K10
      0x5C200800,  //  0031  MOVE	R8	R4
      0x5C240A00,  //  0032  MOVE	R9	R5
      0x7C180600,  //  0033  CALL	R6	3
      0x80000000,  //  0034  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Partition_otadata_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str(maxota),
    /* K1   */  be_const_int(1),
    /* K2   */  be_nested_str(offset),
    /* K3   */  be_nested_str(active_otadata),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str(load),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x880C0100,  //  0001  GETMBR	R3	R0	K0
      0x4C100000,  //  0002  LDNIL	R4
      0x1C0C0604,  //  0003  EQ	R3	R3	R4
      0x780E0000,  //  0004  JMPF	R3	#0006
      0x90020101,  //  0005  SETMBR	R0	K0	K1
      0x90020402,  //  0006  SETMBR	R0	K2	R2
      0x880C0102,  //  0007  GETMBR	R3	R0	K2
      0x4C100000,  //  0008  LDNIL	R4
      0x1C0C0604,  //  0009  EQ	R3	R3	R4
      0x780E0001,  //  000A  JMPF	R3	#000D
      0x540EDFFF,  //  000B  LDINT	R3	57344
      0x90020403,  //  000C  SETMBR	R0	K2	R3
      0x90020704,  //  000D  SETMBR	R0	K3	K4
      0x8C0C0105,  //  000E  GETMET	R3	R0	K5
      0x7C0C0200,  //  000F  CALL	R3	1
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_active
********************************************************************/
be_local_closure(Partition_otadata_set_active,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str(seq0),
    /* K2   */  be_nested_str(seq1),
    /* K3   */  be_const_int(1),
    /* K4   */  be_nested_str(maxota),
    /* K5   */  be_nested_str(_validate),
    }),
    &be_const_str_set_active,
    &be_const_str_solidified,
    ( &(const binstruction[41]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0x580C0000,  //  0001  LDCONST	R3	K0
      0x88100101,  //  0002  GETMBR	R4	R0	K1
      0x4C140000,  //  0003  LDNIL	R5
      0x20100805,  //  0004  NE	R4	R4	R5
      0x78120001,  //  0005  JMPF	R4	#0008
      0x88080101,  //  0006  GETMBR	R2	R0	K1
      0x580C0000,  //  0007  LDCONST	R3	K0
      0x88100102,  //  0008  GETMBR	R4	R0	K2
      0x4C140000,  //  0009  LDNIL	R5
      0x20100805,  //  000A  NE	R4	R4	R5
      0x78120004,  //  000B  JMPF	R4	#0011
      0x88100102,  //  000C  GETMBR	R4	R0	K2
      0x24100802,  //  000D  GT	R4	R4	R2
      0x78120001,  //  000E  JMPF	R4	#0011
      0x88080102,  //  000F  GETMBR	R2	R0	K2
      0x580C0003,  //  0010  LDCONST	R3	K3
      0x04100503,  //  0011  SUB	R4	R2	K3
      0x88140104,  //  0012  GETMBR	R5	R0	K4
      0x00140B03,  //  0013  ADD	R5	R5	K3
      0x10100805,  //  0014  MOD	R4	R4	R5
      0x20140801,  //  0015  NE	R5	R4	R1
      0x78160010,  //  0016  JMPF	R5	#0028
      0x24140204,  //  0017  GT	R5	R1	R4
      0x78160002,  //  0018  JMPF	R5	#001C
      0x04140204,  //  0019  SUB	R5	R1	R4
      0x00080405,  //  001A  ADD	R2	R2	R5
      0x70020004,  //  001B  JMP		#0021
      0x88140104,  //  001C  GETMBR	R5	R0	K4
      0x00140B03,  //  001D  ADD	R5	R5	K3
      0x04140A04,  //  001E  SUB	R5	R5	R4
      0x00140A01,  //  001F  ADD	R5	R5	R1
      0x00080405,  //  0020  ADD	R2	R2	R5
      0x1C140703,  //  0021  EQ	R5	R3	K3
      0x78160001,  //  0022  JMPF	R5	#0025
      0x90020202,  //  0023  SETMBR	R0	K1	R2
      0x70020000,  //  0024  JMP		#0026
      0x90020402,  //  0025  SETMBR	R0	K2	R2
      0x8C140105,  //  0026  GETMET	R5	R0	K5
      0x7C140200,  //  0027  CALL	R5	1
      0x80000000,  //  0028  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_ota_max
********************************************************************/
be_local_closure(Partition_otadata_set_ota_max,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str(maxota),
    }),
    &be_const_str_set_ota_max,
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: crc32_ota_seq
********************************************************************/
be_local_closure(Partition_otadata_crc32_ota_seq,   /* name */
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
    /* K0   */  be_nested_str(crc),
    /* K1   */  be_nested_str(crc32),
    /* K2   */  be_nested_str(add),
    }),
    &be_const_str_crc32_ota_seq,
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x5411FFFE,  //  0002  LDINT	R4	-1
      0x60140015,  //  0003  GETGBL	R5	G21
      0x7C140000,  //  0004  CALL	R5	0
      0x8C140B02,  //  0005  GETMET	R5	R5	K2
      0x5C1C0000,  //  0006  MOVE	R7	R0
      0x54220003,  //  0007  LDINT	R8	4
      0x7C140600,  //  0008  CALL	R5	3
      0x7C080600,  //  0009  CALL	R2	3
      0x80040400,  //  000A  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _validate
********************************************************************/
be_local_closure(Partition_otadata__validate,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str(active_otadata),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str(seq0),
    /* K3   */  be_const_int(1),
    /* K4   */  be_nested_str(maxota),
    /* K5   */  be_nested_str(seq1),
    }),
    &be_const_str__validate,
    &be_const_str_solidified,
    ( &(const binstruction[30]) {  /* code */
      0x90020101,  //  0000  SETMBR	R0	K0	K1
      0x88040102,  //  0001  GETMBR	R1	R0	K2
      0x4C080000,  //  0002  LDNIL	R2
      0x20040202,  //  0003  NE	R1	R1	R2
      0x78060005,  //  0004  JMPF	R1	#000B
      0x88040102,  //  0005  GETMBR	R1	R0	K2
      0x04040303,  //  0006  SUB	R1	R1	K3
      0x88080104,  //  0007  GETMBR	R2	R0	K4
      0x00080503,  //  0008  ADD	R2	R2	K3
      0x10040202,  //  0009  MOD	R1	R1	R2
      0x90020001,  //  000A  SETMBR	R0	K0	R1
      0x88040105,  //  000B  GETMBR	R1	R0	K5
      0x4C080000,  //  000C  LDNIL	R2
      0x20040202,  //  000D  NE	R1	R1	R2
      0x7806000D,  //  000E  JMPF	R1	#001D
      0x88040102,  //  000F  GETMBR	R1	R0	K2
      0x4C080000,  //  0010  LDNIL	R2
      0x1C040202,  //  0011  EQ	R1	R1	R2
      0x74060003,  //  0012  JMPT	R1	#0017
      0x88040105,  //  0013  GETMBR	R1	R0	K5
      0x88080102,  //  0014  GETMBR	R2	R0	K2
      0x24040202,  //  0015  GT	R1	R1	R2
      0x78060005,  //  0016  JMPF	R1	#001D
      0x88040105,  //  0017  GETMBR	R1	R0	K5
      0x04040303,  //  0018  SUB	R1	R1	K3
      0x88080104,  //  0019  GETMBR	R2	R0	K4
      0x00080503,  //  001A  ADD	R2	R2	K3
      0x10040202,  //  001B  MOD	R1	R1	R2
      0x90020001,  //  001C  SETMBR	R0	K0	R1
      0x80000000,  //  001D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Partition_otadata
********************************************************************/
be_local_class(Partition_otadata,
    5,
    NULL,
    be_nested_map(13,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(tostring, 1), be_const_closure(Partition_otadata_tostring_closure) },
        { be_const_key(_validate, -1), be_const_closure(Partition_otadata__validate_closure) },
        { be_const_key(crc32_ota_seq, 12), be_const_static_closure(Partition_otadata_crc32_ota_seq_closure) },
        { be_const_key(active_otadata, -1), be_const_var(2) },
        { be_const_key(save, 8), be_const_closure(Partition_otadata_save_closure) },
        { be_const_key(seq1, -1), be_const_var(4) },
        { be_const_key(init, 7), be_const_closure(Partition_otadata_init_closure) },
        { be_const_key(set_ota_max, -1), be_const_closure(Partition_otadata_set_ota_max_closure) },
        { be_const_key(seq0, 10), be_const_var(3) },
        { be_const_key(set_active, -1), be_const_closure(Partition_otadata_set_active_closure) },
        { be_const_key(offset, -1), be_const_var(1) },
        { be_const_key(load, 2), be_const_closure(Partition_otadata_load_closure) },
        { be_const_key(maxota, -1), be_const_var(0) },
    })),
    (bstring*) &be_const_str_Partition_otadata
);

/********************************************************************
** Solidified function: load_otadata
********************************************************************/
be_local_closure(Partition_load_otadata,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str(ota_max),
    /* K1   */  be_nested_str(slots),
    /* K2   */  be_nested_str(type),
    /* K3   */  be_const_int(1),
    /* K4   */  be_nested_str(subtype),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str(start),
    /* K7   */  be_nested_str(stop_iteration),
    /* K8   */  be_nested_str(otadata),
    /* K9   */  be_nested_str(partition_core),
    /* K10  */  be_nested_str(Partition_otadata),
    }),
    &be_const_str_load_otadata,
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x5406DFFF,  //  0000  LDINT	R1	57344
      0x8C080100,  //  0001  GETMET	R2	R0	K0
      0x7C080200,  //  0002  CALL	R2	1
      0x600C0010,  //  0003  GETGBL	R3	G16
      0x88100101,  //  0004  GETMBR	R4	R0	K1
      0x7C0C0200,  //  0005  CALL	R3	1
      0xA8020009,  //  0006  EXBLK	0	#0011
      0x5C100600,  //  0007  MOVE	R4	R3
      0x7C100000,  //  0008  CALL	R4	0
      0x88140902,  //  0009  GETMBR	R5	R4	K2
      0x1C140B03,  //  000A  EQ	R5	R5	K3
      0x78160003,  //  000B  JMPF	R5	#0010
      0x88140904,  //  000C  GETMBR	R5	R4	K4
      0x1C140B05,  //  000D  EQ	R5	R5	K5
      0x78160000,  //  000E  JMPF	R5	#0010
      0x88040906,  //  000F  GETMBR	R1	R4	K6
      0x7001FFF5,  //  0010  JMP		#0007
      0x580C0007,  //  0011  LDCONST	R3	K7
      0xAC0C0200,  //  0012  CATCH	R3	1	0
      0xB0080000,  //  0013  RAISE	2	R0	R0
      0xB80E1200,  //  0014  GETNGBL	R3	K9
      0x8C0C070A,  //  0015  GETMET	R3	R3	K10
      0x5C140400,  //  0016  MOVE	R5	R2
      0x5C180200,  //  0017  MOVE	R6	R1
      0x7C0C0600,  //  0018  CALL	R3	3
      0x90021003,  //  0019  SETMBR	R0	K8	R3
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tobytes
********************************************************************/
be_local_closure(Partition_tobytes,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str(slots),
    /* K1   */  be_nested_str(value_error),
    /* K2   */  be_nested_str(Too_X20many_X20partiition_X20slots),
    /* K3   */  be_nested_str(tobytes),
    /* K4   */  be_nested_str(stop_iteration),
    /* K5   */  be_nested_str(MD5),
    /* K6   */  be_nested_str(update),
    /* K7   */  be_nested_str(EBEBFFFFFFFFFFFFFFFFFFFFFFFFFFFF),
    /* K8   */  be_nested_str(finish),
    }),
    &be_const_str_tobytes,
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0x6004000C,  //  0000  GETGBL	R1	G12
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x7C040200,  //  0002  CALL	R1	1
      0x540A005E,  //  0003  LDINT	R2	95
      0x24040202,  //  0004  GT	R1	R1	R2
      0x78060000,  //  0005  JMPF	R1	#0007
      0xB0060302,  //  0006  RAISE	1	K1	K2
      0x60040015,  //  0007  GETGBL	R1	G21
      0x7C040000,  //  0008  CALL	R1	0
      0x60080010,  //  0009  GETGBL	R2	G16
      0x880C0100,  //  000A  GETMBR	R3	R0	K0
      0x7C080200,  //  000B  CALL	R2	1
      0xA8020005,  //  000C  EXBLK	0	#0013
      0x5C0C0400,  //  000D  MOVE	R3	R2
      0x7C0C0000,  //  000E  CALL	R3	0
      0x8C100703,  //  000F  GETMET	R4	R3	K3
      0x7C100200,  //  0010  CALL	R4	1
      0x00040204,  //  0011  ADD	R1	R1	R4
      0x7001FFF9,  //  0012  JMP		#000D
      0x58080004,  //  0013  LDCONST	R2	K4
      0xAC080200,  //  0014  CATCH	R2	1	0
      0xB0080000,  //  0015  RAISE	2	R0	R0
      0xB80A0A00,  //  0016  GETNGBL	R2	K5
      0x7C080000,  //  0017  CALL	R2	0
      0x8C0C0506,  //  0018  GETMET	R3	R2	K6
      0x5C140200,  //  0019  MOVE	R5	R1
      0x7C0C0400,  //  001A  CALL	R3	2
      0x600C0015,  //  001B  GETGBL	R3	G21
      0x58100007,  //  001C  LDCONST	R4	K7
      0x7C0C0200,  //  001D  CALL	R3	1
      0x00040203,  //  001E  ADD	R1	R1	R3
      0x8C0C0508,  //  001F  GETMET	R3	R2	K8
      0x7C0C0200,  //  0020  CALL	R3	1
      0x00040203,  //  0021  ADD	R1	R1	R3
      0x80040200,  //  0022  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(Partition_tostring,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str(_X3Cinstance_X3A_X20Partition_X28_X5B_X0A),
    /* K1   */  be_nested_str(slots),
    /* K2   */  be_nested_str(_X20_X20),
    /* K3   */  be_nested_str(tostring),
    /* K4   */  be_nested_str(_X0A),
    /* K5   */  be_nested_str(stop_iteration),
    /* K6   */  be_nested_str(_X5D_X2C_X0A_X20_X20),
    /* K7   */  be_nested_str(otadata),
    /* K8   */  be_nested_str(_X0A_X29_X3E),
    }),
    &be_const_str_tostring,
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x60080010,  //  0001  GETGBL	R2	G16
      0x880C0101,  //  0002  GETMBR	R3	R0	K1
      0x7C080200,  //  0003  CALL	R2	1
      0xA8020007,  //  0004  EXBLK	0	#000D
      0x5C0C0400,  //  0005  MOVE	R3	R2
      0x7C0C0000,  //  0006  CALL	R3	0
      0x00040302,  //  0007  ADD	R1	R1	K2
      0x8C100703,  //  0008  GETMET	R4	R3	K3
      0x7C100200,  //  0009  CALL	R4	1
      0x00040204,  //  000A  ADD	R1	R1	R4
      0x00040304,  //  000B  ADD	R1	R1	K4
      0x7001FFF7,  //  000C  JMP		#0005
      0x58080005,  //  000D  LDCONST	R2	K5
      0xAC080200,  //  000E  CATCH	R2	1	0
      0xB0080000,  //  000F  RAISE	2	R0	R0
      0x00040306,  //  0010  ADD	R1	R1	K6
      0x88080107,  //  0011  GETMBR	R2	R0	K7
      0x8C080503,  //  0012  GETMET	R2	R2	K3
      0x7C080200,  //  0013  CALL	R2	1
      0x00040202,  //  0014  ADD	R1	R1	R2
      0x00040308,  //  0015  ADD	R1	R1	K8
      0x80040200,  //  0016  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_active
********************************************************************/
be_local_closure(Partition_get_active,   /* name */
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
    /* K0   */  be_nested_str(otadata),
    /* K1   */  be_nested_str(active_otadata),
    }),
    &be_const_str_get_active,
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
** Solidified function: init
********************************************************************/
be_local_closure(Partition_init,   /* name */
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
    /* K0   */  be_nested_str(slots),
    /* K1   */  be_nested_str(load),
    /* K2   */  be_nested_str(parse),
    /* K3   */  be_nested_str(load_otadata),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x60040012,  //  0000  GETGBL	R1	G18
      0x7C040000,  //  0001  CALL	R1	0
      0x90020001,  //  0002  SETMBR	R0	K0	R1
      0x8C040101,  //  0003  GETMET	R1	R0	K1
      0x7C040200,  //  0004  CALL	R1	1
      0x8C040102,  //  0005  GETMET	R1	R0	K2
      0x7C040200,  //  0006  CALL	R1	1
      0x8C040103,  //  0007  GETMET	R1	R0	K3
      0x7C040200,  //  0008  CALL	R1	1
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: ota_max
********************************************************************/
be_local_closure(Partition_ota_max,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str(slots),
    /* K2   */  be_nested_str(type),
    /* K3   */  be_nested_str(subtype),
    /* K4   */  be_nested_str(stop_iteration),
    }),
    &be_const_str_ota_max,
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x60080010,  //  0001  GETGBL	R2	G16
      0x880C0101,  //  0002  GETMBR	R3	R0	K1
      0x7C080200,  //  0003  CALL	R2	1
      0xA8020013,  //  0004  EXBLK	0	#0019
      0x5C0C0400,  //  0005  MOVE	R3	R2
      0x7C0C0000,  //  0006  CALL	R3	0
      0x88100702,  //  0007  GETMBR	R4	R3	K2
      0x1C100900,  //  0008  EQ	R4	R4	K0
      0x7812000D,  //  0009  JMPF	R4	#0018
      0x88100703,  //  000A  GETMBR	R4	R3	K3
      0x5416000F,  //  000B  LDINT	R5	16
      0x28100805,  //  000C  GE	R4	R4	R5
      0x78120009,  //  000D  JMPF	R4	#0018
      0x88100703,  //  000E  GETMBR	R4	R3	K3
      0x5416001F,  //  000F  LDINT	R5	32
      0x14100805,  //  0010  LT	R4	R4	R5
      0x78120005,  //  0011  JMPF	R4	#0018
      0x88100703,  //  0012  GETMBR	R4	R3	K3
      0x5416000F,  //  0013  LDINT	R5	16
      0x04100805,  //  0014  SUB	R4	R4	R5
      0x24140801,  //  0015  GT	R5	R4	R1
      0x78160000,  //  0016  JMPF	R5	#0018
      0x5C040800,  //  0017  MOVE	R1	R4
      0x7001FFEB,  //  0018  JMP		#0005
      0x58080004,  //  0019  LDCONST	R2	K4
      0xAC080200,  //  001A  CATCH	R2	1	0
      0xB0080000,  //  001B  RAISE	2	R0	R0
      0x80040200,  //  001C  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse
********************************************************************/
be_local_closure(Partition_parse,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str(raw),
    /* K2   */  be_const_int(1),
    /* K3   */  be_nested_str(get),
    /* K4   */  be_const_int(2),
    /* K5   */  be_nested_str(partition_core),
    /* K6   */  be_nested_str(Partition_info),
    /* K7   */  be_nested_str(slots),
    /* K8   */  be_nested_str(push),
    /* K9   */  be_nested_str(md5),
    /* K10  */  be_nested_str(stop_iteration),
    }),
    &be_const_str_parse,
    &be_const_str_solidified,
    ( &(const binstruction[57]) {  /* code */
      0x60040010,  //  0000  GETGBL	R1	G16
      0x540A005D,  //  0001  LDINT	R2	94
      0x400A0002,  //  0002  CONNECT	R2	K0	R2
      0x7C040200,  //  0003  CALL	R1	1
      0xA802002F,  //  0004  EXBLK	0	#0035
      0x5C080200,  //  0005  MOVE	R2	R1
      0x7C080000,  //  0006  CALL	R2	0
      0x540E001F,  //  0007  LDINT	R3	32
      0x080C0403,  //  0008  MUL	R3	R2	R3
      0x00100502,  //  0009  ADD	R4	R2	K2
      0x5416001F,  //  000A  LDINT	R5	32
      0x08100805,  //  000B  MUL	R4	R4	R5
      0x04100902,  //  000C  SUB	R4	R4	K2
      0x400C0604,  //  000D  CONNECT	R3	R3	R4
      0x88100101,  //  000E  GETMBR	R4	R0	K1
      0x940C0803,  //  000F  GETIDX	R3	R4	R3
      0x8C140703,  //  0010  GETMET	R5	R3	K3
      0x581C0000,  //  0011  LDCONST	R7	K0
      0x58200004,  //  0012  LDCONST	R8	K4
      0x7C140600,  //  0013  CALL	R5	3
      0x5C100A00,  //  0014  MOVE	R4	R5
      0x541650A9,  //  0015  LDINT	R5	20650
      0x1C140805,  //  0016  EQ	R5	R4	R5
      0x78160008,  //  0017  JMPF	R5	#0021
      0xB8160A00,  //  0018  GETNGBL	R5	K5
      0x8C140B06,  //  0019  GETMET	R5	R5	K6
      0x5C1C0600,  //  001A  MOVE	R7	R3
      0x7C140400,  //  001B  CALL	R5	2
      0x88180107,  //  001C  GETMBR	R6	R0	K7
      0x8C180D08,  //  001D  GETMET	R6	R6	K8
      0x5C200A00,  //  001E  MOVE	R8	R5
      0x7C180400,  //  001F  CALL	R6	2
      0x70020010,  //  0020  JMP		#0032
      0x5416EBEA,  //  0021  LDINT	R5	60395
      0x1C140805,  //  0022  EQ	R5	R4	R5
      0x7816000C,  //  0023  JMPF	R5	#0031
      0x5416001F,  //  0024  LDINT	R5	32
      0x08140405,  //  0025  MUL	R5	R2	R5
      0x541A000F,  //  0026  LDINT	R6	16
      0x00140A06,  //  0027  ADD	R5	R5	R6
      0x541A0020,  //  0028  LDINT	R6	33
      0x08180406,  //  0029  MUL	R6	R2	R6
      0x04180D02,  //  002A  SUB	R6	R6	K2
      0x40140A06,  //  002B  CONNECT	R5	R5	R6
      0x88180101,  //  002C  GETMBR	R6	R0	K1
      0x94140C05,  //  002D  GETIDX	R5	R6	R5
      0x90021205,  //  002E  SETMBR	R0	K9	R5
      0x70020002,  //  002F  JMP		#0033
      0x70020000,  //  0030  JMP		#0032
      0x70020000,  //  0031  JMP		#0033
      0x7001FFD1,  //  0032  JMP		#0005
      0xA8040001,  //  0033  EXBLK	1	1
      0x70020002,  //  0034  JMP		#0038
      0x5804000A,  //  0035  LDCONST	R1	K10
      0xAC040200,  //  0036  CATCH	R1	1	0
      0xB0080000,  //  0037  RAISE	2	R0	R0
      0x80000000,  //  0038  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invalidate_spiffs
********************************************************************/
be_local_closure(Partition_invalidate_spiffs,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str(flash),
    /* K1   */  be_nested_str(slots),
    /* K2   */  be_nested_str(is_spiffs),
    /* K3   */  be_nested_str(value_error),
    /* K4   */  be_nested_str(No_X20SPIFFS_X20partition_X20found),
    /* K5   */  be_nested_str(00),
    /* K6   */  be_nested_str(write),
    /* K7   */  be_nested_str(start),
    }),
    &be_const_str_invalidate_spiffs,
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x5409FFFE,  //  0001  LDINT	R2	-1
      0x880C0101,  //  0002  GETMBR	R3	R0	K1
      0x94080602,  //  0003  GETIDX	R2	R3	R2
      0x8C100502,  //  0004  GETMET	R4	R2	K2
      0x7C100200,  //  0005  CALL	R4	1
      0x74120000,  //  0006  JMPT	R4	#0008
      0xB0060704,  //  0007  RAISE	1	K3	K4
      0x600C0015,  //  0008  GETGBL	R3	G21
      0x58100005,  //  0009  LDCONST	R4	K5
      0x7C0C0200,  //  000A  CALL	R3	1
      0x8C100306,  //  000B  GETMET	R4	R1	K6
      0x88180507,  //  000C  GETMBR	R6	R2	K7
      0x5C1C0600,  //  000D  MOVE	R7	R3
      0x7C100600,  //  000E  CALL	R4	3
      0x8C100306,  //  000F  GETMET	R4	R1	K6
      0x88180507,  //  0010  GETMBR	R6	R2	K7
      0x541E0FFF,  //  0011  LDINT	R7	4096
      0x00180C07,  //  0012  ADD	R6	R6	R7
      0x5C1C0600,  //  0013  MOVE	R7	R3
      0x7C100600,  //  0014  CALL	R4	3
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: save
********************************************************************/
be_local_closure(Partition_save,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str(flash),
    /* K1   */  be_nested_str(tobytes),
    /* K2   */  be_nested_str(erase),
    /* K3   */  be_nested_str(write),
    /* K4   */  be_nested_str(otadata),
    /* K5   */  be_nested_str(save),
    }),
    &be_const_str_save,
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080101,  //  0001  GETMET	R2	R0	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x8C0C0302,  //  0003  GETMET	R3	R1	K2
      0x54167FFF,  //  0004  LDINT	R5	32768
      0x541A0FFF,  //  0005  LDINT	R6	4096
      0x7C0C0600,  //  0006  CALL	R3	3
      0x8C0C0303,  //  0007  GETMET	R3	R1	K3
      0x54167FFF,  //  0008  LDINT	R5	32768
      0x5C180400,  //  0009  MOVE	R6	R2
      0x7C0C0600,  //  000A  CALL	R3	3
      0x880C0104,  //  000B  GETMBR	R3	R0	K4
      0x8C0C0705,  //  000C  GETMET	R3	R3	K5
      0x7C0C0200,  //  000D  CALL	R3	1
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_ota_slot
********************************************************************/
be_local_closure(Partition_get_ota_slot,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(slots),
    /* K1   */  be_nested_str(is_ota),
    /* K2   */  be_nested_str(stop_iteration),
    }),
    &be_const_str_get_ota_slot,
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x60080010,  //  0000  GETGBL	R2	G16
      0x880C0100,  //  0001  GETMBR	R3	R0	K0
      0x7C080200,  //  0002  CALL	R2	1
      0xA8020008,  //  0003  EXBLK	0	#000D
      0x5C0C0400,  //  0004  MOVE	R3	R2
      0x7C0C0000,  //  0005  CALL	R3	0
      0x8C100701,  //  0006  GETMET	R4	R3	K1
      0x7C100200,  //  0007  CALL	R4	1
      0x1C100801,  //  0008  EQ	R4	R4	R1
      0x78120001,  //  0009  JMPF	R4	#000C
      0xA8040001,  //  000A  EXBLK	1	1
      0x80040600,  //  000B  RET	1	R3
      0x7001FFF6,  //  000C  JMP		#0004
      0x58080002,  //  000D  LDCONST	R2	K2
      0xAC080200,  //  000E  CATCH	R2	1	0
      0xB0080000,  //  000F  RAISE	2	R0	R0
      0x4C080000,  //  0010  LDNIL	R2
      0x80040400,  //  0011  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: switch_factory
********************************************************************/
be_local_closure(Partition_switch_factory,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(flash),
    /* K1   */  be_nested_str(rollback),
    }),
    &be_const_str_switch_factory,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: load
********************************************************************/
be_local_closure(Partition_load,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(flash),
    /* K1   */  be_nested_str(raw),
    /* K2   */  be_nested_str(read),
    }),
    &be_const_str_load,
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080302,  //  0001  GETMET	R2	R1	K2
      0x54127FFF,  //  0002  LDINT	R4	32768
      0x54160FFF,  //  0003  LDINT	R5	4096
      0x7C080600,  //  0004  CALL	R2	3
      0x90020202,  //  0005  SETMBR	R0	K1	R2
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_active
********************************************************************/
be_local_closure(Partition_set_active,   /* name */
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
    /* K1   */  be_nested_str(ota_max),
    /* K2   */  be_nested_str(value_error),
    /* K3   */  be_nested_str(Invalid_X20ota_X20partition_X20number),
    /* K4   */  be_nested_str(otadata),
    /* K5   */  be_nested_str(set_ota_max),
    /* K6   */  be_nested_str(set_active),
    }),
    &be_const_str_set_active,
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x14080300,  //  0000  LT	R2	R1	K0
      0x740A0003,  //  0001  JMPT	R2	#0006
      0x8C080101,  //  0002  GETMET	R2	R0	K1
      0x7C080200,  //  0003  CALL	R2	1
      0x24080202,  //  0004  GT	R2	R1	R2
      0x780A0000,  //  0005  JMPF	R2	#0007
      0xB0060503,  //  0006  RAISE	1	K2	K3
      0x88080104,  //  0007  GETMBR	R2	R0	K4
      0x8C080505,  //  0008  GETMET	R2	R2	K5
      0x8C100101,  //  0009  GETMET	R4	R0	K1
      0x7C100200,  //  000A  CALL	R4	1
      0x7C080400,  //  000B  CALL	R2	2
      0x88080104,  //  000C  GETMBR	R2	R0	K4
      0x8C080506,  //  000D  GETMET	R2	R2	K6
      0x5C100200,  //  000E  MOVE	R4	R1
      0x7C080400,  //  000F  CALL	R2	2
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Partition
********************************************************************/
be_local_class(Partition,
    4,
    NULL,
    be_nested_map(17,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(load_otadata, -1), be_const_closure(Partition_load_otadata_closure) },
        { be_const_key(set_active, -1), be_const_closure(Partition_set_active_closure) },
        { be_const_key(tostring, 12), be_const_closure(Partition_tostring_closure) },
        { be_const_key(raw, -1), be_const_var(0) },
        { be_const_key(get_active, 16), be_const_closure(Partition_get_active_closure) },
        { be_const_key(load, -1), be_const_closure(Partition_load_closure) },
        { be_const_key(parse, -1), be_const_closure(Partition_parse_closure) },
        { be_const_key(ota_max, 1), be_const_closure(Partition_ota_max_closure) },
        { be_const_key(slots, -1), be_const_var(2) },
        { be_const_key(save, -1), be_const_closure(Partition_save_closure) },
        { be_const_key(invalidate_spiffs, -1), be_const_closure(Partition_invalidate_spiffs_closure) },
        { be_const_key(tobytes, 9), be_const_closure(Partition_tobytes_closure) },
        { be_const_key(get_ota_slot, 6), be_const_closure(Partition_get_ota_slot_closure) },
        { be_const_key(switch_factory, -1), be_const_closure(Partition_switch_factory_closure) },
        { be_const_key(md5, -1), be_const_var(1) },
        { be_const_key(init, 5), be_const_closure(Partition_init_closure) },
        { be_const_key(otadata, -1), be_const_var(3) },
    })),
    (bstring*) &be_const_str_Partition
);

/********************************************************************
** Solidified function: remove_trailing_zeroes
********************************************************************/
be_local_closure(Partition_info_remove_trailing_zeroes,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_const_int(1),
    /* K2   */  be_nested_str(resize),
    }),
    &be_const_str_remove_trailing_zeroes,
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x6004000C,  //  0000  GETGBL	R1	G12
      0x5C080000,  //  0001  MOVE	R2	R0
      0x7C040200,  //  0002  CALL	R1	1
      0x58080000,  //  0003  LDCONST	R2	K0
      0x140C0401,  //  0004  LT	R3	R2	R1
      0x780E0007,  //  0005  JMPF	R3	#000E
      0x540DFFFE,  //  0006  LDINT	R3	-1
      0x040C0602,  //  0007  SUB	R3	R3	R2
      0x940C0003,  //  0008  GETIDX	R3	R0	R3
      0x200C0700,  //  0009  NE	R3	R3	K0
      0x780E0000,  //  000A  JMPF	R3	#000C
      0x70020001,  //  000B  JMP		#000E
      0x00080501,  //  000C  ADD	R2	R2	K1
      0x7001FFF5,  //  000D  JMP		#0004
      0x240C0500,  //  000E  GT	R3	R2	K0
      0x780E0005,  //  000F  JMPF	R3	#0016
      0x8C0C0102,  //  0010  GETMET	R3	R0	K2
      0x6014000C,  //  0011  GETGBL	R5	G12
      0x5C180000,  //  0012  MOVE	R6	R0
      0x7C140200,  //  0013  CALL	R5	1
      0x04140A02,  //  0014  SUB	R5	R5	R2
      0x7C0C0400,  //  0015  CALL	R3	2
      0x80040000,  //  0016  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(Partition_info_tostring,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[30]) {     /* constants */
    /* K0   */  be_nested_str(string),
    /* K1   */  be_nested_str(),
    /* K2   */  be_nested_str(type),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str(app),
    /* K5   */  be_nested_str(subtype),
    /* K6   */  be_nested_str(factory),
    /* K7   */  be_nested_str(ota),
    /* K8   */  be_nested_str(test),
    /* K9   */  be_const_int(1),
    /* K10  */  be_nested_str(data),
    /* K11  */  be_nested_str(otadata),
    /* K12  */  be_nested_str(phy),
    /* K13  */  be_const_int(2),
    /* K14  */  be_nested_str(nvs),
    /* K15  */  be_const_int(3),
    /* K16  */  be_nested_str(coredump),
    /* K17  */  be_nested_str(nvskeys),
    /* K18  */  be_nested_str(efuse_em),
    /* K19  */  be_nested_str(esphttpd),
    /* K20  */  be_nested_str(fat),
    /* K21  */  be_nested_str(spiffs),
    /* K22  */  be_nested_str(_X20_X28),
    /* K23  */  be_nested_str(_X29),
    /* K24  */  be_nested_str(format),
    /* K25  */  be_nested_str(_X3Cinstance_X3A_X20Partition_info_X28_X25d_X25s_X2C_X25d_X25s_X2C0x_X2508X_X2C0x_X2508X_X2C_X27_X25s_X27_X2C0x_X25X_X29_X3E),
    /* K26  */  be_nested_str(start),
    /* K27  */  be_nested_str(size),
    /* K28  */  be_nested_str(label),
    /* K29  */  be_nested_str(flags),
    }),
    &be_const_str_tostring,
    &be_const_str_solidified,
    ( &(const binstruction[109]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x58080001,  //  0001  LDCONST	R2	K1
      0x580C0001,  //  0002  LDCONST	R3	K1
      0x88100102,  //  0003  GETMBR	R4	R0	K2
      0x1C100903,  //  0004  EQ	R4	R4	K3
      0x7812001B,  //  0005  JMPF	R4	#0022
      0x58080004,  //  0006  LDCONST	R2	K4
      0x88100105,  //  0007  GETMBR	R4	R0	K5
      0x1C100903,  //  0008  EQ	R4	R4	K3
      0x78120001,  //  0009  JMPF	R4	#000C
      0x580C0006,  //  000A  LDCONST	R3	K6
      0x70020014,  //  000B  JMP		#0021
      0x88100105,  //  000C  GETMBR	R4	R0	K5
      0x5416000F,  //  000D  LDINT	R5	16
      0x28100805,  //  000E  GE	R4	R4	R5
      0x7812000B,  //  000F  JMPF	R4	#001C
      0x88100105,  //  0010  GETMBR	R4	R0	K5
      0x5416001F,  //  0011  LDINT	R5	32
      0x14100805,  //  0012  LT	R4	R4	R5
      0x78120007,  //  0013  JMPF	R4	#001C
      0x60100008,  //  0014  GETGBL	R4	G8
      0x88140105,  //  0015  GETMBR	R5	R0	K5
      0x541A000F,  //  0016  LDINT	R6	16
      0x04140A06,  //  0017  SUB	R5	R5	R6
      0x7C100200,  //  0018  CALL	R4	1
      0x00120E04,  //  0019  ADD	R4	K7	R4
      0x5C0C0800,  //  001A  MOVE	R3	R4
      0x70020004,  //  001B  JMP		#0021
      0x88100105,  //  001C  GETMBR	R4	R0	K5
      0x5416001F,  //  001D  LDINT	R5	32
      0x1C100805,  //  001E  EQ	R4	R4	R5
      0x78120000,  //  001F  JMPF	R4	#0021
      0x580C0008,  //  0020  LDCONST	R3	K8
      0x70020034,  //  0021  JMP		#0057
      0x88100102,  //  0022  GETMBR	R4	R0	K2
      0x1C100909,  //  0023  EQ	R4	R4	K9
      0x78120031,  //  0024  JMPF	R4	#0057
      0x5808000A,  //  0025  LDCONST	R2	K10
      0x88100105,  //  0026  GETMBR	R4	R0	K5
      0x1C100903,  //  0027  EQ	R4	R4	K3
      0x78120001,  //  0028  JMPF	R4	#002B
      0x580C000B,  //  0029  LDCONST	R3	K11
      0x7002002B,  //  002A  JMP		#0057
      0x88100105,  //  002B  GETMBR	R4	R0	K5
      0x1C100909,  //  002C  EQ	R4	R4	K9
      0x78120001,  //  002D  JMPF	R4	#0030
      0x580C000C,  //  002E  LDCONST	R3	K12
      0x70020026,  //  002F  JMP		#0057
      0x88100105,  //  0030  GETMBR	R4	R0	K5
      0x1C10090D,  //  0031  EQ	R4	R4	K13
      0x78120001,  //  0032  JMPF	R4	#0035
      0x580C000E,  //  0033  LDCONST	R3	K14
      0x70020021,  //  0034  JMP		#0057
      0x88100105,  //  0035  GETMBR	R4	R0	K5
      0x1C10090F,  //  0036  EQ	R4	R4	K15
      0x78120001,  //  0037  JMPF	R4	#003A
      0x580C0010,  //  0038  LDCONST	R3	K16
      0x7002001C,  //  0039  JMP		#0057
      0x88100105,  //  003A  GETMBR	R4	R0	K5
      0x54160003,  //  003B  LDINT	R5	4
      0x1C100805,  //  003C  EQ	R4	R4	R5
      0x78120001,  //  003D  JMPF	R4	#0040
      0x580C0011,  //  003E  LDCONST	R3	K17
      0x70020016,  //  003F  JMP		#0057
      0x88100105,  //  0040  GETMBR	R4	R0	K5
      0x54160004,  //  0041  LDINT	R5	5
      0x1C100805,  //  0042  EQ	R4	R4	R5
      0x78120001,  //  0043  JMPF	R4	#0046
      0x580C0012,  //  0044  LDCONST	R3	K18
      0x70020010,  //  0045  JMP		#0057
      0x88100105,  //  0046  GETMBR	R4	R0	K5
      0x5416007F,  //  0047  LDINT	R5	128
      0x1C100805,  //  0048  EQ	R4	R4	R5
      0x78120001,  //  0049  JMPF	R4	#004C
      0x580C0013,  //  004A  LDCONST	R3	K19
      0x7002000A,  //  004B  JMP		#0057
      0x88100105,  //  004C  GETMBR	R4	R0	K5
      0x54160080,  //  004D  LDINT	R5	129
      0x1C100805,  //  004E  EQ	R4	R4	R5
      0x78120001,  //  004F  JMPF	R4	#0052
      0x580C0014,  //  0050  LDCONST	R3	K20
      0x70020004,  //  0051  JMP		#0057
      0x88100105,  //  0052  GETMBR	R4	R0	K5
      0x54160081,  //  0053  LDINT	R5	130
      0x1C100805,  //  0054  EQ	R4	R4	R5
      0x78120000,  //  0055  JMPF	R4	#0057
      0x580C0015,  //  0056  LDCONST	R3	K21
      0x20100501,  //  0057  NE	R4	R2	K1
      0x78120002,  //  0058  JMPF	R4	#005C
      0x00122C02,  //  0059  ADD	R4	K22	R2
      0x00100917,  //  005A  ADD	R4	R4	K23
      0x5C080800,  //  005B  MOVE	R2	R4
      0x20100701,  //  005C  NE	R4	R3	K1
      0x78120002,  //  005D  JMPF	R4	#0061
      0x00122C03,  //  005E  ADD	R4	K22	R3
      0x00100917,  //  005F  ADD	R4	R4	K23
      0x5C0C0800,  //  0060  MOVE	R3	R4
      0x8C100318,  //  0061  GETMET	R4	R1	K24
      0x58180019,  //  0062  LDCONST	R6	K25
      0x881C0102,  //  0063  GETMBR	R7	R0	K2
      0x5C200400,  //  0064  MOVE	R8	R2
      0x88240105,  //  0065  GETMBR	R9	R0	K5
      0x5C280600,  //  0066  MOVE	R10	R3
      0x882C011A,  //  0067  GETMBR	R11	R0	K26
      0x8830011B,  //  0068  GETMBR	R12	R0	K27
      0x8834011C,  //  0069  GETMBR	R13	R0	K28
      0x8838011D,  //  006A  GETMBR	R14	R0	K29
      0x7C101400,  //  006B  CALL	R4	10
      0x80040800,  //  006C  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tobytes
********************************************************************/
be_local_closure(Partition_info_tobytes,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str(AA50),
    /* K1   */  be_nested_str(resize),
    /* K2   */  be_const_int(2),
    /* K3   */  be_nested_str(add),
    /* K4   */  be_nested_str(type),
    /* K5   */  be_const_int(1),
    /* K6   */  be_nested_str(subtype),
    /* K7   */  be_nested_str(start),
    /* K8   */  be_nested_str(size),
    /* K9   */  be_nested_str(fromstring),
    /* K10  */  be_nested_str(label),
    /* K11  */  be_nested_str(flags),
    }),
    &be_const_str_tobytes,
    &be_const_str_solidified,
    ( &(const binstruction[39]) {  /* code */
      0x60040015,  //  0000  GETGBL	R1	G21
      0x58080000,  //  0001  LDCONST	R2	K0
      0x7C040200,  //  0002  CALL	R1	1
      0x8C080301,  //  0003  GETMET	R2	R1	K1
      0x5412001F,  //  0004  LDINT	R4	32
      0x7C080400,  //  0005  CALL	R2	2
      0x8C080501,  //  0006  GETMET	R2	R2	K1
      0x58100002,  //  0007  LDCONST	R4	K2
      0x7C080400,  //  0008  CALL	R2	2
      0x8C080303,  //  0009  GETMET	R2	R1	K3
      0x88100104,  //  000A  GETMBR	R4	R0	K4
      0x58140005,  //  000B  LDCONST	R5	K5
      0x7C080600,  //  000C  CALL	R2	3
      0x8C080303,  //  000D  GETMET	R2	R1	K3
      0x88100106,  //  000E  GETMBR	R4	R0	K6
      0x58140005,  //  000F  LDCONST	R5	K5
      0x7C080600,  //  0010  CALL	R2	3
      0x8C080303,  //  0011  GETMET	R2	R1	K3
      0x88100107,  //  0012  GETMBR	R4	R0	K7
      0x54160003,  //  0013  LDINT	R5	4
      0x7C080600,  //  0014  CALL	R2	3
      0x8C080303,  //  0015  GETMET	R2	R1	K3
      0x88100108,  //  0016  GETMBR	R4	R0	K8
      0x54160003,  //  0017  LDINT	R5	4
      0x7C080600,  //  0018  CALL	R2	3
      0x60080015,  //  0019  GETGBL	R2	G21
      0x7C080000,  //  001A  CALL	R2	0
      0x8C080509,  //  001B  GETMET	R2	R2	K9
      0x8810010A,  //  001C  GETMBR	R4	R0	K10
      0x7C080400,  //  001D  CALL	R2	2
      0x8C0C0501,  //  001E  GETMET	R3	R2	K1
      0x5416000F,  //  001F  LDINT	R5	16
      0x7C0C0400,  //  0020  CALL	R3	2
      0x00040202,  //  0021  ADD	R1	R1	R2
      0x8C0C0303,  //  0022  GETMET	R3	R1	K3
      0x8814010B,  //  0023  GETMBR	R5	R0	K11
      0x541A0003,  //  0024  LDINT	R6	4
      0x7C0C0600,  //  0025  CALL	R3	3
      0x80040200,  //  0026  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_factory
********************************************************************/
be_local_closure(Partition_info_is_factory,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(type),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str(subtype),
    }),
    &be_const_str_is_factory,
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x1C040301,  //  0001  EQ	R1	R1	K1
      0x78060002,  //  0002  JMPF	R1	#0006
      0x88040102,  //  0003  GETMBR	R1	R0	K2
      0x1C040301,  //  0004  EQ	R1	R1	K1
      0x74060000,  //  0005  JMPT	R1	#0007
      0x50040001,  //  0006  LDBOOL	R1	0	1
      0x50040200,  //  0007  LDBOOL	R1	1	0
      0x80040200,  //  0008  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Partition_info_init,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_nested_str(type),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str(subtype),
    /* K3   */  be_nested_str(start),
    /* K4   */  be_nested_str(size),
    /* K5   */  be_nested_str(label),
    /* K6   */  be_nested_str(),
    /* K7   */  be_nested_str(flags),
    /* K8   */  be_nested_str(get),
    /* K9   */  be_const_int(2),
    /* K10  */  be_const_int(1),
    /* K11  */  be_const_int(3),
    /* K12  */  be_nested_str(remove_trailing_zeroes),
    /* K13  */  be_nested_str(asstring),
    /* K14  */  be_nested_str(string),
    /* K15  */  be_nested_str(format),
    /* K16  */  be_nested_str(invalid_X20magic_X20number_X20_X2502X),
    /* K17  */  be_nested_str(internal_error),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[61]) {  /* code */
      0x90020101,  //  0000  SETMBR	R0	K0	K1
      0x90020501,  //  0001  SETMBR	R0	K2	K1
      0x90020701,  //  0002  SETMBR	R0	K3	K1
      0x90020901,  //  0003  SETMBR	R0	K4	K1
      0x90020B06,  //  0004  SETMBR	R0	K5	K6
      0x90020F01,  //  0005  SETMBR	R0	K7	K1
      0x6008000E,  //  0006  GETGBL	R2	G14
      0x600C0015,  //  0007  GETGBL	R3	G21
      0x5C100200,  //  0008  MOVE	R4	R1
      0x7C080400,  //  0009  CALL	R2	2
      0x740A0000,  //  000A  JMPT	R2	#000C
      0x80000400,  //  000B  RET	0
      0x8C080308,  //  000C  GETMET	R2	R1	K8
      0x58100001,  //  000D  LDCONST	R4	K1
      0x58140009,  //  000E  LDCONST	R5	K9
      0x7C080600,  //  000F  CALL	R2	3
      0x540E50A9,  //  0010  LDINT	R3	20650
      0x1C0C0403,  //  0011  EQ	R3	R2	R3
      0x780E0022,  //  0012  JMPF	R3	#0036
      0x8C0C0308,  //  0013  GETMET	R3	R1	K8
      0x58140009,  //  0014  LDCONST	R5	K9
      0x5818000A,  //  0015  LDCONST	R6	K10
      0x7C0C0600,  //  0016  CALL	R3	3
      0x90020003,  //  0017  SETMBR	R0	K0	R3
      0x8C0C0308,  //  0018  GETMET	R3	R1	K8
      0x5814000B,  //  0019  LDCONST	R5	K11
      0x5818000A,  //  001A  LDCONST	R6	K10
      0x7C0C0600,  //  001B  CALL	R3	3
      0x90020403,  //  001C  SETMBR	R0	K2	R3
      0x8C0C0308,  //  001D  GETMET	R3	R1	K8
      0x54160003,  //  001E  LDINT	R5	4
      0x541A0003,  //  001F  LDINT	R6	4
      0x7C0C0600,  //  0020  CALL	R3	3
      0x90020603,  //  0021  SETMBR	R0	K3	R3
      0x8C0C0308,  //  0022  GETMET	R3	R1	K8
      0x54160007,  //  0023  LDINT	R5	8
      0x541A0003,  //  0024  LDINT	R6	4
      0x7C0C0600,  //  0025  CALL	R3	3
      0x90020803,  //  0026  SETMBR	R0	K4	R3
      0x8C0C010C,  //  0027  GETMET	R3	R0	K12
      0x5416000B,  //  0028  LDINT	R5	12
      0x541A001A,  //  0029  LDINT	R6	27
      0x40140A06,  //  002A  CONNECT	R5	R5	R6
      0x94140205,  //  002B  GETIDX	R5	R1	R5
      0x7C0C0400,  //  002C  CALL	R3	2
      0x8C0C070D,  //  002D  GETMET	R3	R3	K13
      0x7C0C0200,  //  002E  CALL	R3	1
      0x90020A03,  //  002F  SETMBR	R0	K5	R3
      0x8C0C0308,  //  0030  GETMET	R3	R1	K8
      0x5416001B,  //  0031  LDINT	R5	28
      0x541A0003,  //  0032  LDINT	R6	4
      0x7C0C0600,  //  0033  CALL	R3	3
      0x90020E03,  //  0034  SETMBR	R0	K7	R3
      0x70020005,  //  0035  JMP		#003C
      0xA40E1C00,  //  0036  IMPORT	R3	K14
      0x8C10070F,  //  0037  GETMET	R4	R3	K15
      0x58180010,  //  0038  LDCONST	R6	K16
      0x5C1C0400,  //  0039  MOVE	R7	R2
      0x7C100600,  //  003A  CALL	R4	3
      0xB0062204,  //  003B  RAISE	1	K17	R4
      0x80000000,  //  003C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_spiffs
********************************************************************/
be_local_closure(Partition_info_is_spiffs,   /* name */
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
    /* K0   */  be_nested_str(type),
    /* K1   */  be_const_int(1),
    /* K2   */  be_nested_str(subtype),
    }),
    &be_const_str_is_spiffs,
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x1C040301,  //  0001  EQ	R1	R1	K1
      0x78060003,  //  0002  JMPF	R1	#0007
      0x88040102,  //  0003  GETMBR	R1	R0	K2
      0x540A0081,  //  0004  LDINT	R2	130
      0x1C040202,  //  0005  EQ	R1	R1	R2
      0x74060000,  //  0006  JMPT	R1	#0008
      0x50040001,  //  0007  LDBOOL	R1	0	1
      0x50040200,  //  0008  LDBOOL	R1	1	0
      0x80040200,  //  0009  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_image_size
********************************************************************/
be_local_closure(Partition_info_get_image_size,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str(flash),
    /* K1   */  be_nested_str(is_ota),
    /* K2   */  be_nested_str(is_factory),
    /* K3   */  be_nested_str(start),
    /* K4   */  be_nested_str(read),
    /* K5   */  be_const_int(1),
    /* K6   */  be_nested_str(get),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str(tasmota),
    /* K9   */  be_nested_str(log),
    /* K10  */  be_nested_str(BRY_X3A_X20Exception_X3E_X20_X27),
    /* K11  */  be_nested_str(_X27_X20_X2D_X20),
    /* K12  */  be_const_int(2),
    }),
    &be_const_str_get_image_size,
    &be_const_str_solidified,
    ( &(const binstruction[78]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080101,  //  0001  GETMET	R2	R0	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x4C0C0000,  //  0003  LDNIL	R3
      0x1C080403,  //  0004  EQ	R2	R2	R3
      0x780A0004,  //  0005  JMPF	R2	#000B
      0x8C080102,  //  0006  GETMET	R2	R0	K2
      0x7C080200,  //  0007  CALL	R2	1
      0x740A0001,  //  0008  JMPT	R2	#000B
      0x5409FFFE,  //  0009  LDINT	R2	-1
      0x80040400,  //  000A  RET	1	R2
      0xA8020033,  //  000B  EXBLK	0	#0040
      0x88080103,  //  000C  GETMBR	R2	R0	K3
      0x8C0C0304,  //  000D  GETMET	R3	R1	K4
      0x5C140400,  //  000E  MOVE	R5	R2
      0x58180005,  //  000F  LDCONST	R6	K5
      0x7C0C0600,  //  0010  CALL	R3	3
      0x8C0C0706,  //  0011  GETMET	R3	R3	K6
      0x58140007,  //  0012  LDCONST	R5	K7
      0x58180005,  //  0013  LDCONST	R6	K5
      0x7C0C0600,  //  0014  CALL	R3	3
      0x541200E8,  //  0015  LDINT	R4	233
      0x20100604,  //  0016  NE	R4	R3	R4
      0x78120002,  //  0017  JMPF	R4	#001B
      0x5411FFFE,  //  0018  LDINT	R4	-1
      0xA8040001,  //  0019  EXBLK	1	1
      0x80040800,  //  001A  RET	1	R4
      0x8C100304,  //  001B  GETMET	R4	R1	K4
      0x00180505,  //  001C  ADD	R6	R2	K5
      0x581C0005,  //  001D  LDCONST	R7	K5
      0x7C100600,  //  001E  CALL	R4	3
      0x8C100906,  //  001F  GETMET	R4	R4	K6
      0x58180007,  //  0020  LDCONST	R6	K7
      0x581C0005,  //  0021  LDCONST	R7	K5
      0x7C100600,  //  0022  CALL	R4	3
      0x5416001F,  //  0023  LDINT	R5	32
      0x00140405,  //  0024  ADD	R5	R2	R5
      0x58180007,  //  0025  LDCONST	R6	K7
      0x141C0C04,  //  0026  LT	R7	R6	R4
      0x781E0011,  //  0027  JMPF	R7	#003A
      0x8C1C0304,  //  0028  GETMET	R7	R1	K4
      0x54260007,  //  0029  LDINT	R9	8
      0x04240A09,  //  002A  SUB	R9	R5	R9
      0x542A0007,  //  002B  LDINT	R10	8
      0x7C1C0600,  //  002C  CALL	R7	3
      0x8C200F06,  //  002D  GETMET	R8	R7	K6
      0x58280007,  //  002E  LDCONST	R10	K7
      0x542E0003,  //  002F  LDINT	R11	4
      0x7C200600,  //  0030  CALL	R8	3
      0x8C240F06,  //  0031  GETMET	R9	R7	K6
      0x542E0003,  //  0032  LDINT	R11	4
      0x54320003,  //  0033  LDINT	R12	4
      0x7C240600,  //  0034  CALL	R9	3
      0x542A0007,  //  0035  LDINT	R10	8
      0x0028120A,  //  0036  ADD	R10	R9	R10
      0x00140A0A,  //  0037  ADD	R5	R5	R10
      0x00180D05,  //  0038  ADD	R6	R6	K5
      0x7001FFEB,  //  0039  JMP		#0026
      0x041C0A02,  //  003A  SUB	R7	R5	R2
      0x001C0F05,  //  003B  ADD	R7	R7	K5
      0xA8040001,  //  003C  EXBLK	1	1
      0x80040E00,  //  003D  RET	1	R7
      0xA8040001,  //  003E  EXBLK	1	1
      0x7002000C,  //  003F  JMP		#004D
      0xAC080002,  //  0040  CATCH	R2	0	2
      0x70020009,  //  0041  JMP		#004C
      0xB8121000,  //  0042  GETNGBL	R4	K8
      0x8C100909,  //  0043  GETMET	R4	R4	K9
      0x001A1402,  //  0044  ADD	R6	K10	R2
      0x00180D0B,  //  0045  ADD	R6	R6	K11
      0x00180C03,  //  0046  ADD	R6	R6	R3
      0x581C000C,  //  0047  LDCONST	R7	K12
      0x7C100600,  //  0048  CALL	R4	3
      0x5411FFFE,  //  0049  LDINT	R4	-1
      0x80040800,  //  004A  RET	1	R4
      0x70020000,  //  004B  JMP		#004D
      0xB0080000,  //  004C  RAISE	2	R0	R0
      0x80000000,  //  004D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_ota
********************************************************************/
be_local_closure(Partition_info_is_ota,   /* name */
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
    /* K0   */  be_nested_str(subtype),
    /* K1   */  be_nested_str(type),
    /* K2   */  be_const_int(0),
    }),
    &be_const_str_is_ota,
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x1C080502,  //  0002  EQ	R2	R2	K2
      0x780A0008,  //  0003  JMPF	R2	#000D
      0x540A000F,  //  0004  LDINT	R2	16
      0x28080202,  //  0005  GE	R2	R1	R2
      0x780A0005,  //  0006  JMPF	R2	#000D
      0x540A001F,  //  0007  LDINT	R2	32
      0x14080202,  //  0008  LT	R2	R1	R2
      0x780A0002,  //  0009  JMPF	R2	#000D
      0x540A000F,  //  000A  LDINT	R2	16
      0x04080202,  //  000B  SUB	R2	R1	R2
      0x80040400,  //  000C  RET	1	R2
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Partition_info
********************************************************************/
be_local_class(Partition_info,
    6,
    NULL,
    be_nested_map(14,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(start, -1), be_const_var(2) },
        { be_const_key(type, -1), be_const_var(0) },
        { be_const_key(tobytes, 1), be_const_closure(Partition_info_tobytes_closure) },
        { be_const_key(tostring, 2), be_const_closure(Partition_info_tostring_closure) },
        { be_const_key(is_factory, -1), be_const_closure(Partition_info_is_factory_closure) },
        { be_const_key(init, 9), be_const_closure(Partition_info_init_closure) },
        { be_const_key(size, -1), be_const_var(3) },
        { be_const_key(is_spiffs, 8), be_const_closure(Partition_info_is_spiffs_closure) },
        { be_const_key(subtype, -1), be_const_var(1) },
        { be_const_key(get_image_size, -1), be_const_closure(Partition_info_get_image_size_closure) },
        { be_const_key(is_ota, -1), be_const_closure(Partition_info_is_ota_closure) },
        { be_const_key(label, -1), be_const_var(4) },
        { be_const_key(flags, -1), be_const_var(5) },
        { be_const_key(remove_trailing_zeroes, 0), be_const_static_closure(Partition_info_remove_trailing_zeroes_closure) },
    })),
    (bstring*) &be_const_str_Partition_info
);

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(partition_core_init,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(global),
    /* K1   */  be_nested_str(partition_core),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x90060200,  //  0001  SETMBR	R1	K1	R0
      0x80040000,  //  0002  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified module: partition_core
********************************************************************/
be_local_module(partition_core,
    "partition_core",
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(Partition_otadata, -1), be_const_class(be_class_Partition_otadata) },
        { be_const_key(Partition, -1), be_const_class(be_class_Partition) },
        { be_const_key(Partition_info, -1), be_const_class(be_class_Partition_info) },
        { be_const_key(init, -1), be_const_closure(partition_core_init_closure) },
    }))
);
BE_EXPORT_VARIABLE be_define_const_native_module(partition_core);
/********************************************************************/
