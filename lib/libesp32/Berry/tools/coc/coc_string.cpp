/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "coc_string.h"
#include <map>

namespace coc {

uint32_t hashcode(const std::string &string)
{
    size_t len = string.size();
    const char *str = string.data();
    uint32_t hash = 2166136261u;
    while (len--)
        hash = (hash ^ (unsigned char)*str++) * 16777619u;
    return hash;
}

std::string escape_operator(const std::string &string)
{
    int c = string[0];
    if (string == "..")
        return "opt_connect";
    if (c == '.')
        return "dot_" + string.substr(1);
    if (isalpha(c) || c == '_')
        return string;
    const static std::map<std::string, std::string> tab = {
        { "+", "opt_add" }, { "-", "opt_sub" },
        { "*", "opt_mul" }, { "/", "opt_div" },
        { "%", "opt_mod" }, { "&", "opt_and" },
        { "^", "opt_xor" }, { "|", "opt_or" },
        { "<", "opt_lt" }, { ">", "opt_gt" },
        { "<=", "opt_le" }, { ">=", "opt_ge" },
        { "==", "opt_eq" }, { "!=", "opt_neq" },
        { "<<", "opt_shl" }, { ">>", "opt_shr" },
        { "-*", "opt_neg" }, { "~", "opt_flip" },
        { "()", "opt_call" }
    };
    auto it = tab.find(string);
    return it != tab.end() ? it->second : string;
}

}
