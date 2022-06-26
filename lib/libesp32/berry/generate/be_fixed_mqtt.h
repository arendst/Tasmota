#include "be_constobj.h"

static be_define_const_map_slots(m_libmqtt_map) {
    { be_const_key(unsubscribe, -1), be_const_closure(unsubscribe_closure) },
    { be_const_key(subscribe, -1), be_const_closure(subscribe_closure) },
    { be_const_key(publish, 3), be_const_func(be_mqtt_publish) },
    { be_const_key(_, -1), be_const_closure(__closure) },
    { be_const_key(_subscribe, -1), be_const_ctype_func(be_mqtt_subscribe) },
    { be_const_key(_unsubscribe, 1), be_const_ctype_func(be_mqtt_unsubscribe) },
};

static be_define_const_map(
    m_libmqtt_map,
    6
);

static be_define_const_module(
    m_libmqtt,
    "mqtt"
);

BE_EXPORT_VARIABLE be_define_const_native_module(mqtt);
