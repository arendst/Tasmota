#ifndef IRAC_H_
#define IRAC_H_

// Copyright 2019 David Conran

#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#ifndef ARDUINO
#include <string>
#endif
#include "IRremoteESP8266.h"
#include "ir_Argo.h"
#include "ir_Coolix.h"
#include "ir_Daikin.h"
#include "ir_Fujitsu.h"
#include "ir_Gree.h"
#include "ir_Haier.h"
#include "ir_Hitachi.h"
#include "ir_Kelvinator.h"
#include "ir_Midea.h"
#include "ir_Mitsubishi.h"
#include "ir_MitsubishiHeavy.h"
#include "ir_Panasonic.h"
#include "ir_Samsung.h"
#include "ir_Tcl.h"
#include "ir_Teco.h"
#include "ir_Toshiba.h"
#include "ir_Trotec.h"
#include "ir_Vestel.h"
#include "ir_Whirlpool.h"

class IRac {
 public:
  explicit IRac(uint8_t pin);
  static bool isProtocolSupported(const decode_type_t protocol);
  bool sendAc(const decode_type_t vendor, const int16_t model,
              const bool power, const stdAc::opmode_t mode, const float degrees,
              const bool celsius, const stdAc::fanspeed_t fan,
              const stdAc::swingv_t swingv, const stdAc::swingh_t swingh,
              const bool quiet, const bool turbo, const bool econo,
              const bool light, const bool filter, const bool clean,
              const bool beep, const int16_t sleep = -1,
              const int16_t clock = -1);

  static bool strToBool(const char *str, const bool def = false);
  static int16_t strToModel(const char *str, const int16_t def = -1);
  static stdAc::opmode_t strToOpmode(
    const char *str, const stdAc::opmode_t def = stdAc::opmode_t::kAuto);
  static stdAc::fanspeed_t strToFanspeed(
    const char *str,
    const stdAc::fanspeed_t def = stdAc::fanspeed_t::kAuto);
  static stdAc::swingv_t strToSwingV(
    const char *str, const stdAc::swingv_t def = stdAc::swingv_t::kOff);
  static stdAc::swingh_t strToSwingH(
    const char *str, const stdAc::swingh_t def = stdAc::swingh_t::kOff);
#ifndef UNIT_TEST

 private:
#endif
  uint8_t _pin;
#if SEND_ARGO
  void argo(IRArgoAC *ac,
            const bool on, const stdAc::opmode_t mode, const float degrees,
            const stdAc::fanspeed_t fan, const stdAc::swingv_t swingv,
            const bool turbo, const int16_t sleep = -1);
#endif  // SEND_ARGO
#if SEND_COOLIX
  void coolix(IRCoolixAC *ac,
              const bool on, const stdAc::opmode_t mode, const float degrees,
              const stdAc::fanspeed_t fan,
              const stdAc::swingv_t swingv, const stdAc::swingh_t swingh,
              const bool turbo, const bool light, const bool clean,
              const int16_t sleep = -1);
#endif  // SEND_COOLIX
#if SEND_DAIKIN
  void daikin(IRDaikinESP *ac,
              const bool on, const stdAc::opmode_t mode, const float degrees,
              const stdAc::fanspeed_t fan,
              const stdAc::swingv_t swingv, const stdAc::swingh_t swingh,
              const bool quiet, const bool turbo, const bool econo,
              const bool clean);
#endif  // SEND_DAIKIN
#if SEND_DAIKIN2
  void daikin2(IRDaikin2 *ac,
               const bool on, const stdAc::opmode_t mode,
               const float degrees, const stdAc::fanspeed_t fan,
               const stdAc::swingv_t swingv, const stdAc::swingh_t swingh,
               const bool quiet, const bool turbo, const bool light,
               const bool econo, const bool filter, const bool clean,
               const bool beep, const int16_t sleep = -1,
               const int16_t clock = -1);
#endif  // SEND_DAIKIN2
#if SEND_DAIKIN216
void daikin216(IRDaikin216 *ac,
               const bool on, const stdAc::opmode_t mode,
               const float degrees, const stdAc::fanspeed_t fan,
               const stdAc::swingv_t swingv, const stdAc::swingh_t swingh,
               const bool quiet);
#endif  // SEND_DAIKIN216
#if SEND_FUJITSU_AC
  void fujitsu(IRFujitsuAC *ac, const fujitsu_ac_remote_model_t model,
               const bool on, const stdAc::opmode_t mode, const float degrees,
               const stdAc::fanspeed_t fan,
               const stdAc::swingv_t swingv, const stdAc::swingh_t swingh,
               const bool quiet);
#endif  // SEND_FUJITSU_AC
#if SEND_GREE
  void gree(IRGreeAC *ac,
            const bool on, const stdAc::opmode_t mode, const float degrees,
            const stdAc::fanspeed_t fan, const stdAc::swingv_t swingv,
            const bool turbo, const bool light, const bool clean,
            const int16_t sleep = -1);
#endif  // SEND_GREE
#if SEND_HAIER_AC
  void haier(IRHaierAC *ac,
             const bool on, const stdAc::opmode_t mode, const float degrees,
             const stdAc::fanspeed_t fan, const stdAc::swingv_t swingv,
             const bool filter, const int16_t sleep = -1,
             const int16_t clock = -1);
#endif  // SEND_HAIER_AC
#if SEND_HAIER_AC_YRW02
  void haierYrwo2(IRHaierACYRW02 *ac,
                  const bool on, const stdAc::opmode_t mode,
                  const float degrees, const stdAc::fanspeed_t fan,
                  const stdAc::swingv_t swingv,
                  const bool turbo, const bool filter,
                  const int16_t sleep = -1);
#endif  // SEND_HAIER_AC_YRW02
#if SEND_HITACHI_AC
  void hitachi(IRHitachiAc *ac,
               const bool on, const stdAc::opmode_t mode,
               const float degrees, const stdAc::fanspeed_t fan,
               const stdAc::swingv_t swingv, const stdAc::swingh_t swingh);
#endif  // SEND_HITACHI_AC
#if SEND_KELVINATOR
  void kelvinator(IRKelvinatorAC *ac,
                  const bool on, const stdAc::opmode_t mode,
                  const float degrees, const stdAc::fanspeed_t fan,
                  const stdAc::swingv_t swingv, const stdAc::swingh_t swingh,
                  const bool quiet, const bool turbo, const bool light,
                  const bool filter, const bool clean);
#endif  // SEND_KELVINATOR
#if SEND_MIDEA
  void midea(IRMideaAC *ac,
             const bool on, const stdAc::opmode_t mode, const float degrees,
             const stdAc::fanspeed_t fan, const int16_t sleep = -1);
#endif  // SEND_MIDEA
#if SEND_MITSUBISHI_AC
  void mitsubishi(IRMitsubishiAC *ac,
                  const bool on, const stdAc::opmode_t mode,
                  const float degrees,
                  const stdAc::fanspeed_t fan, const stdAc::swingv_t swingv,
                  const bool quiet, const int16_t clock = -1);
#endif  // SEND_MITSUBISHI_AC
#if SEND_MITSUBISHIHEAVY
  void mitsubishiHeavy88(IRMitsubishiHeavy88Ac *ac,
                         const bool on, const stdAc::opmode_t mode,
                         const float degrees, const stdAc::fanspeed_t fan,
                         const stdAc::swingv_t swingv,
                         const stdAc::swingh_t swingh,
                         const bool turbo, const bool econo, const bool clean);
  void mitsubishiHeavy152(IRMitsubishiHeavy152Ac *ac,
                          const bool on, const stdAc::opmode_t mode,
                          const float degrees, const stdAc::fanspeed_t fan,
                          const stdAc::swingv_t swingv,
                          const stdAc::swingh_t swingh,
                          const bool quiet, const bool turbo, const bool econo,
                          const bool filter, const bool clean,
                          const int16_t sleep = -1);
#endif  // SEND_MITSUBISHIHEAVY
#if SEND_PANASONIC_AC
  void panasonic(IRPanasonicAc *ac, const panasonic_ac_remote_model_t model,
                 const bool on, const stdAc::opmode_t mode, const float degrees,
                 const stdAc::fanspeed_t fan,
                 const stdAc::swingv_t swingv, const stdAc::swingh_t swingh,
                 const bool quiet, const bool turbo, const int16_t clock = -1);
#endif  // SEND_PANASONIC_AC
#if SEND_SAMSUNG_AC
  void samsung(IRSamsungAc *ac,
               const bool on, const stdAc::opmode_t mode, const float degrees,
               const stdAc::fanspeed_t fan, const stdAc::swingv_t swingv,
               const bool quiet, const bool turbo, const bool clean,
               const bool beep, const bool sendOnOffHack = true);
#endif  // SEND_SAMSUNG_AC
#if SEND_TCL112AC
  void tcl112(IRTcl112Ac *ac,
              const bool on, const stdAc::opmode_t mode, const float degrees,
              const stdAc::fanspeed_t fan,
              const stdAc::swingv_t swingv, const stdAc::swingh_t swingh,
              const bool turbo, const bool light, const bool econo,
              const bool filter);
#endif  // SEND_TCL112AC
#if SEND_TECO
  void teco(IRTecoAc *ac,
            const bool on, const stdAc::opmode_t mode, const float degrees,
            const stdAc::fanspeed_t fan, const stdAc::swingv_t swingv,
            const int16_t sleep = -1);
#endif  // SEND_TECO
#if SEND_TOSHIBA_AC
  void toshiba(IRToshibaAC *ac,
               const bool on, const stdAc::opmode_t mode, const float degrees,
               const stdAc::fanspeed_t fan);
#endif  // SEND_TOSHIBA_AC
#if SEND_TROTEC
  void trotec(IRTrotecESP *ac,
              const bool on, const stdAc::opmode_t mode, const float degrees,
              const stdAc::fanspeed_t fan, const int16_t sleep = -1);
#endif  // SEND_TROTEC
#if SEND_VESTEL_AC
  void vestel(IRVestelAc *ac,
              const bool on, const stdAc::opmode_t mode, const float degrees,
              const stdAc::fanspeed_t fan, const stdAc::swingv_t swingv,
              const bool turbo, const bool filter,
              const int16_t sleep = -1, const int16_t clock = -1,
              const bool sendNormal = true);
#endif  // SEND_VESTEL_AC
#if SEND_WHIRLPOOL_AC
  void whirlpool(IRWhirlpoolAc *ac, const whirlpool_ac_remote_model_t model,
                 const bool on, const stdAc::opmode_t mode, const float degrees,
                 const stdAc::fanspeed_t fan, const stdAc::swingv_t swingv,
                 const bool turbo, const bool light,
                 const int16_t sleep = -1, const int16_t clock = -1);
#endif  // SEND_WHIRLPOOL_AC
};  // IRac class

// Structure to hold a common A/C state.
typedef struct {
  decode_type_t protocol;
  int16_t model;
  bool power;
  stdAc::opmode_t mode;
  float degrees;
  bool celsius;
  stdAc::fanspeed_t fanspeed;
  stdAc::swingv_t swingv;
  stdAc::swingh_t swingh;
  bool quiet;
  bool turbo;
  bool econo;
  bool light;
  bool filter;
  bool clean;
  bool beep;
  int16_t sleep;
  int16_t clock;
} commonAcState_t;
#endif  // IRAC_H_
