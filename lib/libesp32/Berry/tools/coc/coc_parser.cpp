/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "coc_parser.h"
#include <string.h>

static inline int _isalnum(int c)
{
    return isalnum(c) || c == '_';
}

coc_parser::coc_parser(const std::string &text)
{
    m_ptr = text.c_str();
    while (*m_ptr) {
        switch (*m_ptr) {
        case '@':
            parse_object();
            break;
        case 'b':
            scan_const_string();
            break;
        default:
            ++m_ptr;
        }
    }
}

const std::vector<object_block>& coc_parser::objects() const
{
    return m_objects;
}

const std::vector<std::string>& coc_parser::strtab() const
{
    return m_strtab;
}

void coc_parser::scan_const_string()
{
    const char prefix[] = "be_const_str_";
    const size_t len = sizeof(prefix) - 1;
    if (!strncmp(m_ptr, prefix, len)) {
        m_ptr += len;
        const char *p = m_ptr;
        while (_isalnum(*m_ptr))
            ++m_ptr;
        m_strtab.push_back(std::string(p, m_ptr - p));
    } else {
        ++m_ptr;
    }
}

void coc_parser::skip_space()
{
    while (isspace(*m_ptr))
        ++m_ptr;
}

bool coc_parser::parse_char_base(int c, bool necessary)
{
    bool res = *m_ptr == c;
    if (!res && necessary)
        throw "error";
    if (res)
        ++m_ptr;
    return res;
}

bool coc_parser::parse_char(int c, bool necessary)
{
    skip_space();
    return parse_char_base(c, necessary);
}

bool coc_parser::parse_char_continue(int c, bool necessary)
{
    int ch;
    while (((ch = *m_ptr) == ' ') || ch == '\t')
        ++m_ptr;
    return parse_char_base(c, necessary);
}

std::string coc_parser::parse_word()
{
    skip_space();
    const char *p = m_ptr;
    if (_isalnum(*m_ptr)) {
        while (_isalnum(*(++m_ptr)));
        return std::string(p, m_ptr - p);
    }
    throw "error";
}

std::string coc_parser::parse_tocomma()
{
    int c;
    skip_space();
    const char *p = m_ptr;
    while (((c = *m_ptr) != ',') && !isspace(c))
        ++m_ptr;
    if (p == m_ptr)
        throw "error";
    return std::string(p, m_ptr - p);
}

std::string coc_parser::parse_tonewline()
{
    int c;
    skip_space();
    const char *p = m_ptr;
    while (((c = *m_ptr) != '\r') && c != '\n')
        ++m_ptr;
    if (p == m_ptr)
        throw "error";
    return std::string(p, m_ptr - p);
}

void coc_parser::parse_object()
{
    const char begin_text[] = "@const_object_info_begin";
    const size_t begin_len = sizeof(begin_text) - 1;
    if (!strncmp(m_ptr, begin_text, begin_len)) {
        m_ptr += begin_len;
        do {
            object_block object;
            parse_block(&object);
            m_objects.push_back(object);
        } while (!parse_char('@'));
        const char end_text[] = "const_object_info_end";
        const size_t end_len = sizeof(end_text) - 1;
        if (strncmp(m_ptr, end_text, end_len))
            throw "error";
        m_ptr += end_len;
    } else {
        ++m_ptr;
    }
}

void coc_parser::parse_block(object_block *object)
{
    object->type = parse_word();
    object->name = parse_word();
    parse_attr(object);
    parse_body(object);
}

void coc_parser::parse_attr(object_block *object)
{
    skip_char('(');
    parse_attr_pair(object);
    while (parse_char(',')) {
        parse_attr_pair(object);
    }
    skip_char(')');
}

void coc_parser::parse_attr_pair(object_block *object)
{
    std::string key = parse_word();
    skip_char(':');
    std::string value = parse_word();
    object->attr[key] = value;
}

void coc_parser::parse_body(object_block *object)
{
    skip_char('{');
    if (!parse_char('}')) {
        do {
            parse_body_item(object);
        } while (!parse_char('}'));
    }
}

void coc_parser::parse_body_item(object_block *object)
{
    object_block::data_value value;
    std::string key = parse_tocomma();
    parse_char_continue(',', true);
    value.value = parse_tocomma();
    if (parse_char_continue(','))
        value.depend = parse_tonewline();
    object->data[key] = value;
}
