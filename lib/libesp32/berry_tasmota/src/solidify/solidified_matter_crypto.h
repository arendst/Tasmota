/* Solidification of matter_crypto.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

/********************************************************************
** Solidified function: compute_pB
********************************************************************/
be_local_closure(Matter_Crypto_compute_pB,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(EC_P256),
    /* K2   */  be_nested_str_weak(y),
    /* K3   */  be_nested_str_weak(mod),
    /* K4   */  be_nested_str_weak(pB),
    /* K5   */  be_nested_str_weak(muladd),
    /* K6   */  be_nested_str_weak(w0),
    /* K7   */  be_nested_str_weak(N),
    }),
    be_str_weak(compute_pB),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0501,  //  0001  GETMET	R3	R2	K1
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C100703,  //  0003  GETMET	R4	R3	K3
      0x5C180200,  //  0004  MOVE	R6	R1
      0x7C100400,  //  0005  CALL	R4	2
      0x90020404,  //  0006  SETMBR	R0	K2	R4
      0x8C100705,  //  0007  GETMET	R4	R3	K5
      0x88180106,  //  0008  GETMBR	R6	R0	K6
      0x881C0107,  //  0009  GETMBR	R7	R0	K7
      0x88200102,  //  000A  GETMBR	R8	R0	K2
      0x60240015,  //  000B  GETGBL	R9	G21
      0x7C240000,  //  000C  CALL	R9	0
      0x7C100A00,  //  000D  CALL	R4	5
      0x90020804,  //  000E  SETMBR	R0	K4	R4
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_device_info
********************************************************************/
be_local_closure(Matter_Crypto_set_device_info,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(fromstring),
    /* K2   */  be_nested_str_weak(passcode),
    /* K3   */  be_nested_str_weak(salt),
    /* K4   */  be_nested_str_weak(iterations),
    }),
    be_str_weak(set_device_info),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x60100009,  //  0000  GETGBL	R4	G9
      0x5C140200,  //  0001  MOVE	R5	R1
      0x7C100200,  //  0002  CALL	R4	1
      0x5C040800,  //  0003  MOVE	R1	R4
      0x60100009,  //  0004  GETGBL	R4	G9
      0x5C140600,  //  0005  MOVE	R5	R3
      0x7C100200,  //  0006  CALL	R4	1
      0x5C0C0800,  //  0007  MOVE	R3	R4
      0x60100004,  //  0008  GETGBL	R4	G4
      0x5C140400,  //  0009  MOVE	R5	R2
      0x7C100200,  //  000A  CALL	R4	1
      0x1C100900,  //  000B  EQ	R4	R4	K0
      0x78120005,  //  000C  JMPF	R4	#0013
      0x60100015,  //  000D  GETGBL	R4	G21
      0x7C100000,  //  000E  CALL	R4	0
      0x8C100901,  //  000F  GETMET	R4	R4	K1
      0x5C180400,  //  0010  MOVE	R6	R2
      0x7C100400,  //  0011  CALL	R4	2
      0x5C080800,  //  0012  MOVE	R2	R4
      0x90020401,  //  0013  SETMBR	R0	K2	R1
      0x90020602,  //  0014  SETMBR	R0	K3	R2
      0x90020803,  //  0015  SETMBR	R0	K4	R3
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: compute_ZV_verifier
********************************************************************/
be_local_closure(Matter_Crypto_compute_ZV_verifier,   /* name */
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
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Crypto_init,   /* name */
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
    /* K0   */  be_nested_str_weak(M),
    /* K1   */  be_nested_str_weak(fromhex),
    /* K2   */  be_nested_str_weak(spake_M_hex),
    /* K3   */  be_nested_str_weak(N),
    /* K4   */  be_nested_str_weak(spake_N_hex),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x60040015,  //  0000  GETGBL	R1	G21
      0x7C040000,  //  0001  CALL	R1	0
      0x8C040301,  //  0002  GETMET	R1	R1	K1
      0x880C0102,  //  0003  GETMBR	R3	R0	K2
      0x7C040400,  //  0004  CALL	R1	2
      0x90020001,  //  0005  SETMBR	R0	K0	R1
      0x60040015,  //  0006  GETGBL	R1	G21
      0x7C040000,  //  0007  CALL	R1	0
      0x8C040301,  //  0008  GETMET	R1	R1	K1
      0x880C0104,  //  0009  GETMBR	R3	R0	K4
      0x7C040400,  //  000A  CALL	R1	2
      0x90020601,  //  000B  SETMBR	R0	K3	R1
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: compute_ZV_prover
********************************************************************/
be_local_closure(Matter_Crypto_compute_ZV_prover,   /* name */
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
** Solidified function: compute_pA
********************************************************************/
be_local_closure(Matter_Crypto_compute_pA,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(EC_P256),
    /* K2   */  be_nested_str_weak(x),
    /* K3   */  be_nested_str_weak(mod),
    /* K4   */  be_nested_str_weak(pA),
    /* K5   */  be_nested_str_weak(muladd),
    /* K6   */  be_nested_str_weak(w0),
    /* K7   */  be_nested_str_weak(M),
    }),
    be_str_weak(compute_pA),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0501,  //  0001  GETMET	R3	R2	K1
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C100703,  //  0003  GETMET	R4	R3	K3
      0x5C180200,  //  0004  MOVE	R6	R1
      0x7C100400,  //  0005  CALL	R4	2
      0x90020404,  //  0006  SETMBR	R0	K2	R4
      0x8C100705,  //  0007  GETMET	R4	R3	K5
      0x88180106,  //  0008  GETMBR	R6	R0	K6
      0x881C0107,  //  0009  GETMBR	R7	R0	K7
      0x88200102,  //  000A  GETMBR	R8	R0	K2
      0x60240015,  //  000B  GETGBL	R9	G21
      0x7C240000,  //  000C  CALL	R9	0
      0x7C100A00,  //  000D  CALL	R4	5
      0x90020804,  //  000E  SETMBR	R0	K4	R4
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: compute_w0_w1
********************************************************************/
be_local_closure(Matter_Crypto_compute_w0_w1,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(add),
    /* K2   */  be_nested_str_weak(passcode),
    /* K3   */  be_nested_str_weak(PBKDF2_HMAC_SHA256),
    /* K4   */  be_nested_str_weak(derive),
    /* K5   */  be_nested_str_weak(salt),
    /* K6   */  be_nested_str_weak(iterations),
    /* K7   */  be_const_int(2),
    /* K8   */  be_nested_str_weak(CRYPTO_W_SIZE_BYTES),
    /* K9   */  be_const_int(0),
    /* K10  */  be_const_int(1),
    /* K11  */  be_nested_str_weak(EC_P256),
    /* K12  */  be_nested_str_weak(w0),
    /* K13  */  be_nested_str_weak(mod),
    /* K14  */  be_nested_str_weak(w1),
    /* K15  */  be_nested_str_weak(L),
    /* K16  */  be_nested_str_weak(public_key),
    }),
    be_str_weak(compute_w0_w1),
    &be_const_str_solidified,
    ( &(const binstruction[41]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x60080015,  //  0001  GETGBL	R2	G21
      0x7C080000,  //  0002  CALL	R2	0
      0x8C080501,  //  0003  GETMET	R2	R2	K1
      0x88100102,  //  0004  GETMBR	R4	R0	K2
      0x54160003,  //  0005  LDINT	R5	4
      0x7C080600,  //  0006  CALL	R2	3
      0x8C0C0303,  //  0007  GETMET	R3	R1	K3
      0x7C0C0200,  //  0008  CALL	R3	1
      0x8C0C0704,  //  0009  GETMET	R3	R3	K4
      0x5C140400,  //  000A  MOVE	R5	R2
      0x88180105,  //  000B  GETMBR	R6	R0	K5
      0x881C0106,  //  000C  GETMBR	R7	R0	K6
      0x88200108,  //  000D  GETMBR	R8	R0	K8
      0x08220E08,  //  000E  MUL	R8	K7	R8
      0x7C0C0A00,  //  000F  CALL	R3	5
      0x88100108,  //  0010  GETMBR	R4	R0	K8
      0x0410090A,  //  0011  SUB	R4	R4	K10
      0x40121204,  //  0012  CONNECT	R4	K9	R4
      0x94100604,  //  0013  GETIDX	R4	R3	R4
      0x88140108,  //  0014  GETMBR	R5	R0	K8
      0x88180108,  //  0015  GETMBR	R6	R0	K8
      0x081A0E06,  //  0016  MUL	R6	K7	R6
      0x04180D0A,  //  0017  SUB	R6	R6	K10
      0x40140A06,  //  0018  CONNECT	R5	R5	R6
      0x94140605,  //  0019  GETIDX	R5	R3	R5
      0x8C18030B,  //  001A  GETMET	R6	R1	K11
      0x7C180200,  //  001B  CALL	R6	1
      0x8C1C0D0D,  //  001C  GETMET	R7	R6	K13
      0x5C240800,  //  001D  MOVE	R9	R4
      0x7C1C0400,  //  001E  CALL	R7	2
      0x90021807,  //  001F  SETMBR	R0	K12	R7
      0x8C1C0D0D,  //  0020  GETMET	R7	R6	K13
      0x5C240A00,  //  0021  MOVE	R9	R5
      0x7C1C0400,  //  0022  CALL	R7	2
      0x90021C07,  //  0023  SETMBR	R0	K14	R7
      0x8C1C0D10,  //  0024  GETMET	R7	R6	K16
      0x8824010E,  //  0025  GETMBR	R9	R0	K14
      0x7C1C0400,  //  0026  CALL	R7	2
      0x90021E07,  //  0027  SETMBR	R0	K15	R7
      0x80000000,  //  0028  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Crypto
********************************************************************/
be_local_class(Matter_Crypto,
    14,
    NULL,
    be_nested_map(25,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(Z, 24), be_const_var(8) },
        { be_const_key_weak(salt, 23), be_const_var(1) },
        { be_const_key_weak(spake_M_hex, -1), be_nested_str_weak(04886e2f97ace46e55ba9dd7242579f2993b64e16ef3dcab95afd497333d8fa12f5ff355163e43ce224e0b0e65ff02ac8e5c7be09419c785e0ca547d55a12e2d20) },
        { be_const_key_weak(compute_pB, 4), be_const_closure(Matter_Crypto_compute_pB_closure) },
        { be_const_key_weak(w1, 6), be_const_var(4) },
        { be_const_key_weak(set_device_info, 8), be_const_closure(Matter_Crypto_set_device_info_closure) },
        { be_const_key_weak(compute_w0_w1, -1), be_const_closure(Matter_Crypto_compute_w0_w1_closure) },
        { be_const_key_weak(L, -1), be_const_var(5) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Crypto_init_closure) },
        { be_const_key_weak(w0, -1), be_const_var(3) },
        { be_const_key_weak(pA, -1), be_const_var(6) },
        { be_const_key_weak(spake_N_hex, -1), be_nested_str_weak(04d8bbd6c639c62937b04d997f38c3770719c629d7014d49a24b4f98baa1292b4907d60aa6bfade45008a636337f5168c64d9bd36034808cd564490b1e656edbe7) },
        { be_const_key_weak(compute_ZV_verifier, 10), be_const_closure(Matter_Crypto_compute_ZV_verifier_closure) },
        { be_const_key_weak(M, -1), be_const_var(12) },
        { be_const_key_weak(x, -1), be_const_var(10) },
        { be_const_key_weak(passcode, -1), be_const_var(0) },
        { be_const_key_weak(compute_ZV_prover, -1), be_const_closure(Matter_Crypto_compute_ZV_prover_closure) },
        { be_const_key_weak(CRYPTO_W_SIZE_BYTES, -1), be_const_int(40) },
        { be_const_key_weak(pB, 17), be_const_var(7) },
        { be_const_key_weak(CRYPTO_GROUP_SIZE_BYTES, 16), be_const_int(32) },
        { be_const_key_weak(N, 14), be_const_var(13) },
        { be_const_key_weak(compute_pA, -1), be_const_closure(Matter_Crypto_compute_pA_closure) },
        { be_const_key_weak(V, -1), be_const_var(9) },
        { be_const_key_weak(y, -1), be_const_var(11) },
        { be_const_key_weak(iterations, -1), be_const_var(2) },
    })),
    be_str_weak(Matter_Crypto)
);
/*******************************************************************/

void be_load_Matter_Crypto_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Crypto);
    be_setglobal(vm, "Matter_Crypto");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
