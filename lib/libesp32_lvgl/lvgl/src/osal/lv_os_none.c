/**
 * @file lv_os_none.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#include "../lv_conf_internal.h"
#if LV_USE_OS == LV_OS_NONE

#include "lv_os.h"
#include "../misc/lv_timer.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

uint32_t lv_os_get_idle_percent(void)
{
    return lv_timer_get_idle();
}

/**********************
 *   STATIC FUNCTIONS
 **********************/


#endif
