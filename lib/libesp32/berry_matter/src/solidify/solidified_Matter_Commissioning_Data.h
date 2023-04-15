/* Solidification of Matter_Commissioning_Data.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_PBKDFParamRequest;

/********************************************************************
** Solidified function: parse
********************************************************************/
be_local_closure(Matter_PBKDFParamRequest_parse,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(parse),
    /* K4   */  be_nested_str_weak(initiatorRandom),
    /* K5   */  be_nested_str_weak(getsubval),
    /* K6   */  be_const_int(1),
    /* K7   */  be_nested_str_weak(initiator_session_id),
    /* K8   */  be_const_int(2),
    /* K9   */  be_nested_str_weak(passcodeId),
    /* K10  */  be_const_int(3),
    /* K11  */  be_nested_str_weak(hasPBKDFParameters),
    /* K12  */  be_nested_str_weak(findsub),
    /* K13  */  be_nested_str_weak(SLEEPY_IDLE_INTERVAL),
    /* K14  */  be_nested_str_weak(findsubval),
    /* K15  */  be_nested_str_weak(SLEEPY_ACTIVE_INTERVAL),
    }),
    be_str_weak(parse),
    &be_const_str_solidified,
    ( &(const binstruction[41]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x1C0C0403,  //  0001  EQ	R3	R2	R3
      0x780E0000,  //  0002  JMPF	R3	#0004
      0x58080000,  //  0003  LDCONST	R2	K0
      0xB80E0200,  //  0004  GETNGBL	R3	K1
      0x880C0702,  //  0005  GETMBR	R3	R3	K2
      0x8C0C0703,  //  0006  GETMET	R3	R3	K3
      0x5C140200,  //  0007  MOVE	R5	R1
      0x5C180400,  //  0008  MOVE	R6	R2
      0x7C0C0600,  //  0009  CALL	R3	3
      0x8C100705,  //  000A  GETMET	R4	R3	K5
      0x58180006,  //  000B  LDCONST	R6	K6
      0x7C100400,  //  000C  CALL	R4	2
      0x90020804,  //  000D  SETMBR	R0	K4	R4
      0x8C100705,  //  000E  GETMET	R4	R3	K5
      0x58180008,  //  000F  LDCONST	R6	K8
      0x7C100400,  //  0010  CALL	R4	2
      0x90020E04,  //  0011  SETMBR	R0	K7	R4
      0x8C100705,  //  0012  GETMET	R4	R3	K5
      0x5818000A,  //  0013  LDCONST	R6	K10
      0x7C100400,  //  0014  CALL	R4	2
      0x90021204,  //  0015  SETMBR	R0	K9	R4
      0x8C100705,  //  0016  GETMET	R4	R3	K5
      0x541A0003,  //  0017  LDINT	R6	4
      0x7C100400,  //  0018  CALL	R4	2
      0x90021604,  //  0019  SETMBR	R0	K11	R4
      0x8C10070C,  //  001A  GETMET	R4	R3	K12
      0x541A0004,  //  001B  LDINT	R6	5
      0x7C100400,  //  001C  CALL	R4	2
      0x4C140000,  //  001D  LDNIL	R5
      0x20140805,  //  001E  NE	R5	R4	R5
      0x78160007,  //  001F  JMPF	R5	#0028
      0x8C14090E,  //  0020  GETMET	R5	R4	K14
      0x581C0006,  //  0021  LDCONST	R7	K6
      0x7C140400,  //  0022  CALL	R5	2
      0x90021A05,  //  0023  SETMBR	R0	K13	R5
      0x8C14090E,  //  0024  GETMET	R5	R4	K14
      0x581C0008,  //  0025  LDCONST	R7	K8
      0x7C140400,  //  0026  CALL	R5	2
      0x90021E05,  //  0027  SETMBR	R0	K15	R5
      0x80040000,  //  0028  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_PBKDFParamRequest
********************************************************************/
be_local_class(Matter_PBKDFParamRequest,
    6,
    NULL,
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(initiatorRandom, 6), be_const_var(0) },
        { be_const_key_weak(passcodeId, -1), be_const_var(2) },
        { be_const_key_weak(hasPBKDFParameters, -1), be_const_var(3) },
        { be_const_key_weak(parse, -1), be_const_closure(Matter_PBKDFParamRequest_parse_closure) },
        { be_const_key_weak(initiator_session_id, 0), be_const_var(1) },
        { be_const_key_weak(SLEEPY_IDLE_INTERVAL, 3), be_const_var(4) },
        { be_const_key_weak(SLEEPY_ACTIVE_INTERVAL, -1), be_const_var(5) },
    })),
    be_str_weak(Matter_PBKDFParamRequest)
);
/*******************************************************************/

void be_load_Matter_PBKDFParamRequest_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_PBKDFParamRequest);
    be_setglobal(vm, "Matter_PBKDFParamRequest");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_PBKDFParamResponse;

/********************************************************************
** Solidified function: tlv2raw
********************************************************************/
be_local_closure(Matter_PBKDFParamResponse_tlv2raw,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[19]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(Matter_TLV_struct),
    /* K3   */  be_nested_str_weak(add_TLV),
    /* K4   */  be_const_int(1),
    /* K5   */  be_nested_str_weak(B1),
    /* K6   */  be_nested_str_weak(initiatorRandom),
    /* K7   */  be_const_int(2),
    /* K8   */  be_nested_str_weak(responderRandom),
    /* K9   */  be_const_int(3),
    /* K10  */  be_nested_str_weak(U2),
    /* K11  */  be_nested_str_weak(responderSessionId),
    /* K12  */  be_nested_str_weak(add_struct),
    /* K13  */  be_nested_str_weak(U4),
    /* K14  */  be_nested_str_weak(pbkdf_parameters_iterations),
    /* K15  */  be_nested_str_weak(pbkdf_parameters_salt),
    /* K16  */  be_nested_str_weak(SLEEPY_IDLE_INTERVAL),
    /* K17  */  be_nested_str_weak(SLEEPY_ACTIVE_INTERVAL),
    /* K18  */  be_nested_str_weak(tlv2raw),
    }),
    be_str_weak(tlv2raw),
    &be_const_str_solidified,
    ( &(const binstruction[71]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x88080501,  //  0001  GETMBR	R2	R2	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0x7C080200,  //  0003  CALL	R2	1
      0x8C0C0503,  //  0004  GETMET	R3	R2	K3
      0x58140004,  //  0005  LDCONST	R5	K4
      0xB81A0000,  //  0006  GETNGBL	R6	K0
      0x88180D01,  //  0007  GETMBR	R6	R6	K1
      0x88180D05,  //  0008  GETMBR	R6	R6	K5
      0x881C0106,  //  0009  GETMBR	R7	R0	K6
      0x7C0C0800,  //  000A  CALL	R3	4
      0x8C0C0503,  //  000B  GETMET	R3	R2	K3
      0x58140007,  //  000C  LDCONST	R5	K7
      0xB81A0000,  //  000D  GETNGBL	R6	K0
      0x88180D01,  //  000E  GETMBR	R6	R6	K1
      0x88180D05,  //  000F  GETMBR	R6	R6	K5
      0x881C0108,  //  0010  GETMBR	R7	R0	K8
      0x7C0C0800,  //  0011  CALL	R3	4
      0x8C0C0503,  //  0012  GETMET	R3	R2	K3
      0x58140009,  //  0013  LDCONST	R5	K9
      0xB81A0000,  //  0014  GETNGBL	R6	K0
      0x88180D01,  //  0015  GETMBR	R6	R6	K1
      0x88180D0A,  //  0016  GETMBR	R6	R6	K10
      0x881C010B,  //  0017  GETMBR	R7	R0	K11
      0x7C0C0800,  //  0018  CALL	R3	4
      0x8C0C050C,  //  0019  GETMET	R3	R2	K12
      0x54160003,  //  001A  LDINT	R5	4
      0x7C0C0400,  //  001B  CALL	R3	2
      0x8C100703,  //  001C  GETMET	R4	R3	K3
      0x58180004,  //  001D  LDCONST	R6	K4
      0xB81E0000,  //  001E  GETNGBL	R7	K0
      0x881C0F01,  //  001F  GETMBR	R7	R7	K1
      0x881C0F0D,  //  0020  GETMBR	R7	R7	K13
      0x8820010E,  //  0021  GETMBR	R8	R0	K14
      0x7C100800,  //  0022  CALL	R4	4
      0x8C100703,  //  0023  GETMET	R4	R3	K3
      0x58180007,  //  0024  LDCONST	R6	K7
      0xB81E0000,  //  0025  GETNGBL	R7	K0
      0x881C0F01,  //  0026  GETMBR	R7	R7	K1
      0x881C0F05,  //  0027  GETMBR	R7	R7	K5
      0x8820010F,  //  0028  GETMBR	R8	R0	K15
      0x7C100800,  //  0029  CALL	R4	4
      0x88100110,  //  002A  GETMBR	R4	R0	K16
      0x4C140000,  //  002B  LDNIL	R5
      0x20100805,  //  002C  NE	R4	R4	R5
      0x74120003,  //  002D  JMPT	R4	#0032
      0x88100111,  //  002E  GETMBR	R4	R0	K17
      0x4C140000,  //  002F  LDNIL	R5
      0x20100805,  //  0030  NE	R4	R4	R5
      0x78120010,  //  0031  JMPF	R4	#0043
      0x8C10050C,  //  0032  GETMET	R4	R2	K12
      0x541A0004,  //  0033  LDINT	R6	5
      0x7C100400,  //  0034  CALL	R4	2
      0x8C140903,  //  0035  GETMET	R5	R4	K3
      0x581C0004,  //  0036  LDCONST	R7	K4
      0xB8220000,  //  0037  GETNGBL	R8	K0
      0x88201101,  //  0038  GETMBR	R8	R8	K1
      0x8820110D,  //  0039  GETMBR	R8	R8	K13
      0x88240110,  //  003A  GETMBR	R9	R0	K16
      0x7C140800,  //  003B  CALL	R5	4
      0x8C140903,  //  003C  GETMET	R5	R4	K3
      0x581C0007,  //  003D  LDCONST	R7	K7
      0xB8220000,  //  003E  GETNGBL	R8	K0
      0x88201101,  //  003F  GETMBR	R8	R8	K1
      0x8820110D,  //  0040  GETMBR	R8	R8	K13
      0x88240111,  //  0041  GETMBR	R9	R0	K17
      0x7C140800,  //  0042  CALL	R5	4
      0x8C100512,  //  0043  GETMET	R4	R2	K18
      0x5C180200,  //  0044  MOVE	R6	R1
      0x7C100400,  //  0045  CALL	R4	2
      0x80040800,  //  0046  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_PBKDFParamResponse
********************************************************************/
be_local_class(Matter_PBKDFParamResponse,
    7,
    NULL,
    be_nested_map(8,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(pbkdf_parameters_salt, -1), be_const_var(4) },
        { be_const_key_weak(SLEEPY_ACTIVE_INTERVAL, -1), be_const_var(6) },
        { be_const_key_weak(SLEEPY_IDLE_INTERVAL, 1), be_const_var(5) },
        { be_const_key_weak(responderSessionId, -1), be_const_var(2) },
        { be_const_key_weak(pbkdf_parameters_iterations, -1), be_const_var(3) },
        { be_const_key_weak(initiatorRandom, 7), be_const_var(0) },
        { be_const_key_weak(responderRandom, 3), be_const_var(1) },
        { be_const_key_weak(tlv2raw, -1), be_const_closure(Matter_PBKDFParamResponse_tlv2raw_closure) },
    })),
    be_str_weak(Matter_PBKDFParamResponse)
);
/*******************************************************************/

void be_load_Matter_PBKDFParamResponse_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_PBKDFParamResponse);
    be_setglobal(vm, "Matter_PBKDFParamResponse");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_Pake1;

/********************************************************************
** Solidified function: parse
********************************************************************/
be_local_closure(Matter_Pake1_parse,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(parse),
    /* K4   */  be_nested_str_weak(pA),
    /* K5   */  be_nested_str_weak(getsubval),
    /* K6   */  be_const_int(1),
    }),
    be_str_weak(parse),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x1C0C0403,  //  0001  EQ	R3	R2	R3
      0x780E0000,  //  0002  JMPF	R3	#0004
      0x58080000,  //  0003  LDCONST	R2	K0
      0xB80E0200,  //  0004  GETNGBL	R3	K1
      0x880C0702,  //  0005  GETMBR	R3	R3	K2
      0x8C0C0703,  //  0006  GETMET	R3	R3	K3
      0x5C140200,  //  0007  MOVE	R5	R1
      0x5C180400,  //  0008  MOVE	R6	R2
      0x7C0C0600,  //  0009  CALL	R3	3
      0x8C100705,  //  000A  GETMET	R4	R3	K5
      0x58180006,  //  000B  LDCONST	R6	K6
      0x7C100400,  //  000C  CALL	R4	2
      0x90020804,  //  000D  SETMBR	R0	K4	R4
      0x80040000,  //  000E  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Pake1
********************************************************************/
be_local_class(Matter_Pake1,
    1,
    NULL,
    be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(pA, 1), be_const_var(0) },
        { be_const_key_weak(parse, -1), be_const_closure(Matter_Pake1_parse_closure) },
    })),
    be_str_weak(Matter_Pake1)
);
/*******************************************************************/

void be_load_Matter_Pake1_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Pake1);
    be_setglobal(vm, "Matter_Pake1");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_Pake2;

/********************************************************************
** Solidified function: tlv2raw
********************************************************************/
be_local_closure(Matter_Pake2_tlv2raw,   /* name */
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
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(Matter_TLV_struct),
    /* K3   */  be_nested_str_weak(add_TLV),
    /* K4   */  be_const_int(1),
    /* K5   */  be_nested_str_weak(B1),
    /* K6   */  be_nested_str_weak(pB),
    /* K7   */  be_const_int(2),
    /* K8   */  be_nested_str_weak(cB),
    /* K9   */  be_nested_str_weak(tlv2raw),
    }),
    be_str_weak(tlv2raw),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x88080501,  //  0001  GETMBR	R2	R2	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0x7C080200,  //  0003  CALL	R2	1
      0x8C0C0503,  //  0004  GETMET	R3	R2	K3
      0x58140004,  //  0005  LDCONST	R5	K4
      0xB81A0000,  //  0006  GETNGBL	R6	K0
      0x88180D01,  //  0007  GETMBR	R6	R6	K1
      0x88180D05,  //  0008  GETMBR	R6	R6	K5
      0x881C0106,  //  0009  GETMBR	R7	R0	K6
      0x7C0C0800,  //  000A  CALL	R3	4
      0x8C0C0503,  //  000B  GETMET	R3	R2	K3
      0x58140007,  //  000C  LDCONST	R5	K7
      0xB81A0000,  //  000D  GETNGBL	R6	K0
      0x88180D01,  //  000E  GETMBR	R6	R6	K1
      0x88180D05,  //  000F  GETMBR	R6	R6	K5
      0x881C0108,  //  0010  GETMBR	R7	R0	K8
      0x7C0C0800,  //  0011  CALL	R3	4
      0x8C0C0509,  //  0012  GETMET	R3	R2	K9
      0x5C140200,  //  0013  MOVE	R5	R1
      0x7C0C0400,  //  0014  CALL	R3	2
      0x80040600,  //  0015  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Pake2
********************************************************************/
be_local_class(Matter_Pake2,
    2,
    NULL,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(tlv2raw, -1), be_const_closure(Matter_Pake2_tlv2raw_closure) },
        { be_const_key_weak(cB, -1), be_const_var(1) },
        { be_const_key_weak(pB, 0), be_const_var(0) },
    })),
    be_str_weak(Matter_Pake2)
);
/*******************************************************************/

void be_load_Matter_Pake2_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Pake2);
    be_setglobal(vm, "Matter_Pake2");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_Pake3;

/********************************************************************
** Solidified function: parse
********************************************************************/
be_local_closure(Matter_Pake3_parse,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(parse),
    /* K4   */  be_nested_str_weak(cA),
    /* K5   */  be_nested_str_weak(getsubval),
    /* K6   */  be_const_int(1),
    }),
    be_str_weak(parse),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x1C0C0403,  //  0001  EQ	R3	R2	R3
      0x780E0000,  //  0002  JMPF	R3	#0004
      0x58080000,  //  0003  LDCONST	R2	K0
      0xB80E0200,  //  0004  GETNGBL	R3	K1
      0x880C0702,  //  0005  GETMBR	R3	R3	K2
      0x8C0C0703,  //  0006  GETMET	R3	R3	K3
      0x5C140200,  //  0007  MOVE	R5	R1
      0x5C180400,  //  0008  MOVE	R6	R2
      0x7C0C0600,  //  0009  CALL	R3	3
      0x8C100705,  //  000A  GETMET	R4	R3	K5
      0x58180006,  //  000B  LDCONST	R6	K6
      0x7C100400,  //  000C  CALL	R4	2
      0x90020804,  //  000D  SETMBR	R0	K4	R4
      0x80040000,  //  000E  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Pake3
********************************************************************/
be_local_class(Matter_Pake3,
    1,
    NULL,
    be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(parse, -1), be_const_closure(Matter_Pake3_parse_closure) },
        { be_const_key_weak(cA, -1), be_const_var(0) },
    })),
    be_str_weak(Matter_Pake3)
);
/*******************************************************************/

void be_load_Matter_Pake3_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Pake3);
    be_setglobal(vm, "Matter_Pake3");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_Sigma1;

/********************************************************************
** Solidified function: parse
********************************************************************/
be_local_closure(Matter_Sigma1_parse,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[20]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(parse),
    /* K4   */  be_nested_str_weak(Msg1),
    /* K5   */  be_const_int(2147483647),
    /* K6   */  be_nested_str_weak(initiatorRandom),
    /* K7   */  be_nested_str_weak(getsubval),
    /* K8   */  be_const_int(1),
    /* K9   */  be_nested_str_weak(initiator_session_id),
    /* K10  */  be_const_int(2),
    /* K11  */  be_nested_str_weak(destinationId),
    /* K12  */  be_const_int(3),
    /* K13  */  be_nested_str_weak(initiatorEphPubKey),
    /* K14  */  be_nested_str_weak(findsub),
    /* K15  */  be_nested_str_weak(SLEEPY_IDLE_INTERVAL),
    /* K16  */  be_nested_str_weak(findsubval),
    /* K17  */  be_nested_str_weak(SLEEPY_ACTIVE_INTERVAL),
    /* K18  */  be_nested_str_weak(resumptionID),
    /* K19  */  be_nested_str_weak(initiatorResumeMIC),
    }),
    be_str_weak(parse),
    &be_const_str_solidified,
    ( &(const binstruction[52]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x1C0C0403,  //  0001  EQ	R3	R2	R3
      0x780E0000,  //  0002  JMPF	R3	#0004
      0x58080000,  //  0003  LDCONST	R2	K0
      0xB80E0200,  //  0004  GETNGBL	R3	K1
      0x880C0702,  //  0005  GETMBR	R3	R3	K2
      0x8C0C0703,  //  0006  GETMET	R3	R3	K3
      0x5C140200,  //  0007  MOVE	R5	R1
      0x5C180400,  //  0008  MOVE	R6	R2
      0x7C0C0600,  //  0009  CALL	R3	3
      0x40100505,  //  000A  CONNECT	R4	R2	K5
      0x94100204,  //  000B  GETIDX	R4	R1	R4
      0x90020804,  //  000C  SETMBR	R0	K4	R4
      0x8C100707,  //  000D  GETMET	R4	R3	K7
      0x58180008,  //  000E  LDCONST	R6	K8
      0x7C100400,  //  000F  CALL	R4	2
      0x90020C04,  //  0010  SETMBR	R0	K6	R4
      0x8C100707,  //  0011  GETMET	R4	R3	K7
      0x5818000A,  //  0012  LDCONST	R6	K10
      0x7C100400,  //  0013  CALL	R4	2
      0x90021204,  //  0014  SETMBR	R0	K9	R4
      0x8C100707,  //  0015  GETMET	R4	R3	K7
      0x5818000C,  //  0016  LDCONST	R6	K12
      0x7C100400,  //  0017  CALL	R4	2
      0x90021604,  //  0018  SETMBR	R0	K11	R4
      0x8C100707,  //  0019  GETMET	R4	R3	K7
      0x541A0003,  //  001A  LDINT	R6	4
      0x7C100400,  //  001B  CALL	R4	2
      0x90021A04,  //  001C  SETMBR	R0	K13	R4
      0x8C10070E,  //  001D  GETMET	R4	R3	K14
      0x541A0004,  //  001E  LDINT	R6	5
      0x7C100400,  //  001F  CALL	R4	2
      0x4C140000,  //  0020  LDNIL	R5
      0x20140805,  //  0021  NE	R5	R4	R5
      0x78160007,  //  0022  JMPF	R5	#002B
      0x8C140910,  //  0023  GETMET	R5	R4	K16
      0x581C0008,  //  0024  LDCONST	R7	K8
      0x7C140400,  //  0025  CALL	R5	2
      0x90021E05,  //  0026  SETMBR	R0	K15	R5
      0x8C140910,  //  0027  GETMET	R5	R4	K16
      0x581C000A,  //  0028  LDCONST	R7	K10
      0x7C140400,  //  0029  CALL	R5	2
      0x90022205,  //  002A  SETMBR	R0	K17	R5
      0x8C140710,  //  002B  GETMET	R5	R3	K16
      0x541E0005,  //  002C  LDINT	R7	6
      0x7C140400,  //  002D  CALL	R5	2
      0x90022405,  //  002E  SETMBR	R0	K18	R5
      0x8C140710,  //  002F  GETMET	R5	R3	K16
      0x541E0006,  //  0030  LDINT	R7	7
      0x7C140400,  //  0031  CALL	R5	2
      0x90022605,  //  0032  SETMBR	R0	K19	R5
      0x80040000,  //  0033  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Sigma1
********************************************************************/
be_local_class(Matter_Sigma1,
    9,
    NULL,
    be_nested_map(10,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(SLEEPY_ACTIVE_INTERVAL, -1), be_const_var(5) },
        { be_const_key_weak(Msg1, -1), be_const_var(8) },
        { be_const_key_weak(parse, 6), be_const_closure(Matter_Sigma1_parse_closure) },
        { be_const_key_weak(initiatorRandom, -1), be_const_var(0) },
        { be_const_key_weak(SLEEPY_IDLE_INTERVAL, 7), be_const_var(4) },
        { be_const_key_weak(initiatorEphPubKey, -1), be_const_var(3) },
        { be_const_key_weak(initiatorResumeMIC, -1), be_const_var(7) },
        { be_const_key_weak(resumptionID, -1), be_const_var(6) },
        { be_const_key_weak(destinationId, -1), be_const_var(2) },
        { be_const_key_weak(initiator_session_id, 1), be_const_var(1) },
    })),
    be_str_weak(Matter_Sigma1)
);
/*******************************************************************/

void be_load_Matter_Sigma1_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Sigma1);
    be_setglobal(vm, "Matter_Sigma1");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_Sigma2;

/********************************************************************
** Solidified function: tlv2raw
********************************************************************/
be_local_closure(Matter_Sigma2_tlv2raw,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(Matter_TLV_struct),
    /* K3   */  be_nested_str_weak(add_TLV),
    /* K4   */  be_const_int(1),
    /* K5   */  be_nested_str_weak(B1),
    /* K6   */  be_nested_str_weak(responderRandom),
    /* K7   */  be_const_int(2),
    /* K8   */  be_nested_str_weak(U2),
    /* K9   */  be_nested_str_weak(responderSessionId),
    /* K10  */  be_const_int(3),
    /* K11  */  be_nested_str_weak(responderEphPubKey),
    /* K12  */  be_nested_str_weak(encrypted2),
    /* K13  */  be_nested_str_weak(SLEEPY_IDLE_INTERVAL),
    /* K14  */  be_nested_str_weak(SLEEPY_ACTIVE_INTERVAL),
    /* K15  */  be_nested_str_weak(add_struct),
    /* K16  */  be_nested_str_weak(U4),
    /* K17  */  be_nested_str_weak(tlv2raw),
    }),
    be_str_weak(tlv2raw),
    &be_const_str_solidified,
    ( &(const binstruction[61]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x88080501,  //  0001  GETMBR	R2	R2	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0x7C080200,  //  0003  CALL	R2	1
      0x8C0C0503,  //  0004  GETMET	R3	R2	K3
      0x58140004,  //  0005  LDCONST	R5	K4
      0xB81A0000,  //  0006  GETNGBL	R6	K0
      0x88180D01,  //  0007  GETMBR	R6	R6	K1
      0x88180D05,  //  0008  GETMBR	R6	R6	K5
      0x881C0106,  //  0009  GETMBR	R7	R0	K6
      0x7C0C0800,  //  000A  CALL	R3	4
      0x8C0C0503,  //  000B  GETMET	R3	R2	K3
      0x58140007,  //  000C  LDCONST	R5	K7
      0xB81A0000,  //  000D  GETNGBL	R6	K0
      0x88180D01,  //  000E  GETMBR	R6	R6	K1
      0x88180D08,  //  000F  GETMBR	R6	R6	K8
      0x881C0109,  //  0010  GETMBR	R7	R0	K9
      0x7C0C0800,  //  0011  CALL	R3	4
      0x8C0C0503,  //  0012  GETMET	R3	R2	K3
      0x5814000A,  //  0013  LDCONST	R5	K10
      0xB81A0000,  //  0014  GETNGBL	R6	K0
      0x88180D01,  //  0015  GETMBR	R6	R6	K1
      0x88180D05,  //  0016  GETMBR	R6	R6	K5
      0x881C010B,  //  0017  GETMBR	R7	R0	K11
      0x7C0C0800,  //  0018  CALL	R3	4
      0x8C0C0503,  //  0019  GETMET	R3	R2	K3
      0x54160003,  //  001A  LDINT	R5	4
      0xB81A0000,  //  001B  GETNGBL	R6	K0
      0x88180D01,  //  001C  GETMBR	R6	R6	K1
      0x88180D05,  //  001D  GETMBR	R6	R6	K5
      0x881C010C,  //  001E  GETMBR	R7	R0	K12
      0x7C0C0800,  //  001F  CALL	R3	4
      0x880C010D,  //  0020  GETMBR	R3	R0	K13
      0x4C100000,  //  0021  LDNIL	R4
      0x200C0604,  //  0022  NE	R3	R3	R4
      0x740E0003,  //  0023  JMPT	R3	#0028
      0x880C010E,  //  0024  GETMBR	R3	R0	K14
      0x4C100000,  //  0025  LDNIL	R4
      0x200C0604,  //  0026  NE	R3	R3	R4
      0x780E0010,  //  0027  JMPF	R3	#0039
      0x8C0C050F,  //  0028  GETMET	R3	R2	K15
      0x54160004,  //  0029  LDINT	R5	5
      0x7C0C0400,  //  002A  CALL	R3	2
      0x8C100703,  //  002B  GETMET	R4	R3	K3
      0x58180004,  //  002C  LDCONST	R6	K4
      0xB81E0000,  //  002D  GETNGBL	R7	K0
      0x881C0F01,  //  002E  GETMBR	R7	R7	K1
      0x881C0F10,  //  002F  GETMBR	R7	R7	K16
      0x8820010D,  //  0030  GETMBR	R8	R0	K13
      0x7C100800,  //  0031  CALL	R4	4
      0x8C100703,  //  0032  GETMET	R4	R3	K3
      0x58180007,  //  0033  LDCONST	R6	K7
      0xB81E0000,  //  0034  GETNGBL	R7	K0
      0x881C0F01,  //  0035  GETMBR	R7	R7	K1
      0x881C0F10,  //  0036  GETMBR	R7	R7	K16
      0x8820010E,  //  0037  GETMBR	R8	R0	K14
      0x7C100800,  //  0038  CALL	R4	4
      0x8C0C0511,  //  0039  GETMET	R3	R2	K17
      0x5C140200,  //  003A  MOVE	R5	R1
      0x7C0C0400,  //  003B  CALL	R3	2
      0x80040600,  //  003C  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Sigma2
********************************************************************/
be_local_class(Matter_Sigma2,
    6,
    NULL,
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(tlv2raw, -1), be_const_closure(Matter_Sigma2_tlv2raw_closure) },
        { be_const_key_weak(responderEphPubKey, 3), be_const_var(2) },
        { be_const_key_weak(responderSessionId, -1), be_const_var(1) },
        { be_const_key_weak(SLEEPY_IDLE_INTERVAL, -1), be_const_var(4) },
        { be_const_key_weak(SLEEPY_ACTIVE_INTERVAL, 6), be_const_var(5) },
        { be_const_key_weak(responderRandom, 1), be_const_var(0) },
        { be_const_key_weak(encrypted2, -1), be_const_var(3) },
    })),
    be_str_weak(Matter_Sigma2)
);
/*******************************************************************/

void be_load_Matter_Sigma2_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Sigma2);
    be_setglobal(vm, "Matter_Sigma2");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_Sigma2Resume;

/********************************************************************
** Solidified function: tlv2raw
********************************************************************/
be_local_closure(Matter_Sigma2Resume_tlv2raw,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(Matter_TLV_struct),
    /* K3   */  be_nested_str_weak(add_TLV),
    /* K4   */  be_const_int(1),
    /* K5   */  be_nested_str_weak(B1),
    /* K6   */  be_nested_str_weak(resumptionID),
    /* K7   */  be_const_int(2),
    /* K8   */  be_nested_str_weak(sigma2ResumeMIC),
    /* K9   */  be_const_int(3),
    /* K10  */  be_nested_str_weak(U2),
    /* K11  */  be_nested_str_weak(responderSessionID),
    /* K12  */  be_nested_str_weak(SLEEPY_IDLE_INTERVAL),
    /* K13  */  be_nested_str_weak(SLEEPY_ACTIVE_INTERVAL),
    /* K14  */  be_nested_str_weak(add_struct),
    /* K15  */  be_nested_str_weak(U4),
    /* K16  */  be_nested_str_weak(tlv2raw),
    }),
    be_str_weak(tlv2raw),
    &be_const_str_solidified,
    ( &(const binstruction[54]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x88080501,  //  0001  GETMBR	R2	R2	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0x7C080200,  //  0003  CALL	R2	1
      0x8C0C0503,  //  0004  GETMET	R3	R2	K3
      0x58140004,  //  0005  LDCONST	R5	K4
      0xB81A0000,  //  0006  GETNGBL	R6	K0
      0x88180D01,  //  0007  GETMBR	R6	R6	K1
      0x88180D05,  //  0008  GETMBR	R6	R6	K5
      0x881C0106,  //  0009  GETMBR	R7	R0	K6
      0x7C0C0800,  //  000A  CALL	R3	4
      0x8C0C0503,  //  000B  GETMET	R3	R2	K3
      0x58140007,  //  000C  LDCONST	R5	K7
      0xB81A0000,  //  000D  GETNGBL	R6	K0
      0x88180D01,  //  000E  GETMBR	R6	R6	K1
      0x88180D05,  //  000F  GETMBR	R6	R6	K5
      0x881C0108,  //  0010  GETMBR	R7	R0	K8
      0x7C0C0800,  //  0011  CALL	R3	4
      0x8C0C0503,  //  0012  GETMET	R3	R2	K3
      0x58140009,  //  0013  LDCONST	R5	K9
      0xB81A0000,  //  0014  GETNGBL	R6	K0
      0x88180D01,  //  0015  GETMBR	R6	R6	K1
      0x88180D0A,  //  0016  GETMBR	R6	R6	K10
      0x881C010B,  //  0017  GETMBR	R7	R0	K11
      0x7C0C0800,  //  0018  CALL	R3	4
      0x880C010C,  //  0019  GETMBR	R3	R0	K12
      0x4C100000,  //  001A  LDNIL	R4
      0x200C0604,  //  001B  NE	R3	R3	R4
      0x740E0003,  //  001C  JMPT	R3	#0021
      0x880C010D,  //  001D  GETMBR	R3	R0	K13
      0x4C100000,  //  001E  LDNIL	R4
      0x200C0604,  //  001F  NE	R3	R3	R4
      0x780E0010,  //  0020  JMPF	R3	#0032
      0x8C0C050E,  //  0021  GETMET	R3	R2	K14
      0x54160003,  //  0022  LDINT	R5	4
      0x7C0C0400,  //  0023  CALL	R3	2
      0x8C100703,  //  0024  GETMET	R4	R3	K3
      0x58180004,  //  0025  LDCONST	R6	K4
      0xB81E0000,  //  0026  GETNGBL	R7	K0
      0x881C0F01,  //  0027  GETMBR	R7	R7	K1
      0x881C0F0F,  //  0028  GETMBR	R7	R7	K15
      0x8820010C,  //  0029  GETMBR	R8	R0	K12
      0x7C100800,  //  002A  CALL	R4	4
      0x8C100703,  //  002B  GETMET	R4	R3	K3
      0x58180007,  //  002C  LDCONST	R6	K7
      0xB81E0000,  //  002D  GETNGBL	R7	K0
      0x881C0F01,  //  002E  GETMBR	R7	R7	K1
      0x881C0F0F,  //  002F  GETMBR	R7	R7	K15
      0x8820010D,  //  0030  GETMBR	R8	R0	K13
      0x7C100800,  //  0031  CALL	R4	4
      0x8C0C0510,  //  0032  GETMET	R3	R2	K16
      0x5C140200,  //  0033  MOVE	R5	R1
      0x7C0C0400,  //  0034  CALL	R3	2
      0x80040600,  //  0035  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Sigma2Resume
********************************************************************/
be_local_class(Matter_Sigma2Resume,
    5,
    NULL,
    be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(SLEEPY_IDLE_INTERVAL, 3), be_const_var(3) },
        { be_const_key_weak(resumptionID, -1), be_const_var(0) },
        { be_const_key_weak(sigma2ResumeMIC, -1), be_const_var(1) },
        { be_const_key_weak(responderSessionID, 1), be_const_var(2) },
        { be_const_key_weak(SLEEPY_ACTIVE_INTERVAL, -1), be_const_var(4) },
        { be_const_key_weak(tlv2raw, -1), be_const_closure(Matter_Sigma2Resume_tlv2raw_closure) },
    })),
    be_str_weak(Matter_Sigma2Resume)
);
/*******************************************************************/

void be_load_Matter_Sigma2Resume_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Sigma2Resume);
    be_setglobal(vm, "Matter_Sigma2Resume");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_Sigma3;

/********************************************************************
** Solidified function: parse
********************************************************************/
be_local_closure(Matter_Sigma3_parse,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(parse),
    /* K4   */  be_nested_str_weak(Msg3),
    /* K5   */  be_const_int(2147483647),
    /* K6   */  be_nested_str_weak(TBEData3Encrypted),
    /* K7   */  be_nested_str_weak(getsubval),
    /* K8   */  be_const_int(1),
    }),
    be_str_weak(parse),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x1C0C0403,  //  0001  EQ	R3	R2	R3
      0x780E0000,  //  0002  JMPF	R3	#0004
      0x58080000,  //  0003  LDCONST	R2	K0
      0xB80E0200,  //  0004  GETNGBL	R3	K1
      0x880C0702,  //  0005  GETMBR	R3	R3	K2
      0x8C0C0703,  //  0006  GETMET	R3	R3	K3
      0x5C140200,  //  0007  MOVE	R5	R1
      0x5C180400,  //  0008  MOVE	R6	R2
      0x7C0C0600,  //  0009  CALL	R3	3
      0x40100505,  //  000A  CONNECT	R4	R2	K5
      0x94100204,  //  000B  GETIDX	R4	R1	R4
      0x90020804,  //  000C  SETMBR	R0	K4	R4
      0x8C100707,  //  000D  GETMET	R4	R3	K7
      0x58180008,  //  000E  LDCONST	R6	K8
      0x7C100400,  //  000F  CALL	R4	2
      0x90020C04,  //  0010  SETMBR	R0	K6	R4
      0x80040000,  //  0011  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Sigma3
********************************************************************/
be_local_class(Matter_Sigma3,
    2,
    NULL,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(TBEData3Encrypted, 2), be_const_var(0) },
        { be_const_key_weak(parse, -1), be_const_closure(Matter_Sigma3_parse_closure) },
        { be_const_key_weak(Msg3, -1), be_const_var(1) },
    })),
    be_str_weak(Matter_Sigma3)
);
/*******************************************************************/

void be_load_Matter_Sigma3_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Sigma3);
    be_setglobal(vm, "Matter_Sigma3");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
