/********************************************************************
 * Berry class `mqttclient`
 *
 * Independent MQTT client for Berry code and applications (ESP32 only)
 * Allows connecting to a separate MQTT broker with own credentials.
 *******************************************************************/

#ifdef USE_BERRY_MQTTCLIENT

#include "be_constobj.h"

extern int be_mqttc_init(bvm *vm);
extern int be_mqttc_deinit(bvm *vm);
extern int be_mqttc_connect(bvm *vm);
extern int be_mqttc_disconnect(bvm *vm);
extern int be_mqttc_publish(bvm *vm);
extern int be_mqttc_subscribe(bvm *vm);
extern int be_mqttc_unsubscribe(bvm *vm);
extern int be_mqttc_loop(bvm *vm);
extern int be_mqttc_connected(bvm *vm);
extern int be_mqttc_state(bvm *vm);
extern int be_mqttc_set_on_message(bvm *vm);
extern int be_mqttc_set_on_connect(bvm *vm);
extern int be_mqttc_set_auto_reconnect(bvm *vm);

#include "be_fixed_be_class_mqttclient.h"

/* @const_object_info_begin
class be_class_mqttclient (scope: global, name: mqttclient) {
    .p, var
    .on_message, var
    .on_connect, var
    init, func(be_mqttc_init)
    deinit, func(be_mqttc_deinit)
    connect, func(be_mqttc_connect)
    disconnect, func(be_mqttc_disconnect)
    publish, func(be_mqttc_publish)
    subscribe, func(be_mqttc_subscribe)
    unsubscribe, func(be_mqttc_unsubscribe)
    loop, func(be_mqttc_loop)
    connected, func(be_mqttc_connected)
    state, func(be_mqttc_state)
    set_on_message, func(be_mqttc_set_on_message)
    set_on_connect, func(be_mqttc_set_on_connect)
    set_auto_reconnect, func(be_mqttc_set_auto_reconnect)
}
@const_object_info_end */

#endif // USE_BERRY_MQTTCLIENT
