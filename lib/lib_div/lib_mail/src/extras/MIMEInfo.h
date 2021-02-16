#ifndef MIMEInfo_H
#define MIMEInfo_H
#include <Arduino.h>

enum esp_mail_file_extension
{
  esp_mail_file_extension_html,
  esp_mail_file_extension_htm,
  esp_mail_file_extension_css,
  esp_mail_file_extension_txt,
  esp_mail_file_extension_js,
  esp_mail_file_extension_json,
  esp_mail_file_extension_png,
  esp_mail_file_extension_gif,
  esp_mail_file_extension_jpg,
  esp_mail_file_extension_ico,
  esp_mail_file_extension_svg,
  esp_mail_file_extension_ttf,
  esp_mail_file_extension_otf,
  esp_mail_file_extension_woff,
  esp_mail_file_extension_woff2,
  esp_mail_file_extension_eot,
  esp_mail_file_extension_sfnt,
  esp_mail_file_extension_xml,
  esp_mail_file_extension_pdf,
  esp_mail_file_extension_zip,
  esp_mail_file_extension_gz,
  esp_mail_file_extension_appcache,
  esp_mail_file_extension_none,
  esp_mail_file_extension_maxType
};

struct esp_mail_mime_prop_t
{
   char endsWith[10];
   char mimeType[50];
};

const struct esp_mail_mime_prop_t mimeinfo[esp_mail_file_extension_maxType] PROGMEM =
    {
        {".html", "text/html"},
        {".htm", "text/html"},
        {".css", "text/css"},
        {".txt", "text/plain"},
        {".js", "application/javascript"},
        {".json", "application/json"},
        {".png", "image/png"},
        {".gif", "image/gif"},
        {".jpg", "image/jpeg"},
        {".ico", "image/x-icon"},
        {".svg", "image/svg+xml"},
        {".ttf", "application/x-font-ttf"},
        {".otf", "application/x-font-opentype"},
        {".woff", "application/font-woff"},
        {".woff2", "application/font-woff2"},
        {".eot", "application/vnd.ms-fontobject"},
        {".sfnt", "application/font-sfnt"},
        {".xml", "text/xml"},
        {".pdf", "application/pdf"},
        {".zip", "application/zip"},
        {".gz", "application/x-gzip"},
        {".appcache", "text/cache-manifest"},
        {"", "application/octet-stream"}};

#endif
