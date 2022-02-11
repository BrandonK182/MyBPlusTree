#ifndef MMAP_H
#define MMAP_H

#include "../BPlusTree/BPlusTree.h"
#include "../BPlusTree/BPlusTree.cpp"
#include <vector>
#include <assert.h>

template <typename K, typename V>
struct MPair {
    K key;
    std::vector<V> value_list;

    //--------------------------------------------------------------------------------
    /*      MPair CTORs:
     *  must have these CTORs:
     *  - default CTOR / CTOR with a key and no value: this will create an empty vector
     *  - CTOR with a key AND a value: pushes the value into the value_list
     *  _ CTOR with a key and a vector of values (to replace value_list)
     */
     //--------------------------------------------------------------------------------
    MPair(const K& k = K())
    {
        key = k;
    }
    MPair(const K& k, const V& v)
    {
        key = k;
        value_list.push_back(v);
    }
    MPair(const K& k, const std::vector<V>& vlist)
    {
        key = k;
        value_list = vlist;
    }
    //--------------------------------------------------------------------------------

    //You'll need to overlod << for your vector:
    friend std::ostream& operator <<(std::ostream& outs, const MPair<K, V>& print_me)
    {
        outs << "KEY: " << print_me.key << ", ";
        if (print_me.value_list.size() > 0)
            outs << "VALUES: " << print_me.value_list << std::endl;
        else
            outs << "VALUES LIST IS EMPTY" << std::endl;
        return outs;
    }

    friend bool operator ==(const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        if (lhs.key == rhs.key)
            return true;
        return false;
    }
    friend bool operator !=(const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        if (lhs.key != rhs.key)
            return true;
        return false;
    }
    friend bool operator < (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        if (lhs.key < rhs.key)
            return true;
        return false;
    }
    friend bool operator <= (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        if (lhs.key <= rhs.key)
            return true;
        return false;
    }
    friend bool operator > (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        if (lhs.key > rhs.key)
            return true;
        return false;
    }
    friend bool operator >= (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        if (lhs.key >= rhs.key)
            return true;
        return false;
    }
    friend MPair<K, V> operator + (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        std::vector<V> alist;
        for (int i = 0; i < lhs.value_list.size(); i++)
        {
            alist.push_back(lhs.value_list[i]);
        }
        for (int i = 0; i < rhs.value_list.size(); i++)
        {
            alist.push_back(rhs.value_list[i]);
        }
        return MPair<K, V>(lhs.key, alist);
    }
};

template <typename K, typename V>
class MMap
{
public:
    typedef BPlusTree<MPair<K, V> > map_base;
    class Iterator {
    public:
        friend class MMap;
        Iterator(typename map_base::Iterator it)
        {
            _it = it;
        }
        Iterator operator ++(int unused)
        {
            _it++;
            return *this;
        }
        Iterator operator ++()
        {
            _it++;
            return *this;
        }
        MPair<K, V> operator *()
        {
            return *_it;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs)
        {
            if (lhs._it == rhs._it)
                return true;
            return false;
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs)
        {
            if (lhs._it == rhs._it)
                return false;
            return true;
        }

    private:
        typename map_base::Iterator _it;
    };

    MMap();

    //  Iterators
    Iterator begin();
    Iterator end();

    //  Capacity
    int size() const;
    bool empty() const;

    //  Element Access
    const std::vector<V>& operator[](const K& key) const;
    std::vector<V>& operator[](const K& key);

    //  Modifiers
    void insert(const K& k, const V& v);
    void erase(const K& key);
    void clear();

    //  Operations:
    bool contains(const K& key) const;
    std::vector<V>& get(const K& key);
    bool empty();

    Iterator find(const K& key);
    int count(const K& key);
    Iterator lower_bound(const K& key);
    Iterator upper_bound(const K& key);
    //    equal_range:



    bool is_valid() { return mmap.is_valid(); }

    friend std::ostream& operator<<(std::ostream& outs, const MMap<K, V>& print_me) {
        outs << print_me.mmap << std::endl;
        return outs;
    }

    void print_list() { mmap.print_list(); }

private:
    BPlusTree<MPair<K, V> > mmap;
};

#endif