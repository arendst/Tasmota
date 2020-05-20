#ifndef IRAC_H_
#define IRAC_H_

// Copyright 2019 David Conran

#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include "IRremoteESP8266.h"
#include "ir_Amcor.h"
#include "ir_Argo.h"
#include "ir_Coolix.h"
#include "ir_Daikin.h"
#include "ir_Delonghi.h"
#include "ir_Fujitsu.h"
#include "ir_Electra.h"
#include "ir_Goodweather.h"
#include "ir_Gree.h"
#include "ir_Haier.h"
#include "ir_Hitachi.h"
#include "ir_Kelvinator.h"
#include "ir_LG.h"
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

// Constants
const int8_t kGpioUnused = -1;

// Class
class IRac {
 public:
  explicit IRac(const uint16_t pin, const bool inverted = false,
                const bool use_modulation = true);
  static bool isProtocolSupported(const decode_type_t protocol);
  static void initState(stdAc::state_t *state,
                        const decode_type_t vendor, const int16_t model,
                        const bool power, const stdAc::opmode_t mode,
                        const float degrees, const bool celsius,
                        const stdAc::fanspeed_t fan,
                        const stdAc::swingv_t swingv,
                        const stdAc::swingh_t swingh,
                        const bool quiet, const bool turbo, const bool econo,
                        const bool light, const bool filter, const bool clean,
                        const bool beep, const int16_t sleep,
                        const int16_t clock);
  static void initState(stdAc::state_t *state);
  void markAsSent(void);
  bool sendAc(void);
  bool sendAc(const stdAc::state_t desired, const stdAc::state_t *prev = NULL);
  bool sendAc(const decode_type_t vendor, const int16_t model,
              const bool power, const stdAc::opmode_t mode, const float degrees,
              const bool celsius, const stdAc::fanspeed_t fan,
              const stdAc::swingv_t swingv, const stdAc::swingh_t swingh,
              const bool quiet, const bool turbo, const bool econo,
              const bool light, const bool filter, const bool clean,
              const bool beep, const int16_t sleep = -1,
              const int16_t clock = -1);
  static bool cmpStates(const stdAc::state_t a, const stdAc::state_t b);
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
  static String boolToString(const bool value);
  static String opmodeToString(const stdAc::opmode_t mode);
  static String fanspeedToString(const stdAc::fanspeed_t speed);
  static String swingvToString(const stdAc::swingv_t swingv);
  static String swinghToString(const stdAc::swingh_t swingh);
  stdAc::state_t getState(void);
  stdAc::state_t getStatePrev(void);
  bool hasStateChanged(void);
  stdAc::state_t next;  // The state we want the device to be in after we send.
#ifndef UNIT_TEST

 private:
#endif
  uint16_t _pin;
  bool _inverted;
  bool _modulation;
  stdAc::state_t _prev;  // The state we expect the device to currently be in.
#if SEND_AMCOR
  void amcor(IRAmcorAc *ac,
             const bool on, const stdAc::opmode_t mode, const float degrees,
             const stdAc::fanspeed_t fan);
#endif  // SEND_AMCOR
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
#if SEND_DAIKIN128
  void daikin128(IRDaikin128 *ac,
                 const bool on, const stdAc::opmode_t mode,
                 const float degrees, const stdAc::fanspeed_t fan,
                 const stdAc::swingv_t swingv,
                 const bool quiet, const bool turbo, const bool light,
                 const bool econo, const int16_t sleep = -1,
                 const int16_t clock = -1);
#endif  // SEND_DAIKIN128
#if SEND_DAIKIN152
  void daikin152(IRDaikin152 *ac,
                 const bool on, const stdAc::opmode_t mode,
                 const float degrees, const stdAc::fanspeed_t fan,
                 const stdAc::swingv_t swingv,
                 const bool quiet, const bool turbo, const bool econo);
#endif  // SEND_DAIKIN152
#if SEND_DAIKIN160
  void daikin160(IRDaikin160 *ac,
                 const bool on, const stdAc::opmode_t mode,
                 const float degrees, const stdAc::fanspeed_t fan,
                 const stdAc::swingv_t swingv);
#endif  // SEND_DAIKIN160
#if SEND_DAIKIN176
  void daikin176(IRDaikin176 *ac,
                 const bool on, const stdAc::opmode_t mode,
                 const float degrees, const stdAc::fanspeed_t fan,
                 const stdAc::swingh_t swingh);
#endif  // SEND_DAIKIN176
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
               const bool quiet, const bool turbo);
#endif  // SEND_DAIKIN216
#if SEND_DAIKIN64
  void daikin64(IRDaikin64 *ac,
                 const bool on, const stdAc::opmode_t mode,
                 const float degrees, const stdAc::fanspeed_t fan,
                 const stdAc::swingv_t swingv,
                 const bool quiet, const bool turbo,
                 const int16_t sleep = -1, const int16_t clock = -1);
#endif  // SEND_DAIKIN64
#if SEND_DELONGHI_AC
  void delonghiac(IRDelonghiAc *ac,
                  const bool on, const stdAc::opmode_t mode, const bool celsius,
                  const float degrees, const stdAc::fanspeed_t fan,
                  const bool turbo, const int16_t sleep = -1);
#endif  // SEND_DELONGHI_AC
#if SEND_ELECTRA_AC
void electra(IRElectraAc *ac,
             const bool on, const stdAc::opmode_t mode,
             const float degrees, const stdAc::fanspeed_t fan,
             const stdAc::swingv_t swingv,
             const stdAc::swingh_t swingh, const bool turbo,
             const bool lighttoggle, const bool clean);
#endif  // SEND_ELECTRA_AC
#if SEND_FUJITSU_AC
  void fujitsu(IRFujitsuAC *ac, const fujitsu_ac_remote_model_t model,
               const bool on, const stdAc::opmode_t mode, const float degrees,
               const stdAc::fanspeed_t fan,
               const stdAc::swingv_t swingv, const stdAc::swingh_t swingh,
               const bool quiet, const bool turbo, const bool econo,
               const bool filter, const bool clean);
#endif  // SEND_FUJITSU_AC
#if SEND_GOODWEATHER
  void goodweather(IRGoodweatherAc *ac,
                   const bool on, const stdAc::opmode_t mode,
                   const float degrees,
                   const stdAc::fanspeed_t fan,
                   const stdAc::swingv_t swingv,
                   const bool turbo, const bool light,
                   const int16_t sleep = -1);
#endif  // SEND_GOODWEATHER
#if SEND_GREE
  void gree(IRGreeAC *ac, const gree_ac_remote_model_t model,
            const bool on, const stdAc::opmode_t mode, const bool celsius,
            const float degrees, const stdAc::fanspeed_t fan,
            const stdAc::swingv_t swingv, const bool turbo, const bool light,
            const bool clean, const int16_t sleep = -1);
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
#if SEND_HITACHI_AC1
  void hitachi1(IRHitachiAc1 *ac, const hitachi_ac1_remote_model_t model,
                const bool on, const bool power_toggle,
                const stdAc::opmode_t mode,
                const float degrees, const stdAc::fanspeed_t fan,
                const stdAc::swingv_t swingv, const stdAc::swingh_t swingh,
                const bool swing_toggle, const int16_t sleep = -1);
#endif  // SEND_HITACHI_AC1
#if SEND_HITACHI_AC424
  void hitachi424(IRHitachiAc424 *ac,
                  const bool on, const stdAc::opmode_t mode,
                  const float degrees, const stdAc::fanspeed_t fan,
                  const stdAc::swingv_t swingv);
#endif  // SEND_HITACHI_AC424
#if SEND_KELVINATOR
  void kelvinator(IRKelvinatorAC *ac,
                  const bool on, const stdAc::opmode_t mode,
                  const float degrees, const stdAc::fanspeed_t fan,
                  const stdAc::swingv_t swingv, const stdAc::swingh_t swingh,
                  const bool quiet, const bool turbo, const bool light,
                  const bool filter, const bool clean);
#endif  // SEND_KELVINATOR
#if SEND_LG
  void lg(IRLgAc *ac, const lg_ac_remote_model_t model,
          const bool on, const stdAc::opmode_t mode,
          const float degrees, const stdAc::fanspeed_t fan);
#endif  // SEND_LG
#if SEND_MIDEA
  void midea(IRMideaAC *ac,
             const bool on, const stdAc::opmode_t mode, const bool celsius,
             const float degrees, const stdAc::fanspeed_t fan,
             const stdAc::swingv_t swingv, const int16_t sleep = -1);
#endif  // SEND_MIDEA
#if SEND_MITSUBISHI_AC
  void mitsubishi(IRMitsubishiAC *ac,
                  const bool on, const stdAc::opmode_t mode,
                  const float degrees,
                  const stdAc::fanspeed_t fan, const stdAc::swingv_t swingv,
                  const stdAc::swingh_t swingh,
                  const bool quiet, const int16_t clock = -1);
#endif  // SEND_MITSUBISHI_AC
#if SEND_MITSUBISHI112
  void mitsubishi112(IRMitsubishi112 *ac,
                     const bool on, const stdAc::opmode_t mode,
                     const float degrees, const stdAc::fanspeed_t fan,
                     const stdAc::swingv_t swingv,
                     const stdAc::swingh_t swingh,
                     const bool quiet);
#endif  // SEND_MITSUBISHI112
#if SEND_MITSUBISHI136
  void mitsubishi136(IRMitsubishi136 *ac,
                     const bool on, const stdAc::opmode_t mode,
                     const float degrees, const stdAc::fanspeed_t fan,
                     const stdAc::swingv_t swingv, const bool quiet);
#endif  // SEND_MITSUBISHI136
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
#if SEND_NEOCLIMA
  void neoclima(IRNeoclimaAc *ac, const bool on, const stdAc::opmode_t mode,
                const float degrees, const stdAc::fanspeed_t fan,
                const stdAc::swingv_t swingv, const stdAc::swingh_t swingh,
                const bool turbo, const bool light, const bool filter,
                const int16_t sleep = -1);
#endif  // SEND_NEOCLIMA
#if SEND_PANASONIC_AC
  void panasonic(IRPanasonicAc *ac, const panasonic_ac_remote_model_t model,
                 const bool on, const stdAc::opmode_t mode, const float degrees,
                 const stdAc::fanspeed_t fan,
                 const stdAc::swingv_t swingv, const stdAc::swingh_t swingh,
                 const bool quiet, const bool turbo, const bool filter,
                 const int16_t clock = -1);
#endif  // SEND_PANASONIC_AC
#if SEND_SAMSUNG_AC
  void samsung(IRSamsungAc *ac,
               const bool on, const stdAc::opmode_t mode, const float degrees,
               const stdAc::fanspeed_t fan, const stdAc::swingv_t swingv,
               const bool quiet, const bool turbo, const bool light,
               const bool filter, const bool clean,
               const bool beep, const bool prevpower = true,
               const bool forcepower = true);
#endif  // SEND_SAMSUNG_AC
#if SEND_SHARP_AC
  void sharp(IRSharpAc *ac,
             const bool on, const bool prev_power, const stdAc::opmode_t mode,
             const float degrees, const stdAc::fanspeed_t fan,
             const stdAc::swingv_t swingv, const bool turbo, const bool filter,
             const bool clean);
#endif  // SEND_SHARP_AC
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
            const bool light, const int16_t sleep = -1);
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
static stdAc::state_t cleanState(const stdAc::state_t state);
static stdAc::state_t handleToggles(const stdAc::state_t desired,
                                    const stdAc::state_t *prev = NULL);
};  // IRac class

namespace IRAcUtils {
  String resultAcToString(const decode_results * const results);
  bool decodeToState(const decode_results *decode, stdAc::state_t *result,
                     const stdAc::state_t *prev = NULL);
}  // namespace IRAcUtils
#endif  // IRAC_H_
