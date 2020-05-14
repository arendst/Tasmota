/*
 *Mail Client Arduino Library for ESP32, version 2.1.4
 *
 * April 12, 2020
 *
 * This library allows ESP32 to send Email with/without attachment and receive Email with/without attachment download through SMTP and IMAP servers.
 *
 * The library supports all ESP32 MCU based modules.
 *
 * The MIT License (MIT)
 * Copyright (c) 2019 K. Suwatchai (Mobizt)
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

#ifndef ESP32_MailClient_CPP
#define ESP32_MailClient_CPP

#ifdef ESP32

#include "ESP32_MailClient.h"

struct ESP32_MailClient::IMAP_COMMAND_TYPE
{
  static const uint8_t LOGIN = 0;
  static const uint8_t LIST = 1;
  static const uint8_t SELECT = 2;
  static const uint8_t EXAMINE = 3;
  static const uint8_t STATUS = 4;
  static const uint8_t SEARCH = 5;
  static const uint8_t FETCH_BODY_HEADER = 6;
  static const uint8_t FETCH_BODY_MIME = 7;
  static const uint8_t FETCH_BODY_TEXT = 8;
  static const uint8_t FETCH_BODY_ATTACHMENT = 9;
  static const uint8_t LOGOUT = 10;
};

struct ESP32_MailClient::IMAP_HEADER_TYPE
{
  static const uint8_t FROM = 1;
  static const uint8_t TO = 2;
  static const uint8_t CC = 3;
  static const uint8_t SUBJECT = 4;
  static const uint8_t DATE = 5;
  static const uint8_t MSG_ID = 6;
  static const uint8_t CONT_LANG = 7;
  static const uint8_t ACCEPT_LANG = 8;
};



bool ESP32_MailClient::readMail(IMAPData &imapData)
{

  std::string buf;
  std::string command = "$";

  size_t mailIndex = 0;
  int messageDataIndex = 0;
  int partID = 1;
  int _partID = 1;
  bool res = false;
  bool _res = false;
  bool starttls = imapData._starttls;
  bool connected = false;

  int bufSize = 50;

  char *_val = new char[bufSize];
  char *_part = new char[bufSize];

  unsigned long dataTime = 0;

  int count = 0;

  imapData._net->setDebugCallback(NULL);

  if (imapData._debug)
  {
    ESP32MailDebugInfo(ESP32_MAIL_STR_225);
    ESP32MailDebug(imapData._host.c_str());
    ESP32MailDebug(String(imapData._port).c_str());
  }


  if (imapData._readCallback)
  {
    imapData._cbData._info = ESP32_MAIL_STR_50;
    imapData._cbData._status = ESP32_MAIL_STR_51;
    imapData._cbData._success = false;
    imapData._readCallback(imapData._cbData);
  }

  if (imapData._debug)
    imapData._net->setDebugCallback(ESP32MailDebug);

  if (imapData._rootCA.size() > 0)
    imapData._net->begin(imapData._host.c_str(), imapData._port, ESP32_MAIL_STR_202, (const char *)imapData._rootCA.front());
  else
    imapData._net->begin(imapData._host.c_str(), imapData._port, ESP32_MAIL_STR_202, (const char *)NULL);

  while (!imapData._net->connected() && count < 10)
  {

    count++;

    if (!imapData._net->connect(starttls))
    {

      _imapStatus = IMAP_STATUS_SERVER_CONNECT_FAILED;

      if (imapData._readCallback)
      {
        imapData._cbData._info = ESP32_MAIL_STR_53 + imapErrorReasonStr();
        imapData._cbData._status = ESP32_MAIL_STR_52;
        imapData._cbData._success = false;
        imapData._readCallback(imapData._cbData);
      }
      if (imapData._debug)
      {
        ESP32MailDebugError();
        ESP32MailDebugLine(imapErrorReasonStr().c_str(), true);
      }
    }
    else
    {
      break;
    }
  }

  if (!imapData._net->connect(starttls))
  {
    goto out;
  }

  connected = true;

  if (imapData._readCallback)
  {
    imapData._cbData._info = ESP32_MAIL_STR_54;
    imapData._cbData._status = ESP32_MAIL_STR_55;
    imapData._cbData._success = false;
    imapData._readCallback(imapData._cbData);
  }

  if (imapData._debug)
    ESP32MailDebugInfo(ESP32_MAIL_STR_228);

  //Don't expect handshake from some servers
  dataTime = millis();

  while (imapData._net->connected() && !imapData._net->getStreamPtr()->available() && millis() - 500 < dataTime)
    delay(0);

  if (imapData._net->connected() && imapData._net->getStreamPtr()->available())
    while (imapData._net->getStreamPtr()->available())
      imapData._net->getStreamPtr()->read();

  imapData.clearMessageData();

  if (imapData._readCallback)
  {
    imapData._cbData._info = ESP32_MAIL_STR_56;
    imapData._cbData._status = ESP32_MAIL_STR_57;
    imapData._cbData._success = false;
    imapData._readCallback(imapData._cbData);
  }

  if (imapData._debug)
    ESP32MailDebugInfo(ESP32_MAIL_STR_229);

  imapData._net->getStreamPtr()->print(ESP32_MAIL_STR_130);
  imapData._net->getStreamPtr()->print(imapData._loginEmail.c_str());
  imapData._net->getStreamPtr()->print(ESP32_MAIL_STR_131);
  imapData._net->getStreamPtr()->println(imapData._loginPassword.c_str());

  if (!waitIMAPResponse(imapData, IMAP_COMMAND_TYPE::LOGIN))
  {
    _imapStatus = IMAP_STATUS_LOGIN_FAILED;
    if (imapData._readCallback)
    {
      imapData._cbData._info = ESP32_MAIL_STR_53 + imapErrorReasonStr();
      imapData._cbData._status = ESP32_MAIL_STR_52;
      imapData._cbData._success = false;
      imapData._readCallback(imapData._cbData);
    }
    if (imapData._debug)
    {
      ESP32MailDebugError();
      ESP32MailDebugLine(imapErrorReasonStr().c_str(), true);
    }
    goto out;
  }

  if (imapData._fetchUID.length() > 0)
    imapData._headerOnly = false;
  else
    imapData._headerOnly = true;

  if (imapData._headerOnly)
  {
    if (imapData._readCallback)
    {
      imapData._cbData._info = ESP32_MAIL_STR_58;
      imapData._cbData._status = ESP32_MAIL_STR_59;
      imapData._cbData._success = false;
      imapData._readCallback(imapData._cbData);
    }

    if (imapData._debug)
      ESP32MailDebugInfo(ESP32_MAIL_STR_230);

    imapData._net->getStreamPtr()->println(ESP32_MAIL_STR_133);
    if (!waitIMAPResponse(imapData, IMAP_COMMAND_TYPE::LIST))
    {
      _imapStatus = IMAP_STATUS_BAD_COMMAND;
      if (imapData._readCallback)
      {
        imapData._cbData._info = ESP32_MAIL_STR_53 + imapErrorReasonStr();
        imapData._cbData._status = ESP32_MAIL_STR_52;
        imapData._cbData._success = false;
        imapData._readCallback(imapData._cbData);
      }
      if (imapData._debug)
      {
        ESP32MailDebugError();
        ESP32MailDebugLine(imapErrorReasonStr().c_str(), true);
      }
      imapData._cbData.empty();
    }

    if (imapData._readCallback)
    {

      imapData._cbData._info = ESP32_MAIL_STR_60;
      imapData._cbData._success = false;
      imapData._readCallback(imapData._cbData);

      for (size_t i = 0; i < imapData._folders.size(); i++)
      {
        imapData._cbData._info = imapData._folders[i];
        imapData._cbData._status = "";
        imapData._cbData._success = false;
        imapData._readCallback(imapData._cbData);
      }

      imapData._cbData._info = ESP32_MAIL_STR_61 + imapData._currentFolder + ESP32_MAIL_STR_97;
      imapData._cbData._status = "";
      imapData._cbData._success = false;
      imapData._readCallback(imapData._cbData);
    }
  }

  if (imapData._debug)
    ESP32MailDebugInfo(ESP32_MAIL_STR_231);

  imapData._net->getStreamPtr()->print(ESP32_MAIL_STR_135);
  imapData._net->getStreamPtr()->print(imapData._currentFolder.c_str());
  imapData._net->getStreamPtr()->println(ESP32_MAIL_STR_136);
  if (!waitIMAPResponse(imapData, IMAP_COMMAND_TYPE::EXAMINE))
  {
    _imapStatus = IMAP_STATUS_BAD_COMMAND;
    if (imapData._readCallback)
    {
      imapData._cbData._info = ESP32_MAIL_STR_53 + imapErrorReasonStr();
      imapData._cbData._status = ESP32_MAIL_STR_52;
      imapData._cbData._success = false;
      imapData._readCallback(imapData._cbData);
    }
    if (imapData._debug)
    {
      ESP32MailDebugError();
      ESP32MailDebugLine(imapErrorReasonStr().c_str(), true);
    }
    goto out;
  }

  if (imapData._headerOnly)
  {

    if (imapData._readCallback)
    {
      imapData._cbData._info = ESP32_MAIL_STR_62 + imapData._nextUID;
      imapData._cbData._status = "";
      imapData._cbData._success = false;
      imapData._readCallback(imapData._cbData);

      imapData._cbData._info = ESP32_MAIL_STR_63;
      memset(_val, 0, bufSize);
      itoa(imapData._totalMessage, _val, 10);
      imapData._cbData._info += _val;
      imapData._cbData._status = "";
      imapData._cbData._success = false;
      imapData._readCallback(imapData._cbData);

      imapData._cbData._info = ESP32_MAIL_STR_64;
      imapData._cbData._status = "";
      imapData._cbData._success = false;
      imapData._readCallback(imapData._cbData);

      for (size_t i = 0; i < imapData._flag.size(); i++)
      {
        imapData._cbData._info = imapData._flag[i];
        imapData._cbData._status = "";
        imapData._cbData._success = false;
        imapData._readCallback(imapData._cbData);
      }

      imapData._cbData._info = ESP32_MAIL_STR_65;
      imapData._cbData._status = "";
      imapData._cbData._success = false;
      imapData._readCallback(imapData._cbData);

      imapData._cbData._info = ESP32_MAIL_STR_66;
      imapData._cbData._status = ESP32_MAIL_STR_67;
      imapData._cbData._success = false;
      imapData._readCallback(imapData._cbData);
    }
  }

  imapData._msgNum.clear();
  imapData._uidSearch = false;
  imapData._msgID.clear();
  imapData._contentLanguage.clear();
  imapData._acceptLanguage.clear();
  imapData._attachmentCount.clear();
  imapData._totalAttachFileSize.clear();
  imapData._downloadedByte.clear();
  imapData._error.clear();
  imapData._errorMsg.clear();
  imapData._searchCount = 0;

  if (imapData._headerOnly)
  {

    if (imapData._searchCriteria != "")
    {
      if (imapData._debug)
        ESP32MailDebugInfo(ESP32_MAIL_STR_232);

      if (imapData._searchCriteria.find(ESP32_MAIL_STR_137) != std::string::npos)
      {
        imapData._uidSearch = true;
        command += ESP32_MAIL_STR_138;
      }
      command += ESP32_MAIL_STR_139;

      for (size_t i = 0; i < imapData._searchCriteria.length(); i++)
      {
        if (imapData._searchCriteria[i] != ' ' && imapData._searchCriteria[i] != '\r' && imapData._searchCriteria[i] != '\n' && imapData._searchCriteria[i] != '$')
          buf.append(1, imapData._searchCriteria[i]);

        if (imapData._searchCriteria[i] == ' ')
        {
          if ((imapData._uidSearch && buf == ESP32_MAIL_STR_140) || (imapData._unseen && buf.find(ESP32_MAIL_STR_224) != std::string::npos))
            buf.clear();

          if (buf != ESP32_MAIL_STR_141 && buf != "")
          {
            command += ESP32_MAIL_STR_131;
            command += buf;
          }

          buf.clear();
        }
      }

      if (imapData._unseen && imapData._searchCriteria.find(ESP32_MAIL_STR_223) == std::string::npos)
        command += ESP32_MAIL_STR_223;

      if (buf.length() > 0)
      {
        command += ESP32_MAIL_STR_131;
        command += buf;
      }

      imapData._net->getStreamPtr()->println(command.c_str());

      std::string().swap(command);

      if (!waitIMAPResponse(imapData, IMAP_COMMAND_TYPE::SEARCH, 1))
      {
        _imapStatus = IMAP_STATUS_BAD_COMMAND;
        if (imapData._readCallback)
        {
          imapData._cbData._info = ESP32_MAIL_STR_53 + imapErrorReasonStr();
          imapData._cbData._status = ESP32_MAIL_STR_52;
          imapData._cbData._success = false;
          imapData._readCallback(imapData._cbData);
        }
        if (imapData._debug)
        {
          ESP32MailDebugError();
          ESP32MailDebugLine(imapErrorReasonStr().c_str(), true);
        }
        goto out;
      }

      if (imapData._readCallback)
      {

        imapData._cbData._info = ESP32_MAIL_STR_68;
        memset(_val, 0, bufSize);
        itoa(imapData._emailNumMax, _val, 10);
        imapData._cbData._info += _val;
        imapData._cbData._status = "";
        imapData._cbData._success = false;
        imapData._readCallback(imapData._cbData);

        if (imapData._msgNum.size() > 0)
        {

          imapData._cbData._info = ESP32_MAIL_STR_69;
          memset(_val, 0, bufSize);
          itoa(imapData._searchCount, _val, 10);
          imapData._cbData._info += _val;
          imapData._cbData._info += ESP32_MAIL_STR_70;
          imapData._cbData._status = "";
          imapData._cbData._success = false;
          imapData._readCallback(imapData._cbData);

          imapData._cbData._info = ESP32_MAIL_STR_71;
          memset(_val, 0, bufSize);
          itoa(imapData._msgNum.size(), _val, 10);
          imapData._cbData._info += _val;
          imapData._cbData._info += ESP32_MAIL_STR_70;
          imapData._cbData._status = "";
          imapData._cbData._success = false;
          imapData._readCallback(imapData._cbData);
        }
        else
        {
          imapData._cbData._info = ESP32_MAIL_STR_72;
          imapData._cbData._status = "";
          imapData._cbData._success = false;
          imapData._readCallback(imapData._cbData);
        }
      }
    }
    else
    {

      imapData._msgNum.push_back(imapData._totalMessage);
      if (imapData._readCallback)
      {
        imapData._cbData._info = ESP32_MAIL_STR_73;
        imapData._cbData._status = "";
        imapData._cbData._success = false;
        imapData._readCallback(imapData._cbData);
      }
    }
  }
  else
  {

    imapData._msgNum.push_back(atoi(imapData._fetchUID.c_str()));
  }

  for (int i = 0; i < imapData._messageDataInfo.size(); i++)
    imapData._messageDataInfo[i].clear();

  imapData._messageDataInfo.clear();

  for (int i = 0; i < imapData._msgNum.size(); i++)
  {

    if (imapData._debug)
      ESP32MailDebugInfo(ESP32_MAIL_STR_233);

    if (imapData._readCallback)
    {

      imapData._cbData._info = ESP32_MAIL_STR_74;
      memset(_val, 0, bufSize);
      itoa(i + 1, _val, 10);
      imapData._cbData._info += _val;

      imapData._cbData._status = "";
      if (imapData._uidSearch || imapData._fetchUID.length() > 0)
        imapData._cbData._info += ESP32_MAIL_STR_75;
      else
        imapData._cbData._info += ESP32_MAIL_STR_76;

      memset(_val, 0, bufSize);
      itoa(imapData._msgNum[i], _val, 10);
      imapData._cbData._info += _val;
      imapData._cbData._status = ESP32_MAIL_STR_77;
      imapData._cbData._success = false;
      imapData._readCallback(imapData._cbData);
    }

    imapData._date.push_back(std::string());
    imapData._subject.push_back(std::string());
    imapData._subject_charset.push_back(std::string());
    imapData._from.push_back(std::string());
    imapData._from_charset.push_back(std::string());
    imapData._to.push_back(std::string());
    imapData._to_charset.push_back(std::string());
    imapData._cc.push_back(std::string());
    imapData._attachmentCount.push_back(0);
    imapData._totalAttachFileSize.push_back(0);
    imapData._downloadedByte.push_back(0);
    imapData._messageDataCount.push_back(0);
    imapData._error.push_back(false);
    imapData._errorMsg.push_back(std::string());
    imapData._cc_charset.push_back(std::string());
    imapData._msgID.push_back(std::string());
    imapData._acceptLanguage.push_back(std::string());
    imapData._contentLanguage.push_back(std::string());

    std::vector<messageBodyData> d = std::vector<messageBodyData>();

    imapData._messageDataInfo.push_back(d);

    std::vector<messageBodyData>().swap(d);

    if (imapData._uidSearch || imapData._fetchUID.length() > 0)
      imapData._net->getStreamPtr()->print(ESP32_MAIL_STR_142);
    else
      imapData._net->getStreamPtr()->print(ESP32_MAIL_STR_143);

    imapData._net->getStreamPtr()->print(imapData._msgNum[i]);
    imapData._net->getStreamPtr()->println(ESP32_MAIL_STR_144);

    if (!waitIMAPResponse(imapData, IMAP_COMMAND_TYPE::FETCH_BODY_HEADER, 0, mailIndex))
    {
      if (imapData._headerOnly)
        _imapStatus = IMAP_STATUS_IMAP_RESPONSE_FAILED;
      else
        _imapStatus = IMAP_STATUS_BAD_COMMAND;

      if (imapData._readCallback)
      {
        imapData._cbData._info = ESP32_MAIL_STR_53 + imapErrorReasonStr();
        imapData._cbData._status = ESP32_MAIL_STR_52;
        imapData._cbData._success = false;
        imapData._readCallback(imapData._cbData);
      }
      if (imapData._debug)
      {
        ESP32MailDebugError();
        ESP32MailDebugLine(imapErrorReasonStr().c_str(), true);
      }
      goto out;
    }

    if (!imapData._headerOnly)
    {

      messageDataIndex = 0;
      partID = 1;
      _partID = 1;
      res = false;
      _res = false;

      do
      {

        if (imapData._uidSearch || imapData._fetchUID.length() > 0)
          imapData._net->getStreamPtr()->print(ESP32_MAIL_STR_142);
        else
          imapData._net->getStreamPtr()->print(ESP32_MAIL_STR_143);

        imapData._net->getStreamPtr()->print(imapData._msgNum[i]);
        imapData._net->getStreamPtr()->print(ESP32_MAIL_STR_147);
        imapData._net->getStreamPtr()->print(partID);
        imapData._net->getStreamPtr()->println(ESP32_MAIL_STR_148);

        memset(_part, 0, bufSize);
        memset(_val, 0, bufSize);
        itoa(partID, _val, 10);
        strcpy(_part, _val);
        res = waitIMAPResponse(imapData, IMAP_COMMAND_TYPE::FETCH_BODY_MIME, 0, mailIndex, messageDataIndex, _part);
        if (res)
        {

          if (imapData._messageDataInfo[mailIndex].size() < messageDataIndex + 1)
          {
            messageBodyData b;
            imapData._messageDataInfo[mailIndex].push_back(b);
            b.empty();
            imapData._messageDataCount[mailIndex] = imapData._messageDataInfo[mailIndex].size();
          }

          if (imapData._messageDataInfo[mailIndex][messageDataIndex]._contentType == "")
            continue;

          if (imapData._messageDataInfo[mailIndex][messageDataIndex]._contentType.find(ESP32_MAIL_STR_149) != std::string::npos)
          {
            do
            {

              if (imapData._uidSearch || imapData._fetchUID.length() > 0)
                imapData._net->getStreamPtr()->print(ESP32_MAIL_STR_142);
              else
                imapData._net->getStreamPtr()->print(ESP32_MAIL_STR_143);

              imapData._net->getStreamPtr()->print(imapData._msgNum[i]);
              imapData._net->getStreamPtr()->print(ESP32_MAIL_STR_147);
              imapData._net->getStreamPtr()->print(partID);
              imapData._net->getStreamPtr()->print(".");
              imapData._net->getStreamPtr()->print(_partID);
              imapData._net->getStreamPtr()->println(ESP32_MAIL_STR_148);

              memset(_part, 0, bufSize);
              memset(_val, 0, bufSize);
              itoa(partID, _val, 10);
              strcpy(_part, _val);
              strcat(_part, ".");
              memset(_val, 0, bufSize);
              itoa(_partID, _val, 10);
              strcat(_part, _val);
              _res = waitIMAPResponse(imapData, IMAP_COMMAND_TYPE::FETCH_BODY_MIME, 0, mailIndex, messageDataIndex, _part);

              if (_res)
              {
                messageDataIndex++;
                _partID++;
              }

            } while (_res);
          }
          else
          {
            messageDataIndex++;
          }
          partID++;
        }

      } while (res);

      if (imapData._saveHTMLMsg || imapData._saveTextMsg || imapData._downloadAttachment)
      {

        if (!_sdOk)
        {
          if (imapData._storageType == MailClientStorageType::SD)
          {
            _sdOk = sdTest();
            if (_sdOk)
              if (!SD.exists(imapData._savePath.c_str()))
                createDirs(imapData._savePath);
          }
          else if (imapData._storageType == MailClientStorageType::SPIFFS)
            _sdOk = SPIFFS.begin(true);
        }
      }

      if (imapData._messageDataInfo[mailIndex].size() > 0)
      {
        if (imapData._attachmentCount[mailIndex] > 0 && imapData._readCallback)
        {
          imapData._cbData._info = ESP32_MAIL_STR_78;
          memset(_val, 0, bufSize);
          itoa(imapData._attachmentCount[mailIndex], _val, 10);
          imapData._cbData._info += _val;
          imapData._cbData._info += ESP32_MAIL_STR_79;
          imapData._cbData._status = "";
          imapData._cbData._success = false;
          imapData._readCallback(imapData._cbData);

          for (int j = 0; j < imapData._messageDataInfo[mailIndex].size(); j++)
          {
            if (imapData._messageDataInfo[mailIndex][j]._disposition == ESP32_MAIL_STR_153)
            {
              imapData._cbData._info = imapData._messageDataInfo[mailIndex][j]._filename;
              imapData._cbData._info += ESP32_MAIL_STR_83;
              memset(_val, 0, bufSize);
              itoa(imapData._messageDataInfo[mailIndex][j]._size, _val, 10);
              imapData._cbData._info += _val;
              imapData._cbData._info += ESP32_MAIL_STR_82;
              imapData._cbData._status = "";
              imapData._cbData._success = false;
              imapData._readCallback(imapData._cbData);
            }
          }

          if (imapData._downloadAttachment && _sdOk)
          {
            imapData._cbData._info = ESP32_MAIL_STR_80;
            imapData._cbData._status = ESP32_MAIL_STR_81;
            imapData._cbData._success = false;
            imapData._readCallback(imapData._cbData);
          }
        }

        for (int j = 0; j < imapData._messageDataInfo[mailIndex].size(); j++)
        {

          if (imapData._messageDataInfo[mailIndex][j]._disposition == "")
          {

            if (!imapData._textFormat && imapData._messageDataInfo[mailIndex][j]._contentType != ESP32_MAIL_STR_154)
              continue;

            if (!imapData._htmlFormat && imapData._messageDataInfo[mailIndex][j]._contentType != ESP32_MAIL_STR_155)
              continue;

            if (imapData._uidSearch || imapData._fetchUID.length() > 0)
              imapData._net->getStreamPtr()->print(ESP32_MAIL_STR_142);
            else
              imapData._net->getStreamPtr()->print(ESP32_MAIL_STR_143);

            imapData._net->getStreamPtr()->print(imapData._msgNum[i]);
            imapData._net->getStreamPtr()->print(ESP32_MAIL_STR_147);
            imapData._net->getStreamPtr()->print(imapData._messageDataInfo[mailIndex][j]._part.c_str());
            imapData._net->getStreamPtr()->println(ESP32_MAIL_STR_156);
            if (!waitIMAPResponse(imapData, IMAP_COMMAND_TYPE::FETCH_BODY_TEXT, imapData._message_buffer_size, mailIndex, j))
            {
              _imapStatus = IMAP_STATUS_IMAP_RESPONSE_FAILED;
              if (imapData._readCallback)
              {
                imapData._cbData._info = ESP32_MAIL_STR_53 + imapErrorReasonStr();
                imapData._cbData._status = ESP32_MAIL_STR_52;
                imapData._cbData._success = false;
                imapData._readCallback(imapData._cbData);
              }
              if (imapData._debug)
              {
                ESP32MailDebugError();
                ESP32MailDebugLine(imapErrorReasonStr().c_str(), true);
              }
            }
          }
          else if (imapData._messageDataInfo[mailIndex][j]._disposition == ESP32_MAIL_STR_153 && _sdOk)
          {

            if (imapData._downloadAttachment)
            {
              if (imapData._messageDataInfo[mailIndex][j]._size <= imapData._attacement_max_size)
              {

                if (_sdOk)
                {

                  if (j < imapData._messageDataInfo[mailIndex].size() - 1)
                    if (imapData._messageDataInfo[mailIndex][j + 1]._size > imapData._attacement_max_size)
                      imapData._downloadedByte[mailIndex] += imapData._messageDataInfo[mailIndex][j + 1]._size;

                  if (imapData._uidSearch || imapData._fetchUID.length() > 0)
                    imapData._net->getStreamPtr()->print(ESP32_MAIL_STR_142);
                  else
                    imapData._net->getStreamPtr()->print(ESP32_MAIL_STR_143);

                  imapData._net->getStreamPtr()->print(imapData._msgNum[i]);
                  imapData._net->getStreamPtr()->print(ESP32_MAIL_STR_147);
                  imapData._net->getStreamPtr()->print(imapData._messageDataInfo[mailIndex][j]._part.c_str());
                  imapData._net->getStreamPtr()->println(ESP32_MAIL_STR_156);
                  if (!waitIMAPResponse(imapData, IMAP_COMMAND_TYPE::FETCH_BODY_ATTACHMENT, imapData._message_buffer_size, mailIndex, j))
                  {
                    _imapStatus = IMAP_STATUS_IMAP_RESPONSE_FAILED;
                    if (imapData._readCallback)
                    {
                      imapData._cbData._info = ESP32_MAIL_STR_53 + imapErrorReasonStr();
                      imapData._cbData._status = ESP32_MAIL_STR_52;
                      imapData._cbData._success = false;
                      imapData._readCallback(imapData._cbData);
                    }
                    if (imapData._debug)
                    {
                      ESP32MailDebugError();
                      ESP32MailDebugLine(imapErrorReasonStr().c_str(), true);
                    }
                  }

                  delay(0);
                }
              }
              else
              {
                if (j == imapData._messageDataInfo[mailIndex].size() - 1)
                  imapData._downloadedByte[mailIndex] += imapData._messageDataInfo[mailIndex][j]._size;
              }
            }
          }
        }
      }

      if (imapData._storageType == MailClientStorageType::SD)
      {
        if (_sdOk)
          SD.end();
      }
      else if (imapData._storageType == MailClientStorageType::SPIFFS)
      {
        if (_sdOk)
          SPIFFS.end();
      }

      _sdOk = false;
    }

    if (imapData._readCallback)
    {
      imapData._cbData._info = ESP32_MAIL_STR_84;
      memset(_val, 0, bufSize);
      itoa(ESP.getFreeHeap(), _val, 10);
      imapData._cbData._info += _val;
      imapData._cbData._status = "";
      imapData._cbData._success = false;
      imapData._readCallback(imapData._cbData);
    }

    mailIndex++;
  }

  if (imapData._readCallback)
  {
    imapData._cbData._info = ESP32_MAIL_STR_85;
    imapData._cbData._status = ESP32_MAIL_STR_86;
    imapData._cbData._success = false;
    imapData._readCallback(imapData._cbData);
  }

  if (imapData._debug)
    ESP32MailDebugInfo(ESP32_MAIL_STR_234);

  if (imapData._net->connected())
    while (imapData._net->getStreamPtr()->available())
      imapData._net->getStreamPtr()->read();

  imapData._net->getStreamPtr()->println(ESP32_MAIL_STR_146);

  if (!waitIMAPResponse(imapData, 0))
  {
    _imapStatus = IMAP_STATUS_BAD_COMMAND;
    if (imapData._readCallback)
    {
      imapData._cbData._info = ESP32_MAIL_STR_53 + imapErrorReasonStr();
      imapData._cbData._status = ESP32_MAIL_STR_52;
      imapData._cbData._success = false;
      imapData._readCallback(imapData._cbData);
    }
    if (imapData._debug)
    {
      ESP32MailDebugError();
      ESP32MailDebugLine(imapErrorReasonStr().c_str(), true);
    }
    goto out;
  }

  if (imapData._readCallback)
  {
    imapData._cbData._info = ESP32_MAIL_STR_98;
    imapData._cbData._status = ESP32_MAIL_STR_96;
    imapData._cbData._success = true;
    imapData._readCallback(imapData._cbData);
  }

  if (imapData._debug)
    ESP32MailDebugInfo(ESP32_MAIL_STR_235);

  if (imapData._net->connected())
  {
    while (imapData._net->getStreamPtr()->available())
      imapData._net->getStreamPtr()->read();

    imapData._net->getStreamPtr()->stop();
  }

  imapData._cbData.empty();
  delete[] _val;
  delete[] _part;
  std::string().swap(command);
  std::string().swap(buf);

  return true;

out:

  if (connected)
  {
    if (imapData._net->connected())
    {
      while (imapData._net->getStreamPtr()->available())
        imapData._net->getStreamPtr()->read();
      imapData._net->getStreamPtr()->stop();
    }
  }

  imapData._cbData.empty();
  delete[] _val;
  delete[] _part;
  std::string().swap(command);
  std::string().swap(buf);

  return false;
}

bool ESP32_MailClient::setFlag(IMAPData &imapData, int msgUID, const String &flag)
{
  return _setFlag(imapData, msgUID, flag, 0);
}

bool ESP32_MailClient::addFlag(IMAPData &imapData, int msgUID, const String &flag)
{
  return _setFlag(imapData, msgUID, flag, 1);
}

bool ESP32_MailClient::removeFlag(IMAPData &imapData, int msgUID, const String &flag)
{
  return _setFlag(imapData, msgUID, flag, 2);
}

bool ESP32_MailClient::_setFlag(IMAPData &imapData, int msgUID, const String &flag, uint8_t action)
{

  std::string buf;

  bool starttls = imapData._starttls;
  bool connected = false;

  int bufSize = 50;

  char *_val = new char[bufSize];
  char *_part = new char[bufSize];

  unsigned long dataTime = 0;

  int count = 0;

  imapData._net->setDebugCallback(NULL);

  if (imapData._debug)
  {
    ESP32MailDebugInfo(ESP32_MAIL_STR_225);
    ESP32MailDebug(imapData._host.c_str());
    ESP32MailDebug(String(imapData._port).c_str());
  }

  if (imapData._readCallback)
  {
    imapData._cbData._info = ESP32_MAIL_STR_50;
    imapData._cbData._status = ESP32_MAIL_STR_51;
    imapData._cbData._success = false;
    imapData._readCallback(imapData._cbData);
  }

  if (imapData._debug)
    imapData._net->setDebugCallback(ESP32MailDebug);

  if (imapData._rootCA.size() > 0)
    imapData._net->begin(imapData._host.c_str(), imapData._port, ESP32_MAIL_STR_202, (const char *)imapData._rootCA.front());
  else
    imapData._net->begin(imapData._host.c_str(), imapData._port, ESP32_MAIL_STR_202, (const char *)NULL);

  while (!imapData._net->connected() && count < 10)
  {

    count++;

    if (!imapData._net->connect(starttls))
    {

      _imapStatus = IMAP_STATUS_SERVER_CONNECT_FAILED;

      if (imapData._readCallback)
      {
        imapData._cbData._info = ESP32_MAIL_STR_53 + imapErrorReasonStr();
        imapData._cbData._status = ESP32_MAIL_STR_52;
        imapData._cbData._success = false;
        imapData._readCallback(imapData._cbData);
      }
      if (imapData._debug)
      {
        ESP32MailDebugError();
        ESP32MailDebugLine(imapErrorReasonStr().c_str(), true);
      }
    }
    else
    {
      break;
    }
  }

  if (!imapData._net->connect(starttls))
  {
    goto out;
  }

  connected = true;

  if (imapData._readCallback)
  {
    imapData._cbData._info = ESP32_MAIL_STR_54;
    imapData._cbData._status = ESP32_MAIL_STR_55;
    imapData._cbData._success = false;
    imapData._readCallback(imapData._cbData);
  }

  if (imapData._debug)
    ESP32MailDebugInfo(ESP32_MAIL_STR_228);

  //Don't expect handshake from some servers
  dataTime = millis();

  while (imapData._net->connected() && !imapData._net->getStreamPtr()->available() && millis() - 500 < dataTime)
    delay(0);

  if (imapData._net->connected() && imapData._net->getStreamPtr()->available())
    while (imapData._net->getStreamPtr()->available())
      imapData._net->getStreamPtr()->read();

  imapData.clearMessageData();

  if (imapData._readCallback)
  {
    imapData._cbData._info = ESP32_MAIL_STR_56;
    imapData._cbData._status = ESP32_MAIL_STR_57;
    imapData._cbData._success = false;
    imapData._readCallback(imapData._cbData);
  }

  if (imapData._debug)
    ESP32MailDebugInfo(ESP32_MAIL_STR_229);

  imapData._net->getStreamPtr()->print(ESP32_MAIL_STR_130);
  imapData._net->getStreamPtr()->print(imapData._loginEmail.c_str());
  imapData._net->getStreamPtr()->print(ESP32_MAIL_STR_131);
  imapData._net->getStreamPtr()->println(imapData._loginPassword.c_str());

  if (!waitIMAPResponse(imapData, IMAP_COMMAND_TYPE::LOGIN))
  {
    _imapStatus = IMAP_STATUS_LOGIN_FAILED;
    if (imapData._readCallback)
    {
      imapData._cbData._info = ESP32_MAIL_STR_53 + imapErrorReasonStr();
      imapData._cbData._status = ESP32_MAIL_STR_52;
      imapData._cbData._success = false;
      imapData._readCallback(imapData._cbData);
    }
    if (imapData._debug)
    {
      ESP32MailDebugError();
      ESP32MailDebugLine(imapErrorReasonStr().c_str(), true);
    }
    goto out;
  }

  if (imapData._readCallback)
  {
    imapData._cbData._info = ESP32_MAIL_STR_58;
    imapData._cbData._status = ESP32_MAIL_STR_59;
    imapData._cbData._success = false;
    imapData._readCallback(imapData._cbData);
  }

  if (imapData._debug)
    ESP32MailDebugInfo(ESP32_MAIL_STR_230);

  imapData._net->getStreamPtr()->println(ESP32_MAIL_STR_133);
  if (!waitIMAPResponse(imapData, IMAP_COMMAND_TYPE::LIST))
  {
    _imapStatus = IMAP_STATUS_BAD_COMMAND;
    if (imapData._readCallback)
    {
      imapData._cbData._info = ESP32_MAIL_STR_53 + imapErrorReasonStr();
      imapData._cbData._status = ESP32_MAIL_STR_52;
      imapData._cbData._success = false;
      imapData._readCallback(imapData._cbData);
    }
    if (imapData._debug)
    {
      ESP32MailDebugError();
      ESP32MailDebugLine(imapErrorReasonStr().c_str(), true);
    }
    imapData._cbData.empty();
  }

  if (imapData._readCallback)
  {

    imapData._cbData._info = ESP32_MAIL_STR_60;
    imapData._cbData._success = false;
    imapData._readCallback(imapData._cbData);

    for (size_t i = 0; i < imapData._folders.size(); i++)
    {
      imapData._cbData._info = imapData._folders[i];
      imapData._cbData._status = "";
      imapData._cbData._success = false;
      imapData._readCallback(imapData._cbData);
    }

    imapData._cbData._info = ESP32_MAIL_STR_61 + imapData._currentFolder + ESP32_MAIL_STR_97;
    imapData._cbData._status = "";
    imapData._cbData._success = false;
    imapData._readCallback(imapData._cbData);
  }

  if (imapData._debug)
    ESP32MailDebugInfo(ESP32_MAIL_STR_248);

  imapData._net->getStreamPtr()->print(ESP32_MAIL_STR_247);
  imapData._net->getStreamPtr()->print(imapData._currentFolder.c_str());
  imapData._net->getStreamPtr()->println(ESP32_MAIL_STR_136);
  if (!waitIMAPResponse(imapData, IMAP_COMMAND_TYPE::EXAMINE))
  {
    _imapStatus = IMAP_STATUS_BAD_COMMAND;
    if (imapData._readCallback)
    {
      imapData._cbData._info = ESP32_MAIL_STR_53 + imapErrorReasonStr();
      imapData._cbData._status = ESP32_MAIL_STR_52;
      imapData._cbData._success = false;
      imapData._readCallback(imapData._cbData);
    }
    if (imapData._debug)
    {
      ESP32MailDebugError();
      ESP32MailDebugLine(imapErrorReasonStr().c_str(), true);
    }
    goto out;
  }

  if (imapData._debug)
  {
    if (action == 0)
      ESP32MailDebugInfo(ESP32_MAIL_STR_253);
    else if (action == 1)
      ESP32MailDebugInfo(ESP32_MAIL_STR_254);
    else
      ESP32MailDebugInfo(ESP32_MAIL_STR_255);
  }

  imapData._net->getStreamPtr()->print(ESP32_MAIL_STR_249);
  imapData._net->getStreamPtr()->print(msgUID);
  if (action == 0)
    imapData._net->getStreamPtr()->print(ESP32_MAIL_STR_250);
  else if (action == 1)
    imapData._net->getStreamPtr()->print(ESP32_MAIL_STR_251);
  else
    imapData._net->getStreamPtr()->print(ESP32_MAIL_STR_252);
  imapData._net->getStreamPtr()->print(flag);
  imapData._net->getStreamPtr()->println(ESP32_MAIL_STR_192);

  if (!getIMAPResponse(imapData))
  {
    _imapStatus = IMAP_STATUS_PARSE_FLAG_FAILED;
    if (imapData._readCallback)
    {
      imapData._cbData._info = ESP32_MAIL_STR_53 + imapErrorReasonStr();
      imapData._cbData._status = ESP32_MAIL_STR_52;
      imapData._cbData._success = false;
      imapData._readCallback(imapData._cbData);
    }
    if (imapData._debug)
    {
      ESP32MailDebugError();
      ESP32MailDebugLine(imapErrorReasonStr().c_str(), true);
    }
    goto out;
  }

  if (imapData._readCallback)
  {
    imapData._cbData._info = ESP32_MAIL_STR_85;
    imapData._cbData._status = ESP32_MAIL_STR_86;
    imapData._cbData._success = false;
    imapData._readCallback(imapData._cbData);
  }

  if (imapData._debug)
    ESP32MailDebugInfo(ESP32_MAIL_STR_234);

  if (imapData._net->connected())
    while (imapData._net->getStreamPtr()->available())
      imapData._net->getStreamPtr()->read();

  imapData._net->getStreamPtr()->println(ESP32_MAIL_STR_146);

  if (!waitIMAPResponse(imapData, 0))
  {
    _imapStatus = IMAP_STATUS_BAD_COMMAND;
    if (imapData._readCallback)
    {
      imapData._cbData._info = ESP32_MAIL_STR_53 + imapErrorReasonStr();
      imapData._cbData._status = ESP32_MAIL_STR_52;
      imapData._cbData._success = false;
      imapData._readCallback(imapData._cbData);
    }
    if (imapData._debug)
    {
      ESP32MailDebugError();
      ESP32MailDebugLine(imapErrorReasonStr().c_str(), true);
    }
    goto out;
  }

  if (imapData._readCallback)
  {
    imapData._cbData._info = ESP32_MAIL_STR_98;
    imapData._cbData._status = ESP32_MAIL_STR_96;
    imapData._cbData._success = true;
    imapData._readCallback(imapData._cbData);
  }

  if (imapData._debug)
    ESP32MailDebugInfo(ESP32_MAIL_STR_235);

  if (imapData._net->connected())
  {
    while (imapData._net->getStreamPtr()->available())
      imapData._net->getStreamPtr()->read();

    imapData._net->getStreamPtr()->stop();
  }

  imapData._cbData.empty();
  delete[] _val;
  delete[] _part;
  std::string().swap(buf);

  return true;

out:

  if (connected)
  {
    if (imapData._net->connected())
    {
      while (imapData._net->getStreamPtr()->available())
        imapData._net->getStreamPtr()->read();
      imapData._net->getStreamPtr()->stop();
    }
  }

  imapData._cbData.empty();
  delete[] _val;
  delete[] _part;
  std::string().swap(buf);
  return false;
}


bool ESP32_MailClient::smtpClientAvailable(SMTPData &smtpData, bool available)
{

  if (!smtpData._net->getStreamPtr())
    return false;

  if (available)
    return smtpData._net->getStreamPtr()->connected() && smtpData._net->getStreamPtr()->available();
  else
    return smtpData._net->getStreamPtr()->connected() && !smtpData._net->getStreamPtr()->available();
}

bool ESP32_MailClient::imapClientAvailable(IMAPData &imapData, bool available)
{

  if (!imapData._net->getStreamPtr())
    return false;

  if (available)
    return imapData._net->getStreamPtr()->connected() && imapData._net->getStreamPtr()->available();
  else
    return imapData._net->getStreamPtr()->connected() && !imapData._net->getStreamPtr()->available();
}

void ESP32_MailClient::createDirs(std::string dirs)
{
  std::string dir = "";
  int count = 0;
  for (int i = 0; i < dirs.length(); i++)
  {
    dir.append(1, dirs[i]);
    count++;
    if (dirs[i] == '/')
    {
      if (dir.length() > 0)
        SD.mkdir(dir.substr(0, dir.length() - 1).c_str());
      count = 0;
    }
  }
  if (count > 0)
    SD.mkdir(dir.c_str());
  std::string().swap(dir);
}

bool ESP32_MailClient::sdTest()
{

  if (_sdConfigSet)
    sdBegin(_sck, _miso, _mosi, _ss);
  else
    sdBegin();

  File file = SD.open(ESP32_MAIL_STR_204, FILE_WRITE);
  if (!file)
    return false;

  if (!file.write(32))
    return false;
  file.close();

  file = SD.open(ESP32_MAIL_STR_204);
  if (!file)
    return false;

  while (file.available())
  {
    if (file.read() != 32)
      return false;
  }
  file.close();

  SD.remove(ESP32_MAIL_STR_204);

  return true;
}

bool ESP32_MailClient::sendMail(SMTPData &smtpData)
{

  _smtpStatus = 0;
  std::string buf;
  std::string buf2;
  int bufSize = 50;
  bool starttls = smtpData._starttls;
  bool connected = false;
  char *_val = new char[bufSize];
  int res = 0;

  smtpData._net->setDebugCallback(NULL);

  if (smtpData._sendCallback)
  {
    smtpData._cbData._info = ESP32_MAIL_STR_120;
    smtpData._cbData._success = false;
    smtpData._sendCallback(smtpData._cbData);
  }

  if (smtpData._debug)
  {
    ESP32MailDebugInfo(ESP32_MAIL_STR_236);
    ESP32MailDebug(smtpData._host.c_str());
    ESP32MailDebug(String(smtpData._port).c_str());
  }


  if (smtpData._debug)
    smtpData._net->setDebugCallback(ESP32MailDebug);

  if (smtpData._rootCA.size() > 0)
    smtpData._net->begin(smtpData._host.c_str(), smtpData._port, ESP32_MAIL_STR_202, (const char *)smtpData._rootCA.front());
  else
    smtpData._net->begin(smtpData._host.c_str(), smtpData._port, ESP32_MAIL_STR_202, (const char *)NULL);

  if (smtpData._port == 587)
    starttls = true;

  if (!smtpData._net->connect(starttls))
  {
    _smtpStatus = SMTP_STATUS_SERVER_CONNECT_FAILED;
    if (smtpData._sendCallback)
    {
      smtpData._cbData._info = ESP32_MAIL_STR_53 + smtpErrorReasonStr();
      smtpData._cbData._success = false;
      smtpData._sendCallback(smtpData._cbData);
    }

    if (smtpData._debug)
    {
      ESP32MailDebugError();
      ESP32MailDebugLine(smtpErrorReasonStr().c_str(), true);
    }
    goto failed;
  }

  if (smtpData._debug)
    ESP32MailDebugInfo(ESP32_MAIL_STR_238);

  connected = true;

  if (!starttls)
  {

    if (smtpData._sendCallback)
    {
      smtpData._cbData._info = ESP32_MAIL_STR_121;
      smtpData._cbData._success = false;
      smtpData._sendCallback(smtpData._cbData);
    }

    if (waitSMTPResponse(smtpData) != 220)
    {
      _smtpStatus = SMTP_STATUS_SMTP_RESPONSE_FAILED;
      if (smtpData._sendCallback)
      {
        smtpData._cbData._info = ESP32_MAIL_STR_53 + smtpErrorReasonStr();
        smtpData._cbData._success = false;
        smtpData._sendCallback(smtpData._cbData);
      }
      if (smtpData._debug)
      {
        ESP32MailDebugError();
        ESP32MailDebugLine(smtpErrorReasonStr().c_str(), true);
      }
      goto failed;
    }
  }

  if (smtpData._sendCallback)
  {
    smtpData._cbData._info = ESP32_MAIL_STR_122;
    smtpData._cbData._success = false;
    smtpData._sendCallback(smtpData._cbData);
  }

  if (smtpData._debug)
    ESP32MailDebugInfo(ESP32_MAIL_STR_239);

  smtpData._net->getStreamPtr()->println(ESP32_MAIL_STR_5);

  if (waitSMTPResponse(smtpData) != 250)
  {
    _smtpStatus = SMTP_STATUS_IDENTIFICATION_FAILED;
    if (smtpData._sendCallback)
    {
      smtpData._cbData._info = ESP32_MAIL_STR_53 + smtpErrorReasonStr();
      smtpData._cbData._success = false;
      smtpData._sendCallback(smtpData._cbData);
    }
    if (smtpData._debug)
    {
      ESP32MailDebugError();
      ESP32MailDebugLine(smtpErrorReasonStr().c_str(), true);
    }
    goto failed;
  }

  if (smtpData._sendCallback)
  {
    smtpData._cbData._info = ESP32_MAIL_STR_123;
    smtpData._cbData._success = false;
    smtpData._sendCallback(smtpData._cbData);
  }

  if (smtpData._debug)
    ESP32MailDebugInfo(ESP32_MAIL_STR_240);

  smtpData._net->getStreamPtr()->println(ESP32_MAIL_STR_4);

  if (waitSMTPResponse(smtpData) != 334)
  {
    _smtpStatus = SMTP_STATUS_AUTHEN_FAILED;
    if (smtpData._sendCallback)
    {
      smtpData._cbData._info = ESP32_MAIL_STR_53 + smtpErrorReasonStr();
      smtpData._cbData._success = false;
      smtpData._sendCallback(smtpData._cbData);
    }
    if (smtpData._debug)
    {
      ESP32MailDebugError();
      ESP32MailDebugLine(smtpErrorReasonStr().c_str(), true);
    }
    goto failed;
  }

  if (smtpData._sendCallback)
  {
    smtpData._cbData._info = ESP32_MAIL_STR_124;
    smtpData._cbData._success = false;
    smtpData._sendCallback(smtpData._cbData);
  }
  if (smtpData._debug)
    ESP32MailDebugInfo(ESP32_MAIL_STR_241);

  smtpData._net->getStreamPtr()->println(base64_encode_string((const unsigned char *)smtpData._loginEmail.c_str(), smtpData._loginEmail.length()).c_str());

  if (waitSMTPResponse(smtpData) != 334)
  {
    _smtpStatus = SMTP_STATUS_USER_LOGIN_FAILED;
    if (smtpData._sendCallback)
    {
      smtpData._cbData._info = ESP32_MAIL_STR_53 + smtpErrorReasonStr();
      smtpData._cbData._success = false;
      smtpData._sendCallback(smtpData._cbData);
    }
    if (smtpData._debug)
    {
      ESP32MailDebugError();
      ESP32MailDebugLine(smtpErrorReasonStr().c_str(), true);
    }
    goto failed;
  }

  smtpData._net->getStreamPtr()->println(base64_encode_string((const unsigned char *)smtpData._loginPassword.c_str(), smtpData._loginPassword.length()).c_str());

  if (waitSMTPResponse(smtpData) != 235)
  {
    _smtpStatus = SMTP_STATUS_PASSWORD_LOGIN_FAILED;
    if (smtpData._sendCallback)
    {
      smtpData._cbData._info = ESP32_MAIL_STR_53 + smtpErrorReasonStr();
      smtpData._cbData._success = false;
      smtpData._sendCallback(smtpData._cbData);
    }
    if (smtpData._debug)
    {
      ESP32MailDebugError();
      ESP32MailDebugLine(smtpErrorReasonStr().c_str(), true);
    }
    goto failed;
  }

  if (smtpData._sendCallback)
  {
    smtpData._cbData._info = ESP32_MAIL_STR_125;
    smtpData._cbData._success = false;
    smtpData._sendCallback(smtpData._cbData);
  }

  if (smtpData._debug)
    ESP32MailDebugInfo(ESP32_MAIL_STR_242);

  if (smtpData._priority > 0 && smtpData._priority <= 5)
  {
    memset(_val, 0, bufSize);
    itoa(smtpData._priority, _val, 10);

    buf2 += ESP32_MAIL_STR_17;
    buf2 += _val;
    buf2 += ESP32_MAIL_STR_34;

    if (smtpData._priority == 1)
    {
      buf2 += ESP32_MAIL_STR_18;
      buf2 += ESP32_MAIL_STR_21;
    }
    else if (smtpData._priority == 3)
    {
      buf2 += ESP32_MAIL_STR_19;
      buf2 += ESP32_MAIL_STR_22;
    }
    else if (smtpData._priority == 5)
    {
      buf2 += ESP32_MAIL_STR_20;
      buf2 += ESP32_MAIL_STR_23;
    }
  }

  buf2 += ESP32_MAIL_STR_10;

  if (smtpData._fromName.length() > 0)
    buf2 += smtpData._fromName;

  buf2 += ESP32_MAIL_STR_14;
  buf2 += smtpData._senderEmail;
  buf2 += ESP32_MAIL_STR_15;
  buf2 += ESP32_MAIL_STR_34;

  buf += ESP32_MAIL_STR_8;
  buf += ESP32_MAIL_STR_14;
  buf += smtpData._senderEmail;
  buf += ESP32_MAIL_STR_15;
  smtpData._net->getStreamPtr()->println(buf.c_str());

  if (waitSMTPResponse(smtpData) != 250)
  {
    _smtpStatus = SMTP_STATUS_SEND_HEADER_SENDER_FAILED;
    if (smtpData._sendCallback)
    {
      smtpData._cbData._info = ESP32_MAIL_STR_53 + smtpErrorReasonStr();
      smtpData._cbData._success = false;
      smtpData._sendCallback(smtpData._cbData);
    }
    if (smtpData._debug)
    {
      ESP32MailDebugError();
      ESP32MailDebugLine(smtpErrorReasonStr().c_str(), true);
    }
    goto failed;
  }

  for (uint8_t i = 0; i < smtpData._recipient.size(); i++)
  {
    if (i == 0)
    {
      buf2 += ESP32_MAIL_STR_11;
      buf2 += ESP32_MAIL_STR_14;
      buf2 += smtpData._recipient[i];
      buf2 += ESP32_MAIL_STR_15;
    }
    else
    {
      buf2 += ESP32_MAIL_STR_13;
      buf2 += smtpData._recipient[i];
      buf2 += ESP32_MAIL_STR_15;
    }

    if (i == smtpData._recipient.size() - 1)
      buf2 += ESP32_MAIL_STR_34;

    buf.clear();

    buf += ESP32_MAIL_STR_9;
    buf += ESP32_MAIL_STR_14;
    buf += smtpData._recipient[i];
    buf += ESP32_MAIL_STR_15;

    smtpData._net->getStreamPtr()->println(buf.c_str());

    if (waitSMTPResponse(smtpData) != 250)
    {
      _smtpStatus = SMTP_STATUS_SEND_HEADER_RECIPIENT_FAILED;
      if (smtpData._sendCallback)
      {
        smtpData._cbData._info = ESP32_MAIL_STR_53 + smtpErrorReasonStr();
        smtpData._cbData._success = false;
        smtpData._sendCallback(smtpData._cbData);
      }
      if (smtpData._debug)
      {
        ESP32MailDebugError();
        ESP32MailDebugLine(smtpErrorReasonStr().c_str(), true);
      }
      goto failed;
    }
  }

  for (uint8_t i = 0; i < smtpData._cc.size(); i++)
  {

    if (i == 0)
    {
      buf2 += ESP32_MAIL_STR_12;
      buf2 += ESP32_MAIL_STR_14;
      buf2 += smtpData._cc[i];
      buf2 += ESP32_MAIL_STR_15;
    }
    else
    {
      buf2 += ESP32_MAIL_STR_13;
      buf2 += smtpData._cc[i];
      buf2 += ESP32_MAIL_STR_15;
    }

    if (i == smtpData.ccCount() - 1)
      buf2 += ESP32_MAIL_STR_34;

    buf.clear();

    buf += ESP32_MAIL_STR_9;
    buf += ESP32_MAIL_STR_14;
    buf += smtpData._cc[i];
    buf += ESP32_MAIL_STR_15;
    smtpData._net->getStreamPtr()->println(buf.c_str());

    if (waitSMTPResponse(smtpData) != 250)
    {
      _smtpStatus = SMTP_STATUS_SEND_HEADER_RECIPIENT_FAILED;
      if (smtpData._sendCallback)
      {
        smtpData._cbData._info = ESP32_MAIL_STR_53 + smtpErrorReasonStr();
        smtpData._cbData._success = false;
        smtpData._sendCallback(smtpData._cbData);
      }
      if (smtpData._debug)
      {
        ESP32MailDebugError();
        ESP32MailDebugLine(smtpErrorReasonStr().c_str(), true);
      }
      goto failed;
    }
  }

  for (uint8_t i = 0; i < smtpData._bcc.size(); i++)
  {
    buf.clear();
    buf += ESP32_MAIL_STR_9;
    buf += ESP32_MAIL_STR_14;
    buf += smtpData._bcc[i];
    buf += ESP32_MAIL_STR_15;
    smtpData._net->getStreamPtr()->println(buf.c_str());

    if (waitSMTPResponse(smtpData) != 250)
    {
      _smtpStatus = SMTP_STATUS_SEND_HEADER_RECIPIENT_FAILED;
      if (smtpData._sendCallback)
      {
        smtpData._cbData._info = ESP32_MAIL_STR_53 + smtpErrorReasonStr();
        smtpData._cbData._success = false;
        smtpData._sendCallback(smtpData._cbData);
      }
      if (smtpData._debug)
      {
        ESP32MailDebugError();
        ESP32MailDebugLine(smtpErrorReasonStr().c_str(), true);
      }
      goto failed;
    }
  }

  if (smtpData._sendCallback)
  {
    smtpData._cbData._info = ESP32_MAIL_STR_126;
    smtpData._cbData._success = false;
    smtpData._sendCallback(smtpData._cbData);
  }

  if (smtpData._debug)
    ESP32MailDebugInfo(ESP32_MAIL_STR_243);

  smtpData._net->getStreamPtr()->println(ESP32_MAIL_STR_16);

  if (waitSMTPResponse(smtpData) != 354)
  {
    _smtpStatus = SMTP_STATUS_SEND_BODY_FAILED;
    if (smtpData._sendCallback)
    {
      smtpData._cbData._info = ESP32_MAIL_STR_53 + smtpErrorReasonStr();
      smtpData._cbData._success = false;
      smtpData._sendCallback(smtpData._cbData);
    }
    if (smtpData._debug)
    {
      ESP32MailDebugError();
      ESP32MailDebugLine(smtpErrorReasonStr().c_str(), true);
    }
    goto failed;
  }

  smtpData._net->getStreamPtr()->print(buf2.c_str());

  smtpData._net->getStreamPtr()->print(ESP32_MAIL_STR_24);
  smtpData._net->getStreamPtr()->println(smtpData._subject.c_str());

  if (smtpData._customMessageHeader.size() > 0)
    for (uint8_t k = 0; k < smtpData._customMessageHeader.size(); k++)
      smtpData._net->getStreamPtr()->println(smtpData._customMessageHeader[k].c_str());

  smtpData._net->getStreamPtr()->print(ESP32_MAIL_STR_3);
  smtpData._net->getStreamPtr()->print(ESP32_MAIL_STR_1);
  smtpData._net->getStreamPtr()->print(ESP32_MAIL_STR_2);
  smtpData._net->getStreamPtr()->print(ESP32_MAIL_STR_35);

  buf.clear();

  set_message_header(buf, smtpData._message, smtpData._htmlFormat);

  smtpData._net->getStreamPtr()->print(buf.c_str());

  if (smtpData._attach._index > 0)
  {
    smtpData._cbData._info = ESP32_MAIL_STR_127;
    smtpData._cbData._success = false;
    if (smtpData._sendCallback)
      smtpData._sendCallback(smtpData._cbData);
    if (smtpData._debug)
      ESP32MailDebugInfo(ESP32_MAIL_STR_244);
  }

  for (uint8_t i = 0; i < smtpData._attach._index; i++)
  {
    if (smtpData._attach._type[i] == 0)
    {

      smtpData._cbData._info = smtpData._attach._filename[i];
      smtpData._cbData._success = false;
      if (smtpData._sendCallback)
        smtpData._sendCallback(smtpData._cbData);
      if (smtpData._debug)
        ESP32MailDebug(smtpData._attach._filename[i].c_str());

      buf.clear();
      set_attachment_header(i, buf, smtpData._attach);
      smtpData._net->getStreamPtr()->print(buf.c_str());
      send_base64_encode_mime_data(smtpData._net->getStreamPtr(), smtpData._attach._buf[i].front(), smtpData._attach._size[i]);
      smtpData._net->getStreamPtr()->print(ESP32_MAIL_STR_34);
    }
    else
    {

      if (!_sdOk)
      {
        if (smtpData._storageType == MailClientStorageType::SD)
          _sdOk = sdTest();
        else if (smtpData._storageType == MailClientStorageType::SPIFFS)
          _sdOk = SPIFFS.begin(true);
      }

      if (!_sdOk)
        continue;

      bool file_existed = false;
      if (smtpData._storageType == MailClientStorageType::SD)
        file_existed = SD.exists(smtpData._attach._filename[i].c_str());
      else if (smtpData._storageType == MailClientStorageType::SPIFFS)
        file_existed = SPIFFS.exists(smtpData._attach._filename[i].c_str());

      if (file_existed)
      {
        smtpData._cbData._info = smtpData._attach._filename[i];
        smtpData._cbData._success = false;
        if (smtpData._sendCallback)
          smtpData._sendCallback(smtpData._cbData);

        if (smtpData._debug)
          ESP32MailDebug(smtpData._attach._filename[i].c_str());

        buf.clear();
        set_attachment_header(i, buf, smtpData._attach);
        smtpData._net->getStreamPtr()->print(buf.c_str());

        File file;
        if (smtpData._storageType == MailClientStorageType::SD)
          file = SD.open(smtpData._attach._filename[i].c_str(), FILE_READ);
        else if (smtpData._storageType == MailClientStorageType::SPIFFS)
          file = SPIFFS.open(smtpData._attach._filename[i].c_str(), FILE_READ);

        send_base64_encode_mime_file(smtpData._net->getStreamPtr(), file);
        smtpData._net->getStreamPtr()->print(ESP32_MAIL_STR_34);
      }
    }
  }

  if (smtpData._debug)
    ESP32MailDebugInfo(ESP32_MAIL_STR_245);

  smtpData._net->getStreamPtr()->print(ESP32_MAIL_STR_33);
  smtpData._net->getStreamPtr()->print(ESP32_MAIL_STR_2);
  smtpData._net->getStreamPtr()->print(ESP32_MAIL_STR_33);
  smtpData._net->getStreamPtr()->print(ESP32_MAIL_STR_37);

  if (smtpData._sendCallback)
  {
    smtpData._cbData._info = ESP32_MAIL_STR_128;
    smtpData._cbData._success = false;
    smtpData._sendCallback(smtpData._cbData);
  }

  res = waitSMTPResponse(smtpData);

  if (res != 250 && res != -1000)
  {
    _smtpStatus = SMTP_STATUS_SEND_BODY_FAILED;
    if (smtpData._sendCallback)
    {
      smtpData._cbData._info = ESP32_MAIL_STR_53 + smtpErrorReasonStr();
      smtpData._cbData._success = false;
      smtpData._sendCallback(smtpData._cbData);
    }
    if (smtpData._debug)
    {
      ESP32MailDebugError();
      ESP32MailDebugLine(smtpErrorReasonStr().c_str(), true);
    }
    goto failed;
  }

  if (smtpData._sendCallback)
  {
    smtpData._cbData._info = ESP32_MAIL_STR_129;
    smtpData._cbData._success = true;
    smtpData._sendCallback(smtpData._cbData);
  }

  if (smtpData._debug)
    ESP32MailDebugInfo(ESP32_MAIL_STR_246);

  if (smtpData._net->connected())
    smtpData._net->getStreamPtr()->stop();

  smtpData._cbData.empty();

  std::string().swap(buf);
  std::string().swap(buf2);
  delete[] _val;

  return true;

failed:

  if (connected)
  {
    if (smtpData._net->connected())
      smtpData._net->getStreamPtr()->stop();
  }

  smtpData._cbData.empty();
  std::string().swap(buf);
  std::string().swap(buf2);
  delete[] _val;
  return false;
}

String ESP32_MailClient::smtpErrorReason()
{
  return smtpErrorReasonStr().c_str();
}

std::string ESP32_MailClient::smtpErrorReasonStr()
{
  std::string res = "";
  switch (_smtpStatus)
  {
  case SMTP_STATUS_SERVER_CONNECT_FAILED:
    res = ESP32_MAIL_STR_38;
    break;
  case SMTP_STATUS_SMTP_RESPONSE_FAILED:
    res = ESP32_MAIL_STR_39;
    break;
  case SMTP_STATUS_IDENTIFICATION_FAILED:
    res = ESP32_MAIL_STR_41;
    break;
  case SMTP_STATUS_AUTHEN_NOT_SUPPORT:
    res = ESP32_MAIL_STR_42;
    break;
  case SMTP_STATUS_AUTHEN_FAILED:
    res = ESP32_MAIL_STR_43;
    break;
  case SMTP_STATUS_USER_LOGIN_FAILED:
    res = ESP32_MAIL_STR_44;
    break;
  case SMTP_STATUS_PASSWORD_LOGIN_FAILED:
    res = ESP32_MAIL_STR_47;
    break;
  case SMTP_STATUS_SEND_HEADER_SENDER_FAILED:
    res = ESP32_MAIL_STR_48;
    break;
  case SMTP_STATUS_SEND_HEADER_RECIPIENT_FAILED:
    res = ESP32_MAIL_STR_222;
    break;
  case SMTP_STATUS_SEND_BODY_FAILED:
    res = ESP32_MAIL_STR_49;
    break;
  case MAIL_CLIENT_STATUS_WIFI_CONNECT_FAIL:
    res = ESP32_MAIL_STR_221;
    break;
  default:
    res = "";
  }
  return res;
}

String ESP32_MailClient::imapErrorReason()
{
  std::string res = "";
  switch (_imapStatus)
  {
  case IMAP_STATUS_SERVER_CONNECT_FAILED:
    res = ESP32_MAIL_STR_38;
    break;
  case IMAP_STATUS_IMAP_RESPONSE_FAILED:
    res = ESP32_MAIL_STR_40;
    break;
  case IMAP_STATUS_LOGIN_FAILED:
    res = ESP32_MAIL_STR_45;
    break;
  case IMAP_STATUS_BAD_COMMAND:
    res = ESP32_MAIL_STR_46;
    break;
  case IMAP_STATUS_PARSE_FLAG_FAILED:
    res = ESP32_MAIL_STR_256;
    break;
  case MAIL_CLIENT_STATUS_WIFI_CONNECT_FAIL:
    res = ESP32_MAIL_STR_221;
    break;
  default:
    res = "";
  }
  return res.c_str();
}

std::string ESP32_MailClient::imapErrorReasonStr()
{
  std::string res = "";

  switch (_imapStatus)
  {
  case IMAP_STATUS_SERVER_CONNECT_FAILED:
    res = ESP32_MAIL_STR_38;
    break;
  case IMAP_STATUS_IMAP_RESPONSE_FAILED:
    res = ESP32_MAIL_STR_40;
    break;
  case IMAP_STATUS_LOGIN_FAILED:
    res = ESP32_MAIL_STR_45;
    break;
  case IMAP_STATUS_BAD_COMMAND:
    res = ESP32_MAIL_STR_46;
    break;
  case IMAP_STATUS_PARSE_FLAG_FAILED:
    res = ESP32_MAIL_STR_256;
    break;
  case MAIL_CLIENT_STATUS_WIFI_CONNECT_FAIL:
    res = ESP32_MAIL_STR_221;
    break;
  default:
    res = "";
  }
  return res;
}

void ESP32_MailClient::ESP32MailDebugError()
{
  size_t dbgInfoLen = strlen_P(ESP32_MAIL_STR_227) + 1;
  char *dbgInfo = new char[dbgInfoLen];
  memset(dbgInfo, 0, dbgInfoLen);
  strcpy_P(dbgInfo, ESP32_MAIL_STR_227);
  ESP32MailDebugLine(dbgInfo, false);
  delete[] dbgInfo;
}

void ESP32_MailClient::ESP32MailDebugInfo(PGM_P info)
{
  size_t dbgInfoLen = strlen_P(info) + 1;
  char *dbgInfo = new char[dbgInfoLen];
  memset(dbgInfo, 0, dbgInfoLen);
  strcpy_P(dbgInfo, info);
  ESP32MailDebug(dbgInfo);
  delete[] dbgInfo;
}

bool ESP32_MailClient::sdBegin(uint8_t sck, uint8_t miso, uint8_t mosi, uint8_t ss)
{
  _sck = sck;
  _miso = miso;
  _mosi = mosi;
  _ss = ss;
  _sdConfigSet = true;
  SPI.begin(_sck, _miso, _mosi, _ss);
  return SD.begin(_ss, SPI);
}

bool ESP32_MailClient::sdBegin(void)
{
  _sdConfigSet = false;
  return SD.begin();
}

void ESP32_MailClient::set_message_header(string &header, string &message, bool htmlFormat)
{
  header += ESP32_MAIL_STR_33;
  header += ESP32_MAIL_STR_2;
  header += ESP32_MAIL_STR_34;
  if (!htmlFormat)
    header += ESP32_MAIL_STR_27;
  else
    header += ESP32_MAIL_STR_28;

  header += ESP32_MAIL_STR_29;
  header += ESP32_MAIL_STR_34;

  header += message;
  header += ESP32_MAIL_STR_34;
  header += ESP32_MAIL_STR_34;
}

void ESP32_MailClient::set_attachment_header(uint8_t index, std::string &header, attachmentData &attach)
{

  header += ESP32_MAIL_STR_33;
  header += ESP32_MAIL_STR_2;
  header += ESP32_MAIL_STR_34;

  header += ESP32_MAIL_STR_25;

  if (attach._mime_type[index].length() == 0)
    header += ESP32_MAIL_STR_32;
  else
    header += attach._mime_type[index];

  header += ESP32_MAIL_STR_26;

  std::string filename(attach._filename[index]);

  size_t found = filename.find_last_of("/\\");

  if (found != std::string::npos)
  {
    filename.clear();
    filename += attach._filename[index].substr(found + 1);
  }

  header += filename;
  header += ESP32_MAIL_STR_36;

  header += ESP32_MAIL_STR_30;
  header += filename;
  header += ESP32_MAIL_STR_36;

  header += ESP32_MAIL_STR_31;
  header += ESP32_MAIL_STR_34;

  std::string().swap(filename);
}

int ESP32_MailClient::waitSMTPResponse(SMTPData &smtpData)
{

  long dataTime = millis();
  char c = '\0';
  std::string lineBuf = "";
  int lfCount = 0;
  size_t p1 = 0;
  int resCode = -1000;

  while (smtpClientAvailable(smtpData, false) && millis() - dataTime < smtpData._net->tcpTimeout)
    delay(0);

  dataTime = millis();
  if (smtpClientAvailable(smtpData, true))
  {
    while (smtpClientAvailable(smtpData, true))
    {
      int r = smtpData._net->getStreamPtr()->read();

      if (r < 0)
        continue;

      c = (char)r;

      lineBuf.append(1, c);
      if (c == '\n')
      {
        dataTime = millis();
        if (lfCount == 0)
        {
          p1 = lineBuf.find(" ");
          if (p1 != std::string::npos)
            resCode = atoi(lineBuf.substr(0, p1).c_str());
        }
        if (smtpData._debug)
          ESP32MailDebug(lineBuf.c_str());
        lineBuf.clear();
        lfCount++;
      }

      if (millis() - dataTime > smtpData._net->tcpTimeout + 30000)
        break;
    }
  }
  std::string().swap(lineBuf);
  return resCode;
}

bool ESP32_MailClient::getIMAPResponse(IMAPData &imapData)
{
  long dataTime = millis();
  char c = '\0';
  bool success = false;
  std::string str = "";
  while (imapClientAvailable(imapData, false) && millis() - dataTime < imapData._net->tcpTimeout)
    delay(0);

  dataTime = millis();
  if (imapClientAvailable(imapData, true))
  {
    while (imapClientAvailable(imapData, true))
    {
      int r = imapData._net->getStreamPtr()->read();
      if (r < 0)
        continue;
      c = (char)r;
      if (c == '\n')
      {
        if (imapData._debug)
          ESP32MailDebug(str.c_str());
        str.clear();
      }
      else
        str += c;

      if (str.find(ESP32_MAIL_STR_132) != std::string::npos)
        success = true;
    }
  }

  std::string().swap(str);
  return success;
}

bool ESP32_MailClient::waitIMAPResponse(IMAPData &imapData, uint8_t imapCommandType, int maxChar, int mailIndex, int messageDataIndex, std::string part)
{

  long dataTime = millis();

  char c = 0;
  std::string lineBuf = "";
  std::string msgNumBuf = "";
  std::string filepath = "";
  std::string hpath = "";
  std::string tmp = "";
  std::string msgID = "";
  std::string from = "";
  std::string to = "";
  std::string subject = "";
  std::string date = "";
  std::string cc = "";
  std::string from_charset = "";
  std::string to_charset = "";
  std::string cc_charset = "";
  std::string subject_charset = "";
  std::string acceptLanguage = "";
  std::string contentLanguage = "";

  int bufSize = 100;
  char *dest = new char[bufSize];
  char *buf = new char[bufSize];

  int readCount = 0;
  int lfCount = 0;
  int charCount = 0;
  size_t p1 = 0;
  size_t p2 = 0;
  size_t p3 = 0;
  size_t payloadLength = 0;
  size_t outputLength;

  bool completeResp = false;
  bool validResponse = false;
  bool downloadReq = false;
  size_t currentDownloadByte = 0;

  int max = imapData._emailNumMax;
  if (!imapData._recentSort)
    max = max - 1;

  uint8_t headerType = 0;

  File file;
  int reportState = 0;
  int downloadedByte = 0;

  if (imapCommandType == IMAP_COMMAND_TYPE::LIST)
    std::vector<std::string>()
        .swap(imapData._folders);

  while (imapClientAvailable(imapData, false) && millis() - dataTime < imapData._net->tcpTimeout)
    delay(0);

  dataTime = millis();
  if (imapClientAvailable(imapData, true))
  {
    while (imapClientAvailable(imapData, true) || !completeResp)
    {

      int r = imapData._net->getStreamPtr()->read();

      if (r < 0)
        continue;

      c = (char)r;

      if (payloadLength > 0 && !completeResp)
        charCount++;

      if (imapCommandType == IMAP_COMMAND_TYPE::SEARCH && lfCount == 0)
      {
        delay(0);
        if (c == ' ')
        {
          p3 = msgNumBuf.find(ESP32_MAIL_STR_257);
          if (p3 != std::string::npos)
          {
            validResponse = false;
            break;
          }

          if (msgNumBuf != ESP32_MAIL_STR_183 && msgNumBuf != ESP32_MAIL_STR_141 && imapData._msgNum.size() <= max)
          {
            imapData._msgNum.push_back(atoi(msgNumBuf.c_str()));

            if (imapData._msgNum.size() > imapData._emailNumMax && imapData._recentSort)
              imapData._msgNum.erase(imapData._msgNum.begin());
            imapData._searchCount++;
          }

          msgNumBuf.clear();
        }
        else if (c != '\r' && c != '\n')
        {
          msgNumBuf.append(1, c);
        }
      }

      if (c != '\r' && c != '\n' && imapCommandType != IMAP_COMMAND_TYPE::SEARCH)
        lineBuf.append(1, c);

      if (validResponse && imapCommandType == IMAP_COMMAND_TYPE::FETCH_BODY_TEXT && lfCount > 0)
      {

        if (payloadLength > 0 && charCount < payloadLength - 1)
        {

          if (imapData._messageDataInfo[mailIndex][messageDataIndex]._transfer_encoding != ESP32_MAIL_STR_160)
          {
            if (charCount < maxChar)
              imapData._messageDataInfo[mailIndex][messageDataIndex]._text.append(1, c);

            if (imapData._saveHTMLMsg || imapData._saveTextMsg)
            {

              if (!imapData._messageDataInfo[mailIndex][messageDataIndex]._sdFileOpenWrite)
              {
                imapData._messageDataInfo[mailIndex][messageDataIndex]._sdFileOpenWrite = true;

                if (_sdOk)
                {
                  downloadReq = true;

                  filepath.clear();

                  filepath = imapData._savePath;
                  filepath += ESP32_MAIL_STR_202;

                  char *midx = new char[50];
                  memset(midx, 0, 50);
                  itoa(imapData._msgNum[mailIndex], midx, 10);

                  filepath += midx;

                  delete[] midx;

                  if (imapData._storageType == MailClientStorageType::SD)
                    if (!SD.exists(filepath.c_str()))
                      createDirs(filepath);

                  if (!imapData._headerSaved)
                    hpath = filepath + ESP32_MAIL_STR_203;

                  if (imapData._messageDataInfo[mailIndex][messageDataIndex]._contentType == ESP32_MAIL_STR_155)
                  {
                    if (imapData._saveDecodedText)
                      filepath += ESP32_MAIL_STR_161;
                    else
                      filepath += ESP32_MAIL_STR_162;
                  }
                  else if (imapData._messageDataInfo[mailIndex][messageDataIndex]._contentType == ESP32_MAIL_STR_154)
                  {
                    if (imapData._saveDecodedHTML)
                      filepath += ESP32_MAIL_STR_163;
                    else
                      filepath += ESP32_MAIL_STR_164;
                  }

                  if (imapData._storageType == MailClientStorageType::SD)
                    file = SD.open(filepath.c_str(), FILE_WRITE);
                  else if (imapData._storageType == MailClientStorageType::SPIFFS)
                    file = SPIFFS.open(filepath.c_str(), FILE_WRITE);
                }
                else
                {

                  if (imapData._messageDataCount[mailIndex] == messageDataIndex + 1)
                  {
                    imapData._messageDataInfo[mailIndex][messageDataIndex]._error = true;
                    imapData._messageDataInfo[mailIndex][messageDataIndex]._downloadError.clear();
                    imapData._messageDataInfo[mailIndex][messageDataIndex]._downloadError = ESP32_MAIL_STR_89;
                  }
                }
              }
              if (_sdOk)
                file.write(c);
            }
          }
        }

        if (millis() - dataTime > imapData._net->tcpTimeout + (30 * 1000) || (payloadLength > 0 && charCount == payloadLength && completeResp))
        {

          if (charCount < payloadLength || !completeResp)
            clientReadAll(imapData._net->getStreamPtr());

          break;
        }
      }

      if (c == '\n')
      {
        dataTime = millis();

        if (lfCount == 0)
        {
          if (imapData._debug)
            ESP32MailDebug(lineBuf.c_str());

          if (imapCommandType == IMAP_COMMAND_TYPE::FETCH_BODY_TEXT ||
              imapCommandType == IMAP_COMMAND_TYPE::FETCH_BODY_MIME ||
              imapCommandType == IMAP_COMMAND_TYPE::FETCH_BODY_HEADER ||
              imapCommandType == IMAP_COMMAND_TYPE::FETCH_BODY_ATTACHMENT)
          {

            p1 = lineBuf.find(ESP32_MAIL_STR_165);
            if (p1 != std::string::npos)
              validResponse = true;
          }

          p1 = lineBuf.find(ESP32_MAIL_STR_166);
          if (p1 != std::string::npos)
            validResponse = true;
        }

        p1 = lineBuf.find(ESP32_MAIL_STR_211);
        p2 = lineBuf.find(ESP32_MAIL_STR_158);
        p3 = lineBuf.find(ESP32_MAIL_STR_159);

        if (p1 != std::string::npos || p2 != std::string::npos || p3 != std::string::npos)
        {

          validResponse = true;

          if (p2 != std::string::npos || p3 != std::string::npos)
            validResponse = false;

          if (payloadLength == 0)
          {
            if (imapCommandType == IMAP_COMMAND_TYPE::LOGIN ||
                imapCommandType == IMAP_COMMAND_TYPE::LIST ||
                imapCommandType == IMAP_COMMAND_TYPE::EXAMINE ||
                imapCommandType == IMAP_COMMAND_TYPE::SEARCH ||
                imapCommandType == IMAP_COMMAND_TYPE::FETCH_BODY_MIME ||
                imapCommandType == IMAP_COMMAND_TYPE::FETCH_BODY_HEADER)
            {

              //Cyrus server 3.0 does not comply to rfc3501 as it resonses the CAPABILITY after received LOGIN command with no CAPABILITY command requested.
              if (lineBuf.find(ESP32_MAIL_STR_134) == std::string::npos && lineBuf.find(ESP32_MAIL_STR_145) == std::string::npos)
                completeResp = true;

              //Some servers e.g. STRATO E-Mail-Server does not reply any error when fetching none existing MIME header part at defined index.
              if (imapCommandType == IMAP_COMMAND_TYPE::FETCH_BODY_MIME)
                validResponse = false;
            }
          }
          else
          {

            if ((payloadLength > 0 && charCount >= payloadLength) || imapCommandType == IMAP_COMMAND_TYPE::FETCH_BODY_MIME)
            {
              completeResp = true;
            }
          }
        }

        if (imapCommandType == IMAP_COMMAND_TYPE::SEARCH && lfCount > 0)
        {
          completeResp = true;
          validResponse = true;
        }

        tmp = lineBuf;
        std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);

        if (imapCommandType == IMAP_COMMAND_TYPE::FETCH_BODY_MIME && lfCount > 0)
        {

          if (payloadLength > 0 && validResponse)
          {

            if (imapData._messageDataInfo[mailIndex].size() < messageDataIndex + 1)
            {
              messageBodyData b;
              imapData._messageDataInfo[mailIndex].push_back(b);
              imapData._messageDataCount[mailIndex] = imapData._messageDataInfo[mailIndex].size();
            }

            p1 = tmp.find(ESP32_MAIL_STR_167);
            if (p1 != std::string::npos)
            {

              p2 = lineBuf.find(";", p1 + strlen(ESP32_MAIL_STR_167));
              if (p2 != std::string::npos)
              {

                imapData._messageDataInfo[mailIndex][messageDataIndex]._contentType = lineBuf.substr(p1 + strlen(ESP32_MAIL_STR_167), p2 - p1 - strlen(ESP32_MAIL_STR_167));

                p1 = tmp.find(ESP32_MAIL_STR_168, p2);
                if (p1 != std::string::npos)
                {
                  p2 = lineBuf.find(ESP32_MAIL_STR_136, p1 + strlen(ESP32_MAIL_STR_168));
                  if (p2 != std::string::npos)
                    imapData._messageDataInfo[mailIndex][messageDataIndex]._charset = lineBuf.substr(p1 + strlen(ESP32_MAIL_STR_168), p2 - p1 - strlen(ESP32_MAIL_STR_168));
                }
                else if (tmp.find(ESP32_MAIL_STR_169, p2) != std::string::npos)
                {
                  p1 = tmp.find(ESP32_MAIL_STR_169, p2);
                  imapData._messageDataInfo[mailIndex][messageDataIndex]._charset = lineBuf.substr(p1 + strlen(ESP32_MAIL_STR_169));
                }

                p1 = tmp.find(ESP32_MAIL_STR_170, p2);
                if (p1 != std::string::npos)
                {
                  p2 = lineBuf.find(ESP32_MAIL_STR_136, p1 + strlen(ESP32_MAIL_STR_170));
                  if (p2 != std::string::npos)
                    imapData._messageDataInfo[mailIndex][messageDataIndex]._name = lineBuf.substr(p1 + strlen(ESP32_MAIL_STR_170), p2 - p1 - strlen(ESP32_MAIL_STR_170));
                }
                else if (tmp.find(ESP32_MAIL_STR_171, p2) != std::string::npos)
                {
                  p1 = tmp.find(ESP32_MAIL_STR_171, p2);
                  imapData._messageDataInfo[mailIndex][messageDataIndex]._name = lineBuf.substr(p1 + strlen(ESP32_MAIL_STR_171));
                }
              }
            }

            p1 = tmp.find(ESP32_MAIL_STR_172);
            if (p1 != std::string::npos)
            {

              p2 = lineBuf.find(ESP32_MAIL_STR_173, p1 + strlen(ESP32_MAIL_STR_172));

              if (p2 != std::string::npos)
                imapData._messageDataInfo[mailIndex][messageDataIndex]._transfer_encoding = lineBuf.substr(p1 + strlen(ESP32_MAIL_STR_172), p2 - p1 - strlen(ESP32_MAIL_STR_172));
              else
                imapData._messageDataInfo[mailIndex][messageDataIndex]._transfer_encoding = lineBuf.substr(p1 + strlen(ESP32_MAIL_STR_172));
            }

            p1 = tmp.find(ESP32_MAIL_STR_174);
            if (p1 != std::string::npos)
            {
              p2 = lineBuf.find(ESP32_MAIL_STR_173, p1 + strlen(ESP32_MAIL_STR_174));

              if (p2 != std::string::npos)
                imapData._messageDataInfo[mailIndex][messageDataIndex]._descr = lineBuf.substr(p1 + strlen(ESP32_MAIL_STR_174), p2 - p1 - strlen(ESP32_MAIL_STR_174));
              else
                imapData._messageDataInfo[mailIndex][messageDataIndex]._descr = lineBuf.substr(p1 + strlen(ESP32_MAIL_STR_174));
            }

            p1 = tmp.find(ESP32_MAIL_STR_175);
            if (p1 != std::string::npos)
            {

              p2 = lineBuf.find(";", p1 + strlen(ESP32_MAIL_STR_175));

              if (p2 != std::string::npos)
                imapData._messageDataInfo[mailIndex][messageDataIndex]._disposition = lineBuf.substr(p1 + strlen(ESP32_MAIL_STR_175), p2 - p1 - strlen(ESP32_MAIL_STR_175));
              else
                imapData._messageDataInfo[mailIndex][messageDataIndex]._disposition = lineBuf.substr(p1 + strlen(ESP32_MAIL_STR_175));

              if (imapData._messageDataInfo[mailIndex][messageDataIndex]._disposition == ESP32_MAIL_STR_153)
                imapData._attachmentCount[mailIndex]++;
            }

            if (imapData._messageDataInfo[mailIndex][messageDataIndex]._disposition != "")
            {

              p1 = tmp.find(ESP32_MAIL_STR_176);
              if (p1 != std::string::npos)
              {
                p2 = lineBuf.find(ESP32_MAIL_STR_136, p1 + strlen(ESP32_MAIL_STR_176));

                if (p2 != std::string::npos)
                  imapData._messageDataInfo[mailIndex][messageDataIndex]._filename = lineBuf.substr(p1 + strlen(ESP32_MAIL_STR_176), p2 - p1 - strlen(ESP32_MAIL_STR_176));
              }
              else if (tmp.find(ESP32_MAIL_STR_177) != std::string::npos)
              {

                p1 = tmp.find(ESP32_MAIL_STR_177);
                imapData._messageDataInfo[mailIndex][messageDataIndex]._filename = lineBuf.substr(p1 + strlen(ESP32_MAIL_STR_177));
              }

              p1 = tmp.find(ESP32_MAIL_STR_178);
              if (p1 != std::string::npos)
              {
                p2 = lineBuf.find(";", p1 + strlen(ESP32_MAIL_STR_178) + 1);
                if (p2 != std::string::npos)
                {
                  imapData._messageDataInfo[mailIndex][messageDataIndex]._size = atoi(lineBuf.substr(p1 + strlen(ESP32_MAIL_STR_178), p2 - p1 - strlen(ESP32_MAIL_STR_178)).c_str());
                  imapData._totalAttachFileSize[mailIndex] += imapData._messageDataInfo[mailIndex][messageDataIndex]._size;
                }
                else
                {
                  imapData._messageDataInfo[mailIndex][messageDataIndex]._size = atoi(lineBuf.substr(p1 + strlen(ESP32_MAIL_STR_178)).c_str());
                  imapData._totalAttachFileSize[mailIndex] += imapData._messageDataInfo[mailIndex][messageDataIndex]._size;
                }
              }

              p1 = tmp.find(ESP32_MAIL_STR_179);
              if (p1 != std::string::npos)
              {
                p2 = lineBuf.find(ESP32_MAIL_STR_136, p1 + strlen(ESP32_MAIL_STR_179));
                if (p2 != std::string::npos)
                  imapData._messageDataInfo[mailIndex][messageDataIndex]._creation_date = lineBuf.substr(p1 + strlen(ESP32_MAIL_STR_179), p2 - p1 - strlen(ESP32_MAIL_STR_179));
              }
              else if (tmp.find(ESP32_MAIL_STR_180) != std::string::npos)
              {
                p1 = tmp.find(ESP32_MAIL_STR_180);
                imapData._messageDataInfo[mailIndex][messageDataIndex]._creation_date = lineBuf.substr(p1 + strlen(ESP32_MAIL_STR_180));
              }

              p1 = tmp.find(ESP32_MAIL_STR_181);
              if (p1 != std::string::npos)
              {
                p2 = lineBuf.find(ESP32_MAIL_STR_136, p1 + strlen(ESP32_MAIL_STR_181));
                if (p2 != std::string::npos)
                  imapData._messageDataInfo[mailIndex][messageDataIndex]._modification_date = lineBuf.substr(p1 + strlen(ESP32_MAIL_STR_181), p2 - p1 - strlen(ESP32_MAIL_STR_181));
              }
              else if (tmp.find(ESP32_MAIL_STR_182) != std::string::npos)
              {
                p1 = tmp.find(ESP32_MAIL_STR_182);
                imapData._messageDataInfo[mailIndex][messageDataIndex]._modification_date = lineBuf.substr(p1 + strlen(ESP32_MAIL_STR_182));
              }
            }

            imapData._messageDataInfo[mailIndex][messageDataIndex]._part = part;
          }
        }

        if (imapCommandType == IMAP_COMMAND_TYPE::SEARCH && lfCount == 0)
        {

          if (msgNumBuf.length() > 0 && msgNumBuf != ESP32_MAIL_STR_183 && msgNumBuf != ESP32_MAIL_STR_141 && imapData._msgNum.size() <= max)
          {
            imapData._msgNum.push_back(atoi(msgNumBuf.c_str()));
            imapData._searchCount++;

            if (imapData._msgNum.size() > imapData._emailNumMax && imapData._recentSort)
              imapData._msgNum.erase(imapData._msgNum.begin());
          }

          if (imapData._recentSort)
            std::sort(imapData._msgNum.begin(), imapData._msgNum.end(), compFunc);
        }

        if (imapCommandType == IMAP_COMMAND_TYPE::FETCH_BODY_HEADER)
        {

          uint8_t _headerType = 0;

          p1 = tmp.find(ESP32_MAIL_STR_184);
          if (p1 != std::string::npos)
          {
            headerType = IMAP_HEADER_TYPE::FROM;
            _headerType = IMAP_HEADER_TYPE::FROM;

            p2 = lineBuf.find(ESP32_MAIL_STR_173, p1 + strlen(ESP32_MAIL_STR_184));
            if (p2 != std::string::npos)
              from = lineBuf.substr(p1 + strlen(ESP32_MAIL_STR_184), p2 - p1 - strlen(ESP32_MAIL_STR_184));
            else
              from = lineBuf.substr(p1 + strlen(ESP32_MAIL_STR_184));

            if (from[0] == '=' && from[1] == '?')
            {
              p1 = from.find("?", 2);

              if (p1 != std::string::npos)
                from_charset = from.substr(2, p1 - 2);
            }

            memset(dest, 0, bufSize);
            RFC2047Decoder.rfc2047Decode(dest, from.c_str(), bufSize);
            from = dest;
          }

          p1 = tmp.find(ESP32_MAIL_STR_185);
          if (p1 != std::string::npos)
          {
            headerType = IMAP_HEADER_TYPE::TO;
            _headerType = IMAP_HEADER_TYPE::TO;

            p2 = lineBuf.find(ESP32_MAIL_STR_173, p1 + 1);
            if (p2 != std::string::npos)
              to = lineBuf.substr(p1 + strlen(ESP32_MAIL_STR_185), p2 - p1 - strlen(ESP32_MAIL_STR_185));
            else
              to = lineBuf.substr(p1 + strlen(ESP32_MAIL_STR_185));

            if (to[0] == '=' && to[1] == '?')
            {
              p1 = to.find("?", 2);

              if (p1 != std::string::npos)
                to_charset = to.substr(2, p1 - 2);
            }

            memset(dest, 0, bufSize);
            RFC2047Decoder.rfc2047Decode(dest, to.c_str(), bufSize);
            to = dest;
          }

          p1 = tmp.find(ESP32_MAIL_STR_186);
          if (p1 != std::string::npos)
          {
            headerType = IMAP_HEADER_TYPE::CC;
            _headerType = IMAP_HEADER_TYPE::CC;

            p2 = lineBuf.find(ESP32_MAIL_STR_173, p1 + 1);
            if (p2 != std::string::npos)
              cc = lineBuf.substr(p1 + strlen(ESP32_MAIL_STR_186), p2 - p1 - strlen(ESP32_MAIL_STR_186));
            else
              cc = lineBuf.substr(p1 + strlen(ESP32_MAIL_STR_186));

            if (cc[0] == '=' && cc[1] == '?')
            {
              p1 = cc.find("?", 2);

              if (p1 != std::string::npos)
                cc_charset = cc.substr(2, p1 - 2);
            }

            memset(dest, 0, bufSize);
            RFC2047Decoder.rfc2047Decode(dest, cc.c_str(), bufSize);
            cc = dest;
          }

          p1 = tmp.find(ESP32_MAIL_STR_187);
          if (p1 != std::string::npos)
          {
            headerType = IMAP_HEADER_TYPE::SUBJECT;
            _headerType = IMAP_HEADER_TYPE::SUBJECT;

            p2 = lineBuf.find(ESP32_MAIL_STR_173, p1 + 1);

            memset(dest, 0, bufSize);
            if (p2 != std::string::npos)
              subject = lineBuf.substr(p1 + strlen(ESP32_MAIL_STR_187), p2 - p1 - strlen(ESP32_MAIL_STR_187));
            else
              subject = lineBuf.substr(p1 + strlen(ESP32_MAIL_STR_187));

            if (subject[0] == '=' && subject[1] == '?')
            {
              p1 = subject.find("?", 2);
              if (p1 != std::string::npos)
                subject_charset = subject.substr(2, p1 - 2);
            }

            memset(dest, 0, bufSize);
            RFC2047Decoder.rfc2047Decode(dest, subject.c_str(), bufSize);
            subject = dest;
          }
          p1 = tmp.find(ESP32_MAIL_STR_188);
          if (p1 != std::string::npos)
          {
            headerType = IMAP_HEADER_TYPE::DATE;
            _headerType = IMAP_HEADER_TYPE::DATE;

            p2 = lineBuf.find(ESP32_MAIL_STR_173, p1 + 1);
            if (p2 != std::string::npos)
              date = lineBuf.substr(p1 + strlen(ESP32_MAIL_STR_188), p2 - p1 - strlen(ESP32_MAIL_STR_188));
            else
              date = lineBuf.substr(p1 + strlen(ESP32_MAIL_STR_188));
          }

          p1 = tmp.find(ESP32_MAIL_STR_189);
          if (p1 != std::string::npos)
          {
            headerType = IMAP_HEADER_TYPE::MSG_ID;
            _headerType = IMAP_HEADER_TYPE::MSG_ID;

            p2 = lineBuf.find(ESP32_MAIL_STR_173, p1 + 1);
            if (p2 != std::string::npos)
              msgID = lineBuf.substr(p1 + strlen(ESP32_MAIL_STR_189), p2 - p1 - strlen(ESP32_MAIL_STR_189));
            else
              msgID = lineBuf.substr(p1 + strlen(ESP32_MAIL_STR_189));
          }

          p1 = tmp.find(ESP32_MAIL_STR_190);
          if (p1 != std::string::npos)
          {
            headerType = IMAP_HEADER_TYPE::ACCEPT_LANG;
            _headerType = IMAP_HEADER_TYPE::ACCEPT_LANG;

            p2 = lineBuf.find(ESP32_MAIL_STR_173, p1 + 1);
            if (p2 != std::string::npos)
              acceptLanguage = lineBuf.substr(p1 + strlen(ESP32_MAIL_STR_190), p2 - p1 - strlen(ESP32_MAIL_STR_190));
            else
              acceptLanguage = lineBuf.substr(p1 + strlen(ESP32_MAIL_STR_190));
          }

          p1 = tmp.find(ESP32_MAIL_STR_191);
          if (p1 != std::string::npos)
          {
            headerType = IMAP_HEADER_TYPE::CONT_LANG;
            _headerType = IMAP_HEADER_TYPE::CONT_LANG;

            p2 = lineBuf.find(ESP32_MAIL_STR_173, p1 + 1);
            if (p2 != std::string::npos)
              contentLanguage = lineBuf.substr(p1 + strlen(ESP32_MAIL_STR_191), p2 - p1 - strlen(ESP32_MAIL_STR_191));
            else
              contentLanguage = lineBuf.substr(p1 + strlen(ESP32_MAIL_STR_191));
          }

          if (_headerType == 0 && charCount < payloadLength && payloadLength > 0)
          {
            if (headerType == IMAP_HEADER_TYPE::FROM)
            {
              memset(dest, 0, bufSize);
              RFC2047Decoder.rfc2047Decode(dest, lineBuf.c_str(), bufSize);
              from += dest;
            }
            else if (headerType == IMAP_HEADER_TYPE::TO)
            {
              memset(dest, 0, bufSize);
              RFC2047Decoder.rfc2047Decode(dest, lineBuf.c_str(), bufSize);
              to += dest;
            }
            else if (headerType == IMAP_HEADER_TYPE::CC)
            {
              memset(dest, 0, bufSize);
              RFC2047Decoder.rfc2047Decode(dest, lineBuf.c_str(), bufSize);
              cc += dest;
            }
            else if (headerType == IMAP_HEADER_TYPE::SUBJECT)
            {
              memset(dest, 0, bufSize);
              RFC2047Decoder.rfc2047Decode(dest, lineBuf.c_str(), bufSize);
              subject += dest;
            }
          }
        }

        if (imapCommandType == IMAP_COMMAND_TYPE::LIST)
        {
          p1 = lineBuf.find(ESP32_MAIL_STR_195);
          p2 = lineBuf.find(ESP32_MAIL_STR_196);

          if (p1 != std::string::npos && p2 == std::string::npos)
          {
            p2 = lineBuf.find_last_of(ESP32_MAIL_STR_136);
            if (p2 != std::string::npos)
            {
              p1 = lineBuf.find_last_of(ESP32_MAIL_STR_136, p2 - 1);
              if (p1 != std::string::npos)
                imapData._folders.push_back(lineBuf.substr(p1 + 1, p2 - p1 - 1));
            }
          }
        }

        if (imapCommandType == IMAP_COMMAND_TYPE::SELECT || imapCommandType == IMAP_COMMAND_TYPE::EXAMINE)
        {

          p1 = lineBuf.find(ESP32_MAIL_STR_197);
          if (p1 != std::string::npos)
          {
            p1 = lineBuf.find(ESP32_MAIL_STR_198);
            if (p1 != std::string::npos)
            {
              p2 = lineBuf.find(ESP32_MAIL_STR_192);
              if (p2 != std::string::npos)
              {
                string _tmp;

                _tmp = lineBuf.substr(p1 + 1, p2 - p1 - 1).c_str();
                msgNumBuf.clear();

                for (size_t i = 0; i < _tmp.length(); i++)
                {
                  if (_tmp[i] != '\\' && _tmp[i] != ' ' && _tmp[i] != '\r' && _tmp[i] != '\n')
                    msgNumBuf.append(1, _tmp[i]);

                  if (_tmp[i] == ' ')
                  {
                    imapData._flag.push_back(msgNumBuf);
                    msgNumBuf.clear();
                  }
                }
                if (msgNumBuf.length() > 0)
                {
                  imapData._flag.push_back(msgNumBuf);
                }

                std::string().swap(_tmp);
              }
            }
          }

          p2 = lineBuf.find(ESP32_MAIL_STR_199);
          if (p2 != std::string::npos)
            imapData._totalMessage = atoi(lineBuf.substr(2, p2 - 2).c_str());

          p1 = lineBuf.find(ESP32_MAIL_STR_200);
          if (p1 != std::string::npos)
          {
            p2 = lineBuf.find(ESP32_MAIL_STR_156, p1 + 10);
            if (p2 != std::string::npos)
              imapData._nextUID = lineBuf.substr(p1 + 10, p2 - p1 - 10);
          }
        }

        if (validResponse && imapCommandType == IMAP_COMMAND_TYPE::FETCH_BODY_TEXT && lfCount > 0 && (charCount < maxChar || imapData._saveHTMLMsg || imapData._saveTextMsg))
        {

          if (imapData._messageDataInfo[mailIndex][messageDataIndex]._transfer_encoding == ESP32_MAIL_STR_160)
          {

            unsigned char *decoded = base64_decode_char((const unsigned char *)lineBuf.c_str(), lineBuf.length(), &outputLength);

            if (decoded)
            {
              if (charCount < maxChar)
                imapData._messageDataInfo[mailIndex][messageDataIndex]._text.append((char *)decoded, outputLength);

              if (imapData._saveHTMLMsg || imapData._saveTextMsg)
              {

                if (!imapData._messageDataInfo[mailIndex][messageDataIndex]._sdFileOpenWrite)
                {

                  imapData._messageDataInfo[mailIndex][messageDataIndex]._sdFileOpenWrite = true;

                  if (_sdOk)
                  {

                    downloadReq = true;

                    filepath.clear();
                    filepath += imapData._savePath;
                    filepath += ESP32_MAIL_STR_202;

                    char *midx = new char[50];
                    memset(midx, 0, 50);
                    itoa(imapData._msgNum[mailIndex], midx, 10);

                    filepath += midx;

                    delete[] midx;

                    if (imapData._storageType == MailClientStorageType::SD)
                      if (!SD.exists(filepath.c_str()))
                        createDirs(filepath);

                    if (!imapData._headerSaved)
                      hpath = filepath + ESP32_MAIL_STR_203;

                    if (imapData._messageDataInfo[mailIndex][messageDataIndex]._contentType == ESP32_MAIL_STR_155)
                    {
                      if (imapData._saveDecodedText)
                        filepath += ESP32_MAIL_STR_161;
                      else
                        filepath += ESP32_MAIL_STR_162;
                    }
                    else if (imapData._messageDataInfo[mailIndex][messageDataIndex]._contentType == ESP32_MAIL_STR_154)
                    {
                      if (imapData._saveDecodedHTML)
                        filepath += ESP32_MAIL_STR_163;
                      else
                        filepath += ESP32_MAIL_STR_164;
                    }

                    if (imapData._storageType == MailClientStorageType::SD)
                      file = SD.open(filepath.c_str(), FILE_WRITE);
                    else if (imapData._storageType == MailClientStorageType::SPIFFS)
                      file = SPIFFS.open(filepath.c_str(), FILE_WRITE);
                  }
                  else
                  {
                    if (imapData._messageDataCount[mailIndex] == messageDataIndex + 1)
                    {
                      imapData._messageDataInfo[mailIndex][messageDataIndex]._error = true;
                      imapData._messageDataInfo[mailIndex][messageDataIndex]._downloadError.clear();
                      imapData._messageDataInfo[mailIndex][messageDataIndex]._downloadError = ESP32_MAIL_STR_89;
                    }
                  }
                }

                if (_sdOk)
                {
                  if ((imapData._messageDataInfo[mailIndex][messageDataIndex]._contentType == ESP32_MAIL_STR_155 && imapData._saveDecodedText) ||
                      (imapData._messageDataInfo[mailIndex][messageDataIndex]._contentType == ESP32_MAIL_STR_154 && imapData._saveDecodedHTML))
                    file.write((const uint8_t *)decoded, outputLength);
                  else
                    file.write((const uint8_t *)lineBuf.c_str(), lineBuf.length());
                }
              }

              delete[] decoded;
            }
          }
        }

        if (validResponse && imapCommandType == IMAP_COMMAND_TYPE::FETCH_BODY_ATTACHMENT && lfCount > 0)
        {

          if (imapData._messageDataInfo[mailIndex][messageDataIndex]._transfer_encoding == ESP32_MAIL_STR_160)
          {

            if (!imapData._messageDataInfo[mailIndex][messageDataIndex]._sdFileOpenWrite)
            {

              imapData._messageDataInfo[mailIndex][messageDataIndex]._sdFileOpenWrite = true;

              if (_sdOk)
              {

                downloadReq = true;

                filepath.clear();
                filepath += imapData._savePath;
                filepath += ESP32_MAIL_STR_202;

                char *midx = new char[50];
                memset(midx, 0, 50);
                itoa(imapData._msgNum[mailIndex], midx, 10);

                filepath += midx;

                delete[] midx;

                if (imapData._storageType == MailClientStorageType::SD)
                  if (!SD.exists(filepath.c_str()))
                    createDirs(filepath);

                filepath += ESP32_MAIL_STR_202;

                filepath += imapData._messageDataInfo[mailIndex][messageDataIndex]._filename;

                if (imapData._storageType == MailClientStorageType::SD)
                  file = SD.open(filepath.c_str(), FILE_WRITE);
                else if (imapData._storageType == MailClientStorageType::SPIFFS)
                  file = SPIFFS.open(filepath.c_str(), FILE_WRITE);
              }
              else
              {
                if (imapData._messageDataCount[mailIndex] == messageDataIndex + 1)
                {
                  imapData._messageDataInfo[mailIndex][messageDataIndex]._error = true;
                  imapData._messageDataInfo[mailIndex][messageDataIndex]._downloadError.clear();
                  imapData._messageDataInfo[mailIndex][messageDataIndex]._downloadError = ESP32_MAIL_STR_89;
                }
              }
            }

            if (_sdOk)
            {

              unsigned char *decoded = base64_decode_char((const unsigned char *)lineBuf.c_str(), lineBuf.length(), &outputLength);

              downloadedByte += outputLength;

              if (downloadedByte > imapData._messageDataInfo[mailIndex][messageDataIndex]._size)
                continue;

              if (decoded)
              {
                file.write((const uint8_t *)decoded, outputLength);

                if (imapData._storageType == MailClientStorageType::SPIFFS)
                  delayMicroseconds(1);
                else
                  yield();

                if (imapData._downloadReport)
                {
                  imapData._downloadedByte[mailIndex] += outputLength;
                  currentDownloadByte += outputLength;

                  if (imapData._messageDataInfo[mailIndex][messageDataIndex]._size == 0)
                  {
                    if (payloadLength > 36)
                    {
                      imapData._messageDataInfo[mailIndex][messageDataIndex]._size = base64DecodeSize(lineBuf, payloadLength - (payloadLength / 36));
                      imapData._totalAttachFileSize[mailIndex] += imapData._messageDataInfo[mailIndex][messageDataIndex]._size;
                    }
                  }

                  int p = 0;

                  if (imapData._totalAttachFileSize[mailIndex] > 0)
                    p = 100 * imapData._downloadedByte[mailIndex] / imapData._totalAttachFileSize[mailIndex];

                  if ((p % 5 == 0) && (p <= 100))
                  {

                    if (imapData._readCallback && reportState != -1)
                    {
                      memset(buf, 0, bufSize);
                      itoa(p, buf, 10);

                      std::string dl = ESP32_MAIL_STR_90 + imapData._messageDataInfo[mailIndex][messageDataIndex]._filename + ESP32_MAIL_STR_91 + buf + ESP32_MAIL_STR_92;

                      if (imapData._readCallback)
                      {
                        imapData._cbData._info = dl;
                        imapData._cbData._status = dl;
                        imapData._cbData._success = false;
                        imapData._readCallback(imapData._cbData);
                      }

                      std::string().swap(dl);
                    }
                    reportState = -1;
                  }
                  else
                    reportState = 0;
                }

                delete[] decoded;
              }

              if (millis() - dataTime > imapData._net->tcpTimeout + 1000 * 60 * 5)
                break;
            }
          }
        }

        if (lfCount == 0)
        {
          p1 = lineBuf.find_last_of(ESP32_MAIL_STR_193);
          if (p1 != std::string::npos)
          {
            p2 = lineBuf.find(ESP32_MAIL_STR_194, p1 + 1);
            if (p2 != std::string::npos)
              payloadLength = atoi(lineBuf.substr(p1 + 1, p2 - p1 - 1).c_str());
          }
        }

        lineBuf.clear();
        lfCount++;
        std::string().swap(tmp);
      }

      readCount++;
    }

    if (imapData._error.size() > 0 && mailIndex > -1)
    {
      if (validResponse && !imapData._error[mailIndex])
      {
        imapData._errorMsg[mailIndex].clear();
        imapData._errorMsg[mailIndex] = "";
      }
    }

    if (millis() - dataTime > imapData._net->tcpTimeout)
    {

      if (downloadReq)
      {
        if (imapData._messageDataCount[mailIndex] == messageDataIndex + 1)
        {
          imapData._messageDataInfo[mailIndex][messageDataIndex]._error = true;
          imapData._messageDataInfo[mailIndex][messageDataIndex]._downloadError.clear();
          imapData._messageDataInfo[mailIndex][messageDataIndex]._downloadError = ESP32_MAIL_STR_93;
        }
      }
      else
      {

        if (imapData._error.size() > 0 && mailIndex > -1)
        {
          imapData._error[mailIndex] = true;
          imapData._errorMsg[mailIndex].clear();
          imapData._errorMsg[mailIndex] = ESP32_MAIL_STR_95;
        }
      }
    }
  }

  if (validResponse && (imapCommandType == IMAP_COMMAND_TYPE::FETCH_BODY_ATTACHMENT || imapCommandType == IMAP_COMMAND_TYPE::FETCH_BODY_TEXT) && messageDataIndex != -1)
  {
    if (imapData._messageDataInfo[mailIndex][messageDataIndex]._sdFileOpenWrite)
      file.close();
  }

  if (validResponse && imapCommandType == IMAP_COMMAND_TYPE::FETCH_BODY_ATTACHMENT && imapData._messageDataInfo[mailIndex][messageDataIndex]._size != currentDownloadByte)
  {
    imapData._messageDataInfo[mailIndex][messageDataIndex]._size = currentDownloadByte;
  }

  if (hpath != "")
  {

    if (imapData._storageType == MailClientStorageType::SD)
      file = SD.open(hpath.c_str(), FILE_WRITE);
    else if (imapData._storageType == MailClientStorageType::SPIFFS)
      file = SPIFFS.open(hpath.c_str(), FILE_WRITE);

    file.print(ESP32_MAIL_STR_99);
    file.println(imapData._date[mailIndex].c_str());

    file.print(ESP32_MAIL_STR_100);
    if (imapData._uidSearch)
      file.println(imapData._msgNum[mailIndex]);
    else
      file.println();

    file.print(ESP32_MAIL_STR_101);
    file.println(imapData._msgNum[mailIndex]);

    file.print(ESP32_MAIL_STR_102);
    file.println(imapData._acceptLanguage[mailIndex].c_str());

    file.print(ESP32_MAIL_STR_103);
    file.println(imapData._contentLanguage[mailIndex].c_str());

    file.print(ESP32_MAIL_STR_104);
    file.println(imapData._from[mailIndex].c_str());

    file.print(ESP32_MAIL_STR_105);
    file.println(imapData._from_charset[mailIndex].c_str());

    file.print(ESP32_MAIL_STR_106);
    file.println(imapData._to[mailIndex].c_str());

    file.print(ESP32_MAIL_STR_107);
    file.println(imapData._to_charset[mailIndex].c_str());

    file.print(ESP32_MAIL_STR_108);
    file.println(imapData._cc[mailIndex].c_str());

    file.print(ESP32_MAIL_STR_109);
    file.println(imapData._cc_charset[mailIndex].c_str());

    file.print(ESP32_MAIL_STR_110);
    file.println(imapData._subject[mailIndex].c_str());

    file.print(ESP32_MAIL_STR_111);
    file.println(imapData._subject_charset[mailIndex].c_str());

    file.print(ESP32_MAIL_STR_112);
    file.println(imapData._messageDataInfo[mailIndex][messageDataIndex]._charset.c_str());

    if (imapData._attachmentCount[mailIndex] > 0)
    {

      file.print(ESP32_MAIL_STR_113);
      file.println(imapData._attachmentCount[mailIndex]);

      for (int j = 0; j < imapData._attachmentCount[mailIndex]; j++)
      {
        file.print(ESP32_MAIL_STR_114);
        file.println(j + 1);

        file.print(ESP32_MAIL_STR_115);
        file.println(imapData.getAttachmentFileName(mailIndex, j));

        file.print(ESP32_MAIL_STR_116);
        file.println(imapData.getAttachmentName(mailIndex, j));

        file.print(ESP32_MAIL_STR_117);
        file.println(imapData.getAttachmentFileSize(mailIndex, j));

        file.print(ESP32_MAIL_STR_118);
        file.println(imapData.getAttachmentType(mailIndex, j));

        file.print(ESP32_MAIL_STR_119);
        file.println(imapData.getAttachmentCreationDate(mailIndex, j));
      }
    }

    file.close();
    imapData._headerSaved = true;
  }

  if (imapCommandType == IMAP_COMMAND_TYPE::FETCH_BODY_HEADER)
  {
    if (from != "")
    {
      imapData._msgID[mailIndex] = msgID;
      imapData._from[mailIndex] = from;
      imapData._to[mailIndex] = to;
      imapData._cc[mailIndex] = cc;
      imapData._subject[mailIndex] = subject;
      imapData._date[mailIndex] = date;
      imapData._from_charset[mailIndex] = from_charset;
      imapData._to_charset[mailIndex] = to_charset;
      imapData._cc_charset[mailIndex] = cc_charset;
      imapData._subject_charset[mailIndex] = subject_charset;
      imapData._contentLanguage[mailIndex] = contentLanguage;
      imapData._acceptLanguage[mailIndex] = acceptLanguage;
    }
  }

  delete[] buf;
  delete[] dest;

  std::string().swap(lineBuf);
  std::string().swap(msgNumBuf);
  std::string().swap(filepath);
  std::string().swap(hpath);
  std::string().swap(tmp);

  std::string().swap(msgID);
  std::string().swap(from);
  std::string().swap(to);
  std::string().swap(subject);
  std::string().swap(date);
  std::string().swap(cc);
  std::string().swap(from_charset);
  std::string().swap(to_charset);
  std::string().swap(cc_charset);
  std::string().swap(subject_charset);
  std::string().swap(contentLanguage);
  std::string().swap(acceptLanguage);

  return validResponse;
}

void ESP32_MailClient::clientReadAll(WiFiClient *client)
{
  if (client)
  {
    if (client->available() > 0)
      client->read();
  }
}

double ESP32_MailClient::base64DecodeSize(std::string lastBase64String, int length)
{
  double result = 0;
  int padding = 0;
  if (lastBase64String != "")
  {

    if (lastBase64String[lastBase64String.length() - 1] == '=' && lastBase64String[lastBase64String.length() - 2] == '=')
      padding = 2;
    else if (lastBase64String[lastBase64String.length() - 1] == '=')
      padding = 1;
  }
  result = (ceil(length / 4) * 3) - padding;
  return result;
}

unsigned char *ESP32_MailClient::base64_decode_char(const unsigned char *src, size_t len, size_t *out_len)
{

  unsigned char *out, *pos, block[4], tmp;
  size_t i, count, olen;
  int pad = 0;
  size_t extra_pad;

  unsigned char *dtable = new unsigned char[256];

  memset(dtable, 0x80, 256);

  for (i = 0; i < sizeof(base64_table) - 1; i++)
    dtable[base64_table[i]] = (unsigned char)i;
  dtable['='] = 0;

  count = 0;
  for (i = 0; i < len; i++)
  {
    if (dtable[src[i]] != 0x80)
      count++;
  }

  if (count == 0)
    goto exit;
  extra_pad = (4 - count % 4) % 4;

  olen = (count + extra_pad) / 4 * 3;
  pos = out = (unsigned char *)malloc(olen);
  if (out == NULL)
    goto exit;

  count = 0;
  for (i = 0; i < len + extra_pad; i++)
  {
    unsigned char val;

    if (i >= len)
      val = '=';
    else
      val = src[i];
    tmp = dtable[val];
    if (tmp == 0x80)
      continue;

    if (val == '=')
      pad++;
    block[count] = tmp;
    count++;
    if (count == 4)
    {
      *pos++ = (block[0] << 2) | (block[1] >> 4);
      *pos++ = (block[1] << 4) | (block[2] >> 2);
      *pos++ = (block[2] << 6) | block[3];
      count = 0;
      if (pad)
      {
        if (pad == 1)
          pos--;
        else if (pad == 2)
          pos -= 2;
        else
        {
          free(out);
          goto exit;
        }
        break;
      }
    }
  }

  *out_len = pos - out;
  delete[] dtable;
  return out;

exit:
  delete[] dtable;
  return NULL;
}

std::string ESP32_MailClient::base64_encode_string(const unsigned char *src, size_t len)
{
  unsigned char *out, *pos;
  const unsigned char *end, *in;

  size_t olen;

  olen = 4 * ((len + 2) / 3);

  if (olen < len)
    return std::string();

  std::string outStr = "";
  outStr.resize(olen);
  out = (unsigned char *)&outStr[0];

  end = src + len;
  in = src;
  pos = out;

  while (end - in >= 3)
  {
    *pos++ = base64_table[in[0] >> 2];
    *pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
    *pos++ = base64_table[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
    *pos++ = base64_table[in[2] & 0x3f];
    in += 3;
  }

  if (end - in)
  {
    *pos++ = base64_table[in[0] >> 2];
    if (end - in == 1)
    {
      *pos++ = base64_table[(in[0] & 0x03) << 4];
      *pos++ = '=';
    }
    else
    {
      *pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
      *pos++ = base64_table[(in[1] & 0x0f) << 2];
    }
    *pos++ = '=';
  }

  return outStr;
}

void ESP32_MailClient::send_base64_encode_mime_data(WiFiClient *client, const unsigned char *src, size_t len)
{

  const unsigned char *end, *in;

  size_t olen;

  olen = 4 * ((len + 2) / 3);

  if (olen < len)
    return;

  end = src + len;
  in = src;

  size_t chunkSize = 936;
  size_t byteAdd = 0;
  size_t byteSent = 0;

  int dByte = 0;
  unsigned char *buf = new unsigned char[chunkSize];
  memset(buf, 0, chunkSize);

  while (end - in >= 3)
  {
    buf[byteAdd++] = base64_table[in[0] >> 2];
    buf[byteAdd++] = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
    buf[byteAdd++] = base64_table[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
    buf[byteAdd++] = base64_table[in[2] & 0x3f];
    dByte += 4;
    if (dByte == 76)
    {
      if(byteAdd + 1 < chunkSize)
      {
        buf[byteAdd++] = 0x0d;
        buf[byteAdd++] = 0x0a;
      }
      dByte = 0;
    }
    if (byteAdd >= chunkSize - 4)
    {
      byteSent += byteAdd;
      client->write(buf, byteAdd);
      memset(buf, 0, chunkSize);
      byteAdd = 0;
    }
    in += 3;
  }

  if (byteAdd > 0)
    client->write(buf, byteAdd);

  if (end - in)
  {
    memset(buf, 0, chunkSize);
    byteAdd = 0;

    buf[byteAdd++] = base64_table[in[0] >> 2];
    if (end - in == 1)
    {
      buf[byteAdd++] = base64_table[(in[0] & 0x03) << 4];
      buf[byteAdd++] = '=';
    }
    else
    {
      buf[byteAdd++] = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
      buf[byteAdd++] = base64_table[(in[1] & 0x0f) << 2];
    }
    buf[byteAdd++] = '=';

    client->write(buf, byteAdd);
    memset(buf, 0, chunkSize);
  }
  delete[] buf;
}

void ESP32_MailClient::send_base64_encode_mime_file(WiFiClient *client, File file)
{

  if (!file)
    return;

  size_t chunkSize = 936;
  size_t byteAdd = 0;
  size_t byteSent = 0;

  unsigned char *buf = new unsigned char[chunkSize];
  memset(buf, 0, chunkSize);

  size_t len = file.size();
  size_t fbufIndex = 0;
  unsigned char *fbuf = new unsigned char[3];

  int dByte = 0;

  while (file.available())
  {
    memset(fbuf, 0, 3);
    if (len - fbufIndex >= 3)
    {
      file.read(fbuf, 3);
      buf[byteAdd++] = base64_table[fbuf[0] >> 2];
      buf[byteAdd++] = base64_table[((fbuf[0] & 0x03) << 4) | (fbuf[1] >> 4)];
      buf[byteAdd++] = base64_table[((fbuf[1] & 0x0f) << 2) | (fbuf[2] >> 6)];
      buf[byteAdd++] = base64_table[fbuf[2] & 0x3f];
      dByte += 4;
      if (dByte == 76)
      {
        if(byteAdd + 1 < chunkSize)
        {
          buf[byteAdd++] = 0x0d;
          buf[byteAdd++] = 0x0a;
        }
        dByte = 0;
      }
      if (byteAdd >= chunkSize - 4)
      {
        byteSent += byteAdd;
        client->write(buf, byteAdd);
        memset(buf, 0, chunkSize);
        byteAdd = 0;
      }
      fbufIndex += 3;
    }
    else
    {
      if (len - fbufIndex == 1)
      {
        fbuf[0] = file.read();
      }
      else if (len - fbufIndex == 2)
      {
        fbuf[0] = file.read();
        fbuf[1] = file.read();
      }
      break;
    }
  }

  file.close();
  if (byteAdd > 0)
    client->write(buf, byteAdd);

  if (len - fbufIndex > 0)
  {
    memset(buf, 0, chunkSize);
    byteAdd = 0;
    buf[byteAdd++] = base64_table[fbuf[0] >> 2];
    if (len - fbufIndex == 1)
    {
      buf[byteAdd++] = base64_table[(fbuf[0] & 0x03) << 4];
      buf[byteAdd++] = '=';
    }
    else
    {
      buf[byteAdd++] = base64_table[((fbuf[0] & 0x03) << 4) | (fbuf[1] >> 4)];
      buf[byteAdd++] = base64_table[(fbuf[1] & 0x0f) << 2];
    }
    buf[byteAdd++] = '=';
    client->write(buf, byteAdd);
  }
  delete[] buf;
  delete[] fbuf;
}

IMAPData::IMAPData() {}
IMAPData::~IMAPData()
{
  empty();
  _net.reset();
  _net.release();
}

void IMAPData::setLogin(const String &host, uint16_t port, const String &loginEmail, const String &loginPassword, const char *rootCA)
{

  _host.clear();
  _port = port;
  _loginEmail.clear();
  _loginPassword.clear();

  _host = host.c_str();
  _loginEmail = loginEmail.c_str();
  _loginPassword = loginPassword.c_str();

  _rootCA.clear();
  if (strlen(rootCA) > 0)
    _rootCA.push_back((char *)rootCA);
}

void IMAPData::setLogin(const String &host, uint16_t port, const String &loginEmail, const String &loginPassword)
{
  _host.clear();
  _port = port;
  _loginEmail.clear();
  _loginPassword.clear();

  _host = host.c_str();
  _loginEmail = loginEmail.c_str();
  _loginPassword = loginPassword.c_str();
}

void IMAPData::setSTARTTLS(bool starttls)
{
  _starttls = starttls;
}

void IMAPData::setDebug(bool debug)
{
  _debug = debug;
}

void IMAPData::setFolder(const String &folderName)
{
  _currentFolder.clear();
  _currentFolder = folderName.c_str();
}
void IMAPData::setMessageBufferSize(size_t size)
{
  _message_buffer_size = size;
}

void IMAPData::setAttachmentSizeLimit(size_t size)
{
  _attacement_max_size = size;
}

void IMAPData::setSearchCriteria(const String &criteria)
{
  _searchCriteria.clear();
  _searchCriteria = criteria.c_str();
}

void IMAPData::setSearchUnseenMessage(bool unseenSearch)
{
  _unseen = unseenSearch;
}

void IMAPData::setSaveFilePath(const String &path)
{
  _savePath.clear();
  if (path.c_str()[0] != '/')
  {
    _savePath = "/";
    _savePath += path.c_str();
  }
  else
    _savePath = path.c_str();
}

void IMAPData::setFetchUID(const String &fetchUID)
{
  _fetchUID.clear();
  string tmp = fetchUID.c_str();
  std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);
  if (tmp.find(ESP32_MAIL_STR_140) != std::string::npos || tmp.find(ESP32_MAIL_STR_212) != std::string::npos ||
      tmp.find(ESP32_MAIL_STR_213) != std::string::npos || tmp.find(ESP32_MAIL_STR_214) != std::string::npos || tmp.find(ESP32_MAIL_STR_215) != std::string::npos ||
      tmp.find(ESP32_MAIL_STR_216) != std::string::npos || tmp.find(ESP32_MAIL_STR_217) != std::string::npos || tmp.find(ESP32_MAIL_STR_218) != std::string::npos ||
      tmp.find(ESP32_MAIL_STR_219) != std::string::npos || tmp.find(ESP32_MAIL_STR_220) != std::string::npos)
    _fetchUID = ESP32_MAIL_STR_183;
  else
    _fetchUID = fetchUID.c_str();

  std::string().swap(tmp);
}

void IMAPData::setFileStorageType(uint8_t storageType)
{
  _storageType = storageType;
}

void IMAPData::setDownloadAttachment(bool download)
{
  _downloadAttachment = download;
}
void IMAPData::setRecentSort(bool recentSort)
{
  _recentSort = recentSort;
}

void IMAPData::setHTMLMessage(bool htmlFormat)
{
  _htmlFormat = htmlFormat;
}
void IMAPData::setTextMessage(bool textFormat)
{
  _textFormat = textFormat;
}

void IMAPData::setSearchLimit(uint16_t limit)
{
  if (limit <= MAX_EMAIL_SEARCH_LIMIT)
    _emailNumMax = limit;
}

bool IMAPData::isHeaderOnly()
{
  return _headerOnly;
}

void IMAPData::saveHTMLMessage(bool download, bool decoded)
{
  _saveDecodedHTML = decoded;
  _saveHTMLMsg = download;
}
void IMAPData::saveTextMessage(bool download, bool decoded)
{
  _saveDecodedText = decoded;
  _saveTextMsg = download;
}

void IMAPData::setReadCallback(readStatusCallback readCallback)
{
  _readCallback = std::move(readCallback);
}

void IMAPData::setDownloadReport(bool report)
{
  _downloadReport = report;
}

uint16_t IMAPData::getFolderCount()
{
  return _folders.size();
}
String IMAPData::getFolder(uint16_t folderIndex)
{
  if (folderIndex < _folders.size())
    return _folders[folderIndex].c_str();
  return std::string().c_str();
}

uint16_t IMAPData::getFlagCount()
{
  return _flag.size();
}
String IMAPData::getFlag(uint16_t flagIndex)
{
  if (flagIndex < _flag.size())
    return _flag[flagIndex].c_str();
  return std::string().c_str();
}

size_t IMAPData::totalMessages()
{
  return _totalMessage;
}

size_t IMAPData::searchCount()
{
  return _searchCount;
}

size_t IMAPData::availableMessages()
{
  return _msgNum.size();
}

size_t IMAPData::getAttachmentCount(uint16_t messageIndex)
{
  if (messageIndex < _msgNum.size())
    return _attachmentCount[messageIndex];
  return 0;
}

String IMAPData::getAttachmentFileName(size_t messageIndex, size_t attachmentIndex)
{
  if (messageIndex < _msgNum.size())
  {
    int s = _messageDataInfo[messageIndex].size();
    int id = 0;
    if (s > 0)
    {
      for (int i = 0; i < s; i++)
      {
        if (_messageDataInfo[messageIndex][i]._disposition == ESP32_MAIL_STR_153)
        {
          if (attachmentIndex == id)
            return _messageDataInfo[messageIndex][i]._filename.c_str();
          id++;
        }
      }
    }
    else
      return std::string().c_str();
  }
  else
    return std::string().c_str();

  return std::string().c_str();
}

String IMAPData::getAttachmentName(size_t messageIndex, size_t attachmentIndex)
{
  if (messageIndex < _msgNum.size())
  {
    int s = _messageDataInfo[messageIndex].size();
    int id = 0;
    if (s > 0)
    {
      for (int i = 0; i < s; i++)
      {
        if (_messageDataInfo[messageIndex][i]._disposition == ESP32_MAIL_STR_153)
        {
          if (attachmentIndex == id)
            return _messageDataInfo[messageIndex][i]._name.c_str();
          id++;
        }
      }
    }
    else
      return std::string().c_str();
  }
  else
    return std::string().c_str();

  return std::string().c_str();
}

int IMAPData::getAttachmentFileSize(size_t messageIndex, size_t attachmentIndex)
{
  if (messageIndex < _msgNum.size())
  {
    int s = _messageDataInfo[messageIndex].size();
    int id = 0;
    if (s > 0)
    {
      for (int i = 0; i < s; i++)
      {
        if (_messageDataInfo[messageIndex][i]._disposition == ESP32_MAIL_STR_153)
        {
          if (attachmentIndex == id)
            return _messageDataInfo[messageIndex][i]._size;
          id++;
        }
      }
    }
    else
      return 0;
  }
  else
    return 0;

  return 0;
}

String IMAPData::getAttachmentCreationDate(size_t messageIndex, size_t attachmentIndex)
{
  if (messageIndex < _msgNum.size())
  {
    int s = _messageDataInfo[messageIndex].size();
    int id = 0;
    if (s > 0)
    {
      for (int i = 0; i < s; i++)
      {
        if (_messageDataInfo[messageIndex][i]._disposition == ESP32_MAIL_STR_153)
        {
          if (attachmentIndex == id)
            return _messageDataInfo[messageIndex][i]._creation_date.c_str();
          id++;
        }
      }
    }
    else
      return std::string().c_str();
  }
  else
    return std::string().c_str();

  return std::string().c_str();
}

String IMAPData::getAttachmentType(size_t messageIndex, size_t attachmentIndex)
{
  if (messageIndex < _msgNum.size())
  {
    int s = _messageDataInfo[messageIndex].size();
    int id = 0;
    if (s > 0)
    {
      for (int i = 0; i < s; i++)
      {
        if (_messageDataInfo[messageIndex][i]._disposition == ESP32_MAIL_STR_153)
        {
          if (attachmentIndex == id)
            return _messageDataInfo[messageIndex][i]._contentType.c_str();
          id++;
        }
      }
    }
    else
      return std::string().c_str();
  }
  else
    return std::string().c_str();

  return std::string().c_str();
}

String IMAPData::getFrom(uint16_t messageIndex)
{
  if (messageIndex < _msgNum.size())
    return _from[messageIndex].c_str();
  return std::string().c_str();
}

String IMAPData::getFromCharset(uint16_t messageIndex)
{
  if (messageIndex < _msgNum.size())
    return _from_charset[messageIndex].c_str();
  return std::string().c_str();
}
String IMAPData::getTo(uint16_t messageIndex)
{
  if (messageIndex < _msgNum.size())
    return _to[messageIndex].c_str();
  return std::string().c_str();
}
String IMAPData::getToCharset(uint16_t messageIndex)
{
  if (messageIndex < _msgNum.size())
    return _to_charset[messageIndex].c_str();
  return std::string().c_str();
}
String IMAPData::getCC(uint16_t messageIndex)
{
  if (messageIndex < _msgNum.size())
    return _cc[messageIndex].c_str();
  return std::string().c_str();
}
String IMAPData::getCCCharset(uint16_t messageIndex)
{
  if (messageIndex < _msgNum.size())
    return _cc_charset[messageIndex].c_str();
  return std::string().c_str();
}

String IMAPData::getSubject(uint16_t messageIndex)
{
  if (messageIndex < _msgNum.size())
    return _subject[messageIndex].c_str();
  return std::string().c_str();
}
String IMAPData::getSubjectCharset(uint16_t messageIndex)
{
  if (messageIndex < _msgNum.size())
    return _subject_charset[messageIndex].c_str();
  return std::string().c_str();
}
String IMAPData::getHTMLMessage(uint16_t messageIndex)
{
  if (messageIndex < _msgNum.size())
    return getMessage(messageIndex, true);
  return std::string().c_str();
}

String IMAPData::getTextMessage(uint16_t messageIndex)
{
  if (messageIndex < _msgNum.size())
    return getMessage(messageIndex, false);
  return std::string().c_str();
}

String IMAPData::getMessage(uint16_t messageIndex, bool htmlFormat)
{
  if (messageIndex < _msgNum.size())
  {
    int s = _messageDataInfo[messageIndex].size();

    if (s > 0)
    {
      for (int i = 0; i < s; i++)
      {
        if (_messageDataInfo[messageIndex][i]._contentType == ESP32_MAIL_STR_155 && !htmlFormat)
          return _messageDataInfo[messageIndex][i]._text.c_str();
        else if (_messageDataInfo[messageIndex][i]._contentType == ESP32_MAIL_STR_154 && htmlFormat)
          return _messageDataInfo[messageIndex][i]._text.c_str();
      }
      return std::string().c_str();
    }
    else
      return std::string().c_str();
  }
  else
    return std::string().c_str();

  return std::string().c_str();
}

String IMAPData::getHTMLMessgaeCharset(uint16_t messageIndex)
{
  if (messageIndex < _msgNum.size())
  {
    int s = _messageDataInfo[messageIndex].size();

    if (s > 0)
    {
      for (int i = 0; i < s; i++)
      {
        if (_messageDataInfo[messageIndex][i]._contentType == ESP32_MAIL_STR_154)
          return _messageDataInfo[messageIndex][i]._charset.c_str();
      }
      return std::string().c_str();
    }
    else
      return std::string().c_str();
  }
  else
    return std::string().c_str();

  return std::string().c_str();
}

String IMAPData::getTextMessgaeCharset(uint16_t messageIndex)
{
  if (messageIndex < _msgNum.size())
  {
    int s = _messageDataInfo[messageIndex].size();

    if (s > 0)
    {
      for (int i = 0; i < s; i++)
      {
        if (_messageDataInfo[messageIndex][i]._contentType == ESP32_MAIL_STR_155)
          return _messageDataInfo[messageIndex][i]._charset.c_str();
      }
      return std::string().c_str();
    }
    else
      return std::string().c_str();
  }
  else
    return std::string().c_str();

  return std::string().c_str();
}

String IMAPData::getDate(uint16_t messageIndex)
{
  if (messageIndex < _msgNum.size())
    return _date[messageIndex].c_str();
  return std::string().c_str();
}

String IMAPData::getUID(uint16_t messageIndex)
{
  char *buf = new char[50];
  memset(buf, 0, 50);
  if (_uidSearch)
  {
    if (messageIndex < _msgNum.size())
      itoa(_msgNum[messageIndex], buf, 10);
  }

  String v = buf;
  delete[] buf;
  return v;
}

String IMAPData::getNumber(uint16_t messageIndex)
{
  char *buf = new char[50];
  memset(buf, 0, 50);

  if (messageIndex < _msgNum.size())
  {
    if (!_uidSearch)
      itoa(_msgNum[messageIndex], buf, 10);
    else
      itoa(_msgNum[messageIndex] + 1, buf, 10);
  }

  String v = buf;
  delete[] buf;
  return v;
}

String IMAPData::getMessageID(uint16_t messageIndex)
{
  if (messageIndex < _msgNum.size())
    return _msgID[messageIndex].c_str();
  return std::string().c_str();
}

String IMAPData::getAcceptLanguage(uint16_t messageIndex)
{
  if (messageIndex < _msgNum.size())
    return _acceptLanguage[messageIndex].c_str();
  return std::string().c_str();
}
String IMAPData::getContentLanguage(uint16_t messageIndex)
{
  if (messageIndex < _msgNum.size())
    return _contentLanguage[messageIndex].c_str();
  return std::string().c_str();
}

bool IMAPData::isFetchMessageFailed(uint16_t messageIndex)
{
  if (messageIndex < _msgNum.size())
    return _error[messageIndex];
  return false;
}
String IMAPData::getFetchMessageFailedReason(uint16_t messageIndex)
{
  if (messageIndex < _msgNum.size())
    return _errorMsg[messageIndex].c_str();
  return std::string().c_str();
}

bool IMAPData::isDownloadAttachmentFailed(uint16_t messageIndex, size_t attachmentIndex)
{
  if (messageIndex < _msgNum.size())
  {
    int s = _messageDataInfo[messageIndex].size();
    int id = 0;
    if (s > 0)
    {
      for (int i = 0; i < s; i++)
      {
        if (_messageDataInfo[messageIndex][i]._disposition == ESP32_MAIL_STR_153)
        {
          if (attachmentIndex == id)
            return _messageDataInfo[messageIndex][i]._error;
          id++;
        }
      }
    }
    else
      return false;
  }
  else
    return false;

  return false;
}

String IMAPData::getDownloadAttachmentFailedReason(uint16_t messageIndex, size_t attachmentIndex)
{
  if (messageIndex < _msgNum.size())
  {
    int s = _messageDataInfo[messageIndex].size();
    int id = 0;
    if (s > 0)
    {
      for (int i = 0; i < s; i++)
      {
        if (_messageDataInfo[messageIndex][i]._disposition == ESP32_MAIL_STR_153)
        {
          if (attachmentIndex == id)
            return _messageDataInfo[messageIndex][i]._downloadError.c_str();
          id++;
        }
      }
    }
    else
      return std::string().c_str();
  }
  else
    return std::string().c_str();
  return std::string().c_str();
}

bool IMAPData::isDownloadMessageFailed(uint16_t messageIndex)
{
  if (messageIndex < _msgNum.size())
  {
    int s = _messageDataInfo[messageIndex].size();
    bool res = false;
    if (s > 0)
    {
      for (int i = 0; i < s; i++)
      {
        if (_messageDataInfo[messageIndex][i]._disposition == "")
        {
          res |= _messageDataInfo[messageIndex][i]._error;
        }
      }

      return res;
    }
    else
      return false;
  }
  else
    return false;

  return false;
}
String IMAPData::getDownloadMessageFailedReason(uint16_t messageIndex)
{
  if (messageIndex < _msgNum.size())
  {
    int s = _messageDataInfo[messageIndex].size();
    string res = "";
    if (s > 0)
    {
      for (int i = 0; i < s; i++)
      {
        if (_messageDataInfo[messageIndex][i]._disposition == "")
        {
          if (_messageDataInfo[messageIndex][i]._downloadError != "")
            res = _messageDataInfo[messageIndex][i]._downloadError;
        }
      }

      return res.c_str();
    }
    else
      return std::string().c_str();
  }
  else
    return std::string().c_str();

  return std::string().c_str();
}

void IMAPData::empty()
{
  std::string().swap(_host);
  std::string().swap(_loginEmail);
  std::string().swap(_loginPassword);
  std::string().swap(_currentFolder);
  std::string().swap(_nextUID);
  std::string().swap(_searchCriteria);
  std::vector<std::string>().swap(_date);
  std::vector<std::string>().swap(_subject);
  std::vector<std::string>().swap(_subject_charset);
  std::vector<std::string>().swap(_from);
  std::vector<std::string>().swap(_from_charset);
  std::vector<std::string>().swap(_to);
  std::vector<std::string>().swap(_to_charset);
  std::vector<std::string>().swap(_cc);
  std::vector<std::string>().swap(_cc_charset);
  std::vector<uint32_t>().swap(_msgNum);
  std::vector<std::string>().swap(_folders);
  std::vector<std::string>().swap(_flag);
  std::vector<std::string>().swap(_msgID);
  std::vector<std::string>().swap(_acceptLanguage);
  std::vector<std::string>().swap(_contentLanguage);
  std::vector<int>().swap(_attachmentCount);
  std::vector<int>().swap(_totalAttachFileSize);
  std::vector<int>().swap(_downloadedByte);
  std::vector<bool>().swap(_error);
  std::vector<std::vector<messageBodyData>>().swap(_messageDataInfo);
  std::vector<std::string>().swap(_errorMsg);
}

void IMAPData::clearMessageData()
{
  std::vector<std::string>().swap(_date);
  std::vector<std::string>().swap(_subject);
  std::vector<std::string>().swap(_subject_charset);
  std::vector<std::string>().swap(_from);
  std::vector<std::string>().swap(_from_charset);
  std::vector<std::string>().swap(_to);
  std::vector<std::string>().swap(_to_charset);
  std::vector<std::string>().swap(_cc);
  std::vector<std::string>().swap(_cc_charset);
  std::vector<uint32_t>().swap(_msgNum);
  std::vector<std::string>().swap(_msgID);
  std::vector<std::string>().swap(_contentLanguage);
  std::vector<std::string>().swap(_acceptLanguage);
  std::vector<std::string>().swap(_folders);
  std::vector<std::string>().swap(_flag);
  std::vector<int>().swap(_attachmentCount);
  std::vector<std::vector<messageBodyData>>().swap(_messageDataInfo);
  std::vector<int>().swap(_totalAttachFileSize);
  std::vector<int>().swap(_downloadedByte);
  std::vector<int>().swap(_messageDataCount);
  std::vector<std::string>().swap(_errorMsg);
  std::vector<bool>().swap(_error);
  _searchCount = 0;
}

messageBodyData::messageBodyData()
{
}
messageBodyData::~messageBodyData()
{
  empty();
}

void messageBodyData::empty()
{
  std::string().swap(_text);
  std::string().swap(_filename);
  std::string().swap(_savePath);
  std::string().swap(_name);
  std::string().swap(_disposition);
  std::string().swap(_contentType);
  std::string().swap(_descr);
  std::string().swap(_transfer_encoding);
  std::string().swap(_creation_date);
  std::string().swap(_modification_date);
  std::string().swap(_charset);
  std::string().swap(_part);
  std::string().swap(_downloadError);
}

attachmentData::attachmentData() {}
attachmentData::~attachmentData()
{

  std::vector<std::vector<uint8_t *>>().swap(_buf);
  std::vector<std::string>().swap(_filename);
  std::vector<uint8_t>().swap(_id);
  std::vector<uint8_t>().swap(_type);
  std::vector<size_t>().swap(_size);
  std::vector<std::string>().swap(_mime_type);
}

void attachmentData::add(const String &fileName, const String &mimeType, uint8_t *data, size_t size)
{
  _filename.push_back(fileName.c_str());
  _mime_type.push_back(mimeType.c_str());

  if (size > 0)
  {
    std::vector<uint8_t *> d = std::vector<uint8_t *>();
    d.push_back(data);
    _buf.push_back(d);
    _size.push_back(size);
    _type.push_back(0);
  }
  else
  {
    _buf.push_back(std::vector<uint8_t *>());
    _size.push_back(0);
    _type.push_back(1);
  }

  _id.push_back(_index);
  _index++;
}

void attachmentData::remove(uint8_t index)
{
  _buf.erase(_buf.begin() + index);
  _filename.erase(_filename.begin() + index);
  _type.erase(_type.begin() + index);
  _size.erase(_size.begin() + index);
  _mime_type.erase(_mime_type.begin() + index);
  _id.erase(_id.begin() + index);
}

void attachmentData::free()
{
  std::vector<std::vector<uint8_t *>>().swap(_buf);
  std::vector<std::string>().swap(_filename);
  std::vector<uint8_t>().swap(_id);
  std::vector<uint8_t>().swap(_type);
  std::vector<size_t>().swap(_size);
  std::vector<std::string>().swap(_mime_type);
  _index = 0;
}

String attachmentData::getFileName(uint8_t index)
{
  return _filename[index].c_str();
}

String attachmentData::getMimeType(uint8_t index)
{
  return _mime_type[index].c_str();
}

uint8_t *attachmentData::getData(uint8_t index)
{
  uint8_t *ptr = _buf[index].front();
  return ptr;
}

uint16_t attachmentData::getSize(uint8_t index)
{
  return _size[index];
}

uint8_t attachmentData::getCount()
{
  return _index;
}

uint8_t attachmentData::getType(uint8_t index)
{
  return _type[index];
}

SMTPData::SMTPData() {}

SMTPData::~SMTPData()
{
  empty();
  _net.reset();
  _net.release();
}

void SMTPData::setLogin(const String &host, uint16_t port, const String &loginEmail, const String &loginPassword, const char *rootCA)
{

  _host.clear();
  _port = port;
  _loginEmail.clear();
  _loginPassword.clear();

  _host = host.c_str();
  _loginEmail = loginEmail.c_str();
  _loginPassword = loginPassword.c_str();

  _rootCA.clear();
  if (strlen(rootCA) > 0)
    _rootCA.push_back((char *)rootCA);
}

void SMTPData::setLogin(const String &host, uint16_t port, const String &loginEmail, const String &loginPassword)
{

  _host.clear();
  _port = port;
  _loginEmail.clear();
  _loginPassword.clear();

  _host = host.c_str();
  _loginEmail = loginEmail.c_str();
  _loginPassword = loginPassword.c_str();

  _rootCA.clear();
}

void SMTPData::setSTARTTLS(bool starttls)
{
  _starttls = starttls;
}

void SMTPData::setDebug(bool debug)
{
  _debug = debug;
}

void SMTPData::setSender(const String &fromName, const String &senderEmail)
{

  _fromName.clear();
  _senderEmail.clear();

  _fromName += fromName.c_str();
  _senderEmail += senderEmail.c_str();
}

String SMTPData::getFromName()
{
  return _fromName.c_str();
}

String SMTPData::getSenderEmail()
{
  return _senderEmail.c_str();
}

void SMTPData::setPriority(int priority)
{
  _priority = priority;
}
void SMTPData::setPriority(const String &priority)
{
  if (priority == ESP32_MAIL_STR_205 || priority == ESP32_MAIL_STR_206)
    _priority = 1;
  else if (priority == ESP32_MAIL_STR_207 || priority == ESP32_MAIL_STR_208)
    _priority = 3;
  else if (priority == ESP32_MAIL_STR_209 || priority == ESP32_MAIL_STR_210)
    _priority = 5;
}

uint8_t SMTPData::getPriority()
{
  return _priority;
}

void SMTPData::addRecipient(const String &email)
{
  _recipient.insert(_recipient.end(), email.c_str());
}

void SMTPData::removeRecipient(const String &email)
{
  for (uint8_t i = 0; i < _recipient.size(); i++)
    if (_recipient[i].c_str() == email.c_str())
      _recipient.erase(_recipient.begin() + i);
}

void SMTPData::removeRecipient(uint8_t index)
{
  _recipient.erase(_recipient.begin() + index);
}

void SMTPData::clearRecipient()
{
  std::vector<std::string>().swap(_recipient);
}

uint8_t SMTPData::recipientCount()
{
  return _recipient.size();
}

String SMTPData::getRecipient(uint8_t index)
{
  if (index >= _recipient.size())
    return std::string().c_str();
  return _recipient[index].c_str();
}

void SMTPData::setSubject(const String &subject)
{
  _subject = subject.c_str();
}

String SMTPData::getSubject()
{
  return _subject.c_str();
}

void SMTPData::setMessage(const String &message, bool htmlFormat)
{
  _message.clear();
  _message += message.c_str();
  _htmlFormat = htmlFormat;
}

void SMTPData::clrMessage(bool htmlFormat)
{
  _message.clear();
  _htmlFormat = htmlFormat;
}

void SMTPData::addMessage(const String &message)
{
  _message += message.c_str();
}

String SMTPData::getMessage()
{
  return _message.c_str();
}

bool SMTPData::htmlFormat()
{
  return _htmlFormat;
}
void SMTPData::addCC(const String &email)
{
  _cc.push_back(email.c_str());
}

void SMTPData::removeCC(const String &email)
{
  for (uint8_t i = 0; i < _cc.size(); i++)
    if (_cc[i].c_str() == email.c_str())
      _cc.erase(_cc.begin() + i);
}

void SMTPData::removeCC(uint8_t index)
{
  _cc.erase(_cc.begin() + index);
}
void SMTPData::clearCC()
{
  std::vector<std::string>().swap(_cc);
}

uint8_t SMTPData::ccCount()
{
  return _cc.size();
}

String SMTPData::getCC(uint8_t index)
{
  if (index >= _cc.size())
    return std::string().c_str();
  return _cc[index].c_str();
}

void SMTPData::addBCC(const String &email)
{
  _bcc.push_back(email.c_str());
}

void SMTPData::removeBCC(const String &email)
{
  for (uint8_t i = 0; i < _bcc.size(); i++)
    if (_bcc[i].c_str() == email.c_str())
      _bcc.erase(_bcc.begin() + i);
}

void SMTPData::removeBCC(uint8_t index)
{
  _bcc.erase(_bcc.begin() + index);
}

void SMTPData::clearBCC()
{
  std::vector<std::string>().swap(_bcc);
}

uint8_t SMTPData::bccCount()
{
  return _bcc.size();
}

String SMTPData::getBCC(uint8_t index)
{
  if (index >= _bcc.size())
    return std::string().c_str();
  return _bcc[index].c_str();
}

void SMTPData::addAttachData(const String &fileName, const String &mimeType, uint8_t *data, size_t size)
{
  _attach.add(fileName, mimeType, data, size);
}

void SMTPData::removeAttachData(const String &fileName)
{
  for (uint8_t i = 0; i < _attach.getCount(); i++)
    if (_attach.getFileName(i) == fileName && _attach.getType(i) == 0)
    {
      _attach.remove(i);
    }
}

void SMTPData::removeAttachData(uint8_t index)
{
  uint8_t id = 0;
  for (uint8_t i = 0; i < _attach.getCount(); i++)
    if (_attach.getType(i) == 0)
    {
      if (id == index)
      {
        _attach.remove(i);
        break;
      }
      id++;
    }
}

uint8_t SMTPData::attachDataCount()
{
  uint8_t count = 0;
  for (uint8_t i = 0; i < _attach.getCount(); i++)
    if (_attach.getType(i) == 0)
      count++;

  return count;
}

void SMTPData::addAttachFile(const String &filePath, const String &mimeType)
{
  _attach.add(filePath, mimeType, NULL, 0);
}

void SMTPData::removeAttachFile(const String &filePath)
{
  for (uint8_t i = 0; i < _attach.getCount(); i++)
    if (_attach.getFileName(i) == filePath && _attach.getType(i) == 1)
    {
      _attach.remove(i);
    }
}

void SMTPData::removeAttachFile(uint8_t index)
{
  uint8_t id = 0;
  for (uint8_t i = 0; i < _attach.getCount(); i++)
    if (_attach.getType(i) == 1)
    {
      if (id == index)
      {
        _attach.remove(i);
        break;
      }
      id++;
    }
}

void SMTPData::setFileStorageType(uint8_t storageType)
{
  _storageType = storageType;
}

void SMTPData::clearAttachData()
{
  for (uint8_t i = 0; i < _attach.getCount(); i++)
    if (_attach.getType(i) == 0)
      _attach.remove(i);
}

void SMTPData::clearAttachFile()
{
  for (uint8_t i = 0; i < _attach.getCount(); i++)
    if (_attach.getType(i) == 1)
      _attach.remove(i);
}

void SMTPData::clearAttachment()
{
  _attach.free();
}

uint8_t SMTPData::attachFileCount()
{
  uint8_t count = 0;
  for (uint8_t i = 0; i < _attach.getCount(); i++)
    if (_attach.getType(i) == 1)
      count++;

  return count;
}

void SMTPData::addCustomMessageHeader(const String &commmand)
{
  _customMessageHeader.insert(_customMessageHeader.end(), commmand.c_str());
}

void SMTPData::removeCustomMessageHeader(const String &commmand)
{
  for (uint8_t i = 0; i < _customMessageHeader.size(); i++)
    if (_customMessageHeader[i].c_str() == commmand.c_str())
      _customMessageHeader.erase(_customMessageHeader.begin() + i);
}

void SMTPData::removeCustomMessageHeader(uint8_t index)
{
  _customMessageHeader.erase(_customMessageHeader.begin() + index);
}

void SMTPData::clearCustomMessageHeader()
{
  std::vector<std::string>().swap(_customMessageHeader);
}

uint8_t SMTPData::CustomMessageHeaderCount()
{
  return _customMessageHeader.size();
}

String SMTPData::getCustomMessageHeader(uint8_t index)
{
  if (index >= _customMessageHeader.size())
    return std::string().c_str();
  return _customMessageHeader[index].c_str();
}

void SMTPData::empty()
{
  std::string().swap(_host);
  std::string().swap(_loginEmail);
  std::string().swap(_loginPassword);
  std::string().swap(_fromName);
  std::string().swap(_senderEmail);
  std::string().swap(_subject);
  std::string().swap(_message);
  clearRecipient();
  clearCustomMessageHeader();
  clearCC();
  clearBCC();
  clearAttachment();
}

void SMTPData::setSendCallback(sendStatusCallback sendCallback)
{
  _sendCallback = std::move(sendCallback);
}

ReadStatus::ReadStatus()
{
}
ReadStatus::~ReadStatus()
{
  empty();
}

String ReadStatus::status()
{
  return _status.c_str();
}
String ReadStatus::info()
{
  return _info.c_str();
}

bool ReadStatus::success()
{
  return _success;
}
void ReadStatus::empty()
{
  std::string().swap(_info);
  std::string().swap(_status);
}

SendStatus::SendStatus()
{
}

SendStatus::~SendStatus()
{
  empty();
}

String SendStatus::info()
{
  return _info.c_str();
}
bool SendStatus::success()
{
  return _success;
}
void SendStatus::empty()
{
  std::string().swap(_info);
}

ESP32_MailClient MailClient = ESP32_MailClient();

#endif //ESP32

#endif //ESP32_MailClient_CPP
