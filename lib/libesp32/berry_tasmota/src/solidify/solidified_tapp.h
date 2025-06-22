/* Solidification of tapp.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
extern const bclass be_class_Tapp;
// compact class 'Tapp' ktab size: 16, total: 17 (saved 8 bytes)
static const bvalue be_ktab_class_Tapp[16] = {
  /* K0   */  be_nested_str(tasmota),
  /* K1   */  be_nested_str(add_driver),
  /* K2   */  be_nested_str(autoexec_dir),
  /* K3   */  be_nested_str(_X2F_X2Eextensions_X2F),
  /* K4   */  be_nested_str(_X2F),
  /* K5   */  be_const_class(be_class_Tapp),
  /* K6   */  be_nested_str(path),
  /* K7   */  be_nested_str(string),
  /* K8   */  be_nested_str(listdir),
  /* K9   */  be_nested_str(endswith),
  /* K10  */  be_nested_str(_X2Etapp),
  /* K11  */  be_nested_str(log),
  /* K12  */  be_nested_str(TAP_X3A_X20Loaded_X20Tasmota_X20App_X20_X27_X25s_X25s_X27),
  /* K13  */  be_const_int(2),
  /* K14  */  be_nested_str(load),
  /* K15  */  be_nested_str(stop_iteration),
};


extern const bclass be_class_Tapp;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Tapp_init,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tapp,     /* shared constants */
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: autoexec
********************************************************************/
be_local_closure(class_Tapp_autoexec,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tapp,     /* shared constants */
    &be_const_str_autoexec,
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x8C040102,  //  0000  GETMET	R1	R0	K2
      0x580C0003,  //  0001  LDCONST	R3	K3
      0x7C040400,  //  0002  CALL	R1	2
      0x8C040102,  //  0003  GETMET	R1	R0	K2
      0x580C0004,  //  0004  LDCONST	R3	K4
      0x7C040400,  //  0005  CALL	R1	2
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: autoexec_dir
********************************************************************/
be_local_closure(class_Tapp_autoexec_dir,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tapp,     /* shared constants */
    &be_const_str_autoexec_dir,
    &be_const_str_solidified,
    ( &(const binstruction[33]) {  /* code */
      0x58040005,  //  0000  LDCONST	R1	K5
      0xA40A0C00,  //  0001  IMPORT	R2	K6
      0xA40E0E00,  //  0002  IMPORT	R3	K7
      0x60100010,  //  0003  GETGBL	R4	G16
      0x8C140508,  //  0004  GETMET	R5	R2	K8
      0x5C1C0000,  //  0005  MOVE	R7	R0
      0x7C140400,  //  0006  CALL	R5	2
      0x7C100200,  //  0007  CALL	R4	1
      0xA8020013,  //  0008  EXBLK	0	#001D
      0x5C140800,  //  0009  MOVE	R5	R4
      0x7C140000,  //  000A  CALL	R5	0
      0x8C180709,  //  000B  GETMET	R6	R3	K9
      0x5C200A00,  //  000C  MOVE	R8	R5
      0x5824000A,  //  000D  LDCONST	R9	K10
      0x7C180600,  //  000E  CALL	R6	3
      0x781A000B,  //  000F  JMPF	R6	#001C
      0xB81A1600,  //  0010  GETNGBL	R6	K11
      0x601C0018,  //  0011  GETGBL	R7	G24
      0x5820000C,  //  0012  LDCONST	R8	K12
      0x5C240000,  //  0013  MOVE	R9	R0
      0x5C280A00,  //  0014  MOVE	R10	R5
      0x7C1C0600,  //  0015  CALL	R7	3
      0x5820000D,  //  0016  LDCONST	R8	K13
      0x7C180400,  //  0017  CALL	R6	2
      0xB81A0000,  //  0018  GETNGBL	R6	K0
      0x8C180D0E,  //  0019  GETMET	R6	R6	K14
      0x00200005,  //  001A  ADD	R8	R0	R5
      0x7C180400,  //  001B  CALL	R6	2
      0x7001FFEB,  //  001C  JMP		#0009
      0x5810000F,  //  001D  LDCONST	R4	K15
      0xAC100200,  //  001E  CATCH	R4	1	0
      0xB0080000,  //  001F  RAISE	2	R0	R0
      0x80000000,  //  0020  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Tapp
********************************************************************/
be_local_class(Tapp,
    0,
    NULL,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(init, -1), be_const_closure(class_Tapp_init_closure) },
        { be_const_key(autoexec, -1), be_const_closure(class_Tapp_autoexec_closure) },
        { be_const_key(autoexec_dir, -1), be_const_static_closure(class_Tapp_autoexec_dir_closure) },
    })),
    (bstring*) &be_const_str_Tapp
);

/********************************************************************
** Solidified function: _anonymous_
********************************************************************/
be_local_closure(_anonymous_,   /* name */
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
    /* K0   */  be_const_class(be_class_Tapp),
    }),
    &be_const_str__anonymous_,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0xB4000000,  //  0001  CLASS	K0
      0x5C080200,  //  0002  MOVE	R2	R1
      0x7C080000,  //  0003  CALL	R2	0
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified module: tapp
********************************************************************/
be_local_module(tapp,
    "tapp",
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(init, -1), be_const_closure(_anonymous__closure) },
    }))
);
BE_EXPORT_VARIABLE be_define_const_native_module(tapp);
/********************************************************************/
/********************************************************************/
/* End of solidification */
