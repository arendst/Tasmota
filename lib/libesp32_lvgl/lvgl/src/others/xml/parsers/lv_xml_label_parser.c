/**
 * @file lv_xml_label_parser.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_xml_label_parser.h"
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
static lv_label_long_mode_t long_mode_text_to_enum_value(const char * txt);
static void free_fmt_event_cb(lv_event_t * e);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void * lv_xml_label_create(lv_xml_parser_state_t * state, const char ** attrs)
{
    LV_UNUSED(attrs);
    void * item = lv_label_create(lv_xml_state_get_parent(state));

    return item;
}

void lv_xml_label_apply(lv_xml_parser_state_t * state, const char ** attrs)
{
    void * item = lv_xml_state_get_item(state);

    lv_xml_obj_apply(state, attrs); /*Apply the common properties, e.g. width, height, styles flags etc*/

    for(int i = 0; attrs[i]; i += 2) {
        const char * name = attrs[i];
        const char * value = attrs[i + 1];

        if(lv_streq("text", name)) lv_label_set_text(item, value);
        if(lv_streq("long_mode", name)) lv_label_set_long_mode(item, long_mode_text_to_enum_value(value));
        if(lv_streq("bind_text", name)) {
            char buf[256];
            lv_strncpy(buf, value, sizeof(buf));
            char * bufp = buf;
            char * subject_name = lv_xml_split_str(&bufp, ' ');
            if(subject_name) {
                lv_subject_t * subject = lv_xml_get_subject(&state->scope, subject_name);
                if(subject) {
                    char * fmt = bufp; /*The second part is the format text*/
                    if(fmt && fmt[0] == '\0') fmt = NULL;
                    if(fmt) {
                        if(fmt[0] == '\'') fmt++;
                        size_t fmt_len = lv_strlen(fmt);
                        if(fmt_len != 0 && fmt[fmt_len - 1] == '\'') fmt[fmt_len - 1] = '\0';
                        fmt = lv_strdup(fmt);
                        lv_obj_add_event_cb(item, free_fmt_event_cb, LV_EVENT_DELETE, fmt);
                    }
                    lv_label_bind_text(item, subject, fmt);
                }
                else {
                    LV_LOG_WARN("Subject \"%s\" doesn't exist in label bind_text", value);
                }
            }
        }
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static lv_label_long_mode_t long_mode_text_to_enum_value(const char * txt)
{
    if(lv_streq("wrap", txt)) return LV_LABEL_LONG_MODE_WRAP;
    if(lv_streq("scroll", txt)) return LV_LABEL_LONG_MODE_SCROLL;
    if(lv_streq("scroll_circular", txt)) return LV_LABEL_LONG_MODE_SCROLL_CIRCULAR;
    if(lv_streq("dots", txt)) return LV_LABEL_LONG_MODE_DOTS;
    if(lv_streq("clip", txt)) return LV_LABEL_LONG_MODE_CLIP;

    LV_LOG_WARN("%s is an unknown value for label's long_mode", txt);
    return 0; /*Return 0 in lack of a better option. */
}

static void free_fmt_event_cb(lv_event_t * e)
{
    void * fmt = lv_event_get_user_data(e);
    lv_free(fmt);
}

#endif /* LV_USE_XML */
