/* Solidification of crypto_spake2p_matter.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

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
    13,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[28]) {     /* constants */
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
    /* K15  */  be_nested_str_weak(HKDF_SHA256),
    /* K16  */  be_nested_str_weak(derive),
    /* K17  */  be_nested_str_weak(fromstring),
    /* K18  */  be_nested_str_weak(ConfirmationKeys),
    /* K19  */  be_nested_str_weak(KcA),
    /* K20  */  be_const_int(0),
    /* K21  */  be_nested_str_weak(KcB),
    /* K22  */  be_nested_str_weak(K_shared),
    /* K23  */  be_nested_str_weak(SharedKey),
    /* K24  */  be_nested_str_weak(cA),
    /* K25  */  be_nested_str_weak(HMAC_SHA256),
    /* K26  */  be_nested_str_weak(update),
    /* K27  */  be_nested_str_weak(cB),
    }),
    be_str_weak(compute_TT_hash),
    &be_const_str_solidified,
    ( &(const binstruction[91]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0xB4000000,  //  0001  CLASS	K0
      0xA40A0200,  //  0002  IMPORT	R2	K1
      0x5C0C0200,  //  0003  MOVE	R3	R1
      0x7C0C0000,  //  0004  CALL	R3	0
      0x8C100702,  //  0005  GETMET	R4	R3	K2
      0x88180103,  //  0006  GETMBR	R6	R0	K3
      0x7C100400,  //  0007  CALL	R4	2
      0x8C100702,  //  0008  GETMET	R4	R3	K2
      0x88180104,  //  0009  GETMBR	R6	R0	K4
      0x7C100400,  //  000A  CALL	R4	2
      0x8C100702,  //  000B  GETMET	R4	R3	K2
      0x88180105,  //  000C  GETMBR	R6	R0	K5
      0x7C100400,  //  000D  CALL	R4	2
      0x8C100702,  //  000E  GETMET	R4	R3	K2
      0x88180106,  //  000F  GETMBR	R6	R0	K6
      0x7C100400,  //  0010  CALL	R4	2
      0x8C100702,  //  0011  GETMET	R4	R3	K2
      0x88180107,  //  0012  GETMBR	R6	R0	K7
      0x7C100400,  //  0013  CALL	R4	2
      0x8C100702,  //  0014  GETMET	R4	R3	K2
      0x88180108,  //  0015  GETMBR	R6	R0	K8
      0x7C100400,  //  0016  CALL	R4	2
      0x8C100702,  //  0017  GETMET	R4	R3	K2
      0x88180109,  //  0018  GETMBR	R6	R0	K9
      0x7C100400,  //  0019  CALL	R4	2
      0x8C100702,  //  001A  GETMET	R4	R3	K2
      0x8818010A,  //  001B  GETMBR	R6	R0	K10
      0x7C100400,  //  001C  CALL	R4	2
      0x8C100702,  //  001D  GETMET	R4	R3	K2
      0x8818010B,  //  001E  GETMBR	R6	R0	K11
      0x7C100400,  //  001F  CALL	R4	2
      0x8C100702,  //  0020  GETMET	R4	R3	K2
      0x8818010C,  //  0021  GETMBR	R6	R0	K12
      0x7C100400,  //  0022  CALL	R4	2
      0x8C10070E,  //  0023  GETMET	R4	R3	K14
      0x7C100200,  //  0024  CALL	R4	1
      0x90021A04,  //  0025  SETMBR	R0	K13	R4
      0x8C10050F,  //  0026  GETMET	R4	R2	K15
      0x7C100200,  //  0027  CALL	R4	1
      0x8C140910,  //  0028  GETMET	R5	R4	K16
      0x881C010D,  //  0029  GETMBR	R7	R0	K13
      0x60200015,  //  002A  GETGBL	R8	G21
      0x7C200000,  //  002B  CALL	R8	0
      0x60240015,  //  002C  GETGBL	R9	G21
      0x7C240000,  //  002D  CALL	R9	0
      0x8C241311,  //  002E  GETMET	R9	R9	K17
      0x582C0012,  //  002F  LDCONST	R11	K18
      0x7C240400,  //  0030  CALL	R9	2
      0x542A003F,  //  0031  LDINT	R10	64
      0x7C140A00,  //  0032  CALL	R5	5
      0x541A001E,  //  0033  LDINT	R6	31
      0x401A2806,  //  0034  CONNECT	R6	K20	R6
      0x94180A06,  //  0035  GETIDX	R6	R5	R6
      0x90022606,  //  0036  SETMBR	R0	K19	R6
      0x541A001F,  //  0037  LDINT	R6	32
      0x541E003E,  //  0038  LDINT	R7	63
      0x40180C07,  //  0039  CONNECT	R6	R6	R7
      0x94180A06,  //  003A  GETIDX	R6	R5	R6
      0x90022A06,  //  003B  SETMBR	R0	K21	R6
      0x8C180910,  //  003C  GETMET	R6	R4	K16
      0x8820010D,  //  003D  GETMBR	R8	R0	K13
      0x60240015,  //  003E  GETGBL	R9	G21
      0x7C240000,  //  003F  CALL	R9	0
      0x60280015,  //  0040  GETGBL	R10	G21
      0x7C280000,  //  0041  CALL	R10	0
      0x8C281511,  //  0042  GETMET	R10	R10	K17
      0x58300017,  //  0043  LDCONST	R12	K23
      0x7C280400,  //  0044  CALL	R10	2
      0x542E001F,  //  0045  LDINT	R11	32
      0x7C180A00,  //  0046  CALL	R6	5
      0x90022C06,  //  0047  SETMBR	R0	K22	R6
      0x8C180519,  //  0048  GETMET	R6	R2	K25
      0x88200113,  //  0049  GETMBR	R8	R0	K19
      0x7C180400,  //  004A  CALL	R6	2
      0x8C180D1A,  //  004B  GETMET	R6	R6	K26
      0x88200109,  //  004C  GETMBR	R8	R0	K9
      0x7C180400,  //  004D  CALL	R6	2
      0x8C180D0E,  //  004E  GETMET	R6	R6	K14
      0x7C180200,  //  004F  CALL	R6	1
      0x90023006,  //  0050  SETMBR	R0	K24	R6
      0x8C180519,  //  0051  GETMET	R6	R2	K25
      0x88200115,  //  0052  GETMBR	R8	R0	K21
      0x7C180400,  //  0053  CALL	R6	2
      0x8C180D1A,  //  0054  GETMET	R6	R6	K26
      0x88200108,  //  0055  GETMBR	R8	R0	K8
      0x7C180400,  //  0056  CALL	R6	2
      0x8C180D0E,  //  0057  GETMET	R6	R6	K14
      0x7C180200,  //  0058  CALL	R6	1
      0x90023606,  //  0059  SETMBR	R0	K27	R6
      0x80000000,  //  005A  RET	0
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
    ( &(const binstruction[23]) {  /* code */
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
      0x80000000,  //  0016  RET	0
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
    ( &(const binstruction[23]) {  /* code */
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
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: SPAKE2P_Matter
********************************************************************/
be_local_class(SPAKE2P_Matter,
    20,
    NULL,
    be_nested_map(31,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(pA, 23), be_const_var(3) },
        { be_const_key_weak(set_context, 28), be_const_closure(SPAKE2P_Matter_set_context_closure) },
        { be_const_key_weak(KcA, -1), be_const_var(10) },
        { be_const_key_weak(cA, -1), be_const_var(13) },
        { be_const_key_weak(K_shared, -1), be_const_var(12) },
        { be_const_key_weak(A, 9), be_const_var(15) },
        { be_const_key_weak(V, -1), be_const_var(6) },
        { be_const_key_weak(compute_TT_hash, -1), be_const_closure(SPAKE2P_Matter_compute_TT_hash_closure) },
        { be_const_key_weak(Kmain, -1), be_const_var(9) },
        { be_const_key_weak(w0, -1), be_const_var(0) },
        { be_const_key_weak(N, -1), be_const_var(19) },
        { be_const_key_weak(x, -1), be_const_var(7) },
        { be_const_key_weak(Context, 20), be_const_var(17) },
        { be_const_key_weak(compute_pA, -1), be_const_closure(SPAKE2P_Matter_compute_pA_closure) },
        { be_const_key_weak(w1, -1), be_const_var(1) },
        { be_const_key_weak(B, 22), be_const_var(16) },
        { be_const_key_weak(L, 25), be_const_var(2) },
        { be_const_key_weak(cB, 10), be_const_var(14) },
        { be_const_key_weak(spake_M_hex, -1), be_nested_str_weak(04886e2f97ace46e55ba9dd7242579f2993b64e16ef3dcab95afd497333d8fa12f5ff355163e43ce224e0b0e65ff02ac8e5c7be09419c785e0ca547d55a12e2d20) },
        { be_const_key_weak(spake_N_hex, -1), be_nested_str_weak(04d8bbd6c639c62937b04d997f38c3770719c629d7014d49a24b4f98baa1292b4907d60aa6bfade45008a636337f5168c64d9bd36034808cd564490b1e656edbe7) },
        { be_const_key_weak(CRYPTO_GROUP_SIZE_BYTES, 7), be_const_int(32) },
        { be_const_key_weak(CRYPTO_W_SIZE_BYTES, 6), be_const_int(40) },
        { be_const_key_weak(pB, 8), be_const_var(4) },
        { be_const_key_weak(M, -1), be_const_var(18) },
        { be_const_key_weak(compute_ZV_verifier, -1), be_const_closure(SPAKE2P_Matter_compute_ZV_verifier_closure) },
        { be_const_key_weak(KcB, 19), be_const_var(11) },
        { be_const_key_weak(y, -1), be_const_var(8) },
        { be_const_key_weak(Z, -1), be_const_var(5) },
        { be_const_key_weak(compute_ZV_prover, 2), be_const_closure(SPAKE2P_Matter_compute_ZV_prover_closure) },
        { be_const_key_weak(init, 5), be_const_closure(SPAKE2P_Matter_init_closure) },
        { be_const_key_weak(compute_pB, -1), be_const_closure(SPAKE2P_Matter_compute_pB_closure) },
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
