/* Solidification of crypto_spake2p_matter.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_SPAKE2P_Matter;

/********************************************************************
** Solidified function: compute_pB
********************************************************************/
be_local_closure(SPAKE2P_Matter_compute_pB,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(random),
    /* K2   */  be_nested_str_weak(EC_P256),
    /* K3   */  be_nested_str_weak(y),
    /* K4   */  be_nested_str_weak(mod),
    /* K5   */  be_nested_str_weak(pB),
    /* K6   */  be_nested_str_weak(muladd),
    /* K7   */  be_nested_str_weak(w0),
    /* K8   */  be_nested_str_weak(N),
    }),
    be_str_weak(compute_pB),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x1C0C0203,  //  0002  EQ	R3	R1	R3
      0x780E0003,  //  0003  JMPF	R3	#0008
      0x8C0C0501,  //  0004  GETMET	R3	R2	K1
      0x5416001F,  //  0005  LDINT	R5	32
      0x7C0C0400,  //  0006  CALL	R3	2
      0x5C040600,  //  0007  MOVE	R1	R3
      0x8C0C0502,  //  0008  GETMET	R3	R2	K2
      0x7C0C0200,  //  0009  CALL	R3	1
      0x8C100704,  //  000A  GETMET	R4	R3	K4
      0x5C180200,  //  000B  MOVE	R6	R1
      0x7C100400,  //  000C  CALL	R4	2
      0x90020604,  //  000D  SETMBR	R0	K3	R4
      0x8C100706,  //  000E  GETMET	R4	R3	K6
      0x88180107,  //  000F  GETMBR	R6	R0	K7
      0x881C0108,  //  0010  GETMBR	R7	R0	K8
      0x88200103,  //  0011  GETMBR	R8	R0	K3
      0x60240015,  //  0012  GETGBL	R9	G21
      0x7C240000,  //  0013  CALL	R9	0
      0x7C100A00,  //  0014  CALL	R4	5
      0x90020A04,  //  0015  SETMBR	R0	K5	R4
      0x88100105,  //  0016  GETMBR	R4	R0	K5
      0x80040800,  //  0017  RET	1	R4
    })
  )
);
/*******************************************************************/


extern const bclass be_class_SPAKE_Hasher;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(SPAKE_Hasher_init,   /* name */
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
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(hash),
    /* K2   */  be_nested_str_weak(SHA256),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080302,  //  0001  GETMET	R2	R1	K2
      0x7C080200,  //  0002  CALL	R2	1
      0x90020202,  //  0003  SETMBR	R0	K1	R2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_item
********************************************************************/
be_local_closure(SPAKE_Hasher_add_item,   /* name */
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
    /* K0   */  be_nested_str_weak(size),
    /* K1   */  be_nested_str_weak(add),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(hash),
    /* K4   */  be_nested_str_weak(update),
    }),
    be_str_weak(add_item),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x8C080300,  //  0000  GETMET	R2	R1	K0
      0x7C080200,  //  0001  CALL	R2	1
      0x600C0015,  //  0002  GETGBL	R3	G21
      0x7C0C0000,  //  0003  CALL	R3	0
      0x8C0C0701,  //  0004  GETMET	R3	R3	K1
      0x5C140400,  //  0005  MOVE	R5	R2
      0x541A0003,  //  0006  LDINT	R6	4
      0x7C0C0600,  //  0007  CALL	R3	3
      0x8C0C0701,  //  0008  GETMET	R3	R3	K1
      0x58140002,  //  0009  LDCONST	R5	K2
      0x541A0003,  //  000A  LDINT	R6	4
      0x7C0C0600,  //  000B  CALL	R3	3
      0x88100103,  //  000C  GETMBR	R4	R0	K3
      0x8C100904,  //  000D  GETMET	R4	R4	K4
      0x5C180600,  //  000E  MOVE	R6	R3
      0x7C100400,  //  000F  CALL	R4	2
      0x88100103,  //  0010  GETMBR	R4	R0	K3
      0x8C100904,  //  0011  GETMET	R4	R4	K4
      0x5C180200,  //  0012  MOVE	R6	R1
      0x7C100400,  //  0013  CALL	R4	2
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: out
********************************************************************/
be_local_closure(SPAKE_Hasher_out,   /* name */
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
    /* K0   */  be_nested_str_weak(hash),
    /* K1   */  be_nested_str_weak(out),
    }),
    be_str_weak(out),
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
** Solidified class: SPAKE_Hasher
********************************************************************/
be_local_class(SPAKE_Hasher,
    1,
    NULL,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(out, -1), be_const_closure(SPAKE_Hasher_out_closure) },
        { be_const_key_weak(hash, -1), be_const_var(0) },
        { be_const_key_weak(add_item, -1), be_const_closure(SPAKE_Hasher_add_item_closure) },
        { be_const_key_weak(init, 0), be_const_closure(SPAKE_Hasher_init_closure) },
    })),
    be_str_weak(SPAKE_Hasher)
);

/********************************************************************
** Solidified function: compute_TT_hash
********************************************************************/
be_local_closure(SPAKE2P_Matter_compute_TT_hash,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[29]) {     /* constants */
    /* K0   */  be_const_class(be_class_SPAKE_Hasher),
    /* K1   */  be_nested_str_weak(crypto),
    /* K2   */  be_nested_str_weak(add_item),
    /* K3   */  be_nested_str_weak(Context),
    /* K4   */  be_nested_str_weak(A),
    /* K5   */  be_nested_str_weak(B),
    /* K6   */  be_nested_str_weak(M),
    /* K7   */  be_nested_str_weak(N),
    /* K8   */  be_nested_str_weak(pA),
    /* K9   */  be_nested_str_weak(pB),
    /* K10  */  be_nested_str_weak(Z),
    /* K11  */  be_nested_str_weak(V),
    /* K12  */  be_nested_str_weak(w0),
    /* K13  */  be_nested_str_weak(Kmain),
    /* K14  */  be_nested_str_weak(out),
    /* K15  */  be_nested_str_weak(Ke),
    /* K16  */  be_const_int(0),
    /* K17  */  be_nested_str_weak(HKDF_SHA256),
    /* K18  */  be_nested_str_weak(derive),
    /* K19  */  be_nested_str_weak(fromstring),
    /* K20  */  be_nested_str_weak(ConfirmationKeys),
    /* K21  */  be_nested_str_weak(KcA),
    /* K22  */  be_nested_str_weak(KcB),
    /* K23  */  be_nested_str_weak(K_shared),
    /* K24  */  be_nested_str_weak(SharedKey),
    /* K25  */  be_nested_str_weak(cA),
    /* K26  */  be_nested_str_weak(HMAC_SHA256),
    /* K27  */  be_nested_str_weak(update),
    /* K28  */  be_nested_str_weak(cB),
    }),
    be_str_weak(compute_TT_hash),
    &be_const_str_solidified,
    ( &(const binstruction[115]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0xB4000000,  //  0001  CLASS	K0
      0x5C0C0400,  //  0002  MOVE	R3	R2
      0xA40E0200,  //  0003  IMPORT	R3	K1
      0x5C100400,  //  0004  MOVE	R4	R2
      0x7C100000,  //  0005  CALL	R4	0
      0x8C140902,  //  0006  GETMET	R5	R4	K2
      0x881C0103,  //  0007  GETMBR	R7	R0	K3
      0x7C140400,  //  0008  CALL	R5	2
      0x8C140902,  //  0009  GETMET	R5	R4	K2
      0x881C0104,  //  000A  GETMBR	R7	R0	K4
      0x7C140400,  //  000B  CALL	R5	2
      0x8C140902,  //  000C  GETMET	R5	R4	K2
      0x881C0105,  //  000D  GETMBR	R7	R0	K5
      0x7C140400,  //  000E  CALL	R5	2
      0x8C140902,  //  000F  GETMET	R5	R4	K2
      0x881C0106,  //  0010  GETMBR	R7	R0	K6
      0x7C140400,  //  0011  CALL	R5	2
      0x8C140902,  //  0012  GETMET	R5	R4	K2
      0x881C0107,  //  0013  GETMBR	R7	R0	K7
      0x7C140400,  //  0014  CALL	R5	2
      0x8C140902,  //  0015  GETMET	R5	R4	K2
      0x881C0108,  //  0016  GETMBR	R7	R0	K8
      0x7C140400,  //  0017  CALL	R5	2
      0x8C140902,  //  0018  GETMET	R5	R4	K2
      0x881C0109,  //  0019  GETMBR	R7	R0	K9
      0x7C140400,  //  001A  CALL	R5	2
      0x8C140902,  //  001B  GETMET	R5	R4	K2
      0x881C010A,  //  001C  GETMBR	R7	R0	K10
      0x7C140400,  //  001D  CALL	R5	2
      0x8C140902,  //  001E  GETMET	R5	R4	K2
      0x881C010B,  //  001F  GETMBR	R7	R0	K11
      0x7C140400,  //  0020  CALL	R5	2
      0x8C140902,  //  0021  GETMET	R5	R4	K2
      0x881C010C,  //  0022  GETMBR	R7	R0	K12
      0x7C140400,  //  0023  CALL	R5	2
      0x8C14090E,  //  0024  GETMET	R5	R4	K14
      0x7C140200,  //  0025  CALL	R5	1
      0x90021A05,  //  0026  SETMBR	R0	K13	R5
      0x7806000A,  //  0027  JMPF	R1	#0033
      0x5416000F,  //  0028  LDINT	R5	16
      0x541A001E,  //  0029  LDINT	R6	31
      0x40140A06,  //  002A  CONNECT	R5	R5	R6
      0x8818010D,  //  002B  GETMBR	R6	R0	K13
      0x94140C05,  //  002C  GETIDX	R5	R6	R5
      0x90021E05,  //  002D  SETMBR	R0	K15	R5
      0x5416000E,  //  002E  LDINT	R5	15
      0x40162005,  //  002F  CONNECT	R5	K16	R5
      0x8818010D,  //  0030  GETMBR	R6	R0	K13
      0x94140C05,  //  0031  GETIDX	R5	R6	R5
      0x90021A05,  //  0032  SETMBR	R0	K13	R5
      0x8C140711,  //  0033  GETMET	R5	R3	K17
      0x7C140200,  //  0034  CALL	R5	1
      0x8C180B12,  //  0035  GETMET	R6	R5	K18
      0x8820010D,  //  0036  GETMBR	R8	R0	K13
      0x60240015,  //  0037  GETGBL	R9	G21
      0x7C240000,  //  0038  CALL	R9	0
      0x60280015,  //  0039  GETGBL	R10	G21
      0x7C280000,  //  003A  CALL	R10	0
      0x8C281513,  //  003B  GETMET	R10	R10	K19
      0x58300014,  //  003C  LDCONST	R12	K20
      0x7C280400,  //  003D  CALL	R10	2
      0x542E003F,  //  003E  LDINT	R11	64
      0x7C180A00,  //  003F  CALL	R6	5
      0x78060003,  //  0040  JMPF	R1	#0045
      0x541E000E,  //  0041  LDINT	R7	15
      0x401E2007,  //  0042  CONNECT	R7	K16	R7
      0x941C0C07,  //  0043  GETIDX	R7	R6	R7
      0x70020002,  //  0044  JMP		#0048
      0x541E001E,  //  0045  LDINT	R7	31
      0x401E2007,  //  0046  CONNECT	R7	K16	R7
      0x941C0C07,  //  0047  GETIDX	R7	R6	R7
      0x90022A07,  //  0048  SETMBR	R0	K21	R7
      0x78060004,  //  0049  JMPF	R1	#004F
      0x541E000F,  //  004A  LDINT	R7	16
      0x5422001E,  //  004B  LDINT	R8	31
      0x401C0E08,  //  004C  CONNECT	R7	R7	R8
      0x941C0C07,  //  004D  GETIDX	R7	R6	R7
      0x70020003,  //  004E  JMP		#0053
      0x541E001F,  //  004F  LDINT	R7	32
      0x5422003E,  //  0050  LDINT	R8	63
      0x401C0E08,  //  0051  CONNECT	R7	R7	R8
      0x941C0C07,  //  0052  GETIDX	R7	R6	R7
      0x90022C07,  //  0053  SETMBR	R0	K22	R7
      0x8C1C0B12,  //  0054  GETMET	R7	R5	K18
      0x8824010D,  //  0055  GETMBR	R9	R0	K13
      0x60280015,  //  0056  GETGBL	R10	G21
      0x7C280000,  //  0057  CALL	R10	0
      0x602C0015,  //  0058  GETGBL	R11	G21
      0x7C2C0000,  //  0059  CALL	R11	0
      0x8C2C1713,  //  005A  GETMET	R11	R11	K19
      0x58340018,  //  005B  LDCONST	R13	K24
      0x7C2C0400,  //  005C  CALL	R11	2
      0x5432001F,  //  005D  LDINT	R12	32
      0x7C1C0A00,  //  005E  CALL	R7	5
      0x90022E07,  //  005F  SETMBR	R0	K23	R7
      0x8C1C071A,  //  0060  GETMET	R7	R3	K26
      0x88240115,  //  0061  GETMBR	R9	R0	K21
      0x7C1C0400,  //  0062  CALL	R7	2
      0x8C1C0F1B,  //  0063  GETMET	R7	R7	K27
      0x88240109,  //  0064  GETMBR	R9	R0	K9
      0x7C1C0400,  //  0065  CALL	R7	2
      0x8C1C0F0E,  //  0066  GETMET	R7	R7	K14
      0x7C1C0200,  //  0067  CALL	R7	1
      0x90023207,  //  0068  SETMBR	R0	K25	R7
      0x8C1C071A,  //  0069  GETMET	R7	R3	K26
      0x88240116,  //  006A  GETMBR	R9	R0	K22
      0x7C1C0400,  //  006B  CALL	R7	2
      0x8C1C0F1B,  //  006C  GETMET	R7	R7	K27
      0x88240108,  //  006D  GETMBR	R9	R0	K8
      0x7C1C0400,  //  006E  CALL	R7	2
      0x8C1C0F0E,  //  006F  GETMET	R7	R7	K14
      0x7C1C0200,  //  0070  CALL	R7	1
      0x90023807,  //  0071  SETMBR	R0	K28	R7
      0x80000000,  //  0072  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(SPAKE2P_Matter_init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(M),
    /* K1   */  be_nested_str_weak(fromhex),
    /* K2   */  be_nested_str_weak(spake_M_hex),
    /* K3   */  be_nested_str_weak(N),
    /* K4   */  be_nested_str_weak(spake_N_hex),
    /* K5   */  be_nested_str_weak(w0),
    /* K6   */  be_nested_str_weak(w1),
    /* K7   */  be_nested_str_weak(L),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x60100015,  //  0000  GETGBL	R4	G21
      0x7C100000,  //  0001  CALL	R4	0
      0x8C100901,  //  0002  GETMET	R4	R4	K1
      0x88180102,  //  0003  GETMBR	R6	R0	K2
      0x7C100400,  //  0004  CALL	R4	2
      0x90020004,  //  0005  SETMBR	R0	K0	R4
      0x60100015,  //  0006  GETGBL	R4	G21
      0x7C100000,  //  0007  CALL	R4	0
      0x8C100901,  //  0008  GETMET	R4	R4	K1
      0x88180104,  //  0009  GETMBR	R6	R0	K4
      0x7C100400,  //  000A  CALL	R4	2
      0x90020604,  //  000B  SETMBR	R0	K3	R4
      0x90020A01,  //  000C  SETMBR	R0	K5	R1
      0x90020C02,  //  000D  SETMBR	R0	K6	R2
      0x90020E03,  //  000E  SETMBR	R0	K7	R3
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: compute_pA
********************************************************************/
be_local_closure(SPAKE2P_Matter_compute_pA,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(random),
    /* K2   */  be_nested_str_weak(EC_P256),
    /* K3   */  be_nested_str_weak(x),
    /* K4   */  be_nested_str_weak(mod),
    /* K5   */  be_nested_str_weak(pA),
    /* K6   */  be_nested_str_weak(muladd),
    /* K7   */  be_nested_str_weak(w0),
    /* K8   */  be_nested_str_weak(M),
    }),
    be_str_weak(compute_pA),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x1C0C0203,  //  0002  EQ	R3	R1	R3
      0x780E0003,  //  0003  JMPF	R3	#0008
      0x8C0C0501,  //  0004  GETMET	R3	R2	K1
      0x5416001F,  //  0005  LDINT	R5	32
      0x7C0C0400,  //  0006  CALL	R3	2
      0x5C040600,  //  0007  MOVE	R1	R3
      0x8C0C0502,  //  0008  GETMET	R3	R2	K2
      0x7C0C0200,  //  0009  CALL	R3	1
      0x8C100704,  //  000A  GETMET	R4	R3	K4
      0x5C180200,  //  000B  MOVE	R6	R1
      0x7C100400,  //  000C  CALL	R4	2
      0x90020604,  //  000D  SETMBR	R0	K3	R4
      0x8C100706,  //  000E  GETMET	R4	R3	K6
      0x88180107,  //  000F  GETMBR	R6	R0	K7
      0x881C0108,  //  0010  GETMBR	R7	R0	K8
      0x88200103,  //  0011  GETMBR	R8	R0	K3
      0x60240015,  //  0012  GETGBL	R9	G21
      0x7C240000,  //  0013  CALL	R9	0
      0x7C100A00,  //  0014  CALL	R4	5
      0x90020A04,  //  0015  SETMBR	R0	K5	R4
      0x88100105,  //  0016  GETMBR	R4	R0	K5
      0x80040800,  //  0017  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_context
********************************************************************/
be_local_closure(SPAKE2P_Matter_set_context,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(Context),
    /* K1   */  be_nested_str_weak(A),
    /* K2   */  be_nested_str_weak(B),
    }),
    be_str_weak(set_context),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x4C100000,  //  0000  LDNIL	R4
      0x1C100404,  //  0001  EQ	R4	R2	R4
      0x78120002,  //  0002  JMPF	R4	#0006
      0x60100015,  //  0003  GETGBL	R4	G21
      0x7C100000,  //  0004  CALL	R4	0
      0x5C080800,  //  0005  MOVE	R2	R4
      0x4C100000,  //  0006  LDNIL	R4
      0x1C100604,  //  0007  EQ	R4	R3	R4
      0x78120002,  //  0008  JMPF	R4	#000C
      0x60100015,  //  0009  GETGBL	R4	G21
      0x7C100000,  //  000A  CALL	R4	0
      0x5C0C0800,  //  000B  MOVE	R3	R4
      0x90020001,  //  000C  SETMBR	R0	K0	R1
      0x90020202,  //  000D  SETMBR	R0	K1	R2
      0x90020403,  //  000E  SETMBR	R0	K2	R3
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: compute_ZV_verifier
********************************************************************/
be_local_closure(SPAKE2P_Matter_compute_ZV_verifier,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(EC_P256),
    /* K2   */  be_nested_str_weak(pA),
    /* K3   */  be_nested_str_weak(neg),
    /* K4   */  be_nested_str_weak(w0),
    /* K5   */  be_nested_str_weak(muladd),
    /* K6   */  be_nested_str_weak(01),
    /* K7   */  be_nested_str_weak(M),
    /* K8   */  be_nested_str_weak(Z),
    /* K9   */  be_nested_str_weak(mul),
    /* K10  */  be_nested_str_weak(y),
    /* K11  */  be_nested_str_weak(V),
    /* K12  */  be_nested_str_weak(L),
    }),
    be_str_weak(compute_ZV_verifier),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0501,  //  0001  GETMET	R3	R2	K1
      0x7C0C0200,  //  0002  CALL	R3	1
      0x90020401,  //  0003  SETMBR	R0	K2	R1
      0x8C100703,  //  0004  GETMET	R4	R3	K3
      0x88180104,  //  0005  GETMBR	R6	R0	K4
      0x7C100400,  //  0006  CALL	R4	2
      0x8C140705,  //  0007  GETMET	R5	R3	K5
      0x601C0015,  //  0008  GETGBL	R7	G21
      0x58200006,  //  0009  LDCONST	R8	K6
      0x7C1C0200,  //  000A  CALL	R7	1
      0x88200102,  //  000B  GETMBR	R8	R0	K2
      0x5C240800,  //  000C  MOVE	R9	R4
      0x88280107,  //  000D  GETMBR	R10	R0	K7
      0x7C140A00,  //  000E  CALL	R5	5
      0x8C180709,  //  000F  GETMET	R6	R3	K9
      0x8820010A,  //  0010  GETMBR	R8	R0	K10
      0x5C240A00,  //  0011  MOVE	R9	R5
      0x7C180600,  //  0012  CALL	R6	3
      0x90021006,  //  0013  SETMBR	R0	K8	R6
      0x8C180709,  //  0014  GETMET	R6	R3	K9
      0x8820010A,  //  0015  GETMBR	R8	R0	K10
      0x8824010C,  //  0016  GETMBR	R9	R0	K12
      0x7C180600,  //  0017  CALL	R6	3
      0x90021606,  //  0018  SETMBR	R0	K11	R6
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: compute_ZV_prover
********************************************************************/
be_local_closure(SPAKE2P_Matter_compute_ZV_prover,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(EC_P256),
    /* K2   */  be_nested_str_weak(pB),
    /* K3   */  be_nested_str_weak(neg),
    /* K4   */  be_nested_str_weak(w0),
    /* K5   */  be_nested_str_weak(muladd),
    /* K6   */  be_nested_str_weak(01),
    /* K7   */  be_nested_str_weak(N),
    /* K8   */  be_nested_str_weak(Z),
    /* K9   */  be_nested_str_weak(mul),
    /* K10  */  be_nested_str_weak(x),
    /* K11  */  be_nested_str_weak(V),
    /* K12  */  be_nested_str_weak(w1),
    }),
    be_str_weak(compute_ZV_prover),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0501,  //  0001  GETMET	R3	R2	K1
      0x7C0C0200,  //  0002  CALL	R3	1
      0x90020401,  //  0003  SETMBR	R0	K2	R1
      0x8C100703,  //  0004  GETMET	R4	R3	K3
      0x88180104,  //  0005  GETMBR	R6	R0	K4
      0x7C100400,  //  0006  CALL	R4	2
      0x8C140705,  //  0007  GETMET	R5	R3	K5
      0x601C0015,  //  0008  GETGBL	R7	G21
      0x58200006,  //  0009  LDCONST	R8	K6
      0x7C1C0200,  //  000A  CALL	R7	1
      0x88200102,  //  000B  GETMBR	R8	R0	K2
      0x5C240800,  //  000C  MOVE	R9	R4
      0x88280107,  //  000D  GETMBR	R10	R0	K7
      0x7C140A00,  //  000E  CALL	R5	5
      0x8C180709,  //  000F  GETMET	R6	R3	K9
      0x8820010A,  //  0010  GETMBR	R8	R0	K10
      0x5C240A00,  //  0011  MOVE	R9	R5
      0x7C180600,  //  0012  CALL	R6	3
      0x90021006,  //  0013  SETMBR	R0	K8	R6
      0x8C180709,  //  0014  GETMET	R6	R3	K9
      0x8820010C,  //  0015  GETMBR	R8	R0	K12
      0x5C240A00,  //  0016  MOVE	R9	R5
      0x7C180600,  //  0017  CALL	R6	3
      0x90021606,  //  0018  SETMBR	R0	K11	R6
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: SPAKE2P_Matter
********************************************************************/
be_local_class(SPAKE2P_Matter,
    21,
    NULL,
    be_nested_map(32,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(compute_ZV_prover, 21), be_const_closure(SPAKE2P_Matter_compute_ZV_prover_closure) },
        { be_const_key_weak(cA, -1), be_const_var(14) },
        { be_const_key_weak(CRYPTO_W_SIZE_BYTES, -1), be_const_int(40) },
        { be_const_key_weak(Kmain, 14), be_const_var(9) },
        { be_const_key_weak(compute_pB, -1), be_const_closure(SPAKE2P_Matter_compute_pB_closure) },
        { be_const_key_weak(B, 31), be_const_var(17) },
        { be_const_key_weak(K_shared, -1), be_const_var(12) },
        { be_const_key_weak(x, -1), be_const_var(7) },
        { be_const_key_weak(CRYPTO_GROUP_SIZE_BYTES, -1), be_const_int(32) },
        { be_const_key_weak(V, 1), be_const_var(6) },
        { be_const_key_weak(pA, -1), be_const_var(3) },
        { be_const_key_weak(KcB, 27), be_const_var(11) },
        { be_const_key_weak(A, -1), be_const_var(16) },
        { be_const_key_weak(Z, 4), be_const_var(5) },
        { be_const_key_weak(spake_M_hex, -1), be_nested_str_weak(04886e2f97ace46e55ba9dd7242579f2993b64e16ef3dcab95afd497333d8fa12f5ff355163e43ce224e0b0e65ff02ac8e5c7be09419c785e0ca547d55a12e2d20) },
        { be_const_key_weak(compute_ZV_verifier, -1), be_const_closure(SPAKE2P_Matter_compute_ZV_verifier_closure) },
        { be_const_key_weak(compute_TT_hash, 0), be_const_closure(SPAKE2P_Matter_compute_TT_hash_closure) },
        { be_const_key_weak(N, -1), be_const_var(20) },
        { be_const_key_weak(w0, -1), be_const_var(0) },
        { be_const_key_weak(init, -1), be_const_closure(SPAKE2P_Matter_init_closure) },
        { be_const_key_weak(compute_pA, 25), be_const_closure(SPAKE2P_Matter_compute_pA_closure) },
        { be_const_key_weak(cB, -1), be_const_var(15) },
        { be_const_key_weak(set_context, -1), be_const_closure(SPAKE2P_Matter_set_context_closure) },
        { be_const_key_weak(pB, 22), be_const_var(4) },
        { be_const_key_weak(M, -1), be_const_var(19) },
        { be_const_key_weak(y, -1), be_const_var(8) },
        { be_const_key_weak(spake_N_hex, 15), be_nested_str_weak(04d8bbd6c639c62937b04d997f38c3770719c629d7014d49a24b4f98baa1292b4907d60aa6bfade45008a636337f5168c64d9bd36034808cd564490b1e656edbe7) },
        { be_const_key_weak(L, -1), be_const_var(2) },
        { be_const_key_weak(Context, 8), be_const_var(18) },
        { be_const_key_weak(Ke, -1), be_const_var(13) },
        { be_const_key_weak(KcA, -1), be_const_var(10) },
        { be_const_key_weak(w1, -1), be_const_var(1) },
    })),
    be_str_weak(SPAKE2P_Matter)
);
/*******************************************************************/

void be_load_SPAKE2P_Matter_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_SPAKE2P_Matter);
    be_setglobal(vm, "SPAKE2P_Matter");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
