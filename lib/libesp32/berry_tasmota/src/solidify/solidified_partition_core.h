/* Solidification of partition_core.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
extern const bclass be_class_Partition_otadata;
// compact class 'Partition_otadata' ktab size: 24, total: 52 (saved 224 bytes)
static const bvalue be_ktab_class_Partition_otadata[24] = {
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
  /* K11  */  be_nested_str(_X3Cinstance_X3A_X20Partition_otadata_X28ota_active_X3A_X25s_X2C_X20ota_seq_X3D_X5B_X25d_X2C_X25d_X5D_X2C_X20ota_max_X3D_X25d_X29_X3E),
  /* K12  */  be_nested_str(active_otadata),
  /* K13  */  be_nested_str(ota_),
  /* K14  */  be_nested_str(factory),
  /* K15  */  be_nested_str(maxota),
  /* K16  */  be_nested_str(has_factory),
  /* K17  */  be_nested_str(read),
  /* K18  */  be_nested_str(get),
  /* K19  */  be_nested_str(_validate),
  /* K20  */  be_const_class(be_class_Partition_otadata),
  /* K21  */  be_nested_str(crc),
  /* K22  */  be_nested_str(crc32),
  /* K23  */  be_nested_str(load),
};


extern const bclass be_class_Partition_otadata;

/********************************************************************
** Solidified function: save
********************************************************************/
be_local_closure(class_Partition_otadata_save,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition_otadata,     /* shared constants */
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
** Solidified function: tostring
********************************************************************/
be_local_closure(class_Partition_otadata_tostring,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition_otadata,     /* shared constants */
    &be_const_str_tostring,
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x60040018,  //  0000  GETGBL	R1	G24
      0x5808000B,  //  0001  LDCONST	R2	K11
      0x880C010C,  //  0002  GETMBR	R3	R0	K12
      0x280C0702,  //  0003  GE	R3	R3	K2
      0x780E0004,  //  0004  JMPF	R3	#000A
      0x600C0008,  //  0005  GETGBL	R3	G8
      0x8810010C,  //  0006  GETMBR	R4	R0	K12
      0x7C0C0200,  //  0007  CALL	R3	1
      0x000E1A03,  //  0008  ADD	R3	K13	R3
      0x70020000,  //  0009  JMP		#000B
      0x580C000E,  //  000A  LDCONST	R3	K14
      0x88100101,  //  000B  GETMBR	R4	R0	K1
      0x88140103,  //  000C  GETMBR	R5	R0	K3
      0x8818010F,  //  000D  GETMBR	R6	R0	K15
      0x7C040A00,  //  000E  CALL	R1	5
      0x80040200,  //  000F  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _validate
********************************************************************/
be_local_closure(class_Partition_otadata__validate,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition_otadata,     /* shared constants */
    &be_const_str__validate,
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0x88040110,  //  0000  GETMBR	R1	R0	K16
      0x78060001,  //  0001  JMPF	R1	#0004
      0x5405FFFE,  //  0002  LDINT	R1	-1
      0x70020000,  //  0003  JMP		#0005
      0x58040002,  //  0004  LDCONST	R1	K2
      0x90021801,  //  0005  SETMBR	R0	K12	R1
      0x88040101,  //  0006  GETMBR	R1	R0	K1
      0x4C080000,  //  0007  LDNIL	R2
      0x20040202,  //  0008  NE	R1	R1	R2
      0x78060005,  //  0009  JMPF	R1	#0010
      0x88040101,  //  000A  GETMBR	R1	R0	K1
      0x04040304,  //  000B  SUB	R1	R1	K4
      0x8808010F,  //  000C  GETMBR	R2	R0	K15
      0x00080504,  //  000D  ADD	R2	R2	K4
      0x10040202,  //  000E  MOD	R1	R1	R2
      0x90021801,  //  000F  SETMBR	R0	K12	R1
      0x88040103,  //  0010  GETMBR	R1	R0	K3
      0x4C080000,  //  0011  LDNIL	R2
      0x20040202,  //  0012  NE	R1	R1	R2
      0x7806000D,  //  0013  JMPF	R1	#0022
      0x88040101,  //  0014  GETMBR	R1	R0	K1
      0x4C080000,  //  0015  LDNIL	R2
      0x1C040202,  //  0016  EQ	R1	R1	R2
      0x74060003,  //  0017  JMPT	R1	#001C
      0x88040103,  //  0018  GETMBR	R1	R0	K3
      0x88080101,  //  0019  GETMBR	R2	R0	K1
      0x24040202,  //  001A  GT	R1	R1	R2
      0x78060005,  //  001B  JMPF	R1	#0022
      0x88040103,  //  001C  GETMBR	R1	R0	K3
      0x04040304,  //  001D  SUB	R1	R1	K4
      0x8808010F,  //  001E  GETMBR	R2	R0	K15
      0x00080504,  //  001F  ADD	R2	R2	K4
      0x10040202,  //  0020  MOD	R1	R1	R2
      0x90021801,  //  0021  SETMBR	R0	K12	R1
      0x80000000,  //  0022  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_ota_max
********************************************************************/
be_local_closure(class_Partition_otadata_set_ota_max,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition_otadata,     /* shared constants */
    &be_const_str_set_ota_max,
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90021E01,  //  0000  SETMBR	R0	K15	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: load
********************************************************************/
be_local_closure(class_Partition_otadata_load,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition_otadata,     /* shared constants */
    &be_const_str_load,
    &be_const_str_solidified,
    ( &(const binstruction[48]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080311,  //  0001  GETMET	R2	R1	K17
      0x88100105,  //  0002  GETMBR	R4	R0	K5
      0x5416001F,  //  0003  LDINT	R5	32
      0x7C080600,  //  0004  CALL	R2	3
      0x8C0C0311,  //  0005  GETMET	R3	R1	K17
      0x88140105,  //  0006  GETMBR	R5	R0	K5
      0x541A0FFF,  //  0007  LDINT	R6	4096
      0x00140A06,  //  0008  ADD	R5	R5	R6
      0x541A001F,  //  0009  LDINT	R6	32
      0x7C0C0600,  //  000A  CALL	R3	3
      0x8C100512,  //  000B  GETMET	R4	R2	K18
      0x58180002,  //  000C  LDCONST	R6	K2
      0x541E0003,  //  000D  LDINT	R7	4
      0x7C100600,  //  000E  CALL	R4	3
      0x90020204,  //  000F  SETMBR	R0	K1	R4
      0x8C100712,  //  0010  GETMET	R4	R3	K18
      0x58180002,  //  0011  LDCONST	R6	K2
      0x541E0003,  //  0012  LDINT	R7	4
      0x7C100600,  //  0013  CALL	R4	3
      0x90020604,  //  0014  SETMBR	R0	K3	R4
      0x8C100512,  //  0015  GETMET	R4	R2	K18
      0x541A001B,  //  0016  LDINT	R6	28
      0x541E0003,  //  0017  LDINT	R7	4
      0x7C100600,  //  0018  CALL	R4	3
      0x8C140108,  //  0019  GETMET	R5	R0	K8
      0x881C0101,  //  001A  GETMBR	R7	R0	K1
      0x7C140400,  //  001B  CALL	R5	2
      0x1C100805,  //  001C  EQ	R4	R4	R5
      0x8C140712,  //  001D  GETMET	R5	R3	K18
      0x541E001B,  //  001E  LDINT	R7	28
      0x54220003,  //  001F  LDINT	R8	4
      0x7C140600,  //  0020  CALL	R5	3
      0x8C180108,  //  0021  GETMET	R6	R0	K8
      0x88200103,  //  0022  GETMBR	R8	R0	K3
      0x7C180400,  //  0023  CALL	R6	2
      0x1C140A06,  //  0024  EQ	R5	R5	R6
      0x5C180800,  //  0025  MOVE	R6	R4
      0x741A0001,  //  0026  JMPT	R6	#0029
      0x4C180000,  //  0027  LDNIL	R6
      0x90020206,  //  0028  SETMBR	R0	K1	R6
      0x5C180A00,  //  0029  MOVE	R6	R5
      0x741A0001,  //  002A  JMPT	R6	#002D
      0x4C180000,  //  002B  LDNIL	R6
      0x90020606,  //  002C  SETMBR	R0	K3	R6
      0x8C180113,  //  002D  GETMET	R6	R0	K19
      0x7C180200,  //  002E  CALL	R6	1
      0x80000000,  //  002F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: crc32_ota_seq
********************************************************************/
be_local_closure(class_Partition_otadata_crc32_ota_seq,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition_otadata,     /* shared constants */
    &be_const_str_crc32_ota_seq,
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x58040014,  //  0000  LDCONST	R1	K20
      0xA40A2A00,  //  0001  IMPORT	R2	K21
      0x8C0C0516,  //  0002  GETMET	R3	R2	K22
      0x5415FFFE,  //  0003  LDINT	R5	-1
      0x60180015,  //  0004  GETGBL	R6	G21
      0x7C180000,  //  0005  CALL	R6	0
      0x8C180D06,  //  0006  GETMET	R6	R6	K6
      0x5C200000,  //  0007  MOVE	R8	R0
      0x54260003,  //  0008  LDINT	R9	4
      0x7C180600,  //  0009  CALL	R6	3
      0x7C0C0600,  //  000A  CALL	R3	3
      0x80040600,  //  000B  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_active
********************************************************************/
be_local_closure(class_Partition_otadata_set_active,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition_otadata,     /* shared constants */
    &be_const_str_set_active,
    &be_const_str_solidified,
    ( &(const binstruction[41]) {  /* code */
      0x58080002,  //  0000  LDCONST	R2	K2
      0x580C0002,  //  0001  LDCONST	R3	K2
      0x88100101,  //  0002  GETMBR	R4	R0	K1
      0x4C140000,  //  0003  LDNIL	R5
      0x20100805,  //  0004  NE	R4	R4	R5
      0x78120001,  //  0005  JMPF	R4	#0008
      0x88080101,  //  0006  GETMBR	R2	R0	K1
      0x580C0002,  //  0007  LDCONST	R3	K2
      0x88100103,  //  0008  GETMBR	R4	R0	K3
      0x4C140000,  //  0009  LDNIL	R5
      0x20100805,  //  000A  NE	R4	R4	R5
      0x78120004,  //  000B  JMPF	R4	#0011
      0x88100103,  //  000C  GETMBR	R4	R0	K3
      0x24100802,  //  000D  GT	R4	R4	R2
      0x78120001,  //  000E  JMPF	R4	#0011
      0x88080103,  //  000F  GETMBR	R2	R0	K3
      0x580C0004,  //  0010  LDCONST	R3	K4
      0x04100504,  //  0011  SUB	R4	R2	K4
      0x8814010F,  //  0012  GETMBR	R5	R0	K15
      0x00140B04,  //  0013  ADD	R5	R5	K4
      0x10100805,  //  0014  MOD	R4	R4	R5
      0x20140801,  //  0015  NE	R5	R4	R1
      0x78160010,  //  0016  JMPF	R5	#0028
      0x24140204,  //  0017  GT	R5	R1	R4
      0x78160002,  //  0018  JMPF	R5	#001C
      0x04140204,  //  0019  SUB	R5	R1	R4
      0x00080405,  //  001A  ADD	R2	R2	R5
      0x70020004,  //  001B  JMP		#0021
      0x8814010F,  //  001C  GETMBR	R5	R0	K15
      0x00140B04,  //  001D  ADD	R5	R5	K4
      0x04140A04,  //  001E  SUB	R5	R5	R4
      0x00140A01,  //  001F  ADD	R5	R5	R1
      0x00080405,  //  0020  ADD	R2	R2	R5
      0x1C140704,  //  0021  EQ	R5	R3	K4
      0x78160001,  //  0022  JMPF	R5	#0025
      0x90020202,  //  0023  SETMBR	R0	K1	R2
      0x70020000,  //  0024  JMP		#0026
      0x90020602,  //  0025  SETMBR	R0	K3	R2
      0x8C140113,  //  0026  GETMET	R5	R0	K19
      0x7C140200,  //  0027  CALL	R5	1
      0x80000000,  //  0028  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Partition_otadata_init,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition_otadata,     /* shared constants */
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x90021E01,  //  0000  SETMBR	R0	K15	R1
      0x90022002,  //  0001  SETMBR	R0	K16	R2
      0x8810010F,  //  0002  GETMBR	R4	R0	K15
      0x4C140000,  //  0003  LDNIL	R5
      0x1C100805,  //  0004  EQ	R4	R4	R5
      0x78120000,  //  0005  JMPF	R4	#0007
      0x90021F04,  //  0006  SETMBR	R0	K15	K4
      0x90020A03,  //  0007  SETMBR	R0	K5	R3
      0x88100105,  //  0008  GETMBR	R4	R0	K5
      0x4C140000,  //  0009  LDNIL	R5
      0x1C100805,  //  000A  EQ	R4	R4	R5
      0x78120001,  //  000B  JMPF	R4	#000E
      0x5412DFFF,  //  000C  LDINT	R4	57344
      0x90020A04,  //  000D  SETMBR	R0	K5	R4
      0x5411FFFE,  //  000E  LDINT	R4	-1
      0x90021804,  //  000F  SETMBR	R0	K12	R4
      0x8C100117,  //  0010  GETMET	R4	R0	K23
      0x7C100200,  //  0011  CALL	R4	1
      0x80000000,  //  0012  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Partition_otadata
********************************************************************/
be_local_class(Partition_otadata,
    6,
    NULL,
    be_nested_map(14,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(init, -1), be_const_closure(class_Partition_otadata_init_closure) },
        { be_const_key(seq1, 13), be_const_var(5) },
        { be_const_key(save, -1), be_const_closure(class_Partition_otadata_save_closure) },
        { be_const_key(tostring, -1), be_const_closure(class_Partition_otadata_tostring_closure) },
        { be_const_key(_validate, 6), be_const_closure(class_Partition_otadata__validate_closure) },
        { be_const_key(set_ota_max, 0), be_const_closure(class_Partition_otadata_set_ota_max_closure) },
        { be_const_key(has_factory, -1), be_const_var(1) },
        { be_const_key(load, 8), be_const_closure(class_Partition_otadata_load_closure) },
        { be_const_key(crc32_ota_seq, -1), be_const_static_closure(class_Partition_otadata_crc32_ota_seq_closure) },
        { be_const_key(active_otadata, 11), be_const_var(3) },
        { be_const_key(offset, 9), be_const_var(2) },
        { be_const_key(seq0, -1), be_const_var(4) },
        { be_const_key(set_active, -1), be_const_closure(class_Partition_otadata_set_active_closure) },
        { be_const_key(maxota, -1), be_const_var(0) },
    })),
    (bstring*) &be_const_str_Partition_otadata
);
extern const bclass be_class_Partition;
// compact class 'Partition' ktab size: 71, total: 134 (saved 504 bytes)
static const bvalue be_ktab_class_Partition[71] = {
  /* K0   */  be_nested_str(flash),
  /* K1   */  be_nested_str(tobytes),
  /* K2   */  be_nested_str(erase),
  /* K3   */  be_nested_str(write),
  /* K4   */  be_nested_str(otadata),
  /* K5   */  be_nested_str(save),
  /* K6   */  be_nested_str(raw),
  /* K7   */  be_nested_str(read),
  /* K8   */  be_nested_str(active_otadata),
  /* K9   */  be_nested_str(slots),
  /* K10  */  be_nested_str(load),
  /* K11  */  be_nested_str(parse),
  /* K12  */  be_nested_str(load_otadata),
  /* K13  */  be_const_int(0),
  /* K14  */  be_const_int(1),
  /* K15  */  be_nested_str(get),
  /* K16  */  be_const_int(2),
  /* K17  */  be_nested_str(partition_core),
  /* K18  */  be_nested_str(Partition_info),
  /* K19  */  be_nested_str(push),
  /* K20  */  be_nested_str(md5),
  /* K21  */  be_nested_str(stop_iteration),
  /* K22  */  be_nested_str(tasmota),
  /* K23  */  be_nested_str(memory),
  /* K24  */  be_nested_str(find),
  /* K25  */  be_nested_str(flash_real),
  /* K26  */  be_nested_str(get_flash_definition_sector),
  /* K27  */  be_const_int(3),
  /* K28  */  be_nested_str(log),
  /* K29  */  be_nested_str(UPL_X3A_X20changing_X20flash_X20definition_X20from_X200x02X_X20to_X200x_X2502X),
  /* K30  */  be_nested_str(wrong_X20flash_X20size_X20),
  /* K31  */  be_nested_str(internal_error),
  /* K32  */  be_const_class(be_class_Partition),
  /* K33  */  be_nested_str(E9),
  /* K34  */  be_nested_str(get_unallocated_k),
  /* K35  */  be_nested_str(BRY_X3A_X20Trying_X20to_X20expand_X20FS_X20by_X20_X25i_X20kB),
  /* K36  */  be_nested_str(resize_max_flash_size_k),
  /* K37  */  be_nested_str(sz),
  /* K38  */  be_nested_str(global),
  /* K39  */  be_nested_str(restart_flag),
  /* K40  */  be_nested_str(BRY_X3A_X20Successfully_X20resized_X20FS_X2C_X20restarting),
  /* K41  */  be_nested_str(BRY_X3A_X20Exception_X3E_X20_X27_X25s_X27_X20_X2D_X20_X25s),
  /* K42  */  be_nested_str(factory),
  /* K43  */  be_nested_str(ota_max),
  /* K44  */  be_nested_str(type),
  /* K45  */  be_nested_str(subtype),
  /* K46  */  be_nested_str(start),
  /* K47  */  be_nested_str(Partition_otadata),
  /* K48  */  be_nested_str(has_factory),
  /* K49  */  be_nested_str(_X3Cinstance_X3A_X20Partition_X28_X5B_X0A),
  /* K50  */  be_nested_str(_X20_X20),
  /* K51  */  be_nested_str(tostring),
  /* K52  */  be_nested_str(_X0A),
  /* K53  */  be_nested_str(_X5D_X2C_X0A_X20_X20),
  /* K54  */  be_nested_str(_X0A_X29_X3E),
  /* K55  */  be_nested_str(get_factory_slot),
  /* K56  */  be_nested_str(value_error),
  /* K57  */  be_nested_str(Too_X20many_X20partiition_X20slots),
  /* K58  */  be_nested_str(MD5),
  /* K59  */  be_nested_str(update),
  /* K60  */  be_nested_str(EBEBFFFFFFFFFFFFFFFFFFFFFFFFFFFF),
  /* K61  */  be_nested_str(finish),
  /* K62  */  be_nested_str(is_spiffs),
  /* K63  */  be_nested_str(No_X20FS_X20partition_X20found),
  /* K64  */  be_nested_str(00),
  /* K65  */  be_nested_str(is_factory),
  /* K66  */  be_nested_str(Invalid_X20ota_X20partition_X20number),
  /* K67  */  be_nested_str(set_ota_max),
  /* K68  */  be_nested_str(set_active),
  /* K69  */  be_nested_str(get_max_flash_size_k),
  /* K70  */  be_nested_str(is_ota),
};


extern const bclass be_class_Partition;

/********************************************************************
** Solidified function: save
********************************************************************/
be_local_closure(class_Partition_save,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition,     /* shared constants */
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
** Solidified function: load
********************************************************************/
be_local_closure(class_Partition_load,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition,     /* shared constants */
    &be_const_str_load,
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080307,  //  0001  GETMET	R2	R1	K7
      0x54127FFF,  //  0002  LDINT	R4	32768
      0x54160FFF,  //  0003  LDINT	R5	4096
      0x7C080600,  //  0004  CALL	R2	3
      0x90020C02,  //  0005  SETMBR	R0	K6	R2
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_active
********************************************************************/
be_local_closure(class_Partition_get_active,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition,     /* shared constants */
    &be_const_str_get_active,
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x88040308,  //  0001  GETMBR	R1	R1	K8
      0x80040200,  //  0002  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Partition_init,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition,     /* shared constants */
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x60040012,  //  0000  GETGBL	R1	G18
      0x7C040000,  //  0001  CALL	R1	0
      0x90021201,  //  0002  SETMBR	R0	K9	R1
      0x8C04010A,  //  0003  GETMET	R1	R0	K10
      0x7C040200,  //  0004  CALL	R1	1
      0x8C04010B,  //  0005  GETMET	R1	R0	K11
      0x7C040200,  //  0006  CALL	R1	1
      0x8C04010C,  //  0007  GETMET	R1	R0	K12
      0x7C040200,  //  0008  CALL	R1	1
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse
********************************************************************/
be_local_closure(class_Partition_parse,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition,     /* shared constants */
    &be_const_str_parse,
    &be_const_str_solidified,
    ( &(const binstruction[56]) {  /* code */
      0x60040010,  //  0000  GETGBL	R1	G16
      0x540A005D,  //  0001  LDINT	R2	94
      0x400A1A02,  //  0002  CONNECT	R2	K13	R2
      0x7C040200,  //  0003  CALL	R1	1
      0xA802002E,  //  0004  EXBLK	0	#0034
      0x5C080200,  //  0005  MOVE	R2	R1
      0x7C080000,  //  0006  CALL	R2	0
      0x540E001F,  //  0007  LDINT	R3	32
      0x080C0403,  //  0008  MUL	R3	R2	R3
      0x0010050E,  //  0009  ADD	R4	R2	K14
      0x5416001F,  //  000A  LDINT	R5	32
      0x08100805,  //  000B  MUL	R4	R4	R5
      0x0410090E,  //  000C  SUB	R4	R4	K14
      0x400C0604,  //  000D  CONNECT	R3	R3	R4
      0x88100106,  //  000E  GETMBR	R4	R0	K6
      0x940C0803,  //  000F  GETIDX	R3	R4	R3
      0x8C10070F,  //  0010  GETMET	R4	R3	K15
      0x5818000D,  //  0011  LDCONST	R6	K13
      0x581C0010,  //  0012  LDCONST	R7	K16
      0x7C100600,  //  0013  CALL	R4	3
      0x541650A9,  //  0014  LDINT	R5	20650
      0x1C140805,  //  0015  EQ	R5	R4	R5
      0x78160008,  //  0016  JMPF	R5	#0020
      0xB8162200,  //  0017  GETNGBL	R5	K17
      0x8C140B12,  //  0018  GETMET	R5	R5	K18
      0x5C1C0600,  //  0019  MOVE	R7	R3
      0x7C140400,  //  001A  CALL	R5	2
      0x88180109,  //  001B  GETMBR	R6	R0	K9
      0x8C180D13,  //  001C  GETMET	R6	R6	K19
      0x5C200A00,  //  001D  MOVE	R8	R5
      0x7C180400,  //  001E  CALL	R6	2
      0x70020010,  //  001F  JMP		#0031
      0x5416EBEA,  //  0020  LDINT	R5	60395
      0x1C140805,  //  0021  EQ	R5	R4	R5
      0x7816000C,  //  0022  JMPF	R5	#0030
      0x5416001F,  //  0023  LDINT	R5	32
      0x08140405,  //  0024  MUL	R5	R2	R5
      0x541A000F,  //  0025  LDINT	R6	16
      0x00140A06,  //  0026  ADD	R5	R5	R6
      0x541A0020,  //  0027  LDINT	R6	33
      0x08180406,  //  0028  MUL	R6	R2	R6
      0x04180D0E,  //  0029  SUB	R6	R6	K14
      0x40140A06,  //  002A  CONNECT	R5	R5	R6
      0x88180106,  //  002B  GETMBR	R6	R0	K6
      0x94140C05,  //  002C  GETIDX	R5	R6	R5
      0x90022805,  //  002D  SETMBR	R0	K20	R5
      0x70020002,  //  002E  JMP		#0032
      0x70020000,  //  002F  JMP		#0031
      0x70020000,  //  0030  JMP		#0032
      0x7001FFD2,  //  0031  JMP		#0005
      0xA8040001,  //  0032  EXBLK	1	1
      0x70020002,  //  0033  JMP		#0037
      0x58040015,  //  0034  LDCONST	R1	K21
      0xAC040200,  //  0035  CATCH	R1	1	0
      0xB0080000,  //  0036  RAISE	2	R0	R0
      0x80000000,  //  0037  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_max_flash_size_k
********************************************************************/
be_local_closure(class_Partition_get_max_flash_size_k,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition,     /* shared constants */
    &be_const_str_get_max_flash_size_k,
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0xB8062C00,  //  0000  GETNGBL	R1	K22
      0x8C040317,  //  0001  GETMET	R1	R1	K23
      0x7C040200,  //  0002  CALL	R1	1
      0x94040300,  //  0003  GETIDX	R1	R1	K0
      0xB80A2C00,  //  0004  GETNGBL	R2	K22
      0x8C080517,  //  0005  GETMET	R2	R2	K23
      0x7C080200,  //  0006  CALL	R2	1
      0x8C080518,  //  0007  GETMET	R2	R2	K24
      0x58100019,  //  0008  LDCONST	R4	K25
      0x5C140200,  //  0009  MOVE	R5	R1
      0x7C080600,  //  000A  CALL	R2	3
      0x200C0202,  //  000B  NE	R3	R1	R2
      0x780E0005,  //  000C  JMPF	R3	#0013
      0x8C0C011A,  //  000D  GETMET	R3	R0	K26
      0x7C0C0200,  //  000E  CALL	R3	1
      0x4C100000,  //  000F  LDNIL	R4
      0x200C0604,  //  0010  NE	R3	R3	R4
      0x780E0000,  //  0011  JMPF	R3	#0013
      0x5C040400,  //  0012  MOVE	R1	R2
      0x80040200,  //  0013  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: resize_max_flash_size_k
********************************************************************/
be_local_closure(class_Partition_resize_max_flash_size_k,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition,     /* shared constants */
    &be_const_str_resize_max_flash_size_k,
    &be_const_str_solidified,
    ( &(const binstruction[94]) {  /* code */
      0xB8062C00,  //  0000  GETNGBL	R1	K22
      0x8C040317,  //  0001  GETMET	R1	R1	K23
      0x7C040200,  //  0002  CALL	R1	1
      0x94040300,  //  0003  GETIDX	R1	R1	K0
      0xB80A2C00,  //  0004  GETNGBL	R2	K22
      0x8C080517,  //  0005  GETMET	R2	R2	K23
      0x7C080200,  //  0006  CALL	R2	1
      0x8C080518,  //  0007  GETMET	R2	R2	K24
      0x58100019,  //  0008  LDCONST	R4	K25
      0x5C140200,  //  0009  MOVE	R5	R1
      0x7C080600,  //  000A  CALL	R2	3
      0x8C0C011A,  //  000B  GETMET	R3	R0	K26
      0x7C0C0200,  //  000C  CALL	R3	1
      0x20100202,  //  000D  NE	R4	R1	R2
      0x7812004D,  //  000E  JMPF	R4	#005D
      0x4C100000,  //  000F  LDNIL	R4
      0x20100604,  //  0010  NE	R4	R3	R4
      0x7812004A,  //  0011  JMPF	R4	#005D
      0xA4120000,  //  0012  IMPORT	R4	K0
      0x5C040400,  //  0013  MOVE	R1	R2
      0x8C140907,  //  0014  GETMET	R5	R4	K7
      0x5C1C0600,  //  0015  MOVE	R7	R3
      0x54220003,  //  0016  LDINT	R8	4
      0x7C140600,  //  0017  CALL	R5	3
      0x94180B1B,  //  0018  GETIDX	R6	R5	K27
      0x4C1C0000,  //  0019  LDNIL	R7
      0x542203FF,  //  001A  LDINT	R8	1024
      0x0C200408,  //  001B  DIV	R8	R2	R8
      0x1C24110E,  //  001C  EQ	R9	R8	K14
      0x78260001,  //  001D  JMPF	R9	#0020
      0x581C000D,  //  001E  LDCONST	R7	K13
      0x70020020,  //  001F  JMP		#0041
      0x1C241110,  //  0020  EQ	R9	R8	K16
      0x78260001,  //  0021  JMPF	R9	#0024
      0x541E000F,  //  0022  LDINT	R7	16
      0x7002001C,  //  0023  JMP		#0041
      0x54260003,  //  0024  LDINT	R9	4
      0x1C241009,  //  0025  EQ	R9	R8	R9
      0x78260001,  //  0026  JMPF	R9	#0029
      0x541E001F,  //  0027  LDINT	R7	32
      0x70020017,  //  0028  JMP		#0041
      0x54260007,  //  0029  LDINT	R9	8
      0x1C241009,  //  002A  EQ	R9	R8	R9
      0x78260001,  //  002B  JMPF	R9	#002E
      0x541E002F,  //  002C  LDINT	R7	48
      0x70020012,  //  002D  JMP		#0041
      0x5426000F,  //  002E  LDINT	R9	16
      0x1C241009,  //  002F  EQ	R9	R8	R9
      0x78260001,  //  0030  JMPF	R9	#0033
      0x541E003F,  //  0031  LDINT	R7	64
      0x7002000D,  //  0032  JMP		#0041
      0x5426001F,  //  0033  LDINT	R9	32
      0x1C241009,  //  0034  EQ	R9	R8	R9
      0x78260001,  //  0035  JMPF	R9	#0038
      0x541E004F,  //  0036  LDINT	R7	80
      0x70020008,  //  0037  JMP		#0041
      0x5426003F,  //  0038  LDINT	R9	64
      0x1C241009,  //  0039  EQ	R9	R8	R9
      0x78260001,  //  003A  JMPF	R9	#003D
      0x541E005F,  //  003B  LDINT	R7	96
      0x70020003,  //  003C  JMP		#0041
      0x5426007F,  //  003D  LDINT	R9	128
      0x1C241009,  //  003E  EQ	R9	R8	R9
      0x78260000,  //  003F  JMPF	R9	#0041
      0x541E006F,  //  0040  LDINT	R7	112
      0x4C240000,  //  0041  LDNIL	R9
      0x20240E09,  //  0042  NE	R9	R7	R9
      0x78260013,  //  0043  JMPF	R9	#0058
      0x94240B1B,  //  0044  GETIDX	R9	R5	K27
      0x94280B1B,  //  0045  GETIDX	R10	R5	K27
      0x542E000E,  //  0046  LDINT	R11	15
      0x2C28140B,  //  0047  AND	R10	R10	R11
      0x30281407,  //  0048  OR	R10	R10	R7
      0x9816360A,  //  0049  SETIDX	R5	K27	R10
      0x8C280903,  //  004A  GETMET	R10	R4	K3
      0x5C300600,  //  004B  MOVE	R12	R3
      0x5C340A00,  //  004C  MOVE	R13	R5
      0x7C280600,  //  004D  CALL	R10	3
      0xB82A2C00,  //  004E  GETNGBL	R10	K22
      0x8C28151C,  //  004F  GETMET	R10	R10	K28
      0x60300018,  //  0050  GETGBL	R12	G24
      0x5834001D,  //  0051  LDCONST	R13	K29
      0x5C381200,  //  0052  MOVE	R14	R9
      0x943C0B1B,  //  0053  GETIDX	R15	R5	K27
      0x7C300600,  //  0054  CALL	R12	3
      0x5834001B,  //  0055  LDCONST	R13	K27
      0x7C280600,  //  0056  CALL	R10	3
      0x70020004,  //  0057  JMP		#005D
      0x60240008,  //  0058  GETGBL	R9	G8
      0x5C281000,  //  0059  MOVE	R10	R8
      0x7C240200,  //  005A  CALL	R9	1
      0x00263C09,  //  005B  ADD	R9	K30	R9
      0xB0063E09,  //  005C  RAISE	1	K31	R9
      0x80000000,  //  005D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_flash_definition_sector
********************************************************************/
be_local_closure(class_Partition_get_flash_definition_sector,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    0,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition,     /* shared constants */
    &be_const_str_get_flash_definition_sector,
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x58000020,  //  0000  LDCONST	R0	K32
      0xA4060000,  //  0001  IMPORT	R1	K0
      0x60080010,  //  0002  GETGBL	R2	G16
      0x400E1B0E,  //  0003  CONNECT	R3	K13	K14
      0x7C080200,  //  0004  CALL	R2	1
      0xA802000F,  //  0005  EXBLK	0	#0016
      0x5C0C0400,  //  0006  MOVE	R3	R2
      0x7C0C0000,  //  0007  CALL	R3	0
      0x54120FFF,  //  0008  LDINT	R4	4096
      0x08100604,  //  0009  MUL	R4	R3	R4
      0x8C140307,  //  000A  GETMET	R5	R1	K7
      0x5C1C0800,  //  000B  MOVE	R7	R4
      0x5820000E,  //  000C  LDCONST	R8	K14
      0x7C140600,  //  000D  CALL	R5	3
      0x60180015,  //  000E  GETGBL	R6	G21
      0x581C0021,  //  000F  LDCONST	R7	K33
      0x7C180200,  //  0010  CALL	R6	1
      0x1C140A06,  //  0011  EQ	R5	R5	R6
      0x78160001,  //  0012  JMPF	R5	#0015
      0xA8040001,  //  0013  EXBLK	1	1
      0x80040800,  //  0014  RET	1	R4
      0x7001FFEF,  //  0015  JMP		#0006
      0x58080015,  //  0016  LDCONST	R2	K21
      0xAC080200,  //  0017  CATCH	R2	1	0
      0xB0080000,  //  0018  RAISE	2	R0	R0
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: resize_fs_to_max
********************************************************************/
be_local_closure(class_Partition_resize_fs_to_max,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition,     /* shared constants */
    &be_const_str_resize_fs_to_max,
    &be_const_str_solidified,
    ( &(const binstruction[52]) {  /* code */
      0xA8020024,  //  0000  EXBLK	0	#0026
      0x8C040122,  //  0001  GETMET	R1	R0	K34
      0x7C040200,  //  0002  CALL	R1	1
      0x1808030D,  //  0003  LE	R2	R1	K13
      0x780A0002,  //  0004  JMPF	R2	#0008
      0x4C080000,  //  0005  LDNIL	R2
      0xA8040001,  //  0006  EXBLK	1	1
      0x80040400,  //  0007  RET	1	R2
      0xB80A2C00,  //  0008  GETNGBL	R2	K22
      0x8C08051C,  //  0009  GETMET	R2	R2	K28
      0x60100018,  //  000A  GETGBL	R4	G24
      0x58140023,  //  000B  LDCONST	R5	K35
      0x5C180200,  //  000C  MOVE	R6	R1
      0x7C100400,  //  000D  CALL	R4	2
      0x58140010,  //  000E  LDCONST	R5	K16
      0x7C080600,  //  000F  CALL	R2	3
      0x8C080124,  //  0010  GETMET	R2	R0	K36
      0x7C080200,  //  0011  CALL	R2	1
      0x88080109,  //  0012  GETMBR	R2	R0	K9
      0x540DFFFE,  //  0013  LDINT	R3	-1
      0x94080403,  //  0014  GETIDX	R2	R2	R3
      0x541203FF,  //  0015  LDINT	R4	1024
      0x08100204,  //  0016  MUL	R4	R1	R4
      0x880C0525,  //  0017  GETMBR	R3	R2	K37
      0x000C0604,  //  0018  ADD	R3	R3	R4
      0x900A4A03,  //  0019  SETMBR	R2	K37	R3
      0x8C0C0105,  //  001A  GETMET	R3	R0	K5
      0x7C0C0200,  //  001B  CALL	R3	1
      0xB80E2C00,  //  001C  GETNGBL	R3	K22
      0x880C0726,  //  001D  GETMBR	R3	R3	K38
      0x900E4F10,  //  001E  SETMBR	R3	K39	K16
      0xB80E2C00,  //  001F  GETNGBL	R3	K22
      0x8C0C071C,  //  0020  GETMET	R3	R3	K28
      0x58140028,  //  0021  LDCONST	R5	K40
      0x58180010,  //  0022  LDCONST	R6	K16
      0x7C0C0600,  //  0023  CALL	R3	3
      0xA8040001,  //  0024  EXBLK	1	1
      0x7002000C,  //  0025  JMP		#0033
      0xAC040002,  //  0026  CATCH	R1	0	2
      0x70020009,  //  0027  JMP		#0032
      0xB80E2C00,  //  0028  GETNGBL	R3	K22
      0x8C0C071C,  //  0029  GETMET	R3	R3	K28
      0x60140018,  //  002A  GETGBL	R5	G24
      0x58180029,  //  002B  LDCONST	R6	K41
      0x5C1C0200,  //  002C  MOVE	R7	R1
      0x5C200400,  //  002D  MOVE	R8	R2
      0x7C140600,  //  002E  CALL	R5	3
      0x58180010,  //  002F  LDCONST	R6	K16
      0x7C0C0600,  //  0030  CALL	R3	3
      0x70020000,  //  0031  JMP		#0033
      0xB0080000,  //  0032  RAISE	2	R0	R0
      0x80000000,  //  0033  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: switch_factory
********************************************************************/
be_local_closure(class_Partition_switch_factory,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition,     /* shared constants */
    &be_const_str_switch_factory,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C052A,  //  0001  GETMET	R3	R2	K42
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: load_otadata
********************************************************************/
be_local_closure(class_Partition_load_otadata,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition,     /* shared constants */
    &be_const_str_load_otadata,
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0x5406DFFF,  //  0000  LDINT	R1	57344
      0x8C08012B,  //  0001  GETMET	R2	R0	K43
      0x7C080200,  //  0002  CALL	R2	1
      0x600C0010,  //  0003  GETGBL	R3	G16
      0x88100109,  //  0004  GETMBR	R4	R0	K9
      0x7C0C0200,  //  0005  CALL	R3	1
      0xA8020009,  //  0006  EXBLK	0	#0011
      0x5C100600,  //  0007  MOVE	R4	R3
      0x7C100000,  //  0008  CALL	R4	0
      0x8814092C,  //  0009  GETMBR	R5	R4	K44
      0x1C140B0E,  //  000A  EQ	R5	R5	K14
      0x78160003,  //  000B  JMPF	R5	#0010
      0x8814092D,  //  000C  GETMBR	R5	R4	K45
      0x1C140B0D,  //  000D  EQ	R5	R5	K13
      0x78160000,  //  000E  JMPF	R5	#0010
      0x8804092E,  //  000F  GETMBR	R1	R4	K46
      0x7001FFF5,  //  0010  JMP		#0007
      0x580C0015,  //  0011  LDCONST	R3	K21
      0xAC0C0200,  //  0012  CATCH	R3	1	0
      0xB0080000,  //  0013  RAISE	2	R0	R0
      0xB80E2200,  //  0014  GETNGBL	R3	K17
      0x8C0C072F,  //  0015  GETMET	R3	R3	K47
      0x5C140400,  //  0016  MOVE	R5	R2
      0x8C180130,  //  0017  GETMET	R6	R0	K48
      0x7C180200,  //  0018  CALL	R6	1
      0x5C1C0200,  //  0019  MOVE	R7	R1
      0x7C0C0800,  //  001A  CALL	R3	4
      0x90020803,  //  001B  SETMBR	R0	K4	R3
      0x80000000,  //  001C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_Partition_tostring,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition,     /* shared constants */
    &be_const_str_tostring,
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x58040031,  //  0000  LDCONST	R1	K49
      0x60080010,  //  0001  GETGBL	R2	G16
      0x880C0109,  //  0002  GETMBR	R3	R0	K9
      0x7C080200,  //  0003  CALL	R2	1
      0xA8020007,  //  0004  EXBLK	0	#000D
      0x5C0C0400,  //  0005  MOVE	R3	R2
      0x7C0C0000,  //  0006  CALL	R3	0
      0x00040332,  //  0007  ADD	R1	R1	K50
      0x8C100733,  //  0008  GETMET	R4	R3	K51
      0x7C100200,  //  0009  CALL	R4	1
      0x00040204,  //  000A  ADD	R1	R1	R4
      0x00040334,  //  000B  ADD	R1	R1	K52
      0x7001FFF7,  //  000C  JMP		#0005
      0x58080015,  //  000D  LDCONST	R2	K21
      0xAC080200,  //  000E  CATCH	R2	1	0
      0xB0080000,  //  000F  RAISE	2	R0	R0
      0x00040335,  //  0010  ADD	R1	R1	K53
      0x88080104,  //  0011  GETMBR	R2	R0	K4
      0x8C080533,  //  0012  GETMET	R2	R2	K51
      0x7C080200,  //  0013  CALL	R2	1
      0x00040202,  //  0014  ADD	R1	R1	R2
      0x00040336,  //  0015  ADD	R1	R1	K54
      0x80040200,  //  0016  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: has_factory
********************************************************************/
be_local_closure(class_Partition_has_factory,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition,     /* shared constants */
    &be_const_str_has_factory,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C040137,  //  0000  GETMET	R1	R0	K55
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20040202,  //  0003  NE	R1	R1	R2
      0x80040200,  //  0004  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tobytes
********************************************************************/
be_local_closure(class_Partition_tobytes,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition,     /* shared constants */
    &be_const_str_tobytes,
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0x6004000C,  //  0000  GETGBL	R1	G12
      0x88080109,  //  0001  GETMBR	R2	R0	K9
      0x7C040200,  //  0002  CALL	R1	1
      0x540A005E,  //  0003  LDINT	R2	95
      0x24040202,  //  0004  GT	R1	R1	R2
      0x78060000,  //  0005  JMPF	R1	#0007
      0xB0067139,  //  0006  RAISE	1	K56	K57
      0x60040015,  //  0007  GETGBL	R1	G21
      0x7C040000,  //  0008  CALL	R1	0
      0x60080010,  //  0009  GETGBL	R2	G16
      0x880C0109,  //  000A  GETMBR	R3	R0	K9
      0x7C080200,  //  000B  CALL	R2	1
      0xA8020005,  //  000C  EXBLK	0	#0013
      0x5C0C0400,  //  000D  MOVE	R3	R2
      0x7C0C0000,  //  000E  CALL	R3	0
      0x8C100701,  //  000F  GETMET	R4	R3	K1
      0x7C100200,  //  0010  CALL	R4	1
      0x00040204,  //  0011  ADD	R1	R1	R4
      0x7001FFF9,  //  0012  JMP		#000D
      0x58080015,  //  0013  LDCONST	R2	K21
      0xAC080200,  //  0014  CATCH	R2	1	0
      0xB0080000,  //  0015  RAISE	2	R0	R0
      0xB80A7400,  //  0016  GETNGBL	R2	K58
      0x7C080000,  //  0017  CALL	R2	0
      0x8C0C053B,  //  0018  GETMET	R3	R2	K59
      0x5C140200,  //  0019  MOVE	R5	R1
      0x7C0C0400,  //  001A  CALL	R3	2
      0x600C0015,  //  001B  GETGBL	R3	G21
      0x5810003C,  //  001C  LDCONST	R4	K60
      0x7C0C0200,  //  001D  CALL	R3	1
      0x00040203,  //  001E  ADD	R1	R1	R3
      0x8C0C053D,  //  001F  GETMET	R3	R2	K61
      0x7C0C0200,  //  0020  CALL	R3	1
      0x00040203,  //  0021  ADD	R1	R1	R3
      0x80040200,  //  0022  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: ota_max
********************************************************************/
be_local_closure(class_Partition_ota_max,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition,     /* shared constants */
    &be_const_str_ota_max,
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0x4C040000,  //  0000  LDNIL	R1
      0x60080010,  //  0001  GETGBL	R2	G16
      0x880C0109,  //  0002  GETMBR	R3	R0	K9
      0x7C080200,  //  0003  CALL	R2	1
      0xA8020016,  //  0004  EXBLK	0	#001C
      0x5C0C0400,  //  0005  MOVE	R3	R2
      0x7C0C0000,  //  0006  CALL	R3	0
      0x8810072C,  //  0007  GETMBR	R4	R3	K44
      0x1C10090D,  //  0008  EQ	R4	R4	K13
      0x78120010,  //  0009  JMPF	R4	#001B
      0x8810072D,  //  000A  GETMBR	R4	R3	K45
      0x5416000F,  //  000B  LDINT	R5	16
      0x28100805,  //  000C  GE	R4	R4	R5
      0x7812000C,  //  000D  JMPF	R4	#001B
      0x8810072D,  //  000E  GETMBR	R4	R3	K45
      0x5416001F,  //  000F  LDINT	R5	32
      0x14100805,  //  0010  LT	R4	R4	R5
      0x78120008,  //  0011  JMPF	R4	#001B
      0x8810072D,  //  0012  GETMBR	R4	R3	K45
      0x5416000F,  //  0013  LDINT	R5	16
      0x04100805,  //  0014  SUB	R4	R4	R5
      0x4C140000,  //  0015  LDNIL	R5
      0x1C140205,  //  0016  EQ	R5	R1	R5
      0x74160001,  //  0017  JMPT	R5	#001A
      0x24140801,  //  0018  GT	R5	R4	R1
      0x78160000,  //  0019  JMPF	R5	#001B
      0x5C040800,  //  001A  MOVE	R1	R4
      0x7001FFE8,  //  001B  JMP		#0005
      0x58080015,  //  001C  LDCONST	R2	K21
      0xAC080200,  //  001D  CATCH	R2	1	0
      0xB0080000,  //  001E  RAISE	2	R0	R0
      0x80040200,  //  001F  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invalidate_spiffs
********************************************************************/
be_local_closure(class_Partition_invalidate_spiffs,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition,     /* shared constants */
    &be_const_str_invalidate_spiffs,
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x88080109,  //  0001  GETMBR	R2	R0	K9
      0x540DFFFE,  //  0002  LDINT	R3	-1
      0x94080403,  //  0003  GETIDX	R2	R2	R3
      0x8C0C053E,  //  0004  GETMET	R3	R2	K62
      0x7C0C0200,  //  0005  CALL	R3	1
      0x740E0000,  //  0006  JMPT	R3	#0008
      0xB006713F,  //  0007  RAISE	1	K56	K63
      0x600C0015,  //  0008  GETGBL	R3	G21
      0x58100040,  //  0009  LDCONST	R4	K64
      0x7C0C0200,  //  000A  CALL	R3	1
      0x8C100303,  //  000B  GETMET	R4	R1	K3
      0x8818052E,  //  000C  GETMBR	R6	R2	K46
      0x5C1C0600,  //  000D  MOVE	R7	R3
      0x7C100600,  //  000E  CALL	R4	3
      0x8C100303,  //  000F  GETMET	R4	R1	K3
      0x8818052E,  //  0010  GETMBR	R6	R2	K46
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
** Solidified function: get_factory_slot
********************************************************************/
be_local_closure(class_Partition_get_factory_slot,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition,     /* shared constants */
    &be_const_str_get_factory_slot,
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x60040010,  //  0000  GETGBL	R1	G16
      0x88080109,  //  0001  GETMBR	R2	R0	K9
      0x7C040200,  //  0002  CALL	R1	1
      0xA8020007,  //  0003  EXBLK	0	#000C
      0x5C080200,  //  0004  MOVE	R2	R1
      0x7C080000,  //  0005  CALL	R2	0
      0x8C0C0541,  //  0006  GETMET	R3	R2	K65
      0x7C0C0200,  //  0007  CALL	R3	1
      0x780E0001,  //  0008  JMPF	R3	#000B
      0xA8040001,  //  0009  EXBLK	1	1
      0x80040400,  //  000A  RET	1	R2
      0x7001FFF7,  //  000B  JMP		#0004
      0x58040015,  //  000C  LDCONST	R1	K21
      0xAC040200,  //  000D  CATCH	R1	1	0
      0xB0080000,  //  000E  RAISE	2	R0	R0
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_active
********************************************************************/
be_local_closure(class_Partition_set_active,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition,     /* shared constants */
    &be_const_str_set_active,
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x1408030D,  //  0000  LT	R2	R1	K13
      0x740A0003,  //  0001  JMPT	R2	#0006
      0x8C08012B,  //  0002  GETMET	R2	R0	K43
      0x7C080200,  //  0003  CALL	R2	1
      0x24080202,  //  0004  GT	R2	R1	R2
      0x780A0000,  //  0005  JMPF	R2	#0007
      0xB0067142,  //  0006  RAISE	1	K56	K66
      0x88080104,  //  0007  GETMBR	R2	R0	K4
      0x8C080543,  //  0008  GETMET	R2	R2	K67
      0x8C10012B,  //  0009  GETMET	R4	R0	K43
      0x7C100200,  //  000A  CALL	R4	1
      0x7C080400,  //  000B  CALL	R2	2
      0x88080104,  //  000C  GETMBR	R2	R0	K4
      0x8C080544,  //  000D  GETMET	R2	R2	K68
      0x5C100200,  //  000E  MOVE	R4	R1
      0x7C080400,  //  000F  CALL	R2	2
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_unallocated_k
********************************************************************/
be_local_closure(class_Partition_get_unallocated_k,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition,     /* shared constants */
    &be_const_str_get_unallocated_k,
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x88040109,  //  0000  GETMBR	R1	R0	K9
      0x5409FFFE,  //  0001  LDINT	R2	-1
      0x94040202,  //  0002  GETIDX	R1	R1	R2
      0x8C08033E,  //  0003  GETMET	R2	R1	K62
      0x7C080200,  //  0004  CALL	R2	1
      0x780A000A,  //  0005  JMPF	R2	#0011
      0x8C080145,  //  0006  GETMET	R2	R0	K69
      0x7C080200,  //  0007  CALL	R2	1
      0x880C032E,  //  0008  GETMBR	R3	R1	K46
      0x88100325,  //  0009  GETMBR	R4	R1	K37
      0x000C0604,  //  000A  ADD	R3	R3	R4
      0x541203FF,  //  000B  LDINT	R4	1024
      0x0C0C0604,  //  000C  DIV	R3	R3	R4
      0x14100602,  //  000D  LT	R4	R3	R2
      0x78120001,  //  000E  JMPF	R4	#0011
      0x04100403,  //  000F  SUB	R4	R2	R3
      0x80040800,  //  0010  RET	1	R4
      0x80061A00,  //  0011  RET	1	K13
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_ota_slot
********************************************************************/
be_local_closure(class_Partition_get_ota_slot,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition,     /* shared constants */
    &be_const_str_get_ota_slot,
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x60080010,  //  0000  GETGBL	R2	G16
      0x880C0109,  //  0001  GETMBR	R3	R0	K9
      0x7C080200,  //  0002  CALL	R2	1
      0xA8020008,  //  0003  EXBLK	0	#000D
      0x5C0C0400,  //  0004  MOVE	R3	R2
      0x7C0C0000,  //  0005  CALL	R3	0
      0x8C100746,  //  0006  GETMET	R4	R3	K70
      0x7C100200,  //  0007  CALL	R4	1
      0x1C100801,  //  0008  EQ	R4	R4	R1
      0x78120001,  //  0009  JMPF	R4	#000C
      0xA8040001,  //  000A  EXBLK	1	1
      0x80040600,  //  000B  RET	1	R3
      0x7001FFF6,  //  000C  JMP		#0004
      0x58080015,  //  000D  LDCONST	R2	K21
      0xAC080200,  //  000E  CATCH	R2	1	0
      0xB0080000,  //  000F  RAISE	2	R0	R0
      0x4C080000,  //  0010  LDNIL	R2
      0x80040400,  //  0011  RET	1	R2
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
    be_nested_map(24,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(save, -1), be_const_closure(class_Partition_save_closure) },
        { be_const_key(load, 8), be_const_closure(class_Partition_load_closure) },
        { be_const_key(get_active, -1), be_const_closure(class_Partition_get_active_closure) },
        { be_const_key(init, -1), be_const_closure(class_Partition_init_closure) },
        { be_const_key(parse, 21), be_const_closure(class_Partition_parse_closure) },
        { be_const_key(otadata, -1), be_const_var(3) },
        { be_const_key(slots, 11), be_const_var(2) },
        { be_const_key(get_ota_slot, -1), be_const_closure(class_Partition_get_ota_slot_closure) },
        { be_const_key(resize_max_flash_size_k, -1), be_const_closure(class_Partition_resize_max_flash_size_k_closure) },
        { be_const_key(get_flash_definition_sector, -1), be_const_static_closure(class_Partition_get_flash_definition_sector_closure) },
        { be_const_key(resize_fs_to_max, -1), be_const_closure(class_Partition_resize_fs_to_max_closure) },
        { be_const_key(set_active, -1), be_const_closure(class_Partition_set_active_closure) },
        { be_const_key(get_factory_slot, -1), be_const_closure(class_Partition_get_factory_slot_closure) },
        { be_const_key(tostring, -1), be_const_closure(class_Partition_tostring_closure) },
        { be_const_key(invalidate_spiffs, -1), be_const_closure(class_Partition_invalidate_spiffs_closure) },
        { be_const_key(tobytes, 12), be_const_closure(class_Partition_tobytes_closure) },
        { be_const_key(load_otadata, 20), be_const_closure(class_Partition_load_otadata_closure) },
        { be_const_key(raw, -1), be_const_var(0) },
        { be_const_key(has_factory, 14), be_const_closure(class_Partition_has_factory_closure) },
        { be_const_key(md5, -1), be_const_var(1) },
        { be_const_key(ota_max, -1), be_const_closure(class_Partition_ota_max_closure) },
        { be_const_key(switch_factory, -1), be_const_closure(class_Partition_switch_factory_closure) },
        { be_const_key(get_unallocated_k, -1), be_const_closure(class_Partition_get_unallocated_k_closure) },
        { be_const_key(get_max_flash_size_k, 7), be_const_closure(class_Partition_get_max_flash_size_k_closure) },
    })),
    (bstring*) &be_const_str_Partition
);
extern const bclass be_class_Partition_info;
// compact class 'Partition_info' ktab size: 49, total: 92 (saved 344 bytes)
static const bvalue be_ktab_class_Partition_info[49] = {
  /* K0   */  be_nested_str(type),
  /* K1   */  be_const_int(0),
  /* K2   */  be_nested_str(subtype),
  /* K3   */  be_nested_str(app),
  /* K4   */  be_const_int(1),
  /* K5   */  be_nested_str(data),
  /* K6   */  be_nested_str(0x_X2502X),
  /* K7   */  be_nested_str(start),
  /* K8   */  be_nested_str(sz),
  /* K9   */  be_nested_str(label),
  /* K10  */  be_nested_str(),
  /* K11  */  be_nested_str(flags),
  /* K12  */  be_nested_str(get),
  /* K13  */  be_const_int(2),
  /* K14  */  be_const_int(3),
  /* K15  */  be_nested_str(remove_trailing_zeroes),
  /* K16  */  be_nested_str(asstring),
  /* K17  */  be_nested_str(invalid_X20magic_X20number_X20_X2502X),
  /* K18  */  be_nested_str(internal_error),
  /* K19  */  be_nested_str(factory),
  /* K20  */  be_nested_str(ota_),
  /* K21  */  be_nested_str(test),
  /* K22  */  be_nested_str(otadata),
  /* K23  */  be_nested_str(phy),
  /* K24  */  be_nested_str(nvs),
  /* K25  */  be_nested_str(coredump),
  /* K26  */  be_nested_str(nvskeys),
  /* K27  */  be_nested_str(efuse_em),
  /* K28  */  be_nested_str(esphttpd),
  /* K29  */  be_nested_str(fat),
  /* K30  */  be_nested_str(spiffs),
  /* K31  */  be_nested_str(type_to_string),
  /* K32  */  be_nested_str(subtype_to_string),
  /* K33  */  be_nested_str(_X20_X28),
  /* K34  */  be_nested_str(_X29),
  /* K35  */  be_nested_str(_X3Cinstance_X3A_X20Partition_info_X28_X25d_X25s_X2C_X25d_X25s_X2C0x_X2508X_X2C0x_X2508X_X2C_X27_X25s_X27_X2C0x_X25X_X29_X3E),
  /* K36  */  be_nested_str(AA50),
  /* K37  */  be_nested_str(resize),
  /* K38  */  be_nested_str(add),
  /* K39  */  be_nested_str(fromstring),
  /* K40  */  be_const_class(be_class_Partition_info),
  /* K41  */  be_nested_str(flash),
  /* K42  */  be_nested_str(is_ota),
  /* K43  */  be_nested_str(is_factory),
  /* K44  */  be_nested_str(read),
  /* K45  */  be_nested_str(tasmota),
  /* K46  */  be_nested_str(log),
  /* K47  */  be_nested_str(BRY_X3A_X20Exception_X3E_X20_X27),
  /* K48  */  be_nested_str(_X27_X20_X2D_X20),
};


extern const bclass be_class_Partition_info;

/********************************************************************
** Solidified function: is_factory
********************************************************************/
be_local_closure(class_Partition_info_is_factory,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition_info,     /* shared constants */
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
** Solidified function: type_to_string
********************************************************************/
be_local_closure(class_Partition_info_type_to_string,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition_info,     /* shared constants */
    &be_const_str_type_to_string,
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x1C040301,  //  0001  EQ	R1	R1	K1
      0x78060001,  //  0002  JMPF	R1	#0005
      0x80060600,  //  0003  RET	1	K3
      0x70020003,  //  0004  JMP		#0009
      0x88040100,  //  0005  GETMBR	R1	R0	K0
      0x1C040304,  //  0006  EQ	R1	R1	K4
      0x78060000,  //  0007  JMPF	R1	#0009
      0x80060A00,  //  0008  RET	1	K5
      0x60040018,  //  0009  GETGBL	R1	G24
      0x58080006,  //  000A  LDCONST	R2	K6
      0x880C0100,  //  000B  GETMBR	R3	R0	K0
      0x7C040400,  //  000C  CALL	R1	2
      0x80040200,  //  000D  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Partition_info_init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition_info,     /* shared constants */
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[60]) {  /* code */
      0x90020101,  //  0000  SETMBR	R0	K0	K1
      0x90020501,  //  0001  SETMBR	R0	K2	K1
      0x90020F01,  //  0002  SETMBR	R0	K7	K1
      0x90021101,  //  0003  SETMBR	R0	K8	K1
      0x9002130A,  //  0004  SETMBR	R0	K9	K10
      0x90021701,  //  0005  SETMBR	R0	K11	K1
      0x6008000E,  //  0006  GETGBL	R2	G14
      0x600C0015,  //  0007  GETGBL	R3	G21
      0x5C100200,  //  0008  MOVE	R4	R1
      0x7C080400,  //  0009  CALL	R2	2
      0x740A0000,  //  000A  JMPT	R2	#000C
      0x80000400,  //  000B  RET	0
      0x8C08030C,  //  000C  GETMET	R2	R1	K12
      0x58100001,  //  000D  LDCONST	R4	K1
      0x5814000D,  //  000E  LDCONST	R5	K13
      0x7C080600,  //  000F  CALL	R2	3
      0x540E50A9,  //  0010  LDINT	R3	20650
      0x1C0C0403,  //  0011  EQ	R3	R2	R3
      0x780E0022,  //  0012  JMPF	R3	#0036
      0x8C0C030C,  //  0013  GETMET	R3	R1	K12
      0x5814000D,  //  0014  LDCONST	R5	K13
      0x58180004,  //  0015  LDCONST	R6	K4
      0x7C0C0600,  //  0016  CALL	R3	3
      0x90020003,  //  0017  SETMBR	R0	K0	R3
      0x8C0C030C,  //  0018  GETMET	R3	R1	K12
      0x5814000E,  //  0019  LDCONST	R5	K14
      0x58180004,  //  001A  LDCONST	R6	K4
      0x7C0C0600,  //  001B  CALL	R3	3
      0x90020403,  //  001C  SETMBR	R0	K2	R3
      0x8C0C030C,  //  001D  GETMET	R3	R1	K12
      0x54160003,  //  001E  LDINT	R5	4
      0x541A0003,  //  001F  LDINT	R6	4
      0x7C0C0600,  //  0020  CALL	R3	3
      0x90020E03,  //  0021  SETMBR	R0	K7	R3
      0x8C0C030C,  //  0022  GETMET	R3	R1	K12
      0x54160007,  //  0023  LDINT	R5	8
      0x541A0003,  //  0024  LDINT	R6	4
      0x7C0C0600,  //  0025  CALL	R3	3
      0x90021003,  //  0026  SETMBR	R0	K8	R3
      0x8C0C010F,  //  0027  GETMET	R3	R0	K15
      0x5416000B,  //  0028  LDINT	R5	12
      0x541A001A,  //  0029  LDINT	R6	27
      0x40140A06,  //  002A  CONNECT	R5	R5	R6
      0x94140205,  //  002B  GETIDX	R5	R1	R5
      0x7C0C0400,  //  002C  CALL	R3	2
      0x8C0C0710,  //  002D  GETMET	R3	R3	K16
      0x7C0C0200,  //  002E  CALL	R3	1
      0x90021203,  //  002F  SETMBR	R0	K9	R3
      0x8C0C030C,  //  0030  GETMET	R3	R1	K12
      0x5416001B,  //  0031  LDINT	R5	28
      0x541A0003,  //  0032  LDINT	R6	4
      0x7C0C0600,  //  0033  CALL	R3	3
      0x90021603,  //  0034  SETMBR	R0	K11	R3
      0x70020004,  //  0035  JMP		#003B
      0x600C0018,  //  0036  GETGBL	R3	G24
      0x58100011,  //  0037  LDCONST	R4	K17
      0x5C140400,  //  0038  MOVE	R5	R2
      0x7C0C0400,  //  0039  CALL	R3	2
      0xB0062403,  //  003A  RAISE	1	K18	R3
      0x80000000,  //  003B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: subtype_to_string
********************************************************************/
be_local_closure(class_Partition_info_subtype_to_string,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition_info,     /* shared constants */
    &be_const_str_subtype_to_string,
    &be_const_str_solidified,
    ( &(const binstruction[87]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x1C040301,  //  0001  EQ	R1	R1	K1
      0x7806001A,  //  0002  JMPF	R1	#001E
      0x88040102,  //  0003  GETMBR	R1	R0	K2
      0x1C040301,  //  0004  EQ	R1	R1	K1
      0x78060001,  //  0005  JMPF	R1	#0008
      0x80062600,  //  0006  RET	1	K19
      0x70020014,  //  0007  JMP		#001D
      0x88040102,  //  0008  GETMBR	R1	R0	K2
      0x540A000F,  //  0009  LDINT	R2	16
      0x28040202,  //  000A  GE	R1	R1	R2
      0x7806000B,  //  000B  JMPF	R1	#0018
      0x88040102,  //  000C  GETMBR	R1	R0	K2
      0x540A001F,  //  000D  LDINT	R2	32
      0x14040202,  //  000E  LT	R1	R1	R2
      0x78060007,  //  000F  JMPF	R1	#0018
      0x60040008,  //  0010  GETGBL	R1	G8
      0x88080102,  //  0011  GETMBR	R2	R0	K2
      0x540E000F,  //  0012  LDINT	R3	16
      0x04080403,  //  0013  SUB	R2	R2	R3
      0x7C040200,  //  0014  CALL	R1	1
      0x00062801,  //  0015  ADD	R1	K20	R1
      0x80040200,  //  0016  RET	1	R1
      0x70020004,  //  0017  JMP		#001D
      0x88040102,  //  0018  GETMBR	R1	R0	K2
      0x540A001F,  //  0019  LDINT	R2	32
      0x1C040202,  //  001A  EQ	R1	R1	R2
      0x78060000,  //  001B  JMPF	R1	#001D
      0x80062A00,  //  001C  RET	1	K21
      0x70020033,  //  001D  JMP		#0052
      0x88040100,  //  001E  GETMBR	R1	R0	K0
      0x1C040304,  //  001F  EQ	R1	R1	K4
      0x78060030,  //  0020  JMPF	R1	#0052
      0x88040102,  //  0021  GETMBR	R1	R0	K2
      0x1C040301,  //  0022  EQ	R1	R1	K1
      0x78060001,  //  0023  JMPF	R1	#0026
      0x80062C00,  //  0024  RET	1	K22
      0x7002002B,  //  0025  JMP		#0052
      0x88040102,  //  0026  GETMBR	R1	R0	K2
      0x1C040304,  //  0027  EQ	R1	R1	K4
      0x78060001,  //  0028  JMPF	R1	#002B
      0x80062E00,  //  0029  RET	1	K23
      0x70020026,  //  002A  JMP		#0052
      0x88040102,  //  002B  GETMBR	R1	R0	K2
      0x1C04030D,  //  002C  EQ	R1	R1	K13
      0x78060001,  //  002D  JMPF	R1	#0030
      0x80063000,  //  002E  RET	1	K24
      0x70020021,  //  002F  JMP		#0052
      0x88040102,  //  0030  GETMBR	R1	R0	K2
      0x1C04030E,  //  0031  EQ	R1	R1	K14
      0x78060001,  //  0032  JMPF	R1	#0035
      0x80063200,  //  0033  RET	1	K25
      0x7002001C,  //  0034  JMP		#0052
      0x88040102,  //  0035  GETMBR	R1	R0	K2
      0x540A0003,  //  0036  LDINT	R2	4
      0x1C040202,  //  0037  EQ	R1	R1	R2
      0x78060001,  //  0038  JMPF	R1	#003B
      0x80063400,  //  0039  RET	1	K26
      0x70020016,  //  003A  JMP		#0052
      0x88040102,  //  003B  GETMBR	R1	R0	K2
      0x540A0004,  //  003C  LDINT	R2	5
      0x1C040202,  //  003D  EQ	R1	R1	R2
      0x78060001,  //  003E  JMPF	R1	#0041
      0x80063600,  //  003F  RET	1	K27
      0x70020010,  //  0040  JMP		#0052
      0x88040102,  //  0041  GETMBR	R1	R0	K2
      0x540A007F,  //  0042  LDINT	R2	128
      0x1C040202,  //  0043  EQ	R1	R1	R2
      0x78060001,  //  0044  JMPF	R1	#0047
      0x80063800,  //  0045  RET	1	K28
      0x7002000A,  //  0046  JMP		#0052
      0x88040102,  //  0047  GETMBR	R1	R0	K2
      0x540A0080,  //  0048  LDINT	R2	129
      0x1C040202,  //  0049  EQ	R1	R1	R2
      0x78060001,  //  004A  JMPF	R1	#004D
      0x80063A00,  //  004B  RET	1	K29
      0x70020004,  //  004C  JMP		#0052
      0x88040102,  //  004D  GETMBR	R1	R0	K2
      0x540A0081,  //  004E  LDINT	R2	130
      0x1C040202,  //  004F  EQ	R1	R1	R2
      0x78060000,  //  0050  JMPF	R1	#0052
      0x80063C00,  //  0051  RET	1	K30
      0x60040018,  //  0052  GETGBL	R1	G24
      0x58080006,  //  0053  LDCONST	R2	K6
      0x880C0102,  //  0054  GETMBR	R3	R0	K2
      0x7C040400,  //  0055  CALL	R1	2
      0x80040200,  //  0056  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_Partition_info_tostring,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition_info,     /* shared constants */
    &be_const_str_tostring,
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x8C04011F,  //  0000  GETMET	R1	R0	K31
      0x7C040200,  //  0001  CALL	R1	1
      0x8C080120,  //  0002  GETMET	R2	R0	K32
      0x7C080200,  //  0003  CALL	R2	1
      0x200C030A,  //  0004  NE	R3	R1	K10
      0x780E0002,  //  0005  JMPF	R3	#0009
      0x000E4201,  //  0006  ADD	R3	K33	R1
      0x000C0722,  //  0007  ADD	R3	R3	K34
      0x5C040600,  //  0008  MOVE	R1	R3
      0x200C050A,  //  0009  NE	R3	R2	K10
      0x780E0002,  //  000A  JMPF	R3	#000E
      0x000E4202,  //  000B  ADD	R3	K33	R2
      0x000C0722,  //  000C  ADD	R3	R3	K34
      0x5C080600,  //  000D  MOVE	R2	R3
      0x600C0018,  //  000E  GETGBL	R3	G24
      0x58100023,  //  000F  LDCONST	R4	K35
      0x88140100,  //  0010  GETMBR	R5	R0	K0
      0x5C180200,  //  0011  MOVE	R6	R1
      0x881C0102,  //  0012  GETMBR	R7	R0	K2
      0x5C200400,  //  0013  MOVE	R8	R2
      0x88240107,  //  0014  GETMBR	R9	R0	K7
      0x88280108,  //  0015  GETMBR	R10	R0	K8
      0x882C0109,  //  0016  GETMBR	R11	R0	K9
      0x8830010B,  //  0017  GETMBR	R12	R0	K11
      0x7C0C1200,  //  0018  CALL	R3	9
      0x80040600,  //  0019  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_ota
********************************************************************/
be_local_closure(class_Partition_info_is_ota,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition_info,     /* shared constants */
    &be_const_str_is_ota,
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x88040102,  //  0000  GETMBR	R1	R0	K2
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x1C080501,  //  0002  EQ	R2	R2	K1
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
** Solidified function: tobytes
********************************************************************/
be_local_closure(class_Partition_info_tobytes,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition_info,     /* shared constants */
    &be_const_str_tobytes,
    &be_const_str_solidified,
    ( &(const binstruction[39]) {  /* code */
      0x60040015,  //  0000  GETGBL	R1	G21
      0x58080024,  //  0001  LDCONST	R2	K36
      0x7C040200,  //  0002  CALL	R1	1
      0x8C080325,  //  0003  GETMET	R2	R1	K37
      0x5412001F,  //  0004  LDINT	R4	32
      0x7C080400,  //  0005  CALL	R2	2
      0x8C080525,  //  0006  GETMET	R2	R2	K37
      0x5810000D,  //  0007  LDCONST	R4	K13
      0x7C080400,  //  0008  CALL	R2	2
      0x8C080326,  //  0009  GETMET	R2	R1	K38
      0x88100100,  //  000A  GETMBR	R4	R0	K0
      0x58140004,  //  000B  LDCONST	R5	K4
      0x7C080600,  //  000C  CALL	R2	3
      0x8C080326,  //  000D  GETMET	R2	R1	K38
      0x88100102,  //  000E  GETMBR	R4	R0	K2
      0x58140004,  //  000F  LDCONST	R5	K4
      0x7C080600,  //  0010  CALL	R2	3
      0x8C080326,  //  0011  GETMET	R2	R1	K38
      0x88100107,  //  0012  GETMBR	R4	R0	K7
      0x54160003,  //  0013  LDINT	R5	4
      0x7C080600,  //  0014  CALL	R2	3
      0x8C080326,  //  0015  GETMET	R2	R1	K38
      0x88100108,  //  0016  GETMBR	R4	R0	K8
      0x54160003,  //  0017  LDINT	R5	4
      0x7C080600,  //  0018  CALL	R2	3
      0x60080015,  //  0019  GETGBL	R2	G21
      0x7C080000,  //  001A  CALL	R2	0
      0x8C080527,  //  001B  GETMET	R2	R2	K39
      0x88100109,  //  001C  GETMBR	R4	R0	K9
      0x7C080400,  //  001D  CALL	R2	2
      0x8C0C0525,  //  001E  GETMET	R3	R2	K37
      0x5416000F,  //  001F  LDINT	R5	16
      0x7C0C0400,  //  0020  CALL	R3	2
      0x00040202,  //  0021  ADD	R1	R1	R2
      0x8C0C0326,  //  0022  GETMET	R3	R1	K38
      0x8814010B,  //  0023  GETMBR	R5	R0	K11
      0x541A0003,  //  0024  LDINT	R6	4
      0x7C0C0600,  //  0025  CALL	R3	3
      0x80040200,  //  0026  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_trailing_zeroes
********************************************************************/
be_local_closure(class_Partition_info_remove_trailing_zeroes,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition_info,     /* shared constants */
    &be_const_str_remove_trailing_zeroes,
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x58040028,  //  0000  LDCONST	R1	K40
      0x6008000C,  //  0001  GETGBL	R2	G12
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C080200,  //  0003  CALL	R2	1
      0x580C0001,  //  0004  LDCONST	R3	K1
      0x14100602,  //  0005  LT	R4	R3	R2
      0x78120007,  //  0006  JMPF	R4	#000F
      0x5411FFFE,  //  0007  LDINT	R4	-1
      0x04100803,  //  0008  SUB	R4	R4	R3
      0x94100004,  //  0009  GETIDX	R4	R0	R4
      0x20100901,  //  000A  NE	R4	R4	K1
      0x78120000,  //  000B  JMPF	R4	#000D
      0x70020001,  //  000C  JMP		#000F
      0x000C0704,  //  000D  ADD	R3	R3	K4
      0x7001FFF5,  //  000E  JMP		#0005
      0x24100701,  //  000F  GT	R4	R3	K1
      0x78120005,  //  0010  JMPF	R4	#0017
      0x8C100125,  //  0011  GETMET	R4	R0	K37
      0x6018000C,  //  0012  GETGBL	R6	G12
      0x5C1C0000,  //  0013  MOVE	R7	R0
      0x7C180200,  //  0014  CALL	R6	1
      0x04180C03,  //  0015  SUB	R6	R6	R3
      0x7C100400,  //  0016  CALL	R4	2
      0x80040000,  //  0017  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_spiffs
********************************************************************/
be_local_closure(class_Partition_info_is_spiffs,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition_info,     /* shared constants */
    &be_const_str_is_spiffs,
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x1C040304,  //  0001  EQ	R1	R1	K4
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
be_local_closure(class_Partition_info_get_image_size,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Partition_info,     /* shared constants */
    &be_const_str_get_image_size,
    &be_const_str_solidified,
    ( &(const binstruction[85]) {  /* code */
      0xA4065200,  //  0000  IMPORT	R1	K41
      0x8C08012A,  //  0001  GETMET	R2	R0	K42
      0x7C080200,  //  0002  CALL	R2	1
      0x4C0C0000,  //  0003  LDNIL	R3
      0x1C080403,  //  0004  EQ	R2	R2	R3
      0x780A0004,  //  0005  JMPF	R2	#000B
      0x8C08012B,  //  0006  GETMET	R2	R0	K43
      0x7C080200,  //  0007  CALL	R2	1
      0x740A0001,  //  0008  JMPT	R2	#000B
      0x5409FFFE,  //  0009  LDINT	R2	-1
      0x80040400,  //  000A  RET	1	R2
      0xA802003A,  //  000B  EXBLK	0	#0047
      0x88080107,  //  000C  GETMBR	R2	R0	K7
      0x880C0108,  //  000D  GETMBR	R3	R0	K8
      0x8C10032C,  //  000E  GETMET	R4	R1	K44
      0x5C180400,  //  000F  MOVE	R6	R2
      0x581C0004,  //  0010  LDCONST	R7	K4
      0x7C100600,  //  0011  CALL	R4	3
      0x8C10090C,  //  0012  GETMET	R4	R4	K12
      0x58180001,  //  0013  LDCONST	R6	K1
      0x581C0004,  //  0014  LDCONST	R7	K4
      0x7C100600,  //  0015  CALL	R4	3
      0x541600E8,  //  0016  LDINT	R5	233
      0x20140805,  //  0017  NE	R5	R4	R5
      0x78160002,  //  0018  JMPF	R5	#001C
      0x5415FFFE,  //  0019  LDINT	R5	-1
      0xA8040001,  //  001A  EXBLK	1	1
      0x80040A00,  //  001B  RET	1	R5
      0x8C14032C,  //  001C  GETMET	R5	R1	K44
      0x001C0504,  //  001D  ADD	R7	R2	K4
      0x58200004,  //  001E  LDCONST	R8	K4
      0x7C140600,  //  001F  CALL	R5	3
      0x8C140B0C,  //  0020  GETMET	R5	R5	K12
      0x581C0001,  //  0021  LDCONST	R7	K1
      0x58200004,  //  0022  LDCONST	R8	K4
      0x7C140600,  //  0023  CALL	R5	3
      0x541A001F,  //  0024  LDINT	R6	32
      0x00180406,  //  0025  ADD	R6	R2	R6
      0x581C0001,  //  0026  LDCONST	R7	K1
      0x14200E05,  //  0027  LT	R8	R7	R5
      0x78220017,  //  0028  JMPF	R8	#0041
      0x8C20032C,  //  0029  GETMET	R8	R1	K44
      0x542A0007,  //  002A  LDINT	R10	8
      0x04280C0A,  //  002B  SUB	R10	R6	R10
      0x542E0007,  //  002C  LDINT	R11	8
      0x7C200600,  //  002D  CALL	R8	3
      0x8C24110C,  //  002E  GETMET	R9	R8	K12
      0x582C0001,  //  002F  LDCONST	R11	K1
      0x54320003,  //  0030  LDINT	R12	4
      0x7C240600,  //  0031  CALL	R9	3
      0x8C28110C,  //  0032  GETMET	R10	R8	K12
      0x54320003,  //  0033  LDINT	R12	4
      0x54360003,  //  0034  LDINT	R13	4
      0x7C280600,  //  0035  CALL	R10	3
      0x542E0007,  //  0036  LDINT	R11	8
      0x002C140B,  //  0037  ADD	R11	R10	R11
      0x00180C0B,  //  0038  ADD	R6	R6	R11
      0x002C0403,  //  0039  ADD	R11	R2	R3
      0x282C0C0B,  //  003A  GE	R11	R6	R11
      0x782E0002,  //  003B  JMPF	R11	#003F
      0x542DFFFE,  //  003C  LDINT	R11	-1
      0xA8040001,  //  003D  EXBLK	1	1
      0x80041600,  //  003E  RET	1	R11
      0x001C0F04,  //  003F  ADD	R7	R7	K4
      0x7001FFE5,  //  0040  JMP		#0027
      0x04200C02,  //  0041  SUB	R8	R6	R2
      0x00201104,  //  0042  ADD	R8	R8	K4
      0xA8040001,  //  0043  EXBLK	1	1
      0x80041000,  //  0044  RET	1	R8
      0xA8040001,  //  0045  EXBLK	1	1
      0x7002000C,  //  0046  JMP		#0054
      0xAC080002,  //  0047  CATCH	R2	0	2
      0x70020009,  //  0048  JMP		#0053
      0xB8125A00,  //  0049  GETNGBL	R4	K45
      0x8C10092E,  //  004A  GETMET	R4	R4	K46
      0x001A5E02,  //  004B  ADD	R6	K47	R2
      0x00180D30,  //  004C  ADD	R6	R6	K48
      0x00180C03,  //  004D  ADD	R6	R6	R3
      0x581C000D,  //  004E  LDCONST	R7	K13
      0x7C100600,  //  004F  CALL	R4	3
      0x5411FFFE,  //  0050  LDINT	R4	-1
      0x80040800,  //  0051  RET	1	R4
      0x70020000,  //  0052  JMP		#0054
      0xB0080000,  //  0053  RAISE	2	R0	R0
      0x80000000,  //  0054  RET	0
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
    be_nested_map(16,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(is_factory, -1), be_const_closure(class_Partition_info_is_factory_closure) },
        { be_const_key(get_image_size, -1), be_const_closure(class_Partition_info_get_image_size_closure) },
        { be_const_key(is_spiffs, -1), be_const_closure(class_Partition_info_is_spiffs_closure) },
        { be_const_key(init, -1), be_const_closure(class_Partition_info_init_closure) },
        { be_const_key(type, 10), be_const_var(0) },
        { be_const_key(tostring, -1), be_const_closure(class_Partition_info_tostring_closure) },
        { be_const_key(flags, -1), be_const_var(5) },
        { be_const_key(tobytes, -1), be_const_closure(class_Partition_info_tobytes_closure) },
        { be_const_key(sz, -1), be_const_var(3) },
        { be_const_key(remove_trailing_zeroes, 2), be_const_static_closure(class_Partition_info_remove_trailing_zeroes_closure) },
        { be_const_key(label, 11), be_const_var(4) },
        { be_const_key(subtype, 14), be_const_var(1) },
        { be_const_key(is_ota, 6), be_const_closure(class_Partition_info_is_ota_closure) },
        { be_const_key(type_to_string, 4), be_const_closure(class_Partition_info_type_to_string_closure) },
        { be_const_key(subtype_to_string, -1), be_const_closure(class_Partition_info_subtype_to_string_closure) },
        { be_const_key(start, 1), be_const_var(2) },
    })),
    (bstring*) &be_const_str_Partition_info
);

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(init,   /* name */
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
        { be_const_key(init, -1), be_const_closure(init_closure) },
    }))
);
BE_EXPORT_VARIABLE be_define_const_native_module(partition_core);
/********************************************************************/
/********************************************************************/
/* End of solidification */
