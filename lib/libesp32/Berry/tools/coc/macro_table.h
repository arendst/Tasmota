/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#ifndef __MACRO_TABLE_H
#define __MACRO_TABLE_H

#include <map>
#include <string>
#include <vector>

class macro_table {
public:
    macro_table() {}
    void scan_file(const std::string &filename);
    bool query(const std::string &str) const;
    std::map<std::string, int> table() const;

private:
    std::string readfile(const std::string &filename);
    int parse_value(std::string str);

private:
    std::map<std::string, int> m_map;
};

#endif
