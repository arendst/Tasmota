/* Solidification of animation_dsl.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
extern const bclass be_class_ExpressionResult;
// compact class 'ExpressionResult' ktab size: 11, total: 28 (saved 136 bytes)
static const bvalue be_ktab_class_ExpressionResult[11] = {
  /* K0   */  be_nested_str_weak(has_dynamic),
  /* K1   */  be_nested_str_weak(expr),
  /* K2   */  be_nested_str_weak(),
  /* K3   */  be_nested_str_weak(has_dangerous),
  /* K4   */  be_nested_str_weak(has_computation),
  /* K5   */  be_nested_str_weak(return_type),
  /* K6   */  be_nested_str_weak(instance_for_validation),
  /* K7   */  be_const_class(be_class_ExpressionResult),
  /* K8   */  be_nested_str_weak(instance_X3D_X25s),
  /* K9   */  be_nested_str_weak(instance_X3Dnil),
  /* K10  */  be_nested_str_weak(ExpressionResult_X28expr_X3D_X27_X25s_X27_X2C_X20dynamic_X3D_X25s_X2C_X20dangerous_X3D_X25s_X2C_X20comp_X3D_X25s_X2C_X20type_X3D_X25s_X2C_X20_X25s_X29),
};


extern const bclass be_class_ExpressionResult;

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
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
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
be_local_closure(class_ExpressionResult_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    7,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_ExpressionResult,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x4C1C0000,  //  0000  LDNIL	R7
      0x201C0207,  //  0001  NE	R7	R1	R7
      0x781E0001,  //  0002  JMPF	R7	#0005
      0x5C1C0200,  //  0003  MOVE	R7	R1
      0x70020000,  //  0004  JMP		#0006
      0x581C0002,  //  0005  LDCONST	R7	K2
      0x90020207,  //  0006  SETMBR	R0	K1	R7
      0x601C0017,  //  0007  GETGBL	R7	G23
      0x5C200400,  //  0008  MOVE	R8	R2
      0x7C1C0200,  //  0009  CALL	R7	1
      0x90020007,  //  000A  SETMBR	R0	K0	R7
      0x601C0017,  //  000B  GETGBL	R7	G23
      0x5C200600,  //  000C  MOVE	R8	R3
      0x7C1C0200,  //  000D  CALL	R7	1
      0x90020607,  //  000E  SETMBR	R0	K3	R7
      0x601C0017,  //  000F  GETGBL	R7	G23
      0x5C200800,  //  0010  MOVE	R8	R4
      0x7C1C0200,  //  0011  CALL	R7	1
      0x90020807,  //  0012  SETMBR	R0	K4	R7
      0x4C1C0000,  //  0013  LDNIL	R7
      0x201C0A07,  //  0014  NE	R7	R5	R7
      0x781E0001,  //  0015  JMPF	R7	#0018
      0x5C1C0A00,  //  0016  MOVE	R7	R5
      0x70020000,  //  0017  JMP		#0019
      0x541E000B,  //  0018  LDINT	R7	12
      0x90020A07,  //  0019  SETMBR	R0	K5	R7
      0x90020C06,  //  001A  SETMBR	R0	K6	R6
      0x80000000,  //  001B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: literal
********************************************************************/
be_local_closure(class_ExpressionResult_literal,   /* name */
  be_nested_proto(
    11,                          /* nstack */
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
    ( &(const binstruction[10]) {  /* code */
      0x580C0007,  //  0000  LDCONST	R3	K7
      0x5C100600,  //  0001  MOVE	R4	R3
      0x5C140000,  //  0002  MOVE	R5	R0
      0x50180000,  //  0003  LDBOOL	R6	0	0
      0x501C0000,  //  0004  LDBOOL	R7	0	0
      0x50200000,  //  0005  LDBOOL	R8	0	0
      0x5C240200,  //  0006  MOVE	R9	R1
      0x5C280400,  //  0007  MOVE	R10	R2
      0x7C100C00,  //  0008  CALL	R4	6
      0x80040800,  //  0009  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_ExpressionResult_tostring,   /* name */
  be_nested_proto(
    10,                          /* nstack */
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
    ( &(const binstruction[22]) {  /* code */
      0x88040106,  //  0000  GETMBR	R1	R0	K6
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060006,  //  0003  JMPF	R1	#000B
      0x60040018,  //  0004  GETGBL	R1	G24
      0x58080008,  //  0005  LDCONST	R2	K8
      0x600C0005,  //  0006  GETGBL	R3	G5
      0x88100106,  //  0007  GETMBR	R4	R0	K6
      0x7C0C0200,  //  0008  CALL	R3	1
      0x7C040400,  //  0009  CALL	R1	2
      0x70020000,  //  000A  JMP		#000C
      0x58040009,  //  000B  LDCONST	R1	K9
      0x60080018,  //  000C  GETGBL	R2	G24
      0x580C000A,  //  000D  LDCONST	R3	K10
      0x88100101,  //  000E  GETMBR	R4	R0	K1
      0x88140100,  //  000F  GETMBR	R5	R0	K0
      0x88180103,  //  0010  GETMBR	R6	R0	K3
      0x881C0104,  //  0011  GETMBR	R7	R0	K4
      0x88200105,  //  0012  GETMBR	R8	R0	K5
      0x5C240200,  //  0013  MOVE	R9	R1
      0x7C080E00,  //  0014  CALL	R2	7
      0x80040400,  //  0015  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: function_call
********************************************************************/
be_local_closure(class_ExpressionResult_function_call,   /* name */
  be_nested_proto(
    11,                          /* nstack */
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
    ( &(const binstruction[10]) {  /* code */
      0x580C0007,  //  0000  LDCONST	R3	K7
      0x5C100600,  //  0001  MOVE	R4	R3
      0x5C140000,  //  0002  MOVE	R5	R0
      0x50180200,  //  0003  LDBOOL	R6	1	0
      0x501C0000,  //  0004  LDBOOL	R7	0	0
      0x50200000,  //  0005  LDBOOL	R8	0	0
      0x5C240200,  //  0006  MOVE	R9	R1
      0x5C280400,  //  0007  MOVE	R10	R2
      0x7C100C00,  //  0008  CALL	R4	6
      0x80040800,  //  0009  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: combine
********************************************************************/
be_local_closure(class_ExpressionResult_combine,   /* name */
  be_nested_proto(
    15,                          /* nstack */
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
    ( &(const binstruction[91]) {  /* code */
      0x580C0007,  //  0000  LDCONST	R3	K7
      0x50100000,  //  0001  LDBOOL	R4	0	0
      0x50140000,  //  0002  LDBOOL	R5	0	0
      0x50180200,  //  0003  LDBOOL	R6	1	0
      0x541E000B,  //  0004  LDINT	R7	12
      0x4C200000,  //  0005  LDNIL	R8
      0x20200208,  //  0006  NE	R8	R1	R8
      0x78220011,  //  0007  JMPF	R8	#001A
      0x74120002,  //  0008  JMPT	R4	#000C
      0x88200300,  //  0009  GETMBR	R8	R1	K0
      0x74220000,  //  000A  JMPT	R8	#000C
      0x50200001,  //  000B  LDBOOL	R8	0	1
      0x50200200,  //  000C  LDBOOL	R8	1	0
      0x5C101000,  //  000D  MOVE	R4	R8
      0x74160002,  //  000E  JMPT	R5	#0012
      0x88200303,  //  000F  GETMBR	R8	R1	K3
      0x74220000,  //  0010  JMPT	R8	#0012
      0x50200001,  //  0011  LDBOOL	R8	0	1
      0x50200200,  //  0012  LDBOOL	R8	1	0
      0x5C141000,  //  0013  MOVE	R5	R8
      0x741A0002,  //  0014  JMPT	R6	#0018
      0x88200304,  //  0015  GETMBR	R8	R1	K4
      0x74220000,  //  0016  JMPT	R8	#0018
      0x50200001,  //  0017  LDBOOL	R8	0	1
      0x50200200,  //  0018  LDBOOL	R8	1	0
      0x5C181000,  //  0019  MOVE	R6	R8
      0x4C200000,  //  001A  LDNIL	R8
      0x20200408,  //  001B  NE	R8	R2	R8
      0x78220011,  //  001C  JMPF	R8	#002F
      0x74120002,  //  001D  JMPT	R4	#0021
      0x88200500,  //  001E  GETMBR	R8	R2	K0
      0x74220000,  //  001F  JMPT	R8	#0021
      0x50200001,  //  0020  LDBOOL	R8	0	1
      0x50200200,  //  0021  LDBOOL	R8	1	0
      0x5C101000,  //  0022  MOVE	R4	R8
      0x74160002,  //  0023  JMPT	R5	#0027
      0x88200503,  //  0024  GETMBR	R8	R2	K3
      0x74220000,  //  0025  JMPT	R8	#0027
      0x50200001,  //  0026  LDBOOL	R8	0	1
      0x50200200,  //  0027  LDBOOL	R8	1	0
      0x5C141000,  //  0028  MOVE	R5	R8
      0x741A0002,  //  0029  JMPT	R6	#002D
      0x88200504,  //  002A  GETMBR	R8	R2	K4
      0x74220000,  //  002B  JMPT	R8	#002D
      0x50200001,  //  002C  LDBOOL	R8	0	1
      0x50200200,  //  002D  LDBOOL	R8	1	0
      0x5C181000,  //  002E  MOVE	R6	R8
      0x4C200000,  //  002F  LDNIL	R8
      0x20200208,  //  0030  NE	R8	R1	R8
      0x7822000E,  //  0031  JMPF	R8	#0041
      0x4C200000,  //  0032  LDNIL	R8
      0x20200408,  //  0033  NE	R8	R2	R8
      0x7822000B,  //  0034  JMPF	R8	#0041
      0x88200305,  //  0035  GETMBR	R8	R1	K5
      0x88240505,  //  0036  GETMBR	R9	R2	K5
      0x1C201009,  //  0037  EQ	R8	R8	R9
      0x78220005,  //  0038  JMPF	R8	#003F
      0x88200305,  //  0039  GETMBR	R8	R1	K5
      0x5426000B,  //  003A  LDINT	R9	12
      0x20201009,  //  003B  NE	R8	R8	R9
      0x78220001,  //  003C  JMPF	R8	#003F
      0x881C0305,  //  003D  GETMBR	R7	R1	K5
      0x70020000,  //  003E  JMP		#0040
      0x541E000B,  //  003F  LDINT	R7	12
      0x70020010,  //  0040  JMP		#0052
      0x4C200000,  //  0041  LDNIL	R8
      0x20200208,  //  0042  NE	R8	R1	R8
      0x78220005,  //  0043  JMPF	R8	#004A
      0x781A0001,  //  0044  JMPF	R6	#0047
      0x5422000B,  //  0045  LDINT	R8	12
      0x70020000,  //  0046  JMP		#0048
      0x88200305,  //  0047  GETMBR	R8	R1	K5
      0x5C1C1000,  //  0048  MOVE	R7	R8
      0x70020007,  //  0049  JMP		#0052
      0x4C200000,  //  004A  LDNIL	R8
      0x20200408,  //  004B  NE	R8	R2	R8
      0x78220004,  //  004C  JMPF	R8	#0052
      0x781A0001,  //  004D  JMPF	R6	#0050
      0x5422000B,  //  004E  LDINT	R8	12
      0x70020000,  //  004F  JMP		#0051
      0x88200505,  //  0050  GETMBR	R8	R2	K5
      0x5C1C1000,  //  0051  MOVE	R7	R8
      0x5C200600,  //  0052  MOVE	R8	R3
      0x5C240000,  //  0053  MOVE	R9	R0
      0x5C280800,  //  0054  MOVE	R10	R4
      0x5C2C0A00,  //  0055  MOVE	R11	R5
      0x5C300C00,  //  0056  MOVE	R12	R6
      0x5C340E00,  //  0057  MOVE	R13	R7
      0x4C380000,  //  0058  LDNIL	R14
      0x7C200C00,  //  0059  CALL	R8	6
      0x80041000,  //  005A  RET	1	R8
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: property_access
********************************************************************/
be_local_closure(class_ExpressionResult_property_access,   /* name */
  be_nested_proto(
    11,                          /* nstack */
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
    ( &(const binstruction[10]) {  /* code */
      0x580C0007,  //  0000  LDCONST	R3	K7
      0x5C100600,  //  0001  MOVE	R4	R3
      0x5C140000,  //  0002  MOVE	R5	R0
      0x50180200,  //  0003  LDBOOL	R6	1	0
      0x501C0000,  //  0004  LDBOOL	R7	0	0
      0x50200000,  //  0005  LDBOOL	R8	0	0
      0x5C240200,  //  0006  MOVE	R9	R1
      0x5C280400,  //  0007  MOVE	R10	R2
      0x7C100C00,  //  0008  CALL	R4	6
      0x80040800,  //  0009  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: constructor_call
********************************************************************/
be_local_closure(class_ExpressionResult_constructor_call,   /* name */
  be_nested_proto(
    11,                          /* nstack */
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
    ( &(const binstruction[10]) {  /* code */
      0x580C0007,  //  0000  LDCONST	R3	K7
      0x5C100600,  //  0001  MOVE	R4	R3
      0x5C140000,  //  0002  MOVE	R5	R0
      0x50180000,  //  0003  LDBOOL	R6	0	0
      0x501C0200,  //  0004  LDBOOL	R7	1	0
      0x50200000,  //  0005  LDBOOL	R8	0	0
      0x5C240200,  //  0006  MOVE	R9	R1
      0x5C280400,  //  0007  MOVE	R10	R2
      0x7C100C00,  //  0008  CALL	R4	6
      0x80040800,  //  0009  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: variable_ref
********************************************************************/
be_local_closure(class_ExpressionResult_variable_ref,   /* name */
  be_nested_proto(
    11,                          /* nstack */
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
    ( &(const binstruction[10]) {  /* code */
      0x580C0007,  //  0000  LDCONST	R3	K7
      0x5C100600,  //  0001  MOVE	R4	R3
      0x5C140000,  //  0002  MOVE	R5	R0
      0x50180200,  //  0003  LDBOOL	R6	1	0
      0x501C0000,  //  0004  LDBOOL	R7	0	0
      0x50200000,  //  0005  LDBOOL	R8	0	0
      0x5C240200,  //  0006  MOVE	R9	R1
      0x5C280400,  //  0007  MOVE	R10	R2
      0x7C100C00,  //  0008  CALL	R4	6
      0x80040800,  //  0009  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: ExpressionResult
********************************************************************/
be_local_class(ExpressionResult,
    6,
    NULL,
    be_nested_map(16,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(needs_closure, 7), be_const_closure(class_ExpressionResult_needs_closure_closure) },
        { be_const_key_weak(variable_ref, -1), be_const_static_closure(class_ExpressionResult_variable_ref_closure) },
        { be_const_key_weak(tostring, -1), be_const_closure(class_ExpressionResult_tostring_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_ExpressionResult_init_closure) },
        { be_const_key_weak(literal, -1), be_const_static_closure(class_ExpressionResult_literal_closure) },
        { be_const_key_weak(has_dynamic, 2), be_const_var(1) },
        { be_const_key_weak(function_call, -1), be_const_static_closure(class_ExpressionResult_function_call_closure) },
        { be_const_key_weak(constructor_call, -1), be_const_static_closure(class_ExpressionResult_constructor_call_closure) },
        { be_const_key_weak(instance_for_validation, 13), be_const_var(5) },
        { be_const_key_weak(property_access, -1), be_const_static_closure(class_ExpressionResult_property_access_closure) },
        { be_const_key_weak(combine, -1), be_const_static_closure(class_ExpressionResult_combine_closure) },
        { be_const_key_weak(has_computation, 9), be_const_var(3) },
        { be_const_key_weak(has_dangerous, -1), be_const_var(2) },
        { be_const_key_weak(return_type, -1), be_const_var(4) },
        { be_const_key_weak(expr, -1), be_const_var(0) },
        { be_const_key_weak(needs_function, 1), be_const_closure(class_ExpressionResult_needs_function_closure) },
    })),
    be_str_weak(ExpressionResult)
);
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
// compact class 'Token' ktab size: 13, total: 17 (saved 32 bytes)
static const bvalue be_ktab_class_Token[13] = {
  /* K0   */  be_nested_str_weak(UNKNOWN),
  /* K1   */  be_nested_str_weak(type),
  /* K2   */  be_const_int(0),
  /* K3   */  be_nested_str_weak(names),
  /* K4   */  be_nested_str_weak(Token_X28_X25s_X20at_X20_X25s_X3A_X25s_X29),
  /* K5   */  be_nested_str_weak(line),
  /* K6   */  be_nested_str_weak(column),
  /* K7   */  be_nested_str_weak(value),
  /* K8   */  be_nested_str_weak(_X2E_X2E_X2E),
  /* K9   */  be_nested_str_weak(Token_X28_X25s_X2C_X20_X27_X25s_X27_X20at_X20_X25s_X3A_X25s_X29),
  /* K10  */  be_nested_str_weak(),
  /* K11  */  be_const_int(1),
  /* K12  */  be_nested_str_weak(length),
};


extern const bclass be_class_Token;

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
    ( &(const binstruction[54]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x28080502,  //  0002  GE	R2	R2	K2
      0x780A0008,  //  0003  JMPF	R2	#000D
      0x88080101,  //  0004  GETMBR	R2	R0	K1
      0x600C000C,  //  0005  GETGBL	R3	G12
      0x88100103,  //  0006  GETMBR	R4	R0	K3
      0x7C0C0200,  //  0007  CALL	R3	1
      0x14080403,  //  0008  LT	R2	R2	R3
      0x780A0002,  //  0009  JMPF	R2	#000D
      0x88080103,  //  000A  GETMBR	R2	R0	K3
      0x880C0101,  //  000B  GETMBR	R3	R0	K1
      0x94040403,  //  000C  GETIDX	R1	R2	R3
      0x88080101,  //  000D  GETMBR	R2	R0	K1
      0x540E0022,  //  000E  LDINT	R3	35
      0x1C080403,  //  000F  EQ	R2	R2	R3
      0x780A0007,  //  0010  JMPF	R2	#0019
      0x60080018,  //  0011  GETGBL	R2	G24
      0x580C0004,  //  0012  LDCONST	R3	K4
      0x5C100200,  //  0013  MOVE	R4	R1
      0x88140105,  //  0014  GETMBR	R5	R0	K5
      0x88180106,  //  0015  GETMBR	R6	R0	K6
      0x7C080800,  //  0016  CALL	R2	4
      0x80040400,  //  0017  RET	1	R2
      0x7002001B,  //  0018  JMP		#0035
      0x6008000C,  //  0019  GETGBL	R2	G12
      0x880C0107,  //  001A  GETMBR	R3	R0	K7
      0x7C080200,  //  001B  CALL	R2	1
      0x540E0013,  //  001C  LDINT	R3	20
      0x24080403,  //  001D  GT	R2	R2	R3
      0x780A000D,  //  001E  JMPF	R2	#002D
      0x540A0010,  //  001F  LDINT	R2	17
      0x400A0402,  //  0020  CONNECT	R2	K2	R2
      0x880C0107,  //  0021  GETMBR	R3	R0	K7
      0x94080602,  //  0022  GETIDX	R2	R3	R2
      0x00080508,  //  0023  ADD	R2	R2	K8
      0x600C0018,  //  0024  GETGBL	R3	G24
      0x58100009,  //  0025  LDCONST	R4	K9
      0x5C140200,  //  0026  MOVE	R5	R1
      0x5C180400,  //  0027  MOVE	R6	R2
      0x881C0105,  //  0028  GETMBR	R7	R0	K5
      0x88200106,  //  0029  GETMBR	R8	R0	K6
      0x7C0C0A00,  //  002A  CALL	R3	5
      0x80040600,  //  002B  RET	1	R3
      0x70020007,  //  002C  JMP		#0035
      0x60080018,  //  002D  GETGBL	R2	G24
      0x580C0009,  //  002E  LDCONST	R3	K9
      0x5C100200,  //  002F  MOVE	R4	R1
      0x88140107,  //  0030  GETMBR	R5	R0	K7
      0x88180105,  //  0031  GETMBR	R6	R0	K5
      0x881C0106,  //  0032  GETMBR	R7	R0	K6
      0x7C080A00,  //  0033  CALL	R2	5
      0x80040400,  //  0034  RET	1	R2
      0x80000000,  //  0035  RET	0
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
      0x90020201,  //  0000  SETMBR	R0	K1	R1
      0x4C180000,  //  0001  LDNIL	R6
      0x20180406,  //  0002  NE	R6	R2	R6
      0x781A0001,  //  0003  JMPF	R6	#0006
      0x5C180400,  //  0004  MOVE	R6	R2
      0x70020000,  //  0005  JMP		#0007
      0x5818000A,  //  0006  LDCONST	R6	K10
      0x90020E06,  //  0007  SETMBR	R0	K7	R6
      0x4C180000,  //  0008  LDNIL	R6
      0x20180606,  //  0009  NE	R6	R3	R6
      0x781A0001,  //  000A  JMPF	R6	#000D
      0x5C180600,  //  000B  MOVE	R6	R3
      0x70020000,  //  000C  JMP		#000E
      0x5818000B,  //  000D  LDCONST	R6	K11
      0x90020A06,  //  000E  SETMBR	R0	K5	R6
      0x4C180000,  //  000F  LDNIL	R6
      0x20180806,  //  0010  NE	R6	R4	R6
      0x781A0001,  //  0011  JMPF	R6	#0014
      0x5C180800,  //  0012  MOVE	R6	R4
      0x70020000,  //  0013  JMP		#0015
      0x5818000B,  //  0014  LDCONST	R6	K11
      0x90020C06,  //  0015  SETMBR	R0	K6	R6
      0x4C180000,  //  0016  LDNIL	R6
      0x20180A06,  //  0017  NE	R6	R5	R6
      0x781A0001,  //  0018  JMPF	R6	#001B
      0x5C180A00,  //  0019  MOVE	R6	R5
      0x70020002,  //  001A  JMP		#001E
      0x6018000C,  //  001B  GETGBL	R6	G12
      0x881C0107,  //  001C  GETMBR	R7	R0	K7
      0x7C180200,  //  001D  CALL	R6	1
      0x90021806,  //  001E  SETMBR	R0	K12	R6
      0x80000000,  //  001F  RET	0
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
    be_nested_map(11,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(value, -1), be_const_var(1) },
        { be_const_key_weak(column, -1), be_const_var(3) },
        { be_const_key_weak(line, -1), be_const_var(2) },
        { be_const_key_weak(tostring, -1), be_const_closure(class_Token_tostring_closure) },
        { be_const_key_weak(length, -1), be_const_var(4) },
        { be_const_key_weak(type, 10), be_const_var(0) },
        { be_const_key_weak(color_names, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
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
        be_nested_str_weak(),
        be_nested_str_weak(ERROR),
        be_nested_str_weak(EVENT_ON),
        be_nested_str_weak(EVENT_INTERRUPT),
        be_nested_str_weak(EVENT_RESUME),
        be_nested_str_weak(EVENT_AFTER),
    }))    ) } )) },
        { be_const_key_weak(init, 0), be_const_closure(class_Token_init_closure) },
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
    })),
    be_str_weak(Token)
);

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
** Solidified function: compile_dsl_source
********************************************************************/
be_local_closure(compile_dsl_source,   /* name */
  be_nested_proto(
    5,                          /* nstack */
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
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x5C100000,  //  0002  MOVE	R4	R0
      0x7C080400,  //  0003  CALL	R2	2
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: execute
********************************************************************/
be_local_closure(execute,   /* name */
  be_nested_proto(
    5,                          /* nstack */
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
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x5C100000,  //  0002  MOVE	R4	R0
      0x7C080400,  //  0003  CALL	R2	2
      0x600C000D,  //  0004  GETGBL	R3	G13
      0x5C100400,  //  0005  MOVE	R4	R2
      0x7C0C0200,  //  0006  CALL	R3	1
      0x5C100600,  //  0007  MOVE	R4	R3
      0x7C100000,  //  0008  CALL	R4	0
      0x80040800,  //  0009  RET	1	R4
    })
  )
);
/*******************************************************************/

// compact class 'SymbolTable' ktab size: 55, total: 113 (saved 464 bytes)
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
  /* K34  */  be_nested_str_weak(0x_X2508X),
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
    9,                          /* nstack */
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
      0xA40E0400,  //  0000  IMPORT	R3	K2
      0x88100703,  //  0001  GETMBR	R4	R3	K3
      0x8C100904,  //  0002  GETMET	R4	R4	K4
      0x5C180200,  //  0003  MOVE	R6	R1
      0x5C1C0400,  //  0004  MOVE	R7	R2
      0x50200000,  //  0005  LDBOOL	R8	0	0
      0x7C100800,  //  0006  CALL	R4	4
      0x8C140105,  //  0007  GETMET	R5	R0	K5
      0x5C1C0200,  //  0008  MOVE	R7	R1
      0x5C200800,  //  0009  MOVE	R8	R4
      0x7C140600,  //  000A  CALL	R5	3
      0x80040A00,  //  000B  RET	1	R5
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
    12,                          /* nstack */
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
    ( &(const binstruction[179]) {  /* code */
      0xA40A0400,  //  0000  IMPORT	R2	K2
      0x880C0107,  //  0001  GETMBR	R3	R0	K7
      0x8C0C0708,  //  0002  GETMET	R3	R3	K8
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C0C0400,  //  0004  CALL	R3	2
      0x780E0002,  //  0005  JMPF	R3	#0009
      0x880C0107,  //  0006  GETMBR	R3	R0	K7
      0x940C0601,  //  0007  GETIDX	R3	R3	R1
      0x80040600,  //  0008  RET	1	R3
      0xA80200A1,  //  0009  EXBLK	0	#00AC
      0xA40E1200,  //  000A  IMPORT	R3	K9
      0x8810050A,  //  000B  GETMBR	R4	R2	K10
      0x8C100908,  //  000C  GETMET	R4	R4	K8
      0x5C180200,  //  000D  MOVE	R6	R1
      0x7C100400,  //  000E  CALL	R4	2
      0x78120009,  //  000F  JMPF	R4	#001A
      0x88100503,  //  0010  GETMBR	R4	R2	K3
      0x8C10090B,  //  0011  GETMET	R4	R4	K11
      0x5C180200,  //  0012  MOVE	R6	R1
      0x4C1C0000,  //  0013  LDNIL	R7
      0x50200200,  //  0014  LDBOOL	R8	1	0
      0x7C100800,  //  0015  CALL	R4	4
      0x88140107,  //  0016  GETMBR	R5	R0	K7
      0x98140204,  //  0017  SETIDX	R5	R1	R4
      0xA8040001,  //  0018  EXBLK	1	1
      0x80040800,  //  0019  RET	1	R4
      0x1C10030C,  //  001A  EQ	R4	R1	K12
      0x78120008,  //  001B  JMPF	R4	#0025
      0x88100503,  //  001C  GETMBR	R4	R2	K3
      0x8C10090D,  //  001D  GETMET	R4	R4	K13
      0x5818000C,  //  001E  LDCONST	R6	K12
      0x501C0200,  //  001F  LDBOOL	R7	1	0
      0x7C100600,  //  0020  CALL	R4	3
      0x88140107,  //  0021  GETMBR	R5	R0	K7
      0x98140204,  //  0022  SETIDX	R5	R1	R4
      0xA8040001,  //  0023  EXBLK	1	1
      0x80040800,  //  0024  RET	1	R4
      0xB8121C00,  //  0025  GETNGBL	R4	K14
      0x8C10090F,  //  0026  GETMET	R4	R4	K15
      0x5C180200,  //  0027  MOVE	R6	R1
      0x7C100400,  //  0028  CALL	R4	2
      0x78120008,  //  0029  JMPF	R4	#0033
      0x88100503,  //  002A  GETMBR	R4	R2	K3
      0x8C10090D,  //  002B  GETMET	R4	R4	K13
      0x5C180200,  //  002C  MOVE	R6	R1
      0x501C0200,  //  002D  LDBOOL	R7	1	0
      0x7C100600,  //  002E  CALL	R4	3
      0x88140107,  //  002F  GETMBR	R5	R0	K7
      0x98140204,  //  0030  SETIDX	R5	R1	R4
      0xA8040001,  //  0031  EXBLK	1	1
      0x80040800,  //  0032  RET	1	R4
      0x8C100708,  //  0033  GETMET	R4	R3	K8
      0xB81A1C00,  //  0034  GETNGBL	R6	K14
      0x88180D10,  //  0035  GETMBR	R6	R6	K16
      0x5C1C0200,  //  0036  MOVE	R7	R1
      0x7C100600,  //  0037  CALL	R4	3
      0x78120008,  //  0038  JMPF	R4	#0042
      0x88100503,  //  0039  GETMBR	R4	R2	K3
      0x8C100911,  //  003A  GETMET	R4	R4	K17
      0x5C180200,  //  003B  MOVE	R6	R1
      0x501C0200,  //  003C  LDBOOL	R7	1	0
      0x7C100600,  //  003D  CALL	R4	3
      0x88140107,  //  003E  GETMBR	R5	R0	K7
      0x98140204,  //  003F  SETIDX	R5	R1	R4
      0xA8040001,  //  0040  EXBLK	1	1
      0x80040800,  //  0041  RET	1	R4
      0x8C100708,  //  0042  GETMET	R4	R3	K8
      0xB81A1C00,  //  0043  GETNGBL	R6	K14
      0x5C1C0200,  //  0044  MOVE	R7	R1
      0x7C100600,  //  0045  CALL	R4	3
      0x7812005F,  //  0046  JMPF	R4	#00A7
      0xB8121C00,  //  0047  GETNGBL	R4	K14
      0x88100801,  //  0048  GETMBR	R4	R4	R1
      0x60140004,  //  0049  GETGBL	R5	G4
      0x5C180800,  //  004A  MOVE	R6	R4
      0x7C140200,  //  004B  CALL	R5	1
      0x6018000F,  //  004C  GETGBL	R6	G15
      0x5C1C0800,  //  004D  MOVE	R7	R4
      0x60200015,  //  004E  GETGBL	R8	G21
      0x7C180400,  //  004F  CALL	R6	2
      0x781A0009,  //  0050  JMPF	R6	#005B
      0x88180503,  //  0051  GETMBR	R6	R2	K3
      0x8C180D12,  //  0052  GETMET	R6	R6	K18
      0x5C200200,  //  0053  MOVE	R8	R1
      0x5C240800,  //  0054  MOVE	R9	R4
      0x50280200,  //  0055  LDBOOL	R10	1	0
      0x7C180800,  //  0056  CALL	R6	4
      0x881C0107,  //  0057  GETMBR	R7	R0	K7
      0x981C0206,  //  0058  SETIDX	R7	R1	R6
      0xA8040001,  //  0059  EXBLK	1	1
      0x80040C00,  //  005A  RET	1	R6
      0x1C180B13,  //  005B  EQ	R6	R5	K19
      0x781A0009,  //  005C  JMPF	R6	#0067
      0x88180503,  //  005D  GETMBR	R6	R2	K3
      0x8C180D14,  //  005E  GETMET	R6	R6	K20
      0x5C200200,  //  005F  MOVE	R8	R1
      0x5C240800,  //  0060  MOVE	R9	R4
      0x50280200,  //  0061  LDBOOL	R10	1	0
      0x7C180800,  //  0062  CALL	R6	4
      0x881C0107,  //  0063  GETMBR	R7	R0	K7
      0x981C0206,  //  0064  SETIDX	R7	R1	R6
      0xA8040001,  //  0065  EXBLK	1	1
      0x80040C00,  //  0066  RET	1	R6
      0x1C180B15,  //  0067  EQ	R6	R5	K21
      0x741A0001,  //  0068  JMPT	R6	#006B
      0x1C180B16,  //  0069  EQ	R6	R5	K22
      0x781A003B,  //  006A  JMPF	R6	#00A7
      0xA8020036,  //  006B  EXBLK	0	#00A3
      0x5C180800,  //  006C  MOVE	R6	R4
      0x881C0117,  //  006D  GETMBR	R7	R0	K23
      0x7C180200,  //  006E  CALL	R6	1
      0x601C000F,  //  006F  GETGBL	R7	G15
      0x5C200C00,  //  0070  MOVE	R8	R6
      0xB8261C00,  //  0071  GETNGBL	R9	K14
      0x88241318,  //  0072  GETMBR	R9	R9	K24
      0x7C1C0400,  //  0073  CALL	R7	2
      0x781E000A,  //  0074  JMPF	R7	#0080
      0x881C0503,  //  0075  GETMBR	R7	R2	K3
      0x8C1C0F19,  //  0076  GETMET	R7	R7	K25
      0x5C240200,  //  0077  MOVE	R9	R1
      0x5C280C00,  //  0078  MOVE	R10	R6
      0x502C0200,  //  0079  LDBOOL	R11	1	0
      0x7C1C0800,  //  007A  CALL	R7	4
      0x88200107,  //  007B  GETMBR	R8	R0	K7
      0x98200207,  //  007C  SETIDX	R8	R1	R7
      0xA8040002,  //  007D  EXBLK	1	2
      0x80040E00,  //  007E  RET	1	R7
      0x70020020,  //  007F  JMP		#00A1
      0x601C000F,  //  0080  GETGBL	R7	G15
      0x5C200C00,  //  0081  MOVE	R8	R6
      0xB8261C00,  //  0082  GETNGBL	R9	K14
      0x8824131A,  //  0083  GETMBR	R9	R9	K26
      0x7C1C0400,  //  0084  CALL	R7	2
      0x781E000A,  //  0085  JMPF	R7	#0091
      0x881C0503,  //  0086  GETMBR	R7	R2	K3
      0x8C1C0F1B,  //  0087  GETMET	R7	R7	K27
      0x5C240200,  //  0088  MOVE	R9	R1
      0x5C280C00,  //  0089  MOVE	R10	R6
      0x502C0200,  //  008A  LDBOOL	R11	1	0
      0x7C1C0800,  //  008B  CALL	R7	4
      0x88200107,  //  008C  GETMBR	R8	R0	K7
      0x98200207,  //  008D  SETIDX	R8	R1	R7
      0xA8040002,  //  008E  EXBLK	1	2
      0x80040E00,  //  008F  RET	1	R7
      0x7002000F,  //  0090  JMP		#00A1
      0x601C000F,  //  0091  GETGBL	R7	G15
      0x5C200C00,  //  0092  MOVE	R8	R6
      0xB8261C00,  //  0093  GETNGBL	R9	K14
      0x8824130E,  //  0094  GETMBR	R9	R9	K14
      0x7C1C0400,  //  0095  CALL	R7	2
      0x781E0009,  //  0096  JMPF	R7	#00A1
      0x881C0503,  //  0097  GETMBR	R7	R2	K3
      0x8C1C0F1C,  //  0098  GETMET	R7	R7	K28
      0x5C240200,  //  0099  MOVE	R9	R1
      0x5C280C00,  //  009A  MOVE	R10	R6
      0x502C0200,  //  009B  LDBOOL	R11	1	0
      0x7C1C0800,  //  009C  CALL	R7	4
      0x88200107,  //  009D  GETMBR	R8	R0	K7
      0x98200207,  //  009E  SETIDX	R8	R1	R7
      0xA8040002,  //  009F  EXBLK	1	2
      0x80040E00,  //  00A0  RET	1	R7
      0xA8040001,  //  00A1  EXBLK	1	1
      0x70020003,  //  00A2  JMP		#00A7
      0xAC180002,  //  00A3  CATCH	R6	0	2
      0x70020000,  //  00A4  JMP		#00A6
      0x70020000,  //  00A5  JMP		#00A7
      0xB0080000,  //  00A6  RAISE	2	R0	R0
      0x4C100000,  //  00A7  LDNIL	R4
      0xA8040001,  //  00A8  EXBLK	1	1
      0x80040800,  //  00A9  RET	1	R4
      0xA8040001,  //  00AA  EXBLK	1	1
      0x70020005,  //  00AB  JMP		#00B2
      0xAC0C0002,  //  00AC  CATCH	R3	0	2
      0x70020002,  //  00AD  JMP		#00B1
      0x4C140000,  //  00AE  LDNIL	R5
      0x80040A00,  //  00AF  RET	1	R5
      0x70020000,  //  00B0  JMP		#00B2
      0xB0080000,  //  00B1  RAISE	2	R0	R0
      0x80000000,  //  00B2  RET	0
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
    9,                          /* nstack */
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
      0xA40E0400,  //  0000  IMPORT	R3	K2
      0x88100703,  //  0001  GETMBR	R4	R3	K3
      0x8C10090B,  //  0002  GETMET	R4	R4	K11
      0x5C180200,  //  0003  MOVE	R6	R1
      0x5C1C0400,  //  0004  MOVE	R7	R2
      0x50200000,  //  0005  LDBOOL	R8	0	0
      0x7C100800,  //  0006  CALL	R4	4
      0x8C140105,  //  0007  GETMET	R5	R0	K5
      0x5C1C0200,  //  0008  MOVE	R7	R1
      0x5C200800,  //  0009  MOVE	R8	R4
      0x7C140600,  //  000A  CALL	R5	3
      0x80040A00,  //  000B  RET	1	R5
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
    9,                          /* nstack */
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
      0xA40E0400,  //  0000  IMPORT	R3	K2
      0x88100703,  //  0001  GETMBR	R4	R3	K3
      0x8C10091F,  //  0002  GETMET	R4	R4	K31
      0x5C180200,  //  0003  MOVE	R6	R1
      0x5C1C0400,  //  0004  MOVE	R7	R2
      0x50200000,  //  0005  LDBOOL	R8	0	0
      0x7C100800,  //  0006  CALL	R4	4
      0x8C140105,  //  0007  GETMET	R5	R0	K5
      0x5C1C0200,  //  0008  MOVE	R7	R1
      0x5C200800,  //  0009  MOVE	R8	R4
      0x7C140600,  //  000A  CALL	R5	3
      0x80040A00,  //  000B  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _get_named_color_value
********************************************************************/
be_local_closure(class_SymbolTable__get_named_color_value,   /* name */
  be_nested_proto(
    8,                          /* nstack */
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
    ( &(const binstruction[21]) {  /* code */
      0xA40A0400,  //  0000  IMPORT	R2	K2
      0x8C0C0100,  //  0001  GETMET	R3	R0	K0
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x4C100000,  //  0004  LDNIL	R4
      0x20100604,  //  0005  NE	R4	R3	R4
      0x7812000C,  //  0006  JMPF	R4	#0014
      0x88100720,  //  0007  GETMBR	R4	R3	K32
      0x7812000A,  //  0008  JMPF	R4	#0014
      0x88100721,  //  0009  GETMBR	R4	R3	K33
      0x5416000A,  //  000A  LDINT	R5	11
      0x1C100805,  //  000B  EQ	R4	R4	R5
      0x78120006,  //  000C  JMPF	R4	#0014
      0x8810050A,  //  000D  GETMBR	R4	R2	K10
      0x94100801,  //  000E  GETIDX	R4	R4	R1
      0x60140018,  //  000F  GETGBL	R5	G24
      0x58180022,  //  0010  LDCONST	R6	K34
      0x5C1C0800,  //  0011  MOVE	R7	R4
      0x7C140400,  //  0012  CALL	R5	2
      0x80040A00,  //  0013  RET	1	R5
      0x80064600,  //  0014  RET	1	K35
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_reference
********************************************************************/
be_local_closure(class_SymbolTable_get_reference,   /* name */
  be_nested_proto(
    8,                          /* nstack */
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
    ( &(const binstruction[28]) {  /* code */
      0xA40A0400,  //  0000  IMPORT	R2	K2
      0x8C0C0100,  //  0001  GETMET	R3	R0	K0
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x4C100000,  //  0004  LDNIL	R4
      0x20100604,  //  0005  NE	R4	R3	R4
      0x7812000F,  //  0006  JMPF	R4	#0017
      0x88100720,  //  0007  GETMBR	R4	R3	K32
      0x7812000A,  //  0008  JMPF	R4	#0014
      0x88100721,  //  0009  GETMBR	R4	R3	K33
      0x5416000A,  //  000A  LDINT	R5	11
      0x1C100805,  //  000B  EQ	R4	R4	R5
      0x78120006,  //  000C  JMPF	R4	#0014
      0x8810050A,  //  000D  GETMBR	R4	R2	K10
      0x94100801,  //  000E  GETIDX	R4	R4	R1
      0x60140018,  //  000F  GETGBL	R5	G24
      0x58180022,  //  0010  LDCONST	R6	K34
      0x5C1C0800,  //  0011  MOVE	R7	R4
      0x7C140400,  //  0012  CALL	R5	2
      0x80040A00,  //  0013  RET	1	R5
      0x8C100724,  //  0014  GETMET	R4	R3	K36
      0x7C100200,  //  0015  CALL	R4	1
      0x80040800,  //  0016  RET	1	R4
      0x60100018,  //  0017  GETGBL	R4	G24
      0x58140025,  //  0018  LDCONST	R5	K37
      0x5C180200,  //  0019  MOVE	R6	R1
      0x7C100400,  //  001A  CALL	R4	2
      0x80040800,  //  001B  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_template
********************************************************************/
be_local_closure(class_SymbolTable_create_template,   /* name */
  be_nested_proto(
    9,                          /* nstack */
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
      0xA40E0400,  //  0000  IMPORT	R3	K2
      0x88100703,  //  0001  GETMBR	R4	R3	K3
      0x8C100926,  //  0002  GETMET	R4	R4	K38
      0x5C180200,  //  0003  MOVE	R6	R1
      0x501C0000,  //  0004  LDBOOL	R7	0	0
      0x7C100600,  //  0005  CALL	R4	3
      0x8C140927,  //  0006  GETMET	R5	R4	K39
      0x4C1C0000,  //  0007  LDNIL	R7
      0x201C0407,  //  0008  NE	R7	R2	R7
      0x781E0001,  //  0009  JMPF	R7	#000C
      0x5C1C0400,  //  000A  MOVE	R7	R2
      0x70020001,  //  000B  JMP		#000E
      0x601C0013,  //  000C  GETGBL	R7	G19
      0x7C1C0000,  //  000D  CALL	R7	0
      0x7C140400,  //  000E  CALL	R5	2
      0x8C140105,  //  000F  GETMET	R5	R0	K5
      0x5C1C0200,  //  0010  MOVE	R7	R1
      0x5C200800,  //  0011  MOVE	R8	R4
      0x7C140600,  //  0012  CALL	R5	3
      0x80040A00,  //  0013  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_generic
********************************************************************/
be_local_closure(class_SymbolTable_create_generic,   /* name */
  be_nested_proto(
    12,                          /* nstack */
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
      0xA4160400,  //  0000  IMPORT	R5	K2
      0x8C180B03,  //  0001  GETMET	R6	R5	K3
      0x5C200200,  //  0002  MOVE	R8	R1
      0x5C240400,  //  0003  MOVE	R9	R2
      0x5C280600,  //  0004  MOVE	R10	R3
      0x4C2C0000,  //  0005  LDNIL	R11
      0x202C080B,  //  0006  NE	R11	R4	R11
      0x782E0001,  //  0007  JMPF	R11	#000A
      0x5C2C0800,  //  0008  MOVE	R11	R4
      0x70020000,  //  0009  JMP		#000B
      0x502C0000,  //  000A  LDBOOL	R11	0	0
      0x7C180A00,  //  000B  CALL	R6	5
      0x8C1C0105,  //  000C  GETMET	R7	R0	K5
      0x5C240200,  //  000D  MOVE	R9	R1
      0x5C280C00,  //  000E  MOVE	R10	R6
      0x7C1C0600,  //  000F  CALL	R7	3
      0x80040E00,  //  0010  RET	1	R7
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
    4,                          /* nstack */
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
      0xA4060400,  //  0000  IMPORT	R1	K2
      0x60080013,  //  0001  GETGBL	R2	G19
      0x7C080000,  //  0002  CALL	R2	0
      0x90020E02,  //  0003  SETMBR	R0	K7	R2
      0x8C08032D,  //  0004  GETMET	R2	R1	K45
      0x7C080200,  //  0005  CALL	R2	1
      0x90022E02,  //  0006  SETMBR	R0	K23	R2
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
    8,                          /* nstack */
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
      0xA40A0400,  //  0000  IMPORT	R2	K2
      0x880C0503,  //  0001  GETMBR	R3	R2	K3
      0x8C0C072E,  //  0002  GETMET	R3	R3	K46
      0x5C140200,  //  0003  MOVE	R5	R1
      0x50180000,  //  0004  LDBOOL	R6	0	0
      0x7C0C0600,  //  0005  CALL	R3	3
      0x8C100105,  //  0006  GETMET	R4	R0	K5
      0x5C180200,  //  0007  MOVE	R6	R1
      0x5C1C0600,  //  0008  MOVE	R7	R3
      0x7C100600,  //  0009  CALL	R4	3
      0x80040800,  //  000A  RET	1	R4
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
    8,                          /* nstack */
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
      0xA40A0400,  //  0000  IMPORT	R2	K2
      0x8C0C0108,  //  0001  GETMET	R3	R0	K8
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x740E0008,  //  0004  JMPT	R3	#000E
      0x880C0503,  //  0005  GETMBR	R3	R2	K3
      0x8C0C070D,  //  0006  GETMET	R3	R3	K13
      0x5C140200,  //  0007  MOVE	R5	R1
      0x50180000,  //  0008  LDBOOL	R6	0	0
      0x7C0C0600,  //  0009  CALL	R3	3
      0x8C100105,  //  000A  GETMET	R4	R0	K5
      0x5C180200,  //  000B  MOVE	R6	R1
      0x5C1C0600,  //  000C  MOVE	R7	R3
      0x7C100600,  //  000D  CALL	R4	3
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
    9,                          /* nstack */
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
      0xA40E0400,  //  0000  IMPORT	R3	K2
      0x88100703,  //  0001  GETMBR	R4	R3	K3
      0x8C100930,  //  0002  GETMET	R4	R4	K48
      0x5C180200,  //  0003  MOVE	R6	R1
      0x5C1C0400,  //  0004  MOVE	R7	R2
      0x50200000,  //  0005  LDBOOL	R8	0	0
      0x7C100800,  //  0006  CALL	R4	4
      0x8C140105,  //  0007  GETMET	R5	R0	K5
      0x5C1C0200,  //  0008  MOVE	R7	R1
      0x5C200800,  //  0009  MOVE	R8	R4
      0x7C140600,  //  000A  CALL	R5	3
      0x80040A00,  //  000B  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_variable
********************************************************************/
be_local_closure(class_SymbolTable_create_variable,   /* name */
  be_nested_proto(
    8,                          /* nstack */
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
      0xA40A0400,  //  0000  IMPORT	R2	K2
      0x880C0503,  //  0001  GETMBR	R3	R2	K3
      0x8C0C0731,  //  0002  GETMET	R3	R3	K49
      0x5C140200,  //  0003  MOVE	R5	R1
      0x50180000,  //  0004  LDBOOL	R6	0	0
      0x7C0C0600,  //  0005  CALL	R3	3
      0x8C100105,  //  0006  GETMET	R4	R0	K5
      0x5C180200,  //  0007  MOVE	R6	R1
      0x5C1C0600,  //  0008  MOVE	R7	R3
      0x7C100600,  //  0009  CALL	R4	3
      0x80040800,  //  000A  RET	1	R4
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

/********************************************************************
** Solidified function: is_keyword
********************************************************************/
be_local_closure(is_keyword,   /* name */
  be_nested_proto(
    5,                          /* nstack */
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
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x60080010,  //  0001  GETGBL	R2	G16
      0x880C0301,  //  0002  GETMBR	R3	R1	K1
      0x880C0702,  //  0003  GETMBR	R3	R3	K2
      0x7C080200,  //  0004  CALL	R2	1
      0xA8020007,  //  0005  EXBLK	0	#000E
      0x5C0C0400,  //  0006  MOVE	R3	R2
      0x7C0C0000,  //  0007  CALL	R3	0
      0x1C100003,  //  0008  EQ	R4	R0	R3
      0x78120002,  //  0009  JMPF	R4	#000D
      0x50100200,  //  000A  LDBOOL	R4	1	0
      0xA8040001,  //  000B  EXBLK	1	1
      0x80040800,  //  000C  RET	1	R4
      0x7001FFF7,  //  000D  JMP		#0006
      0x58080003,  //  000E  LDCONST	R2	K3
      0xAC080200,  //  000F  CATCH	R2	1	0
      0xB0080000,  //  0010  RAISE	2	R0	R0
      0x50080000,  //  0011  LDBOOL	R2	0	0
      0x80040400,  //  0012  RET	1	R2
    })
  )
);
/*******************************************************************/

// compact class 'Lexer' ktab size: 109, total: 288 (saved 1432 bytes)
static const bvalue be_ktab_class_Lexer[109] = {
  /* K0   */  be_nested_str_weak(position),
  /* K1   */  be_const_int(0),
  /* K2   */  be_nested_str_weak(line),
  /* K3   */  be_const_int(1),
  /* K4   */  be_nested_str_weak(column),
  /* K5   */  be_nested_str_weak(token_position),
  /* K6   */  be_nested_str_weak(a),
  /* K7   */  be_nested_str_weak(z),
  /* K8   */  be_nested_str_weak(A),
  /* K9   */  be_nested_str_weak(Z),
  /* K10  */  be_nested_str_weak(Line_X20),
  /* K11  */  be_nested_str_weak(_X3A),
  /* K12  */  be_nested_str_weak(_X3A_X20),
  /* K13  */  be_nested_str_weak(lexical_error),
  /* K14  */  be_nested_str_weak(0),
  /* K15  */  be_nested_str_weak(9),
  /* K16  */  be_nested_str_weak(advance),
  /* K17  */  be_nested_str_weak(at_end),
  /* K18  */  be_nested_str_weak(is_hex_digit),
  /* K19  */  be_nested_str_weak(peek),
  /* K20  */  be_nested_str_weak(source),
  /* K21  */  be_nested_str_weak(create_token),
  /* K22  */  be_nested_str_weak(error),
  /* K23  */  be_nested_str_weak(Invalid_X20hex_X20color_X20format_X3A_X20),
  /* K24  */  be_nested_str_weak(_X20_X28expected_X200xRRGGBB_X20or_X200xAARRGGBB_X29),
  /* K25  */  be_nested_str_weak(),
  /* K26  */  be_nested_str_weak(animation_dsl),
  /* K27  */  be_nested_str_weak(Token),
  /* K28  */  be_nested_str_weak(is_digit),
  /* K29  */  be_nested_str_weak(_X2E),
  /* K30  */  be_nested_str_weak(check_time_suffix),
  /* K31  */  be_nested_str_weak(scan_time_suffix),
  /* K32  */  be_nested_str_weak(_X25),
  /* K33  */  be_nested_str_weak(x),
  /* K34  */  be_const_int(2),
  /* K35  */  be_nested_str_weak(next_token),
  /* K36  */  be_nested_str_weak(create_lexer),
  /* K37  */  be_nested_str_weak(_X20),
  /* K38  */  be_nested_str_weak(_X09),
  /* K39  */  be_nested_str_weak(_X0D),
  /* K40  */  be_nested_str_weak(_X0A),
  /* K41  */  be_nested_str_weak(_X23),
  /* K42  */  be_nested_str_weak(scan_comment),
  /* K43  */  be_nested_str_weak(scan_hex_color_0x),
  /* K44  */  be_nested_str_weak(is_alpha),
  /* K45  */  be_nested_str_weak(_),
  /* K46  */  be_nested_str_weak(scan_identifier_or_keyword),
  /* K47  */  be_nested_str_weak(scan_number),
  /* K48  */  be_nested_str_weak(_X22),
  /* K49  */  be_nested_str_weak(_X27),
  /* K50  */  be_nested_str_weak(peek_char_ahead),
  /* K51  */  be_nested_str_weak(scan_triple_quoted_string),
  /* K52  */  be_nested_str_weak(scan_string),
  /* K53  */  be_nested_str_weak(_X24),
  /* K54  */  be_nested_str_weak(scan_variable_reference),
  /* K55  */  be_nested_str_weak(scan_operator_or_delimiter),
  /* K56  */  be_const_int(3),
  /* K57  */  be_nested_str_weak(Unterminated_X20triple_X2Dquoted_X20string_X20literal),
  /* K58  */  be_nested_str_weak(_X3D),
  /* K59  */  be_nested_str_weak(match),
  /* K60  */  be_nested_str_weak(_X3D_X3D),
  /* K61  */  be_nested_str_weak(_X21),
  /* K62  */  be_nested_str_weak(_X21_X3D),
  /* K63  */  be_nested_str_weak(_X3C),
  /* K64  */  be_nested_str_weak(_X3C_X3D),
  /* K65  */  be_nested_str_weak(Left_X20shift_X20operator_X20_X27_X3C_X3C_X27_X20not_X20supported_X20in_X20DSL),
  /* K66  */  be_nested_str_weak(_X3E),
  /* K67  */  be_nested_str_weak(_X3E_X3D),
  /* K68  */  be_nested_str_weak(Right_X20shift_X20operator_X20_X27_X3E_X3E_X27_X20not_X20supported_X20in_X20DSL),
  /* K69  */  be_nested_str_weak(_X26),
  /* K70  */  be_nested_str_weak(_X26_X26),
  /* K71  */  be_nested_str_weak(Single_X20_X27_X26_X27_X20not_X20supported_X20in_X20DSL),
  /* K72  */  be_nested_str_weak(_X7C),
  /* K73  */  be_nested_str_weak(_X7C_X7C),
  /* K74  */  be_nested_str_weak(Single_X20_X27_X7C_X27_X20not_X20supported_X20in_X20DSL),
  /* K75  */  be_nested_str_weak(_X2D),
  /* K76  */  be_nested_str_weak(_X2D_X3E),
  /* K77  */  be_nested_str_weak(_X2B),
  /* K78  */  be_nested_str_weak(_X2A),
  /* K79  */  be_nested_str_weak(_X2F),
  /* K80  */  be_nested_str_weak(_X5E),
  /* K81  */  be_nested_str_weak(_X28),
  /* K82  */  be_nested_str_weak(_X29),
  /* K83  */  be_nested_str_weak(_X7B),
  /* K84  */  be_nested_str_weak(_X7D),
  /* K85  */  be_nested_str_weak(_X5B),
  /* K86  */  be_nested_str_weak(_X5D),
  /* K87  */  be_nested_str_weak(_X2C),
  /* K88  */  be_nested_str_weak(_X3B),
  /* K89  */  be_nested_str_weak(Unexpected_X20character_X3A_X20_X27),
  /* K90  */  be_nested_str_weak(string),
  /* K91  */  be_nested_str_weak(startswith),
  /* K92  */  be_const_int(2147483647),
  /* K93  */  be_nested_str_weak(ms),
  /* K94  */  be_nested_str_weak(s),
  /* K95  */  be_nested_str_weak(m),
  /* K96  */  be_nested_str_weak(h),
  /* K97  */  be_nested_str_weak(Invalid_X20variable_X20reference_X3A_X20_X24_X20must_X20be_X20followed_X20by_X20identifier),
  /* K98  */  be_nested_str_weak(is_alnum),
  /* K99  */  be_nested_str_weak(f),
  /* K100 */  be_nested_str_weak(F),
  /* K101 */  be_nested_str_weak(stop_iteration),
  /* K102 */  be_nested_str_weak(_X5C),
  /* K103 */  be_nested_str_weak(n),
  /* K104 */  be_nested_str_weak(t),
  /* K105 */  be_nested_str_weak(r),
  /* K106 */  be_nested_str_weak(Unterminated_X20string_X20literal),
  /* K107 */  be_nested_str_weak(is_color_name),
  /* K108 */  be_nested_str_weak(is_keyword),
};


extern const bclass be_class_Lexer;

/********************************************************************
** Solidified function: reset
********************************************************************/
be_local_closure(class_Lexer_reset,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Lexer,     /* shared constants */
    be_str_weak(reset),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x90020101,  //  0000  SETMBR	R0	K0	K1
      0x90020503,  //  0001  SETMBR	R0	K2	K3
      0x90020903,  //  0002  SETMBR	R0	K4	K3
      0x90020B01,  //  0003  SETMBR	R0	K5	K1
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_position
********************************************************************/
be_local_closure(class_Lexer_get_position,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Lexer,     /* shared constants */
    be_str_weak(get_position),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040105,  //  0000  GETMBR	R1	R0	K5
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_alpha
********************************************************************/
be_local_closure(class_Lexer_is_alpha,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Lexer,     /* shared constants */
    be_str_weak(is_alpha),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x28080306,  //  0000  GE	R2	R1	K6
      0x780A0001,  //  0001  JMPF	R2	#0004
      0x18080307,  //  0002  LE	R2	R1	K7
      0x740A0004,  //  0003  JMPT	R2	#0009
      0x28080308,  //  0004  GE	R2	R1	K8
      0x780A0001,  //  0005  JMPF	R2	#0008
      0x18080309,  //  0006  LE	R2	R1	K9
      0x740A0000,  //  0007  JMPT	R2	#0009
      0x50080001,  //  0008  LDBOOL	R2	0	1
      0x50080200,  //  0009  LDBOOL	R2	1	0
      0x80040400,  //  000A  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: error
********************************************************************/
be_local_closure(class_Lexer_error,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Lexer,     /* shared constants */
    be_str_weak(error),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x60080008,  //  0000  GETGBL	R2	G8
      0x880C0102,  //  0001  GETMBR	R3	R0	K2
      0x7C080200,  //  0002  CALL	R2	1
      0x000A1402,  //  0003  ADD	R2	K10	R2
      0x0008050B,  //  0004  ADD	R2	R2	K11
      0x600C0008,  //  0005  GETGBL	R3	G8
      0x88100104,  //  0006  GETMBR	R4	R0	K4
      0x7C0C0200,  //  0007  CALL	R3	1
      0x00080403,  //  0008  ADD	R2	R2	R3
      0x0008050C,  //  0009  ADD	R2	R2	K12
      0x00080401,  //  000A  ADD	R2	R2	R1
      0xB0061A02,  //  000B  RAISE	1	K13	R2
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_digit
********************************************************************/
be_local_closure(class_Lexer_is_digit,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Lexer,     /* shared constants */
    be_str_weak(is_digit),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x2808030E,  //  0000  GE	R2	R1	K14
      0x780A0001,  //  0001  JMPF	R2	#0004
      0x1808030F,  //  0002  LE	R2	R1	K15
      0x740A0000,  //  0003  JMPT	R2	#0005
      0x50080001,  //  0004  LDBOOL	R2	0	1
      0x50080200,  //  0005  LDBOOL	R2	1	0
      0x80040400,  //  0006  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: scan_hex_color_0x
********************************************************************/
be_local_closure(class_Lexer_scan_hex_color_0x,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Lexer,     /* shared constants */
    be_str_weak(scan_hex_color_0x),
    &be_const_str_solidified,
    ( &(const binstruction[44]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x04040303,  //  0001  SUB	R1	R1	K3
      0x88080104,  //  0002  GETMBR	R2	R0	K4
      0x04080503,  //  0003  SUB	R2	R2	K3
      0x8C0C0110,  //  0004  GETMET	R3	R0	K16
      0x7C0C0200,  //  0005  CALL	R3	1
      0x580C0001,  //  0006  LDCONST	R3	K1
      0x8C100111,  //  0007  GETMET	R4	R0	K17
      0x7C100200,  //  0008  CALL	R4	1
      0x74120008,  //  0009  JMPT	R4	#0013
      0x8C100112,  //  000A  GETMET	R4	R0	K18
      0x8C180113,  //  000B  GETMET	R6	R0	K19
      0x7C180200,  //  000C  CALL	R6	1
      0x7C100400,  //  000D  CALL	R4	2
      0x78120003,  //  000E  JMPF	R4	#0013
      0x8C100110,  //  000F  GETMET	R4	R0	K16
      0x7C100200,  //  0010  CALL	R4	1
      0x000C0703,  //  0011  ADD	R3	R3	K3
      0x7001FFF3,  //  0012  JMP		#0007
      0x88100100,  //  0013  GETMBR	R4	R0	K0
      0x04100903,  //  0014  SUB	R4	R4	K3
      0x40100204,  //  0015  CONNECT	R4	R1	R4
      0x88140114,  //  0016  GETMBR	R5	R0	K20
      0x94100A04,  //  0017  GETIDX	R4	R5	R4
      0x54160005,  //  0018  LDINT	R5	6
      0x1C140605,  //  0019  EQ	R5	R3	R5
      0x74160002,  //  001A  JMPT	R5	#001E
      0x54160007,  //  001B  LDINT	R5	8
      0x1C140605,  //  001C  EQ	R5	R3	R5
      0x78160008,  //  001D  JMPF	R5	#0027
      0x8C140115,  //  001E  GETMET	R5	R0	K21
      0x541E0003,  //  001F  LDINT	R7	4
      0x5C200800,  //  0020  MOVE	R8	R4
      0x6024000C,  //  0021  GETGBL	R9	G12
      0x5C280800,  //  0022  MOVE	R10	R4
      0x7C240200,  //  0023  CALL	R9	1
      0x7C140800,  //  0024  CALL	R5	4
      0x80040A00,  //  0025  RET	1	R5
      0x70020003,  //  0026  JMP		#002B
      0x8C140116,  //  0027  GETMET	R5	R0	K22
      0x001E2E04,  //  0028  ADD	R7	K23	R4
      0x001C0F18,  //  0029  ADD	R7	R7	K24
      0x7C140400,  //  002A  CALL	R5	2
      0x80000000,  //  002B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: peek
********************************************************************/
be_local_closure(class_Lexer_peek,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Lexer,     /* shared constants */
    be_str_weak(peek),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x8C040111,  //  0000  GETMET	R1	R0	K17
      0x7C040200,  //  0001  CALL	R1	1
      0x78060000,  //  0002  JMPF	R1	#0004
      0x80063200,  //  0003  RET	1	K25
      0x88040114,  //  0004  GETMBR	R1	R0	K20
      0x88080100,  //  0005  GETMBR	R2	R0	K0
      0x94040202,  //  0006  GETIDX	R1	R1	R2
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: advance
********************************************************************/
be_local_closure(class_Lexer_advance,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Lexer,     /* shared constants */
    be_str_weak(advance),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x8C040111,  //  0000  GETMET	R1	R0	K17
      0x7C040200,  //  0001  CALL	R1	1
      0x78060000,  //  0002  JMPF	R1	#0004
      0x80063200,  //  0003  RET	1	K25
      0x88040114,  //  0004  GETMBR	R1	R0	K20
      0x88080100,  //  0005  GETMBR	R2	R0	K0
      0x94040202,  //  0006  GETIDX	R1	R1	R2
      0x88080100,  //  0007  GETMBR	R2	R0	K0
      0x00080503,  //  0008  ADD	R2	R2	K3
      0x90020002,  //  0009  SETMBR	R0	K0	R2
      0x88080104,  //  000A  GETMBR	R2	R0	K4
      0x00080503,  //  000B  ADD	R2	R2	K3
      0x90020802,  //  000C  SETMBR	R0	K4	R2
      0x80040200,  //  000D  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_token
********************************************************************/
be_local_closure(class_Lexer_create_token,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Lexer,     /* shared constants */
    be_str_weak(create_token),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0xA4123400,  //  0000  IMPORT	R4	K26
      0x8C14091B,  //  0001  GETMET	R5	R4	K27
      0x5C1C0200,  //  0002  MOVE	R7	R1
      0x5C200400,  //  0003  MOVE	R8	R2
      0x88240102,  //  0004  GETMBR	R9	R0	K2
      0x88280104,  //  0005  GETMBR	R10	R0	K4
      0x04281403,  //  0006  SUB	R10	R10	R3
      0x5C2C0600,  //  0007  MOVE	R11	R3
      0x7C140C00,  //  0008  CALL	R5	6
      0x80040A00,  //  0009  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: at_end
********************************************************************/
be_local_closure(class_Lexer_at_end,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Lexer,     /* shared constants */
    be_str_weak(at_end),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x6008000C,  //  0001  GETGBL	R2	G12
      0x880C0114,  //  0002  GETMBR	R3	R0	K20
      0x7C080200,  //  0003  CALL	R2	1
      0x28040202,  //  0004  GE	R1	R1	R2
      0x80040200,  //  0005  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: peek_char_ahead
********************************************************************/
be_local_closure(class_Lexer_peek_char_ahead,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Lexer,     /* shared constants */
    be_str_weak(peek_char_ahead),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x00080401,  //  0001  ADD	R2	R2	R1
      0x600C000C,  //  0002  GETGBL	R3	G12
      0x88100114,  //  0003  GETMBR	R4	R0	K20
      0x7C0C0200,  //  0004  CALL	R3	1
      0x28080403,  //  0005  GE	R2	R2	R3
      0x780A0000,  //  0006  JMPF	R2	#0008
      0x80063200,  //  0007  RET	1	K25
      0x88080100,  //  0008  GETMBR	R2	R0	K0
      0x00080401,  //  0009  ADD	R2	R2	R1
      0x880C0114,  //  000A  GETMBR	R3	R0	K20
      0x94080602,  //  000B  GETIDX	R2	R3	R2
      0x80040400,  //  000C  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: scan_number
********************************************************************/
be_local_closure(class_Lexer_scan_number,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Lexer,     /* shared constants */
    be_str_weak(scan_number),
    &be_const_str_solidified,
    ( &(const binstruction[117]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x04040303,  //  0001  SUB	R1	R1	K3
      0x88080104,  //  0002  GETMBR	R2	R0	K4
      0x04080503,  //  0003  SUB	R2	R2	K3
      0x500C0000,  //  0004  LDBOOL	R3	0	0
      0x8C100111,  //  0005  GETMET	R4	R0	K17
      0x7C100200,  //  0006  CALL	R4	1
      0x74120007,  //  0007  JMPT	R4	#0010
      0x8C10011C,  //  0008  GETMET	R4	R0	K28
      0x8C180113,  //  0009  GETMET	R6	R0	K19
      0x7C180200,  //  000A  CALL	R6	1
      0x7C100400,  //  000B  CALL	R4	2
      0x78120002,  //  000C  JMPF	R4	#0010
      0x8C100110,  //  000D  GETMET	R4	R0	K16
      0x7C100200,  //  000E  CALL	R4	1
      0x7001FFF4,  //  000F  JMP		#0005
      0x8C100111,  //  0010  GETMET	R4	R0	K17
      0x7C100200,  //  0011  CALL	R4	1
      0x7412001F,  //  0012  JMPT	R4	#0033
      0x8C100113,  //  0013  GETMET	R4	R0	K19
      0x7C100200,  //  0014  CALL	R4	1
      0x1C10091D,  //  0015  EQ	R4	R4	K29
      0x7812001B,  //  0016  JMPF	R4	#0033
      0x88100100,  //  0017  GETMBR	R4	R0	K0
      0x00100903,  //  0018  ADD	R4	R4	K3
      0x6014000C,  //  0019  GETGBL	R5	G12
      0x88180114,  //  001A  GETMBR	R6	R0	K20
      0x7C140200,  //  001B  CALL	R5	1
      0x14100805,  //  001C  LT	R4	R4	R5
      0x78120014,  //  001D  JMPF	R4	#0033
      0x8C10011C,  //  001E  GETMET	R4	R0	K28
      0x88180100,  //  001F  GETMBR	R6	R0	K0
      0x00180D03,  //  0020  ADD	R6	R6	K3
      0x881C0114,  //  0021  GETMBR	R7	R0	K20
      0x94180E06,  //  0022  GETIDX	R6	R7	R6
      0x7C100400,  //  0023  CALL	R4	2
      0x7812000D,  //  0024  JMPF	R4	#0033
      0x500C0200,  //  0025  LDBOOL	R3	1	0
      0x8C100110,  //  0026  GETMET	R4	R0	K16
      0x7C100200,  //  0027  CALL	R4	1
      0x8C100111,  //  0028  GETMET	R4	R0	K17
      0x7C100200,  //  0029  CALL	R4	1
      0x74120007,  //  002A  JMPT	R4	#0033
      0x8C10011C,  //  002B  GETMET	R4	R0	K28
      0x8C180113,  //  002C  GETMET	R6	R0	K19
      0x7C180200,  //  002D  CALL	R6	1
      0x7C100400,  //  002E  CALL	R4	2
      0x78120002,  //  002F  JMPF	R4	#0033
      0x8C100110,  //  0030  GETMET	R4	R0	K16
      0x7C100200,  //  0031  CALL	R4	1
      0x7001FFF4,  //  0032  JMP		#0028
      0x88100100,  //  0033  GETMBR	R4	R0	K0
      0x04100903,  //  0034  SUB	R4	R4	K3
      0x40100204,  //  0035  CONNECT	R4	R1	R4
      0x88140114,  //  0036  GETMBR	R5	R0	K20
      0x94100A04,  //  0037  GETIDX	R4	R5	R4
      0x8C14011E,  //  0038  GETMET	R5	R0	K30
      0x7C140200,  //  0039  CALL	R5	1
      0x7816000A,  //  003A  JMPF	R5	#0046
      0x8C14011F,  //  003B  GETMET	R5	R0	K31
      0x7C140200,  //  003C  CALL	R5	1
      0x8C180115,  //  003D  GETMET	R6	R0	K21
      0x54220004,  //  003E  LDINT	R8	5
      0x00240805,  //  003F  ADD	R9	R4	R5
      0x6028000C,  //  0040  GETGBL	R10	G12
      0x002C0805,  //  0041  ADD	R11	R4	R5
      0x7C280200,  //  0042  CALL	R10	1
      0x7C180800,  //  0043  CALL	R6	4
      0x80040C00,  //  0044  RET	1	R6
      0x7002002D,  //  0045  JMP		#0074
      0x8C140111,  //  0046  GETMET	R5	R0	K17
      0x7C140200,  //  0047  CALL	R5	1
      0x7416000F,  //  0048  JMPT	R5	#0059
      0x8C140113,  //  0049  GETMET	R5	R0	K19
      0x7C140200,  //  004A  CALL	R5	1
      0x1C140B20,  //  004B  EQ	R5	R5	K32
      0x7816000B,  //  004C  JMPF	R5	#0059
      0x8C140110,  //  004D  GETMET	R5	R0	K16
      0x7C140200,  //  004E  CALL	R5	1
      0x8C140115,  //  004F  GETMET	R5	R0	K21
      0x541E0005,  //  0050  LDINT	R7	6
      0x00200920,  //  0051  ADD	R8	R4	K32
      0x6024000C,  //  0052  GETGBL	R9	G12
      0x5C280800,  //  0053  MOVE	R10	R4
      0x7C240200,  //  0054  CALL	R9	1
      0x00241303,  //  0055  ADD	R9	R9	K3
      0x7C140800,  //  0056  CALL	R5	4
      0x80040A00,  //  0057  RET	1	R5
      0x7002001A,  //  0058  JMP		#0074
      0x8C140111,  //  0059  GETMET	R5	R0	K17
      0x7C140200,  //  005A  CALL	R5	1
      0x7416000F,  //  005B  JMPT	R5	#006C
      0x8C140113,  //  005C  GETMET	R5	R0	K19
      0x7C140200,  //  005D  CALL	R5	1
      0x1C140B21,  //  005E  EQ	R5	R5	K33
      0x7816000B,  //  005F  JMPF	R5	#006C
      0x8C140110,  //  0060  GETMET	R5	R0	K16
      0x7C140200,  //  0061  CALL	R5	1
      0x8C140115,  //  0062  GETMET	R5	R0	K21
      0x541E0006,  //  0063  LDINT	R7	7
      0x00200921,  //  0064  ADD	R8	R4	K33
      0x6024000C,  //  0065  GETGBL	R9	G12
      0x5C280800,  //  0066  MOVE	R10	R4
      0x7C240200,  //  0067  CALL	R9	1
      0x00241303,  //  0068  ADD	R9	R9	K3
      0x7C140800,  //  0069  CALL	R5	4
      0x80040A00,  //  006A  RET	1	R5
      0x70020007,  //  006B  JMP		#0074
      0x8C140115,  //  006C  GETMET	R5	R0	K21
      0x581C0022,  //  006D  LDCONST	R7	K34
      0x5C200800,  //  006E  MOVE	R8	R4
      0x6024000C,  //  006F  GETGBL	R9	G12
      0x5C280800,  //  0070  MOVE	R10	R4
      0x7C240200,  //  0071  CALL	R9	1
      0x7C140800,  //  0072  CALL	R5	4
      0x80040A00,  //  0073  RET	1	R5
      0x80000000,  //  0074  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Lexer_init,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Lexer,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x20080202,  //  0001  NE	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x5C080200,  //  0003  MOVE	R2	R1
      0x70020000,  //  0004  JMP		#0006
      0x58080019,  //  0005  LDCONST	R2	K25
      0x90022802,  //  0006  SETMBR	R0	K20	R2
      0x90020101,  //  0007  SETMBR	R0	K0	K1
      0x90020503,  //  0008  SETMBR	R0	K2	K3
      0x90020903,  //  0009  SETMBR	R0	K4	K3
      0x90020B01,  //  000A  SETMBR	R0	K5	K1
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: peek_token
********************************************************************/
be_local_closure(class_Lexer_peek_token,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Lexer,     /* shared constants */
    be_str_weak(peek_token),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88080102,  //  0001  GETMBR	R2	R0	K2
      0x880C0104,  //  0002  GETMBR	R3	R0	K4
      0x88100105,  //  0003  GETMBR	R4	R0	K5
      0x8C140123,  //  0004  GETMET	R5	R0	K35
      0x7C140200,  //  0005  CALL	R5	1
      0x4C180000,  //  0006  LDNIL	R6
      0x20180A06,  //  0007  NE	R6	R5	R6
      0x781A0003,  //  0008  JMPF	R6	#000D
      0x90020001,  //  0009  SETMBR	R0	K0	R1
      0x90020402,  //  000A  SETMBR	R0	K2	R2
      0x90020803,  //  000B  SETMBR	R0	K4	R3
      0x90020A04,  //  000C  SETMBR	R0	K5	R4
      0x80040A00,  //  000D  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_sub_lexer
********************************************************************/
be_local_closure(class_Lexer_create_sub_lexer,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Lexer,     /* shared constants */
    be_str_weak(create_sub_lexer),
    &be_const_str_solidified,
    ( &(const binstruction[89]) {  /* code */
      0xA40E3400,  //  0000  IMPORT	R3	K26
      0x14100301,  //  0001  LT	R4	R1	K1
      0x74120001,  //  0002  JMPT	R4	#0005
      0x18100401,  //  0003  LE	R4	R2	R1
      0x78120003,  //  0004  JMPF	R4	#0009
      0x8C100724,  //  0005  GETMET	R4	R3	K36
      0x58180019,  //  0006  LDCONST	R6	K25
      0x7C100400,  //  0007  CALL	R4	2
      0x80040800,  //  0008  RET	1	R4
      0x88100100,  //  0009  GETMBR	R4	R0	K0
      0x88140102,  //  000A  GETMBR	R5	R0	K2
      0x88180104,  //  000B  GETMBR	R6	R0	K4
      0x881C0105,  //  000C  GETMBR	R7	R0	K5
      0x90020101,  //  000D  SETMBR	R0	K0	K1
      0x90020503,  //  000E  SETMBR	R0	K2	K3
      0x90020903,  //  000F  SETMBR	R0	K4	K3
      0x90020B01,  //  0010  SETMBR	R0	K5	K1
      0x58200001,  //  0011  LDCONST	R8	K1
      0x6024000C,  //  0012  GETGBL	R9	G12
      0x88280114,  //  0013  GETMBR	R10	R0	K20
      0x7C240200,  //  0014  CALL	R9	1
      0x50280000,  //  0015  LDBOOL	R10	0	0
      0x502C0000,  //  0016  LDBOOL	R11	0	0
      0x88300105,  //  0017  GETMBR	R12	R0	K5
      0x14301801,  //  0018  LT	R12	R12	R1
      0x78320006,  //  0019  JMPF	R12	#0021
      0x8C300111,  //  001A  GETMET	R12	R0	K17
      0x7C300200,  //  001B  CALL	R12	1
      0x74320003,  //  001C  JMPT	R12	#0021
      0x88200100,  //  001D  GETMBR	R8	R0	K0
      0x8C300123,  //  001E  GETMET	R12	R0	K35
      0x7C300200,  //  001F  CALL	R12	1
      0x7001FFF5,  //  0020  JMP		#0017
      0x88300105,  //  0021  GETMBR	R12	R0	K5
      0x1C301801,  //  0022  EQ	R12	R12	R1
      0x78320001,  //  0023  JMPF	R12	#0026
      0x88200100,  //  0024  GETMBR	R8	R0	K0
      0x50280200,  //  0025  LDBOOL	R10	1	0
      0x88300105,  //  0026  GETMBR	R12	R0	K5
      0x14301802,  //  0027  LT	R12	R12	R2
      0x78320005,  //  0028  JMPF	R12	#002F
      0x8C300111,  //  0029  GETMET	R12	R0	K17
      0x7C300200,  //  002A  CALL	R12	1
      0x74320002,  //  002B  JMPT	R12	#002F
      0x8C300123,  //  002C  GETMET	R12	R0	K35
      0x7C300200,  //  002D  CALL	R12	1
      0x7001FFF6,  //  002E  JMP		#0026
      0x88300105,  //  002F  GETMBR	R12	R0	K5
      0x1C301802,  //  0030  EQ	R12	R12	R2
      0x78320001,  //  0031  JMPF	R12	#0034
      0x88240100,  //  0032  GETMBR	R9	R0	K0
      0x502C0200,  //  0033  LDBOOL	R11	1	0
      0x90020004,  //  0034  SETMBR	R0	K0	R4
      0x90020405,  //  0035  SETMBR	R0	K2	R5
      0x90020806,  //  0036  SETMBR	R0	K4	R6
      0x90020A07,  //  0037  SETMBR	R0	K5	R7
      0x5C301400,  //  0038  MOVE	R12	R10
      0x74320003,  //  0039  JMPT	R12	#003E
      0x8C300724,  //  003A  GETMET	R12	R3	K36
      0x58380019,  //  003B  LDCONST	R14	K25
      0x7C300400,  //  003C  CALL	R12	2
      0x80041800,  //  003D  RET	1	R12
      0x6030000C,  //  003E  GETGBL	R12	G12
      0x88340114,  //  003F  GETMBR	R13	R0	K20
      0x7C300200,  //  0040  CALL	R12	1
      0x2430120C,  //  0041  GT	R12	R9	R12
      0x78320003,  //  0042  JMPF	R12	#0047
      0x6030000C,  //  0043  GETGBL	R12	G12
      0x88340114,  //  0044  GETMBR	R13	R0	K20
      0x7C300200,  //  0045  CALL	R12	1
      0x5C241800,  //  0046  MOVE	R9	R12
      0x28301009,  //  0047  GE	R12	R8	R9
      0x78320003,  //  0048  JMPF	R12	#004D
      0x8C300724,  //  0049  GETMET	R12	R3	K36
      0x58380019,  //  004A  LDCONST	R14	K25
      0x7C300400,  //  004B  CALL	R12	2
      0x80041800,  //  004C  RET	1	R12
      0x04301303,  //  004D  SUB	R12	R9	K3
      0x4030100C,  //  004E  CONNECT	R12	R8	R12
      0x88340114,  //  004F  GETMBR	R13	R0	K20
      0x94301A0C,  //  0050  GETIDX	R12	R13	R12
      0x8C340724,  //  0051  GETMET	R13	R3	K36
      0x5C3C1800,  //  0052  MOVE	R15	R12
      0x7C340400,  //  0053  CALL	R13	2
      0x90360101,  //  0054  SETMBR	R13	K0	K1
      0x90360503,  //  0055  SETMBR	R13	K2	K3
      0x90360903,  //  0056  SETMBR	R13	K4	K3
      0x90360B01,  //  0057  SETMBR	R13	K5	K1
      0x80041A00,  //  0058  RET	1	R13
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: match
********************************************************************/
be_local_closure(class_Lexer_match,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Lexer,     /* shared constants */
    be_str_weak(match),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C080111,  //  0000  GETMET	R2	R0	K17
      0x7C080200,  //  0001  CALL	R2	1
      0x740A0004,  //  0002  JMPT	R2	#0008
      0x88080114,  //  0003  GETMBR	R2	R0	K20
      0x880C0100,  //  0004  GETMBR	R3	R0	K0
      0x94080403,  //  0005  GETIDX	R2	R2	R3
      0x20080401,  //  0006  NE	R2	R2	R1
      0x780A0001,  //  0007  JMPF	R2	#000A
      0x50080000,  //  0008  LDBOOL	R2	0	0
      0x80040400,  //  0009  RET	1	R2
      0x88080100,  //  000A  GETMBR	R2	R0	K0
      0x00080503,  //  000B  ADD	R2	R2	K3
      0x90020002,  //  000C  SETMBR	R0	K0	R2
      0x88080104,  //  000D  GETMBR	R2	R0	K4
      0x00080503,  //  000E  ADD	R2	R2	K3
      0x90020802,  //  000F  SETMBR	R0	K4	R2
      0x50080200,  //  0010  LDBOOL	R2	1	0
      0x80040400,  //  0011  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: next_token
********************************************************************/
be_local_closure(class_Lexer_next_token,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Lexer,     /* shared constants */
    be_str_weak(next_token),
    &be_const_str_solidified,
    ( &(const binstruction[137]) {  /* code */
      0x8C040111,  //  0000  GETMET	R1	R0	K17
      0x7C040200,  //  0001  CALL	R1	1
      0x74060083,  //  0002  JMPT	R1	#0087
      0x88040104,  //  0003  GETMBR	R1	R0	K4
      0x8C080110,  //  0004  GETMET	R2	R0	K16
      0x7C080200,  //  0005  CALL	R2	1
      0x1C0C0525,  //  0006  EQ	R3	R2	K37
      0x740E0003,  //  0007  JMPT	R3	#000C
      0x1C0C0526,  //  0008  EQ	R3	R2	K38
      0x740E0001,  //  0009  JMPT	R3	#000C
      0x1C0C0527,  //  000A  EQ	R3	R2	K39
      0x780E0001,  //  000B  JMPF	R3	#000E
      0x7001FFF2,  //  000C  JMP		#0000
      0x70020077,  //  000D  JMP		#0086
      0x1C0C0528,  //  000E  EQ	R3	R2	K40
      0x780E000D,  //  000F  JMPF	R3	#001E
      0x8C0C0115,  //  0010  GETMET	R3	R0	K21
      0x54160022,  //  0011  LDINT	R5	35
      0x58180028,  //  0012  LDCONST	R6	K40
      0x581C0003,  //  0013  LDCONST	R7	K3
      0x7C0C0800,  //  0014  CALL	R3	4
      0x88100102,  //  0015  GETMBR	R4	R0	K2
      0x00100903,  //  0016  ADD	R4	R4	K3
      0x90020404,  //  0017  SETMBR	R0	K2	R4
      0x90020903,  //  0018  SETMBR	R0	K4	K3
      0x88100105,  //  0019  GETMBR	R4	R0	K5
      0x00100903,  //  001A  ADD	R4	R4	K3
      0x90020A04,  //  001B  SETMBR	R0	K5	R4
      0x80040600,  //  001C  RET	1	R3
      0x70020067,  //  001D  JMP		#0086
      0x1C0C0529,  //  001E  EQ	R3	R2	K41
      0x780E0006,  //  001F  JMPF	R3	#0027
      0x8C0C012A,  //  0020  GETMET	R3	R0	K42
      0x7C0C0200,  //  0021  CALL	R3	1
      0x88100105,  //  0022  GETMBR	R4	R0	K5
      0x00100903,  //  0023  ADD	R4	R4	K3
      0x90020A04,  //  0024  SETMBR	R0	K5	R4
      0x80040600,  //  0025  RET	1	R3
      0x7002005E,  //  0026  JMP		#0086
      0x1C0C050E,  //  0027  EQ	R3	R2	K14
      0x780E000A,  //  0028  JMPF	R3	#0034
      0x8C0C0113,  //  0029  GETMET	R3	R0	K19
      0x7C0C0200,  //  002A  CALL	R3	1
      0x1C0C0721,  //  002B  EQ	R3	R3	K33
      0x780E0006,  //  002C  JMPF	R3	#0034
      0x8C0C012B,  //  002D  GETMET	R3	R0	K43
      0x7C0C0200,  //  002E  CALL	R3	1
      0x88100105,  //  002F  GETMBR	R4	R0	K5
      0x00100903,  //  0030  ADD	R4	R4	K3
      0x90020A04,  //  0031  SETMBR	R0	K5	R4
      0x80040600,  //  0032  RET	1	R3
      0x70020051,  //  0033  JMP		#0086
      0x8C0C012C,  //  0034  GETMET	R3	R0	K44
      0x5C140400,  //  0035  MOVE	R5	R2
      0x7C0C0400,  //  0036  CALL	R3	2
      0x740E0001,  //  0037  JMPT	R3	#003A
      0x1C0C052D,  //  0038  EQ	R3	R2	K45
      0x780E0006,  //  0039  JMPF	R3	#0041
      0x8C0C012E,  //  003A  GETMET	R3	R0	K46
      0x7C0C0200,  //  003B  CALL	R3	1
      0x88100105,  //  003C  GETMBR	R4	R0	K5
      0x00100903,  //  003D  ADD	R4	R4	K3
      0x90020A04,  //  003E  SETMBR	R0	K5	R4
      0x80040600,  //  003F  RET	1	R3
      0x70020044,  //  0040  JMP		#0086
      0x8C0C011C,  //  0041  GETMET	R3	R0	K28
      0x5C140400,  //  0042  MOVE	R5	R2
      0x7C0C0400,  //  0043  CALL	R3	2
      0x780E0006,  //  0044  JMPF	R3	#004C
      0x8C0C012F,  //  0045  GETMET	R3	R0	K47
      0x7C0C0200,  //  0046  CALL	R3	1
      0x88100105,  //  0047  GETMBR	R4	R0	K5
      0x00100903,  //  0048  ADD	R4	R4	K3
      0x90020A04,  //  0049  SETMBR	R0	K5	R4
      0x80040600,  //  004A  RET	1	R3
      0x70020039,  //  004B  JMP		#0086
      0x1C0C0530,  //  004C  EQ	R3	R2	K48
      0x740E0001,  //  004D  JMPT	R3	#0050
      0x1C0C0531,  //  004E  EQ	R3	R2	K49
      0x780E0025,  //  004F  JMPF	R3	#0076
      0x1C0C0530,  //  0050  EQ	R3	R2	K48
      0x780E0008,  //  0051  JMPF	R3	#005B
      0x8C0C0113,  //  0052  GETMET	R3	R0	K19
      0x7C0C0200,  //  0053  CALL	R3	1
      0x1C0C0730,  //  0054  EQ	R3	R3	K48
      0x780E0004,  //  0055  JMPF	R3	#005B
      0x8C0C0132,  //  0056  GETMET	R3	R0	K50
      0x58140003,  //  0057  LDCONST	R5	K3
      0x7C0C0400,  //  0058  CALL	R3	2
      0x1C0C0730,  //  0059  EQ	R3	R3	K48
      0x740E000A,  //  005A  JMPT	R3	#0066
      0x1C0C0531,  //  005B  EQ	R3	R2	K49
      0x780E0010,  //  005C  JMPF	R3	#006E
      0x8C0C0113,  //  005D  GETMET	R3	R0	K19
      0x7C0C0200,  //  005E  CALL	R3	1
      0x1C0C0731,  //  005F  EQ	R3	R3	K49
      0x780E000C,  //  0060  JMPF	R3	#006E
      0x8C0C0132,  //  0061  GETMET	R3	R0	K50
      0x58140003,  //  0062  LDCONST	R5	K3
      0x7C0C0400,  //  0063  CALL	R3	2
      0x1C0C0731,  //  0064  EQ	R3	R3	K49
      0x780E0007,  //  0065  JMPF	R3	#006E
      0x8C0C0133,  //  0066  GETMET	R3	R0	K51
      0x5C140400,  //  0067  MOVE	R5	R2
      0x7C0C0400,  //  0068  CALL	R3	2
      0x88100105,  //  0069  GETMBR	R4	R0	K5
      0x00100903,  //  006A  ADD	R4	R4	K3
      0x90020A04,  //  006B  SETMBR	R0	K5	R4
      0x80040600,  //  006C  RET	1	R3
      0x70020006,  //  006D  JMP		#0075
      0x8C0C0134,  //  006E  GETMET	R3	R0	K52
      0x5C140400,  //  006F  MOVE	R5	R2
      0x7C0C0400,  //  0070  CALL	R3	2
      0x88100105,  //  0071  GETMBR	R4	R0	K5
      0x00100903,  //  0072  ADD	R4	R4	K3
      0x90020A04,  //  0073  SETMBR	R0	K5	R4
      0x80040600,  //  0074  RET	1	R3
      0x7002000F,  //  0075  JMP		#0086
      0x1C0C0535,  //  0076  EQ	R3	R2	K53
      0x780E0006,  //  0077  JMPF	R3	#007F
      0x8C0C0136,  //  0078  GETMET	R3	R0	K54
      0x7C0C0200,  //  0079  CALL	R3	1
      0x88100105,  //  007A  GETMBR	R4	R0	K5
      0x00100903,  //  007B  ADD	R4	R4	K3
      0x90020A04,  //  007C  SETMBR	R0	K5	R4
      0x80040600,  //  007D  RET	1	R3
      0x70020006,  //  007E  JMP		#0086
      0x8C0C0137,  //  007F  GETMET	R3	R0	K55
      0x5C140400,  //  0080  MOVE	R5	R2
      0x7C0C0400,  //  0081  CALL	R3	2
      0x88100105,  //  0082  GETMBR	R4	R0	K5
      0x00100903,  //  0083  ADD	R4	R4	K3
      0x90020A04,  //  0084  SETMBR	R0	K5	R4
      0x80040600,  //  0085  RET	1	R3
      0x7001FF78,  //  0086  JMP		#0000
      0x4C040000,  //  0087  LDNIL	R1
      0x80040200,  //  0088  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: scan_triple_quoted_string
********************************************************************/
be_local_closure(class_Lexer_scan_triple_quoted_string,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Lexer,     /* shared constants */
    be_str_weak(scan_triple_quoted_string),
    &be_const_str_solidified,
    ( &(const binstruction[70]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x04080503,  //  0001  SUB	R2	R2	K3
      0x880C0104,  //  0002  GETMBR	R3	R0	K4
      0x040C0703,  //  0003  SUB	R3	R3	K3
      0x58100019,  //  0004  LDCONST	R4	K25
      0x8C140110,  //  0005  GETMET	R5	R0	K16
      0x7C140200,  //  0006  CALL	R5	1
      0x8C140110,  //  0007  GETMET	R5	R0	K16
      0x7C140200,  //  0008  CALL	R5	1
      0x8C140111,  //  0009  GETMET	R5	R0	K17
      0x7C140200,  //  000A  CALL	R5	1
      0x7416001F,  //  000B  JMPT	R5	#002C
      0x8C140113,  //  000C  GETMET	R5	R0	K19
      0x7C140200,  //  000D  CALL	R5	1
      0x1C180A01,  //  000E  EQ	R6	R5	R1
      0x781A0010,  //  000F  JMPF	R6	#0021
      0x8C180132,  //  0010  GETMET	R6	R0	K50
      0x58200003,  //  0011  LDCONST	R8	K3
      0x7C180400,  //  0012  CALL	R6	2
      0x1C180C01,  //  0013  EQ	R6	R6	R1
      0x781A000B,  //  0014  JMPF	R6	#0021
      0x8C180132,  //  0015  GETMET	R6	R0	K50
      0x58200022,  //  0016  LDCONST	R8	K34
      0x7C180400,  //  0017  CALL	R6	2
      0x1C180C01,  //  0018  EQ	R6	R6	R1
      0x781A0006,  //  0019  JMPF	R6	#0021
      0x8C180110,  //  001A  GETMET	R6	R0	K16
      0x7C180200,  //  001B  CALL	R6	1
      0x8C180110,  //  001C  GETMET	R6	R0	K16
      0x7C180200,  //  001D  CALL	R6	1
      0x8C180110,  //  001E  GETMET	R6	R0	K16
      0x7C180200,  //  001F  CALL	R6	1
      0x7002000A,  //  0020  JMP		#002C
      0x8C180110,  //  0021  GETMET	R6	R0	K16
      0x7C180200,  //  0022  CALL	R6	1
      0x5C140C00,  //  0023  MOVE	R5	R6
      0x1C180B28,  //  0024  EQ	R6	R5	K40
      0x781A0003,  //  0025  JMPF	R6	#002A
      0x88180102,  //  0026  GETMBR	R6	R0	K2
      0x00180D03,  //  0027  ADD	R6	R6	K3
      0x90020406,  //  0028  SETMBR	R0	K2	R6
      0x90020903,  //  0029  SETMBR	R0	K4	K3
      0x00100805,  //  002A  ADD	R4	R4	R5
      0x7001FFDC,  //  002B  JMP		#0009
      0x8C140111,  //  002C  GETMET	R5	R0	K17
      0x7C140200,  //  002D  CALL	R5	1
      0x7816000E,  //  002E  JMPF	R5	#003E
      0x88140100,  //  002F  GETMBR	R5	R0	K0
      0x04140B38,  //  0030  SUB	R5	R5	K56
      0x88180100,  //  0031  GETMBR	R6	R0	K0
      0x04180D03,  //  0032  SUB	R6	R6	K3
      0x40140A06,  //  0033  CONNECT	R5	R5	R6
      0x88180114,  //  0034  GETMBR	R6	R0	K20
      0x94140C05,  //  0035  GETIDX	R5	R6	R5
      0x00180201,  //  0036  ADD	R6	R1	R1
      0x00180C01,  //  0037  ADD	R6	R6	R1
      0x1C140A06,  //  0038  EQ	R5	R5	R6
      0x74160003,  //  0039  JMPT	R5	#003E
      0x8C140116,  //  003A  GETMET	R5	R0	K22
      0x581C0039,  //  003B  LDCONST	R7	K57
      0x7C140400,  //  003C  CALL	R5	2
      0x70020006,  //  003D  JMP		#0045
      0x8C140115,  //  003E  GETMET	R5	R0	K21
      0x581C0038,  //  003F  LDCONST	R7	K56
      0x5C200800,  //  0040  MOVE	R8	R4
      0x88240100,  //  0041  GETMBR	R9	R0	K0
      0x04241202,  //  0042  SUB	R9	R9	R2
      0x7C140800,  //  0043  CALL	R5	4
      0x80040A00,  //  0044  RET	1	R5
      0x80000000,  //  0045  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: scan_operator_or_delimiter
********************************************************************/
be_local_closure(class_Lexer_scan_operator_or_delimiter,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Lexer,     /* shared constants */
    be_str_weak(scan_operator_or_delimiter),
    &be_const_str_solidified,
    ( &(const binstruction[292]) {  /* code */
      0x88080104,  //  0000  GETMBR	R2	R0	K4
      0x04080503,  //  0001  SUB	R2	R2	K3
      0x1C0C033A,  //  0002  EQ	R3	R1	K58
      0x780E0011,  //  0003  JMPF	R3	#0016
      0x8C0C013B,  //  0004  GETMET	R3	R0	K59
      0x5814003A,  //  0005  LDCONST	R5	K58
      0x7C0C0400,  //  0006  CALL	R3	2
      0x780E0006,  //  0007  JMPF	R3	#000F
      0x8C0C0115,  //  0008  GETMET	R3	R0	K21
      0x5416000E,  //  0009  LDINT	R5	15
      0x5818003C,  //  000A  LDCONST	R6	K60
      0x581C0022,  //  000B  LDCONST	R7	K34
      0x7C0C0800,  //  000C  CALL	R3	4
      0x80040600,  //  000D  RET	1	R3
      0x70020005,  //  000E  JMP		#0015
      0x8C0C0115,  //  000F  GETMET	R3	R0	K21
      0x54160007,  //  0010  LDINT	R5	8
      0x5818003A,  //  0011  LDCONST	R6	K58
      0x581C0003,  //  0012  LDCONST	R7	K3
      0x7C0C0800,  //  0013  CALL	R3	4
      0x80040600,  //  0014  RET	1	R3
      0x7002010C,  //  0015  JMP		#0123
      0x1C0C033D,  //  0016  EQ	R3	R1	K61
      0x780E0011,  //  0017  JMPF	R3	#002A
      0x8C0C013B,  //  0018  GETMET	R3	R0	K59
      0x5814003A,  //  0019  LDCONST	R5	K58
      0x7C0C0400,  //  001A  CALL	R3	2
      0x780E0006,  //  001B  JMPF	R3	#0023
      0x8C0C0115,  //  001C  GETMET	R3	R0	K21
      0x5416000F,  //  001D  LDINT	R5	16
      0x5818003E,  //  001E  LDCONST	R6	K62
      0x581C0022,  //  001F  LDCONST	R7	K34
      0x7C0C0800,  //  0020  CALL	R3	4
      0x80040600,  //  0021  RET	1	R3
      0x70020005,  //  0022  JMP		#0029
      0x8C0C0115,  //  0023  GETMET	R3	R0	K21
      0x54160016,  //  0024  LDINT	R5	23
      0x5818003D,  //  0025  LDCONST	R6	K61
      0x581C0003,  //  0026  LDCONST	R7	K3
      0x7C0C0800,  //  0027  CALL	R3	4
      0x80040600,  //  0028  RET	1	R3
      0x700200F8,  //  0029  JMP		#0123
      0x1C0C033F,  //  002A  EQ	R3	R1	K63
      0x780E0019,  //  002B  JMPF	R3	#0046
      0x8C0C013B,  //  002C  GETMET	R3	R0	K59
      0x5814003A,  //  002D  LDCONST	R5	K58
      0x7C0C0400,  //  002E  CALL	R3	2
      0x780E0006,  //  002F  JMPF	R3	#0037
      0x8C0C0115,  //  0030  GETMET	R3	R0	K21
      0x54160011,  //  0031  LDINT	R5	18
      0x58180040,  //  0032  LDCONST	R6	K64
      0x581C0022,  //  0033  LDCONST	R7	K34
      0x7C0C0800,  //  0034  CALL	R3	4
      0x80040600,  //  0035  RET	1	R3
      0x7002000D,  //  0036  JMP		#0045
      0x8C0C013B,  //  0037  GETMET	R3	R0	K59
      0x5814003F,  //  0038  LDCONST	R5	K63
      0x7C0C0400,  //  0039  CALL	R3	2
      0x780E0003,  //  003A  JMPF	R3	#003F
      0x8C0C0116,  //  003B  GETMET	R3	R0	K22
      0x58140041,  //  003C  LDCONST	R5	K65
      0x7C0C0400,  //  003D  CALL	R3	2
      0x70020005,  //  003E  JMP		#0045
      0x8C0C0115,  //  003F  GETMET	R3	R0	K21
      0x54160010,  //  0040  LDINT	R5	17
      0x5818003F,  //  0041  LDCONST	R6	K63
      0x581C0003,  //  0042  LDCONST	R7	K3
      0x7C0C0800,  //  0043  CALL	R3	4
      0x80040600,  //  0044  RET	1	R3
      0x700200DC,  //  0045  JMP		#0123
      0x1C0C0342,  //  0046  EQ	R3	R1	K66
      0x780E0019,  //  0047  JMPF	R3	#0062
      0x8C0C013B,  //  0048  GETMET	R3	R0	K59
      0x5814003A,  //  0049  LDCONST	R5	K58
      0x7C0C0400,  //  004A  CALL	R3	2
      0x780E0006,  //  004B  JMPF	R3	#0053
      0x8C0C0115,  //  004C  GETMET	R3	R0	K21
      0x54160013,  //  004D  LDINT	R5	20
      0x58180043,  //  004E  LDCONST	R6	K67
      0x581C0022,  //  004F  LDCONST	R7	K34
      0x7C0C0800,  //  0050  CALL	R3	4
      0x80040600,  //  0051  RET	1	R3
      0x7002000D,  //  0052  JMP		#0061
      0x8C0C013B,  //  0053  GETMET	R3	R0	K59
      0x58140042,  //  0054  LDCONST	R5	K66
      0x7C0C0400,  //  0055  CALL	R3	2
      0x780E0003,  //  0056  JMPF	R3	#005B
      0x8C0C0116,  //  0057  GETMET	R3	R0	K22
      0x58140044,  //  0058  LDCONST	R5	K68
      0x7C0C0400,  //  0059  CALL	R3	2
      0x70020005,  //  005A  JMP		#0061
      0x8C0C0115,  //  005B  GETMET	R3	R0	K21
      0x54160012,  //  005C  LDINT	R5	19
      0x58180042,  //  005D  LDCONST	R6	K66
      0x581C0003,  //  005E  LDCONST	R7	K3
      0x7C0C0800,  //  005F  CALL	R3	4
      0x80040600,  //  0060  RET	1	R3
      0x700200C0,  //  0061  JMP		#0123
      0x1C0C0345,  //  0062  EQ	R3	R1	K69
      0x780E000E,  //  0063  JMPF	R3	#0073
      0x8C0C013B,  //  0064  GETMET	R3	R0	K59
      0x58140045,  //  0065  LDCONST	R5	K69
      0x7C0C0400,  //  0066  CALL	R3	2
      0x780E0006,  //  0067  JMPF	R3	#006F
      0x8C0C0115,  //  0068  GETMET	R3	R0	K21
      0x54160014,  //  0069  LDINT	R5	21
      0x58180046,  //  006A  LDCONST	R6	K70
      0x581C0022,  //  006B  LDCONST	R7	K34
      0x7C0C0800,  //  006C  CALL	R3	4
      0x80040600,  //  006D  RET	1	R3
      0x70020002,  //  006E  JMP		#0072
      0x8C0C0116,  //  006F  GETMET	R3	R0	K22
      0x58140047,  //  0070  LDCONST	R5	K71
      0x7C0C0400,  //  0071  CALL	R3	2
      0x700200AF,  //  0072  JMP		#0123
      0x1C0C0348,  //  0073  EQ	R3	R1	K72
      0x780E000E,  //  0074  JMPF	R3	#0084
      0x8C0C013B,  //  0075  GETMET	R3	R0	K59
      0x58140048,  //  0076  LDCONST	R5	K72
      0x7C0C0400,  //  0077  CALL	R3	2
      0x780E0006,  //  0078  JMPF	R3	#0080
      0x8C0C0115,  //  0079  GETMET	R3	R0	K21
      0x54160015,  //  007A  LDINT	R5	22
      0x58180049,  //  007B  LDCONST	R6	K73
      0x581C0022,  //  007C  LDCONST	R7	K34
      0x7C0C0800,  //  007D  CALL	R3	4
      0x80040600,  //  007E  RET	1	R3
      0x70020002,  //  007F  JMP		#0083
      0x8C0C0116,  //  0080  GETMET	R3	R0	K22
      0x5814004A,  //  0081  LDCONST	R5	K74
      0x7C0C0400,  //  0082  CALL	R3	2
      0x7002009E,  //  0083  JMP		#0123
      0x1C0C034B,  //  0084  EQ	R3	R1	K75
      0x780E0011,  //  0085  JMPF	R3	#0098
      0x8C0C013B,  //  0086  GETMET	R3	R0	K59
      0x58140042,  //  0087  LDCONST	R5	K66
      0x7C0C0400,  //  0088  CALL	R3	2
      0x780E0006,  //  0089  JMPF	R3	#0091
      0x8C0C0115,  //  008A  GETMET	R3	R0	K21
      0x54160021,  //  008B  LDINT	R5	34
      0x5818004C,  //  008C  LDCONST	R6	K76
      0x581C0022,  //  008D  LDCONST	R7	K34
      0x7C0C0800,  //  008E  CALL	R3	4
      0x80040600,  //  008F  RET	1	R3
      0x70020005,  //  0090  JMP		#0097
      0x8C0C0115,  //  0091  GETMET	R3	R0	K21
      0x54160009,  //  0092  LDINT	R5	10
      0x5818004B,  //  0093  LDCONST	R6	K75
      0x581C0003,  //  0094  LDCONST	R7	K3
      0x7C0C0800,  //  0095  CALL	R3	4
      0x80040600,  //  0096  RET	1	R3
      0x7002008A,  //  0097  JMP		#0123
      0x1C0C034D,  //  0098  EQ	R3	R1	K77
      0x780E0006,  //  0099  JMPF	R3	#00A1
      0x8C0C0115,  //  009A  GETMET	R3	R0	K21
      0x54160008,  //  009B  LDINT	R5	9
      0x5818004D,  //  009C  LDCONST	R6	K77
      0x581C0003,  //  009D  LDCONST	R7	K3
      0x7C0C0800,  //  009E  CALL	R3	4
      0x80040600,  //  009F  RET	1	R3
      0x70020081,  //  00A0  JMP		#0123
      0x1C0C034E,  //  00A1  EQ	R3	R1	K78
      0x780E0006,  //  00A2  JMPF	R3	#00AA
      0x8C0C0115,  //  00A3  GETMET	R3	R0	K21
      0x5416000A,  //  00A4  LDINT	R5	11
      0x5818004E,  //  00A5  LDCONST	R6	K78
      0x581C0003,  //  00A6  LDCONST	R7	K3
      0x7C0C0800,  //  00A7  CALL	R3	4
      0x80040600,  //  00A8  RET	1	R3
      0x70020078,  //  00A9  JMP		#0123
      0x1C0C034F,  //  00AA  EQ	R3	R1	K79
      0x780E0006,  //  00AB  JMPF	R3	#00B3
      0x8C0C0115,  //  00AC  GETMET	R3	R0	K21
      0x5416000B,  //  00AD  LDINT	R5	12
      0x5818004F,  //  00AE  LDCONST	R6	K79
      0x581C0003,  //  00AF  LDCONST	R7	K3
      0x7C0C0800,  //  00B0  CALL	R3	4
      0x80040600,  //  00B1  RET	1	R3
      0x7002006F,  //  00B2  JMP		#0123
      0x1C0C0320,  //  00B3  EQ	R3	R1	K32
      0x780E0006,  //  00B4  JMPF	R3	#00BC
      0x8C0C0115,  //  00B5  GETMET	R3	R0	K21
      0x5416000C,  //  00B6  LDINT	R5	13
      0x58180020,  //  00B7  LDCONST	R6	K32
      0x581C0003,  //  00B8  LDCONST	R7	K3
      0x7C0C0800,  //  00B9  CALL	R3	4
      0x80040600,  //  00BA  RET	1	R3
      0x70020066,  //  00BB  JMP		#0123
      0x1C0C0350,  //  00BC  EQ	R3	R1	K80
      0x780E0006,  //  00BD  JMPF	R3	#00C5
      0x8C0C0115,  //  00BE  GETMET	R3	R0	K21
      0x5416000D,  //  00BF  LDINT	R5	14
      0x58180050,  //  00C0  LDCONST	R6	K80
      0x581C0003,  //  00C1  LDCONST	R7	K3
      0x7C0C0800,  //  00C2  CALL	R3	4
      0x80040600,  //  00C3  RET	1	R3
      0x7002005D,  //  00C4  JMP		#0123
      0x1C0C0351,  //  00C5  EQ	R3	R1	K81
      0x780E0006,  //  00C6  JMPF	R3	#00CE
      0x8C0C0115,  //  00C7  GETMET	R3	R0	K21
      0x54160017,  //  00C8  LDINT	R5	24
      0x58180051,  //  00C9  LDCONST	R6	K81
      0x581C0003,  //  00CA  LDCONST	R7	K3
      0x7C0C0800,  //  00CB  CALL	R3	4
      0x80040600,  //  00CC  RET	1	R3
      0x70020054,  //  00CD  JMP		#0123
      0x1C0C0352,  //  00CE  EQ	R3	R1	K82
      0x780E0006,  //  00CF  JMPF	R3	#00D7
      0x8C0C0115,  //  00D0  GETMET	R3	R0	K21
      0x54160018,  //  00D1  LDINT	R5	25
      0x58180052,  //  00D2  LDCONST	R6	K82
      0x581C0003,  //  00D3  LDCONST	R7	K3
      0x7C0C0800,  //  00D4  CALL	R3	4
      0x80040600,  //  00D5  RET	1	R3
      0x7002004B,  //  00D6  JMP		#0123
      0x1C0C0353,  //  00D7  EQ	R3	R1	K83
      0x780E0006,  //  00D8  JMPF	R3	#00E0
      0x8C0C0115,  //  00D9  GETMET	R3	R0	K21
      0x54160019,  //  00DA  LDINT	R5	26
      0x58180053,  //  00DB  LDCONST	R6	K83
      0x581C0003,  //  00DC  LDCONST	R7	K3
      0x7C0C0800,  //  00DD  CALL	R3	4
      0x80040600,  //  00DE  RET	1	R3
      0x70020042,  //  00DF  JMP		#0123
      0x1C0C0354,  //  00E0  EQ	R3	R1	K84
      0x780E0006,  //  00E1  JMPF	R3	#00E9
      0x8C0C0115,  //  00E2  GETMET	R3	R0	K21
      0x5416001A,  //  00E3  LDINT	R5	27
      0x58180054,  //  00E4  LDCONST	R6	K84
      0x581C0003,  //  00E5  LDCONST	R7	K3
      0x7C0C0800,  //  00E6  CALL	R3	4
      0x80040600,  //  00E7  RET	1	R3
      0x70020039,  //  00E8  JMP		#0123
      0x1C0C0355,  //  00E9  EQ	R3	R1	K85
      0x780E0006,  //  00EA  JMPF	R3	#00F2
      0x8C0C0115,  //  00EB  GETMET	R3	R0	K21
      0x5416001B,  //  00EC  LDINT	R5	28
      0x58180055,  //  00ED  LDCONST	R6	K85
      0x581C0003,  //  00EE  LDCONST	R7	K3
      0x7C0C0800,  //  00EF  CALL	R3	4
      0x80040600,  //  00F0  RET	1	R3
      0x70020030,  //  00F1  JMP		#0123
      0x1C0C0356,  //  00F2  EQ	R3	R1	K86
      0x780E0006,  //  00F3  JMPF	R3	#00FB
      0x8C0C0115,  //  00F4  GETMET	R3	R0	K21
      0x5416001C,  //  00F5  LDINT	R5	29
      0x58180056,  //  00F6  LDCONST	R6	K86
      0x581C0003,  //  00F7  LDCONST	R7	K3
      0x7C0C0800,  //  00F8  CALL	R3	4
      0x80040600,  //  00F9  RET	1	R3
      0x70020027,  //  00FA  JMP		#0123
      0x1C0C0357,  //  00FB  EQ	R3	R1	K87
      0x780E0006,  //  00FC  JMPF	R3	#0104
      0x8C0C0115,  //  00FD  GETMET	R3	R0	K21
      0x5416001D,  //  00FE  LDINT	R5	30
      0x58180057,  //  00FF  LDCONST	R6	K87
      0x581C0003,  //  0100  LDCONST	R7	K3
      0x7C0C0800,  //  0101  CALL	R3	4
      0x80040600,  //  0102  RET	1	R3
      0x7002001E,  //  0103  JMP		#0123
      0x1C0C0358,  //  0104  EQ	R3	R1	K88
      0x780E0006,  //  0105  JMPF	R3	#010D
      0x8C0C0115,  //  0106  GETMET	R3	R0	K21
      0x5416001E,  //  0107  LDINT	R5	31
      0x58180058,  //  0108  LDCONST	R6	K88
      0x581C0003,  //  0109  LDCONST	R7	K3
      0x7C0C0800,  //  010A  CALL	R3	4
      0x80040600,  //  010B  RET	1	R3
      0x70020015,  //  010C  JMP		#0123
      0x1C0C030B,  //  010D  EQ	R3	R1	K11
      0x780E0006,  //  010E  JMPF	R3	#0116
      0x8C0C0115,  //  010F  GETMET	R3	R0	K21
      0x5416001F,  //  0110  LDINT	R5	32
      0x5818000B,  //  0111  LDCONST	R6	K11
      0x581C0003,  //  0112  LDCONST	R7	K3
      0x7C0C0800,  //  0113  CALL	R3	4
      0x80040600,  //  0114  RET	1	R3
      0x7002000C,  //  0115  JMP		#0123
      0x1C0C031D,  //  0116  EQ	R3	R1	K29
      0x780E0006,  //  0117  JMPF	R3	#011F
      0x8C0C0115,  //  0118  GETMET	R3	R0	K21
      0x54160020,  //  0119  LDINT	R5	33
      0x5818001D,  //  011A  LDCONST	R6	K29
      0x581C0003,  //  011B  LDCONST	R7	K3
      0x7C0C0800,  //  011C  CALL	R3	4
      0x80040600,  //  011D  RET	1	R3
      0x70020003,  //  011E  JMP		#0123
      0x8C0C0116,  //  011F  GETMET	R3	R0	K22
      0x0016B201,  //  0120  ADD	R5	K89	R1
      0x00140B31,  //  0121  ADD	R5	R5	K49
      0x7C0C0400,  //  0122  CALL	R3	2
      0x80000000,  //  0123  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: scan_time_suffix
********************************************************************/
be_local_closure(class_Lexer_scan_time_suffix,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Lexer,     /* shared constants */
    be_str_weak(scan_time_suffix),
    &be_const_str_solidified,
    ( &(const binstruction[39]) {  /* code */
      0xA406B400,  //  0000  IMPORT	R1	K90
      0x8C08035B,  //  0001  GETMET	R2	R1	K91
      0x88100100,  //  0002  GETMBR	R4	R0	K0
      0x4010095C,  //  0003  CONNECT	R4	R4	K92
      0x88140114,  //  0004  GETMBR	R5	R0	K20
      0x94100A04,  //  0005  GETIDX	R4	R5	R4
      0x5814005D,  //  0006  LDCONST	R5	K93
      0x7C080600,  //  0007  CALL	R2	3
      0x780A0005,  //  0008  JMPF	R2	#000F
      0x8C080110,  //  0009  GETMET	R2	R0	K16
      0x7C080200,  //  000A  CALL	R2	1
      0x8C080110,  //  000B  GETMET	R2	R0	K16
      0x7C080200,  //  000C  CALL	R2	1
      0x8006BA00,  //  000D  RET	1	K93
      0x70020016,  //  000E  JMP		#0026
      0x8C080113,  //  000F  GETMET	R2	R0	K19
      0x7C080200,  //  0010  CALL	R2	1
      0x1C08055E,  //  0011  EQ	R2	R2	K94
      0x780A0003,  //  0012  JMPF	R2	#0017
      0x8C080110,  //  0013  GETMET	R2	R0	K16
      0x7C080200,  //  0014  CALL	R2	1
      0x8006BC00,  //  0015  RET	1	K94
      0x7002000E,  //  0016  JMP		#0026
      0x8C080113,  //  0017  GETMET	R2	R0	K19
      0x7C080200,  //  0018  CALL	R2	1
      0x1C08055F,  //  0019  EQ	R2	R2	K95
      0x780A0003,  //  001A  JMPF	R2	#001F
      0x8C080110,  //  001B  GETMET	R2	R0	K16
      0x7C080200,  //  001C  CALL	R2	1
      0x8006BE00,  //  001D  RET	1	K95
      0x70020006,  //  001E  JMP		#0026
      0x8C080113,  //  001F  GETMET	R2	R0	K19
      0x7C080200,  //  0020  CALL	R2	1
      0x1C080560,  //  0021  EQ	R2	R2	K96
      0x780A0002,  //  0022  JMPF	R2	#0026
      0x8C080110,  //  0023  GETMET	R2	R0	K16
      0x7C080200,  //  0024  CALL	R2	1
      0x8006C000,  //  0025  RET	1	K96
      0x80063200,  //  0026  RET	1	K25
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: scan_variable_reference
********************************************************************/
be_local_closure(class_Lexer_scan_variable_reference,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Lexer,     /* shared constants */
    be_str_weak(scan_variable_reference),
    &be_const_str_solidified,
    ( &(const binstruction[50]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x04040303,  //  0001  SUB	R1	R1	K3
      0x88080104,  //  0002  GETMBR	R2	R0	K4
      0x04080503,  //  0003  SUB	R2	R2	K3
      0x8C0C0111,  //  0004  GETMET	R3	R0	K17
      0x7C0C0200,  //  0005  CALL	R3	1
      0x740E000B,  //  0006  JMPT	R3	#0013
      0x8C0C012C,  //  0007  GETMET	R3	R0	K44
      0x8C140113,  //  0008  GETMET	R5	R0	K19
      0x7C140200,  //  0009  CALL	R5	1
      0x7C0C0400,  //  000A  CALL	R3	2
      0x740E0004,  //  000B  JMPT	R3	#0011
      0x8C0C0113,  //  000C  GETMET	R3	R0	K19
      0x7C0C0200,  //  000D  CALL	R3	1
      0x1C0C072D,  //  000E  EQ	R3	R3	K45
      0x740E0000,  //  000F  JMPT	R3	#0011
      0x500C0001,  //  0010  LDBOOL	R3	0	1
      0x500C0200,  //  0011  LDBOOL	R3	1	0
      0x740E0002,  //  0012  JMPT	R3	#0016
      0x8C0C0116,  //  0013  GETMET	R3	R0	K22
      0x58140061,  //  0014  LDCONST	R5	K97
      0x7C0C0400,  //  0015  CALL	R3	2
      0x8C0C0111,  //  0016  GETMET	R3	R0	K17
      0x7C0C0200,  //  0017  CALL	R3	1
      0x740E000B,  //  0018  JMPT	R3	#0025
      0x8C0C0162,  //  0019  GETMET	R3	R0	K98
      0x8C140113,  //  001A  GETMET	R5	R0	K19
      0x7C140200,  //  001B  CALL	R5	1
      0x7C0C0400,  //  001C  CALL	R3	2
      0x740E0003,  //  001D  JMPT	R3	#0022
      0x8C0C0113,  //  001E  GETMET	R3	R0	K19
      0x7C0C0200,  //  001F  CALL	R3	1
      0x1C0C072D,  //  0020  EQ	R3	R3	K45
      0x780E0002,  //  0021  JMPF	R3	#0025
      0x8C0C0110,  //  0022  GETMET	R3	R0	K16
      0x7C0C0200,  //  0023  CALL	R3	1
      0x7001FFF0,  //  0024  JMP		#0016
      0x880C0100,  //  0025  GETMBR	R3	R0	K0
      0x040C0703,  //  0026  SUB	R3	R3	K3
      0x400C0203,  //  0027  CONNECT	R3	R1	R3
      0x88100114,  //  0028  GETMBR	R4	R0	K20
      0x940C0803,  //  0029  GETIDX	R3	R4	R3
      0x8C100115,  //  002A  GETMET	R4	R0	K21
      0x541A0023,  //  002B  LDINT	R6	36
      0x5C1C0600,  //  002C  MOVE	R7	R3
      0x6020000C,  //  002D  GETGBL	R8	G12
      0x5C240600,  //  002E  MOVE	R9	R3
      0x7C200200,  //  002F  CALL	R8	1
      0x7C100800,  //  0030  CALL	R4	4
      0x80040800,  //  0031  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: scan_comment
********************************************************************/
be_local_closure(class_Lexer_scan_comment,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Lexer,     /* shared constants */
    be_str_weak(scan_comment),
    &be_const_str_solidified,
    ( &(const binstruction[50]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x04040303,  //  0001  SUB	R1	R1	K3
      0x88080104,  //  0002  GETMBR	R2	R0	K4
      0x04080503,  //  0003  SUB	R2	R2	K3
      0x8C0C0111,  //  0004  GETMET	R3	R0	K17
      0x7C0C0200,  //  0005  CALL	R3	1
      0x740E0006,  //  0006  JMPT	R3	#000E
      0x8C0C0113,  //  0007  GETMET	R3	R0	K19
      0x7C0C0200,  //  0008  CALL	R3	1
      0x200C0728,  //  0009  NE	R3	R3	K40
      0x780E0002,  //  000A  JMPF	R3	#000E
      0x8C0C0110,  //  000B  GETMET	R3	R0	K16
      0x7C0C0200,  //  000C  CALL	R3	1
      0x7001FFF5,  //  000D  JMP		#0004
      0x880C0100,  //  000E  GETMBR	R3	R0	K0
      0x040C0703,  //  000F  SUB	R3	R3	K3
      0x400C0203,  //  0010  CONNECT	R3	R1	R3
      0x88100114,  //  0011  GETMBR	R4	R0	K20
      0x940C0803,  //  0012  GETIDX	R3	R4	R3
      0x5C100600,  //  0013  MOVE	R4	R3
      0x6014000C,  //  0014  GETGBL	R5	G12
      0x5C180600,  //  0015  MOVE	R6	R3
      0x7C140200,  //  0016  CALL	R5	1
      0x04140B03,  //  0017  SUB	R5	R5	K3
      0x28180B01,  //  0018  GE	R6	R5	K1
      0x781A000A,  //  0019  JMPF	R6	#0025
      0x94180605,  //  001A  GETIDX	R6	R3	R5
      0x1C180D25,  //  001B  EQ	R6	R6	K37
      0x741A0005,  //  001C  JMPT	R6	#0023
      0x94180605,  //  001D  GETIDX	R6	R3	R5
      0x1C180D26,  //  001E  EQ	R6	R6	K38
      0x741A0002,  //  001F  JMPT	R6	#0023
      0x94180605,  //  0020  GETIDX	R6	R3	R5
      0x1C180D27,  //  0021  EQ	R6	R6	K39
      0x781A0001,  //  0022  JMPF	R6	#0025
      0x04140B03,  //  0023  SUB	R5	R5	K3
      0x7001FFF2,  //  0024  JMP		#0018
      0x28180B01,  //  0025  GE	R6	R5	K1
      0x781A0002,  //  0026  JMPF	R6	#002A
      0x401A0205,  //  0027  CONNECT	R6	K1	R5
      0x94100606,  //  0028  GETIDX	R4	R3	R6
      0x70020000,  //  0029  JMP		#002B
      0x58100029,  //  002A  LDCONST	R4	K41
      0x8C180115,  //  002B  GETMET	R6	R0	K21
      0x54220024,  //  002C  LDINT	R8	37
      0x5C240800,  //  002D  MOVE	R9	R4
      0x88280100,  //  002E  GETMBR	R10	R0	K0
      0x04281401,  //  002F  SUB	R10	R10	R1
      0x7C180800,  //  0030  CALL	R6	4
      0x80040C00,  //  0031  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_position
********************************************************************/
be_local_closure(class_Lexer_set_position,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Lexer,     /* shared constants */
    be_str_weak(set_position),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x14080301,  //  0000  LT	R2	R1	K1
      0x780A0000,  //  0001  JMPF	R2	#0003
      0x80000400,  //  0002  RET	0
      0x88080100,  //  0003  GETMBR	R2	R0	K0
      0x880C0102,  //  0004  GETMBR	R3	R0	K2
      0x88100104,  //  0005  GETMBR	R4	R0	K4
      0x88140105,  //  0006  GETMBR	R5	R0	K5
      0x90020101,  //  0007  SETMBR	R0	K0	K1
      0x90020503,  //  0008  SETMBR	R0	K2	K3
      0x90020903,  //  0009  SETMBR	R0	K4	K3
      0x90020B01,  //  000A  SETMBR	R0	K5	K1
      0x88180105,  //  000B  GETMBR	R6	R0	K5
      0x14180C01,  //  000C  LT	R6	R6	R1
      0x781A0005,  //  000D  JMPF	R6	#0014
      0x8C180111,  //  000E  GETMET	R6	R0	K17
      0x7C180200,  //  000F  CALL	R6	1
      0x741A0002,  //  0010  JMPT	R6	#0014
      0x8C180123,  //  0011  GETMET	R6	R0	K35
      0x7C180200,  //  0012  CALL	R6	1
      0x7001FFF6,  //  0013  JMP		#000B
      0x88180105,  //  0014  GETMBR	R6	R0	K5
      0x20180C01,  //  0015  NE	R6	R6	R1
      0x781A0003,  //  0016  JMPF	R6	#001B
      0x90020002,  //  0017  SETMBR	R0	K0	R2
      0x90020403,  //  0018  SETMBR	R0	K2	R3
      0x90020804,  //  0019  SETMBR	R0	K4	R4
      0x90020A05,  //  001A  SETMBR	R0	K5	R5
      0x80000000,  //  001B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_hex_digit
********************************************************************/
be_local_closure(class_Lexer_is_hex_digit,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Lexer,     /* shared constants */
    be_str_weak(is_hex_digit),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x8C08011C,  //  0000  GETMET	R2	R0	K28
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x740A0008,  //  0003  JMPT	R2	#000D
      0x28080306,  //  0004  GE	R2	R1	K6
      0x780A0001,  //  0005  JMPF	R2	#0008
      0x18080363,  //  0006  LE	R2	R1	K99
      0x740A0004,  //  0007  JMPT	R2	#000D
      0x28080308,  //  0008  GE	R2	R1	K8
      0x780A0001,  //  0009  JMPF	R2	#000C
      0x18080364,  //  000A  LE	R2	R1	K100
      0x740A0000,  //  000B  JMPT	R2	#000D
      0x50080001,  //  000C  LDBOOL	R2	0	1
      0x50080200,  //  000D  LDBOOL	R2	1	0
      0x80040400,  //  000E  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_alnum
********************************************************************/
be_local_closure(class_Lexer_is_alnum,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Lexer,     /* shared constants */
    be_str_weak(is_alnum),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x8C08012C,  //  0000  GETMET	R2	R0	K44
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x740A0004,  //  0003  JMPT	R2	#0009
      0x8C08011C,  //  0004  GETMET	R2	R0	K28
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
** Solidified function: peek_ahead
********************************************************************/
be_local_closure(class_Lexer_peek_ahead,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Lexer,     /* shared constants */
    be_str_weak(peek_ahead),
    &be_const_str_solidified,
    ( &(const binstruction[33]) {  /* code */
      0x18080301,  //  0000  LE	R2	R1	K1
      0x780A0001,  //  0001  JMPF	R2	#0004
      0x4C080000,  //  0002  LDNIL	R2
      0x80040400,  //  0003  RET	1	R2
      0x88080100,  //  0004  GETMBR	R2	R0	K0
      0x880C0102,  //  0005  GETMBR	R3	R0	K2
      0x88100104,  //  0006  GETMBR	R4	R0	K4
      0x88140105,  //  0007  GETMBR	R5	R0	K5
      0x4C180000,  //  0008  LDNIL	R6
      0x601C0010,  //  0009  GETGBL	R7	G16
      0x40220601,  //  000A  CONNECT	R8	K3	R1
      0x7C1C0200,  //  000B  CALL	R7	1
      0xA802000B,  //  000C  EXBLK	0	#0019
      0x5C200E00,  //  000D  MOVE	R8	R7
      0x7C200000,  //  000E  CALL	R8	0
      0x8C240123,  //  000F  GETMET	R9	R0	K35
      0x7C240200,  //  0010  CALL	R9	1
      0x5C181200,  //  0011  MOVE	R6	R9
      0x4C240000,  //  0012  LDNIL	R9
      0x1C240C09,  //  0013  EQ	R9	R6	R9
      0x78260000,  //  0014  JMPF	R9	#0016
      0x70020000,  //  0015  JMP		#0017
      0x7001FFF5,  //  0016  JMP		#000D
      0xA8040001,  //  0017  EXBLK	1	1
      0x70020002,  //  0018  JMP		#001C
      0x581C0065,  //  0019  LDCONST	R7	K101
      0xAC1C0200,  //  001A  CATCH	R7	1	0
      0xB0080000,  //  001B  RAISE	2	R0	R0
      0x90020002,  //  001C  SETMBR	R0	K0	R2
      0x90020403,  //  001D  SETMBR	R0	K2	R3
      0x90020804,  //  001E  SETMBR	R0	K4	R4
      0x90020A05,  //  001F  SETMBR	R0	K5	R5
      0x80040C00,  //  0020  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: check_time_suffix
********************************************************************/
be_local_closure(class_Lexer_check_time_suffix,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Lexer,     /* shared constants */
    be_str_weak(check_time_suffix),
    &be_const_str_solidified,
    ( &(const binstruction[33]) {  /* code */
      0xA406B400,  //  0000  IMPORT	R1	K90
      0x8C080111,  //  0001  GETMET	R2	R0	K17
      0x7C080200,  //  0002  CALL	R2	1
      0x780A0001,  //  0003  JMPF	R2	#0006
      0x50080000,  //  0004  LDBOOL	R2	0	0
      0x80040400,  //  0005  RET	1	R2
      0x88080100,  //  0006  GETMBR	R2	R0	K0
      0x4008055C,  //  0007  CONNECT	R2	R2	K92
      0x880C0114,  //  0008  GETMBR	R3	R0	K20
      0x94080602,  //  0009  GETIDX	R2	R3	R2
      0x8C0C035B,  //  000A  GETMET	R3	R1	K91
      0x5C140400,  //  000B  MOVE	R5	R2
      0x5818005D,  //  000C  LDCONST	R6	K93
      0x7C0C0600,  //  000D  CALL	R3	3
      0x740E000F,  //  000E  JMPT	R3	#001F
      0x8C0C035B,  //  000F  GETMET	R3	R1	K91
      0x5C140400,  //  0010  MOVE	R5	R2
      0x5818005E,  //  0011  LDCONST	R6	K94
      0x7C0C0600,  //  0012  CALL	R3	3
      0x740E000A,  //  0013  JMPT	R3	#001F
      0x8C0C035B,  //  0014  GETMET	R3	R1	K91
      0x5C140400,  //  0015  MOVE	R5	R2
      0x5818005F,  //  0016  LDCONST	R6	K95
      0x7C0C0600,  //  0017  CALL	R3	3
      0x740E0005,  //  0018  JMPT	R3	#001F
      0x8C0C035B,  //  0019  GETMET	R3	R1	K91
      0x5C140400,  //  001A  MOVE	R5	R2
      0x58180060,  //  001B  LDCONST	R6	K96
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
** Solidified function: scan_string
********************************************************************/
be_local_closure(class_Lexer_scan_string,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Lexer,     /* shared constants */
    be_str_weak(scan_string),
    &be_const_str_solidified,
    ( &(const binstruction[73]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x04080503,  //  0001  SUB	R2	R2	K3
      0x880C0104,  //  0002  GETMBR	R3	R0	K4
      0x040C0703,  //  0003  SUB	R3	R3	K3
      0x58100019,  //  0004  LDCONST	R4	K25
      0x8C140111,  //  0005  GETMET	R5	R0	K17
      0x7C140200,  //  0006  CALL	R5	1
      0x7416002F,  //  0007  JMPT	R5	#0038
      0x8C140113,  //  0008  GETMET	R5	R0	K19
      0x7C140200,  //  0009  CALL	R5	1
      0x20140A01,  //  000A  NE	R5	R5	R1
      0x7816002B,  //  000B  JMPF	R5	#0038
      0x8C140110,  //  000C  GETMET	R5	R0	K16
      0x7C140200,  //  000D  CALL	R5	1
      0x1C180B66,  //  000E  EQ	R6	R5	K102
      0x781A001D,  //  000F  JMPF	R6	#002E
      0x8C180111,  //  0010  GETMET	R6	R0	K17
      0x7C180200,  //  0011  CALL	R6	1
      0x741A0018,  //  0012  JMPT	R6	#002C
      0x8C180110,  //  0013  GETMET	R6	R0	K16
      0x7C180200,  //  0014  CALL	R6	1
      0x1C1C0D67,  //  0015  EQ	R7	R6	K103
      0x781E0001,  //  0016  JMPF	R7	#0019
      0x00100928,  //  0017  ADD	R4	R4	K40
      0x70020011,  //  0018  JMP		#002B
      0x1C1C0D68,  //  0019  EQ	R7	R6	K104
      0x781E0001,  //  001A  JMPF	R7	#001D
      0x00100926,  //  001B  ADD	R4	R4	K38
      0x7002000D,  //  001C  JMP		#002B
      0x1C1C0D69,  //  001D  EQ	R7	R6	K105
      0x781E0001,  //  001E  JMPF	R7	#0021
      0x00100927,  //  001F  ADD	R4	R4	K39
      0x70020009,  //  0020  JMP		#002B
      0x1C1C0D66,  //  0021  EQ	R7	R6	K102
      0x781E0001,  //  0022  JMPF	R7	#0025
      0x00100966,  //  0023  ADD	R4	R4	K102
      0x70020005,  //  0024  JMP		#002B
      0x1C1C0C01,  //  0025  EQ	R7	R6	R1
      0x781E0001,  //  0026  JMPF	R7	#0029
      0x00100801,  //  0027  ADD	R4	R4	R1
      0x70020001,  //  0028  JMP		#002B
      0x00100966,  //  0029  ADD	R4	R4	K102
      0x00100806,  //  002A  ADD	R4	R4	R6
      0x70020000,  //  002B  JMP		#002D
      0x00100966,  //  002C  ADD	R4	R4	K102
      0x70020008,  //  002D  JMP		#0037
      0x1C180B28,  //  002E  EQ	R6	R5	K40
      0x781A0005,  //  002F  JMPF	R6	#0036
      0x88180102,  //  0030  GETMBR	R6	R0	K2
      0x00180D03,  //  0031  ADD	R6	R6	K3
      0x90020406,  //  0032  SETMBR	R0	K2	R6
      0x90020903,  //  0033  SETMBR	R0	K4	K3
      0x00100805,  //  0034  ADD	R4	R4	R5
      0x70020000,  //  0035  JMP		#0037
      0x00100805,  //  0036  ADD	R4	R4	R5
      0x7001FFCC,  //  0037  JMP		#0005
      0x8C140111,  //  0038  GETMET	R5	R0	K17
      0x7C140200,  //  0039  CALL	R5	1
      0x78160003,  //  003A  JMPF	R5	#003F
      0x8C140116,  //  003B  GETMET	R5	R0	K22
      0x581C006A,  //  003C  LDCONST	R7	K106
      0x7C140400,  //  003D  CALL	R5	2
      0x70020008,  //  003E  JMP		#0048
      0x8C140110,  //  003F  GETMET	R5	R0	K16
      0x7C140200,  //  0040  CALL	R5	1
      0x8C140115,  //  0041  GETMET	R5	R0	K21
      0x581C0038,  //  0042  LDCONST	R7	K56
      0x5C200800,  //  0043  MOVE	R8	R4
      0x88240100,  //  0044  GETMBR	R9	R0	K0
      0x04241202,  //  0045  SUB	R9	R9	R2
      0x7C140800,  //  0046  CALL	R5	4
      0x80040A00,  //  0047  RET	1	R5
      0x80000000,  //  0048  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: scan_identifier_or_keyword
********************************************************************/
be_local_closure(class_Lexer_scan_identifier_or_keyword,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Lexer,     /* shared constants */
    be_str_weak(scan_identifier_or_keyword),
    &be_const_str_solidified,
    ( &(const binstruction[47]) {  /* code */
      0xA4063400,  //  0000  IMPORT	R1	K26
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x04080503,  //  0002  SUB	R2	R2	K3
      0x880C0104,  //  0003  GETMBR	R3	R0	K4
      0x040C0703,  //  0004  SUB	R3	R3	K3
      0x8C100111,  //  0005  GETMET	R4	R0	K17
      0x7C100200,  //  0006  CALL	R4	1
      0x7412000B,  //  0007  JMPT	R4	#0014
      0x8C100162,  //  0008  GETMET	R4	R0	K98
      0x8C180113,  //  0009  GETMET	R6	R0	K19
      0x7C180200,  //  000A  CALL	R6	1
      0x7C100400,  //  000B  CALL	R4	2
      0x74120003,  //  000C  JMPT	R4	#0011
      0x8C100113,  //  000D  GETMET	R4	R0	K19
      0x7C100200,  //  000E  CALL	R4	1
      0x1C10092D,  //  000F  EQ	R4	R4	K45
      0x78120002,  //  0010  JMPF	R4	#0014
      0x8C100110,  //  0011  GETMET	R4	R0	K16
      0x7C100200,  //  0012  CALL	R4	1
      0x7001FFF0,  //  0013  JMP		#0005
      0x88100100,  //  0014  GETMBR	R4	R0	K0
      0x04100903,  //  0015  SUB	R4	R4	K3
      0x40100404,  //  0016  CONNECT	R4	R2	R4
      0x88140114,  //  0017  GETMBR	R5	R0	K20
      0x94100A04,  //  0018  GETIDX	R4	R5	R4
      0x4C140000,  //  0019  LDNIL	R5
      0x8C18036B,  //  001A  GETMET	R6	R1	K107
      0x5C200800,  //  001B  MOVE	R8	R4
      0x7C180400,  //  001C  CALL	R6	2
      0x781A0001,  //  001D  JMPF	R6	#0020
      0x54160003,  //  001E  LDINT	R5	4
      0x70020006,  //  001F  JMP		#0027
      0x8C18036C,  //  0020  GETMET	R6	R1	K108
      0x5C200800,  //  0021  MOVE	R8	R4
      0x7C180400,  //  0022  CALL	R6	2
      0x781A0001,  //  0023  JMPF	R6	#0026
      0x58140001,  //  0024  LDCONST	R5	K1
      0x70020000,  //  0025  JMP		#0027
      0x58140003,  //  0026  LDCONST	R5	K3
      0x8C180115,  //  0027  GETMET	R6	R0	K21
      0x5C200A00,  //  0028  MOVE	R8	R5
      0x5C240800,  //  0029  MOVE	R9	R4
      0x6028000C,  //  002A  GETGBL	R10	G12
      0x5C2C0800,  //  002B  MOVE	R11	R4
      0x7C280200,  //  002C  CALL	R10	1
      0x7C180800,  //  002D  CALL	R6	4
      0x80040C00,  //  002E  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Lexer
********************************************************************/
be_local_class(Lexer,
    5,
    NULL,
    be_nested_map(34,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(reset, -1), be_const_closure(class_Lexer_reset_closure) },
        { be_const_key_weak(get_position, 27), be_const_closure(class_Lexer_get_position_closure) },
        { be_const_key_weak(is_alpha, 4), be_const_closure(class_Lexer_is_alpha_closure) },
        { be_const_key_weak(error, -1), be_const_closure(class_Lexer_error_closure) },
        { be_const_key_weak(scan_identifier_or_keyword, -1), be_const_closure(class_Lexer_scan_identifier_or_keyword_closure) },
        { be_const_key_weak(scan_hex_color_0x, -1), be_const_closure(class_Lexer_scan_hex_color_0x_closure) },
        { be_const_key_weak(peek, -1), be_const_closure(class_Lexer_peek_closure) },
        { be_const_key_weak(is_digit, 33), be_const_closure(class_Lexer_is_digit_closure) },
        { be_const_key_weak(position, -1), be_const_var(1) },
        { be_const_key_weak(create_token, -1), be_const_closure(class_Lexer_create_token_closure) },
        { be_const_key_weak(at_end, -1), be_const_closure(class_Lexer_at_end_closure) },
        { be_const_key_weak(column, 26), be_const_var(3) },
        { be_const_key_weak(token_position, 24), be_const_var(4) },
        { be_const_key_weak(source, 18), be_const_var(0) },
        { be_const_key_weak(scan_number, -1), be_const_closure(class_Lexer_scan_number_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_Lexer_init_closure) },
        { be_const_key_weak(peek_token, -1), be_const_closure(class_Lexer_peek_token_closure) },
        { be_const_key_weak(create_sub_lexer, -1), be_const_closure(class_Lexer_create_sub_lexer_closure) },
        { be_const_key_weak(next_token, -1), be_const_closure(class_Lexer_next_token_closure) },
        { be_const_key_weak(scan_operator_or_delimiter, -1), be_const_closure(class_Lexer_scan_operator_or_delimiter_closure) },
        { be_const_key_weak(scan_triple_quoted_string, -1), be_const_closure(class_Lexer_scan_triple_quoted_string_closure) },
        { be_const_key_weak(scan_variable_reference, -1), be_const_closure(class_Lexer_scan_variable_reference_closure) },
        { be_const_key_weak(scan_time_suffix, -1), be_const_closure(class_Lexer_scan_time_suffix_closure) },
        { be_const_key_weak(peek_char_ahead, 21), be_const_closure(class_Lexer_peek_char_ahead_closure) },
        { be_const_key_weak(scan_comment, -1), be_const_closure(class_Lexer_scan_comment_closure) },
        { be_const_key_weak(set_position, -1), be_const_closure(class_Lexer_set_position_closure) },
        { be_const_key_weak(is_alnum, 19), be_const_closure(class_Lexer_is_alnum_closure) },
        { be_const_key_weak(is_hex_digit, -1), be_const_closure(class_Lexer_is_hex_digit_closure) },
        { be_const_key_weak(peek_ahead, -1), be_const_closure(class_Lexer_peek_ahead_closure) },
        { be_const_key_weak(check_time_suffix, -1), be_const_closure(class_Lexer_check_time_suffix_closure) },
        { be_const_key_weak(scan_string, -1), be_const_closure(class_Lexer_scan_string_closure) },
        { be_const_key_weak(line, -1), be_const_var(2) },
        { be_const_key_weak(match, 13), be_const_closure(class_Lexer_match_closure) },
        { be_const_key_weak(advance, -1), be_const_closure(class_Lexer_advance_closure) },
    })),
    be_str_weak(Lexer)
);

/********************************************************************
** Solidified function: is_color_name
********************************************************************/
be_local_closure(is_color_name,   /* name */
  be_nested_proto(
    5,                          /* nstack */
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
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x60080010,  //  0001  GETGBL	R2	G16
      0x880C0301,  //  0002  GETMBR	R3	R1	K1
      0x880C0702,  //  0003  GETMBR	R3	R3	K2
      0x7C080200,  //  0004  CALL	R2	1
      0xA8020007,  //  0005  EXBLK	0	#000E
      0x5C0C0400,  //  0006  MOVE	R3	R2
      0x7C0C0000,  //  0007  CALL	R3	0
      0x1C100003,  //  0008  EQ	R4	R0	R3
      0x78120002,  //  0009  JMPF	R4	#000D
      0x50100200,  //  000A  LDBOOL	R4	1	0
      0xA8040001,  //  000B  EXBLK	1	1
      0x80040800,  //  000C  RET	1	R4
      0x7001FFF7,  //  000D  JMP		#0006
      0x58080003,  //  000E  LDCONST	R2	K3
      0xAC080200,  //  000F  CATCH	R2	1	0
      0xB0080000,  //  0010  RAISE	2	R0	R0
      0x50080000,  //  0011  LDBOOL	R2	0	0
      0x80040400,  //  0012  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: compile_dsl
********************************************************************/
be_local_closure(compile_dsl,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation_dsl),
    /* K1   */  be_nested_str_weak(create_lexer),
    /* K2   */  be_nested_str_weak(SimpleDSLTranspiler),
    /* K3   */  be_nested_str_weak(transpile),
    }),
    be_str_weak(compile_dsl),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x5C100000,  //  0002  MOVE	R4	R0
      0x7C080400,  //  0003  CALL	R2	2
      0x8C0C0302,  //  0004  GETMET	R3	R1	K2
      0x5C140400,  //  0005  MOVE	R5	R2
      0x7C0C0400,  //  0006  CALL	R3	2
      0x8C100703,  //  0007  GETMET	R4	R3	K3
      0x7C100200,  //  0008  CALL	R4	1
      0x80040800,  //  0009  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: animation_dsl_init
********************************************************************/
be_local_closure(animation_dsl_init,   /* name */
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
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(animation_web_ui),
    /* K2   */  be_nested_str_weak(web_ui),
    }),
    be_str_weak(animation_dsl_init),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x5C0C0400,  //  0002  MOVE	R3	R2
      0x7C0C0000,  //  0003  CALL	R3	0
      0x90060403,  //  0004  SETMBR	R1	K2	R3
      0x80040000,  //  0005  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: load_file
********************************************************************/
be_local_closure(load_file,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation_dsl),
    /* K1   */  be_nested_str_weak(r),
    /* K2   */  be_nested_str_weak(Cannot_X20open_X20DSL_X20file_X3A_X20_X25s),
    /* K3   */  be_nested_str_weak(io_error),
    /* K4   */  be_nested_str_weak(read),
    /* K5   */  be_nested_str_weak(close),
    /* K6   */  be_nested_str_weak(execute),
    }),
    be_str_weak(load_file),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x60080011,  //  0001  GETGBL	R2	G17
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x58100001,  //  0003  LDCONST	R4	K1
      0x7C080400,  //  0004  CALL	R2	2
      0x4C0C0000,  //  0005  LDNIL	R3
      0x1C0C0403,  //  0006  EQ	R3	R2	R3
      0x780E0004,  //  0007  JMPF	R3	#000D
      0x600C0018,  //  0008  GETGBL	R3	G24
      0x58100002,  //  0009  LDCONST	R4	K2
      0x5C140000,  //  000A  MOVE	R5	R0
      0x7C0C0400,  //  000B  CALL	R3	2
      0xB0060603,  //  000C  RAISE	1	K3	R3
      0x8C0C0504,  //  000D  GETMET	R3	R2	K4
      0x7C0C0200,  //  000E  CALL	R3	1
      0x8C100505,  //  000F  GETMET	R4	R2	K5
      0x7C100200,  //  0010  CALL	R4	1
      0x8C100306,  //  0011  GETMET	R4	R1	K6
      0x5C180600,  //  0012  MOVE	R6	R3
      0x7C100400,  //  0013  CALL	R4	2
      0x80040800,  //  0014  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: compile_file
********************************************************************/
be_local_closure(compile_file,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[25]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(animation_dsl),
    /* K2   */  be_nested_str_weak(endswith),
    /* K3   */  be_nested_str_weak(_X2Eanim),
    /* K4   */  be_nested_str_weak(Input_X20file_X20must_X20have_X20_X2Eanim_X20extension_X3A_X20_X25s),
    /* K5   */  be_nested_str_weak(invalid_filename),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(_X2Ebe),
    /* K8   */  be_nested_str_weak(r),
    /* K9   */  be_nested_str_weak(Cannot_X20open_X20input_X20file_X3A_X20_X25s),
    /* K10  */  be_nested_str_weak(io_error),
    /* K11  */  be_nested_str_weak(read),
    /* K12  */  be_nested_str_weak(close),
    /* K13  */  be_nested_str_weak(compile),
    /* K14  */  be_nested_str_weak(DSL_X20compilation_X20failed_X20for_X3A_X20_X25s),
    /* K15  */  be_nested_str_weak(dsl_compilation_error),
    /* K16  */  be_nested_str_weak(_X23_X20Generated_X20Berry_X20code_X20from_X20Animation_X20DSL_X0A),
    /* K17  */  be_nested_str_weak(_X23_X20Source_X3A_X20_X25s_X0A),
    /* K18  */  be_nested_str_weak(_X23_X20Generated_X20automatically_X20by_X20animation_dsl_X2Ecompile_file_X28_X29_X0A),
    /* K19  */  be_nested_str_weak(_X23_X20_X0A),
    /* K20  */  be_nested_str_weak(_X23_X20Do_X20not_X20edit_X20manually_X20_X2D_X20changes_X20will_X20be_X20overwritten_X0A),
    /* K21  */  be_nested_str_weak(_X0A),
    /* K22  */  be_nested_str_weak(w),
    /* K23  */  be_nested_str_weak(Cannot_X20create_X20output_X20file_X3A_X20_X25s),
    /* K24  */  be_nested_str_weak(write),
    }),
    be_str_weak(compile_file),
    &be_const_str_solidified,
    ( &(const binstruction[71]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x8C0C0302,  //  0002  GETMET	R3	R1	K2
      0x5C140000,  //  0003  MOVE	R5	R0
      0x58180003,  //  0004  LDCONST	R6	K3
      0x7C0C0600,  //  0005  CALL	R3	3
      0x740E0004,  //  0006  JMPT	R3	#000C
      0x600C0018,  //  0007  GETGBL	R3	G24
      0x58100004,  //  0008  LDCONST	R4	K4
      0x5C140000,  //  0009  MOVE	R5	R0
      0x7C0C0400,  //  000A  CALL	R3	2
      0xB0060A03,  //  000B  RAISE	1	K5	R3
      0x540DFFF9,  //  000C  LDINT	R3	-6
      0x400E0C03,  //  000D  CONNECT	R3	K6	R3
      0x940C0003,  //  000E  GETIDX	R3	R0	R3
      0x00100707,  //  000F  ADD	R4	R3	K7
      0x60140011,  //  0010  GETGBL	R5	G17
      0x5C180000,  //  0011  MOVE	R6	R0
      0x581C0008,  //  0012  LDCONST	R7	K8
      0x7C140400,  //  0013  CALL	R5	2
      0x4C180000,  //  0014  LDNIL	R6
      0x1C180A06,  //  0015  EQ	R6	R5	R6
      0x781A0004,  //  0016  JMPF	R6	#001C
      0x60180018,  //  0017  GETGBL	R6	G24
      0x581C0009,  //  0018  LDCONST	R7	K9
      0x5C200000,  //  0019  MOVE	R8	R0
      0x7C180400,  //  001A  CALL	R6	2
      0xB0061406,  //  001B  RAISE	1	K10	R6
      0x8C180B0B,  //  001C  GETMET	R6	R5	K11
      0x7C180200,  //  001D  CALL	R6	1
      0x8C1C0B0C,  //  001E  GETMET	R7	R5	K12
      0x7C1C0200,  //  001F  CALL	R7	1
      0x8C1C050D,  //  0020  GETMET	R7	R2	K13
      0x5C240C00,  //  0021  MOVE	R9	R6
      0x7C1C0400,  //  0022  CALL	R7	2
      0x4C200000,  //  0023  LDNIL	R8
      0x1C200E08,  //  0024  EQ	R8	R7	R8
      0x78220004,  //  0025  JMPF	R8	#002B
      0x60200018,  //  0026  GETGBL	R8	G24
      0x5824000E,  //  0027  LDCONST	R9	K14
      0x5C280000,  //  0028  MOVE	R10	R0
      0x7C200400,  //  0029  CALL	R8	2
      0xB0061E08,  //  002A  RAISE	1	K15	R8
      0x60200018,  //  002B  GETGBL	R8	G24
      0x58240011,  //  002C  LDCONST	R9	K17
      0x5C280000,  //  002D  MOVE	R10	R0
      0x7C200400,  //  002E  CALL	R8	2
      0x00222008,  //  002F  ADD	R8	K16	R8
      0x00201112,  //  0030  ADD	R8	R8	K18
      0x00201113,  //  0031  ADD	R8	R8	K19
      0x00201114,  //  0032  ADD	R8	R8	K20
      0x00201115,  //  0033  ADD	R8	R8	K21
      0x60240011,  //  0034  GETGBL	R9	G17
      0x5C280800,  //  0035  MOVE	R10	R4
      0x582C0016,  //  0036  LDCONST	R11	K22
      0x7C240400,  //  0037  CALL	R9	2
      0x4C280000,  //  0038  LDNIL	R10
      0x1C28120A,  //  0039  EQ	R10	R9	R10
      0x782A0004,  //  003A  JMPF	R10	#0040
      0x60280018,  //  003B  GETGBL	R10	G24
      0x582C0017,  //  003C  LDCONST	R11	K23
      0x5C300800,  //  003D  MOVE	R12	R4
      0x7C280400,  //  003E  CALL	R10	2
      0xB006140A,  //  003F  RAISE	1	K10	R10
      0x8C281318,  //  0040  GETMET	R10	R9	K24
      0x00301007,  //  0041  ADD	R12	R8	R7
      0x7C280400,  //  0042  CALL	R10	2
      0x8C28130C,  //  0043  GETMET	R10	R9	K12
      0x7C280200,  //  0044  CALL	R10	1
      0x50280200,  //  0045  LDBOOL	R10	1	0
      0x80041400,  //  0046  RET	1	R10
    })
  )
);
/*******************************************************************/

// ktab too big for class 'SimpleDSLTranspiler' - skipping

extern const bclass be_class_SimpleDSLTranspiler;

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
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(at_end),
    /* K1   */  be_nested_str_weak(current),
    /* K2   */  be_nested_str_weak(type),
    /* K3   */  be_nested_str_weak(next),
    }),
    be_str_weak(skip_whitespace_including_newlines),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x74060011,  //  0002  JMPT	R1	#0015
      0x8C040101,  //  0003  GETMET	R1	R0	K1
      0x7C040200,  //  0004  CALL	R1	1
      0x4C080000,  //  0005  LDNIL	R2
      0x20080202,  //  0006  NE	R2	R1	R2
      0x780A000A,  //  0007  JMPF	R2	#0013
      0x88080302,  //  0008  GETMBR	R2	R1	K2
      0x540E0024,  //  0009  LDINT	R3	37
      0x1C080403,  //  000A  EQ	R2	R2	R3
      0x740A0003,  //  000B  JMPT	R2	#0010
      0x88080302,  //  000C  GETMBR	R2	R1	K2
      0x540E0022,  //  000D  LDINT	R3	35
      0x1C080403,  //  000E  EQ	R2	R2	R3
      0x780A0002,  //  000F  JMPF	R2	#0013
      0x8C080103,  //  0010  GETMET	R2	R0	K3
      0x7C080200,  //  0011  CALL	R2	1
      0x70020000,  //  0012  JMP		#0014
      0x70020000,  //  0013  JMP		#0015
      0x7001FFEA,  //  0014  JMP		#0000
      0x80000000,  //  0015  RET	0
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
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(next),
    /* K3   */  be_nested_str_weak(error),
    /* K4   */  be_nested_str_weak(Expected_X20_X27_X28_X27),
    }),
    be_str_weak(expect_left_paren),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0006,  //  0004  JMPF	R2	#000C
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0x540E0017,  //  0006  LDINT	R3	24
      0x1C080403,  //  0007  EQ	R2	R2	R3
      0x780A0002,  //  0008  JMPF	R2	#000C
      0x8C080102,  //  0009  GETMET	R2	R0	K2
      0x7C080200,  //  000A  CALL	R2	1
      0x70020002,  //  000B  JMP		#000F
      0x8C080103,  //  000C  GETMET	R2	R0	K3
      0x58100004,  //  000D  LDCONST	R4	K4
      0x7C080400,  //  000E  CALL	R2	2
      0x80000000,  //  000F  RET	0
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
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(next),
    /* K3   */  be_nested_str_weak(error),
    /* K4   */  be_nested_str_weak(Expected_X20_X27_X7B_X27),
    }),
    be_str_weak(expect_left_brace),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0006,  //  0004  JMPF	R2	#000C
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0x540E0019,  //  0006  LDINT	R3	26
      0x1C080403,  //  0007  EQ	R2	R2	R3
      0x780A0002,  //  0008  JMPF	R2	#000C
      0x8C080102,  //  0009  GETMET	R2	R0	K2
      0x7C080200,  //  000A  CALL	R2	1
      0x70020002,  //  000B  JMP		#000F
      0x8C080103,  //  000C  GETMET	R2	R0	K3
      0x58100004,  //  000D  LDCONST	R4	K4
      0x7C080400,  //  000E  CALL	R2	2
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: transpile_template_body
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_transpile_template_body,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(at_end),
    /* K2   */  be_nested_str_weak(current),
    /* K3   */  be_nested_str_weak(type),
    /* K4   */  be_const_int(1),
    /* K5   */  be_nested_str_weak(process_statement),
    /* K6   */  be_nested_str_weak(run_statements),
    /* K7   */  be_nested_str_weak(name),
    /* K8   */  be_nested_str_weak(comment),
    /* K9   */  be_nested_str_weak(add),
    /* K10  */  be_nested_str_weak(engine_X2Eadd_X28_X25s__X29_X25s),
    /* K11  */  be_nested_str_weak(stop_iteration),
    /* K12  */  be_nested_str_weak(join_output),
    /* K13  */  be_nested_str_weak(error),
    /* K14  */  be_nested_str_weak(Template_X20body_X20transpilation_X20failed_X3A_X20_X25s),
    }),
    be_str_weak(transpile_template_body),
    &be_const_str_solidified,
    ( &(const binstruction[78]) {  /* code */
      0xA8020041,  //  0000  EXBLK	0	#0043
      0x58040000,  //  0001  LDCONST	R1	K0
      0x8C080101,  //  0002  GETMET	R2	R0	K1
      0x7C080200,  //  0003  CALL	R2	1
      0x740A001F,  //  0004  JMPT	R2	#0025
      0x8C080102,  //  0005  GETMET	R2	R0	K2
      0x7C080200,  //  0006  CALL	R2	1
      0x4C0C0000,  //  0007  LDNIL	R3
      0x200C0403,  //  0008  NE	R3	R2	R3
      0x780E0006,  //  0009  JMPF	R3	#0011
      0x880C0503,  //  000A  GETMBR	R3	R2	K3
      0x5412001A,  //  000B  LDINT	R4	27
      0x1C0C0604,  //  000C  EQ	R3	R3	R4
      0x780E0002,  //  000D  JMPF	R3	#0011
      0x1C0C0300,  //  000E  EQ	R3	R1	K0
      0x780E0000,  //  000F  JMPF	R3	#0011
      0x70020013,  //  0010  JMP		#0025
      0x4C0C0000,  //  0011  LDNIL	R3
      0x200C0403,  //  0012  NE	R3	R2	R3
      0x780E0005,  //  0013  JMPF	R3	#001A
      0x880C0503,  //  0014  GETMBR	R3	R2	K3
      0x54120019,  //  0015  LDINT	R4	26
      0x1C0C0604,  //  0016  EQ	R3	R3	R4
      0x780E0001,  //  0017  JMPF	R3	#001A
      0x00040304,  //  0018  ADD	R1	R1	K4
      0x70020007,  //  0019  JMP		#0022
      0x4C0C0000,  //  001A  LDNIL	R3
      0x200C0403,  //  001B  NE	R3	R2	R3
      0x780E0004,  //  001C  JMPF	R3	#0022
      0x880C0503,  //  001D  GETMBR	R3	R2	K3
      0x5412001A,  //  001E  LDINT	R4	27
      0x1C0C0604,  //  001F  EQ	R3	R3	R4
      0x780E0000,  //  0020  JMPF	R3	#0022
      0x04040304,  //  0021  SUB	R1	R1	K4
      0x8C0C0105,  //  0022  GETMET	R3	R0	K5
      0x7C0C0200,  //  0023  CALL	R3	1
      0x7001FFDC,  //  0024  JMP		#0002
      0x6008000C,  //  0025  GETGBL	R2	G12
      0x880C0106,  //  0026  GETMBR	R3	R0	K6
      0x7C080200,  //  0027  CALL	R2	1
      0x24080500,  //  0028  GT	R2	R2	K0
      0x780A0012,  //  0029  JMPF	R2	#003D
      0x60080010,  //  002A  GETGBL	R2	G16
      0x880C0106,  //  002B  GETMBR	R3	R0	K6
      0x7C080200,  //  002C  CALL	R2	1
      0xA802000B,  //  002D  EXBLK	0	#003A
      0x5C0C0400,  //  002E  MOVE	R3	R2
      0x7C0C0000,  //  002F  CALL	R3	0
      0x94100707,  //  0030  GETIDX	R4	R3	K7
      0x94140708,  //  0031  GETIDX	R5	R3	K8
      0x8C180109,  //  0032  GETMET	R6	R0	K9
      0x60200018,  //  0033  GETGBL	R8	G24
      0x5824000A,  //  0034  LDCONST	R9	K10
      0x5C280800,  //  0035  MOVE	R10	R4
      0x5C2C0A00,  //  0036  MOVE	R11	R5
      0x7C200600,  //  0037  CALL	R8	3
      0x7C180400,  //  0038  CALL	R6	2
      0x7001FFF3,  //  0039  JMP		#002E
      0x5808000B,  //  003A  LDCONST	R2	K11
      0xAC080200,  //  003B  CATCH	R2	1	0
      0xB0080000,  //  003C  RAISE	2	R0	R0
      0x8C08010C,  //  003D  GETMET	R2	R0	K12
      0x7C080200,  //  003E  CALL	R2	1
      0xA8040001,  //  003F  EXBLK	1	1
      0x80040400,  //  0040  RET	1	R2
      0xA8040001,  //  0041  EXBLK	1	1
      0x70020009,  //  0042  JMP		#004D
      0xAC040002,  //  0043  CATCH	R1	0	2
      0x70020006,  //  0044  JMP		#004C
      0x8C0C010D,  //  0045  GETMET	R3	R0	K13
      0x60140018,  //  0046  GETGBL	R5	G24
      0x5818000E,  //  0047  LDCONST	R6	K14
      0x5C1C0400,  //  0048  MOVE	R7	R2
      0x7C140400,  //  0049  CALL	R5	2
      0x7C0C0400,  //  004A  CALL	R3	2
      0x70020000,  //  004B  JMP		#004D
      0xB0080000,  //  004C  RAISE	2	R0	R0
      0x80000000,  //  004D  RET	0
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
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(next),
    /* K3   */  be_nested_str_weak(error),
    /* K4   */  be_nested_str_weak(Expected_X20_X27_X29_X27),
    }),
    be_str_weak(expect_right_paren),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0006,  //  0004  JMPF	R2	#000C
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0x540E0018,  //  0006  LDINT	R3	25
      0x1C080403,  //  0007  EQ	R2	R2	R3
      0x780A0002,  //  0008  JMPF	R2	#000C
      0x8C080102,  //  0009  GETMET	R2	R0	K2
      0x7C080200,  //  000A  CALL	R2	1
      0x70020002,  //  000B  JMP		#000F
      0x8C080103,  //  000C  GETMET	R2	R0	K3
      0x58100004,  //  000D  LDCONST	R4	K4
      0x7C080400,  //  000E  CALL	R2	2
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _validate_template_parameter_type
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__validate_template_parameter_type,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(color),
    /* K1   */  be_nested_str_weak(palette),
    /* K2   */  be_nested_str_weak(animation),
    /* K3   */  be_nested_str_weak(number),
    /* K4   */  be_nested_str_weak(string),
    /* K5   */  be_nested_str_weak(boolean),
    /* K6   */  be_nested_str_weak(time),
    /* K7   */  be_nested_str_weak(percentage),
    /* K8   */  be_nested_str_weak(variable),
    /* K9   */  be_nested_str_weak(value_provider),
    /* K10  */  be_nested_str_weak(stop_iteration),
    /* K11  */  be_nested_str_weak(error),
    /* K12  */  be_nested_str_weak(Invalid_X20parameter_X20type_X20_X27_X25s_X27_X2E_X20Valid_X20types_X20are_X3A_X20_X25s),
    }),
    be_str_weak(_validate_template_parameter_type),
    &be_const_str_solidified,
    ( &(const binstruction[36]) {  /* code */
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
      0x600C0010,  //  000C  GETGBL	R3	G16
      0x5C100400,  //  000D  MOVE	R4	R2
      0x7C0C0200,  //  000E  CALL	R3	1
      0xA8020007,  //  000F  EXBLK	0	#0018
      0x5C100600,  //  0010  MOVE	R4	R3
      0x7C100000,  //  0011  CALL	R4	0
      0x1C140204,  //  0012  EQ	R5	R1	R4
      0x78160002,  //  0013  JMPF	R5	#0017
      0x50140200,  //  0014  LDBOOL	R5	1	0
      0xA8040001,  //  0015  EXBLK	1	1
      0x80040A00,  //  0016  RET	1	R5
      0x7001FFF7,  //  0017  JMP		#0010
      0x580C000A,  //  0018  LDCONST	R3	K10
      0xAC0C0200,  //  0019  CATCH	R3	1	0
      0xB0080000,  //  001A  RAISE	2	R0	R0
      0x8C0C010B,  //  001B  GETMET	R3	R0	K11
      0x60140018,  //  001C  GETGBL	R5	G24
      0x5818000C,  //  001D  LDCONST	R6	K12
      0x5C1C0200,  //  001E  MOVE	R7	R1
      0x5C200400,  //  001F  MOVE	R8	R2
      0x7C140600,  //  0020  CALL	R5	3
      0x7C0C0400,  //  0021  CALL	R3	2
      0x500C0000,  //  0022  LDBOOL	R3	0	0
      0x80040600,  //  0023  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _split_function_arguments
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__split_function_arguments,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(),
    /* K2   */  be_nested_str_weak(split),
    /* K3   */  be_nested_str_weak(_X2C),
    /* K4   */  be_nested_str_weak(strip),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(push),
    /* K7   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(_split_function_arguments),
    &be_const_str_solidified,
    ( &(const binstruction[37]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x1C0C0301,  //  0001  EQ	R3	R1	K1
      0x740E0002,  //  0002  JMPT	R3	#0006
      0x4C0C0000,  //  0003  LDNIL	R3
      0x1C0C0203,  //  0004  EQ	R3	R1	R3
      0x780E0002,  //  0005  JMPF	R3	#0009
      0x600C0012,  //  0006  GETGBL	R3	G18
      0x7C0C0000,  //  0007  CALL	R3	0
      0x80040600,  //  0008  RET	1	R3
      0x8C0C0502,  //  0009  GETMET	R3	R2	K2
      0x5C140200,  //  000A  MOVE	R5	R1
      0x58180003,  //  000B  LDCONST	R6	K3
      0x7C0C0600,  //  000C  CALL	R3	3
      0x60100012,  //  000D  GETGBL	R4	G18
      0x7C100000,  //  000E  CALL	R4	0
      0x60140010,  //  000F  GETGBL	R5	G16
      0x5C180600,  //  0010  MOVE	R6	R3
      0x7C140200,  //  0011  CALL	R5	1
      0xA802000D,  //  0012  EXBLK	0	#0021
      0x5C180A00,  //  0013  MOVE	R6	R5
      0x7C180000,  //  0014  CALL	R6	0
      0x8C1C0504,  //  0015  GETMET	R7	R2	K4
      0x5C240C00,  //  0016  MOVE	R9	R6
      0x7C1C0400,  //  0017  CALL	R7	2
      0x6020000C,  //  0018  GETGBL	R8	G12
      0x5C240E00,  //  0019  MOVE	R9	R7
      0x7C200200,  //  001A  CALL	R8	1
      0x24201105,  //  001B  GT	R8	R8	K5
      0x78220002,  //  001C  JMPF	R8	#0020
      0x8C200906,  //  001D  GETMET	R8	R4	K6
      0x5C280E00,  //  001E  MOVE	R10	R7
      0x7C200400,  //  001F  CALL	R8	2
      0x7001FFF1,  //  0020  JMP		#0013
      0x58140007,  //  0021  LDCONST	R5	K7
      0xAC140200,  //  0022  CATCH	R5	1	0
      0xB0080000,  //  0023  RAISE	2	R0	R0
      0x80040800,  //  0024  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_warnings
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_get_warnings,   /* name */
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
    /* K0   */  be_nested_str_weak(warnings),
    }),
    be_str_weak(get_warnings),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
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
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_const_int(1),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(can_use_as_identifier),
    /* K5   */  be_nested_str_weak(value),
    /* K6   */  be_nested_str_weak(next),
    /* K7   */  be_nested_str_weak(error),
    /* K8   */  be_nested_str_weak(Expected_X20identifier),
    /* K9   */  be_nested_str_weak(unknown),
    }),
    be_str_weak(expect_identifier),
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0012,  //  0004  JMPF	R2	#0018
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0x1C080502,  //  0006  EQ	R2	R2	K2
      0x740A000A,  //  0007  JMPT	R2	#0013
      0x88080301,  //  0008  GETMBR	R2	R1	K1
      0x540E0003,  //  0009  LDINT	R3	4
      0x1C080403,  //  000A  EQ	R2	R2	R3
      0x740A0006,  //  000B  JMPT	R2	#0013
      0x88080301,  //  000C  GETMBR	R2	R1	K1
      0x1C080503,  //  000D  EQ	R2	R2	K3
      0x780A0008,  //  000E  JMPF	R2	#0018
      0x8C080104,  //  000F  GETMET	R2	R0	K4
      0x88100305,  //  0010  GETMBR	R4	R1	K5
      0x7C080400,  //  0011  CALL	R2	2
      0x780A0004,  //  0012  JMPF	R2	#0018
      0x88080305,  //  0013  GETMBR	R2	R1	K5
      0x8C0C0106,  //  0014  GETMET	R3	R0	K6
      0x7C0C0200,  //  0015  CALL	R3	1
      0x80040400,  //  0016  RET	1	R2
      0x70020003,  //  0017  JMP		#001C
      0x8C080107,  //  0018  GETMET	R2	R0	K7
      0x58100008,  //  0019  LDCONST	R4	K8
      0x7C080400,  //  001A  CALL	R2	2
      0x80061200,  //  001B  RET	1	K9
      0x80000000,  //  001C  RET	0
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
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(expect_left_paren),
    /* K1   */  be_nested_str_weak(_X7B),
    /* K2   */  be_nested_str_weak(at_end),
    /* K3   */  be_nested_str_weak(check_right_paren),
    /* K4   */  be_nested_str_weak(current),
    /* K5   */  be_nested_str_weak(type),
    /* K6   */  be_nested_str_weak(process_time_value),
    /* K7   */  be_nested_str_weak(_X22interval_X22_X3A_X20_X25s),
    /* K8   */  be_nested_str_weak(process_value),
    /* K9   */  be_nested_str_weak(event_param),
    /* K10  */  be_nested_str_weak(_X22value_X22_X3A_X20_X25s),
    /* K11  */  be_nested_str_weak(expr),
    /* K12  */  be_nested_str_weak(expect_right_paren),
    /* K13  */  be_nested_str_weak(_X7D),
    }),
    be_str_weak(process_event_parameters),
    &be_const_str_solidified,
    ( &(const binstruction[38]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x58040001,  //  0002  LDCONST	R1	K1
      0x8C080102,  //  0003  GETMET	R2	R0	K2
      0x7C080200,  //  0004  CALL	R2	1
      0x740A001B,  //  0005  JMPT	R2	#0022
      0x8C080103,  //  0006  GETMET	R2	R0	K3
      0x7C080200,  //  0007  CALL	R2	1
      0x740A0018,  //  0008  JMPT	R2	#0022
      0x8C080104,  //  0009  GETMET	R2	R0	K4
      0x7C080200,  //  000A  CALL	R2	1
      0x4C0C0000,  //  000B  LDNIL	R3
      0x200C0403,  //  000C  NE	R3	R2	R3
      0x780E000B,  //  000D  JMPF	R3	#001A
      0x880C0505,  //  000E  GETMBR	R3	R2	K5
      0x54120004,  //  000F  LDINT	R4	5
      0x1C0C0604,  //  0010  EQ	R3	R3	R4
      0x780E0007,  //  0011  JMPF	R3	#001A
      0x8C0C0106,  //  0012  GETMET	R3	R0	K6
      0x7C0C0200,  //  0013  CALL	R3	1
      0x60100018,  //  0014  GETGBL	R4	G24
      0x58140007,  //  0015  LDCONST	R5	K7
      0x5C180600,  //  0016  MOVE	R6	R3
      0x7C100400,  //  0017  CALL	R4	2
      0x00040204,  //  0018  ADD	R1	R1	R4
      0x70020007,  //  0019  JMP		#0022
      0x8C0C0108,  //  001A  GETMET	R3	R0	K8
      0x58140009,  //  001B  LDCONST	R5	K9
      0x7C0C0400,  //  001C  CALL	R3	2
      0x60100018,  //  001D  GETGBL	R4	G24
      0x5814000A,  //  001E  LDCONST	R5	K10
      0x8818070B,  //  001F  GETMBR	R6	R3	K11
      0x7C100400,  //  0020  CALL	R4	2
      0x00040204,  //  0021  ADD	R1	R1	R4
      0x8C08010C,  //  0022  GETMET	R2	R0	K12
      0x7C080200,  //  0023  CALL	R2	1
      0x0004030D,  //  0024  ADD	R1	R1	K13
      0x80040200,  //  0025  RET	1	R1
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
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(introspect),
    /* K1   */  be_nested_str_weak(contains),
    /* K2   */  be_nested_str_weak(_has_param),
    /* K3   */  be_nested_str_weak(current),
    /* K4   */  be_nested_str_weak(line),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(error),
    /* K7   */  be_nested_str_weak(Animation_X20_X27_X25s_X27_X20does_X20not_X20have_X20parameter_X20_X27_X25s_X27_X2E_X20Check_X20the_X20animation_X20documentation_X20for_X20valid_X20parameters_X2E),
    /* K8   */  be_nested_str_weak(dsl_compilation_error),
    }),
    be_str_weak(_validate_single_parameter),
    &be_const_str_solidified,
    ( &(const binstruction[43]) {  /* code */
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
      0x70020008,  //  0020  JMP		#002A
      0x58100008,  //  0021  LDCONST	R4	K8
      0xAC100202,  //  0022  CATCH	R4	1	2
      0x70020001,  //  0023  JMP		#0026
      0xB0040805,  //  0024  RAISE	1	R4	R5
      0x70020003,  //  0025  JMP		#002A
      0xAC100002,  //  0026  CATCH	R4	0	2
      0x70020000,  //  0027  JMP		#0029
      0x70020000,  //  0028  JMP		#002A
      0xB0080000,  //  0029  RAISE	2	R0	R0
      0x80000000,  //  002A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: validate_user_name
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_validate_user_name,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation_dsl),
    /* K1   */  be_nested_str_weak(symbol_table),
    /* K2   */  be_nested_str_weak(get),
    /* K3   */  be_nested_str_weak(is_builtin),
    /* K4   */  be_nested_str_weak(type),
    /* K5   */  be_nested_str_weak(error),
    /* K6   */  be_nested_str_weak(Cannot_X20redefine_X20predefined_X20color_X20_X27_X25s_X27_X2E_X20Use_X20a_X20different_X20name_X20like_X20_X27_X25s_custom_X27_X20or_X20_X27my__X25s_X27),
    /* K7   */  be_nested_str_weak(Cannot_X20redefine_X20built_X2Din_X20symbol_X20_X27_X25s_X27_X20_X28type_X3A_X20_X25s_X29_X2E_X20Use_X20a_X20different_X20name_X20like_X20_X27_X25s_custom_X27_X20or_X20_X27my__X25s_X27),
    /* K8   */  be_nested_str_weak(Symbol_X20_X27_X25s_X27_X20is_X20already_X20defined_X20as_X20_X25s_X2E_X20Cannot_X20redefine_X20as_X20_X25s_X2E),
    /* K9   */  be_nested_str_weak(Token),
    /* K10  */  be_nested_str_weak(statement_keywords),
    /* K11  */  be_nested_str_weak(Cannot_X20use_X20DSL_X20keyword_X20_X27_X25s_X27_X20as_X20_X25s_X20name_X2E_X20Use_X20a_X20different_X20name_X20like_X20_X27_X25s_custom_X27_X20or_X20_X27my__X25s_X27),
    /* K12  */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(validate_user_name),
    &be_const_str_solidified,
    ( &(const binstruction[77]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0x88100101,  //  0001  GETMBR	R4	R0	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x5C180200,  //  0003  MOVE	R6	R1
      0x7C100400,  //  0004  CALL	R4	2
      0x4C140000,  //  0005  LDNIL	R5
      0x1C140805,  //  0006  EQ	R5	R4	R5
      0x78160000,  //  0007  JMPF	R5	#0009
      0x70020028,  //  0008  JMP		#0032
      0x88140903,  //  0009  GETMBR	R5	R4	K3
      0x7816000E,  //  000A  JMPF	R5	#001A
      0x88140904,  //  000B  GETMBR	R5	R4	K4
      0x541A000A,  //  000C  LDINT	R6	11
      0x1C140A06,  //  000D  EQ	R5	R5	R6
      0x7816000A,  //  000E  JMPF	R5	#001A
      0x8C140105,  //  000F  GETMET	R5	R0	K5
      0x601C0018,  //  0010  GETGBL	R7	G24
      0x58200006,  //  0011  LDCONST	R8	K6
      0x5C240200,  //  0012  MOVE	R9	R1
      0x5C280200,  //  0013  MOVE	R10	R1
      0x5C2C0200,  //  0014  MOVE	R11	R1
      0x7C1C0800,  //  0015  CALL	R7	4
      0x7C140400,  //  0016  CALL	R5	2
      0x50140000,  //  0017  LDBOOL	R5	0	0
      0x80040A00,  //  0018  RET	1	R5
      0x70020017,  //  0019  JMP		#0032
      0x88140903,  //  001A  GETMBR	R5	R4	K3
      0x7816000B,  //  001B  JMPF	R5	#0028
      0x8C140105,  //  001C  GETMET	R5	R0	K5
      0x601C0018,  //  001D  GETGBL	R7	G24
      0x58200007,  //  001E  LDCONST	R8	K7
      0x5C240200,  //  001F  MOVE	R9	R1
      0x88280904,  //  0020  GETMBR	R10	R4	K4
      0x5C2C0200,  //  0021  MOVE	R11	R1
      0x5C300200,  //  0022  MOVE	R12	R1
      0x7C1C0A00,  //  0023  CALL	R7	5
      0x7C140400,  //  0024  CALL	R5	2
      0x50140000,  //  0025  LDBOOL	R5	0	0
      0x80040A00,  //  0026  RET	1	R5
      0x70020009,  //  0027  JMP		#0032
      0x8C140105,  //  0028  GETMET	R5	R0	K5
      0x601C0018,  //  0029  GETGBL	R7	G24
      0x58200008,  //  002A  LDCONST	R8	K8
      0x5C240200,  //  002B  MOVE	R9	R1
      0x88280904,  //  002C  GETMBR	R10	R4	K4
      0x5C2C0400,  //  002D  MOVE	R11	R2
      0x7C1C0800,  //  002E  CALL	R7	4
      0x7C140400,  //  002F  CALL	R5	2
      0x50140000,  //  0030  LDBOOL	R5	0	0
      0x80040A00,  //  0031  RET	1	R5
      0x60140010,  //  0032  GETGBL	R5	G16
      0x88180709,  //  0033  GETMBR	R6	R3	K9
      0x88180D0A,  //  0034  GETMBR	R6	R6	K10
      0x7C140200,  //  0035  CALL	R5	1
      0xA8020010,  //  0036  EXBLK	0	#0048
      0x5C180A00,  //  0037  MOVE	R6	R5
      0x7C180000,  //  0038  CALL	R6	0
      0x1C1C0206,  //  0039  EQ	R7	R1	R6
      0x781E000B,  //  003A  JMPF	R7	#0047
      0x8C1C0105,  //  003B  GETMET	R7	R0	K5
      0x60240018,  //  003C  GETGBL	R9	G24
      0x5828000B,  //  003D  LDCONST	R10	K11
      0x5C2C0200,  //  003E  MOVE	R11	R1
      0x5C300400,  //  003F  MOVE	R12	R2
      0x5C340200,  //  0040  MOVE	R13	R1
      0x5C380200,  //  0041  MOVE	R14	R1
      0x7C240A00,  //  0042  CALL	R9	5
      0x7C1C0400,  //  0043  CALL	R7	2
      0x501C0000,  //  0044  LDBOOL	R7	0	0
      0xA8040001,  //  0045  EXBLK	1	1
      0x80040E00,  //  0046  RET	1	R7
      0x7001FFEE,  //  0047  JMP		#0037
      0x5814000C,  //  0048  LDCONST	R5	K12
      0xAC140200,  //  0049  CATCH	R5	1	0
      0xB0080000,  //  004A  RAISE	2	R0	R0
      0x50140200,  //  004B  LDBOOL	R5	1	0
      0x80040A00,  //  004C  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _determine_symbol_return_type
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__determine_symbol_return_type,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(type),
    /* K1   */  be_const_int(2),
    /* K2   */  be_const_int(1),
    /* K3   */  be_const_int(3),
    }),
    be_str_weak(_determine_symbol_return_type),
    &be_const_str_solidified,
    ( &(const binstruction[71]) {  /* code */
      0x88080300,  //  0000  GETMBR	R2	R1	K0
      0x540E0008,  //  0001  LDINT	R3	9
      0x1C080403,  //  0002  EQ	R2	R2	R3
      0x740A0003,  //  0003  JMPT	R2	#0008
      0x88080300,  //  0004  GETMBR	R2	R1	K0
      0x540E0007,  //  0005  LDINT	R3	8
      0x1C080403,  //  0006  EQ	R2	R2	R3
      0x780A0002,  //  0007  JMPF	R2	#000B
      0x540A0008,  //  0008  LDINT	R2	9
      0x80040400,  //  0009  RET	1	R2
      0x7002003A,  //  000A  JMP		#0046
      0x88080300,  //  000B  GETMBR	R2	R1	K0
      0x540E000A,  //  000C  LDINT	R3	11
      0x1C080403,  //  000D  EQ	R2	R2	R3
      0x740A0003,  //  000E  JMPT	R2	#0013
      0x88080300,  //  000F  GETMBR	R2	R1	K0
      0x540E0009,  //  0010  LDINT	R3	10
      0x1C080403,  //  0011  EQ	R2	R2	R3
      0x780A0002,  //  0012  JMPF	R2	#0016
      0x540A000A,  //  0013  LDINT	R2	11
      0x80040400,  //  0014  RET	1	R2
      0x7002002F,  //  0015  JMP		#0046
      0x88080300,  //  0016  GETMBR	R2	R1	K0
      0x540E0006,  //  0017  LDINT	R3	7
      0x1C080403,  //  0018  EQ	R2	R2	R3
      0x740A0003,  //  0019  JMPT	R2	#001E
      0x88080300,  //  001A  GETMBR	R2	R1	K0
      0x540E0005,  //  001B  LDINT	R3	6
      0x1C080403,  //  001C  EQ	R2	R2	R3
      0x780A0002,  //  001D  JMPF	R2	#0021
      0x540A0006,  //  001E  LDINT	R2	7
      0x80040400,  //  001F  RET	1	R2
      0x70020024,  //  0020  JMP		#0046
      0x88080300,  //  0021  GETMBR	R2	R1	K0
      0x1C080501,  //  0022  EQ	R2	R2	K1
      0x740A0002,  //  0023  JMPT	R2	#0027
      0x88080300,  //  0024  GETMBR	R2	R1	K0
      0x1C080502,  //  0025  EQ	R2	R2	K2
      0x780A0001,  //  0026  JMPF	R2	#0029
      0x80060200,  //  0027  RET	1	K1
      0x7002001C,  //  0028  JMP		#0046
      0x88080300,  //  0029  GETMBR	R2	R1	K0
      0x1C080503,  //  002A  EQ	R2	R2	K3
      0x780A0002,  //  002B  JMPF	R2	#002F
      0x540A000B,  //  002C  LDINT	R2	12
      0x80040400,  //  002D  RET	1	R2
      0x70020016,  //  002E  JMP		#0046
      0x88080300,  //  002F  GETMBR	R2	R1	K0
      0x540E000B,  //  0030  LDINT	R3	12
      0x1C080403,  //  0031  EQ	R2	R2	R3
      0x780A0002,  //  0032  JMPF	R2	#0036
      0x540A000B,  //  0033  LDINT	R2	12
      0x80040400,  //  0034  RET	1	R2
      0x7002000F,  //  0035  JMP		#0046
      0x88080300,  //  0036  GETMBR	R2	R1	K0
      0x540E000C,  //  0037  LDINT	R3	13
      0x1C080403,  //  0038  EQ	R2	R2	R3
      0x780A0002,  //  0039  JMPF	R2	#003D
      0x540A000C,  //  003A  LDINT	R2	13
      0x80040400,  //  003B  RET	1	R2
      0x70020008,  //  003C  JMP		#0046
      0x88080300,  //  003D  GETMBR	R2	R1	K0
      0x540E000D,  //  003E  LDINT	R3	14
      0x1C080403,  //  003F  EQ	R2	R2	R3
      0x780A0002,  //  0040  JMPF	R2	#0044
      0x540A000D,  //  0041  LDINT	R2	14
      0x80040400,  //  0042  RET	1	R2
      0x70020001,  //  0043  JMP		#0046
      0x540A000B,  //  0044  LDINT	R2	12
      0x80040400,  //  0045  RET	1	R2
      0x80000000,  //  0046  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _validate_template_parameter_usage
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__validate_template_parameter_usage,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(_X25s_),
    /* K2   */  be_nested_str_weak(find),
    /* K3   */  be_nested_str_weak(warning),
    /* K4   */  be_nested_str_weak(Template_X20_X27_X25s_X27_X20parameter_X20_X27_X25s_X27_X20is_X20declared_X20but_X20never_X20used_X20in_X20the_X20template_X20body_X2E),
    /* K5   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(_validate_template_parameter_usage),
    &be_const_str_solidified,
    ( &(const binstruction[30]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0x60140010,  //  0001  GETGBL	R5	G16
      0x5C180400,  //  0002  MOVE	R6	R2
      0x7C140200,  //  0003  CALL	R5	1
      0xA8020014,  //  0004  EXBLK	0	#001A
      0x5C180A00,  //  0005  MOVE	R6	R5
      0x7C180000,  //  0006  CALL	R6	0
      0x601C0018,  //  0007  GETGBL	R7	G24
      0x58200001,  //  0008  LDCONST	R8	K1
      0x5C240C00,  //  0009  MOVE	R9	R6
      0x7C1C0400,  //  000A  CALL	R7	2
      0x8C200902,  //  000B  GETMET	R8	R4	K2
      0x5C280600,  //  000C  MOVE	R10	R3
      0x5C2C0E00,  //  000D  MOVE	R11	R7
      0x7C200600,  //  000E  CALL	R8	3
      0x5425FFFE,  //  000F  LDINT	R9	-1
      0x1C201009,  //  0010  EQ	R8	R8	R9
      0x78220006,  //  0011  JMPF	R8	#0019
      0x8C200103,  //  0012  GETMET	R8	R0	K3
      0x60280018,  //  0013  GETGBL	R10	G24
      0x582C0004,  //  0014  LDCONST	R11	K4
      0x5C300200,  //  0015  MOVE	R12	R1
      0x5C340C00,  //  0016  MOVE	R13	R6
      0x7C280600,  //  0017  CALL	R10	3
      0x7C200400,  //  0018  CALL	R8	2
      0x7001FFEA,  //  0019  JMP		#0005
      0x58140005,  //  001A  LDCONST	R5	K5
      0xAC140200,  //  001B  CATCH	R5	1	0
      0xB0080000,  //  001C  RAISE	2	R0	R0
      0x80000000,  //  001D  RET	0
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
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(next),
    /* K1   */  be_nested_str_weak(current),
    /* K2   */  be_nested_str_weak(type),
    /* K3   */  be_const_int(3),
    /* K4   */  be_nested_str_weak(error),
    /* K5   */  be_nested_str_weak(Expected_X20string_X20literal_X20after_X20_X27berry_X27_X20keyword_X2E_X20Use_X20berry_X20_X22_X22_X22_X3Ccode_X3E_X22_X22_X22_X20or_X20berry_X20_X27_X27_X27_X3Ccode_X3E_X27_X27_X27),
    /* K6   */  be_nested_str_weak(skip_statement),
    /* K7   */  be_nested_str_weak(value),
    /* K8   */  be_nested_str_weak(collect_inline_comment),
    /* K9   */  be_nested_str_weak(add),
    /* K10  */  be_nested_str_weak(_X23_X20Berry_X20code_X20block_X25s),
    /* K11  */  be_nested_str_weak(string),
    /* K12  */  be_nested_str_weak(split),
    /* K13  */  be_nested_str_weak(_X0A),
    /* K14  */  be_nested_str_weak(stop_iteration),
    /* K15  */  be_nested_str_weak(_X23_X20End_X20berry_X20code_X20block),
    }),
    be_str_weak(process_berry_code_block),
    &be_const_str_solidified,
    ( &(const binstruction[49]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x7C040200,  //  0003  CALL	R1	1
      0x4C080000,  //  0004  LDNIL	R2
      0x1C080202,  //  0005  EQ	R2	R1	R2
      0x740A0002,  //  0006  JMPT	R2	#000A
      0x88080302,  //  0007  GETMBR	R2	R1	K2
      0x20080503,  //  0008  NE	R2	R2	K3
      0x780A0005,  //  0009  JMPF	R2	#0010
      0x8C080104,  //  000A  GETMET	R2	R0	K4
      0x58100005,  //  000B  LDCONST	R4	K5
      0x7C080400,  //  000C  CALL	R2	2
      0x8C080106,  //  000D  GETMET	R2	R0	K6
      0x7C080200,  //  000E  CALL	R2	1
      0x80000400,  //  000F  RET	0
      0x88080307,  //  0010  GETMBR	R2	R1	K7
      0x8C0C0100,  //  0011  GETMET	R3	R0	K0
      0x7C0C0200,  //  0012  CALL	R3	1
      0x8C0C0108,  //  0013  GETMET	R3	R0	K8
      0x7C0C0200,  //  0014  CALL	R3	1
      0x8C100109,  //  0015  GETMET	R4	R0	K9
      0x60180018,  //  0016  GETGBL	R6	G24
      0x581C000A,  //  0017  LDCONST	R7	K10
      0x5C200600,  //  0018  MOVE	R8	R3
      0x7C180400,  //  0019  CALL	R6	2
      0x7C100400,  //  001A  CALL	R4	2
      0xA4121600,  //  001B  IMPORT	R4	K11
      0x8C14090C,  //  001C  GETMET	R5	R4	K12
      0x5C1C0400,  //  001D  MOVE	R7	R2
      0x5820000D,  //  001E  LDCONST	R8	K13
      0x7C140600,  //  001F  CALL	R5	3
      0x60180010,  //  0020  GETGBL	R6	G16
      0x5C1C0A00,  //  0021  MOVE	R7	R5
      0x7C180200,  //  0022  CALL	R6	1
      0xA8020005,  //  0023  EXBLK	0	#002A
      0x5C1C0C00,  //  0024  MOVE	R7	R6
      0x7C1C0000,  //  0025  CALL	R7	0
      0x8C200109,  //  0026  GETMET	R8	R0	K9
      0x5C280E00,  //  0027  MOVE	R10	R7
      0x7C200400,  //  0028  CALL	R8	2
      0x7001FFF9,  //  0029  JMP		#0024
      0x5818000E,  //  002A  LDCONST	R6	K14
      0xAC180200,  //  002B  CATCH	R6	1	0
      0xB0080000,  //  002C  RAISE	2	R0	R0
      0x8C180109,  //  002D  GETMET	R6	R0	K9
      0x5820000F,  //  002E  LDCONST	R8	K15
      0x7C180400,  //  002F  CALL	R6	2
      0x80000000,  //  0030  RET	0
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
** Solidified function: convert_color
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_convert_color,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation_dsl),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(startswith),
    /* K3   */  be_nested_str_weak(0x),
    /* K4   */  be_nested_str_weak(0xFF_X25s),
    /* K5   */  be_const_int(2),
    /* K6   */  be_const_int(2147483647),
    /* K7   */  be_nested_str_weak(is_color_name),
    /* K8   */  be_nested_str_weak(get_named_color_value),
    /* K9   */  be_nested_str_weak(0xFFFFFFFF),
    }),
    be_str_weak(convert_color),
    &be_const_str_solidified,
    ( &(const binstruction[36]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0x8C100702,  //  0002  GETMET	R4	R3	K2
      0x5C180200,  //  0003  MOVE	R6	R1
      0x581C0003,  //  0004  LDCONST	R7	K3
      0x7C100600,  //  0005  CALL	R4	3
      0x78120013,  //  0006  JMPF	R4	#001B
      0x6010000C,  //  0007  GETGBL	R4	G12
      0x5C140200,  //  0008  MOVE	R5	R1
      0x7C100200,  //  0009  CALL	R4	1
      0x54160009,  //  000A  LDINT	R5	10
      0x1C100805,  //  000B  EQ	R4	R4	R5
      0x78120001,  //  000C  JMPF	R4	#000F
      0x80040200,  //  000D  RET	1	R1
      0x7002000B,  //  000E  JMP		#001B
      0x6010000C,  //  000F  GETGBL	R4	G12
      0x5C140200,  //  0010  MOVE	R5	R1
      0x7C100200,  //  0011  CALL	R4	1
      0x54160007,  //  0012  LDINT	R5	8
      0x1C100805,  //  0013  EQ	R4	R4	R5
      0x78120005,  //  0014  JMPF	R4	#001B
      0x60100018,  //  0015  GETGBL	R4	G24
      0x58140004,  //  0016  LDCONST	R5	K4
      0x401A0B06,  //  0017  CONNECT	R6	K5	K6
      0x94180206,  //  0018  GETIDX	R6	R1	R6
      0x7C100400,  //  0019  CALL	R4	2
      0x80040800,  //  001A  RET	1	R4
      0x8C100507,  //  001B  GETMET	R4	R2	K7
      0x5C180200,  //  001C  MOVE	R6	R1
      0x7C100400,  //  001D  CALL	R4	2
      0x78120003,  //  001E  JMPF	R4	#0023
      0x8C100108,  //  001F  GETMET	R4	R0	K8
      0x5C180200,  //  0020  MOVE	R6	R1
      0x7C100400,  //  0021  CALL	R4	2
      0x80040800,  //  0022  RET	1	R4
      0x80061200,  //  0023  RET	1	K9
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
    ( &(const bvalue[31]) {     /* constants */
    /* K0   */  be_nested_str_weak(next),
    /* K1   */  be_nested_str_weak(expect_identifier),
    /* K2   */  be_nested_str_weak(validate_user_name),
    /* K3   */  be_nested_str_weak(animation),
    /* K4   */  be_nested_str_weak(skip_statement),
    /* K5   */  be_nested_str_weak(expect_assign),
    /* K6   */  be_nested_str_weak(current),
    /* K7   */  be_nested_str_weak(type),
    /* K8   */  be_const_int(0),
    /* K9   */  be_const_int(1),
    /* K10  */  be_nested_str_weak(peek),
    /* K11  */  be_nested_str_weak(value),
    /* K12  */  be_nested_str_weak(),
    /* K13  */  be_nested_str_weak(_X20_X20),
    /* K14  */  be_nested_str_weak(symbol_table),
    /* K15  */  be_nested_str_weak(get),
    /* K16  */  be_nested_str_weak(process_function_arguments),
    /* K17  */  be_nested_str_weak(engine_X2C_X20_X25s),
    /* K18  */  be_nested_str_weak(engine),
    /* K19  */  be_nested_str_weak(add),
    /* K20  */  be_nested_str_weak(var_X20_X25s__X20_X3D_X20_X25s_template_X28_X25s_X29_X25s),
    /* K21  */  be_nested_str_weak(create_animation),
    /* K22  */  be_nested_str_weak(var_X20_X25s__X20_X3D_X20animation_X2Eget_user_function_X28_X27_X25s_X27_X29_X28_X25s_X29_X25s),
    /* K23  */  be_nested_str_weak(error),
    /* K24  */  be_nested_str_weak(Animation_X20factory_X20function_X20_X27_X25s_X27_X20does_X20not_X20exist_X20or_X20does_X20not_X20create_X20an_X20instance_X20of_X20animation_X2Eanimation_X20class_X2E_X20Check_X20the_X20function_X20name_X20and_X20ensure_X20it_X20returns_X20an_X20animation_X20object_X2E),
    /* K25  */  be_nested_str_weak(var_X20_X25s__X20_X3D_X20animation_X2E_X25s_X28engine_X29_X25s),
    /* K26  */  be_nested_str_weak(_create_instance_for_validation),
    /* K27  */  be_nested_str_weak(_process_named_arguments_for_animation),
    /* K28  */  be_nested_str_weak(_X25s_),
    /* K29  */  be_nested_str_weak(_process_simple_value_assignment),
    /* K30  */  be_nested_str_weak(CONTEXT_ANIMATION),
    }),
    be_str_weak(process_animation),
    &be_const_str_solidified,
    ( &(const binstruction[175]) {  /* code */
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
      0x1C0C0708,  //  0011  EQ	R3	R3	K8
      0x740E0002,  //  0012  JMPT	R3	#0016
      0x880C0507,  //  0013  GETMBR	R3	R2	K7
      0x1C0C0709,  //  0014  EQ	R3	R3	K9
      0x780E0091,  //  0015  JMPF	R3	#00A8
      0x8C0C010A,  //  0016  GETMET	R3	R0	K10
      0x7C0C0200,  //  0017  CALL	R3	1
      0x4C100000,  //  0018  LDNIL	R4
      0x200C0604,  //  0019  NE	R3	R3	R4
      0x780E008C,  //  001A  JMPF	R3	#00A8
      0x8C0C010A,  //  001B  GETMET	R3	R0	K10
      0x7C0C0200,  //  001C  CALL	R3	1
      0x880C0707,  //  001D  GETMBR	R3	R3	K7
      0x54120017,  //  001E  LDINT	R4	24
      0x1C0C0604,  //  001F  EQ	R3	R3	R4
      0x780E0086,  //  0020  JMPF	R3	#00A8
      0x880C050B,  //  0021  GETMBR	R3	R2	K11
      0x8C100100,  //  0022  GETMET	R4	R0	K0
      0x7C100200,  //  0023  CALL	R4	1
      0x5810000C,  //  0024  LDCONST	R4	K12
      0x8C140106,  //  0025  GETMET	R5	R0	K6
      0x7C140200,  //  0026  CALL	R5	1
      0x4C180000,  //  0027  LDNIL	R6
      0x20140A06,  //  0028  NE	R5	R5	R6
      0x7816000C,  //  0029  JMPF	R5	#0037
      0x8C140106,  //  002A  GETMET	R5	R0	K6
      0x7C140200,  //  002B  CALL	R5	1
      0x88140B07,  //  002C  GETMBR	R5	R5	K7
      0x541A0024,  //  002D  LDINT	R6	37
      0x1C140A06,  //  002E  EQ	R5	R5	R6
      0x78160006,  //  002F  JMPF	R5	#0037
      0x8C140106,  //  0030  GETMET	R5	R0	K6
      0x7C140200,  //  0031  CALL	R5	1
      0x88140B0B,  //  0032  GETMBR	R5	R5	K11
      0x00161A05,  //  0033  ADD	R5	K13	R5
      0x5C100A00,  //  0034  MOVE	R4	R5
      0x8C140100,  //  0035  GETMET	R5	R0	K0
      0x7C140200,  //  0036  CALL	R5	1
      0x8814010E,  //  0037  GETMBR	R5	R0	K14
      0x8C140B0F,  //  0038  GETMET	R5	R5	K15
      0x5C1C0600,  //  0039  MOVE	R7	R3
      0x7C140400,  //  003A  CALL	R5	2
      0x4C180000,  //  003B  LDNIL	R6
      0x20180A06,  //  003C  NE	R6	R5	R6
      0x781A001D,  //  003D  JMPF	R6	#005C
      0x88180B07,  //  003E  GETMBR	R6	R5	K7
      0x541E000D,  //  003F  LDINT	R7	14
      0x1C180C07,  //  0040  EQ	R6	R6	R7
      0x781A0019,  //  0041  JMPF	R6	#005C
      0x8C180110,  //  0042  GETMET	R6	R0	K16
      0x50200000,  //  0043  LDBOOL	R8	0	0
      0x7C180400,  //  0044  CALL	R6	2
      0x201C0D0C,  //  0045  NE	R7	R6	K12
      0x781E0004,  //  0046  JMPF	R7	#004C
      0x601C0018,  //  0047  GETGBL	R7	G24
      0x58200011,  //  0048  LDCONST	R8	K17
      0x5C240C00,  //  0049  MOVE	R9	R6
      0x7C1C0400,  //  004A  CALL	R7	2
      0x70020000,  //  004B  JMP		#004D
      0x581C0012,  //  004C  LDCONST	R7	K18
      0x8C200113,  //  004D  GETMET	R8	R0	K19
      0x60280018,  //  004E  GETGBL	R10	G24
      0x582C0014,  //  004F  LDCONST	R11	K20
      0x5C300200,  //  0050  MOVE	R12	R1
      0x5C340600,  //  0051  MOVE	R13	R3
      0x5C380E00,  //  0052  MOVE	R14	R7
      0x5C3C0800,  //  0053  MOVE	R15	R4
      0x7C280A00,  //  0054  CALL	R10	5
      0x7C200400,  //  0055  CALL	R8	2
      0x8820010E,  //  0056  GETMBR	R8	R0	K14
      0x8C201115,  //  0057  GETMET	R8	R8	K21
      0x5C280200,  //  0058  MOVE	R10	R1
      0x4C2C0000,  //  0059  LDNIL	R11
      0x7C200600,  //  005A  CALL	R8	3
      0x7002004A,  //  005B  JMP		#00A7
      0x4C180000,  //  005C  LDNIL	R6
      0x20180A06,  //  005D  NE	R6	R5	R6
      0x781A001D,  //  005E  JMPF	R6	#007D
      0x88180B07,  //  005F  GETMBR	R6	R5	K7
      0x541E0004,  //  0060  LDINT	R7	5
      0x1C180C07,  //  0061  EQ	R6	R6	R7
      0x781A0019,  //  0062  JMPF	R6	#007D
      0x8C180110,  //  0063  GETMET	R6	R0	K16
      0x50200000,  //  0064  LDBOOL	R8	0	0
      0x7C180400,  //  0065  CALL	R6	2
      0x201C0D0C,  //  0066  NE	R7	R6	K12
      0x781E0004,  //  0067  JMPF	R7	#006D
      0x601C0018,  //  0068  GETGBL	R7	G24
      0x58200011,  //  0069  LDCONST	R8	K17
      0x5C240C00,  //  006A  MOVE	R9	R6
      0x7C1C0400,  //  006B  CALL	R7	2
      0x70020000,  //  006C  JMP		#006E
      0x581C0012,  //  006D  LDCONST	R7	K18
      0x8C200113,  //  006E  GETMET	R8	R0	K19
      0x60280018,  //  006F  GETGBL	R10	G24
      0x582C0016,  //  0070  LDCONST	R11	K22
      0x5C300200,  //  0071  MOVE	R12	R1
      0x5C340600,  //  0072  MOVE	R13	R3
      0x5C380E00,  //  0073  MOVE	R14	R7
      0x5C3C0800,  //  0074  MOVE	R15	R4
      0x7C280A00,  //  0075  CALL	R10	5
      0x7C200400,  //  0076  CALL	R8	2
      0x8820010E,  //  0077  GETMBR	R8	R0	K14
      0x8C201115,  //  0078  GETMET	R8	R8	K21
      0x5C280200,  //  0079  MOVE	R10	R1
      0x4C2C0000,  //  007A  LDNIL	R11
      0x7C200600,  //  007B  CALL	R8	3
      0x70020029,  //  007C  JMP		#00A7
      0x4C180000,  //  007D  LDNIL	R6
      0x1C180A06,  //  007E  EQ	R6	R5	R6
      0x741A0003,  //  007F  JMPT	R6	#0084
      0x88180B07,  //  0080  GETMBR	R6	R5	K7
      0x541E0007,  //  0081  LDINT	R7	8
      0x20180C07,  //  0082  NE	R6	R6	R7
      0x781A0008,  //  0083  JMPF	R6	#008D
      0x8C180117,  //  0084  GETMET	R6	R0	K23
      0x60200018,  //  0085  GETGBL	R8	G24
      0x58240018,  //  0086  LDCONST	R9	K24
      0x5C280600,  //  0087  MOVE	R10	R3
      0x7C200400,  //  0088  CALL	R8	2
      0x7C180400,  //  0089  CALL	R6	2
      0x8C180104,  //  008A  GETMET	R6	R0	K4
      0x7C180200,  //  008B  CALL	R6	1
      0x80000C00,  //  008C  RET	0
      0x8C180113,  //  008D  GETMET	R6	R0	K19
      0x60200018,  //  008E  GETGBL	R8	G24
      0x58240019,  //  008F  LDCONST	R9	K25
      0x5C280200,  //  0090  MOVE	R10	R1
      0x5C2C0600,  //  0091  MOVE	R11	R3
      0x5C300800,  //  0092  MOVE	R12	R4
      0x7C200800,  //  0093  CALL	R8	4
      0x7C180400,  //  0094  CALL	R6	2
      0x8C18011A,  //  0095  GETMET	R6	R0	K26
      0x5C200600,  //  0096  MOVE	R8	R3
      0x7C180400,  //  0097  CALL	R6	2
      0x4C1C0000,  //  0098  LDNIL	R7
      0x201C0C07,  //  0099  NE	R7	R6	R7
      0x781E0004,  //  009A  JMPF	R7	#00A0
      0x881C010E,  //  009B  GETMBR	R7	R0	K14
      0x8C1C0F15,  //  009C  GETMET	R7	R7	K21
      0x5C240200,  //  009D  MOVE	R9	R1
      0x5C280C00,  //  009E  MOVE	R10	R6
      0x7C1C0600,  //  009F  CALL	R7	3
      0x8C1C011B,  //  00A0  GETMET	R7	R0	K27
      0x60240018,  //  00A1  GETGBL	R9	G24
      0x5828001C,  //  00A2  LDCONST	R10	K28
      0x5C2C0200,  //  00A3  MOVE	R11	R1
      0x7C240400,  //  00A4  CALL	R9	2
      0x5C280600,  //  00A5  MOVE	R10	R3
      0x7C1C0600,  //  00A6  CALL	R7	3
      0x70020004,  //  00A7  JMP		#00AD
      0x8C0C011D,  //  00A8  GETMET	R3	R0	K29
      0x5C140200,  //  00A9  MOVE	R5	R1
      0x8818011E,  //  00AA  GETMBR	R6	R0	K30
      0x841C0000,  //  00AB  CLOSURE	R7	P0
      0x7C0C0800,  //  00AC  CALL	R3	4
      0xA0000000,  //  00AD  CLOSE	R0
      0x80000000,  //  00AE  RET	0
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
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(at_end),
    /* K1   */  be_nested_str_weak(current),
    /* K2   */  be_nested_str_weak(type),
    /* K3   */  be_nested_str_weak(next),
    }),
    be_str_weak(skip_whitespace),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x74060011,  //  0002  JMPT	R1	#0015
      0x8C040101,  //  0003  GETMET	R1	R0	K1
      0x7C040200,  //  0004  CALL	R1	1
      0x4C080000,  //  0005  LDNIL	R2
      0x20080202,  //  0006  NE	R2	R1	R2
      0x780A000A,  //  0007  JMPF	R2	#0013
      0x88080302,  //  0008  GETMBR	R2	R1	K2
      0x540E0022,  //  0009  LDINT	R3	35
      0x1C080403,  //  000A  EQ	R2	R2	R3
      0x740A0003,  //  000B  JMPT	R2	#0010
      0x88080302,  //  000C  GETMBR	R2	R1	K2
      0x540E0024,  //  000D  LDINT	R3	37
      0x1C080403,  //  000E  EQ	R2	R2	R3
      0x780A0002,  //  000F  JMPF	R2	#0013
      0x8C080103,  //  0010  GETMET	R2	R0	K3
      0x7C080200,  //  0011  CALL	R2	1
      0x70020000,  //  0012  JMP		#0014
      0x70020000,  //  0013  JMP		#0015
      0x7001FFEA,  //  0014  JMP		#0000
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_unary_expression
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_unary_expression,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(error),
    /* K2   */  be_nested_str_weak(Expected_X20value),
    /* K3   */  be_nested_str_weak(ExpressionResult),
    /* K4   */  be_nested_str_weak(literal),
    /* K5   */  be_nested_str_weak(nil),
    /* K6   */  be_nested_str_weak(type),
    /* K7   */  be_nested_str_weak(next),
    /* K8   */  be_nested_str_weak(process_unary_expression),
    /* K9   */  be_nested_str_weak(_X28_X2D_X25s_X29),
    /* K10  */  be_nested_str_weak(expr),
    /* K11  */  be_nested_str_weak(has_dynamic),
    /* K12  */  be_nested_str_weak(has_dangerous),
    /* K13  */  be_nested_str_weak(return_type),
    /* K14  */  be_nested_str_weak(instance_for_validation),
    /* K15  */  be_nested_str_weak(process_primary_expression),
    }),
    be_str_weak(process_unary_expression),
    &be_const_str_solidified,
    ( &(const binstruction[54]) {  /* code */
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
      0x541A0009,  //  000E  LDINT	R6	10
      0x1C140A06,  //  000F  EQ	R5	R5	R6
      0x78160012,  //  0010  JMPF	R5	#0024
      0x8C140107,  //  0011  GETMET	R5	R0	K7
      0x7C140200,  //  0012  CALL	R5	1
      0x8C140108,  //  0013  GETMET	R5	R0	K8
      0x5C1C0200,  //  0014  MOVE	R7	R1
      0x50200000,  //  0015  LDBOOL	R8	0	0
      0x5C240600,  //  0016  MOVE	R9	R3
      0x7C140800,  //  0017  CALL	R5	4
      0x8C180103,  //  0018  GETMET	R6	R0	K3
      0x60200018,  //  0019  GETGBL	R8	G24
      0x58240009,  //  001A  LDCONST	R9	K9
      0x88280B0A,  //  001B  GETMBR	R10	R5	K10
      0x7C200400,  //  001C  CALL	R8	2
      0x88240B0B,  //  001D  GETMBR	R9	R5	K11
      0x88280B0C,  //  001E  GETMBR	R10	R5	K12
      0x502C0200,  //  001F  LDBOOL	R11	1	0
      0x88300B0D,  //  0020  GETMBR	R12	R5	K13
      0x88340B0E,  //  0021  GETMBR	R13	R5	K14
      0x7C180E00,  //  0022  CALL	R6	7
      0x80040C00,  //  0023  RET	1	R6
      0x88140906,  //  0024  GETMBR	R5	R4	K6
      0x541A0008,  //  0025  LDINT	R6	9
      0x1C140A06,  //  0026  EQ	R5	R5	R6
      0x78160007,  //  0027  JMPF	R5	#0030
      0x8C140107,  //  0028  GETMET	R5	R0	K7
      0x7C140200,  //  0029  CALL	R5	1
      0x8C140108,  //  002A  GETMET	R5	R0	K8
      0x5C1C0200,  //  002B  MOVE	R7	R1
      0x50200000,  //  002C  LDBOOL	R8	0	0
      0x5C240600,  //  002D  MOVE	R9	R3
      0x7C140800,  //  002E  CALL	R5	4
      0x80040A00,  //  002F  RET	1	R5
      0x8C14010F,  //  0030  GETMET	R5	R0	K15
      0x5C1C0200,  //  0031  MOVE	R7	R1
      0x5C200400,  //  0032  MOVE	R8	R2
      0x5C240600,  //  0033  MOVE	R9	R3
      0x7C140800,  //  0034  CALL	R5	4
      0x80040A00,  //  0035  RET	1	R5
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
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_const_int(2),
    /* K3   */  be_nested_str_weak(value),
    /* K4   */  be_nested_str_weak(next),
    /* K5   */  be_nested_str_weak(error),
    /* K6   */  be_nested_str_weak(Expected_X20number),
    /* K7   */  be_nested_str_weak(0),
    }),
    be_str_weak(expect_number),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0007,  //  0004  JMPF	R2	#000D
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0x1C080502,  //  0006  EQ	R2	R2	K2
      0x780A0004,  //  0007  JMPF	R2	#000D
      0x88080303,  //  0008  GETMBR	R2	R1	K3
      0x8C0C0104,  //  0009  GETMET	R3	R0	K4
      0x7C0C0200,  //  000A  CALL	R3	1
      0x80040400,  //  000B  RET	1	R2
      0x70020003,  //  000C  JMP		#0011
      0x8C080105,  //  000D  GETMET	R2	R0	K5
      0x58100006,  //  000E  LDCONST	R4	K6
      0x7C080400,  //  000F  CALL	R2	2
      0x80060E00,  //  0010  RET	1	K7
      0x80000000,  //  0011  RET	0
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
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(add),
    /* K1   */  be_nested_str_weak(import_X20animation),
    /* K2   */  be_nested_str_weak(),
    /* K3   */  be_nested_str_weak(at_end),
    /* K4   */  be_nested_str_weak(process_statement),
    /* K5   */  be_nested_str_weak(generate_engine_run),
    /* K6   */  be_nested_str_weak(has_warnings),
    /* K7   */  be_nested_str_weak(_X23_X20Compilation_X20warnings_X3A),
    /* K8   */  be_nested_str_weak(warnings),
    /* K9   */  be_nested_str_weak(_X23_X20_X25s),
    /* K10  */  be_nested_str_weak(stop_iteration),
    /* K11  */  be_nested_str_weak(join_output),
    /* K12  */  be_nested_str_weak(error),
    /* K13  */  be_nested_str_weak(Transpilation_X20failed_X3A_X20_X25s),
    }),
    be_str_weak(transpile),
    &be_const_str_solidified,
    ( &(const binstruction[57]) {  /* code */
      0xA802002C,  //  0000  EXBLK	0	#002E
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
      0x8C040106,  //  000F  GETMET	R1	R0	K6
      0x7C040200,  //  0010  CALL	R1	1
      0x78060015,  //  0011  JMPF	R1	#0028
      0x8C040100,  //  0012  GETMET	R1	R0	K0
      0x580C0002,  //  0013  LDCONST	R3	K2
      0x7C040400,  //  0014  CALL	R1	2
      0x8C040100,  //  0015  GETMET	R1	R0	K0
      0x580C0007,  //  0016  LDCONST	R3	K7
      0x7C040400,  //  0017  CALL	R1	2
      0x60040010,  //  0018  GETGBL	R1	G16
      0x88080108,  //  0019  GETMBR	R2	R0	K8
      0x7C040200,  //  001A  CALL	R1	1
      0xA8020008,  //  001B  EXBLK	0	#0025
      0x5C080200,  //  001C  MOVE	R2	R1
      0x7C080000,  //  001D  CALL	R2	0
      0x8C0C0100,  //  001E  GETMET	R3	R0	K0
      0x60140018,  //  001F  GETGBL	R5	G24
      0x58180009,  //  0020  LDCONST	R6	K9
      0x5C1C0400,  //  0021  MOVE	R7	R2
      0x7C140400,  //  0022  CALL	R5	2
      0x7C0C0400,  //  0023  CALL	R3	2
      0x7001FFF6,  //  0024  JMP		#001C
      0x5804000A,  //  0025  LDCONST	R1	K10
      0xAC040200,  //  0026  CATCH	R1	1	0
      0xB0080000,  //  0027  RAISE	2	R0	R0
      0x8C04010B,  //  0028  GETMET	R1	R0	K11
      0x7C040200,  //  0029  CALL	R1	1
      0xA8040001,  //  002A  EXBLK	1	1
      0x80040200,  //  002B  RET	1	R1
      0xA8040001,  //  002C  EXBLK	1	1
      0x70020009,  //  002D  JMP		#0038
      0xAC040002,  //  002E  CATCH	R1	0	2
      0x70020006,  //  002F  JMP		#0037
      0x8C0C010C,  //  0030  GETMET	R3	R0	K12
      0x60140018,  //  0031  GETGBL	R5	G24
      0x5818000D,  //  0032  LDCONST	R6	K13
      0x5C1C0400,  //  0033  MOVE	R7	R2
      0x7C140400,  //  0034  CALL	R5	2
      0x7C0C0400,  //  0035  CALL	R3	2
      0x70020000,  //  0036  JMP		#0038
      0xB0080000,  //  0037  RAISE	2	R0	R0
      0x80000000,  //  0038  RET	0
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
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(next),
    /* K3   */  be_nested_str_weak(error),
    /* K4   */  be_nested_str_weak(Expected_X20_X27_X7D_X27),
    }),
    be_str_weak(expect_right_brace),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0006,  //  0004  JMPF	R2	#000C
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0x540E001A,  //  0006  LDINT	R3	27
      0x1C080403,  //  0007  EQ	R2	R2	R3
      0x780A0002,  //  0008  JMPF	R2	#000C
      0x8C080102,  //  0009  GETMET	R2	R0	K2
      0x7C080200,  //  000A  CALL	R2	1
      0x70020002,  //  000B  JMP		#000F
      0x8C080103,  //  000C  GETMET	R2	R0	K3
      0x58100004,  //  000D  LDCONST	R4	K4
      0x7C080400,  //  000E  CALL	R2	2
      0x80000000,  //  000F  RET	0
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
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(symbol_table),
    /* K1   */  be_nested_str_weak(symbol_exists),
    /* K2   */  be_nested_str_weak(error),
    /* K3   */  be_nested_str_weak(Undefined_X20reference_X20_X27_X25s_X27_X20in_X20_X25s_X20statement_X2E_X20Make_X20sure_X20the_X20value_X20provider_X20or_X20animation_X20is_X20defined_X20before_X20use_X2E),
    /* K4   */  be_nested_str_weak(get),
    /* K5   */  be_nested_str_weak(type),
    /* K6   */  be_nested_str_weak(_X27_X25s_X27_X20in_X20_X25s_X20statement_X20is_X20not_X20a_X20value_X20provider_X20or_X20animation_X20instance_X2E_X20Only_X20value_X20provider_X20instances_X20_X28like_X20oscillators_X29_X20and_X20animation_X20instances_X20can_X20be_X20restarted_X2E),
    /* K7   */  be_nested_str_weak(Could_X20not_X20validate_X20_X27_X25s_X27_X20in_X20_X25s_X20statement_X3A_X20_X25s),
    }),
    be_str_weak(_validate_value_provider_reference),
    &be_const_str_solidified,
    ( &(const binstruction[65]) {  /* code */
      0xA8020030,  //  0000  EXBLK	0	#0032
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
      0x78120015,  //  0016  JMPF	R4	#002D
      0x88100705,  //  0017  GETMBR	R4	R3	K5
      0x54160006,  //  0018  LDINT	R5	7
      0x1C100805,  //  0019  EQ	R4	R4	R5
      0x74120003,  //  001A  JMPT	R4	#001F
      0x88100705,  //  001B  GETMBR	R4	R3	K5
      0x54160008,  //  001C  LDINT	R5	9
      0x1C100805,  //  001D  EQ	R4	R4	R5
      0x78120003,  //  001E  JMPF	R4	#0023
      0x50100200,  //  001F  LDBOOL	R4	1	0
      0xA8040001,  //  0020  EXBLK	1	1
      0x80040800,  //  0021  RET	1	R4
      0x70020009,  //  0022  JMP		#002D
      0x8C100102,  //  0023  GETMET	R4	R0	K2
      0x60180018,  //  0024  GETGBL	R6	G24
      0x581C0006,  //  0025  LDCONST	R7	K6
      0x5C200200,  //  0026  MOVE	R8	R1
      0x5C240400,  //  0027  MOVE	R9	R2
      0x7C180600,  //  0028  CALL	R6	3
      0x7C100400,  //  0029  CALL	R4	2
      0x50100000,  //  002A  LDBOOL	R4	0	0
      0xA8040001,  //  002B  EXBLK	1	1
      0x80040800,  //  002C  RET	1	R4
      0x50100200,  //  002D  LDBOOL	R4	1	0
      0xA8040001,  //  002E  EXBLK	1	1
      0x80040800,  //  002F  RET	1	R4
      0xA8040001,  //  0030  EXBLK	1	1
      0x7002000D,  //  0031  JMP		#0040
      0xAC0C0002,  //  0032  CATCH	R3	0	2
      0x7002000A,  //  0033  JMP		#003F
      0x8C140102,  //  0034  GETMET	R5	R0	K2
      0x601C0018,  //  0035  GETGBL	R7	G24
      0x58200007,  //  0036  LDCONST	R8	K7
      0x5C240200,  //  0037  MOVE	R9	R1
      0x5C280400,  //  0038  MOVE	R10	R2
      0x5C2C0800,  //  0039  MOVE	R11	R4
      0x7C1C0800,  //  003A  CALL	R7	4
      0x7C140400,  //  003B  CALL	R5	2
      0x50140000,  //  003C  LDBOOL	R5	0	0
      0x80040A00,  //  003D  RET	1	R5
      0x70020000,  //  003E  JMP		#0040
      0xB0080000,  //  003F  RAISE	2	R0	R0
      0x80000000,  //  0040  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_palette_color
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_palette_color,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation_dsl),
    /* K1   */  be_nested_str_weak(current),
    /* K2   */  be_nested_str_weak(error),
    /* K3   */  be_nested_str_weak(Expected_X20color_X20value_X20in_X20palette),
    /* K4   */  be_nested_str_weak(0xFFFFFFFF),
    /* K5   */  be_nested_str_weak(type),
    /* K6   */  be_nested_str_weak(next),
    /* K7   */  be_nested_str_weak(convert_color),
    /* K8   */  be_nested_str_weak(value),
    /* K9   */  be_const_int(1),
    /* K10  */  be_nested_str_weak(is_color_name),
    /* K11  */  be_nested_str_weak(get_named_color_value),
    /* K12  */  be_nested_str_weak(Unknown_X20color_X20_X27_X25s_X27_X2E_X20Palettes_X20only_X20accept_X20hex_X20colors_X20_X280xRRGGBB_X29_X20or_X20predefined_X20color_X20names_X20_X28like_X20_X27red_X27_X2C_X20_X27blue_X27_X2C_X20_X27green_X27_X29_X2C_X20but_X20not_X20custom_X20colors_X20defined_X20previously_X2E_X20For_X20dynamic_X20palettes_X20with_X20custom_X20colors_X2C_X20use_X20user_X20functions_X20instead_X2E),
    /* K13  */  be_nested_str_weak(Expected_X20color_X20value_X20in_X20palette_X2E_X20Use_X20hex_X20colors_X20_X280xRRGGBB_X29_X20or_X20predefined_X20color_X20names_X20_X28like_X20_X27red_X27_X2C_X20_X27blue_X27_X2C_X20_X27green_X27_X29_X2E),
    }),
    be_str_weak(process_palette_color),
    &be_const_str_solidified,
    ( &(const binstruction[45]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080101,  //  0001  GETMET	R2	R0	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x4C0C0000,  //  0003  LDNIL	R3
      0x1C0C0403,  //  0004  EQ	R3	R2	R3
      0x780E0003,  //  0005  JMPF	R3	#000A
      0x8C0C0102,  //  0006  GETMET	R3	R0	K2
      0x58140003,  //  0007  LDCONST	R5	K3
      0x7C0C0400,  //  0008  CALL	R3	2
      0x80060800,  //  0009  RET	1	K4
      0x880C0505,  //  000A  GETMBR	R3	R2	K5
      0x54120003,  //  000B  LDINT	R4	4
      0x1C0C0604,  //  000C  EQ	R3	R3	R4
      0x780E0005,  //  000D  JMPF	R3	#0014
      0x8C0C0106,  //  000E  GETMET	R3	R0	K6
      0x7C0C0200,  //  000F  CALL	R3	1
      0x8C0C0107,  //  0010  GETMET	R3	R0	K7
      0x88140508,  //  0011  GETMBR	R5	R2	K8
      0x7C0C0400,  //  0012  CALL	R3	2
      0x80040600,  //  0013  RET	1	R3
      0x880C0505,  //  0014  GETMBR	R3	R2	K5
      0x1C0C0709,  //  0015  EQ	R3	R3	K9
      0x780E0011,  //  0016  JMPF	R3	#0029
      0x880C0508,  //  0017  GETMBR	R3	R2	K8
      0x8C100106,  //  0018  GETMET	R4	R0	K6
      0x7C100200,  //  0019  CALL	R4	1
      0x8C10030A,  //  001A  GETMET	R4	R1	K10
      0x5C180600,  //  001B  MOVE	R6	R3
      0x7C100400,  //  001C  CALL	R4	2
      0x78120003,  //  001D  JMPF	R4	#0022
      0x8C10010B,  //  001E  GETMET	R4	R0	K11
      0x5C180600,  //  001F  MOVE	R6	R3
      0x7C100400,  //  0020  CALL	R4	2
      0x80040800,  //  0021  RET	1	R4
      0x8C100102,  //  0022  GETMET	R4	R0	K2
      0x60180018,  //  0023  GETGBL	R6	G24
      0x581C000C,  //  0024  LDCONST	R7	K12
      0x5C200600,  //  0025  MOVE	R8	R3
      0x7C180400,  //  0026  CALL	R6	2
      0x7C100400,  //  0027  CALL	R4	2
      0x80060800,  //  0028  RET	1	K4
      0x8C0C0102,  //  0029  GETMET	R3	R0	K2
      0x5814000D,  //  002A  LDCONST	R5	K13
      0x7C0C0400,  //  002B  CALL	R3	2
      0x80060800,  //  002C  RET	1	K4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: warning
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_warning,   /* name */
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
    /* K3   */  be_nested_str_weak(warnings),
    /* K4   */  be_nested_str_weak(push),
    /* K5   */  be_nested_str_weak(Line_X20_X25s_X3A_X20_X25s),
    }),
    be_str_weak(warning),
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
** Solidified function: process_value
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_value,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[19]) {     /* constants */
    /* K0   */  be_nested_str_weak(process_additive_expression),
    /* K1   */  be_nested_str_weak(CONTEXT_VARIABLE),
    /* K2   */  be_nested_str_weak(CONTEXT_PROPERTY),
    /* K3   */  be_nested_str_weak(needs_closure),
    /* K4   */  be_nested_str_weak(CONTEXT_REPEAT_COUNT),
    /* K5   */  be_nested_str_weak(needs_function),
    /* K6   */  be_nested_str_weak(def_X20_X28engine_X29_X20return_X20_X25s_X20end),
    /* K7   */  be_nested_str_weak(expr),
    /* K8   */  be_nested_str_weak(ExpressionResult),
    /* K9   */  be_nested_str_weak(function_call),
    /* K10  */  be_nested_str_weak(return_type),
    /* K11  */  be_nested_str_weak(animation_X2Ecreate_closure_value_X28engine_X2C_X20def_X20_X28engine_X29_X20return_X20_X25s_X20end_X29),
    /* K12  */  be_nested_str_weak(has_computation),
    /* K13  */  be_nested_str_weak(_unwrap_resolve),
    /* K14  */  be_nested_str_weak(symbol_table),
    /* K15  */  be_nested_str_weak(get),
    /* K16  */  be_nested_str_weak(closure_value),
    /* K17  */  be_nested_str_weak(type),
    /* K18  */  be_nested_str_weak(instance),
    }),
    be_str_weak(process_value),
    &be_const_str_solidified,
    ( &(const binstruction[66]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0x50140200,  //  0002  LDBOOL	R5	1	0
      0x50180000,  //  0003  LDBOOL	R6	0	0
      0x7C080800,  //  0004  CALL	R2	4
      0x880C0101,  //  0005  GETMBR	R3	R0	K1
      0x1C0C0203,  //  0006  EQ	R3	R1	R3
      0x740E0002,  //  0007  JMPT	R3	#000B
      0x880C0102,  //  0008  GETMBR	R3	R0	K2
      0x1C0C0203,  //  0009  EQ	R3	R1	R3
      0x780E0002,  //  000A  JMPF	R3	#000E
      0x8C0C0503,  //  000B  GETMET	R3	R2	K3
      0x7C0C0200,  //  000C  CALL	R3	1
      0x740E0005,  //  000D  JMPT	R3	#0014
      0x880C0104,  //  000E  GETMBR	R3	R0	K4
      0x1C0C0203,  //  000F  EQ	R3	R1	R3
      0x780E002E,  //  0010  JMPF	R3	#0040
      0x8C0C0505,  //  0011  GETMET	R3	R2	K5
      0x7C0C0200,  //  0012  CALL	R3	1
      0x780E002B,  //  0013  JMPF	R3	#0040
      0x880C0104,  //  0014  GETMBR	R3	R0	K4
      0x1C0C0203,  //  0015  EQ	R3	R1	R3
      0x780E000A,  //  0016  JMPF	R3	#0022
      0x600C0018,  //  0017  GETGBL	R3	G24
      0x58100006,  //  0018  LDCONST	R4	K6
      0x88140507,  //  0019  GETMBR	R5	R2	K7
      0x7C0C0400,  //  001A  CALL	R3	2
      0x88100108,  //  001B  GETMBR	R4	R0	K8
      0x8C100909,  //  001C  GETMET	R4	R4	K9
      0x5C180600,  //  001D  MOVE	R6	R3
      0x881C050A,  //  001E  GETMBR	R7	R2	K10
      0x7C100600,  //  001F  CALL	R4	3
      0x80040800,  //  0020  RET	1	R4
      0x7002001C,  //  0021  JMP		#003F
      0x600C0018,  //  0022  GETGBL	R3	G24
      0x5810000B,  //  0023  LDCONST	R4	K11
      0x88140507,  //  0024  GETMBR	R5	R2	K7
      0x7C0C0400,  //  0025  CALL	R3	2
      0x8810050A,  //  0026  GETMBR	R4	R2	K10
      0x54160008,  //  0027  LDINT	R5	9
      0x1C100805,  //  0028  EQ	R4	R4	R5
      0x78120002,  //  0029  JMPF	R4	#002D
      0x8810050C,  //  002A  GETMBR	R4	R2	K12
      0x74120000,  //  002B  JMPT	R4	#002D
      0x880C0507,  //  002C  GETMBR	R3	R2	K7
      0x8C10010D,  //  002D  GETMET	R4	R0	K13
      0x88180507,  //  002E  GETMBR	R6	R2	K7
      0x7C100400,  //  002F  CALL	R4	2
      0x4C140000,  //  0030  LDNIL	R5
      0x20140805,  //  0031  NE	R5	R4	R5
      0x78160000,  //  0032  JMPF	R5	#0034
      0x5C0C0800,  //  0033  MOVE	R3	R4
      0x8814010E,  //  0034  GETMBR	R5	R0	K14
      0x8C140B0F,  //  0035  GETMET	R5	R5	K15
      0x581C0010,  //  0036  LDCONST	R7	K16
      0x7C140400,  //  0037  CALL	R5	2
      0x88180108,  //  0038  GETMBR	R6	R0	K8
      0x8C180D09,  //  0039  GETMET	R6	R6	K9
      0x5C200600,  //  003A  MOVE	R8	R3
      0x88240B11,  //  003B  GETMBR	R9	R5	K17
      0x88280B12,  //  003C  GETMBR	R10	R5	K18
      0x7C180800,  //  003D  CALL	R6	4
      0x80040C00,  //  003E  RET	1	R6
      0x70020000,  //  003F  JMP		#0041
      0x80040400,  //  0040  RET	1	R2
      0x80000000,  //  0041  RET	0
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
    ( &(const bvalue[32]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(),
    /* K2   */  be_nested_str_weak(type),
    /* K3   */  be_const_int(1),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(value),
    /* K6   */  be_nested_str_weak(next),
    /* K7   */  be_nested_str_weak(error),
    /* K8   */  be_nested_str_weak(Expected_X20function_X20name),
    /* K9   */  be_nested_str_weak(nil),
    /* K10  */  be_nested_str_weak(symbol_table),
    /* K11  */  be_nested_str_weak(get),
    /* K12  */  be_nested_str_weak(process_function_arguments),
    /* K13  */  be_nested_str_weak(_X25s_X28_X25s_X29),
    /* K14  */  be_nested_str_weak(get_reference),
    /* K15  */  be_nested_str_weak(log),
    /* K16  */  be_nested_str_weak(process_log_call),
    /* K17  */  be_nested_str_weak(CONTEXT_EXPRESSION),
    /* K18  */  be_nested_str_weak(engine_X2C_X20_X25s),
    /* K19  */  be_nested_str_weak(engine),
    /* K20  */  be_nested_str_weak(_X25s_template_X28_X25s_X29),
    /* K21  */  be_nested_str_weak(_validate_animation_factory_exists),
    /* K22  */  be_nested_str_weak(Animation_X20factory_X20function_X20_X27_X25s_X27_X20does_X20not_X20exist_X2E_X20Check_X20the_X20function_X20name_X20and_X20ensure_X20it_X27s_X20available_X20in_X20the_X20animation_X20module_X2E),
    /* K23  */  be_nested_str_weak(skip_function_arguments),
    /* K24  */  be_nested_str_weak(expect_left_paren),
    /* K25  */  be_nested_str_weak(_process_parameters_core),
    /* K26  */  be_nested_str_weak(generic),
    /* K27  */  be_nested_str_weak(expect_right_paren),
    /* K28  */  be_nested_str_weak(_X0A),
    /* K29  */  be_nested_str_weak(stop_iteration),
    /* K30  */  be_nested_str_weak(_X28def_X20_X28engine_X29_X0A_X20_X20var_X20provider_X20_X3D_X20animation_X2E_X25s_X28engine_X29_X0A_X25s_X0A_X20_X20return_X20provider_X0Aend_X29_X28engine_X29),
    /* K31  */  be_nested_str_weak(animation_X2E_X25s_X28engine_X29),
    }),
    be_str_weak(process_nested_function_call),
    &be_const_str_solidified,
    ( &(const binstruction[143]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x58080001,  //  0002  LDCONST	R2	K1
      0x4C0C0000,  //  0003  LDNIL	R3
      0x200C0203,  //  0004  NE	R3	R1	R3
      0x780E0009,  //  0005  JMPF	R3	#0010
      0x880C0302,  //  0006  GETMBR	R3	R1	K2
      0x1C0C0703,  //  0007  EQ	R3	R3	K3
      0x740E0002,  //  0008  JMPT	R3	#000C
      0x880C0302,  //  0009  GETMBR	R3	R1	K2
      0x1C0C0704,  //  000A  EQ	R3	R3	K4
      0x780E0003,  //  000B  JMPF	R3	#0010
      0x88080305,  //  000C  GETMBR	R2	R1	K5
      0x8C0C0106,  //  000D  GETMET	R3	R0	K6
      0x7C0C0200,  //  000E  CALL	R3	1
      0x70020003,  //  000F  JMP		#0014
      0x8C0C0107,  //  0010  GETMET	R3	R0	K7
      0x58140008,  //  0011  LDCONST	R5	K8
      0x7C0C0400,  //  0012  CALL	R3	2
      0x80061200,  //  0013  RET	1	K9
      0x880C010A,  //  0014  GETMBR	R3	R0	K10
      0x8C0C070B,  //  0015  GETMET	R3	R3	K11
      0x5C140400,  //  0016  MOVE	R5	R2
      0x7C0C0400,  //  0017  CALL	R3	2
      0x4C100000,  //  0018  LDNIL	R4
      0x20100604,  //  0019  NE	R4	R3	R4
      0x7812000D,  //  001A  JMPF	R4	#0029
      0x88100702,  //  001B  GETMBR	R4	R3	K2
      0x54160003,  //  001C  LDINT	R5	4
      0x1C100805,  //  001D  EQ	R4	R4	R5
      0x78120009,  //  001E  JMPF	R4	#0029
      0x8C10010C,  //  001F  GETMET	R4	R0	K12
      0x50180200,  //  0020  LDBOOL	R6	1	0
      0x7C100400,  //  0021  CALL	R4	2
      0x60140018,  //  0022  GETGBL	R5	G24
      0x5818000D,  //  0023  LDCONST	R6	K13
      0x8C1C070E,  //  0024  GETMET	R7	R3	K14
      0x7C1C0200,  //  0025  CALL	R7	1
      0x5C200800,  //  0026  MOVE	R8	R4
      0x7C140600,  //  0027  CALL	R5	3
      0x80040A00,  //  0028  RET	1	R5
      0x1C10050F,  //  0029  EQ	R4	R2	K15
      0x78120008,  //  002A  JMPF	R4	#0034
      0x8C10010C,  //  002B  GETMET	R4	R0	K12
      0x50180200,  //  002C  LDBOOL	R6	1	0
      0x7C100400,  //  002D  CALL	R4	2
      0x8C140110,  //  002E  GETMET	R5	R0	K16
      0x5C1C0800,  //  002F  MOVE	R7	R4
      0x88200111,  //  0030  GETMBR	R8	R0	K17
      0x58240001,  //  0031  LDCONST	R9	K1
      0x7C140800,  //  0032  CALL	R5	4
      0x80040A00,  //  0033  RET	1	R5
      0x4C100000,  //  0034  LDNIL	R4
      0x20100604,  //  0035  NE	R4	R3	R4
      0x78120015,  //  0036  JMPF	R4	#004D
      0x88100702,  //  0037  GETMBR	R4	R3	K2
      0x5416000D,  //  0038  LDINT	R5	14
      0x1C100805,  //  0039  EQ	R4	R4	R5
      0x78120011,  //  003A  JMPF	R4	#004D
      0x8C10010C,  //  003B  GETMET	R4	R0	K12
      0x50180200,  //  003C  LDBOOL	R6	1	0
      0x7C100400,  //  003D  CALL	R4	2
      0x20140901,  //  003E  NE	R5	R4	K1
      0x78160004,  //  003F  JMPF	R5	#0045
      0x60140018,  //  0040  GETGBL	R5	G24
      0x58180012,  //  0041  LDCONST	R6	K18
      0x5C1C0800,  //  0042  MOVE	R7	R4
      0x7C140400,  //  0043  CALL	R5	2
      0x70020000,  //  0044  JMP		#0046
      0x58140013,  //  0045  LDCONST	R5	K19
      0x60180018,  //  0046  GETGBL	R6	G24
      0x581C0014,  //  0047  LDCONST	R7	K20
      0x5C200400,  //  0048  MOVE	R8	R2
      0x5C240A00,  //  0049  MOVE	R9	R5
      0x7C180600,  //  004A  CALL	R6	3
      0x80040C00,  //  004B  RET	1	R6
      0x70020040,  //  004C  JMP		#008E
      0x8C100115,  //  004D  GETMET	R4	R0	K21
      0x5C180400,  //  004E  MOVE	R6	R2
      0x7C100400,  //  004F  CALL	R4	2
      0x74120008,  //  0050  JMPT	R4	#005A
      0x8C100107,  //  0051  GETMET	R4	R0	K7
      0x60180018,  //  0052  GETGBL	R6	G24
      0x581C0016,  //  0053  LDCONST	R7	K22
      0x5C200400,  //  0054  MOVE	R8	R2
      0x7C180400,  //  0055  CALL	R6	2
      0x7C100400,  //  0056  CALL	R4	2
      0x8C100117,  //  0057  GETMET	R4	R0	K23
      0x7C100200,  //  0058  CALL	R4	1
      0x80061200,  //  0059  RET	1	K9
      0x8C100118,  //  005A  GETMET	R4	R0	K24
      0x7C100200,  //  005B  CALL	R4	1
      0x60100012,  //  005C  GETGBL	R4	G18
      0x7C100000,  //  005D  CALL	R4	0
      0x84140000,  //  005E  CLOSURE	R5	P0
      0x8C180119,  //  005F  GETMET	R6	R0	K25
      0x5C200400,  //  0060  MOVE	R8	R2
      0x5824001A,  //  0061  LDCONST	R9	K26
      0x5C280A00,  //  0062  MOVE	R10	R5
      0x7C180800,  //  0063  CALL	R6	4
      0x8C18011B,  //  0064  GETMET	R6	R0	K27
      0x7C180200,  //  0065  CALL	R6	1
      0x6018000C,  //  0066  GETGBL	R6	G12
      0x5C1C0800,  //  0067  MOVE	R7	R4
      0x7C180200,  //  0068  CALL	R6	1
      0x24180D04,  //  0069  GT	R6	R6	K4
      0x781A001B,  //  006A  JMPF	R6	#0087
      0x58180001,  //  006B  LDCONST	R6	K1
      0x601C0010,  //  006C  GETGBL	R7	G16
      0x6020000C,  //  006D  GETGBL	R8	G12
      0x5C240800,  //  006E  MOVE	R9	R4
      0x7C200200,  //  006F  CALL	R8	1
      0x04201103,  //  0070  SUB	R8	R8	K3
      0x40220808,  //  0071  CONNECT	R8	K4	R8
      0x7C1C0200,  //  0072  CALL	R7	1
      0xA8020007,  //  0073  EXBLK	0	#007C
      0x5C200E00,  //  0074  MOVE	R8	R7
      0x7C200000,  //  0075  CALL	R8	0
      0x24241104,  //  0076  GT	R9	R8	K4
      0x78260000,  //  0077  JMPF	R9	#0079
      0x00180D1C,  //  0078  ADD	R6	R6	K28
      0x94240808,  //  0079  GETIDX	R9	R4	R8
      0x00180C09,  //  007A  ADD	R6	R6	R9
      0x7001FFF7,  //  007B  JMP		#0074
      0x581C001D,  //  007C  LDCONST	R7	K29
      0xAC1C0200,  //  007D  CATCH	R7	1	0
      0xB0080000,  //  007E  RAISE	2	R0	R0
      0x601C0018,  //  007F  GETGBL	R7	G24
      0x5820001E,  //  0080  LDCONST	R8	K30
      0x5C240400,  //  0081  MOVE	R9	R2
      0x5C280C00,  //  0082  MOVE	R10	R6
      0x7C1C0600,  //  0083  CALL	R7	3
      0xA0000000,  //  0084  CLOSE	R0
      0x80040E00,  //  0085  RET	1	R7
      0x70020005,  //  0086  JMP		#008D
      0x60180018,  //  0087  GETGBL	R6	G24
      0x581C001F,  //  0088  LDCONST	R7	K31
      0x5C200400,  //  0089  MOVE	R8	R2
      0x7C180400,  //  008A  CALL	R6	2
      0xA0000000,  //  008B  CLOSE	R0
      0x80040C00,  //  008C  RET	1	R6
      0xA0100000,  //  008D  CLOSE	R4
      0x80000000,  //  008E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: next
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_next,   /* name */
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
    /* K0   */  be_nested_str_weak(pull_lexer),
    /* K1   */  be_nested_str_weak(next_token),
    }),
    be_str_weak(next),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _validate_template_call_arguments
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__validate_template_call_arguments,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(error),
    /* K1   */  be_nested_str_weak(Template_X20_X27_X25s_X27_X20expects_X20_X25s_X20arguments_X20but_X20_X25s_X20were_X20provided_X2E_X20Expected_X20parameters_X3A_X20_X25s),
    }),
    be_str_weak(_validate_template_call_arguments),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x6014000C,  //  0000  GETGBL	R5	G12
      0x5C180400,  //  0001  MOVE	R6	R2
      0x7C140200,  //  0002  CALL	R5	1
      0x6018000C,  //  0003  GETGBL	R6	G12
      0x5C1C0600,  //  0004  MOVE	R7	R3
      0x7C180200,  //  0005  CALL	R6	1
      0x20140A06,  //  0006  NE	R5	R5	R6
      0x7816000E,  //  0007  JMPF	R5	#0017
      0x8C140100,  //  0008  GETMET	R5	R0	K0
      0x601C0018,  //  0009  GETGBL	R7	G24
      0x58200001,  //  000A  LDCONST	R8	K1
      0x5C240200,  //  000B  MOVE	R9	R1
      0x6028000C,  //  000C  GETGBL	R10	G12
      0x5C2C0600,  //  000D  MOVE	R11	R3
      0x7C280200,  //  000E  CALL	R10	1
      0x602C000C,  //  000F  GETGBL	R11	G12
      0x5C300400,  //  0010  MOVE	R12	R2
      0x7C2C0200,  //  0011  CALL	R11	1
      0x5C300600,  //  0012  MOVE	R12	R3
      0x7C1C0A00,  //  0013  CALL	R7	5
      0x7C140400,  //  0014  CALL	R5	2
      0x50140000,  //  0015  LDBOOL	R5	0	0
      0x80040A00,  //  0016  RET	1	R5
      0x50140200,  //  0017  LDBOOL	R5	1	0
      0x80040A00,  //  0018  RET	1	R5
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
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(next),
    /* K1   */  be_nested_str_weak(expect_left_paren),
    /* K2   */  be_nested_str_weak(current),
    /* K3   */  be_nested_str_weak(type),
    /* K4   */  be_const_int(3),
    /* K5   */  be_nested_str_weak(error),
    /* K6   */  be_nested_str_weak(log_X28_X29_X20function_X20requires_X20a_X20string_X20message),
    /* K7   */  be_nested_str_weak(skip_statement),
    /* K8   */  be_nested_str_weak(value),
    /* K9   */  be_nested_str_weak(expect_right_paren),
    /* K10  */  be_nested_str_weak(collect_inline_comment),
    /* K11  */  be_nested_str_weak(process_log_call),
    /* K12  */  be_nested_str_weak(standalone),
    /* K13  */  be_nested_str_weak(add),
    }),
    be_str_weak(process_standalone_log),
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x7C040200,  //  0003  CALL	R1	1
      0x8C040102,  //  0004  GETMET	R1	R0	K2
      0x7C040200,  //  0005  CALL	R1	1
      0x4C080000,  //  0006  LDNIL	R2
      0x1C080202,  //  0007  EQ	R2	R1	R2
      0x740A0002,  //  0008  JMPT	R2	#000C
      0x88080303,  //  0009  GETMBR	R2	R1	K3
      0x20080504,  //  000A  NE	R2	R2	K4
      0x780A0005,  //  000B  JMPF	R2	#0012
      0x8C080105,  //  000C  GETMET	R2	R0	K5
      0x58100006,  //  000D  LDCONST	R4	K6
      0x7C080400,  //  000E  CALL	R2	2
      0x8C080107,  //  000F  GETMET	R2	R0	K7
      0x7C080200,  //  0010  CALL	R2	1
      0x80000400,  //  0011  RET	0
      0x88080308,  //  0012  GETMBR	R2	R1	K8
      0x8C0C0100,  //  0013  GETMET	R3	R0	K0
      0x7C0C0200,  //  0014  CALL	R3	1
      0x8C0C0109,  //  0015  GETMET	R3	R0	K9
      0x7C0C0200,  //  0016  CALL	R3	1
      0x8C0C010A,  //  0017  GETMET	R3	R0	K10
      0x7C0C0200,  //  0018  CALL	R3	1
      0x8C10010B,  //  0019  GETMET	R4	R0	K11
      0x5C180400,  //  001A  MOVE	R6	R2
      0x581C000C,  //  001B  LDCONST	R7	K12
      0x5C200600,  //  001C  MOVE	R8	R3
      0x7C100800,  //  001D  CALL	R4	4
      0x8C14010D,  //  001E  GETMET	R5	R0	K13
      0x5C1C0800,  //  001F  MOVE	R7	R4
      0x7C140400,  //  0020  CALL	R5	2
      0x80000000,  //  0021  RET	0
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
    ( &(const bvalue[39]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(add),
    /* K3   */  be_nested_str_weak(value),
    /* K4   */  be_nested_str_weak(next),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(strip),
    /* K7   */  be_nested_str_weak(error),
    /* K8   */  be_nested_str_weak(_X27strip_X27_X20directive_X20is_X20temporarily_X20disabled_X2E_X20Strip_X20configuration_X20is_X20handled_X20automatically_X2E),
    /* K9   */  be_nested_str_weak(skip_statement),
    /* K10  */  be_nested_str_weak(template),
    /* K11  */  be_nested_str_weak(process_template),
    /* K12  */  be_nested_str_weak(strip_initialized),
    /* K13  */  be_nested_str_weak(generate_default_strip_initialization),
    /* K14  */  be_nested_str_weak(color),
    /* K15  */  be_nested_str_weak(process_color),
    /* K16  */  be_nested_str_weak(palette),
    /* K17  */  be_nested_str_weak(process_palette),
    /* K18  */  be_nested_str_weak(animation),
    /* K19  */  be_nested_str_weak(process_animation),
    /* K20  */  be_nested_str_weak(set),
    /* K21  */  be_nested_str_weak(process_set),
    /* K22  */  be_nested_str_weak(sequence),
    /* K23  */  be_nested_str_weak(process_sequence),
    /* K24  */  be_nested_str_weak(run),
    /* K25  */  be_nested_str_weak(process_run),
    /* K26  */  be_nested_str_weak(import),
    /* K27  */  be_nested_str_weak(process_import),
    /* K28  */  be_nested_str_weak(on),
    /* K29  */  be_nested_str_weak(process_event_handler),
    /* K30  */  be_nested_str_weak(berry),
    /* K31  */  be_nested_str_weak(process_berry_code_block),
    /* K32  */  be_nested_str_weak(Unknown_X20keyword_X20_X27_X25s_X27_X2E),
    /* K33  */  be_const_int(1),
    /* K34  */  be_nested_str_weak(log),
    /* K35  */  be_nested_str_weak(peek),
    /* K36  */  be_nested_str_weak(process_standalone_log),
    /* K37  */  be_nested_str_weak(process_property_assignment),
    /* K38  */  be_nested_str_weak(Unexpected_X20token_X20_X27_X25s_X27_X2E),
    }),
    be_str_weak(process_statement),
    &be_const_str_solidified,
    ( &(const binstruction[145]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x1C080202,  //  0003  EQ	R2	R1	R2
      0x780A0000,  //  0004  JMPF	R2	#0006
      0x80000400,  //  0005  RET	0
      0x88080301,  //  0006  GETMBR	R2	R1	K1
      0x540E0024,  //  0007  LDINT	R3	37
      0x1C080403,  //  0008  EQ	R2	R2	R3
      0x780A0005,  //  0009  JMPF	R2	#0010
      0x8C080102,  //  000A  GETMET	R2	R0	K2
      0x88100303,  //  000B  GETMBR	R4	R1	K3
      0x7C080400,  //  000C  CALL	R2	2
      0x8C080104,  //  000D  GETMET	R2	R0	K4
      0x7C080200,  //  000E  CALL	R2	1
      0x80000400,  //  000F  RET	0
      0x88080301,  //  0010  GETMBR	R2	R1	K1
      0x540E0022,  //  0011  LDINT	R3	35
      0x1C080403,  //  0012  EQ	R2	R2	R3
      0x780A0002,  //  0013  JMPF	R2	#0017
      0x8C080104,  //  0014  GETMET	R2	R0	K4
      0x7C080200,  //  0015  CALL	R2	1
      0x80000400,  //  0016  RET	0
      0x88080301,  //  0017  GETMBR	R2	R1	K1
      0x1C080505,  //  0018  EQ	R2	R2	K5
      0x780A0052,  //  0019  JMPF	R2	#006D
      0x88080303,  //  001A  GETMBR	R2	R1	K3
      0x1C080506,  //  001B  EQ	R2	R2	K6
      0x780A0006,  //  001C  JMPF	R2	#0024
      0x8C080107,  //  001D  GETMET	R2	R0	K7
      0x58100008,  //  001E  LDCONST	R4	K8
      0x7C080400,  //  001F  CALL	R2	2
      0x8C080109,  //  0020  GETMET	R2	R0	K9
      0x7C080200,  //  0021  CALL	R2	1
      0x80000400,  //  0022  RET	0
      0x70020047,  //  0023  JMP		#006C
      0x88080303,  //  0024  GETMBR	R2	R1	K3
      0x1C08050A,  //  0025  EQ	R2	R2	K10
      0x780A0002,  //  0026  JMPF	R2	#002A
      0x8C08010B,  //  0027  GETMET	R2	R0	K11
      0x7C080200,  //  0028  CALL	R2	1
      0x70020041,  //  0029  JMP		#006C
      0x8808010C,  //  002A  GETMBR	R2	R0	K12
      0x740A0001,  //  002B  JMPT	R2	#002E
      0x8C08010D,  //  002C  GETMET	R2	R0	K13
      0x7C080200,  //  002D  CALL	R2	1
      0x88080303,  //  002E  GETMBR	R2	R1	K3
      0x1C08050E,  //  002F  EQ	R2	R2	K14
      0x780A0002,  //  0030  JMPF	R2	#0034
      0x8C08010F,  //  0031  GETMET	R2	R0	K15
      0x7C080200,  //  0032  CALL	R2	1
      0x70020037,  //  0033  JMP		#006C
      0x88080303,  //  0034  GETMBR	R2	R1	K3
      0x1C080510,  //  0035  EQ	R2	R2	K16
      0x780A0002,  //  0036  JMPF	R2	#003A
      0x8C080111,  //  0037  GETMET	R2	R0	K17
      0x7C080200,  //  0038  CALL	R2	1
      0x70020031,  //  0039  JMP		#006C
      0x88080303,  //  003A  GETMBR	R2	R1	K3
      0x1C080512,  //  003B  EQ	R2	R2	K18
      0x780A0002,  //  003C  JMPF	R2	#0040
      0x8C080113,  //  003D  GETMET	R2	R0	K19
      0x7C080200,  //  003E  CALL	R2	1
      0x7002002B,  //  003F  JMP		#006C
      0x88080303,  //  0040  GETMBR	R2	R1	K3
      0x1C080514,  //  0041  EQ	R2	R2	K20
      0x780A0002,  //  0042  JMPF	R2	#0046
      0x8C080115,  //  0043  GETMET	R2	R0	K21
      0x7C080200,  //  0044  CALL	R2	1
      0x70020025,  //  0045  JMP		#006C
      0x88080303,  //  0046  GETMBR	R2	R1	K3
      0x1C080516,  //  0047  EQ	R2	R2	K22
      0x780A0002,  //  0048  JMPF	R2	#004C
      0x8C080117,  //  0049  GETMET	R2	R0	K23
      0x7C080200,  //  004A  CALL	R2	1
      0x7002001F,  //  004B  JMP		#006C
      0x88080303,  //  004C  GETMBR	R2	R1	K3
      0x1C080518,  //  004D  EQ	R2	R2	K24
      0x780A0002,  //  004E  JMPF	R2	#0052
      0x8C080119,  //  004F  GETMET	R2	R0	K25
      0x7C080200,  //  0050  CALL	R2	1
      0x70020019,  //  0051  JMP		#006C
      0x88080303,  //  0052  GETMBR	R2	R1	K3
      0x1C08051A,  //  0053  EQ	R2	R2	K26
      0x780A0002,  //  0054  JMPF	R2	#0058
      0x8C08011B,  //  0055  GETMET	R2	R0	K27
      0x7C080200,  //  0056  CALL	R2	1
      0x70020013,  //  0057  JMP		#006C
      0x88080303,  //  0058  GETMBR	R2	R1	K3
      0x1C08051C,  //  0059  EQ	R2	R2	K28
      0x780A0002,  //  005A  JMPF	R2	#005E
      0x8C08011D,  //  005B  GETMET	R2	R0	K29
      0x7C080200,  //  005C  CALL	R2	1
      0x7002000D,  //  005D  JMP		#006C
      0x88080303,  //  005E  GETMBR	R2	R1	K3
      0x1C08051E,  //  005F  EQ	R2	R2	K30
      0x780A0002,  //  0060  JMPF	R2	#0064
      0x8C08011F,  //  0061  GETMET	R2	R0	K31
      0x7C080200,  //  0062  CALL	R2	1
      0x70020007,  //  0063  JMP		#006C
      0x8C080107,  //  0064  GETMET	R2	R0	K7
      0x60100018,  //  0065  GETGBL	R4	G24
      0x58140020,  //  0066  LDCONST	R5	K32
      0x88180303,  //  0067  GETMBR	R6	R1	K3
      0x7C100400,  //  0068  CALL	R4	2
      0x7C080400,  //  0069  CALL	R2	2
      0x8C080109,  //  006A  GETMET	R2	R0	K9
      0x7C080200,  //  006B  CALL	R2	1
      0x70020022,  //  006C  JMP		#0090
      0x88080301,  //  006D  GETMBR	R2	R1	K1
      0x1C080521,  //  006E  EQ	R2	R2	K33
      0x780A0017,  //  006F  JMPF	R2	#0088
      0x8808010C,  //  0070  GETMBR	R2	R0	K12
      0x740A0001,  //  0071  JMPT	R2	#0074
      0x8C08010D,  //  0072  GETMET	R2	R0	K13
      0x7C080200,  //  0073  CALL	R2	1
      0x88080303,  //  0074  GETMBR	R2	R1	K3
      0x1C080522,  //  0075  EQ	R2	R2	K34
      0x780A000D,  //  0076  JMPF	R2	#0085
      0x8C080123,  //  0077  GETMET	R2	R0	K35
      0x7C080200,  //  0078  CALL	R2	1
      0x4C0C0000,  //  0079  LDNIL	R3
      0x20080403,  //  007A  NE	R2	R2	R3
      0x780A0008,  //  007B  JMPF	R2	#0085
      0x8C080123,  //  007C  GETMET	R2	R0	K35
      0x7C080200,  //  007D  CALL	R2	1
      0x88080501,  //  007E  GETMBR	R2	R2	K1
      0x540E0017,  //  007F  LDINT	R3	24
      0x1C080403,  //  0080  EQ	R2	R2	R3
      0x780A0002,  //  0081  JMPF	R2	#0085
      0x8C080124,  //  0082  GETMET	R2	R0	K36
      0x7C080200,  //  0083  CALL	R2	1
      0x70020001,  //  0084  JMP		#0087
      0x8C080125,  //  0085  GETMET	R2	R0	K37
      0x7C080200,  //  0086  CALL	R2	1
      0x70020007,  //  0087  JMP		#0090
      0x8C080107,  //  0088  GETMET	R2	R0	K7
      0x60100018,  //  0089  GETGBL	R4	G24
      0x58140026,  //  008A  LDCONST	R5	K38
      0x88180303,  //  008B  GETMBR	R6	R1	K3
      0x7C100400,  //  008C  CALL	R4	2
      0x7C080400,  //  008D  CALL	R2	2
      0x8C080109,  //  008E  GETMET	R2	R0	K9
      0x7C080200,  //  008F  CALL	R2	1
      0x80000000,  //  0090  RET	0
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
    ( &(const bvalue[67]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(error),
    /* K2   */  be_nested_str_weak(Expected_X20value),
    /* K3   */  be_nested_str_weak(ExpressionResult),
    /* K4   */  be_nested_str_weak(literal),
    /* K5   */  be_nested_str_weak(nil),
    /* K6   */  be_nested_str_weak(type),
    /* K7   */  be_nested_str_weak(next),
    /* K8   */  be_nested_str_weak(process_additive_expression),
    /* K9   */  be_nested_str_weak(expect_right_paren),
    /* K10  */  be_nested_str_weak(_X28_X25s_X29),
    /* K11  */  be_nested_str_weak(expr),
    /* K12  */  be_nested_str_weak(has_dynamic),
    /* K13  */  be_nested_str_weak(has_dangerous),
    /* K14  */  be_nested_str_weak(has_computation),
    /* K15  */  be_nested_str_weak(return_type),
    /* K16  */  be_nested_str_weak(instance_for_validation),
    /* K17  */  be_nested_str_weak(convert_color),
    /* K18  */  be_nested_str_weak(value),
    /* K19  */  be_nested_str_weak(process_time_value),
    /* K20  */  be_nested_str_weak(process_percentage_value),
    /* K21  */  be_const_int(2),
    /* K22  */  be_const_int(0),
    /* K23  */  be_nested_str_weak(true),
    /* K24  */  be_nested_str_weak(false),
    /* K25  */  be_const_int(3),
    /* K26  */  be_nested_str_weak(_X22_X25s_X22),
    /* K27  */  be_nested_str_weak(process_array_literal),
    /* K28  */  be_const_int(1),
    /* K29  */  be_nested_str_weak(peek),
    /* K30  */  be_nested_str_weak(symbol_table),
    /* K31  */  be_nested_str_weak(get),
    /* K32  */  be_nested_str_weak(Unknown_X20function_X20or_X20identifier_X20_X27_X25s_X27_X2E_X20Make_X20sure_X20it_X27s_X20defined_X20before_X20use_X2E),
    /* K33  */  be_nested_str_weak(skip_statement),
    /* K34  */  be_nested_str_weak(is_user_function),
    /* K35  */  be_nested_str_weak(_process_user_function_call),
    /* K36  */  be_nested_str_weak(function_call),
    /* K37  */  be_nested_str_weak(process_function_arguments),
    /* K38  */  be_nested_str_weak(_X25s_X28_X25s_X29),
    /* K39  */  be_nested_str_weak(get_reference),
    /* K40  */  be_nested_str_weak(),
    /* K41  */  be_nested_str_weak(engine_X2C_X20_X25s),
    /* K42  */  be_nested_str_weak(engine),
    /* K43  */  be_nested_str_weak(_X25s_template_X28_X25s_X29),
    /* K44  */  be_nested_str_weak(Function_X20_X27_X25s_X27_X20not_X20supported_X20in_X20expression_X20context),
    /* K45  */  be_nested_str_weak(takes_named_args),
    /* K46  */  be_nested_str_weak(process_function_call),
    /* K47  */  be_nested_str_weak(_determine_function_return_type),
    /* K48  */  be_nested_str_weak(instance),
    /* K49  */  be_nested_str_weak(CONTEXT_ARGUMENT),
    /* K50  */  be_nested_str_weak(CONTEXT_PROPERTY),
    /* K51  */  be_nested_str_weak(CONTEXT_VARIABLE),
    /* K52  */  be_nested_str_weak(process_nested_function_call),
    /* K53  */  be_nested_str_weak(constructor_call),
    /* K54  */  be_nested_str_weak(Unknown_X20identifier_X20_X27_X25s_X27_X2E_X20Make_X20sure_X20it_X27s_X20defined_X20before_X20use_X2E),
    /* K55  */  be_nested_str_weak(expect_identifier),
    /* K56  */  be_nested_str_weak(_X25s_X2E_X25s),
    /* K57  */  be_nested_str_weak(contains),
    /* K58  */  be_nested_str_weak(_validate_single_parameter),
    /* K59  */  be_nested_str_weak(Sequences_X20like_X20_X27_X25s_X27_X20do_X20not_X20have_X20properties_X2E_X20Property_X20references_X20are_X20only_X20valid_X20for_X20animations_X20and_X20color_X20providers_X2E),
    /* K60  */  be_nested_str_weak(property_access),
    /* K61  */  be_nested_str_weak(variable),
    /* K62  */  be_nested_str_weak(_determine_symbol_return_type),
    /* K63  */  be_nested_str_weak(animation_X2Eresolve_X28_X25s_X29),
    /* K64  */  be_nested_str_weak(variable_ref),
    /* K65  */  be_nested_str_weak(animation_X2E_X25s),
    /* K66  */  be_nested_str_weak(Unexpected_X20value_X3A_X20_X25s),
    }),
    be_str_weak(process_primary_expression),
    &be_const_str_solidified,
    ( &(const binstruction[471]) {  /* code */
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
      0x541A0017,  //  000E  LDINT	R6	24
      0x1C140A06,  //  000F  EQ	R5	R5	R6
      0x78160014,  //  0010  JMPF	R5	#0026
      0x8C140107,  //  0011  GETMET	R5	R0	K7
      0x7C140200,  //  0012  CALL	R5	1
      0x8C140108,  //  0013  GETMET	R5	R0	K8
      0x5C1C0200,  //  0014  MOVE	R7	R1
      0x50200000,  //  0015  LDBOOL	R8	0	0
      0x5C240600,  //  0016  MOVE	R9	R3
      0x7C140800,  //  0017  CALL	R5	4
      0x8C180109,  //  0018  GETMET	R6	R0	K9
      0x7C180200,  //  0019  CALL	R6	1
      0x8C180103,  //  001A  GETMET	R6	R0	K3
      0x60200018,  //  001B  GETGBL	R8	G24
      0x5824000A,  //  001C  LDCONST	R9	K10
      0x88280B0B,  //  001D  GETMBR	R10	R5	K11
      0x7C200400,  //  001E  CALL	R8	2
      0x88240B0C,  //  001F  GETMBR	R9	R5	K12
      0x88280B0D,  //  0020  GETMBR	R10	R5	K13
      0x882C0B0E,  //  0021  GETMBR	R11	R5	K14
      0x88300B0F,  //  0022  GETMBR	R12	R5	K15
      0x88340B10,  //  0023  GETMBR	R13	R5	K16
      0x7C180E00,  //  0024  CALL	R6	7
      0x80040C00,  //  0025  RET	1	R6
      0x88140906,  //  0026  GETMBR	R5	R4	K6
      0x541A0003,  //  0027  LDINT	R6	4
      0x1C140A06,  //  0028  EQ	R5	R5	R6
      0x78160009,  //  0029  JMPF	R5	#0034
      0x8C140107,  //  002A  GETMET	R5	R0	K7
      0x7C140200,  //  002B  CALL	R5	1
      0x88140103,  //  002C  GETMBR	R5	R0	K3
      0x8C140B04,  //  002D  GETMET	R5	R5	K4
      0x8C1C0111,  //  002E  GETMET	R7	R0	K17
      0x88240912,  //  002F  GETMBR	R9	R4	K18
      0x7C1C0400,  //  0030  CALL	R7	2
      0x5422000A,  //  0031  LDINT	R8	11
      0x7C140600,  //  0032  CALL	R5	3
      0x80040A00,  //  0033  RET	1	R5
      0x88140906,  //  0034  GETMBR	R5	R4	K6
      0x541A0004,  //  0035  LDINT	R6	5
      0x1C140A06,  //  0036  EQ	R5	R5	R6
      0x78160005,  //  0037  JMPF	R5	#003E
      0x88140103,  //  0038  GETMBR	R5	R0	K3
      0x8C140B04,  //  0039  GETMET	R5	R5	K4
      0x8C1C0113,  //  003A  GETMET	R7	R0	K19
      0x7C1C0200,  //  003B  CALL	R7	1
      0x7C140400,  //  003C  CALL	R5	2
      0x80040A00,  //  003D  RET	1	R5
      0x88140906,  //  003E  GETMBR	R5	R4	K6
      0x541A0005,  //  003F  LDINT	R6	6
      0x1C140A06,  //  0040  EQ	R5	R5	R6
      0x78160007,  //  0041  JMPF	R5	#004A
      0x88140103,  //  0042  GETMBR	R5	R0	K3
      0x8C140B04,  //  0043  GETMET	R5	R5	K4
      0x601C0008,  //  0044  GETGBL	R7	G8
      0x8C200114,  //  0045  GETMET	R8	R0	K20
      0x7C200200,  //  0046  CALL	R8	1
      0x7C1C0200,  //  0047  CALL	R7	1
      0x7C140400,  //  0048  CALL	R5	2
      0x80040A00,  //  0049  RET	1	R5
      0x88140906,  //  004A  GETMBR	R5	R4	K6
      0x1C140B15,  //  004B  EQ	R5	R5	K21
      0x78160007,  //  004C  JMPF	R5	#0055
      0x88140912,  //  004D  GETMBR	R5	R4	K18
      0x8C180107,  //  004E  GETMET	R6	R0	K7
      0x7C180200,  //  004F  CALL	R6	1
      0x88180103,  //  0050  GETMBR	R6	R0	K3
      0x8C180D04,  //  0051  GETMET	R6	R6	K4
      0x5C200A00,  //  0052  MOVE	R8	R5
      0x7C180400,  //  0053  CALL	R6	2
      0x80040C00,  //  0054  RET	1	R6
      0x88140906,  //  0055  GETMBR	R5	R4	K6
      0x1C140B16,  //  0056  EQ	R5	R5	K22
      0x7816000D,  //  0057  JMPF	R5	#0066
      0x88140912,  //  0058  GETMBR	R5	R4	K18
      0x1C140B17,  //  0059  EQ	R5	R5	K23
      0x74160002,  //  005A  JMPT	R5	#005E
      0x88140912,  //  005B  GETMBR	R5	R4	K18
      0x1C140B18,  //  005C  EQ	R5	R5	K24
      0x78160007,  //  005D  JMPF	R5	#0066
      0x88140912,  //  005E  GETMBR	R5	R4	K18
      0x8C180107,  //  005F  GETMET	R6	R0	K7
      0x7C180200,  //  0060  CALL	R6	1
      0x88180103,  //  0061  GETMBR	R6	R0	K3
      0x8C180D04,  //  0062  GETMET	R6	R6	K4
      0x5C200A00,  //  0063  MOVE	R8	R5
      0x7C180400,  //  0064  CALL	R6	2
      0x80040C00,  //  0065  RET	1	R6
      0x88140906,  //  0066  GETMBR	R5	R4	K6
      0x1C140B19,  //  0067  EQ	R5	R5	K25
      0x7816000A,  //  0068  JMPF	R5	#0074
      0x88140912,  //  0069  GETMBR	R5	R4	K18
      0x8C180107,  //  006A  GETMET	R6	R0	K7
      0x7C180200,  //  006B  CALL	R6	1
      0x88180103,  //  006C  GETMBR	R6	R0	K3
      0x8C180D04,  //  006D  GETMET	R6	R6	K4
      0x60200018,  //  006E  GETGBL	R8	G24
      0x5824001A,  //  006F  LDCONST	R9	K26
      0x5C280A00,  //  0070  MOVE	R10	R5
      0x7C200400,  //  0071  CALL	R8	2
      0x7C180400,  //  0072  CALL	R6	2
      0x80040C00,  //  0073  RET	1	R6
      0x88140906,  //  0074  GETMBR	R5	R4	K6
      0x541A001B,  //  0075  LDINT	R6	28
      0x1C140A06,  //  0076  EQ	R5	R5	R6
      0x78160008,  //  0077  JMPF	R5	#0081
      0x5C140600,  //  0078  MOVE	R5	R3
      0x74160006,  //  0079  JMPT	R5	#0081
      0x8C14011B,  //  007A  GETMET	R5	R0	K27
      0x7C140200,  //  007B  CALL	R5	1
      0x88180103,  //  007C  GETMBR	R6	R0	K3
      0x8C180D04,  //  007D  GETMET	R6	R6	K4
      0x5C200A00,  //  007E  MOVE	R8	R5
      0x7C180400,  //  007F  CALL	R6	2
      0x80040C00,  //  0080  RET	1	R6
      0x88140906,  //  0081  GETMBR	R5	R4	K6
      0x1C140B16,  //  0082  EQ	R5	R5	K22
      0x74160002,  //  0083  JMPT	R5	#0087
      0x88140906,  //  0084  GETMBR	R5	R4	K6
      0x1C140B1C,  //  0085  EQ	R5	R5	K28
      0x7816009D,  //  0086  JMPF	R5	#0125
      0x8C14011D,  //  0087  GETMET	R5	R0	K29
      0x7C140200,  //  0088  CALL	R5	1
      0x4C180000,  //  0089  LDNIL	R6
      0x20140A06,  //  008A  NE	R5	R5	R6
      0x78160098,  //  008B  JMPF	R5	#0125
      0x8C14011D,  //  008C  GETMET	R5	R0	K29
      0x7C140200,  //  008D  CALL	R5	1
      0x88140B06,  //  008E  GETMBR	R5	R5	K6
      0x541A0017,  //  008F  LDINT	R6	24
      0x1C140A06,  //  0090  EQ	R5	R5	R6
      0x78160092,  //  0091  JMPF	R5	#0125
      0x88140912,  //  0092  GETMBR	R5	R4	K18
      0x8818011E,  //  0093  GETMBR	R6	R0	K30
      0x8C180D1F,  //  0094  GETMET	R6	R6	K31
      0x5C200A00,  //  0095  MOVE	R8	R5
      0x7C180400,  //  0096  CALL	R6	2
      0x4C1C0000,  //  0097  LDNIL	R7
      0x1C1C0C07,  //  0098  EQ	R7	R6	R7
      0x781E000C,  //  0099  JMPF	R7	#00A7
      0x8C1C0101,  //  009A  GETMET	R7	R0	K1
      0x60240018,  //  009B  GETGBL	R9	G24
      0x58280020,  //  009C  LDCONST	R10	K32
      0x5C2C0A00,  //  009D  MOVE	R11	R5
      0x7C240400,  //  009E  CALL	R9	2
      0x7C1C0400,  //  009F  CALL	R7	2
      0x8C1C0121,  //  00A0  GETMET	R7	R0	K33
      0x7C1C0200,  //  00A1  CALL	R7	1
      0x881C0103,  //  00A2  GETMBR	R7	R0	K3
      0x8C1C0F04,  //  00A3  GETMET	R7	R7	K4
      0x58240005,  //  00A4  LDCONST	R9	K5
      0x7C1C0400,  //  00A5  CALL	R7	2
      0x80040E00,  //  00A6  RET	1	R7
      0x8C1C0D22,  //  00A7  GETMET	R7	R6	K34
      0x7C1C0200,  //  00A8  CALL	R7	1
      0x781E0009,  //  00A9  JMPF	R7	#00B4
      0x8C1C0107,  //  00AA  GETMET	R7	R0	K7
      0x7C1C0200,  //  00AB  CALL	R7	1
      0x8C1C0123,  //  00AC  GETMET	R7	R0	K35
      0x5C240A00,  //  00AD  MOVE	R9	R5
      0x7C1C0400,  //  00AE  CALL	R7	2
      0x88200103,  //  00AF  GETMBR	R8	R0	K3
      0x8C201124,  //  00B0  GETMET	R8	R8	K36
      0x5C280E00,  //  00B1  MOVE	R10	R7
      0x7C200400,  //  00B2  CALL	R8	2
      0x80041000,  //  00B3  RET	1	R8
      0x780E003B,  //  00B4  JMPF	R3	#00F1
      0x8C1C0107,  //  00B5  GETMET	R7	R0	K7
      0x7C1C0200,  //  00B6  CALL	R7	1
      0x4C1C0000,  //  00B7  LDNIL	R7
      0x201C0C07,  //  00B8  NE	R7	R6	R7
      0x781E000F,  //  00B9  JMPF	R7	#00CA
      0x881C0D06,  //  00BA  GETMBR	R7	R6	K6
      0x54220003,  //  00BB  LDINT	R8	4
      0x1C1C0E08,  //  00BC  EQ	R7	R7	R8
      0x781E000B,  //  00BD  JMPF	R7	#00CA
      0x8C1C0125,  //  00BE  GETMET	R7	R0	K37
      0x50240200,  //  00BF  LDBOOL	R9	1	0
      0x7C1C0400,  //  00C0  CALL	R7	2
      0x88200103,  //  00C1  GETMBR	R8	R0	K3
      0x8C201124,  //  00C2  GETMET	R8	R8	K36
      0x60280018,  //  00C3  GETGBL	R10	G24
      0x582C0026,  //  00C4  LDCONST	R11	K38
      0x8C300D27,  //  00C5  GETMET	R12	R6	K39
      0x7C300200,  //  00C6  CALL	R12	1
      0x5C340E00,  //  00C7  MOVE	R13	R7
      0x7C280600,  //  00C8  CALL	R10	3
      0x7C200400,  //  00C9  CALL	R8	2
      0x4C1C0000,  //  00CA  LDNIL	R7
      0x201C0C07,  //  00CB  NE	R7	R6	R7
      0x781E0017,  //  00CC  JMPF	R7	#00E5
      0x881C0D06,  //  00CD  GETMBR	R7	R6	K6
      0x5422000D,  //  00CE  LDINT	R8	14
      0x1C1C0E08,  //  00CF  EQ	R7	R7	R8
      0x781E0013,  //  00D0  JMPF	R7	#00E5
      0x8C1C0125,  //  00D1  GETMET	R7	R0	K37
      0x50240200,  //  00D2  LDBOOL	R9	1	0
      0x7C1C0400,  //  00D3  CALL	R7	2
      0x20200F28,  //  00D4  NE	R8	R7	K40
      0x78220004,  //  00D5  JMPF	R8	#00DB
      0x60200018,  //  00D6  GETGBL	R8	G24
      0x58240029,  //  00D7  LDCONST	R9	K41
      0x5C280E00,  //  00D8  MOVE	R10	R7
      0x7C200400,  //  00D9  CALL	R8	2
      0x70020000,  //  00DA  JMP		#00DC
      0x5820002A,  //  00DB  LDCONST	R8	K42
      0x88240103,  //  00DC  GETMBR	R9	R0	K3
      0x8C241324,  //  00DD  GETMET	R9	R9	K36
      0x602C0018,  //  00DE  GETGBL	R11	G24
      0x5830002B,  //  00DF  LDCONST	R12	K43
      0x5C340A00,  //  00E0  MOVE	R13	R5
      0x5C381000,  //  00E1  MOVE	R14	R8
      0x7C2C0600,  //  00E2  CALL	R11	3
      0x7C240400,  //  00E3  CALL	R9	2
      0x80041200,  //  00E4  RET	1	R9
      0x8C1C0101,  //  00E5  GETMET	R7	R0	K1
      0x60240018,  //  00E6  GETGBL	R9	G24
      0x5828002C,  //  00E7  LDCONST	R10	K44
      0x5C2C0A00,  //  00E8  MOVE	R11	R5
      0x7C240400,  //  00E9  CALL	R9	2
      0x7C1C0400,  //  00EA  CALL	R7	2
      0x881C0103,  //  00EB  GETMBR	R7	R0	K3
      0x8C1C0F04,  //  00EC  GETMET	R7	R7	K4
      0x58240005,  //  00ED  LDCONST	R9	K5
      0x7C1C0400,  //  00EE  CALL	R7	2
      0x80040E00,  //  00EF  RET	1	R7
      0x70020033,  //  00F0  JMP		#0125
      0x8C1C0D2D,  //  00F1  GETMET	R7	R6	K45
      0x7C1C0200,  //  00F2  CALL	R7	1
      0x741E000D,  //  00F3  JMPT	R7	#0102
      0x8C1C012E,  //  00F4  GETMET	R7	R0	K46
      0x5C240200,  //  00F5  MOVE	R9	R1
      0x7C1C0400,  //  00F6  CALL	R7	2
      0x8C20012F,  //  00F7  GETMET	R8	R0	K47
      0x5C280C00,  //  00F8  MOVE	R10	R6
      0x7C200400,  //  00F9  CALL	R8	2
      0x88240103,  //  00FA  GETMBR	R9	R0	K3
      0x8C241324,  //  00FB  GETMET	R9	R9	K36
      0x5C2C0E00,  //  00FC  MOVE	R11	R7
      0x5C301000,  //  00FD  MOVE	R12	R8
      0x88340D30,  //  00FE  GETMBR	R13	R6	K48
      0x7C240800,  //  00FF  CALL	R9	4
      0x80041200,  //  0100  RET	1	R9
      0x70020022,  //  0101  JMP		#0125
      0x881C0131,  //  0102  GETMBR	R7	R0	K49
      0x1C1C0207,  //  0103  EQ	R7	R1	R7
      0x741E0005,  //  0104  JMPT	R7	#010B
      0x881C0132,  //  0105  GETMBR	R7	R0	K50
      0x1C1C0207,  //  0106  EQ	R7	R1	R7
      0x741E0002,  //  0107  JMPT	R7	#010B
      0x881C0133,  //  0108  GETMBR	R7	R0	K51
      0x1C1C0207,  //  0109  EQ	R7	R1	R7
      0x781E000C,  //  010A  JMPF	R7	#0118
      0x8C1C0134,  //  010B  GETMET	R7	R0	K52
      0x7C1C0200,  //  010C  CALL	R7	1
      0x8C20012F,  //  010D  GETMET	R8	R0	K47
      0x5C280C00,  //  010E  MOVE	R10	R6
      0x7C200400,  //  010F  CALL	R8	2
      0x88240103,  //  0110  GETMBR	R9	R0	K3
      0x8C241335,  //  0111  GETMET	R9	R9	K53
      0x5C2C0E00,  //  0112  MOVE	R11	R7
      0x5C301000,  //  0113  MOVE	R12	R8
      0x88340D30,  //  0114  GETMBR	R13	R6	K48
      0x7C240800,  //  0115  CALL	R9	4
      0x80041200,  //  0116  RET	1	R9
      0x7002000C,  //  0117  JMP		#0125
      0x8C1C012E,  //  0118  GETMET	R7	R0	K46
      0x5C240200,  //  0119  MOVE	R9	R1
      0x7C1C0400,  //  011A  CALL	R7	2
      0x8C20012F,  //  011B  GETMET	R8	R0	K47
      0x5C280C00,  //  011C  MOVE	R10	R6
      0x7C200400,  //  011D  CALL	R8	2
      0x88240103,  //  011E  GETMBR	R9	R0	K3
      0x8C241335,  //  011F  GETMET	R9	R9	K53
      0x5C2C0E00,  //  0120  MOVE	R11	R7
      0x5C301000,  //  0121  MOVE	R12	R8
      0x88340D30,  //  0122  GETMBR	R13	R6	K48
      0x7C240800,  //  0123  CALL	R9	4
      0x80041200,  //  0124  RET	1	R9
      0x88140906,  //  0125  GETMBR	R5	R4	K6
      0x1C140B1C,  //  0126  EQ	R5	R5	K28
      0x78160093,  //  0127  JMPF	R5	#01BC
      0x88140912,  //  0128  GETMBR	R5	R4	K18
      0x8818011E,  //  0129  GETMBR	R6	R0	K30
      0x8C180D1F,  //  012A  GETMET	R6	R6	K31
      0x5C200A00,  //  012B  MOVE	R8	R5
      0x7C180400,  //  012C  CALL	R6	2
      0x4C1C0000,  //  012D  LDNIL	R7
      0x1C1C0C07,  //  012E  EQ	R7	R6	R7
      0x781E000C,  //  012F  JMPF	R7	#013D
      0x8C1C0101,  //  0130  GETMET	R7	R0	K1
      0x60240018,  //  0131  GETGBL	R9	G24
      0x58280036,  //  0132  LDCONST	R10	K54
      0x5C2C0A00,  //  0133  MOVE	R11	R5
      0x7C240400,  //  0134  CALL	R9	2
      0x7C1C0400,  //  0135  CALL	R7	2
      0x8C1C0121,  //  0136  GETMET	R7	R0	K33
      0x7C1C0200,  //  0137  CALL	R7	1
      0x881C0103,  //  0138  GETMBR	R7	R0	K3
      0x8C1C0F04,  //  0139  GETMET	R7	R7	K4
      0x58240005,  //  013A  LDCONST	R9	K5
      0x7C1C0400,  //  013B  CALL	R7	2
      0x80040E00,  //  013C  RET	1	R7
      0x8C1C0107,  //  013D  GETMET	R7	R0	K7
      0x7C1C0200,  //  013E  CALL	R7	1
      0x8C1C0100,  //  013F  GETMET	R7	R0	K0
      0x7C1C0200,  //  0140  CALL	R7	1
      0x4C200000,  //  0141  LDNIL	R8
      0x201C0E08,  //  0142  NE	R7	R7	R8
      0x781E0045,  //  0143  JMPF	R7	#018A
      0x8C1C0100,  //  0144  GETMET	R7	R0	K0
      0x7C1C0200,  //  0145  CALL	R7	1
      0x881C0F06,  //  0146  GETMBR	R7	R7	K6
      0x54220020,  //  0147  LDINT	R8	33
      0x1C1C0E08,  //  0148  EQ	R7	R7	R8
      0x781E003F,  //  0149  JMPF	R7	#018A
      0x8C1C0107,  //  014A  GETMET	R7	R0	K7
      0x7C1C0200,  //  014B  CALL	R7	1
      0x8C1C0137,  //  014C  GETMET	R7	R0	K55
      0x7C1C0200,  //  014D  CALL	R7	1
      0x60200018,  //  014E  GETGBL	R8	G24
      0x58240038,  //  014F  LDCONST	R9	K56
      0x5C280A00,  //  0150  MOVE	R10	R5
      0x5C2C0E00,  //  0151  MOVE	R11	R7
      0x7C200600,  //  0152  CALL	R8	3
      0x5C240600,  //  0153  MOVE	R9	R3
      0x74260026,  //  0154  JMPT	R9	#017C
      0x8824011E,  //  0155  GETMBR	R9	R0	K30
      0x8C241339,  //  0156  GETMET	R9	R9	K57
      0x5C2C0A00,  //  0157  MOVE	R11	R5
      0x7C240400,  //  0158  CALL	R9	2
      0x78260021,  //  0159  JMPF	R9	#017C
      0x4C240000,  //  015A  LDNIL	R9
      0x20240C09,  //  015B  NE	R9	R6	R9
      0x7826000C,  //  015C  JMPF	R9	#016A
      0x88240D30,  //  015D  GETMBR	R9	R6	K48
      0x4C280000,  //  015E  LDNIL	R10
      0x2024120A,  //  015F  NE	R9	R9	R10
      0x78260008,  //  0160  JMPF	R9	#016A
      0x60240005,  //  0161  GETGBL	R9	G5
      0x88280D30,  //  0162  GETMBR	R10	R6	K48
      0x7C240200,  //  0163  CALL	R9	1
      0x8C28013A,  //  0164  GETMET	R10	R0	K58
      0x5C301200,  //  0165  MOVE	R12	R9
      0x5C340E00,  //  0166  MOVE	R13	R7
      0x88380D30,  //  0167  GETMBR	R14	R6	K48
      0x7C280800,  //  0168  CALL	R10	4
      0x70020011,  //  0169  JMP		#017C
      0x4C240000,  //  016A  LDNIL	R9
      0x20240C09,  //  016B  NE	R9	R6	R9
      0x7826000E,  //  016C  JMPF	R9	#017C
      0x88240D06,  //  016D  GETMBR	R9	R6	K6
      0x542A000C,  //  016E  LDINT	R10	13
      0x1C24120A,  //  016F  EQ	R9	R9	R10
      0x7826000A,  //  0170  JMPF	R9	#017C
      0x8C240101,  //  0171  GETMET	R9	R0	K1
      0x602C0018,  //  0172  GETGBL	R11	G24
      0x5830003B,  //  0173  LDCONST	R12	K59
      0x5C340A00,  //  0174  MOVE	R13	R5
      0x7C2C0400,  //  0175  CALL	R11	2
      0x7C240400,  //  0176  CALL	R9	2
      0x88240103,  //  0177  GETMBR	R9	R0	K3
      0x8C241304,  //  0178  GETMET	R9	R9	K4
      0x582C0005,  //  0179  LDCONST	R11	K5
      0x7C240400,  //  017A  CALL	R9	2
      0x80041200,  //  017B  RET	1	R9
      0x8824011E,  //  017C  GETMBR	R9	R0	K30
      0x8C241327,  //  017D  GETMET	R9	R9	K39
      0x5C2C0A00,  //  017E  MOVE	R11	R5
      0x7C240400,  //  017F  CALL	R9	2
      0x88280103,  //  0180  GETMBR	R10	R0	K3
      0x8C28153C,  //  0181  GETMET	R10	R10	K60
      0x60300018,  //  0182  GETGBL	R12	G24
      0x58340038,  //  0183  LDCONST	R13	K56
      0x5C381200,  //  0184  MOVE	R14	R9
      0x5C3C0E00,  //  0185  MOVE	R15	R7
      0x7C300600,  //  0186  CALL	R12	3
      0x5834003D,  //  0187  LDCONST	R13	K61
      0x7C280600,  //  0188  CALL	R10	3
      0x80041400,  //  0189  RET	1	R10
      0x881C0D06,  //  018A  GETMBR	R7	R6	K6
      0x5422000A,  //  018B  LDINT	R8	11
      0x1C1C0E08,  //  018C  EQ	R7	R7	R8
      0x741E0008,  //  018D  JMPT	R7	#0197
      0x881C0D06,  //  018E  GETMBR	R7	R6	K6
      0x1C1C0F15,  //  018F  EQ	R7	R7	K21
      0x741E0005,  //  0190  JMPT	R7	#0197
      0x881C0D06,  //  0191  GETMBR	R7	R6	K6
      0x1C1C0F1C,  //  0192  EQ	R7	R7	K28
      0x741E0002,  //  0193  JMPT	R7	#0197
      0x881C0D06,  //  0194  GETMBR	R7	R6	K6
      0x1C1C0F19,  //  0195  EQ	R7	R7	K25
      0x781E0006,  //  0196  JMPF	R7	#019E
      0x881C0103,  //  0197  GETMBR	R7	R0	K3
      0x8C1C0F04,  //  0198  GETMET	R7	R7	K4
      0x8C240D27,  //  0199  GETMET	R9	R6	K39
      0x7C240200,  //  019A  CALL	R9	1
      0x542A000A,  //  019B  LDINT	R10	11
      0x7C1C0600,  //  019C  CALL	R7	3
      0x80040E00,  //  019D  RET	1	R7
      0x881C011E,  //  019E  GETMBR	R7	R0	K30
      0x8C1C0F27,  //  019F  GETMET	R7	R7	K39
      0x5C240A00,  //  01A0  MOVE	R9	R5
      0x7C1C0400,  //  01A1  CALL	R7	2
      0x8C20013E,  //  01A2  GETMET	R8	R0	K62
      0x5C280C00,  //  01A3  MOVE	R10	R6
      0x7C200400,  //  01A4  CALL	R8	2
      0x88240D06,  //  01A5  GETMBR	R9	R6	K6
      0x542A0006,  //  01A6  LDINT	R10	7
      0x1C24120A,  //  01A7  EQ	R9	R9	R10
      0x74260003,  //  01A8  JMPT	R9	#01AD
      0x88240D06,  //  01A9  GETMBR	R9	R6	K6
      0x542A000B,  //  01AA  LDINT	R10	12
      0x1C24120A,  //  01AB  EQ	R9	R9	R10
      0x78260008,  //  01AC  JMPF	R9	#01B6
      0x88240103,  //  01AD  GETMBR	R9	R0	K3
      0x8C241324,  //  01AE  GETMET	R9	R9	K36
      0x602C0018,  //  01AF  GETGBL	R11	G24
      0x5830003F,  //  01B0  LDCONST	R12	K63
      0x5C340E00,  //  01B1  MOVE	R13	R7
      0x7C2C0400,  //  01B2  CALL	R11	2
      0x5C301000,  //  01B3  MOVE	R12	R8
      0x7C240600,  //  01B4  CALL	R9	3
      0x80041200,  //  01B5  RET	1	R9
      0x88240103,  //  01B6  GETMBR	R9	R0	K3
      0x8C241340,  //  01B7  GETMET	R9	R9	K64
      0x5C2C0E00,  //  01B8  MOVE	R11	R7
      0x5C301000,  //  01B9  MOVE	R12	R8
      0x7C240600,  //  01BA  CALL	R9	3
      0x80041200,  //  01BB  RET	1	R9
      0x88140906,  //  01BC  GETMBR	R5	R4	K6
      0x1C140B16,  //  01BD  EQ	R5	R5	K22
      0x7816000A,  //  01BE  JMPF	R5	#01CA
      0x88140912,  //  01BF  GETMBR	R5	R4	K18
      0x8C180107,  //  01C0  GETMET	R6	R0	K7
      0x7C180200,  //  01C1  CALL	R6	1
      0x88180103,  //  01C2  GETMBR	R6	R0	K3
      0x8C180D04,  //  01C3  GETMET	R6	R6	K4
      0x60200018,  //  01C4  GETGBL	R8	G24
      0x58240041,  //  01C5  LDCONST	R9	K65
      0x5C280A00,  //  01C6  MOVE	R10	R5
      0x7C200400,  //  01C7  CALL	R8	2
      0x7C180400,  //  01C8  CALL	R6	2
      0x80040C00,  //  01C9  RET	1	R6
      0x8C140101,  //  01CA  GETMET	R5	R0	K1
      0x601C0018,  //  01CB  GETGBL	R7	G24
      0x58200042,  //  01CC  LDCONST	R8	K66
      0x88240912,  //  01CD  GETMBR	R9	R4	K18
      0x7C1C0400,  //  01CE  CALL	R7	2
      0x7C140400,  //  01CF  CALL	R5	2
      0x8C140121,  //  01D0  GETMET	R5	R0	K33
      0x7C140200,  //  01D1  CALL	R5	1
      0x88140103,  //  01D2  GETMBR	R5	R0	K3
      0x8C140B04,  //  01D3  GETMET	R5	R5	K4
      0x581C0005,  //  01D4  LDCONST	R7	K5
      0x7C140400,  //  01D5  CALL	R5	2
      0x80040A00,  //  01D6  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: error
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_error,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(line),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(Line_X20_X25s_X3A_X20_X25s),
    /* K4   */  be_nested_str_weak(dsl_compilation_error),
    }),
    be_str_weak(error),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
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
      0x600C0018,  //  000A  GETGBL	R3	G24
      0x58100003,  //  000B  LDCONST	R4	K3
      0x5C140400,  //  000C  MOVE	R5	R2
      0x5C180200,  //  000D  MOVE	R6	R1
      0x7C0C0600,  //  000E  CALL	R3	3
      0xB0060803,  //  000F  RAISE	1	K4	R3
      0x80000000,  //  0010  RET	0
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
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(next),
    /* K3   */  be_nested_str_weak(error),
    /* K4   */  be_nested_str_weak(Expected_X20_X27_X2E_X27),
    }),
    be_str_weak(expect_dot),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0006,  //  0004  JMPF	R2	#000C
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0x540E0020,  //  0006  LDINT	R3	33
      0x1C080403,  //  0007  EQ	R2	R2	R3
      0x780A0002,  //  0008  JMPF	R2	#000C
      0x8C080102,  //  0009  GETMET	R2	R0	K2
      0x7C080200,  //  000A  CALL	R2	1
      0x70020002,  //  000B  JMP		#000F
      0x8C080103,  //  000C  GETMET	R2	R0	K3
      0x58100004,  //  000D  LDCONST	R4	K4
      0x7C080400,  //  000E  CALL	R2	2
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_template
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_template,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[22]) {     /* constants */
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
    /* K11  */  be_const_int(0),
    /* K12  */  be_nested_str_weak(value),
    /* K13  */  be_nested_str_weak(param),
    /* K14  */  be_nested_str_weak(_validate_template_parameter_name),
    /* K15  */  be_nested_str_weak(_validate_template_parameter_type),
    /* K16  */  be_nested_str_weak(push),
    /* K17  */  be_nested_str_weak(generate_template_function_direct),
    /* K18  */  be_nested_str_weak(params),
    /* K19  */  be_nested_str_weak(param_types),
    /* K20  */  be_nested_str_weak(symbol_table),
    /* K21  */  be_nested_str_weak(create_template),
    }),
    be_str_weak(process_template),
    &be_const_str_solidified,
    ( &(const binstruction[123]) {  /* code */
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
      0x60100013,  //  0012  GETGBL	R4	G19
      0x7C100000,  //  0013  CALL	R4	0
      0x8C140106,  //  0014  GETMET	R5	R0	K6
      0x7C140200,  //  0015  CALL	R5	1
      0x74160054,  //  0016  JMPT	R5	#006C
      0x8C140107,  //  0017  GETMET	R5	R0	K7
      0x7C140200,  //  0018  CALL	R5	1
      0x74160051,  //  0019  JMPT	R5	#006C
      0x8C140108,  //  001A  GETMET	R5	R0	K8
      0x7C140200,  //  001B  CALL	R5	1
      0x8C140107,  //  001C  GETMET	R5	R0	K7
      0x7C140200,  //  001D  CALL	R5	1
      0x78160000,  //  001E  JMPF	R5	#0020
      0x7002004B,  //  001F  JMP		#006C
      0x8C140109,  //  0020  GETMET	R5	R0	K9
      0x7C140200,  //  0021  CALL	R5	1
      0x4C180000,  //  0022  LDNIL	R6
      0x20180A06,  //  0023  NE	R6	R5	R6
      0x781A0044,  //  0024  JMPF	R6	#006A
      0x88180B0A,  //  0025  GETMBR	R6	R5	K10
      0x1C180D0B,  //  0026  EQ	R6	R6	K11
      0x781A0041,  //  0027  JMPF	R6	#006A
      0x88180B0C,  //  0028  GETMBR	R6	R5	K12
      0x1C180D0D,  //  0029  EQ	R6	R6	K13
      0x781A003E,  //  002A  JMPF	R6	#006A
      0x8C180100,  //  002B  GETMET	R6	R0	K0
      0x7C180200,  //  002C  CALL	R6	1
      0x8C180101,  //  002D  GETMET	R6	R0	K1
      0x7C180200,  //  002E  CALL	R6	1
      0x8C1C010E,  //  002F  GETMET	R7	R0	K14
      0x5C240C00,  //  0030  MOVE	R9	R6
      0x5C280800,  //  0031  MOVE	R10	R4
      0x7C1C0600,  //  0032  CALL	R7	3
      0x741E0002,  //  0033  JMPT	R7	#0037
      0x8C1C0104,  //  0034  GETMET	R7	R0	K4
      0x7C1C0200,  //  0035  CALL	R7	1
      0x80000E00,  //  0036  RET	0
      0x4C1C0000,  //  0037  LDNIL	R7
      0x8C200109,  //  0038  GETMET	R8	R0	K9
      0x7C200200,  //  0039  CALL	R8	1
      0x4C240000,  //  003A  LDNIL	R9
      0x20201009,  //  003B  NE	R8	R8	R9
      0x78220015,  //  003C  JMPF	R8	#0053
      0x8C200109,  //  003D  GETMET	R8	R0	K9
      0x7C200200,  //  003E  CALL	R8	1
      0x8820110A,  //  003F  GETMBR	R8	R8	K10
      0x1C20110B,  //  0040  EQ	R8	R8	K11
      0x78220010,  //  0041  JMPF	R8	#0053
      0x8C200109,  //  0042  GETMET	R8	R0	K9
      0x7C200200,  //  0043  CALL	R8	1
      0x8820110C,  //  0044  GETMBR	R8	R8	K12
      0x1C20110A,  //  0045  EQ	R8	R8	K10
      0x7822000B,  //  0046  JMPF	R8	#0053
      0x8C200100,  //  0047  GETMET	R8	R0	K0
      0x7C200200,  //  0048  CALL	R8	1
      0x8C200101,  //  0049  GETMET	R8	R0	K1
      0x7C200200,  //  004A  CALL	R8	1
      0x5C1C1000,  //  004B  MOVE	R7	R8
      0x8C20010F,  //  004C  GETMET	R8	R0	K15
      0x5C280E00,  //  004D  MOVE	R10	R7
      0x7C200400,  //  004E  CALL	R8	2
      0x74220002,  //  004F  JMPT	R8	#0053
      0x8C200104,  //  0050  GETMET	R8	R0	K4
      0x7C200200,  //  0051  CALL	R8	1
      0x80001000,  //  0052  RET	0
      0x8C200510,  //  0053  GETMET	R8	R2	K16
      0x5C280C00,  //  0054  MOVE	R10	R6
      0x7C200400,  //  0055  CALL	R8	2
      0x50200200,  //  0056  LDBOOL	R8	1	0
      0x98100C08,  //  0057  SETIDX	R4	R6	R8
      0x4C200000,  //  0058  LDNIL	R8
      0x20200E08,  //  0059  NE	R8	R7	R8
      0x78220000,  //  005A  JMPF	R8	#005C
      0x980C0C07,  //  005B  SETIDX	R3	R6	R7
      0x8C200109,  //  005C  GETMET	R8	R0	K9
      0x7C200200,  //  005D  CALL	R8	1
      0x4C240000,  //  005E  LDNIL	R9
      0x20201009,  //  005F  NE	R8	R8	R9
      0x78220007,  //  0060  JMPF	R8	#0069
      0x8C200109,  //  0061  GETMET	R8	R0	K9
      0x7C200200,  //  0062  CALL	R8	1
      0x8820110A,  //  0063  GETMBR	R8	R8	K10
      0x54260022,  //  0064  LDINT	R9	35
      0x1C201009,  //  0065  EQ	R8	R8	R9
      0x78220001,  //  0066  JMPF	R8	#0069
      0x8C200100,  //  0067  GETMET	R8	R0	K0
      0x7C200200,  //  0068  CALL	R8	1
      0x70020000,  //  0069  JMP		#006B
      0x70020000,  //  006A  JMP		#006C
      0x7001FFA7,  //  006B  JMP		#0014
      0x8C140111,  //  006C  GETMET	R5	R0	K17
      0x5C1C0200,  //  006D  MOVE	R7	R1
      0x5C200400,  //  006E  MOVE	R8	R2
      0x5C240600,  //  006F  MOVE	R9	R3
      0x7C140800,  //  0070  CALL	R5	4
      0x60140013,  //  0071  GETGBL	R5	G19
      0x7C140000,  //  0072  CALL	R5	0
      0x98162402,  //  0073  SETIDX	R5	K18	R2
      0x98162603,  //  0074  SETIDX	R5	K19	R3
      0x88180114,  //  0075  GETMBR	R6	R0	K20
      0x8C180D15,  //  0076  GETMET	R6	R6	K21
      0x5C200200,  //  0077  MOVE	R8	R1
      0x5C240A00,  //  0078  MOVE	R9	R5
      0x7C180600,  //  0079  CALL	R6	3
      0x80000000,  //  007A  RET	0
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
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(next),
    /* K3   */  be_const_int(1),
    /* K4   */  be_nested_str_weak(at_end),
    /* K5   */  be_const_int(0),
    }),
    be_str_weak(skip_function_arguments),
    &be_const_str_solidified,
    ( &(const binstruction[36]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20040202,  //  0003  NE	R1	R1	R2
      0x7806001D,  //  0004  JMPF	R1	#0023
      0x8C040100,  //  0005  GETMET	R1	R0	K0
      0x7C040200,  //  0006  CALL	R1	1
      0x88040301,  //  0007  GETMBR	R1	R1	K1
      0x540A0017,  //  0008  LDINT	R2	24
      0x1C040202,  //  0009  EQ	R1	R1	R2
      0x78060017,  //  000A  JMPF	R1	#0023
      0x8C040102,  //  000B  GETMET	R1	R0	K2
      0x7C040200,  //  000C  CALL	R1	1
      0x58040003,  //  000D  LDCONST	R1	K3
      0x8C080104,  //  000E  GETMET	R2	R0	K4
      0x7C080200,  //  000F  CALL	R2	1
      0x740A0011,  //  0010  JMPT	R2	#0023
      0x24080305,  //  0011  GT	R2	R1	K5
      0x780A000F,  //  0012  JMPF	R2	#0023
      0x8C080100,  //  0013  GETMET	R2	R0	K0
      0x7C080200,  //  0014  CALL	R2	1
      0x880C0501,  //  0015  GETMBR	R3	R2	K1
      0x54120017,  //  0016  LDINT	R4	24
      0x1C0C0604,  //  0017  EQ	R3	R3	R4
      0x780E0001,  //  0018  JMPF	R3	#001B
      0x00040303,  //  0019  ADD	R1	R1	K3
      0x70020004,  //  001A  JMP		#0020
      0x880C0501,  //  001B  GETMBR	R3	R2	K1
      0x54120018,  //  001C  LDINT	R4	25
      0x1C0C0604,  //  001D  EQ	R3	R3	R4
      0x780E0000,  //  001E  JMPF	R3	#0020
      0x04040303,  //  001F  SUB	R1	R1	K3
      0x8C0C0102,  //  0020  GETMET	R3	R0	K2
      0x7C0C0200,  //  0021  CALL	R3	1
      0x7001FFEA,  //  0022  JMP		#000E
      0x80000000,  //  0023  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: at_end
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_at_end,   /* name */
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
    /* K0   */  be_nested_str_weak(pull_lexer),
    /* K1   */  be_nested_str_weak(at_end),
    }),
    be_str_weak(at_end),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_sequence_statement
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_sequence_statement,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[24]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(add),
    /* K3   */  be_nested_str_weak(get_indent),
    /* K4   */  be_nested_str_weak(value),
    /* K5   */  be_nested_str_weak(next),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(play),
    /* K8   */  be_nested_str_weak(process_play_statement_fluent),
    /* K9   */  be_nested_str_weak(wait),
    /* K10  */  be_nested_str_weak(process_wait_statement_fluent),
    /* K11  */  be_const_int(1),
    /* K12  */  be_nested_str_weak(log),
    /* K13  */  be_nested_str_weak(process_log_statement_fluent),
    /* K14  */  be_nested_str_weak(restart),
    /* K15  */  be_nested_str_weak(process_restart_statement_fluent),
    /* K16  */  be_nested_str_weak(repeat),
    /* K17  */  be_nested_str_weak(process_repeat_statement_fluent),
    /* K18  */  be_nested_str_weak(peek),
    /* K19  */  be_nested_str_weak(process_sequence_assignment_fluent),
    /* K20  */  be_nested_str_weak(error),
    /* K21  */  be_nested_str_weak(Unknown_X20command_X20_X27_X25s_X27_X20in_X20sequence_X2E_X20Valid_X20sequence_X20commands_X20are_X3A_X20play_X2C_X20wait_X2C_X20repeat_X2C_X20restart_X2C_X20log_X2C_X20or_X20property_X20assignments_X20_X28object_X2Eproperty_X20_X3D_X20value_X29),
    /* K22  */  be_nested_str_weak(skip_statement),
    /* K23  */  be_nested_str_weak(Invalid_X20statement_X20in_X20sequence_X2E_X20Expected_X3A_X20play_X2C_X20wait_X2C_X20repeat_X2C_X20restart_X2C_X20log_X2C_X20or_X20property_X20assignments),
    }),
    be_str_weak(process_sequence_statement),
    &be_const_str_solidified,
    ( &(const binstruction[105]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x1C080202,  //  0003  EQ	R2	R1	R2
      0x780A0000,  //  0004  JMPF	R2	#0006
      0x80000400,  //  0005  RET	0
      0x88080301,  //  0006  GETMBR	R2	R1	K1
      0x540E0024,  //  0007  LDINT	R3	37
      0x1C080403,  //  0008  EQ	R2	R2	R3
      0x780A0008,  //  0009  JMPF	R2	#0013
      0x8C080102,  //  000A  GETMET	R2	R0	K2
      0x8C100103,  //  000B  GETMET	R4	R0	K3
      0x7C100200,  //  000C  CALL	R4	1
      0x88140304,  //  000D  GETMBR	R5	R1	K4
      0x00100805,  //  000E  ADD	R4	R4	R5
      0x7C080400,  //  000F  CALL	R2	2
      0x8C080105,  //  0010  GETMET	R2	R0	K5
      0x7C080200,  //  0011  CALL	R2	1
      0x80000400,  //  0012  RET	0
      0x88080301,  //  0013  GETMBR	R2	R1	K1
      0x540E0022,  //  0014  LDINT	R3	35
      0x1C080403,  //  0015  EQ	R2	R2	R3
      0x780A0002,  //  0016  JMPF	R2	#001A
      0x8C080105,  //  0017  GETMET	R2	R0	K5
      0x7C080200,  //  0018  CALL	R2	1
      0x80000400,  //  0019  RET	0
      0x88080301,  //  001A  GETMBR	R2	R1	K1
      0x1C080506,  //  001B  EQ	R2	R2	K6
      0x780A0005,  //  001C  JMPF	R2	#0023
      0x88080304,  //  001D  GETMBR	R2	R1	K4
      0x1C080507,  //  001E  EQ	R2	R2	K7
      0x780A0002,  //  001F  JMPF	R2	#0023
      0x8C080108,  //  0020  GETMET	R2	R0	K8
      0x7C080200,  //  0021  CALL	R2	1
      0x70020044,  //  0022  JMP		#0068
      0x88080301,  //  0023  GETMBR	R2	R1	K1
      0x1C080506,  //  0024  EQ	R2	R2	K6
      0x780A0005,  //  0025  JMPF	R2	#002C
      0x88080304,  //  0026  GETMBR	R2	R1	K4
      0x1C080509,  //  0027  EQ	R2	R2	K9
      0x780A0002,  //  0028  JMPF	R2	#002C
      0x8C08010A,  //  0029  GETMET	R2	R0	K10
      0x7C080200,  //  002A  CALL	R2	1
      0x7002003B,  //  002B  JMP		#0068
      0x88080301,  //  002C  GETMBR	R2	R1	K1
      0x1C08050B,  //  002D  EQ	R2	R2	K11
      0x780A0005,  //  002E  JMPF	R2	#0035
      0x88080304,  //  002F  GETMBR	R2	R1	K4
      0x1C08050C,  //  0030  EQ	R2	R2	K12
      0x780A0002,  //  0031  JMPF	R2	#0035
      0x8C08010D,  //  0032  GETMET	R2	R0	K13
      0x7C080200,  //  0033  CALL	R2	1
      0x70020032,  //  0034  JMP		#0068
      0x88080301,  //  0035  GETMBR	R2	R1	K1
      0x1C080506,  //  0036  EQ	R2	R2	K6
      0x780A0005,  //  0037  JMPF	R2	#003E
      0x88080304,  //  0038  GETMBR	R2	R1	K4
      0x1C08050E,  //  0039  EQ	R2	R2	K14
      0x780A0002,  //  003A  JMPF	R2	#003E
      0x8C08010F,  //  003B  GETMET	R2	R0	K15
      0x7C080200,  //  003C  CALL	R2	1
      0x70020029,  //  003D  JMP		#0068
      0x88080301,  //  003E  GETMBR	R2	R1	K1
      0x1C080506,  //  003F  EQ	R2	R2	K6
      0x780A0005,  //  0040  JMPF	R2	#0047
      0x88080304,  //  0041  GETMBR	R2	R1	K4
      0x1C080510,  //  0042  EQ	R2	R2	K16
      0x780A0002,  //  0043  JMPF	R2	#0047
      0x8C080111,  //  0044  GETMET	R2	R0	K17
      0x7C080200,  //  0045  CALL	R2	1
      0x70020020,  //  0046  JMP		#0068
      0x88080301,  //  0047  GETMBR	R2	R1	K1
      0x1C08050B,  //  0048  EQ	R2	R2	K11
      0x780A0016,  //  0049  JMPF	R2	#0061
      0x8C080112,  //  004A  GETMET	R2	R0	K18
      0x7C080200,  //  004B  CALL	R2	1
      0x4C0C0000,  //  004C  LDNIL	R3
      0x20080403,  //  004D  NE	R2	R2	R3
      0x780A0008,  //  004E  JMPF	R2	#0058
      0x8C080112,  //  004F  GETMET	R2	R0	K18
      0x7C080200,  //  0050  CALL	R2	1
      0x88080501,  //  0051  GETMBR	R2	R2	K1
      0x540E0020,  //  0052  LDINT	R3	33
      0x1C080403,  //  0053  EQ	R2	R2	R3
      0x780A0002,  //  0054  JMPF	R2	#0058
      0x8C080113,  //  0055  GETMET	R2	R0	K19
      0x7C080200,  //  0056  CALL	R2	1
      0x70020007,  //  0057  JMP		#0060
      0x8C080114,  //  0058  GETMET	R2	R0	K20
      0x60100018,  //  0059  GETGBL	R4	G24
      0x58140015,  //  005A  LDCONST	R5	K21
      0x88180304,  //  005B  GETMBR	R6	R1	K4
      0x7C100400,  //  005C  CALL	R4	2
      0x7C080400,  //  005D  CALL	R2	2
      0x8C080116,  //  005E  GETMET	R2	R0	K22
      0x7C080200,  //  005F  CALL	R2	1
      0x70020006,  //  0060  JMP		#0068
      0x8C080114,  //  0061  GETMET	R2	R0	K20
      0x60100018,  //  0062  GETGBL	R4	G24
      0x58140017,  //  0063  LDCONST	R5	K23
      0x7C100200,  //  0064  CALL	R4	1
      0x7C080400,  //  0065  CALL	R2	2
      0x8C080116,  //  0066  GETMET	R2	R0	K22
      0x7C080200,  //  0067  CALL	R2	1
      0x80000000,  //  0068  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: current
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_current,   /* name */
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
    /* K0   */  be_nested_str_weak(pull_lexer),
    /* K1   */  be_nested_str_weak(peek_token),
    }),
    be_str_weak(current),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_color
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_color,   /* name */
  be_nested_proto(
    18,                          /* nstack */
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
    /* K8   */  be_const_int(0),
    /* K9   */  be_const_int(1),
    /* K10  */  be_nested_str_weak(peek),
    /* K11  */  be_nested_str_weak(value),
    /* K12  */  be_nested_str_weak(),
    /* K13  */  be_nested_str_weak(_X20_X20),
    /* K14  */  be_nested_str_weak(symbol_table),
    /* K15  */  be_nested_str_weak(get),
    /* K16  */  be_nested_str_weak(process_function_arguments),
    /* K17  */  be_nested_str_weak(_split_function_arguments),
    /* K18  */  be_nested_str_weak(instance),
    /* K19  */  be_nested_str_weak(contains),
    /* K20  */  be_nested_str_weak(params),
    /* K21  */  be_nested_str_weak(find),
    /* K22  */  be_nested_str_weak(param_types),
    /* K23  */  be_nested_str_weak(_validate_template_call_arguments),
    /* K24  */  be_nested_str_weak(engine_X2C_X20_X25s),
    /* K25  */  be_nested_str_weak(engine),
    /* K26  */  be_nested_str_weak(add),
    /* K27  */  be_nested_str_weak(var_X20_X25s__X20_X3D_X20_X25s_template_X28_X25s_X29_X25s),
    /* K28  */  be_nested_str_weak(create_color),
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
    ( &(const binstruction[204]) {  /* code */
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
      0x1C0C0708,  //  0011  EQ	R3	R3	K8
      0x740E0002,  //  0012  JMPT	R3	#0016
      0x880C0507,  //  0013  GETMBR	R3	R2	K7
      0x1C0C0709,  //  0014  EQ	R3	R3	K9
      0x780E00AE,  //  0015  JMPF	R3	#00C5
      0x8C0C010A,  //  0016  GETMET	R3	R0	K10
      0x7C0C0200,  //  0017  CALL	R3	1
      0x4C100000,  //  0018  LDNIL	R4
      0x200C0604,  //  0019  NE	R3	R3	R4
      0x780E00A9,  //  001A  JMPF	R3	#00C5
      0x8C0C010A,  //  001B  GETMET	R3	R0	K10
      0x7C0C0200,  //  001C  CALL	R3	1
      0x880C0707,  //  001D  GETMBR	R3	R3	K7
      0x54120017,  //  001E  LDINT	R4	24
      0x1C0C0604,  //  001F  EQ	R3	R3	R4
      0x780E00A3,  //  0020  JMPF	R3	#00C5
      0x880C050B,  //  0021  GETMBR	R3	R2	K11
      0x8C100100,  //  0022  GETMET	R4	R0	K0
      0x7C100200,  //  0023  CALL	R4	1
      0x5810000C,  //  0024  LDCONST	R4	K12
      0x8C140106,  //  0025  GETMET	R5	R0	K6
      0x7C140200,  //  0026  CALL	R5	1
      0x4C180000,  //  0027  LDNIL	R6
      0x20140A06,  //  0028  NE	R5	R5	R6
      0x7816000C,  //  0029  JMPF	R5	#0037
      0x8C140106,  //  002A  GETMET	R5	R0	K6
      0x7C140200,  //  002B  CALL	R5	1
      0x88140B07,  //  002C  GETMBR	R5	R5	K7
      0x541A0024,  //  002D  LDINT	R6	37
      0x1C140A06,  //  002E  EQ	R5	R5	R6
      0x78160006,  //  002F  JMPF	R5	#0037
      0x8C140106,  //  0030  GETMET	R5	R0	K6
      0x7C140200,  //  0031  CALL	R5	1
      0x88140B0B,  //  0032  GETMBR	R5	R5	K11
      0x00161A05,  //  0033  ADD	R5	K13	R5
      0x5C100A00,  //  0034  MOVE	R4	R5
      0x8C140100,  //  0035  GETMET	R5	R0	K0
      0x7C140200,  //  0036  CALL	R5	1
      0x8814010E,  //  0037  GETMBR	R5	R0	K14
      0x8C140B0F,  //  0038  GETMET	R5	R5	K15
      0x5C1C0600,  //  0039  MOVE	R7	R3
      0x7C140400,  //  003A  CALL	R5	2
      0x4C180000,  //  003B  LDNIL	R6
      0x20180A06,  //  003C  NE	R6	R5	R6
      0x781A003D,  //  003D  JMPF	R6	#007C
      0x88180B07,  //  003E  GETMBR	R6	R5	K7
      0x541E000D,  //  003F  LDINT	R7	14
      0x1C180C07,  //  0040  EQ	R6	R6	R7
      0x781A0039,  //  0041  JMPF	R6	#007C
      0x8C180110,  //  0042  GETMET	R6	R0	K16
      0x50200000,  //  0043  LDBOOL	R8	0	0
      0x7C180400,  //  0044  CALL	R6	2
      0x201C0D0C,  //  0045  NE	R7	R6	K12
      0x781E0003,  //  0046  JMPF	R7	#004B
      0x8C1C0111,  //  0047  GETMET	R7	R0	K17
      0x5C240C00,  //  0048  MOVE	R9	R6
      0x7C1C0400,  //  0049  CALL	R7	2
      0x70020001,  //  004A  JMP		#004D
      0x601C0012,  //  004B  GETGBL	R7	G18
      0x7C1C0000,  //  004C  CALL	R7	0
      0x88200B12,  //  004D  GETMBR	R8	R5	K18
      0x4C240000,  //  004E  LDNIL	R9
      0x20241009,  //  004F  NE	R9	R8	R9
      0x78260013,  //  0050  JMPF	R9	#0065
      0x8C241113,  //  0051  GETMET	R9	R8	K19
      0x582C0014,  //  0052  LDCONST	R11	K20
      0x7C240400,  //  0053  CALL	R9	2
      0x7826000F,  //  0054  JMPF	R9	#0065
      0x94241114,  //  0055  GETIDX	R9	R8	K20
      0x8C281115,  //  0056  GETMET	R10	R8	K21
      0x58300016,  //  0057  LDCONST	R12	K22
      0x60340013,  //  0058  GETGBL	R13	G19
      0x7C340000,  //  0059  CALL	R13	0
      0x7C280600,  //  005A  CALL	R10	3
      0x8C2C0117,  //  005B  GETMET	R11	R0	K23
      0x5C340600,  //  005C  MOVE	R13	R3
      0x5C380E00,  //  005D  MOVE	R14	R7
      0x5C3C1200,  //  005E  MOVE	R15	R9
      0x5C401400,  //  005F  MOVE	R16	R10
      0x7C2C0A00,  //  0060  CALL	R11	5
      0x742E0002,  //  0061  JMPT	R11	#0065
      0x8C2C0104,  //  0062  GETMET	R11	R0	K4
      0x7C2C0200,  //  0063  CALL	R11	1
      0x80001600,  //  0064  RET	0
      0x20240D0C,  //  0065  NE	R9	R6	K12
      0x78260004,  //  0066  JMPF	R9	#006C
      0x60240018,  //  0067  GETGBL	R9	G24
      0x58280018,  //  0068  LDCONST	R10	K24
      0x5C2C0C00,  //  0069  MOVE	R11	R6
      0x7C240400,  //  006A  CALL	R9	2
      0x70020000,  //  006B  JMP		#006D
      0x58240019,  //  006C  LDCONST	R9	K25
      0x8C28011A,  //  006D  GETMET	R10	R0	K26
      0x60300018,  //  006E  GETGBL	R12	G24
      0x5834001B,  //  006F  LDCONST	R13	K27
      0x5C380200,  //  0070  MOVE	R14	R1
      0x5C3C0600,  //  0071  MOVE	R15	R3
      0x5C401200,  //  0072  MOVE	R16	R9
      0x5C440800,  //  0073  MOVE	R17	R4
      0x7C300A00,  //  0074  CALL	R12	5
      0x7C280400,  //  0075  CALL	R10	2
      0x8828010E,  //  0076  GETMBR	R10	R0	K14
      0x8C28151C,  //  0077  GETMET	R10	R10	K28
      0x5C300200,  //  0078  MOVE	R12	R1
      0x4C340000,  //  0079  LDNIL	R13
      0x7C280600,  //  007A  CALL	R10	3
      0x70020047,  //  007B  JMP		#00C4
      0x4C180000,  //  007C  LDNIL	R6
      0x20180A06,  //  007D  NE	R6	R5	R6
      0x781A001D,  //  007E  JMPF	R6	#009D
      0x88180B07,  //  007F  GETMBR	R6	R5	K7
      0x541E0004,  //  0080  LDINT	R7	5
      0x1C180C07,  //  0081  EQ	R6	R6	R7
      0x781A0019,  //  0082  JMPF	R6	#009D
      0x8C180110,  //  0083  GETMET	R6	R0	K16
      0x50200000,  //  0084  LDBOOL	R8	0	0
      0x7C180400,  //  0085  CALL	R6	2
      0x201C0D0C,  //  0086  NE	R7	R6	K12
      0x781E0004,  //  0087  JMPF	R7	#008D
      0x601C0018,  //  0088  GETGBL	R7	G24
      0x58200018,  //  0089  LDCONST	R8	K24
      0x5C240C00,  //  008A  MOVE	R9	R6
      0x7C1C0400,  //  008B  CALL	R7	2
      0x70020000,  //  008C  JMP		#008E
      0x581C0019,  //  008D  LDCONST	R7	K25
      0x8C20011A,  //  008E  GETMET	R8	R0	K26
      0x60280018,  //  008F  GETGBL	R10	G24
      0x582C001D,  //  0090  LDCONST	R11	K29
      0x5C300200,  //  0091  MOVE	R12	R1
      0x5C340600,  //  0092  MOVE	R13	R3
      0x5C380E00,  //  0093  MOVE	R14	R7
      0x5C3C0800,  //  0094  MOVE	R15	R4
      0x7C280A00,  //  0095  CALL	R10	5
      0x7C200400,  //  0096  CALL	R8	2
      0x8820010E,  //  0097  GETMBR	R8	R0	K14
      0x8C20111C,  //  0098  GETMET	R8	R8	K28
      0x5C280200,  //  0099  MOVE	R10	R1
      0x4C2C0000,  //  009A  LDNIL	R11
      0x7C200600,  //  009B  CALL	R8	3
      0x70020026,  //  009C  JMP		#00C4
      0x8C18011E,  //  009D  GETMET	R6	R0	K30
      0x5C200600,  //  009E  MOVE	R8	R3
      0x7C180400,  //  009F  CALL	R6	2
      0x741A0008,  //  00A0  JMPT	R6	#00AA
      0x8C18011F,  //  00A1  GETMET	R6	R0	K31
      0x60200018,  //  00A2  GETGBL	R8	G24
      0x58240020,  //  00A3  LDCONST	R9	K32
      0x5C280600,  //  00A4  MOVE	R10	R3
      0x7C200400,  //  00A5  CALL	R8	2
      0x7C180400,  //  00A6  CALL	R6	2
      0x8C180104,  //  00A7  GETMET	R6	R0	K4
      0x7C180200,  //  00A8  CALL	R6	1
      0x80000C00,  //  00A9  RET	0
      0x8C18011A,  //  00AA  GETMET	R6	R0	K26
      0x60200018,  //  00AB  GETGBL	R8	G24
      0x58240021,  //  00AC  LDCONST	R9	K33
      0x5C280200,  //  00AD  MOVE	R10	R1
      0x5C2C0600,  //  00AE  MOVE	R11	R3
      0x5C300800,  //  00AF  MOVE	R12	R4
      0x7C200800,  //  00B0  CALL	R8	4
      0x7C180400,  //  00B1  CALL	R6	2
      0x8C180122,  //  00B2  GETMET	R6	R0	K34
      0x5C200600,  //  00B3  MOVE	R8	R3
      0x7C180400,  //  00B4  CALL	R6	2
      0x4C1C0000,  //  00B5  LDNIL	R7
      0x201C0C07,  //  00B6  NE	R7	R6	R7
      0x781E0004,  //  00B7  JMPF	R7	#00BD
      0x881C010E,  //  00B8  GETMBR	R7	R0	K14
      0x8C1C0F1C,  //  00B9  GETMET	R7	R7	K28
      0x5C240200,  //  00BA  MOVE	R9	R1
      0x5C280C00,  //  00BB  MOVE	R10	R6
      0x7C1C0600,  //  00BC  CALL	R7	3
      0x8C1C0123,  //  00BD  GETMET	R7	R0	K35
      0x60240018,  //  00BE  GETGBL	R9	G24
      0x58280024,  //  00BF  LDCONST	R10	K36
      0x5C2C0200,  //  00C0  MOVE	R11	R1
      0x7C240400,  //  00C1  CALL	R9	2
      0x5C280600,  //  00C2  MOVE	R10	R3
      0x7C1C0600,  //  00C3  CALL	R7	3
      0x70020004,  //  00C4  JMP		#00CA
      0x8C0C0125,  //  00C5  GETMET	R3	R0	K37
      0x5C140200,  //  00C6  MOVE	R5	R1
      0x88180126,  //  00C7  GETMBR	R6	R0	K38
      0x841C0000,  //  00C8  CLOSURE	R7	P0
      0x7C0C0800,  //  00C9  CALL	R3	4
      0xA0000000,  //  00CA  CLOSE	R0
      0x80000000,  //  00CB  RET	0
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
    ( &(const bvalue[20]) {     /* constants */
    /* K0   */  be_nested_str_weak(next),
    /* K1   */  be_nested_str_weak(),
    /* K2   */  be_nested_str_weak(current),
    /* K3   */  be_nested_str_weak(type),
    /* K4   */  be_const_int(1),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(peek),
    /* K7   */  be_nested_str_weak(process_nested_function_call),
    /* K8   */  be_nested_str_weak(expect_identifier),
    /* K9   */  be_nested_str_weak(_validate_object_reference),
    /* K10  */  be_nested_str_weak(sequence_X20play),
    /* K11  */  be_nested_str_weak(_X25s_),
    /* K12  */  be_nested_str_weak(nil),
    /* K13  */  be_nested_str_weak(value),
    /* K14  */  be_nested_str_weak(for),
    /* K15  */  be_nested_str_weak(process_time_value),
    /* K16  */  be_nested_str_weak(collect_inline_comment),
    /* K17  */  be_nested_str_weak(add),
    /* K18  */  be_nested_str_weak(_X25s_X2Epush_play_step_X28_X25s_X2C_X20_X25s_X29_X25s),
    /* K19  */  be_nested_str_weak(get_indent),
    }),
    be_str_weak(process_play_statement_fluent),
    &be_const_str_solidified,
    ( &(const binstruction[74]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x58040001,  //  0002  LDCONST	R1	K1
      0x8C080102,  //  0003  GETMET	R2	R0	K2
      0x7C080200,  //  0004  CALL	R2	1
      0x4C0C0000,  //  0005  LDNIL	R3
      0x200C0403,  //  0006  NE	R3	R2	R3
      0x780E0014,  //  0007  JMPF	R3	#001D
      0x880C0503,  //  0008  GETMBR	R3	R2	K3
      0x1C0C0704,  //  0009  EQ	R3	R3	K4
      0x740E0002,  //  000A  JMPT	R3	#000E
      0x880C0503,  //  000B  GETMBR	R3	R2	K3
      0x1C0C0705,  //  000C  EQ	R3	R3	K5
      0x780E000E,  //  000D  JMPF	R3	#001D
      0x8C0C0106,  //  000E  GETMET	R3	R0	K6
      0x7C0C0200,  //  000F  CALL	R3	1
      0x4C100000,  //  0010  LDNIL	R4
      0x200C0604,  //  0011  NE	R3	R3	R4
      0x780E0009,  //  0012  JMPF	R3	#001D
      0x8C0C0106,  //  0013  GETMET	R3	R0	K6
      0x7C0C0200,  //  0014  CALL	R3	1
      0x880C0703,  //  0015  GETMBR	R3	R3	K3
      0x54120017,  //  0016  LDINT	R4	24
      0x1C0C0604,  //  0017  EQ	R3	R3	R4
      0x780E0003,  //  0018  JMPF	R3	#001D
      0x8C0C0107,  //  0019  GETMET	R3	R0	K7
      0x7C0C0200,  //  001A  CALL	R3	1
      0x5C040600,  //  001B  MOVE	R1	R3
      0x7002000A,  //  001C  JMP		#0028
      0x8C0C0108,  //  001D  GETMET	R3	R0	K8
      0x7C0C0200,  //  001E  CALL	R3	1
      0x8C100109,  //  001F  GETMET	R4	R0	K9
      0x5C180600,  //  0020  MOVE	R6	R3
      0x581C000A,  //  0021  LDCONST	R7	K10
      0x7C100600,  //  0022  CALL	R4	3
      0x60100018,  //  0023  GETGBL	R4	G24
      0x5814000B,  //  0024  LDCONST	R5	K11
      0x5C180600,  //  0025  MOVE	R6	R3
      0x7C100400,  //  0026  CALL	R4	2
      0x5C040800,  //  0027  MOVE	R1	R4
      0x580C000C,  //  0028  LDCONST	R3	K12
      0x8C100102,  //  0029  GETMET	R4	R0	K2
      0x7C100200,  //  002A  CALL	R4	1
      0x4C140000,  //  002B  LDNIL	R5
      0x20100805,  //  002C  NE	R4	R4	R5
      0x7812000E,  //  002D  JMPF	R4	#003D
      0x8C100102,  //  002E  GETMET	R4	R0	K2
      0x7C100200,  //  002F  CALL	R4	1
      0x88100903,  //  0030  GETMBR	R4	R4	K3
      0x1C100905,  //  0031  EQ	R4	R4	K5
      0x78120009,  //  0032  JMPF	R4	#003D
      0x8C100102,  //  0033  GETMET	R4	R0	K2
      0x7C100200,  //  0034  CALL	R4	1
      0x8810090D,  //  0035  GETMBR	R4	R4	K13
      0x1C10090E,  //  0036  EQ	R4	R4	K14
      0x78120004,  //  0037  JMPF	R4	#003D
      0x8C100100,  //  0038  GETMET	R4	R0	K0
      0x7C100200,  //  0039  CALL	R4	1
      0x8C10010F,  //  003A  GETMET	R4	R0	K15
      0x7C100200,  //  003B  CALL	R4	1
      0x5C0C0800,  //  003C  MOVE	R3	R4
      0x8C100110,  //  003D  GETMET	R4	R0	K16
      0x7C100200,  //  003E  CALL	R4	1
      0x8C140111,  //  003F  GETMET	R5	R0	K17
      0x601C0018,  //  0040  GETGBL	R7	G24
      0x58200012,  //  0041  LDCONST	R8	K18
      0x8C240113,  //  0042  GETMET	R9	R0	K19
      0x7C240200,  //  0043  CALL	R9	1
      0x5C280200,  //  0044  MOVE	R10	R1
      0x5C2C0600,  //  0045  MOVE	R11	R3
      0x5C300800,  //  0046  MOVE	R12	R4
      0x7C1C0A00,  //  0047  CALL	R7	5
      0x7C140400,  //  0048  CALL	R5	2
      0x80000000,  //  0049  RET	0
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
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(next),
    /* K3   */  be_nested_str_weak(error),
    /* K4   */  be_nested_str_weak(Expected_X20_X27_X2C_X27),
    }),
    be_str_weak(expect_comma),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0006,  //  0004  JMPF	R2	#000C
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0x540E001D,  //  0006  LDINT	R3	30
      0x1C080403,  //  0007  EQ	R2	R2	R3
      0x780A0002,  //  0008  JMPF	R2	#000C
      0x8C080102,  //  0009  GETMET	R2	R0	K2
      0x7C080200,  //  000A  CALL	R2	1
      0x70020002,  //  000B  JMP		#000F
      0x8C080103,  //  000C  GETMET	R2	R0	K3
      0x58100004,  //  000D  LDCONST	R4	K4
      0x7C080400,  //  000E  CALL	R2	2
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_repeat_statement_fluent
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_repeat_statement_fluent,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[24]) {     /* constants */
    /* K0   */  be_nested_str_weak(next),
    /* K1   */  be_nested_str_weak(1),
    /* K2   */  be_nested_str_weak(current),
    /* K3   */  be_nested_str_weak(type),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(value),
    /* K6   */  be_nested_str_weak(forever),
    /* K7   */  be_nested_str_weak(_X2D1),
    /* K8   */  be_nested_str_weak(process_value),
    /* K9   */  be_nested_str_weak(CONTEXT_REPEAT_COUNT),
    /* K10  */  be_nested_str_weak(expect_keyword),
    /* K11  */  be_nested_str_weak(times),
    /* K12  */  be_nested_str_weak(expr),
    /* K13  */  be_nested_str_weak(expect_left_brace),
    /* K14  */  be_nested_str_weak(add),
    /* K15  */  be_nested_str_weak(_X25s_X2Epush_repeat_subsequence_X28animation_X2ESequenceManager_X28engine_X2C_X20_X25s_X29),
    /* K16  */  be_nested_str_weak(get_indent),
    /* K17  */  be_nested_str_weak(indent_level),
    /* K18  */  be_const_int(1),
    /* K19  */  be_nested_str_weak(at_end),
    /* K20  */  be_nested_str_weak(check_right_brace),
    /* K21  */  be_nested_str_weak(process_sequence_statement),
    /* K22  */  be_nested_str_weak(expect_right_brace),
    /* K23  */  be_nested_str_weak(_X25s_X29),
    }),
    be_str_weak(process_repeat_statement_fluent),
    &be_const_str_solidified,
    ( &(const binstruction[60]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x58040001,  //  0002  LDCONST	R1	K1
      0x8C080102,  //  0003  GETMET	R2	R0	K2
      0x7C080200,  //  0004  CALL	R2	1
      0x4C0C0000,  //  0005  LDNIL	R3
      0x200C0403,  //  0006  NE	R3	R2	R3
      0x780E0009,  //  0007  JMPF	R3	#0012
      0x880C0503,  //  0008  GETMBR	R3	R2	K3
      0x1C0C0704,  //  0009  EQ	R3	R3	K4
      0x780E0006,  //  000A  JMPF	R3	#0012
      0x880C0505,  //  000B  GETMBR	R3	R2	K5
      0x1C0C0706,  //  000C  EQ	R3	R3	K6
      0x780E0003,  //  000D  JMPF	R3	#0012
      0x8C0C0100,  //  000E  GETMET	R3	R0	K0
      0x7C0C0200,  //  000F  CALL	R3	1
      0x58040007,  //  0010  LDCONST	R1	K7
      0x70020006,  //  0011  JMP		#0019
      0x8C0C0108,  //  0012  GETMET	R3	R0	K8
      0x88140109,  //  0013  GETMBR	R5	R0	K9
      0x7C0C0400,  //  0014  CALL	R3	2
      0x8C10010A,  //  0015  GETMET	R4	R0	K10
      0x5818000B,  //  0016  LDCONST	R6	K11
      0x7C100400,  //  0017  CALL	R4	2
      0x8804070C,  //  0018  GETMBR	R1	R3	K12
      0x8C0C010D,  //  0019  GETMET	R3	R0	K13
      0x7C0C0200,  //  001A  CALL	R3	1
      0x8C0C010E,  //  001B  GETMET	R3	R0	K14
      0x60140018,  //  001C  GETGBL	R5	G24
      0x5818000F,  //  001D  LDCONST	R6	K15
      0x8C1C0110,  //  001E  GETMET	R7	R0	K16
      0x7C1C0200,  //  001F  CALL	R7	1
      0x5C200200,  //  0020  MOVE	R8	R1
      0x7C140600,  //  0021  CALL	R5	3
      0x7C0C0400,  //  0022  CALL	R3	2
      0x880C0111,  //  0023  GETMBR	R3	R0	K17
      0x000C0712,  //  0024  ADD	R3	R3	K18
      0x90022203,  //  0025  SETMBR	R0	K17	R3
      0x8C0C0113,  //  0026  GETMET	R3	R0	K19
      0x7C0C0200,  //  0027  CALL	R3	1
      0x740E0005,  //  0028  JMPT	R3	#002F
      0x8C0C0114,  //  0029  GETMET	R3	R0	K20
      0x7C0C0200,  //  002A  CALL	R3	1
      0x740E0002,  //  002B  JMPT	R3	#002F
      0x8C0C0115,  //  002C  GETMET	R3	R0	K21
      0x7C0C0200,  //  002D  CALL	R3	1
      0x7001FFF6,  //  002E  JMP		#0026
      0x8C0C0116,  //  002F  GETMET	R3	R0	K22
      0x7C0C0200,  //  0030  CALL	R3	1
      0x8C0C010E,  //  0031  GETMET	R3	R0	K14
      0x60140018,  //  0032  GETGBL	R5	G24
      0x58180017,  //  0033  LDCONST	R6	K23
      0x8C1C0110,  //  0034  GETMET	R7	R0	K16
      0x7C1C0200,  //  0035  CALL	R7	1
      0x7C140400,  //  0036  CALL	R5	2
      0x7C0C0400,  //  0037  CALL	R3	2
      0x880C0111,  //  0038  GETMBR	R3	R0	K17
      0x040C0712,  //  0039  SUB	R3	R3	K18
      0x90022203,  //  003A  SETMBR	R0	K17	R3
      0x80000000,  //  003B  RET	0
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
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(process_multiplicative_expression),
    /* K1   */  be_nested_str_weak(at_end),
    /* K2   */  be_nested_str_weak(current),
    /* K3   */  be_nested_str_weak(type),
    /* K4   */  be_nested_str_weak(value),
    /* K5   */  be_nested_str_weak(next),
    /* K6   */  be_nested_str_weak(has_dangerous),
    /* K7   */  be_nested_str_weak(expr),
    /* K8   */  be_nested_str_weak(error),
    /* K9   */  be_nested_str_weak(Expression_X20_X27_X25s_X27_X20cannot_X20be_X20used_X20in_X20computed_X20expressions_X2E_X20This_X20creates_X20a_X20new_X20instance_X20at_X20each_X20evaluation_X2E_X20Use_X20either_X3A_X0A_X20_X20set_X20var_name_X20_X3D_X20_X25s_X28_X29_X20_X20_X23_X20Single_X20function_X20call_X0A_X20_X20set_X20computed_X20_X3D_X20_X28existing_var_X20_X2B_X201_X29_X20_X2F_X202_X20_X20_X23_X20Computation_X20with_X20existing_X20values),
    /* K10  */  be_nested_str_weak(skip_statement),
    /* K11  */  be_nested_str_weak(ExpressionResult),
    /* K12  */  be_nested_str_weak(literal),
    /* K13  */  be_nested_str_weak(nil),
    /* K14  */  be_nested_str_weak(combine),
    /* K15  */  be_nested_str_weak(_X25s_X20_X25s_X20_X25s),
    }),
    be_str_weak(process_additive_expression),
    &be_const_str_solidified,
    ( &(const binstruction[68]) {  /* code */
      0x8C100100,  //  0000  GETMET	R4	R0	K0
      0x5C180200,  //  0001  MOVE	R6	R1
      0x5C1C0400,  //  0002  MOVE	R7	R2
      0x5C200600,  //  0003  MOVE	R8	R3
      0x7C100800,  //  0004  CALL	R4	4
      0x8C140101,  //  0005  GETMET	R5	R0	K1
      0x7C140200,  //  0006  CALL	R5	1
      0x7416003A,  //  0007  JMPT	R5	#0043
      0x8C140102,  //  0008  GETMET	R5	R0	K2
      0x7C140200,  //  0009  CALL	R5	1
      0x4C180000,  //  000A  LDNIL	R6
      0x20180A06,  //  000B  NE	R6	R5	R6
      0x781A0033,  //  000C  JMPF	R6	#0041
      0x88180B03,  //  000D  GETMBR	R6	R5	K3
      0x541E0008,  //  000E  LDINT	R7	9
      0x1C180C07,  //  000F  EQ	R6	R6	R7
      0x741A0003,  //  0010  JMPT	R6	#0015
      0x88180B03,  //  0011  GETMBR	R6	R5	K3
      0x541E0009,  //  0012  LDINT	R7	10
      0x1C180C07,  //  0013  EQ	R6	R6	R7
      0x781A002B,  //  0014  JMPF	R6	#0041
      0x88180B04,  //  0015  GETMBR	R6	R5	K4
      0x8C1C0105,  //  0016  GETMET	R7	R0	K5
      0x7C1C0200,  //  0017  CALL	R7	1
      0x8C1C0100,  //  0018  GETMET	R7	R0	K0
      0x5C240200,  //  0019  MOVE	R9	R1
      0x50280000,  //  001A  LDBOOL	R10	0	0
      0x5C2C0600,  //  001B  MOVE	R11	R3
      0x7C1C0800,  //  001C  CALL	R7	4
      0x88200906,  //  001D  GETMBR	R8	R4	K6
      0x74220001,  //  001E  JMPT	R8	#0021
      0x88200F06,  //  001F  GETMBR	R8	R7	K6
      0x78220012,  //  0020  JMPF	R8	#0034
      0x88200906,  //  0021  GETMBR	R8	R4	K6
      0x78220001,  //  0022  JMPF	R8	#0025
      0x88200907,  //  0023  GETMBR	R8	R4	K7
      0x70020000,  //  0024  JMP		#0026
      0x88200F07,  //  0025  GETMBR	R8	R7	K7
      0x8C240108,  //  0026  GETMET	R9	R0	K8
      0x602C0018,  //  0027  GETGBL	R11	G24
      0x58300009,  //  0028  LDCONST	R12	K9
      0x5C341000,  //  0029  MOVE	R13	R8
      0x5C381000,  //  002A  MOVE	R14	R8
      0x7C2C0600,  //  002B  CALL	R11	3
      0x7C240400,  //  002C  CALL	R9	2
      0x8C24010A,  //  002D  GETMET	R9	R0	K10
      0x7C240200,  //  002E  CALL	R9	1
      0x8824010B,  //  002F  GETMBR	R9	R0	K11
      0x8C24130C,  //  0030  GETMET	R9	R9	K12
      0x582C000D,  //  0031  LDCONST	R11	K13
      0x7C240400,  //  0032  CALL	R9	2
      0x80041200,  //  0033  RET	1	R9
      0x8820010B,  //  0034  GETMBR	R8	R0	K11
      0x8C20110E,  //  0035  GETMET	R8	R8	K14
      0x60280018,  //  0036  GETGBL	R10	G24
      0x582C000F,  //  0037  LDCONST	R11	K15
      0x88300907,  //  0038  GETMBR	R12	R4	K7
      0x5C340C00,  //  0039  MOVE	R13	R6
      0x88380F07,  //  003A  GETMBR	R14	R7	K7
      0x7C280800,  //  003B  CALL	R10	4
      0x5C2C0800,  //  003C  MOVE	R11	R4
      0x5C300E00,  //  003D  MOVE	R12	R7
      0x7C200800,  //  003E  CALL	R8	4
      0x5C101000,  //  003F  MOVE	R4	R8
      0x70020000,  //  0040  JMP		#0042
      0x70020000,  //  0041  JMP		#0043
      0x7001FFC1,  //  0042  JMP		#0005
      0x80040800,  //  0043  RET	1	R4
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
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(value),
    /* K4   */  be_nested_str_weak(next),
    /* K5   */  be_nested_str_weak(error),
    /* K6   */  be_nested_str_weak(Expected_X20_X27_X25s_X27),
    }),
    be_str_weak(expect_keyword),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x7C080200,  //  0001  CALL	R2	1
      0x4C0C0000,  //  0002  LDNIL	R3
      0x200C0403,  //  0003  NE	R3	R2	R3
      0x780E0008,  //  0004  JMPF	R3	#000E
      0x880C0501,  //  0005  GETMBR	R3	R2	K1
      0x1C0C0702,  //  0006  EQ	R3	R3	K2
      0x780E0005,  //  0007  JMPF	R3	#000E
      0x880C0503,  //  0008  GETMBR	R3	R2	K3
      0x1C0C0601,  //  0009  EQ	R3	R3	R1
      0x780E0002,  //  000A  JMPF	R3	#000E
      0x8C0C0104,  //  000B  GETMET	R3	R0	K4
      0x7C0C0200,  //  000C  CALL	R3	1
      0x70020005,  //  000D  JMP		#0014
      0x8C0C0105,  //  000E  GETMET	R3	R0	K5
      0x60140018,  //  000F  GETGBL	R5	G24
      0x58180006,  //  0010  LDCONST	R6	K6
      0x5C1C0200,  //  0011  MOVE	R7	R1
      0x7C140400,  //  0012  CALL	R5	2
      0x7C0C0400,  //  0013  CALL	R3	2
      0x80000000,  //  0014  RET	0
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
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(_X20_X20),
    /* K3   */  be_nested_str_weak(value),
    /* K4   */  be_nested_str_weak(next),
    /* K5   */  be_nested_str_weak(),
    }),
    be_str_weak(collect_inline_comment),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0008,  //  0004  JMPF	R2	#000E
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0x540E0024,  //  0006  LDINT	R3	37
      0x1C080403,  //  0007  EQ	R2	R2	R3
      0x780A0004,  //  0008  JMPF	R2	#000E
      0x88080303,  //  0009  GETMBR	R2	R1	K3
      0x000A0402,  //  000A  ADD	R2	K2	R2
      0x8C0C0104,  //  000B  GETMET	R3	R0	K4
      0x7C0C0200,  //  000C  CALL	R3	1
      0x80040400,  //  000D  RET	1	R2
      0x80060A00,  //  000E  RET	1	K5
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
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(next),
    /* K1   */  be_nested_str_weak(expect_left_paren),
    /* K2   */  be_nested_str_weak(current),
    /* K3   */  be_nested_str_weak(type),
    /* K4   */  be_const_int(3),
    /* K5   */  be_nested_str_weak(error),
    /* K6   */  be_nested_str_weak(log_X28_X29_X20function_X20requires_X20a_X20string_X20message),
    /* K7   */  be_nested_str_weak(skip_statement),
    /* K8   */  be_nested_str_weak(value),
    /* K9   */  be_nested_str_weak(expect_right_paren),
    /* K10  */  be_nested_str_weak(collect_inline_comment),
    /* K11  */  be_nested_str_weak(process_log_call),
    /* K12  */  be_nested_str_weak(fluent),
    /* K13  */  be_nested_str_weak(add),
    }),
    be_str_weak(process_log_statement_fluent),
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x7C040200,  //  0003  CALL	R1	1
      0x8C040102,  //  0004  GETMET	R1	R0	K2
      0x7C040200,  //  0005  CALL	R1	1
      0x4C080000,  //  0006  LDNIL	R2
      0x1C080202,  //  0007  EQ	R2	R1	R2
      0x740A0002,  //  0008  JMPT	R2	#000C
      0x88080303,  //  0009  GETMBR	R2	R1	K3
      0x20080504,  //  000A  NE	R2	R2	K4
      0x780A0005,  //  000B  JMPF	R2	#0012
      0x8C080105,  //  000C  GETMET	R2	R0	K5
      0x58100006,  //  000D  LDCONST	R4	K6
      0x7C080400,  //  000E  CALL	R2	2
      0x8C080107,  //  000F  GETMET	R2	R0	K7
      0x7C080200,  //  0010  CALL	R2	1
      0x80000400,  //  0011  RET	0
      0x88080308,  //  0012  GETMBR	R2	R1	K8
      0x8C0C0100,  //  0013  GETMET	R3	R0	K0
      0x7C0C0200,  //  0014  CALL	R3	1
      0x8C0C0109,  //  0015  GETMET	R3	R0	K9
      0x7C0C0200,  //  0016  CALL	R3	1
      0x8C0C010A,  //  0017  GETMET	R3	R0	K10
      0x7C0C0200,  //  0018  CALL	R3	1
      0x8C10010B,  //  0019  GETMET	R4	R0	K11
      0x5C180400,  //  001A  MOVE	R6	R2
      0x581C000C,  //  001B  LDCONST	R7	K12
      0x5C200600,  //  001C  MOVE	R8	R3
      0x7C100800,  //  001D  CALL	R4	4
      0x8C14010D,  //  001E  GETMET	R5	R0	K13
      0x5C1C0800,  //  001F  MOVE	R7	R4
      0x7C140400,  //  0020  CALL	R5	2
      0x80000000,  //  0021  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _validate_object_reference
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__validate_object_reference,   /* name */
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
    be_str_weak(_validate_object_reference),
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
    ( &(const bvalue[19]) {     /* constants */
    /* K0   */  be_nested_str_weak(expect_left_bracket),
    /* K1   */  be_nested_str_weak(at_end),
    /* K2   */  be_nested_str_weak(check_right_bracket),
    /* K3   */  be_nested_str_weak(process_value),
    /* K4   */  be_nested_str_weak(CONTEXT_ARRAY_ELEMENT),
    /* K5   */  be_nested_str_weak(push),
    /* K6   */  be_nested_str_weak(expr),
    /* K7   */  be_nested_str_weak(current),
    /* K8   */  be_nested_str_weak(type),
    /* K9   */  be_nested_str_weak(next),
    /* K10  */  be_nested_str_weak(error),
    /* K11  */  be_nested_str_weak(Expected_X20_X27_X2C_X27_X20or_X20_X27_X5D_X27_X20in_X20array_X20literal),
    /* K12  */  be_nested_str_weak(expect_right_bracket),
    /* K13  */  be_nested_str_weak(_X5B),
    /* K14  */  be_const_int(0),
    /* K15  */  be_const_int(1),
    /* K16  */  be_nested_str_weak(_X2C_X20),
    /* K17  */  be_nested_str_weak(stop_iteration),
    /* K18  */  be_nested_str_weak(_X5D),
    }),
    be_str_weak(process_array_literal),
    &be_const_str_solidified,
    ( &(const binstruction[62]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x60040012,  //  0002  GETGBL	R1	G18
      0x7C040000,  //  0003  CALL	R1	0
      0x8C080101,  //  0004  GETMET	R2	R0	K1
      0x7C080200,  //  0005  CALL	R2	1
      0x740A001E,  //  0006  JMPT	R2	#0026
      0x8C080102,  //  0007  GETMET	R2	R0	K2
      0x7C080200,  //  0008  CALL	R2	1
      0x740A001B,  //  0009  JMPT	R2	#0026
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
      0x780E0008,  //  0014  JMPF	R3	#001E
      0x8C0C0107,  //  0015  GETMET	R3	R0	K7
      0x7C0C0200,  //  0016  CALL	R3	1
      0x880C0708,  //  0017  GETMBR	R3	R3	K8
      0x5412001D,  //  0018  LDINT	R4	30
      0x1C0C0604,  //  0019  EQ	R3	R3	R4
      0x780E0002,  //  001A  JMPF	R3	#001E
      0x8C0C0109,  //  001B  GETMET	R3	R0	K9
      0x7C0C0200,  //  001C  CALL	R3	1
      0x70020006,  //  001D  JMP		#0025
      0x8C0C0102,  //  001E  GETMET	R3	R0	K2
      0x7C0C0200,  //  001F  CALL	R3	1
      0x740E0003,  //  0020  JMPT	R3	#0025
      0x8C0C010A,  //  0021  GETMET	R3	R0	K10
      0x5814000B,  //  0022  LDCONST	R5	K11
      0x7C0C0400,  //  0023  CALL	R3	2
      0x70020000,  //  0024  JMP		#0026
      0x7001FFDD,  //  0025  JMP		#0004
      0x8C08010C,  //  0026  GETMET	R2	R0	K12
      0x7C080200,  //  0027  CALL	R2	1
      0x5808000D,  //  0028  LDCONST	R2	K13
      0x600C0010,  //  0029  GETGBL	R3	G16
      0x6010000C,  //  002A  GETGBL	R4	G12
      0x5C140200,  //  002B  MOVE	R5	R1
      0x7C100200,  //  002C  CALL	R4	1
      0x0410090F,  //  002D  SUB	R4	R4	K15
      0x40121C04,  //  002E  CONNECT	R4	K14	R4
      0x7C0C0200,  //  002F  CALL	R3	1
      0xA8020007,  //  0030  EXBLK	0	#0039
      0x5C100600,  //  0031  MOVE	R4	R3
      0x7C100000,  //  0032  CALL	R4	0
      0x2414090E,  //  0033  GT	R5	R4	K14
      0x78160000,  //  0034  JMPF	R5	#0036
      0x00080510,  //  0035  ADD	R2	R2	K16
      0x94140204,  //  0036  GETIDX	R5	R1	R4
      0x00080405,  //  0037  ADD	R2	R2	R5
      0x7001FFF7,  //  0038  JMP		#0031
      0x580C0011,  //  0039  LDCONST	R3	K17
      0xAC0C0200,  //  003A  CATCH	R3	1	0
      0xB0080000,  //  003B  RAISE	2	R0	R0
      0x00080512,  //  003C  ADD	R2	R2	K18
      0x80040400,  //  003D  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: has_warnings
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_has_warnings,   /* name */
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
    /* K0   */  be_nested_str_weak(warnings),
    /* K1   */  be_const_int(0),
    }),
    be_str_weak(has_warnings),
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
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_const_int(1),
    /* K3   */  be_nested_str_weak(peek),
    /* K4   */  be_nested_str_weak(value),
    /* K5   */  be_nested_str_weak(process_value),
    /* K6   */  be_nested_str_weak(collect_inline_comment),
    /* K7   */  be_nested_str_weak(add),
    /* K8   */  be_nested_str_weak(var_X20_X25s__X20_X3D_X20_X25s_X25s),
    /* K9   */  be_nested_str_weak(expr),
    /* K10  */  be_nested_str_weak(symbol_table),
    /* K11  */  be_nested_str_weak(contains),
    /* K12  */  be_nested_str_weak(get),
    /* K13  */  be_nested_str_weak(instance),
    }),
    be_str_weak(_process_simple_value_assignment),
    &be_const_str_solidified,
    ( &(const binstruction[73]) {  /* code */
      0x8C100100,  //  0000  GETMET	R4	R0	K0
      0x7C100200,  //  0001  CALL	R4	1
      0x4C140000,  //  0002  LDNIL	R5
      0x20140805,  //  0003  NE	R5	R4	R5
      0x7816000D,  //  0004  JMPF	R5	#0013
      0x88140901,  //  0005  GETMBR	R5	R4	K1
      0x1C140B02,  //  0006  EQ	R5	R5	K2
      0x7816000A,  //  0007  JMPF	R5	#0013
      0x8C140103,  //  0008  GETMET	R5	R0	K3
      0x7C140200,  //  0009  CALL	R5	1
      0x4C180000,  //  000A  LDNIL	R6
      0x1C140A06,  //  000B  EQ	R5	R5	R6
      0x74160006,  //  000C  JMPT	R5	#0014
      0x8C140103,  //  000D  GETMET	R5	R0	K3
      0x7C140200,  //  000E  CALL	R5	1
      0x88140B01,  //  000F  GETMBR	R5	R5	K1
      0x541A0017,  //  0010  LDINT	R6	24
      0x20140A06,  //  0011  NE	R5	R5	R6
      0x74160000,  //  0012  JMPT	R5	#0014
      0x50140001,  //  0013  LDBOOL	R5	0	1
      0x50140200,  //  0014  LDBOOL	R5	1	0
      0x78160001,  //  0015  JMPF	R5	#0018
      0x88180904,  //  0016  GETMBR	R6	R4	K4
      0x70020000,  //  0017  JMP		#0019
      0x4C180000,  //  0018  LDNIL	R6
      0x8C1C0105,  //  0019  GETMET	R7	R0	K5
      0x5C240400,  //  001A  MOVE	R9	R2
      0x7C1C0400,  //  001B  CALL	R7	2
      0x8C200106,  //  001C  GETMET	R8	R0	K6
      0x7C200200,  //  001D  CALL	R8	1
      0x8C240107,  //  001E  GETMET	R9	R0	K7
      0x602C0018,  //  001F  GETGBL	R11	G24
      0x58300008,  //  0020  LDCONST	R12	K8
      0x5C340200,  //  0021  MOVE	R13	R1
      0x88380F09,  //  0022  GETMBR	R14	R7	K9
      0x5C3C1000,  //  0023  MOVE	R15	R8
      0x7C2C0800,  //  0024  CALL	R11	4
      0x7C240400,  //  0025  CALL	R9	2
      0x7816001C,  //  0026  JMPF	R5	#0044
      0x4C240000,  //  0027  LDNIL	R9
      0x20240C09,  //  0028  NE	R9	R6	R9
      0x78260019,  //  0029  JMPF	R9	#0044
      0x8824010A,  //  002A  GETMBR	R9	R0	K10
      0x8C24130B,  //  002B  GETMET	R9	R9	K11
      0x5C2C0C00,  //  002C  MOVE	R11	R6
      0x7C240400,  //  002D  CALL	R9	2
      0x78260014,  //  002E  JMPF	R9	#0044
      0x8824010A,  //  002F  GETMBR	R9	R0	K10
      0x8C24130C,  //  0030  GETMET	R9	R9	K12
      0x5C2C0C00,  //  0031  MOVE	R11	R6
      0x7C240400,  //  0032  CALL	R9	2
      0x4C280000,  //  0033  LDNIL	R10
      0x2028120A,  //  0034  NE	R10	R9	R10
      0x782A0008,  //  0035  JMPF	R10	#003F
      0x8828130D,  //  0036  GETMBR	R10	R9	K13
      0x4C2C0000,  //  0037  LDNIL	R11
      0x2028140B,  //  0038  NE	R10	R10	R11
      0x782A0004,  //  0039  JMPF	R10	#003F
      0x5C280600,  //  003A  MOVE	R10	R3
      0x5C2C0200,  //  003B  MOVE	R11	R1
      0x8830130D,  //  003C  GETMBR	R12	R9	K13
      0x7C280400,  //  003D  CALL	R10	2
      0x70020003,  //  003E  JMP		#0043
      0x5C280600,  //  003F  MOVE	R10	R3
      0x5C2C0200,  //  0040  MOVE	R11	R1
      0x4C300000,  //  0041  LDNIL	R12
      0x7C280400,  //  0042  CALL	R10	2
      0x70020003,  //  0043  JMP		#0048
      0x5C240600,  //  0044  MOVE	R9	R3
      0x5C280200,  //  0045  MOVE	R10	R1
      0x4C2C0000,  //  0046  LDNIL	R11
      0x7C240400,  //  0047  CALL	R9	2
      0x80000000,  //  0048  RET	0
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
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(next),
    /* K3   */  be_nested_str_weak(error),
    /* K4   */  be_nested_str_weak(Expected_X20_X27_X5D_X27),
    }),
    be_str_weak(expect_right_bracket),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0006,  //  0004  JMPF	R2	#000C
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0x540E001C,  //  0006  LDINT	R3	29
      0x1C080403,  //  0007  EQ	R2	R2	R3
      0x780A0002,  //  0008  JMPF	R2	#000C
      0x8C080102,  //  0009  GETMET	R2	R0	K2
      0x7C080200,  //  000A  CALL	R2	1
      0x70020002,  //  000B  JMP		#000F
      0x8C080103,  //  000C  GETMET	R2	R0	K3
      0x58100004,  //  000D  LDCONST	R4	K4
      0x7C080400,  //  000E  CALL	R2	2
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: generate_template_function_direct
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_generate_template_function_direct,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[29]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation_dsl),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(engine),
    /* K3   */  be_nested_str_weak(_X2C_X20_X25s_),
    /* K4   */  be_nested_str_weak(stop_iteration),
    /* K5   */  be_nested_str_weak(add),
    /* K6   */  be_nested_str_weak(_X23_X20Template_X20function_X3A_X20_X25s),
    /* K7   */  be_nested_str_weak(def_X20_X25s_template_X28_X25s_X29),
    /* K8   */  be_nested_str_weak(SimpleDSLTranspiler),
    /* K9   */  be_nested_str_weak(pull_lexer),
    /* K10  */  be_nested_str_weak(symbol_table),
    /* K11  */  be_nested_str_weak(_symbol_table),
    /* K12  */  be_nested_str_weak(strip_initialized),
    /* K13  */  be_nested_str_weak(find),
    /* K14  */  be_nested_str_weak(_add_typed_parameter_to_symbol_table),
    /* K15  */  be_nested_str_weak(create_variable),
    /* K16  */  be_nested_str_weak(transpile_template_body),
    /* K17  */  be_nested_str_weak(split),
    /* K18  */  be_nested_str_weak(_X0A),
    /* K19  */  be_const_int(0),
    /* K20  */  be_nested_str_weak(_X20_X20_X25s),
    /* K21  */  be_nested_str_weak(_validate_template_parameter_usage),
    /* K22  */  be_nested_str_weak(errors),
    /* K23  */  be_nested_str_weak(error),
    /* K24  */  be_nested_str_weak(Template_X20_X27_X25s_X27_X20body_X20error_X3A_X20_X25s),
    /* K25  */  be_nested_str_weak(expect_right_brace),
    /* K26  */  be_nested_str_weak(end),
    /* K27  */  be_nested_str_weak(),
    /* K28  */  be_nested_str_weak(animation_X2Eregister_user_function_X28_X27_X25s_X27_X2C_X20_X25s_template_X29),
    }),
    be_str_weak(generate_template_function_direct),
    &be_const_str_solidified,
    ( &(const binstruction[137]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0xA4160200,  //  0001  IMPORT	R5	K1
      0x58180002,  //  0002  LDCONST	R6	K2
      0x601C0010,  //  0003  GETGBL	R7	G16
      0x5C200400,  //  0004  MOVE	R8	R2
      0x7C1C0200,  //  0005  CALL	R7	1
      0xA8020007,  //  0006  EXBLK	0	#000F
      0x5C200E00,  //  0007  MOVE	R8	R7
      0x7C200000,  //  0008  CALL	R8	0
      0x60240018,  //  0009  GETGBL	R9	G24
      0x58280003,  //  000A  LDCONST	R10	K3
      0x5C2C1000,  //  000B  MOVE	R11	R8
      0x7C240400,  //  000C  CALL	R9	2
      0x00180C09,  //  000D  ADD	R6	R6	R9
      0x7001FFF7,  //  000E  JMP		#0007
      0x581C0004,  //  000F  LDCONST	R7	K4
      0xAC1C0200,  //  0010  CATCH	R7	1	0
      0xB0080000,  //  0011  RAISE	2	R0	R0
      0x8C1C0105,  //  0012  GETMET	R7	R0	K5
      0x60240018,  //  0013  GETGBL	R9	G24
      0x58280006,  //  0014  LDCONST	R10	K6
      0x5C2C0200,  //  0015  MOVE	R11	R1
      0x7C240400,  //  0016  CALL	R9	2
      0x7C1C0400,  //  0017  CALL	R7	2
      0x8C1C0105,  //  0018  GETMET	R7	R0	K5
      0x60240018,  //  0019  GETGBL	R9	G24
      0x58280007,  //  001A  LDCONST	R10	K7
      0x5C2C0200,  //  001B  MOVE	R11	R1
      0x5C300C00,  //  001C  MOVE	R12	R6
      0x7C240600,  //  001D  CALL	R9	3
      0x7C1C0400,  //  001E  CALL	R7	2
      0x8C1C0908,  //  001F  GETMET	R7	R4	K8
      0x88240109,  //  0020  GETMBR	R9	R0	K9
      0x7C1C0400,  //  0021  CALL	R7	2
      0x8C20090B,  //  0022  GETMET	R8	R4	K11
      0x7C200200,  //  0023  CALL	R8	1
      0x901E1408,  //  0024  SETMBR	R7	K10	R8
      0x50200200,  //  0025  LDBOOL	R8	1	0
      0x901E1808,  //  0026  SETMBR	R7	K12	R8
      0x60200010,  //  0027  GETGBL	R8	G16
      0x5C240400,  //  0028  MOVE	R9	R2
      0x7C200200,  //  0029  CALL	R8	1
      0xA8020012,  //  002A  EXBLK	0	#003E
      0x5C241000,  //  002B  MOVE	R9	R8
      0x7C240000,  //  002C  CALL	R9	0
      0x8C28070D,  //  002D  GETMET	R10	R3	K13
      0x5C301200,  //  002E  MOVE	R12	R9
      0x7C280400,  //  002F  CALL	R10	2
      0x4C2C0000,  //  0030  LDNIL	R11
      0x202C140B,  //  0031  NE	R11	R10	R11
      0x782E0005,  //  0032  JMPF	R11	#0039
      0x8C2C010E,  //  0033  GETMET	R11	R0	K14
      0x88340F0A,  //  0034  GETMBR	R13	R7	K10
      0x5C381200,  //  0035  MOVE	R14	R9
      0x5C3C1400,  //  0036  MOVE	R15	R10
      0x7C2C0800,  //  0037  CALL	R11	4
      0x70020003,  //  0038  JMP		#003D
      0x882C0F0A,  //  0039  GETMBR	R11	R7	K10
      0x8C2C170F,  //  003A  GETMET	R11	R11	K15
      0x5C341200,  //  003B  MOVE	R13	R9
      0x7C2C0400,  //  003C  CALL	R11	2
      0x7001FFEC,  //  003D  JMP		#002B
      0x58200004,  //  003E  LDCONST	R8	K4
      0xAC200200,  //  003F  CATCH	R8	1	0
      0xB0080000,  //  0040  RAISE	2	R0	R0
      0x8C200F10,  //  0041  GETMET	R8	R7	K16
      0x7C200200,  //  0042  CALL	R8	1
      0x4C240000,  //  0043  LDNIL	R9
      0x20241009,  //  0044  NE	R9	R8	R9
      0x7826001E,  //  0045  JMPF	R9	#0065
      0x8C240B11,  //  0046  GETMET	R9	R5	K17
      0x5C2C1000,  //  0047  MOVE	R11	R8
      0x58300012,  //  0048  LDCONST	R12	K18
      0x7C240600,  //  0049  CALL	R9	3
      0x60280010,  //  004A  GETGBL	R10	G16
      0x5C2C1200,  //  004B  MOVE	R11	R9
      0x7C280200,  //  004C  CALL	R10	1
      0xA802000D,  //  004D  EXBLK	0	#005C
      0x5C2C1400,  //  004E  MOVE	R11	R10
      0x7C2C0000,  //  004F  CALL	R11	0
      0x6030000C,  //  0050  GETGBL	R12	G12
      0x5C341600,  //  0051  MOVE	R13	R11
      0x7C300200,  //  0052  CALL	R12	1
      0x24301913,  //  0053  GT	R12	R12	K19
      0x78320005,  //  0054  JMPF	R12	#005B
      0x8C300105,  //  0055  GETMET	R12	R0	K5
      0x60380018,  //  0056  GETGBL	R14	G24
      0x583C0014,  //  0057  LDCONST	R15	K20
      0x5C401600,  //  0058  MOVE	R16	R11
      0x7C380400,  //  0059  CALL	R14	2
      0x7C300400,  //  005A  CALL	R12	2
      0x7001FFF1,  //  005B  JMP		#004E
      0x58280004,  //  005C  LDCONST	R10	K4
      0xAC280200,  //  005D  CATCH	R10	1	0
      0xB0080000,  //  005E  RAISE	2	R0	R0
      0x8C280115,  //  005F  GETMET	R10	R0	K21
      0x5C300200,  //  0060  MOVE	R12	R1
      0x5C340400,  //  0061  MOVE	R13	R2
      0x5C381000,  //  0062  MOVE	R14	R8
      0x7C280800,  //  0063  CALL	R10	4
      0x70020010,  //  0064  JMP		#0076
      0x60240010,  //  0065  GETGBL	R9	G16
      0x88280F16,  //  0066  GETMBR	R10	R7	K22
      0x7C240200,  //  0067  CALL	R9	1
      0xA8020009,  //  0068  EXBLK	0	#0073
      0x5C281200,  //  0069  MOVE	R10	R9
      0x7C280000,  //  006A  CALL	R10	0
      0x8C2C0117,  //  006B  GETMET	R11	R0	K23
      0x60340018,  //  006C  GETGBL	R13	G24
      0x58380018,  //  006D  LDCONST	R14	K24
      0x5C3C0200,  //  006E  MOVE	R15	R1
      0x5C401400,  //  006F  MOVE	R16	R10
      0x7C340600,  //  0070  CALL	R13	3
      0x7C2C0400,  //  0071  CALL	R11	2
      0x7001FFF5,  //  0072  JMP		#0069
      0x58240004,  //  0073  LDCONST	R9	K4
      0xAC240200,  //  0074  CATCH	R9	1	0
      0xB0080000,  //  0075  RAISE	2	R0	R0
      0x8C240119,  //  0076  GETMET	R9	R0	K25
      0x7C240200,  //  0077  CALL	R9	1
      0x8C240105,  //  0078  GETMET	R9	R0	K5
      0x582C001A,  //  0079  LDCONST	R11	K26
      0x7C240400,  //  007A  CALL	R9	2
      0x8C240105,  //  007B  GETMET	R9	R0	K5
      0x582C001B,  //  007C  LDCONST	R11	K27
      0x7C240400,  //  007D  CALL	R9	2
      0x8C240105,  //  007E  GETMET	R9	R0	K5
      0x602C0018,  //  007F  GETGBL	R11	G24
      0x5830001C,  //  0080  LDCONST	R12	K28
      0x5C340200,  //  0081  MOVE	R13	R1
      0x5C380200,  //  0082  MOVE	R14	R1
      0x7C2C0600,  //  0083  CALL	R11	3
      0x7C240400,  //  0084  CALL	R9	2
      0x8C240105,  //  0085  GETMET	R9	R0	K5
      0x582C001B,  //  0086  LDCONST	R11	K27
      0x7C240400,  //  0087  CALL	R9	2
      0x80000000,  //  0088  RET	0
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
    ( &(const bvalue[43]) {     /* constants */
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
    /* K13  */  be_nested_str_weak(at_end),
    /* K14  */  be_nested_str_weak(Cannot_X20mix_X20alternative_X20syntax_X20_X5Bcolor1_X2C_X20color2_X2C_X20_X2E_X2E_X2E_X5D_X20with_X20tuple_X20syntax_X20_X28value_X2C_X20color_X29_X2E_X20Use_X20only_X20one_X20syntax_X20per_X20palette_X2E),
    /* K15  */  be_nested_str_weak(expect_left_paren),
    /* K16  */  be_nested_str_weak(expect_number),
    /* K17  */  be_nested_str_weak(expect_comma),
    /* K18  */  be_nested_str_weak(process_palette_color),
    /* K19  */  be_nested_str_weak(expect_right_paren),
    /* K20  */  be_nested_str_weak(convert_to_vrgb),
    /* K21  */  be_nested_str_weak(0x_X25s),
    /* K22  */  be_nested_str_weak(push),
    /* K23  */  be_nested_str_weak(Cannot_X20mix_X20tuple_X20syntax_X20_X28value_X2C_X20color_X29_X20with_X20alternative_X20syntax_X20_X5Bcolor1_X2C_X20color2_X2C_X20_X2E_X2E_X2E_X5D_X2E_X20Use_X20only_X20one_X20syntax_X20per_X20palette_X2E),
    /* K24  */  be_nested_str_weak(),
    /* K25  */  be_nested_str_weak(value),
    /* K26  */  be_nested_str_weak(Expected_X20_X27_X2C_X27_X20or_X20_X27_X5D_X27_X20in_X20palette_X20definition),
    /* K27  */  be_nested_str_weak(expect_right_bracket),
    /* K28  */  be_nested_str_weak(collect_inline_comment),
    /* K29  */  be_nested_str_weak(stop_iteration),
    /* K30  */  be_nested_str_weak(add),
    /* K31  */  be_nested_str_weak(var_X20_X25s__X20_X3D_X20bytes_X28_X25s),
    /* K32  */  be_const_int(0),
    /* K33  */  be_const_int(1),
    /* K34  */  be_nested_str_weak(_X2508X),
    /* K35  */  be_nested_str_weak(_X20_X20_X25s),
    /* K36  */  be_nested_str_weak(_X20_X20_X22_X25s_X22_X25s),
    /* K37  */  be_nested_str_weak(_X29),
    /* K38  */  be_nested_str_weak(_X20),
    /* K39  */  be_nested_str_weak(_X22_X25s_X22),
    /* K40  */  be_nested_str_weak(var_X20_X25s__X20_X3D_X20bytes_X28_X25s_X29_X25s),
    /* K41  */  be_nested_str_weak(symbol_table),
    /* K42  */  be_nested_str_weak(create_palette),
    }),
    be_str_weak(process_palette),
    &be_const_str_solidified,
    ( &(const binstruction[333]) {  /* code */
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
      0x78120005,  //  0023  JMPF	R4	#002A
      0x8C10010B,  //  0024  GETMET	R4	R0	K11
      0x7C100200,  //  0025  CALL	R4	1
      0x8810090C,  //  0026  GETMBR	R4	R4	K12
      0x54160017,  //  0027  LDINT	R5	24
      0x1C100805,  //  0028  EQ	R4	R4	R5
      0x74120000,  //  0029  JMPT	R4	#002B
      0x50100001,  //  002A  LDBOOL	R4	0	1
      0x50100200,  //  002B  LDBOOL	R4	1	0
      0x8C14010D,  //  002C  GETMET	R5	R0	K13
      0x7C140200,  //  002D  CALL	R5	1
      0x741600B1,  //  002E  JMPT	R5	#00E1
      0x8C140108,  //  002F  GETMET	R5	R0	K8
      0x7C140200,  //  0030  CALL	R5	1
      0x741600AE,  //  0031  JMPT	R5	#00E1
      0x8C140107,  //  0032  GETMET	R5	R0	K7
      0x7C140200,  //  0033  CALL	R5	1
      0x8C140108,  //  0034  GETMET	R5	R0	K8
      0x7C140200,  //  0035  CALL	R5	1
      0x78160000,  //  0036  JMPF	R5	#0038
      0x700200A8,  //  0037  JMP		#00E1
      0x78120028,  //  0038  JMPF	R4	#0062
      0x8C14010B,  //  0039  GETMET	R5	R0	K11
      0x7C140200,  //  003A  CALL	R5	1
      0x4C180000,  //  003B  LDNIL	R6
      0x20140A06,  //  003C  NE	R5	R5	R6
      0x7816000B,  //  003D  JMPF	R5	#004A
      0x8C14010B,  //  003E  GETMET	R5	R0	K11
      0x7C140200,  //  003F  CALL	R5	1
      0x88140B0C,  //  0040  GETMBR	R5	R5	K12
      0x541A0017,  //  0041  LDINT	R6	24
      0x20140A06,  //  0042  NE	R5	R5	R6
      0x78160005,  //  0043  JMPF	R5	#004A
      0x8C140109,  //  0044  GETMET	R5	R0	K9
      0x581C000E,  //  0045  LDCONST	R7	K14
      0x7C140400,  //  0046  CALL	R5	2
      0x8C140104,  //  0047  GETMET	R5	R0	K4
      0x7C140200,  //  0048  CALL	R5	1
      0x80000A00,  //  0049  RET	0
      0x8C14010F,  //  004A  GETMET	R5	R0	K15
      0x7C140200,  //  004B  CALL	R5	1
      0x8C140110,  //  004C  GETMET	R5	R0	K16
      0x7C140200,  //  004D  CALL	R5	1
      0x8C180111,  //  004E  GETMET	R6	R0	K17
      0x7C180200,  //  004F  CALL	R6	1
      0x8C180112,  //  0050  GETMET	R6	R0	K18
      0x7C180200,  //  0051  CALL	R6	1
      0x8C1C0113,  //  0052  GETMET	R7	R0	K19
      0x7C1C0200,  //  0053  CALL	R7	1
      0x8C1C0114,  //  0054  GETMET	R7	R0	K20
      0x5C240A00,  //  0055  MOVE	R9	R5
      0x5C280C00,  //  0056  MOVE	R10	R6
      0x7C1C0600,  //  0057  CALL	R7	3
      0x60200009,  //  0058  GETGBL	R8	G9
      0x60240018,  //  0059  GETGBL	R9	G24
      0x58280015,  //  005A  LDCONST	R10	K21
      0x5C2C0E00,  //  005B  MOVE	R11	R7
      0x7C240400,  //  005C  CALL	R9	2
      0x7C200200,  //  005D  CALL	R8	1
      0x8C240516,  //  005E  GETMET	R9	R2	K22
      0x5C2C1000,  //  005F  MOVE	R11	R8
      0x7C240400,  //  0060  CALL	R9	2
      0x7002001F,  //  0061  JMP		#0082
      0x8C14010B,  //  0062  GETMET	R5	R0	K11
      0x7C140200,  //  0063  CALL	R5	1
      0x4C180000,  //  0064  LDNIL	R6
      0x20140A06,  //  0065  NE	R5	R5	R6
      0x7816000B,  //  0066  JMPF	R5	#0073
      0x8C14010B,  //  0067  GETMET	R5	R0	K11
      0x7C140200,  //  0068  CALL	R5	1
      0x88140B0C,  //  0069  GETMBR	R5	R5	K12
      0x541A0017,  //  006A  LDINT	R6	24
      0x1C140A06,  //  006B  EQ	R5	R5	R6
      0x78160005,  //  006C  JMPF	R5	#0073
      0x8C140109,  //  006D  GETMET	R5	R0	K9
      0x581C0017,  //  006E  LDCONST	R7	K23
      0x7C140400,  //  006F  CALL	R5	2
      0x8C140104,  //  0070  GETMET	R5	R0	K4
      0x7C140200,  //  0071  CALL	R5	1
      0x80000A00,  //  0072  RET	0
      0x8C140112,  //  0073  GETMET	R5	R0	K18
      0x7C140200,  //  0074  CALL	R5	1
      0x8C180114,  //  0075  GETMET	R6	R0	K20
      0x542200FE,  //  0076  LDINT	R8	255
      0x5C240A00,  //  0077  MOVE	R9	R5
      0x7C180600,  //  0078  CALL	R6	3
      0x601C0009,  //  0079  GETGBL	R7	G9
      0x60200018,  //  007A  GETGBL	R8	G24
      0x58240015,  //  007B  LDCONST	R9	K21
      0x5C280C00,  //  007C  MOVE	R10	R6
      0x7C200400,  //  007D  CALL	R8	2
      0x7C1C0200,  //  007E  CALL	R7	1
      0x8C200516,  //  007F  GETMET	R8	R2	K22
      0x5C280E00,  //  0080  MOVE	R10	R7
      0x7C200400,  //  0081  CALL	R8	2
      0x58140018,  //  0082  LDCONST	R5	K24
      0x8C18010B,  //  0083  GETMET	R6	R0	K11
      0x7C180200,  //  0084  CALL	R6	1
      0x4C1C0000,  //  0085  LDNIL	R7
      0x20180C07,  //  0086  NE	R6	R6	R7
      0x781A0029,  //  0087  JMPF	R6	#00B2
      0x8C18010B,  //  0088  GETMET	R6	R0	K11
      0x7C180200,  //  0089  CALL	R6	1
      0x88180D0C,  //  008A  GETMBR	R6	R6	K12
      0x541E001D,  //  008B  LDINT	R7	30
      0x1C180C07,  //  008C  EQ	R6	R6	R7
      0x781A0023,  //  008D  JMPF	R6	#00B2
      0x8C180100,  //  008E  GETMET	R6	R0	K0
      0x7C180200,  //  008F  CALL	R6	1
      0x8C18010B,  //  0090  GETMET	R6	R0	K11
      0x7C180200,  //  0091  CALL	R6	1
      0x4C1C0000,  //  0092  LDNIL	R7
      0x20180C07,  //  0093  NE	R6	R6	R7
      0x781A000A,  //  0094  JMPF	R6	#00A0
      0x8C18010B,  //  0095  GETMET	R6	R0	K11
      0x7C180200,  //  0096  CALL	R6	1
      0x88180D0C,  //  0097  GETMBR	R6	R6	K12
      0x541E0024,  //  0098  LDINT	R7	37
      0x1C180C07,  //  0099  EQ	R6	R6	R7
      0x781A0004,  //  009A  JMPF	R6	#00A0
      0x8C18010B,  //  009B  GETMET	R6	R0	K11
      0x7C180200,  //  009C  CALL	R6	1
      0x88140D19,  //  009D  GETMBR	R5	R6	K25
      0x8C180100,  //  009E  GETMET	R6	R0	K0
      0x7C180200,  //  009F  CALL	R6	1
      0x8C18010D,  //  00A0  GETMET	R6	R0	K13
      0x7C180200,  //  00A1  CALL	R6	1
      0x741A000D,  //  00A2  JMPT	R6	#00B1
      0x8C18010B,  //  00A3  GETMET	R6	R0	K11
      0x7C180200,  //  00A4  CALL	R6	1
      0x4C1C0000,  //  00A5  LDNIL	R7
      0x201C0C07,  //  00A6  NE	R7	R6	R7
      0x781E0006,  //  00A7  JMPF	R7	#00AF
      0x881C0D0C,  //  00A8  GETMBR	R7	R6	K12
      0x54220022,  //  00A9  LDINT	R8	35
      0x1C1C0E08,  //  00AA  EQ	R7	R7	R8
      0x781E0002,  //  00AB  JMPF	R7	#00AF
      0x8C1C0100,  //  00AC  GETMET	R7	R0	K0
      0x7C1C0200,  //  00AD  CALL	R7	1
      0x70020000,  //  00AE  JMP		#00B0
      0x70020000,  //  00AF  JMP		#00B1
      0x7001FFEE,  //  00B0  JMP		#00A0
      0x7002002A,  //  00B1  JMP		#00DD
      0x8C18010B,  //  00B2  GETMET	R6	R0	K11
      0x7C180200,  //  00B3  CALL	R6	1
      0x4C1C0000,  //  00B4  LDNIL	R7
      0x20180C07,  //  00B5  NE	R6	R6	R7
      0x781A000A,  //  00B6  JMPF	R6	#00C2
      0x8C18010B,  //  00B7  GETMET	R6	R0	K11
      0x7C180200,  //  00B8  CALL	R6	1
      0x88180D0C,  //  00B9  GETMBR	R6	R6	K12
      0x541E0022,  //  00BA  LDINT	R7	35
      0x1C180C07,  //  00BB  EQ	R6	R6	R7
      0x781A0004,  //  00BC  JMPF	R6	#00C2
      0x8C180100,  //  00BD  GETMET	R6	R0	K0
      0x7C180200,  //  00BE  CALL	R6	1
      0x8C180107,  //  00BF  GETMET	R6	R0	K7
      0x7C180200,  //  00C0  CALL	R6	1
      0x7002001A,  //  00C1  JMP		#00DD
      0x8C180108,  //  00C2  GETMET	R6	R0	K8
      0x7C180200,  //  00C3  CALL	R6	1
      0x741A0017,  //  00C4  JMPT	R6	#00DD
      0x8C18010B,  //  00C5  GETMET	R6	R0	K11
      0x7C180200,  //  00C6  CALL	R6	1
      0x4C1C0000,  //  00C7  LDNIL	R7
      0x20180C07,  //  00C8  NE	R6	R6	R7
      0x781A000B,  //  00C9  JMPF	R6	#00D6
      0x8C18010B,  //  00CA  GETMET	R6	R0	K11
      0x7C180200,  //  00CB  CALL	R6	1
      0x88180D0C,  //  00CC  GETMBR	R6	R6	K12
      0x541E0024,  //  00CD  LDINT	R7	37
      0x1C180C07,  //  00CE  EQ	R6	R6	R7
      0x781A0005,  //  00CF  JMPF	R6	#00D6
      0x8C18010B,  //  00D0  GETMET	R6	R0	K11
      0x7C180200,  //  00D1  CALL	R6	1
      0x88140D19,  //  00D2  GETMBR	R5	R6	K25
      0x8C180100,  //  00D3  GETMET	R6	R0	K0
      0x7C180200,  //  00D4  CALL	R6	1
      0x70020006,  //  00D5  JMP		#00DD
      0x8C180108,  //  00D6  GETMET	R6	R0	K8
      0x7C180200,  //  00D7  CALL	R6	1
      0x741A0003,  //  00D8  JMPT	R6	#00DD
      0x8C180109,  //  00D9  GETMET	R6	R0	K9
      0x5820001A,  //  00DA  LDCONST	R8	K26
      0x7C180400,  //  00DB  CALL	R6	2
      0x70020003,  //  00DC  JMP		#00E1
      0x8C180716,  //  00DD  GETMET	R6	R3	K22
      0x5C200A00,  //  00DE  MOVE	R8	R5
      0x7C180400,  //  00DF  CALL	R6	2
      0x7001FF4A,  //  00E0  JMP		#002C
      0x8C14011B,  //  00E1  GETMET	R5	R0	K27
      0x7C140200,  //  00E2  CALL	R5	1
      0x8C14011C,  //  00E3  GETMET	R5	R0	K28
      0x7C140200,  //  00E4  CALL	R5	1
      0x50180000,  //  00E5  LDBOOL	R6	0	0
      0x601C0010,  //  00E6  GETGBL	R7	G16
      0x5C200600,  //  00E7  MOVE	R8	R3
      0x7C1C0200,  //  00E8  CALL	R7	1
      0xA8020008,  //  00E9  EXBLK	0	#00F3
      0x5C200E00,  //  00EA  MOVE	R8	R7
      0x7C200000,  //  00EB  CALL	R8	0
      0x20241118,  //  00EC  NE	R9	R8	K24
      0x78260001,  //  00ED  JMPF	R9	#00F0
      0x50180200,  //  00EE  LDBOOL	R6	1	0
      0x70020000,  //  00EF  JMP		#00F1
      0x7001FFF8,  //  00F0  JMP		#00EA
      0xA8040001,  //  00F1  EXBLK	1	1
      0x70020002,  //  00F2  JMP		#00F6
      0x581C001D,  //  00F3  LDCONST	R7	K29
      0xAC1C0200,  //  00F4  CATCH	R7	1	0
      0xB0080000,  //  00F5  RAISE	2	R0	R0
      0x781A002C,  //  00F6  JMPF	R6	#0124
      0x8C1C011E,  //  00F7  GETMET	R7	R0	K30
      0x60240018,  //  00F8  GETGBL	R9	G24
      0x5828001F,  //  00F9  LDCONST	R10	K31
      0x5C2C0200,  //  00FA  MOVE	R11	R1
      0x5C300A00,  //  00FB  MOVE	R12	R5
      0x7C240600,  //  00FC  CALL	R9	3
      0x7C1C0400,  //  00FD  CALL	R7	2
      0x601C0010,  //  00FE  GETGBL	R7	G16
      0x6020000C,  //  00FF  GETGBL	R8	G12
      0x5C240400,  //  0100  MOVE	R9	R2
      0x7C200200,  //  0101  CALL	R8	1
      0x04201121,  //  0102  SUB	R8	R8	K33
      0x40224008,  //  0103  CONNECT	R8	K32	R8
      0x7C1C0200,  //  0104  CALL	R7	1
      0xA8020016,  //  0105  EXBLK	0	#011D
      0x5C200E00,  //  0106  MOVE	R8	R7
      0x7C200000,  //  0107  CALL	R8	0
      0x60240018,  //  0108  GETGBL	R9	G24
      0x58280022,  //  0109  LDCONST	R10	K34
      0x942C0408,  //  010A  GETIDX	R11	R2	R8
      0x7C240400,  //  010B  CALL	R9	2
      0x94280608,  //  010C  GETIDX	R10	R3	R8
      0x202C1518,  //  010D  NE	R11	R10	K24
      0x782E0004,  //  010E  JMPF	R11	#0114
      0x602C0018,  //  010F  GETGBL	R11	G24
      0x58300023,  //  0110  LDCONST	R12	K35
      0x5C341400,  //  0111  MOVE	R13	R10
      0x7C2C0400,  //  0112  CALL	R11	2
      0x70020000,  //  0113  JMP		#0115
      0x582C0018,  //  0114  LDCONST	R11	K24
      0x8C30011E,  //  0115  GETMET	R12	R0	K30
      0x60380018,  //  0116  GETGBL	R14	G24
      0x583C0024,  //  0117  LDCONST	R15	K36
      0x5C401200,  //  0118  MOVE	R16	R9
      0x5C441600,  //  0119  MOVE	R17	R11
      0x7C380600,  //  011A  CALL	R14	3
      0x7C300400,  //  011B  CALL	R12	2
      0x7001FFE8,  //  011C  JMP		#0106
      0x581C001D,  //  011D  LDCONST	R7	K29
      0xAC1C0200,  //  011E  CATCH	R7	1	0
      0xB0080000,  //  011F  RAISE	2	R0	R0
      0x8C1C011E,  //  0120  GETMET	R7	R0	K30
      0x58240025,  //  0121  LDCONST	R9	K37
      0x7C1C0400,  //  0122  CALL	R7	2
      0x70020022,  //  0123  JMP		#0147
      0x581C0018,  //  0124  LDCONST	R7	K24
      0x60200010,  //  0125  GETGBL	R8	G16
      0x6024000C,  //  0126  GETGBL	R9	G12
      0x5C280400,  //  0127  MOVE	R10	R2
      0x7C240200,  //  0128  CALL	R9	1
      0x04241321,  //  0129  SUB	R9	R9	K33
      0x40264009,  //  012A  CONNECT	R9	K32	R9
      0x7C200200,  //  012B  CALL	R8	1
      0xA802000E,  //  012C  EXBLK	0	#013C
      0x5C241000,  //  012D  MOVE	R9	R8
      0x7C240000,  //  012E  CALL	R9	0
      0x24281320,  //  012F  GT	R10	R9	K32
      0x782A0000,  //  0130  JMPF	R10	#0132
      0x001C0F26,  //  0131  ADD	R7	R7	K38
      0x60280018,  //  0132  GETGBL	R10	G24
      0x582C0022,  //  0133  LDCONST	R11	K34
      0x94300409,  //  0134  GETIDX	R12	R2	R9
      0x7C280400,  //  0135  CALL	R10	2
      0x602C0018,  //  0136  GETGBL	R11	G24
      0x58300027,  //  0137  LDCONST	R12	K39
      0x5C341400,  //  0138  MOVE	R13	R10
      0x7C2C0400,  //  0139  CALL	R11	2
      0x001C0E0B,  //  013A  ADD	R7	R7	R11
      0x7001FFF0,  //  013B  JMP		#012D
      0x5820001D,  //  013C  LDCONST	R8	K29
      0xAC200200,  //  013D  CATCH	R8	1	0
      0xB0080000,  //  013E  RAISE	2	R0	R0
      0x8C20011E,  //  013F  GETMET	R8	R0	K30
      0x60280018,  //  0140  GETGBL	R10	G24
      0x582C0028,  //  0141  LDCONST	R11	K40
      0x5C300200,  //  0142  MOVE	R12	R1
      0x5C340E00,  //  0143  MOVE	R13	R7
      0x5C380A00,  //  0144  MOVE	R14	R5
      0x7C280800,  //  0145  CALL	R10	4
      0x7C200400,  //  0146  CALL	R8	2
      0x881C0129,  //  0147  GETMBR	R7	R0	K41
      0x8C1C0F2A,  //  0148  GETMET	R7	R7	K42
      0x5C240200,  //  0149  MOVE	R9	R1
      0x4C280000,  //  014A  LDNIL	R10
      0x7C1C0600,  //  014B  CALL	R7	3
      0x80000000,  //  014C  RET	0
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
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(type),
    /* K1   */  be_const_int(1),
    /* K2   */  be_const_int(2),
    }),
    be_str_weak(_determine_function_return_type),
    &be_const_str_solidified,
    ( &(const binstruction[63]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x20080202,  //  0001  NE	R2	R1	R2
      0x780A0039,  //  0002  JMPF	R2	#003D
      0x88080300,  //  0003  GETMBR	R2	R1	K0
      0x540E0007,  //  0004  LDINT	R3	8
      0x1C080403,  //  0005  EQ	R2	R2	R3
      0x740A0003,  //  0006  JMPT	R2	#000B
      0x88080300,  //  0007  GETMBR	R2	R1	K0
      0x540E0008,  //  0008  LDINT	R3	9
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x780A0002,  //  000A  JMPF	R2	#000E
      0x540A0008,  //  000B  LDINT	R2	9
      0x80040400,  //  000C  RET	1	R2
      0x7002002E,  //  000D  JMP		#003D
      0x88080300,  //  000E  GETMBR	R2	R1	K0
      0x540E0009,  //  000F  LDINT	R3	10
      0x1C080403,  //  0010  EQ	R2	R2	R3
      0x740A0003,  //  0011  JMPT	R2	#0016
      0x88080300,  //  0012  GETMBR	R2	R1	K0
      0x540E000A,  //  0013  LDINT	R3	11
      0x1C080403,  //  0014  EQ	R2	R2	R3
      0x780A0002,  //  0015  JMPF	R2	#0019
      0x540A000A,  //  0016  LDINT	R2	11
      0x80040400,  //  0017  RET	1	R2
      0x70020023,  //  0018  JMP		#003D
      0x88080300,  //  0019  GETMBR	R2	R1	K0
      0x540E0005,  //  001A  LDINT	R3	6
      0x1C080403,  //  001B  EQ	R2	R2	R3
      0x740A0003,  //  001C  JMPT	R2	#0021
      0x88080300,  //  001D  GETMBR	R2	R1	K0
      0x540E0006,  //  001E  LDINT	R3	7
      0x1C080403,  //  001F  EQ	R2	R2	R3
      0x780A0002,  //  0020  JMPF	R2	#0024
      0x540A0006,  //  0021  LDINT	R2	7
      0x80040400,  //  0022  RET	1	R2
      0x70020018,  //  0023  JMP		#003D
      0x88080300,  //  0024  GETMBR	R2	R1	K0
      0x1C080501,  //  0025  EQ	R2	R2	K1
      0x740A0002,  //  0026  JMPT	R2	#002A
      0x88080300,  //  0027  GETMBR	R2	R1	K0
      0x1C080502,  //  0028  EQ	R2	R2	K2
      0x780A0001,  //  0029  JMPF	R2	#002C
      0x80060400,  //  002A  RET	1	K2
      0x70020010,  //  002B  JMP		#003D
      0x88080300,  //  002C  GETMBR	R2	R1	K0
      0x540E0003,  //  002D  LDINT	R3	4
      0x1C080403,  //  002E  EQ	R2	R2	R3
      0x780A0002,  //  002F  JMPF	R2	#0033
      0x540A000B,  //  0030  LDINT	R2	12
      0x80040400,  //  0031  RET	1	R2
      0x70020009,  //  0032  JMP		#003D
      0x88080300,  //  0033  GETMBR	R2	R1	K0
      0x540E0004,  //  0034  LDINT	R3	5
      0x1C080403,  //  0035  EQ	R2	R2	R3
      0x740A0003,  //  0036  JMPT	R2	#003B
      0x88080300,  //  0037  GETMBR	R2	R1	K0
      0x540E000D,  //  0038  LDINT	R3	14
      0x1C080403,  //  0039  EQ	R2	R2	R3
      0x780A0001,  //  003A  JMPF	R2	#003D
      0x540A000B,  //  003B  LDINT	R2	12
      0x80040400,  //  003C  RET	1	R2
      0x540A000B,  //  003D  LDINT	R2	12
      0x80040400,  //  003E  RET	1	R2
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
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    }),
    be_str_weak(check_right_paren),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0003,  //  0004  JMPF	R2	#0009
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0x540E0018,  //  0006  LDINT	R3	25
      0x1C080403,  //  0007  EQ	R2	R2	R3
      0x740A0000,  //  0008  JMPT	R2	#000A
      0x50080001,  //  0009  LDBOOL	R2	0	1
      0x50080200,  //  000A  LDBOOL	R2	1	0
      0x80040400,  //  000B  RET	1	R2
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
    ( &(const bvalue[22]) {     /* constants */
    /* K0   */  be_nested_str_weak(expect_identifier),
    /* K1   */  be_nested_str_weak(current),
    /* K2   */  be_nested_str_weak(type),
    /* K3   */  be_nested_str_weak(next),
    /* K4   */  be_nested_str_weak(symbol_table),
    /* K5   */  be_nested_str_weak(contains),
    /* K6   */  be_nested_str_weak(get),
    /* K7   */  be_nested_str_weak(instance),
    /* K8   */  be_nested_str_weak(_validate_single_parameter),
    /* K9   */  be_nested_str_weak(error),
    /* K10  */  be_nested_str_weak(Sequences_X20like_X20_X27_X25s_X27_X20do_X20not_X20have_X20properties_X2E_X20Property_X20assignments_X20are_X20only_X20valid_X20for_X20animations_X20and_X20color_X20providers_X2E),
    /* K11  */  be_nested_str_weak(expect_assign),
    /* K12  */  be_nested_str_weak(process_value),
    /* K13  */  be_nested_str_weak(CONTEXT_PROPERTY),
    /* K14  */  be_nested_str_weak(collect_inline_comment),
    /* K15  */  be_nested_str_weak(get_reference),
    /* K16  */  be_nested_str_weak(def_X20_X28engine_X29_X20_X25s_X2E_X25s_X20_X3D_X20_X25s_X20end),
    /* K17  */  be_nested_str_weak(expr),
    /* K18  */  be_nested_str_weak(add),
    /* K19  */  be_nested_str_weak(_X25s_X25s_X2Epush_X28animation_X2Ecreate_assign_step_X28_X25s_X29_X29_X25s),
    /* K20  */  be_nested_str_weak(Expected_X20property_X20assignment_X20for_X20_X27_X25s_X27_X20but_X20found_X20no_X20dot),
    /* K21  */  be_nested_str_weak(skip_statement),
    }),
    be_str_weak(process_sequence_assignment_generic),
    &be_const_str_solidified,
    ( &(const binstruction[92]) {  /* code */
      0x8C0C0100,  //  0000  GETMET	R3	R0	K0
      0x7C0C0200,  //  0001  CALL	R3	1
      0x8C100101,  //  0002  GETMET	R4	R0	K1
      0x7C100200,  //  0003  CALL	R4	1
      0x4C140000,  //  0004  LDNIL	R5
      0x20100805,  //  0005  NE	R4	R4	R5
      0x7812004B,  //  0006  JMPF	R4	#0053
      0x8C100101,  //  0007  GETMET	R4	R0	K1
      0x7C100200,  //  0008  CALL	R4	1
      0x88100902,  //  0009  GETMBR	R4	R4	K2
      0x54160020,  //  000A  LDINT	R5	33
      0x1C100805,  //  000B  EQ	R4	R4	R5
      0x78120045,  //  000C  JMPF	R4	#0053
      0x8C100103,  //  000D  GETMET	R4	R0	K3
      0x7C100200,  //  000E  CALL	R4	1
      0x8C100100,  //  000F  GETMET	R4	R0	K0
      0x7C100200,  //  0010  CALL	R4	1
      0x88140104,  //  0011  GETMBR	R5	R0	K4
      0x8C140B05,  //  0012  GETMET	R5	R5	K5
      0x5C1C0600,  //  0013  MOVE	R7	R3
      0x7C140400,  //  0014  CALL	R5	2
      0x78160021,  //  0015  JMPF	R5	#0038
      0x88140104,  //  0016  GETMBR	R5	R0	K4
      0x8C140B06,  //  0017  GETMET	R5	R5	K6
      0x5C1C0600,  //  0018  MOVE	R7	R3
      0x7C140400,  //  0019  CALL	R5	2
      0x4C180000,  //  001A  LDNIL	R6
      0x20180A06,  //  001B  NE	R6	R5	R6
      0x781A000C,  //  001C  JMPF	R6	#002A
      0x88180B07,  //  001D  GETMBR	R6	R5	K7
      0x4C1C0000,  //  001E  LDNIL	R7
      0x20180C07,  //  001F  NE	R6	R6	R7
      0x781A0008,  //  0020  JMPF	R6	#002A
      0x60180005,  //  0021  GETGBL	R6	G5
      0x881C0B07,  //  0022  GETMBR	R7	R5	K7
      0x7C180200,  //  0023  CALL	R6	1
      0x8C1C0108,  //  0024  GETMET	R7	R0	K8
      0x5C240C00,  //  0025  MOVE	R9	R6
      0x5C280800,  //  0026  MOVE	R10	R4
      0x882C0B07,  //  0027  GETMBR	R11	R5	K7
      0x7C1C0800,  //  0028  CALL	R7	4
      0x7002000D,  //  0029  JMP		#0038
      0x4C180000,  //  002A  LDNIL	R6
      0x20180A06,  //  002B  NE	R6	R5	R6
      0x781A000A,  //  002C  JMPF	R6	#0038
      0x88180B02,  //  002D  GETMBR	R6	R5	K2
      0x541E000C,  //  002E  LDINT	R7	13
      0x1C180C07,  //  002F  EQ	R6	R6	R7
      0x781A0006,  //  0030  JMPF	R6	#0038
      0x8C180109,  //  0031  GETMET	R6	R0	K9
      0x60200018,  //  0032  GETGBL	R8	G24
      0x5824000A,  //  0033  LDCONST	R9	K10
      0x5C280600,  //  0034  MOVE	R10	R3
      0x7C200400,  //  0035  CALL	R8	2
      0x7C180400,  //  0036  CALL	R6	2
      0x80000C00,  //  0037  RET	0
      0x8C14010B,  //  0038  GETMET	R5	R0	K11
      0x7C140200,  //  0039  CALL	R5	1
      0x8C14010C,  //  003A  GETMET	R5	R0	K12
      0x881C010D,  //  003B  GETMBR	R7	R0	K13
      0x7C140400,  //  003C  CALL	R5	2
      0x8C18010E,  //  003D  GETMET	R6	R0	K14
      0x7C180200,  //  003E  CALL	R6	1
      0x881C0104,  //  003F  GETMBR	R7	R0	K4
      0x8C1C0F0F,  //  0040  GETMET	R7	R7	K15
      0x5C240600,  //  0041  MOVE	R9	R3
      0x7C1C0400,  //  0042  CALL	R7	2
      0x60200018,  //  0043  GETGBL	R8	G24
      0x58240010,  //  0044  LDCONST	R9	K16
      0x5C280E00,  //  0045  MOVE	R10	R7
      0x5C2C0800,  //  0046  MOVE	R11	R4
      0x88300B11,  //  0047  GETMBR	R12	R5	K17
      0x7C200800,  //  0048  CALL	R8	4
      0x8C240112,  //  0049  GETMET	R9	R0	K18
      0x602C0018,  //  004A  GETGBL	R11	G24
      0x58300013,  //  004B  LDCONST	R12	K19
      0x5C340200,  //  004C  MOVE	R13	R1
      0x5C380400,  //  004D  MOVE	R14	R2
      0x5C3C1000,  //  004E  MOVE	R15	R8
      0x5C400C00,  //  004F  MOVE	R16	R6
      0x7C2C0A00,  //  0050  CALL	R11	5
      0x7C240400,  //  0051  CALL	R9	2
      0x70020007,  //  0052  JMP		#005B
      0x8C100109,  //  0053  GETMET	R4	R0	K9
      0x60180018,  //  0054  GETGBL	R6	G24
      0x581C0014,  //  0055  LDCONST	R7	K20
      0x5C200600,  //  0056  MOVE	R8	R3
      0x7C180400,  //  0057  CALL	R6	2
      0x7C100400,  //  0058  CALL	R4	2
      0x8C100115,  //  0059  GETMET	R4	R0	K21
      0x7C100200,  //  005A  CALL	R4	1
      0x80000000,  //  005B  RET	0
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
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(process_unary_expression),
    /* K1   */  be_nested_str_weak(at_end),
    /* K2   */  be_nested_str_weak(current),
    /* K3   */  be_nested_str_weak(type),
    /* K4   */  be_nested_str_weak(value),
    /* K5   */  be_nested_str_weak(next),
    /* K6   */  be_nested_str_weak(has_dangerous),
    /* K7   */  be_nested_str_weak(expr),
    /* K8   */  be_nested_str_weak(error),
    /* K9   */  be_nested_str_weak(Expression_X20_X27_X25s_X27_X20cannot_X20be_X20used_X20in_X20computed_X20expressions_X2E_X20This_X20creates_X20a_X20new_X20instance_X20at_X20each_X20evaluation_X2E_X20Use_X20either_X3A_X0A_X20_X20set_X20var_name_X20_X3D_X20_X25s_X28_X29_X20_X20_X23_X20Single_X20function_X20call_X0A_X20_X20set_X20computed_X20_X3D_X20_X28existing_var_X20_X2B_X201_X29_X20_X2F_X202_X20_X20_X23_X20Computation_X20with_X20existing_X20values),
    /* K10  */  be_nested_str_weak(skip_statement),
    /* K11  */  be_nested_str_weak(ExpressionResult),
    /* K12  */  be_nested_str_weak(literal),
    /* K13  */  be_nested_str_weak(nil),
    /* K14  */  be_nested_str_weak(combine),
    /* K15  */  be_nested_str_weak(_X25s_X20_X25s_X20_X25s),
    }),
    be_str_weak(process_multiplicative_expression),
    &be_const_str_solidified,
    ( &(const binstruction[68]) {  /* code */
      0x8C100100,  //  0000  GETMET	R4	R0	K0
      0x5C180200,  //  0001  MOVE	R6	R1
      0x5C1C0400,  //  0002  MOVE	R7	R2
      0x5C200600,  //  0003  MOVE	R8	R3
      0x7C100800,  //  0004  CALL	R4	4
      0x8C140101,  //  0005  GETMET	R5	R0	K1
      0x7C140200,  //  0006  CALL	R5	1
      0x7416003A,  //  0007  JMPT	R5	#0043
      0x8C140102,  //  0008  GETMET	R5	R0	K2
      0x7C140200,  //  0009  CALL	R5	1
      0x4C180000,  //  000A  LDNIL	R6
      0x20180A06,  //  000B  NE	R6	R5	R6
      0x781A0033,  //  000C  JMPF	R6	#0041
      0x88180B03,  //  000D  GETMBR	R6	R5	K3
      0x541E000A,  //  000E  LDINT	R7	11
      0x1C180C07,  //  000F  EQ	R6	R6	R7
      0x741A0003,  //  0010  JMPT	R6	#0015
      0x88180B03,  //  0011  GETMBR	R6	R5	K3
      0x541E000B,  //  0012  LDINT	R7	12
      0x1C180C07,  //  0013  EQ	R6	R6	R7
      0x781A002B,  //  0014  JMPF	R6	#0041
      0x88180B04,  //  0015  GETMBR	R6	R5	K4
      0x8C1C0105,  //  0016  GETMET	R7	R0	K5
      0x7C1C0200,  //  0017  CALL	R7	1
      0x8C1C0100,  //  0018  GETMET	R7	R0	K0
      0x5C240200,  //  0019  MOVE	R9	R1
      0x50280000,  //  001A  LDBOOL	R10	0	0
      0x5C2C0600,  //  001B  MOVE	R11	R3
      0x7C1C0800,  //  001C  CALL	R7	4
      0x88200906,  //  001D  GETMBR	R8	R4	K6
      0x74220001,  //  001E  JMPT	R8	#0021
      0x88200F06,  //  001F  GETMBR	R8	R7	K6
      0x78220012,  //  0020  JMPF	R8	#0034
      0x88200906,  //  0021  GETMBR	R8	R4	K6
      0x78220001,  //  0022  JMPF	R8	#0025
      0x88200907,  //  0023  GETMBR	R8	R4	K7
      0x70020000,  //  0024  JMP		#0026
      0x88200F07,  //  0025  GETMBR	R8	R7	K7
      0x8C240108,  //  0026  GETMET	R9	R0	K8
      0x602C0018,  //  0027  GETGBL	R11	G24
      0x58300009,  //  0028  LDCONST	R12	K9
      0x5C341000,  //  0029  MOVE	R13	R8
      0x5C381000,  //  002A  MOVE	R14	R8
      0x7C2C0600,  //  002B  CALL	R11	3
      0x7C240400,  //  002C  CALL	R9	2
      0x8C24010A,  //  002D  GETMET	R9	R0	K10
      0x7C240200,  //  002E  CALL	R9	1
      0x8824010B,  //  002F  GETMBR	R9	R0	K11
      0x8C24130C,  //  0030  GETMET	R9	R9	K12
      0x582C000D,  //  0031  LDCONST	R11	K13
      0x7C240400,  //  0032  CALL	R9	2
      0x80041200,  //  0033  RET	1	R9
      0x8820010B,  //  0034  GETMBR	R8	R0	K11
      0x8C20110E,  //  0035  GETMET	R8	R8	K14
      0x60280018,  //  0036  GETGBL	R10	G24
      0x582C000F,  //  0037  LDCONST	R11	K15
      0x88300907,  //  0038  GETMBR	R12	R4	K7
      0x5C340C00,  //  0039  MOVE	R13	R6
      0x88380F07,  //  003A  GETMBR	R14	R7	K7
      0x7C280800,  //  003B  CALL	R10	4
      0x5C2C0800,  //  003C  MOVE	R11	R4
      0x5C300E00,  //  003D  MOVE	R12	R7
      0x7C200800,  //  003E  CALL	R8	4
      0x5C101000,  //  003F  MOVE	R4	R8
      0x70020000,  //  0040  JMP		#0042
      0x70020000,  //  0041  JMP		#0043
      0x7001FFC1,  //  0042  JMP		#0005
      0x80040800,  //  0043  RET	1	R4
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
    /* K0   */  be_nested_str_weak(pull_lexer),
    /* K1   */  be_nested_str_weak(peek_ahead),
    /* K2   */  be_const_int(2),
    }),
    be_str_weak(peek),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x580C0002,  //  0002  LDCONST	R3	K2
      0x7C040400,  //  0003  CALL	R1	2
      0x80040200,  //  0004  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation_dsl),
    /* K1   */  be_nested_str_weak(pull_lexer),
    /* K2   */  be_nested_str_weak(output),
    /* K3   */  be_nested_str_weak(warnings),
    /* K4   */  be_nested_str_weak(run_statements),
    /* K5   */  be_nested_str_weak(strip_initialized),
    /* K6   */  be_nested_str_weak(symbol_table),
    /* K7   */  be_nested_str_weak(_symbol_table),
    /* K8   */  be_nested_str_weak(indent_level),
    /* K9   */  be_const_int(0),
    /* K10  */  be_nested_str_weak(has_template_calls),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x90020201,  //  0001  SETMBR	R0	K1	R1
      0x600C0012,  //  0002  GETGBL	R3	G18
      0x7C0C0000,  //  0003  CALL	R3	0
      0x90020403,  //  0004  SETMBR	R0	K2	R3
      0x600C0012,  //  0005  GETGBL	R3	G18
      0x7C0C0000,  //  0006  CALL	R3	0
      0x90020603,  //  0007  SETMBR	R0	K3	R3
      0x600C0012,  //  0008  GETGBL	R3	G18
      0x7C0C0000,  //  0009  CALL	R3	0
      0x90020803,  //  000A  SETMBR	R0	K4	R3
      0x500C0000,  //  000B  LDBOOL	R3	0	0
      0x90020A03,  //  000C  SETMBR	R0	K5	R3
      0x8C0C0507,  //  000D  GETMET	R3	R2	K7
      0x7C0C0200,  //  000E  CALL	R3	1
      0x90020C03,  //  000F  SETMBR	R0	K6	R3
      0x90021109,  //  0010  SETMBR	R0	K8	K9
      0x500C0000,  //  0011  LDBOOL	R3	0	0
      0x90021403,  //  0012  SETMBR	R0	K10	R3
      0x80000000,  //  0013  RET	0
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
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(value),
    /* K3   */  be_nested_str_weak(next),
    /* K4   */  be_nested_str_weak(convert_time_to_ms),
    /* K5   */  be_const_int(2),
    /* K6   */  be_const_int(1),
    /* K7   */  be_nested_str_weak(_validate_object_reference),
    /* K8   */  be_nested_str_weak(duration),
    /* K9   */  be_nested_str_weak(process_primary_expression),
    /* K10  */  be_nested_str_weak(CONTEXT_TIME),
    /* K11  */  be_nested_str_weak(expr),
    /* K12  */  be_nested_str_weak(error),
    /* K13  */  be_nested_str_weak(Expected_X20time_X20value),
    /* K14  */  be_nested_str_weak(1000),
    }),
    be_str_weak(process_time_value),
    &be_const_str_solidified,
    ( &(const binstruction[63]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A000D,  //  0004  JMPF	R2	#0013
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0x540E0004,  //  0006  LDINT	R3	5
      0x1C080403,  //  0007  EQ	R2	R2	R3
      0x780A0009,  //  0008  JMPF	R2	#0013
      0x88080302,  //  0009  GETMBR	R2	R1	K2
      0x8C0C0103,  //  000A  GETMET	R3	R0	K3
      0x7C0C0200,  //  000B  CALL	R3	1
      0x600C0008,  //  000C  GETGBL	R3	G8
      0x8C100104,  //  000D  GETMET	R4	R0	K4
      0x5C180400,  //  000E  MOVE	R6	R2
      0x7C100400,  //  000F  CALL	R4	2
      0x7C0C0200,  //  0010  CALL	R3	1
      0x80040600,  //  0011  RET	1	R3
      0x7002002A,  //  0012  JMP		#003E
      0x4C080000,  //  0013  LDNIL	R2
      0x20080202,  //  0014  NE	R2	R1	R2
      0x780A0010,  //  0015  JMPF	R2	#0027
      0x88080301,  //  0016  GETMBR	R2	R1	K1
      0x1C080505,  //  0017  EQ	R2	R2	K5
      0x780A000D,  //  0018  JMPF	R2	#0027
      0x88080302,  //  0019  GETMBR	R2	R1	K2
      0x8C0C0103,  //  001A  GETMET	R3	R0	K3
      0x7C0C0200,  //  001B  CALL	R3	1
      0x600C0008,  //  001C  GETGBL	R3	G8
      0x60100009,  //  001D  GETGBL	R4	G9
      0x6014000A,  //  001E  GETGBL	R5	G10
      0x5C180400,  //  001F  MOVE	R6	R2
      0x7C140200,  //  0020  CALL	R5	1
      0x7C100200,  //  0021  CALL	R4	1
      0x541603E7,  //  0022  LDINT	R5	1000
      0x08100805,  //  0023  MUL	R4	R4	R5
      0x7C0C0200,  //  0024  CALL	R3	1
      0x80040600,  //  0025  RET	1	R3
      0x70020016,  //  0026  JMP		#003E
      0x4C080000,  //  0027  LDNIL	R2
      0x20080202,  //  0028  NE	R2	R1	R2
      0x780A000F,  //  0029  JMPF	R2	#003A
      0x88080301,  //  002A  GETMBR	R2	R1	K1
      0x1C080506,  //  002B  EQ	R2	R2	K6
      0x780A000C,  //  002C  JMPF	R2	#003A
      0x88080302,  //  002D  GETMBR	R2	R1	K2
      0x8C0C0107,  //  002E  GETMET	R3	R0	K7
      0x5C140400,  //  002F  MOVE	R5	R2
      0x58180008,  //  0030  LDCONST	R6	K8
      0x7C0C0600,  //  0031  CALL	R3	3
      0x8C0C0109,  //  0032  GETMET	R3	R0	K9
      0x8814010A,  //  0033  GETMBR	R5	R0	K10
      0x50180200,  //  0034  LDBOOL	R6	1	0
      0x501C0000,  //  0035  LDBOOL	R7	0	0
      0x7C0C0800,  //  0036  CALL	R3	4
      0x8810070B,  //  0037  GETMBR	R4	R3	K11
      0x80040800,  //  0038  RET	1	R4
      0x70020003,  //  0039  JMP		#003E
      0x8C08010C,  //  003A  GETMET	R2	R0	K12
      0x5810000D,  //  003B  LDCONST	R4	K13
      0x7C080400,  //  003C  CALL	R2	2
      0x80061C00,  //  003D  RET	1	K14
      0x80000000,  //  003E  RET	0
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
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(next),
    /* K3   */  be_nested_str_weak(error),
    /* K4   */  be_nested_str_weak(Expected_X20_X27_X3D_X27),
    }),
    be_str_weak(expect_assign),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0006,  //  0004  JMPF	R2	#000C
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0x540E0007,  //  0006  LDINT	R3	8
      0x1C080403,  //  0007  EQ	R2	R2	R3
      0x780A0002,  //  0008  JMPF	R2	#000C
      0x8C080102,  //  0009  GETMET	R2	R0	K2
      0x7C080200,  //  000A  CALL	R2	1
      0x70020002,  //  000B  JMP		#000F
      0x8C080103,  //  000C  GETMET	R2	R0	K3
      0x58100004,  //  000D  LDCONST	R4	K4
      0x7C080400,  //  000E  CALL	R2	2
      0x80000000,  //  000F  RET	0
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
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(value),
    /* K3   */  be_nested_str_weak(next),
    /* K4   */  be_const_int(0),
    /* K5   */  be_const_int(2),
    /* K6   */  be_nested_str_weak(error),
    /* K7   */  be_nested_str_weak(Expected_X20percentage_X20value),
    }),
    be_str_weak(process_percentage_value),
    &be_const_str_solidified,
    ( &(const binstruction[47]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0013,  //  0004  JMPF	R2	#0019
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0x540E0005,  //  0006  LDINT	R3	6
      0x1C080403,  //  0007  EQ	R2	R2	R3
      0x780A000F,  //  0008  JMPF	R2	#0019
      0x88080302,  //  0009  GETMBR	R2	R1	K2
      0x8C0C0103,  //  000A  GETMET	R3	R0	K3
      0x7C0C0200,  //  000B  CALL	R3	1
      0x600C000A,  //  000C  GETGBL	R3	G10
      0x5411FFFD,  //  000D  LDINT	R4	-2
      0x40120804,  //  000E  CONNECT	R4	K4	R4
      0x94100404,  //  000F  GETIDX	R4	R2	R4
      0x7C0C0200,  //  0010  CALL	R3	1
      0x60100009,  //  0011  GETGBL	R4	G9
      0x541600FE,  //  0012  LDINT	R5	255
      0x08140605,  //  0013  MUL	R5	R3	R5
      0x541A0063,  //  0014  LDINT	R6	100
      0x0C140A06,  //  0015  DIV	R5	R5	R6
      0x7C100200,  //  0016  CALL	R4	1
      0x80040800,  //  0017  RET	1	R4
      0x70020014,  //  0018  JMP		#002E
      0x4C080000,  //  0019  LDNIL	R2
      0x20080202,  //  001A  NE	R2	R1	R2
      0x780A000C,  //  001B  JMPF	R2	#0029
      0x88080301,  //  001C  GETMBR	R2	R1	K1
      0x1C080505,  //  001D  EQ	R2	R2	K5
      0x780A0009,  //  001E  JMPF	R2	#0029
      0x88080302,  //  001F  GETMBR	R2	R1	K2
      0x8C0C0103,  //  0020  GETMET	R3	R0	K3
      0x7C0C0200,  //  0021  CALL	R3	1
      0x600C0009,  //  0022  GETGBL	R3	G9
      0x6010000A,  //  0023  GETGBL	R4	G10
      0x5C140400,  //  0024  MOVE	R5	R2
      0x7C100200,  //  0025  CALL	R4	1
      0x7C0C0200,  //  0026  CALL	R3	1
      0x80040600,  //  0027  RET	1	R3
      0x70020004,  //  0028  JMP		#002E
      0x8C080106,  //  0029  GETMET	R2	R0	K6
      0x58100007,  //  002A  LDCONST	R4	K7
      0x7C080400,  //  002B  CALL	R2	2
      0x540A00FE,  //  002C  LDINT	R2	255
      0x80040400,  //  002D  RET	1	R2
      0x80000000,  //  002E  RET	0
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
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    }),
    be_str_weak(check_right_brace),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0003,  //  0004  JMPF	R2	#0009
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0x540E001A,  //  0006  LDINT	R3	27
      0x1C080403,  //  0007  EQ	R2	R2	R3
      0x740A0000,  //  0008  JMPT	R2	#000A
      0x50080001,  //  0009  LDBOOL	R2	0	1
      0x50080200,  //  000A  LDBOOL	R2	1	0
      0x80040400,  //  000B  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _add_typed_parameter_to_symbol_table
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__add_typed_parameter_to_symbol_table,   /* name */
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
    /* K0   */  be_nested_str_weak(color),
    /* K1   */  be_nested_str_weak(create_color),
    /* K2   */  be_nested_str_weak(palette),
    /* K3   */  be_nested_str_weak(create_palette),
    /* K4   */  be_nested_str_weak(animation),
    /* K5   */  be_nested_str_weak(create_animation),
    /* K6   */  be_nested_str_weak(value_provider),
    /* K7   */  be_nested_str_weak(create_value_provider),
    /* K8   */  be_nested_str_weak(create_variable),
    }),
    be_str_weak(_add_typed_parameter_to_symbol_table),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0x1C100700,  //  0000  EQ	R4	R3	K0
      0x78120004,  //  0001  JMPF	R4	#0007
      0x8C100301,  //  0002  GETMET	R4	R1	K1
      0x5C180400,  //  0003  MOVE	R6	R2
      0x4C1C0000,  //  0004  LDNIL	R7
      0x7C100600,  //  0005  CALL	R4	3
      0x70020017,  //  0006  JMP		#001F
      0x1C100702,  //  0007  EQ	R4	R3	K2
      0x78120004,  //  0008  JMPF	R4	#000E
      0x8C100303,  //  0009  GETMET	R4	R1	K3
      0x5C180400,  //  000A  MOVE	R6	R2
      0x4C1C0000,  //  000B  LDNIL	R7
      0x7C100600,  //  000C  CALL	R4	3
      0x70020010,  //  000D  JMP		#001F
      0x1C100704,  //  000E  EQ	R4	R3	K4
      0x78120004,  //  000F  JMPF	R4	#0015
      0x8C100305,  //  0010  GETMET	R4	R1	K5
      0x5C180400,  //  0011  MOVE	R6	R2
      0x4C1C0000,  //  0012  LDNIL	R7
      0x7C100600,  //  0013  CALL	R4	3
      0x70020009,  //  0014  JMP		#001F
      0x1C100706,  //  0015  EQ	R4	R3	K6
      0x78120004,  //  0016  JMPF	R4	#001C
      0x8C100307,  //  0017  GETMET	R4	R1	K7
      0x5C180400,  //  0018  MOVE	R6	R2
      0x4C1C0000,  //  0019  LDNIL	R7
      0x7C100600,  //  001A  CALL	R4	3
      0x70020002,  //  001B  JMP		#001F
      0x8C100308,  //  001C  GETMET	R4	R1	K8
      0x5C180400,  //  001D  MOVE	R6	R2
      0x7C100400,  //  001E  CALL	R4	2
      0x80000000,  //  001F  RET	0
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
    ( &(const bvalue[31]) {     /* constants */
    /* K0   */  be_nested_str_weak(expect_identifier),
    /* K1   */  be_nested_str_weak(current),
    /* K2   */  be_nested_str_weak(type),
    /* K3   */  be_nested_str_weak(log),
    /* K4   */  be_nested_str_weak(process_function_arguments),
    /* K5   */  be_nested_str_weak(collect_inline_comment),
    /* K6   */  be_nested_str_weak(process_log_call),
    /* K7   */  be_nested_str_weak(standalone),
    /* K8   */  be_nested_str_weak(add),
    /* K9   */  be_nested_str_weak(symbol_table),
    /* K10  */  be_nested_str_weak(get),
    /* K11  */  be_nested_str_weak(),
    /* K12  */  be_nested_str_weak(engine_X2C_X20_X25s),
    /* K13  */  be_nested_str_weak(engine),
    /* K14  */  be_nested_str_weak(_X25s_template_X28_X25s_X29_X25s),
    /* K15  */  be_nested_str_weak(has_template_calls),
    /* K16  */  be_nested_str_weak(error),
    /* K17  */  be_nested_str_weak(Standalone_X20function_X20calls_X20are_X20only_X20supported_X20for_X20templates_X2E_X20_X27_X25s_X27_X20is_X20not_X20a_X20template_X2E),
    /* K18  */  be_nested_str_weak(skip_statement),
    /* K19  */  be_nested_str_weak(next),
    /* K20  */  be_nested_str_weak(contains),
    /* K21  */  be_nested_str_weak(instance),
    /* K22  */  be_nested_str_weak(_validate_single_parameter),
    /* K23  */  be_nested_str_weak(Sequences_X20like_X20_X27_X25s_X27_X20do_X20not_X20have_X20properties_X2E_X20Property_X20assignments_X20are_X20only_X20valid_X20for_X20animations_X20and_X20color_X20providers_X2E),
    /* K24  */  be_nested_str_weak(expect_assign),
    /* K25  */  be_nested_str_weak(process_value),
    /* K26  */  be_nested_str_weak(CONTEXT_PROPERTY),
    /* K27  */  be_nested_str_weak(get_reference),
    /* K28  */  be_nested_str_weak(_X25s_X2E_X25s_X20_X3D_X20_X25s_X25s),
    /* K29  */  be_nested_str_weak(expr),
    /* K30  */  be_nested_str_weak(Expected_X20property_X20assignment_X20for_X20_X27_X25s_X27_X20but_X20found_X20no_X20dot),
    }),
    be_str_weak(process_property_assignment),
    &be_const_str_solidified,
    ( &(const binstruction[156]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C080101,  //  0002  GETMET	R2	R0	K1
      0x7C080200,  //  0003  CALL	R2	1
      0x4C0C0000,  //  0004  LDNIL	R3
      0x20080403,  //  0005  NE	R2	R2	R3
      0x780A0041,  //  0006  JMPF	R2	#0049
      0x8C080101,  //  0007  GETMET	R2	R0	K1
      0x7C080200,  //  0008  CALL	R2	1
      0x88080502,  //  0009  GETMBR	R2	R2	K2
      0x540E0017,  //  000A  LDINT	R3	24
      0x1C080403,  //  000B  EQ	R2	R2	R3
      0x780A003B,  //  000C  JMPF	R2	#0049
      0x1C080303,  //  000D  EQ	R2	R1	K3
      0x780A000D,  //  000E  JMPF	R2	#001D
      0x8C080104,  //  000F  GETMET	R2	R0	K4
      0x50100000,  //  0010  LDBOOL	R4	0	0
      0x7C080400,  //  0011  CALL	R2	2
      0x8C0C0105,  //  0012  GETMET	R3	R0	K5
      0x7C0C0200,  //  0013  CALL	R3	1
      0x8C100106,  //  0014  GETMET	R4	R0	K6
      0x5C180400,  //  0015  MOVE	R6	R2
      0x581C0007,  //  0016  LDCONST	R7	K7
      0x5C200600,  //  0017  MOVE	R8	R3
      0x7C100800,  //  0018  CALL	R4	4
      0x8C140108,  //  0019  GETMET	R5	R0	K8
      0x5C1C0800,  //  001A  MOVE	R7	R4
      0x7C140400,  //  001B  CALL	R5	2
      0x80000A00,  //  001C  RET	0
      0x88080109,  //  001D  GETMBR	R2	R0	K9
      0x8C08050A,  //  001E  GETMET	R2	R2	K10
      0x5C100200,  //  001F  MOVE	R4	R1
      0x7C080400,  //  0020  CALL	R2	2
      0x4C0C0000,  //  0021  LDNIL	R3
      0x200C0403,  //  0022  NE	R3	R2	R3
      0x780E001B,  //  0023  JMPF	R3	#0040
      0x880C0502,  //  0024  GETMBR	R3	R2	K2
      0x5412000D,  //  0025  LDINT	R4	14
      0x1C0C0604,  //  0026  EQ	R3	R3	R4
      0x780E0017,  //  0027  JMPF	R3	#0040
      0x8C0C0104,  //  0028  GETMET	R3	R0	K4
      0x50140000,  //  0029  LDBOOL	R5	0	0
      0x7C0C0400,  //  002A  CALL	R3	2
      0x2010070B,  //  002B  NE	R4	R3	K11
      0x78120004,  //  002C  JMPF	R4	#0032
      0x60100018,  //  002D  GETGBL	R4	G24
      0x5814000C,  //  002E  LDCONST	R5	K12
      0x5C180600,  //  002F  MOVE	R6	R3
      0x7C100400,  //  0030  CALL	R4	2
      0x70020000,  //  0031  JMP		#0033
      0x5810000D,  //  0032  LDCONST	R4	K13
      0x8C140105,  //  0033  GETMET	R5	R0	K5
      0x7C140200,  //  0034  CALL	R5	1
      0x8C180108,  //  0035  GETMET	R6	R0	K8
      0x60200018,  //  0036  GETGBL	R8	G24
      0x5824000E,  //  0037  LDCONST	R9	K14
      0x5C280200,  //  0038  MOVE	R10	R1
      0x5C2C0800,  //  0039  MOVE	R11	R4
      0x5C300A00,  //  003A  MOVE	R12	R5
      0x7C200800,  //  003B  CALL	R8	4
      0x7C180400,  //  003C  CALL	R6	2
      0x50180200,  //  003D  LDBOOL	R6	1	0
      0x90021E06,  //  003E  SETMBR	R0	K15	R6
      0x70020007,  //  003F  JMP		#0048
      0x8C0C0110,  //  0040  GETMET	R3	R0	K16
      0x60140018,  //  0041  GETGBL	R5	G24
      0x58180011,  //  0042  LDCONST	R6	K17
      0x5C1C0200,  //  0043  MOVE	R7	R1
      0x7C140400,  //  0044  CALL	R5	2
      0x7C0C0400,  //  0045  CALL	R3	2
      0x8C0C0112,  //  0046  GETMET	R3	R0	K18
      0x7C0C0200,  //  0047  CALL	R3	1
      0x80000600,  //  0048  RET	0
      0x8C080101,  //  0049  GETMET	R2	R0	K1
      0x7C080200,  //  004A  CALL	R2	1
      0x4C0C0000,  //  004B  LDNIL	R3
      0x20080403,  //  004C  NE	R2	R2	R3
      0x780A0044,  //  004D  JMPF	R2	#0093
      0x8C080101,  //  004E  GETMET	R2	R0	K1
      0x7C080200,  //  004F  CALL	R2	1
      0x88080502,  //  0050  GETMBR	R2	R2	K2
      0x540E0020,  //  0051  LDINT	R3	33
      0x1C080403,  //  0052  EQ	R2	R2	R3
      0x780A003E,  //  0053  JMPF	R2	#0093
      0x8C080113,  //  0054  GETMET	R2	R0	K19
      0x7C080200,  //  0055  CALL	R2	1
      0x8C080100,  //  0056  GETMET	R2	R0	K0
      0x7C080200,  //  0057  CALL	R2	1
      0x880C0109,  //  0058  GETMBR	R3	R0	K9
      0x8C0C0714,  //  0059  GETMET	R3	R3	K20
      0x5C140200,  //  005A  MOVE	R5	R1
      0x7C0C0400,  //  005B  CALL	R3	2
      0x780E0020,  //  005C  JMPF	R3	#007E
      0x880C0109,  //  005D  GETMBR	R3	R0	K9
      0x8C0C070A,  //  005E  GETMET	R3	R3	K10
      0x5C140200,  //  005F  MOVE	R5	R1
      0x7C0C0400,  //  0060  CALL	R3	2
      0x4C100000,  //  0061  LDNIL	R4
      0x20100604,  //  0062  NE	R4	R3	R4
      0x7812000C,  //  0063  JMPF	R4	#0071
      0x88100715,  //  0064  GETMBR	R4	R3	K21
      0x4C140000,  //  0065  LDNIL	R5
      0x20100805,  //  0066  NE	R4	R4	R5
      0x78120008,  //  0067  JMPF	R4	#0071
      0x60100005,  //  0068  GETGBL	R4	G5
      0x88140715,  //  0069  GETMBR	R5	R3	K21
      0x7C100200,  //  006A  CALL	R4	1
      0x8C140116,  //  006B  GETMET	R5	R0	K22
      0x5C1C0800,  //  006C  MOVE	R7	R4
      0x5C200400,  //  006D  MOVE	R8	R2
      0x88240715,  //  006E  GETMBR	R9	R3	K21
      0x7C140800,  //  006F  CALL	R5	4
      0x7002000C,  //  0070  JMP		#007E
      0x4C100000,  //  0071  LDNIL	R4
      0x20100604,  //  0072  NE	R4	R3	R4
      0x78120009,  //  0073  JMPF	R4	#007E
      0x88100702,  //  0074  GETMBR	R4	R3	K2
      0x5416000C,  //  0075  LDINT	R5	13
      0x1C100805,  //  0076  EQ	R4	R4	R5
      0x78120005,  //  0077  JMPF	R4	#007E
      0x8C100110,  //  0078  GETMET	R4	R0	K16
      0x60180018,  //  0079  GETGBL	R6	G24
      0x581C0017,  //  007A  LDCONST	R7	K23
      0x5C200200,  //  007B  MOVE	R8	R1
      0x7C180400,  //  007C  CALL	R6	2
      0x7C100400,  //  007D  CALL	R4	2
      0x8C0C0118,  //  007E  GETMET	R3	R0	K24
      0x7C0C0200,  //  007F  CALL	R3	1
      0x8C0C0119,  //  0080  GETMET	R3	R0	K25
      0x8814011A,  //  0081  GETMBR	R5	R0	K26
      0x7C0C0400,  //  0082  CALL	R3	2
      0x8C100105,  //  0083  GETMET	R4	R0	K5
      0x7C100200,  //  0084  CALL	R4	1
      0x88140109,  //  0085  GETMBR	R5	R0	K9
      0x8C140B1B,  //  0086  GETMET	R5	R5	K27
      0x5C1C0200,  //  0087  MOVE	R7	R1
      0x7C140400,  //  0088  CALL	R5	2
      0x8C180108,  //  0089  GETMET	R6	R0	K8
      0x60200018,  //  008A  GETGBL	R8	G24
      0x5824001C,  //  008B  LDCONST	R9	K28
      0x5C280A00,  //  008C  MOVE	R10	R5
      0x5C2C0400,  //  008D  MOVE	R11	R2
      0x8830071D,  //  008E  GETMBR	R12	R3	K29
      0x5C340800,  //  008F  MOVE	R13	R4
      0x7C200A00,  //  0090  CALL	R8	5
      0x7C180400,  //  0091  CALL	R6	2
      0x70020007,  //  0092  JMP		#009B
      0x8C080110,  //  0093  GETMET	R2	R0	K16
      0x60100018,  //  0094  GETGBL	R4	G24
      0x5814001E,  //  0095  LDCONST	R5	K30
      0x5C180200,  //  0096  MOVE	R6	R1
      0x7C100400,  //  0097  CALL	R4	2
      0x7C080400,  //  0098  CALL	R2	2
      0x8C080112,  //  0099  GETMET	R2	R0	K18
      0x7C080200,  //  009A  CALL	R2	1
      0x80000000,  //  009B  RET	0
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
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(symbol_table),
    /* K1   */  be_nested_str_weak(get),
    /* K2   */  be_nested_str_weak(type),
    }),
    be_str_weak(_validate_color_provider_factory_exists),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x4C0C0000,  //  0004  LDNIL	R3
      0x200C0403,  //  0005  NE	R3	R2	R3
      0x780E0003,  //  0006  JMPF	R3	#000B
      0x880C0502,  //  0007  GETMBR	R3	R2	K2
      0x54120009,  //  0008  LDINT	R4	10
      0x1C0C0604,  //  0009  EQ	R3	R3	R4
      0x740E0000,  //  000A  JMPT	R3	#000C
      0x500C0001,  //  000B  LDBOOL	R3	0	1
      0x500C0200,  //  000C  LDBOOL	R3	1	0
      0x80040600,  //  000D  RET	1	R3
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
    ( &(const bvalue[22]) {     /* constants */
    /* K0   */  be_nested_str_weak(next),
    /* K1   */  be_nested_str_weak(expect_identifier),
    /* K2   */  be_nested_str_weak(current),
    /* K3   */  be_nested_str_weak(line),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(_X7B_X7D),
    /* K6   */  be_nested_str_weak(type),
    /* K7   */  be_nested_str_weak(process_event_parameters),
    /* K8   */  be_nested_str_weak(expect_colon),
    /* K9   */  be_nested_str_weak(event_handler__X25s__X25s),
    /* K10  */  be_nested_str_weak(add),
    /* K11  */  be_nested_str_weak(def_X20_X25s_X28event_data_X29),
    /* K12  */  be_nested_str_weak(value),
    /* K13  */  be_nested_str_weak(interrupt),
    /* K14  */  be_nested_str_weak(_X20_X20engine_X2Einterrupt_current_X28_X29),
    /* K15  */  be_nested_str_weak(_X20_X20engine_X2Einterrupt_animation_X28_X22_X25s_X22_X29),
    /* K16  */  be_nested_str_weak(process_value),
    /* K17  */  be_nested_str_weak(CONTEXT_ANIMATION),
    /* K18  */  be_nested_str_weak(_X20_X20engine_X2Eadd_X28_X25s_X29),
    /* K19  */  be_nested_str_weak(expr),
    /* K20  */  be_nested_str_weak(end),
    /* K21  */  be_nested_str_weak(animation_X2Eregister_event_handler_X28_X22_X25s_X22_X2C_X20_X25s_X2C_X200_X2C_X20nil_X2C_X20_X25s_X29),
    }),
    be_str_weak(process_event_handler),
    &be_const_str_solidified,
    ( &(const binstruction[91]) {  /* code */
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
      0x78120008,  //  0013  JMPF	R4	#001D
      0x8C100102,  //  0014  GETMET	R4	R0	K2
      0x7C100200,  //  0015  CALL	R4	1
      0x88100906,  //  0016  GETMBR	R4	R4	K6
      0x54160017,  //  0017  LDINT	R5	24
      0x1C100805,  //  0018  EQ	R4	R4	R5
      0x78120002,  //  0019  JMPF	R4	#001D
      0x8C100107,  //  001A  GETMET	R4	R0	K7
      0x7C100200,  //  001B  CALL	R4	1
      0x5C0C0800,  //  001C  MOVE	R3	R4
      0x8C100108,  //  001D  GETMET	R4	R0	K8
      0x7C100200,  //  001E  CALL	R4	1
      0x60100018,  //  001F  GETGBL	R4	G24
      0x58140009,  //  0020  LDCONST	R5	K9
      0x5C180200,  //  0021  MOVE	R6	R1
      0x5C1C0400,  //  0022  MOVE	R7	R2
      0x7C100600,  //  0023  CALL	R4	3
      0x8C14010A,  //  0024  GETMET	R5	R0	K10
      0x601C0018,  //  0025  GETGBL	R7	G24
      0x5820000B,  //  0026  LDCONST	R8	K11
      0x5C240800,  //  0027  MOVE	R9	R4
      0x7C1C0400,  //  0028  CALL	R7	2
      0x7C140400,  //  0029  CALL	R5	2
      0x8C140102,  //  002A  GETMET	R5	R0	K2
      0x7C140200,  //  002B  CALL	R5	1
      0x4C180000,  //  002C  LDNIL	R6
      0x20180A06,  //  002D  NE	R6	R5	R6
      0x781A001F,  //  002E  JMPF	R6	#004F
      0x88180B06,  //  002F  GETMBR	R6	R5	K6
      0x1C180D04,  //  0030  EQ	R6	R6	K4
      0x781A0013,  //  0031  JMPF	R6	#0046
      0x88180B0C,  //  0032  GETMBR	R6	R5	K12
      0x1C180D0D,  //  0033  EQ	R6	R6	K13
      0x781A0010,  //  0034  JMPF	R6	#0046
      0x8C180100,  //  0035  GETMET	R6	R0	K0
      0x7C180200,  //  0036  CALL	R6	1
      0x8C180101,  //  0037  GETMET	R6	R0	K1
      0x7C180200,  //  0038  CALL	R6	1
      0x1C1C0D02,  //  0039  EQ	R7	R6	K2
      0x781E0003,  //  003A  JMPF	R7	#003F
      0x8C1C010A,  //  003B  GETMET	R7	R0	K10
      0x5824000E,  //  003C  LDCONST	R9	K14
      0x7C1C0400,  //  003D  CALL	R7	2
      0x70020005,  //  003E  JMP		#0045
      0x8C1C010A,  //  003F  GETMET	R7	R0	K10
      0x60240018,  //  0040  GETGBL	R9	G24
      0x5828000F,  //  0041  LDCONST	R10	K15
      0x5C2C0C00,  //  0042  MOVE	R11	R6
      0x7C240400,  //  0043  CALL	R9	2
      0x7C1C0400,  //  0044  CALL	R7	2
      0x70020008,  //  0045  JMP		#004F
      0x8C180110,  //  0046  GETMET	R6	R0	K16
      0x88200111,  //  0047  GETMBR	R8	R0	K17
      0x7C180400,  //  0048  CALL	R6	2
      0x8C1C010A,  //  0049  GETMET	R7	R0	K10
      0x60240018,  //  004A  GETGBL	R9	G24
      0x58280012,  //  004B  LDCONST	R10	K18
      0x882C0D13,  //  004C  GETMBR	R11	R6	K19
      0x7C240400,  //  004D  CALL	R9	2
      0x7C1C0400,  //  004E  CALL	R7	2
      0x8C18010A,  //  004F  GETMET	R6	R0	K10
      0x58200014,  //  0050  LDCONST	R8	K20
      0x7C180400,  //  0051  CALL	R6	2
      0x8C18010A,  //  0052  GETMET	R6	R0	K10
      0x60200018,  //  0053  GETGBL	R8	G24
      0x58240015,  //  0054  LDCONST	R9	K21
      0x5C280200,  //  0055  MOVE	R10	R1
      0x5C2C0800,  //  0056  MOVE	R11	R4
      0x5C300600,  //  0057  MOVE	R12	R3
      0x7C200800,  //  0058  CALL	R8	4
      0x7C180400,  //  0059  CALL	R6	2
      0x80000000,  //  005A  RET	0
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
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(next),
    /* K3   */  be_nested_str_weak(error),
    /* K4   */  be_nested_str_weak(Expected_X20_X27_X3A_X27),
    }),
    be_str_weak(expect_colon),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0006,  //  0004  JMPF	R2	#000C
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0x540E001F,  //  0006  LDINT	R3	32
      0x1C080403,  //  0007  EQ	R2	R2	R3
      0x780A0002,  //  0008  JMPF	R2	#000C
      0x8C080102,  //  0009  GETMET	R2	R0	K2
      0x7C080200,  //  000A  CALL	R2	1
      0x70020002,  //  000B  JMP		#000F
      0x8C080103,  //  000C  GETMET	R2	R0	K3
      0x58100004,  //  000D  LDCONST	R4	K4
      0x7C080400,  //  000E  CALL	R2	2
      0x80000000,  //  000F  RET	0
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
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    }),
    be_str_weak(check_right_bracket),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0003,  //  0004  JMPF	R2	#0009
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0x540E001C,  //  0006  LDINT	R3	29
      0x1C080403,  //  0007  EQ	R2	R2	R3
      0x740A0000,  //  0008  JMPT	R2	#000A
      0x50080001,  //  0009  LDBOOL	R2	0	1
      0x50080200,  //  000A  LDBOOL	R2	1	0
      0x80040400,  //  000B  RET	1	R2
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
** Solidified function: _validate_template_parameter_name
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__validate_template_parameter_name,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[30]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation_dsl),
    /* K1   */  be_nested_str_weak(contains),
    /* K2   */  be_nested_str_weak(error),
    /* K3   */  be_nested_str_weak(Duplicate_X20parameter_X20name_X20_X27_X25s_X27_X20in_X20template_X2E_X20Each_X20parameter_X20must_X20have_X20a_X20unique_X20name_X2E),
    /* K4   */  be_nested_str_weak(engine),
    /* K5   */  be_nested_str_weak(self),
    /* K6   */  be_nested_str_weak(animation),
    /* K7   */  be_nested_str_weak(color),
    /* K8   */  be_nested_str_weak(palette),
    /* K9   */  be_nested_str_weak(sequence),
    /* K10  */  be_nested_str_weak(template),
    /* K11  */  be_nested_str_weak(import),
    /* K12  */  be_nested_str_weak(def),
    /* K13  */  be_nested_str_weak(end),
    /* K14  */  be_nested_str_weak(class),
    /* K15  */  be_nested_str_weak(var),
    /* K16  */  be_nested_str_weak(if),
    /* K17  */  be_nested_str_weak(else),
    /* K18  */  be_nested_str_weak(while),
    /* K19  */  be_nested_str_weak(for),
    /* K20  */  be_nested_str_weak(true),
    /* K21  */  be_nested_str_weak(false),
    /* K22  */  be_nested_str_weak(nil),
    /* K23  */  be_nested_str_weak(return),
    /* K24  */  be_nested_str_weak(break),
    /* K25  */  be_nested_str_weak(continue),
    /* K26  */  be_nested_str_weak(Parameter_X20name_X20_X27_X25s_X27_X20conflicts_X20with_X20reserved_X20keyword_X2E_X20Use_X20a_X20different_X20name_X20like_X20_X27_X25s_param_X27_X20or_X20_X27my__X25s_X27_X2E),
    /* K27  */  be_nested_str_weak(stop_iteration),
    /* K28  */  be_nested_str_weak(is_color_name),
    /* K29  */  be_nested_str_weak(Parameter_X20name_X20_X27_X25s_X27_X20conflicts_X20with_X20built_X2Din_X20color_X20name_X2E_X20Use_X20a_X20different_X20name_X20like_X20_X27_X25s_param_X27_X20or_X20_X27my__X25s_X27_X2E),
    }),
    be_str_weak(_validate_template_parameter_name),
    &be_const_str_solidified,
    ( &(const binstruction[76]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0x8C100501,  //  0001  GETMET	R4	R2	K1
      0x5C180200,  //  0002  MOVE	R6	R1
      0x7C100400,  //  0003  CALL	R4	2
      0x78120007,  //  0004  JMPF	R4	#000D
      0x8C100102,  //  0005  GETMET	R4	R0	K2
      0x60180018,  //  0006  GETGBL	R6	G24
      0x581C0003,  //  0007  LDCONST	R7	K3
      0x5C200200,  //  0008  MOVE	R8	R1
      0x7C180400,  //  0009  CALL	R6	2
      0x7C100400,  //  000A  CALL	R4	2
      0x50100000,  //  000B  LDBOOL	R4	0	0
      0x80040800,  //  000C  RET	1	R4
      0x60100012,  //  000D  GETGBL	R4	G18
      0x7C100000,  //  000E  CALL	R4	0
      0x40140904,  //  000F  CONNECT	R5	R4	K4
      0x40140905,  //  0010  CONNECT	R5	R4	K5
      0x40140906,  //  0011  CONNECT	R5	R4	K6
      0x40140907,  //  0012  CONNECT	R5	R4	K7
      0x40140908,  //  0013  CONNECT	R5	R4	K8
      0x40140909,  //  0014  CONNECT	R5	R4	K9
      0x4014090A,  //  0015  CONNECT	R5	R4	K10
      0x4014090B,  //  0016  CONNECT	R5	R4	K11
      0x4014090C,  //  0017  CONNECT	R5	R4	K12
      0x4014090D,  //  0018  CONNECT	R5	R4	K13
      0x4014090E,  //  0019  CONNECT	R5	R4	K14
      0x4014090F,  //  001A  CONNECT	R5	R4	K15
      0x40140910,  //  001B  CONNECT	R5	R4	K16
      0x40140911,  //  001C  CONNECT	R5	R4	K17
      0x40140912,  //  001D  CONNECT	R5	R4	K18
      0x40140913,  //  001E  CONNECT	R5	R4	K19
      0x40140914,  //  001F  CONNECT	R5	R4	K20
      0x40140915,  //  0020  CONNECT	R5	R4	K21
      0x40140916,  //  0021  CONNECT	R5	R4	K22
      0x40140917,  //  0022  CONNECT	R5	R4	K23
      0x40140918,  //  0023  CONNECT	R5	R4	K24
      0x40140919,  //  0024  CONNECT	R5	R4	K25
      0x60140010,  //  0025  GETGBL	R5	G16
      0x5C180800,  //  0026  MOVE	R6	R4
      0x7C140200,  //  0027  CALL	R5	1
      0xA802000F,  //  0028  EXBLK	0	#0039
      0x5C180A00,  //  0029  MOVE	R6	R5
      0x7C180000,  //  002A  CALL	R6	0
      0x1C1C0206,  //  002B  EQ	R7	R1	R6
      0x781E000A,  //  002C  JMPF	R7	#0038
      0x8C1C0102,  //  002D  GETMET	R7	R0	K2
      0x60240018,  //  002E  GETGBL	R9	G24
      0x5828001A,  //  002F  LDCONST	R10	K26
      0x5C2C0200,  //  0030  MOVE	R11	R1
      0x5C300200,  //  0031  MOVE	R12	R1
      0x5C340200,  //  0032  MOVE	R13	R1
      0x7C240800,  //  0033  CALL	R9	4
      0x7C1C0400,  //  0034  CALL	R7	2
      0x501C0000,  //  0035  LDBOOL	R7	0	0
      0xA8040001,  //  0036  EXBLK	1	1
      0x80040E00,  //  0037  RET	1	R7
      0x7001FFEF,  //  0038  JMP		#0029
      0x5814001B,  //  0039  LDCONST	R5	K27
      0xAC140200,  //  003A  CATCH	R5	1	0
      0xB0080000,  //  003B  RAISE	2	R0	R0
      0x8C14071C,  //  003C  GETMET	R5	R3	K28
      0x5C1C0200,  //  003D  MOVE	R7	R1
      0x7C140400,  //  003E  CALL	R5	2
      0x78160009,  //  003F  JMPF	R5	#004A
      0x8C140102,  //  0040  GETMET	R5	R0	K2
      0x601C0018,  //  0041  GETGBL	R7	G24
      0x5820001D,  //  0042  LDCONST	R8	K29
      0x5C240200,  //  0043  MOVE	R9	R1
      0x5C280200,  //  0044  MOVE	R10	R1
      0x5C2C0200,  //  0045  MOVE	R11	R1
      0x7C1C0800,  //  0046  CALL	R7	4
      0x7C140400,  //  0047  CALL	R5	2
      0x50140000,  //  0048  LDBOOL	R5	0	0
      0x80040A00,  //  0049  RET	1	R5
      0x50140200,  //  004A  LDBOOL	R5	1	0
      0x80040A00,  //  004B  RET	1	R5
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
    ( &(const bvalue[23]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(),
    /* K2   */  be_nested_str_weak(type),
    /* K3   */  be_const_int(1),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(value),
    /* K6   */  be_nested_str_weak(next),
    /* K7   */  be_nested_str_weak(error),
    /* K8   */  be_nested_str_weak(Expected_X20function_X20name),
    /* K9   */  be_nested_str_weak(nil),
    /* K10  */  be_nested_str_weak(symbol_table),
    /* K11  */  be_nested_str_weak(get),
    /* K12  */  be_nested_str_weak(process_function_arguments),
    /* K13  */  be_nested_str_weak(_X25s_X28_X25s_X29),
    /* K14  */  be_nested_str_weak(get_reference),
    /* K15  */  be_nested_str_weak(log),
    /* K16  */  be_nested_str_weak(process_log_call),
    /* K17  */  be_nested_str_weak(CONTEXT_EXPRESSION),
    /* K18  */  be_nested_str_weak(engine_X2C_X20_X25s),
    /* K19  */  be_nested_str_weak(engine),
    /* K20  */  be_nested_str_weak(_X25s_template_X28_X25s_X29),
    /* K21  */  be_nested_str_weak(animation_X2E_X25s_X28engine_X2C_X20_X25s_X29),
    /* K22  */  be_nested_str_weak(animation_X2E_X25s_X28engine_X29),
    }),
    be_str_weak(process_function_call),
    &be_const_str_solidified,
    ( &(const binstruction[92]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x7C080200,  //  0001  CALL	R2	1
      0x580C0001,  //  0002  LDCONST	R3	K1
      0x4C100000,  //  0003  LDNIL	R4
      0x20100404,  //  0004  NE	R4	R2	R4
      0x78120009,  //  0005  JMPF	R4	#0010
      0x88100502,  //  0006  GETMBR	R4	R2	K2
      0x1C100903,  //  0007  EQ	R4	R4	K3
      0x74120002,  //  0008  JMPT	R4	#000C
      0x88100502,  //  0009  GETMBR	R4	R2	K2
      0x1C100904,  //  000A  EQ	R4	R4	K4
      0x78120003,  //  000B  JMPF	R4	#0010
      0x880C0505,  //  000C  GETMBR	R3	R2	K5
      0x8C100106,  //  000D  GETMET	R4	R0	K6
      0x7C100200,  //  000E  CALL	R4	1
      0x70020003,  //  000F  JMP		#0014
      0x8C100107,  //  0010  GETMET	R4	R0	K7
      0x58180008,  //  0011  LDCONST	R6	K8
      0x7C100400,  //  0012  CALL	R4	2
      0x80061200,  //  0013  RET	1	K9
      0x8810010A,  //  0014  GETMBR	R4	R0	K10
      0x8C10090B,  //  0015  GETMET	R4	R4	K11
      0x5C180600,  //  0016  MOVE	R6	R3
      0x7C100400,  //  0017  CALL	R4	2
      0x4C140000,  //  0018  LDNIL	R5
      0x20140805,  //  0019  NE	R5	R4	R5
      0x7816000D,  //  001A  JMPF	R5	#0029
      0x88140902,  //  001B  GETMBR	R5	R4	K2
      0x541A0003,  //  001C  LDINT	R6	4
      0x1C140A06,  //  001D  EQ	R5	R5	R6
      0x78160009,  //  001E  JMPF	R5	#0029
      0x8C14010C,  //  001F  GETMET	R5	R0	K12
      0x501C0000,  //  0020  LDBOOL	R7	0	0
      0x7C140400,  //  0021  CALL	R5	2
      0x60180018,  //  0022  GETGBL	R6	G24
      0x581C000D,  //  0023  LDCONST	R7	K13
      0x8C20090E,  //  0024  GETMET	R8	R4	K14
      0x7C200200,  //  0025  CALL	R8	1
      0x5C240A00,  //  0026  MOVE	R9	R5
      0x7C180600,  //  0027  CALL	R6	3
      0x80040C00,  //  0028  RET	1	R6
      0x1C14070F,  //  0029  EQ	R5	R3	K15
      0x78160008,  //  002A  JMPF	R5	#0034
      0x8C14010C,  //  002B  GETMET	R5	R0	K12
      0x501C0000,  //  002C  LDBOOL	R7	0	0
      0x7C140400,  //  002D  CALL	R5	2
      0x8C180110,  //  002E  GETMET	R6	R0	K16
      0x5C200A00,  //  002F  MOVE	R8	R5
      0x88240111,  //  0030  GETMBR	R9	R0	K17
      0x58280001,  //  0031  LDCONST	R10	K1
      0x7C180800,  //  0032  CALL	R6	4
      0x80040C00,  //  0033  RET	1	R6
      0x8C14010C,  //  0034  GETMET	R5	R0	K12
      0x501C0000,  //  0035  LDBOOL	R7	0	0
      0x7C140400,  //  0036  CALL	R5	2
      0x4C180000,  //  0037  LDNIL	R6
      0x20180806,  //  0038  NE	R6	R4	R6
      0x781A0012,  //  0039  JMPF	R6	#004D
      0x88180902,  //  003A  GETMBR	R6	R4	K2
      0x541E000D,  //  003B  LDINT	R7	14
      0x1C180C07,  //  003C  EQ	R6	R6	R7
      0x781A000E,  //  003D  JMPF	R6	#004D
      0x20180B01,  //  003E  NE	R6	R5	K1
      0x781A0004,  //  003F  JMPF	R6	#0045
      0x60180018,  //  0040  GETGBL	R6	G24
      0x581C0012,  //  0041  LDCONST	R7	K18
      0x5C200A00,  //  0042  MOVE	R8	R5
      0x7C180400,  //  0043  CALL	R6	2
      0x70020000,  //  0044  JMP		#0046
      0x58180013,  //  0045  LDCONST	R6	K19
      0x601C0018,  //  0046  GETGBL	R7	G24
      0x58200014,  //  0047  LDCONST	R8	K20
      0x5C240600,  //  0048  MOVE	R9	R3
      0x5C280C00,  //  0049  MOVE	R10	R6
      0x7C1C0600,  //  004A  CALL	R7	3
      0x80040E00,  //  004B  RET	1	R7
      0x7002000D,  //  004C  JMP		#005B
      0x20180B01,  //  004D  NE	R6	R5	K1
      0x781A0006,  //  004E  JMPF	R6	#0056
      0x60180018,  //  004F  GETGBL	R6	G24
      0x581C0015,  //  0050  LDCONST	R7	K21
      0x5C200600,  //  0051  MOVE	R8	R3
      0x5C240A00,  //  0052  MOVE	R9	R5
      0x7C180600,  //  0053  CALL	R6	3
      0x80040C00,  //  0054  RET	1	R6
      0x70020004,  //  0055  JMP		#005B
      0x60180018,  //  0056  GETGBL	R6	G24
      0x581C0016,  //  0057  LDCONST	R7	K22
      0x5C200600,  //  0058  MOVE	R8	R3
      0x7C180400,  //  0059  CALL	R6	2
      0x80040C00,  //  005A  RET	1	R6
      0x80000000,  //  005B  RET	0
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
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(symbol_table),
    /* K1   */  be_nested_str_weak(create_animation),
    /* K2   */  be_nested_str_weak(create_color),
    /* K3   */  be_nested_str_weak(create_value_provider),
    /* K4   */  be_const_int(2),
    /* K5   */  be_nested_str_weak(create_palette),
    /* K6   */  be_nested_str_weak(create_sequence),
    /* K7   */  be_nested_str_weak(create_template),
    /* K8   */  be_nested_str_weak(create_variable),
    }),
    be_str_weak(_create_symbol_by_return_type),
    &be_const_str_solidified,
    ( &(const binstruction[64]) {  /* code */
      0x54120008,  //  0000  LDINT	R4	9
      0x1C100404,  //  0001  EQ	R4	R2	R4
      0x78120006,  //  0002  JMPF	R4	#000A
      0x88100100,  //  0003  GETMBR	R4	R0	K0
      0x8C100901,  //  0004  GETMET	R4	R4	K1
      0x5C180200,  //  0005  MOVE	R6	R1
      0x5C1C0600,  //  0006  MOVE	R7	R3
      0x7C100600,  //  0007  CALL	R4	3
      0x80040800,  //  0008  RET	1	R4
      0x70020034,  //  0009  JMP		#003F
      0x5412000A,  //  000A  LDINT	R4	11
      0x1C100404,  //  000B  EQ	R4	R2	R4
      0x78120006,  //  000C  JMPF	R4	#0014
      0x88100100,  //  000D  GETMBR	R4	R0	K0
      0x8C100902,  //  000E  GETMET	R4	R4	K2
      0x5C180200,  //  000F  MOVE	R6	R1
      0x5C1C0600,  //  0010  MOVE	R7	R3
      0x7C100600,  //  0011  CALL	R4	3
      0x80040800,  //  0012  RET	1	R4
      0x7002002A,  //  0013  JMP		#003F
      0x54120006,  //  0014  LDINT	R4	7
      0x1C100404,  //  0015  EQ	R4	R2	R4
      0x78120006,  //  0016  JMPF	R4	#001E
      0x88100100,  //  0017  GETMBR	R4	R0	K0
      0x8C100903,  //  0018  GETMET	R4	R4	K3
      0x5C180200,  //  0019  MOVE	R6	R1
      0x5C1C0600,  //  001A  MOVE	R7	R3
      0x7C100600,  //  001B  CALL	R4	3
      0x80040800,  //  001C  RET	1	R4
      0x70020020,  //  001D  JMP		#003F
      0x1C100504,  //  001E  EQ	R4	R2	K4
      0x78120006,  //  001F  JMPF	R4	#0027
      0x88100100,  //  0020  GETMBR	R4	R0	K0
      0x8C100905,  //  0021  GETMET	R4	R4	K5
      0x5C180200,  //  0022  MOVE	R6	R1
      0x5C1C0600,  //  0023  MOVE	R7	R3
      0x7C100600,  //  0024  CALL	R4	3
      0x80040800,  //  0025  RET	1	R4
      0x70020017,  //  0026  JMP		#003F
      0x5412000C,  //  0027  LDINT	R4	13
      0x1C100404,  //  0028  EQ	R4	R2	R4
      0x78120005,  //  0029  JMPF	R4	#0030
      0x88100100,  //  002A  GETMBR	R4	R0	K0
      0x8C100906,  //  002B  GETMET	R4	R4	K6
      0x5C180200,  //  002C  MOVE	R6	R1
      0x7C100400,  //  002D  CALL	R4	2
      0x80040800,  //  002E  RET	1	R4
      0x7002000E,  //  002F  JMP		#003F
      0x5412000D,  //  0030  LDINT	R4	14
      0x1C100404,  //  0031  EQ	R4	R2	R4
      0x78120006,  //  0032  JMPF	R4	#003A
      0x88100100,  //  0033  GETMBR	R4	R0	K0
      0x8C100907,  //  0034  GETMET	R4	R4	K7
      0x5C180200,  //  0035  MOVE	R6	R1
      0x4C1C0000,  //  0036  LDNIL	R7
      0x7C100600,  //  0037  CALL	R4	3
      0x80040800,  //  0038  RET	1	R4
      0x70020004,  //  0039  JMP		#003F
      0x88100100,  //  003A  GETMBR	R4	R0	K0
      0x8C100908,  //  003B  GETMET	R4	R4	K8
      0x5C180200,  //  003C  MOVE	R6	R1
      0x7C100400,  //  003D  CALL	R4	2
      0x80040800,  //  003E  RET	1	R4
      0x80000000,  //  003F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _unwrap_resolve
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__unwrap_resolve,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(animation_X2Eresolve_X28),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(_X29),
    /* K5   */  be_const_int(1),
    /* K6   */  be_nested_str_weak(_is_valid_identifier),
    }),
    be_str_weak(_unwrap_resolve),
    &be_const_str_solidified,
    ( &(const binstruction[33]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0501,  //  0001  GETMET	R3	R2	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x58180002,  //  0003  LDCONST	R6	K2
      0x7C0C0600,  //  0004  CALL	R3	3
      0x1C0C0703,  //  0005  EQ	R3	R3	K3
      0x780E0017,  //  0006  JMPF	R3	#001F
      0x540DFFFE,  //  0007  LDINT	R3	-1
      0x940C0203,  //  0008  GETIDX	R3	R1	R3
      0x1C0C0704,  //  0009  EQ	R3	R3	K4
      0x780E0013,  //  000A  JMPF	R3	#001F
      0x600C000C,  //  000B  GETGBL	R3	G12
      0x58100002,  //  000C  LDCONST	R4	K2
      0x7C0C0200,  //  000D  CALL	R3	1
      0x6010000C,  //  000E  GETGBL	R4	G12
      0x5C140200,  //  000F  MOVE	R5	R1
      0x7C100200,  //  0010  CALL	R4	1
      0x04100905,  //  0011  SUB	R4	R4	K5
      0x04140905,  //  0012  SUB	R5	R4	K5
      0x40140605,  //  0013  CONNECT	R5	R3	R5
      0x94140205,  //  0014  GETIDX	R5	R1	R5
      0x6018000C,  //  0015  GETGBL	R6	G12
      0x5C1C0A00,  //  0016  MOVE	R7	R5
      0x7C180200,  //  0017  CALL	R6	1
      0x24180D03,  //  0018  GT	R6	R6	K3
      0x781A0004,  //  0019  JMPF	R6	#001F
      0x8C180106,  //  001A  GETMET	R6	R0	K6
      0x5C200A00,  //  001B  MOVE	R8	R5
      0x7C180400,  //  001C  CALL	R6	2
      0x781A0000,  //  001D  JMPF	R6	#001F
      0x80040A00,  //  001E  RET	1	R5
      0x4C0C0000,  //  001F  LDNIL	R3
      0x80040600,  //  0020  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _is_valid_identifier
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__is_valid_identifier,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_weak(a),
    /* K3   */  be_nested_str_weak(z),
    /* K4   */  be_nested_str_weak(A),
    /* K5   */  be_nested_str_weak(Z),
    /* K6   */  be_nested_str_weak(_),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str_weak(0),
    /* K9   */  be_nested_str_weak(9),
    /* K10  */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(_is_valid_identifier),
    &be_const_str_solidified,
    ( &(const binstruction[61]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x600C000C,  //  0001  GETGBL	R3	G12
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x1C0C0701,  //  0004  EQ	R3	R3	K1
      0x780E0001,  //  0005  JMPF	R3	#0008
      0x500C0000,  //  0006  LDBOOL	R3	0	0
      0x80040600,  //  0007  RET	1	R3
      0x940C0301,  //  0008  GETIDX	R3	R1	K1
      0x28100702,  //  0009  GE	R4	R3	K2
      0x78120001,  //  000A  JMPF	R4	#000D
      0x18100703,  //  000B  LE	R4	R3	K3
      0x74120006,  //  000C  JMPT	R4	#0014
      0x28100704,  //  000D  GE	R4	R3	K4
      0x78120001,  //  000E  JMPF	R4	#0011
      0x18100705,  //  000F  LE	R4	R3	K5
      0x74120002,  //  0010  JMPT	R4	#0014
      0x1C100706,  //  0011  EQ	R4	R3	K6
      0x74120000,  //  0012  JMPT	R4	#0014
      0x50100001,  //  0013  LDBOOL	R4	0	1
      0x50100200,  //  0014  LDBOOL	R4	1	0
      0x74120001,  //  0015  JMPT	R4	#0018
      0x50100000,  //  0016  LDBOOL	R4	0	0
      0x80040800,  //  0017  RET	1	R4
      0x60100010,  //  0018  GETGBL	R4	G16
      0x6014000C,  //  0019  GETGBL	R5	G12
      0x5C180200,  //  001A  MOVE	R6	R1
      0x7C140200,  //  001B  CALL	R5	1
      0x04140B07,  //  001C  SUB	R5	R5	K7
      0x40160E05,  //  001D  CONNECT	R5	K7	R5
      0x7C100200,  //  001E  CALL	R4	1
      0xA8020017,  //  001F  EXBLK	0	#0038
      0x5C140800,  //  0020  MOVE	R5	R4
      0x7C140000,  //  0021  CALL	R5	0
      0x94180205,  //  0022  GETIDX	R6	R1	R5
      0x281C0D02,  //  0023  GE	R7	R6	K2
      0x781E0001,  //  0024  JMPF	R7	#0027
      0x181C0D03,  //  0025  LE	R7	R6	K3
      0x741E000A,  //  0026  JMPT	R7	#0032
      0x281C0D04,  //  0027  GE	R7	R6	K4
      0x781E0001,  //  0028  JMPF	R7	#002B
      0x181C0D05,  //  0029  LE	R7	R6	K5
      0x741E0006,  //  002A  JMPT	R7	#0032
      0x281C0D08,  //  002B  GE	R7	R6	K8
      0x781E0001,  //  002C  JMPF	R7	#002F
      0x181C0D09,  //  002D  LE	R7	R6	K9
      0x741E0002,  //  002E  JMPT	R7	#0032
      0x1C1C0D06,  //  002F  EQ	R7	R6	K6
      0x741E0000,  //  0030  JMPT	R7	#0032
      0x501C0001,  //  0031  LDBOOL	R7	0	1
      0x501C0200,  //  0032  LDBOOL	R7	1	0
      0x741E0002,  //  0033  JMPT	R7	#0037
      0x501C0000,  //  0034  LDBOOL	R7	0	0
      0xA8040001,  //  0035  EXBLK	1	1
      0x80040E00,  //  0036  RET	1	R7
      0x7001FFE7,  //  0037  JMP		#0020
      0x5810000A,  //  0038  LDCONST	R4	K10
      0xAC100200,  //  0039  CATCH	R4	1	0
      0xB0080000,  //  003A  RAISE	2	R0	R0
      0x50100200,  //  003B  LDBOOL	R4	1	0
      0x80040800,  //  003C  RET	1	R4
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
    ( &(const bvalue[20]) {     /* constants */
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
    /* K11  */  be_nested_str_weak(next),
    /* K12  */  be_nested_str_weak(error),
    /* K13  */  be_nested_str_weak(Expected_X20_X27_X2C_X27_X20or_X20_X27_X29_X27_X20in_X20function_X20arguments),
    /* K14  */  be_nested_str_weak(expect_right_paren),
    /* K15  */  be_nested_str_weak(),
    /* K16  */  be_const_int(0),
    /* K17  */  be_const_int(1),
    /* K18  */  be_nested_str_weak(_X2C_X20),
    /* K19  */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(process_function_arguments),
    &be_const_str_solidified,
    ( &(const binstruction[81]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x7C080200,  //  0001  CALL	R2	1
      0x60080012,  //  0002  GETGBL	R2	G18
      0x7C080000,  //  0003  CALL	R2	0
      0x8C0C0101,  //  0004  GETMET	R3	R0	K1
      0x7C0C0200,  //  0005  CALL	R3	1
      0x740E0032,  //  0006  JMPT	R3	#003A
      0x8C0C0102,  //  0007  GETMET	R3	R0	K2
      0x7C0C0200,  //  0008  CALL	R3	1
      0x740E002F,  //  0009  JMPT	R3	#003A
      0x8C0C0103,  //  000A  GETMET	R3	R0	K3
      0x7C0C0200,  //  000B  CALL	R3	1
      0x8C0C0102,  //  000C  GETMET	R3	R0	K2
      0x7C0C0200,  //  000D  CALL	R3	1
      0x780E0000,  //  000E  JMPF	R3	#0010
      0x70020029,  //  000F  JMP		#003A
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
      0x7812000A,  //  0026  JMPF	R4	#0032
      0x8C100109,  //  0027  GETMET	R4	R0	K9
      0x7C100200,  //  0028  CALL	R4	1
      0x8810090A,  //  0029  GETMBR	R4	R4	K10
      0x5416001D,  //  002A  LDINT	R5	30
      0x1C100805,  //  002B  EQ	R4	R4	R5
      0x78120004,  //  002C  JMPF	R4	#0032
      0x8C10010B,  //  002D  GETMET	R4	R0	K11
      0x7C100200,  //  002E  CALL	R4	1
      0x8C100103,  //  002F  GETMET	R4	R0	K3
      0x7C100200,  //  0030  CALL	R4	1
      0x70020006,  //  0031  JMP		#0039
      0x8C100102,  //  0032  GETMET	R4	R0	K2
      0x7C100200,  //  0033  CALL	R4	1
      0x74120003,  //  0034  JMPT	R4	#0039
      0x8C10010C,  //  0035  GETMET	R4	R0	K12
      0x5818000D,  //  0036  LDCONST	R6	K13
      0x7C100400,  //  0037  CALL	R4	2
      0x70020000,  //  0038  JMP		#003A
      0x7001FFC9,  //  0039  JMP		#0004
      0x8C0C010E,  //  003A  GETMET	R3	R0	K14
      0x7C0C0200,  //  003B  CALL	R3	1
      0x580C000F,  //  003C  LDCONST	R3	K15
      0x60100010,  //  003D  GETGBL	R4	G16
      0x6014000C,  //  003E  GETGBL	R5	G12
      0x5C180400,  //  003F  MOVE	R6	R2
      0x7C140200,  //  0040  CALL	R5	1
      0x04140B11,  //  0041  SUB	R5	R5	K17
      0x40162005,  //  0042  CONNECT	R5	K16	R5
      0x7C100200,  //  0043  CALL	R4	1
      0xA8020007,  //  0044  EXBLK	0	#004D
      0x5C140800,  //  0045  MOVE	R5	R4
      0x7C140000,  //  0046  CALL	R5	0
      0x24180B10,  //  0047  GT	R6	R5	K16
      0x781A0000,  //  0048  JMPF	R6	#004A
      0x000C0712,  //  0049  ADD	R3	R3	K18
      0x94180405,  //  004A  GETIDX	R6	R2	R5
      0x000C0606,  //  004B  ADD	R3	R3	R6
      0x7001FFF7,  //  004C  JMP		#0045
      0x58100013,  //  004D  LDCONST	R4	K19
      0xAC100200,  //  004E  CATCH	R4	1	0
      0xB0080000,  //  004F  RAISE	2	R0	R0
      0x80040600,  //  0050  RET	1	R3
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
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(),
    /* K1   */  be_nested_str_weak(_create_instance_for_validation),
    /* K2   */  be_nested_str_weak(at_end),
    /* K3   */  be_nested_str_weak(check_right_paren),
    /* K4   */  be_nested_str_weak(skip_whitespace_including_newlines),
    /* K5   */  be_nested_str_weak(expect_identifier),
    /* K6   */  be_nested_str_weak(_validate_single_parameter),
    /* K7   */  be_nested_str_weak(expect_assign),
    /* K8   */  be_nested_str_weak(process_value),
    /* K9   */  be_nested_str_weak(CONTEXT_VARIABLE),
    /* K10  */  be_nested_str_weak(collect_inline_comment),
    /* K11  */  be_nested_str_weak(expr),
    /* K12  */  be_nested_str_weak(current),
    /* K13  */  be_nested_str_weak(type),
    /* K14  */  be_nested_str_weak(next),
    /* K15  */  be_nested_str_weak(error),
    /* K16  */  be_nested_str_weak(Expected_X20_X27_X2C_X27_X20or_X20_X27_X29_X27_X20in_X20function_X20arguments),
    }),
    be_str_weak(_process_parameters_core),
    &be_const_str_solidified,
    ( &(const binstruction[102]) {  /* code */
      0x4C100000,  //  0000  LDNIL	R4
      0x5C140200,  //  0001  MOVE	R5	R1
      0x20180B00,  //  0002  NE	R6	R5	K0
      0x781A0003,  //  0003  JMPF	R6	#0008
      0x8C180101,  //  0004  GETMET	R6	R0	K1
      0x5C200A00,  //  0005  MOVE	R8	R5
      0x7C180400,  //  0006  CALL	R6	2
      0x5C100C00,  //  0007  MOVE	R4	R6
      0x8C180102,  //  0008  GETMET	R6	R0	K2
      0x7C180200,  //  0009  CALL	R6	1
      0x741A0059,  //  000A  JMPT	R6	#0065
      0x8C180103,  //  000B  GETMET	R6	R0	K3
      0x7C180200,  //  000C  CALL	R6	1
      0x741A0056,  //  000D  JMPT	R6	#0065
      0x8C180104,  //  000E  GETMET	R6	R0	K4
      0x7C180200,  //  000F  CALL	R6	1
      0x8C180103,  //  0010  GETMET	R6	R0	K3
      0x7C180200,  //  0011  CALL	R6	1
      0x781A0000,  //  0012  JMPF	R6	#0014
      0x70020050,  //  0013  JMP		#0065
      0x8C180105,  //  0014  GETMET	R6	R0	K5
      0x7C180200,  //  0015  CALL	R6	1
      0x4C1C0000,  //  0016  LDNIL	R7
      0x201C0807,  //  0017  NE	R7	R4	R7
      0x781E0006,  //  0018  JMPF	R7	#0020
      0x201C0B00,  //  0019  NE	R7	R5	K0
      0x781E0004,  //  001A  JMPF	R7	#0020
      0x8C1C0106,  //  001B  GETMET	R7	R0	K6
      0x5C240A00,  //  001C  MOVE	R9	R5
      0x5C280C00,  //  001D  MOVE	R10	R6
      0x5C2C0800,  //  001E  MOVE	R11	R4
      0x7C1C0800,  //  001F  CALL	R7	4
      0x8C1C0107,  //  0020  GETMET	R7	R0	K7
      0x7C1C0200,  //  0021  CALL	R7	1
      0x8C1C0108,  //  0022  GETMET	R7	R0	K8
      0x88240109,  //  0023  GETMBR	R9	R0	K9
      0x7C1C0400,  //  0024  CALL	R7	2
      0x8C20010A,  //  0025  GETMET	R8	R0	K10
      0x7C200200,  //  0026  CALL	R8	1
      0x5C240600,  //  0027  MOVE	R9	R3
      0x5C280C00,  //  0028  MOVE	R10	R6
      0x882C0F0B,  //  0029  GETMBR	R11	R7	K11
      0x5C301000,  //  002A  MOVE	R12	R8
      0x7C240600,  //  002B  CALL	R9	3
      0x8C240102,  //  002C  GETMET	R9	R0	K2
      0x7C240200,  //  002D  CALL	R9	1
      0x7426000D,  //  002E  JMPT	R9	#003D
      0x8C24010C,  //  002F  GETMET	R9	R0	K12
      0x7C240200,  //  0030  CALL	R9	1
      0x4C280000,  //  0031  LDNIL	R10
      0x2028120A,  //  0032  NE	R10	R9	R10
      0x782A0006,  //  0033  JMPF	R10	#003B
      0x8828130D,  //  0034  GETMBR	R10	R9	K13
      0x542E0024,  //  0035  LDINT	R11	37
      0x1C28140B,  //  0036  EQ	R10	R10	R11
      0x782A0002,  //  0037  JMPF	R10	#003B
      0x8C28010E,  //  0038  GETMET	R10	R0	K14
      0x7C280200,  //  0039  CALL	R10	1
      0x70020000,  //  003A  JMP		#003C
      0x70020000,  //  003B  JMP		#003D
      0x7001FFEE,  //  003C  JMP		#002C
      0x8C24010C,  //  003D  GETMET	R9	R0	K12
      0x7C240200,  //  003E  CALL	R9	1
      0x4C280000,  //  003F  LDNIL	R10
      0x2024120A,  //  0040  NE	R9	R9	R10
      0x7826000A,  //  0041  JMPF	R9	#004D
      0x8C24010C,  //  0042  GETMET	R9	R0	K12
      0x7C240200,  //  0043  CALL	R9	1
      0x8824130D,  //  0044  GETMBR	R9	R9	K13
      0x542A001D,  //  0045  LDINT	R10	30
      0x1C24120A,  //  0046  EQ	R9	R9	R10
      0x78260004,  //  0047  JMPF	R9	#004D
      0x8C24010E,  //  0048  GETMET	R9	R0	K14
      0x7C240200,  //  0049  CALL	R9	1
      0x8C240104,  //  004A  GETMET	R9	R0	K4
      0x7C240200,  //  004B  CALL	R9	1
      0x70020016,  //  004C  JMP		#0064
      0x8C24010C,  //  004D  GETMET	R9	R0	K12
      0x7C240200,  //  004E  CALL	R9	1
      0x4C280000,  //  004F  LDNIL	R10
      0x2024120A,  //  0050  NE	R9	R9	R10
      0x7826000A,  //  0051  JMPF	R9	#005D
      0x8C24010C,  //  0052  GETMET	R9	R0	K12
      0x7C240200,  //  0053  CALL	R9	1
      0x8824130D,  //  0054  GETMBR	R9	R9	K13
      0x542A0022,  //  0055  LDINT	R10	35
      0x1C24120A,  //  0056  EQ	R9	R9	R10
      0x78260004,  //  0057  JMPF	R9	#005D
      0x8C24010E,  //  0058  GETMET	R9	R0	K14
      0x7C240200,  //  0059  CALL	R9	1
      0x8C240104,  //  005A  GETMET	R9	R0	K4
      0x7C240200,  //  005B  CALL	R9	1
      0x70020006,  //  005C  JMP		#0064
      0x8C240103,  //  005D  GETMET	R9	R0	K3
      0x7C240200,  //  005E  CALL	R9	1
      0x74260003,  //  005F  JMPT	R9	#0064
      0x8C24010F,  //  0060  GETMET	R9	R0	K15
      0x582C0010,  //  0061  LDCONST	R11	K16
      0x7C240400,  //  0062  CALL	R9	2
      0x70020000,  //  0063  JMP		#0065
      0x7001FFA2,  //  0064  JMP		#0008
      0x80000000,  //  0065  RET	0
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
    ( &(const bvalue[29]) {     /* constants */
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
    /* K10  */  be_const_int(0),
    /* K11  */  be_nested_str_weak(value),
    /* K12  */  be_nested_str_weak(repeat),
    /* K13  */  be_nested_str_weak(forever),
    /* K14  */  be_nested_str_weak(_X2D1),
    /* K15  */  be_nested_str_weak(process_value),
    /* K16  */  be_nested_str_weak(CONTEXT_REPEAT_COUNT),
    /* K17  */  be_nested_str_weak(expect_keyword),
    /* K18  */  be_nested_str_weak(times),
    /* K19  */  be_nested_str_weak(expr),
    /* K20  */  be_const_int(2),
    /* K21  */  be_nested_str_weak(expect_left_brace),
    /* K22  */  be_nested_str_weak(add),
    /* K23  */  be_nested_str_weak(var_X20_X25s__X20_X3D_X20animation_X2ESequenceManager_X28engine_X2C_X20_X25s_X29),
    /* K24  */  be_nested_str_weak(at_end),
    /* K25  */  be_nested_str_weak(check_right_brace),
    /* K26  */  be_nested_str_weak(process_sequence_statement),
    /* K27  */  be_nested_str_weak(var_X20_X25s__X20_X3D_X20animation_X2ESequenceManager_X28engine_X29),
    /* K28  */  be_nested_str_weak(expect_right_brace),
    }),
    be_str_weak(process_sequence),
    &be_const_str_solidified,
    ( &(const binstruction[115]) {  /* code */
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
      0x78160027,  //  0016  JMPF	R5	#003F
      0x88140909,  //  0017  GETMBR	R5	R4	K9
      0x1C140B0A,  //  0018  EQ	R5	R5	K10
      0x78160024,  //  0019  JMPF	R5	#003F
      0x8814090B,  //  001A  GETMBR	R5	R4	K11
      0x1C140B0C,  //  001B  EQ	R5	R5	K12
      0x78160019,  //  001C  JMPF	R5	#0037
      0x50080200,  //  001D  LDBOOL	R2	1	0
      0x8C140100,  //  001E  GETMET	R5	R0	K0
      0x7C140200,  //  001F  CALL	R5	1
      0x8C140108,  //  0020  GETMET	R5	R0	K8
      0x7C140200,  //  0021  CALL	R5	1
      0x4C180000,  //  0022  LDNIL	R6
      0x20180A06,  //  0023  NE	R6	R5	R6
      0x781A0009,  //  0024  JMPF	R6	#002F
      0x88180B09,  //  0025  GETMBR	R6	R5	K9
      0x1C180D0A,  //  0026  EQ	R6	R6	K10
      0x781A0006,  //  0027  JMPF	R6	#002F
      0x88180B0B,  //  0028  GETMBR	R6	R5	K11
      0x1C180D0D,  //  0029  EQ	R6	R6	K13
      0x781A0003,  //  002A  JMPF	R6	#002F
      0x8C180100,  //  002B  GETMET	R6	R0	K0
      0x7C180200,  //  002C  CALL	R6	1
      0x580C000E,  //  002D  LDCONST	R3	K14
      0x70020006,  //  002E  JMP		#0036
      0x8C18010F,  //  002F  GETMET	R6	R0	K15
      0x88200110,  //  0030  GETMBR	R8	R0	K16
      0x7C180400,  //  0031  CALL	R6	2
      0x8C1C0111,  //  0032  GETMET	R7	R0	K17
      0x58240012,  //  0033  LDCONST	R9	K18
      0x7C1C0400,  //  0034  CALL	R7	2
      0x880C0D13,  //  0035  GETMBR	R3	R6	K19
      0x70020006,  //  0036  JMP		#003E
      0x8814090B,  //  0037  GETMBR	R5	R4	K11
      0x1C140B0D,  //  0038  EQ	R5	R5	K13
      0x78160003,  //  0039  JMPF	R5	#003E
      0x50080200,  //  003A  LDBOOL	R2	1	0
      0x8C140100,  //  003B  GETMET	R5	R0	K0
      0x7C140200,  //  003C  CALL	R5	1
      0x580C000E,  //  003D  LDCONST	R3	K14
      0x7002000D,  //  003E  JMP		#004D
      0x4C140000,  //  003F  LDNIL	R5
      0x20140805,  //  0040  NE	R5	R4	R5
      0x7816000A,  //  0041  JMPF	R5	#004D
      0x88140909,  //  0042  GETMBR	R5	R4	K9
      0x1C140B14,  //  0043  EQ	R5	R5	K20
      0x78160007,  //  0044  JMPF	R5	#004D
      0x50080200,  //  0045  LDBOOL	R2	1	0
      0x8C14010F,  //  0046  GETMET	R5	R0	K15
      0x881C0110,  //  0047  GETMBR	R7	R0	K16
      0x7C140400,  //  0048  CALL	R5	2
      0x8C180111,  //  0049  GETMET	R6	R0	K17
      0x58200012,  //  004A  LDCONST	R8	K18
      0x7C180400,  //  004B  CALL	R6	2
      0x880C0B13,  //  004C  GETMBR	R3	R5	K19
      0x8C140115,  //  004D  GETMET	R5	R0	K21
      0x7C140200,  //  004E  CALL	R5	1
      0x780A0010,  //  004F  JMPF	R2	#0061
      0x8C140116,  //  0050  GETMET	R5	R0	K22
      0x601C0018,  //  0051  GETGBL	R7	G24
      0x58200017,  //  0052  LDCONST	R8	K23
      0x5C240200,  //  0053  MOVE	R9	R1
      0x5C280600,  //  0054  MOVE	R10	R3
      0x7C1C0600,  //  0055  CALL	R7	3
      0x7C140400,  //  0056  CALL	R5	2
      0x8C140118,  //  0057  GETMET	R5	R0	K24
      0x7C140200,  //  0058  CALL	R5	1
      0x74160005,  //  0059  JMPT	R5	#0060
      0x8C140119,  //  005A  GETMET	R5	R0	K25
      0x7C140200,  //  005B  CALL	R5	1
      0x74160002,  //  005C  JMPT	R5	#0060
      0x8C14011A,  //  005D  GETMET	R5	R0	K26
      0x7C140200,  //  005E  CALL	R5	1
      0x7001FFF6,  //  005F  JMP		#0057
      0x7002000E,  //  0060  JMP		#0070
      0x8C140116,  //  0061  GETMET	R5	R0	K22
      0x601C0018,  //  0062  GETGBL	R7	G24
      0x5820001B,  //  0063  LDCONST	R8	K27
      0x5C240200,  //  0064  MOVE	R9	R1
      0x7C1C0400,  //  0065  CALL	R7	2
      0x7C140400,  //  0066  CALL	R5	2
      0x8C140118,  //  0067  GETMET	R5	R0	K24
      0x7C140200,  //  0068  CALL	R5	1
      0x74160005,  //  0069  JMPT	R5	#0070
      0x8C140119,  //  006A  GETMET	R5	R0	K25
      0x7C140200,  //  006B  CALL	R5	1
      0x74160002,  //  006C  JMPT	R5	#0070
      0x8C14011A,  //  006D  GETMET	R5	R0	K26
      0x7C140200,  //  006E  CALL	R5	1
      0x7001FFF6,  //  006F  JMP		#0067
      0x8C14011C,  //  0070  GETMET	R5	R0	K28
      0x7C140200,  //  0071  CALL	R5	1
      0x80000000,  //  0072  RET	0
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
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(at_end),
    /* K1   */  be_nested_str_weak(current),
    /* K2   */  be_nested_str_weak(type),
    /* K3   */  be_nested_str_weak(next),
    }),
    be_str_weak(skip_statement),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x7406000C,  //  0002  JMPT	R1	#0010
      0x8C040101,  //  0003  GETMET	R1	R0	K1
      0x7C040200,  //  0004  CALL	R1	1
      0x4C080000,  //  0005  LDNIL	R2
      0x1C080202,  //  0006  EQ	R2	R1	R2
      0x740A0003,  //  0007  JMPT	R2	#000C
      0x88080302,  //  0008  GETMBR	R2	R1	K2
      0x540E0022,  //  0009  LDINT	R3	35
      0x1C080403,  //  000A  EQ	R2	R2	R3
      0x780A0000,  //  000B  JMPF	R2	#000D
      0x70020002,  //  000C  JMP		#0010
      0x8C080103,  //  000D  GETMET	R2	R0	K3
      0x7C080200,  //  000E  CALL	R2	1
      0x7001FFEF,  //  000F  JMP		#0000
      0x80000000,  //  0010  RET	0
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
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(process_function_arguments),
    /* K3   */  be_nested_str_weak(),
    /* K4   */  be_nested_str_weak(engine_X2C_X20_X25s),
    /* K5   */  be_nested_str_weak(engine),
    /* K6   */  be_nested_str_weak(animation_X2Eget_user_function_X28_X27_X25s_X27_X29_X28_X25s_X29),
    /* K7   */  be_nested_str_weak(error),
    /* K8   */  be_nested_str_weak(User_X20functions_X20must_X20be_X20called_X20with_X20parentheses_X3A_X20user_X2Efunction_name_X28_X29),
    /* K9   */  be_nested_str_weak(nil),
    }),
    be_str_weak(_process_user_function_call),
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x7C080200,  //  0001  CALL	R2	1
      0x4C0C0000,  //  0002  LDNIL	R3
      0x20080403,  //  0003  NE	R2	R2	R3
      0x780A0017,  //  0004  JMPF	R2	#001D
      0x8C080100,  //  0005  GETMET	R2	R0	K0
      0x7C080200,  //  0006  CALL	R2	1
      0x88080501,  //  0007  GETMBR	R2	R2	K1
      0x540E0017,  //  0008  LDINT	R3	24
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x780A0011,  //  000A  JMPF	R2	#001D
      0x8C080102,  //  000B  GETMET	R2	R0	K2
      0x50100200,  //  000C  LDBOOL	R4	1	0
      0x7C080400,  //  000D  CALL	R2	2
      0x200C0503,  //  000E  NE	R3	R2	K3
      0x780E0004,  //  000F  JMPF	R3	#0015
      0x600C0018,  //  0010  GETGBL	R3	G24
      0x58100004,  //  0011  LDCONST	R4	K4
      0x5C140400,  //  0012  MOVE	R5	R2
      0x7C0C0400,  //  0013  CALL	R3	2
      0x70020000,  //  0014  JMP		#0016
      0x580C0005,  //  0015  LDCONST	R3	K5
      0x60100018,  //  0016  GETGBL	R4	G24
      0x58140006,  //  0017  LDCONST	R5	K6
      0x5C180200,  //  0018  MOVE	R6	R1
      0x5C1C0600,  //  0019  MOVE	R7	R3
      0x7C100600,  //  001A  CALL	R4	3
      0x80040800,  //  001B  RET	1	R4
      0x70020003,  //  001C  JMP		#0021
      0x8C080107,  //  001D  GETMET	R2	R0	K7
      0x58100008,  //  001E  LDCONST	R4	K8
      0x7C080400,  //  001F  CALL	R2	2
      0x80061200,  //  0020  RET	1	K9
      0x80000000,  //  0021  RET	0
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
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(),
    /* K1   */  be_nested_str_weak(has_warnings),
    /* K2   */  be_nested_str_weak(Compilation_X20warnings_X3A_X0A),
    /* K3   */  be_nested_str_weak(warnings),
    /* K4   */  be_nested_str_weak(_X20_X20),
    /* K5   */  be_nested_str_weak(_X0A),
    /* K6   */  be_nested_str_weak(stop_iteration),
    /* K7   */  be_nested_str_weak(No_X20compilation_X20warnings),
    }),
    be_str_weak(get_error_report),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x8C080101,  //  0001  GETMET	R2	R0	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x780A000D,  //  0003  JMPF	R2	#0012
      0x00040302,  //  0004  ADD	R1	R1	K2
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
      0x1C080300,  //  0012  EQ	R2	R1	K0
      0x780A0000,  //  0013  JMPF	R2	#0015
      0x80060E00,  //  0014  RET	1	K7
      0x80040200,  //  0015  RET	1	R1
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
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(next),
    /* K3   */  be_nested_str_weak(error),
    /* K4   */  be_nested_str_weak(Expected_X20_X27_X5B_X27),
    }),
    be_str_weak(expect_left_bracket),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0006,  //  0004  JMPF	R2	#000C
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0x540E001B,  //  0006  LDINT	R3	28
      0x1C080403,  //  0007  EQ	R2	R2	R3
      0x780A0002,  //  0008  JMPF	R2	#000C
      0x8C080102,  //  0009  GETMET	R2	R0	K2
      0x7C080200,  //  000A  CALL	R2	1
      0x70020002,  //  000B  JMP		#000F
      0x8C080103,  //  000C  GETMET	R2	R0	K3
      0x58100004,  //  000D  LDCONST	R4	K4
      0x7C080400,  //  000E  CALL	R2	2
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: SimpleDSLTranspiler
********************************************************************/
be_local_class(SimpleDSLTranspiler,
    8,
    NULL,
    be_nested_map(124,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(CONTEXT_COLOR_PROVIDER, -1), be_const_int(11) },
        { be_const_key_weak(convert_time_to_ms, -1), be_const_closure(class_SimpleDSLTranspiler_convert_time_to_ms_closure) },
        { be_const_key_weak(skip_whitespace_including_newlines, -1), be_const_closure(class_SimpleDSLTranspiler_skip_whitespace_including_newlines_closure) },
        { be_const_key_weak(expect_left_paren, -1), be_const_closure(class_SimpleDSLTranspiler_expect_left_paren_closure) },
        { be_const_key_weak(expect_left_brace, -1), be_const_closure(class_SimpleDSLTranspiler_expect_left_brace_closure) },
        { be_const_key_weak(transpile_template_body, -1), be_const_closure(class_SimpleDSLTranspiler_transpile_template_body_closure) },
        { be_const_key_weak(expect_right_paren, 113), be_const_closure(class_SimpleDSLTranspiler_expect_right_paren_closure) },
        { be_const_key_weak(expect_left_bracket, -1), be_const_closure(class_SimpleDSLTranspiler_expect_left_bracket_closure) },
        { be_const_key_weak(_split_function_arguments, 87), be_const_closure(class_SimpleDSLTranspiler__split_function_arguments_closure) },
        { be_const_key_weak(get_warnings, -1), be_const_closure(class_SimpleDSLTranspiler_get_warnings_closure) },
        { be_const_key_weak(CONTEXT_REPEAT_COUNT, -1), be_const_int(6) },
        { be_const_key_weak(process_wait_statement_fluent, 121), be_const_closure(class_SimpleDSLTranspiler_process_wait_statement_fluent_closure) },
        { be_const_key_weak(process_run, -1), be_const_closure(class_SimpleDSLTranspiler_process_run_closure) },
        { be_const_key_weak(CONTEXT_VARIABLE, 74), be_const_int(1) },
        { be_const_key_weak(get_error_report, 17), be_const_closure(class_SimpleDSLTranspiler_get_error_report_closure) },
        { be_const_key_weak(output, 58), be_const_var(1) },
        { be_const_key_weak(CONTEXT_PROPERTY, 45), be_const_int(5) },
        { be_const_key_weak(_process_user_function_call, -1), be_const_closure(class_SimpleDSLTranspiler__process_user_function_call_closure) },
        { be_const_key_weak(_create_instance_for_validation, -1), be_const_closure(class_SimpleDSLTranspiler__create_instance_for_validation_closure) },
        { be_const_key_weak(_validate_template_parameter_usage, -1), be_const_closure(class_SimpleDSLTranspiler__validate_template_parameter_usage_closure) },
        { be_const_key_weak(_determine_symbol_return_type, 99), be_const_closure(class_SimpleDSLTranspiler__determine_symbol_return_type_closure) },
        { be_const_key_weak(process_berry_code_block, -1), be_const_closure(class_SimpleDSLTranspiler_process_berry_code_block_closure) },
        { be_const_key_weak(_process_named_arguments_for_color_provider, -1), be_const_closure(class_SimpleDSLTranspiler__process_named_arguments_for_color_provider_closure) },
        { be_const_key_weak(join_output, -1), be_const_closure(class_SimpleDSLTranspiler_join_output_closure) },
        { be_const_key_weak(CONTEXT_ANIMATION, -1), be_const_int(3) },
        { be_const_key_weak(_validate_animation_factory_exists, 14), be_const_closure(class_SimpleDSLTranspiler__validate_animation_factory_exists_closure) },
        { be_const_key_weak(symbol_table, -1), be_const_var(5) },
        { be_const_key_weak(skip_statement, -1), be_const_closure(class_SimpleDSLTranspiler_skip_statement_closure) },
        { be_const_key_weak(warnings, -1), be_const_var(2) },
        { be_const_key_weak(process_unary_expression, -1), be_const_closure(class_SimpleDSLTranspiler_process_unary_expression_closure) },
        { be_const_key_weak(process_value, 12), be_const_closure(class_SimpleDSLTranspiler_process_value_closure) },
        { be_const_key_weak(process_restart_statement_fluent, -1), be_const_closure(class_SimpleDSLTranspiler_process_restart_statement_fluent_closure) },
        { be_const_key_weak(_process_parameters_core, 41), be_const_closure(class_SimpleDSLTranspiler__process_parameters_core_closure) },
        { be_const_key_weak(transpile, -1), be_const_closure(class_SimpleDSLTranspiler_transpile_closure) },
        { be_const_key_weak(process_function_arguments, -1), be_const_closure(class_SimpleDSLTranspiler_process_function_arguments_closure) },
        { be_const_key_weak(_process_named_arguments_for_animation, -1), be_const_closure(class_SimpleDSLTranspiler__process_named_arguments_for_animation_closure) },
        { be_const_key_weak(_validate_value_provider_reference, -1), be_const_closure(class_SimpleDSLTranspiler__validate_value_provider_reference_closure) },
        { be_const_key_weak(_is_valid_identifier, -1), be_const_closure(class_SimpleDSLTranspiler__is_valid_identifier_closure) },
        { be_const_key_weak(_unwrap_resolve, 103), be_const_closure(class_SimpleDSLTranspiler__unwrap_resolve_closure) },
        { be_const_key_weak(process_template, -1), be_const_closure(class_SimpleDSLTranspiler_process_template_closure) },
        { be_const_key_weak(skip_function_arguments, -1), be_const_closure(class_SimpleDSLTranspiler_skip_function_arguments_closure) },
        { be_const_key_weak(at_end, -1), be_const_closure(class_SimpleDSLTranspiler_at_end_closure) },
        { be_const_key_weak(process_nested_function_call, 93), be_const_closure(class_SimpleDSLTranspiler_process_nested_function_call_closure) },
        { be_const_key_weak(generate_default_strip_initialization, 46), be_const_closure(class_SimpleDSLTranspiler_generate_default_strip_initialization_closure) },
        { be_const_key_weak(check_right_bracket, 51), be_const_closure(class_SimpleDSLTranspiler_check_right_bracket_closure) },
        { be_const_key_weak(add, -1), be_const_closure(class_SimpleDSLTranspiler_add_closure) },
        { be_const_key_weak(process_event_handler, -1), be_const_closure(class_SimpleDSLTranspiler_process_event_handler_closure) },
        { be_const_key_weak(_validate_template_call_arguments, -1), be_const_closure(class_SimpleDSLTranspiler__validate_template_call_arguments_closure) },
        { be_const_key_weak(process_log_call, 67), be_const_closure(class_SimpleDSLTranspiler_process_log_call_closure) },
        { be_const_key_weak(process_standalone_log, 39), be_const_closure(class_SimpleDSLTranspiler_process_standalone_log_closure) },
        { be_const_key_weak(get_symbol_table_report, -1), be_const_closure(class_SimpleDSLTranspiler_get_symbol_table_report_closure) },
        { be_const_key_weak(process_repeat_statement_fluent, -1), be_const_closure(class_SimpleDSLTranspiler_process_repeat_statement_fluent_closure) },
        { be_const_key_weak(process_property_assignment, 94), be_const_closure(class_SimpleDSLTranspiler_process_property_assignment_closure) },
        { be_const_key_weak(process_sequence_assignment_fluent, -1), be_const_closure(class_SimpleDSLTranspiler_process_sequence_assignment_fluent_closure) },
        { be_const_key_weak(skip_whitespace, 59), be_const_closure(class_SimpleDSLTranspiler_skip_whitespace_closure) },
        { be_const_key_weak(get_named_color_value, -1), be_const_closure(class_SimpleDSLTranspiler_get_named_color_value_closure) },
        { be_const_key_weak(CONTEXT_TIME, 119), be_const_int(8) },
        { be_const_key_weak(error, -1), be_const_closure(class_SimpleDSLTranspiler_error_closure) },
        { be_const_key_weak(_add_typed_parameter_to_symbol_table, 28), be_const_closure(class_SimpleDSLTranspiler__add_typed_parameter_to_symbol_table_closure) },
        { be_const_key_weak(CONTEXT_GENERIC, -1), be_const_int(10) },
        { be_const_key_weak(expect_dot, 40), be_const_closure(class_SimpleDSLTranspiler_expect_dot_closure) },
        { be_const_key_weak(_process_simple_value_assignment, -1), be_const_closure(class_SimpleDSLTranspiler__process_simple_value_assignment_closure) },
        { be_const_key_weak(expect_identifier, 32), be_const_closure(class_SimpleDSLTranspiler_expect_identifier_closure) },
        { be_const_key_weak(validate_user_name, 105), be_const_closure(class_SimpleDSLTranspiler_validate_user_name_closure) },
        { be_const_key_weak(process_sequence_statement, -1), be_const_closure(class_SimpleDSLTranspiler_process_sequence_statement_closure) },
        { be_const_key_weak(can_use_as_identifier, 71), be_const_closure(class_SimpleDSLTranspiler_can_use_as_identifier_closure) },
        { be_const_key_weak(process_color, 95), be_const_closure(class_SimpleDSLTranspiler_process_color_closure) },
        { be_const_key_weak(strip_initialized, -1), be_const_var(4) },
        { be_const_key_weak(expect_comma, -1), be_const_closure(class_SimpleDSLTranspiler_expect_comma_closure) },
        { be_const_key_weak(expect_right_brace, 44), be_const_closure(class_SimpleDSLTranspiler_expect_right_brace_closure) },
        { be_const_key_weak(ExpressionResult, 101), be_const_class(be_class_ExpressionResult) },
        { be_const_key_weak(expect_assign, -1), be_const_closure(class_SimpleDSLTranspiler_expect_assign_closure) },
        { be_const_key_weak(expect_keyword, -1), be_const_closure(class_SimpleDSLTranspiler_expect_keyword_closure) },
        { be_const_key_weak(collect_inline_comment, -1), be_const_closure(class_SimpleDSLTranspiler_collect_inline_comment_closure) },
        { be_const_key_weak(CONTEXT_ARRAY_ELEMENT, 123), be_const_int(7) },
        { be_const_key_weak(process_time_value, -1), be_const_closure(class_SimpleDSLTranspiler_process_time_value_closure) },
        { be_const_key_weak(process_array_literal, -1), be_const_closure(class_SimpleDSLTranspiler_process_array_literal_closure) },
        { be_const_key_weak(has_warnings, -1), be_const_closure(class_SimpleDSLTranspiler_has_warnings_closure) },
        { be_const_key_weak(process_import, -1), be_const_closure(class_SimpleDSLTranspiler_process_import_closure) },
        { be_const_key_weak(_process_named_arguments_unified, -1), be_const_closure(class_SimpleDSLTranspiler__process_named_arguments_unified_closure) },
        { be_const_key_weak(convert_to_vrgb, 61), be_const_closure(class_SimpleDSLTranspiler_convert_to_vrgb_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_SimpleDSLTranspiler_init_closure) },
        { be_const_key_weak(process_event_parameters, 52), be_const_closure(class_SimpleDSLTranspiler_process_event_parameters_closure) },
        { be_const_key_weak(peek, -1), be_const_closure(class_SimpleDSLTranspiler_peek_closure) },
        { be_const_key_weak(_determine_function_return_type, -1), be_const_closure(class_SimpleDSLTranspiler__determine_function_return_type_closure) },
        { be_const_key_weak(check_right_paren, -1), be_const_closure(class_SimpleDSLTranspiler_check_right_paren_closure) },
        { be_const_key_weak(has_template_calls, -1), be_const_var(7) },
        { be_const_key_weak(process_sequence_assignment_generic, 86), be_const_closure(class_SimpleDSLTranspiler_process_sequence_assignment_generic_closure) },
        { be_const_key_weak(process_multiplicative_expression, -1), be_const_closure(class_SimpleDSLTranspiler_process_multiplicative_expression_closure) },
        { be_const_key_weak(run_statements, -1), be_const_var(3) },
        { be_const_key_weak(process_palette_color, 83), be_const_closure(class_SimpleDSLTranspiler_process_palette_color_closure) },
        { be_const_key_weak(warning, 81), be_const_closure(class_SimpleDSLTranspiler_warning_closure) },
        { be_const_key_weak(CONTEXT_COLOR, 75), be_const_int(2) },
        { be_const_key_weak(process_palette, -1), be_const_closure(class_SimpleDSLTranspiler_process_palette_closure) },
        { be_const_key_weak(generate_template_function_direct, -1), be_const_closure(class_SimpleDSLTranspiler_generate_template_function_direct_closure) },
        { be_const_key_weak(expect_right_bracket, -1), be_const_closure(class_SimpleDSLTranspiler_expect_right_bracket_closure) },
        { be_const_key_weak(next, 65), be_const_closure(class_SimpleDSLTranspiler_next_closure) },
        { be_const_key_weak(process_percentage_value, -1), be_const_closure(class_SimpleDSLTranspiler_process_percentage_value_closure) },
        { be_const_key_weak(check_right_brace, -1), be_const_closure(class_SimpleDSLTranspiler_check_right_brace_closure) },
        { be_const_key_weak(process_log_statement_fluent, 30), be_const_closure(class_SimpleDSLTranspiler_process_log_statement_fluent_closure) },
        { be_const_key_weak(convert_color, 56), be_const_closure(class_SimpleDSLTranspiler_convert_color_closure) },
        { be_const_key_weak(process_additive_expression, -1), be_const_closure(class_SimpleDSLTranspiler_process_additive_expression_closure) },
        { be_const_key_weak(_validate_color_provider_factory_exists, -1), be_const_closure(class_SimpleDSLTranspiler__validate_color_provider_factory_exists_closure) },
        { be_const_key_weak(process_play_statement_fluent, -1), be_const_closure(class_SimpleDSLTranspiler_process_play_statement_fluent_closure) },
        { be_const_key_weak(expect_colon, -1), be_const_closure(class_SimpleDSLTranspiler_expect_colon_closure) },
        { be_const_key_weak(CONTEXT_EXPRESSION, -1), be_const_int(9) },
        { be_const_key_weak(process_primary_expression, 43), be_const_closure(class_SimpleDSLTranspiler_process_primary_expression_closure) },
        { be_const_key_weak(_validate_template_parameter_name, -1), be_const_closure(class_SimpleDSLTranspiler__validate_template_parameter_name_closure) },
        { be_const_key_weak(process_function_call, -1), be_const_closure(class_SimpleDSLTranspiler_process_function_call_closure) },
        { be_const_key_weak(_create_symbol_by_return_type, -1), be_const_closure(class_SimpleDSLTranspiler__create_symbol_by_return_type_closure) },
        { be_const_key_weak(CONTEXT_ARGUMENT, 38), be_const_int(4) },
        { be_const_key_weak(pull_lexer, 37), be_const_var(0) },
        { be_const_key_weak(_validate_object_reference, 34), be_const_closure(class_SimpleDSLTranspiler__validate_object_reference_closure) },
        { be_const_key_weak(get_indent, -1), be_const_closure(class_SimpleDSLTranspiler_get_indent_closure) },
        { be_const_key_weak(process_sequence, -1), be_const_closure(class_SimpleDSLTranspiler_process_sequence_closure) },
        { be_const_key_weak(generate_engine_run, 7), be_const_closure(class_SimpleDSLTranspiler_generate_engine_run_closure) },
        { be_const_key_weak(process_statement, 27), be_const_closure(class_SimpleDSLTranspiler_process_statement_closure) },
        { be_const_key_weak(process_set, 24), be_const_closure(class_SimpleDSLTranspiler_process_set_closure) },
        { be_const_key_weak(current, 18), be_const_closure(class_SimpleDSLTranspiler_current_closure) },
        { be_const_key_weak(expect_number, -1), be_const_closure(class_SimpleDSLTranspiler_expect_number_closure) },
        { be_const_key_weak(_validate_template_parameter_type, 15), be_const_closure(class_SimpleDSLTranspiler__validate_template_parameter_type_closure) },
        { be_const_key_weak(process_animation, -1), be_const_closure(class_SimpleDSLTranspiler_process_animation_closure) },
        { be_const_key_weak(indent_level, -1), be_const_var(6) },
        { be_const_key_weak(_validate_single_parameter, -1), be_const_closure(class_SimpleDSLTranspiler__validate_single_parameter_closure) },
    })),
    be_str_weak(SimpleDSLTranspiler)
);
// compact class 'AnimationWebUI' ktab size: 62, total: 73 (saved 88 bytes)
static const bvalue be_ktab_class_AnimationWebUI[62] = {
  /* K0   */  be_nested_str_weak(webserver),
  /* K1   */  be_nested_str_weak(content_start),
  /* K2   */  be_nested_str_weak(Berry_X20Animation_X20Framework),
  /* K3   */  be_nested_str_weak(content_send_style),
  /* K4   */  be_nested_str_weak(content_send),
  /* K5   */  be_nested_str_long(_X3Cstyle_X3E_X2Eanim_X2Dcontainer_X7Bmin_X2Dwidth_X3A350px_X3Bmargin_X3A0_X20auto_X3Bpadding_X3A10px_X3Bwidth_X3A100_X25_X3Bmax_X2Dwidth_X3Anone_X3B_X7Dbody_X20_X3E_X20div_X7Bwidth_X3Acalc_X28100_X25_X20_X2D_X2020px_X29_X20_X21important_X3Bmax_X2Dwidth_X3A1200px_X20_X21important_X3Bdisplay_X3Ablock_X20_X21important_X3Bbox_X2Dsizing_X3Aborder_X2Dbox_X20_X21important_X3B_X7D_X2Eanim_X2Deditor_X7Bwidth_X3A100_X25_X3Bmin_X2Dheight_X3A300px_X3Bfont_X2Dfamily_X3Amonospace_X3Bfont_X2Dsize_X3A12px_X3Bborder_X3A1px_X20solid_X20var_X28_X2D_X2Dc_frm_X29_X3Bpadding_X3A8px_X3Bbackground_X3Avar_X28_X2D_X2Dc_intxt_X29_X3Bcolor_X3A_X23b19cd9_X3Bbox_X2Dsizing_X3Aborder_X2Dbox_X3B_X7D_X2Eanim_X2Doutput_X7Bwidth_X3A100_X25_X3Bmin_X2Dheight_X3A200px_X3Bfont_X2Dfamily_X3Amonospace_X3Bfont_X2Dsize_X3A11px_X3Bborder_X3A1px_X20solid_X20var_X28_X2D_X2Dc_frm_X29_X3Bpadding_X3A8px_X3Bbackground_X3Avar_X28_X2D_X2Dc_intxt_X29_X3Bcolor_X3A_X23fb1_X3Bbox_X2Dsizing_X3Aborder_X2Dbox_X3B_X7D_X2Eanim_X2Derror_X7Bcolor_X3Avar_X28_X2D_X2Dc_btnrst_X29_X3Bbackground_X3A_X23ffe6e6_X3Bpadding_X3A8px_X3Bborder_X3A1px_X20solid_X20var_X28_X2D_X2Dc_btnrst_X29_X3Bmargin_X3A5px_X200_X3B_X7D_X2Eanim_X2Dsuccess_X7Bcolor_X3Avar_X28_X2D_X2Dc_btnsv_X29_X3Bbackground_X3A_X23e6ffe6_X3Bpadding_X3A8px_X3Bborder_X3A1px_X20solid_X20var_X28_X2D_X2Dc_btnsv_X29_X3Bmargin_X3A5px_X200_X3B_X7Dbutton_X3Adisabled_X7Bopacity_X3A0_X2E5_X3Bcursor_X3Anot_X2Dallowed_X3B_X7D_X2Etextarea_X2Dcontainer_X7Bposition_X3Arelative_X3B_X7D_X2Ecopy_X2Dbtn_X7Bposition_X3Aabsolute_X3Btop_X3A8px_X3Bright_X3A0_X3Bwidth_X3A20px_X3Bheight_X3A20px_X3Bcursor_X3Apointer_X3Buser_X2Dselect_X3Anone_X3Btransition_X3Aall_X200_X2E2s_X3Bbackground_X3Atransparent_X3Bborder_X3Anone_X3B_X7D_X2Eanim_X2Deditor_X20_X2B_X20_X2Ecopy_X2Dbtn_X3A_X3Abefore_X2C_X2Eanim_X2Doutput_X20_X2B_X20_X2Ecopy_X2Dbtn_X3A_X3Abefore_X7Bcontent_X3A_X27_X27_X3Bposition_X3Aabsolute_X3Btop_X3A2px_X3Bleft_X3A2px_X3Bwidth_X3A10px_X3Bheight_X3A10px_X3Bborder_X2Dleft_X3A2px_X20solid_X20var_X28_X2D_X2Dc_txt_X29_X3Bborder_X2Dtop_X3A2px_X20solid_X20var_X28_X2D_X2Dc_txt_X29_X3Bbackground_X3Atransparent_X3B_X7D_X2Eanim_X2Deditor_X20_X2B_X20_X2Ecopy_X2Dbtn_X3A_X3Aafter_X2C_X2Eanim_X2Doutput_X20_X2B_X20_X2Ecopy_X2Dbtn_X3A_X3Aafter_X7Bcontent_X3A_X27_X27_X3Bposition_X3Aabsolute_X3Btop_X3A6px_X3Bleft_X3A6px_X3Bwidth_X3A10px_X3Bheight_X3A10px_X3Bborder_X3A2px_X20solid_X20var_X28_X2D_X2Dc_txt_X29_X3B_X7D_X2Ecopy_X2Dbtn_X3Ahover_X3A_X3Abefore_X2C_X2Ecopy_X2Dbtn_X3Ahover_X3A_X3Aafter_X7Bopacity_X3A0_X2E7_X3B_X7D_X2Ecopy_X2Dmessage_X7Bposition_X3Aabsolute_X3Btop_X3A35px_X3Bright_X3A8px_X3Bbackground_X3Avar_X28_X2D_X2Dc_intxt_X29_X3Bcolor_X3Awhite_X3Bpadding_X3A4px_X208px_X3Bborder_X2Dradius_X3A3px_X3Bfont_X2Dsize_X3A11px_X3Bopacity_X3A0_X3Btransition_X3Aopacity_X200_X2E3s_X3Bpointer_X2Devents_X3Anone_X3Bwhite_X2Dspace_X3Anowrap_X3B_X7D_X2Ecopy_X2Dmessage_X2Eshow_X7Bopacity_X3A1_X3B_X7D_X3C_X2Fstyle_X3E),
  /* K6   */  be_nested_str_weak(_X3Cdiv_X20class_X3D_X27anim_X2Dcontainer_X27_X3E_X3Ch3_X3EDSL_X20Code_X20Editor_X3C_X2Fh3_X3E_X3Cdiv_X20class_X3D_X27textarea_X2Dcontainer_X27_X3E_X3Ctextarea_X20id_X3D_X27dsl_code_X27_X20class_X3D_X27anim_X2Deditor_X27_X20spellcheck_X3D_X27false_X27_X20placeholder_X3D_X27Enter_X20your_X20Berry_X20Animation_X20DSL_X20code_X20here_X2E_X2E_X2E_X27_X3E),
  /* K7   */  be_nested_str_weak(last_dsl_code),
  /* K8   */  be_nested_str_long(_X3C_X2Ftextarea_X3E_X3Cdiv_X20class_X3D_X27copy_X2Dbtn_X27_X20onclick_X3D_X27copyDslCode_X28_X29_X27_X20title_X3D_X27Copy_X20DSL_X20code_X27_X3E_X3C_X2Fdiv_X3E_X3Cdiv_X20id_X3D_X27dsl_X2Dcopy_X2Dmsg_X27_X20class_X3D_X27copy_X2Dmessage_X27_X3E_X3C_X2Fdiv_X3E_X3C_X2Fdiv_X3E_X3Cdiv_X20id_X3D_X27status_X2Dmessage_X27_X3E_X3Cdiv_X20class_X3D_X27anim_X2Dsuccess_X27_X3E_X3Cstrong_X3EStatus_X3A_X3C_X2Fstrong_X3E_X20Ready_X3C_X2Fdiv_X3E_X3C_X2Fdiv_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3Cbutton_X20id_X3D_X27btn_X2Dcompile_X27_X20onclick_X3D_X27sendAction_X28_X22compile_X22_X29_X27_X20class_X3D_X27button_X20bgrn_X27_X3ECompile_X20_X26_X20Run_X3C_X2Fbutton_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3Cbutton_X20id_X3D_X27btn_X2Dcompile_X2Donly_X27_X20onclick_X3D_X27sendAction_X28_X22compile_only_X22_X29_X27_X20class_X3D_X27button_X27_X3ECompile_X20Only_X3C_X2Fbutton_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3Cbutton_X20id_X3D_X27btn_X2Dstop_X27_X20onclick_X3D_X27sendAction_X28_X22stop_X22_X29_X27_X20class_X3D_X27button_X27_X3EStop_X20Animation_X3C_X2Fbutton_X3E),
  /* K9   */  be_nested_str_weak(_X3Ch3_X3EGenerated_X20Berry_X20Code_X3C_X2Fh3_X3E_X3Cdiv_X20class_X3D_X27textarea_X2Dcontainer_X27_X3E_X3Ctextarea_X20id_X3D_X27berry_output_X27_X20class_X3D_X27anim_X2Doutput_X27_X20readonly_X3E),
  /* K10  */  be_nested_str_weak(html_escape),
  /* K11  */  be_nested_str_weak(last_berry_code),
  /* K12  */  be_nested_str_weak(_X3C_X2Ftextarea_X3E_X3Cdiv_X20class_X3D_X27copy_X2Dbtn_X27_X20onclick_X3D_X27copyBerryCode_X28_X29_X27_X20title_X3D_X27Copy_X20Berry_X20code_X27_X3E_X3C_X2Fdiv_X3E_X3Cdiv_X20id_X3D_X27berry_X2Dcopy_X2Dmsg_X27_X20class_X3D_X27copy_X2Dmessage_X27_X3E_X3C_X2Fdiv_X3E_X3C_X2Fdiv_X3E),
  /* K13  */  be_nested_str_weak(content_button),
  /* K14  */  be_nested_str_weak(BUTTON_MANAGEMENT),
  /* K15  */  be_nested_str_long(_X3Cscript_X3Efunction_X20showStatus_X28message_X2CisError_X29_X7Bvar_X20statusDiv_X3Deb_X28_X27status_X2Dmessage_X27_X29_X3Bif_X28message_X29_X7BstatusDiv_X2EinnerHTML_X3D_X27_X3Cdiv_X20class_X3D_X22anim_X2D_X27_X2B_X28isError_X3F_X27error_X27_X3A_X27success_X27_X29_X2B_X27_X22_X3E_X3Cstrong_X3E_X27_X2B_X28isError_X3F_X27Error_X27_X3A_X27Success_X27_X29_X2B_X27_X3A_X3C_X2Fstrong_X3E_X20_X27_X2Bmessage_X2B_X27_X3C_X2Fdiv_X3E_X27_X3B_X7Delse_X7BstatusDiv_X2EinnerHTML_X3D_X27_X27_X3B_X7D_X7Dfunction_X20showProcessingStatus_X28_X29_X7Bvar_X20statusDiv_X3Deb_X28_X27status_X2Dmessage_X27_X29_X3BstatusDiv_X2EinnerHTML_X3D_X27_X3Cdiv_X20class_X3D_X22anim_X2Dsuccess_X22_X3E_X3Cstrong_X3EStatus_X3A_X3C_X2Fstrong_X3E_X20Processing_X2E_X2E_X2E_X3C_X2Fdiv_X3E_X27_X3B_X7Dfunction_X20setButtonsDisabled_X28disabled_X29_X7Bvar_X20btnIds_X3D_X5B_X27btn_X2Dcompile_X27_X2C_X27btn_X2Dcompile_X2Donly_X27_X2C_X27btn_X2Dstop_X27_X5D_X3Bfor_X28var_X20i_X3D0_X3Bi_X3CbtnIds_X2Elength_X3Bi_X2B_X2B_X29_X7Bvar_X20btn_X3Deb_X28btnIds_X5Bi_X5D_X29_X3Bif_X28btn_X29btn_X2Edisabled_X3Ddisabled_X3B_X7D_X7D),
  /* K16  */  be_nested_str_long(function_X20sendAction_X28action_X29_X7BsetButtonsDisabled_X28true_X29_X3BshowProcessingStatus_X28_X29_X3Bvar_X20xhr_X3Dnew_X20XMLHttpRequest_X28_X29_X3Bvar_X20formData_X3Dnew_X20FormData_X28_X29_X3BformData_X2Eappend_X28_X27action_X27_X2Caction_X29_X3Bif_X28action_X21_X3D_X3D_X27stop_X27_X26_X26action_X21_X3D_X3D_X27clear_X27_X29_X7BformData_X2Eappend_X28_X27dsl_code_X27_X2Ceb_X28_X27dsl_code_X27_X29_X2Evalue_X29_X3B_X7Dxhr_X2Eopen_X28_X27POST_X27_X2C_X27_X2Fberry_anim_X3Fapi_X3Daction_X27_X2Ctrue_X29_X3Bxhr_X2Eonreadystatechange_X3Dfunction_X28_X29_X7Bif_X28xhr_X2EreadyState_X3D_X3D_X3D4_X29_X7BsetButtonsDisabled_X28false_X29_X3Bif_X28xhr_X2Estatus_X3D_X3D_X3D200_X29_X7Btry_X7Bvar_X20result_X3DJSON_X2Eparse_X28xhr_X2EresponseText_X29_X3Bif_X28result_X2Esuccess_X29_X7BshowStatus_X28result_X2Emessage_X2Cfalse_X29_X3Bif_X28result_X2Eberry_code_X21_X3D_X3Dundefined_X29_X7Beb_X28_X27berry_output_X27_X29_X2Evalue_X3Dresult_X2Eberry_code_X3B_X7Dif_X28result_X2Edsl_code_X21_X3D_X3Dundefined_X29_X7Beb_X28_X27dsl_code_X27_X29_X2Evalue_X3Dresult_X2Edsl_code_X3B_X7D_X7Delse_X7BshowStatus_X28result_X2Eerror_X2Ctrue_X29_X3Bif_X28result_X2Eerror_X2Eincludes_X28_X27Compilation_X20failed_X27_X29_X29_X7Beb_X28_X27berry_output_X27_X29_X2Evalue_X3D_X27_X23_X20Compilation_X20failed_X5Cn_X23_X20_X27_X2Bresult_X2Eerror_X3B_X7D_X7D_X7Dcatch_X28e_X29_X7BshowStatus_X28_X27Invalid_X20response_X20from_X20server_X27_X2Ctrue_X29_X3B_X7D_X7Delse_X7BshowStatus_X28_X27Network_X20error_X3A_X20_X27_X2Bxhr_X2Estatus_X2Ctrue_X29_X3B_X7D_X7D_X7D_X3Bxhr_X2Esend_X28formData_X29_X3B_X7D),
  /* K17  */  be_nested_str_long(function_X20showCopyMessage_X28msgId_X2Ctext_X2CisError_X29_X7Bvar_X20msgDiv_X3Deb_X28msgId_X29_X3BmsgDiv_X2EtextContent_X3Dtext_X3BmsgDiv_X2Estyle_X2Ebackground_X3D_X27color_X2Dmix_X28in_X20srgb_X2C_X20var_X28_X27_X2B_X28isError_X3F_X27_X2D_X2Dc_btnrst_X27_X3A_X27_X2D_X2Dc_btnsv_X27_X29_X2B_X27_X29_X2090_X25_X2C_X20transparent_X29_X27_X3BmsgDiv_X2EclassList_X2Eadd_X28_X27show_X27_X29_X3BsetTimeout_X28function_X28_X29_X7BmsgDiv_X2EclassList_X2Eremove_X28_X27show_X27_X29_X3B_X7D_X2C2000_X29_X3B_X7Dfunction_X20copyTextarea_X28textareaId_X2CmsgId_X29_X7Bvar_X20textarea_X3Deb_X28textareaId_X29_X3Btextarea_X2Eselect_X28_X29_X3Btextarea_X2EsetSelectionRange_X280_X2C99999_X29_X3Btry_X7Bdocument_X2EexecCommand_X28_X27copy_X27_X29_X3BshowCopyMessage_X28msgId_X2C_X27Copied_X21_X27_X2Cfalse_X29_X3B_X7Dcatch_X28err_X29_X7BshowCopyMessage_X28msgId_X2C_X27Copy_X20failed_X27_X2Ctrue_X29_X3B_X7D_X7Dfunction_X20copyDslCode_X28_X29_X7BcopyTextarea_X28_X27dsl_code_X27_X2C_X27dsl_X2Dcopy_X2Dmsg_X27_X29_X3B_X7Dfunction_X20copyBerryCode_X28_X29_X7BcopyTextarea_X28_X27berry_output_X27_X2C_X27berry_X2Dcopy_X2Dmsg_X27_X29_X3B_X7D_X3C_X2Fscript_X3E),
  /* K18  */  be_nested_str_weak(content_stop),
  /* K19  */  be_nested_str_weak(animation_dsl),
  /* K20  */  be_nested_str_weak(has_arg),
  /* K21  */  be_nested_str_weak(api),
  /* K22  */  be_nested_str_weak(arg),
  /* K23  */  be_nested_str_weak(action),
  /* K24  */  be_nested_str_weak(content_open),
  /* K25  */  be_nested_str_weak(application_X2Fjson),
  /* K26  */  be_nested_str_weak(compile),
  /* K27  */  be_nested_str_weak(compile_only),
  /* K28  */  be_nested_str_weak(dsl_code),
  /* K29  */  be_nested_str_weak(success),
  /* K30  */  be_nested_str_weak(berry_code),
  /* K31  */  be_nested_str_weak(execute),
  /* K32  */  be_nested_str_weak(message),
  /* K33  */  be_nested_str_weak(Animation_X20compiled_X20and_X20started),
  /* K34  */  be_nested_str_weak(DSL_X20compiled_X20successfully),
  /* K35  */  be_nested_str_weak(error),
  /* K36  */  be_nested_str_weak(_X25s_X3A_X20_X25s),
  /* K37  */  be_nested_str_weak(_X23_X20Compilation_X20failed_X0A_X23_X20_X25s),
  /* K38  */  be_nested_str_weak(No_X20DSL_X20code_X20provided),
  /* K39  */  be_nested_str_weak(stop),
  /* K40  */  be_nested_str_weak(animation),
  /* K41  */  be_nested_str_weak(init_strip),
  /* K42  */  be_nested_str_weak(Animation_X20stopped),
  /* K43  */  be_nested_str_weak(Unknown_X20action_X3A_X20_X25s),
  /* K44  */  be_nested_str_weak(No_X20action_X20specified),
  /* K45  */  be_nested_str_weak(json),
  /* K46  */  be_nested_str_weak(dump),
  /* K47  */  be_nested_str_weak(content_close),
  /* K48  */  be_nested_str_weak(page_main),
  /* K49  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3Cform_X20id_X3Dbut_part_mgr_X20style_X3D_X27display_X3A_X20block_X3B_X27_X20action_X3D_X27berry_anim_X27_X20method_X3D_X27get_X27_X3E_X3Cbutton_X3ELED_X20Animation_X20Console_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
  /* K50  */  be_nested_str_weak(log),
  /* K51  */  be_nested_str_weak(LED_X3A_X20Berry_X20Animation_X20WebUI_X20deinitialized),
  /* K52  */  be_const_int(3),
  /* K53  */  be_nested_str_weak(on),
  /* K54  */  be_nested_str_weak(_X2Fberry_anim),
  /* K55  */  be_nested_str_weak(DEFAULT_DSL),
  /* K56  */  be_nested_str_weak(),
  /* K57  */  be_nested_str_weak(tasmota),
  /* K58  */  be_nested_str_weak(add_driver),
  /* K59  */  be_nested_str_weak(is_network_up),
  /* K60  */  be_nested_str_weak(web_add_handler),
  /* K61  */  be_nested_str_weak(LED_X3A_X20Berry_X20Animation_X20WebUI_X20initialized),
};


extern const bclass be_class_AnimationWebUI;

/********************************************************************
** Solidified function: page_main
********************************************************************/
be_local_closure(class_AnimationWebUI_page_main,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationWebUI,     /* shared constants */
    be_str_weak(page_main),
    &be_const_str_solidified,
    ( &(const binstruction[44]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x58100002,  //  0002  LDCONST	R4	K2
      0x7C080400,  //  0003  CALL	R2	2
      0x8C080303,  //  0004  GETMET	R2	R1	K3
      0x7C080200,  //  0005  CALL	R2	1
      0x8C080304,  //  0006  GETMET	R2	R1	K4
      0x58100005,  //  0007  LDCONST	R4	K5
      0x7C080400,  //  0008  CALL	R2	2
      0x8C080304,  //  0009  GETMET	R2	R1	K4
      0x58100006,  //  000A  LDCONST	R4	K6
      0x7C080400,  //  000B  CALL	R2	2
      0x8C080304,  //  000C  GETMET	R2	R1	K4
      0x88100107,  //  000D  GETMBR	R4	R0	K7
      0x7C080400,  //  000E  CALL	R2	2
      0x8C080304,  //  000F  GETMET	R2	R1	K4
      0x58100008,  //  0010  LDCONST	R4	K8
      0x7C080400,  //  0011  CALL	R2	2
      0x8C080304,  //  0012  GETMET	R2	R1	K4
      0x58100009,  //  0013  LDCONST	R4	K9
      0x7C080400,  //  0014  CALL	R2	2
      0x8C080304,  //  0015  GETMET	R2	R1	K4
      0x8C10030A,  //  0016  GETMET	R4	R1	K10
      0x8818010B,  //  0017  GETMBR	R6	R0	K11
      0x7C100400,  //  0018  CALL	R4	2
      0x7C080400,  //  0019  CALL	R2	2
      0x8C080304,  //  001A  GETMET	R2	R1	K4
      0x5810000C,  //  001B  LDCONST	R4	K12
      0x7C080400,  //  001C  CALL	R2	2
      0x8C08030D,  //  001D  GETMET	R2	R1	K13
      0x8810030E,  //  001E  GETMBR	R4	R1	K14
      0x7C080400,  //  001F  CALL	R2	2
      0x8C080304,  //  0020  GETMET	R2	R1	K4
      0x5810000F,  //  0021  LDCONST	R4	K15
      0x7C080400,  //  0022  CALL	R2	2
      0x8C080304,  //  0023  GETMET	R2	R1	K4
      0x58100010,  //  0024  LDCONST	R4	K16
      0x7C080400,  //  0025  CALL	R2	2
      0x8C080304,  //  0026  GETMET	R2	R1	K4
      0x58100011,  //  0027  LDCONST	R4	K17
      0x7C080400,  //  0028  CALL	R2	2
      0x8C080312,  //  0029  GETMET	R2	R1	K18
      0x7C080200,  //  002A  CALL	R2	1
      0x80000000,  //  002B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: handle_request
********************************************************************/
be_local_closure(class_AnimationWebUI_handle_request,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationWebUI,     /* shared constants */
    be_str_weak(handle_request),
    &be_const_str_solidified,
    ( &(const binstruction[109]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A2600,  //  0001  IMPORT	R2	K19
      0x8C0C0314,  //  0002  GETMET	R3	R1	K20
      0x58140015,  //  0003  LDCONST	R5	K21
      0x7C0C0400,  //  0004  CALL	R3	2
      0x780E0063,  //  0005  JMPF	R3	#006A
      0x8C0C0316,  //  0006  GETMET	R3	R1	K22
      0x58140015,  //  0007  LDCONST	R5	K21
      0x7C0C0400,  //  0008  CALL	R3	2
      0x1C100717,  //  0009  EQ	R4	R3	K23
      0x7812005D,  //  000A  JMPF	R4	#0069
      0x8C100318,  //  000B  GETMET	R4	R1	K24
      0x541A00C7,  //  000C  LDINT	R6	200
      0x581C0019,  //  000D  LDCONST	R7	K25
      0x7C100600,  //  000E  CALL	R4	3
      0x60100013,  //  000F  GETGBL	R4	G19
      0x7C100000,  //  0010  CALL	R4	0
      0x8C140314,  //  0011  GETMET	R5	R1	K20
      0x581C0017,  //  0012  LDCONST	R7	K23
      0x7C140400,  //  0013  CALL	R5	2
      0x78160048,  //  0014  JMPF	R5	#005E
      0x8C140316,  //  0015  GETMET	R5	R1	K22
      0x581C0017,  //  0016  LDCONST	R7	K23
      0x7C140400,  //  0017  CALL	R5	2
      0x1C180B1A,  //  0018  EQ	R6	R5	K26
      0x741A0001,  //  0019  JMPT	R6	#001C
      0x1C180B1B,  //  001A  EQ	R6	R5	K27
      0x781A0030,  //  001B  JMPF	R6	#004D
      0x8C180314,  //  001C  GETMET	R6	R1	K20
      0x5820001C,  //  001D  LDCONST	R8	K28
      0x7C180400,  //  001E  CALL	R6	2
      0x781A0028,  //  001F  JMPF	R6	#0049
      0x8C180316,  //  0020  GETMET	R6	R1	K22
      0x5820001C,  //  0021  LDCONST	R8	K28
      0x7C180400,  //  0022  CALL	R6	2
      0x90020E06,  //  0023  SETMBR	R0	K7	R6
      0xA8020011,  //  0024  EXBLK	0	#0037
      0x8C18051A,  //  0025  GETMET	R6	R2	K26
      0x88200107,  //  0026  GETMBR	R8	R0	K7
      0x7C180400,  //  0027  CALL	R6	2
      0x90021606,  //  0028  SETMBR	R0	K11	R6
      0x50180200,  //  0029  LDBOOL	R6	1	0
      0x98123A06,  //  002A  SETIDX	R4	K29	R6
      0x8818010B,  //  002B  GETMBR	R6	R0	K11
      0x98123C06,  //  002C  SETIDX	R4	K30	R6
      0x1C180B1A,  //  002D  EQ	R6	R5	K26
      0x781A0004,  //  002E  JMPF	R6	#0034
      0x8C18051F,  //  002F  GETMET	R6	R2	K31
      0x88200107,  //  0030  GETMBR	R8	R0	K7
      0x7C180400,  //  0031  CALL	R6	2
      0x98124121,  //  0032  SETIDX	R4	K32	K33
      0x70020000,  //  0033  JMP		#0035
      0x98124122,  //  0034  SETIDX	R4	K32	K34
      0xA8040001,  //  0035  EXBLK	1	1
      0x70020010,  //  0036  JMP		#0048
      0xAC180002,  //  0037  CATCH	R6	0	2
      0x7002000D,  //  0038  JMP		#0047
      0x50200000,  //  0039  LDBOOL	R8	0	0
      0x98123A08,  //  003A  SETIDX	R4	K29	R8
      0x60200018,  //  003B  GETGBL	R8	G24
      0x58240024,  //  003C  LDCONST	R9	K36
      0x5C280C00,  //  003D  MOVE	R10	R6
      0x5C2C0E00,  //  003E  MOVE	R11	R7
      0x7C200600,  //  003F  CALL	R8	3
      0x98124608,  //  0040  SETIDX	R4	K35	R8
      0x60200018,  //  0041  GETGBL	R8	G24
      0x58240025,  //  0042  LDCONST	R9	K37
      0x94280923,  //  0043  GETIDX	R10	R4	K35
      0x7C200400,  //  0044  CALL	R8	2
      0x90021608,  //  0045  SETMBR	R0	K11	R8
      0x70020000,  //  0046  JMP		#0048
      0xB0080000,  //  0047  RAISE	2	R0	R0
      0x70020002,  //  0048  JMP		#004C
      0x50180000,  //  0049  LDBOOL	R6	0	0
      0x98123A06,  //  004A  SETIDX	R4	K29	R6
      0x98124726,  //  004B  SETIDX	R4	K35	K38
      0x7002000F,  //  004C  JMP		#005D
      0x1C180B27,  //  004D  EQ	R6	R5	K39
      0x781A0006,  //  004E  JMPF	R6	#0056
      0xB81A5000,  //  004F  GETNGBL	R6	K40
      0x8C180D29,  //  0050  GETMET	R6	R6	K41
      0x7C180200,  //  0051  CALL	R6	1
      0x50180200,  //  0052  LDBOOL	R6	1	0
      0x98123A06,  //  0053  SETIDX	R4	K29	R6
      0x9812412A,  //  0054  SETIDX	R4	K32	K42
      0x70020006,  //  0055  JMP		#005D
      0x50180000,  //  0056  LDBOOL	R6	0	0
      0x98123A06,  //  0057  SETIDX	R4	K29	R6
      0x60180018,  //  0058  GETGBL	R6	G24
      0x581C002B,  //  0059  LDCONST	R7	K43
      0x5C200A00,  //  005A  MOVE	R8	R5
      0x7C180400,  //  005B  CALL	R6	2
      0x98124606,  //  005C  SETIDX	R4	K35	R6
      0x70020002,  //  005D  JMP		#0061
      0x50140000,  //  005E  LDBOOL	R5	0	0
      0x98123A05,  //  005F  SETIDX	R4	K29	R5
      0x9812472C,  //  0060  SETIDX	R4	K35	K44
      0xA4165A00,  //  0061  IMPORT	R5	K45
      0x8C180304,  //  0062  GETMET	R6	R1	K4
      0x8C200B2E,  //  0063  GETMET	R8	R5	K46
      0x5C280800,  //  0064  MOVE	R10	R4
      0x7C200400,  //  0065  CALL	R8	2
      0x7C180400,  //  0066  CALL	R6	2
      0x8C18032F,  //  0067  GETMET	R6	R1	K47
      0x7C180200,  //  0068  CALL	R6	1
      0x70020001,  //  0069  JMP		#006C
      0x8C0C0130,  //  006A  GETMET	R3	R0	K48
      0x7C0C0200,  //  006B  CALL	R3	1
      0x80000000,  //  006C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_add_button
********************************************************************/
be_local_closure(class_AnimationWebUI_web_add_button,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationWebUI,     /* shared constants */
    be_str_weak(web_add_button),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080304,  //  0001  GETMET	R2	R1	K4
      0x58100031,  //  0002  LDCONST	R4	K49
      0x7C080400,  //  0003  CALL	R2	2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: deinit
********************************************************************/
be_local_closure(class_AnimationWebUI_deinit,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationWebUI,     /* shared constants */
    be_str_weak(deinit),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0xB8066400,  //  0000  GETNGBL	R1	K50
      0x58080033,  //  0001  LDCONST	R2	K51
      0x580C0034,  //  0002  LDCONST	R3	K52
      0x7C040400,  //  0003  CALL	R1	2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_add_handler
********************************************************************/
be_local_closure(class_AnimationWebUI_web_add_handler,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
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
        /* K0   */  be_nested_str_weak(handle_request),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 4]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0x80040000,  //  0003  RET	1	R0
        })
      ),
    }),
    1,                          /* has constants */
    &be_ktab_class_AnimationWebUI,     /* shared constants */
    be_str_weak(web_add_handler),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080335,  //  0001  GETMET	R2	R1	K53
      0x58100036,  //  0002  LDCONST	R4	K54
      0x84140000,  //  0003  CLOSURE	R5	P0
      0x7C080600,  //  0004  CALL	R2	3
      0xA0000000,  //  0005  CLOSE	R0
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_AnimationWebUI_init,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AnimationWebUI,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x88040137,  //  0000  GETMBR	R1	R0	K55
      0x90020E01,  //  0001  SETMBR	R0	K7	R1
      0x90021738,  //  0002  SETMBR	R0	K11	K56
      0xB8067200,  //  0003  GETNGBL	R1	K57
      0x8C04033A,  //  0004  GETMET	R1	R1	K58
      0x5C0C0000,  //  0005  MOVE	R3	R0
      0x7C040400,  //  0006  CALL	R1	2
      0xB8067200,  //  0007  GETNGBL	R1	K57
      0x8C04033B,  //  0008  GETMET	R1	R1	K59
      0x7C040200,  //  0009  CALL	R1	1
      0x78060001,  //  000A  JMPF	R1	#000D
      0x8C04013C,  //  000B  GETMET	R1	R0	K60
      0x7C040200,  //  000C  CALL	R1	1
      0xB8066400,  //  000D  GETNGBL	R1	K50
      0x5808003D,  //  000E  LDCONST	R2	K61
      0x580C0034,  //  000F  LDCONST	R3	K52
      0x7C040400,  //  0010  CALL	R1	2
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: AnimationWebUI
********************************************************************/
be_local_class(AnimationWebUI,
    2,
    NULL,
    be_nested_map(9,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(page_main, -1), be_const_closure(class_AnimationWebUI_page_main_closure) },
        { be_const_key_weak(handle_request, -1), be_const_closure(class_AnimationWebUI_handle_request_closure) },
        { be_const_key_weak(last_dsl_code, -1), be_const_var(0) },
        { be_const_key_weak(DEFAULT_DSL, -1), be_nested_str_long(_X23_X20Simple_X20Berry_X20Animation_X20Example_X20_X2D_X20Cylon_X20red_X20eye_X0A_X0Aset_X20strip_len_X20_X3D_X20strip_length_X28_X29_X0A_X0Aanimation_X20red_eye_X20_X3D_X20beacon_animation_X28_X0A_X20_X20color_X20_X3D_X20red_X0A_X20_X20pos_X20_X3D_X20smooth_X28min_value_X20_X3D_X200_X2C_X20max_value_X20_X3D_X20strip_len_X20_X2D_X202_X2C_X20duration_X20_X3D_X205s_X29_X0A_X20_X20beacon_size_X20_X3D_X203_X20_X20_X20_X20_X20_X20_X20_X23_X20small_X203_X20pixels_X20eye_X0A_X20_X20slew_size_X20_X3D_X202_X20_X20_X20_X20_X20_X20_X20_X20_X20_X23_X20with_X202_X20pixel_X20shading_X20around_X0A_X29_X0A_X0Arun_X20red_eye_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X23_X20run_X20the_X20animation_X0A) },
        { be_const_key_weak(deinit, 3), be_const_closure(class_AnimationWebUI_deinit_closure) },
        { be_const_key_weak(web_add_handler, 2), be_const_closure(class_AnimationWebUI_web_add_handler_closure) },
        { be_const_key_weak(last_berry_code, 7), be_const_var(1) },
        { be_const_key_weak(init, 8), be_const_closure(class_AnimationWebUI_init_closure) },
        { be_const_key_weak(web_add_button, -1), be_const_closure(class_AnimationWebUI_web_add_button_closure) },
    })),
    be_str_weak(AnimationWebUI)
);

/********************************************************************
** Solidified module: animation_dsl
********************************************************************/
be_local_module(animation_dsl,
    "animation_dsl",
    be_nested_map(17,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(animation_web_ui, -1), be_const_class(be_class_AnimationWebUI) },
        { be_const_key_weak(MockEngine, 14), be_const_class(be_class_MockEngine) },
        { be_const_key_weak(compile_dsl, -1), be_const_closure(compile_dsl_closure) },
        { be_const_key_weak(is_keyword, -1), be_const_closure(is_keyword_closure) },
        { be_const_key_weak(Token, -1), be_const_class(be_class_Token) },
        { be_const_key_weak(create_lexer, -1), be_const_class(be_class_Lexer) },
        { be_const_key_weak(compile, -1), be_const_closure(compile_dsl_source_closure) },
        { be_const_key_weak(compile_file, 9), be_const_closure(compile_file_closure) },
        { be_const_key_weak(named_colors, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(37,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(brown, -1), be_const_int(-5952982) },
        { be_const_key_weak(silver, -1), be_const_int(-4144960) },
        { be_const_key_weak(salmon, -1), be_const_int(-360334) },
        { be_const_key_weak(transparent, -1), be_const_int(0) },
        { be_const_key_weak(lime, 19), be_const_int(-16711936) },
        { be_const_key_weak(coral, 26), be_const_int(-32944) },
        { be_const_key_weak(cyan, -1), be_const_int(-16711681) },
        { be_const_key_weak(olive, -1), be_const_int(-8355840) },
        { be_const_key_weak(snow, 16), be_const_int(-1286) },
        { be_const_key_weak(violet, -1), be_const_int(-1146130) },
        { be_const_key_weak(green, 20), be_const_int(-16744448) },
        { be_const_key_weak(turquoise, -1), be_const_int(-12525360) },
        { be_const_key_weak(grey, -1), be_const_int(-8355712) },
        { be_const_key_weak(indigo, -1), be_const_int(-11861886) },
        { be_const_key_weak(navy, 23), be_const_int(-16777088) },
        { be_const_key_weak(white, 30), be_const_int(-1) },
        { be_const_key_weak(red, -1), be_const_int(-65536) },
        { be_const_key_weak(orange, -1), be_const_int(-23296) },
        { be_const_key_weak(gray, -1), be_const_int(-8355712) },
        { be_const_key_weak(purple, -1), be_const_int(-8388480) },
        { be_const_key_weak(beige, -1), be_const_int(-657956) },
        { be_const_key_weak(fuchsia, -1), be_const_int(-65281) },
        { be_const_key_weak(crimson, 12), be_const_int(-2354116) },
        { be_const_key_weak(black, 18), be_const_int(-16777216) },
        { be_const_key_weak(magenta, 2), be_const_int(-65281) },
        { be_const_key_weak(yellow, 15), be_const_int(-256) },
        { be_const_key_weak(tan, 27), be_const_int(-2968436) },
        { be_const_key_weak(gold, -1), be_const_int(-10496) },
        { be_const_key_weak(pink, -1), be_const_int(-16181) },
        { be_const_key_weak(ivory, -1), be_const_int(-16) },
        { be_const_key_weak(khaki, 35), be_const_int(-989556) },
        { be_const_key_weak(aqua, -1), be_const_int(-16711681) },
        { be_const_key_weak(blue, -1), be_const_int(-16776961) },
        { be_const_key_weak(plum, 21), be_const_int(-2252579) },
        { be_const_key_weak(orchid, -1), be_const_int(-2461482) },
        { be_const_key_weak(teal, -1), be_const_int(-16744320) },
        { be_const_key_weak(maroon, -1), be_const_int(-8388608) },
    }))    ) } )) },
        { be_const_key_weak(load_file, -1), be_const_closure(load_file_closure) },
        { be_const_key_weak(_symbol_entry, 6), be_const_class(be_class_SymbolEntry) },
        { be_const_key_weak(is_color_name, 7), be_const_closure(is_color_name_closure) },
        { be_const_key_weak(execute, 0), be_const_closure(execute_closure) },
        { be_const_key_weak(init, -1), be_const_closure(animation_dsl_init_closure) },
        { be_const_key_weak(VERSION, -1), be_const_int(65536) },
        { be_const_key_weak(_symbol_table, 13), be_const_class(be_class_SymbolTable) },
        { be_const_key_weak(SimpleDSLTranspiler, -1), be_const_class(be_class_SimpleDSLTranspiler) },
    }))
);
BE_EXPORT_VARIABLE be_define_const_native_module(animation_dsl);
/********************************************************************/
/********************************************************************/
/* End of solidification */
