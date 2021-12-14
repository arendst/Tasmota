/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "hash_map.h"
#include "coc_string.h"

#define is_empty(entry) ((entry).next == NODE_EMPTY)

hash_map::hash_map()
{
    resize(2);
}

hash_map::hash_map(std::map<std::string, std::string> map)
{
    resize(2);
    for (auto it : map) {
        insert(it.first, it.second);
    }
}

hash_map::~hash_map()
{
}

void hash_map::resize(size_t size)
{
    entry_table bucket = m_bucket;
    m_bucket.resize(size);
    /* set all slot to empty */
    for (int i = 0; i < size; ++i) {
        m_bucket[i].next = NODE_EMPTY;
    }
    m_lastfree = size - 1;
    for (auto slot : bucket) {
        if (!is_empty(slot)) {
            insert_p(slot.key, slot.value);
        }
    }
}

hash_map::entry* hash_map::findprev(entry *list, entry *slot)
{
    int next;
    entry *prev = list;
    for (;;) {
        next = prev->next;
        if (next == NODE_NULL || &m_bucket[next] == slot) {
            break;
        }
        prev = &m_bucket[next];
    }
    if (next == NODE_NULL) {
        return NULL;
    }
    return prev;
}

int hash_map::nextfree()
{
    while (m_lastfree >= 0) {
        if (is_empty(m_bucket[m_lastfree])) {
            return int(m_lastfree);
        }
        --m_lastfree;
    }
    return -1;
}

hash_map::entry hash_map::find(const std::string &key)
{
    uint32_t hash = coc::hashcode(key);
    entry null, *slot = &m_bucket[hash % m_bucket.size()];
    if (is_empty(*slot)) {
        return null;
    }
    while (slot->key != key) {
        if (slot->next == NODE_NULL) {
            return null;
        }
        slot = &m_bucket[slot->next];
    }
    return *slot;
}

void hash_map::insert_p(const std::string &key, const std::string value)
{
    entry *slot = &m_bucket[coc::hashcode(key) % m_bucket.size()];
    if (is_empty(*slot)) { /* empty slot */
        slot->next = NODE_NULL;
    } else {
        int newidx = nextfree();
        /* get the main-slot index */
        entry *mainslot = &m_bucket[coc::hashcode(slot->key) % m_bucket.size()];
        entry *newslot = &m_bucket[newidx]; /* get a free slot index */
        if (mainslot == slot) { /* old is main slot */
            newslot->next = mainslot->next;
            mainslot->next = newidx;
            slot = newslot;
        } else { /* link to list */
            entry *prev = findprev(mainslot, slot);
            prev->next = newidx; /* link the previous node */
            *newslot = *slot; /* copy to new slot */
            slot->next = NODE_NULL;
        }
    }
    slot->key = key;
    slot->value = value;
}

void hash_map::insert(const std::string &key, const std::string value)
{
    entry slot = find(key);
    if (slot.next == NODE_EMPTY) { /* new entry */
        if (m_count >= m_bucket.size()) {
            resize(m_bucket.size() * 2);
        }
        insert_p(key, value);
        ++m_count;
    }
}

hash_map::entry hash_map::entry_modify(entry entry, int *var_count)
{
    entry.key = coc::escape_operator(entry.key);
    if (entry.value == "var") {
        entry.value = "be_const_var("
                + std::to_string(*var_count) + ")";
        ++(*var_count);
    } else {
        entry.value = "be_const_" + entry.value;
    }
    return entry;
}

hash_map::entry_table hash_map::entry_list()
{
    entry_table list;
    int var_count = 0;

    resize(m_count);
    for (auto it : m_bucket) {
        list.push_back(entry_modify(it, &var_count));
    }
    return list;
}

int hash_map::var_count()
{
    int count = 0;

    resize(m_count);
    for (auto it : m_bucket) {
        count += it.value == "var" ? 1 : 0;
    }
    return count;
}
