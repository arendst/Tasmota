// Need to make this copy because
//
//      current_int_priority_get()
//
// includes nRF specific code.

#ifndef APP_UTIL_PLATFORM_H__
#define APP_UTIL_PLATFORM_H__

void critical_region_enter (void);
void critical_region_exit (void);

#define CRITICAL_REGION_ENTER() critical_region_enter()
#define CRITICAL_REGION_EXIT() critical_region_exit()

static __INLINE uint8_t current_int_priority_get(void) {
	return 4; // APP_IRQ_PRIORITY_THREAD
}

#endif
