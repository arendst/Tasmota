#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_calendar_map) {
    { be_const_key(create, -1), be_const_func(lvbe_calendar_create) },
    { be_const_key(get_day_of_week, -1), be_const_func(lvbe_calendar_get_day_of_week) },
    { be_const_key(get_highlighted_dates_num, 5), be_const_func(lvbe_calendar_get_highlighted_dates_num) },
    { be_const_key(tostring, -1), be_const_func(lvx_tostring) },
    { be_const_key(set_highlighted_dates, -1), be_const_func(lvbe_calendar_set_highlighted_dates) },
    { be_const_key(set_showed_date, -1), be_const_func(lvbe_calendar_set_showed_date) },
    { be_const_key(set_day_names, -1), be_const_func(lvbe_calendar_set_day_names) },
    { be_const_key(set_today_date, -1), be_const_func(lvbe_calendar_set_today_date) },
    { be_const_key(dot_p, -1), be_const_int(0) },
    { be_const_key(init, 4), be_const_func(lvbe_calendar_create) },
    { be_const_key(set_month_names, -1), be_const_func(lvbe_calendar_set_month_names) },
};

static be_define_const_map(
    be_class_lv_calendar_map,
    11
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_calendar,
    1,
    (bclass *)&be_class_lv_obj,
    lv_calendar
);
