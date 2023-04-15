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
    /* K1   */  be_nested_str_weak(),
    /* K2   */  be_nested_str_weak(endpoint),
    /* K3   */  be_nested_str_weak(format),
    /* K4   */  be_nested_str_weak(_X5B_X2502X_X5D),
    /* K5   */  be_nested_str_weak(_X5B_X2A_X2A_X5D),
    /* K6   */  be_nested_str_weak(cluster),
    /* K7   */  be_nested_str_weak(_X2504X_X2F),
    /* K8   */  be_nested_str_weak(_X2A_X2A_X2A_X2A_X2F),
    /* K9   */  be_nested_str_weak(attribute),
    /* K10  */  be_nested_str_weak(_X2504X),
    /* K11  */  be_nested_str_weak(command),
    /* K12  */  be_nested_str_weak(_X2A_X2A_X2A_X2A),
    /* K13  */  be_nested_str_weak(Exception_X3E_X20),
    /* K14  */  be_nested_str_weak(_X2C_X20),
    }),
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[75]) {  /* code */
      0xA802003A,  //  0000  EXBLK	0	#003C
      0xA4060000,  //  0001  IMPORT	R1	K0
      0x58080001,  //  0002  LDCONST	R2	K1
      0x880C0102,  //  0003  GETMBR	R3	R0	K2
      0x4C100000,  //  0004  LDNIL	R4
      0x200C0604,  //  0005  NE	R3	R3	R4
      0x780E0004,  //  0006  JMPF	R3	#000C
      0x8C0C0303,  //  0007  GETMET	R3	R1	K3
      0x58140004,  //  0008  LDCONST	R5	K4
      0x88180102,  //  0009  GETMBR	R6	R0	K2
      0x7C0C0600,  //  000A  CALL	R3	3
      0x70020000,  //  000B  JMP		#000D
      0x580C0005,  //  000C  LDCONST	R3	K5
      0x00080403,  //  000D  ADD	R2	R2	R3
      0x880C0106,  //  000E  GETMBR	R3	R0	K6
      0x4C100000,  //  000F  LDNIL	R4
      0x200C0604,  //  0010  NE	R3	R3	R4
      0x780E0004,  //  0011  JMPF	R3	#0017
      0x8C0C0303,  //  0012  GETMET	R3	R1	K3
      0x58140007,  //  0013  LDCONST	R5	K7
      0x88180106,  //  0014  GETMBR	R6	R0	K6
      0x7C0C0600,  //  0015  CALL	R3	3
      0x70020000,  //  0016  JMP		#0018
      0x580C0008,  //  0017  LDCONST	R3	K8
      0x00080403,  //  0018  ADD	R2	R2	R3
      0x880C0109,  //  0019  GETMBR	R3	R0	K9
      0x4C100000,  //  001A  LDNIL	R4
      0x200C0604,  //  001B  NE	R3	R3	R4
      0x780E0004,  //  001C  JMPF	R3	#0022
      0x8C0C0303,  //  001D  GETMET	R3	R1	K3
      0x5814000A,  //  001E  LDCONST	R5	K10
      0x88180109,  //  001F  GETMBR	R6	R0	K9
      0x7C0C0600,  //  0020  CALL	R3	3
      0x70020000,  //  0021  JMP		#0023
      0x580C0001,  //  0022  LDCONST	R3	K1
      0x00080403,  //  0023  ADD	R2	R2	R3
      0x880C010B,  //  0024  GETMBR	R3	R0	K11
      0x4C100000,  //  0025  LDNIL	R4
      0x200C0604,  //  0026  NE	R3	R3	R4
      0x780E0004,  //  0027  JMPF	R3	#002D
      0x8C0C0303,  //  0028  GETMET	R3	R1	K3
      0x5814000A,  //  0029  LDCONST	R5	K10
      0x8818010B,  //  002A  GETMBR	R6	R0	K11
      0x7C0C0600,  //  002B  CALL	R3	3
      0x70020000,  //  002C  JMP		#002E
      0x580C0001,  //  002D  LDCONST	R3	K1
      0x00080403,  //  002E  ADD	R2	R2	R3
      0x880C0109,  //  002F  GETMBR	R3	R0	K9
      0x4C100000,  //  0030  LDNIL	R4
      0x1C0C0604,  //  0031  EQ	R3	R3	R4
      0x780E0004,  //  0032  JMPF	R3	#0038
      0x880C010B,  //  0033  GETMBR	R3	R0	K11
      0x4C100000,  //  0034  LDNIL	R4
      0x1C0C0604,  //  0035  EQ	R3	R3	R4
      0x780E0000,  //  0036  JMPF	R3	#0038
      0x0008050C,  //  0037  ADD	R2	R2	K12
      0xA8040001,  //  0038  EXBLK	1	1
      0x80040400,  //  0039  RET	1	R2
      0xA8040001,  //  003A  EXBLK	1	1
      0x7002000D,  //  003B  JMP		#004A
      0xAC040002,  //  003C  CATCH	R1	0	2
      0x7002000A,  //  003D  JMP		#0049
      0x600C0008,  //  003E  GETGBL	R3	G8
      0x5C100200,  //  003F  MOVE	R4	R1
      0x7C0C0200,  //  0040  CALL	R3	1
      0x000E1A03,  //  0041  ADD	R3	K13	R3
      0x000C070E,  //  0042  ADD	R3	R3	K14
      0x60100008,  //  0043  GETGBL	R4	G8
      0x5C140400,  //  0044  MOVE	R5	R2
      0x7C100200,  //  0045  CALL	R4	1
      0x000C0604,  //  0046  ADD	R3	R3	R4
      0x80040600,  //  0047  RET	1	R3
      0x70020000,  //  0048  JMP		#004A
      0xB0080000,  //  0049  RAISE	2	R0	R0
      0x80000000,  //  004A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Path
********************************************************************/
be_local_class(Matter_Path,
    6,
    NULL,
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(log, 4), be_const_var(5) },
        { be_const_key_weak(command, 2), be_const_var(3) },
        { be_const_key_weak(status, -1), be_const_var(4) },
        { be_const_key_weak(tostring, -1), be_const_closure(Matter_Path_tostring_closure) },
        { be_const_key_weak(cluster, 5), be_const_var(1) },
        { be_const_key_weak(endpoint, -1), be_const_var(0) },
        { be_const_key_weak(attribute, -1), be_const_var(2) },
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
