
#pragma once

#ifndef ESP_MAIL_VERSION

#define ESP_MAIL_VERSION "3.4.9"
#define ESP_MAIL_VERSION_NUM 30409

/* The inconsistent file version checking to prevent mixed versions compilation. */
#define VALID_VERSION_CHECK(ver) (ver == ESP_MAIL_VERSION_NUM)

#endif