/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#ifndef __BUILD_MAP_H
#define __BUILD_MAP_H

#include <map>
#include <string>
#include <vector>

class str_build
{
public:
    str_build(std::map<std::string, int> map);
    ~str_build();
    void build(const std::string &path);

private:
    void keywords();
    void make_ceil(const std::string &string, int extra = 0);
    std::string build_table_def();
    std::string build_table_ext();
    void writefile(const std::string &filename, const std::string &text);

private:
    struct str_info {
        uint32_t hash;
        std::string str;
        int extra;
    };
    size_t m_count;
    std::vector< std::vector<str_info> > m_hashtable;
};

#endif
