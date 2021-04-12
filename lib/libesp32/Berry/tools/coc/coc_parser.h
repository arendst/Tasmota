/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#ifndef __COC_PARSER_H
#define __COC_PARSER_H

#include <string>
#include <vector>
#include "object_block.h"

class coc_parser {
public:
    coc_parser(const std::string &text);
    const std::vector<object_block>& objects() const;
    const std::vector<std::string>& strtab() const;

private:
    void scan_const_string();
    void skip_space();
    void skip_char(int c) {
        parse_char(c, true);
    }
    bool parse_char_base(int c, bool necessary);
    bool parse_char(int c, bool necessary = false);
    bool parse_char_continue(int c, bool necessary = false);
    std::string parse_word();
    std::string parse_tocomma();
    std::string parse_tonewline();
    void parse_object();
    void parse_block(object_block *object);
    void parse_attr(object_block *object);
    void parse_attr_pair(object_block *object);
    void parse_body(object_block *object);
    void parse_body_item(object_block *object);

private:
    const char *m_ptr;
    std::vector<object_block> m_objects;
    std::vector<std::string> m_strtab;
};

#endif // !__COC_PARSER_H
