/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#ifndef __MAIN_H
#define __MAIN_H

#include <string>
#include <vector>
#include <map>

class macro_table;

class builder {
public:
    builder(int argc, char **argv);
    ~builder();
    void build();

private:
    void push_strtab(const std::vector<std::string>& list);
    void add_arg(const std::string& arg);
    std::string info_block(const std::string &text);
    void parse_all(const std::string &filename);
    void scandir(const std::string &srcpath);
    std::string readfile(const std::string &filename);

private:
    enum arg_state {
        Input,
        Output,
        Config
    };
    std::string m_output;
    std::vector<std::string> m_input;
    std::vector<std::string> m_config;
    arg_state m_state;
    macro_table *m_macro;
    std::map<std::string, int> m_strmap;
};

#endif
