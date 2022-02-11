#ifndef BTREE_H
#define BTREE_H

#include <vector>
#include <iostream>
#include <cstdlib>
#include <iomanip>

/***********************************************
* Author         :Brandon Kieng                *
* Assignment     :BTree                        *
* CLASS          :CS 008          		       *
* SECTION        :MWR 1:00PM-3:05PM            *
* Due Date       :11/9/2020                    *
*************************************************/

template <typename T>
class BTree
{
public:
    BTree(bool dups = false);
    //big three:
    BTree(const BTree<T>& other);
    ~BTree();
    BTree<T>& operator =(const BTree<T>& RHS);

    void insert(const T& entry);                //insert entry into the tree
    void remove(const T& entry);                //remove entry from the tree

    void clear_tree();                          //clear this object (delete all nodes etc.)
    void copy_tree(const BTree<T>& other);      //copy other into this object

    bool contains(const T& entry);              //true if entry can be found in the array
    T& get(const T& entry);                     //return a reference to entry in the tree
    T* find(const T& entry);                    //return a pointer to this key. NULL if not there.

    int size() const;                           //count the number of elements in the tree
    bool empty() const;                         //true if the tree is empty

    bool is_valid();

    void print_tree(int level = 0, std::ostream& outs = cout) const; //print a readable version of the tree
    friend std::ostream& operator<<(std::ostream& outs, const BTree<T>& print_me) {
        print_me.print_tree(0, outs);
        return outs;
    }

    /*************************************************
    *TEST FUNCTIONS
    *************************************************/
    void construct_sample();

    //big three test functions
    void test_sample_copy_constructor();
    void test_sample_destructor();
    void test_sample_assignment();

    //primary test functions
    void test_sample_print();
    void test_sample_insert();
    void test_sample_copy();
    void test_sample_clear();
    void test_sample_remove();

    //contains, find, and get test functions
    void test_sample_contains();
    void test_sample_find();
    void test_sample_get();

    //array function tests
    void test_sample_maximal();
    void test_sample_swap();
    void test_sample_index_of_maximal();
    void test_sample_ordered_insert();
    void test_sample_first_ge();
    void test_sample_attach_item();
    void test_sample_insert_item();
    void test_sample_detach_item();
    void test_sample_delete_item();
    void test_sample_merge();
    void test_sample_split();
    void test_sample_copy_array();
    void test_sample_print_array();
    void test_sample_is_gt();
    void test_sample_is_le();

    //test rotating functions
    void test_rotate_right();
    void test_rotate_left();

private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool dups_ok;                                   //true if duplicate keys may be inserted
    int data_count = 0;                             //number of data elements
    T data[MAXIMUM + 1];                            //holds the keys
    int child_count = 0;                            //number of children
    BTree* subset[MAXIMUM + 2];                     //subtrees

    bool is_leaf() const { return child_count == 0; }   //true if this is a leaf node

    //insert element functions
    void loose_insert(const T& entry);              //allows MAXIMUM+1 data elements in the root
    void fix_excess(int i);                         //fix excess of data elements in child i

    //remove element functions:
    void loose_remove(const T& entry);              //allows MINIMUM-1 data elements in the root
    void fix_shortage(int i);                       //fix shortage of data elements in child i

    void remove_biggest(T& entry);                  //remove the biggest child of this tree->entry
    void rotate_left(int i);                        //transfer one element LEFT from child i
    void rotate_right(int i);                       //transfer one element RIGHT from child i
    void merge_with_next_subset(int i);             //merge subset i with subset i+1

    T& get_existing(const T& entry);
};


template <class T>
T maximal(const T& a, const T& b);                      //return the larger of the two items

template <class T>
void swap(T& a, T& b);                                  //swap the two items

template <class T>
int index_of_maximal(T data[], int n);                 //return index of the largest item in data

template <class T>
void ordered_insert(T data[], int& n, T entry);        //insert entry into the sorted array
                                                        //data with length n

template <class T>
int first_ge(const T data[], int n, const T& entry);   //return the first element in data that is
                                                        //not less than entry

template <class T>
void attach_item(T data[], int& n, const T& entry);    //append entry to the right of data

template <class T>
void insert_item(T data[], int i, int& n, T entry);    //insert entry at index i in data

template <class T>
void detach_item(T data[], int& n, T& entry);          //remove the last element in data and place
                                                        //it in entry

template <class T>
void delete_item(T data[], int i, int& n, T& entry);   //delete item at index i and place it in entry

template <class T>
void merge(T data1[], int& n1, T data2[], int& n2);   //append data2 to the right of data1

template <class T>
void split(T data1[], int& n1, T data2[], int& n2);   //move n/2 elements from the right of data1
                                                        //and move to data2

template <class T>
void copy_array(T dest[], const T src[],
    int& dest_size, int src_size);              //copy src[] into dest[]

template <class T>
void print_array(const T data[], int n, int pos = -1);  //print array data

template <class T>
bool is_gt(const T data[], int n, const T& item);       //item > all data[i]

template <class T>
bool is_le(const T data[], int n, const T& item);       //item <= all data[i]

//-------------- Vector Extra operators: ---------------------

template <typename T>
std::ostream& operator <<(std::ostream& outs, const std::vector<T>& list); //print vector list

template <typename T>
std::vector<T>& operator +=(std::vector<T>& list, const T& addme); //list.push_back addme

template <typename T>
std::vector<T>& operator +=(std::vector<T>& list, const std::vector<T> addme);

#endif