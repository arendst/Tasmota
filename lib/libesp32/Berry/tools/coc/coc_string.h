/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#ifndef __COC_STRING_H
#define __COC_STRING_H

#include <string>

namespace coc {
    uint32_t hashcode(const std::string &string);
    std::string escape_operator(const std::string &string);
}

#endif
