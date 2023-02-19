/* Solidification of Matter_IM_Subscription.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_IM_Subscription;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_IM_Subscription_init,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[24]) {     /* constants */
    /* K0   */  be_nested_str_weak(subscription_id),
    /* K1   */  be_nested_str_weak(session),
    /* K2   */  be_nested_str_weak(min_interval_floor),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(min_interval),
    /* K5   */  be_nested_str_weak(max_interval_ceiling),
    /* K6   */  be_nested_str_weak(max_interval),
    /* K7   */  be_nested_str_weak(fabric_filtered),
    /* K8   */  be_nested_str_weak(path_list),
    /* K9   */  be_nested_str_weak(attributes_requests),
    /* K10  */  be_nested_str_weak(matter),
    /* K11  */  be_nested_str_weak(Path),
    /* K12  */  be_nested_str_weak(endpoint),
    /* K13  */  be_nested_str_weak(cluster),
    /* K14  */  be_nested_str_weak(attribute),
    /* K15  */  be_nested_str_weak(push),
    /* K16  */  be_nested_str_weak(stop_iteration),
    /* K17  */  be_nested_str_weak(updates),
    /* K18  */  be_nested_str_weak(clear_and_arm),
    /* K19  */  be_nested_str_weak(tasmota),
    /* K20  */  be_nested_str_weak(log),
    /* K21  */  be_nested_str_weak(MTR_X3A_X20new_X20subsctiption_X20),
    /* K22  */  be_nested_str_weak(inspect),
    /* K23  */  be_const_int(2),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[64]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x90020202,  //  0001  SETMBR	R0	K1	R2
      0x88100702,  //  0002  GETMBR	R4	R3	K2
      0x14140903,  //  0003  LT	R5	R4	K3
      0x78160000,  //  0004  JMPF	R5	#0006
      0x58100003,  //  0005  LDCONST	R4	K3
      0x5416003B,  //  0006  LDINT	R5	60
      0x24140805,  //  0007  GT	R5	R4	R5
      0x78160000,  //  0008  JMPF	R5	#000A
      0x5412003B,  //  0009  LDINT	R4	60
      0x90020804,  //  000A  SETMBR	R0	K4	R4
      0x88140705,  //  000B  GETMBR	R5	R3	K5
      0x541A003B,  //  000C  LDINT	R6	60
      0x14180A06,  //  000D  LT	R6	R5	R6
      0x781A0000,  //  000E  JMPF	R6	#0010
      0x5416003B,  //  000F  LDINT	R5	60
      0x541A0E0F,  //  0010  LDINT	R6	3600
      0x24180A06,  //  0011  GT	R6	R5	R6
      0x781A0000,  //  0012  JMPF	R6	#0014
      0x54160E0F,  //  0013  LDINT	R5	3600
      0x90020C05,  //  0014  SETMBR	R0	K6	R5
      0x88180707,  //  0015  GETMBR	R6	R3	K7
      0x90020E06,  //  0016  SETMBR	R0	K7	R6
      0x60180012,  //  0017  GETGBL	R6	G18
      0x7C180000,  //  0018  CALL	R6	0
      0x90021006,  //  0019  SETMBR	R0	K8	R6
      0x60180010,  //  001A  GETGBL	R6	G16
      0x881C0709,  //  001B  GETMBR	R7	R3	K9
      0x7C180200,  //  001C  CALL	R6	1
      0xA802000F,  //  001D  EXBLK	0	#002E
      0x5C1C0C00,  //  001E  MOVE	R7	R6
      0x7C1C0000,  //  001F  CALL	R7	0
      0xB8221400,  //  0020  GETNGBL	R8	K10
      0x8C20110B,  //  0021  GETMET	R8	R8	K11
      0x7C200200,  //  0022  CALL	R8	1
      0x88240F0C,  //  0023  GETMBR	R9	R7	K12
      0x90221809,  //  0024  SETMBR	R8	K12	R9
      0x88240F0D,  //  0025  GETMBR	R9	R7	K13
      0x90221A09,  //  0026  SETMBR	R8	K13	R9
      0x88240F0E,  //  0027  GETMBR	R9	R7	K14
      0x90221C09,  //  0028  SETMBR	R8	K14	R9
      0x88240108,  //  0029  GETMBR	R9	R0	K8
      0x8C24130F,  //  002A  GETMET	R9	R9	K15
      0x5C2C1000,  //  002B  MOVE	R11	R8
      0x7C240400,  //  002C  CALL	R9	2
      0x7001FFEF,  //  002D  JMP		#001E
      0x58180010,  //  002E  LDCONST	R6	K16
      0xAC180200,  //  002F  CATCH	R6	1	0
      0xB0080000,  //  0030  RAISE	2	R0	R0
      0x60180012,  //  0031  GETGBL	R6	G18
      0x7C180000,  //  0032  CALL	R6	0
      0x90022206,  //  0033  SETMBR	R0	K17	R6
      0x8C180112,  //  0034  GETMET	R6	R0	K18
      0x7C180200,  //  0035  CALL	R6	1
      0xB81A2600,  //  0036  GETNGBL	R6	K19
      0x8C180D14,  //  0037  GETMET	R6	R6	K20
      0xB8221400,  //  0038  GETNGBL	R8	K10
      0x8C201116,  //  0039  GETMET	R8	R8	K22
      0x5C280000,  //  003A  MOVE	R10	R0
      0x7C200400,  //  003B  CALL	R8	2
      0x00222A08,  //  003C  ADD	R8	K21	R8
      0x58240017,  //  003D  LDCONST	R9	K23
      0x7C180600,  //  003E  CALL	R6	3
      0x80000000,  //  003F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: attribute_updated
********************************************************************/
be_local_closure(Matter_IM_Subscription_attribute_updated,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(path_list),
    /* K2   */  be_nested_str_weak(endpoint),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(attribute),
    /* K5   */  be_nested_str_weak(updates),
    /* K6   */  be_nested_str_weak(push),
    /* K7   */  be_const_int(1),
    }),
    be_str_weak(attribute_updated),
    &be_const_str_solidified,
    ( &(const binstruction[39]) {  /* code */
      0x580C0000,  //  0000  LDCONST	R3	K0
      0x6010000C,  //  0001  GETGBL	R4	G12
      0x88140101,  //  0002  GETMBR	R5	R0	K1
      0x7C100200,  //  0003  CALL	R4	1
      0x14100604,  //  0004  LT	R4	R3	R4
      0x7812001F,  //  0005  JMPF	R4	#0026
      0x88100101,  //  0006  GETMBR	R4	R0	K1
      0x94100803,  //  0007  GETIDX	R4	R4	R3
      0x88140902,  //  0008  GETMBR	R5	R4	K2
      0x4C180000,  //  0009  LDNIL	R6
      0x1C140A06,  //  000A  EQ	R5	R5	R6
      0x74160003,  //  000B  JMPT	R5	#0010
      0x88140902,  //  000C  GETMBR	R5	R4	K2
      0x88180302,  //  000D  GETMBR	R6	R1	K2
      0x1C140A06,  //  000E  EQ	R5	R5	R6
      0x78160013,  //  000F  JMPF	R5	#0024
      0x88140903,  //  0010  GETMBR	R5	R4	K3
      0x4C180000,  //  0011  LDNIL	R6
      0x1C140A06,  //  0012  EQ	R5	R5	R6
      0x74160003,  //  0013  JMPT	R5	#0018
      0x88140903,  //  0014  GETMBR	R5	R4	K3
      0x88180303,  //  0015  GETMBR	R6	R1	K3
      0x1C140A06,  //  0016  EQ	R5	R5	R6
      0x7816000B,  //  0017  JMPF	R5	#0024
      0x88140904,  //  0018  GETMBR	R5	R4	K4
      0x4C180000,  //  0019  LDNIL	R6
      0x1C140A06,  //  001A  EQ	R5	R5	R6
      0x74160003,  //  001B  JMPT	R5	#0020
      0x88140904,  //  001C  GETMBR	R5	R4	K4
      0x88180304,  //  001D  GETMBR	R6	R1	K4
      0x1C140A06,  //  001E  EQ	R5	R5	R6
      0x78160003,  //  001F  JMPF	R5	#0024
      0x88140105,  //  0020  GETMBR	R5	R0	K5
      0x8C140B06,  //  0021  GETMET	R5	R5	K6
      0x5C1C0200,  //  0022  MOVE	R7	R1
      0x7C140400,  //  0023  CALL	R5	2
      0x000C0707,  //  0024  ADD	R3	R3	K7
      0x7001FFDA,  //  0025  JMP		#0001
      0x80000000,  //  0026  RET	0
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
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(updates),
    /* K1   */  be_nested_str_weak(clear),
    /* K2   */  be_nested_str_weak(expiration),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(millis),
    /* K5   */  be_nested_str_weak(max_interval),
    /* K6   */  be_nested_str_weak(MAX_INTERVAL_MARGIN),
    }),
    be_str_weak(clear_and_arm),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0xB8060600,  //  0003  GETNGBL	R1	K3
      0x8C040304,  //  0004  GETMET	R1	R1	K4
      0x7C040200,  //  0005  CALL	R1	1
      0x88080105,  //  0006  GETMBR	R2	R0	K5
      0x880C0106,  //  0007  GETMBR	R3	R0	K6
      0x04080403,  //  0008  SUB	R2	R2	R3
      0x540E03E7,  //  0009  LDINT	R3	1000
      0x08080403,  //  000A  MUL	R2	R2	R3
      0x00040202,  //  000B  ADD	R1	R1	R2
      0x90020401,  //  000C  SETMBR	R0	K2	R1
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_IM_Subscription
********************************************************************/
be_local_class(Matter_IM_Subscription,
    8,
    NULL,
    be_nested_map(12,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(expiration, -1), be_const_var(6) },
        { be_const_key_weak(updates, 9), be_const_var(7) },
        { be_const_key_weak(fabric_filtered, -1), be_const_var(5) },
        { be_const_key_weak(max_interval, 10), be_const_var(4) },
        { be_const_key_weak(session, -1), be_const_var(1) },
        { be_const_key_weak(min_interval, -1), be_const_var(3) },
        { be_const_key_weak(MAX_INTERVAL_MARGIN, -1), be_const_int(10) },
        { be_const_key_weak(attribute_updated, 4), be_const_closure(Matter_IM_Subscription_attribute_updated_closure) },
        { be_const_key_weak(subscription_id, -1), be_const_var(0) },
        { be_const_key_weak(clear_and_arm, -1), be_const_closure(Matter_IM_Subscription_clear_and_arm_closure) },
        { be_const_key_weak(path_list, 11), be_const_var(2) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_IM_Subscription_init_closure) },
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
** Solidified function: new_subscription
********************************************************************/
be_local_closure(Matter_IM_Subscription_Shop_new_subscription,   /* name */
  be_nested_proto(
    10,                          /* nstack */
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
    ( &(const binstruction[32]) {  /* code */
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
      0x5C1C0800,  //  0017  MOVE	R7	R4
      0x5C200200,  //  0018  MOVE	R8	R1
      0x5C240400,  //  0019  MOVE	R9	R2
      0x7C140800,  //  001A  CALL	R5	4
      0x88180108,  //  001B  GETMBR	R6	R0	K8
      0x8C180D09,  //  001C  GETMET	R6	R6	K9
      0x5C200A00,  //  001D  MOVE	R8	R5
      0x7C180400,  //  001E  CALL	R6	2
      0x80040A00,  //  001F  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(Matter_IM_Subscription_Shop_every_second,   /* name */
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
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(subs),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(time_reached),
    /* K4   */  be_nested_str_weak(expiration),
    /* K5   */  be_nested_str_weak(im),
    /* K6   */  be_nested_str_weak(send_subscribe_update),
    /* K7   */  be_nested_str_weak(clear_and_arm),
    /* K8   */  be_const_int(1),
    }),
    be_str_weak(every_second),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x6008000C,  //  0001  GETGBL	R2	G12
      0x880C0101,  //  0002  GETMBR	R3	R0	K1
      0x7C080200,  //  0003  CALL	R2	1
      0x14080202,  //  0004  LT	R2	R1	R2
      0x780A000E,  //  0005  JMPF	R2	#0015
      0x88080101,  //  0006  GETMBR	R2	R0	K1
      0x94080401,  //  0007  GETIDX	R2	R2	R1
      0xB80E0400,  //  0008  GETNGBL	R3	K2
      0x8C0C0703,  //  0009  GETMET	R3	R3	K3
      0x88140504,  //  000A  GETMBR	R5	R2	K4
      0x7C0C0400,  //  000B  CALL	R3	2
      0x780E0005,  //  000C  JMPF	R3	#0013
      0x880C0105,  //  000D  GETMBR	R3	R0	K5
      0x8C0C0706,  //  000E  GETMET	R3	R3	K6
      0x5C140400,  //  000F  MOVE	R5	R2
      0x7C0C0400,  //  0010  CALL	R3	2
      0x8C0C0507,  //  0011  GETMET	R3	R2	K7
      0x7C0C0200,  //  0012  CALL	R3	1
      0x00040308,  //  0013  ADD	R1	R1	K8
      0x7001FFEB,  //  0014  JMP		#0001
      0x80000000,  //  0015  RET	0
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
** Solidified function: attribute_updated
********************************************************************/
be_local_closure(Matter_IM_Subscription_Shop_attribute_updated,   /* name */
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
    /* K2   */  be_nested_str_weak(attribute_updated),
    /* K3   */  be_const_int(1),
    }),
    be_str_weak(attribute_updated),
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
** Solidified class: Matter_IM_Subscription_Shop
********************************************************************/
be_local_class(Matter_IM_Subscription_Shop,
    2,
    NULL,
    be_nested_map(8,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(attribute_updated, -1), be_const_closure(Matter_IM_Subscription_Shop_attribute_updated_closure) },
        { be_const_key_weak(every_second, -1), be_const_closure(Matter_IM_Subscription_Shop_every_second_closure) },
        { be_const_key_weak(subs, -1), be_const_var(0) },
        { be_const_key_weak(init, 5), be_const_closure(Matter_IM_Subscription_Shop_init_closure) },
        { be_const_key_weak(remove_by_session, -1), be_const_closure(Matter_IM_Subscription_Shop_remove_by_session_closure) },
        { be_const_key_weak(im, 6), be_const_var(1) },
        { be_const_key_weak(get_by_id, -1), be_const_closure(Matter_IM_Subscription_Shop_get_by_id_closure) },
        { be_const_key_weak(new_subscription, 0), be_const_closure(Matter_IM_Subscription_Shop_new_subscription_closure) },
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
