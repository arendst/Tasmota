#include "uDisplay.h"
#include "uDisplay_config.h"

#ifdef USE_UNIVERSAL_TOUCH

// ===== Touch IRQ Handler =====

uint8_t ut_irq_flg;

void IRAM_ATTR ut_touch_irq(void) {
  ut_irq_flg = 1;
}

// ===== Touch Initialization =====

bool uDisplay::utouch_Init(char **name) {
  *name = ut_name;
  
  if (ut_init_code) {
    if (ut_reset >= 0) {
      pinMode(ut_reset, OUTPUT);
      digitalWrite(ut_reset, HIGH);
      delay(10);
      digitalWrite(ut_reset, LOW);
      delay(5);
      digitalWrite(ut_reset, HIGH);
      delay(10);
    }
    
    if (ut_irq >= 0) {
      pinMode(ut_irq, INPUT);
      attachInterrupt(ut_irq, ut_touch_irq, FALLING);
    }

    if (ut_spi_nr == spi_nr) {
      // same as display
      ut_spi = uspi;
    } else {
#ifdef ESP32
      ut_spi = SpiBegin(ut_spi_nr);
#endif
    }
    
    return ut_execute(ut_init_code);
  }
  
  return false;
}

// ===== Touch Detection =====

uint16_t uDisplay::touched(void) {
  if (ut_irq >= 0) {
    if (!ut_irq_flg) {
      return false;
    }
    ut_irq_flg = 0;
  }
  
  if (ut_touch_code) {
    return ut_execute(ut_touch_code);
  }
  
  return 0;
}

// ===== Touch Coordinate Reading =====

int16_t uDisplay::getPoint_x(void) {
  if (ut_getx_code) {
    return ut_execute(ut_getx_code);
  }
  return 0;
}

int16_t uDisplay::getPoint_y(void) {
  if (ut_gety_code) {
    return ut_execute(ut_gety_code);
  }
  return 0;
}

// ===== Touch Command Execution =====

// ===== Touch Code Translation =====

void uDisplay::ut_trans(char **sp, uint8_t **code) {
  char *cp = *sp;
  uint16_t wval;
  uint8_t tmp_code[64];
  uint8_t *ut_code = tmp_code;
  
  while (*cp) {
    if (*cp == ':' || *cp == '#') {
      break;
    }
    if (*cp == ';') {
      // skip comment line
      while (*cp) {
        if (*cp == '\n') {
          cp++;
          break;
        }
        cp++;
      }
    }
    
    if (!strncmp(cp, "RDWM", 4)) {
      // read word many
      *ut_code++ = UT_RDWM;
      wval = ut_par(&cp, 0);
      *ut_code++ = wval>>8;
      *ut_code++ = wval;
      wval = ut_par(&cp, 1);
      if (wval > sizeof(ut_array)) {
        wval = sizeof(ut_array);
      }
      *ut_code++ = wval;
    } else if (!strncmp(cp, "RDW", 3)) {
      // read word one
      *ut_code++ = UT_RDW;
      wval = ut_par(&cp, 0);
      *ut_code++ = wval>>8;
      *ut_code++ = wval;
    } else if (!strncmp(cp, "RDM", 3)) {
      // read many
      *ut_code++ = UT_RDM;
      *ut_code++ = ut_par(&cp, 0);
      wval = ut_par(&cp, 1);
      if (wval > sizeof(ut_array)) {
        wval = sizeof(ut_array);
      }
      *ut_code++ = wval;
    } else if (!strncmp(cp, "RD", 2)) {
      // read one
      *ut_code++ = UT_RD;
      *ut_code++ = ut_par(&cp, 0);
    } else if (!strncmp(cp, "CPR", 3)) {
      // cmp and set
      *ut_code++ = UT_CPR;
      *ut_code++ = ut_par(&cp, 0);
    } else if (!strncmp(cp, "CPM", 3)) {
      // cmp multiple and set
      *ut_code++ = UT_CPM;
      uint8_t num = ut_par(&cp, 0);
      *ut_code++ = num;
      for (uint32_t cnt = 0; cnt < num; cnt++) {
        *ut_code++ = ut_par(&cp, 0);
      }
    } else if (!strncmp(cp, "CP", 2)) {
      // cmp and set
      *ut_code++ = UT_CP;
      *ut_code++ = ut_par(&cp, 0);
    } else if (!strncmp(cp, "RTF", 3)) {
      // return when false
      *ut_code++ = UT_RTF;
    } else if (!strncmp(cp, "RTT", 3)) {
      // return when true
      *ut_code++ = UT_RTT;
    } else if (!strncmp(cp, "MVB", 3)) {
      // move
      *ut_code++ = UT_MVB;
      *ut_code++ = ut_par(&cp, 1);
      *ut_code++ = ut_par(&cp, 1);
    } else if (!strncmp(cp, "MV", 2)) {
      // move
      *ut_code++ = UT_MV;
      *ut_code++ = ut_par(&cp, 1);
      *ut_code++ = ut_par(&cp, 1);
    } else if (!strncmp(cp, "RT", 2)) {
      // return status
      *ut_code++ = UT_RT;
    } else if (!strncmp(cp, "WRW", 3)) {
      *ut_code++ = UT_WRW;
      wval = ut_par(&cp, 0);
      *ut_code++ = wval>>8;
      *ut_code++ = wval;
      wval = ut_par(&cp, 0);
      *ut_code++ = wval;
    } else if (!strncmp(cp, "WR", 2)) {
      *ut_code++ = UT_WR;
      wval = ut_par(&cp, 0);
      *ut_code++ = wval;
      wval = ut_par(&cp, 0);
      *ut_code++ = wval;
    } else if (!strncmp(cp, "AND", 3)) {
      *ut_code++ = UT_AND;
      wval = ut_par(&cp, 0);
      *ut_code++ = wval >> 8;
      *ut_code++ = wval;
    } else if (!strncmp(cp, "SCL", 3)) {
      *ut_code++ = UT_SCALE;
      wval = ut_par(&cp, 1);
      *ut_code++ = wval >> 8;
      *ut_code++ = wval;
      uint32_t lval = ut_par(&cp, 2);
      *ut_code++ = lval >> 24;
      *ut_code++ = lval >> 16;
      *ut_code++ = lval >> 8;
      *ut_code++ = lval;
    } else if (!strncmp(cp, "LIM", 3)) {
      *ut_code++ = UT_LIM;
      wval = ut_par(&cp, 1);
      *ut_code++ = wval >> 8;
      *ut_code++ = wval;
    } else if (!strncmp(cp, "GSRT", 4)) {
      *ut_code++ = UT_GSRT;
      wval = ut_par(&cp, 1);
      *ut_code++ = wval >> 8;
      *ut_code++ = wval;
    } else if (!strncmp(cp, "XPT", 3)) {
      *ut_code++ = UT_XPT;
      wval = ut_par(&cp, 1);
      *ut_code++ = wval >> 8;
      *ut_code++ = wval;
    } else if (!strncmp(cp, "DBG", 3)) {
      *ut_code++ = UT_DBG;
      wval = ut_par(&cp, 1);
      *ut_code++ = wval;
    }
    cp++;
  }
  
  *ut_code++ = UT_END;
  *sp = cp - 1;
  uint16_t memsize = (uint32_t)ut_code - (uint32_t)tmp_code;
  
  // allocate memory
  uint8_t *mp = (uint8_t*)malloc(memsize + 2);
  if (mp) {
    memmove(mp, tmp_code, memsize);
    *code = mp;
  }
}

// ===== Touch Parameter Parsing =====

uint32_t uDisplay::ut_par(char **lp, uint32_t mode) {
  char *cp = *lp;
  while (*cp != ' ') {
    if (!cp) break;
    cp++;
  }
  cp++;
  uint32_t result;
  
  if (!mode) {
    // hex
    result = strtol(cp, &cp, 16);
  } else if (mode == 1) {
    // word
    result = strtol(cp, &cp, 10);
  } else {
    // float as 32bit integer
    float fval = CharToFloat(cp);
    result = *(uint32_t*)&fval;
    while (*cp) {
      if (*cp == ' ' || *cp =='\n') {
        break;
      }
      cp++;
    }
  }
  
  *lp = cp;
  return result;
}

int16_t uDisplay::ut_execute(uint8_t *ut_code) {
  int16_t result = 0;
  uint8_t iob, len;
  uint16_t wval;

  while (*ut_code != UT_END) {
    iob = *ut_code++;
    switch (iob) {
      case UT_RD:
        // read 1 byte
        ut_code = ut_rd(ut_code, 1, 1);
        break;

      case UT_RDM:
        // read multiple bytes
        ut_code = ut_rd(ut_code, 2, 1);
        break;

      case UT_RDW:
        // read 1 byte
        ut_code = ut_rd(ut_code, 1, 2);
        break;

      case UT_RDWM:
        // read multiple bytes
        ut_code = ut_rd(ut_code, 2, 2);
        break;

      case UT_WR:
        ut_code = ut_wr(ut_code, 1);
        break;

      case UT_WRW:
        ut_code = ut_wr(ut_code, 2);
        break;

      case UT_CP:
        // compare
        iob = *ut_code++;
        result = (iob == ut_array[0]);
        break;

      case UT_CPM:
        // compare multiple
        len = *ut_code++;
        result = 0;
        for (uint32_t cnt = 0; cnt < len; cnt++) {
          iob = *ut_code++;
          result |= (iob == ut_array[0]);
        }
        break;

      case UT_CPR:
        // compare
        iob = *ut_code++;
        result = (iob == result);
        break;

      case UT_RTF:
        // return when false
        if (result == 0) {
          return false;
        }
        break;

      case UT_RTT:
        // return when true
        if (result > 0) {
          return false;
        }
        break;

      case UT_MVB:
        // move byte from index to high or low result
        wval = *ut_code++;
        iob = *ut_code++;
        if (wval == 0) {
          result &= 0xff00;
          result |= ut_array[iob]; 
        } else {
          result &= 0x00ff;
          result |= (ut_array[iob] << 8); 
        }
        break;

      case UT_MV:
        // move
        // source
        result = *ut_code++;
        iob = *ut_code++;
        if (iob == 1) {
          result = ut_array[result];
        } else if (iob == 2) {
          iob = result;
          result = ut_array[iob] << 8;
          result |= ut_array[iob + 1];
        } else {
          iob = result;
          result = ut_array[iob + 1] << 8;
          result |= ut_array[iob];
        }
        result &= 0xfff;
        break;

      case UT_AND:
        // and 
        wval = *ut_code++ << 8;
        wval |= *ut_code++;
        result &= wval;
        break;
      
      case UT_SCALE:
        {
          wval = *ut_code++ << 8;
          wval |= *ut_code++;
          result -= wval;
          uint32_t lval = (uint32_t)*ut_code++ << 24;
          lval |= (uint32_t)*ut_code++ << 16;
          lval |= (uint32_t)*ut_code++ << 8;
          lval |= (uint32_t)*ut_code++;
          float fval = *(float*)&lval;
          fval *= (float)result;  
          result = fval;
        }
        break;

      case UT_LIM:
        wval = *ut_code++ << 8;
        wval |= *ut_code++;
        if (result > wval) {
          result = wval;
        }
        break;

      case UT_RT:
        // result
        return result;
        break;

      case UT_GSRT:
#ifdef USE_ESP32_S3      
        { 
          uint32_t val = get_sr_touch(SIMPLERS_XP, SIMPLERS_XM, SIMPLERS_YP, SIMPLERS_YM);
          if (val == 0) {
            return false;
          }
          uint16_t xp = val >> 16;
          uint16_t yp = val;

          wval = *ut_code++ << 8;
          wval |= *ut_code++;
          if (xp > wval && yp > wval) {
            ut_array[0] = val >> 24;
            ut_array[1] = val >> 16;
            ut_array[2] = val >> 8;
            ut_array[3] = val;
            return true;
          }
          return false;
        }
#endif // USE_ESP32_S3
        break;

      case UT_XPT:
        wval = *ut_code++ << 8;
        wval |= *ut_code++;
        result = ut_XPT2046(wval);
        break;

      case UT_DBG:
        // debug show result
        wval = *ut_code++;
        AddLog(LOG_LEVEL_INFO, PSTR("UTDBG %d: %02x : %02x,%02x,%02x,%02x"), wval, result, ut_array[0], ut_array[1], ut_array[2], ut_array[3]);
        break;

      case UT_END:
        break;
    }
  }
  
  return result;
}

// ===== Low-Level Touch Communication =====
uint8_t *uDisplay::ut_rd(uint8_t *iop, uint32_t len, uint32_t amode) {
  if (ut_wire) {
    // i2c mode
    ut_wire->beginTransmission(ut_i2caddr);
    ut_wire->write(*iop++);
    if (amode == 2) {
      ut_wire->write(*iop++);
    }
    ut_wire->endTransmission(false);
    if (len > 1) {
      len = *iop++;
    }
    ut_wire->requestFrom(ut_i2caddr, (size_t)len);
    uint8_t index = 0;
    while (ut_wire->available()) {
      ut_array[index++] = ut_wire->read();
    }
  } else {
    // spi mode
    if (amode == 1) {
      uint16_t val = *iop++;
      uint16_t len = *iop++;
      if (ut_spi) {
        digitalWrite(ut_spi_cs, LOW);
        ut_spi->beginTransaction(ut_spiSettings);
        ut_spi->transfer(val);
        val = ut_spi->transfer16(0);
        ut_spi->endTransaction();
        ut_array[len] = val << 8;
        ut_array[len + 1] = val;
        digitalWrite(ut_spi_cs, HIGH);
      }
    }
  }
  
  return iop;
}

uint8_t *uDisplay::ut_wr(uint8_t *iop, uint32_t amode) {
  if (ut_wire) {
    // i2c mode
    ut_wire->beginTransmission(ut_i2caddr);
    ut_wire->write(*iop++);
    if (amode == 2) {
      ut_wire->write(*iop++);
    }
    ut_wire->write(*iop++);
    ut_wire->endTransmission(true);
  } else {
    // spi mode
  }
  
  return iop;
}

// ===== XPT2046 Touch Controller =====

uint16_t uDisplay::ut_XPT2046(uint16_t z_th) {
  uint16_t result = 0;
  
  if (ut_spi) {
    int16_t data[6];
    ut_spi->beginTransaction(ut_spiSettings);
    digitalWrite(ut_spi_cs, LOW);
    ut_spi->transfer(0xB1 /* Z1 */);
    int16_t z1 = ut_spi->transfer16(0xC1 /* Z2 */) >> 3;
    int16_t z = z1 + 4095;
    int16_t z2 = ut_spi->transfer16(0x91 /* X */) >> 3;
    z -= z2;
    
    if (z >= z_th) {
      ut_spi->transfer16(0x91 /* X */);  // dummy X measure, 1st is always noisy
      data[0] = ut_spi->transfer16(0xD1 /* Y */) >> 3;
      data[1] = ut_spi->transfer16(0x91 /* X */) >> 3; // make 3 x-y measurements
      data[2] = ut_spi->transfer16(0xD1 /* Y */) >> 3;
      data[3] = ut_spi->transfer16(0x91 /* X */) >> 3;
      result = 1;
    } else {
      data[0] = data[1] = data[2] = data[3] = 0;
    }
    
    data[4] = ut_spi->transfer16(0xD0 /* Y */) >> 3;  // Last Y touch power down
    data[5] = ut_spi->transfer16(0) >> 3;
    digitalWrite(ut_spi_cs, HIGH);
    ut_spi->endTransaction();

    uint16_t x = besttwoavg(data[0], data[2], data[4]);
    uint16_t y = besttwoavg(data[1], data[3], data[5]);

    ut_array[0] = x >> 8;
    ut_array[1] = x;
    ut_array[2] = y >> 8;
    ut_array[3] = y;
  }
  
  return result;
}

// ===== Touch Data Processing =====

int16_t uDisplay::besttwoavg(int16_t x, int16_t y, int16_t z) {
  int16_t da, db, dc;
  int16_t reta = 0;
  
  if (x > y) da = x - y; else da = y - x;
  if (x > z) db = x - z; else db = z - x;
  if (z > y) dc = z - y; else dc = y - z;

  if (da <= db && da <= dc) reta = (x + y) >> 1;
  else if (db <= da && db <= dc) reta = (x + z) >> 1;
  else reta = (y + z) >> 1;

  return (reta);
}

#endif // USE_UNIVERSAL_TOUCH