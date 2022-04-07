Attention when updating library. Changes in lib needed!!

Changes in src/nimble/porting/nimble/include/log_common/log_common.h

```
/*
#define LOG_LEVEL_DEBUG    (0)
#define LOG_LEVEL_INFO     (1)
#define LOG_LEVEL_WARN     (2)
#define LOG_LEVEL_ERROR    (3)
#define LOG_LEVEL_CRITICAL (4)
#define LOG_LEVEL_NONE     (5)
*/
#define LOG_LEVEL_DEBUG    (3)
#define LOG_LEVEL_INFO     (2)
#define LOG_LEVEL_WARN     (4)
#define LOG_LEVEL_ERROR    (1)
#define LOG_LEVEL_CRITICAL (5)
#define LOG_LEVEL_NONE     (0)
```

Change in nimconfig.h

`#define CONFIG_BT_NIMBLE_NVS_PERSIST 1`
to
`#define CONFIG_BT_NIMBLE_NVS_PERSIST 0`

and line 249 from

`
#if !defined(CONFIG_IDF_TARGET_ESP32) && !defined(CONFIG_IDF_TARGET_ESP32C3)
`

to

`
#if !defined(CONFIG_IDF_TARGET_ESP32) && !defined(CONFIG_IDF_TARGET_ESP32C3) && !defined(CONFIG_IDF_TARGET_ESP32S3)
`
