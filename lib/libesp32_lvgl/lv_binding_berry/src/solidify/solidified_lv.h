/* Solidification of lv.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

/********************************************************************
** Solidified function: lv_module_init
********************************************************************/
be_local_closure(lv_module_init,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(lv),
    /* K1   */  be_nested_str_weak(member),
    /* K2   */  be_nested_str_weak(scr_act),
    /* K3   */  be_nested_str_weak(screen_active),
    /* K4   */  be_nested_str_weak(img),
    /* K5   */  be_nested_str_weak(image),
    /* K6   */  be_nested_str_weak(disp),
    /* K7   */  be_nested_str_weak(display),
    /* K8   */  be_nested_str_weak(btn),
    /* K9   */  be_nested_str_weak(button),
    /* K10  */  be_nested_str_weak(btnmatrix),
    /* K11  */  be_nested_str_weak(buttonmatrix),
    }),
    be_str_weak(lv_module_init),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x6004000B,  //  0000  GETGBL	R1	G11
      0x58080000,  //  0001  LDCONST	R2	K0
      0x7C040200,  //  0002  CALL	R1	1
      0x88080101,  //  0003  GETMBR	R2	R0	K1
      0x90060202,  //  0004  SETMBR	R1	K1	R2
      0x88080103,  //  0005  GETMBR	R2	R0	K3
      0x90060402,  //  0006  SETMBR	R1	K2	R2
      0x88080105,  //  0007  GETMBR	R2	R0	K5
      0x90060802,  //  0008  SETMBR	R1	K4	R2
      0x88080107,  //  0009  GETMBR	R2	R0	K7
      0x90060C02,  //  000A  SETMBR	R1	K6	R2
      0x88080109,  //  000B  GETMBR	R2	R0	K9
      0x90061002,  //  000C  SETMBR	R1	K8	R2
      0x8808010B,  //  000D  GETMBR	R2	R0	K11
      0x90061402,  //  000E  SETMBR	R1	K10	R2
      0x80040200,  //  000F  RET	1	R1
    })
  )
);
/*******************************************************************/

/********************************************************************/
/* End of solidification */
