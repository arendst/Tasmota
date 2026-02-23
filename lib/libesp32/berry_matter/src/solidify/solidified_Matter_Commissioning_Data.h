/* Solidification of Matter_Commissioning_Data.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_PBKDFParamRequest;

/********************************************************************
** Solidified function: parse
********************************************************************/
be_local_closure(class_Matter_PBKDFParamRequest_parse,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_const_int(0),
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
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x880C0701,  //  0001  GETMBR	R3	R3	K1
      0x4C100000,  //  0002  LDNIL	R4
      0x1C100404,  //  0003  EQ	R4	R2	R4
      0x78120000,  //  0004  JMPF	R4	#0006
      0x58080002,  //  0005  LDCONST	R2	K2
      0x8C100703,  //  0006  GETMET	R4	R3	K3
      0x5C180200,  //  0007  MOVE	R6	R1
      0x5C1C0400,  //  0008  MOVE	R7	R2
      0x7C100600,  //  0009  CALL	R4	3
      0x8C140905,  //  000A  GETMET	R5	R4	K5
      0x581C0006,  //  000B  LDCONST	R7	K6
      0x7C140400,  //  000C  CALL	R5	2
      0x90020805,  //  000D  SETMBR	R0	K4	R5
      0x8C140905,  //  000E  GETMET	R5	R4	K5
      0x581C0008,  //  000F  LDCONST	R7	K8
      0x7C140400,  //  0010  CALL	R5	2
      0x90020E05,  //  0011  SETMBR	R0	K7	R5
      0x8C140905,  //  0012  GETMET	R5	R4	K5
      0x581C000A,  //  0013  LDCONST	R7	K10
      0x7C140400,  //  0014  CALL	R5	2
      0x90021205,  //  0015  SETMBR	R0	K9	R5
      0x8C140905,  //  0016  GETMET	R5	R4	K5
      0x541E0003,  //  0017  LDINT	R7	4
      0x7C140400,  //  0018  CALL	R5	2
      0x90021605,  //  0019  SETMBR	R0	K11	R5
      0x8C14090C,  //  001A  GETMET	R5	R4	K12
      0x541E0004,  //  001B  LDINT	R7	5
      0x7C140400,  //  001C  CALL	R5	2
      0x4C180000,  //  001D  LDNIL	R6
      0x20180A06,  //  001E  NE	R6	R5	R6
      0x781A0007,  //  001F  JMPF	R6	#0028
      0x8C180B0E,  //  0020  GETMET	R6	R5	K14
      0x58200006,  //  0021  LDCONST	R8	K6
      0x7C180400,  //  0022  CALL	R6	2
      0x90021A06,  //  0023  SETMBR	R0	K13	R6
      0x8C180B0E,  //  0024  GETMET	R6	R5	K14
      0x58200008,  //  0025  LDCONST	R8	K8
      0x7C180400,  //  0026  CALL	R6	2
      0x90021E06,  //  0027  SETMBR	R0	K15	R6
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
        { be_const_key_weak(parse, -1), be_const_closure(class_Matter_PBKDFParamRequest_parse_closure) },
        { be_const_key_weak(initiator_session_id, 0), be_const_var(1) },
        { be_const_key_weak(SLEEPY_IDLE_INTERVAL, 3), be_const_var(4) },
        { be_const_key_weak(SLEEPY_ACTIVE_INTERVAL, -1), be_const_var(5) },
    })),
    be_str_weak(Matter_PBKDFParamRequest)
);

extern const bclass be_class_Matter_PBKDFParamResponse;

/********************************************************************
** Solidified function: tlv2raw
********************************************************************/
be_local_closure(class_Matter_PBKDFParamResponse_tlv2raw,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[20]) {     /* constants */
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
    /* K18  */  be_const_int(17039616),
    /* K19  */  be_nested_str_weak(tlv2raw),
    }),
    be_str_weak(tlv2raw),
    &be_const_str_solidified,
    ( &(const binstruction[74]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x88080501,  //  0001  GETMBR	R2	R2	K1
      0x8C0C0502,  //  0002  GETMET	R3	R2	K2
      0x7C0C0200,  //  0003  CALL	R3	1
      0x8C100703,  //  0004  GETMET	R4	R3	K3
      0x58180004,  //  0005  LDCONST	R6	K4
      0x881C0505,  //  0006  GETMBR	R7	R2	K5
      0x88200106,  //  0007  GETMBR	R8	R0	K6
      0x7C100800,  //  0008  CALL	R4	4
      0x8C100703,  //  0009  GETMET	R4	R3	K3
      0x58180007,  //  000A  LDCONST	R6	K7
      0x881C0505,  //  000B  GETMBR	R7	R2	K5
      0x88200108,  //  000C  GETMBR	R8	R0	K8
      0x7C100800,  //  000D  CALL	R4	4
      0x8C100703,  //  000E  GETMET	R4	R3	K3
      0x58180009,  //  000F  LDCONST	R6	K9
      0x881C050A,  //  0010  GETMBR	R7	R2	K10
      0x8820010B,  //  0011  GETMBR	R8	R0	K11
      0x7C100800,  //  0012  CALL	R4	4
      0x8C10070C,  //  0013  GETMET	R4	R3	K12
      0x541A0003,  //  0014  LDINT	R6	4
      0x7C100400,  //  0015  CALL	R4	2
      0x8C140903,  //  0016  GETMET	R5	R4	K3
      0x581C0004,  //  0017  LDCONST	R7	K4
      0x8820050D,  //  0018  GETMBR	R8	R2	K13
      0x8824010E,  //  0019  GETMBR	R9	R0	K14
      0x7C140800,  //  001A  CALL	R5	4
      0x8C140903,  //  001B  GETMET	R5	R4	K3
      0x581C0007,  //  001C  LDCONST	R7	K7
      0x88200505,  //  001D  GETMBR	R8	R2	K5
      0x8824010F,  //  001E  GETMBR	R9	R0	K15
      0x7C140800,  //  001F  CALL	R5	4
      0x8C14070C,  //  0020  GETMET	R5	R3	K12
      0x541E0004,  //  0021  LDINT	R7	5
      0x7C140400,  //  0022  CALL	R5	2
      0x8C180B03,  //  0023  GETMET	R6	R5	K3
      0x58200004,  //  0024  LDCONST	R8	K4
      0x8824050D,  //  0025  GETMBR	R9	R2	K13
      0x88280110,  //  0026  GETMBR	R10	R0	K16
      0x7C180800,  //  0027  CALL	R6	4
      0x8C180B03,  //  0028  GETMET	R6	R5	K3
      0x58200007,  //  0029  LDCONST	R8	K7
      0x8824050D,  //  002A  GETMBR	R9	R2	K13
      0x88280111,  //  002B  GETMBR	R10	R0	K17
      0x7C180800,  //  002C  CALL	R6	4
      0x8C180B03,  //  002D  GETMET	R6	R5	K3
      0x58200009,  //  002E  LDCONST	R8	K9
      0x8824050A,  //  002F  GETMBR	R9	R2	K10
      0x542A0F9F,  //  0030  LDINT	R10	4000
      0x7C180800,  //  0031  CALL	R6	4
      0x8C180B03,  //  0032  GETMET	R6	R5	K3
      0x54220003,  //  0033  LDINT	R8	4
      0x8824050A,  //  0034  GETMBR	R9	R2	K10
      0x542A0011,  //  0035  LDINT	R10	18
      0x7C180800,  //  0036  CALL	R6	4
      0x8C180B03,  //  0037  GETMET	R6	R5	K3
      0x54220004,  //  0038  LDINT	R8	5
      0x8824050A,  //  0039  GETMBR	R9	R2	K10
      0x542A000B,  //  003A  LDINT	R10	12
      0x7C180800,  //  003B  CALL	R6	4
      0x8C180B03,  //  003C  GETMET	R6	R5	K3
      0x54220005,  //  003D  LDINT	R8	6
      0x8824050D,  //  003E  GETMBR	R9	R2	K13
      0x58280012,  //  003F  LDCONST	R10	K18
      0x7C180800,  //  0040  CALL	R6	4
      0x8C180B03,  //  0041  GETMET	R6	R5	K3
      0x54220006,  //  0042  LDINT	R8	7
      0x8824050A,  //  0043  GETMBR	R9	R2	K10
      0x58280004,  //  0044  LDCONST	R10	K4
      0x7C180800,  //  0045  CALL	R6	4
      0x8C180713,  //  0046  GETMET	R6	R3	K19
      0x5C200200,  //  0047  MOVE	R8	R1
      0x7C180400,  //  0048  CALL	R6	2
      0x80040C00,  //  0049  RET	1	R6
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
        { be_const_key_weak(tlv2raw, -1), be_const_closure(class_Matter_PBKDFParamResponse_tlv2raw_closure) },
    })),
    be_str_weak(Matter_PBKDFParamResponse)
);

extern const bclass be_class_Matter_Pake1;

/********************************************************************
** Solidified function: parse
********************************************************************/
be_local_closure(class_Matter_Pake1_parse,   /* name */
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
        { be_const_key_weak(parse, -1), be_const_closure(class_Matter_Pake1_parse_closure) },
    })),
    be_str_weak(Matter_Pake1)
);

extern const bclass be_class_Matter_Pake2;

/********************************************************************
** Solidified function: tlv2raw
********************************************************************/
be_local_closure(class_Matter_Pake2_tlv2raw,   /* name */
  be_nested_proto(
    9,                          /* nstack */
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
    ( &(const binstruction[18]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x88080501,  //  0001  GETMBR	R2	R2	K1
      0x8C0C0502,  //  0002  GETMET	R3	R2	K2
      0x7C0C0200,  //  0003  CALL	R3	1
      0x8C100703,  //  0004  GETMET	R4	R3	K3
      0x58180004,  //  0005  LDCONST	R6	K4
      0x881C0505,  //  0006  GETMBR	R7	R2	K5
      0x88200106,  //  0007  GETMBR	R8	R0	K6
      0x7C100800,  //  0008  CALL	R4	4
      0x8C100703,  //  0009  GETMET	R4	R3	K3
      0x58180007,  //  000A  LDCONST	R6	K7
      0x881C0505,  //  000B  GETMBR	R7	R2	K5
      0x88200108,  //  000C  GETMBR	R8	R0	K8
      0x7C100800,  //  000D  CALL	R4	4
      0x8C100709,  //  000E  GETMET	R4	R3	K9
      0x5C180200,  //  000F  MOVE	R6	R1
      0x7C100400,  //  0010  CALL	R4	2
      0x80040800,  //  0011  RET	1	R4
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
        { be_const_key_weak(tlv2raw, -1), be_const_closure(class_Matter_Pake2_tlv2raw_closure) },
        { be_const_key_weak(cB, -1), be_const_var(1) },
        { be_const_key_weak(pB, 0), be_const_var(0) },
    })),
    be_str_weak(Matter_Pake2)
);

extern const bclass be_class_Matter_Pake3;

/********************************************************************
** Solidified function: parse
********************************************************************/
be_local_closure(class_Matter_Pake3_parse,   /* name */
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
        { be_const_key_weak(parse, -1), be_const_closure(class_Matter_Pake3_parse_closure) },
        { be_const_key_weak(cA, -1), be_const_var(0) },
    })),
    be_str_weak(Matter_Pake3)
);

extern const bclass be_class_Matter_Sigma1;

/********************************************************************
** Solidified function: parse
********************************************************************/
be_local_closure(class_Matter_Sigma1_parse,   /* name */
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
        { be_const_key_weak(parse, 6), be_const_closure(class_Matter_Sigma1_parse_closure) },
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

extern const bclass be_class_Matter_Sigma2;

/********************************************************************
** Solidified function: tlv2raw
********************************************************************/
be_local_closure(class_Matter_Sigma2_tlv2raw,   /* name */
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
    /* K6   */  be_nested_str_weak(responderRandom),
    /* K7   */  be_const_int(2),
    /* K8   */  be_nested_str_weak(U2),
    /* K9   */  be_nested_str_weak(responderSessionId),
    /* K10  */  be_const_int(3),
    /* K11  */  be_nested_str_weak(responderEphPubKey),
    /* K12  */  be_nested_str_weak(encrypted2),
    /* K13  */  be_nested_str_weak(add_struct),
    /* K14  */  be_nested_str_weak(U4),
    /* K15  */  be_nested_str_weak(SLEEPY_IDLE_INTERVAL),
    /* K16  */  be_nested_str_weak(SLEEPY_ACTIVE_INTERVAL),
    /* K17  */  be_const_int(17039616),
    /* K18  */  be_nested_str_weak(tlv2raw),
    }),
    be_str_weak(tlv2raw),
    &be_const_str_solidified,
    ( &(const binstruction[66]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x88080501,  //  0001  GETMBR	R2	R2	K1
      0x8C0C0502,  //  0002  GETMET	R3	R2	K2
      0x7C0C0200,  //  0003  CALL	R3	1
      0x8C100703,  //  0004  GETMET	R4	R3	K3
      0x58180004,  //  0005  LDCONST	R6	K4
      0x881C0505,  //  0006  GETMBR	R7	R2	K5
      0x88200106,  //  0007  GETMBR	R8	R0	K6
      0x7C100800,  //  0008  CALL	R4	4
      0x8C100703,  //  0009  GETMET	R4	R3	K3
      0x58180007,  //  000A  LDCONST	R6	K7
      0x881C0508,  //  000B  GETMBR	R7	R2	K8
      0x88200109,  //  000C  GETMBR	R8	R0	K9
      0x7C100800,  //  000D  CALL	R4	4
      0x8C100703,  //  000E  GETMET	R4	R3	K3
      0x5818000A,  //  000F  LDCONST	R6	K10
      0x881C0505,  //  0010  GETMBR	R7	R2	K5
      0x8820010B,  //  0011  GETMBR	R8	R0	K11
      0x7C100800,  //  0012  CALL	R4	4
      0x8C100703,  //  0013  GETMET	R4	R3	K3
      0x541A0003,  //  0014  LDINT	R6	4
      0x881C0505,  //  0015  GETMBR	R7	R2	K5
      0x8820010C,  //  0016  GETMBR	R8	R0	K12
      0x7C100800,  //  0017  CALL	R4	4
      0x8C10070D,  //  0018  GETMET	R4	R3	K13
      0x541A0004,  //  0019  LDINT	R6	5
      0x7C100400,  //  001A  CALL	R4	2
      0x8C140903,  //  001B  GETMET	R5	R4	K3
      0x581C0004,  //  001C  LDCONST	R7	K4
      0x8820050E,  //  001D  GETMBR	R8	R2	K14
      0x8824010F,  //  001E  GETMBR	R9	R0	K15
      0x7C140800,  //  001F  CALL	R5	4
      0x8C140903,  //  0020  GETMET	R5	R4	K3
      0x581C0007,  //  0021  LDCONST	R7	K7
      0x8820050E,  //  0022  GETMBR	R8	R2	K14
      0x88240110,  //  0023  GETMBR	R9	R0	K16
      0x7C140800,  //  0024  CALL	R5	4
      0x8C140903,  //  0025  GETMET	R5	R4	K3
      0x581C000A,  //  0026  LDCONST	R7	K10
      0x88200508,  //  0027  GETMBR	R8	R2	K8
      0x54260F9F,  //  0028  LDINT	R9	4000
      0x7C140800,  //  0029  CALL	R5	4
      0x8C140903,  //  002A  GETMET	R5	R4	K3
      0x541E0003,  //  002B  LDINT	R7	4
      0x88200508,  //  002C  GETMBR	R8	R2	K8
      0x54260011,  //  002D  LDINT	R9	18
      0x7C140800,  //  002E  CALL	R5	4
      0x8C140903,  //  002F  GETMET	R5	R4	K3
      0x541E0004,  //  0030  LDINT	R7	5
      0x88200508,  //  0031  GETMBR	R8	R2	K8
      0x5426000B,  //  0032  LDINT	R9	12
      0x7C140800,  //  0033  CALL	R5	4
      0x8C140903,  //  0034  GETMET	R5	R4	K3
      0x541E0005,  //  0035  LDINT	R7	6
      0x8820050E,  //  0036  GETMBR	R8	R2	K14
      0x58240011,  //  0037  LDCONST	R9	K17
      0x7C140800,  //  0038  CALL	R5	4
      0x8C140903,  //  0039  GETMET	R5	R4	K3
      0x541E0006,  //  003A  LDINT	R7	7
      0x88200508,  //  003B  GETMBR	R8	R2	K8
      0x58240004,  //  003C  LDCONST	R9	K4
      0x7C140800,  //  003D  CALL	R5	4
      0x8C140712,  //  003E  GETMET	R5	R3	K18
      0x5C1C0200,  //  003F  MOVE	R7	R1
      0x7C140400,  //  0040  CALL	R5	2
      0x80040A00,  //  0041  RET	1	R5
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
        { be_const_key_weak(tlv2raw, -1), be_const_closure(class_Matter_Sigma2_tlv2raw_closure) },
        { be_const_key_weak(responderEphPubKey, 3), be_const_var(2) },
        { be_const_key_weak(responderSessionId, -1), be_const_var(1) },
        { be_const_key_weak(SLEEPY_IDLE_INTERVAL, -1), be_const_var(4) },
        { be_const_key_weak(SLEEPY_ACTIVE_INTERVAL, 6), be_const_var(5) },
        { be_const_key_weak(responderRandom, 1), be_const_var(0) },
        { be_const_key_weak(encrypted2, -1), be_const_var(3) },
    })),
    be_str_weak(Matter_Sigma2)
);

extern const bclass be_class_Matter_Sigma2Resume;

/********************************************************************
** Solidified function: tlv2raw
********************************************************************/
be_local_closure(class_Matter_Sigma2Resume_tlv2raw,   /* name */
  be_nested_proto(
    10,                          /* nstack */
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
    /* K6   */  be_nested_str_weak(resumptionID),
    /* K7   */  be_const_int(2),
    /* K8   */  be_nested_str_weak(sigma2ResumeMIC),
    /* K9   */  be_const_int(3),
    /* K10  */  be_nested_str_weak(U2),
    /* K11  */  be_nested_str_weak(responderSessionID),
    /* K12  */  be_nested_str_weak(add_struct),
    /* K13  */  be_nested_str_weak(U4),
    /* K14  */  be_nested_str_weak(SLEEPY_IDLE_INTERVAL),
    /* K15  */  be_nested_str_weak(SLEEPY_ACTIVE_INTERVAL),
    /* K16  */  be_const_int(17039616),
    /* K17  */  be_nested_str_weak(tlv2raw),
    }),
    be_str_weak(tlv2raw),
    &be_const_str_solidified,
    ( &(const binstruction[61]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x88080501,  //  0001  GETMBR	R2	R2	K1
      0x8C0C0502,  //  0002  GETMET	R3	R2	K2
      0x7C0C0200,  //  0003  CALL	R3	1
      0x8C100703,  //  0004  GETMET	R4	R3	K3
      0x58180004,  //  0005  LDCONST	R6	K4
      0x881C0505,  //  0006  GETMBR	R7	R2	K5
      0x88200106,  //  0007  GETMBR	R8	R0	K6
      0x7C100800,  //  0008  CALL	R4	4
      0x8C100703,  //  0009  GETMET	R4	R3	K3
      0x58180007,  //  000A  LDCONST	R6	K7
      0x881C0505,  //  000B  GETMBR	R7	R2	K5
      0x88200108,  //  000C  GETMBR	R8	R0	K8
      0x7C100800,  //  000D  CALL	R4	4
      0x8C100703,  //  000E  GETMET	R4	R3	K3
      0x58180009,  //  000F  LDCONST	R6	K9
      0x881C050A,  //  0010  GETMBR	R7	R2	K10
      0x8820010B,  //  0011  GETMBR	R8	R0	K11
      0x7C100800,  //  0012  CALL	R4	4
      0x8C10070C,  //  0013  GETMET	R4	R3	K12
      0x541A0003,  //  0014  LDINT	R6	4
      0x7C100400,  //  0015  CALL	R4	2
      0x8C140903,  //  0016  GETMET	R5	R4	K3
      0x581C0004,  //  0017  LDCONST	R7	K4
      0x8820050D,  //  0018  GETMBR	R8	R2	K13
      0x8824010E,  //  0019  GETMBR	R9	R0	K14
      0x7C140800,  //  001A  CALL	R5	4
      0x8C140903,  //  001B  GETMET	R5	R4	K3
      0x581C0007,  //  001C  LDCONST	R7	K7
      0x8820050D,  //  001D  GETMBR	R8	R2	K13
      0x8824010F,  //  001E  GETMBR	R9	R0	K15
      0x7C140800,  //  001F  CALL	R5	4
      0x8C140903,  //  0020  GETMET	R5	R4	K3
      0x581C0009,  //  0021  LDCONST	R7	K9
      0x8820050A,  //  0022  GETMBR	R8	R2	K10
      0x54260F9F,  //  0023  LDINT	R9	4000
      0x7C140800,  //  0024  CALL	R5	4
      0x8C140903,  //  0025  GETMET	R5	R4	K3
      0x541E0003,  //  0026  LDINT	R7	4
      0x8820050A,  //  0027  GETMBR	R8	R2	K10
      0x54260011,  //  0028  LDINT	R9	18
      0x7C140800,  //  0029  CALL	R5	4
      0x8C140903,  //  002A  GETMET	R5	R4	K3
      0x541E0004,  //  002B  LDINT	R7	5
      0x8820050A,  //  002C  GETMBR	R8	R2	K10
      0x5426000B,  //  002D  LDINT	R9	12
      0x7C140800,  //  002E  CALL	R5	4
      0x8C140903,  //  002F  GETMET	R5	R4	K3
      0x541E0005,  //  0030  LDINT	R7	6
      0x8820050D,  //  0031  GETMBR	R8	R2	K13
      0x58240010,  //  0032  LDCONST	R9	K16
      0x7C140800,  //  0033  CALL	R5	4
      0x8C140903,  //  0034  GETMET	R5	R4	K3
      0x541E0006,  //  0035  LDINT	R7	7
      0x8820050A,  //  0036  GETMBR	R8	R2	K10
      0x58240004,  //  0037  LDCONST	R9	K4
      0x7C140800,  //  0038  CALL	R5	4
      0x8C140711,  //  0039  GETMET	R5	R3	K17
      0x5C1C0200,  //  003A  MOVE	R7	R1
      0x7C140400,  //  003B  CALL	R5	2
      0x80040A00,  //  003C  RET	1	R5
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
        { be_const_key_weak(tlv2raw, -1), be_const_closure(class_Matter_Sigma2Resume_tlv2raw_closure) },
    })),
    be_str_weak(Matter_Sigma2Resume)
);

extern const bclass be_class_Matter_Sigma3;

/********************************************************************
** Solidified function: parse
********************************************************************/
be_local_closure(class_Matter_Sigma3_parse,   /* name */
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
        { be_const_key_weak(parse, -1), be_const_closure(class_Matter_Sigma3_parse_closure) },
        { be_const_key_weak(Msg3, -1), be_const_var(1) },
    })),
    be_str_weak(Matter_Sigma3)
);
/********************************************************************/
/* End of solidification */
