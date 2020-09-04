/*-------------------------------------------------------------------------
NeoPixel library helper functions for Esp8266 UART hardware

Written by Michael C. Miller.

I invest time and resources providing this open source code,
please support me by dontating (see https://github.com/Makuna/NeoPixelBus)

-------------------------------------------------------------------------
This file is part of the Makuna/NeoPixelBus library.

NeoPixelBus is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3 of
the License, or (at your option) any later version.

NeoPixelBus is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with NeoPixel.  If not, see
<http://www.gnu.org/licenses/>.
-------------------------------------------------------------------------*/

#ifdef ARDUINO_ARCH_ESP8266
#include "NeoEsp8266UartMethod.h"
#include <utility>
extern "C"
{
    #include <eagle_soc.h>
    #include <ets_sys.h>
    #include <uart.h>
    #include <uart_register.h>
}

#define UART1 1
#define UART1_INV_MASK (0x3f << 19)

// Gets the number of bytes waiting in the TX FIFO of UART1
static inline uint8_t getUartTxFifoLength()
{
    return (U1S >> USTXC) & 0xff;
}

// Append a byte to the TX FIFO of UART1
// You must ensure the TX FIFO isn't full
static inline void enqueue(uint8_t byte)
{
    U1F = byte;
}

static const uint8_t* esp8266_uart1_async_buf;
static const uint8_t* esp8266_uart1_async_buf_end;

NeoEsp8266Uart::NeoEsp8266Uart(uint16_t pixelCount, size_t elementSize)
{
    _sizePixels = pixelCount * elementSize;
    _pixels = (uint8_t*)malloc(_sizePixels);
    memset(_pixels, 0x00, _sizePixels);
}

NeoEsp8266Uart::~NeoEsp8266Uart()
{
    // Wait until the TX fifo is empty. This way we avoid broken frames
    // when destroying & creating a NeoPixelBus to change its length.
    while (getUartTxFifoLength() > 0)
    {
        yield();
    }

    free(_pixels);
}

void NeoEsp8266Uart::InitializeUart(uint32_t uartBaud)
{
    // Configure the serial line with 1 start bit (0), 6 data bits and 1 stop bit (1)
    Serial1.begin(uartBaud, SERIAL_6N1, SERIAL_TX_ONLY);

    // Invert the TX voltage associated with logic level so:
    //    - A logic level 0 will generate a Vcc signal
    //    - A logic level 1 will generate a Gnd signal
    CLEAR_PERI_REG_MASK(UART_CONF0(UART1), UART1_INV_MASK);
    SET_PERI_REG_MASK(UART_CONF0(UART1), (BIT(22)));
}

void NeoEsp8266Uart::UpdateUart()
{
    // Since the UART can finish sending queued bytes in the FIFO in
    // the background, instead of waiting for the FIFO to flush
    // we annotate the start time of the frame so we can calculate
    // when it will finish.
    _startTime = micros();

    // Then keep filling the FIFO until done
    const uint8_t* ptr = _pixels;
    const uint8_t* end = ptr + _sizePixels;
    while (ptr != end)
    {
        ptr = FillUartFifo(ptr, end);
    }
}

const uint8_t* ICACHE_RAM_ATTR NeoEsp8266Uart::FillUartFifo(const uint8_t* pixels, const uint8_t* end)
{
    // Remember: UARTs send less significant bit (LSB) first so
    //      pushing ABCDEF byte will generate a 0FEDCBA1 signal,
    //      including a LOW(0) start & a HIGH(1) stop bits.
    // Also, we have configured UART to invert logic levels, so:
    const uint8_t _uartData[4] = {
        0b110111, // On wire: 1 000 100 0 [Neopixel reads 00]
        0b000111, // On wire: 1 000 111 0 [Neopixel reads 01]
        0b110100, // On wire: 1 110 100 0 [Neopixel reads 10]
        0b000100, // On wire: 1 110 111 0 [NeoPixel reads 11]
    };
    uint8_t avail = (UART_TX_FIFO_SIZE - getUartTxFifoLength()) / 4;
    if (end - pixels > avail)
    {
        end = pixels + avail;
    }
    while (pixels < end)
    {
        uint8_t subpix = *pixels++;
        enqueue(_uartData[(subpix >> 6) & 0x3]);
        enqueue(_uartData[(subpix >> 4) & 0x3]);
        enqueue(_uartData[(subpix >> 2) & 0x3]);
        enqueue(_uartData[ subpix       & 0x3]);
    }
    return pixels;
}

NeoEsp8266AsyncUart::NeoEsp8266AsyncUart(uint16_t pixelCount, size_t elementSize)
    : NeoEsp8266Uart(pixelCount, elementSize)
{
    _asyncPixels = (uint8_t*)malloc(_sizePixels);
}

NeoEsp8266AsyncUart::~NeoEsp8266AsyncUart()
{
    // Remember: the UART interrupt can be sending data from _asyncPixels in the background
    while (esp8266_uart1_async_buf != esp8266_uart1_async_buf_end)
    {
        yield();
    }
    free(_asyncPixels);
}

void ICACHE_RAM_ATTR NeoEsp8266AsyncUart::InitializeUart(uint32_t uartBaud)
{
    NeoEsp8266Uart::InitializeUart(uartBaud);

    // Disable all interrupts
    ETS_UART_INTR_DISABLE();

    // Clear the RX & TX FIFOS
    SET_PERI_REG_MASK(UART_CONF0(UART1), UART_RXFIFO_RST | UART_TXFIFO_RST);
    CLEAR_PERI_REG_MASK(UART_CONF0(UART1), UART_RXFIFO_RST | UART_TXFIFO_RST);

    // Set the interrupt handler
    ETS_UART_INTR_ATTACH(IntrHandler, NULL);

    // Set tx fifo trigger. 80 bytes gives us 200 microsecs to refill the FIFO
    WRITE_PERI_REG(UART_CONF1(UART1), 80 << UART_TXFIFO_EMPTY_THRHD_S);

    // Disable RX & TX interrupts. It is enabled by uart.c in the SDK
    CLEAR_PERI_REG_MASK(UART_INT_ENA(UART1), UART_RXFIFO_FULL_INT_ENA | UART_TXFIFO_EMPTY_INT_ENA);

    // Clear all pending interrupts in UART1
    WRITE_PERI_REG(UART_INT_CLR(UART1), 0xffff);

    // Reenable interrupts
    ETS_UART_INTR_ENABLE();
}

void NeoEsp8266AsyncUart::UpdateUart()
{
    // Instruct ESP8266 hardware uart1 to send the pixels asynchronously
    esp8266_uart1_async_buf = _pixels;
    esp8266_uart1_async_buf_end = _pixels + _sizePixels;
    SET_PERI_REG_MASK(UART_INT_ENA(1), UART_TXFIFO_EMPTY_INT_ENA);

    // Annotate when we started to send bytes, so we can calculate when we are ready to send again
    _startTime = micros();

    // Copy the pixels to the idle buffer and swap them
    memcpy(_asyncPixels, _pixels, _sizePixels);
    std::swap(_asyncPixels, _pixels);
}

void ICACHE_RAM_ATTR NeoEsp8266AsyncUart::IntrHandler(void* param)
{
    // Interrupt handler is shared between UART0 & UART1
    if (READ_PERI_REG(UART_INT_ST(UART1)))   //any UART1 stuff
    {
        // Fill the FIFO with new data
        esp8266_uart1_async_buf = FillUartFifo(esp8266_uart1_async_buf, esp8266_uart1_async_buf_end);
        // Disable TX interrupt when done
        if (esp8266_uart1_async_buf == esp8266_uart1_async_buf_end)
        {
            CLEAR_PERI_REG_MASK(UART_INT_ENA(UART1), UART_TXFIFO_EMPTY_INT_ENA);
        }
        // Clear all interrupts flags (just in case)
        WRITE_PERI_REG(UART_INT_CLR(UART1), 0xffff);
    }

    if (READ_PERI_REG(UART_INT_ST(UART0)))
    {
        // TODO: gdbstub uses the interrupt of UART0, but there is no way to call its
        // interrupt handler gdbstub_uart_hdlr since it's static.
        WRITE_PERI_REG(UART_INT_CLR(UART0), 0xffff);
    }
}

#endif

