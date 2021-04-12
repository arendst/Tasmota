/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#ifndef __HASH_MAP
#define __HASH_MAP

#include <string>
#include <vector>
#include <map>

#define NODE_EMPTY      -2
#define NODE_NULL       -1

class hash_map {
public:
    struct entry {
        std::string key;
        std::string value;
        int next = NODE_EMPTY;
    };
    typedef std::vector<hash_map::entry> entry_table;

    hash_map();
    hash_map(std::map<std::string, std::string> map);
    ~hash_map();
    void insert(const std::string &key, const std::string value);
    hash_map::entry find(const std::string &key);
    entry_table entry_list();
    int var_count();
    
private:
    int nextfree();
    hash_map::entry* findprev(entry *list, entry *slot);
    void resize(size_t size);
    void insert_p(const std::string &key, const std::string value);
    hash_map::entry entry_modify(entry entry, int *var_count);

private:
    size_t m_count = 0, m_lastfree = 0;
    entry_table m_bucket;
};

#endif // !__HASH_MAP
