/* Solidification of Matter_Path.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Path;

/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(Matter_Path_tostring,   /* name */
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
    /* K0   */  be_nested_str_weak(),
    /* K1   */  be_nested_str_weak(endpoint),
    /* K2   */  be_nested_str_weak(_X5B_X2502X_X5D),
    /* K3   */  be_nested_str_weak(_X5B_X2A_X2A_X5D),
    /* K4   */  be_nested_str_weak(cluster),
    /* K5   */  be_nested_str_weak(_X2504X_X2F),
    /* K6   */  be_nested_str_weak(_X2A_X2A_X2A_X2A_X2F),
    /* K7   */  be_nested_str_weak(attribute),
    /* K8   */  be_nested_str_weak(_X2504X),
    /* K9   */  be_nested_str_weak(command),
    /* K10  */  be_nested_str_weak(fabric_filtered),
    /* K11  */  be_nested_str_weak(_X21),
    /* K12  */  be_nested_str_weak(_X2A_X2A_X2A_X2A),
    /* K13  */  be_nested_str_weak(Exception_X3E_X20),
    /* K14  */  be_nested_str_weak(_X2C_X20),
    }),
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[77]) {  /* code */
      0xA802003C,  //  0000  EXBLK	0	#003E
      0x58040000,  //  0001  LDCONST	R1	K0
      0x88080101,  //  0002  GETMBR	R2	R0	K1
      0x4C0C0000,  //  0003  LDNIL	R3
      0x20080403,  //  0004  NE	R2	R2	R3
      0x780A0004,  //  0005  JMPF	R2	#000B
      0x60080018,  //  0006  GETGBL	R2	G24
      0x580C0002,  //  0007  LDCONST	R3	K2
      0x88100101,  //  0008  GETMBR	R4	R0	K1
      0x7C080400,  //  0009  CALL	R2	2
      0x70020000,  //  000A  JMP		#000C
      0x58080003,  //  000B  LDCONST	R2	K3
      0x00040202,  //  000C  ADD	R1	R1	R2
      0x88080104,  //  000D  GETMBR	R2	R0	K4
      0x4C0C0000,  //  000E  LDNIL	R3
      0x20080403,  //  000F  NE	R2	R2	R3
      0x780A0004,  //  0010  JMPF	R2	#0016
      0x60080018,  //  0011  GETGBL	R2	G24
      0x580C0005,  //  0012  LDCONST	R3	K5
      0x88100104,  //  0013  GETMBR	R4	R0	K4
      0x7C080400,  //  0014  CALL	R2	2
      0x70020000,  //  0015  JMP		#0017
      0x58080006,  //  0016  LDCONST	R2	K6
      0x00040202,  //  0017  ADD	R1	R1	R2
      0x88080107,  //  0018  GETMBR	R2	R0	K7
      0x4C0C0000,  //  0019  LDNIL	R3
      0x20080403,  //  001A  NE	R2	R2	R3
      0x780A0004,  //  001B  JMPF	R2	#0021
      0x60080018,  //  001C  GETGBL	R2	G24
      0x580C0008,  //  001D  LDCONST	R3	K8
      0x88100107,  //  001E  GETMBR	R4	R0	K7
      0x7C080400,  //  001F  CALL	R2	2
      0x70020000,  //  0020  JMP		#0022
      0x58080000,  //  0021  LDCONST	R2	K0
      0x00040202,  //  0022  ADD	R1	R1	R2
      0x88080109,  //  0023  GETMBR	R2	R0	K9
      0x4C0C0000,  //  0024  LDNIL	R3
      0x20080403,  //  0025  NE	R2	R2	R3
      0x780A0004,  //  0026  JMPF	R2	#002C
      0x60080018,  //  0027  GETGBL	R2	G24
      0x580C0008,  //  0028  LDCONST	R3	K8
      0x88100109,  //  0029  GETMBR	R4	R0	K9
      0x7C080400,  //  002A  CALL	R2	2
      0x70020000,  //  002B  JMP		#002D
      0x58080000,  //  002C  LDCONST	R2	K0
      0x00040202,  //  002D  ADD	R1	R1	R2
      0x8808010A,  //  002E  GETMBR	R2	R0	K10
      0x780A0000,  //  002F  JMPF	R2	#0031
      0x0004030B,  //  0030  ADD	R1	R1	K11
      0x88080107,  //  0031  GETMBR	R2	R0	K7
      0x4C0C0000,  //  0032  LDNIL	R3
      0x1C080403,  //  0033  EQ	R2	R2	R3
      0x780A0004,  //  0034  JMPF	R2	#003A
      0x88080109,  //  0035  GETMBR	R2	R0	K9
      0x4C0C0000,  //  0036  LDNIL	R3
      0x1C080403,  //  0037  EQ	R2	R2	R3
      0x780A0000,  //  0038  JMPF	R2	#003A
      0x0004030C,  //  0039  ADD	R1	R1	K12
      0xA8040001,  //  003A  EXBLK	1	1
      0x80040200,  //  003B  RET	1	R1
      0xA8040001,  //  003C  EXBLK	1	1
      0x7002000D,  //  003D  JMP		#004C
      0xAC040002,  //  003E  CATCH	R1	0	2
      0x7002000A,  //  003F  JMP		#004B
      0x600C0008,  //  0040  GETGBL	R3	G8
      0x5C100200,  //  0041  MOVE	R4	R1
      0x7C0C0200,  //  0042  CALL	R3	1
      0x000E1A03,  //  0043  ADD	R3	K13	R3
      0x000C070E,  //  0044  ADD	R3	R3	K14
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
** Solidified function: reset
********************************************************************/
be_local_closure(Matter_Path_reset,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(endpoint),
    /* K1   */  be_nested_str_weak(cluster),
    /* K2   */  be_nested_str_weak(attribute),
    /* K3   */  be_nested_str_weak(fabric_filtered),
    /* K4   */  be_nested_str_weak(command),
    /* K5   */  be_nested_str_weak(status),
    /* K6   */  be_nested_str_weak(log),
    /* K7   */  be_nested_str_weak(msg),
    }),
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
** Solidified class: Matter_Path
********************************************************************/
be_local_class(Matter_Path,
    8,
    NULL,
    be_nested_map(10,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(attribute, -1), be_const_var(2) },
        { be_const_key_weak(log, 0), be_const_var(6) },
        { be_const_key_weak(fabric_filtered, 6), be_const_var(3) },
        { be_const_key_weak(command, -1), be_const_var(4) },
        { be_const_key_weak(msg, -1), be_const_var(7) },
        { be_const_key_weak(tostring, -1), be_const_closure(Matter_Path_tostring_closure) },
        { be_const_key_weak(reset, -1), be_const_closure(Matter_Path_reset_closure) },
        { be_const_key_weak(cluster, -1), be_const_var(1) },
        { be_const_key_weak(endpoint, 3), be_const_var(0) },
        { be_const_key_weak(status, -1), be_const_var(5) },
    })),
    be_str_weak(Matter_Path)
);
/*******************************************************************/

void be_load_Matter_Path_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Path);
    be_setglobal(vm, "Matter_Path");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
