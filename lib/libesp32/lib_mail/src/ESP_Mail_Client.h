#ifndef ESP_Mail_Client_H
#define ESP_Mail_Client_H

#define ESP_MAIL_VERSION "1.2.0"

/**
 * Mail Client Arduino Library for Espressif's ESP32 and ESP8266
 *
 *   Version:   1.2.0
 *   Released:  May 17, 2021
 *
 *   Updates:
 * - Add support ESP8266 Core SDK v3.x.x.
 *
 *
 * This library allows Espressif's ESP32 and ESP8266 devices to send and read
 * Email
 * through the SMTP and IMAP servers.
 *
 * The MIT License (MIT)
 * Copyright (c) 2021 K. Suwatchai (Mobizt)
 *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of
 * the Software, and to permit persons to whom the Software is furnished to do
 * so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
 * OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <Arduino.h>
#include "extras/RFC2047.h"
#include "extras/ESPTimeHelper.h"

#if defined(ESP32)
#define FORMAT_FLASH FORMAT_FLASH_IF_MOUNT_FAILED
#include <WiFi.h>
#include <FS.h>
//#include <SPIFFS.h>
#include <ETH.h>
#include "wcs/esp32/ESP_Mail_HTTPClient32.h"
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#define FS_NO_GLOBALS
#include <FS.h>
#include "wcs/esp8266/ESP_Mail_HTTPClient.h"
#endif

#include "extras/MIMEInfo.h"

#include <SD.h>
#include <vector>
#include <string>

#if defined(ESP8266)
#define SD_CS_PIN 15
#endif

#define SMTP_STATUS_SERVER_CONNECT_FAILED -100
#define SMTP_STATUS_SMTP_GREETING_GET_RESPONSE_FAILED -101
#define SMTP_STATUS_SMTP_GREETING_SEND_ACK_FAILED -102
#define SMTP_STATUS_AUTHEN_NOT_SUPPORT -103
#define SMTP_STATUS_AUTHEN_FAILED -104
#define SMTP_STATUS_USER_LOGIN_FAILED -105
#define SMTP_STATUS_PASSWORD_LOGIN_FAILED -106
#define SMTP_STATUS_SEND_HEADER_SENDER_FAILED -107
#define SMTP_STATUS_SEND_HEADER_RECIPIENT_FAILED -108
#define SMTP_STATUS_SEND_BODY_FAILED -109
#define SMTP_STATUS_SERVER_OAUTH2_LOGIN_DISABLED -110
#define SMTP_STATUS_NO_VALID_RECIPIENTS_EXISTED -111
#define SMTP_STATUS_NO_VALID_SENDER_EXISTED -112
#define SMTP_STATUS_NO_SUPPORTED_AUTH -113

#define IMAP_STATUS_SERVER_CONNECT_FAILED -200
#define IMAP_STATUS_IMAP_RESPONSE_FAILED -201
#define IMAP_STATUS_LOGIN_FAILED -202
#define IMAP_STATUS_BAD_COMMAND -203
#define IMAP_STATUS_PARSE_FLAG_FAILED -204
#define IMAP_STATUS_SERVER_OAUTH2_LOGIN_DISABLED -205
#define IMAP_STATUS_NO_MESSAGE -206
#define IMAP_STATUS_ERROR_DOWNLAD_TIMEOUT -207
#define IMAP_STATUS_CLOSE_MAILBOX_FAILED -208
#define IMAP_STATUS_OPEN_MAILBOX_FAILED -209
#define IMAP_STATUS_LIST_MAILBOXS_FAILED -210
#define IMAP_STATUS_CHECK_CAPABILITIES_FAILED -211
#define IMAP_STATUS_NO_SUPPORTED_AUTH -212
#define IMAP_STATUS_NO_MAILBOX_FOLDER_OPENED -213

#define MAIL_CLIENT_ERROR_CONNECTION_LOST -28
#define MAIL_CLIENT_ERROR_READ_TIMEOUT -29
#define MAIL_CLIENT_ERROR_FILE_IO_ERROR -30
#define MAIL_CLIENT_ERROR_SERVER_CONNECTION_FAILED -31
#define MAIL_CLIENT_ERROR_SSL_TLS_STRUCTURE_SETUP -32
#define MAIL_CLIENT_ERROR_OUT_OF_MEMORY -33

#define MAX_EMAIL_SEARCH_LIMIT 1000
#define BASE64_CHUNKED_LEN 76
#define FLOWED_TEXT_LEN 78
#define ESP_MAIL_WIFI_RECONNECT_TIMEOUT 10000
#define ESP_MAIL_PROGRESS_REPORT_STEP 20
#define ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED 0
#define ESP_MAIL_CLIENT_STREAM_CHUNK_SIZE 256
#define ESP_MAIL_CLIENT_VALID_TS 1577836800

#if defined(ESP32)
#define ESP_MAIL_MIN_MEM 70000
#elif defined(ESP8266)
#define ESP_MAIL_MIN_MEM 4000
#endif

class IMAPSession;
class SMTPSession;
class SMTP_Status;
class DownloadProgress;
class MessageData;

enum esp_mail_smtp_status_code
{
  esp_mail_smtp_status_code_0, // default

  /* Positive Completion */
  esp_mail_smtp_status_code_211 = 221, // System status, or system help reply
  esp_mail_smtp_status_code_214 =
      214,                             // Help message(A response to the HELP command)
  esp_mail_smtp_status_code_220 = 220, //<domain> Service ready
  esp_mail_smtp_status_code_221 =
      221, //<domain> Service closing transmission channel [RFC 2034]
  esp_mail_smtp_status_code_235 =
      235,                             // 2.7.0 Authentication succeeded[RFC 4954]
  esp_mail_smtp_status_code_250 = 250, // Requested mail action okay, completed
  esp_mail_smtp_status_code_251 = 251, // User not local; will forward
  esp_mail_smtp_status_code_252 = 252, // Cannot verify the user, but it will
                                       // try to deliver the message anyway

  /* Positive Intermediate */
  esp_mail_smtp_status_code_334 = 334, //(Server challenge - the text part
                                       //contains the Base64 - encoded
                                       //challenge)[RFC 4954]
  esp_mail_smtp_status_code_354 = 354, // Start mail input

  /* Transient Negative Completion */
  /* "Transient Negative" means the error condition is temporary, and the action
     may be requested again.*/
  esp_mail_smtp_status_code_421 =
      421, // Service is unavailable because the server is shutting down.
  esp_mail_smtp_status_code_432 =
      432, // 4.7.12 A password transition is needed [RFC 4954]
  esp_mail_smtp_status_code_450 =
      450, // Requested mail action not taken: mailbox unavailable (e.g.,
           // mailbox busy or temporarily blocked for policy reasons)
  esp_mail_smtp_status_code_451 =
      451, // Requested action aborted : local error in processing
  // e.g.IMAP server unavailable[RFC 4468]
  esp_mail_smtp_status_code_452 =
      452, // Requested action not taken : insufficient system storage
  esp_mail_smtp_status_code_454 =
      454, // 4.7.0 Temporary authentication failure[RFC 4954]
  esp_mail_smtp_status_code_455 =
      455, // Server unable to accommodate parameters

  /* Permanent Negative Completion */
  esp_mail_smtp_status_code_500 = 500, // Syntax error, command unrecognized
                                       // (This may include errors such as
                                       // command line too long)
  // e.g. Authentication Exchange line is too long [RFC 4954]
  esp_mail_smtp_status_code_501 =
      501, // Syntax error in parameters or arguments
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
  esp_mail_smtp_status_code_530 =
      530, // 5.7.0 Authentication required [RFC 4954]
  esp_mail_smtp_status_code_534 =
      534, // 5.7.9 Authentication mechanism is too weak [RFC 4954]
  esp_mail_smtp_status_code_535 =
      535,                             // 5.7.8 Authentication credentials invalid [RFC 4954]
  esp_mail_smtp_status_code_538 = 538, // 5.7.11 Encryption required for
                                       // requested authentication mechanism[RFC
                                       // 4954]
  esp_mail_smtp_status_code_550 =
      550, // Requested action not taken: mailbox unavailable (e.g., mailbox not
           // found, no access, or command rejected for policy reasons)
  esp_mail_smtp_status_code_551 =
      551, // User not local; please try <forward-path>
  esp_mail_smtp_status_code_552 =
      552, // Requested mail action aborted: exceeded storage allocation
  esp_mail_smtp_status_code_553 =
      553,                             // Requested action not taken: mailbox name not allowed
  esp_mail_smtp_status_code_554 = 554, // Transaction has failed (Or, in the
                                       // case of a connection-opening response,
                                       // "No SMTP service here")
  // e.g. 5.3.4 Message too big for system [RFC 4468]
  esp_mail_smtp_status_code_556 = 556, // Domain does not accept mail[RFC 7504]
};

enum esp_mail_smtp_port
{
  esp_mail_smtp_port_25 = 25,   // PLAIN/TLS with STARTTLS
  esp_mail_smtp_port_465 = 465, // SSL
  esp_mail_smtp_port_587 = 587  // TLS with STARTTLS
};

enum esp_mail_imap_port
{
  esp_mail_imap_port_143 = 143, // PLAIN/TLS with STARTTLS
  esp_mail_imap_port_993 = 993, // SSL
};

enum esp_mail_smtp_notify
{
  esp_mail_smtp_notify_never = 0,
  esp_mail_smtp_notify_success = 1,
  esp_mail_smtp_notify_failure = 2,
  esp_mail_smtp_notify_delay = 4
};

enum esp_mail_attach_type
{
  esp_mail_att_type_none,
  esp_mail_att_type_attachment,
  esp_mail_att_type_inline
};

enum esp_mail_message_type
{
  esp_mail_msg_type_none = 0,
  esp_mail_msg_type_plain = 1,
  esp_mail_msg_type_html = 2,
  esp_mail_msg_type_enriched = 1
};

enum esp_mail_auth_type
{
  esp_mail_auth_type_psw,
  esp_mail_auth_type_oath2,
  esp_mail_auth_type_token
};

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

enum esp_mail_imap_header_state
{
  esp_mail_imap_state_from = 1,
  esp_mail_imap_state_to,
  esp_mail_imap_state_cc,
  esp_mail_imap_state_subject,
  esp_mail_imap_state_content_type,
  esp_mail_imap_state_content_transfer_encoding,
  esp_mail_imap_state_accept_language,
  esp_mail_imap_state_content_language,
  esp_mail_imap_state_date,
  esp_mail_imap_state_msg_id,
  esp_mail_imap_state_char_set,
  esp_mail_imap_state_boundary
};

enum esp_mail_imap_command
{
  esp_mail_imap_cmd_capability,
  esp_mail_imap_cmd_starttls,
  esp_mail_imap_cmd_login,
  esp_mail_imap_cmd_plain,
  esp_mail_imap_cmd_auth,
  esp_mail_imap_cmd_list,
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
  esp_mail_imap_cmd_logout,
  esp_mail_imap_cmd_store,
  esp_mail_imap_cmd_expunge,
  esp_mail_imap_cmd_create,
  esp_mail_imap_cmd_delete
};

enum esp_mail_imap_mime_fetch_type
{
  esp_mail_imap_mime_fetch_type_part,
  esp_mail_imap_mime_fetch_type_sub_part1,
  esp_mail_imap_mime_fetch_type_sub_part2
};

enum esp_mail_smtp_command
{
  esp_mail_smtp_cmd_initial_state,
  esp_mail_smtp_cmd_greeting,
  esp_mail_smtp_cmd_start_tls,
  esp_mail_smtp_cmd_login_user,
  esp_mail_smtp_cmd_auth_plain,
  esp_mail_smtp_cmd_auth,
  esp_mail_smtp_cmd_login_psw,
  esp_mail_smtp_cmd_send_header_sender,
  esp_mail_smtp_cmd_send_header_recipient,
  esp_mail_smtp_cmd_send_body,
  esp_mail_smtp_cmd_chunk_termination,
  esp_mail_smtp_cmd_logout
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

enum esp_mail_char_decoding_scheme
{
  esp_mail_char_decoding_scheme_default,
  esp_mail_char_decoding_scheme_iso8859_1,
  esp_mail_char_decoding_scheme_tis620
};

enum esp_mail_smtp_priority
{
  esp_mail_smtp_priority_high = 1,
  esp_mail_smtp_priority_normal = 3,
  esp_mail_smtp_priority_low = 5,
};

enum esp_mail_smtp_embed_message_type
{
  esp_mail_smtp_embed_message_type_attachment = 0,
  esp_mail_smtp_embed_message_type_inline
};

enum esp_mail_debug_level
{
  esp_mail_debug_level_0 = 0,
  esp_mail_debug_level_1,
  esp_mail_debug_level_2 = 222,
  esp_mail_debug_level_3 = 333
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

/** content disposition rfc 2183
 *
 * Parameters:
 * "filename", "creation-date","modification-date",
 * "read-date", * "size"
*/
struct esp_mail_imap_content_disposition_type_t
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

struct esp_mail_internal_use_t
{
  bool binary = false;
  std::string cid = "";
};

struct esp_mail_content_transfer_encoding_t
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

struct esp_mail_smtp_response_status_t
{
  int respCode = 0;
  int statusCode = 0;
  std::string text = "";
};

struct esp_mail_imap_response_status_t
{
  int statusCode = 0;
  std::string text = "";
};

/* The option to embed this message content as a file */
struct esp_mail_smtp_embed_message_body_t
{
  /* Enable to send this message body as file */
  bool enable = false;

  /* The name of embedded file */
  const char *filename = "";

  /** The embedded type
   * esp_mail_smtp_embed_message_type_attachment or 0
   * esp_mail_smtp_embed_message_type_inline or 1
  */
  esp_mail_smtp_embed_message_type type =
      esp_mail_smtp_embed_message_type_attachment;
};

struct esp_mail_file_message_content_t
{
  /* The file path include its name */
  const char *name = "";

  /** The type of file storages e.g.
   * esp_mail_file_storage_type_none,
   * esp_mail_file_storage_type_flash, and
   * esp_mail_file_storage_type_sd
  */
  esp_mail_file_storage_type type = esp_mail_file_storage_type_flash;
};

struct esp_mail_blob_message_content_t
{
  /* The array of content in flash memory */
  const uint8_t *data = nullptr;

  /* The array size in bytes */
  size_t size = 0;
};

/* The PLAIN text body details of the message */
struct esp_mail_plain_body_t
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
  const char *content_type = "text/plain";

  /* The option to encode the content for data transfer */
  const char *transfer_encoding = "7bit";

  /* The option to send the PLAIN text with wrapping */
  bool flowed = false;

  /* The internal usage data */
  struct esp_mail_internal_use_t _int;
};

struct esp_mail_html_body_t
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
  const char *content_type = "text/html";

  /* The option to encode the content for data transfer */
  const char *transfer_encoding = "7bit";

  /* The internal usage data */
  struct esp_mail_internal_use_t _int;
};

struct esp_mail_link_internal_t
{
  std::string cid = "";
};

struct esp_mail_email_info_t
{
  /* The name of Email author*/
  const char *name = "";

  /* The Email address */
  const char *email = "";
};
struct esp_mail_smtp_msg_response_t
{
  /* The author Email address to reply */
  const char *reply_to = "";

  /* The sender Email address to return the message */
  const char *return_path = "";

  /** The Delivery Status Notifications e.g. esp_mail_smtp_notify_never,
   * esp_mail_smtp_notify_success,
   * esp_mail_smtp_notify_failure, and
   * esp_mail_smtp_notify_delay
  */
  int notify = esp_mail_smtp_notify::esp_mail_smtp_notify_never;
};

struct esp_mail_smtp_enable_option_t
{
  /* Enable chunk data sending for large message */
  bool chunking = false;
};

struct esp_mail_attach_blob_t
{
  /* BLOB data (flash or ram) */
  const uint8_t *data = nullptr;

  /* BLOB data size in byte */
  size_t size = 0;
};

struct esp_mail_attach_file_t
{
  const char *path = "";
  /** The file storage type e.g. esp_mail_file_storage_type_none,
   * esp_mail_file_storage_type_flash, and
   * esp_mail_file_storage_type_sd
  */
  esp_mail_file_storage_type storage_type = esp_mail_file_storage_type_none;
};

struct esp_mail_attach_descr_t
{
  /* The name of attachment */
  const char *name = "";

  /* The attachment file name */
  const char *filename = "";

  /* The MIME type of attachment */
  const char *mime = "";

  /* The transfer encoding of attachment e.g. base64 */
  const char *transfer_encoding = "base64";

  /* The content encoding of attachment e.g. base64 */
  const char *content_encoding = "";

  /* The content id of attachment file */
  const char *content_id = "";
};

struct esp_mail_attach_internal_t
{
  esp_mail_attach_type att_type = esp_mail_att_type_attachment;
  int index = 0;
  int msg_uid = 0;
  bool flash_blob = false;
  bool binary = false;
  bool parallel = false;
  std::string cid = "";
};

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

struct esp_mail_smtp_recipient_t
{
  /* The recipient's name */
  const char *name = "";

  /* The recipient's Email address */
  const char *email = "";
};

struct esp_mail_smtp_recipient_address_t
{
  /* The recipient's Email address */
  const char *email = "";
};

struct esp_mail_smtp_send_status_t
{
  /* The status of the message */
  bool completed = false;

  /* The primary recipient mailbox of the message */
  const char *recipients = "";

  /* The topic of the message */
  const char *subject = "";

  /* The timestamp of the message */
  time_t timesstamp = 0;
};

struct esp_mail_attacment_info_t
{
  const char *filename = "";
  const char *name = "";
  const char *creationDate = "";
  const char *mime = "";
  esp_mail_attach_type type = esp_mail_att_type_none;
  size_t size;
};

struct esp_mail_auth_capability_t
{
  bool plain = false;
  bool xoauth2 = false;
  bool cram_md5 = false;
  bool digest_md5 = false;
  bool login = false;
  bool start_tls = false;
};

struct esp_mail_smtp_capability_t
{
  bool esmtp = false;
  bool binaryMIME = false;
  bool _8bitMIME = false;
  bool chunking = false;
  bool utf8 = false;
  bool pipelining = false;
  bool dsn = false;
};

struct esp_mail_imap_rfc822_msg_header_item_t
{
  std::string sender = "";
  std::string from;
  std::string subject = "";
  std::string messageID = "";
  std::string keyword = "";
  std::string comment = "";
  std::string date = "";
  std::string return_path = "";
  std::string reply_to;
  std::string to = "";
  std::string cc = "";
  std::string bcc = "";
};

struct esp_mail_message_part_info_t
{
  int octetLen = 0;
  int attach_data_size = 0;
  int textLen = 0;
  bool sizeProp = false;
  int nestedLevel = 0;
  std::string partNumStr = "";
  std::string partNumFetchStr = "";
  std::string text = "";
  std::string filename = "";
  std::string type = "";
  std::string save_path = "";
  std::string name = "";
  std::string content_disposition = "";
  std::string content_type = "";
  std::string descr = "";
  std::string content_transfer_encoding = "";
  std::string creation_date = "";
  std::string modification_date = "";
  std::string charset = "";
  std::string download_error = "";
  esp_mail_attach_type attach_type = esp_mail_att_type_none;
  esp_mail_message_type msg_type = esp_mail_msg_type_none;
  bool file_open_write = false;
  bool multipart = false;
  esp_mail_imap_multipart_sub_type multipart_sub_type =
      esp_mail_imap_multipart_sub_type_none;
  esp_mail_imap_message_sub_type message_sub_type =
      esp_mail_imap_message_sub_type_none;
  bool rfc822_part = false;
  int rfc822_msg_Idx = 0;
  struct esp_mail_imap_rfc822_msg_header_item_t rfc822_header;
  bool error = false;
  bool plain_flowed = false;
  bool plain_delsp = false;
};

struct esp_mail_message_header_t
{
  int header_data_len = 0;
  std::string from = "";
  std::string to = "";
  std::string cc = "";
  std::string subject = "";
  std::string content_type = "";
  std::string content_transfer_encoding = "";
  std::string date = "";
  std::string message_id = "";
  std::string message_uid = "";
  std::string message_no = "";
  std::string boundary = "";
  std::string accept_language = "";
  std::string content_language = "";
  std::string char_set = "";
  bool multipart = false;
  bool rfc822_part = false;
  int rfc822Idx = 0;
  std::string partNumStr = "";

  esp_mail_imap_multipart_sub_type multipart_sub_type =
      esp_mail_imap_multipart_sub_type_none;
  esp_mail_imap_message_sub_type message_sub_type =
      esp_mail_imap_message_sub_type_none;
  std::string from_charset = "";
  std::string to_charset = "";
  std::string cc_charset = "";
  std::string subject_charset = "";
  std::string msgID = "";
  std::string error_msg = "";
  bool error = false;
  std::vector<struct esp_mail_message_part_info_t> part_headers =
      std::vector<struct esp_mail_message_part_info_t>();
  int attachment_count = 0;
  int total_download_size = 0;
  int downloaded_size = 0;
  int total_attach_data_size = 0;
  int downloaded_bytes = 0;
  int message_data_count = 0;
};

/* Internal use */
struct esp_mail_folder_info_t
{
  std::string name = "";
  std::string attributes = "";
  std::string delimiter = "";
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

struct esp_mail_sesson_cert_config_t
{
  /* The certificate data (base64 data) */
  const char *cert_data = "";

  /* The certificate file (DER format) */
  const char *cert_file = "";

  /* The storage type */
  esp_mail_file_storage_type cert_file_storage_type;
};

struct esp_mail_sesson_sever_config_t
{
  /* The hostName of the server */
  const char *host_name = "";
  /* The port on the server to connect to */
  uint16_t port = 0;
};

/* The log in credentials */
struct esp_mail_sesson_login_config_t
{
  /* The user Email address to log in */
  const char *email = "";

  /* The user password to log in */
  const char *password = "";

  /* The OAuth2.0 access token to log in */
  const char *accessToken = "";

  /* The user domain or ip of client */
  const char *user_domain = "";
};

struct esp_mail_sesson_secure_config_t
{
  /* The option to send the command to start the TLS connection */
  bool startTLS = false;
};

struct esp_mail_session_config_t
{
  /* The server config */
  struct esp_mail_sesson_sever_config_t server;

  /* The log in config */
  struct esp_mail_sesson_login_config_t login;

  /* The secure config */
  struct esp_mail_sesson_secure_config_t secure;

  /* The certificate config */
  struct esp_mail_sesson_cert_config_t certificate;
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
};

struct esp_mail_imap_limit_config_t
{
  /* The maximum messages from the search result */
  size_t search = 10;

  /** The maximum size of the memory buffer to store the message content.
   * This is only limit for data to be stored in the IMAPSession.
  */
  size_t msg_size = 1024;

  /* The maximum size of each attachment to download */
  size_t attachment_size = 1024 * 1024 * 5;
};

struct esp_mail_imap_storage_config_t
{
  /* The path to save the downloaded file */
  const char *saved_path = "/";

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
  const char *criteria = "";

  /* The option to search the unseen message */
  bool unseen_msg = false;
};

struct esp_mail_imap_fetch_config_t
{
  /* The UID of message to fetch */
  const char *uid = "";

  /* Set the message flag as seen */
  bool set_seen = false;
};

struct esp_mail_imap_read_config_t
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
};

/* Mail and MIME Header Fields */
struct esp_mail_imap_msg_item_t
{
  /* The message number */
  const char *msgNo = "";

  /* The message UID */
  const char *UID = "";

  /* The message identifier (RFC 4021) */
  const char *ID = "";

  /* The language(s) for auto-responses (RFC 4021) */
  const char *acceptLang = "";

  /* The language of message content (RFC 4021) */
  const char *contentLang = "";

  /* The mailbox of message author (RFC 4021) */
  const char *from = "";

  /* The charset of the mailbox of message author */
  const char *fromCharset = "";

  /* The primary recipient mailbox (RFC 4021) */
  const char *to = "";

  /* The charset of the primary recipient mailbox */
  const char *toCharset = "";

  /* The Carbon-copy recipient mailboxes (RFC 4021) */
  const char *cc = "";

  /* The charset of the Carbon-copy recipient mailbox header */
  const char *ccCharset = "";

  /* The message date and time (RFC 4021) */
  const char *date = "";

  /* The topic of message (RFC 4021) */
  const char *subject = "";

  /* The topic of message charset */
  const char *subjectCharset = "";

  /* The PLAIN text content of the message */
  struct esp_mail_plain_body_t text;

  /* The HTML content of the message */
  struct esp_mail_html_body_t html;

  /* rfc822 related */

  /* The sender Email  */
  const char *sender;

  /* The message identifier */
  const char *messageID = "";

  /* The keywords or phrases, separated by commas */
  const char *keyword = "";

  /* The comment about message */
  const char *comment = "";

  /* The return recipient of the message */
  const char *return_path = "";

  /* The Email address to reply */
  const char *reply_to;

  /* The Blind carbon-copy recipients */
  const char *bcc = "";

  /* The error description from fetching the message */
  const char *fetchError = "";

  /* The info about the attachments in the message */
  std::vector<struct esp_mail_attacment_info_t> attachments =
      std::vector<struct esp_mail_attacment_info_t>();

  /* The info about the rfc822 messages included in the message */
  std::vector<esp_mail_imap_msg_item_t> rfc822 =
      std::vector<esp_mail_imap_msg_item_t>();
};

struct esp_mail_imap_msg_list_t
{
  /* The info of a message */
  std::vector<esp_mail_imap_msg_item_t> msgItems =
      std::vector<esp_mail_imap_msg_item_t>();
};

struct esp_mail_smtp_msg_type_t
{
  bool rfc822 = false;
  int rfc822Idx = 0;
};
struct esp_mail_imap_multipart_level_t
{
  uint8_t level = 0;
  bool fetch_rfc822_header = false;
  bool append_body_text = false;
};

/** The content transfer encoding
 * enc_7bit or "7bit"
 * enc_qp or "quoted-printable"
 * enc_base64 or "base64"
 * enc_binary or "binary"
 * enc_8bit or "8bit"
*/
typedef struct esp_mail_content_transfer_encoding_t Content_Transfer_Encoding;

/* The result from sending the Email */
typedef struct esp_mail_smtp_send_status_t SMTP_Result;

/* The attachment item details for a message which returned from fetching the
 * Email */
typedef struct esp_mail_attacment_info_t IMAP_Attach_Item;

/* The attachment details for sending the Email */
typedef struct esp_mail_attachment_t SMTP_Attachment;

/* The info of the selected or open mailbox folder e.g. name, attributes and
 * delimiter */
typedef struct esp_mail_folder_info_item_t FolderInfo;

/* The session configuations */
typedef struct esp_mail_session_config_t ESP_Mail_Session;

/** The IMAP operation configuations */
typedef struct esp_mail_imap_read_config_t IMAP_Config;

/* The message item data of the IMAP_MSG_List which contains header, body and
 * attachments info for eacch message*/
typedef struct esp_mail_imap_msg_item_t IMAP_MSG_Item;

/* The list that contains the message items from searching or fetching the Email
 */
typedef struct esp_mail_imap_msg_list_t IMAP_MSG_List;

static const char esp_mail_str_1[] PROGMEM =
    "Content-Type: multipart/mixed; boundary=\"";
static const char esp_mail_str_2[] PROGMEM = "$ CAPABILITY";
static const char esp_mail_str_3[] PROGMEM = "Mime-Version: 1.0\r\n";
static const char esp_mail_str_4[] PROGMEM = "AUTH LOGIN";
static const char esp_mail_str_5[] PROGMEM = "HELO ";
static const char esp_mail_str_6[] PROGMEM = "EHLO ";
static const char esp_mail_str_7[] PROGMEM = "QUIT";
static const char esp_mail_str_8[] PROGMEM = "MAIL FROM:";
static const char esp_mail_str_9[] PROGMEM = "RCPT TO:";
static const char esp_mail_str_10[] PROGMEM = "From: ";
static const char esp_mail_str_11[] PROGMEM = "To: ";
static const char esp_mail_str_12[] PROGMEM = "Cc: ";
static const char esp_mail_str_13[] PROGMEM = ",<";
static const char esp_mail_str_14[] PROGMEM = "<";
static const char esp_mail_str_15[] PROGMEM = ">";
static const char esp_mail_str_16[] PROGMEM = "DATA";
static const char esp_mail_str_17[] PROGMEM = "X-Priority: ";
static const char esp_mail_str_18[] PROGMEM = "X-MSMail-Priority: High\r\n";
static const char esp_mail_str_19[] PROGMEM = "X-MSMail-Priority: Normal\r\n";
static const char esp_mail_str_20[] PROGMEM = "X-MSMail-Priority: Low\r\n";
static const char esp_mail_str_21[] PROGMEM = "Importance: High\r\n";
static const char esp_mail_str_22[] PROGMEM = "Importance: Normal\r\n";
static const char esp_mail_str_23[] PROGMEM = "Importance: Low\r\n";
static const char esp_mail_str_24[] PROGMEM = "Subject: ";
static const char esp_mail_str_25[] PROGMEM = "Content-Type: ";
static const char esp_mail_str_26[] PROGMEM = "; Name=\"";
static const char esp_mail_str_27[] PROGMEM = "$";
static const char esp_mail_str_28[] PROGMEM =
    "Content-Type: multipart/parallel; boundary=\"";
static const char esp_mail_str_29[] PROGMEM = "7bit";
static const char esp_mail_str_30[] PROGMEM =
    "Content-Disposition: attachment; filename=\"";
static const char esp_mail_str_31[] PROGMEM = "base64";
static const char esp_mail_str_32[] PROGMEM = "application/octet-stream";
static const char esp_mail_str_33[] PROGMEM = "--";
static const char esp_mail_str_34[] PROGMEM = "\r\n";
static const char esp_mail_str_35[] PROGMEM = "\"\r\n\r\n";
static const char esp_mail_str_36[] PROGMEM = "\"\r\n";
static const char esp_mail_str_37[] PROGMEM = "\r\n.\r\n";
static const char esp_mail_str_38[] PROGMEM = "unable to connect to server";
static const char esp_mail_str_39[] PROGMEM = "SMTP server greeting failed";
static const char esp_mail_str_40[] PROGMEM = ".dat";
static const char esp_mail_str_41[] PROGMEM = "$ AUTHENTICATE PLAIN ";
static const char esp_mail_str_42[] PROGMEM =
    "the provided SASL authentication mechanism is not support";
static const char esp_mail_str_43[] PROGMEM = "authentication failed";
static const char esp_mail_str_44[] PROGMEM = "mydomain.com";
static const char esp_mail_str_45[] PROGMEM = "AUTH PLAIN";
static const char esp_mail_str_46[] PROGMEM = "Return-Path: ";
static const char esp_mail_str_47[] PROGMEM = "login password is not valid";
static const char esp_mail_str_48[] PROGMEM = "send header failed";
static const char esp_mail_str_49[] PROGMEM = "send body failed";
static const char esp_mail_str_50[] PROGMEM = "Connecting to IMAP server...";
static const char esp_mail_str_51[] PROGMEM =
    ".HEADER.FIELDS (SUBJECT FROM SENDER RETURN-PATH TO REPLY-TO DATE CC "
    "Message-ID COMMENT KEYWORD content-type Content-transfer-encoding)]";
static const char esp_mail_str_52[] PROGMEM = "failed";
static const char esp_mail_str_53[] PROGMEM = "Error, ";
static const char esp_mail_str_54[] PROGMEM = "IMAP server connected";
static const char esp_mail_str_55[] PROGMEM = "> C: download attachment";
static const char esp_mail_str_56[] PROGMEM = "Logging in...";
static const char esp_mail_str_57[] PROGMEM = "Downloading messages...";
static const char esp_mail_str_58[] PROGMEM =
    "Reading the list of mailboxes...";
static const char esp_mail_str_59[] PROGMEM =
    "> C: download plain TEXT message";
static const char esp_mail_str_60[] PROGMEM = "> C: download HTML message";
static const char esp_mail_str_61[] PROGMEM = "Selecting the ";
static const char esp_mail_str_62[] PROGMEM = "fail to list the mailboxes";
static const char esp_mail_str_63[] PROGMEM = "fail to check the capabilities";
static const char esp_mail_str_64[] PROGMEM = "Check the capability...";
static const char esp_mail_str_65[] PROGMEM = "> C: check the capability";
static const char esp_mail_str_66[] PROGMEM = "Searching messages...";
static const char esp_mail_str_67[] PROGMEM = "message";
static const char esp_mail_str_68[] PROGMEM = "Search limit:";
static const char esp_mail_str_69[] PROGMEM = "Found ";
static const char esp_mail_str_70[] PROGMEM = " messages";
static const char esp_mail_str_71[] PROGMEM = "Show ";
static const char esp_mail_str_72[] PROGMEM =
    "No message found for search criteria";
static const char esp_mail_str_73[] PROGMEM =
    "Search criteria does not set, fetch the recent message";
static const char esp_mail_str_74[] PROGMEM = "Fetching message ";
static const char esp_mail_str_75[] PROGMEM = ", UID: ";
static const char esp_mail_str_76[] PROGMEM = ", Number: ";
static const char esp_mail_str_77[] PROGMEM = "> C: fetch message header";
static const char esp_mail_str_78[] PROGMEM = "Attachments (";
static const char esp_mail_str_79[] PROGMEM = ")";
static const char esp_mail_str_80[] PROGMEM = "Downloading attachments...";
static const char esp_mail_str_81[] PROGMEM = "> C: fetch body part header, ";
static const char esp_mail_str_82[] PROGMEM = "rfc822";
static const char esp_mail_str_83[] PROGMEM = "reading";
static const char esp_mail_str_84[] PROGMEM = "Free Heap: ";
static const char esp_mail_str_85[] PROGMEM = "Logging out...";
static const char esp_mail_str_86[] PROGMEM =
    "> C: fetch body sub part header, ";
static const char esp_mail_str_87[] PROGMEM = "Finished reading Email";
static const char esp_mail_str_88[] PROGMEM = "> C: finished reading Email";
static const char esp_mail_str_89[] PROGMEM = "SD card mount failed";
static const char esp_mail_str_90[] PROGMEM = "download";
static const char esp_mail_str_91[] PROGMEM = ", ";
static const char esp_mail_str_92[] PROGMEM = "%";
static const char esp_mail_str_93[] PROGMEM = "connection timeout";
static const char esp_mail_str_94[] PROGMEM = ".html";
static const char esp_mail_str_95[] PROGMEM = ".txt";
static const char esp_mail_str_96[] PROGMEM = " folder...";
static const char esp_mail_str_97[] PROGMEM = ";";
static const char esp_mail_str_98[] PROGMEM =
    "Content-Disposition: attachment\r\n";
static const char esp_mail_str_99[] PROGMEM = "Date: ";
static const char esp_mail_str_100[] PROGMEM = "Messsage UID: ";
static const char esp_mail_str_101[] PROGMEM = "Messsage ID: ";
static const char esp_mail_str_102[] PROGMEM = "Accept Language: ";
static const char esp_mail_str_103[] PROGMEM = "Content Language: ";
static const char esp_mail_str_104[] PROGMEM = " BODY=BINARYMIME";
static const char esp_mail_str_105[] PROGMEM = "From Charset: ";
static const char esp_mail_str_106[] PROGMEM = "BDAT ";
static const char esp_mail_str_107[] PROGMEM = "To Charset: ";
static const char esp_mail_str_108[] PROGMEM = "CC: ";
static const char esp_mail_str_109[] PROGMEM = "CC Charset: ";
static const char esp_mail_str_110[] PROGMEM = "delsp=\"no\"";
static const char esp_mail_str_111[] PROGMEM = "Subject Charset: ";
static const char esp_mail_str_112[] PROGMEM = "Message Charset: ";
static const char esp_mail_str_113[] PROGMEM = "Attachment: ";
static const char esp_mail_str_114[] PROGMEM = "File Index: ";
static const char esp_mail_str_115[] PROGMEM = "Filename: ";
static const char esp_mail_str_116[] PROGMEM = "Name: ";
static const char esp_mail_str_117[] PROGMEM = "Size: ";
static const char esp_mail_str_118[] PROGMEM = "Type: ";
static const char esp_mail_str_119[] PROGMEM = "Creation Date: ";
static const char esp_mail_str_120[] PROGMEM = "Connecting to SMTP server...";
static const char esp_mail_str_121[] PROGMEM =
    "SMTP server connected, wait for greeting...";
static const char esp_mail_str_122[] PROGMEM = "Sending greeting response...";
static const char esp_mail_str_123[] PROGMEM = "message/rfc822";
static const char esp_mail_str_124[] PROGMEM =
    "Saving message header to file...";
static const char esp_mail_str_125[] PROGMEM = "Sending message header...";
static const char esp_mail_str_126[] PROGMEM = "Sending message body...";
static const char esp_mail_str_127[] PROGMEM = "Sending attachments...";
static const char esp_mail_str_128[] PROGMEM = "Closing the session...";
static const char esp_mail_str_129[] PROGMEM = "Message sent successfully";
static const char esp_mail_str_130[] PROGMEM = "$ LOGIN ";
static const char esp_mail_str_131[] PROGMEM = " ";
static const char esp_mail_str_132[] PROGMEM =
    "fail to set up the SSL/TLS structure";
static const char esp_mail_str_133[] PROGMEM = "$ LIST \"\" *";
static const char esp_mail_str_134[] PROGMEM = "Comment: ";
static const char esp_mail_str_135[] PROGMEM = "$ EXAMINE \"";
static const char esp_mail_str_136[] PROGMEM = "\"";
static const char esp_mail_str_137[] PROGMEM = "UID ";
static const char esp_mail_str_138[] PROGMEM = " UID";
static const char esp_mail_str_139[] PROGMEM = " SEARCH";
static const char esp_mail_str_140[] PROGMEM = "UID";
static const char esp_mail_str_141[] PROGMEM = "SEARCH";
static const char esp_mail_str_142[] PROGMEM = "$ UID FETCH ";
static const char esp_mail_str_143[] PROGMEM = "$ FETCH ";
static const char esp_mail_str_144[] PROGMEM =
    "HEADER.FIELDS (SUBJECT FROM TO DATE CC Message-ID Accept-Language "
    "content-type Content-transfer-encoding Content-Language)";
static const char esp_mail_str_145[] PROGMEM = "Keyword: ";
static const char esp_mail_str_146[] PROGMEM = "$ LOGOUT";
static const char esp_mail_str_147[] PROGMEM = " BODY";
static const char esp_mail_str_148[] PROGMEM = ".MIME]";
static const char esp_mail_str_149[] PROGMEM = "Bcc: ";
static const char esp_mail_str_150[] PROGMEM = "Sender: ";
static const char esp_mail_str_151[] PROGMEM = "no mailbox opened";
static const char esp_mail_str_152[] PROGMEM = ".";
static const char esp_mail_str_153[] PROGMEM = "No mailbox opened";
static const char esp_mail_str_154[] PROGMEM = "Remove FLAG";
static const char esp_mail_str_155[] PROGMEM = "Add FLAG";
static const char esp_mail_str_156[] PROGMEM = "]";
static const char esp_mail_str_157[] PROGMEM = "Set FLAG";
static const char esp_mail_str_158[] PROGMEM =
    "file does not exist or can't access";
static const char esp_mail_str_159[] PROGMEM = "msg.html";
static const char esp_mail_str_160[] PROGMEM = "upload ";
static const char esp_mail_str_161[] PROGMEM = "/msg";
static const char esp_mail_str_163[] PROGMEM = "/rfc822_msg";
static const char esp_mail_str_164[] PROGMEM = "msg.txt";
static const char esp_mail_str_165[] PROGMEM = "Content-Length: ";
static const char esp_mail_str_166[] PROGMEM = "binary";
static const char esp_mail_str_167[] PROGMEM = "Sending inline data...";
static const char esp_mail_str_168[] PROGMEM = "charset=\"";
static const char esp_mail_str_169[] PROGMEM = "charset=";
static const char esp_mail_str_170[] PROGMEM = "name=\"";
static const char esp_mail_str_171[] PROGMEM = "name=";
static const char esp_mail_str_172[] PROGMEM = "content-transfer-encoding: ";
static const char esp_mail_str_173[] PROGMEM = " LAST";
static const char esp_mail_str_174[] PROGMEM = "content-description: ";
static const char esp_mail_str_175[] PROGMEM = "content-disposition: ";
static const char esp_mail_str_176[] PROGMEM = "filename=\"";
static const char esp_mail_str_177[] PROGMEM = "filename=";
static const char esp_mail_str_178[] PROGMEM = "size=";
static const char esp_mail_str_179[] PROGMEM = "creation-date=\"";
static const char esp_mail_str_180[] PROGMEM = "creation-date=";
static const char esp_mail_str_181[] PROGMEM = "modification-date=\"";
static const char esp_mail_str_182[] PROGMEM = "modification-date=";
static const char esp_mail_str_183[] PROGMEM = "*";
static const char esp_mail_str_184[] PROGMEM = "Reply-To: ";
static const char esp_mail_str_185[] PROGMEM = "> E: ";
static const char esp_mail_str_186[] PROGMEM = "out of memory";
static const char esp_mail_str_187[] PROGMEM = "Message fetch cmpleted";
static const char esp_mail_str_188[] PROGMEM = "fail to close the mailbox";
static const char esp_mail_str_189[] PROGMEM = "message-id: ";
static const char esp_mail_str_190[] PROGMEM = "accept-language: ";
static const char esp_mail_str_191[] PROGMEM = "content-language: ";
static const char esp_mail_str_192[] PROGMEM = ")";
static const char esp_mail_str_193[] PROGMEM = "{";
static const char esp_mail_str_194[] PROGMEM = "}";
static const char esp_mail_str_195[] PROGMEM = "$ CLOSE\r\n";
static const char esp_mail_str_196[] PROGMEM = "> C: send STARTTLS command";
static const char esp_mail_str_197[] PROGMEM = "> C: close the mailbox folder";
static const char esp_mail_str_198[] PROGMEM = "(";
static const char esp_mail_str_199[] PROGMEM = " EXISTS";
static const char esp_mail_str_200[] PROGMEM = " [UIDNEXT ";
static const char esp_mail_str_201[] PROGMEM = "port > ";
static const char esp_mail_str_202[] PROGMEM = "/";
static const char esp_mail_str_203[] PROGMEM = "/header.txt";
static const char esp_mail_str_204[] PROGMEM = "/esp.32";
static const char esp_mail_str_205[] PROGMEM =
    "sender Email address is not valid";
static const char esp_mail_str_206[] PROGMEM =
    "some of the recipient Email address is not valid";
static const char esp_mail_str_207[] PROGMEM = "> C: send Email";
static const char esp_mail_str_208[] PROGMEM = "Sending Email...";
static const char esp_mail_str_209[] PROGMEM = "Send command, STARTTLS";
static const char esp_mail_str_210[] PROGMEM = "Closing the ";
static const char esp_mail_str_211[] PROGMEM = "host > ";
static const char esp_mail_str_212[] PROGMEM = "FLAGS";
static const char esp_mail_str_213[] PROGMEM = "BODY";
static const char esp_mail_str_214[] PROGMEM = "PEEK";
static const char esp_mail_str_215[] PROGMEM = "TEXT";
static const char esp_mail_str_216[] PROGMEM = "HEADER";
static const char esp_mail_str_217[] PROGMEM = "FIELDS";
static const char esp_mail_str_218[] PROGMEM = "[";
static const char esp_mail_str_219[] PROGMEM = "]";
static const char esp_mail_str_220[] PROGMEM = "MIME";
static const char esp_mail_str_221[] PROGMEM = "connection lost";
static const char esp_mail_str_222[] PROGMEM = "set recipient failed";
static const char esp_mail_str_223[] PROGMEM = " NEW";
static const char esp_mail_str_224[] PROGMEM = "ALL";
static const char esp_mail_str_225[] PROGMEM = "> C: connect to IMAP server";
static const char esp_mail_str_226[] PROGMEM = "windows-874";
static const char esp_mail_str_227[] PROGMEM = "iso-8859-1";
static const char esp_mail_str_228[] PROGMEM = "> C: server connected";
static const char esp_mail_str_229[] PROGMEM = "> C: send imap command, LOGIN";
static const char esp_mail_str_230[] PROGMEM = "> C: send imap command, LIST";
static const char esp_mail_str_231[] PROGMEM = "iso-8859-11";
static const char esp_mail_str_232[] PROGMEM = "> C: search messages";
static const char esp_mail_str_233[] PROGMEM = "> C: send imap command, FETCH";
static const char esp_mail_str_234[] PROGMEM = "> C: send imap command, LOGOUT";
static const char esp_mail_str_235[] PROGMEM = "> C: message fetch completed";
static const char esp_mail_str_236[] PROGMEM = "> C: connect to SMTP server";
static const char esp_mail_str_237[] PROGMEM = "tis-620";
static const char esp_mail_str_238[] PROGMEM = "> C: smtp server connected";
static const char esp_mail_str_239[] PROGMEM = "> C: send smtp command, HELO";
static const char esp_mail_str_240[] PROGMEM =
    "> C: send smtp command, AUTH LOGIN";
static const char esp_mail_str_241[] PROGMEM =
    "> C: send smtp command, AUTH PLAIN";
static const char esp_mail_str_242[] PROGMEM = "> C: send message header";
static const char esp_mail_str_243[] PROGMEM = "> C: send message body";
static const char esp_mail_str_244[] PROGMEM = "> C: send attachments";
static const char esp_mail_str_245[] PROGMEM =
    "> C: terminate the SMTP session";
static const char esp_mail_str_246[] PROGMEM = "> C: Message sent successfully";
static const char esp_mail_str_247[] PROGMEM = "$ SELECT \"";
static const char esp_mail_str_248[] PROGMEM = "> C: open the mailbox folder";
static const char esp_mail_str_249[] PROGMEM = "$ UID STORE ";
static const char esp_mail_str_250[] PROGMEM = " FLAGS (";
static const char esp_mail_str_251[] PROGMEM = " +FLAGS (";
static const char esp_mail_str_252[] PROGMEM = " -FLAGS (";
static const char esp_mail_str_253[] PROGMEM = "> C: set FLAG";
static const char esp_mail_str_254[] PROGMEM = "> C: add FLAG";
static const char esp_mail_str_255[] PROGMEM = "> C: remove FLAG";
static const char esp_mail_str_256[] PROGMEM = "could not parse flag";
static const char esp_mail_str_257[] PROGMEM = "delsp=\"yes\"";
static const char esp_mail_str_258[] PROGMEM = "session timed out";
static const char esp_mail_str_259[] PROGMEM = "delsp=yes";
static const char esp_mail_str_260[] PROGMEM = "< S: ";
static const char esp_mail_str_261[] PROGMEM = "> C: ";
static const char esp_mail_str_262[] PROGMEM = " NOTIFY=";
static const char esp_mail_str_263[] PROGMEM = ",";
static const char esp_mail_str_264[] PROGMEM = "SUCCESS";
static const char esp_mail_str_265[] PROGMEM = "FAILURE";
static const char esp_mail_str_266[] PROGMEM = "DELAY";
static const char esp_mail_str_267[] PROGMEM = "Sending next Email...";
static const char esp_mail_str_268[] PROGMEM = "> C: send next Email";
static const char esp_mail_str_269[] PROGMEM =
    "header.fields (content-type Content-transfer-encoding)]";
static const char esp_mail_str_270[] PROGMEM = "format=\"flowed\"";
static const char esp_mail_str_271[] PROGMEM = "> C: send inline data";
static const char esp_mail_str_272[] PROGMEM = "Content-transfer-encoding: ";
static const char esp_mail_str_273[] PROGMEM = "Date: ";
static const char esp_mail_str_274[] PROGMEM = "Message-ID:";
static const char esp_mail_str_275[] PROGMEM = "format=flowed";
static const char esp_mail_str_276[] PROGMEM = "CC: ";
static const char esp_mail_str_277[] PROGMEM = "boundary=\"";
static const char esp_mail_str_278[] PROGMEM = "quoted-printable";
static const char esp_mail_str_279[] PROGMEM = "Subject:";
static const char esp_mail_str_280[] PROGMEM = "> C: no content";
static const char esp_mail_str_281[] PROGMEM = "fail to open the mailbox";
static const char esp_mail_str_282[] PROGMEM = "file I/O error";
static const char esp_mail_str_283[] PROGMEM = "time.nist.gov";
static const char esp_mail_str_284[] PROGMEM =
    "log in was disabled for this server";
static const char esp_mail_str_285[] PROGMEM = "user=";
static const char esp_mail_str_286[] PROGMEM = "\1auth=Bearer ";
static const char esp_mail_str_287[] PROGMEM = "\1\1";
static const char esp_mail_str_288[] PROGMEM =
    "> C: send smtp command, AUTH XOAUTH2";
static const char esp_mail_str_289[] PROGMEM = "AUTH XOAUTH2 ";
static const char esp_mail_str_290[] PROGMEM =
    "> C: send imap command, AUTHENTICATE PLAIN";
static const char esp_mail_str_291[] PROGMEM =
    "> C: send imap command, AUTH XOAUTH2";
static const char esp_mail_str_292[] PROGMEM = "$ AUTHENTICATE XOAUTH2 ";
static const char esp_mail_str_293[] PROGMEM =
    "OAuth2.0 log in was disabled for this server";
static const char esp_mail_str_294[] PROGMEM = "{\"status\":";
static const char esp_mail_str_295[] PROGMEM = "0123456789ABCDEF";
static const char esp_mail_str_296[] PROGMEM = "pool.ntp.org";
static const char esp_mail_str_297[] PROGMEM =
    "Content-Type: multipart/alternative; boundary=\"";
static const char esp_mail_str_298[] PROGMEM =
    "Content-Type: multipart/related; boundary=\"";
static const char esp_mail_str_299[] PROGMEM =
    "Content-Disposition: inline; filename=\"";
static const char esp_mail_str_300[] PROGMEM = "Content-Location: ";
static const char esp_mail_str_301[] PROGMEM = "Content-ID: <";
static const char esp_mail_str_302[] PROGMEM = "cid:";
static const char esp_mail_str_303[] PROGMEM =
    "Finishing the message sending...";
static const char esp_mail_str_304[] PROGMEM =
    "> C: Finish the message sending";
static const char esp_mail_str_305[] PROGMEM = "connection failed";
static const char esp_mail_str_306[] PROGMEM =
    "some of the requested messages no longer exist";
static const char esp_mail_str_307[] PROGMEM = "Reading messages...";
static const char esp_mail_str_308[] PROGMEM =
    "> C: reading plain TEXT message";
static const char esp_mail_str_309[] PROGMEM = "> C: reading HTML message";
static const char esp_mail_str_310[] PROGMEM =
    "> C: performing the SSL/TLS handshake";
static const char esp_mail_str_311[] PROGMEM = "STARTTLS\r\n";
static const char esp_mail_str_312[] PROGMEM = "code: ";
static const char esp_mail_str_313[] PROGMEM = ", text: ";
static const char esp_mail_str_314[] PROGMEM = "> C: ESP Mail Client v";
static const char esp_mail_str_315[] PROGMEM = " +FLAGS.SILENT (\\Deleted)";
static const char esp_mail_str_316[] PROGMEM = "> C: delete message(s)";
static const char esp_mail_str_317[] PROGMEM = "$ EXPUNGE";
static const char esp_mail_str_318[] PROGMEM = "> C: copy message(s) to ";
static const char esp_mail_str_319[] PROGMEM = "$ UID COPY ";
static const char esp_mail_str_320[] PROGMEM = "> C: create folder";
static const char esp_mail_str_321[] PROGMEM = "> C: delete folder";
static const char esp_mail_str_322[] PROGMEM = "$ CREATE ";
static const char esp_mail_str_323[] PROGMEM = "$ DELETE ";
static const char esp_mail_str_324[] PROGMEM = "HEADER.FIELDS";
static const char esp_mail_str_325[] PROGMEM = "flash content message";
static const char esp_mail_str_326[] PROGMEM = "file content message";
static const char esp_mail_str_327[] PROGMEM = "\"; size=";
static const char esp_mail_str_328[] PROGMEM = "0.0.0.0";

static const char esp_mail_smtp_response_1[] PROGMEM = "AUTH ";
static const char esp_mail_smtp_response_2[] PROGMEM = " LOGIN";
static const char esp_mail_smtp_response_3[] PROGMEM = " PLAIN";
static const char esp_mail_smtp_response_4[] PROGMEM = " XOAUTH2";
static const char esp_mail_smtp_response_5[] PROGMEM = "STARTTLS";
static const char esp_mail_smtp_response_6[] PROGMEM = "8BITMIME";
static const char esp_mail_smtp_response_7[] PROGMEM = "BINARYMIME";
static const char esp_mail_smtp_response_8[] PROGMEM = "CHUNKING";
static const char esp_mail_smtp_response_9[] PROGMEM = "SMTPUTF8";
static const char esp_mail_smtp_response_10[] PROGMEM = "PIPELINING";
static const char esp_mail_smtp_response_11[] PROGMEM = " CRAM-MD5";
static const char esp_mail_smtp_response_12[] PROGMEM = " DIGEST-MD5";
static const char esp_mail_smtp_response_13[] PROGMEM = "DSN";
// Tagged
static const char esp_mail_imap_response_1[] PROGMEM = "$ OK ";
static const char esp_mail_imap_response_2[] PROGMEM = "$ NO ";
static const char esp_mail_imap_response_3[] PROGMEM = "$ BAD ";
// Untagged
static const char esp_mail_imap_response_4[] PROGMEM = "* LIST ";
static const char esp_mail_imap_response_5[] PROGMEM = "* FLAGS ";
static const char esp_mail_imap_response_6[] PROGMEM = "* SEARCH ";
static const char esp_mail_imap_response_7[] PROGMEM = " FETCH ";
static const char esp_mail_imap_response_8[] PROGMEM = " NIL ";
static const char esp_mail_imap_response_9[] PROGMEM = " UID ";
static const char esp_mail_imap_response_10[] PROGMEM = "* CAPABILITY ";
static const char esp_mail_imap_response_11[] PROGMEM = "LOGINDISABLED";
static const char esp_mail_imap_response_12[] PROGMEM = "AUTH=PLAIN";
static const char esp_mail_imap_response_13[] PROGMEM = "AUTH=XOAUTH2";
static const char esp_mail_imap_response_14[] PROGMEM = "STARTTLS";
static const char esp_mail_imap_response_15[] PROGMEM = "CRAM-MD5";
static const char esp_mail_imap_response_16[] PROGMEM = "DIGEST-MD5";

static const char imap_7bit_key1[] PROGMEM = "=20";
static const char imap_7bit_val1[] PROGMEM = " ";
static const char imap_7bit_key2[] PROGMEM = "=2C";
static const char imap_7bit_val2[] PROGMEM = ",";
static const char imap_7bit_key3[] PROGMEM = "=E2=80=99";
static const char imap_7bit_val3[] PROGMEM = "'";
static const char imap_7bit_key4[] PROGMEM = "=0A";
static const char imap_7bit_val4[] PROGMEM = "\r\n";
static const char imap_7bit_key5[] PROGMEM = "=0D";
static const char imap_7bit_val5[] PROGMEM = "\r\n";
static const char imap_7bit_key6[] PROGMEM = "=A0";
static const char imap_7bit_val6[] PROGMEM = " ";
static const char imap_7bit_key7[] PROGMEM = "=B9";
static const char imap_7bit_val7[] PROGMEM = "$sup1";
static const char imap_7bit_key8[] PROGMEM = "=C2=A0";
static const char imap_7bit_val8[] PROGMEM = " ";
static const char imap_7bit_key9[] PROGMEM = "=\r\n";
static const char imap_7bit_val9[] PROGMEM = "";
static const char imap_7bit_key10[] PROGMEM = "=E2=80=A6";
static const char imap_7bit_val10[] PROGMEM = "&hellip;";
static const char imap_7bit_key11[] PROGMEM = "=E2=80=A2";
static const char imap_7bit_val11[] PROGMEM = "&bull;";
static const char imap_7bit_key12[] PROGMEM = "=E2=80=93";
static const char imap_7bit_val12[] PROGMEM = "&ndash;";
static const char imap_7bit_key13[] PROGMEM = "=E2=80=94";
static const char imap_7bit_val13[] PROGMEM = "&mdash;";

static const unsigned char b64_index_table[65] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char boundary_table[] PROGMEM =
    "=_abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

__attribute__((used)) static bool compFunc(uint32_t i, uint32_t j)
{
  return (i > j);
}

class MessageList
{
public:
  friend class IMAPSession;
  MessageList(){};
  ~MessageList() { clear(); };
  void add(int uid)
  {
    if (uid > 0)
      _list.push_back(uid);
  }

  void clear() { _list.clear(); }

private:
  std::vector<int> _list = std::vector<int>();
};

/* The class that provides the info of selected or opened mailbox folder */
class SelectedFolderInfo
{
public:
  friend class ESP_Mail_Client;
  friend class IMAPSession;
  SelectedFolderInfo(){};
  ~SelectedFolderInfo() { clear(); };

  /* Get the flags count for this mailbox */
  size_t flagCount() { return _flags.size(); };

  /* Get the numbers of messages in this mailbox */
  size_t msgCount() { return _msgCount; };

  /* Get the predict next message UID */
  size_t nextUID() { return _nextUID; };

  /* Get the numbers of messages from search result based on the search criteria
   */
  size_t searchCount() { return _searchCount; };

  /* Get the numbers of messages to be stored in the ressult */
  size_t availableMessages() { return _availableItems; };

  /* Get the flag argument at the specified index */
  String flag(size_t index)
  {
    if (index < _flags.size())
      return _flags[index].c_str();
    return "";
  }

private:
  void addFlag(const char *flag) { _flags.push_back(flag); };
  void clear()
  {
    for (size_t i = 0; i < _flags.size(); i++)
      std::string().swap(_flags[i]);
    _flags.clear();
  }
  size_t _msgCount = 0;
  size_t _nextUID = 0;
  size_t _searchCount = 0;
  size_t _availableItems = 0;
  std::vector<std::string> _flags = std::vector<std::string>();
};

/* The class that provides the list of FolderInfo e.g. name, attributes and
 * delimiter */
class FoldersCollection
{
public:
  friend class ESP_Mail_Client;
  friend class IMAPSession;
  FoldersCollection(){};
  ~FoldersCollection() { clear(); };
  size_t size() { return _folders.size(); };

  struct esp_mail_folder_info_item_t info(size_t index)
  {
    struct esp_mail_folder_info_item_t fd;
    if (index < _folders.size())
    {
      fd.name = _folders[index].name.c_str();
      fd.attributes = _folders[index].attributes.c_str();
      fd.delimiter = _folders[index].delimiter.c_str();
    }
    return fd;
  }

private:
  void add(struct esp_mail_folder_info_t &fd) { _folders.push_back(fd); };
  void clear()
  {
    for (size_t i = 0; i < _folders.size(); i++)
    {
      if (_folders[i].name.length() > 0)
        std::string().swap(_folders[i].name);
      if (_folders[i].attributes.length() > 0)
        std::string().swap(_folders[i].attributes);
      if (_folders[i].delimiter.length() > 0)
        std::string().swap(_folders[i].delimiter);
    }
    _folders.clear();
  }
  std::vector<esp_mail_folder_info_t> _folders =
      std::vector<esp_mail_folder_info_t>();
};

/* The class that provides the status of message feching and searching */
class IMAP_Status
{
public:
  IMAP_Status();
  ~IMAP_Status();
  const char *info();
  bool success();
  void empty();
  friend class IMAPSession;

  std::string _info = "";
  bool _success = false;
};

/* The SMTP message class */
class SMTP_Message
{
public:
  SMTP_Message(){};
  ~SMTP_Message() { clear(); };

  void resetAttachItem(SMTP_Attachment &att)
  {
    att.blob.size = 0;
    att.blob.data = nullptr;
    att.file.path = "";
    att.file.storage_type = esp_mail_file_storage_type_none;
    att.descr.name = "";
    att.descr.filename = "";
    att.descr.transfer_encoding = "";
    att.descr.content_encoding = "";
    att.descr.mime = "";
    att.descr.content_id = "";
    att._int.att_type = esp_mail_att_type_none;
    att._int.index = 0;
    att._int.msg_uid = 0;
    att._int.flash_blob = false;
    att._int.binary = false;
    att._int.parallel = false;
    att._int.cid = "";
  }

  void clear()
  {
    sender.name = "";
    sender.email = "";
    subject = "";
    text.charSet = "";
    text.content = "";
    text.content_type = "";
    text.embed.enable = false;
    html.charSet = "";
    html.content = "";
    html.content_type = "";
    html.embed.enable = false;
    response.reply_to = "";
    response.notify = esp_mail_smtp_notify::esp_mail_smtp_notify_never;
    priority = esp_mail_smtp_priority::esp_mail_smtp_priority_normal;

    for (size_t i = 0; i < _rcp.size(); i++)
    {
      _rcp[i].name = "";
      _rcp[i].email = "";
    }

    for (size_t i = 0; i < _cc.size(); i++)
      _cc[i].email = "";

    for (size_t i = 0; i < _bcc.size(); i++)
      _bcc[i].email = "";

    for (size_t i = 0; i < _hdr.size(); i++)
      _hdr[i] = "";

    for (size_t i = 0; i < _att.size(); i++)
    {
      _att[i].descr.filename = "";
      _att[i].blob.data = nullptr;
      _att[i].descr.mime = "";
      _att[i].descr.name = "";
      _att[i].blob.size = 0;
      _att[i].descr.transfer_encoding = "";
      _att[i].file.path = "";
      _att[i].file.storage_type = esp_mail_file_storage_type_none;
    }

    for (size_t i = 0; i < _parallel.size(); i++)
    {
      _parallel[i].descr.filename = "";
      _parallel[i].blob.data = nullptr;
      _parallel[i].descr.mime = "";
      _parallel[i].descr.name = "";
      _parallel[i].blob.size = 0;
      _parallel[i].descr.transfer_encoding = "";
      _parallel[i].file.path = "";
      _parallel[i].file.storage_type = esp_mail_file_storage_type_none;
    }
    _rcp.clear();
    _cc.clear();
    _bcc.clear();
    _hdr.clear();
    _att.clear();
    _parallel.clear();
  }

  /** Clear all the inline images
  */
  void clearInlineimages()
  {
    for (int i = (int)_att.size() - 1; i >= 0; i--)
    {
      if (_att[i]._int.att_type == esp_mail_att_type_inline)
        _att.erase(_att.begin() + i);
    }
  };

  /* Clear all the attachments */
  void clearAttachments()
  {
    for (int i = (int)_att.size() - 1; i >= 0; i--)
    {
      if (_att[i]._int.att_type == esp_mail_att_type_attachment)
        _att.erase(_att.begin() + i);
    }

    for (int i = (int)_parallel.size() - 1; i >= 0; i--)
      _parallel.erase(_parallel.begin() + i);
  };

  /** Clear all rfc822 message attachment
  */
  void clearRFC822Messages()
  {
    for (int i = (int)_rfc822.size() - 1; i >= 0; i--)
    {
      _rfc822[i].clear();
      _rfc822.erase(_rfc822.begin() + i);
    }
  };

  /** Clear the primary recipient mailboxes
  */
  void clearRecipients() { _rcp.clear(); };

  /** Clear the Carbon-copy recipient mailboxes
  */
  void clearCc() { _cc.clear(); };

  /** Clear the Blind-carbon-copy recipient mailboxes
  */
  void clearBcc() { _bcc.clear(); };

  /** Clear the custom message headers
  */
  void clearHeader() { _hdr.clear(); };

  /** Add attachment to the message
   *
   * @param att The SMTP_Attachment data item
  */
  void addAttachment(SMTP_Attachment &att)
  {
    att._int.att_type = esp_mail_att_type_attachment;
    att._int.parallel = false;
    att._int.flash_blob = true;
    _att.push_back(att);
  };

  /** Add parallel attachment to the message
   *
   * @param att The SMTP_Attachment data item
  */
  void addParallelAttachment(SMTP_Attachment &att)
  {
    att._int.att_type = esp_mail_att_type_attachment;
    att._int.parallel = true;
    att._int.flash_blob = true;
    _parallel.push_back(att);
  };

  /** Add inline image to the message
   *
   * @param att The SMTP_Attachment data item
  */
  void addInlineImage(SMTP_Attachment &att)
  {
    att._int.flash_blob = true;
    att._int.parallel = false;
    att._int.att_type = esp_mail_att_type_inline;
    char *tmp = new char[36];
    memset(tmp, 0, 36);
    itoa(random(10000000, 20000000), tmp, 10);
    att._int.cid = tmp;
    delete[] tmp;
    _att.push_back(att);
  };

  /** Add rfc822 message to the message
   *
   * @param msg The RFC822_Message class object
  */
  void addMessage(SMTP_Message &msg) { _rfc822.push_back(msg); }

  /** Add the primary recipient mailbox to the message
   *
   * @param name The name of primary recipient
   * @param email The Email address of primary recipient
  */
  void addRecipient(const char *name, const char *email)
  {
    struct esp_mail_smtp_recipient_t rcp;
    rcp.name = name;
    rcp.email = email;
    _rcp.push_back(rcp);
  };

  /** Add Carbon-copy recipient mailbox
   *
   * @param email The Email address of the secondary recipient
  */
  void addCc(const char *email)
  {
    struct esp_mail_smtp_recipient_address_t cc;
    cc.email = email;
    _cc.push_back(cc);
  };

  /** Add Blind-carbon-copy recipient mailbox
   *
   * @param email The Email address of the tertiary recipient
  */
  void addBcc(const char *email)
  {
    struct esp_mail_smtp_recipient_address_t bcc;
    bcc.email = email;
    _bcc.push_back(bcc);
  };

  /** Add the custom header to the message
   *
   * @param hdr The header name and value
  */
  void addHeader(const char *hdr) { _hdr.push_back(hdr); };

  /* The message author config */
  struct esp_mail_email_info_t sender;

  /* The topic of message */
  const char *subject = "";

  /* The message type */
  byte type = esp_mail_msg_type_none;

  /* The PLAIN text message */
  struct esp_mail_plain_body_t text;

  /* The HTML text message */
  struct esp_mail_html_body_t html;

  /* The response config */
  struct esp_mail_smtp_msg_response_t response;

  /* The priority of the message */
  esp_mail_smtp_priority priority =
      esp_mail_smtp_priority::esp_mail_smtp_priority_normal;

  /* The enable options */
  struct esp_mail_smtp_enable_option_t enable;

  /* The message from config */
  struct esp_mail_email_info_t from;

  /* The message identifier */
  const char *messageID = "";

  /* The keywords or phrases, separated by commas */
  const char *keyword = "";

  /* The comment about message */
  const char *comment = "";

  /* The date of message */
  const char *date = "";

  /* The return recipient of the message */
  const char *return_path = "";

private:
  friend class ESP_Mail_Client;
  std::vector<struct esp_mail_smtp_recipient_t> _rcp =
      std::vector<struct esp_mail_smtp_recipient_t>();
  std::vector<struct esp_mail_smtp_recipient_address_t> _cc =
      std::vector<struct esp_mail_smtp_recipient_address_t>();
  std::vector<struct esp_mail_smtp_recipient_address_t> _bcc =
      std::vector<struct esp_mail_smtp_recipient_address_t>();
  std::vector<const char *> _hdr = std::vector<const char *>();
  std::vector<SMTP_Attachment> _att = std::vector<SMTP_Attachment>();
  std::vector<SMTP_Attachment> _parallel = std::vector<SMTP_Attachment>();
  std::vector<SMTP_Message> _rfc822 = std::vector<SMTP_Message>();
};

class SMTP_Status
{
public:
  friend class SMTPSession;
  friend class ESP_Mail_Client;

  SMTP_Status();
  ~SMTP_Status();
  const char *info();
  bool success();
  void empty();
  size_t completedCount();
  size_t failedCount();

private:
  std::string _info = "";
  bool _success = false;
  size_t _sentSuccess = 0;
  size_t _sentFailed = 0;
};

typedef void (*imapStatusCallback)(IMAP_Status);
typedef void (*smtpStatusCallback)(SMTP_Status);

class ESP_Mail_Client
{

public:
  /** Sending Email through the SMTP server
   *
   * @param smtp The pointer to SMTP session object which holds the data and the
   * TCP client.
   * @param msg The pointer to SMTP_Message class which contains the header,
   * body, and attachments.
   * @param closeSession The option to Close the SMTP session after sent.
   * @return The boolean value indicates the success of operation.
  */
  bool sendMail(SMTPSession *smtp, SMTP_Message *msg, bool closeSession = true);

  /** Reading Email through IMAP server.
   *
   * @param imap The pointer to IMAP sesssion object which holds the data and
   the TCP client.

   * @param closeSession The option to close the IMAP session after fetching or
   searching the Email.
   * @return The boolean value indicates the success of operation.
  */
  bool readMail(IMAPSession *imap, bool closeSession = true);

  /** Set the argument to the Flags for the specified message.
   *
   * @param imap The pointer to IMAP session object which holds the data and the
   * TCP client.
   * @param msgUID The UID of the message.
   * @param flags The flag list to set.
   * @param closeSession The option to close the IMAP session after set flag.
   * @return The boolean value indicates the success of operation.
  */
  bool setFlag(IMAPSession *imap, int msgUID, const char *flags,
               bool closeSession);

  /** Add the argument to the Flags for the specified message.
   *
   * @param imap The pointer to IMAP session object which holds the data and the
   * TCP client.
   * @param msgUID The UID of the message.
   * @param flags The flag list to set.
   * @param closeSession The option to close the IMAP session after add flag.
   * @return The boolean value indicates the success of operation.
  */
  bool addFlag(IMAPSession *imap, int msgUID, const char *flags,
               bool closeSession);

  /** Remove the argument from the Flags for the specified message.
   *
   * @param imap The pointer to IMAP session object which holds the data and the
   * TCP client.
   * @param msgUID The UID of the message that flags to be removed.
   * @param flags The flag list to remove.
   * @param closeSession The option to close the IMAP session after remove flag.
   * @return The boolean value indicates the success of operation.
  */
  bool removeFlag(IMAPSession *imap, int msgUID, const char *flags,
                  bool closeSession);

  /** Initialize the SD card with the SPI port.
   *
   * @param sck The SPI Clock pin (ESP32 only).
   * @param miso The SPI MISO pin (ESSP32 only).
   * @param mosi The SPI MOSI pin (ESP32 only).
   * @param ss The SPI Chip/Slave Select pin (ESP32 and ESP8266).
   * @return The boolean value indicates the success of operation.
  */
  bool sdBegin(uint8_t sck, uint8_t miso, uint8_t mosi, uint8_t ss);

  /** Initialize the SD card with the default SPI port.
   *
   * @return The boolean value which indicates the success of operation.
  */
  bool sdBegin();

  /** Initialize the SD_MMC card (ESSP32 only).
  *
  * @param mountpoint The mounting point.
  * @param mode1bit Allow 1 bit data line.
  * @param format_if_mount_failed Format SD_MMC card if mount failed.
  * @return The boolean value indicates the success of operation.
  */
  bool sdMMCBegin(const char *mountpoint = "/sdcard", bool mode1bit = false, bool format_if_mount_failed = false);


  ESPTimeHelper Time;

private:
  friend class SMTPSession;
  friend class IMAPSession;
#if defined(ESP8266)
  void setClock(float offset);
#endif

  RFC2047_Decoder RFC2047Decoder;
  File file;

  bool _sdOk = false;
  bool _flashOk = false;
  bool _sdConfigSet = false;
  uint8_t _sck, _miso, _mosi, _ss;
  const char *sd_mmc_mountpoint = "";
  bool sd_mmc_mode1bit = false;
  bool sd_mmc_format_if_mount_failed = false;

#if defined(ESP8266)
  bool _clockReady = false;
  uint8_t _sdPin = SD_CS_PIN;
  float _gmtOffset = 0.0;
#endif

  unsigned long _lastReconnectMillis = 0;
  uint16_t _reconnectTimeout = ESP_MAIL_WIFI_RECONNECT_TIMEOUT;

  bool _sendMail(SMTPSession *smtp, SMTP_Message *msg,
                 bool closeSession = true);
  bool ethLinkUp();
  bool reconnect(SMTPSession *smtp, unsigned long dataTime = 0);
  bool reconnect(IMAPSession *imap, unsigned long dataTime = 0,
                 bool downloadRequestuest = false);
  void closeTCP(SMTPSession *smtp);
  void closeTCP(IMAPSession *imap);
  void getMIME(const char *ext, std::string &mime);
  void mimeFromFile(const char *name, std::string &mime);
#if defined(ESP32)
  void setSecure(ESP_Mail_HTTPClient32 &httpClient, ESP_Mail_Session *session,
                 std::shared_ptr<const char> caCert);
#elif defined(ESP8266)
  void setSecure(ESP_Mail_HTTPClient &httpClient, ESP_Mail_Session *session,
                 std::shared_ptr<const char> caCert);
#endif
  void delS(char *p);
  char *newS(size_t len);
  char *newS(char *p, size_t len);
  char *newS(char *p, size_t len, char *d);
  bool strcmpP(const char *buf, int ofs, PGM_P beginH);
  int strposP(const char *buf, PGM_P beginH, int ofs);
  char *strP(PGM_P pgm);
  void appendP(std::string &buf, PGM_P p, bool empty);
  char *intStr(int value);
  void errorStatusCB(SMTPSession *smtp, int error);
  size_t smtpSendP(SMTPSession *smtp, PGM_P v, bool newline = false);
  size_t smtpSend(SMTPSession *smtp, const char *data, bool newline = false);
  size_t smtpSend(SMTPSession *smtp, int data, bool newline = false);
  size_t smtpSend(SMTPSession *smtp, uint8_t *data, size_t size);
  bool getMultipartFechCmd(IMAPSession *imap, int msgIdx,
                           std::string &partText);
  bool multipartMember(const std::string &part, const std::string &check);
  bool fetchMultipartBodyHeader(IMAPSession *imap, int msgIdx);
  bool connected(IMAPSession *imap);
  bool imapAuth(IMAPSession *imap);
  bool sendIMAPCommand(IMAPSession *imap, int msgIndex, int cmdCase);
  bool handleSMTPError(SMTPSession *smtp, int err, bool ret = false);
  void errorStatusCB(IMAPSession *imap, int error);
  size_t imapSendP(IMAPSession *imap, PGM_P v, bool newline = false);
  size_t imapSend(IMAPSession *imap, const char *data, bool nwline = false);
  size_t imapSend(IMAPSession *imap, int data, bool newline = false);
  std::string getBoundary(size_t len);
  std::string getEncodedToken(IMAPSession *imap);
  bool imapLogout(IMAPSession *imap);
  bool sendParallelAttachments(SMTPSession *smtp, SMTP_Message *msg,
                               const std::string &boundary);
  bool sendAttachments(SMTPSession *smtp, SMTP_Message *msg,
                       const std::string &boundary, bool parallel = false);

  bool sendMSGData(SMTPSession *smtp, SMTP_Message *msg, bool closeSession,
                   bool rfc822MSG);
  bool sendRFC822Msg(SMTPSession *smtp, SMTP_Message *msg,
                     const std::string &boundary, bool closeSession,
                     bool rfc822MSG);
  void getRFC822MsgEnvelope(SMTPSession *smtp, SMTP_Message *msg,
                            std::string &buf);
  bool bdat(SMTPSession *smtp, SMTP_Message *msg, int len, bool last);
  void checkBinaryData(SMTPSession *smtp, SMTP_Message *msg);
  bool sendBlob(SMTPSession *smtp, SMTP_Message *msg, SMTP_Attachment *att);
  bool sendFile(SMTPSession *smtp, SMTP_Message *msg, SMTP_Attachment *att,
                File &file);
  bool openFileRead(SMTPSession *smtp, SMTP_Message *msg, SMTP_Attachment *att,
                    File &file, std::string &s, std::string &buf,
                    const std::string &boundary, bool inlined);
  bool openFileRead2(SMTPSession *smtp, SMTP_Message *msg, File &file,
                     const char *path, esp_mail_file_storage_type storageType);
  bool sendInline(SMTPSession *smtp, SMTP_Message *msg,
                  const std::string &boundary, byte type);
  void debugInfoP(PGM_P info);
  size_t numAtt(SMTPSession *smtp, esp_mail_attach_type type,
                SMTP_Message *msg);
  bool validEmail(const char *s);
  bool checkEmail(SMTPSession *smtp, SMTP_Message *msg);
  bool sendPartText(SMTPSession *smtp, SMTP_Message *msg, byte type,
                    const char *boundary);
  char *getUID();
  bool sendBlobBody(SMTPSession *smtp, SMTP_Message *msg, uint8_t type);
  bool sendFileBody(SMTPSession *smtp, SMTP_Message *msg, uint8_t type);
  void encodingText(SMTPSession *smtp, SMTP_Message *msg, uint8_t type,
                    std::string &content);
  void splitTk(std::string &str, std::vector<std::string> &tk,
               const char *delim);
  void formatFlowedText(std::string &content);
  void softBreak(std::string &content, const char *quoteMarks);
  bool sendMSG(SMTPSession *smtp, SMTP_Message *msg,
               const std::string &boundary);
  void getAttachHeader(std::string &header, const std::string &boundary,
                       SMTP_Attachment *attach, size_t size);
  void getRFC822PartHeader(SMTPSession *smtp, std::string &header,
                           const std::string &boundary);
  void getInlineHeader(std::string &header, const std::string &boundary,
                       SMTP_Attachment *inlineAttach, size_t size);
  unsigned char *decodeBase64(const unsigned char *src, size_t len,
                              size_t *out_len);
  std::string encodeBase64Str(const unsigned char *src, size_t len);
  std::string encodeBase64Str(uint8_t *src, size_t len);
  void encodeQP(const char *buf, char *out);
  bool sendBase64(SMTPSession *smtp, SMTP_Message *msg,
                  const unsigned char *data, size_t len, bool flashMem,
                  const char *filename, bool report);
  bool sendBase64Stream(SMTPSession *smtp, SMTP_Message *msg, File file,
                        const char *filename, bool report);
  void smtpCBP(SMTPSession *smtp, PGM_P info, bool success = false);
  void smtpCB(SMTPSession *smtp, const char *info, bool success = false);
  void imapCBP(IMAPSession *imap, PGM_P info, bool success);
  void imapCB(IMAPSession *imap, const char *info, bool success);
  int readLine(WiFiClient *stream, char *buf, int bufLen, bool crlf,
               int &count);
#if defined(ESP32)
  int _readLine(ESP_Mail_WCS32 *stream, char *buf, int bufLen, bool crlf,
                int &count);
#elif defined(ESP8266)
  int _readLine(ESP_Mail::ESP_Mail_WCS *stream, char *buf, int bufLen,
                bool crlf, int &count);
#endif
  int getMSGNUM(IMAPSession *imap, char *buf, int bufLen, int &chunkIdx,
                bool &endSearch, int &nump, const char *key, const char *pc);
  void handleHeader(IMAPSession *imap, char *buf, int bufLen, int &chunkIdx,
                    struct esp_mail_message_header_t &header, int &headerState,
                    int &octetCount);
  void setHeader(IMAPSession *imap, char *buf,
                 struct esp_mail_message_header_t &header, int state);
  void handlePartHeader(IMAPSession *imap, char *buf, int &chunkIdx,
                        struct esp_mail_message_part_info_t &part);
  char *subStr(const char *buf, PGM_P beginH, PGM_P endH, int beginPos,
               int endPos = 0);
  struct esp_mail_message_part_info_t *cPart(IMAPSession *imap);
  struct esp_mail_message_header_t *cHeader(IMAPSession *imap);
  void strcat_c(char *str, char c);
  int strpos(const char *haystack, const char *needle, int offset);
  char *stristr(const char *str1, const char *str2);
  char *rstrstr(const char *haystack, const char *needle);
  int rstrpos(const char *haystack, const char *needle, int offset);
  void getResponseStatus(const char *buf, esp_mail_smtp_status_code respCode,
                         int beginPos,
                         struct esp_mail_smtp_response_status_t &status);
  void handleAuth(SMTPSession *smtp, char *buf);
  std::string getEncodedToken(SMTPSession *smtp);
  bool connected(SMTPSession *smtp);
  bool setSendingResult(SMTPSession *smtp, SMTP_Message *msg, bool result);
  bool smtpAuth(SMTPSession *smtp);
  int available(SMTPSession *smtp);
  bool handleSMTPResponse(SMTPSession *smtp, esp_mail_smtp_status_code respCode,
                          int errCode);
  int available(IMAPSession *imap);
  bool handleIMAPResponse(IMAPSession *imap, int errCode, bool closeSession);
  void downloadReport(IMAPSession *imap, int progress);
  void fetchReport(IMAPSession *imap, int progress, bool html);
  void searchReport(int progress, const char *percent);
  void uploadReport(const char *filename, int progress);
  int cMSG(IMAPSession *imap);
  int cIdx(IMAPSession *imap);
  esp_mail_imap_response_status imapResponseStatus(IMAPSession *imap,
                                                   char *response);
  void saveHeader(IMAPSession *imap);
  esp_mail_char_decoding_scheme getEncodingFromCharset(const char *enc);
  void decodeHeader(std::string &headerField, std::string &headerEnc);
  bool handleAttachment(IMAPSession *imap, char *buf, int bufLen, int &chunkIdx,
                        File &file, std::string &filePath,
                        bool &downloadRequest, int &octetCount,
                        int &octetLength, int &oCount, int &reportState,
                        int &downloadCount);
  int decodeLatin1_UTF8(unsigned char *out, int *outlen,
                        const unsigned char *in, int *inlen);
  void decodeTIS620_UTF8(char *out, const char *in, size_t len);
  void decodeText(IMAPSession *imap, char *buf, int bufLen, int &chunkIdx,
                  File &file, std::string &filePath, bool &downloadRequest,
                  int &octetLength, int &readDataLen, int &readCount);
  void prepareFilePath(IMAPSession *imap, std::string &filePath, bool header);
  int decodeChar(const char *s);
  void decodeQP(const char *buf, char *out);
  char *decode7Bit(char *buf);
  char *strReplace(char *orig, char *rep, char *with);
  char *strReplaceP(char *buf, PGM_P key, PGM_P value);
  bool authFailed(char *buf, int bufLen, int &chunkIdx, int ofs);
  void handleFolders(IMAPSession *imap, char *buf);
  void handleCapability(IMAPSession *imap, char *buf, int &chunkIdx);
  void handleExamine(IMAPSession *imap, char *buf);
  bool handleIMAPError(IMAPSession *imap, int err, bool ret);
  bool _setFlag(IMAPSession *imap, int msgUID, const char *flags,
                uint8_t action, bool closeSession);
  void createDirs(std::string dirs);
  bool sdTest();
};

class IMAPSession
{
public:
  IMAPSession();
  ~IMAPSession();

  /** Begin the IMAP server connection.
   *
   * @param session The pointer to ESP_Mail_Session structured data that keeps
   * the server and log in details.
   * @param config The pointer to IMAP_Config structured data that keeps the
   * operation options.
   * @return The boolean value which indicates the success of operation.
  */
  bool connect(ESP_Mail_Session *session, IMAP_Config *config);

  /** Close the IMAP session.
   *
   * @return The boolean value which indicates the success of operation.
  */
  bool closeSession();

  /** Set to enable the debug.
   *
   * @param level The level to enable the debug message
   * level = 0, no debug
   * level = 1, basic debug
   * level = 2, full debug 1
   * level = 333, full debug 2
  */
  void debug(int level);

  /** Get the list of all the mailbox folders since the TCP session was opened
   * and user was authenticated.
   *
   * @param folders The FoldersCollection class that contains the collection of
   * the
   * FolderInfo structured data.
   * @return The boolean value which indicates the success of operation.
  */
  bool getFolders(FoldersCollection &folders);

  /** Select or open the mailbox folder to search or fetch the message inside.
   *
   * @param folderName The known mailbox folder name. The default name is INBOX.
   * @param readOnly The option to open the mailbox for read only. Set this
   * option to false when you wish
   * to modify the Flags using the setFlag, addFlag and removeFlag functions.
   * @return The boolean value which indicates the success of operation.
  */
  bool selectFolder(const char *folderName, bool readOnly = true);

  /** Open the mailbox folder to read or search the mesages.
   *
   * @param folderName The name of known mailbox folder to be opened.
   * @param readOnly The option to open the mailbox for reading only. Set this
   * option to false when you wish
   * to modify the flags using the setFlag, addFlag and removeFlag functions.
   * @return The boolean value which indicates the success of operation.
  */
  bool openFolder(const char *folderName, bool readOnly = true);

  /** Close the mailbox folder that was opened.
   *
   * @param folderName The known mailbox folder name.
   * @return The boolean value which indicates the success of operation.
  */
  bool closeFolder(const char *folderName);

  /** Create folder.
   *
   * @param folderName The name of folder to create.
   * @return The boolean value which indicates the success of operation.
  */
  bool createFolder(const char *folderName);

  /** Delete folder.
   *
   * @param folderName The name of folder to delete.
   * @return The boolean value which indicates the success of operation.
  */
  bool deleteFolder(const char *folderName);

  /** Copy the messages to the defined mailbox folder.
   *
   * @param toCopy The pointer to the MessageListList class that contains the
   * list of messages to copy.
   * @param dest The destination folder that the messages to copy to.
   * @return The boolean value which indicates the success of operation.
  */
  bool copyMessages(MessageList *toCopy, const char *dest);

  /** Delete the messages in the opened mailbox folder.
   *
   * @param toDelete The pointer to the MessageListList class that contains the
   * list of messages to delete.
   * @param expunge The boolean option to expunge all messages.
   * @return The boolean value which indicates the success of operation.
  */
  bool deleteMessages(MessageList *toDelete, bool expunge = false);

  /** Assign the callback function that returns the operating status when
   * fetching or reading the Email.
   *
   * @param imapCallback The function that accepts the imapStatusCallback as
   * parameter.
  */
  void callback(imapStatusCallback imapCallback);

  /** Determine if no message body contained in the search result and only the
   * message header is available.
  */
  bool headerOnly();

  /** Get the message list from search or fetch the Emails
   *
   * @return The IMAP_MSG_List structured data which contains text and html
   * contents,
   * attachments, inline images, embedded rfc822 messages details for each
   * message.
  */
  IMAP_MSG_List data();

  /** Get the details of the selected or opned mailbox folder
   *
   * @return The SelectedFolderInfo class which contains the info about flags,
   * total messages, next UID,
   * search count and the available messages count.
  */
  SelectedFolderInfo selectedFolder();

  /** Get the error details when readingg the Emails
   *
   * @return The string of error details.
  */
  String errorReason();

  /** Clear all the cache data stored in the IMAP session object.
  */
  void empty();

  friend class ESP_Mail_Client;
  friend class foldderList;

private:
  void clearMessageData();
  void checkUID();
  void checkPath();
  void getMessages(uint16_t messageIndex, struct esp_mail_imap_msg_item_t &msg);
  void getRFC822Messages(uint16_t messageIndex,
                         struct esp_mail_imap_msg_item_t &msg);
  bool closeMailbox();
  bool openMailbox(const char *folder, esp_mail_imap_auth_mode mode,
                   bool waitResponse);
  bool getMailboxes(FoldersCollection &flders);
  bool checkCapability();

  bool _tcpConnected = false;
  struct esp_mail_imap_response_status_t _imapStatus;
  int _cMsgIdx = 0;
  int _cPartIdx = 0;
  int _totalRead = 0;
  std::vector<struct esp_mail_message_header_t> _headers =
      std::vector<struct esp_mail_message_header_t>();

  esp_mail_imap_command _imap_cmd =
      esp_mail_imap_command::esp_mail_imap_cmd_login;
  // std::string _partNumStr = "";
  std::vector<struct esp_mail_imap_multipart_level_t> _multipart_levels =
      std::vector<struct esp_mail_imap_multipart_level_t>();
  int _rfc822_part_count = 0;
  esp_mail_file_storage_type _storageType =
      esp_mail_file_storage_type::esp_mail_file_storage_type_flash;
  bool _unseen = false;
  bool _readOnlyMode = true;
  struct esp_mail_auth_capability_t _auth_capability;
  ESP_Mail_Session *_sesson_cfg;
  std::string _currentFolder = "";
  bool _mailboxOpened = false;
  std::string _nextUID = "";

  struct esp_mail_imap_read_config_t *_config = nullptr;

  bool _headerOnly = true;
  bool _uidSearch = false;
  bool _headerSaved = false;
  bool _debug = false;
  int _debugLevel = 0;
  bool _secure = false;
  imapStatusCallback _readCallback = NULL;

  std::vector<uint32_t> _msgNum = std::vector<uint32_t>();
  FoldersCollection _folders;
  SelectedFolderInfo _mbif;

  int _certType = -1;
  std::shared_ptr<const char> _caCert = nullptr;

#if defined(ESP32)
  ESP_Mail_HTTPClient32 httpClient;
#elif defined(ESP8266)
  ESP_Mail_HTTPClient httpClient;
#endif

  IMAP_Status _cbData;
};

class SendingResult
{
private:
  std::vector<struct esp_mail_smtp_send_status_t> _result =
      std::vector<struct esp_mail_smtp_send_status_t>();
  void add(struct esp_mail_smtp_send_status_t r)
  {
    struct esp_mail_smtp_send_status_t _r = r;
    _result.push_back(_r);
  }
  void clear()
  {
    for (size_t i = 0; i < _result.size(); i++)
    {
      _result[i].recipients = "";
      _result[i].subject = "";
      _result[i].timesstamp = 0;
      _result[i].completed = false;
    }
    _result.clear();
  }

public:
  friend class SMTPSession;
  friend class ESP_Mail_Client;
  SendingResult(){};
  ~SendingResult() { clear(); };
  SMTP_Result getItem(size_t index)
  {
    struct esp_mail_smtp_send_status_t r;
    if (index < _result.size())
      return _result[index];
    return r;
  }
  size_t size() { return _result.size(); };
};

class SMTPSession
{
public:
  SMTPSession();
  ~SMTPSession();

  /** Begin the SMTP server connection.
   *
   * @param session The pointer to ESP_Mail_Session structured data that keeps
   * the server and log in details.
   * @return The boolean value indicates the success of operation.
  */
  bool connect(ESP_Mail_Session *session);

  /** Close the SMTP session.
   *
  */
  bool closeSession();

  /** Set to enable the debug.
   *
   * @param level The level to enable the debug message
   * level = 0, no debug
   * level = 1, basic debug
   * level = 2, full debug 1
   * level = 333, full debug 2
  */
  void debug(int level);

  /** Get the error details when sending the Email
   *
   * @return The string of error details.
  */
  String errorReason();

  /** Set the Email sending status callback function.
   *
   * @param smtpCallback The callback function that accept the
   * smtpStatusCallback param.
  */
  void callback(smtpStatusCallback smtpCallback);

  SendingResult sendingResult;

  friend class ESP_Mail_Client;

private:
  bool _tcpConnected = false;
  struct esp_mail_smtp_response_status_t _smtpStatus;
  int _sentSuccessCount = 0;
  int _sentFailedCount = 0;
  bool _chunkedEnable = false;
  int _chunkCount = 0;

  esp_mail_smtp_command _smtp_cmd =
      esp_mail_smtp_command::esp_mail_smtp_cmd_greeting;
  struct esp_mail_auth_capability_t _auth_capability;
  struct esp_mail_smtp_capability_t _send_capability;
  ESP_Mail_Session *_sesson_cfg = NULL;

  bool _debug = false;
  int _debugLevel = 0;
  bool _secure = false;
  smtpStatusCallback _sendCallback = NULL;

  SMTP_Status _cbData;
  struct esp_mail_smtp_msg_type_t _msgType;

  int _certType = -1;
  std::shared_ptr<const char> _caCert = nullptr;

#if defined(ESP32)
  ESP_Mail_HTTPClient32 httpClient;
#elif defined(ESP8266)
  ESP_Mail_HTTPClient httpClient;
#endif
};


//#define EMAIL_DEBUG_PRINTLN Serial.println
//#define EMAIL_DEBUG_PRINT Serial.print
#define EMAIL_DEBUG_PRINTLN Tasmota_print
#define EMAIL_DEBUG_PRINT Tasmota_print

extern void Tasmota_print(const char *);

static void __attribute__((used)) esp_mail_debug(const char *msg)
{
  delay(0);
  EMAIL_DEBUG_PRINTLN(msg);
}

static void __attribute__((used))
esp_mail_debug_line(const char *msg, bool newline)
{
  delay(0);
  if (newline)
    EMAIL_DEBUG_PRINTLN(msg);
  else
    EMAIL_DEBUG_PRINT(msg);
}

extern ESP_Mail_Client MailClient;

extern FS *ufsp;

#endif // ESP_Mail_Client_H
