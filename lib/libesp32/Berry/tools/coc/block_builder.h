/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#ifndef __BLOCK_BUILDER_H
#define __BLOCK_BUILDER_H

#include <map>
#include <vector>
#include <string>
#include "object_block.h"

class macro_table;
class object_block;

class block_builder {
public:
    block_builder(const object_block *object, const macro_table *macro);
    void dumpfile(const std::string &path);
    const std::vector<std::string>& strtab() const;

private:
    struct block {
        std::string type;
        std::string name;
        std::map<std::string, std::string> attr;
        std::map<std::string, std::string> data;
    };

    std::string block_tostring(const block &block);
    std::string class_tostring(const block &block);
    std::string vartab_tostring(const block &block);
    std::string module_tostring(const block &block);
    std::string map_tostring(const block &block, const std::string &name, bool local = false);
    std::string scope(const block &block);
    std::string super(const block &block);
    std::string name(const block &block);
    std::string init(const block &block);
    void writefile(const std::string &filename, const std::string &text);

private:
    block m_block;
    std::vector<std::string> m_strtab;
};

#endif // !__BLOCK_BUILDER_H
