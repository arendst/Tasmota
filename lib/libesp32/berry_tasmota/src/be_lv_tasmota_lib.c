/********************************************************************
 * Tasmota LVGL lv_signal_bars widget
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_LVGL

extern int lv0_start(bvm *vm);

extern int lv0_register_button_encoder(bvm *vm);  // add buttons with encoder logic

extern int lv0_load_montserrat_font(bvm *vm);
extern int lv0_load_seg7_font(bvm *vm);
extern int lv0_load_robotocondensed_latin1_font(bvm *vm);

extern int lv0_screenshot(bvm *vm);
extern int lv0_load_freetype_font(bvm *vm);

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(lv_tasmota_init,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[23]) {     /* constants */
    /* K0   */  be_nested_str(lv),
    /* K1   */  be_nested_str(start),
    /* K2   */  be_nested_str(font_montserrat),
    /* K3   */  be_nested_str(montserrat_font),
    /* K4   */  be_nested_str(font_seg7),
    /* K5   */  be_nested_str(seg7_font),
    /* K6   */  be_nested_str(load_freetype_font),
    /* K7   */  be_nested_str(register_button_encoder),
    /* K8   */  be_nested_str(screenshot),
    /* K9   */  be_nested_str(clock_icon),
    /* K10  */  be_nested_str(lv_clock_icon),
    /* K11  */  be_nested_str(signal_arcs),
    /* K12  */  be_nested_str(lv_signal_arcs),
    /* K13  */  be_nested_str(signal_bars),
    /* K14  */  be_nested_str(lv_signal_bars),
    /* K15  */  be_nested_str(wifi_arcs_icon),
    /* K16  */  be_nested_str(lv_wifi_arcs_icon),
    /* K17  */  be_nested_str(wifi_arcs),
    /* K18  */  be_nested_str(lv_wifi_arcs),
    /* K19  */  be_nested_str(wifi_bars_icon),
    /* K20  */  be_nested_str(lv_wifi_bars_icon),
    /* K21  */  be_nested_str(wifi_bars),
    /* K22  */  be_nested_str(lv_wifi_bars),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[33]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x90060202,  //  0002  SETMBR	R1	K1	R2
      0x88080102,  //  0003  GETMBR	R2	R0	K2
      0x90060402,  //  0004  SETMBR	R1	K2	R2
      0x88080102,  //  0005  GETMBR	R2	R0	K2
      0x90060602,  //  0006  SETMBR	R1	K3	R2
      0x88080104,  //  0007  GETMBR	R2	R0	K4
      0x90060802,  //  0008  SETMBR	R1	K4	R2
      0x88080104,  //  0009  GETMBR	R2	R0	K4
      0x90060A02,  //  000A  SETMBR	R1	K5	R2
      0x88080106,  //  000B  GETMBR	R2	R0	K6
      0x90060C02,  //  000C  SETMBR	R1	K6	R2
      0x88080107,  //  000D  GETMBR	R2	R0	K7
      0x90060E02,  //  000E  SETMBR	R1	K7	R2
      0x88080108,  //  000F  GETMBR	R2	R0	K8
      0x90061002,  //  0010  SETMBR	R1	K8	R2
      0xB80A1400,  //  0011  GETNGBL	R2	K10
      0x90061202,  //  0012  SETMBR	R1	K9	R2
      0xB80A1800,  //  0013  GETNGBL	R2	K12
      0x90061602,  //  0014  SETMBR	R1	K11	R2
      0xB80A1C00,  //  0015  GETNGBL	R2	K14
      0x90061A02,  //  0016  SETMBR	R1	K13	R2
      0xB80A2000,  //  0017  GETNGBL	R2	K16
      0x90061E02,  //  0018  SETMBR	R1	K15	R2
      0xB80A2400,  //  0019  GETNGBL	R2	K18
      0x90062202,  //  001A  SETMBR	R1	K17	R2
      0xB80A2800,  //  001B  GETNGBL	R2	K20
      0x90062602,  //  001C  SETMBR	R1	K19	R2
      0xB80A2C00,  //  001D  GETNGBL	R2	K22
      0x90062A02,  //  001E  SETMBR	R1	K21	R2
      0x4C080000,  //  001F  LDNIL	R2
      0x80040400,  //  0020  RET	1	R2
    })
  )
);
/*******************************************************************/

/* @const_object_info_begin
module lv_tasmota (scope: global) {
    init, closure(lv_tasmota_init_closure)

    start, func(lv0_start)

    font_montserrat, func(lv0_load_montserrat_font)
    montserrat_font, func(lv0_load_montserrat_font)
    font_seg7, func(lv0_load_seg7_font)
    seg7_font, func(lv0_load_seg7_font)
    register_button_encoder, func(lv0_register_button_encoder)
    screenshot, func(lv0_screenshot)

    load_freetype_font, func(lv0_load_freetype_font)
}
@const_object_info_end */
#include "be_fixed_lv_tasmota.h"

#endif // USE_LVGL