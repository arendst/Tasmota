/********************************************************************
 * Tasmota lib
 *
 * To use: `import tasmota`
 *******************************************************************/
#include "be_constobj.h"
#include "be_ctypes.h"
#include "be_mapping.h"
#include "be_ctypes.h"

extern struct TasmotaGlobal_t TasmotaGlobal;
extern struct TSettings * Settings;
extern const be_ctypes_structure_t be_tasmota_global_struct;
extern const be_ctypes_structure_t be_tasmota_settings_struct;

extern int l_getFreeHeap(bvm *vm);
extern int l_arch(bvm *vm);
extern int be_mqtt_publish(bvm *vm);
extern int l_publish_result(bvm *vm);
extern int l_publish_rule(bvm *vm);
extern int l_cmd(bvm *vm);
extern int l_getoption(bvm *vm);
extern int l_millis(bvm *vm);
extern int l_timereached(bvm *vm);
extern int l_rtc(bvm *vm);
extern int l_rtc_utc(bvm *vm);
extern int l_time_dump(bvm *vm);
extern int l_strftime(bvm *vm);
extern int l_strptime(bvm *vm);
extern int l_memory(bvm *vm);
extern int l_wifi(bvm *vm);
extern int l_eth(bvm *vm);
extern int l_hostname(bvm *vm);
extern int l_yield(bvm *vm);
extern int l_delay(bvm *vm);
extern int l_delay_microseconds(bvm *vm);
extern int l_scaleuint(bvm *vm);
extern int l_scaleint(bvm *vm);
extern int l_sineint(bvm *vm);
extern int l_logInfo(bvm *vm);
extern int l_loglevel(bvm *vm);
extern int l_save(bvm *vm);
extern int t_random_byte(bvm *vm);
extern int l_locale(bvm *vm);

extern int l_read_sensors(bvm *vm);

extern int l_respCmnd(bvm *vm);
extern int l_respCmndStr(bvm *vm);
extern int l_respCmndDone(bvm *vm);
extern int l_respCmndError(bvm *vm);
extern int l_respCmndFailed(bvm *vm);
extern int l_resolveCmnd(bvm *vm);

extern int l_respAppend(bvm *vm);
extern int l_webSend(bvm *vm);
extern int l_webSendDecimal(bvm *vm);

extern int l_webcolor(bvm *vm);

extern int l_getlight(bvm *vm);
extern int l_setlight(bvm *vm);
extern int l_getpower(bvm *vm);
extern int l_setpower(bvm *vm);
extern int l_getswitch(bvm *vm);

extern int l_i2cenabled(bvm *vm);
extern int tasm_find_op(bvm *vm);
extern int tasm_apply_str_op(bvm *vm);

// tasmota.version() -> int
extern int32_t be_Tasmota_version(void);
BE_FUNC_CTYPE_DECLARE(be_Tasmota_version, "i", "-");

extern bbool BerryBECLoader(const char * url);
BE_FUNC_CTYPE_DECLARE(BerryBECLoader, "b", "s")

#include "solidify/solidified_tasmota_class.h"
#include "solidify/solidified_rule_matcher.h"
#include "solidify/solidified_trigger_class.h"

#include "be_fixed_be_class_tasmota.h"

/* @const_object_info_begin
class be_class_tasmota (scope: global, name: Tasmota) {
    _fl, var                            // list of active fast-loop object (faster than drivers)
    _rules, var                         // list of active rules
    _timers, var                        // list of active timers
    _defer, var                         // list of deferred functions to be called at next millisecond
    _crons, var                         // list of active crons
    _ccmd, var                          // list of active Tasmota commands implemented in Berry
    _drivers, var                       // list of active drivers
    _ext, var                           // list of active extensions
    _wnu, var                           // list of closures to call when network is connected
    wire1, var                          // Tasmota I2C Wire1
    wire2, var                          // Tasmota I2C Wire2
    cmd_res, var                        // store the command result, nil if disables, true if capture enabled, contains return value
    global, var                         // mapping to TasmotaGlobal
    settings, var                       // mapping to Tasmota Settings
    wd, var                             // working dir
    _debug_present, var                 // is `import debug` present?

    _global_def, comptr(&be_tasmota_global_struct)
    _settings_def, comptr(&be_tasmota_settings_struct)
    _global_addr, comptr(&TasmotaGlobal)
    _settings_ptr, comptr(&Settings)

    init, closure(class_Tasmota_init_closure)

    get_free_heap, func(l_getFreeHeap)
    arch, static_func(l_arch)
    publish, func(be_mqtt_publish)
    publish_result, func(l_publish_result)
    publish_rule, func(l_publish_rule)
    _cmd, func(l_cmd)
    get_option, func(l_getoption)
    millis, func(l_millis)
    time_reached, func(l_timereached)
    rtc, static_func(l_rtc)
    rtc_utc, func(l_rtc_utc)
    time_dump, func(l_time_dump)
    strftime, func(l_strftime)
    strptime, func(l_strptime)
    memory, static_func(l_memory)
    wifi, static_func(l_wifi)
    eth, static_func(l_eth)
    hostname, func(l_hostname)
    yield, func(l_yield)
    delay, func(l_delay)
    delay_microseconds, func(l_delay_microseconds)
    scale_uint, static_func(l_scaleuint)
    scale_int, static_func(l_scaleint)
    sine_int, static_func(l_sineint)
    log, func(l_logInfo)
    loglevel, func(l_loglevel)
    save, func(l_save)
    locale, func(l_locale)

    read_sensors, func(l_read_sensors)

    resp_cmnd, func(l_respCmnd)
    resp_cmnd_str, func(l_respCmndStr)
    resp_cmnd_done, func(l_respCmndDone)
    resp_cmnd_error, func(l_respCmndError)
    resp_cmnd_failed, func(l_respCmndFailed)
    resolvecmnd, func(l_resolveCmnd)

    response_append, func(l_respAppend)
    web_send, func(l_webSend)
    web_send_decimal, func(l_webSendDecimal)
    webcolor, static_func(l_webcolor)

    int, static_closure(class_Tasmota_int_closure)

    get_power, func(l_getpower)
    set_power, func(l_setpower)
    get_switch, func(l_getswitch)     // deprecated
    get_switches, func(l_getswitch)

    i2c_enabled, func(l_i2cenabled)
    version, ctype_func(be_Tasmota_version)

    fast_loop, closure(class_Tasmota_fast_loop_closure)
    add_fast_loop, closure(class_Tasmota_add_fast_loop_closure)
    remove_fast_loop, closure(class_Tasmota_remove_fast_loop_closure)
    cmd, closure(class_Tasmota_cmd_closure)
    _find_op, func(tasm_find_op)        // new C version for finding a rule operator
    _apply_str_op, func(tasm_apply_str_op)
    find_key_i, closure(class_Tasmota_find_key_i_closure)
    find_list_i, closure(class_Tasmota_find_list_i_closure)
    find_op, closure(class_Tasmota_find_op_closure)
    add_rule, closure(class_Tasmota_add_rule_closure)
    add_rule_once, closure(class_Tasmota_add_rule_once_closure)
    remove_rule, closure(class_Tasmota_remove_rule_closure)
    try_rule, closure(class_Tasmota_try_rule_closure)
    exec_rules, closure(class_Tasmota_exec_rules_closure)
    exec_tele, closure(class_Tasmota_exec_tele_closure)
    set_timer, closure(class_Tasmota_set_timer_closure)
    run_timers, closure(class_Tasmota_run_timers_closure)
    defer, closure(class_Tasmota_defer_closure)
    run_deferred, closure(class_Tasmota_run_deferred_closure)
    remove_timer, closure(class_Tasmota_remove_timer_closure)
    add_cmd, closure(class_Tasmota_add_cmd_closure)
    remove_cmd, closure(class_Tasmota_remove_cmd_closure)
    exec_cmd, closure(class_Tasmota_exec_cmd_closure)
    gc, closure(class_Tasmota_gc_closure)
    event, closure(class_Tasmota_event_closure)
    is_network_up, closure(class_Tasmota_is_network_up_closure)
    when_network_up, closure(class_Tasmota_when_network_up_closure)
    run_network_up, closure(class_Tasmota_run_network_up_closure)
    add_driver, closure(class_Tasmota_add_driver_closure)
    remove_driver, closure(class_Tasmota_remove_driver_closure)
    add_extension, closure(class_Tasmota_add_extension_closure)
    read_extension_manifest, closure(class_Tasmota_read_extension_manifest_closure)
    unload_extension, closure(class_Tasmota_unload_extension_closure)
    load, closure(class_Tasmota_load_closure)
    compile, closure(class_Tasmota_compile_closure)
    wire_scan, closure(class_Tasmota_wire_scan_closure)
    time_str, closure(class_Tasmota_time_str_closure)
    urlfetch, closure(class_Tasmota_urlfetch_closure)
    urlfetch_cmd, closure(class_Tasmota_urlfetch_cmd_closure)
    urlbecload, static_ctype_func(BerryBECLoader)

    add_cron, closure(class_Tasmota_add_cron_closure)
    run_cron, closure(class_Tasmota_run_cron_closure)
    next_cron, closure(class_Tasmota_next_cron_closure)
    remove_cron, closure(class_Tasmota_remove_cron_closure)

    check_not_method, closure(class_Tasmota_check_not_method_closure)

    hs2rgb, closure(class_Tasmota_hs2rgb_closure)

    gen_cb, closure(class_Tasmota_gen_cb_closure)

    get_light, closure(class_Tasmota_get_light_closure)
    set_light, closure(class_Tasmota_set_light_closure)

    Rule_Matcher, class(be_class_Rule_Matcher)
}
@const_object_info_end */
