/******************************************************************************/
// softdevice_handler.c
//
// It seems in SDK 11 there is no way to compile softdevice_handler.c without
// being on a nRF51822. Given this, the options are to either:
//  - Not call any functions in softdevice_handler. This means editing
//    simple_ble.c to basically not initialize things. That is doable, but
//    not ideal as #ifdefs are EVIL and make unmaintainable garbage code.
//  - Put stub functions here and hope nothing bad happens.
/******************************************************************************/

#include "softdevice_handler.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "nordic_common.h"
#include "app_error.h"
#include "nrf_assert.h"
// #include "nrf_nvic.h"
#include "nrf.h"
#include "nrf_log.h"
#include "sdk_common.h"
#include "nrf_drv_config.h"
#if CLOCK_ENABLED
#include "nrf_drv_clock.h"
#endif

#if defined(ANT_STACK_SUPPORT_REQD) && defined(BLE_STACK_SUPPORT_REQD)
    #include "ant_interface.h"
#elif defined(ANT_STACK_SUPPORT_REQD)
    #include "ant_interface.h"
#elif defined(BLE_STACK_SUPPORT_REQD)
    #include "ble.h"
#endif


#if defined(NRF_LOG_USES_RTT) && NRF_LOG_USES_RTT == 1
#define SD_HANDLER_LOG(...) NRF_LOG_PRINTF(__VA_ARGS__)
#else
#define SD_HANDLER_LOG(...)
#endif

#if defined(NRF_LOG_USES_RTT) && NRF_LOG_USES_RTT == 1
#define SD_HANDLER_LOG_INIT() NRF_LOG_INIT()
#else
#define SD_HANDLER_LOG_INIT()
#endif



#define RAM_START_ADDRESS         0x20000000
// #define SOFTDEVICE_EVT_IRQ        SD_EVT_IRQn       /**< SoftDevice Event IRQ number. Used for both protocol events and SoC events. */
// #define SOFTDEVICE_EVT_IRQHandler SD_EVT_IRQHandler
// #define RAM_TOTAL_SIZE            ((NRF_FICR->INFO.RAM)*1024)
#define RAM_END_ADDRESS           (RAM_START_ADDRESS + RAM_TOTAL_SIZE)


#define SOFTDEVICE_VS_UUID_COUNT       0
#define SOFTDEVICE_GATTS_ATTR_TAB_SIZE BLE_GATTS_ATTR_TAB_SIZE_DEFAULT
#define SOFTDEVICE_GATTS_SRV_CHANGED   0
#define SOFTDEVICE_PERIPH_CONN_COUNT   1
#define SOFTDEVICE_CENTRAL_CONN_COUNT  4
#define SOFTDEVICE_CENTRAL_SEC_COUNT   1

/* Global nvic state instance, required by nrf_nvic.h */
// nrf_nvic_state_t nrf_nvic_state;

static softdevice_evt_schedule_func_t m_evt_schedule_func;              /**< Pointer to function for propagating SoftDevice events to the scheduler. */

static volatile bool                  m_softdevice_enabled = false;     /**< Variable to indicate whether the SoftDevice is enabled. */

#ifdef BLE_STACK_SUPPORT_REQD
// The following three definitions is needed only if BLE events are needed to be pulled from the stack.
static uint8_t                      * mp_ble_evt_buffer;                /**< Buffer for receiving BLE events from the SoftDevice. */
static uint16_t                       m_ble_evt_buffer_size;            /**< Size of BLE event buffer. */
static ble_evt_handler_t              m_ble_evt_handler;                /**< Application event handler for handling BLE events. */
#endif

#ifdef ANT_STACK_SUPPORT_REQD
// The following two definition is needed only if ANT events are needed to be pulled from the stack.
static ant_evt_t                      m_ant_evt_buffer;                 /**< Buffer for receiving ANT events from the SoftDevice. */
static ant_evt_handler_t              m_ant_evt_handler;                /**< Application event handler for handling ANT events.  */
#endif

static sys_evt_handler_t              m_sys_evt_handler;                /**< Application event handler for handling System (SOC) events.  */


/**@brief       Callback function for asserts in the SoftDevice.
 *
 * @details     A pointer to this function will be passed to the SoftDevice. This function will be
 *              called by the SoftDevice if certain unrecoverable errors occur within the
 *              application or SoftDevice.
 *
 *              See @ref nrf_fault_handler_t for more details.
 *
 * @param[in] id    Fault identifier. See @ref NRF_FAULT_IDS.
 * @param[in] pc    The program counter of the instruction that triggered the fault.
 * @param[in] info  Optional additional information regarding the fault. Refer to each fault
 *                  identifier for details.
 */
void softdevice_fault_handler(uint32_t id, uint32_t pc, uint32_t info)
{
    app_error_fault_handler(id, pc, info);
}


void intern_softdevice_events_execute(void)
{
    if (!m_softdevice_enabled)
    {
        // SoftDevice not enabled. This can be possible if the SoftDevice was enabled by the
        // application without using this module's API (i.e softdevice_handler_init)

        return;
    }
#if CLOCK_ENABLED
    bool no_more_soc_evts = false;
#else
    bool no_more_soc_evts = (m_sys_evt_handler == NULL);
#endif
#ifdef BLE_STACK_SUPPORT_REQD
    bool no_more_ble_evts = (m_ble_evt_handler == NULL);
#endif
#ifdef ANT_STACK_SUPPORT_REQD
    bool no_more_ant_evts = (m_ant_evt_handler == NULL);
#endif

    for (;;)
    {
        uint32_t err_code;

        if (!no_more_soc_evts)
        {
            uint32_t evt_id;

            // Pull event from SOC.
            err_code = sd_evt_get(&evt_id);

            if (err_code == NRF_ERROR_NOT_FOUND)
            {
                no_more_soc_evts = true;
            }
            else if (err_code != NRF_SUCCESS)
            {
                APP_ERROR_HANDLER(err_code);
            }
            else
            {
                // Call application's SOC event handler.
#if CLOCK_ENABLED
                nrf_drv_clock_on_soc_event(evt_id);
                if (m_sys_evt_handler)
                {
                    m_sys_evt_handler(evt_id);
                }
#else
                m_sys_evt_handler(evt_id);
#endif
            }
        }

#ifdef BLE_STACK_SUPPORT_REQD
        // Fetch BLE Events.
        if (!no_more_ble_evts)
        {
            // Pull event from stack
            uint16_t evt_len = m_ble_evt_buffer_size;

            err_code = sd_ble_evt_get(mp_ble_evt_buffer, &evt_len);
            if (err_code == NRF_ERROR_NOT_FOUND)
            {
                no_more_ble_evts = true;
            }
            else if (err_code != NRF_SUCCESS)
            {
                APP_ERROR_HANDLER(err_code);
            }
            else
            {
                // Call application's BLE stack event handler.
                m_ble_evt_handler((ble_evt_t *)mp_ble_evt_buffer);
            }
        }
#endif

#ifdef ANT_STACK_SUPPORT_REQD
        // Fetch ANT Events.
        if (!no_more_ant_evts)
        {
            // Pull event from stack
            err_code = sd_ant_event_get(&m_ant_evt_buffer.channel,
                                        &m_ant_evt_buffer.event,
                                        m_ant_evt_buffer.msg.evt_buffer);
            if (err_code == NRF_ERROR_NOT_FOUND)
            {
                no_more_ant_evts = true;
            }
            else if (err_code != NRF_SUCCESS)
            {
                APP_ERROR_HANDLER(err_code);
            }
            else
            {
                // Call application's ANT stack event handler.
                m_ant_evt_handler(&m_ant_evt_buffer);
            }
        }
#endif

        if (no_more_soc_evts)
        {
            // There are no remaining System (SOC) events to be fetched from the SoftDevice.
#if defined(ANT_STACK_SUPPORT_REQD) && defined(BLE_STACK_SUPPORT_REQD)
            // Check if there are any remaining BLE and ANT events.
            if (no_more_ble_evts && no_more_ant_evts)
            {
                break;
            }
#elif defined(BLE_STACK_SUPPORT_REQD)
            // Check if there are any remaining BLE events.
            if (no_more_ble_evts)
            {
                break;
            }
#elif defined(ANT_STACK_SUPPORT_REQD)
            // Check if there are any remaining ANT events.
            if (no_more_ant_evts)
            {
                break;
            }
#else
            // No need to check for BLE or ANT events since there is no support for BLE and ANT
            // required.
            break;
#endif
        }
    }
}

bool softdevice_handler_isEnabled(void)
{
    return m_softdevice_enabled;
}

uint32_t softdevice_handler_init(nrf_clock_lf_cfg_t	*           p_clock_lf_cfg,
                                 void *                         p_ble_evt_buffer,
                                 uint16_t                       ble_evt_buffer_size,
                                 softdevice_evt_schedule_func_t evt_schedule_func)
{
    uint32_t err_code;

    SD_HANDLER_LOG_INIT();

    // Save configuration.
#if defined (BLE_STACK_SUPPORT_REQD)
    // Check that buffer is not NULL.
    if (p_ble_evt_buffer == NULL)
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    // Check that buffer is correctly aligned.
    if (!is_word_aligned(p_ble_evt_buffer))
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    mp_ble_evt_buffer     = (uint8_t *)p_ble_evt_buffer;
    m_ble_evt_buffer_size = ble_evt_buffer_size;
#else
    // The variables p_ble_evt_buffer and ble_evt_buffer_size is not needed if BLE Stack support
    // is not required.
    UNUSED_PARAMETER(p_ble_evt_buffer);
    UNUSED_PARAMETER(ble_evt_buffer_size);
#endif

    m_evt_schedule_func = evt_schedule_func;

    // Initialize SoftDevice.
#if defined(S212) || defined(S332)
    err_code = sd_softdevice_enable(p_clock_lf_cfg, softdevice_fault_handler, ANT_LICENSE_KEY);
#else
    err_code = sd_softdevice_enable(p_clock_lf_cfg, softdevice_fault_handler);
#endif
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    m_softdevice_enabled = true;

    return NRF_SUCCESS;
}


uint32_t softdevice_handler_sd_disable(void)
{
    uint32_t err_code = sd_softdevice_disable();

    m_softdevice_enabled = !(err_code == NRF_SUCCESS);

    return err_code;
}


#ifdef BLE_STACK_SUPPORT_REQD
uint32_t softdevice_ble_evt_handler_set(ble_evt_handler_t ble_evt_handler)
{
    VERIFY_PARAM_NOT_NULL(ble_evt_handler);

    m_ble_evt_handler = ble_evt_handler;

    return NRF_SUCCESS;
}
#endif


#ifdef ANT_STACK_SUPPORT_REQD
uint32_t softdevice_ant_evt_handler_set(ant_evt_handler_t ant_evt_handler)
{
    VERIFY_PARAM_NOT_NULL(ant_evt_handler);

    m_ant_evt_handler = ant_evt_handler;

    return NRF_SUCCESS;
}
#endif


uint32_t softdevice_sys_evt_handler_set(sys_evt_handler_t sys_evt_handler)
{
    VERIFY_PARAM_NOT_NULL(sys_evt_handler);

    m_sys_evt_handler = sys_evt_handler;

    return NRF_SUCCESS;
}


/**@brief   Function for handling the Application's BLE Stack events interrupt.
 *
 * @details This function is called whenever an event is ready to be pulled.
 */
void SOFTDEVICE_EVT_IRQHandler(void)
{
    if (m_evt_schedule_func != NULL)
    {
        uint32_t err_code = m_evt_schedule_func();
        APP_ERROR_CHECK(err_code);
    }
    else
    {
        intern_softdevice_events_execute();
    }
}

#if defined(BLE_STACK_SUPPORT_REQD)
uint32_t softdevice_enable_get_default_config(uint8_t central_links_count,
                                              uint8_t periph_links_count,
                                              ble_enable_params_t * p_ble_enable_params)
{
    memset(p_ble_enable_params, 0, sizeof(ble_enable_params_t));
    p_ble_enable_params->common_enable_params.vs_uuid_count   = 1;
    p_ble_enable_params->gatts_enable_params.attr_tab_size    = SOFTDEVICE_GATTS_ATTR_TAB_SIZE;
    p_ble_enable_params->gatts_enable_params.service_changed  = SOFTDEVICE_GATTS_SRV_CHANGED;
    p_ble_enable_params->gap_enable_params.periph_conn_count  = periph_links_count;
    p_ble_enable_params->gap_enable_params.central_conn_count = central_links_count;
    if (p_ble_enable_params->gap_enable_params.central_conn_count != 0)
    {
        p_ble_enable_params->gap_enable_params.central_sec_count  = SOFTDEVICE_CENTRAL_SEC_COUNT;
    }

    return NRF_SUCCESS;
}


#if defined(NRF_LOG_USES_RTT) && NRF_LOG_USES_RTT == 1
static inline uint32_t ram_total_size_get(void)
{
#ifdef NRF51
    uint32_t size_ram_blocks = (uint32_t)NRF_FICR->SIZERAMBLOCKS;
    uint32_t total_ram_size = size_ram_blocks;
    total_ram_size = total_ram_size*(NRF_FICR->NUMRAMBLOCK);
    return total_ram_size;
#elif defined (NRF52)
    return RAM_TOTAL_SIZE;
#endif /* NRF51 */
}

/*lint --e{528} -save suppress 528: symbol not referenced */
/**@brief   Function for finding the end address of the RAM.
 *
 * @retval  ram_end_address  Address of the end of the RAM.
 */
static inline uint32_t ram_end_address_get(void)
{
    uint32_t ram_end_address = (uint32_t)RAM_START_ADDRESS;
    ram_end_address+= ram_total_size_get();
    return ram_end_address;
}
/*lint -restore*/
#endif  //ENABLE_DEBUG_LOG_SUPPORT

/*lint --e{10} --e{19} --e{27} --e{40} --e{529} -save suppress Error 27: Illegal character */
uint32_t sd_check_ram_start(__attribute__ ((unused)) uint32_t sd_req_ram_start)
{
// #if (defined(S130) || defined(S132) || defined(S332))
// #if defined ( __CC_ARM )
//     extern uint32_t Image$$RW_IRAM1$$Base;
//     const volatile uint32_t ram_start = (uint32_t) &Image$$RW_IRAM1$$Base;
// #elif defined ( __ICCARM__ )
//     extern uint32_t __ICFEDIT_region_RAM_start__;
//     volatile uint32_t ram_start = (uint32_t) &__ICFEDIT_region_RAM_start__;
// #elif defined   ( __GNUC__ )
//     extern uint32_t __data_start__;
//     volatile uint32_t ram_start = (uint32_t) &__data_start__;
// #endif//__CC_ARM
//     if (ram_start != sd_req_ram_start)
//     {
// #if defined(NRF_LOG_USES_RTT) && NRF_LOG_USES_RTT == 1
//         uint32_t app_ram_size= ram_end_address_get();
//         SD_HANDLER_LOG("RAM START ADDR 0x%x should be adjusted to 0x%x\r\n",
//                   ram_start,
//                   sd_req_ram_start);
//         app_ram_size -= sd_req_ram_start;
//         SD_HANDLER_LOG("RAM SIZE should be adjusted to 0x%x \r\n",
//                   app_ram_size);
// #endif //NRF_LOG_USES_RTT
//         return NRF_SUCCESS;
//     }
// #endif//defined(S130) || defined(S132) || defined(S332)
    return NRF_SUCCESS;
}

uint32_t softdevice_enable(ble_enable_params_t * p_ble_enable_params)
{
// #if (defined(S130) || defined(S132) || defined(S332))
    uint32_t err_code;
//     uint32_t app_ram_base;

// #if defined ( __CC_ARM )
//     extern uint32_t Image$$RW_IRAM1$$Base;
//     const volatile uint32_t ram_start = (uint32_t) &Image$$RW_IRAM1$$Base;
// #elif defined ( __ICCARM__ )
//     extern uint32_t __ICFEDIT_region_RAM_start__;
//     volatile uint32_t ram_start = (uint32_t) &__ICFEDIT_region_RAM_start__;
// #elif defined   ( __GNUC__ )
//     extern uint32_t __data_start__;
//     volatile uint32_t ram_start = (uint32_t) &__data_start__;
// #endif

//     app_ram_base = ram_start;
//     SD_HANDLER_LOG("sd_ble_enable: RAM START at 0x%x\r\n",
//                     app_ram_base);
    // err_code = sd_ble_enable(p_ble_enable_params, &app_ram_base);
    err_code = sd_ble_enable(p_ble_enable_params, NULL);

// #if defined(NRF_LOG_USES_RTT) && NRF_LOG_USES_RTT == 1
//     if (app_ram_base != ram_start)
//     {
//         uint32_t app_ram_size= ram_end_address_get();
//         SD_HANDLER_LOG("sd_ble_enable: app_ram_base should be adjusted to 0x%x\r\n",
//                   app_ram_base);
//         app_ram_size -= app_ram_base;
//         SD_HANDLER_LOG("ram size should be adjusted to 0x%x \r\n",
//                   app_ram_size);
//     }
//     else if (err_code != NRF_SUCCESS)
//     {
//         SD_HANDLER_LOG("sd_ble_enable: error 0x%x\r\n", err_code);
//         while(1);
//     }
// #endif   // NRF_LOG_USES_RTT
    return err_code;
// #else
//     return NRF_SUCCESS;
// #endif   //defined(S130) || defined(S132) || defined(S332)

}
/*lint -restore*/

#endif //BLE_STACK_SUPPORT_REQD
