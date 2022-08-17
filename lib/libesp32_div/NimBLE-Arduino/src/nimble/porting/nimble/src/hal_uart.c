/*
 * SPDX-FileCopyrightText: 2021 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#if CONFIG_IDF_TARGET_ESP32H2 || CONFIG_IDF_TARGET_ESP32C2

#include "freertos/FreeRTOS.h"
#include "os/os.h"
#include "hal/hal_uart.h"
#include "soc/soc.h"
#include "soc/system_reg.h"
#include "driver/gpio.h"
#include "esp_intr_alloc.h"
#include "riscv/interrupt.h"
#include "hal/uart_ll.h"
#include "freertos/semphr.h"
#include "rom/ets_sys.h"

#include "driver/uart.h"
#include "driver/periph_ctrl.h"

struct hal_uart {
    uint8_t u_open:1;
    uint8_t u_rx_stall:1;
    uint8_t u_tx_started:1;
    uint8_t u_tx_buf;
    hal_uart_rx_char u_rx_func;
    hal_uart_tx_char u_tx_func;
    hal_uart_tx_done u_tx_done;
    SemaphoreHandle_t u_rx_sem;
    void *u_func_arg;
};
static struct hal_uart uart;

#define UART_FIFO_LEN       (128)

uint8_t rxbuffer[256];
static uint16_t rd_ptr = 0;
static uint16_t wr_ptr = 0;
#define BUFFER_MASK (0xff)

static TaskHandle_t hci_uart_task_h;

void uart_init(uint32_t baud);
void uart0_init(uint32_t baud);
void uart0_tout_isr(void);
void uart_tout_isr(void);
void
hal_uart_blocking_tx(int port, uint8_t data){

}
int hal_uart_init(int uart_no, void *cfg)
{
    // Uart_Init(uart_no, UART_CLK_FREQ_ROM);
    return 0;
}

void hal_uart_start_rx(int uart)
{
    ets_printf("rx support???\n");
}
int hal_uart_init_cbs(int uart_no, hal_uart_tx_char tx_func,
  hal_uart_tx_done tx_done, hal_uart_rx_char rx_func, void *arg)
{
    struct hal_uart *u;

    u = &uart;

    if (u->u_open) {
        return -1;
    }

    u->u_rx_func = rx_func;
    u->u_tx_func = tx_func;
    u->u_tx_done = tx_done;
    u->u_func_arg = arg;
    return 0;
}

int hal_uart_config(int uart_no, int32_t speed, uint8_t databits, uint8_t stopbits,
  enum hal_uart_parity parity, enum hal_uart_flow_ctl flow_ctl)
{
    struct hal_uart *u;
    u = &uart;

    if (u->u_open) {
        return -1;
    }
    if (uart_no) {
        uart_init(speed);
    } else {
        uart0_init(speed);
    }
    u->u_open = 1;
    return 0;
}

static int
IRAM_ATTR_64MCPU hal_uart_tx_fill_buf(struct hal_uart *u, int uart_no)
{
    int data;
    int i;
    uart_dev_t *hw = &UART1;
    if(!uart_no){
        hw = &UART0;
    }
    i = 0;
    while(hw->status.txfifo_cnt < UART_FIFO_LEN){
        data = u->u_tx_func(u->u_func_arg);
        if (data >= 0) {
            hw->ahb_fifo.rw_byte = data;
            i++;
        }else{
            break;
        }
    }
    return i;
}

void hal_uart_start_tx(int uart_no)
{
    struct hal_uart *u;
    os_sr_t sr;
    uart_dev_t *hw = &UART1;
    if(!uart_no){
        hw = &UART0;
    }
    u = &uart;
    OS_ENTER_CRITICAL(sr);
    if (u->u_tx_started == 0) {
        u->u_tx_started = 1;
        hw->int_ena.tx_done = 1;
        hal_uart_tx_fill_buf(u,uart_no);
    }
    OS_EXIT_CRITICAL(sr);
}

int hal_uart0_close(int uart_no)
{
    struct hal_uart *u;
    u = &uart;
    u->u_open = 0;

    REG_CLR_BIT(SYSTEM_PERIP_CLK_EN0_REG, SYSTEM_UART_CLK_EN);
    REG_SET_BIT(SYSTEM_PERIP_RST_EN0_REG, SYSTEM_UART_RST);
    return 0;
}

int hal_uart_close(int uart_no)
{
    struct hal_uart *u;
    u = &uart;
    u->u_open = 0;

    REG_CLR_BIT(SYSTEM_PERIP_CLK_EN0_REG, SYSTEM_UART1_CLK_EN);
    REG_SET_BIT(SYSTEM_PERIP_RST_EN0_REG, SYSTEM_UART1_RST);
    return 0;
}

IRAM_ATTR_64MCPU void uart0_tout_isr(void)
{
    struct hal_uart *u;
    int rc;
    uint32_t ch;
    uart_dev_t *hw = &UART0;

    u = &uart;
    uint32_t uart_int_st = hw->int_st.val;
    //clear
    hw->int_clr.val = uart_int_st;
    if (uart_int_st&UART_TX_DONE_INT_ST_M) {
        rc = hal_uart_tx_fill_buf(u,0);
        if (rc == 0) {
            if (u->u_tx_done) {
                u->u_tx_done(u->u_func_arg);
            }
            u->u_tx_started = 0;
            hw->int_ena.tx_done = 0;
        }
    }

    while (hw->status.rxfifo_cnt) {
        int rd_len = hw->status.rxfifo_cnt;
        for (int i = 0; i < rd_len; i++) {
            ch = hw->ahb_fifo.rw_byte;
            rc = u->u_rx_func(u->u_func_arg, ch);
            if (rc < 0) {
                u->u_rx_stall = 1;
            }
        }

    }
}

IRAM_ATTR_64MCPU void uart_tout_isr(void)
{
    struct hal_uart *u;
    int rc;
    bool rx_update = false;
    uart_dev_t *hw = &UART1;
    u = &uart;
    uint32_t uart_int_st = hw->int_st.val;
    //clear
    hw->int_clr.val = uart_int_st;
    if (uart_int_st&UART_TX_DONE_INT_ST_M) {
        rc = hal_uart_tx_fill_buf(u,1);
        if (rc == 0) {
            if (u->u_tx_done) {
                u->u_tx_done(u->u_func_arg);
            }
            u->u_tx_started = 0;
            hw->int_ena.tx_done = 0;
        }
    }
    while (hw->status.rxfifo_cnt) {
        rxbuffer[(wr_ptr++)&BUFFER_MASK] = hw->ahb_fifo.rw_byte;
        rx_update = true;
    }
    if (rx_update) {
        xSemaphoreGiveFromISR(u->u_rx_sem, 0);
    }
}

void
IRAM_ATTR uart_rx_task(void *arg){
    int rc;
    struct hal_uart *u;
    u = &uart;
    while (1) {
        xSemaphoreTake(u->u_rx_sem, portMAX_DELAY);
        while (rd_ptr != wr_ptr){
            rc = u->u_rx_func(u->u_func_arg, rxbuffer[(rd_ptr++)&BUFFER_MASK]);
            if (rc < 0) {
                u->u_rx_stall = 1;
            }
        }
    }
}


void uart0_init(uint32_t baud)
{
    REG_SET_BIT(SYSTEM_PERIP_RST_EN0_REG, SYSTEM_UART_RST);
    REG_CLR_BIT(SYSTEM_PERIP_RST_EN0_REG, SYSTEM_UART_RST);
    REG_CLR_BIT(SYSTEM_PERIP_CLK_EN0_REG, SYSTEM_UART_CLK_EN_M);
    REG_SET_BIT(SYSTEM_PERIP_CLK_EN0_REG, SYSTEM_UART_CLK_EN_M);

    const int sclk_div = 1;
    uint32_t sclk_freq = XTAL_CLK_FREQ;
    uint32_t clk_div = ((sclk_freq) << 4) / baud;
    uart_dev_t *hw = &UART0;
    hw->clk_conf.sclk_en = 0;
    hw->clk_conf.rx_sclk_en = 0;
    hw->clk_conf.tx_sclk_en = 0;


    hw->clk_conf.sclk_en = 1;
    hw->clk_conf.rx_sclk_en = 1;
    hw->clk_conf.tx_sclk_en = 1;

    hw->clk_div.div_int = clk_div >> 4;
    hw->clk_div.div_frag = clk_div &  0xf;
    hw->clk_conf.sclk_div_num = sclk_div - 1;//7;//255;

    hw->conf0.parity_en = 0;

    hw->conf0.irda_en = 0;
    hw->rs485_conf.en = 0;
    hw->rs485_conf.tx_rx_en = 0;
    hw->rs485_conf.rx_busy_tx_en = 0;

    hw->conf0.bit_num = 3;
    hw->conf0.stop_bit_num = 1;

    hw->conf1.rxfifo_full_thrhd = 80;

    hw->mem_conf.rx_tout_thrhd = 20;
    hw->conf1.rx_tout_en = 1;

    hw->conf1.rx_flow_en = 0;
    hw->conf0.tx_flow_en = 0;

    hw->conf0.rxfifo_rst = 1;
    hw->conf0.rxfifo_rst = 0;
    hw->conf0.txfifo_rst = 1;
    hw->conf0.txfifo_rst = 0;
    hw->int_ena.rxfifo_full = 1;
    //enable rx fifo timeout interrupt
    hw->int_ena.rxfifo_tout = 1;

    uint8_t tx_pin = 21, rx_pin = 20, ISR_ID = ETS_UART0_INUM;

    ets_isr_mask((1<<ISR_ID)); //ETS_INTR_DISABLE

    intr_handler_set(ISR_ID, (intr_handler_t)&uart0_tout_isr, NULL);
    intr_matrix_route(ETS_UART0_INTR_SOURCE, ISR_ID);
    esprv_intc_int_enable(BIT(ISR_ID));
    esprv_intc_int_set_type(BIT(ISR_ID), INTR_TYPE_LEVEL);
    esprv_intc_int_set_priority(ISR_ID, 1);

    esp_intr_reserve(ISR_ID, xPortGetCoreID());

    ESP_ERROR_CHECK(uart_set_pin(0, tx_pin, rx_pin, -1, -1));

}



void uart_init(uint32_t baud)
{
    periph_module_enable(PERIPH_UART1_MODULE);

    REG_SET_BIT(SYSTEM_PERIP_RST_EN0_REG, SYSTEM_UART1_RST);
    REG_CLR_BIT(SYSTEM_PERIP_RST_EN0_REG, SYSTEM_UART1_RST);
    REG_CLR_BIT(SYSTEM_PERIP_CLK_EN0_REG, SYSTEM_UART1_CLK_EN_M);
    REG_SET_BIT(SYSTEM_PERIP_CLK_EN0_REG, SYSTEM_UART1_CLK_EN_M);

    const int sclk_div = 1;
    uint32_t sclk_freq = XTAL_CLK_FREQ;

    uint32_t clk_div = ((sclk_freq) << 4) / baud;
    struct hal_uart *u = &uart;

    uart_dev_t *hw = &UART1;
    hw->clk_conf.sclk_en = 0;
    hw->clk_conf.rx_sclk_en = 0;
    hw->clk_conf.tx_sclk_en = 0;


    hw->clk_conf.sclk_en = 1;
    hw->clk_conf.rx_sclk_en = 1;
    hw->clk_conf.tx_sclk_en = 1;

    hw->clk_div.div_int = clk_div >> 4;
    hw->clk_div.div_frag = clk_div &  0xf;
    hw->clk_conf.sclk_div_num = sclk_div - 1;//7;//255;

    hw->conf0.parity_en = 0;

    hw->conf0.irda_en = 0;
    hw->rs485_conf.en = 0;
    hw->rs485_conf.tx_rx_en = 0;
    hw->rs485_conf.rx_busy_tx_en = 0;

    hw->conf0.bit_num = 3;
    hw->conf0.stop_bit_num = 1;

    //set full threshold to (2/3)*FIFO_LEN
    hw->conf1.rxfifo_full_thrhd = 80;
    hw->mem_conf.rx_tout_thrhd = 20;

    hw->conf1.rx_tout_en = 1;

    hw->conf0.rxfifo_rst = 1;
    hw->conf0.rxfifo_rst = 0;
    hw->conf0.txfifo_rst = 1;
    hw->conf0.txfifo_rst = 0;
    hw->int_ena.rxfifo_full = 1;
    //enable rx fifo timeout interrupt
    hw->int_ena.rxfifo_tout = 1;

    u->u_rx_sem = xSemaphoreCreateBinary();

    uint8_t TX_IO = CONFIG_BT_LE_HCI_UART_TX_PIN, RX_IO = CONFIG_BT_LE_HCI_UART_RX_PIN, ISR_ID = ETS_UART1_INUM;
    printf("set nimble port tx:%d, rx:%d.\n", TX_IO, RX_IO);
    printf("set baud:%d.\n", baud);
    intr_handler_set(ISR_ID, (intr_handler_t)&uart_tout_isr, NULL);
    intr_matrix_route(ETS_UART1_INTR_SOURCE, ISR_ID);
    esprv_intc_int_enable(BIT(ISR_ID));
    esprv_intc_int_set_type(BIT(ISR_ID), INTR_TYPE_LEVEL);
    esprv_intc_int_set_priority(ISR_ID, 1);
    // TODO ESP32-C3 IDF-2126, maybe can use interrupt allocation API for all of the above? unsure...
    esp_intr_reserve(ISR_ID, xPortGetCoreID());

    ESP_ERROR_CHECK(uart_set_pin(1, TX_IO, RX_IO, -1, -1));

    //Enable hw flow control of UART1 for BQB test, which if not enabled, the tester takes quite long time to send a byte to DUT


    // uart_ll_set_hw_flow_ctrl(hw, UART_HW_FLOWCTRL_CTS_RTS, 110);

    xTaskCreate(uart_rx_task, "uart_rx", CONFIG_BT_LE_HCI_UART_TASK_STACK_SIZE,
                NULL, 1, &hci_uart_task_h);
}

#endif // CONFIG_IDF_TARGET_ESP32H2 || CONFIG_IDF_TARGET_ESP32C2