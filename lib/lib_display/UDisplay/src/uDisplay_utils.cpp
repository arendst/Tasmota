#include "uDisplay.h"
#include "uDisplay_config.h"

// ===== String and Parsing Utilities =====

uint8_t uDisplay::strlen_ln(char *str) {
  for (uint32_t cnt = 0; cnt < 256; cnt++) {
    if (!str[cnt] || str[cnt] == '\n' || str[cnt] == ' ') return cnt;
  }
  return 0;
}

char *uDisplay::devname(void) {
  return dname;
}

uint32_t uDisplay::str2c(char **sp, char *vp, uint32_t len) {
    char *lp = *sp;
    if (len) len--;
    char *cp = strchr(lp, ',');
    if (cp) {
        while (1) {
            if (*lp == ',') {
                *vp = 0;
                *sp = lp + 1;
                return 0;
            }
            if (len) {
                *vp++ = *lp++;
                len--;
            } else {
                lp++;
            }
        }
    } else {
      uint16_t slen = strlen(lp);
      if (slen) {
        strlcpy(vp, *sp, len);
        *sp = lp + slen;
        return 0;
      }
    }
    return 1;
}

int32_t uDisplay::next_val(char **sp) {
  char ibuff[16];
  if (!str2c(sp, ibuff, sizeof(ibuff))) {
    return atoi(ibuff);
  }
  return 0xff;
}

uint32_t uDisplay::next_hex(char **sp) {
  char ibuff[16];
  if (!str2c(sp, ibuff, sizeof(ibuff))) {
    return strtol(ibuff, 0, 16);
  }
  return 0xff;
}

// ===== Touch Coordinate Conversion =====
// the cases are PSEUDO_OPCODES from MODULE_DESCRIPTOR
// and may be expanded with more opcodes
void uDisplay::TS_RotConvert(int16_t *x, int16_t *y) {
  int16_t temp;

  if (rot_t[cur_rot] & 0x80) {
    temp = *y;
    *y = *x;
    *x = temp;
  }

  if (rotmap_xmin >= 0) {
    *y = map(*y, rotmap_ymin, rotmap_ymax, 0, gys);
    *x = map(*x, rotmap_xmin, rotmap_xmax, 0, gxs);
    *x = constrain(*x, 0, gxs);
    *y = constrain(*y, 0, gys);
  }

  switch (rot_t[cur_rot] & 0xf) {
    case 0:
      break;
    case 1:
      temp = *y;
      *y = height() - *x;
      *x = temp;
      break;
    case 2:
      *x = width() - *x;
      *y = height() - *y;
      break;
    case 3:
      temp = *y;
      *y = *x;
      *x = width() - temp;
      break;
    case 4:
      *x = width() - *x;
      break;
    case 5:
      *y = height() - *y;
      break;
  }
}

// ===== Color Conversion Helper =====

// static inline void lvgl_color_swap(uint16_t *data, uint16_t len) { 
//   for (uint32_t i = 0; i < len; i++) (data[i] = data[i] << 8 | data[i] >> 8); 
// }