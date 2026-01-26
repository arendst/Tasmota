/* Solidification of Matter_Path_0.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_Path' ktab size: 19, total: 30 (saved 88 bytes)
static const bvalue be_ktab_class_Matter_Path[19] = {
  /* K0   */  be_nested_str_weak(endpoint),
  /* K1   */  be_nested_str_weak(cluster),
  /* K2   */  be_nested_str_weak(attribute),
  /* K3   */  be_nested_str_weak(fabric_filtered),
  /* K4   */  be_nested_str_weak(command),
  /* K5   */  be_nested_str_weak(status),
  /* K6   */  be_nested_str_weak(log),
  /* K7   */  be_nested_str_weak(msg),
  /* K8   */  be_nested_str_weak(),
  /* K9   */  be_nested_str_weak(_X5B_X2502X_X5D),
  /* K10  */  be_nested_str_weak(_X5B_X2A_X2A_X5D),
  /* K11  */  be_nested_str_weak(_X2504X_X2F),
  /* K12  */  be_nested_str_weak(_X2A_X2A_X2A_X2A_X2F),
  /* K13  */  be_nested_str_weak(_X2504X),
  /* K14  */  be_nested_str_weak(_X21),
  /* K15  */  be_nested_str_weak(_X2A_X2A_X2A_X2A),
  /* K16  */  be_nested_str_weak(Exception_X3E_X20),
  /* K17  */  be_nested_str_weak(_X2C_X20),
  /* K18  */  be_nested_str_weak(reset),
};


extern const bclass be_class_Matter_Path;

/********************************************************************
** Solidified function: reset
********************************************************************/
be_local_closure(class_Matter_Path_reset,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Path,     /* shared constants */
    be_str_weak(reset),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x4C040000,  //  0000  LDNIL	R1
      0x90020001,  //  0001  SETMBR	R0	K0	R1
      0x90020201,  //  0002  SETMBR	R0	K1	R1
      0x90020401,  //  0003  SETMBR	R0	K2	R1
      0x90020601,  //  0004  SETMBR	R0	K3	R1
      0x90020801,  //  0005  SETMBR	R0	K4	R1
      0x90020A01,  //  0006  SETMBR	R0	K5	R1
      0x90020C01,  //  0007  SETMBR	R0	K6	R1
      0x90020E01,  //  0008  SETMBR	R0	K7	R1
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_Path_init,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Path,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x90020202,  //  0001  SETMBR	R0	K1	R2
      0x90020403,  //  0002  SETMBR	R0	K2	R3
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_Matter_Path_tostring,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Path,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[77]) {  /* code */
      0xA802003C,  //  0000  EXBLK	0	#003E
      0x58040008,  //  0001  LDCONST	R1	K8
      0x88080100,  //  0002  GETMBR	R2	R0	K0
      0x4C0C0000,  //  0003  LDNIL	R3
      0x20080403,  //  0004  NE	R2	R2	R3
      0x780A0004,  //  0005  JMPF	R2	#000B
      0x60080018,  //  0006  GETGBL	R2	G24
      0x580C0009,  //  0007  LDCONST	R3	K9
      0x88100100,  //  0008  GETMBR	R4	R0	K0
      0x7C080400,  //  0009  CALL	R2	2
      0x70020000,  //  000A  JMP		#000C
      0x5808000A,  //  000B  LDCONST	R2	K10
      0x00040202,  //  000C  ADD	R1	R1	R2
      0x88080101,  //  000D  GETMBR	R2	R0	K1
      0x4C0C0000,  //  000E  LDNIL	R3
      0x20080403,  //  000F  NE	R2	R2	R3
      0x780A0004,  //  0010  JMPF	R2	#0016
      0x60080018,  //  0011  GETGBL	R2	G24
      0x580C000B,  //  0012  LDCONST	R3	K11
      0x88100101,  //  0013  GETMBR	R4	R0	K1
      0x7C080400,  //  0014  CALL	R2	2
      0x70020000,  //  0015  JMP		#0017
      0x5808000C,  //  0016  LDCONST	R2	K12
      0x00040202,  //  0017  ADD	R1	R1	R2
      0x88080102,  //  0018  GETMBR	R2	R0	K2
      0x4C0C0000,  //  0019  LDNIL	R3
      0x20080403,  //  001A  NE	R2	R2	R3
      0x780A0004,  //  001B  JMPF	R2	#0021
      0x60080018,  //  001C  GETGBL	R2	G24
      0x580C000D,  //  001D  LDCONST	R3	K13
      0x88100102,  //  001E  GETMBR	R4	R0	K2
      0x7C080400,  //  001F  CALL	R2	2
      0x70020000,  //  0020  JMP		#0022
      0x58080008,  //  0021  LDCONST	R2	K8
      0x00040202,  //  0022  ADD	R1	R1	R2
      0x88080104,  //  0023  GETMBR	R2	R0	K4
      0x4C0C0000,  //  0024  LDNIL	R3
      0x20080403,  //  0025  NE	R2	R2	R3
      0x780A0004,  //  0026  JMPF	R2	#002C
      0x60080018,  //  0027  GETGBL	R2	G24
      0x580C000D,  //  0028  LDCONST	R3	K13
      0x88100104,  //  0029  GETMBR	R4	R0	K4
      0x7C080400,  //  002A  CALL	R2	2
      0x70020000,  //  002B  JMP		#002D
      0x58080008,  //  002C  LDCONST	R2	K8
      0x00040202,  //  002D  ADD	R1	R1	R2
      0x88080103,  //  002E  GETMBR	R2	R0	K3
      0x780A0000,  //  002F  JMPF	R2	#0031
      0x0004030E,  //  0030  ADD	R1	R1	K14
      0x88080102,  //  0031  GETMBR	R2	R0	K2
      0x4C0C0000,  //  0032  LDNIL	R3
      0x1C080403,  //  0033  EQ	R2	R2	R3
      0x780A0004,  //  0034  JMPF	R2	#003A
      0x88080104,  //  0035  GETMBR	R2	R0	K4
      0x4C0C0000,  //  0036  LDNIL	R3
      0x1C080403,  //  0037  EQ	R2	R2	R3
      0x780A0000,  //  0038  JMPF	R2	#003A
      0x0004030F,  //  0039  ADD	R1	R1	K15
      0xA8040001,  //  003A  EXBLK	1	1
      0x80040200,  //  003B  RET	1	R1
      0xA8040001,  //  003C  EXBLK	1	1
      0x7002000D,  //  003D  JMP		#004C
      0xAC040002,  //  003E  CATCH	R1	0	2
      0x7002000A,  //  003F  JMP		#004B
      0x600C0008,  //  0040  GETGBL	R3	G8
      0x5C100200,  //  0041  MOVE	R4	R1
      0x7C0C0200,  //  0042  CALL	R3	1
      0x000E2003,  //  0043  ADD	R3	K16	R3
      0x000C0711,  //  0044  ADD	R3	R3	K17
      0x60100008,  //  0045  GETGBL	R4	G8
      0x5C140400,  //  0046  MOVE	R5	R2
      0x7C100200,  //  0047  CALL	R4	1
      0x000C0604,  //  0048  ADD	R3	R3	R4
      0x80040600,  //  0049  RET	1	R3
      0x70020000,  //  004A  JMP		#004C
      0xB0080000,  //  004B  RAISE	2	R0	R0
      0x80000000,  //  004C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: copy
********************************************************************/
be_local_closure(class_Matter_Path_copy,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Path,     /* shared constants */
    be_str_weak(copy),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x8C080112,  //  0000  GETMET	R2	R0	K18
      0x7C080200,  //  0001  CALL	R2	1
      0x88080300,  //  0002  GETMBR	R2	R1	K0
      0x90020002,  //  0003  SETMBR	R0	K0	R2
      0x88080301,  //  0004  GETMBR	R2	R1	K1
      0x90020202,  //  0005  SETMBR	R0	K1	R2
      0x88080302,  //  0006  GETMBR	R2	R1	K2
      0x90020402,  //  0007  SETMBR	R0	K2	R2
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Path
********************************************************************/
be_local_class(Matter_Path,
    8,
    NULL,
    be_nested_map(12,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(reset, -1), be_const_closure(class_Matter_Path_reset_closure) },
        { be_const_key_weak(cluster, 5), be_const_var(1) },
        { be_const_key_weak(fabric_filtered, 6), be_const_var(3) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_Path_init_closure) },
        { be_const_key_weak(endpoint, -1), be_const_var(0) },
        { be_const_key_weak(tostring, -1), be_const_closure(class_Matter_Path_tostring_closure) },
        { be_const_key_weak(command, 10), be_const_var(4) },
        { be_const_key_weak(status, -1), be_const_var(5) },
        { be_const_key_weak(copy, -1), be_const_closure(class_Matter_Path_copy_closure) },
        { be_const_key_weak(log, -1), be_const_var(6) },
        { be_const_key_weak(msg, -1), be_const_var(7) },
        { be_const_key_weak(attribute, -1), be_const_var(2) },
    })),
    be_str_weak(Matter_Path)
);
/********************************************************************/
/* End of solidification */
