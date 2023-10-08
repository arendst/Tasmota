#ifndef BUILD_OPTIONS_H
#define BUILD_OPTIONS_H

#if defined(DISABLE_ALL_OPTIONS)
#undef ENABLE_NTP_TIME
#undef ENABLE_ERROR_STRING
#undef ENABLE_IMAP 
#undef ENABLE_SMTP 
#undef ESP_MAIL_USE_PSRAM
#undef ESP_MAIL_DEFAULT_SD_FS
#undef ESP_MAIL_CARD_TYPE_SD
#undef ESP_MAIL_CARD_TYPE_SD_MMC
#undef ESP_MAIL_DEFAULT_FLASH_FS
#undef ESP_MAIL_DEFAULT_DEBUG_PORT
#endif

#if defined(DISABLE_NTP_TIME)
#undef ENABLE_NTP_TIME
#endif

#if defined(DISABLE_ERROR_STRING)
#undef ENABLE_ERROR_STRING
#endif

#if defined(DISABLE_IMAP)
#undef ENABLE_IMAP 
#endif

#if defined(DISABLE_SMTP)
#undef ENABLE_SMTP 
#endif

#if defined(DISABLE_PSRAM)
#undef ESP_MAIL_USE_PSRAM 
#endif

#if defined(DISABLE_SD)
#undef ESP_MAIL_DEFAULT_SD_FS
#undef ESP_MAIL_CARD_TYPE_SD_MMC
#undef ESP_MAIL_DEFAULT_FLASH_FS
#endif

#if defined(DISABLE_FLASH)
#undef ESP_MAIL_DEFAULT_FLASH_FS
#undef ESP_MAIL_FORMAT_FLASH_IF_MOUNT_FAILED
#endif

#if defined(ESP_MAIL_DEFAULT_DEBUG_PORT)
#undef ESP_MAIL_DEFAULT_DEBUG_PORT
#endif

#endif