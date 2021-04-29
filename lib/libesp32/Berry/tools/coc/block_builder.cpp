/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "block_builder.h"
#include "hash_map.h"
#include "macro_table.h"
#include "object_block.h"
#include <sstream>
#include <fstream>

static bool depend(const object_block *object, const macro_table *macro)
{
    auto it = object->attr.find("depend");
    if (it != object->attr.end()) {
        return macro->query(it->second);
    }
    return true;
}

block_builder::block_builder(const object_block *object, const macro_table *macro)
{
    m_block.name = object->name;
    if (depend(object, macro)) {
        m_block.type = object->type;
        m_block.attr = object->attr;

        auto it = object->attr.find("name");
        if (it != object->attr.end()) {
            m_strtab.push_back(it->second);
        }
        
        for (auto i : object->data) {
            if (i.second.depend.empty() || macro->query(i.second.depend)) {
                m_block.data[i.first] = i.second.value;
                m_strtab.push_back(i.first);
            }
        }
    }
}

std::string block_builder::block_tostring(const block &block)
{
    std::ostringstream ostr;
    if (block.type == "map") {
        ostr << map_tostring(block, block.name);
    } else if (block.type == "class") {
        ostr << class_tostring(block);
    } else if (block.type == "vartab") {
        ostr << vartab_tostring(block);
    } else if (block.type == "module") {
        ostr << module_tostring(block);
    }
    return ostr.str();
}

std::string block_builder::class_tostring(const block &block)
{
    bool empty_map = block.data.empty();
    std::ostringstream ostr;
    hash_map map(block.data);
    std::string map_name(block.name + "_map");

    if (!empty_map) {
        ostr << map_tostring(block, map_name, true) << std::endl;
    }
    ostr << scope(block) << " be_define_const_class(\n    "
         << block.name << ",\n    "
         << map.var_count() << ",\n    "
         << super(block) << ",\n    "
         << name(block) << "\n"
            ");" << std::endl;
    return ostr.str();
}

std::string block_builder::map_tostring(const block &block, const std::string &name, bool local)
{
    std::ostringstream ostr;
    hash_map map(block.data);

    hash_map::entry_table list = map.entry_list();
    ostr << "static be_define_const_map_slots(" << name << ") {\n";
    for (auto it : list) {
        ostr << "    { be_const_key(" << it.key << ", "
             << it.next << "), " << it.value << " }," << std::endl;
    }
    ostr << "};\n\n";

    ostr << (local ? "static" : scope(block))
         << " be_define_const_map(\n    "
         << name << ",\n    "
         << list.size() << "\n"
            ");" << std::endl;
    return ostr.str();
}

std::string block_builder::vartab_tostring(const block &block)
{
    std::ostringstream ostr;
    struct block idxblk;
    std::vector<std::string> varvec;
    int index = 0;

    idxblk = block;
    idxblk.data.clear();
    for (auto it : block.data) {
        varvec.push_back(it.second);
        it.second = "int(" + std::to_string(index++) + ")";
        idxblk.data.insert(it);
    }

    ostr << map_tostring(idxblk, block.name + "_map", true) << std::endl;
    ostr << "static const bvalue __vlist_array[] = {\n";
    for (auto it : varvec) {
        ostr << "    be_const_" << it << "," << std::endl;
    }
    ostr << "};\n\n";

    ostr << "static be_define_const_vector(\n    "
         << block.name << "_vector,\n    "
            "__vlist_array,\n    "
         << varvec.size() << "\n"
            ");" << std::endl;
    return ostr.str();
}

std::string block_builder::module_tostring(const block &block)
{
    std::ostringstream ostr;
    std::string name("m_lib" + block.name);
    std::string map_name(name + "_map");

    ostr << map_tostring(block, map_name, true) << std::endl
         << "static be_define_const_module(\n    "
         << name << ",\n    "
            "\"" << block.name << "\"\n"
            ");" << std::endl;
    std::string scp = scope(block);
    if (scp != "static") { /* extern */
        ostr << "\n" << scp
             << " be_define_const_native_module("
             << block.name << ", "
             << init(block) << ");" << std::endl;
    }
    return ostr.str();
}

std::string block_builder::scope(const block &block)
{
    auto it = block.attr.find("scope");
    return it != block.attr.end() && it->second == "local" ?
        "static" : "BE_EXPORT_VARIABLE";
}

std::string block_builder::super(const block &block)
{
    auto it = block.attr.find("super");
    return it == block.attr.end() ? "NULL" : "(bclass *)&" + it->second;
}

std::string block_builder::name(const block &block)
{
    auto it = block.attr.find("name");
    return it == block.attr.end() ? block.name : it->second;
}

std::string block_builder::init(const block &block)
{
    auto it = block.attr.find("init");
    return it == block.attr.end() ? "NULL" : it->second;
}

void block_builder::writefile(const std::string &filename, const std::string &text)
{
    std::string pathname(filename);
    std::string otext("#include \"be_constobj.h\"\n\n" + text);

    std::ostringstream buf;
    std::ifstream fin(pathname);
    buf << fin.rdbuf();
    if (buf.str() != otext) {
        std::ofstream fout;
        fout.open(pathname, std::ios::out);
        fout << otext;
        fout.close();
    }
}

void block_builder::dumpfile(const std::string &path)
{
    std::string s = block_tostring(m_block);
    auto it = m_block.attr.find("file");
    std::string &name = it != m_block.attr.end() ? it->second : m_block.name;
    writefile(path + "/be_fixed_" + name + ".h", s);
}

const std::vector<std::string>& block_builder::strtab() const
{
    return m_strtab;
}
