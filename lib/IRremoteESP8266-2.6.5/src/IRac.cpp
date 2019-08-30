// Copyright 2019 David Conran

// Provide a universal/standard interface for sending A/C nessages.
// It does not provide complete and maximum granular control but tries
// to off most common functionallity across all supported devices.

#include "IRac.h"
#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <string.h>
#ifndef ARDUINO
#include <string>
#endif
#include "IRsend.h"
#include "IRremoteESP8266.h"
#include "IRutils.h"
#include "ir_Amcor.h"
#include "ir_Argo.h"
#include "ir_Coolix.h"
#include "ir_Daikin.h"
#include "ir_Electra.h"
#include "ir_Fujitsu.h"
#include "ir_Haier.h"
#include "ir_Hitachi.h"
#include "ir_Kelvinator.h"
#include "ir_Midea.h"
#include "ir_Mitsubishi.h"
#include "ir_MitsubishiHeavy.h"
#include "ir_Neoclima.h"
#include "ir_Panasonic.h"
#include "ir_Samsung.h"
#include "ir_Sharp.h"
#include "ir_Tcl.h"
#include "ir_Teco.h"
#include "ir_Toshiba.h"
#include "ir_Trotec.h"
#include "ir_Vestel.h"
#include "ir_Whirlpool.h"

IRac::IRac(const uint16_t pin, const bool inverted, const bool use_modulation) {
  _pin = pin;
  _inverted = inverted;
  _modulation = use_modulation;
}

// Is the given protocol supported by the IRac class?
bool IRac::isProtocolSupported(const decode_type_t protocol) {
  switch (protocol) {
#if SEND_AMCOR
    case decode_type_t::AMCOR:
#endif
#if SEND_AMCOR
    case decode_type_t::ARGO:
#endif
#if SEND_COOLIX
    case decode_type_t::COOLIX:
#endif
#if SEND_DAIKIN
    case decode_type_t::DAIKIN:
#endif
#if SEND_DAIKIN128
    case decode_type_t::DAIKIN128:
#endif
#if SEND_DAIKIN160
    case decode_type_t::DAIKIN160:
#endif
#if SEND_DAIKIN176
    case decode_type_t::DAIKIN176:
#endif
#if SEND_DAIKIN2
    case decode_type_t::DAIKIN2:
#endif
#if SEND_DAIKIN216
    case decode_type_t::DAIKIN216:
#endif
#if SEND_ELECTRA_AC
    case decode_type_t::ELECTRA_AC:
#endif
#if SEND_FUJITSU_AC
    case decode_type_t::FUJITSU_AC:
#endif
#if SEND_GOODWEATHER
    case decode_type_t::GOODWEATHER:
#endif
#if SEND_GREE
    case decode_type_t::GREE:
#endif
#if SEND_HAIER_AC
    case decode_type_t::HAIER_AC:
#endif
#if SEND_HAIER_AC_YRW02
    case decode_type_t::HAIER_AC_YRW02:
#endif
#if SEND_HITACHI_AC
    case decode_type_t::HITACHI_AC:
#endif
#if SEND_KELVINATOR
    case decode_type_t::KELVINATOR:
#endif
#if SEND_MIDEA
    case decode_type_t::MIDEA:
#endif
#if SEND_MITSUBISHI_AC
    case decode_type_t::MITSUBISHI_AC:
#endif
#if SEND_MITSUBISHIHEAVY
    case decode_type_t::MITSUBISHI_HEAVY_88:
    case decode_type_t::MITSUBISHI_HEAVY_152:
#endif
#if SEND_NEOCLIMA
    case decode_type_t::NEOCLIMA:
#endif
#if SEND_PANASONIC_AC
    case decode_type_t::PANASONIC_AC:
#endif
#if SEND_SAMSUNG_AC
    case decode_type_t::SAMSUNG_AC:
#endif
#if SEND_SHARP_AC
    case decode_type_t::SHARP_AC:
#endif
#if SEND_TCL112AC
    case decode_type_t::TCL112AC:
#endif
#if SEND_TECO
    case decode_type_t::TECO:
#endif
#if SEND_TOSHIBA_AC
    case decode_type_t::TOSHIBA_AC:
#endif
#if SEND_TROTEC
    case decode_type_t::TROTEC:
#endif
#if SEND_VESTEL_AC
    case decode_type_t::VESTEL_AC:
#endif
#if SEND_WHIRLPOOL_AC
    case decode_type_t::WHIRLPOOL_AC:
#endif
      return true;
    default:
      return false;
  }
}

#if SEND_AMCOR
void IRac::amcor(IRAmcorAc *ac,
                const bool on, const stdAc::opmode_t mode, const float degrees,
                const stdAc::fanspeed_t fan) {
  ac->setPower(on);
  ac->setMode(ac->convertMode(mode));
  ac->setTemp(degrees);
  ac->setFan(ac->convertFan(fan));
  // No Swing setting available.
  // No Quiet setting available.
  // No Light setting available.
  // No Filter setting available.
  // No Turbo setting available.
  // No Economy setting available.
  // No Clean setting available.
  // No Beep setting available.
  // No Sleep setting available.
  ac->send();
}
#endif  // SEND_AMCOR

#if SEND_ARGO
void IRac::argo(IRArgoAC *ac,
                const bool on, const stdAc::opmode_t mode, const float degrees,
                const stdAc::fanspeed_t fan, const stdAc::swingv_t swingv,
                const bool turbo, const int16_t sleep) {
  ac->setPower(on);
  ac->setMode(ac->convertMode(mode));
  ac->setTemp(degrees);
  ac->setFan(ac->convertFan(fan));
  ac->setFlap(ac->convertSwingV(swingv));
  // No Quiet setting available.
  // No Light setting available.
  // No Filter setting available.
  ac->setMax(turbo);
  // No Economy setting available.
  // No Clean setting available.
  // No Beep setting available.
  ac->setNight(sleep >= 0);  // Convert to a boolean.
  ac->send();
}
#endif  // SEND_ARGO

#if SEND_COOLIX
void IRac::coolix(IRCoolixAC *ac,
                  const bool on, const stdAc::opmode_t mode,
                  const float degrees, const stdAc::fanspeed_t fan,
                  const stdAc::swingv_t swingv, const stdAc::swingh_t swingh,
                  const bool turbo, const bool light, const bool clean,
                  const int16_t sleep) {
  ac->setMode(ac->convertMode(mode));
  ac->setTemp(degrees);
  ac->setFan(ac->convertFan(fan));
  // No Filter setting available.
  // No Beep setting available.
  // No Clock setting available.
  // No Econo setting available.
  // No Quiet setting available.
  if (swingv != stdAc::swingv_t::kOff || swingh != stdAc::swingh_t::kOff) {
    // Swing has a special command that needs to be sent independently.
    ac->setSwing();
    ac->send();
  }
  if (turbo) {
    // Turbo has a special command that needs to be sent independently.
    ac->setTurbo();
    ac->send();
  }
  if (sleep > 0) {
    // Sleep has a special command that needs to be sent independently.
    ac->setSleep();
    ac->send();
  }
  if (light) {
    // Light has a special command that needs to be sent independently.
    ac->setLed();
    ac->send();
  }
  if (clean) {
    // Clean has a special command that needs to be sent independently.
    ac->setClean();
    ac->send();
  }
  // Power gets done last, as off has a special command.
  ac->setPower(on);
  ac->send();
}
#endif  // SEND_COOLIX

#if SEND_DAIKIN
void IRac::daikin(IRDaikinESP *ac,
                  const bool on, const stdAc::opmode_t mode,
                  const float degrees, const stdAc::fanspeed_t fan,
                  const stdAc::swingv_t swingv, const stdAc::swingh_t swingh,
                  const bool quiet, const bool turbo, const bool econo,
                  const bool clean) {
  ac->setPower(on);
  ac->setMode(ac->convertMode(mode));
  ac->setTemp(degrees);
  ac->setFan(ac->convertFan(fan));
  ac->setSwingVertical((int8_t)swingv >= 0);
  ac->setSwingHorizontal((int8_t)swingh >= 0);
  ac->setQuiet(quiet);
  // No Light setting available.
  // No Filter setting available.
  ac->setPowerful(turbo);
  ac->setEcono(econo);
  ac->setMold(clean);
  // No Beep setting available.
  // No Sleep setting available.
  // No Clock setting available.
  ac->send();
}
#endif  // SEND_DAIKIN

#if SEND_DAIKIN128
void IRac::daikin128(IRDaikin128 *ac,
                  const bool on, const stdAc::opmode_t mode,
                  const float degrees, const stdAc::fanspeed_t fan,
                  const stdAc::swingv_t swingv,
                  const bool quiet, const bool turbo, const bool light,
                  const bool econo, const int16_t sleep, const int16_t clock) {
  ac->setPowerToggle(on);
  ac->setMode(ac->convertMode(mode));
  ac->setTemp(degrees);
  ac->setFan(ac->convertFan(fan));
  ac->setSwingVertical((int8_t)swingv >= 0);
  // No Horizontal Swing setting avaliable.
  ac->setQuiet(quiet);
  ac->setLightToggle(light ? kDaikin128BitWall : 0);
  // No Filter setting available.
  ac->setPowerful(turbo);
  ac->setEcono(econo);
  // No Clean setting available.
  // No Beep setting available.
  ac->setSleep(sleep > 0);
  if (clock >= 0) ac->setClock(clock);
  ac->send();
}
#endif  // SEND_DAIKIN128

#if SEND_DAIKIN160
void IRac::daikin160(IRDaikin160 *ac,
                     const bool on, const stdAc::opmode_t mode,
                     const float degrees, const stdAc::fanspeed_t fan,
                     const stdAc::swingv_t swingv) {
  ac->setPower(on);
  ac->setMode(ac->convertMode(mode));
  ac->setTemp(degrees);
  ac->setFan(ac->convertFan(fan));
  ac->setSwingVertical(ac->convertSwingV(swingv));
  ac->send();
}
#endif  // SEND_DAIKIN160

#if SEND_DAIKIN176
void IRac::daikin176(IRDaikin176 *ac,
                     const bool on, const stdAc::opmode_t mode,
                     const float degrees, const stdAc::fanspeed_t fan,
                     const stdAc::swingh_t swingh) {
  ac->setPower(on);
  ac->setMode(ac->convertMode(mode));
  ac->setTemp(degrees);
  ac->setFan(ac->convertFan(fan));
  ac->setSwingHorizontal(ac->convertSwingH(swingh));
  ac->send();
}
#endif  // SEND_DAIKIN176

#if SEND_DAIKIN2
void IRac::daikin2(IRDaikin2 *ac,
                   const bool on, const stdAc::opmode_t mode,
                   const float degrees, const stdAc::fanspeed_t fan,
                   const stdAc::swingv_t swingv, const stdAc::swingh_t swingh,
                   const bool quiet, const bool turbo, const bool light,
                   const bool econo, const bool filter, const bool clean,
                   const bool beep, const int16_t sleep, const int16_t clock) {
  ac->setPower(on);
  ac->setMode(ac->convertMode(mode));
  ac->setTemp(degrees);
  ac->setFan(ac->convertFan(fan));
  ac->setSwingVertical(ac->convertSwingV(swingv));
  ac->setSwingHorizontal((int8_t)swingh >= 0);
  ac->setQuiet(quiet);
  ac->setLight(light);
  ac->setPowerful(turbo);
  ac->setEcono(econo);
  ac->setPurify(filter);
  ac->setMold(clean);
  ac->setBeep(beep);
  if (sleep > 0) ac->enableSleepTimer(sleep);
  if (clock >= 0) ac->setCurrentTime(clock);
  ac->send();
}
#endif  // SEND_DAIKIN2

#if SEND_DAIKIN216
void IRac::daikin216(IRDaikin216 *ac,
                     const bool on, const stdAc::opmode_t mode,
                     const float degrees, const stdAc::fanspeed_t fan,
                     const stdAc::swingv_t swingv, const stdAc::swingh_t swingh,
                     const bool quiet, const bool turbo) {
  ac->setPower(on);
  ac->setMode(ac->convertMode(mode));
  ac->setTemp(degrees);
  ac->setFan(ac->convertFan(fan));
  ac->setSwingVertical((int8_t)swingv >= 0);
  ac->setSwingHorizontal((int8_t)swingh >= 0);
  ac->setQuiet(quiet);
  ac->setPowerful(turbo);
  ac->send();
}
#endif  // SEND_DAIKIN216

#if SEND_ELECTRA_AC
void IRac::electra(IRElectraAc *ac,
                   const bool on, const stdAc::opmode_t mode,
                   const float degrees, const stdAc::fanspeed_t fan,
                   const stdAc::swingv_t swingv,
                   const stdAc::swingh_t swingh) {
  ac->setPower(on);
  ac->setMode(ac->convertMode(mode));
  ac->setTemp(degrees);
  ac->setFan(ac->convertFan(fan));
  ac->setSwingV(swingv != stdAc::swingv_t::kOff);
  ac->setSwingH(swingh != stdAc::swingh_t::kOff);
  // No Quiet setting available.
  // No Turbo setting available.
  // No Light setting available.
  // No Econo setting available.
  // No Filter setting available.
  // No Clean setting available.
  // No Beep setting available.
  // No Sleep setting available.
  // No Clock setting available.
  ac->send();
}
#endif  // SEND_ELECTRA_AC

#if SEND_FUJITSU_AC
void IRac::fujitsu(IRFujitsuAC *ac, const fujitsu_ac_remote_model_t model,
                   const bool on, const stdAc::opmode_t mode,
                   const float degrees, const stdAc::fanspeed_t fan,
                   const stdAc::swingv_t swingv, const stdAc::swingh_t swingh,
                   const bool quiet, const bool turbo, const bool econo) {
  ac->setModel(model);
  if (on) {
    // Do all special messages (except "Off") first,
    // These need to be sent separately.
    switch (ac->getModel()) {
      // Some functions are only available on some models.
      case fujitsu_ac_remote_model_t::ARREB1E:
        if (turbo) {
          ac->setCmd(kFujitsuAcCmdPowerful);
          // Powerful is a separate command.
          ac->send();
        }
        if (econo) {
          ac->setCmd(kFujitsuAcCmdEcono);
          // Econo is a separate command.
          ac->send();
        }
        break;
      default:
        {};
    }
    // Normal operation.
    ac->setMode(ac->convertMode(mode));
    ac->setTemp(degrees);
    ac->setFanSpeed(ac->convertFan(fan));
    uint8_t swing = kFujitsuAcSwingOff;
    if (swingv > stdAc::swingv_t::kOff) swing |= kFujitsuAcSwingVert;
    if (swingh > stdAc::swingh_t::kOff) swing |= kFujitsuAcSwingHoriz;
    ac->setSwing(swing);
    if (quiet) ac->setFanSpeed(kFujitsuAcFanQuiet);
    // No Light setting available.
    // No Filter setting available.
    // No Clean setting available.
    // No Beep setting available.
    // No Sleep setting available.
    // No Clock setting available.
    ac->on();  // Ref: Issue #860
  } else {
    // Off is special case/message. We don't need to send other messages.
    ac->off();
  }
  ac->send();
}
#endif  // SEND_FUJITSU_AC

#if SEND_GOODWEATHER
void IRac::goodweather(IRGoodweatherAc *ac,
                       const bool on, const stdAc::opmode_t mode,
                       const float degrees,
                       const stdAc::fanspeed_t fan,
                       const stdAc::swingv_t swingv,
                       const bool turbo, const bool light,
                       const int16_t sleep) {
  ac->setMode(ac->convertMode(mode));
  ac->setTemp(degrees);
  ac->setFan(ac->convertFan(fan));
  ac->setSwing(swingv == stdAc::swingv_t::kOff ? kGoodweatherSwingOff
                                               : kGoodweatherSwingSlow);
  ac->setTurbo(turbo);
  ac->setLight(light);
  // No Clean setting available.
  ac->setSleep(sleep >= 0);  // Sleep on this A/C is either on or off.
  // No Horizontal Swing setting available.
  // No Econo setting available.
  // No Filter setting available.
  // No Beep setting available.
  // No Quiet setting available.
  // No Clock setting available.
  ac->setPower(on);
  ac->send();
}
#endif  // SEND_GOODWEATHER

#if SEND_GREE
void IRac::gree(IRGreeAC *ac, const gree_ac_remote_model_t model,
                const bool on, const stdAc::opmode_t mode, const float degrees,
                const stdAc::fanspeed_t fan, const stdAc::swingv_t swingv,
                const bool turbo, const bool light, const bool clean,
                const int16_t sleep) {
  ac->setModel(model);
  ac->setPower(on);
  ac->setMode(ac->convertMode(mode));
  ac->setTemp(degrees);
  ac->setFan(ac->convertFan(fan));
  ac->setSwingVertical(swingv == stdAc::swingv_t::kAuto,  // Set auto flag.
                       ac->convertSwingV(swingv));
  ac->setLight(light);
  ac->setTurbo(turbo);
  ac->setXFan(clean);
  ac->setSleep(sleep >= 0);  // Sleep on this A/C is either on or off.
  // No Horizontal Swing setting available.
  // No Econo setting available.
  // No Filter setting available.
  // No Beep setting available.
  // No Quiet setting available.
  // No Clock setting available.
  ac->send();
}
#endif  // SEND_GREE

#if SEND_HAIER_AC
void IRac::haier(IRHaierAC *ac,
                 const bool on, const stdAc::opmode_t mode, const float degrees,
                 const stdAc::fanspeed_t fan, const stdAc::swingv_t swingv,
                 const bool filter, const int16_t sleep, const int16_t clock) {
  ac->setMode(ac->convertMode(mode));
  ac->setTemp(degrees);
  ac->setFan(ac->convertFan(fan));
  ac->setSwing(ac->convertSwingV(swingv));
  // No Horizontal Swing setting available.
  // No Quiet setting available.
  // No Turbo setting available.
  // No Light setting available.
  ac->setHealth(filter);
  // No Clean setting available.
  // No Beep setting available.
  ac->setSleep(sleep >= 0);  // Sleep on this A/C is either on or off.
  if (clock >=0) ac->setCurrTime(clock);
  if (on)
    ac->setCommand(kHaierAcCmdOn);
  else
    ac->setCommand(kHaierAcCmdOff);
  ac->send();
}
#endif  // SEND_HAIER_AC

#if SEND_HAIER_AC_YRW02
void IRac::haierYrwo2(IRHaierACYRW02 *ac,
                      const bool on, const stdAc::opmode_t mode,
                      const float degrees, const stdAc::fanspeed_t fan,
                      const stdAc::swingv_t swingv, const bool turbo,
                      const bool filter, const int16_t sleep) {
  ac->setMode(ac->convertMode(mode));
  ac->setTemp(degrees);
  ac->setFan(ac->convertFan(fan));
  ac->setSwing(ac->convertSwingV(swingv));
  // No Horizontal Swing setting available.
  // No Quiet setting available.
  ac->setTurbo(turbo);
  // No Light setting available.
  ac->setHealth(filter);
  // No Clean setting available.
  // No Beep setting available.
  ac->setSleep(sleep >= 0);  // Sleep on this A/C is either on or off.
  ac->setPower(on);
  ac->send();
}
#endif  // SEND_HAIER_AC_YRW02

#if SEND_HITACHI_AC
void IRac::hitachi(IRHitachiAc *ac,
                   const bool on, const stdAc::opmode_t mode,
                   const float degrees, const stdAc::fanspeed_t fan,
                   const stdAc::swingv_t swingv, const stdAc::swingh_t swingh) {
  ac->setPower(on);
  ac->setMode(ac->convertMode(mode));
  ac->setTemp(degrees);
  ac->setFan(ac->convertFan(fan));
  ac->setSwingVertical(swingv != stdAc::swingv_t::kOff);
  ac->setSwingHorizontal(swingh != stdAc::swingh_t::kOff);
  // No Quiet setting available.
  // No Turbo setting available.
  // No Light setting available.
  // No Filter setting available.
  // No Clean setting available.
  // No Beep setting available.
  // No Sleep setting available.
  // No Clock setting available.
  ac->send();
}
#endif  // SEND_HITACHI_AC

#if SEND_KELVINATOR
void IRac::kelvinator(IRKelvinatorAC *ac,
                      const bool on, const stdAc::opmode_t mode,
                      const float degrees, const stdAc::fanspeed_t fan,
                      const stdAc::swingv_t swingv,
                      const stdAc::swingh_t swingh,
                      const bool quiet, const bool turbo, const bool light,
                      const bool filter, const bool clean) {
  ac->setPower(on);
  ac->setMode(ac->convertMode(mode));
  ac->setTemp(degrees);
  ac->setFan((uint8_t)fan);  // No conversion needed.
  ac->setSwingVertical((int8_t)swingv >= 0);
  ac->setSwingHorizontal((int8_t)swingh >= 0);
  ac->setQuiet(quiet);
  ac->setTurbo(turbo);
  ac->setLight(light);
  ac->setIonFilter(filter);
  ac->setXFan(clean);
  // No Beep setting available.
  // No Sleep setting available.
  // No Clock setting available.
  ac->send();
}
#endif  // SEND_KELVINATOR

#if SEND_MIDEA
void IRac::midea(IRMideaAC *ac,
                 const bool on, const stdAc::opmode_t mode, const bool celsius,
                 const float degrees, const stdAc::fanspeed_t fan,
                 const stdAc::swingv_t swingv, const int16_t sleep) {
  ac->setPower(on);
  ac->setMode(ac->convertMode(mode));
  ac->setUseCelsius(celsius);
  ac->setTemp(degrees, celsius);
  ac->setFan(ac->convertFan(fan));
  ac->setSwingVToggle(swingv != stdAc::swingv_t::kOff);
  // No Horizontal swing setting available.
  // No Quiet setting available.
  // No Turbo setting available.
  // No Light setting available.
  // No Filter setting available.
  // No Clean setting available.
  // No Beep setting available.
  ac->setSleep(sleep >= 0);  // Sleep on this A/C is either on or off.
  // No Clock setting available.
  ac->send();
}
#endif  // SEND_MIDEA

#if SEND_MITSUBISHI_AC
void IRac::mitsubishi(IRMitsubishiAC *ac,
                      const bool on, const stdAc::opmode_t mode,
                      const float degrees,
                      const stdAc::fanspeed_t fan, const stdAc::swingv_t swingv,
                      const stdAc::swingh_t swingh,
                      const bool quiet, const int16_t clock) {
  ac->setPower(on);
  ac->setMode(ac->convertMode(mode));
  ac->setTemp(degrees);
  ac->setFan(ac->convertFan(fan));
  ac->setVane(ac->convertSwingV(swingv));
  ac->setWideVane(ac->convertSwingH(swingh));
  if (quiet) ac->setFan(kMitsubishiAcFanSilent);
  // No Turbo setting available.
  // No Light setting available.
  // No Filter setting available.
  // No Clean setting available.
  // No Beep setting available.
  // No Sleep setting available.
  if (clock >= 0) ac->setClock(clock / 10);  // Clock is in 10 min increments.
  ac->send();
}
#endif  // SEND_MITSUBISHI_AC

#if SEND_MITSUBISHIHEAVY
void IRac::mitsubishiHeavy88(IRMitsubishiHeavy88Ac *ac,
                             const bool on, const stdAc::opmode_t mode,
                             const float degrees,
                             const stdAc::fanspeed_t fan,
                             const stdAc::swingv_t swingv,
                             const stdAc::swingh_t swingh,
                             const bool turbo, const bool econo,
                             const bool clean) {
  ac->setPower(on);
  ac->setMode(ac->convertMode(mode));
  ac->setTemp(degrees);
  ac->setFan(ac->convertFan(fan));
  ac->setSwingVertical(ac->convertSwingV(swingv));
  ac->setSwingHorizontal(ac->convertSwingH(swingh));
  // No Quiet setting available.
  ac->setTurbo(turbo);
  // No Light setting available.
  ac->setEcono(econo);
  // No Filter setting available.
  ac->setClean(clean);
  // No Beep setting available.
  // No Sleep setting available.
  // No Clock setting available.
  ac->send();
}

void IRac::mitsubishiHeavy152(IRMitsubishiHeavy152Ac *ac,
                              const bool on, const stdAc::opmode_t mode,
                              const float degrees,
                              const stdAc::fanspeed_t fan,
                              const stdAc::swingv_t swingv,
                              const stdAc::swingh_t swingh,
                              const bool quiet, const bool turbo,
                              const bool econo, const bool filter,
                              const bool clean, const int16_t sleep) {
  ac->setPower(on);
  ac->setMode(ac->convertMode(mode));
  ac->setTemp(degrees);
  ac->setFan(ac->convertFan(fan));
  ac->setSwingVertical(ac->convertSwingV(swingv));
  ac->setSwingHorizontal(ac->convertSwingH(swingh));
  ac->setSilent(quiet);
  ac->setTurbo(turbo);
  // No Light setting available.
  ac->setEcono(econo);
  ac->setClean(clean);
  ac->setFilter(filter);
  // No Beep setting available.
  ac->setNight(sleep >= 0);  // Sleep is either on/off, so convert to boolean.
  // No Clock setting available.
  ac->send();
}
#endif  // SEND_MITSUBISHIHEAVY

#if SEND_NEOCLIMA
void IRac::neoclima(IRNeoclimaAc *ac,
                    const bool on, const stdAc::opmode_t mode,
                    const float degrees, const stdAc::fanspeed_t fan,
                    const stdAc::swingv_t swingv, const stdAc::swingh_t swingh,
                    const bool turbo, const bool light, const bool filter,
                    const int16_t sleep) {
  ac->setMode(ac->convertMode(mode));
  ac->setTemp(degrees);
  ac->setFan(ac->convertFan(fan));
  ac->setSwingV(swingv != stdAc::swingv_t::kOff);
  ac->setSwingH(swingh != stdAc::swingh_t::kOff);
  // No Quiet setting available.
  ac->setTurbo(turbo);
  ac->setLight(light);
  // No Econo setting available.
  ac->setIon(filter);
  // No Clean setting available.
  // No Beep setting available.
  ac->setSleep(sleep >= 0);  // Sleep is either on/off, so convert to boolean.
  // No Clock setting available.
  ac->setPower(on);
  ac->send();
}
#endif  // SEND_NEOCLIMA

#if SEND_PANASONIC_AC
void IRac::panasonic(IRPanasonicAc *ac, const panasonic_ac_remote_model_t model,
                     const bool on, const stdAc::opmode_t mode,
                     const float degrees, const stdAc::fanspeed_t fan,
                     const stdAc::swingv_t swingv, const stdAc::swingh_t swingh,
                     const bool quiet, const bool turbo, const int16_t clock) {
  ac->setModel(model);
  ac->setPower(on);
  ac->setMode(ac->convertMode(mode));
  ac->setTemp(degrees);
  ac->setFan(ac->convertFan(fan));
  ac->setSwingVertical(ac->convertSwingV(swingv));
  ac->setSwingHorizontal(ac->convertSwingH(swingh));
  ac->setQuiet(quiet);
  ac->setPowerful(turbo);
  // No Light setting available.
  // No Econo setting available.
  // No Filter setting available.
  // No Clean setting available.
  // No Beep setting available.
  // No Sleep setting available.
  if (clock >= 0) ac->setClock(clock);
  ac->send();
}
#endif  // SEND_PANASONIC_AC

#if SEND_SAMSUNG_AC
void IRac::samsung(IRSamsungAc *ac,
                   const bool on, const stdAc::opmode_t mode,
                   const float degrees,
                   const stdAc::fanspeed_t fan, const stdAc::swingv_t swingv,
                   const bool quiet, const bool turbo, const bool clean,
                   const bool beep, const bool dopower) {
  // dopower is for unit testing only. It should only ever be false in tests.
  if (dopower) ac->setPower(on);
  ac->setMode(ac->convertMode(mode));
  ac->setTemp(degrees);
  ac->setFan(ac->convertFan(fan));
  ac->setSwing(swingv != stdAc::swingv_t::kOff);
  // No Horizontal swing setting available.
  ac->setQuiet(quiet);
  ac->setPowerful(turbo);
  // No Light setting available.
  // No Econo setting available.
  // No Filter setting available.
  ac->setClean(clean);
  ac->setBeep(beep);
  // No Sleep setting available.
  // No Clock setting available.
  // Do setMode() again as it can affect fan speed.
  ac->setMode(ac->convertMode(mode));
  ac->send();
}
#endif  // SEND_SAMSUNG_AC

#if SEND_SHARP_AC
void IRac::sharp(IRSharpAc *ac,
                 const bool on, const stdAc::opmode_t mode,
                 const float degrees, const stdAc::fanspeed_t fan) {
  ac->setPower(on);
  ac->setMode(ac->convertMode(mode));
  ac->setTemp(degrees);
  ac->setFan(ac->convertFan(fan));
  // No Vertical swing setting available.
  // No Horizontal swing setting available.
  // No Quiet setting available.
  // No Turbo setting available.
  // No Light setting available.
  // No Econo setting available.
  // No Filter setting available.
  // No Clean setting available.
  // No Beep setting available.
  // No Sleep setting available.
  // No Clock setting available.
  // Do setMode() again as it can affect fan speed and temp.
  ac->setMode(ac->convertMode(mode));
  ac->send();
}
#endif  // SEND_SHARP_AC

#if SEND_TCL112AC
void IRac::tcl112(IRTcl112Ac *ac,
                  const bool on, const stdAc::opmode_t mode,
                  const float degrees, const stdAc::fanspeed_t fan,
                  const stdAc::swingv_t swingv, const stdAc::swingh_t swingh,
                  const bool turbo, const bool light, const bool econo,
                  const bool filter) {
  ac->setPower(on);
  ac->setMode(ac->convertMode(mode));
  ac->setTemp(degrees);
  ac->setFan(ac->convertFan(fan));
  ac->setSwingVertical(swingv != stdAc::swingv_t::kOff);
  ac->setSwingHorizontal(swingh != stdAc::swingh_t::kOff);
  // No Quiet setting available.
  ac->setTurbo(turbo);
  ac->setLight(light);
  ac->setEcono(econo);
  ac->setHealth(filter);
  // No Clean setting available.
  // No Beep setting available.
  // No Sleep setting available.
  // No Clock setting available.
  ac->send();
}
#endif  // SEND_TCL112AC

#if SEND_TECO
void IRac::teco(IRTecoAc *ac,
                const bool on, const stdAc::opmode_t mode, const float degrees,
                const stdAc::fanspeed_t fan, const stdAc::swingv_t swingv,
                const bool light, const int16_t sleep) {
  ac->setPower(on);
  ac->setMode(ac->convertMode(mode));
  ac->setTemp(degrees);
  ac->setFan(ac->convertFan(fan));
  ac->setSwing(swingv != stdAc::swingv_t::kOff);
  // No Horizontal swing setting available.
  // No Quiet setting available.
  // No Turbo setting available.
  ac->setLight(light);
  // No Filter setting available.
  // No Clean setting available.
  // No Beep setting available.
  ac->setSleep(sleep >= 0);  // Sleep is either on/off, so convert to boolean.
  // No Clock setting available.
  ac->send();
}
#endif  // SEND_TECO

#if SEND_TOSHIBA_AC
void IRac::toshiba(IRToshibaAC *ac,
                   const bool on, const stdAc::opmode_t mode,
                   const float degrees, const stdAc::fanspeed_t fan) {
  ac->setPower(on);
  ac->setMode(ac->convertMode(mode));
  ac->setTemp(degrees);
  ac->setFan(ac->convertFan(fan));
  // No Vertical swing setting available.
  // No Horizontal swing setting available.
  // No Quiet setting available.
  // No Turbo setting available.
  // No Light setting available.
  // No Filter setting available.
  // No Clean setting available.
  // No Beep setting available.
  // No Sleep setting available.
  // No Clock setting available.
  ac->send();
}
#endif  // SEND_TOSHIBA_AC

#if SEND_TROTEC
void IRac::trotec(IRTrotecESP *ac,
                  const bool on, const stdAc::opmode_t mode,
                  const float degrees, const stdAc::fanspeed_t fan,
                  const int16_t sleep) {
  ac->setPower(on);
  ac->setMode(ac->convertMode(mode));
  ac->setTemp(degrees);
  ac->setSpeed(ac->convertFan(fan));
  // No Vertical swing setting available.
  // No Horizontal swing setting available.
  // No Quiet setting available.
  // No Turbo setting available.
  // No Light setting available.
  // No Filter setting available.
  // No Clean setting available.
  // No Beep setting available.
  ac->setSleep(sleep >= 0);  // Sleep is either on/off, so convert to boolean.
  // No Clock setting available.
  ac->send();
}
#endif  // SEND_TROTEC

#if SEND_VESTEL_AC
void IRac::vestel(IRVestelAc *ac,
                  const bool on, const stdAc::opmode_t mode,
                  const float degrees,
                  const stdAc::fanspeed_t fan, const stdAc::swingv_t swingv,
                  const bool turbo, const bool filter, const int16_t sleep,
                  const int16_t clock, const bool sendNormal) {
  ac->setPower(on);
  ac->setMode(ac->convertMode(mode));
  ac->setTemp(degrees);
  ac->setFan(ac->convertFan(fan));
  ac->setSwing(swingv != stdAc::swingv_t::kOff);
  // No Horizontal swing setting available.
  // No Quiet setting available.
  ac->setTurbo(turbo);
  // No Light setting available.
  ac->setIon(filter);
  // No Clean setting available.
  // No Beep setting available.
  ac->setSleep(sleep >= 0);  // Sleep is either on/off, so convert to boolean.
  if (sendNormal) ac->send();  // Send the normal message.
  if (clock >= 0) {
    ac->setTime(clock);
    ac->send();  // Setting the clock requires a different "timer" message.
  }
}
#endif  // SEND_VESTEL_AC

#if SEND_WHIRLPOOL_AC
void IRac::whirlpool(IRWhirlpoolAc *ac, const whirlpool_ac_remote_model_t model,
                     const bool on, const stdAc::opmode_t mode,
                     const float degrees,
                     const stdAc::fanspeed_t fan, const stdAc::swingv_t swingv,
                     const bool turbo, const bool light,
                     const int16_t sleep, const int16_t clock) {
  ac->setModel(model);
  ac->setMode(ac->convertMode(mode));
  ac->setTemp(degrees);
  ac->setFan(ac->convertFan(fan));
  ac->setSwing(swingv != stdAc::swingv_t::kOff);
  // No Horizontal swing setting available.
  // No Quiet setting available.
  ac->setSuper(turbo);
  ac->setLight(light);
  // No Filter setting available
  // No Clean setting available.
  // No Beep setting available.
  ac->setSleep(sleep >= 0);  // Sleep is either on/off, so convert to boolean.
  if (clock >= 0) ac->setClock(clock);
  ac->setPowerToggle(on);
  ac->send();
}
#endif  // SEND_WHIRLPOOL_AC

// Create a new state base on desired & previous states but handle
// any state changes for options that need to be toggled.
// Args:
//   desired: The state_t structure describing the desired a/c state.
//   prev:    Ptr to the previous state_t structure.
//
// Returns:
//   A stdAc::state_t with the needed settings.
stdAc::state_t IRac::handleToggles(const stdAc::state_t desired,
                                   const stdAc::state_t *prev) {
  stdAc::state_t result = desired;
  // If we've been given a previous state AND the it's the same A/C basically.
  if (prev != NULL && desired.protocol == prev->protocol &&
      desired.model == prev->model) {
    // Check if we have to handle toggle settings for specific A/C protocols.
    switch (desired.protocol) {
      case decode_type_t::COOLIX:
        if ((desired.swingv == stdAc::swingv_t::kOff) ^
            (prev->swingv == stdAc::swingv_t::kOff))  // It changed, so toggle.
          result.swingv = stdAc::swingv_t::kAuto;
        else
          result.swingv = stdAc::swingv_t::kOff;  // No change, so no toggle.
        result.turbo = desired.turbo ^ prev->turbo;
        result.light = desired.light ^ prev->light;
        result.clean = desired.clean ^ prev->clean;
        result.sleep = ((desired.sleep >= 0) ^ (prev->sleep >= 0)) ? 0 : -1;
        break;
      case decode_type_t::DAIKIN128:
        result.power = desired.power ^ prev->power;
        result.light = desired.light ^ prev->light;
        break;
      case decode_type_t::MIDEA:
        if ((desired.swingv == stdAc::swingv_t::kOff) ^
            (prev->swingv == stdAc::swingv_t::kOff))  // It changed, so toggle.
          result.swingv = stdAc::swingv_t::kAuto;
        else
          result.swingv = stdAc::swingv_t::kOff;  // No change, so no toggle.
        break;
      case decode_type_t::WHIRLPOOL_AC:
        result.power = desired.power ^ prev->power;
        break;
      case decode_type_t::PANASONIC_AC:
        // CKP models use a power mode toggle.
        if (desired.model == panasonic_ac_remote_model_t::kPanasonicCkp)
          result.power = desired.power ^ prev->power;
        break;
      default:
        {};
    }
  }
  return result;
}

// Send A/C message for a given device using common A/C settings.
// Args:
//   vendor:  The type of A/C protocol to use.
//   model:   The specific model of A/C if supported/applicable.
//   on:      Should the unit be powered on? (or in some cases, toggled)
//   mode:    What operating mode should the unit perform? e.g. Cool, Heat etc.
//   degrees: What temperature should the unit be set to?
//   celsius: Use degrees Celsius, otherwise Fahrenheit.
//   fan:     Fan speed.
// The following args are all "if supported" by the underlying A/C classes.
//   swingv:  Control the vertical swing of the vanes.
//   swingh:  Control the horizontal swing of the vanes.
//   quiet:   Set the unit to quiet (fan) operation mode.
//   turbo:   Set the unit to turbo operating mode. e.g. Max fan & cooling etc.
//   econo:   Set the unit to economical operating mode.
//   light:   Turn on the display/LEDs etc.
//   filter:  Turn on any particle/ion/allergy filter etc.
//   clean:   Turn on any settings to reduce mold etc. (Not self-clean mode.)
//   beep:    Control if the unit beeps upon receiving commands.
//   sleep:   Nr. of mins of sleep mode, or use sleep mode. (< 0 means off.)
//   clock:   Nr. of mins past midnight to set the clock to. (< 0 means off.)
// Returns:
//   boolean: True, if accepted/converted/attempted. False, if unsupported.
bool IRac::sendAc(const decode_type_t vendor, const int16_t model,
                  const bool power, const stdAc::opmode_t mode,
                  const float degrees, const bool celsius,
                  const stdAc::fanspeed_t fan,
                  const stdAc::swingv_t swingv, const stdAc::swingh_t swingh,
                  const bool quiet, const bool turbo, const bool econo,
                  const bool light, const bool filter, const bool clean,
                  const bool beep, const int16_t sleep, const int16_t clock) {
  // Convert the temperature to Celsius.
  float degC;
  if (celsius)
    degC = degrees;
  else
    degC = fahrenheitToCelsius(degrees);
  bool on = power;
  // A hack for Home Assistant, it appears to need/want an Off opmode.
  if (mode == stdAc::opmode_t::kOff) on = false;
  // Per vendor settings & setup.
  switch (vendor) {
#if SEND_AMCOR
    case AMCOR:
    {
      IRAmcorAc ac(_pin, _inverted, _modulation);
      amcor(&ac, on, mode, degC, fan);
      break;
    }
#endif  // SEND_AMCOR
#if SEND_ARGO
    case ARGO:
    {
      IRArgoAC ac(_pin, _inverted, _modulation);
      argo(&ac, on, mode, degC, fan, swingv, turbo, sleep);
      break;
    }
#endif  // SEND_ARGO
#if SEND_COOLIX
    case COOLIX:
    {
      IRCoolixAC ac(_pin, _inverted, _modulation);
      coolix(&ac, on, mode, degC, fan, swingv, swingh,
             turbo, light, clean, sleep);
      break;
    }
#endif  // SEND_COOLIX
#if SEND_DAIKIN
    case DAIKIN:
    {
      IRDaikinESP ac(_pin, _inverted, _modulation);
      daikin(&ac, on, mode, degC, fan, swingv, swingh,
             quiet, turbo, econo, clean);
      break;
    }
#endif  // SEND_DAIKIN
#if SEND_DAIKIN128
    case DAIKIN128:
    {
      IRDaikin128 ac(_pin, _inverted, _modulation);
      daikin128(&ac, on, mode, degC, fan, swingv, quiet, turbo,
                light, econo, sleep, clock);
      break;
    }
#endif  // SEND_DAIKIN2
#if SEND_DAIKIN160
    case DAIKIN160:
    {
      IRDaikin160 ac(_pin, _inverted, _modulation);
      daikin160(&ac, on, mode, degC, fan, swingv);
      break;
    }
#endif  // SEND_DAIKIN160
#if SEND_DAIKIN176
    case DAIKIN176:
    {
      IRDaikin176 ac(_pin, _inverted, _modulation);
      daikin176(&ac, on, mode, degC, fan, swingh);
      break;
    }
#endif  // SEND_DAIKIN176
#if SEND_DAIKIN2
    case DAIKIN2:
    {
      IRDaikin2 ac(_pin, _inverted, _modulation);
      daikin2(&ac, on, mode, degC, fan, swingv, swingh, quiet, turbo,
              light, econo, filter, clean, beep, sleep, clock);
      break;
    }
#endif  // SEND_DAIKIN2
#if SEND_DAIKIN216
    case DAIKIN216:
    {
      IRDaikin216 ac(_pin, _inverted, _modulation);
      daikin216(&ac, on, mode, degC, fan, swingv, swingh, quiet, turbo);
      break;
    }
#endif  // SEND_DAIKIN216
#if SEND_ELECTRA_AC
    case ELECTRA_AC:
    {
      IRElectraAc ac(_pin, _inverted, _modulation);
      ac.begin();
      electra(&ac, on, mode, degC, fan, swingv, swingh);
      break;
    }
#endif  // SEND_ELECTRA_AC
#if SEND_FUJITSU_AC
    case FUJITSU_AC:
    {
      IRFujitsuAC ac(_pin, (fujitsu_ac_remote_model_t)model, _inverted,
                     _modulation);
      ac.begin();
      fujitsu(&ac, (fujitsu_ac_remote_model_t)model, on, mode, degC, fan,
              swingv, swingh, quiet, turbo, econo);
      break;
    }
#endif  // SEND_FUJITSU_AC
#if SEND_GOODWEATHER
    case GOODWEATHER:
    {
      IRGoodweatherAc ac(_pin, _inverted, _modulation);
      ac.begin();
      goodweather(&ac, on, mode, degC, fan, swingv, turbo, light, sleep);
      break;
    }
#endif  // SEND_GOODWEATHER
#if SEND_GREE
    case GREE:
    {
      IRGreeAC ac(_pin, (gree_ac_remote_model_t)model, _inverted, _modulation);
      ac.begin();
      gree(&ac, (gree_ac_remote_model_t)model, on, mode, degC, fan, swingv,
           turbo, light, clean, sleep);
      break;
    }
#endif  // SEND_GREE
#if SEND_HAIER_AC
    case HAIER_AC:
    {
      IRHaierAC ac(_pin, _inverted, _modulation);
      ac.begin();
      haier(&ac, on, mode, degC, fan, swingv, filter, sleep, clock);
      break;
    }
#endif  // SEND_HAIER_AC
#if SEND_HAIER_AC_YRW02
    case HAIER_AC_YRW02:
    {
      IRHaierACYRW02 ac(_pin, _inverted, _modulation);
      ac.begin();
      haierYrwo2(&ac, on, mode, degC, fan, swingv, turbo, filter, sleep);
      break;
    }
#endif  // SEND_HAIER_AC_YRW02
#if SEND_HITACHI_AC
    case HITACHI_AC:
    {
      IRHitachiAc ac(_pin, _inverted, _modulation);
      ac.begin();
      hitachi(&ac, on, mode, degC, fan, swingv, swingh);
      break;
    }
#endif  // SEND_HITACHI_AC
#if SEND_KELVINATOR
    case KELVINATOR:
    {
      IRKelvinatorAC ac(_pin, _inverted, _modulation);
      ac.begin();
      kelvinator(&ac, on, mode, degC, fan, swingv, swingh, quiet, turbo,
                 light, filter, clean);
      break;
    }
#endif  // SEND_KELVINATOR
#if SEND_MIDEA
    case MIDEA:
    {
      IRMideaAC ac(_pin, _inverted, _modulation);
      ac.begin();
      midea(&ac, on, mode, celsius, degrees, fan, swingv, sleep);
      break;
    }
#endif  // SEND_MIDEA
#if SEND_MITSUBISHI_AC
    case MITSUBISHI_AC:
    {
      IRMitsubishiAC ac(_pin, _inverted, _modulation);
      ac.begin();
      mitsubishi(&ac, on, mode, degC, fan, swingv, swingh, quiet, clock);
      break;
    }
#endif  // SEND_MITSUBISHI_AC
#if SEND_MITSUBISHIHEAVY
    case MITSUBISHI_HEAVY_88:
    {
      IRMitsubishiHeavy88Ac ac(_pin, _inverted, _modulation);
      ac.begin();
      mitsubishiHeavy88(&ac, on, mode, degC, fan, swingv, swingh,
                        turbo, econo, clean);
      break;
    }
    case MITSUBISHI_HEAVY_152:
    {
      IRMitsubishiHeavy152Ac ac(_pin, _inverted, _modulation);
      ac.begin();
      mitsubishiHeavy152(&ac, on, mode, degC, fan, swingv, swingh,
                         quiet, turbo, econo, filter, clean, sleep);
      break;
    }
#endif  // SEND_MITSUBISHIHEAVY
#if SEND_NEOCLIMA
    case NEOCLIMA:
    {
      IRNeoclimaAc ac(_pin, _inverted, _modulation);
      ac.begin();
      neoclima(&ac, on, mode, degC, fan, swingv, swingh, turbo, light, filter,
               sleep);
      break;
    }
#endif  // SEND_NEOCLIMA
#if SEND_PANASONIC_AC
    case PANASONIC_AC:
    {
      IRPanasonicAc ac(_pin, _inverted, _modulation);
      ac.begin();
      panasonic(&ac, (panasonic_ac_remote_model_t)model, on, mode, degC, fan,
                swingv, swingh, quiet, turbo, clock);
      break;
    }
#endif  // SEND_PANASONIC_AC
#if SEND_SAMSUNG_AC
    case SAMSUNG_AC:
    {
      IRSamsungAc ac(_pin, _inverted, _modulation);
      ac.begin();
      samsung(&ac, on, mode, degC, fan, swingv, quiet, turbo, clean, beep);
      break;
    }
#endif  // SEND_SAMSUNG_AC
#if SEND_SHARP_AC
    case SHARP_AC:
    {
      IRSharpAc ac(_pin, _inverted, _modulation);
      ac.begin();
      sharp(&ac, on, mode, degC, fan);
      break;
    }
#endif  // SEND_SHARP_AC
#if SEND_TCL112AC
    case TCL112AC:
    {
      IRTcl112Ac ac(_pin, _inverted, _modulation);
      ac.begin();
      tcl112(&ac, on, mode, degC, fan, swingv, swingh, turbo, light, econo,
             filter);
      break;
    }
#endif  // SEND_TCL112AC
#if SEND_TECO
    case TECO:
    {
      IRTecoAc ac(_pin, _inverted, _modulation);
      ac.begin();
      teco(&ac, on, mode, degC, fan, swingv, light, sleep);
      break;
    }
#endif  // SEND_TECO
#if SEND_TOSHIBA_AC
    case TOSHIBA_AC:
    {
      IRToshibaAC ac(_pin, _inverted, _modulation);
      ac.begin();
      toshiba(&ac, on, mode, degC, fan);
      break;
    }
#endif  // SEND_TOSHIBA_AC
#if SEND_TROTEC
    case TROTEC:
    {
      IRTrotecESP ac(_pin, _inverted, _modulation);
      ac.begin();
      trotec(&ac, on, mode, degC, fan, sleep);
      break;
    }
#endif  // SEND_TROTEC
#if SEND_VESTEL_AC
    case VESTEL_AC:
    {
      IRVestelAc ac(_pin, _inverted, _modulation);
      ac.begin();
      vestel(&ac, on, mode, degC, fan, swingv, turbo, filter, sleep, clock);
      break;
    }
#endif  // SEND_VESTEL_AC
#if SEND_WHIRLPOOL_AC
    case WHIRLPOOL_AC:
    {
      IRWhirlpoolAc ac(_pin, _inverted, _modulation);
      ac.begin();
      whirlpool(&ac, (whirlpool_ac_remote_model_t)model, on, mode, degC, fan,
                swingv, turbo, light, sleep, clock);
      break;
    }
#endif  // SEND_WHIRLPOOL_AC
    default:
      return false;  // Fail, didn't match anything.
  }
  return true;  // Success.
}

// Send A/C message for a given device using state_t structures.
// Args:
//   desired: The state_t structure describing the desired new a/c state.
//   prev:    Ptr to the previous state_t structure.
//
// Returns:
//   boolean: True, if accepted/converted/attempted. False, if unsupported.
bool IRac::sendAc(const stdAc::state_t desired, const stdAc::state_t *prev) {
  stdAc::state_t final = this->handleToggles(desired, prev);
  return this->sendAc(final.protocol, final.model, final.power, final.mode,
                      final.degrees, final.celsius, final.fanspeed,
                      final.swingv, final.swingh, final.quiet, final.turbo,
                      final.econo, final.light, final.filter, final.clean,
                      final.beep, final.sleep, final.clock);
}

// Compare two AirCon states.
// Returns: True if they differ, False if they don't.
// Note: Excludes clock.
bool IRac::cmpStates(const stdAc::state_t a, const stdAc::state_t b) {
  return a.protocol != b.protocol || a.model != b.model || a.power != b.power ||
      a.mode != b.mode || a.degrees != b.degrees || a.celsius != b.celsius ||
      a.fanspeed != b.fanspeed || a.swingv != b.swingv ||
      a.swingh != b.swingh || a.quiet != b.quiet || a.turbo != b.turbo ||
      a.econo != b.econo || a.light != b.light || a.filter != b.filter ||
      a.clean != b.clean || a.beep != b.beep || a.sleep != b.sleep;
}

stdAc::opmode_t IRac::strToOpmode(const char *str,
                                const stdAc::opmode_t def) {
  if (!strcasecmp(str, "AUTO") || !strcasecmp(str, "AUTOMATIC"))
    return stdAc::opmode_t::kAuto;
  else if (!strcasecmp(str, "OFF") || !strcasecmp(str, "STOP"))
    return stdAc::opmode_t::kOff;
  else if (!strcasecmp(str, "COOL") || !strcasecmp(str, "COOLING"))
    return stdAc::opmode_t::kCool;
  else if (!strcasecmp(str, "HEAT") || !strcasecmp(str, "HEATING"))
    return stdAc::opmode_t::kHeat;
  else if (!strcasecmp(str, "DRY") || !strcasecmp(str, "DRYING") ||
           !strcasecmp(str, "DEHUMIDIFY"))
    return stdAc::opmode_t::kDry;
  else if (!strcasecmp(str, "FAN") || !strcasecmp(str, "FANONLY") ||
           !strcasecmp(str, "FAN_ONLY"))
    return stdAc::opmode_t::kFan;
  else
    return def;
}

stdAc::fanspeed_t IRac::strToFanspeed(const char *str,
                                      const stdAc::fanspeed_t def) {
  if (!strcasecmp(str, "AUTO") || !strcasecmp(str, "AUTOMATIC"))
    return stdAc::fanspeed_t::kAuto;
  else if (!strcasecmp(str, "MIN") || !strcasecmp(str, "MINIMUM") ||
           !strcasecmp(str, "LOWEST"))
    return stdAc::fanspeed_t::kMin;
  else if (!strcasecmp(str, "LOW"))
    return stdAc::fanspeed_t::kLow;
  else if (!strcasecmp(str, "MED") || !strcasecmp(str, "MEDIUM") ||
           !strcasecmp(str, "MID"))
    return stdAc::fanspeed_t::kMedium;
  else if (!strcasecmp(str, "HIGH") || !strcasecmp(str, "HI"))
    return stdAc::fanspeed_t::kHigh;
  else if (!strcasecmp(str, "MAX") || !strcasecmp(str, "MAXIMUM") ||
           !strcasecmp(str, "HIGHEST"))
    return stdAc::fanspeed_t::kMax;
  else
    return def;
}

stdAc::swingv_t IRac::strToSwingV(const char *str,
                                  const stdAc::swingv_t def) {
  if (!strcasecmp(str, "AUTO") || !strcasecmp(str, "AUTOMATIC") ||
      !strcasecmp(str, "ON") || !strcasecmp(str, "SWING"))
    return stdAc::swingv_t::kAuto;
  else if (!strcasecmp(str, "OFF") || !strcasecmp(str, "STOP"))
    return stdAc::swingv_t::kOff;
  else if (!strcasecmp(str, "MIN") || !strcasecmp(str, "MINIMUM") ||
           !strcasecmp(str, "LOWEST") || !strcasecmp(str, "BOTTOM") ||
           !strcasecmp(str, "DOWN"))
    return stdAc::swingv_t::kLowest;
  else if (!strcasecmp(str, "LOW"))
    return stdAc::swingv_t::kLow;
  else if (!strcasecmp(str, "MID") || !strcasecmp(str, "MIDDLE") ||
           !strcasecmp(str, "MED") || !strcasecmp(str, "MEDIUM") ||
           !strcasecmp(str, "CENTRE") || !strcasecmp(str, "CENTER"))
    return stdAc::swingv_t::kMiddle;
  else if (!strcasecmp(str, "HIGH") || !strcasecmp(str, "HI"))
    return stdAc::swingv_t::kHigh;
  else if (!strcasecmp(str, "HIGHEST") || !strcasecmp(str, "MAX") ||
           !strcasecmp(str, "MAXIMUM") || !strcasecmp(str, "TOP") ||
           !strcasecmp(str, "UP"))
    return stdAc::swingv_t::kHighest;
  else
    return def;
}

stdAc::swingh_t IRac::strToSwingH(const char *str,
                                  const stdAc::swingh_t def) {
  if (!strcasecmp(str, "AUTO") || !strcasecmp(str, "AUTOMATIC") ||
      !strcasecmp(str, "ON") || !strcasecmp(str, "SWING"))
    return stdAc::swingh_t::kAuto;
  else if (!strcasecmp(str, "OFF") || !strcasecmp(str, "STOP"))
    return stdAc::swingh_t::kOff;
  else if (!strcasecmp(str, "LEFTMAX") || !strcasecmp(str, "LEFT MAX") ||
           !strcasecmp(str, "MAXLEFT") || !strcasecmp(str, "MAX LEFT") ||
           !strcasecmp(str, "FARLEFT") || !strcasecmp(str, "FAR LEFT"))
    return stdAc::swingh_t::kLeftMax;
  else if (!strcasecmp(str, "LEFT"))
    return stdAc::swingh_t::kLeft;
  else if (!strcasecmp(str, "MID") || !strcasecmp(str, "MIDDLE") ||
           !strcasecmp(str, "MED") || !strcasecmp(str, "MEDIUM") ||
           !strcasecmp(str, "CENTRE") || !strcasecmp(str, "CENTER"))
    return stdAc::swingh_t::kMiddle;
  else if (!strcasecmp(str, "RIGHT"))
    return stdAc::swingh_t::kRight;
  else if (!strcasecmp(str, "RIGHTMAX") || !strcasecmp(str, "RIGHT MAX") ||
           !strcasecmp(str, "MAXRIGHT") || !strcasecmp(str, "MAX RIGHT") ||
           !strcasecmp(str, "FARRIGHT") || !strcasecmp(str, "FAR RIGHT"))
    return stdAc::swingh_t::kRightMax;
  else if (!strcasecmp(str, "WIDE"))
    return stdAc::swingh_t::kWide;
  else
    return def;
}

// Assumes str is the model or an integer >= 1.
int16_t IRac::strToModel(const char *str, const int16_t def) {
  // Gree
  if (!strcasecmp(str, "YAW1F")) {
    return gree_ac_remote_model_t::YAW1F;
  } else if (!strcasecmp(str, "YBOFB")) {
    return gree_ac_remote_model_t::YBOFB;
  // Fujitsu A/C models
  } else if (!strcasecmp(str, "ARRAH2E")) {
    return fujitsu_ac_remote_model_t::ARRAH2E;
  } else if (!strcasecmp(str, "ARDB1")) {
    return fujitsu_ac_remote_model_t::ARDB1;
  } else if (!strcasecmp(str, "ARREB1E")) {
    return fujitsu_ac_remote_model_t::ARREB1E;
  } else if (!strcasecmp(str, "ARJW2")) {
    return fujitsu_ac_remote_model_t::ARJW2;
  // Panasonic A/C families
  } else if (!strcasecmp(str, "LKE") || !strcasecmp(str, "PANASONICLKE")) {
    return panasonic_ac_remote_model_t::kPanasonicLke;
  } else if (!strcasecmp(str, "NKE") || !strcasecmp(str, "PANASONICNKE")) {
    return panasonic_ac_remote_model_t::kPanasonicNke;
  } else if (!strcasecmp(str, "DKE") || !strcasecmp(str, "PANASONICDKE")) {
    return panasonic_ac_remote_model_t::kPanasonicDke;
  } else if (!strcasecmp(str, "JKE") || !strcasecmp(str, "PANASONICJKE")) {
    return panasonic_ac_remote_model_t::kPanasonicJke;
  } else if (!strcasecmp(str, "CKP") || !strcasecmp(str, "PANASONICCKP")) {
    return panasonic_ac_remote_model_t::kPanasonicCkp;
  } else if (!strcasecmp(str, "RKR") || !strcasecmp(str, "PANASONICRKR")) {
    return panasonic_ac_remote_model_t::kPanasonicRkr;
  // Whirlpool A/C models
  } else if (!strcasecmp(str, "DG11J13A") || !strcasecmp(str, "DG11J104") ||
             !strcasecmp(str, "DG11J1-04")) {
    return whirlpool_ac_remote_model_t::DG11J13A;
  } else if (!strcasecmp(str, "DG11J191")) {
    return whirlpool_ac_remote_model_t::DG11J191;
  } else {
    int16_t number = atoi(str);
    if (number > 0)
      return number;
    else
      return def;
  }
}

bool IRac::strToBool(const char *str, const bool def) {
  if (!strcasecmp(str, "ON") || !strcasecmp(str, "1") ||
      !strcasecmp(str, "YES") || !strcasecmp(str, "TRUE"))
    return true;
  else if (!strcasecmp(str, "OFF") || !strcasecmp(str, "0") ||
           !strcasecmp(str, "NO") || !strcasecmp(str, "FALSE"))
    return false;
  else
    return def;
}

String IRac::boolToString(const bool value) {
  return value ? F("on") : F("off");
}

String IRac::opmodeToString(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kOff:
      return F("off");
    case stdAc::opmode_t::kAuto:
      return F("auto");
    case stdAc::opmode_t::kCool:
      return F("cool");
    case stdAc::opmode_t::kHeat:
      return F("heat");
    case stdAc::opmode_t::kDry:
      return F("dry");
    case stdAc::opmode_t::kFan:
      return F("fan_only");
    default:
      return F("unknown");
  }
}

String IRac::fanspeedToString(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kAuto:
      return F("auto");
    case stdAc::fanspeed_t::kMax:
      return F("max");
    case stdAc::fanspeed_t::kHigh:
      return F("high");
    case stdAc::fanspeed_t::kMedium:
      return F("medium");
    case stdAc::fanspeed_t::kLow:
      return F("low");
    case stdAc::fanspeed_t::kMin:
      return F("min");
    default:
      return F("unknown");
  }
}

String IRac::swingvToString(const stdAc::swingv_t swingv) {
  switch (swingv) {
    case stdAc::swingv_t::kOff:
      return F("off");
    case stdAc::swingv_t::kAuto:
      return F("auto");
    case stdAc::swingv_t::kHighest:
      return F("highest");
    case stdAc::swingv_t::kHigh:
      return F("high");
    case stdAc::swingv_t::kMiddle:
      return F("middle");
    case stdAc::swingv_t::kLow:
      return F("low");
    case stdAc::swingv_t::kLowest:
      return F("lowest");
    default:
      return F("unknown");
  }
}

String IRac::swinghToString(const stdAc::swingh_t swingh) {
  switch (swingh) {
    case stdAc::swingh_t::kOff:
      return F("off");
    case stdAc::swingh_t::kAuto:
      return F("auto");
    case stdAc::swingh_t::kLeftMax:
      return F("leftmax");
    case stdAc::swingh_t::kLeft:
      return F("left");
    case stdAc::swingh_t::kMiddle:
      return F("middle");
    case stdAc::swingh_t::kRight:
      return F("right");
    case stdAc::swingh_t::kRightMax:
      return F("rightmax");
    case stdAc::swingh_t::kWide:
      return F("wide");
    default:
      return F("unknown");
  }
}

namespace IRAcUtils {
  // Display the human readable state of an A/C message if we can.
  // Args:
  //   result: A Ptr to the captured `decode_results` that contains an A/C mesg.
  // Returns:
  //   A string with the human description of the A/C message. "" if we can't.
  String resultAcToString(const decode_results * const result) {
    switch (result->decode_type) {
#if DECODE_AMCOR
      case decode_type_t::AMCOR: {
        IRAmcorAc ac(0);
        ac.setRaw(result->state);
        return ac.toString();
      }
#endif  // DECODE_AMCOR
#if DECODE_ARGO
      case decode_type_t::ARGO: {
        IRArgoAC ac(0);
        ac.setRaw(result->state);
        return ac.toString();
      }
#endif  // DECODE_ARGO
#if DECODE_DAIKIN
      case decode_type_t::DAIKIN: {
        IRDaikinESP ac(0);
        ac.setRaw(result->state);
        return ac.toString();
      }
#endif  // DECODE_DAIKIN
#if DECODE_DAIKIN128
      case decode_type_t::DAIKIN128: {
        IRDaikin128 ac(0);
        ac.setRaw(result->state);
        return ac.toString();
      }
#endif  // DECODE_DAIKIN128
#if DECODE_DAIKIN160
      case decode_type_t::DAIKIN160: {
        IRDaikin160 ac(0);
        ac.setRaw(result->state);
        return ac.toString();
      }
#endif  // DECODE_DAIKIN160
#if DECODE_DAIKIN176
      case decode_type_t::DAIKIN176: {
        IRDaikin176 ac(0);
        ac.setRaw(result->state);
        return ac.toString();
      }
#endif  // DECODE_DAIKIN160
#if DECODE_DAIKIN2
      case decode_type_t::DAIKIN2: {
        IRDaikin2 ac(0);
        ac.setRaw(result->state);
        return ac.toString();
      }
#endif  // DECODE_DAIKIN2
#if DECODE_DAIKIN216
      case decode_type_t::DAIKIN216: {
        IRDaikin216 ac(0);
        ac.setRaw(result->state);
        return ac.toString();
      }
#endif  // DECODE_DAIKIN216
#if DECODE_ELECTRA_AC
      case decode_type_t::ELECTRA_AC: {
        IRElectraAc ac(0);
        ac.setRaw(result->state);
        return ac.toString();
      }
#endif  // DECODE_ELECTRA_AC
#if DECODE_FUJITSU_AC
      case decode_type_t::FUJITSU_AC: {
        IRFujitsuAC ac(0);
        ac.setRaw(result->state, result->bits / 8);
        return ac.toString();
      }
#endif  // DECODE_FUJITSU_AC
#if DECODE_KELVINATOR
      case decode_type_t::KELVINATOR: {
        IRKelvinatorAC ac(0);
        ac.setRaw(result->state);
        return ac.toString();
      }
#endif  // DECODE_KELVINATOR
#if DECODE_MITSUBISHI_AC
      case decode_type_t::MITSUBISHI_AC: {
        IRMitsubishiAC ac(0);
        ac.setRaw(result->state);
        return ac.toString();
      }
#endif  // DECODE_MITSUBISHI_AC
#if DECODE_MITSUBISHIHEAVY
      case decode_type_t::MITSUBISHI_HEAVY_88: {
        IRMitsubishiHeavy88Ac ac(0);
        ac.setRaw(result->state);
        return ac.toString();
      }
      case decode_type_t::MITSUBISHI_HEAVY_152: {
        IRMitsubishiHeavy152Ac ac(0);
        ac.setRaw(result->state);
        return ac.toString();
      }
#endif  // DECODE_MITSUBISHIHEAVY
#if DECODE_NEOCLIMA
      case decode_type_t::NEOCLIMA: {
        IRNeoclimaAc ac(0);
        ac.setRaw(result->state);
        return ac.toString();
      }
#endif  // DECODE_NEOCLIMA
#if DECODE_TOSHIBA_AC
      case decode_type_t::TOSHIBA_AC: {
        IRToshibaAC ac(0);
        ac.setRaw(result->state);
        return ac.toString();
      }
#endif  // DECODE_TOSHIBA_AC
#if DECODE_TROTEC
      case decode_type_t::TROTEC: {
        IRTrotecESP ac(0);
        ac.setRaw(result->state);
        return ac.toString();
      }
#endif  // DECODE_TROTEC
#if DECODE_GOODWEATHER
      case decode_type_t::GOODWEATHER: {
        IRGoodweatherAc ac(0);
        ac.setRaw(result->value);  // Goodweather uses value instead of state.
        return ac.toString();
      }
#endif  // DECODE_GOODWEATHER
#if DECODE_GREE
      case decode_type_t::GREE: {
        IRGreeAC ac(0);
        ac.setRaw(result->state);
        return ac.toString();
      }
#endif  // DECODE_GREE
#if DECODE_MIDEA
      case decode_type_t::MIDEA: {
        IRMideaAC ac(0);
        ac.setRaw(result->value);  // Midea uses value instead of state.
        return ac.toString();
      }
#endif  // DECODE_MIDEA
#if DECODE_HAIER_AC
      case decode_type_t::HAIER_AC: {
        IRHaierAC ac(0);
        ac.setRaw(result->state);
        return ac.toString();
      }
#endif  // DECODE_HAIER_AC
#if DECODE_HAIER_AC_YRW02
      case decode_type_t::HAIER_AC_YRW02: {
        IRHaierACYRW02 ac(0);
        ac.setRaw(result->state);
        return ac.toString();
      }
#endif  // DECODE_HAIER_AC_YRW02
#if DECODE_SAMSUNG_AC
      case decode_type_t::SAMSUNG_AC: {
        IRSamsungAc ac(0);
        ac.setRaw(result->state, result->bits / 8);
        return ac.toString();
      }
#endif  // DECODE_SAMSUNG_AC
#if DECODE_SHARP_AC
      case decode_type_t::SHARP_AC: {
        IRSharpAc ac(0);
        ac.setRaw(result->state);
        return ac.toString();
      }
#endif  // DECODE_SHARP_AC
#if DECODE_COOLIX
      case decode_type_t::COOLIX: {
        IRCoolixAC ac(0);
        ac.setRaw(result->value);  // Coolix uses value instead of state.
        return ac.toString();
      }
#endif  // DECODE_COOLIX
#if DECODE_PANASONIC_AC
      case decode_type_t::PANASONIC_AC: {
        if (result->bits > kPanasonicAcShortBits) {
          IRPanasonicAc ac(0);
          ac.setRaw(result->state);
          return ac.toString();
        }
        return "";
      }
#endif  // DECODE_PANASONIC_AC
#if DECODE_HITACHI_AC
      case decode_type_t::HITACHI_AC: {
        IRHitachiAc ac(0);
        ac.setRaw(result->state);
        return ac.toString();
      }
#endif  // DECODE_HITACHI_AC
#if DECODE_WHIRLPOOL_AC
      case decode_type_t::WHIRLPOOL_AC: {
        IRWhirlpoolAc ac(0);
        ac.setRaw(result->state);
        return ac.toString();
      }
#endif  // DECODE_WHIRLPOOL_AC
#if DECODE_VESTEL_AC
      case decode_type_t::VESTEL_AC: {
        IRVestelAc ac(0);
        ac.setRaw(result->value);  // Like Coolix, use value instead of state.
        return ac.toString();
      }
#endif  // DECODE_VESTEL_AC
#if DECODE_TECO
      case decode_type_t::TECO: {
        IRTecoAc ac(0);
        ac.setRaw(result->value);  // Like Coolix, use value instead of state.
        return ac.toString();
      }
#endif  // DECODE_TECO
#if DECODE_TCL112AC
      case decode_type_t::TCL112AC: {
        IRTcl112Ac ac(0);
        ac.setRaw(result->state);
        return ac.toString();
      }
#endif  // DECODE_TCL112AC
      default:
        return "";
    }
  }

  // Convert a valid IR A/C remote message that we understand enough into a
  // Common A/C state.
  //
  // Args:
  //   decode: A PTR to a successful raw IR decode object.
  //   result: A PTR to a state structure to store the result in.
  //   prev:   A PTR to a state structure which has the prev. state. (optional)
  // Returns:
  //   A boolean indicating success or failure.
  bool decodeToState(const decode_results *decode, stdAc::state_t *result,
                     const stdAc::state_t *prev) {
    if (decode == NULL || result == NULL) return false;  // Safety check.
    switch (decode->decode_type) {
#if DECODE_AMCOR
      case decode_type_t::AMCOR: {
        IRAmcorAc ac(kGpioUnused);
        ac.setRaw(decode->state);
        *result = ac.toCommon();
        break;
      }
#endif  // DECODE_AMCOR
#if DECODE_ARGO
      case decode_type_t::ARGO: {
        IRArgoAC ac(kGpioUnused);
        ac.setRaw(decode->state);
        *result = ac.toCommon();
        break;
      }
#endif  // DECODE_ARGO
#if DECODE_COOLIX
      case decode_type_t::COOLIX: {
        IRCoolixAC ac(kGpioUnused);
        ac.setRaw(decode->value);  // Uses value instead of state.
        *result = ac.toCommon(prev);
        break;
      }
#endif  // DECODE_COOLIX
#if DECODE_DAIKIN
      case decode_type_t::DAIKIN: {
        IRDaikinESP ac(kGpioUnused);
        ac.setRaw(decode->state);
        *result = ac.toCommon();
        break;
      }
#endif  // DECODE_DAIKIN
#if DECODE_DAIKIN160
      case decode_type_t::DAIKIN160: {
        IRDaikin160 ac(kGpioUnused);
        ac.setRaw(decode->state);
        *result = ac.toCommon();
        break;
      }
#endif  // DECODE_DAIKIN160
#if DECODE_DAIKIN176
      case decode_type_t::DAIKIN176: {
        IRDaikin176 ac(kGpioUnused);
        ac.setRaw(decode->state);
        *result = ac.toCommon();
        break;
      }
#endif  // DECODE_DAIKIN160
#if DECODE_DAIKIN2
      case decode_type_t::DAIKIN2: {
        IRDaikin2 ac(kGpioUnused);
        ac.setRaw(decode->state);
        *result = ac.toCommon();
        break;
      }
#endif  // DECODE_DAIKIN2
#if DECODE_DAIKIN216
      case decode_type_t::DAIKIN216: {
        IRDaikin216 ac(kGpioUnused);
        ac.setRaw(decode->state);
        *result = ac.toCommon();
        break;
      }
#endif  // DECODE_DAIKIN216
#if DECODE_ELECTRA_AC
      case decode_type_t::ELECTRA_AC: {
        IRElectraAc ac(kGpioUnused);
        ac.setRaw(decode->state);
        *result = ac.toCommon();
        break;
      }
#endif  // DECODE_ELECTRA_AC
#if DECODE_FUJITSU_AC
      case decode_type_t::FUJITSU_AC: {
        IRFujitsuAC ac(kGpioUnused);
        ac.setRaw(decode->state, decode->bits / 8);
        *result = ac.toCommon();
        break;
      }
#endif  // DECODE_FUJITSU_AC
#if DECODE_GOODWEATHER
      case decode_type_t::GOODWEATHER: {
        IRGoodweatherAc ac(kGpioUnused);
        ac.setRaw(decode->value);  // Uses value instead of state.
        *result = ac.toCommon();
        break;
      }
#endif  // DECODE_GOODWEATHER
#if DECODE_GREE
      case decode_type_t::GREE: {
        IRGreeAC ac(kGpioUnused);
        ac.setRaw(decode->state);
        *result = ac.toCommon();
        break;
      }
#endif  // DECODE_GREE
#if DECODE_HAIER_AC
      case decode_type_t::HAIER_AC: {
        IRHaierAC ac(kGpioUnused);
        ac.setRaw(decode->state);
        *result = ac.toCommon();
        break;
      }
#endif  // DECODE_HAIER_AC
#if DECODE_HAIER_AC_YRW02
      case decode_type_t::HAIER_AC_YRW02: {
        IRHaierACYRW02 ac(kGpioUnused);
        ac.setRaw(decode->state);
        *result = ac.toCommon();
        break;
      }
#endif  // DECODE_HAIER_AC_YRW02
#if (DECODE_HITACHI_AC || DECODE_HITACHI_AC2)
      case decode_type_t::HITACHI_AC: {
        IRHitachiAc ac(kGpioUnused);
        ac.setRaw(decode->state);
        *result = ac.toCommon();
        break;
      }
#endif  // (DECODE_HITACHI_AC || DECODE_HITACHI_AC2)
#if DECODE_KELVINATOR
      case decode_type_t::KELVINATOR: {
        IRKelvinatorAC ac(kGpioUnused);
        ac.setRaw(decode->state);
        *result = ac.toCommon();
        break;
      }
#endif  // DECODE_KELVINATOR
#if DECODE_MIDEA
      case decode_type_t::MIDEA: {
        IRMideaAC ac(kGpioUnused);
        ac.setRaw(decode->value);  // Uses value instead of state.
        *result = ac.toCommon(prev);
        break;
      }
#endif  // DECODE_MIDEA
#if DECODE_MITSUBISHI_AC
      case decode_type_t::MITSUBISHI_AC: {
        IRMitsubishiAC ac(kGpioUnused);
        ac.setRaw(decode->state);
        *result = ac.toCommon();
        break;
      }
#endif  // DECODE_MITSUBISHI_AC
#if DECODE_MITSUBISHIHEAVY
      case decode_type_t::MITSUBISHI_HEAVY_88: {
        IRMitsubishiHeavy88Ac ac(kGpioUnused);
        ac.setRaw(decode->state);
        *result = ac.toCommon();
        break;
      }
      case decode_type_t::MITSUBISHI_HEAVY_152: {
        IRMitsubishiHeavy152Ac ac(kGpioUnused);
        ac.setRaw(decode->state);
        *result = ac.toCommon();
        break;
      }
#endif  // DECODE_MITSUBISHIHEAVY
#if DECODE_NEOCLIMA
      case decode_type_t::NEOCLIMA: {
        IRNeoclimaAc ac(kGpioUnused);
        ac.setRaw(decode->state);
        *result = ac.toCommon();
        break;
      }
#endif  // DECODE_NEOCLIMA
#if DECODE_PANASONIC_AC
      case decode_type_t::PANASONIC_AC: {
        IRPanasonicAc ac(kGpioUnused);
        ac.setRaw(decode->state);
        *result = ac.toCommon();
        break;
      }
#endif  // DECODE_PANASONIC_AC
#if DECODE_SAMSUNG_AC
      case decode_type_t::SAMSUNG_AC: {
        IRSamsungAc ac(kGpioUnused);
        ac.setRaw(decode->state);
        *result = ac.toCommon();
        break;
      }
#endif  // DECODE_SAMSUNG_AC
#if DECODE_SHARP_AC
      case decode_type_t::SHARP_AC: {
        IRSharpAc ac(kGpioUnused);
        ac.setRaw(decode->state);
        *result = ac.toCommon();
        break;
      }
#endif  // DECODE_SHARP_AC
#if DECODE_TCL112AC
      case decode_type_t::TCL112AC: {
        IRTcl112Ac ac(kGpioUnused);
        ac.setRaw(decode->state);
        *result = ac.toCommon();
        break;
      }
#endif  // DECODE_TCL112AC
#if DECODE_TECO
      case decode_type_t::TECO: {
        IRTecoAc ac(kGpioUnused);
        ac.setRaw(decode->value);  // Uses value instead of state.
        *result = ac.toCommon();
        break;
      }
#endif  // DECODE_TECO
#if DECODE_TOSHIBA_AC
      case decode_type_t::TOSHIBA_AC: {
        IRToshibaAC ac(kGpioUnused);
        ac.setRaw(decode->state);
        *result = ac.toCommon();
        break;
      }
#endif  // DECODE_TOSHIBA_AC
#if DECODE_TROTEC
      case decode_type_t::TROTEC: {
        IRTrotecESP ac(kGpioUnused);
        ac.setRaw(decode->state);
        *result = ac.toCommon();
        break;
      }
#endif  // DECODE_TROTEC
#if DECODE_VESTEL_AC
      case decode_type_t::VESTEL_AC: {
        IRVestelAc ac(kGpioUnused);
        ac.setRaw(decode->value);  // Uses value instead of state.
        *result = ac.toCommon();
        break;
      }
#endif  // DECODE_VESTEL_AC
#if DECODE_WHIRLPOOL_AC
      case decode_type_t::WHIRLPOOL_AC: {
        IRWhirlpoolAc ac(kGpioUnused);
        ac.setRaw(decode->state);
        *result = ac.toCommon();
        break;
      }
#endif  // DECODE_WHIRLPOOL_AC
      default:
        return false;
    }
    return true;
  }
}  // namespace IRAcUtils
