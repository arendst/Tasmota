/********************************************************************
 * Berry module `mqtt`
 * 
 *******************************************************************/

#include "be_constobj.h"
#include "be_mapping.h"

extern int be_mqtt_publish(bvm *vm);

extern void be_mqtt_subscribe(const char* topic);     BE_FUNC_CTYPE_DECLARE(be_mqtt_subscribe, "", "-s")
extern void be_mqtt_unsubscribe(const char* topic);   BE_FUNC_CTYPE_DECLARE(be_mqtt_unsubscribe, "", "-s")
extern bbool be_mqtt_connected(void);   BE_FUNC_CTYPE_DECLARE(be_mqtt_connected, "b", "-")

/* @const_object_info_begin
class be_class_MQTT_ntv (scope: global, name: MQTT_ntv) {
  publish, func(be_mqtt_publish)
  _subscribe, ctype_func(be_mqtt_subscribe)
  _unsubscribe, ctype_func(be_mqtt_unsubscribe)
  connected, ctype_func(be_mqtt_connected)
}
@const_object_info_end */

#include "be_fixed_be_class_MQTT_ntv.h"

// mqtt module
extern const bclass be_class_MQTT;
static int mqtt_init(bvm *vm) {
  be_pushntvclass(vm, &be_class_MQTT);
  be_call(vm, 0);
  be_return(vm);
}

#include "be_fixed_mqtt.h"
/* @const_object_info_begin
module mqtt (scope: global) {
  init, func(mqtt_init)
}
@const_object_info_end */

// private class MQTT
#include "solidify/solidified_mqtt.h"
