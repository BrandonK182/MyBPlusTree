#ifndef MAP_H
#define MAP_H

#include "../BPlusTree/BPlusTree.h"
#include <vector>


template <typename K, typename V>
struct Pair {
	K key;
	V value;
	
	Pair(const K& k = K(), const V& v = V())
	{
		key = k;
		value = v;
	};
	friend std::ostream& operator <<(std::ostream& outs, const Pair<K, V>& print_me){
		outs << "KEY: " << print_me.key <<" , VALUES: " << print_me.value << std::endl;
		return outs;
	}
	friend bool operator ==(const Pair<K, V>& lhs, const Pair<K, V>& rhs)
	{
		if (lhs.key == rhs.key)
			return true;
		return false;
	}
	friend bool operator !=(const Pair<K, V>& lhs, const Pair<K, V>& rhs)
	{
		if (lhs.key != rhs.key)
			return true;
		return false;
	}
	friend bool operator < (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
	{
		if (lhs.key < rhs.key)
			return true;
		return false;
	}
	friend bool operator > (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
	{
		if (lhs.key > rhs.key)
			return true;
		return false;
	}
	friend bool operator <= (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
	{
		if (lhs.key <= rhs.key)
			return true;
		return false;
	}
	friend bool operator >= (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
	{
		if (lhs.key >= rhs.key)
			return true;
		return false;
	}
	friend Pair<K, V> operator + (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
	{
		return Pair(lhs.key, lhs.value + rhs.value);
	}
	
};

template <typename K, typename V>
class Map
{
public:
	typedef BPlusTree<Pair<K, V> > map_base;
	class Iterator {
	public:
		friend class Map;
		Iterator(typename map_base::Iterator it)
		{
			_it = it;
		}
		Iterator operator ++(int unused)
		{
			_it++;
		}
		Iterator operator ++()
		{
			_it++;
		}
		Pair<K, V> operator *()
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
			if (lhs._it != rhs._it)
				return true;
			return false;
		}
	private:
		typename map_base::Iterator _it;
	};

	Map();

	//  Iterators
	Iterator begin();
	Iterator end();

	//  Capacity
	int size() const;
	bool empty() const;

	//  Element Access
	V& operator[](const K& key);
	const V& operator[](const K& key) const;
	V& at(const K& key);
	const V& at(const K& key) const;


	//  Modifiers
	void insert(const K& k, const V& v);
	void erase(const K& key);
	void clear();
	V get(const K& key);

	//  Operations:
	Iterator find(const K& key);
	bool empty();
	bool contains(const Pair<K, V>& target) const;

	int count(const K& key);
	Iterator lower_bound();
	Iterator upper_bound();
	//    equal_range:

	bool is_valid() { return map.is_valid(); }

	friend std::ostream& operator<<(std::ostream& outs, const Map<K, V>& print_me) {
		outs << print_me.map << std::endl;
		return outs;
	}

private:
	int key_count;
	BPlusTree<Pair<K, V>> map;
};
#endif