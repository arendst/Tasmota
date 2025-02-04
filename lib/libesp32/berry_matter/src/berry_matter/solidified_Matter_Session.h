/* Solidification of Matter_Session.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
extern const bclass be_class_Matter_Session;
// compact class 'Matter_Session' ktab size: 109, total: 196 (saved 696 bytes)
static const bvalue be_ktab_class_Matter_Session[109] = {
  /* K0   */  be_nested_str_weak(_fabric),
  /* K1   */  be_nested_str_weak(device_id),
  /* K2   */  be_nested_str_weak(log),
  /* K3   */  be_nested_str_weak(MTR_X3A_X20_X2DSession_X20_X20_X20_X28_X256i_X29_X20_X28removed_X29),
  /* K4   */  be_nested_str_weak(local_session_id),
  /* K5   */  be_const_int(3),
  /* K6   */  be_nested_str_weak(set_mode),
  /* K7   */  be_nested_str_weak(_CASE),
  /* K8   */  be_nested_str_weak(mode),
  /* K9   */  be_nested_str_weak(add_session),
  /* K10  */  be_nested_str_weak(noc),
  /* K11  */  be_nested_str_weak(crypto),
  /* K12  */  be_nested_str_weak(_store),
  /* K13  */  be_const_int(0),
  /* K14  */  be_nested_str_weak(initiator_session_id),
  /* K15  */  be_nested_str_weak(_counter_snd_impl),
  /* K16  */  be_nested_str_weak(matter),
  /* K17  */  be_nested_str_weak(Counter),
  /* K18  */  be_nested_str_weak(_counter_rcv_impl),
  /* K19  */  be_nested_str_weak(counter_rcv),
  /* K20  */  be_nested_str_weak(counter_snd),
  /* K21  */  be_nested_str_weak(next),
  /* K22  */  be_nested_str_weak(_COUNTER_SND_INCR),
  /* K23  */  be_nested_str_weak(_counter_insecure_rcv),
  /* K24  */  be_nested_str_weak(_counter_insecure_snd),
  /* K25  */  be_nested_str_weak(_breadcrumb),
  /* K26  */  be_nested_str_weak(_exchange_id),
  /* K27  */  be_nested_str_weak(random),
  /* K28  */  be_const_int(2),
  /* K29  */  be_nested_str_weak(get),
  /* K30  */  be_nested_str_weak(update),
  /* K31  */  be_nested_str_weak(admin_vendor),
  /* K32  */  be_nested_str_weak(i2rkey),
  /* K33  */  be_nested_str_weak(validate),
  /* K34  */  be_nested_str_weak(val),
  /* K35  */  be_nested_str_weak(root_ca_certificate),
  /* K36  */  be_nested_str_weak(fabric_index),
  /* K37  */  be_nested_str_weak(r2ikey),
  /* K38  */  be_nested_str_weak(is_greater),
  /* K39  */  be_nested_str_weak(does_persist),
  /* K40  */  be_nested_str_weak(save),
  /* K41  */  be_nested_str_weak(_temp_root_ca_certificate),
  /* K42  */  be_nested_str_weak(TLV),
  /* K43  */  be_nested_str_weak(parse),
  /* K44  */  be_nested_str_weak(findsubval),
  /* K45  */  be_nested_str_weak(fabric_id),
  /* K46  */  be_nested_str_weak(_PASE),
  /* K47  */  be_nested_str_weak(fabric_label),
  /* K48  */  be_nested_str_weak(last_used),
  /* K49  */  be_nested_str_weak(tasmota),
  /* K50  */  be_nested_str_weak(rtc_utc),
  /* K51  */  be_nested_str_weak(icac),
  /* K52  */  be_nested_str_weak(string),
  /* K53  */  be_nested_str_weak(get_ca_pub),
  /* K54  */  be_nested_str_weak(get_pk),
  /* K55  */  be_nested_str_weak(_temp_pk),
  /* K56  */  be_nested_str_weak(json),
  /* K57  */  be_nested_str_weak(introspect),
  /* K58  */  be_nested_str_weak(persist_pre),
  /* K59  */  be_nested_str_weak(members),
  /* K60  */  be_nested_str_weak(function),
  /* K61  */  be_nested_str_weak(_),
  /* K62  */  be_nested_str_weak(push),
  /* K63  */  be_nested_str_weak(stop_iteration),
  /* K64  */  be_nested_str_weak(sort),
  /* K65  */  be_nested_str_weak(_X24_X24),
  /* K66  */  be_nested_str_weak(tob64),
  /* K67  */  be_nested_str_weak(instance),
  /* K68  */  be_nested_str_weak(_X25s_X3A_X25s),
  /* K69  */  be_nested_str_weak(dump),
  /* K70  */  be_nested_str_weak(persist_post),
  /* K71  */  be_nested_str_weak(_X7B),
  /* K72  */  be_nested_str_weak(concat),
  /* K73  */  be_nested_str_weak(_X2C),
  /* K74  */  be_nested_str_weak(_X7D),
  /* K75  */  be_nested_str_weak(__future_local_session_id),
  /* K76  */  be_nested_str_weak(__future_initiator_session_id),
  /* K77  */  be_nested_str_weak(reset),
  /* K78  */  be_nested_str_weak(_i2r_privacy),
  /* K79  */  be_nested_str_weak(attestation_challenge),
  /* K80  */  be_const_int(1),
  /* K81  */  be_nested_str_weak(HKDF_SHA256),
  /* K82  */  be_nested_str_weak(derive),
  /* K83  */  be_nested_str_weak(get_i2r),
  /* K84  */  be_nested_str_weak(fromstring),
  /* K85  */  be_nested_str_weak(PrivacyKey),
  /* K86  */  be_const_class(be_class_Matter_Session),
  /* K87  */  be_nested_str_weak(Session),
  /* K88  */  be_nested_str_weak(keys),
  /* K89  */  be_nested_str_weak(find),
  /* K90  */  be_nested_str_weak(0x),
  /* K91  */  be_nested_str_weak(set),
  /* K92  */  be_nested_str_weak(fromhex),
  /* K93  */  be_const_int(2147483647),
  /* K94  */  be_nested_str_weak(fromb64),
  /* K95  */  be_nested_str_weak(hydrate_post),
  /* K96  */  be_nested_str_weak(save_fabrics),
  /* K97  */  be_nested_str_weak(admin_subject),
  /* K98  */  be_nested_str_weak(ipk_epoch_key),
  /* K99  */  be_nested_str_weak(fabric_compressed),
  /* K100 */  be_nested_str_weak(created),
  /* K101 */  be_nested_str_weak(EC_P256),
  /* K102 */  be_nested_str_weak(public_key),
  /* K103 */  be_nested_str_weak(3070020100300E310C300A060355040A0C034353523059301306072A8648CE3D020106082A8648CE3D030107034200),
  /* K104 */  be_nested_str_weak(A000),
  /* K105 */  be_nested_str_weak(300C06082A8648CE3D0403020500),
  /* K106 */  be_nested_str_weak(ecdsa_sign_sha256_asn1),
  /* K107 */  be_nested_str_weak(add),
  /* K108 */  be_nested_str_weak(get_ipk_group_key),
};


extern const bclass be_class_Matter_Session;

/********************************************************************
** Solidified function: get_device_id
********************************************************************/
be_local_closure(class_Matter_Session_get_device_id,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(get_device_id),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060002,  //  0001  JMPF	R1	#0005
      0x88040100,  //  0002  GETMBR	R1	R0	K0
      0x88040301,  //  0003  GETMBR	R1	R1	K1
      0x70020000,  //  0004  JMP		#0006
      0x4C040000,  //  0005  LDNIL	R1
      0x80040200,  //  0006  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: before_remove
********************************************************************/
be_local_closure(class_Matter_Session_before_remove,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(before_remove),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0xB8060400,  //  0000  GETNGBL	R1	K2
      0x60080018,  //  0001  GETGBL	R2	G24
      0x580C0003,  //  0002  LDCONST	R3	K3
      0x88100104,  //  0003  GETMBR	R4	R0	K4
      0x7C080400,  //  0004  CALL	R2	2
      0x580C0005,  //  0005  LDCONST	R3	K5
      0x7C040400,  //  0006  CALL	R1	2
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_mode_CASE
********************************************************************/
be_local_closure(class_Matter_Session_set_mode_CASE,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(set_mode_CASE),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C040106,  //  0000  GETMET	R1	R0	K6
      0x880C0107,  //  0001  GETMBR	R3	R0	K7
      0x7C040400,  //  0002  CALL	R1	2
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_CASE
********************************************************************/
be_local_closure(class_Matter_Session_is_CASE,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(is_CASE),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040108,  //  0000  GETMBR	R1	R0	K8
      0x88080107,  //  0001  GETMBR	R2	R0	K7
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: persist_to_fabric
********************************************************************/
be_local_closure(class_Matter_Session_persist_to_fabric,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(persist_to_fabric),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040309,  //  0001  GETMET	R1	R1	K9
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_noc
********************************************************************/
be_local_closure(class_Matter_Session_get_noc,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(get_noc),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8804030A,  //  0001  GETMBR	R1	R1	K10
      0x80040200,  //  0002  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_Session_init,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[41]) {  /* code */
      0xA4161600,  //  0000  IMPORT	R5	K11
      0x90021801,  //  0001  SETMBR	R0	K12	R1
      0x9002110D,  //  0002  SETMBR	R0	K8	K13
      0x90020802,  //  0003  SETMBR	R0	K4	R2
      0x90021C03,  //  0004  SETMBR	R0	K14	R3
      0xB81A2000,  //  0005  GETNGBL	R6	K16
      0x8C180D11,  //  0006  GETMET	R6	R6	K17
      0x7C180200,  //  0007  CALL	R6	1
      0x90021E06,  //  0008  SETMBR	R0	K15	R6
      0xB81A2000,  //  0009  GETNGBL	R6	K16
      0x8C180D11,  //  000A  GETMET	R6	R6	K17
      0x7C180200,  //  000B  CALL	R6	1
      0x90022406,  //  000C  SETMBR	R0	K18	R6
      0x9002270D,  //  000D  SETMBR	R0	K19	K13
      0x8818010F,  //  000E  GETMBR	R6	R0	K15
      0x8C180D15,  //  000F  GETMET	R6	R6	K21
      0x7C180200,  //  0010  CALL	R6	1
      0x881C0116,  //  0011  GETMBR	R7	R0	K22
      0x00180C07,  //  0012  ADD	R6	R6	R7
      0x90022806,  //  0013  SETMBR	R0	K20	R6
      0xB81A2000,  //  0014  GETNGBL	R6	K16
      0x8C180D11,  //  0015  GETMET	R6	R6	K17
      0x7C180200,  //  0016  CALL	R6	1
      0x90022E06,  //  0017  SETMBR	R0	K23	R6
      0xB81A2000,  //  0018  GETNGBL	R6	K16
      0x8C180D11,  //  0019  GETMET	R6	R6	K17
      0x7C180200,  //  001A  CALL	R6	1
      0x90023006,  //  001B  SETMBR	R0	K24	R6
      0x9002330D,  //  001C  SETMBR	R0	K25	K13
      0x8C180B1B,  //  001D  GETMET	R6	R5	K27
      0x5820001C,  //  001E  LDCONST	R8	K28
      0x7C180400,  //  001F  CALL	R6	2
      0x8C180D1D,  //  0020  GETMET	R6	R6	K29
      0x5820000D,  //  0021  LDCONST	R8	K13
      0x5824001C,  //  0022  LDCONST	R9	K28
      0x7C180600,  //  0023  CALL	R6	3
      0x90023406,  //  0024  SETMBR	R0	K26	R6
      0x90020004,  //  0025  SETMBR	R0	K0	R4
      0x8C18011E,  //  0026  GETMET	R6	R0	K30
      0x7C180200,  //  0027  CALL	R6	1
      0x80000000,  //  0028  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_admin_vendor
********************************************************************/
be_local_closure(class_Matter_Session_get_admin_vendor,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(get_admin_vendor),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060002,  //  0001  JMPF	R1	#0005
      0x88040100,  //  0002  GETMBR	R1	R0	K0
      0x8804031F,  //  0003  GETMBR	R1	R1	K31
      0x70020000,  //  0004  JMP		#0006
      0x4C040000,  //  0005  LDNIL	R1
      0x80040200,  //  0006  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_i2r
********************************************************************/
be_local_closure(class_Matter_Session_get_i2r,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(get_i2r),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040120,  //  0000  GETMBR	R1	R0	K32
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: counter_rcv_validate
********************************************************************/
be_local_closure(class_Matter_Session_counter_rcv_validate,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(counter_rcv_validate),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x880C0112,  //  0000  GETMBR	R3	R0	K18
      0x8C0C0721,  //  0001  GETMET	R3	R3	K33
      0x5C140200,  //  0002  MOVE	R5	R1
      0x5C180400,  //  0003  MOVE	R6	R2
      0x7C0C0600,  //  0004  CALL	R3	3
      0x780E0003,  //  0005  JMPF	R3	#000A
      0x88100112,  //  0006  GETMBR	R4	R0	K18
      0x8C100922,  //  0007  GETMET	R4	R4	K34
      0x7C100200,  //  0008  CALL	R4	1
      0x90022604,  //  0009  SETMBR	R0	K19	R4
      0x80040600,  //  000A  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_ca
********************************************************************/
be_local_closure(class_Matter_Session_get_ca,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(get_ca),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88040323,  //  0001  GETMBR	R1	R1	K35
      0x80040200,  //  0002  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_fabric_index
********************************************************************/
be_local_closure(class_Matter_Session_get_fabric_index,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(get_fabric_index),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060002,  //  0001  JMPF	R1	#0005
      0x88040100,  //  0002  GETMBR	R1	R0	K0
      0x88040324,  //  0003  GETMBR	R1	R1	K36
      0x70020000,  //  0004  JMP		#0006
      0x4C040000,  //  0005  LDNIL	R1
      0x80040200,  //  0006  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_r2i
********************************************************************/
be_local_closure(class_Matter_Session_get_r2i,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(get_r2i),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040125,  //  0000  GETMBR	R1	R0	K37
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: counter_snd_next
********************************************************************/
be_local_closure(class_Matter_Session_counter_snd_next,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(counter_snd_next),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x8804010F,  //  0000  GETMBR	R1	R0	K15
      0x8C040315,  //  0001  GETMET	R1	R1	K21
      0x7C040200,  //  0002  CALL	R1	1
      0xB80A2000,  //  0003  GETNGBL	R2	K16
      0x88080511,  //  0004  GETMBR	R2	R2	K17
      0x8C080526,  //  0005  GETMET	R2	R2	K38
      0x5C100200,  //  0006  MOVE	R4	R1
      0x88140114,  //  0007  GETMBR	R5	R0	K20
      0x7C080600,  //  0008  CALL	R2	3
      0x780A0007,  //  0009  JMPF	R2	#0012
      0x88080116,  //  000A  GETMBR	R2	R0	K22
      0x00080202,  //  000B  ADD	R2	R1	R2
      0x90022802,  //  000C  SETMBR	R0	K20	R2
      0x8C080127,  //  000D  GETMET	R2	R0	K39
      0x7C080200,  //  000E  CALL	R2	1
      0x780A0001,  //  000F  JMPF	R2	#0012
      0x8C080128,  //  0010  GETMET	R2	R0	K40
      0x7C080200,  //  0011  CALL	R2	1
      0x80040200,  //  0012  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_temp_ca_pub
********************************************************************/
be_local_closure(class_Matter_Session_get_temp_ca_pub,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(get_temp_ca_pub),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x88040129,  //  0000  GETMBR	R1	R0	K41
      0x78060008,  //  0001  JMPF	R1	#000B
      0xB80A2000,  //  0002  GETNGBL	R2	K16
      0x8808052A,  //  0003  GETMBR	R2	R2	K42
      0x8C08052B,  //  0004  GETMET	R2	R2	K43
      0x5C100200,  //  0005  MOVE	R4	R1
      0x7C080400,  //  0006  CALL	R2	2
      0x8C0C052C,  //  0007  GETMET	R3	R2	K44
      0x54160008,  //  0008  LDINT	R5	9
      0x7C0C0400,  //  0009  CALL	R3	2
      0x80040600,  //  000A  RET	1	R3
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_temp_ca
********************************************************************/
be_local_closure(class_Matter_Session_get_temp_ca,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(get_temp_ca),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040129,  //  0000  GETMBR	R1	R0	K41
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_fabric_id
********************************************************************/
be_local_closure(class_Matter_Session_get_fabric_id,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(get_fabric_id),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8804032D,  //  0001  GETMBR	R1	R1	K45
      0x80040200,  //  0002  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_mode_PASE
********************************************************************/
be_local_closure(class_Matter_Session_set_mode_PASE,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(set_mode_PASE),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C040106,  //  0000  GETMET	R1	R0	K6
      0x880C012E,  //  0001  GETMBR	R3	R0	K46
      0x7C040400,  //  0002  CALL	R1	2
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_fabric_label
********************************************************************/
be_local_closure(class_Matter_Session_get_fabric_label,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(get_fabric_label),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060002,  //  0001  JMPF	R1	#0005
      0x88040100,  //  0002  GETMBR	R1	R0	K0
      0x8804032F,  //  0003  GETMBR	R1	R1	K47
      0x70020000,  //  0004  JMP		#0006
      0x4C040000,  //  0005  LDNIL	R1
      0x80040200,  //  0006  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update
********************************************************************/
be_local_closure(class_Matter_Session_update,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(update),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0xB8066200,  //  0000  GETNGBL	R1	K49
      0x8C040332,  //  0001  GETMET	R1	R1	K50
      0x7C040200,  //  0002  CALL	R1	1
      0x90026001,  //  0003  SETMBR	R0	K48	R1
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_icac
********************************************************************/
be_local_closure(class_Matter_Session_get_icac,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(get_icac),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88040333,  //  0001  GETMBR	R1	R1	K51
      0x80040200,  //  0002  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_PASE
********************************************************************/
be_local_closure(class_Matter_Session_is_PASE,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(is_PASE),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040108,  //  0000  GETMBR	R1	R0	K8
      0x8808012E,  //  0001  GETMBR	R2	R0	K46
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_fabric_label
********************************************************************/
be_local_closure(class_Matter_Session_set_fabric_label,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(set_fabric_label),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x60080004,  //  0000  GETGBL	R2	G4
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x1C080534,  //  0003  EQ	R2	R2	K52
      0x780A0001,  //  0004  JMPF	R2	#0007
      0x88080100,  //  0005  GETMBR	R2	R0	K0
      0x900A5E01,  //  0006  SETMBR	R2	K47	R1
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_node_id
********************************************************************/
be_local_closure(class_Matter_Session_get_node_id,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(get_node_id),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060002,  //  0001  JMPF	R1	#0005
      0x88040100,  //  0002  GETMBR	R1	R0	K0
      0x88040301,  //  0003  GETMBR	R1	R1	K1
      0x70020000,  //  0004  JMP		#0006
      0x4C040000,  //  0005  LDNIL	R1
      0x80040200,  //  0006  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_ca_pub
********************************************************************/
be_local_closure(class_Matter_Session_get_ca_pub,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(get_ca_pub),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040335,  //  0001  GETMET	R1	R1	K53
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_pk
********************************************************************/
be_local_closure(class_Matter_Session_get_pk,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(get_pk),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060004,  //  0001  JMPF	R1	#0007
      0x88040100,  //  0002  GETMBR	R1	R0	K0
      0x8C040336,  //  0003  GETMET	R1	R1	K54
      0x7C040200,  //  0004  CALL	R1	1
      0x80040200,  //  0005  RET	1	R1
      0x70020008,  //  0006  JMP		#0010
      0x88040137,  //  0007  GETMBR	R1	R0	K55
      0x74060004,  //  0008  JMPT	R1	#000E
      0xA4061600,  //  0009  IMPORT	R1	K11
      0x8C08031B,  //  000A  GETMET	R2	R1	K27
      0x5412001F,  //  000B  LDINT	R4	32
      0x7C080400,  //  000C  CALL	R2	2
      0x90026E02,  //  000D  SETMBR	R0	K55	R2
      0x88040137,  //  000E  GETMBR	R1	R0	K55
      0x80040200,  //  000F  RET	1	R1
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tojson
********************************************************************/
be_local_closure(class_Matter_Session_tojson,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(tojson),
    &be_const_str_solidified,
    ( &(const binstruction[95]) {  /* code */
      0xA4067000,  //  0000  IMPORT	R1	K56
      0xA40A7200,  //  0001  IMPORT	R2	K57
      0x8C0C013A,  //  0002  GETMET	R3	R0	K58
      0x7C0C0200,  //  0003  CALL	R3	1
      0x600C0012,  //  0004  GETGBL	R3	G18
      0x7C0C0000,  //  0005  CALL	R3	0
      0x60100010,  //  0006  GETGBL	R4	G16
      0x8C14053B,  //  0007  GETMET	R5	R2	K59
      0x5C1C0000,  //  0008  MOVE	R7	R0
      0x7C140400,  //  0009  CALL	R5	2
      0x7C100200,  //  000A  CALL	R4	1
      0xA8020011,  //  000B  EXBLK	0	#001E
      0x5C140800,  //  000C  MOVE	R5	R4
      0x7C140000,  //  000D  CALL	R5	0
      0x8C18051D,  //  000E  GETMET	R6	R2	K29
      0x5C200000,  //  000F  MOVE	R8	R0
      0x5C240A00,  //  0010  MOVE	R9	R5
      0x7C180600,  //  0011  CALL	R6	3
      0x601C0004,  //  0012  GETGBL	R7	G4
      0x5C200C00,  //  0013  MOVE	R8	R6
      0x7C1C0200,  //  0014  CALL	R7	1
      0x201C0F3C,  //  0015  NE	R7	R7	K60
      0x781E0005,  //  0016  JMPF	R7	#001D
      0x941C0B0D,  //  0017  GETIDX	R7	R5	K13
      0x201C0F3D,  //  0018  NE	R7	R7	K61
      0x781E0002,  //  0019  JMPF	R7	#001D
      0x8C1C073E,  //  001A  GETMET	R7	R3	K62
      0x5C240A00,  //  001B  MOVE	R9	R5
      0x7C1C0400,  //  001C  CALL	R7	2
      0x7001FFED,  //  001D  JMP		#000C
      0x5810003F,  //  001E  LDCONST	R4	K63
      0xAC100200,  //  001F  CATCH	R4	1	0
      0xB0080000,  //  0020  RAISE	2	R0	R0
      0xB8122000,  //  0021  GETNGBL	R4	K16
      0x8C100940,  //  0022  GETMET	R4	R4	K64
      0x5C180600,  //  0023  MOVE	R6	R3
      0x7C100400,  //  0024  CALL	R4	2
      0x5C0C0800,  //  0025  MOVE	R3	R4
      0x60100012,  //  0026  GETGBL	R4	G18
      0x7C100000,  //  0027  CALL	R4	0
      0x60140010,  //  0028  GETGBL	R5	G16
      0x5C180600,  //  0029  MOVE	R6	R3
      0x7C140200,  //  002A  CALL	R5	1
      0xA8020027,  //  002B  EXBLK	0	#0054
      0x5C180A00,  //  002C  MOVE	R6	R5
      0x7C180000,  //  002D  CALL	R6	0
      0x8C1C051D,  //  002E  GETMET	R7	R2	K29
      0x5C240000,  //  002F  MOVE	R9	R0
      0x5C280C00,  //  0030  MOVE	R10	R6
      0x7C1C0600,  //  0031  CALL	R7	3
      0x4C200000,  //  0032  LDNIL	R8
      0x1C200E08,  //  0033  EQ	R8	R7	R8
      0x78220000,  //  0034  JMPF	R8	#0036
      0x7001FFF5,  //  0035  JMP		#002C
      0x6020000F,  //  0036  GETGBL	R8	G15
      0x5C240E00,  //  0037  MOVE	R9	R7
      0x60280015,  //  0038  GETGBL	R10	G21
      0x7C200400,  //  0039  CALL	R8	2
      0x78220004,  //  003A  JMPF	R8	#0040
      0x8C200F42,  //  003B  GETMET	R8	R7	K66
      0x7C200200,  //  003C  CALL	R8	1
      0x00228208,  //  003D  ADD	R8	K65	R8
      0x5C1C1000,  //  003E  MOVE	R7	R8
      0x70020005,  //  003F  JMP		#0046
      0x60200004,  //  0040  GETGBL	R8	G4
      0x5C240E00,  //  0041  MOVE	R9	R7
      0x7C200200,  //  0042  CALL	R8	1
      0x1C201143,  //  0043  EQ	R8	R8	K67
      0x78220000,  //  0044  JMPF	R8	#0046
      0x7001FFE5,  //  0045  JMP		#002C
      0x8C20093E,  //  0046  GETMET	R8	R4	K62
      0x60280018,  //  0047  GETGBL	R10	G24
      0x582C0044,  //  0048  LDCONST	R11	K68
      0x8C300345,  //  0049  GETMET	R12	R1	K69
      0x60380008,  //  004A  GETGBL	R14	G8
      0x5C3C0C00,  //  004B  MOVE	R15	R6
      0x7C380200,  //  004C  CALL	R14	1
      0x7C300400,  //  004D  CALL	R12	2
      0x8C340345,  //  004E  GETMET	R13	R1	K69
      0x5C3C0E00,  //  004F  MOVE	R15	R7
      0x7C340400,  //  0050  CALL	R13	2
      0x7C280600,  //  0051  CALL	R10	3
      0x7C200400,  //  0052  CALL	R8	2
      0x7001FFD7,  //  0053  JMP		#002C
      0x5814003F,  //  0054  LDCONST	R5	K63
      0xAC140200,  //  0055  CATCH	R5	1	0
      0xB0080000,  //  0056  RAISE	2	R0	R0
      0x8C140146,  //  0057  GETMET	R5	R0	K70
      0x7C140200,  //  0058  CALL	R5	1
      0x8C140948,  //  0059  GETMET	R5	R4	K72
      0x581C0049,  //  005A  LDCONST	R7	K73
      0x7C140400,  //  005B  CALL	R5	2
      0x00168E05,  //  005C  ADD	R5	K71	R5
      0x00140B4A,  //  005D  ADD	R5	R5	K74
      0x80040A00,  //  005E  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: close
********************************************************************/
be_local_closure(class_Matter_Session_close,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(close),
    &be_const_str_solidified,
    ( &(const binstruction[59]) {  /* code */
      0x8804014B,  //  0000  GETMBR	R1	R0	K75
      0x90020801,  //  0001  SETMBR	R0	K4	R1
      0x8804014C,  //  0002  GETMBR	R1	R0	K76
      0x90021C01,  //  0003  SETMBR	R0	K14	R1
      0x88040112,  //  0004  GETMBR	R1	R0	K18
      0x8C04034D,  //  0005  GETMET	R1	R1	K77
      0x7C040200,  //  0006  CALL	R1	1
      0x8804010F,  //  0007  GETMBR	R1	R0	K15
      0x8C04034D,  //  0008  GETMET	R1	R1	K77
      0x7C040200,  //  0009  CALL	R1	1
      0x9002270D,  //  000A  SETMBR	R0	K19	K13
      0x8804010F,  //  000B  GETMBR	R1	R0	K15
      0x8C040315,  //  000C  GETMET	R1	R1	K21
      0x7C040200,  //  000D  CALL	R1	1
      0x90022801,  //  000E  SETMBR	R0	K20	R1
      0x4C040000,  //  000F  LDNIL	R1
      0x90024001,  //  0010  SETMBR	R0	K32	R1
      0x4C040000,  //  0011  LDNIL	R1
      0x90029C01,  //  0012  SETMBR	R0	K78	R1
      0x4C040000,  //  0013  LDNIL	R1
      0x90024A01,  //  0014  SETMBR	R0	K37	R1
      0x4C040000,  //  0015  LDNIL	R1
      0x90029E01,  //  0016  SETMBR	R0	K79	R1
      0xA4067200,  //  0017  IMPORT	R1	K57
      0x60080010,  //  0018  GETGBL	R2	G16
      0x8C0C033B,  //  0019  GETMET	R3	R1	K59
      0x5C140000,  //  001A  MOVE	R5	R0
      0x7C0C0400,  //  001B  CALL	R3	2
      0x7C080200,  //  001C  CALL	R2	1
      0xA8020018,  //  001D  EXBLK	0	#0037
      0x5C0C0400,  //  001E  MOVE	R3	R2
      0x7C0C0000,  //  001F  CALL	R3	0
      0x8C10031D,  //  0020  GETMET	R4	R1	K29
      0x5C180000,  //  0021  MOVE	R6	R0
      0x5C1C0600,  //  0022  MOVE	R7	R3
      0x7C100600,  //  0023  CALL	R4	3
      0x60140004,  //  0024  GETGBL	R5	G4
      0x5C180800,  //  0025  MOVE	R6	R4
      0x7C140200,  //  0026  CALL	R5	1
      0x20140B3C,  //  0027  NE	R5	R5	K60
      0x7816000C,  //  0028  JMPF	R5	#0036
      0x60140004,  //  0029  GETGBL	R5	G4
      0x5C180800,  //  002A  MOVE	R6	R4
      0x7C140200,  //  002B  CALL	R5	1
      0x20140B43,  //  002C  NE	R5	R5	K67
      0x78160007,  //  002D  JMPF	R5	#0036
      0x9414070D,  //  002E  GETIDX	R5	R3	K13
      0x1C140B3D,  //  002F  EQ	R5	R5	K61
      0x78160004,  //  0030  JMPF	R5	#0036
      0x94140750,  //  0031  GETIDX	R5	R3	K80
      0x1C140B3D,  //  0032  EQ	R5	R5	K61
      0x78160001,  //  0033  JMPF	R5	#0036
      0x4C140000,  //  0034  LDNIL	R5
      0x90000605,  //  0035  SETMBR	R0	R3	R5
      0x7001FFE6,  //  0036  JMP		#001E
      0x5808003F,  //  0037  LDCONST	R2	K63
      0xAC080200,  //  0038  CATCH	R2	1	0
      0xB0080000,  //  0039  RAISE	2	R0	R0
      0x80000000,  //  003A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_ac
********************************************************************/
be_local_closure(class_Matter_Session_get_ac,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(get_ac),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x8804014F,  //  0000  GETMBR	R1	R0	K79
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_fabric
********************************************************************/
be_local_closure(class_Matter_Session_get_fabric,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(get_fabric),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_i2r_privacy
********************************************************************/
be_local_closure(class_Matter_Session_get_i2r_privacy,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(get_i2r_privacy),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x8804014E,  //  0000  GETMBR	R1	R0	K78
      0x4C080000,  //  0001  LDNIL	R2
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x7806000F,  //  0003  JMPF	R1	#0014
      0xA4061600,  //  0004  IMPORT	R1	K11
      0x8C080351,  //  0005  GETMET	R2	R1	K81
      0x7C080200,  //  0006  CALL	R2	1
      0x8C080552,  //  0007  GETMET	R2	R2	K82
      0x8C100153,  //  0008  GETMET	R4	R0	K83
      0x7C100200,  //  0009  CALL	R4	1
      0x60140015,  //  000A  GETGBL	R5	G21
      0x7C140000,  //  000B  CALL	R5	0
      0x60180015,  //  000C  GETGBL	R6	G21
      0x7C180000,  //  000D  CALL	R6	0
      0x8C180D54,  //  000E  GETMET	R6	R6	K84
      0x58200055,  //  000F  LDCONST	R8	K85
      0x7C180400,  //  0010  CALL	R6	2
      0x541E000F,  //  0011  LDINT	R7	16
      0x7C080A00,  //  0012  CALL	R2	5
      0x90029C02,  //  0013  SETMBR	R0	K78	R2
      0x8804014E,  //  0014  GETMBR	R1	R0	K78
      0x80040200,  //  0015  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: fromjson
********************************************************************/
be_local_closure(class_Matter_Session_fromjson,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    3,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(fromjson),
    &be_const_str_solidified,
    ( &(const binstruction[75]) {  /* code */
      0x580C0056,  //  0000  LDCONST	R3	K86
      0xA4126800,  //  0001  IMPORT	R4	K52
      0xA4167200,  //  0002  IMPORT	R5	K57
      0xB81A2000,  //  0003  GETNGBL	R6	K16
      0x8C180D57,  //  0004  GETMET	R6	R6	K87
      0x5C200000,  //  0005  MOVE	R8	R0
      0x4C240000,  //  0006  LDNIL	R9
      0x4C280000,  //  0007  LDNIL	R10
      0x5C2C0400,  //  0008  MOVE	R11	R2
      0x7C180A00,  //  0009  CALL	R6	5
      0x601C0010,  //  000A  GETGBL	R7	G16
      0x8C200358,  //  000B  GETMET	R8	R1	K88
      0x7C200200,  //  000C  CALL	R8	1
      0x7C1C0200,  //  000D  CALL	R7	1
      0xA8020035,  //  000E  EXBLK	0	#0045
      0x5C200E00,  //  000F  MOVE	R8	R7
      0x7C200000,  //  0010  CALL	R8	0
      0x94240208,  //  0011  GETIDX	R9	R1	R8
      0x60280004,  //  0012  GETGBL	R10	G4
      0x5C2C1200,  //  0013  MOVE	R11	R9
      0x7C280200,  //  0014  CALL	R10	1
      0x1C281534,  //  0015  EQ	R10	R10	K52
      0x782A0027,  //  0016  JMPF	R10	#003F
      0x8C280959,  //  0017  GETMET	R10	R4	K89
      0x5C301200,  //  0018  MOVE	R12	R9
      0x5834005A,  //  0019  LDCONST	R13	K90
      0x7C280600,  //  001A  CALL	R10	3
      0x1C28150D,  //  001B  EQ	R10	R10	K13
      0x782A000A,  //  001C  JMPF	R10	#0028
      0x8C280B5B,  //  001D  GETMET	R10	R5	K91
      0x5C300C00,  //  001E  MOVE	R12	R6
      0x5C341000,  //  001F  MOVE	R13	R8
      0x60380015,  //  0020  GETGBL	R14	G21
      0x7C380000,  //  0021  CALL	R14	0
      0x8C381D5C,  //  0022  GETMET	R14	R14	K92
      0x4042395D,  //  0023  CONNECT	R16	K28	K93
      0x94401210,  //  0024  GETIDX	R16	R9	R16
      0x7C380400,  //  0025  CALL	R14	2
      0x7C280800,  //  0026  CALL	R10	4
      0x70020015,  //  0027  JMP		#003E
      0x8C280959,  //  0028  GETMET	R10	R4	K89
      0x5C301200,  //  0029  MOVE	R12	R9
      0x58340041,  //  002A  LDCONST	R13	K65
      0x7C280600,  //  002B  CALL	R10	3
      0x1C28150D,  //  002C  EQ	R10	R10	K13
      0x782A000A,  //  002D  JMPF	R10	#0039
      0x8C280B5B,  //  002E  GETMET	R10	R5	K91
      0x5C300C00,  //  002F  MOVE	R12	R6
      0x5C341000,  //  0030  MOVE	R13	R8
      0x60380015,  //  0031  GETGBL	R14	G21
      0x7C380000,  //  0032  CALL	R14	0
      0x8C381D5E,  //  0033  GETMET	R14	R14	K94
      0x4042395D,  //  0034  CONNECT	R16	K28	K93
      0x94401210,  //  0035  GETIDX	R16	R9	R16
      0x7C380400,  //  0036  CALL	R14	2
      0x7C280800,  //  0037  CALL	R10	4
      0x70020004,  //  0038  JMP		#003E
      0x8C280B5B,  //  0039  GETMET	R10	R5	K91
      0x5C300C00,  //  003A  MOVE	R12	R6
      0x5C341000,  //  003B  MOVE	R13	R8
      0x5C381200,  //  003C  MOVE	R14	R9
      0x7C280800,  //  003D  CALL	R10	4
      0x70020004,  //  003E  JMP		#0044
      0x8C280B5B,  //  003F  GETMET	R10	R5	K91
      0x5C300C00,  //  0040  MOVE	R12	R6
      0x5C341000,  //  0041  MOVE	R13	R8
      0x5C381200,  //  0042  MOVE	R14	R9
      0x7C280800,  //  0043  CALL	R10	4
      0x7001FFC9,  //  0044  JMP		#000F
      0x581C003F,  //  0045  LDCONST	R7	K63
      0xAC1C0200,  //  0046  CATCH	R7	1	0
      0xB0080000,  //  0047  RAISE	2	R0	R0
      0x8C1C0D5F,  //  0048  GETMET	R7	R6	K95
      0x7C1C0200,  //  0049  CALL	R7	1
      0x80040C00,  //  004A  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: save
********************************************************************/
be_local_closure(class_Matter_Session_save,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(save),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8804010C,  //  0000  GETMBR	R1	R0	K12
      0x8C040360,  //  0001  GETMET	R1	R1	K96
      0x7C040200,  //  0002  CALL	R1	1
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: hydrate_post
********************************************************************/
be_local_closure(class_Matter_Session_hydrate_post,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(hydrate_post),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x8804010F,  //  0000  GETMBR	R1	R0	K15
      0x8C04034D,  //  0001  GETMET	R1	R1	K77
      0x880C0114,  //  0002  GETMBR	R3	R0	K20
      0x7C040400,  //  0003  CALL	R1	2
      0x88040112,  //  0004  GETMBR	R1	R0	K18
      0x8C04034D,  //  0005  GETMET	R1	R1	K77
      0x880C0113,  //  0006  GETMBR	R3	R0	K19
      0x7C040400,  //  0007  CALL	R1	2
      0x8804010F,  //  0008  GETMBR	R1	R0	K15
      0x8C040322,  //  0009  GETMET	R1	R1	K34
      0x7C040200,  //  000A  CALL	R1	1
      0x90022801,  //  000B  SETMBR	R0	K20	R1
      0x88040112,  //  000C  GETMBR	R1	R0	K18
      0x8C040322,  //  000D  GETMET	R1	R1	K34
      0x7C040200,  //  000E  CALL	R1	1
      0x90022601,  //  000F  SETMBR	R0	K19	R1
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_admin_subject
********************************************************************/
be_local_closure(class_Matter_Session_get_admin_subject,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(get_admin_subject),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060002,  //  0001  JMPF	R1	#0005
      0x88040100,  //  0002  GETMBR	R1	R0	K0
      0x88040361,  //  0003  GETMBR	R1	R1	K97
      0x70020000,  //  0004  JMP		#0006
      0x4C040000,  //  0005  LDNIL	R1
      0x80040200,  //  0006  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_ipk_epoch_key
********************************************************************/
be_local_closure(class_Matter_Session_get_ipk_epoch_key,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(get_ipk_epoch_key),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88040362,  //  0001  GETMBR	R1	R1	K98
      0x80040200,  //  0002  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_fabric_compressed
********************************************************************/
be_local_closure(class_Matter_Session_get_fabric_compressed,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(get_fabric_compressed),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060002,  //  0001  JMPF	R1	#0005
      0x88040100,  //  0002  GETMBR	R1	R0	K0
      0x88040363,  //  0003  GETMBR	R1	R1	K99
      0x70020000,  //  0004  JMP		#0006
      0x4C040000,  //  0005  LDNIL	R1
      0x80040200,  //  0006  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_mode
********************************************************************/
be_local_closure(class_Matter_Session_get_mode,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(get_mode),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040108,  //  0000  GETMBR	R1	R0	K8
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_temp_ca
********************************************************************/
be_local_closure(class_Matter_Session_set_temp_ca,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(set_temp_ca),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90025201,  //  0000  SETMBR	R0	K41	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_mode
********************************************************************/
be_local_closure(class_Matter_Session_set_mode,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(set_mode),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90021001,  //  0000  SETMBR	R0	K8	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_keys
********************************************************************/
be_local_closure(class_Matter_Session_set_keys,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(set_keys),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x90024001,  //  0000  SETMBR	R0	K32	R1
      0x4C140000,  //  0001  LDNIL	R5
      0x90029C05,  //  0002  SETMBR	R0	K78	R5
      0x90024A02,  //  0003  SETMBR	R0	K37	R2
      0x90029E03,  //  0004  SETMBR	R0	K79	R3
      0x9002C804,  //  0005  SETMBR	R0	K100	R4
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: gen_CSR
********************************************************************/
be_local_closure(class_Matter_Session_gen_CSR,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(gen_CSR),
    &be_const_str_solidified,
    ( &(const binstruction[73]) {  /* code */
      0x8C040136,  //  0000  GETMET	R1	R0	K54
      0x7C040200,  //  0001  CALL	R1	1
      0xA40A1600,  //  0002  IMPORT	R2	K11
      0x8C0C0565,  //  0003  GETMET	R3	R2	K101
      0x7C0C0200,  //  0004  CALL	R3	1
      0x8C100766,  //  0005  GETMET	R4	R3	K102
      0x5C180200,  //  0006  MOVE	R6	R1
      0x7C100400,  //  0007  CALL	R4	2
      0x60140015,  //  0008  GETGBL	R5	G21
      0x58180067,  //  0009  LDCONST	R6	K103
      0x7C140200,  //  000A  CALL	R5	1
      0x40180A04,  //  000B  CONNECT	R6	R5	R4
      0x60180015,  //  000C  GETGBL	R6	G21
      0x581C0068,  //  000D  LDCONST	R7	K104
      0x7C180200,  //  000E  CALL	R6	1
      0x40180A06,  //  000F  CONNECT	R6	R5	R6
      0x60180015,  //  0010  GETGBL	R6	G21
      0x581C0069,  //  0011  LDCONST	R7	K105
      0x7C180200,  //  0012  CALL	R6	1
      0x8C1C076A,  //  0013  GETMET	R7	R3	K106
      0x5C240200,  //  0014  MOVE	R9	R1
      0x5C280A00,  //  0015  MOVE	R10	R5
      0x7C1C0600,  //  0016  CALL	R7	3
      0x60200015,  //  0017  GETGBL	R8	G21
      0x5426007F,  //  0018  LDINT	R9	128
      0x7C200200,  //  0019  CALL	R8	1
      0x8C24116B,  //  001A  GETMET	R9	R8	K107
      0x582C0005,  //  001B  LDCONST	R11	K5
      0x58300050,  //  001C  LDCONST	R12	K80
      0x7C240600,  //  001D  CALL	R9	3
      0x8C24116B,  //  001E  GETMET	R9	R8	K107
      0x602C000C,  //  001F  GETGBL	R11	G12
      0x5C300E00,  //  0020  MOVE	R12	R7
      0x7C2C0200,  //  0021  CALL	R11	1
      0x002C1750,  //  0022  ADD	R11	R11	K80
      0x58300050,  //  0023  LDCONST	R12	K80
      0x7C240600,  //  0024  CALL	R9	3
      0x8C24116B,  //  0025  GETMET	R9	R8	K107
      0x582C000D,  //  0026  LDCONST	R11	K13
      0x58300050,  //  0027  LDCONST	R12	K80
      0x7C240600,  //  0028  CALL	R9	3
      0x40241007,  //  0029  CONNECT	R9	R8	R7
      0x6024000C,  //  002A  GETGBL	R9	G12
      0x5C280A00,  //  002B  MOVE	R10	R5
      0x7C240200,  //  002C  CALL	R9	1
      0x6028000C,  //  002D  GETGBL	R10	G12
      0x5C2C0C00,  //  002E  MOVE	R11	R6
      0x7C280200,  //  002F  CALL	R10	1
      0x0024120A,  //  0030  ADD	R9	R9	R10
      0x6028000C,  //  0031  GETGBL	R10	G12
      0x5C2C1000,  //  0032  MOVE	R11	R8
      0x7C280200,  //  0033  CALL	R10	1
      0x0024120A,  //  0034  ADD	R9	R9	R10
      0x60280015,  //  0035  GETGBL	R10	G21
      0x542E00CF,  //  0036  LDINT	R11	208
      0x7C280200,  //  0037  CALL	R10	1
      0x8C2C156B,  //  0038  GETMET	R11	R10	K107
      0x5436002F,  //  0039  LDINT	R13	48
      0x58380050,  //  003A  LDCONST	R14	K80
      0x7C2C0600,  //  003B  CALL	R11	3
      0x8C2C156B,  //  003C  GETMET	R11	R10	K107
      0x54360080,  //  003D  LDINT	R13	129
      0x58380050,  //  003E  LDCONST	R14	K80
      0x7C2C0600,  //  003F  CALL	R11	3
      0x8C2C156B,  //  0040  GETMET	R11	R10	K107
      0x543600FE,  //  0041  LDINT	R13	255
      0x2C34120D,  //  0042  AND	R13	R9	R13
      0x58380050,  //  0043  LDCONST	R14	K80
      0x7C2C0600,  //  0044  CALL	R11	3
      0x402C1405,  //  0045  CONNECT	R11	R10	R5
      0x402C1406,  //  0046  CONNECT	R11	R10	R6
      0x402C1408,  //  0047  CONNECT	R11	R10	R8
      0x80041400,  //  0048  RET	1	R10
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_ipk_group_key
********************************************************************/
be_local_closure(class_Matter_Session_get_ipk_group_key,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session,     /* shared constants */
    be_str_weak(get_ipk_group_key),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C04036C,  //  0001  GETMET	R1	R1	K108
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Session
********************************************************************/
extern const bclass be_class_Matter_Expirable;
be_local_class(Matter_Session,
    38,
    &be_class_Matter_Expirable,
    be_nested_map(85,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(_temp_pk, -1), be_const_var(9) },
        { be_const_key_weak(get_device_id, -1), be_const_closure(class_Matter_Session_get_device_id_closure) },
        { be_const_key_weak(_exchange_id, 46), be_const_var(16) },
        { be_const_key_weak(_PASE, 37), be_const_int(1) },
        { be_const_key_weak(set_mode_CASE, -1), be_const_closure(class_Matter_Session_set_mode_CASE_closure) },
        { be_const_key_weak(get_ipk_group_key, 9), be_const_closure(class_Matter_Session_get_ipk_group_key_closure) },
        { be_const_key_weak(is_CASE, -1), be_const_closure(class_Matter_Session_is_CASE_closure) },
        { be_const_key_weak(_message_handler, -1), be_const_var(19) },
        { be_const_key_weak(persist_to_fabric, 2), be_const_closure(class_Matter_Session_persist_to_fabric_closure) },
        { be_const_key_weak(mode, -1), be_const_var(1) },
        { be_const_key_weak(before_remove, 58), be_const_closure(class_Matter_Session_before_remove_closure) },
        { be_const_key_weak(gen_CSR, 41), be_const_closure(class_Matter_Session_gen_CSR_closure) },
        { be_const_key_weak(__responder_pub, 23), be_const_var(31) },
        { be_const_key_weak(i2rkey, 83), be_const_var(22) },
        { be_const_key_weak(_port, 29), be_const_var(18) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_Session_init_closure) },
        { be_const_key_weak(get_admin_vendor, -1), be_const_closure(class_Matter_Session_get_admin_vendor_closure) },
        { be_const_key_weak(get_i2r, -1), be_const_closure(class_Matter_Session_get_i2r_closure) },
        { be_const_key_weak(_fabric, 73), be_const_var(2) },
        { be_const_key_weak(local_session_id, 84), be_const_var(3) },
        { be_const_key_weak(counter_rcv_validate, -1), be_const_closure(class_Matter_Session_counter_rcv_validate_closure) },
        { be_const_key_weak(get_ca, -1), be_const_closure(class_Matter_Session_get_ca_closure) },
        { be_const_key_weak(__responder_priv, -1), be_const_var(30) },
        { be_const_key_weak(_counter_snd_impl, -1), be_const_var(15) },
        { be_const_key_weak(__spake_cA, 67), be_const_var(33) },
        { be_const_key_weak(__future_initiator_session_id, -1), be_const_var(10) },
        { be_const_key_weak(__future_local_session_id, -1), be_const_var(11) },
        { be_const_key_weak(set_mode, -1), be_const_closure(class_Matter_Session_set_mode_closure) },
        { be_const_key_weak(counter_snd_next, -1), be_const_closure(class_Matter_Session_counter_snd_next_closure) },
        { be_const_key_weak(set_temp_ca, 33), be_const_closure(class_Matter_Session_set_temp_ca_closure) },
        { be_const_key_weak(get_temp_ca_pub, -1), be_const_closure(class_Matter_Session_get_temp_ca_pub_closure) },
        { be_const_key_weak(get_temp_ca, 54), be_const_closure(class_Matter_Session_get_temp_ca_closure) },
        { be_const_key_weak(get_fabric_id, -1), be_const_closure(class_Matter_Session_get_fabric_id_closure) },
        { be_const_key_weak(get_mode, -1), be_const_closure(class_Matter_Session_get_mode_closure) },
        { be_const_key_weak(initiator_session_id, 22), be_const_var(4) },
        { be_const_key_weak(set_mode_PASE, -1), be_const_closure(class_Matter_Session_set_mode_PASE_closure) },
        { be_const_key_weak(get_fabric_compressed, 43), be_const_closure(class_Matter_Session_get_fabric_compressed_closure) },
        { be_const_key_weak(__Msg2, -1), be_const_var(36) },
        { be_const_key_weak(_counter_insecure_snd, 26), be_const_var(21) },
        { be_const_key_weak(get_fabric_label, 52), be_const_closure(class_Matter_Session_get_fabric_label_closure) },
        { be_const_key_weak(last_used, -1), be_const_var(6) },
        { be_const_key_weak(close, -1), be_const_closure(class_Matter_Session_close_closure) },
        { be_const_key_weak(counter_snd, 74), be_const_var(13) },
        { be_const_key_weak(_store, -1), be_const_var(0) },
        { be_const_key_weak(_breadcrumb, 72), be_const_var(27) },
        { be_const_key_weak(_counter_insecure_rcv, -1), be_const_var(20) },
        { be_const_key_weak(get_ac, -1), be_const_closure(class_Matter_Session_get_ac_closure) },
        { be_const_key_weak(set_fabric_label, -1), be_const_closure(class_Matter_Session_set_fabric_label_closure) },
        { be_const_key_weak(get_node_id, -1), be_const_closure(class_Matter_Session_get_node_id_closure) },
        { be_const_key_weak(get_ca_pub, -1), be_const_closure(class_Matter_Session_get_ca_pub_closure) },
        { be_const_key_weak(_COUNTER_SND_INCR, 55), be_const_int(1024) },
        { be_const_key_weak(tojson, -1), be_const_closure(class_Matter_Session_tojson_closure) },
        { be_const_key_weak(hydrate_post, -1), be_const_closure(class_Matter_Session_hydrate_post_closure) },
        { be_const_key_weak(__spake_Ke, 36), be_const_var(34) },
        { be_const_key_weak(_i2r_privacy, -1), be_const_var(24) },
        { be_const_key_weak(get_fabric, 71), be_const_closure(class_Matter_Session_get_fabric_closure) },
        { be_const_key_weak(__chunked_attr_reports, -1), be_const_var(37) },
        { be_const_key_weak(_source_node_id, 66), be_const_var(7) },
        { be_const_key_weak(get_i2r_privacy, 19), be_const_closure(class_Matter_Session_get_i2r_privacy_closure) },
        { be_const_key_weak(_CASE, 57), be_const_int(2) },
        { be_const_key_weak(fromjson, -1), be_const_static_closure(class_Matter_Session_fromjson_closure) },
        { be_const_key_weak(get_noc, 60), be_const_closure(class_Matter_Session_get_noc_closure) },
        { be_const_key_weak(save, -1), be_const_closure(class_Matter_Session_save_closure) },
        { be_const_key_weak(counter_rcv, -1), be_const_var(12) },
        { be_const_key_weak(update, 56), be_const_closure(class_Matter_Session_update_closure) },
        { be_const_key_weak(_GROUP_KEY, -1), be_nested_str_weak(GroupKey_X20v1_X2E0) },
        { be_const_key_weak(peer_node_id, -1), be_const_var(26) },
        { be_const_key_weak(__Msg1, 82), be_const_var(35) },
        { be_const_key_weak(get_admin_subject, -1), be_const_closure(class_Matter_Session_get_admin_subject_closure) },
        { be_const_key_weak(get_ipk_epoch_key, -1), be_const_closure(class_Matter_Session_get_ipk_epoch_key_closure) },
        { be_const_key_weak(__initiator_pub, -1), be_const_var(32) },
        { be_const_key_weak(get_pk, -1), be_const_closure(class_Matter_Session_get_pk_closure) },
        { be_const_key_weak(is_PASE, -1), be_const_closure(class_Matter_Session_is_PASE_closure) },
        { be_const_key_weak(get_icac, -1), be_const_closure(class_Matter_Session_get_icac_closure) },
        { be_const_key_weak(resumption_id, -1), be_const_var(28) },
        { be_const_key_weak(_ip, 27), be_const_var(17) },
        { be_const_key_weak(set_keys, -1), be_const_closure(class_Matter_Session_set_keys_closure) },
        { be_const_key_weak(r2ikey, 25), be_const_var(23) },
        { be_const_key_weak(attestation_challenge, 5), be_const_var(25) },
        { be_const_key_weak(created, -1), be_const_var(5) },
        { be_const_key_weak(_temp_root_ca_certificate, -1), be_const_var(8) },
        { be_const_key_weak(get_r2i, 11), be_const_closure(class_Matter_Session_get_r2i_closure) },
        { be_const_key_weak(get_fabric_index, -1), be_const_closure(class_Matter_Session_get_fabric_index_closure) },
        { be_const_key_weak(_counter_rcv_impl, -1), be_const_var(14) },
        { be_const_key_weak(shared_secret, -1), be_const_var(29) },
    })),
    be_str_weak(Matter_Session)
);
/********************************************************************/
/* End of solidification */
