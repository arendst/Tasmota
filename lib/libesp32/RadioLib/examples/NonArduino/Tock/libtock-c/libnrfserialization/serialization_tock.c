#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include <timer.h>
#include <gpio.h>

#include "nrf.h"
#include "ser_phy.h"
#include "ser_config.h"
#include "nrf_error.h"
#include "nordic_common.h"
#include "app_timer.h"

#include "nrf51_serialization.h"
#include "ble_serialization.h"
#include "ser_sd_transport.h"

// Circular queue for event packets. Event packets are generated asynchronously
// from the nRF (e.g. advertisement discovery or read requests).
#define RX_BUFFER_SIZE 5
static uint8_t rx_event_buffers[RX_BUFFER_SIZE][SER_HAL_TRANSPORT_RX_MAX_PKT_SIZE];
static uint8_t _head_event = 0;
static uint8_t _tail_event = 0;

// Single entry queue for receiving response packets after sending commands.
static uint8_t rx_rsp_buffer[SER_HAL_TRANSPORT_RX_MAX_PKT_SIZE];
static bool _have_rsp_packet = false;

// Buffer to receive packets from the nrf51 in.
// The upper layer also has a buffer, which we could use, but to make
// the timing work out better we just keep a buffer around that the kernel
// can keep a pointer to.
static uint8_t rx[SER_HAL_TRANSPORT_RX_MAX_PKT_SIZE];
// This is a pointer to the RX buffer passed in by the upper serialization
// layer.
static uint8_t* hal_rx_buf = NULL;

// Buffer to create an outgoing packet into.
static uint8_t tx[SER_HAL_TRANSPORT_TX_MAX_PKT_SIZE];
// Length of the outgoing packet.
static uint16_t tx_len = 0;

// Callback that we pass TX done and RX events to
static ser_phy_events_handler_t _ser_phy_event_handler;
// Data structure that we pass for receive events
static ser_phy_evt_t _ser_phy_rx_event;
// Data structure for TX events.
static ser_phy_evt_t _ser_phy_tx_event;
// Flag so we don't overwhelm the serialization library
static bool _receiving_packet = false;
// Need to call back into the nordic library after getting a buffer.
static bool _need_wakeup = false;
// Whether there are multiple packets in a single UART receive callback that
// need to be processed.
static bool _queued_packets = false;
// Timer to detect when we fail to get a response message after sending a
// command.
static tock_timer_t* _timeout_timer = NULL;
// yield() variable.
static bool nrf_serialization_done = false;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

void timeout_timer_cb (int a, int b, int c, void* ud);
void ble_serialization_callback (int callback_type, int rx_len, int c, void* other);

uint32_t sd_app_evt_wait (void);
void serialization_timer_cb (int a, int b, int c, void* timer_id);

uint32_t ser_app_hal_hw_init (void);
void ser_app_hal_delay (uint32_t ms);
void ser_app_hal_nrf_reset_pin_clear (void);
void ser_app_hal_nrf_reset_pin_set (void);
void ser_app_hal_nrf_evt_irq_priority_set (void);
void ser_app_hal_nrf_evt_pending (void);

void ser_app_power_system_off_set (void);
bool ser_app_power_system_off_get (void);
void ser_app_power_system_off_enter (void);
void critical_region_enter (void);
void critical_region_exit (void);

/*******************************************************************************
 * Callback from the UART layer in the kernel
 ******************************************************************************/

void timeout_timer_cb (int a, int b, int c, void* ud) {
    UNUSED_PARAMETER(a);
    UNUSED_PARAMETER(b);
    UNUSED_PARAMETER(c);
    UNUSED_PARAMETER(ud);

    // Uh oh did not get a response to a command packet.
    // Send up an error.
    _ser_phy_rx_event.evt_type = SER_PHY_EVT_HW_ERROR;
    if (_ser_phy_event_handler) {
        _ser_phy_event_handler(_ser_phy_rx_event);
    }
}

void ble_serialization_callback (int callback_type, int rx_len, int c, void* other) {
    UNUSED_PARAMETER(c);
    UNUSED_PARAMETER(other);

    nrf_serialization_done = true;
    if (callback_type == 1) {
        // TX DONE

        // Reset that we are no longer sending a packet.
        tx_len = 0;

        // Notify the upper layer
        _ser_phy_tx_event.evt_type = SER_PHY_EVT_TX_PKT_SENT;

        if (_ser_phy_event_handler) {
            _ser_phy_event_handler(_ser_phy_tx_event);
        }

    } else if (callback_type == 4) {
        // RX entire buffer

        // Copy all received packets into our circular buffer
        int offset = 0;
        while (1 && (rx_len - offset >= SER_PHY_HEADER_SIZE)) {
            int pktlen = (rx[offset] | rx[offset+1] << 8) + SER_PHY_HEADER_SIZE;
            // Make sure that pktlen is reasonable
            if (pktlen > (int) SER_HAL_TRANSPORT_RX_MAX_PKT_SIZE ||
                pktlen+offset > (int) SER_HAL_TRANSPORT_RX_MAX_PKT_SIZE ||
                pktlen > rx_len - offset) {
                // Too big to copy, something went wrong.
                break;
            }

            // Check which type of packet this is.
            uint8_t packet_type = rx[offset+2];
            switch (packet_type) {
                case SER_PKT_TYPE_RESP:
                case SER_PKT_TYPE_DTM_RESP:
                    if (_have_rsp_packet) {
                        printf("Already have response packet?\n");
                    } else {
                        _have_rsp_packet = true;
                        memcpy(rx_rsp_buffer, rx+offset, pktlen);

                        // Got a response, cancel any pending timer
                        if (_timeout_timer != NULL) {
                            timer_cancel(_timeout_timer);
                            free(_timeout_timer);
                            _timeout_timer = NULL;
                        }
                    }
                    break;

                case SER_PKT_TYPE_EVT:
                    // Check if there is room
                    if ((_tail_event + 1) % RX_BUFFER_SIZE == _head_event) {
                        // No empty slots in the queue
                        break;
                    }
                    // Copy into open slot and increment the pointer.
                    memcpy(rx_event_buffers[_tail_event], rx+offset, pktlen);
                    _tail_event = (_tail_event + 1) % RX_BUFFER_SIZE;
                    break;
            }

            // Check for another packet in the buffer.
            offset += pktlen;
            if (rx_len <= offset) {
                break;
            }
        }

        // Only pass this buffer up if we don't have any others in flight. We
        // can only ask for one buffer from serialization at a time.
        if (!_receiving_packet) {
            uint16_t buf_len = 0;

            // Make sure we have a packet in the queue to send to the serialization
            // library.
            if (_have_rsp_packet) {
                buf_len = (rx_rsp_buffer[0] | rx_rsp_buffer[1] << 8);
            } else if (!ser_sd_transport_is_busy() && _head_event != _tail_event) {
                // DO NOT PROCESS EVENTS IF THERE IS A CMD/RSP PAIR STILL
                // OUTSTANDING. Processing an events can generate a new command
                // and things break. We use ser_sd_transport_is_busy() to do
                // this check because it is essentially contingent on there
                // being an outstanding response for a request.
                buf_len = (rx_event_buffers[_head_event][0] | rx_event_buffers[_head_event][1] << 8);
            }

            if (buf_len > 0) {
                // Need a dummy request for a buffer to keep the state machines
                // in the serialization library happy. We do use this buffer, but
                // we don't block packet receive until we get it.
                _ser_phy_rx_event.evt_type = SER_PHY_EVT_RX_BUF_REQUEST;
                _ser_phy_rx_event.evt_params.rx_buf_request.num_of_bytes = buf_len;

                if (_ser_phy_event_handler) {
                    // Need to mark things as busy on this end. The serialization state
                    // machine does not like it if you do things out of order, so just
                    // enforce some sanity on our end.
                    _receiving_packet = true;
                    _ser_phy_event_handler(_ser_phy_rx_event);
                }
            }
        }

    } else if (callback_type == 17) {
        // Great, we have a serialization approved buffer to use.

        // Check that we actually have a buffer to pass to the upper layers.
        // This buffer MUST be the same buffer that it passed us.
        if (hal_rx_buf) {
            uint16_t buf_len = 0;

            if (_have_rsp_packet) {
                _have_rsp_packet = false;
                buf_len = (rx_rsp_buffer[0] | rx_rsp_buffer[1] << 8);
                memcpy(hal_rx_buf, rx_rsp_buffer+SER_PHY_HEADER_SIZE, buf_len);

            } else {
                buf_len = rx_event_buffers[_head_event][0] | (((uint16_t) rx_event_buffers[_head_event][1]) << 8);
                memcpy(hal_rx_buf, rx_event_buffers[_head_event]+SER_PHY_HEADER_SIZE, buf_len);

                // Remove this packet from our queue.
                _head_event = (_head_event + 1) % RX_BUFFER_SIZE;
            }

            _ser_phy_rx_event.evt_type = SER_PHY_EVT_RX_PKT_RECEIVED;
            _ser_phy_rx_event.evt_params.rx_pkt_received.num_of_bytes = buf_len;
            _ser_phy_rx_event.evt_params.rx_pkt_received.p_buffer = hal_rx_buf;

            hal_rx_buf = NULL;

            // Check if there are more packets in the queue.
            if (_head_event != _tail_event || _have_rsp_packet) {
                _queued_packets = true;
            }

            if (_ser_phy_event_handler) {
                _receiving_packet = false;
                _ser_phy_event_handler(_ser_phy_rx_event);
            }
        } else {
            // Buffer is NULL. That means we have to drop this packet. We also
            // need to notify the serialization library that we did so.
            _ser_phy_rx_event.evt_type = SER_PHY_EVT_RX_PKT_DROPPED;
            if (_ser_phy_event_handler) {
                _receiving_packet = false;
                _ser_phy_event_handler(_ser_phy_rx_event);
            }
        }
    }


    // When we get here, we _may_ end up in a wait() state. Wait is good because
    // it lets us check if we have any outstanding things to do and if so run
    // them instead of calling yield. However, depending on where the nRF
    // state machine is at, wait may not get called. We can check this by seeing
    // if the nordic state machine thinks its busy.
    if (!ser_sd_transport_is_busy()) {
        if (_need_wakeup) {
            _need_wakeup = false;
            ble_serialization_callback(17, 0, 0, NULL);
        } else if (_queued_packets) {
            _queued_packets = false;
            ble_serialization_callback(4, 0, 0, NULL);
        }
    }
}

/*******************************************************************************
 * Main API for upper layers of BLE serialization
 ******************************************************************************/

//
// ser_app_hal_nrf51.c
//

uint32_t ser_app_hal_hw_init (void) {
    return NRF_SUCCESS;
}

void ser_app_hal_delay (uint32_t ms)  {
    delay_ms(ms);
}

void ser_app_hal_nrf_reset_pin_clear (void) {}

void ser_app_hal_nrf_reset_pin_set (void) {}

void ser_app_hal_nrf_evt_irq_priority_set (void) {
    // Since we aren't using an actual interrupt, not needed
}

void ser_app_hal_nrf_evt_pending (void) {
    // Not sure if we can do software interrupts, so try just doing a function
    // call.
    TOCK_EVT_IRQHandler();
}


//
// ser_phy_nrf51_uart.c
//

uint32_t ser_phy_open (ser_phy_events_handler_t events_handler) {
    int ret;

    if (events_handler == NULL) {
        return NRF_ERROR_NULL;
    }

    // Check that we haven't already opened the phy layer
    if (_ser_phy_event_handler != NULL) {
        return NRF_ERROR_INVALID_STATE;
    }

    // Start by doing a reset.
    ret = nrf51_serialization_reset();
    if (ret < 0) return NRF_ERROR_INTERNAL;

    // Configure the serialization layer in the kernel
    ret = nrf51_serialization_subscribe(ble_serialization_callback);
    if (ret < 0) return NRF_ERROR_INTERNAL;

    ret = nrf51_serialization_setup_receive_buffer((char*) rx, SER_HAL_TRANSPORT_RX_MAX_PKT_SIZE);
    if (ret < 0) return NRF_ERROR_INTERNAL;

    ret = nrf51_serialization_read(SER_HAL_TRANSPORT_RX_MAX_PKT_SIZE);
    if (ret < 0) return NRF_ERROR_INTERNAL;
    
    // Save the callback handler
    _ser_phy_event_handler = events_handler;

    return NRF_SUCCESS;
}

uint32_t ser_phy_tx_pkt_send (const uint8_t* p_buffer, uint16_t num_of_bytes) {
    // Error checks
    if (p_buffer == NULL) {
        return NRF_ERROR_NULL;
    } else if (num_of_bytes == 0) {
        return NRF_ERROR_INVALID_PARAM;
    }

    // Check if there is no ongoing transmission at the moment
    if (tx_len == 0) {
        // We need to set a timer in case we never get the response packet.
        if (ser_sd_transport_is_busy()) {
            _timeout_timer = (tock_timer_t*)malloc(sizeof(tock_timer_t));
            timer_in(100, timeout_timer_cb, NULL, _timeout_timer);
        }

        // Encode the number of bytes as the first two bytes of the outgoing
        // packet.
        tx[0] = num_of_bytes & 0xFF;
        tx[1] = (num_of_bytes >> 8) & 0xFF;

        // Copy in the outgoing data
        memcpy(tx+2, p_buffer, num_of_bytes);

        // Add in that we added the header (2 length bytes)
        tx_len = num_of_bytes + SER_PHY_HEADER_SIZE;

        // Call tx procedure to start transmission of a packet
        int ret = nrf51_serialization_write((char*) tx, tx_len);
        if (ret < 0) {
            return NRF_ERROR_INTERNAL;
        }
    } else {
        return NRF_ERROR_BUSY;
    }

    return NRF_SUCCESS;
}


uint32_t ser_phy_rx_buf_set (uint8_t* p_buffer) {
    // Save a pointer to the buffer we can use.
    hal_rx_buf = p_buffer;

    _need_wakeup = true;

    return NRF_SUCCESS;
}

void ser_phy_close (void) {
    printf("close\n");
    _ser_phy_event_handler = NULL;
}

void ser_phy_interrupts_enable (void) { }

void ser_phy_interrupts_disable (void) { }


// Essentially sleep this process
uint32_t sd_app_evt_wait (void) {
    if (_need_wakeup == true) {
        // We needed to let the nordic library get to its wait function, but we
        // already have a buffer ready to pass to it. Rather than calling
        // through the kernel, just call the function here.
        _need_wakeup = false;
        ble_serialization_callback(17, 0, 0, NULL);
    } else if (_queued_packets) {
        _queued_packets = false;
        ble_serialization_callback(4, 0, 0, NULL);
    } else {
        nrf_serialization_done = false;
        yield_for(&nrf_serialization_done);
    }
    return NRF_SUCCESS;
}





/**@brief Timer node type. The nodes will be used form a linked list of running timers. */
typedef struct
{
    uint32_t                    ticks_to_expire;                            /**< Number of ticks from previous timer interrupt to timer expiry. */
    uint32_t                    ticks_at_start;                             /**< Current RTC counter value when the timer was started. */
    uint32_t                    ticks_first_interval;                       /**< Number of ticks in the first timer interval. */
    uint32_t                    ticks_periodic_interval;                    /**< Timer period (for repeating timers). */
    bool                        is_running;                                 /**< True if timer is running, False otherwise. */
    app_timer_mode_t            mode;                                       /**< Timer mode. */
    app_timer_timeout_handler_t p_timeout_handler;                          /**< Pointer to function to be executed when the timer expires. */
    void *                      p_context;                                  /**< General purpose pointer. Will be passed to the timeout handler when the timer expires. */
    void *                      next;                                       /**< Pointer to the next node. */
} timer_node_t;

#define APP_TIMER_MS(TICKS, PRESCALER)\
    ( ((uint64_t) TICKS * ((PRESCALER+1)*1000)) / ((uint64_t) APP_TIMER_CLOCK_FREQ) )

uint32_t app_timer_init (uint32_t                      prescaler,
                         uint8_t                       op_queues_size,
                         void *                        p_buffer,
                         app_timer_evt_schedule_func_t evt_schedule_func) {
    UNUSED_PARAMETER(prescaler);
    UNUSED_PARAMETER(op_queues_size);
    UNUSED_PARAMETER(p_buffer);
    UNUSED_PARAMETER(evt_schedule_func);
    return NRF_SUCCESS;
}

/**@brief Function for creating a timer instance.
 *
 * @param[in]  p_timer_id        Pointer to timer identifier.
 * @param[in]  mode              Timer mode.
 * @param[in]  timeout_handler   Function to be executed when the timer expires.
 *
 * @retval     NRF_SUCCESS               If the timer was successfully created.
 * @retval     NRF_ERROR_INVALID_PARAM   If a parameter was invalid.
 * @retval     NRF_ERROR_INVALID_STATE   If the application timer module has not been initialized or
 *                                       the timer is running.
 *
 * @note This function does the timer allocation in the caller's context. It is also not protected
 *       by a critical region. Therefore care must be taken not to call it from several interrupt
 *       levels simultaneously.
 * @note The function can be called again on the timer instance and will re-initialize the instance if
 *       the timer is not running.
 * @attention The FreeRTOS and RTX app_timer implementation does not allow app_timer_create to
 *       be called on the previously initialized instance.
 */
uint32_t app_timer_create (app_timer_id_t const *      p_timer_id,
                           app_timer_mode_t            mode,
                           app_timer_timeout_handler_t timeout_handler) {
    // UNUSED_PARAMETER(p_timer_id);
    // UNUSED_PARAMETER(mode);
    // UNUSED_PARAMETER(timeout_handler);
    timer_node_t * p_node     = (timer_node_t*) *p_timer_id;
    p_node->is_running        = false;
    p_node->mode              = mode;
    p_node->p_timeout_handler = timeout_handler;



    return NRF_SUCCESS;
}



void serialization_timer_cb (int a, int b, int c, void* timer_id) {
    UNUSED_PARAMETER(a);
    UNUSED_PARAMETER(b);
    UNUSED_PARAMETER(c);

    timer_node_t* p_node = (timer_node_t*) timer_id;

    p_node->p_timeout_handler(p_node->p_context);
}

/**@brief Function for starting a timer.
 *
 * @param[in]       timer_id      Timer identifier.
 * @param[in]       timeout_ticks Number of ticks (of RTC1, including prescaling) to time-out event
 *                                (minimum 5 ticks).
 * @param[in]       p_context     General purpose pointer. Will be passed to the time-out handler when
 *                                the timer expires.
 *
 * @retval     NRF_SUCCESS               If the timer was successfully started.
 * @retval     NRF_ERROR_INVALID_PARAM   If a parameter was invalid.
 * @retval     NRF_ERROR_INVALID_STATE   If the application timer module has not been initialized or the timer
 *                                       has not been created.
 * @retval     NRF_ERROR_NO_MEM          If the timer operations queue was full.
 *
 * @note The minimum timeout_ticks value is 5.
 * @note For multiple active timers, time-outs occurring in close proximity to each other (in the
 *       range of 1 to 3 ticks) will have a positive jitter of maximum 3 ticks.
 * @note When calling this method on a timer that is already running, the second start operation
 *       is ignored.
 */
uint32_t app_timer_start (app_timer_id_t timer_id,
                          uint32_t timeout_ticks,
                          void* p_context) {
    // UNUSED_PARAMETER(timer_id);
    // UNUSED_PARAMETER(timeout_ticks);
    UNUSED_PARAMETER(p_context);

    timer_node_t* p_node = (timer_node_t*) timer_id;

    if (p_node->mode == APP_TIMER_MODE_REPEATED) {
        p_node->p_context = p_context;
        // timer_repeating_subscribe(p_node->p_timeout_handler, &timer_id);
        // Use 0 for the prescaler
        tock_timer_t* timer = (tock_timer_t*)malloc(sizeof(tock_timer_t));
        timer_every(APP_TIMER_MS(timeout_ticks, 0), serialization_timer_cb, timer_id, timer);
    } else {
        // timer_oneshot_subscribe(p_node->p_timeout_handler, &timer_id);
    }

    return NRF_SUCCESS;
}

/**@brief Function for stopping the specified timer.
 *
 * @param[in]  timer_id                  Timer identifier.
 *
 * @retval     NRF_SUCCESS               If the timer was successfully stopped.
 * @retval     NRF_ERROR_INVALID_PARAM   If a parameter was invalid.
 * @retval     NRF_ERROR_INVALID_STATE   If the application timer module has not been initialized or the timer
 *                                       has not been created.
 * @retval     NRF_ERROR_NO_MEM          If the timer operations queue was full.
 */
uint32_t app_timer_stop (app_timer_id_t timer_id) {
    UNUSED_PARAMETER(timer_id);
    return NRF_SUCCESS;
}

/**@brief Function for stopping all running timers.
 *
 * @retval     NRF_SUCCESS               If all timers were successfully stopped.
 * @retval     NRF_ERROR_INVALID_STATE   If the application timer module has not been initialized.
 * @retval     NRF_ERROR_NO_MEM          If the timer operations queue was full.
 */
uint32_t app_timer_stop_all (void) {
    return NRF_SUCCESS;
}

/**@brief Function for returning the current value of the RTC1 counter.
 *
 * @param[out] p_ticks   Current value of the RTC1 counter.
 *
 * @retval     NRF_SUCCESS   If the counter was successfully read.
 */
uint32_t app_timer_cnt_get (uint32_t* p_ticks) {
    UNUSED_PARAMETER(p_ticks);
    return NRF_SUCCESS;
}

/**@brief Function for computing the difference between two RTC1 counter values.
 *
 * @param[in]  ticks_to       Value returned by app_timer_cnt_get().
 * @param[in]  ticks_from     Value returned by app_timer_cnt_get().
 * @param[out] p_ticks_diff   Number of ticks from ticks_from to ticks_to.
 *
 * @retval     NRF_SUCCESS   If the counter difference was successfully computed.
 */
uint32_t app_timer_cnt_diff_compute (uint32_t ticks_to,
                                     uint32_t ticks_from,
                                     uint32_t* p_ticks_diff) {
    UNUSED_PARAMETER(ticks_to);
    UNUSED_PARAMETER(ticks_from);
    UNUSED_PARAMETER(p_ticks_diff);
    return NRF_SUCCESS;
}

void ser_app_power_system_off_set (void) {}
bool ser_app_power_system_off_get (void) { return false; }
void ser_app_power_system_off_enter (void) {}
void critical_region_enter (void) {}
void critical_region_exit (void) {}
