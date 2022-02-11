#ifndef MMAP_CPP
#define MMAP_CPP
#include "mmap.h"

/************************************************
 * MMap()
 * default constructor that doesn't do anything
 * parameter: none
 * return: none
 * *******************************************/
template<typename K, typename V>
MMap<K, V>::MMap()
{
	mmap.empty();
}

/************************************************
 * Iterator MMap<K,V>::begin()
 * function that returns iterator pointing
 * to the left most leaf
 * parameter: none
 * return: Iterator pointing to left
 * *******************************************/
template<typename K, typename V>
typename MMap<K, V>::Iterator MMap<K,V>::begin()
{
	return mmap.begin();
}

/************************************************
 * Iterator MMap<K,V>::end()
 * function that returns iterator pointing
 * to a null pointer
 * parameter: none
 * return: none
 * *******************************************/
template<typename K, typename V>
typename MMap<K, V>::Iterator MMap<K, V>::end()
{
	return mmap.end();
}

/************************************************
 * int MMap<K,V>::size()
 * function that returns map's size
 * parameter: none
 * return: int size
 * *******************************************/
template<typename K, typename V>
int MMap<K, V>::size() const
{
	return mmap.size();
}

/************************************************
 * int MMap<K,V>::empty()
 * function that returns true if the map is empty
 * parameter: none
 * return: if map is empty
 * *******************************************/
template<typename K, typename V>
bool MMap<K, V>::empty() const
{
	return mmap.empty();
}

/************************************************
 * V& MMap<K,V>::operator[] (const K& key)
 * function that overload the bracket operator
 * and gets values
 * parameter: K key to match
 * return: value matching key
 * *******************************************/
template<typename K, typename V>
std::vector<V>& MMap<K, V>::operator[](const K& key)
{
	MPair<K, V> temp(key);
	return mmap.get(temp).value_list;
}


/************************************************
 * const V& MMap<K,V>::operator[] (const K& key) const
 * function that overload the bracket operator
 * and gets values
 * parameter: K key to match
 * return: value matching key
 * *******************************************/
template<typename K, typename V>
const std::vector<V>& MMap<K, V>::operator[](const K& key) const
{
	MPair<K, V> temp(key);
	assert(mmap.contains(temp) && "MMAP DOES NOT HAVE THE KEY");
	return mmap.get(temp).value_list;
}

/************************************************
 * void MMap<K, V>::insert(const K& k, const V& v)
 * function that inserts a key and value as a pair
 * into map
 * parameter: K key to insert
 *		      V value to insert
 * return: none
 * *******************************************/
template<typename K, typename V>
void MMap<K, V>::insert(const K& k, const V& v)
{
	MPair<K, V> temp(k, v);
	mmap.insert(temp);
}

/************************************************
 * void MMap<K, V>::erase(const K& key)
 * function that removes an entry with a given
 * key
 * parameter: K key to match
 * return: none
 * *******************************************/
template<typename K, typename V>
void MMap<K, V>::erase(const K& key)
{
	if (contains(key))
	{
		MPair<K, V> temp(key);
		mmap.remove(temp);
	}
}

/************************************************
 * void MMap<K, V>::clear()
 * function that clears map
 * parameter: K key to match
 * return: none
 * *******************************************/
template<typename K, typename V>
void MMap<K, V>::clear()
{
	mmap.clear_tree();
}

/************************************************
 * V MMap<K,V>::get(const K& key)
 * function that gets a data entry
 * parameter: K key to match
 * return: value with given key
 * *******************************************/
template<typename K, typename V>
std::vector<V>& MMap<K, V>::get(const K& key)
{
	MPair<K, V> temp(key);
	return mmap.get(temp).value;
}

/************************************************
 * Iterator MMap<K, V>::find(const K& key)
 * function that returns iterator of an entry with
 * a matching key
 * parameter: K key to match
 * return: iterator pointing to entry
 * *******************************************/
template<typename K, typename V>
typename MMap<K, V>::Iterator MMap<K, V>::find(const K& key)
{
	MPair<K, V> temp(key);
	Iterator it(mmap.find(temp));
	return it;
}

/************************************************
 * bool MMap<K, V>::contains(const Pair<K, V>& target) const
 * function that returns true if the map has pair
 * parameter: none
 * return: true if has pair
 * *******************************************/
template<typename K, typename V>
bool MMap<K, V>::contains(const K& key) const
{
	MPair<K, V> pair(key);
	if (mmap.contains(pair))
		return true;
	return false;
}

/************************************************
 * bool MMap<K, V>::empty()
 * function that returns true if the map is empty
 * parameter: none
 * return: true if empty
 * *******************************************/
template<typename K, typename V>
bool MMap<K, V>::empty()
{
	if (mmap.empty())
		return true;
	return false;
}

/************************************************
 * int MMap<K, V>::count(const K& key)
 * function that returns amount of values with a key
 * parameter: none
 * return: amount with a key
 * *******************************************/
template<typename K, typename V>
int MMap<K, V>::count(const K& key)
{
	if (contains(key))
	{
		MPair<K, V> temp(key);
		return mmap.get(temp).value_list.size();
	}
	return 0;
}

/************************************************
 * Iterator MMap<K, V>::lower_bound()
 * function that returns Iterator at lower end of key
 * parameter: none
 * return: iterator
 * *******************************************/
template<typename K, typename V>
typename MMap<K, V>::Iterator MMap<K, V>::lower_bound(const K& key)
{
	return Iterator(mmap.lower_bound(key));
}

/************************************************
 * Iterator MMap<K, V>::upper_bound()
 * function that returns Iterator at upper end of key
 * parameter: none
 * return: iterator
 * *******************************************/
template<typename K, typename V>
typename MMap<K, V>::Iterator MMap<K, V>::upper_bound(const K& key)
{
	return Iterator(mmap.upper_bound(key));
}

#endif

/*
	MMap<std::string, int> m;
	std::cout << "read key G: " << m["G"] << std::endl;

	//reading a non_existent key will crash without warning.
	//  we will need to deal with this.
	//  perhaps the operator that will take m["Z"] 
	//  (operator << in this example) can verify that 
	//  it is not an empty vector. 
	//
	//cout<<"read non_existent key Z: "<<m["Z"]<<endl;

	std::cout << "add 99 to the non_existent key Z: ";
	m["Z"] += 99;                     //returns a reference to an empty 
									//  vector to which we are pushing back
									//  the value 99
									//could also use m["Z"].push_back(99);
	std::cout << m["Z"];

	//or simply;
	//cout<<"add 99 to the non_existent key Z: "
	//    <<(m["Z"]+=99)<<endl; //could also push_back
*/