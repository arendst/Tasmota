/* Solidification of Matter_IM_Subscription.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_IM_Subscription;

/********************************************************************
** Solidified function: remove_self
********************************************************************/
be_local_closure(Matter_IM_Subscription_remove_self,   /* name */
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
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(log),
    /* K2   */  be_nested_str_weak(MTR_X3A_X20Remove_Sub_X20sub_id_X3D),
    /* K3   */  be_nested_str_weak(subscription_id),
    /* K4   */  be_nested_str_weak(subs),
    /* K5   */  be_nested_str_weak(remove_sub),
    }),
    be_str_weak(remove_self),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x600C0008,  //  0002  GETGBL	R3	G8
      0x88100103,  //  0003  GETMBR	R4	R0	K3
      0x7C0C0200,  //  0004  CALL	R3	1
      0x000E0403,  //  0005  ADD	R3	K2	R3
      0x7C040400,  //  0006  CALL	R1	2
      0x88040104,  //  0007  GETMBR	R1	R0	K4
      0x8C040305,  //  0008  GETMET	R1	R1	K5
      0x5C0C0000,  //  0009  MOVE	R3	R0
      0x7C040400,  //  000A  CALL	R1	2
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_IM_Subscription_init,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[25]) {     /* constants */
    /* K0   */  be_nested_str_weak(subs),
    /* K1   */  be_nested_str_weak(subscription_id),
    /* K2   */  be_nested_str_weak(session),
    /* K3   */  be_nested_str_weak(min_interval_floor),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(min_interval),
    /* K6   */  be_nested_str_weak(max_interval_ceiling),
    /* K7   */  be_nested_str_weak(max_interval),
    /* K8   */  be_nested_str_weak(fabric_filtered),
    /* K9   */  be_nested_str_weak(path_list),
    /* K10  */  be_nested_str_weak(attributes_requests),
    /* K11  */  be_nested_str_weak(matter),
    /* K12  */  be_nested_str_weak(Path),
    /* K13  */  be_nested_str_weak(endpoint),
    /* K14  */  be_nested_str_weak(cluster),
    /* K15  */  be_nested_str_weak(attribute),
    /* K16  */  be_nested_str_weak(push),
    /* K17  */  be_nested_str_weak(stop_iteration),
    /* K18  */  be_nested_str_weak(updates),
    /* K19  */  be_nested_str_weak(clear_and_arm),
    /* K20  */  be_nested_str_weak(tasmota),
    /* K21  */  be_nested_str_weak(log),
    /* K22  */  be_nested_str_weak(MTR_X3A_X20new_X20subsctiption_X20),
    /* K23  */  be_nested_str_weak(inspect),
    /* K24  */  be_const_int(3),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[65]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x90020202,  //  0001  SETMBR	R0	K1	R2
      0x90020403,  //  0002  SETMBR	R0	K2	R3
      0x88140903,  //  0003  GETMBR	R5	R4	K3
      0x14180B04,  //  0004  LT	R6	R5	K4
      0x781A0000,  //  0005  JMPF	R6	#0007
      0x58140004,  //  0006  LDCONST	R5	K4
      0x541A003B,  //  0007  LDINT	R6	60
      0x24180A06,  //  0008  GT	R6	R5	R6
      0x781A0000,  //  0009  JMPF	R6	#000B
      0x5416003B,  //  000A  LDINT	R5	60
      0x90020A05,  //  000B  SETMBR	R0	K5	R5
      0x88180906,  //  000C  GETMBR	R6	R4	K6
      0x541E003B,  //  000D  LDINT	R7	60
      0x141C0C07,  //  000E  LT	R7	R6	R7
      0x781E0000,  //  000F  JMPF	R7	#0011
      0x541A003B,  //  0010  LDINT	R6	60
      0x541E0E0F,  //  0011  LDINT	R7	3600
      0x241C0C07,  //  0012  GT	R7	R6	R7
      0x781E0000,  //  0013  JMPF	R7	#0015
      0x541A0E0F,  //  0014  LDINT	R6	3600
      0x90020E06,  //  0015  SETMBR	R0	K7	R6
      0x881C0908,  //  0016  GETMBR	R7	R4	K8
      0x90021007,  //  0017  SETMBR	R0	K8	R7
      0x601C0012,  //  0018  GETGBL	R7	G18
      0x7C1C0000,  //  0019  CALL	R7	0
      0x90021207,  //  001A  SETMBR	R0	K9	R7
      0x601C0010,  //  001B  GETGBL	R7	G16
      0x8820090A,  //  001C  GETMBR	R8	R4	K10
      0x7C1C0200,  //  001D  CALL	R7	1
      0xA802000F,  //  001E  EXBLK	0	#002F
      0x5C200E00,  //  001F  MOVE	R8	R7
      0x7C200000,  //  0020  CALL	R8	0
      0xB8261600,  //  0021  GETNGBL	R9	K11
      0x8C24130C,  //  0022  GETMET	R9	R9	K12
      0x7C240200,  //  0023  CALL	R9	1
      0x8828110D,  //  0024  GETMBR	R10	R8	K13
      0x90261A0A,  //  0025  SETMBR	R9	K13	R10
      0x8828110E,  //  0026  GETMBR	R10	R8	K14
      0x90261C0A,  //  0027  SETMBR	R9	K14	R10
      0x8828110F,  //  0028  GETMBR	R10	R8	K15
      0x90261E0A,  //  0029  SETMBR	R9	K15	R10
      0x88280109,  //  002A  GETMBR	R10	R0	K9
      0x8C281510,  //  002B  GETMET	R10	R10	K16
      0x5C301200,  //  002C  MOVE	R12	R9
      0x7C280400,  //  002D  CALL	R10	2
      0x7001FFEF,  //  002E  JMP		#001F
      0x581C0011,  //  002F  LDCONST	R7	K17
      0xAC1C0200,  //  0030  CATCH	R7	1	0
      0xB0080000,  //  0031  RAISE	2	R0	R0
      0x601C0012,  //  0032  GETGBL	R7	G18
      0x7C1C0000,  //  0033  CALL	R7	0
      0x90022407,  //  0034  SETMBR	R0	K18	R7
      0x8C1C0113,  //  0035  GETMET	R7	R0	K19
      0x7C1C0200,  //  0036  CALL	R7	1
      0xB81E2800,  //  0037  GETNGBL	R7	K20
      0x8C1C0F15,  //  0038  GETMET	R7	R7	K21
      0xB8261600,  //  0039  GETNGBL	R9	K11
      0x8C241317,  //  003A  GETMET	R9	R9	K23
      0x5C2C0000,  //  003B  MOVE	R11	R0
      0x7C240400,  //  003C  CALL	R9	2
      0x00262C09,  //  003D  ADD	R9	K22	R9
      0x58280018,  //  003E  LDCONST	R10	K24
      0x7C1C0600,  //  003F  CALL	R7	3
      0x80000000,  //  0040  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _add_attribute_unique_path
********************************************************************/
be_local_closure(Matter_IM_Subscription__add_attribute_unique_path,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(updates),
    /* K2   */  be_nested_str_weak(endpoint),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(attribute),
    /* K5   */  be_const_int(1),
    /* K6   */  be_nested_str_weak(push),
    }),
    be_str_weak(_add_attribute_unique_path),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0x600C000C,  //  0001  GETGBL	R3	G12
      0x88100101,  //  0002  GETMBR	R4	R0	K1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x140C0403,  //  0004  LT	R3	R2	R3
      0x780E0010,  //  0005  JMPF	R3	#0017
      0x880C0101,  //  0006  GETMBR	R3	R0	K1
      0x940C0602,  //  0007  GETIDX	R3	R3	R2
      0x88100702,  //  0008  GETMBR	R4	R3	K2
      0x88140302,  //  0009  GETMBR	R5	R1	K2
      0x1C100805,  //  000A  EQ	R4	R4	R5
      0x78120008,  //  000B  JMPF	R4	#0015
      0x88100703,  //  000C  GETMBR	R4	R3	K3
      0x88140303,  //  000D  GETMBR	R5	R1	K3
      0x1C100805,  //  000E  EQ	R4	R4	R5
      0x78120004,  //  000F  JMPF	R4	#0015
      0x88100704,  //  0010  GETMBR	R4	R3	K4
      0x88140304,  //  0011  GETMBR	R5	R1	K4
      0x1C100805,  //  0012  EQ	R4	R4	R5
      0x78120000,  //  0013  JMPF	R4	#0015
      0x80000800,  //  0014  RET	0
      0x00080505,  //  0015  ADD	R2	R2	K5
      0x7001FFE9,  //  0016  JMP		#0001
      0x880C0101,  //  0017  GETMBR	R3	R0	K1
      0x8C0C0706,  //  0018  GETMET	R3	R3	K6
      0x5C140200,  //  0019  MOVE	R5	R1
      0x7C0C0400,  //  001A  CALL	R3	2
      0x80000000,  //  001B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: clear_and_arm
********************************************************************/
be_local_closure(Matter_IM_Subscription_clear_and_arm,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(updates),
    /* K1   */  be_nested_str_weak(clear),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(millis),
    /* K4   */  be_nested_str_weak(expiration),
    /* K5   */  be_nested_str_weak(max_interval),
    /* K6   */  be_nested_str_weak(MAX_INTERVAL_MARGIN),
    /* K7   */  be_nested_str_weak(not_before),
    /* K8   */  be_nested_str_weak(min_interval),
    /* K9   */  be_const_int(1),
    }),
    be_str_weak(clear_and_arm),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0xB8060400,  //  0003  GETNGBL	R1	K2
      0x8C040303,  //  0004  GETMET	R1	R1	K3
      0x7C040200,  //  0005  CALL	R1	1
      0x88080105,  //  0006  GETMBR	R2	R0	K5
      0x880C0106,  //  0007  GETMBR	R3	R0	K6
      0x04080403,  //  0008  SUB	R2	R2	R3
      0x540E03E7,  //  0009  LDINT	R3	1000
      0x08080403,  //  000A  MUL	R2	R2	R3
      0x00080202,  //  000B  ADD	R2	R1	R2
      0x90020802,  //  000C  SETMBR	R0	K4	R2
      0x88080108,  //  000D  GETMBR	R2	R0	K8
      0x540E03E7,  //  000E  LDINT	R3	1000
      0x08080403,  //  000F  MUL	R2	R2	R3
      0x00080202,  //  0010  ADD	R2	R1	R2
      0x04080509,  //  0011  SUB	R2	R2	K9
      0x90020E02,  //  0012  SETMBR	R0	K7	R2
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: attribute_updated_ctx
********************************************************************/
be_local_closure(Matter_IM_Subscription_attribute_updated_ctx,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(path_list),
    /* K2   */  be_nested_str_weak(endpoint),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(attribute),
    /* K5   */  be_nested_str_weak(_add_attribute_unique_path),
    /* K6   */  be_const_int(1),
    }),
    be_str_weak(attribute_updated_ctx),
    &be_const_str_solidified,
    ( &(const binstruction[38]) {  /* code */
      0x580C0000,  //  0000  LDCONST	R3	K0
      0x6010000C,  //  0001  GETGBL	R4	G12
      0x88140101,  //  0002  GETMBR	R5	R0	K1
      0x7C100200,  //  0003  CALL	R4	1
      0x14100604,  //  0004  LT	R4	R3	R4
      0x7812001E,  //  0005  JMPF	R4	#0025
      0x88100101,  //  0006  GETMBR	R4	R0	K1
      0x94100803,  //  0007  GETIDX	R4	R4	R3
      0x88140902,  //  0008  GETMBR	R5	R4	K2
      0x4C180000,  //  0009  LDNIL	R6
      0x1C140A06,  //  000A  EQ	R5	R5	R6
      0x74160003,  //  000B  JMPT	R5	#0010
      0x88140902,  //  000C  GETMBR	R5	R4	K2
      0x88180302,  //  000D  GETMBR	R6	R1	K2
      0x1C140A06,  //  000E  EQ	R5	R5	R6
      0x78160012,  //  000F  JMPF	R5	#0023
      0x88140903,  //  0010  GETMBR	R5	R4	K3
      0x4C180000,  //  0011  LDNIL	R6
      0x1C140A06,  //  0012  EQ	R5	R5	R6
      0x74160003,  //  0013  JMPT	R5	#0018
      0x88140903,  //  0014  GETMBR	R5	R4	K3
      0x88180303,  //  0015  GETMBR	R6	R1	K3
      0x1C140A06,  //  0016  EQ	R5	R5	R6
      0x7816000A,  //  0017  JMPF	R5	#0023
      0x88140904,  //  0018  GETMBR	R5	R4	K4
      0x4C180000,  //  0019  LDNIL	R6
      0x1C140A06,  //  001A  EQ	R5	R5	R6
      0x74160003,  //  001B  JMPT	R5	#0020
      0x88140904,  //  001C  GETMBR	R5	R4	K4
      0x88180304,  //  001D  GETMBR	R6	R1	K4
      0x1C140A06,  //  001E  EQ	R5	R5	R6
      0x78160002,  //  001F  JMPF	R5	#0023
      0x8C140105,  //  0020  GETMET	R5	R0	K5
      0x5C1C0200,  //  0021  MOVE	R7	R1
      0x7C140400,  //  0022  CALL	R5	2
      0x000C0706,  //  0023  ADD	R3	R3	K6
      0x7001FFDB,  //  0024  JMP		#0001
      0x80000000,  //  0025  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_IM_Subscription
********************************************************************/
be_local_class(Matter_IM_Subscription,
    10,
    NULL,
    be_nested_map(16,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(remove_self, -1), be_const_closure(Matter_IM_Subscription_remove_self_closure) },
        { be_const_key_weak(min_interval, -1), be_const_var(4) },
        { be_const_key_weak(subs, -1), be_const_var(0) },
        { be_const_key_weak(init, 15), be_const_closure(Matter_IM_Subscription_init_closure) },
        { be_const_key_weak(subscription_id, -1), be_const_var(1) },
        { be_const_key_weak(updates, -1), be_const_var(9) },
        { be_const_key_weak(MAX_INTERVAL_MARGIN, -1), be_const_int(5) },
        { be_const_key_weak(session, 11), be_const_var(2) },
        { be_const_key_weak(expiration, 14), be_const_var(8) },
        { be_const_key_weak(fabric_filtered, -1), be_const_var(6) },
        { be_const_key_weak(_add_attribute_unique_path, 9), be_const_closure(Matter_IM_Subscription__add_attribute_unique_path_closure) },
        { be_const_key_weak(max_interval, -1), be_const_var(5) },
        { be_const_key_weak(attribute_updated_ctx, -1), be_const_closure(Matter_IM_Subscription_attribute_updated_ctx_closure) },
        { be_const_key_weak(clear_and_arm, 12), be_const_closure(Matter_IM_Subscription_clear_and_arm_closure) },
        { be_const_key_weak(not_before, -1), be_const_var(7) },
        { be_const_key_weak(path_list, -1), be_const_var(3) },
    })),
    be_str_weak(Matter_IM_Subscription)
);
/*******************************************************************/

void be_load_Matter_IM_Subscription_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_IM_Subscription);
    be_setglobal(vm, "Matter_IM_Subscription");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_IM_Subscription_Shop;

/********************************************************************
** Solidified function: get_by_id
********************************************************************/
be_local_closure(Matter_IM_Subscription_Shop_get_by_id,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(subs),
    /* K2   */  be_nested_str_weak(subscription_id),
    /* K3   */  be_const_int(1),
    }),
    be_str_weak(get_by_id),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0x600C000C,  //  0001  GETGBL	R3	G12
      0x88100101,  //  0002  GETMBR	R4	R0	K1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x140C0403,  //  0004  LT	R3	R2	R3
      0x780E0009,  //  0005  JMPF	R3	#0010
      0x880C0101,  //  0006  GETMBR	R3	R0	K1
      0x940C0602,  //  0007  GETIDX	R3	R3	R2
      0x880C0702,  //  0008  GETMBR	R3	R3	K2
      0x1C0C0601,  //  0009  EQ	R3	R3	R1
      0x780E0002,  //  000A  JMPF	R3	#000E
      0x880C0101,  //  000B  GETMBR	R3	R0	K1
      0x940C0602,  //  000C  GETIDX	R3	R3	R2
      0x80040600,  //  000D  RET	1	R3
      0x00080503,  //  000E  ADD	R2	R2	K3
      0x7001FFF0,  //  000F  JMP		#0001
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_250ms
********************************************************************/
be_local_closure(Matter_IM_Subscription_Shop_every_250ms,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(subs),
    /* K2   */  be_nested_str_weak(updates),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(time_reached),
    /* K5   */  be_nested_str_weak(not_before),
    /* K6   */  be_nested_str_weak(im),
    /* K7   */  be_nested_str_weak(send_subscribe_update),
    /* K8   */  be_nested_str_weak(clear_and_arm),
    /* K9   */  be_const_int(1),
    /* K10  */  be_nested_str_weak(expiration),
    }),
    be_str_weak(every_250ms),
    &be_const_str_solidified,
    ( &(const binstruction[48]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x6008000C,  //  0001  GETGBL	R2	G12
      0x880C0101,  //  0002  GETMBR	R3	R0	K1
      0x7C080200,  //  0003  CALL	R2	1
      0x14080202,  //  0004  LT	R2	R1	R2
      0x780A0013,  //  0005  JMPF	R2	#001A
      0x88080101,  //  0006  GETMBR	R2	R0	K1
      0x94080401,  //  0007  GETIDX	R2	R2	R1
      0x600C000C,  //  0008  GETGBL	R3	G12
      0x88100502,  //  0009  GETMBR	R4	R2	K2
      0x7C0C0200,  //  000A  CALL	R3	1
      0x240C0700,  //  000B  GT	R3	R3	K0
      0x780E000A,  //  000C  JMPF	R3	#0018
      0xB80E0600,  //  000D  GETNGBL	R3	K3
      0x8C0C0704,  //  000E  GETMET	R3	R3	K4
      0x88140505,  //  000F  GETMBR	R5	R2	K5
      0x7C0C0400,  //  0010  CALL	R3	2
      0x780E0005,  //  0011  JMPF	R3	#0018
      0x880C0106,  //  0012  GETMBR	R3	R0	K6
      0x8C0C0707,  //  0013  GETMET	R3	R3	K7
      0x5C140400,  //  0014  MOVE	R5	R2
      0x7C0C0400,  //  0015  CALL	R3	2
      0x8C0C0508,  //  0016  GETMET	R3	R2	K8
      0x7C0C0200,  //  0017  CALL	R3	1
      0x00040309,  //  0018  ADD	R1	R1	K9
      0x7001FFE6,  //  0019  JMP		#0001
      0x58040000,  //  001A  LDCONST	R1	K0
      0x6008000C,  //  001B  GETGBL	R2	G12
      0x880C0101,  //  001C  GETMBR	R3	R0	K1
      0x7C080200,  //  001D  CALL	R2	1
      0x14080202,  //  001E  LT	R2	R1	R2
      0x780A000E,  //  001F  JMPF	R2	#002F
      0x88080101,  //  0020  GETMBR	R2	R0	K1
      0x94080401,  //  0021  GETIDX	R2	R2	R1
      0xB80E0600,  //  0022  GETNGBL	R3	K3
      0x8C0C0704,  //  0023  GETMET	R3	R3	K4
      0x8814050A,  //  0024  GETMBR	R5	R2	K10
      0x7C0C0400,  //  0025  CALL	R3	2
      0x780E0005,  //  0026  JMPF	R3	#002D
      0x880C0106,  //  0027  GETMBR	R3	R0	K6
      0x8C0C0707,  //  0028  GETMET	R3	R3	K7
      0x5C140400,  //  0029  MOVE	R5	R2
      0x7C0C0400,  //  002A  CALL	R3	2
      0x8C0C0508,  //  002B  GETMET	R3	R2	K8
      0x7C0C0200,  //  002C  CALL	R3	1
      0x00040309,  //  002D  ADD	R1	R1	K9
      0x7001FFEB,  //  002E  JMP		#001B
      0x80000000,  //  002F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: new_subscription
********************************************************************/
be_local_closure(Matter_IM_Subscription_Shop_new_subscription,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(random),
    /* K2   */  be_const_int(2),
    /* K3   */  be_nested_str_weak(get),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(get_by_id),
    /* K6   */  be_nested_str_weak(matter),
    /* K7   */  be_nested_str_weak(IM_Subscription),
    /* K8   */  be_nested_str_weak(subs),
    /* K9   */  be_nested_str_weak(push),
    }),
    be_str_weak(new_subscription),
    &be_const_str_solidified,
    ( &(const binstruction[33]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0x8C100701,  //  0001  GETMET	R4	R3	K1
      0x58180002,  //  0002  LDCONST	R6	K2
      0x7C100400,  //  0003  CALL	R4	2
      0x8C100903,  //  0004  GETMET	R4	R4	K3
      0x58180004,  //  0005  LDCONST	R6	K4
      0x581C0002,  //  0006  LDCONST	R7	K2
      0x7C100600,  //  0007  CALL	R4	3
      0x8C140105,  //  0008  GETMET	R5	R0	K5
      0x5C1C0800,  //  0009  MOVE	R7	R4
      0x7C140400,  //  000A  CALL	R5	2
      0x78160008,  //  000B  JMPF	R5	#0015
      0x8C140701,  //  000C  GETMET	R5	R3	K1
      0x581C0002,  //  000D  LDCONST	R7	K2
      0x7C140400,  //  000E  CALL	R5	2
      0x8C140B03,  //  000F  GETMET	R5	R5	K3
      0x581C0004,  //  0010  LDCONST	R7	K4
      0x58200002,  //  0011  LDCONST	R8	K2
      0x7C140600,  //  0012  CALL	R5	3
      0x5C100A00,  //  0013  MOVE	R4	R5
      0x7001FFF2,  //  0014  JMP		#0008
      0xB8160C00,  //  0015  GETNGBL	R5	K6
      0x8C140B07,  //  0016  GETMET	R5	R5	K7
      0x5C1C0000,  //  0017  MOVE	R7	R0
      0x5C200800,  //  0018  MOVE	R8	R4
      0x5C240200,  //  0019  MOVE	R9	R1
      0x5C280400,  //  001A  MOVE	R10	R2
      0x7C140A00,  //  001B  CALL	R5	5
      0x88180108,  //  001C  GETMBR	R6	R0	K8
      0x8C180D09,  //  001D  GETMET	R6	R6	K9
      0x5C200A00,  //  001E  MOVE	R8	R5
      0x7C180400,  //  001F  CALL	R6	2
      0x80040A00,  //  0020  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: attribute_updated_ctx
********************************************************************/
be_local_closure(Matter_IM_Subscription_Shop_attribute_updated_ctx,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(subs),
    /* K2   */  be_nested_str_weak(attribute_updated_ctx),
    /* K3   */  be_const_int(1),
    }),
    be_str_weak(attribute_updated_ctx),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x580C0000,  //  0000  LDCONST	R3	K0
      0x6010000C,  //  0001  GETGBL	R4	G12
      0x88140101,  //  0002  GETMBR	R5	R0	K1
      0x7C100200,  //  0003  CALL	R4	1
      0x14100604,  //  0004  LT	R4	R3	R4
      0x78120007,  //  0005  JMPF	R4	#000E
      0x88100101,  //  0006  GETMBR	R4	R0	K1
      0x94100803,  //  0007  GETIDX	R4	R4	R3
      0x8C100902,  //  0008  GETMET	R4	R4	K2
      0x5C180200,  //  0009  MOVE	R6	R1
      0x5C1C0400,  //  000A  MOVE	R7	R2
      0x7C100600,  //  000B  CALL	R4	3
      0x000C0703,  //  000C  ADD	R3	R3	K3
      0x7001FFF2,  //  000D  JMP		#0001
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_IM_Subscription_Shop_init,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(im),
    /* K1   */  be_nested_str_weak(subs),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x60080012,  //  0001  GETGBL	R2	G18
      0x7C080000,  //  0002  CALL	R2	0
      0x90020202,  //  0003  SETMBR	R0	K1	R2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_sub
********************************************************************/
be_local_closure(Matter_IM_Subscription_Shop_remove_sub,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(subs),
    /* K2   */  be_nested_str_weak(remove),
    /* K3   */  be_const_int(1),
    }),
    be_str_weak(remove_sub),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0x600C000C,  //  0001  GETGBL	R3	G12
      0x88100101,  //  0002  GETMBR	R4	R0	K1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x140C0403,  //  0004  LT	R3	R2	R3
      0x780E000A,  //  0005  JMPF	R3	#0011
      0x880C0101,  //  0006  GETMBR	R3	R0	K1
      0x940C0602,  //  0007  GETIDX	R3	R3	R2
      0x1C0C0601,  //  0008  EQ	R3	R3	R1
      0x780E0004,  //  0009  JMPF	R3	#000F
      0x880C0101,  //  000A  GETMBR	R3	R0	K1
      0x8C0C0702,  //  000B  GETMET	R3	R3	K2
      0x5C140400,  //  000C  MOVE	R5	R2
      0x7C0C0400,  //  000D  CALL	R3	2
      0x70020000,  //  000E  JMP		#0010
      0x00080503,  //  000F  ADD	R2	R2	K3
      0x7001FFEF,  //  0010  JMP		#0001
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_by_session
********************************************************************/
be_local_closure(Matter_IM_Subscription_Shop_remove_by_session,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(subs),
    /* K2   */  be_nested_str_weak(session),
    /* K3   */  be_nested_str_weak(remove),
    /* K4   */  be_const_int(1),
    }),
    be_str_weak(remove_by_session),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0x600C000C,  //  0001  GETGBL	R3	G12
      0x88100101,  //  0002  GETMBR	R4	R0	K1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x140C0403,  //  0004  LT	R3	R2	R3
      0x780E000B,  //  0005  JMPF	R3	#0012
      0x880C0101,  //  0006  GETMBR	R3	R0	K1
      0x940C0602,  //  0007  GETIDX	R3	R3	R2
      0x880C0702,  //  0008  GETMBR	R3	R3	K2
      0x1C0C0601,  //  0009  EQ	R3	R3	R1
      0x780E0004,  //  000A  JMPF	R3	#0010
      0x880C0101,  //  000B  GETMBR	R3	R0	K1
      0x8C0C0703,  //  000C  GETMET	R3	R3	K3
      0x5C140400,  //  000D  MOVE	R5	R2
      0x7C0C0400,  //  000E  CALL	R3	2
      0x70020000,  //  000F  JMP		#0011
      0x00080504,  //  0010  ADD	R2	R2	K4
      0x7001FFEE,  //  0011  JMP		#0001
      0x80000000,  //  0012  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_IM_Subscription_Shop
********************************************************************/
be_local_class(Matter_IM_Subscription_Shop,
    2,
    NULL,
    be_nested_map(9,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(get_by_id, -1), be_const_closure(Matter_IM_Subscription_Shop_get_by_id_closure) },
        { be_const_key_weak(attribute_updated_ctx, -1), be_const_closure(Matter_IM_Subscription_Shop_attribute_updated_ctx_closure) },
        { be_const_key_weak(every_250ms, -1), be_const_closure(Matter_IM_Subscription_Shop_every_250ms_closure) },
        { be_const_key_weak(remove_sub, 1), be_const_closure(Matter_IM_Subscription_Shop_remove_sub_closure) },
        { be_const_key_weak(new_subscription, 3), be_const_closure(Matter_IM_Subscription_Shop_new_subscription_closure) },
        { be_const_key_weak(subs, -1), be_const_var(0) },
        { be_const_key_weak(im, 7), be_const_var(1) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_IM_Subscription_Shop_init_closure) },
        { be_const_key_weak(remove_by_session, -1), be_const_closure(Matter_IM_Subscription_Shop_remove_by_session_closure) },
    })),
    be_str_weak(Matter_IM_Subscription_Shop)
);
/*******************************************************************/

void be_load_Matter_IM_Subscription_Shop_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_IM_Subscription_Shop);
    be_setglobal(vm, "Matter_IM_Subscription_Shop");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
