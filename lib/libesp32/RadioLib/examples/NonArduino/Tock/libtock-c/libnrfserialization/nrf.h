
#ifndef NRF_H
#define NRF_H

#if defined(_WIN32)
    /* Do not include nrf51 specific files when building for PC host */
#elif defined(__unix)
    /* Do not include nrf51 specific files when building for PC host */
#elif defined(__APPLE__)
    /* Do not include nrf51 specific files when building for PC host */
#elif defined(__TOCK__)

// Need this define to make the softdevice code compile. It is meaningless.

typedef enum {
/* -------------------  Cortex-M0 Processor Exceptions Numbers  ------------------- */
  Reset_IRQn                    = -15,              /*!<   1  Reset Vector, invoked on Power up and warm reset                 */
  NonMaskableInt_IRQn           = -14,              /*!<   2  Non maskable Interrupt, cannot be stopped or preempted           */
  HardFault_IRQn                = -13,              /*!<   3  Hard Fault, all classes of Fault                                 */
  SVCall_IRQn                   =  -5,              /*!<  11  System Service Call via SVC instruction                          */
  DebugMonitor_IRQn             =  -4,              /*!<  12  Debug Monitor                                                    */
  PendSV_IRQn                   =  -2,              /*!<  14  Pendable request for system service                              */
  SysTick_IRQn                  =  -1,              /*!<  15  System Tick Timer                                                */
/* ----------------------  nrf51 Specific Interrupt Numbers  ---------------------- */
  POWER_CLOCK_IRQn              =   0,              /*!<   0  POWER_CLOCK                                                      */
  RADIO_IRQn                    =   1,              /*!<   1  RADIO                                                            */
  UART0_IRQn                    =   2,              /*!<   2  UART0                                                            */
  SPI0_TWI0_IRQn                =   3,              /*!<   3  SPI0_TWI0                                                        */
  SPI1_TWI1_IRQn                =   4,              /*!<   4  SPI1_TWI1                                                        */
  GPIOTE_IRQn                   =   6,              /*!<   6  GPIOTE                                                           */
  ADC_IRQn                      =   7,              /*!<   7  ADC                                                              */
  TIMER0_IRQn                   =   8,              /*!<   8  TIMER0                                                           */
  TIMER1_IRQn                   =   9,              /*!<   9  TIMER1                                                           */
  TIMER2_IRQn                   =  10,              /*!<  10  TIMER2                                                           */
  RTC0_IRQn                     =  11,              /*!<  11  RTC0                                                             */
  TEMP_IRQn                     =  12,              /*!<  12  TEMP                                                             */
  RNG_IRQn                      =  13,              /*!<  13  RNG                                                              */
  ECB_IRQn                      =  14,              /*!<  14  ECB                                                              */
  CCM_AAR_IRQn                  =  15,              /*!<  15  CCM_AAR                                                          */
  WDT_IRQn                      =  16,              /*!<  16  WDT                                                              */
  RTC1_IRQn                     =  17,              /*!<  17  RTC1                                                             */
  QDEC_IRQn                     =  18,              /*!<  18  QDEC                                                             */
  LPCOMP_IRQn                   =  19,              /*!<  19  LPCOMP                                                           */
  SWI0_IRQn                     =  20,              /*!<  20  SWI0                                                             */
  SWI1_IRQn                     =  21,              /*!<  21  SWI1                                                             */
  SWI2_IRQn                     =  22,              /*!<  22  SWI2                                                             */
  SWI3_IRQn                     =  23,              /*!<  23  SWI3                                                             */
  SWI4_IRQn                     =  24,              /*!<  24  SWI4                                                             */
  SWI5_IRQn                     =  25               /*!<  25  SWI5                                                             */
} IRQn_Type;

#define SOFTDEVICE_EVT_IRQ        SWI2_IRQn       /**< SoftDevice Event IRQ number. Used for both protocol events and SoC events. */
#define SOFTDEVICE_EVT_IRQHandler TOCK_EVT_IRQHandler

void SOFTDEVICE_EVT_IRQHandler(void);

#else

    /* Family selection for family includes. */
    #if defined (NRF51)
        #include "nrf51.h"
        #include "nrf51_bitfields.h"
        #include "nrf51_deprecated.h"
    #elif defined (NRF52)
        #include "nrf52.h"
        #include "nrf52_bitfields.h"
        #include "nrf51_to_nrf52.h"
    #else
        #error "Device family must be defined. See nrf.h."
    #endif /* NRF51, NRF52 */

    #include "compiler_abstraction.h"

#endif /* _WIN32 || __unix || __APPLE__ */

#endif /* NRF_H */

