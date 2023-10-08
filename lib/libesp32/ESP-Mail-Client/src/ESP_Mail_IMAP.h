
#ifndef ESP_MAIL_IMAP_H
#define ESP_MAIL_IMAP_H

#include "ESP_Mail_Client_Version.h"
#if !VALID_VERSION_CHECK(30409)
#error "Mixed versions compilation."
#endif

/**
 * Mail Client Arduino Library for Espressif's ESP32 and ESP8266, Raspberry Pi RP2040 Pico, and SAMD21 with u-blox NINA-W102 WiFi/Bluetooth module
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

#include "ESP_Mail_Client_Version.h"
#include "ESP_Mail_Client.h"

#if defined(ENABLE_IMAP)

#if defined(MB_ARDUINO_PICO)
extern uint8_t _FS_start;
extern uint8_t _FS_end;
#endif

int ESP_Mail_Client::decodeChar(const char *s)
{
    return 16 * hexval(*(s + 1)) + hexval(*(s + 2));
}

void ESP_Mail_Client::decodeQP_UTF8(const char *buf, char *out)
{
    char *tmp = strP(esp_mail_str_47 /* "0123456789ABCDEF" */);
    int idx = 0;
    while (*buf)
    {
        if (*buf != '=')
            out[idx++] = *buf++;
        else if (*(buf + 1) == '\r' && *(buf + 2) == '\n')
            buf += 3;
        else if (*(buf + 1) == '\n')
            buf += 2;
        else if (!strchr(tmp, *(buf + 1)))
            out[idx++] = *buf++;
        else if (!strchr(tmp, *(buf + 2)))
            out[idx++] = *buf++;
        else
        {
            out[idx++] = decodeChar(buf);
            buf += 3;
        }
    }

    // release memory
    freeMem(&tmp);
}

char *ESP_Mail_Client::decode7Bit_UTF8(char *buf)
{
    MB_String s;

    // only non NULL and 7-bit ASCII are allowed

    // rfc2045 section 2.7

    size_t len = buf ? strlen(buf) : 0;

    for (size_t i = 0; i < len; i++)
    {
        if (buf[i] > 0 && buf[i] < 128 && i < 998)
            s.append(1, buf[i]);
    }

    // some special chars can't send in 7bit unless encoded as queoted printable string
    char *decoded = allocMem<char *>(s.length() + 10);
    decodeQP_UTF8(s.c_str(), decoded);
    s.clear();
    return decoded;
}

char *ESP_Mail_Client::decode8Bit_UTF8(char *buf)
{
    MB_String s;

    // only non NULL and less than 998 octet length are allowed

    // rfc2045 section 2.8

    size_t len = buf ? strlen(buf) : 0;

    for (size_t i = 0; i < len; i++)
    {
        if (buf[i] > 0 && i < 998)
            s.append(1, buf[i]);
    }

    char *decoded = allocMem<char *>(s.length() + 1);
    strcpy(decoded, s.c_str());
    s.clear();
    return decoded;
}

void ESP_Mail_Client::decodeString(IMAPSession *imap, MB_String &str, const char *enc)
{

    size_t p1 = 0, p2 = 0;
    MB_String headerEnc;

    if (strlen(enc) == 0)
    {
        while (str[p1] == ' ' && p1 < str.length() - 1)
            p1++;

        if (str[p1] == '=' && str[p1 + 1] == '?')
        {
            p2 = str.find('?', p1 + 2);
            if (p2 != MB_String::npos)
                headerEnc = str.substr(p1 + 2, p2 - p1 - 2);
        }
    }
    else
        headerEnc = enc;

    int bufSize = str.length() + 10;
    char *buf = allocMem<char *>(bufSize);

    // Content Q and B decodings
    RFC2047Decoder.decode(mbfs, buf, str.c_str(), bufSize);

    // Char set decoding
    esp_mail_char_decoding_scheme scheme = getEncodingFromCharset(headerEnc.c_str());
    if (imap->_charDecCallback)
    {
        IMAP_Decoding_Info decoding;
        decoding.charset = headerEnc.c_str();
        decoding.data = buf;
        decoding.type = IMAP_Decoding_Info::message_part_type_header;

        imap->_charDecCallback(&decoding);

        if (decoding.decodedString.length() > 0)
        {
            char *buf2 = allocMem<char *>(decoding.decodedString.length() + 1);
            strcpy(buf2, decoding.decodedString.c_str());
            // release memory and point to new buffer
            freeMem(&buf);
            buf = buf2;
        }
    }
    else if (scheme == esp_mail_char_decoding_scheme_iso8859_1)
    {
        int len = strlen(buf);
        int olen = (len + 1) * 2;
        unsigned char *out = allocMem<unsigned char *>(olen);
        decodeLatin1_UTF8(out, &olen, (unsigned char *)buf, &len);
        // release memory and point to new buffer
        freeMem(&buf);
        buf = (char *)out;
    }
    else if (scheme == esp_mail_char_decoding_scheme_tis_620 || scheme == esp_mail_char_decoding_scheme_iso8859_11 || scheme == esp_mail_char_decoding_scheme_windows_874)
    {
        size_t len2 = strlen(buf);
        char *tmp = allocMem<char *>((len2 + 1) * 3);
        decodeTIS620_UTF8(tmp, buf, len2);
        // release memory and point to new buffer
        freeMem(&buf);
        buf = tmp;
    }

    str = buf;
    // release memory
    freeMem(&buf);
}

esp_mail_char_decoding_scheme ESP_Mail_Client::getEncodingFromCharset(const char *enc)
{
    esp_mail_char_decoding_scheme scheme = esp_mail_char_decoding_scheme_default;

    for (int i = esp_mail_char_decoding_utf8; i < esp_mail_char_decoding_maxType; i++)
    {
        if (strpos(enc, char_decodings[i].text, 0, false) > -1)
            scheme = (esp_mail_char_decoding_scheme)i;
    }

    return scheme;
}

int ESP_Mail_Client::encodeUnicode_UTF8(char *out, uint32_t utf)
{
    if (utf <= 0x7F)
    {
        // Plain ASCII
        out[0] = (char)utf;
        out[1] = 0;
        return 1;
    }
    else if (utf <= 0x07FF)
    {
        // 2-byte unicode
        out[0] = (char)(((utf >> 6) & 0x1F) | 0xC0);
        out[1] = (char)(((utf >> 0) & 0x3F) | 0x80);
        out[2] = 0;
        return 2;
    }
    else if (utf <= 0xFFFF)
    {
        // 3-byte unicode
        out[0] = (char)(((utf >> 12) & 0x0F) | 0xE0);
        out[1] = (char)(((utf >> 6) & 0x3F) | 0x80);
        out[2] = (char)(((utf >> 0) & 0x3F) | 0x80);
        out[3] = 0;
        return 3;
    }
    else if (utf <= 0x10FFFF)
    {
        // 4-byte unicode
        out[0] = (char)(((utf >> 18) & 0x07) | 0xF0);
        out[1] = (char)(((utf >> 12) & 0x3F) | 0x80);
        out[2] = (char)(((utf >> 6) & 0x3F) | 0x80);
        out[3] = (char)(((utf >> 0) & 0x3F) | 0x80);
        out[4] = 0;
        return 4;
    }
    else
    {
        // error - use replacement character
        out[0] = (char)0xEF;
        out[1] = (char)0xBF;
        out[2] = (char)0xBD;
        out[3] = 0;
        return 0;
    }
}

void ESP_Mail_Client::decodeTIS620_UTF8(char *out, const char *in, size_t len)
{
    // output is the 3-byte value UTF-8
    int j = 0;
    for (size_t i = 0; i < len; i++)
    {
        if (in[i] < 0x80)
            out[j++] = in[i];
        else if ((in[i] >= 0xa0 && in[i] < 0xdb) || (in[i] > 0xde && in[i] < 0xfc))
        {
            int unicode = 0x0e00 + in[i] - 0xa0;
            char o[5];
            memset(o, 0, 5);
            int r = encodeUnicode_UTF8(o, unicode);
            for (int x = 0; x < r; x++)
                out[j++] = o[x];
        }
    }
}

int ESP_Mail_Client::decodeLatin1_UTF8(unsigned char *out, int *outlen, const unsigned char *in, int *inlen)
{
    unsigned char *outstart = out;
    const unsigned char *base = in;
    const unsigned char *processed = in;
    unsigned char *outend = out + *outlen;
    const unsigned char *inend;
    unsigned int c;
    int bits;

    inend = in + (*inlen);
    while ((in < inend) && (out - outstart + 5 < *outlen))
    {
        c = *in++;

        /* assertion: c is a single UTF-4 value */
        if (out >= outend)
            break;
        if (c < 0x80)
        {
            *out++ = c;
            bits = -6;
        }
        else
        {
            *out++ = ((c >> 6) & 0x1F) | 0xC0;
            bits = 0;
        }

        for (; bits >= 0; bits -= 6)
        {
            if (out >= outend)
                break;
            *out++ = ((c >> bits) & 0x3F) | 0x80;
        }
        processed = (const unsigned char *)in;
    }
    *outlen = out - outstart;
    *inlen = processed - base;
    return (0);
}

bool ESP_Mail_Client::sendFetchCommand(IMAPSession *imap, int msgIndex, esp_mail_imap_command cmdCase)
{

    MB_String cmd, cmd2, cmd3;
    appendHeadersFetchCommand(imap, cmd, msgIndex, false);

    if (cmdCase == esp_mail_imap_cmd_fetch_body_mime)
    {
        joinStringDot(cmd2, 2, imap_commands[esp_mail_imap_command_header].text, imap_commands[esp_mail_imap_command_fields].text);
        appendSpace(cmd2);
        joinStringSpace(cmd3, false, 2, message_headers[esp_mail_message_header_field_content_type].text, message_headers[esp_mail_message_header_field_content_transfer_encoding].text);
        appendString(cmd2, cmd3.c_str(), false, false, esp_mail_string_mark_type_round_bracket);
    }
    else if (cmdCase == esp_mail_imap_cmd_fetch_body_text)
        cmd2 = cPart(imap)->partNumFetchStr.length() > 0 ? cPart(imap)->partNumFetchStr.c_str() : imap_commands[esp_mail_imap_command_text].text;
    else if (cmdCase == esp_mail_imap_cmd_fetch_body_attachment)
        cmd2 = cPart(imap)->partNumFetchStr;

    if (cmd2.length() > 0)
        appendString(cmd, cmd2.c_str(), false, false, esp_mail_string_mark_type_square_bracket);

    bool allowPartialFetch = (cmdCase == esp_mail_imap_cmd_fetch_body_attachment && cPart(imap)->is_firmware_file) ? false : true;

    if (imap->_mimeDataStreamCallback)
        allowPartialFetch = false;

    if (allowPartialFetch)
    {
        //  Apply partial fetch in case download was disabled.
        if (!imap->_storageReady && imap->_attDownload && cmdCase == esp_mail_imap_cmd_fetch_body_attachment)
            cmd += esp_mail_str_48; /* "<0.0>" */ // This case should not happen because the memory storage was previousely checked.
        else if ((!imap->_msgDownload && cmdCase == esp_mail_imap_cmd_fetch_body_text) || (imap->_msgDownload && !imap->_storageReady))
        {
            cmd += esp_mail_str_49; /* "<0." */
            cmd += imap->_imap_data->limit.msg_size;
            cmd += esp_mail_str_20; /* ">" */
        }
    }

    if (imapSend(imap, cmd.c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
        return false;

    return true;
}

bool ESP_Mail_Client::readMail(IMAPSession *imap, bool closeSession)
{
    if (!imap || !sessionExisted<IMAPSession *>(imap))
        return false;

    imap->checkUID();
    imap->checkPath();
    imap->_cbData._success = false;

    if (!imap->connected())
        imap->_mailboxOpened = false;

    imap->_isFirmwareUpdated = false;

    MB_String buf, command, _uid;

    size_t readCount = 0;
    imap->_multipart_levels.clear();

    if (!reconnect(imap))
        return false;

    imap->_msgDownload = imap->_imap_data->download.text || imap->_imap_data->download.html;
    imap->_attDownload = imap->_imap_data->download.attachment || imap->_imap_data->download.inlineImg;

    if (!imap->_storageChecked)
    {
        imap->_storageChecked = true;
        imap->_storageReady = imap->_imap_data->download.header || (!imap->_imap_data->fetch.headerOnly && (imap->_msgDownload || imap->_attDownload)) ? mbfs->checkStorageReady(mbfs_type imap->_imap_data->storage.type) : true;
    }

    bool readyToDownload = (imap->_msgDownload || imap->_attDownload) && imap->_storageReady;

    if (!imap->_storageReady)
        sendStorageNotReadyError(imap, imap->_imap_data->storage.type);

#if defined(MB_ARDUINO_ESP) || defined(MB_ARDUINO_PICO)

    int cmem = MailClient.getFreeHeap();

    if (cmem < ESP_MAIL_MIN_MEM)
    {
#if !defined(SILENT_MODE)
        if (imap->_debug && imap->_statusCallback && !imap->_customCmdResCallback)
        {
            esp_mail_debug_print();
            errorStatusCB<IMAPSession *, IMAPSession *>(imap, nullptr, MAIL_CLIENT_ERROR_OUT_OF_MEMORY, true);
        }
#endif

        goto out;
    }
#endif

    if (!imap->connected() && !imap->_loginStatus)
    {
#if !defined(SILENT_MODE)
        if (imap->_debug && imap->_statusCallback && !imap->_customCmdResCallback)
        {
            esp_mail_debug_print();
            errorStatusCB<IMAPSession *, IMAPSession *>(imap, nullptr, MAIL_CLIENT_ERROR_NOT_YET_LOGIN, true);
        }
#endif
        return false;
    }

    // new session
    if (!imap->connected())
    {
        // authenticate new

        bool ssl = false;

        if (!imap->connect(ssl))
        {
            closeTCPSession<IMAPSession *>(imap);
            return false;
        }

        if (!imapAuth(imap, ssl))
        {
            closeTCPSession<IMAPSession *>(imap);
            return false;
        }
    }
    else
    {
        // If time config changed, we will update time
        MailClient.prepareTime<IMAPSession *>(imap->_session_cfg, imap);

        // reuse session
        for (size_t i = 0; i < imap->_headers.size(); i++)
            imap->_headers[i].part_headers.clear();
        imap->_headers.clear();

        if (imap->_imap_data->fetch.sequence_set.string.length() > 0 || imap->_imap_data->fetch.uid.length() > 0 || imap->_imap_data->fetch.number.length() > 0)
            imap->_headerOnly = false;
        else
            imap->_headerOnly = true;
    }

    imap->_rfc822_part_count = 0;
    imap->_mbif._availableItems = 0;
    imap->_imap_msg_num.clear();
    imap->_uidSearch = false;
    imap->_mbif._searchCount = 0;

    if (imap->_currentFolder.length() == 0)
        return handleIMAPError(imap, IMAP_STATUS_NO_MAILBOX_FOLDER_OPENED, false);

    if (!imap->_mailboxOpened || (imap->_imap_data->fetch.set_seen && !imap->_headerOnly && imap->_readOnlyMode))
    {
        if (!imap->openFolder(imap->_currentFolder.c_str(), imap->_readOnlyMode && !imap->_imap_data->fetch.set_seen))
            return handleIMAPError(imap, IMAP_STATUS_OPEN_MAILBOX_FAILED, false);
    }

    if (imap->_headerOnly)
    {
        if (imap->_imap_data->search.criteria.length() > 0)
        {
            command = esp_mail_imap_tag_str;

#if !defined(SILENT_MODE)
            printDebug<IMAPSession *>(imap,
                                      esp_mail_cb_str_18 /* "Searching messages..." */,
                                      esp_mail_dbg_str_36 /* "searching messages" */,
                                      esp_mail_debug_tag_type_client,
                                      true,
                                      false);

#endif

            if (strposP(imap->_imap_data->search.criteria.c_str(), imap_cmd_post_tokens[esp_mail_imap_command_uid].c_str(), 0) != -1)
            {
                imap->_uidSearch = true;
                command += imap_cmd_pre_tokens[esp_mail_imap_command_uid];
            }

            command += imap_cmd_pre_tokens[esp_mail_imap_command_search];

            imap->_imap_data->search.criteria.trim();

            MB_String tag;
            appendTagSpace(tag);

            // Remove internal used reserved tag
            if (strpos(imap->_imap_data->search.criteria.c_str(), tag.c_str(), 0, true) == 0)
                imap->_imap_data->search.criteria.erase(0, tag.length());

            for (size_t i = 0; i < imap->_imap_data->search.criteria.length(); i++)
            {
                if (imap->_imap_data->search.criteria[i] != ' ' && imap->_imap_data->search.criteria[i] != '\r' && imap->_imap_data->search.criteria[i] != '\n')
                    buf.append(1, imap->_imap_data->search.criteria[i]);

                if (imap->_imap_data->search.criteria[i] == ' ')
                {

                    if ((imap->_uidSearch && strcmp(buf.c_str(), imap_commands[esp_mail_imap_command_uid].text) == 0) || (imap->_unseen && buf.find(imap_commands[esp_mail_imap_command_all].text) != MB_String::npos))
                        buf.clear();

                    if (strcmp(buf.c_str(), imap_commands[esp_mail_imap_command_search].text) != 0 && buf.length() > 0)
                        prependSpace(command, buf.c_str());

                    buf.clear();
                }
            }

            if (imap->_unseen && strpos(imap->_imap_data->search.criteria.c_str(), imap_cmd_pre_tokens[esp_mail_imap_command_new].c_str(), 0) == -1)
                command += imap_cmd_pre_tokens[esp_mail_imap_command_new];

            if (buf.length() > 0)
                prependSpace(command, buf.c_str());

            if (!imap->isModseqSupported() && strpos(imap->_imap_data->search.criteria.c_str(), imap_cmd_pre_tokens[esp_mail_imap_command_modsec].c_str(), 0, false) != -1)
            {
                imap->_responseStatus.errorCode = IMAP_STATUS_MODSEQ_WAS_NOT_SUPPORTED;
#if !defined(SILENT_MODE)

                if (imap->_statusCallback)
                    sendErrorCB<IMAPSession *>(imap, imap->errorReason().c_str(), false, false);

                if (imap->_debug)
                    esp_mail_debug_print_tag(imap->errorReason().c_str(), esp_mail_debug_tag_type_error, true);

#endif
                return false;
            }

            if (imapSend(imap, command.c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
                return false;

            command.clear();

            imap->_imap_cmd = esp_mail_imap_cmd_search;

            if (!handleIMAPResponse(imap, IMAP_STATUS_BAD_COMMAND, closeSession))
                return false;

#if !defined(SILENT_MODE)
            if (imap->_statusCallback)
            {
                callBackSendNewLine<IMAPSession *>(imap, false);
                if (imap->_imap_msg_num.size() > 0)
                {
                    int bufLen = 100;
                    char *buf = allocMem<char *>(bufLen);
                    snprintf(buf, bufLen, pgm2Str(esp_mail_str_50 /* "Search limit: %d\nFound %d messages\nShow %d messages\n" */), (int)imap->_imap_data->limit.search, imap->_mbif._searchCount, (int)imap->_imap_msg_num.size());
                    sendCallback<IMAPSession *>(imap, buf, false, false);
                    // release memory
                    freeMem(&buf);
                }
                else
                    sendCallback<IMAPSession *>(imap, esp_mail_error_imap_str_9 /* "no messages found for the specified search criteria" */, false, false);
            }
#endif
        }
        else
        {
#if !defined(SILENT_MODE)
            if (imap->_statusCallback)
                sendCallback<IMAPSession *>(imap, esp_mail_error_imap_str_10 /* "no search criteria provided, then fetching the latest message" */, false, false);
#endif
            imap->_mbif._availableItems++;

            esp_mail_imap_msg_num_t msg_num;
            msg_num.type = esp_mail_imap_msg_num_type_number;
            msg_num.value = (uint32_t)imap->_mbif._msgCount;

            imap->_imap_msg_num.push_back(msg_num);
            imap->_headerOnly = false;
            imap->_imap_data->fetch.number = imap->_mbif._msgCount;
        }
    }
    else
    {

        if (imap->_imap_data->fetch.sequence_set.string.length() > 0)
        {
            imap->_headerOnly = imap->_imap_data->fetch.sequence_set.headerOnly;
            imap->mFetchSequenceSet();

            imap->_mbif._availableItems = imap->_imap_msg_num.size();
        }
        else
        {
            if (imap->_imap_data->fetch.uid.length() > 0)
            {
                imap->_mbif._availableItems++;

                esp_mail_imap_msg_num_t msg_num;
                msg_num.type = esp_mail_imap_msg_num_type_uid;
                msg_num.value = (uint32_t)atoi(imap->_imap_data->fetch.uid.c_str());

                imap->_imap_msg_num.push_back(msg_num);
            }

            if (imap->_imap_data->fetch.number.length() > 0)
            {
                imap->_mbif._availableItems++;

                esp_mail_imap_msg_num_t msg_num;
                msg_num.type = esp_mail_imap_msg_num_type_number;
                msg_num.value = (uint32_t)atoi(imap->_imap_data->fetch.number.c_str());

                imap->_imap_msg_num.push_back(msg_num);
            }
        }
    }

    if (imap->_imap_data->fetch.headerOnly)
        imap->_headerOnly = true;

    for (size_t i = 0; i < imap->_imap_msg_num.size(); i++)
    {
        imap->_cMsgIdx = i;
        imap->_totalRead++;

#if defined(MB_ARDUINO_ESP) || defined(MB_ARDUINO_PICO)
        if (MailClient.getFreeHeap() - (imap->_imap_data->limit.msg_size * (i + 1)) < ESP_MAIL_MIN_MEM)
        {
            errorStatusCB<IMAPSession *, IMAPSession *>(imap, nullptr, MAIL_CLIENT_ERROR_OUT_OF_MEMORY, true);
            goto out;
        }
#endif

#if !defined(SILENT_MODE)
        if (imap->_statusCallback)
        {
            readCount++;
            int bufLen = 100;
            PGM_P p = imap->_uidSearch || imap->_imap_msg_num[i].type == esp_mail_imap_msg_num_type_uid ? esp_mail_str_52 /* "Fetch message %d, UID: %d" */ : esp_mail_str_53 /* "Fetch message %d, Number: %d" */;
            char *buf = allocMem<char *>(bufLen);
            snprintf(buf, bufLen, pgm2Str(p), imap->_totalRead, (int)imap->_imap_msg_num[i].value);
            sendCallback<IMAPSession *>(imap, buf, true, false);
            // release memory
            freeMem(&buf);
        }

        if (imap->_debug)
            esp_mail_debug_print_tag(esp_mail_dbg_str_37 /* "send IMAP command, FETCH" */, esp_mail_debug_tag_type_client, true);
#endif
        MB_String cmd;
        appendHeadersFetchCommand(imap, cmd, i, true);

        // We fetch only known RFC822 headers because
        // using Fetch RFC822.HEADER reurns all included unused headers
        // which required more memory and network bandwidth.
        MB_String cmd2;
        appendRFC822HeadersFetchCommand(cmd2);

        appendString(cmd, cmd2.c_str(), false, false, esp_mail_string_mark_type_square_bracket);

        imap->addModifier(cmd, esp_mail_imap_command_changedsince, imap->_imap_data->fetch.modsequence);

        if (imapSend(imap, cmd.c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
        {
            if (i < imap->_imap_msg_num.size() - 1)
                continue;
            return false;
        }

        imap->_imap_cmd = esp_mail_imap_cmd_fetch_body_header;

        int err = imap->_headerOnly ? IMAP_STATUS_IMAP_RESPONSE_FAILED : IMAP_STATUS_BAD_COMMAND;

        if (!handleIMAPResponse(imap, err, closeSession))
        {
            if (i < imap->_imap_msg_num.size() - 1)
                continue;
            return false;
        }

        if (!cHeader(imap))
            continue;

        if (imap->_imap_msg_num[i].type == esp_mail_imap_msg_num_type_number)
            cHeader(imap)->message_uid = imap->mGetUID(cHeader(imap)->message_no);

        cHeader(imap)->flags = imap->getFlags(cHeader(imap)->message_no);

        if (!imap->_headerOnly)
        {
            imap->_cPartIdx = 0;

            // Reset attachment state if it was set by "multipart/mixed" content type header
            cHeader(imap)->hasAttachment = false;

#if !defined(SILENT_MODE)
            if (imap->_debug)
                debugPrintNewLine();
#endif

            // multipart
            if (cHeader(imap)->multipart)
            {
                struct esp_mail_imap_multipart_level_t mlevel;
                mlevel.level = 1;
                mlevel.fetch_rfc822_header = false;
                mlevel.append_body_text = false;
                imap->_multipart_levels.push_back(mlevel);

                if (!fetchMultipartBodyHeader(imap, i))
                    return false;
            }
            else
            {
                // single part
                if (imap->_debug)
                    printBodyPartFechingDubug(imap, "1", false);

                cHeader(imap)->partNumStr.clear();
                if (!sendFetchCommand(imap, i, esp_mail_imap_cmd_fetch_body_mime))
                    return false;

                imap->_imap_cmd = esp_mail_imap_cmd_fetch_body_mime;
                if (!handleIMAPResponse(imap, IMAP_STATUS_BAD_COMMAND, closeSession))
                    return false;
            }

            if (readyToDownload && imap->_imap_data->storage.saved_path.length() == 0)
                imap->_imap_data->storage.saved_path = esp_mail_str_10; /* "/" */

            if (cHeader(imap)->part_headers.size() > 0)
            {

                cHeader(imap)->sd_alias_file_count = 0;

                imap->_sdFileList.clear();
                if (!mbfs->longNameSupported())
                    imap->_sdFileList = esp_mail_str_40; /* "[" */

#if !defined(SILENT_MODE)

                if (cHeader(imap)->attachment_count > 0 && imap->_statusCallback)
                {
                    int bufLen = 100;
                    char *buf = allocMem<char *>(bufLen);
                    snprintf(buf, bufLen, pgm2Str(esp_mail_str_54 /* "Attachments (%d)" */), cHeader(imap)->attachment_count);
                    callBackSendNewLine<IMAPSession *>(imap, false);
                    sendCallback<IMAPSession *>(imap, buf, false, false);
                    // release memory
                    freeMem(&buf);

                    int count = 0;

                    for (size_t j = 0; j < cHeader(imap)->part_headers.size(); j++)
                    {
                        imap->_cPartIdx = j;
                        if (!cPart(imap)->rfc822_part && cPart(imap)->attach_type != esp_mail_att_type_none)
                        {
                            count++;
                            MB_String str = count;
                            appendDot(str);
                            prependSpace(str, cPart(imap)->filename.c_str());
                            sendCallback<IMAPSession *>(imap, str.c_str(), false, false);
                        }
                    }
                }

#endif

                MB_String s1, s2;
                int _idx1 = 0;
                for (size_t j = 0; j < cHeader(imap)->part_headers.size(); j++)
                {
                    imap->_cPartIdx = j;
                    if (cPart(imap)->rfc822_part)
                    {
                        s1 = cPart(imap)->partNumStr;
                        _idx1 = cPart(imap)->rfc822_msg_Idx;
                    }
                    else if (s1.length() > 0)
                    {
                        if (multipartMember(s1, cPart(imap)->partNumStr))
                        {
                            cPart(imap)->message_sub_type = esp_mail_imap_message_sub_type_rfc822;
                            cPart(imap)->rfc822_msg_Idx = _idx1;
                        }
                    }

                    if (cPart(imap)->multipart_sub_type == esp_mail_imap_multipart_sub_type_parallel)
                        s2 = cPart(imap)->partNumStr;
                    else if (s2.length() > 0)
                    {
                        if (multipartMember(s2, cPart(imap)->partNumStr))
                        {
                            cPart(imap)->attach_type = esp_mail_att_type_attachment;
                            if (cPart(imap)->filename.length() == 0)
                            {
                                if (cPart(imap)->name.length() > 0)
                                    cPart(imap)->filename = cPart(imap)->name;
                                else
                                {
                                    char *uid = getRandomUID();
                                    cPart(imap)->filename = uid;
                                    cPart(imap)->filename += mimeinfo[esp_mail_file_extension_dat].endsWith;
                                    freeMem(&uid);
                                }
                            }
                        }
                    }

                    checkFirmwareFile(imap, cPart(imap)->filename.c_str(), *cPart(imap), true);
                }

                int attach_count = 0;
                int ccnt = 0;

                for (size_t j = 0; j < cHeader(imap)->part_headers.size(); j++)
                {
                    imap->_cPartIdx = j;

                    if (cPart(imap)->rfc822_part || cPart(imap)->multipart_sub_type != esp_mail_imap_multipart_sub_type_none)
                        continue;

                    bool rfc822_body_subtype = cPart(imap)->message_sub_type == esp_mail_imap_message_sub_type_rfc822 && cPart(imap)->attach_type != esp_mail_att_type_attachment;

                    if (cPart(imap)->attach_type == esp_mail_att_type_none && (cPart(imap)->msg_type == esp_mail_msg_type_html || cPart(imap)->msg_type == esp_mail_msg_type_plain || cPart(imap)->msg_type == esp_mail_msg_type_enriched))
                    {

                        bool ret = ((imap->_imap_data->enable.rfc822 || imap->_imap_data->download.rfc822) && rfc822_body_subtype) || (!rfc822_body_subtype && ((imap->_imap_data->enable.text && (cPart(imap)->msg_type == esp_mail_msg_type_plain || cPart(imap)->msg_type == esp_mail_msg_type_enriched)) || (imap->_imap_data->enable.html && cPart(imap)->msg_type == esp_mail_msg_type_html) || (cPart(imap)->msg_type == esp_mail_msg_type_html && imap->_imap_data->download.html) || ((cPart(imap)->msg_type == esp_mail_msg_type_plain || cPart(imap)->msg_type == esp_mail_msg_type_enriched) && imap->_imap_data->download.text)));
                        if (!ret)
                            continue;

#if !defined(SILENT_MODE)

                        if ((imap->_imap_data->download.rfc822 && rfc822_body_subtype) || (!rfc822_body_subtype && ((cPart(imap)->msg_type == esp_mail_msg_type_html && imap->_imap_data->download.html) || ((cPart(imap)->msg_type == esp_mail_msg_type_plain || cPart(imap)->msg_type == esp_mail_msg_type_enriched) && imap->_imap_data->download.text))))
                        {

                            if (ccnt == 0 && imap->_statusCallback)
                                sendCallback<IMAPSession *>(imap, esp_mail_cb_str_43 /* "Downloading messages..." */, true, false);

                            if (imap->_debug)
                            {
                                debugPrintNewLine();
                                if (cPart(imap)->msg_type == esp_mail_msg_type_plain || cPart(imap)->msg_type == esp_mail_msg_type_enriched)
                                    esp_mail_debug_print_tag(esp_mail_dbg_str_74 /* "download plain TEXT message" */, esp_mail_debug_tag_type_client, true);
                                else if (cPart(imap)->msg_type == esp_mail_msg_type_html)
                                    esp_mail_debug_print_tag(esp_mail_dbg_str_71 /* "download HTML message" */, esp_mail_debug_tag_type_client, true);
                            }
                        }
                        else
                        {
                            if (ccnt == 0)
                                sendCallback<IMAPSession *>(imap, esp_mail_cb_str_28 /* "Reading messages..." */, true, false);

                            if (imap->_debug)
                            {
                                debugPrintNewLine();
                                if (cPart(imap)->msg_type == esp_mail_msg_type_plain || cPart(imap)->msg_type == esp_mail_msg_type_enriched)
                                    esp_mail_debug_print_tag(esp_mail_dbg_str_46 /* "reading plain TEXT message" */, esp_mail_debug_tag_type_client, true);
                                else if (cPart(imap)->msg_type == esp_mail_msg_type_html)
                                    esp_mail_debug_print_tag(esp_mail_dbg_str_47 /* "reading HTML message" */, esp_mail_debug_tag_type_client, true);
                            }
                        }

#endif

                        ccnt++;

                        if (!sendFetchCommand(imap, i, esp_mail_imap_cmd_fetch_body_text))
                            return false;

                        imap->_imap_cmd = esp_mail_imap_cmd_fetch_body_text;
                        if (!handleIMAPResponse(imap, IMAP_STATUS_IMAP_RESPONSE_FAILED, closeSession))
                            return false;
                    }
                    else if (cPart(imap)->attach_type != esp_mail_att_type_none && (imap->_storageReady || cPart(imap)->is_firmware_file))
                    {

                        if (cPart(imap)->is_firmware_file || (imap->_imap_data->download.attachment && cPart(imap)->attach_type == esp_mail_att_type_attachment) || (imap->_imap_data->download.inlineImg && cPart(imap)->attach_type == esp_mail_att_type_inline))
                        {
#if !defined(SILENT_MODE)
                            if (cPart(imap)->save_to_file)
                            {

                                if (attach_count == 0 && imap->_statusCallback)
                                    sendCallback<IMAPSession *>(imap, esp_mail_cb_str_19 /* "Downloading attachments..." */, true, false);

                                if (imap->_debug)
                                {
                                    debugPrintNewLine();
                                    int bufLen = 100;
                                    char *buf = allocMem<char *>(bufLen);
                                    snprintf(buf, bufLen, pgm2Str(esp_mail_dbg_str_70 /* "download attachment %d of %d" */), attach_count + 1, (int)cHeader(imap)->attachment_count);
                                    esp_mail_debug_print_tag(buf, esp_mail_debug_tag_type_client, true);
                                    // release memory
                                    freeMem(&buf);

                                    MB_String filePath = imap->_imap_data->storage.saved_path;
                                    filePath += esp_mail_str_10; /* "/" */
                                    filePath += cHeader(imap)->message_uid;
                                    filePath += esp_mail_str_10; /* "/" */
                                    filePath += cPart(imap)->filename;

                                    esp_mail_debug_print_tag(filePath.c_str(), esp_mail_debug_tag_type_client, true);
                                }
                            }
#endif

                            attach_count++;

                            if (cPart(imap)->octetLen <= (int)imap->_imap_data->limit.attachment_size)
                            {

                                if (imap->_storageReady || cPart(imap)->is_firmware_file)
                                {

                                    if ((int)j < (int)cHeader(imap)->part_headers.size() - 1)
                                        if (cHeader(imap)->part_headers[j + 1].octetLen > (int)imap->_imap_data->limit.attachment_size)
                                            cHeader(imap)->downloaded_bytes += cHeader(imap)->part_headers[j + 1].octetLen;

                                    if (!sendFetchCommand(imap, i, esp_mail_imap_cmd_fetch_body_attachment))
                                        return false;

                                    imap->_imap_cmd = esp_mail_imap_cmd_fetch_body_attachment;
                                    if (!handleIMAPResponse(imap, IMAP_STATUS_IMAP_RESPONSE_FAILED, closeSession))
                                        return false;

                                    yield_impl();
                                }
                            }
                            else
                            {
                                if ((int)j == (int)cHeader(imap)->part_headers.size() - 1)
                                    cHeader(imap)->downloaded_bytes += cPart(imap)->octetLen;
                            }
                        }
                    }
                }
            }

            if (imap->_storageReady && imap->_imap_data->download.header && !imap->_headerSaved)
            {
#if !defined(SILENT_MODE)
                if (imap->_statusCallback)
                    sendCallback<IMAPSession *>(imap, esp_mail_cb_str_21 /* "Saving message header to file..." */, true, false);
                else if (imap->_debug)
                    debugPrintNewLine();
#endif

                saveHeader(imap, false);
                saveHeader(imap, true);
            }

            // save files list to file
            if (imap->_storageReady && imap->_sdFileList.length() > 0)
            {
                MB_String filepath = cHeader(imap)->message_uid;
                filepath += mimeinfo[esp_mail_file_extension_txt].endsWith;
                if (mbfs->open(filepath, mbfs_type imap->_imap_data->storage.type, mb_fs_open_mode_write) > -1)
                {
                    mbfs->print(mbfs_type imap->_imap_data->storage.type, imap->_sdFileList.c_str());
                    mbfs->close(mbfs_type imap->_imap_data->storage.type);
                }
            }

            imap->_cMsgIdx++;
        }
#if !defined(SILENT_MODE)
        if (imap->_debug)
        {
            MB_String str = esp_mail_str_55; /* "Free Heap: " */
            str += MailClient.getFreeHeap();
            esp_mail_debug_print_tag(str.c_str(), esp_mail_debug_tag_type_client, true);
        }
#endif
    }
#if defined(MB_ARDUINO_ESP) || defined(MB_ARDUINO_PICO)
out:
#endif
    if (readCount < imap->_imap_msg_num.size())
    {
        imap->_mbif._availableItems = readCount;
        imap->_imap_msg_num.erase(imap->_imap_msg_num.begin() + readCount, imap->_imap_msg_num.end());
    }

    if (closeSession)
    {
        if (!imap->closeSession())
            return false;
    }
    else
    {
#if !defined(SILENT_MODE)
        printDebug<IMAPSession *>(imap,
                                  esp_mail_cb_str_46 /* "Finished reading Email" */,
                                  esp_mail_dbg_str_29 /* "finished reading Email" */,
                                  esp_mail_debug_tag_type_client,
                                  true,
                                  false);
#endif
    }

    imap->_cbData._success = true;

#if !defined(SILENT_MODE)
    if (imap->_statusCallback)
        callBackSendNewLine<IMAPSession *>(imap, true);
#endif

    return true;
}

void ESP_Mail_Client::appendHeadersFetchCommand(IMAPSession *imap, MB_String &cmd, int index, bool debug)
{
    if (imap->_uidSearch || imap->_imap_msg_num[index].type == esp_mail_imap_msg_num_type_uid)
        appendSpace(cmd, true, 2, imap_commands[esp_mail_imap_command_uid].text, imap_commands[esp_mail_imap_command_fetch].text);
    else
        appendSpace(cmd, true, imap_commands[esp_mail_imap_command_fetch].text);
#if !defined(SILENT_MODE)
    if (debug && imap->_debug)
        esp_mail_debug_print_tag(esp_mail_dbg_str_26 /* "fetch message header" */, esp_mail_debug_tag_type_client, true);
#endif
    joinStringSpace(cmd, false, 2, MB_String((int)imap->_imap_msg_num[index].value).c_str(), imap_commands[esp_mail_imap_command_body].text);

    if (!imap->_imap_data->fetch.set_seen)
        prependDot(cmd, imap_commands[esp_mail_imap_command_peek].text);
}

void ESP_Mail_Client::appendRFC822HeadersFetchCommand(MB_String &cmd)
{
    joinStringDot(cmd, 2, imap_commands[esp_mail_imap_command_header].text, imap_commands[esp_mail_imap_command_fields].text);
    appendSpace(cmd);

    MB_String cmd2;

    for (int i = 0; i < esp_mail_rfc822_header_field_maxType; i++)
        appendSpace(cmd2, false, rfc822_headers[i].text);

    joinStringSpace(cmd2, false, 4, message_headers[esp_mail_message_header_field_content_type].text,
                    message_headers[esp_mail_message_header_field_content_transfer_encoding].text,
                    message_headers[esp_mail_message_header_field_content_language].text,
                    message_headers[esp_mail_message_header_field_accept_language].text);

    appendString(cmd, cmd2.c_str(), false, false, esp_mail_string_mark_type_round_bracket);
}

bool ESP_Mail_Client::getMultipartFechCmd(IMAPSession *imap, int msgIdx, MB_String &partText)
{
    if (imap->_multipart_levels.size() == 0)
        return false;

    int cLevel = imap->_multipart_levels.size() - 1;

    cHeader(imap)->partNumStr.clear();

    appendHeadersFetchCommand(imap, partText, msgIdx, false);

    MB_String cmd1;
    for (size_t i = 0; i < imap->_multipart_levels.size(); i++)
    {
        if (i > 0)
        {
            cmd1 += esp_mail_str_27;                      /* "." */
            cHeader(imap)->partNumStr += esp_mail_str_27; /* "." */
        }

        cmd1 += imap->_multipart_levels[i].level;
        cHeader(imap)->partNumStr += imap->_multipart_levels[i].level;
    }

    if (imap->_multipart_levels[cLevel].fetch_rfc822_header)
    {
        MB_String cmd2;
        appendRFC822HeadersFetchCommand(cmd2);
        prependDot(cmd1, cmd2.c_str());
        imap->_multipart_levels[cLevel].append_body_text = true;
    }
    else
        prependDot(cmd1, imap_commands[esp_mail_imap_command_mime].text);

    appendString(partText, cmd1.c_str(), false, false, esp_mail_string_mark_type_square_bracket);

    imap->_multipart_levels[cLevel].fetch_rfc822_header = false;

    return true;
}

bool ESP_Mail_Client::multipartMember(const MB_String &parent, const MB_String &child)
{
    if (parent.length() > child.length())
        return false;

    for (size_t i = 0; i < parent.length(); i++)
        if (parent[i] != child[i])
            return false;

    return true;
}

bool ESP_Mail_Client::fetchMultipartBodyHeader(IMAPSession *imap, int msgIdx)
{
    bool ret = true;

    if (!connected<IMAPSession *>(imap))
    {
        closeTCPSession<IMAPSession *>(imap);
        return false;
    }
    int cLevel = 0;

    // slower than BODYSTRUCTURE parsing but sure
    do
    {

#if defined(MB_ARDUINO_ESP) || defined(MB_ARDUINO_PICO)
        // Prevent stack overflow
        if (MailClient.getFreeHeap() < ESP_MAIL_MIN_MEM)
        {
            errorStatusCB<IMAPSession *, IMAPSession *>(imap, nullptr, MAIL_CLIENT_ERROR_OUT_OF_MEMORY, true);
            break;
        }
#endif

        struct esp_mail_message_part_info_t *_cpart = &cHeader(imap)->part_headers[cHeader(imap)->message_data_count - 1];
        bool rfc822_body_subtype = _cpart->message_sub_type == esp_mail_imap_message_sub_type_rfc822 && _cpart->attach_type != esp_mail_att_type_attachment;

        MB_String cmd;
        if (!getMultipartFechCmd(imap, msgIdx, cmd))
            return true;

        if (imap->_debug)
            printBodyPartFechingDubug(imap, cHeader(imap)->partNumStr.c_str(), imap->_multipart_levels.size() > 1);

        // Try fetching the part and its sub parts hierarchically
        // Some sub part may not exist at the current multipart level
        if (imapSend(imap, cmd.c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
            return false;

        imap->_imap_cmd = esp_mail_imap_cmd_fetch_body_mime;
        ret = handleIMAPResponse(imap, IMAP_STATUS_IMAP_RESPONSE_FAILED, false);

        _cpart = &cHeader(imap)->part_headers[cHeader(imap)->message_data_count - 1];
        rfc822_body_subtype = _cpart->message_sub_type == esp_mail_imap_message_sub_type_rfc822 && _cpart->attach_type != esp_mail_att_type_attachment;
        cLevel = imap->_multipart_levels.size() - 1;

        if (ret)
        {
            if (_cpart->multipart)
            {
                if (_cpart->multipart_sub_type == esp_mail_imap_multipart_sub_type_parallel || _cpart->multipart_sub_type == esp_mail_imap_multipart_sub_type_alternative || _cpart->multipart_sub_type == esp_mail_imap_multipart_sub_type_related || _cpart->multipart_sub_type == esp_mail_imap_multipart_sub_type_mixed)
                {
                    struct esp_mail_imap_multipart_level_t mlevel;
                    mlevel.level = 1;
                    mlevel.fetch_rfc822_header = false;
                    mlevel.append_body_text = false;
                    imap->_multipart_levels.push_back(mlevel);
                    fetchMultipartBodyHeader(imap, msgIdx);
                }
                else
                    imap->_multipart_levels[cLevel].level++;
            }
            else
            {
                if (rfc822_body_subtype)
                {
                    // Get additional rfc822 message header
                    imap->_multipart_levels[cLevel].fetch_rfc822_header = true;
                    fetchMultipartBodyHeader(imap, msgIdx);
                }
                else
                {
                    if (imap->_multipart_levels[cLevel].append_body_text)
                    {
                        // single part rfc822 message body, append TEXT to the body fetch command
                        prependDot(_cpart->partNumFetchStr, imap_commands[esp_mail_imap_command_text].text);
                        imap->_multipart_levels[cLevel].append_body_text = false;
                    }
                    imap->_multipart_levels[cLevel].level++;
                }
            }
        }

    } while (ret);

    imap->_multipart_levels.pop_back();

    if (imap->_multipart_levels.size() > 0)
    {
        cLevel = imap->_multipart_levels.size() - 1;
        imap->_multipart_levels[cLevel].level++;
    }

    return true;
}

void ESP_Mail_Client::printBodyPartFechingDubug(IMAPSession *imap, const char *partNum, bool multiLevel)
{
#if !defined(SILENT_MODE)
    MB_String str = multiLevel ? esp_mail_dbg_str_28 /* "fetch body sub part header, " */ : esp_mail_dbg_str_27; /* "fetch body part header, " */
    str += partNum;
    esp_mail_debug_print_tag(str.c_str(), esp_mail_debug_tag_type_client, true);
#endif
}

bool ESP_Mail_Client::imapAuth(IMAPSession *imap, bool &ssl)
{

    if (!sessionExisted<IMAPSession *>(imap))
        return false;

    imap->_auth_capability[esp_mail_auth_capability_login] = false;

    imap->_session_cfg->int_start_tls = imap->_session_cfg->secure.startTLS;
    imap->_session_cfg->int_mode = imap->_session_cfg->secure.mode;

#if !defined(ESP_MAIL_DISABLE_SSL)
unauthenticate:
#endif

    // capabilities may change after TLS negotiation
    if (!imap->checkCapabilities())
        return false;

#if !defined(ESP_MAIL_DISABLE_SSL)

    if (imap->_session_cfg->int_mode != esp_mail_secure_mode_nonsecure)
    {
        // start TLS when needed or the server issues
        if ((imap->_auth_capability[esp_mail_auth_capability_starttls] || imap->_session_cfg->int_start_tls || imap->_session_cfg->int_mode == esp_mail_secure_mode_ssl_tls) && !ssl)
        {
#if !defined(SILENT_MODE)
            printDebug<IMAPSession *>(imap,
                                      esp_mail_cb_str_2 /* "Sending STARTTLS command..." */,
                                      esp_mail_dbg_str_1 /* "send command, STARTTLS" */,
                                      esp_mail_debug_tag_type_client,
                                      true,
                                      false);
#endif

            imapSend(imap, imap->prependTag(imap_commands[esp_mail_imap_command_starttls].text).c_str(), true);

            // rfc2595 section 3.1
            imap->_imap_cmd = esp_mail_imap_cmd_starttls;
            if (!handleIMAPResponse(imap, IMAP_STATUS_BAD_COMMAND, false))
                return false;

#if !defined(SILENT_MODE)
            if (imap->_debug)
                esp_mail_debug_print_tag(esp_mail_dbg_str_22 /* "perform SSL/TLS handshake" */, esp_mail_debug_tag_type_client, true);
#endif

            // connect in secure mode
            // do TLS handshake

            if (!imap->client.connectSSL(imap->_session_cfg->certificate.verify))
                return handleIMAPError(imap, MAIL_CLIENT_ERROR_SSL_TLS_STRUCTURE_SETUP, false);

            // set the secure mode
            imap->_session_cfg->int_start_tls = false;
            imap->_session_cfg->int_mode = esp_mail_secure_mode_undefined;
            ssl = true;
            imap->_secure = true;

            // check the capabilitiy again to prevent the man in the middle attack
            goto unauthenticate;
        }
    }

#endif

    imap->clearMessageData();
    imap->_mailboxOpened = false;

    bool creds = imap->_session_cfg->login.email.length() > 0 && imap->_session_cfg->login.password.length() > 0;
    bool sasl_auth_oauth = imap->_session_cfg->login.accessToken.length() > 0 && imap->_auth_capability[esp_mail_auth_capability_xoauth2];
    bool sasl_login = creds;
    bool sasl_auth_plain = imap->_auth_capability[esp_mail_auth_capability_plain] && creds;

    bool supported_sasl = sasl_auth_oauth || sasl_login || sasl_auth_plain;

    if (!supported_sasl)
        return handleIMAPError(imap, IMAP_STATUS_NO_SUPPORTED_AUTH, false);

    // rfc4959
    if (supported_sasl)
    {
#if !defined(SILENT_MODE)
        if (imap->_statusCallback)
            sendCallback<IMAPSession *>(imap, esp_mail_cb_str_14 /* "Logging in..." */, true, false);
        else if (imap->_debug)
            debugPrintNewLine();
#endif
    }

    if (sasl_auth_oauth)
    {

        if (!imap->_auth_capability[esp_mail_auth_capability_xoauth2])
        {
            handleIMAPError(imap, IMAP_STATUS_SERVER_OAUTH2_LOGIN_DISABLED, false);
            return false;
        }

#if !defined(SILENT_MODE)
        if (imap->_debug)
            esp_mail_debug_print_tag(esp_mail_dbg_str_45 /* "send IMAP command, AUTH XOAUTH2" */, esp_mail_debug_tag_type_client, true);
#endif

        MB_String cmd;
        joinStringSpace(cmd, true, 2, imap_commands[esp_mail_imap_command_authenticate].text, imap_commands[esp_mail_imap_command_xoauth2].text);

        if (imap->_auth_capability[esp_mail_auth_capability_sasl_ir])
        {
            prependSpace(cmd, getXOAUTH2String(imap->_session_cfg->login.email, imap->_session_cfg->login.accessToken).c_str());
            if (imapSend(imap, cmd.c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
                return false;
        }
        else
        {
            if (imapSend(imap, cmd.c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
                return false;

            imap->_imap_cmd = esp_mail_imap_cmd_sasl_auth_oauth;
            if (!handleIMAPResponse(imap, IMAP_STATUS_AUTHENTICATE_FAILED, true))
                return false;

            cmd = getXOAUTH2String(imap->_session_cfg->login.email, imap->_session_cfg->login.accessToken);

            if (imapSend(imap, cmd.c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
                return false;
        }

        imap->_imap_cmd = esp_mail_imap_cmd_sasl_auth_oauth;
        if (!handleIMAPResponse(imap, IMAP_STATUS_AUTHENTICATE_FAILED, false))
            return false;
    }
    else if (sasl_auth_plain)
    {

#if !defined(SILENT_MODE)
        if (imap->_debug)
            esp_mail_debug_print_tag(esp_mail_dbg_str_44 /* "send IMAP command, AUTHENTICATE PLAIN" */, esp_mail_debug_tag_type_client, true);
#endif

        int len = imap->_session_cfg->login.email.length() + imap->_session_cfg->login.password.length() + 2;
        uint8_t *tmp = allocMem<uint8_t *>(len);
        memset(tmp, 0, len);
        int p = 1;
        memcpy(tmp + p, imap->_session_cfg->login.email.c_str(), imap->_session_cfg->login.email.length());
        p += imap->_session_cfg->login.email.length() + 1;
        memcpy(tmp + p, imap->_session_cfg->login.password.c_str(), imap->_session_cfg->login.password.length());
        p += imap->_session_cfg->login.password.length();

        MB_String cmd;
        joinStringSpace(cmd, true, 2, imap_commands[esp_mail_imap_command_authenticate].text, imap_commands[esp_mail_imap_command_plain].text);

        if (imap->_auth_capability[esp_mail_auth_capability_sasl_ir])
        {
            prependSpace(cmd, encodeBase64Str(tmp, p).c_str());
            // release memory
            freeMem(&tmp);

            if (imapSend(imap, cmd.c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
                return false;
        }
        else
        {

            if (imapSend(imap, cmd.c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
                return false;

            imap->_imap_cmd = esp_mail_imap_cmd_sasl_auth_plain;

            if (!handleIMAPResponse(imap, IMAP_STATUS_AUTHENTICATE_FAILED, true))
                return false;

            cmd = encodeBase64Str(tmp, p);
            // release memory
            freeMem(&tmp);

            if (imapSend(imap, cmd.c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
                return false;
        }

        imap->_imap_cmd = esp_mail_imap_cmd_sasl_auth_plain;
        if (!handleIMAPResponse(imap, IMAP_STATUS_AUTHENTICATE_FAILED, true))
            return false;
    }
    else if (sasl_login)
    {

#if !defined(SILENT_MODE)
        if (imap->_debug)
            esp_mail_debug_print_tag(esp_mail_dbg_str_34 /* "send IMAP command, LOGIN" */, esp_mail_debug_tag_type_client, true);
#endif
        MB_String cmd;
        joinStringSpace(cmd, true, 3, imap_commands[esp_mail_imap_command_login].text, imap->_session_cfg->login.email.c_str(), imap->_session_cfg->login.password.c_str());

        if (imapSend(imap, cmd.c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
            return false;

        imap->_imap_cmd = esp_mail_imap_cmd_sasl_login;
        if (!handleIMAPResponse(imap, IMAP_STATUS_AUTHENTICATE_FAILED, true))
            return false;
    }

    // auto capabilities after login?
    if (!imap->_feature_capability[esp_mail_imap_read_capability_auto_caps])
    {
        if (!imap->checkCapabilities())
            return false;
    }

    if (imap->_feature_capability[esp_mail_imap_read_capability_id])
    {
        if (!imap->id(&imap->_imap_data->identification))
            return false;
    }

    if (supported_sasl)
        imap->_authenticated = true;

    return true;
}

bool ESP_Mail_Client::imapLogout(IMAPSession *imap)
{

#if defined(ESP8266)
    return false;
#endif

    if (!sessionExisted<IMAPSession *>(imap))
        return false;

#if !defined(SILENT_MODE)
    printDebug<IMAPSession *>(imap,
                              esp_mail_cb_str_20 /* "Logging out..." */,
                              esp_mail_dbg_str_38 /* "send IMAP command, LOGOUT" */,
                              esp_mail_debug_tag_type_client,
                              true,
                              false);
#endif

    if (imapSend(imap, imap->prependTag(imap_commands[esp_mail_imap_command_logout].text).c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
        return false;

    imap->_imap_cmd = esp_mail_imap_cmd_logout;
    if (!handleIMAPResponse(imap, IMAP_STATUS_BAD_COMMAND, false))
        return false;

    imap->_authenticated = false;

#if !defined(SILENT_MODE)
    printDebug<IMAPSession *>(imap,
                              esp_mail_cb_str_47 /* "Log out completed" */,
                              esp_mail_dbg_str_31 /* "log out completed" */,
                              esp_mail_debug_tag_type_client,
                              true,
                              false);
#endif
    return true;
}

size_t ESP_Mail_Client::imapSend(IMAPSession *imap, PGM_P data, bool newline)
{
    if (!imap || !sessionReady<IMAPSession *>(imap))
        return 0;

    int sent = 0;

    MB_String s = data;

    int toSend = newline ? s.length() + 2 : s.length();

    if (imap->_debug && imap->_debugLevel > esp_mail_debug_level_maintainer && !imap->_customCmdResCallback)
        esp_mail_debug_print(s.c_str(), newline);

    sent = newline ? imap->client.println(s.c_str()) : imap->client.print(s.c_str());

    if (sent != toSend)
    {
        errorStatusCB<IMAPSession *, IMAPSession *>(imap, nullptr, sent, true);
        sent = 0;
    }

    return sent;
}

size_t ESP_Mail_Client::imapSend(IMAPSession *imap, int data, bool newline)
{
    MB_String s = data;
    return imapSend(imap, s.c_str(), newline);
}

size_t ESP_Mail_Client::imapSend(IMAPSession *imap, uint8_t *data, size_t size)
{
    if (!imap || !sessionReady<IMAPSession *>(imap))
        return 0;

    int sent = 0;

    sent = imap->client.write(data, size);

    if (sent != (int)size)
    {
        errorStatusCB<IMAPSession *, IMAPSession *>(imap, nullptr, sent, true);
        sent = 0;
    }

    return sent;
}

bool ESP_Mail_Client::mSetFlag(IMAPSession *imap, MB_StringPtr sequenceSet, MB_StringPtr flag, esp_mail_imap_store_flag_type type, bool closeSession, bool silent, bool UID, int32_t modsequence)
{
    if (!reconnect(imap))
        return false;

    if (!imap->connected())
    {
        imap->_mailboxOpened = false;
        return false;
    }

    if (imap->_currentFolder.length() == 0)
    {
#if !defined(SILENT_MODE)
        printDebug<IMAPSession *>(imap,
                                  esp_mail_error_imap_str_11 /* "no mailbox opened" */,
                                  esp_mail_error_imap_str_11 /* "no mailbox opened" */,
                                  esp_mail_debug_tag_type_client,
                                  true,
                                  false);
#endif
    }
    else
    {
        if (imap->_readOnlyMode || !imap->_mailboxOpened)
        {
            if (!imap->selectFolder(imap->_currentFolder.c_str(), false))
                return false;
        }
    }

#if !defined(SILENT_MODE)
    PGM_P p1 = NULL;
    PGM_P p2 = NULL;
    if (type == esp_mail_imap_store_flag_type_set)
    {
        p1 = esp_mail_cb_str_26;  /* "Setting FLAG..." */
        p2 = esp_mail_dbg_str_41; /* "setting FLAG" */
    }
    else if (type == esp_mail_imap_store_flag_type_add)
    {
        p1 = esp_mail_cb_str_24;  /* "Adding FLAG..." */
        p2 = esp_mail_dbg_str_42; /* "adding FLAG" */
    }
    else
    {
        p1 = esp_mail_cb_str_23;  /* "Removing FLAG..." */
        p2 = esp_mail_dbg_str_43; /* "removing FLAG" */
    }

    printDebug<IMAPSession *>(imap,
                              p1,
                              p2,
                              esp_mail_debug_tag_type_client,
                              true,
                              false);
#endif

    if (!sessionExisted<IMAPSession *>(imap))
        return false;

    MB_String cmd;
    if (UID)
        appendSpace(cmd, true, 2, imap_commands[esp_mail_imap_command_uid].text, imap_commands[esp_mail_imap_command_store].text);
    else
        appendSpace(cmd, true, imap_commands[esp_mail_imap_command_store].text);

    cmd += sequenceSet;

    imap->addModifier(cmd, esp_mail_imap_command_unchangedsince, modsequence);

    if (type == esp_mail_imap_store_flag_type_set)
        cmd += imap_cmd_pre_tokens[esp_mail_imap_command_flags];
    else if (type == esp_mail_imap_store_flag_type_add)
        cmd += imap_cmd_pre_tokens[esp_mail_imap_command_plus_flags];
    else
        cmd += imap_cmd_pre_tokens[esp_mail_imap_command_minus_flags];

    if (silent)
        prependDot(cmd, imap_commands[esp_mail_imap_command_silent].text);

    appendSpace(cmd);
    appendString(cmd, MB_String(flag).c_str(), false, false, esp_mail_string_mark_type_round_bracket);

    if (imapSend(imap, cmd.c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
        return false;

    imap->_imap_cmd = esp_mail_imap_cmd_store;

    if (!handleIMAPResponse(imap, IMAP_STATUS_STORE_FAILED, false))
        return false;

    if (closeSession)
        imap->closeSession();

    return true;
}

int ESP_Mail_Client::parseSearchResponse(IMAPSession *imap, esp_mail_imap_response_data &res, PGM_P tag, const char *key)
{
    int bufLen = res.chunkBufSize;
    int ret = -1;
    char c = 0;
    int idx = 0;
    int num = 0;

    size_t tagLen = strlen_P(tag);
    MB_String _tag = tag;

    while (imap->client.available() > 0 && idx < bufLen)
    {
        yield_impl();

        ret = imap->client.read();

        if (ret > -1)
        {

            if (idx >= bufLen - 1)
                return idx;

            c = (char)ret;

            if (c == '\n')
                c = ' ';

            res.response[idx++] = c;

            if (res.chunkIdx == 0)
            {
                // Search response parsing
                if (strcmp(res.response, key) == 0)
                {
                    res.chunkIdx++;
                    return 0;
                }
                else
                {
                    // Status response parsing
                    res.imapResp = imapResponseStatus(imap, res.response, esp_mail_imap_tag_str);

                    // Exit if error or complete (no messages found)
                    if (res.imapResp != esp_mail_imap_resp_unknown)
                        goto end_search;
                }
            }
            else
            {
                if (c == ' ')
                {
                    imap->_mbif._searchCount++;
                    if (imap->_imap_data->enable.recent_sort)
                    {
                        esp_mail_imap_msg_num_t msg_num;
                        msg_num.type = imap->_uidSearch ? esp_mail_imap_msg_num_type_uid : esp_mail_imap_msg_num_type_number;
                        msg_num.value = (uint32_t)atoi(res.response);

                        imap->_imap_msg_num.push_back(msg_num);

                        if (imap->_imap_msg_num.size() > imap->_imap_data->limit.search)
                            imap->_imap_msg_num.erase(imap->_imap_msg_num.begin());
                    }
                    else
                    {
                        if (imap->_imap_msg_num.size() < imap->_imap_data->limit.search)
                        {
                            esp_mail_imap_msg_num_t msg_num;
                            msg_num.type = imap->_uidSearch ? esp_mail_imap_msg_num_type_uid : esp_mail_imap_msg_num_type_number;
                            msg_num.value = (uint32_t)atoi(res.response);

                            imap->_imap_msg_num.push_back(msg_num);
                        }
                    }

                    if (imap->_debug)
                    {
                        num = (float)(100.0f * imap->_mbif._searchCount / imap->_mbif._msgCount);
                        if (res.searchCount != num)
                        {
                            res.searchCount = num;
                            searchReport(imap, num);
                        }
                    }

                    res.chunkIdx++;
                    return idx;
                }
                else if (idx >= (int)tagLen)
                {
                    if (strpos(res.response, _tag.c_str(), 0, false) > -1)
                    {
#if defined(MB_USE_STD_VECTOR)
                        if (imap->_imap_data->enable.recent_sort)
                            std::sort(imap->_imap_msg_num.begin(), imap->_imap_msg_num.end(), compareMore);
#else
                        if (imap->_imap_data->enable.recent_sort)
                            numDecSort(imap->_imap_msg_num);
#endif
                        goto end_search;
                    }
                }
            }
        }
    }

    return idx;

end_search:

    res.endSearch = true;
    int read = imap->client.available();
    read = imap->client.readBytes(res.response + idx, read);
    return idx + read;
}

#if !defined(MB_USE_STD_VECTOR)
void ESP_Mail_Client::numDecSort(_vectorImpl<struct esp_mail_imap_msg_num_t> &arr)
{

    struct esp_mail_imap_msg_num_t tmp;

    for (size_t i = 0; i < arr.size(); ++i)
    {
        for (size_t j = i + 1; j < arr.size(); ++j)
        {
            if (arr[i].value < arr[j].value)
            {
                tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            }
        }
    }
}
#endif

struct esp_mail_message_part_info_t *ESP_Mail_Client::cPart(IMAPSession *imap)
{
    if (cHeader(imap) && imap->_cPartIdx < (int)cHeader(imap)->part_headers.size())
        return &cHeader(imap)->part_headers[imap->_cPartIdx];
    return nullptr;
}

struct esp_mail_message_header_t *ESP_Mail_Client::cHeader(IMAPSession *imap)
{
    if (cIdx(imap) < (int)imap->_headers.size())
        return &imap->_headers[cIdx(imap)];
    return nullptr;
}

bool ESP_Mail_Client::parseHeaderField(IMAPSession *imap, const char *buf, PGM_P beginToken, bool caseSensitive, struct esp_mail_message_header_t &header, int &headerState, int state)
{
    if (strcmpP(buf, 0, beginToken, caseSensitive))
    {
        headerState = state;
        char *tmp = subStr(buf, beginToken, NULL, 0, -1, caseSensitive);
        if (tmp)
        {
            collectHeaderField(imap, tmp, header, headerState);
            // release memory
            freeMem(&tmp);
            return true;
        }
    }

    return false;
}

void ESP_Mail_Client::parseHeaderResponse(IMAPSession *imap, esp_mail_imap_response_data &res, bool caseSensitive)
{
    char *tmp = nullptr;
    if (res.chunkIdx == 0)
    {
        MB_String str;
        joinStringDot(str, 2, imap_commands[esp_mail_imap_command_header].text, imap_commands[esp_mail_imap_command_fields].text);

        if (!res.isUntaggedResponse && strposP(res.response, str.c_str(), 0, caseSensitive) != -1 && res.response[0] == '*')
            res.isUntaggedResponse = true;

        if (res.isUntaggedResponse && res.response[strlen(res.response) - 1] == '}')
            res.untaggedRespCompleted = true;

        if (res.isUntaggedResponse && res.header.message_no == 0)
        {
            tmp = subStr(res.response, imap_responses[esp_mail_imap_response_untagged].text, imap_responses[esp_mail_imap_response_fetch].text, 0);
            if (tmp)
            {
                res.header.message_no = atoi(tmp);
                // release memory
                freeMem(&tmp);
            }
        }

        if (res.isUntaggedResponse && res.untaggedRespCompleted)
        {
            tmp = subStr(res.response, esp_mail_str_36 /* "{" */, esp_mail_str_37 /* "}" */, 0, 0, caseSensitive);
            if (tmp)
            {
                res.octetCount = 2;
                res.header.header_data_len = atoi(tmp);
                // release memory
                freeMem(&tmp);
                res.chunkIdx++;
            }
        }
    }
    else
    {
        if (res.octetCount > res.header.header_data_len + 2)
            return;

        res.chunkIdx++;

        MB_String field;

        for (int i = esp_mail_rfc822_header_field_from; i < esp_mail_rfc822_header_field_maxType; i++)
        {
            appendHeaderName(field, rfc822_headers[i].text, true, false, false);
            if (parseHeaderField(imap, res.response, field.c_str(), caseSensitive, res.header, res.headerState, i))
                return;
        }

        appendHeaderName(field, message_headers[esp_mail_message_header_field_content_transfer_encoding].text, true, true, false);

        if (parseHeaderField(imap, res.response, field.c_str(), caseSensitive, res.header, res.headerState, esp_mail_imap_state_content_transfer_encoding))
            return;

        appendHeaderName(field, message_headers[esp_mail_message_header_field_accept_language].text, true, true, false);

        if (parseHeaderField(imap, res.response, field.c_str(), caseSensitive, res.header, res.headerState, esp_mail_imap_state_accept_language))
            return;

        appendHeaderName(field, message_headers[esp_mail_message_header_field_content_language].text, true, true, false);

        if (parseHeaderField(imap, res.response, field.c_str(), caseSensitive, res.header, res.headerState, esp_mail_imap_state_content_language))
            return;

        MB_String contentTypeName;
        appendHeaderName(contentTypeName, message_headers[esp_mail_message_header_field_content_type].text, false, false, false);
        if (strcmpP(res.response, 0, contentTypeName.c_str(), caseSensitive))
        {
            res.headerState = esp_mail_imap_state_content_type;
            tmp = subStr(res.response, contentTypeName.c_str(), esp_mail_str_35 /* ";" */, 0, 0, caseSensitive);
            if (tmp)
            {
                // We set attachment status here as attachment should be included in multipart/mixed message,
                // unless no real attachments included which we don't know until fetching the sub part.
                if (strpos(tmp, esp_mail_imap_multipart_sub_type_t::mixed, 0, caseSensitive) != -1)
                    res.header.hasAttachment = true;

                collectHeaderField(imap, res.response, res.header, res.headerState);
                // release memory
                freeMem(&tmp);
            }
        }
    }
}

void ESP_Mail_Client::collectHeaderField(IMAPSession *imap, char *buf, struct esp_mail_message_header_t &header, int state)
{
    size_t i = 0;
    while (buf[i] == ' ')
    {
        i++;
        if (strlen(buf) <= i)
            return;
    }

    if (state < esp_mail_rfc822_header_field_maxType)
    {
        int ptr = getRFC822HeaderPtr(state, &header.header_fields);
        if (ptr > 0)
        {
            *(addrTo<MB_String *>(ptr)) += &buf[i];
        }
        return;
    }

    switch (state)
    {
    case esp_mail_imap_state_content_type:
        header.content_type += &buf[i];
        break;
    case esp_mail_imap_state_content_transfer_encoding:
        header.content_transfer_encoding += &buf[i];
        break;
    case esp_mail_imap_state_accept_language:
        header.accept_language += &buf[i];
        break;
    case esp_mail_imap_state_content_language:
        header.content_language += &buf[i];
        break;
    case esp_mail_imap_state_char_set:
        header.char_set += &buf[i];
        break;
    case esp_mail_imap_state_boundary:
        header.boundary += &buf[i];
        break;
    default:
        break;
    }
}

bool ESP_Mail_Client::getDecodedHeader(IMAPSession *imap, const char *buf, PGM_P beginToken, MB_String &out, bool caseSensitive)
{
    if (getHeader(buf, beginToken, out, caseSensitive))
    {
        // decode header text
        decodeString(imap, out);
        return true;
    }
    return false;
}

void ESP_Mail_Client::checkFirmwareFile(IMAPSession *imap, const char *filename, struct esp_mail_message_part_info_t &part, bool defaultSize)
{
    if (strcmp(filename, imap->_imap_data->firmware_update.attach_filename.c_str()) == 0 && part.attach_type == esp_mail_att_type_attachment)
    {
        part.is_firmware_file = true;
        // If no file size prop from Content-Disposition header
        if (part.attach_data_size == 0 && defaultSize)
        {
#if defined(ESP32) || defined(ESP8266)
            int sketchFreeSpace = ESP.getFreeSketchSpace();
            part.attach_data_size = sketchFreeSpace ? sketchFreeSpace : 1024000;
#elif defined(MB_ARDUINO_PICO)
            size_t spiffsSize = ((size_t)&_FS_end - (size_t)&_FS_start);
            part.attach_data_size = spiffsSize ? spiffsSize / 2 : 1024000;
#endif
        }

        if (!imap->_imap_data->firmware_update.save_to_file)
            part.save_to_file = false;
    }
}

void ESP_Mail_Client::parsePartHeaderResponse(IMAPSession *imap, esp_mail_imap_response_data &res, bool caseSensitive)
{
    char *tmp = nullptr;
    if (res.chunkIdx == 0)
    {
        tmp = subStr(res.response, imap_responses[esp_mail_imap_response_fetch].text, NULL, 0, -1);
        if (tmp)
        {
            // release memory
            freeMem(&tmp);
            tmp = subStr(res.response, esp_mail_str_36 /* "{" */, esp_mail_str_37 /* "}" */, 0);
            if (tmp)
            {
                res.chunkIdx++;
                res.part.octetLen = atoi(tmp);
                res.octetCount = 2;
                // release memory
                freeMem(&tmp);
            }
        }
    }
    else
    {
        MB_String value, old_value;
        bool valueStored = false;
        res.chunkIdx++;

        // if all octets read

        if (res.octetCount > res.part.octetLen)
        {

            // Is inline attachment without content id or name or filename?
            // It is supposed to be the inline message txt content, reset attach type to none

            if (res.part.attach_type == esp_mail_att_type_inline && res.part.CID.length() == 0)
                res.part.attach_type = esp_mail_att_type_none;

            // Is attachment file extension missing?
            // append extension

            if (res.part.attach_type == esp_mail_att_type_inline || res.part.attach_type == esp_mail_att_type_attachment)
            {
                if (res.part.filename.length() > 0 && res.part.filename.find('.') == MB_String::npos)
                {
                    MB_String ext;
                    getExtfromMIME(res.part.content_type.c_str(), ext);
                    res.part.filename += ext;
                }

                checkFirmwareFile(imap, res.part.filename.c_str(), res.part);
            }

            return;
        }

        // Content header field parse
        if (strcmpP(res.response, 0, esp_mail_str_56 /* "content-" */, caseSensitive))
        {
            // Content-Type
            MB_String contentTypeName;
            appendHeaderName(contentTypeName, message_headers[esp_mail_message_header_field_content_type].text, false, false, false);
            if (strcmpP(res.response, 0, contentTypeName.c_str(), caseSensitive))
            {

                res.part.cur_content_hdr = esp_mail_message_part_info_t::content_header_field_type;
                resetStringPtr(res.part);

                tmp = subStr(res.response, contentTypeName.c_str(), esp_mail_str_35 /* ";" */, 0, 0, caseSensitive);
                if (tmp)
                {
                    res.part.content_type = tmp;
                    // release memory
                    freeMem(&tmp);
                    int p1 = strposP(res.part.content_type.c_str(), esp_mail_imap_composite_media_type_t::multipart, 0, caseSensitive);
                    if (p1 != -1)
                    {
                        p1 += strlen(esp_mail_imap_composite_media_type_t::multipart) + 1;
                        res.part.multipart = true;
                        // inline or embedded images
                        if (strpos(res.part.content_type.c_str(), esp_mail_imap_multipart_sub_type_t::related, p1, caseSensitive) != -1)
                            res.part.multipart_sub_type = esp_mail_imap_multipart_sub_type_related;
                        // multiple text formats e.g. plain, html, enriched
                        else if (strpos(res.part.content_type.c_str(), esp_mail_imap_multipart_sub_type_t::alternative, p1, caseSensitive) != -1)
                            res.part.multipart_sub_type = esp_mail_imap_multipart_sub_type_alternative;
                        // medias
                        else if (strpos(res.part.content_type.c_str(), esp_mail_imap_multipart_sub_type_t::parallel, p1, caseSensitive) != -1)
                            res.part.multipart_sub_type = esp_mail_imap_multipart_sub_type_parallel;
                        // rfc822 encapsulated
                        else if (strpos(res.part.content_type.c_str(), esp_mail_imap_multipart_sub_type_t::digest, p1, caseSensitive) != -1)
                            res.part.multipart_sub_type = esp_mail_imap_multipart_sub_type_digest;
                        else if (strpos(res.part.content_type.c_str(), esp_mail_imap_multipart_sub_type_t::report, p1, caseSensitive) != -1)
                            res.part.multipart_sub_type = esp_mail_imap_multipart_sub_type_report;
                        // others can be attachments
                        else if (strpos(res.part.content_type.c_str(), esp_mail_imap_multipart_sub_type_t::mixed, p1, caseSensitive) != -1)
                            res.part.multipart_sub_type = esp_mail_imap_multipart_sub_type_mixed;
                    }

                    p1 = strposP(res.part.content_type.c_str(), esp_mail_imap_composite_media_type_t::message, 0, caseSensitive);
                    if (p1 != -1)
                    {
                        p1 += strlen(esp_mail_imap_composite_media_type_t::message) + 1;
                        if (strpos(res.part.content_type.c_str(), esp_mail_imap_message_sub_type_t::rfc822, p1, caseSensitive) != -1)
                            res.part.message_sub_type = esp_mail_imap_message_sub_type_rfc822;
                        else if (strpos(res.part.content_type.c_str(), esp_mail_imap_message_sub_type_t::Partial, p1, caseSensitive) != -1)
                            res.part.message_sub_type = esp_mail_imap_message_sub_type_partial;
                        else if (strpos(res.part.content_type.c_str(), esp_mail_imap_message_sub_type_t::External_Body, p1, caseSensitive) != -1)
                            res.part.message_sub_type = esp_mail_imap_message_sub_type_external_body;
                        else if (strpos(res.part.content_type.c_str(), esp_mail_imap_message_sub_type_t::delivery_status, p1, caseSensitive) != -1)
                            res.part.message_sub_type = esp_mail_imap_message_sub_type_delivery_status;
                    }

                    p1 = strpos(res.part.content_type.c_str(), esp_mail_imap_descrete_media_type_t::text, 0, caseSensitive);
                    if (p1 != -1)
                    {
                        p1 += strlen(esp_mail_imap_descrete_media_type_t::text) + 1;
                        if (strpos(res.part.content_type.c_str(), esp_mail_imap_media_text_sub_type_t::plain, p1, caseSensitive) != -1)
                            res.part.msg_type = esp_mail_msg_type_plain;
                        else if (strpos(res.part.content_type.c_str(), esp_mail_imap_media_text_sub_type_t::enriched, p1, caseSensitive) != -1)
                            res.part.msg_type = esp_mail_msg_type_enriched;
                        else if (strpos(res.part.content_type.c_str(), esp_mail_imap_media_text_sub_type_t::html, p1, caseSensitive) != -1)
                            res.part.msg_type = esp_mail_msg_type_html;
                        else
                            res.part.msg_type = esp_mail_msg_type_plain;
                    }
                }
            }

            // Content-Description
            MB_String contentDescrName;
            appendHeaderName(contentDescrName, message_headers[esp_mail_message_header_field_content_description].text, false, true, false);
            if (getDecodedHeader(imap, res.response, contentDescrName.c_str(), res.part.descr, caseSensitive))
            {
                res.part.cur_content_hdr = esp_mail_message_part_info_t::content_header_field_description;

                tmp = subStr(res.response, contentDescrName.c_str(), NULL, 0, -1, caseSensitive);
                if (tmp)
                {
                    value = tmp;
                    // release memory
                    freeMem(&tmp);

                    res.part.stringPtr = toAddr(res.part.content_description);
                    value.trim();
                    if (value.length() == 0)
                        return;
                }
            }

            // Content-ID
            MB_String contentIdName;
            appendHeaderName(contentIdName, message_headers[esp_mail_message_header_field_content_id].text, false, true, false);
            if (strcmpP(res.response, 0, contentIdName.c_str(), caseSensitive))
            {
                tmp = subStr(res.response, contentIdName.c_str(), NULL, 0, -1, caseSensitive);
                if (tmp)
                {
                    res.part.CID = tmp;
                    // release memory
                    freeMem(&tmp);
                    res.part.CID.trim();

                    if (res.part.CID[0] == '<')
                        res.part.CID.erase(0, 1);

                    if (res.part.CID[res.part.CID.length() - 1] == '>')
                        res.part.CID.erase(res.part.CID.length() - 1, 1);

                    // if inline attachment file name was not assigned
                    if (res.part.attach_type == esp_mail_att_type_inline && res.part.filename.length() == 0)
                    {
                        // set filename from content id and append extension later
                        res.part.filename = res.part.CID;
                        res.part.name = res.part.filename;
                    }
                }

                res.part.cur_content_hdr = esp_mail_message_part_info_t::content_header_field_id;
                resetStringPtr(res.part);
            }

            // Content-Disposition
            MB_String contentDispositionName;
            appendHeaderName(contentDispositionName, message_headers[esp_mail_message_header_field_content_disposition].text, false, true, false);
            if (strcmpP(res.response, 0, contentDispositionName.c_str(), caseSensitive))
            {

                res.part.cur_content_hdr = esp_mail_message_part_info_t::content_header_field_disposition;
                resetStringPtr(res.part);

                tmp = subStr(res.response, contentDispositionName.c_str(), esp_mail_str_35 /* ";" */, 0, 0, caseSensitive);
                if (tmp)
                {
                    // don't count altenative part text and html as embedded contents
                    if (cHeader(imap)->multipart_sub_type != esp_mail_imap_multipart_sub_type_alternative)
                    {
                        res.part.content_disposition = tmp;
                        if (caseSensitive)
                        {
                            if (strcmp(tmp, esp_mail_content_disposition_type_t::attachment) == 0)
                                res.part.attach_type = esp_mail_att_type_attachment;
                            else if (strcmp(tmp, esp_mail_content_disposition_type_t::inline_) == 0)
                                res.part.attach_type = esp_mail_att_type_inline;
                        }
                        else
                        {
                            if (strcasecmp(tmp, esp_mail_content_disposition_type_t::attachment) == 0)
                                res.part.attach_type = esp_mail_att_type_attachment;
                            else if (strcasecmp(tmp, esp_mail_content_disposition_type_t::inline_) == 0)
                                res.part.attach_type = esp_mail_att_type_inline;
                        }
                    }
                    // release memory
                    freeMem(&tmp);
                }
            }

            // Content-Transfer-Encoding
            MB_String contentTEName;
            appendHeaderName(contentTEName, message_headers[esp_mail_message_header_field_content_transfer_encoding].text, false, true, false);
            if (strcmpP(res.response, 0, contentTEName.c_str(), caseSensitive))
            {
                // store last text field

                res.part.cur_content_hdr = esp_mail_message_part_info_t::content_header_field_transfer_enc;
                resetStringPtr(res.part);

                tmp = subStr(res.response, contentTEName.c_str(), NULL, 0, -1, caseSensitive);
                if (tmp)
                {
                    res.part.content_transfer_encoding = tmp;

                    if (strcmpP(tmp, 0, esp_mail_transfer_encoding_t::enc_base64))
                        res.part.xencoding = esp_mail_msg_xencoding_base64;
                    else if (strcmpP(tmp, 0, esp_mail_transfer_encoding_t::enc_qp))
                        res.part.xencoding = esp_mail_msg_xencoding_qp;
                    else if (strcmpP(tmp, 0, esp_mail_transfer_encoding_t::enc_7bit))
                        res.part.xencoding = esp_mail_msg_xencoding_7bit;
                    else if (strcmpP(tmp, 0, esp_mail_transfer_encoding_t::enc_8bit))
                        res.part.xencoding = esp_mail_msg_xencoding_8bit;
                    else if (strcmpP(tmp, 0, esp_mail_transfer_encoding_t::enc_binary))
                        res.part.xencoding = esp_mail_msg_xencoding_binary;
                    // release memory
                    freeMem(&tmp);
                }
            }
        }
        else
        {

            if (res.part.cur_content_hdr == esp_mail_message_part_info_t::content_header_field_none)
            {

                resetStringPtr(res.part);

                MB_String field;
                for (int i = esp_mail_rfc822_header_field_from; i < esp_mail_rfc822_header_field_maxType; i++)
                {
                    field = rfc822_headers[i].text;
                    field += esp_mail_str_34; /* ":" */
                    ;

                    int ptr = getRFC822HeaderPtr(i, &res.part.rfc822_header);
                    if (ptr > 0)
                    {
                        if (getDecodedHeader(imap, res.response, field.c_str(), *(addrTo<MB_String *>(ptr)), caseSensitive))
                            return;
                    }
                }
            }
        }

        // parse content type header sub type properties
        if (res.part.cur_content_hdr == esp_mail_message_part_info_t::content_header_field_type)
        {

            if (res.part.msg_type == esp_mail_msg_type_plain || res.part.msg_type == esp_mail_msg_type_enriched)
            {
                MB_String charset;
                appendLowerCaseString(charset, message_headers[esp_mail_message_header_field_charset].text, false);
                // We have to check for both quotes string or non quote string
                if (getPartHeaderProperties(imap, res.response, charset.c_str(), esp_mail_str_11 /* "\"" */, false, value, old_value, res.part.stringEnc, caseSensitive))
                {
                    res.part.charset = value;
                    resetStringPtr(res.part);
                }
                else if (getPartHeaderProperties(imap, res.response, charset.c_str(), esp_mail_str_35 /* ";" */, true, value, old_value, res.part.stringEnc, caseSensitive))
                {
                    res.part.charset = value;
                    resetStringPtr(res.part);
                }

                if (strposP(res.response, esp_mail_str_59 /* "format=flowed" */, 0, caseSensitive) > -1 || strposP(res.response, esp_mail_str_58 /* "format=\"flowed\"" */, 0, caseSensitive) > -1)
                {
                    res.part.plain_flowed = true;
                    resetStringPtr(res.part);
                }

                if (strposP(res.response, esp_mail_str_61 /* "delsp=yes" */, 0, caseSensitive) > -1 || strposP(res.response, esp_mail_str_60 /* "delsp=\"yes\"" */, 0, caseSensitive) > -1)
                {
                    res.part.plain_delsp = true;
                    resetStringPtr(res.part);
                }
            }

            if (res.part.charset.length() == 0)
            {
                MB_String charset;
                appendLowerCaseString(charset, message_headers[esp_mail_message_header_field_charset].text, false);
                // We have to check for both quotes string or non quote string
                if (getPartHeaderProperties(imap, res.response, charset.c_str(), esp_mail_str_11 /* "\"" */, false, value, old_value, res.part.stringEnc, caseSensitive))
                {
                    res.part.charset = value;
                    resetStringPtr(res.part);
                }
                else if (getPartHeaderProperties(imap, res.response, charset.c_str(), esp_mail_str_35 /* ";" */, true, value, old_value, res.part.stringEnc, caseSensitive))
                {
                    res.part.charset = value;
                    resetStringPtr(res.part);
                }
            }

            MB_String name;
            appendLowerCaseString(name, message_headers[esp_mail_message_header_field_name].text, false);
            // We have to check for both quotes string or non quote string
            if (getPartHeaderProperties(imap, res.response, name.c_str(), esp_mail_str_11 /* "\"" */, false, value, old_value, res.part.stringEnc, caseSensitive))
            {
                res.part.stringPtr = toAddr(res.part.name);
                value.trim();
                if (value.length() == 0)
                    return;
            }
            else if (getPartHeaderProperties(imap, res.response, name.c_str(), esp_mail_str_35 /* ";" */, true, value, old_value, res.part.stringEnc, caseSensitive))
            {
                res.part.stringPtr = toAddr(res.part.name);
                value.trim();
                if (value.length() == 0)
                    return;
            }
        }

        // parse content disposition header sub type properties
        if (res.part.cur_content_hdr == esp_mail_message_part_info_t::content_header_field_disposition && res.part.content_disposition.length() > 0)
        {
            // filename prop
            MB_String filename;
            appendLowerCaseString(filename, message_headers[esp_mail_message_header_field_filename].text, false);
            // We have to check for both quotes string or non quote string
            if (getPartHeaderProperties(imap, res.response, filename.c_str(), esp_mail_str_11 /* "\"" */, false, value, old_value, res.part.stringEnc, caseSensitive))
            {
                res.part.stringPtr = toAddr(res.part.filename);
                value.trim();
                if (value.length() == 0)
                    return;
            }
            else if (getPartHeaderProperties(imap, res.response, filename.c_str(), esp_mail_str_35 /* ";" */, true, value, old_value, res.part.stringEnc, caseSensitive))
            {
                res.part.stringPtr = toAddr(res.part.filename);
                value.trim();
                if (value.length() == 0)
                    return;
            }

            // size prop
            MB_String size;
            appendLowerCaseString(size, message_headers[esp_mail_message_header_field_size].text, false);

            if (getPartHeaderProperties(imap, res.response, size.c_str(), esp_mail_str_35 /* ";" */, true, value, old_value, res.part.stringEnc, caseSensitive))
            {
                res.part.attach_data_size = atoi(value.c_str());
                cHeader(imap)->total_attach_data_size += res.part.attach_data_size;
                res.part.sizeProp = true;

                if (!valueStored && old_value.length() > 0)
                    valueStored = storeStringPtr(imap, res.part.stringPtr, old_value, res.response);
                resetStringPtr(res.part);
            }

            // creation date prop
            MB_String creationDate;
            appendLowerCaseString(creationDate, message_headers[esp_mail_message_header_field_creation_date].text, false);

            if (getPartHeaderProperties(imap, res.response, creationDate.c_str(), esp_mail_str_11 /* "\"" */, false, value, old_value, res.part.stringEnc, caseSensitive))
            {
                res.part.creation_date = value;
                if (!valueStored && old_value.length() > 0)
                    valueStored = storeStringPtr(imap, res.part.stringPtr, old_value, res.response);
                resetStringPtr(res.part);
            }

            // mod date prop
            MB_String modDate;
            appendLowerCaseString(modDate, message_headers[esp_mail_message_header_field_modification_date].text, false);

            if (getPartHeaderProperties(imap, res.response, modDate.c_str(), esp_mail_str_11 /* "\"" */, false, value, old_value, res.part.stringEnc, caseSensitive))
            {
                res.part.modification_date = value;
                if (!valueStored && old_value.length() > 0)
                    valueStored = storeStringPtr(imap, res.part.stringPtr, old_value, res.response);
                resetStringPtr(res.part);
            }
        }

        if (!valueStored && (res.part.cur_content_hdr == esp_mail_message_part_info_t::content_header_field_description || res.part.cur_content_hdr == esp_mail_message_part_info_t::content_header_field_type || res.part.cur_content_hdr == esp_mail_message_part_info_t::content_header_field_disposition))
            storeStringPtr(imap, res.part.stringPtr, value, res.response);
    }
}

void ESP_Mail_Client::resetStringPtr(struct esp_mail_message_part_info_t &part)
{
    part.stringPtr = 0;
    part.stringEnc = esp_mail_char_decoding_scheme_default;
}

int ESP_Mail_Client::countChar(const char *buf, char find)
{
    if (!buf)
        return 0;

    int count = 0;

    for (size_t i = 0; i < strlen(buf); i++)
    {
        if (buf[i] == find)
            count++;
    }

    return count;
}

bool ESP_Mail_Client::storeStringPtr(IMAPSession *imap, uint32_t addr, MB_String &value, const char *buf)
{
    if (addr)
    {
        MB_String *a = addrTo<MB_String *>(addr);
        MB_String s = value.length() > 0 ? value : buf;

        // is value string contains double quotes?
        // trim it
        if (countChar(s.c_str(), '"') == 2)
            s.trim();

        if (s[0] == '"')
            s.erase(0, 1);

        if (s[s.length() - 1] == '"')
            s.erase(s.length() - 1, 1);

        decodeString(imap, s);

        *a += s;

        return true;
    }

    return false;
}

bool ESP_Mail_Client::getPartHeaderProperties(IMAPSession *imap, const char *buf, PGM_P p, PGM_P e, bool num, MB_String &value, MB_String &old_value, esp_mail_char_decoding_scheme &scheme, bool caseSensitive)
{

    MB_String str = p;
    str += esp_mail_str_7; /* "=" */
    if (!num)
        str += esp_mail_str_11; /* "\"" */

    char *tmp = subStr(buf, str.c_str(), e, 0, 0, caseSensitive);
    if (!tmp)
    {
        str = p;
        str += esp_mail_str_7; /* "=" */
        tmp = subStr(buf, str.c_str(), e, 0, 0, caseSensitive);
        if (tmp)
        {
            // other sub headers found?
            int p2 = strposP(tmp, esp_mail_str_35 /* ";" */, 0, caseSensitive);
            if (p2 > -1)
            {
                // release memory
                freeMem(&tmp);
                tmp = subStr(buf, str.c_str(), esp_mail_str_35 /* ";" */, 0, 0, caseSensitive);
            }
            // release memory in case above condition does not match
            freeMem(&tmp);
        }
        else
        {
            // Extended notation rfc5987
            str = p;
            str += esp_mail_str_3; /* "*" */
            int p2 = strpos(buf, str.c_str(), 0, caseSensitive);
            if (p2 > -1)
            {
                int p3 = strposP(buf, esp_mail_str_3 /* "*" */, p2 + str.length() + 1, caseSensitive);
                if (p3 > -1 && p3 < (int)strlen(buf))
                {
                    MB_String charset;

                    p3 += 2;

                    int p4 = strpos(buf, "'", p3, caseSensitive);
                    if (p4 > -1)
                    {

                        scheme = getEncodingFromCharset(buf);
                        int c1 = p4 + 1;
                        p4 = strpos(buf, "'", p4 + 1, caseSensitive);
                        int c2 = p4;
                        if (c2 > -1)
                        {
                            charset.append(buf + c1, c2 - c1);
                        }
                        p3 = p4 + 1;
                    }

                    int len = strlen(buf) - p3;
                    tmp = allocMem<char *>(len + 1);

                    if (buf[strlen(buf) - 1] == ';')
                        len--;

                    memcpy(tmp, &buf[p3], len);

                    if (scheme == esp_mail_char_decoding_scheme_utf_8)
                    {
                        char *buf2 = urlDecode(tmp);
                        // release memory and point to new buffer
                        freeMem(&tmp);
                        tmp = buf2;
                    }
                    else if (imap->_charDecCallback)
                    {
                        IMAP_Decoding_Info decoding;
                        decoding.charset = charset.c_str();
                        decoding.data = tmp;
                        decoding.type = IMAP_Decoding_Info::message_part_type_header;

                        imap->_charDecCallback(&decoding);

                        if (decoding.decodedString.length() > 0)
                        {
                            char *buf2 = allocMem<char *>(decoding.decodedString.length() + 1);
                            strcpy(buf2, decoding.decodedString.c_str());
                            // release memory and point to new buffer
                            freeMem(&tmp);
                            tmp = buf2;
                        }
                    }
                    else if (scheme == esp_mail_char_decoding_scheme_iso8859_1)
                    {
                        int ilen = strlen(tmp);
                        int olen = (ilen + 1) * 2;
                        char *buf2 = allocMem<char *>(olen);
                        decodeLatin1_UTF8((unsigned char *)buf2, &olen, (unsigned char *)tmp, &ilen);
                        // release memory and point to new buffer
                        freeMem(&tmp);
                        tmp = buf2;
                    }
                    else if (scheme == esp_mail_char_decoding_scheme_tis_620 || scheme == esp_mail_char_decoding_scheme_iso8859_11 || scheme == esp_mail_char_decoding_scheme_windows_874)
                    {
                        int ilen = strlen(tmp);
                        char *buf2 = allocMem<char *>((ilen + 1) * 3);
                        decodeTIS620_UTF8(buf2, tmp, ilen);
                        // release memory and point to new buffer
                        freeMem(&tmp);
                        tmp = buf2;
                    }
                }
            }
        }
    }

    if (tmp)
    {
        old_value = value;
        value = tmp;
        // release memory
        freeMem(&tmp);
        return true;
    }

    return false;
}

char *ESP_Mail_Client::urlDecode(const char *str)
{
    int d = 0; /* whether or not the string is decoded */
    char *dStr = allocMem<char *>(strlen(str) + 1);
    char eStr[] = "00"; /* for a hex code */

    strcpy(dStr, str);

    while (!d)
    {
        d = 1;
        size_t i; /* the counter for the string */

        for (i = 0; i < strlen(dStr); ++i)
        {

            if (dStr[i] == '%')
            {
                if (dStr[i + 1] == 0)
                    return dStr;

                if (isxdigit(dStr[i + 1]) && isxdigit(dStr[i + 2]))
                {
                    d = 0;

                    /* combine the next to numbers into one */
                    eStr[0] = dStr[i + 1];
                    eStr[1] = dStr[i + 2];

                    /* convert it to decimal */
                    long int x = strtol(eStr, NULL, 16);

                    /* remove the hex */
                    memmove(&dStr[i + 1], &dStr[i + 3], strlen(&dStr[i + 3]) + 1);

                    dStr[i] = x;
                }
            }
        }
    }

    return dStr;
}

bool ESP_Mail_Client::handleIMAPResponse(IMAPSession *imap, int errCode, bool closeSession)
{

    if (!reconnect(imap))
        return false;

    esp_mail_imap_response_data res(imap->client.available());
    imap->_lastProgress = -1;

    // Flag used for CRLF inclusion in response reading in case 8bit/binary attachment and base64 encoded and binary messages
    bool withLineBreak = imap->_imap_cmd == esp_mail_imap_cmd_fetch_body_text && (cPart(imap)->xencoding == esp_mail_msg_xencoding_base64 || cPart(imap)->xencoding == esp_mail_msg_xencoding_binary);
    withLineBreak |= imap->_imap_cmd == esp_mail_imap_cmd_fetch_body_attachment && cPart(imap)->xencoding != esp_mail_msg_xencoding_base64;

    // custom cmd IDLE?, waiting incoming server response
    if (res.chunkBufSize == 0 && imap->_prev_imap_custom_cmd == imap->_imap_custom_cmd && imap->_imap_custom_cmd == esp_mail_imap_cmd_idle)
    {
        if (!reconnect(imap))
            return false;

        return true;
    }

    while (imap->connected() && res.chunkBufSize <= 0)
    {

        if (!reconnect(imap, res.dataTime))
            return false;

        if (!connected<IMAPSession *>(imap))
        {
#if defined(ESP32)
            if (imap->_imap_cmd == esp_mail_imap_cmd_logout) // suppress the error due to server closes the connection immediately in ESP32 core v2.0.4
                return true;
#endif
            if (millis() - imap->_last_network_error_ms > 1000)
            {
                imap->_last_network_error_ms = millis();
                errorStatusCB<IMAPSession *, IMAPSession *>(imap, nullptr, MAIL_CLIENT_ERROR_CONNECTION_CLOSED, true);
            }

            return false;
        }
        res.chunkBufSize = imap->client.available();
        yield_impl();
    }

    res.dataTime = millis();

    if (res.chunkBufSize > 0)
    {
        if (imap->_imap_cmd == esp_mail_imap_cmd_examine)
        {
            imap->_mbif.clear();
            imap->_nextUID.clear();
            imap->_unseenMsgIndex.clear();
        }

        if (imap->_imap_cmd == esp_mail_imap_cmd_search)
        {
            imap->_mbif._searchCount = 0;
            imap->_imap_msg_num.clear();
        }

        // response buffer
        res.chunkBufSize = ESP_MAIL_CLIENT_RESPONSE_BUFFER_SIZE;
        res.response = allocMem<char *>(res.chunkBufSize + 1);

        if (imap->_imap_cmd == esp_mail_imap_cmd_fetch_body_attachment || imap->_imap_cmd == esp_mail_imap_cmd_fetch_body_inline)
            res.lastBuf = allocMem<char *>(BASE64_CHUNKED_LEN + 1);

        while (!res.completedResponse) // looking for operation finishing
        {
            yield_impl();

            if (imap->_imap_cmd == esp_mail_imap_cmd_append || (imap->_imap_custom_cmd == esp_mail_imap_cmd_append && imap->_imap_cmd == esp_mail_imap_cmd_custom && imap->_customCmdResCallback))
            {
                // No waiting time out for APPEND
                res.dataTime = millis();
            }

            if (!reconnect(imap, res.dataTime) || !connected<IMAPSession *>(imap))
            {

                if (!connected<IMAPSession *>(imap))
                {

                    if (cPart(imap) && cPart(imap)->file_open_write)
                        mbfs->close(mbfs_type imap->_imap_data->storage.type);

#if defined(ESP32)
                    if (imap->_imap_cmd == esp_mail_imap_cmd_logout) // suppress the error due to server closes the connection immediately in ESP32 core v2.0.4
                        return true;
#endif

                    if (millis() - imap->_last_network_error_ms > 1000)
                    {
                        imap->_last_network_error_ms = millis();
                        errorStatusCB<IMAPSession *, IMAPSession *>(imap, nullptr, MAIL_CLIENT_ERROR_CONNECTION_CLOSED, true);
                    }

                    return false;
                }
                return false;
            }

            res.chunkBufSize = imap->client.available();

            if (res.chunkBufSize > 0)
            {
                res.chunkBufSize = ESP_MAIL_CLIENT_RESPONSE_BUFFER_SIZE;

                if (imap->_imap_cmd == esp_mail_imap_cmd_search)
                {

                    res.readLen = parseSearchResponse(imap, res, esp_mail_imap_tag_str, imap_responses[esp_mail_imap_response_search].text);
                    imap->_mbif._availableItems = imap->_imap_msg_num.size();

                    if (imap->_mbif._availableItems == 0)
                    {
                        res.imapResp = imapResponseStatus(imap, res.response, esp_mail_imap_tag_str);

                        if (res.imapResp != esp_mail_imap_resp_unknown)
                            res.endSearch = true;

                        if (res.imapResp == esp_mail_imap_resp_bad)
                        {
                            errorStatusCB<IMAPSession *, IMAPSession *>(imap, nullptr, IMAP_STATUS_BAD_COMMAND, false);
                            return false;
                        }
                    }
                }
                else
                {
                    // response read as chunk ended with CRLF or complete buffer size
                    int o = res.octetCount;
                    res.readLen = 0;
                    MB_String ovfBuf;
                    if (!readResponse<IMAPSession *>(imap, res.response, res.chunkBufSize, res.readLen, withLineBreak, res.octetCount, ovfBuf))
                    {
                        errorStatusCB<IMAPSession *, IMAPSession *>(imap, nullptr, MAIL_CLIENT_ERROR_READ_TIMEOUT, true);
                        return false;
                    }

                    // If buffer overflown, copy from overflow buffer
                    if (ovfBuf.length() > 0)
                    {
                        // release memory
                        freeMem(&res.response);
                        res.response = allocMem<char *>(ovfBuf.length() + 1);
                        strcpy(res.response, ovfBuf.c_str());
                        ovfBuf.clear();
                    }

                    if (res.readLen == 0 && o != res.octetCount && res.octetCount <= res.octetLength && cPart(imap)->xencoding != esp_mail_msg_xencoding_base64 && cPart(imap)->xencoding != esp_mail_msg_xencoding_binary && cPart(imap)->xencoding != esp_mail_msg_xencoding_qp)
                    {
                        strcpy_P(res.response, esp_mail_str_42 /* "\r\n" */);
                        res.readLen = 2;
                    }
                }

                if (res.readLen)
                {
                    if (imap->_debug && imap->_debugLevel > esp_mail_debug_level_basic && !imap->_customCmdResCallback)
                    {
                        if (imap->_imap_cmd != esp_mail_imap_cmd_search && imap->_imap_cmd != esp_mail_imap_cmd_fetch_body_text && imap->_imap_cmd != esp_mail_imap_cmd_fetch_body_attachment && imap->_imap_cmd != esp_mail_imap_cmd_fetch_body_inline)
                            esp_mail_debug_print((const char *)res.response, true);
                    }

                    if (imap->_imap_cmd != esp_mail_imap_cmd_search || (imap->_imap_cmd == esp_mail_imap_cmd_search && res.endSearch))
                        res.imapResp = imapResponseStatus(imap, res.response, esp_mail_imap_tag_str);

                    if (res.imapResp != esp_mail_imap_resp_unknown)
                    {

                        // We've got the right response,
                        // prepare to exit

                        res.completedResponse = true;

                        if (imap->_debug && imap->_debugLevel > esp_mail_debug_level_basic && !imap->_customCmdResCallback)
                        {
                            if (imap->_imap_cmd == esp_mail_imap_cmd_fetch_body_text || imap->_imap_cmd == esp_mail_imap_cmd_fetch_body_attachment || imap->_imap_cmd == esp_mail_imap_cmd_fetch_body_inline)
                                esp_mail_debug_print((const char *)res.response, true);
                        }

                        MB_String ovfBuf;

                        while (imap->client.available())
                        {

                            if (!readResponse<IMAPSession *>(imap, res.response, res.chunkBufSize, res.readLen, true, res.octetCount, ovfBuf))
                            {
                                errorStatusCB<IMAPSession *, IMAPSession *>(imap, nullptr, MAIL_CLIENT_ERROR_READ_TIMEOUT, true);
                                return false;
                            }
                            // If buffer overflown, copy from overflow buffer
                            if (ovfBuf.length() > 0)
                            {
                                // release memory
                                freeMem(&res.response);
                                res.response = allocMem<char *>(ovfBuf.length() + 1);
                                strcpy(res.response, ovfBuf.c_str());
                                ovfBuf.clear();
                            }

                            if (res.readLen)
                            {
                                if (imap->_debug && imap->_debugLevel > esp_mail_debug_level_basic && !imap->_customCmdResCallback)
                                {
                                    if (imap->_imap_cmd == esp_mail_imap_cmd_fetch_body_text || imap->_imap_cmd == esp_mail_imap_cmd_fetch_body_attachment || imap->_imap_cmd == esp_mail_imap_cmd_fetch_body_inline)
                                        esp_mail_debug_print((const char *)res.response, true);
                                }
                            }
                        }
                    }
                    else
                    {

                        // No response ever parsed

                        if (imap->_imap_cmd == esp_mail_imap_cmd_sasl_auth_plain || imap->_imap_cmd == esp_mail_imap_cmd_sasl_auth_oauth)
                        {
                            if (imap->_imap_cmd == esp_mail_imap_cmd_sasl_auth_oauth)
                            {
                                if (isOAuthError(res.response, res.readLen, res.chunkIdx, 2))
                                    res.completedResponse = true;
                            }

                            // In case SASL-IR extension does not support, check for initial zero-length server challenge first "+ "
                            if (!imap->_auth_capability[esp_mail_auth_capability_sasl_ir] && strcmp(res.response, pgm2Str(esp_mail_str_63 /* "+ " */)) == 0)
                            {
                                res.imapResp = esp_mail_imap_resp_ok;
                                res.completedResponse = true;
                            }
                        }

                        if (imap->_imap_cmd == esp_mail_imap_cmd_sasl_login || imap->_imap_cmd == esp_mail_imap_cmd_sasl_auth_oauth || imap->_imap_cmd == esp_mail_imap_cmd_sasl_auth_plain)
                        {
                            int i = 0;
                            if (parseCapabilityResponse(imap, res.response, i))
                                imap->_feature_capability[esp_mail_imap_read_capability_auto_caps] = true;
                        }
                        else if (imap->_imap_cmd == esp_mail_imap_cmd_custom && imap->_customCmdResCallback)
                        {

                            res.imapResp = imapResponseStatus(imap, res.response, imap->_responseStatus.tag.c_str());

                            // get response or custom cmd APPEND or custom cmd IDLE?
                            if (res.imapResp > esp_mail_imap_resp_unknown || strposP(imap->_cmd.c_str(), imap_commands[esp_mail_imap_command_append].text, 0, false) > -1 || imap->_imap_custom_cmd == esp_mail_imap_cmd_idle)
                                res.completedResponse = true;

                            imap->_responseStatus.text = res.response;

                            imap->_customCmdResCallback(imap->_responseStatus);

                            if (res.completedResponse)
                                return true;
                        }
                        else if (imap->_imap_cmd == esp_mail_imap_cmd_append)
                        {
                            res.imapResp = esp_mail_imap_resp_ok;
                            res.completedResponse = true;
                        }
                        else if (imap->_imap_cmd == esp_mail_imap_cmd_capability)
                            parseCapabilityResponse(imap, res.response, res.chunkIdx);
                        else if (imap->_imap_cmd == esp_mail_imap_cmd_list)
                            parseFoldersResponse(imap, res.response, true);
                        else if (imap->_imap_cmd == esp_mail_imap_cmd_lsub)
                            parseFoldersResponse(imap, res.response, false);
                        else if (imap->_imap_cmd == esp_mail_imap_cmd_select || imap->_imap_cmd == esp_mail_imap_cmd_examine)
                            parseExamineResponse(imap, res.response);
                        else if (imap->_imap_cmd == esp_mail_imap_cmd_get_uid)
                        {
                            MB_String str;
                            appendFetchString(str, true);
                            parseCmdResponse(imap, res.response, str.c_str());
                        }
                        else if (imap->_imap_cmd == esp_mail_imap_cmd_get_flags)
                        {
                            MB_String str;
                            appendFetchString(str, false);
                            parseCmdResponse(imap, res.response, str.c_str());
                        }
                        else if (imap->_imap_cmd == esp_mail_imap_cmd_get_quota)
                            parseCmdResponse(imap, res.response, imap_responses[esp_mail_imap_response_quota].text);
                        else if (imap->_imap_cmd == esp_mail_imap_cmd_id)
                            parseCmdResponse(imap, res.response, imap_responses[esp_mail_imap_response_id].text);
                        else if (imap->_imap_cmd == esp_mail_imap_cmd_get_quota_root)
                        {
                            parseCmdResponse(imap, res.response, imap_responses[esp_mail_imap_response_quotaroot].text);
                            imap->_quota_tmp.clear();
                            parseCmdResponse(imap, res.response, imap_responses[esp_mail_imap_response_quota].text);
                            if (imap->_quota_tmp.length() > 0)
                            {
                                imap->_quota_root_tmp += (char)':';
                                imap->_quota_root_tmp += imap->_quota_tmp;
                            }
                        }
                        else if (imap->_imap_cmd == esp_mail_imap_cmd_get_acl || imap->_imap_cmd == esp_mail_imap_cmd_my_rights)
                            parseCmdResponse(imap, res.response, imap->_imap_cmd == esp_mail_imap_cmd_get_acl ? imap_responses[esp_mail_imap_response_acl].text : imap_responses[esp_mail_imap_response_myrights].text);
                        else if (imap->_imap_cmd == esp_mail_imap_cmd_namespace)
                            parseCmdResponse(imap, res.response, imap_responses[esp_mail_imap_response_namespace].text);
                        else if (imap->_imap_cmd == esp_mail_imap_cmd_idle)
                        {
                            res.completedResponse = res.response[0] == '+';
                            res.imapResp = esp_mail_imap_resp_ok;

                            imap->_last_host_check_ms = millis();
                        }
                        else if (imap->_imap_cmd == esp_mail_imap_cmd_fetch_sequence_set)
                        {
                            MB_String str;
                            appendFetchString(str, true);
                            parseCmdResponse(imap, res.response, str.c_str());
                        }
                        else if (imap->_imap_cmd == esp_mail_imap_cmd_fetch_body_header)
                        {

                            if (res.headerState == 0 && cMSG(imap).type == esp_mail_imap_msg_num_type_uid)
                                res.header.message_uid = cMSG(imap).value;

                            int _st = res.headerState;
                            parseHeaderResponse(imap, res, imap->_imap_data->enable.header_case_sensitive);
                            if (_st == res.headerState && res.headerState > 0 && res.octetCount <= res.header.header_data_len)
                                collectHeaderField(imap, res.response, res.header, res.headerState);
                        }
                        else if (imap->_imap_cmd == esp_mail_imap_cmd_fetch_body_mime)
                            parsePartHeaderResponse(imap, res, imap->_imap_data->enable.header_case_sensitive);
                        else if (imap->_imap_cmd == esp_mail_imap_cmd_fetch_body_text)
                            decodeText(imap, res);
                        else if (imap->_imap_cmd == esp_mail_imap_cmd_fetch_body_attachment || imap->_imap_cmd == esp_mail_imap_cmd_fetch_body_inline)
                        {

                            if (cPart(imap)->xencoding == esp_mail_msg_xencoding_base64)
                            {
                                // Multi-line chunked base64 string attachment handle
                                if (res.octetCount < res.octetLength && res.readLen < BASE64_CHUNKED_LEN)
                                {
                                    if (strlen(res.lastBuf) > 0)
                                    {
                                        res.buf = allocMem<char *>(res.readLen + strlen(res.lastBuf) + 2);
                                        strcpy(res.buf, res.lastBuf);
                                        strcat(res.buf, res.response);
                                        res.readLen = strlen(res.buf);
                                        res.tmo = parseAttachmentResponse(imap, res.buf, res);
                                        // release memory
                                        freeMem(&res.buf);
                                        memset(res.lastBuf, 0, BASE64_CHUNKED_LEN + 1);
                                        if (!res.tmo)
                                            break;
                                    }
                                    else if (res.readLen < BASE64_CHUNKED_LEN + 1)
                                        strcpy(res.lastBuf, res.response);
                                }
                                else
                                {
                                    res.tmo = parseAttachmentResponse(imap, res.response, res);
                                    if (!res.tmo)
                                        break;
                                }
                            }
                            else
                                res.tmo = parseAttachmentResponse(imap, res.response, res);
                        }

                        res.dataTime = millis();
                    }
                }
                memset(res.response, 0, res.chunkBufSize);
            }
        }

        if (imap->_imap_cmd == esp_mail_imap_cmd_search)
        {
            if (imap->_debug && res.searchCount > 0 && res.searchCount < 100)
            {
                searchReport(imap, 100);
            }
        }
    }

    if ((res.imapResp != esp_mail_imap_resp_ok && imap->_imap_cmd == esp_mail_imap_cmd_fetch_body_header && res.header.header_data_len == 0) || res.imapResp == esp_mail_imap_resp_no)
    {
        // We don't get any response

        if (res.imapResp == esp_mail_imap_resp_no)
            imap->_responseStatus.errorCode = IMAP_STATUS_IMAP_RESPONSE_FAILED;
        else if (imap->_imap_data->fetch.modsequence > -1 && imap->isModseqSupported() && res.imapResp == esp_mail_imap_resp_ok && res.header.header_data_len == 0)
            imap->_responseStatus.errorCode = IMAP_STATUS_CHANGEDSINC_MODSEQ_TEST_FAILED;
        else
            imap->_responseStatus.errorCode = IMAP_STATUS_NO_MESSAGE;

#if !defined(SILENT_MODE)

        if (imap->_statusCallback && imap->_imap_cmd != esp_mail_imap_cmd_fetch_body_mime)
            sendErrorCB<IMAPSession *>(imap, imap->errorReason().c_str(), false, false);

        if (imap->_debug && imap->_imap_cmd != esp_mail_imap_cmd_fetch_body_mime)
            esp_mail_debug_print_tag(imap->errorReason().c_str(), esp_mail_debug_tag_type_error, true);

#endif

        return false;
    }

    // We've got OK or NO responses

    if (res.imapResp == esp_mail_imap_resp_ok)
    {
        // Response OK

        if (imap->_imap_cmd == esp_mail_imap_cmd_fetch_body_header)
        {
            // Headers management

            char *buf = allocMem<char *>(res.header.content_type.length() + 1);
            strcpy(buf, res.header.content_type.c_str());
            res.header.content_type.clear();

            MB_String contentTypeName;
            appendHeaderName(contentTypeName, message_headers[esp_mail_message_header_field_content_type].text, false, false, false);

            res.buf = subStr(buf, contentTypeName.c_str(), esp_mail_str_35 /* ";" */, 0, 0, false);
            if (res.buf)
            {
                res.headerState = esp_mail_imap_state_content_type;
                collectHeaderField(imap, res.buf, res.header, res.headerState);
                // release memory
                freeMem(&res.buf);

                if (res.header.content_type.length() > 0)
                {
                    int p1 = strposP(res.header.content_type.c_str(), esp_mail_imap_composite_media_type_t::multipart, 0);
                    if (p1 != -1)
                    {
                        p1 += strlen(esp_mail_imap_composite_media_type_t::multipart) + 1;
                        res.header.multipart = true;
                        // inline or embedded images
                        if (strpos(res.header.content_type.c_str(), esp_mail_imap_multipart_sub_type_t::related, p1) != -1)
                            res.header.multipart_sub_type = esp_mail_imap_multipart_sub_type_related;
                        // multiple text formats e.g. plain, html, enriched
                        else if (strpos(res.header.content_type.c_str(), esp_mail_imap_multipart_sub_type_t::alternative, p1) != -1)
                            res.header.multipart_sub_type = esp_mail_imap_multipart_sub_type_alternative;
                        // medias
                        else if (strpos(res.header.content_type.c_str(), esp_mail_imap_multipart_sub_type_t::parallel, p1) != -1)
                            res.header.multipart_sub_type = esp_mail_imap_multipart_sub_type_parallel;
                        // rfc822 encapsulated
                        else if (strpos(res.header.content_type.c_str(), esp_mail_imap_multipart_sub_type_t::digest, p1) != -1)
                            res.header.multipart_sub_type = esp_mail_imap_multipart_sub_type_digest;
                        else if (strpos(res.header.content_type.c_str(), esp_mail_imap_multipart_sub_type_t::report, p1) != -1)
                            res.header.multipart_sub_type = esp_mail_imap_multipart_sub_type_report;
                        // others can be attachments
                        else if (strpos(res.header.content_type.c_str(), esp_mail_imap_multipart_sub_type_t::mixed, p1) != -1)
                            res.header.multipart_sub_type = esp_mail_imap_multipart_sub_type_mixed;
                    }

                    p1 = strposP(res.header.content_type.c_str(), esp_mail_imap_composite_media_type_t::message, 0);
                    if (p1 != -1)
                    {
                        p1 += strlen(esp_mail_imap_composite_media_type_t::message) + 1;
                        if (strpos(res.part.content_type.c_str(), esp_mail_imap_message_sub_type_t::rfc822, p1) != -1)
                        {
                            res.header.rfc822_part = true;
                            res.header.message_sub_type = esp_mail_imap_message_sub_type_rfc822;
                        }
                        else if (strpos(res.part.content_type.c_str(), esp_mail_imap_message_sub_type_t::Partial, p1) != -1)
                            res.header.message_sub_type = esp_mail_imap_message_sub_type_partial;
                        else if (strpos(res.part.content_type.c_str(), esp_mail_imap_message_sub_type_t::External_Body, p1) != -1)
                            res.header.message_sub_type = esp_mail_imap_message_sub_type_external_body;
                        else if (strpos(res.part.content_type.c_str(), esp_mail_imap_message_sub_type_t::delivery_status, p1) != -1)
                            res.header.message_sub_type = esp_mail_imap_message_sub_type_delivery_status;
                    }
                }

                MB_String charset;
                appendLowerCaseString(charset, message_headers[esp_mail_message_header_field_charset].text, false);
                charset += esp_mail_str_7; /* "=" */

                res.buf = subStr(buf, charset.c_str(), NULL, 0, -1, false);
                if (res.buf)
                {
                    res.headerState = esp_mail_imap_state_char_set;
                    collectHeaderField(imap, res.buf, res.header, res.headerState);
                    // release memory
                    freeMem(&res.buf);
                }

                if (res.header.multipart)
                {
                    if (strcmpP(buf, 0, esp_mail_str_64 /* "boundary=\"" */))
                    {
                        res.buf = subStr(buf, esp_mail_str_64 /* "boundary=\"" */, esp_mail_str_11 /* "\"" */, 0, 0, false);
                        if (res.buf)
                        {
                            res.headerState = esp_mail_imap_state_boundary;
                            collectHeaderField(imap, res.buf, res.header, res.headerState);
                            // release memory
                            freeMem(&res.buf);
                        }
                    }
                }
            }

            // release memory
            freeMem(&buf);

            // Decode the headers fields

            for (int i = esp_mail_rfc822_header_field_from; i < esp_mail_rfc822_header_field_maxType; i++)
            {
                if (i != esp_mail_rfc822_header_field_msg_id && i != esp_mail_rfc822_header_field_flags)
                    decodeString(imap, res.header.header_fields.header_items[i]);
            }

            imap->_headers.push_back(res.header);
        }

        if (imap->_imap_cmd == esp_mail_imap_cmd_fetch_body_mime)
        {
            // Expect the octet length in the response for the existent part
            if (res.part.octetLen > 0)
            {

                res.part.partNumStr = cHeader(imap)->partNumStr;
                res.part.partNumFetchStr = cHeader(imap)->partNumStr;
                if (cHeader(imap)->part_headers.size() > 0)
                {

                    struct esp_mail_message_part_info_t *_part = &cHeader(imap)->part_headers[cHeader(imap)->part_headers.size() - 1];
                    bool rfc822_body_subtype = _part->message_sub_type == esp_mail_imap_message_sub_type_rfc822;

                    if (rfc822_body_subtype)
                    {
                        if (!_part->rfc822_part)
                        {
                            // additional rfc822 message header, store it to the rfc822 part header
                            _part->rfc822_part = true;
                            _part->rfc822_header = res.part.rfc822_header;
                            imap->_rfc822_part_count++;
                            _part->rfc822_msg_Idx = imap->_rfc822_part_count;
                        }
                    }
                }

                cHeader(imap)->part_headers.push_back(res.part);
                cHeader(imap)->message_data_count = cHeader(imap)->part_headers.size();

                if (res.part.msg_type != esp_mail_msg_type_none ||
                    res.part.attach_type != esp_mail_att_type_none)
                {
                    if (res.part.attach_type == esp_mail_att_type_attachment ||
                        res.part.message_sub_type != esp_mail_imap_message_sub_type_rfc822)
                    {
                        if (res.part.attach_type != esp_mail_att_type_none &&
                            cHeader(imap)->multipart_sub_type != esp_mail_imap_multipart_sub_type_alternative)
                        {
                            cHeader(imap)->hasAttachment = true;
                            cHeader(imap)->attachment_count++;
                        }
                    }
                }
            }
            else
            {
                // nonexistent part
                // return false to exit the loop without closing the connection
                if (closeSession)
                    imap->closeSession();
                return false;
            }
        }

        if (imap->_imap_cmd == esp_mail_imap_cmd_fetch_body_attachment || imap->_imap_cmd == esp_mail_imap_cmd_fetch_body_text || imap->_imap_cmd == esp_mail_imap_cmd_fetch_body_inline)
        {
            if (cPart(imap) && cPart(imap)->file_open_write)
                mbfs->close(mbfs_type imap->_imap_data->storage.type);
        }

        if (cPart(imap) && imap->_imap_cmd == esp_mail_imap_cmd_fetch_body_text)
            cPart(imap)->text[cPart(imap)->textLen] = 0;
    }
    else
    {

        // Response NO

        // Some server responses NO and should exit (false) from MIME feching loop without
        // closing the session
        if (imap->_imap_cmd != esp_mail_imap_cmd_fetch_body_mime)
            return handleIMAPError(imap, errCode, false);

        if (closeSession)
            imap->closeSession();
        return false;
    }

    return true;
}

void ESP_Mail_Client::addHeader(MB_String &s, PGM_P name, const char *s_value, int num_value, bool trim, bool isJson)
{
    if (isJson)
    {
        s += (s.length() > 0) ? esp_mail_str_77 /* ",\"" */ : esp_mail_str_78 /* "{\"" */;
        s += name;

        if (strlen(s_value) > 0)
        {
            s += esp_mail_str_79; /* "\":\"" */
            ;
            if (trim)
            {
                MB_String t = s_value;
                t.replaceAll("\"", "");
                s += t;
            }
            else
                s += s_value;
            s += esp_mail_str_11; /* "\"" */
        }
        else
        {
            s += esp_mail_str_34 /* ":" */;
            appendSpace(s);
            s += num_value;
        }
    }
    else
    {
        if (s.length() > 0)
            appendNewline(s);
        s += name;
        s += esp_mail_str_34 /* ":" */;
        appendSpace(s);
        if (strlen(s_value) > 0)
            s += s_value;
        else
            s += num_value;
    }
}

void ESP_Mail_Client::saveHeader(IMAPSession *imap, bool json)
{
    if (!imap->_storageReady)
        return;

    MB_String headerFilePath;

    prepareFilePath(imap, headerFilePath, true);

    headerFilePath += json ? esp_mail_str_65 /* "/header.json" */ : esp_mail_str_66 /* "/header.txt" */;

    prepareFileList(imap, headerFilePath);

    int sz = mbfs->open(headerFilePath, mbfs_type imap->_imap_data->storage.type, mb_fs_open_mode_write);
    if (sz < 0)
    {
        imap->_responseStatus.errorCode = sz;
        imap->_responseStatus.text.clear();
#if !defined(SILENT_MODE)
        if (imap->_debug)
        {
            esp_mail_debug_print_tag(imap->errorReason().c_str(), esp_mail_debug_tag_type_error, true);
        }
#endif
        return;
    }

    MB_String s;
    for (size_t i = 0; i < imap->_headers.size(); i++)
        addHeaderItem(s, &imap->_headers[i], json);

    if (json)
        s += esp_mail_str_75; /* "]}" */

    mbfs->print(mbfs_type imap->_imap_data->storage.type, s.c_str());

    mbfs->close(mbfs_type imap->_imap_data->storage.type);

    imap->_headerSaved = true;
}

void ESP_Mail_Client::addHeaderItem(MB_String &str, esp_mail_message_header_t *header, bool json)
{
    MB_String s;
    if (json)
    {
        if (str.length() > 0)
            str += esp_mail_str_8; /* "," */
        else
            str = esp_mail_str_76; /* "{\"Messages\":[" */
    }

    addHeader(s, message_headers[esp_mail_message_header_field_number].text, "", header->message_no, false, json);
    addHeader(s, message_headers[esp_mail_message_header_field_uid].text, "", header->message_uid, false, json);

    if (header->accept_language.length() > 0)
        addHeader(s, message_headers[esp_mail_message_header_field_accept_language].text, header->accept_language.c_str(), 0, false, json);

    if (header->content_language.length() > 0)
        addHeader(s, message_headers[esp_mail_message_header_field_content_language].text, header->content_language.c_str(), 0, false, json);

    addRFC822Headers(s, &header->header_fields, json);

    for (size_t j = 0; j < header->part_headers.size(); j++)
    {
        if (header->part_headers[j].rfc822_part)
        {
            MB_String s1;
            addRFC822Headers(s1, &header->part_headers[j].rfc822_header, json);

            if (json)
            {
                s += esp_mail_str_69; /* ",\"RFC822\":" */
                s += s1;
                s += esp_mail_str_36; /* "}" */
            }
            else
            {
                s += esp_mail_str_70; /* "\r\n\r\nRFC822:\r\n" */
                s += s1;
            }
        }
    }

    if (header->attachment_count > 0)
    {
        if (json)
        {
            s += esp_mail_str_71 /* ",\"Attachments\":{\"Count\":" */;
            s += header->attachment_count;
            s += esp_mail_str_72; /* ",\"Files\":[" */
        }
        else
        {
            s += esp_mail_str_73; /* "\r\n\r\nAttachments (" */
            s += header->attachment_count;
            s += esp_mail_str_74; /* ")\r\n" */
        }

        int index = 0;
        for (size_t j = 0; j < header->part_headers.size(); j++)
        {

            if (header->part_headers[j].attach_type == esp_mail_att_type_none || header->part_headers[j].rfc822_part)
                continue;

            if (json)
            {
                if (index > 0)
                    s += esp_mail_str_8; /* "," */
                s += esp_mail_str_67;    /* /"{\"Filename\":\"" */
                s += header->part_headers[j].filename;
                s += esp_mail_str_11; /* "\"" */
            }
            else
            {
                if (index > 0)
                    appendNewline(s);
                appendNewline(s);
                s += esp_mail_str_68; /* /"Index: " */
                s += index + 1;
                addHeader(s, message_headers[esp_mail_message_header_field_filename].text, header->part_headers[j].filename.c_str(), 0, false, json);
            }

            addHeader(s, message_headers[esp_mail_message_header_field_name].text, header->part_headers[j].name.c_str(), 0, false, json);
            addHeader(s, message_headers[esp_mail_message_header_field_size].text, "", header->part_headers[j].attach_data_size, false, json);
            addHeader(s, message_headers[esp_mail_message_header_field_mime].text, header->part_headers[j].content_type.c_str(), 0, false, json);
            addHeader(s, message_headers[esp_mail_message_header_field_type].text, header->part_headers[j].attach_type == esp_mail_att_type_attachment ? esp_mail_content_disposition_type_t::attachment : esp_mail_content_disposition_type_t::inline_, 0, false, json);
            addHeader(s, message_headers[esp_mail_message_header_field_description].text, header->part_headers[j].content_description.c_str(), 0, false, json);
            addHeader(s, message_headers[esp_mail_message_header_field_creation_date].text, header->part_headers[j].creation_date.c_str(), 0, false, json);

            if (json)
                s += esp_mail_str_36; /* "}" */

            index++;
        }

        if (json)
            s += esp_mail_str_75; /* "]}" */
    }

    if (json)
    {
        s += esp_mail_str_36; /* "}" */
    }

    str += s;
}

int ESP_Mail_Client::getRFC822HeaderPtr(int index, esp_mail_imap_rfc822_msg_header_item_t *header)
{
    if (index > esp_mail_rfc822_header_field_from && index < esp_mail_rfc822_header_field_maxType)
        return toAddr(header->header_items[index]);
    return 0;
}

void ESP_Mail_Client::addRFC822Headers(MB_String &s, esp_mail_imap_rfc822_msg_header_item_t *header, bool json)
{
    for (int i = esp_mail_rfc822_header_field_from; i < esp_mail_rfc822_header_field_maxType; i++)
        addRFC822HeaderItem(s, header, i, json);
}

void ESP_Mail_Client::addRFC822HeaderItem(MB_String &s, esp_mail_imap_rfc822_msg_header_item_t *header, int index, bool json)
{
    int ptr = getRFC822HeaderPtr(index, header);
    if (ptr > 0)
        addHeader(s, rfc822_headers[index].text, addrTo<MB_String *>(ptr)->c_str(), 0, rfc822_headers[index].trim, json);
}

esp_mail_imap_response_status ESP_Mail_Client::imapResponseStatus(IMAPSession *imap, char *response, PGM_P tag)
{
    imap->_responseStatus.clear(false);
    MB_String test;
    esp_mail_imap_response_status status = esp_mail_imap_resp_unknown;
    esp_mail_imap_response_types type = esp_mail_imap_response_maxType;

    if (strpos(response, imap->prependTag(imap_responses[esp_mail_imap_response_ok].text, tag).c_str(), 0) > -1)
    {
        status = esp_mail_imap_resp_ok;
        type = esp_mail_imap_response_ok;
    }
    else if (strpos(response, imap->prependTag(imap_responses[esp_mail_imap_response_no].text, tag).c_str(), 0) > -1)
    {
        status = esp_mail_imap_resp_no;
        type = esp_mail_imap_response_no;
    }
    else if (strpos(response, imap->prependTag(imap_responses[esp_mail_imap_response_bad].text, tag).c_str(), 0) > -1)
    {
        status = esp_mail_imap_resp_bad;
        type = esp_mail_imap_response_bad;
    }

    if (status != esp_mail_imap_resp_unknown)
    {
        test = imap->prependTag(imap_responses[type].text, tag);

        imap->_responseStatus.text = &response[test.length()];
        if (imap->_responseStatus.text[imap->_responseStatus.text.length() - 2] == '\r')
            imap->_responseStatus.text[imap->_responseStatus.text.length() - 2] = 0;

        test = imap_responses[type].text;
        test.trim();
        imap->_responseStatus.status = test;
        imap->_responseStatus.completed = true;
    }
    return status;
}

bool ESP_Mail_Client::parseCapabilityResponse(IMAPSession *imap, const char *buf, int &chunkIdx)
{
    if (chunkIdx == 0)
    {
        MB_String res;
        // We add white space to make post token checking to work in all capabilities.
        // This will allow us to check "IDLE " and "ID " correctly.
        appendSpace(res, false, buf);

        if (strposP(res.c_str(), imap_responses[esp_mail_imap_response_capability_untagged].text, 0) > -1 || strposP(res.c_str(), imap_responses[esp_mail_imap_response_capability].text, 0) > -1)
        {
            for (int i = esp_mail_auth_capability_plain; i < esp_mail_auth_capability_maxType; i++)
            {
                if (strposP(res.c_str(), imap_auth_cap_pre_tokens[i].c_str(), 0) > -1)
                    imap->_auth_capability[i] = true;
            }

            for (int i = esp_mail_imap_read_capability_imap4; i < esp_mail_imap_read_capability_maxType; i++)
            {
                if (strposP(res.c_str(), imap_read_cap_post_tokens[i].c_str(), 0) > -1)
                {
                    imap->_feature_capability[i] = true;
                    if (i == esp_mail_imap_read_capability_logindisable)
                        imap->_auth_capability[esp_mail_auth_capability_login] = false;
                }
            }

            return true;
        }
    }

    return false;
}

char *ESP_Mail_Client::getList(char *buf, bool &isList)
{
    if (buf[0] == '(' && buf[1] != ')')
    {
        if (buf[strlen(buf) - 1] == ')')
            buf[strlen(buf) - 1] = 0;
        else
            isList = true;

        return &buf[1];
    }
    else if (isList)
    {
        if (buf[strlen(buf) - 1] == ')')
        {
            buf[strlen(buf) - 1] = 0;
            isList = false;
        }
    }

    return buf;
}

void ESP_Mail_Client::parseFoldersResponse(IMAPSession *imap, char *buf, bool list)
{
    struct esp_mail_folder_info_t fd;
    int pos = list ? strposP(buf, imap_responses[esp_mail_imap_response_list].text, 0) : strposP(buf, imap_responses[esp_mail_imap_response_lsub].text, 0);
    bool isList = false, delimOk = false;
    if (pos != -1)
    {
        char *p = allocMem<char *>(strlen(buf));
        strcpy(p, buf);
        char *pp = p;
        char *end = p;
        int count = 0;
        int tkPos = 3;

        while (pp != NULL)
        {
            // See RFC2047.h
            ESP_MAIL_STRSEP(&end, " ");
            count++;

            if (count >= tkPos && strlen(pp) > 0)
            {
                if (count == tkPos && pp[0] == '(' && pp[1] != ')')
                    fd.attributes = getList(pp, isList);
                else if (isList)
                {
                    fd.attributes += ' ';
                    fd.attributes += getList(pp, isList);
                }
                else
                {
                    if (pp[strlen(pp) - 1] == '"')
                        pp[strlen(pp) - 1] = 0;

                    const char *ptr = pp[0] == '"' ? &pp[1] : &pp[0];

                    if (!delimOk)
                    {
                        delimOk = true;
                        fd.delimiter = ptr;
                    }
                    else
                    {
                        if (fd.name.length() > 0)
                            fd.name += ' ';
                        fd.name += ptr;
                    }
                }
            }
            pp = end;
        }

        // release memory
        freeMem(&p);

        imap->_folders.add(fd);
    }
}

bool ESP_Mail_Client::parseIdleResponse(IMAPSession *imap)
{

    int chunkBufSize = 0;

    if (!reconnect(imap))
        return false;

    if (imap->client.connected())
        chunkBufSize = imap->client.available();
    else
        return false;

    if (chunkBufSize > 0)
    {
        chunkBufSize = ESP_MAIL_CLIENT_RESPONSE_BUFFER_SIZE;

        char *buf = allocMem<char *>(chunkBufSize + 1);

        int octetCount = 0;

        int readLen = 0;

        MB_String ovfBuf;
        readResponse<IMAPSession *>(imap, buf, chunkBufSize, readLen, false, octetCount, ovfBuf);

        // If buffer overflown, copy from overflow buffer
        if (ovfBuf.length() > 0)
        {
            // release memory
            freeMem(&buf);
            buf = allocMem<char *>(ovfBuf.length() + 1);
            strcpy(buf, ovfBuf.c_str());
            ovfBuf.clear();
        }

        if (readLen > 0)
        {

            if (imap->_debug && imap->_debugLevel > esp_mail_debug_level_basic)
                esp_mail_debug_print((const char *)buf, true);

            char *tmp = nullptr;
            int p1 = -1;
            bool exists = false;

            tmp = subStr(buf, NULL, imap_responses[esp_mail_imap_response_exists].text, 0);
            if (tmp)
            {
                int numMsg = imap->_mbif._msgCount;
                imap->_mbif._msgCount = atoi(tmp);
                // release memory
                freeMem(&tmp);
                exists = true;
                imap->_mbif._folderChanged |= (int)imap->_mbif._msgCount != numMsg;
                if ((int)imap->_mbif._msgCount > numMsg)
                {
                    imap->_mbif._polling_status.type = imap_polling_status_type_new_message;
                    imap->_mbif._polling_status.messageNum = imap->_mbif._msgCount;
                }
                goto ex;
            }

            tmp = subStr(buf, NULL, imap_responses[esp_mail_imap_response_expunge].text, 0);
            if (tmp)
            {
                imap->_mbif._polling_status.type = imap_polling_status_type_remove_message;
                imap->_mbif._polling_status.messageNum = atoi(tmp);

                if (imap->_mbif._polling_status.messageNum == imap->_mbif._msgCount && imap->_mbif._nextUID > 0)
                    imap->_mbif._nextUID--;

                // release memory
                freeMem(&tmp);
                imap->_mbif._folderChanged = true;
                goto ex;
            }

            tmp = subStr(buf, NULL, imap_responses[esp_mail_imap_response_recent].text, 0);
            if (tmp)
            {
                imap->_mbif._recentCount = atoi(tmp);
                // release memory
                freeMem(&tmp);
                goto ex;
            }

            tmp = subStr(buf, NULL, imap_responses[esp_mail_imap_response_fetch].text, 0);
            if (tmp)
            {
                imap->_mbif._polling_status.messageNum = atoi(tmp);
                // release memory
                freeMem(&tmp);

                imap->_mbif._polling_status.argument = buf;
                imap->_mbif._polling_status.argument.erase(0, p1 + 8);
                imap->_mbif._polling_status.argument.pop_back();
                imap->_mbif._folderChanged = true;
                imap->_mbif._polling_status.type = imap_polling_status_type_fetch_message;
                goto ex;
            }

        ex:

            imap->_mbif._floderChangedState = (imap->_mbif._folderChanged && exists) || imap->_mbif._polling_status.type == imap_polling_status_type_fetch_message;
        }

        // release memory
        freeMem(&buf);
    }

    size_t imap_idle_tmo = imap->_imap_data->limit.imap_idle_timeout;

    if (imap_idle_tmo < 60 * 1000 || imap_idle_tmo > 29 * 60 * 1000)
        imap_idle_tmo = 10 * 60 * 1000;

    if (millis() - imap->_mbif._idleTimeMs > imap_idle_tmo)
    {
        if (imap->mStopListen(true))
            return imap->mListen(true);
        return false;
    }

    return true;
}

void ESP_Mail_Client::appendFetchString(MB_String &buf, bool uid)
{
    if (uid)
        buf += imap_cmd_post_tokens[esp_mail_imap_command_uid];
    else
        joinStringSpace(buf, false, 2, imap_commands[esp_mail_imap_command_flags].text, esp_mail_str_38 /* "(" */);
}

void ESP_Mail_Client::parseCmdResponse(IMAPSession *imap, char *buf, PGM_P find)
{
    if (imap->_imap_cmd == esp_mail_imap_cmd_get_uid)
        imap->_uid_tmp = 0;

    char *tmp = nullptr;
    int p1 = strposP(buf, find, 0);
    if (p1 != -1)
    {
        if (imap->_imap_cmd == esp_mail_imap_cmd_get_quota_root ||
            imap->_imap_cmd == esp_mail_imap_cmd_get_acl ||
            imap->_imap_cmd == esp_mail_imap_cmd_my_rights)
        {
            int ofs = imap->_imap_cmd == esp_mail_imap_cmd_get_quota_root ? 0 : 1;
            int p2 = strposP(buf, esp_mail_str_2 /* " " */, p1 + strlen_P(find) + ofs);
            if (p2 != -1)
            {
                int len = strlen(buf) - p2 - 1;
                tmp = allocMem<char *>(len);
                strncpy(tmp, buf + p2 + 1, strlen(buf) - p2 - 1);
                if (imap->_imap_cmd == esp_mail_imap_cmd_get_quota_root)
                {
                    if (imap->_quota_root_tmp.length() > 0)
                        imap->_quota_root_tmp += esp_mail_str_8; /* ";" */
                    imap->_quota_root_tmp += tmp;
                }
                else
                {
                    imap->_acl_tmp = tmp;
                }

                // release memory
                freeMem(&tmp);
            }
        }
        else
        {
            int len = imap->_imap_cmd == esp_mail_imap_cmd_get_uid ? 20 : strlen(buf) - p1 - strlen_P(find);
            int ofs = imap->_imap_cmd == esp_mail_imap_cmd_get_uid ||
                              imap->_imap_cmd == esp_mail_imap_cmd_fetch_sequence_set
                          ? 1
                      : imap->_imap_cmd == esp_mail_imap_cmd_namespace || imap->_imap_cmd == esp_mail_imap_cmd_id ? 0
                                                                                                                  : 2;
            tmp = allocMem<char *>(len);

            strncpy(tmp, buf + p1 + strlen_P(find), strlen(buf) - p1 - strlen_P(find) - ofs);

            esp_mail_imap_msg_num_t msg_num;

            switch ((int)imap->_imap_cmd)
            {
            case esp_mail_imap_cmd_get_uid:
                imap->_uid_tmp = atoi(tmp);
                break;
            case esp_mail_imap_cmd_get_flags:
                imap->_flags_tmp = tmp;
                break;
            case esp_mail_imap_cmd_get_quota:
                imap->_quota_tmp = tmp;
                break;
            case esp_mail_imap_cmd_id:
                imap->_server_id_tmp = tmp;
                break;
            case esp_mail_imap_cmd_namespace:
                imap->_ns_tmp += tmp;
                break;
            case esp_mail_imap_cmd_fetch_sequence_set:

                msg_num.type = esp_mail_imap_msg_num_type_uid;
                msg_num.value = (uint32_t)atoi(tmp);
                imap->_imap_msg_num.push_back(msg_num);

                if (imap->_imap_msg_num.size() > imap->_imap_data->limit.fetch)
                    imap->_imap_msg_num.erase(imap->_imap_msg_num.begin());

                break;
            default:
                break;
            }
        }

        // release memory
        freeMem(&tmp);
    }
}

bool ESP_Mail_Client::getFlags(IMAPSession *imap, char *buf, esp_mail_imap_response_types type)
{
    if (strposP(buf, imap_responses[type].text, 0) != -1)
    {
        char *p = allocMem<char *>(strlen(buf));
        strcpy(p, buf);
        char *pp = p;
        char *end = p;
        int count = 0;
        bool isList = false;
        int tkPos = (type == esp_mail_imap_response_permanent_flags) ? 4 : 3;

        while (pp != NULL)
        {
            // See RFC2047.h
            ESP_MAIL_STRSEP(&end, " ");
            count++;
            if (count >= tkPos && strlen(pp) > 0)
            {
                if (type == esp_mail_imap_response_permanent_flags && pp[strlen(pp) - 1] == ']')
                    pp[strlen(pp) - 1] = 0;

                if (count == tkPos && pp[0] == '(' && pp[1] != ')')
                    imap->_mbif.addFlag(getList(pp, isList), type == esp_mail_imap_response_permanent_flags);
                else if (isList)
                    imap->_mbif.addFlag(getList(pp, isList), type == esp_mail_imap_response_permanent_flags);
            }
            pp = end;
        }

        // release memory
        freeMem(&p);

        return true;
    }

    return false;
}

void ESP_Mail_Client::parseExamineResponse(IMAPSession *imap, char *buf)
{
    char *tmp = NULL;

    tmp = subStr(buf, NULL, imap_responses[esp_mail_imap_response_exists].text, 0);
    if (tmp)
    {
        imap->_mbif._msgCount = atoi(tmp);
        // release memory
        freeMem(&tmp);
        return;
    }

    tmp = subStr(buf, NULL, imap_responses[esp_mail_imap_response_recent].text, 0);
    if (tmp)
    {
        imap->_mbif._recentCount = atoi(tmp);
        // release memory
        freeMem(&tmp);
        return;
    }

    if (imap->_mbif._flags.size() == 0 && getFlags(imap, buf, esp_mail_imap_response_flags))
        return;

    if (imap->_mbif._permanent_flags.size() == 0 && getFlags(imap, buf, esp_mail_imap_response_permanent_flags))
        return;

    if (imap->_mbif._uidValidity == 0)
    {
        tmp = subStr(buf, imap_responses[esp_mail_imap_response_uidvalidity].text, esp_mail_str_41 /* "]" */, 0, 0);
        if (tmp)
        {
            imap->_mbif._uidValidity = atoi(tmp);
            // release memory
            freeMem(&tmp);
            return;
        }
    }

    if (imap->_nextUID.length() == 0)
    {
        tmp = subStr(buf, imap_responses[esp_mail_imap_response_uidnext].text, esp_mail_str_41 /* "]" */, 0, 0);
        if (tmp)
        {
            imap->_nextUID = tmp;
            imap->_mbif._nextUID = atoi(tmp);
            // release memory
            freeMem(&tmp);
            return;
        }
    }

    if (imap->_unseenMsgIndex.length() == 0)
    {
        tmp = subStr(buf, imap_responses[esp_mail_imap_response_unseen].text, esp_mail_str_41 /* "]" */, 0, 0);
        if (tmp)
        {
            imap->_unseenMsgIndex = tmp;
            imap->_mbif._unseenMsgIndex = atoi(tmp);
            // release memory
            freeMem(&tmp);
            return;
        }
    }

    if (imap->isCondStoreSupported())
    {
        tmp = subStr(buf, imap_responses[esp_mail_imap_response_highest_modsec].text, esp_mail_str_41 /* "]" */, 0, 0);
        if (tmp)
        {
            imap->_mbif._highestModSeq = tmp;
            // release memory
            freeMem(&tmp);
            return;
        }

        tmp = subStr(buf, imap_responses[esp_mail_imap_response_nomodsec].text, nullptr, 0, 0);
        if (tmp)
        {
            imap->_mbif._nomodsec = true;
            // release memory
            freeMem(&tmp);
            return;
        }
    }
}

bool ESP_Mail_Client::handleIMAPError(IMAPSession *imap, int err, bool ret)
{
    if (err < 0)
    {
        errorStatusCB<IMAPSession *, IMAPSession *>(imap, nullptr, err, true);

        if (imap->_headers.size() > 0 && cHeader(imap))
        {
            if ((imap->_imap_cmd == esp_mail_imap_cmd_fetch_body_attachment || imap->_imap_cmd == esp_mail_imap_cmd_fetch_body_inline) && (imap->_imap_data->download.attachment || imap->_imap_data->download.inlineImg))
            {
                if (cPart(imap) && cHeader(imap)->part_headers.size() > 0)
                    cPart(imap)->download_error = imap->errorReason().c_str();
            }
            else if (cHeader(imap))
                cHeader(imap)->error_msg = imap->errorReason().c_str();

            cHeader(imap)->error = true;
        }
    }

    closeTCPSession<IMAPSession *>(imap);

    imap->_cbData.empty();

    return ret;
}

void ESP_Mail_Client::prepareFileList(IMAPSession *imap, MB_String &filePath)
{
#if defined(MBFS_SD_FS)
    if (!mbfs->longNameSupported())
    {
        cHeader(imap)->sd_alias_file_count++;
        MB_String alias = cHeader(imap)->message_uid;
        alias += esp_mail_str_83; /* "_" */
        alias += cHeader(imap)->sd_alias_file_count;

        if (imap->_sdFileList.length() > 0)
        {
            if (imap->_sdFileList[imap->_sdFileList.length() - 1] == ']')
            {
                imap->_sdFileList[imap->_sdFileList.length() - 1] = 0;
                imap->_sdFileList += esp_mail_str_8; /* "," */
            }
        }
        imap->_sdFileList += esp_mail_str_80; /* "{\"Renamed\":\"" */
        imap->_sdFileList += alias;
        imap->_sdFileList += esp_mail_str_81; /* "\",\"Original\":\"" */
        imap->_sdFileList += filePath;
        imap->_sdFileList += esp_mail_str_82; /* "\"}]" */
        // rename the original file
        filePath = alias;
    }
#endif
}

bool ESP_Mail_Client::parseAttachmentResponse(IMAPSession *imap, char *buf, esp_mail_imap_response_data &res)
{
    int bufLen = res.readLen;

    if (res.chunkIdx == 0)
    {
        char *tmp = subStr(buf, esp_mail_str_36 /* "{" */, esp_mail_str_37 /* "}" */, 0);
        if (tmp)
        {
            res.chunkIdx++;
            res.octetCount = 0; // CRLF counted from first line
            res.octetLength = atoi(tmp);
            // release memory
            freeMem(&tmp);
            cPart(imap)->octetLen = res.octetLength;
            cPart(imap)->octetCount = 0;
            cHeader(imap)->total_download_size += res.octetLength;
            imap->_lastProgress = -1;

#if defined(ESP_MAIL_OTA_UPDATE_ENABLED)
            if (cPart(imap)->is_firmware_file)
            {
                cPart(imap)->is_firmware_file = Update.begin(cPart(imap)->attach_data_size);

                if (!cPart(imap)->is_firmware_file)
                {
                    imap->_responseStatus.errorCode = IMAP_STATUS_FIRMWARE_UPDATE_INIT_FAILED;
                    imap->_responseStatus.text.clear();
                }

#if !defined(SILENT_MODE)

                sendCallback<IMAPSession *>(imap, esp_mail_cb_str_42 /* "Updating firmware..." */, true, false);

                if (!cPart(imap)->is_firmware_file)
                {
                    printDebug<IMAPSession *>(imap,
                                              imap->errorReason().c_str(),
                                              imap->errorReason().c_str(),
                                              esp_mail_debug_tag_type_error,
                                              true,
                                              false);
                }

#endif
            }
#endif

            if (!cPart(imap)->file_open_write)
            {

                if (imap->_storageReady && cPart(imap)->save_to_file)
                {
                    res.downloadRequest = true;

                    res.filePath.clear();

                    res.filePath += imap->_imap_data->storage.saved_path;
                    res.filePath += esp_mail_str_10; /* "/" */
                    res.filePath += cHeader(imap)->message_uid;
                    res.filePath += esp_mail_str_10; /* "/" */
                    res.filePath += cPart(imap)->filename;

                    prepareFileList(imap, res.filePath);

                    int sz = mbfs->open(res.filePath, mbfs_type imap->_imap_data->storage.type, mb_fs_open_mode_write);

                    if (sz < 0)
                    {
                        imap->_responseStatus.errorCode = sz;
                        imap->_responseStatus.text.clear();
#if !defined(SILENT_MODE)
                        printDebug<IMAPSession *>(imap,
                                                  imap->errorReason().c_str(),
                                                  imap->errorReason().c_str(),
                                                  esp_mail_debug_tag_type_error,
                                                  true,
                                                  false);
#endif
                    }

                    cPart(imap)->file_open_write = true;
                }
            }
        }
        return true;
    }

    yield_impl();

    if (res.octetLength == 0)
        return true;

    if (cPart(imap)->octetCount <= res.octetLength)
    {
        if (cPart(imap)->octetCount + bufLen > res.octetLength)
        {
            bufLen = res.octetLength - cPart(imap)->octetCount;
            buf[bufLen] = 0;
            cPart(imap)->octetCount += bufLen;
        }
        else
            cPart(imap)->octetCount = res.octetCount;

        if (imap->_imap_data->enable.download_status)
        {
            if (imap->_debug)
                downloadReport(imap, 100 * cPart(imap)->octetCount / res.octetLength);
        }

        if (cPart(imap)->octetCount > res.octetLength)
            return true;

        bool write_error = false, fw_write_error = false;

        if (cPart(imap)->xencoding == esp_mail_msg_xencoding_base64)
        {

            size_t olen = 0;
            unsigned char *decoded = decodeBase64((const unsigned char *)buf, bufLen, &olen);

            if (decoded)
            {

                if (!cPart(imap)->sizeProp)
                {
                    cPart(imap)->attach_data_size += olen;
                    cHeader(imap)->total_attach_data_size += cPart(imap)->attach_data_size;
                }

                sendStreamCB(imap, (void *)decoded, olen, res.chunkIdx, false);

                size_t write = olen;

                if (cPart(imap)->is_firmware_file)
                {
#if defined(ESP_MAIL_OTA_UPDATE_ENABLED)
                    size_t fw_write = Update.write((uint8_t *)decoded, olen);
                    cPart(imap)->firmware_downloaded_byte += fw_write == olen ? olen : 0;
                    fw_write_error = fw_write != olen;
#endif
                }

                if (cPart(imap)->save_to_file)
                {
                    if (mbfs->ready(mbfs_type imap->_imap_data->storage.type))
                        write = mbfs->write(mbfs_type imap->_imap_data->storage.type, (uint8_t *)decoded, olen);
                }

                yield_impl();
                // release memory
                freeMem(&decoded);

                write_error = write != olen;
            }

            if (!reconnect(imap))
                return false;
        }
        else
        {
            // binary content
            if (!cPart(imap)->sizeProp)
            {
                cPart(imap)->attach_data_size += bufLen;
                cHeader(imap)->total_attach_data_size += cPart(imap)->attach_data_size;
            }

            sendStreamCB(imap, (void *)buf, bufLen, res.chunkIdx, false);

            int write = bufLen;

            if (cPart(imap)->is_firmware_file)
            {
#if defined(ESP_MAIL_OTA_UPDATE_ENABLED)
                int fw_write = Update.write((uint8_t *)buf, bufLen);
                cPart(imap)->firmware_downloaded_byte += fw_write == bufLen ? (size_t)bufLen : 0;
                fw_write_error = fw_write != bufLen;
#endif
            }

            if (cPart(imap)->save_to_file)
            {
                if (mbfs->ready(mbfs_type imap->_imap_data->storage.type))
                    write = mbfs->write(mbfs_type imap->_imap_data->storage.type, (uint8_t *)buf, bufLen);
            }

            yield_impl();

            write_error = write != bufLen;

            if (!reconnect(imap))
                return false;
        }

#if defined(ESP_MAIL_OTA_UPDATE_ENABLED)
        if (cPart(imap)->is_firmware_file)
        {
            bool update_result_ok = !fw_write_error;

            if (!imap->_isFirmwareUpdated && update_result_ok &&
                (cPart(imap)->firmware_downloaded_byte == (size_t)cPart(imap)->attach_data_size || cPart(imap)->octetCount >= res.octetLength))
            {
                update_result_ok = Update.end(cPart(imap)->octetCount >= res.octetLength);
                if (update_result_ok)
                    imap->_isFirmwareUpdated = true;
            }

            if (!update_result_ok)
            {
                cPart(imap)->is_firmware_file = false;

                imap->_responseStatus.errorCode = fw_write_error ? IMAP_STATUS_FIRMWARE_UPDATE_WRITE_FAILED : IMAP_STATUS_FIRMWARE_UPDATE_END_FAILED;
                imap->_responseStatus.text.clear();

#if !defined(SILENT_MODE)
                printDebug<IMAPSession *>(imap,
                                          imap->_responseStatus.text.c_str(),
                                          imap->errorReason().c_str(),
                                          esp_mail_debug_tag_type_error,
                                          !imap->_debug,
                                          false);
#endif
            }
        }
#endif

        if (write_error || fw_write_error)
            return false;
    }

    res.chunkIdx++;
    return true;
}

void ESP_Mail_Client::downloadReport(IMAPSession *imap, int progress)
{
    printProgress(progress, imap->_lastProgress);
}

void ESP_Mail_Client::fetchReport(IMAPSession *imap, int progress, bool download)
{
#if !defined(SILENT_MODE)
    if (imap->_debug && imap->_lastProgress == -1 && strcmp_P(cPart(imap)->filename.c_str(), esp_mail_str_84 /* "message" */) != 0)
        esp_mail_debug_print_tag(cPart(imap)->filename.c_str(), esp_mail_debug_tag_type_client, true);
    printProgress(progress, imap->_lastProgress);
#endif
}

void ESP_Mail_Client::searchReport(IMAPSession *imap, int progress)
{
    printProgress(progress, imap->_lastProgress);
}

struct esp_mail_imap_msg_num_t ESP_Mail_Client::cMSG(IMAPSession *imap)
{
    return imap->_imap_msg_num[cIdx(imap)];
}

int ESP_Mail_Client::cIdx(IMAPSession *imap)
{
    return imap->_cMsgIdx;
}

void ESP_Mail_Client::decodeText(IMAPSession *imap, esp_mail_imap_response_data &res)
{
    int bufLen = res.readLen;
    bool rfc822_body_subtype = cPart(imap)->message_sub_type == esp_mail_imap_message_sub_type_rfc822;
    if (res.chunkIdx == 0)
    {

        imap->_lastProgress = -1;
        char *tmp = subStr(res.response, esp_mail_str_36 /* "{" */, esp_mail_str_37 /* "}" */, 0);
        if (tmp)
        {
            res.chunkIdx++;
            res.octetCount = 0;
            res.octetLength = atoi(tmp);
            // release memory
            freeMem(&tmp);
            cPart(imap)->octetLen = res.octetLength;
            cPart(imap)->octetCount = 0;

            bool dlMsg = (rfc822_body_subtype && imap->_imap_data->download.rfc822) || (!rfc822_body_subtype && ((cPart(imap)->msg_type == esp_mail_msg_type_html && imap->_imap_data->download.html) || ((cPart(imap)->msg_type == esp_mail_msg_type_plain || cPart(imap)->msg_type == esp_mail_msg_type_enriched) && imap->_imap_data->download.text)));
            if (dlMsg)
                prepareFilePath(imap, res.filePath, false);

            if (res.filePath.length() == 0)
            {
                if (!rfc822_body_subtype)
                    res.filePath += esp_mail_str_84; /* "message" */
                else
                    joinStringSpace(res.filePath, false, 2, esp_mail_str_85 /* "rfc822" */, esp_mail_str_84 /* "message" */);
            }
            cPart(imap)->filename = res.filePath;

            if (imap->_storageReady && !cPart(imap)->file_open_write && dlMsg)
            {

                prepareFileList(imap, res.filePath);

                int sz = mbfs->open(res.filePath, mbfs_type imap->_imap_data->storage.type, mb_fs_open_mode_write);
                if (sz > -1)
                {
                    res.downloadRequest = true;
                    cPart(imap)->file_open_write = true;
                }
                else
                {
                    imap->_responseStatus.errorCode = sz;
                    imap->_responseStatus.text.clear();
#if !defined(SILENT_MODE)
                    if (imap->_debug)
                        esp_mail_debug_print_tag(imap->errorReason().c_str(), esp_mail_debug_tag_type_error, true);
#endif
                }
            }

            return;
        }
        else
        {
#if !defined(SILENT_MODE)
            if (imap->_debug)
                esp_mail_debug_print_tag(esp_mail_error_imap_str_12 /* "no centent" */, esp_mail_debug_tag_type_client, false);
#endif
        }
    }

    yield_impl();

    if (res.octetLength == 0)
        return;

    bool enableDownloads = (imap->_imap_data->download.rfc822 && rfc822_body_subtype) || (!rfc822_body_subtype && ((cPart(imap)->msg_type == esp_mail_msg_type_html && imap->_imap_data->download.html) || ((cPart(imap)->msg_type == esp_mail_msg_type_plain || cPart(imap)->msg_type == esp_mail_msg_type_enriched) && imap->_imap_data->download.text)));

    if (imap->_imap_data->download.rfc822 || imap->_imap_data->download.html || imap->_imap_data->download.text || (rfc822_body_subtype && imap->_imap_data->enable.rfc822) || (!rfc822_body_subtype && ((cPart(imap)->msg_type == esp_mail_msg_type_html && imap->_imap_data->enable.html) || ((cPart(imap)->msg_type == esp_mail_msg_type_plain || cPart(imap)->msg_type == esp_mail_msg_type_enriched) && imap->_imap_data->enable.text))))
    {

        if (cPart(imap)->octetCount + bufLen >= res.octetLength)
        {
            bufLen = res.octetLength - cPart(imap)->octetCount;
            cPart(imap)->octetCount += bufLen;

            if (res.octetLength <= res.octetCount && cPart(imap)->xencoding != esp_mail_msg_xencoding_base64 && cPart(imap)->xencoding != esp_mail_msg_xencoding_qp)
                bufLen = 0;

            res.response[bufLen] = 0;
        }
        else
            cPart(imap)->octetCount = res.octetCount;

        if (imap->_debug)
            fetchReport(imap, 100 * cPart(imap)->octetCount / res.octetLength, enableDownloads);

        if (cPart(imap)->octetCount <= res.octetLength)
        {
            bool hrdBrk = cPart(imap)->xencoding == esp_mail_msg_xencoding_qp;
            hrdBrk &= cPart(imap)->octetCount < res.octetLength;
            hrdBrk &= bufLen == 2 && res.response[bufLen - 2] != '\r' && res.response[bufLen - 1] != '\n';

            // remove soft break for QP
            if (bufLen <= QP_ENC_MSG_LEN && res.response[bufLen - 1] == '=' && cPart(imap)->xencoding == esp_mail_msg_xencoding_qp)
            {
                hrdBrk = false;
                res.response[bufLen - 1] = 0;
                bufLen--;
            }

            size_t olen = 0;
            char *decoded = nullptr;
            MB_String str;
            bool dontDeleteOrModify = false;

            // decode the content based on the transfer decoding
            if (cPart(imap)->xencoding == esp_mail_msg_xencoding_base64)
            {
                decoded = (char *)decodeBase64((const unsigned char *)res.response, bufLen, &olen);
            }
            else if (cPart(imap)->xencoding == esp_mail_msg_xencoding_qp)
            {
                decoded = allocMem<char *>(bufLen + 10);
                decodeQP_UTF8(res.response, decoded);
                olen = strlen(decoded);
            }
            else if (cPart(imap)->xencoding == esp_mail_msg_xencoding_7bit)
            {
                decoded = decode7Bit_UTF8(res.response);
                olen = strlen(decoded);
            }
            else if (cPart(imap)->xencoding == esp_mail_msg_xencoding_8bit)
            {
                decoded = decode8Bit_UTF8(res.response);
                olen = strlen(decoded);
            }
            else
            {
                // binary
                dontDeleteOrModify = true;
                decoded = res.response;
                olen = bufLen;
            }

            if (decoded)
            {
                // charset? apply character decoding to the decoded or nonencoded content
                if ((rfc822_body_subtype && imap->_imap_data->enable.rfc822) || (!rfc822_body_subtype && ((cPart(imap)->msg_type == esp_mail_msg_type_html && imap->_imap_data->enable.html) || ((cPart(imap)->msg_type == esp_mail_msg_type_plain || cPart(imap)->msg_type == esp_mail_msg_type_enriched) && imap->_imap_data->enable.text))))
                {
                    if (imap->_charDecCallback)
                    {
                        IMAP_Decoding_Info decoding;
                        decoding.charset = cPart(imap)->charset.c_str();
                        decoding.data = decoded;
                        decoding.type = IMAP_Decoding_Info::message_part_type_text;

                        imap->_charDecCallback(&decoding);

                        if (decoding.decodedString.length() > 0)
                        {
                            char *buf2 = allocMem<char *>(decoding.decodedString.length() + 1);
                            strcpy(buf2, decoding.decodedString.c_str());

                            if (decoded && !dontDeleteOrModify)
                                // release memory
                                freeMem(&decoded);

                            decoded = buf2;
                            olen = strlen(buf2);
                        }
                    }
                    else
                    {
                        esp_mail_char_decoding_scheme scheme = getEncodingFromCharset(cPart(imap)->charset.c_str());

                        if (scheme == esp_mail_char_decoding_scheme_iso8859_1)
                        {
                            int ilen = olen;
                            int olen2 = (ilen + 1) * 2;
                            unsigned char *tmp = allocMem<unsigned char *>(olen2);
                            decodeLatin1_UTF8(tmp, &olen2, (unsigned char *)decoded, &ilen);

                            if (decoded && !dontDeleteOrModify)
                                // release memory
                                freeMem(&decoded);

                            olen = olen2;
                            decoded = (char *)tmp;
                        }
                        else if (scheme == esp_mail_char_decoding_scheme_tis_620 || scheme == esp_mail_char_decoding_scheme_iso8859_11 || scheme == esp_mail_char_decoding_scheme_windows_874)
                        {
                            char *out = allocMem<char *>((olen + 1) * 3);
                            decodeTIS620_UTF8(out, decoded, olen);
                            olen = strlen(out);
                            if (decoded && !dontDeleteOrModify)
                                // release memory
                                freeMem(&decoded);
                            decoded = out;
                        }
                    }

                    if (cPart(imap)->text.length() < imap->_imap_data->limit.msg_size)
                    {

                        if (cPart(imap)->text.length() + olen < imap->_imap_data->limit.msg_size)
                        {
                            cPart(imap)->textLen += olen;
                            cPart(imap)->text.append(decoded, olen);
                            if (hrdBrk)
                            {
                                appendNewline(cPart(imap)->text);
                                cPart(imap)->textLen += 2;
                            }
                        }
                        else
                        {
                            int d = imap->_imap_data->limit.msg_size - cPart(imap)->text.length();
                            cPart(imap)->textLen += d;
                            if (d > 0)
                                cPart(imap)->text.append(decoded, d);

                            if (hrdBrk)
                            {
                                appendNewline(cPart(imap)->text);
                                cPart(imap)->textLen += 2;
                            }
                        }
                    }
                }

                if (res.filePath.length() > 0 && res.downloadRequest)
                {
                    if (mbfs->ready(mbfs_type imap->_imap_data->storage.type))
                    {
                        if (olen > 0)
                            mbfs->write(mbfs_type imap->_imap_data->storage.type, (uint8_t *)decoded, olen);
                        if (hrdBrk)
                            mbfs->write(mbfs_type imap->_imap_data->storage.type, (uint8_t *)"\r\n", 2);
                    }
                }

                sendStreamCB(imap, (void *)decoded, olen, res.chunkIdx, hrdBrk);

                if (decoded && !dontDeleteOrModify)
                    // release memory
                    freeMem(&decoded);
            }
        }
    }

    res.chunkIdx++;
}

void ESP_Mail_Client::sendStreamCB(IMAPSession *imap, void *buf, size_t len, int chunkIndex, bool hrdBrk)
{
    if (imap->_mimeDataStreamCallback && len > 0)
    {
        MIME_Data_Stream_Info streaminfo;
        streaminfo.uid = cHeader(imap)->message_uid;
        streaminfo.disposition = cPart(imap)->content_disposition.c_str();
        streaminfo.type = cPart(imap)->content_type.c_str();
        streaminfo.charSet = cPart(imap)->charset.c_str();
        streaminfo.transfer_encoding = cPart(imap)->content_transfer_encoding.c_str();
        streaminfo.cid = cPart(imap)->CID.c_str();
        streaminfo.description = cPart(imap)->content_description.c_str();
        streaminfo.date = cPart(imap)->creation_date.c_str();
        streaminfo.filename = cPart(imap)->filename.c_str();
        streaminfo.size = (cPart(imap)->sizeProp) ? cPart(imap)->attach_data_size : cPart(imap)->octetLen;
        streaminfo.name = cPart(imap)->name.c_str();
        streaminfo.octet_size = cPart(imap)->octetLen;
        streaminfo.octet_count = cPart(imap)->octetCount;
        streaminfo.isFirstData = chunkIndex == 1;
        streaminfo.isLastData = !hrdBrk ? cPart(imap)->octetLen == cPart(imap)->octetCount : false;
        streaminfo.data_size = len;
        streaminfo.data = buf;
        streaminfo.flowed = cPart(imap)->plain_flowed;
        streaminfo.delsp = cPart(imap)->plain_delsp;

        imap->_mimeDataStreamCallback(streaminfo);

        if (hrdBrk)
        {
            streaminfo.isFirstData = false;
            streaminfo.isLastData = cPart(imap)->octetLen == cPart(imap)->octetCount;
            streaminfo.data_size = 2;
            streaminfo.data = (void *)"\r\n";
            imap->_mimeDataStreamCallback(streaminfo);
        }
    }
}

void ESP_Mail_Client::prepareFilePath(IMAPSession *imap, MB_String &filePath, bool header)
{
    bool rfc822_body_subtype = cPart(imap)->message_sub_type == esp_mail_imap_message_sub_type_rfc822;
    MB_String fpath = imap->_imap_data->storage.saved_path;
    fpath += esp_mail_str_10; /* "/" */
    fpath += cHeader(imap)->message_uid;

    if (!header)
    {
        if (!rfc822_body_subtype)
        {
            fpath += esp_mail_str_86; /* "/msg" */
            if (cPart(imap)->msg_type == esp_mail_msg_type_plain || cPart(imap)->msg_type == esp_mail_msg_type_enriched)
                fpath += mimeinfo[esp_mail_file_extension_txt].endsWith;
            else if (cPart(imap)->msg_type == esp_mail_msg_type_html)
                fpath += mimeinfo[esp_mail_file_extension_htm].endsWith;
        }
        else
        {
            fpath += esp_mail_str_87; /* "/rfc822_msg" */

            if (cPart(imap)->rfc822_msg_Idx > 0)
                fpath += cPart(imap)->rfc822_msg_Idx;

            if (cPart(imap)->msg_type == esp_mail_msg_type_plain || cPart(imap)->msg_type == esp_mail_msg_type_enriched)
                fpath += mimeinfo[esp_mail_file_extension_txt].endsWith;
            else if (cPart(imap)->msg_type == esp_mail_msg_type_html)
                fpath += mimeinfo[esp_mail_file_extension_htm].endsWith;
            else
                // possible rfc822 encapsulated message which cannot fetch its header
                fpath += mimeinfo[esp_mail_file_extension_dat].endsWith;
        }
    }

    filePath = fpath;
}

void ESP_Mail_Client::sendStorageNotReadyError(IMAPSession *imap, esp_mail_file_storage_type storageType)
{

#if defined(MBFS_FLASH_FS) || defined(MBFS_SD_FS)
#if !defined(SILENT_MODE)
    if (imap->_debug && (storageType == esp_mail_file_storage_type_flash || storageType == esp_mail_file_storage_type_sd))
    {
        if (storageType == esp_mail_file_storage_type_flash)
        {
            esp_mail_debug_print_tag(esp_mail_error_mem_str_1 /* "flash Storage is not ready." */, esp_mail_debug_tag_type_error, true);
#if defined(MB_ARDUINO_PICO)
            esp_mail_debug_print_tag(esp_mail_error_mem_str_10 /* "please make sure that the size of flash filesystem is not 0 in Pico." */, esp_mail_debug_tag_type_error, true);
#endif
        }
        else if (storageType == esp_mail_file_storage_type_sd)
            esp_mail_debug_print_tag(esp_mail_error_mem_str_2 /* "SD Storage is not ready." */, esp_mail_debug_tag_type_error, true);
    }
#endif
#endif
}

IMAPSession::IMAPSession(Client *client)
{
    setClient(client);
}

IMAPSession::IMAPSession()
{
}

IMAPSession::~IMAPSession()
{
    empty();
}

bool IMAPSession::closeSession()
{
    _prev_imap_cmd = esp_mail_imap_cmd_sasl_login;
    _prev_imap_custom_cmd = esp_mail_imap_cmd_custom;

    if (!connected())
        return false;

    if (_mbif._idleTimeMs > 0)
        mStopListen(false);

    if (_loginStatus)
    {
#if !defined(ESP8266)
        /**
         * The strange behavior in ESP8266 SSL client, BearSSLWiFiClientSecure
         * The client disposed without memory released after the server close
         * the connection due to LOGOUT command, which caused the memory leaks.
         */
        if (!MailClient.imapLogout(this))
            return false;
#endif
    }

    return MailClient.handleIMAPError(this, 0, true);
}

bool IMAPSession::connected()
{
    return client.connected();
}

bool IMAPSession::connect(Session_Config *session_config, IMAP_Data *imap_data, bool login)
{
    _sessionSSL = false;
    _sessionLogin = login;

    if (session_config)
        session_config->clearPorts();

    this->_customCmdResCallback = NULL;

    int ptr = toAddr(*session_config);
    session_config->addPtr(&_configPtrList, ptr);

    if (!handleConnection(session_config, imap_data, _sessionSSL))
        return false;

    if (!_sessionLogin)
        return true;

    _loginStatus = MailClient.imapAuth(this, _sessionSSL);

    return _loginStatus;
}

bool IMAPSession::mLogin(MB_StringPtr email, MB_StringPtr password, bool isToken)
{
    if (_loginStatus)
        return true;

    if (!MailClient.sessionExisted<IMAPSession *>(this))
        return false;

    _session_cfg->login.email = email;

    _session_cfg->login.accessToken.clear();
    _session_cfg->login.password.clear();

    if (isToken)
        _session_cfg->login.accessToken = password;
    else
        _session_cfg->login.password = password;

    _loginStatus = MailClient.imapAuth(this, _sessionSSL);

    return _loginStatus;
}

void IMAPSession::appendIdList(MB_String &list, IMAP_Identification *identification)
{

    if (identification->name.length() > 0 && identification->name.length() <= 1024)
        MailClient.appendImap4KeyValue(list, imap_identification_keys[esp_mail_imap_identification_key_name].text, identification->name.c_str());

    if (identification->version.length() > 0 && identification->version.length() <= 1024)
    {
        if (list.length() > 0)
            MailClient.appendSpace(list);
        MailClient.appendImap4KeyValue(list, imap_identification_keys[esp_mail_imap_identification_key_version].text, identification->version.c_str());
    }

    if (identification->os.length() > 0 && identification->os.length() <= 1024)
    {
        if (list.length() > 0)
            MailClient.appendSpace(list);
        MailClient.appendImap4KeyValue(list, imap_identification_keys[esp_mail_imap_identification_key_os].text, identification->os.c_str());
    }

    if (identification->vendor.length() > 0 && identification->vendor.length() <= 1024)
    {
        if (list.length() > 0)
            MailClient.appendSpace(list);
        MailClient.appendImap4KeyValue(list, imap_identification_keys[esp_mail_imap_identification_key_vendor].text, identification->vendor.c_str());
    }

    if (identification->support_url.length() > 0 && identification->support_url.length() <= 1024)
    {
        if (list.length() > 0)
            MailClient.appendSpace(list);
        MailClient.appendImap4KeyValue(list, imap_identification_keys[esp_mail_imap_identification_key_support_url].text, identification->support_url.c_str());
    }

    if (identification->address.length() > 0 && identification->address.length() <= 1024)
    {
        if (list.length() > 0)
            MailClient.appendSpace(list);
        MailClient.appendImap4KeyValue(list, imap_identification_keys[esp_mail_imap_identification_key_address].text, identification->address.c_str());
    }

    if (identification->date.length() > 0 && identification->date.length() <= 1024)
    {
        if (list.length() > 0)
            MailClient.appendSpace(list);
        MailClient.appendImap4KeyValue(list, imap_identification_keys[esp_mail_imap_identification_key_date].text, identification->date.c_str());
    }

    if (identification->command.length() > 0 && identification->command.length() <= 1024)
    {
        if (list.length() > 0)
            MailClient.appendSpace(list);
        MailClient.appendImap4KeyValue(list, imap_identification_keys[esp_mail_imap_identification_key_command].text, identification->command.c_str());
    }

    if (identification->arguments.length() > 0 && identification->arguments.length() <= 1024)
    {
        if (list.length() > 0)
            MailClient.appendSpace(list);
        MailClient.appendImap4KeyValue(list, imap_identification_keys[esp_mail_imap_identification_key_arguments].text, identification->arguments.c_str());
    }

    if (identification->environment.length() > 0 && identification->environment.length() <= 1024)
    {
        if (list.length() > 0)
            MailClient.appendSpace(list);
        MailClient.appendImap4KeyValue(list, imap_identification_keys[esp_mail_imap_identification_key_environment].text, identification->environment.c_str());
    }
}

bool IMAPSession::id(IMAP_Identification *identification)
{
#if !defined(SILENT_MODE)
    MailClient.printDebug<IMAPSession *>(this,
                                         esp_mail_cb_str_61 /* "Send client identification..." */,
                                         esp_mail_dbg_str_82 /* "send IMAP command, ID" */,
                                         esp_mail_debug_tag_type_client,
                                         true,
                                         false);
#endif
    if (!_feature_capability[esp_mail_imap_read_capability_id])
    {
#if !defined(SILENT_MODE)
        printDebugNotSupported();
#endif
        return false;
    }

    if (!MailClient.reconnect(this))
        return false;

    MB_String cmd, idList;

    appendIdList(idList, identification);

    MailClient.appendSpace(cmd, true, imap_commands[esp_mail_imap_command_id].text);

    if (idList.length() > 0)
        MailClient.appendString(cmd, idList.c_str(), false, false, esp_mail_string_mark_type_round_bracket);
    else
    {
        int bufLen = 50;
        char *buf = MailClient.allocMem<char *>(bufLen);
        snprintf(buf, bufLen, pgm2Str(esp_mail_str_21 /* "(\"name\" \"ESP Mail Client\" \"version\" \"%s\")" */), ESP_MAIL_VERSION);
        cmd += buf;
        // release memory
        MailClient.freeMem(&buf);
    }

    if (MailClient.imapSend(this, cmd.c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
        return false;

    _imap_cmd = esp_mail_imap_cmd_id;
    if (!MailClient.handleIMAPResponse(this, IMAP_STATUS_BAD_COMMAND, false))
        return false;

    return true;
}

String IMAPSession::serverID()
{
    return _server_id_tmp.c_str();
}

bool IMAPSession::isAuthenticated()
{
    return _authenticated;
}

bool IMAPSession::isLoggedIn()
{
    return _loginStatus;
}

bool IMAPSession::isFirmwareUpdateSuccess()
{
    return _isFirmwareUpdated;
}

bool IMAPSession::mCustomConnect(Session_Config *session_config, imapResponseCallback callback, MB_StringPtr tag)
{
    this->_customCmdResCallback = callback;
    this->_responseStatus.tag = tag;

    bool ssl = false;
    if (!handleConnection(session_config, NULL, ssl))
        return false;

    return true;
}

bool IMAPSession::handleConnection(Session_Config *session_config, IMAP_Data *imap_data, bool &ssl)
{

    _session_cfg = session_config;

    if (!client.isInitialized())
        return MailClient.handleIMAPError(this, TCP_CLIENT_ERROR_NOT_INITIALIZED, false);

    // Resources are also released if network disconnected.
    if (!MailClient.reconnect(this))
        return false;

    // Need to close previous connection first to free resources.
    MailClient.closeTCPSession<IMAPSession *>(this);

    _session_cfg = session_config;
    _imap_data = imap_data;

    MailClient.setCert(_session_cfg, _session_cfg->certificate.cert_data);

    ssl = false;
    if (!connect(ssl))
    {
        MailClient.closeTCPSession<IMAPSession *>(this);
        return false;
    }

    return true;
}

bool IMAPSession::connect(bool &ssl)
{
    ssl = false;
    MB_String buf;

    if (_imap_data)
    {
        if (_imap_data->fetch.sequence_set.string.length() > 0 || _imap_data->fetch.uid.length() > 0 || _imap_data->fetch.number.length() > 0)
            _headerOnly = false;
        else
            _headerOnly = true;
    }

    _totalRead = 0;
    _secure = true;
    bool secureMode = true;

    client.txBufDivider = 16; // minimum, tx buffer size for ssl data and request command data
    client.rxBufDivider = 1;
    if (_imap_data)
    {
        if (!_headerOnly && !_imap_data->firmware_update.attach_filename.length() == 0 && !_imap_data->enable.html && !_imap_data->enable.text && !_imap_data->download.attachment && !_imap_data->download.inlineImg && !_imap_data->download.html && !_imap_data->download.text)
            client.rxBufDivider = 16; // minimum rx buffer size for only message header
    }

    MailClient.preparePortFunction(_session_cfg, false, _secure, secureMode, ssl);

#if !defined(SILENT_MODE)
    MailClient.printLibInfo<IMAPSession *>(this);
#endif

    MailClient.prepareTime<IMAPSession *>(_session_cfg, this);

    MailClient.setSecure(client, _session_cfg);

    if (!MailClient.beginConnection<IMAPSession *>(_session_cfg, this, secureMode))
        return false;

    // server connected

    client.setTimeout(tcpTimeout);

    // wait for greeting
    unsigned long dataMs = millis();
    while (client.connected() && client.available() == 0 && millis() - dataMs < 2000)
    {
       yield_impl();
    }

    int chunkBufSize = client.available();

    if (chunkBufSize > 0)
    {
        char *buf = MailClient.allocMem<char *>(chunkBufSize + 1);
        client.readBytes(buf, chunkBufSize);
        if (_debug && _debugLevel > esp_mail_debug_level_basic && !_customCmdResCallback)
            esp_mail_debug_print((const char *)buf, true);

        if (_customCmdResCallback)
        {
            MailClient.imapResponseStatus(this, buf, esp_mail_str_3 /* "*" */);
            _responseStatus.text = buf;

            if (_responseStatus.text[_responseStatus.text.length() - 2] == '\r' && _responseStatus.text[_responseStatus.text.length() - 1] == '\n')
                _responseStatus.text[_responseStatus.text.length() - 2] = 0;

            if (_responseStatus.tag.length() == 0)
                this->_responseStatus.tag = esp_mail_imap_tag_str;

            _customCmdResCallback(_responseStatus);
        }

        // release memory
        MailClient.freeMem(&buf);
    }

    if (!_customCmdResCallback)
    {
#if !defined(SILENT_MODE)
        MailClient.printDebug<IMAPSession *>(this,
                                             esp_mail_cb_str_48 /* "IMAP server connected" */,
                                             esp_mail_dbg_str_33 /* "IMAP server connected" */,
                                             esp_mail_debug_tag_type_client,
                                             true,
                                             false);
#endif
    }

    return true;
}

void IMAPSession::debug(int level)
{
    if (level > esp_mail_debug_level_none)
    {
        if (level > esp_mail_debug_level_basic && level < esp_mail_debug_level_maintainer)
            level = esp_mail_debug_level_basic;
        _debugLevel = level;
        _debug = true;
        client.setDebugLevel(level);
    }
    else
    {
        _debugLevel = esp_mail_debug_level_none;
        _debug = false;
        client.setDebugLevel(0);
    }
}

String IMAPSession::errorReason()
{
    return MailClient.errorReason(false, _responseStatus.errorCode, _responseStatus.text.c_str());
}

int IMAPSession::errorCode()
{
    return _responseStatus.errorCode;
}

bool IMAPSession::mSelectFolder(MB_StringPtr folderName, bool readOnly)
{
    if (connected())
    {
        if (!openFolder(folderName, readOnly))
            return false;
    }
    else
    {
        _currentFolder = folderName;
    }

    if (!connected())
    {
        _responseStatus.errorCode = IMAP_STATUS_OPEN_MAILBOX_FAILED;
        _responseStatus.clear();
    }

    return connected();
}

void IMAPSession::setTCPTimeout(unsigned long timeoutSec)
{
    tcpTimeout = timeoutSec;
}

void IMAPSession::setClient(Client *client)
{
    this->client.setClient(client);
}

void IMAPSession::setGSMClient(Client *client, void *modem, const char *pin, const char *apn, const char *user, const char *password)
{
    this->client.setGSMClient(client, modem, pin, apn, user, password);
}

void IMAPSession::networkConnectionRequestCallback(NetworkConnectionRequestCallback networkConnectionCB)
{
    this->client.networkConnectionRequestCallback(networkConnectionCB);
}

void IMAPSession::networkStatusRequestCallback(NetworkStatusRequestCallback networkStatusCB)
{
    this->client.networkStatusRequestCallback(networkStatusCB);
}

void IMAPSession::setNetworkStatus(bool status)
{
    this->client.setNetworkStatus(status);
    MailClient.networkStatus = status;
}

void IMAPSession::setSSLBufferSize(int rx, int tx)
{
    this->client.setIOBufferSize(rx, tx);
}

bool IMAPSession::mOpenFolder(MB_StringPtr folderName, bool readOnly)
{
    if (!connected())
    {
        _responseStatus.errorCode = IMAP_STATUS_OPEN_MAILBOX_FAILED;
        _responseStatus.clear();
        return false;
    }

    if (readOnly)
        return openMailbox(folderName, esp_mail_imap_auth_mode::esp_mail_imap_mode_examine, true, false);
    else
        return openMailbox(folderName, esp_mail_imap_auth_mode::esp_mail_imap_mode_select, true, false);
}

bool IMAPSession::getFolders(FoldersCollection &folders)
{
    if (!connected())
        return false;
    return getMailboxes(folders);
}

bool IMAPSession::mCloseFolder(bool expunge)
{

    // no folder opened
    if (_currentFolder.length() == 0)
    {
#if !defined(SILENT_MODE)
        MailClient.printDebug<IMAPSession *>(this,
                                             esp_mail_error_imap_str_11 /* "no mailbox opened" */,
                                             esp_mail_error_imap_str_11 /* "no mailbox opened" */,
                                             esp_mail_debug_tag_type_client,
                                             true,
                                             false);
#endif
        return false;
    }

    return closeMailbox(expunge);
}

bool IMAPSession::mListen(bool recon)
{
    if (!MailClient.sessionExisted<IMAPSession *>(this))
        return false;

    if (_currentFolder.length() == 0)
    {
        _mbif._floderChangedState = false;
        _mbif._folderChanged = false;
        return false;
    }

    if (!MailClient.reconnect(this))
        return false;

    if (!connected())
    {
        if (!_imap_data || (millis() - _last_server_connect_ms < 2000 && _last_server_connect_ms > 0))
            return false;

        _last_server_connect_ms = millis();

        bool ssl = false;

        if (!connect(ssl))
        {
            MailClient.closeTCPSession<IMAPSession *>(this);
            return false;
        }

        // re-authenticate after session closed
        if (!MailClient.imapAuth(this, ssl))
        {
            MailClient.closeTCPSession<IMAPSession *>(this);
            return false;
        }

        // re-open folder
        if (!selectFolder(_currentFolder.c_str()))
            return false;
    }

    // no IDLE was not supported (should be checked after imapAuth)
    if (!_feature_capability[esp_mail_imap_read_capability_idle])
    {
#if !defined(SILENT_MODE)
        printDebugNotSupported();
#endif
    }

    if (_mbif._idleTimeMs == 0)
    {
        _mbif._polling_status.messageNum = 0;
        _mbif._polling_status.type = imap_polling_status_type_undefined;
        _mbif._polling_status.argument.clear();
        _mbif._recentCount = 0;
        _mbif._folderChanged = false;

#if !defined(SILENT_MODE)
        MB_String dbMsg;

        if (!recon)
        {

            dbMsg = esp_mail_dbg_str_51; /* "listening to " */
            dbMsg += _currentFolder;
            dbMsg += esp_mail_dbg_str_52; /* " folder changes" */
            MailClient.printDebug<IMAPSession *>(this,
                                                 esp_mail_cb_str_29 /* "Listening to mailbox changes..." */,
                                                 dbMsg.c_str(),
                                                 esp_mail_debug_tag_type_client,
                                                 true,
                                                 false);
        }

#endif

        if (MailClient.imapSend(this, prependTag(imap_commands[esp_mail_imap_command_idle].text).c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
            return false;

        _imap_cmd = esp_mail_imap_cmd_idle;
        if (!MailClient.handleIMAPResponse(this, IMAP_STATUS_BAD_COMMAND, false))
            return false;

#if !defined(SILENT_MODE)
        if (!recon)
        {

            dbMsg = esp_mail_str_88 /* "polling established on " */;
            dbMsg += MailClient.Time.getDateTimeString(MailClient.Time.getCurrentTimestamp(), "%B %d, %Y %H:%M:%S");
            MailClient.printDebug<IMAPSession *>(this,
                                                 esp_mail_cb_str_49 /* "Polling established" */,
                                                 dbMsg.c_str(),
                                                 esp_mail_debug_tag_type_client,
                                                 true,
                                                 false);
        }

#endif

        _mbif._idleTimeMs = millis();
    }
    else
    {
        if (_mbif._floderChangedState)
        {
            _mbif._floderChangedState = false;
            _mbif._folderChanged = false;
            _mbif._polling_status.messageNum = 0;
            _mbif._polling_status.type = imap_polling_status_type_undefined;
            _mbif._polling_status.argument.clear();
            _mbif._recentCount = 0;
        }

        size_t imap_idle_tmo = _imap_data->limit.imap_idle_timeout;

        if (imap_idle_tmo < 60 * 1000 || imap_idle_tmo > 29 * 60 * 1000)
            imap_idle_tmo = 10 * 60 * 1000;

        size_t host_check_interval = _imap_data->limit.imap_idle_host_check_interval;

        if (host_check_interval < 30 * 1000 || host_check_interval > imap_idle_tmo)
            host_check_interval = 60 * 1000;

        if (millis() - _last_host_check_ms > host_check_interval && connected())
        {
            _last_host_check_ms = millis();

            IPAddress ip;

            if (client.hostByName(_session_cfg->server.host_name.c_str(), ip) != 1)
            {
                closeSession();
                _mbif._idleTimeMs = millis();
                return false;
            }
        }

        return MailClient.parseIdleResponse(this);
    }

    return true;
}

bool IMAPSession::mStopListen(bool recon)
{
    _mbif._idleTimeMs = 0;
    _mbif._floderChangedState = false;
    _mbif._folderChanged = false;
    _mbif._polling_status.messageNum = 0;
    _mbif._polling_status.type = imap_polling_status_type_undefined;
    _mbif._polling_status.argument.clear();
    _mbif._recentCount = 0;

    if (!connected() || _currentFolder.length() == 0 || !_feature_capability[esp_mail_imap_read_capability_idle])
        return false;

    if (MailClient.imapSend(this, imap_commands[esp_mail_imap_command_done].text, true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
        return false;

    _imap_cmd = esp_mail_imap_cmd_done;
    if (!MailClient.handleIMAPResponse(this, IMAP_STATUS_BAD_COMMAND, false))
        return false;

#if !defined(SILENT_MODE)
    if (!recon)
    {
        MailClient.printDebug<IMAPSession *>(this,
                                             esp_mail_cb_str_50 /* "Mailbox listening stopped" */,
                                             esp_mail_dbg_str_54 /* "Mailbox listening stopped" */,
                                             esp_mail_debug_tag_type_client,
                                             true,
                                             false);
    }
#endif

    return true;
}

bool IMAPSession::folderChanged()
{
    return _mbif._floderChangedState;
}

bool IMAPSession::noop()
{

#if !defined(SILENT_MODE)
    MailClient.printDebug<IMAPSession *>(this,
                                         esp_mail_cb_str_62 /* "Sending noop..." */,
                                         esp_mail_dbg_str_83 /* "send IMAP command, NOOP" */,
                                         esp_mail_debug_tag_type_client,
                                         true,
                                         false);
#endif

    if (MailClient.imapSend(this, prependTag(imap_commands[esp_mail_imap_command_noop].text).c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
        return false;

    _imap_cmd = esp_mail_imap_cmd_noop;
    if (!MailClient.handleIMAPResponse(this, IMAP_STATUS_BAD_COMMAND, false))
        return false;

    return true;
}

void IMAPSession::checkUID()
{
    if (MailClient.strcmpP(_imap_data->fetch.uid.c_str(), 0, imap_commands[esp_mail_imap_command_uid].text) || MailClient.strcmpP(_imap_data->fetch.uid.c_str(), 0, imap_commands[esp_mail_imap_command_flags].text) ||
        MailClient.strcmpP(_imap_data->fetch.uid.c_str(), 0, imap_commands[esp_mail_imap_command_body].text) || MailClient.strcmpP(_imap_data->fetch.uid.c_str(), 0, imap_commands[esp_mail_imap_command_peek].text) || MailClient.strcmpP(_imap_data->fetch.uid.c_str(), 0, imap_commands[esp_mail_imap_command_text].text) ||
        MailClient.strcmpP(_imap_data->fetch.uid.c_str(), 0, imap_commands[esp_mail_imap_command_header].text) || MailClient.strcmpP(_imap_data->fetch.uid.c_str(), 0, imap_commands[esp_mail_imap_command_fields].text) || MailClient.strcmpP(_imap_data->fetch.uid.c_str(), 0, esp_mail_str_40 /* "[" */) ||
        MailClient.strcmpP(_imap_data->fetch.uid.c_str(), 0, esp_mail_str_41 /* "]" */) || MailClient.strcmpP(_imap_data->fetch.uid.c_str(), 0, imap_commands[esp_mail_imap_command_mime].text))
        _imap_data->fetch.uid = esp_mail_str_3; /* "*" */
}

void IMAPSession::checkPath()
{
    MB_String path = _imap_data->storage.saved_path;
    if (path[0] != '/')
    {
        path = '/';
        path += _imap_data->storage.saved_path;
        path = path.c_str();
    }
}

bool IMAPSession::headerOnly()
{
    return _headerOnly;
}

struct esp_mail_imap_msg_list_t IMAPSession::data()
{
    struct esp_mail_imap_msg_list_t ret;

    for (size_t i = 0; i < _headers.size(); i++)
    {

#if defined(MB_ARDUINO_ESP) || defined(MB_ARDUINO_PICO)
        if (MailClient.getFreeHeap() < ESP_MAIL_MIN_MEM)
            continue;
#endif
        struct esp_mail_imap_msg_item_t itm;
        itm.setRFC822Headers(&_headers[i].header_fields);
        itm.UID = _headers[i].message_uid;
        itm.msgNo = _headers[i].message_no;
        itm.flags = _headers[i].flags.c_str();
        itm.acceptLang = _headers[i].accept_language.c_str();
        itm.contentLang = _headers[i].content_language.c_str();
        itm.hasAttachment = _headers[i].hasAttachment;
        itm.fetchError = _headers[i].error_msg.c_str();

        getMessages(i, itm);

        getRFC822Messages(i, itm);

        ret.msgItems.push_back(itm);
    }

    return ret;
}

SelectedFolderInfo IMAPSession::selectedFolder()
{
    return _mbif;
}

void IMAPSession::callback(imapStatusCallback imapCallback)
{
    _statusCallback = imapCallback;
}

void IMAPSession::characterDecodingCallback(imapCharacterDecodingCallback callback)
{
    _charDecCallback = callback;
}

void IMAPSession::mimeDataStreamCallback(MIMEDataStreamCallback mimeDataStreamCallback)
{
    _mimeDataStreamCallback = mimeDataStreamCallback;
}

void IMAPSession::setSystemTime(time_t ts, float gmtOffset)
{
    MailClient.Time.setTimestamp(ts, gmtOffset);
}

void IMAPSession::keepAlive(int tcpKeepIdleSeconds, int tcpKeepIntervalSeconds, int tcpKeepCount)
{
    this->client.keepAlive(tcpKeepIdleSeconds, tcpKeepIntervalSeconds, tcpKeepCount);
}

bool IMAPSession::isKeepAlive()
{
    return this->client.isKeepAlive();
}

void IMAPSession::getMessages(uint16_t messageIndex, struct esp_mail_imap_msg_item_t &msg)
{
    msg.text.content = "";
    msg.text.charSet = "";
    msg.text.content_type = "";
    msg.text.transfer_encoding = "";
    msg.html.content = "";
    msg.html.charSet = "";
    msg.html.content_type = "";
    msg.html.transfer_encoding = "";

    if (messageIndex < _headers.size())
    {
        int sz = _headers[messageIndex].part_headers.size();
        if (sz > 0)
        {
            for (int i = 0; i < sz; i++)
            {
                if (_headers[messageIndex].part_headers[i].attach_type == esp_mail_att_type_attachment || (!_headers[messageIndex].part_headers[i].rfc822_part && _headers[messageIndex].part_headers[i].message_sub_type != esp_mail_imap_message_sub_type_rfc822))
                {
                    if (_headers[messageIndex].part_headers[i].attach_type == esp_mail_att_type_none)
                    {
                        if (_headers[messageIndex].part_headers[i].msg_type == esp_mail_msg_type_plain || _headers[messageIndex].part_headers[i].msg_type == esp_mail_msg_type_enriched)
                        {
                            msg.text.content = _headers[messageIndex].part_headers[i].text.c_str();
                            msg.text.charSet = _headers[messageIndex].part_headers[i].charset.c_str();
                            msg.text.content_type = _headers[messageIndex].part_headers[i].content_type.c_str();
                            msg.text.transfer_encoding = _headers[messageIndex].part_headers[i].content_transfer_encoding.c_str();
                        }

                        if (_headers[messageIndex].part_headers[i].msg_type == esp_mail_msg_type_html)
                        {
                            msg.html.content = _headers[messageIndex].part_headers[i].text.c_str();
                            msg.html.charSet = _headers[messageIndex].part_headers[i].charset.c_str();
                            msg.html.content_type = _headers[messageIndex].part_headers[i].content_type.c_str();
                            msg.html.transfer_encoding = _headers[messageIndex].part_headers[i].content_transfer_encoding.c_str();
                        }
                    }
                    else
                    {
                        struct esp_mail_attachment_info_t att;
                        att.filename = _headers[messageIndex].part_headers[i].filename.c_str();
                        att.mime = _headers[messageIndex].part_headers[i].content_type.c_str();
                        att.name = _headers[messageIndex].part_headers[i].name.c_str();
                        att.size = _headers[messageIndex].part_headers[i].attach_data_size;
                        att.description = _headers[messageIndex].part_headers[i].content_description.c_str();
                        att.creationDate = _headers[messageIndex].part_headers[i].creation_date.c_str();
                        att.type = _headers[messageIndex].part_headers[i].attach_type;
                        msg.attachments.push_back(att);
                    }
                }
            }
        }
    }
}

void IMAPSession::getRFC822Messages(uint16_t messageIndex, struct esp_mail_imap_msg_item_t &msg)
{
    if (messageIndex < _headers.size())
    {
        int size = _headers[messageIndex].part_headers.size();
        int partIdx = 0;
        int cIdx = 0;
        IMAP_MSG_Item *_rfc822 = nullptr;
        if (size > 0)
        {
            for (int i = 0; i < size; i++)
            {
                if (_headers[messageIndex].part_headers[i].message_sub_type == esp_mail_imap_message_sub_type_rfc822 && _headers[messageIndex].part_headers[i].attach_type != esp_mail_att_type_attachment)
                {
                    if (_headers[messageIndex].part_headers[i].rfc822_part)
                    {
                        if (partIdx > 0)
                            msg.rfc822.push_back(*_rfc822);
                        cIdx = i;
                        partIdx++;
                        _rfc822 = new IMAP_MSG_Item();
                        _rfc822->setRFC822Headers(&_headers[messageIndex].part_headers[i].rfc822_header);
                    }
                    else
                    {
                        if (MailClient.multipartMember(_headers[messageIndex].part_headers[cIdx].partNumStr, _headers[messageIndex].part_headers[i].partNumStr))
                        {
                            if (_headers[messageIndex].part_headers[i].attach_type == esp_mail_att_type_none)
                            {
                                if (_headers[messageIndex].part_headers[i].msg_type == esp_mail_msg_type_plain || _headers[messageIndex].part_headers[i].msg_type == esp_mail_msg_type_enriched)
                                {
                                    _rfc822->text.charSet = _headers[messageIndex].part_headers[i].charset.c_str();
                                    _rfc822->text.content_type = _headers[messageIndex].part_headers[i].content_type.c_str();
                                    _rfc822->text.content = _headers[messageIndex].part_headers[i].text.c_str();
                                    _rfc822->text.transfer_encoding = _headers[messageIndex].part_headers[i].content_transfer_encoding.c_str();
                                }
                                if (_headers[messageIndex].part_headers[i].msg_type == esp_mail_msg_type_html)
                                {
                                    _rfc822->html.charSet = _headers[messageIndex].part_headers[i].charset.c_str();
                                    _rfc822->html.content_type = _headers[messageIndex].part_headers[i].content_type.c_str();
                                    _rfc822->html.content = _headers[messageIndex].part_headers[i].text.c_str();
                                    _rfc822->html.transfer_encoding = _headers[messageIndex].part_headers[i].content_transfer_encoding.c_str();
                                }
                            }
                            else
                            {
                                struct esp_mail_attachment_info_t att;
                                att.filename = _headers[messageIndex].part_headers[i].filename.c_str();
                                att.mime = _headers[messageIndex].part_headers[i].content_type.c_str();
                                att.name = _headers[messageIndex].part_headers[i].name.c_str();
                                att.size = _headers[messageIndex].part_headers[i].attach_data_size;
                                att.description = _headers[messageIndex].part_headers[i].content_description.c_str();
                                att.creationDate = _headers[messageIndex].part_headers[i].creation_date.c_str();
                                att.type = _headers[messageIndex].part_headers[i].attach_type;
                                _rfc822->attachments.push_back(att);
                            }
                        }
                    }
                }
            }

            if ((int)msg.rfc822.size() < partIdx && _rfc822 != nullptr)
                msg.rfc822.push_back(*_rfc822);
        }
    }
}

bool IMAPSession::closeMailbox(bool expunge)
{
#if !defined(SILENT_MODE)
    MB_String dbMsg = esp_mail_dbg_str_32; /* "closing the " */
    dbMsg += _currentFolder;
    dbMsg += esp_mail_str_89; /* " folder..." */

    MailClient.printDebug<IMAPSession *>(this,
                                         esp_mail_cb_str_27 /* "Closing the mailbox folder..." */,
                                         dbMsg.c_str(),
                                         esp_mail_debug_tag_type_client,
                                         true,
                                         false);
#endif
    if (!MailClient.reconnect(this))
        return false;

    // If folder was opened in readonly mode, use CLOSE command will not expunge the deleted messages
    // Or user intent to expunge the deleted message after close the folder.
    if (expunge || _readOnlyMode)
    {

        if (MailClient.imapSend(this, prependTag(imap_commands[esp_mail_imap_command_close].text).c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
            return false;

        _imap_cmd = esp_mail_imap_cmd_close;
        if (!MailClient.handleIMAPResponse(this, IMAP_STATUS_CLOSE_MAILBOX_FAILED, false))
            return false;
    }
    else
    {
        // Close folder without expunge
        if (_feature_capability[esp_mail_imap_read_capability_unselect])
        {
            if (MailClient.imapSend(this, prependTag(imap_commands[esp_mail_imap_command_unselect].text).c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
                return false;

            _imap_cmd = esp_mail_imap_cmd_unselect;
            if (!MailClient.handleIMAPResponse(this, IMAP_STATUS_CLOSE_MAILBOX_FAILED, false))
                return false;
        }
        else
        {
            // Open non-existing folder
            MB_String folder = esp_mail_str_84 /* "message" */;
            folder += MailClient.Time.getCurrentTimestamp();
            MB_StringPtr folderPtr;
            folderPtr = toStringPtr(folder);
            openMailbox(folderPtr, esp_mail_imap_auth_mode::esp_mail_imap_mode_examine, true, true);
        }
    }

    _currentFolder.clear();
    _mailboxOpened = false;

    return true;
}

bool IMAPSession::openMailbox(MB_StringPtr folder, esp_mail_imap_auth_mode mode, bool waitResponse, bool unselect)
{

    if (!MailClient.sessionExisted<IMAPSession *>(this))
        return false;

    MB_String _folder = folder;

    if (!MailClient.reconnect(this))
        return false;

    if (_folder.length() == 0)
        return false;

    bool _dbg = _debug;
    imapStatusCallback _cb = _statusCallback;

    // The SELECT/EXAMINE command automatically deselects any currently selected mailbox
    // before attempting the new selection (RFC3501 p.33)

    // folder should not close for re-selection otherwise the server returned * BAD Command Argument Error. 12

    if (!unselect)
    {

        bool sameFolder = strcmp(_currentFolder.c_str(), _folder.c_str()) == 0;

        // guards 3 seconds to prevent accidently frequently select the same folder with the same mode
        if (!_mailboxOpened && sameFolder && millis() - _lastSameFolderOpenMillis < 3000)
        {
            if ((_readOnlyMode && mode == esp_mail_imap_mode_examine) || (!_readOnlyMode && mode == esp_mail_imap_mode_select))
                return true;
        }

        if (!sameFolder)
            _currentFolder = folder;

#if !defined(SILENT_MODE)
        MB_String dbMsg = esp_mail_dbg_str_68; /* "selecting the " */
        dbMsg += _currentFolder;
        dbMsg += esp_mail_str_89; /* " folder..." */

        MailClient.printDebug<IMAPSession *>(this,
                                             esp_mail_cb_str_51 /* "Open the mailbox folder..." */,
                                             dbMsg.c_str(),
                                             esp_mail_debug_tag_type_client,
                                             true,
                                             false);
#endif
    }
    else
    {
        // Hide the callback and debug info
        _debug = false;
        _statusCallback = NULL;
        _currentFolder = folder;
    }

    MB_String cmd;
    MailClient.appendSpace(cmd, true, mode == esp_mail_imap_mode_examine ? imap_commands[esp_mail_imap_command_examine].text : imap_commands[esp_mail_imap_command_select].text);
    MailClient.appendString(cmd, _currentFolder.c_str(), false, false, esp_mail_string_mark_type_double_quote);

    if (isCondStoreSupported())
    {
        MailClient.appendSpace(cmd);
        MailClient.appendString(cmd, imap_commands[esp_mail_imap_command_condstore].text, false, false, esp_mail_string_mark_type_round_bracket);
    }

    _imap_cmd = mode == esp_mail_imap_mode_examine ? esp_mail_imap_cmd_examine : esp_mail_imap_cmd_select;

    if (MailClient.imapSend(this, cmd.c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
        return false;

    if (!unselect)
        _lastSameFolderOpenMillis = millis();

    if (waitResponse && !MailClient.handleIMAPResponse(this, IMAP_STATUS_OPEN_MAILBOX_FAILED, false))
    {
        if (!unselect)
            return false;
    }

    if (unselect)
    {
        _statusCallback = _cb;
        _debug = _dbg;
    }

    if (mode == esp_mail_imap_mode_examine)
        _readOnlyMode = true;
    else if (mode == esp_mail_imap_mode_select)
        _readOnlyMode = false;

    _mailboxOpened = !unselect;

    return true;
}

bool IMAPSession::getMailboxes(FoldersCollection &folders)
{
#if !defined(SILENT_MODE)
    MailClient.printDebug<IMAPSession *>(this,
                                         esp_mail_cb_str_16 /* "Reading the list of mailboxes..." */,
                                         esp_mail_dbg_str_35 /* "send IMAP command, LIST" */,
                                         esp_mail_debug_tag_type_client,
                                         true,
                                         false);
#endif
    _folders.clear();

    MB_String cmd;
    MailClient.appendSpace(cmd, true, imap_commands[esp_mail_imap_command_list].text);
    MailClient.appendString(cmd, NULL, false, false, esp_mail_string_mark_type_double_quote);
    MailClient.prependSpace(cmd, esp_mail_str_3 /* "*" */);

    if (MailClient.imapSend(this, cmd.c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
        return false;

    _imap_cmd = esp_mail_imap_cmd_list;
    if (!MailClient.handleIMAPResponse(this, IMAP_STATUS_LIST_MAILBOXS_FAILED, false))
        return false;

    folders = _folders;
    return true;
}

bool IMAPSession::mGetSubscribesMailboxes(MB_StringPtr reference, MB_StringPtr mailbox, FoldersCollection &folders)
{
#if !defined(SILENT_MODE)
    MailClient.printDebug<IMAPSession *>(this,
                                         esp_mail_cb_str_30 /* "Listing the subscribed mailboxes..." */,
                                         esp_mail_dbg_str_56 /* "send IMAP command, LSUB" */,
                                         esp_mail_debug_tag_type_client,
                                         true,
                                         false);
#endif
    folders.clear();

    MB_String cmd;
    MailClient.appendSpace(cmd, true, imap_commands[esp_mail_imap_command_lsub].text);
    MailClient.appendString(cmd, MB_String(reference).c_str(), false, false, esp_mail_string_mark_type_double_quote);
    MailClient.appendSpace(cmd);
    MailClient.appendString(cmd, MB_String(mailbox).c_str(), false, false, esp_mail_string_mark_type_double_quote);

    if (MailClient.imapSend(this, cmd.c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
        return false;

    FoldersCollection tmp;

    for (size_t i = 0; i < this->_folders.size(); i++)
        tmp.add(this->_folders[i]);

    this->_folders.clear();

    _imap_cmd = esp_mail_imap_cmd_lsub;
    if (!MailClient.handleIMAPResponse(this, IMAP_STATUS_LIST_MAILBOXS_FAILED, false))
        return false;

    for (size_t i = 0; i < this->_folders.size(); i++)
        folders.add(this->_folders[i]);

    this->_folders.clear();

    for (size_t i = 0; i < tmp.size(); i++)
        this->_folders.add(tmp[i]);

    tmp.clear();

    return true;
}

bool IMAPSession::mSubscribe(MB_StringPtr folder)
{
#if !defined(SILENT_MODE)
    MailClient.printDebug<IMAPSession *>(this,
                                         esp_mail_cb_str_31 /* "Subscribe mailbox..." */,
                                         esp_mail_dbg_str_57 /* "send IMAP command, SUBSCRIBE" */,
                                         esp_mail_debug_tag_type_client,
                                         true,
                                         false);
#endif
    MB_String cmd;
    MailClient.appendSpace(cmd, true, imap_commands[esp_mail_imap_command_subscribe].text);
    MailClient.appendString(cmd, MB_String(folder).c_str(), false, false, esp_mail_string_mark_type_double_quote);

    if (MailClient.imapSend(this, cmd.c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
        return false;

    _imap_cmd = esp_mail_imap_cmd_subscribe;
    if (!MailClient.handleIMAPResponse(this, IMAP_STATUS_LIST_MAILBOXS_FAILED, false))
        return false;

    return true;
}

bool IMAPSession::mUnSubscribe(MB_StringPtr folder)
{
#if !defined(SILENT_MODE)
    MailClient.printDebug<IMAPSession *>(this,
                                         esp_mail_cb_str_32 /* "Unsubscribe mailbox..." */,
                                         esp_mail_dbg_str_58 /* "send IMAP command, UNSUBSCRIBE" */,
                                         esp_mail_debug_tag_type_client,
                                         true,
                                         false);
#endif
    MB_String cmd;
    MailClient.appendSpace(cmd, true, imap_commands[esp_mail_imap_command_unsubscribe].text);
    MailClient.appendString(cmd, MB_String(folder).c_str(), false, false, esp_mail_string_mark_type_double_quote);

    if (MailClient.imapSend(this, cmd.c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
        return false;

    _imap_cmd = esp_mail_imap_cmd_unsubscribe;
    if (!MailClient.handleIMAPResponse(this, IMAP_STATUS_LIST_MAILBOXS_FAILED, false))
        return false;

    return true;
}

bool IMAPSession::mFetchSequenceSet()
{

    MB_String cmd;
    if (_imap_data->fetch.sequence_set.UID)
        MailClient.appendSpace(cmd, true, 2, imap_commands[esp_mail_imap_command_uid].text, imap_commands[esp_mail_imap_command_fetch].text);
    else
        MailClient.appendSpace(cmd, true, imap_commands[esp_mail_imap_command_fetch].text);

    MailClient.appendSpace(cmd, false, _imap_data->fetch.sequence_set.string.c_str());
    MailClient.appendString(cmd, imap_commands[esp_mail_imap_command_uid].text, false, false, esp_mail_string_mark_type_round_bracket);

    addModifier(cmd, esp_mail_imap_command_changedsince, _imap_data->fetch.modsequence);

    if (MailClient.imapSend(this, cmd.c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
        return false;

    _imap_msg_num.clear();

    _imap_cmd = esp_mail_imap_cmd_fetch_sequence_set;
    if (!MailClient.handleIMAPResponse(this, IMAP_STATUS_LIST_MAILBOXS_FAILED, false))
        return false;

    return true;
}

MB_String IMAPSession::prependTag(PGM_P cmd, PGM_P tag)
{
    MB_String s = (tag == NULL) ? esp_mail_imap_tag_str : tag;
    MailClient.appendSpace(s);
    s += cmd;
    return s;
}

bool IMAPSession::checkCapabilities()
{
#if !defined(SILENT_MODE)
    MailClient.printDebug<IMAPSession *>(this,
                                         esp_mail_cb_str_52 /* "Checking the capability..." */,
                                         esp_mail_dbg_str_76 /* "check the capability" */,
                                         esp_mail_debug_tag_type_client,
                                         true,
                                         false);
#endif
    if (MailClient.imapSend(this, prependTag(imap_commands[esp_mail_imap_command_capability].text).c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
        return false;

    _imap_cmd = esp_mail_imap_cmd_capability;
    if (!MailClient.handleIMAPResponse(this, IMAP_STATUS_CHECK_CAPABILITIES_FAILED, false))
        return false;

    return true;
}

bool IMAPSession::mCreateFolder(MB_StringPtr folderName)
{
#if !defined(SILENT_MODE)
    MailClient.printDebug<IMAPSession *>(this,
                                         esp_mail_cb_str_59 /* "Creating folder..." */,
                                         esp_mail_dbg_str_49 /* "creating folder" */,
                                         esp_mail_debug_tag_type_client,
                                         true,
                                         false);
#endif
    MB_String cmd;
    MailClient.joinStringSpace(cmd, true, 2, imap_commands[esp_mail_imap_command_create].text, folderName);

    if (MailClient.imapSend(this, cmd.c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
        return false;

    _imap_cmd = esp_mail_imap_cmd_create;
    if (!MailClient.handleIMAPResponse(this, IMAP_STATUS_BAD_COMMAND, false))
        return false;

    return true;
}

bool IMAPSession::mRenameFolder(MB_StringPtr currentFolderName, MB_StringPtr newFolderName)
{
#if !defined(SILENT_MODE)
    MailClient.printDebug<IMAPSession *>(this,
                                         esp_mail_cb_str_53 /* "Renaming folder..." */,
                                         esp_mail_dbg_str_55 /* "renaming folder" */,
                                         esp_mail_debug_tag_type_client,
                                         true,
                                         false);
#endif
    MB_String o = currentFolderName;
    MB_String n = newFolderName;

    o.trim();
    n.trim();

    if (o == n)
        return true;

    if (o.length() == 0 || n.length() == 0)
        return false;

    MB_String cmd;
    MailClient.appendSpace(cmd, true, imap_commands[esp_mail_imap_command_rename].text);
    MailClient.appendString(cmd, o.c_str(), false, false, esp_mail_string_mark_type_double_quote);
    MailClient.appendSpace(cmd);
    MailClient.appendString(cmd, n.c_str(), false, false, esp_mail_string_mark_type_double_quote);

    if (MailClient.imapSend(this, cmd.c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
        return false;

    _imap_cmd = esp_mail_imap_cmd_rename;
    if (!MailClient.handleIMAPResponse(this, IMAP_STATUS_BAD_COMMAND, false))
        return false;

    if (_currentFolder == o)
        selectFolder(n.c_str(), false);

    return true;
}

int IMAPSession::getUID(int msgNum)
{
#if !defined(SILENT_MODE)
    MailClient.printDebug<IMAPSession *>(this,
                                         esp_mail_cb_str_25 /* "Get UID..." */,
                                         esp_mail_dbg_str_79 /* "get UID" */,
                                         esp_mail_debug_tag_type_client,
                                         true,
                                         false);
#endif
    int uid = mGetUID(msgNum);

    if (!MailClient.sessionExisted<IMAPSession *>(this))
        return false;

#if !defined(SILENT_MODE)
    MB_String dbMsg = esp_mail_cb_str_54; /* "UID is " */
    dbMsg += uid;

    MailClient.printDebug<IMAPSession *>(this,
                                         dbMsg.c_str(),
                                         dbMsg.c_str(),
                                         esp_mail_debug_tag_type_client,
                                         true,
                                         false);
#endif
    return uid;
}

int IMAPSession::mGetUID(int msgNum)
{

    if (_currentFolder.length() == 0)
        return 0;

    MB_String cmd;
    MailClient.joinStringSpace(cmd, true, 3, imap_commands[esp_mail_imap_command_fetch].text, MB_String(msgNum).c_str(), imap_commands[esp_mail_imap_command_uid].text);

    if (MailClient.imapSend(this, cmd.c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
        return 0;

    _imap_cmd = esp_mail_imap_cmd_get_uid;
    if (!MailClient.handleIMAPResponse(this, IMAP_STATUS_BAD_COMMAND, false))
        return 0;

    return _uid_tmp;
}

const char *IMAPSession::getFlags(int msgNum)
{
#if !defined(SILENT_MODE)
    MailClient.printDebug<IMAPSession *>(this,
                                         esp_mail_cb_str_55 /* "Get Flags..." */,
                                         esp_mail_dbg_str_80 /* "get flags" */,
                                         esp_mail_debug_tag_type_client,
                                         true,
                                         false);
#endif

    _flags_tmp.clear();
    if (_currentFolder.length() == 0)
        return _flags_tmp.c_str();

    MB_String cmd, cmd2;
    MailClient.appendSpace(cmd, true, imap_commands[esp_mail_imap_command_fetch].text);
    MailClient.appendString(cmd2, imap_commands[esp_mail_imap_command_flags].text, false, false, esp_mail_string_mark_type_round_bracket);
    MailClient.joinStringSpace(cmd, false, 2, MB_String(msgNum).c_str(), cmd2.c_str());

    if (MailClient.imapSend(this, cmd.c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
        return _flags_tmp.c_str();

    _imap_cmd = esp_mail_imap_cmd_get_flags;
    MailClient.handleIMAPResponse(this, IMAP_STATUS_BAD_COMMAND, false);

    return _flags_tmp.c_str();
}

bool IMAPSession::mSendCustomCommand(MB_StringPtr cmd, imapResponseCallback callback, MB_StringPtr tag)
{

    _customCmdResCallback = callback;
    _cmd = cmd;
    _cmd.trim();

    MB_String _tag = cmd;
    _tag.trim();

    MB_String _tag2 = tag;
    _tag2.trim();

    if (_tag2.length() == 0)
    {
        int p = MailClient.strpos(_tag.c_str(), " ", 0);
        if (p > -1)
        {
            _tag.erase(p, _tag.length() - p);
            _tag.trim();
            _responseStatus.tag = _tag;
        }
    }
    else
    {
        _responseStatus.tag = tag;
        _responseStatus.tag.trim();
        if (MailClient.strpos(_cmd.c_str(), _responseStatus.tag.c_str(), 0, false) == -1)
            _cmd = prependTag(_cmd.c_str(), _responseStatus.tag.c_str());
    }

    // filter for specific command
    if (_cmd.find(imap_cmd_pre_tokens[esp_mail_imap_command_idle].c_str()) != MB_String::npos)
        _imap_custom_cmd = esp_mail_imap_cmd_idle;
    else if (_cmd.find(imap_cmd_pre_tokens[esp_mail_imap_command_append].c_str()) != MB_String::npos)
        _imap_custom_cmd = esp_mail_imap_cmd_append;
    else if (_cmd.find(imap_cmd_pre_tokens[esp_mail_imap_command_login].c_str()) != MB_String::npos)
        _imap_custom_cmd = esp_mail_imap_cmd_sasl_login;
    else if (_cmd.find(imap_cmd_pre_tokens[esp_mail_imap_command_logout].c_str()) != MB_String::npos)
        _imap_custom_cmd = esp_mail_imap_cmd_logout;
    else
        _imap_custom_cmd = esp_mail_imap_cmd_custom;

    if (_prev_imap_custom_cmd != _imap_custom_cmd || _imap_custom_cmd != esp_mail_imap_cmd_idle)
    {
        if (MailClient.imapSend(this, _cmd.c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
        {
            _prev_imap_custom_cmd = esp_mail_imap_cmd_custom;
            return false;
        }
    }

    _imap_cmd = esp_mail_imap_cmd_custom;

    if (!MailClient.handleIMAPResponse(this, IMAP_STATUS_BAD_COMMAND, false))
    {
        _prev_imap_custom_cmd = esp_mail_imap_cmd_custom;
        return false;
    }

    if (_imap_custom_cmd == esp_mail_imap_cmd_sasl_login)
    {
        _authenticated = true;
        _loginStatus = true;
    }
    else if (_imap_custom_cmd == esp_mail_imap_cmd_logout)
    {
        _authenticated = false;
        _loginStatus = false;
    }

    _prev_imap_custom_cmd = _imap_custom_cmd;

    if (MailClient.strposP(_cmd.c_str(), imap_auth_capabilities[esp_mail_auth_capability_starttls].text, 0, false) == 0)
    {
        bool verify = false;

        if (_session_cfg)
            verify = _session_cfg->certificate.verify;

        if (!client.connectSSL(verify))
            return false;

        // set the secure mode
        if (_session_cfg)
        {
            // We reset the prefer connection mode in case user set it.
            _session_cfg->secure.startTLS = false;
            _session_cfg->secure.mode = esp_mail_secure_mode_undefined;
        }

        _secure = true;
    }

    return true;
}

bool IMAPSession::mSendData(MB_StringPtr data, bool lastData, esp_mail_imap_command cmd)
{
    MB_String _data = data;
    if (MailClient.imapSend(this, _data.c_str(), lastData) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
        return false;

    if (lastData)
    {
        _imap_cmd = cmd;
        _cmd.clear();

        if (!MailClient.handleIMAPResponse(this, IMAP_STATUS_BAD_COMMAND, false))
            return false;
    }

    return true;
}

bool IMAPSession::mSendData(uint8_t *data, size_t size, bool lastData, esp_mail_imap_command cmd)
{

    if (MailClient.imapSend(this, data, size) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
        return false;

    if (lastData)
    {
        if (MailClient.imapSend(this, pgm2Str(esp_mail_str_42 /* "\r\n" */), false) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
            return false;

        _imap_cmd = cmd;
        _cmd.clear();

        if (!MailClient.handleIMAPResponse(this, IMAP_STATUS_BAD_COMMAND, false))
            return false;
    }

    return true;
}

bool IMAPSession::mDeleteFolder(MB_StringPtr folderName)
{
#if !defined(SILENT_MODE)
    MailClient.printDebug<IMAPSession *>(this,
                                         esp_mail_cb_str_56 /* "Deleting folder..." */,
                                         esp_mail_dbg_str_81 /* "delete folder" */,
                                         esp_mail_debug_tag_type_client,
                                         true,
                                         false);
#endif
    MB_String cmd;
    MailClient.joinStringSpace(cmd, true, 2, imap_commands[esp_mail_imap_command_delete].text, MB_String(folderName).c_str());

    if (MailClient.imapSend(this, cmd.c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
        return false;

    _imap_cmd = esp_mail_imap_cmd_delete;
    if (!MailClient.handleIMAPResponse(this, IMAP_STATUS_BAD_COMMAND, false))
        return false;

    return true;
}

bool IMAPSession::isCondStoreSupported()
{
    return _feature_capability[esp_mail_imap_read_capability_condstore];
}

bool IMAPSession::isModseqSupported()
{
    return isCondStoreSupported() && !_mbif._nomodsec;
}

void IMAPSession::addModifier(MB_String &cmd, esp_mail_imap_command_types type, int32_t modsequence)
{
    if (modsequence > -1 && isModseqSupported())
    {
        MB_String modifier;
        MailClient.joinStringSpace(modifier, false, 2, imap_commands[type].text, MB_String(modsequence).c_str());
        MailClient.appendSpace(cmd);
        MailClient.appendString(cmd, modifier.c_str(), false, false, esp_mail_string_mark_type_round_bracket);
    }
}

bool IMAPSession::deleteMsg(MessageList *toDelete, const char *sequenceSet, bool UID, bool expunge, int32_t modsequence)
{
    if ((toDelete && toDelete->_list.size() == 0) || (!toDelete && strlen(sequenceSet) == 0))
        return false;

    if (!selectFolder(_currentFolder.c_str(), false))
        return false;

#if !defined(SILENT_MODE)
    MailClient.printDebug<IMAPSession *>(this,
                                         esp_mail_cb_str_57 /* "Deleting message(s)..." */,
                                         esp_mail_dbg_str_75 /* "delete message(s)" */,
                                         esp_mail_debug_tag_type_client,
                                         true,
                                         false);
#endif

    if (!MailClient.sessionExisted<IMAPSession *>(this))
        return false;

    MB_String cmd;
    if (UID || toDelete)
    {
        MailClient.appendSpace(cmd, true, 2, imap_commands[esp_mail_imap_command_uid].text, imap_commands[esp_mail_imap_command_store].text);

        if (toDelete && toDelete->_list.size() > 0)
            MailClient.appendList<int>(cmd, toDelete->_list);
    }
    else if (!toDelete && strlen(sequenceSet) > 0)
        MailClient.appendSpace(cmd, true, imap_commands[esp_mail_imap_command_store].text);

    if (!toDelete && strlen(sequenceSet) > 0)
        cmd += sequenceSet;

    addModifier(cmd, esp_mail_imap_command_unchangedsince, modsequence);

    cmd += imap_cmd_pre_tokens[esp_mail_imap_command_plus_flags];
    MailClient.prependDot(cmd, imap_commands[esp_mail_imap_command_silent].text);
    MailClient.appendSpace(cmd);
    MailClient.appendString(cmd, esp_mail_str_91 /* "\\Deleted" */, false, false, esp_mail_string_mark_type_round_bracket);

    if (MailClient.imapSend(this, cmd.c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
        return false;

    _imap_cmd = esp_mail_imap_cmd_store;
    if (!MailClient.handleIMAPResponse(this, IMAP_STATUS_BAD_COMMAND, false))
        return false;

    if (expunge)
    {
        if (MailClient.imapSend(this, prependTag(imap_commands[esp_mail_imap_command_expunge].text).c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
            return false;

        _imap_cmd = esp_mail_imap_cmd_expunge;
        if (!MailClient.handleIMAPResponse(this, IMAP_STATUS_BAD_COMMAND, false))
            return false;
    }

    return true;
}

bool IMAPSession::mDeleteMessages(MessageList *toDelete, bool expunge, int32_t modsequence)
{
    if (toDelete->_list.size() > 0)
        return deleteMsg(toDelete, "", false, expunge);
    return true;
}

bool IMAPSession::mDeleteMessagesSet(MB_StringPtr sequenceSet, bool UID, bool expunge, int32_t modsequence)
{
    return deleteMsg(nullptr, MB_String(sequenceSet).c_str(), UID, expunge);
}

bool IMAPSession::copyMsg(MessageList *toCopy, const char *sequenceSet, bool UID, MB_StringPtr dest)
{
#if !defined(SILENT_MODE)
    MB_String dbMsg = esp_mail_dbg_str_48; /* "copying message(s) to " */
    dbMsg += dest;

    MailClient.printDebug<IMAPSession *>(this,
                                         esp_mail_cb_str_58 /* "Copying message(s)..." */,
                                         dbMsg.c_str(),
                                         esp_mail_debug_tag_type_client,
                                         true,
                                         false);
#endif

    if (!MailClient.sessionExisted<IMAPSession *>(this))
        return false;

    if ((toCopy && toCopy->_list.size() == 0) || (!toCopy && strlen(sequenceSet) == 0))
        return false;

    if (!selectFolder(_currentFolder.c_str(), false))
        return false;

    MB_String cmd;
    if (UID || toCopy)
    {
        MailClient.appendSpace(cmd, true, 2, imap_commands[esp_mail_imap_command_uid].text, imap_commands[esp_mail_imap_command_copy].text);

        if (toCopy && toCopy->_list.size() > 0)
            MailClient.appendList<int>(cmd, toCopy->_list);
    }
    else if (!toCopy && strlen(sequenceSet) > 0)
        MailClient.appendSpace(cmd, true, imap_commands[esp_mail_imap_command_copy].text);

    if (!toCopy && strlen(sequenceSet) > 0)
        cmd += sequenceSet;

    MailClient.prependSpace(cmd, MB_String(dest).c_str());

    if (MailClient.imapSend(this, cmd.c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
        return false;

    _imap_cmd = esp_mail_imap_cmd_copy;
    if (!MailClient.handleIMAPResponse(this, IMAP_STATUS_BAD_COMMAND, false))
        return false;

    return true;
}

bool IMAPSession::mCopyMessages(MessageList *toCopy, MB_StringPtr dest)
{
    return copyMsg(toCopy, "", false, dest);
}

bool IMAPSession::mCopyMessagesSet(MB_StringPtr sequenceSet, bool UID, MB_StringPtr dest)
{
    return copyMsg(nullptr, MB_String(sequenceSet).c_str(), UID, dest);
}

bool IMAPSession::moveMsg(MessageList *toMove, const char *sequenceSet, bool UID, MB_StringPtr dest)
{
#if !defined(SILENT_MODE)
    MB_String dbMsg = esp_mail_dbg_str_59; /* "moving message(s) to " */
    dbMsg += dest;

    MailClient.printDebug<IMAPSession *>(this,
                                         esp_mail_cb_str_60 /* "Moving message(s)..." */,
                                         dbMsg.c_str(),
                                         esp_mail_debug_tag_type_client,
                                         true,
                                         false);
#endif

    if (!MailClient.sessionExisted<IMAPSession *>(this))
        return false;

    if ((toMove && toMove->_list.size() == 0) || (!toMove && strlen(sequenceSet) == 0))
        return false;

    if (!_feature_capability[esp_mail_imap_read_capability_move])
    {

        bool ret = mCopyMessages(toMove, dest);

        if (ret)
            ret = mDeleteMessages(toMove, true);

        return ret;
    }

    if (!selectFolder(_currentFolder.c_str(), false))
        return false;

    MB_String cmd;
    if (UID || toMove)
    {
        MailClient.appendSpace(cmd, true, 2, imap_commands[esp_mail_imap_command_uid].text, imap_commands[esp_mail_imap_command_move].text);
        if (toMove && toMove->_list.size() > 0)
            MailClient.appendList<int>(cmd, toMove->_list);
    }
    else if (!toMove && strlen(sequenceSet) > 0)
        MailClient.appendSpace(cmd, true, imap_commands[esp_mail_imap_command_move].text);

    if (!toMove && strlen(sequenceSet) > 0)
        cmd += sequenceSet;

    MailClient.prependSpace(cmd, MB_String(dest).c_str());

    if (MailClient.imapSend(this, cmd.c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
        return false;

    _imap_cmd = esp_mail_imap_cmd_move;
    if (!MailClient.handleIMAPResponse(this, IMAP_STATUS_BAD_COMMAND, false))
        return false;

    return true;
}

bool IMAPSession::mMoveMessages(MessageList *toMove, MB_StringPtr dest)
{
    return moveMsg(toMove, "", false, dest);
}

bool IMAPSession::mMoveMessagesSet(MB_StringPtr sequenceSet, bool UID, MB_StringPtr dest)
{
    return moveMsg(nullptr, MB_String(sequenceSet).c_str(), UID, dest);
}

bool IMAPSession::mGetSetQuota(MB_StringPtr quotaRoot, IMAP_Quota_Root_Info *data, bool getMode)
{

#if !defined(SILENT_MODE)
    PGM_P p1 = getMode ? esp_mail_cb_str_33 /* "Get quota root resource usage and limit..." */ : esp_mail_cb_str_34 /* "Set quota root resource usage and limit..." */;
    PGM_P p2 = getMode ? esp_mail_dbg_str_60 /* "send IMAP command, GETQUOTA" */ : esp_mail_dbg_str_61 /* "send IMAP command, SETQUOTA" */;
    MailClient.printDebug<IMAPSession *>(this, p1, p2, esp_mail_debug_tag_type_client, true, false);
#endif

    if (!_feature_capability[esp_mail_imap_read_capability_quota])
    {
#if !defined(SILENT_MODE)
        printDebugNotSupported();
#endif
        return false;
    }

    MB_String _quotaRoot = quotaRoot;

    MB_String cmd;
    if (getMode)
    {
        MailClient.appendSpace(cmd, true, imap_commands[esp_mail_imap_command_getquota].text);
        MailClient.appendString(cmd, _quotaRoot.c_str(), false, false, esp_mail_string_mark_type_double_quote);
    }
    else
    {
        MailClient.appendSpace(cmd, true, imap_commands[esp_mail_imap_command_setquota].text);
        MailClient.appendString(cmd, _quotaRoot.c_str(), false, false, esp_mail_string_mark_type_double_quote);
        MailClient.appendSpace(cmd);

        MB_String cmd2;
        MailClient.joinStringSpace(cmd2, false, 2, data->name.c_str(), MB_String((int)data->limit).c_str());
        MailClient.appendString(cmd, cmd2.c_str(), false, false, esp_mail_string_mark_type_round_bracket);
    }

    if (MailClient.imapSend(this, cmd.c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
        return false;

    _quota_tmp.clear();

    _imap_cmd = (getMode) ? esp_mail_imap_cmd_get_quota : esp_mail_imap_cmd_set_quota;
    if (!MailClient.handleIMAPResponse(this, IMAP_STATUS_BAD_COMMAND, false))
        return false;

    if (getMode)
    {
        mParseQuota(_quota_tmp.c_str(), data);
    }

    _quota_tmp.clear();

    return true;
}

void IMAPSession::mParseQuota(const char *quota, IMAP_Quota_Root_Info *data)
{
    _vectorImpl<MB_String> tokens;
    MailClient.splitToken(quota, tokens, " ");
    data->quota_root = tokens[0];
    tokens[1].erase(0, 1);
    data->name = tokens[1];
    data->usage = atoi(tokens[2].c_str());
    data->limit = atoi(tokens[3].c_str());
}

bool IMAPSession::mGetQuotaRoots(MB_StringPtr mailbox, IMAP_Quota_Roots_List *quotaRootsList)
{
#if !defined(SILENT_MODE)
    MailClient.printDebug<IMAPSession *>(this,
                                         esp_mail_cb_str_35 /* "Get the list of quota roots..." */,
                                         esp_mail_dbg_str_62 /* "send IMAP command, GETQUOTAROOT" */,
                                         esp_mail_debug_tag_type_client,
                                         true,
                                         false);
#endif

    if (!MailClient.sessionExisted<IMAPSession *>(this))
        return false;

    if (!_feature_capability[esp_mail_imap_read_capability_quota])
    {
#if !defined(SILENT_MODE)
        printDebugNotSupported();
#endif
        return false;
    }

    MB_String _mailbox = mailbox;

    MB_String cmd;
    MailClient.appendSpace(cmd, true, 2, imap_commands[esp_mail_imap_command_getquota].text, imap_commands[esp_mail_imap_command_root].text);
    MailClient.appendString(cmd, _mailbox.c_str(), false, false, esp_mail_string_mark_type_double_quote);

    if (MailClient.imapSend(this, cmd.c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
        return false;

    _quota_root_tmp.clear();
    _quota_tmp.clear();

    _imap_cmd = esp_mail_imap_cmd_get_quota_root;
    if (!MailClient.handleIMAPResponse(this, IMAP_STATUS_BAD_COMMAND, false))
        return false;

    _vectorImpl<MB_String> tokens;
    MailClient.splitToken(_quota_root_tmp.c_str(), tokens, ",");

    for (size_t i = 0; i < tokens.size(); i++)
    {
        _vectorImpl<MB_String> tk;
        MailClient.splitToken(tokens[i].c_str(), tk, ":");
        IMAP_Quota_Root_Info data;
        if (tk.size() > 1)
            mParseQuota(tk[1].c_str(), &data);
        else
            data.quota_root = tk[0];

        quotaRootsList->add(data);
    }

    _quota_root_tmp.clear();
    _quota_tmp.clear();

    return true;
}

bool IMAPSession::mManageACL(MB_StringPtr mailbox, IMAP_Rights_List *acl_list, IMAP_Rights_Info *acl, MB_StringPtr identifier, esp_mail_imap_command type)
{

#if !defined(SILENT_MODE)

    PGM_P p1 = NULL;
    PGM_P p2 = NULL;
    if (type == esp_mail_imap_cmd_get_acl)
    {
        p1 = esp_mail_cb_str_36;  /* "Get the ACL..." */
        p2 = esp_mail_dbg_str_77; /* "get the ACL" */
    }
    else if (type == esp_mail_imap_cmd_set_acl)
    {
        p1 = esp_mail_cb_str_37;  /* "Setting the ACL..." */
        p2 = esp_mail_dbg_str_78; /* "set the ACL" */
    }
    else if (type == esp_mail_imap_cmd_delete_acl)
    {
        p1 = esp_mail_cb_str_38;  /* "Deleting the ACL..." */
        p2 = esp_mail_dbg_str_72; /* "delete the ACL" */
    }
    else if (type == esp_mail_imap_cmd_my_rights)
    {
        p1 = esp_mail_cb_str_39;  /* "Get my ACL..." */
        p2 = esp_mail_dbg_str_23; /* "get my ACL" */
    }

    MailClient.printDebug<IMAPSession *>(this, p1, p2, esp_mail_debug_tag_type_client, true, false);

#endif

    if (!MailClient.sessionExisted<IMAPSession *>(this))
        return false;

    if (!_feature_capability[esp_mail_imap_read_capability_acl])
    {
#if !defined(SILENT_MODE)
        printDebugNotSupported();
#endif
        return false;
    }

    MB_String _mailbox = mailbox;
    MB_String _identifier = identifier;

    MB_String cmd;
    if (type == esp_mail_imap_cmd_get_acl)
    {
        MailClient.appendSpace(cmd, true, imap_commands[esp_mail_imap_command_get_acl].text);
        MailClient.appendString(cmd, _mailbox.c_str(), false, false, esp_mail_string_mark_type_double_quote);
    }
    else if (type == esp_mail_imap_cmd_set_acl)
    {
        MailClient.appendSpace(cmd, true, imap_commands[esp_mail_imap_command_set_acl].text);
        MailClient.appendString(cmd, MB_String(mailbox).c_str(), false, false, esp_mail_string_mark_type_double_quote);
        MailClient.appendSpace(cmd);
        MailClient.appendString(cmd, acl->identifier.c_str(), false, false, esp_mail_string_mark_type_double_quote);
        MailClient.appendSpace(cmd);
        MB_String rights;
        getRights(rights, acl);
        MailClient.appendString(cmd, rights.c_str(), false, false, esp_mail_string_mark_type_double_quote);
    }
    else if (type == esp_mail_imap_cmd_delete_acl)
    {
        MailClient.appendSpace(cmd, true, imap_commands[esp_mail_imap_command_delete_acl].text);
        MailClient.appendString(cmd, _mailbox.c_str(), false, false, esp_mail_string_mark_type_double_quote);
        MailClient.appendSpace(cmd);
        MailClient.appendString(cmd, _identifier.c_str(), false, false, esp_mail_string_mark_type_double_quote);
    }
    else if (type == esp_mail_imap_cmd_my_rights)
    {
        MailClient.appendSpace(cmd, true, imap_commands[esp_mail_imap_command_myrights].text);
        MailClient.appendString(cmd, _mailbox.c_str(), false, false, esp_mail_string_mark_type_double_quote);
    }

    if (MailClient.imapSend(this, cmd.c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
        return false;

    _acl_tmp.clear();

    _imap_cmd = type;
    if (!MailClient.handleIMAPResponse(this, IMAP_STATUS_BAD_COMMAND, false))
        return false;

    if (type == esp_mail_imap_cmd_get_acl)
        parseACL(_acl_tmp, acl_list);
    else if (type == esp_mail_imap_cmd_my_rights)
        parseRights(_acl_tmp, acl);

    _acl_tmp.clear();

    return true;
}

void IMAPSession::parseACL(MB_String &acl_str, IMAP_Rights_List *right_list)
{
    _vectorImpl<MB_String> tokens;
    MailClient.splitToken(acl_str.c_str(), tokens, " ");

    for (size_t i = 0; i < tokens.size(); i += 2)
    {
        IMAP_Rights_Info info;
        info.identifier = tokens[i];
        parseRights(tokens[i + 1], &info);
        right_list->add(info);
    }
}

void IMAPSession::parseRights(MB_String &righs_str, IMAP_Rights_Info *info)
{
    for (size_t i = 0; i < righs_str.length(); i++)
    {
        uint8_t c = righs_str[i] - 97;
        if (c >= esp_mail_imap_rights_administer && c < esp_mail_imap_rights_maxType)
            info->rights[c] = true;
    }
}

void IMAPSession::getRights(MB_String &righs_str, IMAP_Rights_Info *info)
{
    for (size_t i = esp_mail_imap_rights_administer; i < esp_mail_imap_rights_maxType; i++)
    {
        if (info->rights[i])
            righs_str += (char)(i + 97);
    }
}

bool IMAPSession::mNamespace(IMAP_Namespaces_List *ns)
{

#if !defined(SILENT_MODE)
    MailClient.printDebug<IMAPSession *>(this,
                                         esp_mail_cb_str_40 /* "Get namespace..." */,
                                         esp_mail_dbg_str_67 /* "send IMAP command, NAMESPACE" */,
                                         esp_mail_debug_tag_type_client,
                                         true,
                                         false);
#endif

    if (!MailClient.sessionExisted<IMAPSession *>(this))
        return false;

    if (!_feature_capability[esp_mail_imap_read_capability_namespace])
    {
#if !defined(SILENT_MODE)
        printDebugNotSupported();
#endif
        return false;
    }

    if (MailClient.imapSend(this, prependTag(imap_commands[esp_mail_imap_command_namespace].text).c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
        return false;

    _ns_tmp.clear();

    _imap_cmd = esp_mail_imap_cmd_namespace;
    if (!MailClient.handleIMAPResponse(this, IMAP_STATUS_BAD_COMMAND, false))
        return false;

    int cnt = 0;
    MB_String personal, other, shared;

    for (size_t i = 0; i < _ns_tmp.length(); i++)
    {
        if (i > 0 && i < _ns_tmp.length() - 1 && _ns_tmp[i] == ' ' && _ns_tmp[i - 1] != '"' && _ns_tmp[i + 1] != '"')
        {
            if (cnt == 0)
                personal = _ns_tmp.substr(0, i);
            else if (cnt == 1)
            {
                other = _ns_tmp.substr(personal.length() + 1, i - personal.length() - 1);
                shared = _ns_tmp.substr(i + 1, _ns_tmp.length() - i - 1);
            }
            cnt++;
        }
    }

    if (personal.length() > 4)
        parseNamespaces(personal, &(ns->personal_namespaces));

    if (other.length() > 4)
        parseNamespaces(other, &(ns->other_users_namespaces));

    if (shared.length() > 4)
        parseNamespaces(shared, &(ns->shared_namespaces));

    return true;
}

bool IMAPSession::mEnable(MB_StringPtr capability)
{

#if !defined(SILENT_MODE)
    MailClient.printDebug<IMAPSession *>(this,
                                         esp_mail_cb_str_41 /* "Enable capability..." */,
                                         esp_mail_dbg_str_68 /* "send IMAP command, ENABLE" */,
                                         esp_mail_debug_tag_type_client,
                                         true,
                                         false);
#endif

    if (!_feature_capability[esp_mail_imap_read_capability_enable])
    {
#if !defined(SILENT_MODE)
        printDebugNotSupported();
#endif
        return false;
    }

    MB_String _cap = capability;

    MB_String cmd;
    MailClient.joinStringSpace(cmd, true, 2, imap_commands[esp_mail_imap_command_enable].text, _cap.c_str());

    if (MailClient.imapSend(this, cmd.c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
        return false;

    _imap_cmd = esp_mail_imap_cmd_enable;
    if (!MailClient.handleIMAPResponse(this, IMAP_STATUS_BAD_COMMAND, false))
        return false;

    return true;
}

void IMAPSession::parseNamespaces(MB_String &ns_str, IMAP_Namespaces *ns)
{
    MB_String tmp = ns_str.substr(2, ns_str.length() - 4);
    tmp.replaceAll(")(", " ");
    _vectorImpl<MB_String> tokens;
    MailClient.splitToken(tmp.c_str(), tokens, " ");

    for (size_t i = 0; i < tokens.size(); i += 2)
    {
        IMAP_Namespace_Info info;
        info.prefix = tokens[i];
        info.delimiter = tokens[i + 1];
        ns->add(info);
    }
}

void IMAPSession::empty()
{
    _nextUID.clear();
    _unseenMsgIndex.clear();
    _flags_tmp.clear();
    _quota_tmp.clear();
    _quota_root_tmp.clear();
    _acl_tmp.clear();
    _ns_tmp.clear();
    _server_id_tmp.clear();
    _sdFileList.clear();
    clearMessageData();
}

IMAP_Status IMAPSession::status()
{
    return _cbData;
}

String IMAPSession::fileList()
{
    return _sdFileList.c_str();
}

void IMAPSession::clearMessageData()
{
    for (size_t i = 0; i < _headers.size(); i++)
    {
        _headers[i].part_headers.clear();
    }

    _headers.clear();
    _imap_msg_num.clear();
    _mbif._searchCount = 0;
    _flags_tmp.clear();

#if defined(MB_USE_STD_VECTOR)
    _folders.clear();
    _mbif._flags.clear();
#endif
}

void IMAPSession::printDebugNotSupported()
{

#if !defined(SILENT_MODE)
    MailClient.printDebug<IMAPSession *>(this,
                                         esp_mail_error_imap_str_13 /* "not supported by IMAP server" */,
                                         esp_mail_error_imap_str_13 /* "not supported by IMAP server" */,
                                         esp_mail_debug_tag_type_error,
                                         true,
                                         false);
#endif
}

IMAP_Status::IMAP_Status()
{
}

IMAP_Status::~IMAP_Status()
{
    empty();
}

const char *IMAP_Status::info()
{
    return _info.c_str();
}

bool IMAP_Status::success()
{
    return _success;
}

void IMAP_Status::empty()
{
    _info.clear();
}

#endif

#endif /* ESP_MAIL_IMAP_H */