// Created August 20, 2023

#pragma once

#ifndef ESP_MAIL_CONST_H
#define ESP_MAIL_CONST_H

#include "ESP_Mail_Client_Version.h"
#if !VALID_VERSION_CHECK(30409)
#error "Mixed versions compilation."
#endif

#include <Arduino.h>
#include "ESP_Mail_FS.h"
#include "ESP_Mail_Error.h"
#include "extras/MB_FS.h"
#include "extras/RFC2047.h"
#include <time.h>
#include <ctype.h>

#if defined(ESP_MAIL_DEBUG_PORT)
#define ESP_MAIL_DEFAULT_DEBUG_PORT ESP_MAIL_DEBUG_PORT
#else
#define ESP_MAIL_DEFAULT_DEBUG_PORT Serial
#endif

#include <vector>
#include <algorithm>

#define _vectorImpl std::vector
#define MB_VECTOR std::vector

#if !defined(FPSTR)
#define FPSTR
#endif

#include "extras/Networks_Provider.h"
#include "extras/ESP8266_Supports.h"

#if defined(ESP8266)
#if __has_include(<core_esp8266_version.h>)
#include <core_esp8266_version.h>
#endif
#endif

#if defined(ENABLE_IMAP) && (defined(ESP32) || defined(ESP8266) || defined(MB_ARDUINO_PICO))
#if defined(ESP32)
#include <Update.h>
#elif defined(ESP8266) || defined(MB_ARDUINO_PICO)
#include <Updater.h>
#endif
#define ESP_MAIL_OTA_UPDATE_ENABLED
#endif

#if !defined(SILENT_MODE) && (defined(ENABLE_SMTP) || defined(ENABLE_IMAP))
#define SESSION_DEBUG_ENABLED
#endif

#define TCP_CLIENT_DEFAULT_TCP_TIMEOUT_SEC 30

#if defined(ENABLE_SMTP) || defined(ENABLE_IMAP)

#define MAX_EMAIL_SEARCH_LIMIT 1000
#define BASE64_CHUNKED_LEN 76
#define FLOWED_TEXT_LEN 78
#define QP_ENC_MSG_LEN 76
#define ESP_MAIL_NETWORK_RECONNECT_TIMEOUT 10000
#define ESP_MAIL_PROGRESS_REPORT_STEP 5
#define ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED 0
#define ESP_MAIL_CLIENT_STREAM_CHUNK_SIZE 256
#define ESP_MAIL_CLIENT_RESPONSE_BUFFER_SIZE 1024 // should be 1 k or more to prevent buffer overflow
#define ESP_MAIL_CLIENT_VALID_TS 1577836800

#endif

typedef enum
{
    esp_mail_cert_type_undefined = -1,
    esp_mail_cert_type_none = 0,
    esp_mail_cert_type_data,
    esp_mail_cert_type_file,
    esp_mail_cert_type_bundle

} esp_mail_cert_type;

typedef enum
{
    esp_mail_client_type_undefined,
    esp_mail_client_type_internal_basic_client,
    esp_mail_client_type_external_basic_client,
    esp_mail_client_type_external_gsm_client

} esp_mail_client_type;

/* The filesystem types enum */
enum esp_mail_file_storage_type
{
    esp_mail_file_storage_type_none,
    esp_mail_file_storage_type_flash,
    esp_mail_file_storage_type_sd
};

/* The session types enum */
enum esp_mail_session_type
{
    esp_mail_session_type_smtp,
    esp_mail_session_type_imap
};

/* The secure connection mode preference */
typedef enum
{
    // No preferences
    esp_mail_secure_mode_undefined = 0,
    // Always use SSL and TLS via STARTTLS rfc2595 section 3 and rfc3207
    esp_mail_secure_mode_ssl_tls,
    // Plain text mode only (non SSL/TLS)
    // To disable SSL/TLS permanently, define ESP_MAIL_DISABLE_SSL in ESP_Mail_FS.h
    esp_mail_secure_mode_nonsecure

} esp_mail_secure_mode;

using namespace mb_string;

#if defined(ENABLE_SMTP) || defined(ENABLE_IMAP)

typedef void (*NetworkConnectionHandler)(void);

/* The file types enum */
enum esp_mail_file_extension
{
    /** The arrangement is related to mimeinfo struct.
     *  Do not modify or remove.
     */
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
    esp_mail_file_extension_dat,
    esp_mail_file_extension_binary,
    esp_mail_file_extension_maxType
};

struct esp_mail_mime_prop_t
{
    char endsWith[10];
    char mimeType[30];
};

const struct esp_mail_mime_prop_t mimeinfo[esp_mail_file_extension_maxType] PROGMEM = {
    /** The arrangement is related to esp_mail_file_extension enum.
     *  Do not modify or remove.
     */
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
    {".dat", "application/dat"},
    {"", "application/octet-stream"}};

#if defined(ENABLE_SMTP)

/* The SMTP commands types enum */
enum esp_mail_smtp_command_types
{
    /** The SMTP commands per stansards.
     *  The arrangement is related to smtp_commands struct.
     *  Do not modify or remove.
     */
    esp_mail_smtp_command_auth,
    esp_mail_smtp_command_login,
    esp_mail_smtp_command_helo,
    esp_mail_smtp_command_ehlo,
    esp_mail_smtp_command_quit,
    esp_mail_smtp_command_mail,
    esp_mail_smtp_command_rcpt,
    esp_mail_smtp_command_data,
    esp_mail_smtp_command_bdat,
    esp_mail_smtp_command_last,
    esp_mail_smtp_command_plain,
    esp_mail_smtp_command_from,
    esp_mail_smtp_command_to,
    esp_mail_smtp_command_notify,
    esp_mail_smtp_command_success,
    esp_mail_smtp_command_failure,
    esp_mail_smtp_command_delay,
    esp_mail_smtp_command_body,
    esp_mail_smtp_command_terminate,
    esp_mail_smtp_command_starttls,
    esp_mail_smtp_command_maxType
};

/* The SMTP server capability types enum (except for SASL) */
enum esp_mail_smtp_send_capability_types
{
    /** The server capability keywords per standard.
     *  The arrangement is related to smtp_send_capabilities struct.
     *  Do not modify or remove.
     */
    esp_mail_smtp_send_capability_binary_mime,
    esp_mail_smtp_send_capability_8bit_mime,
    esp_mail_smtp_send_capability_chunking,
    esp_mail_smtp_send_capability_utf8,
    esp_mail_smtp_send_capability_pipelining,
    esp_mail_smtp_send_capability_dsn,
    esp_mail_smtp_send_capability_esmtp,
    esp_mail_smtp_send_capability_maxType
};

#endif

#if defined(ENABLE_IMAP)

/* The IMAP server response types enum */
enum esp_mail_imap_response_types
{
    /** The IMAP response.
     *  The arrangement is related to imap_responses struct.
     *  Do not modify or remove.
     */
    esp_mail_imap_response_ok,
    esp_mail_imap_response_no,
    esp_mail_imap_response_bad,
    esp_mail_imap_response_list,
    esp_mail_imap_response_flags,
    esp_mail_imap_response_search,
    esp_mail_imap_response_fetch,
    esp_mail_imap_response_nil,
    esp_mail_imap_response_uid,
    esp_mail_imap_response_capability_untagged,
    esp_mail_imap_response_capability,
    esp_mail_imap_response_lsub,
    esp_mail_imap_response_quota,
    esp_mail_imap_response_quotaroot,
    esp_mail_imap_response_acl,
    esp_mail_imap_response_myrights,
    esp_mail_imap_response_namespace,
    esp_mail_imap_response_untagged,
    esp_mail_imap_response_exists,
    esp_mail_imap_response_expunge,
    esp_mail_imap_response_recent,
    esp_mail_imap_response_uidnext,
    esp_mail_imap_response_unseen,
    esp_mail_imap_response_id,
    esp_mail_imap_response_highest_modsec,
    esp_mail_imap_response_nomodsec,
    esp_mail_imap_response_permanent_flags,
    esp_mail_imap_response_uidvalidity,
    esp_mail_imap_response_maxType
};

/* The IMAP commands types enum */
enum esp_mail_imap_command_types
{
    /** The IMAP commands per standards.
     *  The arrangement is related to imap_commands struct.
     *  Do not modify or remove.
     */
    esp_mail_imap_command_starttls,
    esp_mail_imap_command_append,
    esp_mail_imap_command_capability,
    esp_mail_imap_command_authenticate,
    esp_mail_imap_command_plain,
    esp_mail_imap_command_uid,
    esp_mail_imap_command_fetch,
    esp_mail_imap_command_flags,
    esp_mail_imap_command_login,
    esp_mail_imap_command_list,
    esp_mail_imap_command_examine,
    esp_mail_imap_command_search,
    esp_mail_imap_command_logout,
    esp_mail_imap_command_body,
    esp_mail_imap_command_mime,
    esp_mail_imap_command_close,
    esp_mail_imap_command_exists,
    esp_mail_imap_command_peek,
    esp_mail_imap_command_text,
    esp_mail_imap_command_fields,
    esp_mail_imap_command_header,
    esp_mail_imap_command_new,
    esp_mail_imap_command_all,
    esp_mail_imap_command_select,
    esp_mail_imap_command_expunge,
    esp_mail_imap_command_create,
    esp_mail_imap_command_delete,
    esp_mail_imap_command_idle,
    esp_mail_imap_command_done,
    esp_mail_imap_command_recent,
    esp_mail_imap_command_unseen,
    esp_mail_imap_command_rename,
    esp_mail_imap_command_lsub,
    esp_mail_imap_command_subscribe,
    esp_mail_imap_command_unsubscribe,
    esp_mail_imap_command_silent,
    esp_mail_imap_command_move,
    esp_mail_imap_command_getquota,
    esp_mail_imap_command_setquota,
    esp_mail_imap_command_root,
    esp_mail_imap_command_get_acl,
    esp_mail_imap_command_set_acl,
    esp_mail_imap_command_delete_acl,
    esp_mail_imap_command_myrights,
    esp_mail_imap_command_namespace,
    esp_mail_imap_command_enable,
    esp_mail_imap_command_xoauth2,
    esp_mail_imap_command_store,
    esp_mail_imap_command_plus_flags,
    esp_mail_imap_command_minus_flags,
    esp_mail_imap_command_copy,
    esp_mail_imap_command_id,
    esp_mail_imap_command_unselect,
    esp_mail_imap_command_condstore,
    esp_mail_imap_command_noop,
    esp_mail_imap_command_unchangedsince,
    esp_mail_imap_command_changedsince,
    esp_mail_imap_command_modsec,
    esp_mail_imap_command_maxType
};

/* The IMAP server capability types enum (except for SASL) */
enum esp_mail_imap_read_capability_types
{
    /** The server capability keywords per standard.
     *  The arrangement is related imap_read_capabilities struct.
     *  Do not modify or remove.
     */

    esp_mail_imap_read_capability_imap4,
    esp_mail_imap_read_capability_imap4rev1,
    // rfc2177
    esp_mail_imap_read_capability_idle,
    esp_mail_imap_read_capability_literal_plus,
    esp_mail_imap_read_capability_literal_minus,
    esp_mail_imap_read_capability_multiappend,
    esp_mail_imap_read_capability_uidplus,
    // rfc4314
    esp_mail_imap_read_capability_acl,
    esp_mail_imap_read_capability_binary,
    esp_mail_imap_read_capability_logindisable,
    // rfc6851
    esp_mail_imap_read_capability_move,
    // rfc2087
    esp_mail_imap_read_capability_quota,
    // rfc2342
    esp_mail_imap_read_capability_namespace,
    // rfc5161
    esp_mail_imap_read_capability_enable,
    // rfc2971
    esp_mail_imap_read_capability_id,
    esp_mail_imap_read_capability_unselect,
    esp_mail_imap_read_capability_children,
    // rfc7162 (rfc4551 obsoleted)
    esp_mail_imap_read_capability_condstore,
    esp_mail_imap_read_capability_auto_caps,
    esp_mail_imap_read_capability_maxType
};

/* The Identification keys enum used for IMAP ID command */
enum esp_mail_imap_identification_keys
{
    /** The identification keys per standard.
     *  The arrangement is related imap_identification_keys struct.
     *  Do not modify or remove.
     */

    esp_mail_imap_identification_key_name,
    esp_mail_imap_identification_key_version,
    esp_mail_imap_identification_key_os,
    esp_mail_imap_identification_key_os_version,
    esp_mail_imap_identification_key_vendor,
    esp_mail_imap_identification_key_support_url,
    esp_mail_imap_identification_key_address,
    esp_mail_imap_identification_key_date,
    esp_mail_imap_identification_key_command,
    esp_mail_imap_identification_key_arguments,
    esp_mail_imap_identification_key_environment,
    esp_mail_imap_identification_key_maxType
};

#endif

/* The character encoding types enum used for decoding */
enum esp_mail_char_decoding_types
{
    /** Supported charactor encodings.
     *  The arrangement is related to char_decodings struct.
     *  Do not modify or remove.
     */
    esp_mail_char_decoding_utf8,
    esp_mail_char_decoding_iso_8859_1,
    esp_mail_char_decoding_iso_8859_11,
    esp_mail_char_decoding_tis_620,
    esp_mail_char_decoding_windows_874,
    esp_mail_char_decoding_maxType
};

/* The MIME multipart message types */
enum esp_mail_multipart_types
{
    /** MultiPart MIME.
     *  The arrangement is related to multipart_types struct.
     *  Do not modify or remove.
     */
    esp_mail_multipart_type_mixed,
    esp_mail_multipart_type_related,
    esp_mail_multipart_type_parallel,
    esp_mail_multipart_type_alternative,
    esp_mail_multipart_maxType
};

/* The rfc822 message header fileds types enum */
enum esp_mail_rfc822_header_field_types
{
    /** The rfc822 message header fields.
     *  The arrangement is related to rfc822_headers struct.
     *  Do not modify or remove.
     */
    esp_mail_rfc822_header_field_from,
    esp_mail_rfc822_header_field_sender,
    esp_mail_rfc822_header_field_to,
    esp_mail_rfc822_header_field_cc,
    esp_mail_rfc822_header_field_subject,
    esp_mail_rfc822_header_field_date,
    esp_mail_rfc822_header_field_msg_id,
    esp_mail_rfc822_header_field_return_path,
    esp_mail_rfc822_header_field_reply_to,
    esp_mail_rfc822_header_field_in_reply_to,
    esp_mail_rfc822_header_field_references,
    esp_mail_rfc822_header_field_comments,
    esp_mail_rfc822_header_field_keywords,
    esp_mail_rfc822_header_field_bcc,
    esp_mail_rfc822_header_field_flags,
    esp_mail_rfc822_header_field_maxType
};

/* The message header fileds types enum and its subproperties enum */
enum esp_mail_message_header_field_types
{
    /** Additional fields and props.
     *  Some are used for the library information data field name.
     *  The arrangement is related to message_headers struct.
     *  Do not modify or remove.
     */
    esp_mail_message_header_field_number,
    esp_mail_message_header_field_uid,
    esp_mail_message_header_field_accept_language,
    esp_mail_message_header_field_content_language,
    esp_mail_message_header_field_filename,
    esp_mail_message_header_field_name,
    esp_mail_message_header_field_size,
    esp_mail_message_header_field_mime,
    esp_mail_message_header_field_type,
    esp_mail_message_header_field_description,
    esp_mail_message_header_field_creation_date,
    esp_mail_message_header_field_x_priority,
    esp_mail_message_header_field_x_msmail_priority,
    esp_mail_message_header_field_importance,
    esp_mail_message_header_field_content_type,
    esp_mail_message_header_field_content_transfer_encoding,
    esp_mail_message_header_field_content_disposition,
    esp_mail_message_header_field_content_location,
    esp_mail_message_header_field_content_id,
    esp_mail_message_header_field_content_description,
    esp_mail_message_header_field_mime_version,
    esp_mail_message_header_field_charset,
    esp_mail_message_header_field_format,
    esp_mail_message_header_field_delsp,
    esp_mail_message_header_field_modification_date,
    esp_mail_message_header_field_maxType
};

/* The auth capability types enum which shared usage between SMTP and IMAP */
enum esp_mail_auth_capability_types
{
    /** The server capability keywords per standard.
     *  The arrangement is related to smtp_auth_capabilities and imap_auth_capabilities structs.
     *  Do not modify or remove.
     */

    esp_mail_auth_capability_plain,
    esp_mail_auth_capability_xoauth2,
    esp_mail_auth_capability_cram_md5,
    esp_mail_auth_capability_digest_md5,
    esp_mail_auth_capability_login,
    esp_mail_auth_capability_starttls,
    // imap rfc4959
    esp_mail_auth_capability_sasl_ir,

    esp_mail_auth_capability_maxType,
};

/* The smessage types enum */
enum esp_mail_message_type
{
    esp_mail_msg_type_none = 0,
    esp_mail_msg_type_plain = 1,
    esp_mail_msg_type_html = 2,
    esp_mail_msg_type_enriched = 1
};

/* The string mark types enum used in joinStringx functions */
enum esp_mail_string_mark_type
{
    esp_mail_string_mark_type_none,
    esp_mail_string_mark_type_double_quote,
    esp_mail_string_mark_type_angle_bracket,
    esp_mail_string_mark_type_round_bracket,
    esp_mail_string_mark_type_curly_bracket,
    esp_mail_string_mark_type_square_bracket
};

/* The debug TAG types enum */
enum esp_mail_debug_tag_type
{
    esp_mail_debug_tag_type_client,
    esp_mail_debug_tag_type_server,
    esp_mail_debug_tag_type_error,
    esp_mail_debug_tag_type_info,
    esp_mail_debug_tag_type_warning
};

/* The embed attachment types enum */
enum esp_mail_smtp_embed_message_type
{
    esp_mail_smtp_embed_message_type_attachment = 0,
    esp_mail_smtp_embed_message_type_inline
};

/* The attachment types enum */
enum esp_mail_attach_type
{
    esp_mail_att_type_none,
    esp_mail_att_type_attachment,
    esp_mail_att_type_inline
};

/* The debug levels for printing the debug information via Serial port */
enum esp_mail_debug_level
{
    esp_mail_debug_level_none = 0,
    esp_mail_debug_level_basic,
    esp_mail_debug_level_maintainer = 0xfff,
    esp_mail_debug_level_developer = esp_mail_debug_level_maintainer + 1
};

/* The content transfer encoding enum */
enum esp_mail_msg_xencoding
{
    esp_mail_msg_xencoding_none,
    /* rfc2045 section 2.7 */
    esp_mail_msg_xencoding_7bit,
    esp_mail_msg_xencoding_qp,
    esp_mail_msg_xencoding_base64,
    /* rfc2045 section 2.8 */
    esp_mail_msg_xencoding_8bit,
    /* rfc2045 section 2.9 */
    esp_mail_msg_xencoding_binary
};

/* The port protocols enum */
enum esp_mail_protocol
{
    esp_mail_protocol_plain_text,
    esp_mail_protocol_ssl,
    esp_mail_protocol_tls
};

/* The internal use strct */
struct esp_mail_internal_use_t
{
    esp_mail_msg_xencoding xencoding = esp_mail_msg_xencoding_none;
    MB_String cid;
};

/* The struct contains port number and its protocol */
struct port_function
{
    uint16_t port;
    esp_mail_protocol protocol;
};

/* The struct that contains the list of port_function and its size */
struct esp_mail_ports_functions
{
    friend class IMAPSession;
    friend class SMTPSession;
    friend class esp_mail_session_config_t;

    uint16_t size = 0;
    port_function *list = nullptr;
    bool use_internal_list = false;
};

/* The content transfer encoding type struct */
struct esp_mail_transfer_encoding_t
{
    /* The default 7-bit transfer encoding for US-ACII characters*/
    static constexpr const char *enc_7bit = "7bit";

    /* The quoted printable transfer encoding for non-US-ASCII characters*/
    static constexpr const char *enc_qp = "quoted-printable";

    /* The base64 encoded transfer encoding */
    static constexpr const char *enc_base64 = "base64";

    /* The 8-bit transfer encoding for extended-US-ASCII characters*/
    static constexpr const char *enc_8bit = "8bit";

    /* The binary transfer encoding for extended-US-ASCII characters with no line
     * length limit*/
    static constexpr const char *enc_binary = "binary";
};

/* The content disposition types strucr (rfc 2183) */
struct esp_mail_content_disposition_type_t
{
    /** if it is intended to be displayed automatically
     * upon display of the message.
     */
    static constexpr const char *inline_ = "inline";

    /** to indicate that they are separate from the main body
     * of the mail message, and that their display should not
     * be automatic, but contingent upon some further action of the user.
     */
    static constexpr const char *attachment = "attachment";
};

/* The file (SD/flash) message content struct used for providing SMTP message content from file  */
struct esp_mail_file_message_content_t
{
    /* The file path include its name */
    MB_String name;

    /** The type of file storages e.g.
     * esp_mail_file_storage_type_none,
     * esp_mail_file_storage_type_flash, and
     * esp_mail_file_storage_type_sd
     */
    esp_mail_file_storage_type type = esp_mail_file_storage_type_flash;
};

/* The blob or flash string message content struct used for providing SMTP message content from blob or flash string  */
struct esp_mail_blob_message_content_t
{
    /* The array of content in flash memory */
    const uint8_t *data = nullptr;

    /* The array size in bytes */
    size_t size = 0;
};

/* The option to embed this message content as a file */
struct esp_mail_smtp_embed_message_body_t
{
    /* Enable to send this message body as file */
    bool enable = false;

    /* The name of embedded file */
    MB_String filename;

    /** The embedded type
     * esp_mail_smtp_embed_message_type_attachment or 0
     * esp_mail_smtp_embed_message_type_inline or 1
     */
    esp_mail_smtp_embed_message_type type = esp_mail_smtp_embed_message_type_attachment;
};

/** This is the base struct for SMTP message.
 *  This is for input and later used as esp_mail_plain_body_t
 *  and esp_mail_html_body_t.
 *  Its members are similar to esp_mail_imap_plain_body_t
 *  and esp_mail_imap_html_body_t unless it contains
 *  MB_String object to hold the string.
 */
struct esp_mail_message_body_t
{
    /* The option to embed this message content as a file */
    struct esp_mail_smtp_embed_message_body_t embed;

    /* The PLAIN/HTML content of the message */
    MB_String content;

    const char *nonCopyContent = "";

    /* The blob that contins PLAIN/HTML content of the message */
    struct esp_mail_blob_message_content_t blob;

    /* The file that contins PLAIN/HTML content of the message */
    struct esp_mail_file_message_content_t file;

    /* The charset of the PLAIN/HTML content of the message */
    MB_String charSet = "UTF-8";

    /* The content type of message */
    MB_String content_type = mimeinfo[esp_mail_file_extension_txt].mimeType;

    /* The option to encode the content for data transfer */
    MB_String transfer_encoding = "7bit";

    /* The option to send the PLAIN text with wrapping */
    bool flowed = false;

    /* The internal usage data */
    struct esp_mail_internal_use_t _int;
};

/** The PLAIN text body details of the fetching message.
 *  This is for output and its members are similar to
 *  esp_mail_message_body_t unless there is no string object
 *  to hold string data unless the pointer to the const strings
 *  in IMAPSession object.
 */
struct esp_mail_imap_plain_body_t
{
    /* The option to embed this message content as a file */
    struct esp_mail_smtp_embed_message_body_t embed;

    /* The PLAIN text content of the message */
    const char *content = "";

    /* The blob that contins PLAIN text content of the message */
    struct esp_mail_blob_message_content_t blob;

    /* The file that contins PLAIN text content of the message */
    struct esp_mail_file_message_content_t file;

    /* The charset of the PLAIN text content of the message */
    const char *charSet = "UTF-8";

    /* The content type of message */
    const char *content_type = mimeinfo[esp_mail_file_extension_txt].mimeType;

    /* The option to encode the content for data transfer */
    const char *transfer_encoding = "7bit";

    /* The option to send the PLAIN text with wrapping */
    bool flowed = false;

    /* The internal usage data */
    struct esp_mail_internal_use_t _int;
};

/** The HTML body details of the fetching message.
 *  This is for output and its members are similar to
 *  esp_mail_message_body_t unless there is no string object
 *  to hold string data unless the pointer to the const strings
 *  in IMAPSession object.
 */
struct esp_mail_imap_html_body_t
{
    /* The option to embedded the content as a file */
    struct esp_mail_smtp_embed_message_body_t embed;

    /* The HTML content of the message */
    const char *content = "";

    /* The blob that contins HTML content of the message */
    struct esp_mail_blob_message_content_t blob;

    /* The file that contins HTML content of the message */
    struct esp_mail_file_message_content_t file;

    /* The charset of the HTML content of the message */
    const char *charSet = "UTF-8";

    /* The content type of message */
    const char *content_type = mimeinfo[esp_mail_file_extension_html].mimeType;

    /* The option to encode the content for data transfer */
    const char *transfer_encoding = "7bit";

    /* The internal usage data */
    struct esp_mail_internal_use_t _int;
};

/* The PLAIN text body details of the message */
typedef struct esp_mail_message_body_t esp_mail_plain_body_t;

/* The HTML text body details of the message */
typedef struct esp_mail_message_body_t esp_mail_html_body_t;

/* The attachment info struct used for output */
struct esp_mail_attachment_info_t
{
    const char *filename = "";
    const char *name = "";
    const char *creationDate = "";
    const char *mime = "";
    const char *description = "";
    esp_mail_attach_type type = esp_mail_att_type_none;
    size_t size;
};

#if defined(ENABLE_SMTP)

struct esp_mail_smtp_command_t
{
    char text[9];
};

struct esp_mail_timestamp_value_t
{
   /* The time format of timestamp to inject into subject or content as using in strftime C++ function */
    MB_String format;
    /* The tag that will be replaced with current timestamp */
    MB_String tag;
};

/** The SMTP commands per stansards.
 *  The arrangement is related to esp_mail_smtp_command_types enum.
 *  Do not modify or remove.
 */
const struct esp_mail_smtp_command_t smtp_commands[esp_mail_smtp_command_maxType] PROGMEM = {
    "AUTH",
    "LOGIN",
    "HELO",
    "EHLO",
    "QUIT",
    "MAIL",
    "RCPT",
    "DATA",
    "BDAT",
    "LAST",
    "PLAIN",
    "FROM:",
    "TO:",
    "NOTIFY",
    "SUCCESS",
    "FAILURE",
    "DELAY",
    "BODY",
    "\r\n.\r\n",
    "STARTTLS"};

struct esp_mail_smtp_commands_tokens
{
public:
    esp_mail_smtp_commands_tokens(bool pre)
    {
        preToken = pre;
    }
    MB_String operator[](size_t index)
    {
        MB_String s = preToken ? " " : smtp_commands[index].text;
        s += !preToken ? " " : smtp_commands[index].text;
        return s;
    }

private:
    bool preToken = false;
};

// The smtp commands with leading space.
static esp_mail_smtp_commands_tokens smtp_cmd_pre_tokens(true);

// The smtp commands with trailing space.
static esp_mail_smtp_commands_tokens smtp_cmd_post_tokens(false);

#endif

#if defined(ENABLE_IMAP)

struct esp_mail_imap_command_t
{
    char text[15];
};

/** The IMAP commands per standards.
 *  The arrangement is related to esp_mail_imap_command_types enum.
 *  Do not modify or remove.
 */
const struct esp_mail_imap_command_t imap_commands[esp_mail_imap_command_maxType] PROGMEM = {
    "STARTTLS",
    "APPEND",
    "CAPABILITY",
    "AUTHENTICATE",
    "PLAIN",
    "UID",
    "FETCH",
    "FLAGS",
    "LOGIN",
    "LIST",
    "EXAMINE",
    "SEARCH",
    "LOGOUT",
    "BODY",
    "MIME",
    "CLOSE",
    "EXISTS",
    "PEEK",
    "TEXT",
    "FIELDS",
    "HEADER",
    "NEW",
    "ALL",
    "SELECT",
    "EXPUNGE",
    "CREATE",
    "DELETE",
    "IDLE",
    "DONE",
    "RECENT",
    "UNSEEN",
    "RENAME",
    "LSUB",
    "SUBSCRIBE",
    "UNSUBSCRIBE",
    "SILENT",
    "MOVE",
    "GETQUOTA",
    "SETQUOTA",
    "ROOT",
    "GETACL",
    "SETACL",
    "DELETEACL",
    "MYRIGHTS",
    "NAMESPACE",
    "ENABLE",
    "XOAUTH2",
    "STORE",
    "+FLAGS",
    "-FLAGS",
    "COPY",
    "ID",
    "UNSELECT",
    "CONDSTORE",
    "NOOP",
    "UNCHANGEDSINCE",
    "CHANGEDSINCE",
    "MODSEC"};

struct esp_mail_imap_commands_tokens
{
public:
    esp_mail_imap_commands_tokens(bool pre)
    {
        preToken = pre;
    }
    MB_String operator[](size_t index)
    {
        MB_String s = preToken ? " " : imap_commands[index].text;
        s += !preToken ? " " : imap_commands[index].text;
        return s;
    }

private:
    bool preToken = false;
};

// The imap commands with leading space.
static esp_mail_imap_commands_tokens imap_cmd_pre_tokens(true);

// The imap commands with trailing space.
static esp_mail_imap_commands_tokens imap_cmd_post_tokens(false);

struct esp_mail_imap_response_t
{
    char text[18];
};

/** The IMAP response.
 *  The arrangement is related to esp_mail_imap_response_types enum.
 *  Do not modify or remove.
 */
const struct esp_mail_imap_response_t imap_responses[esp_mail_imap_response_maxType] PROGMEM = {
    // Tagged
    "OK ",
    "NO ",
    "BAD ",
    // Untagged
    "* LIST ",
    "* FLAGS ",
    "* SEARCH ",
    " FETCH ",
    " NIL ",
    " UID ",
    "* CAPABILITY ",
    "CAPABILITY ",
    "* LSUB ",
    "* QUOTA ",
    "* QUOTAROOT ",
    "* ACL ",
    "* MYRIGHTS ",
    "* NAMESPACE ",
    "* ",
    " EXISTS",
    " EXPUNGE",
    " RECENT",
    " [UIDNEXT ",
    " [UNSEEN ",
    "* ID ",
    " [HIGHESTMODSEQ ",
    " [NOMODSEQ]",
    " [PERMANENTFLAGS ",
    " [UIDVALIDITY "};

#endif

struct esp_mail_char_decoding_t
{
    char text[12];
};

/** Supported charactor encodings.
 *  The arrangement is related to esp_mail_char_decoding_types enum.
 *  Do not modify or remove.
 */
const struct esp_mail_char_decoding_t char_decodings[esp_mail_char_decoding_maxType] PROGMEM = {
    "utf-8",
    "iso-8859-1",
    "iso-8859-11",
    "tis-620",
    "windows-874"};

struct esp_mail_multipart_t
{
    char text[22];
};

/** MultiPart MIME.
 *  The arrangement is related to esp_mail_multipart_types enum.
 *  Do not modify or remove.
 */
const struct esp_mail_multipart_t multipart_types[esp_mail_multipart_maxType] PROGMEM = {
    "multipart/mixed",
    "multipart/related",
    "multipart/parallel",
    "multipart/alternative"};

struct esp_mail_rfc822_header_field_t
{
    char text[20];
    bool isNum;
    bool trim;
};

/** The rfc822 message header fields.
 *  The arrangement is related to esp_mail_rfc822_header_field_types enum.
 *  Do not modify or remove.
 */
const struct esp_mail_rfc822_header_field_t rfc822_headers[esp_mail_rfc822_header_field_maxType] PROGMEM = {
    {"From", false, true},
    {"Sender", false, true},
    {"To", false, true},
    {"Cc", false, true},
    {"Subject", false, false},
    {"Date", false, false},
    {"Message-ID", false, false},
    {"Return-Path", false, true},
    {"Reply-To", false, true},
    {"In-Reply-To", false, true},
    {"References", false, true},
    {"Comments", false, false},
    {"Keywords", false, false},
    {"Bcc", false, true},
    {"Flags", false, false}};

struct esp_mail_message_header_field_t
{
    char text[26];
};

/** Additional fields and props.
 *  The arrangement is related to esp_mail_message_header_field_types enum.
 *  Do not modify or remove.
 */
const struct esp_mail_message_header_field_t message_headers[esp_mail_message_header_field_maxType] PROGMEM = {
    "Number",
    "UID",
    "Accept-Language",
    "Content-Language",
    "Filename",
    "Name",
    "Size",
    "MIME",
    "Type",
    "Description",
    "Creation-Date",
    "X-Priority",
    "X-MSMail-Priority",
    "Importance",
    "Content-Type",
    "Content-transfer-encoding",
    "Content-Disposition",
    "Content-Location",
    "Content-ID",
    "Content-Description",
    "Mime-Version",
    "Charset",
    "format",
    "delsp",
    "Modification-Date"};

struct esp_mail_auth_capability_t
{
    char text[20];
};

#if defined(ENABLE_SMTP)

/** The server capability keywords per standard.
 *  The arrangement is related to esp_mail_auth_capability_types enum.
 *  Do not modify or remove.
 */
const struct esp_mail_auth_capability_t smtp_auth_capabilities[esp_mail_auth_capability_maxType] PROGMEM = {
    "PLAIN",
    "XOAUTH2",
    "CRAM-MD5",
    "DIGEST-MD5",
    "LOGIN",
    "STARTTLS",
    "DUMMY_AUTH" /* SASL-IR */};

struct esp_mail_smtp_auth_tokens
{
public:
    esp_mail_smtp_auth_tokens(bool pre)
    {
        preToken = pre;
    }
    MB_String operator[](size_t index)
    {
        MB_String s = preToken ? " " : smtp_auth_capabilities[index].text;
        s += !preToken ? " " : smtp_auth_capabilities[index].text;
        return s;
    }

private:
    bool preToken = false;
};

// The smtp auth capability with leading space.
static esp_mail_smtp_auth_tokens smtp_auth_cap_pre_tokens(true);
// The smtp auth capability with trailing space.
static esp_mail_smtp_auth_tokens smtp_auth_cap_post_tokens(false);

struct esp_mail_smtp_send_capability_t
{
    char text[15];
};

/** The server capability keywords per standard.
 *  The arrangement is related esp_mail_smtp_send_capability_types enum.
 *  Do not modify or remove.
 */
const struct esp_mail_smtp_send_capability_t smtp_send_capabilities[esp_mail_smtp_send_capability_maxType] PROGMEM = {
    "BINARYMIME",
    "8BITMIME",
    "CHUNKING",
    "SMTPUTF8",
    "PIPELINING",
    "DSN",
    "" /* ESMTP */};

struct esp_mail_smtp_send_tokens
{
public:
    esp_mail_smtp_send_tokens(bool pre)
    {
        preToken = pre;
    }
    MB_String operator[](size_t index)
    {
        MB_String s = preToken ? " " : smtp_send_capabilities[index].text;
        s += !preToken ? " " : smtp_send_capabilities[index].text;
        return s;
    }

private:
    bool preToken = false;
};

static esp_mail_smtp_send_tokens smtp_send_cap_pre_tokens(true);

#endif

#if defined(ENABLE_IMAP)

/** The server capability keywords per standard.
 *  The arrangement is related esp_mail_auth_capability_types enum.
 *  Do not modify or remove.
 */
const struct esp_mail_auth_capability_t imap_auth_capabilities[esp_mail_auth_capability_maxType] PROGMEM = {
    "AUTH=PLAIN",
    "AUTH=XOAUTH2",
    "CRAM-MD5",
    "DIGEST-MD5",
    "DUMMY_AUTH", /* Log in */
    "STARTTLS",
    "SASL-IR"};

struct esp_mail_imap_auth_tokens
{
public:
    esp_mail_imap_auth_tokens(bool pre)
    {
        preToken = pre;
    }
    MB_String operator[](size_t index)
    {
        MB_String s = preToken ? " " : imap_auth_capabilities[index].text;
        s += !preToken ? " " : imap_auth_capabilities[index].text;
        return s;
    }

private:
    bool preToken = false;
};

// The imap auth capability with leading space.
static esp_mail_imap_auth_tokens imap_auth_cap_pre_tokens(true);

struct esp_mail_imap_read_capability_t
{
    char text[15];
};

/** The server capability keywords per standard.
 *  The arrangement is related esp_mail_imap_read_capability_types enum.
 *  Do not modify or remove.
 */
const struct esp_mail_imap_read_capability_t imap_read_capabilities[esp_mail_imap_read_capability_maxType] PROGMEM = {
    "IMAP4",
    "IMAP4rev1",
    "IDLE",
    "LITERAL+",
    "LITERAL-",
    "MULTIAPPEND",
    "UIDPLUS",
    "ACL",
    "BINARY",
    "LOGINDISABLED",
    "MOVE",
    "QUOTA",
    "NAMESPACE",
    "ENABLE",
    "ID",
    "UNSELECT",
    "CHILDREN",
    "CONDSTORE"
    "" /* Auto cap */};

struct esp_mail_imap_read_tokens
{
public:
    esp_mail_imap_read_tokens(bool pre)
    {
        preToken = pre;
    }
    MB_String operator[](size_t index)
    {
        MB_String s = preToken ? " " : imap_read_capabilities[index].text;
        s += !preToken ? " " : imap_read_capabilities[index].text;
        return s;
    }

private:
    bool preToken = false;
};

// The imap auth capability with leading space.
static esp_mail_imap_read_tokens imap_read_cap_pre_tokens(true);

// The imap auth capability with trailing space.
static esp_mail_imap_read_tokens imap_read_cap_post_tokens(false);

struct esp_mail_imap_identification_key_t
{
    char text[15];
};

/** The identification keys per standard.
 *  The arrangement is related esp_mail_imap_identification_key_types enum.
 *  Do not modify or remove.
 */
const struct esp_mail_imap_identification_key_t imap_identification_keys[esp_mail_imap_identification_key_maxType] PROGMEM = {
    "name",
    "version",
    "os",
    "os-version",
    "vendor",
    "support-url",
    "address",
    "date",
    "command",
    "arguments",
    "environment"};

/* The IMAP ID data struct */
typedef struct esp_mail_imap_identity_t
{
    MB_String name;
    MB_String version;
    MB_String os;
    MB_String os_version;
    MB_String vendor;
    MB_String support_url;
    MB_String address;
    MB_String date;
    MB_String command;
    MB_String arguments;
    MB_String environment;

} IMAP_Identification;

#endif

#endif

#if defined(ENABLE_SMTP) || defined(ENABLE_IMAP)
/* The email address info [SMTP_Message]*/
struct esp_mail_address_info_t
{
    /* The name of Email author/sender */
    MB_String name;

    /* The Email address */
    MB_String email;
};
#endif

#if defined(ENABLE_SMTP)

/* The SMTP message notification types enum */
enum esp_mail_smtp_notify
{
    esp_mail_smtp_notify_never = 0,
    esp_mail_smtp_notify_success = 1,
    esp_mail_smtp_notify_failure = 2,
    esp_mail_smtp_notify_delay = 4
};

/* The SMTP status codes enum */
enum esp_mail_smtp_status_code
{
    esp_mail_smtp_status_code_0, // default

    /* Positive Completion */
    esp_mail_smtp_status_code_211 = 221, // System status, or system help reply
    esp_mail_smtp_status_code_214 = 214, // Help message(A response to the HELP command)
    esp_mail_smtp_status_code_220 = 220, //<domain> Service ready
    esp_mail_smtp_status_code_221 = 221, //<domain> Service closing transmission channel [RFC 2034]
    esp_mail_smtp_status_code_235 = 235, // 2.7.0 Authentication succeeded[RFC 4954]
    esp_mail_smtp_status_code_250 = 250, // Requested mail action okay, completed
    esp_mail_smtp_status_code_251 = 251, // User not local; will forward
    esp_mail_smtp_status_code_252 = 252, // Cannot verify the user, but it will
                                         // try to deliver the message anyway

    /* Positive Intermediate */
    esp_mail_smtp_status_code_334 = 334, //(Server challenge - the text part
                                         // contains the Base64 - encoded
                                         // challenge)[RFC 4954]
    esp_mail_smtp_status_code_354 = 354, // Start mail input

    /* Transient Negative Completion */
    /* "Transient Negative" means the error condition is temporary, and the action
     may be requested again.*/
    esp_mail_smtp_status_code_421 = 421, // Service is unavailable because the server is shutting down.
    esp_mail_smtp_status_code_432 = 432, // 4.7.12 A password transition is needed [RFC 4954]
    esp_mail_smtp_status_code_450 = 450, // Requested mail action not taken: mailbox unavailable (e.g.,
                                         // mailbox busy or temporarily blocked for policy reasons)
    esp_mail_smtp_status_code_451 = 451, // Requested action aborted : local error in processing
    // e.g.IMAP server unavailable[RFC 4468]
    esp_mail_smtp_status_code_452 = 452, // Requested action not taken : insufficient system storage
    esp_mail_smtp_status_code_454 = 454, // 4.7.0 Temporary authentication failure[RFC 4954]
    esp_mail_smtp_status_code_455 = 455, // Server unable to accommodate parameters

    /* Permanent Negative Completion */
    esp_mail_smtp_status_code_500 = 500, // Syntax error, command unrecognized
                                         // (This may include errors such as
                                         // command line too long)
    // e.g. Authentication Exchange line is too long [RFC 4954]
    esp_mail_smtp_status_code_501 = 501, // Syntax error in parameters or arguments
    // e.g. 5.5.2 Cannot Base64-decode Client responses [RFC 4954]
    // 5.7.0 Client initiated Authentication Exchange (only when the SASL
    // mechanism specified that client does not begin the authentication exchange)
    // [RFC 4954]
    esp_mail_smtp_status_code_502 = 502, // Command not implemented
    esp_mail_smtp_status_code_503 = 503, // Bad sequence of commands
    esp_mail_smtp_status_code_504 = 504, // Command parameter is not implemented
    // e.g. 5.5.4 Unrecognized authentication type [RFC 4954]
    esp_mail_smtp_status_code_521 = 521, // Server does not accept mail [RFC 7504]
    esp_mail_smtp_status_code_523 = 523, // Encryption Needed [RFC 5248]
    esp_mail_smtp_status_code_530 = 530, // 5.7.0 Authentication required [RFC 4954]
    esp_mail_smtp_status_code_534 = 534, // 5.7.9 Authentication mechanism is too weak [RFC 4954]
    esp_mail_smtp_status_code_535 = 535, // 5.7.8 Authentication credentials invalid [RFC 4954]
    esp_mail_smtp_status_code_538 = 538, // 5.7.11 Encryption required for
                                         // requested authentication mechanism[RFC
                                         // 4954]
    esp_mail_smtp_status_code_550 = 550, // Requested action not taken: mailbox unavailable (e.g., mailbox not
                                         // found, no access, or command rejected for policy reasons)
    esp_mail_smtp_status_code_551 = 551, // User not local; please try <forward-path>
    esp_mail_smtp_status_code_552 = 552, // Requested mail action aborted: exceeded storage allocation
    esp_mail_smtp_status_code_553 = 553, // Requested action not taken: mailbox name not allowed
    esp_mail_smtp_status_code_554 = 554, // Transaction has failed (Or, in the
                                         // case of a connection-opening response,
                                         // "No SMTP service here")
    // e.g. 5.3.4 Message too big for system [RFC 4468]
    esp_mail_smtp_status_code_556 = 556, // Domain does not accept mail[RFC 7504]
};

/* The SMTP ports enum */
enum esp_mail_smtp_port
{
    esp_mail_smtp_port_25 = 25,   // PLAIN/TLS with STARTTLS
    esp_mail_smtp_port_465 = 465, // SSL
    esp_mail_smtp_port_587 = 587, // TLS with STARTTLS
};

const struct port_function smtp_ports[3] = {
    {esp_mail_smtp_port_25, esp_mail_protocol_plain_text},
    {esp_mail_smtp_port_465, esp_mail_protocol_ssl},
    {esp_mail_smtp_port_587, esp_mail_protocol_tls}};

/* The SMTP message response [SMTP_Message] */
struct esp_mail_smtp_msg_response_t
{
    /* The author Email address to reply */
    MB_String reply_to;

    /* The sender Email address to return the message */
    MB_String return_path;

    /** The Delivery Status Notifications e.g. esp_mail_smtp_notify_never,
     * esp_mail_smtp_notify_success,
     * esp_mail_smtp_notify_failure, and
     * esp_mail_smtp_notify_delay
     */
    int notify = esp_mail_smtp_notify::esp_mail_smtp_notify_never;
};

/* The SMTP enable option [SMTP_Message] */
struct esp_mail_smtp_enable_option_t
{
    /* Enable chunk data sending for large message */
    bool chunking = false;
};

/* The SMTP blob data attachment data [Session_Config] */
struct esp_mail_attach_blob_t
{
    /* BLOB data (flash or ram) */
    const uint8_t *data = nullptr;

    /* BLOB data size in byte */
    size_t size = 0;
};

/* The SMTP file attachment data [Session_Config] */
struct esp_mail_attach_file_t
{
    MB_String path;
    /** The file storage type e.g. esp_mail_file_storage_type_none,
     * esp_mail_file_storage_type_flash, and
     * esp_mail_file_storage_type_sd
     */
    esp_mail_file_storage_type storage_type = esp_mail_file_storage_type_none;
};

/* The SMTP attachment decription [Session_Config] */
struct esp_mail_attach_descr_t
{
    /* The name of attachment */
    MB_String name;

    /* The attachment file name */
    MB_String filename;

    /* The MIME type of attachment */
    MB_String mime;

    /* The transfer encoding of attachment e.g. base64 */
    MB_String transfer_encoding = "base64";

    /* The content encoding of attachment e.g. base64 */
    MB_String content_encoding;

    /* The content id of attachment file */
    MB_String content_id;

    /* The description of attachment file */
    MB_String description;
};

/* Used internally in esp_mail_attachment_t */
struct esp_mail_attach_internal_t
{
    esp_mail_attach_type att_type = esp_mail_att_type_attachment;
    int index = 0;
    int msg_uid = 0;
    bool flash_blob = false;
    esp_mail_msg_xencoding xencoding = esp_mail_msg_xencoding_none;
    bool parallel = false;
    MB_String cid;
};

/* The struct used as SMTP_Attachment for SMTP and ESP_Mail_Attachment for IMAP */
struct esp_mail_attachment_t
{
    /* The attachment description */
    struct esp_mail_attach_descr_t descr;

    /* The BLOB data config */
    struct esp_mail_attach_blob_t blob;

    /* The file data config */
    struct esp_mail_attach_file_t file;

    /* reserved for internal usage */
    struct esp_mail_attach_internal_t _int;
};

/* The struct used as SMTP_Result */
struct esp_mail_smtp_send_status_t
{
    /* The status of the message */
    bool completed = false;

    /* The primary recipient mailbox of the message */
    MB_String recipients;

    /* The topic of the message */
    MB_String subject;

    /* The timestamp of the message */
    uint32_t timestamp = 0;
};

/* Used internally for SMTPSession */
struct esp_mail_smtp_msg_type_t
{
    bool rfc822 = false;
    int rfc822Idx = 0;
};

/* Used internally for holding base64 data sources */
struct esp_mail_smtp_send_base64_data_info_t
{
    esp_mail_file_storage_type storageType = esp_mail_file_storage_type_none;
    const char *filename = "";
    const uint8_t *rawPtr = nullptr;
    bool flashMem = false;
    size_t size = 0;
    size_t dataIndex = 0;
};

/* SMTP commands types enum */
enum esp_mail_smtp_command
{
    esp_mail_smtp_cmd_undefined,
    esp_mail_smtp_cmd_initial_state,
    esp_mail_smtp_cmd_greeting,
    esp_mail_smtp_cmd_start_tls,
    esp_mail_smtp_cmd_login_user,
    esp_mail_smtp_cmd_auth_plain,
    esp_mail_smtp_cmd_auth_login,
    esp_mail_smtp_cmd_auth_xoauth2,
    esp_mail_smtp_cmd_login_psw,
    esp_mail_smtp_cmd_send_header_sender,
    esp_mail_smtp_cmd_send_header_recipient,
    esp_mail_smtp_cmd_send_body,
    esp_mail_smtp_cmd_chunk_termination,
    esp_mail_smtp_cmd_logout,
    esp_mail_smtp_cmd_custom
};

/* SMTP message priority level enum */
enum esp_mail_smtp_priority
{
    esp_mail_smtp_priority_high = 1,
    esp_mail_smtp_priority_normal = 3,
    esp_mail_smtp_priority_low = 5,
};

/* SMTP response data */
typedef struct esp_mail_smtp_response_status_t
{
    /* The SMTP server response status code */
    int statusCode = 0;

    /* error code */
    int errorCode = 0;
    int id = -1;
    MB_String text;
} SMTP_Response;

#endif

#if defined(ENABLE_IMAP)

enum esp_mail_imap_msg_num_type
{
    esp_mail_imap_msg_num_type_undefined,
    esp_mail_imap_msg_num_type_uid,
    esp_mail_imap_msg_num_type_number
};

enum esp_mail_imap_store_flag_type
{
    esp_mail_imap_store_flag_type_set,
    esp_mail_imap_store_flag_type_add,
    esp_mail_imap_store_flag_type_remove
};

enum esp_mail_char_decoding_scheme
{
    esp_mail_char_decoding_scheme_default = -1,
    esp_mail_char_decoding_scheme_utf_8,
    esp_mail_char_decoding_scheme_iso8859_1,
    esp_mail_char_decoding_scheme_iso8859_11,
    esp_mail_char_decoding_scheme_tis_620,
    esp_mail_char_decoding_scheme_windows_874
};

enum esp_mail_imap_rights_type_t
{
    /* a - administer (perform SETACL/DELETEACL/GETACL/LISTRIGHTS) */
    esp_mail_imap_rights_administer = 'a' - 'a',
    /* c - RFC2086 (obsoleted) create (CREATE new sub-mailboxes in any implementation-defined hierarchy) */
    esp_mail_imap_rights_create_c = 'c' - 'a',
    /* d - RFC2086 (obsoleted) delete (STORE DELETED flag, perform EXPUNGE) */
    esp_mail_imap_rights_delete_d = 'd' - 'a',
    /* e - perform EXPUNGE and expunge as a part of CLOSE */
    esp_mail_imap_rights_expunge = 'e' - 'a',
    /* i - insert (perform APPEND, COPY into mailbox) */
    esp_mail_imap_rights_insert = 'i' - 'a',
    /* k - RFC4314 create mailboxes (CREATE new sub-mailboxes in any implementation-defined hierarchy, parent mailbox for the new mailbox name in RENAME)*/
    esp_mail_imap_rights_create = 'k' - 'a',
    /* l - lookup (mailbox is visible to LIST/LSUB commands, SUBSCRIBE mailbox) */
    esp_mail_imap_rights_lookup = 'l' - 'a',
    /* p - post (send mail to submission address for mailbox, not enforced by IMAP4 itself) */
    esp_mail_imap_rights_post = 'p' - 'a',
    /* r - read (SELECT the mailbox, perform STATUS) */
    esp_mail_imap_rights_read = 'r' - 'a',
    /* s - keep seen/unseen information across sessions (set or clear \SEEN flag via STORE, also set \SEEN during APPEND/COPY/ FETCH BODY[...]) */
    esp_mail_imap_rights_seen = 's' - 'a',
    /* t - RFC4314 delete messages (set or clear \DELETED flag via STORE, set \DELETED flag during APPEND/COPY) */
    esp_mail_imap_rights_delete_message = 't' - 'a',
    /* w - write (set or clear flags other than \SEEN and \DELETED via STORE, also set them during APPEND/COPY) */
    esp_mail_imap_rights_write = 'w' - 'a',
    /* x - RFC4314 delete mailbox (DELETE mailbox, old mailbox name in RENAME) */
    esp_mail_imap_rights_delete_mailbox = 'x' - 'a',

    esp_mail_imap_rights_maxType = esp_mail_imap_rights_delete_mailbox + 1
};

enum esp_mail_imap_port
{
    esp_mail_imap_port_143 = 143, // PLAIN/TLS with STARTTLS
    esp_mail_imap_port_993 = 993, // SSL
};

const struct port_function imap_ports[2] = {
    {esp_mail_imap_port_143, esp_mail_protocol_plain_text},
    {esp_mail_imap_port_993, esp_mail_protocol_ssl}};

enum esp_mail_imap_auth_mode
{
    esp_mail_imap_mode_examine,
    esp_mail_imap_mode_select
};

enum esp_mail_imap_response_status
{
    esp_mail_imap_resp_unknown,
    esp_mail_imap_resp_ok,
    esp_mail_imap_resp_no,
    esp_mail_imap_resp_bad
};

enum esp_mail_imap_polling_status_type
{
    imap_polling_status_type_undefined,
    imap_polling_status_type_new_message,
    imap_polling_status_type_remove_message,
    imap_polling_status_type_fetch_message
};

enum esp_mail_imap_header_state
{
    esp_mail_imap_state_content_type = esp_mail_rfc822_header_field_maxType,
    esp_mail_imap_state_content_transfer_encoding,
    esp_mail_imap_state_accept_language,
    esp_mail_imap_state_content_language,
    esp_mail_imap_state_char_set,
    esp_mail_imap_state_boundary
};

enum esp_mail_imap_command
{
    esp_mail_imap_cmd_capability,
    esp_mail_imap_cmd_starttls,
    esp_mail_imap_cmd_sasl_login,
    esp_mail_imap_cmd_sasl_auth_plain,
    esp_mail_imap_cmd_sasl_auth_oauth,
    esp_mail_imap_cmd_list,
    esp_mail_imap_cmd_lsub,
    esp_mail_imap_cmd_subscribe,
    esp_mail_imap_cmd_unsubscribe,
    esp_mail_imap_cmd_select,
    esp_mail_imap_cmd_examine,
    esp_mail_imap_cmd_close,
    esp_mail_imap_cmd_status,
    esp_mail_imap_cmd_search,
    esp_mail_imap_cmd_fetch_body_header,
    esp_mail_imap_cmd_fetch_body_mime,
    esp_mail_imap_cmd_fetch_body_text,
    esp_mail_imap_cmd_fetch_body_attachment,
    esp_mail_imap_cmd_fetch_body_inline,
    esp_mail_imap_cmd_fetch_sequence_set,
    esp_mail_imap_cmd_logout,
    esp_mail_imap_cmd_store,
    esp_mail_imap_cmd_move,
    esp_mail_imap_cmd_get_quota,
    esp_mail_imap_cmd_set_quota,
    esp_mail_imap_cmd_get_quota_root,
    esp_mail_imap_cmd_get_acl,
    esp_mail_imap_cmd_set_acl,
    esp_mail_imap_cmd_delete_acl,
    esp_mail_imap_cmd_my_rights,
    esp_mail_imap_cmd_namespace,
    esp_mail_imap_cmd_expunge,
    esp_mail_imap_cmd_create,
    esp_mail_imap_cmd_rename,
    esp_mail_imap_cmd_delete,
    esp_mail_imap_cmd_idle,
    esp_mail_imap_cmd_done,
    esp_mail_imap_cmd_get_uid,
    esp_mail_imap_cmd_get_flags,
    esp_mail_imap_cmd_append,
    esp_mail_imap_cmd_append_last,
    esp_mail_imap_cmd_enable,
    esp_mail_imap_cmd_id,
    esp_mail_imap_cmd_unselect,
    esp_mail_imap_cmd_noop,
    esp_mail_imap_cmd_copy,
    esp_mail_imap_cmd_custom
};

enum esp_mail_imap_mime_fetch_type
{
    esp_mail_imap_mime_fetch_type_part,
    esp_mail_imap_mime_fetch_type_sub_part1,
    esp_mail_imap_mime_fetch_type_sub_part2
};

enum esp_mail_imap_header_type
{
    esp_mail_imap_header_from,
    esp_mail_imap_header_to,
    esp_mail_imap_header_cc,
    esp_mail_imap_header_subject,
    esp_mail_imap_header_date,
    esp_mail_imap_header_msg_id,
    esp_mail_imap_header_cont_lang,
    esp_mail_imap_header_accept_lang
};

enum esp_mail_imap_multipart_sub_type
{
    esp_mail_imap_multipart_sub_type_none = 0,
    esp_mail_imap_multipart_sub_type_mixed,
    esp_mail_imap_multipart_sub_type_alternative,
    esp_mail_imap_multipart_sub_type_parallel,
    esp_mail_imap_multipart_sub_type_digest,
    esp_mail_imap_multipart_sub_type_related,
    esp_mail_imap_multipart_sub_type_report,
};

enum esp_mail_imap_message_sub_type
{
    esp_mail_imap_message_sub_type_none = 0,
    esp_mail_imap_message_sub_type_rfc822,
    esp_mail_imap_message_sub_type_delivery_status,
    esp_mail_imap_message_sub_type_partial,
    esp_mail_imap_message_sub_type_external_body,
};

typedef struct esp_mail_imap_response_status_t
{
    // No IMAP server response status code (statusCode), server returns OK, NO and BAD response instead

    /*  error code */
    int errorCode = 0;
    MB_String tag;
    MB_String text;
    MB_String status;
    bool completed = false;

public:
    void clear(bool clearTag = true)
    {
        if (clearTag)
            tag.clear();
        status.clear();
        text.clear();
        completed = false;
    }
} IMAP_Response;

struct esp_mail_imap_msg_num_t
{
    esp_mail_imap_msg_num_type type = esp_mail_imap_msg_num_type_undefined;
    uint32_t value = 0;
};

__attribute__((used)) struct
{
    bool operator()(struct esp_mail_imap_msg_num_t a, struct esp_mail_imap_msg_num_t b) const { return a.value > b.value; }
} compareMore;

struct esp_mail_imap_rfc822_msg_header_item_t
{
    MB_String header_items[esp_mail_rfc822_header_field_maxType];
};

/* IMAP quota root info */
typedef struct esp_mail_imap_quota_root_info_t
{
    /* The quota root */
    MB_String quota_root;

    /* The resource name e.g. STORAGE and MESSAGE */
    MB_String name;

    /* The resource usage in kilo octets */
    size_t usage = 0;

    /* The resource limit in kilo octets */
    size_t limit = 0;

} IMAP_Quota_Root_Info;

/* IMAP namespace info */
typedef struct esp_mail_imap_namespace_info_t
{
    /* The leading prefix */
    MB_String prefix;

    /* The hierarchy delimiter */
    MB_String delimiter;

} IMAP_Namespace_Info;

typedef struct esp_mail_imap_rights_info_t
{
    MB_String identifier;
    bool rights[esp_mail_imap_rights_maxType];

} IMAP_Rights_Info;

/* descrete media types (rfc 2046) */
struct esp_mail_imap_descrete_media_type_t
{
    /** textual information with subtypes
     * "plain"
     * "enriched" (rfc 1896 revised from richtext in rfc 1341)
     *
     * unrecognized subtypes and charset should be interpreted as
     * application/octet-stream
     *
     * parameters:
     * "charset" (rfc 2045) default is us-ascii
     * for character set includes 8-bit characters
     * and such characters are used in the body, Content-Transfer-Encoding
     * header field and a corresponding encoding on the data are required
     *
     * ISO-8859-X where "X" is to be replaced, as
     * necessary, for the parts of ISO-8859 [ISO-8859].
     */
    static constexpr const char *text = "text";

    /** image data with subtypes (rfc 2048)
     * "jpeg"
     * "gif"
     *
     * unrecognized subtypes should be interpreted as application/octet-stream
     */
    static constexpr const char *image = "image";

    /** audio data with initial subtype
     * "baic" -- for single channel audio encoded using 8bit ISDN mu-law [PCM]
     * at a sample rate of 8000 Hz.
     *
     * Unrecognized subtypes of "audio" should at a miniumum be treated as
     * "application/octet-stream"
     */
    static constexpr const char *audio = "audio";

    /** video data with initial subtype
     * "mpeg"
     *
     * Unrecognized subtypes of "video" should at a minumum be treated as
     * "application/octet-stream"
     */
    static constexpr const char *video = "video";

    /** some other kind of data, typically either
     * uninterpreted binary data or information to be
     * processed by an application with subtypes
     *
     * "octet-stream" -- uninterpreted binary data
     * "PostScript" -- for the transport of PostScript material
     *
     * Other expected uses include spreadsheets, data for mail-based
     * scheduling systems, and languages for "active" (computational)
     * messaging, and word processing formats that are not directly readable.
     *
     * The octet-stream subtype parameters:
     * TYPE, PADDING, NAME
     */
    static constexpr const char *application = "application";
};

/** composite media types (rfc 2046)
 *
 * As stated in the definition of the Content-Transfer-Encoding field
 * [RFC 2045], no encoding other than "7bit", "8bit", or "binary" is
 * permitted for entities of type "multipart".  The "multipart" boundary
 * delimiters and header fields are always represented as 7bit US-ASCII
 * in any case (though the header fields may encode non-US-ASCII header
 * text as per RFC 2047) and data within the body parts can be encoded
 * on a part-by-part basis, with Content-Transfer-Encoding fields for
 * each appropriate body part.
 */
struct esp_mail_imap_composite_media_type_t
{
    /** data consisting of multiple entities of independent data types
     * The Content-Type field for multipart entities requires one parameter,
     * "boundary".
     * The boundary delimiter line is then defined as a line
     * consisting entirely of two hyphen characters ("-", decimal value 45)
     * followed by the boundary parameter value from the Content-Type header
     * field, optional linear whitespace, and a terminating CRLF.
     *
     * NOTE: The CRLF preceding the boundary delimiter line is conceptually
     * attached to the boundary so that it is possible to have a part that
     * does not end with a CRLF (line  break).  Body parts that must be
     * considered to end with line breaks, therefore, must have two CRLFs
     * preceding the boundary delimiter line, the first of which is part of
     * the preceding body part, and the second of which is part of the
     * encapsulation boundary.
     *
     * Boundary delimiters must not appear within the encapsulated material,
     * and must be no longer than 70 characters, not counting the two
     * leading hyphens.
     *
     * The boundary delimiter line following the last body part is a
     * distinguished delimiter that indicates that no further body parts
     * will follow.  Such a delimiter line is identical to the previous
     * delimiter lines, with the addition of two more hyphens after the
     * boundary parameter value.
     *
     * See rfc2049 Appendix A for a Complex Multipart Example
     */
    static constexpr const char *multipart = "multipart";

    /* an encapsulated message */
    static constexpr const char *message = "message";
};

typedef struct esp_mail_imap_mime_data_stream_info_t
{
    /* message UID */
    uint32_t uid = 0;

    /* content type */
    const char *type = "";

    /* disposition */
    const char *disposition = "";

    /* content character set */
    const char *charSet = "";

    /* text content flowed format parameter */
    bool flowed = false;

    /* text content format DelSp parameter */
    bool delsp = false;

    /* content transfer encoding  */
    const char *transfer_encoding = "";

    /* content ID */
    const char *cid = "";

    /* content description */
    const char *description = "";

    /* content file name */
    const char *filename = "";

    /* content name  */
    const char *name = "";

    /* creation date */
    const char *date = "";

    /* content size */
    size_t size = 0;

    /* octet size */
    size_t octet_size = 0;

    /* current octet count */
    int octet_count = 0;

    /* data size */
    size_t data_size = 0;

    /* data buffer */
    void *data = NULL;

    bool isFirstData = false;

    bool isLastData = false;

} MIME_Data_Stream_Info;

typedef struct esp_mail_imap_decoding_info
{
    enum message_part_type
    {
        message_part_type_header,
        message_part_type_text
    };

    /* The character set of the string to decode */
    const char *charset = "";

    /* The string to decode */
    const char *data = "";

    /* The decoded string to be processed */
    MB_String decodedString;

    /** The type of data that currently processed
     *  0 or IMAP_Decoding_Info::message_part_type_header
     *  1 or IMAP_Decoding_Info::message_part_type_text
     */
    message_part_type type = message_part_type_header;

} IMAP_Decoding_Info;

struct esp_mail_imap_media_text_sub_type_t
{
    static constexpr const char *plain = "plain";
    static constexpr const char *enriched = "enriched";
    static constexpr const char *html = "html";
};

/* multipart sub types */
struct esp_mail_imap_multipart_sub_type_t
{
    /* a generic mixed set of parts */
    static constexpr const char *mixed = "mixed";

    /* the same data in multiple formats */
    static constexpr const char *alternative = "alternative";

    /* parts intended to be viewed simultaneously */
    static constexpr const char *parallel = "parallel";

    /* multipart entities in which each part has a default type of
     * "message/rfc822" */
    static constexpr const char *digest = "digest";

    /* for compound objects consisting of several inter-related body parts (rfc
     * 2387) */
    static constexpr const char *related = "related";

    /* rfc 3462 */
    static constexpr const char *report = "report";
};

/* message body sub types */
struct esp_mail_imap_message_sub_type_t
{
    /* body contains  an encapsulated message, with the syntax of an RFC 822
     * message. */
    static constexpr const char *rfc822 = "rfc822";

    /* to allow large objects to be delivered as several separate pieces of mail
     */
    static constexpr const char *Partial = "Partial";

    /* the actual body data are not included, but merely referenced */
    static constexpr const char *External_Body = "External-Body";

    static constexpr const char *delivery_status = "delivery-status";
};

/* IMAP polling status */
typedef struct esp_mail_imap_polling_status_t
{
    /** The type of status e.g. imap_polling_status_type_undefined, imap_polling_status_type_new_message,
     * imap_polling_status_type_fetch_message and imap_polling_status_type_remove_message.
     */
    esp_mail_imap_polling_status_type type = imap_polling_status_type_undefined;

    /** Message number or order from the total number of message that added, fetched or deleted.
     */
    size_t messageNum = 0;

    /** Argument of commands e.g. FETCH
     */
    MB_String argument;
} IMAP_Polling_Status;

struct esp_mail_message_part_info_t
{
    enum content_header_field
    {
        content_header_field_none,
        content_header_field_type,
        content_header_field_description,
        content_header_field_id,
        content_header_field_disposition,
        content_header_field_transfer_enc,
        content_header_field_ext

    };

    int octetLen = 0;
    int octetCount = 0;
    int attach_data_size = 0;
    int textLen = 0;
    bool sizeProp = false;
    int nestedLevel = 0;

    // pointer to the MB_String for storing multi-line header field content.
    uint32_t stringPtr = 0;
    esp_mail_char_decoding_scheme stringEnc = esp_mail_char_decoding_scheme_default;

    content_header_field cur_content_hdr = content_header_field_none;

    MB_String partNumStr;
    MB_String partNumFetchStr;
    MB_String text;
    MB_String filename;
    MB_String CID;
    MB_String type;
    MB_String save_path;
    MB_String name;
    MB_String content_disposition;
    MB_String content_description;
    MB_String content_type;
    MB_String descr;
    MB_String content_transfer_encoding;
    MB_String creation_date;
    MB_String modification_date;
    MB_String charset;
    MB_String download_error;
    esp_mail_attach_type attach_type = esp_mail_att_type_none;
    esp_mail_message_type msg_type = esp_mail_msg_type_none;
    bool file_open_write = false;
    bool multipart = false;
    bool is_firmware_file = false;
    bool save_to_file = true;
    size_t firmware_downloaded_byte = 0;
    esp_mail_imap_multipart_sub_type multipart_sub_type = esp_mail_imap_multipart_sub_type_none;
    esp_mail_imap_message_sub_type message_sub_type = esp_mail_imap_message_sub_type_none;
    bool rfc822_part = false;
    int rfc822_msg_Idx = 0;
    struct esp_mail_imap_rfc822_msg_header_item_t rfc822_header;
    bool error = false;
    bool plain_flowed = false;
    bool plain_delsp = false;
    esp_mail_msg_xencoding xencoding = esp_mail_msg_xencoding_none;
};

struct esp_mail_message_header_t
{
    int header_data_len = 0;

    struct esp_mail_imap_rfc822_msg_header_item_t header_fields;

    MB_String content_type;
    MB_String content_transfer_encoding;
    uint32_t message_uid = 0;
    uint32_t message_no = 0;
    MB_String boundary;
    MB_String accept_language;
    MB_String content_language;
    MB_String char_set;
    bool multipart = false;
    bool rfc822_part = false;
    bool hasAttachment = false;
    int rfc822Idx = 0;
    MB_String partNumStr;

    esp_mail_imap_multipart_sub_type multipart_sub_type = esp_mail_imap_multipart_sub_type_none;
    esp_mail_imap_message_sub_type message_sub_type = esp_mail_imap_message_sub_type_none;
    MB_String msgID;
    MB_String flags;
    MB_String error_msg;
    bool error = false;
    _vectorImpl<struct esp_mail_message_part_info_t> part_headers;
    int attachment_count = 0;
    int sd_alias_file_count = 0;
    int total_download_size = 0;
    int downloaded_size = 0;
    int total_attach_data_size = 0;
    int downloaded_bytes = 0;
    int message_data_count = 0;
};

/* Internal use */
struct esp_mail_folder_info_t
{
    MB_String name;
    MB_String attributes;
    MB_String delimiter;
};

struct esp_mail_folder_info_item_t
{
    /* The name of folder */
    const char *name = "";

    /* The attributes of folder */
    const char *attributes = "";

    /* The delimeter of folder */
    const char *delimiter = "";
};

struct esp_mail_imap_download_config_t
{
    /* To download the PLAIN text content of the message */
    bool text = false;

    /* To download the HTML content of the message */
    bool html = false;

    /* To download the attachments of the message */
    bool attachment = false;

    /* To download the inline image of the message */
    bool inlineImg = false;

    /* To download the rfc822 mesages in the message */
    bool rfc822 = false;

    /* To download the message header */
    bool header = false;
};

struct esp_mail_imap_enable_config_t
{
    /* To store the PLAIN text of the message in the IMAPSession */
    bool text = false;

    /* To store the HTML of the message in the IMAPSession */
    bool html = false;

    /* To store the rfc822 messages in the IMAPSession */
    bool rfc822 = false;

    /* To enable the download status via the serial port */
    bool download_status = false;

    /* To sort the message UID of the search result in descending order */
    bool recent_sort = false;

    /* To allow case sesitive in header parsing */
    bool header_case_sensitive = false;
};

struct esp_mail_imap_limit_config_t
{
    /* The maximum messages from the search result */
    size_t search = 10;

    /* The maximum messages from the sequence set fetching result */
    size_t fetch = 30;

    /** The maximum size of the memory buffer to store the message content.
     * This is only limit for data to be stored in the IMAPSession.
     */
    size_t msg_size = 1024;

    /* The maximum size of each attachment to download */
    size_t attachment_size = 1024 * 1024 * 5;

    /* The IMAP idle timeout in ms (1 min to 29 min). Default is 8 min */
    size_t imap_idle_timeout = 8 * 60 * 1000;

    /** The IMAP idle host check interval in ms (30 sec to imap_idle_timeout)
     * for internet availability checking to ensure the connection is active.
     * Default is 1 min.
     */
    size_t imap_idle_host_check_interval = 60 * 1000;
};

struct esp_mail_imap_storage_config_t
{
    /* The path to save the downloaded file */
    MB_String saved_path;

    /** The type of file storages e.g.
     * esp_mail_file_storage_type_none,
     * esp_mail_file_storage_type_flash, and
     * esp_mail_file_storage_type_sd
     */
    esp_mail_file_storage_type type = esp_mail_file_storage_type_flash;
};

struct esp_mail_imap_search_config_t
{
    /* The search criteria */
    MB_String criteria;

    /* The option to search the unseen message */
    bool unseen_msg = false;
};

struct esp_mail_imap_sequence_set_t
{
    /* The sequence set string i.g., unique identifier (UID) or message sequence number or ranges of UID or sequence number */
    MB_String string;

    /* The option for sequenceSet whether it is UID or message sequence number */
    bool UID = false;

    /* The option for header only fetching */
    bool headerOnly = false;
};

struct esp_mail_imap_fetch_config_t
{
    /* The UID of message to fetch */
    MB_String uid;

    /* The message sequence number to fetch */
    MB_String number;

    /* The sequence set options */
    esp_mail_imap_sequence_set_t sequence_set;

    /* Set the message flag as seen */
    bool set_seen = false;

    /* The int32_t option for CHANGESINCE conditional test. */
    int32_t modsequence = 0;

    /* The config to fetch only the header */
    bool headerOnly = false;
};

struct esp_mail_imap_firmware_config_t
{
    /* Update firmware using message attachments if one of its filename matches. */
    MB_String attach_filename;

    /* Save firmware file */
    bool save_to_file = false;
};

struct esp_mail_imap_data_config_t
{
    /* The config for fetching */
    struct esp_mail_imap_fetch_config_t fetch;

    /* The config for search */
    struct esp_mail_imap_search_config_t search;

    /* The config about the limits */
    struct esp_mail_imap_limit_config_t limit;

    /* The config to enable the features */
    struct esp_mail_imap_enable_config_t enable;

    /* The config about downloads */
    struct esp_mail_imap_download_config_t download;

    /* The config about the storage and path to save the downloaded file */
    struct esp_mail_imap_storage_config_t storage;

    /* The config about firmware updates and downloads for ESP32, ESP8266 and Raspberry Pi Pico */
    struct esp_mail_imap_firmware_config_t firmware_update;

    IMAP_Identification identification;
};

/* Mail and MIME Header Fields */
struct esp_mail_imap_msg_item_t
{
    friend class IMAPSession;

public:
    esp_mail_imap_msg_item_t()
    {
        text.content_type = mimeinfo[esp_mail_file_extension_txt].mimeType;
        html.content_type = mimeinfo[esp_mail_file_extension_html].mimeType;
    };

    /* The message number */
    int msgNo = 0;

    /* The message UID */
    int UID = 0;

    /* The message identifier (RFC 4021) */
    const char *ID = "";

    /* The language(s) for auto-responses (RFC 4021) */
    const char *acceptLang = "";

    /* The language of message content (RFC 4021) */
    const char *contentLang = "";

    /* The mailbox of message author (RFC 4021) */
    const char *from = "";

    /* The charset of the mailbox of message author */
    // deprecated
    const char *fromCharset = "";

    /* The primary recipient mailbox (RFC 4021) */
    const char *to = "";

    /* The charset of the primary recipient mailbox */
    // deprecated
    const char *toCharset = "";

    /* The Carbon-copy recipient mailboxes (RFC 4021) */
    const char *cc = "";

    /* The charset of the Carbon-copy recipient mailbox header */
    // deprecated
    const char *ccCharset = "";

    /* The Blind-carbon-copy recipient mailboxes (RFC 4021) */
    const char *bcc = "";

    /* The charset of the Blind-carbon-copy recipient mailbox header */
    // deprecated
    const char *bccCharset = "";

    /* The message date and time (RFC 4021) */
    const char *date = "";

    /* The topic of message (RFC 4021) */
    const char *subject = "";

    /* The topic of message charset */
    // deprecated
    const char *subjectCharset = "";

    /* The message flags */
    const char *flags = "";

    /* The PLAIN text content of the message */
    esp_mail_imap_plain_body_t text;

    /* The HTML content of the message */
    esp_mail_imap_html_body_t html;

    /* rfc822 related */

    /* The sender Email  */
    const char *sender;

    /* The charset of the sender Email */
    // deprecated
    const char *senderCharset = "";

    /* The keywords or phrases, separated by commas */
    const char *keywords = "";

    /* The comments about message */
    const char *comments = "";

    /* The field that contains the parent's message ID of the message to which this one is a reply */
    const char *in_reply_to = "";

    /* The field that contains the parent's references (if any) and followed by the parent's message ID (if any) of the message to which this one is a reply */
    const char *references = "";

    /* The return recipient of the message */
    const char *return_path = "";

    /* The Email address to reply */
    const char *reply_to;

    /* The error description from fetching the message */
    const char *fetchError = "";

    /* The info about the attachments in the message */
    _vectorImpl<struct esp_mail_attachment_info_t> attachments;

    /* The info about the rfc822 messages included in the message */
    _vectorImpl<esp_mail_imap_msg_item_t> rfc822;

    /* The status for message that contains attachment */
    bool hasAttachment = false;

private:
    void setRFC822Headers(struct esp_mail_imap_rfc822_msg_header_item_t *rfc822_header)
    {
        from = rfc822_header->header_items[esp_mail_rfc822_header_field_from].c_str();
        sender = rfc822_header->header_items[esp_mail_rfc822_header_field_sender].c_str();
        to = rfc822_header->header_items[esp_mail_rfc822_header_field_to].c_str();
        cc = rfc822_header->header_items[esp_mail_rfc822_header_field_cc].c_str();
        subject = rfc822_header->header_items[esp_mail_rfc822_header_field_subject].c_str();
        date = rfc822_header->header_items[esp_mail_rfc822_header_field_date].c_str();
        ID = rfc822_header->header_items[esp_mail_rfc822_header_field_msg_id].c_str();
        return_path = rfc822_header->header_items[esp_mail_rfc822_header_field_return_path].c_str();
        reply_to = rfc822_header->header_items[esp_mail_rfc822_header_field_reply_to].c_str();
        in_reply_to = rfc822_header->header_items[esp_mail_rfc822_header_field_in_reply_to].c_str();
        references = rfc822_header->header_items[esp_mail_rfc822_header_field_references].c_str();
        comments = rfc822_header->header_items[esp_mail_rfc822_header_field_comments].c_str();
        keywords = rfc822_header->header_items[esp_mail_rfc822_header_field_keywords].c_str();
        bcc = rfc822_header->header_items[esp_mail_rfc822_header_field_bcc].c_str();
        flags = rfc822_header->header_items[esp_mail_rfc822_header_field_flags].c_str();
        text.charSet = "";
        text.content_type = "";
        text.transfer_encoding = "";
        html.charSet = "";
        html.content_type = "";
        html.transfer_encoding = "";
    }
};

struct esp_mail_imap_msg_list_t
{
    /* The info of a message */
    _vectorImpl<esp_mail_imap_msg_item_t> msgItems;
};

struct esp_mail_imap_multipart_level_t
{
    uint8_t level = 0;
    bool fetch_rfc822_header = false;
    bool append_body_text = false;
};

struct esp_mail_imap_response_data
{
public:
    esp_mail_imap_response_status imapResp = esp_mail_imap_resp_unknown;
    char *response = nullptr;
    int readLen = 0;
    long dataTime = millis();
    int chunkBufSize = 512;
    int chunkIdx = 0;
    bool isUntaggedResponse = false;
    bool untaggedRespCompleted = false;
    bool completedResponse = false;
    bool endSearch = false;
    struct esp_mail_message_header_t header;
    struct esp_mail_message_part_info_t part;
    MB_String filePath;
    bool downloadRequest = false;
    int octetCount = 0;
    int octetLength = 0;
    bool tmo = false;
    int headerState = 0;
    int searchCount = 0;
    char *lastBuf = nullptr;
    char *buf = nullptr;

    esp_mail_imap_response_data(int bufLen) { chunkBufSize = bufLen; };
    ~esp_mail_imap_response_data() { clear(); }
    void clear()
    {
        if (response)
            free(response);
        if (lastBuf)
            free(lastBuf);
        if (buf)
            free(buf);

        response = nullptr;
        lastBuf = nullptr;
        buf = nullptr;
    }
};

#endif

#if defined(ENABLE_SMTP) || defined(ENABLE_IMAP)

struct esp_mail_link_internal_t
{
    MB_String cid;
};

struct esp_mail_sesson_cert_config_t
{
    /* The certificate data (base64 data) */
    const char *cert_data = NULL;

    /* The certificate file (DER format) */
    MB_String cert_file;

    /* The storage type */
    esp_mail_file_storage_type cert_file_storage_type;

    /* The cerificate verification option */
    bool verify = false;
};

struct esp_mail_smtp_logs_config_t
{

    /* The log file path */
    MB_String filename;

    /* The storage type */
    esp_mail_file_storage_type storage_type;
};

struct esp_mail_sesson_sever_config_t
{
    /* The hostName of the server */
    MB_String host_name;
    /* The port on the server to connect to */
    uint16_t port = 0;
};

/* The log in credentials */
struct esp_mail_sesson_login_config_t
{
    /* The user Email address to log in */
    MB_String email;

    /* The user password to log in */
    MB_String password;

    /* The OAuth2.0 access token to log in */
    MB_String accessToken;

    /* The host name or public IP of client system */
    MB_String user_domain;
};

/* The device time config */
struct esp_mail_sesson_time_config_t
{
    /* set the NTP servers (use comma to separate the servers) to let the library to set the time from NTP server */
    MB_String ntp_server;

    /* the GMT offset or time zone */
    float gmt_offset = 0;

    /* the day light saving offset */
    float day_light_offset = 0;

    /** TZ environment variable for local time setting
     * See https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
     */
    MB_String timezone_env_string;

    /* the file path to store TZ environment variable */
    MB_String timezone_file = "/tze.txt";
};

struct esp_mail_sesson_secure_config_t
{
    /** The option (obsoleted) to send the SMTP and IMAP commands to start the TLS connection rfc2595 section 3 and rfc3207 */
    bool startTLS = false;

    /* The secure connection mode preference */
    esp_mail_secure_mode mode = esp_mail_secure_mode_undefined;
};

struct esp_mail_spi_ethernet_module_t
{
#if defined(ESP8266) && defined(ESP8266_CORE_SDK_V3_X_X)
#ifdef INC_ENC28J60_LWIP
    ENC28J60lwIP *enc28j60 = nullptr;
#endif
#ifdef INC_W5100_LWIP
    Wiznet5100lwIP *w5100 = nullptr;
#endif
#ifdef INC_W5500_LWIP
    Wiznet5500lwIP *w5500 = nullptr;
#endif
#elif defined(MB_ARDUINO_PICO)

#endif
};

struct esp_mail_session_config_t
{
    friend class IMAPSession;
    friend class SMTPSession;
    friend class ESP_Mail_Client;

    /* The server config */
    struct esp_mail_sesson_sever_config_t server;

    /* The log in config */
    struct esp_mail_sesson_login_config_t login;

    /* The device time config */
    struct esp_mail_sesson_time_config_t time;

    /* The secure config */
    struct esp_mail_sesson_secure_config_t secure;

    /* The certificate config */
    struct esp_mail_sesson_cert_config_t certificate;

    /* SPI Ethernet Module config for ESP8266 */
    struct esp_mail_spi_ethernet_module_t spi_ethernet_module;

    /* The callback function for WiFi connection */
    NetworkConnectionHandler network_connection_handler = NULL;

    /* specific ports and its protocols */
    struct esp_mail_ports_functions ports_functions;

    /* The mail sending logs config */
    struct esp_mail_smtp_logs_config_t sentLogs;

public:
    esp_mail_session_config_t(){};

    ~esp_mail_session_config_t()
    {
        clear();
        aremovePtr();
    }

    void addPtr(_vectorImpl<int> *listPtr, int ptr)
    {
        if (listPtr)
        {
            this->listPtr = listPtr;
            bool existed = false;

            for (size_t i = 0; i < this->listPtr->size(); i++)
            {
                if ((*this->listPtr)[i] == ptr)
                    existed = true;
            }

            if (!existed)
                this->listPtr->push_back(ptr);
        }
    }

    void aremovePtr()
    {
        if (listPtr)
        {
            int ptr = toAddr(*this);
            for (size_t i = 0; i < listPtr->size(); i++)
            {
                if ((*listPtr)[i] == ptr)
                {
                    listPtr->erase(listPtr->begin() + i, listPtr->begin() + i + 1);
                    break;
                }
            }
        }
    }

    void clear()
    {
        server.host_name.clear();
        server.port = 0;

        secure.startTLS = false;
        secure.mode = esp_mail_secure_mode_undefined;

        login.email.clear();
        login.password.clear();
        login.user_domain.clear();
        login.accessToken.clear();

        time.day_light_offset = 0;
        time.gmt_offset = 0;
        time.ntp_server.clear();
        time.timezone_env_string.clear();

        certificate.cert_data = "";
        certificate.cert_file = "";
        certificate.cert_file_storage_type = esp_mail_file_storage_type_none;
        certificate.verify = false;

        clearPorts();
    }

private:
    int cert_ptr = 0;
    bool cert_updated = false;
    _vectorImpl<int> *listPtr = nullptr;

    // Internal flags use to keep user sercure.startTLS and secure.mode.
    bool int_start_tls = false;
    esp_mail_secure_mode int_mode = esp_mail_secure_mode_undefined;

    void clearPorts()
    {
        if (ports_functions.list)
        {
            if (ports_functions.use_internal_list)
            {
                ports_functions.size = 0;
                ports_functions.use_internal_list = false;
                delete[] ports_functions.list;
                ports_functions.list = nullptr;
            }
        }
    }
};

/** The content transfer encoding
 * enc_7bit or "7bit"
 * enc_qp or "quoted-printable"
 * enc_base64 or "base64"
 * enc_binary or "binary"
 * enc_8bit or "8bit"
 */
typedef struct esp_mail_transfer_encoding_t Content_Transfer_Encoding;

/* The session configuations */
typedef struct esp_mail_session_config_t ESP_Mail_Session; // obsoleted
typedef struct esp_mail_session_config_t Session_Config;

#endif

#if defined(ENABLE_SMTP)
/* The result from sending the Email */
typedef struct esp_mail_smtp_send_status_t SMTP_Result;

/* The attachment details for sending the Email */
typedef struct esp_mail_attachment_t SMTP_Attachment;
#endif

#if defined(ENABLE_SMTP) && defined(ENABLE_IMAP)

typedef struct esp_mail_attachment_t ESP_Mail_Attachment;

#endif

#if defined(ENABLE_IMAP)
/* The info of the selected or open mailbox folder e.g. name, attributes and
 * delimiter */
typedef struct esp_mail_folder_info_item_t FolderInfo;
/* The attachment item details for a message which returned from fetching the
 * Email */
typedef struct esp_mail_attachment_info_t IMAP_Attach_Item;

/** The IMAP operation configuations */
typedef struct esp_mail_imap_data_config_t IMAP_Config; // obsoleted

typedef struct esp_mail_imap_data_config_t IMAP_Data;

/* The message item data of the IMAP_MSG_List which contains header, body and
 * attachments info for eacch message*/
typedef struct esp_mail_imap_msg_item_t IMAP_MSG_Item;

/* The list that contains the message items from searching or fetching the Email
 */
typedef struct esp_mail_imap_msg_list_t IMAP_MSG_List;

#endif

struct esp_mail_wifi_credential_t
{
    MB_String ssid;
    MB_String password;
};

struct esp_mail_wifi_credentials_t
{
    friend class ESP_Mail_Client;
    friend class ESP_Mail_TCPClient;

public:
    esp_mail_wifi_credentials_t(){};
    ~esp_mail_wifi_credentials_t()
    {
        clearAP();
        clearMulti();
    };
    void addAP(const String &ssid, const String &password)
    {
        esp_mail_wifi_credential_t cred;
        cred.ssid = ssid;
        cred.password = password;
        credentials.push_back(cred);
    }
    void clearAP()
    {
        credentials.clear();
    }

private:
    _vectorImpl<esp_mail_wifi_credential_t> credentials;
#if defined(ESP_MAIL_HAS_WIFIMULTI)
    WiFiMulti *multi = nullptr;
#endif

    void reconnect()
    {
        if (credentials.size())
        {
            disconnect();
            connect();
        }
    }

    void connect()
    {
#if defined(ESP_MAIL_HAS_WIFIMULTI)

        clearMulti();
        multi = new WiFiMulti();
        for (size_t i = 0; i < credentials.size(); i++)
            multi->addAP(credentials[i].ssid.c_str(), credentials[i].password.c_str());

        if (credentials.size() > 0)
            multi->run();

#elif defined(ESP_MAIL_WIFI_IS_AVAILABLE)
        WiFi.begin((CONST_STRING_CAST)credentials[0].ssid.c_str(), credentials[0].password.c_str());
#endif
    }

    void disconnect()
    {
#if defined(ESP_MAIL_WIFI_IS_AVAILABLE)
        WiFi.disconnect();
#endif
    }

    void clearMulti()
    {
#if defined(ESP_MAIL_HAS_WIFIMULTI)
        if (multi)
            delete multi;
        multi = nullptr;
#endif
    }
};

static const char esp_mail_imap_tag_str[] PROGMEM = "xmail";

#if !defined(SILENT_MODE)
static const char esp_mail_version_str[] PROGMEM = "ESP Mail Client v";

/////////////////////////
// SMTP debug string

static const char esp_mail_dbg_str_1[] PROGMEM = "send command, STARTTLS";
static const char esp_mail_dbg_str_2[] PROGMEM = "connecting to SMTP server";

#if defined(ENABLE_SMTP)
static const char esp_mail_dbg_str_3[] PROGMEM = "send Email";
static const char esp_mail_dbg_str_4[] PROGMEM = "SMTP server connected";
static const char esp_mail_dbg_str_5[] PROGMEM = "send SMTP command, EHLO";
static const char esp_mail_dbg_str_6[] PROGMEM = "send SMTP command, AUTH LOGIN";
static const char esp_mail_dbg_str_7[] PROGMEM = "send SMTP command, AUTH PLAIN";
static const char esp_mail_dbg_str_8[] PROGMEM = "send message header";
static const char esp_mail_dbg_str_9[] PROGMEM = "send message body";
static const char esp_mail_dbg_str_10[] PROGMEM = "send attachments";
static const char esp_mail_dbg_str_11[] PROGMEM = "terminate the SMTP session";
static const char esp_mail_dbg_str_12[] PROGMEM = "message sent successfully";
static const char esp_mail_dbg_str_13[] PROGMEM = "send next Email";
static const char esp_mail_dbg_str_14[] PROGMEM = "send inline data";
static const char esp_mail_dbg_str_15[] PROGMEM = "send smtp command, AUTH XOAUTH2";
static const char esp_mail_dbg_str_16[] PROGMEM = "finishing the message sending";
static const char esp_mail_dbg_str_17[] PROGMEM = "No ESMTP supported, send SMTP command, HELO";
#endif

/////////////////////////
// IMAP debug string
static const char esp_mail_dbg_str_18[] PROGMEM = "connecting to IMAP server";
static const char esp_mail_dbg_str_19[] PROGMEM = "Host > ";
static const char esp_mail_dbg_str_20[] PROGMEM = "Port > ";
static const char esp_mail_dbg_str_21[] PROGMEM = "Reading time from NTP server";
static const char esp_mail_dbg_str_22[] PROGMEM = "perform SSL/TLS handshake";

#if defined(ENABLE_IMAP)
static const char esp_mail_dbg_str_23[] PROGMEM = "get my ACL";
static const char esp_mail_dbg_str_24[] PROGMEM = "checking the capability";
static const char esp_mail_dbg_str_25[] PROGMEM = "fetching message ";
static const char esp_mail_dbg_str_26[] PROGMEM = "fetch message header";
static const char esp_mail_dbg_str_27[] PROGMEM = "fetch body part header, ";
static const char esp_mail_dbg_str_28[] PROGMEM = "fetch body sub part header, ";
static const char esp_mail_dbg_str_29[] PROGMEM = "finished reading Email";
static const char esp_mail_dbg_str_30[] PROGMEM = "UID is ";
static const char esp_mail_dbg_str_31[] PROGMEM = "log out completed";
static const char esp_mail_dbg_str_32[] PROGMEM = "closing the ";
static const char esp_mail_dbg_str_33[] PROGMEM = "IMAP server connected";
static const char esp_mail_dbg_str_34[] PROGMEM = "send IMAP command, LOGIN";
static const char esp_mail_dbg_str_35[] PROGMEM = "send IMAP command, LIST";
static const char esp_mail_dbg_str_36[] PROGMEM = "searching messages";
static const char esp_mail_dbg_str_37[] PROGMEM = "send IMAP command, FETCH";
static const char esp_mail_dbg_str_38[] PROGMEM = "send IMAP command, LOGOUT";
static const char esp_mail_dbg_str_39[] PROGMEM = "message fetching completed";
static const char esp_mail_dbg_str_40[] PROGMEM = "opening the mailbox folder";
static const char esp_mail_dbg_str_41[] PROGMEM = "setting FLAG";
static const char esp_mail_dbg_str_42[] PROGMEM = "adding FLAG";
static const char esp_mail_dbg_str_43[] PROGMEM = "removing FLAG";
static const char esp_mail_dbg_str_44[] PROGMEM = "send IMAP command, AUTHENTICATE PLAIN";
static const char esp_mail_dbg_str_45[] PROGMEM = "send IMAP command, AUTH XOAUTH2";
static const char esp_mail_dbg_str_46[] PROGMEM = "reading plain TEXT message";
static const char esp_mail_dbg_str_47[] PROGMEM = "reading HTML message";
static const char esp_mail_dbg_str_48[] PROGMEM = "copying message(s) to ";
static const char esp_mail_dbg_str_49[] PROGMEM = "creating folder";
static const char esp_mail_dbg_str_50[] PROGMEM = "deleting folder";
static const char esp_mail_dbg_str_51[] PROGMEM = "listening to ";
static const char esp_mail_dbg_str_52[] PROGMEM = " folder changes";
static const char esp_mail_dbg_str_53[] PROGMEM = "polling established";
static const char esp_mail_dbg_str_54[] PROGMEM = "Mailbox listening stopped";
static const char esp_mail_dbg_str_55[] PROGMEM = "renaming folder";
static const char esp_mail_dbg_str_56[] PROGMEM = "send IMAP command, LSUB";
static const char esp_mail_dbg_str_57[] PROGMEM = "send IMAP command, SUBSCRIBE";
static const char esp_mail_dbg_str_58[] PROGMEM = "send IMAP command, UNSUBSCRIBE";
static const char esp_mail_dbg_str_59[] PROGMEM = "moving message(s) to ";
static const char esp_mail_dbg_str_60[] PROGMEM = "send IMAP command, GETQUOTA";
static const char esp_mail_dbg_str_61[] PROGMEM = "send IMAP command, SETQUOTA";
static const char esp_mail_dbg_str_62[] PROGMEM = "send IMAP command, GETQUOTAROOT";
static const char esp_mail_dbg_str_63[] PROGMEM = "send IMAP command, GETACL";
static const char esp_mail_dbg_str_64[] PROGMEM = "send IMAP command, SETACL";
static const char esp_mail_dbg_str_65[] PROGMEM = "send IMAP command, DELETEACL";
static const char esp_mail_dbg_str_66[] PROGMEM = "send IMAP command, MYRIGHTS";
static const char esp_mail_dbg_str_67[] PROGMEM = "send IMAP command, NAMESPACE";
static const char esp_mail_dbg_str_68[] PROGMEM = "selecting the ";
static const char esp_mail_dbg_str_69[] PROGMEM = "appending message";
static const char esp_mail_dbg_str_70[] PROGMEM = "download attachment %d of %d";
static const char esp_mail_dbg_str_71[] PROGMEM = "download HTML message";
static const char esp_mail_dbg_str_72[] PROGMEM = "deleting the ACL";
static const char esp_mail_dbg_str_73[] PROGMEM = "message append successfully";
static const char esp_mail_dbg_str_74[] PROGMEM = "download plain TEXT message";
static const char esp_mail_dbg_str_75[] PROGMEM = "deleting message(s)";
static const char esp_mail_dbg_str_76[] PROGMEM = "check the capability";
static const char esp_mail_dbg_str_77[] PROGMEM = "get the ACL";
static const char esp_mail_dbg_str_78[] PROGMEM = "set the ACL";
static const char esp_mail_dbg_str_79[] PROGMEM = "get UID";
static const char esp_mail_dbg_str_80[] PROGMEM = "get Flags";
static const char esp_mail_dbg_str_81[] PROGMEM = "delete folder";
static const char esp_mail_dbg_str_82[] PROGMEM = "send IMAP command, ID";
static const char esp_mail_dbg_str_83[] PROGMEM = "send IMAP command, NOOP";
#endif

/////////////////////////
// SMTP callback string

static const char esp_mail_cb_str_1[] PROGMEM = "Connecting to SMTP server...";
static const char esp_mail_cb_str_2[] PROGMEM = "Sending STARTTLS command...";

#if defined(ENABLE_SMTP)
static const char esp_mail_cb_str_3[] PROGMEM = "Sending greeting response...";
static const char esp_mail_cb_str_4[] PROGMEM = "Sending message header...";
static const char esp_mail_cb_str_5[] PROGMEM = "Sending message body...";
static const char esp_mail_cb_str_6[] PROGMEM = "Sending attachments...";
static const char esp_mail_cb_str_7[] PROGMEM = "Closing the session...";
static const char esp_mail_cb_str_8[] PROGMEM = "Sending inline data...";
static const char esp_mail_cb_str_9[] PROGMEM = "Sending Email...";
static const char esp_mail_cb_str_10[] PROGMEM = "Sending next Email...";
static const char esp_mail_cb_str_11[] PROGMEM = "Finishing the message sending...";
static const char esp_mail_cb_str_12[] PROGMEM = "SMTP server connected, wait for greeting...";
static const char esp_mail_cb_str_13[] PROGMEM = "Message sent successfully";
#endif

/////////////////////////
// IMAP callback string

static const char esp_mail_cb_str_14[] PROGMEM = "Logging in..."; // shared with SMTP
static const char esp_mail_cb_str_15[] PROGMEM = "Connecting to IMAP server...";

#if defined(ENABLE_IMAP)

static const char esp_mail_cb_str_16[] PROGMEM = "Reading the list of mailboxes...";
static const char esp_mail_cb_str_17[] PROGMEM = "Checking the capability...";
static const char esp_mail_cb_str_18[] PROGMEM = "Searching messages...";
static const char esp_mail_cb_str_19[] PROGMEM = "Downloading attachments...";
static const char esp_mail_cb_str_20[] PROGMEM = "Logging out...";
static const char esp_mail_cb_str_21[] PROGMEM = "Saving message header to file...";
static const char esp_mail_cb_str_22[] PROGMEM = "Get FLAG...";
static const char esp_mail_cb_str_23[] PROGMEM = "Removing FLAG...";
static const char esp_mail_cb_str_24[] PROGMEM = "Adding FLAG...";
static const char esp_mail_cb_str_25[] PROGMEM = "Get UID...";
static const char esp_mail_cb_str_26[] PROGMEM = "Setting FLAG...";
static const char esp_mail_cb_str_27[] PROGMEM = "Closing the mailbox folder...";
static const char esp_mail_cb_str_28[] PROGMEM = "Reading messages...";
static const char esp_mail_cb_str_29[] PROGMEM = "Listening to mailbox changes...";
static const char esp_mail_cb_str_30[] PROGMEM = "Listing the subscribed mailboxes...";
static const char esp_mail_cb_str_31[] PROGMEM = "Subscribe mailbox...";
static const char esp_mail_cb_str_32[] PROGMEM = "Unsubscribe mailbox...";
static const char esp_mail_cb_str_33[] PROGMEM = "Get quota root resource usage and limit...";
static const char esp_mail_cb_str_34[] PROGMEM = "Set quota root resource usage and limit...";
static const char esp_mail_cb_str_35[] PROGMEM = "Get the list of quota roots...";
static const char esp_mail_cb_str_36[] PROGMEM = "Get the ACL...";
static const char esp_mail_cb_str_37[] PROGMEM = "Setting the ACL...";
static const char esp_mail_cb_str_38[] PROGMEM = "Deleting the ACL...";
static const char esp_mail_cb_str_39[] PROGMEM = "Get my ACL...";
static const char esp_mail_cb_str_40[] PROGMEM = "Get namespace...";
static const char esp_mail_cb_str_41[] PROGMEM = "Enable capability...";
static const char esp_mail_cb_str_42[] PROGMEM = "Updating firmware...";
static const char esp_mail_cb_str_43[] PROGMEM = "Downloading messages...";
static const char esp_mail_cb_str_44[] PROGMEM = "Appending message...";
static const char esp_mail_cb_str_45[] PROGMEM = "Message append successfully";
static const char esp_mail_cb_str_46[] PROGMEM = "Finished reading Email";
static const char esp_mail_cb_str_47[] PROGMEM = "Log out completed";
static const char esp_mail_cb_str_48[] PROGMEM = "IMAP server connected";
static const char esp_mail_cb_str_49[] PROGMEM = "Polling established";
static const char esp_mail_cb_str_50[] PROGMEM = "Mailbox listening stopped";
static const char esp_mail_cb_str_51[] PROGMEM = "Open the mailbox folder...";
static const char esp_mail_cb_str_52[] PROGMEM = "Checking the capability...";
static const char esp_mail_cb_str_53[] PROGMEM = "Renaming folder...";
static const char esp_mail_cb_str_54[] PROGMEM = "UID is ";
static const char esp_mail_cb_str_55[] PROGMEM = "Get Flags...";
static const char esp_mail_cb_str_56[] PROGMEM = "Deleting folder...";
static const char esp_mail_cb_str_57[] PROGMEM = "Deleting message(s)...";
static const char esp_mail_cb_str_58[] PROGMEM = "Copying message(s)...";
static const char esp_mail_cb_str_59[] PROGMEM = "Creating folder...";
static const char esp_mail_cb_str_60[] PROGMEM = "Moving message(s)...";
static const char esp_mail_cb_str_61[] PROGMEM = "Send client identification...";
static const char esp_mail_cb_str_62[] PROGMEM = "Send noop...";
#endif

#endif

/////////////////////////
// Mem error string

#if defined(ENABLE_ERROR_STRING) || !defined(SILENT_MODE)
#if defined(MBFS_FLASH_FS) || defined(MBFS_SD_FS)
static const char esp_mail_error_mem_str_1[] PROGMEM = "flash Storage is not ready.";
static const char esp_mail_error_mem_str_2[] PROGMEM = "SD Storage is not ready.";
static const char esp_mail_error_mem_str_3[] PROGMEM = "file does not exist or can't access";
#endif
static const char esp_mail_error_mem_str_4[] PROGMEM = "PSRAM was enabled but not detected.";
#endif

#if defined(ENABLE_ERROR_STRING)

#if defined(MBFS_FLASH_FS) || defined(MBFS_SD_FS)
static const char esp_mail_error_mem_str_5[] PROGMEM = "file is still opened.";
static const char esp_mail_error_mem_str_6[] PROGMEM = "file not found.";
static const char esp_mail_error_mem_str_7[] PROGMEM = "file I/O error";
#endif

static const char esp_mail_error_mem_str_8[] PROGMEM = "out of memory";
static const char esp_mail_error_mem_str_9[] PROGMEM = "buffer overflow";

#endif

#if defined(MB_ARDUINO_PICO)
#if defined(ENABLE_ERROR_STRING) || !defined(SILENT_MODE)
static const char esp_mail_error_mem_str_10[] PROGMEM = "please make sure that the size of flash filesystem is not 0 in Pico.";
#endif
#endif

/////////////////////////
// Client error string

#if !defined(SILENT_MODE)
static const char esp_mail_error_client_str_1[] PROGMEM = "client and/or necessary callback functions are not yet assigned";
static const char esp_mail_error_client_str_2[] PROGMEM = "network connection callback is required";
static const char esp_mail_error_client_str_3[] PROGMEM = "network connection status callback is required";
static const char esp_mail_error_client_str_4[] PROGMEM = "NTP server time reading cannot begin when valid time is required because of no WiFi capability/activity detected.";
static const char esp_mail_error_client_str_5[] PROGMEM = "Please set the library reference time manually using smtp.setSystemTime or imap.setSystemTime.";
#endif

/////////////////////////
// Network error string

#if !defined(SILENT_MODE)
static const char esp_mail_error_network_str_1[] PROGMEM = "unable to connect to server";
#if defined(ENABLE_ERROR_STRING)
static const char esp_mail_error_network_str_2[] PROGMEM = "NTP server time reading timed out";
static const char esp_mail_error_network_str_3[] PROGMEM = "response read timed out";
static const char esp_mail_error_network_str_4[] PROGMEM = "not connected";
static const char esp_mail_error_network_str_5[] PROGMEM = "connection timeout";
static const char esp_mail_error_network_str_6[] PROGMEM = "connection closed";
static const char esp_mail_error_network_str_7[] PROGMEM = "connection refused";
static const char esp_mail_error_network_str_8[] PROGMEM = "data sending failed";
#endif
#endif

#if defined(ENABLE_ERROR_STRING) || !defined(SILENT_MODE)
static const char esp_mail_error_network_str_9[] PROGMEM = "response read timed out";
#endif

/////////////////////////
// SSL error string

#if defined(ENABLE_ERROR_STRING)
static const char esp_mail_error_ssl_str_1[] PROGMEM = "fail to set up the SSL/TLS structure";
#endif
#if defined(ENABLE_ERROR_STRING) || !defined(SILENT_MODE)
static const char esp_mail_error_ssl_str_2[] PROGMEM = "the alert SSL record received";
static const char esp_mail_error_ssl_str_3[] PROGMEM = "make sure the SSL/TLS handshake was done before sending the data";
#endif

/////////////////////////
// Auth error string

#if defined(ENABLE_ERROR_STRING)
static const char esp_mail_error_auth_str_1[] PROGMEM = "the provided SASL authentication mechanism is not support";
static const char esp_mail_error_auth_str_2[] PROGMEM = "OAuth2.0 log in was disabled for this server";
static const char esp_mail_error_auth_str_3[] PROGMEM = "not yet log in";
#endif

/////////////////////////
// Session error string

#if defined(ENABLE_ERROR_STRING)
static const char esp_mail_error_session_str_1[] PROGMEM = "the Session_Config object was not assigned";
#endif

/////////////////////////
// Time error string
#if defined(ENABLE_ERROR_STRING)
static const char esp_mail_error_time_str_1[] PROGMEM = "library or device time was not set, see examples/SMTP/Set_Time.ino for manually time setting";
#endif

/////////////////////////
// SMTP error string
#if defined(ENABLE_SMTP)
#if defined(ENABLE_ERROR_STRING)
static const char esp_mail_error_smtp_str_1[] PROGMEM = "SMTP server greeting failed";
static const char esp_mail_error_smtp_str_2[] PROGMEM = "authentication failed";
static const char esp_mail_error_smtp_str_3[] PROGMEM = "login password is not valid";
static const char esp_mail_error_smtp_str_4[] PROGMEM = "send header failed";
static const char esp_mail_error_smtp_str_5[] PROGMEM = "send body failed";
static const char esp_mail_error_smtp_str_7[] PROGMEM = "sender Email address is not valid";
static const char esp_mail_error_smtp_str_8[] PROGMEM = "some of the recipient Email address is not valid";
static const char esp_mail_error_smtp_str_9[] PROGMEM = "set recipient failed";
static const char esp_mail_error_smtp_str_10[] PROGMEM = "send custom command failed";
static const char esp_mail_error_smtp_str_11[] PROGMEM = "XOAuth2 authenticate failed";
static const char esp_mail_error_smtp_str_12[] PROGMEM = "undefined error";
#endif
#endif

/////////////////////////
// IMAP error string
#if defined(ENABLE_IMAP)
#if defined(ENABLE_ERROR_STRING)
static const char esp_mail_error_imap_str_1[] PROGMEM = "fail to list the mailboxes";
static const char esp_mail_error_imap_str_2[] PROGMEM = "fail to check the capabilities";

static const char esp_mail_error_imap_str_3[] PROGMEM = "fail to close the mailbox";

static const char esp_mail_error_imap_str_4[] PROGMEM = "fail to open the mailbox";
static const char esp_mail_error_imap_str_5[] PROGMEM = "some of the requested messages no longer exist";
static const char esp_mail_error_imap_str_6[] PROGMEM = "firmware update initialization failed";
static const char esp_mail_error_imap_str_7[] PROGMEM = "firmware update write failed";
static const char esp_mail_error_imap_str_8[] PROGMEM = "firmware update finalize failed";

#endif
#if defined(ENABLE_ERROR_STRING) || !defined(SILENT_MODE)
static const char esp_mail_error_imap_str_9[] PROGMEM = "no messages found for the specified search criteria";
static const char esp_mail_error_imap_str_10[] PROGMEM = "no search criteria provided, then fetching the latest message";
static const char esp_mail_error_imap_str_11[] PROGMEM = "no mailbox opened";
static const char esp_mail_error_imap_str_12[] PROGMEM = "no content";
static const char esp_mail_error_imap_str_13[] PROGMEM = "this feature was not supported";
static const char esp_mail_error_imap_str_14[] PROGMEM = "no message changed since (assigned) modsec";
static const char esp_mail_error_imap_str_15[] PROGMEM = "CONDSTORE was not supported or modsec was not supported for selected mailbox";
static const char esp_mail_error_imap_str_17[] PROGMEM = "could not parse command";
static const char esp_mail_error_imap_str_18[] PROGMEM = "server disconnected or returned error";
static const char esp_mail_error_imap_str_19[] PROGMEM = "authenticate failed";
static const char esp_mail_error_imap_str_20[] PROGMEM = "flags or keywords store failed";
static const char esp_mail_error_imap_str_21[] PROGMEM = "server is not support OAuth2 login";
#endif
#endif

/////////////////////////
// General use string
static const char esp_mail_str_1[] PROGMEM = "127.0.0.1";
static const char esp_mail_str_2[] PROGMEM = " ";
static const char esp_mail_str_3[] PROGMEM = "*";
static const char esp_mail_str_4[] PROGMEM = "High";
static const char esp_mail_str_5[] PROGMEM = "Normal";
static const char esp_mail_str_6[] PROGMEM = "Low";
static const char esp_mail_str_7[] PROGMEM = "=";
static const char esp_mail_str_8[] PROGMEM = ",";
static const char esp_mail_str_9[] PROGMEM = "--";
static const char esp_mail_str_10[] PROGMEM = "/";
static const char esp_mail_str_11[] PROGMEM = "\"";
static const char esp_mail_str_12[] PROGMEM = "Error, ";
static const char esp_mail_str_13[] PROGMEM = "msg.txt";
static const char esp_mail_str_14[] PROGMEM = "msg.html";
static const char esp_mail_str_15[] PROGMEM = "flash content message";
static const char esp_mail_str_16[] PROGMEM = "file content message";
static const char esp_mail_str_17[] PROGMEM = "cid:";
static const char esp_mail_str_18[] PROGMEM = "\r\n";
static const char esp_mail_str_19[] PROGMEM = "<";
static const char esp_mail_str_20[] PROGMEM = ">";
static const char esp_mail_str_21[] PROGMEM = "(\"name\" \"ESP Mail Client\" \"version\" \"%s\")";
static const char esp_mail_str_22[] PROGMEM = "message/rfc822";
static const char esp_mail_str_23[] PROGMEM = "upload";
static const char esp_mail_str_24[] PROGMEM = "%";
static const char esp_mail_str_25[] PROGMEM = "status code: ";
static const char esp_mail_str_26[] PROGMEM = ", text: ";
static const char esp_mail_str_27[] PROGMEM = ".";
static const char esp_mail_str_28[] PROGMEM = "> C: ";
static const char esp_mail_str_29[] PROGMEM = "< S: ";
static const char esp_mail_str_30[] PROGMEM = "! E: ";
static const char esp_mail_str_31[] PROGMEM = "! I: ";
static const char esp_mail_str_32[] PROGMEM = "! W: ";
static const char esp_mail_str_33[] PROGMEM = "#### ";
static const char esp_mail_str_34[] PROGMEM = ":";
static const char esp_mail_str_35[] PROGMEM = ";";
static const char esp_mail_str_36[] PROGMEM = "{";
static const char esp_mail_str_37[] PROGMEM = "}";
static const char esp_mail_str_38[] PROGMEM = "(";
static const char esp_mail_str_39[] PROGMEM = ")";
static const char esp_mail_str_40[] PROGMEM = "[";
static const char esp_mail_str_41[] PROGMEM = "]";
static const char esp_mail_str_42[] PROGMEM = "\r\n";
static const char esp_mail_str_43[] PROGMEM = "\1\1";
static const char esp_mail_str_44[] PROGMEM = "{\"status\":";
static const char esp_mail_str_45[] PROGMEM = "user=";
static const char esp_mail_str_46[] PROGMEM = "\1auth=Bearer ";
static const char esp_mail_str_47[] PROGMEM = "0123456789ABCDEF";
static const char esp_mail_str_48[] PROGMEM = "<0.0>";
static const char esp_mail_str_49[] PROGMEM = "<0.";
static const char esp_mail_str_50[] PROGMEM = "Search limit: %d\nFound %d messages\nShow %d messages";
static const char esp_mail_str_51[] PROGMEM = "1.0";
static const char esp_mail_str_52[] PROGMEM = "Fetch message %d, UID: %d";
static const char esp_mail_str_53[] PROGMEM = "Fetch message %d, Number: %d";
static const char esp_mail_str_54[] PROGMEM = "Attachments (%d)";
static const char esp_mail_str_55[] PROGMEM = "Free Heap: ";
static const char esp_mail_str_56[] PROGMEM = "content-";
static const char esp_mail_str_58[] PROGMEM = "format=\"flowed\"";
static const char esp_mail_str_59[] PROGMEM = "format=flowed";
static const char esp_mail_str_60[] PROGMEM = "delsp=\"yes\"";
static const char esp_mail_str_61[] PROGMEM = "delsp=yes";
static const char esp_mail_str_62[] PROGMEM = "name";
static const char esp_mail_str_63[] PROGMEM = "+ ";
static const char esp_mail_str_64[] PROGMEM = "boundary=\"";
static const char esp_mail_str_65[] PROGMEM = "/header.json";
static const char esp_mail_str_66[] PROGMEM = "/header.txt";
static const char esp_mail_str_67[] PROGMEM = "{\"Filename\":\"";
static const char esp_mail_str_68[] PROGMEM = "Index: ";
static const char esp_mail_str_69[] PROGMEM = ",\"RFC822\":";
static const char esp_mail_str_70[] PROGMEM = "\r\n\r\nRFC822:\r\n";
static const char esp_mail_str_71[] PROGMEM = ",\"Attachments\":{\"Count\":";
static const char esp_mail_str_72[] PROGMEM = ",\"Files\":[";
static const char esp_mail_str_73[] PROGMEM = "\r\n\r\nAttachments (";
static const char esp_mail_str_74[] PROGMEM = ")\r\n";
static const char esp_mail_str_75[] PROGMEM = "]}";
static const char esp_mail_str_76[] PROGMEM = "{\"Messages\":[";
static const char esp_mail_str_77[] PROGMEM = ",\"";
static const char esp_mail_str_78[] PROGMEM = "{\"";
static const char esp_mail_str_79[] PROGMEM = "\":\"";
static const char esp_mail_str_80[] PROGMEM = "{\"Renamed\":\"";
static const char esp_mail_str_81[] PROGMEM = "\",\"Original\":\"";
static const char esp_mail_str_82[] PROGMEM = "\"}]";
static const char esp_mail_str_83[] PROGMEM = "_";
static const char esp_mail_str_84[] PROGMEM = "message";
static const char esp_mail_str_85[] PROGMEM = "rfc822";
static const char esp_mail_str_86[] PROGMEM = "/msg";
static const char esp_mail_str_87[] PROGMEM = "/rfc822_msg";
static const char esp_mail_str_88[] PROGMEM = "polling established on ";
static const char esp_mail_str_89[] PROGMEM = " folder...";
static const char esp_mail_str_90[] PROGMEM = "boundary";
static const char esp_mail_str_91[] PROGMEM = "\\Deleted";
static const char esp_mail_str_92[] PROGMEM = "Subject: %s";
static const char esp_mail_str_93[] PROGMEM = "Message sent success: %d";
static const char esp_mail_str_94[] PROGMEM = "Message sent failed: %d";
static const char esp_mail_str_95[] PROGMEM = "Status: %s";
static const char esp_mail_str_96[] PROGMEM = "Date/Time: %s";
static const char esp_mail_str_97[] PROGMEM = "Recipient: %s";
static const char esp_mail_str_98[] PROGMEM = "success";
static const char esp_mail_str_99[] PROGMEM = "failed";

#if defined(ENABLE_SMTP)
static const char boundary_table[] PROGMEM = "=_abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
#endif

#if defined(ENABLE_SMTP) || defined(ENABLE_IMAP)

static const unsigned char b64_index_table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static void __attribute__((used))
appendDebugTag(MB_String &buf, esp_mail_debug_tag_type type, bool clear, PGM_P text = NULL)
{
    if (clear)
        buf.clear();

    switch (type)
    {
    case esp_mail_debug_tag_type_client:
        buf += esp_mail_str_28; /* "> C: "*/
        break;
    case esp_mail_debug_tag_type_server:
        buf += esp_mail_str_29; /* "< S: " */
        break;
    case esp_mail_debug_tag_type_error:
        buf += esp_mail_str_30; /* "! E: "*/
        break;
    case esp_mail_debug_tag_type_info:
        buf += esp_mail_str_31; /* "! I: "*/
        break;
    case esp_mail_debug_tag_type_warning:
        buf += esp_mail_str_32; /* "! W: "*/
        break;
    default:
        break;
    }

    if (text != NULL)
        buf += text;
}

static void __attribute__((used))
yield_impl()
{
#if defined(ARDUINO_ESP8266_MAJOR) && defined(ARDUINO_ESP8266_MINOR) && defined(ARDUINO_ESP8266_REVISION) && ((ARDUINO_ESP8266_MAJOR == 3 && ARDUINO_ESP8266_MINOR >= 1) || ARDUINO_ESP8266_MAJOR > 3)
    esp_yield();
#else
    delay(0);
#endif
}

// Print debug message w/wo new line to debug port
static void __attribute__((used))
esp_mail_debug_print(PGM_P msg = "", bool newLine = true)
{
    yield_impl();
    if (newLine)
        ESP_MAIL_DEFAULT_DEBUG_PORT.println(msg);
    else
        ESP_MAIL_DEFAULT_DEBUG_PORT.print(msg);
}

static void __attribute__((used))
esp_mail_debug_print_tag(PGM_P msg, esp_mail_debug_tag_type type, bool newLine = true, bool showTag = true)
{
    yield_impl();

    MB_String s;
    if (showTag)
        appendDebugTag(s, type, false, msg);
    else
        s = msg;

    if (newLine)
        ESP_MAIL_DEFAULT_DEBUG_PORT.println(s.c_str());
    else
        ESP_MAIL_DEFAULT_DEBUG_PORT.print(s.c_str());
}

#endif

typedef void (*ConnectionUpgradeRequestCallback)(void);
typedef void (*NetworkConnectionRequestCallback)(void);
typedef void (*NetworkDisconnectionRequestCallback)(void);
typedef void (*NetworkStatusRequestCallback)(void);
// Optional
typedef void (*ConnectionRequestCallback)(const char *, int);
#endif