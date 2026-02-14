#ifndef ESP_MAIL_CLIENT_H
#define ESP_MAIL_CLIENT_H

#include "ESP_Mail_Client_Version.h"
#if !VALID_VERSION_CHECK(30409)
#error "Mixed versions compilation."
#endif

/**
 * Mail Client Arduino Library for Arduino devices.
 *
 * Created August 28, 2023
 *
 * This library allows Espressif's ESP32, ESP8266, SAMD and RP2040 Pico devices to send and read Email through the SMTP and IMAP servers.
 *
 * The MIT License (MIT)
 * Copyright (c) 2023 K. Suwatchai (Mobizt)
 *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <Arduino.h>
#include "extras/RFC2047.h"
#include <time.h>
#include <ctype.h>

#include <algorithm>
#include <string>
#include <vector>

#if __has_include(<string.h>)
#include <string.h>
#endif

#if __has_include(<stdarg.h>)
#include <stdarg.h>
#endif

#include "ESP_Mail_FS.h"
#include "ESP_Mail_Const.h"
#include "extras/MB_Time.h"

#if defined(MB_ARDUINO_ESP) || defined(MB_ARDUINO_PICO)
#define ESP_MAIL_PRINTF ESP_MAIL_DEFAULT_DEBUG_PORT.printf
#else
#define ESP_MAIL_PRINTF MailClient.printf
#endif

#if defined(ESP32) || defined(ESP8266) || defined(MB_ARDUINO_PICO)

#define UPLOAD_CHUNKS_NUM 12

#if defined(ESP32)

#define ESP_MAIL_MIN_MEM 70000

#elif defined(ESP8266)

#define SD_CS_PIN 15
#define ESP_MAIL_MIN_MEM 8000

#elif defined(MB_ARDUINO_PICO)

#define ESP_MAIL_MIN_MEM 70000
#define SD_CS_PIN PIN_SPI1_SS

#endif

#else

#undef min
#undef max
#define ESP_MAIL_MIN_MEM 3000
#define UPLOAD_CHUNKS_NUM 5

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char *sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif // __arm__

#endif

#include "ESP_Mail_TCPClient.h"

using namespace mb_string;

class IMAPSession;
class SMTPSession;
class SMTP_Status;
class DownloadProgress;
class MessageData;

#if defined(ENABLE_IMAP)

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
  _vectorImpl<int> _list;
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
  size_t flagCount(bool permanent = false) { return permanent ? _permanent_flags.size() : _flags.size(); };

  /* Get the numbers of messages in this mailbox */
  size_t msgCount() { return _msgCount; };

  /* Get the numbers of messages in this mailbox that recent flag was set */
  size_t recentCount() { return _recentCount; };

  /* Get the order of message in number of message in this mailbox that reoved */
  /**
   * The IMAP_Polling_Status has the properties e.g. type, messageNum, and argument.
   *
   * The type property is the type of status e.g.imap_polling_status_type_undefined, imap_polling_status_type_new_message,
   * imap_polling_status_type_remove_message, and imap_polling_status_type_fetch_message.
   *
   * The messageNum property is message number or order from the total number of message that added, fetched or deleted.
   *
   * The argument property is the argument of commands e.g. FETCH
   */
  IMAP_Polling_Status pollingStatus() { return _polling_status; };

  /* Get the The unique identifier (UID) validity value */
  size_t uidValidity() { return _uidValidity; };

  /* Get the predict next message UID */
  size_t nextUID() { return _nextUID; };

  /* Get the index of first unseen message */
  size_t unseenIndex() { return _unseenMsgIndex; };

  /* Get the highest modification sequence */
  uint64_t highestModSeq() { return strtoull(_highestModSeq.c_str(), NULL, 10); };

  /* Check for the modification sequence supports */
  bool modSeqSupported() { return _highestModSeq.length() > 0 && !_nomodsec; };

  /* Get the numbers of messages from search result based on the search criteria
   */
  size_t searchCount() { return _searchCount; };

  /* Get the numbers of messages to be stored in the ressult */
  size_t availableMessages() { return _availableItems; };

  /* Get the flag argument at the specified index */
  String flag(size_t index, bool permanent = false)
  {
    size_t size = permanent ? _permanent_flags.size() : _flags.size();
    if (index < size)
      return permanent ? _permanent_flags[index].c_str() : _flags[index].c_str();
    return "";
  }

private:
  void addFlag(const char *flag, bool permanent)
  {
    MB_String s = flag;
    if (permanent)
      _permanent_flags.push_back(s);
    else
      _flags.push_back(s);
  };
  void clear()
  {
    for (size_t i = 0; i < _flags.size(); i++)
      _flags[i].clear();
    _flags.clear();

    for (size_t i = 0; i < _permanent_flags.size(); i++)
      _permanent_flags[i].clear();
    _permanent_flags.clear();

    _msgCount = 0;
    _polling_status.argument.clear();
    _polling_status.messageNum = 0;
    _polling_status.type = imap_polling_status_type_undefined;
    _idleTimeMs = 0;
    _searchCount = 0;
  }

  size_t _msgCount = 0;
  size_t _recentCount = 0;
  size_t _uidValidity = 0;
  size_t _nextUID = 0;
  size_t _unseenMsgIndex = 0;
  MB_String _highestModSeq;
  size_t _searchCount = 0;
  size_t _availableItems = 0;
  unsigned long _idleTimeMs = 0;
  bool _folderChanged = false;
  bool _floderChangedState = false;
  bool _nomodsec = false;
  IMAP_Polling_Status _polling_status;
  _vectorImpl<MB_String> _flags;
  _vectorImpl<MB_String> _permanent_flags;
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

  esp_mail_folder_info_t operator[](size_t index)
  {
    if (index < _folders.size())
      return _folders[index];

    return esp_mail_folder_info_t();
  }

private:
  void add(struct esp_mail_folder_info_t fd) { _folders.push_back(fd); };
  void clear()
  {
    for (size_t i = 0; i < _folders.size(); i++)
    {
      _folders[i].name.clear();
      _folders[i].attributes.clear();
      _folders[i].delimiter.clear();
    }
    _folders.clear();
  }
  _vectorImpl<esp_mail_folder_info_t> _folders;
};

/* The class that provides the list of IMAP_Quota_Root_Info e.g. resource name, used and limit */
class IMAP_Quota_Roots_List
{
  friend class IMAPSession;

public:
  IMAP_Quota_Roots_List(){};
  ~IMAP_Quota_Roots_List() { clear(); };

  size_t size() { return _quota_roots.size(); };

  IMAP_Quota_Root_Info operator[](size_t index)
  {
    if (index < _quota_roots.size())
      return _quota_roots[index];

    return IMAP_Quota_Root_Info();
  }

private:
  _vectorImpl<IMAP_Quota_Root_Info> _quota_roots;

  void add(IMAP_Quota_Root_Info v)
  {
    _quota_roots.push_back(v);
  }
  void clear()
  {
    _quota_roots.clear();
  }
};

/* The class that provides the list of IMAP_Namespaces */
class IMAP_Namespaces
{
  friend class IMAPSession;

public:
  IMAP_Namespaces(){};
  ~IMAP_Namespaces() { clear(); };

  size_t size() { return _ns_list.size(); };

  IMAP_Namespace_Info operator[](size_t index)
  {
    if (index < _ns_list.size())
      return _ns_list[index];

    return IMAP_Namespace_Info();
  }

private:
  _vectorImpl<IMAP_Namespace_Info> _ns_list;

  void add(IMAP_Namespace_Info v)
  {
    _ns_list.push_back(v);
  }
  void clear()
  {
    _ns_list.clear();
  }
};

/* The class that provides the list of IMAP_Namespaces */
class IMAP_Rights_List
{
  friend class IMAPSession;

public:
  IMAP_Rights_List(){};
  ~IMAP_Rights_List() { clear(); };

  size_t size() { return _rights_list.size(); };

  IMAP_Rights_Info operator[](size_t index)
  {
    if (index < _rights_list.size())
      return _rights_list[index];

    return IMAP_Rights_Info();
  }

private:
  _vectorImpl<IMAP_Rights_Info> _rights_list;

  void add(IMAP_Rights_Info v)
  {
    _rights_list.push_back(v);
  }
  void clear()
  {
    _rights_list.clear();
  }
};

typedef struct esp_mail_imap_nanespace_list_t
{
  IMAP_Namespaces personal_namespaces;
  IMAP_Namespaces other_users_namespaces;
  IMAP_Namespaces shared_namespaces;
} IMAP_Namespaces_List;

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

  MB_String _info;
  bool _success = false;
};

typedef void (*imapStatusCallback)(IMAP_Status);
typedef void (*imapResponseCallback)(IMAP_Response);
typedef void (*MIMEDataStreamCallback)(MIME_Data_Stream_Info);
typedef void (*imapCharacterDecodingCallback)(IMAP_Decoding_Info *);

#else

enum esp_mail_imap_read_capability_types
{
  esp_mail_imap_read_capability_maxType
};

// Dummy class used in template functions (errorStatusCB).
class IMAPSession
{
public:
  struct IMAP_Status
  {
  public:
    const char *info() { return ""; };
    bool success() { return false; };
    void empty();
    MB_String _info;
    bool _success = false;
  };

  typedef void (*imapStatusCallback)(IMAP_Status);
  MB_String errorReason() { return ""; }
  bool _debug;
  imapStatusCallback _statusCallback = nullptr;
  void *_customCmdResCallback = nullptr;
  esp_mail_session_type _sessionType = esp_mail_session_type_imap;

  struct esp_mail_imap_response_status_t
  {
    int errorCode = 0;
    MB_String tag;
    MB_String text;
    MB_String status;
    bool completed = false;
  };

  esp_mail_imap_response_status_t _responseStatus;
  IMAP_Status _cbData;
};
#endif

#if defined(ENABLE_SMTP)

/* The SMTP message class */
class SMTP_Message
{
public:
  SMTP_Message()
  {
    text.content_type = "text/plain";
    html.content_type = "text/html";
  };

  ~SMTP_Message() { clear(); };

  void resetAttachItem(SMTP_Attachment &att)
  {
    att.blob.size = 0;
    att.blob.data = nullptr;
    att.file.path.clear();
    att.file.storage_type = esp_mail_file_storage_type_none;
    att.descr.name.clear();
    att.descr.filename.clear();
    att.descr.transfer_encoding.clear();
    att.descr.content_encoding.clear();
    att.descr.mime.clear();
    att.descr.content_id.clear();
    att._int.att_type = esp_mail_att_type_none;
    att._int.index = 0;
    att._int.msg_uid = 0;
    att._int.flash_blob = false;
    att._int.xencoding = esp_mail_msg_xencoding_none;
    att._int.parallel = false;
    att._int.cid.clear();
  }

  void clear()
  {
    sender.name.clear();
    sender.email.clear();
    subject.clear();
    text.charSet.clear();
    text.content.clear();
    text.content_type.clear();
    text.embed.enable = false;
    html.charSet.clear();
    html.content.clear();
    html.content_type.clear();
    html.embed.enable = false;
    response.reply_to.clear();
    response.notify = esp_mail_smtp_notify::esp_mail_smtp_notify_never;
    priority = esp_mail_smtp_priority::esp_mail_smtp_priority_normal;

    for (size_t i = 0; i < _rcp.size(); i++)
    {
      _rcp[i].name.clear();
      _rcp[i].email.clear();
    }

    for (size_t i = 0; i < _cc.size(); i++)
      _cc[i].email.clear();

    for (size_t i = 0; i < _bcc.size(); i++)
      _bcc[i].email.clear();

    for (size_t i = 0; i < _hdr.size(); i++)
      _hdr[i].clear();

    for (size_t i = 0; i < _att.size(); i++)
    {
      _att[i].descr.filename.clear();
      _att[i].blob.data = nullptr;
      _att[i].descr.mime.clear();
      _att[i].descr.name.clear();
      _att[i].blob.size = 0;
      _att[i].descr.transfer_encoding.clear();
      _att[i].file.path.clear();
      _att[i].file.storage_type = esp_mail_file_storage_type_none;
    }

    for (size_t i = 0; i < _parallel.size(); i++)
    {
      _parallel[i].descr.filename.clear();
      _parallel[i].blob.data = nullptr;
      _parallel[i].descr.mime.clear();
      _parallel[i].descr.name.clear();
      _parallel[i].blob.size = 0;
      _parallel[i].descr.transfer_encoding.clear();
      _parallel[i].file.path.clear();
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
    att._int.cid = random(2000, 4000);
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
  template <typename T1 = const char *, typename T2 = const char *>
  void addRecipient(T1 name, T2 email)
  {
    struct esp_mail_address_info_t rcp;
    rcp.name = toStringPtr(name);
    rcp.email = toStringPtr(email);
    _rcp.push_back(rcp);
  };

  /** Add Carbon-copy recipient mailbox
   *
   * @param email The Email address of the secondary recipient
   */
  template <typename T = const char *>
  void addCc(T email)
  {
    struct esp_mail_address_info_t cc;
    cc.email = toStringPtr(email);
    _cc.push_back(cc);
  };

  /** Add Blind-carbon-copy recipient mailbox
   *
   * @param email The Email address of the tertiary recipient
   */
  template <typename T = const char *>
  void addBcc(T email)
  {
    struct esp_mail_address_info_t bcc;
    bcc.email = toStringPtr(email);
    _bcc.push_back(bcc);
  };

  /** Add the custom header to the message
   *
   * @param hdr The header name and value
   */
  template <typename T = const char *>
  void addHeader(T hdr)
  {
    _hdr.push_back(MB_String().setPtr(toStringPtr(hdr)));
  };

  /* The message author config */
  struct esp_mail_address_info_t author;

  /* The message sender (agent or teansmitter) config */
  struct esp_mail_address_info_t sender;

  /* The topic of message */
  MB_String subject;

  /* The message type */
  byte type = esp_mail_msg_type_none;

  /* The PLAIN text message */
  esp_mail_plain_body_t text;

  /* The HTML text message */
  esp_mail_html_body_t html;

  /* The response config */
  struct esp_mail_smtp_msg_response_t response;

  /* The priority of the message */
  esp_mail_smtp_priority priority = esp_mail_smtp_priority::esp_mail_smtp_priority_normal;

  /* The enable options */
  struct esp_mail_smtp_enable_option_t enable;

  /* The message from config */
  struct esp_mail_address_info_t from;

  /* The message identifier */
  MB_String messageID;

  /* The keywords or phrases, separated by commas */
  MB_String keywords;

  /* The comments about message */
  MB_String comments;

  /* The date of message */
  MB_String date;

  /* The field that contains the parent's message ID of the message to which this one is a reply */
  MB_String in_reply_to;

  /* The field that contains the parent's references (if any) and followed by the parent's message ID (if any) of the message to which this one is a reply */
  MB_String references;
  
  /* The timestamp value to replace in text */
  esp_mail_timestamp_value_t timestamp;

private:
  friend class ESP_Mail_Client;
  _vectorImpl<struct esp_mail_address_info_t> _rcp;
  _vectorImpl<struct esp_mail_address_info_t> _cc;
  _vectorImpl<struct esp_mail_address_info_t> _bcc;
  _vectorImpl<MB_String> _hdr;
  _vectorImpl<SMTP_Attachment> _att;
  _vectorImpl<SMTP_Attachment> _parallel;
  _vectorImpl<SMTP_Message> _rfc822;
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
  MB_String _info;
  bool _success = false;
  size_t _sentSuccess = 0;
  size_t _sentFailed = 0;
};

typedef void (*smtpStatusCallback)(SMTP_Status);
typedef void (*smtpResponseCallback)(SMTP_Response);

#endif

class ESP_Mail_Client
{

public:
  ESP_Mail_Client()
  {
    mbfs = new MB_FS();
  };

  ~ESP_Mail_Client()
  {
    if (mbfs)
      delete mbfs;
    mbfs = nullptr;

    wifi.clearAP();
  };

#if defined(ENABLE_SMTP)
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
#endif

#if defined(ENABLE_SMTP) && defined(ENABLE_IMAP)
  /** Append message to the mailbox
   *
   * @param imap The pointer to IMAP session object which holds the data and the
   * TCP client.
   * @param msg The pointer to SMTP_Message class which contains the header,
   * body, and attachments.
   * @param lastAppend The last message to append (optional). In case of MULTIAPPEND extension
   * is supported, set this to false will append messages in single APPEND command.
   * @param flags The flags to set to this message (optional).
   * @param dateTime The date/time to set to this message (optional).
   * @return The boolean value indicates the success of operation.
   */
  template <typename T1 = const char *, typename T2 = const char *>
  bool appendMessage(IMAPSession *imap, SMTP_Message *msg, bool lastAppend = true, T1 flags = "", T2 dateTime = "") { return mAppendMessage(imap, msg, lastAppend, toStringPtr(flags), toStringPtr(dateTime)); }
#endif

#if defined(ENABLE_IMAP)
  /** Reading Email through IMAP server.
   *
   * @param imap The pointer to IMAP session object which holds the data and
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
   * @param silent The option to ignore the response.
   * @param modsequence The int32_t option for UNCHANGESINCE conditional test.
   * @return The boolean value indicates the success of operation.
   *
   * The modsequence value can be used only if IMAP server supports Conditional STORE extension
   * and the selected mailbox supports modsequences.
   */
  template <typename T = const char *>
  bool setFlag(IMAPSession *imap, int msgUID, T flags, bool closeSession, bool silent = false, int32_t modsequence = -1) { return mSetFlag(imap, toStringPtr(msgUID), toStringPtr(flags), esp_mail_imap_store_flag_type_set, closeSession, silent, true, modsequence); }

  /** Set the argument to the Flags for the specified message.
   *
   * @param imap The pointer to IMAP session object which holds the data and the
   * TCP client.
   * @param sequenceSet The sequence set string i.g., unique identifier (UID) or message sequence number or ranges of UID or sequence number.
   * @param UID The option for sequenceSet whether it is UID or message sequence number.
   * @param flags The flag list to set.
   * @param closeSession The option to close the IMAP session after set flag.
   * @param silent The option to ignore the response.
   * @param modsequence The int32_t option for UNCHANGESINCE conditional test.
   * @return The boolean value indicates the success of operation.
   *
   * The modsequence value can be used only if IMAP server supports Conditional STORE extension
   * and the selected mailbox supports modsequences.
   */
  template <typename T1 = const char *, typename T2 = const char *>
  bool setFlag(IMAPSession *imap, T1 sequenceSet, bool UID, T2 flags, bool closeSession, bool silent = false, int32_t modsequence = -1) { return mSetFlag(imap, toStringPtr(sequenceSet), toStringPtr(flags), esp_mail_imap_store_flag_type_set, closeSession, silent, UID, modsequence); }

  /** Add the argument to the Flags for the specified message.
   *
   * @param imap The pointer to IMAP session object which holds the data and the
   * TCP client.
   * @param msgUID The UID of the message.
   * @param flags The flag list to add.
   * @param closeSession The option to close the IMAP session after add flag.
   * @param silent The option to ignore the response.
   * @param modsequence The int32_t option for UNCHANGESINCE conditional test.
   * @return The boolean value indicates the success of operation.
   *
   * The modsequence value can be used only if IMAP server supports Conditional STORE extension
   * and the selected mailbox supports modsequences.
   */
  template <typename T = const char *>
  bool addFlag(IMAPSession *imap, int msgUID, T flags, bool closeSession, bool silent = false, int32_t modsequence = -1) { return mSetFlag(imap, toStringPtr(msgUID), toStringPtr(flags), esp_mail_imap_store_flag_type_add, closeSession, silent, true, modsequence); }

  /** Add the argument to the Flags for the specified message.
   *
   * @param imap The pointer to IMAP session object which holds the data and the
   * TCP client.
   * @param sequenceSet The sequence set string i.g., unique identifier (UID) or message sequence number or ranges of UID or sequence number.
   * @param UID The option for sequenceSet whether it is UID or message sequence number.
   * @param flags The flag list to add.
   * @param closeSession The option to close the IMAP session after set flag.
   * @param silent The option to ignore the response.
   * @param modsequence The int32_t option for UNCHANGESINCE conditional test.
   * @return The boolean value indicates the success of operation.
   *
   * The modsequence value can be used only if IMAP server supports Conditional STORE extension
   * and the selected mailbox supports modsequences.
   */
  template <typename T1 = const char *, typename T2 = const char *>
  bool addFlag(IMAPSession *imap, T1 sequenceSet, bool UID, T2 flags, bool closeSession, bool silent = false, int32_t modsequence = -1) { return mSetFlag(imap, toStringPtr(sequenceSet), toStringPtr(flags), esp_mail_imap_store_flag_type_add, closeSession, silent, UID, modsequence); }

  /** Remove the argument from the Flags for the specified message.
   *
   * @param imap The pointer to IMAP session object which holds the data and the
   * TCP client.
   * @param msgUID The UID of the message that flags to be removed.
   * @param flags The flag list to remove.
   * @param closeSession The option to close the IMAP session after remove flag.
   * @param silent The option to ignore the response.
   * @param modsequence The int32_t option for UNCHANGESINCE conditional test.
   * @return The boolean value indicates the success of operation.
   *
   * The modsequence value can be used only if IMAP server supports Conditional STORE extension
   * and the selected mailbox supports modsequences.
   */
  template <typename T = const char *>
  bool removeFlag(IMAPSession *imap, int msgUID, T flags, bool closeSession, bool silent = false, int32_t modsequence = -1) { return mSetFlag(imap, toStringPtr(msgUID), toStringPtr(flags), esp_mail_imap_store_flag_type_remove, closeSession, silent, true, modsequence); }

  /** Remove the argument from the Flags for the specified message.
   *
   * @param imap The pointer to IMAP session object which holds the data and the
   * TCP client.
   * @param sequenceSet The sequence set string i.g., unique identifier (UID) or message sequence number or ranges of UID or sequence number.
   * @param UID The option for sequenceSet whether it is UID or message sequence number.
   * @param flags The flag list to remove.
   * @param closeSession The option to close the IMAP session after set flag.
   * @param silent The option to ignore the response.
   * @param modsequence The int32_t option for UNCHANGESINCE conditional test.
   * @return The boolean value indicates the success of operation.
   *
   * The modsequence value can be used only if IMAP server supports Conditional STORE extension
   * and the selected mailbox supports modsequences.
   */
  template <typename T1 = const char *, typename T2 = const char *>
  bool removeFlag(IMAPSession *imap, T1 sequenceSet, bool UID, T2 flags, bool closeSession, bool silent = false, int32_t modsequence = -1) { return mSetFlag(imap, toStringPtr(sequenceSet), toStringPtr(flags), esp_mail_imap_store_flag_type_remove, closeSession, silent, UID, modsequence); }

#endif

  /** Reconnect WiFi or network if lost connection.
   *
   * @param reconnect The boolean to set/unset WiFi AP reconnection.
   */
  void networkReconnect(bool reconnect);

  /* Obsoleted */
  void setUDPClient(void *client, float gmtOffset) {}

  /** Clear all WiFi access points assigned.
   *
   */
  void clearAP();

  /** Add WiFi access point for non-ESP device to resume WiFi connection.
   *
   * @param ssid The WiFi SSID.
   * @param password The WiFi password.
   */
  void addAP(const String &ssid, const String &password);

  /**
   * Formatted printing on debug port.
   *
   */
  void printf(const char *format, ...);

#if defined(MBFS_SD_FS) && defined(MBFS_CARD_TYPE_SD)

  /** Initiate SD card with SPI port configuration.
   *
   * @param ss SPI Chip/Slave Select pin.
   * @param sck SPI Clock pin.
   * @param miso SPI MISO pin.
   * @param mosi SPI MOSI pin.
   * @param frequency The SPI frequency
   * @return Boolean type status indicates the success of the operation.
   */
  bool sdBegin(int8_t ss = -1, int8_t sck = -1, int8_t miso = -1, int8_t mosi = -1, uint32_t frequency = 4000000);

#if defined(ESP8266) || defined(MB_ARDUINO_PICO)

  /** Initiate SD card with SD FS configurations (ESP8266 only).
   *
   * @param ss SPI Chip/Slave Select pin.
   * @param sdFSConfig The pointer to SDFSConfig object (ESP8266 only).
   * @return Boolean type status indicates the success of the operation.
   */
  bool sdBegin(SDFSConfig *sdFSConfig);

#endif

#if defined(ESP32)
  /** Initiate SD card with chip select and SPI configuration (ESP32 only).
   *
   * @param ss SPI Chip/Slave Select pin.
   * @param spiConfig The pointer to SPIClass object for SPI configuartion.
   * @param frequency The SPI frequency.
   * @return Boolean type status indicates the success of the operation.
   */
  bool sdBegin(int8_t ss, SPIClass *spiConfig = nullptr, uint32_t frequency = 4000000);
#endif

#if defined(MBFS_ESP32_SDFAT_ENABLED) || defined(MBFS_SDFAT_ENABLED)
  /** Initiate SD card with SdFat SPI and pins configurations (with SdFat included only).
   *
   * @param sdFatSPIConfig The pointer to SdSpiConfig object for SdFat SPI configuration.
   * @param ss SPI Chip/Slave Select pin.
   * @param sck SPI Clock pin.
   * @param miso SPI MISO pin.
   * @param mosi SPI MOSI pin.
   * @return Boolean type status indicates the success of the operation.
   */
  bool sdBegin(SdSpiConfig *sdFatSPIConfig, int8_t ss = -1, int8_t sck = -1, int8_t miso = -1, int8_t mosi = -1);

  /** Initiate SD card with SdFat SDIO configuration (with SdFat included only).
   *
   * @param sdFatSDIOConfig The pointer to SdioConfig object for SdFat SDIO configuration.
   * @return Boolean type status indicates the success of the operation.
   */
  bool sdBegin(SdioConfig *sdFatSDIOConfig);

#endif

#endif

#if defined(ESP32) && defined(MBFS_SD_FS) && defined(MBFS_CARD_TYPE_SD_MMC)
  /** Initialize the SD_MMC card (ESP32 only).
   *
   * @param mountpoint The mounting point.
   * @param mode1bit Allow 1 bit data line (SPI mode).
   * @param format_if_mount_failed Format SD_MMC card if mount failed.
   * @return The boolean value indicates the success of operation.
   */
  bool sdMMCBegin(const char *mountpoint = "/sdcard", bool mode1bit = false, bool format_if_mount_failed = false);
#endif

  /** Get free Heap memory.
   *
   * @return Free memory amount in byte.
   */
  int getFreeHeap();

  /** Get base64 encode string.
   *
   * @return String of base64 encoded string.
   */
  template <typename T = const char *>
  String toBase64(T str) { return mGetBase64(toStringPtr(str)).c_str(); }

  MB_Time Time;

#if defined(ENABLE_IMAP)

  // Get encoding type from character set string
  esp_mail_char_decoding_scheme getEncodingFromCharset(const char *enc);

  // Decode Latin1 to UTF-8
  int decodeLatin1_UTF8(unsigned char *out, int *outlen, const unsigned char *in, int *inlen);

  // Decode TIS620 to UTF-8
  void decodeTIS620_UTF8(char *out, const char *in, size_t len);

  // handle rfc2047 Q (quoted printable) and B (base64) decodings
  RFC2047_Decoder RFC2047Decoder;

#endif

private:
  friend class SMTPSession;
  friend class IMAPSession;

  MB_FS *mbfs = nullptr;
  bool timeStatus = false;
  time_t ts = 0;
  bool networkAutoReconnect = true;
  volatile bool networkStatus = false;
  esp_mail_wifi_credentials_t wifi;
  bool timezoneEnvSet = false;

  IMAPSession *imap = nullptr;
  bool calDataLen = false;
  uint32_t dataLen = 0;
  uint32_t imap_ts = 0;

#if defined(ENABLE_SMTP) || defined(ENABLE_IMAP)

  unsigned long _lastReconnectMillis = 0;
  uint16_t _reconnectTimeout = ESP_MAIL_NETWORK_RECONNECT_TIMEOUT;

  // Resume network connection
  void resumeNetwork(ESP_Mail_TCPClient *client);

  // Get the CRLF ending string w/wo CRLF included. Return the size of string read and the current octet read.
  int readLine(ESP_Mail_TCPClient *client, char *buf, int bufLen, bool withLineBreak, int &count, bool &ovf, unsigned long timeoutSec, bool &isTimeout);

  // readLine with overflow handling.
  template <class T>
  bool readResponse(T sessionPtr, char *buf, int bufLen, int &readLen, bool withLineBreak, int &count, MB_String &ovfBuf);

  // Network reconnection and return the connection status
  template <class T>
  bool reconnect(T sessionPtr, unsigned long dataTime = 0, bool downloadRequest = false);

  // Send callback
  template <class T>
  void sendCB(T sessionPtr, PGM_P info = "", bool prependCRLF = false, bool success = false);

  // PGM string replacement
  void strReplaceP(MB_String &buf, PGM_P key, PGM_P value);

  // Check for OAuth log in error response
  bool isOAuthError(char *buf, int bufLen, int &chunkIdx, int ofs);

  // Get SASL XOAUTH2 string
  MB_String getXOAUTH2String(const MB_String &email, const MB_String &accessToken);

  // Send error callback
  template <class T>
  void sendErrorCB(T sessionPtr, PGM_P info, bool prependCRLF = false, bool success = false);

  // Send the error status callback
  template <class T1, class T2>
  void errorStatusCB(T1 sessionPtr, T2 sessionPtr2, int error, bool clearLastStatus);

  // Check response callback was assigned?
  template <class T>
  bool isResponseCB(T sessionPtr);

  // Print library info
  template <class T>
  void printLibInfo(T sessionPtr);

  // Begin server connection
  template <class T>
  bool beginConnection(Session_Config *session_config, T sessionPtr, bool secureMode);

  // Prepare system time
  template <class T>
  bool prepareTime(Session_Config *session_config, T sessionPtr);

  // Check for session. Close session If not ready.
  template <class T>
  bool sessionReady(T sessionPtr);

  // Set cert data
  void setCert(Session_Config *session_cfg, const char *ca);
  // Set secure data
  void setSecure(ESP_Mail_TCPClient &client, Session_Config *session_config);

  void appendMultipartContentType(MB_String &buf, esp_mail_multipart_types type, const char *boundary);

  String errorReason(bool isSMTP, int errorCode, const char *msg);

  // Close TCP session and clear auth_capability, read/send_capability, connected and authenticate statuses
  template <class T>
  void closeTCPSession(T sessionPtr);

  // Get and set timezone
  void getSetTimezoneEnv(const char *TZ_file, const char *TZ_Var);

  // Get TCP connected status
  template <class T>
  bool connected(T sessionPtr);

  // Get the memory allocation block size of multiple of 4
  size_t getReservedLen(size_t len);

  // Check Email for valid format
  bool validEmail(const char *s);

  // Get random UID for SMTP content ID and IMAP attachment default file name
  char *getRandomUID();

  // Spit the string into token strings
  void splitToken(const char *str, _vectorImpl<MB_String> &tk, const char *delim);

  // Decode base64 encoded string
  unsigned char *decodeBase64(const unsigned char *src, size_t len, size_t *out_len);

  // Decode base64 encoded string
  MB_String encodeBase64Str(const unsigned char *src, size_t len);

  // Decode base64 encoded string
  MB_String encodeBase64Str(uint8_t *src, size_t len);

  // Decode base64 encoded string
  MB_String mGetBase64(MB_StringPtr str);

  // Sub string
  char *subStr(const char *buf, PGM_P beginToken, PGM_P endToken, int beginPos, int endPos = 0, bool caseSensitive = true);

  // Find string
  int strpos(const char *haystack, const char *needle, int offset, bool caseSensitive = true);

  // Memory allocation
  template <typename T>
  T allocMem(size_t size, bool clear = true);

  // Memory deallocation
  void freeMem(void *ptr);

  // PGM string compare
  bool strcmpP(const char *buf, int ofs, PGM_P beginToken, bool caseSensitive = true);

  // Find PGM string
  int strposP(const char *buf, PGM_P beginToken, int ofs, bool caseSensitive = true);

  // Memory allocation for PGM string
  char *strP(PGM_P pgm);

  // Memory allocation for PGM lower case string
  char *strP2Lower(PGM_P pgm);

  // Set or sync device system time with NTP server
  // Do not modify or remove
  void setTime(const char *TZ_Var, const char *TZ_file, bool wait, bool debugProgress);

  // Set the device time zone via TZ environment variable
  void setTimezone(const char *TZ_Var, const char *TZ_file);

  // Get TZ environment variable from file
  // Do not modify or remove
  void getTimezone(const char *TZ_file, MB_String &out);

  // Check the session existent
  template <class T>
  bool sessionExisted(T sessionPtr);

  // Send SMTP/IMAP callback
  template <class T>
  void sendCallback(T sessionPtr, PGM_P info, bool prependCRLF, bool success);

  // Send IMAP/SMTP response callback and print debug message
  template <class T>
  void printDebug(T sessionPtr, PGM_P cbMsg, PGM_P dbMsg, esp_mail_debug_tag_type type, bool prependCRLF, bool success);

  // Get header content from response based on the field name
  bool getHeader(const char *buf, PGM_P beginToken, MB_String &out, bool caseSensitive);

  // Append header field to buffer
  void appendHeaderField(MB_String &buf, const char *name, PGM_P value, bool comma, bool newLine, esp_mail_string_mark_type type = esp_mail_string_mark_type_none);

  // Append SMTP address header field
  void appendAddressHeaderField(MB_String &buf, esp_mail_address_info_t &source, esp_mail_rfc822_header_field_types type, bool header, bool comma, bool newLine);

  // Append header field name to buffer
  void appendHeaderName(MB_String &buf, const char *name, bool clear = false, bool lowercase = false, bool space = true);

  // Append lowercase string to buffer
  void appendLowerCaseString(MB_String &buf, PGM_P value, bool clear = false);

  // Append header field property to buffer
  void appendHeaderProp(MB_String &buf, PGM_P prop, const char *value, bool &firstProp, bool lowerCase, bool isString, bool newLine);

  // Append quote string to buffer
  void appendString(MB_String &buf, PGM_P value, bool comma, bool newLine, esp_mail_string_mark_type type = esp_mail_string_mark_type_none);

  // Append list to buffer
  template <class T>
  void appendList(MB_String &buf, _vectorImpl<T> &list);

  // Append space to buffer
  void appendSpace(MB_String &buf);

  // Append space to buffer after value
  void appendSpace(MB_String &buf, bool withTag, PGM_P value);

  // Append space to buffer after values
  void appendSpace(MB_String &buf, bool withTag, int nunArgs, ...);

  // Append space to buffer before value
  void prependSpace(MB_String &buf, PGM_P value);

  // Append dot to buffer
  void appendDot(MB_String &buf);

  // Append IMAP string key value list
  void appendImap4KeyValue(MB_String &buf, PGM_P key, PGM_P value);

  // Append dot to buffer before value
  void prependDot(MB_String &buf, PGM_P value);

  // Join 2 strings to buffer with space
  void joinStringSpace(MB_String &buf, bool withTag, int nunArgs, ...);

  // Join 2 strings to buffer with dot
  void joinStringDot(MB_String &buf, int nunArgs, ...);

  // Append mask(*) string to buffer
  void maskString(MB_String &buf, int len);

  // Append domain to buffer
  void appendDomain(MB_String &buf, const char *domain);

  // Append embedded message header to buffer
  void appendEmbedMessage(MB_String &buf, esp_mail_message_body_t &body, bool isHtml);

  // Append crlf to buffer
  void appendNewline(MB_String &buf);

  // Append tag to the buffer
  void appendTagSpace(MB_String &buf, PGM_P tag = NULL);

  // Print newline
  void debugPrintNewLine();

  // Send newline to callback
  template <class T>
  void callBackSendNewLine(T sessionPtr, bool success);

  // Print progress bar
  void printProgress(int progress, int &lastProgress);

  // Get file extension with dot from MIME string
  void getExtfromMIME(const char *mime, MB_String &ext);

  // Prepare ports
  void preparePortFunction(Session_Config *session_config, bool isSMTP, bool &secure, bool &secureMode, bool &ssl);

  // Get operation config based on port and its protocol
  void getPortFunction(uint16_t port, struct esp_mail_ports_functions &ports_functions, bool &secure, bool &secureMode, bool &ssl, bool &starttls);

#endif

#if defined(ENABLE_SMTP)

  // Encode Quoted Printable string
  void encodeQP(const char *buf, char *out);

  // Add the soft line break to the long text line rfc 3676
  void formatFlowedText(MB_String &content);

  // Insert soft break
  void softBreak(MB_String &content, const char *quoteMarks);

  // Get content type (MIME) from file extension
  void getMIME(const char *ext, MB_String &mime);

  // Get content type (MIME) from file name
  void mimeFromFile(const char *name, MB_String &mime);

  // Get MIME boundary string
  MB_String getMIMEBoundary(size_t len);

  // Send Email function
  bool mSendMail(SMTPSession *smtp, SMTP_Message *msg, bool closeSession = true);

  // SMTP send data
  size_t smtpSend(SMTPSession *smtp, PGM_P data, bool newline = false);

  // SMTP send data
  size_t smtpSend(SMTPSession *smtp, int data, bool newline = false);

  // SMTP send data
  size_t smtpSend(SMTPSession *smtp, uint8_t *data, size_t size);

  // Handle the error by sending callback and close session
  bool handleSMTPError(SMTPSession *smtp, int err, bool ret = false);

  // Send parallel attachment RFC1521
  bool sendParallelAttachments(SMTPSession *smtp, SMTP_Message *msg, const MB_String &boundary);

  // Send attachment
  bool sendAttachments(SMTPSession *smtp, SMTP_Message *msg, const MB_String &boundary, bool parallel = false);

  // Send message content
  bool sendContent(SMTPSession *smtp, SMTP_Message *msg, bool closeSession, bool rfc822MSG);

  // Send imap or smtp callback
  void altSendCallback(SMTPSession *smtp, PGM_P cbMsg, PGM_P dbMsg, esp_mail_debug_tag_type type, bool prependCRLF, bool success);

  // Send message data
  bool sendMSGData(SMTPSession *smtp, SMTP_Message *msg, bool closeSession, bool rfc822MSG);

  // Send RFC 822 message
  bool sendRFC822Msg(SMTPSession *smtp, SMTP_Message *msg, const MB_String &boundary, bool closeSession, bool rfc822MSG);

  // Get RFC 822 message envelope
  void getRFC822MsgEnvelope(SMTPSession *smtp, SMTP_Message *msg, MB_String &buf);

  // Append boundary string to buffer
  void appendBoundaryString(MB_String &buf, const char *value, bool endMark, bool newLine);

  // Send BDAT command RFC 3030
  bool sendBDAT(SMTPSession *smtp, SMTP_Message *msg, int len, bool last);

  // Get transfer encoding
  void getXEncoding(esp_mail_msg_xencoding &xencoding, const char *enc);

  // Set the unencoded xencoding enum for html, text and attachment from its xencoding string
  void checkUnencodedData(SMTPSession *smtp, SMTP_Message *msg);

  // Check imap or smtp has callback set
  bool altIsCB(SMTPSession *smtp);

  // Check imap or smtp has debug set
  bool altIsDebug(SMTPSession *smtp);

  // Send BLOB attachment
  bool sendBlobAttachment(SMTPSession *smtp, SMTP_Message *msg, SMTP_Attachment *att);

  // Send file content
  bool sendFile(SMTPSession *smtp, SMTP_Message *msg, SMTP_Attachment *att);

  // Send imap or smtp storage error callback
  void altSendStorageErrorCB(SMTPSession *smtp, int err);

  // Open file to send an attachment
  bool openFileRead(SMTPSession *smtp, SMTP_Message *msg, SMTP_Attachment *att, MB_String &buf, const MB_String &boundary, bool inlined);

  // Open text file or html file for to send message
  bool openFileRead2(SMTPSession *smtp, SMTP_Message *msg, const char *path, esp_mail_file_storage_type storageType);

  // Send inline attachments
  bool sendInline(SMTPSession *smtp, SMTP_Message *msg, const MB_String &boundary, byte type);

  // Send storage error callback
  void sendStorageNotReadyError(SMTPSession *smtp, esp_mail_file_storage_type storageType);

  // Append message
  bool mAppendMessage(IMAPSession *imap, SMTP_Message *msg, bool lastAppend, MB_StringPtr flags, MB_StringPtr dateTime);

  // Get numbers of attachment based on type
  size_t numAtt(SMTPSession *smtp, esp_mail_attach_type type, SMTP_Message *msg);

  // Check for valid recipient Email
  bool checkEmail(SMTPSession *smtp, SMTP_Message *msg);

  // Send text parts MIME message
  bool sendPartText(SMTPSession *smtp, SMTP_Message *msg, byte type, const char *boundary);

  // Alternative string data sending to send imap APPEND data or smtp data
  bool altSendData(MB_String &s, bool newLine, SMTPSession *smtp, SMTP_Message *msg, bool addSendResult, bool getResponse, esp_mail_smtp_command cmd, esp_mail_smtp_status_code statusCode, int errCode);

  // Alternative bytes data sending to send imap APPEND data or smtp data
  bool altSendData(uint8_t *data, size_t size, SMTPSession *smtp, SMTP_Message *msg, bool addSendResult, bool getResponse, esp_mail_smtp_command cmd, esp_mail_smtp_status_code statusCode, int errCode);

  // Send MIME message
  bool sendMSG(SMTPSession *smtp, SMTP_Message *msg, const MB_String &boundary);

  // Get an attachment part header string
  void getAttachHeader(MB_String &header, const MB_String &boundary, SMTP_Attachment *attach, size_t size, bool isInline);

  // Get RFC 8222 part header string
  void getRFC822PartHeader(SMTPSession *smtp, MB_String &header, const MB_String &boundary);

  // Send BLOB type text part or html part MIME message
  bool sendBlobBody(SMTPSession *smtp, SMTP_Message *msg, uint8_t type);

  // Send file type text part or html part MIME message
  bool sendFileBody(SMTPSession *smtp, SMTP_Message *msg, uint8_t type);

  // Base64 and QP encodings for text and html messages and replace embeded attachment file name with content ID
  void encodingText(SMTPSession *smtp, SMTP_Message *msg, uint8_t type, MB_String &content);

  // Blob or Stream available
  int chunkAvailable(SMTPSession *smtp, esp_mail_smtp_send_base64_data_info_t &data_info);

  // Read chunk data of blob or file
  int getChunk(SMTPSession *smtp, esp_mail_smtp_send_base64_data_info_t &data_info, unsigned char *rawChunk, bool base64);

  // Terminate chunk reading
  void closeChunk(esp_mail_smtp_send_base64_data_info_t &data_info);

  // Get base64 encoded buffer or raw buffer
  void getBuffer(bool base64, uint8_t *out, uint8_t *in, int &encodedCount, int &bufIndex, bool &dataReady, int &size, size_t chunkSize);

  // Send blob or file as base64 encoded chunk
  bool sendBase64(SMTPSession *smtp, SMTP_Message *msg, esp_mail_smtp_send_base64_data_info_t &data_info, bool base64, bool report);

  // Save sending logs to file
  void saveSendingLogs(SMTPSession *smtp, SMTP_Message *msg, bool result);

  // Get imap or smtp report progress var pointer
  uint32_t altProgressPtr(SMTPSession *smtp);

  // Get SMTP response status (statusCode and text)
  void getResponseStatus(const char *buf, esp_mail_smtp_status_code statusCode, int beginPos, struct esp_mail_smtp_response_status_t &status);

  // Parse SMTP authentication capability
  void parseAuthCapability(SMTPSession *smtp, char *buf);

  // Add the sending result
  bool addSendingResult(SMTPSession *smtp, SMTP_Message *msg, bool result, bool showResult);

  // Handle SMTP server authentication
  bool smtpAuth(SMTPSession *smtp, bool &ssl);

  // Handle SMTP response
  bool handleSMTPResponse(SMTPSession *smtp, esp_mail_smtp_command cmd, esp_mail_smtp_status_code statusCode, int errCode);

  // Print the upload status to the debug port
  void uploadReport(const char *filename, uint32_t pgAddr, int progress);

  // Get MB_FS object pointer
  MB_FS *getMBFS();

  // Set device system time
  int setTimestamp(time_t ts);
#endif

#if defined(ENABLE_IMAP)

  // Check if child part (part number string) is a member of the parent part (part number string)
  // part number string format: <part number>.<sub part number>.<sub part number>
  bool multipartMember(const MB_String &parent, const MB_String &child);

  // Decode string
  int decodeChar(const char *s);

  // Decode Quoted Printable string
  void decodeQP_UTF8(const char *buf, char *out);

  // Actually not decode because 7bit string is enencode string unless prepare valid 7bit string and do qp decoding
  char *decode7Bit_UTF8(char *buf);

  // Actually not decode because 8bit string is enencode string unless prepare valid 8bit string
  char *decode8Bit_UTF8(char *buf);

  // Decode string base on encoding
  void decodeString(IMAPSession *imap, MB_String &string, const char *enc = "");

  /**
   * Encode a code point using UTF-8
   *
   * @author Ondřej Hruška <ondra@ondrovo.com>
   * https://gist.github.com/MightyPork/52eda3e5677b4b03524e40c9f0ab1da5
   *
   * @license MIT
   *
   * @param out - output buffer (min 5 characters), will be 0-terminated
   * @param utf - code point 0-0x10FFFF
   * @return number of bytes on success, 0 on failure (also produces U+FFFD, which uses 3 bytes)
   */
  int encodeUnicode_UTF8(char *out, uint32_t utf);

  // Append headers fetch command
  void appendHeadersFetchCommand(IMAPSession *imap, MB_String &cmd, int index, bool debug);

  // Append rfc822 headers fetch command
  void appendRFC822HeadersFetchCommand(MB_String &cmd);

  // Get multipart MIME fetch command
  bool getMultipartFechCmd(IMAPSession *imap, int msgIdx, MB_String &partText);

  // Fetch multipart MIME body header
  bool fetchMultipartBodyHeader(IMAPSession *imap, int msgIdx);

  // Print body part fetching debug
  void printBodyPartFechingDubug(IMAPSession *imap, const char *partNum, bool multiLevel);

  // Handle IMAP server authentication
  bool imapAuth(IMAPSession *imap, bool &ssl);

  // Send IMAP command
  bool sendFetchCommand(IMAPSession *imap, int msgIndex, esp_mail_imap_command cmdCase);

  // Send data
  size_t imapSend(IMAPSession *imap, PGM_P data, bool newline = false);

  // Send data
  size_t imapSend(IMAPSession *imap, int data, bool newline = false);

  // Send data
  size_t imapSend(IMAPSession *imap, uint8_t *data, size_t size);

  // Log out
  bool imapLogout(IMAPSession *imap);

  // Send storage error callback
  void sendStorageNotReadyError(IMAPSession *imap, esp_mail_file_storage_type storageType);

  // Parse search response
  int parseSearchResponse(IMAPSession *imap, esp_mail_imap_response_data &res, PGM_P tag, const char *key);

  // Parse header state
  bool parseHeaderField(IMAPSession *imap, const char *buf, PGM_P beginToken, bool caseSensitive, struct esp_mail_message_header_t &header, int &headerState, int state);

  // Parse header response
  void parseHeaderResponse(IMAPSession *imap, esp_mail_imap_response_data &res, bool caseSensitive = true);

  // Set the header based on state parsed
  void collectHeaderField(IMAPSession *imap, char *buf, struct esp_mail_message_header_t &header, int state);

  // Get decoded header
  bool getDecodedHeader(IMAPSession *imap, const char *buf, PGM_P beginToken, MB_String &out, bool caseSensitive);

  // Check attachment for firmware file
  void checkFirmwareFile(IMAPSession *imap, const char *filename, struct esp_mail_message_part_info_t &part, bool defaultSize = false);

  // Parse part header response
  void parsePartHeaderResponse(IMAPSession *imap, esp_mail_imap_response_data &res, bool caseSensitive = true);

  // Count char in string
  int countChar(const char *buf, char find);

  // Store the value to string via its the pointer
  bool storeStringPtr(IMAPSession *imap, uint32_t addr, MB_String &value, const char *buf);

  // Get part header properties
  bool getPartHeaderProperties(IMAPSession *imap, const char *buf, PGM_P p, PGM_P e, bool num, MB_String &value, MB_String &old_value, esp_mail_char_decoding_scheme &scheme, bool caseSensitive);

  // Url decode for UTF-8 encoded header text
  char *urlDecode(const char *str);

  // Reset the pointer to multiline response keeping string
  void resetStringPtr(struct esp_mail_message_part_info_t &part);

  // Get current part
  struct esp_mail_message_part_info_t *cPart(IMAPSession *imap);

  // Get current header
  struct esp_mail_message_header_t *cHeader(IMAPSession *imap);

#if !defined(MB_USE_STD_VECTOR)
  // Decending sort
  void numDecSort(_vectorImpl<struct esp_mail_imap_msg_num_t> &arr);
#endif

  // Handle IMAP response
  bool handleIMAPResponse(IMAPSession *imap, int errCode, bool closeSession);

  // Print the file download status via debug port
  void downloadReport(IMAPSession *imap, int progress);

  // Print the message fetch status via debug port
  void fetchReport(IMAPSession *imap, int progress, bool html);

  // Print the message search status via debug port
  void searchReport(IMAPSession *imap, int progress);

  // Get current message num item
  struct esp_mail_imap_msg_num_t cMSG(IMAPSession *imap);

  // Get current message Index
  int cIdx(IMAPSession *imap);

  // Get IMAP response status e.g. OK, NO and Bad status enum value
  esp_mail_imap_response_status imapResponseStatus(IMAPSession *imap, char *response, PGM_P tag);

  // Add header item to string buffer
  void addHeaderItem(MB_String &str, esp_mail_message_header_t *header, bool json);

  // Get RFC822 header string pointer by index
  int getRFC822HeaderPtr(int index, esp_mail_imap_rfc822_msg_header_item_t *header);

  // Add RFC822 headers to string buffer
  void addRFC822Headers(MB_String &s, esp_mail_imap_rfc822_msg_header_item_t *header, bool json);

  // Add RFC822 header item to string buffer
  void addRFC822HeaderItem(MB_String &s, esp_mail_imap_rfc822_msg_header_item_t *header, int index, bool json);

  // Add header string by name and value to string buffer
  void addHeader(MB_String &s, const char *name, const char *s_value, int num_value, bool trim, bool isJson);

  // Save header string buffer to file
  void saveHeader(IMAPSession *imap, bool json);

  // Send MIME stream to callback
  void sendStreamCB(IMAPSession *imap, void *buf, size_t len, int chunkIndex, bool hrdBrk);

  // Prepare file path for saving
  void prepareFilePath(IMAPSession *imap, MB_String &filePath, bool header);

  // Decode text and store it to buffer or file
  void decodeText(IMAPSession *imap, esp_mail_imap_response_data &res);

  // Handle atachment parsing and download
  bool parseAttachmentResponse(IMAPSession *imap, char *buf, esp_mail_imap_response_data &res);

  // Get List
  char *getList(char *buf, bool &isList);

  // Parse mailbox folder open response
  void parseFoldersResponse(IMAPSession *imap, char *buf, bool list);

  // Prepare alias (short name) file list for unsupported long file name filesystem
  void prepareFileList(IMAPSession *imap, MB_String &filePath);

  // Parse capability response
  bool parseCapabilityResponse(IMAPSession *imap, const char *buf, int &chunkIdx);

  // Parse Idle response
  bool parseIdleResponse(IMAPSession *imap);

  // Append Fetch UID/Flags string to buffer
  void appendFetchString(MB_String &buf, bool uid);

  // Parse command response
  void parseCmdResponse(IMAPSession *imap, char *buf, PGM_P find);

  // Get flags
  bool getFlags(IMAPSession *imap, char *buf, esp_mail_imap_response_types type);

  // Parse examine response
  void parseExamineResponse(IMAPSession *imap, char *buf);

  // Handle the error by sending callback and close session
  bool handleIMAPError(IMAPSession *imap, int err, bool ret);

  // Set Flag
  bool mSetFlag(IMAPSession *imap, MB_StringPtr sequenceSet, MB_StringPtr flags, esp_mail_imap_store_flag_type type, bool closeSession, bool silent = false, bool UID = true, int32_t modsequence = -1);

#endif
};

#if defined(ENABLE_IMAP)

class IMAPSession
{
public:
  IMAPSession(Client *client);
  IMAPSession();
  ~IMAPSession();

  /** Set the tcp timeout.
   *
   * @param timeoutSec The tcp timeout in seconds.
   */
  void setTCPTimeout(unsigned long timeoutSec);

  /** Assign custom Client from Arduino Clients.
   *
   * @param client The pointer to Arduino Client derived class e.g. WiFiClient, EthernetClient or GSMClient.
   */
  void setClient(Client *client);

  /** Assign TinyGsm Clients.
   *
   * @param client The pointer to TinyGsmClient.
   * @param modem The pointer to TinyGsm modem object. Modem should be initialized and/or set mode before transfering data.
   * @param pin The SIM pin.
   * @param apn The GPRS APN (Access Point Name).
   * @param user The GPRS user.
   * @param password The GPRS password.
   */
  void setGSMClient(Client *client, void *modem, const char *pin, const char *apn, const char *user, const char *password);

  /** Assign the callback function to handle the network connection for custom Client.
   *
   * @param networkConnectionCB The function that handles the network connection.
   */
  void networkConnectionRequestCallback(NetworkConnectionRequestCallback networkConnectionCB);

  /** Assign the callback function to handle the network connection status acknowledgement.
   *
   * @param networkStatusCB The function that handle the network connection status acknowledgement.
   */
  void networkStatusRequestCallback(NetworkStatusRequestCallback networkStatusCB);

  /** Set the network status acknowledgement.
   *
   * @param status The network status.
   */
  void setNetworkStatus(bool status);

  /** Set the BearSSL IO buffer size.
   *
   * @param rx The BearSSL receive buffer size in bytes.
   * @param tx The BearSSL trasmit buffer size in bytes.
   */
  void setSSLBufferSize(int rx = -1, int tx = -1);

  /** Begin the IMAP server connection.
   *
   * @param session_config The pointer to Session_Config structured data that keeps
   * the server and log in details.
   * @param imap_data The pointer to IMAP_Data structured data that keeps the
   * operation options.
   * @param login The bool option for login after server connection.
   * @return The boolean value which indicates the success of operation.
   */
  bool connect(Session_Config *session_config, IMAP_Data *imap_data, bool login = true);

  /** Log in to IMAP server using Email and password.
   *
   * @param email The IMAP server account email.
   * @param password The IMAP server account password.
   * @return The boolean value which indicates the success of operation.
   */
  template <typename T1 = const char *, typename T2 = const char *>
  bool loginWithPassword(T1 email, T2 password) { return mLogin(toStringPtr(email), toStringPtr(password), false); };

  /** Log in to IMAP server using Email and access token.
   *
   * @param email The IMAP server account email.
   * @param token The Access token to log in.
   * @return The boolean value which indicates the success of operation.
   */
  template <typename T1 = const char *, typename T2 = const char *>
  bool loginWithAccessToken(T1 email, T2 token) { return mLogin(toStringPtr(email), toStringPtr(token), true); };

  /** Send the client identification to the server
   *
   * @param identification The pointer to IMAP_Identification structured data that keeps
   * the key properties e.g., name, version, os, os_version, vendor, support_url, address,
   * date, command, arguments, and environment.
   */
  bool id(IMAP_Identification *identification);

  /** Return the server ID returns from ID command.
   * @return The server ID string.
   */
  String serverID();

  /** Return the SASL authentication status.
   * @return The boolean value indicates SASL authentication status.
   */
  bool isAuthenticated();

  /** Return the log status.
   * @return The boolean value log in status.
   */
  bool isLoggedIn();

  /** Return firmware update result when attachment filename matches.
   * @return The boolean value indicates the firmware update status.
   */
  bool isFirmwareUpdateSuccess();

  /** Begin the IMAP server connection without authentication.
   *
   * @param session_config The pointer to Session_Config structured data that keeps
   * the server and log in details.
   * @param callback The callback function that accepts IMAP_Response as parameter.
   * @param tag The tag that pass to the callback function.
   * @return The boolean value indicates the success of operation.
   */
  template <typename T = const char *>
  bool customConnect(Session_Config *session_config, imapResponseCallback callback, T tag = "") { return mCustomConnect(session_config, callback, toStringPtr(tag)); };

  /** Close the IMAP session.
   *
   * @return The boolean value which indicates the success of operation.
   */
  bool closeSession();

  /** Get TCP connection status.
   *
   * @return The boolean value indicates the connection status.
   */
  bool connected();

  /** Set to enable the debug.
   *
   * @param level The level to enable the debug message
   * level = 0, no debugging
   * level = 1, basic level debugging
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
   *
   * @note: the function will exit immediately and return true if the time since previous success folder selection (open)
   * with the same readOnly mode, is less than 5 seconds.
   */
  template <typename T = const char *>
  bool selectFolder(T folderName, bool readOnly = true) { return mSelectFolder(toStringPtr(folderName), readOnly); }

  /** Open the mailbox folder to read or search the mesages.
   *
   * @param folderName The name of known mailbox folder to be opened.
   * @param readOnly The option to open the mailbox for reading only. Set this
   * option to false when you wish
   * to modify the flags using the setFlag, addFlag and removeFlag functions.
   * @return The boolean value which indicates the success of operation.
   *
   * @note: the function will exit immediately and return true if the time since previous success folder selection (open)
   * with the same readOnly mode, is less than 5 seconds.
   */
  template <typename T = const char *>
  bool openFolder(T folderName, bool readOnly = true) { return mOpenFolder(toStringPtr(folderName), readOnly); }

  /** Close the mailbox folder that was opened.
   *
   * @param expunge The option to allow emty the deleted flag set messages in case folder was open with editable mode.
   * @return The boolean value which indicates the success of operation.
   */
  bool closeFolder(bool expunge = false) { return mCloseFolder(expunge); }

  /** Create folder.
   *
   * @param folderName The name of folder to create.
   * @return The boolean value which indicates the success of operation.
   */
  template <typename T = const char *>
  bool createFolder(T folderName) { return mCreateFolder(toStringPtr(folderName)); }

  /** Rename folder.
   *
   * @param currentFolderName The name of folder to rename.
   * @param newFolderName The new name of folder to rename.
   * @return The boolean value which indicates the success of operation.
   */
  template <typename T1 = const char *, typename T2 = const char *>
  bool renameFolder(T1 currentFolderName, T2 newFolderName) { return mRenameFolder(toStringPtr(currentFolderName), toStringPtr(newFolderName)); }

  /** Delete folder.
   *
   * @param folderName The name of folder to delete.
   * @return The boolean value which indicates the success of operation.
   */
  template <typename T = const char *>
  bool deleteFolder(T folderName) { return mDeleteFolder(toStringPtr(folderName)); }

  /** Get subscribes mailboxes.
   *
   * @param reference The reference name.
   * @param mailbox The mailbox name with possible wildcards.
   * @param folders The return FoldersCollection that contains the folder info e.g., name, attribute and delimiter.
   * @return The boolean value which indicates the success of operation.
   */
  template <typename T1 = const char *, typename T2 = const char *>
  bool getSubscribesMailboxes(T1 reference, T2 mailbox, FoldersCollection &folders) { return mGetSubscribesMailboxes(toStringPtr(reference), toStringPtr(mailbox), folders); }

  /** Subscribe mailbox.
   *
   * @param folderName The name of folder to subscribe.
   * @return The boolean value which indicates the success of operation.
   */
  template <typename T = const char *>
  bool subscribe(T folderName) { return mSubscribe(toStringPtr(folderName)); }

  /** Unsubscribe mailbox.
   *
   * @param folderName The name of folder to unsubscribe.
   * @return The boolean value which indicates the success of operation.
   */
  template <typename T = const char *>
  bool unSubscribe(T folderName) { return mUnSubscribe(toStringPtr(folderName)); }

  /** Get UID number in selected or opened mailbox.
   *
   * @param msgNum The message number or order in the total message numbers.
   * @return UID number in selected or opened mailbox.
   *
   * @note Returns 0 when fail to get UID.
   */
  int getUID(int msgNum);

  /** Get message flags in selected or opened mailbox.
   *
   * @param msgNum The message number or order in the total message numbers.
   * @return Message flags in selected or opened mailbox.
   *
   * @note Returns empty string when fail to get flags.
   */
  const char *getFlags(int msgNum);

  /** Send the custom IMAP command and get the result via callback.
   *
   * @param cmd The command string.
   * @param callback The callback function that accepts IMAP_Response as parameter.
   * @param tag The tag string to pass to the callback function.
   * @return The boolean value which indicates the success of operation.
   *
   * @note imap.connect and imap.selectFolder or imap.openFolder are needed to call once prior to call this function.
   */
  template <typename T1 = const char *, typename T2 = const char *>
  bool sendCustomCommand(T1 cmd, imapResponseCallback callback, T2 tag = "") { return mSendCustomCommand(toStringPtr(cmd), callback, toStringPtr(tag)); }

  /** Send the custom IMAP command data string.
   *
   * @param data The string data.
   * @param last The flag represents the last data to send (optional).
   * @return The boolean value which indicates the success of operation.
   *
   * @note Should be used after calling sendCustomCommand("APPEND xxxxxx");
   */
  template <typename T = const char *>
  bool sendCustomData(T data, bool lastData = false) { return mSendData(toStringPtr(data), lastData, esp_mail_imap_cmd_custom); }

  /** Send the custom IMAP command data.
   *
   * @param data The byte data.
   * @param size The data size.
   * @param lastData The flag represents the last data to send (optional).
   * @return The boolean value which indicates the success of operation.
   *
   * @note Should be used after calling ssendCustomCommand("APPEND xxxxxx");
   */
  bool sendCustomData(uint8_t *data, size_t size, bool lastData = false) { return mSendData(data, size, lastData, esp_mail_imap_cmd_custom); }

  /** Copy the messages to the defined mailbox folder.
   *
   * @param toCopy The pointer to the MessageList class that contains the
   * list of messages to copy.
   * @param dest The destination folder that the messages to copy to.
   * @return The boolean value which indicates the success of operation.
   */
  template <typename T = const char *>
  bool copyMessages(MessageList *toCopy, T dest) { return mCopyMessages(toCopy, toStringPtr(dest)); }

  /** Copy the messages to the defined mailbox folder.
   *
   * @param sequenceSet The sequence set string i.g., unique identifier (UID) or message sequence number or ranges of UID or sequence number.
   * @param UID The option for sequenceSet whether it is UID or message sequence number.
   * @param dest The destination folder that the messages to copy to.
   * @return The boolean value indicates the success of operation.
   */
  template <typename T1 = const char *, typename T2 = const char *>
  bool copyMessages(T1 sequenceSet, bool UID, T2 dest) { return mCopyMessagesSet(toStringPtr(sequenceSet), UID, toStringPtr(dest)); }

  /** Move the messages to the defined mailbox folder.
   *
   * @param toMove The pointer to the MessageList class that contains the
   * list of messages to move.
   * @param dest The destination folder that the messages to move to.
   * @return The boolean value which indicates the success of operation.
   */
  template <typename T = const char *>
  bool moveMessages(MessageList *toCopy, T dest) { return mMoveMessages(toCopy, toStringPtr(dest)); }

  /** Move the messages to the defined mailbox folder.
   *
   * @param sequenceSet The sequence set string i.g., unique identifier (UID) or message sequence number or ranges of UID or sequence number.
   * @param UID The option for sequenceSet whether it is UID or message sequence number.
   * @param dest The destination folder that the messages to move to.
   * @return The boolean value indicates the success of operation.
   */
  template <typename T1 = const char *, typename T2 = const char *>
  bool moveMessages(T1 sequenceSet, bool UID, T2 dest) { return mMoveMessagesSet(toStringPtr(sequenceSet), UID, toStringPtr(dest)); }

  /** Delete the messages in the opened mailbox folder.
   *
   * @param toDelete The pointer to the MessageList class that contains the
   * list of messages to delete.
   * @param expunge The boolean option to expunge all messages.
   * @param modsequence The int32_t option for UNCHANGESINCE conditional test.
   * @return The boolean value which indicates the success of operation.
   *
   * The modsequence value can be used only if IMAP server supports Conditional STORE extension
   * and the selected mailbox supports modsequences.
   */
  bool deleteMessages(MessageList *toDelete, bool expunge = false, int32_t modsequence = -1) { return mDeleteMessages(toDelete, expunge, modsequence); }

  /** Delete the messages in the opened mailbox folder.
   *
   * @param sequenceSet The sequence set string i.g., unique identifier (UID) or message sequence number or ranges of UID or sequence number.
   * @param UID The option for sequenceSet whether it is UID or message sequence number.
   * @param expunge The boolean option to expunge all messages.
   * @param modsequence The int32_t option for UNCHANGESINCE conditional test.
   * @return The boolean value which indicates the success of operation.
   *
   * The modsequence value can be used only if IMAP server supports Conditional STORE extension
   * and the selected mailbox supports modsequences.
   */
  template <typename T = const char *>
  bool deleteMessages(T sequenceSet, bool UID, bool expunge = false, int32_t modsequence = -1) { return mDeleteMessagesSet(toStringPtr(sequenceSet), UID, expunge, modsequence); }

  /** Get the quota root's resource usage and limits.
   *
   * @param quotaRoot The quota root to get.
   * @param info The pointer to IMAP_Quota_Root_Info that contains quota root's resource name, usage and limit.
   * @return The boolean value which indicates the success of operation.
   */
  template <typename T = const char *>
  bool getQuota(T quotaRoot, IMAP_Quota_Root_Info *info) { return mGetSetQuota(toStringPtr(quotaRoot), info, true); }

  /** Set the quota root's resource usage and limits.
   *
   * @param quotaRoot The quota root to set.
   * @param data The pointer to IMAP_Quota_Root_Info that contains quota root's resource name, usage and limit.
   * @return The boolean value which indicates the success of operation.
   */
  template <typename T = const char *>
  bool setQuota(T quotaRoot, IMAP_Quota_Root_Info *data) { return mGetSetQuota(toStringPtr(quotaRoot), data, false); }

  /** Get the list of quota roots for the named mailbox.
   *
   * @param mailbox The mailbox name.
   * @param quotaRootsList The pointer to IMAP_Quota_Roots_List that contains the list of IMAP_Quota_Root_Info.
   * @return The boolean value which indicates the success of operation.
   */
  template <typename T = const char *>
  bool getQuotaRoot(T mailbox, IMAP_Quota_Roots_List *quotaRootsList) { return mGetQuotaRoots(toStringPtr(mailbox), quotaRootsList); }

  /** Get the ACLs for a mailbox.
   *
   * @param mailbox The mailbox name.
   * @param aclList The pointer to the returning IMAP_Rights_List object.
   * @return The boolean value which indicates the success of operation.
   */
  template <typename T = const char *>
  bool getACL(T mailbox, IMAP_Rights_List *aclList) { return mManageACL(toStringPtr(mailbox), aclList, nullptr, toStringPtr(""), esp_mail_imap_cmd_get_acl); };

  /** Get the ACLs for a mailbox.
   *
   * @param mailbox The mailbox name.
   * @param acl The pointer to the acl IMAP_Rights_Info to set.
   * @return The boolean value which indicates the success of operation.
   */
  template <typename T = const char *>
  bool setACL(T mailbox, IMAP_Rights_Info *acl) { return mManageACL(toStringPtr(mailbox), nullptr, acl, toStringPtr(""), esp_mail_imap_cmd_set_acl); };

  /** Delete the ACLs set for identifier on mailbox.
   *
   * @param mailbox The mailbox name.
   * @param identifier The identifier (user) to remove the rights.
   * @return The boolean value which indicates the success of operation.
   */
  template <typename T1 = const char *, typename T2 = const char *>
  bool deleteACL(T1 mailbox, T2 identifier) { return mManageACL(toStringPtr(mailbox), nullptr, nullptr, toStringPtr(identifier), esp_mail_imap_cmd_delete_acl); };

  /** Show my ACLs for a mailbox.
   *
   * @param mailbox The mailbox name.
   * @param acl The pointer to the returning IMAP_Rights_Info object.
   * @return The boolean value which indicates the success of operation.
   */
  template <typename T = const char *>
  bool myRights(T mailbox, IMAP_Rights_Info *acl) { return mManageACL(toStringPtr(mailbox), nullptr, acl, toStringPtr(""), esp_mail_imap_cmd_my_rights); };

  /** Returns IMAP namespaces.
   *
   * @param mailbox The mailbox name.
   * @param ns The pointer to the returning IMAP_Namespaces_List object.
   * @return The boolean value which indicates the success of operation.
   */
  bool getNamespace(IMAP_Namespaces_List *ns) { return mNamespace(ns); };

  /** Enable IMAP capability.
   *
   * @param capability The mailbox name.
   * @return The boolean value which indicates the success of operation.
   */
  template <typename T = const char *>
  bool enable(T capability) { return mEnable(toStringPtr(capability)); };

  /** Listen for the selected or open mailbox for updates.
   * @return The boolean value which indicates the success of operation.
   */
  bool listen() { return mListen(false); };

  /** Stop listen for the mailbox for updates.
   * @return The boolean value which indicates the success of operation.
   */
  bool stopListen() { return mStopListen(false); };

  /** Check for the selected or open mailbox updates.
   * @return The boolean value which indicates the changes status of mailbox.
   */
  bool folderChanged();

  /** Send NOOP command to IMAP server.
   * @return The boolean value which indicates the success of operation.
   */
  bool noop();

  /** Assign the callback function that returns the operating status when
   * fetching or reading the Email.
   *
   * @param imapCallback The function that accepts the imapStatusCallback as
   * parameter.
   */
  void callback(imapStatusCallback imapCallback);

  /** Assign the callback function to decode the string based on the character set.
   *
   * @param callback The function that accepts the pointer to IMAP_Decoding_Info as parameter.
   */
  void characterDecodingCallback(imapCharacterDecodingCallback callback);

  /** Assign the callback function that returns the MIME data stream from
   * fetching or reading the Email.
   *
   * @param mimeDataStreamCallback The function that accepts the MIME_Stream_Info as
   * parameter.
   */
  void mimeDataStreamCallback(MIMEDataStreamCallback mimeDataStreamCallback);

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

  /** Get the operating status error code.
   *
   * @return The int value of operating status error code.
   *
   * The negative value indicated error.
   * See src/ESP_Mail_Error.h and extras/MB_FS.h
   */
  int errorCode();

  /** Clear all the cache data stored in the IMAP session object.
   */
  void empty();

  /** Get the status of message fetching and searching.
   *
   * @return The IMAP_Status object contains the fetching and searching statuses.
   */
  IMAP_Status status();

  /** Get the JSON string of file name list of files that stored in SD card.
   *
   * @return The JSON string of filenames.
   * @note This will available only when standard SD library was used and file storage is SD.
   */
  String fileList();

  /** Set the current timestamp.
   *
   * @param ts The current timestamp.
   * @param gmtOffset The GMT offset.
   */
  void setSystemTime(time_t ts, float gmtOffset = 0);

  /** Setup TCP KeepAlive for internal TCP client.
   *
   * @param tcpKeepIdleSeconds lwIP TCP Keepalive idle in seconds.
   * @param tcpKeepIntervalSeconds lwIP TCP Keepalive interval in seconds.
   * @param tcpKeepCount lwIP TCP Keepalive count.
   *
   * For the TCP (KeepAlive) options, see https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/lwip.html#tcp-options.
   *
   * If value of one of these parameters is zero, the TCP KeepAlive will be disabled.
   *
   * You can check the server connecting status, by exexuting <IMAPSession>.connected() which will return true when connection to the server is still alive.
   */
  void keepAlive(int tcpKeepIdleSeconds, int tcpKeepIntervalSeconds, int tcpKeepCount);

  /** Get TCP KeepAlive status.
   *
   * @return Boolean status of TCP KeepAlive.
   */
  bool isKeepAlive();

  friend class ESP_Mail_Client;
  friend class foldderList;

private:
  // Log in to IMAP server
  bool mLogin(MB_StringPtr email, MB_StringPtr password, bool isToken);

  // Clear message data
  void clearMessageData();

  // Check for valid UID or set wildcard * as UID
  void checkUID();

  // Check for valid saving file path or prepend /
  void checkPath();

  // Get message item by index
  void getMessages(uint16_t messageIndex, struct esp_mail_imap_msg_item_t &msg);

  // Get RFC822 message item by index
  void getRFC822Messages(uint16_t messageIndex, struct esp_mail_imap_msg_item_t &msg);

  // Close mailbox
  bool closeMailbox(bool expunge = false);

  // Open mailbox
  bool openMailbox(MB_StringPtr folder, esp_mail_imap_auth_mode mode, bool waitResponse, bool unselect);

  // Get folders list
  bool getMailboxes(FoldersCollection &folders);

  // Get subscribes mailboxes
  bool mGetSubscribesMailboxes(MB_StringPtr reference, MB_StringPtr mailbox, FoldersCollection &folders);

  // Subscribe the mailbox
  bool mSubscribe(MB_StringPtr folder);

  // Unsubscribe the mailbox
  bool mUnSubscribe(MB_StringPtr folder);

  // Get UID
  int mGetUID(int msgNum);

  // Fetch by sequence set
  bool mFetchSequenceSet();

  // Return string from TAG prepended command
  MB_String prependTag(PGM_P cmd, PGM_P tag = NULL);

  // Check capabilities
  bool checkCapabilities();

  // Listen mailbox changes
  bool mListen(bool recon);

  // Stop listen mailbox
  bool mStopListen(bool recon);

  // Send custom command
  bool mSendCustomCommand(MB_StringPtr cmd, imapResponseCallback callback, MB_StringPtr tag);

  // Send data after sending APPEND command
  bool mSendData(MB_StringPtr data, bool lastData, esp_mail_imap_command cmd);

  // Send data after sending APPEND command
  bool mSendData(uint8_t *data, size_t size, bool lastData, esp_mail_imap_command cmd);

  // Delete folder
  bool mDeleteFolder(MB_StringPtr folderName);

  // Create folder
  bool mCreateFolder(MB_StringPtr folderName);

  // Rename folder
  bool mRenameFolder(MB_StringPtr currentFolderName, MB_StringPtr newFolderName);

  // Copy message
  bool copyMsg(MessageList *toCopy, const char *sequenceSet, bool UID, MB_StringPtr dest);

  // Copy message
  bool mCopyMessages(MessageList *toCopy, MB_StringPtr dest);

  // Copy message using sequence set
  bool mCopyMessagesSet(MB_StringPtr sequenceSet, bool UID, MB_StringPtr dest);

  // Move message
  bool moveMsg(MessageList *toMove, const char *sequenceSet, bool UID, MB_StringPtr dest);

  // Move message
  bool mMoveMessages(MessageList *toMove, MB_StringPtr dest);

  // Move message using sequence set
  bool mMoveMessagesSet(MB_StringPtr sequenceSet, bool UID, MB_StringPtr dest);

  // Check for conditional STORE extention support
  bool isCondStoreSupported();

  // Check for mailbox mod-sequence support
  bool isModseqSupported();

  // add UNCHANGEDSINCE STORE modifier and CHANGEDSINCE FETCH modifier to command
  void addModifier(MB_String &cmd, esp_mail_imap_command_types type, int32_t modsequence);

  // Delete message
  bool deleteMsg(MessageList *toDelete, const char *sequenceSet, bool UID, bool expunge, int32_t modsequence = -1);

  // Delete messages
  bool mDeleteMessages(MessageList *toDelete, bool expunge = false, int32_t modsequence = -1);

  // Delete messages
  bool mDeleteMessagesSet(MB_StringPtr sequenceSet, bool UID, bool expunge = false, int32_t modsequence = -1);

  // Get or set the quota root's resource usage and limits.
  bool mGetSetQuota(MB_StringPtr quotaRoot, IMAP_Quota_Root_Info *data, bool getMode);

  // Parse the IMAP_Quota_Root_info
  void mParseQuota(const char *quota, IMAP_Quota_Root_Info *data);

  // Get the list of quota roots for the named mailbox.
  bool mGetQuotaRoots(MB_StringPtr mailbox, IMAP_Quota_Roots_List *quotaRootsList);

  // Get or set ACL.
  bool mManageACL(MB_StringPtr mailbox, IMAP_Rights_List *acl_list, IMAP_Rights_Info *acl, MB_StringPtr identifier, esp_mail_imap_command type);

  // Parse ACL
  void parseACL(MB_String &acl_str, IMAP_Rights_List *right_list);

  // parse Rights
  void parseRights(MB_String &righs_str, IMAP_Rights_Info *info);

  // Get Rights from IMAP_Rights_Info
  void getRights(MB_String &righs_str, IMAP_Rights_Info *info);

  // Get namespace
  bool mNamespace(IMAP_Namespaces_List *ns);

  // Enable the IMAP capability
  bool mEnable(MB_StringPtr capability);

  // Parse namespaces
  void parseNamespaces(MB_String &ns_str, IMAP_Namespaces *ns);

  // Close folder
  bool mCloseFolder(bool expunge = false);

  // Open folder
  bool mOpenFolder(MB_StringPtr folderName, bool readOnly);

  // Select folder
  bool mSelectFolder(MB_StringPtr folderName, bool readOnly);

  // Custom TCP connection
  bool mCustomConnect(Session_Config *session_config, imapResponseCallback callback, MB_StringPtr tag);

  // Append ID list to buffer
  void appendIdList(MB_String &list, IMAP_Identification *identification);

  // Handle connection
  bool handleConnection(Session_Config *session_config, IMAP_Data *imap_data, bool &ssl);

  // Start TCP connection
  bool connect(bool &ssl);

  // Print features not supported debug error message
  void printDebugNotSupported();

  bool _sessionSSL = false;
  bool _sessionLogin = false;
  bool _loginStatus = false;
  unsigned long _last_polling_error_ms = 0;
  unsigned long _last_host_check_ms = 0;
  unsigned long _last_server_connect_ms = 0;
  unsigned long _last_network_error_ms = 0;
  unsigned long tcpTimeout = TCP_CLIENT_DEFAULT_TCP_TIMEOUT_SEC;
  struct esp_mail_imap_response_status_t _responseStatus;
  int _cMsgIdx = 0;
  int _cPartIdx = 0;
  int _totalRead = 0;
  _vectorImpl<struct esp_mail_message_header_t> _headers;

  esp_mail_imap_command _imap_cmd = esp_mail_imap_command::esp_mail_imap_cmd_sasl_login;
  esp_mail_imap_command _prev_imap_cmd = esp_mail_imap_command::esp_mail_imap_cmd_sasl_login;
  esp_mail_imap_command _imap_custom_cmd = esp_mail_imap_cmd_custom;
  esp_mail_imap_command _prev_imap_custom_cmd = esp_mail_imap_cmd_custom;
  bool _idle = false;
  MB_String _cmd;
  _vectorImpl<struct esp_mail_imap_multipart_level_t> _multipart_levels;
  int _rfc822_part_count = 0;
  bool _unseen = false;
  bool _readOnlyMode = true;
  bool _msgDownload = false;
  bool _attDownload = false;
  bool _storageReady = false;
  bool _storageChecked = false;

  bool _auth_capability[esp_mail_auth_capability_maxType];
  bool _feature_capability[esp_mail_imap_read_capability_maxType];
  Session_Config *_session_cfg;
  _vectorImpl<int> _configPtrList;
  MB_String _currentFolder;
  bool _mailboxOpened = false;
  unsigned long _lastSameFolderOpenMillis = 0;
  MB_String _nextUID;
  MB_String _unseenMsgIndex;
  MB_String _flags_tmp;
  MB_String _quota_tmp;
  MB_String _quota_root_tmp;
  MB_String _acl_tmp;
  MB_String _ns_tmp;
  MB_String _server_id_tmp;
  MB_String _sdFileList;

  struct esp_mail_imap_data_config_t *_imap_data = nullptr;

  int _userHeaderOnly = -1;
  bool _headerOnly = true;
  bool _uidSearch = false;
  bool _headerSaved = false;
  bool _debug = false;
  int _debugLevel = 0;
  bool _secure = false;
  bool _authenticated = false;
  bool _isFirmwareUpdated = false;
  imapStatusCallback _statusCallback = NULL;
  imapResponseCallback _customCmdResCallback = NULL;
  MIMEDataStreamCallback _mimeDataStreamCallback = NULL;
  imapCharacterDecodingCallback _charDecCallback = NULL;

  _vectorImpl<struct esp_mail_imap_msg_num_t> _imap_msg_num;
  esp_mail_session_type _sessionType = esp_mail_session_type_imap;

  FoldersCollection _folders;
  SelectedFolderInfo _mbif;
  int _uid_tmp = 0;
  int _lastProgress = -1;

  ESP_Mail_TCPClient client;

  IMAP_Status _cbData;
};

#endif

#if defined(ENABLE_SMTP)

class SendingResult
{
private:
  _vectorImpl<SMTP_Result> _result;

  void add(SMTP_Result *r)
  {
    _result.push_back(*r);
  }

public:
  friend class SMTPSession;
  friend class ESP_Mail_Client;
  SendingResult(){};
  ~SendingResult() { clear(); };

  void clear()
  {
    for (size_t i = 0; i < _result.size(); i++)
    {
      _result[i].recipients.clear();
      _result[i].subject.clear();
      _result[i].timestamp = 0;
      _result[i].completed = false;
    }
    _result.clear();
  }

  SMTP_Result getItem(size_t index)
  {
    SMTP_Result r;
    if (index < _result.size())
      return _result[index];
    return r;
  }
  size_t size() { return _result.size(); };
};

class SMTPSession
{
public:
  SMTPSession(Client *client);
  SMTPSession();
  ~SMTPSession();

  /** Set the tcp timeout.
   *
   * @param timeoutSec The tcp timeout in seconds.
   */
  void setTCPTimeout(unsigned long timeoutSec);

  /** Assign custom Client from Arduino Clients.
   *
   * @param client The pointer to Arduino Client derived class e.g. WiFiClient, EthernetClient or GSMClient.
   */
  void setClient(Client *client);

  /** Assign TinyGsm Clients.
   *
   * @param client The pointer to TinyGsmClient.
   * @param modem The pointer to TinyGsm modem object. Modem should be initialized and/or set mode before transfering data.
   * @param pin The SIM pin.
   * @param apn The GPRS APN (Access Point Name).
   * @param user The GPRS user.
   * @param password The GPRS password.
   */
  void setGSMClient(Client *client, void *modem, const char *pin, const char *apn, const char *user, const char *password);

  /** Assign the callback function to handle the network connection for custom Client.
   *
   * @param networkConnectionCB The function that handles the network connection.
   */
  void networkConnectionRequestCallback(NetworkConnectionRequestCallback networkConnectionCB);

  /** Assign the callback function to handle the network connection status acknowledgement.
   *
   * @param networkStatusCB The function that handle the network connection status acknowledgement.
   */
  void networkStatusRequestCallback(NetworkStatusRequestCallback networkStatusCB);

  /** Set the network status acknowledgement.
   *
   * @param status The network status.
   */
  void setNetworkStatus(bool status);

  /** Set the BearSSL IO buffer size.
   *
   * @param rx The BearSSL receive buffer size in bytes.
   * @param tx The BearSSL trasmit buffer size in bytes.
   */
  void setSSLBufferSize(int rx = -1, int tx = -1);

  /** Begin the SMTP server connection.
   *
   * @param session_config The pointer to Session_Config structured data that keeps
   * the server and log in details.
   * @param login The bool option for login after server connection.
   * @return The boolean value indicates the success of operation.
   */
  bool connect(Session_Config *session_config, bool login = true);

  /** Log in to SMTP server using Email and password.
   *
   * @param email The SMTP server account email.
   * @param password The SMTP server account password.
   * @return The boolean value which indicates the success of operation.
   */
  template <typename T1 = const char *, typename T2 = const char *>
  bool loginWithPassword(T1 email, T2 password) { return mLogin(toStringPtr(email), toStringPtr(password), false); };

  /** Log in to SMTP server using Email and access token.
   *
   * @param email The SMTP server account email.
   * @param token The Access token to log in.
   * @return The boolean value which indicates the success of operation.
   */
  template <typename T1 = const char *, typename T2 = const char *>
  bool loginWithAccessToken(T1 email, T2 token) { return mLogin(toStringPtr(email), toStringPtr(token), true); };

  /** Return the SASL authentication status.
   * @return The boolean value indicates SASL authentication status.
   */
  bool isAuthenticated();

  /** Return the log status.
   * @return The boolean value indicates log in status.
   */
  bool isLoggedIn();

  /** Begin the SMTP server connection without authentication.
   *
   * @param session_config The pointer to Session_Config structured data that keeps
   * the server and log in details.
   * @param callback The callback function that accepts the SMTP_Response as parameter.
   * @param commandID The command identifier number that will pass to the callback.
   * @return The int value of status code.
   *
   * @note If commandID was not set or set to -1, the command identifier will be auto increased started from zero.
   */
  int customConnect(Session_Config *session_config, smtpResponseCallback callback, int commandID = -1);

  /** Close the SMTP session.
   *
   */
  bool closeSession();

  /** Get TCP connection status.
   *
   * @return The boolean value indicates the connection status.
   */
  bool connected();

  /** Send the custom SMTP command and get the result via callback.
   *
   * @param cmd The command string.
   * @param callback The function that accepts the SMTP_Response as parameter.
   * @param commandID The command identifier number that will pass to the callback.
   * @return The integer value of response code.
   *
   * @note smtp.connect or smtp.customConnect is needed to call once prior to call this function.
   *
   * If commandID was not set or set to -1, the command identifier will be auto increased started from zero.
   */
  template <typename T = const char *>
  int sendCustomCommand(T cmd, smtpResponseCallback callback, int commandID = -1) { return mSendCustomCommand(toStringPtr(cmd), callback, commandID); }

  /** Send the custom SMTP command data string.
   *
   * @param data The string data.
   * @return The boolean value which indicates the success of operation.
   *
   * @note Should be used after calling sendCustomCommand("DATA");
   */
  template <typename T = const char *>
  bool sendCustomData(T data) { return mSendData(toStringPtr(data)); }

  /** Send the custom SMTP command data.
   *
   * @param data The byte data.
   * @param size The data size.
   * @return The boolean value which indicates the success of operation.
   *
   * @note Should be used after calling sendCustomCommand("DATA");
   */
  bool sendCustomData(uint8_t *data, size_t size) { return mSendData(data, size); }

  /** Set to enable the debug.
   *
   * @param level The level to enable the debug message
   * level = 0, no debugging
   * level = 1, basic level debugging
   */
  void debug(int level);

  /** Get the error details when sending the Email
   *
   * @return The string of error details.
   */
  String errorReason();

  /** Get the SMTP server response status code.
   *
   * @return The int value of SMTP server response status code.
   *
   * See RFC 5321 standard's documentation.
   */
  int statusCode();

  /** Get the SMTP server response status message.
   *
   * @return The int value of SMTP server response status message.
   *
   */
  String statusMessage();

  /** Get the operating status error code.
   *
   * @return The int value of operating status error code.
   *
   * The negative value indicated error.
   * See src/ESP_Mail_Error.h and extras/MB_FS.h
   */
  int errorCode();

  /** Set the Email sending status callback function.
   *
   * @param smtpCallback The callback function that accept the
   * smtpStatusCallback param.
   */
  void callback(smtpStatusCallback smtpCallback);

  /** Get the status of message fetching and searching.
   *
   * @return The SMTP_Status object contains the sending status.
   */
  SMTP_Status status();

  /** Set the current timestamp.
   *
   * @param ts The current timestamp.
   * @param gmtOffset The GMT offset.
   */
  void setSystemTime(time_t ts, float gmtOffset = 0);

  /** Setup TCP KeepAlive for internal TCP client.
   *
   * @param tcpKeepIdleSeconds lwIP TCP Keepalive idle in seconds.
   * @param tcpKeepIntervalSeconds lwIP TCP Keepalive interval in seconds.
   * @param tcpKeepCount lwIP TCP Keepalive count.
   *
   * For the TCP (KeepAlive) options, see https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/lwip.html#tcp-options.
   *
   * If value of one of these parameters is zero, the TCP KeepAlive will be disabled.
   *
   * You can check the server connecting status, by exexuting <SMTPSession>.connected() which will return true when connection to the server is still alive.
   */
  void keepAlive(int tcpKeepIdleSeconds, int tcpKeepIntervalSeconds, int tcpKeepCount);

  /** Get TCP KeepAlive status.
   *
   * @return Boolean status of TCP KeepAlive.
   */
  bool isKeepAlive();

  SendingResult sendingResult;

  friend class ESP_Mail_Client;

private:
  bool _sessionSSL = false;
  bool _sessionLogin = false;
  struct esp_mail_smtp_response_status_t _responseStatus;
  int _sentSuccessCount = 0;
  int _sentFailedCount = 0;
  bool _chunkedEnable = false;
  int _chunkCount = 0;
  uint32_t ts = 0;
  unsigned long tcpTimeout = TCP_CLIENT_DEFAULT_TCP_TIMEOUT_SEC;

  esp_mail_smtp_command _smtp_cmd = esp_mail_smtp_command::esp_mail_smtp_cmd_greeting;

  bool _auth_capability[esp_mail_auth_capability_maxType];
  bool _feature_capability[esp_mail_smtp_send_capability_maxType];

  Session_Config *_session_cfg = NULL;
  _vectorImpl<int> _configPtrList;

  bool _debug = false;
  int _debugLevel = 0;
  bool _secure = false;
  bool _authenticated = false;
  bool _loginStatus = false;
  bool _waitForAuthenticate = false;
  bool _canForward = false;
  smtpStatusCallback _statusCallback = NULL;
  smtpResponseCallback _customCmdResCallback = NULL;
  int _commandID = -1;
  bool _sdStorageReady = false;
  bool _flashStorageReady = false;
  bool _sdStorageChecked = false;
  bool _flashStorageChecked = false;

  esp_mail_session_type _sessionType = esp_mail_session_type_smtp;
  SMTP_Status _cbData;
  struct esp_mail_smtp_msg_type_t _msgType;
  int _lastProgress = -1;

  ESP_Mail_TCPClient client;

  // Start TCP connection
  bool connect(bool &ssl);

  // Log in to SMTP server
  bool mLogin(MB_StringPtr email, MB_StringPtr password, bool isToken);

  // Handle TCP connection
  bool handleConnection(Session_Config *session_config, bool &ssl);

  // Send custom command
  int mSendCustomCommand(MB_StringPtr cmd, smtpResponseCallback callback, int commandID = -1);

  // Send data after sending DATA command
  bool mSendData(MB_StringPtr data);

  // Send data after sending DATA command
  bool mSendData(uint8_t *data, size_t size);
};

#endif

#if defined(ENABLE_SMTP) && defined(ENABLE_IMAP)

class ESP_Mail_Message : public SMTP_Message
{
public:
  ESP_Mail_Message(){};
  ~ESP_Mail_Message(){};
};

#endif

extern ESP_Mail_Client MailClient;

#endif /* ESP_MAIL_CLIENT_H */
