#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define console_printf(_fmt, ...) printf(_fmt, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* __CONSOLE_H__ */