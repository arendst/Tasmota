/* Solidification of extension_manager.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
extern const bclass be_class_Extension_manager;
// compact class 'Extension_manager' ktab size: 182, total: 319 (saved 1096 bytes)
static const bvalue be_ktab_class_Extension_manager[182] = {
  /* K0   */  be_nested_str(webserver),
  /* K1   */  be_nested_str(check_privileged_access),
  /* K2   */  be_nested_str(has_arg),
  /* K3   */  be_nested_str(store),
  /* K4   */  be_nested_str(page_extensions_store),
  /* K5   */  be_nested_str(page_extensions_mgr),
  /* K6   */  be_const_class(be_class_Extension_manager),
  /* K7   */  be_nested_str(tapp_name),
  /* K8   */  be_nested_str(list_installed_ext),
  /* K9   */  be_nested_str(find),
  /* K10  */  be_nested_str(path),
  /* K11  */  be_nested_str(run_stop_ext),
  /* K12  */  be_nested_str(remove),
  /* K13  */  be_nested_str(on),
  /* K14  */  be_nested_str(_X2Fext),
  /* K15  */  be_nested_str(HTTP_GET),
  /* K16  */  be_nested_str(HTTP_POST),
  /* K17  */  be_nested_str(exists),
  /* K18  */  be_nested_str(isdir),
  /* K19  */  be_nested_str(mkdir),
  /* K20  */  be_nested_str(cannot_X20create_X20folder_X20_X27_X25s_X27),
  /* K21  */  be_nested_str(io_error),
  /* K22  */  be_nested_str(string),
  /* K23  */  be_const_int(0),
  /* K24  */  be_const_int(1),
  /* K25  */  be_nested_str(_X23),
  /* K26  */  be_nested_str(_),
  /* K27  */  be_nested_str(_X2Etapp),
  /* K28  */  be_nested_str(_X2F),
  /* K29  */  be_nested_str(listdir),
  /* K30  */  be_nested_str(EXT_FOLDER),
  /* K31  */  be_nested_str(endswith),
  /* K32  */  be_nested_str(_X2Etapp_),
  /* K33  */  be_nested_str(push),
  /* K34  */  be_nested_str(stop_iteration),
  /* K35  */  be_nested_str(ext_repo),
  /* K36  */  be_nested_str(),
  /* K37  */  be_nested_str(tasmota),
  /* K38  */  be_nested_str(add_driver),
  /* K39  */  be_nested_str(json),
  /* K40  */  be_nested_str(load),
  /* K41  */  be_nested_str(log),
  /* K42  */  be_nested_str(EXT_X3A_X20unable_X20to_X20parse_X20manifest_X20line_X20_X27_X25s_X27),
  /* K43  */  be_const_int(3),
  /* K44  */  be_nested_str(contains),
  /* K45  */  be_nested_str(name),
  /* K46  */  be_nested_str(file),
  /* K47  */  be_nested_str(version),
  /* K48  */  be_nested_str(EXT_X3A_X20manifest_X20is_X20missing_X20_X27name_X2Ffile_X2Fversion_X27_X20in_X20map_X20_X27_X25s_X27),
  /* K49  */  be_nested_str(description),
  /* K50  */  be_nested_str(_X5Bno_X20description_X5D),
  /* K51  */  be_nested_str(author),
  /* K52  */  be_nested_str(content_send),
  /* K53  */  be_nested_str(_X3Cp_X3E_X3C_X2Fp_X3E_X3Cform_X20id_X3Dbut_part_mgr_X20style_X3D_X27display_X3A_X20block_X3B_X27_X20action_X3D_X27ext_X27_X20method_X3D_X27get_X27_X3E_X3Cbutton_X3EExtension_X20Manager_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
  /* K54  */  be_nested_str(sortedmap),
  /* K55  */  be_nested_str(list_extensions),
  /* K56  */  be_nested_str(read_extension_manifest),
  /* K57  */  be_nested_str(EXT_X3A_X20unable_X20to_X20read_X20details_X20from_X20_X27_X25s_X27),
  /* K58  */  be_nested_str(v_X25s_X2E_X25s_X2E_X25s_X2E_X25s),
  /* K59  */  be_nested_str(_X25s_X25s_X25s),
  /* K60  */  be_nested_str(EXT_REPO_FOLDER),
  /* K61  */  be_nested_str(EXT_X3A_X20installing_X20from_X20_X27_X25s_X27),
  /* K62  */  be_nested_str(check_or_create_dir),
  /* K63  */  be_nested_str(_X25s_X25s),
  /* K64  */  be_nested_str(webclient),
  /* K65  */  be_nested_str(begin),
  /* K66  */  be_nested_str(GET),
  /* K67  */  be_nested_str(EXT_X3A_X20return_code_X3D_X25s),
  /* K68  */  be_const_int(2),
  /* K69  */  be_nested_str(write_file),
  /* K70  */  be_nested_str(close),
  /* K71  */  be_nested_str(EXT_X3A_X20successfully_X20installed_X20_X27_X25s_X27_X20_X25s_X20bytes),
  /* K72  */  be_nested_str(could_X20not_X20download_X20into_X20_X27_X25s_X27_X20ret_X3D_X25s),
  /* K73  */  be_nested_str(EXT_X3A_X20exception_X20_X27_X25s_X27_X20_X2D_X20_X27_X25s_X27),
  /* K74  */  be_nested_str(arg_name),
  /* K75  */  be_const_int(2147483647),
  /* K76  */  be_nested_str(r),
  /* K77  */  be_nested_str(s),
  /* K78  */  be_nested_str(a),
  /* K79  */  be_nested_str(A),
  /* K80  */  be_nested_str(enable_disable_ext),
  /* K81  */  be_nested_str(d),
  /* K82  */  be_nested_str(delete_ext),
  /* K83  */  be_nested_str(u),
  /* K84  */  be_nested_str(install_from_store),
  /* K85  */  be_nested_str(i),
  /* K86  */  be_nested_str(I),
  /* K87  */  be_nested_str(x),
  /* K88  */  be_nested_str(arg),
  /* K89  */  be_nested_str(0),
  /* K90  */  be_nested_str(1),
  /* K91  */  be_nested_str(EXT_REPO),
  /* K92  */  be_nested_str(split),
  /* K93  */  be_nested_str(size),
  /* K94  */  be_nested_str(http_X3A),
  /* K95  */  be_nested_str(https_X3A),
  /* K96  */  be_nested_str(extensions),
  /* K97  */  be_nested_str(EXT_X3A_X20wrong_X20action_X20_X27_X25s_X27),
  /* K98  */  be_nested_str(redirect),
  /* K99  */  be_nested_str(EXT_X3A_X20Exception_X3E_X20_X27_X25s_X27_X20_X2D_X20_X25s),
  /* K100 */  be_nested_str(content_start),
  /* K101 */  be_nested_str(Parameter_X20error),
  /* K102 */  be_nested_str(content_send_style),
  /* K103 */  be_nested_str(_X3Cp_X20style_X3D_X27width_X3A340px_X3B_X27_X3E_X3Cb_X3EException_X3A_X3C_X2Fb_X3E_X3Cbr_X3E_X27_X25s_X27_X3Cbr_X3E_X25s_X3C_X2Fp_X3E),
  /* K104 */  be_nested_str(html_escape),
  /* K105 */  be_nested_str(content_button),
  /* K106 */  be_nested_str(BUTTON_MANAGEMENT),
  /* K107 */  be_nested_str(content_stop),
  /* K108 */  be_nested_str(Extensions_X20Manager),
  /* K109 */  be_nested_str(_X3Cdiv_X20style_X3D_X27padding_X3A0px_X205px_X3Btext_X2Dalign_X3Acenter_X3B_X27_X3E_X3Ch3_X3E_X3Chr_X3EExtension_X20Manager_X3Chr_X3E_X3C_X2Fh3_X3E_X3C_X2Fdiv_X3E),
  /* K110 */  be_nested_str_long(_X3Cscript_X3Efunction_X20loadext_X28_X29_X20_X7Beb_X28_X27store_X27_X29_X2Edisabled_X3Dtrue_X3Bx_X3Dnew_X20XMLHttpRequest_X28_X29_X3Bx_X2Etimeout_X3D4000_X3Bx_X2Eonreadystatechange_X20_X3D_X20_X28_X29_X20_X3D_X3E_X20_X7Bif_X28x_X2EreadyState_X3D_X3D4_X29_X7Bif_X28x_X2Estatus_X3D_X3D200_X29_X7Beb_X28_X27inet_X27_X29_X2Estyle_X2Edisplay_X3D_X27none_X27_X3Beb_X28_X27store_X27_X29_X2EouterHTML_X3Dx_X2EresponseText_X3B_X7D_X7D_X7D_X3Bx_X2Eopen_X28_X27GET_X27_X2C_X27_X3Fstore_X3D_X27_X29_X3Bx_X2Esend_X28_X29_X3B_X7Dwindow_X2Eonload_X3Dfunction_X28_X29_X7Bloadext_X28_X29_X3B_X7D_X3Bfunction_X20toggleDesc_X28id_X29_X20_X7Bvar_X20desc_X20_X3D_X20document_X2EgetElementById_X28_X27desc_X2D_X27_X20_X2B_X20id_X29_X3Bvar_X20arrow_X20_X3D_X20document_X2EgetElementById_X28_X27arrow_X2D_X27_X20_X2B_X20id_X29_X3Bif_X20_X28desc_X2Estyle_X2Edisplay_X20_X3D_X3D_X3D_X20_X27none_X27_X20_X7C_X7C_X20desc_X2Estyle_X2Edisplay_X20_X3D_X3D_X3D_X20_X27_X27_X29_X20_X7Bdesc_X2Estyle_X2Edisplay_X20_X3D_X20_X27block_X27_X3Barrow_X2EinnerHTML_X20_X3D_X20_X27_XE2_X96_XBC_X27_X3B_X7D_X20else_X20_X7Bdesc_X2Estyle_X2Edisplay_X20_X3D_X20_X27none_X27_X3Barrow_X2EinnerHTML_X20_X3D_X20_X27_XE2_X96_XB6_X27_X3B_X7D_X7Dfunction_X20filterExtensions_X28query_X29_X20_X7Bvar_X20items_X20_X3D_X20document_X2EgetElementsByClassName_X28_X27ext_X2Dstore_X2Ditem_X27_X29_X3Bquery_X20_X3D_X20query_X2EtoLowerCase_X28_X29_X3Bfor_X20_X28var_X20i_X20_X3D_X200_X3B_X20i_X20_X3C_X20items_X2Elength_X3B_X20i_X2B_X2B_X29_X20_X7Bvar_X20name_X20_X3D_X20items_X5Bi_X5D_X2EgetElementsByClassName_X28_X27ext_X2Dname_X27_X29_X5B0_X5D_X2EtextContent_X2EtoLowerCase_X28_X29_X3Bvar_X20desc_X20_X3D_X20items_X5Bi_X5D_X2EgetElementsByClassName_X28_X27ext_X2Ddesc_X27_X29_X5B0_X5D_X2EtextContent_X2EtoLowerCase_X28_X29_X3Bif_X20_X28name_X2Eincludes_X28query_X29_X20_X7C_X7C_X20desc_X2Eincludes_X28query_X29_X29_X20_X7Bitems_X5Bi_X5D_X2Estyle_X2Edisplay_X20_X3D_X20_X27block_X27_X3B_X7D_X20else_X20_X7Bitems_X5Bi_X5D_X2Estyle_X2Edisplay_X20_X3D_X20_X27none_X27_X3B_X7D_X7D_X7D_X3C_X2Fscript_X3E),
  /* K111 */  be_nested_str_long(_X3Cfieldset_X20style_X3D_X27padding_X3A0_X205px_X3B_X27_X3E_X3Cstyle_X3E_X2Eext_X2Ditem_X7Bwidth_X3Amin_X2Dcontent_X3Bmin_X2Dwidth_X3A100_X25_X3B_X7D_X2Eext_X2Ditem_X20small_X7Bdisplay_X3Ablock_X3Bword_X2Dwrap_X3Abreak_X2Dword_X3Boverflow_X2Dwrap_X3Abreak_X2Dword_X3Bwhite_X2Dspace_X3Anormal_X3Bpadding_X2Dright_X3A5px_X3Bpadding_X2Dtop_X3A2px_X3B_X7D_X2Eext_X2Dcontrols_X7Bdisplay_X3Aflex_X3Bgap_X3A8px_X3Balign_X2Ditems_X3Acenter_X3Bmargin_X2Dtop_X3A4px_X3Bpadding_X3A0px_X7D_X2Ebtn_X2Dsmall_X7Bpadding_X3A0_X206px_X3Bline_X2Dheight_X3A1_X2E8rem_X3Bfont_X2Dsize_X3A0_X2E9rem_X3Bmin_X2Dwidth_X3Aauto_X3Bwidth_X3Aauto_X3Bflex_X2Dshrink_X3A0_X3B_X7Dform_X7Bpadding_X2Dtop_X3A0px_X3Bpadding_X2Dbottom_X3A0px_X3B_X7D_X2Erunning_X2Dindicator_X7Bdisplay_X3Ainline_X2Dblock_X3Bwidth_X3A8px_X3Bheight_X3A8px_X3Bborder_X2Dradius_X3A50_X25_X3Bmargin_X2Dright_X3A8px_X3Bbackground_X3Avar_X28_X2D_X2Dc_btnsvhvr_X29_X3Banimation_X3Apulse_X201_X2E5s_X20infinite_X3B_X7D_X40keyframes_X20pulse_X7B0_X25_X7Bopacity_X3A1_X3B_X7D50_X25_X7Bopacity_X3A0_X2E5_X3B_X7D100_X25_X7Bopacity_X3A1_X3B_X7D_X7D_X2Estore_X2Dheader_X7Bdisplay_X3Aflex_X3Bjustify_X2Dcontent_X3Aspace_X2Dbetween_X3Balign_X2Ditems_X3Acenter_X3Bmargin_X2Dbottom_X3A10px_X3B_X7D_X2Estore_X2Dstats_X7Bfont_X2Dsize_X3A0_X2E9em_X3Bcolor_X3Avar_X28_X2D_X2Dc_in_X29_X3B_X7D_X2Eext_X2Dstore_X2Ditem_X7Bbackground_X3Avar_X28_X2D_X2Dc_bg_X29_X3Bborder_X2Dradius_X3A0_X2E3em_X3Bmargin_X2Dbottom_X3A5px_X3Bpadding_X3A4px_X3B_X7D_X2Eext_X2Dheader_X7Bdisplay_X3Aflex_X3Bjustify_X2Dcontent_X3Aspace_X2Dbetween_X3Balign_X2Ditems_X3Acenter_X3Bcursor_X3Apointer_X3Buser_X2Dselect_X3Anone_X3Bpadding_X3A5px_X3B_X7D_X2Eext_X2Dtitle_X7Bdisplay_X3Aflex_X3Balign_X2Ditems_X3Acenter_X3Bgap_X3A6px_X3Bflex_X3A1_X3Bpadding_X3A0_X3B_X7D_X2Eext_X2Dname_X7Bfont_X2Dweight_X3Abold_X3B_X7D_X2Eext_X2Dversion_X7Bfont_X2Dsize_X3A0_X2E8em_X3B_X7D_X2Eext_X2Darrow_X7Bcolor_X3Avar_X28_X2D_X2Dc_in_X29_X3Bfont_X2Dsize_X3A0_X2E8em_X3B_X7D_X2Eext_X2Dbadges_X7Bpadding_X3A0_X3B_X7D_X2Eext_X2Ddetails_X7Bwidth_X3Amin_X2Dcontent_X3Bmin_X2Dwidth_X3A100_X25_X3Bpadding_X3A0_X3Bdisplay_X3Anone_X3B_X7D_X2Eext_X2Ddesc_X7Bcolor_X3Avar_X28_X2D_X2Dc_in_X29_X3Bfont_X2Dsize_X3A0_X2E8em_X3Bline_X2Dheight_X3A1_X2E4_X3Bdisplay_X3Ablock_X3Bword_X2Dwrap_X3Abreak_X2Dword_X3Boverflow_X2Dwrap_X3Abreak_X2Dword_X3Bwhite_X2Dspace_X3Anormal_X3Bpadding_X3A0_X205px_X3B_X7D_X2Eext_X2Dactions_X7Bdisplay_X3Aflex_X3Bgap_X3A8px_X3Bpadding_X3A5px_X3B_X7D_X2Ebtn_X2Daction_X7Bpadding_X3A0_X2012px_X3Bline_X2Dheight_X3A1_X2E8em_X3Bfont_X2Dsize_X3A0_X2E9em_X3Bflex_X3A1_X3B_X7D_X2Einstalled_X2Dbadge_X7Bborder_X2Dcolor_X3Avar_X28_X2D_X2Dc_btnhvr_X29_X3Bpadding_X3A0px_X204px_X3Bborder_X2Dradius_X3A4px_X3Bfont_X2Dsize_X3A0_X2E7em_X3Bborder_X2Dwidth_X3A2px_X3Bborder_X2Dstyle_X3Asolid_X3Bmargin_X2Dright_X3A3px_X3B_X7D_X2Eupdate_X2Dbadge_X7Bbackground_X3Avar_X28_X2D_X2Dc_btnhvr_X29_X3Bpadding_X3A2px_X206px_X3Bborder_X2Dradius_X3A4px_X3Bfont_X2Dsize_X3A0_X2E7em_X3Bmargin_X2Dright_X3A3px_X3Banimation_X3Apulse_X202s_X20infinite_X3B_X7D_X40keyframes_X20pulse_X7B0_X25_X7Bopacity_X3A1_X3B_X7D50_X25_X7Bopacity_X3A0_X2E7_X3B_X7D100_X25_X7Bopacity_X3A1_X3B_X7D_X7D_X3C_X2Fstyle_X3E_X3Clegend_X3E_X3Cb_X20title_X3D_X27Running_X20extensions_X27_X3E_X26nbsp_X3BInstalled_X20extensions_X3C_X2Fb_X3E_X3C_X2Flegend_X3E),
  /* K112 */  be_nested_str(list_extensions_in_fs),
  /* K113 */  be_nested_str(_X3Chr_X20style_X3D_X27margin_X3A2px_X200_X200_X200_X3B_X27_X3E),
  /* K114 */  be_nested_str(get_by_index),
  /* K115 */  be_nested_str(_ext),
  /* K116 */  be_nested_str(_X20_X3Cspan_X20class_X3D_X27running_X2Dindicator_X27_X20title_X3D_X27Running_X27_X3E_X3C_X2Fspan_X3E),
  /* K117 */  be_nested_str(autorun),
  /* K118 */  be_nested_str(style_X3D_X27background_X3Avar_X28_X2D_X2Dc_btnsvhvr_X29_X3B_X27),
  /* K119 */  be_nested_str(style_X3D_X27background_X3Avar_X28_X2D_X2Dc_btnoff_X29_X3B_X27),
  /* K120 */  be_nested_str(_X3Cdiv_X20class_X3D_X27ext_X2Ditem_X27_X3E),
  /* K121 */  be_nested_str(_X3Cspan_X20title_X3D_X27path_X3A_X20_X25s_X27_X3E_X3Cb_X3E_X25s_X3C_X2Fb_X3E_X25s_X3C_X2Fspan_X3E_X3Cbr_X3E),
  /* K122 */  be_nested_str(_X3Csmall_X3E_X25s_X3C_X2Fsmall_X3E),
  /* K123 */  be_nested_str(version_string),
  /* K124 */  be_nested_str(_X3Cdiv_X20class_X3D_X27ext_X2Dcontrols_X27_X20style_X3D_X27padding_X2Dtop_X3A0px_X3Bpadding_X2Dbottom_X3A0px_X3B_X27_X3E),
  /* K125 */  be_nested_str(_X3Cform_X20action_X3D_X27_X2Fext_X27_X20method_X3D_X27post_X27_X20class_X3D_X27ext_X2Dcontrols_X27_X3E),
  /* K126 */  be_nested_str(_X3Cbutton_X20type_X3D_X27submit_X27_X20class_X3D_X27btn_X2Dsmall_X27_X20_X25s_X20name_X3D_X27_X25s_X25s_X27_X3E_X25s_X3C_X2Fbutton_X3E),
  /* K127 */  be_nested_str(Running),
  /* K128 */  be_nested_str(Stopped),
  /* K129 */  be_nested_str(_X3Cbutton_X20type_X3D_X27submit_X27_X20class_X3D_X27btn_X2Dsmall_X27_X20_X25s_X20name_X3D_X27_X25s_X25s_X27_X3EAuto_X2Drun_X3A_X20_X25s_X3C_X2Fbutton_X3E),
  /* K130 */  be_nested_str(ON),
  /* K131 */  be_nested_str(OFF),
  /* K132 */  be_nested_str(_X3Cbutton_X20type_X3D_X27submit_X27_X20class_X3D_X27btn_X2Dsmall_X20bred_X27_X20name_X3D_X27d_X25s_X27_X20onclick_X3D_X27return_X20confirm_X28_X22Confirm_X20deletion_X20of_X20_X25s_X2Etapp_X22_X29_X27_X3EUninstall_X3C_X2Fbutton_X3E),
  /* K133 */  be_nested_str(_X3C_X2Fform_X3E_X3C_X2Fdiv_X3E_X3C_X2Fdiv_X3E),
  /* K134 */  be_nested_str(_X3Cdiv_X3E_X3Csmall_X3E_X3Ci_X3ENo_X20installed_X20extension_X2E_X3C_X2Fi_X3E_X3C_X2Fsmall_X3E_X3C_X2Fp_X3E),
  /* K135 */  be_nested_str(_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
  /* K136 */  be_nested_str(_X3Cdiv_X20style_X3D_X27padding_X3A0px_X205px_X3Btext_X2Dalign_X3Acenter_X3B_X27_X3E_X3Ch3_X3E_X3Chr_X3EOnline_X20Store_X3Chr_X20style_X3D_X27margin_X2Dbottom_X3A0_X3B_X27_X3E_X3Cspan_X20id_X3D_X27inet_X27_X20style_X3D_X27font_X2Dsize_X3Asmall_X3Bfont_X2Dweight_X3Anormal_X3B_X27_X27_X3E_X26nbsp_X3B_X28This_X20feature_X20requires_X20an_X20internet_X20connection_X29_X3C_X2Fspan_X3E_X3C_X2Fh3_X3E_X3C_X2Fdiv_X3E),
  /* K137 */  be_nested_str(_X3Cb_X20id_X3D_X27store_X27_X3E_X5B_X20_X3Cspan_X20style_X3D_X27color_X3Avar_X28_X2D_X2Dc_btnsv_X29_X3B_X27_X3ELoading_X20from_X20Store_X2E_X2E_X2E_X3C_X2Fspan_X3E_X20_X5D_X3C_X2Fb_X3E),
  /* K138 */  be_nested_str(arch),
  /* K139 */  be_nested_str(0x_X2508X),
  /* K140 */  be_nested_str(cmd),
  /* K141 */  be_nested_str(OtaUrl),
  /* K142 */  be_nested_str(_X25s_X2F_X2F_X25s_X2Fextensions_X2F),
  /* K143 */  be_nested_str(_X25s_X25s_X3Fa_X3D_X25s_X26v_X3D_X25s),
  /* K144 */  be_nested_str(EXT_REPO_MANIFEST),
  /* K145 */  be_nested_str(EXT_X3A_X20fetching_X20extensions_X20manifest_X20_X27_X25s_X27),
  /* K146 */  be_nested_str(get_size),
  /* K147 */  be_nested_str(deinit),
  /* K148 */  be_nested_str(EXT_X3A_X20error_X20fetching_X20manifest_X20_X25s),
  /* K149 */  be_nested_str(Error_X20fetching_X20manifest_X20code_X3D_X25s),
  /* K150 */  be_nested_str(webclient_error),
  /* K151 */  be_nested_str(get_string),
  /* K152 */  be_nested_str(rename),
  /* K153 */  be_nested_str(unload_extension),
  /* K154 */  be_nested_str(content_open),
  /* K155 */  be_nested_str(text_X2Fhtml),
  /* K156 */  be_nested_str(load_manifest),
  /* K157 */  be_nested_str(_X3Cb_X20id_X3D_X27store_X27_X3E_X5B_X20_X3Cspan_X20style_X3D_X27color_X3Avar_X28_X2D_X2Dc_btnrst_X29_X3B_X27_X3EError_X20loading_X20manifest_X2E_X3C_X2Fspan_X3E_X20_X5D_X3C_X2Fb_X3E),
  /* K158 */  be_nested_str(_X3Cp_X3E_X3Csmall_X3E_X25s_X3C_X2Fsmall_X3E_X3C_X2Fp_X3E),
  /* K159 */  be_nested_str(content_close),
  /* K160 */  be_nested_str(count),
  /* K161 */  be_nested_str(_X22name_X22_X3A),
  /* K162 */  be_nested_str(_X3Cfieldset_X20id_X3D_X27store_X27_X3E),
  /* K163 */  be_nested_str(_X3Cdiv_X20class_X3D_X27store_X2Dheader_X27_X3E_X3Cspan_X3EBrowse_X20Extensions_X3C_X2Fspan_X3E_X3Cspan_X20class_X3D_X27store_X2Dstats_X27_X3E_X25s_X20available_X3C_X2Fspan_X3E_X3C_X2Fdiv_X3E),
  /* K164 */  be_nested_str(_X3Cinput_X20type_X3D_X27text_X27_X20placeholder_X3D_X27Search_X20extensions_X2E_X2E_X2E_X27_X20onkeyup_X3D_X27filterExtensions_X28this_X2Evalue_X29_X27_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
  /* K165 */  be_nested_str(_X0A),
  /* K166 */  be_nested_str(manifest_decode),
  /* K167 */  be_nested_str(replace),
  /* K168 */  be_nested_str(_X5Cn),
  /* K169 */  be_nested_str(_X3Cbr_X3E),
  /* K170 */  be_nested_str(_X3Cdiv_X20class_X3D_X27ext_X2Dstore_X2Ditem_X27_X3E_X3Cdiv_X20class_X3D_X27ext_X2Dheader_X27_X20onclick_X3D_X27toggleDesc_X28_X22_X25s_X22_X29_X27_X3E_X3Cdiv_X20class_X3D_X27ext_X2Dtitle_X27_X3E_X3Cspan_X20class_X3D_X27ext_X2Dname_X27_X3E_X25s_X3C_X2Fspan_X3E_X3Cspan_X20class_X3D_X27ext_X2Dversion_X27_X3E_X3Csmall_X3E_X25s_X3C_X2Fsmall_X3E_X3C_X2Fspan_X3E_X3C_X2Fdiv_X3E),
  /* K171 */  be_nested_str(_X3Cdiv_X20class_X3D_X27ext_X2Dbadges_X27_X3E_X3Cspan_X20class_X3D_X27update_X2Dbadge_X27_X3EUpgrade_X3C_X2Fspan_X3E_X3C_X2Fdiv_X3E),
  /* K172 */  be_nested_str(_X3Cdiv_X20class_X3D_X27ext_X2Dbadges_X27_X3E_X3Cspan_X20class_X3D_X27installed_X2Dbadge_X27_X3EInstalled_X3C_X2Fspan_X3E_X3C_X2Fdiv_X3E),
  /* K173 */  be_nested_str(_X3Cspan_X20id_X3D_X27arrow_X2D_X25s_X27_X20class_X3D_X27ext_X2Darrow_X27_X3E_XE2_X96_XB6_X3C_X2Fspan_X3E_X3C_X2Fdiv_X3E_X3Cdiv_X20id_X3D_X27desc_X2D_X25s_X27_X20class_X3D_X27ext_X2Ddetails_X27_X3E_X3Cdiv_X20class_X3D_X27ext_X2Ddesc_X27_X3E_X25s),
  /* K174 */  be_nested_str(_X3Cbr_X3E_X25s_X20_XE2_X86_X92_X20_X25s),
  /* K175 */  be_nested_str(_X3C_X2Fdiv_X3E_X3Cform_X20action_X3D_X27_X2Fext_X27_X20method_X3D_X27post_X27_X20class_X3D_X27ext_X2Dactions_X27_X3E_X3Cdiv_X20style_X3D_X27width_X3A30_X25_X27_X3E_X3C_X2Fdiv_X3E),
  /* K176 */  be_nested_str(_X3Cbutton_X20type_X3D_X27submit_X27_X20class_X3D_X27btn_X2Daction_X27_X20name_X3D_X27u_X25s_X27_X20onclick_X3D_X27return_X20confirm_X28_X22Confirm_X20upgrade_X20of_X20_X25s_X22_X29_X27_X3EUpgrade_X3C_X2Fbutton_X3E),
  /* K177 */  be_nested_str(_X3Cbutton_X20type_X3D_X27submit_X27_X20class_X3D_X27btn_X2Daction_X27_X20style_X3D_X27visibility_X3Ahidden_X3B_X27_X3E_X3C_X2Fbutton_X3E),
  /* K178 */  be_nested_str(_X3Cbutton_X20type_X3D_X27submit_X27_X20class_X3D_X27btn_X2Daction_X20bred_X27_X20name_X3D_X27d_X25s_X27_X20onclick_X3D_X27return_X20confirm_X28_X22Confirm_X20deletion_X20of_X20_X25s_X22_X29_X27_X3EUninstall_X3C_X2Fbutton_X3E),
  /* K179 */  be_nested_str_long(_X3Cbutton_X20type_X3D_X27submit_X27_X20class_X3D_X27btn_X2Daction_X27_X20name_X3D_X27i_X25s_X27_X20onclick_X3D_X27return_X20confirm_X28_X22Confirm_X20installation_X20of_X20_X25s_X22_X29_X27_X3EInstall_X3C_X2Fbutton_X3E_X3Cbutton_X20type_X3D_X27submit_X27_X20class_X3D_X27btn_X2Daction_X20bgrn_X27_X20name_X3D_X27I_X25s_X27_X20onclick_X3D_X27return_X20confirm_X28_X22Confirm_X20installation_X20of_X20_X25s_X22_X29_X27_X3EInstall_X2BRun_X3C_X2Fbutton_X3E),
  /* K180 */  be_nested_str(_X3Cp_X3E_X3C_X2Fp_X3E_X3Chr_X20style_X3D_X27margin_X3A2px_X200_X200_X200_X3B_X27_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
  /* K181 */  be_nested_str(_X3Cform_X20action_X3D_X27_X2Fext_X27_X20method_X3D_X27post_X27_X3E_X3Cinput_X20type_X3D_X27text_X27_X20id_X3D_X27x_X27_X20name_X3D_X27x_X27_X20placeholder_X3D_X27_X25s_X27_X3E_X3C_X2Fform_X3E),
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
** Solidified function: delete_ext
********************************************************************/
be_local_closure(class_Extension_manager_delete_ext,   /* name */
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
    &be_const_str_delete_ext,
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x58040006,  //  0000  LDCONST	R1	K6
      0x8C080307,  //  0001  GETMET	R2	R1	K7
      0x5C100000,  //  0002  MOVE	R4	R0
      0x7C080400,  //  0003  CALL	R2	2
      0x5C000400,  //  0004  MOVE	R0	R2
      0x8C080308,  //  0005  GETMET	R2	R1	K8
      0x7C080200,  //  0006  CALL	R2	1
      0x8C080509,  //  0007  GETMET	R2	R2	K9
      0x5C100000,  //  0008  MOVE	R4	R0
      0x7C080400,  //  0009  CALL	R2	2
      0x4C0C0000,  //  000A  LDNIL	R3
      0x200C0403,  //  000B  NE	R3	R2	R3
      0x780E0009,  //  000C  JMPF	R3	#0017
      0xA40E1400,  //  000D  IMPORT	R3	K10
      0x8C10030B,  //  000E  GETMET	R4	R1	K11
      0x5C180000,  //  000F  MOVE	R6	R0
      0x501C0000,  //  0010  LDBOOL	R7	0	0
      0x7C100600,  //  0011  CALL	R4	3
      0x8C10070C,  //  0012  GETMET	R4	R3	K12
      0x5C180400,  //  0013  MOVE	R6	R2
      0x7C100400,  //  0014  CALL	R4	2
      0x80040800,  //  0015  RET	1	R4
      0x70020001,  //  0016  JMP		#0019
      0x500C0000,  //  0017  LDBOOL	R3	0	0
      0x80040600,  //  0018  RET	1	R3
      0x80000000,  //  0019  RET	0
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
      0x8C08030D,  //  0001  GETMET	R2	R1	K13
      0x5810000E,  //  0002  LDCONST	R4	K14
      0x84140000,  //  0003  CLOSURE	R5	P0
      0x8818030F,  //  0004  GETMBR	R6	R1	K15
      0x7C080800,  //  0005  CALL	R2	4
      0x8C08030D,  //  0006  GETMET	R2	R1	K13
      0x5810000E,  //  0007  LDCONST	R4	K14
      0x84140001,  //  0008  CLOSURE	R5	P1
      0x88180310,  //  0009  GETMBR	R6	R1	K16
      0x7C080800,  //  000A  CALL	R2	4
      0xA0000000,  //  000B  CLOSE	R0
      0x80000000,  //  000C  RET	0
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
      0x58040006,  //  0000  LDCONST	R1	K6
      0xA40A1400,  //  0001  IMPORT	R2	K10
      0x8C0C0511,  //  0002  GETMET	R3	R2	K17
      0x5C140000,  //  0003  MOVE	R5	R0
      0x7C0C0400,  //  0004  CALL	R3	2
      0x780E0008,  //  0005  JMPF	R3	#000F
      0x8C0C0512,  //  0006  GETMET	R3	R2	K18
      0x5C140000,  //  0007  MOVE	R5	R0
      0x7C0C0400,  //  0008  CALL	R3	2
      0x780E0001,  //  0009  JMPF	R3	#000C
      0x80000600,  //  000A  RET	0
      0x70020002,  //  000B  JMP		#000F
      0x8C0C050C,  //  000C  GETMET	R3	R2	K12
      0x5C140000,  //  000D  MOVE	R5	R0
      0x7C0C0400,  //  000E  CALL	R3	2
      0x8C0C0513,  //  000F  GETMET	R3	R2	K19
      0x5C140000,  //  0010  MOVE	R5	R0
      0x7C0C0400,  //  0011  CALL	R3	2
      0x8C0C0511,  //  0012  GETMET	R3	R2	K17
      0x5C140000,  //  0013  MOVE	R5	R0
      0x7C0C0400,  //  0014  CALL	R3	2
      0x780E0003,  //  0015  JMPF	R3	#001A
      0x8C0C0512,  //  0016  GETMET	R3	R2	K18
      0x5C140000,  //  0017  MOVE	R5	R0
      0x7C0C0400,  //  0018  CALL	R3	2
      0x740E0004,  //  0019  JMPT	R3	#001F
      0x600C0018,  //  001A  GETGBL	R3	G24
      0x58100014,  //  001B  LDCONST	R4	K20
      0x5C140000,  //  001C  MOVE	R5	R0
      0x7C0C0400,  //  001D  CALL	R3	2
      0xB0062A03,  //  001E  RAISE	1	K21	R3
      0x80000000,  //  001F  RET	0
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
      0x58040006,  //  0000  LDCONST	R1	K6
      0xA40A2C00,  //  0001  IMPORT	R2	K22
      0x580C0017,  //  0002  LDCONST	R3	K23
      0x6010000C,  //  0003  GETGBL	R4	G12
      0x5C140000,  //  0004  MOVE	R5	R0
      0x7C100200,  //  0005  CALL	R4	1
      0x04100918,  //  0006  SUB	R4	R4	K24
      0x94140004,  //  0007  GETIDX	R5	R0	R4
      0x1C140B19,  //  0008  EQ	R5	R5	K25
      0x74160002,  //  0009  JMPT	R5	#000D
      0x94140004,  //  000A  GETIDX	R5	R0	R4
      0x1C140B1A,  //  000B  EQ	R5	R5	K26
      0x78160000,  //  000C  JMPF	R5	#000E
      0x04100918,  //  000D  SUB	R4	R4	K24
      0x8C140509,  //  000E  GETMET	R5	R2	K9
      0x5C1C0000,  //  000F  MOVE	R7	R0
      0x5820001B,  //  0010  LDCONST	R8	K27
      0x00240918,  //  0011  ADD	R9	R4	K24
      0x6028000C,  //  0012  GETGBL	R10	G12
      0x582C001B,  //  0013  LDCONST	R11	K27
      0x7C280200,  //  0014  CALL	R10	1
      0x0424120A,  //  0015  SUB	R9	R9	R10
      0x7C140800,  //  0016  CALL	R5	4
      0x28140B17,  //  0017  GE	R5	R5	K23
      0x78160003,  //  0018  JMPF	R5	#001D
      0x6014000C,  //  0019  GETGBL	R5	G12
      0x5818001B,  //  001A  LDCONST	R6	K27
      0x7C140200,  //  001B  CALL	R5	1
      0x04100805,  //  001C  SUB	R4	R4	R5
      0x4C140000,  //  001D  LDNIL	R5
      0x8C180509,  //  001E  GETMET	R6	R2	K9
      0x5C200000,  //  001F  MOVE	R8	R0
      0x5824001C,  //  0020  LDCONST	R9	K28
      0x5C280600,  //  0021  MOVE	R10	R3
      0x7C180800,  //  0022  CALL	R6	4
      0x5C140C00,  //  0023  MOVE	R5	R6
      0x28180D17,  //  0024  GE	R6	R6	K23
      0x781A0002,  //  0025  JMPF	R6	#0029
      0x00180B18,  //  0026  ADD	R6	R5	K24
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
      0x58000006,  //  0000  LDCONST	R0	K6
      0xA4061400,  //  0001  IMPORT	R1	K10
      0xA40A2C00,  //  0002  IMPORT	R2	K22
      0x600C0012,  //  0003  GETGBL	R3	G18
      0x7C0C0000,  //  0004  CALL	R3	0
      0x60100010,  //  0005  GETGBL	R4	G16
      0x8C14031D,  //  0006  GETMET	R5	R1	K29
      0x881C011E,  //  0007  GETMBR	R7	R0	K30
      0x7C140400,  //  0008  CALL	R5	2
      0x7C100200,  //  0009  CALL	R4	1
      0xA8020010,  //  000A  EXBLK	0	#001C
      0x5C140800,  //  000B  MOVE	R5	R4
      0x7C140000,  //  000C  CALL	R5	0
      0x8C18051F,  //  000D  GETMET	R6	R2	K31
      0x5C200A00,  //  000E  MOVE	R8	R5
      0x5824001B,  //  000F  LDCONST	R9	K27
      0x7C180600,  //  0010  CALL	R6	3
      0x741A0004,  //  0011  JMPT	R6	#0017
      0x8C18051F,  //  0012  GETMET	R6	R2	K31
      0x5C200A00,  //  0013  MOVE	R8	R5
      0x58240020,  //  0014  LDCONST	R9	K32
      0x7C180600,  //  0015  CALL	R6	3
      0x781A0003,  //  0016  JMPF	R6	#001B
      0x8C180721,  //  0017  GETMET	R6	R3	K33
      0x8820011E,  //  0018  GETMBR	R8	R0	K30
      0x00201005,  //  0019  ADD	R8	R8	R5
      0x7C180400,  //  001A  CALL	R6	2
      0x7001FFEE,  //  001B  JMP		#000B
      0x58100022,  //  001C  LDCONST	R4	K34
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
    ( &(const binstruction[ 6]) {  /* code */
      0x90024724,  //  0000  SETMBR	R0	K35	K36
      0xB8064A00,  //  0001  GETNGBL	R1	K37
      0x8C040326,  //  0002  GETMET	R1	R1	K38
      0x5C0C0000,  //  0003  MOVE	R3	R0
      0x7C040400,  //  0004  CALL	R1	2
      0x80000000,  //  0005  RET	0
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
      0x58040006,  //  0000  LDCONST	R1	K6
      0xA40A4E00,  //  0001  IMPORT	R2	K39
      0x8C0C0528,  //  0002  GETMET	R3	R2	K40
      0x5C140000,  //  0003  MOVE	R5	R0
      0x7C0C0400,  //  0004  CALL	R3	2
      0x4C100000,  //  0005  LDNIL	R4
      0x1C100604,  //  0006  EQ	R4	R3	R4
      0x78120008,  //  0007  JMPF	R4	#0011
      0xB8125200,  //  0008  GETNGBL	R4	K41
      0x60140018,  //  0009  GETGBL	R5	G24
      0x5818002A,  //  000A  LDCONST	R6	K42
      0x5C1C0000,  //  000B  MOVE	R7	R0
      0x7C140400,  //  000C  CALL	R5	2
      0x5818002B,  //  000D  LDCONST	R6	K43
      0x7C100400,  //  000E  CALL	R4	2
      0x4C100000,  //  000F  LDNIL	R4
      0x80040800,  //  0010  RET	1	R4
      0x8C10072C,  //  0011  GETMET	R4	R3	K44
      0x5818002D,  //  0012  LDCONST	R6	K45
      0x7C100400,  //  0013  CALL	R4	2
      0x78120007,  //  0014  JMPF	R4	#001D
      0x8C10072C,  //  0015  GETMET	R4	R3	K44
      0x5818002E,  //  0016  LDCONST	R6	K46
      0x7C100400,  //  0017  CALL	R4	2
      0x78120003,  //  0018  JMPF	R4	#001D
      0x8C10072C,  //  0019  GETMET	R4	R3	K44
      0x5818002F,  //  001A  LDCONST	R6	K47
      0x7C100400,  //  001B  CALL	R4	2
      0x74120007,  //  001C  JMPT	R4	#0025
      0xB8125200,  //  001D  GETNGBL	R4	K41
      0x60140018,  //  001E  GETGBL	R5	G24
      0x58180030,  //  001F  LDCONST	R6	K48
      0x5C1C0600,  //  0020  MOVE	R7	R3
      0x7C140400,  //  0021  CALL	R5	2
      0x7C100200,  //  0022  CALL	R4	1
      0x4C100000,  //  0023  LDNIL	R4
      0x80040800,  //  0024  RET	1	R4
      0x60100013,  //  0025  GETGBL	R4	G19
      0x7C100000,  //  0026  CALL	R4	0
      0x9414072D,  //  0027  GETIDX	R5	R3	K45
      0x98125A05,  //  0028  SETIDX	R4	K45	R5
      0x9414072E,  //  0029  GETIDX	R5	R3	K46
      0x98125C05,  //  002A  SETIDX	R4	K46	R5
      0x60140009,  //  002B  GETGBL	R5	G9
      0x9418072F,  //  002C  GETIDX	R6	R3	K47
      0x7C140200,  //  002D  CALL	R5	1
      0x98125E05,  //  002E  SETIDX	R4	K47	R5
      0x8C140709,  //  002F  GETMET	R5	R3	K9
      0x581C0031,  //  0030  LDCONST	R7	K49
      0x58200032,  //  0031  LDCONST	R8	K50
      0x7C140600,  //  0032  CALL	R5	3
      0x98126205,  //  0033  SETIDX	R4	K49	R5
      0x8C140709,  //  0034  GETMET	R5	R3	K9
      0x581C0033,  //  0035  LDCONST	R7	K51
      0x58200024,  //  0036  LDCONST	R8	K36
      0x7C140600,  //  0037  CALL	R5	3
      0x98126605,  //  0038  SETIDX	R4	K51	R5
      0x80040800,  //  0039  RET	1	R4
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
      0x8C080334,  //  0001  GETMET	R2	R1	K52
      0x58100035,  //  0002  LDCONST	R4	K53
      0x7C080400,  //  0003  CALL	R2	2
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
      0x58000006,  //  0000  LDCONST	R0	K6
      0xA4062C00,  //  0001  IMPORT	R1	K22
      0xB80A6C00,  //  0002  GETNGBL	R2	K54
      0x7C080000,  //  0003  CALL	R2	0
      0x600C0010,  //  0004  GETGBL	R3	G16
      0x8C100137,  //  0005  GETMET	R4	R0	K55
      0x7C100200,  //  0006  CALL	R4	1
      0x7C0C0200,  //  0007  CALL	R3	1
      0xA8020016,  //  0008  EXBLK	0	#0020
      0x5C100600,  //  0009  MOVE	R4	R3
      0x7C100000,  //  000A  CALL	R4	0
      0xB8164A00,  //  000B  GETNGBL	R5	K37
      0x8C140B38,  //  000C  GETMET	R5	R5	K56
      0x5C1C0800,  //  000D  MOVE	R7	R4
      0x7C140400,  //  000E  CALL	R5	2
      0x4C180000,  //  000F  LDNIL	R6
      0x20180A06,  //  0010  NE	R6	R5	R6
      0x781A0005,  //  0011  JMPF	R6	#0018
      0x8C180B09,  //  0012  GETMET	R6	R5	K9
      0x5820002D,  //  0013  LDCONST	R8	K45
      0x7C180400,  //  0014  CALL	R6	2
      0x781A0000,  //  0015  JMPF	R6	#0017
      0x98080C04,  //  0016  SETIDX	R2	R6	R4
      0x70020006,  //  0017  JMP		#001F
      0xB81A5200,  //  0018  GETNGBL	R6	K41
      0x601C0018,  //  0019  GETGBL	R7	G24
      0x58200039,  //  001A  LDCONST	R8	K57
      0x5C240800,  //  001B  MOVE	R9	R4
      0x7C1C0400,  //  001C  CALL	R7	2
      0x5820002B,  //  001D  LDCONST	R8	K43
      0x7C180400,  //  001E  CALL	R6	2
      0x7001FFE8,  //  001F  JMP		#0009
      0x580C0022,  //  0020  LDCONST	R3	K34
      0xAC0C0200,  //  0021  CATCH	R3	1	0
      0xB0080000,  //  0022  RAISE	2	R0	R0
      0x80040400,  //  0023  RET	1	R2
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
      0x58040006,  //  0000  LDCONST	R1	K6
      0x60080018,  //  0001  GETGBL	R2	G24
      0x580C003A,  //  0002  LDCONST	R3	K58
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
    ( &(const binstruction[95]) {  /* code */
      0xA40A2C00,  //  0000  IMPORT	R2	K22
      0xA40E1400,  //  0001  IMPORT	R3	K10
      0x8C100107,  //  0002  GETMET	R4	R0	K7
      0x5C180200,  //  0003  MOVE	R6	R1
      0x7C100400,  //  0004  CALL	R4	2
      0x0010091B,  //  0005  ADD	R4	R4	K27
      0x5C040800,  //  0006  MOVE	R1	R4
      0x60100018,  //  0007  GETGBL	R4	G24
      0x5814003B,  //  0008  LDCONST	R5	K59
      0x88180123,  //  0009  GETMBR	R6	R0	K35
      0x881C013C,  //  000A  GETMBR	R7	R0	K60
      0x5C200200,  //  000B  MOVE	R8	R1
      0x7C100800,  //  000C  CALL	R4	4
      0xB8165200,  //  000D  GETNGBL	R5	K41
      0x60180018,  //  000E  GETGBL	R6	G24
      0x581C003D,  //  000F  LDCONST	R7	K61
      0x5C200800,  //  0010  MOVE	R8	R4
      0x7C180400,  //  0011  CALL	R6	2
      0x581C002B,  //  0012  LDCONST	R7	K43
      0x7C140400,  //  0013  CALL	R5	2
      0xA802003A,  //  0014  EXBLK	0	#0050
      0x8C14013E,  //  0015  GETMET	R5	R0	K62
      0x881C011E,  //  0016  GETMBR	R7	R0	K30
      0x7C140400,  //  0017  CALL	R5	2
      0x60140018,  //  0018  GETGBL	R5	G24
      0x5818003F,  //  0019  LDCONST	R6	K63
      0x881C011E,  //  001A  GETMBR	R7	R0	K30
      0x5C200200,  //  001B  MOVE	R8	R1
      0x7C140600,  //  001C  CALL	R5	3
      0xB81A8000,  //  001D  GETNGBL	R6	K64
      0x7C180000,  //  001E  CALL	R6	0
      0x8C1C0D41,  //  001F  GETMET	R7	R6	K65
      0x5C240800,  //  0020  MOVE	R9	R4
      0x7C1C0400,  //  0021  CALL	R7	2
      0x8C1C0D42,  //  0022  GETMET	R7	R6	K66
      0x7C1C0200,  //  0023  CALL	R7	1
      0x542200C7,  //  0024  LDINT	R8	200
      0x20200E08,  //  0025  NE	R8	R7	R8
      0x78220009,  //  0026  JMPF	R8	#0031
      0xB8225200,  //  0027  GETNGBL	R8	K41
      0x60240018,  //  0028  GETGBL	R9	G24
      0x58280043,  //  0029  LDCONST	R10	K67
      0x5C2C0E00,  //  002A  MOVE	R11	R7
      0x7C240400,  //  002B  CALL	R9	2
      0x58280044,  //  002C  LDCONST	R10	K68
      0x7C200400,  //  002D  CALL	R8	2
      0x50200000,  //  002E  LDBOOL	R8	0	0
      0xA8040001,  //  002F  EXBLK	1	1
      0x80041000,  //  0030  RET	1	R8
      0x8C200D45,  //  0031  GETMET	R8	R6	K69
      0x5C280A00,  //  0032  MOVE	R10	R5
      0x7C200400,  //  0033  CALL	R8	2
      0x8C240D46,  //  0034  GETMET	R9	R6	K70
      0x7C240200,  //  0035  CALL	R9	1
      0x24241117,  //  0036  GT	R9	R8	K23
      0x7826000F,  //  0037  JMPF	R9	#0048
      0x8C240711,  //  0038  GETMET	R9	R3	K17
      0x5C2C0A00,  //  0039  MOVE	R11	R5
      0x7C240400,  //  003A  CALL	R9	2
      0x7826000B,  //  003B  JMPF	R9	#0048
      0xB8265200,  //  003C  GETNGBL	R9	K41
      0x60280018,  //  003D  GETGBL	R10	G24
      0x582C0047,  //  003E  LDCONST	R11	K71
      0x5C300A00,  //  003F  MOVE	R12	R5
      0x5C341000,  //  0040  MOVE	R13	R8
      0x7C280600,  //  0041  CALL	R10	3
      0x582C002B,  //  0042  LDCONST	R11	K43
      0x7C240400,  //  0043  CALL	R9	2
      0x50240200,  //  0044  LDBOOL	R9	1	0
      0xA8040001,  //  0045  EXBLK	1	1
      0x80041200,  //  0046  RET	1	R9
      0x70020005,  //  0047  JMP		#004E
      0x60240018,  //  0048  GETGBL	R9	G24
      0x58280048,  //  0049  LDCONST	R10	K72
      0x5C2C0A00,  //  004A  MOVE	R11	R5
      0x5C301000,  //  004B  MOVE	R12	R8
      0x7C240600,  //  004C  CALL	R9	3
      0xB0062A09,  //  004D  RAISE	1	K21	R9
      0xA8040001,  //  004E  EXBLK	1	1
      0x7002000D,  //  004F  JMP		#005E
      0xAC140002,  //  0050  CATCH	R5	0	2
      0x7002000A,  //  0051  JMP		#005D
      0xB81E5200,  //  0052  GETNGBL	R7	K41
      0x60200018,  //  0053  GETGBL	R8	G24
      0x58240049,  //  0054  LDCONST	R9	K73
      0x5C280A00,  //  0055  MOVE	R10	R5
      0x5C2C0C00,  //  0056  MOVE	R11	R6
      0x7C200600,  //  0057  CALL	R8	3
      0x58240044,  //  0058  LDCONST	R9	K68
      0x7C1C0400,  //  0059  CALL	R7	2
      0x501C0000,  //  005A  LDBOOL	R7	0	0
      0x80040E00,  //  005B  RET	1	R7
      0x70020000,  //  005C  JMP		#005E
      0xB0080000,  //  005D  RAISE	2	R0	R0
      0x80000000,  //  005E  RET	0
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
    ( &(const binstruction[177]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A1400,  //  0001  IMPORT	R2	K10
      0xA40E2C00,  //  0002  IMPORT	R3	K22
      0x8C100301,  //  0003  GETMET	R4	R1	K1
      0x7C100200,  //  0004  CALL	R4	1
      0x74120001,  //  0005  JMPT	R4	#0008
      0x4C100000,  //  0006  LDNIL	R4
      0x80040800,  //  0007  RET	1	R4
      0xA8020085,  //  0008  EXBLK	0	#008F
      0x8C10034A,  //  0009  GETMET	R4	R1	K74
      0x58180017,  //  000A  LDCONST	R6	K23
      0x7C100400,  //  000B  CALL	R4	2
      0x94140917,  //  000C  GETIDX	R5	R4	K23
      0x401A314B,  //  000D  CONNECT	R6	K24	K75
      0x94180806,  //  000E  GETIDX	R6	R4	R6
      0x1C1C0B4C,  //  000F  EQ	R7	R5	K76
      0x741E0001,  //  0010  JMPT	R7	#0013
      0x1C1C0B4D,  //  0011  EQ	R7	R5	K77
      0x781E0004,  //  0012  JMPF	R7	#0018
      0x8C1C010B,  //  0013  GETMET	R7	R0	K11
      0x5C240C00,  //  0014  MOVE	R9	R6
      0x1C280B4C,  //  0015  EQ	R10	R5	K76
      0x7C1C0600,  //  0016  CALL	R7	3
      0x7002006F,  //  0017  JMP		#0088
      0x1C1C0B4E,  //  0018  EQ	R7	R5	K78
      0x741E0001,  //  0019  JMPT	R7	#001C
      0x1C1C0B4F,  //  001A  EQ	R7	R5	K79
      0x781E0004,  //  001B  JMPF	R7	#0021
      0x8C1C0150,  //  001C  GETMET	R7	R0	K80
      0x5C240C00,  //  001D  MOVE	R9	R6
      0x1C280B4F,  //  001E  EQ	R10	R5	K79
      0x7C1C0600,  //  001F  CALL	R7	3
      0x70020066,  //  0020  JMP		#0088
      0x1C1C0B51,  //  0021  EQ	R7	R5	K81
      0x781E0003,  //  0022  JMPF	R7	#0027
      0x8C1C0152,  //  0023  GETMET	R7	R0	K82
      0x5C240C00,  //  0024  MOVE	R9	R6
      0x7C1C0400,  //  0025  CALL	R7	2
      0x70020060,  //  0026  JMP		#0088
      0x1C1C0B53,  //  0027  EQ	R7	R5	K83
      0x781E0009,  //  0028  JMPF	R7	#0033
      0x8C1C010B,  //  0029  GETMET	R7	R0	K11
      0x5C240C00,  //  002A  MOVE	R9	R6
      0x50280000,  //  002B  LDBOOL	R10	0	0
      0x7C1C0600,  //  002C  CALL	R7	3
      0x8C1C0154,  //  002D  GETMET	R7	R0	K84
      0x8C240107,  //  002E  GETMET	R9	R0	K7
      0x5C2C0C00,  //  002F  MOVE	R11	R6
      0x7C240400,  //  0030  CALL	R9	2
      0x7C1C0400,  //  0031  CALL	R7	2
      0x70020054,  //  0032  JMP		#0088
      0x1C1C0B55,  //  0033  EQ	R7	R5	K85
      0x741E0001,  //  0034  JMPT	R7	#0037
      0x1C1C0B56,  //  0035  EQ	R7	R5	K86
      0x781E0011,  //  0036  JMPF	R7	#0049
      0x8C1C0154,  //  0037  GETMET	R7	R0	K84
      0x8C240107,  //  0038  GETMET	R9	R0	K7
      0x5C2C0C00,  //  0039  MOVE	R11	R6
      0x7C240400,  //  003A  CALL	R9	2
      0x7C1C0400,  //  003B  CALL	R7	2
      0x781E000A,  //  003C  JMPF	R7	#0048
      0x1C200B56,  //  003D  EQ	R8	R5	K86
      0x78220004,  //  003E  JMPF	R8	#0044
      0x8C20010B,  //  003F  GETMET	R8	R0	K11
      0x5C280C00,  //  0040  MOVE	R10	R6
      0x502C0200,  //  0041  LDBOOL	R11	1	0
      0x7C200600,  //  0042  CALL	R8	3
      0x70020003,  //  0043  JMP		#0048
      0x8C200150,  //  0044  GETMET	R8	R0	K80
      0x5C280C00,  //  0045  MOVE	R10	R6
      0x502C0000,  //  0046  LDBOOL	R11	0	0
      0x7C200600,  //  0047  CALL	R8	3
      0x7002003E,  //  0048  JMP		#0088
      0x1C1C0B57,  //  0049  EQ	R7	R5	K87
      0x781E0035,  //  004A  JMPF	R7	#0081
      0x8C1C0358,  //  004B  GETMET	R7	R1	K88
      0x58240017,  //  004C  LDCONST	R9	K23
      0x7C1C0400,  //  004D  CALL	R7	2
      0x90024724,  //  004E  SETMBR	R0	K35	K36
      0x6020000C,  //  004F  GETGBL	R8	G12
      0x5C240E00,  //  0050  MOVE	R9	R7
      0x7C200200,  //  0051  CALL	R8	1
      0x24201117,  //  0052  GT	R8	R8	K23
      0x7822002B,  //  0053  JMPF	R8	#0080
      0x1C200F59,  //  0054  EQ	R8	R7	K89
      0x78220000,  //  0055  JMPF	R8	#0057
      0x70020028,  //  0056  JMP		#0080
      0x1C200F5A,  //  0057  EQ	R8	R7	K90
      0x78220002,  //  0058  JMPF	R8	#005C
      0x8820015B,  //  0059  GETMBR	R8	R0	K91
      0x90024608,  //  005A  SETMBR	R0	K35	R8
      0x70020023,  //  005B  JMP		#0080
      0x8C20075C,  //  005C  GETMET	R8	R3	K92
      0x5C280E00,  //  005D  MOVE	R10	R7
      0x582C001C,  //  005E  LDCONST	R11	K28
      0x7C200600,  //  005F  CALL	R8	3
      0x8C24115D,  //  0060  GETMET	R9	R8	K93
      0x7C240200,  //  0061  CALL	R9	1
      0x24241344,  //  0062  GT	R9	R9	K68
      0x7826001B,  //  0063  JMPF	R9	#0080
      0x94241117,  //  0064  GETIDX	R9	R8	K23
      0x1C24135E,  //  0065  EQ	R9	R9	K94
      0x74260003,  //  0066  JMPT	R9	#006B
      0x94241117,  //  0067  GETIDX	R9	R8	K23
      0x1C24135F,  //  0068  EQ	R9	R9	K95
      0x74260000,  //  0069  JMPT	R9	#006B
      0x50240001,  //  006A  LDBOOL	R9	0	1
      0x50240200,  //  006B  LDBOOL	R9	1	0
      0x5429FFFE,  //  006C  LDINT	R10	-1
      0x94280E0A,  //  006D  GETIDX	R10	R7	R10
      0x1C28151C,  //  006E  EQ	R10	R10	K28
      0x782A0001,  //  006F  JMPF	R10	#0072
      0x542DFFFD,  //  0070  LDINT	R11	-2
      0x70020000,  //  0071  JMP		#0073
      0x542DFFFE,  //  0072  LDINT	R11	-1
      0x942C100B,  //  0073  GETIDX	R11	R8	R11
      0x1C2C1760,  //  0074  EQ	R11	R11	K96
      0x78260009,  //  0075  JMPF	R9	#0080
      0x94301118,  //  0076  GETIDX	R12	R8	K24
      0x1C301924,  //  0077  EQ	R12	R12	K36
      0x78320006,  //  0078  JMPF	R12	#0080
      0x782E0005,  //  0079  JMPF	R11	#0080
      0x90024607,  //  007A  SETMBR	R0	K35	R7
      0x5C301400,  //  007B  MOVE	R12	R10
      0x74320002,  //  007C  JMPT	R12	#0080
      0x88300123,  //  007D  GETMBR	R12	R0	K35
      0x0030191C,  //  007E  ADD	R12	R12	K28
      0x9002460C,  //  007F  SETMBR	R0	K35	R12
      0x70020006,  //  0080  JMP		#0088
      0xB81E5200,  //  0081  GETNGBL	R7	K41
      0x60200018,  //  0082  GETGBL	R8	G24
      0x58240061,  //  0083  LDCONST	R9	K97
      0x5C280800,  //  0084  MOVE	R10	R4
      0x7C200400,  //  0085  CALL	R8	2
      0x5824002B,  //  0086  LDCONST	R9	K43
      0x7C1C0400,  //  0087  CALL	R7	2
      0x8C1C0362,  //  0088  GETMET	R7	R1	K98
      0x60240018,  //  0089  GETGBL	R9	G24
      0x5828000E,  //  008A  LDCONST	R10	K14
      0x7C240200,  //  008B  CALL	R9	1
      0x7C1C0400,  //  008C  CALL	R7	2
      0xA8040001,  //  008D  EXBLK	1	1
      0x70020020,  //  008E  JMP		#00B0
      0xAC100002,  //  008F  CATCH	R4	0	2
      0x7002001D,  //  0090  JMP		#00AF
      0xB81A5200,  //  0091  GETNGBL	R6	K41
      0x601C0018,  //  0092  GETGBL	R7	G24
      0x58200063,  //  0093  LDCONST	R8	K99
      0x5C240800,  //  0094  MOVE	R9	R4
      0x5C280A00,  //  0095  MOVE	R10	R5
      0x7C1C0600,  //  0096  CALL	R7	3
      0x58200044,  //  0097  LDCONST	R8	K68
      0x7C180400,  //  0098  CALL	R6	2
      0x8C180364,  //  0099  GETMET	R6	R1	K100
      0x58200065,  //  009A  LDCONST	R8	K101
      0x7C180400,  //  009B  CALL	R6	2
      0x8C180366,  //  009C  GETMET	R6	R1	K102
      0x7C180200,  //  009D  CALL	R6	1
      0x8C180334,  //  009E  GETMET	R6	R1	K52
      0x60200018,  //  009F  GETGBL	R8	G24
      0x58240067,  //  00A0  LDCONST	R9	K103
      0x8C280368,  //  00A1  GETMET	R10	R1	K104
      0x5C300800,  //  00A2  MOVE	R12	R4
      0x7C280400,  //  00A3  CALL	R10	2
      0x8C2C0368,  //  00A4  GETMET	R11	R1	K104
      0x5C340A00,  //  00A5  MOVE	R13	R5
      0x7C2C0400,  //  00A6  CALL	R11	2
      0x7C200600,  //  00A7  CALL	R8	3
      0x7C180400,  //  00A8  CALL	R6	2
      0x8C180369,  //  00A9  GETMET	R6	R1	K105
      0x8820036A,  //  00AA  GETMBR	R8	R1	K106
      0x7C180400,  //  00AB  CALL	R6	2
      0x8C18036B,  //  00AC  GETMET	R6	R1	K107
      0x7C180200,  //  00AD  CALL	R6	1
      0x70020000,  //  00AE  JMP		#00B0
      0xB0080000,  //  00AF  RAISE	2	R0	R0
      0x80000000,  //  00B0  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: page_extensions_mgr
********************************************************************/
be_local_closure(class_Extension_manager_page_extensions_mgr,   /* name */
  be_nested_proto(
    23,                          /* nstack */
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
    ( &(const binstruction[178]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A2C00,  //  0001  IMPORT	R2	K22
      0x8C0C0364,  //  0002  GETMET	R3	R1	K100
      0x5814006C,  //  0003  LDCONST	R5	K108
      0x7C0C0400,  //  0004  CALL	R3	2
      0x8C0C0366,  //  0005  GETMET	R3	R1	K102
      0x7C0C0200,  //  0006  CALL	R3	1
      0x8C0C0334,  //  0007  GETMET	R3	R1	K52
      0x5814006D,  //  0008  LDCONST	R5	K109
      0x7C0C0400,  //  0009  CALL	R3	2
      0x8C0C0334,  //  000A  GETMET	R3	R1	K52
      0x5814006E,  //  000B  LDCONST	R5	K110
      0x7C0C0400,  //  000C  CALL	R3	2
      0x8C0C0334,  //  000D  GETMET	R3	R1	K52
      0x5814006F,  //  000E  LDCONST	R5	K111
      0x7C0C0400,  //  000F  CALL	R3	2
      0x8C0C0170,  //  0010  GETMET	R3	R0	K112
      0x50140200,  //  0011  LDBOOL	R5	1	0
      0x7C0C0400,  //  0012  CALL	R3	2
      0x6010000C,  //  0013  GETGBL	R4	G12
      0x5C140600,  //  0014  MOVE	R5	R3
      0x7C100200,  //  0015  CALL	R4	1
      0x24100917,  //  0016  GT	R4	R4	K23
      0x78120087,  //  0017  JMPF	R4	#00A0
      0x58100017,  //  0018  LDCONST	R4	K23
      0x6014000C,  //  0019  GETGBL	R5	G12
      0x5C180600,  //  001A  MOVE	R6	R3
      0x7C140200,  //  001B  CALL	R5	1
      0x14140805,  //  001C  LT	R5	R4	R5
      0x78160080,  //  001D  JMPF	R5	#009F
      0x24140917,  //  001E  GT	R5	R4	K23
      0x78160002,  //  001F  JMPF	R5	#0023
      0x8C140334,  //  0020  GETMET	R5	R1	K52
      0x581C0071,  //  0021  LDCONST	R7	K113
      0x7C140400,  //  0022  CALL	R5	2
      0x8C140772,  //  0023  GETMET	R5	R3	K114
      0x5C1C0800,  //  0024  MOVE	R7	R4
      0x7C140400,  //  0025  CALL	R5	2
      0x8C180107,  //  0026  GETMET	R6	R0	K7
      0x5C200A00,  //  0027  MOVE	R8	R5
      0x7C180400,  //  0028  CALL	R6	2
      0x8C1C0368,  //  0029  GETMET	R7	R1	K104
      0x5C240C00,  //  002A  MOVE	R9	R6
      0x7C1C0400,  //  002B  CALL	R7	2
      0xB8224A00,  //  002C  GETNGBL	R8	K37
      0x8C201138,  //  002D  GETMET	R8	R8	K56
      0x5C280A00,  //  002E  MOVE	R10	R5
      0x7C200400,  //  002F  CALL	R8	2
      0x60240009,  //  0030  GETGBL	R9	G9
      0x8C281109,  //  0031  GETMET	R10	R8	K9
      0x5830002F,  //  0032  LDCONST	R12	K47
      0x58340017,  //  0033  LDCONST	R13	K23
      0x7C280600,  //  0034  CALL	R10	3
      0x7C240200,  //  0035  CALL	R9	1
      0xB82A4A00,  //  0036  GETNGBL	R10	K37
      0x88281573,  //  0037  GETMBR	R10	R10	K115
      0x782A0005,  //  0038  JMPF	R10	#003F
      0xB82A4A00,  //  0039  GETNGBL	R10	K37
      0x88281573,  //  003A  GETMBR	R10	R10	K115
      0x8C28152C,  //  003B  GETMET	R10	R10	K44
      0x5C300A00,  //  003C  MOVE	R12	R5
      0x7C280400,  //  003D  CALL	R10	2
      0x70020000,  //  003E  JMP		#0040
      0x50280000,  //  003F  LDBOOL	R10	0	0
      0x782A0001,  //  0040  JMPF	R10	#0043
      0x582C0074,  //  0041  LDCONST	R11	K116
      0x70020000,  //  0042  JMP		#0044
      0x582C0024,  //  0043  LDCONST	R11	K36
      0x8C301109,  //  0044  GETMET	R12	R8	K9
      0x58380075,  //  0045  LDCONST	R14	K117
      0x503C0000,  //  0046  LDBOOL	R15	0	0
      0x7C300600,  //  0047  CALL	R12	3
      0x58340076,  //  0048  LDCONST	R13	K118
      0x58380077,  //  0049  LDCONST	R14	K119
      0x8C3C0334,  //  004A  GETMET	R15	R1	K52
      0x58440078,  //  004B  LDCONST	R17	K120
      0x7C3C0400,  //  004C  CALL	R15	2
      0x8C3C0334,  //  004D  GETMET	R15	R1	K52
      0x60440018,  //  004E  GETGBL	R17	G24
      0x58480079,  //  004F  LDCONST	R18	K121
      0x5C4C0E00,  //  0050  MOVE	R19	R7
      0x8C500368,  //  0051  GETMET	R20	R1	K104
      0x9458112D,  //  0052  GETIDX	R22	R8	K45
      0x7C500400,  //  0053  CALL	R20	2
      0x5C541600,  //  0054  MOVE	R21	R11
      0x7C440800,  //  0055  CALL	R17	4
      0x7C3C0400,  //  0056  CALL	R15	2
      0x8C3C0334,  //  0057  GETMET	R15	R1	K52
      0x60440018,  //  0058  GETGBL	R17	G24
      0x5848007A,  //  0059  LDCONST	R18	K122
      0x8C4C0368,  //  005A  GETMET	R19	R1	K104
      0x94541131,  //  005B  GETIDX	R21	R8	K49
      0x7C4C0400,  //  005C  CALL	R19	2
      0x7C440400,  //  005D  CALL	R17	2
      0x7C3C0400,  //  005E  CALL	R15	2
      0x243C1317,  //  005F  GT	R15	R9	K23
      0x783E0007,  //  0060  JMPF	R15	#0069
      0x8C3C0334,  //  0061  GETMET	R15	R1	K52
      0x60440018,  //  0062  GETGBL	R17	G24
      0x5848007A,  //  0063  LDCONST	R18	K122
      0x8C4C017B,  //  0064  GETMET	R19	R0	K123
      0x5C541200,  //  0065  MOVE	R21	R9
      0x7C4C0400,  //  0066  CALL	R19	2
      0x7C440400,  //  0067  CALL	R17	2
      0x7C3C0400,  //  0068  CALL	R15	2
      0x8C3C0334,  //  0069  GETMET	R15	R1	K52
      0x5844007C,  //  006A  LDCONST	R17	K124
      0x7C3C0400,  //  006B  CALL	R15	2
      0x8C3C0334,  //  006C  GETMET	R15	R1	K52
      0x5844007D,  //  006D  LDCONST	R17	K125
      0x7C3C0400,  //  006E  CALL	R15	2
      0x8C3C0334,  //  006F  GETMET	R15	R1	K52
      0x60440018,  //  0070  GETGBL	R17	G24
      0x5848007E,  //  0071  LDCONST	R18	K126
      0x782A0001,  //  0072  JMPF	R10	#0075
      0x5C4C1A00,  //  0073  MOVE	R19	R13
      0x70020000,  //  0074  JMP		#0076
      0x5C4C1C00,  //  0075  MOVE	R19	R14
      0x782A0001,  //  0076  JMPF	R10	#0079
      0x5850004D,  //  0077  LDCONST	R20	K77
      0x70020000,  //  0078  JMP		#007A
      0x5850004C,  //  0079  LDCONST	R20	K76
      0x5C540E00,  //  007A  MOVE	R21	R7
      0x782A0001,  //  007B  JMPF	R10	#007E
      0x5858007F,  //  007C  LDCONST	R22	K127
      0x70020000,  //  007D  JMP		#007F
      0x58580080,  //  007E  LDCONST	R22	K128
      0x7C440A00,  //  007F  CALL	R17	5
      0x7C3C0400,  //  0080  CALL	R15	2
      0x8C3C0334,  //  0081  GETMET	R15	R1	K52
      0x60440018,  //  0082  GETGBL	R17	G24
      0x58480081,  //  0083  LDCONST	R18	K129
      0x78320001,  //  0084  JMPF	R12	#0087
      0x584C0024,  //  0085  LDCONST	R19	K36
      0x70020000,  //  0086  JMP		#0088
      0x5C4C1C00,  //  0087  MOVE	R19	R14
      0x78320001,  //  0088  JMPF	R12	#008B
      0x5850004E,  //  0089  LDCONST	R20	K78
      0x70020000,  //  008A  JMP		#008C
      0x5850004F,  //  008B  LDCONST	R20	K79
      0x5C540E00,  //  008C  MOVE	R21	R7
      0x78320001,  //  008D  JMPF	R12	#0090
      0x58580082,  //  008E  LDCONST	R22	K130
      0x70020000,  //  008F  JMP		#0091
      0x58580083,  //  0090  LDCONST	R22	K131
      0x7C440A00,  //  0091  CALL	R17	5
      0x7C3C0400,  //  0092  CALL	R15	2
      0x8C3C0334,  //  0093  GETMET	R15	R1	K52
      0x60440018,  //  0094  GETGBL	R17	G24
      0x58480084,  //  0095  LDCONST	R18	K132
      0x5C4C0E00,  //  0096  MOVE	R19	R7
      0x5C500E00,  //  0097  MOVE	R20	R7
      0x7C440600,  //  0098  CALL	R17	3
      0x7C3C0400,  //  0099  CALL	R15	2
      0x8C3C0334,  //  009A  GETMET	R15	R1	K52
      0x58440085,  //  009B  LDCONST	R17	K133
      0x7C3C0400,  //  009C  CALL	R15	2
      0x00100918,  //  009D  ADD	R4	R4	K24
      0x7001FF79,  //  009E  JMP		#0019
      0x70020002,  //  009F  JMP		#00A3
      0x8C100334,  //  00A0  GETMET	R4	R1	K52
      0x58180086,  //  00A1  LDCONST	R6	K134
      0x7C100400,  //  00A2  CALL	R4	2
      0x8C100334,  //  00A3  GETMET	R4	R1	K52
      0x58180087,  //  00A4  LDCONST	R6	K135
      0x7C100400,  //  00A5  CALL	R4	2
      0x8C100334,  //  00A6  GETMET	R4	R1	K52
      0x58180088,  //  00A7  LDCONST	R6	K136
      0x7C100400,  //  00A8  CALL	R4	2
      0x8C100334,  //  00A9  GETMET	R4	R1	K52
      0x58180089,  //  00AA  LDCONST	R6	K137
      0x7C100400,  //  00AB  CALL	R4	2
      0x8C100369,  //  00AC  GETMET	R4	R1	K105
      0x8818036A,  //  00AD  GETMBR	R6	R1	K106
      0x7C100400,  //  00AE  CALL	R4	2
      0x8C10036B,  //  00AF  GETMET	R4	R1	K107
      0x7C100200,  //  00B0  CALL	R4	1
      0x80000000,  //  00B1  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: load_manifest
********************************************************************/
be_local_closure(class_Extension_manager_load_manifest,   /* name */
  be_nested_proto(
    13,                          /* nstack */
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
    ( &(const binstruction[129]) {  /* code */
      0xA8020071,  //  0000  EXBLK	0	#0073
      0xA4062C00,  //  0001  IMPORT	R1	K22
      0xB80A4A00,  //  0002  GETNGBL	R2	K37
      0x8C08058A,  //  0003  GETMET	R2	R2	K138
      0x7C080200,  //  0004  CALL	R2	1
      0x600C0018,  //  0005  GETGBL	R3	G24
      0x5810008B,  //  0006  LDCONST	R4	K139
      0xB8164A00,  //  0007  GETNGBL	R5	K37
      0x8C140B2F,  //  0008  GETMET	R5	R5	K47
      0x7C140200,  //  0009  CALL	R5	1
      0x7C0C0400,  //  000A  CALL	R3	2
      0x88100123,  //  000B  GETMBR	R4	R0	K35
      0x7412000F,  //  000C  JMPT	R4	#001D
      0xB8124A00,  //  000D  GETNGBL	R4	K37
      0x8C10098C,  //  000E  GETMET	R4	R4	K140
      0x5818008D,  //  000F  LDCONST	R6	K141
      0x501C0200,  //  0010  LDBOOL	R7	1	0
      0x7C100600,  //  0011  CALL	R4	3
      0x9410098D,  //  0012  GETIDX	R4	R4	K141
      0x8C14035C,  //  0013  GETMET	R5	R1	K92
      0x5C1C0800,  //  0014  MOVE	R7	R4
      0x5820001C,  //  0015  LDCONST	R8	K28
      0x7C140600,  //  0016  CALL	R5	3
      0x60180018,  //  0017  GETGBL	R6	G24
      0x581C008E,  //  0018  LDCONST	R7	K142
      0x94200B17,  //  0019  GETIDX	R8	R5	K23
      0x94240B44,  //  001A  GETIDX	R9	R5	K68
      0x7C180600,  //  001B  CALL	R6	3
      0x90024606,  //  001C  SETMBR	R0	K35	R6
      0x60100018,  //  001D  GETGBL	R4	G24
      0x5814008F,  //  001E  LDCONST	R5	K143
      0x88180123,  //  001F  GETMBR	R6	R0	K35
      0x881C0190,  //  0020  GETMBR	R7	R0	K144
      0x5C200400,  //  0021  MOVE	R8	R2
      0x5C240600,  //  0022  MOVE	R9	R3
      0x7C100A00,  //  0023  CALL	R4	5
      0xB8165200,  //  0024  GETNGBL	R5	K41
      0x60180018,  //  0025  GETGBL	R6	G24
      0x581C0091,  //  0026  LDCONST	R7	K145
      0x5C200800,  //  0027  MOVE	R8	R4
      0x7C180400,  //  0028  CALL	R6	2
      0x581C002B,  //  0029  LDCONST	R7	K43
      0x7C140400,  //  002A  CALL	R5	2
      0xB8168000,  //  002B  GETNGBL	R5	K64
      0x7C140000,  //  002C  CALL	R5	0
      0x8C180B41,  //  002D  GETMET	R6	R5	K65
      0x5C200800,  //  002E  MOVE	R8	R4
      0x7C180400,  //  002F  CALL	R6	2
      0x8C180B42,  //  0030  GETMET	R6	R5	K66
      0x7C180200,  //  0031  CALL	R6	1
      0x541E00C7,  //  0032  LDINT	R7	200
      0x201C0C07,  //  0033  NE	R7	R6	R7
      0x781E0035,  //  0034  JMPF	R7	#006B
      0x881C015B,  //  0035  GETMBR	R7	R0	K91
      0x88200123,  //  0036  GETMBR	R8	R0	K35
      0x201C0E08,  //  0037  NE	R7	R7	R8
      0x781E0022,  //  0038  JMPF	R7	#005C
      0x8C1C0B92,  //  0039  GETMET	R7	R5	K146
      0x7C1C0200,  //  003A  CALL	R7	1
      0x141C0F17,  //  003B  LT	R7	R7	K23
      0x781E0005,  //  003C  JMPF	R7	#0043
      0x8C1C0B93,  //  003D  GETMET	R7	R5	K147
      0x7C1C0200,  //  003E  CALL	R7	1
      0xB81E8000,  //  003F  GETNGBL	R7	K64
      0x7C1C0000,  //  0040  CALL	R7	0
      0x5C140E00,  //  0041  MOVE	R5	R7
      0x70020001,  //  0042  JMP		#0045
      0x8C1C0B46,  //  0043  GETMET	R7	R5	K70
      0x7C1C0200,  //  0044  CALL	R7	1
      0x881C015B,  //  0045  GETMBR	R7	R0	K91
      0x90024607,  //  0046  SETMBR	R0	K35	R7
      0x601C0018,  //  0047  GETGBL	R7	G24
      0x5820008F,  //  0048  LDCONST	R8	K143
      0x88240123,  //  0049  GETMBR	R9	R0	K35
      0x88280190,  //  004A  GETMBR	R10	R0	K144
      0x5C2C0400,  //  004B  MOVE	R11	R2
      0x5C300600,  //  004C  MOVE	R12	R3
      0x7C1C0A00,  //  004D  CALL	R7	5
      0x5C100E00,  //  004E  MOVE	R4	R7
      0xB81E5200,  //  004F  GETNGBL	R7	K41
      0x60200018,  //  0050  GETGBL	R8	G24
      0x58240091,  //  0051  LDCONST	R9	K145
      0x5C280800,  //  0052  MOVE	R10	R4
      0x7C200400,  //  0053  CALL	R8	2
      0x5824002B,  //  0054  LDCONST	R9	K43
      0x7C1C0400,  //  0055  CALL	R7	2
      0x8C1C0B41,  //  0056  GETMET	R7	R5	K65
      0x5C240800,  //  0057  MOVE	R9	R4
      0x7C1C0400,  //  0058  CALL	R7	2
      0x8C1C0B42,  //  0059  GETMET	R7	R5	K66
      0x7C1C0200,  //  005A  CALL	R7	1
      0x5C180E00,  //  005B  MOVE	R6	R7
      0x541E00C7,  //  005C  LDINT	R7	200
      0x201C0C07,  //  005D  NE	R7	R6	R7
      0x781E000B,  //  005E  JMPF	R7	#006B
      0xB81E5200,  //  005F  GETNGBL	R7	K41
      0x60200018,  //  0060  GETGBL	R8	G24
      0x58240094,  //  0061  LDCONST	R9	K148
      0x5C280C00,  //  0062  MOVE	R10	R6
      0x7C200400,  //  0063  CALL	R8	2
      0x58240044,  //  0064  LDCONST	R9	K68
      0x7C1C0400,  //  0065  CALL	R7	2
      0x601C0018,  //  0066  GETGBL	R7	G24
      0x58200095,  //  0067  LDCONST	R8	K149
      0x5C240C00,  //  0068  MOVE	R9	R6
      0x7C1C0400,  //  0069  CALL	R7	2
      0xB0072C07,  //  006A  RAISE	1	K150	R7
      0x8C1C0B97,  //  006B  GETMET	R7	R5	K151
      0x7C1C0200,  //  006C  CALL	R7	1
      0x8C200B46,  //  006D  GETMET	R8	R5	K70
      0x7C200200,  //  006E  CALL	R8	1
      0xA8040001,  //  006F  EXBLK	1	1
      0x80040E00,  //  0070  RET	1	R7
      0xA8040001,  //  0071  EXBLK	1	1
      0x7002000C,  //  0072  JMP		#0080
      0xAC040002,  //  0073  CATCH	R1	0	2
      0x70020009,  //  0074  JMP		#007F
      0xB80E5200,  //  0075  GETNGBL	R3	K41
      0x60100018,  //  0076  GETGBL	R4	G24
      0x58140049,  //  0077  LDCONST	R5	K73
      0x5C180200,  //  0078  MOVE	R6	R1
      0x5C1C0400,  //  0079  MOVE	R7	R2
      0x7C100600,  //  007A  CALL	R4	3
      0x58140044,  //  007B  LDCONST	R5	K68
      0x7C0C0400,  //  007C  CALL	R3	2
      0xB0040202,  //  007D  RAISE	1	R1	R2
      0x70020000,  //  007E  JMP		#0080
      0xB0080000,  //  007F  RAISE	2	R0	R0
      0x80000000,  //  0080  RET	0
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
      0x58000006,  //  0000  LDCONST	R0	K6
      0x60040013,  //  0001  GETGBL	R1	G19
      0x7C040000,  //  0002  CALL	R1	0
      0x60080010,  //  0003  GETGBL	R2	G16
      0x8C0C0137,  //  0004  GETMET	R3	R0	K55
      0x7C0C0200,  //  0005  CALL	R3	1
      0x7C080200,  //  0006  CALL	R2	1
      0xA8020006,  //  0007  EXBLK	0	#000F
      0x5C0C0400,  //  0008  MOVE	R3	R2
      0x7C0C0000,  //  0009  CALL	R3	0
      0x8C100107,  //  000A  GETMET	R4	R0	K7
      0x5C180600,  //  000B  MOVE	R6	R3
      0x7C100400,  //  000C  CALL	R4	2
      0x98040803,  //  000D  SETIDX	R1	R4	R3
      0x7001FFF8,  //  000E  JMP		#0008
      0x58080022,  //  000F  LDCONST	R2	K34
      0xAC080200,  //  0010  CATCH	R2	1	0
      0xB0080000,  //  0011  RAISE	2	R0	R0
      0x80040200,  //  0012  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: enable_disable_ext
********************************************************************/
be_local_closure(class_Extension_manager_enable_disable_ext,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Extension_manager,     /* shared constants */
    &be_const_str_enable_disable_ext,
    &be_const_str_solidified,
    ( &(const binstruction[66]) {  /* code */
      0x58080006,  //  0000  LDCONST	R2	K6
      0xA40E2C00,  //  0001  IMPORT	R3	K22
      0x8C100507,  //  0002  GETMET	R4	R2	K7
      0x5C180000,  //  0003  MOVE	R6	R0
      0x7C100400,  //  0004  CALL	R4	2
      0x5C000800,  //  0005  MOVE	R0	R4
      0x8C100508,  //  0006  GETMET	R4	R2	K8
      0x7C100200,  //  0007  CALL	R4	1
      0x8C100909,  //  0008  GETMET	R4	R4	K9
      0x5C180000,  //  0009  MOVE	R6	R0
      0x7C100400,  //  000A  CALL	R4	2
      0x4C140000,  //  000B  LDNIL	R5
      0x20140805,  //  000C  NE	R5	R4	R5
      0x78160031,  //  000D  JMPF	R5	#0040
      0x4C140000,  //  000E  LDNIL	R5
      0x78060008,  //  000F  JMPF	R1	#0019
      0x8C18071F,  //  0010  GETMET	R6	R3	K31
      0x5C200800,  //  0011  MOVE	R8	R4
      0x58240020,  //  0012  LDCONST	R9	K32
      0x7C180600,  //  0013  CALL	R6	3
      0x781A0003,  //  0014  JMPF	R6	#0019
      0x5419FFFD,  //  0015  LDINT	R6	-2
      0x401A2E06,  //  0016  CONNECT	R6	K23	R6
      0x94140806,  //  0017  GETIDX	R5	R4	R6
      0x70020008,  //  0018  JMP		#0022
      0x5C180200,  //  0019  MOVE	R6	R1
      0x741A0006,  //  001A  JMPT	R6	#0022
      0x8C18071F,  //  001B  GETMET	R6	R3	K31
      0x5C200800,  //  001C  MOVE	R8	R4
      0x5824001B,  //  001D  LDCONST	R9	K27
      0x7C180600,  //  001E  CALL	R6	3
      0x781A0001,  //  001F  JMPF	R6	#0022
      0x0018091A,  //  0020  ADD	R6	R4	K26
      0x5C140C00,  //  0021  MOVE	R5	R6
      0x7816001C,  //  0022  JMPF	R5	#0040
      0xA41A1400,  //  0023  IMPORT	R6	K10
      0x8C1C0D98,  //  0024  GETMET	R7	R6	K152
      0x5C240800,  //  0025  MOVE	R9	R4
      0x5C280A00,  //  0026  MOVE	R10	R5
      0x7C1C0600,  //  0027  CALL	R7	3
      0x781E0015,  //  0028  JMPF	R7	#003F
      0xB8224A00,  //  0029  GETNGBL	R8	K37
      0x88201173,  //  002A  GETMBR	R8	R8	K115
      0x4C240000,  //  002B  LDNIL	R9
      0x20201009,  //  002C  NE	R8	R8	R9
      0x78220010,  //  002D  JMPF	R8	#003F
      0xB8224A00,  //  002E  GETNGBL	R8	K37
      0x88201173,  //  002F  GETMBR	R8	R8	K115
      0x8C20112C,  //  0030  GETMET	R8	R8	K44
      0x5C280800,  //  0031  MOVE	R10	R4
      0x7C200400,  //  0032  CALL	R8	2
      0x7822000A,  //  0033  JMPF	R8	#003F
      0xB8224A00,  //  0034  GETNGBL	R8	K37
      0x88201173,  //  0035  GETMBR	R8	R8	K115
      0xB8264A00,  //  0036  GETNGBL	R9	K37
      0x88241373,  //  0037  GETMBR	R9	R9	K115
      0x94241204,  //  0038  GETIDX	R9	R9	R4
      0x98200A09,  //  0039  SETIDX	R8	R5	R9
      0xB8224A00,  //  003A  GETNGBL	R8	K37
      0x88201173,  //  003B  GETMBR	R8	R8	K115
      0x8C20110C,  //  003C  GETMET	R8	R8	K12
      0x5C280800,  //  003D  MOVE	R10	R4
      0x7C200400,  //  003E  CALL	R8	2
      0x80040E00,  //  003F  RET	1	R7
      0x50140000,  //  0040  LDBOOL	R5	0	0
      0x80040A00,  //  0041  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: run_stop_ext
********************************************************************/
be_local_closure(class_Extension_manager_run_stop_ext,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Extension_manager,     /* shared constants */
    &be_const_str_run_stop_ext,
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0x58080006,  //  0000  LDCONST	R2	K6
      0x8C0C0507,  //  0001  GETMET	R3	R2	K7
      0x5C140000,  //  0002  MOVE	R5	R0
      0x7C0C0400,  //  0003  CALL	R3	2
      0x5C000600,  //  0004  MOVE	R0	R3
      0x8C0C0508,  //  0005  GETMET	R3	R2	K8
      0x7C0C0200,  //  0006  CALL	R3	1
      0x8C0C0709,  //  0007  GETMET	R3	R3	K9
      0x5C140000,  //  0008  MOVE	R5	R0
      0x7C0C0400,  //  0009  CALL	R3	2
      0x4C100000,  //  000A  LDNIL	R4
      0x20100604,  //  000B  NE	R4	R3	R4
      0x7812000C,  //  000C  JMPF	R4	#001A
      0x78060005,  //  000D  JMPF	R1	#0014
      0xB8124A00,  //  000E  GETNGBL	R4	K37
      0x8C100928,  //  000F  GETMET	R4	R4	K40
      0x5C180600,  //  0010  MOVE	R6	R3
      0x7C100400,  //  0011  CALL	R4	2
      0x80040800,  //  0012  RET	1	R4
      0x70020004,  //  0013  JMP		#0019
      0xB8124A00,  //  0014  GETNGBL	R4	K37
      0x8C100999,  //  0015  GETMET	R4	R4	K153
      0x5C180600,  //  0016  MOVE	R6	R3
      0x7C100400,  //  0017  CALL	R4	2
      0x80040800,  //  0018  RET	1	R4
      0x70020001,  //  0019  JMP		#001C
      0x50100000,  //  001A  LDBOOL	R4	0	0
      0x80040800,  //  001B  RET	1	R4
      0x80000000,  //  001C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: page_extensions_store
********************************************************************/
be_local_closure(class_Extension_manager_page_extensions_store,   /* name */
  be_nested_proto(
    32,                          /* nstack */
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
    ( &(const binstruction[216]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A2C00,  //  0001  IMPORT	R2	K22
      0xA40E4E00,  //  0002  IMPORT	R3	K39
      0x8C10039A,  //  0003  GETMET	R4	R1	K154
      0x541A00C7,  //  0004  LDINT	R6	200
      0x581C009B,  //  0005  LDCONST	R7	K155
      0x7C100600,  //  0006  CALL	R4	3
      0x4C100000,  //  0007  LDNIL	R4
      0xA8020004,  //  0008  EXBLK	0	#000E
      0x8C14019C,  //  0009  GETMET	R5	R0	K156
      0x7C140200,  //  000A  CALL	R5	1
      0x5C100A00,  //  000B  MOVE	R4	R5
      0xA8040001,  //  000C  EXBLK	1	1
      0x70020011,  //  000D  JMP		#0020
      0xAC140002,  //  000E  CATCH	R5	0	2
      0x7002000E,  //  000F  JMP		#001F
      0x8C1C0334,  //  0010  GETMET	R7	R1	K52
      0x5824009D,  //  0011  LDCONST	R9	K157
      0x7C1C0400,  //  0012  CALL	R7	2
      0x8C1C0334,  //  0013  GETMET	R7	R1	K52
      0x60240018,  //  0014  GETGBL	R9	G24
      0x5828009E,  //  0015  LDCONST	R10	K158
      0x8C2C0368,  //  0016  GETMET	R11	R1	K104
      0x5C340C00,  //  0017  MOVE	R13	R6
      0x7C2C0400,  //  0018  CALL	R11	2
      0x7C240400,  //  0019  CALL	R9	2
      0x7C1C0400,  //  001A  CALL	R7	2
      0x8C1C039F,  //  001B  GETMET	R7	R1	K159
      0x7C1C0200,  //  001C  CALL	R7	1
      0x80000E00,  //  001D  RET	0
      0x70020000,  //  001E  JMP		#0020
      0xB0080000,  //  001F  RAISE	2	R0	R0
      0x8C1405A0,  //  0020  GETMET	R5	R2	K160
      0x5C1C0800,  //  0021  MOVE	R7	R4
      0x582000A1,  //  0022  LDCONST	R8	K161
      0x7C140600,  //  0023  CALL	R5	3
      0x8C180334,  //  0024  GETMET	R6	R1	K52
      0x582000A2,  //  0025  LDCONST	R8	K162
      0x7C180400,  //  0026  CALL	R6	2
      0x8C180334,  //  0027  GETMET	R6	R1	K52
      0x60200018,  //  0028  GETGBL	R8	G24
      0x582400A3,  //  0029  LDCONST	R9	K163
      0x5C280A00,  //  002A  MOVE	R10	R5
      0x7C200400,  //  002B  CALL	R8	2
      0x7C180400,  //  002C  CALL	R6	2
      0x8C180334,  //  002D  GETMET	R6	R1	K52
      0x582000A4,  //  002E  LDCONST	R8	K164
      0x7C180400,  //  002F  CALL	R6	2
      0x8C180108,  //  0030  GETMET	R6	R0	K8
      0x7C180200,  //  0031  CALL	R6	1
      0x581C0018,  //  0032  LDCONST	R7	K24
      0x58200017,  //  0033  LDCONST	R8	K23
      0x6024000C,  //  0034  GETGBL	R9	G12
      0x5C280800,  //  0035  MOVE	R10	R4
      0x7C240200,  //  0036  CALL	R9	1
      0x14241009,  //  0037  LT	R9	R8	R9
      0x7826008F,  //  0038  JMPF	R9	#00C9
      0x8C240509,  //  0039  GETMET	R9	R2	K9
      0x5C2C0800,  //  003A  MOVE	R11	R4
      0x583000A5,  //  003B  LDCONST	R12	K165
      0x5C341000,  //  003C  MOVE	R13	R8
      0x7C240800,  //  003D  CALL	R9	4
      0x14281317,  //  003E  LT	R10	R9	K23
      0x782A0003,  //  003F  JMPF	R10	#0044
      0x6028000C,  //  0040  GETGBL	R10	G12
      0x5C2C0800,  //  0041  MOVE	R11	R4
      0x7C280200,  //  0042  CALL	R10	1
      0x5C241400,  //  0043  MOVE	R9	R10
      0x40281009,  //  0044  CONNECT	R10	R8	R9
      0x9428080A,  //  0045  GETIDX	R10	R4	R10
      0x8C2C01A6,  //  0046  GETMET	R11	R0	K166
      0x5C341400,  //  0047  MOVE	R13	R10
      0x7C2C0400,  //  0048  CALL	R11	2
      0x4C300000,  //  0049  LDNIL	R12
      0x2030160C,  //  004A  NE	R12	R11	R12
      0x78320079,  //  004B  JMPF	R12	#00C6
      0x9430172F,  //  004C  GETIDX	R12	R11	K47
      0x8C34017B,  //  004D  GETMET	R13	R0	K123
      0x5C3C1800,  //  004E  MOVE	R15	R12
      0x7C340400,  //  004F  CALL	R13	2
      0x8C380368,  //  0050  GETMET	R14	R1	K104
      0x9440172D,  //  0051  GETIDX	R16	R11	K45
      0x7C380400,  //  0052  CALL	R14	2
      0x943C172E,  //  0053  GETIDX	R15	R11	K46
      0x8C4005A7,  //  0054  GETMET	R16	R2	K167
      0x8C480368,  //  0055  GETMET	R18	R1	K104
      0x94501731,  //  0056  GETIDX	R20	R11	K49
      0x7C480400,  //  0057  CALL	R18	2
      0x584C00A8,  //  0058  LDCONST	R19	K168
      0x585000A9,  //  0059  LDCONST	R20	K169
      0x7C400800,  //  005A  CALL	R16	4
      0x94441733,  //  005B  GETIDX	R17	R11	K51
      0x50480000,  //  005C  LDBOOL	R18	0	0
      0x4C4C0000,  //  005D  LDNIL	R19
      0x4C500000,  //  005E  LDNIL	R20
      0x8C540107,  //  005F  GETMET	R21	R0	K7
      0x945C172E,  //  0060  GETIDX	R23	R11	K46
      0x7C540400,  //  0061  CALL	R21	2
      0x5C502A00,  //  0062  MOVE	R20	R21
      0x8C540368,  //  0063  GETMET	R21	R1	K104
      0x5C5C2800,  //  0064  MOVE	R23	R20
      0x7C540400,  //  0065  CALL	R21	2
      0x8C580D2C,  //  0066  GETMET	R22	R6	K44
      0x5C602800,  //  0067  MOVE	R24	R20
      0x7C580400,  //  0068  CALL	R22	2
      0x5C482C00,  //  0069  MOVE	R18	R22
      0x784A000B,  //  006A  JMPF	R18	#0077
      0x94580C14,  //  006B  GETIDX	R22	R6	R20
      0xB85E4A00,  //  006C  GETNGBL	R23	K37
      0x8C5C2F38,  //  006D  GETMET	R23	R23	K56
      0x5C642C00,  //  006E  MOVE	R25	R22
      0x7C5C0400,  //  006F  CALL	R23	2
      0x60600009,  //  0070  GETGBL	R24	G9
      0x8C642F09,  //  0071  GETMET	R25	R23	K9
      0x586C002F,  //  0072  LDCONST	R27	K47
      0x58700017,  //  0073  LDCONST	R28	K23
      0x7C640600,  //  0074  CALL	R25	3
      0x7C600200,  //  0075  CALL	R24	1
      0x5C4C3000,  //  0076  MOVE	R19	R24
      0x784A0001,  //  0077  JMPF	R18	#007A
      0x1458260C,  //  0078  LT	R22	R19	R12
      0x745A0000,  //  0079  JMPT	R22	#007B
      0x50580001,  //  007A  LDBOOL	R22	0	1
      0x50580200,  //  007B  LDBOOL	R22	1	0
      0x8C5C0334,  //  007C  GETMET	R23	R1	K52
      0x60640018,  //  007D  GETGBL	R25	G24
      0x586800AA,  //  007E  LDCONST	R26	K170
      0x5C6C0E00,  //  007F  MOVE	R27	R7
      0x5C701C00,  //  0080  MOVE	R28	R14
      0x8C74017B,  //  0081  GETMET	R29	R0	K123
      0x5C7C1800,  //  0082  MOVE	R31	R12
      0x7C740400,  //  0083  CALL	R29	2
      0x7C640800,  //  0084  CALL	R25	4
      0x7C5C0400,  //  0085  CALL	R23	2
      0x785A0003,  //  0086  JMPF	R22	#008B
      0x8C5C0334,  //  0087  GETMET	R23	R1	K52
      0x586400AB,  //  0088  LDCONST	R25	K171
      0x7C5C0400,  //  0089  CALL	R23	2
      0x70020003,  //  008A  JMP		#008F
      0x784A0002,  //  008B  JMPF	R18	#008F
      0x8C5C0334,  //  008C  GETMET	R23	R1	K52
      0x586400AC,  //  008D  LDCONST	R25	K172
      0x7C5C0400,  //  008E  CALL	R23	2
      0x8C5C0334,  //  008F  GETMET	R23	R1	K52
      0x60640018,  //  0090  GETGBL	R25	G24
      0x586800AD,  //  0091  LDCONST	R26	K173
      0x5C6C0E00,  //  0092  MOVE	R27	R7
      0x5C700E00,  //  0093  MOVE	R28	R7
      0x5C742000,  //  0094  MOVE	R29	R16
      0x7C640800,  //  0095  CALL	R25	4
      0x7C5C0400,  //  0096  CALL	R23	2
      0x785A0008,  //  0097  JMPF	R22	#00A1
      0x8C5C0334,  //  0098  GETMET	R23	R1	K52
      0x60640018,  //  0099  GETGBL	R25	G24
      0x586800AE,  //  009A  LDCONST	R26	K174
      0x8C6C017B,  //  009B  GETMET	R27	R0	K123
      0x5C742600,  //  009C  MOVE	R29	R19
      0x7C6C0400,  //  009D  CALL	R27	2
      0x5C701A00,  //  009E  MOVE	R28	R13
      0x7C640600,  //  009F  CALL	R25	3
      0x7C5C0400,  //  00A0  CALL	R23	2
      0x8C5C0334,  //  00A1  GETMET	R23	R1	K52
      0x586400AF,  //  00A2  LDCONST	R25	K175
      0x7C5C0400,  //  00A3  CALL	R23	2
      0x784A0013,  //  00A4  JMPF	R18	#00B9
      0x785A0007,  //  00A5  JMPF	R22	#00AE
      0x8C5C0334,  //  00A6  GETMET	R23	R1	K52
      0x60640018,  //  00A7  GETGBL	R25	G24
      0x586800B0,  //  00A8  LDCONST	R26	K176
      0x5C6C2A00,  //  00A9  MOVE	R27	R21
      0x5C702A00,  //  00AA  MOVE	R28	R21
      0x7C640600,  //  00AB  CALL	R25	3
      0x7C5C0400,  //  00AC  CALL	R23	2
      0x70020002,  //  00AD  JMP		#00B1
      0x8C5C0334,  //  00AE  GETMET	R23	R1	K52
      0x586400B1,  //  00AF  LDCONST	R25	K177
      0x7C5C0400,  //  00B0  CALL	R23	2
      0x8C5C0334,  //  00B1  GETMET	R23	R1	K52
      0x60640018,  //  00B2  GETGBL	R25	G24
      0x586800B2,  //  00B3  LDCONST	R26	K178
      0x5C6C2A00,  //  00B4  MOVE	R27	R21
      0x5C702A00,  //  00B5  MOVE	R28	R21
      0x7C640600,  //  00B6  CALL	R25	3
      0x7C5C0400,  //  00B7  CALL	R23	2
      0x70020008,  //  00B8  JMP		#00C2
      0x8C5C0334,  //  00B9  GETMET	R23	R1	K52
      0x60640018,  //  00BA  GETGBL	R25	G24
      0x586800B3,  //  00BB  LDCONST	R26	K179
      0x5C6C2A00,  //  00BC  MOVE	R27	R21
      0x5C701C00,  //  00BD  MOVE	R28	R14
      0x5C742A00,  //  00BE  MOVE	R29	R21
      0x5C781C00,  //  00BF  MOVE	R30	R14
      0x7C640A00,  //  00C0  CALL	R25	5
      0x7C5C0400,  //  00C1  CALL	R23	2
      0x8C5C0334,  //  00C2  GETMET	R23	R1	K52
      0x58640085,  //  00C3  LDCONST	R25	K133
      0x7C5C0400,  //  00C4  CALL	R23	2
      0x001C0F18,  //  00C5  ADD	R7	R7	K24
      0x00301318,  //  00C6  ADD	R12	R9	K24
      0x5C201800,  //  00C7  MOVE	R8	R12
      0x7001FF6A,  //  00C8  JMP		#0034
      0x8C240334,  //  00C9  GETMET	R9	R1	K52
      0x582C00B4,  //  00CA  LDCONST	R11	K180
      0x7C240400,  //  00CB  CALL	R9	2
      0x8C240334,  //  00CC  GETMET	R9	R1	K52
      0x602C0018,  //  00CD  GETGBL	R11	G24
      0x583000B5,  //  00CE  LDCONST	R12	K181
      0x88340123,  //  00CF  GETMBR	R13	R0	K35
      0x7C2C0400,  //  00D0  CALL	R11	2
      0x7C240400,  //  00D1  CALL	R9	2
      0x8C240334,  //  00D2  GETMET	R9	R1	K52
      0x582C0087,  //  00D3  LDCONST	R11	K135
      0x7C240400,  //  00D4  CALL	R9	2
      0x8C24039F,  //  00D5  GETMET	R9	R1	K159
      0x7C240200,  //  00D6  CALL	R9	1
      0x80000000,  //  00D7  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Extension_manager
********************************************************************/
be_local_class(Extension_manager,
    1,
    NULL,
    be_nested_map(24,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(page_extensions_mgr_dispatcher, 22), be_const_closure(class_Extension_manager_page_extensions_mgr_dispatcher_closure) },
        { be_const_key(ext_repo, -1), be_const_var(0) },
        { be_const_key(delete_ext, 4), be_const_static_closure(class_Extension_manager_delete_ext_closure) },
        { be_const_key(EXT_REPO_MANIFEST, 5), be_nested_str(extensions_X2Ejsonl) },
        { be_const_key(web_add_handler, -1), be_const_closure(class_Extension_manager_web_add_handler_closure) },
        { be_const_key(run_stop_ext, 7), be_const_static_closure(class_Extension_manager_run_stop_ext_closure) },
        { be_const_key(check_or_create_dir, -1), be_const_static_closure(class_Extension_manager_check_or_create_dir_closure) },
        { be_const_key(init, -1), be_const_closure(class_Extension_manager_init_closure) },
        { be_const_key(list_extensions, -1), be_const_static_closure(class_Extension_manager_list_extensions_closure) },
        { be_const_key(version_string, -1), be_const_static_closure(class_Extension_manager_version_string_closure) },
        { be_const_key(load_manifest, -1), be_const_closure(class_Extension_manager_load_manifest_closure) },
        { be_const_key(EXT_REPO_FOLDER, -1), be_nested_str(tapp_X2F) },
        { be_const_key(list_extensions_in_fs, -1), be_const_static_closure(class_Extension_manager_list_extensions_in_fs_closure) },
        { be_const_key(EXT_REPO, 9), be_nested_str(https_X3A_X2F_X2Fota_X2Etasmota_X2Ecom_X2Fextensions_X2F) },
        { be_const_key(page_extensions_ctl, -1), be_const_closure(class_Extension_manager_page_extensions_ctl_closure) },
        { be_const_key(install_from_store, 14), be_const_closure(class_Extension_manager_install_from_store_closure) },
        { be_const_key(page_extensions_mgr, -1), be_const_closure(class_Extension_manager_page_extensions_mgr_closure) },
        { be_const_key(EXT_FOLDER, 13), be_nested_str(_X2F_X2Eextensions_X2F) },
        { be_const_key(web_add_button, 11), be_const_closure(class_Extension_manager_web_add_button_closure) },
        { be_const_key(manifest_decode, 10), be_const_static_closure(class_Extension_manager_manifest_decode_closure) },
        { be_const_key(list_installed_ext, -1), be_const_static_closure(class_Extension_manager_list_installed_ext_closure) },
        { be_const_key(enable_disable_ext, -1), be_const_static_closure(class_Extension_manager_enable_disable_ext_closure) },
        { be_const_key(tapp_name, -1), be_const_static_closure(class_Extension_manager_tapp_name_closure) },
        { be_const_key(page_extensions_store, -1), be_const_closure(class_Extension_manager_page_extensions_store_closure) },
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
