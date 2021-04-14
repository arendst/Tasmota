/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "main.h"
#include "block_builder.h"
#include "coc_parser.h"
#include "macro_table.h"
#include "str_build.h"
#include <fstream>
#include <sstream>

void builder::parse_all(const std::string &filename)
{
    size_t pos = filename.find_last_of(".");
    std::string ext = pos < filename.size() ? filename.substr(pos) : "";
    if (ext == ".c" || ext == ".cc" || ext == ".cpp") {
        std::string text = readfile(filename);
        coc_parser parser(text);
        push_strtab(parser.strtab());
        for (auto object : parser.objects()) {
            block_builder builder(&object, m_macro);
            push_strtab(builder.strtab());
            builder.dumpfile(m_output);
        }
    }
}

void builder::push_strtab(const std::vector<std::string> &list)
{
    for (auto s : list)
        m_strmap[s] = 0;
}

std::string builder::readfile(const std::string &filename)
{
    std::ifstream in(filename);
    std::ostringstream tmp;
    tmp << in.rdbuf();
    return tmp.str();
}

#ifndef _MSC_VER
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#else
#include <windows.h>
#include <direct.h>
#endif

#ifndef _MSC_VER
void builder::scandir(const std::string &srcpath)
{
    DIR *dp;
    struct dirent *ep;
    dp = opendir(srcpath.data());
    if (dp != NULL) {
        while ((ep = readdir(dp)) != NULL) {
            std::string fname(ep->d_name);
            parse_all(srcpath + "/" + fname);
        }
        closedir(dp);
    }
}
#else
void builder::scandir(const std::string &srcpath)
{
    HANDLE find;
    WIN32_FIND_DATA data;
    find = FindFirstFile((srcpath + "/*").data(), &data);
    if (find != INVALID_HANDLE_VALUE) {
        do {
            std::string fname(data.cFileName);
            parse_all(srcpath + "/" + fname);
        } while (FindNextFile(find, &data) != 0);
        FindClose(find);
    }
}
#endif

void builder::build()
{
    for (auto it : m_input) {
        scandir(it);
    }
    str_build sb(m_strmap);
    sb.build(m_output);
}

builder::builder(int argc, char **argv)
{
    m_state = Input;
    for (int i = 1; i < argc; ++i) {
        add_arg(argv[i]);
    }
    m_macro = new macro_table();
    for (auto it : m_config) {
        m_macro->scan_file(it);
    }
}

builder::~builder()
{
    delete m_macro;
}

void builder::add_arg(const std::string &arg)
{
    if (arg == "-i") {
        m_state = Input;
    } else if (arg == "-o") {
        m_state = Output;
    } else if (arg == "-c") {
        m_state = Config;
    } else {
        switch (m_state) {
        case Output:
            m_output = arg;
            break;
        case Config:
            m_config.push_back(arg);
            break;
        case Input:
        default:
            m_input.push_back(arg);
            break;
        }
        m_state = Input;
    }
}

int main(int argc, char *argv[])
{
    builder arg(argc, argv);
    arg.build();
    return 0;
}
