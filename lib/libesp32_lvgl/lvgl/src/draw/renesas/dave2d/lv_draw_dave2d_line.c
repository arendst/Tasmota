#include "lv_draw_dave2d.h"
#if LV_USE_DRAW_DAVE2D

void lv_draw_dave2d_line(lv_draw_dave2d_unit_t * u, const lv_draw_line_dsc_t * dsc)
{

    lv_area_t clip_line;
    d2_u32     mode;
    d2_s32 result;
    lv_area_t buffer_area;
    uint32_t res;
    lv_value_precise_t p1_x;
    lv_value_precise_t p1_y;
    lv_value_precise_t p2_x;
    lv_value_precise_t p2_y;
    int32_t x;
    int32_t y;

    clip_line.x1 = LV_MIN(dsc->p1.x, dsc->p2.x) - dsc->width / 2;
    clip_line.x2 = LV_MAX(dsc->p1.x, dsc->p2.x) + dsc->width / 2;
    clip_line.y1 = LV_MIN(dsc->p1.y, dsc->p2.y) - dsc->width / 2;
    clip_line.y2 = LV_MAX(dsc->p1.y, dsc->p2.y) + dsc->width / 2;

    bool is_common;
    is_common = _lv_area_intersect(&clip_line, &clip_line, u->base_unit.clip_area);
    if(!is_common) return;

#if LV_USE_OS
    lv_result_t  status;
    status = lv_mutex_lock(u->pd2Mutex);
    if(LV_RESULT_OK != status) {
        __BKPT(0);
    }
#endif

    buffer_area = u->base_unit.target_layer->buf_area;
    p1_x = dsc->p1.x - buffer_area.x1;
    p1_y = dsc->p1.y - buffer_area.y1;
    p2_x = dsc->p2.x - buffer_area.x1;
    p2_y = dsc->p2.y - buffer_area.y1;

    x = 0 - u->base_unit.target_layer->buf_area.x1;
    y = 0 - u->base_unit.target_layer->buf_area.y1;

    lv_area_move(&clip_line, x, y);
    lv_area_move(&buffer_area, x, y);

    bool dashed = dsc->dash_gap && dsc->dash_width;

    if(dashed) {
        /* TODO */
        __BKPT(0);
    }

#if D2_RENDER_EACH_OPERATION
    d2_selectrenderbuffer(u->d2_handle, u->renderbuffer);
#endif
    //
    // Generate render operations
    //
    d2_framebuffer_from_layer(u->d2_handle, u->base_unit.target_layer);

    d2_setcolor(u->d2_handle, 0, lv_draw_dave2d_lv_colour_to_d2_colour(dsc->color));

    d2_setalpha(u->d2_handle, dsc->opa);

    d2_cliprect(u->d2_handle, clip_line.x1, clip_line.y1, clip_line.x2, clip_line.y2);

    if((dsc->round_end == 1) || (dsc->round_start == 1)) {
        mode = d2_lc_round;
    }
    else {
        mode = d2_lc_butt; // lines end directly at endpoints
    }

    d2_setlinecap(u->d2_handle, mode);

    d2_renderline(u->d2_handle, D2_FIX4(p1_x), D2_FIX4(p1_y), D2_FIX4(p2_x),
                  D2_FIX4(p2_y), D2_FIX4(dsc->width), d2_le_exclude_none);

    //
    // Execute render operations
    //
#if D2_RENDER_EACH_OPERATION
    d2_executerenderbuffer(u->d2_handle, u->renderbuffer, 0);
    d2_flushframe(u->d2_handle);
#endif

#if LV_USE_OS
    status = lv_mutex_unlock(u->pd2Mutex);
    if(LV_RESULT_OK != status) {
        __BKPT(0);
    }
#endif
}

#endif /*LV_USE_DRAW_DAVE2D*/
