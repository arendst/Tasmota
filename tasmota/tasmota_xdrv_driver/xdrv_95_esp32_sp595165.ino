/*
  xdrv_95_esp32_sp595165.ino - IONE 74HC595 + 74HC165 8CH 보드 (SP595165)

  릴레이(595) + 입력(165) — 스키매틱 핀 (SP595165_빌드가이드.md 일치):
    GPIO13  74HC_EN   (165 CE#, 595 OE#=GND)
    GPIO25  74HC_RCK  (595 RCK + 165 PL#)
    GPIO26  74HC_SCK  (595 SCK + 165 CP)
    GPIO33  74HC_IN   (595 SIN)
    GPIO27  74HC_OUT  (165 Q7 → Dingtian Q7)

  Dingtian 통합 모드 (대체 배선): GPIO13 OE, 25 CLK, 32 SDI, 33 PL, 26 Q7

  SetOption133 - 595 출력 반전 (기존 보드는 보통 0)
  SetOption81  - 165 입력 반전 (Active-Low 입력)
*/

#ifdef ESP32
#ifdef USE_SP595165

#define XDRV_95           95

#define SP595165_SET_OUTPUT(pin, value)  { pinMode((pin), OUTPUT); digitalWrite((pin), (value)); }
#define SP595165_SET_INPUT(pin)          { pinMode((pin), INPUT); }

#ifndef SP595165_IONE_PINS
#error "SP595165: IONE 빌드는 SP595165_IONE_PINS 필요 (user_config_override.h / platformio_override.ini)"
#endif

// IONE 보드 물리 GPIO (Configure Module Pin() 우회)
#define SP595165_PHY_OE   13
#define SP595165_PHY_RCK  25
#define SP595165_PHY_SCK  26
#define SP595165_PHY_SER  33
#define SP595165_PHY_Q7   27

#define SP595165_OUT(pin, val)  gpio_set_level((gpio_num_t)(pin), (val))
#define SP595165_IN(pin)        gpio_get_level((gpio_num_t)(pin))

static void Sp595165IoneGpioOutInit(uint8_t pin, int level) {
  gpio_reset_pin((gpio_num_t)pin);
  gpio_set_direction((gpio_num_t)pin, GPIO_MODE_OUTPUT);
  gpio_set_level((gpio_num_t)pin, level);
}

static void Sp595165IoneGpioInInit(uint8_t pin) {
  gpio_reset_pin((gpio_num_t)pin);
  gpio_set_direction((gpio_num_t)pin, GPIO_MODE_INPUT);
  gpio_set_pull_mode((gpio_num_t)pin, GPIO_FLOATING);
}

// DingtianReadWrite 비트 순서(D7..D0) -> IN1(PIN1)..IN8(PIN8)
static const uint8_t kSp595165RawBitForIn[8] = { 3, 4, 2, 5, 1, 6, 0, 7 };

enum Sp595165Mode {
  SP595165_MODE_NONE = 0,
  SP595165_MODE_SHIFT595,   // IONE 기존: SRCLK/RCLK/SER 분리
  SP595165_MODE_DINGTIAN    // 595+165 공유 버스
};

struct SP595165_DATA {
  uint32_t outputs;
  uint32_t last_inputs;
  uint32_t last_inputs_raw;   // SO81/리맵 전 (진단용)
  uint8_t  count;
  uint8_t  first;
  uint8_t  mode;
  uint8_t  pin_clk;         // Dingtian CLK 또는 미사용
  uint8_t  pin_srclk;       // Shift595 SRCLK
  uint8_t  pin_rclk;        // Shift595 RCLK / 165 PL#
  uint8_t  pin_sdi;         // Dingtian SDI / Shift595 SER
  uint8_t  pin_q7;
  uint8_t  pin_pl;
  uint8_t  pin_oe;
  bool     has_q7;
  bool     oe_configured;   // GPIO_SHIFT595_OE / DINGTIAN_OE 템플릿 배선 여부
  bool     oe_warn_logged;  // CE# 미배선 경고 1회만
  uint32_t last_diag_ms;    // 10초 진단 주기
  uint32_t last_diag_raw;   // 이전 진단 RAW (변화 감지)
  bool     hw_check_logged; // 165/Q7 하드웨어 점검 ERROR 1회
} *Sp595165 = nullptr;

/*********************************************************************************************\
 * 공통
\*********************************************************************************************/

uint32_t Sp595165RemapInputs(uint32_t raw) {
  uint32_t logical = 0;
  for (uint32_t i = 0; i < 8; i++) {
    if (bitRead(raw, kSp595165RawBitForIn[i])) {
      bitSet(logical, i);
    }
  }
  return logical;
}

void Sp595165PulsePin(uint8_t pin) {
  SP595165_OUT(pin, 1);
  SP595165_OUT(pin, 0);
}

uint32_t Sp595165ApplyInputInvert(uint32_t inputs) {
  if (Settings->flag3.pcf8574_ports_inverted) {  // SetOption81
    inputs = ~inputs;
  }
  return Sp595165RemapInputs(inputs);
}

bool Sp595165RelayBit(uint32_t relay_index) {
  return bitRead(Sp595165->outputs, relay_index);
}

void Sp595165Enable165(bool enable) {
  // IONE 보드: GPIO13 = 165 CE# (595 OE#=GND). CE# LOW = 165 Q7 출력 활성
  // CE# 미배선(플로팅+풀업)이면 Q7 high-Z → ESP32가 0으로 읽음
  if (!Sp595165->oe_configured) {
    return;
  }
  SP595165_OUT(Sp595165->pin_oe, enable ? 0 : 1);
}

void Sp595165WarnCeNotConfigured(void) {
  if (!Sp595165->has_q7 || Sp595165->oe_configured || Sp595165->oe_warn_logged) {
    return;
  }
  Sp595165->oe_warn_logged = true;
  AddLog(LOG_LEVEL_ERROR, PSTR("SP595165: Q7 있으나 OE(165 CE#) 미배선 — RAW=00 고정. Gpio13 8352 설정 필요"));
}

void Sp595165WriteRelayBit(bool relay_state) {
  bool level = Settings->flag5.shift595_invert_outputs ? !relay_state : relay_state;  // SetOption133
  SP595165_OUT(Sp595165->pin_sdi, level);
}

/*********************************************************************************************\
 * IONE 공유 버스 (GPIO25 = 595 RCLK + 165 PL#)
 *  IO13 CE#:      LOW 유지 (165 Q7 출력 활성)
 *  IO25 PL#/RCLK: LOW 5us=165 병렬로드 → HIGH 5us=165 직렬모드+595 래치 → 시프트 중 HIGH 유지
 *  IO26 SCK/CP:   SER 설정 → Q7 샘플(클럭 전) → SCK 펄스
\*********************************************************************************************/

uint32_t Sp595165ReadWriteShift595(power_t rpower) {
  uint32_t relay_max = Sp595165->count;
  DevicesPresentNonDisplayOrLight(relay_max);
  uint32_t relay_offset = Sp595165->count - relay_max + Sp595165->first;
  uint32_t power_bit = relay_max - 1;

  Sp595165WarnCeNotConfigured();

  // 1) CE# LOW — 165 Q7 출력 활성
  Sp595165Enable165(true);

  // 2) RCK LOW 5us — 165 병렬 로드
  SP595165_OUT(Sp595165->pin_rclk, 0);
  delayMicroseconds(5);

  // 3) RCK HIGH 5us — 165 직렬 활성 + 595 1회 래치
  SP595165_OUT(Sp595165->pin_rclk, 1);
  delayMicroseconds(5);

  uint32_t inputs = 0;
  uint32_t in_bit = 1;

  // 4) 8회: SER → Q7(클럭 전) → SCK 펄스 (RCK=HIGH 유지)
  for (uint32_t i = 0; i < Sp595165->count; i++) {
    uint32_t relay_state = 0;
    if (i >= relay_offset) {
      relay_state = bitRead(rpower, power_bit);
      power_bit--;
    }
    Sp595165WriteRelayBit(relay_state);

    if (Sp595165->has_q7) {
      inputs |= SP595165_IN(Sp595165->pin_q7) ? in_bit : 0;
      in_bit <<= 1;
    }
    SP595165_OUT(Sp595165->pin_srclk, 1);
    SP595165_OUT(Sp595165->pin_srclk, 0);
  }
  // RCK는 HIGH 유지

  if (Sp595165->has_q7) {
    Sp595165->last_inputs_raw = inputs;
    return Sp595165ApplyInputInvert(inputs);
  }
  return 0;
}

/*********************************************************************************************\
 * 릴레이만 (Q7 미배선)
\*********************************************************************************************/

void Sp595165Shift595FromPower(power_t rpower) {
  uint32_t relay_max = Sp595165->count;
  DevicesPresentNonDisplayOrLight(relay_max);
  uint32_t relay_offset = Sp595165->count - relay_max + Sp595165->first;
  uint32_t power_bit = relay_max - 1;

  if (Sp595165->oe_configured) {
    Sp595165Enable165(true);
  }

  SP595165_OUT(Sp595165->pin_rclk, 0);

  for (uint32_t i = 0; i < Sp595165->count; i++) {
    uint32_t relay_state = 0;
    if (i >= relay_offset) {
      relay_state = bitRead(rpower, power_bit);
      power_bit--;
    }
    Sp595165WriteRelayBit(relay_state);
    Sp595165PulsePin(Sp595165->pin_srclk);
  }

  Sp595165PulsePin(Sp595165->pin_rclk);
}

/*********************************************************************************************\
 * Dingtian 공유 버스 (대체 배선)
\*********************************************************************************************/

uint32_t Sp595165ReadWriteDingtian(uint32_t outputs) {
  if (Settings->flag5.shift595_invert_outputs) {
    outputs = ~outputs;
  }

  if (Sp595165->oe_configured) {
    Sp595165Enable165(true);
  }

  uint32_t inputs = 0;
  uint32_t in_bit = 1;

  SP595165_OUT(Sp595165->pin_rclk, 0);
  if (PinUsed(GPIO_DINGTIAN_PL)) {
    SP595165_OUT(Sp595165->pin_pl, 1);  // upstream Dingtian: 165 병렬 로드
  }

  for (int i = Sp595165->count; i > 0; i--) {
    SP595165_OUT(Sp595165->pin_sdi, outputs & 1);
    outputs >>= 1;
    if (Sp595165->has_q7) {
      inputs |= SP595165_IN(Sp595165->pin_q7) ? in_bit : 0;
      in_bit <<= 1;
    }
    SP595165_OUT(Sp595165->pin_clk, 1);
    SP595165_OUT(Sp595165->pin_clk, 0);
  }

  SP595165_OUT(Sp595165->pin_rclk, 1);
  if (PinUsed(GPIO_DINGTIAN_PL)) {
    SP595165_OUT(Sp595165->pin_pl, 0);
  }

  return Sp595165ApplyInputInvert(inputs);
}

uint32_t Sp595165ReadWrite(uint32_t outputs) {
  (void)outputs;
  return 0;
}

/*********************************************************************************************\
 * 초기화
\*********************************************************************************************/

void Sp595165Init(void) {
#ifdef SP595165_IONE_PINS
  bool shift595 = true;
  bool dingtian = false;
#else
  bool shift595 = PinUsed(GPIO_SHIFT595_SRCLK) && PinUsed(GPIO_SHIFT595_RCLK) && PinUsed(GPIO_SHIFT595_SER);
  bool dingtian = PinUsed(GPIO_DINGTIAN_CLK, GPIO_ANY) && PinUsed(GPIO_DINGTIAN_SDI)
               && PinUsed(GPIO_DINGTIAN_PL) && PinUsed(GPIO_DINGTIAN_OE);
#endif

  if (!shift595 && !dingtian) {
    return;
  }

  Sp595165 = (struct SP595165_DATA*)calloc(1, sizeof(struct SP595165_DATA));
  if (!Sp595165) {
    return;
  }

#ifdef SP595165_IONE_PINS
  // IONE 물리 GPIO 직접 사용 (Pin()/Configure Module 우회)
  Sp595165->mode = SP595165_MODE_SHIFT595;
  Sp595165->pin_oe = SP595165_PHY_OE;
  Sp595165->pin_rclk = SP595165_PHY_RCK;
  Sp595165->pin_srclk = SP595165_PHY_SCK;
  Sp595165->pin_sdi = SP595165_PHY_SER;
  Sp595165->pin_q7 = SP595165_PHY_Q7;
  Sp595165->has_q7 = true;
  Sp595165->oe_configured = true;
  Sp595165->count = 8;
  Sp595165->last_diag_raw = 0xFFFFFFFF;  // 첫 진단 비교 스킵 방지
#else
  if (shift595) {
    Sp595165->mode = SP595165_MODE_SHIFT595;
    Sp595165->pin_srclk = Pin(GPIO_SHIFT595_SRCLK);
    Sp595165->pin_rclk = Pin(GPIO_SHIFT595_RCLK);
    Sp595165->pin_sdi = Pin(GPIO_SHIFT595_SER);
    Sp595165->count = Settings->shift595_device_count * 8;
    if (0 == Sp595165->count) {
      Sp595165->count = 8;
    }
  } else {
    Sp595165->mode = SP595165_MODE_DINGTIAN;
    Sp595165->pin_clk = Pin(GPIO_DINGTIAN_CLK, GPIO_ANY);
    Sp595165->pin_sdi = Pin(GPIO_DINGTIAN_SDI);
    Sp595165->pin_pl = Pin(GPIO_DINGTIAN_PL);
    Sp595165->pin_rclk = PinUsed(GPIO_DINGTIAN_RCK) ? Pin(GPIO_DINGTIAN_RCK) : Pin(GPIO_DINGTIAN_PL);
    Sp595165->count = ((GetPin(Sp595165->pin_clk) - AGPIO(GPIO_DINGTIAN_CLK)) + 1) * 8;
  }
#endif  // SP595165_IONE_PINS

#ifndef SP595165_IONE_PINS
  if (PinUsed(GPIO_SHIFT595_OE)) {
    Sp595165->pin_oe = Pin(GPIO_SHIFT595_OE);
    Sp595165->oe_configured = true;
  } else if (PinUsed(GPIO_DINGTIAN_OE)) {
    Sp595165->pin_oe = Pin(GPIO_DINGTIAN_OE);
    Sp595165->oe_configured = true;
  }

  if (PinUsed(GPIO_DINGTIAN_Q7)) {
    Sp595165->pin_q7 = Pin(GPIO_DINGTIAN_Q7);
    Sp595165->has_q7 = true;
#ifdef ESP32
    gpio_reset_pin((gpio_num_t)Sp595165->pin_q7);
    gpio_set_direction((gpio_num_t)Sp595165->pin_q7, GPIO_MODE_INPUT);
    gpio_set_pull_mode((gpio_num_t)Sp595165->pin_q7, GPIO_FLOATING);  // 풀다운/풀업 비활성
#else
    SP595165_SET_INPUT(Sp595165->pin_q7);
#endif
  }
#endif  // !SP595165_IONE_PINS

#ifdef SP595165_IONE_PINS
  AddLog(LOG_LEVEL_INFO, PSTR("SP595165: IONE phy oe:%d q7:%d rck:%d sck:%d ser:%d ch:%d SO133:%d SO81:%d"),
    Sp595165->pin_oe, Sp595165->pin_q7, Sp595165->pin_rclk, Sp595165->pin_srclk, Sp595165->pin_sdi,
    Sp595165->count, Settings->flag5.shift595_invert_outputs, Settings->flag3.pcf8574_ports_inverted);
#else
  AddLog(LOG_LEVEL_INFO, PSTR("SP595165: mode:%d clk:%d srclk:%d rclk:%d sdi:%d ch:%d SO133:%d SO81:%d"),
    Sp595165->mode, Sp595165->pin_clk, Sp595165->pin_srclk, Sp595165->pin_rclk, Sp595165->pin_sdi,
    Sp595165->count, Settings->flag5.shift595_invert_outputs, Settings->flag3.pcf8574_ports_inverted);

  AddLog(LOG_LEVEL_INFO, PSTR("SP595165: q7:%d(has:%d) oe:%d(cfg:%d)"),
    Sp595165->pin_q7, Sp595165->has_q7, Sp595165->pin_oe, Sp595165->oe_configured);

  if (Sp595165->has_q7 && !Sp595165->oe_configured) {
    AddLog(LOG_LEVEL_ERROR, PSTR("SP595165: Q7 배선됨, OE(165 CE#) 미배선 — Configure Module: Gpio13 8352"));
  }
#endif

  if (SP595165_MODE_SHIFT595 == Sp595165->mode) {
#ifdef SP595165_IONE_PINS
    Sp595165IoneGpioOutInit(Sp595165->pin_srclk, 0);
    Sp595165IoneGpioOutInit(Sp595165->pin_rclk, 0);
    Sp595165IoneGpioOutInit(Sp595165->pin_sdi, 0);
    Sp595165IoneGpioOutInit(Sp595165->pin_oe, 0);
    Sp595165IoneGpioInInit(Sp595165->pin_q7);
    gpio_hold_dis((gpio_num_t)Sp595165->pin_srclk);
    gpio_hold_dis((gpio_num_t)Sp595165->pin_rclk);
#else
    SP595165_SET_OUTPUT(Sp595165->pin_srclk, 0);
    SP595165_SET_OUTPUT(Sp595165->pin_rclk, 0);
    SP595165_SET_OUTPUT(Sp595165->pin_sdi, 0);
#ifdef ESP32
    gpio_hold_dis((gpio_num_t)Sp595165->pin_srclk);
    gpio_hold_dis((gpio_num_t)Sp595165->pin_rclk);
#endif
#endif
  } else {
    SP595165_SET_OUTPUT(Sp595165->pin_clk, 0);
    SP595165_SET_OUTPUT(Sp595165->pin_sdi, 0);
    SP595165_SET_OUTPUT(Sp595165->pin_pl, 0);
    SP595165_SET_OUTPUT(Sp595165->pin_rclk, 0);
  }

  if (Sp595165->oe_configured) {
#ifndef SP595165_IONE_PINS
    SP595165_SET_OUTPUT(Sp595165->pin_oe, 0);
#endif
    Sp595165Enable165(true);  // 165 CE# LOW 유지 (입력 활성)
  }

  Sp595165->first = TasmotaGlobal.devices_present;
  UpdateDevicesPresent(Sp595165->count);
  AddLog(LOG_LEVEL_INFO, PSTR("SP595165: POWER%d..POWER%d, IN1..IN%d"),
    Sp595165->first + 1, TasmotaGlobal.devices_present, Sp595165->has_q7 ? Sp595165->count : 0);
}

/*********************************************************************************************\
 * 루프 / 릴레이 / MQTT
\*********************************************************************************************/

void Sp595165DiagPeriodic(void) {
  if (!Sp595165->has_q7) {
    return;
  }
  uint32_t now = millis();
  if (now - Sp595165->last_diag_ms < 10000) {
    return;
  }
  int q7_level = SP595165_IN(Sp595165->pin_q7);
  uint32_t raw = Sp595165->last_inputs_raw;
  AddLog(LOG_LEVEL_DEBUG, PSTR("SP595165: q7:%d raw:%02X"), q7_level, raw);
  if (Sp595165->last_diag_raw != 0xFFFFFFFF && raw == Sp595165->last_diag_raw && !Sp595165->hw_check_logged) {
    Sp595165->hw_check_logged = true;
    AddLog(LOG_LEVEL_ERROR, PSTR("SP595165: 165/Q7 하드웨어 점검"));
  }
  Sp595165->last_diag_raw = raw;
  Sp595165->last_diag_ms = now;
}

// SP595165 JSON 본문 (RAW + IN1..INn)
static void Sp595165AppendInputsBody(void) {
  ResponseAppend_P(PSTR("\"RAW\":\"%02X\","), Sp595165->last_inputs_raw);
  bool first_done = false;
  for (int i = 0; i < Sp595165->count; i++) {
    if (first_done) { ResponseAppend_P(PSTR(",")); }
    ResponseAppend_P(PSTR("\"IN%d\":%d"), i + 1, bitRead(Sp595165->last_inputs, i));
    first_done = true;
  }
}

// 입력 변화 시 stat/SP595165 전체 발행 (tele/SENSOR TelePeriod 대기 없음)
static void Sp595165PublishStat(void) {
  if (!Sp595165->has_q7) {
    return;
  }
  ResponseTime_P(PSTR(",\"SP595165\":{"));
  Sp595165AppendInputsBody();
  ResponseAppend_P(PSTR("}}"));
  MqttPublishPrefixTopicRulesProcess_P(STAT, PSTR("SP595165"));
}

// 변경된 채널만 stat/SP595165_CHG 발행
static void Sp595165PublishChg(uint32_t last_inputs, uint32_t inputs) {
  bool first_done = false;
  ResponseTime_P(PSTR(",\"SP595165_CHG\":{"));
  for (int i = 0; i < Sp595165->count; i++, last_inputs >>= 1, inputs >>= 1) {
    if ((last_inputs & 1) != (inputs & 1)) {
      if (first_done) { ResponseAppend_P(PSTR(",")); }
      ResponseAppend_P(PSTR("\"IN%d\":%d"), i + 1, (inputs & 1));
      first_done = true;
    }
  }
  ResponseAppend_P(PSTR("}}"));
  if (first_done) {
    MqttPublishPrefixTopicRulesProcess_P(STAT, PSTR("SP595165_CHG"));
  }
}

static void Sp595165OnInputsChanged(uint32_t last_inputs, uint32_t inputs) {
  AddLog(LOG_LEVEL_DEBUG, PSTR("SP595165: in=0x%02X last=0x%02X"), inputs, last_inputs);
  Sp595165PublishStat();
  Sp595165PublishChg(last_inputs, inputs);
}

void Sp595165Loop(void) {
  if (SP595165_MODE_SHIFT595 == Sp595165->mode) {
    Sp595165WarnCeNotConfigured();
    uint32_t last_inputs = Sp595165->last_inputs;
    if (Sp595165->has_q7) {
      Sp595165->last_inputs = Sp595165ReadWriteShift595(TasmotaGlobal.power);
      Sp595165DiagPeriodic();
    }
    if (Sp595165->has_q7 && Sp595165->last_inputs != last_inputs) {
      Sp595165OnInputsChanged(last_inputs, Sp595165->last_inputs);
    }
    return;
  }

  uint32_t inputs = Sp595165ReadWriteDingtian(Sp595165->outputs);
  uint32_t last_inputs = Sp595165->last_inputs;
  Sp595165->last_inputs = inputs;

  if (inputs != last_inputs) {
    Sp595165OnInputsChanged(last_inputs, inputs);
  }
}

void Sp595165SetPower(void) {
  Sp595165->outputs = (XdrvMailbox.index >> Sp595165->first) & ~(0xFFFFFFFFU << Sp595165->count);
  if (SP595165_MODE_SHIFT595 == Sp595165->mode) {
    if (Sp595165->has_q7) {
      Sp595165->last_inputs = Sp595165ReadWriteShift595(XdrvMailbox.index);
    } else {
      Sp595165Shift595FromPower(XdrvMailbox.index);
    }
  } else {
    Sp595165Loop();
  }
}

void Sp595165JsonAppend(void) {
  if (!Sp595165->has_q7) {
    return;
  }
  ResponseAppend_P(PSTR(",\"SP595165\":{"));
  Sp595165AppendInputsBody();
  ResponseAppend_P(PSTR("}"));
}

#ifdef USE_WEBSERVER
const char HTTP_SP595165_INPUTS[] PROGMEM = "{s}SP595165 " D_SENSOR_INPUT "%d.." D_SENSOR_INPUT "%d{m}%s{e}";

void Sp595165WebSensor(void) {
  if (!Sp595165->has_q7) {
    return;
  }
  char input_str[9];
  char raw_str[4];
  snprintf_P(raw_str, sizeof(raw_str), PSTR("%02X"), Sp595165->last_inputs_raw);
  for (int block = 0; block < Sp595165->count; block += 8) {
    for (int i = 0; i < 8; i++) {
      input_str[i] = '0' + bitRead(Sp595165->last_inputs, block + i);
    }
    input_str[8] = '\0';
    WSContentSend_P(HTTP_SP595165_INPUTS, block + 1, block + 8, input_str);
  }
  WSContentSend_P(PSTR("{s}SP595165 RAW{m}%s{e}"), raw_str);
}
#endif

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv95(uint32_t function) {
  bool result = false;

  if (FUNC_SETUP_RING2 == function) {
    Sp595165Init();
  } else if (Sp595165) {
    switch (function) {
      case FUNC_SET_POWER:
        Sp595165SetPower();
        break;
      case FUNC_EVERY_50_MSECOND:
        Sp595165Loop();
        break;
      case FUNC_JSON_APPEND:
        Sp595165JsonAppend();
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Sp595165WebSensor();
        break;
#endif
#ifdef ESP32
      case FUNC_SAVE_BEFORE_RESTART:
        if (SP595165_MODE_SHIFT595 == Sp595165->mode) {
          gpio_hold_en((gpio_num_t)Sp595165->pin_srclk);
          gpio_hold_en((gpio_num_t)Sp595165->pin_rclk);
        }
        break;
#endif
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  }
  return result;
}

#endif  // USE_SP595165
#endif  // ESP32
