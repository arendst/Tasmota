/********************************************************************
 * Berry module `mqtt`
 * 
 *******************************************************************/

#include "be_constobj.h"
#include "be_mapping.h"

extern int be_mqtt_publish(bvm *vm);

extern void be_mqtt_subscribe(const char* topic);     BE_FUNC_CTYPE_DECLARE(be_mqtt_subscribe, "", "s")
extern void be_mqtt_unsubscribe(const char* topic);   BE_FUNC_CTYPE_DECLARE(be_mqtt_unsubscribe, "", "s")

/* @const_object_info_begin
module mqtt (scope: global) {
  publish, func(be_mqtt_publish)
  subscribe, ctype_func(be_mqtt_subscribe)
  unsubscribe, ctype_func(be_mqtt_unsubscribe)
}
@const_object_info_end */
#include "be_fixed_mqtt.h"
