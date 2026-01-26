/*
Copyright (c) 2018 Jan Gromeš
Copyright (c) 2022 STMicroelectronics

This file is licensed under the MIT License: https://opensource.org/licenses/MIT
*/

#if !defined(_RADIOLIB_STM32WLx_H)
#define _RADIOLIB_STM32WLx_H

#include "../../TypeDef.h"

#if !RADIOLIB_EXCLUDE_STM32WLX

#include "../../Module.h"
#include "SX1262.h"
#include "STM32WLx_Module.h"

/*!
  \class STM32WLx

  \brief Derived class for STM32WL modules.

  The radio integrated into these modules is essentially the same as the
  Semtech %SX126x external radio chips, so most of the documentation for
  those also applies here.

  One notable difference with the %SX126x radios is that this radio
  essentially combines the %SX1261 and %SX1262 by integrating both the
  low-power (LP) and high-power (HP) amplifier. See setOutputPower() and
  setRfSwitchTable() for details on how this is handled.
*/
class STM32WLx : public SX1262 {
  // NOTE: This class could not be named STM32WL (or STM32WLxx), since
  // those are macros defined by
  // system/Drivers/CMSIS/Device/ST/STM32WLxxx/Include/stm32wlxx.h
  public:
    /*!
      \brief Default constructor.
      \param mod Instance of STM32WLx_Module that will be used to communicate with the radio.
    */
    STM32WLx(STM32WLx_Module* mod);

    /*!
      \brief Custom operation modes for STMWLx.
     
      This splits the TX mode into two modes: Low-power and high-power.
      These constants can be used with the setRfSwitchTable() method,
      instead of the Module::OpMode_t constants.
    */
    enum OpMode_t {
        /*! End of table marker, use \ref END_OF_MODE_TABLE constant instead */
        MODE_END_OF_TABLE = Module::MODE_END_OF_TABLE,
        /*! Idle mode */
        MODE_IDLE = Module::MODE_IDLE,
        /*! Receive mode */
        MODE_RX = Module::MODE_RX,
        /*! Low power transmission mode */
        MODE_TX_LP = Module::MODE_TX,
        /*! High power transmission mode */
        MODE_TX_HP,
    };

    // basic methods

    /*!
      \copydoc SX1262::begin
    */
    int16_t begin(float freq = 434.0, float bw = 125.0, uint8_t sf = 9, uint8_t cr = 7, uint8_t syncWord = RADIOLIB_SX126X_SYNC_WORD_PRIVATE, int8_t power = 10, uint16_t preambleLength = 8, float tcxoVoltage = 1.6, bool useRegulatorLDO = false);

    /*!
      \copydoc SX1262::beginFSK
    */
    int16_t beginFSK(float freq = 434.0, float br = 4.8, float freqDev = 5.0, float rxBw = 156.2, int8_t power = 10, uint16_t preambleLength = 16, float tcxoVoltage = 1.6, bool useRegulatorLDO = false);

    // configuration methods

    /*!
      \brief Sets output power. Allowed values are in range from -17 to 22 dBm.

      This automatically switches between the low-power (LP) and high-power (HP) amplifier.

      LP is preferred and supports -17 to +14dBm. When a higher power is
      requested (or the LP amplifier is marked as unavailable using
      setRfSwitchTable()), HP is used, which supports -9 to +22dBm. If the LP is marked as unavailable,
      HP output will be used instead.

      \param power Output power to be set in dBm.

      \returns \ref status_codes
    */
    virtual int16_t setOutputPower(int8_t power) override;

    /*!
      \copybrief Module::setRfSwitchTable

      This method works like Module::setRfSwitchTable(), except that you
      should use STM32WLx::OpMode_t constants for modes, which
      distinguishes between a low-power (LP) and high-power (HP) TX mode.

      For boards that do not support both modes, just omit the
      unsupported mode from the table and it will not be used (and the
      valid power range is adjusted by setOutputPower() accordingly).

      Note that the setRfSwitchTable() method should be called *before* the
      begin() method, to ensure the radio knows which modes are supported
      during initialization.
     */
    // Note: This explicitly inherits this method only to override docs
    using SX126x::setRfSwitchTable;

    /*!
      \brief Sets interrupt service routine to call when DIO1/2/3 activates.
      \param func ISR to call.
    */
    void setDio1Action(void (*func)(void));

    /*!
      \brief Clears interrupt service routine to call when DIO1/2/3 activates.
    */
    void clearDio1Action();

    /*!
      \brief Sets interrupt service routine to call when a packet is received.
      \param func ISR to call.
    */
    void setPacketReceivedAction(void (*func)(void));

    /*!
      \brief Clears interrupt service routine to call when a packet is received.
    */
    void clearPacketReceivedAction();

    /*!
      \brief Sets interrupt service routine to call when a packet is sent.
      \param func ISR to call.
    */
    void setPacketSentAction(void (*func)(void));

    /*!
      \brief Clears interrupt service routine to call when a packet is sent.
    */
    void clearPacketSentAction();

    /*!
      \brief Sets interrupt service routine to call when a channel scan is finished.
      \param func ISR to call.
    */
    void setChannelScanAction(void (*func)(void));

    /*!
      \brief Clears interrupt service routine to call when a channel scan is finished.
    */
    void clearChannelScanAction();

#if !RADIOLIB_GODMODE
  protected:
#endif
    virtual int16_t clearIrqStatus(uint16_t clearIrqParams) override;

#if !RADIOLIB_GODMODE
  private:
#endif
};

#endif

#endif
