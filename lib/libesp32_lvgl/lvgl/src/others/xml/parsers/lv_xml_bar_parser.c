/**
 * @file lv_xml_bar_parser.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_xml_bar_parser.h"
#if LV_USE_XML

#include "../../../lvgl.h"
#include "../../../lvgl_private.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static lv_bar_orientation_t orentation_text_to_enum_value(const char * txt);
static lv_bar_mode_t mode_text_to_enum_value(const char * txt);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void * lv_xml_bar_create(lv_xml_parser_state_t * state, const char ** attrs)
{
    LV_UNUSED(attrs);

    void * item = lv_bar_create(lv_xml_state_get_parent(state));
    return item;
}

void lv_xml_bar_apply(lv_xml_parser_state_t * state, const char ** attrs)
{
    void * item = lv_xml_state_get_item(state);
    lv_xml_obj_apply(state, attrs); /*Apply the common properties, e.g. width, height, styles flags etc*/

    for(int i = 0; attrs[i]; i += 2) {
        const char * name = attrs[i];
        const char * value = attrs[i + 1];

        if(lv_streq("value", name)) {
            char buf[64];
            lv_strlcpy(buf, value, sizeof(buf));
            char * buf_p = buf;
            int32_t v1 = lv_xml_atoi(lv_xml_split_str(&buf_p, ' '));
            bool v2 = lv_xml_to_bool(buf_p);
            lv_bar_set_value(item, v1, v2);
        }
        if(lv_streq("start_value", name)) {
            char buf[64];
            lv_strlcpy(buf, value, sizeof(buf));
            char * buf_p = buf;
            int32_t v1 = lv_xml_atoi(lv_xml_split_str(&buf_p, ' '));
            bool v2 = lv_xml_to_bool(buf_p);
            lv_bar_set_start_value(item, v1, v2);
        }
        if(lv_streq("orientation", name)) lv_bar_set_orientation(item, orentation_text_to_enum_value(value));
        if(lv_streq("mode", name)) lv_bar_set_mode(item, mode_text_to_enum_value(value));
        if(lv_streq("range", name)) {
            char buf[64];
            lv_strlcpy(buf, value, sizeof(buf));
            char * buf_p = buf;
            int32_t v1 = lv_xml_atoi(lv_xml_split_str(&buf_p, ' '));
            int32_t v2 = lv_xml_atoi(buf_p);
            lv_bar_set_range(item, v1, v2);
        }
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static lv_bar_orientation_t orentation_text_to_enum_value(const char * txt)
{
    if(lv_streq("auto", txt)) return LV_BAR_ORIENTATION_AUTO;
    if(lv_streq("horizontal", txt)) return LV_BAR_ORIENTATION_HORIZONTAL;
    if(lv_streq("vertical", txt)) return LV_BAR_ORIENTATION_VERTICAL;

    LV_LOG_WARN("%s is an unknown value for bar's orientation", txt);
    return 0; /*Return 0 in lack of a better option. */
}

static lv_bar_mode_t mode_text_to_enum_value(const char * txt)
{
    if(lv_streq("normal", txt)) return LV_BAR_MODE_NORMAL;
    if(lv_streq("range", txt)) return LV_BAR_MODE_RANGE;
    if(lv_streq("symmetrical", txt)) return LV_BAR_MODE_SYMMETRICAL;

    LV_LOG_WARN("%s is an unknown value for bar's mode", txt);
    return 0; /*Return 0 in lack of a better option. */
}

#endif /* LV_USE_XML */
