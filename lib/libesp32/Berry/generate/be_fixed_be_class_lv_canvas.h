#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_canvas_map) {
    { be_const_key(init, 16), be_const_func(lvbe_canvas_create) },
    { be_const_key(set_palette, -1), be_const_func(lvbe_canvas_set_palette) },
    { be_const_key(tostring, -1), be_const_func(lvx_tostring) },
    { be_const_key(draw_img, -1), be_const_func(lvbe_canvas_draw_img) },
    { be_const_key(dot_p, -1), be_const_int(0) },
    { be_const_key(copy_buf, 6), be_const_func(lvbe_canvas_copy_buf) },
    { be_const_key(create, 17), be_const_func(lvbe_canvas_create) },
    { be_const_key(draw_arc, 10), be_const_func(lvbe_canvas_draw_arc) },
    { be_const_key(get_px, 7), be_const_func(lvbe_canvas_get_px) },
    { be_const_key(transform, -1), be_const_func(lvbe_canvas_transform) },
    { be_const_key(draw_text, 3), be_const_func(lvbe_canvas_draw_text) },
    { be_const_key(fill_bg, -1), be_const_func(lvbe_canvas_fill_bg) },
    { be_const_key(draw_polygon, -1), be_const_func(lvbe_canvas_draw_polygon) },
    { be_const_key(blur_ver, 0), be_const_func(lvbe_canvas_blur_ver) },
    { be_const_key(blur_hor, -1), be_const_func(lvbe_canvas_blur_hor) },
    { be_const_key(draw_line, -1), be_const_func(lvbe_canvas_draw_line) },
    { be_const_key(draw_rect, -1), be_const_func(lvbe_canvas_draw_rect) },
    { be_const_key(set_buffer, -1), be_const_func(lvbe_canvas_set_buffer) },
    { be_const_key(set_px, -1), be_const_func(lvbe_canvas_set_px) },
};

static be_define_const_map(
    be_class_lv_canvas_map,
    19
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_canvas,
    1,
    (bclass *)&be_class_lv_obj,
    lv_canvas
);
