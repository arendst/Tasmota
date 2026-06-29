/*
  xdrv_95_esp32_sp595165.ino - IONE 74HC595 + 74HC165 8CH 보드 (SP595165)

  ESP32 핀 (IONE 보일러 밸브 스키매틱):
    IO32  74HC_IN   -> 595 SER
    IO33  74HC_RCK  -> 595 RCK / 165 PL#  (공유)
    IO25  74HC_SCK  -> 595 SCK / 165 CP   (공유)
    IO26  74HC_OUT  -> 165 Q7
    IO13  74HC_EN   -> 595 OE# / 165 CE#  (공유, Active-Low, R5 풀업)

  165 D0..D7 -> PIN8,PIN6,PIN4,PIN2,PIN1,PIN3,PIN5,PIN7
  MQTT IN1..IN8 = 실크 PIN1..PIN8

  SetOption133 - 595 출력 반전
  SetOption81  - 165 입력 반전 (Active-Low 입력 보드)
*/

#ifdef ESP32
#ifdef USE_SP595165

#define XDRV_95           95

#define SP595165_SET_OUTPUT(pin, value)  { pinMode((pin), OUTPUT); digitalWrite((pin), (value)); }
#define SP595165_SET_INPUT(pin)          { pinMode((pin), INPUT); }

// DingtianReadWrite 비트 순서(D7..D0) -> IN1(PIN1)..IN8(PIN8)
static const uint8_t kSp595165RawBitForIn[8] = { 3, 4, 2, 5, 1, 6, 0, 7 };

struct SP595165_DATA {
  uint32_t outputs;
  uint32_t last_inputs;
  uint8_t  count;
  uint8_t  first;
  uint8_t  pin_clk;
  uint8_t  pin_sdi;
  uint8_t  pin_q7;
  uint8_t  pin_pl;
  uint8_t  pin_oe;
  uint8_t  pin_rck;
} *Sp595165 = nullptr;

/*********************************************************************************************\
 * 165 raw -> IN1..IN8 (PIN1..PIN8)
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

/*********************************************************************************************\
 * 595/165 공용 버스 읽기·쓰기
\*********************************************************************************************/

uint32_t Sp595165ReadWrite(uint32_t outputs) {
  if (Settings->flag5.shift595_invert_outputs) {  // SetOption133
    outputs = ~outputs;
  }

  // EN(OE#/CE#) LOW — 595 출력·165 클럭 동시 활성 (R5 풀업 대비)
  if (PinUsed(GPIO_DINGTIAN_OE)) {
    digitalWrite(Sp595165->pin_oe, 0);
  }

  uint32_t inputs = 0;
  uint32_t in_bit = 1;

  // 165 병렬 로드 (PL# Active-Low)
  digitalWrite(Sp595165->pin_rck, 0);
  if (PinUsed(GPIO_DINGTIAN_PL)) {
    digitalWrite(Sp595165->pin_pl, 0);
    delayMicroseconds(2);
    digitalWrite(Sp595165->pin_pl, 1);  // 165 시프트 모드
  }

  for (int i = Sp595165->count; i > 0; i--) {
    digitalWrite(Sp595165->pin_sdi, outputs & 1);
    outputs >>= 1;
    inputs |= digitalRead(Sp595165->pin_q7) ? in_bit : 0;
    in_bit <<= 1;
    digitalWrite(Sp595165->pin_clk, 1);
    digitalWrite(Sp595165->pin_clk, 0);
  }

  // 595 출력 래치
  digitalWrite(Sp595165->pin_rck, 1);
  if (PinUsed(GPIO_DINGTIAN_PL)) {
    digitalWrite(Sp595165->pin_pl, 0);
  }

  if (Settings->flag3.pcf8574_ports_inverted) {  // SetOption81
    inputs = ~inputs;
  }
  return Sp595165RemapInputs(inputs);
}

/*********************************************************************************************\
 * 초기화
\*********************************************************************************************/

void Sp595165Init(void) {
  if (PinUsed(GPIO_DINGTIAN_CLK, GPIO_ANY) && PinUsed(GPIO_DINGTIAN_SDI) && PinUsed(GPIO_DINGTIAN_Q7)
   && PinUsed(GPIO_DINGTIAN_PL) && PinUsed(GPIO_DINGTIAN_OE)) {
    Sp595165 = (struct SP595165_DATA*)calloc(1, sizeof(struct SP595165_DATA));
    if (Sp595165) {
      Sp595165->pin_clk = Pin(GPIO_DINGTIAN_CLK, GPIO_ANY);
      Sp595165->pin_sdi = Pin(GPIO_DINGTIAN_SDI);
      Sp595165->pin_q7  = Pin(GPIO_DINGTIAN_Q7);
      Sp595165->pin_pl  = Pin(GPIO_DINGTIAN_PL);
      Sp595165->pin_oe  = Pin(GPIO_DINGTIAN_OE);
      if (PinUsed(GPIO_DINGTIAN_RCK)) {
        Sp595165->pin_rck = Pin(GPIO_DINGTIAN_RCK);
      } else {
        Sp595165->pin_rck = Sp595165->pin_pl;
      }

      Sp595165->count = ((GetPin(Sp595165->pin_clk) - AGPIO(GPIO_DINGTIAN_CLK)) + 1) * 8;

      AddLog(LOG_LEVEL_DEBUG, PSTR("SP595165: clk:%d sdi:%d q7:%d pl:%d oe:%d rck:%d ch:%d SO133:%d SO81:%d"),
        Sp595165->pin_clk, Sp595165->pin_sdi, Sp595165->pin_q7, Sp595165->pin_pl, Sp595165->pin_oe, Sp595165->pin_rck,
        Sp595165->count,
        Settings->flag5.shift595_invert_outputs,
        Settings->flag3.pcf8574_ports_inverted);

      SP595165_SET_OUTPUT(Sp595165->pin_clk, 0);
      SP595165_SET_OUTPUT(Sp595165->pin_sdi, 0);
      SP595165_SET_INPUT(Sp595165->pin_q7);
      SP595165_SET_OUTPUT(Sp595165->pin_pl, 0);
      SP595165_SET_OUTPUT(Sp595165->pin_rck, 0);
      SP595165_SET_OUTPUT(Sp595165->pin_oe, 0);
      digitalWrite(Sp595165->pin_oe, 0);

      Sp595165->first = TasmotaGlobal.devices_present;
      UpdateDevicesPresent(Sp595165->count);
      AddLog(LOG_LEVEL_INFO, PSTR("SP595165: POWER%d..POWER%d, IN1..IN%d"),
        Sp595165->first + 1, TasmotaGlobal.devices_present, Sp595165->count);
    }
  }
}

/*********************************************************************************************\
 * 루프 / 릴레이 / MQTT
\*********************************************************************************************/

void Sp595165Loop(void) {
  uint32_t inputs = Sp595165ReadWrite(Sp595165->outputs);
  uint32_t last_inputs = Sp595165->last_inputs;
  Sp595165->last_inputs = inputs;

  if (inputs != last_inputs) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("SP595165: in=0x%02X last=0x%02X"), inputs, last_inputs);
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
}

void Sp595165SetPower(void) {
  Sp595165->outputs = (XdrvMailbox.index >> Sp595165->first) & ~(0xFFFFFFFFU << Sp595165->count);
  Sp595165Loop();
}

void Sp595165JsonAppend(void) {
  bool first_done = false;
  ResponseAppend_P(PSTR(",\"SP595165\":{"));
  for (int i = 0; i < Sp595165->count; i++) {
    if (first_done) { ResponseAppend_P(PSTR(",")); }
    ResponseAppend_P(PSTR("\"IN%d\":%d"), i + 1, bitRead(Sp595165->last_inputs, i));
    first_done = true;
  }
  ResponseAppend_P(PSTR("}"));
}

#ifdef USE_WEBSERVER
const char HTTP_SP595165_INPUTS[] PROGMEM = "{s}SP595165 " D_SENSOR_INPUT "%d.." D_SENSOR_INPUT "%d{m}%s{e}";

void Sp595165WebSensor(void) {
  char input_str[9];
  for (int block = 0; block < Sp595165->count; block += 8) {
    for (int i = 0; i < 8; i++) {
      input_str[i] = '0' + bitRead(Sp595165->last_inputs, block + i);
    }
    input_str[8] = '\0';
    WSContentSend_P(HTTP_SP595165_INPUTS, block + 1, block + 8, input_str);
  }
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
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  }
  return result;
}

#endif  // USE_SP595165
#endif  // ESP32
