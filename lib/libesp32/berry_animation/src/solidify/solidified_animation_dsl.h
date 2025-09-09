/* Solidification of animation_dsl.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
extern const bclass be_class_ExpressionResult;
// compact class 'ExpressionResult' ktab size: 13, total: 38 (saved 200 bytes)
static const bvalue be_ktab_class_ExpressionResult[13] = {
  /* K0   */  be_nested_str_weak(instance_for_validation),
  /* K1   */  be_nested_str_weak(instance_X3D_X25s),
  /* K2   */  be_nested_str_weak(instance_X3Dnil),
  /* K3   */  be_nested_str_weak(ExpressionResult_X28expr_X3D_X27_X25s_X27_X2C_X20funcs_X3D_X25s_X2C_X20vars_X3D_X25s_X2C_X20props_X3D_X25s_X2C_X20comp_X3D_X25s_X2C_X20type_X3D_X25s_X2C_X20_X25s_X29),
  /* K4   */  be_nested_str_weak(expr),
  /* K5   */  be_nested_str_weak(has_functions),
  /* K6   */  be_nested_str_weak(has_variables),
  /* K7   */  be_nested_str_weak(has_properties),
  /* K8   */  be_nested_str_weak(has_computation),
  /* K9   */  be_nested_str_weak(return_type),
  /* K10  */  be_nested_str_weak(),
  /* K11  */  be_nested_str_weak(variable),
  /* K12  */  be_const_class(be_class_ExpressionResult),
};


extern const bclass be_class_ExpressionResult;

/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_ExpressionResult_tostring,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ExpressionResult,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060006,  //  0003  JMPF	R1	#000B
      0x60040018,  //  0004  GETGBL	R1	G24
      0x58080001,  //  0005  LDCONST	R2	K1
      0x600C0005,  //  0006  GETGBL	R3	G5
      0x88100100,  //  0007  GETMBR	R4	R0	K0
      0x7C0C0200,  //  0008  CALL	R3	1
      0x7C040400,  //  0009  CALL	R1	2
      0x70020000,  //  000A  JMP		#000C
      0x58040002,  //  000B  LDCONST	R1	K2
      0x60080018,  //  000C  GETGBL	R2	G24
      0x580C0003,  //  000D  LDCONST	R3	K3
      0x88100104,  //  000E  GETMBR	R4	R0	K4
      0x88140105,  //  000F  GETMBR	R5	R0	K5
      0x88180106,  //  0010  GETMBR	R6	R0	K6
      0x881C0107,  //  0011  GETMBR	R7	R0	K7
      0x88200108,  //  0012  GETMBR	R8	R0	K8
      0x88240109,  //  0013  GETMBR	R9	R0	K9
      0x5C280200,  //  0014  MOVE	R10	R1
      0x7C081000,  //  0015  CALL	R2	8
      0x80040400,  //  0016  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_ExpressionResult_init,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    8,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ExpressionResult,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0x4C200000,  //  0000  LDNIL	R8
      0x20200208,  //  0001  NE	R8	R1	R8
      0x78220001,  //  0002  JMPF	R8	#0005
      0x5C200200,  //  0003  MOVE	R8	R1
      0x70020000,  //  0004  JMP		#0006
      0x5820000A,  //  0005  LDCONST	R8	K10
      0x90020808,  //  0006  SETMBR	R0	K4	R8
      0x60200017,  //  0007  GETGBL	R8	G23
      0x5C240400,  //  0008  MOVE	R9	R2
      0x7C200200,  //  0009  CALL	R8	1
      0x90020A08,  //  000A  SETMBR	R0	K5	R8
      0x60200017,  //  000B  GETGBL	R8	G23
      0x5C240600,  //  000C  MOVE	R9	R3
      0x7C200200,  //  000D  CALL	R8	1
      0x90020C08,  //  000E  SETMBR	R0	K6	R8
      0x60200017,  //  000F  GETGBL	R8	G23
      0x5C240800,  //  0010  MOVE	R9	R4
      0x7C200200,  //  0011  CALL	R8	1
      0x90020E08,  //  0012  SETMBR	R0	K7	R8
      0x60200017,  //  0013  GETGBL	R8	G23
      0x5C240A00,  //  0014  MOVE	R9	R5
      0x7C200200,  //  0015  CALL	R8	1
      0x90021008,  //  0016  SETMBR	R0	K8	R8
      0x4C200000,  //  0017  LDNIL	R8
      0x20200C08,  //  0018  NE	R8	R6	R8
      0x78220001,  //  0019  JMPF	R8	#001C
      0x5C200C00,  //  001A  MOVE	R8	R6
      0x70020000,  //  001B  JMP		#001D
      0x5820000B,  //  001C  LDCONST	R8	K11
      0x90021208,  //  001D  SETMBR	R0	K9	R8
      0x90020007,  //  001E  SETMBR	R0	K0	R7
      0x80000000,  //  001F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: needs_function
********************************************************************/
be_local_closure(class_ExpressionResult_needs_function,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ExpressionResult,     /* shared constants */
    be_str_weak(needs_function),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88040105,  //  0000  GETMBR	R1	R0	K5
      0x74060004,  //  0001  JMPT	R1	#0007
      0x88040106,  //  0002  GETMBR	R1	R0	K6
      0x74060002,  //  0003  JMPT	R1	#0007
      0x88040107,  //  0004  GETMBR	R1	R0	K7
      0x74060000,  //  0005  JMPT	R1	#0007
      0x50040001,  //  0006  LDBOOL	R1	0	1
      0x50040200,  //  0007  LDBOOL	R1	1	0
      0x80040200,  //  0008  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: constructor_call
********************************************************************/
be_local_closure(class_ExpressionResult_constructor_call,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    3,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ExpressionResult,     /* shared constants */
    be_str_weak(constructor_call),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x580C000C,  //  0000  LDCONST	R3	K12
      0x5C100600,  //  0001  MOVE	R4	R3
      0x5C140000,  //  0002  MOVE	R5	R0
      0x50180200,  //  0003  LDBOOL	R6	1	0
      0x501C0000,  //  0004  LDBOOL	R7	0	0
      0x50200000,  //  0005  LDBOOL	R8	0	0
      0x50240000,  //  0006  LDBOOL	R9	0	0
      0x5C280200,  //  0007  MOVE	R10	R1
      0x5C2C0400,  //  0008  MOVE	R11	R2
      0x7C100E00,  //  0009  CALL	R4	7
      0x80040800,  //  000A  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: variable_ref
********************************************************************/
be_local_closure(class_ExpressionResult_variable_ref,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    3,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ExpressionResult,     /* shared constants */
    be_str_weak(variable_ref),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x580C000C,  //  0000  LDCONST	R3	K12
      0x5C100600,  //  0001  MOVE	R4	R3
      0x5C140000,  //  0002  MOVE	R5	R0
      0x50180000,  //  0003  LDBOOL	R6	0	0
      0x501C0200,  //  0004  LDBOOL	R7	1	0
      0x50200000,  //  0005  LDBOOL	R8	0	0
      0x50240000,  //  0006  LDBOOL	R9	0	0
      0x5C280200,  //  0007  MOVE	R10	R1
      0x5C2C0400,  //  0008  MOVE	R11	R2
      0x7C100E00,  //  0009  CALL	R4	7
      0x80040800,  //  000A  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: combine
********************************************************************/
be_local_closure(class_ExpressionResult_combine,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    3,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ExpressionResult,     /* shared constants */
    be_str_weak(combine),
    &be_const_str_solidified,
    ( &(const binstruction[104]) {  /* code */
      0x580C000C,  //  0000  LDCONST	R3	K12
      0x50100000,  //  0001  LDBOOL	R4	0	0
      0x50140000,  //  0002  LDBOOL	R5	0	0
      0x50180000,  //  0003  LDBOOL	R6	0	0
      0x501C0200,  //  0004  LDBOOL	R7	1	0
      0x5820000B,  //  0005  LDCONST	R8	K11
      0x4C240000,  //  0006  LDNIL	R9
      0x20240209,  //  0007  NE	R9	R1	R9
      0x78260017,  //  0008  JMPF	R9	#0021
      0x74120002,  //  0009  JMPT	R4	#000D
      0x88240305,  //  000A  GETMBR	R9	R1	K5
      0x74260000,  //  000B  JMPT	R9	#000D
      0x50240001,  //  000C  LDBOOL	R9	0	1
      0x50240200,  //  000D  LDBOOL	R9	1	0
      0x5C101200,  //  000E  MOVE	R4	R9
      0x74160002,  //  000F  JMPT	R5	#0013
      0x88240306,  //  0010  GETMBR	R9	R1	K6
      0x74260000,  //  0011  JMPT	R9	#0013
      0x50240001,  //  0012  LDBOOL	R9	0	1
      0x50240200,  //  0013  LDBOOL	R9	1	0
      0x5C141200,  //  0014  MOVE	R5	R9
      0x741A0002,  //  0015  JMPT	R6	#0019
      0x88240307,  //  0016  GETMBR	R9	R1	K7
      0x74260000,  //  0017  JMPT	R9	#0019
      0x50240001,  //  0018  LDBOOL	R9	0	1
      0x50240200,  //  0019  LDBOOL	R9	1	0
      0x5C181200,  //  001A  MOVE	R6	R9
      0x741E0002,  //  001B  JMPT	R7	#001F
      0x88240308,  //  001C  GETMBR	R9	R1	K8
      0x74260000,  //  001D  JMPT	R9	#001F
      0x50240001,  //  001E  LDBOOL	R9	0	1
      0x50240200,  //  001F  LDBOOL	R9	1	0
      0x5C1C1200,  //  0020  MOVE	R7	R9
      0x4C240000,  //  0021  LDNIL	R9
      0x20240409,  //  0022  NE	R9	R2	R9
      0x78260017,  //  0023  JMPF	R9	#003C
      0x74120002,  //  0024  JMPT	R4	#0028
      0x88240505,  //  0025  GETMBR	R9	R2	K5
      0x74260000,  //  0026  JMPT	R9	#0028
      0x50240001,  //  0027  LDBOOL	R9	0	1
      0x50240200,  //  0028  LDBOOL	R9	1	0
      0x5C101200,  //  0029  MOVE	R4	R9
      0x74160002,  //  002A  JMPT	R5	#002E
      0x88240506,  //  002B  GETMBR	R9	R2	K6
      0x74260000,  //  002C  JMPT	R9	#002E
      0x50240001,  //  002D  LDBOOL	R9	0	1
      0x50240200,  //  002E  LDBOOL	R9	1	0
      0x5C141200,  //  002F  MOVE	R5	R9
      0x741A0002,  //  0030  JMPT	R6	#0034
      0x88240507,  //  0031  GETMBR	R9	R2	K7
      0x74260000,  //  0032  JMPT	R9	#0034
      0x50240001,  //  0033  LDBOOL	R9	0	1
      0x50240200,  //  0034  LDBOOL	R9	1	0
      0x5C181200,  //  0035  MOVE	R6	R9
      0x741E0002,  //  0036  JMPT	R7	#003A
      0x88240508,  //  0037  GETMBR	R9	R2	K8
      0x74260000,  //  0038  JMPT	R9	#003A
      0x50240001,  //  0039  LDBOOL	R9	0	1
      0x50240200,  //  003A  LDBOOL	R9	1	0
      0x5C1C1200,  //  003B  MOVE	R7	R9
      0x4C240000,  //  003C  LDNIL	R9
      0x20240209,  //  003D  NE	R9	R1	R9
      0x7826000D,  //  003E  JMPF	R9	#004D
      0x4C240000,  //  003F  LDNIL	R9
      0x20240409,  //  0040  NE	R9	R2	R9
      0x7826000A,  //  0041  JMPF	R9	#004D
      0x88240309,  //  0042  GETMBR	R9	R1	K9
      0x88280509,  //  0043  GETMBR	R10	R2	K9
      0x1C24120A,  //  0044  EQ	R9	R9	R10
      0x78260004,  //  0045  JMPF	R9	#004B
      0x88240309,  //  0046  GETMBR	R9	R1	K9
      0x2024130B,  //  0047  NE	R9	R9	K11
      0x78260001,  //  0048  JMPF	R9	#004B
      0x88200309,  //  0049  GETMBR	R8	R1	K9
      0x70020000,  //  004A  JMP		#004C
      0x5820000B,  //  004B  LDCONST	R8	K11
      0x70020010,  //  004C  JMP		#005E
      0x4C240000,  //  004D  LDNIL	R9
      0x20240209,  //  004E  NE	R9	R1	R9
      0x78260005,  //  004F  JMPF	R9	#0056
      0x781E0001,  //  0050  JMPF	R7	#0053
      0x5824000B,  //  0051  LDCONST	R9	K11
      0x70020000,  //  0052  JMP		#0054
      0x88240309,  //  0053  GETMBR	R9	R1	K9
      0x5C201200,  //  0054  MOVE	R8	R9
      0x70020007,  //  0055  JMP		#005E
      0x4C240000,  //  0056  LDNIL	R9
      0x20240409,  //  0057  NE	R9	R2	R9
      0x78260004,  //  0058  JMPF	R9	#005E
      0x781E0001,  //  0059  JMPF	R7	#005C
      0x5824000B,  //  005A  LDCONST	R9	K11
      0x70020000,  //  005B  JMP		#005D
      0x88240509,  //  005C  GETMBR	R9	R2	K9
      0x5C201200,  //  005D  MOVE	R8	R9
      0x5C240600,  //  005E  MOVE	R9	R3
      0x5C280000,  //  005F  MOVE	R10	R0
      0x5C2C0800,  //  0060  MOVE	R11	R4
      0x5C300A00,  //  0061  MOVE	R12	R5
      0x5C340C00,  //  0062  MOVE	R13	R6
      0x5C380E00,  //  0063  MOVE	R14	R7
      0x5C3C1000,  //  0064  MOVE	R15	R8
      0x4C400000,  //  0065  LDNIL	R16
      0x7C240E00,  //  0066  CALL	R9	7
      0x80041200,  //  0067  RET	1	R9
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: property_access
********************************************************************/
be_local_closure(class_ExpressionResult_property_access,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    3,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ExpressionResult,     /* shared constants */
    be_str_weak(property_access),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x580C000C,  //  0000  LDCONST	R3	K12
      0x5C100600,  //  0001  MOVE	R4	R3
      0x5C140000,  //  0002  MOVE	R5	R0
      0x50180000,  //  0003  LDBOOL	R6	0	0
      0x501C0000,  //  0004  LDBOOL	R7	0	0
      0x50200200,  //  0005  LDBOOL	R8	1	0
      0x50240000,  //  0006  LDBOOL	R9	0	0
      0x5C280200,  //  0007  MOVE	R10	R1
      0x5C2C0400,  //  0008  MOVE	R11	R2
      0x7C100E00,  //  0009  CALL	R4	7
      0x80040800,  //  000A  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: function_call
********************************************************************/
be_local_closure(class_ExpressionResult_function_call,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    3,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ExpressionResult,     /* shared constants */
    be_str_weak(function_call),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x580C000C,  //  0000  LDCONST	R3	K12
      0x5C100600,  //  0001  MOVE	R4	R3
      0x5C140000,  //  0002  MOVE	R5	R0
      0x50180200,  //  0003  LDBOOL	R6	1	0
      0x501C0000,  //  0004  LDBOOL	R7	0	0
      0x50200000,  //  0005  LDBOOL	R8	0	0
      0x50240200,  //  0006  LDBOOL	R9	1	0
      0x5C280200,  //  0007  MOVE	R10	R1
      0x5C2C0400,  //  0008  MOVE	R11	R2
      0x7C100E00,  //  0009  CALL	R4	7
      0x80040800,  //  000A  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: needs_closure
********************************************************************/
be_local_closure(class_ExpressionResult_needs_closure,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ExpressionResult,     /* shared constants */
    be_str_weak(needs_closure),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x88040108,  //  0000  GETMBR	R1	R0	K8
      0x78060005,  //  0001  JMPF	R1	#0008
      0x88040105,  //  0002  GETMBR	R1	R0	K5
      0x74060004,  //  0003  JMPT	R1	#0009
      0x88040106,  //  0004  GETMBR	R1	R0	K6
      0x74060002,  //  0005  JMPT	R1	#0009
      0x88040107,  //  0006  GETMBR	R1	R0	K7
      0x74060000,  //  0007  JMPT	R1	#0009
      0x50040001,  //  0008  LDBOOL	R1	0	1
      0x50040200,  //  0009  LDBOOL	R1	1	0
      0x80040200,  //  000A  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: literal
********************************************************************/
be_local_closure(class_ExpressionResult_literal,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    3,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ExpressionResult,     /* shared constants */
    be_str_weak(literal),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x580C000C,  //  0000  LDCONST	R3	K12
      0x5C100600,  //  0001  MOVE	R4	R3
      0x5C140000,  //  0002  MOVE	R5	R0
      0x50180000,  //  0003  LDBOOL	R6	0	0
      0x501C0000,  //  0004  LDBOOL	R7	0	0
      0x50200000,  //  0005  LDBOOL	R8	0	0
      0x50240000,  //  0006  LDBOOL	R9	0	0
      0x5C280200,  //  0007  MOVE	R10	R1
      0x5C2C0400,  //  0008  MOVE	R11	R2
      0x7C100E00,  //  0009  CALL	R4	7
      0x80040800,  //  000A  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: ExpressionResult
********************************************************************/
be_local_class(ExpressionResult,
    7,
    NULL,
    be_nested_map(17,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(return_type, -1), be_const_var(5) },
        { be_const_key_weak(literal, 4), be_const_static_closure(class_ExpressionResult_literal_closure) },
        { be_const_key_weak(tostring, -1), be_const_closure(class_ExpressionResult_tostring_closure) },
        { be_const_key_weak(expr, -1), be_const_var(0) },
        { be_const_key_weak(needs_closure, 9), be_const_closure(class_ExpressionResult_needs_closure_closure) },
        { be_const_key_weak(needs_function, -1), be_const_closure(class_ExpressionResult_needs_function_closure) },
        { be_const_key_weak(has_properties, 10), be_const_var(3) },
        { be_const_key_weak(constructor_call, -1), be_const_static_closure(class_ExpressionResult_constructor_call_closure) },
        { be_const_key_weak(has_computation, 16), be_const_var(4) },
        { be_const_key_weak(variable_ref, -1), be_const_static_closure(class_ExpressionResult_variable_ref_closure) },
        { be_const_key_weak(combine, -1), be_const_static_closure(class_ExpressionResult_combine_closure) },
        { be_const_key_weak(property_access, -1), be_const_static_closure(class_ExpressionResult_property_access_closure) },
        { be_const_key_weak(instance_for_validation, 11), be_const_var(6) },
        { be_const_key_weak(function_call, -1), be_const_static_closure(class_ExpressionResult_function_call_closure) },
        { be_const_key_weak(has_variables, -1), be_const_var(2) },
        { be_const_key_weak(init, 1), be_const_closure(class_ExpressionResult_init_closure) },
        { be_const_key_weak(has_functions, -1), be_const_var(1) },
    })),
    be_str_weak(ExpressionResult)
);
// compact class 'DSLRuntime' ktab size: 26, total: 45 (saved 152 bytes)
static const bvalue be_ktab_class_DSLRuntime[26] = {
  /* K0   */  be_const_int(0),
  /* K1   */  be_nested_str_weak(debug_mode),
  /* K2   */  be_nested_str_weak(DSL_X3A_X20Empty_X20source_X20code),
  /* K3   */  be_nested_str_weak(DSL_X3A_X20Compiling_X20source_X2E_X2E_X2E),
  /* K4   */  be_nested_str_weak(animation_dsl),
  /* K5   */  be_nested_str_weak(compile),
  /* K6   */  be_nested_str_weak(execute_berry_code),
  /* K7   */  be_nested_str_weak(dsl_compilation_error),
  /* K8   */  be_nested_str_weak(DSL_X3A_X20Compilation_X20failed_X20_X2D_X20),
  /* K9   */  be_nested_str_weak(active_source),
  /* K10  */  be_nested_str_weak(engine),
  /* K11  */  be_nested_str_weak(r),
  /* K12  */  be_nested_str_weak(DSL_X3A_X20Cannot_X20open_X20file_X20_X25s),
  /* K13  */  be_nested_str_weak(read),
  /* K14  */  be_nested_str_weak(close),
  /* K15  */  be_nested_str_weak(DSL_X3A_X20Loaded_X20_X25s_X20characters_X20from_X20_X25s),
  /* K16  */  be_nested_str_weak(load_dsl),
  /* K17  */  be_nested_str_weak(DSL_X3A_X20File_X20loading_X20error_X3A_X20_X25s),
  /* K18  */  be_nested_str_weak(DSL_X3A_X20Code_X20generation_X20failed_X20_X2D_X20),
  /* K19  */  be_nested_str_weak(DSL_X3A_X20No_X20active_X20DSL_X20to_X20reload),
  /* K20  */  be_nested_str_weak(DSL_X3A_X20Reloading_X20current_X20DSL_X2E_X2E_X2E),
  /* K21  */  be_nested_str_weak(stop),
  /* K22  */  be_nested_str_weak(clear),
  /* K23  */  be_nested_str_weak(DSL_X3A_X20Berry_X20compilation_X20failed),
  /* K24  */  be_nested_str_weak(DSL_X3A_X20Execution_X20successful),
  /* K25  */  be_nested_str_weak(DSL_X3A_X20Execution_X20error_X3A_X20_X25s),
};


extern const bclass be_class_DSLRuntime;

/********************************************************************
** Solidified function: load_dsl
********************************************************************/
be_local_closure(class_DSLRuntime_load_dsl,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLRuntime,     /* shared constants */
    be_str_weak(load_dsl),
    &be_const_str_solidified,
    ( &(const binstruction[46]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x740A0004,  //  0002  JMPT	R2	#0008
      0x6008000C,  //  0003  GETGBL	R2	G12
      0x5C0C0200,  //  0004  MOVE	R3	R1
      0x7C080200,  //  0005  CALL	R2	1
      0x1C080500,  //  0006  EQ	R2	R2	K0
      0x780A0006,  //  0007  JMPF	R2	#000F
      0x88080101,  //  0008  GETMBR	R2	R0	K1
      0x780A0002,  //  0009  JMPF	R2	#000D
      0x60080001,  //  000A  GETGBL	R2	G1
      0x580C0002,  //  000B  LDCONST	R3	K2
      0x7C080200,  //  000C  CALL	R2	1
      0x50080000,  //  000D  LDBOOL	R2	0	0
      0x80040400,  //  000E  RET	1	R2
      0x88080101,  //  000F  GETMBR	R2	R0	K1
      0x780A0002,  //  0010  JMPF	R2	#0014
      0x60080001,  //  0011  GETGBL	R2	G1
      0x580C0003,  //  0012  LDCONST	R3	K3
      0x7C080200,  //  0013  CALL	R2	1
      0xA802000B,  //  0014  EXBLK	0	#0021
      0xB80A0800,  //  0015  GETNGBL	R2	K4
      0x8C080505,  //  0016  GETMET	R2	R2	K5
      0x5C100200,  //  0017  MOVE	R4	R1
      0x7C080400,  //  0018  CALL	R2	2
      0x8C0C0106,  //  0019  GETMET	R3	R0	K6
      0x5C140400,  //  001A  MOVE	R5	R2
      0x5C180200,  //  001B  MOVE	R6	R1
      0x7C0C0600,  //  001C  CALL	R3	3
      0xA8040001,  //  001D  EXBLK	1	1
      0x80040600,  //  001E  RET	1	R3
      0xA8040001,  //  001F  EXBLK	1	1
      0x7002000B,  //  0020  JMP		#002D
      0x58080007,  //  0021  LDCONST	R2	K7
      0xAC080202,  //  0022  CATCH	R2	1	2
      0x70020007,  //  0023  JMP		#002C
      0x88100101,  //  0024  GETMBR	R4	R0	K1
      0x78120002,  //  0025  JMPF	R4	#0029
      0x60100001,  //  0026  GETGBL	R4	G1
      0x00161003,  //  0027  ADD	R5	K8	R3
      0x7C100200,  //  0028  CALL	R4	1
      0x50100000,  //  0029  LDBOOL	R4	0	0
      0x80040800,  //  002A  RET	1	R4
      0x70020000,  //  002B  JMP		#002D
      0xB0080000,  //  002C  RAISE	2	R0	R0
      0x80000000,  //  002D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_active_source
********************************************************************/
be_local_closure(class_DSLRuntime_get_active_source,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLRuntime,     /* shared constants */
    be_str_weak(get_active_source),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040109,  //  0000  GETMBR	R1	R0	K9
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_DSLRuntime_init,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLRuntime,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x90021401,  //  0000  SETMBR	R0	K10	R1
      0x4C0C0000,  //  0001  LDNIL	R3
      0x90021203,  //  0002  SETMBR	R0	K9	R3
      0x4C0C0000,  //  0003  LDNIL	R3
      0x200C0403,  //  0004  NE	R3	R2	R3
      0x780E0001,  //  0005  JMPF	R3	#0008
      0x5C0C0400,  //  0006  MOVE	R3	R2
      0x70020000,  //  0007  JMP		#0009
      0x500C0000,  //  0008  LDBOOL	R3	0	0
      0x90020203,  //  0009  SETMBR	R0	K1	R3
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: load_dsl_file
********************************************************************/
be_local_closure(class_DSLRuntime_load_dsl_file,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLRuntime,     /* shared constants */
    be_str_weak(load_dsl_file),
    &be_const_str_solidified,
    ( &(const binstruction[56]) {  /* code */
      0xA8020027,  //  0000  EXBLK	0	#0029
      0x60080011,  //  0001  GETGBL	R2	G17
      0x5C0C0200,  //  0002  MOVE	R3	R1
      0x5810000B,  //  0003  LDCONST	R4	K11
      0x7C080400,  //  0004  CALL	R2	2
      0x4C0C0000,  //  0005  LDNIL	R3
      0x1C0C0403,  //  0006  EQ	R3	R2	R3
      0x780E000A,  //  0007  JMPF	R3	#0013
      0x880C0101,  //  0008  GETMBR	R3	R0	K1
      0x780E0005,  //  0009  JMPF	R3	#0010
      0x600C0001,  //  000A  GETGBL	R3	G1
      0x60100018,  //  000B  GETGBL	R4	G24
      0x5814000C,  //  000C  LDCONST	R5	K12
      0x5C180200,  //  000D  MOVE	R6	R1
      0x7C100400,  //  000E  CALL	R4	2
      0x7C0C0200,  //  000F  CALL	R3	1
      0x500C0000,  //  0010  LDBOOL	R3	0	0
      0xA8040001,  //  0011  EXBLK	1	1
      0x80040600,  //  0012  RET	1	R3
      0x8C0C050D,  //  0013  GETMET	R3	R2	K13
      0x7C0C0200,  //  0014  CALL	R3	1
      0x8C10050E,  //  0015  GETMET	R4	R2	K14
      0x7C100200,  //  0016  CALL	R4	1
      0x88100101,  //  0017  GETMBR	R4	R0	K1
      0x78120008,  //  0018  JMPF	R4	#0022
      0x60100001,  //  0019  GETGBL	R4	G1
      0x60140018,  //  001A  GETGBL	R5	G24
      0x5818000F,  //  001B  LDCONST	R6	K15
      0x601C000C,  //  001C  GETGBL	R7	G12
      0x5C200600,  //  001D  MOVE	R8	R3
      0x7C1C0200,  //  001E  CALL	R7	1
      0x5C200200,  //  001F  MOVE	R8	R1
      0x7C140600,  //  0020  CALL	R5	3
      0x7C100200,  //  0021  CALL	R4	1
      0x8C100110,  //  0022  GETMET	R4	R0	K16
      0x5C180600,  //  0023  MOVE	R6	R3
      0x7C100400,  //  0024  CALL	R4	2
      0xA8040001,  //  0025  EXBLK	1	1
      0x80040800,  //  0026  RET	1	R4
      0xA8040001,  //  0027  EXBLK	1	1
      0x7002000D,  //  0028  JMP		#0037
      0xAC080002,  //  0029  CATCH	R2	0	2
      0x7002000A,  //  002A  JMP		#0036
      0x88100101,  //  002B  GETMBR	R4	R0	K1
      0x78120005,  //  002C  JMPF	R4	#0033
      0x60100001,  //  002D  GETGBL	R4	G1
      0x60140018,  //  002E  GETGBL	R5	G24
      0x58180011,  //  002F  LDCONST	R6	K17
      0x5C1C0600,  //  0030  MOVE	R7	R3
      0x7C140400,  //  0031  CALL	R5	2
      0x7C100200,  //  0032  CALL	R4	1
      0x50100000,  //  0033  LDBOOL	R4	0	0
      0x80040800,  //  0034  RET	1	R4
      0x70020000,  //  0035  JMP		#0037
      0xB0080000,  //  0036  RAISE	2	R0	R0
      0x80000000,  //  0037  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_engine
********************************************************************/
be_local_closure(class_DSLRuntime_get_engine,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLRuntime,     /* shared constants */
    be_str_weak(get_engine),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x8804010A,  //  0000  GETMBR	R1	R0	K10
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_generated_code
********************************************************************/
be_local_closure(class_DSLRuntime_get_generated_code,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLRuntime,     /* shared constants */
    be_str_weak(get_generated_code),
    &be_const_str_solidified,
    ( &(const binstruction[31]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0000,  //  0002  JMPF	R2	#0004
      0x88040109,  //  0003  GETMBR	R1	R0	K9
      0x4C080000,  //  0004  LDNIL	R2
      0x1C080202,  //  0005  EQ	R2	R1	R2
      0x780A0001,  //  0006  JMPF	R2	#0009
      0x4C080000,  //  0007  LDNIL	R2
      0x80040400,  //  0008  RET	1	R2
      0xA8020007,  //  0009  EXBLK	0	#0012
      0xB80A0800,  //  000A  GETNGBL	R2	K4
      0x8C080505,  //  000B  GETMET	R2	R2	K5
      0x5C100200,  //  000C  MOVE	R4	R1
      0x7C080400,  //  000D  CALL	R2	2
      0xA8040001,  //  000E  EXBLK	1	1
      0x80040400,  //  000F  RET	1	R2
      0xA8040001,  //  0010  EXBLK	1	1
      0x7002000B,  //  0011  JMP		#001E
      0x58080007,  //  0012  LDCONST	R2	K7
      0xAC080202,  //  0013  CATCH	R2	1	2
      0x70020007,  //  0014  JMP		#001D
      0x88100101,  //  0015  GETMBR	R4	R0	K1
      0x78120002,  //  0016  JMPF	R4	#001A
      0x60100001,  //  0017  GETGBL	R4	G1
      0x00162403,  //  0018  ADD	R5	K18	R3
      0x7C100200,  //  0019  CALL	R4	1
      0x4C100000,  //  001A  LDNIL	R4
      0x80040800,  //  001B  RET	1	R4
      0x70020000,  //  001C  JMP		#001E
      0xB0080000,  //  001D  RAISE	2	R0	R0
      0x80000000,  //  001E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: reload_dsl
********************************************************************/
be_local_closure(class_DSLRuntime_reload_dsl,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLRuntime,     /* shared constants */
    be_str_weak(reload_dsl),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x88040109,  //  0000  GETMBR	R1	R0	K9
      0x4C080000,  //  0001  LDNIL	R2
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060006,  //  0003  JMPF	R1	#000B
      0x88040101,  //  0004  GETMBR	R1	R0	K1
      0x78060002,  //  0005  JMPF	R1	#0009
      0x60040001,  //  0006  GETGBL	R1	G1
      0x58080013,  //  0007  LDCONST	R2	K19
      0x7C040200,  //  0008  CALL	R1	1
      0x50040000,  //  0009  LDBOOL	R1	0	0
      0x80040200,  //  000A  RET	1	R1
      0x88040101,  //  000B  GETMBR	R1	R0	K1
      0x78060002,  //  000C  JMPF	R1	#0010
      0x60040001,  //  000D  GETGBL	R1	G1
      0x58080014,  //  000E  LDCONST	R2	K20
      0x7C040200,  //  000F  CALL	R1	1
      0x8804010A,  //  0010  GETMBR	R1	R0	K10
      0x8C040315,  //  0011  GETMET	R1	R1	K21
      0x7C040200,  //  0012  CALL	R1	1
      0x8804010A,  //  0013  GETMBR	R1	R0	K10
      0x8C040316,  //  0014  GETMET	R1	R1	K22
      0x7C040200,  //  0015  CALL	R1	1
      0x8C040110,  //  0016  GETMET	R1	R0	K16
      0x880C0109,  //  0017  GETMBR	R3	R0	K9
      0x7C040400,  //  0018  CALL	R1	2
      0x80040200,  //  0019  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: execute_berry_code
********************************************************************/
be_local_closure(class_DSLRuntime_execute_berry_code,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLRuntime,     /* shared constants */
    be_str_weak(execute_berry_code),
    &be_const_str_solidified,
    ( &(const binstruction[49]) {  /* code */
      0xA8020020,  //  0000  EXBLK	0	#0022
      0x880C010A,  //  0001  GETMBR	R3	R0	K10
      0x8C0C0715,  //  0002  GETMET	R3	R3	K21
      0x7C0C0200,  //  0003  CALL	R3	1
      0x880C010A,  //  0004  GETMBR	R3	R0	K10
      0x8C0C0716,  //  0005  GETMET	R3	R3	K22
      0x7C0C0200,  //  0006  CALL	R3	1
      0x600C000D,  //  0007  GETGBL	R3	G13
      0x5C100200,  //  0008  MOVE	R4	R1
      0x7C0C0200,  //  0009  CALL	R3	1
      0x4C100000,  //  000A  LDNIL	R4
      0x1C100604,  //  000B  EQ	R4	R3	R4
      0x78120007,  //  000C  JMPF	R4	#0015
      0x88100101,  //  000D  GETMBR	R4	R0	K1
      0x78120002,  //  000E  JMPF	R4	#0012
      0x60100001,  //  000F  GETGBL	R4	G1
      0x58140017,  //  0010  LDCONST	R5	K23
      0x7C100200,  //  0011  CALL	R4	1
      0x50100000,  //  0012  LDBOOL	R4	0	0
      0xA8040001,  //  0013  EXBLK	1	1
      0x80040800,  //  0014  RET	1	R4
      0x5C100600,  //  0015  MOVE	R4	R3
      0x7C100000,  //  0016  CALL	R4	0
      0x90021202,  //  0017  SETMBR	R0	K9	R2
      0x88100101,  //  0018  GETMBR	R4	R0	K1
      0x78120002,  //  0019  JMPF	R4	#001D
      0x60100001,  //  001A  GETGBL	R4	G1
      0x58140018,  //  001B  LDCONST	R5	K24
      0x7C100200,  //  001C  CALL	R4	1
      0x50100200,  //  001D  LDBOOL	R4	1	0
      0xA8040001,  //  001E  EXBLK	1	1
      0x80040800,  //  001F  RET	1	R4
      0xA8040001,  //  0020  EXBLK	1	1
      0x7002000D,  //  0021  JMP		#0030
      0xAC0C0002,  //  0022  CATCH	R3	0	2
      0x7002000A,  //  0023  JMP		#002F
      0x88140101,  //  0024  GETMBR	R5	R0	K1
      0x78160005,  //  0025  JMPF	R5	#002C
      0x60140001,  //  0026  GETGBL	R5	G1
      0x60180018,  //  0027  GETGBL	R6	G24
      0x581C0019,  //  0028  LDCONST	R7	K25
      0x5C200800,  //  0029  MOVE	R8	R4
      0x7C180400,  //  002A  CALL	R6	2
      0x7C140200,  //  002B  CALL	R5	1
      0x50140000,  //  002C  LDBOOL	R5	0	0
      0x80040A00,  //  002D  RET	1	R5
      0x70020000,  //  002E  JMP		#0030
      0xB0080000,  //  002F  RAISE	2	R0	R0
      0x80000000,  //  0030  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_loaded
********************************************************************/
be_local_closure(class_DSLRuntime_is_loaded,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLRuntime,     /* shared constants */
    be_str_weak(is_loaded),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040109,  //  0000  GETMBR	R1	R0	K9
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: DSLRuntime
********************************************************************/
be_local_class(DSLRuntime,
    3,
    NULL,
    be_nested_map(12,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(is_loaded, -1), be_const_closure(class_DSLRuntime_is_loaded_closure) },
        { be_const_key_weak(load_dsl, -1), be_const_closure(class_DSLRuntime_load_dsl_closure) },
        { be_const_key_weak(get_active_source, -1), be_const_closure(class_DSLRuntime_get_active_source_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_DSLRuntime_init_closure) },
        { be_const_key_weak(load_dsl_file, -1), be_const_closure(class_DSLRuntime_load_dsl_file_closure) },
        { be_const_key_weak(active_source, -1), be_const_var(1) },
        { be_const_key_weak(get_engine, -1), be_const_closure(class_DSLRuntime_get_engine_closure) },
        { be_const_key_weak(get_generated_code, -1), be_const_closure(class_DSLRuntime_get_generated_code_closure) },
        { be_const_key_weak(reload_dsl, -1), be_const_closure(class_DSLRuntime_reload_dsl_closure) },
        { be_const_key_weak(execute_berry_code, -1), be_const_closure(class_DSLRuntime_execute_berry_code_closure) },
        { be_const_key_weak(debug_mode, -1), be_const_var(2) },
        { be_const_key_weak(engine, 0), be_const_var(0) },
    })),
    be_str_weak(DSLRuntime)
);

/********************************************************************
** Solidified function: create_runtime
********************************************************************/
be_local_closure(create_runtime,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(create_engine),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(DSLRuntime),
    }),
    be_str_weak(create_runtime),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100000,  //  0002  MOVE	R4	R0
      0x7C080400,  //  0003  CALL	R2	2
      0xB80E0400,  //  0004  GETNGBL	R3	K2
      0x8C0C0703,  //  0005  GETMET	R3	R3	K3
      0x5C140400,  //  0006  MOVE	R5	R2
      0x5C180200,  //  0007  MOVE	R6	R1
      0x7C0C0600,  //  0008  CALL	R3	3
      0x80040600,  //  0009  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_dsl_runtime
********************************************************************/
be_local_closure(create_dsl_runtime,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(create_engine),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(DSLRuntime),
    }),
    be_str_weak(create_dsl_runtime),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100000,  //  0002  MOVE	R4	R0
      0x7C080400,  //  0003  CALL	R2	2
      0xB80E0400,  //  0004  GETNGBL	R3	K2
      0x8C0C0703,  //  0005  GETMET	R3	R3	K3
      0x5C140400,  //  0006  MOVE	R5	R2
      0x5C180200,  //  0007  MOVE	R6	R1
      0x7C0C0600,  //  0008  CALL	R3	3
      0x80040600,  //  0009  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: execute
********************************************************************/
be_local_closure(execute,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation_dsl),
    /* K1   */  be_nested_str_weak(compile),
    }),
    be_str_weak(execute),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x6008000D,  //  0004  GETGBL	R2	G13
      0x5C0C0200,  //  0005  MOVE	R3	R1
      0x7C080200,  //  0006  CALL	R2	1
      0x5C0C0400,  //  0007  MOVE	R3	R2
      0x7C0C0000,  //  0008  CALL	R3	0
      0x80040600,  //  0009  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: compile_dsl
********************************************************************/
be_local_closure(compile_dsl,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation_dsl),
    /* K1   */  be_nested_str_weak(DSLLexer),
    /* K2   */  be_nested_str_weak(tokenize),
    /* K3   */  be_nested_str_weak(has_errors),
    /* K4   */  be_nested_str_weak(DSL_X20Lexer_X20errors_X3A_X0A),
    /* K5   */  be_nested_str_weak(get_errors),
    /* K6   */  be_nested_str_weak(_X20_X20),
    /* K7   */  be_nested_str_weak(_X0A),
    /* K8   */  be_nested_str_weak(stop_iteration),
    /* K9   */  be_nested_str_weak(dsl_compilation_error),
    /* K10  */  be_nested_str_weak(SimpleDSLTranspiler),
    /* K11  */  be_nested_str_weak(transpile),
    /* K12  */  be_nested_str_weak(DSL_X20Transpiler_X20errors_X3A_X0A),
    }),
    be_str_weak(compile_dsl),
    &be_const_str_solidified,
    ( &(const binstruction[51]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x8C080302,  //  0004  GETMET	R2	R1	K2
      0x7C080200,  //  0005  CALL	R2	1
      0x8C0C0303,  //  0006  GETMET	R3	R1	K3
      0x7C0C0200,  //  0007  CALL	R3	1
      0x780E000F,  //  0008  JMPF	R3	#0019
      0x580C0004,  //  0009  LDCONST	R3	K4
      0x60100010,  //  000A  GETGBL	R4	G16
      0x8C140305,  //  000B  GETMET	R5	R1	K5
      0x7C140200,  //  000C  CALL	R5	1
      0x7C100200,  //  000D  CALL	R4	1
      0xA8020005,  //  000E  EXBLK	0	#0015
      0x5C140800,  //  000F  MOVE	R5	R4
      0x7C140000,  //  0010  CALL	R5	0
      0x001A0C05,  //  0011  ADD	R6	K6	R5
      0x00180D07,  //  0012  ADD	R6	R6	K7
      0x000C0606,  //  0013  ADD	R3	R3	R6
      0x7001FFF9,  //  0014  JMP		#000F
      0x58100008,  //  0015  LDCONST	R4	K8
      0xAC100200,  //  0016  CATCH	R4	1	0
      0xB0080000,  //  0017  RAISE	2	R0	R0
      0xB0061203,  //  0018  RAISE	1	K9	R3
      0xB80E0000,  //  0019  GETNGBL	R3	K0
      0x8C0C070A,  //  001A  GETMET	R3	R3	K10
      0x5C140400,  //  001B  MOVE	R5	R2
      0x7C0C0400,  //  001C  CALL	R3	2
      0x8C10070B,  //  001D  GETMET	R4	R3	K11
      0x7C100200,  //  001E  CALL	R4	1
      0x8C140703,  //  001F  GETMET	R5	R3	K3
      0x7C140200,  //  0020  CALL	R5	1
      0x7816000F,  //  0021  JMPF	R5	#0032
      0x5814000C,  //  0022  LDCONST	R5	K12
      0x60180010,  //  0023  GETGBL	R6	G16
      0x8C1C0705,  //  0024  GETMET	R7	R3	K5
      0x7C1C0200,  //  0025  CALL	R7	1
      0x7C180200,  //  0026  CALL	R6	1
      0xA8020005,  //  0027  EXBLK	0	#002E
      0x5C1C0C00,  //  0028  MOVE	R7	R6
      0x7C1C0000,  //  0029  CALL	R7	0
      0x00220C07,  //  002A  ADD	R8	K6	R7
      0x00201107,  //  002B  ADD	R8	R8	K7
      0x00140A08,  //  002C  ADD	R5	R5	R8
      0x7001FFF9,  //  002D  JMP		#0028
      0x58180008,  //  002E  LDCONST	R6	K8
      0xAC180200,  //  002F  CATCH	R6	1	0
      0xB0080000,  //  0030  RAISE	2	R0	R0
      0xB0061205,  //  0031  RAISE	1	K9	R5
      0x80040800,  //  0032  RET	1	R4
    })
  )
);
/*******************************************************************/

extern const bclass be_class_SymbolEntry;
// compact class 'SymbolEntry' ktab size: 61, total: 146 (saved 680 bytes)
static const bvalue be_ktab_class_SymbolEntry[61] = {
  /* K0   */  be_const_class(be_class_SymbolEntry),
  /* K1   */  be_nested_str_weak(TYPE_VARIABLE),
  /* K2   */  be_nested_str_weak(is_dangerous),
  /* K3   */  be_nested_str_weak(param_types),
  /* K4   */  be_nested_str_weak(type),
  /* K5   */  be_nested_str_weak(TYPE_COLOR),
  /* K6   */  be_nested_str_weak(TYPE_CONSTANT),
  /* K7   */  be_nested_str_weak(TYPE_VALUE_PROVIDER),
  /* K8   */  be_nested_str_weak(TYPE_MATH_FUNCTION),
  /* K9   */  be_nested_str_weak(takes_args),
  /* K10  */  be_nested_str_weak(arg_type),
  /* K11  */  be_nested_str_weak(positional),
  /* K12  */  be_nested_str_weak(TYPE_TEMPLATE),
  /* K13  */  be_nested_str_weak(TYPE_COLOR_CONSTRUCTOR),
  /* K14  */  be_nested_str_weak(TYPE_ANIMATION_CONSTRUCTOR),
  /* K15  */  be_nested_str_weak(TYPE_PALETTE),
  /* K16  */  be_nested_str_weak(TYPE_SEQUENCE),
  /* K17  */  be_nested_str_weak(is_builtin),
  /* K18  */  be_nested_str_weak(animation_X2E_math_X2E_X25s),
  /* K19  */  be_nested_str_weak(name),
  /* K20  */  be_nested_str_weak(animation_X2E_X25s),
  /* K21  */  be_nested_str_weak(_X25s_),
  /* K22  */  be_nested_str_weak(TYPE_ANIMATION),
  /* K23  */  be_nested_str_weak(TYPE_PALETTE_CONSTANT),
  /* K24  */  be_nested_str_weak(palette_constant),
  /* K25  */  be_nested_str_weak(palette),
  /* K26  */  be_nested_str_weak(constant),
  /* K27  */  be_nested_str_weak(math_function),
  /* K28  */  be_nested_str_weak(TYPE_USER_FUNCTION),
  /* K29  */  be_nested_str_weak(user_function),
  /* K30  */  be_nested_str_weak(TYPE_VALUE_PROVIDER_CONSTRUCTOR),
  /* K31  */  be_nested_str_weak(value_provider_constructor),
  /* K32  */  be_nested_str_weak(value_provider),
  /* K33  */  be_nested_str_weak(animation_constructor),
  /* K34  */  be_nested_str_weak(animation),
  /* K35  */  be_nested_str_weak(color_constructor),
  /* K36  */  be_nested_str_weak(color),
  /* K37  */  be_nested_str_weak(variable),
  /* K38  */  be_nested_str_weak(sequence),
  /* K39  */  be_nested_str_weak(template),
  /* K40  */  be_nested_str_weak(unknown_X28_X25s_X29),
  /* K41  */  be_nested_str_weak(named),
  /* K42  */  be_nested_str_weak(instance),
  /* K43  */  be_nested_str_weak(none),
  /* K44  */  be_nested_str_weak(_detect_arg_characteristics),
  /* K45  */  be_nested_str_weak(_detect_danger_level),
  /* K46  */  be_nested_str_weak(string),
  /* K47  */  be_nested_str_weak(nil),
  /* K48  */  be_nested_str_weak(_X3C_X25s_X3E),
  /* K49  */  be_nested_str_weak(_X3C_X25s_X3A_X25s_X3E),
  /* K50  */  be_nested_str_weak(),
  /* K51  */  be_const_int(0),
  /* K52  */  be_nested_str_weak(keys),
  /* K53  */  be_nested_str_weak(_X2C),
  /* K54  */  be_nested_str_weak(_X25s_X3A_X25s),
  /* K55  */  be_nested_str_weak(stop_iteration),
  /* K56  */  be_nested_str_weak(_X20params_X3D_X5B_X25s_X5D),
  /* K57  */  be_nested_str_weak(SymbolEntry_X28name_X3D_X27_X25s_X27_X2C_X20type_X3D_X27_X25s_X27_X2C_X20instance_X3D_X25s_X2C_X20),
  /* K58  */  be_nested_str_weak(type_to_string),
  /* K59  */  be_nested_str_weak(takes_args_X3D_X25s_X2C_X20arg_type_X3D_X27_X25s_X27_X2C_X20),
  /* K60  */  be_nested_str_weak(is_builtin_X3D_X25s_X2C_X20is_dangerous_X3D_X25s_X25s_X29),
};


extern const bclass be_class_SymbolEntry;

/********************************************************************
** Solidified function: create_variable
********************************************************************/
be_local_closure(class_SymbolEntry_create_variable,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolEntry,     /* shared constants */
    be_str_weak(create_variable),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0x5C0C0400,  //  0001  MOVE	R3	R2
      0x5C100000,  //  0002  MOVE	R4	R0
      0x88140501,  //  0003  GETMBR	R5	R2	K1
      0x4C180000,  //  0004  LDNIL	R6
      0x5C1C0200,  //  0005  MOVE	R7	R1
      0x7C0C0800,  //  0006  CALL	R3	4
      0x80040600,  //  0007  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_dangerous_call
********************************************************************/
be_local_closure(class_SymbolEntry_is_dangerous_call,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolEntry,     /* shared constants */
    be_str_weak(is_dangerous_call),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040102,  //  0000  GETMBR	R1	R0	K2
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_param_types
********************************************************************/
be_local_closure(class_SymbolEntry_set_param_types,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolEntry,     /* shared constants */
    be_str_weak(set_param_types),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x20080202,  //  0001  NE	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x5C080200,  //  0003  MOVE	R2	R1
      0x70020001,  //  0004  JMP		#0007
      0x60080013,  //  0005  GETGBL	R2	G19
      0x7C080000,  //  0006  CALL	R2	0
      0x90020602,  //  0007  SETMBR	R0	K3	R2
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_color_instance
********************************************************************/
be_local_closure(class_SymbolEntry_is_color_instance,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolEntry,     /* shared constants */
    be_str_weak(is_color_instance),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x88080105,  //  0001  GETMBR	R2	R0	K5
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_constant
********************************************************************/
be_local_closure(class_SymbolEntry_create_constant,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolEntry,     /* shared constants */
    be_str_weak(create_constant),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x580C0000,  //  0000  LDCONST	R3	K0
      0x5C100600,  //  0001  MOVE	R4	R3
      0x5C140000,  //  0002  MOVE	R5	R0
      0x88180706,  //  0003  GETMBR	R6	R3	K6
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x5C200400,  //  0005  MOVE	R8	R2
      0x7C100800,  //  0006  CALL	R4	4
      0x80040800,  //  0007  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_value_provider_instance
********************************************************************/
be_local_closure(class_SymbolEntry_create_value_provider_instance,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolEntry,     /* shared constants */
    be_str_weak(create_value_provider_instance),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x580C0000,  //  0000  LDCONST	R3	K0
      0x5C100600,  //  0001  MOVE	R4	R3
      0x5C140000,  //  0002  MOVE	R5	R0
      0x88180707,  //  0003  GETMBR	R6	R3	K7
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x5C200400,  //  0005  MOVE	R8	R2
      0x7C100800,  //  0006  CALL	R4	4
      0x80040800,  //  0007  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_param_types
********************************************************************/
be_local_closure(class_SymbolEntry_get_param_types,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolEntry,     /* shared constants */
    be_str_weak(get_param_types),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040103,  //  0000  GETMBR	R1	R0	K3
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_math_function
********************************************************************/
be_local_closure(class_SymbolEntry_is_math_function,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolEntry,     /* shared constants */
    be_str_weak(is_math_function),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x88080108,  //  0001  GETMBR	R2	R0	K8
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_color_instance
********************************************************************/
be_local_closure(class_SymbolEntry_create_color_instance,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolEntry,     /* shared constants */
    be_str_weak(create_color_instance),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x580C0000,  //  0000  LDCONST	R3	K0
      0x5C100600,  //  0001  MOVE	R4	R3
      0x5C140000,  //  0002  MOVE	R5	R0
      0x88180705,  //  0003  GETMBR	R6	R3	K5
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x5C200400,  //  0005  MOVE	R8	R2
      0x7C100800,  //  0006  CALL	R4	4
      0x80040800,  //  0007  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: takes_positional_args
********************************************************************/
be_local_closure(class_SymbolEntry_takes_positional_args,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolEntry,     /* shared constants */
    be_str_weak(takes_positional_args),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88040109,  //  0000  GETMBR	R1	R0	K9
      0x78060002,  //  0001  JMPF	R1	#0005
      0x8804010A,  //  0002  GETMBR	R1	R0	K10
      0x1C04030B,  //  0003  EQ	R1	R1	K11
      0x74060000,  //  0004  JMPT	R1	#0006
      0x50040001,  //  0005  LDBOOL	R1	0	1
      0x50040200,  //  0006  LDBOOL	R1	1	0
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_template
********************************************************************/
be_local_closure(class_SymbolEntry_create_template,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolEntry,     /* shared constants */
    be_str_weak(create_template),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0x5C0C0400,  //  0001  MOVE	R3	R2
      0x5C100000,  //  0002  MOVE	R4	R0
      0x8814050C,  //  0003  GETMBR	R5	R2	K12
      0x4C180000,  //  0004  LDNIL	R6
      0x5C1C0200,  //  0005  MOVE	R7	R1
      0x7C0C0800,  //  0006  CALL	R3	4
      0x80040600,  //  0007  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_color_constructor
********************************************************************/
be_local_closure(class_SymbolEntry_is_color_constructor,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolEntry,     /* shared constants */
    be_str_weak(is_color_constructor),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x8808010D,  //  0001  GETMBR	R2	R0	K13
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_animation_constructor
********************************************************************/
be_local_closure(class_SymbolEntry_is_animation_constructor,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolEntry,     /* shared constants */
    be_str_weak(is_animation_constructor),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x8808010E,  //  0001  GETMBR	R2	R0	K14
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_palette_instance
********************************************************************/
be_local_closure(class_SymbolEntry_create_palette_instance,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolEntry,     /* shared constants */
    be_str_weak(create_palette_instance),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x580C0000,  //  0000  LDCONST	R3	K0
      0x5C100600,  //  0001  MOVE	R4	R3
      0x5C140000,  //  0002  MOVE	R5	R0
      0x8818070F,  //  0003  GETMBR	R6	R3	K15
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x5C200400,  //  0005  MOVE	R8	R2
      0x7C100800,  //  0006  CALL	R4	4
      0x80040800,  //  0007  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_sequence
********************************************************************/
be_local_closure(class_SymbolEntry_create_sequence,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolEntry,     /* shared constants */
    be_str_weak(create_sequence),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0x5C0C0400,  //  0001  MOVE	R3	R2
      0x5C100000,  //  0002  MOVE	R4	R0
      0x88140510,  //  0003  GETMBR	R5	R2	K16
      0x4C180000,  //  0004  LDNIL	R6
      0x5C1C0200,  //  0005  MOVE	R7	R1
      0x7C0C0800,  //  0006  CALL	R3	4
      0x80040600,  //  0007  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_reference
********************************************************************/
be_local_closure(class_SymbolEntry_get_reference,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolEntry,     /* shared constants */
    be_str_weak(get_reference),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x88040111,  //  0000  GETMBR	R1	R0	K17
      0x7806000F,  //  0001  JMPF	R1	#0012
      0x88040104,  //  0002  GETMBR	R1	R0	K4
      0x88080108,  //  0003  GETMBR	R2	R0	K8
      0x1C040202,  //  0004  EQ	R1	R1	R2
      0x78060005,  //  0005  JMPF	R1	#000C
      0x60040018,  //  0006  GETGBL	R1	G24
      0x58080012,  //  0007  LDCONST	R2	K18
      0x880C0113,  //  0008  GETMBR	R3	R0	K19
      0x7C040400,  //  0009  CALL	R1	2
      0x80040200,  //  000A  RET	1	R1
      0x70020004,  //  000B  JMP		#0011
      0x60040018,  //  000C  GETGBL	R1	G24
      0x58080014,  //  000D  LDCONST	R2	K20
      0x880C0113,  //  000E  GETMBR	R3	R0	K19
      0x7C040400,  //  000F  CALL	R1	2
      0x80040200,  //  0010  RET	1	R1
      0x70020004,  //  0011  JMP		#0017
      0x60040018,  //  0012  GETGBL	R1	G24
      0x58080015,  //  0013  LDCONST	R2	K21
      0x880C0113,  //  0014  GETMBR	R3	R0	K19
      0x7C040400,  //  0015  CALL	R1	2
      0x80040200,  //  0016  RET	1	R1
      0x80000000,  //  0017  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_math_function
********************************************************************/
be_local_closure(class_SymbolEntry_create_math_function,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolEntry,     /* shared constants */
    be_str_weak(create_math_function),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0x5C0C0400,  //  0001  MOVE	R3	R2
      0x5C100000,  //  0002  MOVE	R4	R0
      0x88140508,  //  0003  GETMBR	R5	R2	K8
      0x4C180000,  //  0004  LDNIL	R6
      0x5C1C0200,  //  0005  MOVE	R7	R1
      0x7C0C0800,  //  0006  CALL	R3	4
      0x80040600,  //  0007  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_animation_instance
********************************************************************/
be_local_closure(class_SymbolEntry_is_animation_instance,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolEntry,     /* shared constants */
    be_str_weak(is_animation_instance),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x88080116,  //  0001  GETMBR	R2	R0	K22
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: type_to_string
********************************************************************/
be_local_closure(class_SymbolEntry_type_to_string,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolEntry,     /* shared constants */
    be_str_weak(type_to_string),
    &be_const_str_solidified,
    ( &(const binstruction[90]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x88080117,  //  0001  GETMBR	R2	R0	K23
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060001,  //  0003  JMPF	R1	#0006
      0x80063000,  //  0004  RET	1	K24
      0x70020052,  //  0005  JMP		#0059
      0x88040104,  //  0006  GETMBR	R1	R0	K4
      0x8808010F,  //  0007  GETMBR	R2	R0	K15
      0x1C040202,  //  0008  EQ	R1	R1	R2
      0x78060001,  //  0009  JMPF	R1	#000C
      0x80063200,  //  000A  RET	1	K25
      0x7002004C,  //  000B  JMP		#0059
      0x88040104,  //  000C  GETMBR	R1	R0	K4
      0x88080106,  //  000D  GETMBR	R2	R0	K6
      0x1C040202,  //  000E  EQ	R1	R1	R2
      0x78060001,  //  000F  JMPF	R1	#0012
      0x80063400,  //  0010  RET	1	K26
      0x70020046,  //  0011  JMP		#0059
      0x88040104,  //  0012  GETMBR	R1	R0	K4
      0x88080108,  //  0013  GETMBR	R2	R0	K8
      0x1C040202,  //  0014  EQ	R1	R1	R2
      0x78060001,  //  0015  JMPF	R1	#0018
      0x80063600,  //  0016  RET	1	K27
      0x70020040,  //  0017  JMP		#0059
      0x88040104,  //  0018  GETMBR	R1	R0	K4
      0x8808011C,  //  0019  GETMBR	R2	R0	K28
      0x1C040202,  //  001A  EQ	R1	R1	R2
      0x78060001,  //  001B  JMPF	R1	#001E
      0x80063A00,  //  001C  RET	1	K29
      0x7002003A,  //  001D  JMP		#0059
      0x88040104,  //  001E  GETMBR	R1	R0	K4
      0x8808011E,  //  001F  GETMBR	R2	R0	K30
      0x1C040202,  //  0020  EQ	R1	R1	R2
      0x78060001,  //  0021  JMPF	R1	#0024
      0x80063E00,  //  0022  RET	1	K31
      0x70020034,  //  0023  JMP		#0059
      0x88040104,  //  0024  GETMBR	R1	R0	K4
      0x88080107,  //  0025  GETMBR	R2	R0	K7
      0x1C040202,  //  0026  EQ	R1	R1	R2
      0x78060001,  //  0027  JMPF	R1	#002A
      0x80064000,  //  0028  RET	1	K32
      0x7002002E,  //  0029  JMP		#0059
      0x88040104,  //  002A  GETMBR	R1	R0	K4
      0x8808010E,  //  002B  GETMBR	R2	R0	K14
      0x1C040202,  //  002C  EQ	R1	R1	R2
      0x78060001,  //  002D  JMPF	R1	#0030
      0x80064200,  //  002E  RET	1	K33
      0x70020028,  //  002F  JMP		#0059
      0x88040104,  //  0030  GETMBR	R1	R0	K4
      0x88080116,  //  0031  GETMBR	R2	R0	K22
      0x1C040202,  //  0032  EQ	R1	R1	R2
      0x78060001,  //  0033  JMPF	R1	#0036
      0x80064400,  //  0034  RET	1	K34
      0x70020022,  //  0035  JMP		#0059
      0x88040104,  //  0036  GETMBR	R1	R0	K4
      0x8808010D,  //  0037  GETMBR	R2	R0	K13
      0x1C040202,  //  0038  EQ	R1	R1	R2
      0x78060001,  //  0039  JMPF	R1	#003C
      0x80064600,  //  003A  RET	1	K35
      0x7002001C,  //  003B  JMP		#0059
      0x88040104,  //  003C  GETMBR	R1	R0	K4
      0x88080105,  //  003D  GETMBR	R2	R0	K5
      0x1C040202,  //  003E  EQ	R1	R1	R2
      0x78060001,  //  003F  JMPF	R1	#0042
      0x80064800,  //  0040  RET	1	K36
      0x70020016,  //  0041  JMP		#0059
      0x88040104,  //  0042  GETMBR	R1	R0	K4
      0x88080101,  //  0043  GETMBR	R2	R0	K1
      0x1C040202,  //  0044  EQ	R1	R1	R2
      0x78060001,  //  0045  JMPF	R1	#0048
      0x80064A00,  //  0046  RET	1	K37
      0x70020010,  //  0047  JMP		#0059
      0x88040104,  //  0048  GETMBR	R1	R0	K4
      0x88080110,  //  0049  GETMBR	R2	R0	K16
      0x1C040202,  //  004A  EQ	R1	R1	R2
      0x78060001,  //  004B  JMPF	R1	#004E
      0x80064C00,  //  004C  RET	1	K38
      0x7002000A,  //  004D  JMP		#0059
      0x88040104,  //  004E  GETMBR	R1	R0	K4
      0x8808010C,  //  004F  GETMBR	R2	R0	K12
      0x1C040202,  //  0050  EQ	R1	R1	R2
      0x78060001,  //  0051  JMPF	R1	#0054
      0x80064E00,  //  0052  RET	1	K39
      0x70020004,  //  0053  JMP		#0059
      0x60040018,  //  0054  GETGBL	R1	G24
      0x58080028,  //  0055  LDCONST	R2	K40
      0x880C0104,  //  0056  GETMBR	R3	R0	K4
      0x7C040400,  //  0057  CALL	R1	2
      0x80040200,  //  0058  RET	1	R1
      0x80000000,  //  0059  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_value_provider_instance
********************************************************************/
be_local_closure(class_SymbolEntry_is_value_provider_instance,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolEntry,     /* shared constants */
    be_str_weak(is_value_provider_instance),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x88080107,  //  0001  GETMBR	R2	R0	K7
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_color_constructor
********************************************************************/
be_local_closure(class_SymbolEntry_create_color_constructor,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolEntry,     /* shared constants */
    be_str_weak(create_color_constructor),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x580C0000,  //  0000  LDCONST	R3	K0
      0x5C100600,  //  0001  MOVE	R4	R3
      0x5C140000,  //  0002  MOVE	R5	R0
      0x8818070D,  //  0003  GETMBR	R6	R3	K13
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x5C200400,  //  0005  MOVE	R8	R2
      0x7C100800,  //  0006  CALL	R4	4
      0x80040800,  //  0007  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_animation_constructor
********************************************************************/
be_local_closure(class_SymbolEntry_create_animation_constructor,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolEntry,     /* shared constants */
    be_str_weak(create_animation_constructor),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x580C0000,  //  0000  LDCONST	R3	K0
      0x5C100600,  //  0001  MOVE	R4	R3
      0x5C140000,  //  0002  MOVE	R5	R0
      0x8818070E,  //  0003  GETMBR	R6	R3	K14
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x5C200400,  //  0005  MOVE	R8	R2
      0x7C100800,  //  0006  CALL	R4	4
      0x80040800,  //  0007  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_animation_instance
********************************************************************/
be_local_closure(class_SymbolEntry_create_animation_instance,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolEntry,     /* shared constants */
    be_str_weak(create_animation_instance),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x580C0000,  //  0000  LDCONST	R3	K0
      0x5C100600,  //  0001  MOVE	R4	R3
      0x5C140000,  //  0002  MOVE	R5	R0
      0x88180716,  //  0003  GETMBR	R6	R3	K22
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x5C200400,  //  0005  MOVE	R8	R2
      0x7C100800,  //  0006  CALL	R4	4
      0x80040800,  //  0007  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: takes_named_args
********************************************************************/
be_local_closure(class_SymbolEntry_takes_named_args,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolEntry,     /* shared constants */
    be_str_weak(takes_named_args),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88040109,  //  0000  GETMBR	R1	R0	K9
      0x78060002,  //  0001  JMPF	R1	#0005
      0x8804010A,  //  0002  GETMBR	R1	R0	K10
      0x1C040329,  //  0003  EQ	R1	R1	K41
      0x74060000,  //  0004  JMPT	R1	#0006
      0x50040001,  //  0005  LDBOOL	R1	0	1
      0x50040200,  //  0006  LDBOOL	R1	1	0
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_bytes_instance
********************************************************************/
be_local_closure(class_SymbolEntry_is_bytes_instance,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolEntry,     /* shared constants */
    be_str_weak(is_bytes_instance),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x88080117,  //  0001  GETMBR	R2	R0	K23
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x74060003,  //  0003  JMPT	R1	#0008
      0x88040104,  //  0004  GETMBR	R1	R0	K4
      0x8808010F,  //  0005  GETMBR	R2	R0	K15
      0x1C040202,  //  0006  EQ	R1	R1	R2
      0x78060008,  //  0007  JMPF	R1	#0011
      0x8804012A,  //  0008  GETMBR	R1	R0	K42
      0x4C080000,  //  0009  LDNIL	R2
      0x20040202,  //  000A  NE	R1	R1	R2
      0x78060004,  //  000B  JMPF	R1	#0011
      0x6004000F,  //  000C  GETGBL	R1	G15
      0x8808012A,  //  000D  GETMBR	R2	R0	K42
      0x600C0015,  //  000E  GETGBL	R3	G21
      0x7C040400,  //  000F  CALL	R1	2
      0x74060000,  //  0010  JMPT	R1	#0012
      0x50040001,  //  0011  LDBOOL	R1	0	1
      0x50040200,  //  0012  LDBOOL	R1	1	0
      0x80040200,  //  0013  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _detect_danger_level
********************************************************************/
be_local_closure(class_SymbolEntry__detect_danger_level,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolEntry,     /* shared constants */
    be_str_weak(_detect_danger_level),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x8808011E,  //  0001  GETMBR	R2	R0	K30
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060002,  //  0003  JMPF	R1	#0007
      0x50040200,  //  0004  LDBOOL	R1	1	0
      0x90020401,  //  0005  SETMBR	R0	K2	R1
      0x7002000F,  //  0006  JMP		#0017
      0x88040104,  //  0007  GETMBR	R1	R0	K4
      0x8808010E,  //  0008  GETMBR	R2	R0	K14
      0x1C040202,  //  0009  EQ	R1	R1	R2
      0x78060002,  //  000A  JMPF	R1	#000E
      0x50040200,  //  000B  LDBOOL	R1	1	0
      0x90020401,  //  000C  SETMBR	R0	K2	R1
      0x70020008,  //  000D  JMP		#0017
      0x88040104,  //  000E  GETMBR	R1	R0	K4
      0x8808010D,  //  000F  GETMBR	R2	R0	K13
      0x1C040202,  //  0010  EQ	R1	R1	R2
      0x78060002,  //  0011  JMPF	R1	#0015
      0x50040200,  //  0012  LDBOOL	R1	1	0
      0x90020401,  //  0013  SETMBR	R0	K2	R1
      0x70020001,  //  0014  JMP		#0017
      0x50040000,  //  0015  LDBOOL	R1	0	0
      0x90020401,  //  0016  SETMBR	R0	K2	R1
      0x80000000,  //  0017  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_palette_constant
********************************************************************/
be_local_closure(class_SymbolEntry_create_palette_constant,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolEntry,     /* shared constants */
    be_str_weak(create_palette_constant),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x580C0000,  //  0000  LDCONST	R3	K0
      0x5C100600,  //  0001  MOVE	R4	R3
      0x5C140000,  //  0002  MOVE	R5	R0
      0x88180717,  //  0003  GETMBR	R6	R3	K23
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x5C200400,  //  0005  MOVE	R8	R2
      0x7C100800,  //  0006  CALL	R4	4
      0x80040800,  //  0007  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_SymbolEntry_init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolEntry,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x90022601,  //  0000  SETMBR	R0	K19	R1
      0x90020802,  //  0001  SETMBR	R0	K4	R2
      0x90025403,  //  0002  SETMBR	R0	K42	R3
      0x4C140000,  //  0003  LDNIL	R5
      0x20140805,  //  0004  NE	R5	R4	R5
      0x78160001,  //  0005  JMPF	R5	#0008
      0x5C140800,  //  0006  MOVE	R5	R4
      0x70020000,  //  0007  JMP		#0009
      0x50140000,  //  0008  LDBOOL	R5	0	0
      0x90022205,  //  0009  SETMBR	R0	K17	R5
      0x50140000,  //  000A  LDBOOL	R5	0	0
      0x90021205,  //  000B  SETMBR	R0	K9	R5
      0x9002152B,  //  000C  SETMBR	R0	K10	K43
      0x50140000,  //  000D  LDBOOL	R5	0	0
      0x90020405,  //  000E  SETMBR	R0	K2	R5
      0x60140013,  //  000F  GETGBL	R5	G19
      0x7C140000,  //  0010  CALL	R5	0
      0x90020605,  //  0011  SETMBR	R0	K3	R5
      0x8C14012C,  //  0012  GETMET	R5	R0	K44
      0x7C140200,  //  0013  CALL	R5	1
      0x8C14012D,  //  0014  GETMET	R5	R0	K45
      0x7C140200,  //  0015  CALL	R5	1
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_value_provider_constructor
********************************************************************/
be_local_closure(class_SymbolEntry_is_value_provider_constructor,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolEntry,     /* shared constants */
    be_str_weak(is_value_provider_constructor),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x8808011E,  //  0001  GETMBR	R2	R0	K30
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_user_function
********************************************************************/
be_local_closure(class_SymbolEntry_create_user_function,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolEntry,     /* shared constants */
    be_str_weak(create_user_function),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0x5C0C0400,  //  0001  MOVE	R3	R2
      0x5C100000,  //  0002  MOVE	R4	R0
      0x8814051C,  //  0003  GETMBR	R5	R2	K28
      0x4C180000,  //  0004  LDNIL	R6
      0x5C1C0200,  //  0005  MOVE	R7	R1
      0x7C0C0800,  //  0006  CALL	R3	4
      0x80040600,  //  0007  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_SymbolEntry_tostring,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolEntry,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[84]) {  /* code */
      0xA4065C00,  //  0000  IMPORT	R1	K46
      0x5808002F,  //  0001  LDCONST	R2	K47
      0x880C012A,  //  0002  GETMBR	R3	R0	K42
      0x4C100000,  //  0003  LDNIL	R4
      0x200C0604,  //  0004  NE	R3	R3	R4
      0x780E0014,  //  0005  JMPF	R3	#001B
      0x600C0004,  //  0006  GETGBL	R3	G4
      0x8810012A,  //  0007  GETMBR	R4	R0	K42
      0x7C0C0200,  //  0008  CALL	R3	1
      0x1C10072A,  //  0009  EQ	R4	R3	K42
      0x78120007,  //  000A  JMPF	R4	#0013
      0x60100018,  //  000B  GETGBL	R4	G24
      0x58140030,  //  000C  LDCONST	R5	K48
      0x60180005,  //  000D  GETGBL	R6	G5
      0x881C012A,  //  000E  GETMBR	R7	R0	K42
      0x7C180200,  //  000F  CALL	R6	1
      0x7C100400,  //  0010  CALL	R4	2
      0x5C080800,  //  0011  MOVE	R2	R4
      0x70020007,  //  0012  JMP		#001B
      0x60100018,  //  0013  GETGBL	R4	G24
      0x58140031,  //  0014  LDCONST	R5	K49
      0x5C180600,  //  0015  MOVE	R6	R3
      0x601C0008,  //  0016  GETGBL	R7	G8
      0x8820012A,  //  0017  GETMBR	R8	R0	K42
      0x7C1C0200,  //  0018  CALL	R7	1
      0x7C100600,  //  0019  CALL	R4	3
      0x5C080800,  //  001A  MOVE	R2	R4
      0x580C0032,  //  001B  LDCONST	R3	K50
      0x6010000C,  //  001C  GETGBL	R4	G12
      0x88140103,  //  001D  GETMBR	R5	R0	K3
      0x7C100200,  //  001E  CALL	R4	1
      0x24100933,  //  001F  GT	R4	R4	K51
      0x7812001D,  //  0020  JMPF	R4	#003F
      0x58100032,  //  0021  LDCONST	R4	K50
      0x50140200,  //  0022  LDBOOL	R5	1	0
      0x60180010,  //  0023  GETGBL	R6	G16
      0x881C0103,  //  0024  GETMBR	R7	R0	K3
      0x8C1C0F34,  //  0025  GETMET	R7	R7	K52
      0x7C1C0200,  //  0026  CALL	R7	1
      0x7C180200,  //  0027  CALL	R6	1
      0xA802000D,  //  0028  EXBLK	0	#0037
      0x5C1C0C00,  //  0029  MOVE	R7	R6
      0x7C1C0000,  //  002A  CALL	R7	0
      0x5C200A00,  //  002B  MOVE	R8	R5
      0x74220000,  //  002C  JMPT	R8	#002E
      0x00100935,  //  002D  ADD	R4	R4	K53
      0x60200018,  //  002E  GETGBL	R8	G24
      0x58240036,  //  002F  LDCONST	R9	K54
      0x5C280E00,  //  0030  MOVE	R10	R7
      0x882C0103,  //  0031  GETMBR	R11	R0	K3
      0x942C1607,  //  0032  GETIDX	R11	R11	R7
      0x7C200600,  //  0033  CALL	R8	3
      0x00100808,  //  0034  ADD	R4	R4	R8
      0x50140000,  //  0035  LDBOOL	R5	0	0
      0x7001FFF1,  //  0036  JMP		#0029
      0x58180037,  //  0037  LDCONST	R6	K55
      0xAC180200,  //  0038  CATCH	R6	1	0
      0xB0080000,  //  0039  RAISE	2	R0	R0
      0x60180018,  //  003A  GETGBL	R6	G24
      0x581C0038,  //  003B  LDCONST	R7	K56
      0x5C200800,  //  003C  MOVE	R8	R4
      0x7C180400,  //  003D  CALL	R6	2
      0x5C0C0C00,  //  003E  MOVE	R3	R6
      0x60100018,  //  003F  GETGBL	R4	G24
      0x58140039,  //  0040  LDCONST	R5	K57
      0x88180113,  //  0041  GETMBR	R6	R0	K19
      0x8C1C013A,  //  0042  GETMET	R7	R0	K58
      0x7C1C0200,  //  0043  CALL	R7	1
      0x5C200400,  //  0044  MOVE	R8	R2
      0x7C100800,  //  0045  CALL	R4	4
      0x60140018,  //  0046  GETGBL	R5	G24
      0x5818003B,  //  0047  LDCONST	R6	K59
      0x881C0109,  //  0048  GETMBR	R7	R0	K9
      0x8820010A,  //  0049  GETMBR	R8	R0	K10
      0x7C140600,  //  004A  CALL	R5	3
      0x00100805,  //  004B  ADD	R4	R4	R5
      0x60140018,  //  004C  GETGBL	R5	G24
      0x5818003C,  //  004D  LDCONST	R6	K60
      0x881C0111,  //  004E  GETMBR	R7	R0	K17
      0x88200102,  //  004F  GETMBR	R8	R0	K2
      0x5C240600,  //  0050  MOVE	R9	R3
      0x7C140800,  //  0051  CALL	R5	4
      0x00100805,  //  0052  ADD	R4	R4	R5
      0x80040800,  //  0053  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_value_provider_constructor
********************************************************************/
be_local_closure(class_SymbolEntry_create_value_provider_constructor,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolEntry,     /* shared constants */
    be_str_weak(create_value_provider_constructor),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x580C0000,  //  0000  LDCONST	R3	K0
      0x5C100600,  //  0001  MOVE	R4	R3
      0x5C140000,  //  0002  MOVE	R5	R0
      0x8818071E,  //  0003  GETMBR	R6	R3	K30
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x5C200400,  //  0005  MOVE	R8	R2
      0x7C100800,  //  0006  CALL	R4	4
      0x80040800,  //  0007  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_user_function
********************************************************************/
be_local_closure(class_SymbolEntry_is_user_function,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolEntry,     /* shared constants */
    be_str_weak(is_user_function),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x8808011C,  //  0001  GETMBR	R2	R0	K28
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _detect_arg_characteristics
********************************************************************/
be_local_closure(class_SymbolEntry__detect_arg_characteristics,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolEntry,     /* shared constants */
    be_str_weak(_detect_arg_characteristics),
    &be_const_str_solidified,
    ( &(const binstruction[52]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x88080117,  //  0001  GETMBR	R2	R0	K23
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x74060007,  //  0003  JMPT	R1	#000C
      0x88040104,  //  0004  GETMBR	R1	R0	K4
      0x8808010F,  //  0005  GETMBR	R2	R0	K15
      0x1C040202,  //  0006  EQ	R1	R1	R2
      0x74060003,  //  0007  JMPT	R1	#000C
      0x88040104,  //  0008  GETMBR	R1	R0	K4
      0x88080106,  //  0009  GETMBR	R2	R0	K6
      0x1C040202,  //  000A  EQ	R1	R1	R2
      0x78060003,  //  000B  JMPF	R1	#0010
      0x50040000,  //  000C  LDBOOL	R1	0	0
      0x90021201,  //  000D  SETMBR	R0	K9	R1
      0x9002152B,  //  000E  SETMBR	R0	K10	K43
      0x70020022,  //  000F  JMP		#0033
      0x88040104,  //  0010  GETMBR	R1	R0	K4
      0x88080108,  //  0011  GETMBR	R2	R0	K8
      0x1C040202,  //  0012  EQ	R1	R1	R2
      0x78060003,  //  0013  JMPF	R1	#0018
      0x50040200,  //  0014  LDBOOL	R1	1	0
      0x90021201,  //  0015  SETMBR	R0	K9	R1
      0x9002150B,  //  0016  SETMBR	R0	K10	K11
      0x7002001A,  //  0017  JMP		#0033
      0x88040104,  //  0018  GETMBR	R1	R0	K4
      0x8808011C,  //  0019  GETMBR	R2	R0	K28
      0x1C040202,  //  001A  EQ	R1	R1	R2
      0x78060003,  //  001B  JMPF	R1	#0020
      0x50040200,  //  001C  LDBOOL	R1	1	0
      0x90021201,  //  001D  SETMBR	R0	K9	R1
      0x9002150B,  //  001E  SETMBR	R0	K10	K11
      0x70020012,  //  001F  JMP		#0033
      0x88040104,  //  0020  GETMBR	R1	R0	K4
      0x8808011E,  //  0021  GETMBR	R2	R0	K30
      0x1C040202,  //  0022  EQ	R1	R1	R2
      0x74060007,  //  0023  JMPT	R1	#002C
      0x88040104,  //  0024  GETMBR	R1	R0	K4
      0x8808010E,  //  0025  GETMBR	R2	R0	K14
      0x1C040202,  //  0026  EQ	R1	R1	R2
      0x74060003,  //  0027  JMPT	R1	#002C
      0x88040104,  //  0028  GETMBR	R1	R0	K4
      0x8808010D,  //  0029  GETMBR	R2	R0	K13
      0x1C040202,  //  002A  EQ	R1	R1	R2
      0x78060003,  //  002B  JMPF	R1	#0030
      0x50040200,  //  002C  LDBOOL	R1	1	0
      0x90021201,  //  002D  SETMBR	R0	K9	R1
      0x90021529,  //  002E  SETMBR	R0	K10	K41
      0x70020002,  //  002F  JMP		#0033
      0x50040000,  //  0030  LDBOOL	R1	0	0
      0x90021201,  //  0031  SETMBR	R0	K9	R1
      0x9002152B,  //  0032  SETMBR	R0	K10	K43
      0x80000000,  //  0033  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: SymbolEntry
********************************************************************/
be_local_class(SymbolEntry,
    8,
    NULL,
    be_nested_map(56,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(TYPE_TEMPLATE, 53), be_const_int(14) },
        { be_const_key_weak(TYPE_COLOR, -1), be_const_int(11) },
        { be_const_key_weak(create_variable, 52), be_const_static_closure(class_SymbolEntry_create_variable_closure) },
        { be_const_key_weak(is_dangerous_call, -1), be_const_closure(class_SymbolEntry_is_dangerous_call_closure) },
        { be_const_key_weak(_detect_arg_characteristics, -1), be_const_closure(class_SymbolEntry__detect_arg_characteristics_closure) },
        { be_const_key_weak(is_color_instance, -1), be_const_closure(class_SymbolEntry_is_color_instance_closure) },
        { be_const_key_weak(TYPE_CONSTANT, -1), be_const_int(3) },
        { be_const_key_weak(TYPE_SEQUENCE, -1), be_const_int(13) },
        { be_const_key_weak(TYPE_USER_FUNCTION, -1), be_const_int(5) },
        { be_const_key_weak(create_value_provider_instance, -1), be_const_static_closure(class_SymbolEntry_create_value_provider_instance_closure) },
        { be_const_key_weak(takes_args, 22), be_const_var(3) },
        { be_const_key_weak(get_param_types, -1), be_const_closure(class_SymbolEntry_get_param_types_closure) },
        { be_const_key_weak(is_user_function, 15), be_const_closure(class_SymbolEntry_is_user_function_closure) },
        { be_const_key_weak(create_color_instance, 49), be_const_static_closure(class_SymbolEntry_create_color_instance_closure) },
        { be_const_key_weak(takes_positional_args, -1), be_const_closure(class_SymbolEntry_takes_positional_args_closure) },
        { be_const_key_weak(create_value_provider_constructor, 25), be_const_static_closure(class_SymbolEntry_create_value_provider_constructor_closure) },
        { be_const_key_weak(TYPE_VARIABLE, 32), be_const_int(12) },
        { be_const_key_weak(TYPE_VALUE_PROVIDER_CONSTRUCTOR, 24), be_const_int(6) },
        { be_const_key_weak(is_color_constructor, -1), be_const_closure(class_SymbolEntry_is_color_constructor_closure) },
        { be_const_key_weak(is_animation_constructor, 31), be_const_closure(class_SymbolEntry_is_animation_constructor_closure) },
        { be_const_key_weak(TYPE_ANIMATION, -1), be_const_int(9) },
        { be_const_key_weak(create_palette_instance, 44), be_const_static_closure(class_SymbolEntry_create_palette_instance_closure) },
        { be_const_key_weak(tostring, 28), be_const_closure(class_SymbolEntry_tostring_closure) },
        { be_const_key_weak(create_sequence, -1), be_const_static_closure(class_SymbolEntry_create_sequence_closure) },
        { be_const_key_weak(create_user_function, 42), be_const_static_closure(class_SymbolEntry_create_user_function_closure) },
        { be_const_key_weak(is_value_provider_constructor, -1), be_const_closure(class_SymbolEntry_is_value_provider_constructor_closure) },
        { be_const_key_weak(instance, -1), be_const_var(2) },
        { be_const_key_weak(TYPE_ANIMATION_CONSTRUCTOR, -1), be_const_int(8) },
        { be_const_key_weak(type, 51), be_const_var(1) },
        { be_const_key_weak(TYPE_MATH_FUNCTION, 48), be_const_int(4) },
        { be_const_key_weak(is_animation_instance, 17), be_const_closure(class_SymbolEntry_is_animation_instance_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_SymbolEntry_init_closure) },
        { be_const_key_weak(create_palette_constant, -1), be_const_static_closure(class_SymbolEntry_create_palette_constant_closure) },
        { be_const_key_weak(is_builtin, 43), be_const_var(5) },
        { be_const_key_weak(create_color_constructor, -1), be_const_static_closure(class_SymbolEntry_create_color_constructor_closure) },
        { be_const_key_weak(create_animation_constructor, -1), be_const_static_closure(class_SymbolEntry_create_animation_constructor_closure) },
        { be_const_key_weak(param_types, -1), be_const_var(7) },
        { be_const_key_weak(_detect_danger_level, -1), be_const_closure(class_SymbolEntry__detect_danger_level_closure) },
        { be_const_key_weak(create_animation_instance, -1), be_const_static_closure(class_SymbolEntry_create_animation_instance_closure) },
        { be_const_key_weak(takes_named_args, -1), be_const_closure(class_SymbolEntry_takes_named_args_closure) },
        { be_const_key_weak(arg_type, 37), be_const_var(4) },
        { be_const_key_weak(is_bytes_instance, -1), be_const_closure(class_SymbolEntry_is_bytes_instance_closure) },
        { be_const_key_weak(name, -1), be_const_var(0) },
        { be_const_key_weak(is_value_provider_instance, 47), be_const_closure(class_SymbolEntry_is_value_provider_instance_closure) },
        { be_const_key_weak(type_to_string, -1), be_const_closure(class_SymbolEntry_type_to_string_closure) },
        { be_const_key_weak(is_math_function, 10), be_const_closure(class_SymbolEntry_is_math_function_closure) },
        { be_const_key_weak(is_dangerous, 12), be_const_var(6) },
        { be_const_key_weak(TYPE_PALETTE, -1), be_const_int(2) },
        { be_const_key_weak(create_math_function, -1), be_const_static_closure(class_SymbolEntry_create_math_function_closure) },
        { be_const_key_weak(TYPE_VALUE_PROVIDER, -1), be_const_int(7) },
        { be_const_key_weak(TYPE_COLOR_CONSTRUCTOR, 16), be_const_int(10) },
        { be_const_key_weak(get_reference, -1), be_const_closure(class_SymbolEntry_get_reference_closure) },
        { be_const_key_weak(TYPE_PALETTE_CONSTANT, -1), be_const_int(1) },
        { be_const_key_weak(create_template, -1), be_const_static_closure(class_SymbolEntry_create_template_closure) },
        { be_const_key_weak(create_constant, 8), be_const_static_closure(class_SymbolEntry_create_constant_closure) },
        { be_const_key_weak(set_param_types, 4), be_const_closure(class_SymbolEntry_set_param_types_closure) },
    })),
    be_str_weak(SymbolEntry)
);

/********************************************************************
** Solidified function: compile_dsl_source
********************************************************************/
be_local_closure(compile_dsl_source,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation_dsl),
    /* K1   */  be_nested_str_weak(compile_dsl),
    }),
    be_str_weak(compile_dsl_source),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x80040200,  //  0004  RET	1	R1
    })
  )
);
/*******************************************************************/

// ktab too big for class 'SimpleDSLTranspiler' - skipping

extern const bclass be_class_SimpleDSLTranspiler;

/********************************************************************
** Solidified function: _process_named_arguments_generic
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__process_named_arguments_generic,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_process_named_arguments_unified),
    /* K1   */  be_nested_str_weak(CONTEXT_GENERIC),
    }),
    be_str_weak(_process_named_arguments_generic),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C0C0100,  //  0000  GETMET	R3	R0	K0
      0x5C140200,  //  0001  MOVE	R5	R1
      0x5C180400,  //  0002  MOVE	R6	R2
      0x881C0101,  //  0003  GETMBR	R7	R0	K1
      0x7C0C0800,  //  0004  CALL	R3	4
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _process_named_arguments_unified
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__process_named_arguments_unified,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        11,                          /* nstack */
        3,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 2]) {  /* upvals */
          be_local_const_upval(1, 0),
          be_local_const_upval(1, 1),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 2]) {     /* constants */
        /* K0   */  be_nested_str_weak(add),
        /* K1   */  be_nested_str_weak(_X25s_X2E_X25s_X20_X3D_X20_X25s_X25s),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[11]) {  /* code */
          0x680C0000,  //  0000  GETUPV	R3	U0
          0x8C0C0700,  //  0001  GETMET	R3	R3	K0
          0x60140018,  //  0002  GETGBL	R5	G24
          0x58180001,  //  0003  LDCONST	R6	K1
          0x681C0001,  //  0004  GETUPV	R7	U1
          0x5C200000,  //  0005  MOVE	R8	R0
          0x5C240200,  //  0006  MOVE	R9	R1
          0x5C280400,  //  0007  MOVE	R10	R2
          0x7C140A00,  //  0008  CALL	R5	5
          0x7C0C0400,  //  0009  CALL	R3	2
          0x80000000,  //  000A  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(expect_left_paren),
    /* K1   */  be_nested_str_weak(_process_parameters_core),
    /* K2   */  be_nested_str_weak(expect_right_paren),
    }),
    be_str_weak(_process_named_arguments_unified),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x8C100100,  //  0000  GETMET	R4	R0	K0
      0x7C100200,  //  0001  CALL	R4	1
      0x84100000,  //  0002  CLOSURE	R4	P0
      0x8C140101,  //  0003  GETMET	R5	R0	K1
      0x5C1C0400,  //  0004  MOVE	R7	R2
      0x5C200600,  //  0005  MOVE	R8	R3
      0x5C240800,  //  0006  MOVE	R9	R4
      0x7C140800,  //  0007  CALL	R5	4
      0x8C140102,  //  0008  GETMET	R5	R0	K2
      0x7C140200,  //  0009  CALL	R5	1
      0xA0000000,  //  000A  CLOSE	R0
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _create_instance_for_validation
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__create_instance_for_validation,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(symbol_table),
    /* K1   */  be_nested_str_weak(get),
    /* K2   */  be_nested_str_weak(instance),
    }),
    be_str_weak(_create_instance_for_validation),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x4C0C0000,  //  0004  LDNIL	R3
      0x200C0403,  //  0005  NE	R3	R2	R3
      0x780E0001,  //  0006  JMPF	R3	#0009
      0x880C0502,  //  0007  GETMBR	R3	R2	K2
      0x70020000,  //  0008  JMP		#000A
      0x4C0C0000,  //  0009  LDNIL	R3
      0x80040600,  //  000A  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_run
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_run,   /* name */
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
    /* K0   */  be_nested_str_weak(next),
    /* K1   */  be_nested_str_weak(expect_identifier),
    /* K2   */  be_nested_str_weak(_validate_object_reference),
    /* K3   */  be_nested_str_weak(run),
    /* K4   */  be_nested_str_weak(collect_inline_comment),
    /* K5   */  be_nested_str_weak(run_statements),
    /* K6   */  be_nested_str_weak(push),
    /* K7   */  be_nested_str_weak(name),
    /* K8   */  be_nested_str_weak(comment),
    }),
    be_str_weak(process_run),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x7C040200,  //  0003  CALL	R1	1
      0x8C080102,  //  0004  GETMET	R2	R0	K2
      0x5C100200,  //  0005  MOVE	R4	R1
      0x58140003,  //  0006  LDCONST	R5	K3
      0x7C080600,  //  0007  CALL	R2	3
      0x8C080104,  //  0008  GETMET	R2	R0	K4
      0x7C080200,  //  0009  CALL	R2	1
      0x880C0105,  //  000A  GETMBR	R3	R0	K5
      0x8C0C0706,  //  000B  GETMET	R3	R3	K6
      0x60140013,  //  000C  GETGBL	R5	G19
      0x7C140000,  //  000D  CALL	R5	0
      0x98160E01,  //  000E  SETIDX	R5	K7	R1
      0x98161002,  //  000F  SETIDX	R5	K8	R2
      0x7C0C0400,  //  0010  CALL	R3	2
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _create_symbol_by_return_type
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__create_symbol_by_return_type,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(symbol_table),
    /* K2   */  be_nested_str_weak(create_animation),
    /* K3   */  be_nested_str_weak(color),
    /* K4   */  be_nested_str_weak(create_color),
    /* K5   */  be_nested_str_weak(value_provider),
    /* K6   */  be_nested_str_weak(create_value_provider),
    /* K7   */  be_nested_str_weak(palette),
    /* K8   */  be_nested_str_weak(create_palette),
    /* K9   */  be_nested_str_weak(sequence),
    /* K10  */  be_nested_str_weak(create_sequence),
    /* K11  */  be_nested_str_weak(template),
    /* K12  */  be_nested_str_weak(create_template),
    /* K13  */  be_nested_str_weak(create_variable),
    }),
    be_str_weak(_create_symbol_by_return_type),
    &be_const_str_solidified,
    ( &(const binstruction[59]) {  /* code */
      0x1C100500,  //  0000  EQ	R4	R2	K0
      0x78120006,  //  0001  JMPF	R4	#0009
      0x88100101,  //  0002  GETMBR	R4	R0	K1
      0x8C100902,  //  0003  GETMET	R4	R4	K2
      0x5C180200,  //  0004  MOVE	R6	R1
      0x5C1C0600,  //  0005  MOVE	R7	R3
      0x7C100600,  //  0006  CALL	R4	3
      0x80040800,  //  0007  RET	1	R4
      0x70020030,  //  0008  JMP		#003A
      0x1C100503,  //  0009  EQ	R4	R2	K3
      0x78120006,  //  000A  JMPF	R4	#0012
      0x88100101,  //  000B  GETMBR	R4	R0	K1
      0x8C100904,  //  000C  GETMET	R4	R4	K4
      0x5C180200,  //  000D  MOVE	R6	R1
      0x5C1C0600,  //  000E  MOVE	R7	R3
      0x7C100600,  //  000F  CALL	R4	3
      0x80040800,  //  0010  RET	1	R4
      0x70020027,  //  0011  JMP		#003A
      0x1C100505,  //  0012  EQ	R4	R2	K5
      0x78120006,  //  0013  JMPF	R4	#001B
      0x88100101,  //  0014  GETMBR	R4	R0	K1
      0x8C100906,  //  0015  GETMET	R4	R4	K6
      0x5C180200,  //  0016  MOVE	R6	R1
      0x5C1C0600,  //  0017  MOVE	R7	R3
      0x7C100600,  //  0018  CALL	R4	3
      0x80040800,  //  0019  RET	1	R4
      0x7002001E,  //  001A  JMP		#003A
      0x1C100507,  //  001B  EQ	R4	R2	K7
      0x78120006,  //  001C  JMPF	R4	#0024
      0x88100101,  //  001D  GETMBR	R4	R0	K1
      0x8C100908,  //  001E  GETMET	R4	R4	K8
      0x5C180200,  //  001F  MOVE	R6	R1
      0x5C1C0600,  //  0020  MOVE	R7	R3
      0x7C100600,  //  0021  CALL	R4	3
      0x80040800,  //  0022  RET	1	R4
      0x70020015,  //  0023  JMP		#003A
      0x1C100509,  //  0024  EQ	R4	R2	K9
      0x78120005,  //  0025  JMPF	R4	#002C
      0x88100101,  //  0026  GETMBR	R4	R0	K1
      0x8C10090A,  //  0027  GETMET	R4	R4	K10
      0x5C180200,  //  0028  MOVE	R6	R1
      0x7C100400,  //  0029  CALL	R4	2
      0x80040800,  //  002A  RET	1	R4
      0x7002000D,  //  002B  JMP		#003A
      0x1C10050B,  //  002C  EQ	R4	R2	K11
      0x78120006,  //  002D  JMPF	R4	#0035
      0x88100101,  //  002E  GETMBR	R4	R0	K1
      0x8C10090C,  //  002F  GETMET	R4	R4	K12
      0x5C180200,  //  0030  MOVE	R6	R1
      0x4C1C0000,  //  0031  LDNIL	R7
      0x7C100600,  //  0032  CALL	R4	3
      0x80040800,  //  0033  RET	1	R4
      0x70020004,  //  0034  JMP		#003A
      0x88100101,  //  0035  GETMBR	R4	R0	K1
      0x8C10090D,  //  0036  GETMET	R4	R4	K13
      0x5C180200,  //  0037  MOVE	R6	R1
      0x7C100400,  //  0038  CALL	R4	2
      0x80040800,  //  0039  RET	1	R4
      0x80000000,  //  003A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: expect_left_paren
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_expect_left_paren,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(LEFT_PAREN),
    /* K5   */  be_nested_str_weak(next),
    /* K6   */  be_nested_str_weak(error),
    /* K7   */  be_nested_str_weak(Expected_X20_X27_X28_X27),
    }),
    be_str_weak(expect_left_paren),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0008,  //  0004  JMPF	R2	#000E
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x780A0002,  //  000A  JMPF	R2	#000E
      0x8C080105,  //  000B  GETMET	R2	R0	K5
      0x7C080200,  //  000C  CALL	R2	1
      0x70020002,  //  000D  JMP		#0011
      0x8C080106,  //  000E  GETMET	R2	R0	K6
      0x58100007,  //  000F  LDCONST	R4	K7
      0x7C080400,  //  0010  CALL	R2	2
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: validate_user_name
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_validate_user_name,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_str_weak(symbol_table),
    /* K1   */  be_nested_str_weak(get),
    /* K2   */  be_nested_str_weak(is_builtin),
    /* K3   */  be_nested_str_weak(type),
    /* K4   */  be_nested_str_weak(animation_dsl),
    /* K5   */  be_nested_str_weak(_symbol_entry),
    /* K6   */  be_nested_str_weak(TYPE_COLOR),
    /* K7   */  be_nested_str_weak(error),
    /* K8   */  be_nested_str_weak(Cannot_X20redefine_X20predefined_X20color_X20_X27_X25s_X27_X2E_X20Use_X20a_X20different_X20name_X20like_X20_X27_X25s_custom_X27_X20or_X20_X27my__X25s_X27),
    /* K9   */  be_nested_str_weak(Cannot_X20redefine_X20built_X2Din_X20symbol_X20_X27_X25s_X27_X20_X28type_X3A_X20_X25s_X29_X2E_X20Use_X20a_X20different_X20name_X20like_X20_X27_X25s_custom_X27_X20or_X20_X27my__X25s_X27),
    /* K10  */  be_nested_str_weak(Symbol_X20_X27_X25s_X27_X20is_X20already_X20defined_X20as_X20_X25s_X2E_X20Cannot_X20redefine_X20as_X20_X25s_X2E),
    /* K11  */  be_nested_str_weak(Token),
    /* K12  */  be_nested_str_weak(statement_keywords),
    /* K13  */  be_nested_str_weak(Cannot_X20use_X20DSL_X20keyword_X20_X27_X25s_X27_X20as_X20_X25s_X20name_X2E_X20Use_X20a_X20different_X20name_X20like_X20_X27_X25s_custom_X27_X20or_X20_X27my__X25s_X27),
    /* K14  */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(validate_user_name),
    &be_const_str_solidified,
    ( &(const binstruction[79]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x4C100000,  //  0004  LDNIL	R4
      0x1C100604,  //  0005  EQ	R4	R3	R4
      0x78120000,  //  0006  JMPF	R4	#0008
      0x7002002A,  //  0007  JMP		#0033
      0x88100702,  //  0008  GETMBR	R4	R3	K2
      0x78120010,  //  0009  JMPF	R4	#001B
      0x88100703,  //  000A  GETMBR	R4	R3	K3
      0xB8160800,  //  000B  GETNGBL	R5	K4
      0x88140B05,  //  000C  GETMBR	R5	R5	K5
      0x88140B06,  //  000D  GETMBR	R5	R5	K6
      0x1C100805,  //  000E  EQ	R4	R4	R5
      0x7812000A,  //  000F  JMPF	R4	#001B
      0x8C100107,  //  0010  GETMET	R4	R0	K7
      0x60180018,  //  0011  GETGBL	R6	G24
      0x581C0008,  //  0012  LDCONST	R7	K8
      0x5C200200,  //  0013  MOVE	R8	R1
      0x5C240200,  //  0014  MOVE	R9	R1
      0x5C280200,  //  0015  MOVE	R10	R1
      0x7C180800,  //  0016  CALL	R6	4
      0x7C100400,  //  0017  CALL	R4	2
      0x50100000,  //  0018  LDBOOL	R4	0	0
      0x80040800,  //  0019  RET	1	R4
      0x70020017,  //  001A  JMP		#0033
      0x88100702,  //  001B  GETMBR	R4	R3	K2
      0x7812000B,  //  001C  JMPF	R4	#0029
      0x8C100107,  //  001D  GETMET	R4	R0	K7
      0x60180018,  //  001E  GETGBL	R6	G24
      0x581C0009,  //  001F  LDCONST	R7	K9
      0x5C200200,  //  0020  MOVE	R8	R1
      0x88240703,  //  0021  GETMBR	R9	R3	K3
      0x5C280200,  //  0022  MOVE	R10	R1
      0x5C2C0200,  //  0023  MOVE	R11	R1
      0x7C180A00,  //  0024  CALL	R6	5
      0x7C100400,  //  0025  CALL	R4	2
      0x50100000,  //  0026  LDBOOL	R4	0	0
      0x80040800,  //  0027  RET	1	R4
      0x70020009,  //  0028  JMP		#0033
      0x8C100107,  //  0029  GETMET	R4	R0	K7
      0x60180018,  //  002A  GETGBL	R6	G24
      0x581C000A,  //  002B  LDCONST	R7	K10
      0x5C200200,  //  002C  MOVE	R8	R1
      0x88240703,  //  002D  GETMBR	R9	R3	K3
      0x5C280400,  //  002E  MOVE	R10	R2
      0x7C180800,  //  002F  CALL	R6	4
      0x7C100400,  //  0030  CALL	R4	2
      0x50100000,  //  0031  LDBOOL	R4	0	0
      0x80040800,  //  0032  RET	1	R4
      0x60100010,  //  0033  GETGBL	R4	G16
      0xB8160800,  //  0034  GETNGBL	R5	K4
      0x88140B0B,  //  0035  GETMBR	R5	R5	K11
      0x88140B0C,  //  0036  GETMBR	R5	R5	K12
      0x7C100200,  //  0037  CALL	R4	1
      0xA8020010,  //  0038  EXBLK	0	#004A
      0x5C140800,  //  0039  MOVE	R5	R4
      0x7C140000,  //  003A  CALL	R5	0
      0x1C180205,  //  003B  EQ	R6	R1	R5
      0x781A000B,  //  003C  JMPF	R6	#0049
      0x8C180107,  //  003D  GETMET	R6	R0	K7
      0x60200018,  //  003E  GETGBL	R8	G24
      0x5824000D,  //  003F  LDCONST	R9	K13
      0x5C280200,  //  0040  MOVE	R10	R1
      0x5C2C0400,  //  0041  MOVE	R11	R2
      0x5C300200,  //  0042  MOVE	R12	R1
      0x5C340200,  //  0043  MOVE	R13	R1
      0x7C200A00,  //  0044  CALL	R8	5
      0x7C180400,  //  0045  CALL	R6	2
      0x50180000,  //  0046  LDBOOL	R6	0	0
      0xA8040001,  //  0047  EXBLK	1	1
      0x80040C00,  //  0048  RET	1	R6
      0x7001FFEE,  //  0049  JMP		#0039
      0x5810000E,  //  004A  LDCONST	R4	K14
      0xAC100200,  //  004B  CATCH	R4	1	0
      0xB0080000,  //  004C  RAISE	2	R0	R0
      0x50100200,  //  004D  LDBOOL	R4	1	0
      0x80040800,  //  004E  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _validate_value_provider_reference
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__validate_value_provider_reference,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(symbol_table),
    /* K1   */  be_nested_str_weak(symbol_exists),
    /* K2   */  be_nested_str_weak(error),
    /* K3   */  be_nested_str_weak(Undefined_X20reference_X20_X27_X25s_X27_X20in_X20_X25s_X20statement_X2E_X20Make_X20sure_X20the_X20value_X20provider_X20or_X20animation_X20is_X20defined_X20before_X20use_X2E),
    /* K4   */  be_nested_str_weak(get),
    /* K5   */  be_nested_str_weak(type),
    /* K6   */  be_nested_str_weak(animation_dsl),
    /* K7   */  be_nested_str_weak(_symbol_entry),
    /* K8   */  be_nested_str_weak(TYPE_VALUE_PROVIDER),
    /* K9   */  be_nested_str_weak(TYPE_ANIMATION),
    /* K10  */  be_nested_str_weak(_X27_X25s_X27_X20in_X20_X25s_X20statement_X20is_X20not_X20a_X20value_X20provider_X20or_X20animation_X20instance_X2E_X20Only_X20value_X20provider_X20instances_X20_X28like_X20oscillators_X29_X20and_X20animation_X20instances_X20can_X20be_X20restarted_X2E),
    /* K11  */  be_nested_str_weak(Could_X20not_X20validate_X20_X27_X25s_X27_X20in_X20_X25s_X20statement_X3A_X20_X25s),
    }),
    be_str_weak(_validate_value_provider_reference),
    &be_const_str_solidified,
    ( &(const binstruction[69]) {  /* code */
      0xA8020034,  //  0000  EXBLK	0	#0036
      0x880C0100,  //  0001  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0002  GETMET	R3	R3	K1
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C0C0400,  //  0004  CALL	R3	2
      0x740E0009,  //  0005  JMPT	R3	#0010
      0x8C0C0102,  //  0006  GETMET	R3	R0	K2
      0x60140018,  //  0007  GETGBL	R5	G24
      0x58180003,  //  0008  LDCONST	R6	K3
      0x5C1C0200,  //  0009  MOVE	R7	R1
      0x5C200400,  //  000A  MOVE	R8	R2
      0x7C140600,  //  000B  CALL	R5	3
      0x7C0C0400,  //  000C  CALL	R3	2
      0x500C0000,  //  000D  LDBOOL	R3	0	0
      0xA8040001,  //  000E  EXBLK	1	1
      0x80040600,  //  000F  RET	1	R3
      0x880C0100,  //  0010  GETMBR	R3	R0	K0
      0x8C0C0704,  //  0011  GETMET	R3	R3	K4
      0x5C140200,  //  0012  MOVE	R5	R1
      0x7C0C0400,  //  0013  CALL	R3	2
      0x4C100000,  //  0014  LDNIL	R4
      0x20100604,  //  0015  NE	R4	R3	R4
      0x78120019,  //  0016  JMPF	R4	#0031
      0x88100705,  //  0017  GETMBR	R4	R3	K5
      0xB8160C00,  //  0018  GETNGBL	R5	K6
      0x88140B07,  //  0019  GETMBR	R5	R5	K7
      0x88140B08,  //  001A  GETMBR	R5	R5	K8
      0x1C100805,  //  001B  EQ	R4	R4	R5
      0x74120005,  //  001C  JMPT	R4	#0023
      0x88100705,  //  001D  GETMBR	R4	R3	K5
      0xB8160C00,  //  001E  GETNGBL	R5	K6
      0x88140B07,  //  001F  GETMBR	R5	R5	K7
      0x88140B09,  //  0020  GETMBR	R5	R5	K9
      0x1C100805,  //  0021  EQ	R4	R4	R5
      0x78120003,  //  0022  JMPF	R4	#0027
      0x50100200,  //  0023  LDBOOL	R4	1	0
      0xA8040001,  //  0024  EXBLK	1	1
      0x80040800,  //  0025  RET	1	R4
      0x70020009,  //  0026  JMP		#0031
      0x8C100102,  //  0027  GETMET	R4	R0	K2
      0x60180018,  //  0028  GETGBL	R6	G24
      0x581C000A,  //  0029  LDCONST	R7	K10
      0x5C200200,  //  002A  MOVE	R8	R1
      0x5C240400,  //  002B  MOVE	R9	R2
      0x7C180600,  //  002C  CALL	R6	3
      0x7C100400,  //  002D  CALL	R4	2
      0x50100000,  //  002E  LDBOOL	R4	0	0
      0xA8040001,  //  002F  EXBLK	1	1
      0x80040800,  //  0030  RET	1	R4
      0x50100200,  //  0031  LDBOOL	R4	1	0
      0xA8040001,  //  0032  EXBLK	1	1
      0x80040800,  //  0033  RET	1	R4
      0xA8040001,  //  0034  EXBLK	1	1
      0x7002000D,  //  0035  JMP		#0044
      0xAC0C0002,  //  0036  CATCH	R3	0	2
      0x7002000A,  //  0037  JMP		#0043
      0x8C140102,  //  0038  GETMET	R5	R0	K2
      0x601C0018,  //  0039  GETGBL	R7	G24
      0x5820000B,  //  003A  LDCONST	R8	K11
      0x5C240200,  //  003B  MOVE	R9	R1
      0x5C280400,  //  003C  MOVE	R10	R2
      0x5C2C0800,  //  003D  MOVE	R11	R4
      0x7C1C0800,  //  003E  CALL	R7	4
      0x7C140400,  //  003F  CALL	R5	2
      0x50140000,  //  0040  LDBOOL	R5	0	0
      0x80040A00,  //  0041  RET	1	R5
      0x70020000,  //  0042  JMP		#0044
      0xB0080000,  //  0043  RAISE	2	R0	R0
      0x80000000,  //  0044  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _validate_animation_factory_creates_animation
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__validate_animation_factory_creates_animation,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(symbol_table),
    /* K1   */  be_nested_str_weak(get),
    /* K2   */  be_nested_str_weak(type),
    /* K3   */  be_nested_str_weak(animation_dsl),
    /* K4   */  be_nested_str_weak(_symbol_entry),
    /* K5   */  be_nested_str_weak(TYPE_ANIMATION_CONSTRUCTOR),
    }),
    be_str_weak(_validate_animation_factory_creates_animation),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x4C0C0000,  //  0004  LDNIL	R3
      0x200C0403,  //  0005  NE	R3	R2	R3
      0x780E0005,  //  0006  JMPF	R3	#000D
      0x880C0502,  //  0007  GETMBR	R3	R2	K2
      0xB8120600,  //  0008  GETNGBL	R4	K3
      0x88100904,  //  0009  GETMBR	R4	R4	K4
      0x88100905,  //  000A  GETMBR	R4	R4	K5
      0x1C0C0604,  //  000B  EQ	R3	R3	R4
      0x740E0000,  //  000C  JMPT	R3	#000E
      0x500C0001,  //  000D  LDBOOL	R3	0	1
      0x500C0200,  //  000E  LDBOOL	R3	1	0
      0x80040600,  //  000F  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _determine_function_return_type
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__determine_function_return_type,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[19]) {     /* constants */
    /* K0   */  be_nested_str_weak(type),
    /* K1   */  be_nested_str_weak(animation_dsl),
    /* K2   */  be_nested_str_weak(_symbol_entry),
    /* K3   */  be_nested_str_weak(TYPE_ANIMATION_CONSTRUCTOR),
    /* K4   */  be_nested_str_weak(TYPE_ANIMATION),
    /* K5   */  be_nested_str_weak(animation),
    /* K6   */  be_nested_str_weak(TYPE_COLOR_CONSTRUCTOR),
    /* K7   */  be_nested_str_weak(TYPE_COLOR),
    /* K8   */  be_nested_str_weak(color),
    /* K9   */  be_nested_str_weak(TYPE_VALUE_PROVIDER_CONSTRUCTOR),
    /* K10  */  be_nested_str_weak(TYPE_VALUE_PROVIDER),
    /* K11  */  be_nested_str_weak(value_provider),
    /* K12  */  be_nested_str_weak(TYPE_PALETTE_CONSTANT),
    /* K13  */  be_nested_str_weak(TYPE_PALETTE),
    /* K14  */  be_nested_str_weak(palette),
    /* K15  */  be_nested_str_weak(TYPE_MATH_FUNCTION),
    /* K16  */  be_nested_str_weak(variable),
    /* K17  */  be_nested_str_weak(TYPE_USER_FUNCTION),
    /* K18  */  be_nested_str_weak(TYPE_TEMPLATE),
    }),
    be_str_weak(_determine_function_return_type),
    &be_const_str_solidified,
    ( &(const binstruction[81]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x20080202,  //  0001  NE	R2	R1	R2
      0x780A004C,  //  0002  JMPF	R2	#0050
      0x88080300,  //  0003  GETMBR	R2	R1	K0
      0xB80E0200,  //  0004  GETNGBL	R3	K1
      0x880C0702,  //  0005  GETMBR	R3	R3	K2
      0x880C0703,  //  0006  GETMBR	R3	R3	K3
      0x1C080403,  //  0007  EQ	R2	R2	R3
      0x740A0005,  //  0008  JMPT	R2	#000F
      0x88080300,  //  0009  GETMBR	R2	R1	K0
      0xB80E0200,  //  000A  GETNGBL	R3	K1
      0x880C0702,  //  000B  GETMBR	R3	R3	K2
      0x880C0704,  //  000C  GETMBR	R3	R3	K4
      0x1C080403,  //  000D  EQ	R2	R2	R3
      0x780A0001,  //  000E  JMPF	R2	#0011
      0x80060A00,  //  000F  RET	1	K5
      0x7002003E,  //  0010  JMP		#0050
      0x88080300,  //  0011  GETMBR	R2	R1	K0
      0xB80E0200,  //  0012  GETNGBL	R3	K1
      0x880C0702,  //  0013  GETMBR	R3	R3	K2
      0x880C0706,  //  0014  GETMBR	R3	R3	K6
      0x1C080403,  //  0015  EQ	R2	R2	R3
      0x740A0005,  //  0016  JMPT	R2	#001D
      0x88080300,  //  0017  GETMBR	R2	R1	K0
      0xB80E0200,  //  0018  GETNGBL	R3	K1
      0x880C0702,  //  0019  GETMBR	R3	R3	K2
      0x880C0707,  //  001A  GETMBR	R3	R3	K7
      0x1C080403,  //  001B  EQ	R2	R2	R3
      0x780A0001,  //  001C  JMPF	R2	#001F
      0x80061000,  //  001D  RET	1	K8
      0x70020030,  //  001E  JMP		#0050
      0x88080300,  //  001F  GETMBR	R2	R1	K0
      0xB80E0200,  //  0020  GETNGBL	R3	K1
      0x880C0702,  //  0021  GETMBR	R3	R3	K2
      0x880C0709,  //  0022  GETMBR	R3	R3	K9
      0x1C080403,  //  0023  EQ	R2	R2	R3
      0x740A0005,  //  0024  JMPT	R2	#002B
      0x88080300,  //  0025  GETMBR	R2	R1	K0
      0xB80E0200,  //  0026  GETNGBL	R3	K1
      0x880C0702,  //  0027  GETMBR	R3	R3	K2
      0x880C070A,  //  0028  GETMBR	R3	R3	K10
      0x1C080403,  //  0029  EQ	R2	R2	R3
      0x780A0001,  //  002A  JMPF	R2	#002D
      0x80061600,  //  002B  RET	1	K11
      0x70020022,  //  002C  JMP		#0050
      0x88080300,  //  002D  GETMBR	R2	R1	K0
      0xB80E0200,  //  002E  GETNGBL	R3	K1
      0x880C0702,  //  002F  GETMBR	R3	R3	K2
      0x880C070C,  //  0030  GETMBR	R3	R3	K12
      0x1C080403,  //  0031  EQ	R2	R2	R3
      0x740A0005,  //  0032  JMPT	R2	#0039
      0x88080300,  //  0033  GETMBR	R2	R1	K0
      0xB80E0200,  //  0034  GETNGBL	R3	K1
      0x880C0702,  //  0035  GETMBR	R3	R3	K2
      0x880C070D,  //  0036  GETMBR	R3	R3	K13
      0x1C080403,  //  0037  EQ	R2	R2	R3
      0x780A0001,  //  0038  JMPF	R2	#003B
      0x80061C00,  //  0039  RET	1	K14
      0x70020014,  //  003A  JMP		#0050
      0x88080300,  //  003B  GETMBR	R2	R1	K0
      0xB80E0200,  //  003C  GETNGBL	R3	K1
      0x880C0702,  //  003D  GETMBR	R3	R3	K2
      0x880C070F,  //  003E  GETMBR	R3	R3	K15
      0x1C080403,  //  003F  EQ	R2	R2	R3
      0x780A0001,  //  0040  JMPF	R2	#0043
      0x80062000,  //  0041  RET	1	K16
      0x7002000C,  //  0042  JMP		#0050
      0x88080300,  //  0043  GETMBR	R2	R1	K0
      0xB80E0200,  //  0044  GETNGBL	R3	K1
      0x880C0702,  //  0045  GETMBR	R3	R3	K2
      0x880C0711,  //  0046  GETMBR	R3	R3	K17
      0x1C080403,  //  0047  EQ	R2	R2	R3
      0x740A0005,  //  0048  JMPT	R2	#004F
      0x88080300,  //  0049  GETMBR	R2	R1	K0
      0xB80E0200,  //  004A  GETNGBL	R3	K1
      0x880C0702,  //  004B  GETMBR	R3	R3	K2
      0x880C0712,  //  004C  GETMBR	R3	R3	K18
      0x1C080403,  //  004D  EQ	R2	R2	R3
      0x780A0000,  //  004E  JMPF	R2	#0050
      0x80062000,  //  004F  RET	1	K16
      0x80062000,  //  0050  RET	1	K16
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_set
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_set,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(next),
    /* K1   */  be_nested_str_weak(expect_identifier),
    /* K2   */  be_nested_str_weak(validate_user_name),
    /* K3   */  be_nested_str_weak(variable),
    /* K4   */  be_nested_str_weak(skip_statement),
    /* K5   */  be_nested_str_weak(expect_assign),
    /* K6   */  be_nested_str_weak(process_value),
    /* K7   */  be_nested_str_weak(CONTEXT_VARIABLE),
    /* K8   */  be_nested_str_weak(collect_inline_comment),
    /* K9   */  be_nested_str_weak(_create_symbol_by_return_type),
    /* K10  */  be_nested_str_weak(return_type),
    /* K11  */  be_nested_str_weak(instance_for_validation),
    /* K12  */  be_nested_str_weak(get_reference),
    /* K13  */  be_nested_str_weak(_X25s_),
    /* K14  */  be_nested_str_weak(add),
    /* K15  */  be_nested_str_weak(var_X20_X25s_X20_X3D_X20_X25s_X25s),
    /* K16  */  be_nested_str_weak(expr),
    }),
    be_str_weak(process_set),
    &be_const_str_solidified,
    ( &(const binstruction[43]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x7C040200,  //  0003  CALL	R1	1
      0x8C080102,  //  0004  GETMET	R2	R0	K2
      0x5C100200,  //  0005  MOVE	R4	R1
      0x58140003,  //  0006  LDCONST	R5	K3
      0x7C080600,  //  0007  CALL	R2	3
      0x740A0002,  //  0008  JMPT	R2	#000C
      0x8C080104,  //  0009  GETMET	R2	R0	K4
      0x7C080200,  //  000A  CALL	R2	1
      0x80000400,  //  000B  RET	0
      0x8C080105,  //  000C  GETMET	R2	R0	K5
      0x7C080200,  //  000D  CALL	R2	1
      0x8C080106,  //  000E  GETMET	R2	R0	K6
      0x88100107,  //  000F  GETMBR	R4	R0	K7
      0x7C080400,  //  0010  CALL	R2	2
      0x8C0C0108,  //  0011  GETMET	R3	R0	K8
      0x7C0C0200,  //  0012  CALL	R3	1
      0x8C100109,  //  0013  GETMET	R4	R0	K9
      0x5C180200,  //  0014  MOVE	R6	R1
      0x881C050A,  //  0015  GETMBR	R7	R2	K10
      0x8820050B,  //  0016  GETMBR	R8	R2	K11
      0x7C100800,  //  0017  CALL	R4	4
      0x4C140000,  //  0018  LDNIL	R5
      0x20140805,  //  0019  NE	R5	R4	R5
      0x78160002,  //  001A  JMPF	R5	#001E
      0x8C14090C,  //  001B  GETMET	R5	R4	K12
      0x7C140200,  //  001C  CALL	R5	1
      0x70020003,  //  001D  JMP		#0022
      0x60140018,  //  001E  GETGBL	R5	G24
      0x5818000D,  //  001F  LDCONST	R6	K13
      0x5C1C0200,  //  0020  MOVE	R7	R1
      0x7C140400,  //  0021  CALL	R5	2
      0x8C18010E,  //  0022  GETMET	R6	R0	K14
      0x60200018,  //  0023  GETGBL	R8	G24
      0x5824000F,  //  0024  LDCONST	R9	K15
      0x5C280A00,  //  0025  MOVE	R10	R5
      0x882C0510,  //  0026  GETMBR	R11	R2	K16
      0x5C300600,  //  0027  MOVE	R12	R3
      0x7C200800,  //  0028  CALL	R8	4
      0x7C180400,  //  0029  CALL	R6	2
      0x80000000,  //  002A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_sequence_assignment_fluent
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_sequence_assignment_fluent,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(expect_identifier),
    /* K1   */  be_nested_str_weak(expect_dot),
    /* K2   */  be_nested_str_weak(expect_assign),
    /* K3   */  be_nested_str_weak(process_value),
    /* K4   */  be_nested_str_weak(CONTEXT_PROPERTY),
    /* K5   */  be_nested_str_weak(collect_inline_comment),
    /* K6   */  be_nested_str_weak(def_X20_X28engine_X29_X20_X25s__X2E_X25s_X20_X3D_X20_X25s_X20end),
    /* K7   */  be_nested_str_weak(expr),
    /* K8   */  be_nested_str_weak(add),
    /* K9   */  be_nested_str_weak(_X25s_X2Epush_closure_step_X28_X25s_X29_X25s),
    /* K10  */  be_nested_str_weak(get_indent),
    }),
    be_str_weak(process_sequence_assignment_fluent),
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C080101,  //  0002  GETMET	R2	R0	K1
      0x7C080200,  //  0003  CALL	R2	1
      0x8C080100,  //  0004  GETMET	R2	R0	K0
      0x7C080200,  //  0005  CALL	R2	1
      0x8C0C0102,  //  0006  GETMET	R3	R0	K2
      0x7C0C0200,  //  0007  CALL	R3	1
      0x8C0C0103,  //  0008  GETMET	R3	R0	K3
      0x88140104,  //  0009  GETMBR	R5	R0	K4
      0x7C0C0400,  //  000A  CALL	R3	2
      0x8C100105,  //  000B  GETMET	R4	R0	K5
      0x7C100200,  //  000C  CALL	R4	1
      0x60140018,  //  000D  GETGBL	R5	G24
      0x58180006,  //  000E  LDCONST	R6	K6
      0x5C1C0200,  //  000F  MOVE	R7	R1
      0x5C200400,  //  0010  MOVE	R8	R2
      0x88240707,  //  0011  GETMBR	R9	R3	K7
      0x7C140800,  //  0012  CALL	R5	4
      0x8C180108,  //  0013  GETMET	R6	R0	K8
      0x60200018,  //  0014  GETGBL	R8	G24
      0x58240009,  //  0015  LDCONST	R9	K9
      0x8C28010A,  //  0016  GETMET	R10	R0	K10
      0x7C280200,  //  0017  CALL	R10	1
      0x5C2C0A00,  //  0018  MOVE	R11	R5
      0x5C300800,  //  0019  MOVE	R12	R4
      0x7C200800,  //  001A  CALL	R8	4
      0x7C180400,  //  001B  CALL	R6	2
      0x80000000,  //  001C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_percentage_value
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_percentage_value,   /* name */
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
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(PERCENTAGE),
    /* K5   */  be_nested_str_weak(value),
    /* K6   */  be_nested_str_weak(next),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str_weak(NUMBER),
    /* K9   */  be_nested_str_weak(error),
    /* K10  */  be_nested_str_weak(Expected_X20percentage_X20value),
    }),
    be_str_weak(process_percentage_value),
    &be_const_str_solidified,
    ( &(const binstruction[52]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0015,  //  0004  JMPF	R2	#001B
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x780A000F,  //  000A  JMPF	R2	#001B
      0x88080305,  //  000B  GETMBR	R2	R1	K5
      0x8C0C0106,  //  000C  GETMET	R3	R0	K6
      0x7C0C0200,  //  000D  CALL	R3	1
      0x600C000A,  //  000E  GETGBL	R3	G10
      0x5411FFFD,  //  000F  LDINT	R4	-2
      0x40120E04,  //  0010  CONNECT	R4	K7	R4
      0x94100404,  //  0011  GETIDX	R4	R2	R4
      0x7C0C0200,  //  0012  CALL	R3	1
      0x60100009,  //  0013  GETGBL	R4	G9
      0x541600FE,  //  0014  LDINT	R5	255
      0x08140605,  //  0015  MUL	R5	R3	R5
      0x541A0063,  //  0016  LDINT	R6	100
      0x0C140A06,  //  0017  DIV	R5	R5	R6
      0x7C100200,  //  0018  CALL	R4	1
      0x80040800,  //  0019  RET	1	R4
      0x70020017,  //  001A  JMP		#0033
      0x4C080000,  //  001B  LDNIL	R2
      0x20080202,  //  001C  NE	R2	R1	R2
      0x780A000F,  //  001D  JMPF	R2	#002E
      0x88080301,  //  001E  GETMBR	R2	R1	K1
      0xB80E0400,  //  001F  GETNGBL	R3	K2
      0x880C0703,  //  0020  GETMBR	R3	R3	K3
      0x880C0708,  //  0021  GETMBR	R3	R3	K8
      0x1C080403,  //  0022  EQ	R2	R2	R3
      0x780A0009,  //  0023  JMPF	R2	#002E
      0x88080305,  //  0024  GETMBR	R2	R1	K5
      0x8C0C0106,  //  0025  GETMET	R3	R0	K6
      0x7C0C0200,  //  0026  CALL	R3	1
      0x600C0009,  //  0027  GETGBL	R3	G9
      0x6010000A,  //  0028  GETGBL	R4	G10
      0x5C140400,  //  0029  MOVE	R5	R2
      0x7C100200,  //  002A  CALL	R4	1
      0x7C0C0200,  //  002B  CALL	R3	1
      0x80040600,  //  002C  RET	1	R3
      0x70020004,  //  002D  JMP		#0033
      0x8C080109,  //  002E  GETMET	R2	R0	K9
      0x5810000A,  //  002F  LDCONST	R4	K10
      0x7C080400,  //  0030  CALL	R2	2
      0x540A00FE,  //  0031  LDINT	R2	255
      0x80040400,  //  0032  RET	1	R2
      0x80000000,  //  0033  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: generate_template_function
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_generate_template_function,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[24]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(engine),
    /* K2   */  be_nested_str_weak(_X2C_X20_X25s_),
    /* K3   */  be_nested_str_weak(stop_iteration),
    /* K4   */  be_nested_str_weak(add),
    /* K5   */  be_nested_str_weak(_X23_X20Template_X20function_X3A_X20_X25s),
    /* K6   */  be_nested_str_weak(def_X20_X25s_template_X28_X25s_X29),
    /* K7   */  be_nested_str_weak(animation_dsl),
    /* K8   */  be_nested_str_weak(SimpleDSLTranspiler),
    /* K9   */  be_nested_str_weak(symbol_table),
    /* K10  */  be_nested_str_weak(_symbol_table),
    /* K11  */  be_nested_str_weak(strip_initialized),
    /* K12  */  be_nested_str_weak(create_variable),
    /* K13  */  be_nested_str_weak(transpile_template_body),
    /* K14  */  be_nested_str_weak(split),
    /* K15  */  be_nested_str_weak(_X0A),
    /* K16  */  be_const_int(0),
    /* K17  */  be_nested_str_weak(_X20_X20_X25s),
    /* K18  */  be_nested_str_weak(errors),
    /* K19  */  be_nested_str_weak(error),
    /* K20  */  be_nested_str_weak(Template_X20_X27_X25s_X27_X20body_X20error_X3A_X20_X25s),
    /* K21  */  be_nested_str_weak(end),
    /* K22  */  be_nested_str_weak(),
    /* K23  */  be_nested_str_weak(animation_X2Eregister_user_function_X28_X27_X25s_X27_X2C_X20_X25s_template_X29),
    }),
    be_str_weak(generate_template_function),
    &be_const_str_solidified,
    ( &(const binstruction[119]) {  /* code */
      0xA4160000,  //  0000  IMPORT	R5	K0
      0x58180001,  //  0001  LDCONST	R6	K1
      0x601C0010,  //  0002  GETGBL	R7	G16
      0x5C200400,  //  0003  MOVE	R8	R2
      0x7C1C0200,  //  0004  CALL	R7	1
      0xA8020007,  //  0005  EXBLK	0	#000E
      0x5C200E00,  //  0006  MOVE	R8	R7
      0x7C200000,  //  0007  CALL	R8	0
      0x60240018,  //  0008  GETGBL	R9	G24
      0x58280002,  //  0009  LDCONST	R10	K2
      0x5C2C1000,  //  000A  MOVE	R11	R8
      0x7C240400,  //  000B  CALL	R9	2
      0x00180C09,  //  000C  ADD	R6	R6	R9
      0x7001FFF7,  //  000D  JMP		#0006
      0x581C0003,  //  000E  LDCONST	R7	K3
      0xAC1C0200,  //  000F  CATCH	R7	1	0
      0xB0080000,  //  0010  RAISE	2	R0	R0
      0x8C1C0104,  //  0011  GETMET	R7	R0	K4
      0x60240018,  //  0012  GETGBL	R9	G24
      0x58280005,  //  0013  LDCONST	R10	K5
      0x5C2C0200,  //  0014  MOVE	R11	R1
      0x7C240400,  //  0015  CALL	R9	2
      0x7C1C0400,  //  0016  CALL	R7	2
      0x8C1C0104,  //  0017  GETMET	R7	R0	K4
      0x60240018,  //  0018  GETGBL	R9	G24
      0x58280006,  //  0019  LDCONST	R10	K6
      0x5C2C0200,  //  001A  MOVE	R11	R1
      0x5C300C00,  //  001B  MOVE	R12	R6
      0x7C240600,  //  001C  CALL	R9	3
      0x7C1C0400,  //  001D  CALL	R7	2
      0xB81E0E00,  //  001E  GETNGBL	R7	K7
      0x8C1C0F08,  //  001F  GETMET	R7	R7	K8
      0x5C240800,  //  0020  MOVE	R9	R4
      0x7C1C0400,  //  0021  CALL	R7	2
      0xB8220E00,  //  0022  GETNGBL	R8	K7
      0x8C20110A,  //  0023  GETMET	R8	R8	K10
      0x7C200200,  //  0024  CALL	R8	1
      0x901E1208,  //  0025  SETMBR	R7	K9	R8
      0x50200200,  //  0026  LDBOOL	R8	1	0
      0x901E1608,  //  0027  SETMBR	R7	K11	R8
      0x60200010,  //  0028  GETGBL	R8	G16
      0x5C240400,  //  0029  MOVE	R9	R2
      0x7C200200,  //  002A  CALL	R8	1
      0xA8020006,  //  002B  EXBLK	0	#0033
      0x5C241000,  //  002C  MOVE	R9	R8
      0x7C240000,  //  002D  CALL	R9	0
      0x88280F09,  //  002E  GETMBR	R10	R7	K9
      0x8C28150C,  //  002F  GETMET	R10	R10	K12
      0x5C301200,  //  0030  MOVE	R12	R9
      0x7C280400,  //  0031  CALL	R10	2
      0x7001FFF8,  //  0032  JMP		#002C
      0x58200003,  //  0033  LDCONST	R8	K3
      0xAC200200,  //  0034  CATCH	R8	1	0
      0xB0080000,  //  0035  RAISE	2	R0	R0
      0x8C200F0D,  //  0036  GETMET	R8	R7	K13
      0x7C200200,  //  0037  CALL	R8	1
      0x4C240000,  //  0038  LDNIL	R9
      0x20241009,  //  0039  NE	R9	R8	R9
      0x78260019,  //  003A  JMPF	R9	#0055
      0x8C240B0E,  //  003B  GETMET	R9	R5	K14
      0x5C2C1000,  //  003C  MOVE	R11	R8
      0x5830000F,  //  003D  LDCONST	R12	K15
      0x7C240600,  //  003E  CALL	R9	3
      0x60280010,  //  003F  GETGBL	R10	G16
      0x5C2C1200,  //  0040  MOVE	R11	R9
      0x7C280200,  //  0041  CALL	R10	1
      0xA802000D,  //  0042  EXBLK	0	#0051
      0x5C2C1400,  //  0043  MOVE	R11	R10
      0x7C2C0000,  //  0044  CALL	R11	0
      0x6030000C,  //  0045  GETGBL	R12	G12
      0x5C341600,  //  0046  MOVE	R13	R11
      0x7C300200,  //  0047  CALL	R12	1
      0x24301910,  //  0048  GT	R12	R12	K16
      0x78320005,  //  0049  JMPF	R12	#0050
      0x8C300104,  //  004A  GETMET	R12	R0	K4
      0x60380018,  //  004B  GETGBL	R14	G24
      0x583C0011,  //  004C  LDCONST	R15	K17
      0x5C401600,  //  004D  MOVE	R16	R11
      0x7C380400,  //  004E  CALL	R14	2
      0x7C300400,  //  004F  CALL	R12	2
      0x7001FFF1,  //  0050  JMP		#0043
      0x58280003,  //  0051  LDCONST	R10	K3
      0xAC280200,  //  0052  CATCH	R10	1	0
      0xB0080000,  //  0053  RAISE	2	R0	R0
      0x70020010,  //  0054  JMP		#0066
      0x60240010,  //  0055  GETGBL	R9	G16
      0x88280F12,  //  0056  GETMBR	R10	R7	K18
      0x7C240200,  //  0057  CALL	R9	1
      0xA8020009,  //  0058  EXBLK	0	#0063
      0x5C281200,  //  0059  MOVE	R10	R9
      0x7C280000,  //  005A  CALL	R10	0
      0x8C2C0113,  //  005B  GETMET	R11	R0	K19
      0x60340018,  //  005C  GETGBL	R13	G24
      0x58380014,  //  005D  LDCONST	R14	K20
      0x5C3C0200,  //  005E  MOVE	R15	R1
      0x5C401400,  //  005F  MOVE	R16	R10
      0x7C340600,  //  0060  CALL	R13	3
      0x7C2C0400,  //  0061  CALL	R11	2
      0x7001FFF5,  //  0062  JMP		#0059
      0x58240003,  //  0063  LDCONST	R9	K3
      0xAC240200,  //  0064  CATCH	R9	1	0
      0xB0080000,  //  0065  RAISE	2	R0	R0
      0x8C240104,  //  0066  GETMET	R9	R0	K4
      0x582C0015,  //  0067  LDCONST	R11	K21
      0x7C240400,  //  0068  CALL	R9	2
      0x8C240104,  //  0069  GETMET	R9	R0	K4
      0x582C0016,  //  006A  LDCONST	R11	K22
      0x7C240400,  //  006B  CALL	R9	2
      0x8C240104,  //  006C  GETMET	R9	R0	K4
      0x602C0018,  //  006D  GETGBL	R11	G24
      0x58300017,  //  006E  LDCONST	R12	K23
      0x5C340200,  //  006F  MOVE	R13	R1
      0x5C380200,  //  0070  MOVE	R14	R1
      0x7C2C0600,  //  0071  CALL	R11	3
      0x7C240400,  //  0072  CALL	R9	2
      0x8C240104,  //  0073  GETMET	R9	R0	K4
      0x582C0016,  //  0074  LDCONST	R11	K22
      0x7C240400,  //  0075  CALL	R9	2
      0x80000000,  //  0076  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_property_assignment
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_property_assignment,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[38]) {     /* constants */
    /* K0   */  be_nested_str_weak(expect_identifier),
    /* K1   */  be_nested_str_weak(current),
    /* K2   */  be_nested_str_weak(type),
    /* K3   */  be_nested_str_weak(animation_dsl),
    /* K4   */  be_nested_str_weak(Token),
    /* K5   */  be_nested_str_weak(LEFT_PAREN),
    /* K6   */  be_nested_str_weak(log),
    /* K7   */  be_nested_str_weak(process_function_arguments),
    /* K8   */  be_nested_str_weak(collect_inline_comment),
    /* K9   */  be_nested_str_weak(process_log_call),
    /* K10  */  be_nested_str_weak(standalone),
    /* K11  */  be_nested_str_weak(add),
    /* K12  */  be_nested_str_weak(symbol_table),
    /* K13  */  be_nested_str_weak(get),
    /* K14  */  be_nested_str_weak(_symbol_entry),
    /* K15  */  be_nested_str_weak(TYPE_TEMPLATE),
    /* K16  */  be_nested_str_weak(),
    /* K17  */  be_nested_str_weak(engine_X2C_X20_X25s),
    /* K18  */  be_nested_str_weak(engine),
    /* K19  */  be_nested_str_weak(_X25s_template_X28_X25s_X29_X25s),
    /* K20  */  be_nested_str_weak(has_template_calls),
    /* K21  */  be_nested_str_weak(error),
    /* K22  */  be_nested_str_weak(Standalone_X20function_X20calls_X20are_X20only_X20supported_X20for_X20templates_X2E_X20_X27_X25s_X27_X20is_X20not_X20a_X20template_X2E),
    /* K23  */  be_nested_str_weak(skip_statement),
    /* K24  */  be_nested_str_weak(DOT),
    /* K25  */  be_nested_str_weak(next),
    /* K26  */  be_nested_str_weak(contains),
    /* K27  */  be_nested_str_weak(instance),
    /* K28  */  be_nested_str_weak(_validate_single_parameter),
    /* K29  */  be_nested_str_weak(TYPE_SEQUENCE),
    /* K30  */  be_nested_str_weak(Sequences_X20like_X20_X27_X25s_X27_X20do_X20not_X20have_X20properties_X2E_X20Property_X20assignments_X20are_X20only_X20valid_X20for_X20animations_X20and_X20color_X20providers_X2E),
    /* K31  */  be_nested_str_weak(expect_assign),
    /* K32  */  be_nested_str_weak(process_value),
    /* K33  */  be_nested_str_weak(CONTEXT_PROPERTY),
    /* K34  */  be_nested_str_weak(get_reference),
    /* K35  */  be_nested_str_weak(_X25s_X2E_X25s_X20_X3D_X20_X25s_X25s),
    /* K36  */  be_nested_str_weak(expr),
    /* K37  */  be_nested_str_weak(Expected_X20property_X20assignment_X20for_X20_X27_X25s_X27_X20but_X20found_X20no_X20dot),
    }),
    be_str_weak(process_property_assignment),
    &be_const_str_solidified,
    ( &(const binstruction[164]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C080101,  //  0002  GETMET	R2	R0	K1
      0x7C080200,  //  0003  CALL	R2	1
      0x4C0C0000,  //  0004  LDNIL	R3
      0x20080403,  //  0005  NE	R2	R2	R3
      0x780A0045,  //  0006  JMPF	R2	#004D
      0x8C080101,  //  0007  GETMET	R2	R0	K1
      0x7C080200,  //  0008  CALL	R2	1
      0x88080502,  //  0009  GETMBR	R2	R2	K2
      0xB80E0600,  //  000A  GETNGBL	R3	K3
      0x880C0704,  //  000B  GETMBR	R3	R3	K4
      0x880C0705,  //  000C  GETMBR	R3	R3	K5
      0x1C080403,  //  000D  EQ	R2	R2	R3
      0x780A003D,  //  000E  JMPF	R2	#004D
      0x1C080306,  //  000F  EQ	R2	R1	K6
      0x780A000D,  //  0010  JMPF	R2	#001F
      0x8C080107,  //  0011  GETMET	R2	R0	K7
      0x50100000,  //  0012  LDBOOL	R4	0	0
      0x7C080400,  //  0013  CALL	R2	2
      0x8C0C0108,  //  0014  GETMET	R3	R0	K8
      0x7C0C0200,  //  0015  CALL	R3	1
      0x8C100109,  //  0016  GETMET	R4	R0	K9
      0x5C180400,  //  0017  MOVE	R6	R2
      0x581C000A,  //  0018  LDCONST	R7	K10
      0x5C200600,  //  0019  MOVE	R8	R3
      0x7C100800,  //  001A  CALL	R4	4
      0x8C14010B,  //  001B  GETMET	R5	R0	K11
      0x5C1C0800,  //  001C  MOVE	R7	R4
      0x7C140400,  //  001D  CALL	R5	2
      0x80000A00,  //  001E  RET	0
      0x8808010C,  //  001F  GETMBR	R2	R0	K12
      0x8C08050D,  //  0020  GETMET	R2	R2	K13
      0x5C100200,  //  0021  MOVE	R4	R1
      0x7C080400,  //  0022  CALL	R2	2
      0x4C0C0000,  //  0023  LDNIL	R3
      0x200C0403,  //  0024  NE	R3	R2	R3
      0x780E001D,  //  0025  JMPF	R3	#0044
      0x880C0502,  //  0026  GETMBR	R3	R2	K2
      0xB8120600,  //  0027  GETNGBL	R4	K3
      0x8810090E,  //  0028  GETMBR	R4	R4	K14
      0x8810090F,  //  0029  GETMBR	R4	R4	K15
      0x1C0C0604,  //  002A  EQ	R3	R3	R4
      0x780E0017,  //  002B  JMPF	R3	#0044
      0x8C0C0107,  //  002C  GETMET	R3	R0	K7
      0x50140000,  //  002D  LDBOOL	R5	0	0
      0x7C0C0400,  //  002E  CALL	R3	2
      0x20100710,  //  002F  NE	R4	R3	K16
      0x78120004,  //  0030  JMPF	R4	#0036
      0x60100018,  //  0031  GETGBL	R4	G24
      0x58140011,  //  0032  LDCONST	R5	K17
      0x5C180600,  //  0033  MOVE	R6	R3
      0x7C100400,  //  0034  CALL	R4	2
      0x70020000,  //  0035  JMP		#0037
      0x58100012,  //  0036  LDCONST	R4	K18
      0x8C140108,  //  0037  GETMET	R5	R0	K8
      0x7C140200,  //  0038  CALL	R5	1
      0x8C18010B,  //  0039  GETMET	R6	R0	K11
      0x60200018,  //  003A  GETGBL	R8	G24
      0x58240013,  //  003B  LDCONST	R9	K19
      0x5C280200,  //  003C  MOVE	R10	R1
      0x5C2C0800,  //  003D  MOVE	R11	R4
      0x5C300A00,  //  003E  MOVE	R12	R5
      0x7C200800,  //  003F  CALL	R8	4
      0x7C180400,  //  0040  CALL	R6	2
      0x50180200,  //  0041  LDBOOL	R6	1	0
      0x90022806,  //  0042  SETMBR	R0	K20	R6
      0x70020007,  //  0043  JMP		#004C
      0x8C0C0115,  //  0044  GETMET	R3	R0	K21
      0x60140018,  //  0045  GETGBL	R5	G24
      0x58180016,  //  0046  LDCONST	R6	K22
      0x5C1C0200,  //  0047  MOVE	R7	R1
      0x7C140400,  //  0048  CALL	R5	2
      0x7C0C0400,  //  0049  CALL	R3	2
      0x8C0C0117,  //  004A  GETMET	R3	R0	K23
      0x7C0C0200,  //  004B  CALL	R3	1
      0x80000600,  //  004C  RET	0
      0x8C080101,  //  004D  GETMET	R2	R0	K1
      0x7C080200,  //  004E  CALL	R2	1
      0x4C0C0000,  //  004F  LDNIL	R3
      0x20080403,  //  0050  NE	R2	R2	R3
      0x780A0048,  //  0051  JMPF	R2	#009B
      0x8C080101,  //  0052  GETMET	R2	R0	K1
      0x7C080200,  //  0053  CALL	R2	1
      0x88080502,  //  0054  GETMBR	R2	R2	K2
      0xB80E0600,  //  0055  GETNGBL	R3	K3
      0x880C0704,  //  0056  GETMBR	R3	R3	K4
      0x880C0718,  //  0057  GETMBR	R3	R3	K24
      0x1C080403,  //  0058  EQ	R2	R2	R3
      0x780A0040,  //  0059  JMPF	R2	#009B
      0x8C080119,  //  005A  GETMET	R2	R0	K25
      0x7C080200,  //  005B  CALL	R2	1
      0x8C080100,  //  005C  GETMET	R2	R0	K0
      0x7C080200,  //  005D  CALL	R2	1
      0x880C010C,  //  005E  GETMBR	R3	R0	K12
      0x8C0C071A,  //  005F  GETMET	R3	R3	K26
      0x5C140200,  //  0060  MOVE	R5	R1
      0x7C0C0400,  //  0061  CALL	R3	2
      0x780E0022,  //  0062  JMPF	R3	#0086
      0x880C010C,  //  0063  GETMBR	R3	R0	K12
      0x8C0C070D,  //  0064  GETMET	R3	R3	K13
      0x5C140200,  //  0065  MOVE	R5	R1
      0x7C0C0400,  //  0066  CALL	R3	2
      0x4C100000,  //  0067  LDNIL	R4
      0x20100604,  //  0068  NE	R4	R3	R4
      0x7812000C,  //  0069  JMPF	R4	#0077
      0x8810071B,  //  006A  GETMBR	R4	R3	K27
      0x4C140000,  //  006B  LDNIL	R5
      0x20100805,  //  006C  NE	R4	R4	R5
      0x78120008,  //  006D  JMPF	R4	#0077
      0x60100005,  //  006E  GETGBL	R4	G5
      0x8814071B,  //  006F  GETMBR	R5	R3	K27
      0x7C100200,  //  0070  CALL	R4	1
      0x8C14011C,  //  0071  GETMET	R5	R0	K28
      0x5C1C0800,  //  0072  MOVE	R7	R4
      0x5C200400,  //  0073  MOVE	R8	R2
      0x8824071B,  //  0074  GETMBR	R9	R3	K27
      0x7C140800,  //  0075  CALL	R5	4
      0x7002000E,  //  0076  JMP		#0086
      0x4C100000,  //  0077  LDNIL	R4
      0x20100604,  //  0078  NE	R4	R3	R4
      0x7812000B,  //  0079  JMPF	R4	#0086
      0x88100702,  //  007A  GETMBR	R4	R3	K2
      0xB8160600,  //  007B  GETNGBL	R5	K3
      0x88140B0E,  //  007C  GETMBR	R5	R5	K14
      0x88140B1D,  //  007D  GETMBR	R5	R5	K29
      0x1C100805,  //  007E  EQ	R4	R4	R5
      0x78120005,  //  007F  JMPF	R4	#0086
      0x8C100115,  //  0080  GETMET	R4	R0	K21
      0x60180018,  //  0081  GETGBL	R6	G24
      0x581C001E,  //  0082  LDCONST	R7	K30
      0x5C200200,  //  0083  MOVE	R8	R1
      0x7C180400,  //  0084  CALL	R6	2
      0x7C100400,  //  0085  CALL	R4	2
      0x8C0C011F,  //  0086  GETMET	R3	R0	K31
      0x7C0C0200,  //  0087  CALL	R3	1
      0x8C0C0120,  //  0088  GETMET	R3	R0	K32
      0x88140121,  //  0089  GETMBR	R5	R0	K33
      0x7C0C0400,  //  008A  CALL	R3	2
      0x8C100108,  //  008B  GETMET	R4	R0	K8
      0x7C100200,  //  008C  CALL	R4	1
      0x8814010C,  //  008D  GETMBR	R5	R0	K12
      0x8C140B22,  //  008E  GETMET	R5	R5	K34
      0x5C1C0200,  //  008F  MOVE	R7	R1
      0x7C140400,  //  0090  CALL	R5	2
      0x8C18010B,  //  0091  GETMET	R6	R0	K11
      0x60200018,  //  0092  GETGBL	R8	G24
      0x58240023,  //  0093  LDCONST	R9	K35
      0x5C280A00,  //  0094  MOVE	R10	R5
      0x5C2C0400,  //  0095  MOVE	R11	R2
      0x88300724,  //  0096  GETMBR	R12	R3	K36
      0x5C340800,  //  0097  MOVE	R13	R4
      0x7C200A00,  //  0098  CALL	R8	5
      0x7C180400,  //  0099  CALL	R6	2
      0x70020007,  //  009A  JMP		#00A3
      0x8C080115,  //  009B  GETMET	R2	R0	K21
      0x60100018,  //  009C  GETGBL	R4	G24
      0x58140025,  //  009D  LDCONST	R5	K37
      0x5C180200,  //  009E  MOVE	R6	R1
      0x7C100400,  //  009F  CALL	R4	2
      0x7C080400,  //  00A0  CALL	R2	2
      0x8C080117,  //  00A1  GETMET	R2	R0	K23
      0x7C080200,  //  00A2  CALL	R2	1
      0x80000000,  //  00A3  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: transform_expression_for_closure
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_transform_expression_for_closure,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(_X2B_X2B_X2B_X20transform_expression_for_closure_X20_X27_X25s_X27),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(find),
    /* K4   */  be_nested_str_weak(animation_X2E),
    /* K5   */  be_nested_str_weak(is_identifier_char),
    /* K6   */  be_nested_str_weak(_),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str_weak(_X28engine_X29),
    /* K9   */  be_nested_str_weak(animation_X2Eresolve_X28),
    /* K10  */  be_nested_str_weak(animation_X2Eresolve_X28_X25s_X29),
    /* K11  */  be_nested_str_weak(),
    /* K12  */  be_const_int(2147483647),
    /* K13  */  be_nested_str_weak(_X3D_X3D_X3D_X20transform_expression_for_closure_X20_X27_X25s_X27),
    }),
    be_str_weak(transform_expression_for_closure),
    &be_const_str_solidified,
    ( &(const binstruction[181]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x600C0001,  //  0001  GETGBL	R3	G1
      0x60100018,  //  0002  GETGBL	R4	G24
      0x58140001,  //  0003  LDCONST	R5	K1
      0x5C180200,  //  0004  MOVE	R6	R1
      0x7C100400,  //  0005  CALL	R4	2
      0x7C0C0200,  //  0006  CALL	R3	1
      0x5C0C0200,  //  0007  MOVE	R3	R1
      0x58100002,  //  0008  LDCONST	R4	K2
      0x58140002,  //  0009  LDCONST	R5	K2
      0x50180200,  //  000A  LDBOOL	R6	1	0
      0x781A002D,  //  000B  JMPF	R6	#003A
      0x8C180503,  //  000C  GETMET	R6	R2	K3
      0x5C200600,  //  000D  MOVE	R8	R3
      0x58240004,  //  000E  LDCONST	R9	K4
      0x5C280A00,  //  000F  MOVE	R10	R5
      0x7C180800,  //  0010  CALL	R6	4
      0x141C0D02,  //  0011  LT	R7	R6	K2
      0x781E0000,  //  0012  JMPF	R7	#0014
      0x70020025,  //  0013  JMP		#003A
      0x541E0009,  //  0014  LDINT	R7	10
      0x001C0C07,  //  0015  ADD	R7	R6	R7
      0x5C200E00,  //  0016  MOVE	R8	R7
      0x6024000C,  //  0017  GETGBL	R9	G12
      0x5C280600,  //  0018  MOVE	R10	R3
      0x7C240200,  //  0019  CALL	R9	1
      0x14241009,  //  001A  LT	R9	R8	R9
      0x78260008,  //  001B  JMPF	R9	#0025
      0x8C240105,  //  001C  GETMET	R9	R0	K5
      0x942C0608,  //  001D  GETIDX	R11	R3	R8
      0x7C240400,  //  001E  CALL	R9	2
      0x74260002,  //  001F  JMPT	R9	#0023
      0x94240608,  //  0020  GETIDX	R9	R3	R8
      0x1C241306,  //  0021  EQ	R9	R9	K6
      0x78260001,  //  0022  JMPF	R9	#0025
      0x00201107,  //  0023  ADD	R8	R8	K7
      0x7001FFF1,  //  0024  JMP		#0017
      0x24241007,  //  0025  GT	R9	R8	R7
      0x7826000F,  //  0026  JMPF	R9	#0037
      0x54260007,  //  0027  LDINT	R9	8
      0x00241009,  //  0028  ADD	R9	R8	R9
      0x6028000C,  //  0029  GETGBL	R10	G12
      0x5C2C0600,  //  002A  MOVE	R11	R3
      0x7C280200,  //  002B  CALL	R10	1
      0x1424120A,  //  002C  LT	R9	R9	R10
      0x78260008,  //  002D  JMPF	R9	#0037
      0x54260006,  //  002E  LDINT	R9	7
      0x00241009,  //  002F  ADD	R9	R8	R9
      0x40241009,  //  0030  CONNECT	R9	R8	R9
      0x94240609,  //  0031  GETIDX	R9	R3	R9
      0x1C241308,  //  0032  EQ	R9	R9	K8
      0x78260002,  //  0033  JMPF	R9	#0037
      0x04241107,  //  0034  SUB	R9	R8	K7
      0x40240E09,  //  0035  CONNECT	R9	R7	R9
      0x94240609,  //  0036  GETIDX	R9	R3	R9
      0x00240D07,  //  0037  ADD	R9	R6	K7
      0x5C141200,  //  0038  MOVE	R5	R9
      0x7001FFCF,  //  0039  JMP		#000A
      0x58100002,  //  003A  LDCONST	R4	K2
      0x6018000C,  //  003B  GETGBL	R6	G12
      0x5C1C0600,  //  003C  MOVE	R7	R3
      0x7C180200,  //  003D  CALL	R6	1
      0x14180806,  //  003E  LT	R6	R4	R6
      0x781A006D,  //  003F  JMPF	R6	#00AE
      0x8C180503,  //  0040  GETMET	R6	R2	K3
      0x5C200600,  //  0041  MOVE	R8	R3
      0x58240006,  //  0042  LDCONST	R9	K6
      0x5C280800,  //  0043  MOVE	R10	R4
      0x7C180800,  //  0044  CALL	R6	4
      0x141C0D02,  //  0045  LT	R7	R6	K2
      0x781E0000,  //  0046  JMPF	R7	#0048
      0x70020065,  //  0047  JMP		#00AE
      0x5C1C0C00,  //  0048  MOVE	R7	R6
      0x24200F02,  //  0049  GT	R8	R7	K2
      0x78220006,  //  004A  JMPF	R8	#0052
      0x8C200105,  //  004B  GETMET	R8	R0	K5
      0x04280F07,  //  004C  SUB	R10	R7	K7
      0x9428060A,  //  004D  GETIDX	R10	R3	R10
      0x7C200400,  //  004E  CALL	R8	2
      0x78220001,  //  004F  JMPF	R8	#0052
      0x041C0F07,  //  0050  SUB	R7	R7	K7
      0x7001FFF6,  //  0051  JMP		#0049
      0x50200200,  //  0052  LDBOOL	R8	1	0
      0x54260011,  //  0053  LDINT	R9	18
      0x28240E09,  //  0054  GE	R9	R7	R9
      0x78260010,  //  0055  JMPF	R9	#0067
      0x54260011,  //  0056  LDINT	R9	18
      0x28240E09,  //  0057  GE	R9	R7	R9
      0x78260002,  //  0058  JMPF	R9	#005C
      0x54260011,  //  0059  LDINT	R9	18
      0x04240E09,  //  005A  SUB	R9	R7	R9
      0x70020000,  //  005B  JMP		#005D
      0x58240002,  //  005C  LDCONST	R9	K2
      0x04280F07,  //  005D  SUB	R10	R7	K7
      0x4028120A,  //  005E  CONNECT	R10	R9	R10
      0x9428060A,  //  005F  GETIDX	R10	R3	R10
      0x8C2C0503,  //  0060  GETMET	R11	R2	K3
      0x5C341400,  //  0061  MOVE	R13	R10
      0x58380009,  //  0062  LDCONST	R14	K9
      0x7C2C0600,  //  0063  CALL	R11	3
      0x282C1702,  //  0064  GE	R11	R11	K2
      0x782E0000,  //  0065  JMPF	R11	#0067
      0x50200000,  //  0066  LDBOOL	R8	0	0
      0x78220013,  //  0067  JMPF	R8	#007C
      0x54260009,  //  0068  LDINT	R9	10
      0x28240E09,  //  0069  GE	R9	R7	R9
      0x78260010,  //  006A  JMPF	R9	#007C
      0x54260009,  //  006B  LDINT	R9	10
      0x28240E09,  //  006C  GE	R9	R7	R9
      0x78260002,  //  006D  JMPF	R9	#0071
      0x54260009,  //  006E  LDINT	R9	10
      0x04240E09,  //  006F  SUB	R9	R7	R9
      0x70020000,  //  0070  JMP		#0072
      0x58240002,  //  0071  LDCONST	R9	K2
      0x04280F07,  //  0072  SUB	R10	R7	K7
      0x4028120A,  //  0073  CONNECT	R10	R9	R10
      0x9428060A,  //  0074  GETIDX	R10	R3	R10
      0x8C2C0503,  //  0075  GETMET	R11	R2	K3
      0x5C341400,  //  0076  MOVE	R13	R10
      0x58380004,  //  0077  LDCONST	R14	K4
      0x7C2C0600,  //  0078  CALL	R11	3
      0x282C1702,  //  0079  GE	R11	R11	K2
      0x782E0000,  //  007A  JMPF	R11	#007C
      0x50200000,  //  007B  LDBOOL	R8	0	0
      0x7822002D,  //  007C  JMPF	R8	#00AB
      0x14240E06,  //  007D  LT	R9	R7	R6
      0x7826002B,  //  007E  JMPF	R9	#00AB
      0x40240E06,  //  007F  CONNECT	R9	R7	R6
      0x94240609,  //  0080  GETIDX	R9	R3	R9
      0x00280D07,  //  0081  ADD	R10	R6	K7
      0x602C000C,  //  0082  GETGBL	R11	G12
      0x5C300600,  //  0083  MOVE	R12	R3
      0x7C2C0200,  //  0084  CALL	R11	1
      0x282C140B,  //  0085  GE	R11	R10	R11
      0x742E0003,  //  0086  JMPT	R11	#008B
      0x8C2C0105,  //  0087  GETMET	R11	R0	K5
      0x9434060A,  //  0088  GETIDX	R13	R3	R10
      0x7C2C0400,  //  0089  CALL	R11	2
      0x742E001C,  //  008A  JMPT	R11	#00A8
      0x602C0018,  //  008B  GETGBL	R11	G24
      0x5830000A,  //  008C  LDCONST	R12	K10
      0x5C341200,  //  008D  MOVE	R13	R9
      0x7C2C0400,  //  008E  CALL	R11	2
      0x24300F02,  //  008F  GT	R12	R7	K2
      0x78320003,  //  0090  JMPF	R12	#0095
      0x04300F07,  //  0091  SUB	R12	R7	K7
      0x4032040C,  //  0092  CONNECT	R12	K2	R12
      0x9430060C,  //  0093  GETIDX	R12	R3	R12
      0x70020000,  //  0094  JMP		#0096
      0x5830000B,  //  0095  LDCONST	R12	K11
      0x6034000C,  //  0096  GETGBL	R13	G12
      0x5C380600,  //  0097  MOVE	R14	R3
      0x7C340200,  //  0098  CALL	R13	1
      0x1434140D,  //  0099  LT	R13	R10	R13
      0x78360002,  //  009A  JMPF	R13	#009E
      0x4034150C,  //  009B  CONNECT	R13	R10	K12
      0x9434060D,  //  009C  GETIDX	R13	R3	R13
      0x70020000,  //  009D  JMP		#009F
      0x5834000B,  //  009E  LDCONST	R13	K11
      0x0038180B,  //  009F  ADD	R14	R12	R11
      0x00381C0D,  //  00A0  ADD	R14	R14	R13
      0x5C0C1C00,  //  00A1  MOVE	R3	R14
      0x6038000C,  //  00A2  GETGBL	R14	G12
      0x5C3C1600,  //  00A3  MOVE	R15	R11
      0x7C380200,  //  00A4  CALL	R14	1
      0x00380E0E,  //  00A5  ADD	R14	R7	R14
      0x5C101C00,  //  00A6  MOVE	R4	R14
      0x70020001,  //  00A7  JMP		#00AA
      0x002C0D07,  //  00A8  ADD	R11	R6	K7
      0x5C101600,  //  00A9  MOVE	R4	R11
      0x70020001,  //  00AA  JMP		#00AD
      0x00240D07,  //  00AB  ADD	R9	R6	K7
      0x5C101200,  //  00AC  MOVE	R4	R9
      0x7001FF8C,  //  00AD  JMP		#003B
      0x60180001,  //  00AE  GETGBL	R6	G1
      0x601C0018,  //  00AF  GETGBL	R7	G24
      0x5820000D,  //  00B0  LDCONST	R8	K13
      0x5C240600,  //  00B1  MOVE	R9	R3
      0x7C1C0400,  //  00B2  CALL	R7	2
      0x7C180200,  //  00B3  CALL	R6	1
      0x80040600,  //  00B4  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: check_right_paren
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_check_right_paren,   /* name */
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
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(RIGHT_PAREN),
    }),
    be_str_weak(check_right_paren),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0005,  //  0004  JMPF	R2	#000B
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x740A0000,  //  000A  JMPT	R2	#000C
      0x50080001,  //  000B  LDBOOL	R2	0	1
      0x50080200,  //  000C  LDBOOL	R2	1	0
      0x80040400,  //  000D  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: expect_keyword
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_expect_keyword,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(KEYWORD),
    /* K5   */  be_nested_str_weak(value),
    /* K6   */  be_nested_str_weak(next),
    /* K7   */  be_nested_str_weak(error),
    /* K8   */  be_nested_str_weak(Expected_X20_X27_X25s_X27),
    }),
    be_str_weak(expect_keyword),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x7C080200,  //  0001  CALL	R2	1
      0x4C0C0000,  //  0002  LDNIL	R3
      0x200C0403,  //  0003  NE	R3	R2	R3
      0x780E000B,  //  0004  JMPF	R3	#0011
      0x880C0501,  //  0005  GETMBR	R3	R2	K1
      0xB8120400,  //  0006  GETNGBL	R4	K2
      0x88100903,  //  0007  GETMBR	R4	R4	K3
      0x88100904,  //  0008  GETMBR	R4	R4	K4
      0x1C0C0604,  //  0009  EQ	R3	R3	R4
      0x780E0005,  //  000A  JMPF	R3	#0011
      0x880C0505,  //  000B  GETMBR	R3	R2	K5
      0x1C0C0601,  //  000C  EQ	R3	R3	R1
      0x780E0002,  //  000D  JMPF	R3	#0011
      0x8C0C0106,  //  000E  GETMET	R3	R0	K6
      0x7C0C0200,  //  000F  CALL	R3	1
      0x70020005,  //  0010  JMP		#0017
      0x8C0C0107,  //  0011  GETMET	R3	R0	K7
      0x60140018,  //  0012  GETGBL	R5	G24
      0x58180008,  //  0013  LDCONST	R6	K8
      0x5C1C0200,  //  0014  MOVE	R7	R1
      0x7C140400,  //  0015  CALL	R5	2
      0x7C0C0400,  //  0016  CALL	R3	2
      0x80000000,  //  0017  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_sequence_assignment_generic
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_sequence_assignment_generic,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[27]) {     /* constants */
    /* K0   */  be_nested_str_weak(expect_identifier),
    /* K1   */  be_nested_str_weak(current),
    /* K2   */  be_nested_str_weak(type),
    /* K3   */  be_nested_str_weak(animation_dsl),
    /* K4   */  be_nested_str_weak(Token),
    /* K5   */  be_nested_str_weak(DOT),
    /* K6   */  be_nested_str_weak(next),
    /* K7   */  be_nested_str_weak(symbol_table),
    /* K8   */  be_nested_str_weak(contains),
    /* K9   */  be_nested_str_weak(get),
    /* K10  */  be_nested_str_weak(instance),
    /* K11  */  be_nested_str_weak(_validate_single_parameter),
    /* K12  */  be_nested_str_weak(_symbol_entry),
    /* K13  */  be_nested_str_weak(TYPE_SEQUENCE),
    /* K14  */  be_nested_str_weak(error),
    /* K15  */  be_nested_str_weak(Sequences_X20like_X20_X27_X25s_X27_X20do_X20not_X20have_X20properties_X2E_X20Property_X20assignments_X20are_X20only_X20valid_X20for_X20animations_X20and_X20color_X20providers_X2E),
    /* K16  */  be_nested_str_weak(expect_assign),
    /* K17  */  be_nested_str_weak(process_value),
    /* K18  */  be_nested_str_weak(CONTEXT_PROPERTY),
    /* K19  */  be_nested_str_weak(collect_inline_comment),
    /* K20  */  be_nested_str_weak(get_reference),
    /* K21  */  be_nested_str_weak(def_X20_X28engine_X29_X20_X25s_X2E_X25s_X20_X3D_X20_X25s_X20end),
    /* K22  */  be_nested_str_weak(expr),
    /* K23  */  be_nested_str_weak(add),
    /* K24  */  be_nested_str_weak(_X25s_X25s_X2Epush_X28animation_X2Ecreate_assign_step_X28_X25s_X29_X29_X25s),
    /* K25  */  be_nested_str_weak(Expected_X20property_X20assignment_X20for_X20_X27_X25s_X27_X20but_X20found_X20no_X20dot),
    /* K26  */  be_nested_str_weak(skip_statement),
    }),
    be_str_weak(process_sequence_assignment_generic),
    &be_const_str_solidified,
    ( &(const binstruction[96]) {  /* code */
      0x8C0C0100,  //  0000  GETMET	R3	R0	K0
      0x7C0C0200,  //  0001  CALL	R3	1
      0x8C100101,  //  0002  GETMET	R4	R0	K1
      0x7C100200,  //  0003  CALL	R4	1
      0x4C140000,  //  0004  LDNIL	R5
      0x20100805,  //  0005  NE	R4	R4	R5
      0x7812004F,  //  0006  JMPF	R4	#0057
      0x8C100101,  //  0007  GETMET	R4	R0	K1
      0x7C100200,  //  0008  CALL	R4	1
      0x88100902,  //  0009  GETMBR	R4	R4	K2
      0xB8160600,  //  000A  GETNGBL	R5	K3
      0x88140B04,  //  000B  GETMBR	R5	R5	K4
      0x88140B05,  //  000C  GETMBR	R5	R5	K5
      0x1C100805,  //  000D  EQ	R4	R4	R5
      0x78120047,  //  000E  JMPF	R4	#0057
      0x8C100106,  //  000F  GETMET	R4	R0	K6
      0x7C100200,  //  0010  CALL	R4	1
      0x8C100100,  //  0011  GETMET	R4	R0	K0
      0x7C100200,  //  0012  CALL	R4	1
      0x88140107,  //  0013  GETMBR	R5	R0	K7
      0x8C140B08,  //  0014  GETMET	R5	R5	K8
      0x5C1C0600,  //  0015  MOVE	R7	R3
      0x7C140400,  //  0016  CALL	R5	2
      0x78160023,  //  0017  JMPF	R5	#003C
      0x88140107,  //  0018  GETMBR	R5	R0	K7
      0x8C140B09,  //  0019  GETMET	R5	R5	K9
      0x5C1C0600,  //  001A  MOVE	R7	R3
      0x7C140400,  //  001B  CALL	R5	2
      0x4C180000,  //  001C  LDNIL	R6
      0x20180A06,  //  001D  NE	R6	R5	R6
      0x781A000C,  //  001E  JMPF	R6	#002C
      0x88180B0A,  //  001F  GETMBR	R6	R5	K10
      0x4C1C0000,  //  0020  LDNIL	R7
      0x20180C07,  //  0021  NE	R6	R6	R7
      0x781A0008,  //  0022  JMPF	R6	#002C
      0x60180005,  //  0023  GETGBL	R6	G5
      0x881C0B0A,  //  0024  GETMBR	R7	R5	K10
      0x7C180200,  //  0025  CALL	R6	1
      0x8C1C010B,  //  0026  GETMET	R7	R0	K11
      0x5C240C00,  //  0027  MOVE	R9	R6
      0x5C280800,  //  0028  MOVE	R10	R4
      0x882C0B0A,  //  0029  GETMBR	R11	R5	K10
      0x7C1C0800,  //  002A  CALL	R7	4
      0x7002000F,  //  002B  JMP		#003C
      0x4C180000,  //  002C  LDNIL	R6
      0x20180A06,  //  002D  NE	R6	R5	R6
      0x781A000C,  //  002E  JMPF	R6	#003C
      0x88180B02,  //  002F  GETMBR	R6	R5	K2
      0xB81E0600,  //  0030  GETNGBL	R7	K3
      0x881C0F0C,  //  0031  GETMBR	R7	R7	K12
      0x881C0F0D,  //  0032  GETMBR	R7	R7	K13
      0x1C180C07,  //  0033  EQ	R6	R6	R7
      0x781A0006,  //  0034  JMPF	R6	#003C
      0x8C18010E,  //  0035  GETMET	R6	R0	K14
      0x60200018,  //  0036  GETGBL	R8	G24
      0x5824000F,  //  0037  LDCONST	R9	K15
      0x5C280600,  //  0038  MOVE	R10	R3
      0x7C200400,  //  0039  CALL	R8	2
      0x7C180400,  //  003A  CALL	R6	2
      0x80000C00,  //  003B  RET	0
      0x8C140110,  //  003C  GETMET	R5	R0	K16
      0x7C140200,  //  003D  CALL	R5	1
      0x8C140111,  //  003E  GETMET	R5	R0	K17
      0x881C0112,  //  003F  GETMBR	R7	R0	K18
      0x7C140400,  //  0040  CALL	R5	2
      0x8C180113,  //  0041  GETMET	R6	R0	K19
      0x7C180200,  //  0042  CALL	R6	1
      0x881C0107,  //  0043  GETMBR	R7	R0	K7
      0x8C1C0F14,  //  0044  GETMET	R7	R7	K20
      0x5C240600,  //  0045  MOVE	R9	R3
      0x7C1C0400,  //  0046  CALL	R7	2
      0x60200018,  //  0047  GETGBL	R8	G24
      0x58240015,  //  0048  LDCONST	R9	K21
      0x5C280E00,  //  0049  MOVE	R10	R7
      0x5C2C0800,  //  004A  MOVE	R11	R4
      0x88300B16,  //  004B  GETMBR	R12	R5	K22
      0x7C200800,  //  004C  CALL	R8	4
      0x8C240117,  //  004D  GETMET	R9	R0	K23
      0x602C0018,  //  004E  GETGBL	R11	G24
      0x58300018,  //  004F  LDCONST	R12	K24
      0x5C340200,  //  0050  MOVE	R13	R1
      0x5C380400,  //  0051  MOVE	R14	R2
      0x5C3C1000,  //  0052  MOVE	R15	R8
      0x5C400C00,  //  0053  MOVE	R16	R6
      0x7C2C0A00,  //  0054  CALL	R11	5
      0x7C240400,  //  0055  CALL	R9	2
      0x70020007,  //  0056  JMP		#005F
      0x8C10010E,  //  0057  GETMET	R4	R0	K14
      0x60180018,  //  0058  GETGBL	R6	G24
      0x581C0019,  //  0059  LDCONST	R7	K25
      0x5C200600,  //  005A  MOVE	R8	R3
      0x7C180400,  //  005B  CALL	R6	2
      0x7C100400,  //  005C  CALL	R4	2
      0x8C10011A,  //  005D  GETMET	R4	R0	K26
      0x7C100200,  //  005E  CALL	R4	1
      0x80000000,  //  005F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_named_arguments_for_variable
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_named_arguments_for_variable,   /* name */
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
    /* K0   */  be_nested_str_weak(_process_named_arguments_unified),
    /* K1   */  be_nested_str_weak(),
    /* K2   */  be_nested_str_weak(CONTEXT_VARIABLE),
    }),
    be_str_weak(process_named_arguments_for_variable),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0x58140001,  //  0002  LDCONST	R5	K1
      0x88180102,  //  0003  GETMBR	R6	R0	K2
      0x7C080800,  //  0004  CALL	R2	4
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: skip_whitespace_including_newlines
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_skip_whitespace_including_newlines,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(at_end),
    /* K1   */  be_nested_str_weak(current),
    /* K2   */  be_nested_str_weak(type),
    /* K3   */  be_nested_str_weak(animation_dsl),
    /* K4   */  be_nested_str_weak(Token),
    /* K5   */  be_nested_str_weak(COMMENT),
    /* K6   */  be_nested_str_weak(NEWLINE),
    /* K7   */  be_nested_str_weak(next),
    }),
    be_str_weak(skip_whitespace_including_newlines),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x74060015,  //  0002  JMPT	R1	#0019
      0x8C040101,  //  0003  GETMET	R1	R0	K1
      0x7C040200,  //  0004  CALL	R1	1
      0x4C080000,  //  0005  LDNIL	R2
      0x20080202,  //  0006  NE	R2	R1	R2
      0x780A000E,  //  0007  JMPF	R2	#0017
      0x88080302,  //  0008  GETMBR	R2	R1	K2
      0xB80E0600,  //  0009  GETNGBL	R3	K3
      0x880C0704,  //  000A  GETMBR	R3	R3	K4
      0x880C0705,  //  000B  GETMBR	R3	R3	K5
      0x1C080403,  //  000C  EQ	R2	R2	R3
      0x740A0005,  //  000D  JMPT	R2	#0014
      0x88080302,  //  000E  GETMBR	R2	R1	K2
      0xB80E0600,  //  000F  GETNGBL	R3	K3
      0x880C0704,  //  0010  GETMBR	R3	R3	K4
      0x880C0706,  //  0011  GETMBR	R3	R3	K6
      0x1C080403,  //  0012  EQ	R2	R2	R3
      0x780A0002,  //  0013  JMPF	R2	#0017
      0x8C080107,  //  0014  GETMET	R2	R0	K7
      0x7C080200,  //  0015  CALL	R2	1
      0x70020000,  //  0016  JMP		#0018
      0x70020000,  //  0017  JMP		#0019
      0x7001FFE6,  //  0018  JMP		#0000
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_nested_function_call
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_nested_function_call,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        10,                          /* nstack */
        3,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 4),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 2]) {     /* constants */
        /* K0   */  be_nested_str_weak(push),
        /* K1   */  be_nested_str_weak(_X20_X20provider_X2E_X25s_X20_X3D_X20_X25s_X25s),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[10]) {  /* code */
          0x680C0000,  //  0000  GETUPV	R3	U0
          0x8C0C0700,  //  0001  GETMET	R3	R3	K0
          0x60140018,  //  0002  GETGBL	R5	G24
          0x58180001,  //  0003  LDCONST	R6	K1
          0x5C1C0000,  //  0004  MOVE	R7	R0
          0x5C200200,  //  0005  MOVE	R8	R1
          0x5C240400,  //  0006  MOVE	R9	R2
          0x7C140800,  //  0007  CALL	R5	4
          0x7C0C0400,  //  0008  CALL	R3	2
          0x80000000,  //  0009  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[39]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(),
    /* K2   */  be_nested_str_weak(type),
    /* K3   */  be_nested_str_weak(animation_dsl),
    /* K4   */  be_nested_str_weak(Token),
    /* K5   */  be_nested_str_weak(IDENTIFIER),
    /* K6   */  be_nested_str_weak(KEYWORD),
    /* K7   */  be_nested_str_weak(value),
    /* K8   */  be_nested_str_weak(next),
    /* K9   */  be_nested_str_weak(error),
    /* K10  */  be_nested_str_weak(Expected_X20function_X20name),
    /* K11  */  be_nested_str_weak(nil),
    /* K12  */  be_nested_str_weak(symbol_table),
    /* K13  */  be_nested_str_weak(get),
    /* K14  */  be_nested_str_weak(_symbol_entry),
    /* K15  */  be_nested_str_weak(TYPE_MATH_FUNCTION),
    /* K16  */  be_nested_str_weak(process_function_arguments),
    /* K17  */  be_nested_str_weak(_X25s_X28_X25s_X29),
    /* K18  */  be_nested_str_weak(get_reference),
    /* K19  */  be_nested_str_weak(log),
    /* K20  */  be_nested_str_weak(process_log_call),
    /* K21  */  be_nested_str_weak(CONTEXT_EXPRESSION),
    /* K22  */  be_nested_str_weak(TYPE_TEMPLATE),
    /* K23  */  be_nested_str_weak(engine_X2C_X20_X25s),
    /* K24  */  be_nested_str_weak(engine),
    /* K25  */  be_nested_str_weak(_X25s_template_X28_X25s_X29),
    /* K26  */  be_nested_str_weak(_validate_animation_factory_exists),
    /* K27  */  be_nested_str_weak(Animation_X20factory_X20function_X20_X27_X25s_X27_X20does_X20not_X20exist_X2E_X20Check_X20the_X20function_X20name_X20and_X20ensure_X20it_X27s_X20available_X20in_X20the_X20animation_X20module_X2E),
    /* K28  */  be_nested_str_weak(skip_function_arguments),
    /* K29  */  be_nested_str_weak(expect_left_paren),
    /* K30  */  be_nested_str_weak(_process_parameters_core),
    /* K31  */  be_nested_str_weak(generic),
    /* K32  */  be_nested_str_weak(expect_right_paren),
    /* K33  */  be_const_int(0),
    /* K34  */  be_const_int(1),
    /* K35  */  be_nested_str_weak(_X0A),
    /* K36  */  be_nested_str_weak(stop_iteration),
    /* K37  */  be_nested_str_weak(_X28def_X20_X28engine_X29_X0A_X20_X20var_X20provider_X20_X3D_X20animation_X2E_X25s_X28engine_X29_X0A_X25s_X0A_X20_X20return_X20provider_X0Aend_X29_X28engine_X29),
    /* K38  */  be_nested_str_weak(animation_X2E_X25s_X28engine_X29),
    }),
    be_str_weak(process_nested_function_call),
    &be_const_str_solidified,
    ( &(const binstruction[153]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x58080001,  //  0002  LDCONST	R2	K1
      0x4C0C0000,  //  0003  LDNIL	R3
      0x200C0203,  //  0004  NE	R3	R1	R3
      0x780E000F,  //  0005  JMPF	R3	#0016
      0x880C0302,  //  0006  GETMBR	R3	R1	K2
      0xB8120600,  //  0007  GETNGBL	R4	K3
      0x88100904,  //  0008  GETMBR	R4	R4	K4
      0x88100905,  //  0009  GETMBR	R4	R4	K5
      0x1C0C0604,  //  000A  EQ	R3	R3	R4
      0x740E0005,  //  000B  JMPT	R3	#0012
      0x880C0302,  //  000C  GETMBR	R3	R1	K2
      0xB8120600,  //  000D  GETNGBL	R4	K3
      0x88100904,  //  000E  GETMBR	R4	R4	K4
      0x88100906,  //  000F  GETMBR	R4	R4	K6
      0x1C0C0604,  //  0010  EQ	R3	R3	R4
      0x780E0003,  //  0011  JMPF	R3	#0016
      0x88080307,  //  0012  GETMBR	R2	R1	K7
      0x8C0C0108,  //  0013  GETMET	R3	R0	K8
      0x7C0C0200,  //  0014  CALL	R3	1
      0x70020003,  //  0015  JMP		#001A
      0x8C0C0109,  //  0016  GETMET	R3	R0	K9
      0x5814000A,  //  0017  LDCONST	R5	K10
      0x7C0C0400,  //  0018  CALL	R3	2
      0x80061600,  //  0019  RET	1	K11
      0x880C010C,  //  001A  GETMBR	R3	R0	K12
      0x8C0C070D,  //  001B  GETMET	R3	R3	K13
      0x5C140400,  //  001C  MOVE	R5	R2
      0x7C0C0400,  //  001D  CALL	R3	2
      0x4C100000,  //  001E  LDNIL	R4
      0x20100604,  //  001F  NE	R4	R3	R4
      0x7812000F,  //  0020  JMPF	R4	#0031
      0x88100702,  //  0021  GETMBR	R4	R3	K2
      0xB8160600,  //  0022  GETNGBL	R5	K3
      0x88140B0E,  //  0023  GETMBR	R5	R5	K14
      0x88140B0F,  //  0024  GETMBR	R5	R5	K15
      0x1C100805,  //  0025  EQ	R4	R4	R5
      0x78120009,  //  0026  JMPF	R4	#0031
      0x8C100110,  //  0027  GETMET	R4	R0	K16
      0x50180200,  //  0028  LDBOOL	R6	1	0
      0x7C100400,  //  0029  CALL	R4	2
      0x60140018,  //  002A  GETGBL	R5	G24
      0x58180011,  //  002B  LDCONST	R6	K17
      0x8C1C0712,  //  002C  GETMET	R7	R3	K18
      0x7C1C0200,  //  002D  CALL	R7	1
      0x5C200800,  //  002E  MOVE	R8	R4
      0x7C140600,  //  002F  CALL	R5	3
      0x80040A00,  //  0030  RET	1	R5
      0x1C100513,  //  0031  EQ	R4	R2	K19
      0x78120008,  //  0032  JMPF	R4	#003C
      0x8C100110,  //  0033  GETMET	R4	R0	K16
      0x50180200,  //  0034  LDBOOL	R6	1	0
      0x7C100400,  //  0035  CALL	R4	2
      0x8C140114,  //  0036  GETMET	R5	R0	K20
      0x5C1C0800,  //  0037  MOVE	R7	R4
      0x88200115,  //  0038  GETMBR	R8	R0	K21
      0x58240001,  //  0039  LDCONST	R9	K1
      0x7C140800,  //  003A  CALL	R5	4
      0x80040A00,  //  003B  RET	1	R5
      0x4C100000,  //  003C  LDNIL	R4
      0x20100604,  //  003D  NE	R4	R3	R4
      0x78120017,  //  003E  JMPF	R4	#0057
      0x88100702,  //  003F  GETMBR	R4	R3	K2
      0xB8160600,  //  0040  GETNGBL	R5	K3
      0x88140B0E,  //  0041  GETMBR	R5	R5	K14
      0x88140B16,  //  0042  GETMBR	R5	R5	K22
      0x1C100805,  //  0043  EQ	R4	R4	R5
      0x78120011,  //  0044  JMPF	R4	#0057
      0x8C100110,  //  0045  GETMET	R4	R0	K16
      0x50180200,  //  0046  LDBOOL	R6	1	0
      0x7C100400,  //  0047  CALL	R4	2
      0x20140901,  //  0048  NE	R5	R4	K1
      0x78160004,  //  0049  JMPF	R5	#004F
      0x60140018,  //  004A  GETGBL	R5	G24
      0x58180017,  //  004B  LDCONST	R6	K23
      0x5C1C0800,  //  004C  MOVE	R7	R4
      0x7C140400,  //  004D  CALL	R5	2
      0x70020000,  //  004E  JMP		#0050
      0x58140018,  //  004F  LDCONST	R5	K24
      0x60180018,  //  0050  GETGBL	R6	G24
      0x581C0019,  //  0051  LDCONST	R7	K25
      0x5C200400,  //  0052  MOVE	R8	R2
      0x5C240A00,  //  0053  MOVE	R9	R5
      0x7C180600,  //  0054  CALL	R6	3
      0x80040C00,  //  0055  RET	1	R6
      0x70020040,  //  0056  JMP		#0098
      0x8C10011A,  //  0057  GETMET	R4	R0	K26
      0x5C180400,  //  0058  MOVE	R6	R2
      0x7C100400,  //  0059  CALL	R4	2
      0x74120008,  //  005A  JMPT	R4	#0064
      0x8C100109,  //  005B  GETMET	R4	R0	K9
      0x60180018,  //  005C  GETGBL	R6	G24
      0x581C001B,  //  005D  LDCONST	R7	K27
      0x5C200400,  //  005E  MOVE	R8	R2
      0x7C180400,  //  005F  CALL	R6	2
      0x7C100400,  //  0060  CALL	R4	2
      0x8C10011C,  //  0061  GETMET	R4	R0	K28
      0x7C100200,  //  0062  CALL	R4	1
      0x80061600,  //  0063  RET	1	K11
      0x8C10011D,  //  0064  GETMET	R4	R0	K29
      0x7C100200,  //  0065  CALL	R4	1
      0x60100012,  //  0066  GETGBL	R4	G18
      0x7C100000,  //  0067  CALL	R4	0
      0x84140000,  //  0068  CLOSURE	R5	P0
      0x8C18011E,  //  0069  GETMET	R6	R0	K30
      0x5C200400,  //  006A  MOVE	R8	R2
      0x5824001F,  //  006B  LDCONST	R9	K31
      0x5C280A00,  //  006C  MOVE	R10	R5
      0x7C180800,  //  006D  CALL	R6	4
      0x8C180120,  //  006E  GETMET	R6	R0	K32
      0x7C180200,  //  006F  CALL	R6	1
      0x6018000C,  //  0070  GETGBL	R6	G12
      0x5C1C0800,  //  0071  MOVE	R7	R4
      0x7C180200,  //  0072  CALL	R6	1
      0x24180D21,  //  0073  GT	R6	R6	K33
      0x781A001B,  //  0074  JMPF	R6	#0091
      0x58180001,  //  0075  LDCONST	R6	K1
      0x601C0010,  //  0076  GETGBL	R7	G16
      0x6020000C,  //  0077  GETGBL	R8	G12
      0x5C240800,  //  0078  MOVE	R9	R4
      0x7C200200,  //  0079  CALL	R8	1
      0x04201122,  //  007A  SUB	R8	R8	K34
      0x40224208,  //  007B  CONNECT	R8	K33	R8
      0x7C1C0200,  //  007C  CALL	R7	1
      0xA8020007,  //  007D  EXBLK	0	#0086
      0x5C200E00,  //  007E  MOVE	R8	R7
      0x7C200000,  //  007F  CALL	R8	0
      0x24241121,  //  0080  GT	R9	R8	K33
      0x78260000,  //  0081  JMPF	R9	#0083
      0x00180D23,  //  0082  ADD	R6	R6	K35
      0x94240808,  //  0083  GETIDX	R9	R4	R8
      0x00180C09,  //  0084  ADD	R6	R6	R9
      0x7001FFF7,  //  0085  JMP		#007E
      0x581C0024,  //  0086  LDCONST	R7	K36
      0xAC1C0200,  //  0087  CATCH	R7	1	0
      0xB0080000,  //  0088  RAISE	2	R0	R0
      0x601C0018,  //  0089  GETGBL	R7	G24
      0x58200025,  //  008A  LDCONST	R8	K37
      0x5C240400,  //  008B  MOVE	R9	R2
      0x5C280C00,  //  008C  MOVE	R10	R6
      0x7C1C0600,  //  008D  CALL	R7	3
      0xA0000000,  //  008E  CLOSE	R0
      0x80040E00,  //  008F  RET	1	R7
      0x70020005,  //  0090  JMP		#0097
      0x60180018,  //  0091  GETGBL	R6	G24
      0x581C0026,  //  0092  LDCONST	R7	K38
      0x5C200400,  //  0093  MOVE	R8	R2
      0x7C180400,  //  0094  CALL	R6	2
      0xA0000000,  //  0095  CLOSE	R0
      0x80040C00,  //  0096  RET	1	R6
      0xA0100000,  //  0097  CLOSE	R4
      0x80000000,  //  0098  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: join_output
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_join_output,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(),
    /* K1   */  be_nested_str_weak(output),
    /* K2   */  be_nested_str_weak(_X0A),
    /* K3   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(join_output),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x60080010,  //  0001  GETGBL	R2	G16
      0x880C0101,  //  0002  GETMBR	R3	R0	K1
      0x7C080200,  //  0003  CALL	R2	1
      0xA8020004,  //  0004  EXBLK	0	#000A
      0x5C0C0400,  //  0005  MOVE	R3	R2
      0x7C0C0000,  //  0006  CALL	R3	0
      0x00100702,  //  0007  ADD	R4	R3	K2
      0x00040204,  //  0008  ADD	R1	R1	R4
      0x7001FFFA,  //  0009  JMP		#0005
      0x58080003,  //  000A  LDCONST	R2	K3
      0xAC080200,  //  000B  CATCH	R2	1	0
      0xB0080000,  //  000C  RAISE	2	R0	R0
      0x80040200,  //  000D  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: expect_left_brace
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_expect_left_brace,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(LEFT_BRACE),
    /* K5   */  be_nested_str_weak(next),
    /* K6   */  be_nested_str_weak(error),
    /* K7   */  be_nested_str_weak(Expected_X20_X27_X7B_X27),
    }),
    be_str_weak(expect_left_brace),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0008,  //  0004  JMPF	R2	#000E
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x780A0002,  //  000A  JMPF	R2	#000E
      0x8C080105,  //  000B  GETMET	R2	R0	K5
      0x7C080200,  //  000C  CALL	R2	1
      0x70020002,  //  000D  JMP		#0011
      0x8C080106,  //  000E  GETMET	R2	R0	K6
      0x58100007,  //  000F  LDCONST	R4	K7
      0x7C080400,  //  0010  CALL	R2	2
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _validate_object_reference
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__validate_object_reference,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(validate_symbol_reference),
    }),
    be_str_weak(_validate_object_reference),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0xA8020005,  //  0000  EXBLK	0	#0007
      0x8C0C0100,  //  0001  GETMET	R3	R0	K0
      0x5C140200,  //  0002  MOVE	R5	R1
      0x5C180400,  //  0003  MOVE	R6	R2
      0x7C0C0600,  //  0004  CALL	R3	3
      0xA8040001,  //  0005  EXBLK	1	1
      0x70020003,  //  0006  JMP		#000B
      0xAC0C0002,  //  0007  CATCH	R3	0	2
      0x70020000,  //  0008  JMP		#000A
      0x70020000,  //  0009  JMP		#000B
      0xB0080000,  //  000A  RAISE	2	R0	R0
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_standalone_log
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_standalone_log,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(next),
    /* K1   */  be_nested_str_weak(expect_left_paren),
    /* K2   */  be_nested_str_weak(current),
    /* K3   */  be_nested_str_weak(type),
    /* K4   */  be_nested_str_weak(animation_dsl),
    /* K5   */  be_nested_str_weak(Token),
    /* K6   */  be_nested_str_weak(STRING),
    /* K7   */  be_nested_str_weak(error),
    /* K8   */  be_nested_str_weak(log_X28_X29_X20function_X20requires_X20a_X20string_X20message),
    /* K9   */  be_nested_str_weak(skip_statement),
    /* K10  */  be_nested_str_weak(value),
    /* K11  */  be_nested_str_weak(expect_right_paren),
    /* K12  */  be_nested_str_weak(collect_inline_comment),
    /* K13  */  be_nested_str_weak(process_log_call),
    /* K14  */  be_nested_str_weak(standalone),
    /* K15  */  be_nested_str_weak(add),
    }),
    be_str_weak(process_standalone_log),
    &be_const_str_solidified,
    ( &(const binstruction[37]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x7C040200,  //  0003  CALL	R1	1
      0x8C040102,  //  0004  GETMET	R1	R0	K2
      0x7C040200,  //  0005  CALL	R1	1
      0x4C080000,  //  0006  LDNIL	R2
      0x1C080202,  //  0007  EQ	R2	R1	R2
      0x740A0005,  //  0008  JMPT	R2	#000F
      0x88080303,  //  0009  GETMBR	R2	R1	K3
      0xB80E0800,  //  000A  GETNGBL	R3	K4
      0x880C0705,  //  000B  GETMBR	R3	R3	K5
      0x880C0706,  //  000C  GETMBR	R3	R3	K6
      0x20080403,  //  000D  NE	R2	R2	R3
      0x780A0005,  //  000E  JMPF	R2	#0015
      0x8C080107,  //  000F  GETMET	R2	R0	K7
      0x58100008,  //  0010  LDCONST	R4	K8
      0x7C080400,  //  0011  CALL	R2	2
      0x8C080109,  //  0012  GETMET	R2	R0	K9
      0x7C080200,  //  0013  CALL	R2	1
      0x80000400,  //  0014  RET	0
      0x8808030A,  //  0015  GETMBR	R2	R1	K10
      0x8C0C0100,  //  0016  GETMET	R3	R0	K0
      0x7C0C0200,  //  0017  CALL	R3	1
      0x8C0C010B,  //  0018  GETMET	R3	R0	K11
      0x7C0C0200,  //  0019  CALL	R3	1
      0x8C0C010C,  //  001A  GETMET	R3	R0	K12
      0x7C0C0200,  //  001B  CALL	R3	1
      0x8C10010D,  //  001C  GETMET	R4	R0	K13
      0x5C180400,  //  001D  MOVE	R6	R2
      0x581C000E,  //  001E  LDCONST	R7	K14
      0x5C200600,  //  001F  MOVE	R8	R3
      0x7C100800,  //  0020  CALL	R4	4
      0x8C14010F,  //  0021  GETMET	R5	R0	K15
      0x5C1C0800,  //  0022  MOVE	R7	R4
      0x7C140400,  //  0023  CALL	R5	2
      0x80000000,  //  0024  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_event_handler
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_event_handler,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[27]) {     /* constants */
    /* K0   */  be_nested_str_weak(next),
    /* K1   */  be_nested_str_weak(expect_identifier),
    /* K2   */  be_nested_str_weak(current),
    /* K3   */  be_nested_str_weak(line),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(_X7B_X7D),
    /* K6   */  be_nested_str_weak(type),
    /* K7   */  be_nested_str_weak(animation_dsl),
    /* K8   */  be_nested_str_weak(Token),
    /* K9   */  be_nested_str_weak(LEFT_PAREN),
    /* K10  */  be_nested_str_weak(process_event_parameters),
    /* K11  */  be_nested_str_weak(expect_colon),
    /* K12  */  be_nested_str_weak(event_handler__X25s__X25s),
    /* K13  */  be_nested_str_weak(add),
    /* K14  */  be_nested_str_weak(def_X20_X25s_X28event_data_X29),
    /* K15  */  be_nested_str_weak(KEYWORD),
    /* K16  */  be_nested_str_weak(value),
    /* K17  */  be_nested_str_weak(interrupt),
    /* K18  */  be_nested_str_weak(_X20_X20engine_X2Einterrupt_current_X28_X29),
    /* K19  */  be_nested_str_weak(_X20_X20engine_X2Einterrupt_animation_X28_X22_X25s_X22_X29),
    /* K20  */  be_nested_str_weak(process_value),
    /* K21  */  be_nested_str_weak(CONTEXT_ANIMATION),
    /* K22  */  be_nested_str_weak(_X20_X20var_X20temp_anim_X20_X3D_X20_X25s),
    /* K23  */  be_nested_str_weak(expr),
    /* K24  */  be_nested_str_weak(_X20_X20engine_X2Eadd_X28temp_anim_X29),
    /* K25  */  be_nested_str_weak(end),
    /* K26  */  be_nested_str_weak(animation_X2Eregister_event_handler_X28_X22_X25s_X22_X2C_X20_X25s_X2C_X200_X2C_X20nil_X2C_X20_X25s_X29),
    }),
    be_str_weak(process_event_handler),
    &be_const_str_solidified,
    ( &(const binstruction[101]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x7C040200,  //  0003  CALL	R1	1
      0x8C080102,  //  0004  GETMET	R2	R0	K2
      0x7C080200,  //  0005  CALL	R2	1
      0x4C0C0000,  //  0006  LDNIL	R3
      0x20080403,  //  0007  NE	R2	R2	R3
      0x780A0003,  //  0008  JMPF	R2	#000D
      0x8C080102,  //  0009  GETMET	R2	R0	K2
      0x7C080200,  //  000A  CALL	R2	1
      0x88080503,  //  000B  GETMBR	R2	R2	K3
      0x70020000,  //  000C  JMP		#000E
      0x58080004,  //  000D  LDCONST	R2	K4
      0x580C0005,  //  000E  LDCONST	R3	K5
      0x8C100102,  //  000F  GETMET	R4	R0	K2
      0x7C100200,  //  0010  CALL	R4	1
      0x4C140000,  //  0011  LDNIL	R5
      0x20100805,  //  0012  NE	R4	R4	R5
      0x7812000A,  //  0013  JMPF	R4	#001F
      0x8C100102,  //  0014  GETMET	R4	R0	K2
      0x7C100200,  //  0015  CALL	R4	1
      0x88100906,  //  0016  GETMBR	R4	R4	K6
      0xB8160E00,  //  0017  GETNGBL	R5	K7
      0x88140B08,  //  0018  GETMBR	R5	R5	K8
      0x88140B09,  //  0019  GETMBR	R5	R5	K9
      0x1C100805,  //  001A  EQ	R4	R4	R5
      0x78120002,  //  001B  JMPF	R4	#001F
      0x8C10010A,  //  001C  GETMET	R4	R0	K10
      0x7C100200,  //  001D  CALL	R4	1
      0x5C0C0800,  //  001E  MOVE	R3	R4
      0x8C10010B,  //  001F  GETMET	R4	R0	K11
      0x7C100200,  //  0020  CALL	R4	1
      0x60100018,  //  0021  GETGBL	R4	G24
      0x5814000C,  //  0022  LDCONST	R5	K12
      0x5C180200,  //  0023  MOVE	R6	R1
      0x5C1C0400,  //  0024  MOVE	R7	R2
      0x7C100600,  //  0025  CALL	R4	3
      0x8C14010D,  //  0026  GETMET	R5	R0	K13
      0x601C0018,  //  0027  GETGBL	R7	G24
      0x5820000E,  //  0028  LDCONST	R8	K14
      0x5C240800,  //  0029  MOVE	R9	R4
      0x7C1C0400,  //  002A  CALL	R7	2
      0x7C140400,  //  002B  CALL	R5	2
      0x8C140102,  //  002C  GETMET	R5	R0	K2
      0x7C140200,  //  002D  CALL	R5	1
      0x4C180000,  //  002E  LDNIL	R6
      0x20180A06,  //  002F  NE	R6	R5	R6
      0x781A0027,  //  0030  JMPF	R6	#0059
      0x88180B06,  //  0031  GETMBR	R6	R5	K6
      0xB81E0E00,  //  0032  GETNGBL	R7	K7
      0x881C0F08,  //  0033  GETMBR	R7	R7	K8
      0x881C0F0F,  //  0034  GETMBR	R7	R7	K15
      0x1C180C07,  //  0035  EQ	R6	R6	R7
      0x781A0013,  //  0036  JMPF	R6	#004B
      0x88180B10,  //  0037  GETMBR	R6	R5	K16
      0x1C180D11,  //  0038  EQ	R6	R6	K17
      0x781A0010,  //  0039  JMPF	R6	#004B
      0x8C180100,  //  003A  GETMET	R6	R0	K0
      0x7C180200,  //  003B  CALL	R6	1
      0x8C180101,  //  003C  GETMET	R6	R0	K1
      0x7C180200,  //  003D  CALL	R6	1
      0x1C1C0D02,  //  003E  EQ	R7	R6	K2
      0x781E0003,  //  003F  JMPF	R7	#0044
      0x8C1C010D,  //  0040  GETMET	R7	R0	K13
      0x58240012,  //  0041  LDCONST	R9	K18
      0x7C1C0400,  //  0042  CALL	R7	2
      0x70020005,  //  0043  JMP		#004A
      0x8C1C010D,  //  0044  GETMET	R7	R0	K13
      0x60240018,  //  0045  GETGBL	R9	G24
      0x58280013,  //  0046  LDCONST	R10	K19
      0x5C2C0C00,  //  0047  MOVE	R11	R6
      0x7C240400,  //  0048  CALL	R9	2
      0x7C1C0400,  //  0049  CALL	R7	2
      0x7002000D,  //  004A  JMP		#0059
      0x8C180114,  //  004B  GETMET	R6	R0	K20
      0x88200115,  //  004C  GETMBR	R8	R0	K21
      0x7C180400,  //  004D  CALL	R6	2
      0x8C1C010D,  //  004E  GETMET	R7	R0	K13
      0x60240018,  //  004F  GETGBL	R9	G24
      0x58280016,  //  0050  LDCONST	R10	K22
      0x882C0D17,  //  0051  GETMBR	R11	R6	K23
      0x7C240400,  //  0052  CALL	R9	2
      0x7C1C0400,  //  0053  CALL	R7	2
      0x8C1C010D,  //  0054  GETMET	R7	R0	K13
      0x60240018,  //  0055  GETGBL	R9	G24
      0x58280018,  //  0056  LDCONST	R10	K24
      0x7C240200,  //  0057  CALL	R9	1
      0x7C1C0400,  //  0058  CALL	R7	2
      0x8C18010D,  //  0059  GETMET	R6	R0	K13
      0x58200019,  //  005A  LDCONST	R8	K25
      0x7C180400,  //  005B  CALL	R6	2
      0x8C18010D,  //  005C  GETMET	R6	R0	K13
      0x60200018,  //  005D  GETGBL	R8	G24
      0x5824001A,  //  005E  LDCONST	R9	K26
      0x5C280200,  //  005F  MOVE	R10	R1
      0x5C2C0800,  //  0060  MOVE	R11	R4
      0x5C300600,  //  0061  MOVE	R12	R3
      0x7C200800,  //  0062  CALL	R8	4
      0x7C180400,  //  0063  CALL	R6	2
      0x80000000,  //  0064  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _determine_symbol_return_type
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__determine_symbol_return_type,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[25]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation_dsl),
    /* K1   */  be_nested_str_weak(is_color_name),
    /* K2   */  be_nested_str_weak(color),
    /* K3   */  be_nested_str_weak(symbol_table),
    /* K4   */  be_nested_str_weak(get),
    /* K5   */  be_nested_str_weak(type),
    /* K6   */  be_nested_str_weak(_symbol_entry),
    /* K7   */  be_nested_str_weak(TYPE_ANIMATION),
    /* K8   */  be_nested_str_weak(TYPE_ANIMATION_CONSTRUCTOR),
    /* K9   */  be_nested_str_weak(animation),
    /* K10  */  be_nested_str_weak(TYPE_COLOR),
    /* K11  */  be_nested_str_weak(TYPE_COLOR_CONSTRUCTOR),
    /* K12  */  be_nested_str_weak(TYPE_VALUE_PROVIDER),
    /* K13  */  be_nested_str_weak(TYPE_VALUE_PROVIDER_CONSTRUCTOR),
    /* K14  */  be_nested_str_weak(value_provider),
    /* K15  */  be_nested_str_weak(TYPE_PALETTE),
    /* K16  */  be_nested_str_weak(TYPE_PALETTE_CONSTANT),
    /* K17  */  be_nested_str_weak(palette),
    /* K18  */  be_nested_str_weak(TYPE_CONSTANT),
    /* K19  */  be_nested_str_weak(variable),
    /* K20  */  be_nested_str_weak(TYPE_VARIABLE),
    /* K21  */  be_nested_str_weak(TYPE_SEQUENCE),
    /* K22  */  be_nested_str_weak(sequence),
    /* K23  */  be_nested_str_weak(TYPE_TEMPLATE),
    /* K24  */  be_nested_str_weak(template),
    }),
    be_str_weak(_determine_symbol_return_type),
    &be_const_str_solidified,
    ( &(const binstruction[101]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x780A0000,  //  0004  JMPF	R2	#0006
      0x80060400,  //  0005  RET	1	K2
      0x88080103,  //  0006  GETMBR	R2	R0	K3
      0x8C080504,  //  0007  GETMET	R2	R2	K4
      0x5C100200,  //  0008  MOVE	R4	R1
      0x7C080400,  //  0009  CALL	R2	2
      0x4C0C0000,  //  000A  LDNIL	R3
      0x200C0403,  //  000B  NE	R3	R2	R3
      0x780E0056,  //  000C  JMPF	R3	#0064
      0x880C0505,  //  000D  GETMBR	R3	R2	K5
      0xB8120000,  //  000E  GETNGBL	R4	K0
      0x88100906,  //  000F  GETMBR	R4	R4	K6
      0x88100907,  //  0010  GETMBR	R4	R4	K7
      0x1C0C0604,  //  0011  EQ	R3	R3	R4
      0x740E0005,  //  0012  JMPT	R3	#0019
      0x880C0505,  //  0013  GETMBR	R3	R2	K5
      0xB8120000,  //  0014  GETNGBL	R4	K0
      0x88100906,  //  0015  GETMBR	R4	R4	K6
      0x88100908,  //  0016  GETMBR	R4	R4	K8
      0x1C0C0604,  //  0017  EQ	R3	R3	R4
      0x780E0001,  //  0018  JMPF	R3	#001B
      0x80061200,  //  0019  RET	1	K9
      0x70020048,  //  001A  JMP		#0064
      0x880C0505,  //  001B  GETMBR	R3	R2	K5
      0xB8120000,  //  001C  GETNGBL	R4	K0
      0x88100906,  //  001D  GETMBR	R4	R4	K6
      0x8810090A,  //  001E  GETMBR	R4	R4	K10
      0x1C0C0604,  //  001F  EQ	R3	R3	R4
      0x740E0005,  //  0020  JMPT	R3	#0027
      0x880C0505,  //  0021  GETMBR	R3	R2	K5
      0xB8120000,  //  0022  GETNGBL	R4	K0
      0x88100906,  //  0023  GETMBR	R4	R4	K6
      0x8810090B,  //  0024  GETMBR	R4	R4	K11
      0x1C0C0604,  //  0025  EQ	R3	R3	R4
      0x780E0001,  //  0026  JMPF	R3	#0029
      0x80060400,  //  0027  RET	1	K2
      0x7002003A,  //  0028  JMP		#0064
      0x880C0505,  //  0029  GETMBR	R3	R2	K5
      0xB8120000,  //  002A  GETNGBL	R4	K0
      0x88100906,  //  002B  GETMBR	R4	R4	K6
      0x8810090C,  //  002C  GETMBR	R4	R4	K12
      0x1C0C0604,  //  002D  EQ	R3	R3	R4
      0x740E0005,  //  002E  JMPT	R3	#0035
      0x880C0505,  //  002F  GETMBR	R3	R2	K5
      0xB8120000,  //  0030  GETNGBL	R4	K0
      0x88100906,  //  0031  GETMBR	R4	R4	K6
      0x8810090D,  //  0032  GETMBR	R4	R4	K13
      0x1C0C0604,  //  0033  EQ	R3	R3	R4
      0x780E0001,  //  0034  JMPF	R3	#0037
      0x80061C00,  //  0035  RET	1	K14
      0x7002002C,  //  0036  JMP		#0064
      0x880C0505,  //  0037  GETMBR	R3	R2	K5
      0xB8120000,  //  0038  GETNGBL	R4	K0
      0x88100906,  //  0039  GETMBR	R4	R4	K6
      0x8810090F,  //  003A  GETMBR	R4	R4	K15
      0x1C0C0604,  //  003B  EQ	R3	R3	R4
      0x740E0005,  //  003C  JMPT	R3	#0043
      0x880C0505,  //  003D  GETMBR	R3	R2	K5
      0xB8120000,  //  003E  GETNGBL	R4	K0
      0x88100906,  //  003F  GETMBR	R4	R4	K6
      0x88100910,  //  0040  GETMBR	R4	R4	K16
      0x1C0C0604,  //  0041  EQ	R3	R3	R4
      0x780E0001,  //  0042  JMPF	R3	#0045
      0x80062200,  //  0043  RET	1	K17
      0x7002001E,  //  0044  JMP		#0064
      0x880C0505,  //  0045  GETMBR	R3	R2	K5
      0xB8120000,  //  0046  GETNGBL	R4	K0
      0x88100906,  //  0047  GETMBR	R4	R4	K6
      0x88100912,  //  0048  GETMBR	R4	R4	K18
      0x1C0C0604,  //  0049  EQ	R3	R3	R4
      0x780E0001,  //  004A  JMPF	R3	#004D
      0x80062600,  //  004B  RET	1	K19
      0x70020016,  //  004C  JMP		#0064
      0x880C0505,  //  004D  GETMBR	R3	R2	K5
      0xB8120000,  //  004E  GETNGBL	R4	K0
      0x88100906,  //  004F  GETMBR	R4	R4	K6
      0x88100914,  //  0050  GETMBR	R4	R4	K20
      0x1C0C0604,  //  0051  EQ	R3	R3	R4
      0x780E0001,  //  0052  JMPF	R3	#0055
      0x80062600,  //  0053  RET	1	K19
      0x7002000E,  //  0054  JMP		#0064
      0x880C0505,  //  0055  GETMBR	R3	R2	K5
      0xB8120000,  //  0056  GETNGBL	R4	K0
      0x88100906,  //  0057  GETMBR	R4	R4	K6
      0x88100915,  //  0058  GETMBR	R4	R4	K21
      0x1C0C0604,  //  0059  EQ	R3	R3	R4
      0x780E0001,  //  005A  JMPF	R3	#005D
      0x80062C00,  //  005B  RET	1	K22
      0x70020006,  //  005C  JMP		#0064
      0x880C0505,  //  005D  GETMBR	R3	R2	K5
      0xB8120000,  //  005E  GETNGBL	R4	K0
      0x88100906,  //  005F  GETMBR	R4	R4	K6
      0x88100917,  //  0060  GETMBR	R4	R4	K23
      0x1C0C0604,  //  0061  EQ	R3	R3	R4
      0x780E0000,  //  0062  JMPF	R3	#0064
      0x80063000,  //  0063  RET	1	K24
      0x80062600,  //  0064  RET	1	K19
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_sequence
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_sequence,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[31]) {     /* constants */
    /* K0   */  be_nested_str_weak(next),
    /* K1   */  be_nested_str_weak(expect_identifier),
    /* K2   */  be_nested_str_weak(validate_user_name),
    /* K3   */  be_nested_str_weak(sequence),
    /* K4   */  be_nested_str_weak(skip_statement),
    /* K5   */  be_nested_str_weak(symbol_table),
    /* K6   */  be_nested_str_weak(create_sequence),
    /* K7   */  be_nested_str_weak(1),
    /* K8   */  be_nested_str_weak(current),
    /* K9   */  be_nested_str_weak(type),
    /* K10  */  be_nested_str_weak(animation_dsl),
    /* K11  */  be_nested_str_weak(Token),
    /* K12  */  be_nested_str_weak(KEYWORD),
    /* K13  */  be_nested_str_weak(value),
    /* K14  */  be_nested_str_weak(repeat),
    /* K15  */  be_nested_str_weak(forever),
    /* K16  */  be_nested_str_weak(_X2D1),
    /* K17  */  be_nested_str_weak(process_value),
    /* K18  */  be_nested_str_weak(CONTEXT_REPEAT_COUNT),
    /* K19  */  be_nested_str_weak(expect_keyword),
    /* K20  */  be_nested_str_weak(times),
    /* K21  */  be_nested_str_weak(expr),
    /* K22  */  be_nested_str_weak(NUMBER),
    /* K23  */  be_nested_str_weak(expect_left_brace),
    /* K24  */  be_nested_str_weak(add),
    /* K25  */  be_nested_str_weak(var_X20_X25s__X20_X3D_X20animation_X2ESequenceManager_X28engine_X2C_X20_X25s_X29),
    /* K26  */  be_nested_str_weak(at_end),
    /* K27  */  be_nested_str_weak(check_right_brace),
    /* K28  */  be_nested_str_weak(process_sequence_statement),
    /* K29  */  be_nested_str_weak(var_X20_X25s__X20_X3D_X20animation_X2ESequenceManager_X28engine_X29),
    /* K30  */  be_nested_str_weak(expect_right_brace),
    }),
    be_str_weak(process_sequence),
    &be_const_str_solidified,
    ( &(const binstruction[124]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x7C040200,  //  0003  CALL	R1	1
      0x8C080102,  //  0004  GETMET	R2	R0	K2
      0x5C100200,  //  0005  MOVE	R4	R1
      0x58140003,  //  0006  LDCONST	R5	K3
      0x7C080600,  //  0007  CALL	R2	3
      0x740A0002,  //  0008  JMPT	R2	#000C
      0x8C080104,  //  0009  GETMET	R2	R0	K4
      0x7C080200,  //  000A  CALL	R2	1
      0x80000400,  //  000B  RET	0
      0x88080105,  //  000C  GETMBR	R2	R0	K5
      0x8C080506,  //  000D  GETMET	R2	R2	K6
      0x5C100200,  //  000E  MOVE	R4	R1
      0x7C080400,  //  000F  CALL	R2	2
      0x50080000,  //  0010  LDBOOL	R2	0	0
      0x580C0007,  //  0011  LDCONST	R3	K7
      0x8C100108,  //  0012  GETMET	R4	R0	K8
      0x7C100200,  //  0013  CALL	R4	1
      0x4C140000,  //  0014  LDNIL	R5
      0x20140805,  //  0015  NE	R5	R4	R5
      0x7816002D,  //  0016  JMPF	R5	#0045
      0x88140909,  //  0017  GETMBR	R5	R4	K9
      0xB81A1400,  //  0018  GETNGBL	R6	K10
      0x88180D0B,  //  0019  GETMBR	R6	R6	K11
      0x88180D0C,  //  001A  GETMBR	R6	R6	K12
      0x1C140A06,  //  001B  EQ	R5	R5	R6
      0x78160027,  //  001C  JMPF	R5	#0045
      0x8814090D,  //  001D  GETMBR	R5	R4	K13
      0x1C140B0E,  //  001E  EQ	R5	R5	K14
      0x7816001C,  //  001F  JMPF	R5	#003D
      0x50080200,  //  0020  LDBOOL	R2	1	0
      0x8C140100,  //  0021  GETMET	R5	R0	K0
      0x7C140200,  //  0022  CALL	R5	1
      0x8C140108,  //  0023  GETMET	R5	R0	K8
      0x7C140200,  //  0024  CALL	R5	1
      0x4C180000,  //  0025  LDNIL	R6
      0x20180A06,  //  0026  NE	R6	R5	R6
      0x781A000C,  //  0027  JMPF	R6	#0035
      0x88180B09,  //  0028  GETMBR	R6	R5	K9
      0xB81E1400,  //  0029  GETNGBL	R7	K10
      0x881C0F0B,  //  002A  GETMBR	R7	R7	K11
      0x881C0F0C,  //  002B  GETMBR	R7	R7	K12
      0x1C180C07,  //  002C  EQ	R6	R6	R7
      0x781A0006,  //  002D  JMPF	R6	#0035
      0x88180B0D,  //  002E  GETMBR	R6	R5	K13
      0x1C180D0F,  //  002F  EQ	R6	R6	K15
      0x781A0003,  //  0030  JMPF	R6	#0035
      0x8C180100,  //  0031  GETMET	R6	R0	K0
      0x7C180200,  //  0032  CALL	R6	1
      0x580C0010,  //  0033  LDCONST	R3	K16
      0x70020006,  //  0034  JMP		#003C
      0x8C180111,  //  0035  GETMET	R6	R0	K17
      0x88200112,  //  0036  GETMBR	R8	R0	K18
      0x7C180400,  //  0037  CALL	R6	2
      0x8C1C0113,  //  0038  GETMET	R7	R0	K19
      0x58240014,  //  0039  LDCONST	R9	K20
      0x7C1C0400,  //  003A  CALL	R7	2
      0x880C0D15,  //  003B  GETMBR	R3	R6	K21
      0x70020006,  //  003C  JMP		#0044
      0x8814090D,  //  003D  GETMBR	R5	R4	K13
      0x1C140B0F,  //  003E  EQ	R5	R5	K15
      0x78160003,  //  003F  JMPF	R5	#0044
      0x50080200,  //  0040  LDBOOL	R2	1	0
      0x8C140100,  //  0041  GETMET	R5	R0	K0
      0x7C140200,  //  0042  CALL	R5	1
      0x580C0010,  //  0043  LDCONST	R3	K16
      0x70020010,  //  0044  JMP		#0056
      0x4C140000,  //  0045  LDNIL	R5
      0x20140805,  //  0046  NE	R5	R4	R5
      0x7816000D,  //  0047  JMPF	R5	#0056
      0x88140909,  //  0048  GETMBR	R5	R4	K9
      0xB81A1400,  //  0049  GETNGBL	R6	K10
      0x88180D0B,  //  004A  GETMBR	R6	R6	K11
      0x88180D16,  //  004B  GETMBR	R6	R6	K22
      0x1C140A06,  //  004C  EQ	R5	R5	R6
      0x78160007,  //  004D  JMPF	R5	#0056
      0x50080200,  //  004E  LDBOOL	R2	1	0
      0x8C140111,  //  004F  GETMET	R5	R0	K17
      0x881C0112,  //  0050  GETMBR	R7	R0	K18
      0x7C140400,  //  0051  CALL	R5	2
      0x8C180113,  //  0052  GETMET	R6	R0	K19
      0x58200014,  //  0053  LDCONST	R8	K20
      0x7C180400,  //  0054  CALL	R6	2
      0x880C0B15,  //  0055  GETMBR	R3	R5	K21
      0x8C140117,  //  0056  GETMET	R5	R0	K23
      0x7C140200,  //  0057  CALL	R5	1
      0x780A0010,  //  0058  JMPF	R2	#006A
      0x8C140118,  //  0059  GETMET	R5	R0	K24
      0x601C0018,  //  005A  GETGBL	R7	G24
      0x58200019,  //  005B  LDCONST	R8	K25
      0x5C240200,  //  005C  MOVE	R9	R1
      0x5C280600,  //  005D  MOVE	R10	R3
      0x7C1C0600,  //  005E  CALL	R7	3
      0x7C140400,  //  005F  CALL	R5	2
      0x8C14011A,  //  0060  GETMET	R5	R0	K26
      0x7C140200,  //  0061  CALL	R5	1
      0x74160005,  //  0062  JMPT	R5	#0069
      0x8C14011B,  //  0063  GETMET	R5	R0	K27
      0x7C140200,  //  0064  CALL	R5	1
      0x74160002,  //  0065  JMPT	R5	#0069
      0x8C14011C,  //  0066  GETMET	R5	R0	K28
      0x7C140200,  //  0067  CALL	R5	1
      0x7001FFF6,  //  0068  JMP		#0060
      0x7002000E,  //  0069  JMP		#0079
      0x8C140118,  //  006A  GETMET	R5	R0	K24
      0x601C0018,  //  006B  GETGBL	R7	G24
      0x5820001D,  //  006C  LDCONST	R8	K29
      0x5C240200,  //  006D  MOVE	R9	R1
      0x7C1C0400,  //  006E  CALL	R7	2
      0x7C140400,  //  006F  CALL	R5	2
      0x8C14011A,  //  0070  GETMET	R5	R0	K26
      0x7C140200,  //  0071  CALL	R5	1
      0x74160005,  //  0072  JMPT	R5	#0079
      0x8C14011B,  //  0073  GETMET	R5	R0	K27
      0x7C140200,  //  0074  CALL	R5	1
      0x74160002,  //  0075  JMPT	R5	#0079
      0x8C14011C,  //  0076  GETMET	R5	R0	K28
      0x7C140200,  //  0077  CALL	R5	1
      0x7001FFF6,  //  0078  JMP		#0070
      0x8C14011E,  //  0079  GETMET	R5	R0	K30
      0x7C140200,  //  007A  CALL	R5	1
      0x80000000,  //  007B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_computed_expression
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_is_computed_expression,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(_X28),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(animation_X2E),
    /* K5   */  be_nested_str_weak(_),
    }),
    be_str_weak(is_computed_expression),
    &be_const_str_solidified,
    ( &(const binstruction[40]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0x8C140901,  //  0001  GETMET	R5	R4	K1
      0x5C1C0200,  //  0002  MOVE	R7	R1
      0x58200002,  //  0003  LDCONST	R8	K2
      0x7C140600,  //  0004  CALL	R5	3
      0x28140B03,  //  0005  GE	R5	R5	K3
      0x7416001E,  //  0006  JMPT	R5	#0026
      0x8C140901,  //  0007  GETMET	R5	R4	K1
      0x5C1C0600,  //  0008  MOVE	R7	R3
      0x58200002,  //  0009  LDCONST	R8	K2
      0x7C140600,  //  000A  CALL	R5	3
      0x28140B03,  //  000B  GE	R5	R5	K3
      0x74160018,  //  000C  JMPT	R5	#0026
      0x8C140901,  //  000D  GETMET	R5	R4	K1
      0x5C1C0200,  //  000E  MOVE	R7	R1
      0x58200004,  //  000F  LDCONST	R8	K4
      0x7C140600,  //  0010  CALL	R5	3
      0x28140B03,  //  0011  GE	R5	R5	K3
      0x74160012,  //  0012  JMPT	R5	#0026
      0x8C140901,  //  0013  GETMET	R5	R4	K1
      0x5C1C0600,  //  0014  MOVE	R7	R3
      0x58200004,  //  0015  LDCONST	R8	K4
      0x7C140600,  //  0016  CALL	R5	3
      0x28140B03,  //  0017  GE	R5	R5	K3
      0x7416000C,  //  0018  JMPT	R5	#0026
      0x8C140901,  //  0019  GETMET	R5	R4	K1
      0x5C1C0200,  //  001A  MOVE	R7	R1
      0x58200005,  //  001B  LDCONST	R8	K5
      0x7C140600,  //  001C  CALL	R5	3
      0x28140B03,  //  001D  GE	R5	R5	K3
      0x74160006,  //  001E  JMPT	R5	#0026
      0x8C140901,  //  001F  GETMET	R5	R4	K1
      0x5C1C0600,  //  0020  MOVE	R7	R3
      0x58200005,  //  0021  LDCONST	R8	K5
      0x7C140600,  //  0022  CALL	R5	3
      0x28140B03,  //  0023  GE	R5	R5	K3
      0x74160000,  //  0024  JMPT	R5	#0026
      0x50140001,  //  0025  LDBOOL	R5	0	1
      0x50140200,  //  0026  LDBOOL	R5	1	0
      0x80040A00,  //  0027  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _validate_animation_factory_exists
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__validate_animation_factory_exists,   /* name */
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
    /* K0   */  be_nested_str_weak(symbol_table),
    /* K1   */  be_nested_str_weak(get),
    }),
    be_str_weak(_validate_animation_factory_exists),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x4C0C0000,  //  0004  LDNIL	R3
      0x200C0403,  //  0005  NE	R3	R2	R3
      0x80040600,  //  0006  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: convert_to_vrgb
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_convert_to_vrgb,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_weak(format),
    /* K3   */  be_nested_str_weak(_X2502X),
    /* K4   */  be_nested_str_weak(FFFFFF),
    /* K5   */  be_nested_str_weak(startswith),
    /* K6   */  be_nested_str_weak(0x),
    /* K7   */  be_const_int(2),
    }),
    be_str_weak(convert_to_vrgb),
    &be_const_str_solidified,
    ( &(const binstruction[54]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0x60100009,  //  0001  GETGBL	R4	G9
      0x6014000A,  //  0002  GETGBL	R5	G10
      0x5C180200,  //  0003  MOVE	R6	R1
      0x7C140200,  //  0004  CALL	R5	1
      0x7C100200,  //  0005  CALL	R4	1
      0x14140901,  //  0006  LT	R5	R4	K1
      0x78160001,  //  0007  JMPF	R5	#000A
      0x58100001,  //  0008  LDCONST	R4	K1
      0x70020003,  //  0009  JMP		#000E
      0x541600FE,  //  000A  LDINT	R5	255
      0x24140805,  //  000B  GT	R5	R4	R5
      0x78160000,  //  000C  JMPF	R5	#000E
      0x541200FE,  //  000D  LDINT	R4	255
      0x8C140702,  //  000E  GETMET	R5	R3	K2
      0x581C0003,  //  000F  LDCONST	R7	K3
      0x5C200800,  //  0010  MOVE	R8	R4
      0x7C140600,  //  0011  CALL	R5	3
      0x60180008,  //  0012  GETGBL	R6	G8
      0x5C1C0400,  //  0013  MOVE	R7	R2
      0x7C180200,  //  0014  CALL	R6	1
      0x581C0004,  //  0015  LDCONST	R7	K4
      0x8C200705,  //  0016  GETMET	R8	R3	K5
      0x5C280C00,  //  0017  MOVE	R10	R6
      0x582C0006,  //  0018  LDCONST	R11	K6
      0x7C200600,  //  0019  CALL	R8	3
      0x7822000A,  //  001A  JMPF	R8	#0026
      0x6020000C,  //  001B  GETGBL	R8	G12
      0x5C240C00,  //  001C  MOVE	R9	R6
      0x7C200200,  //  001D  CALL	R8	1
      0x54260009,  //  001E  LDINT	R9	10
      0x28201009,  //  001F  GE	R8	R8	R9
      0x78220004,  //  0020  JMPF	R8	#0026
      0x54220003,  //  0021  LDINT	R8	4
      0x54260008,  //  0022  LDINT	R9	9
      0x40201009,  //  0023  CONNECT	R8	R8	R9
      0x941C0C08,  //  0024  GETIDX	R7	R6	R8
      0x7002000D,  //  0025  JMP		#0034
      0x8C200705,  //  0026  GETMET	R8	R3	K5
      0x5C280C00,  //  0027  MOVE	R10	R6
      0x582C0006,  //  0028  LDCONST	R11	K6
      0x7C200600,  //  0029  CALL	R8	3
      0x78220008,  //  002A  JMPF	R8	#0034
      0x6020000C,  //  002B  GETGBL	R8	G12
      0x5C240C00,  //  002C  MOVE	R9	R6
      0x7C200200,  //  002D  CALL	R8	1
      0x54260007,  //  002E  LDINT	R9	8
      0x1C201009,  //  002F  EQ	R8	R8	R9
      0x78220002,  //  0030  JMPF	R8	#0034
      0x54220006,  //  0031  LDINT	R8	7
      0x40220E08,  //  0032  CONNECT	R8	K7	R8
      0x941C0C08,  //  0033  GETIDX	R7	R6	R8
      0x00200A07,  //  0034  ADD	R8	R5	R7
      0x80041000,  //  0035  RET	1	R8
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: expect_right_bracket
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_expect_right_bracket,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(RIGHT_BRACKET),
    /* K5   */  be_nested_str_weak(next),
    /* K6   */  be_nested_str_weak(error),
    /* K7   */  be_nested_str_weak(Expected_X20_X27_X5D_X27),
    }),
    be_str_weak(expect_right_bracket),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0008,  //  0004  JMPF	R2	#000E
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x780A0002,  //  000A  JMPF	R2	#000E
      0x8C080105,  //  000B  GETMET	R2	R0	K5
      0x7C080200,  //  000C  CALL	R2	1
      0x70020002,  //  000D  JMP		#0011
      0x8C080106,  //  000E  GETMET	R2	R0	K6
      0x58100007,  //  000F  LDCONST	R4	K7
      0x7C080400,  //  0010  CALL	R2	2
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: skip_function_arguments
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_skip_function_arguments,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(LEFT_PAREN),
    /* K5   */  be_nested_str_weak(next),
    /* K6   */  be_const_int(1),
    /* K7   */  be_nested_str_weak(at_end),
    /* K8   */  be_const_int(0),
    /* K9   */  be_nested_str_weak(RIGHT_PAREN),
    }),
    be_str_weak(skip_function_arguments),
    &be_const_str_solidified,
    ( &(const binstruction[42]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20040202,  //  0003  NE	R1	R1	R2
      0x78060023,  //  0004  JMPF	R1	#0029
      0x8C040100,  //  0005  GETMET	R1	R0	K0
      0x7C040200,  //  0006  CALL	R1	1
      0x88040301,  //  0007  GETMBR	R1	R1	K1
      0xB80A0400,  //  0008  GETNGBL	R2	K2
      0x88080503,  //  0009  GETMBR	R2	R2	K3
      0x88080504,  //  000A  GETMBR	R2	R2	K4
      0x1C040202,  //  000B  EQ	R1	R1	R2
      0x7806001B,  //  000C  JMPF	R1	#0029
      0x8C040105,  //  000D  GETMET	R1	R0	K5
      0x7C040200,  //  000E  CALL	R1	1
      0x58040006,  //  000F  LDCONST	R1	K6
      0x8C080107,  //  0010  GETMET	R2	R0	K7
      0x7C080200,  //  0011  CALL	R2	1
      0x740A0015,  //  0012  JMPT	R2	#0029
      0x24080308,  //  0013  GT	R2	R1	K8
      0x780A0013,  //  0014  JMPF	R2	#0029
      0x8C080100,  //  0015  GETMET	R2	R0	K0
      0x7C080200,  //  0016  CALL	R2	1
      0x880C0501,  //  0017  GETMBR	R3	R2	K1
      0xB8120400,  //  0018  GETNGBL	R4	K2
      0x88100903,  //  0019  GETMBR	R4	R4	K3
      0x88100904,  //  001A  GETMBR	R4	R4	K4
      0x1C0C0604,  //  001B  EQ	R3	R3	R4
      0x780E0001,  //  001C  JMPF	R3	#001F
      0x00040306,  //  001D  ADD	R1	R1	K6
      0x70020006,  //  001E  JMP		#0026
      0x880C0501,  //  001F  GETMBR	R3	R2	K1
      0xB8120400,  //  0020  GETNGBL	R4	K2
      0x88100903,  //  0021  GETMBR	R4	R4	K3
      0x88100909,  //  0022  GETMBR	R4	R4	K9
      0x1C0C0604,  //  0023  EQ	R3	R3	R4
      0x780E0000,  //  0024  JMPF	R3	#0026
      0x04040306,  //  0025  SUB	R1	R1	K6
      0x8C0C0105,  //  0026  GETMET	R3	R0	K5
      0x7C0C0200,  //  0027  CALL	R3	1
      0x7001FFE6,  //  0028  JMP		#0010
      0x80000000,  //  0029  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: expect_comma
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_expect_comma,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(COMMA),
    /* K5   */  be_nested_str_weak(next),
    /* K6   */  be_nested_str_weak(error),
    /* K7   */  be_nested_str_weak(Expected_X20_X27_X2C_X27),
    }),
    be_str_weak(expect_comma),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0008,  //  0004  JMPF	R2	#000E
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x780A0002,  //  000A  JMPF	R2	#000E
      0x8C080105,  //  000B  GETMET	R2	R0	K5
      0x7C080200,  //  000C  CALL	R2	1
      0x70020002,  //  000D  JMP		#0011
      0x8C080106,  //  000E  GETMET	R2	R0	K6
      0x58100007,  //  000F  LDCONST	R4	K7
      0x7C080400,  //  0010  CALL	R2	2
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_additive_expression
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_additive_expression,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(process_multiplicative_expression),
    /* K1   */  be_nested_str_weak(at_end),
    /* K2   */  be_nested_str_weak(current),
    /* K3   */  be_nested_str_weak(type),
    /* K4   */  be_nested_str_weak(animation_dsl),
    /* K5   */  be_nested_str_weak(Token),
    /* K6   */  be_nested_str_weak(PLUS),
    /* K7   */  be_nested_str_weak(MINUS),
    /* K8   */  be_nested_str_weak(value),
    /* K9   */  be_nested_str_weak(next),
    /* K10  */  be_nested_str_weak(ExpressionResult),
    /* K11  */  be_nested_str_weak(combine),
    /* K12  */  be_nested_str_weak(_X25s_X20_X25s_X20_X25s),
    /* K13  */  be_nested_str_weak(expr),
    }),
    be_str_weak(process_additive_expression),
    &be_const_str_solidified,
    ( &(const binstruction[49]) {  /* code */
      0x8C100100,  //  0000  GETMET	R4	R0	K0
      0x5C180200,  //  0001  MOVE	R6	R1
      0x5C1C0400,  //  0002  MOVE	R7	R2
      0x5C200600,  //  0003  MOVE	R8	R3
      0x7C100800,  //  0004  CALL	R4	4
      0x8C140101,  //  0005  GETMET	R5	R0	K1
      0x7C140200,  //  0006  CALL	R5	1
      0x74160027,  //  0007  JMPT	R5	#0030
      0x8C140102,  //  0008  GETMET	R5	R0	K2
      0x7C140200,  //  0009  CALL	R5	1
      0x4C180000,  //  000A  LDNIL	R6
      0x20180A06,  //  000B  NE	R6	R5	R6
      0x781A0020,  //  000C  JMPF	R6	#002E
      0x88180B03,  //  000D  GETMBR	R6	R5	K3
      0xB81E0800,  //  000E  GETNGBL	R7	K4
      0x881C0F05,  //  000F  GETMBR	R7	R7	K5
      0x881C0F06,  //  0010  GETMBR	R7	R7	K6
      0x1C180C07,  //  0011  EQ	R6	R6	R7
      0x741A0005,  //  0012  JMPT	R6	#0019
      0x88180B03,  //  0013  GETMBR	R6	R5	K3
      0xB81E0800,  //  0014  GETNGBL	R7	K4
      0x881C0F05,  //  0015  GETMBR	R7	R7	K5
      0x881C0F07,  //  0016  GETMBR	R7	R7	K7
      0x1C180C07,  //  0017  EQ	R6	R6	R7
      0x781A0014,  //  0018  JMPF	R6	#002E
      0x88180B08,  //  0019  GETMBR	R6	R5	K8
      0x8C1C0109,  //  001A  GETMET	R7	R0	K9
      0x7C1C0200,  //  001B  CALL	R7	1
      0x8C1C0100,  //  001C  GETMET	R7	R0	K0
      0x5C240200,  //  001D  MOVE	R9	R1
      0x50280000,  //  001E  LDBOOL	R10	0	0
      0x5C2C0600,  //  001F  MOVE	R11	R3
      0x7C1C0800,  //  0020  CALL	R7	4
      0x8820010A,  //  0021  GETMBR	R8	R0	K10
      0x8C20110B,  //  0022  GETMET	R8	R8	K11
      0x60280018,  //  0023  GETGBL	R10	G24
      0x582C000C,  //  0024  LDCONST	R11	K12
      0x8830090D,  //  0025  GETMBR	R12	R4	K13
      0x5C340C00,  //  0026  MOVE	R13	R6
      0x88380F0D,  //  0027  GETMBR	R14	R7	K13
      0x7C280800,  //  0028  CALL	R10	4
      0x5C2C0800,  //  0029  MOVE	R11	R4
      0x5C300E00,  //  002A  MOVE	R12	R7
      0x7C200800,  //  002B  CALL	R8	4
      0x5C101000,  //  002C  MOVE	R4	R8
      0x70020000,  //  002D  JMP		#002F
      0x70020000,  //  002E  JMP		#0030
      0x7001FFD4,  //  002F  JMP		#0005
      0x80040800,  //  0030  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_errors
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_get_errors,   /* name */
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
    /* K0   */  be_nested_str_weak(errors),
    }),
    be_str_weak(get_errors),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_computed_expression_string
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_is_computed_expression_string,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(_X20_X2B_X20),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(_X20_X2D_X20),
    /* K5   */  be_nested_str_weak(_X20_X2A_X20),
    /* K6   */  be_nested_str_weak(_X20_X2F_X20),
    /* K7   */  be_nested_str_weak(_X28),
    /* K8   */  be_const_int(1),
    /* K9   */  be_nested_str_weak(is_identifier_char),
    /* K10  */  be_nested_str_weak(_is_named_arg_constructor),
    }),
    be_str_weak(is_computed_expression_string),
    &be_const_str_solidified,
    ( &(const binstruction[63]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0501,  //  0001  GETMET	R3	R2	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x58180002,  //  0003  LDCONST	R6	K2
      0x7C0C0600,  //  0004  CALL	R3	3
      0x280C0703,  //  0005  GE	R3	R3	K3
      0x740E0012,  //  0006  JMPT	R3	#001A
      0x8C0C0501,  //  0007  GETMET	R3	R2	K1
      0x5C140200,  //  0008  MOVE	R5	R1
      0x58180004,  //  0009  LDCONST	R6	K4
      0x7C0C0600,  //  000A  CALL	R3	3
      0x280C0703,  //  000B  GE	R3	R3	K3
      0x740E000C,  //  000C  JMPT	R3	#001A
      0x8C0C0501,  //  000D  GETMET	R3	R2	K1
      0x5C140200,  //  000E  MOVE	R5	R1
      0x58180005,  //  000F  LDCONST	R6	K5
      0x7C0C0600,  //  0010  CALL	R3	3
      0x280C0703,  //  0011  GE	R3	R3	K3
      0x740E0006,  //  0012  JMPT	R3	#001A
      0x8C0C0501,  //  0013  GETMET	R3	R2	K1
      0x5C140200,  //  0014  MOVE	R5	R1
      0x58180006,  //  0015  LDCONST	R6	K6
      0x7C0C0600,  //  0016  CALL	R3	3
      0x280C0703,  //  0017  GE	R3	R3	K3
      0x740E0000,  //  0018  JMPT	R3	#001A
      0x500C0001,  //  0019  LDBOOL	R3	0	1
      0x500C0200,  //  001A  LDBOOL	R3	1	0
      0x50100000,  //  001B  LDBOOL	R4	0	0
      0x8C140501,  //  001C  GETMET	R5	R2	K1
      0x5C1C0200,  //  001D  MOVE	R7	R1
      0x58200007,  //  001E  LDCONST	R8	K7
      0x7C140600,  //  001F  CALL	R5	3
      0x24180B03,  //  0020  GT	R6	R5	K3
      0x781A0017,  //  0021  JMPF	R6	#003A
      0x04180B08,  //  0022  SUB	R6	R5	K8
      0x94180206,  //  0023  GETIDX	R6	R1	R6
      0x8C1C0109,  //  0024  GETMET	R7	R0	K9
      0x5C240C00,  //  0025  MOVE	R9	R6
      0x7C1C0400,  //  0026  CALL	R7	2
      0x781E0011,  //  0027  JMPF	R7	#003A
      0x041C0B08,  //  0028  SUB	R7	R5	K8
      0x28200F03,  //  0029  GE	R8	R7	K3
      0x78220005,  //  002A  JMPF	R8	#0031
      0x8C200109,  //  002B  GETMET	R8	R0	K9
      0x94280207,  //  002C  GETIDX	R10	R1	R7
      0x7C200400,  //  002D  CALL	R8	2
      0x78220001,  //  002E  JMPF	R8	#0031
      0x041C0F08,  //  002F  SUB	R7	R7	K8
      0x7001FFF7,  //  0030  JMP		#0029
      0x001C0F08,  //  0031  ADD	R7	R7	K8
      0x04200B08,  //  0032  SUB	R8	R5	K8
      0x40200E08,  //  0033  CONNECT	R8	R7	R8
      0x94200208,  //  0034  GETIDX	R8	R1	R8
      0x8C24010A,  //  0035  GETMET	R9	R0	K10
      0x5C2C1000,  //  0036  MOVE	R11	R8
      0x7C240400,  //  0037  CALL	R9	2
      0x78260000,  //  0038  JMPF	R9	#003A
      0x50100200,  //  0039  LDBOOL	R4	1	0
      0x740E0001,  //  003A  JMPT	R3	#003D
      0x74120000,  //  003B  JMPT	R4	#003D
      0x50180001,  //  003C  LDBOOL	R6	0	1
      0x50180200,  //  003D  LDBOOL	R6	1	0
      0x80040C00,  //  003E  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: expect_right_paren
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_expect_right_paren,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(RIGHT_PAREN),
    /* K5   */  be_nested_str_weak(next),
    /* K6   */  be_nested_str_weak(error),
    /* K7   */  be_nested_str_weak(Expected_X20_X27_X29_X27),
    }),
    be_str_weak(expect_right_paren),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0008,  //  0004  JMPF	R2	#000E
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x780A0002,  //  000A  JMPF	R2	#000E
      0x8C080105,  //  000B  GETMET	R2	R0	K5
      0x7C080200,  //  000C  CALL	R2	1
      0x70020002,  //  000D  JMP		#0011
      0x8C080106,  //  000E  GETMET	R2	R0	K6
      0x58100007,  //  000F  LDCONST	R4	K7
      0x7C080400,  //  0010  CALL	R2	2
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _process_named_arguments_for_color_provider
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__process_named_arguments_for_color_provider,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_process_named_arguments_unified),
    /* K1   */  be_nested_str_weak(CONTEXT_COLOR_PROVIDER),
    }),
    be_str_weak(_process_named_arguments_for_color_provider),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C0C0100,  //  0000  GETMET	R3	R0	K0
      0x5C140200,  //  0001  MOVE	R5	R1
      0x5C180400,  //  0002  MOVE	R6	R2
      0x881C0101,  //  0003  GETMBR	R7	R0	K1
      0x7C0C0800,  //  0004  CALL	R3	4
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_berry_code_block
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_berry_code_block,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_nested_str_weak(next),
    /* K1   */  be_nested_str_weak(current),
    /* K2   */  be_nested_str_weak(type),
    /* K3   */  be_nested_str_weak(animation_dsl),
    /* K4   */  be_nested_str_weak(Token),
    /* K5   */  be_nested_str_weak(STRING),
    /* K6   */  be_nested_str_weak(error),
    /* K7   */  be_nested_str_weak(Expected_X20string_X20literal_X20after_X20_X27berry_X27_X20keyword_X2E_X20Use_X20berry_X20_X22_X22_X22_X3Ccode_X3E_X22_X22_X22_X20or_X20berry_X20_X27_X27_X27_X3Ccode_X3E_X27_X27_X27),
    /* K8   */  be_nested_str_weak(skip_statement),
    /* K9   */  be_nested_str_weak(value),
    /* K10  */  be_nested_str_weak(collect_inline_comment),
    /* K11  */  be_nested_str_weak(add),
    /* K12  */  be_nested_str_weak(_X23_X20Berry_X20code_X20block_X25s),
    /* K13  */  be_nested_str_weak(string),
    /* K14  */  be_nested_str_weak(split),
    /* K15  */  be_nested_str_weak(_X0A),
    /* K16  */  be_nested_str_weak(stop_iteration),
    /* K17  */  be_nested_str_weak(_X23_X20End_X20berry_X20code_X20block),
    }),
    be_str_weak(process_berry_code_block),
    &be_const_str_solidified,
    ( &(const binstruction[52]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x7C040200,  //  0003  CALL	R1	1
      0x4C080000,  //  0004  LDNIL	R2
      0x1C080202,  //  0005  EQ	R2	R1	R2
      0x740A0005,  //  0006  JMPT	R2	#000D
      0x88080302,  //  0007  GETMBR	R2	R1	K2
      0xB80E0600,  //  0008  GETNGBL	R3	K3
      0x880C0704,  //  0009  GETMBR	R3	R3	K4
      0x880C0705,  //  000A  GETMBR	R3	R3	K5
      0x20080403,  //  000B  NE	R2	R2	R3
      0x780A0005,  //  000C  JMPF	R2	#0013
      0x8C080106,  //  000D  GETMET	R2	R0	K6
      0x58100007,  //  000E  LDCONST	R4	K7
      0x7C080400,  //  000F  CALL	R2	2
      0x8C080108,  //  0010  GETMET	R2	R0	K8
      0x7C080200,  //  0011  CALL	R2	1
      0x80000400,  //  0012  RET	0
      0x88080309,  //  0013  GETMBR	R2	R1	K9
      0x8C0C0100,  //  0014  GETMET	R3	R0	K0
      0x7C0C0200,  //  0015  CALL	R3	1
      0x8C0C010A,  //  0016  GETMET	R3	R0	K10
      0x7C0C0200,  //  0017  CALL	R3	1
      0x8C10010B,  //  0018  GETMET	R4	R0	K11
      0x60180018,  //  0019  GETGBL	R6	G24
      0x581C000C,  //  001A  LDCONST	R7	K12
      0x5C200600,  //  001B  MOVE	R8	R3
      0x7C180400,  //  001C  CALL	R6	2
      0x7C100400,  //  001D  CALL	R4	2
      0xA4121A00,  //  001E  IMPORT	R4	K13
      0x8C14090E,  //  001F  GETMET	R5	R4	K14
      0x5C1C0400,  //  0020  MOVE	R7	R2
      0x5820000F,  //  0021  LDCONST	R8	K15
      0x7C140600,  //  0022  CALL	R5	3
      0x60180010,  //  0023  GETGBL	R6	G16
      0x5C1C0A00,  //  0024  MOVE	R7	R5
      0x7C180200,  //  0025  CALL	R6	1
      0xA8020005,  //  0026  EXBLK	0	#002D
      0x5C1C0C00,  //  0027  MOVE	R7	R6
      0x7C1C0000,  //  0028  CALL	R7	0
      0x8C20010B,  //  0029  GETMET	R8	R0	K11
      0x5C280E00,  //  002A  MOVE	R10	R7
      0x7C200400,  //  002B  CALL	R8	2
      0x7001FFF9,  //  002C  JMP		#0027
      0x58180010,  //  002D  LDCONST	R6	K16
      0xAC180200,  //  002E  CATCH	R6	1	0
      0xB0080000,  //  002F  RAISE	2	R0	R0
      0x8C18010B,  //  0030  GETMET	R6	R0	K11
      0x58200011,  //  0031  LDCONST	R8	K17
      0x7C180400,  //  0032  CALL	R6	2
      0x80000000,  //  0033  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: expect_colon
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_expect_colon,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(COLON),
    /* K5   */  be_nested_str_weak(next),
    /* K6   */  be_nested_str_weak(error),
    /* K7   */  be_nested_str_weak(Expected_X20_X27_X3A_X27),
    }),
    be_str_weak(expect_colon),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0008,  //  0004  JMPF	R2	#000E
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x780A0002,  //  000A  JMPF	R2	#000E
      0x8C080105,  //  000B  GETMET	R2	R0	K5
      0x7C080200,  //  000C  CALL	R2	1
      0x70020002,  //  000D  JMP		#0011
      0x8C080106,  //  000E  GETMET	R2	R0	K6
      0x58100007,  //  000F  LDCONST	R4	K7
      0x7C080400,  //  0010  CALL	R2	2
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_palette_color
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_palette_color,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(error),
    /* K2   */  be_nested_str_weak(Expected_X20color_X20value_X20in_X20palette),
    /* K3   */  be_nested_str_weak(0xFFFFFFFF),
    /* K4   */  be_nested_str_weak(type),
    /* K5   */  be_nested_str_weak(animation_dsl),
    /* K6   */  be_nested_str_weak(Token),
    /* K7   */  be_nested_str_weak(COLOR),
    /* K8   */  be_nested_str_weak(next),
    /* K9   */  be_nested_str_weak(convert_color),
    /* K10  */  be_nested_str_weak(value),
    /* K11  */  be_nested_str_weak(IDENTIFIER),
    /* K12  */  be_nested_str_weak(is_color_name),
    /* K13  */  be_nested_str_weak(get_named_color_value),
    /* K14  */  be_nested_str_weak(Unknown_X20color_X20_X27_X25s_X27_X2E_X20Palettes_X20only_X20accept_X20hex_X20colors_X20_X280xRRGGBB_X29_X20or_X20predefined_X20color_X20names_X20_X28like_X20_X27red_X27_X2C_X20_X27blue_X27_X2C_X20_X27green_X27_X29_X2C_X20but_X20not_X20custom_X20colors_X20defined_X20previously_X2E_X20For_X20dynamic_X20palettes_X20with_X20custom_X20colors_X2C_X20use_X20user_X20functions_X20instead_X2E),
    /* K15  */  be_nested_str_weak(Expected_X20color_X20value_X20in_X20palette_X2E_X20Use_X20hex_X20colors_X20_X280xRRGGBB_X29_X20or_X20predefined_X20color_X20names_X20_X28like_X20_X27red_X27_X2C_X20_X27blue_X27_X2C_X20_X27green_X27_X29_X2E),
    }),
    be_str_weak(process_palette_color),
    &be_const_str_solidified,
    ( &(const binstruction[50]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x1C080202,  //  0003  EQ	R2	R1	R2
      0x780A0003,  //  0004  JMPF	R2	#0009
      0x8C080101,  //  0005  GETMET	R2	R0	K1
      0x58100002,  //  0006  LDCONST	R4	K2
      0x7C080400,  //  0007  CALL	R2	2
      0x80060600,  //  0008  RET	1	K3
      0x88080304,  //  0009  GETMBR	R2	R1	K4
      0xB80E0A00,  //  000A  GETNGBL	R3	K5
      0x880C0706,  //  000B  GETMBR	R3	R3	K6
      0x880C0707,  //  000C  GETMBR	R3	R3	K7
      0x1C080403,  //  000D  EQ	R2	R2	R3
      0x780A0005,  //  000E  JMPF	R2	#0015
      0x8C080108,  //  000F  GETMET	R2	R0	K8
      0x7C080200,  //  0010  CALL	R2	1
      0x8C080109,  //  0011  GETMET	R2	R0	K9
      0x8810030A,  //  0012  GETMBR	R4	R1	K10
      0x7C080400,  //  0013  CALL	R2	2
      0x80040400,  //  0014  RET	1	R2
      0x88080304,  //  0015  GETMBR	R2	R1	K4
      0xB80E0A00,  //  0016  GETNGBL	R3	K5
      0x880C0706,  //  0017  GETMBR	R3	R3	K6
      0x880C070B,  //  0018  GETMBR	R3	R3	K11
      0x1C080403,  //  0019  EQ	R2	R2	R3
      0x780A0012,  //  001A  JMPF	R2	#002E
      0x8808030A,  //  001B  GETMBR	R2	R1	K10
      0x8C0C0108,  //  001C  GETMET	R3	R0	K8
      0x7C0C0200,  //  001D  CALL	R3	1
      0xB80E0A00,  //  001E  GETNGBL	R3	K5
      0x8C0C070C,  //  001F  GETMET	R3	R3	K12
      0x5C140400,  //  0020  MOVE	R5	R2
      0x7C0C0400,  //  0021  CALL	R3	2
      0x780E0003,  //  0022  JMPF	R3	#0027
      0x8C0C010D,  //  0023  GETMET	R3	R0	K13
      0x5C140400,  //  0024  MOVE	R5	R2
      0x7C0C0400,  //  0025  CALL	R3	2
      0x80040600,  //  0026  RET	1	R3
      0x8C0C0101,  //  0027  GETMET	R3	R0	K1
      0x60140018,  //  0028  GETGBL	R5	G24
      0x5818000E,  //  0029  LDCONST	R6	K14
      0x5C1C0400,  //  002A  MOVE	R7	R2
      0x7C140400,  //  002B  CALL	R5	2
      0x7C0C0400,  //  002C  CALL	R3	2
      0x80060600,  //  002D  RET	1	K3
      0x8C080101,  //  002E  GETMET	R2	R0	K1
      0x5810000F,  //  002F  LDCONST	R4	K15
      0x7C080400,  //  0030  CALL	R2	2
      0x80060600,  //  0031  RET	1	K3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: transpile_template_body
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_transpile_template_body,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(at_end),
    /* K1   */  be_nested_str_weak(process_statement),
    /* K2   */  be_nested_str_weak(run_statements),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(name),
    /* K5   */  be_nested_str_weak(comment),
    /* K6   */  be_nested_str_weak(add),
    /* K7   */  be_nested_str_weak(engine_X2Eadd_X28_X25s__X29_X25s),
    /* K8   */  be_nested_str_weak(stop_iteration),
    /* K9   */  be_nested_str_weak(errors),
    /* K10  */  be_nested_str_weak(join_output),
    /* K11  */  be_nested_str_weak(error),
    /* K12  */  be_nested_str_weak(Template_X20body_X20transpilation_X20failed_X3A_X20_X25s),
    }),
    be_str_weak(transpile_template_body),
    &be_const_str_solidified,
    ( &(const binstruction[57]) {  /* code */
      0xA802002A,  //  0000  EXBLK	0	#002C
      0x8C040100,  //  0001  GETMET	R1	R0	K0
      0x7C040200,  //  0002  CALL	R1	1
      0x74060002,  //  0003  JMPT	R1	#0007
      0x8C040101,  //  0004  GETMET	R1	R0	K1
      0x7C040200,  //  0005  CALL	R1	1
      0x7001FFF9,  //  0006  JMP		#0001
      0x6004000C,  //  0007  GETGBL	R1	G12
      0x88080102,  //  0008  GETMBR	R2	R0	K2
      0x7C040200,  //  0009  CALL	R1	1
      0x24040303,  //  000A  GT	R1	R1	K3
      0x78060012,  //  000B  JMPF	R1	#001F
      0x60040010,  //  000C  GETGBL	R1	G16
      0x88080102,  //  000D  GETMBR	R2	R0	K2
      0x7C040200,  //  000E  CALL	R1	1
      0xA802000B,  //  000F  EXBLK	0	#001C
      0x5C080200,  //  0010  MOVE	R2	R1
      0x7C080000,  //  0011  CALL	R2	0
      0x940C0504,  //  0012  GETIDX	R3	R2	K4
      0x94100505,  //  0013  GETIDX	R4	R2	K5
      0x8C140106,  //  0014  GETMET	R5	R0	K6
      0x601C0018,  //  0015  GETGBL	R7	G24
      0x58200007,  //  0016  LDCONST	R8	K7
      0x5C240600,  //  0017  MOVE	R9	R3
      0x5C280800,  //  0018  MOVE	R10	R4
      0x7C1C0600,  //  0019  CALL	R7	3
      0x7C140400,  //  001A  CALL	R5	2
      0x7001FFF3,  //  001B  JMP		#0010
      0x58040008,  //  001C  LDCONST	R1	K8
      0xAC040200,  //  001D  CATCH	R1	1	0
      0xB0080000,  //  001E  RAISE	2	R0	R0
      0x6004000C,  //  001F  GETGBL	R1	G12
      0x88080109,  //  0020  GETMBR	R2	R0	K9
      0x7C040200,  //  0021  CALL	R1	1
      0x1C040303,  //  0022  EQ	R1	R1	K3
      0x78060002,  //  0023  JMPF	R1	#0027
      0x8C04010A,  //  0024  GETMET	R1	R0	K10
      0x7C040200,  //  0025  CALL	R1	1
      0x70020000,  //  0026  JMP		#0028
      0x4C040000,  //  0027  LDNIL	R1
      0xA8040001,  //  0028  EXBLK	1	1
      0x80040200,  //  0029  RET	1	R1
      0xA8040001,  //  002A  EXBLK	1	1
      0x7002000B,  //  002B  JMP		#0038
      0xAC040002,  //  002C  CATCH	R1	0	2
      0x70020008,  //  002D  JMP		#0037
      0x8C0C010B,  //  002E  GETMET	R3	R0	K11
      0x60140018,  //  002F  GETGBL	R5	G24
      0x5818000C,  //  0030  LDCONST	R6	K12
      0x5C1C0400,  //  0031  MOVE	R7	R2
      0x7C140400,  //  0032  CALL	R5	2
      0x7C0C0400,  //  0033  CALL	R3	2
      0x4C0C0000,  //  0034  LDNIL	R3
      0x80040600,  //  0035  RET	1	R3
      0x70020000,  //  0036  JMP		#0038
      0xB0080000,  //  0037  RAISE	2	R0	R0
      0x80000000,  //  0038  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: generate_engine_run
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_generate_engine_run,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(run_statements),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_weak(has_template_calls),
    /* K3   */  be_nested_str_weak(name),
    /* K4   */  be_nested_str_weak(comment),
    /* K5   */  be_nested_str_weak(add),
    /* K6   */  be_nested_str_weak(engine_X2Eadd_X28_X25s__X29_X25s),
    /* K7   */  be_nested_str_weak(stop_iteration),
    /* K8   */  be_nested_str_weak(engine_X2Erun_X28_X29),
    }),
    be_str_weak(generate_engine_run),
    &be_const_str_solidified,
    ( &(const binstruction[31]) {  /* code */
      0x6004000C,  //  0000  GETGBL	R1	G12
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x7C040200,  //  0002  CALL	R1	1
      0x1C040301,  //  0003  EQ	R1	R1	K1
      0x78060002,  //  0004  JMPF	R1	#0008
      0x88040102,  //  0005  GETMBR	R1	R0	K2
      0x74060000,  //  0006  JMPT	R1	#0008
      0x80000200,  //  0007  RET	0
      0x60040010,  //  0008  GETGBL	R1	G16
      0x88080100,  //  0009  GETMBR	R2	R0	K0
      0x7C040200,  //  000A  CALL	R1	1
      0xA802000B,  //  000B  EXBLK	0	#0018
      0x5C080200,  //  000C  MOVE	R2	R1
      0x7C080000,  //  000D  CALL	R2	0
      0x940C0503,  //  000E  GETIDX	R3	R2	K3
      0x94100504,  //  000F  GETIDX	R4	R2	K4
      0x8C140105,  //  0010  GETMET	R5	R0	K5
      0x601C0018,  //  0011  GETGBL	R7	G24
      0x58200006,  //  0012  LDCONST	R8	K6
      0x5C240600,  //  0013  MOVE	R9	R3
      0x5C280800,  //  0014  MOVE	R10	R4
      0x7C1C0600,  //  0015  CALL	R7	3
      0x7C140400,  //  0016  CALL	R5	2
      0x7001FFF3,  //  0017  JMP		#000C
      0x58040007,  //  0018  LDCONST	R1	K7
      0xAC040200,  //  0019  CATCH	R1	1	0
      0xB0080000,  //  001A  RAISE	2	R0	R0
      0x8C040105,  //  001B  GETMET	R1	R0	K5
      0x580C0008,  //  001C  LDCONST	R3	K8
      0x7C040400,  //  001D  CALL	R1	2
      0x80000000,  //  001E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _has_function_parameters
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__has_function_parameters,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(pos),
    /* K1   */  be_nested_str_weak(current),
    /* K2   */  be_nested_str_weak(type),
    /* K3   */  be_nested_str_weak(animation_dsl),
    /* K4   */  be_nested_str_weak(Token),
    /* K5   */  be_nested_str_weak(LEFT_PAREN),
    /* K6   */  be_nested_str_weak(next),
    /* K7   */  be_nested_str_weak(skip_whitespace_including_newlines),
    /* K8   */  be_nested_str_weak(RIGHT_PAREN),
    }),
    be_str_weak(_has_function_parameters),
    &be_const_str_solidified,
    ( &(const binstruction[40]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C080101,  //  0001  GETMET	R2	R0	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x4C0C0000,  //  0003  LDNIL	R3
      0x1C080403,  //  0004  EQ	R2	R2	R3
      0x740A0007,  //  0005  JMPT	R2	#000E
      0x8C080101,  //  0006  GETMET	R2	R0	K1
      0x7C080200,  //  0007  CALL	R2	1
      0x88080502,  //  0008  GETMBR	R2	R2	K2
      0xB80E0600,  //  0009  GETNGBL	R3	K3
      0x880C0704,  //  000A  GETMBR	R3	R3	K4
      0x880C0705,  //  000B  GETMBR	R3	R3	K5
      0x20080403,  //  000C  NE	R2	R2	R3
      0x780A0001,  //  000D  JMPF	R2	#0010
      0x50080000,  //  000E  LDBOOL	R2	0	0
      0x80040400,  //  000F  RET	1	R2
      0x8C080106,  //  0010  GETMET	R2	R0	K6
      0x7C080200,  //  0011  CALL	R2	1
      0x8C080107,  //  0012  GETMET	R2	R0	K7
      0x7C080200,  //  0013  CALL	R2	1
      0x8C080101,  //  0014  GETMET	R2	R0	K1
      0x7C080200,  //  0015  CALL	R2	1
      0x4C0C0000,  //  0016  LDNIL	R3
      0x20080403,  //  0017  NE	R2	R2	R3
      0x780A0007,  //  0018  JMPF	R2	#0021
      0x8C080101,  //  0019  GETMET	R2	R0	K1
      0x7C080200,  //  001A  CALL	R2	1
      0x88080502,  //  001B  GETMBR	R2	R2	K2
      0xB80E0600,  //  001C  GETNGBL	R3	K3
      0x880C0704,  //  001D  GETMBR	R3	R3	K4
      0x880C0708,  //  001E  GETMBR	R3	R3	K8
      0x1C080403,  //  001F  EQ	R2	R2	R3
      0x740A0000,  //  0020  JMPT	R2	#0022
      0x50080001,  //  0021  LDBOOL	R2	0	1
      0x50080200,  //  0022  LDBOOL	R2	1	0
      0x780A0000,  //  0023  JMPF	R2	#0025
      0x50080001,  //  0024  LDBOOL	R2	0	1
      0x50080200,  //  0025  LDBOOL	R2	1	0
      0x90020001,  //  0026  SETMBR	R0	K0	R1
      0x80040400,  //  0027  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_indent
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_get_indent,   /* name */
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
    /* K0   */  be_nested_str_weak(_X20_X20),
    /* K1   */  be_nested_str_weak(indent_level),
    /* K2   */  be_const_int(1),
    }),
    be_str_weak(get_indent),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040101,  //  0000  GETMBR	R1	R0	K1
      0x00040302,  //  0001  ADD	R1	R1	K2
      0x08060001,  //  0002  MUL	R1	K0	R1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _validate_single_parameter
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__validate_single_parameter,   /* name */
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
    /* K0   */  be_nested_str_weak(introspect),
    /* K1   */  be_nested_str_weak(contains),
    /* K2   */  be_nested_str_weak(_has_param),
    /* K3   */  be_nested_str_weak(current),
    /* K4   */  be_nested_str_weak(line),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(error),
    /* K7   */  be_nested_str_weak(Animation_X20_X27_X25s_X27_X20does_X20not_X20have_X20parameter_X20_X27_X25s_X27_X2E_X20Check_X20the_X20animation_X20documentation_X20for_X20valid_X20parameters_X2E),
    }),
    be_str_weak(_validate_single_parameter),
    &be_const_str_solidified,
    ( &(const binstruction[38]) {  /* code */
      0xA802001F,  //  0000  EXBLK	0	#0021
      0xA4120000,  //  0001  IMPORT	R4	K0
      0x4C140000,  //  0002  LDNIL	R5
      0x20140605,  //  0003  NE	R5	R3	R5
      0x78160019,  //  0004  JMPF	R5	#001F
      0x8C140901,  //  0005  GETMET	R5	R4	K1
      0x5C1C0600,  //  0006  MOVE	R7	R3
      0x58200002,  //  0007  LDCONST	R8	K2
      0x7C140600,  //  0008  CALL	R5	3
      0x78160014,  //  0009  JMPF	R5	#001F
      0x8C140702,  //  000A  GETMET	R5	R3	K2
      0x5C1C0400,  //  000B  MOVE	R7	R2
      0x7C140400,  //  000C  CALL	R5	2
      0x74160010,  //  000D  JMPT	R5	#001F
      0x8C140103,  //  000E  GETMET	R5	R0	K3
      0x7C140200,  //  000F  CALL	R5	1
      0x4C180000,  //  0010  LDNIL	R6
      0x20140A06,  //  0011  NE	R5	R5	R6
      0x78160003,  //  0012  JMPF	R5	#0017
      0x8C140103,  //  0013  GETMET	R5	R0	K3
      0x7C140200,  //  0014  CALL	R5	1
      0x88140B04,  //  0015  GETMBR	R5	R5	K4
      0x70020000,  //  0016  JMP		#0018
      0x58140005,  //  0017  LDCONST	R5	K5
      0x8C180106,  //  0018  GETMET	R6	R0	K6
      0x60200018,  //  0019  GETGBL	R8	G24
      0x58240007,  //  001A  LDCONST	R9	K7
      0x5C280200,  //  001B  MOVE	R10	R1
      0x5C2C0400,  //  001C  MOVE	R11	R2
      0x7C200600,  //  001D  CALL	R8	3
      0x7C180400,  //  001E  CALL	R6	2
      0xA8040001,  //  001F  EXBLK	1	1
      0x70020003,  //  0020  JMP		#0025
      0xAC100002,  //  0021  CATCH	R4	0	2
      0x70020000,  //  0022  JMP		#0024
      0x70020000,  //  0023  JMP		#0025
      0xB0080000,  //  0024  RAISE	2	R0	R0
      0x80000000,  //  0025  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_sequence_assignment
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_sequence_assignment,   /* name */
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
    /* K0   */  be_nested_str_weak(process_sequence_assignment_generic),
    /* K1   */  be_nested_str_weak(steps),
    }),
    be_str_weak(process_sequence_assignment),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0x58140001,  //  0002  LDCONST	R5	K1
      0x7C080600,  //  0003  CALL	R2	3
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: validate_symbol_reference
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_validate_symbol_reference,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(symbol_table),
    /* K1   */  be_nested_str_weak(symbol_exists),
    /* K2   */  be_nested_str_weak(error),
    /* K3   */  be_nested_str_weak(Undefined_X20reference_X20_X27_X25s_X27_X20in_X20_X25s_X2E_X20Make_X20sure_X20the_X20object_X20is_X20defined_X20before_X20use_X2E),
    }),
    be_str_weak(validate_symbol_reference),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x740E0008,  //  0004  JMPT	R3	#000E
      0x8C0C0102,  //  0005  GETMET	R3	R0	K2
      0x60140018,  //  0006  GETGBL	R5	G24
      0x58180003,  //  0007  LDCONST	R6	K3
      0x5C1C0200,  //  0008  MOVE	R7	R1
      0x5C200400,  //  0009  MOVE	R8	R2
      0x7C140600,  //  000A  CALL	R5	3
      0x7C0C0400,  //  000B  CALL	R3	2
      0x500C0000,  //  000C  LDBOOL	R3	0	0
      0x80040600,  //  000D  RET	1	R3
      0x500C0200,  //  000E  LDBOOL	R3	1	0
      0x80040600,  //  000F  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: current
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_current,   /* name */
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
    /* K0   */  be_nested_str_weak(pos),
    /* K1   */  be_nested_str_weak(tokens),
    }),
    be_str_weak(current),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x6008000C,  //  0001  GETGBL	R2	G12
      0x880C0101,  //  0002  GETMBR	R3	R0	K1
      0x7C080200,  //  0003  CALL	R2	1
      0x14040202,  //  0004  LT	R1	R1	R2
      0x78060003,  //  0005  JMPF	R1	#000A
      0x88040101,  //  0006  GETMBR	R1	R0	K1
      0x88080100,  //  0007  GETMBR	R2	R0	K0
      0x94040202,  //  0008  GETIDX	R1	R1	R2
      0x70020000,  //  0009  JMP		#000B
      0x4C040000,  //  000A  LDNIL	R1
      0x80040200,  //  000B  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_function_call
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_function_call,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[28]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(),
    /* K2   */  be_nested_str_weak(type),
    /* K3   */  be_nested_str_weak(animation_dsl),
    /* K4   */  be_nested_str_weak(Token),
    /* K5   */  be_nested_str_weak(IDENTIFIER),
    /* K6   */  be_nested_str_weak(KEYWORD),
    /* K7   */  be_nested_str_weak(value),
    /* K8   */  be_nested_str_weak(next),
    /* K9   */  be_nested_str_weak(error),
    /* K10  */  be_nested_str_weak(Expected_X20function_X20name),
    /* K11  */  be_nested_str_weak(nil),
    /* K12  */  be_nested_str_weak(symbol_table),
    /* K13  */  be_nested_str_weak(get),
    /* K14  */  be_nested_str_weak(_symbol_entry),
    /* K15  */  be_nested_str_weak(TYPE_MATH_FUNCTION),
    /* K16  */  be_nested_str_weak(process_function_arguments),
    /* K17  */  be_nested_str_weak(_X25s_X28_X25s_X29),
    /* K18  */  be_nested_str_weak(get_reference),
    /* K19  */  be_nested_str_weak(log),
    /* K20  */  be_nested_str_weak(process_log_call),
    /* K21  */  be_nested_str_weak(CONTEXT_EXPRESSION),
    /* K22  */  be_nested_str_weak(TYPE_TEMPLATE),
    /* K23  */  be_nested_str_weak(engine_X2C_X20_X25s),
    /* K24  */  be_nested_str_weak(engine),
    /* K25  */  be_nested_str_weak(_X25s_template_X28_X25s_X29),
    /* K26  */  be_nested_str_weak(animation_X2E_X25s_X28engine_X2C_X20_X25s_X29),
    /* K27  */  be_nested_str_weak(animation_X2E_X25s_X28engine_X29),
    }),
    be_str_weak(process_function_call),
    &be_const_str_solidified,
    ( &(const binstruction[102]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x7C080200,  //  0001  CALL	R2	1
      0x580C0001,  //  0002  LDCONST	R3	K1
      0x4C100000,  //  0003  LDNIL	R4
      0x20100404,  //  0004  NE	R4	R2	R4
      0x7812000F,  //  0005  JMPF	R4	#0016
      0x88100502,  //  0006  GETMBR	R4	R2	K2
      0xB8160600,  //  0007  GETNGBL	R5	K3
      0x88140B04,  //  0008  GETMBR	R5	R5	K4
      0x88140B05,  //  0009  GETMBR	R5	R5	K5
      0x1C100805,  //  000A  EQ	R4	R4	R5
      0x74120005,  //  000B  JMPT	R4	#0012
      0x88100502,  //  000C  GETMBR	R4	R2	K2
      0xB8160600,  //  000D  GETNGBL	R5	K3
      0x88140B04,  //  000E  GETMBR	R5	R5	K4
      0x88140B06,  //  000F  GETMBR	R5	R5	K6
      0x1C100805,  //  0010  EQ	R4	R4	R5
      0x78120003,  //  0011  JMPF	R4	#0016
      0x880C0507,  //  0012  GETMBR	R3	R2	K7
      0x8C100108,  //  0013  GETMET	R4	R0	K8
      0x7C100200,  //  0014  CALL	R4	1
      0x70020003,  //  0015  JMP		#001A
      0x8C100109,  //  0016  GETMET	R4	R0	K9
      0x5818000A,  //  0017  LDCONST	R6	K10
      0x7C100400,  //  0018  CALL	R4	2
      0x80061600,  //  0019  RET	1	K11
      0x8810010C,  //  001A  GETMBR	R4	R0	K12
      0x8C10090D,  //  001B  GETMET	R4	R4	K13
      0x5C180600,  //  001C  MOVE	R6	R3
      0x7C100400,  //  001D  CALL	R4	2
      0x4C140000,  //  001E  LDNIL	R5
      0x20140805,  //  001F  NE	R5	R4	R5
      0x7816000F,  //  0020  JMPF	R5	#0031
      0x88140902,  //  0021  GETMBR	R5	R4	K2
      0xB81A0600,  //  0022  GETNGBL	R6	K3
      0x88180D0E,  //  0023  GETMBR	R6	R6	K14
      0x88180D0F,  //  0024  GETMBR	R6	R6	K15
      0x1C140A06,  //  0025  EQ	R5	R5	R6
      0x78160009,  //  0026  JMPF	R5	#0031
      0x8C140110,  //  0027  GETMET	R5	R0	K16
      0x501C0000,  //  0028  LDBOOL	R7	0	0
      0x7C140400,  //  0029  CALL	R5	2
      0x60180018,  //  002A  GETGBL	R6	G24
      0x581C0011,  //  002B  LDCONST	R7	K17
      0x8C200912,  //  002C  GETMET	R8	R4	K18
      0x7C200200,  //  002D  CALL	R8	1
      0x5C240A00,  //  002E  MOVE	R9	R5
      0x7C180600,  //  002F  CALL	R6	3
      0x80040C00,  //  0030  RET	1	R6
      0x1C140713,  //  0031  EQ	R5	R3	K19
      0x78160008,  //  0032  JMPF	R5	#003C
      0x8C140110,  //  0033  GETMET	R5	R0	K16
      0x501C0000,  //  0034  LDBOOL	R7	0	0
      0x7C140400,  //  0035  CALL	R5	2
      0x8C180114,  //  0036  GETMET	R6	R0	K20
      0x5C200A00,  //  0037  MOVE	R8	R5
      0x88240115,  //  0038  GETMBR	R9	R0	K21
      0x58280001,  //  0039  LDCONST	R10	K1
      0x7C180800,  //  003A  CALL	R6	4
      0x80040C00,  //  003B  RET	1	R6
      0x8C140110,  //  003C  GETMET	R5	R0	K16
      0x501C0000,  //  003D  LDBOOL	R7	0	0
      0x7C140400,  //  003E  CALL	R5	2
      0x4C180000,  //  003F  LDNIL	R6
      0x20180806,  //  0040  NE	R6	R4	R6
      0x781A0014,  //  0041  JMPF	R6	#0057
      0x88180902,  //  0042  GETMBR	R6	R4	K2
      0xB81E0600,  //  0043  GETNGBL	R7	K3
      0x881C0F0E,  //  0044  GETMBR	R7	R7	K14
      0x881C0F16,  //  0045  GETMBR	R7	R7	K22
      0x1C180C07,  //  0046  EQ	R6	R6	R7
      0x781A000E,  //  0047  JMPF	R6	#0057
      0x20180B01,  //  0048  NE	R6	R5	K1
      0x781A0004,  //  0049  JMPF	R6	#004F
      0x60180018,  //  004A  GETGBL	R6	G24
      0x581C0017,  //  004B  LDCONST	R7	K23
      0x5C200A00,  //  004C  MOVE	R8	R5
      0x7C180400,  //  004D  CALL	R6	2
      0x70020000,  //  004E  JMP		#0050
      0x58180018,  //  004F  LDCONST	R6	K24
      0x601C0018,  //  0050  GETGBL	R7	G24
      0x58200019,  //  0051  LDCONST	R8	K25
      0x5C240600,  //  0052  MOVE	R9	R3
      0x5C280C00,  //  0053  MOVE	R10	R6
      0x7C1C0600,  //  0054  CALL	R7	3
      0x80040E00,  //  0055  RET	1	R7
      0x7002000D,  //  0056  JMP		#0065
      0x20180B01,  //  0057  NE	R6	R5	K1
      0x781A0006,  //  0058  JMPF	R6	#0060
      0x60180018,  //  0059  GETGBL	R6	G24
      0x581C001A,  //  005A  LDCONST	R7	K26
      0x5C200600,  //  005B  MOVE	R8	R3
      0x5C240A00,  //  005C  MOVE	R9	R5
      0x7C180600,  //  005D  CALL	R6	3
      0x80040C00,  //  005E  RET	1	R6
      0x70020004,  //  005F  JMP		#0065
      0x60180018,  //  0060  GETGBL	R6	G24
      0x581C001B,  //  0061  LDCONST	R7	K27
      0x5C200600,  //  0062  MOVE	R8	R3
      0x7C180400,  //  0063  CALL	R6	2
      0x80040C00,  //  0064  RET	1	R6
      0x80000000,  //  0065  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_play_statement_fluent
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_play_statement_fluent,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[23]) {     /* constants */
    /* K0   */  be_nested_str_weak(next),
    /* K1   */  be_nested_str_weak(),
    /* K2   */  be_nested_str_weak(current),
    /* K3   */  be_nested_str_weak(type),
    /* K4   */  be_nested_str_weak(animation_dsl),
    /* K5   */  be_nested_str_weak(Token),
    /* K6   */  be_nested_str_weak(IDENTIFIER),
    /* K7   */  be_nested_str_weak(KEYWORD),
    /* K8   */  be_nested_str_weak(peek),
    /* K9   */  be_nested_str_weak(LEFT_PAREN),
    /* K10  */  be_nested_str_weak(process_nested_function_call),
    /* K11  */  be_nested_str_weak(expect_identifier),
    /* K12  */  be_nested_str_weak(_validate_object_reference),
    /* K13  */  be_nested_str_weak(sequence_X20play),
    /* K14  */  be_nested_str_weak(_X25s_),
    /* K15  */  be_nested_str_weak(nil),
    /* K16  */  be_nested_str_weak(value),
    /* K17  */  be_nested_str_weak(for),
    /* K18  */  be_nested_str_weak(process_time_value),
    /* K19  */  be_nested_str_weak(collect_inline_comment),
    /* K20  */  be_nested_str_weak(add),
    /* K21  */  be_nested_str_weak(_X25s_X2Epush_play_step_X28_X25s_X2C_X20_X25s_X29_X25s),
    /* K22  */  be_nested_str_weak(get_indent),
    }),
    be_str_weak(process_play_statement_fluent),
    &be_const_str_solidified,
    ( &(const binstruction[85]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x58040001,  //  0002  LDCONST	R1	K1
      0x8C080102,  //  0003  GETMET	R2	R0	K2
      0x7C080200,  //  0004  CALL	R2	1
      0x4C0C0000,  //  0005  LDNIL	R3
      0x200C0403,  //  0006  NE	R3	R2	R3
      0x780E001C,  //  0007  JMPF	R3	#0025
      0x880C0503,  //  0008  GETMBR	R3	R2	K3
      0xB8120800,  //  0009  GETNGBL	R4	K4
      0x88100905,  //  000A  GETMBR	R4	R4	K5
      0x88100906,  //  000B  GETMBR	R4	R4	K6
      0x1C0C0604,  //  000C  EQ	R3	R3	R4
      0x740E0005,  //  000D  JMPT	R3	#0014
      0x880C0503,  //  000E  GETMBR	R3	R2	K3
      0xB8120800,  //  000F  GETNGBL	R4	K4
      0x88100905,  //  0010  GETMBR	R4	R4	K5
      0x88100907,  //  0011  GETMBR	R4	R4	K7
      0x1C0C0604,  //  0012  EQ	R3	R3	R4
      0x780E0010,  //  0013  JMPF	R3	#0025
      0x8C0C0108,  //  0014  GETMET	R3	R0	K8
      0x7C0C0200,  //  0015  CALL	R3	1
      0x4C100000,  //  0016  LDNIL	R4
      0x200C0604,  //  0017  NE	R3	R3	R4
      0x780E000B,  //  0018  JMPF	R3	#0025
      0x8C0C0108,  //  0019  GETMET	R3	R0	K8
      0x7C0C0200,  //  001A  CALL	R3	1
      0x880C0703,  //  001B  GETMBR	R3	R3	K3
      0xB8120800,  //  001C  GETNGBL	R4	K4
      0x88100905,  //  001D  GETMBR	R4	R4	K5
      0x88100909,  //  001E  GETMBR	R4	R4	K9
      0x1C0C0604,  //  001F  EQ	R3	R3	R4
      0x780E0003,  //  0020  JMPF	R3	#0025
      0x8C0C010A,  //  0021  GETMET	R3	R0	K10
      0x7C0C0200,  //  0022  CALL	R3	1
      0x5C040600,  //  0023  MOVE	R1	R3
      0x7002000A,  //  0024  JMP		#0030
      0x8C0C010B,  //  0025  GETMET	R3	R0	K11
      0x7C0C0200,  //  0026  CALL	R3	1
      0x8C10010C,  //  0027  GETMET	R4	R0	K12
      0x5C180600,  //  0028  MOVE	R6	R3
      0x581C000D,  //  0029  LDCONST	R7	K13
      0x7C100600,  //  002A  CALL	R4	3
      0x60100018,  //  002B  GETGBL	R4	G24
      0x5814000E,  //  002C  LDCONST	R5	K14
      0x5C180600,  //  002D  MOVE	R6	R3
      0x7C100400,  //  002E  CALL	R4	2
      0x5C040800,  //  002F  MOVE	R1	R4
      0x580C000F,  //  0030  LDCONST	R3	K15
      0x8C100102,  //  0031  GETMET	R4	R0	K2
      0x7C100200,  //  0032  CALL	R4	1
      0x4C140000,  //  0033  LDNIL	R5
      0x20100805,  //  0034  NE	R4	R4	R5
      0x78120011,  //  0035  JMPF	R4	#0048
      0x8C100102,  //  0036  GETMET	R4	R0	K2
      0x7C100200,  //  0037  CALL	R4	1
      0x88100903,  //  0038  GETMBR	R4	R4	K3
      0xB8160800,  //  0039  GETNGBL	R5	K4
      0x88140B05,  //  003A  GETMBR	R5	R5	K5
      0x88140B07,  //  003B  GETMBR	R5	R5	K7
      0x1C100805,  //  003C  EQ	R4	R4	R5
      0x78120009,  //  003D  JMPF	R4	#0048
      0x8C100102,  //  003E  GETMET	R4	R0	K2
      0x7C100200,  //  003F  CALL	R4	1
      0x88100910,  //  0040  GETMBR	R4	R4	K16
      0x1C100911,  //  0041  EQ	R4	R4	K17
      0x78120004,  //  0042  JMPF	R4	#0048
      0x8C100100,  //  0043  GETMET	R4	R0	K0
      0x7C100200,  //  0044  CALL	R4	1
      0x8C100112,  //  0045  GETMET	R4	R0	K18
      0x7C100200,  //  0046  CALL	R4	1
      0x5C0C0800,  //  0047  MOVE	R3	R4
      0x8C100113,  //  0048  GETMET	R4	R0	K19
      0x7C100200,  //  0049  CALL	R4	1
      0x8C140114,  //  004A  GETMET	R5	R0	K20
      0x601C0018,  //  004B  GETGBL	R7	G24
      0x58200015,  //  004C  LDCONST	R8	K21
      0x8C240116,  //  004D  GETMET	R9	R0	K22
      0x7C240200,  //  004E  CALL	R9	1
      0x5C280200,  //  004F  MOVE	R10	R1
      0x5C2C0600,  //  0050  MOVE	R11	R3
      0x5C300800,  //  0051  MOVE	R12	R4
      0x7C1C0A00,  //  0052  CALL	R7	5
      0x7C140400,  //  0053  CALL	R5	2
      0x80000000,  //  0054  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _process_named_arguments_for_animation
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__process_named_arguments_for_animation,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_process_named_arguments_unified),
    /* K1   */  be_nested_str_weak(CONTEXT_ANIMATION),
    }),
    be_str_weak(_process_named_arguments_for_animation),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C0C0100,  //  0000  GETMET	R3	R0	K0
      0x5C140200,  //  0001  MOVE	R5	R1
      0x5C180400,  //  0002  MOVE	R6	R2
      0x881C0101,  //  0003  GETMBR	R7	R0	K1
      0x7C0C0800,  //  0004  CALL	R3	4
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: peek
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_peek,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(pos),
    /* K1   */  be_const_int(1),
    /* K2   */  be_nested_str_weak(tokens),
    }),
    be_str_weak(peek),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x00040301,  //  0001  ADD	R1	R1	K1
      0x6008000C,  //  0002  GETGBL	R2	G12
      0x880C0102,  //  0003  GETMBR	R3	R0	K2
      0x7C080200,  //  0004  CALL	R2	1
      0x14040202,  //  0005  LT	R1	R1	R2
      0x78060004,  //  0006  JMPF	R1	#000C
      0x88040100,  //  0007  GETMBR	R1	R0	K0
      0x00040301,  //  0008  ADD	R1	R1	K1
      0x88080102,  //  0009  GETMBR	R2	R0	K2
      0x94040401,  //  000A  GETIDX	R1	R2	R1
      0x70020000,  //  000B  JMP		#000D
      0x4C040000,  //  000C  LDNIL	R1
      0x80040200,  //  000D  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: at_end
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_at_end,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(pos),
    /* K1   */  be_nested_str_weak(tokens),
    /* K2   */  be_nested_str_weak(current),
    /* K3   */  be_nested_str_weak(type),
    /* K4   */  be_nested_str_weak(animation_dsl),
    /* K5   */  be_nested_str_weak(Token),
    /* K6   */  be_nested_str_weak(EOF),
    }),
    be_str_weak(at_end),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x6008000C,  //  0001  GETGBL	R2	G12
      0x880C0101,  //  0002  GETMBR	R3	R0	K1
      0x7C080200,  //  0003  CALL	R2	1
      0x28040202,  //  0004  GE	R1	R1	R2
      0x7406000D,  //  0005  JMPT	R1	#0014
      0x8C040102,  //  0006  GETMET	R1	R0	K2
      0x7C040200,  //  0007  CALL	R1	1
      0x4C080000,  //  0008  LDNIL	R2
      0x20040202,  //  0009  NE	R1	R1	R2
      0x78060007,  //  000A  JMPF	R1	#0013
      0x8C040102,  //  000B  GETMET	R1	R0	K2
      0x7C040200,  //  000C  CALL	R1	1
      0x88040303,  //  000D  GETMBR	R1	R1	K3
      0xB80A0800,  //  000E  GETNGBL	R2	K4
      0x88080505,  //  000F  GETMBR	R2	R2	K5
      0x88080506,  //  0010  GETMBR	R2	R2	K6
      0x1C040202,  //  0011  EQ	R1	R1	R2
      0x74060000,  //  0012  JMPT	R1	#0014
      0x50040001,  //  0013  LDBOOL	R1	0	1
      0x50040200,  //  0014  LDBOOL	R1	1	0
      0x80040200,  //  0015  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_identifier_char
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_is_identifier_char,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(a),
    /* K1   */  be_nested_str_weak(z),
    /* K2   */  be_nested_str_weak(A),
    /* K3   */  be_nested_str_weak(Z),
    /* K4   */  be_nested_str_weak(0),
    /* K5   */  be_nested_str_weak(9),
    /* K6   */  be_nested_str_weak(_),
    }),
    be_str_weak(is_identifier_char),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x28080300,  //  0000  GE	R2	R1	K0
      0x780A0001,  //  0001  JMPF	R2	#0004
      0x18080301,  //  0002  LE	R2	R1	K1
      0x740A000A,  //  0003  JMPT	R2	#000F
      0x28080302,  //  0004  GE	R2	R1	K2
      0x780A0001,  //  0005  JMPF	R2	#0008
      0x18080303,  //  0006  LE	R2	R1	K3
      0x740A0006,  //  0007  JMPT	R2	#000F
      0x28080304,  //  0008  GE	R2	R1	K4
      0x780A0001,  //  0009  JMPF	R2	#000C
      0x18080305,  //  000A  LE	R2	R1	K5
      0x740A0002,  //  000B  JMPT	R2	#000F
      0x1C080306,  //  000C  EQ	R2	R1	K6
      0x740A0000,  //  000D  JMPT	R2	#000F
      0x50080001,  //  000E  LDBOOL	R2	0	1
      0x50080200,  //  000F  LDBOOL	R2	1	0
      0x80040400,  //  0010  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: collect_inline_comment
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_collect_inline_comment,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(COMMENT),
    /* K5   */  be_nested_str_weak(_X20_X20),
    /* K6   */  be_nested_str_weak(value),
    /* K7   */  be_nested_str_weak(next),
    /* K8   */  be_nested_str_weak(),
    }),
    be_str_weak(collect_inline_comment),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A000A,  //  0004  JMPF	R2	#0010
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x780A0004,  //  000A  JMPF	R2	#0010
      0x88080306,  //  000B  GETMBR	R2	R1	K6
      0x000A0A02,  //  000C  ADD	R2	K5	R2
      0x8C0C0107,  //  000D  GETMET	R3	R0	K7
      0x7C0C0200,  //  000E  CALL	R3	1
      0x80040400,  //  000F  RET	1	R2
      0x80061000,  //  0010  RET	1	K8
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_template
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_template,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[27]) {     /* constants */
    /* K0   */  be_nested_str_weak(next),
    /* K1   */  be_nested_str_weak(expect_identifier),
    /* K2   */  be_nested_str_weak(validate_user_name),
    /* K3   */  be_nested_str_weak(template),
    /* K4   */  be_nested_str_weak(skip_statement),
    /* K5   */  be_nested_str_weak(expect_left_brace),
    /* K6   */  be_nested_str_weak(at_end),
    /* K7   */  be_nested_str_weak(check_right_brace),
    /* K8   */  be_nested_str_weak(skip_whitespace_including_newlines),
    /* K9   */  be_nested_str_weak(current),
    /* K10  */  be_nested_str_weak(type),
    /* K11  */  be_nested_str_weak(animation_dsl),
    /* K12  */  be_nested_str_weak(Token),
    /* K13  */  be_nested_str_weak(KEYWORD),
    /* K14  */  be_nested_str_weak(value),
    /* K15  */  be_nested_str_weak(param),
    /* K16  */  be_nested_str_weak(push),
    /* K17  */  be_nested_str_weak(NEWLINE),
    /* K18  */  be_const_int(0),
    /* K19  */  be_nested_str_weak(EOF),
    /* K20  */  be_nested_str_weak(LEFT_BRACE),
    /* K21  */  be_const_int(1),
    /* K22  */  be_nested_str_weak(RIGHT_BRACE),
    /* K23  */  be_nested_str_weak(expect_right_brace),
    /* K24  */  be_nested_str_weak(generate_template_function),
    /* K25  */  be_nested_str_weak(symbol_table),
    /* K26  */  be_nested_str_weak(create_template),
    }),
    be_str_weak(process_template),
    &be_const_str_solidified,
    ( &(const binstruction[161]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x7C040200,  //  0003  CALL	R1	1
      0x8C080102,  //  0004  GETMET	R2	R0	K2
      0x5C100200,  //  0005  MOVE	R4	R1
      0x58140003,  //  0006  LDCONST	R5	K3
      0x7C080600,  //  0007  CALL	R2	3
      0x740A0002,  //  0008  JMPT	R2	#000C
      0x8C080104,  //  0009  GETMET	R2	R0	K4
      0x7C080200,  //  000A  CALL	R2	1
      0x80000400,  //  000B  RET	0
      0x8C080105,  //  000C  GETMET	R2	R0	K5
      0x7C080200,  //  000D  CALL	R2	1
      0x60080012,  //  000E  GETGBL	R2	G18
      0x7C080000,  //  000F  CALL	R2	0
      0x600C0013,  //  0010  GETGBL	R3	G19
      0x7C0C0000,  //  0011  CALL	R3	0
      0x8C100106,  //  0012  GETMET	R4	R0	K6
      0x7C100200,  //  0013  CALL	R4	1
      0x7412004B,  //  0014  JMPT	R4	#0061
      0x8C100107,  //  0015  GETMET	R4	R0	K7
      0x7C100200,  //  0016  CALL	R4	1
      0x74120048,  //  0017  JMPT	R4	#0061
      0x8C100108,  //  0018  GETMET	R4	R0	K8
      0x7C100200,  //  0019  CALL	R4	1
      0x8C100107,  //  001A  GETMET	R4	R0	K7
      0x7C100200,  //  001B  CALL	R4	1
      0x78120000,  //  001C  JMPF	R4	#001E
      0x70020042,  //  001D  JMP		#0061
      0x8C100109,  //  001E  GETMET	R4	R0	K9
      0x7C100200,  //  001F  CALL	R4	1
      0x4C140000,  //  0020  LDNIL	R5
      0x20140805,  //  0021  NE	R5	R4	R5
      0x7816003B,  //  0022  JMPF	R5	#005F
      0x8814090A,  //  0023  GETMBR	R5	R4	K10
      0xB81A1600,  //  0024  GETNGBL	R6	K11
      0x88180D0C,  //  0025  GETMBR	R6	R6	K12
      0x88180D0D,  //  0026  GETMBR	R6	R6	K13
      0x1C140A06,  //  0027  EQ	R5	R5	R6
      0x78160035,  //  0028  JMPF	R5	#005F
      0x8814090E,  //  0029  GETMBR	R5	R4	K14
      0x1C140B0F,  //  002A  EQ	R5	R5	K15
      0x78160032,  //  002B  JMPF	R5	#005F
      0x8C140100,  //  002C  GETMET	R5	R0	K0
      0x7C140200,  //  002D  CALL	R5	1
      0x8C140101,  //  002E  GETMET	R5	R0	K1
      0x7C140200,  //  002F  CALL	R5	1
      0x4C180000,  //  0030  LDNIL	R6
      0x8C1C0109,  //  0031  GETMET	R7	R0	K9
      0x7C1C0200,  //  0032  CALL	R7	1
      0x4C200000,  //  0033  LDNIL	R8
      0x201C0E08,  //  0034  NE	R7	R7	R8
      0x781E0011,  //  0035  JMPF	R7	#0048
      0x8C1C0109,  //  0036  GETMET	R7	R0	K9
      0x7C1C0200,  //  0037  CALL	R7	1
      0x881C0F0A,  //  0038  GETMBR	R7	R7	K10
      0xB8221600,  //  0039  GETNGBL	R8	K11
      0x8820110C,  //  003A  GETMBR	R8	R8	K12
      0x8820110D,  //  003B  GETMBR	R8	R8	K13
      0x1C1C0E08,  //  003C  EQ	R7	R7	R8
      0x781E0009,  //  003D  JMPF	R7	#0048
      0x8C1C0109,  //  003E  GETMET	R7	R0	K9
      0x7C1C0200,  //  003F  CALL	R7	1
      0x881C0F0E,  //  0040  GETMBR	R7	R7	K14
      0x1C1C0F0A,  //  0041  EQ	R7	R7	K10
      0x781E0004,  //  0042  JMPF	R7	#0048
      0x8C1C0100,  //  0043  GETMET	R7	R0	K0
      0x7C1C0200,  //  0044  CALL	R7	1
      0x8C1C0101,  //  0045  GETMET	R7	R0	K1
      0x7C1C0200,  //  0046  CALL	R7	1
      0x5C180E00,  //  0047  MOVE	R6	R7
      0x8C1C0510,  //  0048  GETMET	R7	R2	K16
      0x5C240A00,  //  0049  MOVE	R9	R5
      0x7C1C0400,  //  004A  CALL	R7	2
      0x4C1C0000,  //  004B  LDNIL	R7
      0x201C0C07,  //  004C  NE	R7	R6	R7
      0x781E0000,  //  004D  JMPF	R7	#004F
      0x980C0A06,  //  004E  SETIDX	R3	R5	R6
      0x8C1C0109,  //  004F  GETMET	R7	R0	K9
      0x7C1C0200,  //  0050  CALL	R7	1
      0x4C200000,  //  0051  LDNIL	R8
      0x201C0E08,  //  0052  NE	R7	R7	R8
      0x781E0009,  //  0053  JMPF	R7	#005E
      0x8C1C0109,  //  0054  GETMET	R7	R0	K9
      0x7C1C0200,  //  0055  CALL	R7	1
      0x881C0F0A,  //  0056  GETMBR	R7	R7	K10
      0xB8221600,  //  0057  GETNGBL	R8	K11
      0x8820110C,  //  0058  GETMBR	R8	R8	K12
      0x88201111,  //  0059  GETMBR	R8	R8	K17
      0x1C1C0E08,  //  005A  EQ	R7	R7	R8
      0x781E0001,  //  005B  JMPF	R7	#005E
      0x8C1C0100,  //  005C  GETMET	R7	R0	K0
      0x7C1C0200,  //  005D  CALL	R7	1
      0x70020000,  //  005E  JMP		#0060
      0x70020000,  //  005F  JMP		#0061
      0x7001FFB0,  //  0060  JMP		#0012
      0x60100012,  //  0061  GETGBL	R4	G18
      0x7C100000,  //  0062  CALL	R4	0
      0x58140012,  //  0063  LDCONST	R5	K18
      0x8C180106,  //  0064  GETMET	R6	R0	K6
      0x7C180200,  //  0065  CALL	R6	1
      0x741A002B,  //  0066  JMPT	R6	#0093
      0x8C180109,  //  0067  GETMET	R6	R0	K9
      0x7C180200,  //  0068  CALL	R6	1
      0x4C1C0000,  //  0069  LDNIL	R7
      0x1C1C0C07,  //  006A  EQ	R7	R6	R7
      0x741E0005,  //  006B  JMPT	R7	#0072
      0x881C0D0A,  //  006C  GETMBR	R7	R6	K10
      0xB8221600,  //  006D  GETNGBL	R8	K11
      0x8820110C,  //  006E  GETMBR	R8	R8	K12
      0x88201113,  //  006F  GETMBR	R8	R8	K19
      0x1C1C0E08,  //  0070  EQ	R7	R7	R8
      0x781E0000,  //  0071  JMPF	R7	#0073
      0x7002001F,  //  0072  JMP		#0093
      0x881C0D0A,  //  0073  GETMBR	R7	R6	K10
      0xB8221600,  //  0074  GETNGBL	R8	K11
      0x8820110C,  //  0075  GETMBR	R8	R8	K12
      0x88201114,  //  0076  GETMBR	R8	R8	K20
      0x1C1C0E08,  //  0077  EQ	R7	R7	R8
      0x781E0004,  //  0078  JMPF	R7	#007E
      0x00140B15,  //  0079  ADD	R5	R5	K21
      0x8C1C0910,  //  007A  GETMET	R7	R4	K16
      0x5C240C00,  //  007B  MOVE	R9	R6
      0x7C1C0400,  //  007C  CALL	R7	2
      0x70020011,  //  007D  JMP		#0090
      0x881C0D0A,  //  007E  GETMBR	R7	R6	K10
      0xB8221600,  //  007F  GETNGBL	R8	K11
      0x8820110C,  //  0080  GETMBR	R8	R8	K12
      0x88201116,  //  0081  GETMBR	R8	R8	K22
      0x1C1C0E08,  //  0082  EQ	R7	R7	R8
      0x781E0008,  //  0083  JMPF	R7	#008D
      0x1C1C0B12,  //  0084  EQ	R7	R5	K18
      0x781E0001,  //  0085  JMPF	R7	#0088
      0x7002000B,  //  0086  JMP		#0093
      0x70020003,  //  0087  JMP		#008C
      0x04140B15,  //  0088  SUB	R5	R5	K21
      0x8C1C0910,  //  0089  GETMET	R7	R4	K16
      0x5C240C00,  //  008A  MOVE	R9	R6
      0x7C1C0400,  //  008B  CALL	R7	2
      0x70020002,  //  008C  JMP		#0090
      0x8C1C0910,  //  008D  GETMET	R7	R4	K16
      0x5C240C00,  //  008E  MOVE	R9	R6
      0x7C1C0400,  //  008F  CALL	R7	2
      0x8C1C0100,  //  0090  GETMET	R7	R0	K0
      0x7C1C0200,  //  0091  CALL	R7	1
      0x7001FFD0,  //  0092  JMP		#0064
      0x8C180117,  //  0093  GETMET	R6	R0	K23
      0x7C180200,  //  0094  CALL	R6	1
      0x8C180118,  //  0095  GETMET	R6	R0	K24
      0x5C200200,  //  0096  MOVE	R8	R1
      0x5C240400,  //  0097  MOVE	R9	R2
      0x5C280600,  //  0098  MOVE	R10	R3
      0x5C2C0800,  //  0099  MOVE	R11	R4
      0x7C180A00,  //  009A  CALL	R6	5
      0x88180119,  //  009B  GETMBR	R6	R0	K25
      0x8C180D1A,  //  009C  GETMET	R6	R6	K26
      0x5C200200,  //  009D  MOVE	R8	R1
      0x5C240600,  //  009E  MOVE	R9	R3
      0x7C180600,  //  009F  CALL	R6	3
      0x80000000,  //  00A0  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: expect_dot
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_expect_dot,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(DOT),
    /* K5   */  be_nested_str_weak(next),
    /* K6   */  be_nested_str_weak(error),
    /* K7   */  be_nested_str_weak(Expected_X20_X27_X2E_X27),
    }),
    be_str_weak(expect_dot),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0008,  //  0004  JMPF	R2	#000E
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x780A0002,  //  000A  JMPF	R2	#000E
      0x8C080105,  //  000B  GETMET	R2	R0	K5
      0x7C080200,  //  000C  CALL	R2	1
      0x70020002,  //  000D  JMP		#0011
      0x8C080106,  //  000E  GETMET	R2	R0	K6
      0x58100007,  //  000F  LDCONST	R4	K7
      0x7C080400,  //  0010  CALL	R2	2
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _process_simple_value_assignment
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__process_simple_value_assignment,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(IDENTIFIER),
    /* K5   */  be_nested_str_weak(peek),
    /* K6   */  be_nested_str_weak(LEFT_PAREN),
    /* K7   */  be_nested_str_weak(value),
    /* K8   */  be_nested_str_weak(process_value),
    /* K9   */  be_nested_str_weak(collect_inline_comment),
    /* K10  */  be_nested_str_weak(add),
    /* K11  */  be_nested_str_weak(var_X20_X25s__X20_X3D_X20_X25s_X25s),
    /* K12  */  be_nested_str_weak(expr),
    /* K13  */  be_nested_str_weak(symbol_table),
    /* K14  */  be_nested_str_weak(contains),
    /* K15  */  be_nested_str_weak(get),
    /* K16  */  be_nested_str_weak(instance),
    }),
    be_str_weak(_process_simple_value_assignment),
    &be_const_str_solidified,
    ( &(const binstruction[78]) {  /* code */
      0x8C100100,  //  0000  GETMET	R4	R0	K0
      0x7C100200,  //  0001  CALL	R4	1
      0x4C140000,  //  0002  LDNIL	R5
      0x20140805,  //  0003  NE	R5	R4	R5
      0x78160012,  //  0004  JMPF	R5	#0018
      0x88140901,  //  0005  GETMBR	R5	R4	K1
      0xB81A0400,  //  0006  GETNGBL	R6	K2
      0x88180D03,  //  0007  GETMBR	R6	R6	K3
      0x88180D04,  //  0008  GETMBR	R6	R6	K4
      0x1C140A06,  //  0009  EQ	R5	R5	R6
      0x7816000C,  //  000A  JMPF	R5	#0018
      0x8C140105,  //  000B  GETMET	R5	R0	K5
      0x7C140200,  //  000C  CALL	R5	1
      0x4C180000,  //  000D  LDNIL	R6
      0x1C140A06,  //  000E  EQ	R5	R5	R6
      0x74160008,  //  000F  JMPT	R5	#0019
      0x8C140105,  //  0010  GETMET	R5	R0	K5
      0x7C140200,  //  0011  CALL	R5	1
      0x88140B01,  //  0012  GETMBR	R5	R5	K1
      0xB81A0400,  //  0013  GETNGBL	R6	K2
      0x88180D03,  //  0014  GETMBR	R6	R6	K3
      0x88180D06,  //  0015  GETMBR	R6	R6	K6
      0x20140A06,  //  0016  NE	R5	R5	R6
      0x74160000,  //  0017  JMPT	R5	#0019
      0x50140001,  //  0018  LDBOOL	R5	0	1
      0x50140200,  //  0019  LDBOOL	R5	1	0
      0x78160001,  //  001A  JMPF	R5	#001D
      0x88180907,  //  001B  GETMBR	R6	R4	K7
      0x70020000,  //  001C  JMP		#001E
      0x4C180000,  //  001D  LDNIL	R6
      0x8C1C0108,  //  001E  GETMET	R7	R0	K8
      0x5C240400,  //  001F  MOVE	R9	R2
      0x7C1C0400,  //  0020  CALL	R7	2
      0x8C200109,  //  0021  GETMET	R8	R0	K9
      0x7C200200,  //  0022  CALL	R8	1
      0x8C24010A,  //  0023  GETMET	R9	R0	K10
      0x602C0018,  //  0024  GETGBL	R11	G24
      0x5830000B,  //  0025  LDCONST	R12	K11
      0x5C340200,  //  0026  MOVE	R13	R1
      0x88380F0C,  //  0027  GETMBR	R14	R7	K12
      0x5C3C1000,  //  0028  MOVE	R15	R8
      0x7C2C0800,  //  0029  CALL	R11	4
      0x7C240400,  //  002A  CALL	R9	2
      0x7816001C,  //  002B  JMPF	R5	#0049
      0x4C240000,  //  002C  LDNIL	R9
      0x20240C09,  //  002D  NE	R9	R6	R9
      0x78260019,  //  002E  JMPF	R9	#0049
      0x8824010D,  //  002F  GETMBR	R9	R0	K13
      0x8C24130E,  //  0030  GETMET	R9	R9	K14
      0x5C2C0C00,  //  0031  MOVE	R11	R6
      0x7C240400,  //  0032  CALL	R9	2
      0x78260014,  //  0033  JMPF	R9	#0049
      0x8824010D,  //  0034  GETMBR	R9	R0	K13
      0x8C24130F,  //  0035  GETMET	R9	R9	K15
      0x5C2C0C00,  //  0036  MOVE	R11	R6
      0x7C240400,  //  0037  CALL	R9	2
      0x4C280000,  //  0038  LDNIL	R10
      0x2028120A,  //  0039  NE	R10	R9	R10
      0x782A0008,  //  003A  JMPF	R10	#0044
      0x88281310,  //  003B  GETMBR	R10	R9	K16
      0x4C2C0000,  //  003C  LDNIL	R11
      0x2028140B,  //  003D  NE	R10	R10	R11
      0x782A0004,  //  003E  JMPF	R10	#0044
      0x5C280600,  //  003F  MOVE	R10	R3
      0x5C2C0200,  //  0040  MOVE	R11	R1
      0x88301310,  //  0041  GETMBR	R12	R9	K16
      0x7C280400,  //  0042  CALL	R10	2
      0x70020003,  //  0043  JMP		#0048
      0x5C280600,  //  0044  MOVE	R10	R3
      0x5C2C0200,  //  0045  MOVE	R11	R1
      0x4C300000,  //  0046  LDNIL	R12
      0x7C280400,  //  0047  CALL	R10	2
      0x70020003,  //  0048  JMP		#004D
      0x5C240600,  //  0049  MOVE	R9	R3
      0x5C280200,  //  004A  MOVE	R10	R1
      0x4C2C0000,  //  004B  LDNIL	R11
      0x7C240400,  //  004C  CALL	R9	2
      0x80000000,  //  004D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: expect_left_bracket
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_expect_left_bracket,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(LEFT_BRACKET),
    /* K5   */  be_nested_str_weak(next),
    /* K6   */  be_nested_str_weak(error),
    /* K7   */  be_nested_str_weak(Expected_X20_X27_X5B_X27),
    }),
    be_str_weak(expect_left_bracket),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0008,  //  0004  JMPF	R2	#000E
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x780A0002,  //  000A  JMPF	R2	#000E
      0x8C080105,  //  000B  GETMET	R2	R0	K5
      0x7C080200,  //  000C  CALL	R2	1
      0x70020002,  //  000D  JMP		#0011
      0x8C080106,  //  000E  GETMET	R2	R0	K6
      0x58100007,  //  000F  LDCONST	R4	K7
      0x7C080400,  //  0010  CALL	R2	2
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_computation_closure_from_string
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_create_computation_closure_from_string,   /* name */
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
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(transform_expression_for_closure),
    /* K2   */  be_nested_str_weak(find),
    /* K3   */  be_nested_str_weak(_X20_X20),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(replace),
    /* K6   */  be_nested_str_weak(_X20),
    /* K7   */  be_nested_str_weak(def_X20_X28engine_X29_X20return_X20_X25s_X20end),
    /* K8   */  be_nested_str_weak(animation_X2Ecreate_closure_value_X28engine_X2C_X20_X25s_X29),
    }),
    be_str_weak(create_computation_closure_from_string),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0101,  //  0001  GETMET	R3	R0	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x8C100502,  //  0004  GETMET	R4	R2	K2
      0x5C180600,  //  0005  MOVE	R6	R3
      0x581C0003,  //  0006  LDCONST	R7	K3
      0x7C100600,  //  0007  CALL	R4	3
      0x28100904,  //  0008  GE	R4	R4	K4
      0x78120006,  //  0009  JMPF	R4	#0011
      0x8C100505,  //  000A  GETMET	R4	R2	K5
      0x5C180600,  //  000B  MOVE	R6	R3
      0x581C0003,  //  000C  LDCONST	R7	K3
      0x58200006,  //  000D  LDCONST	R8	K6
      0x7C100800,  //  000E  CALL	R4	4
      0x5C0C0800,  //  000F  MOVE	R3	R4
      0x7001FFF2,  //  0010  JMP		#0004
      0x60100018,  //  0011  GETGBL	R4	G24
      0x58140007,  //  0012  LDCONST	R5	K7
      0x5C180600,  //  0013  MOVE	R6	R3
      0x7C100400,  //  0014  CALL	R4	2
      0x60140018,  //  0015  GETGBL	R5	G24
      0x58180008,  //  0016  LDCONST	R6	K8
      0x5C1C0800,  //  0017  MOVE	R7	R4
      0x7C140400,  //  0018  CALL	R5	2
      0x80040A00,  //  0019  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_simple_function_from_string
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_create_simple_function_from_string,   /* name */
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
    /* K0   */  be_nested_str_weak(def_X20_X28engine_X29_X20return_X20_X25s_X20end),
    }),
    be_str_weak(create_simple_function_from_string),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x60080018,  //  0000  GETGBL	R2	G24
      0x580C0000,  //  0001  LDCONST	R3	K0
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: has_errors
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_has_errors,   /* name */
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
    /* K0   */  be_nested_str_weak(errors),
    /* K1   */  be_const_int(0),
    }),
    be_str_weak(has_errors),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x6004000C,  //  0000  GETGBL	R1	G12
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x7C040200,  //  0002  CALL	R1	1
      0x24040301,  //  0003  GT	R1	R1	K1
      0x80040200,  //  0004  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _is_named_arg_constructor
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__is_named_arg_constructor,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(symbol_table),
    /* K1   */  be_nested_str_weak(get),
    /* K2   */  be_nested_str_weak(takes_named_args),
    }),
    be_str_weak(_is_named_arg_constructor),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x4C0C0000,  //  0004  LDNIL	R3
      0x200C0403,  //  0005  NE	R3	R2	R3
      0x780E0002,  //  0006  JMPF	R3	#000A
      0x8C0C0502,  //  0007  GETMET	R3	R2	K2
      0x7C0C0200,  //  0008  CALL	R3	1
      0x80040600,  //  0009  RET	1	R3
      0x500C0200,  //  000A  LDBOOL	R3	1	0
      0x80040600,  //  000B  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_unary_expression
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_unary_expression,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[19]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(error),
    /* K2   */  be_nested_str_weak(Expected_X20value),
    /* K3   */  be_nested_str_weak(ExpressionResult),
    /* K4   */  be_nested_str_weak(literal),
    /* K5   */  be_nested_str_weak(nil),
    /* K6   */  be_nested_str_weak(type),
    /* K7   */  be_nested_str_weak(animation_dsl),
    /* K8   */  be_nested_str_weak(Token),
    /* K9   */  be_nested_str_weak(MINUS),
    /* K10  */  be_nested_str_weak(next),
    /* K11  */  be_nested_str_weak(process_unary_expression),
    /* K12  */  be_nested_str_weak(_X28_X2D_X25s_X29),
    /* K13  */  be_nested_str_weak(expr),
    /* K14  */  be_nested_str_weak(has_functions),
    /* K15  */  be_nested_str_weak(has_variables),
    /* K16  */  be_nested_str_weak(has_properties),
    /* K17  */  be_nested_str_weak(PLUS),
    /* K18  */  be_nested_str_weak(process_primary_expression),
    }),
    be_str_weak(process_unary_expression),
    &be_const_str_solidified,
    ( &(const binstruction[57]) {  /* code */
      0x8C100100,  //  0000  GETMET	R4	R0	K0
      0x7C100200,  //  0001  CALL	R4	1
      0x4C140000,  //  0002  LDNIL	R5
      0x1C140805,  //  0003  EQ	R5	R4	R5
      0x78160007,  //  0004  JMPF	R5	#000D
      0x8C140101,  //  0005  GETMET	R5	R0	K1
      0x581C0002,  //  0006  LDCONST	R7	K2
      0x7C140400,  //  0007  CALL	R5	2
      0x88140103,  //  0008  GETMBR	R5	R0	K3
      0x8C140B04,  //  0009  GETMET	R5	R5	K4
      0x581C0005,  //  000A  LDCONST	R7	K5
      0x7C140400,  //  000B  CALL	R5	2
      0x80040A00,  //  000C  RET	1	R5
      0x88140906,  //  000D  GETMBR	R5	R4	K6
      0xB81A0E00,  //  000E  GETNGBL	R6	K7
      0x88180D08,  //  000F  GETMBR	R6	R6	K8
      0x88180D09,  //  0010  GETMBR	R6	R6	K9
      0x1C140A06,  //  0011  EQ	R5	R5	R6
      0x78160011,  //  0012  JMPF	R5	#0025
      0x8C14010A,  //  0013  GETMET	R5	R0	K10
      0x7C140200,  //  0014  CALL	R5	1
      0x8C14010B,  //  0015  GETMET	R5	R0	K11
      0x5C1C0200,  //  0016  MOVE	R7	R1
      0x50200000,  //  0017  LDBOOL	R8	0	0
      0x5C240600,  //  0018  MOVE	R9	R3
      0x7C140800,  //  0019  CALL	R5	4
      0x8C180103,  //  001A  GETMET	R6	R0	K3
      0x60200018,  //  001B  GETGBL	R8	G24
      0x5824000C,  //  001C  LDCONST	R9	K12
      0x88280B0D,  //  001D  GETMBR	R10	R5	K13
      0x7C200400,  //  001E  CALL	R8	2
      0x88240B0E,  //  001F  GETMBR	R9	R5	K14
      0x88280B0F,  //  0020  GETMBR	R10	R5	K15
      0x882C0B10,  //  0021  GETMBR	R11	R5	K16
      0x50300200,  //  0022  LDBOOL	R12	1	0
      0x7C180C00,  //  0023  CALL	R6	6
      0x80040C00,  //  0024  RET	1	R6
      0x88140906,  //  0025  GETMBR	R5	R4	K6
      0xB81A0E00,  //  0026  GETNGBL	R6	K7
      0x88180D08,  //  0027  GETMBR	R6	R6	K8
      0x88180D11,  //  0028  GETMBR	R6	R6	K17
      0x1C140A06,  //  0029  EQ	R5	R5	R6
      0x78160007,  //  002A  JMPF	R5	#0033
      0x8C14010A,  //  002B  GETMET	R5	R0	K10
      0x7C140200,  //  002C  CALL	R5	1
      0x8C14010B,  //  002D  GETMET	R5	R0	K11
      0x5C1C0200,  //  002E  MOVE	R7	R1
      0x50200000,  //  002F  LDBOOL	R8	0	0
      0x5C240600,  //  0030  MOVE	R9	R3
      0x7C140800,  //  0031  CALL	R5	4
      0x80040A00,  //  0032  RET	1	R5
      0x8C140112,  //  0033  GETMET	R5	R0	K18
      0x5C1C0200,  //  0034  MOVE	R7	R1
      0x5C200400,  //  0035  MOVE	R8	R2
      0x5C240600,  //  0036  MOVE	R9	R3
      0x7C140800,  //  0037  CALL	R5	4
      0x80040A00,  //  0038  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_log_statement_fluent
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_log_statement_fluent,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(next),
    /* K1   */  be_nested_str_weak(expect_left_paren),
    /* K2   */  be_nested_str_weak(current),
    /* K3   */  be_nested_str_weak(type),
    /* K4   */  be_nested_str_weak(animation_dsl),
    /* K5   */  be_nested_str_weak(Token),
    /* K6   */  be_nested_str_weak(STRING),
    /* K7   */  be_nested_str_weak(error),
    /* K8   */  be_nested_str_weak(log_X28_X29_X20function_X20requires_X20a_X20string_X20message),
    /* K9   */  be_nested_str_weak(skip_statement),
    /* K10  */  be_nested_str_weak(value),
    /* K11  */  be_nested_str_weak(expect_right_paren),
    /* K12  */  be_nested_str_weak(collect_inline_comment),
    /* K13  */  be_nested_str_weak(process_log_call),
    /* K14  */  be_nested_str_weak(fluent),
    /* K15  */  be_nested_str_weak(add),
    }),
    be_str_weak(process_log_statement_fluent),
    &be_const_str_solidified,
    ( &(const binstruction[37]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x7C040200,  //  0003  CALL	R1	1
      0x8C040102,  //  0004  GETMET	R1	R0	K2
      0x7C040200,  //  0005  CALL	R1	1
      0x4C080000,  //  0006  LDNIL	R2
      0x1C080202,  //  0007  EQ	R2	R1	R2
      0x740A0005,  //  0008  JMPT	R2	#000F
      0x88080303,  //  0009  GETMBR	R2	R1	K3
      0xB80E0800,  //  000A  GETNGBL	R3	K4
      0x880C0705,  //  000B  GETMBR	R3	R3	K5
      0x880C0706,  //  000C  GETMBR	R3	R3	K6
      0x20080403,  //  000D  NE	R2	R2	R3
      0x780A0005,  //  000E  JMPF	R2	#0015
      0x8C080107,  //  000F  GETMET	R2	R0	K7
      0x58100008,  //  0010  LDCONST	R4	K8
      0x7C080400,  //  0011  CALL	R2	2
      0x8C080109,  //  0012  GETMET	R2	R0	K9
      0x7C080200,  //  0013  CALL	R2	1
      0x80000400,  //  0014  RET	0
      0x8808030A,  //  0015  GETMBR	R2	R1	K10
      0x8C0C0100,  //  0016  GETMET	R3	R0	K0
      0x7C0C0200,  //  0017  CALL	R3	1
      0x8C0C010B,  //  0018  GETMET	R3	R0	K11
      0x7C0C0200,  //  0019  CALL	R3	1
      0x8C0C010C,  //  001A  GETMET	R3	R0	K12
      0x7C0C0200,  //  001B  CALL	R3	1
      0x8C10010D,  //  001C  GETMET	R4	R0	K13
      0x5C180400,  //  001D  MOVE	R6	R2
      0x581C000E,  //  001E  LDCONST	R7	K14
      0x5C200600,  //  001F  MOVE	R8	R3
      0x7C100800,  //  0020  CALL	R4	4
      0x8C14010F,  //  0021  GETMET	R5	R0	K15
      0x5C1C0800,  //  0022  MOVE	R7	R4
      0x7C140400,  //  0023  CALL	R5	2
      0x80000000,  //  0024  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_time_value
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_time_value,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(TIME),
    /* K5   */  be_nested_str_weak(value),
    /* K6   */  be_nested_str_weak(next),
    /* K7   */  be_nested_str_weak(convert_time_to_ms),
    /* K8   */  be_nested_str_weak(NUMBER),
    /* K9   */  be_nested_str_weak(IDENTIFIER),
    /* K10  */  be_nested_str_weak(_validate_object_reference),
    /* K11  */  be_nested_str_weak(duration),
    /* K12  */  be_nested_str_weak(process_primary_expression),
    /* K13  */  be_nested_str_weak(CONTEXT_TIME),
    /* K14  */  be_nested_str_weak(expr),
    /* K15  */  be_nested_str_weak(error),
    /* K16  */  be_nested_str_weak(Expected_X20time_X20value),
    /* K17  */  be_nested_str_weak(1000),
    }),
    be_str_weak(process_time_value),
    &be_const_str_solidified,
    ( &(const binstruction[71]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A000F,  //  0004  JMPF	R2	#0015
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x780A0009,  //  000A  JMPF	R2	#0015
      0x88080305,  //  000B  GETMBR	R2	R1	K5
      0x8C0C0106,  //  000C  GETMET	R3	R0	K6
      0x7C0C0200,  //  000D  CALL	R3	1
      0x600C0008,  //  000E  GETGBL	R3	G8
      0x8C100107,  //  000F  GETMET	R4	R0	K7
      0x5C180400,  //  0010  MOVE	R6	R2
      0x7C100400,  //  0011  CALL	R4	2
      0x7C0C0200,  //  0012  CALL	R3	1
      0x80040600,  //  0013  RET	1	R3
      0x70020030,  //  0014  JMP		#0046
      0x4C080000,  //  0015  LDNIL	R2
      0x20080202,  //  0016  NE	R2	R1	R2
      0x780A0013,  //  0017  JMPF	R2	#002C
      0x88080301,  //  0018  GETMBR	R2	R1	K1
      0xB80E0400,  //  0019  GETNGBL	R3	K2
      0x880C0703,  //  001A  GETMBR	R3	R3	K3
      0x880C0708,  //  001B  GETMBR	R3	R3	K8
      0x1C080403,  //  001C  EQ	R2	R2	R3
      0x780A000D,  //  001D  JMPF	R2	#002C
      0x88080305,  //  001E  GETMBR	R2	R1	K5
      0x8C0C0106,  //  001F  GETMET	R3	R0	K6
      0x7C0C0200,  //  0020  CALL	R3	1
      0x600C0008,  //  0021  GETGBL	R3	G8
      0x60100009,  //  0022  GETGBL	R4	G9
      0x6014000A,  //  0023  GETGBL	R5	G10
      0x5C180400,  //  0024  MOVE	R6	R2
      0x7C140200,  //  0025  CALL	R5	1
      0x7C100200,  //  0026  CALL	R4	1
      0x541603E7,  //  0027  LDINT	R5	1000
      0x08100805,  //  0028  MUL	R4	R4	R5
      0x7C0C0200,  //  0029  CALL	R3	1
      0x80040600,  //  002A  RET	1	R3
      0x70020019,  //  002B  JMP		#0046
      0x4C080000,  //  002C  LDNIL	R2
      0x20080202,  //  002D  NE	R2	R1	R2
      0x780A0012,  //  002E  JMPF	R2	#0042
      0x88080301,  //  002F  GETMBR	R2	R1	K1
      0xB80E0400,  //  0030  GETNGBL	R3	K2
      0x880C0703,  //  0031  GETMBR	R3	R3	K3
      0x880C0709,  //  0032  GETMBR	R3	R3	K9
      0x1C080403,  //  0033  EQ	R2	R2	R3
      0x780A000C,  //  0034  JMPF	R2	#0042
      0x88080305,  //  0035  GETMBR	R2	R1	K5
      0x8C0C010A,  //  0036  GETMET	R3	R0	K10
      0x5C140400,  //  0037  MOVE	R5	R2
      0x5818000B,  //  0038  LDCONST	R6	K11
      0x7C0C0600,  //  0039  CALL	R3	3
      0x8C0C010C,  //  003A  GETMET	R3	R0	K12
      0x8814010D,  //  003B  GETMBR	R5	R0	K13
      0x50180200,  //  003C  LDBOOL	R6	1	0
      0x501C0000,  //  003D  LDBOOL	R7	0	0
      0x7C0C0800,  //  003E  CALL	R3	4
      0x8810070E,  //  003F  GETMBR	R4	R3	K14
      0x80040800,  //  0040  RET	1	R4
      0x70020003,  //  0041  JMP		#0046
      0x8C08010F,  //  0042  GETMET	R2	R0	K15
      0x58100010,  //  0043  LDCONST	R4	K16
      0x7C080400,  //  0044  CALL	R2	2
      0x80062200,  //  0045  RET	1	K17
      0x80000000,  //  0046  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_restart_statement_fluent
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_restart_statement_fluent,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(value),
    /* K2   */  be_nested_str_weak(next),
    /* K3   */  be_nested_str_weak(expect_identifier),
    /* K4   */  be_nested_str_weak(_validate_value_provider_reference),
    /* K5   */  be_nested_str_weak(skip_statement),
    /* K6   */  be_nested_str_weak(collect_inline_comment),
    /* K7   */  be_nested_str_weak(def_X20_X28engine_X29_X20_X25s__X2Estart_X28engine_X2Etime_ms_X29_X20end),
    /* K8   */  be_nested_str_weak(add),
    /* K9   */  be_nested_str_weak(_X25s_X2Epush_closure_step_X28_X25s_X29_X25s),
    /* K10  */  be_nested_str_weak(get_indent),
    }),
    be_str_weak(process_restart_statement_fluent),
    &be_const_str_solidified,
    ( &(const binstruction[31]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x88040301,  //  0002  GETMBR	R1	R1	K1
      0x8C080102,  //  0003  GETMET	R2	R0	K2
      0x7C080200,  //  0004  CALL	R2	1
      0x8C080103,  //  0005  GETMET	R2	R0	K3
      0x7C080200,  //  0006  CALL	R2	1
      0x8C0C0104,  //  0007  GETMET	R3	R0	K4
      0x5C140400,  //  0008  MOVE	R5	R2
      0x5C180200,  //  0009  MOVE	R6	R1
      0x7C0C0600,  //  000A  CALL	R3	3
      0x740E0002,  //  000B  JMPT	R3	#000F
      0x8C0C0105,  //  000C  GETMET	R3	R0	K5
      0x7C0C0200,  //  000D  CALL	R3	1
      0x80000600,  //  000E  RET	0
      0x8C0C0106,  //  000F  GETMET	R3	R0	K6
      0x7C0C0200,  //  0010  CALL	R3	1
      0x60100018,  //  0011  GETGBL	R4	G24
      0x58140007,  //  0012  LDCONST	R5	K7
      0x5C180400,  //  0013  MOVE	R6	R2
      0x7C100400,  //  0014  CALL	R4	2
      0x8C140108,  //  0015  GETMET	R5	R0	K8
      0x601C0018,  //  0016  GETGBL	R7	G24
      0x58200009,  //  0017  LDCONST	R8	K9
      0x8C24010A,  //  0018  GETMET	R9	R0	K10
      0x7C240200,  //  0019  CALL	R9	1
      0x5C280800,  //  001A  MOVE	R10	R4
      0x5C2C0600,  //  001B  MOVE	R11	R3
      0x7C1C0800,  //  001C  CALL	R7	4
      0x7C140400,  //  001D  CALL	R5	2
      0x80000000,  //  001E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: expect_assign
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_expect_assign,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(ASSIGN),
    /* K5   */  be_nested_str_weak(next),
    /* K6   */  be_nested_str_weak(error),
    /* K7   */  be_nested_str_weak(Expected_X20_X27_X3D_X27),
    }),
    be_str_weak(expect_assign),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0008,  //  0004  JMPF	R2	#000E
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x780A0002,  //  000A  JMPF	R2	#000E
      0x8C080105,  //  000B  GETMET	R2	R0	K5
      0x7C080200,  //  000C  CALL	R2	1
      0x70020002,  //  000D  JMP		#0011
      0x8C080106,  //  000E  GETMET	R2	R0	K6
      0x58100007,  //  000F  LDCONST	R4	K7
      0x7C080400,  //  0010  CALL	R2	2
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: generate_default_strip_initialization
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_generate_default_strip_initialization,   /* name */
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
    /* K0   */  be_nested_str_weak(strip_initialized),
    /* K1   */  be_nested_str_weak(add),
    /* K2   */  be_nested_str_weak(_X23_X20Auto_X2Dgenerated_X20strip_X20initialization_X20_X28using_X20Tasmota_X20configuration_X29),
    /* K3   */  be_nested_str_weak(var_X20engine_X20_X3D_X20animation_X2Einit_strip_X28_X29),
    /* K4   */  be_nested_str_weak(),
    }),
    be_str_weak(generate_default_strip_initialization),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060000,  //  0001  JMPF	R1	#0003
      0x80000200,  //  0002  RET	0
      0x8C040101,  //  0003  GETMET	R1	R0	K1
      0x580C0002,  //  0004  LDCONST	R3	K2
      0x7C040400,  //  0005  CALL	R1	2
      0x8C040101,  //  0006  GETMET	R1	R0	K1
      0x580C0003,  //  0007  LDCONST	R3	K3
      0x7C040400,  //  0008  CALL	R1	2
      0x8C040101,  //  0009  GETMET	R1	R0	K1
      0x580C0004,  //  000A  LDCONST	R3	K4
      0x7C040400,  //  000B  CALL	R1	2
      0x50040200,  //  000C  LDBOOL	R1	1	0
      0x90020001,  //  000D  SETMBR	R0	K0	R1
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_primary_expression
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_primary_expression,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[82]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(error),
    /* K2   */  be_nested_str_weak(Expected_X20value),
    /* K3   */  be_nested_str_weak(ExpressionResult),
    /* K4   */  be_nested_str_weak(literal),
    /* K5   */  be_nested_str_weak(nil),
    /* K6   */  be_nested_str_weak(type),
    /* K7   */  be_nested_str_weak(animation_dsl),
    /* K8   */  be_nested_str_weak(Token),
    /* K9   */  be_nested_str_weak(LEFT_PAREN),
    /* K10  */  be_nested_str_weak(next),
    /* K11  */  be_nested_str_weak(process_additive_expression),
    /* K12  */  be_nested_str_weak(expect_right_paren),
    /* K13  */  be_nested_str_weak(_X28_X25s_X29),
    /* K14  */  be_nested_str_weak(expr),
    /* K15  */  be_nested_str_weak(has_functions),
    /* K16  */  be_nested_str_weak(has_variables),
    /* K17  */  be_nested_str_weak(has_properties),
    /* K18  */  be_nested_str_weak(COLOR),
    /* K19  */  be_nested_str_weak(convert_color),
    /* K20  */  be_nested_str_weak(value),
    /* K21  */  be_nested_str_weak(color),
    /* K22  */  be_nested_str_weak(TIME),
    /* K23  */  be_nested_str_weak(process_time_value),
    /* K24  */  be_nested_str_weak(PERCENTAGE),
    /* K25  */  be_nested_str_weak(process_percentage_value),
    /* K26  */  be_nested_str_weak(NUMBER),
    /* K27  */  be_nested_str_weak(KEYWORD),
    /* K28  */  be_nested_str_weak(true),
    /* K29  */  be_nested_str_weak(false),
    /* K30  */  be_nested_str_weak(STRING),
    /* K31  */  be_nested_str_weak(_X22_X25s_X22),
    /* K32  */  be_nested_str_weak(LEFT_BRACKET),
    /* K33  */  be_nested_str_weak(process_array_literal),
    /* K34  */  be_nested_str_weak(IDENTIFIER),
    /* K35  */  be_nested_str_weak(peek),
    /* K36  */  be_nested_str_weak(symbol_table),
    /* K37  */  be_nested_str_weak(get),
    /* K38  */  be_nested_str_weak(Unknown_X20function_X20or_X20identifier_X20_X27_X25s_X27_X2E_X20Make_X20sure_X20it_X27s_X20defined_X20before_X20use_X2E),
    /* K39  */  be_nested_str_weak(skip_statement),
    /* K40  */  be_nested_str_weak(is_user_function),
    /* K41  */  be_nested_str_weak(_process_user_function_call),
    /* K42  */  be_nested_str_weak(function_call),
    /* K43  */  be_nested_str_weak(is_dangerous_call),
    /* K44  */  be_nested_str_weak(Function_X20_X27_X25s_X27_X20cannot_X20be_X20used_X20in_X20computed_X20expressions_X2E_X20This_X20creates_X20a_X20new_X20instance_X20at_X20each_X20evaluation_X2E_X20Use_X20either_X3A_X0A_X20_X20set_X20var_name_X20_X3D_X20_X25s_X28_X29_X20_X20_X23_X20Single_X20function_X20call_X0A_X20_X20set_X20computed_X20_X3D_X20_X28existing_var_X20_X2B_X201_X29_X20_X2F_X202_X20_X20_X23_X20Computation_X20with_X20existing_X20values),
    /* K45  */  be_nested_str_weak(_symbol_entry),
    /* K46  */  be_nested_str_weak(TYPE_MATH_FUNCTION),
    /* K47  */  be_nested_str_weak(process_function_arguments),
    /* K48  */  be_nested_str_weak(_X25s_X28_X25s_X29),
    /* K49  */  be_nested_str_weak(get_reference),
    /* K50  */  be_nested_str_weak(TYPE_TEMPLATE),
    /* K51  */  be_nested_str_weak(),
    /* K52  */  be_nested_str_weak(engine_X2C_X20_X25s),
    /* K53  */  be_nested_str_weak(engine),
    /* K54  */  be_nested_str_weak(_X25s_template_X28_X25s_X29),
    /* K55  */  be_nested_str_weak(Function_X20_X27_X25s_X27_X20not_X20supported_X20in_X20expression_X20context),
    /* K56  */  be_nested_str_weak(takes_named_args),
    /* K57  */  be_nested_str_weak(process_function_call),
    /* K58  */  be_nested_str_weak(_determine_function_return_type),
    /* K59  */  be_nested_str_weak(instance),
    /* K60  */  be_nested_str_weak(CONTEXT_ARGUMENT),
    /* K61  */  be_nested_str_weak(CONTEXT_PROPERTY),
    /* K62  */  be_nested_str_weak(CONTEXT_VARIABLE),
    /* K63  */  be_nested_str_weak(process_nested_function_call),
    /* K64  */  be_nested_str_weak(constructor_call),
    /* K65  */  be_nested_str_weak(Unknown_X20identifier_X20_X27_X25s_X27_X2E_X20Make_X20sure_X20it_X27s_X20defined_X20before_X20use_X2E),
    /* K66  */  be_nested_str_weak(DOT),
    /* K67  */  be_nested_str_weak(expect_identifier),
    /* K68  */  be_nested_str_weak(_X25s_X2E_X25s),
    /* K69  */  be_nested_str_weak(contains),
    /* K70  */  be_nested_str_weak(_validate_single_parameter),
    /* K71  */  be_nested_str_weak(TYPE_SEQUENCE),
    /* K72  */  be_nested_str_weak(Sequences_X20like_X20_X27_X25s_X27_X20do_X20not_X20have_X20properties_X2E_X20Property_X20references_X20are_X20only_X20valid_X20for_X20animations_X20and_X20color_X20providers_X2E),
    /* K73  */  be_nested_str_weak(CONTEXT_REPEAT_COUNT),
    /* K74  */  be_nested_str_weak(property_access),
    /* K75  */  be_nested_str_weak(variable),
    /* K76  */  be_nested_str_weak(animation_X2Eresolve_X28_X25s_X2C_X20_X27_X25s_X27_X29),
    /* K77  */  be_nested_str_weak(symbol_exists),
    /* K78  */  be_nested_str_weak(_determine_symbol_return_type),
    /* K79  */  be_nested_str_weak(variable_ref),
    /* K80  */  be_nested_str_weak(animation_X2E_X25s),
    /* K81  */  be_nested_str_weak(Unexpected_X20value_X3A_X20_X25s),
    }),
    be_str_weak(process_primary_expression),
    &be_const_str_solidified,
    ( &(const binstruction[527]) {  /* code */
      0x8C100100,  //  0000  GETMET	R4	R0	K0
      0x7C100200,  //  0001  CALL	R4	1
      0x4C140000,  //  0002  LDNIL	R5
      0x1C140805,  //  0003  EQ	R5	R4	R5
      0x78160007,  //  0004  JMPF	R5	#000D
      0x8C140101,  //  0005  GETMET	R5	R0	K1
      0x581C0002,  //  0006  LDCONST	R7	K2
      0x7C140400,  //  0007  CALL	R5	2
      0x88140103,  //  0008  GETMBR	R5	R0	K3
      0x8C140B04,  //  0009  GETMET	R5	R5	K4
      0x581C0005,  //  000A  LDCONST	R7	K5
      0x7C140400,  //  000B  CALL	R5	2
      0x80040A00,  //  000C  RET	1	R5
      0x88140906,  //  000D  GETMBR	R5	R4	K6
      0xB81A0E00,  //  000E  GETNGBL	R6	K7
      0x88180D08,  //  000F  GETMBR	R6	R6	K8
      0x88180D09,  //  0010  GETMBR	R6	R6	K9
      0x1C140A06,  //  0011  EQ	R5	R5	R6
      0x78160013,  //  0012  JMPF	R5	#0027
      0x8C14010A,  //  0013  GETMET	R5	R0	K10
      0x7C140200,  //  0014  CALL	R5	1
      0x8C14010B,  //  0015  GETMET	R5	R0	K11
      0x5C1C0200,  //  0016  MOVE	R7	R1
      0x50200000,  //  0017  LDBOOL	R8	0	0
      0x5C240600,  //  0018  MOVE	R9	R3
      0x7C140800,  //  0019  CALL	R5	4
      0x8C18010C,  //  001A  GETMET	R6	R0	K12
      0x7C180200,  //  001B  CALL	R6	1
      0x8C180103,  //  001C  GETMET	R6	R0	K3
      0x60200018,  //  001D  GETGBL	R8	G24
      0x5824000D,  //  001E  LDCONST	R9	K13
      0x88280B0E,  //  001F  GETMBR	R10	R5	K14
      0x7C200400,  //  0020  CALL	R8	2
      0x88240B0F,  //  0021  GETMBR	R9	R5	K15
      0x88280B10,  //  0022  GETMBR	R10	R5	K16
      0x882C0B11,  //  0023  GETMBR	R11	R5	K17
      0x50300200,  //  0024  LDBOOL	R12	1	0
      0x7C180C00,  //  0025  CALL	R6	6
      0x80040C00,  //  0026  RET	1	R6
      0x88140906,  //  0027  GETMBR	R5	R4	K6
      0xB81A0E00,  //  0028  GETNGBL	R6	K7
      0x88180D08,  //  0029  GETMBR	R6	R6	K8
      0x88180D12,  //  002A  GETMBR	R6	R6	K18
      0x1C140A06,  //  002B  EQ	R5	R5	R6
      0x78160009,  //  002C  JMPF	R5	#0037
      0x8C14010A,  //  002D  GETMET	R5	R0	K10
      0x7C140200,  //  002E  CALL	R5	1
      0x88140103,  //  002F  GETMBR	R5	R0	K3
      0x8C140B04,  //  0030  GETMET	R5	R5	K4
      0x8C1C0113,  //  0031  GETMET	R7	R0	K19
      0x88240914,  //  0032  GETMBR	R9	R4	K20
      0x7C1C0400,  //  0033  CALL	R7	2
      0x58200015,  //  0034  LDCONST	R8	K21
      0x7C140600,  //  0035  CALL	R5	3
      0x80040A00,  //  0036  RET	1	R5
      0x88140906,  //  0037  GETMBR	R5	R4	K6
      0xB81A0E00,  //  0038  GETNGBL	R6	K7
      0x88180D08,  //  0039  GETMBR	R6	R6	K8
      0x88180D16,  //  003A  GETMBR	R6	R6	K22
      0x1C140A06,  //  003B  EQ	R5	R5	R6
      0x78160005,  //  003C  JMPF	R5	#0043
      0x88140103,  //  003D  GETMBR	R5	R0	K3
      0x8C140B04,  //  003E  GETMET	R5	R5	K4
      0x8C1C0117,  //  003F  GETMET	R7	R0	K23
      0x7C1C0200,  //  0040  CALL	R7	1
      0x7C140400,  //  0041  CALL	R5	2
      0x80040A00,  //  0042  RET	1	R5
      0x88140906,  //  0043  GETMBR	R5	R4	K6
      0xB81A0E00,  //  0044  GETNGBL	R6	K7
      0x88180D08,  //  0045  GETMBR	R6	R6	K8
      0x88180D18,  //  0046  GETMBR	R6	R6	K24
      0x1C140A06,  //  0047  EQ	R5	R5	R6
      0x78160007,  //  0048  JMPF	R5	#0051
      0x88140103,  //  0049  GETMBR	R5	R0	K3
      0x8C140B04,  //  004A  GETMET	R5	R5	K4
      0x601C0008,  //  004B  GETGBL	R7	G8
      0x8C200119,  //  004C  GETMET	R8	R0	K25
      0x7C200200,  //  004D  CALL	R8	1
      0x7C1C0200,  //  004E  CALL	R7	1
      0x7C140400,  //  004F  CALL	R5	2
      0x80040A00,  //  0050  RET	1	R5
      0x88140906,  //  0051  GETMBR	R5	R4	K6
      0xB81A0E00,  //  0052  GETNGBL	R6	K7
      0x88180D08,  //  0053  GETMBR	R6	R6	K8
      0x88180D1A,  //  0054  GETMBR	R6	R6	K26
      0x1C140A06,  //  0055  EQ	R5	R5	R6
      0x78160007,  //  0056  JMPF	R5	#005F
      0x88140914,  //  0057  GETMBR	R5	R4	K20
      0x8C18010A,  //  0058  GETMET	R6	R0	K10
      0x7C180200,  //  0059  CALL	R6	1
      0x88180103,  //  005A  GETMBR	R6	R0	K3
      0x8C180D04,  //  005B  GETMET	R6	R6	K4
      0x5C200A00,  //  005C  MOVE	R8	R5
      0x7C180400,  //  005D  CALL	R6	2
      0x80040C00,  //  005E  RET	1	R6
      0x88140906,  //  005F  GETMBR	R5	R4	K6
      0xB81A0E00,  //  0060  GETNGBL	R6	K7
      0x88180D08,  //  0061  GETMBR	R6	R6	K8
      0x88180D1B,  //  0062  GETMBR	R6	R6	K27
      0x1C140A06,  //  0063  EQ	R5	R5	R6
      0x7816000D,  //  0064  JMPF	R5	#0073
      0x88140914,  //  0065  GETMBR	R5	R4	K20
      0x1C140B1C,  //  0066  EQ	R5	R5	K28
      0x74160002,  //  0067  JMPT	R5	#006B
      0x88140914,  //  0068  GETMBR	R5	R4	K20
      0x1C140B1D,  //  0069  EQ	R5	R5	K29
      0x78160007,  //  006A  JMPF	R5	#0073
      0x88140914,  //  006B  GETMBR	R5	R4	K20
      0x8C18010A,  //  006C  GETMET	R6	R0	K10
      0x7C180200,  //  006D  CALL	R6	1
      0x88180103,  //  006E  GETMBR	R6	R0	K3
      0x8C180D04,  //  006F  GETMET	R6	R6	K4
      0x5C200A00,  //  0070  MOVE	R8	R5
      0x7C180400,  //  0071  CALL	R6	2
      0x80040C00,  //  0072  RET	1	R6
      0x88140906,  //  0073  GETMBR	R5	R4	K6
      0xB81A0E00,  //  0074  GETNGBL	R6	K7
      0x88180D08,  //  0075  GETMBR	R6	R6	K8
      0x88180D1E,  //  0076  GETMBR	R6	R6	K30
      0x1C140A06,  //  0077  EQ	R5	R5	R6
      0x7816000A,  //  0078  JMPF	R5	#0084
      0x88140914,  //  0079  GETMBR	R5	R4	K20
      0x8C18010A,  //  007A  GETMET	R6	R0	K10
      0x7C180200,  //  007B  CALL	R6	1
      0x88180103,  //  007C  GETMBR	R6	R0	K3
      0x8C180D04,  //  007D  GETMET	R6	R6	K4
      0x60200018,  //  007E  GETGBL	R8	G24
      0x5824001F,  //  007F  LDCONST	R9	K31
      0x5C280A00,  //  0080  MOVE	R10	R5
      0x7C200400,  //  0081  CALL	R8	2
      0x7C180400,  //  0082  CALL	R6	2
      0x80040C00,  //  0083  RET	1	R6
      0x88140906,  //  0084  GETMBR	R5	R4	K6
      0xB81A0E00,  //  0085  GETNGBL	R6	K7
      0x88180D08,  //  0086  GETMBR	R6	R6	K8
      0x88180D20,  //  0087  GETMBR	R6	R6	K32
      0x1C140A06,  //  0088  EQ	R5	R5	R6
      0x78160008,  //  0089  JMPF	R5	#0093
      0x5C140600,  //  008A  MOVE	R5	R3
      0x74160006,  //  008B  JMPT	R5	#0093
      0x8C140121,  //  008C  GETMET	R5	R0	K33
      0x7C140200,  //  008D  CALL	R5	1
      0x88180103,  //  008E  GETMBR	R6	R0	K3
      0x8C180D04,  //  008F  GETMET	R6	R6	K4
      0x5C200A00,  //  0090  MOVE	R8	R5
      0x7C180400,  //  0091  CALL	R6	2
      0x80040C00,  //  0092  RET	1	R6
      0x88140906,  //  0093  GETMBR	R5	R4	K6
      0xB81A0E00,  //  0094  GETNGBL	R6	K7
      0x88180D08,  //  0095  GETMBR	R6	R6	K8
      0x88180D1B,  //  0096  GETMBR	R6	R6	K27
      0x1C140A06,  //  0097  EQ	R5	R5	R6
      0x74160005,  //  0098  JMPT	R5	#009F
      0x88140906,  //  0099  GETMBR	R5	R4	K6
      0xB81A0E00,  //  009A  GETNGBL	R6	K7
      0x88180D08,  //  009B  GETMBR	R6	R6	K8
      0x88180D22,  //  009C  GETMBR	R6	R6	K34
      0x1C140A06,  //  009D  EQ	R5	R5	R6
      0x781600B6,  //  009E  JMPF	R5	#0156
      0x8C140123,  //  009F  GETMET	R5	R0	K35
      0x7C140200,  //  00A0  CALL	R5	1
      0x4C180000,  //  00A1  LDNIL	R6
      0x20140A06,  //  00A2  NE	R5	R5	R6
      0x781600B1,  //  00A3  JMPF	R5	#0156
      0x8C140123,  //  00A4  GETMET	R5	R0	K35
      0x7C140200,  //  00A5  CALL	R5	1
      0x88140B06,  //  00A6  GETMBR	R5	R5	K6
      0xB81A0E00,  //  00A7  GETNGBL	R6	K7
      0x88180D08,  //  00A8  GETMBR	R6	R6	K8
      0x88180D09,  //  00A9  GETMBR	R6	R6	K9
      0x1C140A06,  //  00AA  EQ	R5	R5	R6
      0x781600A9,  //  00AB  JMPF	R5	#0156
      0x88140914,  //  00AC  GETMBR	R5	R4	K20
      0x88180124,  //  00AD  GETMBR	R6	R0	K36
      0x8C180D25,  //  00AE  GETMET	R6	R6	K37
      0x5C200A00,  //  00AF  MOVE	R8	R5
      0x7C180400,  //  00B0  CALL	R6	2
      0x4C1C0000,  //  00B1  LDNIL	R7
      0x1C1C0C07,  //  00B2  EQ	R7	R6	R7
      0x781E000C,  //  00B3  JMPF	R7	#00C1
      0x8C1C0101,  //  00B4  GETMET	R7	R0	K1
      0x60240018,  //  00B5  GETGBL	R9	G24
      0x58280026,  //  00B6  LDCONST	R10	K38
      0x5C2C0A00,  //  00B7  MOVE	R11	R5
      0x7C240400,  //  00B8  CALL	R9	2
      0x7C1C0400,  //  00B9  CALL	R7	2
      0x8C1C0127,  //  00BA  GETMET	R7	R0	K39
      0x7C1C0200,  //  00BB  CALL	R7	1
      0x881C0103,  //  00BC  GETMBR	R7	R0	K3
      0x8C1C0F04,  //  00BD  GETMET	R7	R7	K4
      0x58240005,  //  00BE  LDCONST	R9	K5
      0x7C1C0400,  //  00BF  CALL	R7	2
      0x80040E00,  //  00C0  RET	1	R7
      0x8C1C0D28,  //  00C1  GETMET	R7	R6	K40
      0x7C1C0200,  //  00C2  CALL	R7	1
      0x781E0009,  //  00C3  JMPF	R7	#00CE
      0x8C1C010A,  //  00C4  GETMET	R7	R0	K10
      0x7C1C0200,  //  00C5  CALL	R7	1
      0x8C1C0129,  //  00C6  GETMET	R7	R0	K41
      0x5C240A00,  //  00C7  MOVE	R9	R5
      0x7C1C0400,  //  00C8  CALL	R7	2
      0x88200103,  //  00C9  GETMBR	R8	R0	K3
      0x8C20112A,  //  00CA  GETMET	R8	R8	K42
      0x5C280E00,  //  00CB  MOVE	R10	R7
      0x7C200400,  //  00CC  CALL	R8	2
      0x80041000,  //  00CD  RET	1	R8
      0x5C1C0400,  //  00CE  MOVE	R7	R2
      0x741E0010,  //  00CF  JMPT	R7	#00E1
      0x8C1C0D2B,  //  00D0  GETMET	R7	R6	K43
      0x7C1C0200,  //  00D1  CALL	R7	1
      0x781E000D,  //  00D2  JMPF	R7	#00E1
      0x8C1C0101,  //  00D3  GETMET	R7	R0	K1
      0x60240018,  //  00D4  GETGBL	R9	G24
      0x5828002C,  //  00D5  LDCONST	R10	K44
      0x5C2C0A00,  //  00D6  MOVE	R11	R5
      0x5C300A00,  //  00D7  MOVE	R12	R5
      0x7C240600,  //  00D8  CALL	R9	3
      0x7C1C0400,  //  00D9  CALL	R7	2
      0x8C1C0127,  //  00DA  GETMET	R7	R0	K39
      0x7C1C0200,  //  00DB  CALL	R7	1
      0x881C0103,  //  00DC  GETMBR	R7	R0	K3
      0x8C1C0F04,  //  00DD  GETMET	R7	R7	K4
      0x58240005,  //  00DE  LDCONST	R9	K5
      0x7C1C0400,  //  00DF  CALL	R7	2
      0x80040E00,  //  00E0  RET	1	R7
      0x780E003F,  //  00E1  JMPF	R3	#0122
      0x8C1C010A,  //  00E2  GETMET	R7	R0	K10
      0x7C1C0200,  //  00E3  CALL	R7	1
      0x4C1C0000,  //  00E4  LDNIL	R7
      0x201C0C07,  //  00E5  NE	R7	R6	R7
      0x781E0011,  //  00E6  JMPF	R7	#00F9
      0x881C0D06,  //  00E7  GETMBR	R7	R6	K6
      0xB8220E00,  //  00E8  GETNGBL	R8	K7
      0x8820112D,  //  00E9  GETMBR	R8	R8	K45
      0x8820112E,  //  00EA  GETMBR	R8	R8	K46
      0x1C1C0E08,  //  00EB  EQ	R7	R7	R8
      0x781E000B,  //  00EC  JMPF	R7	#00F9
      0x8C1C012F,  //  00ED  GETMET	R7	R0	K47
      0x50240200,  //  00EE  LDBOOL	R9	1	0
      0x7C1C0400,  //  00EF  CALL	R7	2
      0x88200103,  //  00F0  GETMBR	R8	R0	K3
      0x8C20112A,  //  00F1  GETMET	R8	R8	K42
      0x60280018,  //  00F2  GETGBL	R10	G24
      0x582C0030,  //  00F3  LDCONST	R11	K48
      0x8C300D31,  //  00F4  GETMET	R12	R6	K49
      0x7C300200,  //  00F5  CALL	R12	1
      0x5C340E00,  //  00F6  MOVE	R13	R7
      0x7C280600,  //  00F7  CALL	R10	3
      0x7C200400,  //  00F8  CALL	R8	2
      0x4C1C0000,  //  00F9  LDNIL	R7
      0x201C0C07,  //  00FA  NE	R7	R6	R7
      0x781E0019,  //  00FB  JMPF	R7	#0116
      0x881C0D06,  //  00FC  GETMBR	R7	R6	K6
      0xB8220E00,  //  00FD  GETNGBL	R8	K7
      0x8820112D,  //  00FE  GETMBR	R8	R8	K45
      0x88201132,  //  00FF  GETMBR	R8	R8	K50
      0x1C1C0E08,  //  0100  EQ	R7	R7	R8
      0x781E0013,  //  0101  JMPF	R7	#0116
      0x8C1C012F,  //  0102  GETMET	R7	R0	K47
      0x50240200,  //  0103  LDBOOL	R9	1	0
      0x7C1C0400,  //  0104  CALL	R7	2
      0x20200F33,  //  0105  NE	R8	R7	K51
      0x78220004,  //  0106  JMPF	R8	#010C
      0x60200018,  //  0107  GETGBL	R8	G24
      0x58240034,  //  0108  LDCONST	R9	K52
      0x5C280E00,  //  0109  MOVE	R10	R7
      0x7C200400,  //  010A  CALL	R8	2
      0x70020000,  //  010B  JMP		#010D
      0x58200035,  //  010C  LDCONST	R8	K53
      0x88240103,  //  010D  GETMBR	R9	R0	K3
      0x8C24132A,  //  010E  GETMET	R9	R9	K42
      0x602C0018,  //  010F  GETGBL	R11	G24
      0x58300036,  //  0110  LDCONST	R12	K54
      0x5C340A00,  //  0111  MOVE	R13	R5
      0x5C381000,  //  0112  MOVE	R14	R8
      0x7C2C0600,  //  0113  CALL	R11	3
      0x7C240400,  //  0114  CALL	R9	2
      0x80041200,  //  0115  RET	1	R9
      0x8C1C0101,  //  0116  GETMET	R7	R0	K1
      0x60240018,  //  0117  GETGBL	R9	G24
      0x58280037,  //  0118  LDCONST	R10	K55
      0x5C2C0A00,  //  0119  MOVE	R11	R5
      0x7C240400,  //  011A  CALL	R9	2
      0x7C1C0400,  //  011B  CALL	R7	2
      0x881C0103,  //  011C  GETMBR	R7	R0	K3
      0x8C1C0F04,  //  011D  GETMET	R7	R7	K4
      0x58240005,  //  011E  LDCONST	R9	K5
      0x7C1C0400,  //  011F  CALL	R7	2
      0x80040E00,  //  0120  RET	1	R7
      0x70020033,  //  0121  JMP		#0156
      0x8C1C0D38,  //  0122  GETMET	R7	R6	K56
      0x7C1C0200,  //  0123  CALL	R7	1
      0x741E000D,  //  0124  JMPT	R7	#0133
      0x8C1C0139,  //  0125  GETMET	R7	R0	K57
      0x5C240200,  //  0126  MOVE	R9	R1
      0x7C1C0400,  //  0127  CALL	R7	2
      0x8C20013A,  //  0128  GETMET	R8	R0	K58
      0x5C280C00,  //  0129  MOVE	R10	R6
      0x7C200400,  //  012A  CALL	R8	2
      0x88240103,  //  012B  GETMBR	R9	R0	K3
      0x8C24132A,  //  012C  GETMET	R9	R9	K42
      0x5C2C0E00,  //  012D  MOVE	R11	R7
      0x5C301000,  //  012E  MOVE	R12	R8
      0x88340D3B,  //  012F  GETMBR	R13	R6	K59
      0x7C240800,  //  0130  CALL	R9	4
      0x80041200,  //  0131  RET	1	R9
      0x70020022,  //  0132  JMP		#0156
      0x881C013C,  //  0133  GETMBR	R7	R0	K60
      0x1C1C0207,  //  0134  EQ	R7	R1	R7
      0x741E0005,  //  0135  JMPT	R7	#013C
      0x881C013D,  //  0136  GETMBR	R7	R0	K61
      0x1C1C0207,  //  0137  EQ	R7	R1	R7
      0x741E0002,  //  0138  JMPT	R7	#013C
      0x881C013E,  //  0139  GETMBR	R7	R0	K62
      0x1C1C0207,  //  013A  EQ	R7	R1	R7
      0x781E000C,  //  013B  JMPF	R7	#0149
      0x8C1C013F,  //  013C  GETMET	R7	R0	K63
      0x7C1C0200,  //  013D  CALL	R7	1
      0x8C20013A,  //  013E  GETMET	R8	R0	K58
      0x5C280C00,  //  013F  MOVE	R10	R6
      0x7C200400,  //  0140  CALL	R8	2
      0x88240103,  //  0141  GETMBR	R9	R0	K3
      0x8C241340,  //  0142  GETMET	R9	R9	K64
      0x5C2C0E00,  //  0143  MOVE	R11	R7
      0x5C301000,  //  0144  MOVE	R12	R8
      0x88340D3B,  //  0145  GETMBR	R13	R6	K59
      0x7C240800,  //  0146  CALL	R9	4
      0x80041200,  //  0147  RET	1	R9
      0x7002000C,  //  0148  JMP		#0156
      0x8C1C0139,  //  0149  GETMET	R7	R0	K57
      0x5C240200,  //  014A  MOVE	R9	R1
      0x7C1C0400,  //  014B  CALL	R7	2
      0x8C20013A,  //  014C  GETMET	R8	R0	K58
      0x5C280C00,  //  014D  MOVE	R10	R6
      0x7C200400,  //  014E  CALL	R8	2
      0x88240103,  //  014F  GETMBR	R9	R0	K3
      0x8C241340,  //  0150  GETMET	R9	R9	K64
      0x5C2C0E00,  //  0151  MOVE	R11	R7
      0x5C301000,  //  0152  MOVE	R12	R8
      0x88340D3B,  //  0153  GETMBR	R13	R6	K59
      0x7C240800,  //  0154  CALL	R9	4
      0x80041200,  //  0155  RET	1	R9
      0x88140906,  //  0156  GETMBR	R5	R4	K6
      0xB81A0E00,  //  0157  GETNGBL	R6	K7
      0x88180D08,  //  0158  GETMBR	R6	R6	K8
      0x88180D22,  //  0159  GETMBR	R6	R6	K34
      0x1C140A06,  //  015A  EQ	R5	R5	R6
      0x78160094,  //  015B  JMPF	R5	#01F1
      0x88140914,  //  015C  GETMBR	R5	R4	K20
      0x88180124,  //  015D  GETMBR	R6	R0	K36
      0x8C180D25,  //  015E  GETMET	R6	R6	K37
      0x5C200A00,  //  015F  MOVE	R8	R5
      0x7C180400,  //  0160  CALL	R6	2
      0x4C1C0000,  //  0161  LDNIL	R7
      0x1C1C0C07,  //  0162  EQ	R7	R6	R7
      0x781E000C,  //  0163  JMPF	R7	#0171
      0x8C1C0101,  //  0164  GETMET	R7	R0	K1
      0x60240018,  //  0165  GETGBL	R9	G24
      0x58280041,  //  0166  LDCONST	R10	K65
      0x5C2C0A00,  //  0167  MOVE	R11	R5
      0x7C240400,  //  0168  CALL	R9	2
      0x7C1C0400,  //  0169  CALL	R7	2
      0x8C1C0127,  //  016A  GETMET	R7	R0	K39
      0x7C1C0200,  //  016B  CALL	R7	1
      0x881C0103,  //  016C  GETMBR	R7	R0	K3
      0x8C1C0F04,  //  016D  GETMET	R7	R7	K4
      0x58240005,  //  016E  LDCONST	R9	K5
      0x7C1C0400,  //  016F  CALL	R7	2
      0x80040E00,  //  0170  RET	1	R7
      0x8C1C010A,  //  0171  GETMET	R7	R0	K10
      0x7C1C0200,  //  0172  CALL	R7	1
      0x8C1C0100,  //  0173  GETMET	R7	R0	K0
      0x7C1C0200,  //  0174  CALL	R7	1
      0x4C200000,  //  0175  LDNIL	R8
      0x201C0E08,  //  0176  NE	R7	R7	R8
      0x781E0058,  //  0177  JMPF	R7	#01D1
      0x8C1C0100,  //  0178  GETMET	R7	R0	K0
      0x7C1C0200,  //  0179  CALL	R7	1
      0x881C0F06,  //  017A  GETMBR	R7	R7	K6
      0xB8220E00,  //  017B  GETNGBL	R8	K7
      0x88201108,  //  017C  GETMBR	R8	R8	K8
      0x88201142,  //  017D  GETMBR	R8	R8	K66
      0x1C1C0E08,  //  017E  EQ	R7	R7	R8
      0x781E0050,  //  017F  JMPF	R7	#01D1
      0x8C1C010A,  //  0180  GETMET	R7	R0	K10
      0x7C1C0200,  //  0181  CALL	R7	1
      0x8C1C0143,  //  0182  GETMET	R7	R0	K67
      0x7C1C0200,  //  0183  CALL	R7	1
      0x60200018,  //  0184  GETGBL	R8	G24
      0x58240044,  //  0185  LDCONST	R9	K68
      0x5C280A00,  //  0186  MOVE	R10	R5
      0x5C2C0E00,  //  0187  MOVE	R11	R7
      0x7C200600,  //  0188  CALL	R8	3
      0x5C240600,  //  0189  MOVE	R9	R3
      0x74260028,  //  018A  JMPT	R9	#01B4
      0x88240124,  //  018B  GETMBR	R9	R0	K36
      0x8C241345,  //  018C  GETMET	R9	R9	K69
      0x5C2C0A00,  //  018D  MOVE	R11	R5
      0x7C240400,  //  018E  CALL	R9	2
      0x78260023,  //  018F  JMPF	R9	#01B4
      0x4C240000,  //  0190  LDNIL	R9
      0x20240C09,  //  0191  NE	R9	R6	R9
      0x7826000C,  //  0192  JMPF	R9	#01A0
      0x88240D3B,  //  0193  GETMBR	R9	R6	K59
      0x4C280000,  //  0194  LDNIL	R10
      0x2024120A,  //  0195  NE	R9	R9	R10
      0x78260008,  //  0196  JMPF	R9	#01A0
      0x60240005,  //  0197  GETGBL	R9	G5
      0x88280D3B,  //  0198  GETMBR	R10	R6	K59
      0x7C240200,  //  0199  CALL	R9	1
      0x8C280146,  //  019A  GETMET	R10	R0	K70
      0x5C301200,  //  019B  MOVE	R12	R9
      0x5C340E00,  //  019C  MOVE	R13	R7
      0x88380D3B,  //  019D  GETMBR	R14	R6	K59
      0x7C280800,  //  019E  CALL	R10	4
      0x70020013,  //  019F  JMP		#01B4
      0x4C240000,  //  01A0  LDNIL	R9
      0x20240C09,  //  01A1  NE	R9	R6	R9
      0x78260010,  //  01A2  JMPF	R9	#01B4
      0x88240D06,  //  01A3  GETMBR	R9	R6	K6
      0xB82A0E00,  //  01A4  GETNGBL	R10	K7
      0x8828152D,  //  01A5  GETMBR	R10	R10	K45
      0x88281547,  //  01A6  GETMBR	R10	R10	K71
      0x1C24120A,  //  01A7  EQ	R9	R9	R10
      0x7826000A,  //  01A8  JMPF	R9	#01B4
      0x8C240101,  //  01A9  GETMET	R9	R0	K1
      0x602C0018,  //  01AA  GETGBL	R11	G24
      0x58300048,  //  01AB  LDCONST	R12	K72
      0x5C340A00,  //  01AC  MOVE	R13	R5
      0x7C2C0400,  //  01AD  CALL	R11	2
      0x7C240400,  //  01AE  CALL	R9	2
      0x88240103,  //  01AF  GETMBR	R9	R0	K3
      0x8C241304,  //  01B0  GETMET	R9	R9	K4
      0x582C0005,  //  01B1  LDCONST	R11	K5
      0x7C240400,  //  01B2  CALL	R9	2
      0x80041200,  //  01B3  RET	1	R9
      0x88240124,  //  01B4  GETMBR	R9	R0	K36
      0x8C241331,  //  01B5  GETMET	R9	R9	K49
      0x5C2C0A00,  //  01B6  MOVE	R11	R5
      0x7C240400,  //  01B7  CALL	R9	2
      0x740E0002,  //  01B8  JMPT	R3	#01BC
      0x88280149,  //  01B9  GETMBR	R10	R0	K73
      0x1C28020A,  //  01BA  EQ	R10	R1	R10
      0x782A000A,  //  01BB  JMPF	R10	#01C7
      0x88280103,  //  01BC  GETMBR	R10	R0	K3
      0x8C28154A,  //  01BD  GETMET	R10	R10	K74
      0x60300018,  //  01BE  GETGBL	R12	G24
      0x58340044,  //  01BF  LDCONST	R13	K68
      0x5C381200,  //  01C0  MOVE	R14	R9
      0x5C3C0E00,  //  01C1  MOVE	R15	R7
      0x7C300600,  //  01C2  CALL	R12	3
      0x5834004B,  //  01C3  LDCONST	R13	K75
      0x7C280600,  //  01C4  CALL	R10	3
      0x80041400,  //  01C5  RET	1	R10
      0x70020009,  //  01C6  JMP		#01D1
      0x88280103,  //  01C7  GETMBR	R10	R0	K3
      0x8C28154A,  //  01C8  GETMET	R10	R10	K74
      0x60300018,  //  01C9  GETGBL	R12	G24
      0x5834004C,  //  01CA  LDCONST	R13	K76
      0x5C381200,  //  01CB  MOVE	R14	R9
      0x5C3C0E00,  //  01CC  MOVE	R15	R7
      0x7C300600,  //  01CD  CALL	R12	3
      0x5834004B,  //  01CE  LDCONST	R13	K75
      0x7C280600,  //  01CF  CALL	R10	3
      0x80041400,  //  01D0  RET	1	R10
      0x881C0124,  //  01D1  GETMBR	R7	R0	K36
      0x8C1C0F31,  //  01D2  GETMET	R7	R7	K49
      0x5C240A00,  //  01D3  MOVE	R9	R5
      0x7C1C0400,  //  01D4  CALL	R7	2
      0x88200124,  //  01D5  GETMBR	R8	R0	K36
      0x8C20114D,  //  01D6  GETMET	R8	R8	K77
      0x5C280A00,  //  01D7  MOVE	R10	R5
      0x7C200400,  //  01D8  CALL	R8	2
      0x78220009,  //  01D9  JMPF	R8	#01E4
      0x8C20014E,  //  01DA  GETMET	R8	R0	K78
      0x5C280A00,  //  01DB  MOVE	R10	R5
      0x7C200400,  //  01DC  CALL	R8	2
      0x88240103,  //  01DD  GETMBR	R9	R0	K3
      0x8C24134F,  //  01DE  GETMET	R9	R9	K79
      0x5C2C0E00,  //  01DF  MOVE	R11	R7
      0x5C301000,  //  01E0  MOVE	R12	R8
      0x7C240600,  //  01E1  CALL	R9	3
      0x80041200,  //  01E2  RET	1	R9
      0x7002000C,  //  01E3  JMP		#01F1
      0x8C200101,  //  01E4  GETMET	R8	R0	K1
      0x60280018,  //  01E5  GETGBL	R10	G24
      0x582C0041,  //  01E6  LDCONST	R11	K65
      0x5C300A00,  //  01E7  MOVE	R12	R5
      0x7C280400,  //  01E8  CALL	R10	2
      0x7C200400,  //  01E9  CALL	R8	2
      0x8C200127,  //  01EA  GETMET	R8	R0	K39
      0x7C200200,  //  01EB  CALL	R8	1
      0x88200103,  //  01EC  GETMBR	R8	R0	K3
      0x8C201104,  //  01ED  GETMET	R8	R8	K4
      0x58280005,  //  01EE  LDCONST	R10	K5
      0x7C200400,  //  01EF  CALL	R8	2
      0x80041000,  //  01F0  RET	1	R8
      0x88140906,  //  01F1  GETMBR	R5	R4	K6
      0xB81A0E00,  //  01F2  GETNGBL	R6	K7
      0x88180D08,  //  01F3  GETMBR	R6	R6	K8
      0x88180D1B,  //  01F4  GETMBR	R6	R6	K27
      0x1C140A06,  //  01F5  EQ	R5	R5	R6
      0x7816000A,  //  01F6  JMPF	R5	#0202
      0x88140914,  //  01F7  GETMBR	R5	R4	K20
      0x8C18010A,  //  01F8  GETMET	R6	R0	K10
      0x7C180200,  //  01F9  CALL	R6	1
      0x88180103,  //  01FA  GETMBR	R6	R0	K3
      0x8C180D04,  //  01FB  GETMET	R6	R6	K4
      0x60200018,  //  01FC  GETGBL	R8	G24
      0x58240050,  //  01FD  LDCONST	R9	K80
      0x5C280A00,  //  01FE  MOVE	R10	R5
      0x7C200400,  //  01FF  CALL	R8	2
      0x7C180400,  //  0200  CALL	R6	2
      0x80040C00,  //  0201  RET	1	R6
      0x8C140101,  //  0202  GETMET	R5	R0	K1
      0x601C0018,  //  0203  GETGBL	R7	G24
      0x58200051,  //  0204  LDCONST	R8	K81
      0x88240914,  //  0205  GETMBR	R9	R4	K20
      0x7C1C0400,  //  0206  CALL	R7	2
      0x7C140400,  //  0207  CALL	R5	2
      0x8C140127,  //  0208  GETMET	R5	R0	K39
      0x7C140200,  //  0209  CALL	R5	1
      0x88140103,  //  020A  GETMBR	R5	R0	K3
      0x8C140B04,  //  020B  GETMET	R5	R5	K4
      0x581C0005,  //  020C  LDCONST	R7	K5
      0x7C140400,  //  020D  CALL	R5	2
      0x80040A00,  //  020E  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: transpile
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_transpile,   /* name */
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
    /* K0   */  be_nested_str_weak(add),
    /* K1   */  be_nested_str_weak(import_X20animation),
    /* K2   */  be_nested_str_weak(),
    /* K3   */  be_nested_str_weak(at_end),
    /* K4   */  be_nested_str_weak(process_statement),
    /* K5   */  be_nested_str_weak(generate_engine_run),
    /* K6   */  be_nested_str_weak(errors),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str_weak(join_output),
    /* K9   */  be_nested_str_weak(error),
    /* K10  */  be_nested_str_weak(Transpilation_X20failed_X3A_X20_X25s),
    }),
    be_str_weak(transpile),
    &be_const_str_solidified,
    ( &(const binstruction[41]) {  /* code */
      0xA802001A,  //  0000  EXBLK	0	#001C
      0x8C040100,  //  0001  GETMET	R1	R0	K0
      0x580C0001,  //  0002  LDCONST	R3	K1
      0x7C040400,  //  0003  CALL	R1	2
      0x8C040100,  //  0004  GETMET	R1	R0	K0
      0x580C0002,  //  0005  LDCONST	R3	K2
      0x7C040400,  //  0006  CALL	R1	2
      0x8C040103,  //  0007  GETMET	R1	R0	K3
      0x7C040200,  //  0008  CALL	R1	1
      0x74060002,  //  0009  JMPT	R1	#000D
      0x8C040104,  //  000A  GETMET	R1	R0	K4
      0x7C040200,  //  000B  CALL	R1	1
      0x7001FFF9,  //  000C  JMP		#0007
      0x8C040105,  //  000D  GETMET	R1	R0	K5
      0x7C040200,  //  000E  CALL	R1	1
      0x6004000C,  //  000F  GETGBL	R1	G12
      0x88080106,  //  0010  GETMBR	R2	R0	K6
      0x7C040200,  //  0011  CALL	R1	1
      0x1C040307,  //  0012  EQ	R1	R1	K7
      0x78060002,  //  0013  JMPF	R1	#0017
      0x8C040108,  //  0014  GETMET	R1	R0	K8
      0x7C040200,  //  0015  CALL	R1	1
      0x70020000,  //  0016  JMP		#0018
      0x4C040000,  //  0017  LDNIL	R1
      0xA8040001,  //  0018  EXBLK	1	1
      0x80040200,  //  0019  RET	1	R1
      0xA8040001,  //  001A  EXBLK	1	1
      0x7002000B,  //  001B  JMP		#0028
      0xAC040002,  //  001C  CATCH	R1	0	2
      0x70020008,  //  001D  JMP		#0027
      0x8C0C0109,  //  001E  GETMET	R3	R0	K9
      0x60140018,  //  001F  GETGBL	R5	G24
      0x5818000A,  //  0020  LDCONST	R6	K10
      0x5C1C0400,  //  0021  MOVE	R7	R2
      0x7C140400,  //  0022  CALL	R5	2
      0x7C0C0400,  //  0023  CALL	R3	2
      0x4C0C0000,  //  0024  LDNIL	R3
      0x80040600,  //  0025  RET	1	R3
      0x70020000,  //  0026  JMP		#0028
      0xB0080000,  //  0027  RAISE	2	R0	R0
      0x80000000,  //  0028  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: can_use_as_identifier
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_can_use_as_identifier,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(color),
    /* K1   */  be_nested_str_weak(animation),
    /* K2   */  be_nested_str_weak(palette),
    /* K3   */  be_nested_str_weak(startup),
    /* K4   */  be_nested_str_weak(shutdown),
    /* K5   */  be_nested_str_weak(button_press),
    /* K6   */  be_nested_str_weak(button_hold),
    /* K7   */  be_nested_str_weak(motion_detected),
    /* K8   */  be_nested_str_weak(brightness_change),
    /* K9   */  be_nested_str_weak(timer),
    /* K10  */  be_nested_str_weak(time),
    /* K11  */  be_nested_str_weak(sound_peak),
    /* K12  */  be_nested_str_weak(network_message),
    /* K13  */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(can_use_as_identifier),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0x60080012,  //  0000  GETGBL	R2	G18
      0x7C080000,  //  0001  CALL	R2	0
      0x400C0500,  //  0002  CONNECT	R3	R2	K0
      0x400C0501,  //  0003  CONNECT	R3	R2	K1
      0x400C0502,  //  0004  CONNECT	R3	R2	K2
      0x400C0503,  //  0005  CONNECT	R3	R2	K3
      0x400C0504,  //  0006  CONNECT	R3	R2	K4
      0x400C0505,  //  0007  CONNECT	R3	R2	K5
      0x400C0506,  //  0008  CONNECT	R3	R2	K6
      0x400C0507,  //  0009  CONNECT	R3	R2	K7
      0x400C0508,  //  000A  CONNECT	R3	R2	K8
      0x400C0509,  //  000B  CONNECT	R3	R2	K9
      0x400C050A,  //  000C  CONNECT	R3	R2	K10
      0x400C050B,  //  000D  CONNECT	R3	R2	K11
      0x400C050C,  //  000E  CONNECT	R3	R2	K12
      0x600C0010,  //  000F  GETGBL	R3	G16
      0x5C100400,  //  0010  MOVE	R4	R2
      0x7C0C0200,  //  0011  CALL	R3	1
      0xA8020007,  //  0012  EXBLK	0	#001B
      0x5C100600,  //  0013  MOVE	R4	R3
      0x7C100000,  //  0014  CALL	R4	0
      0x1C140204,  //  0015  EQ	R5	R1	R4
      0x78160002,  //  0016  JMPF	R5	#001A
      0x50140200,  //  0017  LDBOOL	R5	1	0
      0xA8040001,  //  0018  EXBLK	1	1
      0x80040A00,  //  0019  RET	1	R5
      0x7001FFF7,  //  001A  JMP		#0013
      0x580C000D,  //  001B  LDCONST	R3	K13
      0xAC0C0200,  //  001C  CATCH	R3	1	0
      0xB0080000,  //  001D  RAISE	2	R0	R0
      0x500C0000,  //  001E  LDBOOL	R3	0	0
      0x80040600,  //  001F  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: convert_time_to_ms
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_convert_time_to_ms,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(endswith),
    /* K2   */  be_nested_str_weak(ms),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(s),
    /* K5   */  be_nested_str_weak(m),
    /* K6   */  be_nested_str_weak(h),
    /* K7   */  be_const_int(3600000),
    }),
    be_str_weak(convert_time_to_ms),
    &be_const_str_solidified,
    ( &(const binstruction[63]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0501,  //  0001  GETMET	R3	R2	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x58180002,  //  0003  LDCONST	R6	K2
      0x7C0C0600,  //  0004  CALL	R3	3
      0x780E0008,  //  0005  JMPF	R3	#000F
      0x600C0009,  //  0006  GETGBL	R3	G9
      0x6010000A,  //  0007  GETGBL	R4	G10
      0x5415FFFC,  //  0008  LDINT	R5	-3
      0x40160605,  //  0009  CONNECT	R5	K3	R5
      0x94140205,  //  000A  GETIDX	R5	R1	R5
      0x7C100200,  //  000B  CALL	R4	1
      0x7C0C0200,  //  000C  CALL	R3	1
      0x80040600,  //  000D  RET	1	R3
      0x7002002D,  //  000E  JMP		#003D
      0x8C0C0501,  //  000F  GETMET	R3	R2	K1
      0x5C140200,  //  0010  MOVE	R5	R1
      0x58180004,  //  0011  LDCONST	R6	K4
      0x7C0C0600,  //  0012  CALL	R3	3
      0x780E000A,  //  0013  JMPF	R3	#001F
      0x600C0009,  //  0014  GETGBL	R3	G9
      0x6010000A,  //  0015  GETGBL	R4	G10
      0x5415FFFD,  //  0016  LDINT	R5	-2
      0x40160605,  //  0017  CONNECT	R5	K3	R5
      0x94140205,  //  0018  GETIDX	R5	R1	R5
      0x7C100200,  //  0019  CALL	R4	1
      0x541603E7,  //  001A  LDINT	R5	1000
      0x08100805,  //  001B  MUL	R4	R4	R5
      0x7C0C0200,  //  001C  CALL	R3	1
      0x80040600,  //  001D  RET	1	R3
      0x7002001D,  //  001E  JMP		#003D
      0x8C0C0501,  //  001F  GETMET	R3	R2	K1
      0x5C140200,  //  0020  MOVE	R5	R1
      0x58180005,  //  0021  LDCONST	R6	K5
      0x7C0C0600,  //  0022  CALL	R3	3
      0x780E000A,  //  0023  JMPF	R3	#002F
      0x600C0009,  //  0024  GETGBL	R3	G9
      0x6010000A,  //  0025  GETGBL	R4	G10
      0x5415FFFD,  //  0026  LDINT	R5	-2
      0x40160605,  //  0027  CONNECT	R5	K3	R5
      0x94140205,  //  0028  GETIDX	R5	R1	R5
      0x7C100200,  //  0029  CALL	R4	1
      0x5416EA5F,  //  002A  LDINT	R5	60000
      0x08100805,  //  002B  MUL	R4	R4	R5
      0x7C0C0200,  //  002C  CALL	R3	1
      0x80040600,  //  002D  RET	1	R3
      0x7002000D,  //  002E  JMP		#003D
      0x8C0C0501,  //  002F  GETMET	R3	R2	K1
      0x5C140200,  //  0030  MOVE	R5	R1
      0x58180006,  //  0031  LDCONST	R6	K6
      0x7C0C0600,  //  0032  CALL	R3	3
      0x780E0008,  //  0033  JMPF	R3	#003D
      0x600C0009,  //  0034  GETGBL	R3	G9
      0x6010000A,  //  0035  GETGBL	R4	G10
      0x5415FFFD,  //  0036  LDINT	R5	-2
      0x40160605,  //  0037  CONNECT	R5	K3	R5
      0x94140205,  //  0038  GETIDX	R5	R1	R5
      0x7C100200,  //  0039  CALL	R4	1
      0x08100907,  //  003A  MUL	R4	R4	K7
      0x7C0C0200,  //  003B  CALL	R3	1
      0x80040600,  //  003C  RET	1	R3
      0x540E03E7,  //  003D  LDINT	R3	1000
      0x80040600,  //  003E  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: skip_statement
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_skip_statement,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(at_end),
    /* K1   */  be_nested_str_weak(current),
    /* K2   */  be_nested_str_weak(type),
    /* K3   */  be_nested_str_weak(animation_dsl),
    /* K4   */  be_nested_str_weak(Token),
    /* K5   */  be_nested_str_weak(NEWLINE),
    /* K6   */  be_nested_str_weak(EOF),
    /* K7   */  be_nested_str_weak(next),
    }),
    be_str_weak(skip_statement),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x74060011,  //  0002  JMPT	R1	#0015
      0x8C040101,  //  0003  GETMET	R1	R0	K1
      0x7C040200,  //  0004  CALL	R1	1
      0x88080302,  //  0005  GETMBR	R2	R1	K2
      0xB80E0600,  //  0006  GETNGBL	R3	K3
      0x880C0704,  //  0007  GETMBR	R3	R3	K4
      0x880C0705,  //  0008  GETMBR	R3	R3	K5
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x740A0005,  //  000A  JMPT	R2	#0011
      0x88080302,  //  000B  GETMBR	R2	R1	K2
      0xB80E0600,  //  000C  GETNGBL	R3	K3
      0x880C0704,  //  000D  GETMBR	R3	R3	K4
      0x880C0706,  //  000E  GETMBR	R3	R3	K6
      0x1C080403,  //  000F  EQ	R2	R2	R3
      0x780A0000,  //  0010  JMPF	R2	#0012
      0x70020002,  //  0011  JMP		#0015
      0x8C080107,  //  0012  GETMET	R2	R0	K7
      0x7C080200,  //  0013  CALL	R2	1
      0x7001FFEA,  //  0014  JMP		#0000
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_log_call
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_log_call,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(fluent),
    /* K1   */  be_nested_str_weak(def_X20_X28engine_X29_X20log_X28f_X22_X25s_X22_X2C_X203_X29_X20end),
    /* K2   */  be_nested_str_weak(_X25s_X2Epush_closure_step_X28_X25s_X29_X25s),
    /* K3   */  be_nested_str_weak(get_indent),
    /* K4   */  be_nested_str_weak(CONTEXT_EXPRESSION),
    /* K5   */  be_nested_str_weak(log_X28f_X22_X25s_X22_X2C_X203_X29),
    /* K6   */  be_nested_str_weak(log_X28f_X22_X25s_X22_X2C_X203_X29_X25s),
    }),
    be_str_weak(process_log_call),
    &be_const_str_solidified,
    ( &(const binstruction[31]) {  /* code */
      0x1C100500,  //  0000  EQ	R4	R2	K0
      0x7812000C,  //  0001  JMPF	R4	#000F
      0x60100018,  //  0002  GETGBL	R4	G24
      0x58140001,  //  0003  LDCONST	R5	K1
      0x5C180200,  //  0004  MOVE	R6	R1
      0x7C100400,  //  0005  CALL	R4	2
      0x60140018,  //  0006  GETGBL	R5	G24
      0x58180002,  //  0007  LDCONST	R6	K2
      0x8C1C0103,  //  0008  GETMET	R7	R0	K3
      0x7C1C0200,  //  0009  CALL	R7	1
      0x5C200800,  //  000A  MOVE	R8	R4
      0x5C240600,  //  000B  MOVE	R9	R3
      0x7C140800,  //  000C  CALL	R5	4
      0x80040A00,  //  000D  RET	1	R5
      0x7002000E,  //  000E  JMP		#001E
      0x88100104,  //  000F  GETMBR	R4	R0	K4
      0x1C100404,  //  0010  EQ	R4	R2	R4
      0x78120005,  //  0011  JMPF	R4	#0018
      0x60100018,  //  0012  GETGBL	R4	G24
      0x58140005,  //  0013  LDCONST	R5	K5
      0x5C180200,  //  0014  MOVE	R6	R1
      0x7C100400,  //  0015  CALL	R4	2
      0x80040800,  //  0016  RET	1	R4
      0x70020005,  //  0017  JMP		#001E
      0x60100018,  //  0018  GETGBL	R4	G24
      0x58140006,  //  0019  LDCONST	R5	K6
      0x5C180200,  //  001A  MOVE	R6	R1
      0x5C1C0600,  //  001B  MOVE	R7	R3
      0x7C100600,  //  001C  CALL	R4	3
      0x80040800,  //  001D  RET	1	R4
      0x80000000,  //  001E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: expect_identifier
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_expect_identifier,   /* name */
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
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(IDENTIFIER),
    /* K5   */  be_nested_str_weak(COLOR),
    /* K6   */  be_nested_str_weak(KEYWORD),
    /* K7   */  be_nested_str_weak(can_use_as_identifier),
    /* K8   */  be_nested_str_weak(value),
    /* K9   */  be_nested_str_weak(next),
    /* K10  */  be_nested_str_weak(error),
    /* K11  */  be_nested_str_weak(Expected_X20identifier),
    /* K12  */  be_nested_str_weak(unknown),
    }),
    be_str_weak(expect_identifier),
    &be_const_str_solidified,
    ( &(const binstruction[37]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A001A,  //  0004  JMPF	R2	#0020
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x740A000F,  //  000A  JMPT	R2	#001B
      0x88080301,  //  000B  GETMBR	R2	R1	K1
      0xB80E0400,  //  000C  GETNGBL	R3	K2
      0x880C0703,  //  000D  GETMBR	R3	R3	K3
      0x880C0705,  //  000E  GETMBR	R3	R3	K5
      0x1C080403,  //  000F  EQ	R2	R2	R3
      0x740A0009,  //  0010  JMPT	R2	#001B
      0x88080301,  //  0011  GETMBR	R2	R1	K1
      0xB80E0400,  //  0012  GETNGBL	R3	K2
      0x880C0703,  //  0013  GETMBR	R3	R3	K3
      0x880C0706,  //  0014  GETMBR	R3	R3	K6
      0x1C080403,  //  0015  EQ	R2	R2	R3
      0x780A0008,  //  0016  JMPF	R2	#0020
      0x8C080107,  //  0017  GETMET	R2	R0	K7
      0x88100308,  //  0018  GETMBR	R4	R1	K8
      0x7C080400,  //  0019  CALL	R2	2
      0x780A0004,  //  001A  JMPF	R2	#0020
      0x88080308,  //  001B  GETMBR	R2	R1	K8
      0x8C0C0109,  //  001C  GETMET	R3	R0	K9
      0x7C0C0200,  //  001D  CALL	R3	1
      0x80040400,  //  001E  RET	1	R2
      0x70020003,  //  001F  JMP		#0024
      0x8C08010A,  //  0020  GETMET	R2	R0	K10
      0x5810000B,  //  0021  LDCONST	R4	K11
      0x7C080400,  //  0022  CALL	R2	2
      0x80061800,  //  0023  RET	1	K12
      0x80000000,  //  0024  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: skip_whitespace
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_skip_whitespace,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(at_end),
    /* K1   */  be_nested_str_weak(current),
    /* K2   */  be_nested_str_weak(type),
    /* K3   */  be_nested_str_weak(animation_dsl),
    /* K4   */  be_nested_str_weak(Token),
    /* K5   */  be_nested_str_weak(NEWLINE),
    /* K6   */  be_nested_str_weak(COMMENT),
    /* K7   */  be_nested_str_weak(next),
    }),
    be_str_weak(skip_whitespace),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x74060015,  //  0002  JMPT	R1	#0019
      0x8C040101,  //  0003  GETMET	R1	R0	K1
      0x7C040200,  //  0004  CALL	R1	1
      0x4C080000,  //  0005  LDNIL	R2
      0x20080202,  //  0006  NE	R2	R1	R2
      0x780A000E,  //  0007  JMPF	R2	#0017
      0x88080302,  //  0008  GETMBR	R2	R1	K2
      0xB80E0600,  //  0009  GETNGBL	R3	K3
      0x880C0704,  //  000A  GETMBR	R3	R3	K4
      0x880C0705,  //  000B  GETMBR	R3	R3	K5
      0x1C080403,  //  000C  EQ	R2	R2	R3
      0x740A0005,  //  000D  JMPT	R2	#0014
      0x88080302,  //  000E  GETMBR	R2	R1	K2
      0xB80E0600,  //  000F  GETNGBL	R3	K3
      0x880C0704,  //  0010  GETMBR	R3	R3	K4
      0x880C0706,  //  0011  GETMBR	R3	R3	K6
      0x1C080403,  //  0012  EQ	R2	R2	R3
      0x780A0002,  //  0013  JMPF	R2	#0017
      0x8C080107,  //  0014  GETMET	R2	R0	K7
      0x7C080200,  //  0015  CALL	R2	1
      0x70020000,  //  0016  JMP		#0018
      0x70020000,  //  0017  JMP		#0019
      0x7001FFE6,  //  0018  JMP		#0000
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_add,   /* name */
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
    /* K0   */  be_nested_str_weak(output),
    /* K1   */  be_nested_str_weak(push),
    }),
    be_str_weak(add),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_value
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_value,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[21]) {     /* constants */
    /* K0   */  be_nested_str_weak(process_additive_expression),
    /* K1   */  be_nested_str_weak(_X3E_X20process_value_X20context_X3D_X25s_X20result_X3D_X25s),
    /* K2   */  be_nested_str_weak(CONTEXT_VARIABLE),
    /* K3   */  be_nested_str_weak(CONTEXT_PROPERTY),
    /* K4   */  be_nested_str_weak(needs_closure),
    /* K5   */  be_nested_str_weak(CONTEXT_REPEAT_COUNT),
    /* K6   */  be_nested_str_weak(needs_function),
    /* K7   */  be_nested_str_weak(string),
    /* K8   */  be_nested_str_weak(find),
    /* K9   */  be_nested_str_weak(expr),
    /* K10  */  be_nested_str_weak(_X2E),
    /* K11  */  be_const_int(0),
    /* K12  */  be_nested_str_weak(create_simple_function_from_string),
    /* K13  */  be_nested_str_weak(ExpressionResult),
    /* K14  */  be_nested_str_weak(function_call),
    /* K15  */  be_nested_str_weak(create_computation_closure_from_string),
    /* K16  */  be_nested_str_weak(symbol_table),
    /* K17  */  be_nested_str_weak(get),
    /* K18  */  be_nested_str_weak(closure_value),
    /* K19  */  be_nested_str_weak(type),
    /* K20  */  be_nested_str_weak(instance),
    }),
    be_str_weak(process_value),
    &be_const_str_solidified,
    ( &(const binstruction[65]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0x50140200,  //  0002  LDBOOL	R5	1	0
      0x50180000,  //  0003  LDBOOL	R6	0	0
      0x7C080800,  //  0004  CALL	R2	4
      0x600C0001,  //  0005  GETGBL	R3	G1
      0x60100018,  //  0006  GETGBL	R4	G24
      0x58140001,  //  0007  LDCONST	R5	K1
      0x5C180200,  //  0008  MOVE	R6	R1
      0x5C1C0400,  //  0009  MOVE	R7	R2
      0x7C100600,  //  000A  CALL	R4	3
      0x7C0C0200,  //  000B  CALL	R3	1
      0x880C0102,  //  000C  GETMBR	R3	R0	K2
      0x1C0C0203,  //  000D  EQ	R3	R1	R3
      0x740E0002,  //  000E  JMPT	R3	#0012
      0x880C0103,  //  000F  GETMBR	R3	R0	K3
      0x1C0C0203,  //  0010  EQ	R3	R1	R3
      0x780E0002,  //  0011  JMPF	R3	#0015
      0x8C0C0504,  //  0012  GETMET	R3	R2	K4
      0x7C0C0200,  //  0013  CALL	R3	1
      0x740E0005,  //  0014  JMPT	R3	#001B
      0x880C0105,  //  0015  GETMBR	R3	R0	K5
      0x1C0C0203,  //  0016  EQ	R3	R1	R3
      0x780E0026,  //  0017  JMPF	R3	#003F
      0x8C0C0506,  //  0018  GETMET	R3	R2	K6
      0x7C0C0200,  //  0019  CALL	R3	1
      0x780E0023,  //  001A  JMPF	R3	#003F
      0x880C0105,  //  001B  GETMBR	R3	R0	K5
      0x1C0C0203,  //  001C  EQ	R3	R1	R3
      0x780E0011,  //  001D  JMPF	R3	#0030
      0xA40E0E00,  //  001E  IMPORT	R3	K7
      0x8C100708,  //  001F  GETMET	R4	R3	K8
      0x88180509,  //  0020  GETMBR	R6	R2	K9
      0x581C000A,  //  0021  LDCONST	R7	K10
      0x7C100600,  //  0022  CALL	R4	3
      0x2810090B,  //  0023  GE	R4	R4	K11
      0x78120008,  //  0024  JMPF	R4	#002E
      0x8C10010C,  //  0025  GETMET	R4	R0	K12
      0x88180509,  //  0026  GETMBR	R6	R2	K9
      0x7C100400,  //  0027  CALL	R4	2
      0x8814010D,  //  0028  GETMBR	R5	R0	K13
      0x8C140B0E,  //  0029  GETMET	R5	R5	K14
      0x5C1C0800,  //  002A  MOVE	R7	R4
      0x7C140400,  //  002B  CALL	R5	2
      0x80040A00,  //  002C  RET	1	R5
      0x70020000,  //  002D  JMP		#002F
      0x80040400,  //  002E  RET	1	R2
      0x7002000D,  //  002F  JMP		#003E
      0x8C0C010F,  //  0030  GETMET	R3	R0	K15
      0x88140509,  //  0031  GETMBR	R5	R2	K9
      0x7C0C0400,  //  0032  CALL	R3	2
      0x88100110,  //  0033  GETMBR	R4	R0	K16
      0x8C100911,  //  0034  GETMET	R4	R4	K17
      0x58180012,  //  0035  LDCONST	R6	K18
      0x7C100400,  //  0036  CALL	R4	2
      0x8814010D,  //  0037  GETMBR	R5	R0	K13
      0x8C140B0E,  //  0038  GETMET	R5	R5	K14
      0x5C1C0600,  //  0039  MOVE	R7	R3
      0x88200913,  //  003A  GETMBR	R8	R4	K19
      0x88240914,  //  003B  GETMBR	R9	R4	K20
      0x7C140800,  //  003C  CALL	R5	4
      0x80040A00,  //  003D  RET	1	R5
      0x70020000,  //  003E  JMP		#0040
      0x80040400,  //  003F  RET	1	R2
      0x80000000,  //  0040  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_init,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(tokens),
    /* K1   */  be_nested_str_weak(pos),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(output),
    /* K4   */  be_nested_str_weak(errors),
    /* K5   */  be_nested_str_weak(run_statements),
    /* K6   */  be_nested_str_weak(strip_initialized),
    /* K7   */  be_nested_str_weak(symbol_table),
    /* K8   */  be_nested_str_weak(animation_dsl),
    /* K9   */  be_nested_str_weak(_symbol_table),
    /* K10  */  be_nested_str_weak(indent_level),
    /* K11  */  be_nested_str_weak(has_template_calls),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x20080202,  //  0001  NE	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x5C080200,  //  0003  MOVE	R2	R1
      0x70020001,  //  0004  JMP		#0007
      0x60080012,  //  0005  GETGBL	R2	G18
      0x7C080000,  //  0006  CALL	R2	0
      0x90020002,  //  0007  SETMBR	R0	K0	R2
      0x90020302,  //  0008  SETMBR	R0	K1	K2
      0x60080012,  //  0009  GETGBL	R2	G18
      0x7C080000,  //  000A  CALL	R2	0
      0x90020602,  //  000B  SETMBR	R0	K3	R2
      0x60080012,  //  000C  GETGBL	R2	G18
      0x7C080000,  //  000D  CALL	R2	0
      0x90020802,  //  000E  SETMBR	R0	K4	R2
      0x60080012,  //  000F  GETGBL	R2	G18
      0x7C080000,  //  0010  CALL	R2	0
      0x90020A02,  //  0011  SETMBR	R0	K5	R2
      0x50080000,  //  0012  LDBOOL	R2	0	0
      0x90020C02,  //  0013  SETMBR	R0	K6	R2
      0xB80A1000,  //  0014  GETNGBL	R2	K8
      0x8C080509,  //  0015  GETMET	R2	R2	K9
      0x7C080200,  //  0016  CALL	R2	1
      0x90020E02,  //  0017  SETMBR	R0	K7	R2
      0x90021502,  //  0018  SETMBR	R0	K10	K2
      0x50080000,  //  0019  LDBOOL	R2	0	0
      0x90021602,  //  001A  SETMBR	R0	K11	R2
      0x80000000,  //  001B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_palette
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_palette,   /* name */
  be_nested_proto(
    18,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[49]) {     /* constants */
    /* K0   */  be_nested_str_weak(next),
    /* K1   */  be_nested_str_weak(expect_identifier),
    /* K2   */  be_nested_str_weak(validate_user_name),
    /* K3   */  be_nested_str_weak(palette),
    /* K4   */  be_nested_str_weak(skip_statement),
    /* K5   */  be_nested_str_weak(expect_assign),
    /* K6   */  be_nested_str_weak(expect_left_bracket),
    /* K7   */  be_nested_str_weak(skip_whitespace_including_newlines),
    /* K8   */  be_nested_str_weak(check_right_bracket),
    /* K9   */  be_nested_str_weak(error),
    /* K10  */  be_nested_str_weak(Empty_X20palettes_X20are_X20not_X20allowed_X2E_X20A_X20palette_X20must_X20contain_X20at_X20least_X20one_X20color_X20entry_X2E),
    /* K11  */  be_nested_str_weak(current),
    /* K12  */  be_nested_str_weak(type),
    /* K13  */  be_nested_str_weak(animation_dsl),
    /* K14  */  be_nested_str_weak(Token),
    /* K15  */  be_nested_str_weak(LEFT_PAREN),
    /* K16  */  be_nested_str_weak(at_end),
    /* K17  */  be_nested_str_weak(Cannot_X20mix_X20alternative_X20syntax_X20_X5Bcolor1_X2C_X20color2_X2C_X20_X2E_X2E_X2E_X5D_X20with_X20tuple_X20syntax_X20_X28value_X2C_X20color_X29_X2E_X20Use_X20only_X20one_X20syntax_X20per_X20palette_X2E),
    /* K18  */  be_nested_str_weak(expect_left_paren),
    /* K19  */  be_nested_str_weak(expect_number),
    /* K20  */  be_nested_str_weak(expect_comma),
    /* K21  */  be_nested_str_weak(process_palette_color),
    /* K22  */  be_nested_str_weak(expect_right_paren),
    /* K23  */  be_nested_str_weak(convert_to_vrgb),
    /* K24  */  be_nested_str_weak(0x_X25s),
    /* K25  */  be_nested_str_weak(push),
    /* K26  */  be_nested_str_weak(Cannot_X20mix_X20tuple_X20syntax_X20_X28value_X2C_X20color_X29_X20with_X20alternative_X20syntax_X20_X5Bcolor1_X2C_X20color2_X2C_X20_X2E_X2E_X2E_X5D_X2E_X20Use_X20only_X20one_X20syntax_X20per_X20palette_X2E),
    /* K27  */  be_nested_str_weak(),
    /* K28  */  be_nested_str_weak(COMMA),
    /* K29  */  be_nested_str_weak(COMMENT),
    /* K30  */  be_nested_str_weak(value),
    /* K31  */  be_nested_str_weak(NEWLINE),
    /* K32  */  be_nested_str_weak(Expected_X20_X27_X2C_X27_X20or_X20_X27_X5D_X27_X20in_X20palette_X20definition),
    /* K33  */  be_nested_str_weak(expect_right_bracket),
    /* K34  */  be_nested_str_weak(collect_inline_comment),
    /* K35  */  be_nested_str_weak(stop_iteration),
    /* K36  */  be_nested_str_weak(add),
    /* K37  */  be_nested_str_weak(var_X20_X25s__X20_X3D_X20bytes_X28_X25s),
    /* K38  */  be_const_int(0),
    /* K39  */  be_const_int(1),
    /* K40  */  be_nested_str_weak(_X2508X),
    /* K41  */  be_nested_str_weak(_X20_X20_X25s),
    /* K42  */  be_nested_str_weak(_X20_X20_X22_X25s_X22_X25s),
    /* K43  */  be_nested_str_weak(_X29),
    /* K44  */  be_nested_str_weak(_X20),
    /* K45  */  be_nested_str_weak(_X22_X25s_X22),
    /* K46  */  be_nested_str_weak(var_X20_X25s__X20_X3D_X20bytes_X28_X25s_X29_X25s),
    /* K47  */  be_nested_str_weak(symbol_table),
    /* K48  */  be_nested_str_weak(create_palette),
    }),
    be_str_weak(process_palette),
    &be_const_str_solidified,
    ( &(const binstruction[349]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x7C040200,  //  0003  CALL	R1	1
      0x8C080102,  //  0004  GETMET	R2	R0	K2
      0x5C100200,  //  0005  MOVE	R4	R1
      0x58140003,  //  0006  LDCONST	R5	K3
      0x7C080600,  //  0007  CALL	R2	3
      0x740A0002,  //  0008  JMPT	R2	#000C
      0x8C080104,  //  0009  GETMET	R2	R0	K4
      0x7C080200,  //  000A  CALL	R2	1
      0x80000400,  //  000B  RET	0
      0x8C080105,  //  000C  GETMET	R2	R0	K5
      0x7C080200,  //  000D  CALL	R2	1
      0x8C080106,  //  000E  GETMET	R2	R0	K6
      0x7C080200,  //  000F  CALL	R2	1
      0x60080012,  //  0010  GETGBL	R2	G18
      0x7C080000,  //  0011  CALL	R2	0
      0x600C0012,  //  0012  GETGBL	R3	G18
      0x7C0C0000,  //  0013  CALL	R3	0
      0x8C100107,  //  0014  GETMET	R4	R0	K7
      0x7C100200,  //  0015  CALL	R4	1
      0x8C100108,  //  0016  GETMET	R4	R0	K8
      0x7C100200,  //  0017  CALL	R4	1
      0x78120005,  //  0018  JMPF	R4	#001F
      0x8C100109,  //  0019  GETMET	R4	R0	K9
      0x5818000A,  //  001A  LDCONST	R6	K10
      0x7C100400,  //  001B  CALL	R4	2
      0x8C100104,  //  001C  GETMET	R4	R0	K4
      0x7C100200,  //  001D  CALL	R4	1
      0x80000800,  //  001E  RET	0
      0x8C10010B,  //  001F  GETMET	R4	R0	K11
      0x7C100200,  //  0020  CALL	R4	1
      0x4C140000,  //  0021  LDNIL	R5
      0x20100805,  //  0022  NE	R4	R4	R5
      0x78120007,  //  0023  JMPF	R4	#002C
      0x8C10010B,  //  0024  GETMET	R4	R0	K11
      0x7C100200,  //  0025  CALL	R4	1
      0x8810090C,  //  0026  GETMBR	R4	R4	K12
      0xB8161A00,  //  0027  GETNGBL	R5	K13
      0x88140B0E,  //  0028  GETMBR	R5	R5	K14
      0x88140B0F,  //  0029  GETMBR	R5	R5	K15
      0x1C100805,  //  002A  EQ	R4	R4	R5
      0x74120000,  //  002B  JMPT	R4	#002D
      0x50100001,  //  002C  LDBOOL	R4	0	1
      0x50100200,  //  002D  LDBOOL	R4	1	0
      0x8C140110,  //  002E  GETMET	R5	R0	K16
      0x7C140200,  //  002F  CALL	R5	1
      0x741600BF,  //  0030  JMPT	R5	#00F1
      0x8C140108,  //  0031  GETMET	R5	R0	K8
      0x7C140200,  //  0032  CALL	R5	1
      0x741600BC,  //  0033  JMPT	R5	#00F1
      0x8C140107,  //  0034  GETMET	R5	R0	K7
      0x7C140200,  //  0035  CALL	R5	1
      0x8C140108,  //  0036  GETMET	R5	R0	K8
      0x7C140200,  //  0037  CALL	R5	1
      0x78160000,  //  0038  JMPF	R5	#003A
      0x700200B6,  //  0039  JMP		#00F1
      0x7812002A,  //  003A  JMPF	R4	#0066
      0x8C14010B,  //  003B  GETMET	R5	R0	K11
      0x7C140200,  //  003C  CALL	R5	1
      0x4C180000,  //  003D  LDNIL	R6
      0x20140A06,  //  003E  NE	R5	R5	R6
      0x7816000D,  //  003F  JMPF	R5	#004E
      0x8C14010B,  //  0040  GETMET	R5	R0	K11
      0x7C140200,  //  0041  CALL	R5	1
      0x88140B0C,  //  0042  GETMBR	R5	R5	K12
      0xB81A1A00,  //  0043  GETNGBL	R6	K13
      0x88180D0E,  //  0044  GETMBR	R6	R6	K14
      0x88180D0F,  //  0045  GETMBR	R6	R6	K15
      0x20140A06,  //  0046  NE	R5	R5	R6
      0x78160005,  //  0047  JMPF	R5	#004E
      0x8C140109,  //  0048  GETMET	R5	R0	K9
      0x581C0011,  //  0049  LDCONST	R7	K17
      0x7C140400,  //  004A  CALL	R5	2
      0x8C140104,  //  004B  GETMET	R5	R0	K4
      0x7C140200,  //  004C  CALL	R5	1
      0x80000A00,  //  004D  RET	0
      0x8C140112,  //  004E  GETMET	R5	R0	K18
      0x7C140200,  //  004F  CALL	R5	1
      0x8C140113,  //  0050  GETMET	R5	R0	K19
      0x7C140200,  //  0051  CALL	R5	1
      0x8C180114,  //  0052  GETMET	R6	R0	K20
      0x7C180200,  //  0053  CALL	R6	1
      0x8C180115,  //  0054  GETMET	R6	R0	K21
      0x7C180200,  //  0055  CALL	R6	1
      0x8C1C0116,  //  0056  GETMET	R7	R0	K22
      0x7C1C0200,  //  0057  CALL	R7	1
      0x8C1C0117,  //  0058  GETMET	R7	R0	K23
      0x5C240A00,  //  0059  MOVE	R9	R5
      0x5C280C00,  //  005A  MOVE	R10	R6
      0x7C1C0600,  //  005B  CALL	R7	3
      0x60200009,  //  005C  GETGBL	R8	G9
      0x60240018,  //  005D  GETGBL	R9	G24
      0x58280018,  //  005E  LDCONST	R10	K24
      0x5C2C0E00,  //  005F  MOVE	R11	R7
      0x7C240400,  //  0060  CALL	R9	2
      0x7C200200,  //  0061  CALL	R8	1
      0x8C240519,  //  0062  GETMET	R9	R2	K25
      0x5C2C1000,  //  0063  MOVE	R11	R8
      0x7C240400,  //  0064  CALL	R9	2
      0x70020021,  //  0065  JMP		#0088
      0x8C14010B,  //  0066  GETMET	R5	R0	K11
      0x7C140200,  //  0067  CALL	R5	1
      0x4C180000,  //  0068  LDNIL	R6
      0x20140A06,  //  0069  NE	R5	R5	R6
      0x7816000D,  //  006A  JMPF	R5	#0079
      0x8C14010B,  //  006B  GETMET	R5	R0	K11
      0x7C140200,  //  006C  CALL	R5	1
      0x88140B0C,  //  006D  GETMBR	R5	R5	K12
      0xB81A1A00,  //  006E  GETNGBL	R6	K13
      0x88180D0E,  //  006F  GETMBR	R6	R6	K14
      0x88180D0F,  //  0070  GETMBR	R6	R6	K15
      0x1C140A06,  //  0071  EQ	R5	R5	R6
      0x78160005,  //  0072  JMPF	R5	#0079
      0x8C140109,  //  0073  GETMET	R5	R0	K9
      0x581C001A,  //  0074  LDCONST	R7	K26
      0x7C140400,  //  0075  CALL	R5	2
      0x8C140104,  //  0076  GETMET	R5	R0	K4
      0x7C140200,  //  0077  CALL	R5	1
      0x80000A00,  //  0078  RET	0
      0x8C140115,  //  0079  GETMET	R5	R0	K21
      0x7C140200,  //  007A  CALL	R5	1
      0x8C180117,  //  007B  GETMET	R6	R0	K23
      0x542200FE,  //  007C  LDINT	R8	255
      0x5C240A00,  //  007D  MOVE	R9	R5
      0x7C180600,  //  007E  CALL	R6	3
      0x601C0009,  //  007F  GETGBL	R7	G9
      0x60200018,  //  0080  GETGBL	R8	G24
      0x58240018,  //  0081  LDCONST	R9	K24
      0x5C280C00,  //  0082  MOVE	R10	R6
      0x7C200400,  //  0083  CALL	R8	2
      0x7C1C0200,  //  0084  CALL	R7	1
      0x8C200519,  //  0085  GETMET	R8	R2	K25
      0x5C280E00,  //  0086  MOVE	R10	R7
      0x7C200400,  //  0087  CALL	R8	2
      0x5814001B,  //  0088  LDCONST	R5	K27
      0x8C18010B,  //  0089  GETMET	R6	R0	K11
      0x7C180200,  //  008A  CALL	R6	1
      0x4C1C0000,  //  008B  LDNIL	R7
      0x20180C07,  //  008C  NE	R6	R6	R7
      0x781A002F,  //  008D  JMPF	R6	#00BE
      0x8C18010B,  //  008E  GETMET	R6	R0	K11
      0x7C180200,  //  008F  CALL	R6	1
      0x88180D0C,  //  0090  GETMBR	R6	R6	K12
      0xB81E1A00,  //  0091  GETNGBL	R7	K13
      0x881C0F0E,  //  0092  GETMBR	R7	R7	K14
      0x881C0F1C,  //  0093  GETMBR	R7	R7	K28
      0x1C180C07,  //  0094  EQ	R6	R6	R7
      0x781A0027,  //  0095  JMPF	R6	#00BE
      0x8C180100,  //  0096  GETMET	R6	R0	K0
      0x7C180200,  //  0097  CALL	R6	1
      0x8C18010B,  //  0098  GETMET	R6	R0	K11
      0x7C180200,  //  0099  CALL	R6	1
      0x4C1C0000,  //  009A  LDNIL	R7
      0x20180C07,  //  009B  NE	R6	R6	R7
      0x781A000C,  //  009C  JMPF	R6	#00AA
      0x8C18010B,  //  009D  GETMET	R6	R0	K11
      0x7C180200,  //  009E  CALL	R6	1
      0x88180D0C,  //  009F  GETMBR	R6	R6	K12
      0xB81E1A00,  //  00A0  GETNGBL	R7	K13
      0x881C0F0E,  //  00A1  GETMBR	R7	R7	K14
      0x881C0F1D,  //  00A2  GETMBR	R7	R7	K29
      0x1C180C07,  //  00A3  EQ	R6	R6	R7
      0x781A0004,  //  00A4  JMPF	R6	#00AA
      0x8C18010B,  //  00A5  GETMET	R6	R0	K11
      0x7C180200,  //  00A6  CALL	R6	1
      0x88140D1E,  //  00A7  GETMBR	R5	R6	K30
      0x8C180100,  //  00A8  GETMET	R6	R0	K0
      0x7C180200,  //  00A9  CALL	R6	1
      0x8C180110,  //  00AA  GETMET	R6	R0	K16
      0x7C180200,  //  00AB  CALL	R6	1
      0x741A000F,  //  00AC  JMPT	R6	#00BD
      0x8C18010B,  //  00AD  GETMET	R6	R0	K11
      0x7C180200,  //  00AE  CALL	R6	1
      0x4C1C0000,  //  00AF  LDNIL	R7
      0x201C0C07,  //  00B0  NE	R7	R6	R7
      0x781E0008,  //  00B1  JMPF	R7	#00BB
      0x881C0D0C,  //  00B2  GETMBR	R7	R6	K12
      0xB8221A00,  //  00B3  GETNGBL	R8	K13
      0x8820110E,  //  00B4  GETMBR	R8	R8	K14
      0x8820111F,  //  00B5  GETMBR	R8	R8	K31
      0x1C1C0E08,  //  00B6  EQ	R7	R7	R8
      0x781E0002,  //  00B7  JMPF	R7	#00BB
      0x8C1C0100,  //  00B8  GETMET	R7	R0	K0
      0x7C1C0200,  //  00B9  CALL	R7	1
      0x70020000,  //  00BA  JMP		#00BC
      0x70020000,  //  00BB  JMP		#00BD
      0x7001FFEC,  //  00BC  JMP		#00AA
      0x7002002E,  //  00BD  JMP		#00ED
      0x8C18010B,  //  00BE  GETMET	R6	R0	K11
      0x7C180200,  //  00BF  CALL	R6	1
      0x4C1C0000,  //  00C0  LDNIL	R7
      0x20180C07,  //  00C1  NE	R6	R6	R7
      0x781A000C,  //  00C2  JMPF	R6	#00D0
      0x8C18010B,  //  00C3  GETMET	R6	R0	K11
      0x7C180200,  //  00C4  CALL	R6	1
      0x88180D0C,  //  00C5  GETMBR	R6	R6	K12
      0xB81E1A00,  //  00C6  GETNGBL	R7	K13
      0x881C0F0E,  //  00C7  GETMBR	R7	R7	K14
      0x881C0F1F,  //  00C8  GETMBR	R7	R7	K31
      0x1C180C07,  //  00C9  EQ	R6	R6	R7
      0x781A0004,  //  00CA  JMPF	R6	#00D0
      0x8C180100,  //  00CB  GETMET	R6	R0	K0
      0x7C180200,  //  00CC  CALL	R6	1
      0x8C180107,  //  00CD  GETMET	R6	R0	K7
      0x7C180200,  //  00CE  CALL	R6	1
      0x7002001C,  //  00CF  JMP		#00ED
      0x8C180108,  //  00D0  GETMET	R6	R0	K8
      0x7C180200,  //  00D1  CALL	R6	1
      0x741A0019,  //  00D2  JMPT	R6	#00ED
      0x8C18010B,  //  00D3  GETMET	R6	R0	K11
      0x7C180200,  //  00D4  CALL	R6	1
      0x4C1C0000,  //  00D5  LDNIL	R7
      0x20180C07,  //  00D6  NE	R6	R6	R7
      0x781A000D,  //  00D7  JMPF	R6	#00E6
      0x8C18010B,  //  00D8  GETMET	R6	R0	K11
      0x7C180200,  //  00D9  CALL	R6	1
      0x88180D0C,  //  00DA  GETMBR	R6	R6	K12
      0xB81E1A00,  //  00DB  GETNGBL	R7	K13
      0x881C0F0E,  //  00DC  GETMBR	R7	R7	K14
      0x881C0F1D,  //  00DD  GETMBR	R7	R7	K29
      0x1C180C07,  //  00DE  EQ	R6	R6	R7
      0x781A0005,  //  00DF  JMPF	R6	#00E6
      0x8C18010B,  //  00E0  GETMET	R6	R0	K11
      0x7C180200,  //  00E1  CALL	R6	1
      0x88140D1E,  //  00E2  GETMBR	R5	R6	K30
      0x8C180100,  //  00E3  GETMET	R6	R0	K0
      0x7C180200,  //  00E4  CALL	R6	1
      0x70020006,  //  00E5  JMP		#00ED
      0x8C180108,  //  00E6  GETMET	R6	R0	K8
      0x7C180200,  //  00E7  CALL	R6	1
      0x741A0003,  //  00E8  JMPT	R6	#00ED
      0x8C180109,  //  00E9  GETMET	R6	R0	K9
      0x58200020,  //  00EA  LDCONST	R8	K32
      0x7C180400,  //  00EB  CALL	R6	2
      0x70020003,  //  00EC  JMP		#00F1
      0x8C180719,  //  00ED  GETMET	R6	R3	K25
      0x5C200A00,  //  00EE  MOVE	R8	R5
      0x7C180400,  //  00EF  CALL	R6	2
      0x7001FF3C,  //  00F0  JMP		#002E
      0x8C140121,  //  00F1  GETMET	R5	R0	K33
      0x7C140200,  //  00F2  CALL	R5	1
      0x8C140122,  //  00F3  GETMET	R5	R0	K34
      0x7C140200,  //  00F4  CALL	R5	1
      0x50180000,  //  00F5  LDBOOL	R6	0	0
      0x601C0010,  //  00F6  GETGBL	R7	G16
      0x5C200600,  //  00F7  MOVE	R8	R3
      0x7C1C0200,  //  00F8  CALL	R7	1
      0xA8020008,  //  00F9  EXBLK	0	#0103
      0x5C200E00,  //  00FA  MOVE	R8	R7
      0x7C200000,  //  00FB  CALL	R8	0
      0x2024111B,  //  00FC  NE	R9	R8	K27
      0x78260001,  //  00FD  JMPF	R9	#0100
      0x50180200,  //  00FE  LDBOOL	R6	1	0
      0x70020000,  //  00FF  JMP		#0101
      0x7001FFF8,  //  0100  JMP		#00FA
      0xA8040001,  //  0101  EXBLK	1	1
      0x70020002,  //  0102  JMP		#0106
      0x581C0023,  //  0103  LDCONST	R7	K35
      0xAC1C0200,  //  0104  CATCH	R7	1	0
      0xB0080000,  //  0105  RAISE	2	R0	R0
      0x781A002C,  //  0106  JMPF	R6	#0134
      0x8C1C0124,  //  0107  GETMET	R7	R0	K36
      0x60240018,  //  0108  GETGBL	R9	G24
      0x58280025,  //  0109  LDCONST	R10	K37
      0x5C2C0200,  //  010A  MOVE	R11	R1
      0x5C300A00,  //  010B  MOVE	R12	R5
      0x7C240600,  //  010C  CALL	R9	3
      0x7C1C0400,  //  010D  CALL	R7	2
      0x601C0010,  //  010E  GETGBL	R7	G16
      0x6020000C,  //  010F  GETGBL	R8	G12
      0x5C240400,  //  0110  MOVE	R9	R2
      0x7C200200,  //  0111  CALL	R8	1
      0x04201127,  //  0112  SUB	R8	R8	K39
      0x40224C08,  //  0113  CONNECT	R8	K38	R8
      0x7C1C0200,  //  0114  CALL	R7	1
      0xA8020016,  //  0115  EXBLK	0	#012D
      0x5C200E00,  //  0116  MOVE	R8	R7
      0x7C200000,  //  0117  CALL	R8	0
      0x60240018,  //  0118  GETGBL	R9	G24
      0x58280028,  //  0119  LDCONST	R10	K40
      0x942C0408,  //  011A  GETIDX	R11	R2	R8
      0x7C240400,  //  011B  CALL	R9	2
      0x94280608,  //  011C  GETIDX	R10	R3	R8
      0x202C151B,  //  011D  NE	R11	R10	K27
      0x782E0004,  //  011E  JMPF	R11	#0124
      0x602C0018,  //  011F  GETGBL	R11	G24
      0x58300029,  //  0120  LDCONST	R12	K41
      0x5C341400,  //  0121  MOVE	R13	R10
      0x7C2C0400,  //  0122  CALL	R11	2
      0x70020000,  //  0123  JMP		#0125
      0x582C001B,  //  0124  LDCONST	R11	K27
      0x8C300124,  //  0125  GETMET	R12	R0	K36
      0x60380018,  //  0126  GETGBL	R14	G24
      0x583C002A,  //  0127  LDCONST	R15	K42
      0x5C401200,  //  0128  MOVE	R16	R9
      0x5C441600,  //  0129  MOVE	R17	R11
      0x7C380600,  //  012A  CALL	R14	3
      0x7C300400,  //  012B  CALL	R12	2
      0x7001FFE8,  //  012C  JMP		#0116
      0x581C0023,  //  012D  LDCONST	R7	K35
      0xAC1C0200,  //  012E  CATCH	R7	1	0
      0xB0080000,  //  012F  RAISE	2	R0	R0
      0x8C1C0124,  //  0130  GETMET	R7	R0	K36
      0x5824002B,  //  0131  LDCONST	R9	K43
      0x7C1C0400,  //  0132  CALL	R7	2
      0x70020022,  //  0133  JMP		#0157
      0x581C001B,  //  0134  LDCONST	R7	K27
      0x60200010,  //  0135  GETGBL	R8	G16
      0x6024000C,  //  0136  GETGBL	R9	G12
      0x5C280400,  //  0137  MOVE	R10	R2
      0x7C240200,  //  0138  CALL	R9	1
      0x04241327,  //  0139  SUB	R9	R9	K39
      0x40264C09,  //  013A  CONNECT	R9	K38	R9
      0x7C200200,  //  013B  CALL	R8	1
      0xA802000E,  //  013C  EXBLK	0	#014C
      0x5C241000,  //  013D  MOVE	R9	R8
      0x7C240000,  //  013E  CALL	R9	0
      0x24281326,  //  013F  GT	R10	R9	K38
      0x782A0000,  //  0140  JMPF	R10	#0142
      0x001C0F2C,  //  0141  ADD	R7	R7	K44
      0x60280018,  //  0142  GETGBL	R10	G24
      0x582C0028,  //  0143  LDCONST	R11	K40
      0x94300409,  //  0144  GETIDX	R12	R2	R9
      0x7C280400,  //  0145  CALL	R10	2
      0x602C0018,  //  0146  GETGBL	R11	G24
      0x5830002D,  //  0147  LDCONST	R12	K45
      0x5C341400,  //  0148  MOVE	R13	R10
      0x7C2C0400,  //  0149  CALL	R11	2
      0x001C0E0B,  //  014A  ADD	R7	R7	R11
      0x7001FFF0,  //  014B  JMP		#013D
      0x58200023,  //  014C  LDCONST	R8	K35
      0xAC200200,  //  014D  CATCH	R8	1	0
      0xB0080000,  //  014E  RAISE	2	R0	R0
      0x8C200124,  //  014F  GETMET	R8	R0	K36
      0x60280018,  //  0150  GETGBL	R10	G24
      0x582C002E,  //  0151  LDCONST	R11	K46
      0x5C300200,  //  0152  MOVE	R12	R1
      0x5C340E00,  //  0153  MOVE	R13	R7
      0x5C380A00,  //  0154  MOVE	R14	R5
      0x7C280800,  //  0155  CALL	R10	4
      0x7C200400,  //  0156  CALL	R8	2
      0x881C012F,  //  0157  GETMBR	R7	R0	K47
      0x8C1C0F30,  //  0158  GETMET	R7	R7	K48
      0x5C240200,  //  0159  MOVE	R9	R1
      0x4C280000,  //  015A  LDNIL	R10
      0x7C1C0600,  //  015B  CALL	R7	3
      0x80000000,  //  015C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_error_report
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_get_error_report,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(has_errors),
    /* K1   */  be_nested_str_weak(No_X20compilation_X20errors),
    /* K2   */  be_nested_str_weak(Compilation_X20errors_X3A_X0A),
    /* K3   */  be_nested_str_weak(errors),
    /* K4   */  be_nested_str_weak(_X20_X20),
    /* K5   */  be_nested_str_weak(_X0A),
    /* K6   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(get_error_report),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x74060000,  //  0002  JMPT	R1	#0004
      0x80060200,  //  0003  RET	1	K1
      0x58040002,  //  0004  LDCONST	R1	K2
      0x60080010,  //  0005  GETGBL	R2	G16
      0x880C0103,  //  0006  GETMBR	R3	R0	K3
      0x7C080200,  //  0007  CALL	R2	1
      0xA8020005,  //  0008  EXBLK	0	#000F
      0x5C0C0400,  //  0009  MOVE	R3	R2
      0x7C0C0000,  //  000A  CALL	R3	0
      0x00120803,  //  000B  ADD	R4	K4	R3
      0x00100905,  //  000C  ADD	R4	R4	K5
      0x00040204,  //  000D  ADD	R1	R1	R4
      0x7001FFF9,  //  000E  JMP		#0009
      0x58080006,  //  000F  LDCONST	R2	K6
      0xAC080200,  //  0010  CATCH	R2	1	0
      0xB0080000,  //  0011  RAISE	2	R0	R0
      0x80040200,  //  0012  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _validate_color_provider_factory_exists
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__validate_color_provider_factory_exists,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(symbol_table),
    /* K1   */  be_nested_str_weak(get),
    /* K2   */  be_nested_str_weak(type),
    /* K3   */  be_nested_str_weak(animation_dsl),
    /* K4   */  be_nested_str_weak(_symbol_entry),
    /* K5   */  be_nested_str_weak(TYPE_COLOR_CONSTRUCTOR),
    }),
    be_str_weak(_validate_color_provider_factory_exists),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x4C0C0000,  //  0004  LDNIL	R3
      0x200C0403,  //  0005  NE	R3	R2	R3
      0x780E0005,  //  0006  JMPF	R3	#000D
      0x880C0502,  //  0007  GETMBR	R3	R2	K2
      0xB8120600,  //  0008  GETNGBL	R4	K3
      0x88100904,  //  0009  GETMBR	R4	R4	K4
      0x88100905,  //  000A  GETMBR	R4	R4	K5
      0x1C0C0604,  //  000B  EQ	R3	R3	R4
      0x740E0000,  //  000C  JMPT	R3	#000E
      0x500C0001,  //  000D  LDBOOL	R3	0	1
      0x500C0200,  //  000E  LDBOOL	R3	1	0
      0x80040600,  //  000F  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_array_literal
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_array_literal,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[22]) {     /* constants */
    /* K0   */  be_nested_str_weak(expect_left_bracket),
    /* K1   */  be_nested_str_weak(at_end),
    /* K2   */  be_nested_str_weak(check_right_bracket),
    /* K3   */  be_nested_str_weak(process_value),
    /* K4   */  be_nested_str_weak(CONTEXT_ARRAY_ELEMENT),
    /* K5   */  be_nested_str_weak(push),
    /* K6   */  be_nested_str_weak(expr),
    /* K7   */  be_nested_str_weak(current),
    /* K8   */  be_nested_str_weak(type),
    /* K9   */  be_nested_str_weak(animation_dsl),
    /* K10  */  be_nested_str_weak(Token),
    /* K11  */  be_nested_str_weak(COMMA),
    /* K12  */  be_nested_str_weak(next),
    /* K13  */  be_nested_str_weak(error),
    /* K14  */  be_nested_str_weak(Expected_X20_X27_X2C_X27_X20or_X20_X27_X5D_X27_X20in_X20array_X20literal),
    /* K15  */  be_nested_str_weak(expect_right_bracket),
    /* K16  */  be_nested_str_weak(_X5B),
    /* K17  */  be_const_int(0),
    /* K18  */  be_const_int(1),
    /* K19  */  be_nested_str_weak(_X2C_X20),
    /* K20  */  be_nested_str_weak(stop_iteration),
    /* K21  */  be_nested_str_weak(_X5D),
    }),
    be_str_weak(process_array_literal),
    &be_const_str_solidified,
    ( &(const binstruction[64]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x60040012,  //  0002  GETGBL	R1	G18
      0x7C040000,  //  0003  CALL	R1	0
      0x8C080101,  //  0004  GETMET	R2	R0	K1
      0x7C080200,  //  0005  CALL	R2	1
      0x740A0020,  //  0006  JMPT	R2	#0028
      0x8C080102,  //  0007  GETMET	R2	R0	K2
      0x7C080200,  //  0008  CALL	R2	1
      0x740A001D,  //  0009  JMPT	R2	#0028
      0x8C080103,  //  000A  GETMET	R2	R0	K3
      0x88100104,  //  000B  GETMBR	R4	R0	K4
      0x7C080400,  //  000C  CALL	R2	2
      0x8C0C0305,  //  000D  GETMET	R3	R1	K5
      0x88140506,  //  000E  GETMBR	R5	R2	K6
      0x7C0C0400,  //  000F  CALL	R3	2
      0x8C0C0107,  //  0010  GETMET	R3	R0	K7
      0x7C0C0200,  //  0011  CALL	R3	1
      0x4C100000,  //  0012  LDNIL	R4
      0x200C0604,  //  0013  NE	R3	R3	R4
      0x780E000A,  //  0014  JMPF	R3	#0020
      0x8C0C0107,  //  0015  GETMET	R3	R0	K7
      0x7C0C0200,  //  0016  CALL	R3	1
      0x880C0708,  //  0017  GETMBR	R3	R3	K8
      0xB8121200,  //  0018  GETNGBL	R4	K9
      0x8810090A,  //  0019  GETMBR	R4	R4	K10
      0x8810090B,  //  001A  GETMBR	R4	R4	K11
      0x1C0C0604,  //  001B  EQ	R3	R3	R4
      0x780E0002,  //  001C  JMPF	R3	#0020
      0x8C0C010C,  //  001D  GETMET	R3	R0	K12
      0x7C0C0200,  //  001E  CALL	R3	1
      0x70020006,  //  001F  JMP		#0027
      0x8C0C0102,  //  0020  GETMET	R3	R0	K2
      0x7C0C0200,  //  0021  CALL	R3	1
      0x740E0003,  //  0022  JMPT	R3	#0027
      0x8C0C010D,  //  0023  GETMET	R3	R0	K13
      0x5814000E,  //  0024  LDCONST	R5	K14
      0x7C0C0400,  //  0025  CALL	R3	2
      0x70020000,  //  0026  JMP		#0028
      0x7001FFDB,  //  0027  JMP		#0004
      0x8C08010F,  //  0028  GETMET	R2	R0	K15
      0x7C080200,  //  0029  CALL	R2	1
      0x58080010,  //  002A  LDCONST	R2	K16
      0x600C0010,  //  002B  GETGBL	R3	G16
      0x6010000C,  //  002C  GETGBL	R4	G12
      0x5C140200,  //  002D  MOVE	R5	R1
      0x7C100200,  //  002E  CALL	R4	1
      0x04100912,  //  002F  SUB	R4	R4	K18
      0x40122204,  //  0030  CONNECT	R4	K17	R4
      0x7C0C0200,  //  0031  CALL	R3	1
      0xA8020007,  //  0032  EXBLK	0	#003B
      0x5C100600,  //  0033  MOVE	R4	R3
      0x7C100000,  //  0034  CALL	R4	0
      0x24140911,  //  0035  GT	R5	R4	K17
      0x78160000,  //  0036  JMPF	R5	#0038
      0x00080513,  //  0037  ADD	R2	R2	K19
      0x94140204,  //  0038  GETIDX	R5	R1	R4
      0x00080405,  //  0039  ADD	R2	R2	R5
      0x7001FFF7,  //  003A  JMP		#0033
      0x580C0014,  //  003B  LDCONST	R3	K20
      0xAC0C0200,  //  003C  CATCH	R3	1	0
      0xB0080000,  //  003D  RAISE	2	R0	R0
      0x00080515,  //  003E  ADD	R2	R2	K21
      0x80040400,  //  003F  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: expect_right_brace
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_expect_right_brace,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(RIGHT_BRACE),
    /* K5   */  be_nested_str_weak(next),
    /* K6   */  be_nested_str_weak(error),
    /* K7   */  be_nested_str_weak(Expected_X20_X27_X7D_X27),
    }),
    be_str_weak(expect_right_brace),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0008,  //  0004  JMPF	R2	#000E
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x780A0002,  //  000A  JMPF	R2	#000E
      0x8C080105,  //  000B  GETMET	R2	R0	K5
      0x7C080200,  //  000C  CALL	R2	1
      0x70020002,  //  000D  JMP		#0011
      0x8C080106,  //  000E  GETMET	R2	R0	K6
      0x58100007,  //  000F  LDCONST	R4	K7
      0x7C080400,  //  0010  CALL	R2	2
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_multiplicative_expression
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_multiplicative_expression,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(process_unary_expression),
    /* K1   */  be_nested_str_weak(at_end),
    /* K2   */  be_nested_str_weak(current),
    /* K3   */  be_nested_str_weak(type),
    /* K4   */  be_nested_str_weak(animation_dsl),
    /* K5   */  be_nested_str_weak(Token),
    /* K6   */  be_nested_str_weak(MULTIPLY),
    /* K7   */  be_nested_str_weak(DIVIDE),
    /* K8   */  be_nested_str_weak(value),
    /* K9   */  be_nested_str_weak(next),
    /* K10  */  be_nested_str_weak(ExpressionResult),
    /* K11  */  be_nested_str_weak(combine),
    /* K12  */  be_nested_str_weak(_X25s_X20_X25s_X20_X25s),
    /* K13  */  be_nested_str_weak(expr),
    }),
    be_str_weak(process_multiplicative_expression),
    &be_const_str_solidified,
    ( &(const binstruction[49]) {  /* code */
      0x8C100100,  //  0000  GETMET	R4	R0	K0
      0x5C180200,  //  0001  MOVE	R6	R1
      0x5C1C0400,  //  0002  MOVE	R7	R2
      0x5C200600,  //  0003  MOVE	R8	R3
      0x7C100800,  //  0004  CALL	R4	4
      0x8C140101,  //  0005  GETMET	R5	R0	K1
      0x7C140200,  //  0006  CALL	R5	1
      0x74160027,  //  0007  JMPT	R5	#0030
      0x8C140102,  //  0008  GETMET	R5	R0	K2
      0x7C140200,  //  0009  CALL	R5	1
      0x4C180000,  //  000A  LDNIL	R6
      0x20180A06,  //  000B  NE	R6	R5	R6
      0x781A0020,  //  000C  JMPF	R6	#002E
      0x88180B03,  //  000D  GETMBR	R6	R5	K3
      0xB81E0800,  //  000E  GETNGBL	R7	K4
      0x881C0F05,  //  000F  GETMBR	R7	R7	K5
      0x881C0F06,  //  0010  GETMBR	R7	R7	K6
      0x1C180C07,  //  0011  EQ	R6	R6	R7
      0x741A0005,  //  0012  JMPT	R6	#0019
      0x88180B03,  //  0013  GETMBR	R6	R5	K3
      0xB81E0800,  //  0014  GETNGBL	R7	K4
      0x881C0F05,  //  0015  GETMBR	R7	R7	K5
      0x881C0F07,  //  0016  GETMBR	R7	R7	K7
      0x1C180C07,  //  0017  EQ	R6	R6	R7
      0x781A0014,  //  0018  JMPF	R6	#002E
      0x88180B08,  //  0019  GETMBR	R6	R5	K8
      0x8C1C0109,  //  001A  GETMET	R7	R0	K9
      0x7C1C0200,  //  001B  CALL	R7	1
      0x8C1C0100,  //  001C  GETMET	R7	R0	K0
      0x5C240200,  //  001D  MOVE	R9	R1
      0x50280000,  //  001E  LDBOOL	R10	0	0
      0x5C2C0600,  //  001F  MOVE	R11	R3
      0x7C1C0800,  //  0020  CALL	R7	4
      0x8820010A,  //  0021  GETMBR	R8	R0	K10
      0x8C20110B,  //  0022  GETMET	R8	R8	K11
      0x60280018,  //  0023  GETGBL	R10	G24
      0x582C000C,  //  0024  LDCONST	R11	K12
      0x8830090D,  //  0025  GETMBR	R12	R4	K13
      0x5C340C00,  //  0026  MOVE	R13	R6
      0x88380F0D,  //  0027  GETMBR	R14	R7	K13
      0x7C280800,  //  0028  CALL	R10	4
      0x5C2C0800,  //  0029  MOVE	R11	R4
      0x5C300E00,  //  002A  MOVE	R12	R7
      0x7C200800,  //  002B  CALL	R8	4
      0x5C101000,  //  002C  MOVE	R4	R8
      0x70020000,  //  002D  JMP		#002F
      0x70020000,  //  002E  JMP		#0030
      0x7001FFD4,  //  002F  JMP		#0005
      0x80040800,  //  0030  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_sequence_statement
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_sequence_statement,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[48]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(EOF),
    /* K5   */  be_nested_str_weak(COMMENT),
    /* K6   */  be_nested_str_weak(add),
    /* K7   */  be_nested_str_weak(get_indent),
    /* K8   */  be_nested_str_weak(value),
    /* K9   */  be_nested_str_weak(next),
    /* K10  */  be_nested_str_weak(NEWLINE),
    /* K11  */  be_nested_str_weak(KEYWORD),
    /* K12  */  be_nested_str_weak(play),
    /* K13  */  be_nested_str_weak(process_play_statement_fluent),
    /* K14  */  be_nested_str_weak(wait),
    /* K15  */  be_nested_str_weak(process_wait_statement_fluent),
    /* K16  */  be_nested_str_weak(IDENTIFIER),
    /* K17  */  be_nested_str_weak(log),
    /* K18  */  be_nested_str_weak(process_log_statement_fluent),
    /* K19  */  be_nested_str_weak(restart),
    /* K20  */  be_nested_str_weak(process_restart_statement_fluent),
    /* K21  */  be_nested_str_weak(reset),
    /* K22  */  be_nested_str_weak(error),
    /* K23  */  be_nested_str_weak(_X27reset_X27_X20command_X20is_X20no_X20longer_X20supported_X2E_X20Use_X20_X27restart_X27_X20instead_X2E),
    /* K24  */  be_nested_str_weak(skip_statement),
    /* K25  */  be_nested_str_weak(repeat),
    /* K26  */  be_nested_str_weak(1),
    /* K27  */  be_nested_str_weak(forever),
    /* K28  */  be_nested_str_weak(_X2D1),
    /* K29  */  be_nested_str_weak(process_value),
    /* K30  */  be_nested_str_weak(CONTEXT_REPEAT_COUNT),
    /* K31  */  be_nested_str_weak(expect_keyword),
    /* K32  */  be_nested_str_weak(times),
    /* K33  */  be_nested_str_weak(expr),
    /* K34  */  be_nested_str_weak(expect_left_brace),
    /* K35  */  be_nested_str_weak(_X25s_X2Epush_repeat_subsequence_X28animation_X2ESequenceManager_X28engine_X2C_X20_X25s_X29),
    /* K36  */  be_nested_str_weak(indent_level),
    /* K37  */  be_const_int(1),
    /* K38  */  be_nested_str_weak(at_end),
    /* K39  */  be_nested_str_weak(check_right_brace),
    /* K40  */  be_nested_str_weak(process_sequence_statement),
    /* K41  */  be_nested_str_weak(expect_right_brace),
    /* K42  */  be_nested_str_weak(_X25s_X29),
    /* K43  */  be_nested_str_weak(peek),
    /* K44  */  be_nested_str_weak(DOT),
    /* K45  */  be_nested_str_weak(process_sequence_assignment_fluent),
    /* K46  */  be_nested_str_weak(Unknown_X20command_X20_X27_X25s_X27_X20in_X20sequence_X2E_X20Valid_X20sequence_X20commands_X20are_X3A_X20play_X2C_X20wait_X2C_X20repeat_X2C_X20restart_X2C_X20log_X2C_X20or_X20property_X20assignments_X20_X28object_X2Eproperty_X20_X3D_X20value_X29),
    /* K47  */  be_nested_str_weak(Invalid_X20statement_X20in_X20sequence_X2E_X20Expected_X3A_X20play_X2C_X20wait_X2C_X20repeat_X2C_X20restart_X2C_X20log_X2C_X20or_X20property_X20assignments),
    }),
    be_str_weak(process_sequence_statement),
    &be_const_str_solidified,
    ( &(const binstruction[210]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x1C080202,  //  0003  EQ	R2	R1	R2
      0x740A0005,  //  0004  JMPT	R2	#000B
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x780A0000,  //  000A  JMPF	R2	#000C
      0x80000400,  //  000B  RET	0
      0x88080301,  //  000C  GETMBR	R2	R1	K1
      0xB80E0400,  //  000D  GETNGBL	R3	K2
      0x880C0703,  //  000E  GETMBR	R3	R3	K3
      0x880C0705,  //  000F  GETMBR	R3	R3	K5
      0x1C080403,  //  0010  EQ	R2	R2	R3
      0x780A0008,  //  0011  JMPF	R2	#001B
      0x8C080106,  //  0012  GETMET	R2	R0	K6
      0x8C100107,  //  0013  GETMET	R4	R0	K7
      0x7C100200,  //  0014  CALL	R4	1
      0x88140308,  //  0015  GETMBR	R5	R1	K8
      0x00100805,  //  0016  ADD	R4	R4	R5
      0x7C080400,  //  0017  CALL	R2	2
      0x8C080109,  //  0018  GETMET	R2	R0	K9
      0x7C080200,  //  0019  CALL	R2	1
      0x80000400,  //  001A  RET	0
      0x88080301,  //  001B  GETMBR	R2	R1	K1
      0xB80E0400,  //  001C  GETNGBL	R3	K2
      0x880C0703,  //  001D  GETMBR	R3	R3	K3
      0x880C070A,  //  001E  GETMBR	R3	R3	K10
      0x1C080403,  //  001F  EQ	R2	R2	R3
      0x780A0002,  //  0020  JMPF	R2	#0024
      0x8C080109,  //  0021  GETMET	R2	R0	K9
      0x7C080200,  //  0022  CALL	R2	1
      0x80000400,  //  0023  RET	0
      0x88080301,  //  0024  GETMBR	R2	R1	K1
      0xB80E0400,  //  0025  GETNGBL	R3	K2
      0x880C0703,  //  0026  GETMBR	R3	R3	K3
      0x880C070B,  //  0027  GETMBR	R3	R3	K11
      0x1C080403,  //  0028  EQ	R2	R2	R3
      0x780A0005,  //  0029  JMPF	R2	#0030
      0x88080308,  //  002A  GETMBR	R2	R1	K8
      0x1C08050C,  //  002B  EQ	R2	R2	K12
      0x780A0002,  //  002C  JMPF	R2	#0030
      0x8C08010D,  //  002D  GETMET	R2	R0	K13
      0x7C080200,  //  002E  CALL	R2	1
      0x700200A0,  //  002F  JMP		#00D1
      0x88080301,  //  0030  GETMBR	R2	R1	K1
      0xB80E0400,  //  0031  GETNGBL	R3	K2
      0x880C0703,  //  0032  GETMBR	R3	R3	K3
      0x880C070B,  //  0033  GETMBR	R3	R3	K11
      0x1C080403,  //  0034  EQ	R2	R2	R3
      0x780A0005,  //  0035  JMPF	R2	#003C
      0x88080308,  //  0036  GETMBR	R2	R1	K8
      0x1C08050E,  //  0037  EQ	R2	R2	K14
      0x780A0002,  //  0038  JMPF	R2	#003C
      0x8C08010F,  //  0039  GETMET	R2	R0	K15
      0x7C080200,  //  003A  CALL	R2	1
      0x70020094,  //  003B  JMP		#00D1
      0x88080301,  //  003C  GETMBR	R2	R1	K1
      0xB80E0400,  //  003D  GETNGBL	R3	K2
      0x880C0703,  //  003E  GETMBR	R3	R3	K3
      0x880C0710,  //  003F  GETMBR	R3	R3	K16
      0x1C080403,  //  0040  EQ	R2	R2	R3
      0x780A0005,  //  0041  JMPF	R2	#0048
      0x88080308,  //  0042  GETMBR	R2	R1	K8
      0x1C080511,  //  0043  EQ	R2	R2	K17
      0x780A0002,  //  0044  JMPF	R2	#0048
      0x8C080112,  //  0045  GETMET	R2	R0	K18
      0x7C080200,  //  0046  CALL	R2	1
      0x70020088,  //  0047  JMP		#00D1
      0x88080301,  //  0048  GETMBR	R2	R1	K1
      0xB80E0400,  //  0049  GETNGBL	R3	K2
      0x880C0703,  //  004A  GETMBR	R3	R3	K3
      0x880C070B,  //  004B  GETMBR	R3	R3	K11
      0x1C080403,  //  004C  EQ	R2	R2	R3
      0x780A0005,  //  004D  JMPF	R2	#0054
      0x88080308,  //  004E  GETMBR	R2	R1	K8
      0x1C080513,  //  004F  EQ	R2	R2	K19
      0x780A0002,  //  0050  JMPF	R2	#0054
      0x8C080114,  //  0051  GETMET	R2	R0	K20
      0x7C080200,  //  0052  CALL	R2	1
      0x7002007C,  //  0053  JMP		#00D1
      0x88080301,  //  0054  GETMBR	R2	R1	K1
      0xB80E0400,  //  0055  GETNGBL	R3	K2
      0x880C0703,  //  0056  GETMBR	R3	R3	K3
      0x880C070B,  //  0057  GETMBR	R3	R3	K11
      0x1C080403,  //  0058  EQ	R2	R2	R3
      0x780A0008,  //  0059  JMPF	R2	#0063
      0x88080308,  //  005A  GETMBR	R2	R1	K8
      0x1C080515,  //  005B  EQ	R2	R2	K21
      0x780A0005,  //  005C  JMPF	R2	#0063
      0x8C080116,  //  005D  GETMET	R2	R0	K22
      0x58100017,  //  005E  LDCONST	R4	K23
      0x7C080400,  //  005F  CALL	R2	2
      0x8C080118,  //  0060  GETMET	R2	R0	K24
      0x7C080200,  //  0061  CALL	R2	1
      0x7002006D,  //  0062  JMP		#00D1
      0x88080301,  //  0063  GETMBR	R2	R1	K1
      0xB80E0400,  //  0064  GETNGBL	R3	K2
      0x880C0703,  //  0065  GETMBR	R3	R3	K3
      0x880C070B,  //  0066  GETMBR	R3	R3	K11
      0x1C080403,  //  0067  EQ	R2	R2	R3
      0x780A0041,  //  0068  JMPF	R2	#00AB
      0x88080308,  //  0069  GETMBR	R2	R1	K8
      0x1C080519,  //  006A  EQ	R2	R2	K25
      0x780A003E,  //  006B  JMPF	R2	#00AB
      0x8C080109,  //  006C  GETMET	R2	R0	K9
      0x7C080200,  //  006D  CALL	R2	1
      0x5808001A,  //  006E  LDCONST	R2	K26
      0x8C0C0100,  //  006F  GETMET	R3	R0	K0
      0x7C0C0200,  //  0070  CALL	R3	1
      0x4C100000,  //  0071  LDNIL	R4
      0x20100604,  //  0072  NE	R4	R3	R4
      0x7812000C,  //  0073  JMPF	R4	#0081
      0x88100701,  //  0074  GETMBR	R4	R3	K1
      0xB8160400,  //  0075  GETNGBL	R5	K2
      0x88140B03,  //  0076  GETMBR	R5	R5	K3
      0x88140B0B,  //  0077  GETMBR	R5	R5	K11
      0x1C100805,  //  0078  EQ	R4	R4	R5
      0x78120006,  //  0079  JMPF	R4	#0081
      0x88100708,  //  007A  GETMBR	R4	R3	K8
      0x1C10091B,  //  007B  EQ	R4	R4	K27
      0x78120003,  //  007C  JMPF	R4	#0081
      0x8C100109,  //  007D  GETMET	R4	R0	K9
      0x7C100200,  //  007E  CALL	R4	1
      0x5808001C,  //  007F  LDCONST	R2	K28
      0x70020006,  //  0080  JMP		#0088
      0x8C10011D,  //  0081  GETMET	R4	R0	K29
      0x8818011E,  //  0082  GETMBR	R6	R0	K30
      0x7C100400,  //  0083  CALL	R4	2
      0x8C14011F,  //  0084  GETMET	R5	R0	K31
      0x581C0020,  //  0085  LDCONST	R7	K32
      0x7C140400,  //  0086  CALL	R5	2
      0x88080921,  //  0087  GETMBR	R2	R4	K33
      0x8C100122,  //  0088  GETMET	R4	R0	K34
      0x7C100200,  //  0089  CALL	R4	1
      0x8C100106,  //  008A  GETMET	R4	R0	K6
      0x60180018,  //  008B  GETGBL	R6	G24
      0x581C0023,  //  008C  LDCONST	R7	K35
      0x8C200107,  //  008D  GETMET	R8	R0	K7
      0x7C200200,  //  008E  CALL	R8	1
      0x5C240400,  //  008F  MOVE	R9	R2
      0x7C180600,  //  0090  CALL	R6	3
      0x7C100400,  //  0091  CALL	R4	2
      0x88100124,  //  0092  GETMBR	R4	R0	K36
      0x00100925,  //  0093  ADD	R4	R4	K37
      0x90024804,  //  0094  SETMBR	R0	K36	R4
      0x8C100126,  //  0095  GETMET	R4	R0	K38
      0x7C100200,  //  0096  CALL	R4	1
      0x74120005,  //  0097  JMPT	R4	#009E
      0x8C100127,  //  0098  GETMET	R4	R0	K39
      0x7C100200,  //  0099  CALL	R4	1
      0x74120002,  //  009A  JMPT	R4	#009E
      0x8C100128,  //  009B  GETMET	R4	R0	K40
      0x7C100200,  //  009C  CALL	R4	1
      0x7001FFF6,  //  009D  JMP		#0095
      0x8C100129,  //  009E  GETMET	R4	R0	K41
      0x7C100200,  //  009F  CALL	R4	1
      0x8C100106,  //  00A0  GETMET	R4	R0	K6
      0x60180018,  //  00A1  GETGBL	R6	G24
      0x581C002A,  //  00A2  LDCONST	R7	K42
      0x8C200107,  //  00A3  GETMET	R8	R0	K7
      0x7C200200,  //  00A4  CALL	R8	1
      0x7C180400,  //  00A5  CALL	R6	2
      0x7C100400,  //  00A6  CALL	R4	2
      0x88100124,  //  00A7  GETMBR	R4	R0	K36
      0x04100925,  //  00A8  SUB	R4	R4	K37
      0x90024804,  //  00A9  SETMBR	R0	K36	R4
      0x70020025,  //  00AA  JMP		#00D1
      0x88080301,  //  00AB  GETMBR	R2	R1	K1
      0xB80E0400,  //  00AC  GETNGBL	R3	K2
      0x880C0703,  //  00AD  GETMBR	R3	R3	K3
      0x880C0710,  //  00AE  GETMBR	R3	R3	K16
      0x1C080403,  //  00AF  EQ	R2	R2	R3
      0x780A0018,  //  00B0  JMPF	R2	#00CA
      0x8C08012B,  //  00B1  GETMET	R2	R0	K43
      0x7C080200,  //  00B2  CALL	R2	1
      0x4C0C0000,  //  00B3  LDNIL	R3
      0x20080403,  //  00B4  NE	R2	R2	R3
      0x780A000A,  //  00B5  JMPF	R2	#00C1
      0x8C08012B,  //  00B6  GETMET	R2	R0	K43
      0x7C080200,  //  00B7  CALL	R2	1
      0x88080501,  //  00B8  GETMBR	R2	R2	K1
      0xB80E0400,  //  00B9  GETNGBL	R3	K2
      0x880C0703,  //  00BA  GETMBR	R3	R3	K3
      0x880C072C,  //  00BB  GETMBR	R3	R3	K44
      0x1C080403,  //  00BC  EQ	R2	R2	R3
      0x780A0002,  //  00BD  JMPF	R2	#00C1
      0x8C08012D,  //  00BE  GETMET	R2	R0	K45
      0x7C080200,  //  00BF  CALL	R2	1
      0x70020007,  //  00C0  JMP		#00C9
      0x8C080116,  //  00C1  GETMET	R2	R0	K22
      0x60100018,  //  00C2  GETGBL	R4	G24
      0x5814002E,  //  00C3  LDCONST	R5	K46
      0x88180308,  //  00C4  GETMBR	R6	R1	K8
      0x7C100400,  //  00C5  CALL	R4	2
      0x7C080400,  //  00C6  CALL	R2	2
      0x8C080118,  //  00C7  GETMET	R2	R0	K24
      0x7C080200,  //  00C8  CALL	R2	1
      0x70020006,  //  00C9  JMP		#00D1
      0x8C080116,  //  00CA  GETMET	R2	R0	K22
      0x60100018,  //  00CB  GETGBL	R4	G24
      0x5814002F,  //  00CC  LDCONST	R5	K47
      0x7C100200,  //  00CD  CALL	R4	1
      0x7C080400,  //  00CE  CALL	R2	2
      0x8C080118,  //  00CF  GETMET	R2	R0	K24
      0x7C080200,  //  00D0  CALL	R2	1
      0x80000000,  //  00D1  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: check_right_bracket
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_check_right_bracket,   /* name */
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
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(RIGHT_BRACKET),
    }),
    be_str_weak(check_right_bracket),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0005,  //  0004  JMPF	R2	#000B
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x740A0000,  //  000A  JMPT	R2	#000C
      0x50080001,  //  000B  LDBOOL	R2	0	1
      0x50080200,  //  000C  LDBOOL	R2	1	0
      0x80040400,  //  000D  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _validate_value_provider_factory_exists
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__validate_value_provider_factory_exists,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(symbol_table),
    /* K1   */  be_nested_str_weak(get),
    /* K2   */  be_nested_str_weak(type),
    /* K3   */  be_nested_str_weak(animation_dsl),
    /* K4   */  be_nested_str_weak(_symbol_entry),
    /* K5   */  be_nested_str_weak(TYPE_VALUE_PROVIDER_CONSTRUCTOR),
    }),
    be_str_weak(_validate_value_provider_factory_exists),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x4C0C0000,  //  0004  LDNIL	R3
      0x200C0403,  //  0005  NE	R3	R2	R3
      0x780E0005,  //  0006  JMPF	R3	#000D
      0x880C0502,  //  0007  GETMBR	R3	R2	K2
      0xB8120600,  //  0008  GETNGBL	R4	K3
      0x88100904,  //  0009  GETMBR	R4	R4	K4
      0x88100905,  //  000A  GETMBR	R4	R4	K5
      0x1C0C0604,  //  000B  EQ	R3	R3	R4
      0x740E0000,  //  000C  JMPT	R3	#000E
      0x500C0001,  //  000D  LDBOOL	R3	0	1
      0x500C0200,  //  000E  LDBOOL	R3	1	0
      0x80040600,  //  000F  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_statement
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_statement,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[45]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(EOF),
    /* K5   */  be_nested_str_weak(COMMENT),
    /* K6   */  be_nested_str_weak(add),
    /* K7   */  be_nested_str_weak(value),
    /* K8   */  be_nested_str_weak(next),
    /* K9   */  be_nested_str_weak(NEWLINE),
    /* K10  */  be_nested_str_weak(KEYWORD),
    /* K11  */  be_nested_str_weak(strip),
    /* K12  */  be_nested_str_weak(error),
    /* K13  */  be_nested_str_weak(_X27strip_X27_X20directive_X20is_X20temporarily_X20disabled_X2E_X20Strip_X20configuration_X20is_X20handled_X20automatically_X2E),
    /* K14  */  be_nested_str_weak(skip_statement),
    /* K15  */  be_nested_str_weak(template),
    /* K16  */  be_nested_str_weak(process_template),
    /* K17  */  be_nested_str_weak(strip_initialized),
    /* K18  */  be_nested_str_weak(generate_default_strip_initialization),
    /* K19  */  be_nested_str_weak(color),
    /* K20  */  be_nested_str_weak(process_color),
    /* K21  */  be_nested_str_weak(palette),
    /* K22  */  be_nested_str_weak(process_palette),
    /* K23  */  be_nested_str_weak(animation),
    /* K24  */  be_nested_str_weak(process_animation),
    /* K25  */  be_nested_str_weak(set),
    /* K26  */  be_nested_str_weak(process_set),
    /* K27  */  be_nested_str_weak(sequence),
    /* K28  */  be_nested_str_weak(process_sequence),
    /* K29  */  be_nested_str_weak(run),
    /* K30  */  be_nested_str_weak(process_run),
    /* K31  */  be_nested_str_weak(import),
    /* K32  */  be_nested_str_weak(process_import),
    /* K33  */  be_nested_str_weak(on),
    /* K34  */  be_nested_str_weak(process_event_handler),
    /* K35  */  be_nested_str_weak(berry),
    /* K36  */  be_nested_str_weak(process_berry_code_block),
    /* K37  */  be_nested_str_weak(Unknown_X20keyword_X20_X27_X25s_X27_X2E),
    /* K38  */  be_nested_str_weak(IDENTIFIER),
    /* K39  */  be_nested_str_weak(log),
    /* K40  */  be_nested_str_weak(peek),
    /* K41  */  be_nested_str_weak(LEFT_PAREN),
    /* K42  */  be_nested_str_weak(process_standalone_log),
    /* K43  */  be_nested_str_weak(process_property_assignment),
    /* K44  */  be_nested_str_weak(Unexpected_X20token_X20_X27_X25s_X27_X2E),
    }),
    be_str_weak(process_statement),
    &be_const_str_solidified,
    ( &(const binstruction[163]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x1C080202,  //  0003  EQ	R2	R1	R2
      0x740A0005,  //  0004  JMPT	R2	#000B
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x780A0000,  //  000A  JMPF	R2	#000C
      0x80000400,  //  000B  RET	0
      0x88080301,  //  000C  GETMBR	R2	R1	K1
      0xB80E0400,  //  000D  GETNGBL	R3	K2
      0x880C0703,  //  000E  GETMBR	R3	R3	K3
      0x880C0705,  //  000F  GETMBR	R3	R3	K5
      0x1C080403,  //  0010  EQ	R2	R2	R3
      0x780A0005,  //  0011  JMPF	R2	#0018
      0x8C080106,  //  0012  GETMET	R2	R0	K6
      0x88100307,  //  0013  GETMBR	R4	R1	K7
      0x7C080400,  //  0014  CALL	R2	2
      0x8C080108,  //  0015  GETMET	R2	R0	K8
      0x7C080200,  //  0016  CALL	R2	1
      0x80000400,  //  0017  RET	0
      0x88080301,  //  0018  GETMBR	R2	R1	K1
      0xB80E0400,  //  0019  GETNGBL	R3	K2
      0x880C0703,  //  001A  GETMBR	R3	R3	K3
      0x880C0709,  //  001B  GETMBR	R3	R3	K9
      0x1C080403,  //  001C  EQ	R2	R2	R3
      0x780A0002,  //  001D  JMPF	R2	#0021
      0x8C080108,  //  001E  GETMET	R2	R0	K8
      0x7C080200,  //  001F  CALL	R2	1
      0x80000400,  //  0020  RET	0
      0x88080301,  //  0021  GETMBR	R2	R1	K1
      0xB80E0400,  //  0022  GETNGBL	R3	K2
      0x880C0703,  //  0023  GETMBR	R3	R3	K3
      0x880C070A,  //  0024  GETMBR	R3	R3	K10
      0x1C080403,  //  0025  EQ	R2	R2	R3
      0x780A0052,  //  0026  JMPF	R2	#007A
      0x88080307,  //  0027  GETMBR	R2	R1	K7
      0x1C08050B,  //  0028  EQ	R2	R2	K11
      0x780A0006,  //  0029  JMPF	R2	#0031
      0x8C08010C,  //  002A  GETMET	R2	R0	K12
      0x5810000D,  //  002B  LDCONST	R4	K13
      0x7C080400,  //  002C  CALL	R2	2
      0x8C08010E,  //  002D  GETMET	R2	R0	K14
      0x7C080200,  //  002E  CALL	R2	1
      0x80000400,  //  002F  RET	0
      0x70020047,  //  0030  JMP		#0079
      0x88080307,  //  0031  GETMBR	R2	R1	K7
      0x1C08050F,  //  0032  EQ	R2	R2	K15
      0x780A0002,  //  0033  JMPF	R2	#0037
      0x8C080110,  //  0034  GETMET	R2	R0	K16
      0x7C080200,  //  0035  CALL	R2	1
      0x70020041,  //  0036  JMP		#0079
      0x88080111,  //  0037  GETMBR	R2	R0	K17
      0x740A0001,  //  0038  JMPT	R2	#003B
      0x8C080112,  //  0039  GETMET	R2	R0	K18
      0x7C080200,  //  003A  CALL	R2	1
      0x88080307,  //  003B  GETMBR	R2	R1	K7
      0x1C080513,  //  003C  EQ	R2	R2	K19
      0x780A0002,  //  003D  JMPF	R2	#0041
      0x8C080114,  //  003E  GETMET	R2	R0	K20
      0x7C080200,  //  003F  CALL	R2	1
      0x70020037,  //  0040  JMP		#0079
      0x88080307,  //  0041  GETMBR	R2	R1	K7
      0x1C080515,  //  0042  EQ	R2	R2	K21
      0x780A0002,  //  0043  JMPF	R2	#0047
      0x8C080116,  //  0044  GETMET	R2	R0	K22
      0x7C080200,  //  0045  CALL	R2	1
      0x70020031,  //  0046  JMP		#0079
      0x88080307,  //  0047  GETMBR	R2	R1	K7
      0x1C080517,  //  0048  EQ	R2	R2	K23
      0x780A0002,  //  0049  JMPF	R2	#004D
      0x8C080118,  //  004A  GETMET	R2	R0	K24
      0x7C080200,  //  004B  CALL	R2	1
      0x7002002B,  //  004C  JMP		#0079
      0x88080307,  //  004D  GETMBR	R2	R1	K7
      0x1C080519,  //  004E  EQ	R2	R2	K25
      0x780A0002,  //  004F  JMPF	R2	#0053
      0x8C08011A,  //  0050  GETMET	R2	R0	K26
      0x7C080200,  //  0051  CALL	R2	1
      0x70020025,  //  0052  JMP		#0079
      0x88080307,  //  0053  GETMBR	R2	R1	K7
      0x1C08051B,  //  0054  EQ	R2	R2	K27
      0x780A0002,  //  0055  JMPF	R2	#0059
      0x8C08011C,  //  0056  GETMET	R2	R0	K28
      0x7C080200,  //  0057  CALL	R2	1
      0x7002001F,  //  0058  JMP		#0079
      0x88080307,  //  0059  GETMBR	R2	R1	K7
      0x1C08051D,  //  005A  EQ	R2	R2	K29
      0x780A0002,  //  005B  JMPF	R2	#005F
      0x8C08011E,  //  005C  GETMET	R2	R0	K30
      0x7C080200,  //  005D  CALL	R2	1
      0x70020019,  //  005E  JMP		#0079
      0x88080307,  //  005F  GETMBR	R2	R1	K7
      0x1C08051F,  //  0060  EQ	R2	R2	K31
      0x780A0002,  //  0061  JMPF	R2	#0065
      0x8C080120,  //  0062  GETMET	R2	R0	K32
      0x7C080200,  //  0063  CALL	R2	1
      0x70020013,  //  0064  JMP		#0079
      0x88080307,  //  0065  GETMBR	R2	R1	K7
      0x1C080521,  //  0066  EQ	R2	R2	K33
      0x780A0002,  //  0067  JMPF	R2	#006B
      0x8C080122,  //  0068  GETMET	R2	R0	K34
      0x7C080200,  //  0069  CALL	R2	1
      0x7002000D,  //  006A  JMP		#0079
      0x88080307,  //  006B  GETMBR	R2	R1	K7
      0x1C080523,  //  006C  EQ	R2	R2	K35
      0x780A0002,  //  006D  JMPF	R2	#0071
      0x8C080124,  //  006E  GETMET	R2	R0	K36
      0x7C080200,  //  006F  CALL	R2	1
      0x70020007,  //  0070  JMP		#0079
      0x8C08010C,  //  0071  GETMET	R2	R0	K12
      0x60100018,  //  0072  GETGBL	R4	G24
      0x58140025,  //  0073  LDCONST	R5	K37
      0x88180307,  //  0074  GETMBR	R6	R1	K7
      0x7C100400,  //  0075  CALL	R4	2
      0x7C080400,  //  0076  CALL	R2	2
      0x8C08010E,  //  0077  GETMET	R2	R0	K14
      0x7C080200,  //  0078  CALL	R2	1
      0x70020027,  //  0079  JMP		#00A2
      0x88080301,  //  007A  GETMBR	R2	R1	K1
      0xB80E0400,  //  007B  GETNGBL	R3	K2
      0x880C0703,  //  007C  GETMBR	R3	R3	K3
      0x880C0726,  //  007D  GETMBR	R3	R3	K38
      0x1C080403,  //  007E  EQ	R2	R2	R3
      0x780A0019,  //  007F  JMPF	R2	#009A
      0x88080111,  //  0080  GETMBR	R2	R0	K17
      0x740A0001,  //  0081  JMPT	R2	#0084
      0x8C080112,  //  0082  GETMET	R2	R0	K18
      0x7C080200,  //  0083  CALL	R2	1
      0x88080307,  //  0084  GETMBR	R2	R1	K7
      0x1C080527,  //  0085  EQ	R2	R2	K39
      0x780A000F,  //  0086  JMPF	R2	#0097
      0x8C080128,  //  0087  GETMET	R2	R0	K40
      0x7C080200,  //  0088  CALL	R2	1
      0x4C0C0000,  //  0089  LDNIL	R3
      0x20080403,  //  008A  NE	R2	R2	R3
      0x780A000A,  //  008B  JMPF	R2	#0097
      0x8C080128,  //  008C  GETMET	R2	R0	K40
      0x7C080200,  //  008D  CALL	R2	1
      0x88080501,  //  008E  GETMBR	R2	R2	K1
      0xB80E0400,  //  008F  GETNGBL	R3	K2
      0x880C0703,  //  0090  GETMBR	R3	R3	K3
      0x880C0729,  //  0091  GETMBR	R3	R3	K41
      0x1C080403,  //  0092  EQ	R2	R2	R3
      0x780A0002,  //  0093  JMPF	R2	#0097
      0x8C08012A,  //  0094  GETMET	R2	R0	K42
      0x7C080200,  //  0095  CALL	R2	1
      0x70020001,  //  0096  JMP		#0099
      0x8C08012B,  //  0097  GETMET	R2	R0	K43
      0x7C080200,  //  0098  CALL	R2	1
      0x70020007,  //  0099  JMP		#00A2
      0x8C08010C,  //  009A  GETMET	R2	R0	K12
      0x60100018,  //  009B  GETGBL	R4	G24
      0x5814002C,  //  009C  LDCONST	R5	K44
      0x88180307,  //  009D  GETMBR	R6	R1	K7
      0x7C100400,  //  009E  CALL	R4	2
      0x7C080400,  //  009F  CALL	R2	2
      0x8C08010E,  //  00A0  GETMET	R2	R0	K14
      0x7C080200,  //  00A1  CALL	R2	1
      0x80000000,  //  00A2  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_event_parameters
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_event_parameters,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(expect_left_paren),
    /* K1   */  be_nested_str_weak(_X7B),
    /* K2   */  be_nested_str_weak(at_end),
    /* K3   */  be_nested_str_weak(check_right_paren),
    /* K4   */  be_nested_str_weak(current),
    /* K5   */  be_nested_str_weak(type),
    /* K6   */  be_nested_str_weak(animation_dsl),
    /* K7   */  be_nested_str_weak(Token),
    /* K8   */  be_nested_str_weak(TIME),
    /* K9   */  be_nested_str_weak(process_time_value),
    /* K10  */  be_nested_str_weak(_X22interval_X22_X3A_X20_X25s),
    /* K11  */  be_nested_str_weak(process_value),
    /* K12  */  be_nested_str_weak(event_param),
    /* K13  */  be_nested_str_weak(_X22value_X22_X3A_X20_X25s),
    /* K14  */  be_nested_str_weak(expr),
    /* K15  */  be_nested_str_weak(expect_right_paren),
    /* K16  */  be_nested_str_weak(_X7D),
    }),
    be_str_weak(process_event_parameters),
    &be_const_str_solidified,
    ( &(const binstruction[40]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x58040001,  //  0002  LDCONST	R1	K1
      0x8C080102,  //  0003  GETMET	R2	R0	K2
      0x7C080200,  //  0004  CALL	R2	1
      0x740A001D,  //  0005  JMPT	R2	#0024
      0x8C080103,  //  0006  GETMET	R2	R0	K3
      0x7C080200,  //  0007  CALL	R2	1
      0x740A001A,  //  0008  JMPT	R2	#0024
      0x8C080104,  //  0009  GETMET	R2	R0	K4
      0x7C080200,  //  000A  CALL	R2	1
      0x4C0C0000,  //  000B  LDNIL	R3
      0x200C0403,  //  000C  NE	R3	R2	R3
      0x780E000D,  //  000D  JMPF	R3	#001C
      0x880C0505,  //  000E  GETMBR	R3	R2	K5
      0xB8120C00,  //  000F  GETNGBL	R4	K6
      0x88100907,  //  0010  GETMBR	R4	R4	K7
      0x88100908,  //  0011  GETMBR	R4	R4	K8
      0x1C0C0604,  //  0012  EQ	R3	R3	R4
      0x780E0007,  //  0013  JMPF	R3	#001C
      0x8C0C0109,  //  0014  GETMET	R3	R0	K9
      0x7C0C0200,  //  0015  CALL	R3	1
      0x60100018,  //  0016  GETGBL	R4	G24
      0x5814000A,  //  0017  LDCONST	R5	K10
      0x5C180600,  //  0018  MOVE	R6	R3
      0x7C100400,  //  0019  CALL	R4	2
      0x00040204,  //  001A  ADD	R1	R1	R4
      0x70020007,  //  001B  JMP		#0024
      0x8C0C010B,  //  001C  GETMET	R3	R0	K11
      0x5814000C,  //  001D  LDCONST	R5	K12
      0x7C0C0400,  //  001E  CALL	R3	2
      0x60100018,  //  001F  GETGBL	R4	G24
      0x5814000D,  //  0020  LDCONST	R5	K13
      0x8818070E,  //  0021  GETMBR	R6	R3	K14
      0x7C100400,  //  0022  CALL	R4	2
      0x00040204,  //  0023  ADD	R1	R1	R4
      0x8C08010F,  //  0024  GETMET	R2	R0	K15
      0x7C080200,  //  0025  CALL	R2	1
      0x00040310,  //  0026  ADD	R1	R1	K16
      0x80040200,  //  0027  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_named_color_value
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_get_named_color_value,   /* name */
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
    /* K0   */  be_nested_str_weak(symbol_table),
    /* K1   */  be_nested_str_weak(get_reference),
    }),
    be_str_weak(get_named_color_value),
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
** Solidified function: expect_number
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_expect_number,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(NUMBER),
    /* K5   */  be_nested_str_weak(value),
    /* K6   */  be_nested_str_weak(next),
    /* K7   */  be_nested_str_weak(error),
    /* K8   */  be_nested_str_weak(Expected_X20number),
    /* K9   */  be_nested_str_weak(0),
    }),
    be_str_weak(expect_number),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A000A,  //  0004  JMPF	R2	#0010
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x780A0004,  //  000A  JMPF	R2	#0010
      0x88080305,  //  000B  GETMBR	R2	R1	K5
      0x8C0C0106,  //  000C  GETMET	R3	R0	K6
      0x7C0C0200,  //  000D  CALL	R3	1
      0x80040400,  //  000E  RET	1	R2
      0x70020003,  //  000F  JMP		#0014
      0x8C080107,  //  0010  GETMET	R2	R0	K7
      0x58100008,  //  0011  LDCONST	R4	K8
      0x7C080400,  //  0012  CALL	R2	2
      0x80061200,  //  0013  RET	1	K9
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_function_arguments
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_function_arguments,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[23]) {     /* constants */
    /* K0   */  be_nested_str_weak(expect_left_paren),
    /* K1   */  be_nested_str_weak(at_end),
    /* K2   */  be_nested_str_weak(check_right_paren),
    /* K3   */  be_nested_str_weak(skip_whitespace),
    /* K4   */  be_nested_str_weak(process_additive_expression),
    /* K5   */  be_nested_str_weak(CONTEXT_ARGUMENT),
    /* K6   */  be_nested_str_weak(expr),
    /* K7   */  be_nested_str_weak(process_value),
    /* K8   */  be_nested_str_weak(push),
    /* K9   */  be_nested_str_weak(current),
    /* K10  */  be_nested_str_weak(type),
    /* K11  */  be_nested_str_weak(animation_dsl),
    /* K12  */  be_nested_str_weak(Token),
    /* K13  */  be_nested_str_weak(COMMA),
    /* K14  */  be_nested_str_weak(next),
    /* K15  */  be_nested_str_weak(error),
    /* K16  */  be_nested_str_weak(Expected_X20_X27_X2C_X27_X20or_X20_X27_X29_X27_X20in_X20function_X20arguments),
    /* K17  */  be_nested_str_weak(expect_right_paren),
    /* K18  */  be_nested_str_weak(),
    /* K19  */  be_const_int(0),
    /* K20  */  be_const_int(1),
    /* K21  */  be_nested_str_weak(_X2C_X20),
    /* K22  */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(process_function_arguments),
    &be_const_str_solidified,
    ( &(const binstruction[83]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x7C080200,  //  0001  CALL	R2	1
      0x60080012,  //  0002  GETGBL	R2	G18
      0x7C080000,  //  0003  CALL	R2	0
      0x8C0C0101,  //  0004  GETMET	R3	R0	K1
      0x7C0C0200,  //  0005  CALL	R3	1
      0x740E0034,  //  0006  JMPT	R3	#003C
      0x8C0C0102,  //  0007  GETMET	R3	R0	K2
      0x7C0C0200,  //  0008  CALL	R3	1
      0x740E0031,  //  0009  JMPT	R3	#003C
      0x8C0C0103,  //  000A  GETMET	R3	R0	K3
      0x7C0C0200,  //  000B  CALL	R3	1
      0x8C0C0102,  //  000C  GETMET	R3	R0	K2
      0x7C0C0200,  //  000D  CALL	R3	1
      0x780E0000,  //  000E  JMPF	R3	#0010
      0x7002002B,  //  000F  JMP		#003C
      0x4C0C0000,  //  0010  LDNIL	R3
      0x78060006,  //  0011  JMPF	R1	#0019
      0x8C100104,  //  0012  GETMET	R4	R0	K4
      0x88180105,  //  0013  GETMBR	R6	R0	K5
      0x501C0200,  //  0014  LDBOOL	R7	1	0
      0x50200200,  //  0015  LDBOOL	R8	1	0
      0x7C100800,  //  0016  CALL	R4	4
      0x880C0906,  //  0017  GETMBR	R3	R4	K6
      0x70020003,  //  0018  JMP		#001D
      0x8C100107,  //  0019  GETMET	R4	R0	K7
      0x88180105,  //  001A  GETMBR	R6	R0	K5
      0x7C100400,  //  001B  CALL	R4	2
      0x880C0906,  //  001C  GETMBR	R3	R4	K6
      0x8C100508,  //  001D  GETMET	R4	R2	K8
      0x5C180600,  //  001E  MOVE	R6	R3
      0x7C100400,  //  001F  CALL	R4	2
      0x8C100103,  //  0020  GETMET	R4	R0	K3
      0x7C100200,  //  0021  CALL	R4	1
      0x8C100109,  //  0022  GETMET	R4	R0	K9
      0x7C100200,  //  0023  CALL	R4	1
      0x4C140000,  //  0024  LDNIL	R5
      0x20100805,  //  0025  NE	R4	R4	R5
      0x7812000C,  //  0026  JMPF	R4	#0034
      0x8C100109,  //  0027  GETMET	R4	R0	K9
      0x7C100200,  //  0028  CALL	R4	1
      0x8810090A,  //  0029  GETMBR	R4	R4	K10
      0xB8161600,  //  002A  GETNGBL	R5	K11
      0x88140B0C,  //  002B  GETMBR	R5	R5	K12
      0x88140B0D,  //  002C  GETMBR	R5	R5	K13
      0x1C100805,  //  002D  EQ	R4	R4	R5
      0x78120004,  //  002E  JMPF	R4	#0034
      0x8C10010E,  //  002F  GETMET	R4	R0	K14
      0x7C100200,  //  0030  CALL	R4	1
      0x8C100103,  //  0031  GETMET	R4	R0	K3
      0x7C100200,  //  0032  CALL	R4	1
      0x70020006,  //  0033  JMP		#003B
      0x8C100102,  //  0034  GETMET	R4	R0	K2
      0x7C100200,  //  0035  CALL	R4	1
      0x74120003,  //  0036  JMPT	R4	#003B
      0x8C10010F,  //  0037  GETMET	R4	R0	K15
      0x58180010,  //  0038  LDCONST	R6	K16
      0x7C100400,  //  0039  CALL	R4	2
      0x70020000,  //  003A  JMP		#003C
      0x7001FFC7,  //  003B  JMP		#0004
      0x8C0C0111,  //  003C  GETMET	R3	R0	K17
      0x7C0C0200,  //  003D  CALL	R3	1
      0x580C0012,  //  003E  LDCONST	R3	K18
      0x60100010,  //  003F  GETGBL	R4	G16
      0x6014000C,  //  0040  GETGBL	R5	G12
      0x5C180400,  //  0041  MOVE	R6	R2
      0x7C140200,  //  0042  CALL	R5	1
      0x04140B14,  //  0043  SUB	R5	R5	K20
      0x40162605,  //  0044  CONNECT	R5	K19	R5
      0x7C100200,  //  0045  CALL	R4	1
      0xA8020007,  //  0046  EXBLK	0	#004F
      0x5C140800,  //  0047  MOVE	R5	R4
      0x7C140000,  //  0048  CALL	R5	0
      0x24180B13,  //  0049  GT	R6	R5	K19
      0x781A0000,  //  004A  JMPF	R6	#004C
      0x000C0715,  //  004B  ADD	R3	R3	K21
      0x94180405,  //  004C  GETIDX	R6	R2	R5
      0x000C0606,  //  004D  ADD	R3	R3	R6
      0x7001FFF7,  //  004E  JMP		#0047
      0x58100016,  //  004F  LDCONST	R4	K22
      0xAC100200,  //  0050  CATCH	R4	1	0
      0xB0080000,  //  0051  RAISE	2	R0	R0
      0x80040600,  //  0052  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_import
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_import,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(next),
    /* K1   */  be_nested_str_weak(expect_identifier),
    /* K2   */  be_nested_str_weak(collect_inline_comment),
    /* K3   */  be_nested_str_weak(add),
    /* K4   */  be_nested_str_weak(import_X20_X25s_X20_X25s),
    }),
    be_str_weak(process_import),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x7C040200,  //  0003  CALL	R1	1
      0x8C080102,  //  0004  GETMET	R2	R0	K2
      0x7C080200,  //  0005  CALL	R2	1
      0x8C0C0103,  //  0006  GETMET	R3	R0	K3
      0x60140018,  //  0007  GETGBL	R5	G24
      0x58180004,  //  0008  LDCONST	R6	K4
      0x5C1C0200,  //  0009  MOVE	R7	R1
      0x5C200400,  //  000A  MOVE	R8	R2
      0x7C140600,  //  000B  CALL	R5	3
      0x7C0C0400,  //  000C  CALL	R3	2
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _process_parameters_core
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__process_parameters_core,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[30]) {     /* constants */
    /* K0   */  be_nested_str_weak(CONTEXT_VARIABLE),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(find),
    /* K3   */  be_nested_str_weak(temp_),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(split),
    /* K6   */  be_nested_str_weak(_),
    /* K7   */  be_const_int(2),
    /* K8   */  be_const_int(1),
    /* K9   */  be_nested_str_weak(),
    /* K10  */  be_nested_str_weak(_create_instance_for_validation),
    /* K11  */  be_nested_str_weak(at_end),
    /* K12  */  be_nested_str_weak(check_right_paren),
    /* K13  */  be_nested_str_weak(skip_whitespace_including_newlines),
    /* K14  */  be_nested_str_weak(expect_identifier),
    /* K15  */  be_nested_str_weak(_validate_single_parameter),
    /* K16  */  be_nested_str_weak(expect_assign),
    /* K17  */  be_nested_str_weak(process_value),
    /* K18  */  be_nested_str_weak(collect_inline_comment),
    /* K19  */  be_nested_str_weak(expr),
    /* K20  */  be_nested_str_weak(current),
    /* K21  */  be_nested_str_weak(type),
    /* K22  */  be_nested_str_weak(animation_dsl),
    /* K23  */  be_nested_str_weak(Token),
    /* K24  */  be_nested_str_weak(COMMENT),
    /* K25  */  be_nested_str_weak(next),
    /* K26  */  be_nested_str_weak(COMMA),
    /* K27  */  be_nested_str_weak(NEWLINE),
    /* K28  */  be_nested_str_weak(error),
    /* K29  */  be_nested_str_weak(Expected_X20_X27_X2C_X27_X20or_X20_X27_X29_X27_X20in_X20function_X20arguments),
    }),
    be_str_weak(_process_parameters_core),
    &be_const_str_solidified,
    ( &(const binstruction[128]) {  /* code */
      0x4C100000,  //  0000  LDNIL	R4
      0x5C140200,  //  0001  MOVE	R5	R1
      0x88180100,  //  0002  GETMBR	R6	R0	K0
      0x1C180406,  //  0003  EQ	R6	R2	R6
      0x781A0010,  //  0004  JMPF	R6	#0016
      0xA41A0200,  //  0005  IMPORT	R6	K1
      0x8C1C0D02,  //  0006  GETMET	R7	R6	K2
      0x5C240200,  //  0007  MOVE	R9	R1
      0x58280003,  //  0008  LDCONST	R10	K3
      0x7C1C0600,  //  0009  CALL	R7	3
      0x1C1C0F04,  //  000A  EQ	R7	R7	K4
      0x781E0009,  //  000B  JMPF	R7	#0016
      0x8C1C0D05,  //  000C  GETMET	R7	R6	K5
      0x5C240200,  //  000D  MOVE	R9	R1
      0x58280006,  //  000E  LDCONST	R10	K6
      0x7C1C0600,  //  000F  CALL	R7	3
      0x6020000C,  //  0010  GETGBL	R8	G12
      0x5C240E00,  //  0011  MOVE	R9	R7
      0x7C200200,  //  0012  CALL	R8	1
      0x28201107,  //  0013  GE	R8	R8	K7
      0x78220000,  //  0014  JMPF	R8	#0016
      0x94140F08,  //  0015  GETIDX	R5	R7	K8
      0x20180B09,  //  0016  NE	R6	R5	K9
      0x781A0003,  //  0017  JMPF	R6	#001C
      0x8C18010A,  //  0018  GETMET	R6	R0	K10
      0x5C200A00,  //  0019  MOVE	R8	R5
      0x7C180400,  //  001A  CALL	R6	2
      0x5C100C00,  //  001B  MOVE	R4	R6
      0x8C18010B,  //  001C  GETMET	R6	R0	K11
      0x7C180200,  //  001D  CALL	R6	1
      0x741A005F,  //  001E  JMPT	R6	#007F
      0x8C18010C,  //  001F  GETMET	R6	R0	K12
      0x7C180200,  //  0020  CALL	R6	1
      0x741A005C,  //  0021  JMPT	R6	#007F
      0x8C18010D,  //  0022  GETMET	R6	R0	K13
      0x7C180200,  //  0023  CALL	R6	1
      0x8C18010C,  //  0024  GETMET	R6	R0	K12
      0x7C180200,  //  0025  CALL	R6	1
      0x781A0000,  //  0026  JMPF	R6	#0028
      0x70020056,  //  0027  JMP		#007F
      0x8C18010E,  //  0028  GETMET	R6	R0	K14
      0x7C180200,  //  0029  CALL	R6	1
      0x4C1C0000,  //  002A  LDNIL	R7
      0x201C0807,  //  002B  NE	R7	R4	R7
      0x781E0006,  //  002C  JMPF	R7	#0034
      0x201C0B09,  //  002D  NE	R7	R5	K9
      0x781E0004,  //  002E  JMPF	R7	#0034
      0x8C1C010F,  //  002F  GETMET	R7	R0	K15
      0x5C240A00,  //  0030  MOVE	R9	R5
      0x5C280C00,  //  0031  MOVE	R10	R6
      0x5C2C0800,  //  0032  MOVE	R11	R4
      0x7C1C0800,  //  0033  CALL	R7	4
      0x8C1C0110,  //  0034  GETMET	R7	R0	K16
      0x7C1C0200,  //  0035  CALL	R7	1
      0x8C1C0111,  //  0036  GETMET	R7	R0	K17
      0x88240100,  //  0037  GETMBR	R9	R0	K0
      0x7C1C0400,  //  0038  CALL	R7	2
      0x8C200112,  //  0039  GETMET	R8	R0	K18
      0x7C200200,  //  003A  CALL	R8	1
      0x5C240600,  //  003B  MOVE	R9	R3
      0x5C280C00,  //  003C  MOVE	R10	R6
      0x882C0F13,  //  003D  GETMBR	R11	R7	K19
      0x5C301000,  //  003E  MOVE	R12	R8
      0x7C240600,  //  003F  CALL	R9	3
      0x8C24010B,  //  0040  GETMET	R9	R0	K11
      0x7C240200,  //  0041  CALL	R9	1
      0x7426000F,  //  0042  JMPT	R9	#0053
      0x8C240114,  //  0043  GETMET	R9	R0	K20
      0x7C240200,  //  0044  CALL	R9	1
      0x4C280000,  //  0045  LDNIL	R10
      0x2028120A,  //  0046  NE	R10	R9	R10
      0x782A0008,  //  0047  JMPF	R10	#0051
      0x88281315,  //  0048  GETMBR	R10	R9	K21
      0xB82E2C00,  //  0049  GETNGBL	R11	K22
      0x882C1717,  //  004A  GETMBR	R11	R11	K23
      0x882C1718,  //  004B  GETMBR	R11	R11	K24
      0x1C28140B,  //  004C  EQ	R10	R10	R11
      0x782A0002,  //  004D  JMPF	R10	#0051
      0x8C280119,  //  004E  GETMET	R10	R0	K25
      0x7C280200,  //  004F  CALL	R10	1
      0x70020000,  //  0050  JMP		#0052
      0x70020000,  //  0051  JMP		#0053
      0x7001FFEC,  //  0052  JMP		#0040
      0x8C240114,  //  0053  GETMET	R9	R0	K20
      0x7C240200,  //  0054  CALL	R9	1
      0x4C280000,  //  0055  LDNIL	R10
      0x2024120A,  //  0056  NE	R9	R9	R10
      0x7826000C,  //  0057  JMPF	R9	#0065
      0x8C240114,  //  0058  GETMET	R9	R0	K20
      0x7C240200,  //  0059  CALL	R9	1
      0x88241315,  //  005A  GETMBR	R9	R9	K21
      0xB82A2C00,  //  005B  GETNGBL	R10	K22
      0x88281517,  //  005C  GETMBR	R10	R10	K23
      0x8828151A,  //  005D  GETMBR	R10	R10	K26
      0x1C24120A,  //  005E  EQ	R9	R9	R10
      0x78260004,  //  005F  JMPF	R9	#0065
      0x8C240119,  //  0060  GETMET	R9	R0	K25
      0x7C240200,  //  0061  CALL	R9	1
      0x8C24010D,  //  0062  GETMET	R9	R0	K13
      0x7C240200,  //  0063  CALL	R9	1
      0x70020018,  //  0064  JMP		#007E
      0x8C240114,  //  0065  GETMET	R9	R0	K20
      0x7C240200,  //  0066  CALL	R9	1
      0x4C280000,  //  0067  LDNIL	R10
      0x2024120A,  //  0068  NE	R9	R9	R10
      0x7826000C,  //  0069  JMPF	R9	#0077
      0x8C240114,  //  006A  GETMET	R9	R0	K20
      0x7C240200,  //  006B  CALL	R9	1
      0x88241315,  //  006C  GETMBR	R9	R9	K21
      0xB82A2C00,  //  006D  GETNGBL	R10	K22
      0x88281517,  //  006E  GETMBR	R10	R10	K23
      0x8828151B,  //  006F  GETMBR	R10	R10	K27
      0x1C24120A,  //  0070  EQ	R9	R9	R10
      0x78260004,  //  0071  JMPF	R9	#0077
      0x8C240119,  //  0072  GETMET	R9	R0	K25
      0x7C240200,  //  0073  CALL	R9	1
      0x8C24010D,  //  0074  GETMET	R9	R0	K13
      0x7C240200,  //  0075  CALL	R9	1
      0x70020006,  //  0076  JMP		#007E
      0x8C24010C,  //  0077  GETMET	R9	R0	K12
      0x7C240200,  //  0078  CALL	R9	1
      0x74260003,  //  0079  JMPT	R9	#007E
      0x8C24011C,  //  007A  GETMET	R9	R0	K28
      0x582C001D,  //  007B  LDCONST	R11	K29
      0x7C240400,  //  007C  CALL	R9	2
      0x70020000,  //  007D  JMP		#007F
      0x7001FF9C,  //  007E  JMP		#001C
      0x80000000,  //  007F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_color
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_color,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        6,                          /* nstack */
        2,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 2]) {     /* constants */
        /* K0   */  be_nested_str_weak(symbol_table),
        /* K1   */  be_nested_str_weak(create_color),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 7]) {  /* code */
          0x68080000,  //  0000  GETUPV	R2	U0
          0x88080500,  //  0001  GETMBR	R2	R2	K0
          0x8C080501,  //  0002  GETMET	R2	R2	K1
          0x5C100000,  //  0003  MOVE	R4	R0
          0x5C140200,  //  0004  MOVE	R5	R1
          0x7C080600,  //  0005  CALL	R2	3
          0x80040400,  //  0006  RET	1	R2
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[39]) {     /* constants */
    /* K0   */  be_nested_str_weak(next),
    /* K1   */  be_nested_str_weak(expect_identifier),
    /* K2   */  be_nested_str_weak(validate_user_name),
    /* K3   */  be_nested_str_weak(color),
    /* K4   */  be_nested_str_weak(skip_statement),
    /* K5   */  be_nested_str_weak(expect_assign),
    /* K6   */  be_nested_str_weak(current),
    /* K7   */  be_nested_str_weak(type),
    /* K8   */  be_nested_str_weak(animation_dsl),
    /* K9   */  be_nested_str_weak(Token),
    /* K10  */  be_nested_str_weak(KEYWORD),
    /* K11  */  be_nested_str_weak(IDENTIFIER),
    /* K12  */  be_nested_str_weak(peek),
    /* K13  */  be_nested_str_weak(LEFT_PAREN),
    /* K14  */  be_nested_str_weak(value),
    /* K15  */  be_nested_str_weak(),
    /* K16  */  be_nested_str_weak(COMMENT),
    /* K17  */  be_nested_str_weak(_X20_X20),
    /* K18  */  be_nested_str_weak(symbol_table),
    /* K19  */  be_nested_str_weak(get),
    /* K20  */  be_nested_str_weak(_symbol_entry),
    /* K21  */  be_nested_str_weak(TYPE_TEMPLATE),
    /* K22  */  be_nested_str_weak(process_function_arguments),
    /* K23  */  be_nested_str_weak(engine_X2C_X20_X25s),
    /* K24  */  be_nested_str_weak(engine),
    /* K25  */  be_nested_str_weak(add),
    /* K26  */  be_nested_str_weak(var_X20_X25s__X20_X3D_X20_X25s_template_X28_X25s_X29_X25s),
    /* K27  */  be_nested_str_weak(create_color),
    /* K28  */  be_nested_str_weak(TYPE_USER_FUNCTION),
    /* K29  */  be_nested_str_weak(var_X20_X25s__X20_X3D_X20animation_X2Eget_user_function_X28_X27_X25s_X27_X29_X28_X25s_X29_X25s),
    /* K30  */  be_nested_str_weak(_validate_color_provider_factory_exists),
    /* K31  */  be_nested_str_weak(error),
    /* K32  */  be_nested_str_weak(Color_X20provider_X20factory_X20function_X20_X27_X25s_X27_X20does_X20not_X20exist_X2E_X20Check_X20the_X20function_X20name_X20and_X20ensure_X20it_X27s_X20available_X20in_X20the_X20animation_X20module_X2E),
    /* K33  */  be_nested_str_weak(var_X20_X25s__X20_X3D_X20animation_X2E_X25s_X28engine_X29_X25s),
    /* K34  */  be_nested_str_weak(_create_instance_for_validation),
    /* K35  */  be_nested_str_weak(_process_named_arguments_for_color_provider),
    /* K36  */  be_nested_str_weak(_X25s_),
    /* K37  */  be_nested_str_weak(_process_simple_value_assignment),
    /* K38  */  be_nested_str_weak(CONTEXT_COLOR),
    }),
    be_str_weak(process_color),
    &be_const_str_solidified,
    ( &(const binstruction[186]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x7C040200,  //  0003  CALL	R1	1
      0x8C080102,  //  0004  GETMET	R2	R0	K2
      0x5C100200,  //  0005  MOVE	R4	R1
      0x58140003,  //  0006  LDCONST	R5	K3
      0x7C080600,  //  0007  CALL	R2	3
      0x740A0002,  //  0008  JMPT	R2	#000C
      0x8C080104,  //  0009  GETMET	R2	R0	K4
      0x7C080200,  //  000A  CALL	R2	1
      0x80000400,  //  000B  RET	0
      0x8C080105,  //  000C  GETMET	R2	R0	K5
      0x7C080200,  //  000D  CALL	R2	1
      0x8C080106,  //  000E  GETMET	R2	R0	K6
      0x7C080200,  //  000F  CALL	R2	1
      0x880C0507,  //  0010  GETMBR	R3	R2	K7
      0xB8121000,  //  0011  GETNGBL	R4	K8
      0x88100909,  //  0012  GETMBR	R4	R4	K9
      0x8810090A,  //  0013  GETMBR	R4	R4	K10
      0x1C0C0604,  //  0014  EQ	R3	R3	R4
      0x740E0005,  //  0015  JMPT	R3	#001C
      0x880C0507,  //  0016  GETMBR	R3	R2	K7
      0xB8121000,  //  0017  GETNGBL	R4	K8
      0x88100909,  //  0018  GETMBR	R4	R4	K9
      0x8810090B,  //  0019  GETMBR	R4	R4	K11
      0x1C0C0604,  //  001A  EQ	R3	R3	R4
      0x780E0096,  //  001B  JMPF	R3	#00B3
      0x8C0C010C,  //  001C  GETMET	R3	R0	K12
      0x7C0C0200,  //  001D  CALL	R3	1
      0x4C100000,  //  001E  LDNIL	R4
      0x200C0604,  //  001F  NE	R3	R3	R4
      0x780E0091,  //  0020  JMPF	R3	#00B3
      0x8C0C010C,  //  0021  GETMET	R3	R0	K12
      0x7C0C0200,  //  0022  CALL	R3	1
      0x880C0707,  //  0023  GETMBR	R3	R3	K7
      0xB8121000,  //  0024  GETNGBL	R4	K8
      0x88100909,  //  0025  GETMBR	R4	R4	K9
      0x8810090D,  //  0026  GETMBR	R4	R4	K13
      0x1C0C0604,  //  0027  EQ	R3	R3	R4
      0x780E0089,  //  0028  JMPF	R3	#00B3
      0x880C050E,  //  0029  GETMBR	R3	R2	K14
      0x8C100100,  //  002A  GETMET	R4	R0	K0
      0x7C100200,  //  002B  CALL	R4	1
      0x5810000F,  //  002C  LDCONST	R4	K15
      0x8C140106,  //  002D  GETMET	R5	R0	K6
      0x7C140200,  //  002E  CALL	R5	1
      0x4C180000,  //  002F  LDNIL	R6
      0x20140A06,  //  0030  NE	R5	R5	R6
      0x7816000E,  //  0031  JMPF	R5	#0041
      0x8C140106,  //  0032  GETMET	R5	R0	K6
      0x7C140200,  //  0033  CALL	R5	1
      0x88140B07,  //  0034  GETMBR	R5	R5	K7
      0xB81A1000,  //  0035  GETNGBL	R6	K8
      0x88180D09,  //  0036  GETMBR	R6	R6	K9
      0x88180D10,  //  0037  GETMBR	R6	R6	K16
      0x1C140A06,  //  0038  EQ	R5	R5	R6
      0x78160006,  //  0039  JMPF	R5	#0041
      0x8C140106,  //  003A  GETMET	R5	R0	K6
      0x7C140200,  //  003B  CALL	R5	1
      0x88140B0E,  //  003C  GETMBR	R5	R5	K14
      0x00162205,  //  003D  ADD	R5	K17	R5
      0x5C100A00,  //  003E  MOVE	R4	R5
      0x8C140100,  //  003F  GETMET	R5	R0	K0
      0x7C140200,  //  0040  CALL	R5	1
      0x88140112,  //  0041  GETMBR	R5	R0	K18
      0x8C140B13,  //  0042  GETMET	R5	R5	K19
      0x5C1C0600,  //  0043  MOVE	R7	R3
      0x7C140400,  //  0044  CALL	R5	2
      0x4C180000,  //  0045  LDNIL	R6
      0x20180A06,  //  0046  NE	R6	R5	R6
      0x781A001F,  //  0047  JMPF	R6	#0068
      0x88180B07,  //  0048  GETMBR	R6	R5	K7
      0xB81E1000,  //  0049  GETNGBL	R7	K8
      0x881C0F14,  //  004A  GETMBR	R7	R7	K20
      0x881C0F15,  //  004B  GETMBR	R7	R7	K21
      0x1C180C07,  //  004C  EQ	R6	R6	R7
      0x781A0019,  //  004D  JMPF	R6	#0068
      0x8C180116,  //  004E  GETMET	R6	R0	K22
      0x50200000,  //  004F  LDBOOL	R8	0	0
      0x7C180400,  //  0050  CALL	R6	2
      0x201C0D0F,  //  0051  NE	R7	R6	K15
      0x781E0004,  //  0052  JMPF	R7	#0058
      0x601C0018,  //  0053  GETGBL	R7	G24
      0x58200017,  //  0054  LDCONST	R8	K23
      0x5C240C00,  //  0055  MOVE	R9	R6
      0x7C1C0400,  //  0056  CALL	R7	2
      0x70020000,  //  0057  JMP		#0059
      0x581C0018,  //  0058  LDCONST	R7	K24
      0x8C200119,  //  0059  GETMET	R8	R0	K25
      0x60280018,  //  005A  GETGBL	R10	G24
      0x582C001A,  //  005B  LDCONST	R11	K26
      0x5C300200,  //  005C  MOVE	R12	R1
      0x5C340600,  //  005D  MOVE	R13	R3
      0x5C380E00,  //  005E  MOVE	R14	R7
      0x5C3C0800,  //  005F  MOVE	R15	R4
      0x7C280A00,  //  0060  CALL	R10	5
      0x7C200400,  //  0061  CALL	R8	2
      0x88200112,  //  0062  GETMBR	R8	R0	K18
      0x8C20111B,  //  0063  GETMET	R8	R8	K27
      0x5C280200,  //  0064  MOVE	R10	R1
      0x4C2C0000,  //  0065  LDNIL	R11
      0x7C200600,  //  0066  CALL	R8	3
      0x70020049,  //  0067  JMP		#00B2
      0x4C180000,  //  0068  LDNIL	R6
      0x20180A06,  //  0069  NE	R6	R5	R6
      0x781A001F,  //  006A  JMPF	R6	#008B
      0x88180B07,  //  006B  GETMBR	R6	R5	K7
      0xB81E1000,  //  006C  GETNGBL	R7	K8
      0x881C0F14,  //  006D  GETMBR	R7	R7	K20
      0x881C0F1C,  //  006E  GETMBR	R7	R7	K28
      0x1C180C07,  //  006F  EQ	R6	R6	R7
      0x781A0019,  //  0070  JMPF	R6	#008B
      0x8C180116,  //  0071  GETMET	R6	R0	K22
      0x50200000,  //  0072  LDBOOL	R8	0	0
      0x7C180400,  //  0073  CALL	R6	2
      0x201C0D0F,  //  0074  NE	R7	R6	K15
      0x781E0004,  //  0075  JMPF	R7	#007B
      0x601C0018,  //  0076  GETGBL	R7	G24
      0x58200017,  //  0077  LDCONST	R8	K23
      0x5C240C00,  //  0078  MOVE	R9	R6
      0x7C1C0400,  //  0079  CALL	R7	2
      0x70020000,  //  007A  JMP		#007C
      0x581C0018,  //  007B  LDCONST	R7	K24
      0x8C200119,  //  007C  GETMET	R8	R0	K25
      0x60280018,  //  007D  GETGBL	R10	G24
      0x582C001D,  //  007E  LDCONST	R11	K29
      0x5C300200,  //  007F  MOVE	R12	R1
      0x5C340600,  //  0080  MOVE	R13	R3
      0x5C380E00,  //  0081  MOVE	R14	R7
      0x5C3C0800,  //  0082  MOVE	R15	R4
      0x7C280A00,  //  0083  CALL	R10	5
      0x7C200400,  //  0084  CALL	R8	2
      0x88200112,  //  0085  GETMBR	R8	R0	K18
      0x8C20111B,  //  0086  GETMET	R8	R8	K27
      0x5C280200,  //  0087  MOVE	R10	R1
      0x4C2C0000,  //  0088  LDNIL	R11
      0x7C200600,  //  0089  CALL	R8	3
      0x70020026,  //  008A  JMP		#00B2
      0x8C18011E,  //  008B  GETMET	R6	R0	K30
      0x5C200600,  //  008C  MOVE	R8	R3
      0x7C180400,  //  008D  CALL	R6	2
      0x741A0008,  //  008E  JMPT	R6	#0098
      0x8C18011F,  //  008F  GETMET	R6	R0	K31
      0x60200018,  //  0090  GETGBL	R8	G24
      0x58240020,  //  0091  LDCONST	R9	K32
      0x5C280600,  //  0092  MOVE	R10	R3
      0x7C200400,  //  0093  CALL	R8	2
      0x7C180400,  //  0094  CALL	R6	2
      0x8C180104,  //  0095  GETMET	R6	R0	K4
      0x7C180200,  //  0096  CALL	R6	1
      0x80000C00,  //  0097  RET	0
      0x8C180119,  //  0098  GETMET	R6	R0	K25
      0x60200018,  //  0099  GETGBL	R8	G24
      0x58240021,  //  009A  LDCONST	R9	K33
      0x5C280200,  //  009B  MOVE	R10	R1
      0x5C2C0600,  //  009C  MOVE	R11	R3
      0x5C300800,  //  009D  MOVE	R12	R4
      0x7C200800,  //  009E  CALL	R8	4
      0x7C180400,  //  009F  CALL	R6	2
      0x8C180122,  //  00A0  GETMET	R6	R0	K34
      0x5C200600,  //  00A1  MOVE	R8	R3
      0x7C180400,  //  00A2  CALL	R6	2
      0x4C1C0000,  //  00A3  LDNIL	R7
      0x201C0C07,  //  00A4  NE	R7	R6	R7
      0x781E0004,  //  00A5  JMPF	R7	#00AB
      0x881C0112,  //  00A6  GETMBR	R7	R0	K18
      0x8C1C0F1B,  //  00A7  GETMET	R7	R7	K27
      0x5C240200,  //  00A8  MOVE	R9	R1
      0x5C280C00,  //  00A9  MOVE	R10	R6
      0x7C1C0600,  //  00AA  CALL	R7	3
      0x8C1C0123,  //  00AB  GETMET	R7	R0	K35
      0x60240018,  //  00AC  GETGBL	R9	G24
      0x58280024,  //  00AD  LDCONST	R10	K36
      0x5C2C0200,  //  00AE  MOVE	R11	R1
      0x7C240400,  //  00AF  CALL	R9	2
      0x5C280600,  //  00B0  MOVE	R10	R3
      0x7C1C0600,  //  00B1  CALL	R7	3
      0x70020004,  //  00B2  JMP		#00B8
      0x8C0C0125,  //  00B3  GETMET	R3	R0	K37
      0x5C140200,  //  00B4  MOVE	R5	R1
      0x88180126,  //  00B5  GETMBR	R6	R0	K38
      0x841C0000,  //  00B6  CLOSURE	R7	P0
      0x7C0C0800,  //  00B7  CALL	R3	4
      0xA0000000,  //  00B8  CLOSE	R0
      0x80000000,  //  00B9  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_symbol_table_report
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_get_symbol_table_report,   /* name */
  be_nested_proto(
    27,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 4]) {
      be_nested_proto(
        10,                          /* nstack */
        1,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 8]) {     /* constants */
        /* K0   */  be_nested_str_weak(_XE2_X9C_X93),
        /* K1   */  be_const_int(1),
        /* K2   */  be_nested_str_weak(_XE2_X9A_XA0_XEF_XB8_X8F),
        /* K3   */  be_const_int(2),
        /* K4   */  be_nested_str_weak(_XE2_X9A_XA0),
        /* K5   */  be_const_int(0),
        /* K6   */  be_nested_str_weak(keys),
        /* K7   */  be_nested_str_weak(stop_iteration),
        }),
        be_str_weak(display_width),
        &be_const_str_solidified,
        ( &(const binstruction[58]) {  /* code */
          0x60040013,  //  0000  GETGBL	R1	G19
          0x7C040000,  //  0001  CALL	R1	0
          0x98060101,  //  0002  SETIDX	R1	K0	K1
          0x98060503,  //  0003  SETIDX	R1	K2	K3
          0x98060901,  //  0004  SETIDX	R1	K4	K1
          0x58080005,  //  0005  LDCONST	R2	K5
          0x580C0005,  //  0006  LDCONST	R3	K5
          0x6010000C,  //  0007  GETGBL	R4	G12
          0x5C140000,  //  0008  MOVE	R5	R0
          0x7C100200,  //  0009  CALL	R4	1
          0x14100604,  //  000A  LT	R4	R3	R4
          0x7812002C,  //  000B  JMPF	R4	#0039
          0x50100000,  //  000C  LDBOOL	R4	0	0
          0x60140010,  //  000D  GETGBL	R5	G16
          0x8C180306,  //  000E  GETMET	R6	R1	K6
          0x7C180200,  //  000F  CALL	R6	1
          0x7C140200,  //  0010  CALL	R5	1
          0xA802001E,  //  0011  EXBLK	0	#0031
          0x5C180A00,  //  0012  MOVE	R6	R5
          0x7C180000,  //  0013  CALL	R6	0
          0x601C000C,  //  0014  GETGBL	R7	G12
          0x5C200C00,  //  0015  MOVE	R8	R6
          0x7C1C0200,  //  0016  CALL	R7	1
          0x001C0607,  //  0017  ADD	R7	R3	R7
          0x6020000C,  //  0018  GETGBL	R8	G12
          0x5C240000,  //  0019  MOVE	R9	R0
          0x7C200200,  //  001A  CALL	R8	1
          0x181C0E08,  //  001B  LE	R7	R7	R8
          0x781E0010,  //  001C  JMPF	R7	#002E
          0x601C000C,  //  001D  GETGBL	R7	G12
          0x5C200C00,  //  001E  MOVE	R8	R6
          0x7C1C0200,  //  001F  CALL	R7	1
          0x001C0607,  //  0020  ADD	R7	R3	R7
          0x041C0F01,  //  0021  SUB	R7	R7	K1
          0x401C0607,  //  0022  CONNECT	R7	R3	R7
          0x941C0007,  //  0023  GETIDX	R7	R0	R7
          0x1C1C0E06,  //  0024  EQ	R7	R7	R6
          0x781E0007,  //  0025  JMPF	R7	#002E
          0x941C0206,  //  0026  GETIDX	R7	R1	R6
          0x00080407,  //  0027  ADD	R2	R2	R7
          0x601C000C,  //  0028  GETGBL	R7	G12
          0x5C200C00,  //  0029  MOVE	R8	R6
          0x7C1C0200,  //  002A  CALL	R7	1
          0x000C0607,  //  002B  ADD	R3	R3	R7
          0x50100200,  //  002C  LDBOOL	R4	1	0
          0x70020000,  //  002D  JMP		#002F
          0x7001FFE2,  //  002E  JMP		#0012
          0xA8040001,  //  002F  EXBLK	1	1
          0x70020002,  //  0030  JMP		#0034
          0x58140007,  //  0031  LDCONST	R5	K7
          0xAC140200,  //  0032  CATCH	R5	1	0
          0xB0080000,  //  0033  RAISE	2	R0	R0
          0x5C140800,  //  0034  MOVE	R5	R4
          0x74160001,  //  0035  JMPT	R5	#0038
          0x00080501,  //  0036  ADD	R2	R2	K1
          0x000C0701,  //  0037  ADD	R3	R3	K1
          0x7001FFCD,  //  0038  JMP		#0007
          0x80040400,  //  0039  RET	1	R2
        })
      ),
      be_nested_proto(
        8,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 5),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 3]) {     /* constants */
        /* K0   */  be_const_int(1),
        /* K1   */  be_nested_str_weak(name),
        /* K2   */  be_const_int(0),
        }),
        be_str_weak(_sort_symbol_data),
        &be_const_str_solidified,
        ( &(const binstruction[33]) {  /* code */
          0x6000000C,  //  0000  GETGBL	R0	G12
          0x68040000,  //  0001  GETUPV	R1	U0
          0x7C000200,  //  0002  CALL	R0	1
          0x18040100,  //  0003  LE	R1	R0	K0
          0x78060000,  //  0004  JMPF	R1	#0006
          0x80000200,  //  0005  RET	0
          0x58040000,  //  0006  LDCONST	R1	K0
          0x14080200,  //  0007  LT	R2	R1	R0
          0x780A0016,  //  0008  JMPF	R2	#0020
          0x68080000,  //  0009  GETUPV	R2	U0
          0x94080401,  //  000A  GETIDX	R2	R2	R1
          0x940C0501,  //  000B  GETIDX	R3	R2	K1
          0x5C100200,  //  000C  MOVE	R4	R1
          0x24140902,  //  000D  GT	R5	R4	K2
          0x7816000C,  //  000E  JMPF	R5	#001C
          0x04140900,  //  000F  SUB	R5	R4	K0
          0x68180000,  //  0010  GETUPV	R6	U0
          0x94140C05,  //  0011  GETIDX	R5	R6	R5
          0x94140B01,  //  0012  GETIDX	R5	R5	K1
          0x24140A03,  //  0013  GT	R5	R5	R3
          0x78160006,  //  0014  JMPF	R5	#001C
          0x68140000,  //  0015  GETUPV	R5	U0
          0x04180900,  //  0016  SUB	R6	R4	K0
          0x681C0000,  //  0017  GETUPV	R7	U0
          0x94180E06,  //  0018  GETIDX	R6	R7	R6
          0x98140806,  //  0019  SETIDX	R5	R4	R6
          0x04100900,  //  001A  SUB	R4	R4	K0
          0x7001FFF0,  //  001B  JMP		#000D
          0x68140000,  //  001C  GETUPV	R5	U0
          0x98140802,  //  001D  SETIDX	R5	R4	R2
          0x00040300,  //  001E  ADD	R1	R1	K0
          0x7001FFE6,  //  001F  JMP		#0007
          0x80000000,  //  0020  RET	0
        })
      ),
      be_nested_proto(
        4,                          /* nstack */
        2,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 4),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 2]) {     /* constants */
        /* K0   */  be_const_int(0),
        /* K1   */  be_nested_str_weak(_X20),
        }),
        be_str_weak(pad_string),
        &be_const_str_solidified,
        ( &(const binstruction[10]) {  /* code */
          0x68080000,  //  0000  GETUPV	R2	U0
          0x5C0C0000,  //  0001  MOVE	R3	R0
          0x7C080200,  //  0002  CALL	R2	1
          0x04080202,  //  0003  SUB	R2	R1	R2
          0x180C0500,  //  0004  LE	R3	R2	K0
          0x780E0000,  //  0005  JMPF	R3	#0007
          0x80040000,  //  0006  RET	1	R0
          0x080E0202,  //  0007  MUL	R3	K1	R2
          0x000C0003,  //  0008  ADD	R3	R0	R3
          0x80040600,  //  0009  RET	1	R3
        })
      ),
      be_nested_proto(
        7,                          /* nstack */
        2,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 4),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 3]) {     /* constants */
        /* K0   */  be_const_int(0),
        /* K1   */  be_const_int(2),
        /* K2   */  be_nested_str_weak(_X20),
        }),
        be_str_weak(center_string),
        &be_const_str_solidified,
        ( &(const binstruction[14]) {  /* code */
          0x68080000,  //  0000  GETUPV	R2	U0
          0x5C0C0000,  //  0001  MOVE	R3	R0
          0x7C080200,  //  0002  CALL	R2	1
          0x04080202,  //  0003  SUB	R2	R1	R2
          0x180C0500,  //  0004  LE	R3	R2	K0
          0x780E0000,  //  0005  JMPF	R3	#0007
          0x80040000,  //  0006  RET	1	R0
          0x0C0C0501,  //  0007  DIV	R3	R2	K1
          0x04100403,  //  0008  SUB	R4	R2	R3
          0x08160403,  //  0009  MUL	R5	K2	R3
          0x00140A00,  //  000A  ADD	R5	R5	R0
          0x081A0404,  //  000B  MUL	R6	K2	R4
          0x00140A06,  //  000C  ADD	R5	R5	R6
          0x80040A00,  //  000D  RET	1	R5
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[34]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(_X23_X23_X20Symbol_X20Table_X0A_X0A),
    /* K2   */  be_nested_str_weak(symbol_table),
    /* K3   */  be_nested_str_weak(list_symbols),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(No_X20symbols_X20defined_X0A_X0A),
    /* K6   */  be_nested_str_weak(split),
    /* K7   */  be_nested_str_weak(_X3A_X20),
    /* K8   */  be_const_int(2),
    /* K9   */  be_const_int(1),
    /* K10  */  be_nested_str_weak(get),
    /* K11  */  be_nested_str_weak(is_builtin),
    /* K12  */  be_nested_str_weak(_XE2_X9C_X93),
    /* K13  */  be_nested_str_weak(),
    /* K14  */  be_nested_str_weak(is_dangerous_call),
    /* K15  */  be_nested_str_weak(_XE2_X9A_XA0_XEF_XB8_X8F),
    /* K16  */  be_nested_str_weak(takes_args),
    /* K17  */  be_nested_str_weak(_X20_XE2_X9C_X93_X20),
    /* K18  */  be_nested_str_weak(_X60_X25s_X60),
    /* K19  */  be_nested_str_weak(push),
    /* K20  */  be_nested_str_weak(name),
    /* K21  */  be_nested_str_weak(typ),
    /* K22  */  be_nested_str_weak(builtin),
    /* K23  */  be_nested_str_weak(dangerous),
    /* K24  */  be_nested_str_weak(stop_iteration),
    /* K25  */  be_nested_str_weak(_X7C_X20_X25s_X20_X7C_X20_X25s_X20_X7C_X20_X25s_X20_X7C_X20_X25s_X20_X7C_X20_X25s_X20_X7C_X0A),
    /* K26  */  be_nested_str_weak(Symbol),
    /* K27  */  be_nested_str_weak(Type),
    /* K28  */  be_nested_str_weak(Builtin),
    /* K29  */  be_nested_str_weak(Dangerous),
    /* K30  */  be_nested_str_weak(Takes_X20Args),
    /* K31  */  be_nested_str_weak(_X7C_X25s_X7C_X25s_X7C_X25s_X7C_X25s_X7C_X25s_X7C_X0A),
    /* K32  */  be_nested_str_weak(_X2D),
    /* K33  */  be_nested_str_weak(_X0A),
    }),
    be_str_weak(get_symbol_table_report),
    &be_const_str_solidified,
    ( &(const binstruction[202]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x58080001,  //  0001  LDCONST	R2	K1
      0x880C0102,  //  0002  GETMBR	R3	R0	K2
      0x8C0C0703,  //  0003  GETMET	R3	R3	K3
      0x7C0C0200,  //  0004  CALL	R3	1
      0x6010000C,  //  0005  GETGBL	R4	G12
      0x5C140600,  //  0006  MOVE	R5	R3
      0x7C100200,  //  0007  CALL	R4	1
      0x1C100904,  //  0008  EQ	R4	R4	K4
      0x78120001,  //  0009  JMPF	R4	#000C
      0x00080505,  //  000A  ADD	R2	R2	K5
      0x80040400,  //  000B  RET	1	R2
      0x84100000,  //  000C  CLOSURE	R4	P0
      0x60140012,  //  000D  GETGBL	R5	G18
      0x7C140000,  //  000E  CALL	R5	0
      0x541A0005,  //  000F  LDINT	R6	6
      0x541E0003,  //  0010  LDINT	R7	4
      0x54220006,  //  0011  LDINT	R8	7
      0x54260008,  //  0012  LDINT	R9	9
      0x542A0009,  //  0013  LDINT	R10	10
      0x602C0010,  //  0014  GETGBL	R11	G16
      0x5C300600,  //  0015  MOVE	R12	R3
      0x7C2C0200,  //  0016  CALL	R11	1
      0xA802005E,  //  0017  EXBLK	0	#0077
      0x5C301600,  //  0018  MOVE	R12	R11
      0x7C300000,  //  0019  CALL	R12	0
      0x8C340306,  //  001A  GETMET	R13	R1	K6
      0x5C3C1800,  //  001B  MOVE	R15	R12
      0x58400007,  //  001C  LDCONST	R16	K7
      0x7C340600,  //  001D  CALL	R13	3
      0x6038000C,  //  001E  GETGBL	R14	G12
      0x5C3C1A00,  //  001F  MOVE	R15	R13
      0x7C380200,  //  0020  CALL	R14	1
      0x28381D08,  //  0021  GE	R14	R14	K8
      0x783A0052,  //  0022  JMPF	R14	#0076
      0x94381B04,  //  0023  GETIDX	R14	R13	K4
      0x943C1B09,  //  0024  GETIDX	R15	R13	K9
      0x88400102,  //  0025  GETMBR	R16	R0	K2
      0x8C40210A,  //  0026  GETMET	R16	R16	K10
      0x5C481C00,  //  0027  MOVE	R18	R14
      0x7C400400,  //  0028  CALL	R16	2
      0x4C440000,  //  0029  LDNIL	R17
      0x20442011,  //  002A  NE	R17	R16	R17
      0x78460049,  //  002B  JMPF	R17	#0076
      0x8844210B,  //  002C  GETMBR	R17	R16	K11
      0x78460001,  //  002D  JMPF	R17	#0030
      0x5844000C,  //  002E  LDCONST	R17	K12
      0x70020000,  //  002F  JMP		#0031
      0x5844000D,  //  0030  LDCONST	R17	K13
      0x8C48210E,  //  0031  GETMET	R18	R16	K14
      0x7C480200,  //  0032  CALL	R18	1
      0x784A0001,  //  0033  JMPF	R18	#0036
      0x5848000F,  //  0034  LDCONST	R18	K15
      0x70020000,  //  0035  JMP		#0037
      0x5848000D,  //  0036  LDCONST	R18	K13
      0x884C2110,  //  0037  GETMBR	R19	R16	K16
      0x784E0001,  //  0038  JMPF	R19	#003B
      0x584C0011,  //  0039  LDCONST	R19	K17
      0x70020000,  //  003A  JMP		#003C
      0x584C000D,  //  003B  LDCONST	R19	K13
      0x60500018,  //  003C  GETGBL	R20	G24
      0x58540012,  //  003D  LDCONST	R21	K18
      0x5C581C00,  //  003E  MOVE	R22	R14
      0x7C500400,  //  003F  CALL	R20	2
      0x5C540800,  //  0040  MOVE	R21	R4
      0x5C582800,  //  0041  MOVE	R22	R20
      0x7C540200,  //  0042  CALL	R21	1
      0x24542A06,  //  0043  GT	R21	R21	R6
      0x78560003,  //  0044  JMPF	R21	#0049
      0x5C540800,  //  0045  MOVE	R21	R4
      0x5C582800,  //  0046  MOVE	R22	R20
      0x7C540200,  //  0047  CALL	R21	1
      0x5C182A00,  //  0048  MOVE	R6	R21
      0x5C540800,  //  0049  MOVE	R21	R4
      0x5C581E00,  //  004A  MOVE	R22	R15
      0x7C540200,  //  004B  CALL	R21	1
      0x24542A07,  //  004C  GT	R21	R21	R7
      0x78560003,  //  004D  JMPF	R21	#0052
      0x5C540800,  //  004E  MOVE	R21	R4
      0x5C581E00,  //  004F  MOVE	R22	R15
      0x7C540200,  //  0050  CALL	R21	1
      0x5C1C2A00,  //  0051  MOVE	R7	R21
      0x5C540800,  //  0052  MOVE	R21	R4
      0x5C582200,  //  0053  MOVE	R22	R17
      0x7C540200,  //  0054  CALL	R21	1
      0x24542A08,  //  0055  GT	R21	R21	R8
      0x78560003,  //  0056  JMPF	R21	#005B
      0x5C540800,  //  0057  MOVE	R21	R4
      0x5C582200,  //  0058  MOVE	R22	R17
      0x7C540200,  //  0059  CALL	R21	1
      0x5C202A00,  //  005A  MOVE	R8	R21
      0x5C540800,  //  005B  MOVE	R21	R4
      0x5C582400,  //  005C  MOVE	R22	R18
      0x7C540200,  //  005D  CALL	R21	1
      0x24542A09,  //  005E  GT	R21	R21	R9
      0x78560003,  //  005F  JMPF	R21	#0064
      0x5C540800,  //  0060  MOVE	R21	R4
      0x5C582400,  //  0061  MOVE	R22	R18
      0x7C540200,  //  0062  CALL	R21	1
      0x5C242A00,  //  0063  MOVE	R9	R21
      0x5C540800,  //  0064  MOVE	R21	R4
      0x5C582600,  //  0065  MOVE	R22	R19
      0x7C540200,  //  0066  CALL	R21	1
      0x24542A0A,  //  0067  GT	R21	R21	R10
      0x78560003,  //  0068  JMPF	R21	#006D
      0x5C540800,  //  0069  MOVE	R21	R4
      0x5C582600,  //  006A  MOVE	R22	R19
      0x7C540200,  //  006B  CALL	R21	1
      0x5C282A00,  //  006C  MOVE	R10	R21
      0x8C540B13,  //  006D  GETMET	R21	R5	K19
      0x605C0013,  //  006E  GETGBL	R23	G19
      0x7C5C0000,  //  006F  CALL	R23	0
      0x985E2814,  //  0070  SETIDX	R23	K20	R20
      0x985E2A0F,  //  0071  SETIDX	R23	K21	R15
      0x985E2C11,  //  0072  SETIDX	R23	K22	R17
      0x985E2E12,  //  0073  SETIDX	R23	K23	R18
      0x985E2013,  //  0074  SETIDX	R23	K16	R19
      0x7C540400,  //  0075  CALL	R21	2
      0x7001FFA0,  //  0076  JMP		#0018
      0x582C0018,  //  0077  LDCONST	R11	K24
      0xAC2C0200,  //  0078  CATCH	R11	1	0
      0xB0080000,  //  0079  RAISE	2	R0	R0
      0x842C0001,  //  007A  CLOSURE	R11	P1
      0x5C301600,  //  007B  MOVE	R12	R11
      0x7C300000,  //  007C  CALL	R12	0
      0x84300002,  //  007D  CLOSURE	R12	P2
      0x84340003,  //  007E  CLOSURE	R13	P3
      0x60380018,  //  007F  GETGBL	R14	G24
      0x583C0019,  //  0080  LDCONST	R15	K25
      0x5C401800,  //  0081  MOVE	R16	R12
      0x5844001A,  //  0082  LDCONST	R17	K26
      0x5C480C00,  //  0083  MOVE	R18	R6
      0x7C400400,  //  0084  CALL	R16	2
      0x5C441800,  //  0085  MOVE	R17	R12
      0x5848001B,  //  0086  LDCONST	R18	K27
      0x5C4C0E00,  //  0087  MOVE	R19	R7
      0x7C440400,  //  0088  CALL	R17	2
      0x5C481800,  //  0089  MOVE	R18	R12
      0x584C001C,  //  008A  LDCONST	R19	K28
      0x5C501000,  //  008B  MOVE	R20	R8
      0x7C480400,  //  008C  CALL	R18	2
      0x5C4C1800,  //  008D  MOVE	R19	R12
      0x5850001D,  //  008E  LDCONST	R20	K29
      0x5C541200,  //  008F  MOVE	R21	R9
      0x7C4C0400,  //  0090  CALL	R19	2
      0x5C501800,  //  0091  MOVE	R20	R12
      0x5854001E,  //  0092  LDCONST	R21	K30
      0x5C581400,  //  0093  MOVE	R22	R10
      0x7C500400,  //  0094  CALL	R20	2
      0x7C380C00,  //  0095  CALL	R14	6
      0x603C0018,  //  0096  GETGBL	R15	G24
      0x5840001F,  //  0097  LDCONST	R16	K31
      0x00440D08,  //  0098  ADD	R17	R6	K8
      0x08464011,  //  0099  MUL	R17	K32	R17
      0x00480F08,  //  009A  ADD	R18	R7	K8
      0x084A4012,  //  009B  MUL	R18	K32	R18
      0x004C1108,  //  009C  ADD	R19	R8	K8
      0x084E4013,  //  009D  MUL	R19	K32	R19
      0x00501308,  //  009E  ADD	R20	R9	K8
      0x08524014,  //  009F  MUL	R20	K32	R20
      0x00541508,  //  00A0  ADD	R21	R10	K8
      0x08564015,  //  00A1  MUL	R21	K32	R21
      0x7C3C0C00,  //  00A2  CALL	R15	6
      0x0008040E,  //  00A3  ADD	R2	R2	R14
      0x0008040F,  //  00A4  ADD	R2	R2	R15
      0x60400010,  //  00A5  GETGBL	R16	G16
      0x5C440A00,  //  00A6  MOVE	R17	R5
      0x7C400200,  //  00A7  CALL	R16	1
      0xA802001A,  //  00A8  EXBLK	0	#00C4
      0x5C442000,  //  00A9  MOVE	R17	R16
      0x7C440000,  //  00AA  CALL	R17	0
      0x60480018,  //  00AB  GETGBL	R18	G24
      0x584C0019,  //  00AC  LDCONST	R19	K25
      0x5C501800,  //  00AD  MOVE	R20	R12
      0x94542314,  //  00AE  GETIDX	R21	R17	K20
      0x5C580C00,  //  00AF  MOVE	R22	R6
      0x7C500400,  //  00B0  CALL	R20	2
      0x5C541800,  //  00B1  MOVE	R21	R12
      0x94582315,  //  00B2  GETIDX	R22	R17	K21
      0x5C5C0E00,  //  00B3  MOVE	R23	R7
      0x7C540400,  //  00B4  CALL	R21	2
      0x5C581A00,  //  00B5  MOVE	R22	R13
      0x945C2316,  //  00B6  GETIDX	R23	R17	K22
      0x5C601000,  //  00B7  MOVE	R24	R8
      0x7C580400,  //  00B8  CALL	R22	2
      0x5C5C1A00,  //  00B9  MOVE	R23	R13
      0x94602317,  //  00BA  GETIDX	R24	R17	K23
      0x5C641200,  //  00BB  MOVE	R25	R9
      0x7C5C0400,  //  00BC  CALL	R23	2
      0x5C601A00,  //  00BD  MOVE	R24	R13
      0x94642310,  //  00BE  GETIDX	R25	R17	K16
      0x5C681400,  //  00BF  MOVE	R26	R10
      0x7C600400,  //  00C0  CALL	R24	2
      0x7C480C00,  //  00C1  CALL	R18	6
      0x00080412,  //  00C2  ADD	R2	R2	R18
      0x7001FFE4,  //  00C3  JMP		#00A9
      0x58400018,  //  00C4  LDCONST	R16	K24
      0xAC400200,  //  00C5  CATCH	R16	1	0
      0xB0080000,  //  00C6  RAISE	2	R0	R0
      0x00080521,  //  00C7  ADD	R2	R2	K33
      0xA0000000,  //  00C8  CLOSE	R0
      0x80040400,  //  00C9  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: next
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_next,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(pos),
    /* K1   */  be_nested_str_weak(tokens),
    /* K2   */  be_const_int(1),
    }),
    be_str_weak(next),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x6008000C,  //  0001  GETGBL	R2	G12
      0x880C0101,  //  0002  GETMBR	R3	R0	K1
      0x7C080200,  //  0003  CALL	R2	1
      0x14040202,  //  0004  LT	R1	R1	R2
      0x78060002,  //  0005  JMPF	R1	#0009
      0x88040100,  //  0006  GETMBR	R1	R0	K0
      0x00040302,  //  0007  ADD	R1	R1	K2
      0x90020001,  //  0008  SETMBR	R0	K0	R1
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _process_user_function_call
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__process_user_function_call,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(LEFT_PAREN),
    /* K5   */  be_nested_str_weak(process_function_arguments),
    /* K6   */  be_nested_str_weak(),
    /* K7   */  be_nested_str_weak(engine_X2C_X20_X25s),
    /* K8   */  be_nested_str_weak(engine),
    /* K9   */  be_nested_str_weak(animation_X2Eget_user_function_X28_X27_X25s_X27_X29_X28_X25s_X29),
    /* K10  */  be_nested_str_weak(error),
    /* K11  */  be_nested_str_weak(User_X20functions_X20must_X20be_X20called_X20with_X20parentheses_X3A_X20user_X2Efunction_name_X28_X29),
    /* K12  */  be_nested_str_weak(nil),
    }),
    be_str_weak(_process_user_function_call),
    &be_const_str_solidified,
    ( &(const binstruction[36]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x7C080200,  //  0001  CALL	R2	1
      0x4C0C0000,  //  0002  LDNIL	R3
      0x20080403,  //  0003  NE	R2	R2	R3
      0x780A0019,  //  0004  JMPF	R2	#001F
      0x8C080100,  //  0005  GETMET	R2	R0	K0
      0x7C080200,  //  0006  CALL	R2	1
      0x88080501,  //  0007  GETMBR	R2	R2	K1
      0xB80E0400,  //  0008  GETNGBL	R3	K2
      0x880C0703,  //  0009  GETMBR	R3	R3	K3
      0x880C0704,  //  000A  GETMBR	R3	R3	K4
      0x1C080403,  //  000B  EQ	R2	R2	R3
      0x780A0011,  //  000C  JMPF	R2	#001F
      0x8C080105,  //  000D  GETMET	R2	R0	K5
      0x50100200,  //  000E  LDBOOL	R4	1	0
      0x7C080400,  //  000F  CALL	R2	2
      0x200C0506,  //  0010  NE	R3	R2	K6
      0x780E0004,  //  0011  JMPF	R3	#0017
      0x600C0018,  //  0012  GETGBL	R3	G24
      0x58100007,  //  0013  LDCONST	R4	K7
      0x5C140400,  //  0014  MOVE	R5	R2
      0x7C0C0400,  //  0015  CALL	R3	2
      0x70020000,  //  0016  JMP		#0018
      0x580C0008,  //  0017  LDCONST	R3	K8
      0x60100018,  //  0018  GETGBL	R4	G24
      0x58140009,  //  0019  LDCONST	R5	K9
      0x5C180200,  //  001A  MOVE	R6	R1
      0x5C1C0600,  //  001B  MOVE	R7	R3
      0x7C100600,  //  001C  CALL	R4	3
      0x80040800,  //  001D  RET	1	R4
      0x70020003,  //  001E  JMP		#0023
      0x8C08010A,  //  001F  GETMET	R2	R0	K10
      0x5810000B,  //  0020  LDCONST	R4	K11
      0x7C080400,  //  0021  CALL	R2	2
      0x80061800,  //  0022  RET	1	K12
      0x80000000,  //  0023  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_animation
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_animation,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        6,                          /* nstack */
        2,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 2]) {     /* constants */
        /* K0   */  be_nested_str_weak(symbol_table),
        /* K1   */  be_nested_str_weak(create_animation),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 7]) {  /* code */
          0x68080000,  //  0000  GETUPV	R2	U0
          0x88080500,  //  0001  GETMBR	R2	R2	K0
          0x8C080501,  //  0002  GETMET	R2	R2	K1
          0x5C100000,  //  0003  MOVE	R4	R0
          0x5C140200,  //  0004  MOVE	R5	R1
          0x7C080600,  //  0005  CALL	R2	3
          0x80040400,  //  0006  RET	1	R2
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[39]) {     /* constants */
    /* K0   */  be_nested_str_weak(next),
    /* K1   */  be_nested_str_weak(expect_identifier),
    /* K2   */  be_nested_str_weak(validate_user_name),
    /* K3   */  be_nested_str_weak(animation),
    /* K4   */  be_nested_str_weak(skip_statement),
    /* K5   */  be_nested_str_weak(expect_assign),
    /* K6   */  be_nested_str_weak(current),
    /* K7   */  be_nested_str_weak(type),
    /* K8   */  be_nested_str_weak(animation_dsl),
    /* K9   */  be_nested_str_weak(Token),
    /* K10  */  be_nested_str_weak(KEYWORD),
    /* K11  */  be_nested_str_weak(IDENTIFIER),
    /* K12  */  be_nested_str_weak(peek),
    /* K13  */  be_nested_str_weak(LEFT_PAREN),
    /* K14  */  be_nested_str_weak(value),
    /* K15  */  be_nested_str_weak(),
    /* K16  */  be_nested_str_weak(COMMENT),
    /* K17  */  be_nested_str_weak(_X20_X20),
    /* K18  */  be_nested_str_weak(symbol_table),
    /* K19  */  be_nested_str_weak(get),
    /* K20  */  be_nested_str_weak(_symbol_entry),
    /* K21  */  be_nested_str_weak(TYPE_TEMPLATE),
    /* K22  */  be_nested_str_weak(process_function_arguments),
    /* K23  */  be_nested_str_weak(engine_X2C_X20_X25s),
    /* K24  */  be_nested_str_weak(engine),
    /* K25  */  be_nested_str_weak(add),
    /* K26  */  be_nested_str_weak(var_X20_X25s__X20_X3D_X20_X25s_template_X28_X25s_X29_X25s),
    /* K27  */  be_nested_str_weak(create_animation),
    /* K28  */  be_nested_str_weak(TYPE_USER_FUNCTION),
    /* K29  */  be_nested_str_weak(var_X20_X25s__X20_X3D_X20animation_X2Eget_user_function_X28_X27_X25s_X27_X29_X28_X25s_X29_X25s),
    /* K30  */  be_nested_str_weak(_validate_animation_factory_creates_animation),
    /* K31  */  be_nested_str_weak(error),
    /* K32  */  be_nested_str_weak(Animation_X20factory_X20function_X20_X27_X25s_X27_X20does_X20not_X20exist_X20or_X20does_X20not_X20create_X20an_X20instance_X20of_X20animation_X2Eanimation_X20class_X2E_X20Check_X20the_X20function_X20name_X20and_X20ensure_X20it_X20returns_X20an_X20animation_X20object_X2E),
    /* K33  */  be_nested_str_weak(var_X20_X25s__X20_X3D_X20animation_X2E_X25s_X28engine_X29_X25s),
    /* K34  */  be_nested_str_weak(_create_instance_for_validation),
    /* K35  */  be_nested_str_weak(_process_named_arguments_for_animation),
    /* K36  */  be_nested_str_weak(_X25s_),
    /* K37  */  be_nested_str_weak(_process_simple_value_assignment),
    /* K38  */  be_nested_str_weak(CONTEXT_ANIMATION),
    }),
    be_str_weak(process_animation),
    &be_const_str_solidified,
    ( &(const binstruction[186]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x7C040200,  //  0003  CALL	R1	1
      0x8C080102,  //  0004  GETMET	R2	R0	K2
      0x5C100200,  //  0005  MOVE	R4	R1
      0x58140003,  //  0006  LDCONST	R5	K3
      0x7C080600,  //  0007  CALL	R2	3
      0x740A0002,  //  0008  JMPT	R2	#000C
      0x8C080104,  //  0009  GETMET	R2	R0	K4
      0x7C080200,  //  000A  CALL	R2	1
      0x80000400,  //  000B  RET	0
      0x8C080105,  //  000C  GETMET	R2	R0	K5
      0x7C080200,  //  000D  CALL	R2	1
      0x8C080106,  //  000E  GETMET	R2	R0	K6
      0x7C080200,  //  000F  CALL	R2	1
      0x880C0507,  //  0010  GETMBR	R3	R2	K7
      0xB8121000,  //  0011  GETNGBL	R4	K8
      0x88100909,  //  0012  GETMBR	R4	R4	K9
      0x8810090A,  //  0013  GETMBR	R4	R4	K10
      0x1C0C0604,  //  0014  EQ	R3	R3	R4
      0x740E0005,  //  0015  JMPT	R3	#001C
      0x880C0507,  //  0016  GETMBR	R3	R2	K7
      0xB8121000,  //  0017  GETNGBL	R4	K8
      0x88100909,  //  0018  GETMBR	R4	R4	K9
      0x8810090B,  //  0019  GETMBR	R4	R4	K11
      0x1C0C0604,  //  001A  EQ	R3	R3	R4
      0x780E0096,  //  001B  JMPF	R3	#00B3
      0x8C0C010C,  //  001C  GETMET	R3	R0	K12
      0x7C0C0200,  //  001D  CALL	R3	1
      0x4C100000,  //  001E  LDNIL	R4
      0x200C0604,  //  001F  NE	R3	R3	R4
      0x780E0091,  //  0020  JMPF	R3	#00B3
      0x8C0C010C,  //  0021  GETMET	R3	R0	K12
      0x7C0C0200,  //  0022  CALL	R3	1
      0x880C0707,  //  0023  GETMBR	R3	R3	K7
      0xB8121000,  //  0024  GETNGBL	R4	K8
      0x88100909,  //  0025  GETMBR	R4	R4	K9
      0x8810090D,  //  0026  GETMBR	R4	R4	K13
      0x1C0C0604,  //  0027  EQ	R3	R3	R4
      0x780E0089,  //  0028  JMPF	R3	#00B3
      0x880C050E,  //  0029  GETMBR	R3	R2	K14
      0x8C100100,  //  002A  GETMET	R4	R0	K0
      0x7C100200,  //  002B  CALL	R4	1
      0x5810000F,  //  002C  LDCONST	R4	K15
      0x8C140106,  //  002D  GETMET	R5	R0	K6
      0x7C140200,  //  002E  CALL	R5	1
      0x4C180000,  //  002F  LDNIL	R6
      0x20140A06,  //  0030  NE	R5	R5	R6
      0x7816000E,  //  0031  JMPF	R5	#0041
      0x8C140106,  //  0032  GETMET	R5	R0	K6
      0x7C140200,  //  0033  CALL	R5	1
      0x88140B07,  //  0034  GETMBR	R5	R5	K7
      0xB81A1000,  //  0035  GETNGBL	R6	K8
      0x88180D09,  //  0036  GETMBR	R6	R6	K9
      0x88180D10,  //  0037  GETMBR	R6	R6	K16
      0x1C140A06,  //  0038  EQ	R5	R5	R6
      0x78160006,  //  0039  JMPF	R5	#0041
      0x8C140106,  //  003A  GETMET	R5	R0	K6
      0x7C140200,  //  003B  CALL	R5	1
      0x88140B0E,  //  003C  GETMBR	R5	R5	K14
      0x00162205,  //  003D  ADD	R5	K17	R5
      0x5C100A00,  //  003E  MOVE	R4	R5
      0x8C140100,  //  003F  GETMET	R5	R0	K0
      0x7C140200,  //  0040  CALL	R5	1
      0x88140112,  //  0041  GETMBR	R5	R0	K18
      0x8C140B13,  //  0042  GETMET	R5	R5	K19
      0x5C1C0600,  //  0043  MOVE	R7	R3
      0x7C140400,  //  0044  CALL	R5	2
      0x4C180000,  //  0045  LDNIL	R6
      0x20180A06,  //  0046  NE	R6	R5	R6
      0x781A001F,  //  0047  JMPF	R6	#0068
      0x88180B07,  //  0048  GETMBR	R6	R5	K7
      0xB81E1000,  //  0049  GETNGBL	R7	K8
      0x881C0F14,  //  004A  GETMBR	R7	R7	K20
      0x881C0F15,  //  004B  GETMBR	R7	R7	K21
      0x1C180C07,  //  004C  EQ	R6	R6	R7
      0x781A0019,  //  004D  JMPF	R6	#0068
      0x8C180116,  //  004E  GETMET	R6	R0	K22
      0x50200000,  //  004F  LDBOOL	R8	0	0
      0x7C180400,  //  0050  CALL	R6	2
      0x201C0D0F,  //  0051  NE	R7	R6	K15
      0x781E0004,  //  0052  JMPF	R7	#0058
      0x601C0018,  //  0053  GETGBL	R7	G24
      0x58200017,  //  0054  LDCONST	R8	K23
      0x5C240C00,  //  0055  MOVE	R9	R6
      0x7C1C0400,  //  0056  CALL	R7	2
      0x70020000,  //  0057  JMP		#0059
      0x581C0018,  //  0058  LDCONST	R7	K24
      0x8C200119,  //  0059  GETMET	R8	R0	K25
      0x60280018,  //  005A  GETGBL	R10	G24
      0x582C001A,  //  005B  LDCONST	R11	K26
      0x5C300200,  //  005C  MOVE	R12	R1
      0x5C340600,  //  005D  MOVE	R13	R3
      0x5C380E00,  //  005E  MOVE	R14	R7
      0x5C3C0800,  //  005F  MOVE	R15	R4
      0x7C280A00,  //  0060  CALL	R10	5
      0x7C200400,  //  0061  CALL	R8	2
      0x88200112,  //  0062  GETMBR	R8	R0	K18
      0x8C20111B,  //  0063  GETMET	R8	R8	K27
      0x5C280200,  //  0064  MOVE	R10	R1
      0x4C2C0000,  //  0065  LDNIL	R11
      0x7C200600,  //  0066  CALL	R8	3
      0x70020049,  //  0067  JMP		#00B2
      0x4C180000,  //  0068  LDNIL	R6
      0x20180A06,  //  0069  NE	R6	R5	R6
      0x781A001F,  //  006A  JMPF	R6	#008B
      0x88180B07,  //  006B  GETMBR	R6	R5	K7
      0xB81E1000,  //  006C  GETNGBL	R7	K8
      0x881C0F14,  //  006D  GETMBR	R7	R7	K20
      0x881C0F1C,  //  006E  GETMBR	R7	R7	K28
      0x1C180C07,  //  006F  EQ	R6	R6	R7
      0x781A0019,  //  0070  JMPF	R6	#008B
      0x8C180116,  //  0071  GETMET	R6	R0	K22
      0x50200000,  //  0072  LDBOOL	R8	0	0
      0x7C180400,  //  0073  CALL	R6	2
      0x201C0D0F,  //  0074  NE	R7	R6	K15
      0x781E0004,  //  0075  JMPF	R7	#007B
      0x601C0018,  //  0076  GETGBL	R7	G24
      0x58200017,  //  0077  LDCONST	R8	K23
      0x5C240C00,  //  0078  MOVE	R9	R6
      0x7C1C0400,  //  0079  CALL	R7	2
      0x70020000,  //  007A  JMP		#007C
      0x581C0018,  //  007B  LDCONST	R7	K24
      0x8C200119,  //  007C  GETMET	R8	R0	K25
      0x60280018,  //  007D  GETGBL	R10	G24
      0x582C001D,  //  007E  LDCONST	R11	K29
      0x5C300200,  //  007F  MOVE	R12	R1
      0x5C340600,  //  0080  MOVE	R13	R3
      0x5C380E00,  //  0081  MOVE	R14	R7
      0x5C3C0800,  //  0082  MOVE	R15	R4
      0x7C280A00,  //  0083  CALL	R10	5
      0x7C200400,  //  0084  CALL	R8	2
      0x88200112,  //  0085  GETMBR	R8	R0	K18
      0x8C20111B,  //  0086  GETMET	R8	R8	K27
      0x5C280200,  //  0087  MOVE	R10	R1
      0x4C2C0000,  //  0088  LDNIL	R11
      0x7C200600,  //  0089  CALL	R8	3
      0x70020026,  //  008A  JMP		#00B2
      0x8C18011E,  //  008B  GETMET	R6	R0	K30
      0x5C200600,  //  008C  MOVE	R8	R3
      0x7C180400,  //  008D  CALL	R6	2
      0x741A0008,  //  008E  JMPT	R6	#0098
      0x8C18011F,  //  008F  GETMET	R6	R0	K31
      0x60200018,  //  0090  GETGBL	R8	G24
      0x58240020,  //  0091  LDCONST	R9	K32
      0x5C280600,  //  0092  MOVE	R10	R3
      0x7C200400,  //  0093  CALL	R8	2
      0x7C180400,  //  0094  CALL	R6	2
      0x8C180104,  //  0095  GETMET	R6	R0	K4
      0x7C180200,  //  0096  CALL	R6	1
      0x80000C00,  //  0097  RET	0
      0x8C180119,  //  0098  GETMET	R6	R0	K25
      0x60200018,  //  0099  GETGBL	R8	G24
      0x58240021,  //  009A  LDCONST	R9	K33
      0x5C280200,  //  009B  MOVE	R10	R1
      0x5C2C0600,  //  009C  MOVE	R11	R3
      0x5C300800,  //  009D  MOVE	R12	R4
      0x7C200800,  //  009E  CALL	R8	4
      0x7C180400,  //  009F  CALL	R6	2
      0x8C180122,  //  00A0  GETMET	R6	R0	K34
      0x5C200600,  //  00A1  MOVE	R8	R3
      0x7C180400,  //  00A2  CALL	R6	2
      0x4C1C0000,  //  00A3  LDNIL	R7
      0x201C0C07,  //  00A4  NE	R7	R6	R7
      0x781E0004,  //  00A5  JMPF	R7	#00AB
      0x881C0112,  //  00A6  GETMBR	R7	R0	K18
      0x8C1C0F1B,  //  00A7  GETMET	R7	R7	K27
      0x5C240200,  //  00A8  MOVE	R9	R1
      0x5C280C00,  //  00A9  MOVE	R10	R6
      0x7C1C0600,  //  00AA  CALL	R7	3
      0x8C1C0123,  //  00AB  GETMET	R7	R0	K35
      0x60240018,  //  00AC  GETGBL	R9	G24
      0x58280024,  //  00AD  LDCONST	R10	K36
      0x5C2C0200,  //  00AE  MOVE	R11	R1
      0x7C240400,  //  00AF  CALL	R9	2
      0x5C280600,  //  00B0  MOVE	R10	R3
      0x7C1C0600,  //  00B1  CALL	R7	3
      0x70020004,  //  00B2  JMP		#00B8
      0x8C0C0125,  //  00B3  GETMET	R3	R0	K37
      0x5C140200,  //  00B4  MOVE	R5	R1
      0x88180126,  //  00B5  GETMBR	R6	R0	K38
      0x841C0000,  //  00B6  CLOSURE	R7	P0
      0x7C0C0800,  //  00B7  CALL	R3	4
      0xA0000000,  //  00B8  CLOSE	R0
      0x80000000,  //  00B9  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_wait_statement_fluent
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_wait_statement_fluent,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(next),
    /* K1   */  be_nested_str_weak(process_time_value),
    /* K2   */  be_nested_str_weak(collect_inline_comment),
    /* K3   */  be_nested_str_weak(add),
    /* K4   */  be_nested_str_weak(_X25s_X2Epush_wait_step_X28_X25s_X29_X25s),
    /* K5   */  be_nested_str_weak(get_indent),
    }),
    be_str_weak(process_wait_statement_fluent),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x7C040200,  //  0003  CALL	R1	1
      0x8C080102,  //  0004  GETMET	R2	R0	K2
      0x7C080200,  //  0005  CALL	R2	1
      0x8C0C0103,  //  0006  GETMET	R3	R0	K3
      0x60140018,  //  0007  GETGBL	R5	G24
      0x58180004,  //  0008  LDCONST	R6	K4
      0x8C1C0105,  //  0009  GETMET	R7	R0	K5
      0x7C1C0200,  //  000A  CALL	R7	1
      0x5C200200,  //  000B  MOVE	R8	R1
      0x5C240400,  //  000C  MOVE	R9	R2
      0x7C140800,  //  000D  CALL	R5	4
      0x7C0C0400,  //  000E  CALL	R3	2
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: check_right_brace
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_check_right_brace,   /* name */
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
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(RIGHT_BRACE),
    }),
    be_str_weak(check_right_brace),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0005,  //  0004  JMPF	R2	#000B
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x740A0000,  //  000A  JMPT	R2	#000C
      0x50080001,  //  000B  LDBOOL	R2	0	1
      0x50080200,  //  000C  LDBOOL	R2	1	0
      0x80040400,  //  000D  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: error
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_error,   /* name */
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
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(line),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(errors),
    /* K4   */  be_nested_str_weak(push),
    /* K5   */  be_nested_str_weak(Line_X20_X25s_X3A_X20_X25s),
    }),
    be_str_weak(error),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x7C080200,  //  0001  CALL	R2	1
      0x4C0C0000,  //  0002  LDNIL	R3
      0x20080403,  //  0003  NE	R2	R2	R3
      0x780A0003,  //  0004  JMPF	R2	#0009
      0x8C080100,  //  0005  GETMET	R2	R0	K0
      0x7C080200,  //  0006  CALL	R2	1
      0x88080501,  //  0007  GETMBR	R2	R2	K1
      0x70020000,  //  0008  JMP		#000A
      0x58080002,  //  0009  LDCONST	R2	K2
      0x880C0103,  //  000A  GETMBR	R3	R0	K3
      0x8C0C0704,  //  000B  GETMET	R3	R3	K4
      0x60140018,  //  000C  GETGBL	R5	G24
      0x58180005,  //  000D  LDCONST	R6	K5
      0x5C1C0400,  //  000E  MOVE	R7	R2
      0x5C200200,  //  000F  MOVE	R8	R1
      0x7C140600,  //  0010  CALL	R5	3
      0x7C0C0400,  //  0011  CALL	R3	2
      0x80000000,  //  0012  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: convert_color
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_convert_color,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(startswith),
    /* K2   */  be_nested_str_weak(0x),
    /* K3   */  be_nested_str_weak(0xFF_X25s),
    /* K4   */  be_const_int(2),
    /* K5   */  be_const_int(2147483647),
    /* K6   */  be_nested_str_weak(_X23),
    /* K7   */  be_nested_str_weak(0x_X25s),
    /* K8   */  be_const_int(1),
    /* K9   */  be_const_int(3),
    /* K10  */  be_nested_str_weak(0x_X25s_X25s_X25s_X25s_X25s_X25s_X25s_X25s),
    /* K11  */  be_nested_str_weak(0xFF_X25s_X25s_X25s_X25s_X25s_X25s),
    /* K12  */  be_nested_str_weak(animation_dsl),
    /* K13  */  be_nested_str_weak(is_color_name),
    /* K14  */  be_nested_str_weak(get_named_color_value),
    /* K15  */  be_nested_str_weak(0xFFFFFFFF),
    }),
    be_str_weak(convert_color),
    &be_const_str_solidified,
    ( &(const binstruction[110]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0501,  //  0001  GETMET	R3	R2	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x58180002,  //  0003  LDCONST	R6	K2
      0x7C0C0600,  //  0004  CALL	R3	3
      0x780E0013,  //  0005  JMPF	R3	#001A
      0x600C000C,  //  0006  GETGBL	R3	G12
      0x5C100200,  //  0007  MOVE	R4	R1
      0x7C0C0200,  //  0008  CALL	R3	1
      0x54120009,  //  0009  LDINT	R4	10
      0x1C0C0604,  //  000A  EQ	R3	R3	R4
      0x780E0001,  //  000B  JMPF	R3	#000E
      0x80040200,  //  000C  RET	1	R1
      0x7002000B,  //  000D  JMP		#001A
      0x600C000C,  //  000E  GETGBL	R3	G12
      0x5C100200,  //  000F  MOVE	R4	R1
      0x7C0C0200,  //  0010  CALL	R3	1
      0x54120007,  //  0011  LDINT	R4	8
      0x1C0C0604,  //  0012  EQ	R3	R3	R4
      0x780E0005,  //  0013  JMPF	R3	#001A
      0x600C0018,  //  0014  GETGBL	R3	G24
      0x58100003,  //  0015  LDCONST	R4	K3
      0x40160905,  //  0016  CONNECT	R5	K4	K5
      0x94140205,  //  0017  GETIDX	R5	R1	R5
      0x7C0C0400,  //  0018  CALL	R3	2
      0x80040600,  //  0019  RET	1	R3
      0x8C0C0501,  //  001A  GETMET	R3	R2	K1
      0x5C140200,  //  001B  MOVE	R5	R1
      0x58180006,  //  001C  LDCONST	R6	K6
      0x7C0C0600,  //  001D  CALL	R3	3
      0x780E0044,  //  001E  JMPF	R3	#0064
      0x600C000C,  //  001F  GETGBL	R3	G12
      0x5C100200,  //  0020  MOVE	R4	R1
      0x7C0C0200,  //  0021  CALL	R3	1
      0x54120008,  //  0022  LDINT	R4	9
      0x1C0C0604,  //  0023  EQ	R3	R3	R4
      0x780E0006,  //  0024  JMPF	R3	#002C
      0x600C0018,  //  0025  GETGBL	R3	G24
      0x58100007,  //  0026  LDCONST	R4	K7
      0x40161105,  //  0027  CONNECT	R5	K8	K5
      0x94140205,  //  0028  GETIDX	R5	R1	R5
      0x7C0C0400,  //  0029  CALL	R3	2
      0x80040600,  //  002A  RET	1	R3
      0x70020037,  //  002B  JMP		#0064
      0x600C000C,  //  002C  GETGBL	R3	G12
      0x5C100200,  //  002D  MOVE	R4	R1
      0x7C0C0200,  //  002E  CALL	R3	1
      0x54120006,  //  002F  LDINT	R4	7
      0x1C0C0604,  //  0030  EQ	R3	R3	R4
      0x780E0006,  //  0031  JMPF	R3	#0039
      0x600C0018,  //  0032  GETGBL	R3	G24
      0x58100003,  //  0033  LDCONST	R4	K3
      0x40161105,  //  0034  CONNECT	R5	K8	K5
      0x94140205,  //  0035  GETIDX	R5	R1	R5
      0x7C0C0400,  //  0036  CALL	R3	2
      0x80040600,  //  0037  RET	1	R3
      0x7002002A,  //  0038  JMP		#0064
      0x600C000C,  //  0039  GETGBL	R3	G12
      0x5C100200,  //  003A  MOVE	R4	R1
      0x7C0C0200,  //  003B  CALL	R3	1
      0x54120004,  //  003C  LDINT	R4	5
      0x1C0C0604,  //  003D  EQ	R3	R3	R4
      0x780E0011,  //  003E  JMPF	R3	#0051
      0x940C0308,  //  003F  GETIDX	R3	R1	K8
      0x94100304,  //  0040  GETIDX	R4	R1	K4
      0x94140309,  //  0041  GETIDX	R5	R1	K9
      0x541A0003,  //  0042  LDINT	R6	4
      0x94180206,  //  0043  GETIDX	R6	R1	R6
      0x601C0018,  //  0044  GETGBL	R7	G24
      0x5820000A,  //  0045  LDCONST	R8	K10
      0x5C240600,  //  0046  MOVE	R9	R3
      0x5C280600,  //  0047  MOVE	R10	R3
      0x5C2C0800,  //  0048  MOVE	R11	R4
      0x5C300800,  //  0049  MOVE	R12	R4
      0x5C340A00,  //  004A  MOVE	R13	R5
      0x5C380A00,  //  004B  MOVE	R14	R5
      0x5C3C0C00,  //  004C  MOVE	R15	R6
      0x5C400C00,  //  004D  MOVE	R16	R6
      0x7C1C1200,  //  004E  CALL	R7	9
      0x80040E00,  //  004F  RET	1	R7
      0x70020012,  //  0050  JMP		#0064
      0x600C000C,  //  0051  GETGBL	R3	G12
      0x5C100200,  //  0052  MOVE	R4	R1
      0x7C0C0200,  //  0053  CALL	R3	1
      0x54120003,  //  0054  LDINT	R4	4
      0x1C0C0604,  //  0055  EQ	R3	R3	R4
      0x780E000C,  //  0056  JMPF	R3	#0064
      0x940C0308,  //  0057  GETIDX	R3	R1	K8
      0x94100304,  //  0058  GETIDX	R4	R1	K4
      0x94140309,  //  0059  GETIDX	R5	R1	K9
      0x60180018,  //  005A  GETGBL	R6	G24
      0x581C000B,  //  005B  LDCONST	R7	K11
      0x5C200600,  //  005C  MOVE	R8	R3
      0x5C240600,  //  005D  MOVE	R9	R3
      0x5C280800,  //  005E  MOVE	R10	R4
      0x5C2C0800,  //  005F  MOVE	R11	R4
      0x5C300A00,  //  0060  MOVE	R12	R5
      0x5C340A00,  //  0061  MOVE	R13	R5
      0x7C180E00,  //  0062  CALL	R6	7
      0x80040C00,  //  0063  RET	1	R6
      0xB80E1800,  //  0064  GETNGBL	R3	K12
      0x8C0C070D,  //  0065  GETMET	R3	R3	K13
      0x5C140200,  //  0066  MOVE	R5	R1
      0x7C0C0400,  //  0067  CALL	R3	2
      0x780E0003,  //  0068  JMPF	R3	#006D
      0x8C0C010E,  //  0069  GETMET	R3	R0	K14
      0x5C140200,  //  006A  MOVE	R5	R1
      0x7C0C0400,  //  006B  CALL	R3	2
      0x80040600,  //  006C  RET	1	R3
      0x80061E00,  //  006D  RET	1	K15
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: SimpleDSLTranspiler
********************************************************************/
be_local_class(SimpleDSLTranspiler,
    9,
    NULL,
    be_nested_map(129,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(_process_named_arguments_generic, 86), be_const_closure(class_SimpleDSLTranspiler__process_named_arguments_generic_closure) },
        { be_const_key_weak(CONTEXT_ANIMATION, -1), be_const_int(3) },
        { be_const_key_weak(CONTEXT_PROPERTY, 113), be_const_int(5) },
        { be_const_key_weak(convert_color, -1), be_const_closure(class_SimpleDSLTranspiler_convert_color_closure) },
        { be_const_key_weak(_create_instance_for_validation, -1), be_const_closure(class_SimpleDSLTranspiler__create_instance_for_validation_closure) },
        { be_const_key_weak(CONTEXT_REPEAT_COUNT, 48), be_const_int(6) },
        { be_const_key_weak(process_run, -1), be_const_closure(class_SimpleDSLTranspiler_process_run_closure) },
        { be_const_key_weak(process_nested_function_call, -1), be_const_closure(class_SimpleDSLTranspiler_process_nested_function_call_closure) },
        { be_const_key_weak(expect_left_paren, -1), be_const_closure(class_SimpleDSLTranspiler_expect_left_paren_closure) },
        { be_const_key_weak(_create_symbol_by_return_type, 104), be_const_closure(class_SimpleDSLTranspiler__create_symbol_by_return_type_closure) },
        { be_const_key_weak(_validate_value_provider_reference, -1), be_const_closure(class_SimpleDSLTranspiler__validate_value_provider_reference_closure) },
        { be_const_key_weak(has_template_calls, -1), be_const_var(8) },
        { be_const_key_weak(_validate_animation_factory_creates_animation, -1), be_const_closure(class_SimpleDSLTranspiler__validate_animation_factory_creates_animation_closure) },
        { be_const_key_weak(check_right_brace, 39), be_const_closure(class_SimpleDSLTranspiler_check_right_brace_closure) },
        { be_const_key_weak(process_set, -1), be_const_closure(class_SimpleDSLTranspiler_process_set_closure) },
        { be_const_key_weak(process_sequence_assignment_fluent, -1), be_const_closure(class_SimpleDSLTranspiler_process_sequence_assignment_fluent_closure) },
        { be_const_key_weak(process_percentage_value, -1), be_const_closure(class_SimpleDSLTranspiler_process_percentage_value_closure) },
        { be_const_key_weak(ExpressionResult, -1), be_const_class(be_class_ExpressionResult) },
        { be_const_key_weak(CONTEXT_ARGUMENT, -1), be_const_int(4) },
        { be_const_key_weak(transform_expression_for_closure, -1), be_const_closure(class_SimpleDSLTranspiler_transform_expression_for_closure_closure) },
        { be_const_key_weak(check_right_paren, -1), be_const_closure(class_SimpleDSLTranspiler_check_right_paren_closure) },
        { be_const_key_weak(process_wait_statement_fluent, -1), be_const_closure(class_SimpleDSLTranspiler_process_wait_statement_fluent_closure) },
        { be_const_key_weak(process_sequence_assignment_generic, 126), be_const_closure(class_SimpleDSLTranspiler_process_sequence_assignment_generic_closure) },
        { be_const_key_weak(errors, -1), be_const_var(3) },
        { be_const_key_weak(run_statements, 49), be_const_var(4) },
        { be_const_key_weak(generate_template_function, 7), be_const_closure(class_SimpleDSLTranspiler_generate_template_function_closure) },
        { be_const_key_weak(join_output, -1), be_const_closure(class_SimpleDSLTranspiler_join_output_closure) },
        { be_const_key_weak(expect_left_brace, -1), be_const_closure(class_SimpleDSLTranspiler_expect_left_brace_closure) },
        { be_const_key_weak(expect_colon, -1), be_const_closure(class_SimpleDSLTranspiler_expect_colon_closure) },
        { be_const_key_weak(CONTEXT_GENERIC, 99), be_const_int(10) },
        { be_const_key_weak(process_event_handler, -1), be_const_closure(class_SimpleDSLTranspiler_process_event_handler_closure) },
        { be_const_key_weak(next, -1), be_const_closure(class_SimpleDSLTranspiler_next_closure) },
        { be_const_key_weak(CONTEXT_TIME, -1), be_const_int(8) },
        { be_const_key_weak(_validate_object_reference, 123), be_const_closure(class_SimpleDSLTranspiler__validate_object_reference_closure) },
        { be_const_key_weak(tokens, 95), be_const_var(0) },
        { be_const_key_weak(_validate_animation_factory_exists, 36), be_const_closure(class_SimpleDSLTranspiler__validate_animation_factory_exists_closure) },
        { be_const_key_weak(process_color, -1), be_const_closure(class_SimpleDSLTranspiler_process_color_closure) },
        { be_const_key_weak(_process_parameters_core, -1), be_const_closure(class_SimpleDSLTranspiler__process_parameters_core_closure) },
        { be_const_key_weak(skip_function_arguments, -1), be_const_closure(class_SimpleDSLTranspiler_skip_function_arguments_closure) },
        { be_const_key_weak(pos, 62), be_const_var(1) },
        { be_const_key_weak(expect_comma, -1), be_const_closure(class_SimpleDSLTranspiler_expect_comma_closure) },
        { be_const_key_weak(process_additive_expression, -1), be_const_closure(class_SimpleDSLTranspiler_process_additive_expression_closure) },
        { be_const_key_weak(get_errors, 29), be_const_closure(class_SimpleDSLTranspiler_get_errors_closure) },
        { be_const_key_weak(is_computed_expression_string, -1), be_const_closure(class_SimpleDSLTranspiler_is_computed_expression_string_closure) },
        { be_const_key_weak(expect_right_paren, -1), be_const_closure(class_SimpleDSLTranspiler_expect_right_paren_closure) },
        { be_const_key_weak(process_import, -1), be_const_closure(class_SimpleDSLTranspiler_process_import_closure) },
        { be_const_key_weak(process_berry_code_block, 100), be_const_closure(class_SimpleDSLTranspiler_process_berry_code_block_closure) },
        { be_const_key_weak(_process_named_arguments_for_color_provider, 23), be_const_closure(class_SimpleDSLTranspiler__process_named_arguments_for_color_provider_closure) },
        { be_const_key_weak(process_function_arguments, 64), be_const_closure(class_SimpleDSLTranspiler_process_function_arguments_closure) },
        { be_const_key_weak(is_identifier_char, 28), be_const_closure(class_SimpleDSLTranspiler_is_identifier_char_closure) },
        { be_const_key_weak(transpile_template_body, -1), be_const_closure(class_SimpleDSLTranspiler_transpile_template_body_closure) },
        { be_const_key_weak(generate_engine_run, -1), be_const_closure(class_SimpleDSLTranspiler_generate_engine_run_closure) },
        { be_const_key_weak(get_named_color_value, -1), be_const_closure(class_SimpleDSLTranspiler_get_named_color_value_closure) },
        { be_const_key_weak(process_event_parameters, -1), be_const_closure(class_SimpleDSLTranspiler_process_event_parameters_closure) },
        { be_const_key_weak(_process_named_arguments_unified, 112), be_const_closure(class_SimpleDSLTranspiler__process_named_arguments_unified_closure) },
        { be_const_key_weak(indent_level, -1), be_const_var(7) },
        { be_const_key_weak(symbol_table, -1), be_const_var(6) },
        { be_const_key_weak(process_named_arguments_for_variable, 32), be_const_closure(class_SimpleDSLTranspiler_process_named_arguments_for_variable_closure) },
        { be_const_key_weak(process_sequence_assignment, -1), be_const_closure(class_SimpleDSLTranspiler_process_sequence_assignment_closure) },
        { be_const_key_weak(validate_symbol_reference, -1), be_const_closure(class_SimpleDSLTranspiler_validate_symbol_reference_closure) },
        { be_const_key_weak(current, -1), be_const_closure(class_SimpleDSLTranspiler_current_closure) },
        { be_const_key_weak(process_statement, -1), be_const_closure(class_SimpleDSLTranspiler_process_statement_closure) },
        { be_const_key_weak(_validate_value_provider_factory_exists, -1), be_const_closure(class_SimpleDSLTranspiler__validate_value_provider_factory_exists_closure) },
        { be_const_key_weak(_process_named_arguments_for_animation, -1), be_const_closure(class_SimpleDSLTranspiler__process_named_arguments_for_animation_closure) },
        { be_const_key_weak(_is_named_arg_constructor, -1), be_const_closure(class_SimpleDSLTranspiler__is_named_arg_constructor_closure) },
        { be_const_key_weak(process_time_value, 119), be_const_closure(class_SimpleDSLTranspiler_process_time_value_closure) },
        { be_const_key_weak(CONTEXT_EXPRESSION, -1), be_const_int(9) },
        { be_const_key_weak(validate_user_name, -1), be_const_closure(class_SimpleDSLTranspiler_validate_user_name_closure) },
        { be_const_key_weak(process_multiplicative_expression, -1), be_const_closure(class_SimpleDSLTranspiler_process_multiplicative_expression_closure) },
        { be_const_key_weak(_has_function_parameters, 72), be_const_closure(class_SimpleDSLTranspiler__has_function_parameters_closure) },
        { be_const_key_weak(output, 21), be_const_var(2) },
        { be_const_key_weak(process_template, 31), be_const_closure(class_SimpleDSLTranspiler_process_template_closure) },
        { be_const_key_weak(collect_inline_comment, -1), be_const_closure(class_SimpleDSLTranspiler_collect_inline_comment_closure) },
        { be_const_key_weak(_process_simple_value_assignment, -1), be_const_closure(class_SimpleDSLTranspiler__process_simple_value_assignment_closure) },
        { be_const_key_weak(expect_left_bracket, -1), be_const_closure(class_SimpleDSLTranspiler_expect_left_bracket_closure) },
        { be_const_key_weak(expect_right_brace, -1), be_const_closure(class_SimpleDSLTranspiler_expect_right_brace_closure) },
        { be_const_key_weak(create_simple_function_from_string, -1), be_const_closure(class_SimpleDSLTranspiler_create_simple_function_from_string_closure) },
        { be_const_key_weak(has_errors, 82), be_const_closure(class_SimpleDSLTranspiler_has_errors_closure) },
        { be_const_key_weak(process_array_literal, -1), be_const_closure(class_SimpleDSLTranspiler_process_array_literal_closure) },
        { be_const_key_weak(process_unary_expression, -1), be_const_closure(class_SimpleDSLTranspiler_process_unary_expression_closure) },
        { be_const_key_weak(process_log_statement_fluent, -1), be_const_closure(class_SimpleDSLTranspiler_process_log_statement_fluent_closure) },
        { be_const_key_weak(process_standalone_log, 65), be_const_closure(class_SimpleDSLTranspiler_process_standalone_log_closure) },
        { be_const_key_weak(_validate_color_provider_factory_exists, -1), be_const_closure(class_SimpleDSLTranspiler__validate_color_provider_factory_exists_closure) },
        { be_const_key_weak(strip_initialized, 45), be_const_var(5) },
        { be_const_key_weak(get_error_report, -1), be_const_closure(class_SimpleDSLTranspiler_get_error_report_closure) },
        { be_const_key_weak(generate_default_strip_initialization, -1), be_const_closure(class_SimpleDSLTranspiler_generate_default_strip_initialization_closure) },
        { be_const_key_weak(expect_dot, -1), be_const_closure(class_SimpleDSLTranspiler_expect_dot_closure) },
        { be_const_key_weak(transpile, -1), be_const_closure(class_SimpleDSLTranspiler_transpile_closure) },
        { be_const_key_weak(can_use_as_identifier, -1), be_const_closure(class_SimpleDSLTranspiler_can_use_as_identifier_closure) },
        { be_const_key_weak(convert_time_to_ms, 78), be_const_closure(class_SimpleDSLTranspiler_convert_time_to_ms_closure) },
        { be_const_key_weak(skip_statement, -1), be_const_closure(class_SimpleDSLTranspiler_skip_statement_closure) },
        { be_const_key_weak(skip_whitespace, -1), be_const_closure(class_SimpleDSLTranspiler_skip_whitespace_closure) },
        { be_const_key_weak(_determine_symbol_return_type, 97), be_const_closure(class_SimpleDSLTranspiler__determine_symbol_return_type_closure) },
        { be_const_key_weak(process_property_assignment, 91), be_const_closure(class_SimpleDSLTranspiler_process_property_assignment_closure) },
        { be_const_key_weak(add, -1), be_const_closure(class_SimpleDSLTranspiler_add_closure) },
        { be_const_key_weak(process_value, -1), be_const_closure(class_SimpleDSLTranspiler_process_value_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_SimpleDSLTranspiler_init_closure) },
        { be_const_key_weak(expect_identifier, -1), be_const_closure(class_SimpleDSLTranspiler_expect_identifier_closure) },
        { be_const_key_weak(skip_whitespace_including_newlines, 84), be_const_closure(class_SimpleDSLTranspiler_skip_whitespace_including_newlines_closure) },
        { be_const_key_weak(process_log_call, 107), be_const_closure(class_SimpleDSLTranspiler_process_log_call_closure) },
        { be_const_key_weak(process_primary_expression, -1), be_const_closure(class_SimpleDSLTranspiler_process_primary_expression_closure) },
        { be_const_key_weak(get_indent, 75), be_const_closure(class_SimpleDSLTranspiler_get_indent_closure) },
        { be_const_key_weak(CONTEXT_VARIABLE, -1), be_const_int(1) },
        { be_const_key_weak(expect_keyword, 68), be_const_closure(class_SimpleDSLTranspiler_expect_keyword_closure) },
        { be_const_key_weak(process_restart_statement_fluent, 67), be_const_closure(class_SimpleDSLTranspiler_process_restart_statement_fluent_closure) },
        { be_const_key_weak(CONTEXT_ARRAY_ELEMENT, -1), be_const_int(7) },
        { be_const_key_weak(check_right_bracket, -1), be_const_closure(class_SimpleDSLTranspiler_check_right_bracket_closure) },
        { be_const_key_weak(process_play_statement_fluent, -1), be_const_closure(class_SimpleDSLTranspiler_process_play_statement_fluent_closure) },
        { be_const_key_weak(process_palette, 61), be_const_closure(class_SimpleDSLTranspiler_process_palette_closure) },
        { be_const_key_weak(CONTEXT_COLOR_PROVIDER, 53), be_const_int(11) },
        { be_const_key_weak(expect_assign, 52), be_const_closure(class_SimpleDSLTranspiler_expect_assign_closure) },
        { be_const_key_weak(expect_number, -1), be_const_closure(class_SimpleDSLTranspiler_expect_number_closure) },
        { be_const_key_weak(CONTEXT_COLOR, 118), be_const_int(2) },
        { be_const_key_weak(peek, -1), be_const_closure(class_SimpleDSLTranspiler_peek_closure) },
        { be_const_key_weak(at_end, 13), be_const_closure(class_SimpleDSLTranspiler_at_end_closure) },
        { be_const_key_weak(create_computation_closure_from_string, 37), be_const_closure(class_SimpleDSLTranspiler_create_computation_closure_from_string_closure) },
        { be_const_key_weak(is_computed_expression, -1), be_const_closure(class_SimpleDSLTranspiler_is_computed_expression_closure) },
        { be_const_key_weak(get_symbol_table_report, -1), be_const_closure(class_SimpleDSLTranspiler_get_symbol_table_report_closure) },
        { be_const_key_weak(_validate_single_parameter, -1), be_const_closure(class_SimpleDSLTranspiler__validate_single_parameter_closure) },
        { be_const_key_weak(process_palette_color, -1), be_const_closure(class_SimpleDSLTranspiler_process_palette_color_closure) },
        { be_const_key_weak(_process_user_function_call, -1), be_const_closure(class_SimpleDSLTranspiler__process_user_function_call_closure) },
        { be_const_key_weak(_determine_function_return_type, 24), be_const_closure(class_SimpleDSLTranspiler__determine_function_return_type_closure) },
        { be_const_key_weak(process_animation, -1), be_const_closure(class_SimpleDSLTranspiler_process_animation_closure) },
        { be_const_key_weak(expect_right_bracket, 116), be_const_closure(class_SimpleDSLTranspiler_expect_right_bracket_closure) },
        { be_const_key_weak(process_function_call, 18), be_const_closure(class_SimpleDSLTranspiler_process_function_call_closure) },
        { be_const_key_weak(convert_to_vrgb, 17), be_const_closure(class_SimpleDSLTranspiler_convert_to_vrgb_closure) },
        { be_const_key_weak(process_sequence, -1), be_const_closure(class_SimpleDSLTranspiler_process_sequence_closure) },
        { be_const_key_weak(error, -1), be_const_closure(class_SimpleDSLTranspiler_error_closure) },
        { be_const_key_weak(process_sequence_statement, 3), be_const_closure(class_SimpleDSLTranspiler_process_sequence_statement_closure) },
    })),
    be_str_weak(SimpleDSLTranspiler)
);
// compact class 'SymbolTable' ktab size: 55, total: 115 (saved 480 bytes)
static const bvalue be_ktab_class_SymbolTable[55] = {
  /* K0   */  be_nested_str_weak(get),
  /* K1   */  be_nested_str_weak(takes_named_args),
  /* K2   */  be_nested_str_weak(animation_dsl),
  /* K3   */  be_nested_str_weak(_symbol_entry),
  /* K4   */  be_nested_str_weak(create_animation_instance),
  /* K5   */  be_nested_str_weak(add),
  /* K6   */  be_nested_str_weak(takes_positional_args),
  /* K7   */  be_nested_str_weak(entries),
  /* K8   */  be_nested_str_weak(contains),
  /* K9   */  be_nested_str_weak(introspect),
  /* K10  */  be_nested_str_weak(named_colors),
  /* K11  */  be_nested_str_weak(create_color_instance),
  /* K12  */  be_nested_str_weak(log),
  /* K13  */  be_nested_str_weak(create_user_function),
  /* K14  */  be_nested_str_weak(animation),
  /* K15  */  be_nested_str_weak(is_user_function),
  /* K16  */  be_nested_str_weak(_math),
  /* K17  */  be_nested_str_weak(create_math_function),
  /* K18  */  be_nested_str_weak(create_palette_constant),
  /* K19  */  be_nested_str_weak(int),
  /* K20  */  be_nested_str_weak(create_constant),
  /* K21  */  be_nested_str_weak(function),
  /* K22  */  be_nested_str_weak(class),
  /* K23  */  be_nested_str_weak(mock_engine),
  /* K24  */  be_nested_str_weak(color_provider),
  /* K25  */  be_nested_str_weak(create_color_constructor),
  /* K26  */  be_nested_str_weak(value_provider),
  /* K27  */  be_nested_str_weak(create_value_provider_constructor),
  /* K28  */  be_nested_str_weak(create_animation_constructor),
  /* K29  */  be_nested_str_weak(is_dangerous_call),
  /* K30  */  be_nested_str_weak(_detect_and_cache_symbol),
  /* K31  */  be_nested_str_weak(create_value_provider_instance),
  /* K32  */  be_nested_str_weak(is_builtin),
  /* K33  */  be_nested_str_weak(type),
  /* K34  */  be_nested_str_weak(TYPE_COLOR),
  /* K35  */  be_nested_str_weak(0xFFFFFFFF),
  /* K36  */  be_nested_str_weak(get_reference),
  /* K37  */  be_nested_str_weak(_X25s_),
  /* K38  */  be_nested_str_weak(create_template),
  /* K39  */  be_nested_str_weak(set_param_types),
  /* K40  */  be_nested_str_weak(Cannot_X20define_X20_X27_X25s_X27_X20as_X20_X25s_X20_X2D_X20it_X20conflicts_X20with_X20built_X2Din_X20_X25s),
  /* K41  */  be_nested_str_weak(type_to_string),
  /* K42  */  be_nested_str_weak(symbol_redefinition_error),
  /* K43  */  be_nested_str_weak(find),
  /* K44  */  be_nested_str_weak(Cannot_X20redefine_X20symbol_X20_X27_X25s_X27_X20as_X20_X25s_X20_X2D_X20it_X27s_X20already_X20defined_X20as_X20_X25s),
  /* K45  */  be_nested_str_weak(MockEngine),
  /* K46  */  be_nested_str_weak(create_sequence),
  /* K47  */  be_nested_str_weak(takes_args),
  /* K48  */  be_nested_str_weak(create_palette_instance),
  /* K49  */  be_nested_str_weak(create_variable),
  /* K50  */  be_nested_str_weak(keys),
  /* K51  */  be_nested_str_weak(push),
  /* K52  */  be_nested_str_weak(_X25s_X3A_X20_X25s),
  /* K53  */  be_nested_str_weak(stop_iteration),
  /* K54  */  be_nested_str_weak(instance),
};


extern const bclass be_class_SymbolTable;

/********************************************************************
** Solidified function: takes_named_args
********************************************************************/
be_local_closure(class_SymbolTable_takes_named_args,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolTable,     /* shared constants */
    be_str_weak(takes_named_args),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x200C0403,  //  0004  NE	R3	R2	R3
      0x780E0002,  //  0005  JMPF	R3	#0009
      0x8C0C0501,  //  0006  GETMET	R3	R2	K1
      0x7C0C0200,  //  0007  CALL	R3	1
      0x70020000,  //  0008  JMP		#000A
      0x500C0000,  //  0009  LDBOOL	R3	0	0
      0x80040600,  //  000A  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_animation
********************************************************************/
be_local_closure(class_SymbolTable_create_animation,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolTable,     /* shared constants */
    be_str_weak(create_animation),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0xB80E0400,  //  0000  GETNGBL	R3	K2
      0x880C0703,  //  0001  GETMBR	R3	R3	K3
      0x8C0C0704,  //  0002  GETMET	R3	R3	K4
      0x5C140200,  //  0003  MOVE	R5	R1
      0x5C180400,  //  0004  MOVE	R6	R2
      0x501C0000,  //  0005  LDBOOL	R7	0	0
      0x7C0C0800,  //  0006  CALL	R3	4
      0x8C100105,  //  0007  GETMET	R4	R0	K5
      0x5C180200,  //  0008  MOVE	R6	R1
      0x5C1C0600,  //  0009  MOVE	R7	R3
      0x7C100600,  //  000A  CALL	R4	3
      0x80040800,  //  000B  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: takes_positional_args
********************************************************************/
be_local_closure(class_SymbolTable_takes_positional_args,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolTable,     /* shared constants */
    be_str_weak(takes_positional_args),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x200C0403,  //  0004  NE	R3	R2	R3
      0x780E0002,  //  0005  JMPF	R3	#0009
      0x8C0C0506,  //  0006  GETMET	R3	R2	K6
      0x7C0C0200,  //  0007  CALL	R3	1
      0x70020000,  //  0008  JMP		#000A
      0x500C0000,  //  0009  LDBOOL	R3	0	0
      0x80040600,  //  000A  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _detect_and_cache_symbol
********************************************************************/
be_local_closure(class_SymbolTable__detect_and_cache_symbol,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolTable,     /* shared constants */
    be_str_weak(_detect_and_cache_symbol),
    &be_const_str_solidified,
    ( &(const binstruction[188]) {  /* code */
      0x88080107,  //  0000  GETMBR	R2	R0	K7
      0x8C080508,  //  0001  GETMET	R2	R2	K8
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x780A0002,  //  0004  JMPF	R2	#0008
      0x88080107,  //  0005  GETMBR	R2	R0	K7
      0x94080401,  //  0006  GETIDX	R2	R2	R1
      0x80040400,  //  0007  RET	1	R2
      0xA80200AB,  //  0008  EXBLK	0	#00B5
      0xA40A1200,  //  0009  IMPORT	R2	K9
      0xB80E0400,  //  000A  GETNGBL	R3	K2
      0x880C070A,  //  000B  GETMBR	R3	R3	K10
      0x8C0C0708,  //  000C  GETMET	R3	R3	K8
      0x5C140200,  //  000D  MOVE	R5	R1
      0x7C0C0400,  //  000E  CALL	R3	2
      0x780E000A,  //  000F  JMPF	R3	#001B
      0xB80E0400,  //  0010  GETNGBL	R3	K2
      0x880C0703,  //  0011  GETMBR	R3	R3	K3
      0x8C0C070B,  //  0012  GETMET	R3	R3	K11
      0x5C140200,  //  0013  MOVE	R5	R1
      0x4C180000,  //  0014  LDNIL	R6
      0x501C0200,  //  0015  LDBOOL	R7	1	0
      0x7C0C0800,  //  0016  CALL	R3	4
      0x88100107,  //  0017  GETMBR	R4	R0	K7
      0x98100203,  //  0018  SETIDX	R4	R1	R3
      0xA8040001,  //  0019  EXBLK	1	1
      0x80040600,  //  001A  RET	1	R3
      0x1C0C030C,  //  001B  EQ	R3	R1	K12
      0x780E0009,  //  001C  JMPF	R3	#0027
      0xB80E0400,  //  001D  GETNGBL	R3	K2
      0x880C0703,  //  001E  GETMBR	R3	R3	K3
      0x8C0C070D,  //  001F  GETMET	R3	R3	K13
      0x5814000C,  //  0020  LDCONST	R5	K12
      0x50180200,  //  0021  LDBOOL	R6	1	0
      0x7C0C0600,  //  0022  CALL	R3	3
      0x88100107,  //  0023  GETMBR	R4	R0	K7
      0x98100203,  //  0024  SETIDX	R4	R1	R3
      0xA8040001,  //  0025  EXBLK	1	1
      0x80040600,  //  0026  RET	1	R3
      0xB80E1C00,  //  0027  GETNGBL	R3	K14
      0x8C0C070F,  //  0028  GETMET	R3	R3	K15
      0x5C140200,  //  0029  MOVE	R5	R1
      0x7C0C0400,  //  002A  CALL	R3	2
      0x780E0009,  //  002B  JMPF	R3	#0036
      0xB80E0400,  //  002C  GETNGBL	R3	K2
      0x880C0703,  //  002D  GETMBR	R3	R3	K3
      0x8C0C070D,  //  002E  GETMET	R3	R3	K13
      0x5C140200,  //  002F  MOVE	R5	R1
      0x50180200,  //  0030  LDBOOL	R6	1	0
      0x7C0C0600,  //  0031  CALL	R3	3
      0x88100107,  //  0032  GETMBR	R4	R0	K7
      0x98100203,  //  0033  SETIDX	R4	R1	R3
      0xA8040001,  //  0034  EXBLK	1	1
      0x80040600,  //  0035  RET	1	R3
      0x8C0C0508,  //  0036  GETMET	R3	R2	K8
      0xB8161C00,  //  0037  GETNGBL	R5	K14
      0x88140B10,  //  0038  GETMBR	R5	R5	K16
      0x5C180200,  //  0039  MOVE	R6	R1
      0x7C0C0600,  //  003A  CALL	R3	3
      0x780E0009,  //  003B  JMPF	R3	#0046
      0xB80E0400,  //  003C  GETNGBL	R3	K2
      0x880C0703,  //  003D  GETMBR	R3	R3	K3
      0x8C0C0711,  //  003E  GETMET	R3	R3	K17
      0x5C140200,  //  003F  MOVE	R5	R1
      0x50180200,  //  0040  LDBOOL	R6	1	0
      0x7C0C0600,  //  0041  CALL	R3	3
      0x88100107,  //  0042  GETMBR	R4	R0	K7
      0x98100203,  //  0043  SETIDX	R4	R1	R3
      0xA8040001,  //  0044  EXBLK	1	1
      0x80040600,  //  0045  RET	1	R3
      0x8C0C0508,  //  0046  GETMET	R3	R2	K8
      0xB8161C00,  //  0047  GETNGBL	R5	K14
      0x5C180200,  //  0048  MOVE	R6	R1
      0x7C0C0600,  //  0049  CALL	R3	3
      0x780E0064,  //  004A  JMPF	R3	#00B0
      0xB80E1C00,  //  004B  GETNGBL	R3	K14
      0x880C0601,  //  004C  GETMBR	R3	R3	R1
      0x60100004,  //  004D  GETGBL	R4	G4
      0x5C140600,  //  004E  MOVE	R5	R3
      0x7C100200,  //  004F  CALL	R4	1
      0x6014000F,  //  0050  GETGBL	R5	G15
      0x5C180600,  //  0051  MOVE	R6	R3
      0x601C0015,  //  0052  GETGBL	R7	G21
      0x7C140400,  //  0053  CALL	R5	2
      0x7816000A,  //  0054  JMPF	R5	#0060
      0xB8160400,  //  0055  GETNGBL	R5	K2
      0x88140B03,  //  0056  GETMBR	R5	R5	K3
      0x8C140B12,  //  0057  GETMET	R5	R5	K18
      0x5C1C0200,  //  0058  MOVE	R7	R1
      0x5C200600,  //  0059  MOVE	R8	R3
      0x50240200,  //  005A  LDBOOL	R9	1	0
      0x7C140800,  //  005B  CALL	R5	4
      0x88180107,  //  005C  GETMBR	R6	R0	K7
      0x98180205,  //  005D  SETIDX	R6	R1	R5
      0xA8040001,  //  005E  EXBLK	1	1
      0x80040A00,  //  005F  RET	1	R5
      0x1C140913,  //  0060  EQ	R5	R4	K19
      0x7816000A,  //  0061  JMPF	R5	#006D
      0xB8160400,  //  0062  GETNGBL	R5	K2
      0x88140B03,  //  0063  GETMBR	R5	R5	K3
      0x8C140B14,  //  0064  GETMET	R5	R5	K20
      0x5C1C0200,  //  0065  MOVE	R7	R1
      0x5C200600,  //  0066  MOVE	R8	R3
      0x50240200,  //  0067  LDBOOL	R9	1	0
      0x7C140800,  //  0068  CALL	R5	4
      0x88180107,  //  0069  GETMBR	R6	R0	K7
      0x98180205,  //  006A  SETIDX	R6	R1	R5
      0xA8040001,  //  006B  EXBLK	1	1
      0x80040A00,  //  006C  RET	1	R5
      0x1C140915,  //  006D  EQ	R5	R4	K21
      0x74160001,  //  006E  JMPT	R5	#0071
      0x1C140916,  //  006F  EQ	R5	R4	K22
      0x7816003E,  //  0070  JMPF	R5	#00B0
      0xA8020039,  //  0071  EXBLK	0	#00AC
      0x5C140600,  //  0072  MOVE	R5	R3
      0x88180117,  //  0073  GETMBR	R6	R0	K23
      0x7C140200,  //  0074  CALL	R5	1
      0x6018000F,  //  0075  GETGBL	R6	G15
      0x5C1C0A00,  //  0076  MOVE	R7	R5
      0xB8221C00,  //  0077  GETNGBL	R8	K14
      0x88201118,  //  0078  GETMBR	R8	R8	K24
      0x7C180400,  //  0079  CALL	R6	2
      0x781A000B,  //  007A  JMPF	R6	#0087
      0xB81A0400,  //  007B  GETNGBL	R6	K2
      0x88180D03,  //  007C  GETMBR	R6	R6	K3
      0x8C180D19,  //  007D  GETMET	R6	R6	K25
      0x5C200200,  //  007E  MOVE	R8	R1
      0x5C240A00,  //  007F  MOVE	R9	R5
      0x50280200,  //  0080  LDBOOL	R10	1	0
      0x7C180800,  //  0081  CALL	R6	4
      0x881C0107,  //  0082  GETMBR	R7	R0	K7
      0x981C0206,  //  0083  SETIDX	R7	R1	R6
      0xA8040002,  //  0084  EXBLK	1	2
      0x80040C00,  //  0085  RET	1	R6
      0x70020022,  //  0086  JMP		#00AA
      0x6018000F,  //  0087  GETGBL	R6	G15
      0x5C1C0A00,  //  0088  MOVE	R7	R5
      0xB8221C00,  //  0089  GETNGBL	R8	K14
      0x8820111A,  //  008A  GETMBR	R8	R8	K26
      0x7C180400,  //  008B  CALL	R6	2
      0x781A000B,  //  008C  JMPF	R6	#0099
      0xB81A0400,  //  008D  GETNGBL	R6	K2
      0x88180D03,  //  008E  GETMBR	R6	R6	K3
      0x8C180D1B,  //  008F  GETMET	R6	R6	K27
      0x5C200200,  //  0090  MOVE	R8	R1
      0x5C240A00,  //  0091  MOVE	R9	R5
      0x50280200,  //  0092  LDBOOL	R10	1	0
      0x7C180800,  //  0093  CALL	R6	4
      0x881C0107,  //  0094  GETMBR	R7	R0	K7
      0x981C0206,  //  0095  SETIDX	R7	R1	R6
      0xA8040002,  //  0096  EXBLK	1	2
      0x80040C00,  //  0097  RET	1	R6
      0x70020010,  //  0098  JMP		#00AA
      0x6018000F,  //  0099  GETGBL	R6	G15
      0x5C1C0A00,  //  009A  MOVE	R7	R5
      0xB8221C00,  //  009B  GETNGBL	R8	K14
      0x8820110E,  //  009C  GETMBR	R8	R8	K14
      0x7C180400,  //  009D  CALL	R6	2
      0x781A000A,  //  009E  JMPF	R6	#00AA
      0xB81A0400,  //  009F  GETNGBL	R6	K2
      0x88180D03,  //  00A0  GETMBR	R6	R6	K3
      0x8C180D1C,  //  00A1  GETMET	R6	R6	K28
      0x5C200200,  //  00A2  MOVE	R8	R1
      0x5C240A00,  //  00A3  MOVE	R9	R5
      0x50280200,  //  00A4  LDBOOL	R10	1	0
      0x7C180800,  //  00A5  CALL	R6	4
      0x881C0107,  //  00A6  GETMBR	R7	R0	K7
      0x981C0206,  //  00A7  SETIDX	R7	R1	R6
      0xA8040002,  //  00A8  EXBLK	1	2
      0x80040C00,  //  00A9  RET	1	R6
      0xA8040001,  //  00AA  EXBLK	1	1
      0x70020003,  //  00AB  JMP		#00B0
      0xAC140002,  //  00AC  CATCH	R5	0	2
      0x70020000,  //  00AD  JMP		#00AF
      0x70020000,  //  00AE  JMP		#00B0
      0xB0080000,  //  00AF  RAISE	2	R0	R0
      0x4C0C0000,  //  00B0  LDNIL	R3
      0xA8040001,  //  00B1  EXBLK	1	1
      0x80040600,  //  00B2  RET	1	R3
      0xA8040001,  //  00B3  EXBLK	1	1
      0x70020005,  //  00B4  JMP		#00BB
      0xAC080002,  //  00B5  CATCH	R2	0	2
      0x70020002,  //  00B6  JMP		#00BA
      0x4C100000,  //  00B7  LDNIL	R4
      0x80040800,  //  00B8  RET	1	R4
      0x70020000,  //  00B9  JMP		#00BB
      0xB0080000,  //  00BA  RAISE	2	R0	R0
      0x80000000,  //  00BB  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_dangerous
********************************************************************/
be_local_closure(class_SymbolTable_is_dangerous,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolTable,     /* shared constants */
    be_str_weak(is_dangerous),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x200C0403,  //  0004  NE	R3	R2	R3
      0x780E0002,  //  0005  JMPF	R3	#0009
      0x8C0C051D,  //  0006  GETMET	R3	R2	K29
      0x7C0C0200,  //  0007  CALL	R3	1
      0x70020000,  //  0008  JMP		#000A
      0x500C0000,  //  0009  LDBOOL	R3	0	0
      0x80040600,  //  000A  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_color
********************************************************************/
be_local_closure(class_SymbolTable_create_color,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolTable,     /* shared constants */
    be_str_weak(create_color),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0xB80E0400,  //  0000  GETNGBL	R3	K2
      0x880C0703,  //  0001  GETMBR	R3	R3	K3
      0x8C0C070B,  //  0002  GETMET	R3	R3	K11
      0x5C140200,  //  0003  MOVE	R5	R1
      0x5C180400,  //  0004  MOVE	R6	R2
      0x501C0000,  //  0005  LDBOOL	R7	0	0
      0x7C0C0800,  //  0006  CALL	R3	4
      0x8C100105,  //  0007  GETMET	R4	R0	K5
      0x5C180200,  //  0008  MOVE	R6	R1
      0x5C1C0600,  //  0009  MOVE	R7	R3
      0x7C100600,  //  000A  CALL	R4	3
      0x80040800,  //  000B  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: contains
********************************************************************/
be_local_closure(class_SymbolTable_contains,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolTable,     /* shared constants */
    be_str_weak(contains),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x88080107,  //  0000  GETMBR	R2	R0	K7
      0x8C080508,  //  0001  GETMET	R2	R2	K8
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x780A0001,  //  0004  JMPF	R2	#0007
      0x50080200,  //  0005  LDBOOL	R2	1	0
      0x80040400,  //  0006  RET	1	R2
      0x8C08011E,  //  0007  GETMET	R2	R0	K30
      0x5C100200,  //  0008  MOVE	R4	R1
      0x7C080400,  //  0009  CALL	R2	2
      0x4C0C0000,  //  000A  LDNIL	R3
      0x200C0403,  //  000B  NE	R3	R2	R3
      0x80040600,  //  000C  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_value_provider
********************************************************************/
be_local_closure(class_SymbolTable_create_value_provider,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolTable,     /* shared constants */
    be_str_weak(create_value_provider),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0xB80E0400,  //  0000  GETNGBL	R3	K2
      0x880C0703,  //  0001  GETMBR	R3	R3	K3
      0x8C0C071F,  //  0002  GETMET	R3	R3	K31
      0x5C140200,  //  0003  MOVE	R5	R1
      0x5C180400,  //  0004  MOVE	R6	R2
      0x501C0000,  //  0005  LDBOOL	R7	0	0
      0x7C0C0800,  //  0006  CALL	R3	4
      0x8C100105,  //  0007  GETMET	R4	R0	K5
      0x5C180200,  //  0008  MOVE	R6	R1
      0x5C1C0600,  //  0009  MOVE	R7	R3
      0x7C100600,  //  000A  CALL	R4	3
      0x80040800,  //  000B  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _get_named_color_value
********************************************************************/
be_local_closure(class_SymbolTable__get_named_color_value,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolTable,     /* shared constants */
    be_str_weak(_get_named_color_value),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x200C0403,  //  0004  NE	R3	R2	R3
      0x780E000B,  //  0005  JMPF	R3	#0012
      0x880C0520,  //  0006  GETMBR	R3	R2	K32
      0x780E0009,  //  0007  JMPF	R3	#0012
      0x880C0521,  //  0008  GETMBR	R3	R2	K33
      0xB8120400,  //  0009  GETNGBL	R4	K2
      0x88100903,  //  000A  GETMBR	R4	R4	K3
      0x88100922,  //  000B  GETMBR	R4	R4	K34
      0x1C0C0604,  //  000C  EQ	R3	R3	R4
      0x780E0003,  //  000D  JMPF	R3	#0012
      0xB80E0400,  //  000E  GETNGBL	R3	K2
      0x880C070A,  //  000F  GETMBR	R3	R3	K10
      0x940C0601,  //  0010  GETIDX	R3	R3	R1
      0x80040600,  //  0011  RET	1	R3
      0x80064600,  //  0012  RET	1	K35
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_reference
********************************************************************/
be_local_closure(class_SymbolTable_get_reference,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolTable,     /* shared constants */
    be_str_weak(get_reference),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x200C0403,  //  0004  NE	R3	R2	R3
      0x780E000E,  //  0005  JMPF	R3	#0015
      0x880C0520,  //  0006  GETMBR	R3	R2	K32
      0x780E0009,  //  0007  JMPF	R3	#0012
      0x880C0521,  //  0008  GETMBR	R3	R2	K33
      0xB8120400,  //  0009  GETNGBL	R4	K2
      0x88100903,  //  000A  GETMBR	R4	R4	K3
      0x88100922,  //  000B  GETMBR	R4	R4	K34
      0x1C0C0604,  //  000C  EQ	R3	R3	R4
      0x780E0003,  //  000D  JMPF	R3	#0012
      0xB80E0400,  //  000E  GETNGBL	R3	K2
      0x880C070A,  //  000F  GETMBR	R3	R3	K10
      0x940C0601,  //  0010  GETIDX	R3	R3	R1
      0x80040600,  //  0011  RET	1	R3
      0x8C0C0524,  //  0012  GETMET	R3	R2	K36
      0x7C0C0200,  //  0013  CALL	R3	1
      0x80040600,  //  0014  RET	1	R3
      0x600C0018,  //  0015  GETGBL	R3	G24
      0x58100025,  //  0016  LDCONST	R4	K37
      0x5C140200,  //  0017  MOVE	R5	R1
      0x7C0C0400,  //  0018  CALL	R3	2
      0x80040600,  //  0019  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_template
********************************************************************/
be_local_closure(class_SymbolTable_create_template,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolTable,     /* shared constants */
    be_str_weak(create_template),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0xB80E0400,  //  0000  GETNGBL	R3	K2
      0x880C0703,  //  0001  GETMBR	R3	R3	K3
      0x8C0C0726,  //  0002  GETMET	R3	R3	K38
      0x5C140200,  //  0003  MOVE	R5	R1
      0x50180000,  //  0004  LDBOOL	R6	0	0
      0x7C0C0600,  //  0005  CALL	R3	3
      0x8C100727,  //  0006  GETMET	R4	R3	K39
      0x4C180000,  //  0007  LDNIL	R6
      0x20180406,  //  0008  NE	R6	R2	R6
      0x781A0001,  //  0009  JMPF	R6	#000C
      0x5C180400,  //  000A  MOVE	R6	R2
      0x70020001,  //  000B  JMP		#000E
      0x60180013,  //  000C  GETGBL	R6	G19
      0x7C180000,  //  000D  CALL	R6	0
      0x7C100400,  //  000E  CALL	R4	2
      0x8C100105,  //  000F  GETMET	R4	R0	K5
      0x5C180200,  //  0010  MOVE	R6	R1
      0x5C1C0600,  //  0011  MOVE	R7	R3
      0x7C100600,  //  0012  CALL	R4	3
      0x80040800,  //  0013  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_generic
********************************************************************/
be_local_closure(class_SymbolTable_create_generic,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolTable,     /* shared constants */
    be_str_weak(create_generic),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0xB8160400,  //  0000  GETNGBL	R5	K2
      0x8C140B03,  //  0001  GETMET	R5	R5	K3
      0x5C1C0200,  //  0002  MOVE	R7	R1
      0x5C200400,  //  0003  MOVE	R8	R2
      0x5C240600,  //  0004  MOVE	R9	R3
      0x4C280000,  //  0005  LDNIL	R10
      0x2028080A,  //  0006  NE	R10	R4	R10
      0x782A0001,  //  0007  JMPF	R10	#000A
      0x5C280800,  //  0008  MOVE	R10	R4
      0x70020000,  //  0009  JMP		#000B
      0x50280000,  //  000A  LDBOOL	R10	0	0
      0x7C140A00,  //  000B  CALL	R5	5
      0x8C180105,  //  000C  GETMET	R6	R0	K5
      0x5C200200,  //  000D  MOVE	R8	R1
      0x5C240A00,  //  000E  MOVE	R9	R5
      0x7C180600,  //  000F  CALL	R6	3
      0x80040C00,  //  0010  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add
********************************************************************/
be_local_closure(class_SymbolTable_add,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolTable,     /* shared constants */
    be_str_weak(add),
    &be_const_str_solidified,
    ( &(const binstruction[42]) {  /* code */
      0x8C0C011E,  //  0000  GETMET	R3	R0	K30
      0x5C140200,  //  0001  MOVE	R5	R1
      0x7C0C0400,  //  0002  CALL	R3	2
      0x4C100000,  //  0003  LDNIL	R4
      0x20100604,  //  0004  NE	R4	R3	R4
      0x7812000C,  //  0005  JMPF	R4	#0013
      0x88100721,  //  0006  GETMBR	R4	R3	K33
      0x88140521,  //  0007  GETMBR	R5	R2	K33
      0x20100805,  //  0008  NE	R4	R4	R5
      0x78120008,  //  0009  JMPF	R4	#0013
      0x60100018,  //  000A  GETGBL	R4	G24
      0x58140028,  //  000B  LDCONST	R5	K40
      0x5C180200,  //  000C  MOVE	R6	R1
      0x8C1C0529,  //  000D  GETMET	R7	R2	K41
      0x7C1C0200,  //  000E  CALL	R7	1
      0x8C200729,  //  000F  GETMET	R8	R3	K41
      0x7C200200,  //  0010  CALL	R8	1
      0x7C100800,  //  0011  CALL	R4	4
      0xB0065404,  //  0012  RAISE	1	K42	R4
      0x88100107,  //  0013  GETMBR	R4	R0	K7
      0x8C10092B,  //  0014  GETMET	R4	R4	K43
      0x5C180200,  //  0015  MOVE	R6	R1
      0x7C100400,  //  0016  CALL	R4	2
      0x4C140000,  //  0017  LDNIL	R5
      0x20140805,  //  0018  NE	R5	R4	R5
      0x7816000C,  //  0019  JMPF	R5	#0027
      0x88140921,  //  001A  GETMBR	R5	R4	K33
      0x88180521,  //  001B  GETMBR	R6	R2	K33
      0x20140A06,  //  001C  NE	R5	R5	R6
      0x78160008,  //  001D  JMPF	R5	#0027
      0x60140018,  //  001E  GETGBL	R5	G24
      0x5818002C,  //  001F  LDCONST	R6	K44
      0x5C1C0200,  //  0020  MOVE	R7	R1
      0x8C200529,  //  0021  GETMET	R8	R2	K41
      0x7C200200,  //  0022  CALL	R8	1
      0x8C240929,  //  0023  GETMET	R9	R4	K41
      0x7C240200,  //  0024  CALL	R9	1
      0x7C140800,  //  0025  CALL	R5	4
      0xB0065405,  //  0026  RAISE	1	K42	R5
      0x88140107,  //  0027  GETMBR	R5	R0	K7
      0x98140202,  //  0028  SETIDX	R5	R1	R2
      0x80040400,  //  0029  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get
********************************************************************/
be_local_closure(class_SymbolTable_get,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolTable,     /* shared constants */
    be_str_weak(get),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x88080107,  //  0000  GETMBR	R2	R0	K7
      0x8C08052B,  //  0001  GETMET	R2	R2	K43
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x4C0C0000,  //  0004  LDNIL	R3
      0x200C0403,  //  0005  NE	R3	R2	R3
      0x780E0000,  //  0006  JMPF	R3	#0008
      0x80040400,  //  0007  RET	1	R2
      0x8C0C011E,  //  0008  GETMET	R3	R0	K30
      0x5C140200,  //  0009  MOVE	R5	R1
      0x7C0C0400,  //  000A  CALL	R3	2
      0x80040600,  //  000B  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_SymbolTable_init,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolTable,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x60040013,  //  0000  GETGBL	R1	G19
      0x7C040000,  //  0001  CALL	R1	0
      0x90020E01,  //  0002  SETMBR	R0	K7	R1
      0xB8060400,  //  0003  GETNGBL	R1	K2
      0x8C04032D,  //  0004  GETMET	R1	R1	K45
      0x7C040200,  //  0005  CALL	R1	1
      0x90022E01,  //  0006  SETMBR	R0	K23	R1
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_sequence
********************************************************************/
be_local_closure(class_SymbolTable_create_sequence,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolTable,     /* shared constants */
    be_str_weak(create_sequence),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0xB80A0400,  //  0000  GETNGBL	R2	K2
      0x88080503,  //  0001  GETMBR	R2	R2	K3
      0x8C08052E,  //  0002  GETMET	R2	R2	K46
      0x5C100200,  //  0003  MOVE	R4	R1
      0x50140000,  //  0004  LDBOOL	R5	0	0
      0x7C080600,  //  0005  CALL	R2	3
      0x8C0C0105,  //  0006  GETMET	R3	R0	K5
      0x5C140200,  //  0007  MOVE	R5	R1
      0x5C180400,  //  0008  MOVE	R6	R2
      0x7C0C0600,  //  0009  CALL	R3	3
      0x80040600,  //  000A  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: symbol_exists
********************************************************************/
be_local_closure(class_SymbolTable_symbol_exists,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolTable,     /* shared constants */
    be_str_weak(symbol_exists),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C080108,  //  0000  GETMET	R2	R0	K8
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x80040400,  //  0003  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: register_user_function
********************************************************************/
be_local_closure(class_SymbolTable_register_user_function,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolTable,     /* shared constants */
    be_str_weak(register_user_function),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x8C080108,  //  0000  GETMET	R2	R0	K8
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x740A0009,  //  0003  JMPT	R2	#000E
      0xB80A0400,  //  0004  GETNGBL	R2	K2
      0x88080503,  //  0005  GETMBR	R2	R2	K3
      0x8C08050D,  //  0006  GETMET	R2	R2	K13
      0x5C100200,  //  0007  MOVE	R4	R1
      0x50140000,  //  0008  LDBOOL	R5	0	0
      0x7C080600,  //  0009  CALL	R2	3
      0x8C0C0105,  //  000A  GETMET	R3	R0	K5
      0x5C140200,  //  000B  MOVE	R5	R1
      0x5C180400,  //  000C  MOVE	R6	R2
      0x7C0C0600,  //  000D  CALL	R3	3
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: takes_args
********************************************************************/
be_local_closure(class_SymbolTable_takes_args,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolTable,     /* shared constants */
    be_str_weak(takes_args),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x200C0403,  //  0004  NE	R3	R2	R3
      0x780E0001,  //  0005  JMPF	R3	#0008
      0x880C052F,  //  0006  GETMBR	R3	R2	K47
      0x70020000,  //  0007  JMP		#0009
      0x500C0000,  //  0008  LDBOOL	R3	0	0
      0x80040600,  //  0009  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_palette
********************************************************************/
be_local_closure(class_SymbolTable_create_palette,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolTable,     /* shared constants */
    be_str_weak(create_palette),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0xB80E0400,  //  0000  GETNGBL	R3	K2
      0x880C0703,  //  0001  GETMBR	R3	R3	K3
      0x8C0C0730,  //  0002  GETMET	R3	R3	K48
      0x5C140200,  //  0003  MOVE	R5	R1
      0x5C180400,  //  0004  MOVE	R6	R2
      0x501C0000,  //  0005  LDBOOL	R7	0	0
      0x7C0C0800,  //  0006  CALL	R3	4
      0x8C100105,  //  0007  GETMET	R4	R0	K5
      0x5C180200,  //  0008  MOVE	R6	R1
      0x5C1C0600,  //  0009  MOVE	R7	R3
      0x7C100600,  //  000A  CALL	R4	3
      0x80040800,  //  000B  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_variable
********************************************************************/
be_local_closure(class_SymbolTable_create_variable,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolTable,     /* shared constants */
    be_str_weak(create_variable),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0xB80A0400,  //  0000  GETNGBL	R2	K2
      0x88080503,  //  0001  GETMBR	R2	R2	K3
      0x8C080531,  //  0002  GETMET	R2	R2	K49
      0x5C100200,  //  0003  MOVE	R4	R1
      0x50140000,  //  0004  LDBOOL	R5	0	0
      0x7C080600,  //  0005  CALL	R2	3
      0x8C0C0105,  //  0006  GETMET	R3	R0	K5
      0x5C140200,  //  0007  MOVE	R5	R1
      0x5C180400,  //  0008  MOVE	R6	R2
      0x7C0C0600,  //  0009  CALL	R3	3
      0x80040600,  //  000A  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: list_symbols
********************************************************************/
be_local_closure(class_SymbolTable_list_symbols,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolTable,     /* shared constants */
    be_str_weak(list_symbols),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x60040012,  //  0000  GETGBL	R1	G18
      0x7C040000,  //  0001  CALL	R1	0
      0x60080010,  //  0002  GETGBL	R2	G16
      0x880C0107,  //  0003  GETMBR	R3	R0	K7
      0x8C0C0732,  //  0004  GETMET	R3	R3	K50
      0x7C0C0200,  //  0005  CALL	R3	1
      0x7C080200,  //  0006  CALL	R2	1
      0xA802000C,  //  0007  EXBLK	0	#0015
      0x5C0C0400,  //  0008  MOVE	R3	R2
      0x7C0C0000,  //  0009  CALL	R3	0
      0x88100107,  //  000A  GETMBR	R4	R0	K7
      0x94100803,  //  000B  GETIDX	R4	R4	R3
      0x8C140333,  //  000C  GETMET	R5	R1	K51
      0x601C0018,  //  000D  GETGBL	R7	G24
      0x58200034,  //  000E  LDCONST	R8	K52
      0x5C240600,  //  000F  MOVE	R9	R3
      0x8C280929,  //  0010  GETMET	R10	R4	K41
      0x7C280200,  //  0011  CALL	R10	1
      0x7C1C0600,  //  0012  CALL	R7	3
      0x7C140400,  //  0013  CALL	R5	2
      0x7001FFF2,  //  0014  JMP		#0008
      0x58080035,  //  0015  LDCONST	R2	K53
      0xAC080200,  //  0016  CATCH	R2	1	0
      0xB0080000,  //  0017  RAISE	2	R0	R0
      0x80040200,  //  0018  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_type
********************************************************************/
be_local_closure(class_SymbolTable_get_type,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolTable,     /* shared constants */
    be_str_weak(get_type),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x200C0403,  //  0004  NE	R3	R2	R3
      0x780E0002,  //  0005  JMPF	R3	#0009
      0x8C0C0529,  //  0006  GETMET	R3	R2	K41
      0x7C0C0200,  //  0007  CALL	R3	1
      0x70020000,  //  0008  JMP		#000A
      0x4C0C0000,  //  0009  LDNIL	R3
      0x80040600,  //  000A  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_instance
********************************************************************/
be_local_closure(class_SymbolTable_get_instance,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_SymbolTable,     /* shared constants */
    be_str_weak(get_instance),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x200C0403,  //  0004  NE	R3	R2	R3
      0x780E0001,  //  0005  JMPF	R3	#0008
      0x880C0536,  //  0006  GETMBR	R3	R2	K54
      0x70020000,  //  0007  JMP		#0009
      0x4C0C0000,  //  0008  LDNIL	R3
      0x80040600,  //  0009  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: SymbolTable
********************************************************************/
be_local_class(SymbolTable,
    2,
    NULL,
    be_nested_map(26,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(takes_positional_args, -1), be_const_closure(class_SymbolTable_takes_positional_args_closure) },
        { be_const_key_weak(create_value_provider, -1), be_const_closure(class_SymbolTable_create_value_provider_closure) },
        { be_const_key_weak(create_animation, 0), be_const_closure(class_SymbolTable_create_animation_closure) },
        { be_const_key_weak(list_symbols, 15), be_const_closure(class_SymbolTable_list_symbols_closure) },
        { be_const_key_weak(is_dangerous, -1), be_const_closure(class_SymbolTable_is_dangerous_closure) },
        { be_const_key_weak(create_color, -1), be_const_closure(class_SymbolTable_create_color_closure) },
        { be_const_key_weak(mock_engine, -1), be_const_var(1) },
        { be_const_key_weak(entries, 21), be_const_var(0) },
        { be_const_key_weak(add, -1), be_const_closure(class_SymbolTable_add_closure) },
        { be_const_key_weak(_detect_and_cache_symbol, 16), be_const_closure(class_SymbolTable__detect_and_cache_symbol_closure) },
        { be_const_key_weak(_get_named_color_value, -1), be_const_closure(class_SymbolTable__get_named_color_value_closure) },
        { be_const_key_weak(get_reference, -1), be_const_closure(class_SymbolTable_get_reference_closure) },
        { be_const_key_weak(create_template, -1), be_const_closure(class_SymbolTable_create_template_closure) },
        { be_const_key_weak(create_generic, -1), be_const_closure(class_SymbolTable_create_generic_closure) },
        { be_const_key_weak(contains, 8), be_const_closure(class_SymbolTable_contains_closure) },
        { be_const_key_weak(create_palette, -1), be_const_closure(class_SymbolTable_create_palette_closure) },
        { be_const_key_weak(takes_args, 1), be_const_closure(class_SymbolTable_takes_args_closure) },
        { be_const_key_weak(create_sequence, -1), be_const_closure(class_SymbolTable_create_sequence_closure) },
        { be_const_key_weak(symbol_exists, -1), be_const_closure(class_SymbolTable_symbol_exists_closure) },
        { be_const_key_weak(init, 20), be_const_closure(class_SymbolTable_init_closure) },
        { be_const_key_weak(register_user_function, -1), be_const_closure(class_SymbolTable_register_user_function_closure) },
        { be_const_key_weak(get, -1), be_const_closure(class_SymbolTable_get_closure) },
        { be_const_key_weak(create_variable, -1), be_const_closure(class_SymbolTable_create_variable_closure) },
        { be_const_key_weak(takes_named_args, 3), be_const_closure(class_SymbolTable_takes_named_args_closure) },
        { be_const_key_weak(get_type, -1), be_const_closure(class_SymbolTable_get_type_closure) },
        { be_const_key_weak(get_instance, -1), be_const_closure(class_SymbolTable_get_instance_closure) },
    })),
    be_str_weak(SymbolTable)
);
extern const bclass be_class_Token;
// compact class 'Token' ktab size: 52, total: 108 (saved 448 bytes)
static const bvalue be_ktab_class_Token[52] = {
  /* K0   */  be_nested_str_weak(type),
  /* K1   */  be_nested_str_weak(animation_dsl),
  /* K2   */  be_nested_str_weak(Token),
  /* K3   */  be_nested_str_weak(value),
  /* K4   */  be_nested_str_weak(line),
  /* K5   */  be_nested_str_weak(column),
  /* K6   */  be_nested_str_weak(length),
  /* K7   */  be_const_int(1),
  /* K8   */  be_nested_str_weak(),
  /* K9   */  be_nested_str_weak(string),
  /* K10  */  be_nested_str_weak(math),
  /* K11  */  be_const_int(2),
  /* K12  */  be_nested_str_weak(round),
  /* K13  */  be_nested_str_weak(endswith),
  /* K14  */  be_nested_str_weak(ms),
  /* K15  */  be_const_int(0),
  /* K16  */  be_nested_str_weak(s),
  /* K17  */  be_nested_str_weak(m),
  /* K18  */  be_nested_str_weak(h),
  /* K19  */  be_const_int(3600000),
  /* K20  */  be_nested_str_weak(tasmota),
  /* K21  */  be_nested_str_weak(scale_uint),
  /* K22  */  be_nested_str_weak(is_literal),
  /* K23  */  be_const_class(be_class_Token),
  /* K24  */  be_nested_str_weak(names),
  /* K25  */  be_nested_str_weak(UNKNOWN),
  /* K26  */  be_nested_str_weak(end_X20of_X20file),
  /* K27  */  be_nested_str_weak(newline),
  /* K28  */  be_nested_str_weak(keyword_X20_X27_X25s_X27),
  /* K29  */  be_nested_str_weak(identifier_X20_X27_X25s_X27),
  /* K30  */  be_const_int(3),
  /* K31  */  be_nested_str_weak(string_X20_X27_X25s_X27),
  /* K32  */  be_nested_str_weak(number_X20_X27_X25s_X27),
  /* K33  */  be_nested_str_weak(color_X20_X27_X25s_X27),
  /* K34  */  be_nested_str_weak(time_X20_X27_X25s_X27),
  /* K35  */  be_nested_str_weak(percentage_X20_X27_X25s_X27),
  /* K36  */  be_nested_str_weak(invalid_X20token_X20_X27_X25s_X27),
  /* K37  */  be_nested_str_weak(_X27_X25s_X27),
  /* K38  */  be_nested_str_weak(is_boolean),
  /* K39  */  be_nested_str_weak(true),
  /* K40  */  be_nested_str_weak(statement_keywords),
  /* K41  */  be_nested_str_weak(stop_iteration),
  /* K42  */  be_nested_str_weak(introspect),
  /* K43  */  be_nested_str_weak(global),
  /* K44  */  be_nested_str_weak(animation),
  /* K45  */  be_nested_str_weak(members),
  /* K46  */  be_nested_str_weak(find),
  /* K47  */  be_nested_str_weak(false),
  /* K48  */  be_nested_str_weak(to_string),
  /* K49  */  be_nested_str_weak(Token_X28_X25s_X20at_X20_X25s_X3A_X25s_X29),
  /* K50  */  be_nested_str_weak(_X2E_X2E_X2E),
  /* K51  */  be_nested_str_weak(Token_X28_X25s_X2C_X20_X27_X25s_X27_X20at_X20_X25s_X3A_X25s_X29),
};


extern const bclass be_class_Token;

/********************************************************************
** Solidified function: is_type
********************************************************************/
be_local_closure(class_Token_is_type,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(is_type),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x1C080401,  //  0001  EQ	R2	R2	R1
      0x80040400,  //  0002  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: with_type
********************************************************************/
be_local_closure(class_Token_with_type,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(with_type),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0xB80A0200,  //  0000  GETNGBL	R2	K1
      0x8C080502,  //  0001  GETMET	R2	R2	K2
      0x5C100200,  //  0002  MOVE	R4	R1
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
** Solidified function: end_column
********************************************************************/
be_local_closure(class_Token_end_column,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(end_column),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88040105,  //  0000  GETMBR	R1	R0	K5
      0x88080106,  //  0001  GETMBR	R2	R0	K6
      0x00040202,  //  0002  ADD	R1	R1	R2
      0x04040307,  //  0003  SUB	R1	R1	K7
      0x80040200,  //  0004  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_operator
********************************************************************/
be_local_closure(class_Token_is_operator,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(is_operator),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x540A0007,  //  0001  LDINT	R2	8
      0x28040202,  //  0002  GE	R1	R1	R2
      0x78060003,  //  0003  JMPF	R1	#0008
      0x88040100,  //  0004  GETMBR	R1	R0	K0
      0x540A0016,  //  0005  LDINT	R2	23
      0x18040202,  //  0006  LE	R1	R1	R2
      0x74060000,  //  0007  JMPT	R1	#0009
      0x50040001,  //  0008  LDBOOL	R1	0	1
      0x50040200,  //  0009  LDBOOL	R1	1	0
      0x80040200,  //  000A  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Token_init,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    6,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x4C180000,  //  0001  LDNIL	R6
      0x20180406,  //  0002  NE	R6	R2	R6
      0x781A0001,  //  0003  JMPF	R6	#0006
      0x5C180400,  //  0004  MOVE	R6	R2
      0x70020000,  //  0005  JMP		#0007
      0x58180008,  //  0006  LDCONST	R6	K8
      0x90020606,  //  0007  SETMBR	R0	K3	R6
      0x4C180000,  //  0008  LDNIL	R6
      0x20180606,  //  0009  NE	R6	R3	R6
      0x781A0001,  //  000A  JMPF	R6	#000D
      0x5C180600,  //  000B  MOVE	R6	R3
      0x70020000,  //  000C  JMP		#000E
      0x58180007,  //  000D  LDCONST	R6	K7
      0x90020806,  //  000E  SETMBR	R0	K4	R6
      0x4C180000,  //  000F  LDNIL	R6
      0x20180806,  //  0010  NE	R6	R4	R6
      0x781A0001,  //  0011  JMPF	R6	#0014
      0x5C180800,  //  0012  MOVE	R6	R4
      0x70020000,  //  0013  JMP		#0015
      0x58180007,  //  0014  LDCONST	R6	K7
      0x90020A06,  //  0015  SETMBR	R0	K5	R6
      0x4C180000,  //  0016  LDNIL	R6
      0x20180A06,  //  0017  NE	R6	R5	R6
      0x781A0001,  //  0018  JMPF	R6	#001B
      0x5C180A00,  //  0019  MOVE	R6	R5
      0x70020002,  //  001A  JMP		#001E
      0x6018000C,  //  001B  GETGBL	R6	G12
      0x881C0103,  //  001C  GETMBR	R7	R0	K3
      0x7C180200,  //  001D  CALL	R6	1
      0x90020C06,  //  001E  SETMBR	R0	K6	R6
      0x80000000,  //  001F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_delimiter
********************************************************************/
be_local_closure(class_Token_is_delimiter,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(is_delimiter),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x540A0017,  //  0001  LDINT	R2	24
      0x28040202,  //  0002  GE	R1	R1	R2
      0x78060003,  //  0003  JMPF	R1	#0008
      0x88040100,  //  0004  GETMBR	R1	R0	K0
      0x540A001C,  //  0005  LDINT	R2	29
      0x18040202,  //  0006  LE	R1	R1	R2
      0x74060000,  //  0007  JMPT	R1	#0009
      0x50040001,  //  0008  LDBOOL	R1	0	1
      0x50040200,  //  0009  LDBOOL	R1	1	0
      0x80040200,  //  000A  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_numeric_value
********************************************************************/
be_local_closure(class_Token_get_numeric_value,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(get_numeric_value),
    &be_const_str_solidified,
    ( &(const binstruction[117]) {  /* code */
      0xA4061200,  //  0000  IMPORT	R1	K9
      0xA40A1400,  //  0001  IMPORT	R2	K10
      0x880C0100,  //  0002  GETMBR	R3	R0	K0
      0x1C0C070B,  //  0003  EQ	R3	R3	K11
      0x780E0006,  //  0004  JMPF	R3	#000C
      0x8C0C050C,  //  0005  GETMET	R3	R2	K12
      0x6014000A,  //  0006  GETGBL	R5	G10
      0x88180103,  //  0007  GETMBR	R6	R0	K3
      0x7C140200,  //  0008  CALL	R5	1
      0x7C0C0400,  //  0009  CALL	R3	2
      0x80040600,  //  000A  RET	1	R3
      0x70020066,  //  000B  JMP		#0073
      0x880C0100,  //  000C  GETMBR	R3	R0	K0
      0x54120004,  //  000D  LDINT	R4	5
      0x1C0C0604,  //  000E  EQ	R3	R3	R4
      0x780E003D,  //  000F  JMPF	R3	#004E
      0x880C0103,  //  0010  GETMBR	R3	R0	K3
      0x8C10030D,  //  0011  GETMET	R4	R1	K13
      0x5C180600,  //  0012  MOVE	R6	R3
      0x581C000E,  //  0013  LDCONST	R7	K14
      0x7C100600,  //  0014  CALL	R4	3
      0x78120008,  //  0015  JMPF	R4	#001F
      0x8C10050C,  //  0016  GETMET	R4	R2	K12
      0x6018000A,  //  0017  GETGBL	R6	G10
      0x541DFFFC,  //  0018  LDINT	R7	-3
      0x401E1E07,  //  0019  CONNECT	R7	K15	R7
      0x941C0607,  //  001A  GETIDX	R7	R3	R7
      0x7C180200,  //  001B  CALL	R6	1
      0x7C100400,  //  001C  CALL	R4	2
      0x80040800,  //  001D  RET	1	R4
      0x7002002D,  //  001E  JMP		#004D
      0x8C10030D,  //  001F  GETMET	R4	R1	K13
      0x5C180600,  //  0020  MOVE	R6	R3
      0x581C0010,  //  0021  LDCONST	R7	K16
      0x7C100600,  //  0022  CALL	R4	3
      0x7812000A,  //  0023  JMPF	R4	#002F
      0x8C10050C,  //  0024  GETMET	R4	R2	K12
      0x6018000A,  //  0025  GETGBL	R6	G10
      0x541DFFFD,  //  0026  LDINT	R7	-2
      0x401E1E07,  //  0027  CONNECT	R7	K15	R7
      0x941C0607,  //  0028  GETIDX	R7	R3	R7
      0x7C180200,  //  0029  CALL	R6	1
      0x541E03E7,  //  002A  LDINT	R7	1000
      0x08180C07,  //  002B  MUL	R6	R6	R7
      0x7C100400,  //  002C  CALL	R4	2
      0x80040800,  //  002D  RET	1	R4
      0x7002001D,  //  002E  JMP		#004D
      0x8C10030D,  //  002F  GETMET	R4	R1	K13
      0x5C180600,  //  0030  MOVE	R6	R3
      0x581C0011,  //  0031  LDCONST	R7	K17
      0x7C100600,  //  0032  CALL	R4	3
      0x7812000A,  //  0033  JMPF	R4	#003F
      0x8C10050C,  //  0034  GETMET	R4	R2	K12
      0x6018000A,  //  0035  GETGBL	R6	G10
      0x541DFFFD,  //  0036  LDINT	R7	-2
      0x401E1E07,  //  0037  CONNECT	R7	K15	R7
      0x941C0607,  //  0038  GETIDX	R7	R3	R7
      0x7C180200,  //  0039  CALL	R6	1
      0x541EEA5F,  //  003A  LDINT	R7	60000
      0x08180C07,  //  003B  MUL	R6	R6	R7
      0x7C100400,  //  003C  CALL	R4	2
      0x80040800,  //  003D  RET	1	R4
      0x7002000D,  //  003E  JMP		#004D
      0x8C10030D,  //  003F  GETMET	R4	R1	K13
      0x5C180600,  //  0040  MOVE	R6	R3
      0x581C0012,  //  0041  LDCONST	R7	K18
      0x7C100600,  //  0042  CALL	R4	3
      0x78120008,  //  0043  JMPF	R4	#004D
      0x8C10050C,  //  0044  GETMET	R4	R2	K12
      0x6018000A,  //  0045  GETGBL	R6	G10
      0x541DFFFD,  //  0046  LDINT	R7	-2
      0x401E1E07,  //  0047  CONNECT	R7	K15	R7
      0x941C0607,  //  0048  GETIDX	R7	R3	R7
      0x7C180200,  //  0049  CALL	R6	1
      0x08180D13,  //  004A  MUL	R6	R6	K19
      0x7C100400,  //  004B  CALL	R4	2
      0x80040800,  //  004C  RET	1	R4
      0x70020024,  //  004D  JMP		#0073
      0x880C0100,  //  004E  GETMBR	R3	R0	K0
      0x54120005,  //  004F  LDINT	R4	6
      0x1C0C0604,  //  0050  EQ	R3	R3	R4
      0x780E0011,  //  0051  JMPF	R3	#0064
      0x8C0C050C,  //  0052  GETMET	R3	R2	K12
      0x6014000A,  //  0053  GETGBL	R5	G10
      0x5419FFFD,  //  0054  LDINT	R6	-2
      0x401A1E06,  //  0055  CONNECT	R6	K15	R6
      0x881C0103,  //  0056  GETMBR	R7	R0	K3
      0x94180E06,  //  0057  GETIDX	R6	R7	R6
      0x7C140200,  //  0058  CALL	R5	1
      0x7C0C0400,  //  0059  CALL	R3	2
      0xB8122800,  //  005A  GETNGBL	R4	K20
      0x8C100915,  //  005B  GETMET	R4	R4	K21
      0x5C180600,  //  005C  MOVE	R6	R3
      0x581C000F,  //  005D  LDCONST	R7	K15
      0x54220063,  //  005E  LDINT	R8	100
      0x5824000F,  //  005F  LDCONST	R9	K15
      0x542A00FE,  //  0060  LDINT	R10	255
      0x7C100C00,  //  0061  CALL	R4	6
      0x80040800,  //  0062  RET	1	R4
      0x7002000E,  //  0063  JMP		#0073
      0x880C0100,  //  0064  GETMBR	R3	R0	K0
      0x54120006,  //  0065  LDINT	R4	7
      0x1C0C0604,  //  0066  EQ	R3	R3	R4
      0x780E000A,  //  0067  JMPF	R3	#0073
      0x600C000A,  //  0068  GETGBL	R3	G10
      0x5411FFFD,  //  0069  LDINT	R4	-2
      0x40121E04,  //  006A  CONNECT	R4	K15	R4
      0x88140103,  //  006B  GETMBR	R5	R0	K3
      0x94100A04,  //  006C  GETIDX	R4	R5	R4
      0x7C0C0200,  //  006D  CALL	R3	1
      0x8C10050C,  //  006E  GETMET	R4	R2	K12
      0x541A00FF,  //  006F  LDINT	R6	256
      0x08180606,  //  0070  MUL	R6	R3	R6
      0x7C100400,  //  0071  CALL	R4	2
      0x80040800,  //  0072  RET	1	R4
      0x4C0C0000,  //  0073  LDNIL	R3
      0x80040600,  //  0074  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: can_end_expression
********************************************************************/
be_local_closure(class_Token_can_end_expression,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(can_end_expression),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x8C040116,  //  0000  GETMET	R1	R0	K22
      0x7C040200,  //  0001  CALL	R1	1
      0x7406000B,  //  0002  JMPT	R1	#000F
      0x88040100,  //  0003  GETMBR	R1	R0	K0
      0x1C040307,  //  0004  EQ	R1	R1	K7
      0x74060008,  //  0005  JMPT	R1	#000F
      0x88040100,  //  0006  GETMBR	R1	R0	K0
      0x540A0023,  //  0007  LDINT	R2	36
      0x1C040202,  //  0008  EQ	R1	R1	R2
      0x74060004,  //  0009  JMPT	R1	#000F
      0x88040100,  //  000A  GETMBR	R1	R0	K0
      0x540A0018,  //  000B  LDINT	R2	25
      0x1C040202,  //  000C  EQ	R1	R1	R2
      0x74060000,  //  000D  JMPT	R1	#000F
      0x50040001,  //  000E  LDBOOL	R1	0	1
      0x50040200,  //  000F  LDBOOL	R1	1	0
      0x80040200,  //  0010  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: to_string
********************************************************************/
be_local_closure(class_Token_to_string,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(to_string),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x58040017,  //  0000  LDCONST	R1	K23
      0x2808010F,  //  0001  GE	R2	R0	K15
      0x780A0007,  //  0002  JMPF	R2	#000B
      0x6008000C,  //  0003  GETGBL	R2	G12
      0x880C0318,  //  0004  GETMBR	R3	R1	K24
      0x7C080200,  //  0005  CALL	R2	1
      0x14080002,  //  0006  LT	R2	R0	R2
      0x780A0002,  //  0007  JMPF	R2	#000B
      0x88080318,  //  0008  GETMBR	R2	R1	K24
      0x94080400,  //  0009  GETIDX	R2	R2	R0
      0x80040400,  //  000A  RET	1	R2
      0x80063200,  //  000B  RET	1	K25
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: to_error_string
********************************************************************/
be_local_closure(class_Token_to_error_string,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(to_error_string),
    &be_const_str_solidified,
    ( &(const binstruction[94]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x540A0025,  //  0001  LDINT	R2	38
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060001,  //  0003  JMPF	R1	#0006
      0x80063400,  //  0004  RET	1	K26
      0x70020056,  //  0005  JMP		#005D
      0x88040100,  //  0006  GETMBR	R1	R0	K0
      0x540A0022,  //  0007  LDINT	R2	35
      0x1C040202,  //  0008  EQ	R1	R1	R2
      0x78060001,  //  0009  JMPF	R1	#000C
      0x80063600,  //  000A  RET	1	K27
      0x70020050,  //  000B  JMP		#005D
      0x88040100,  //  000C  GETMBR	R1	R0	K0
      0x1C04030F,  //  000D  EQ	R1	R1	K15
      0x78060005,  //  000E  JMPF	R1	#0015
      0x60040018,  //  000F  GETGBL	R1	G24
      0x5808001C,  //  0010  LDCONST	R2	K28
      0x880C0103,  //  0011  GETMBR	R3	R0	K3
      0x7C040400,  //  0012  CALL	R1	2
      0x80040200,  //  0013  RET	1	R1
      0x70020047,  //  0014  JMP		#005D
      0x88040100,  //  0015  GETMBR	R1	R0	K0
      0x1C040307,  //  0016  EQ	R1	R1	K7
      0x78060005,  //  0017  JMPF	R1	#001E
      0x60040018,  //  0018  GETGBL	R1	G24
      0x5808001D,  //  0019  LDCONST	R2	K29
      0x880C0103,  //  001A  GETMBR	R3	R0	K3
      0x7C040400,  //  001B  CALL	R1	2
      0x80040200,  //  001C  RET	1	R1
      0x7002003E,  //  001D  JMP		#005D
      0x88040100,  //  001E  GETMBR	R1	R0	K0
      0x1C04031E,  //  001F  EQ	R1	R1	K30
      0x78060005,  //  0020  JMPF	R1	#0027
      0x60040018,  //  0021  GETGBL	R1	G24
      0x5808001F,  //  0022  LDCONST	R2	K31
      0x880C0103,  //  0023  GETMBR	R3	R0	K3
      0x7C040400,  //  0024  CALL	R1	2
      0x80040200,  //  0025  RET	1	R1
      0x70020035,  //  0026  JMP		#005D
      0x88040100,  //  0027  GETMBR	R1	R0	K0
      0x1C04030B,  //  0028  EQ	R1	R1	K11
      0x78060005,  //  0029  JMPF	R1	#0030
      0x60040018,  //  002A  GETGBL	R1	G24
      0x58080020,  //  002B  LDCONST	R2	K32
      0x880C0103,  //  002C  GETMBR	R3	R0	K3
      0x7C040400,  //  002D  CALL	R1	2
      0x80040200,  //  002E  RET	1	R1
      0x7002002C,  //  002F  JMP		#005D
      0x88040100,  //  0030  GETMBR	R1	R0	K0
      0x540A0003,  //  0031  LDINT	R2	4
      0x1C040202,  //  0032  EQ	R1	R1	R2
      0x78060005,  //  0033  JMPF	R1	#003A
      0x60040018,  //  0034  GETGBL	R1	G24
      0x58080021,  //  0035  LDCONST	R2	K33
      0x880C0103,  //  0036  GETMBR	R3	R0	K3
      0x7C040400,  //  0037  CALL	R1	2
      0x80040200,  //  0038  RET	1	R1
      0x70020022,  //  0039  JMP		#005D
      0x88040100,  //  003A  GETMBR	R1	R0	K0
      0x540A0004,  //  003B  LDINT	R2	5
      0x1C040202,  //  003C  EQ	R1	R1	R2
      0x78060005,  //  003D  JMPF	R1	#0044
      0x60040018,  //  003E  GETGBL	R1	G24
      0x58080022,  //  003F  LDCONST	R2	K34
      0x880C0103,  //  0040  GETMBR	R3	R0	K3
      0x7C040400,  //  0041  CALL	R1	2
      0x80040200,  //  0042  RET	1	R1
      0x70020018,  //  0043  JMP		#005D
      0x88040100,  //  0044  GETMBR	R1	R0	K0
      0x540A0005,  //  0045  LDINT	R2	6
      0x1C040202,  //  0046  EQ	R1	R1	R2
      0x78060005,  //  0047  JMPF	R1	#004E
      0x60040018,  //  0048  GETGBL	R1	G24
      0x58080023,  //  0049  LDCONST	R2	K35
      0x880C0103,  //  004A  GETMBR	R3	R0	K3
      0x7C040400,  //  004B  CALL	R1	2
      0x80040200,  //  004C  RET	1	R1
      0x7002000E,  //  004D  JMP		#005D
      0x88040100,  //  004E  GETMBR	R1	R0	K0
      0x540A0026,  //  004F  LDINT	R2	39
      0x1C040202,  //  0050  EQ	R1	R1	R2
      0x78060005,  //  0051  JMPF	R1	#0058
      0x60040018,  //  0052  GETGBL	R1	G24
      0x58080024,  //  0053  LDCONST	R2	K36
      0x880C0103,  //  0054  GETMBR	R3	R0	K3
      0x7C040400,  //  0055  CALL	R1	2
      0x80040200,  //  0056  RET	1	R1
      0x70020004,  //  0057  JMP		#005D
      0x60040018,  //  0058  GETGBL	R1	G24
      0x58080025,  //  0059  LDCONST	R2	K37
      0x880C0103,  //  005A  GETMBR	R3	R0	K3
      0x7C040400,  //  005B  CALL	R1	2
      0x80040200,  //  005C  RET	1	R1
      0x80000000,  //  005D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_separator
********************************************************************/
be_local_closure(class_Token_is_separator,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(is_separator),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x540A001D,  //  0001  LDINT	R2	30
      0x28040202,  //  0002  GE	R1	R1	R2
      0x78060003,  //  0003  JMPF	R1	#0008
      0x88040100,  //  0004  GETMBR	R1	R0	K0
      0x540A0021,  //  0005  LDINT	R2	34
      0x18040202,  //  0006  LE	R1	R1	R2
      0x74060000,  //  0007  JMPT	R1	#0009
      0x50040001,  //  0008  LDBOOL	R1	0	1
      0x50040200,  //  0009  LDBOOL	R1	1	0
      0x80040200,  //  000A  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_keyword
********************************************************************/
be_local_closure(class_Token_is_keyword,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(is_keyword),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x1C08050F,  //  0001  EQ	R2	R2	K15
      0x780A0002,  //  0002  JMPF	R2	#0006
      0x88080103,  //  0003  GETMBR	R2	R0	K3
      0x1C080401,  //  0004  EQ	R2	R2	R1
      0x740A0000,  //  0005  JMPT	R2	#0007
      0x50080001,  //  0006  LDBOOL	R2	0	1
      0x50080200,  //  0007  LDBOOL	R2	1	0
      0x80040400,  //  0008  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_literal
********************************************************************/
be_local_closure(class_Token_is_literal,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(is_literal),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x1C04030B,  //  0001  EQ	R1	R1	K11
      0x74060013,  //  0002  JMPT	R1	#0017
      0x88040100,  //  0003  GETMBR	R1	R0	K0
      0x1C04031E,  //  0004  EQ	R1	R1	K30
      0x74060010,  //  0005  JMPT	R1	#0017
      0x88040100,  //  0006  GETMBR	R1	R0	K0
      0x540A0003,  //  0007  LDINT	R2	4
      0x1C040202,  //  0008  EQ	R1	R1	R2
      0x7406000C,  //  0009  JMPT	R1	#0017
      0x88040100,  //  000A  GETMBR	R1	R0	K0
      0x540A0004,  //  000B  LDINT	R2	5
      0x1C040202,  //  000C  EQ	R1	R1	R2
      0x74060008,  //  000D  JMPT	R1	#0017
      0x88040100,  //  000E  GETMBR	R1	R0	K0
      0x540A0005,  //  000F  LDINT	R2	6
      0x1C040202,  //  0010  EQ	R1	R1	R2
      0x74060004,  //  0011  JMPT	R1	#0017
      0x88040100,  //  0012  GETMBR	R1	R0	K0
      0x540A0006,  //  0013  LDINT	R2	7
      0x1C040202,  //  0014  EQ	R1	R1	R2
      0x74060000,  //  0015  JMPT	R1	#0017
      0x50040001,  //  0016  LDBOOL	R1	0	1
      0x50040200,  //  0017  LDBOOL	R1	1	0
      0x80040200,  //  0018  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: can_start_expression
********************************************************************/
be_local_closure(class_Token_can_start_expression,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(can_start_expression),
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0x8C040116,  //  0000  GETMET	R1	R0	K22
      0x7C040200,  //  0001  CALL	R1	1
      0x74060017,  //  0002  JMPT	R1	#001B
      0x88040100,  //  0003  GETMBR	R1	R0	K0
      0x1C040307,  //  0004  EQ	R1	R1	K7
      0x74060014,  //  0005  JMPT	R1	#001B
      0x88040100,  //  0006  GETMBR	R1	R0	K0
      0x540A0023,  //  0007  LDINT	R2	36
      0x1C040202,  //  0008  EQ	R1	R1	R2
      0x74060010,  //  0009  JMPT	R1	#001B
      0x88040100,  //  000A  GETMBR	R1	R0	K0
      0x540A0017,  //  000B  LDINT	R2	24
      0x1C040202,  //  000C  EQ	R1	R1	R2
      0x7406000C,  //  000D  JMPT	R1	#001B
      0x88040100,  //  000E  GETMBR	R1	R0	K0
      0x540A0016,  //  000F  LDINT	R2	23
      0x1C040202,  //  0010  EQ	R1	R1	R2
      0x74060008,  //  0011  JMPT	R1	#001B
      0x88040100,  //  0012  GETMBR	R1	R0	K0
      0x540A0009,  //  0013  LDINT	R2	10
      0x1C040202,  //  0014  EQ	R1	R1	R2
      0x74060004,  //  0015  JMPT	R1	#001B
      0x88040100,  //  0016  GETMBR	R1	R0	K0
      0x540A0008,  //  0017  LDINT	R2	9
      0x1C040202,  //  0018  EQ	R1	R1	R2
      0x74060000,  //  0019  JMPT	R1	#001B
      0x50040001,  //  001A  LDBOOL	R1	0	1
      0x50040200,  //  001B  LDBOOL	R1	1	0
      0x80040200,  //  001C  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_boolean_value
********************************************************************/
be_local_closure(class_Token_get_boolean_value,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(get_boolean_value),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x8C040126,  //  0000  GETMET	R1	R0	K38
      0x7C040200,  //  0001  CALL	R1	1
      0x78060002,  //  0002  JMPF	R1	#0006
      0x88040103,  //  0003  GETMBR	R1	R0	K3
      0x1C040327,  //  0004  EQ	R1	R1	K39
      0x80040200,  //  0005  RET	1	R1
      0x4C040000,  //  0006  LDNIL	R1
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_numeric
********************************************************************/
be_local_closure(class_Token_is_numeric,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(is_numeric),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x1C04030B,  //  0001  EQ	R1	R1	K11
      0x7406000C,  //  0002  JMPT	R1	#0010
      0x88040100,  //  0003  GETMBR	R1	R0	K0
      0x540A0004,  //  0004  LDINT	R2	5
      0x1C040202,  //  0005  EQ	R1	R1	R2
      0x74060008,  //  0006  JMPT	R1	#0010
      0x88040100,  //  0007  GETMBR	R1	R0	K0
      0x540A0005,  //  0008  LDINT	R2	6
      0x1C040202,  //  0009  EQ	R1	R1	R2
      0x74060004,  //  000A  JMPT	R1	#0010
      0x88040100,  //  000B  GETMBR	R1	R0	K0
      0x540A0006,  //  000C  LDINT	R2	7
      0x1C040202,  //  000D  EQ	R1	R1	R2
      0x74060000,  //  000E  JMPT	R1	#0010
      0x50040001,  //  000F  LDBOOL	R1	0	1
      0x50040200,  //  0010  LDBOOL	R1	1	0
      0x80040200,  //  0011  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_statement_start
********************************************************************/
be_local_closure(class_Token_is_statement_start,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(is_statement_start),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x2004030F,  //  0001  NE	R1	R1	K15
      0x78060001,  //  0002  JMPF	R1	#0005
      0x50040000,  //  0003  LDBOOL	R1	0	0
      0x80040200,  //  0004  RET	1	R1
      0x60040010,  //  0005  GETGBL	R1	G16
      0x88080128,  //  0006  GETMBR	R2	R0	K40
      0x7C040200,  //  0007  CALL	R1	1
      0xA8020008,  //  0008  EXBLK	0	#0012
      0x5C080200,  //  0009  MOVE	R2	R1
      0x7C080000,  //  000A  CALL	R2	0
      0x880C0103,  //  000B  GETMBR	R3	R0	K3
      0x1C0C0602,  //  000C  EQ	R3	R3	R2
      0x780E0002,  //  000D  JMPF	R3	#0011
      0x500C0200,  //  000E  LDBOOL	R3	1	0
      0xA8040001,  //  000F  EXBLK	1	1
      0x80040600,  //  0010  RET	1	R3
      0x7001FFF6,  //  0011  JMP		#0009
      0x58040029,  //  0012  LDCONST	R1	K41
      0xAC040200,  //  0013  CATCH	R1	1	0
      0xB0080000,  //  0014  RAISE	2	R0	R0
      0x50040000,  //  0015  LDBOOL	R1	0	0
      0x80040200,  //  0016  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_dsl_function
********************************************************************/
be_local_closure(class_Token_is_dsl_function,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(is_dsl_function),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x2004030F,  //  0001  NE	R1	R1	K15
      0x78060001,  //  0002  JMPF	R1	#0005
      0x50040000,  //  0003  LDBOOL	R1	0	0
      0x80040200,  //  0004  RET	1	R1
      0xA8020011,  //  0005  EXBLK	0	#0018
      0xA4065400,  //  0006  IMPORT	R1	K42
      0xB80A5600,  //  0007  GETNGBL	R2	K43
      0x8808052C,  //  0008  GETMBR	R2	R2	K44
      0x4C0C0000,  //  0009  LDNIL	R3
      0x200C0403,  //  000A  NE	R3	R2	R3
      0x780E0009,  //  000B  JMPF	R3	#0016
      0x8C0C032D,  //  000C  GETMET	R3	R1	K45
      0x5C140400,  //  000D  MOVE	R5	R2
      0x7C0C0400,  //  000E  CALL	R3	2
      0x8C10072E,  //  000F  GETMET	R4	R3	K46
      0x88180103,  //  0010  GETMBR	R6	R0	K3
      0x7C100400,  //  0011  CALL	R4	2
      0x4C140000,  //  0012  LDNIL	R5
      0x20100805,  //  0013  NE	R4	R4	R5
      0xA8040001,  //  0014  EXBLK	1	1
      0x80040800,  //  0015  RET	1	R4
      0xA8040001,  //  0016  EXBLK	1	1
      0x70020005,  //  0017  JMP		#001E
      0xAC040002,  //  0018  CATCH	R1	0	2
      0x70020002,  //  0019  JMP		#001D
      0x500C0000,  //  001A  LDBOOL	R3	0	0
      0x80040600,  //  001B  RET	1	R3
      0x70020000,  //  001C  JMP		#001E
      0xB0080000,  //  001D  RAISE	2	R0	R0
      0x50040000,  //  001E  LDBOOL	R1	0	0
      0x80040200,  //  001F  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_identifier
********************************************************************/
be_local_closure(class_Token_is_identifier,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(is_identifier),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x1C080507,  //  0001  EQ	R2	R2	K7
      0x780A0002,  //  0002  JMPF	R2	#0006
      0x88080103,  //  0003  GETMBR	R2	R0	K3
      0x1C080401,  //  0004  EQ	R2	R2	R1
      0x740A0000,  //  0005  JMPT	R2	#0007
      0x50080001,  //  0006  LDBOOL	R2	0	1
      0x50080200,  //  0007  LDBOOL	R2	1	0
      0x80040400,  //  0008  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_boolean
********************************************************************/
be_local_closure(class_Token_is_boolean,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(is_boolean),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x1C04030F,  //  0001  EQ	R1	R1	K15
      0x78060005,  //  0002  JMPF	R1	#0009
      0x88040103,  //  0003  GETMBR	R1	R0	K3
      0x1C040327,  //  0004  EQ	R1	R1	K39
      0x74060003,  //  0005  JMPT	R1	#000A
      0x88040103,  //  0006  GETMBR	R1	R0	K3
      0x1C04032F,  //  0007  EQ	R1	R1	K47
      0x74060000,  //  0008  JMPT	R1	#000A
      0x50040001,  //  0009  LDBOOL	R1	0	1
      0x50040200,  //  000A  LDBOOL	R1	1	0
      0x80040200,  //  000B  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_Token_tostring,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[56]) {  /* code */
      0x8C040130,  //  0000  GETMET	R1	R0	K48
      0x880C0100,  //  0001  GETMBR	R3	R0	K0
      0x7C040400,  //  0002  CALL	R1	2
      0x88080100,  //  0003  GETMBR	R2	R0	K0
      0x540E0025,  //  0004  LDINT	R3	38
      0x1C080403,  //  0005  EQ	R2	R2	R3
      0x780A0007,  //  0006  JMPF	R2	#000F
      0x60080018,  //  0007  GETGBL	R2	G24
      0x580C0031,  //  0008  LDCONST	R3	K49
      0x5C100200,  //  0009  MOVE	R4	R1
      0x88140104,  //  000A  GETMBR	R5	R0	K4
      0x88180105,  //  000B  GETMBR	R6	R0	K5
      0x7C080800,  //  000C  CALL	R2	4
      0x80040400,  //  000D  RET	1	R2
      0x70020027,  //  000E  JMP		#0037
      0x88080100,  //  000F  GETMBR	R2	R0	K0
      0x540E0022,  //  0010  LDINT	R3	35
      0x1C080403,  //  0011  EQ	R2	R2	R3
      0x780A0007,  //  0012  JMPF	R2	#001B
      0x60080018,  //  0013  GETGBL	R2	G24
      0x580C0031,  //  0014  LDCONST	R3	K49
      0x5C100200,  //  0015  MOVE	R4	R1
      0x88140104,  //  0016  GETMBR	R5	R0	K4
      0x88180105,  //  0017  GETMBR	R6	R0	K5
      0x7C080800,  //  0018  CALL	R2	4
      0x80040400,  //  0019  RET	1	R2
      0x7002001B,  //  001A  JMP		#0037
      0x6008000C,  //  001B  GETGBL	R2	G12
      0x880C0103,  //  001C  GETMBR	R3	R0	K3
      0x7C080200,  //  001D  CALL	R2	1
      0x540E0013,  //  001E  LDINT	R3	20
      0x24080403,  //  001F  GT	R2	R2	R3
      0x780A000D,  //  0020  JMPF	R2	#002F
      0x540A0010,  //  0021  LDINT	R2	17
      0x400A1E02,  //  0022  CONNECT	R2	K15	R2
      0x880C0103,  //  0023  GETMBR	R3	R0	K3
      0x94080602,  //  0024  GETIDX	R2	R3	R2
      0x00080532,  //  0025  ADD	R2	R2	K50
      0x600C0018,  //  0026  GETGBL	R3	G24
      0x58100033,  //  0027  LDCONST	R4	K51
      0x5C140200,  //  0028  MOVE	R5	R1
      0x5C180400,  //  0029  MOVE	R6	R2
      0x881C0104,  //  002A  GETMBR	R7	R0	K4
      0x88200105,  //  002B  GETMBR	R8	R0	K5
      0x7C0C0A00,  //  002C  CALL	R3	5
      0x80040600,  //  002D  RET	1	R3
      0x70020007,  //  002E  JMP		#0037
      0x60080018,  //  002F  GETGBL	R2	G24
      0x580C0033,  //  0030  LDCONST	R3	K51
      0x5C100200,  //  0031  MOVE	R4	R1
      0x88140103,  //  0032  GETMBR	R5	R0	K3
      0x88180104,  //  0033  GETMBR	R6	R0	K4
      0x881C0105,  //  0034  GETMBR	R7	R0	K5
      0x7C080A00,  //  0035  CALL	R2	5
      0x80040400,  //  0036  RET	1	R2
      0x80000000,  //  0037  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: with_value
********************************************************************/
be_local_closure(class_Token_with_value,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(with_value),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0xB80A0200,  //  0000  GETNGBL	R2	K1
      0x8C080502,  //  0001  GETMET	R2	R2	K2
      0x88100100,  //  0002  GETMBR	R4	R0	K0
      0x5C140200,  //  0003  MOVE	R5	R1
      0x88180104,  //  0004  GETMBR	R6	R0	K4
      0x881C0105,  //  0005  GETMBR	R7	R0	K5
      0x6020000C,  //  0006  GETGBL	R8	G12
      0x5C240200,  //  0007  MOVE	R9	R1
      0x7C200200,  //  0008  CALL	R8	1
      0x7C080C00,  //  0009  CALL	R2	6
      0x80040400,  //  000A  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Token
********************************************************************/
be_local_class(Token,
    5,
    NULL,
    be_nested_map(75,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(with_value, 17), be_const_closure(class_Token_with_value_closure) },
        { be_const_key_weak(TIME, -1), be_const_int(5) },
        { be_const_key_weak(ASSIGN, 25), be_const_int(8) },
        { be_const_key_weak(POWER, -1), be_const_int(14) },
        { be_const_key_weak(MULTIPLIER, -1), be_const_int(7) },
        { be_const_key_weak(MULTIPLY, 0), be_const_int(11) },
        { be_const_key_weak(with_type, 74), be_const_closure(class_Token_with_type_closure) },
        { be_const_key_weak(end_column, -1), be_const_closure(class_Token_end_column_closure) },
        { be_const_key_weak(value, 29), be_const_var(1) },
        { be_const_key_weak(LEFT_PAREN, -1), be_const_int(24) },
        { be_const_key_weak(is_operator, -1), be_const_closure(class_Token_is_operator_closure) },
        { be_const_key_weak(LOGICAL_NOT, -1), be_const_int(23) },
        { be_const_key_weak(NEWLINE, -1), be_const_int(35) },
        { be_const_key_weak(LEFT_BRACKET, -1), be_const_int(28) },
        { be_const_key_weak(tostring, 28), be_const_closure(class_Token_tostring_closure) },
        { be_const_key_weak(GREATER_EQUAL, -1), be_const_int(20) },
        { be_const_key_weak(is_delimiter, -1), be_const_closure(class_Token_is_delimiter_closure) },
        { be_const_key_weak(COMMENT, -1), be_const_int(37) },
        { be_const_key_weak(LOGICAL_OR, -1), be_const_int(22) },
        { be_const_key_weak(is_type, 73), be_const_closure(class_Token_is_type_closure) },
        { be_const_key_weak(PERCENTAGE, -1), be_const_int(6) },
        { be_const_key_weak(SEMICOLON, -1), be_const_int(31) },
        { be_const_key_weak(column, 72), be_const_var(3) },
        { be_const_key_weak(color_names, 11), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(37,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(red),
        be_nested_str_weak(green),
        be_nested_str_weak(blue),
        be_nested_str_weak(white),
        be_nested_str_weak(black),
        be_nested_str_weak(yellow),
        be_nested_str_weak(orange),
        be_nested_str_weak(purple),
        be_nested_str_weak(pink),
        be_nested_str_weak(cyan),
        be_nested_str_weak(magenta),
        be_nested_str_weak(gray),
        be_nested_str_weak(grey),
        be_nested_str_weak(silver),
        be_nested_str_weak(gold),
        be_nested_str_weak(brown),
        be_nested_str_weak(lime),
        be_nested_str_weak(navy),
        be_nested_str_weak(olive),
        be_nested_str_weak(maroon),
        be_nested_str_weak(teal),
        be_nested_str_weak(aqua),
        be_nested_str_weak(fuchsia),
        be_nested_str_weak(indigo),
        be_nested_str_weak(violet),
        be_nested_str_weak(crimson),
        be_nested_str_weak(coral),
        be_nested_str_weak(salmon),
        be_nested_str_weak(khaki),
        be_nested_str_weak(plum),
        be_nested_str_weak(orchid),
        be_nested_str_weak(turquoise),
        be_nested_str_weak(tan),
        be_nested_str_weak(beige),
        be_nested_str_weak(ivory),
        be_nested_str_weak(snow),
        be_nested_str_weak(transparent),
    }))    ) } )) },
        { be_const_key_weak(is_identifier, 44), be_const_closure(class_Token_is_identifier_closure) },
        { be_const_key_weak(is_dsl_function, -1), be_const_closure(class_Token_is_dsl_function_closure) },
        { be_const_key_weak(EOF, 61), be_const_int(38) },
        { be_const_key_weak(GREATER_THAN, -1), be_const_int(19) },
        { be_const_key_weak(is_numeric, 66), be_const_closure(class_Token_is_numeric_closure) },
        { be_const_key_weak(MODULO, -1), be_const_int(13) },
        { be_const_key_weak(init, 67), be_const_closure(class_Token_init_closure) },
        { be_const_key_weak(get_numeric_value, -1), be_const_closure(class_Token_get_numeric_value_closure) },
        { be_const_key_weak(IDENTIFIER, -1), be_const_int(1) },
        { be_const_key_weak(KEYWORD, 8), be_const_int(0) },
        { be_const_key_weak(RIGHT_PAREN, 68), be_const_int(25) },
        { be_const_key_weak(can_end_expression, 27), be_const_closure(class_Token_can_end_expression_closure) },
        { be_const_key_weak(MINUS, 56), be_const_int(10) },
        { be_const_key_weak(COLOR, -1), be_const_int(4) },
        { be_const_key_weak(get_boolean_value, 46), be_const_closure(class_Token_get_boolean_value_closure) },
        { be_const_key_weak(to_string, -1), be_const_static_closure(class_Token_to_string_closure) },
        { be_const_key_weak(COLON, -1), be_const_int(32) },
        { be_const_key_weak(EVENT_INTERRUPT, -1), be_const_int(41) },
        { be_const_key_weak(LESS_THAN, 64), be_const_int(17) },
        { be_const_key_weak(VARIABLE_REF, -1), be_const_int(36) },
        { be_const_key_weak(type, -1), be_const_var(0) },
        { be_const_key_weak(NOT_EQUAL, 34), be_const_int(16) },
        { be_const_key_weak(length, 63), be_const_var(4) },
        { be_const_key_weak(to_error_string, -1), be_const_closure(class_Token_to_error_string_closure) },
        { be_const_key_weak(line, 24), be_const_var(2) },
        { be_const_key_weak(is_separator, -1), be_const_closure(class_Token_is_separator_closure) },
        { be_const_key_weak(NUMBER, -1), be_const_int(2) },
        { be_const_key_weak(keywords, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(73,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(strip),
        be_nested_str_weak(set),
        be_nested_str_weak(import),
        be_nested_str_weak(berry),
        be_nested_str_weak(color),
        be_nested_str_weak(palette),
        be_nested_str_weak(animation),
        be_nested_str_weak(sequence),
        be_nested_str_weak(function),
        be_nested_str_weak(zone),
        be_nested_str_weak(template),
        be_nested_str_weak(param),
        be_nested_str_weak(type),
        be_nested_str_weak(play),
        be_nested_str_weak(for),
        be_nested_str_weak(with),
        be_nested_str_weak(repeat),
        be_nested_str_weak(times),
        be_nested_str_weak(forever),
        be_nested_str_weak(if),
        be_nested_str_weak(else),
        be_nested_str_weak(elif),
        be_nested_str_weak(choose),
        be_nested_str_weak(random),
        be_nested_str_weak(on),
        be_nested_str_weak(run),
        be_nested_str_weak(wait),
        be_nested_str_weak(goto),
        be_nested_str_weak(interrupt),
        be_nested_str_weak(resume),
        be_nested_str_weak(while),
        be_nested_str_weak(from),
        be_nested_str_weak(to),
        be_nested_str_weak(return),
        be_nested_str_weak(reset),
        be_nested_str_weak(restart),
        be_nested_str_weak(at),
        be_nested_str_weak(ease),
        be_nested_str_weak(sync),
        be_nested_str_weak(every),
        be_nested_str_weak(stagger),
        be_nested_str_weak(across),
        be_nested_str_weak(pixels),
        be_nested_str_weak(rgb),
        be_nested_str_weak(hsv),
        be_nested_str_weak(all),
        be_nested_str_weak(even),
        be_nested_str_weak(odd),
        be_nested_str_weak(center),
        be_nested_str_weak(edges),
        be_nested_str_weak(left),
        be_nested_str_weak(right),
        be_nested_str_weak(top),
        be_nested_str_weak(bottom),
        be_nested_str_weak(true),
        be_nested_str_weak(false),
        be_nested_str_weak(nil),
        be_nested_str_weak(transparent),
        be_nested_str_weak(startup),
        be_nested_str_weak(shutdown),
        be_nested_str_weak(button_press),
        be_nested_str_weak(button_hold),
        be_nested_str_weak(motion_detected),
        be_nested_str_weak(brightness_change),
        be_nested_str_weak(timer),
        be_nested_str_weak(time),
        be_nested_str_weak(sound_peak),
        be_nested_str_weak(network_message),
        be_nested_str_weak(ms),
        be_nested_str_weak(s),
        be_nested_str_weak(m),
        be_nested_str_weak(h),
        be_nested_str_weak(bpm),
    }))    ) } )) },
        { be_const_key_weak(ERROR, -1), be_const_int(39) },
        { be_const_key_weak(DOT, 60), be_const_int(33) },
        { be_const_key_weak(EQUAL, 38), be_const_int(15) },
        { be_const_key_weak(is_literal, -1), be_const_closure(class_Token_is_literal_closure) },
        { be_const_key_weak(LOGICAL_AND, -1), be_const_int(21) },
        { be_const_key_weak(can_start_expression, -1), be_const_closure(class_Token_can_start_expression_closure) },
        { be_const_key_weak(DIVIDE, -1), be_const_int(12) },
        { be_const_key_weak(LEFT_BRACE, 3), be_const_int(26) },
        { be_const_key_weak(is_keyword, -1), be_const_closure(class_Token_is_keyword_closure) },
        { be_const_key_weak(LESS_EQUAL, -1), be_const_int(18) },
        { be_const_key_weak(RIGHT_BRACE, 20), be_const_int(27) },
        { be_const_key_weak(RIGHT_BRACKET, -1), be_const_int(29) },
        { be_const_key_weak(COMMA, -1), be_const_int(30) },
        { be_const_key_weak(is_statement_start, -1), be_const_closure(class_Token_is_statement_start_closure) },
        { be_const_key_weak(names, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(44,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(KEYWORD),
        be_nested_str_weak(IDENTIFIER),
        be_nested_str_weak(NUMBER),
        be_nested_str_weak(STRING),
        be_nested_str_weak(COLOR),
        be_nested_str_weak(TIME),
        be_nested_str_weak(PERCENTAGE),
        be_nested_str_weak(MULTIPLIER),
        be_nested_str_weak(ASSIGN),
        be_nested_str_weak(PLUS),
        be_nested_str_weak(MINUS),
        be_nested_str_weak(MULTIPLY),
        be_nested_str_weak(DIVIDE),
        be_nested_str_weak(MODULO),
        be_nested_str_weak(POWER),
        be_nested_str_weak(EQUAL),
        be_nested_str_weak(NOT_EQUAL),
        be_nested_str_weak(LESS_THAN),
        be_nested_str_weak(LESS_EQUAL),
        be_nested_str_weak(GREATER_THAN),
        be_nested_str_weak(GREATER_EQUAL),
        be_nested_str_weak(LOGICAL_AND),
        be_nested_str_weak(LOGICAL_OR),
        be_nested_str_weak(LOGICAL_NOT),
        be_nested_str_weak(LEFT_PAREN),
        be_nested_str_weak(RIGHT_PAREN),
        be_nested_str_weak(LEFT_BRACE),
        be_nested_str_weak(RIGHT_BRACE),
        be_nested_str_weak(LEFT_BRACKET),
        be_nested_str_weak(RIGHT_BRACKET),
        be_nested_str_weak(COMMA),
        be_nested_str_weak(SEMICOLON),
        be_nested_str_weak(COLON),
        be_nested_str_weak(DOT),
        be_nested_str_weak(ARROW),
        be_nested_str_weak(NEWLINE),
        be_nested_str_weak(VARIABLE_REF),
        be_nested_str_weak(COMMENT),
        be_nested_str_weak(EOF),
        be_nested_str_weak(ERROR),
        be_nested_str_weak(EVENT_ON),
        be_nested_str_weak(EVENT_INTERRUPT),
        be_nested_str_weak(EVENT_RESUME),
        be_nested_str_weak(EVENT_AFTER),
    }))    ) } )) },
        { be_const_key_weak(ARROW, -1), be_const_int(34) },
        { be_const_key_weak(PLUS, -1), be_const_int(9) },
        { be_const_key_weak(is_boolean, -1), be_const_closure(class_Token_is_boolean_closure) },
        { be_const_key_weak(STRING, 14), be_const_int(3) },
        { be_const_key_weak(EVENT_ON, -1), be_const_int(40) },
        { be_const_key_weak(EVENT_AFTER, -1), be_const_int(43) },
        { be_const_key_weak(statement_keywords, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(14,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(strip),
        be_nested_str_weak(set),
        be_nested_str_weak(color),
        be_nested_str_weak(palette),
        be_nested_str_weak(animation),
        be_nested_str_weak(sequence),
        be_nested_str_weak(function),
        be_nested_str_weak(zone),
        be_nested_str_weak(on),
        be_nested_str_weak(run),
        be_nested_str_weak(template),
        be_nested_str_weak(param),
        be_nested_str_weak(import),
        be_nested_str_weak(berry),
    }))    ) } )) },
        { be_const_key_weak(EVENT_RESUME, -1), be_const_int(42) },
    })),
    be_str_weak(Token)
);

/********************************************************************
** Solidified function: is_keyword
********************************************************************/
be_local_closure(is_keyword,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation_dsl),
    /* K1   */  be_nested_str_weak(Token),
    /* K2   */  be_nested_str_weak(keywords),
    /* K3   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(is_keyword),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x60040010,  //  0000  GETGBL	R1	G16
      0xB80A0000,  //  0001  GETNGBL	R2	K0
      0x88080501,  //  0002  GETMBR	R2	R2	K1
      0x88080502,  //  0003  GETMBR	R2	R2	K2
      0x7C040200,  //  0004  CALL	R1	1
      0xA8020007,  //  0005  EXBLK	0	#000E
      0x5C080200,  //  0006  MOVE	R2	R1
      0x7C080000,  //  0007  CALL	R2	0
      0x1C0C0002,  //  0008  EQ	R3	R0	R2
      0x780E0002,  //  0009  JMPF	R3	#000D
      0x500C0200,  //  000A  LDBOOL	R3	1	0
      0xA8040001,  //  000B  EXBLK	1	1
      0x80040600,  //  000C  RET	1	R3
      0x7001FFF7,  //  000D  JMP		#0006
      0x58040003,  //  000E  LDCONST	R1	K3
      0xAC040200,  //  000F  CATCH	R1	1	0
      0xB0080000,  //  0010  RAISE	2	R0	R0
      0x50040000,  //  0011  LDBOOL	R1	0	0
      0x80040200,  //  0012  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_right_associative
********************************************************************/
be_local_closure(is_right_associative,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(type),
    }),
    be_str_weak(is_right_associative),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x540A000D,  //  0001  LDINT	R2	14
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_operator_precedence
********************************************************************/
be_local_closure(get_operator_precedence,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(type),
    /* K1   */  be_const_int(1),
    /* K2   */  be_const_int(2),
    /* K3   */  be_const_int(3),
    /* K4   */  be_const_int(0),
    }),
    be_str_weak(get_operator_precedence),
    &be_const_str_solidified,
    ( &(const binstruction[74]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x540A0015,  //  0001  LDINT	R2	22
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060001,  //  0003  JMPF	R1	#0006
      0x80060200,  //  0004  RET	1	K1
      0x70020042,  //  0005  JMP		#0049
      0x88040100,  //  0006  GETMBR	R1	R0	K0
      0x540A0014,  //  0007  LDINT	R2	21
      0x1C040202,  //  0008  EQ	R1	R1	R2
      0x78060001,  //  0009  JMPF	R1	#000C
      0x80060400,  //  000A  RET	1	K2
      0x7002003C,  //  000B  JMP		#0049
      0x88040100,  //  000C  GETMBR	R1	R0	K0
      0x540A000E,  //  000D  LDINT	R2	15
      0x1C040202,  //  000E  EQ	R1	R1	R2
      0x74060003,  //  000F  JMPT	R1	#0014
      0x88040100,  //  0010  GETMBR	R1	R0	K0
      0x540A000F,  //  0011  LDINT	R2	16
      0x1C040202,  //  0012  EQ	R1	R1	R2
      0x78060001,  //  0013  JMPF	R1	#0016
      0x80060600,  //  0014  RET	1	K3
      0x70020032,  //  0015  JMP		#0049
      0x88040100,  //  0016  GETMBR	R1	R0	K0
      0x540A0010,  //  0017  LDINT	R2	17
      0x1C040202,  //  0018  EQ	R1	R1	R2
      0x7406000B,  //  0019  JMPT	R1	#0026
      0x88040100,  //  001A  GETMBR	R1	R0	K0
      0x540A0011,  //  001B  LDINT	R2	18
      0x1C040202,  //  001C  EQ	R1	R1	R2
      0x74060007,  //  001D  JMPT	R1	#0026
      0x88040100,  //  001E  GETMBR	R1	R0	K0
      0x540A0012,  //  001F  LDINT	R2	19
      0x1C040202,  //  0020  EQ	R1	R1	R2
      0x74060003,  //  0021  JMPT	R1	#0026
      0x88040100,  //  0022  GETMBR	R1	R0	K0
      0x540A0013,  //  0023  LDINT	R2	20
      0x1C040202,  //  0024  EQ	R1	R1	R2
      0x78060002,  //  0025  JMPF	R1	#0029
      0x54060003,  //  0026  LDINT	R1	4
      0x80040200,  //  0027  RET	1	R1
      0x7002001F,  //  0028  JMP		#0049
      0x88040100,  //  0029  GETMBR	R1	R0	K0
      0x540A0008,  //  002A  LDINT	R2	9
      0x1C040202,  //  002B  EQ	R1	R1	R2
      0x74060003,  //  002C  JMPT	R1	#0031
      0x88040100,  //  002D  GETMBR	R1	R0	K0
      0x540A0009,  //  002E  LDINT	R2	10
      0x1C040202,  //  002F  EQ	R1	R1	R2
      0x78060002,  //  0030  JMPF	R1	#0034
      0x54060004,  //  0031  LDINT	R1	5
      0x80040200,  //  0032  RET	1	R1
      0x70020014,  //  0033  JMP		#0049
      0x88040100,  //  0034  GETMBR	R1	R0	K0
      0x540A000A,  //  0035  LDINT	R2	11
      0x1C040202,  //  0036  EQ	R1	R1	R2
      0x74060007,  //  0037  JMPT	R1	#0040
      0x88040100,  //  0038  GETMBR	R1	R0	K0
      0x540A000B,  //  0039  LDINT	R2	12
      0x1C040202,  //  003A  EQ	R1	R1	R2
      0x74060003,  //  003B  JMPT	R1	#0040
      0x88040100,  //  003C  GETMBR	R1	R0	K0
      0x540A000C,  //  003D  LDINT	R2	13
      0x1C040202,  //  003E  EQ	R1	R1	R2
      0x78060002,  //  003F  JMPF	R1	#0043
      0x54060005,  //  0040  LDINT	R1	6
      0x80040200,  //  0041  RET	1	R1
      0x70020005,  //  0042  JMP		#0049
      0x88040100,  //  0043  GETMBR	R1	R0	K0
      0x540A000D,  //  0044  LDINT	R2	14
      0x1C040202,  //  0045  EQ	R1	R1	R2
      0x78060001,  //  0046  JMPF	R1	#0049
      0x54060006,  //  0047  LDINT	R1	7
      0x80040200,  //  0048  RET	1	R1
      0x80060800,  //  0049  RET	1	K4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_eof_token
********************************************************************/
be_local_closure(create_eof_token,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation_dsl),
    /* K1   */  be_nested_str_weak(Token),
    /* K2   */  be_nested_str_weak(),
    /* K3   */  be_const_int(0),
    }),
    be_str_weak(create_eof_token),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x54120025,  //  0002  LDINT	R4	38
      0x58140002,  //  0003  LDCONST	R5	K2
      0x5C180000,  //  0004  MOVE	R6	R0
      0x5C1C0200,  //  0005  MOVE	R7	R1
      0x58200003,  //  0006  LDCONST	R8	K3
      0x7C080C00,  //  0007  CALL	R2	6
      0x80040400,  //  0008  RET	1	R2
    })
  )
);
/*******************************************************************/


extern const bclass be_class_MockEngine;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_MockEngine_init,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(time_ms),
    /* K1   */  be_const_int(0),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020101,  //  0000  SETMBR	R0	K0	K1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_strip_length
********************************************************************/
be_local_closure(class_MockEngine_get_strip_length,   /* name */
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
    be_str_weak(get_strip_length),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x5406001D,  //  0000  LDINT	R1	30
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: MockEngine
********************************************************************/
be_local_class(MockEngine,
    1,
    NULL,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(time_ms, 1), be_const_var(0) },
        { be_const_key_weak(init, -1), be_const_closure(class_MockEngine_init_closure) },
        { be_const_key_weak(get_strip_length, -1), be_const_closure(class_MockEngine_get_strip_length_closure) },
    })),
    be_str_weak(MockEngine)
);

/********************************************************************
** Solidified function: tokenize_dsl
********************************************************************/
be_local_closure(tokenize_dsl,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation_dsl),
    /* K1   */  be_nested_str_weak(DSLLexer),
    /* K2   */  be_nested_str_weak(tokenize),
    }),
    be_str_weak(tokenize_dsl),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x8C080302,  //  0004  GETMET	R2	R1	K2
      0x7C080200,  //  0005  CALL	R2	1
      0x80040400,  //  0006  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_color_name
********************************************************************/
be_local_closure(is_color_name,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation_dsl),
    /* K1   */  be_nested_str_weak(Token),
    /* K2   */  be_nested_str_weak(color_names),
    /* K3   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(is_color_name),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x60040010,  //  0000  GETGBL	R1	G16
      0xB80A0000,  //  0001  GETNGBL	R2	K0
      0x88080501,  //  0002  GETMBR	R2	R2	K1
      0x88080502,  //  0003  GETMBR	R2	R2	K2
      0x7C040200,  //  0004  CALL	R1	1
      0xA8020007,  //  0005  EXBLK	0	#000E
      0x5C080200,  //  0006  MOVE	R2	R1
      0x7C080000,  //  0007  CALL	R2	0
      0x1C0C0002,  //  0008  EQ	R3	R0	R2
      0x780E0002,  //  0009  JMPF	R3	#000D
      0x500C0200,  //  000A  LDBOOL	R3	1	0
      0xA8040001,  //  000B  EXBLK	1	1
      0x80040600,  //  000C  RET	1	R3
      0x7001FFF7,  //  000D  JMP		#0006
      0x58040003,  //  000E  LDCONST	R1	K3
      0xAC040200,  //  000F  CATCH	R1	1	0
      0xB0080000,  //  0010  RAISE	2	R0	R0
      0x50040000,  //  0011  LDBOOL	R1	0	0
      0x80040200,  //  0012  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_error_token
********************************************************************/
be_local_closure(create_error_token,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation_dsl),
    /* K1   */  be_nested_str_weak(Token),
    }),
    be_str_weak(create_error_token),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x54160026,  //  0002  LDINT	R5	39
      0x5C180000,  //  0003  MOVE	R6	R0
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x5C200400,  //  0005  MOVE	R8	R2
      0x6024000C,  //  0006  GETGBL	R9	G12
      0x5C280000,  //  0007  MOVE	R10	R0
      0x7C240200,  //  0008  CALL	R9	1
      0x7C0C0C00,  //  0009  CALL	R3	6
      0x80040600,  //  000A  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: load_file
********************************************************************/
be_local_closure(load_file,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(r),
    /* K1   */  be_nested_str_weak(Cannot_X20open_X20DSL_X20file_X3A_X20_X25s),
    /* K2   */  be_nested_str_weak(io_error),
    /* K3   */  be_nested_str_weak(read),
    /* K4   */  be_nested_str_weak(close),
    /* K5   */  be_nested_str_weak(animation_dsl),
    /* K6   */  be_nested_str_weak(execute),
    }),
    be_str_weak(load_file),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x60040011,  //  0000  GETGBL	R1	G17
      0x5C080000,  //  0001  MOVE	R2	R0
      0x580C0000,  //  0002  LDCONST	R3	K0
      0x7C040400,  //  0003  CALL	R1	2
      0x4C080000,  //  0004  LDNIL	R2
      0x1C080202,  //  0005  EQ	R2	R1	R2
      0x780A0004,  //  0006  JMPF	R2	#000C
      0x60080018,  //  0007  GETGBL	R2	G24
      0x580C0001,  //  0008  LDCONST	R3	K1
      0x5C100000,  //  0009  MOVE	R4	R0
      0x7C080400,  //  000A  CALL	R2	2
      0xB0060402,  //  000B  RAISE	1	K2	R2
      0x8C080303,  //  000C  GETMET	R2	R1	K3
      0x7C080200,  //  000D  CALL	R2	1
      0x8C0C0304,  //  000E  GETMET	R3	R1	K4
      0x7C0C0200,  //  000F  CALL	R3	1
      0xB80E0A00,  //  0010  GETNGBL	R3	K5
      0x8C0C0706,  //  0011  GETMET	R3	R3	K6
      0x5C140400,  //  0012  MOVE	R5	R2
      0x7C0C0400,  //  0013  CALL	R3	2
      0x80040600,  //  0014  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tokenize_dsl_with_errors
********************************************************************/
be_local_closure(tokenize_dsl_with_errors,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation_dsl),
    /* K1   */  be_nested_str_weak(DSLLexer),
    /* K2   */  be_nested_str_weak(tokenize_with_errors),
    }),
    be_str_weak(tokenize_dsl_with_errors),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x8C080302,  //  0004  GETMET	R2	R1	K2
      0x7C080200,  //  0005  CALL	R2	1
      0x80040400,  //  0006  RET	1	R2
    })
  )
);
/*******************************************************************/

// compact class 'DSLLexer' ktab size: 116, total: 294 (saved 1424 bytes)
static const bvalue be_ktab_class_DSLLexer[116] = {
  /* K0   */  be_nested_str_weak(column),
  /* K1   */  be_nested_str_weak(advance),
  /* K2   */  be_nested_str_weak(_X20),
  /* K3   */  be_nested_str_weak(_X09),
  /* K4   */  be_nested_str_weak(_X0D),
  /* K5   */  be_nested_str_weak(_X0A),
  /* K6   */  be_nested_str_weak(add_token),
  /* K7   */  be_const_int(1),
  /* K8   */  be_nested_str_weak(line),
  /* K9   */  be_nested_str_weak(_X23),
  /* K10  */  be_nested_str_weak(scan_comment),
  /* K11  */  be_nested_str_weak(0),
  /* K12  */  be_nested_str_weak(peek),
  /* K13  */  be_nested_str_weak(x),
  /* K14  */  be_nested_str_weak(scan_hex_color_0x),
  /* K15  */  be_nested_str_weak(is_alpha),
  /* K16  */  be_nested_str_weak(_),
  /* K17  */  be_nested_str_weak(scan_identifier_or_keyword),
  /* K18  */  be_nested_str_weak(is_digit),
  /* K19  */  be_nested_str_weak(scan_number),
  /* K20  */  be_nested_str_weak(_X22),
  /* K21  */  be_nested_str_weak(_X27),
  /* K22  */  be_nested_str_weak(peek_ahead),
  /* K23  */  be_nested_str_weak(scan_triple_quoted_string),
  /* K24  */  be_nested_str_weak(scan_string),
  /* K25  */  be_nested_str_weak(_X24),
  /* K26  */  be_nested_str_weak(scan_variable_reference),
  /* K27  */  be_nested_str_weak(scan_operator_or_delimiter),
  /* K28  */  be_nested_str_weak(position),
  /* K29  */  be_nested_str_weak(source),
  /* K30  */  be_nested_str_weak(),
  /* K31  */  be_nested_str_weak(at_end),
  /* K32  */  be_nested_str_weak(_X5C),
  /* K33  */  be_nested_str_weak(n),
  /* K34  */  be_nested_str_weak(t),
  /* K35  */  be_nested_str_weak(r),
  /* K36  */  be_nested_str_weak(add_error),
  /* K37  */  be_nested_str_weak(Unterminated_X20string_X20literal),
  /* K38  */  be_const_int(3),
  /* K39  */  be_nested_str_weak(animation_dsl),
  /* K40  */  be_nested_str_weak(Token),
  /* K41  */  be_nested_str_weak(tokens),
  /* K42  */  be_nested_str_weak(push),
  /* K43  */  be_nested_str_weak(has_errors),
  /* K44  */  be_nested_str_weak(No_X20lexical_X20errors),
  /* K45  */  be_nested_str_weak(Lexical_X20errors_X20_X28),
  /* K46  */  be_nested_str_weak(errors),
  /* K47  */  be_nested_str_weak(_X29_X3A_X0A),
  /* K48  */  be_nested_str_weak(_X20_X20Line_X20),
  /* K49  */  be_nested_str_weak(_X3A),
  /* K50  */  be_nested_str_weak(_X3A_X20),
  /* K51  */  be_nested_str_weak(message),
  /* K52  */  be_nested_str_weak(stop_iteration),
  /* K53  */  be_nested_str_weak(a),
  /* K54  */  be_nested_str_weak(f),
  /* K55  */  be_nested_str_weak(A),
  /* K56  */  be_nested_str_weak(F),
  /* K57  */  be_const_int(0),
  /* K58  */  be_nested_str_weak(_X3D),
  /* K59  */  be_nested_str_weak(match),
  /* K60  */  be_nested_str_weak(_X3D_X3D),
  /* K61  */  be_const_int(2),
  /* K62  */  be_nested_str_weak(_X21),
  /* K63  */  be_nested_str_weak(_X21_X3D),
  /* K64  */  be_nested_str_weak(_X3C),
  /* K65  */  be_nested_str_weak(_X3C_X3D),
  /* K66  */  be_nested_str_weak(_X3C_X3C),
  /* K67  */  be_nested_str_weak(_X3E),
  /* K68  */  be_nested_str_weak(_X3E_X3D),
  /* K69  */  be_nested_str_weak(_X3E_X3E),
  /* K70  */  be_nested_str_weak(_X26),
  /* K71  */  be_nested_str_weak(_X26_X26),
  /* K72  */  be_nested_str_weak(Single_X20_X27_X26_X27_X20not_X20supported_X20in_X20DSL),
  /* K73  */  be_nested_str_weak(_X7C),
  /* K74  */  be_nested_str_weak(_X7C_X7C),
  /* K75  */  be_nested_str_weak(Single_X20_X27_X7C_X27_X20not_X20supported_X20in_X20DSL),
  /* K76  */  be_nested_str_weak(_X2D),
  /* K77  */  be_nested_str_weak(_X2D_X3E),
  /* K78  */  be_nested_str_weak(_X2B),
  /* K79  */  be_nested_str_weak(_X2A),
  /* K80  */  be_nested_str_weak(_X2F),
  /* K81  */  be_nested_str_weak(_X25),
  /* K82  */  be_nested_str_weak(_X5E),
  /* K83  */  be_nested_str_weak(_X28),
  /* K84  */  be_nested_str_weak(_X29),
  /* K85  */  be_nested_str_weak(_X7B),
  /* K86  */  be_nested_str_weak(_X7D),
  /* K87  */  be_nested_str_weak(_X5B),
  /* K88  */  be_nested_str_weak(_X5D),
  /* K89  */  be_nested_str_weak(_X2C),
  /* K90  */  be_nested_str_weak(_X3B),
  /* K91  */  be_nested_str_weak(_X2E),
  /* K92  */  be_nested_str_weak(Unexpected_X20character_X3A_X20_X27),
  /* K93  */  be_nested_str_weak(9),
  /* K94  */  be_nested_str_weak(tokenize),
  /* K95  */  be_nested_str_weak(success),
  /* K96  */  be_nested_str_weak(Invalid_X20variable_X20reference_X3A_X20_X24_X20must_X20be_X20followed_X20by_X20identifier),
  /* K97  */  be_nested_str_weak(is_alnum),
  /* K98  */  be_nested_str_weak(string),
  /* K99  */  be_const_int(2147483647),
  /* K100 */  be_nested_str_weak(startswith),
  /* K101 */  be_nested_str_weak(ms),
  /* K102 */  be_nested_str_weak(s),
  /* K103 */  be_nested_str_weak(m),
  /* K104 */  be_nested_str_weak(h),
  /* K105 */  be_nested_str_weak(check_time_suffix),
  /* K106 */  be_nested_str_weak(scan_time_suffix),
  /* K107 */  be_nested_str_weak(is_color_name),
  /* K108 */  be_nested_str_weak(is_keyword),
  /* K109 */  be_nested_str_weak(is_hex_digit),
  /* K110 */  be_nested_str_weak(Invalid_X20hex_X20color_X20format_X3A_X20),
  /* K111 */  be_nested_str_weak(_X20_X28expected_X200xRRGGBB_X20or_X200xAARRGGBB_X29),
  /* K112 */  be_nested_str_weak(z),
  /* K113 */  be_nested_str_weak(Z),
  /* K114 */  be_nested_str_weak(Unterminated_X20triple_X2Dquoted_X20string_X20literal),
  /* K115 */  be_nested_str_weak(scan_token),
};


extern const bclass be_class_DSLLexer;

/********************************************************************
** Solidified function: scan_token
********************************************************************/
be_local_closure(class_DSLLexer_scan_token,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(scan_token),
    &be_const_str_solidified,
    ( &(const binstruction[97]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C080101,  //  0001  GETMET	R2	R0	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x1C0C0502,  //  0003  EQ	R3	R2	K2
      0x740E0003,  //  0004  JMPT	R3	#0009
      0x1C0C0503,  //  0005  EQ	R3	R2	K3
      0x740E0001,  //  0006  JMPT	R3	#0009
      0x1C0C0504,  //  0007  EQ	R3	R2	K4
      0x780E0001,  //  0008  JMPF	R3	#000B
      0x80000600,  //  0009  RET	0
      0x70020054,  //  000A  JMP		#0060
      0x1C0C0505,  //  000B  EQ	R3	R2	K5
      0x780E000A,  //  000C  JMPF	R3	#0018
      0x8C0C0106,  //  000D  GETMET	R3	R0	K6
      0x54160022,  //  000E  LDINT	R5	35
      0x58180005,  //  000F  LDCONST	R6	K5
      0x581C0007,  //  0010  LDCONST	R7	K7
      0x7C0C0800,  //  0011  CALL	R3	4
      0x880C0108,  //  0012  GETMBR	R3	R0	K8
      0x000C0707,  //  0013  ADD	R3	R3	K7
      0x90021003,  //  0014  SETMBR	R0	K8	R3
      0x90020107,  //  0015  SETMBR	R0	K0	K7
      0x80000600,  //  0016  RET	0
      0x70020047,  //  0017  JMP		#0060
      0x1C0C0509,  //  0018  EQ	R3	R2	K9
      0x780E0002,  //  0019  JMPF	R3	#001D
      0x8C0C010A,  //  001A  GETMET	R3	R0	K10
      0x7C0C0200,  //  001B  CALL	R3	1
      0x70020042,  //  001C  JMP		#0060
      0x1C0C050B,  //  001D  EQ	R3	R2	K11
      0x780E0006,  //  001E  JMPF	R3	#0026
      0x8C0C010C,  //  001F  GETMET	R3	R0	K12
      0x7C0C0200,  //  0020  CALL	R3	1
      0x1C0C070D,  //  0021  EQ	R3	R3	K13
      0x780E0002,  //  0022  JMPF	R3	#0026
      0x8C0C010E,  //  0023  GETMET	R3	R0	K14
      0x7C0C0200,  //  0024  CALL	R3	1
      0x70020039,  //  0025  JMP		#0060
      0x8C0C010F,  //  0026  GETMET	R3	R0	K15
      0x5C140400,  //  0027  MOVE	R5	R2
      0x7C0C0400,  //  0028  CALL	R3	2
      0x740E0001,  //  0029  JMPT	R3	#002C
      0x1C0C0510,  //  002A  EQ	R3	R2	K16
      0x780E0002,  //  002B  JMPF	R3	#002F
      0x8C0C0111,  //  002C  GETMET	R3	R0	K17
      0x7C0C0200,  //  002D  CALL	R3	1
      0x70020030,  //  002E  JMP		#0060
      0x8C0C0112,  //  002F  GETMET	R3	R0	K18
      0x5C140400,  //  0030  MOVE	R5	R2
      0x7C0C0400,  //  0031  CALL	R3	2
      0x780E0002,  //  0032  JMPF	R3	#0036
      0x8C0C0113,  //  0033  GETMET	R3	R0	K19
      0x7C0C0200,  //  0034  CALL	R3	1
      0x70020029,  //  0035  JMP		#0060
      0x1C0C0514,  //  0036  EQ	R3	R2	K20
      0x740E0001,  //  0037  JMPT	R3	#003A
      0x1C0C0515,  //  0038  EQ	R3	R2	K21
      0x780E001D,  //  0039  JMPF	R3	#0058
      0x1C0C0514,  //  003A  EQ	R3	R2	K20
      0x780E0008,  //  003B  JMPF	R3	#0045
      0x8C0C010C,  //  003C  GETMET	R3	R0	K12
      0x7C0C0200,  //  003D  CALL	R3	1
      0x1C0C0714,  //  003E  EQ	R3	R3	K20
      0x780E0004,  //  003F  JMPF	R3	#0045
      0x8C0C0116,  //  0040  GETMET	R3	R0	K22
      0x58140007,  //  0041  LDCONST	R5	K7
      0x7C0C0400,  //  0042  CALL	R3	2
      0x1C0C0714,  //  0043  EQ	R3	R3	K20
      0x740E000A,  //  0044  JMPT	R3	#0050
      0x1C0C0515,  //  0045  EQ	R3	R2	K21
      0x780E000C,  //  0046  JMPF	R3	#0054
      0x8C0C010C,  //  0047  GETMET	R3	R0	K12
      0x7C0C0200,  //  0048  CALL	R3	1
      0x1C0C0715,  //  0049  EQ	R3	R3	K21
      0x780E0008,  //  004A  JMPF	R3	#0054
      0x8C0C0116,  //  004B  GETMET	R3	R0	K22
      0x58140007,  //  004C  LDCONST	R5	K7
      0x7C0C0400,  //  004D  CALL	R3	2
      0x1C0C0715,  //  004E  EQ	R3	R3	K21
      0x780E0003,  //  004F  JMPF	R3	#0054
      0x8C0C0117,  //  0050  GETMET	R3	R0	K23
      0x5C140400,  //  0051  MOVE	R5	R2
      0x7C0C0400,  //  0052  CALL	R3	2
      0x70020002,  //  0053  JMP		#0057
      0x8C0C0118,  //  0054  GETMET	R3	R0	K24
      0x5C140400,  //  0055  MOVE	R5	R2
      0x7C0C0400,  //  0056  CALL	R3	2
      0x70020007,  //  0057  JMP		#0060
      0x1C0C0519,  //  0058  EQ	R3	R2	K25
      0x780E0002,  //  0059  JMPF	R3	#005D
      0x8C0C011A,  //  005A  GETMET	R3	R0	K26
      0x7C0C0200,  //  005B  CALL	R3	1
      0x70020002,  //  005C  JMP		#0060
      0x8C0C011B,  //  005D  GETMET	R3	R0	K27
      0x5C140400,  //  005E  MOVE	R5	R2
      0x7C0C0400,  //  005F  CALL	R3	2
      0x80000000,  //  0060  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: peek_ahead
********************************************************************/
be_local_closure(class_DSLLexer_peek_ahead,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(peek_ahead),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x8808011C,  //  0000  GETMBR	R2	R0	K28
      0x00080401,  //  0001  ADD	R2	R2	R1
      0x600C000C,  //  0002  GETGBL	R3	G12
      0x8810011D,  //  0003  GETMBR	R4	R0	K29
      0x7C0C0200,  //  0004  CALL	R3	1
      0x28080403,  //  0005  GE	R2	R2	R3
      0x780A0000,  //  0006  JMPF	R2	#0008
      0x80063C00,  //  0007  RET	1	K30
      0x8808011C,  //  0008  GETMBR	R2	R0	K28
      0x00080401,  //  0009  ADD	R2	R2	R1
      0x880C011D,  //  000A  GETMBR	R3	R0	K29
      0x94080602,  //  000B  GETIDX	R2	R3	R2
      0x80040400,  //  000C  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: scan_string
********************************************************************/
be_local_closure(class_DSLLexer_scan_string,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(scan_string),
    &be_const_str_solidified,
    ( &(const binstruction[78]) {  /* code */
      0x8808011C,  //  0000  GETMBR	R2	R0	K28
      0x04080507,  //  0001  SUB	R2	R2	K7
      0x880C0100,  //  0002  GETMBR	R3	R0	K0
      0x040C0707,  //  0003  SUB	R3	R3	K7
      0x5810001E,  //  0004  LDCONST	R4	K30
      0x8C14011F,  //  0005  GETMET	R5	R0	K31
      0x7C140200,  //  0006  CALL	R5	1
      0x7416002F,  //  0007  JMPT	R5	#0038
      0x8C14010C,  //  0008  GETMET	R5	R0	K12
      0x7C140200,  //  0009  CALL	R5	1
      0x20140A01,  //  000A  NE	R5	R5	R1
      0x7816002B,  //  000B  JMPF	R5	#0038
      0x8C140101,  //  000C  GETMET	R5	R0	K1
      0x7C140200,  //  000D  CALL	R5	1
      0x1C180B20,  //  000E  EQ	R6	R5	K32
      0x781A001D,  //  000F  JMPF	R6	#002E
      0x8C18011F,  //  0010  GETMET	R6	R0	K31
      0x7C180200,  //  0011  CALL	R6	1
      0x741A0018,  //  0012  JMPT	R6	#002C
      0x8C180101,  //  0013  GETMET	R6	R0	K1
      0x7C180200,  //  0014  CALL	R6	1
      0x1C1C0D21,  //  0015  EQ	R7	R6	K33
      0x781E0001,  //  0016  JMPF	R7	#0019
      0x00100905,  //  0017  ADD	R4	R4	K5
      0x70020011,  //  0018  JMP		#002B
      0x1C1C0D22,  //  0019  EQ	R7	R6	K34
      0x781E0001,  //  001A  JMPF	R7	#001D
      0x00100903,  //  001B  ADD	R4	R4	K3
      0x7002000D,  //  001C  JMP		#002B
      0x1C1C0D23,  //  001D  EQ	R7	R6	K35
      0x781E0001,  //  001E  JMPF	R7	#0021
      0x00100904,  //  001F  ADD	R4	R4	K4
      0x70020009,  //  0020  JMP		#002B
      0x1C1C0D20,  //  0021  EQ	R7	R6	K32
      0x781E0001,  //  0022  JMPF	R7	#0025
      0x00100920,  //  0023  ADD	R4	R4	K32
      0x70020005,  //  0024  JMP		#002B
      0x1C1C0C01,  //  0025  EQ	R7	R6	R1
      0x781E0001,  //  0026  JMPF	R7	#0029
      0x00100801,  //  0027  ADD	R4	R4	R1
      0x70020001,  //  0028  JMP		#002B
      0x00100920,  //  0029  ADD	R4	R4	K32
      0x00100806,  //  002A  ADD	R4	R4	R6
      0x70020000,  //  002B  JMP		#002D
      0x00100920,  //  002C  ADD	R4	R4	K32
      0x70020008,  //  002D  JMP		#0037
      0x1C180B05,  //  002E  EQ	R6	R5	K5
      0x781A0005,  //  002F  JMPF	R6	#0036
      0x88180108,  //  0030  GETMBR	R6	R0	K8
      0x00180D07,  //  0031  ADD	R6	R6	K7
      0x90021006,  //  0032  SETMBR	R0	K8	R6
      0x90020107,  //  0033  SETMBR	R0	K0	K7
      0x00100805,  //  0034  ADD	R4	R4	R5
      0x70020000,  //  0035  JMP		#0037
      0x00100805,  //  0036  ADD	R4	R4	R5
      0x7001FFCC,  //  0037  JMP		#0005
      0x8C14011F,  //  0038  GETMET	R5	R0	K31
      0x7C140200,  //  0039  CALL	R5	1
      0x78160009,  //  003A  JMPF	R5	#0045
      0x8C140124,  //  003B  GETMET	R5	R0	K36
      0x581C0025,  //  003C  LDCONST	R7	K37
      0x7C140400,  //  003D  CALL	R5	2
      0x8C140106,  //  003E  GETMET	R5	R0	K6
      0x541E0026,  //  003F  LDINT	R7	39
      0x5C200800,  //  0040  MOVE	R8	R4
      0x8824011C,  //  0041  GETMBR	R9	R0	K28
      0x04241202,  //  0042  SUB	R9	R9	R2
      0x7C140800,  //  0043  CALL	R5	4
      0x70020007,  //  0044  JMP		#004D
      0x8C140101,  //  0045  GETMET	R5	R0	K1
      0x7C140200,  //  0046  CALL	R5	1
      0x8C140106,  //  0047  GETMET	R5	R0	K6
      0x581C0026,  //  0048  LDCONST	R7	K38
      0x5C200800,  //  0049  MOVE	R8	R4
      0x8824011C,  //  004A  GETMBR	R9	R0	K28
      0x04241202,  //  004B  SUB	R9	R9	R2
      0x7C140800,  //  004C  CALL	R5	4
      0x80000000,  //  004D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_token
********************************************************************/
be_local_closure(class_DSLLexer_add_token,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(add_token),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0xB8124E00,  //  0000  GETNGBL	R4	K39
      0x8C100928,  //  0001  GETMET	R4	R4	K40
      0x5C180200,  //  0002  MOVE	R6	R1
      0x5C1C0400,  //  0003  MOVE	R7	R2
      0x88200108,  //  0004  GETMBR	R8	R0	K8
      0x88240100,  //  0005  GETMBR	R9	R0	K0
      0x04241203,  //  0006  SUB	R9	R9	R3
      0x5C280600,  //  0007  MOVE	R10	R3
      0x7C100C00,  //  0008  CALL	R4	6
      0x88140129,  //  0009  GETMBR	R5	R0	K41
      0x8C140B2A,  //  000A  GETMET	R5	R5	K42
      0x5C1C0800,  //  000B  MOVE	R7	R4
      0x7C140400,  //  000C  CALL	R5	2
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: peek
********************************************************************/
be_local_closure(class_DSLLexer_peek,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(peek),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x8C04011F,  //  0000  GETMET	R1	R0	K31
      0x7C040200,  //  0001  CALL	R1	1
      0x78060000,  //  0002  JMPF	R1	#0004
      0x80063C00,  //  0003  RET	1	K30
      0x8804011D,  //  0004  GETMBR	R1	R0	K29
      0x8808011C,  //  0005  GETMBR	R2	R0	K28
      0x94040202,  //  0006  GETIDX	R1	R1	R2
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_error_report
********************************************************************/
be_local_closure(class_DSLLexer_get_error_report,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(get_error_report),
    &be_const_str_solidified,
    ( &(const binstruction[36]) {  /* code */
      0x8C04012B,  //  0000  GETMET	R1	R0	K43
      0x7C040200,  //  0001  CALL	R1	1
      0x74060000,  //  0002  JMPT	R1	#0004
      0x80065800,  //  0003  RET	1	K44
      0x60040008,  //  0004  GETGBL	R1	G8
      0x6008000C,  //  0005  GETGBL	R2	G12
      0x880C012E,  //  0006  GETMBR	R3	R0	K46
      0x7C080200,  //  0007  CALL	R2	1
      0x7C040200,  //  0008  CALL	R1	1
      0x00065A01,  //  0009  ADD	R1	K45	R1
      0x0004032F,  //  000A  ADD	R1	R1	K47
      0x60080010,  //  000B  GETGBL	R2	G16
      0x880C012E,  //  000C  GETMBR	R3	R0	K46
      0x7C080200,  //  000D  CALL	R2	1
      0xA8020010,  //  000E  EXBLK	0	#0020
      0x5C0C0400,  //  000F  MOVE	R3	R2
      0x7C0C0000,  //  0010  CALL	R3	0
      0x60100008,  //  0011  GETGBL	R4	G8
      0x94140708,  //  0012  GETIDX	R5	R3	K8
      0x7C100200,  //  0013  CALL	R4	1
      0x00126004,  //  0014  ADD	R4	K48	R4
      0x00100931,  //  0015  ADD	R4	R4	K49
      0x60140008,  //  0016  GETGBL	R5	G8
      0x94180700,  //  0017  GETIDX	R6	R3	K0
      0x7C140200,  //  0018  CALL	R5	1
      0x00100805,  //  0019  ADD	R4	R4	R5
      0x00100932,  //  001A  ADD	R4	R4	K50
      0x94140733,  //  001B  GETIDX	R5	R3	K51
      0x00100805,  //  001C  ADD	R4	R4	R5
      0x00100905,  //  001D  ADD	R4	R4	K5
      0x00040204,  //  001E  ADD	R1	R1	R4
      0x7001FFEE,  //  001F  JMP		#000F
      0x58080034,  //  0020  LDCONST	R2	K52
      0xAC080200,  //  0021  CATCH	R2	1	0
      0xB0080000,  //  0022  RAISE	2	R0	R0
      0x80040200,  //  0023  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: advance
********************************************************************/
be_local_closure(class_DSLLexer_advance,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(advance),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x8C04011F,  //  0000  GETMET	R1	R0	K31
      0x7C040200,  //  0001  CALL	R1	1
      0x78060000,  //  0002  JMPF	R1	#0004
      0x80063C00,  //  0003  RET	1	K30
      0x8804011D,  //  0004  GETMBR	R1	R0	K29
      0x8808011C,  //  0005  GETMBR	R2	R0	K28
      0x94040202,  //  0006  GETIDX	R1	R1	R2
      0x8808011C,  //  0007  GETMBR	R2	R0	K28
      0x00080507,  //  0008  ADD	R2	R2	K7
      0x90023802,  //  0009  SETMBR	R0	K28	R2
      0x88080100,  //  000A  GETMBR	R2	R0	K0
      0x00080507,  //  000B  ADD	R2	R2	K7
      0x90020002,  //  000C  SETMBR	R0	K0	R2
      0x80040200,  //  000D  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_hex_digit
********************************************************************/
be_local_closure(class_DSLLexer_is_hex_digit,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(is_hex_digit),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x8C080112,  //  0000  GETMET	R2	R0	K18
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x740A0008,  //  0003  JMPT	R2	#000D
      0x28080335,  //  0004  GE	R2	R1	K53
      0x780A0001,  //  0005  JMPF	R2	#0008
      0x18080336,  //  0006  LE	R2	R1	K54
      0x740A0004,  //  0007  JMPT	R2	#000D
      0x28080337,  //  0008  GE	R2	R1	K55
      0x780A0001,  //  0009  JMPF	R2	#000C
      0x18080338,  //  000A  LE	R2	R1	K56
      0x740A0000,  //  000B  JMPT	R2	#000D
      0x50080001,  //  000C  LDBOOL	R2	0	1
      0x50080200,  //  000D  LDBOOL	R2	1	0
      0x80040400,  //  000E  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_DSLLexer_init,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x20080202,  //  0001  NE	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x5C080200,  //  0003  MOVE	R2	R1
      0x70020000,  //  0004  JMP		#0006
      0x5808001E,  //  0005  LDCONST	R2	K30
      0x90023A02,  //  0006  SETMBR	R0	K29	R2
      0x90023939,  //  0007  SETMBR	R0	K28	K57
      0x90021107,  //  0008  SETMBR	R0	K8	K7
      0x90020107,  //  0009  SETMBR	R0	K0	K7
      0x60080012,  //  000A  GETGBL	R2	G18
      0x7C080000,  //  000B  CALL	R2	0
      0x90025202,  //  000C  SETMBR	R0	K41	R2
      0x60080012,  //  000D  GETGBL	R2	G18
      0x7C080000,  //  000E  CALL	R2	0
      0x90025C02,  //  000F  SETMBR	R0	K46	R2
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: scan_operator_or_delimiter
********************************************************************/
be_local_closure(class_DSLLexer_scan_operator_or_delimiter,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(scan_operator_or_delimiter),
    &be_const_str_solidified,
    ( &(const binstruction[299]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x04080507,  //  0001  SUB	R2	R2	K7
      0x1C0C033A,  //  0002  EQ	R3	R1	K58
      0x780E000F,  //  0003  JMPF	R3	#0014
      0x8C0C013B,  //  0004  GETMET	R3	R0	K59
      0x5814003A,  //  0005  LDCONST	R5	K58
      0x7C0C0400,  //  0006  CALL	R3	2
      0x780E0005,  //  0007  JMPF	R3	#000E
      0x8C0C0106,  //  0008  GETMET	R3	R0	K6
      0x5416000E,  //  0009  LDINT	R5	15
      0x5818003C,  //  000A  LDCONST	R6	K60
      0x581C003D,  //  000B  LDCONST	R7	K61
      0x7C0C0800,  //  000C  CALL	R3	4
      0x70020004,  //  000D  JMP		#0013
      0x8C0C0106,  //  000E  GETMET	R3	R0	K6
      0x54160007,  //  000F  LDINT	R5	8
      0x5818003A,  //  0010  LDCONST	R6	K58
      0x581C0007,  //  0011  LDCONST	R7	K7
      0x7C0C0800,  //  0012  CALL	R3	4
      0x70020115,  //  0013  JMP		#012A
      0x1C0C033E,  //  0014  EQ	R3	R1	K62
      0x780E000F,  //  0015  JMPF	R3	#0026
      0x8C0C013B,  //  0016  GETMET	R3	R0	K59
      0x5814003A,  //  0017  LDCONST	R5	K58
      0x7C0C0400,  //  0018  CALL	R3	2
      0x780E0005,  //  0019  JMPF	R3	#0020
      0x8C0C0106,  //  001A  GETMET	R3	R0	K6
      0x5416000F,  //  001B  LDINT	R5	16
      0x5818003F,  //  001C  LDCONST	R6	K63
      0x581C003D,  //  001D  LDCONST	R7	K61
      0x7C0C0800,  //  001E  CALL	R3	4
      0x70020004,  //  001F  JMP		#0025
      0x8C0C0106,  //  0020  GETMET	R3	R0	K6
      0x54160016,  //  0021  LDINT	R5	23
      0x5818003E,  //  0022  LDCONST	R6	K62
      0x581C0007,  //  0023  LDCONST	R7	K7
      0x7C0C0800,  //  0024  CALL	R3	4
      0x70020103,  //  0025  JMP		#012A
      0x1C0C0340,  //  0026  EQ	R3	R1	K64
      0x780E0019,  //  0027  JMPF	R3	#0042
      0x8C0C013B,  //  0028  GETMET	R3	R0	K59
      0x5814003A,  //  0029  LDCONST	R5	K58
      0x7C0C0400,  //  002A  CALL	R3	2
      0x780E0005,  //  002B  JMPF	R3	#0032
      0x8C0C0106,  //  002C  GETMET	R3	R0	K6
      0x54160011,  //  002D  LDINT	R5	18
      0x58180041,  //  002E  LDCONST	R6	K65
      0x581C003D,  //  002F  LDCONST	R7	K61
      0x7C0C0800,  //  0030  CALL	R3	4
      0x7002000E,  //  0031  JMP		#0041
      0x8C0C013B,  //  0032  GETMET	R3	R0	K59
      0x58140040,  //  0033  LDCONST	R5	K64
      0x7C0C0400,  //  0034  CALL	R3	2
      0x780E0005,  //  0035  JMPF	R3	#003C
      0x8C0C0106,  //  0036  GETMET	R3	R0	K6
      0x54160026,  //  0037  LDINT	R5	39
      0x58180042,  //  0038  LDCONST	R6	K66
      0x581C003D,  //  0039  LDCONST	R7	K61
      0x7C0C0800,  //  003A  CALL	R3	4
      0x70020004,  //  003B  JMP		#0041
      0x8C0C0106,  //  003C  GETMET	R3	R0	K6
      0x54160010,  //  003D  LDINT	R5	17
      0x58180040,  //  003E  LDCONST	R6	K64
      0x581C0007,  //  003F  LDCONST	R7	K7
      0x7C0C0800,  //  0040  CALL	R3	4
      0x700200E7,  //  0041  JMP		#012A
      0x1C0C0343,  //  0042  EQ	R3	R1	K67
      0x780E0019,  //  0043  JMPF	R3	#005E
      0x8C0C013B,  //  0044  GETMET	R3	R0	K59
      0x5814003A,  //  0045  LDCONST	R5	K58
      0x7C0C0400,  //  0046  CALL	R3	2
      0x780E0005,  //  0047  JMPF	R3	#004E
      0x8C0C0106,  //  0048  GETMET	R3	R0	K6
      0x54160013,  //  0049  LDINT	R5	20
      0x58180044,  //  004A  LDCONST	R6	K68
      0x581C003D,  //  004B  LDCONST	R7	K61
      0x7C0C0800,  //  004C  CALL	R3	4
      0x7002000E,  //  004D  JMP		#005D
      0x8C0C013B,  //  004E  GETMET	R3	R0	K59
      0x58140043,  //  004F  LDCONST	R5	K67
      0x7C0C0400,  //  0050  CALL	R3	2
      0x780E0005,  //  0051  JMPF	R3	#0058
      0x8C0C0106,  //  0052  GETMET	R3	R0	K6
      0x54160026,  //  0053  LDINT	R5	39
      0x58180045,  //  0054  LDCONST	R6	K69
      0x581C003D,  //  0055  LDCONST	R7	K61
      0x7C0C0800,  //  0056  CALL	R3	4
      0x70020004,  //  0057  JMP		#005D
      0x8C0C0106,  //  0058  GETMET	R3	R0	K6
      0x54160012,  //  0059  LDINT	R5	19
      0x58180043,  //  005A  LDCONST	R6	K67
      0x581C0007,  //  005B  LDCONST	R7	K7
      0x7C0C0800,  //  005C  CALL	R3	4
      0x700200CB,  //  005D  JMP		#012A
      0x1C0C0346,  //  005E  EQ	R3	R1	K70
      0x780E0012,  //  005F  JMPF	R3	#0073
      0x8C0C013B,  //  0060  GETMET	R3	R0	K59
      0x58140046,  //  0061  LDCONST	R5	K70
      0x7C0C0400,  //  0062  CALL	R3	2
      0x780E0005,  //  0063  JMPF	R3	#006A
      0x8C0C0106,  //  0064  GETMET	R3	R0	K6
      0x54160014,  //  0065  LDINT	R5	21
      0x58180047,  //  0066  LDCONST	R6	K71
      0x581C003D,  //  0067  LDCONST	R7	K61
      0x7C0C0800,  //  0068  CALL	R3	4
      0x70020007,  //  0069  JMP		#0072
      0x8C0C0124,  //  006A  GETMET	R3	R0	K36
      0x58140048,  //  006B  LDCONST	R5	K72
      0x7C0C0400,  //  006C  CALL	R3	2
      0x8C0C0106,  //  006D  GETMET	R3	R0	K6
      0x54160026,  //  006E  LDINT	R5	39
      0x58180046,  //  006F  LDCONST	R6	K70
      0x581C0007,  //  0070  LDCONST	R7	K7
      0x7C0C0800,  //  0071  CALL	R3	4
      0x700200B6,  //  0072  JMP		#012A
      0x1C0C0349,  //  0073  EQ	R3	R1	K73
      0x780E0012,  //  0074  JMPF	R3	#0088
      0x8C0C013B,  //  0075  GETMET	R3	R0	K59
      0x58140049,  //  0076  LDCONST	R5	K73
      0x7C0C0400,  //  0077  CALL	R3	2
      0x780E0005,  //  0078  JMPF	R3	#007F
      0x8C0C0106,  //  0079  GETMET	R3	R0	K6
      0x54160015,  //  007A  LDINT	R5	22
      0x5818004A,  //  007B  LDCONST	R6	K74
      0x581C003D,  //  007C  LDCONST	R7	K61
      0x7C0C0800,  //  007D  CALL	R3	4
      0x70020007,  //  007E  JMP		#0087
      0x8C0C0124,  //  007F  GETMET	R3	R0	K36
      0x5814004B,  //  0080  LDCONST	R5	K75
      0x7C0C0400,  //  0081  CALL	R3	2
      0x8C0C0106,  //  0082  GETMET	R3	R0	K6
      0x54160026,  //  0083  LDINT	R5	39
      0x58180049,  //  0084  LDCONST	R6	K73
      0x581C0007,  //  0085  LDCONST	R7	K7
      0x7C0C0800,  //  0086  CALL	R3	4
      0x700200A1,  //  0087  JMP		#012A
      0x1C0C034C,  //  0088  EQ	R3	R1	K76
      0x780E000F,  //  0089  JMPF	R3	#009A
      0x8C0C013B,  //  008A  GETMET	R3	R0	K59
      0x58140043,  //  008B  LDCONST	R5	K67
      0x7C0C0400,  //  008C  CALL	R3	2
      0x780E0005,  //  008D  JMPF	R3	#0094
      0x8C0C0106,  //  008E  GETMET	R3	R0	K6
      0x54160021,  //  008F  LDINT	R5	34
      0x5818004D,  //  0090  LDCONST	R6	K77
      0x581C003D,  //  0091  LDCONST	R7	K61
      0x7C0C0800,  //  0092  CALL	R3	4
      0x70020004,  //  0093  JMP		#0099
      0x8C0C0106,  //  0094  GETMET	R3	R0	K6
      0x54160009,  //  0095  LDINT	R5	10
      0x5818004C,  //  0096  LDCONST	R6	K76
      0x581C0007,  //  0097  LDCONST	R7	K7
      0x7C0C0800,  //  0098  CALL	R3	4
      0x7002008F,  //  0099  JMP		#012A
      0x1C0C034E,  //  009A  EQ	R3	R1	K78
      0x780E0005,  //  009B  JMPF	R3	#00A2
      0x8C0C0106,  //  009C  GETMET	R3	R0	K6
      0x54160008,  //  009D  LDINT	R5	9
      0x5818004E,  //  009E  LDCONST	R6	K78
      0x581C0007,  //  009F  LDCONST	R7	K7
      0x7C0C0800,  //  00A0  CALL	R3	4
      0x70020087,  //  00A1  JMP		#012A
      0x1C0C034F,  //  00A2  EQ	R3	R1	K79
      0x780E0005,  //  00A3  JMPF	R3	#00AA
      0x8C0C0106,  //  00A4  GETMET	R3	R0	K6
      0x5416000A,  //  00A5  LDINT	R5	11
      0x5818004F,  //  00A6  LDCONST	R6	K79
      0x581C0007,  //  00A7  LDCONST	R7	K7
      0x7C0C0800,  //  00A8  CALL	R3	4
      0x7002007F,  //  00A9  JMP		#012A
      0x1C0C0350,  //  00AA  EQ	R3	R1	K80
      0x780E0005,  //  00AB  JMPF	R3	#00B2
      0x8C0C0106,  //  00AC  GETMET	R3	R0	K6
      0x5416000B,  //  00AD  LDINT	R5	12
      0x58180050,  //  00AE  LDCONST	R6	K80
      0x581C0007,  //  00AF  LDCONST	R7	K7
      0x7C0C0800,  //  00B0  CALL	R3	4
      0x70020077,  //  00B1  JMP		#012A
      0x1C0C0351,  //  00B2  EQ	R3	R1	K81
      0x780E0005,  //  00B3  JMPF	R3	#00BA
      0x8C0C0106,  //  00B4  GETMET	R3	R0	K6
      0x5416000C,  //  00B5  LDINT	R5	13
      0x58180051,  //  00B6  LDCONST	R6	K81
      0x581C0007,  //  00B7  LDCONST	R7	K7
      0x7C0C0800,  //  00B8  CALL	R3	4
      0x7002006F,  //  00B9  JMP		#012A
      0x1C0C0352,  //  00BA  EQ	R3	R1	K82
      0x780E0005,  //  00BB  JMPF	R3	#00C2
      0x8C0C0106,  //  00BC  GETMET	R3	R0	K6
      0x5416000D,  //  00BD  LDINT	R5	14
      0x58180052,  //  00BE  LDCONST	R6	K82
      0x581C0007,  //  00BF  LDCONST	R7	K7
      0x7C0C0800,  //  00C0  CALL	R3	4
      0x70020067,  //  00C1  JMP		#012A
      0x1C0C0353,  //  00C2  EQ	R3	R1	K83
      0x780E0005,  //  00C3  JMPF	R3	#00CA
      0x8C0C0106,  //  00C4  GETMET	R3	R0	K6
      0x54160017,  //  00C5  LDINT	R5	24
      0x58180053,  //  00C6  LDCONST	R6	K83
      0x581C0007,  //  00C7  LDCONST	R7	K7
      0x7C0C0800,  //  00C8  CALL	R3	4
      0x7002005F,  //  00C9  JMP		#012A
      0x1C0C0354,  //  00CA  EQ	R3	R1	K84
      0x780E0005,  //  00CB  JMPF	R3	#00D2
      0x8C0C0106,  //  00CC  GETMET	R3	R0	K6
      0x54160018,  //  00CD  LDINT	R5	25
      0x58180054,  //  00CE  LDCONST	R6	K84
      0x581C0007,  //  00CF  LDCONST	R7	K7
      0x7C0C0800,  //  00D0  CALL	R3	4
      0x70020057,  //  00D1  JMP		#012A
      0x1C0C0355,  //  00D2  EQ	R3	R1	K85
      0x780E0005,  //  00D3  JMPF	R3	#00DA
      0x8C0C0106,  //  00D4  GETMET	R3	R0	K6
      0x54160019,  //  00D5  LDINT	R5	26
      0x58180055,  //  00D6  LDCONST	R6	K85
      0x581C0007,  //  00D7  LDCONST	R7	K7
      0x7C0C0800,  //  00D8  CALL	R3	4
      0x7002004F,  //  00D9  JMP		#012A
      0x1C0C0356,  //  00DA  EQ	R3	R1	K86
      0x780E0005,  //  00DB  JMPF	R3	#00E2
      0x8C0C0106,  //  00DC  GETMET	R3	R0	K6
      0x5416001A,  //  00DD  LDINT	R5	27
      0x58180056,  //  00DE  LDCONST	R6	K86
      0x581C0007,  //  00DF  LDCONST	R7	K7
      0x7C0C0800,  //  00E0  CALL	R3	4
      0x70020047,  //  00E1  JMP		#012A
      0x1C0C0357,  //  00E2  EQ	R3	R1	K87
      0x780E0005,  //  00E3  JMPF	R3	#00EA
      0x8C0C0106,  //  00E4  GETMET	R3	R0	K6
      0x5416001B,  //  00E5  LDINT	R5	28
      0x58180057,  //  00E6  LDCONST	R6	K87
      0x581C0007,  //  00E7  LDCONST	R7	K7
      0x7C0C0800,  //  00E8  CALL	R3	4
      0x7002003F,  //  00E9  JMP		#012A
      0x1C0C0358,  //  00EA  EQ	R3	R1	K88
      0x780E0005,  //  00EB  JMPF	R3	#00F2
      0x8C0C0106,  //  00EC  GETMET	R3	R0	K6
      0x5416001C,  //  00ED  LDINT	R5	29
      0x58180058,  //  00EE  LDCONST	R6	K88
      0x581C0007,  //  00EF  LDCONST	R7	K7
      0x7C0C0800,  //  00F0  CALL	R3	4
      0x70020037,  //  00F1  JMP		#012A
      0x1C0C0359,  //  00F2  EQ	R3	R1	K89
      0x780E0005,  //  00F3  JMPF	R3	#00FA
      0x8C0C0106,  //  00F4  GETMET	R3	R0	K6
      0x5416001D,  //  00F5  LDINT	R5	30
      0x58180059,  //  00F6  LDCONST	R6	K89
      0x581C0007,  //  00F7  LDCONST	R7	K7
      0x7C0C0800,  //  00F8  CALL	R3	4
      0x7002002F,  //  00F9  JMP		#012A
      0x1C0C035A,  //  00FA  EQ	R3	R1	K90
      0x780E0005,  //  00FB  JMPF	R3	#0102
      0x8C0C0106,  //  00FC  GETMET	R3	R0	K6
      0x5416001E,  //  00FD  LDINT	R5	31
      0x5818005A,  //  00FE  LDCONST	R6	K90
      0x581C0007,  //  00FF  LDCONST	R7	K7
      0x7C0C0800,  //  0100  CALL	R3	4
      0x70020027,  //  0101  JMP		#012A
      0x1C0C0331,  //  0102  EQ	R3	R1	K49
      0x780E0005,  //  0103  JMPF	R3	#010A
      0x8C0C0106,  //  0104  GETMET	R3	R0	K6
      0x5416001F,  //  0105  LDINT	R5	32
      0x58180031,  //  0106  LDCONST	R6	K49
      0x581C0007,  //  0107  LDCONST	R7	K7
      0x7C0C0800,  //  0108  CALL	R3	4
      0x7002001F,  //  0109  JMP		#012A
      0x1C0C035B,  //  010A  EQ	R3	R1	K91
      0x780E0014,  //  010B  JMPF	R3	#0121
      0x8C0C013B,  //  010C  GETMET	R3	R0	K59
      0x5814005B,  //  010D  LDCONST	R5	K91
      0x7C0C0400,  //  010E  CALL	R3	2
      0x780E000A,  //  010F  JMPF	R3	#011B
      0x8C0C0106,  //  0110  GETMET	R3	R0	K6
      0x54160020,  //  0111  LDINT	R5	33
      0x5818005B,  //  0112  LDCONST	R6	K91
      0x581C0007,  //  0113  LDCONST	R7	K7
      0x7C0C0800,  //  0114  CALL	R3	4
      0x8C0C0106,  //  0115  GETMET	R3	R0	K6
      0x54160020,  //  0116  LDINT	R5	33
      0x5818005B,  //  0117  LDCONST	R6	K91
      0x581C0007,  //  0118  LDCONST	R7	K7
      0x7C0C0800,  //  0119  CALL	R3	4
      0x70020004,  //  011A  JMP		#0120
      0x8C0C0106,  //  011B  GETMET	R3	R0	K6
      0x54160020,  //  011C  LDINT	R5	33
      0x5818005B,  //  011D  LDCONST	R6	K91
      0x581C0007,  //  011E  LDCONST	R7	K7
      0x7C0C0800,  //  011F  CALL	R3	4
      0x70020008,  //  0120  JMP		#012A
      0x8C0C0124,  //  0121  GETMET	R3	R0	K36
      0x0016B801,  //  0122  ADD	R5	K92	R1
      0x00140B15,  //  0123  ADD	R5	R5	K21
      0x7C0C0400,  //  0124  CALL	R3	2
      0x8C0C0106,  //  0125  GETMET	R3	R0	K6
      0x54160026,  //  0126  LDINT	R5	39
      0x5C180200,  //  0127  MOVE	R6	R1
      0x581C0007,  //  0128  LDCONST	R7	K7
      0x7C0C0800,  //  0129  CALL	R3	4
      0x80000000,  //  012A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: match
********************************************************************/
be_local_closure(class_DSLLexer_match,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(match),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C08011F,  //  0000  GETMET	R2	R0	K31
      0x7C080200,  //  0001  CALL	R2	1
      0x740A0004,  //  0002  JMPT	R2	#0008
      0x8808011D,  //  0003  GETMBR	R2	R0	K29
      0x880C011C,  //  0004  GETMBR	R3	R0	K28
      0x94080403,  //  0005  GETIDX	R2	R2	R3
      0x20080401,  //  0006  NE	R2	R2	R1
      0x780A0001,  //  0007  JMPF	R2	#000A
      0x50080000,  //  0008  LDBOOL	R2	0	0
      0x80040400,  //  0009  RET	1	R2
      0x8808011C,  //  000A  GETMBR	R2	R0	K28
      0x00080507,  //  000B  ADD	R2	R2	K7
      0x90023802,  //  000C  SETMBR	R0	K28	R2
      0x88080100,  //  000D  GETMBR	R2	R0	K0
      0x00080507,  //  000E  ADD	R2	R2	K7
      0x90020002,  //  000F  SETMBR	R0	K0	R2
      0x50080200,  //  0010  LDBOOL	R2	1	0
      0x80040400,  //  0011  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: peek_next
********************************************************************/
be_local_closure(class_DSLLexer_peek_next,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(peek_next),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x8804011C,  //  0000  GETMBR	R1	R0	K28
      0x00040307,  //  0001  ADD	R1	R1	K7
      0x6008000C,  //  0002  GETGBL	R2	G12
      0x880C011D,  //  0003  GETMBR	R3	R0	K29
      0x7C080200,  //  0004  CALL	R2	1
      0x28040202,  //  0005  GE	R1	R1	R2
      0x78060000,  //  0006  JMPF	R1	#0008
      0x80063C00,  //  0007  RET	1	K30
      0x8804011C,  //  0008  GETMBR	R1	R0	K28
      0x00040307,  //  0009  ADD	R1	R1	K7
      0x8808011D,  //  000A  GETMBR	R2	R0	K29
      0x94040401,  //  000B  GETIDX	R1	R2	R1
      0x80040200,  //  000C  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_digit
********************************************************************/
be_local_closure(class_DSLLexer_is_digit,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(is_digit),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x2808030B,  //  0000  GE	R2	R1	K11
      0x780A0001,  //  0001  JMPF	R2	#0004
      0x1808035D,  //  0002  LE	R2	R1	K93
      0x740A0000,  //  0003  JMPT	R2	#0005
      0x50080001,  //  0004  LDBOOL	R2	0	1
      0x50080200,  //  0005  LDBOOL	R2	1	0
      0x80040400,  //  0006  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tokenize_with_errors
********************************************************************/
be_local_closure(class_DSLLexer_tokenize_with_errors,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(tokenize_with_errors),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x8C04015E,  //  0000  GETMET	R1	R0	K94
      0x7C040200,  //  0001  CALL	R1	1
      0x60080013,  //  0002  GETGBL	R2	G19
      0x7C080000,  //  0003  CALL	R2	0
      0x980A5201,  //  0004  SETIDX	R2	K41	R1
      0x880C012E,  //  0005  GETMBR	R3	R0	K46
      0x980A5C03,  //  0006  SETIDX	R2	K46	R3
      0x8C0C012B,  //  0007  GETMET	R3	R0	K43
      0x7C0C0200,  //  0008  CALL	R3	1
      0x780E0000,  //  0009  JMPF	R3	#000B
      0x500C0001,  //  000A  LDBOOL	R3	0	1
      0x500C0200,  //  000B  LDBOOL	R3	1	0
      0x980ABE03,  //  000C  SETIDX	R2	K95	R3
      0x80040400,  //  000D  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: at_end
********************************************************************/
be_local_closure(class_DSLLexer_at_end,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(at_end),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8804011C,  //  0000  GETMBR	R1	R0	K28
      0x6008000C,  //  0001  GETGBL	R2	G12
      0x880C011D,  //  0002  GETMBR	R3	R0	K29
      0x7C080200,  //  0003  CALL	R2	1
      0x28040202,  //  0004  GE	R1	R1	R2
      0x80040200,  //  0005  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: reset
********************************************************************/
be_local_closure(class_DSLLexer_reset,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(reset),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x20080202,  //  0001  NE	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x5C080200,  //  0003  MOVE	R2	R1
      0x70020000,  //  0004  JMP		#0006
      0x5808001E,  //  0005  LDCONST	R2	K30
      0x90023A02,  //  0006  SETMBR	R0	K29	R2
      0x90023939,  //  0007  SETMBR	R0	K28	K57
      0x90021107,  //  0008  SETMBR	R0	K8	K7
      0x90020107,  //  0009  SETMBR	R0	K0	K7
      0x60080012,  //  000A  GETGBL	R2	G18
      0x7C080000,  //  000B  CALL	R2	0
      0x90025202,  //  000C  SETMBR	R0	K41	R2
      0x60080012,  //  000D  GETGBL	R2	G18
      0x7C080000,  //  000E  CALL	R2	0
      0x90025C02,  //  000F  SETMBR	R0	K46	R2
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: scan_variable_reference
********************************************************************/
be_local_closure(class_DSLLexer_scan_variable_reference,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(scan_variable_reference),
    &be_const_str_solidified,
    ( &(const binstruction[56]) {  /* code */
      0x8804011C,  //  0000  GETMBR	R1	R0	K28
      0x04040307,  //  0001  SUB	R1	R1	K7
      0x88080100,  //  0002  GETMBR	R2	R0	K0
      0x04080507,  //  0003  SUB	R2	R2	K7
      0x8C0C011F,  //  0004  GETMET	R3	R0	K31
      0x7C0C0200,  //  0005  CALL	R3	1
      0x740E000B,  //  0006  JMPT	R3	#0013
      0x8C0C010F,  //  0007  GETMET	R3	R0	K15
      0x8C14010C,  //  0008  GETMET	R5	R0	K12
      0x7C140200,  //  0009  CALL	R5	1
      0x7C0C0400,  //  000A  CALL	R3	2
      0x740E0004,  //  000B  JMPT	R3	#0011
      0x8C0C010C,  //  000C  GETMET	R3	R0	K12
      0x7C0C0200,  //  000D  CALL	R3	1
      0x1C0C0710,  //  000E  EQ	R3	R3	K16
      0x740E0000,  //  000F  JMPT	R3	#0011
      0x500C0001,  //  0010  LDBOOL	R3	0	1
      0x500C0200,  //  0011  LDBOOL	R3	1	0
      0x740E0008,  //  0012  JMPT	R3	#001C
      0x8C0C0124,  //  0013  GETMET	R3	R0	K36
      0x58140060,  //  0014  LDCONST	R5	K96
      0x7C0C0400,  //  0015  CALL	R3	2
      0x8C0C0106,  //  0016  GETMET	R3	R0	K6
      0x54160026,  //  0017  LDINT	R5	39
      0x58180019,  //  0018  LDCONST	R6	K25
      0x581C0007,  //  0019  LDCONST	R7	K7
      0x7C0C0800,  //  001A  CALL	R3	4
      0x80000600,  //  001B  RET	0
      0x8C0C011F,  //  001C  GETMET	R3	R0	K31
      0x7C0C0200,  //  001D  CALL	R3	1
      0x740E000B,  //  001E  JMPT	R3	#002B
      0x8C0C0161,  //  001F  GETMET	R3	R0	K97
      0x8C14010C,  //  0020  GETMET	R5	R0	K12
      0x7C140200,  //  0021  CALL	R5	1
      0x7C0C0400,  //  0022  CALL	R3	2
      0x740E0003,  //  0023  JMPT	R3	#0028
      0x8C0C010C,  //  0024  GETMET	R3	R0	K12
      0x7C0C0200,  //  0025  CALL	R3	1
      0x1C0C0710,  //  0026  EQ	R3	R3	K16
      0x780E0002,  //  0027  JMPF	R3	#002B
      0x8C0C0101,  //  0028  GETMET	R3	R0	K1
      0x7C0C0200,  //  0029  CALL	R3	1
      0x7001FFF0,  //  002A  JMP		#001C
      0x880C011C,  //  002B  GETMBR	R3	R0	K28
      0x040C0707,  //  002C  SUB	R3	R3	K7
      0x400C0203,  //  002D  CONNECT	R3	R1	R3
      0x8810011D,  //  002E  GETMBR	R4	R0	K29
      0x940C0803,  //  002F  GETIDX	R3	R4	R3
      0x8C100106,  //  0030  GETMET	R4	R0	K6
      0x541A0023,  //  0031  LDINT	R6	36
      0x5C1C0600,  //  0032  MOVE	R7	R3
      0x6020000C,  //  0033  GETGBL	R8	G12
      0x5C240600,  //  0034  MOVE	R9	R3
      0x7C200200,  //  0035  CALL	R8	1
      0x7C100800,  //  0036  CALL	R4	4
      0x80000000,  //  0037  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: check_time_suffix
********************************************************************/
be_local_closure(class_DSLLexer_check_time_suffix,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(check_time_suffix),
    &be_const_str_solidified,
    ( &(const binstruction[33]) {  /* code */
      0xA406C400,  //  0000  IMPORT	R1	K98
      0x8C08011F,  //  0001  GETMET	R2	R0	K31
      0x7C080200,  //  0002  CALL	R2	1
      0x780A0001,  //  0003  JMPF	R2	#0006
      0x50080000,  //  0004  LDBOOL	R2	0	0
      0x80040400,  //  0005  RET	1	R2
      0x8808011C,  //  0006  GETMBR	R2	R0	K28
      0x40080563,  //  0007  CONNECT	R2	R2	K99
      0x880C011D,  //  0008  GETMBR	R3	R0	K29
      0x94080602,  //  0009  GETIDX	R2	R3	R2
      0x8C0C0364,  //  000A  GETMET	R3	R1	K100
      0x5C140400,  //  000B  MOVE	R5	R2
      0x58180065,  //  000C  LDCONST	R6	K101
      0x7C0C0600,  //  000D  CALL	R3	3
      0x740E000F,  //  000E  JMPT	R3	#001F
      0x8C0C0364,  //  000F  GETMET	R3	R1	K100
      0x5C140400,  //  0010  MOVE	R5	R2
      0x58180066,  //  0011  LDCONST	R6	K102
      0x7C0C0600,  //  0012  CALL	R3	3
      0x740E000A,  //  0013  JMPT	R3	#001F
      0x8C0C0364,  //  0014  GETMET	R3	R1	K100
      0x5C140400,  //  0015  MOVE	R5	R2
      0x58180067,  //  0016  LDCONST	R6	K103
      0x7C0C0600,  //  0017  CALL	R3	3
      0x740E0005,  //  0018  JMPT	R3	#001F
      0x8C0C0364,  //  0019  GETMET	R3	R1	K100
      0x5C140400,  //  001A  MOVE	R5	R2
      0x58180068,  //  001B  LDCONST	R6	K104
      0x7C0C0600,  //  001C  CALL	R3	3
      0x740E0000,  //  001D  JMPT	R3	#001F
      0x500C0001,  //  001E  LDBOOL	R3	0	1
      0x500C0200,  //  001F  LDBOOL	R3	1	0
      0x80040600,  //  0020  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: scan_number
********************************************************************/
be_local_closure(class_DSLLexer_scan_number,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(scan_number),
    &be_const_str_solidified,
    ( &(const binstruction[113]) {  /* code */
      0x8804011C,  //  0000  GETMBR	R1	R0	K28
      0x04040307,  //  0001  SUB	R1	R1	K7
      0x88080100,  //  0002  GETMBR	R2	R0	K0
      0x04080507,  //  0003  SUB	R2	R2	K7
      0x500C0000,  //  0004  LDBOOL	R3	0	0
      0x8C10011F,  //  0005  GETMET	R4	R0	K31
      0x7C100200,  //  0006  CALL	R4	1
      0x74120007,  //  0007  JMPT	R4	#0010
      0x8C100112,  //  0008  GETMET	R4	R0	K18
      0x8C18010C,  //  0009  GETMET	R6	R0	K12
      0x7C180200,  //  000A  CALL	R6	1
      0x7C100400,  //  000B  CALL	R4	2
      0x78120002,  //  000C  JMPF	R4	#0010
      0x8C100101,  //  000D  GETMET	R4	R0	K1
      0x7C100200,  //  000E  CALL	R4	1
      0x7001FFF4,  //  000F  JMP		#0005
      0x8C10011F,  //  0010  GETMET	R4	R0	K31
      0x7C100200,  //  0011  CALL	R4	1
      0x7412001F,  //  0012  JMPT	R4	#0033
      0x8C10010C,  //  0013  GETMET	R4	R0	K12
      0x7C100200,  //  0014  CALL	R4	1
      0x1C10095B,  //  0015  EQ	R4	R4	K91
      0x7812001B,  //  0016  JMPF	R4	#0033
      0x8810011C,  //  0017  GETMBR	R4	R0	K28
      0x00100907,  //  0018  ADD	R4	R4	K7
      0x6014000C,  //  0019  GETGBL	R5	G12
      0x8818011D,  //  001A  GETMBR	R6	R0	K29
      0x7C140200,  //  001B  CALL	R5	1
      0x14100805,  //  001C  LT	R4	R4	R5
      0x78120014,  //  001D  JMPF	R4	#0033
      0x8C100112,  //  001E  GETMET	R4	R0	K18
      0x8818011C,  //  001F  GETMBR	R6	R0	K28
      0x00180D07,  //  0020  ADD	R6	R6	K7
      0x881C011D,  //  0021  GETMBR	R7	R0	K29
      0x94180E06,  //  0022  GETIDX	R6	R7	R6
      0x7C100400,  //  0023  CALL	R4	2
      0x7812000D,  //  0024  JMPF	R4	#0033
      0x500C0200,  //  0025  LDBOOL	R3	1	0
      0x8C100101,  //  0026  GETMET	R4	R0	K1
      0x7C100200,  //  0027  CALL	R4	1
      0x8C10011F,  //  0028  GETMET	R4	R0	K31
      0x7C100200,  //  0029  CALL	R4	1
      0x74120007,  //  002A  JMPT	R4	#0033
      0x8C100112,  //  002B  GETMET	R4	R0	K18
      0x8C18010C,  //  002C  GETMET	R6	R0	K12
      0x7C180200,  //  002D  CALL	R6	1
      0x7C100400,  //  002E  CALL	R4	2
      0x78120002,  //  002F  JMPF	R4	#0033
      0x8C100101,  //  0030  GETMET	R4	R0	K1
      0x7C100200,  //  0031  CALL	R4	1
      0x7001FFF4,  //  0032  JMP		#0028
      0x8810011C,  //  0033  GETMBR	R4	R0	K28
      0x04100907,  //  0034  SUB	R4	R4	K7
      0x40100204,  //  0035  CONNECT	R4	R1	R4
      0x8814011D,  //  0036  GETMBR	R5	R0	K29
      0x94100A04,  //  0037  GETIDX	R4	R5	R4
      0x8C140169,  //  0038  GETMET	R5	R0	K105
      0x7C140200,  //  0039  CALL	R5	1
      0x78160009,  //  003A  JMPF	R5	#0045
      0x8C14016A,  //  003B  GETMET	R5	R0	K106
      0x7C140200,  //  003C  CALL	R5	1
      0x8C180106,  //  003D  GETMET	R6	R0	K6
      0x54220004,  //  003E  LDINT	R8	5
      0x00240805,  //  003F  ADD	R9	R4	R5
      0x6028000C,  //  0040  GETGBL	R10	G12
      0x002C0805,  //  0041  ADD	R11	R4	R5
      0x7C280200,  //  0042  CALL	R10	1
      0x7C180800,  //  0043  CALL	R6	4
      0x7002002A,  //  0044  JMP		#0070
      0x8C14011F,  //  0045  GETMET	R5	R0	K31
      0x7C140200,  //  0046  CALL	R5	1
      0x7416000E,  //  0047  JMPT	R5	#0057
      0x8C14010C,  //  0048  GETMET	R5	R0	K12
      0x7C140200,  //  0049  CALL	R5	1
      0x1C140B51,  //  004A  EQ	R5	R5	K81
      0x7816000A,  //  004B  JMPF	R5	#0057
      0x8C140101,  //  004C  GETMET	R5	R0	K1
      0x7C140200,  //  004D  CALL	R5	1
      0x8C140106,  //  004E  GETMET	R5	R0	K6
      0x541E0005,  //  004F  LDINT	R7	6
      0x00200951,  //  0050  ADD	R8	R4	K81
      0x6024000C,  //  0051  GETGBL	R9	G12
      0x5C280800,  //  0052  MOVE	R10	R4
      0x7C240200,  //  0053  CALL	R9	1
      0x00241307,  //  0054  ADD	R9	R9	K7
      0x7C140800,  //  0055  CALL	R5	4
      0x70020018,  //  0056  JMP		#0070
      0x8C14011F,  //  0057  GETMET	R5	R0	K31
      0x7C140200,  //  0058  CALL	R5	1
      0x7416000E,  //  0059  JMPT	R5	#0069
      0x8C14010C,  //  005A  GETMET	R5	R0	K12
      0x7C140200,  //  005B  CALL	R5	1
      0x1C140B0D,  //  005C  EQ	R5	R5	K13
      0x7816000A,  //  005D  JMPF	R5	#0069
      0x8C140101,  //  005E  GETMET	R5	R0	K1
      0x7C140200,  //  005F  CALL	R5	1
      0x8C140106,  //  0060  GETMET	R5	R0	K6
      0x541E0006,  //  0061  LDINT	R7	7
      0x0020090D,  //  0062  ADD	R8	R4	K13
      0x6024000C,  //  0063  GETGBL	R9	G12
      0x5C280800,  //  0064  MOVE	R10	R4
      0x7C240200,  //  0065  CALL	R9	1
      0x00241307,  //  0066  ADD	R9	R9	K7
      0x7C140800,  //  0067  CALL	R5	4
      0x70020006,  //  0068  JMP		#0070
      0x8C140106,  //  0069  GETMET	R5	R0	K6
      0x581C003D,  //  006A  LDCONST	R7	K61
      0x5C200800,  //  006B  MOVE	R8	R4
      0x6024000C,  //  006C  GETGBL	R9	G12
      0x5C280800,  //  006D  MOVE	R10	R4
      0x7C240200,  //  006E  CALL	R9	1
      0x7C140800,  //  006F  CALL	R5	4
      0x80000000,  //  0070  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_alnum
********************************************************************/
be_local_closure(class_DSLLexer_is_alnum,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(is_alnum),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x8C08010F,  //  0000  GETMET	R2	R0	K15
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x740A0004,  //  0003  JMPT	R2	#0009
      0x8C080112,  //  0004  GETMET	R2	R0	K18
      0x5C100200,  //  0005  MOVE	R4	R1
      0x7C080400,  //  0006  CALL	R2	2
      0x740A0000,  //  0007  JMPT	R2	#0009
      0x50080001,  //  0008  LDBOOL	R2	0	1
      0x50080200,  //  0009  LDBOOL	R2	1	0
      0x80040400,  //  000A  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: has_errors
********************************************************************/
be_local_closure(class_DSLLexer_has_errors,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(has_errors),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x6004000C,  //  0000  GETGBL	R1	G12
      0x8808012E,  //  0001  GETMBR	R2	R0	K46
      0x7C040200,  //  0002  CALL	R1	1
      0x24040339,  //  0003  GT	R1	R1	K57
      0x80040200,  //  0004  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: scan_identifier_or_keyword
********************************************************************/
be_local_closure(class_DSLLexer_scan_identifier_or_keyword,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(scan_identifier_or_keyword),
    &be_const_str_solidified,
    ( &(const binstruction[48]) {  /* code */
      0x8804011C,  //  0000  GETMBR	R1	R0	K28
      0x04040307,  //  0001  SUB	R1	R1	K7
      0x88080100,  //  0002  GETMBR	R2	R0	K0
      0x04080507,  //  0003  SUB	R2	R2	K7
      0x8C0C011F,  //  0004  GETMET	R3	R0	K31
      0x7C0C0200,  //  0005  CALL	R3	1
      0x740E000B,  //  0006  JMPT	R3	#0013
      0x8C0C0161,  //  0007  GETMET	R3	R0	K97
      0x8C14010C,  //  0008  GETMET	R5	R0	K12
      0x7C140200,  //  0009  CALL	R5	1
      0x7C0C0400,  //  000A  CALL	R3	2
      0x740E0003,  //  000B  JMPT	R3	#0010
      0x8C0C010C,  //  000C  GETMET	R3	R0	K12
      0x7C0C0200,  //  000D  CALL	R3	1
      0x1C0C0710,  //  000E  EQ	R3	R3	K16
      0x780E0002,  //  000F  JMPF	R3	#0013
      0x8C0C0101,  //  0010  GETMET	R3	R0	K1
      0x7C0C0200,  //  0011  CALL	R3	1
      0x7001FFF0,  //  0012  JMP		#0004
      0x880C011C,  //  0013  GETMBR	R3	R0	K28
      0x040C0707,  //  0014  SUB	R3	R3	K7
      0x400C0203,  //  0015  CONNECT	R3	R1	R3
      0x8810011D,  //  0016  GETMBR	R4	R0	K29
      0x940C0803,  //  0017  GETIDX	R3	R4	R3
      0x4C100000,  //  0018  LDNIL	R4
      0xB8164E00,  //  0019  GETNGBL	R5	K39
      0x8C140B6B,  //  001A  GETMET	R5	R5	K107
      0x5C1C0600,  //  001B  MOVE	R7	R3
      0x7C140400,  //  001C  CALL	R5	2
      0x78160001,  //  001D  JMPF	R5	#0020
      0x54120003,  //  001E  LDINT	R4	4
      0x70020007,  //  001F  JMP		#0028
      0xB8164E00,  //  0020  GETNGBL	R5	K39
      0x8C140B6C,  //  0021  GETMET	R5	R5	K108
      0x5C1C0600,  //  0022  MOVE	R7	R3
      0x7C140400,  //  0023  CALL	R5	2
      0x78160001,  //  0024  JMPF	R5	#0027
      0x58100039,  //  0025  LDCONST	R4	K57
      0x70020000,  //  0026  JMP		#0028
      0x58100007,  //  0027  LDCONST	R4	K7
      0x8C140106,  //  0028  GETMET	R5	R0	K6
      0x5C1C0800,  //  0029  MOVE	R7	R4
      0x5C200600,  //  002A  MOVE	R8	R3
      0x6024000C,  //  002B  GETGBL	R9	G12
      0x5C280600,  //  002C  MOVE	R10	R3
      0x7C240200,  //  002D  CALL	R9	1
      0x7C140800,  //  002E  CALL	R5	4
      0x80000000,  //  002F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: scan_hex_color_0x
********************************************************************/
be_local_closure(class_DSLLexer_scan_hex_color_0x,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(scan_hex_color_0x),
    &be_const_str_solidified,
    ( &(const binstruction[50]) {  /* code */
      0x8804011C,  //  0000  GETMBR	R1	R0	K28
      0x04040307,  //  0001  SUB	R1	R1	K7
      0x88080100,  //  0002  GETMBR	R2	R0	K0
      0x04080507,  //  0003  SUB	R2	R2	K7
      0x8C0C0101,  //  0004  GETMET	R3	R0	K1
      0x7C0C0200,  //  0005  CALL	R3	1
      0x580C0039,  //  0006  LDCONST	R3	K57
      0x8C10011F,  //  0007  GETMET	R4	R0	K31
      0x7C100200,  //  0008  CALL	R4	1
      0x74120008,  //  0009  JMPT	R4	#0013
      0x8C10016D,  //  000A  GETMET	R4	R0	K109
      0x8C18010C,  //  000B  GETMET	R6	R0	K12
      0x7C180200,  //  000C  CALL	R6	1
      0x7C100400,  //  000D  CALL	R4	2
      0x78120003,  //  000E  JMPF	R4	#0013
      0x8C100101,  //  000F  GETMET	R4	R0	K1
      0x7C100200,  //  0010  CALL	R4	1
      0x000C0707,  //  0011  ADD	R3	R3	K7
      0x7001FFF3,  //  0012  JMP		#0007
      0x8810011C,  //  0013  GETMBR	R4	R0	K28
      0x04100907,  //  0014  SUB	R4	R4	K7
      0x40100204,  //  0015  CONNECT	R4	R1	R4
      0x8814011D,  //  0016  GETMBR	R5	R0	K29
      0x94100A04,  //  0017  GETIDX	R4	R5	R4
      0x54160005,  //  0018  LDINT	R5	6
      0x1C140605,  //  0019  EQ	R5	R3	R5
      0x74160002,  //  001A  JMPT	R5	#001E
      0x54160007,  //  001B  LDINT	R5	8
      0x1C140605,  //  001C  EQ	R5	R3	R5
      0x78160007,  //  001D  JMPF	R5	#0026
      0x8C140106,  //  001E  GETMET	R5	R0	K6
      0x541E0003,  //  001F  LDINT	R7	4
      0x5C200800,  //  0020  MOVE	R8	R4
      0x6024000C,  //  0021  GETGBL	R9	G12
      0x5C280800,  //  0022  MOVE	R10	R4
      0x7C240200,  //  0023  CALL	R9	1
      0x7C140800,  //  0024  CALL	R5	4
      0x7002000A,  //  0025  JMP		#0031
      0x8C140124,  //  0026  GETMET	R5	R0	K36
      0x001EDC04,  //  0027  ADD	R7	K110	R4
      0x001C0F6F,  //  0028  ADD	R7	R7	K111
      0x7C140400,  //  0029  CALL	R5	2
      0x8C140106,  //  002A  GETMET	R5	R0	K6
      0x541E0026,  //  002B  LDINT	R7	39
      0x5C200800,  //  002C  MOVE	R8	R4
      0x6024000C,  //  002D  GETGBL	R9	G12
      0x5C280800,  //  002E  MOVE	R10	R4
      0x7C240200,  //  002F  CALL	R9	1
      0x7C140800,  //  0030  CALL	R5	4
      0x80000000,  //  0031  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: scan_time_suffix
********************************************************************/
be_local_closure(class_DSLLexer_scan_time_suffix,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(scan_time_suffix),
    &be_const_str_solidified,
    ( &(const binstruction[39]) {  /* code */
      0xA406C400,  //  0000  IMPORT	R1	K98
      0x8C080364,  //  0001  GETMET	R2	R1	K100
      0x8810011C,  //  0002  GETMBR	R4	R0	K28
      0x40100963,  //  0003  CONNECT	R4	R4	K99
      0x8814011D,  //  0004  GETMBR	R5	R0	K29
      0x94100A04,  //  0005  GETIDX	R4	R5	R4
      0x58140065,  //  0006  LDCONST	R5	K101
      0x7C080600,  //  0007  CALL	R2	3
      0x780A0005,  //  0008  JMPF	R2	#000F
      0x8C080101,  //  0009  GETMET	R2	R0	K1
      0x7C080200,  //  000A  CALL	R2	1
      0x8C080101,  //  000B  GETMET	R2	R0	K1
      0x7C080200,  //  000C  CALL	R2	1
      0x8006CA00,  //  000D  RET	1	K101
      0x70020016,  //  000E  JMP		#0026
      0x8C08010C,  //  000F  GETMET	R2	R0	K12
      0x7C080200,  //  0010  CALL	R2	1
      0x1C080566,  //  0011  EQ	R2	R2	K102
      0x780A0003,  //  0012  JMPF	R2	#0017
      0x8C080101,  //  0013  GETMET	R2	R0	K1
      0x7C080200,  //  0014  CALL	R2	1
      0x8006CC00,  //  0015  RET	1	K102
      0x7002000E,  //  0016  JMP		#0026
      0x8C08010C,  //  0017  GETMET	R2	R0	K12
      0x7C080200,  //  0018  CALL	R2	1
      0x1C080567,  //  0019  EQ	R2	R2	K103
      0x780A0003,  //  001A  JMPF	R2	#001F
      0x8C080101,  //  001B  GETMET	R2	R0	K1
      0x7C080200,  //  001C  CALL	R2	1
      0x8006CE00,  //  001D  RET	1	K103
      0x70020006,  //  001E  JMP		#0026
      0x8C08010C,  //  001F  GETMET	R2	R0	K12
      0x7C080200,  //  0020  CALL	R2	1
      0x1C080568,  //  0021  EQ	R2	R2	K104
      0x780A0002,  //  0022  JMPF	R2	#0026
      0x8C080101,  //  0023  GETMET	R2	R0	K1
      0x7C080200,  //  0024  CALL	R2	1
      0x8006D000,  //  0025  RET	1	K104
      0x80063C00,  //  0026  RET	1	K30
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_position_info
********************************************************************/
be_local_closure(class_DSLLexer_get_position_info,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(get_position_info),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x60040013,  //  0000  GETGBL	R1	G19
      0x7C040000,  //  0001  CALL	R1	0
      0x8808011C,  //  0002  GETMBR	R2	R0	K28
      0x98063802,  //  0003  SETIDX	R1	K28	R2
      0x88080108,  //  0004  GETMBR	R2	R0	K8
      0x98061002,  //  0005  SETIDX	R1	K8	R2
      0x88080100,  //  0006  GETMBR	R2	R0	K0
      0x98060002,  //  0007  SETIDX	R1	K0	R2
      0x8C08011F,  //  0008  GETMET	R2	R0	K31
      0x7C080200,  //  0009  CALL	R2	1
      0x98063E02,  //  000A  SETIDX	R1	K31	R2
      0x80040200,  //  000B  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_alpha
********************************************************************/
be_local_closure(class_DSLLexer_is_alpha,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(is_alpha),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x28080335,  //  0000  GE	R2	R1	K53
      0x780A0001,  //  0001  JMPF	R2	#0004
      0x18080370,  //  0002  LE	R2	R1	K112
      0x740A0004,  //  0003  JMPT	R2	#0009
      0x28080337,  //  0004  GE	R2	R1	K55
      0x780A0001,  //  0005  JMPF	R2	#0008
      0x18080371,  //  0006  LE	R2	R1	K113
      0x740A0000,  //  0007  JMPT	R2	#0009
      0x50080001,  //  0008  LDBOOL	R2	0	1
      0x50080200,  //  0009  LDBOOL	R2	1	0
      0x80040400,  //  000A  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: scan_triple_quoted_string
********************************************************************/
be_local_closure(class_DSLLexer_scan_triple_quoted_string,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(scan_triple_quoted_string),
    &be_const_str_solidified,
    ( &(const binstruction[75]) {  /* code */
      0x8808011C,  //  0000  GETMBR	R2	R0	K28
      0x04080507,  //  0001  SUB	R2	R2	K7
      0x880C0100,  //  0002  GETMBR	R3	R0	K0
      0x040C0707,  //  0003  SUB	R3	R3	K7
      0x5810001E,  //  0004  LDCONST	R4	K30
      0x8C140101,  //  0005  GETMET	R5	R0	K1
      0x7C140200,  //  0006  CALL	R5	1
      0x8C140101,  //  0007  GETMET	R5	R0	K1
      0x7C140200,  //  0008  CALL	R5	1
      0x8C14011F,  //  0009  GETMET	R5	R0	K31
      0x7C140200,  //  000A  CALL	R5	1
      0x7416001F,  //  000B  JMPT	R5	#002C
      0x8C14010C,  //  000C  GETMET	R5	R0	K12
      0x7C140200,  //  000D  CALL	R5	1
      0x1C180A01,  //  000E  EQ	R6	R5	R1
      0x781A0010,  //  000F  JMPF	R6	#0021
      0x8C180116,  //  0010  GETMET	R6	R0	K22
      0x58200007,  //  0011  LDCONST	R8	K7
      0x7C180400,  //  0012  CALL	R6	2
      0x1C180C01,  //  0013  EQ	R6	R6	R1
      0x781A000B,  //  0014  JMPF	R6	#0021
      0x8C180116,  //  0015  GETMET	R6	R0	K22
      0x5820003D,  //  0016  LDCONST	R8	K61
      0x7C180400,  //  0017  CALL	R6	2
      0x1C180C01,  //  0018  EQ	R6	R6	R1
      0x781A0006,  //  0019  JMPF	R6	#0021
      0x8C180101,  //  001A  GETMET	R6	R0	K1
      0x7C180200,  //  001B  CALL	R6	1
      0x8C180101,  //  001C  GETMET	R6	R0	K1
      0x7C180200,  //  001D  CALL	R6	1
      0x8C180101,  //  001E  GETMET	R6	R0	K1
      0x7C180200,  //  001F  CALL	R6	1
      0x7002000A,  //  0020  JMP		#002C
      0x8C180101,  //  0021  GETMET	R6	R0	K1
      0x7C180200,  //  0022  CALL	R6	1
      0x5C140C00,  //  0023  MOVE	R5	R6
      0x1C180B05,  //  0024  EQ	R6	R5	K5
      0x781A0003,  //  0025  JMPF	R6	#002A
      0x88180108,  //  0026  GETMBR	R6	R0	K8
      0x00180D07,  //  0027  ADD	R6	R6	K7
      0x90021006,  //  0028  SETMBR	R0	K8	R6
      0x90020107,  //  0029  SETMBR	R0	K0	K7
      0x00100805,  //  002A  ADD	R4	R4	R5
      0x7001FFDC,  //  002B  JMP		#0009
      0x8C14011F,  //  002C  GETMET	R5	R0	K31
      0x7C140200,  //  002D  CALL	R5	1
      0x78160014,  //  002E  JMPF	R5	#0044
      0x8814011C,  //  002F  GETMBR	R5	R0	K28
      0x04140B26,  //  0030  SUB	R5	R5	K38
      0x8818011C,  //  0031  GETMBR	R6	R0	K28
      0x04180D07,  //  0032  SUB	R6	R6	K7
      0x40140A06,  //  0033  CONNECT	R5	R5	R6
      0x8818011D,  //  0034  GETMBR	R6	R0	K29
      0x94140C05,  //  0035  GETIDX	R5	R6	R5
      0x00180201,  //  0036  ADD	R6	R1	R1
      0x00180C01,  //  0037  ADD	R6	R6	R1
      0x1C140A06,  //  0038  EQ	R5	R5	R6
      0x74160009,  //  0039  JMPT	R5	#0044
      0x8C140124,  //  003A  GETMET	R5	R0	K36
      0x581C0072,  //  003B  LDCONST	R7	K114
      0x7C140400,  //  003C  CALL	R5	2
      0x8C140106,  //  003D  GETMET	R5	R0	K6
      0x541E0026,  //  003E  LDINT	R7	39
      0x5C200800,  //  003F  MOVE	R8	R4
      0x8824011C,  //  0040  GETMBR	R9	R0	K28
      0x04241202,  //  0041  SUB	R9	R9	R2
      0x7C140800,  //  0042  CALL	R5	4
      0x70020005,  //  0043  JMP		#004A
      0x8C140106,  //  0044  GETMET	R5	R0	K6
      0x581C0026,  //  0045  LDCONST	R7	K38
      0x5C200800,  //  0046  MOVE	R8	R4
      0x8824011C,  //  0047  GETMBR	R9	R0	K28
      0x04241202,  //  0048  SUB	R9	R9	R2
      0x7C140800,  //  0049  CALL	R5	4
      0x80000000,  //  004A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_error
********************************************************************/
be_local_closure(class_DSLLexer_add_error,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(add_error),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x8808012E,  //  0000  GETMBR	R2	R0	K46
      0x8C08052A,  //  0001  GETMET	R2	R2	K42
      0x60100013,  //  0002  GETGBL	R4	G19
      0x7C100000,  //  0003  CALL	R4	0
      0x98126601,  //  0004  SETIDX	R4	K51	R1
      0x88140108,  //  0005  GETMBR	R5	R0	K8
      0x98121005,  //  0006  SETIDX	R4	K8	R5
      0x88140100,  //  0007  GETMBR	R5	R0	K0
      0x98120005,  //  0008  SETIDX	R4	K0	R5
      0x8814011C,  //  0009  GETMBR	R5	R0	K28
      0x98123805,  //  000A  SETIDX	R4	K28	R5
      0x7C080400,  //  000B  CALL	R2	2
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: scan_comment
********************************************************************/
be_local_closure(class_DSLLexer_scan_comment,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(scan_comment),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x8804011C,  //  0000  GETMBR	R1	R0	K28
      0x04040307,  //  0001  SUB	R1	R1	K7
      0x88080100,  //  0002  GETMBR	R2	R0	K0
      0x04080507,  //  0003  SUB	R2	R2	K7
      0x8C0C011F,  //  0004  GETMET	R3	R0	K31
      0x7C0C0200,  //  0005  CALL	R3	1
      0x740E0006,  //  0006  JMPT	R3	#000E
      0x8C0C010C,  //  0007  GETMET	R3	R0	K12
      0x7C0C0200,  //  0008  CALL	R3	1
      0x200C0705,  //  0009  NE	R3	R3	K5
      0x780E0002,  //  000A  JMPF	R3	#000E
      0x8C0C0101,  //  000B  GETMET	R3	R0	K1
      0x7C0C0200,  //  000C  CALL	R3	1
      0x7001FFF5,  //  000D  JMP		#0004
      0x880C011C,  //  000E  GETMBR	R3	R0	K28
      0x040C0707,  //  000F  SUB	R3	R3	K7
      0x400C0203,  //  0010  CONNECT	R3	R1	R3
      0x8810011D,  //  0011  GETMBR	R4	R0	K29
      0x940C0803,  //  0012  GETIDX	R3	R4	R3
      0x8C100106,  //  0013  GETMET	R4	R0	K6
      0x541A0024,  //  0014  LDINT	R6	37
      0x5C1C0600,  //  0015  MOVE	R7	R3
      0x8820011C,  //  0016  GETMBR	R8	R0	K28
      0x04201001,  //  0017  SUB	R8	R8	R1
      0x7C100800,  //  0018  CALL	R4	4
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tokenize
********************************************************************/
be_local_closure(class_DSLLexer_tokenize,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(tokenize),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x60040012,  //  0000  GETGBL	R1	G18
      0x7C040000,  //  0001  CALL	R1	0
      0x90025201,  //  0002  SETMBR	R0	K41	R1
      0x60040012,  //  0003  GETGBL	R1	G18
      0x7C040000,  //  0004  CALL	R1	0
      0x90025C01,  //  0005  SETMBR	R0	K46	R1
      0x90023939,  //  0006  SETMBR	R0	K28	K57
      0x90021107,  //  0007  SETMBR	R0	K8	K7
      0x90020107,  //  0008  SETMBR	R0	K0	K7
      0x8C04011F,  //  0009  GETMET	R1	R0	K31
      0x7C040200,  //  000A  CALL	R1	1
      0x74060002,  //  000B  JMPT	R1	#000F
      0x8C040173,  //  000C  GETMET	R1	R0	K115
      0x7C040200,  //  000D  CALL	R1	1
      0x7001FFF9,  //  000E  JMP		#0009
      0x8C040106,  //  000F  GETMET	R1	R0	K6
      0x540E0025,  //  0010  LDINT	R3	38
      0x5810001E,  //  0011  LDCONST	R4	K30
      0x58140039,  //  0012  LDCONST	R5	K57
      0x7C040800,  //  0013  CALL	R1	4
      0x88040129,  //  0014  GETMBR	R1	R0	K41
      0x80040200,  //  0015  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_errors
********************************************************************/
be_local_closure(class_DSLLexer_get_errors,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(get_errors),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x8804012E,  //  0000  GETMBR	R1	R0	K46
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: DSLLexer
********************************************************************/
be_local_class(DSLLexer,
    6,
    NULL,
    be_nested_map(37,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(scan_token, 36), be_const_closure(class_DSLLexer_scan_token_closure) },
        { be_const_key_weak(get_errors, 11), be_const_closure(class_DSLLexer_get_errors_closure) },
        { be_const_key_weak(scan_string, -1), be_const_closure(class_DSLLexer_scan_string_closure) },
        { be_const_key_weak(add_token, 14), be_const_closure(class_DSLLexer_add_token_closure) },
        { be_const_key_weak(tokenize, -1), be_const_closure(class_DSLLexer_tokenize_closure) },
        { be_const_key_weak(peek, -1), be_const_closure(class_DSLLexer_peek_closure) },
        { be_const_key_weak(source, -1), be_const_var(0) },
        { be_const_key_weak(get_error_report, -1), be_const_closure(class_DSLLexer_get_error_report_closure) },
        { be_const_key_weak(advance, -1), be_const_closure(class_DSLLexer_advance_closure) },
        { be_const_key_weak(scan_operator_or_delimiter, -1), be_const_closure(class_DSLLexer_scan_operator_or_delimiter_closure) },
        { be_const_key_weak(add_error, -1), be_const_closure(class_DSLLexer_add_error_closure) },
        { be_const_key_weak(scan_triple_quoted_string, 24), be_const_closure(class_DSLLexer_scan_triple_quoted_string_closure) },
        { be_const_key_weak(scan_number, -1), be_const_closure(class_DSLLexer_scan_number_closure) },
        { be_const_key_weak(peek_ahead, 9), be_const_closure(class_DSLLexer_peek_ahead_closure) },
        { be_const_key_weak(is_alnum, 32), be_const_closure(class_DSLLexer_is_alnum_closure) },
        { be_const_key_weak(peek_next, -1), be_const_closure(class_DSLLexer_peek_next_closure) },
        { be_const_key_weak(is_digit, -1), be_const_closure(class_DSLLexer_is_digit_closure) },
        { be_const_key_weak(tokenize_with_errors, -1), be_const_closure(class_DSLLexer_tokenize_with_errors_closure) },
        { be_const_key_weak(at_end, -1), be_const_closure(class_DSLLexer_at_end_closure) },
        { be_const_key_weak(reset, -1), be_const_closure(class_DSLLexer_reset_closure) },
        { be_const_key_weak(scan_variable_reference, -1), be_const_closure(class_DSLLexer_scan_variable_reference_closure) },
        { be_const_key_weak(column, -1), be_const_var(3) },
        { be_const_key_weak(scan_identifier_or_keyword, -1), be_const_closure(class_DSLLexer_scan_identifier_or_keyword_closure) },
        { be_const_key_weak(tokens, 12), be_const_var(4) },
        { be_const_key_weak(scan_hex_color_0x, -1), be_const_closure(class_DSLLexer_scan_hex_color_0x_closure) },
        { be_const_key_weak(has_errors, -1), be_const_closure(class_DSLLexer_has_errors_closure) },
        { be_const_key_weak(errors, 22), be_const_var(5) },
        { be_const_key_weak(check_time_suffix, 1), be_const_closure(class_DSLLexer_check_time_suffix_closure) },
        { be_const_key_weak(line, -1), be_const_var(2) },
        { be_const_key_weak(scan_time_suffix, -1), be_const_closure(class_DSLLexer_scan_time_suffix_closure) },
        { be_const_key_weak(get_position_info, -1), be_const_closure(class_DSLLexer_get_position_info_closure) },
        { be_const_key_weak(is_alpha, -1), be_const_closure(class_DSLLexer_is_alpha_closure) },
        { be_const_key_weak(match, -1), be_const_closure(class_DSLLexer_match_closure) },
        { be_const_key_weak(position, 10), be_const_var(1) },
        { be_const_key_weak(scan_comment, -1), be_const_closure(class_DSLLexer_scan_comment_closure) },
        { be_const_key_weak(is_hex_digit, 4), be_const_closure(class_DSLLexer_is_hex_digit_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_DSLLexer_init_closure) },
    })),
    be_str_weak(DSLLexer)
);

/********************************************************************
** Solidified function: create_newline_token
********************************************************************/
be_local_closure(create_newline_token,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation_dsl),
    /* K1   */  be_nested_str_weak(Token),
    /* K2   */  be_nested_str_weak(_X0A),
    /* K3   */  be_const_int(1),
    }),
    be_str_weak(create_newline_token),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x54120022,  //  0002  LDINT	R4	35
      0x58140002,  //  0003  LDCONST	R5	K2
      0x5C180000,  //  0004  MOVE	R6	R0
      0x5C1C0200,  //  0005  MOVE	R7	R1
      0x58200003,  //  0006  LDCONST	R8	K3
      0x7C080C00,  //  0007  CALL	R2	6
      0x80040400,  //  0008  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified module: animation_dsl
********************************************************************/
be_local_module(animation_dsl,
    "animation_dsl",
    be_nested_map(24,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(DSLRuntime, -1), be_const_class(be_class_DSLRuntime) },
        { be_const_key_weak(create_newline_token, -1), be_const_closure(create_newline_token_closure) },
        { be_const_key_weak(SimpleDSLTranspiler, -1), be_const_class(be_class_SimpleDSLTranspiler) },
        { be_const_key_weak(tokenize_dsl_with_errors, -1), be_const_closure(tokenize_dsl_with_errors_closure) },
        { be_const_key_weak(is_keyword, -1), be_const_closure(is_keyword_closure) },
        { be_const_key_weak(_symbol_entry, 4), be_const_class(be_class_SymbolEntry) },
        { be_const_key_weak(compile, -1), be_const_closure(compile_dsl_source_closure) },
        { be_const_key_weak(VERSION, -1), be_const_int(65536) },
        { be_const_key_weak(get_operator_precedence, -1), be_const_closure(get_operator_precedence_closure) },
        { be_const_key_weak(create_eof_token, 2), be_const_closure(create_eof_token_closure) },
        { be_const_key_weak(create_dsl_runtime, 11), be_const_closure(create_dsl_runtime_closure) },
        { be_const_key_weak(Token, 23), be_const_class(be_class_Token) },
        { be_const_key_weak(is_right_associative, 1), be_const_closure(is_right_associative_closure) },
        { be_const_key_weak(_symbol_table, 8), be_const_class(be_class_SymbolTable) },
        { be_const_key_weak(create_runtime, 9), be_const_closure(create_runtime_closure) },
        { be_const_key_weak(tokenize_dsl, -1), be_const_closure(tokenize_dsl_closure) },
        { be_const_key_weak(execute, 15), be_const_closure(execute_closure) },
        { be_const_key_weak(is_color_name, -1), be_const_closure(is_color_name_closure) },
        { be_const_key_weak(create_error_token, -1), be_const_closure(create_error_token_closure) },
        { be_const_key_weak(named_colors, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(37,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(brown, -1), be_nested_str_weak(0xFFA52A2A) },
        { be_const_key_weak(silver, -1), be_nested_str_weak(0xFFC0C0C0) },
        { be_const_key_weak(salmon, -1), be_nested_str_weak(0xFFFA8072) },
        { be_const_key_weak(transparent, -1), be_nested_str_weak(0x00000000) },
        { be_const_key_weak(lime, 19), be_nested_str_weak(0xFF00FF00) },
        { be_const_key_weak(coral, 26), be_nested_str_weak(0xFFFF7F50) },
        { be_const_key_weak(cyan, -1), be_nested_str_weak(0xFF00FFFF) },
        { be_const_key_weak(olive, -1), be_nested_str_weak(0xFF808000) },
        { be_const_key_weak(snow, 16), be_nested_str_weak(0xFFFFFAFA) },
        { be_const_key_weak(violet, -1), be_nested_str_weak(0xFFEE82EE) },
        { be_const_key_weak(green, 20), be_nested_str_weak(0xFF008000) },
        { be_const_key_weak(turquoise, -1), be_nested_str_weak(0xFF40E0D0) },
        { be_const_key_weak(grey, -1), be_nested_str_weak(0xFF808080) },
        { be_const_key_weak(indigo, -1), be_nested_str_weak(0xFF4B0082) },
        { be_const_key_weak(navy, 23), be_nested_str_weak(0xFF000080) },
        { be_const_key_weak(white, 30), be_nested_str_weak(0xFFFFFFFF) },
        { be_const_key_weak(red, -1), be_nested_str_weak(0xFFFF0000) },
        { be_const_key_weak(orange, -1), be_nested_str_weak(0xFFFFA500) },
        { be_const_key_weak(gray, -1), be_nested_str_weak(0xFF808080) },
        { be_const_key_weak(purple, -1), be_nested_str_weak(0xFF800080) },
        { be_const_key_weak(beige, -1), be_nested_str_weak(0xFFF5F5DC) },
        { be_const_key_weak(fuchsia, -1), be_nested_str_weak(0xFFFF00FF) },
        { be_const_key_weak(crimson, 12), be_nested_str_weak(0xFFDC143C) },
        { be_const_key_weak(black, 18), be_nested_str_weak(0xFF000000) },
        { be_const_key_weak(magenta, 2), be_nested_str_weak(0xFFFF00FF) },
        { be_const_key_weak(yellow, 15), be_nested_str_weak(0xFFFFFF00) },
        { be_const_key_weak(tan, 27), be_nested_str_weak(0xFFD2B48C) },
        { be_const_key_weak(gold, -1), be_nested_str_weak(0xFFFFD700) },
        { be_const_key_weak(pink, -1), be_nested_str_weak(0xFFFFC0CB) },
        { be_const_key_weak(ivory, -1), be_nested_str_weak(0xFFFFFFF0) },
        { be_const_key_weak(khaki, 35), be_nested_str_weak(0xFFF0E68C) },
        { be_const_key_weak(aqua, -1), be_nested_str_weak(0xFF00FFFF) },
        { be_const_key_weak(blue, -1), be_nested_str_weak(0xFF0000FF) },
        { be_const_key_weak(plum, 21), be_nested_str_weak(0xFFDDA0DD) },
        { be_const_key_weak(orchid, -1), be_nested_str_weak(0xFFDA70D6) },
        { be_const_key_weak(teal, -1), be_nested_str_weak(0xFF008080) },
        { be_const_key_weak(maroon, -1), be_nested_str_weak(0xFF800000) },
    }))    ) } )) },
        { be_const_key_weak(load_file, -1), be_const_closure(load_file_closure) },
        { be_const_key_weak(MockEngine, 3), be_const_class(be_class_MockEngine) },
        { be_const_key_weak(DSLLexer, -1), be_const_class(be_class_DSLLexer) },
        { be_const_key_weak(compile_dsl, -1), be_const_closure(compile_dsl_closure) },
    }))
);
BE_EXPORT_VARIABLE be_define_const_native_module(animation_dsl);
/********************************************************************/
/********************************************************************/
/* End of solidification */
