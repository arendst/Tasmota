/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "str_build.h"
#include "coc_string.h"
#include <sstream>
#include <fstream>

str_build::str_build(std::map<std::string, int> map)
{
    size_t size = map.size() / 2;
    m_count = map.size();
    m_hashtable.resize(size < 4 ? 4 : size);
    for (auto it : map) {
        make_ceil(it.first, it.second);
    }
    keywords();
}

str_build::~str_build()
{
}

void str_build::build(const std::string &path)
{
    std::string prefix(path + "/be_const_strtab");
    writefile(prefix + "_def.h", build_table_def());
    writefile(prefix + ".h", build_table_ext());
}

void str_build::keywords()
{
    const int opif = 50; /* note the definition in be_lexer.h */
    const static std::map<std::string, int> tab = {
        { "if", opif}, { "elif", opif + 1 },
        { "else", opif + 2 }, { "while", opif + 3 },
        { "for", opif + 4 }, { "def", opif + 5 },
        { "end", opif + 6 }, { "class", opif + 7 },
        { "break", opif + 8 }, { "continue", opif + 9 },
        { "return", opif + 10 }, { "true", opif + 11 },
        { "false", opif + 12 }, { "nil", opif + 13 },
        { "var", opif + 14 }, { "do", opif + 15 },
        { "import", opif + 16 }, { "as", opif + 17 },
        { "try", opif + 18 }, { "except", opif + 19 },
        { "raise", opif + 20 }
    };
    for (auto it : tab) {
        make_ceil(it.first, it.second);
    }
}

void str_build::make_ceil(const std::string &string, int extra)
{
    str_info info;
    info.hash = coc::hashcode(string);
    info.str = string;
    info.extra = extra;
    m_hashtable[info.hash % m_hashtable.size()].push_back(info);
}

void str_build::writefile(const std::string &filename, const std::string &text)
{
    std::ostringstream buf;
    std::ifstream fin(filename);
    buf << fin.rdbuf();
    if (buf.str() != text) {
        std::ofstream fout;
        fout.open(filename, std::ios::out);
        fout << text;
        fout.close();
    }
}

std::string str_build::build_table_def()
{
    std::ostringstream ostr;
    for (auto bucket : m_hashtable) {
        size_t size = bucket.size();
        for (size_t i = 0; i < size; ++i) {
            str_info info = bucket[i];
            std::string node = coc::escape_operator(info.str);
            std::string next = i < size - 1 ?
                "&be_const_str_" + coc::escape_operator(bucket[i + 1].str) :
                "NULL";
            ostr << "be_define_const_str("
                 << node << ", \"" << info.str << "\", "
                 << info.hash << "u, " << info.extra << ", "
                 << info.str.size() << ", " << next << ");"
                 << std::endl;
        }
    }
    ostr << std::endl;
    ostr << "static const bstring* const m_string_table[] = {" << std::endl;
    size_t size = m_hashtable.size();
    for (size_t i = 0; i < size; ++i) {
        auto bucket = m_hashtable[i];
        if (bucket.size()) {
            ostr << "    (const bstring *)&be_const_str_"
                 << coc::escape_operator(bucket[0].str);
        } else {
            ostr << "    NULL";
        }
        ostr << (i < size - 1 ? "," : "") << std::endl;
    }
    ostr << "};" << std::endl << std::endl;
    ostr <<
        "static const struct bconststrtab m_const_string_table = {\n"
        "    .size = " << size << ",\n" <<
        "    .count = " << m_count << ",\n" <<
        "    .table = m_string_table\n" <<
        "};" << std::endl;
    return ostr.str();
}

std::string str_build::build_table_ext()
{
    std::ostringstream ostr;
    for (auto bucket : m_hashtable) {
        for (auto info : bucket) {
            ostr << "extern const bcstring be_const_str_"
                << coc::escape_operator(info.str) << ";" << std::endl;
        }
    }
    return ostr.str();
}
