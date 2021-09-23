/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#ifndef __OBJECT_BLOCK_H
#define __OBJECT_BLOCK_H

#include <map>
#include <string>

struct object_block {
    struct data_value {
        std::string value;
        std::string depend;
    };
    std::string type;
    std::string name;
    std::map<std::string, std::string> attr;
    std::map<std::string, data_value> data;
    std::vector<std::string> data_ordered;  /* preserve order of keys */
};

#endif
