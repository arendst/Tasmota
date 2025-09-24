/* Solidification of extension_manager.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
extern const bclass be_class_Extension_manager;
// compact class 'Extension_manager' ktab size: 164, total: 269 (saved 840 bytes)
static const bvalue be_ktab_class_Extension_manager[164] = {
  /* K0   */  be_nested_str(webserver),
  /* K1   */  be_nested_str(check_privileged_access),
  /* K2   */  be_nested_str(has_arg),
  /* K3   */  be_nested_str(store),
  /* K4   */  be_nested_str(page_extensions_store),
  /* K5   */  be_nested_str(page_extensions_mgr),
  /* K6   */  be_nested_str(tasmota),
  /* K7   */  be_nested_str(arch),
  /* K8   */  be_nested_str(0x_X2508X),
  /* K9   */  be_nested_str(version),
  /* K10  */  be_nested_str(_X25s_X25s_X3Fa_X3D_X25s_X26v_X3D_X25s),
  /* K11  */  be_nested_str(EXT_REPO),
  /* K12  */  be_nested_str(EXT_REPO_MANIFEST),
  /* K13  */  be_nested_str(log),
  /* K14  */  be_nested_str(EXT_X3A_X20fetching_X20extensions_X20manifest_X20_X27_X25s_X27),
  /* K15  */  be_const_int(3),
  /* K16  */  be_nested_str(webclient),
  /* K17  */  be_nested_str(begin),
  /* K18  */  be_nested_str(GET),
  /* K19  */  be_nested_str(EXT_X3A_X20error_X20fetching_X20manifest_X20_X25s),
  /* K20  */  be_const_int(2),
  /* K21  */  be_nested_str(Error_X20fetching_X20manifest_X20code_X3D_X25s),
  /* K22  */  be_nested_str(webclient_error),
  /* K23  */  be_nested_str(get_string),
  /* K24  */  be_nested_str(close),
  /* K25  */  be_nested_str(EXT_X3A_X20exception_X20_X27_X25s_X27_X20_X2D_X20_X27_X25s_X27),
  /* K26  */  be_nested_str(on),
  /* K27  */  be_nested_str(_X2Fext),
  /* K28  */  be_nested_str(HTTP_GET),
  /* K29  */  be_nested_str(HTTP_POST),
  /* K30  */  be_nested_str(string),
  /* K31  */  be_nested_str(content_start),
  /* K32  */  be_nested_str(Extensions_X20Manager),
  /* K33  */  be_nested_str(content_send_style),
  /* K34  */  be_nested_str(content_send),
  /* K35  */  be_nested_str(_X3Cdiv_X20style_X3D_X27padding_X3A0px_X205px_X3Btext_X2Dalign_X3Acenter_X3B_X27_X3E_X3Ch3_X3E_X3Chr_X3EExtension_X20Manager_X3Chr_X3E_X3C_X2Fh3_X3E_X3C_X2Fdiv_X3E),
  /* K36  */  be_nested_str_long(_X3Cscript_X3Efunction_X20loadext_X28_X29_X20_X7Beb_X28_X27store_X27_X29_X2Edisabled_X3Dtrue_X3Bx_X3Dnew_X20XMLHttpRequest_X28_X29_X3Bx_X2Etimeout_X3D4000_X3Bx_X2Eonreadystatechange_X20_X3D_X20_X28_X29_X20_X3D_X3E_X20_X7Bif_X28x_X2EreadyState_X3D_X3D4_X29_X7Bif_X28x_X2Estatus_X3D_X3D200_X29_X7Beb_X28_X27inet_X27_X29_X2Estyle_X2Edisplay_X3D_X27none_X27_X3Beb_X28_X27store_X27_X29_X2EouterHTML_X3Dx_X2EresponseText_X3B_X7D_X7D_X7D_X3Bx_X2Eopen_X28_X27GET_X27_X2C_X27_X3Fstore_X3D_X27_X29_X3Bx_X2Esend_X28_X29_X3B_X7Dwindow_X2Eonload_X3Dfunction_X28_X29_X7Bloadext_X28_X29_X3B_X7D_X3Bfunction_X20toggleDesc_X28id_X29_X20_X7Bvar_X20desc_X20_X3D_X20document_X2EgetElementById_X28_X27desc_X2D_X27_X20_X2B_X20id_X29_X3Bvar_X20arrow_X20_X3D_X20document_X2EgetElementById_X28_X27arrow_X2D_X27_X20_X2B_X20id_X29_X3Bif_X20_X28desc_X2Estyle_X2Edisplay_X20_X3D_X3D_X3D_X20_X27none_X27_X20_X7C_X7C_X20desc_X2Estyle_X2Edisplay_X20_X3D_X3D_X3D_X20_X27_X27_X29_X20_X7Bdesc_X2Estyle_X2Edisplay_X20_X3D_X20_X27block_X27_X3Barrow_X2EinnerHTML_X20_X3D_X20_X27_XE2_X96_XBC_X27_X3B_X7D_X20else_X20_X7Bdesc_X2Estyle_X2Edisplay_X20_X3D_X20_X27none_X27_X3Barrow_X2EinnerHTML_X20_X3D_X20_X27_XE2_X96_XB6_X27_X3B_X7D_X7Dfunction_X20filterExtensions_X28query_X29_X20_X7Bvar_X20items_X20_X3D_X20document_X2EgetElementsByClassName_X28_X27ext_X2Dstore_X2Ditem_X27_X29_X3Bquery_X20_X3D_X20query_X2EtoLowerCase_X28_X29_X3Bfor_X20_X28var_X20i_X20_X3D_X200_X3B_X20i_X20_X3C_X20items_X2Elength_X3B_X20i_X2B_X2B_X29_X20_X7Bvar_X20name_X20_X3D_X20items_X5Bi_X5D_X2EgetElementsByClassName_X28_X27ext_X2Dname_X27_X29_X5B0_X5D_X2EtextContent_X2EtoLowerCase_X28_X29_X3Bvar_X20desc_X20_X3D_X20items_X5Bi_X5D_X2EgetElementsByClassName_X28_X27ext_X2Ddesc_X27_X29_X5B0_X5D_X2EtextContent_X2EtoLowerCase_X28_X29_X3Bif_X20_X28name_X2Eincludes_X28query_X29_X20_X7C_X7C_X20desc_X2Eincludes_X28query_X29_X29_X20_X7Bitems_X5Bi_X5D_X2Estyle_X2Edisplay_X20_X3D_X20_X27block_X27_X3B_X7D_X20else_X20_X7Bitems_X5Bi_X5D_X2Estyle_X2Edisplay_X20_X3D_X20_X27none_X27_X3B_X7D_X7D_X7D_X3C_X2Fscript_X3E),
  /* K37  */  be_nested_str_long(_X3Cfieldset_X20style_X3D_X27padding_X3A0_X205px_X3B_X27_X3E_X3Cstyle_X3E_X2Eext_X2Ditem_X7Bwidth_X3Amin_X2Dcontent_X3Bmin_X2Dwidth_X3A100_X25_X3B_X7D_X2Eext_X2Ditem_X20small_X7Bdisplay_X3Ablock_X3Bword_X2Dwrap_X3Abreak_X2Dword_X3Boverflow_X2Dwrap_X3Abreak_X2Dword_X3Bwhite_X2Dspace_X3Anormal_X3Bpadding_X2Dright_X3A5px_X3Bpadding_X2Dtop_X3A2px_X3B_X7D_X2Eext_X2Dcontrols_X7Bdisplay_X3Aflex_X3Bgap_X3A8px_X3Balign_X2Ditems_X3Acenter_X3Bmargin_X2Dtop_X3A4px_X3Bpadding_X3A0px_X7D_X2Ebtn_X2Dsmall_X7Bpadding_X3A0_X206px_X3Bline_X2Dheight_X3A1_X2E8rem_X3Bfont_X2Dsize_X3A0_X2E9rem_X3Bmin_X2Dwidth_X3Aauto_X3Bwidth_X3Aauto_X3Bflex_X2Dshrink_X3A0_X3B_X7Dform_X7Bpadding_X2Dtop_X3A0px_X3Bpadding_X2Dbottom_X3A0px_X3B_X7D_X2Erunning_X2Dindicator_X7Bdisplay_X3Ainline_X2Dblock_X3Bwidth_X3A8px_X3Bheight_X3A8px_X3Bborder_X2Dradius_X3A50_X25_X3Bmargin_X2Dright_X3A8px_X3Bbackground_X3Avar_X28_X2D_X2Dc_btnsvhvr_X29_X3Banimation_X3Apulse_X201_X2E5s_X20infinite_X3B_X7D_X40keyframes_X20pulse_X7B0_X25_X7Bopacity_X3A1_X3B_X7D50_X25_X7Bopacity_X3A0_X2E5_X3B_X7D100_X25_X7Bopacity_X3A1_X3B_X7D_X7D_X2Estore_X2Dheader_X7Bdisplay_X3Aflex_X3Bjustify_X2Dcontent_X3Aspace_X2Dbetween_X3Balign_X2Ditems_X3Acenter_X3Bmargin_X2Dbottom_X3A10px_X3B_X7D_X2Estore_X2Dstats_X7Bfont_X2Dsize_X3A0_X2E9em_X3Bcolor_X3Avar_X28_X2D_X2Dc_in_X29_X3B_X7D_X2Eext_X2Dstore_X2Ditem_X7Bbackground_X3Avar_X28_X2D_X2Dc_bg_X29_X3Bborder_X2Dradius_X3A0_X2E3em_X3Bmargin_X2Dbottom_X3A5px_X3Bpadding_X3A4px_X3B_X7D_X2Eext_X2Dheader_X7Bdisplay_X3Aflex_X3Bjustify_X2Dcontent_X3Aspace_X2Dbetween_X3Balign_X2Ditems_X3Acenter_X3Bcursor_X3Apointer_X3Buser_X2Dselect_X3Anone_X3Bpadding_X3A5px_X3B_X7D_X2Eext_X2Dtitle_X7Bdisplay_X3Aflex_X3Balign_X2Ditems_X3Acenter_X3Bgap_X3A6px_X3Bflex_X3A1_X3Bpadding_X3A0_X3B_X7D_X2Eext_X2Dname_X7Bfont_X2Dweight_X3Abold_X3B_X7D_X2Eext_X2Dversion_X7Bfont_X2Dsize_X3A0_X2E8em_X3B_X7D_X2Eext_X2Darrow_X7Bcolor_X3Avar_X28_X2D_X2Dc_in_X29_X3Bfont_X2Dsize_X3A0_X2E8em_X3B_X7D_X2Eext_X2Dbadges_X7Bpadding_X3A0_X3B_X7D_X2Eext_X2Ddetails_X7Bwidth_X3Amin_X2Dcontent_X3Bmin_X2Dwidth_X3A100_X25_X3Bpadding_X3A0_X3Bdisplay_X3Anone_X3B_X7D_X2Eext_X2Ddesc_X7Bcolor_X3Avar_X28_X2D_X2Dc_in_X29_X3Bfont_X2Dsize_X3A0_X2E8em_X3Bline_X2Dheight_X3A1_X2E4_X3Bdisplay_X3Ablock_X3Bword_X2Dwrap_X3Abreak_X2Dword_X3Boverflow_X2Dwrap_X3Abreak_X2Dword_X3Bwhite_X2Dspace_X3Anormal_X3Bpadding_X3A0_X205px_X3B_X7D_X2Eext_X2Dactions_X7Bdisplay_X3Aflex_X3Bgap_X3A8px_X3Bpadding_X3A5px_X3B_X7D_X2Ebtn_X2Daction_X7Bpadding_X3A0_X2012px_X3Bline_X2Dheight_X3A1_X2E8em_X3Bfont_X2Dsize_X3A0_X2E9em_X3Bflex_X3A1_X3B_X7D_X2Einstalled_X2Dbadge_X7Bborder_X2Dcolor_X3Avar_X28_X2D_X2Dc_btnhvr_X29_X3Bpadding_X3A0px_X204px_X3Bborder_X2Dradius_X3A4px_X3Bfont_X2Dsize_X3A0_X2E7em_X3Bborder_X2Dwidth_X3A2px_X3Bborder_X2Dstyle_X3Asolid_X3Bmargin_X2Dright_X3A3px_X3B_X7D_X2Eupdate_X2Dbadge_X7Bbackground_X3Avar_X28_X2D_X2Dc_btnhvr_X29_X3Bpadding_X3A2px_X206px_X3Bborder_X2Dradius_X3A4px_X3Bfont_X2Dsize_X3A0_X2E7em_X3Bmargin_X2Dright_X3A3px_X3Banimation_X3Apulse_X202s_X20infinite_X3B_X7D_X40keyframes_X20pulse_X7B0_X25_X7Bopacity_X3A1_X3B_X7D50_X25_X7Bopacity_X3A0_X2E7_X3B_X7D100_X25_X7Bopacity_X3A1_X3B_X7D_X7D_X3C_X2Fstyle_X3E_X3Clegend_X3E_X3Cb_X20title_X3D_X27Running_X20extensions_X27_X3E_X26nbsp_X3BInstalled_X20extensions_X3C_X2Fb_X3E_X3C_X2Flegend_X3E),
  /* K38  */  be_nested_str(list_extensions_in_fs),
  /* K39  */  be_const_int(0),
  /* K40  */  be_nested_str(_X3Chr_X20style_X3D_X27margin_X3A2px_X200_X200_X200_X3B_X27_X3E),
  /* K41  */  be_nested_str(get_by_index),
  /* K42  */  be_nested_str(html_escape),
  /* K43  */  be_nested_str(read_extension_manifest),
  /* K44  */  be_nested_str(_ext),
  /* K45  */  be_nested_str(contains),
  /* K46  */  be_nested_str(_X20_X3Cspan_X20class_X3D_X27running_X2Dindicator_X27_X20title_X3D_X27Running_X27_X3E_X3C_X2Fspan_X3E),
  /* K47  */  be_nested_str(),
  /* K48  */  be_nested_str(find),
  /* K49  */  be_nested_str(autorun),
  /* K50  */  be_nested_str(style_X3D_X27background_X3Avar_X28_X2D_X2Dc_btnsvhvr_X29_X3B_X27),
  /* K51  */  be_nested_str(style_X3D_X27background_X3Avar_X28_X2D_X2Dc_btnoff_X29_X3B_X27),
  /* K52  */  be_nested_str(_X3Cdiv_X20class_X3D_X27ext_X2Ditem_X27_X3E),
  /* K53  */  be_nested_str(_X3Cspan_X20title_X3D_X27path_X3A_X20_X25s_X27_X3E_X3Cb_X3E_X25s_X3C_X2Fb_X3E_X25s_X3C_X2Fspan_X3E_X3Cbr_X3E),
  /* K54  */  be_nested_str(name),
  /* K55  */  be_nested_str(_X3Csmall_X3E_X25s_X3C_X2Fsmall_X3E),
  /* K56  */  be_nested_str(description),
  /* K57  */  be_nested_str(_X3Cdiv_X20class_X3D_X27ext_X2Dcontrols_X27_X20style_X3D_X27padding_X2Dtop_X3A0px_X3Bpadding_X2Dbottom_X3A0px_X3B_X27_X3E),
  /* K58  */  be_nested_str(_X3Cform_X20action_X3D_X27_X2Fext_X27_X20method_X3D_X27post_X27_X20class_X3D_X27ext_X2Dcontrols_X27_X3E),
  /* K59  */  be_nested_str(_X3Cbutton_X20type_X3D_X27submit_X27_X20class_X3D_X27btn_X2Dsmall_X27_X20_X25s_X20name_X3D_X27_X25s_X25s_X27_X3E_X25s_X3C_X2Fbutton_X3E),
  /* K60  */  be_nested_str(s),
  /* K61  */  be_nested_str(r),
  /* K62  */  be_nested_str(Running),
  /* K63  */  be_nested_str(Stopped),
  /* K64  */  be_nested_str(_X3Cbutton_X20type_X3D_X27submit_X27_X20class_X3D_X27btn_X2Dsmall_X27_X20_X25s_X20name_X3D_X27_X25s_X25s_X27_X3EAuto_X2Drun_X3A_X20_X25s_X3C_X2Fbutton_X3E),
  /* K65  */  be_nested_str(a),
  /* K66  */  be_nested_str(A),
  /* K67  */  be_nested_str(ON),
  /* K68  */  be_nested_str(OFF),
  /* K69  */  be_nested_str(_X3Cbutton_X20type_X3D_X27submit_X27_X20class_X3D_X27btn_X2Dsmall_X20bred_X27_X20name_X3D_X27d_X25s_X27_X20onclick_X3D_X27return_X20confirm_X28_X22Confirm_X20deletion_X20of_X20_X25s_X22_X29_X27_X3EUninstall_X3C_X2Fbutton_X3E),
  /* K70  */  be_nested_str(_X3C_X2Fform_X3E_X3C_X2Fdiv_X3E_X3C_X2Fdiv_X3E),
  /* K71  */  be_const_int(1),
  /* K72  */  be_nested_str(_X3Cdiv_X3E_X3Csmall_X3E_X3Ci_X3ENo_X20installed_X20extension_X2E_X3C_X2Fi_X3E_X3C_X2Fsmall_X3E_X3C_X2Fp_X3E),
  /* K73  */  be_nested_str(_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
  /* K74  */  be_nested_str(_X3Cdiv_X20style_X3D_X27padding_X3A0px_X205px_X3Btext_X2Dalign_X3Acenter_X3B_X27_X3E_X3Ch3_X3E_X3Chr_X3EOnline_X20Store_X3Chr_X20style_X3D_X27margin_X2Dbottom_X3A0_X3B_X27_X3E_X3Cspan_X20id_X3D_X27inet_X27_X20style_X3D_X27font_X2Dsize_X3Asmall_X3Bfont_X2Dweight_X3Anormal_X3B_X27_X27_X3E_X26nbsp_X3B_X28This_X20feature_X20requires_X20an_X20internet_X20connection_X29_X3C_X2Fspan_X3E_X3C_X2Fh3_X3E_X3C_X2Fdiv_X3E),
  /* K75  */  be_nested_str(_X3Cb_X20id_X3D_X27store_X27_X3E_X5B_X20_X3Cspan_X20style_X3D_X27color_X3Avar_X28_X2D_X2Dc_btnsv_X29_X3B_X27_X3ELoading_X20from_X20Store_X2E_X2E_X2E_X3C_X2Fspan_X3E_X20_X5D_X3C_X2Fb_X3E),
  /* K76  */  be_nested_str(content_button),
  /* K77  */  be_nested_str(BUTTON_MANAGEMENT),
  /* K78  */  be_nested_str(content_stop),
  /* K79  */  be_nested_str(_X3Cp_X3E_X3C_X2Fp_X3E_X3Cform_X20id_X3Dbut_part_mgr_X20style_X3D_X27display_X3A_X20block_X3B_X27_X20action_X3D_X27ext_X27_X20method_X3D_X27get_X27_X3E_X3Cbutton_X3EExtension_X20Manager_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
  /* K80  */  be_nested_str(path),
  /* K81  */  be_nested_str(tapp_name),
  /* K82  */  be_nested_str(endswith),
  /* K83  */  be_nested_str(_X2Etapp),
  /* K84  */  be_nested_str(_X25s_X25s_X25s),
  /* K85  */  be_nested_str(EXT_REPO_FOLDER),
  /* K86  */  be_nested_str(EXT_X3A_X20installing_X20from_X20_X27_X25s_X27),
  /* K87  */  be_nested_str(check_or_create_dir),
  /* K88  */  be_nested_str(EXT_FOLDER),
  /* K89  */  be_nested_str(_X25s_X25s),
  /* K90  */  be_nested_str(EXT_X3A_X20return_code_X3D_X25s),
  /* K91  */  be_nested_str(write_file),
  /* K92  */  be_nested_str(exists),
  /* K93  */  be_nested_str(EXT_X3A_X20successfully_X20installed_X20_X27_X25s_X27_X20_X25s_X20bytes),
  /* K94  */  be_nested_str(could_X20not_X20download_X20into_X20_X27_X25s_X27_X20ret_X3D_X25s),
  /* K95  */  be_nested_str(io_error),
  /* K96  */  be_const_class(be_class_Extension_manager),
  /* K97  */  be_nested_str(listdir),
  /* K98  */  be_nested_str(_X2Etapp_),
  /* K99  */  be_nested_str(push),
  /* K100 */  be_nested_str(stop_iteration),
  /* K101 */  be_nested_str(add_driver),
  /* K102 */  be_nested_str(sortedmap),
  /* K103 */  be_nested_str(list_extensions),
  /* K104 */  be_nested_str(EXT_X3A_X20unable_X20to_X20read_X20details_X20from_X20_X27_X25s_X27),
  /* K105 */  be_nested_str(json),
  /* K106 */  be_nested_str(load),
  /* K107 */  be_nested_str(EXT_X3A_X20unable_X20to_X20parse_X20manifest_X20line_X20_X27_X25s_X27),
  /* K108 */  be_nested_str(file),
  /* K109 */  be_nested_str(EXT_X3A_X20manifest_X20is_X20missing_X20_X27name_X2Ffile_X2Fversion_X27_X20in_X20map_X20_X27_X25s_X27),
  /* K110 */  be_nested_str(_X5Bno_X20description_X5D),
  /* K111 */  be_nested_str(author),
  /* K112 */  be_nested_str(_X23),
  /* K113 */  be_nested_str(_),
  /* K114 */  be_nested_str(_X2F),
  /* K115 */  be_nested_str(content_open),
  /* K116 */  be_nested_str(text_X2Fhtml),
  /* K117 */  be_nested_str(load_manifest),
  /* K118 */  be_nested_str(_X3Cb_X20id_X3D_X27store_X27_X3E_X5B_X20_X3Cspan_X20style_X3D_X27color_X3Avar_X28_X2D_X2Dc_btnrst_X29_X3B_X27_X3EError_X20loading_X20manifest_X2E_X3C_X2Fspan_X3E_X20_X5D_X3C_X2Fb_X3E),
  /* K119 */  be_nested_str(_X3Cp_X3E_X3Csmall_X3E_X25s_X3C_X2Fsmall_X3E_X3C_X2Fp_X3E),
  /* K120 */  be_nested_str(content_close),
  /* K121 */  be_nested_str(count),
  /* K122 */  be_nested_str(_X22name_X22_X3A),
  /* K123 */  be_nested_str(_X3Cfieldset_X20id_X3D_X27store_X27_X3E),
  /* K124 */  be_nested_str(_X3Cdiv_X20class_X3D_X27store_X2Dheader_X27_X3E_X3Cspan_X3EBrowse_X20Extensions_X3C_X2Fspan_X3E_X3Cspan_X20class_X3D_X27store_X2Dstats_X27_X3E_X25s_X20available_X3C_X2Fspan_X3E_X3C_X2Fdiv_X3E),
  /* K125 */  be_nested_str(_X3Cinput_X20type_X3D_X27text_X27_X20placeholder_X3D_X27Search_X20extensions_X2E_X2E_X2E_X27_X20onkeyup_X3D_X27filterExtensions_X28this_X2Evalue_X29_X27_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
  /* K126 */  be_nested_str(list_installed_ext),
  /* K127 */  be_nested_str(_X0A),
  /* K128 */  be_nested_str(manifest_decode),
  /* K129 */  be_nested_str(version_string),
  /* K130 */  be_nested_str(replace),
  /* K131 */  be_nested_str(_X5Cn),
  /* K132 */  be_nested_str(_X3Cbr_X3E),
  /* K133 */  be_nested_str(_X3Cdiv_X20class_X3D_X27ext_X2Dstore_X2Ditem_X27_X3E_X3Cdiv_X20class_X3D_X27ext_X2Dheader_X27_X20onclick_X3D_X27toggleDesc_X28_X22_X25s_X22_X29_X27_X3E_X3Cdiv_X20class_X3D_X27ext_X2Dtitle_X27_X3E_X3Cspan_X20class_X3D_X27ext_X2Dname_X27_X3E_X25s_X3C_X2Fspan_X3E_X3Cspan_X20class_X3D_X27ext_X2Dversion_X27_X3E_X3Csmall_X3E_X25s_X3C_X2Fsmall_X3E_X3C_X2Fspan_X3E_X3C_X2Fdiv_X3E),
  /* K134 */  be_nested_str(_X3Cdiv_X20class_X3D_X27ext_X2Dbadges_X27_X3E_X3Cspan_X20class_X3D_X27update_X2Dbadge_X27_X3EUpgrade_X3C_X2Fspan_X3E_X3C_X2Fdiv_X3E),
  /* K135 */  be_nested_str(_X3Cdiv_X20class_X3D_X27ext_X2Dbadges_X27_X3E_X3Cspan_X20class_X3D_X27installed_X2Dbadge_X27_X3EInstalled_X3C_X2Fspan_X3E_X3C_X2Fdiv_X3E),
  /* K136 */  be_nested_str(_X3Cspan_X20id_X3D_X27arrow_X2D_X25s_X27_X20class_X3D_X27ext_X2Darrow_X27_X3E_XE2_X96_XB6_X3C_X2Fspan_X3E_X3C_X2Fdiv_X3E_X3Cdiv_X20id_X3D_X27desc_X2D_X25s_X27_X20class_X3D_X27ext_X2Ddetails_X27_X3E_X3Cdiv_X20class_X3D_X27ext_X2Ddesc_X27_X3E_X25s),
  /* K137 */  be_nested_str(_X3Cbr_X3E_X25s_X20_XE2_X86_X92_X20_X25s),
  /* K138 */  be_nested_str(_X3C_X2Fdiv_X3E_X3Cform_X20action_X3D_X27_X2Fext_X27_X20method_X3D_X27post_X27_X20class_X3D_X27ext_X2Dactions_X27_X3E_X3Cdiv_X20style_X3D_X27width_X3A30_X25_X27_X3E_X3C_X2Fdiv_X3E),
  /* K139 */  be_nested_str(_X3Cbutton_X20type_X3D_X27submit_X27_X20class_X3D_X27btn_X2Daction_X27_X20name_X3D_X27u_X25s_X27_X20onclick_X3D_X27return_X20confirm_X28_X22Confirm_X20upgrade_X20of_X20_X25s_X22_X29_X27_X3EUpgrade_X3C_X2Fbutton_X3E),
  /* K140 */  be_nested_str(_X3Cbutton_X20type_X3D_X27submit_X27_X20class_X3D_X27btn_X2Daction_X27_X20style_X3D_X27visibility_X3Ahidden_X3B_X27_X3E_X3C_X2Fbutton_X3E),
  /* K141 */  be_nested_str(_X3Cbutton_X20type_X3D_X27submit_X27_X20class_X3D_X27btn_X2Daction_X20bred_X27_X20name_X3D_X27d_X25s_X27_X20onclick_X3D_X27return_X20confirm_X28_X22Confirm_X20deletion_X20of_X20_X25s_X22_X29_X27_X3EUninstall_X3C_X2Fbutton_X3E),
  /* K142 */  be_nested_str(_X3Cbutton_X20type_X3D_X27submit_X27_X20class_X3D_X27btn_X2Daction_X27_X20style_X3D_X27visibility_X3Ahidden_X3B_X27_X3E_X3C_X2Fbutton_X3E_X3Cbutton_X20type_X3D_X27submit_X27_X20class_X3D_X27btn_X2Daction_X20bgrn_X27_X20name_X3D_X27i_X25s_X27_X20onclick_X3D_X27return_X20confirm_X28_X22Confirm_X20installation_X20of_X20_X25s_X22_X29_X27_X3EInstall_X3C_X2Fbutton_X3E),
  /* K143 */  be_nested_str(v_X25s_X2E_X25s_X2E_X25s_X2E_X25s),
  /* K144 */  be_nested_str(isdir),
  /* K145 */  be_nested_str(remove),
  /* K146 */  be_nested_str(mkdir),
  /* K147 */  be_nested_str(cannot_X20create_X20folder_X20_X27_X25s_X27),
  /* K148 */  be_nested_str(arg_name),
  /* K149 */  be_const_int(2147483647),
  /* K150 */  be_nested_str(unload_extension),
  /* K151 */  be_nested_str(tapp_),
  /* K152 */  be_nested_str(tapp),
  /* K153 */  be_nested_str(rename),
  /* K154 */  be_nested_str(EXT_X3A_X20wrong_X20action_X20_X27_X25s_X27),
  /* K155 */  be_nested_str(d),
  /* K156 */  be_nested_str(u),
  /* K157 */  be_nested_str(install_from_store),
  /* K158 */  be_nested_str(i),
  /* K159 */  be_nested_str(redirect),
  /* K160 */  be_nested_str(CFG_X3A_X20Exception_X3E_X20_X27_X25s_X27_X20_X2D_X20_X25s),
  /* K161 */  be_nested_str(Parameter_X20error),
  /* K162 */  be_nested_str(_X3Cp_X20style_X3D_X27width_X3A340px_X3B_X27_X3E_X3Cb_X3EException_X3A_X3C_X2Fb_X3E_X3Cbr_X3E_X27_X25s_X27_X3Cbr_X3E_X25s_X3C_X2Fp_X3E),
  /* K163 */  be_nested_str(BUTTON_CONFIGURATION),
};


extern const bclass be_class_Extension_manager;

/********************************************************************
** Solidified function: page_extensions_mgr_dispatcher
********************************************************************/
be_local_closure(class_Extension_manager_page_extensions_mgr_dispatcher,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Extension_manager,     /* shared constants */
    &be_const_str_page_extensions_mgr_dispatcher,
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x740A0001,  //  0003  JMPT	R2	#0006
      0x4C080000,  //  0004  LDNIL	R2
      0x80040400,  //  0005  RET	1	R2
      0x8C080302,  //  0006  GETMET	R2	R1	K2
      0x58100003,  //  0007  LDCONST	R4	K3
      0x7C080400,  //  0008  CALL	R2	2
      0x780A0003,  //  0009  JMPF	R2	#000E
      0x8C080104,  //  000A  GETMET	R2	R0	K4
      0x7C080200,  //  000B  CALL	R2	1
      0x80040400,  //  000C  RET	1	R2
      0x70020002,  //  000D  JMP		#0011
      0x8C080105,  //  000E  GETMET	R2	R0	K5
      0x7C080200,  //  000F  CALL	R2	1
      0x80040400,  //  0010  RET	1	R2
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: load_manifest
********************************************************************/
be_local_closure(class_Extension_manager_load_manifest,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Extension_manager,     /* shared constants */
    &be_const_str_load_manifest,
    &be_const_str_solidified,
    ( &(const binstruction[70]) {  /* code */
      0xA8020035,  //  0000  EXBLK	0	#0037
      0xB8060C00,  //  0001  GETNGBL	R1	K6
      0x8C040307,  //  0002  GETMET	R1	R1	K7
      0x7C040200,  //  0003  CALL	R1	1
      0x60080018,  //  0004  GETGBL	R2	G24
      0x580C0008,  //  0005  LDCONST	R3	K8
      0xB8120C00,  //  0006  GETNGBL	R4	K6
      0x8C100909,  //  0007  GETMET	R4	R4	K9
      0x7C100200,  //  0008  CALL	R4	1
      0x7C080400,  //  0009  CALL	R2	2
      0x600C0018,  //  000A  GETGBL	R3	G24
      0x5810000A,  //  000B  LDCONST	R4	K10
      0x8814010B,  //  000C  GETMBR	R5	R0	K11
      0x8818010C,  //  000D  GETMBR	R6	R0	K12
      0x5C1C0200,  //  000E  MOVE	R7	R1
      0x5C200400,  //  000F  MOVE	R8	R2
      0x7C0C0A00,  //  0010  CALL	R3	5
      0xB8121A00,  //  0011  GETNGBL	R4	K13
      0x60140018,  //  0012  GETGBL	R5	G24
      0x5818000E,  //  0013  LDCONST	R6	K14
      0x5C1C0600,  //  0014  MOVE	R7	R3
      0x7C140400,  //  0015  CALL	R5	2
      0x5818000F,  //  0016  LDCONST	R6	K15
      0x7C100400,  //  0017  CALL	R4	2
      0xB8122000,  //  0018  GETNGBL	R4	K16
      0x7C100000,  //  0019  CALL	R4	0
      0x8C140911,  //  001A  GETMET	R5	R4	K17
      0x5C1C0600,  //  001B  MOVE	R7	R3
      0x7C140400,  //  001C  CALL	R5	2
      0x8C140912,  //  001D  GETMET	R5	R4	K18
      0x7C140200,  //  001E  CALL	R5	1
      0x541A00C7,  //  001F  LDINT	R6	200
      0x20180A06,  //  0020  NE	R6	R5	R6
      0x781A000C,  //  0021  JMPF	R6	#002F
      0xB81A0C00,  //  0022  GETNGBL	R6	K6
      0x8C180D0D,  //  0023  GETMET	R6	R6	K13
      0x60200018,  //  0024  GETGBL	R8	G24
      0x58240013,  //  0025  LDCONST	R9	K19
      0x5C280A00,  //  0026  MOVE	R10	R5
      0x7C200400,  //  0027  CALL	R8	2
      0x58240014,  //  0028  LDCONST	R9	K20
      0x7C180600,  //  0029  CALL	R6	3
      0x60180018,  //  002A  GETGBL	R6	G24
      0x581C0015,  //  002B  LDCONST	R7	K21
      0x5C200A00,  //  002C  MOVE	R8	R5
      0x7C180400,  //  002D  CALL	R6	2
      0xB0062C06,  //  002E  RAISE	1	K22	R6
      0x8C180917,  //  002F  GETMET	R6	R4	K23
      0x7C180200,  //  0030  CALL	R6	1
      0x8C1C0918,  //  0031  GETMET	R7	R4	K24
      0x7C1C0200,  //  0032  CALL	R7	1
      0xA8040001,  //  0033  EXBLK	1	1
      0x80040C00,  //  0034  RET	1	R6
      0xA8040001,  //  0035  EXBLK	1	1
      0x7002000D,  //  0036  JMP		#0045
      0xAC040002,  //  0037  CATCH	R1	0	2
      0x7002000A,  //  0038  JMP		#0044
      0xB80E0C00,  //  0039  GETNGBL	R3	K6
      0x8C0C070D,  //  003A  GETMET	R3	R3	K13
      0x60140018,  //  003B  GETGBL	R5	G24
      0x58180019,  //  003C  LDCONST	R6	K25
      0x5C1C0200,  //  003D  MOVE	R7	R1
      0x5C200400,  //  003E  MOVE	R8	R2
      0x7C140600,  //  003F  CALL	R5	3
      0x58180014,  //  0040  LDCONST	R6	K20
      0x7C0C0600,  //  0041  CALL	R3	3
      0xB0040202,  //  0042  RAISE	1	R1	R2
      0x70020000,  //  0043  JMP		#0045
      0xB0080000,  //  0044  RAISE	2	R0	R0
      0x80000000,  //  0045  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_add_handler
********************************************************************/
be_local_closure(class_Extension_manager_web_add_handler,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        2,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str(page_extensions_mgr_dispatcher),
        }),
        &be_const_str__X3Clambda_X3E,
        &be_const_str_solidified,
        ( &(const binstruction[ 4]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0x80040000,  //  0003  RET	1	R0
        })
      ),
      be_nested_proto(
        2,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str(page_extensions_ctl),
        }),
        &be_const_str__X3Clambda_X3E,
        &be_const_str_solidified,
        ( &(const binstruction[ 4]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0x80040000,  //  0003  RET	1	R0
        })
      ),
    }),
    1,                          /* has constants */
    &be_ktab_class_Extension_manager,     /* shared constants */
    &be_const_str_web_add_handler,
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C08031A,  //  0001  GETMET	R2	R1	K26
      0x5810001B,  //  0002  LDCONST	R4	K27
      0x84140000,  //  0003  CLOSURE	R5	P0
      0x8818031C,  //  0004  GETMBR	R6	R1	K28
      0x7C080800,  //  0005  CALL	R2	4
      0x8C08031A,  //  0006  GETMET	R2	R1	K26
      0x5810001B,  //  0007  LDCONST	R4	K27
      0x84140001,  //  0008  CLOSURE	R5	P1
      0x8818031D,  //  0009  GETMBR	R6	R1	K29
      0x7C080800,  //  000A  CALL	R2	4
      0xA0000000,  //  000B  CLOSE	R0
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: page_extensions_mgr
********************************************************************/
be_local_closure(class_Extension_manager_page_extensions_mgr,   /* name */
  be_nested_proto(
    21,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Extension_manager,     /* shared constants */
    &be_const_str_page_extensions_mgr,
    &be_const_str_solidified,
    ( &(const binstruction[161]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A3C00,  //  0001  IMPORT	R2	K30
      0x8C0C031F,  //  0002  GETMET	R3	R1	K31
      0x58140020,  //  0003  LDCONST	R5	K32
      0x7C0C0400,  //  0004  CALL	R3	2
      0x8C0C0321,  //  0005  GETMET	R3	R1	K33
      0x7C0C0200,  //  0006  CALL	R3	1
      0x8C0C0322,  //  0007  GETMET	R3	R1	K34
      0x58140023,  //  0008  LDCONST	R5	K35
      0x7C0C0400,  //  0009  CALL	R3	2
      0x8C0C0322,  //  000A  GETMET	R3	R1	K34
      0x58140024,  //  000B  LDCONST	R5	K36
      0x7C0C0400,  //  000C  CALL	R3	2
      0x8C0C0322,  //  000D  GETMET	R3	R1	K34
      0x58140025,  //  000E  LDCONST	R5	K37
      0x7C0C0400,  //  000F  CALL	R3	2
      0x8C0C0126,  //  0010  GETMET	R3	R0	K38
      0x50140200,  //  0011  LDBOOL	R5	1	0
      0x7C0C0400,  //  0012  CALL	R3	2
      0x6010000C,  //  0013  GETGBL	R4	G12
      0x5C140600,  //  0014  MOVE	R5	R3
      0x7C100200,  //  0015  CALL	R4	1
      0x24100927,  //  0016  GT	R4	R4	K39
      0x78120076,  //  0017  JMPF	R4	#008F
      0x58100027,  //  0018  LDCONST	R4	K39
      0x6014000C,  //  0019  GETGBL	R5	G12
      0x5C180600,  //  001A  MOVE	R6	R3
      0x7C140200,  //  001B  CALL	R5	1
      0x14140805,  //  001C  LT	R5	R4	R5
      0x7816006F,  //  001D  JMPF	R5	#008E
      0x24140927,  //  001E  GT	R5	R4	K39
      0x78160002,  //  001F  JMPF	R5	#0023
      0x8C140322,  //  0020  GETMET	R5	R1	K34
      0x581C0028,  //  0021  LDCONST	R7	K40
      0x7C140400,  //  0022  CALL	R5	2
      0x8C140729,  //  0023  GETMET	R5	R3	K41
      0x5C1C0800,  //  0024  MOVE	R7	R4
      0x7C140400,  //  0025  CALL	R5	2
      0x8C18032A,  //  0026  GETMET	R6	R1	K42
      0x5C200A00,  //  0027  MOVE	R8	R5
      0x7C180400,  //  0028  CALL	R6	2
      0xB81E0C00,  //  0029  GETNGBL	R7	K6
      0x8C1C0F2B,  //  002A  GETMET	R7	R7	K43
      0x5C240A00,  //  002B  MOVE	R9	R5
      0x7C1C0400,  //  002C  CALL	R7	2
      0xB8220C00,  //  002D  GETNGBL	R8	K6
      0x8820112C,  //  002E  GETMBR	R8	R8	K44
      0x78220005,  //  002F  JMPF	R8	#0036
      0xB8220C00,  //  0030  GETNGBL	R8	K6
      0x8820112C,  //  0031  GETMBR	R8	R8	K44
      0x8C20112D,  //  0032  GETMET	R8	R8	K45
      0x5C280A00,  //  0033  MOVE	R10	R5
      0x7C200400,  //  0034  CALL	R8	2
      0x70020000,  //  0035  JMP		#0037
      0x50200000,  //  0036  LDBOOL	R8	0	0
      0x78220001,  //  0037  JMPF	R8	#003A
      0x5824002E,  //  0038  LDCONST	R9	K46
      0x70020000,  //  0039  JMP		#003B
      0x5824002F,  //  003A  LDCONST	R9	K47
      0x8C280F30,  //  003B  GETMET	R10	R7	K48
      0x58300031,  //  003C  LDCONST	R12	K49
      0x50340000,  //  003D  LDBOOL	R13	0	0
      0x7C280600,  //  003E  CALL	R10	3
      0x582C0032,  //  003F  LDCONST	R11	K50
      0x58300033,  //  0040  LDCONST	R12	K51
      0x8C340322,  //  0041  GETMET	R13	R1	K34
      0x583C0034,  //  0042  LDCONST	R15	K52
      0x7C340400,  //  0043  CALL	R13	2
      0x8C340322,  //  0044  GETMET	R13	R1	K34
      0x603C0018,  //  0045  GETGBL	R15	G24
      0x58400035,  //  0046  LDCONST	R16	K53
      0x5C440C00,  //  0047  MOVE	R17	R6
      0x8C48032A,  //  0048  GETMET	R18	R1	K42
      0x94500F36,  //  0049  GETIDX	R20	R7	K54
      0x7C480400,  //  004A  CALL	R18	2
      0x5C4C1200,  //  004B  MOVE	R19	R9
      0x7C3C0800,  //  004C  CALL	R15	4
      0x7C340400,  //  004D  CALL	R13	2
      0x8C340322,  //  004E  GETMET	R13	R1	K34
      0x603C0018,  //  004F  GETGBL	R15	G24
      0x58400037,  //  0050  LDCONST	R16	K55
      0x8C44032A,  //  0051  GETMET	R17	R1	K42
      0x944C0F38,  //  0052  GETIDX	R19	R7	K56
      0x7C440400,  //  0053  CALL	R17	2
      0x7C3C0400,  //  0054  CALL	R15	2
      0x7C340400,  //  0055  CALL	R13	2
      0x8C340322,  //  0056  GETMET	R13	R1	K34
      0x583C0039,  //  0057  LDCONST	R15	K57
      0x7C340400,  //  0058  CALL	R13	2
      0x8C340322,  //  0059  GETMET	R13	R1	K34
      0x583C003A,  //  005A  LDCONST	R15	K58
      0x7C340400,  //  005B  CALL	R13	2
      0x8C340322,  //  005C  GETMET	R13	R1	K34
      0x603C0018,  //  005D  GETGBL	R15	G24
      0x5840003B,  //  005E  LDCONST	R16	K59
      0x78220001,  //  005F  JMPF	R8	#0062
      0x5C441600,  //  0060  MOVE	R17	R11
      0x70020000,  //  0061  JMP		#0063
      0x5C441800,  //  0062  MOVE	R17	R12
      0x78220001,  //  0063  JMPF	R8	#0066
      0x5848003C,  //  0064  LDCONST	R18	K60
      0x70020000,  //  0065  JMP		#0067
      0x5848003D,  //  0066  LDCONST	R18	K61
      0x5C4C0C00,  //  0067  MOVE	R19	R6
      0x78220001,  //  0068  JMPF	R8	#006B
      0x5850003E,  //  0069  LDCONST	R20	K62
      0x70020000,  //  006A  JMP		#006C
      0x5850003F,  //  006B  LDCONST	R20	K63
      0x7C3C0A00,  //  006C  CALL	R15	5
      0x7C340400,  //  006D  CALL	R13	2
      0x8C340322,  //  006E  GETMET	R13	R1	K34
      0x603C0018,  //  006F  GETGBL	R15	G24
      0x58400040,  //  0070  LDCONST	R16	K64
      0x782A0001,  //  0071  JMPF	R10	#0074
      0x5844002F,  //  0072  LDCONST	R17	K47
      0x70020000,  //  0073  JMP		#0075
      0x5C441800,  //  0074  MOVE	R17	R12
      0x782A0001,  //  0075  JMPF	R10	#0078
      0x58480041,  //  0076  LDCONST	R18	K65
      0x70020000,  //  0077  JMP		#0079
      0x58480042,  //  0078  LDCONST	R18	K66
      0x5C4C0C00,  //  0079  MOVE	R19	R6
      0x782A0001,  //  007A  JMPF	R10	#007D
      0x58500043,  //  007B  LDCONST	R20	K67
      0x70020000,  //  007C  JMP		#007E
      0x58500044,  //  007D  LDCONST	R20	K68
      0x7C3C0A00,  //  007E  CALL	R15	5
      0x7C340400,  //  007F  CALL	R13	2
      0x8C340322,  //  0080  GETMET	R13	R1	K34
      0x603C0018,  //  0081  GETGBL	R15	G24
      0x58400045,  //  0082  LDCONST	R16	K69
      0x5C440C00,  //  0083  MOVE	R17	R6
      0x8C48032A,  //  0084  GETMET	R18	R1	K42
      0x5C500A00,  //  0085  MOVE	R20	R5
      0x7C480400,  //  0086  CALL	R18	2
      0x7C3C0600,  //  0087  CALL	R15	3
      0x7C340400,  //  0088  CALL	R13	2
      0x8C340322,  //  0089  GETMET	R13	R1	K34
      0x583C0046,  //  008A  LDCONST	R15	K70
      0x7C340400,  //  008B  CALL	R13	2
      0x00100947,  //  008C  ADD	R4	R4	K71
      0x7001FF8A,  //  008D  JMP		#0019
      0x70020002,  //  008E  JMP		#0092
      0x8C100322,  //  008F  GETMET	R4	R1	K34
      0x58180048,  //  0090  LDCONST	R6	K72
      0x7C100400,  //  0091  CALL	R4	2
      0x8C100322,  //  0092  GETMET	R4	R1	K34
      0x58180049,  //  0093  LDCONST	R6	K73
      0x7C100400,  //  0094  CALL	R4	2
      0x8C100322,  //  0095  GETMET	R4	R1	K34
      0x5818004A,  //  0096  LDCONST	R6	K74
      0x7C100400,  //  0097  CALL	R4	2
      0x8C100322,  //  0098  GETMET	R4	R1	K34
      0x5818004B,  //  0099  LDCONST	R6	K75
      0x7C100400,  //  009A  CALL	R4	2
      0x8C10034C,  //  009B  GETMET	R4	R1	K76
      0x8818034D,  //  009C  GETMBR	R6	R1	K77
      0x7C100400,  //  009D  CALL	R4	2
      0x8C10034E,  //  009E  GETMET	R4	R1	K78
      0x7C100200,  //  009F  CALL	R4	1
      0x80000000,  //  00A0  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_add_button
********************************************************************/
be_local_closure(class_Extension_manager_web_add_button,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Extension_manager,     /* shared constants */
    &be_const_str_web_add_button,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080322,  //  0001  GETMET	R2	R1	K34
      0x5810004F,  //  0002  LDCONST	R4	K79
      0x7C080400,  //  0003  CALL	R2	2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: install_from_store
********************************************************************/
be_local_closure(class_Extension_manager_install_from_store,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Extension_manager,     /* shared constants */
    &be_const_str_install_from_store,
    &be_const_str_solidified,
    ( &(const binstruction[97]) {  /* code */
      0xA40A3C00,  //  0000  IMPORT	R2	K30
      0xA40EA000,  //  0001  IMPORT	R3	K80
      0x8C100151,  //  0002  GETMET	R4	R0	K81
      0x5C180200,  //  0003  MOVE	R6	R1
      0x7C100400,  //  0004  CALL	R4	2
      0x5C040800,  //  0005  MOVE	R1	R4
      0x8C100552,  //  0006  GETMET	R4	R2	K82
      0x5C180200,  //  0007  MOVE	R6	R1
      0x581C0053,  //  0008  LDCONST	R7	K83
      0x7C100600,  //  0009  CALL	R4	3
      0x74120000,  //  000A  JMPT	R4	#000C
      0x00040353,  //  000B  ADD	R1	R1	K83
      0x60100018,  //  000C  GETGBL	R4	G24
      0x58140054,  //  000D  LDCONST	R5	K84
      0x8818010B,  //  000E  GETMBR	R6	R0	K11
      0x881C0155,  //  000F  GETMBR	R7	R0	K85
      0x5C200200,  //  0010  MOVE	R8	R1
      0x7C100800,  //  0011  CALL	R4	4
      0xB8161A00,  //  0012  GETNGBL	R5	K13
      0x60180018,  //  0013  GETGBL	R6	G24
      0x581C0056,  //  0014  LDCONST	R7	K86
      0x5C200800,  //  0015  MOVE	R8	R4
      0x7C180400,  //  0016  CALL	R6	2
      0x581C000F,  //  0017  LDCONST	R7	K15
      0x7C140400,  //  0018  CALL	R5	2
      0xA8020036,  //  0019  EXBLK	0	#0051
      0x8C140157,  //  001A  GETMET	R5	R0	K87
      0x881C0158,  //  001B  GETMBR	R7	R0	K88
      0x7C140400,  //  001C  CALL	R5	2
      0x60140018,  //  001D  GETGBL	R5	G24
      0x58180059,  //  001E  LDCONST	R6	K89
      0x881C0158,  //  001F  GETMBR	R7	R0	K88
      0x5C200200,  //  0020  MOVE	R8	R1
      0x7C140600,  //  0021  CALL	R5	3
      0xB81A2000,  //  0022  GETNGBL	R6	K16
      0x7C180000,  //  0023  CALL	R6	0
      0x8C1C0D11,  //  0024  GETMET	R7	R6	K17
      0x5C240800,  //  0025  MOVE	R9	R4
      0x7C1C0400,  //  0026  CALL	R7	2
      0x8C1C0D12,  //  0027  GETMET	R7	R6	K18
      0x7C1C0200,  //  0028  CALL	R7	1
      0x542200C7,  //  0029  LDINT	R8	200
      0x20200E08,  //  002A  NE	R8	R7	R8
      0x78220008,  //  002B  JMPF	R8	#0035
      0xB8221A00,  //  002C  GETNGBL	R8	K13
      0x60240018,  //  002D  GETGBL	R9	G24
      0x5828005A,  //  002E  LDCONST	R10	K90
      0x5C2C0E00,  //  002F  MOVE	R11	R7
      0x7C240400,  //  0030  CALL	R9	2
      0x58280014,  //  0031  LDCONST	R10	K20
      0x7C200400,  //  0032  CALL	R8	2
      0xA8040001,  //  0033  EXBLK	1	1
      0x80001000,  //  0034  RET	0
      0x8C200D5B,  //  0035  GETMET	R8	R6	K91
      0x5C280A00,  //  0036  MOVE	R10	R5
      0x7C200400,  //  0037  CALL	R8	2
      0x8C240D18,  //  0038  GETMET	R9	R6	K24
      0x7C240200,  //  0039  CALL	R9	1
      0x24241127,  //  003A  GT	R9	R8	K39
      0x7826000C,  //  003B  JMPF	R9	#0049
      0x8C24075C,  //  003C  GETMET	R9	R3	K92
      0x5C2C0A00,  //  003D  MOVE	R11	R5
      0x7C240400,  //  003E  CALL	R9	2
      0x78260008,  //  003F  JMPF	R9	#0049
      0xB8261A00,  //  0040  GETNGBL	R9	K13
      0x60280018,  //  0041  GETGBL	R10	G24
      0x582C005D,  //  0042  LDCONST	R11	K93
      0x5C300A00,  //  0043  MOVE	R12	R5
      0x5C341000,  //  0044  MOVE	R13	R8
      0x7C280600,  //  0045  CALL	R10	3
      0x582C000F,  //  0046  LDCONST	R11	K15
      0x7C240400,  //  0047  CALL	R9	2
      0x70020005,  //  0048  JMP		#004F
      0x60240018,  //  0049  GETGBL	R9	G24
      0x5828005E,  //  004A  LDCONST	R10	K94
      0x5C2C0A00,  //  004B  MOVE	R11	R5
      0x5C301000,  //  004C  MOVE	R12	R8
      0x7C240600,  //  004D  CALL	R9	3
      0xB006BE09,  //  004E  RAISE	1	K95	R9
      0xA8040001,  //  004F  EXBLK	1	1
      0x7002000E,  //  0050  JMP		#0060
      0xAC140002,  //  0051  CATCH	R5	0	2
      0x7002000B,  //  0052  JMP		#005F
      0xB81E0C00,  //  0053  GETNGBL	R7	K6
      0x8C1C0F0D,  //  0054  GETMET	R7	R7	K13
      0x60240018,  //  0055  GETGBL	R9	G24
      0x58280019,  //  0056  LDCONST	R10	K25
      0x5C2C0A00,  //  0057  MOVE	R11	R5
      0x5C300C00,  //  0058  MOVE	R12	R6
      0x7C240600,  //  0059  CALL	R9	3
      0x58280014,  //  005A  LDCONST	R10	K20
      0x7C1C0600,  //  005B  CALL	R7	3
      0x4C1C0000,  //  005C  LDNIL	R7
      0x80040E00,  //  005D  RET	1	R7
      0x70020000,  //  005E  JMP		#0060
      0xB0080000,  //  005F  RAISE	2	R0	R0
      0x80000000,  //  0060  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: list_extensions
********************************************************************/
be_local_closure(class_Extension_manager_list_extensions,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    0,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Extension_manager,     /* shared constants */
    &be_const_str_list_extensions,
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0x58000060,  //  0000  LDCONST	R0	K96
      0xA406A000,  //  0001  IMPORT	R1	K80
      0xA40A3C00,  //  0002  IMPORT	R2	K30
      0x600C0012,  //  0003  GETGBL	R3	G18
      0x7C0C0000,  //  0004  CALL	R3	0
      0x60100010,  //  0005  GETGBL	R4	G16
      0x8C140361,  //  0006  GETMET	R5	R1	K97
      0x881C0158,  //  0007  GETMBR	R7	R0	K88
      0x7C140400,  //  0008  CALL	R5	2
      0x7C100200,  //  0009  CALL	R4	1
      0xA8020010,  //  000A  EXBLK	0	#001C
      0x5C140800,  //  000B  MOVE	R5	R4
      0x7C140000,  //  000C  CALL	R5	0
      0x8C180552,  //  000D  GETMET	R6	R2	K82
      0x5C200A00,  //  000E  MOVE	R8	R5
      0x58240053,  //  000F  LDCONST	R9	K83
      0x7C180600,  //  0010  CALL	R6	3
      0x741A0004,  //  0011  JMPT	R6	#0017
      0x8C180552,  //  0012  GETMET	R6	R2	K82
      0x5C200A00,  //  0013  MOVE	R8	R5
      0x58240062,  //  0014  LDCONST	R9	K98
      0x7C180600,  //  0015  CALL	R6	3
      0x781A0003,  //  0016  JMPF	R6	#001B
      0x8C180763,  //  0017  GETMET	R6	R3	K99
      0x88200158,  //  0018  GETMBR	R8	R0	K88
      0x00201005,  //  0019  ADD	R8	R8	R5
      0x7C180400,  //  001A  CALL	R6	2
      0x7001FFEE,  //  001B  JMP		#000B
      0x58100064,  //  001C  LDCONST	R4	K100
      0xAC100200,  //  001D  CATCH	R4	1	0
      0xB0080000,  //  001E  RAISE	2	R0	R0
      0x80040600,  //  001F  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Extension_manager_init,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Extension_manager,     /* shared constants */
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0xB8060C00,  //  0000  GETNGBL	R1	K6
      0x8C040365,  //  0001  GETMET	R1	R1	K101
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: list_extensions_in_fs
********************************************************************/
be_local_closure(class_Extension_manager_list_extensions_in_fs,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    0,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Extension_manager,     /* shared constants */
    &be_const_str_list_extensions_in_fs,
    &be_const_str_solidified,
    ( &(const binstruction[36]) {  /* code */
      0x58000060,  //  0000  LDCONST	R0	K96
      0xA4063C00,  //  0001  IMPORT	R1	K30
      0xB80ACC00,  //  0002  GETNGBL	R2	K102
      0x7C080000,  //  0003  CALL	R2	0
      0x600C0010,  //  0004  GETGBL	R3	G16
      0x8C100167,  //  0005  GETMET	R4	R0	K103
      0x7C100200,  //  0006  CALL	R4	1
      0x7C0C0200,  //  0007  CALL	R3	1
      0xA8020016,  //  0008  EXBLK	0	#0020
      0x5C100600,  //  0009  MOVE	R4	R3
      0x7C100000,  //  000A  CALL	R4	0
      0xB8160C00,  //  000B  GETNGBL	R5	K6
      0x8C140B2B,  //  000C  GETMET	R5	R5	K43
      0x5C1C0800,  //  000D  MOVE	R7	R4
      0x7C140400,  //  000E  CALL	R5	2
      0x4C180000,  //  000F  LDNIL	R6
      0x20180A06,  //  0010  NE	R6	R5	R6
      0x781A0005,  //  0011  JMPF	R6	#0018
      0x8C180B30,  //  0012  GETMET	R6	R5	K48
      0x58200036,  //  0013  LDCONST	R8	K54
      0x7C180400,  //  0014  CALL	R6	2
      0x781A0000,  //  0015  JMPF	R6	#0017
      0x98080C04,  //  0016  SETIDX	R2	R6	R4
      0x70020006,  //  0017  JMP		#001F
      0xB81A1A00,  //  0018  GETNGBL	R6	K13
      0x601C0018,  //  0019  GETGBL	R7	G24
      0x58200068,  //  001A  LDCONST	R8	K104
      0x5C240800,  //  001B  MOVE	R9	R4
      0x7C1C0400,  //  001C  CALL	R7	2
      0x5820000F,  //  001D  LDCONST	R8	K15
      0x7C180400,  //  001E  CALL	R6	2
      0x7001FFE8,  //  001F  JMP		#0009
      0x580C0064,  //  0020  LDCONST	R3	K100
      0xAC0C0200,  //  0021  CATCH	R3	1	0
      0xB0080000,  //  0022  RAISE	2	R0	R0
      0x80040400,  //  0023  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: manifest_decode
********************************************************************/
be_local_closure(class_Extension_manager_manifest_decode,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Extension_manager,     /* shared constants */
    &be_const_str_manifest_decode,
    &be_const_str_solidified,
    ( &(const binstruction[58]) {  /* code */
      0x58040060,  //  0000  LDCONST	R1	K96
      0xA40AD200,  //  0001  IMPORT	R2	K105
      0x8C0C056A,  //  0002  GETMET	R3	R2	K106
      0x5C140000,  //  0003  MOVE	R5	R0
      0x7C0C0400,  //  0004  CALL	R3	2
      0x4C100000,  //  0005  LDNIL	R4
      0x1C100604,  //  0006  EQ	R4	R3	R4
      0x78120008,  //  0007  JMPF	R4	#0011
      0xB8121A00,  //  0008  GETNGBL	R4	K13
      0x60140018,  //  0009  GETGBL	R5	G24
      0x5818006B,  //  000A  LDCONST	R6	K107
      0x5C1C0000,  //  000B  MOVE	R7	R0
      0x7C140400,  //  000C  CALL	R5	2
      0x5818000F,  //  000D  LDCONST	R6	K15
      0x7C100400,  //  000E  CALL	R4	2
      0x4C100000,  //  000F  LDNIL	R4
      0x80040800,  //  0010  RET	1	R4
      0x8C10072D,  //  0011  GETMET	R4	R3	K45
      0x58180036,  //  0012  LDCONST	R6	K54
      0x7C100400,  //  0013  CALL	R4	2
      0x78120007,  //  0014  JMPF	R4	#001D
      0x8C10072D,  //  0015  GETMET	R4	R3	K45
      0x5818006C,  //  0016  LDCONST	R6	K108
      0x7C100400,  //  0017  CALL	R4	2
      0x78120003,  //  0018  JMPF	R4	#001D
      0x8C10072D,  //  0019  GETMET	R4	R3	K45
      0x58180009,  //  001A  LDCONST	R6	K9
      0x7C100400,  //  001B  CALL	R4	2
      0x74120007,  //  001C  JMPT	R4	#0025
      0xB8121A00,  //  001D  GETNGBL	R4	K13
      0x60140018,  //  001E  GETGBL	R5	G24
      0x5818006D,  //  001F  LDCONST	R6	K109
      0x5C1C0600,  //  0020  MOVE	R7	R3
      0x7C140400,  //  0021  CALL	R5	2
      0x7C100200,  //  0022  CALL	R4	1
      0x4C100000,  //  0023  LDNIL	R4
      0x80040800,  //  0024  RET	1	R4
      0x60100013,  //  0025  GETGBL	R4	G19
      0x7C100000,  //  0026  CALL	R4	0
      0x94140736,  //  0027  GETIDX	R5	R3	K54
      0x98126C05,  //  0028  SETIDX	R4	K54	R5
      0x9414076C,  //  0029  GETIDX	R5	R3	K108
      0x9812D805,  //  002A  SETIDX	R4	K108	R5
      0x60140009,  //  002B  GETGBL	R5	G9
      0x94180709,  //  002C  GETIDX	R6	R3	K9
      0x7C140200,  //  002D  CALL	R5	1
      0x98121205,  //  002E  SETIDX	R4	K9	R5
      0x8C140730,  //  002F  GETMET	R5	R3	K48
      0x581C0038,  //  0030  LDCONST	R7	K56
      0x5820006E,  //  0031  LDCONST	R8	K110
      0x7C140600,  //  0032  CALL	R5	3
      0x98127005,  //  0033  SETIDX	R4	K56	R5
      0x8C140730,  //  0034  GETMET	R5	R3	K48
      0x581C006F,  //  0035  LDCONST	R7	K111
      0x5820002F,  //  0036  LDCONST	R8	K47
      0x7C140600,  //  0037  CALL	R5	3
      0x9812DE05,  //  0038  SETIDX	R4	K111	R5
      0x80040800,  //  0039  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tapp_name
********************************************************************/
be_local_closure(class_Extension_manager_tapp_name,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Extension_manager,     /* shared constants */
    &be_const_str_tapp_name,
    &be_const_str_solidified,
    ( &(const binstruction[44]) {  /* code */
      0x58040060,  //  0000  LDCONST	R1	K96
      0xA40A3C00,  //  0001  IMPORT	R2	K30
      0x580C0027,  //  0002  LDCONST	R3	K39
      0x6010000C,  //  0003  GETGBL	R4	G12
      0x5C140000,  //  0004  MOVE	R5	R0
      0x7C100200,  //  0005  CALL	R4	1
      0x04100947,  //  0006  SUB	R4	R4	K71
      0x94140004,  //  0007  GETIDX	R5	R0	R4
      0x1C140B70,  //  0008  EQ	R5	R5	K112
      0x74160002,  //  0009  JMPT	R5	#000D
      0x94140004,  //  000A  GETIDX	R5	R0	R4
      0x1C140B71,  //  000B  EQ	R5	R5	K113
      0x78160000,  //  000C  JMPF	R5	#000E
      0x04100947,  //  000D  SUB	R4	R4	K71
      0x8C140530,  //  000E  GETMET	R5	R2	K48
      0x5C1C0000,  //  000F  MOVE	R7	R0
      0x58200053,  //  0010  LDCONST	R8	K83
      0x00240947,  //  0011  ADD	R9	R4	K71
      0x6028000C,  //  0012  GETGBL	R10	G12
      0x582C0053,  //  0013  LDCONST	R11	K83
      0x7C280200,  //  0014  CALL	R10	1
      0x0424120A,  //  0015  SUB	R9	R9	R10
      0x7C140800,  //  0016  CALL	R5	4
      0x28140B27,  //  0017  GE	R5	R5	K39
      0x78160003,  //  0018  JMPF	R5	#001D
      0x6014000C,  //  0019  GETGBL	R5	G12
      0x58180053,  //  001A  LDCONST	R6	K83
      0x7C140200,  //  001B  CALL	R5	1
      0x04100805,  //  001C  SUB	R4	R4	R5
      0x4C140000,  //  001D  LDNIL	R5
      0x8C180530,  //  001E  GETMET	R6	R2	K48
      0x5C200000,  //  001F  MOVE	R8	R0
      0x58240072,  //  0020  LDCONST	R9	K114
      0x5C280600,  //  0021  MOVE	R10	R3
      0x7C180800,  //  0022  CALL	R6	4
      0x5C140C00,  //  0023  MOVE	R5	R6
      0x28180D27,  //  0024  GE	R6	R6	K39
      0x781A0002,  //  0025  JMPF	R6	#0029
      0x00180B47,  //  0026  ADD	R6	R5	K71
      0x5C0C0C00,  //  0027  MOVE	R3	R6
      0x7001FFF4,  //  0028  JMP		#001E
      0x40180604,  //  0029  CONNECT	R6	R3	R4
      0x94180006,  //  002A  GETIDX	R6	R0	R6
      0x80040C00,  //  002B  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: page_extensions_store
********************************************************************/
be_local_closure(class_Extension_manager_page_extensions_store,   /* name */
  be_nested_proto(
    33,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Extension_manager,     /* shared constants */
    &be_const_str_page_extensions_store,
    &be_const_str_solidified,
    ( &(const binstruction[210]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A3C00,  //  0001  IMPORT	R2	K30
      0xA40ED200,  //  0002  IMPORT	R3	K105
      0x8C100373,  //  0003  GETMET	R4	R1	K115
      0x541A00C7,  //  0004  LDINT	R6	200
      0x581C0074,  //  0005  LDCONST	R7	K116
      0x7C100600,  //  0006  CALL	R4	3
      0x4C100000,  //  0007  LDNIL	R4
      0xA8020004,  //  0008  EXBLK	0	#000E
      0x8C140175,  //  0009  GETMET	R5	R0	K117
      0x7C140200,  //  000A  CALL	R5	1
      0x5C100A00,  //  000B  MOVE	R4	R5
      0xA8040001,  //  000C  EXBLK	1	1
      0x70020011,  //  000D  JMP		#0020
      0xAC140002,  //  000E  CATCH	R5	0	2
      0x7002000E,  //  000F  JMP		#001F
      0x8C1C0322,  //  0010  GETMET	R7	R1	K34
      0x58240076,  //  0011  LDCONST	R9	K118
      0x7C1C0400,  //  0012  CALL	R7	2
      0x8C1C0322,  //  0013  GETMET	R7	R1	K34
      0x60240018,  //  0014  GETGBL	R9	G24
      0x58280077,  //  0015  LDCONST	R10	K119
      0x8C2C032A,  //  0016  GETMET	R11	R1	K42
      0x5C340C00,  //  0017  MOVE	R13	R6
      0x7C2C0400,  //  0018  CALL	R11	2
      0x7C240400,  //  0019  CALL	R9	2
      0x7C1C0400,  //  001A  CALL	R7	2
      0x8C1C0378,  //  001B  GETMET	R7	R1	K120
      0x7C1C0200,  //  001C  CALL	R7	1
      0x80000E00,  //  001D  RET	0
      0x70020000,  //  001E  JMP		#0020
      0xB0080000,  //  001F  RAISE	2	R0	R0
      0x8C140579,  //  0020  GETMET	R5	R2	K121
      0x5C1C0800,  //  0021  MOVE	R7	R4
      0x5820007A,  //  0022  LDCONST	R8	K122
      0x7C140600,  //  0023  CALL	R5	3
      0x8C180322,  //  0024  GETMET	R6	R1	K34
      0x5820007B,  //  0025  LDCONST	R8	K123
      0x7C180400,  //  0026  CALL	R6	2
      0x8C180322,  //  0027  GETMET	R6	R1	K34
      0x60200018,  //  0028  GETGBL	R8	G24
      0x5824007C,  //  0029  LDCONST	R9	K124
      0x5C280A00,  //  002A  MOVE	R10	R5
      0x7C200400,  //  002B  CALL	R8	2
      0x7C180400,  //  002C  CALL	R6	2
      0x8C180322,  //  002D  GETMET	R6	R1	K34
      0x5820007D,  //  002E  LDCONST	R8	K125
      0x7C180400,  //  002F  CALL	R6	2
      0x8C18017E,  //  0030  GETMET	R6	R0	K126
      0x7C180200,  //  0031  CALL	R6	1
      0x581C0047,  //  0032  LDCONST	R7	K71
      0x58200027,  //  0033  LDCONST	R8	K39
      0x6024000C,  //  0034  GETGBL	R9	G12
      0x5C280800,  //  0035  MOVE	R10	R4
      0x7C240200,  //  0036  CALL	R9	1
      0x14241009,  //  0037  LT	R9	R8	R9
      0x78260092,  //  0038  JMPF	R9	#00CC
      0x8C240530,  //  0039  GETMET	R9	R2	K48
      0x5C2C0800,  //  003A  MOVE	R11	R4
      0x5830007F,  //  003B  LDCONST	R12	K127
      0x5C341000,  //  003C  MOVE	R13	R8
      0x7C240800,  //  003D  CALL	R9	4
      0x14281327,  //  003E  LT	R10	R9	K39
      0x782A0003,  //  003F  JMPF	R10	#0044
      0x6028000C,  //  0040  GETGBL	R10	G12
      0x5C2C0800,  //  0041  MOVE	R11	R4
      0x7C280200,  //  0042  CALL	R10	1
      0x5C241400,  //  0043  MOVE	R9	R10
      0x40281009,  //  0044  CONNECT	R10	R8	R9
      0x9428080A,  //  0045  GETIDX	R10	R4	R10
      0x8C2C0180,  //  0046  GETMET	R11	R0	K128
      0x5C341400,  //  0047  MOVE	R13	R10
      0x7C2C0400,  //  0048  CALL	R11	2
      0x4C300000,  //  0049  LDNIL	R12
      0x2030160C,  //  004A  NE	R12	R11	R12
      0x7832007C,  //  004B  JMPF	R12	#00C9
      0x94301709,  //  004C  GETIDX	R12	R11	K9
      0x8C340181,  //  004D  GETMET	R13	R0	K129
      0x5C3C1800,  //  004E  MOVE	R15	R12
      0x7C340400,  //  004F  CALL	R13	2
      0x8C38032A,  //  0050  GETMET	R14	R1	K42
      0x94401736,  //  0051  GETIDX	R16	R11	K54
      0x7C380400,  //  0052  CALL	R14	2
      0x943C176C,  //  0053  GETIDX	R15	R11	K108
      0x8C400582,  //  0054  GETMET	R16	R2	K130
      0x8C48032A,  //  0055  GETMET	R18	R1	K42
      0x94501738,  //  0056  GETIDX	R20	R11	K56
      0x7C480400,  //  0057  CALL	R18	2
      0x584C0083,  //  0058  LDCONST	R19	K131
      0x58500084,  //  0059  LDCONST	R20	K132
      0x7C400800,  //  005A  CALL	R16	4
      0x9444176F,  //  005B  GETIDX	R17	R11	K111
      0x50480000,  //  005C  LDBOOL	R18	0	0
      0x4C4C0000,  //  005D  LDNIL	R19
      0x4C500000,  //  005E  LDNIL	R20
      0x8C540151,  //  005F  GETMET	R21	R0	K81
      0x945C176C,  //  0060  GETIDX	R23	R11	K108
      0x7C540400,  //  0061  CALL	R21	2
      0x5C502A00,  //  0062  MOVE	R20	R21
      0x8C54032A,  //  0063  GETMET	R21	R1	K42
      0x5C5C2800,  //  0064  MOVE	R23	R20
      0x7C540400,  //  0065  CALL	R21	2
      0x8C580D2D,  //  0066  GETMET	R22	R6	K45
      0x5C602800,  //  0067  MOVE	R24	R20
      0x7C580400,  //  0068  CALL	R22	2
      0x5C482C00,  //  0069  MOVE	R18	R22
      0x4C580000,  //  006A  LDNIL	R22
      0x784A000F,  //  006B  JMPF	R18	#007C
      0x945C0C14,  //  006C  GETIDX	R23	R6	R20
      0x8C60032A,  //  006D  GETMET	R24	R1	K42
      0x5C682E00,  //  006E  MOVE	R26	R23
      0x7C600400,  //  006F  CALL	R24	2
      0x5C583000,  //  0070  MOVE	R22	R24
      0xB8620C00,  //  0071  GETNGBL	R24	K6
      0x8C60312B,  //  0072  GETMET	R24	R24	K43
      0x5C682E00,  //  0073  MOVE	R26	R23
      0x7C600400,  //  0074  CALL	R24	2
      0x60640009,  //  0075  GETGBL	R25	G9
      0x8C683130,  //  0076  GETMET	R26	R24	K48
      0x58700009,  //  0077  LDCONST	R28	K9
      0x58740027,  //  0078  LDCONST	R29	K39
      0x7C680600,  //  0079  CALL	R26	3
      0x7C640200,  //  007A  CALL	R25	1
      0x5C4C3200,  //  007B  MOVE	R19	R25
      0x784A0001,  //  007C  JMPF	R18	#007F
      0x145C260C,  //  007D  LT	R23	R19	R12
      0x745E0000,  //  007E  JMPT	R23	#0080
      0x505C0001,  //  007F  LDBOOL	R23	0	1
      0x505C0200,  //  0080  LDBOOL	R23	1	0
      0x8C600322,  //  0081  GETMET	R24	R1	K34
      0x60680018,  //  0082  GETGBL	R26	G24
      0x586C0085,  //  0083  LDCONST	R27	K133
      0x5C700E00,  //  0084  MOVE	R28	R7
      0x5C741C00,  //  0085  MOVE	R29	R14
      0x8C780181,  //  0086  GETMET	R30	R0	K129
      0x5C801800,  //  0087  MOVE	R32	R12
      0x7C780400,  //  0088  CALL	R30	2
      0x7C680800,  //  0089  CALL	R26	4
      0x7C600400,  //  008A  CALL	R24	2
      0x785E0003,  //  008B  JMPF	R23	#0090
      0x8C600322,  //  008C  GETMET	R24	R1	K34
      0x58680086,  //  008D  LDCONST	R26	K134
      0x7C600400,  //  008E  CALL	R24	2
      0x70020003,  //  008F  JMP		#0094
      0x784A0002,  //  0090  JMPF	R18	#0094
      0x8C600322,  //  0091  GETMET	R24	R1	K34
      0x58680087,  //  0092  LDCONST	R26	K135
      0x7C600400,  //  0093  CALL	R24	2
      0x8C600322,  //  0094  GETMET	R24	R1	K34
      0x60680018,  //  0095  GETGBL	R26	G24
      0x586C0088,  //  0096  LDCONST	R27	K136
      0x5C700E00,  //  0097  MOVE	R28	R7
      0x5C740E00,  //  0098  MOVE	R29	R7
      0x5C782000,  //  0099  MOVE	R30	R16
      0x7C680800,  //  009A  CALL	R26	4
      0x7C600400,  //  009B  CALL	R24	2
      0x785E0008,  //  009C  JMPF	R23	#00A6
      0x8C600322,  //  009D  GETMET	R24	R1	K34
      0x60680018,  //  009E  GETGBL	R26	G24
      0x586C0089,  //  009F  LDCONST	R27	K137
      0x8C700181,  //  00A0  GETMET	R28	R0	K129
      0x5C782600,  //  00A1  MOVE	R30	R19
      0x7C700400,  //  00A2  CALL	R28	2
      0x5C741A00,  //  00A3  MOVE	R29	R13
      0x7C680600,  //  00A4  CALL	R26	3
      0x7C600400,  //  00A5  CALL	R24	2
      0x8C600322,  //  00A6  GETMET	R24	R1	K34
      0x5868008A,  //  00A7  LDCONST	R26	K138
      0x7C600400,  //  00A8  CALL	R24	2
      0x784A0013,  //  00A9  JMPF	R18	#00BE
      0x785E0007,  //  00AA  JMPF	R23	#00B3
      0x8C600322,  //  00AB  GETMET	R24	R1	K34
      0x60680018,  //  00AC  GETGBL	R26	G24
      0x586C008B,  //  00AD  LDCONST	R27	K139
      0x5C702C00,  //  00AE  MOVE	R28	R22
      0x5C742C00,  //  00AF  MOVE	R29	R22
      0x7C680600,  //  00B0  CALL	R26	3
      0x7C600400,  //  00B1  CALL	R24	2
      0x70020002,  //  00B2  JMP		#00B6
      0x8C600322,  //  00B3  GETMET	R24	R1	K34
      0x5868008C,  //  00B4  LDCONST	R26	K140
      0x7C600400,  //  00B5  CALL	R24	2
      0x8C600322,  //  00B6  GETMET	R24	R1	K34
      0x60680018,  //  00B7  GETGBL	R26	G24
      0x586C008D,  //  00B8  LDCONST	R27	K141
      0x5C702C00,  //  00B9  MOVE	R28	R22
      0x5C742C00,  //  00BA  MOVE	R29	R22
      0x7C680600,  //  00BB  CALL	R26	3
      0x7C600400,  //  00BC  CALL	R24	2
      0x70020006,  //  00BD  JMP		#00C5
      0x8C600322,  //  00BE  GETMET	R24	R1	K34
      0x60680018,  //  00BF  GETGBL	R26	G24
      0x586C008E,  //  00C0  LDCONST	R27	K142
      0x5C702A00,  //  00C1  MOVE	R28	R21
      0x5C741C00,  //  00C2  MOVE	R29	R14
      0x7C680600,  //  00C3  CALL	R26	3
      0x7C600400,  //  00C4  CALL	R24	2
      0x8C600322,  //  00C5  GETMET	R24	R1	K34
      0x58680046,  //  00C6  LDCONST	R26	K70
      0x7C600400,  //  00C7  CALL	R24	2
      0x001C0F47,  //  00C8  ADD	R7	R7	K71
      0x00301347,  //  00C9  ADD	R12	R9	K71
      0x5C201800,  //  00CA  MOVE	R8	R12
      0x7001FF67,  //  00CB  JMP		#0034
      0x8C240322,  //  00CC  GETMET	R9	R1	K34
      0x582C0049,  //  00CD  LDCONST	R11	K73
      0x7C240400,  //  00CE  CALL	R9	2
      0x8C240378,  //  00CF  GETMET	R9	R1	K120
      0x7C240200,  //  00D0  CALL	R9	1
      0x80000000,  //  00D1  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: list_installed_ext
********************************************************************/
be_local_closure(class_Extension_manager_list_installed_ext,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    0,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Extension_manager,     /* shared constants */
    &be_const_str_list_installed_ext,
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x58000060,  //  0000  LDCONST	R0	K96
      0x60040013,  //  0001  GETGBL	R1	G19
      0x7C040000,  //  0002  CALL	R1	0
      0x60080010,  //  0003  GETGBL	R2	G16
      0x8C0C0167,  //  0004  GETMET	R3	R0	K103
      0x7C0C0200,  //  0005  CALL	R3	1
      0x7C080200,  //  0006  CALL	R2	1
      0xA8020006,  //  0007  EXBLK	0	#000F
      0x5C0C0400,  //  0008  MOVE	R3	R2
      0x7C0C0000,  //  0009  CALL	R3	0
      0x8C100151,  //  000A  GETMET	R4	R0	K81
      0x5C180600,  //  000B  MOVE	R6	R3
      0x7C100400,  //  000C  CALL	R4	2
      0x98040803,  //  000D  SETIDX	R1	R4	R3
      0x7001FFF8,  //  000E  JMP		#0008
      0x58080064,  //  000F  LDCONST	R2	K100
      0xAC080200,  //  0010  CATCH	R2	1	0
      0xB0080000,  //  0011  RAISE	2	R0	R0
      0x80040200,  //  0012  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: version_string
********************************************************************/
be_local_closure(class_Extension_manager_version_string,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Extension_manager,     /* shared constants */
    &be_const_str_version_string,
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x58040060,  //  0000  LDCONST	R1	K96
      0x60080018,  //  0001  GETGBL	R2	G24
      0x580C008F,  //  0002  LDCONST	R3	K143
      0x54120017,  //  0003  LDINT	R4	24
      0x3C100004,  //  0004  SHR	R4	R0	R4
      0x541600FE,  //  0005  LDINT	R5	255
      0x2C100805,  //  0006  AND	R4	R4	R5
      0x5416000F,  //  0007  LDINT	R5	16
      0x3C140005,  //  0008  SHR	R5	R0	R5
      0x541A00FE,  //  0009  LDINT	R6	255
      0x2C140A06,  //  000A  AND	R5	R5	R6
      0x541A0007,  //  000B  LDINT	R6	8
      0x3C180006,  //  000C  SHR	R6	R0	R6
      0x541E00FE,  //  000D  LDINT	R7	255
      0x2C180C07,  //  000E  AND	R6	R6	R7
      0x541E00FE,  //  000F  LDINT	R7	255
      0x2C1C0007,  //  0010  AND	R7	R0	R7
      0x7C080A00,  //  0011  CALL	R2	5
      0x80040400,  //  0012  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: check_or_create_dir
********************************************************************/
be_local_closure(class_Extension_manager_check_or_create_dir,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Extension_manager,     /* shared constants */
    &be_const_str_check_or_create_dir,
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0x58040060,  //  0000  LDCONST	R1	K96
      0xA40AA000,  //  0001  IMPORT	R2	K80
      0x8C0C055C,  //  0002  GETMET	R3	R2	K92
      0x5C140000,  //  0003  MOVE	R5	R0
      0x7C0C0400,  //  0004  CALL	R3	2
      0x780E0008,  //  0005  JMPF	R3	#000F
      0x8C0C0590,  //  0006  GETMET	R3	R2	K144
      0x5C140000,  //  0007  MOVE	R5	R0
      0x7C0C0400,  //  0008  CALL	R3	2
      0x780E0001,  //  0009  JMPF	R3	#000C
      0x80000600,  //  000A  RET	0
      0x70020002,  //  000B  JMP		#000F
      0x8C0C0591,  //  000C  GETMET	R3	R2	K145
      0x5C140000,  //  000D  MOVE	R5	R0
      0x7C0C0400,  //  000E  CALL	R3	2
      0x8C0C0592,  //  000F  GETMET	R3	R2	K146
      0x5C140000,  //  0010  MOVE	R5	R0
      0x7C0C0400,  //  0011  CALL	R3	2
      0x8C0C055C,  //  0012  GETMET	R3	R2	K92
      0x5C140000,  //  0013  MOVE	R5	R0
      0x7C0C0400,  //  0014  CALL	R3	2
      0x780E0003,  //  0015  JMPF	R3	#001A
      0x8C0C0590,  //  0016  GETMET	R3	R2	K144
      0x5C140000,  //  0017  MOVE	R5	R0
      0x7C0C0400,  //  0018  CALL	R3	2
      0x740E0004,  //  0019  JMPT	R3	#001F
      0x600C0018,  //  001A  GETGBL	R3	G24
      0x58100093,  //  001B  LDCONST	R4	K147
      0x5C140000,  //  001C  MOVE	R5	R0
      0x7C0C0400,  //  001D  CALL	R3	2
      0xB006BE03,  //  001E  RAISE	1	K95	R3
      0x80000000,  //  001F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: page_extensions_ctl
********************************************************************/
be_local_closure(class_Extension_manager_page_extensions_ctl,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Extension_manager,     /* shared constants */
    &be_const_str_page_extensions_ctl,
    &be_const_str_solidified,
    ( &(const binstruction[165]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40AA000,  //  0001  IMPORT	R2	K80
      0xA40E3C00,  //  0002  IMPORT	R3	K30
      0x8C100301,  //  0003  GETMET	R4	R1	K1
      0x7C100200,  //  0004  CALL	R4	1
      0x74120001,  //  0005  JMPT	R4	#0008
      0x4C100000,  //  0006  LDNIL	R4
      0x80040800,  //  0007  RET	1	R4
      0xA8020079,  //  0008  EXBLK	0	#0083
      0x8C100394,  //  0009  GETMET	R4	R1	K148
      0x58180027,  //  000A  LDCONST	R6	K39
      0x7C100400,  //  000B  CALL	R4	2
      0x94140927,  //  000C  GETIDX	R5	R4	K39
      0x401A8F95,  //  000D  CONNECT	R6	K71	K149
      0x94180806,  //  000E  GETIDX	R6	R4	R6
      0x1C1C0B3D,  //  000F  EQ	R7	R5	K61
      0x781E0006,  //  0010  JMPF	R7	#0018
      0x201C0D2F,  //  0011  NE	R7	R6	K47
      0x781E0003,  //  0012  JMPF	R7	#0017
      0xB81E0C00,  //  0013  GETNGBL	R7	K6
      0x8C1C0F6A,  //  0014  GETMET	R7	R7	K106
      0x5C240C00,  //  0015  MOVE	R9	R6
      0x7C1C0400,  //  0016  CALL	R7	2
      0x70020063,  //  0017  JMP		#007C
      0x1C1C0B3C,  //  0018  EQ	R7	R5	K60
      0x781E0004,  //  0019  JMPF	R7	#001F
      0xB81E0C00,  //  001A  GETNGBL	R7	K6
      0x8C1C0F96,  //  001B  GETMET	R7	R7	K150
      0x5C240C00,  //  001C  MOVE	R9	R6
      0x7C1C0400,  //  001D  CALL	R7	2
      0x7002005C,  //  001E  JMP		#007C
      0x1C1C0B41,  //  001F  EQ	R7	R5	K65
      0x741E0001,  //  0020  JMPT	R7	#0023
      0x1C1C0B42,  //  0021  EQ	R7	R5	K66
      0x781E0039,  //  0022  JMPF	R7	#005D
      0x4C1C0000,  //  0023  LDNIL	R7
      0x1C200B41,  //  0024  EQ	R8	R5	K65
      0x7822000A,  //  0025  JMPF	R8	#0031
      0x8C200752,  //  0026  GETMET	R8	R3	K82
      0x5C280C00,  //  0027  MOVE	R10	R6
      0x582C0053,  //  0028  LDCONST	R11	K83
      0x7C200600,  //  0029  CALL	R8	3
      0x78220005,  //  002A  JMPF	R8	#0031
      0x5421FFFA,  //  002B  LDINT	R8	-5
      0x40224E08,  //  002C  CONNECT	R8	K39	R8
      0x94200C08,  //  002D  GETIDX	R8	R6	R8
      0x00201197,  //  002E  ADD	R8	R8	K151
      0x5C1C1000,  //  002F  MOVE	R7	R8
      0x7002000B,  //  0030  JMP		#003D
      0x1C200B42,  //  0031  EQ	R8	R5	K66
      0x78220009,  //  0032  JMPF	R8	#003D
      0x8C200752,  //  0033  GETMET	R8	R3	K82
      0x5C280C00,  //  0034  MOVE	R10	R6
      0x582C0062,  //  0035  LDCONST	R11	K98
      0x7C200600,  //  0036  CALL	R8	3
      0x78220004,  //  0037  JMPF	R8	#003D
      0x5421FFF9,  //  0038  LDINT	R8	-6
      0x40224E08,  //  0039  CONNECT	R8	K39	R8
      0x94200C08,  //  003A  GETIDX	R8	R6	R8
      0x00201198,  //  003B  ADD	R8	R8	K152
      0x5C1C1000,  //  003C  MOVE	R7	R8
      0x781E0016,  //  003D  JMPF	R7	#0055
      0x8C200599,  //  003E  GETMET	R8	R2	K153
      0x5C280C00,  //  003F  MOVE	R10	R6
      0x5C2C0E00,  //  0040  MOVE	R11	R7
      0x7C200600,  //  0041  CALL	R8	3
      0x78220010,  //  0042  JMPF	R8	#0054
      0xB8260C00,  //  0043  GETNGBL	R9	K6
      0x8824132C,  //  0044  GETMBR	R9	R9	K44
      0x8C24132D,  //  0045  GETMET	R9	R9	K45
      0x5C2C0C00,  //  0046  MOVE	R11	R6
      0x7C240400,  //  0047  CALL	R9	2
      0x7826000A,  //  0048  JMPF	R9	#0054
      0xB8260C00,  //  0049  GETNGBL	R9	K6
      0x8824132C,  //  004A  GETMBR	R9	R9	K44
      0xB82A0C00,  //  004B  GETNGBL	R10	K6
      0x8828152C,  //  004C  GETMBR	R10	R10	K44
      0x94281406,  //  004D  GETIDX	R10	R10	R6
      0x98240E0A,  //  004E  SETIDX	R9	R7	R10
      0xB8260C00,  //  004F  GETNGBL	R9	K6
      0x8824132C,  //  0050  GETMBR	R9	R9	K44
      0x8C241391,  //  0051  GETMET	R9	R9	K145
      0x5C2C0C00,  //  0052  MOVE	R11	R6
      0x7C240400,  //  0053  CALL	R9	2
      0x70020006,  //  0054  JMP		#005C
      0xB8221A00,  //  0055  GETNGBL	R8	K13
      0x60240018,  //  0056  GETGBL	R9	G24
      0x5828009A,  //  0057  LDCONST	R10	K154
      0x5C2C0800,  //  0058  MOVE	R11	R4
      0x7C240400,  //  0059  CALL	R9	2
      0x5828000F,  //  005A  LDCONST	R10	K15
      0x7C200400,  //  005B  CALL	R8	2
      0x7002001E,  //  005C  JMP		#007C
      0x1C1C0B9B,  //  005D  EQ	R7	R5	K155
      0x781E0009,  //  005E  JMPF	R7	#0069
      0x201C0D2F,  //  005F  NE	R7	R6	K47
      0x781E0006,  //  0060  JMPF	R7	#0068
      0xB81E0C00,  //  0061  GETNGBL	R7	K6
      0x8C1C0F96,  //  0062  GETMET	R7	R7	K150
      0x5C240C00,  //  0063  MOVE	R9	R6
      0x7C1C0400,  //  0064  CALL	R7	2
      0x8C1C0591,  //  0065  GETMET	R7	R2	K145
      0x5C240C00,  //  0066  MOVE	R9	R6
      0x7C1C0400,  //  0067  CALL	R7	2
      0x70020012,  //  0068  JMP		#007C
      0x1C1C0B9C,  //  0069  EQ	R7	R5	K156
      0x781E0009,  //  006A  JMPF	R7	#0075
      0xB81E0C00,  //  006B  GETNGBL	R7	K6
      0x8C1C0F96,  //  006C  GETMET	R7	R7	K150
      0x5C240C00,  //  006D  MOVE	R9	R6
      0x7C1C0400,  //  006E  CALL	R7	2
      0x8C1C019D,  //  006F  GETMET	R7	R0	K157
      0x8C240151,  //  0070  GETMET	R9	R0	K81
      0x5C2C0C00,  //  0071  MOVE	R11	R6
      0x7C240400,  //  0072  CALL	R9	2
      0x7C1C0400,  //  0073  CALL	R7	2
      0x70020006,  //  0074  JMP		#007C
      0x1C1C0B9E,  //  0075  EQ	R7	R5	K158
      0x781E0004,  //  0076  JMPF	R7	#007C
      0x8C1C019D,  //  0077  GETMET	R7	R0	K157
      0x8C240151,  //  0078  GETMET	R9	R0	K81
      0x5C2C0C00,  //  0079  MOVE	R11	R6
      0x7C240400,  //  007A  CALL	R9	2
      0x7C1C0400,  //  007B  CALL	R7	2
      0x8C1C039F,  //  007C  GETMET	R7	R1	K159
      0x60240018,  //  007D  GETGBL	R9	G24
      0x5828001B,  //  007E  LDCONST	R10	K27
      0x7C240200,  //  007F  CALL	R9	1
      0x7C1C0400,  //  0080  CALL	R7	2
      0xA8040001,  //  0081  EXBLK	1	1
      0x70020020,  //  0082  JMP		#00A4
      0xAC100002,  //  0083  CATCH	R4	0	2
      0x7002001D,  //  0084  JMP		#00A3
      0xB81A1A00,  //  0085  GETNGBL	R6	K13
      0x601C0018,  //  0086  GETGBL	R7	G24
      0x582000A0,  //  0087  LDCONST	R8	K160
      0x5C240800,  //  0088  MOVE	R9	R4
      0x5C280A00,  //  0089  MOVE	R10	R5
      0x7C1C0600,  //  008A  CALL	R7	3
      0x58200014,  //  008B  LDCONST	R8	K20
      0x7C180400,  //  008C  CALL	R6	2
      0x8C18031F,  //  008D  GETMET	R6	R1	K31
      0x582000A1,  //  008E  LDCONST	R8	K161
      0x7C180400,  //  008F  CALL	R6	2
      0x8C180321,  //  0090  GETMET	R6	R1	K33
      0x7C180200,  //  0091  CALL	R6	1
      0x8C180322,  //  0092  GETMET	R6	R1	K34
      0x60200018,  //  0093  GETGBL	R8	G24
      0x582400A2,  //  0094  LDCONST	R9	K162
      0x8C28032A,  //  0095  GETMET	R10	R1	K42
      0x5C300800,  //  0096  MOVE	R12	R4
      0x7C280400,  //  0097  CALL	R10	2
      0x8C2C032A,  //  0098  GETMET	R11	R1	K42
      0x5C340A00,  //  0099  MOVE	R13	R5
      0x7C2C0400,  //  009A  CALL	R11	2
      0x7C200600,  //  009B  CALL	R8	3
      0x7C180400,  //  009C  CALL	R6	2
      0x8C18034C,  //  009D  GETMET	R6	R1	K76
      0x882003A3,  //  009E  GETMBR	R8	R1	K163
      0x7C180400,  //  009F  CALL	R6	2
      0x8C18034E,  //  00A0  GETMET	R6	R1	K78
      0x7C180200,  //  00A1  CALL	R6	1
      0x70020000,  //  00A2  JMP		#00A4
      0xB0080000,  //  00A3  RAISE	2	R0	R0
      0x80000000,  //  00A4  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Extension_manager
********************************************************************/
be_local_class(Extension_manager,
    0,
    NULL,
    be_nested_map(20,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(page_extensions_mgr_dispatcher, 17), be_const_closure(class_Extension_manager_page_extensions_mgr_dispatcher_closure) },
        { be_const_key(page_extensions_ctl, -1), be_const_closure(class_Extension_manager_page_extensions_ctl_closure) },
        { be_const_key(web_add_handler, -1), be_const_closure(class_Extension_manager_web_add_handler_closure) },
        { be_const_key(EXT_REPO_MANIFEST, -1), be_nested_str(extensions_X2Ejsonl) },
        { be_const_key(page_extensions_mgr, -1), be_const_closure(class_Extension_manager_page_extensions_mgr_closure) },
        { be_const_key(EXT_REPO, 7), be_nested_str(https_X3A_X2F_X2Fota_X2Etasmota_X2Ecom_X2Fextensions_X2F) },
        { be_const_key(check_or_create_dir, 14), be_const_static_closure(class_Extension_manager_check_or_create_dir_closure) },
        { be_const_key(version_string, -1), be_const_static_closure(class_Extension_manager_version_string_closure) },
        { be_const_key(list_installed_ext, 11), be_const_static_closure(class_Extension_manager_list_installed_ext_closure) },
        { be_const_key(EXT_FOLDER, -1), be_nested_str(_X2F_X2Eextensions_X2F) },
        { be_const_key(page_extensions_store, 13), be_const_closure(class_Extension_manager_page_extensions_store_closure) },
        { be_const_key(tapp_name, -1), be_const_static_closure(class_Extension_manager_tapp_name_closure) },
        { be_const_key(list_extensions_in_fs, 8), be_const_static_closure(class_Extension_manager_list_extensions_in_fs_closure) },
        { be_const_key(manifest_decode, 16), be_const_static_closure(class_Extension_manager_manifest_decode_closure) },
        { be_const_key(EXT_REPO_FOLDER, -1), be_nested_str(tapp_X2F) },
        { be_const_key(install_from_store, 10), be_const_closure(class_Extension_manager_install_from_store_closure) },
        { be_const_key(init, -1), be_const_closure(class_Extension_manager_init_closure) },
        { be_const_key(list_extensions, -1), be_const_static_closure(class_Extension_manager_list_extensions_closure) },
        { be_const_key(web_add_button, 6), be_const_closure(class_Extension_manager_web_add_button_closure) },
        { be_const_key(load_manifest, 1), be_const_closure(class_Extension_manager_load_manifest_closure) },
    })),
    (bstring*) &be_const_str_Extension_manager
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
    /* K0   */  be_nested_str(Extension_manager),
    }),
    &be_const_str__anonymous_,
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x80040200,  //  0002  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified module: extension_manager
********************************************************************/
be_local_module(extension_manager,
    "extension_manager",
    be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(Extension_manager, -1), be_const_class(be_class_Extension_manager) },
        { be_const_key(init, -1), be_const_closure(_anonymous__closure) },
    }))
);
BE_EXPORT_VARIABLE be_define_const_native_module(extension_manager);
/********************************************************************/
/********************************************************************/
/* End of solidification */
