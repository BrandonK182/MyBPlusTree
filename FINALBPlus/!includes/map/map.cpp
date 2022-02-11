#ifndef MAP_CPP
#define MAP_CPP
#include "map.h"


/************************************************
 * Map()
 * default constructor that doesn't do anything
 * parameter: none
 * return: none
 * *******************************************/
template<typename K, typename V>
Map<K, V>::Map()
{
	
}

/************************************************
 * Iterator Map<K,V>::begin()
 * function that returns iterator pointing
 * to the left most leaf
 * parameter: none
 * return: Iterator pointing to left
 * *******************************************/
template<typename K, typename V>
typename Map<K, V>::Iterator Map<K,V>::begin()
{
	return map.begin();
}

/************************************************
 * Iterator Map<K,V>::end()
 * function that returns iterator pointing
 * to a null pointer
 * parameter: none
 * return: none
 * *******************************************/
template<typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::end()
{
	return map.end();
}

/************************************************
 * int Map<K,V>::size()
 * function that returns map's size
 * parameter: none
 * return: int size
 * *******************************************/
template<typename K, typename V>
int Map<K, V>::size() const
{
	return map.size();
}

/************************************************
 * int Map<K,V>::empty()
 * function that returns true if the map is empty
 * parameter: none
 * return: if map is empty
 * *******************************************/
template<typename K, typename V>
bool Map<K, V>::empty() const
{
	return map.empty();
}

/************************************************
 * V& Map<K,V>::operator[] (const K& key)
 * function that overload the bracket operator
 * and gets values
 * parameter: K key to match
 * return: value matching key
 * *******************************************/
template<typename K, typename V>
V& Map<K,V>::operator[] (const K& key)
{
	return at(key);
}

/************************************************
 * const V& Map<K,V>::operator[] (const K& key) const
 * function that overload the bracket operator
 * and gets values
 * parameter: K key to match 
 * return: value matching key
 * *******************************************/
template<typename K, typename V>
const V& Map<K, V>::operator[] (const K & key) const
{
	return at(key);
}

/************************************************
 * V& Map<K, V>::at(const K& key)
 * function that gets values
 * parameter: K key to match
 * return: value matching key
 * *******************************************/
template<typename K, typename V>
V& Map<K, V>::at(const K& key)
{
	Pair<K, V> temp(key);
	return map.get(temp).value;
}

/************************************************
 * const V& Map<K, V>::at(const K& key) const
 * function that gets values
 * parameter: K key to match
 * return: value matching key
 * *******************************************/
template<typename K, typename V>
const V& Map<K, V>::at(const K& key) const
{
	Pair<K, V> temp(key);
	return map.get(temp).value;
}

/************************************************
 * void Map<K, V>::insert(const K& k, const V& v)
 * function that inserts a key and value as a pair
 * into map
 * parameter: K key to insert
 *		      V value to insert
 * return: none
 * *******************************************/
template<typename K, typename V>
void Map<K, V>::insert(const K& k, const V& v)
{
	Pair<K, V> temp(k, v);
	map.insert(temp);
}

/************************************************
 * void Map<K, V>::erase(const K& key)
 * function that removes an entry with a given
 * key
 * parameter: K key to match
 * return: none
 * *******************************************/
template<typename K, typename V>
void Map<K, V>::erase(const K& key)
{
	if (contains(key))
	{
		Pair<K, V> temp(key);
		map.remove(temp);
	}
}

/************************************************
 * void Map<K, V>::clear()
 * function that clears map
 * parameter: K key to match
 * return: none
 * *******************************************/
template<typename K, typename V>
void Map<K, V>::clear()
{
	map.clear_tree();
}

/************************************************
 * V Map<K,V>::get(const K& key)
 * function that gets a data entry
 * parameter: K key to match
 * return: value with given key
 * *******************************************/
template<typename K, typename V>
V Map<K,V>::get(const K& key)
{
	Pair<K, V> temp(key);
	return map.get(temp).value;
}

/************************************************
 * Map<K, V>::Iterator Map<K, V>::find(const K& key)
 * function that returns iterator of an entry with
 * a matching key
 * parameter: K key to match
 * return: iterator pointing to entry
 * *******************************************/
template<typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::find(const K& key)
{
	Pair<K, V> temp(key);
	Iterator it(map.find(temp));
	return it;
}

/************************************************
 * bool Map<K, V>::empty()
 * function that returns true if the map is empty
 * parameter: none
 * return: true if empty
 * *******************************************/
template<typename K, typename V>
bool Map<K, V>::empty()
{
	if (map.empty())
		return true;
	return false;
}

/************************************************
 * bool Map<K, V>::contains(const Pair<K, V>& target) const
 * function that returns true if the map has pair
 * parameter: none
 * return: true if has pair
 * *******************************************/
template<typename K, typename V>
bool Map<K, V>::contains(const Pair<K, V>& target) const
{
	if (map.contains(target))
		return true;
	return false;
}

/************************************************
 * int Map<K, V>::count(const K& key)
 * function that returns amount of values with a key
 * parameter: none
 * return: amount with a key
 * *******************************************/
template<typename K, typename V>
int Map<K, V>::count(const K& key)
{
	if (map.contains(key))
	{
		return 1;
	}
	return 0;
}

/************************************************
 * Iterator Map<K, V>::lower_bound()
 * function that returns Iterator at lower end of key
 * parameter: none
 * return: iterator
 * *******************************************/
template<typename K, typename V>
typename Map<K,V>::Iterator Map<K, V>::lower_bound()
{
	return Iterator(map.lower_bound());
}

/************************************************
 * Iterator Map<K, V>::upper_bound()
 * function that returns Iterator at upper end of key
 * parameter: none
 * return: iterator
 * *******************************************/
template<typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::upper_bound()
{
	return Iterator(map.upper_bound());
}

#endif